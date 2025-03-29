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
/*  F i l e               &F: ecos_ertec_bsp_spi_flash.c                :F&  */
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

/**
* @file     rtos_rzt2_flash_qspi.c
* @brief    functions for SPI flash usage
*
*
*/

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*                                                                           */
/*  Date        Version    What                                              */
/*---------------------------------------------------------------------------*/

#include "compiler.h"

#if (PNIOD_PLATFORM && PNIOD_PLATFORM_RZT2)
#include <pnioerrx.h>
#include <os.h>
#include <pniousrd.h>
#include "r_xspi_qspi.h"
#include "r_spi_flash_api.h"


extern void OsFlashLock(void);
extern void OsFlashUnlock(void);
/********************************internal function prototypes*******************************/
/*******************************************************************************************/

static int		flash_identify					( void );

static void 	flash_wait_ready_rsk		( void );
static int 		flash_read_status_rsk		( unsigned char * byte_1, unsigned char * byte_2 );
static void 	flash_read_dev_info_rsk		( unsigned char * receive_addr );
  int 		flash_program_one_page_rsk	( unsigned int flash_strt_addr, unsigned char * transmit_addr, signed int data_size, unsigned int * pError );
static int		flash_read_rsk				( unsigned int flash_strt_addr, unsigned char * receive_addr, unsigned int data_size, unsigned int * pError );
static int		flash_program_rsk			( unsigned int flash_strt_addr, unsigned char * transmit_addr, signed int data_size, unsigned int * pError );
static int 		flash_read_erase_write_rsk	( unsigned int flash_strt_addr, unsigned char * transmit_addr, signed int data_size, unsigned int * pError );
static int 		flash_erase_rsk				( unsigned int flash_strt_addr, unsigned int * pError );
static void 	flash_write_enable_rsk		( void );

void qspi_set_Quad_Mode_enable(void);
/********************************glabal variables*******************************************/
/*******************************************************************************************/

unsigned char flash_type = 0;
int g_flash_init = 0;

#define FLASH_PAGE_SIZE				64
#define FLASH_SECTOR_SIZE			4096
#define QSPI_DEVICE_START_ADDRESS   (0x60000000)

extern const spi_flash_cfg_t g_qspi0_cfg;
extern xspi_qspi_instance_ctrl_t g_qspi0_ctrl;

extern void handle_error (fsp_err_t err);

/*******************************************************************************************/
/**
 *  @brief Init SPI and decide flash type
 *
 *  @return      			Void
 *
 *
 */
void spi_flash_init( void )
{
	OsFlashLock();
	fsp_err_t err = R_XSPI_QSPI_Open(&g_qspi0_ctrl, &g_qspi0_cfg);

	if(FSP_ERR_ALREADY_OPEN == err)
	{
		err = R_XSPI_QSPI_Close(&g_qspi0_ctrl);
		err = R_XSPI_QSPI_Open(&g_qspi0_ctrl, &g_qspi0_cfg);
	}

	if(FSP_SUCCESS == err)
	{
		qspi_set_Quad_Mode_enable();
		g_flash_init = 1;
		flash_type = flash_identify(  );
	}

	OsFlashUnlock();
}	/*spi_flash_init(  )*/



/**
 *  @brief Identifies flash type
 *
 *  @return      			Returns	flash ID or 0 if unknown/fail
 *
 *
 */
static int flash_identify( void )
{

	return 0;
}	/*flash_identify(  )*/



/**
 *  @brief Decide which flash write function to call, call it
 *
 *  @param[in]   flash_strt_addr 	Address in SPi flash, where write will begin
 *  @param[in]   * transmit_addr 	Pointer to data, which will be written
 *  @param[in]   data_size 			Amount of data to write
 *  @param[out]  * pError 			Pointer for error return
 *  @return      					Returns	0 if OK, 1 if error
 *
 *
 */
int spi_flash_program(
		unsigned int flash_strt_addr,
		unsigned char * transmit_addr,
		signed int data_size,
		unsigned int * pError )
{
	int Status = PNIO_OK;
	int write = 0;

	OsFlashLock();
	write = flash_read_erase_write_rsk( flash_strt_addr, transmit_addr, data_size, pError );
	if( data_size != write)
	{
		if(NULL != pError)
		{
		 	*pError = (flash_strt_addr + write);
		}

		Status = PNIO_NOT_OK;
	}
	OsFlashUnlock();
	return Status;
}	/*spi_flash_program(  )*/




