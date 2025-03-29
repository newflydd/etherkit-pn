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
/*  F i l e               &F: ertec200.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  ERTEC200 register structure                                              */
/*                                                                           */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*                                                                           */
/*  V1.0 - first implementation                                              */
/*                                                                           */
/*---------------------------------------------------------------------------*/


#ifndef ERTEC200_H
#define ERTEC200_H


/*  BASE address of the hardware in the ERTEC ASIC. */
#define IRTE_BASE           0x10000000
#define EMIF_SDRAM_BASE     0x20000000
#define EMIF_MEM_BASE       0x30000000
#define BOOT_ROM_BASE       0x40000000
#define TIMER_BASE          0x40002000
#define WD_BASE             0x40002100
#define SPI1_BASE           0x40002200
#define UART1_BASE          0x40002300
#define SSC_BASE            0x40002400
#define GPIO_BASE           0x40002500
#define SCRB_BASE           0x40002600      // System-Control-Register-Block
#define FCOUNT_BASE         0x40002700      // F-Counter
#define ESMA2_BASE          0x40010000
#define ARM_ICU_BASE        0x50000000
#define EMIF_REG_BASE       0x70000000
#define DMA_REG_BASE        0x80000000

#define ERTEC200_ITRE       (volatile ERTEC200_IO_IRTE *)IRTE_BASE
#define ERTEC200_TIMER      (volatile ERTEC200_IO_TIMER *)TIMER_BASE
#define ERTEC200_WD         (volatile ERTEC200_IO_WD *)WD_BASE
#define ERTEC200_SPI1       (volatile ERTEC200_IO_SPI1 *)SPI1_BASE
#define ERTEC200_UART1      (volatile ERTEC200_IO_UART1 *)UART1_BASE
#define ERTEC200_SSC        (volatile ERTEC200_IO_SSC *)SSC_BASE
#define ERTEC200_GPIO       (volatile ERTEC200_IO_GPIO *)GPIO_BASE
#define ERTEC200_SCRB       (volatile ERTEC200_IO_SCRB *)SCRB_BASE
#define ERTEC200_FCOUNT     (volatile ERTEC200_IO_FCOUNT *)FCOUNT_BASE
#define ERTEC200_ESMA2      (volatile ERTEC200_IO_ESMA2 *)ESMA2_BASE
#define ERTEC200_ARM_ICU    (volatile ERTEC200_IO_ARM_ICU *)ARM_ICU_BASE
#define ERTEC200_EMIF_REG   (volatile ERTEC200_IO_EMIF_REG *)EMIF_REG_BASE
#define ERTEC200_DMA_REG    (volatile ERTEC200_IO_DMA_REG *)DMA_REG_BASE

#ifndef _LITTLE_ENDIAN /* start of "#ifndef _LITTLE_ENDIAN" clause */

/* start of BIG ENDIAN MODE.*/

/************** IRTE_BASE *******************************
definition of the registers in the EtherNet
Switch area starting @ 0x1000_0000
*************** IRTE_BASE *******************************/

typedef struct
{
    int dummy;
} ERTEC200_IO_IRTE;



/*************** TIMER_BASE ******************************
definition of the registers in the Timer
area starting @ 0x4000_2000
**************** TIMER_BASE ******************************/

typedef struct
{

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 26; /* D06:31 */
         unsigned status     : 1;  /* D05    */
         unsigned            : 2;  /* D03:04 */
         unsigned reload     : 1;  /* D02    */
         unsigned load       : 1;  /* D01    */
         unsigned run        : 1;  /* D00    */
      } bits;
   } ctrl_stat0;                   // Control / Status Register Timer 0

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 25; /* D07:31 */
         unsigned kask       : 1;  /* D06 */
         unsigned status     : 1;  /* D05 */
         unsigned            : 2;  /* D03:04 */
         unsigned reload     : 1;  /* D02 */
         unsigned load       : 1;  /* D01 */
         unsigned run        : 1;  /* D00 */
      } bits;
   } ctrl_stat1;                   // Control / Status Register Timer 1

   unsigned long reld0;               // Reload Register 0

   unsigned long reld1;               // Reload Register 1

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 28; /* D04:31 */
         unsigned load_v1    : 1;  /* D03 */
         unsigned run_v1     : 1;  /* D02 */
         unsigned load_v0    : 1;  /* D01 */
         unsigned run_v0     : 1;  /* D00 */
      } bits;
   } ctrl_prediv;                  // Control register for 2 predividers for timer 0/1

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 16; /* D16:31 */
         unsigned prediv1    : 8;  /* D08:15 */
         unsigned prediv0    : 8;  /* D00:07 */
      } bits;
   } reld_prediv;                  // Reload register for the 2 predividers

   unsigned long tim0;                // Timer Register 0

   unsigned long tim1;                // Timer Register 1

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 13; /* D19:31 */
         unsigned timer      : 1;  /* D18    */
         unsigned one_shot   : 1;  /* D17    */
         unsigned run        : 1;  /* D16    */
         unsigned reload     : 16; /* D00:15 */
      } bits;
   } timer2_ctrl;                  // Timer 2 Control Register

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 16; /* D16:31 */
         unsigned timer      : 16; /* D00:15 */
      } bits;
   } tim2;                         /* Timer 2 Register */

} ERTEC200_IO_TIMER;



/************** WD_BASE ********************************
definition of the registers in the Watchdog
area starting @ 0x4000_2100
*************** WD_BASE ********************************/

typedef struct
{

   union
   {
      unsigned long reg;
      struct
      {
         unsigned keybits    : 16; /* D16:31 */
         unsigned            : 11; /* D05:15 */
         unsigned stat_wd1   : 1;  /* D04    */
         unsigned stat_wd0   : 1;  /* D03    */
         unsigned load       : 1;  /* D02    */
         unsigned run_wd1    : 1;  /* D01    */
         unsigned run_wd0    : 1;  /* D00    */
      } bits;
   } ctrl_status;                  // Configuration and control bits for the watchdog

   union
   {
      unsigned long reg;
      struct
      {
         unsigned keybits    : 16; /* D16:31 */
         unsigned reload     : 16; /* D00:15 */
      } bits;
   } reld0_low,                    // Reload value for bits 15:0 of watchdog counter 0
     reld0_high,                   // Reload value for bits 31:16 of watchdog counter 0
     reld1_low,                    // Reload value for bits 19:4 of watchdog counter 1
     reld1_high;                   // Reload value for bits 35:20 of watchdog counter 1

   unsigned long wdog0;               // Value of the watchdog counter 0

   unsigned long wdog1;               // Value of the watchdog counter 1

} ERTEC200_IO_WD;



/************** SPI1_BASE *******************************
definition of the registers in the
SPI1 Module starting @ 0x4000_2200
*************** SPI1_BASE *******************************/

typedef struct
{

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 16; /* D16:31 */
         unsigned scr        : 8;  /* D08:15 */
         unsigned sph        : 1;  /* D07    */
         unsigned spo        : 1;  /* D06    */
         unsigned frf        : 2;  /* D04:05 */
         unsigned dss        : 4;  /* D00:03 */
      } bits;
   }sspcr0;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 25; /* D07:31 */
         unsigned sod        : 1;  /* D06    */
         unsigned ms         : 1;  /* D05    */
         unsigned sse        : 1;  /* D04    */
         unsigned lbm        : 1;  /* D03    */
         unsigned rorie      : 1;  /* D02    */
         unsigned tie        : 1;  /* D01    */
         unsigned rie        : 1;  /* D00    */
      } bits;
   }sspcr1;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 16; /* D16:31 */
         unsigned data       : 16; /* D00:15 */
      } bits;
   }sspdr;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 27; /* D05:31 */
         unsigned bsy        : 1;  /* D04    */
         unsigned rff        : 1;  /* D03    */
         unsigned rne        : 1;  /* D02    */
         unsigned tnf        : 1;  /* D01    */
         unsigned tfe        : 1;  /* D00    */
      } bits;
   }sspsr;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 24; /* D08:31 */
         unsigned cpsdvsr    : 8;  /* D00:07 */
      } bits;
   }sspcpsr;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 16; /* D16:31 */
         unsigned data       : 16; /* D00:15 */
      } write_bits;
      struct
      {
         unsigned            : 29; /* D03:31 */
         unsigned roris      : 1;  /* D02    */
         unsigned tis        : 1;  /* D01    */
         unsigned ris        : 1;  /* D00    */
      } read_bits;
   }sspiir_icr;

} ERTEC200_IO_SPI1;



/************** UART1_BASE *******************************
definition of the registers in the
UART1 Module starting @ 0x4000_2300
*************** UART1_BASE *******************************/

typedef struct
{

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 24; /* D08:31 */
         unsigned data       : 8;  /* D00:07 */
      } bits;
   }uartdr;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 24; /* D08:31 */
         unsigned data       : 8;  /* D00:07 */
      }write_bits;
      struct
      {
         unsigned            : 28; /* D04:31 */
         unsigned oe         : 1;  /* D03    */
         unsigned be         : 1;  /* D02    */
         unsigned pe         : 1;  /* D01    */
         unsigned fe         : 1;  /* D00    */
      }read_bits;
   }uartrsr_ecr;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 25; /* D07:31 */
         unsigned wlen       : 2;  /* D05:06 */
         unsigned fen        : 1;  /* D04    */
         unsigned stp2       : 1;  /* D03    */
         unsigned eps        : 1;  /* D02    */
         unsigned pen        : 1;  /* D01    */
         unsigned brk        : 1;  /* D00    */
      } bits;
   }uartlcr_h;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 24; /* D08:31 */
         unsigned baudrate   : 8;  /* D00:07 */
      } bits;
   }uartlcr_m;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 24; /* D08:31 */
         unsigned baudrate   : 8;  /* D00:07 */
      } bits;
   }uartlcr_l;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 24; /* D08:31 */
         unsigned lbe        : 1;  /* D07    */
         unsigned rtie       : 1;  /* D06    */
         unsigned tie        : 1;  /* D05    */
         unsigned rie        : 1;  /* D04    */
         unsigned msie       : 1;  /* D03    */
         unsigned sirlp      : 1;  /* D02    */
         unsigned siren      : 1;  /* D01    */
         unsigned uarten     : 1;  /* D00    */
      } bits;
   }uartcr;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 24; /* D08:31 */
         unsigned txfe       : 1;  /* D07    */
         unsigned rxff       : 1;  /* D06    */
         unsigned txff       : 1;  /* D05    */
         unsigned rxfe       : 1;  /* D04    */
         unsigned busy       : 1;  /* D03    */
         unsigned dcd        : 1;  /* D02    */
         unsigned dsr        : 1;  /* D01    */
         unsigned cts        : 1;  /* D00    */
      } bits;
   }uartfr;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 24; /* D08:31 */
         unsigned data       : 8;  /* D00:07 */
      }write_bits;
      struct
      {
         unsigned            : 28; /* D04:31 */
         unsigned rtis       : 1;  /* D03 */
         unsigned tis        : 1;  /* D02 */
         unsigned ris        : 1;  /* D01 */
         unsigned mis        : 1;  /* D00 */
      }read_bits;
   }uartiir_icr;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 24; /* D08:31 */
         unsigned ilpdvsr    : 8;  /* D00:07 */
      } bits;
   }uartilpr;

} ERTEC200_IO_UART1;



