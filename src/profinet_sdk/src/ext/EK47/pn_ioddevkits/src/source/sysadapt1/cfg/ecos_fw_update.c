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
/*  F i l e               &F: ecos_fw_update.c                          :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  interface between PNIO stack and Board Support Package  simulation       */
/*                                                                           */
/*****************************************************************************/

#include "compiler.h"

#include "pniousrd.h"

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_ERTEC)
    #include "compiler_stdlibs.h"
    //#include "pniousrd.h"
    #include "iodapi_rema.h"
    #include "os.h"
    #include "os_taskprio.h"
    #include "nv_data.h"

#define FLASH_FIRMWARE_START 0
#define FLASH_FIRMWARE_SIZE  0x200000
#define CYG_FLASH_ERR_OK  1

#if(PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200_400)

	#define FLASH_FIRMWARE_START    FLASH_BASE
	#define FLASH_FIRMWARE_SIZE   	0x003d0000
    #define FLASH_TRACE_START       (FLASH_FIRMWARE_START + FLASH_FIRMWARE_SIZE)
    #define FLASH_TRACE_SIZE        0x00020000

#elif (PNIOD_PLATFORM & (PNIOD_PLATFORM_ECOS_EB200P | PNIOD_PLATFORM_POSIX_EB200P))

    #define FLASH_FIRMWARE_START    U_EMC_MEM_ASYNC_CHIP_SELECT0__START
	#define FLASH_FIRMWARE_SIZE   	0x00800000	// take the first 8MB  (max. (FLASH_SIZE_TOTAL - FLASH_SIZE_RESERVED) bytes are available..)
    #define FLASH_TRACE_START       (U_EMC_MEM_ASYNC_CHIP_SELECT0__START + 0x00E70000)
	#define SPI_FLASH_TRACE_START	0x00370000
    #define FLASH_TRACE_SIZE        0x00080000  // 0x__FD0000..0x__FEFFFF => 0x__FF0000..0x__FFFFFF is used by user data

