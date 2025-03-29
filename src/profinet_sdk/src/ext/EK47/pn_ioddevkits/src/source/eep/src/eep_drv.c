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
/*  F i l e               &F: eep_drv.c                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Treatment functions for the internal EEPROM of the T89C51RD2,            */
/*  and an external I2C-EEPROM on the MSP430, TMS470, NEC V850,              */
/*  used in peripheral modules for the ET200S / iS / iSP                     */
/*                                                                           */
/*  In addition, based on the I2C basic functions, there are still more      */
/*  Functions for controlling a 4-channel DAU provided.                      */
/*  It is used in the 4AO_HART.                                              */
/*                                                                           */
/*                                                                           */
/*  Functions: (*** -> not implemented yet)                                  */
/*                                                                           */
/*  - EEPROM_readWord()                                                      */
/*  - EEPROM_readBlock()                                                     */
/*  - EEPROM_readPage()                                                      */
/*                                                                           */
/*  - EEPROM_writeWord()                                                     */
/*  - EEPROM_writeBlock()                                                    */
/*  - EEPROM_writePage()                                                     */
/*                                                                           */
/*  - EEPROM_compareBlock()                                                  */
/*                                                                           */
/*  - EEPROM_readBlock_linear()                                              */
/*  - EEPROM_writeBlock_linear()                                             */
/*                                                                           */
/*  - DAU_init()                                                             */
/*  - DAU_writeOutputChannel()                                               */
/*                                                                           */
/*****************************************************************************/