/************** SSC_BASE *******************************
definition of the registers in the
SSC Module starting @ 0x4000_2400
*************** SSC_BASE *******************************/

typedef struct
{

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 24; /* D08:31 */
         unsigned rtbuf      : 8;  /* D0:07 */
      } bits;
   } ssc_rtbuf;                    // Receive Transmit Buffer

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 24; /* D08:31 */
         unsigned busy       : 1;  /* D07 */
         unsigned            : 3;  /* D04:06 */
         unsigned recerr     : 1;  /* D03 */
         unsigned perr       : 1;  /* D02 */
         unsigned rbfu       : 1;  /* D01 */
         unsigned tbem       : 1;  /* D00 */
      } bits;
   } ssc_stat;                     // Status-Register

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 24; /* D08:31 */
         unsigned bren       : 1;  /* D07 */
         unsigned            : 1;  /* D06 */
         unsigned podd       : 1;  /* D05 */
         unsigned ppos       : 1;  /* D04 */
         unsigned pen        : 1;  /* D03 */
         unsigned hcb        : 1;  /* D02 */
         unsigned cpol       : 1;  /* D01 */
         unsigned cpha       : 1;  /* D00 */
      } bits;
   } ssc_ctrl1;                    // Control-Register 1

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 29; /* D03:31 */
         unsigned dw         : 3;  /* D00:02 */
      } bits;
   } ssc_ctrl2;                    // Control-Register 2

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 28; /* D04:31 */
         unsigned enrecerr   : 1;  /* D03 */
         unsigned enperr     : 1;  /* D02 */
         unsigned enrbfu     : 1;  /* D01 */
         unsigned entbem     : 1;  /* D00 */
      } bits;
   } ssc_int_en;                   // Interrupt-Enable-Register

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 24; /* D08:31 */
         unsigned g          : 8;  /* D00:07 */
      } bits;
   } ssc_baud;                     // Baud rate register

} ERTEC200_IO_SSC;


/************** GPIO_BASE *******************************
definition of the registers in the
GPIO Module starting @ 0x4000_2500
*************** GPIO_BASE *******************************/

typedef struct
{

   union
   {
      unsigned long reg;
      struct
      {
          unsigned char port_a_24_31;
          unsigned char port_a_16_23;
          unsigned char port_a_08_15;
          unsigned char port_a_00_07;
      }byte_access;
      struct
      {
         unsigned p31        : 1;  /* D31 */ // GPIO 31
         unsigned p30        : 1;  /* D30 */ // GPIO 30
         unsigned p29        : 1;  /* D29 */ // GPIO 29
         unsigned p28        : 1;  /* D28 */ // GPIO 28
         unsigned p27        : 1;  /* D27 */ // GPIO 27
         unsigned p26        : 1;  /* D26 */ // GPIO 26
         unsigned p25        : 1;  /* D25 */ // GPIO 25
         unsigned p24        : 1;  /* D24 */ // GPIO 24
         unsigned p23        : 1;  /* D23 */ // GPIO 23
         unsigned p22        : 1;  /* D22 */ // GPIO 22
         unsigned p21        : 1;  /* D21 */ // GPIO 21
         unsigned p20        : 1;  /* D20 */ // GPIO 20
         unsigned p19        : 1;  /* D19 */ // GPIO 19
         unsigned p18        : 1;  /* D18 */ // GPIO 18
         unsigned p17        : 1;  /* D17 */ // GPIO 17
         unsigned p16        : 1;  /* D16 */ // GPIO 16
         unsigned p15        : 1;  /* D15 */ // GPIO 15
         unsigned p14        : 1;  /* D14 */ // GPIO 14
         unsigned p13        : 1;  /* D13 */ // GPIO 13
         unsigned p12        : 1;  /* D12 */ // GPIO 12
         unsigned p11        : 1;  /* D11 */ // GPIO 11
         unsigned p10        : 1;  /* D10 */ // GPIO 10
         unsigned p09        : 1;  /* D09 */ // GPIO 9
         unsigned p08        : 1;  /* D08 */ // GPIO 8
         unsigned p07        : 1;  /* D07 */ // GPIO 7
         unsigned p06        : 1;  /* D06 */ // GPIO 6
         unsigned p05        : 1;  /* D05 */ // GPIO 5
         unsigned p04        : 1;  /* D04 */ // GPIO 4
         unsigned p03        : 1;  /* D03 */ // GPIO 3
         unsigned p02        : 1;  /* D02 */ // GPIO 2
         unsigned p01        : 1;  /* D01 */ // GPIO 1
         unsigned p00        : 1;  /* D00 */ // GPIO 0
      } pin;
   }gpio_ioctrl,                   // Configuration register for general purpose IOs
    gpio_out,                      // Output register for general purpose IOs
    gpio_in;                       // Input register for general purpose IOs

   union
   {
      unsigned long reg;
      struct
      {
         unsigned gpio_15    : 2;  /* D30:31 */
         unsigned gpio_14    : 2;  /* D28:29 */
         unsigned gpio_13    : 2;  /* D26:27 */
         unsigned gpio_12    : 2;  /* D24:25 */
         unsigned gpio_11    : 2;  /* D22:23 */
         unsigned gpio_10    : 2;  /* D20:21 */
         unsigned gpio_9     : 2;  /* D19:18 */
         unsigned gpio_8     : 2;  /* D16:17 */
         unsigned gpio_7     : 2;  /* D14:15 */
         unsigned gpio_6     : 2;  /* D12:13 */
         unsigned gpio_5     : 2;  /* D10:11 */
         unsigned gpio_4     : 2;  /* D08:09 */
         unsigned gpio_3     : 2;  /* D06:07 */
         unsigned gpio_2     : 2;  /* D04:05 */
         unsigned gpio_1     : 2;  /* D02:03 */
         unsigned gpio_0     : 2;  /* D00:01 */
      } bits;
   }gpio_port_mode_l;              // Configuration register for GPIO port

   union
   {
      unsigned long reg;
      struct
      {
         unsigned gpio_31    : 2;  /* D30:31 */
         unsigned gpio_30    : 2;  /* D28:29 */
         unsigned gpio_29    : 2;  /* D26:27 */
         unsigned gpio_28    : 2;  /* D24:25 */
         unsigned gpio_27    : 2;  /* D22:23 */
         unsigned gpio_26    : 2;  /* D20:21 */
         unsigned gpio_25    : 2;  /* D18:19 */
         unsigned gpio_24    : 2;  /* D16:17 */
         unsigned gpio_23    : 2;  /* D14:15 */
         unsigned gpio_22    : 2;  /* D12:13 */
         unsigned gpio_21    : 2;  /* D10:11 */
         unsigned gpio_20    : 2;  /* D08:09 */
         unsigned gpio_19    : 2;  /* D06:07 */
         unsigned gpio_18    : 2;  /* D04:05 */
         unsigned gpio_17    : 2;  /* D02:03 */
         unsigned gpio_16    : 2;  /* D00:01 */
      } bits;
   }gpio_port_mode_h;              // Configuration register for GPIO port

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 28; /* D04:31 */
         unsigned gpio31     : 1;  /* D03 */
         unsigned gpio30     : 1;  /* D02 */
         unsigned gpio1      : 1;  /* D01 */
         unsigned gpio0      : 1;  /* D00 */
      } bits;
   } gpio_polsel;                  // Interrupt polarity for GPIO interrupts

   unsigned long notused_1;
   unsigned long notused_2;

   union
   {
      unsigned long reg;
      struct
      {
          unsigned char port_b_unused[2];
          unsigned char port_b_08_15;
          unsigned char port_b_00_07;
      }byte_access;
      struct
      {
         unsigned            : 19; /* D13:31 */
         unsigned p44        : 1;  /* D12 */ // GPIO 44
         unsigned p43        : 1;  /* D11 */ // GPIO 43
         unsigned p42        : 1;  /* D10 */ // GPIO 42
         unsigned p41        : 1;  /* D09 */ // GPIO 41
         unsigned p40        : 1;  /* D08 */ // GPIO 40
         unsigned p39        : 1;  /* D07 */ // GPIO 39
         unsigned p38        : 1;  /* D06 */ // GPIO 38
         unsigned p37        : 1;  /* D05 */ // GPIO 37
         unsigned p36        : 1;  /* D04 */ // GPIO 36
         unsigned p35        : 1;  /* D03 */ // GPIO 35
         unsigned p34        : 1;  /* D02 */ // GPIO 34
         unsigned p33        : 1;  /* D01 */ // GPIO 33
         unsigned p32        : 1;  /* D00 */ // GPIO 32
      } bits;
   } gpio2_ioctrl,                 // Configuration register for general purpose IOs
     gpio2_out,                    // Output register for general purpose IOs
     gpio2_in;                     // Input register for general purpose IOs

} ERTEC200_IO_GPIO;


/************** SCRB_BASE *******************************
definition of the registers in the
SCRB Module starting @ 0x4000_2600
*************** SCRB_BASE *******************************/

