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
/*  F i l e               &F: eep_dat.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  header file                                                              */
/*  Routines for the internal EEPROM of the TEMIC T89C51RD2                  */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  H i s t o r y :                                                          */
/*                                                                           */
/*  date     person  comment                                                 */
/*  -------- ------  -------                                                 */
/*  23.07.01 MaSi  Function: EEPROM_GetManufacturerCode ()                   */
/*  16.05.03 KG    KG porting to MSP430 IAR                                  */
/*  05.08.03 MaSi  GET_EEPROM_BUSY () for MSP with FALSE (dummy)             */
/*  07.08.03 MaSi  Functions for MSP extended with I2C-EEPROM                */
/*  11.08.03 MaSi  Prototypes from p_in.h used                               */
/*  12.08.03 MaSi  Function: EEPROM_GetManufacturerCode () out               */
/*  27.08.03 MaSi  EEPROM addr. (0xa0) as define                             */
/*  08.09.03 MaSi  DAU addr. (0x9e / 0xc0) as define - provisional?          */
/*  08.01.04 MaSi  I2C data moved from P3.0 to P3.1 according to HW redesign */
/*  01.03.04 MaSi  Init function with return value                           */
/*  12.03.04 MaSi  Error entry in the EEPROM configurable                    */
/*  02.06.04 MaSi  first test implementation for new AT89C51ED2              */
/*  09.07.04 MaSi  MSP: pages sorted for test - KALI all the way back        */
/*  30.07.04 MaSi  MSP-8DI (TRACE): Show int lock                            */
/*  26.10.05 MaSi  EEPROM-specific constants and macros according to e_cfg.h */
/*  28.10.05 MaSi  Transfer EEPROM <-> Flash for Kali with MSP430 possible   */
/*  10.04.06 MaSi  Porting to TMS470 IAR                                     */
/*  11.09.07 MaSi  Init divided into ..._ init_PINs () and ..._ init ()      */
/*  05.10.07 MaSi  NEC850 added                                              */
/*  10.10.07 JaGa  New function EEPROM_Reset ()                              */
/*  21.02.08 MaSI  New functions EEPROM_readBlock_linear ()                  */
/*                               EEPROM_writeBlock_linear()                  */
/*                               EEPROM_compareBlock_linear()                */
/*                                                                           */ 
/*****************************************************************************/

#ifndef EEP_DAT_EXTERN_ATTR

    #define EEP_DAT_EXTERN_ATTR extern

#else

    #define EEP_DAT_EXTERN_ATTR_DECLARE

#endif


/* Check tool chain (controller)
------------------------------------------------------------------------- */
#ifndef TOOL_CHAIN_KEIL
  #ifndef MSP430_IAR
    #ifndef TMS470_IAR
      #ifndef NEC850_IAR
        #ifndef ERTEC_GHS
		  #ifndef ATMEL_SAM9X25
            #error "8051 or MSP430 or TMS470 or NEC850 or ATMEL_SAM9X25 must be defined !!"
          #endif
        #endif
      #endif
    #endif
  #endif
#endif

#ifdef TOOL_CHAIN_KEIL
  #ifdef MSP430_IAR
    #error "Do not define 8051 and MSP430 at the same time!"
  #endif
  #ifdef TMS430_IAR
    #error "Do not define 8051 and TMS470 at the same time!"
  #endif
  #ifdef NEC850_IAR
    #error "Do not define 8051 and NEC850 at the same time!"
  #endif
#endif
#ifdef MSP430_IAR
  #ifdef TOOL_CHAIN_KEIL
    #error "Do not define MSP430 and 8051 at the same time!"
  #endif
  #ifdef TMS430_IAR
    #error "Do not define MSP430 and TMS470 at the same time!"
  #endif
  #ifdef NEC850_IAR
    #error "Do not define MSP430 and NEC850 at the same time!"
  #endif
