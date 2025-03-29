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
/*  F i l e               &F: eep_com.h                                 :F&  */
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
/*****************************************************************************/


/*****************************************************************************/
/*****************************************************************************/
/*******************************                 *****************************/
/*******************************     COMMON      *****************************/
/*******************************                 *****************************/
/*****************************************************************************/
/*****************************************************************************/

/*****************************************************************************/
/* return values */

#define EEP_OK                              ((UnsignedOpt)0x01)

#define EEP_ERR_MASK                        ((UnsignedOpt)0x80)
#define EEP_ERR_NOT_SUPPORTED               ((UnsignedOpt)0x83)
#define EEP_ERR_RESOURCE                    ((UnsignedOpt)0x84)
#define EEP_ERR_PARAMETER                   ((UnsignedOpt)0x86)
#define EEP_ERR_SEQUENCE                    ((UnsignedOpt)0x87)



/*****************************************************************************/
/*****************************************************************************/
/*******************************                 *****************************/
/*******************************      SYSTEM     *****************************/
/*******************************                 *****************************/
/*****************************************************************************/
/*****************************************************************************/


/*****************************************************************************/
/* system service

    -> apma_version( version_len, version_ptr)
*/


/*
    version_ptr (call parameter) is LSA_VERSION_TYPE:
*/


typedef LSA_VERSION_TYPE  * EEP_SYS_VERSION_PTR;

#define EEP_LSA_COMPONENT_ID                   0x0000
#define EEP_LSA_PREFIX                         "    -EEP"
#define EEP_KIND                               /* &K */ 'P'  /* K& */
                                               /* preliminary: 'R': release       */
                                               /*              'C': correction    */
                                               /*              'S': spezial       */
                                               /*              'T': test          */
                                               /*              'B': labor         */
                                               /* prereleased: 'P': pilot         */
                                               /* released:    'V': version       */
                                               /*              'K': correction    */
                                               /*              'D': demonstration */
#define EEP_VERSION                            /* &V */ 0    /* V& */ /* [1 - 99] */
#define EEP_DISTRIBUTION                       /* &D */ 0    /* D& */ /* [0 - 99] */
#define EEP_FIX                                /* &F */ 1    /* F& */ /* [0 - 99] */
#define EEP_HOTFIX                             /* &H */ 0    /* H& */ /* [0]      */
#define EEP_PROJECT_NUMBER                     /* &P */ 0    /* P& */ /* [0 - 99] */
                                                             /* At LSA always 0!  */
#define EEP_INCREMENT                          /* &I */ 0    /* I& */ /* [1 - 99] */
#define EEP_INTEGRATION_COUNTER                /* &C */ 0    /* C& */ /* [1 - 99] */
#define EEP_GEN_COUNTER                        /* &G */ 0    /* G& */ /* [1]      */


/*****************************************************************************/
/*****************************************************************************/
/*******************************                 *****************************/
/*******************************       EEP       *****************************/
/*******************************                 *****************************/
/*****************************************************************************/
/*****************************************************************************/

/* CHIP specific sizes
------------------------------------------------------------------------- */


#if defined(EEPROM_24C01)
  #define EEPROM_INTERFACE_VERSION 2   /* Interface version of the driver */
  #define EEPROM_SIZE            128   /* EEPROM size in bytes */
  #define EEPROM_PAGE_SIZE         8   /* Size of an EEPROM internal page when writing */
  #define EEPROM_ONE_BYTE_ADDRESS      /* An address byte follows the device address */
#elif defined(EEPROM_24C02)
  #define EEPROM_INTERFACE_VERSION 2   /* Interface version of the driver */
  #define EEPROM_SIZE            256   /* EEPROM size in bytes */
  #define EEPROM_PAGE_SIZE         8   /* Size of an EEPROM internal page when writing */
  #define EEPROM_ONE_BYTE_ADDRESS      /* An address byte follows the device address */
#elif defined(EEPROM_24C04)
  #define EEPROM_INTERFACE_VERSION 2   /* Interface version of the driver */
  #define EEPROM_SIZE            512   /* EEPROM size in bytes */
  #define EEPROM_PAGE_SIZE        16   /* Size of an EEPROM internal page when writing */
  #define EEPROM_ONE_BYTE_ADDRESS      /* An address byte follows the device address */
#elif defined(EEPROM_24C08)
  #define EEPROM_INTERFACE_VERSION 2   /* Interface version of the driver */
  #define EEPROM_SIZE           1028   /* EEPROM size in bytes */
  #define EEPROM_PAGE_SIZE        16   /* Size of an EEPROM internal page when writing */
  #define EEPROM_ONE_BYTE_ADDRESS      /* An address byte follows the device address */