typedef struct
{

   union
   {
      unsigned long reg;
      struct
      {
         unsigned id         : 16; /* D16:31 */
         unsigned hw_release : 8;  /* D08:15 */
         unsigned metall_fix : 8;  /* D00:07 */
      } bits;
   }id_reg;                        // Identification ERTEC200

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 28; /* D04:31 */
         unsigned p3         : 1;  /* D03 */
         unsigned p2         : 1;  /* D02 */
         unsigned p1         : 1;  /* D01 */
         unsigned p0         : 1;  /* D00 */
      } bits;
   }boot_reg;                      // Boot-Mode-Pins

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 25; /* D07:31 */
         unsigned p5         : 1;  /* D06 */
         unsigned p4         : 1;  /* D05 */
         unsigned p3         : 1;  /* D04 */
         unsigned p2         : 1;  /* D03 */
         unsigned p1         : 1;  /* D02 */
         unsigned p0         : 1;  /* D01 */
         unsigned            : 1;  /* D00 */
      } bits;
   }config_reg;                    // ERTEC200-Config-Pins

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 19; /* D13:31 */
         unsigned pulse_dur  : 10; /* D03:12 */
         unsigned en_wd_soft_res_irte : 1; /* D02 */
         unsigned xres_soft  : 1;  /* D01 */
         unsigned wd_res_frei : 1; /* D00 */
      } bits;
   }res_ctrl_reg;                  // Control register for reset

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 29; /* D03:31 */
         unsigned poweron_hardware_reset : 1; /* D02 */
         unsigned software_reset : 1; /* D01 */
         unsigned watchdog_reset : 1; /* D00 */
      } bits;
   }res_stat_reg;                  // Status register for reset

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 14; /* D18:31 */
         unsigned int_mask_loss : 1; /* D17 */
         unsigned int_mask_lock : 1; /* D16 */
         unsigned            : 10; /* D06:15 */
         unsigned int_qvz_emif : 1; /* D05 */
         unsigned            : 1;  /* D04 */
         unsigned int_loss   : 1;  /* D03 */
         unsigned int_lock   : 1;  /* D02 */
         unsigned loss       : 1;  /* D01 */
         unsigned lock       : 1;  /* D00 */
      } bits;
   }pll_stat_reg;                  // Status register for PLL

   unsigned long dummy1[4];

   unsigned long qvz_ahb_adr;         // Address of incorrect addressing on the multi-layer AHB

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 25; /* D07:31 */
         unsigned hburts     : 3;  /* D04:06 */
         unsigned hsize      : 3;  /* D01:03 */
         unsigned r_rw       : 1;  /* D00 */
      } bits;
   }qvz_ahb_ctrl;                  // Control signals of incorrect addressing on the multi-layer AHB

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 28; /* D04:31 */
         unsigned dma        : 1;  /* D03 */
         unsigned irte       : 1;  /* D02 */
         unsigned lub        : 1;  /* D01 */
         unsigned arm946     : 1;  /* D00 */
      } bits;
   }qvz_ahb_m;                     // Mast detection of incorrect addressing on the multi-layer AHB

   unsigned long qvz_apb_adr;         // Address of incorrect addressing at the APB

   unsigned long qvz_emif_adr;        // Address that led to the QVZ at EMIF

   unsigned long dummy2[2];

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 30; /* D02:31 */
         unsigned seg0       : 2;  /* D00:01 */
      } bits;
   }mem_swap;                      // Memory swapping in segment 0 on the AHB

   unsigned long dummy3;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 28; /* D04:31 */
         unsigned arb_mode   : 1;  /* D03 */
         unsigned irte       : 1;  /* D02 */
         unsigned lbu        : 1;  /* D01 */
         unsigned arm9       : 1;  /* D00 */
      } bits;
   }mem_lock_ctrl;                 // AHB-Master-Lock-Enable

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 18; /* D14:31 */
         unsigned bigendian  : 1;  /* D13 */
         unsigned gatetheclk : 1;  /* D12 */
         unsigned dbgen      : 1;  /* D11 */
         unsigned micebypass : 1;  /* D10 */
         unsigned initram    : 1;  /* D09 */
         unsigned emt_opt    : 9;  /* D00:08 */
      } bits;
   }arm9_ctrl;                     // Control of ARM9 inputs

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 31; /* D01:31 */
         unsigned write_en   : 1;  /* D00 */
      } bits;
   }arm9_we;                       // Write protection register for the register ARM9_CTRL

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 8;  /* D24:31 */
         unsigned revision_id : 8; /* D16:23 */
         unsigned version_id : 8;  /* D08:15 */
         unsigned debug_id   : 8;  /* D00:07 */
      } bits;
   }ertec200_tag;                  // Tag number of the current circuit status

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 15; /* D17:31 */
         unsigned phy_res_sel : 1; /* D16 */
         unsigned            : 2;  /* D14:15 */
         unsigned p2_automdixen : 1; /* D13 */
         unsigned p2_phy_mode : 3; /* D10:12 */
         unsigned p2_fx_mode : 1;  /* D09 */
         unsigned p2_phy_enb : 1;  /* D08 */
         unsigned            : 2;  /* D06:07 */
         unsigned p1_automdixen : 1; /* D05 */
         unsigned p1_phy_mode : 3; /* D02:04 */
         unsigned p1_fx_mode : 1;  /* D01 */
         unsigned p1_phy_enb : 1;  /* D00 */
      } bits;
   }phy_config;                    // Configuration of PHY1 and PHY2

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 29; /* D03:31 */
         unsigned p2_pwruprst : 1; /* D02 */
         unsigned            : 1;  /* D01 */
         unsigned p1_pwruprst : 1; /* D00 */
      } bits;
   }phy_status;                    // Status of PHY1 and PHY2

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 28; /* D04:31 */
         unsigned enb        : 4;  /* D00:03 */
      } bits;
   }handrad_ctrl;                  // Check the handwheel input filter

   union
   {
      unsigned long reg;
      struct
      {
         unsigned           : 16;  /* D16:31 */
         unsigned counter   : 16;  /* D00:15 */
      } bits;
   }handrad_counter1,
    handrad_counter2;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 31; /* D01:31 */
         unsigned clk        : 1;  /* D00 */
      } bits;
   }uart_clk;
} ERTEC200_IO_SCRB;


/************** FCOUNT_BASE *******************************
definition of the registers in the
F-COUNTER Module starting @ 0x4000_2700
*************** FCOUNT_BASE *******************************/

typedef struct
{

   unsigned long f_counter_val;       // F-counter timer value

   union
   {
      unsigned long reg;
      struct
      {
         unsigned f_cnt_res_high : 16; /* D16:31 */
         unsigned f_cnt_res_low : 16; /* D00:15 */
      } bits;
   }f_counter_res;

} ERTEC200_IO_FCOUNT;


/************** ESMA2_BASE *******************************
definition of the registers in the
ESMA2 Module starting @ 0x4001_0000
*************** ESMA2_BASE *******************************/

typedef struct
{

   // Registerblock
   struct
   {
      unsigned short auftrag;          //0x0000
      unsigned short dataptr;          //0x0002
      unsigned short blockptr;         //0x0004
      unsigned short auftragsstatus;   //0x0006
      unsigned short stapelstart;      //0x0008
      unsigned short stapelstatus;     //0x000A
      unsigned short stapelfehler;     //0x000C
      unsigned short dummy1;           //0x000E
      unsigned short max_ausbau;       //0x0010
      unsigned short modul_anz;        //0x0012
      unsigned short dummy2;           //0x0014
      unsigned short dummy3;           //0x0016
      unsigned short paa_laenge;       //0x0018
      unsigned short lueckenreg;       //0x001A
      unsigned short event;            //0x001C
      unsigned short laufzeitreg;      //0x001E
      unsigned short baudrate;         //0x0020
      unsigned short steuer;           //0x0022
      unsigned short status;           //0x0024
      unsigned short kommando;         //0x0026
      unsigned short steuerwort_se;    //0x0028
      unsigned short se_daten_ptr;     //0x002A
      unsigned short dummy4;           //0x002C
      unsigned short dummy5;//48       //0x002E
      unsigned short fehleranzahl;     //0x0030
      unsigned short fehler_adr_1;     //0x0032
      unsigned short fehler_adr_2;     //0x0034
      unsigned short fehler_adr_3;     //0x0036
      unsigned short fehler_adr_4;     //0x0038
      unsigned short fehler_adr_5;     //0x003A
      unsigned short fehler_adr_6;     //0x003C
      unsigned short dummy6; //64      //0x003E
      unsigned short pae_event_anz;    //0x0040
      unsigned short dummy7[31];//128  //0x0042 - 0x007F -> End of the 1st line
      unsigned short dummy8[448];      //0x0080 - 0x03FF -> 896 bytes reserved

      //Register der 2. Zeile
      unsigned short auftrag_Z2;       //0x0400
      unsigned short dataptr_Z2;       //0x0402
      unsigned short blockptr_Z2;      //0x0404
      unsigned short auftragsstatus_Z2; //0x0406
      unsigned short stapelstart_Z2;   //0x0408
      unsigned short stapelstatus_Z2;  //0x040A
      unsigned short stapelfehler_Z2;  //0x040C
      unsigned short dummy1_Z2;        //0x040E
      unsigned short max_ausbau_Z2;    //0x0410
      unsigned short modul_anz_Z2;     //0x0412
      unsigned short dummy2_Z2;        //0x0414
      unsigned short dummy3_Z2;        //0x0416
      unsigned short paa_laenge_Z2;    //0x0418
      unsigned short lueckenreg_Z2;    //0x041A
      unsigned short event_Z2;         //0x041C
      unsigned short laufzeitreg_Z2;   //0x041E
      unsigned short baudrate_Z2;      //0x0420
      unsigned short steuer_Z2;        //0x0422
      unsigned short status_Z2;        //0x0424
      unsigned short kommando_Z2;      //0x0426
      unsigned short steuerwort_se_Z2; //0x0428
      unsigned short se_daten_ptr_Z2;  //0x042A
      unsigned short dummy4_Z2;        //0x042C
      unsigned short dummy5_Z2;        //0x042E
      unsigned short fehleranzahl_Z2;  //0x0430
      unsigned short fehler_adr_1_Z2;  //0x0432
      unsigned short fehler_adr_2_Z2;  //0x0434
      unsigned short fehler_adr_3_Z2;  //0x0436
      unsigned short fehler_adr_4_Z2;  //0x0438
      unsigned short fehler_adr_5_Z2;  //0x043A
      unsigned short fehler_adr_6_Z2;  //0x043C
      unsigned short dummy6_Z2;        //0x043E
      unsigned short pae_event_anz_Z2; //0x0440
      unsigned short dummy7_Z2[31];    //0x0442 - 0x047F -> End of the 2nd line
      unsigned short dummy8_Z2[448];   //0x0480 - 0x07FF -> 896 bytes reserved
      unsigned short hw_kennung;       //0x0800
      unsigned short dummy9[63];       //0x0802 - 0x087F -> 126 bytes reserved
      unsigned short eingabeport;//2176 //0x00880
      unsigned short ausgabeport;      //0x00882
      unsigned short int_status0;      //0x00884
      unsigned short int_status1;//2182 //0x00886
      unsigned short reserved[956];    //0x00888 - 0x0FFF -> End of register area
   } ESMA2;

   // globaler RAM
   struct
   {
      unsigned short input_buf [128];  //0x1000, Input data for modules on the ES bus
      unsigned short output_buf[128];  //0x1100, Output data from modules on the ES bus
      unsigned short cmd_def   [256];  //0x1200, Command definition - storage of several consecutive bus commands (128 4-byte commands)
      unsigned short reserved  [1536]; //0x13FF
   } GLOBAL_RAM;

   // Zeile 1
   struct
   {
      unsigned short job_buf   [128];  //0x2000 (0x3000), Order buffer - universal data buffer for command-specific data
      unsigned short event_buf [128];  //0x2100 (0x3100), Event buffer - module-specific event buffer (ES bus), 2 bytes / module as PAE word
                                   //0x2180 (0x3180), Diagnostic buffer - module-specific diagnostic buffer (ES bus), 2 bytes / module
      unsigned short ea_def    [128];  //0x2200 (0x3200), I / O definition - storage of the configuration of the input / output buffer, 4 bytes / module
      unsigned short ev_state  [128];  //0x2300 (0x3300), Event list - events when processing the command stack 2 bytes / module + event address
      unsigned short cmd_stack [512];  //0x2400 (0x3400), Command stack - storage of frequently used command sequences
      unsigned short reserved  [1024]; //0x2800 (0x3800)
                                   // up to 0x2FFF (0x3FFF)
   } EXCL_RAM;

} ERTEC200_IO_ESMA2;