#endif
#ifdef TMS430_IAR
  #ifdef MSP430_IAR
    #error "Do not define TMS470 and MSP430 at the same time!"
  #endif
  #ifdef TOOL_CHAIN_KEIL
    #error "Do not define TMS430 and 8051 at the same time!"
  #endif
  #ifdef NEC850_IAR
    #error "Do not define TMS470 and NEC850 at the same time!"
  #endif
#endif
#ifdef NEC850_IAR
  #ifdef MSP430_IAR
    #error "Do not define NEC850 and MSP430 at the same time!"
  #endif
  #ifdef TOOL_CHAIN_KEIL
    #error "Do not define NEC850 and 8051 at the same time!"
  #endif
  #ifdef TMS430_IAR
    #error "Do not define NEC850 and TMS430 at the same time!"
  #endif
#endif


/* Test der Konfiguration
------------------------------------------------------------------------- */
#if (MAX_KALI_VALUES > 64)
  #error "too many calibration values specified in e_string.h"
#endif

#define MAX_KALI_INDEX  (MAX_KALI_VALUES - 1)


/* Plausibility checks for EEPROM_2_FLASH
------------------------------------------------------------------------- */
#ifdef EEPROM_2_FLASH_ENABLE
  #ifdef MSP430_IAR
  #else
    #error "EEPROM_2_FLASH_ENABLE only works with MSP430"
  #endif
#endif

#ifdef EEPROM_2_FLASH_ENABLE
  #if ( ANZ_KALI_VAL_4_FLASH < 0 )
    #error "ANZ_KALI_VAL_4_FLASH too small ((0), 1..32)"
  #endif

  #if ( ANZ_KALI_VAL_4_FLASH > /*32*/64 )
    #error "ANZ_KALI_VAL_4_FLASH too large (1..32 (64))"
  #endif

  #if ( FIRST_KALI_VAL_4_FLASH > (MAX_KALI_VALUES-1) )
    #error "FIRST_KALI_VAL_4_FLASH too large (0 .. (MAX_KALI_VALUES-1))"
  #endif

  #if ( (FIRST_KALI_VAL_4_FLASH+ANZ_KALI_VAL_4_FLASH) > MAX_KALI_VALUES )
    #error "FIRST_KALI_VAL_4_FLASH + ANZ_KALI_VAL_4_FLASH too large (0..MAX_KALI_VALUES)"
  #endif
#endif


/* Storage attributes
------------------------------------------------------------------------- */
#ifdef TOOL_CHAIN_KEIL

  #define E_CONST_DATA_ATTR code
  #define E_DAT_ALLG_ATTR   xdata
  #define E_DAT_INT_ATTR    data

#else

  #define E_CONST_DATA_ATTR       /* constant data in ROM */
  #define E_DAT_ALLG_ATTR         /* general working data in RAM */
  #define E_DAT_INT_ATTR          /* Work data in the internal RAM */

#endif


/* The EEPROM is logically divided into 64-byte pages, 
   regardless of how it is organized internally.
------------------------------------------------------------------------- */
#define PAGE_SIZE  64
#define ANZ_PAGES (I_M_PAGES + KALIBRIER_PAGES)

/* first free address in the EEPROM after the 64 byte page  
   for I&M data sets and calibration values
------------------------------------------------------------------------- */
#define FIRST_FREE_EEPROM_ADRESS (ANZ_PAGES * PAGE_SIZE)

/* Tests on Pages
------------------------------------------------------------------------- */
#ifndef ANZ_PAGES
  #error "ANZ_PAGES ist nicht definiert."
#endif

#define MAX_PAGES (EEPROM_SIZE/PAGE_SIZE) /* 1k * 8 has 16 (virtual) pages of 64 bytes each */

#if (ANZ_PAGES > MAX_PAGES) || (ANZ_PAGES == 0)
  #error "ANZ_PAGES too small or too big."
#endif

#if (FIRST_FREE_EEPROM_ADRESS >= EEPROM_SIZE)
  #error "ANZ_PAGES so big that there are no more free blocks."
#endif


#if !defined(EEPROM_ONE_BYTE_ADDRESS) && !defined(EEPROM_TWO_BYTE_ADDRESS) && !defined(EEPROM_THREE_BYTE_ADDRESS)
  #error "How wide is the address in the EEPROM?"