#include "compiler.h"
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_DKSW_ATMEL)
    #include "eep_inc.h"

    #include "eep_dat.h"       /* EEPROM constants and defines */


    #ifdef ATMEL_SAM9X25

	    #include <cyg/hal/hal_io.h>			// GPIO access

	    inline Boolean HAL_ARM_AT91_GPIO_GET_func(void)
	    {
		    Boolean value;
		    HAL_ARM_AT91_GPIO_GET (AT91_GPIO_PA30, value);
		    return value;
	    }

    #endif

    /* defines / checks
    ------------------------------------------------------------------------- */

    #if 0
    #if ( INT_SPERRE_RD == 0 )
      #ifdef TMS470_IAR
        /* oK */
        #warning "ACHTUNG: EEPROM reading without interrupt lock"
      #elif defined NEC850_IAR
        /* oK */
        // #warning "ATTENTION: EEPROM reading without interrupt lock"
      #else
        #error "e_drv.c: INT_SPERRE_RD == 0 nur bei TMS470!"
      #endif
    #elif ( INT_SPERRE_RD == 1 )
      #ifdef MSP430_IAR
        #error "e_drv.c: INT_SPERRE_RD == 1 nicht bei MSP430!"
      #endif
      #ifdef TMS470_IAR
        #error "e_drv.c: INT_SPERRE_RD == 1 nicht bei MSP430!"
      #endif
      #define LOCAL_DI_RD
    #elif ( INT_SPERRE_RD == 2 )
      #define GLOBAL_DI_RD
    #else
      #error "e_drv.c: INT_SPERRE_RD not defined or incorrectly defined!"
    #endif

    #if ( INT_SPERRE_WR == 0 )
      #ifdef TMS470_IAR
        /* oK */
        #warning "ACHTUNG: EEPROM writing without interrupt lock"
      #elif defined NEC850_IAR
        /* oK */
        // #warning "ATTENTION: EEPROM writing without interrupt lock"
      #else
        #error "e_drv.c: INT_SPERRE_WR == 0 nur bei TMS470!"
      #endif
    #elif ( INT_SPERRE_WR == 1 )
      #ifdef MSP430_IAR
        #error "e_drv.c: INT_SPERRE_WR == 1 nicht bei MSP430!"
      #endif
      #ifdef TMS470_IAR
        #error "e_drv.c: INT_SPERRE_WR == 1 nicht bei MSP430!"
      #endif
      #define LOCAL_DI_WR
    #elif ( INT_SPERRE_WR == 2 )
      #define GLOBAL_DI_WR
    #else
      #error "e_drv.c: INT_SPERRE_WR not defined or incorrectly defined!"
    #endif
    #endif

    /* I2C applies to the MSP430 */
    #ifdef MSP430_IAR
      #if ( (EEPROM_I2C_ADR & 0xF0 ) == 0xa0 )
      #else
        #error "e_drv.c: EEPROM_I2C_ADR incorrectly defined! (must be 0xa0)"
      #endif

      #if ( DAU_I2C_ADR_1 == 0x9e )
      #else
        #error "e_drv.c: DAU_I2C_ADR_1 incorrectly defined! (must be 0x9e)"
      #endif

      #if ( DAU_I2C_ADR_2 == 0xc0 )
      #else
        #error "e_drv.c: DAU_I2C_ADR_2 incorrectly defined! (must be 0xc0)"
      #endif

      #if ( DAU_I2C_ADR_1 == EEPROM_I2C_ADR )
        #error "e_drv.c: DAU_I2C_ADR_1 and EEPROM_I2C_ADR defined the same!"
      #endif
    #endif

    /* I2C gilt beim TMS470 */
    #ifdef TMS470_IAR
      #if ( (EEPROM_I2C_ADR & 0xF0 ) == 0xa0 )
      #else
        #error "e_drv.c: EEPROM_I2C_ADR incorrectly defined! (must be 0xa0)"
      #endif

      #if ( DAU_I2C_ADR_1 == 0x9e )
      #else
        #error "e_drv.c: DAU_I2C_ADR_1 incorrectly defined! (must be 0x9e)"
      #endif

      #if ( DAU_I2C_ADR_2 == 0xc0 )
      #else
        #error "e_drv.c: DAU_I2C_ADR_2 incorrectly defined! (must be 0xc0)"
      #endif

      #if ( DAU_I2C_ADR_1 == EEPROM_I2C_ADR )
        #error "e_drv.c: DAU_I2C_ADR_1 and EEPROM_I2C_ADR defined the same!"
      #endif
    #endif


    /* Variable
    ------------------------------------------------------------------------- */
    #ifdef INT_MESSAGE
      #define SET_INT_MESSAGE() {int_message_bit = 1;}
      UBYTE int_message_bit;
    #else
      #define SET_INT_MESSAGE()
    #endif

    #ifdef TOOL_CHAIN_KEIL

      #if defined(EEPROM_ONE_BYTE_ADDRESS)
        UWORD E_DAT_INT_ATTR adress;
      #elif defined(EEPROM_TWO_BYTE_ADDRESS) || defined(EEPROM_THREE_BYTE_ADDRESS)
        ULONG E_DAT_INT_ATTR adress;
      #endif
      UWORD E_DAT_INT_ATTR value;

    #else

      /* interne Prototypen
      ----------------------------------------------------------------------- */
      #ifdef TMS470_IAR
        void wait_n_mal_1us( UBYTE anz_us );
      #endif

      void  I2C_write( UBYTE b );
      UBYTE I2C_read( void );
      void  I2C_startCond( void );
      void  I2C_stopCond( void );
      UBYTE I2C_getack( void );
      void  I2C_setack( void );
      void  I2C_noack( void );

      UBYTE I2C_buffer; /* global send and receive byte */

      #if defined(EEPROM_ONE_BYTE_ADDRESS)
        UWORD adress;
      #elif defined(EEPROM_TWO_BYTE_ADDRESS) || defined(EEPROM_THREE_BYTE_ADDRESS)
        ULONG adress;
      #endif
      UWORD value;
      UBYTE temp_buffer;
      UBYTE ack_byte;
      UBYTE first_page_anz;
      UBYTE last_page_anz;
      UBYTE page_count;
      //UBYTE i;
      UWORD e_timeout;

    #endif



    /* ######################################################################

       Read functions

       ######################################################################
    ------------------------------------------------------------------------- */

    /* Read a word from the EEPROM.
       Input: page and offset of the memory cell (theoretically the linear address also works), 
              pointer to target area

              JaGa: So far, the linear address has been checked,
              that it doesn't exceed the first page boundary, 
              so practically the linear address doesn't work.

       ATTENTION (applies to all functions of the TEMIC):
       ##############################################
       It is necessary to enter the EEE bit in the EECON register here
       Activate function immediately before accessing the EEPROM,
       otherwise all other movx commands that use the DPTR would access the EEPROM instead of XDATA.

       ATTENTION: Routine toggles the DPTR. In the end, the same DPTR must be set as when it was entered.
    ------------------------------------------------------------------------- */
    UBYTE EEPROM_readWord( EEPROM_PAGE_OPT page, EEPROM_OFFSET_OPT offs, UWORD E_DAT_ALLG_ATTR *pt )
    {
    #ifdef TOOL_CHAIN_KEIL

      /* Area test (Can be omitted for runtime optimization after no errors have occurred in the test phase.)
      ----------------------------------------------------------------------- */
      #ifdef EEPROM_TEST
        /* Here logical pages are still used, 
         * which are independent of the internal organization of the EEPROM. */
        if ( (page >= MAX_PAGES) || (offs > (PAGE_SIZE-2)) )
        {
          return ( FALSE );
        }
      #endif

      #ifdef GLOBAL_DI_RD
        DISABLE; /* EEPROM access always under global INT lock */
      #endif

      /* Assemble the address and load it into the DPTR */
      adress = page;
      adress = adress << 6;
      adress = adress | offs;

      #ifndef EEPROM_ONE_BYTE_ADDRESS
        #error "Vorerst nur 1-Byte-Adressen (wie frueher) erlaubt."
      #endif

      #pragma ASM
        #ifdef LOCAL_DI_RD
          clr EA        ; /* EEPROM access always under INT lock */
        #endif
        inc auxr1       ; DPTR umschalten
        mov dph,adress  ; hi- vor lo-Byte
        mov dpl,adress+1

        mov eecon,#2    ; EEPROM aktivieren
        movx a,@dptr    ; EEPROM lesen
        mov value,a     ; und abspeichern
        inc dptr        ; naechste Adresse einstellen
        movx a,@dptr    ; EEPROM lesen
        mov value+1,a   ; und abspeichern
        mov eecon,#0    ; EEPROM deaktivieren
        inc auxr1       ; DPTR umschalten
        #ifdef LOCAL_DI_RD
          setb EA       ; /* EEPROM access always under INT lock */
        #endif
      #pragma ENDASM

      /* Write the read word in the target area */
      *pt = value;

      #ifdef GLOBAL_DI_RD
        ENABLE;
      #endif

      return ( TRUE );

    #else

      /* So far, a word has been read from the EEPROM and put together from two bytes
         as if it were always stored in the EEPROM in little endian format 
         (Intel, MSP, ...). This is dangerous because the user may want 
         to read several words at once with the EEPROM_readBlock () function 
         and is wondering why this leads to byte salad on big endian CPUs (TMS470) 
         when he calls EEPROM_ {read, write } Block () and EEPROM_ {read, write} Word () 
         mixes.
         Therefore, the word is now arranged in the EEPROM as it corresponds 
         to the natural sequence of the CPU, i.e. Order in the EEPROM 
         is the same order in RAM / Flash. */
      return EEPROM_readBlock(page, offs, sizeof(UWORD), (UBYTE*)pt);
    #endif
    }


    /* Read a block (<= 64 bytes) from the EEPROM, only permitted within one page.
       (theoretically beyond the page limitations, but not necessary for the application, 
       therefore limitation to page granularity)

       Input: page, offset, length, pointer to target area

       REGISTER: A, DPTR, AUXR1, EECON

       ATTENTION: Routine toggles the DPTR. In the end, the same DPTR must be set as when it was entered.
    ------------------------------------------------------------------------- */
    UBYTE EEPROM_readBlock( EEPROM_PAGE_OPT page, EEPROM_OFFSET_OPT offs, EEPROM_OFFSET_OPT len, UBYTE E_DAT_ALLG_ATTR *pt )
    {
    #ifdef TOOL_CHAIN_KEIL
      UBYTE i;
    #endif

      /* Area test (Can be omitted for runtime optimization after no errors have occurred in the test phase.)
      ----------------------------------------------------------------------- */
      #ifdef EEPROM_TEST
        if ( (page >= MAX_PAGES) || (offs >= PAGE_SIZE) || (len > (PAGE_SIZE - offs)) )
        {
          return ( FALSE );
        }
      #endif

    #ifdef TOOL_CHAIN_KEIL

      #ifdef GLOBAL_DI_RD
        DISABLE; /* EEPROM access always under global INT lock */
      #endif

      i = len; /* for() uses the DPTR to load len (5.5 compiler) */

      /* Assemble the (source) address in the EEPROM and load it into the DPTR */
      adress = page;
      adress = adress << 6;
      adress = adress | offs;

      #ifndef EEPROM_ONE_BYTE_ADDRESS
        #error "For the time being only 1-byte addresses (as before) allowed."
      #endif

      #pragma ASM
        mov dph,adress    ; hi- before lo-Byte
        mov dpl,adress+1
      #pragma ENDASM

      #pragma ASM
        inc auxr1         ; Switch DPTR
      #pragma ENDASM

      /* Assemble the (target) address in the XDATA and load it into the DPTR */
      adress = pt;

      #pragma ASM
        mov dph,adress    ; hi- vor lo-Byte
        mov dpl,adress+1
      #pragma ENDASM

      /* Loop over len byte */
    //  for ( i = 0; i < len; i++ )
      while ( i-- )
      {
        #pragma ASM
          #ifdef LOCAL_DI_RD
            clr EA        ; /* EEPROM access always under INT lock */
          #endif
          inc auxr1       ; DPTR umschalten

          mov eecon,#2    ; EEPROM aktivieren
          movx a,@dptr    ; EEPROM lesen
          mov eecon,#0    ; EEPROM deaktivieren
          inc dptr        ; DPTR++

          inc auxr1       ; DPTR umschalten

          movx @dptr,a    ; und abspeichern
          inc dptr        ; DPTR++
          #ifdef LOCAL_DI_RD
            setb EA       ; /* EEPROM access always under INT lock */
          #endif
        #pragma ENDASM
      }

      #pragma ASM
        inc auxr1         ; DPTR umschalten
      #pragma ENDASM

      #ifdef GLOBAL_DI_RD
        ENABLE;
      #endif

      return ( TRUE );

    #else

      #ifdef GLOBAL_DI_RD
        DISABLE; /* EEPROM access always under global INT lock */
        SET_INT_MESSAGE();
      #endif

      ack_byte = 1; /* To indicate that something went wrong */

      /* Determine linear address
      ----------------------------------------------------------------------- */
      adress = page;
      adress = adress << 6;
      adress = adress | offs;


      /* The area test was at the beginning of the function.
      ----------------------------------------------------------------------- */
      do
      {
        SET_WC();  /* 08.07.2004 MaSi: set write protection for security */

        I2C_startCond();

        I2C_write( EEPROM_I2C_ADR | (EEPROM_ADDRESS_BYTE_0(adress) << 1) ); /* (first part of) address (WITH WRITE !!!!) */
        ack_byte = I2C_getack();
        if ( ack_byte )
          break;

        #if defined(EEPROM_ONE_BYTE_ADDRESS) || defined(EEPROM_TWO_BYTE_ADDRESS) || defined(EEPROM_THREE_BYTE_ADDRESS)
          I2C_write( EEPROM_ADDRESS_BYTE_1(adress) ); /* (next part of) address */
          ack_byte = I2C_getack();
          if ( ack_byte )
            break;
        #endif

        #if defined(EEPROM_TWO_BYTE_ADDRESS) || defined(EEPROM_THREE_BYTE_ADDRESS)
          I2C_write( EEPROM_ADDRESS_BYTE_2(adress) ); /* (next part of) address */
          ack_byte = I2C_getack();
          if ( ack_byte )
            break;
        #endif

        #if defined(EEPROM_THREE_BYTE_ADDRESS)
          I2C_write( EEPROM_ADDRESS_BYTE_3(adress) ); /* (next part of) address */
          ack_byte = I2C_getack();
          if ( ack_byte )
            break;
        #endif

        I2C_startCond();

        I2C_write( EEPROM_I2C_ADR | 0x01 | (EEPROM_ADDRESS_BYTE_0(adress) << 1) ); /* Block address (WITH READ !!) */
        ack_byte = I2C_getack();
        if ( ack_byte )
          break;

        while (len--)
        {
          *pt = I2C_read();
          if ( len )
          {
            I2C_setack();
            pt++;
          }
          /* 11.09.2007 MaSi neu: Trigger WD */
          E_WATCHDOG;
        }
        I2C_noack();
        I2C_stopCond();
      }
      while (0);

      #ifdef GLOBAL_DI_RD
        ENABLE;
      #endif

      return( !ack_byte );

    #endif
    }


    /* Read a page (64 bytes) from the EEPROM.
       Input: page, pointer to target area

       REGISTER: A, DPTR, AUXR1, EECON

       ACHTUNG: Routine toggles the DPTR. In the end, the same DPTR must be set as when it was entered.
    ------------------------------------------------------------------------- */
    UBYTE EEPROM_readPage( EEPROM_PAGE_OPT page, UBYTE E_DAT_ALLG_ATTR *pt )
    {
    #ifdef TOOL_CHAIN_KEIL
      UBYTE i;
    #endif

      /* Area test (Can be omitted for runtime optimization after no errors have occurred in the test phase.)
      ----------------------------------------------------------------------- */
      #ifdef EEPROM_TEST
        if ( page > MAX_PAGES )
        {
          return ( FALSE );
        }
      #endif

    #ifdef TOOL_CHAIN_KEIL

      #ifdef GLOBAL_DI_RD
        DISABLE; /* EEPROM access always under global INT lock */
      #endif

      /* Assemble the (source) address in the EEPROM and load it into the DPTR */
      adress = page;
      adress = adress << 6;

      #pragma ASM
        mov dph,adress    ; hi- vor lo-Byte
        mov dpl,adress+1
      #pragma ENDASM

      #pragma ASM
        inc auxr1         ; DPTR umschalten
      #pragma ENDASM

      /* Assemble the (target) address in the XDATA and load it into the DPTR */
      adress = pt;

      #pragma ASM
        mov dph,adress    ; hi- vor lo-Byte
        mov dpl,adress+1
      #pragma ENDASM

      /* Loop over 64 bytes */
      for ( i = 0; i < 64; i++ )
      {
        #pragma ASM
          #ifdef LOCAL_DI_RD
            clr EA        ; /* EEPROM access always under INT lock */
          #endif
          inc auxr1       ; DPTR umschalten

          mov eecon,#2    ; EEPROM aktivieren
          movx a,@dptr    ; EEPROM lesen
          mov eecon,#0    ; EEPROM deaktivieren
          inc dptr        ; DPTR++

          inc auxr1       ; DPTR umschalten

          movx @dptr,a    ; und abspeichern
          inc dptr        ; DPTR++
          #ifdef LOCAL_DI_RD
            setb EA       ; /* EEPROM access always under INT lock */
          #endif
        #pragma ENDASM
      }

      #pragma ASM
        inc auxr1         ; DPTR umschalten
      #pragma ENDASM

      #ifdef GLOBAL_DI_RD
        ENABLE;
      #endif

      return ( TRUE );

    #else

      return EEPROM_readBlock(page, 0, PAGE_SIZE, pt);

    #endif
    }


    /* ######################################################################

       Writing functions

       ######################################################################
    ------------------------------------------------------------------------- */

    /* Write a word in the EEPROM.
       Input: page and offset of the memory cell (theoretically the linear address 
              also works), word to be written

              JaGa: So far, the linear address has been checked to ensure that 
              it does not exceed the first page limit, 
              so practically the linear address does not work.

       TEMIC:
       After a .._ write _ .. () function, you must then
       START_EEPROM_WRITE () and GET_EEPROM_BUSY () should be called!

       ATMEL AT89C51ED2:
       In contrast to TEMIC, it can only write in bytes.
       There is only the block write function.
       It is optimized so that only changed bytes are written.
       The waiting for the writer happens in the function.

       MSP430 with I2C-EEPROM:
       After a .._ write _ .. () function, EEPROM_waitForAck () must be called! 
       This happens right here in the function, since there can be several write 
       operations in succession for the block and page functions,
    ------------------------------------------------------------------------- */
    #ifndef AT89C51ED2 // the WordWrite function is not available for the new ATMEL

    UBYTE EEPROM_writeWord( EEPROM_PAGE_OPT page, EEPROM_OFFSET_OPT offs, UWORD writeWord )
    {
    #ifdef TOOL_CHAIN_KEIL
      /* Area test (Can be omitted for runtime optimization after no errors 
         have occurred in the test phase.)
      ----------------------------------------------------------------------- */
      #ifdef EEPROM_TEST
        if ( (page > MAX_PAGES) || (offs > (PAGE_SIZE-2)) )
        {
          return ( FALSE );
        }
      #endif

      #ifdef GLOBAL_DI_WR
        DISABLE; /* EEPROM access always under global INT lock */
      #endif

        /* Bring the word to be written into the module-internal variable */
        value = writeWord;

        /* Assemble the address and load it into the DPTR */
        adress = page;
        adress = adress << 6;
        adress = adress | offs;

        #pragma ASM
          #ifdef LOCAL_DI_WR
            clr EA        ; /* EEPROM access always under INT lock */
          #endif
          mov dph,adress  ; hi- vor lo-Byte
          mov dpl,adress+1

          mov eecon,#2    ; EEPROM aktivieren
          mov a,value     ; 1. Byte holen
          movx @dptr,a    ; und im EEPROM abspeichern
          inc dptr
          mov a,value+1   ; 2. Byte holen
          movx @dptr,a    ; und im EEPROM abspeichern
          mov eecon,#0    ; EEPROM deaktivieren
          #ifdef LOCAL_DI_WR
            setb EA       ; /* EEPROM access always under INT lock */
          #endif
        #pragma ENDASM

      #ifdef GLOBAL_DI_WR
        ENABLE;
      #endif

      return ( TRUE );

    #else

      /* So far, a word has been written in the EEPROM and put together from two bytes
          as if it were always stored in the EEPROM in little endian format (Intel, MSP, ...). 
         This is dangerous because the user may want to read several words at once with 
         the EEPROM_readBlock () function and is wondering why this leads to 
         byte salad on big endian CPUs (TMS470) when he calls 
         EEPROM_ {read, write } Block () and EEPROM_ {read, write} Word () 
         mixes.
         Therefore, the word is now arranged in the EEPROM as it corresponds to
         the natural sequence of the CPU, i.e. Order in the EEPROM 
         is the same order in RAM / Flash. */
      return EEPROM_writeBlock(page, offs, sizeof(UWORD), (UBYTE *)&writeWord);

    #endif
    }
    #endif // AT89C51ED2


    /* Write a block (<= 64 bytes) in the EEPROM, 
       only allowed within one page.
       Input: page, offset, length, pointer to target area

       REGISTER: A, DPTR, AUXR1, EECON

       ACHTUNG: Routine toggles the DPTR. In the end, 
                the same DPTR must be set as when it was entered.
    ------------------------------------------------------------------------- */
    UBYTE EEPROM_writeBlock( EEPROM_PAGE_OPT page, EEPROM_OFFSET_OPT offs, EEPROM_OFFSET_OPT len, UBYTE E_DAT_ALLG_ATTR *pt )
    {
    #ifdef TOOL_CHAIN_KEIL
      UBYTE i;
    #endif

      /* Area test (Can be omitted for runtime optimization after 
         no errors have occurred in the test phase.)
      ----------------------------------------------------------------------- */
      #ifdef EEPROM_TEST
        if ( (page >= MAX_PAGES) || (offs >= PAGE_SIZE) || (len > (PAGE_SIZE - offs)) )
        {
          return ( FALSE );
        }
      #endif

    #ifdef TOOL_CHAIN_KEIL

      #ifdef GLOBAL_DI_WR
        DISABLE; /* EEPROM access always under global INT lock */
      #endif

        i = len; /* for() uses the DPTR to load len (5.5 compiler) */

        /* Assemble the (source) address in XDATA and load it into the DPTR */
        adress = pt;

        #pragma ASM
          mov dph,adress    ; hi- vor lo-Byte
          mov dpl,adress+1
        #pragma ENDASM

        #pragma ASM
          inc auxr1         ; DPTR umschalten
        #pragma ENDASM

        /* Assemble the (target) address in the EEPROM and load it into the DPTR */
        adress = page;
        adress = adress << 6;
        adress = adress | offs;

        #ifdef AT89C51ED2
          WD_TRIGGERN; /* since the writing process can take a long time, ... */
        #endif

        #pragma ASM
          mov dph,adress    ; hi- vor lo-Byte
          mov dpl,adress+1

    weiter:
        #pragma ENDASM

        /* Loop over up to 64 bytes */
        while ( i-- )
        {
          #pragma ASM
            #ifdef LOCAL_DI_WR
              clr EA        ; /* EEPROM access always under INT lock */
            #endif
            inc auxr1       ; DPTR umschalten

            movx a,@dptr    ; XDATA lesen
            inc dptr        ; DPTR++

            inc auxr1       ; DPTR umschalten

            mov eecon,#2    ; EEPROM aktivieren

            #ifdef AT89C51ED2
              ; vor dem Schreiben lesen und vergleichen
              xch a,b         ; Akku retten
              movx a,@dptr    ; EEPROM lesen
              xrl a,b         ; gleich?
              jz no_wr        ; ja
              xch a,b         ; Akku wiederholen
            #endif

            movx @dptr,a    ; und abspeichern
    no_wr:
            mov eecon,#0    ; EEPROM deaktivieren
            inc dptr        ; DPTR++
            #ifdef LOCAL_DI_WR
              setb EA       ; /* EEPROM access always under INT lock */
            #endif

            #ifdef AT89C51ED2
    warte:
              mov a,eecon     ; EEPROM-Status lesen
              jnb acc.0,weiter ; Schreiben fertig?
              sjmp warte      ; nein
            #endif

          #pragma ENDASM
        }

        #pragma ASM
          inc auxr1         ; DPTR umschalten
        #pragma ENDASM

      #ifdef GLOBAL_DI_WR
        ENABLE;
      #endif

      return ( TRUE );

    #else

      #ifdef GLOBAL_DI_WR
        DISABLE; /* EEPROM access always under global INT lock */
        SET_INT_MESSAGE();
      #endif

      /* Determine linear address
      ----------------------------------------------------------------------- */
      adress = page;
      adress = adress << 6;
      adress = adress | offs;

    #ifdef EEPROM_ORIGINALVARIANTE_SCHREIBEN
      /* Attention, this variant is not suitable for EEPROMs that expect more than one address byte after 
         the device byte. Since the address transmission would have to be changed in several places, 
         there is instead a new variant that only transmits the address once. */

      /* Calculate "half" and "whole" pages
      ----------------------------------------------------------------------- */
    #if 0 /* 09.07.2004 MaSi - Correction of the calculation */
      first_page_anz = 16 - (offs % 16);
    #else
      if (offs % 16) /* only if the offset is not equal to the start of the page */
        first_page_anz = 16 - (offs % 16);
      else
        first_page_anz = 0;
    #endif

      if ( first_page_anz >= len )
      {
        first_page_anz = len;
        page_count     = 0;
        last_page_anz  = 0;
      }
      else
      {
        if ( (first_page_anz > 0) && (first_page_anz < 16) )
          len -= first_page_anz;

        page_count    = len / 16;
        last_page_anz = len % 16;
      }
      RES_WC();  /* Remove write protection */

      if ( first_page_anz )
      {
        /* Encrypt to EEPROM address
        ----------------------------------------------------------------------- */
        offs    = (UBYTE)adress;
        page    = (UBYTE)(adress >> 8);
        page  <<= 1;
        len     = first_page_anz;
        adress += first_page_anz;

        /* Write first_page_anz bytes */
        I2C_startCond();

        I2C_write( EEPROM_I2C_ADR | page ); /* Block address */
        ack_byte = I2C_getack();

        if ( ack_byte == 0 )
        {
          I2C_write( offs ); /* Offset address */
          ack_byte = I2C_getack();

          if ( ack_byte == 0 )
          {
            while ( len-- )
            {
              temp_buffer = *pt;
              I2C_write( temp_buffer ); /* Byte */
              ack_byte = I2C_getack();
              pt++;

              /* 11.09.2007 MaSi new: trigger WD */
              E_WATCHDOG;
            }
            I2C_stopCond();
          }
        }
        /* wait for the end of the internal write operation */
        e_timeout = 1000;
        do
        {
          ack_byte = !EEPROM_waitForAck();
          e_timeout--;

          /* 11.09.2007 MaSi new: trigger WD */
          E_WATCHDOG;
        }
        while ( (ack_byte == 1) && (e_timeout) );
      } /* first_page_anz */

      if ( page_count )
      {
        for ( i = 0, len = 16; i < page_count; i++, adress+=16, len = 16 )
        {
          /* Encrypt to EEPROM address
          ----------------------------------------------------------------------- */
          offs = (UBYTE)adress;
          page = (UBYTE)(adress >> 8);
          page <<= 1;

          /* Write 16 bytes */
          I2C_startCond();

          I2C_write( EEPROM_I2C_ADR | page ); /* Block address */
          ack_byte = I2C_getack();

          if ( ack_byte == 0 )
          {
            I2C_write( offs ); /* Offset address */
            ack_byte = I2C_getack();

            if ( ack_byte == 0 )
            {
              while ( len-- )
              {
                temp_buffer = *pt;
                I2C_write( temp_buffer ); /* Byte */
                ack_byte = I2C_getack();
                pt++;

                /* 11.09.2007 MaSi new: trigger WD */
                E_WATCHDOG;
              }
              I2C_stopCond();
            }
          }
          /* wait for the end of the internal write operation */
          e_timeout = 1000;
          do
          {
            ack_byte = !EEPROM_waitForAck();
            e_timeout--;

            /* 11.09.2007 MaSi new: trigger WD */
            E_WATCHDOG;
          }
          while ( (ack_byte == 1) && (e_timeout) );

        }
      } /* page_count */

      if ( last_page_anz )
      {
        /* Encrypt to EEPROM address
        ----------------------------------------------------------------------- */
        offs = (UBYTE)adress;
        page = (UBYTE)(adress >> 8);
        page <<= 1;
        len  = last_page_anz;

        /* Write last_page_anz bytes */
        I2C_startCond();

        I2C_write( EEPROM_I2C_ADR | page ); /* Block address */
        ack_byte = I2C_getack();

        if ( ack_byte == 0 )
        {
          I2C_write( offs ); /* Offset address */
          ack_byte = I2C_getack();

          if ( ack_byte == 0 )
          {
            while ( len-- )
            {
              temp_buffer = *pt;
              I2C_write( temp_buffer ); /* Byte */
              ack_byte = I2C_getack();
              pt++;

              /* 11.09.2007 MaSi new: trigger WD */
              E_WATCHDOG;
            }
            I2C_stopCond();
          }
        }
        /* wait for the end of the internal write operation */
        e_timeout = 1000;
        do
        {
          ack_byte = !EEPROM_waitForAck();
          e_timeout--;

          /* 11.09.2007 MaSi new: trigger WD */
          E_WATCHDOG;
        }
        while ( (ack_byte == 1) && (e_timeout) );
      } /* last_page_anz */

      SET_WC();  /* Set write protection again */

    #else  /* #ifdef EEPROM_ORIGINALVARIANTE_SCHREIBEN */

      RES_WC();  /* Remove write protection */

      while ( len )
      {

        /* Write first_page_anz bytes */
        I2C_startCond();

        I2C_write( EEPROM_I2C_ADR | (EEPROM_ADDRESS_BYTE_0(adress) << 1) ); /* (first part of) address */
        ack_byte = I2C_getack();
        if ( ack_byte )
          break;

        #if defined(EEPROM_ONE_BYTE_ADDRESS) || defined(EEPROM_TWO_BYTE_ADDRESS) || defined(EEPROM_THREE_BYTE_ADDRESS)
          I2C_write( EEPROM_ADDRESS_BYTE_1(adress) ); /* (next part of) address */
          ack_byte = I2C_getack();
          if ( ack_byte )
            break;
        #endif

        #if defined(EEPROM_TWO_BYTE_ADDRESS) || defined(EEPROM_THREE_BYTE_ADDRESS)
          I2C_write( EEPROM_ADDRESS_BYTE_2(adress) ); /* (next part of) address */
          ack_byte = I2C_getack();
          if ( ack_byte )
            break;
        #endif

        #if defined(EEPROM_THREE_BYTE_ADDRESS)
          I2C_write( EEPROM_ADDRESS_BYTE_3(adress) ); /* (next part of) address */
          ack_byte = I2C_getack();
          if ( ack_byte )
            break;
        #endif

        do
        {
          /* 11.09.2007 MaSi new: trigger WD */
          E_WATCHDOG;

          temp_buffer = *pt;
          I2C_write( temp_buffer ); /* Byte */
          ack_byte = I2C_getack();
          if ( ack_byte )
            break;
          pt++;
          len--;
          adress++;
    //      if (adress & (EEPROM_PAGE_SIZE-1))
          if ( !(adress & (EEPROM_PAGE_SIZE-1)) )
            break; /* Transfer canceled because new address is on new page */
        }
        while ( len );
        if ( ack_byte )
          break;

        /* wait for the end of the internal write operation */
        e_timeout = 1024; /* A simple power of two can be generated on the TMS470 
                             with just one assembler command. For the number 1000 
                             he already needs 2 commands. */
        do
        {
          I2C_stopCond(); /* Ends the last write access or polling Ack */
          ack_byte = !EEPROM_waitForAck();
          e_timeout--;

          /* 11.09.2007 MaSi new: trigger WD */
          E_WATCHDOG;
        }
        while ( (ack_byte == 1) && (e_timeout) );

        if ( len )
          I2C_stopCond(); /* If necessary, ends the last query from Ack */
      } /* len */

      I2C_stopCond();

      SET_WC();  /* Set write protection again */

    #endif

      #ifdef GLOBAL_DI_WR
        ENABLE;
      #endif

      return ( !ack_byte );

    #endif
    }


    /* Write a page (64 bytes) into the EEPROM.
       Input: page, pointer to target area

       REGISTER: A, DPTR, AUXR1, EECON

       ACHTUNG: Routine toggles the DPTR. In the end, the same DPTR 
                must be set as when it was entered.
    ------------------------------------------------------------------------- */
    #ifndef AT89C51ED2 // the PageWrite function is not available for the new ATMEL

    UBYTE EEPROM_writePage( EEPROM_PAGE_OPT page, UBYTE E_DAT_ALLG_ATTR *pt )
    {
    #ifdef TOOL_CHAIN_KEIL
      UBYTE i;
    #endif

    #ifdef TOOL_CHAIN_KEIL
      /* Area test (Can be omitted for runtime optimization after 
         no errors have occurred in the test phase.)
      ----------------------------------------------------------------------- */
      #ifdef EEPROM_TEST
        if ( page >= MAX_PAGES )
        {
          return ( FALSE );
        }
      #endif

      #ifdef GLOBAL_DI_WR
        DISABLE; /* EEPROM access always under global INT lock */
      #endif

        /* Assemble the (source) address in XDATA and load it into the DPTR */
        adress = pt;

        #pragma ASM
          mov dph,adress    ; hi- vor lo-Byte
          mov dpl,adress+1
        #pragma ENDASM

        #pragma ASM
          inc auxr1         ; DPTR umschalten
        #pragma ENDASM

        /* Assemble the (target) address in the EEPROM and load it into the DPTR */
        adress = page;
        adress = adress << 6;

        #pragma ASM
          mov dph,adress    ; hi- vor lo-Byte
          mov dpl,adress+1
        #pragma ENDASM

        /* Loop over 64 bytes */
        for ( i = 0; i < 64; i++ )
        {
          #pragma ASM
            #ifdef LOCAL_DI_WR
              clr EA        ; /* EEPROM access always under INT lock */
            #endif
            inc auxr1       ; DPTR umschalten

            movx a,@dptr    ; XDATA lesen
            inc dptr        ; DPTR++

            inc auxr1       ; DPTR umschalten

            mov eecon,#2    ; EEPROM aktivieren
            movx @dptr,a    ; und abspeichern
            mov eecon,#0    ; EEPROM deaktivieren
            inc dptr        ; DPTR++
            #ifdef LOCAL_DI_WR
              setb EA       ; /* EEPROM access always under INT lock */
            #endif
          #pragma ENDASM
        }

        #pragma ASM
          inc auxr1         ; DPTR umschalten
        #pragma ENDASM

      #ifdef GLOBAL_DI_WR
        ENABLE;
      #endif

      return ( TRUE );

    #else

      return EEPROM_writeBlock(page, 0, PAGE_SIZE, pt);

    #endif
    }
    #endif // AT89C51ED2


    /* ######################################################################

       Compare functions

       ######################################################################
    ------------------------------------------------------------------------- */

    /* Compare a block (<= 64 bytes) with the EEPROM, only allowed 
       within one page.
       Input: page, offset, length, pointer to target area

       REGISTER: A, DPTR, AUXR1, EECON

       ATTENTION: Routine toggles the DPTR. In the end, the same 
                DPTR must be set as when it was entered.

       returns: TRUE in case of equality
                FALSE in case of inequality
    ------------------------------------------------------------------------- */
    UBYTE EEPROM_compareBlock( EEPROM_PAGE_OPT page, EEPROM_OFFSET_OPT offs, EEPROM_OFFSET_OPT len, UBYTE E_DAT_ALLG_ATTR *cmp_data_pt )
    {
    #ifdef TOOL_CHAIN_KEIL
      UBYTE i;
    #else
      UnsignedOpt i;
    #endif

      Unsigned8 *rd_data_ptr;


      for (i = 0; i < PAGE_SIZE; i++)
      {
          eep_data.rd_cmp_data[i] = 0x88;
      }

      /* Area test (Can be omitted for runtime optimization after 
         no errors have occurred in the test phase.)
      ----------------------------------------------------------------------- */
      #ifdef EEPROM_TEST
        if ( (page >= MAX_PAGES) || (offs >= PAGE_SIZE) || (len > (PAGE_SIZE - offs)) )
        {
          return ( FALSE );
        }
      #endif

    #ifdef TOOL_CHAIN_KEIL

      #ifdef GLOBAL_DI_RD
        DISABLE; /* EEPROM access always under global INT lock */
      #endif

      i = len; /* for() uses the DPTR to load len (5.5 compiler) */

      /* Assemble the (source) address in XDATA and load it into the DPTR */
      adress = pt;

      #pragma ASM
        mov dph,adress    ; hi- vor lo-Byte
        mov dpl,adress+1
      #pragma ENDASM

      #pragma ASM
        inc auxr1         ; DPTR umschalten
      #pragma ENDASM

      /* Assemble the (target) address in the EEPROM and load it into the DPTR */
      adress = page;
      adress = adress << 6;
      adress = adress | offs;

      #pragma ASM
        mov dph,adress    ; hi- vor lo-Byte
        mov dpl,adress+1
      #pragma ENDASM

      /* Loop over 64 bytes */
    //  for ( i = 0; i < len; i++ )
      while ( i-- )
      {
        #pragma ASM
          #ifdef LOCAL_DI_RD
            clr EA        ; /* EEPROM access always under INT lock */
          #endif
          inc auxr1       ; DPTR umschalten

          movx a,@dptr    ; XDATA lesen
          mov r4,a        ; und merken

          inc dptr        ; DPTR++

          inc auxr1       ; DPTR umschalten

          mov eecon,#2    ; EEPROM aktivieren

          movx a,@dptr    ; EEPROM-Wert holen

          mov eecon,#0    ; EEPROM deaktivieren
          inc dptr        ; DPTR++

          #ifdef LOCAL_DI_RD
            setb EA       ; /* EEPROM access always under INT lock */
          #endif

          xrl a,r4        ; wenn beide gleich sind, kommt 0 raus
          jnz ungl        ; Sprung bei Differenz
        #pragma ENDASM
      }

      #pragma ASM
        inc auxr1         ; DPTR umschalten
      #pragma ENDASM

      #ifdef GLOBAL_DI_RD
        ENABLE;
      #endif

      return ( TRUE );

      #pragma ASM
        ungl:
          inc auxr1         ; DPTR umschalten
      #pragma ENDASM

      #ifdef GLOBAL_DI_RD
        ENABLE;
      #endif

      return( FALSE);

    #else

      #ifdef GLOBAL_DI_RD
        DISABLE; /* EEPROM access always under global INT lock */
        SET_INT_MESSAGE();
      #endif

      ack_byte = 1; /* To indicate that something went wrong */

      /* Determine linear address
      ----------------------------------------------------------------------- */
      adress = page;
      adress = adress << 6;
      adress = adress | offs;

      rd_data_ptr = &eep_data.rd_cmp_data[0];

      do
      {
        SET_WC();  /* 08.07.2004 MaSi: set write protection for security */

        I2C_startCond();

        I2C_write( EEPROM_I2C_ADR | (EEPROM_ADDRESS_BYTE_0(adress) << 1) ); /* (first part of) address (WITH WRITE !!!!) */
        ack_byte = I2C_getack();
        if ( ack_byte )
          break;

        #if defined(EEPROM_ONE_BYTE_ADDRESS) || defined(EEPROM_TWO_BYTE_ADDRESS) || defined(EEPROM_THREE_BYTE_ADDRESS)
          I2C_write( EEPROM_ADDRESS_BYTE_1(adress) ); /* (next part of) address */
          ack_byte = I2C_getack();
          if ( ack_byte )
            break;
        #endif

        #if defined(EEPROM_TWO_BYTE_ADDRESS) || defined(EEPROM_THREE_BYTE_ADDRESS)
          I2C_write( EEPROM_ADDRESS_BYTE_2(adress) ); /* (next part of) address */
          ack_byte = I2C_getack();
          if ( ack_byte )
            break;
        #endif

        #if defined(EEPROM_THREE_BYTE_ADDRESS)
          I2C_write( EEPROM_ADDRESS_BYTE_3(adress) ); /* (next part of) address */
          ack_byte = I2C_getack();
          if ( ack_byte )
            break;
        #endif

        I2C_startCond();

        I2C_write( EEPROM_I2C_ADR | 0x01 | (EEPROM_ADDRESS_BYTE_0(adress) << 1) ); /* Block address (WITH READ !!) */
        ack_byte = I2C_getack();
        if ( ack_byte )
          break;

        while (len--)
        {
            *rd_data_ptr = I2C_read();

          if ( *cmp_data_pt != *rd_data_ptr )
          {
            I2C_noack();
            I2C_stopCond();

            #ifdef GLOBAL_DI_RD
              ENABLE;
            #endif

            return ( FALSE );
          }

          if ( len )
          {
            I2C_setack();
            cmp_data_pt++;
            rd_data_ptr++;
          }

          /* 11.09.2007 MaSi new: trigger WD */
          E_WATCHDOG;
        }
        I2C_noack();
        I2C_stopCond();
      }
      while (0);

      #ifdef GLOBAL_DI_RD
        ENABLE;
      #endif

      return ( TRUE );

    #endif
    }


    /* ######################################################################

       Functions for reading and writing large linear blocks 
       (not for AT89C51ED2)

       ######################################################################
    ------------------------------------------------------------------------- */

    #ifndef AT89C51ED2

    /* Read a block from the EEPROM.
       Should only work for the area that lies after 
       the area divided into virtual pages.
       --> configurable: ALLOW_COMPLETE_LINEAR_ACCESS

       Input:  address, length, pointer to read data
       Output: TRUE - oK, FALSE - error
    ------------------------------------------------------------------------- */
    UBYTE EEPROM_readBlock_linear( EEPROM_OFFSET_OPT adr, EEPROM_OFFSET_OPT len, UBYTE E_DAT_ALLG_ATTR *pt )
    {
      /* Area test (Can be omitted for runtime optimization 
         after no errors have occurred in the test phase.)
      ----------------------------------------------------------------------- */
      #ifdef EEPROM_TEST
        if ( (adr >= EEPROM_SIZE) || ((adr+len) > EEPROM_SIZE) || (len == 0) || (pt == 0) )
          return( FALSE );
      #endif

      #ifndef ALLOW_COMPLETE_LINEAR_ACCESS
        if ( adr < FIRST_FREE_EEPROM_ADRESS )
          return( FALSE );
      #endif

      #ifdef GLOBAL_DI_RD
        DISABLE;
        SET_INT_MESSAGE();
      #endif

      ack_byte = 1; /* To indicate that something went wrong */

      /* Determine linear address (is already the correct one)
      ----------------------------------------------------------------------- */
      adress = adr;

      /* The area test was at the beginning of the function.
      ----------------------------------------------------------------------- */
      do
      {
        SET_WC();  /* 08.07.2004 MaSi: set write protection for security */

        I2C_startCond();

        I2C_write( EEPROM_I2C_ADR | (EEPROM_ADDRESS_BYTE_0(adress) << 1) ); /* (first part of) address (WITH WRITE !!!!) */
        ack_byte = I2C_getack();
        if ( ack_byte )
          break;

        #if defined(EEPROM_ONE_BYTE_ADDRESS) || defined(EEPROM_TWO_BYTE_ADDRESS) || defined(EEPROM_THREE_BYTE_ADDRESS)
          I2C_write( EEPROM_ADDRESS_BYTE_1(adress) ); /* (next part of) address */
          ack_byte = I2C_getack();
          if ( ack_byte )
            break;
        #endif

        #if defined(EEPROM_TWO_BYTE_ADDRESS) || defined(EEPROM_THREE_BYTE_ADDRESS)
          I2C_write( EEPROM_ADDRESS_BYTE_2(adress) ); /* (next part of) address */
          ack_byte = I2C_getack();
          if ( ack_byte )
            break;
        #endif

        #if defined(EEPROM_THREE_BYTE_ADDRESS)
          I2C_write( EEPROM_ADDRESS_BYTE_3(adress) ); /* (next part of) address */
          ack_byte = I2C_getack();
          if ( ack_byte )
            break;
        #endif

        I2C_startCond();

        I2C_write( EEPROM_I2C_ADR | 0x01 | (EEPROM_ADDRESS_BYTE_0(adress) << 1) ); /* Block address (WITH READ !!) */
        ack_byte = I2C_getack();
        if ( ack_byte )
          break;

        while (len--)
        {
          *pt = I2C_read();
          if ( len )
          {
            I2C_setack();
            pt++;
          }
          E_WATCHDOG;
        }
        I2C_noack();
        I2C_stopCond();
      }
      while (0);

      #ifdef GLOBAL_DI_RD
        ENABLE;
      #endif

      return( !ack_byte );
    }


    /* Write a block in the EEPROM.
       Should only work for the area that lies 
       after the area divided into virtual pages.
       --> configurable: ALLOW_COMPLETE_LINEAR_ACCESS

       Input:  address, length, pointer to write data
       Output: TRUE - oK, FALSE - error
    ------------------------------------------------------------------------- */
    UBYTE EEPROM_writeBlock_linear( EEPROM_OFFSET_OPT adr, EEPROM_OFFSET_OPT len, UBYTE E_DAT_ALLG_ATTR *pt )
    {
      /* Area test (Can be omitted for runtime optimization 
         after no errors have occurred in the test phase.)
      ----------------------------------------------------------------------- */
      #ifdef EEPROM_TEST
        if ( (adr >= EEPROM_SIZE) || ((adr+len) > EEPROM_SIZE) || (len == 0) || (pt == 0) )
          return( FALSE );
      #endif

      #ifndef ALLOW_COMPLETE_LINEAR_ACCESS
        if ( adr < FIRST_FREE_EEPROM_ADRESS )
          return( FALSE );
      #endif

      RES_WC();  /* Remove write protection */

      #ifdef GLOBAL_DI_WR
        DISABLE; /* EEPROM access always under global INT lock */
        SET_INT_MESSAGE();
      #endif

      /* Determine linear address (is already the correct one)
      ----------------------------------------------------------------------- */
      adress = adr;

      while ( len )
      {
        /* Write first_page_anz bytes */
        I2C_startCond();

        I2C_write( EEPROM_I2C_ADR | (EEPROM_ADDRESS_BYTE_0(adress) << 1) ); /* (first part of) address */
        ack_byte = I2C_getack();
        if ( ack_byte )
          break;

        #if defined(EEPROM_ONE_BYTE_ADDRESS) || defined(EEPROM_TWO_BYTE_ADDRESS) || defined(EEPROM_THREE_BYTE_ADDRESS)
          I2C_write( EEPROM_ADDRESS_BYTE_1(adress) ); /* (next part of) address */
          ack_byte = I2C_getack();
          if ( ack_byte )
            break;
        #endif

        #if defined(EEPROM_TWO_BYTE_ADDRESS) || defined(EEPROM_THREE_BYTE_ADDRESS)
          I2C_write( EEPROM_ADDRESS_BYTE_2(adress) ); /* (next part of) address */
          ack_byte = I2C_getack();
          if ( ack_byte )
            break;
        #endif

        #if defined(EEPROM_THREE_BYTE_ADDRESS)
          I2C_write( EEPROM_ADDRESS_BYTE_3(adress) ); /* (next part of) address */
          ack_byte = I2C_getack();
          if ( ack_byte )
            break;
        #endif

        do
        {
          E_WATCHDOG;
          temp_buffer = *pt;
          I2C_write( temp_buffer ); /* Byte */
          ack_byte = I2C_getack();
          if ( ack_byte )
            break;
          pt++;
          len--;
          adress++;
          if ( !(adress & (EEPROM_PAGE_SIZE-1)) )
            break; /* Transfer canceled because new address is on new page */
        }
        while ( len );
        if ( ack_byte )
          break;

        /* wait for the end of the internal write operation */
        e_timeout = 1024; /* A simple power of two can be generated on the TMS470 
                             with just one assembler command. For the number 1000 
                             he already needs 2 commands. */
        do
        {
          I2C_stopCond(); /* Ends the last write access or polling Ack */
          ack_byte = !EEPROM_waitForAck();
          e_timeout--;
          E_WATCHDOG;
        }
        while ( (ack_byte == 1) && (e_timeout) );

        if ( len )
          I2C_stopCond(); /* If necessary, ends the last query from Ack */
      } /* len */

      I2C_stopCond();

      SET_WC();  /* Set write protection again */

      #ifdef GLOBAL_DI_WR
        ENABLE;
      #endif

      return ( !ack_byte );
    }


    /* Compare a block in the EEPROM.
       Should only work for the area that lies after 
       the area divided into virtual pages.
       --> configurable: ALLOW_COMPLETE_LINEAR_ACCESS

       Input:  address, length, pointer to comparison data
       Output: TRUE - oK, FALSE - error
    ------------------------------------------------------------------------- */
    UBYTE EEPROM_compareBlock_linear( EEPROM_OFFSET_OPT adr, EEPROM_OFFSET_OPT len, UBYTE E_DAT_ALLG_ATTR *pt )
    {
      /* Area test (Can be omitted for runtime optimization 
         after no errors have occurred in the test phase.)
      ----------------------------------------------------------------------- */
      #ifdef EEPROM_TEST
        if ( (adr >= EEPROM_SIZE) || ((adr+len) > EEPROM_SIZE) || (len == 0) || (pt == 0) )
          return( FALSE );
      #endif

      #ifndef ALLOW_COMPLETE_LINEAR_ACCESS
        if ( adr < FIRST_FREE_EEPROM_ADRESS )
          return( FALSE );
      #endif

      #ifdef GLOBAL_DI_RD
        DISABLE; /* EEPROM access always under global INT lock */
        SET_INT_MESSAGE();
      #endif

      ack_byte = 1; /* To indicate that something went wrong */

      /* Determine linear address (is already the correct one)
      ----------------------------------------------------------------------- */
      adress = adr;

      do
      {
        SET_WC();  /* 08.07.2004 MaSi: set write protection for security */

        I2C_startCond();

        I2C_write( EEPROM_I2C_ADR | (EEPROM_ADDRESS_BYTE_0(adress) << 1) ); /* (first part of) address (WITH WRITE !!!!) */
        ack_byte = I2C_getack();
        if ( ack_byte )
          break;

        #if defined(EEPROM_ONE_BYTE_ADDRESS) || defined(EEPROM_TWO_BYTE_ADDRESS) || defined(EEPROM_THREE_BYTE_ADDRESS)
          I2C_write( EEPROM_ADDRESS_BYTE_1(adress) ); /* (next part of) address */
          ack_byte = I2C_getack();
          if ( ack_byte )
            break;
        #endif

        #if defined(EEPROM_TWO_BYTE_ADDRESS) || defined(EEPROM_THREE_BYTE_ADDRESS)
          I2C_write( EEPROM_ADDRESS_BYTE_2(adress) ); /* (next part of) address */
          ack_byte = I2C_getack();
          if ( ack_byte )
            break;
        #endif

        #if defined(EEPROM_THREE_BYTE_ADDRESS)
          I2C_write( EEPROM_ADDRESS_BYTE_3(adress) ); /* (next part of) address */
          ack_byte = I2C_getack();
          if ( ack_byte )
            break;
        #endif

        I2C_startCond();

        I2C_write( EEPROM_I2C_ADR | 0x01 | (EEPROM_ADDRESS_BYTE_0(adress) << 1) ); /* Block address (WITH READ !!) */
        ack_byte = I2C_getack();
        if ( ack_byte )
          break;

        while (len--)
        {
          temp_buffer = I2C_read();

          if ( *pt != temp_buffer )
          {
            I2C_noack();
            I2C_stopCond();

            #ifdef GLOBAL_DI_RD
              ENABLE;
            #endif

            return ( FALSE );
          }
          if ( len )
          {
            I2C_setack();
            pt++;
          }
          E_WATCHDOG;
        }
        I2C_noack();
        I2C_stopCond();
      }
      while (0);

      #ifdef GLOBAL_DI_RD
        ENABLE;
      #endif

      return !ack_byte;
    }


    #endif /* #ifndef AT89C51ED2 */



    /* ######################################################################

       Write functions for DAU (only for MSP430)

       ######################################################################
    ------------------------------------------------------------------------- */

    #ifndef TOOL_CHAIN_KEIL

    void DAU_init( void )
    {
      /* I2C addressing DAU */
    //  P1OUT |= ( BIT4 | BIT5 | BIT6 | BIT7 );
    //  P1DIR |= ( BIT4 | BIT5 | BIT6 | BIT7 );
    }

    /* Write and output a setpoint word in the DAU.
       Input: Channel, setpoint

       TEMIC:
       unsupported

       MSP430 with I2C-DAU:
       ???

       liefert: TRUE at Ok
                FALSE on error
    ------------------------------------------------------------------------- */
    UBYTE DAU_writeOutputChannel( UBYTE channel, UWORD outWord )
    {
      /* Area test (Can be omitted for runtime optimization 
         after no errors have occurred in the test phase.)
      ----------------------------------------------------------------------- */
      #ifdef EEPROM_TEST
        if ( (channel > 3) )
        {
          return ( FALSE );
        }
      #endif

      #ifdef GLOBAL_DI_WR
        DISABLE; /* DAC access always under global INT lock ???  */
        SET_INT_MESSAGE();
      #endif

      /* Encrypt to DAU address
      ----------------------------------------------------------------------- */
      channel <<= 1;

      I2C_startCond();

      I2C_write( DAU_I2C_ADR_1 ); /* DAU address */
      ack_byte = I2C_getack();

      if ( ack_byte == 0 )
      {
        I2C_write( DAU_I2C_ADR_2 | LOAD_MODE_1 | channel ); /* load and spend right away */
        ack_byte = I2C_getack();

        if ( ack_byte == 0 )
        {
          temp_buffer = (UBYTE)(outWord >> 8);
          I2C_write( temp_buffer ); /* hi-Byte */
          ack_byte = I2C_getack();

          temp_buffer = (UBYTE)outWord;
          I2C_write( temp_buffer ); /* lo-Byte */
          ack_byte = I2C_getack();

          I2C_stopCond();
        }
      }

      #ifdef GLOBAL_DI_WR
        ENABLE;
      #endif

      return( !ack_byte );
    }

    #endif

    /* ######################################################################
    ------------------------------------------------------------------------- */

    #ifndef TOOL_CHAIN_KEIL

      #ifdef TMS470_IAR
        /* Software queue
           Is specified for 48MHz core clock.
           Works exactly enough for times from 2..5us.
        --------------------------------------------------------------------- */
        void wait_n_mal_1us( UBYTE anz_us )
        {
          for ( ; anz_us; anz_us-- )
          {
            __no_operation(); __no_operation();
            __no_operation(); __no_operation();
            __no_operation(); __no_operation();
            __no_operation(); __no_operation();
          }
        }
      #endif

    /* Write 8 bits into the EEPROM
       - Data may only be changed during CLK = low
       - EEPROM takes over with a rising edge
    ----------------------------------------------------------------------- */
    void I2C_write( UBYTE b )
    {
      I2C_buffer = b;

      SET_I2C_DATA_OUT();  /* Set direction register */

      RES_I2C_CLK();
      if ( I2C_buffer & BIT7 )
        SET_I2C_DATA();
      else
        RES_I2C_DATA();
      WAIT_WR_LO(); // NOP_1();
      SET_I2C_CLK();
      WAIT_WR_HI(); // NOP_1();

      RES_I2C_CLK();
      if ( I2C_buffer & BIT6 )
        SET_I2C_DATA();
      else
        RES_I2C_DATA();
      WAIT_WR_LO(); // NOP_1();
      SET_I2C_CLK();
      WAIT_WR_HI(); // NOP_1();

      RES_I2C_CLK();
      if ( I2C_buffer & BIT5 )
        SET_I2C_DATA();
      else
        RES_I2C_DATA();
      WAIT_WR_LO(); // NOP_1();
      SET_I2C_CLK();
      WAIT_WR_HI(); // NOP_1();

      RES_I2C_CLK();
      if ( I2C_buffer & BIT4 )
        SET_I2C_DATA();
      else
        RES_I2C_DATA();
      WAIT_WR_LO(); // NOP_1();
      SET_I2C_CLK();
      WAIT_WR_HI(); // NOP_1();

      RES_I2C_CLK();
      if ( I2C_buffer & BIT3 )
        SET_I2C_DATA();
      else
        RES_I2C_DATA();
      WAIT_WR_LO(); // NOP_1();
      SET_I2C_CLK();
      WAIT_WR_HI(); // NOP_1();

      RES_I2C_CLK();
      if ( I2C_buffer & BIT2 )
        SET_I2C_DATA();
      else
        RES_I2C_DATA();
      WAIT_WR_LO(); // NOP_1();
      SET_I2C_CLK();
      WAIT_WR_HI(); // NOP_1();

      RES_I2C_CLK();
      if ( I2C_buffer & BIT1 )
        SET_I2C_DATA();
      else
        RES_I2C_DATA();
      WAIT_WR_LO(); // NOP_1();
      SET_I2C_CLK();
      WAIT_WR_HI(); // NOP_1();

      RES_I2C_CLK();
      if ( I2C_buffer & BIT0 )
        SET_I2C_DATA();
      else
        RES_I2C_DATA();
      WAIT_WR_LO(); // NOP_1();
      SET_I2C_CLK();
      WAIT_WR_HI(); // NOP_5();

      return;
    }

    /* Read 8 bits from the EEPROM
       - EEPROM outputs with a falling edge
    ----------------------------------------------------------------------- */
    UBYTE I2C_read( void )
    {
      I2C_buffer = 0;

      SET_I2C_DATA_IN();  /* Set direction register */

      RES_I2C_CLK();
      WAIT_RD_LO(); // NOP_4();
      SET_I2C_CLK();
      if ( GET_I2C_DATA() )
        I2C_buffer |= BIT7;
      WAIT_RD_HI(); // NOP_5();

      RES_I2C_CLK();
      WAIT_RD_LO(); // NOP_4();
      SET_I2C_CLK();
      if ( GET_I2C_DATA() )
        I2C_buffer |= BIT6;
      WAIT_RD_HI(); // NOP_5();

      RES_I2C_CLK();
      WAIT_RD_LO(); // NOP_4();
      SET_I2C_CLK();
      if ( GET_I2C_DATA() )
        I2C_buffer |= BIT5;
      WAIT_RD_HI(); // NOP_5();

      RES_I2C_CLK();
      WAIT_RD_LO(); // NOP_4();
      SET_I2C_CLK();
      if ( GET_I2C_DATA() )
        I2C_buffer |= BIT4;
      WAIT_RD_HI(); // NOP_5();

      RES_I2C_CLK();
      WAIT_RD_LO(); // NOP_4();
      SET_I2C_CLK();
      if ( GET_I2C_DATA() )
        I2C_buffer |= BIT3;
      WAIT_RD_HI(); // NOP_5();

      RES_I2C_CLK();
      WAIT_RD_LO(); // NOP_4();
      SET_I2C_CLK();
      if ( GET_I2C_DATA() )
        I2C_buffer |= BIT2;
      WAIT_RD_HI(); // NOP_5();

      RES_I2C_CLK();
      WAIT_RD_LO(); // NOP_4();
      SET_I2C_CLK();
      if ( GET_I2C_DATA() )
        I2C_buffer |= BIT1;
      WAIT_RD_HI(); // NOP_5();

      RES_I2C_CLK();
      WAIT_RD_LO(); // NOP_4();
      SET_I2C_CLK();
      if ( GET_I2C_DATA() )
        I2C_buffer |= BIT0;
      WAIT_RD_HI(); // NOP_5();

      return ( I2C_buffer );
    }

    /* Start condition: falling DATA edge at CLK = high
    ------------------------------------------------------------------------- */
    void I2C_startCond( void )
    {
      SET_I2C_DATA();
      SET_I2C_DATA_OUT();
      WAIT_CTRL(); // NOP_2();
      SET_I2C_CLK();
      WAIT_CTRL(); // NOP_2();
      RES_I2C_DATA();
      WAIT_CTRL(); // NOP_5();
    }

    /* Stop condition: rising DATA edge with CLK = high
    ------------------------------------------------------------------------- */
    void I2C_stopCond( void )
    {
      RES_I2C_DATA();
      SET_I2C_DATA_OUT();
      WAIT_CTRL(); // NOP_2();
      RES_I2C_CLK();
      WAIT_WR_LO(); // NOP_3();
      SET_I2C_CLK();
      WAIT_CTRL(); // NOP_2();
      SET_I2C_DATA();
      WAIT_CTRL(); // NOP_5();
    }

    /* Read Acknowledge: Output 9th beat, check DATA during high time
    ------------------------------------------------------------------------- */
    UBYTE I2C_getack( void )
    {
      UBYTE ack;

      WAIT_CTRL(); // NOP_5();
      RES_I2C_CLK();
      SET_I2C_DATA_IN();
      WAIT_CTRL(); // NOP_1();
      SET_I2C_CLK();
      WAIT_WR_HI(); // NOP_3();
      ack = ( GET_I2C_DATA() ? 1 : 0);
      RES_I2C_CLK();   /* ??? */
      WAIT_CTRL(); // NOP_5();

      return( ack );
    }

    /* Write positive acknowledge: continue reading
    ------------------------------------------------------------------------- */
    void I2C_setack( void )
    {
      WAIT_CTRL(); // NOP_5();
      RES_I2C_CLK();
    //  SET_I2C_DATA_OUT();
    //  RES_I2C_DATA();
      RES_I2C_DATA(); /*  */
      SET_I2C_DATA_OUT(); /*  */
      WAIT_CTRL(); // NOP_1();
      SET_I2C_CLK();
      WAIT_WR_HI(); // NOP_3();
      RES_I2C_CLK();
    //  SET_I2C_DATA();
      SET_I2C_DATA_IN(); /*  */ /* Set direction register */
      WAIT_CTRL(); // NOP_5();
    }

    /* Write negative acknowledge: cancel reading
    ------------------------------------------------------------------------- */
    void I2C_noack( void )
    {
      WAIT_CTRL(); // NOP_5();
      RES_I2C_CLK();
      WAIT_CTRL(); // NOP_1();
      SET_I2C_DATA_OUT();
      SET_I2C_DATA();
      WAIT_CTRL(); // NOP_3();
      SET_I2C_CLK();
      WAIT_WR_HI(); // NOP_3();
      RES_I2C_CLK();
      WAIT_CTRL(); // NOP_5();
    }

    /* Reset protocol
    ------------------------------------------------------------------------- */
    void I2C_reset( void )
    {
      unsigned int i;

      WAIT_CTRL(); // NOP_1();
      RES_I2C_CLK();
      SET_I2C_CLK_OUT();
      SET_I2C_DATA_IN();

      for (i = 9; i; i--)
      {
        RES_I2C_CLK();

        WAIT_CTRL(); // NOP_1();
        SET_I2C_CLK();

        WAIT_WR_HI(); // NOP_3();
        if (GET_I2C_DATA())
          break;
      }

      WAIT_CTRL(); // NOP_1();
      I2C_startCond();

      WAIT_CTRL(); // NOP_1();
      I2C_stopCond();
    }

    /* Wait for acknowledge after writing
    ------------------------------------------------------------------------- */
    UBYTE EEPROM_waitForAck( void )
    {
      UBYTE ack_wait;

      I2C_startCond();

      I2C_write(EEPROM_I2C_ADR); /* Block address 0 ??? */

      WAIT_CTRL();
      WAIT_CTRL();

      ack_wait = I2C_getack();

      return( !ack_wait );
    }

    /* Reset interface to EEPROM
    ------------------------------------------------------------------------- */
    void EEPROM_reset( void )
    {
      SET_WC();  /* Set write protection */
      SET_WC_OUT();

      I2C_reset();
    }


    /* Testfunktionen
    ------------------------------------------------------------------------- */
    #ifdef EEPROM_SONDER_TEST

    /* Send an invalid sequence to the EEPROM.
    ------------------------------------------------------------------------- */
    void EEPROM_invalid_access( void )
    {
    }

    #endif /* EEPROM_SONDER_TEST */


    #endif /* #ifndef TOOL_CHAIN_KEIL */


#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