/************** ARM_ICU_BASE *******************************
definition of the registers in the
ARM_ICU Module starting @ 0x5000_0000
*************** ARM_ICU_BASE *******************************/

typedef struct
{

   union
   {
      unsigned long reg;
      struct
      {
         unsigned vectorkennung : 28; /* D04:31 */
         unsigned irvec      : 4;  /* D00:03 */
      } bits;
   }irvec;                         // Interrupt Vector Register

   union
   {
      unsigned long reg;
      struct
      {
         unsigned vectorkennung : 29; /* D03:31 */
         unsigned fivec      : 3;  /* D00:02 */
      } bits;
   }fivec;                         // Fast Interrupt Vector Register

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 24; /* D08:31 */
         unsigned aktiv      : 1;  /* D07 */
         unsigned            : 3;  /* D04:06 */
         unsigned prio       : 4;  /* D00:03 */
      } bits;
   }lockreg;                       // Priority Lock Register

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 24; /* D08:31 */
         unsigned fiqsreg    : 1;  /* D07 */
         unsigned            : 3;  /* D04:06 */
         unsigned fiqsenable : 4;  /* D00:03 */
      } bits;
   }fiq1reg,                       // Fast Interrupt Request 1 Select Register
    fiq2reg;                       // Fast Interrupt Request 2 Select Register

   union
   {
      unsigned long reg;
      struct
      {
         unsigned vectorkennung : 28; /* D04:31 */
         unsigned irvec      : 4;  /* D00:03 */
      } bits;
   }irqack;                        // Interrupt Vector Register with IRQ Acknowledge

   union
   {
      unsigned long reg;
      struct
      {
         unsigned vectorkennung : 29; /* D03:31 */
         unsigned fivec      : 3;  /* D00:02 */
      } bits;
   }fiqack;                        // Fast Interrupt Vector Register with FIQ Acknowledge

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 28; /* D04:31 */
         unsigned irclvec    : 4;  /* D00:03 */
      } bits;
   }irclvec;                       // Interrupt Request Clear Vector

   union
   {
      unsigned long reg;
      struct
      {
         unsigned           : 31;  /* D01:31 */
         unsigned maskall   : 1;   /* D00 */
      } bits;
   }maskall;                       // Mask all Interrupts

   unsigned long irqend;              // End of  Interrupt

   unsigned long fiqend;              // End of  Interrupt

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 29; /* D03:31 */
         unsigned fiqpr      : 3;  /* D00:02 */
      } bits;
   }fiqpr0,                        // FIQ Priority Register
    fiqpr1,
    fiqpr2,
    fiqpr3,
    fiqpr4,
    fiqpr5,
    fiqpr6,
    fiqpr7;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 24; /* D08:31 */
         unsigned res1       : 1;  /* D07 */
         unsigned res0       : 1;  /* D06 */
         unsigned commtx     : 1;  /* D05 */
         unsigned commrx     : 1;  /* D04 */
         unsigned pll        : 1;  /* D03 */
         unsigned ahb        : 1;  /* D02 */
         unsigned apb        : 1;  /* D01 */
         unsigned wd         : 1;  /* D00 */
      } bits;
   }fiqisr,                        // FIQ In Service Register
    fiqirr,                        // FIQ Request Register
    fiqmaskreg;                    // Interrupt mask register for FIQ

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 16; /* D16:31 */
         unsigned dma        : 1;  /* D15 */
         unsigned esma2      : 1;  /* D14 */
         unsigned irte_l     : 1;  /* D13 */
         unsigned irte_h     : 1;  /* D12 */
         unsigned spi1_rec   : 1;  /* D11 */
         unsigned spi1       : 1;  /* D10 */
         unsigned phy        : 1;  /* D09 */
         unsigned uart       : 1;  /* D08 */
         unsigned ssc        : 1;  /* D07 */
         unsigned timer2     : 1;  /* D06 */
         unsigned gpio31     : 1;  /* D05 */
         unsigned gpio30     : 1;  /* D04 */
         unsigned gpio1      : 1;  /* D03 */
         unsigned gpio0      : 1;  /* D02 */
         unsigned timer1     : 1;  /* D01 */
         unsigned timer0     : 1;  /* D00 */
      } bits;
   }irreg,                         // Interrupt Request Register
    maskreg,                       // Interrupt Mask Register
    isreg,                         // Interrupt Service Register
    trigreg,                       // Trigger Select Register
    edgereg,                       // Edge Select Register
    swirreg;                       // Software Interrupt Register

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 28; /* D04:31 */
         unsigned prio       : 4;  /* D00:03 */
      } bits;
   }prioreg0,                      // Priority Register
    prioreg1,
    prioreg2,
    prioreg3,
    prioreg4,
    prioreg5,
    prioreg6,
    prioreg7,
    prioreg8,
    prioreg9,
    prioreg10,
    prioreg11,
    prioreg12,
    prioreg13,
    prioreg14,
    prioreg15;

} ERTEC200_IO_ARM_ICU;

/************** EMIF_REG_BASE *******************************
definition of the registers in the
EMIF_REGISTER Module starting @ 0x7000_0000
*************** EMIF_REG_BASE *******************************/

typedef struct
{

   union
   {
      unsigned long reg;
      struct
      {
         unsigned           : 16;   /* D16:31 */
         unsigned major     : 8;    /* D08:15 */
         unsigned minor     : 8;    /* D00:07 */
      } bits;
   }rev_code_stat;                 // Revison Code und Status Register

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 1;  /* D31 */
         unsigned wp         : 1;  /* D30 */
         unsigned            : 22; /* D08:29 */
         unsigned max_ext_wait : 8; /* D00:07 */
      } bits;
   }async_wait_cycle_conf;         // Async Wait Cycle Config Register

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 18; /* D14:31 */
         unsigned cl         : 1;  /* D13 */
         unsigned            : 2;  /* D11:12 */
         unsigned rows       : 3;  /* D08:10 */
         unsigned            : 1;  /* D07 */
         unsigned ibank      : 3;  /* D04:06 */
         unsigned            : 1;  /* D03 */
         unsigned pagesize   : 3;  /* D00:02 */
      } bits;
   }sdram_bank_conf;               // SDRAM Bank Config Register

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 1;  /* D31 */
         unsigned at         : 1;  /* D30 */
         unsigned init_done  : 1;  /* D29 */
         unsigned            : 16; /* D13:28 */
         unsigned rate       : 13; /* D00:12 */
      } bits;
   }sdram_refresh_ctrl;            // Setting the refresh frequency

   union
   {
      unsigned long reg;
      struct
      {
         unsigned ews_xas    : 1;  /* D31 */
         unsigned ew         : 1;  /* D30 */
         unsigned w_su       : 4;  /* D26:29 */
         unsigned w_strobe   : 6;  /* D20:25 */
         unsigned w_hold     : 3;  /* D17:19 */
         unsigned r_su       : 4;  /* D13:16 */
         unsigned r_strobe   : 6;  /* D07:12 */
         unsigned r_hold     : 3;  /* D04:06 */
         unsigned            : 2;  /* D02:03 */
         unsigned asize      : 2;  /* D00:01 */
      } bits;
   }async_bank0_conf,              // Setting the timing and the data bus width
    async_bank1_conf,
    async_bank2_conf,
    async_bank3_conf;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 1;  /* D31 */
         unsigned test1      : 1;  /* D30 */
         unsigned test2      : 1;  /* D29 */
         unsigned            : 3;  /* D26:28 */
         unsigned adb        : 1;  /* D25 */
         unsigned asdb       : 1;  /* D24 */
         unsigned            : 4;  /* D20:23 */
         unsigned test3      : 1;  /* D19 */
         unsigned            : 1;  /* D18 */
         unsigned burst_lenght : 2; /* D16:17 */
         unsigned            : 1;  /* D15 */
         unsigned trcd_tcd   : 1;  /* D14 */
         unsigned            : 5;  /* D09:13 */
         unsigned sdsize     : 1;  /* D08 */
         unsigned atirq      : 1;  /* D07 */
         unsigned            : 7;  /* D00:06 */
      } bits;
   }extended_conf;                 // Setting additional functions

} ERTEC200_IO_EMIF_REG;


/************** DMA_REG_BASE *******************************
definition of the registers in the
DMA REGISTER Module starting @ 0x8000_0000
*************** DMA_REG_BASE *******************************/

typedef struct
{

   unsigned long dma_src_addr;        // Start address of the data block to be transferred from the DMA controller

   unsigned long dma_dest_addr;       // Destination address of the data block to be transferred by the DMA controller

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 8;  /* D24:31 */
         unsigned d_delay    : 3;  /* D21:23 */
         unsigned s_delay    : 5;  /* D16:20 */
         unsigned byte_count : 16; /* D00:15 */
      } bits;
   }dma_ctrl;                      // To define the data block length

   union
   {
      unsigned long reg;
      struct
      {
         unsigned start      : 1;  /* D31 */
         unsigned            : 1;  /* D30 */
         unsigned intr_enable : 1; /* D29 */
         unsigned sync       : 2;  /* D27:28 */
         unsigned            : 3;  /* D24:26 */
         unsigned s_addr_mode : 2; /* D22:23 */
         unsigned s_dma_requ : 3;  /* D19:21 */
         unsigned s_widht    : 3;  /* D16:18 */
         unsigned d_addr_mode : 2; /* D14:15 */
         unsigned d_dma_requ : 3;  /* D11:13 */
         unsigned d_widht    : 3;  /* D08:10 */
         unsigned d_delay    : 4;  /* D04:07 */
         unsigned s_delay    : 4;  /* D00:03 */
      } bits;
   }dma_conf;                      // Control bits

} ERTEC200_IO_DMA_REG;


#else /* end of "#ifndef _LITTLE_ENDIAN" clause */


/* Little Endian */

/************** IRTE_BASE *******************************
definition of the registers in the EtherNet
Switch area starting @ 0x1000_0000
*************** IRTE_BASE *******************************/