#endif
#if defined(EEPROM_ONE_BYTE_ADDRESS) && (defined(EEPROM_TWO_BYTE_ADDRESS) || defined(EEPROM_THREE_BYTE_ADDRESS))
  #error "How wide is the address in the EEPROM?!?"
#endif
#if defined(EEPROM_TWO_BYTE_ADDRESS) && defined(EEPROM_THREE_BYTE_ADDRESS)
  #error "How wide is the address in the EEPROM?!?"
#endif

#ifdef EEP_BIG_ENDIAN
  #ifdef EEP_LITTLE_ENDIAN
    #error "Define either EEP_BIG_ENDIAN or EEP_LITTLE_ENDIAN."
  #endif
  #if defined(EEPROM_ONE_BYTE_ADDRESS)
    #define EEPROM_ADDRESS_BYTE_0(address)   (((UBYTE*)&(address))[0])  /* Address byte to be sent as part of the device address */
    #define EEPROM_ADDRESS_BYTE_1(address)   (((UBYTE*)&(address))[1])  /* First pure address byte */
  #elif defined(EEPROM_TWO_BYTE_ADDRESS)
    /* Byte 0 at address & address is the most significant, but because it is always 0, it is not sent to the EEPROM */
    #define EEPROM_ADDRESS_BYTE_0(address)   (((UBYTE*)&(address))[1])  /* Address byte to be sent as part of the device address */
    #define EEPROM_ADDRESS_BYTE_1(address)   (((UBYTE*)&(address))[2])  /* First pure address byte */
    #define EEPROM_ADDRESS_BYTE_2(address)   (((UBYTE*)&(address))[3])  /* Second pure address byte */
  #elif defined(EEPROM_THREE_BYTE_ADDRESS)
    #define EEPROM_ADDRESS_BYTE_0(address)   (((UBYTE*)&(address))[0])  /* Address byte to be sent as part of the device address */
    #define EEPROM_ADDRESS_BYTE_1(address)   (((UBYTE*)&(address))[1])  /* First pure address byte */
    #define EEPROM_ADDRESS_BYTE_2(address)   (((UBYTE*)&(address))[2])  /* Second pure address byte */
    #define EEPROM_ADDRESS_BYTE_3(address)   (((UBYTE*)&(address))[3])  /* Third pure address byte */
  #else
    #error "More than 3 bytes of EEPROM addresses are not supported."
  #endif
#elif defined(EEP_LITTLE_ENDIAN)
  #if defined(EEPROM_ONE_BYTE_ADDRESS)
    #define EEPROM_ADDRESS_BYTE_0(address)   (((UBYTE*)&(address))[1])  /* Address byte to be sent as part of the device address */
    #define EEPROM_ADDRESS_BYTE_1(address)   (((UBYTE*)&(address))[0])  /* First pure address byte */
  #elif defined(EEPROM_TWO_BYTE_ADDRESS)
    #define EEPROM_ADDRESS_BYTE_0(address)   (((UBYTE*)&(address))[2])  /* Address byte to be sent as part of the device address */
    #define EEPROM_ADDRESS_BYTE_1(address)   (((UBYTE*)&(address))[1])  /* First pure address byte */
    #define EEPROM_ADDRESS_BYTE_2(address)   (((UBYTE*)&(address))[0])  /* Second pure address byte */
  #elif defined(EEPROM_THREE_BYTE_ADDRESS)
    #define EEPROM_ADDRESS_BYTE_0(address)   (((UBYTE*)&(address))[3])  /* Address byte to be sent as part of the device address */
    #define EEPROM_ADDRESS_BYTE_1(address)   (((UBYTE*)&(address))[2])  /* First pure address byte */
    #define EEPROM_ADDRESS_BYTE_2(address)   (((UBYTE*)&(address))[1])  /* Second pure address byte */
    #define EEPROM_ADDRESS_BYTE_3(address)   (((UBYTE*)&(address))[0])  /* Third pure address byte */
  #else
    #error "More than 3 bytes of EEPROM addresses are not supported."
  #endif