/**
 *  @brief Decide which flash read function to call, call it
 *
 *  @param[in]   flash_strt_addr 	Address in SPi flash, where read will begin
 *  @param[out]  * receive_addr 	Pointer to buffer, where read data will be placed
 *  @param[in]   data_size 			Amount of data to read
 *  @param[out]  * pError 			Pointer for error return
 *  @return      					Returns	0 if Error, return value of read function if OK
 *
 *
 */
int spi_flash_read(
		unsigned int flash_strt_addr,
		unsigned char * receive_addr,
		signed int data_size,
		unsigned int * pError )
{
	int Status = PNIO_OK;
	int read =0;
	OsFlashLock();
	read = flash_read_rsk( flash_strt_addr, receive_addr, data_size, pError );
	if( data_size != read)
	{
		if(NULL != pError)
		{
		 	*pError = (flash_strt_addr + read);
		}

		Status = PNIO_NOT_OK;
	}
	OsFlashUnlock();
	return Status;
}	/*spi_flash_read(  )*/



/**
 *  @brief Dummy function for erase
 *
 *  @param[in]   flash_strt_addr 	Dummy
 *  @param[in]   data_size 			Dummy
 *  @param[out]  * pError 			Pointer for error return
 *  @return      					Returns	0
 *
 *	Dummy function for compatibility with fw update process
 *	Write functions cares about erasing, no need to solve it
 *
 */
int	spi_flash_erase	( unsigned int flash_strt_addr, signed int data_size, unsigned int * pError )
{

	unsigned char *buffer = NULL;
	unsigned int  FlashAddr = flash_strt_addr;
	volatile int sector, sector_begin, addr_in_sector, size;
	unsigned char *dst;
	int Status = PNIO_OK;
	int i;

	OsFlashLock();

	Status = OsAlloc ( ( void** )&buffer, 0xFF, FLASH_SECTOR_SIZE );

	while ( (PNIO_OK == Status) && (0 < data_size) )
	{
		size = 0;
		/*identify sector*/
		sector = FlashAddr / FLASH_SECTOR_SIZE;
		sector_begin = sector * FLASH_SECTOR_SIZE;
		addr_in_sector = FlashAddr % FLASH_SECTOR_SIZE;
		/*copy whole sector? -> read and modify approach not needed*/
		if( ( 0 == addr_in_sector ) && ( data_size >= FLASH_SECTOR_SIZE ) )
		{
			/*erase sector*/
			Status = flash_erase_rsk( sector_begin, pError );
			size = FLASH_SECTOR_SIZE;
		}
		else
		{
			/*download sector*/
			flash_read_rsk( sector_begin, &buffer[ 0 ], FLASH_SECTOR_SIZE, pError );
			/*erase sector*/
			Status = flash_erase_rsk( sector_begin, pError );
			if(PNIO_OK == Status)
			{
				/*modify downloaded sector */
					/*count bytes to write*/
				if( ( 0 == addr_in_sector ) || ( data_size < ( FLASH_SECTOR_SIZE - addr_in_sector ) ) )
				{	/*last write or ( first and also last write ) */
					size = data_size;
				}
				else
				{	/*first write, but there will be next*/
					size = FLASH_SECTOR_SIZE - addr_in_sector;
				}
				/*memcpy to buffer - risk of alignment problems, so done manually byte by byte*/
				dst =  &buffer[ addr_in_sector ];

				for ( i = 0; i < size; i++ )
				{
					*dst = 0xFF;
					dst++;
				}

				/*upload modified sector*/
				flash_program_rsk( sector_begin, &buffer[ 0 ], FLASH_SECTOR_SIZE, pError );
			}
		}

		/*actualise counters and pointers*/
		FlashAddr += size;
		data_size -= size;
		OsWait_ms(1);
	}

	if(NULL != buffer)
	{
		OsFree( buffer );
	}

	OsFlashUnlock();

	return Status;

}	/*spi_flash_erase(  )*/



/**
 *  @brief Erase whole SPI flash
 *
 *  @return      			Returns	0
 *
 *  These erase functions are for 8MB  or 4MB Adesto only
 *
 */