typedef struct
{
    int dummy;
} ERTEC200_IO_IRTE;



/*************** TIMER_BASE ******************************
definition of the registers in the Timer
area starting @ 0x4000_2000
**************** TIMER_BASE ******************************/

typedef struct
{

   union
   {
      unsigned long reg;
      struct
      {
         unsigned run        : 1;  /* D00    */
         unsigned load       : 1;  /* D01    */
         unsigned reload     : 1;  /* D02    */
         unsigned            : 2;  /* D03:04 */
         unsigned status     : 1;  /* D05    */
         unsigned            : 26; /* D06:31 */
      } bits;
   } ctrl_stat0;                   // Control / Status Register Timer 0

   union
   {
      unsigned long reg;
      struct
      {
         unsigned run        : 1;  /* D00 */
         unsigned load       : 1;  /* D01 */
         unsigned reload     : 1;  /* D02 */
         unsigned            : 2;  /* D03:04 */
         unsigned status     : 1;  /* D05 */
         unsigned kask       : 1;  /* D06 */
         unsigned            : 25; /* D07:31 */
      } bits;
   } ctrl_stat1;                   // Control / Status Register Timer 1

   unsigned long reld0;               // Reload Register 0

   unsigned long reld1;               // Reload Register 1

   union
   {
      unsigned long reg;
      struct
      {
         unsigned run_v0     : 1;  /* D00 */
         unsigned load_v0    : 1;  /* D01 */
         unsigned run_v1     : 1;  /* D02 */
         unsigned load_v1    : 1;  /* D03 */
         unsigned            : 28; /* D04:31 */
      } bits;
   } ctrl_prediv;                  // Control Register fuer 2 Predivider fuer Timer 0/1

   union
   {
      unsigned long reg;
      struct
      {
         unsigned prediv0    : 8;  /* D00:07 */
         unsigned prediv1    : 8;  /* D08:15 */
         unsigned            : 16; /* D16:31 */
      } bits;
   } reld_prediv;                  // Reload-Register fuer die 2 Predivider

   unsigned long tim0;                // Timer Register 0

   unsigned long tim1;                // Timer Register 1

   union
   {
      unsigned long reg;
      struct
      {
         unsigned reload     : 16; /* D00:15 */
         unsigned run        : 1;  /* D16    */
         unsigned one_shot   : 1;  /* D17    */
         unsigned timer      : 1;  /* D18    */
         unsigned            : 13; /* D19:31 */
      } bits;
   } timer2_ctrl;                  // Timer 2 Control Register

   union
   {
      unsigned long reg;
      struct
      {
         unsigned timer      : 16; /* D00:15 */
         unsigned            : 16; /* D16:31 */
      } bits;
   } tim2;                         /* Timer 2 Register */

} ERTEC200_IO_TIMER;



/************** WD_BASE ********************************
definition of the registers in the Watchdog
area starting @ 0x4000_2100
*************** WD_BASE ********************************/

typedef struct
{

   union
   {
      unsigned long reg;
      struct
      {
         unsigned run_wd0    : 1;  /* D00    */
         unsigned run_wd1    : 1;  /* D01    */
         unsigned load       : 1;  /* D02    */
         unsigned stat_wd0   : 1;  /* D03    */
         unsigned stat_wd1   : 1;  /* D04    */
         unsigned            : 11; /* D05:15 */
         unsigned keybits    : 16; /* D16:31 */
      } bits;
   } ctrl_status;                  // Configuration and control bits for the watchdog

   union
   {
      unsigned long reg;
      struct
      {
         unsigned reload     : 16; /* D00:15 */
         unsigned keybits    : 16; /* D16:31 */
      } bits;
   } reld0_low,                    // Reload value for bits 15:0 of watchdog counter 0
     reld0_high,                   // Reload value for bits 31:16 of watchdog counter 0
     reld1_low,                    // Reload value for bits 19:4 of watchdog counter 1
     reld1_high;                   // Reload value for bits 35:20 of watchdog counter 1

   unsigned long wdog0;               // Value of the watchdog counter 0

   unsigned long wdog1;               // Value of the watchdog counter 1

} ERTEC200_IO_WD;



/************** SPI1_BASE *******************************
definition of the registers in the
SPI1 Module starting @ 0x4000_2200
*************** SPI1_BASE *******************************/

typedef struct
{

   union
   {
      unsigned long reg;
      struct
      {
         unsigned dss        : 4;  /* D00:03 */
         unsigned frf        : 2;  /* D04:05 */
         unsigned spo        : 1;  /* D06    */
         unsigned sph        : 1;  /* D07    */
         unsigned scr        : 8;  /* D08:15 */
         unsigned            : 16; /* D16:31 */
      } bits;
   }sspcr0;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned rie        : 1;  /* D00    */
         unsigned tie        : 1;  /* D01    */
         unsigned rorie      : 1;  /* D02    */
         unsigned lbm        : 1;  /* D03    */
         unsigned sse        : 1;  /* D04    */
         unsigned ms         : 1;  /* D05    */
         unsigned sod        : 1;  /* D06    */
         unsigned            : 25; /* D07:31 */
      } bits;
   }sspcr1;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned data       : 16; /* D00:15 */
         unsigned            : 16; /* D16:31 */
      } bits;
   }sspdr;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned tfe        : 1;  /* D00    */
         unsigned tnf        : 1;  /* D01    */
         unsigned rne        : 1;  /* D02    */
         unsigned rff        : 1;  /* D03    */
         unsigned bsy        : 1;  /* D04    */
         unsigned            : 27; /* D05:31 */
      } bits;
   }sspsr;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned cpsdvsr    : 8;  /* D00:07 */
         unsigned            : 24; /* D08:31 */
      } bits;
   }sspcpsr;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned data       : 16; /* D00:15 */
         unsigned            : 16; /* D16:31 */
      } write_bits;
      struct
      {
         unsigned ris        : 1;  /* D00    */
         unsigned tis        : 1;  /* D01    */
         unsigned roris      : 1;  /* D02    */
         unsigned            : 29; /* D03:31 */
      } read_bits;
   }sspiir_icr;

} ERTEC200_IO_SPI1;



/************** UART1_BASE *******************************
definition of the registers in the
UART1 Module starting @ 0x4000_2300
*************** UART1_BASE *******************************/

typedef struct
{

   union
   {
      unsigned long reg;
      struct
      {
         unsigned data       : 8;  /* D00:07 */
         unsigned            : 24; /* D08:31 */
      } bits;
   }uartdr;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned data       : 8;  /* D00:07 */
         unsigned            : 24; /* D08:31 */
      }write_bits;
      struct
      {
         unsigned fe         : 1;  /* D00    */
         unsigned pe         : 1;  /* D01    */
         unsigned be         : 1;  /* D02    */
         unsigned oe         : 1;  /* D03    */
         unsigned            : 28; /* D04:31 */
      }read_bits;
   }uartrsr_ecr;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned brk        : 1;  /* D00    */
         unsigned pen        : 1;  /* D01    */
         unsigned eps        : 1;  /* D02    */
         unsigned stp2       : 1;  /* D03    */
         unsigned fen        : 1;  /* D04    */
         unsigned wlen       : 2;  /* D05:06 */
         unsigned            : 25; /* D07:31 */
      } bits;
   }uartlcr_h;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned baudrate   : 8;  /* D00:07 */
         unsigned            : 24; /* D08:31 */
      } bits;
   }uartlcr_m;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned baudrate   : 8;  /* D00:07 */
         unsigned            : 24; /* D08:31 */
      } bits;
   }uartlcr_l;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned uarten     : 1;  /* D00    */
         unsigned siren      : 1;  /* D01    */
         unsigned sirlp      : 1;  /* D02    */
         unsigned msie       : 1;  /* D03    */
         unsigned rie        : 1;  /* D04    */
         unsigned tie        : 1;  /* D05    */
         unsigned rtie       : 1;  /* D06    */
         unsigned lbe        : 1;  /* D07    */
         unsigned            : 24; /* D08:31 */
      } bits;
   }uartcr;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned cts        : 1;  /* D00    */
         unsigned dsr        : 1;  /* D01    */
         unsigned dcd        : 1;  /* D02    */
         unsigned busy       : 1;  /* D03    */
         unsigned rxfe       : 1;  /* D04    */
         unsigned txff       : 1;  /* D05    */
         unsigned rxff       : 1;  /* D06    */
         unsigned txfe       : 1;  /* D07    */
         unsigned            : 24; /* D08:31 */
      } bits;
   }uartfr;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned data       : 8;  /* D00:07 */
         unsigned            : 24; /* D08:31 */
      }write_bits;
      struct
      {
         unsigned mis        : 1;  /* D00 */
         unsigned ris        : 1;  /* D01 */
         unsigned tis        : 1;  /* D02 */
         unsigned rtis       : 1;  /* D03 */
         unsigned            : 28; /* D04:31 */
      }read_bits;
   }uartiir_icr;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned ilpdvsr    : 8;  /* D00:07 */
         unsigned            : 24; /* D08:31 */
      } bits;
   }uartilpr;

} ERTEC200_IO_UART1;



/************** SSC_BASE *******************************
definition of the registers in the
SSC Module starting @ 0x4000_2400
*************** SSC_BASE *******************************/

typedef struct
{

   union
   {
      unsigned long reg;
      struct
      {
         unsigned rtbuf      : 8;  /* D0:07 */
         unsigned            : 24; /* D08:31 */
      } bits;
   } ssc_rtbuf;                    // Receive Transmit Buffer

   union
   {
      unsigned long reg;
      struct
      {
         unsigned tbem       : 1;  /* D00 */
         unsigned rbfu       : 1;  /* D01 */
         unsigned perr       : 1;  /* D02 */
         unsigned recerr     : 1;  /* D03 */
         unsigned            : 3;  /* D04:06 */
         unsigned busy       : 1;  /* D07 */
         unsigned            : 24; /* D08:31 */
      } bits;
   } ssc_stat;                     // Status-Register

   union
   {
      unsigned long reg;
      struct
      {
         unsigned cpha       : 1;  /* D00 */
         unsigned cpol       : 1;  /* D01 */
         unsigned hcb        : 1;  /* D02 */
         unsigned pen        : 1;  /* D03 */
         unsigned ppos       : 1;  /* D04 */
         unsigned podd       : 1;  /* D05 */
         unsigned            : 1;  /* D06 */
         unsigned bren       : 1;  /* D07 */
         unsigned            : 24; /* D08:31 */
      } bits;
   } ssc_ctrl1;                    // Control-Register 1

   union
   {
      unsigned long reg;
      struct
      {
         unsigned dw         : 3;  /* D00:02 */
         unsigned            : 29; /* D03:31 */
      } bits;
   } ssc_ctrl2;                    // Control-Register 2

   union
   {
      unsigned long reg;
      struct
      {
         unsigned entbem     : 1;  /* D00 */
         unsigned enrbfu     : 1;  /* D01 */
         unsigned enperr     : 1;  /* D02 */
         unsigned enrecerr   : 1;  /* D03 */
         unsigned            : 28; /* D04:31 */
      } bits;
   } ssc_int_en;                   // Interrupt-Enable-Register

   union
   {
      unsigned long reg;
      struct
      {
         unsigned g          : 8;  /* D00:07 */
         unsigned            : 24; /* D08:31 */
      } bits;
   } ssc_baud;                     // Baud rate register

} ERTEC200_IO_SSC;


