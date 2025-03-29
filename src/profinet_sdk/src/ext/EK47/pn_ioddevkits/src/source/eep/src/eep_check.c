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
/*  F i l e               &F: eep_check.c                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Functions, that use an EEPROM,                                           */
/*  used in peripheral modules for the ET200S                                */
/*                                                                           */
/*  Functions: (*** -> not yet implemented)                                  */
/*                                                                           */
/*  - EEPROM_init()                                                          */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  H i s t o r y :                                                          */
/*                                                                           */
/*  date    person  comment                                                  */
/*  -----------------------------------------------------------------------  */
/*  2003-05-22 KG Porting to MSP430                                          */
/*  07.08.03 MaSi I2C-EEPROM functions for MSP430                            */
/*  19.09.03 MaSi Preset the buffer at BlankCheck with 0                     */
/*  01.03.04 MaSi Init function with return value                            */
/*  12.03.04 MaSi Error entry in the EEPROM configurable                     */
/*  08.07.04 MaSi Make SET_WC () before SET_WC_OUT (),                       */
/*                otherwise writing is allowed                               */
/*  27.10.05 MaSi Configuration from now on in e_cfg.h                       */
/*  10.04.06 MaSi porting to TMS470                                          */
/*  14.12.06 MaSi Conditionally test INIT string for identical versions      */
/*  11.09.07 MaSi Init divided into ..._ init_PINs () and ..._ init ()       */
/*  10.10.07 JaGa EEPROM protocol reset in EEPROM_init_PINs ()               */
/*  19.10.07 MaSi MULTIPLE_INIT_STRING_READ                                  */
/*  28.02.08 MaSi erase and blank-check MAX_PAGES instead of ANZ_PAGES       */
/*                                                                           */
/*****************************************************************************/