#endif

    PNIO_BOOL      EnableOsFlashPrintf = 0;

	/*------------------------------------------------------------------------------
	 *
	 * Function   : TcpFlashFirmware  (void* pBuf, PNIO_UINT32 BufSize)
	 *
	 * Description: erases the flash and programs new image
	 *
	 *
	 * Returns    : PNIO_OK, PNIO_NOT_OK
	 *
	 *----------------------------------------------------------------------------*/
	PNIO_UINT32 TcpFlashFirmware  (void* pBuf, PNIO_UINT32 BufSize)
	{
          PNIO_UINT32 status;
          PNIO_UINT32 err_addr;
        PNIO_UINT8  *pFlash8 = (PNIO_UINT8*) FLASH_FIRMWARE_START;
        PNIO_UINT8  *pBuf8   = (PNIO_UINT8*) pBuf;
        static PNIO_UINT8  spi_flash_validity_check = 0x5a;
        static PNIO_UINT32 reg1, reg2, reg3;
        PNIO_UINT16 tcm_size;
        PNIO_UINT32 i;

     //   EnableOsFlashPrintf = PNIO_TRUE;

		#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200_400)
        PNIO_ConsolePrintf("\n(0) Disable data cache ...\n    ");
        {
            PNIO_INT d_cache = 0;
            PNIO_INT i_cache = 0;
            /* ecos flash operations do not disable data cache anymore due to uncached NvData section */
            HAL_DATA_CACHE_OFF(d_cache, i_cache); /*lint !e529 !e530 assembler code is not checked correctly */
            PNIO_UNUSED_ARG(d_cache);
            PNIO_UNUSED_ARG(i_cache);
        } /*lint !e438 last value assigned is not used */
		#else
       	// for ERTEC200P the flash region is configured via MMU as uncached/unbuffered
		#endif

        PNIO_ConsolePrintf("\nChoose flash to be loaded:\n");
        PNIO_ConsolePrintf("  [1]  NOR flash\n");
        PNIO_ConsolePrintf("  [2]  SPI flash\n");
        PNIO_INT32 PressedKey = OsGetChar();

        if( '1' == PressedKey )

        {
            // ******************* flash the image ***********
            PNIO_ConsolePrintf("\n(1) Erase flash ...\n    ");
    		status = flash_erase(FLASH_FIRMWARE_START, FLASH_FIRMWARE_SIZE, &err_addr);
    		if (status != CYG_FLASH_ERR_OK)
    		{
    			//PNIO_ConsolePrintf("\nERROR flash_erase(%s) @%p: %s\n","FIRMWARE_SECTOR",err_addr,flash_errmsg(status));
    			return (PNIO_NOT_OK);
    		}
    		PNIO_ConsolePrintf("    OK, done\n");

    		// *************** check if flash is really erased *********
    		PNIO_ConsolePrintf("\n(2) Erase Verify  ...");
            for (i = 0; i < FLASH_FIRMWARE_SIZE; i++)
            {
                if (*(pFlash8+i) != 0xff)
                {
                	PNIO_ConsolePrintf("ERROR flash is not erased at offset 0x%x\n", i);
    			    return (PNIO_NOT_OK);
                }
            }
            PNIO_ConsolePrintf(" OK, done\n");

    		// ******************* program flash***********
            PNIO_ConsolePrintf("\n(3) Program Flash ...\n    ");
            status = flash_program((FLASH_FIRMWARE_START),pBuf, BufSize,&err_addr);
    		if (status != CYG_FLASH_ERR_OK)
    		{
    			//PNIO_ConsolePrintf("\nERROR flash_program(%s) @%p: %s\n","FIRMWARE_SECTOR",err_addr,flash_errmsg(status));
    			return (PNIO_NOT_OK);
    		}
    		PNIO_ConsolePrintf("    OK, done\n");

    		// *************** verify firmware *********
    		PNIO_ConsolePrintf("\n(4) Program Verify ...");
            for (i = 0; i < BufSize; i++)
            {
                if (*(pFlash8+i) != *(pBuf8+i))
                {
                	PNIO_ConsolePrintf("\nERROR flash program at offset 0x%x: value = 0x%x (0x%x) \n",
                                 i, *(pFlash8+i), *(pBuf8+i) );
    			    return (PNIO_NOT_OK);
                }
            }

            PNIO_ConsolePrintf(" OK, done\n");
           // EnableOsFlashPrintf = PNIO_FALSE;
        	/*reset nv data*/
        	Bsp_nv_data_clear( PNIO_RTF_RES_FWUP );
    		return (PNIO_OK);
        }
        if( '2' == PressedKey )
        {
        	if( MODE_SPI_0x03 != ( REG32( U_SCRB__BOOT_REG ) &0xf ) )
        	{
				reg1 = REG32(U_GPIO__GPIO_IOCTRL_0);
				reg2 = REG32(U_GPIO__GPIO_PORT_MODE_0_H);
				reg3 = REG32(U_GPIO__GPIO_OUT_0);
				FLASH_INIT;
        	}
        	spi_in_use = 1;
            // ******************* flash the image ***********
            PNIO_ConsolePrintf("\n(1) Erase flash ...    ");
            /*SPI flashes are erased in the process of programming*/
    		/*for SPI flash, this macro leads to dummy function*/
            status = FLASH_ERASE( ( unsigned int )pFlash8, BufSize, ( unsigned int *)&err_addr );

    		if (status != CYG_FLASH_ERR_OK)
    		{
    			return (PNIO_NOT_OK);
    		}
    		PNIO_ConsolePrintf("    OK, done\n");

    		// *************** check if flash is really erased *********
    		PNIO_ConsolePrintf("\n(2) Erase Verify  ...");
    		/*SPI flashes are erased in the process of programming*/
    		/*for SPI flash, this macro leads to dummy function*/
    		status = FLASH_ERASE_CHECK( ( unsigned int )pFlash8, BufSize );
    		if (status != CYG_FLASH_ERR_OK)
    		{
    		    return (PNIO_NOT_OK);
    		}
            PNIO_ConsolePrintf(" OK, done\n");

    		// ******************* program flash***********
            PNIO_ConsolePrintf("\n(3) Program Flash ...    ");

            /*first byte is value 0x5a for check flash validity*/
    		status = FLASH_PROGRAM( ( unsigned int )pFlash8, ( &spi_flash_validity_check ), 1, ( unsigned int *)&err_addr );
    		if (status != CYG_FLASH_ERR_OK)
    		{
    			PNIO_ConsolePrintf("\nERROR flash_program length(%s) @%p: %s\n","FIRMWARE_SECTOR",err_addr,flash_errmsg(status));
    			return (PNIO_NOT_OK);
    		}
    		pFlash8 += 1;

            /*if working with SPI flash, than first two bytes are length of SPI section*/
    		tcm_size = ( ( PNIO_UINT16 )( *( pBuf8 + BufSize - 2 ) ) ) << 8 ;
    		tcm_size |= ( PNIO_UINT8 )( *( pBuf8 + BufSize - 1 ) );
    		status = FLASH_PROGRAM( ( unsigned int )pFlash8, ( unsigned char * )( &tcm_size ), 2, ( unsigned int *)&err_addr );
    		if (status != CYG_FLASH_ERR_OK)
    		{
    			PNIO_ConsolePrintf("\nERROR flash_program length(%s) @%p: %s\n","FIRMWARE_SECTOR",err_addr,flash_errmsg(status));
    			return (PNIO_NOT_OK);
    		}
    		pFlash8 += 2;
    		pFlash8 += 0x44;

    		status = FLASH_PROGRAM( ( unsigned int )pFlash8, pBuf, BufSize, ( unsigned int *)&err_addr );
    		if (status != CYG_FLASH_ERR_OK)
    		{
    			PNIO_ConsolePrintf("\nERROR flash_program(%s) @%p: %s\n","FIRMWARE_SECTOR",err_addr,flash_errmsg(status));
    			return (PNIO_NOT_OK);
    		}
    		PNIO_ConsolePrintf("    OK, done\n");

    		// *************** verify firmware *********
    		PNIO_ConsolePrintf("\n(4) Program Verify ...");
            if( PNIO_NOT_OK == FLASH_VERIFY( ( unsigned char * )pFlash8, pBuf8, BufSize ) )
			{
				return (PNIO_NOT_OK);
			}

            PNIO_ConsolePrintf(" OK, done\n");
            EnableOsFlashPrintf = PNIO_FALSE;
            spi_in_use = 0;
        	if( MODE_SPI_0x03 != ( REG32( U_SCRB__BOOT_REG ) &0xf ) )
        	{
				REG32(U_GPIO__GPIO_IOCTRL_0) = reg1;
				REG32(U_GPIO__GPIO_PORT_MODE_0_H) = reg2;
				REG32(U_GPIO__GPIO_OUT_0) = reg3;
        	}
        	/*reset nv data*/
        	Bsp_nv_data_clear( PNIO_RTF_RES_FWUP );
    		return (PNIO_OK);
        }
    	PNIO_ConsolePrintf("\n unsupported choice %c\n", PressedKey);

    	return (PNIO_NOT_OK);
	}

	/*------------------------------------------------------------------------------
	 *
	 * Function   : FlashTraceBuf  (void* pBuf, PNIO_UINT32 BufSize)
	 *
	 * Description: erases the appropriate flash sectors and stores the buffer
	 *
	 *
	 * Returns    : PNIO_OK, PNIO_NOT_OK
	 *
	 *----------------------------------------------------------------------------*/
	PNIO_UINT32 FlashTraceBuf  (void* pBuf, PNIO_UINT32 BufSize)
	{
		PNIO_UINT32 status;
		void*       err_addr;
        PNIO_UINT8  *pFlash8 = (PNIO_UINT8*) FLASH_TRACE_START;
        PNIO_UINT8 * tBuf;
        PNIO_UINT32 i;
        volatile PNIO_UINT32 boot_reg;
        OsAllocF ((void**) &tBuf, FLASH_TRACE_SIZE);
        PNIO_UINT8  *pBuf8   = (PNIO_UINT8*) tBuf;

		// ******************* flash the image ***********
        if (BufSize > FLASH_TRACE_SIZE)
        {
        	PNIO_ConsolePrintf ("ERROR: flash size too small for trace buffer, check configuration\n");
			return (PNIO_NOT_OK);
        }

        /*first we have to copy buffer , so it will not be corrupted by new traces*/
        OsMemCpy(tBuf, pBuf, FLASH_TRACE_SIZE);

        PNIO_ConsolePrintf("now erase flash....");


	    boot_reg = REG32(U_SCRB__BOOT_REG);
	    boot_reg &= 0xf;

	    if(MODE_SPI_0x03 == boot_reg)
	    {
	    	status = FLASH_ERASE(SPI_FLASH_TRACE_START,
								 FLASH_TRACE_SIZE,
								 ( unsigned int *)&err_addr);
	    }
	    else
	    {
	    	status = flash_erase((void *)(pFlash8),
	    						FLASH_TRACE_SIZE,
								&err_addr);
	    }
		if (status != FLASH_ERR_OK)
		{
			PNIO_ConsolePrintf("\nflash_erase(%s) error @%p: %s\n","TRACE_SECTOR",err_addr,flash_errmsg(status));
	        OsFree(tBuf);
			return (PNIO_NOT_OK);
		}
		PNIO_ConsolePrintf("ok, done\n");

		// *************** check if flash is really erased *********
		PNIO_ConsolePrintf("now verify if flash is erased....");

	    if(MODE_SPI_0x03 == boot_reg)
	    {
	    	FLASH_ERASE_CHECK( SPI_FLASH_TRACE_START, FLASH_TRACE_SIZE );
	    }
	    else
	    {
			for (i = 0; i < FLASH_TRACE_SIZE; i++)
			{
				if (*(pFlash8+i) != 0xff)
				{
					PNIO_ConsolePrintf("\nflash at offset 0x%x is not erased\n", i);
			        OsFree(tBuf);
					return (PNIO_NOT_OK);

				}
			}
        }
        PNIO_ConsolePrintf("ok, done\n");

		// ******************* program flash***********
        PNIO_ConsolePrintf("now program flash....");

	    if(MODE_SPI_0x03 == boot_reg)
	    {
	    	status = FLASH_PROGRAM(SPI_FLASH_TRACE_START, tBuf, FLASH_TRACE_SIZE, ( unsigned int *)&err_addr);
	    }
	    else
	    {
	    	status = flash_program((void*)(pFlash8), tBuf, FLASH_TRACE_SIZE, &err_addr);
	    }
		if (status != FLASH_ERR_OK)
		{
			PNIO_ConsolePrintf("\nflash_program(%s) error @%p: %s\n","TRACE_SECTOR",err_addr,flash_errmsg(status));
	        OsFree(tBuf);
			return (PNIO_NOT_OK);
		}
		PNIO_ConsolePrintf("ok, done\n");

		// *************** verify trace *********
		PNIO_ConsolePrintf("now verify entries in flash....");
	    if(MODE_SPI_0x03 == boot_reg)
	    {
			if( PNIO_NOT_OK == FLASH_VERIFY( ( unsigned char * )SPI_FLASH_TRACE_START, pBuf8, BufSize ) )
			{
		        OsFree(tBuf);
				return (PNIO_NOT_OK);
			}
	    }
	    else
	    {
			for (i = 0; i < FLASH_TRACE_SIZE; i++)
			{
				if (*(pFlash8+i) != *(pBuf8+i))
				{
					PNIO_ConsolePrintf("\nflash error at offset 0x%x: value = 0x%x (0x%x) \n",
								 i,
								 *(pFlash8+i),
								 *(pBuf8+i)
								);
			        OsFree(tBuf);
					return (PNIO_NOT_OK);

				}
			}
	    }
        PNIO_ConsolePrintf("ok, done\n");
        OsFree(tBuf);
		return (PNIO_OK);
	}


	/*------------------------------------------------------------------------------
	 *
	 * Function   : RestoreTraceBuf  (void* pBuf, PNIO_UINT32 BufSize)
	 *
	 * Description: read trace from flash and store to trace buffer
	 *
	 *
	 * Returns    : PNIO_OK, PNIO_NOT_OK
	 *
	 *----------------------------------------------------------------------------*/
	PNIO_UINT32 RestoreTraceBuf  (void* pBuf, PNIO_UINT32 BufSize)
	{
        PNIO_UINT8  *pFlash8 = (PNIO_UINT8*) FLASH_TRACE_START;
        PNIO_UINT8  *pBuf8   = (PNIO_UINT8*) pBuf;
        PNIO_UINT32 i;
        PNIO_UINT32 boot_reg;
        PNIO_UINT8 error;

		// ******************* flash the image ***********

	    boot_reg = REG32(U_SCRB__BOOT_REG);
	    boot_reg &= 0xf;

        if (BufSize > FLASH_TRACE_SIZE)
        {
        	PNIO_ConsolePrintf ("ERROR: flash size too small for trace buffer restore\n");
			return (PNIO_NOT_OK);
        }
        PNIO_ConsolePrintf("read trace from flash ... ");

	    if(MODE_SPI_0x03 == boot_reg)
	    {
	    	FLASH_READ( ( unsigned int )SPI_FLASH_TRACE_START, pBuf, BufSize, ( unsigned int *)&error );
	    }
	    else
	    {
			for (i = 0; i < BufSize; i++)
			{
				*(pBuf8+i) = *(pFlash8+i);
			}
	    }
        PNIO_ConsolePrintf("ok, done\n");
		return (PNIO_OK);
	}

#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