int	spi_flash_chip_erase( void )
{
	int Status = PNIO_OK;

	OsFlashLock();

	flash_wait_ready_rsk(  );
	flash_write_enable_rsk(  );

	/*---------- Sector Erase ----------*/
	if(FSP_SUCCESS == R_XSPI_QSPI_Erase(&g_qspi0_ctrl, (uint8_t *)QSPI_DEVICE_START_ADDRESS, SPI_FLASH_ERASE_SIZE_CHIP_ERASE))
	{
		return PNIO_OK;
	}

	OsFlashUnlock();

	return Status;
}	/*spi_flash_chip_erase(  )*/



/**
 *  @brief Dummy function for erase
 *
 *  @param[in]   flash_strt_addr 	Dummy
 *  @param[in]   data_size 			Dummy
 *  @return      					Returns	0
 *
 *	Dummy function for compatibility with fw update process
 *	Write functions cares about erasing, no need to solve it
 *
 */
int	spi_flash_erase_verify( unsigned int flash_strt_addr, signed int data_size )
{
	return 0;
}	/*spi_flash_erase_verify(  )*/


/**
 *  @brief Compares data in buffer with data in SPI flash
 *
 *  @param[in]   * pFlash 			Start of data in flash
 *  @param[in]   * pBuf 			Pointer to Buffer with data
 *  @param[in]   data_size 			Amount of data to compare
 *  @return      					Returns	PNIO_OK or PNIO_NOT_OK
 *
 *	Reads data from flash, compare two buffers
 *
 */
int spi_flash_verify( unsigned char * pFlash, unsigned char * pBuf, int BufSize )
{
#define FLASH_READ_SIZE 64

	int status = PNIO_OK;
	int nCnt = 0;
	int read = 0;
	int read_size = FLASH_READ_SIZE;
	unsigned int  err_addr = 0U;
	unsigned int flash_addr = (unsigned int)pFlash;
	PNIO_UINT8  read_data[FLASH_READ_SIZE];

	OsFlashLock();

	while( (PNIO_OK == status) && (read < BufSize) )
	{
		if( BufSize < (read + FLASH_READ_SIZE) )
		{
			read_size = (BufSize - read);
		}

		if( (0U == read_size) || (FLASH_READ_SIZE < read_size) )
		{
			read_size = FLASH_READ_SIZE;
		}


		read_size = flash_read_rsk( flash_addr, &read_data[0], read_size, &err_addr );

		if(0 == read_size)
		{
			status = PNIO_NOT_OK;
		}

		nCnt = 0;
		while( (PNIO_OK == status) && (nCnt < read_size) )
		{
			if(read_data[nCnt] != pBuf[read + nCnt])
			{
				status = PNIO_NOT_OK;
			}
			nCnt++;
		}

		flash_addr += read_size;
		read += read_size;
	}

	OsFlashUnlock();

	return status;
}	/*spi_flash_verify(  )*/

/**
 *  @brief Software write enable
 *
 *  @return      			Void
 *
 *  After some types of operations, the  flash automatically goes to write disable state
 *  It is recommended to call this function before any write or erase request
 *  Internal, implemented in all  SPI flash functions, which needs it
 *
 */
static void flash_write_enable_rsk( void )
{

}	/*flash_write_enble_rsk(  )*/



/**
 *  @brief Reads falsh device ID
 *
 *  @param[out]  * receive_addr 	Buffer to return read data
 *  @return      					Void
 *
 *	Calling function should check read data
 *
 */
__attribute__((unused)) static void flash_read_dev_info_rsk( unsigned char * receive_addr )
{

}	/*flash_read_dev_info_rsk(  )*/



/**
 *  @brief Write data to flash, Simulate RAM write
 *
 *  @param[in]   flash_strt_addr 	Address in SPi flash, where write will begin
 *  @param[in]   * transmit_addr 	Pointer to data, which will be written
 *  @param[in]   data_size 			Amount of data to write
 *  @param[out]  * pError 			Pointer for error return
 *  @return      					Returns	amount of written data
 *
 *  Simulates RAM write: Erase before write not needed and does not damage rest of data in sector
 *  For this, this function performs read and erase, then modify read data and write them
 *  Divides data to sectors ( 4096B ) as it process one sector at a time
 *  Start or end in the middle of sector is not a problem
 *
 */