#include "compiler.h"
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_DKSW_ATMEL)
    #include "eep_inc.h"

    #include "eep_dat.h"          /* EEPROM constants and defines */


    /* defines
    ------------------------------------------------------------------------- */
    #define INIT_STRING_PAGE  INHALTS_PAGE
    #define INIT_STRING_OFFS  32
    #define INIT_STRING_LEN   30
    #define WRITE_ERROR_PAGE  INHALTS_PAGE
    #define WRITE_ERROR_OFFS  (INIT_STRING_OFFS + INIT_STRING_LEN)

    #ifdef INIT_CHECK_FW_VERSION
      #define INIT_STRING_CHECK_LEN  (INIT_STRING_LEN)
    #else
      #define INIT_STRING_CHECK_LEN  (INIT_STRING_LEN-6)
    #endif


    /* Constants in ROM
       module-specific init string for the EEPROM
    ------------------------------------------------------------------------- */
    #include "eep_string.h"


    /* Variable
    ------------------------------------------------------------------------- */
    UWORD writeErrorVar;

    #ifdef MULTIPLE_INIT_STRING_READ
      #define MAX_INIT_STRING_READ 10
      UBYTE read_status;
      UBYTE read_count;
    #endif


    /* In-module prototypes
    ------------------------------------------------------------------------- */
    UBYTE EEPROM_blankCheck( void );


    /* ######################################################################

       Initialization functions

       ######################################################################
    ------------------------------------------------------------------------- */

    #ifdef TOOL_CHAIN_KEIL

    void EEPROM_init_PINs( UBYTE eetim_val )
    {

    #else

    void EEPROM_init_PINs( void )
    {

    #endif

      #ifdef TOOL_CHAIN_KEIL                     /* internal EEPROM of the TEMIC */

        #ifdef AT89C51ED2 /* new ATMEL with byte-by-letter writing */
          eetim_val = eetim_val; /* because of compiler */
        #else
          EETIM = eetim_val; /* old ATMEL with 64 byte page */
        #endif

      #else                               /* external EEPROM with I2C on the MSP430 */

        #ifdef EEPROM_WITH_PROTOCOL_RESET
          #ifdef HAVE_PRAEPROCESSOR_WARNING
            #warning "INFO: EEPROM-Protokoll-Reset bei Firmware-Start"
          #elif defined(HAVE_PRAEPROCESSOR_INFO)
            #info    "INFO: EEPROM-Protokoll-Reset bei Firmware-Start"
          #endif
          EEPROM_reset();
        #endif

        SET_I2C_CLK(); /* 21.07.2005 MaSi */
        SET_I2C_CLK_OUT();

        SET_I2C_DATA(); /* 21.07.2005 MaSi */
        SET_I2C_DATA_OUT(); /* 21.07.2005 MaSi */

        SET_WC(); /* 08.07.2004 MaSi */
        SET_WC_OUT();

      #endif
    }


    /* TRUE:  EEPROM has been deleted and reinitialized
              (First start-up after FW update)
       FALSE: EEPROM was not deleted ("more normal" Start)
    ------------------------------------------------------------------------- */
    UBYTE EEPROM_init( void )
    {
      UBYTE geloescht;

      geloescht = FALSE;

      /* Test whether the EEPROM already has logical content.
         If not, delete it and write the init string.
      ----------------------------------------------------------------------- */
      eep_data.write_error   = EEPROM_ERROR_NO_ERROR;
      eep_data.retry_counter = EEPROM_MAX_RETRY * 2; /* twice as much for the first start */

    #ifdef MULTIPLE_INIT_STRING_READ
      /* In order to make the start-up more robust, the reading attempt is repeated several times
         carried out. As soon as it went well, the loop is broken.
      ----------------------------------------------------------------------- */
      read_status = FALSE;

      for ( read_count = 0; read_count < MAX_INIT_STRING_READ; read_count++ )
      {
        read_status = EEPROM_check_InitString();

        if ( read_status )
          break;
      }

      if ( FALSE == read_status )
    #else
      if ( FALSE == EEPROM_check_InitString() )
    #endif
      {
        while ( eep_data.retry_counter )
        {
          eep_data.retry_counter--;

          if ( TRUE == EEPROM_blankCheck() )
          {
            EEPROM_write_InitString();

            /* Test whether it worked. If so, cancel the loop, otherwise repeat.
               If the counter has reached 0, enter an error.
            ------------------------------------------------------------------- */
            if ( TRUE == EEPROM_check_InitString() )
            {
              geloescht = TRUE;
              break;
            }
            else
            {
              if ( eep_data.retry_counter == 0 )
              {
                eep_data.write_error = EEPROM_ERROR_INIT_STRING;
                EEPROM_set_writeError();
              }
            }
          }
          else
          {
            EEPROM_delete();

            if ( eep_data.retry_counter == 0 )
            {
              eep_data.write_error = EEPROM_ERROR_DELETE;
              EEPROM_set_writeError();
            }
          }
        }
      }
      /* If the InitString read went well, test whether the writeError is set.
         In this case, user data operation is blocked.
         There is also a diagnosis in the cycle.
      ----------------------------------------------------------------------- */
      #ifdef EEPROM_ERROR_REMANENT

        if ( eeprom.write_error == EEPROM_ERROR_NO_ERROR )
        {
          if ( TRUE == EEPROM_get_writeError() )
          {
            eeprom.write_error = EEPROM_ERROR_POWER_ON;
          }
        }

      #endif

      return( geloescht );
    }


    /* ######################################################################

       Startup and testing functions

       ######################################################################
    ------------------------------------------------------------------------- */

    #if (MAX_LEN_I_A_SZL < PAGE_SIZE)
      #error "Achtung, SZL-Buffer ist zu klein fuer die folgenden Funktionen."
    #endif

    /* delete complete EEPROM
    ------------------------------------------------------------------------- */
    void EEPROM_delete( void )
    {
      {
        EEPROM_OFFSET_OPT xx;

        for ( xx = 0; xx < PAGE_SIZE; xx++ )
        {
          szl_i_a_buffer[xx] = 0xff;
        }
      }

      {
        EEPROM_PAGE_OPT xx;

        for ( xx = 0; xx < MAX_PAGES; xx++ )
        {
          #ifdef AT89C51ED2 // the PageWrite function is not available for the new ATMEL
            EEPROM_writeBlock( xx, 0, PAGE_SIZE, szl_i_a_buffer );
            WD_TRIGGERN; // the writing takes a little longer (10ms / byte)
          #else
            EEPROM_writePage( xx, szl_i_a_buffer );
          #endif

          START_EEPROM_WRITE();

          while ( GET_EEPROM_BUSY() )
          {
            ;
          }
        }
      }
    }

    /* Test complete EEPROM for "deleted"
    ------------------------------------------------------------------------- */
    UBYTE EEPROM_blankCheck( void )
    {
      EEPROM_PAGE_OPT   xx;
      EEPROM_OFFSET_OPT yy;

      for ( xx = 0; xx < MAX_PAGES; xx++ )
      {
        for ( yy = 0; yy < PAGE_SIZE; yy++ ) /* Preset buffer with 0x00 */
        {
          szl_i_a_buffer[yy] = 0x00;
        }

        EEPROM_readPage( xx, szl_i_a_buffer );

        for ( yy = 0; yy < PAGE_SIZE; yy++ ) /* Test buffer for 0xff */
        {
          if ( szl_i_a_buffer[yy] != 0xff )
          {
            return( FALSE );
          }
        }
      }
      return( TRUE );
    }

    /* Write the INIT string to the EEPROM.
       That should happen the first time the power is switched on.
       The init string is currently in the second half of the content page.
    ------------------------------------------------------------------------- */
    void EEPROM_write_InitString( void )
    {
      EEPROM_OFFSET_OPT xx;

      for ( xx = 0; xx < INIT_STRING_LEN; xx++ )
      {
        szl_i_a_buffer[xx] = eeprom_init_string[xx];
      }
      EEPROM_writeBlock( INIT_STRING_PAGE, INIT_STRING_OFFS, INIT_STRING_LEN, szl_i_a_buffer );

      START_EEPROM_WRITE();

      while ( GET_EEPROM_BUSY() )
      {
        ;
      }
    }

    /* Enter the identifier for the EEPROM write error.
       The error code is currently at the end of the content page.
       There is of course something daring to put an error info in 
       the faulty component itself but where else can it be stored in the case of power failure?
    ------------------------------------------------------------------------- */
    void EEPROM_set_writeError( void )
    {

    #ifdef EEPROM_ERROR_REMANENT

      EEPROM_writeWord( WRITE_ERROR_PAGE, WRITE_ERROR_OFFS, eeprom.write_error );

      START_EEPROM_WRITE();

      while ( GET_EEPROM_BUSY() )
      {
        ;
      }

    #endif

    }

    /* Test whether the EEPROM write error has been entered.
       TRUE - There was an error.
       FALSE - There was no error.
    ------------------------------------------------------------------------- */
    UBYTE EEPROM_get_writeError( void )
    {
      writeErrorVar = 0xaffe;

      EEPROM_readWord( WRITE_ERROR_PAGE, WRITE_ERROR_OFFS, &writeErrorVar );

      /* In the error-free case there is 0xffff in the EEPROM, if that is not in it, it is an error.
      ----------------------------------------------------------------------- */
      if ( writeErrorVar != 0xffff )
        return( TRUE );
      else
        return( FALSE );
    }

    /* Test whether the init string is entered.
       TRUE  - It's in there.
       FALSE - It is not in it.
    ------------------------------------------------------------------------- */
    UBYTE EEPROM_check_InitString( void )
    {
      EEPROM_OFFSET_OPT xx;

    #if 0 /* Test */
      for ( xx = 0; xx < INIT_STRING_LEN; xx++ )
      {
        szl_i_a_buffer[xx] = 0xa5; /* pre-set for test */
      }
    #endif
      EEPROM_readBlock( INIT_STRING_PAGE, INIT_STRING_OFFS, INIT_STRING_LEN, szl_i_a_buffer );

    //  for ( xx = 0; xx < INIT_STRING_LEN; xx++ )
      for ( xx = 0; xx < INIT_STRING_CHECK_LEN; xx++ )
      {
        if ( szl_i_a_buffer[xx] != eeprom_init_string[xx] )
          return( FALSE );
      }
      return( TRUE );
    }


    /* Test function
    ------------------------------------------------------------------------- */
    #ifdef EEPROM_SONDER_TEST

    typedef struct
    {
      UBYTE eeprom_page[PAGE_SIZE];
    } EEPROM_PAGE;

    //  UBYTE eeprom_sonder_buffer[EEPROM_SIZE];
    EEPROM_PAGE eeprom_buffer[ANZ_PAGES];

    UBYTE EEPROM_read2buffer( void )
    {
      EEPROM_PAGE_OPT   zz;
      EEPROM_OFFSET_OPT yy;

      for ( zz = 0; zz < ANZ_PAGES; zz++ )
      {
        for ( yy = 0; yy < PAGE_SIZE; yy++ ) /* Preset buffer with 0x55 */
        {
          eeprom_buffer[zz].eeprom_page[yy] = 0x55;
        }
        EEPROM_readPage( zz, &eeprom_buffer[zz].eeprom_page[0] );
      }
      return ( TRUE );
    }


    #endif /* EEPROM_SONDER_TEST */
#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