#elif defined(EEPROM_24C16)
  #define EEPROM_INTERFACE_VERSION 2   /* Interface version of the driver */
  #define EEPROM_SIZE           2048   /* EEPROM size in bytes */
  #define EEPROM_PAGE_SIZE        16   /* Size of an EEPROM internal page when writing */
  #define EEPROM_ONE_BYTE_ADDRESS      /* Nach der Device-Adresse kommt ein Adress-Byte */
#elif defined(EEPROM_24C32)
  #define EEPROM_INTERFACE_VERSION 2   /* Interface version of the driver */
  #define EEPROM_SIZE           4096   /* EEPROM size in bytes */
  #define EEPROM_PAGE_SIZE        32   /* Size of an EEPROM internal page when writing */
  #define EEPROM_TWO_BYTE_ADDRESS      /* Two address bytes follow the device address */
#elif defined(EEPROM_24C64)
  #define EEPROM_INTERFACE_VERSION 2   /* Interface version of the driver */
  #define EEPROM_SIZE           8192   /* EEPROM size in bytes */
  #define EEPROM_PAGE_SIZE        32   /* Size of an EEPROM internal page when writing */
  #define EEPROM_TWO_BYTE_ADDRESS      /* Two address bytes follow the device address */
#elif defined(EEPROM_24C128)
  #define EEPROM_INTERFACE_VERSION 2   /* Interface version of the driver */
  #define EEPROM_SIZE          16384   /* EEPROM size in bytes */
  #define EEPROM_PAGE_SIZE        64   /* Size of an EEPROM internal page when writing */
  #define EEPROM_TWO_BYTE_ADDRESS      /* Two address bytes follow the device address */
#elif defined(EEPROM_24C256)
  #define EEPROM_INTERFACE_VERSION 2   /* Interface version of the driver */
  #define EEPROM_SIZE          32768   /* EEPROM size in bytes */
  #define EEPROM_PAGE_SIZE        64   /* Size of an EEPROM internal page when writing */
  #define EEPROM_TWO_BYTE_ADDRESS      /* Two address bytes follow the device address */
#elif defined(EEPROM_24C512)
  #define EEPROM_INTERFACE_VERSION 3   /* Interface version of the driver */
  #define EEPROM_SIZE          65536   /* EEPROM size in bytes */
  #define EEPROM_PAGE_SIZE        64   /* Size of an EEPROM internal page when writing */
  #define EEPROM_TWO_BYTE_ADDRESS      /* Two address bytes follow the device address */
#endif



/* Tests bezueglich Pages
------------------------------------------------------------------------- */

#if !defined(EEPROM_INTERFACE_VERSION)  ||  (EEPROM_INTERFACE_VERSION < 2)
  typedef UBYTE  EEPROM_PAGE_OPT;
  typedef UBYTE  EEPROM_OFFSET_OPT;

  #define EEPROM_PAGE_SIZE        16   /* Size of an EEPROM internal page when writing */
  #define EEPROM_ONE_BYTE_ADDRESS      /* Usually only one address byte after device address */
#elif (EEPROM_INTERFACE_VERSION == 3)
  // ... EEPROM_24C512
#endif


#ifdef EEPROM_ONE_BYTE_ADDRESS
  typedef Unsigned8    EEPROM_PAGE_OPT;
#elif defined(EEPROM_TWO_BYTE_ADDRESS)
  typedef Unsigned16   EEPROM_PAGE_OPT;
#elif defined(EEPROM_THREE_BYTE_ADDRESS)
  typedef Unsigned32   EEPROM_PAGE_OPT;
#endif

#if (EEPROM_INTERFACE_VERSION == 3)
  typedef Unsigned16   EEPROM_OFFSET_OPT;
#else
	#if (PAGE_SIZE <= 255)
	  typedef Unsigned8   EEPROM_OFFSET_OPT;
	#elif (PAGE_SIZE <= 65535)
	  typedef Unsigned16   EEPROM_OFFSET_OPT;
	#else
	  #error
	#endif
#endif



/* Defines
----------------------------------------------------------------------- */

#define EEPR_ID_TEILLISTE1  0
#define EEPR_ID_TEILLISTE2  1
#define EEPR_ID_TEILLISTE3  2
#define EEPR_ID_TEILLISTE4  3

#define EEPR_ID_MAX         4