/*write specified data without damaging data in rest of sector*/
static int flash_read_erase_write_rsk(
		unsigned int flash_strt_addr,
		unsigned char * transmit_addr,
		signed int data_size,
		unsigned int * pError )
{
	unsigned char *buffer = NULL;
	unsigned int  FlashAddr = flash_strt_addr;
	unsigned char *pTransmitAddr = transmit_addr;
	volatile int sector, sector_begin, addr_in_sector, size, program_sum;
	unsigned char *src;
	unsigned char *dst;
	int Status = PNIO_OK;
	int i;

	program_sum = 0;

	Status = OsAlloc ( ( void** )&buffer, 0xFF, FLASH_SECTOR_SIZE );

	while ( (PNIO_OK == Status) && (0 < data_size) )
	{
		size = 0;
		/*identify sector*/
		sector = FlashAddr / FLASH_SECTOR_SIZE;
		sector_begin = sector * FLASH_SECTOR_SIZE;
		addr_in_sector = FlashAddr % FLASH_SECTOR_SIZE;
		/*copy whole sector? -> read and modify approach not needed*/
		if( ( 0 == addr_in_sector ) && ( data_size >= FLASH_SECTOR_SIZE ) )
		{
			/*erase sector*/
		//	Status = flash_erase_rsk( sector_begin, pError );

			/*upload new sector*/
			if(PNIO_OK == Status)
			{
				 size = flash_program_rsk( sector_begin, pTransmitAddr, FLASH_SECTOR_SIZE, pError );
			}

			src = (unsigned char*)pTransmitAddr;
			dst = (unsigned char*)sector_begin;

		}
		else
		{
			/*download sector*/
			flash_read_rsk( sector_begin, &buffer[ 0 ], FLASH_SECTOR_SIZE, pError );
			/*erase sector*/
			Status = flash_erase_rsk( sector_begin, pError );
			if(PNIO_OK == Status)
			{
				/*modify downloaded sector */
					/*count bytes to write*/
				if( ( 0 == addr_in_sector ) || ( data_size < ( FLASH_SECTOR_SIZE - addr_in_sector ) ) )
				{	/*last write or ( first and also last write ) */
					size = data_size;
				}
				else
				{	/*first write, but there will be next*/
					size = FLASH_SECTOR_SIZE - addr_in_sector;
				}
				/*memcpy to buffer - risk of alignment problems, so done manually byte by byte*/
				dst =  &buffer[ addr_in_sector ];
				src = pTransmitAddr;
				for ( i = 0; i < size; i++ )
				{
					*dst = *src;
					src++;
					dst++;
				}

				/*upload modified sector*/
				flash_program_rsk( sector_begin, &buffer[ 0 ], FLASH_SECTOR_SIZE, pError );

				src = pTransmitAddr;
				dst = (unsigned char*)sector_begin;
				dst = &dst[addr_in_sector];
			}
		}
#if 0
		if(PNIO_OK == Status)
		{
			i = 0;
			while( (PNIO_OK == Status) && (i < size) )
			{
				if(src[i] != dst[i])
				{
					Status = PNIO_NOT_OK;
					if(NULL != pError)
					{
						*pError = (unsigned int)(sector_begin + i);
					}
				}
				i++;
			}
		}
#endif

		/*actualise counters and pointers*/
		int tmp = program_sum;
		program_sum = tmp + size;
		pTransmitAddr += size;
		FlashAddr += size;
		data_size -= size;
		OsWait_ms(1);


	}

	if(NULL != buffer)
	{
		OsFree( buffer );
	}
	
	return program_sum;
}	/*flash_read_erase_write_rsk(  )*/



/**
 *  @brief Write up to one page ( 256B ) of data to flash
 *
 *  @param[in]   flash_strt_addr 	Address in SPi flash, where write will begin
 *  @param[in]   * transmit_addr 	Pointer to data, which will be written
 *  @param[in]   data_size 			Amount of data to write
 *  @param[out]  * pError 			Pointer for error return
 *  @return      					Returns	amount of written data
 *
 *  Flash write: flash have to be erased before
 *  Cannot go over edge of page ( start at one page and end on other )
 *
 */