/************** GPIO_BASE *******************************
definition of the registers in the
GPIO Module starting @ 0x4000_2500
*************** GPIO_BASE *******************************/

typedef struct
{

   union
   {
      unsigned long reg;
      struct
      {
          unsigned char port_a_00_07;
          unsigned char port_a_08_15;
          unsigned char port_a_16_23;
          unsigned char port_a_24_31;
      }byte_access;
      struct
      {
         unsigned p00        : 1;  /* D00 */ // GPIO 0
         unsigned p01        : 1;  /* D01 */ // GPIO 1
         unsigned p02        : 1;  /* D02 */ // GPIO 2
         unsigned p03        : 1;  /* D03 */ // GPIO 3
         unsigned p04        : 1;  /* D04 */ // GPIO 4
         unsigned p05        : 1;  /* D05 */ // GPIO 5
         unsigned p06        : 1;  /* D06 */ // GPIO 6
         unsigned p07        : 1;  /* D07 */ // GPIO 7
         unsigned p08        : 1;  /* D08 */ // GPIO 8
         unsigned p09        : 1;  /* D09 */ // GPIO 9
         unsigned p10        : 1;  /* D10 */ // GPIO 10
         unsigned p11        : 1;  /* D11 */ // GPIO 11
         unsigned p12        : 1;  /* D12 */ // GPIO 12
         unsigned p13        : 1;  /* D13 */ // GPIO 13
         unsigned p14        : 1;  /* D14 */ // GPIO 14
         unsigned p15        : 1;  /* D15 */ // GPIO 15
         unsigned p16        : 1;  /* D16 */ // GPIO 16
         unsigned p17        : 1;  /* D17 */ // GPIO 17
         unsigned p18        : 1;  /* D18 */ // GPIO 18
         unsigned p19        : 1;  /* D19 */ // GPIO 19
         unsigned p20        : 1;  /* D20 */ // GPIO 20
         unsigned p21        : 1;  /* D21 */ // GPIO 21
         unsigned p22        : 1;  /* D22 */ // GPIO 22
         unsigned p23        : 1;  /* D23 */ // GPIO 23
         unsigned p24        : 1;  /* D24 */ // GPIO 24
         unsigned p25        : 1;  /* D25 */ // GPIO 25
         unsigned p26        : 1;  /* D26 */ // GPIO 26
         unsigned p27        : 1;  /* D27 */ // GPIO 27
         unsigned p28        : 1;  /* D28 */ // GPIO 28
         unsigned p29        : 1;  /* D29 */ // GPIO 29
         unsigned p30        : 1;  /* D30 */ // GPIO 30
         unsigned p31        : 1;  /* D31 */ // GPIO 31
      } pin;
   }gpio_ioctrl,                   // Configuration register for general purpose IOs
    gpio_out,                      // Output register for general purpose IOs
    gpio_in;                       // Input register for general purpose IOs

   union
   {
      unsigned long reg;
      struct
      {
         unsigned gpio_0     : 2;  /* D00:01 */
         unsigned gpio_1     : 2;  /* D02:03 */
         unsigned gpio_2     : 2;  /* D04:05 */
         unsigned gpio_3     : 2;  /* D06:07 */
         unsigned gpio_4     : 2;  /* D08:09 */
         unsigned gpio_5     : 2;  /* D10:11 */
         unsigned gpio_6     : 2;  /* D12:13 */
         unsigned gpio_7     : 2;  /* D14:15 */
         unsigned gpio_8     : 2;  /* D16:17 */
         unsigned gpio_9     : 2;  /* D18:19 */
         unsigned gpio_10    : 2;  /* D20:21 */
         unsigned gpio_11    : 2;  /* D22:23 */
         unsigned gpio_12    : 2;  /* D24:25 */
         unsigned gpio_13    : 2;  /* D26:27 */
         unsigned gpio_14    : 2;  /* D28:29 */
         unsigned gpio_15    : 2;  /* D30:31 */
      } bits;
   }gpio_port_mode_l;              // Configuration register for GPIO port

   union
   {
      unsigned long reg;
      struct
      {
         unsigned gpio_16    : 2;  /* D00:01 */
         unsigned gpio_17    : 2;  /* D02:03 */
         unsigned gpio_18    : 2;  /* D04:05 */
         unsigned gpio_19    : 2;  /* D06:07 */
         unsigned gpio_20    : 2;  /* D08:09 */
         unsigned gpio_21    : 2;  /* D10:11 */
         unsigned gpio_22    : 2;  /* D12:13 */
         unsigned gpio_23    : 2;  /* D14:15 */
         unsigned gpio_24    : 2;  /* D16:17 */
         unsigned gpio_25    : 2;  /* D18:19 */
         unsigned gpio_26    : 2;  /* D20:21 */
         unsigned gpio_27    : 2;  /* D22:23 */
         unsigned gpio_28    : 2;  /* D24:25 */
         unsigned gpio_29    : 2;  /* D26:27 */
         unsigned gpio_30    : 2;  /* D28:29 */
         unsigned gpio_31    : 2;  /* D30:31 */
      } bits;
   }gpio_port_mode_h;              // Configuration register for GPIO port

   union
   {
      unsigned long reg;
      struct
      {
         unsigned gpio0      : 1;  /* D00 */
         unsigned gpio1      : 1;  /* D01 */
         unsigned gpio30     : 1;  /* D02 */
         unsigned gpio31     : 1;  /* D03 */
         unsigned            : 28; /* D04:31 */
      } bits;
   } gpio_polsel;                  // Interrupt polarity for GPIO interrupts

   unsigned long notused_1;
   unsigned long notused_2;

   union
   {
      unsigned long reg;
      struct
      {
          unsigned char port_b_00_07;
          unsigned char port_b_08_15;
          unsigned char port_b_unused[2];
      }byte_access;
      struct
      {
         unsigned p32        : 1;  /* D00 */ // GPIO 32
         unsigned p33        : 1;  /* D01 */ // GPIO 33
         unsigned p34        : 1;  /* D02 */ // GPIO 34
         unsigned p35        : 1;  /* D03 */ // GPIO 35
         unsigned p36        : 1;  /* D04 */ // GPIO 36
         unsigned p37        : 1;  /* D05 */ // GPIO 37
         unsigned p38        : 1;  /* D06 */ // GPIO 38
         unsigned p39        : 1;  /* D07 */ // GPIO 39
         unsigned p40        : 1;  /* D08 */ // GPIO 40
         unsigned p41        : 1;  /* D09 */ // GPIO 41
         unsigned p42        : 1;  /* D10 */ // GPIO 42
         unsigned p43        : 1;  /* D11 */ // GPIO 43
         unsigned p44        : 1;  /* D12 */ // GPIO 44
         unsigned            : 19; /* D13:31 */
      } bits;
   } gpio2_ioctrl,                 // Configuration register for general purpose IOs
     gpio2_out,                    // Output register for general purpose IOs
     gpio2_in;                     // Input register for general purpose IOs

} ERTEC200_IO_GPIO;


/************** SCRB_BASE *******************************
definition of the registers in the
SCRB Module starting @ 0x4000_2600
*************** SCRB_BASE *******************************/