#else
  #error "Define EEP_LITTLE_ENDIAN or EEP_BIG_ENDIAN."
#endif


/* Makros
----------------------------------------------------------------------- */

/* ------------------------------------------------------ */
#ifdef ATMEL_SAM9X25

    #define GET_EEPROM_BUSY()	   (FALSE)
    #define START_EEPROM_WRITE() {}

    #define EEPROM_I2C_ADR       (0xa0 | EEPROM_HW_ADR)

    #define DAU_I2C_ADR_1        0x9e
    #define DAU_I2C_ADR_2        0xc0

    #ifdef INT_MESSAGE
      #define GET_INT_MESSAGE()     (int_message_bit)
      #define CLEAR_INT_MESSAGE()  {int_message_bit = 0;}
    #endif

#endif

/* ------------------------------------------------------ */
#ifdef TOOL_CHAIN_KEIL

  #ifdef AT89C51ED2

    #define GET_EEPROM_BUSY()	   (FALSE)
    #define START_EEPROM_WRITE() {}

  #else

    #define GET_EEPROM_BUSY()    (EECON & 0x01)
    #define START_EEPROM_WRITE() {EECON = 0x50; EECON = 0xa0;}

  #endif

#endif

/* ------------------------------------------------------ */
#ifdef MSP430_IAR

  #define GET_EEPROM_BUSY()	   (FALSE)
  #define START_EEPROM_WRITE() {}

    /* New 06.08.2003 MaSi: external EEPROM with I2C-SS - START */
    /* only on the MSP430 */
    /* WC_N:    Write Control P2.7  - is always the exit */
    /* I2C_CLK: Clock         P3.3  - is always the exit */
    /* SCL:     Data          P3.0  - is output or input, depending on the operating state */

    /* New 08.01.2004 MaSi: HW redesign -> port allocation */
    /* SCL:     Data          P3.1  - is output or input, depending on the operating state */
  /* --> now in _cfg\e_cfg.h*/

  #define EEPROM_I2C_ADR       (0xa0 | EEPROM_HW_ADR)

  #define DAU_I2C_ADR_1        0x9e
  #define DAU_I2C_ADR_2        0xc0

  #ifdef INT_MESSAGE
    #define GET_INT_MESSAGE()     (int_message_bit)
    #define CLEAR_INT_MESSAGE()  {int_message_bit = 0;}
  #endif

#endif

/* ------------------------------------------------------ */
#ifdef TMS470_IAR

  #define GET_EEPROM_BUSY()	   (FALSE)
  #define START_EEPROM_WRITE() {}

  #define EEPROM_I2C_ADR       (0xa0 | EEPROM_HW_ADR)

  #define DAU_I2C_ADR_1        0x9e
  #define DAU_I2C_ADR_2        0xc0

  #ifdef INT_MESSAGE
    #define GET_INT_MESSAGE()     (int_message_bit)
    #define CLEAR_INT_MESSAGE()  {int_message_bit = 0;}
  #endif

#endif


  /* ------------------------------------------------------ */
  #ifdef ERTEC_GHS

    #define GET_EEPROM_BUSY()    (FALSE)
    #define START_EEPROM_WRITE() {}

    #define EEPROM_I2C_ADR       (0xa0 | EEPROM_HW_ADR)

    #define DAU_I2C_ADR_1        0x9e
    #define DAU_I2C_ADR_2        0xc0

    #ifdef INT_MESSAGE
      #define GET_INT_MESSAGE()     (int_message_bit)
      #define CLEAR_INT_MESSAGE()  {int_message_bit = 0;}
    #endif

  #endif


/* ------------------------------------------------------ */
#ifdef NEC850_IAR

  #define GET_EEPROM_BUSY()	   (FALSE)
  #define START_EEPROM_WRITE() {}

  #define EEPROM_I2C_ADR       (0xa0 | EEPROM_HW_ADR)

  #define DAU_I2C_ADR_1        0x9e
  #define DAU_I2C_ADR_2        0xc0

  #ifdef INT_MESSAGE
    #define GET_INT_MESSAGE()     (int_message_bit)
    #define CLEAR_INT_MESSAGE()  {int_message_bit = 0;}
  #endif