int flash_program_one_page_rsk(
		unsigned int flash_strt_addr,
		unsigned char * transmit_addr,
		signed int data_size,
		unsigned int * pError )
{
	int wrote = 0;

	flash_wait_ready_rsk(  );
	flash_write_enable_rsk(  );
	if(FSP_SUCCESS == R_XSPI_QSPI_Write(&g_qspi0_ctrl,  (uint8_t *)transmit_addr, (uint8_t *)flash_strt_addr, data_size))
	{
		wrote = data_size;
	}
	else
	{
		wrote = 0;
	}

	return ( wrote );
}	/*flash_program_one_page_rsk(  )*/



/**
 *  @brief Divide data into pages, call write
 *
 *  @param[in]   flash_strt_addr 	Address in SPi flash, where write will begin
 *  @param[in]   * transmit_addr 	Pointer to data, which will be written
 *  @param[in]   data_size 			Amount of data to write
 *  @param[out]  * pError 			Pointer for error return
 *  @return      					Returns	amount of written data
 *
 *  Finds beginning of page to write
 *  Any data length
 *  Flash area to write have to be previously erased
 *
 */
static int flash_program_rsk(
		unsigned int flash_strt_addr,
		unsigned char * transmit_addr,
		signed int data_size,
		unsigned int * pError )
{
	unsigned int tmp, wrote = 0, to_write;
	/*phase 1 - finish incomplete page*/
	tmp = flash_strt_addr % FLASH_PAGE_SIZE;
	to_write = FLASH_PAGE_SIZE - tmp;

	if( (0 < tmp) && (0 < to_write) )
	{
		wrote = flash_program_one_page_rsk( flash_strt_addr, transmit_addr, ( to_write ), pError );
		flash_strt_addr += ( to_write );
		data_size -= ( to_write );
		transmit_addr += ( to_write );
	}

	/*phase 2 write all remaining pages*/
	while( data_size > 0 )
	{
		/*complete or incomplete page data size*/
		if( data_size > FLASH_PAGE_SIZE )
		{
			to_write = FLASH_PAGE_SIZE;
		}
		else
		{
			to_write = data_size;
		}
		wrote += flash_program_one_page_rsk( flash_strt_addr, transmit_addr, ( to_write ), pError );
		flash_strt_addr += ( to_write );
		data_size -= ( to_write );
		//lint -e{662} possible creation of out-of-bounds pointer ('Integer' beyond end of data) by operator 'String'
		transmit_addr += ( to_write );
	}
	return wrote;
}	/*flash_program_rsk(  )*/



/**
 *  @brief Reads data from SPI flash to provided buffer
 *
 *  @param[in]   flash_strt_addr 	Address in SPi flash, where read will begin
 *  @param[out]  * receive_addr 	Pointer to buffer, where read data will be placed
 *  @param[in]   data_size 			Amount of data to read
 *  @param[out]  * pError 			Pointer for error return
 *  @return      					Returns	amount of read data
 *
 *
 */
static int flash_read_rsk(
		unsigned int flash_strt_addr,
		unsigned char * receive_addr,
		unsigned int data_size,
		unsigned int * pError )
{
	int read = 0;
	unsigned char *pSrc = (unsigned char*)flash_strt_addr;
	flash_wait_ready_rsk(  );
	flash_write_enable_rsk(  );

	while(read < data_size)
	{
		receive_addr[read] = pSrc[read];
		read++;
	}
	//OsMemCpy((PNIO_VOID*)receive_addr, (PNIO_VOID*)flash_strt_addr, data_size);

#if 0
    spi_flash_direct_transfer_t transfer = {0};
    //spi_flash_cfg_t *p_cfg =(spi_flash_cfg_t*) g_qspi0_ctrl->p_cfg;

    transfer.command        = g_qspi0_ctrl.p_cfg->read_command;
    transfer.command_length = data_size;
    transfer.address = flash_strt_addr;
    transfer.data = (uint32_t)receive_addr;
    if(FSP_SUCCESS == R_XSPI_QSPI_DirectTransfer(&g_qspi0_ctrl, &transfer, SPI_FLASH_DIRECT_TRANSFER_DIR_READ))
    {
    	read =  data_size;
    }
#endif
   // R_XSPI_QSPI_direct_transfer(g_qspi0_ctrl->p_cfg, &direct_command, SPI_FLASH_DIRECT_TRANSFER_DIR_READ);
	return read;
}	/*flash_read_rsk(  )*/