typedef struct
{

   union
   {
      unsigned long reg;
      struct
      {
         unsigned metall_fix : 8;  /* D00:07 */
         unsigned hw_release : 8;  /* D08:15 */
         unsigned id         : 16; /* D16:31 */
      } bits;
   }id_reg;                        // Identification ERTEC200

   union
   {
      unsigned long reg;
      struct
      {
         unsigned p0         : 1;  /* D00 */
         unsigned p1         : 1;  /* D01 */
         unsigned p2         : 1;  /* D02 */
         unsigned p3         : 1;  /* D03 */
         unsigned            : 28; /* D04:31 */
      } bits;
   }boot_reg;                      // Boot-Mode-Pins

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 1;  /* D00 */
         unsigned p0         : 1;  /* D01 */
         unsigned p1         : 1;  /* D02 */
         unsigned p2         : 1;  /* D03 */
         unsigned p3         : 1;  /* D04 */
         unsigned p4         : 1;  /* D05 */
         unsigned p5         : 1;  /* D06 */
         unsigned            : 25; /* D07:31 */
      } bits;
   }config_reg;                    // ERTEC200-Config-Pins

   union
   {
      unsigned long reg;
      struct
      {
         unsigned wd_res_frei : 1; /* D00 */
         unsigned xres_soft  : 1;  /* D01 */
         unsigned en_wd_soft_res_irte : 1; /* D02 */
         unsigned pulse_dur  : 10; /* D03:12 */
         unsigned            : 19; /* D13:31 */
      } bits;
   }res_ctrl_reg;                  // Control register for reset

   union
   {
      unsigned long reg;
      struct
      {
         unsigned watchdog_reset : 1; /* D00 */
         unsigned software_reset : 1; /* D01 */
         unsigned poweron_hardware_reset : 1; /* D02 */
         unsigned            : 29; /* D03:31 */
      } bits;
   }res_stat_reg;                  // Status register for reset

   union
   {
      unsigned long reg;
      struct
      {
         unsigned lock       : 1;  /* D00 */
         unsigned loss       : 1;  /* D01 */
         unsigned int_lock   : 1;  /* D02 */
         unsigned int_loss   : 1;  /* D03 */
         unsigned            : 1;  /* D04 */
         unsigned int_qvz_emif : 1; /* D05 */
         unsigned            : 10; /* D06:15 */
         unsigned int_mask_lock : 1; /* D16 */
         unsigned int_mask_loss : 1; /* D17 */
         unsigned            : 14; /* D18:31 */
      } bits;
   }pll_stat_reg;                  // Status register for PLL

   unsigned long dummy1[4];

   unsigned long qvz_ahb_adr;         // Address of incorrect addressing on the multi-layer AHB

   union
   {
      unsigned long reg;
      struct
      {
         unsigned r_rw       : 1;  /* D00 */
         unsigned hsize      : 3;  /* D01:03 */
         unsigned hburts     : 3;  /* D04:06 */
         unsigned            : 25; /* D07:31 */
      } bits;
   }qvz_ahb_ctrl;                  // Control signals of incorrect addressing on the multi-layer AHB

   union
   {
      unsigned long reg;
      struct
      {
         unsigned arm946     : 1;  /* D00 */
         unsigned lub        : 1;  /* D01 */
         unsigned irte       : 1;  /* D02 */
         unsigned dma        : 1;  /* D03 */
         unsigned            : 28; /* D04:31 */
      } bits;
   }qvz_ahb_m;                     // Mast detection of incorrect addressing on the multi-layer AHB

   unsigned long qvz_apb_adr;         // Address of incorrect addressing at the APB

   unsigned long qvz_emif_adr;        // Address that led to the QVZ at EMIF

   unsigned long dummy2[2];

   union
   {
      unsigned long reg;
      struct
      {
         unsigned seg0       : 2;  /* D00:01 */
         unsigned            : 30; /* D02:31 */
      } bits;
   }mem_swap;                      // Memory Swapping in Segment 0 auf dem AHB

   unsigned long dummy3;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned arm9       : 1;  /* D00 */
         unsigned lbu        : 1;  /* D01 */
         unsigned irte       : 1;  /* D02 */
         unsigned arb_mode   : 1;  /* D03 */
         unsigned            : 28; /* D04:31 */
      } bits;
   }mem_lock_ctrl;                 // AHB-Master-Lock-Enable

   union
   {
      unsigned long reg;
      struct
      {
         unsigned emt_opt    : 9;  /* D00:08 */
         unsigned initram    : 1;  /* D09 */
         unsigned micebypass : 1;  /* D10 */
         unsigned dbgen      : 1;  /* D11 */
         unsigned gatetheclk : 1;  /* D12 */
         unsigned bigendian  : 1;  /* D13 */
         unsigned            : 18; /* D14:31 */
      } bits;
   }arm9_ctrl;                     // Control of ARM9 inputs

   union
   {
      unsigned long reg;
      struct
      {
         unsigned write_en   : 1;  /* D00 */
         unsigned            : 31; /* D01:31 */
      } bits;
   }arm9_we;                       // Write protection register for the register ARM9_CTRL

   union
   {
      unsigned long reg;
      struct
      {
         unsigned debug_id   : 8;  /* D00:07 */
         unsigned version_id : 8;  /* D08:15 */
         unsigned revision_id : 8; /* D16:23 */
         unsigned            : 8;  /* D24:31 */
      } bits;
   }ertec200_tag;                  // Tag number of the current circuit status

   union
   {
      unsigned long reg;
      struct
      {
         unsigned p1_phy_enb : 1;  /* D00 */
         unsigned p1_fx_mode : 1;  /* D01 */
         unsigned p1_phy_mode : 3; /* D02:04 */
         unsigned p1_automdixen : 1; /* D05 */
         unsigned            : 2;  /* D06:07 */
         unsigned p2_phy_enb : 1;  /* D08 */
         unsigned p2_fx_mode : 1;  /* D09 */
         unsigned p2_phy_mode : 3; /* D10:12 */
         unsigned p2_automdixen : 1; /* D13 */
         unsigned            : 2;  /* D14:15 */
         unsigned phy_res_sel : 1; /* D16 */
         unsigned            : 15; /* D17:31 */
      } bits;
   }phy_config;                    // Configuration of PHY1 and PHY2

   union
   {
      unsigned long reg;
      struct
      {
         unsigned p1_pwruprst : 1; /* D00 */
         unsigned            : 1;  /* D01 */
         unsigned p2_pwruprst : 1; /* D02 */
         unsigned            : 29; /* D03:31 */
      } bits;
   }phy_status;                    // Status of PHY1 and PHY2

   union
   {
      unsigned long reg;
      struct
      {
         unsigned enb        : 4;  /* D00:03 */
         unsigned            : 28; /* D04:31 */
      } bits;
   }handrad_ctrl;                  // Check the handwheel input filter

   union
   {
      unsigned long reg;
      struct
      {
         unsigned counter   : 16;  /* D00:15 */
         unsigned           : 16;  /* D16:31 */
      } bits;
   }handrad_counter1,
    handrad_counter2;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned clk        : 1;  /* D00 */
         unsigned            : 31; /* D01:31 */
      } bits;
   }uart_clk;
} ERTEC200_IO_SCRB;


/************** FCOUNT_BASE *******************************
definition of the registers in the
F-COUNTER Module starting @ 0x4000_2700
*************** FCOUNT_BASE *******************************/

typedef struct
{

   unsigned long f_counter_val;       // F-counter timer value

   union
   {
      unsigned long reg;
      struct
      {
         unsigned f_cnt_res_low : 16; /* D00:15 */
         unsigned f_cnt_res_high : 16; /* D16:31 */
      } bits;
   }f_counter_res;

} ERTEC200_IO_FCOUNT;


/************** ESMA2_BASE *******************************
definition of the registers in the
ESMA2 Module starting @ 0x4001_0000
*************** ESMA2_BASE *******************************/

typedef struct
{

   // Registerblock
   struct
   {
      unsigned short auftrag;          //0x0000
      unsigned short dataptr;          //0x0002
      unsigned short blockptr;         //0x0004
      unsigned short auftragsstatus;   //0x0006
      unsigned short stapelstart;      //0x0008
      unsigned short stapelstatus;     //0x000A
      unsigned short stapelfehler;     //0x000C
      unsigned short dummy1;           //0x000E
      unsigned short max_ausbau;       //0x0010
      unsigned short modul_anz;        //0x0012
      unsigned short dummy2;           //0x0014
      unsigned short dummy3;           //0x0016
      unsigned short paa_laenge;       //0x0018
      unsigned short lueckenreg;       //0x001A
      unsigned short event;            //0x001C
      unsigned short laufzeitreg;      //0x001E
      unsigned short baudrate;         //0x0020
      unsigned short steuer;           //0x0022
      unsigned short status;           //0x0024
      unsigned short kommando;         //0x0026
      unsigned short steuerwort_se;    //0x0028
      unsigned short se_daten_ptr;     //0x002A
      unsigned short dummy4;           //0x002C
      unsigned short dummy5;//48       //0x002E
      unsigned short fehleranzahl;     //0x0030
      unsigned short fehler_adr_1;     //0x0032
      unsigned short fehler_adr_2;     //0x0034
      unsigned short fehler_adr_3;     //0x0036
      unsigned short fehler_adr_4;     //0x0038
      unsigned short fehler_adr_5;     //0x003A
      unsigned short fehler_adr_6;     //0x003C
      unsigned short dummy6; //64      //0x003E
      unsigned short pae_event_anz;    //0x0040
      unsigned short dummy7[31];//128  //0x0042 - 0x007F -> End of the 1st line
      unsigned short dummy8[448];      //0x0080 - 0x03FF -> 896 bytes reserved

      //Register of the 2nd line
      unsigned short auftrag_Z2;       //0x0400
      unsigned short dataptr_Z2;       //0x0402
      unsigned short blockptr_Z2;      //0x0404
      unsigned short auftragsstatus_Z2; //0x0406
      unsigned short stapelstart_Z2;   //0x0408
      unsigned short stapelstatus_Z2;  //0x040A
      unsigned short stapelfehler_Z2;  //0x040C
      unsigned short dummy1_Z2;        //0x040E
      unsigned short max_ausbau_Z2;    //0x0410
      unsigned short modul_anz_Z2;     //0x0412
      unsigned short dummy2_Z2;        //0x0414
      unsigned short dummy3_Z2;        //0x0416
      unsigned short paa_laenge_Z2;    //0x0418
      unsigned short lueckenreg_Z2;    //0x041A
      unsigned short event_Z2;         //0x041C
      unsigned short laufzeitreg_Z2;   //0x041E
      unsigned short baudrate_Z2;      //0x0420
      unsigned short steuer_Z2;        //0x0422
      unsigned short status_Z2;        //0x0424
      unsigned short kommando_Z2;      //0x0426
      unsigned short steuerwort_se_Z2; //0x0428
      unsigned short se_daten_ptr_Z2;  //0x042A
      unsigned short dummy4_Z2;        //0x042C
      unsigned short dummy5_Z2;        //0x042E
      unsigned short fehleranzahl_Z2;  //0x0430
      unsigned short fehler_adr_1_Z2;  //0x0432
      unsigned short fehler_adr_2_Z2;  //0x0434
      unsigned short fehler_adr_3_Z2;  //0x0436
      unsigned short fehler_adr_4_Z2;  //0x0438
      unsigned short fehler_adr_5_Z2;  //0x043A
      unsigned short fehler_adr_6_Z2;  //0x043C
      unsigned short dummy6_Z2;        //0x043E
      unsigned short pae_event_anz_Z2; //0x0440
      unsigned short dummy7_Z2[31];    //0x0442 - 0x047F -> End of the 2nd line
      unsigned short dummy8_Z2[448];   //0x0480 - 0x07FF -> 896 bytes reserved
      unsigned short hw_kennung;       //0x0800
      unsigned short dummy9[63];       //0x0802 - 0x087F -> 126 bytes reserved
      unsigned short eingabeport;//2176 //0x00880
      unsigned short ausgabeport;      //0x00882
      unsigned short int_status0;      //0x00884
      unsigned short int_status1;//2182 //0x00886
      unsigned short reserved[956];    //0x00888 - 0x0FFF -> End of register area
   } ESMA2;

   // globaler RAM
   struct
   {
      unsigned short input_buf [128];  //0x1000, Input data for modules on the ES bus
      unsigned short output_buf[128];  //0x1100, Output data from modules on the ES bus
      unsigned short cmd_def   [256];  //0x1200, Command definition - storage of several consecutive bus commands (128 4-byte commands)
      unsigned short reserved  [1536]; //0x13FF
   } GLOBAL_RAM;

   // Zeile 1
   struct
   {
      unsigned short job_buf   [128];  //0x2000 (0x3000), Order buffer - universal data buffer for command-specific data
      unsigned short event_buf [128];  //0x2100 (0x3100), Event buffer - module-specific event buffer (ES bus), 2 bytes / module as PAE word
                                   //0x2180 (0x3180), Diagnostic buffer - module-specific diagnostic buffer (ES bus), 2 bytes / module
      unsigned short ea_def    [128];  //0x2200 (0x3200), I / O definition - storage of the configuration of the input / output buffer, 4 bytes / module
      unsigned short ev_state  [128];  //0x2300 (0x3300), Event list - events when processing the command stack 2 bytes / module + event address
      unsigned short cmd_stack [512];  //0x2400 (0x3400), Command stack - storage of frequently used command sequences
      unsigned short reserved  [1024]; //0x2800 (0x3800)
                                   // up to 0x2FFF (0x3FFF)
   } EXCL_RAM;

} ERTEC200_IO_ESMA2;