#endif


/* defines
----------------------------------------------------------------------- */
/* Allocation of the pages of the EEPROM */
/* --> now in e_cfg.h */


/* Offsets of the toggles in the table of contents (Page 0) */
#define OFFS_KALI_TOGGLE     0
#define OFFS_PARA_TOGGLE     2
#define OFFS_DIR_TOGGLE      4
#define OFFS_SUBDIR_TOGGLE   6
#define OFFS_TL_1_TOGGLE     8
#define OFFS_TL_2_TOGGLE    10
#define OFFS_TL_3_TOGGLE    12
#define OFFS_TL_4_TOGGLE    14

/* Offsets of certain data */
#define OFFS_DS231_MAN_CODE 10

/* Repetition counter when writing EEPROM */
#define EEPROM_MAX_RETRY     3   /* a total of 3 attempts */

/* Error location when writing EEPROM */
#define EEPROM_ERROR_NO_ERROR        0
#define EEPROM_ERROR_DELETE          1
#define EEPROM_ERROR_INIT_STRING     2
#define EEPROM_ERROR_DIA_SZL         3
#define EEPROM_ERROR_KALI            4
#define EEPROM_ERROR_PARA_BLOCK      5
#define EEPROM_ERROR_PARA_INH_VERZ   6
#define EEPROM_ERROR_POWER_ON        7


#define PARAMETRIER_PAGE_0  DS128_BUF0_PAGE
#define PARAMETRIER_PAGE_1  DS128_BUF1_PAGE


/* for the 4-channel DAU on the 4AO_HART */
#define LOAD_MODE_0 0x00
#define LOAD_MODE_1 0x10
#define LOAD_MODE_2 0x20
#define LOAD_MODE_3 0x30


/* for req interface */

typedef enum eep_req_kind_tag
{
    EEP_REQ_KIND_IDLE   = 0,
    EEP_REQ_KIND_WRITE,
    EEP_REQ_KIND_READ

} EEP_REQ_KIND_TYPE;




/* Variable
----------------------------------------------------------------------- */
extern UBYTE page;

typedef struct eep_data_tag
{
    UBYTE write_error;
    UBYTE retry_counter;
    UWORD counter;

    struct
    {
        EEP_REQ_KIND_TYPE kind;
        Unsigned8   id;
        Unsigned8   data[PAGE_SIZE];
        Unsigned8   len;
        Unsigned32  count;

    } wr_req;

    struct
    {
        EEP_REQ_KIND_TYPE kind;
        Unsigned8   id;
        Unsigned8   *data_ptr;
        Unsigned8   len;
        Unsigned32  count;
        void        (*done_fct_ptr) (Unsigned8 rsp);

    }   rd_req;

    struct
    {
        Boolean   valid;
        Unsigned8 data[PAGE_SIZE];

    }   rd_teilliste[EEPR_ID_MAX];

    Unsigned8 rd_cmp_data[PAGE_SIZE];

} EEP_DATA_TYPE;

EEP_DAT_EXTERN_ATTR EEP_DATA_TYPE         eep_data;
EEP_DAT_EXTERN_ATTR UBYTE                 szl_i_a_buffer[MAX_LEN_I_A_SZL];    //temporary memory for I & A-SZL´s
EEP_DAT_EXTERN_ATTR LSA_FATAL_ERROR_TYPE  eep_errcb;

#ifdef INT_MESSAGE
  extern UBYTE int_message_bit;
#endif


#define EEP_ERR_MODULE_CHECK                  ((Unsigned8)0x01)
#define EEP_ERR_MODULE_DIA                    ((Unsigned8)0x02)
#define EEP_ERR_MODULE_DRV                    ((Unsigned8)0x03)

/* Prototypes
----------------------------------------------------------------------- */
void eep_in_fatal_error( Unsigned8 error_module, Unsigned16 error_line, Unsigned16 error_code);


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