/**
 *  @brief Asks flash for its status
 *
 *  @param[out]  byte_1 	Return pointer for first byte of status
 *  @param[out]  byte_2 	Return pointer for second byte of status
 *  @return      			Returns	1
 *
 *
 */
__attribute__((unused)) static int flash_read_status_rsk( unsigned char * byte_1, unsigned char * byte_2 )
{
	if(NULL != byte_1)
	{
		spi_flash_status_t flash_status;
	    flash_status.write_in_progress = 0;

		R_XSPI_QSPI_StatusGet(&g_qspi0_ctrl, &flash_status);
		*((unsigned char*) byte_1) = flash_status.write_in_progress;
		return 1;
	}
	else
	{
		return 0;
	}
}	/*flash_read_status_rsk(  )*/



/**
 *  @brief Wait if flash is busy
 *
 *  @return      			Void
 *
 *	Blocking, cyclically check flash status until it is not ready
 *
 */
static void flash_wait_ready_rsk( void )
{
	fsp_err_t err = FSP_SUCCESS;
	spi_flash_status_t flash_status;
	flash_status.write_in_progress = 1;
	while (0U < flash_status.write_in_progress)
	{
		err = R_XSPI_QSPI_StatusGet(&g_qspi0_ctrl, &flash_status);
	}
	
	(void)(err);
}	/*flash_wait_ready_rsk(  )*/



/**
 *  @brief Erase one 4K sector
 *
 *  @param[in]   flash_strt_addr 	Adress in flash, sector around it will be erased
 *  @param[out]  * pError 			Pointer for error return
 *  @return      					Returns	0
 *
 *	Start address might be anywhere in sector
 *	Always erase one whole sector
 *
 */
int flash_erase_rsk(
		unsigned int flash_strt_addr,
		unsigned int * pError )
{
	unsigned int status = PNIO_OK;
	/* Enter QPI mode */
	flash_wait_ready_rsk(  );
	flash_write_enable_rsk(  );

	switch(0xFF000000 & flash_strt_addr)
	{
	case 0x40000000: // XSPI0
	case 0x48000000: // XSPI1
		flash_strt_addr &= 0x0FFFFFFF;
		flash_strt_addr += 0x60000000;
		break;
	case 0x60000000:
	case 0x68000000:
		break;
	default:
		status = PNIO_NOT_OK;
		break;
	}

	/*---------- Sector Erase ----------*/
	if( (PNIO_OK == status) &&
		(FSP_SUCCESS != R_XSPI_QSPI_Erase(&g_qspi0_ctrl, (uint8_t *)flash_strt_addr, FLASH_SECTOR_SIZE)) )
	{
		status = PNIO_NOT_OK;
	}

	return status;

}	/*flash_erase_rsk(  )*/