#define EEPR_ID_TEILLISTE1_LENGTH   ((Unsigned16)56) /* same as APMA_IUM_0_BLOCK_LENGTH */
#define EEPR_ID_TEILLISTE2_LENGTH   ((Unsigned16)56) /* same as APMA_IUM_1_BLOCK_LENGTH */
#define EEPR_ID_TEILLISTE3_LENGTH   ((Unsigned16)18) /* same as APMA_IUM_2_BLOCK_LENGTH */
#define EEPR_ID_TEILLISTE4_LENGTH   ((Unsigned16)56) /* same as APMA_IUM_3_BLOCK_LENGTH */



/* Funktionen
----------------------------------------------------------------------- */

  #ifdef TOOL_CHAIN_KEIL
  void EEPROM_init_PINs( UBYTE eetim_val );
#else
  void EEPROM_init_PINs( void );
#endif

UBYTE EEPROM_init( void );

UBYTE EEPROM_readWord(  EEPROM_PAGE_OPT page, EEPROM_OFFSET_OPT offs, UWORD  *pt );
UBYTE EEPROM_readBlock( EEPROM_PAGE_OPT page, EEPROM_OFFSET_OPT offs, EEPROM_OFFSET_OPT len, UBYTE  *pt );
UBYTE EEPROM_readPage(  EEPROM_PAGE_OPT page, UBYTE  *pt );

#ifndef AT89C51ED2
  UBYTE EEPROM_writeWord( EEPROM_PAGE_OPT page, EEPROM_OFFSET_OPT offs, UWORD writeWord );
  UBYTE EEPROM_writePage( EEPROM_PAGE_OPT page, UBYTE  *pt );
#endif
UBYTE EEPROM_writeBlock( EEPROM_PAGE_OPT page, EEPROM_OFFSET_OPT offs, EEPROM_OFFSET_OPT len, UBYTE  *pt );

UBYTE EEPROM_compareBlock( EEPROM_PAGE_OPT page, EEPROM_OFFSET_OPT offs, EEPROM_OFFSET_OPT len, UBYTE  *pt );

#ifndef AT89C51ED2
  UBYTE EEPROM_readBlock_linear( EEPROM_OFFSET_OPT adr, EEPROM_OFFSET_OPT len, UBYTE  *pt );
  UBYTE EEPROM_writeBlock_linear( EEPROM_OFFSET_OPT adr, EEPROM_OFFSET_OPT len, UBYTE  *pt );
  UBYTE EEPROM_compareBlock_linear( EEPROM_OFFSET_OPT adr, EEPROM_OFFSET_OPT len, UBYTE  *pt );
#endif

#ifndef TOOL_CHAIN_KEIL
  UBYTE EEPROM_waitForAck( void );
  void  EEPROM_reset( void );

  #ifdef EEPROM_SONDER_TEST
    void EEPROM_invalid_access( void );
  #endif /* EEPROM_SONDER_TEST */
#endif

#ifdef EEPROM_2_FLASH_ENABLE
  #ifdef MSP430_IAR
    UBYTE EEPROM_Kali2Flash( void );
    UBYTE EEPROM_Kali2Eeprom( void );
    ULONG EEPROM_FlashReadValue( UBYTE index );
  #endif
#endif



void         eep_init              ( void);
void         eep_open              ( void);
UnsignedOpt  eep_write_req         ( UBYTE id, UBYTE P_DAT_DIA_ATTR *src_ptr, UBYTE len);
Boolean      eep_read_req          ( UBYTE id, void (*done_fct_ptr) (Unsigned8  resp));
Boolean      eep_read_sync         ( UBYTE id, UBYTE P_DAT_DIA_ATTR *dst_ptr, UBYTE *len);
void         eep_perform_services  ( void);
UBYTE        EEP_Write             ( UBYTE id, UBYTE  *src_ptr, UBYTE offset, UBYTE len);
UBYTE        EEP_Read              ( UBYTE id, UBYTE  *dest_ptr, UBYTE offset, UBYTE len);



UBYTE EEPROM_writeKaliWert_sync( UBYTE number, ULONG value );
ULONG EEPROM_readKaliWert_sync( UBYTE number );


#ifdef PARAMETER_IM_EEPROM
  UBYTE EEPROM_writeParaBlock_sync( UBYTE len, UBYTE E_DAT_ALLG_ATTR *ptr );
  UBYTE EEPROM_readParaBlock_sync( UBYTE len, UBYTE E_DAT_ALLG_ATTR *ptr );
#endif


void  EEPROM_delete( void );
void  EEPROM_write_InitString( void );
UBYTE EEPROM_check_InitString( void );
void  EEPROM_set_writeError( void );
UBYTE EEPROM_get_writeError( void );


/* for the 4-channel DAU on the 4AO_HART */
#ifdef MSP430_IAR
  void  DAU_init( void );
  UBYTE DAU_writeOutputChannel( UBYTE channel, UWORD outWord );
#endif


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/