/************** ARM_ICU_BASE *******************************
definition of the registers in the
ARM_ICU Module starting @ 0x5000_0000
*************** ARM_ICU_BASE *******************************/

typedef struct
{

   union
   {
      unsigned long reg;
      struct
      {
         unsigned irvec      : 4;  /* D00:03 */
         unsigned vectorkennung : 28; /* D04:31 */
      } bits;
   }irvec;                         // Interrupt Vector Register

   union
   {
      unsigned long reg;
      struct
      {
         unsigned fivec      : 3;  /* D00:02 */
         unsigned vectorkennung : 29; /* D03:31 */
      } bits;
   }fivec;                         // Fast Interrupt Vector Register

   union
   {
      unsigned long reg;
      struct
      {
         unsigned prio       : 4;  /* D00:03 */
         unsigned            : 3;  /* D04:06 */
         unsigned aktiv      : 1;  /* D07 */
         unsigned            : 24; /* D08:31 */
      } bits;
   }lockreg;                       // Priority Lock Register

   union
   {
      unsigned long reg;
      struct
      {
         unsigned fiqsenable : 4;  /* D00:03 */
         unsigned            : 3;  /* D04:06 */
         unsigned fiqsreg    : 1;  /* D07 */
         unsigned            : 24; /* D08:31 */
      } bits;
   }fiq1reg,                       // Fast Interrupt Request 1 Select Register
    fiq2reg;                       // Fast Interrupt Request 2 Select Register

   union
   {
      unsigned long reg;
      struct
      {
         unsigned irvec      : 4;  /* D00:03 */
         unsigned vectorkennung : 28; /* D04:31 */
      } bits;
   }irqack;                        // Interrupt Vector Register with IRQ Acknowledge

   union
   {
      unsigned long reg;
      struct
      {
         unsigned fivec      : 3;  /* D00:02 */
         unsigned vectorkennung : 29; /* D03:31 */
      } bits;
   }fiqack;                        // Fast Interrupt Vector Register with FIQ Acknowledge

   union
   {
      unsigned long reg;
      struct
      {
         unsigned irclvec    : 4;  /* D00:03 */
         unsigned            : 28; /* D04:31 */
      } bits;
   }irclvec;                       // Interrupt Request Clear Vector

   union
   {
      unsigned long reg;
      struct
      {
         unsigned maskall   : 1;   /* D00 */
         unsigned           : 31;  /* D01:31 */
      } bits;
   }maskall;                       // Mask all Interrupts

   unsigned long irqend;              // End of  Interrupt

   unsigned long fiqend;              // End of  Interrupt

   union
   {
      unsigned long reg;
      struct
      {
         unsigned fiqpr      : 3;  /* D00:02 */
         unsigned            : 29; /* D03:31 */
      } bits;
   }fiqpr0,                        // FIQ Priority Register
    fiqpr1,
    fiqpr2,
    fiqpr3,
    fiqpr4,
    fiqpr5,
    fiqpr6,
    fiqpr7;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned wd         : 1;  /* D00 */
         unsigned apb        : 1;  /* D01 */
         unsigned ahb        : 1;  /* D02 */
         unsigned pll        : 1;  /* D03 */
         unsigned commrx     : 1;  /* D04 */
         unsigned commtx     : 1;  /* D05 */
         unsigned res0       : 1;  /* D06 */
         unsigned res1       : 1;  /* D07 */
         unsigned            : 24; /* D08:31 */
      } bits;
   }fiqisr,                        // FIQ In Service Register
    fiqirr,                        // FIQ Request Register
    fiqmaskreg;                    // Interrupt mask register for FIQ

   union
   {
      unsigned long reg;
      struct
      {
         unsigned timer0     : 1;  /* D00 */
         unsigned timer1     : 1;  /* D01 */
         unsigned gpio0      : 1;  /* D02 */
         unsigned gpio1      : 1;  /* D03 */
         unsigned gpio30     : 1;  /* D04 */
         unsigned gpio31     : 1;  /* D05 */
         unsigned timer2     : 1;  /* D06 */
         unsigned ssc        : 1;  /* D07 */
         unsigned uart       : 1;  /* D08 */
         unsigned phy        : 1;  /* D09 */
         unsigned spi1       : 1;  /* D10 */
         unsigned spi1_rec   : 1;  /* D11 */
         unsigned irte_h     : 1;  /* D12 */
         unsigned irte_l     : 1;  /* D13 */
         unsigned esma2      : 1;  /* D14 */
         unsigned dma        : 1;  /* D15 */
         unsigned            : 16; /* D16:31 */
      } bits;
   }irreg,                         // Interrupt Request Register
    maskreg,                       // Interrupt Mask Register
    isreg,                         // Interrupt Service Register
    trigreg,                       // Trigger Select Register
    edgereg,                       // Edge Select Register
    swirreg;                       // Software Interrupt Register

   union
   {
      unsigned long reg;
      struct
      {
         unsigned prio       : 4;  /* D00:03 */
         unsigned            : 28; /* D04:31 */
      } bits;
   }prioreg0,                      // Priority Register
    prioreg1,
    prioreg2,
    prioreg3,
    prioreg4,
    prioreg5,
    prioreg6,
    prioreg7,
    prioreg8,
    prioreg9,
    prioreg10,
    prioreg11,
    prioreg12,
    prioreg13,
    prioreg14,
    prioreg15;

} ERTEC200_IO_ARM_ICU;


/************** EMIF_REG_BASE *******************************
definition of the registers in the
EMIF_REGISTER Module starting @ 0x7000_0000
*************** EMIF_REG_BASE *******************************/

typedef struct
{

   union
   {
      unsigned long reg;
      struct
      {
         unsigned minor     : 8;    /* D00:07 */
         unsigned major     : 8;    /* D08:15 */
         unsigned           : 16;   /* D16:31 */
      } bits;
   }rev_code_stat;                 // Revision Code and Status Register

   union
   {
      unsigned long reg;
      struct
      {
         unsigned max_ext_wait : 8; /* D00:07 */
         unsigned            : 22; /* D08:29 */
         unsigned wp         : 1;  /* D30 */
         unsigned            : 1;  /* D31 */
      } bits;
   }async_wait_cycle_conf;         // Async Wait Cycle Config Register

   union
   {
      unsigned long reg;
      struct
      {
         unsigned pagesize   : 3;  /* D00:02 */
         unsigned            : 1;  /* D03 */
         unsigned ibank      : 3;  /* D04:06 */
         unsigned            : 1;  /* D07 */
         unsigned rows       : 3;  /* D08:10 */
         unsigned            : 2;  /* D11:12 */
         unsigned cl         : 1;  /* D13 */
         unsigned            : 18; /* D14:31 */
      } bits;
   }sdram_bank_conf;               // SDRAM Bank Config Register

   union
   {
      unsigned long reg;
      struct
      {
         unsigned rate       : 13; /* D00:12 */
         unsigned            : 16; /* D13:28 */
         unsigned init_done  : 1;  /* D29 */
         unsigned at         : 1;  /* D30 */
         unsigned            : 1;  /* D31 */
      } bits;
   }sdram_refresh_ctrl;            // Setting the refresh frequency

   union
   {
      unsigned long reg;
      struct
      {
         unsigned asize      : 2;  /* D00:01 */
         unsigned            : 2;  /* D02:03 */
         unsigned r_hold     : 3;  /* D04:06 */
         unsigned r_strobe   : 6;  /* D07:12 */
         unsigned r_su       : 4;  /* D13:16 */
         unsigned w_hold     : 3;  /* D17:19 */
         unsigned w_strobe   : 6;  /* D20:25 */
         unsigned w_su       : 4;  /* D26:29 */
         unsigned ew         : 1;  /* D30 */
         unsigned ews_xas    : 1;  /* D31 */
      } bits;
   }async_bank0_conf,              // Setting the timing and the data bus width
    async_bank1_conf,
    async_bank2_conf,
    async_bank3_conf;

   union
   {
      unsigned long reg;
      struct
      {
         unsigned            : 7;  /* D00:06 */
         unsigned atirq      : 1;  /* D07 */
         unsigned sdsize     : 1;  /* D08 */
         unsigned            : 5;  /* D09:13 */
         unsigned trcd_tcd   : 1;  /* D14 */
         unsigned            : 1;  /* D15 */
         unsigned burst_lenght : 2; /* D16:17 */
         unsigned            : 1;  /* D18 */
         unsigned test3      : 1;  /* D19 */
         unsigned            : 4;  /* D20:23 */
         unsigned asdb       : 1;  /* D24 */
         unsigned adb        : 1;  /* D25 */
         unsigned            : 3;  /* D26:28 */
         unsigned test2      : 1;  /* D29 */
         unsigned test1      : 1;  /* D30 */
         unsigned            : 1;  /* D31 */
      } bits;
   }extended_conf;                 // Setting additional functions

} ERTEC200_IO_EMIF_REG;


/************** DMA_REG_BASE *******************************
definition of the registers in the
DMA REGISTER Module starting @ 0x8000_0000
*************** DMA_REG_BASE *******************************/

typedef struct
{

   unsigned long dma_src_addr;        // Start address of the data block to be transferred from the DMA controller

   unsigned long dma_dest_addr;       // Destination address of the data block to be transferred by the DMA controller

   union
   {
      unsigned long reg;
      struct
      {
         unsigned byte_count : 16; /* D00:15 */
         unsigned s_delay    : 5;  /* D16:20 */
         unsigned d_delay    : 3;  /* D21:23 */
         unsigned            : 8;  /* D24:31 */
      } bits;
   }dma_ctrl;                      // To define the data block length

   union
   {
      unsigned long reg;
      struct
      {
         unsigned s_delay    : 4;  /* D00:03 */
         unsigned d_delay    : 4;  /* D04:07 */
         unsigned d_widht    : 3;  /* D08:10 */
         unsigned d_dma_requ : 3;  /* D11:13 */
         unsigned d_addr_mode : 2; /* D14:15 */
         unsigned s_widht    : 3;  /* D16:18 */
         unsigned s_dma_requ : 3;  /* D19:21 */
         unsigned s_addr_mode : 2; /* D22:23 */
         unsigned            : 3;  /* D24:26 */
         unsigned sync       : 2;  /* D27:28 */
         unsigned intr_enable : 1; /* D29 */
         unsigned            : 1;  /* D30 */
         unsigned start      : 1;  /* D31 */
      } bits;
   }dma_conf;                      // Control bits

} ERTEC200_IO_DMA_REG;


#endif  /* end of "#ifndef _LITTLE_ENDIAN" clause */


#else   /* else of #ifndef ERTEC200_H */
   #pragma message ("The header ERTEC200.H is included twice or more !")
#endif  /* end of "#ifndef ERTEC200_H" clause */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