void qspi_set_Quad_Mode_enable(void)
{
  /* For following settings, temporary set protocol 1S-1S-1S */
    R_XSPI_QSPI_SpiProtocolSet(&g_qspi0_ctrl, SPI_FLASH_PROTOCOL_1S_1S_1S);

    /*--------------- Read status register(RDSR) (05h) ---------------*/
    spi_flash_direct_transfer_t direct_command_RDSR = {0};
    direct_command_RDSR.command        = 0x05U;
    direct_command_RDSR.command_length = 1U;
    direct_command_RDSR.data_length    = 1U;

    fsp_err_t err = R_XSPI_QSPI_DirectTransfer(&g_qspi0_ctrl, &direct_command_RDSR, SPI_FLASH_DIRECT_TRANSFER_DIR_READ);
    handle_error(err);
    uint32_t read_status = direct_command_RDSR.data;
    FSP_PARAMETER_NOT_USED(read_status);
    /*----------------------------------------------------------*/

    /*---------------- Read configure register(RDCR) (15h) ---------------*/
    spi_flash_direct_transfer_t direct_command_RDCR = {0};
    direct_command_RDCR.command        = 0x15U;
    direct_command_RDCR.command_length = 1U;
    direct_command_RDCR.data_length    = 1U;

    err = R_XSPI_QSPI_DirectTransfer(&g_qspi0_ctrl, &direct_command_RDCR, SPI_FLASH_DIRECT_TRANSFER_DIR_READ);
    handle_error(err);
    uint32_t read_configure = direct_command_RDCR.data;
    FSP_PARAMETER_NOT_USED(read_configure);
    /*--------------------------------------------------------------*/
#if 0
    /*----------------- Enter 4byte address mode(EN4B) (0xB7) ----------------*/
    spi_flash_direct_transfer_t direct_command_EN4B = {0};
    direct_command_EN4B.command        = 0xB7U;
    direct_command_EN4B.command_length = 1U;

    err = R_XSPI_QSPI_DirectTransfer(&g_qspi0_ctrl, &direct_command_EN4B, SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE);
    handle_error(err);
    /*--------------------------------------------------------------*/
#endif
#if 0
    /*--------------- Enable QPI mode(EQIO) (35h)---------------*/
    spi_flash_direct_transfer_t direct_command_EQIO = {0};
    direct_command_EQIO.command        = 0x35U;
    direct_command_EQIO.command_length = 1U;

    err = R_XSPI_QSPI_DirectTransfer(&g_qspi0_ctrl, &direct_command_EQIO, SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE);
    handle_error(err);
    /*----------------------------------------------------------*/

    /* 4S-4S-4S protocol settings. */
    R_XSPI_QSPI_SpiProtocolSet(&g_qspi0_ctrl, SPI_FLASH_PROTOCOL_4S_4S_4S);
#endif
    /*--------------- Read status register(RDSR) (05h) ---------------*/
    spi_flash_direct_transfer_t direct_command_RDSR_2 = {0};
    direct_command_RDSR_2.command        = 0x05U;
    direct_command_RDSR_2.command_length = 1U;
    direct_command_RDSR_2.data_length    = 1U;

    err = R_XSPI_QSPI_DirectTransfer(&g_qspi0_ctrl, &direct_command_RDSR_2, SPI_FLASH_DIRECT_TRANSFER_DIR_READ);
    handle_error(err);
    read_status = direct_command_RDSR_2.data;
    /*----------------------------------------------------------*/

    /*---------------- Read configure register(RDCR) (15h) ---------------*/
    spi_flash_direct_transfer_t direct_command_RDCR_2 = {0};
    direct_command_RDCR_2.command        = 0x15U;
    direct_command_RDCR_2.command_length = 1U;
    direct_command_RDCR_2.data_length    = 1U;

    err = R_XSPI_QSPI_DirectTransfer(&g_qspi0_ctrl, &direct_command_RDCR_2, SPI_FLASH_DIRECT_TRANSFER_DIR_READ);
    handle_error(err);
    read_configure = direct_command_RDCR_2.data;
    /*--------------------------------------------------------------*/
}

void qspi_set_Quad_Mode_disable(void)
{
#if 0
    /*--------------- Reset QPI(RSTQIO) (F5h)---------------*/
    spi_flash_direct_transfer_t direct_command_RSTQIO = {0};
    direct_command_RSTQIO.command        = 0xF5U;
    direct_command_RSTQIO.command_length = 1U;

    fsp_err_t err = R_XSPI_QSPI_DirectTransfer(&g_qspi0_ctrl, &direct_command_RSTQIO, SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE);
    handle_error(err);
    /*----------------------------------------------------------*/
#else
    fsp_err_t err;
#endif
    /* 1S-1S-1S protocol settings. */
    R_XSPI_QSPI_SpiProtocolSet(&g_qspi0_ctrl, SPI_FLASH_PROTOCOL_1S_1S_1S);

    /*--------------- Read status register (05h) ---------------*/
    spi_flash_direct_transfer_t direct_command = {0};
    direct_command.command        = 0x05U;
    direct_command.command_length = 1U;
    direct_command.data_length    = 1U;

    err = R_XSPI_QSPI_DirectTransfer(&g_qspi0_ctrl, &direct_command, SPI_FLASH_DIRECT_TRANSFER_DIR_READ);
    handle_error(err);
    /*----------------------------------------------------------*/

    /*---------------- Read configure register (15h) ---------------*/
    direct_command.command        = 0x15U;
    direct_command.command_length = 1U;
    direct_command.data_length    = 1U;

    err = R_XSPI_QSPI_DirectTransfer(&g_qspi0_ctrl, &direct_command, SPI_FLASH_DIRECT_TRANSFER_DIR_READ);
    handle_error(err);
    /*--------------------------------------------------------------*/

}

#endif	/* #if ( PNIOD_PLATFORM & ) */


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
