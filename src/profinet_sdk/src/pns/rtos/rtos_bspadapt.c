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
/*  F i l e               &F: ecos_ertec_bspadapt.c                     :F&  */
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
* @file     ecos_ertec_bspadapt.c
* @brief    functions for SPI flash usage
*
* Only for Adesto and Winbond flashes
* Covers identification, initialization, read, write and erase of both flashes
* Needs functions from ecos_ertec_bsp_spi.c to work
*
*/

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*                                                                           */
/*  Date        Version    What                                              */
/*---------------------------------------------------------------------------*/
#include "compiler.h"

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_RZT2)
    #include "cm_inc.h"
    #include "compiler_stdlibs.h"
    #include "os.h"
	#include "bspadapt.h"

	#include "nv_data.h"
	#include "nv_data.h"
    #include "os_taskprio.h"
    #include "bsp_api.h"
    #include "r_ether_rzt2.h"


#if 0
/* FreeRTOS includes. */
//#include "FreeRTOS.h"
//#include "task.h"

/* Standard libraries includes. */
#include <stdlib.h>

#if defined(__IAR_SYSTEMS_ICC__)
 #pragma diag_suppress=Pe1644
#endif

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_IP.h"
#include "FreeRTOS_IP_Private.h"

#if defined(__IAR_SYSTEMS_ICC__)
 #pragma diag_default=Pe1644
#endif
#include "NetworkInterface.h"
#include "NetworkBufferManagement.h"

/* Renesas includes. */
#include "r_ether_rzt2.h"
/* #include "log.h"*/

#include "Common_data.h"
#endif

    #define LTRC_ACT_MODUL_ID   110
    #define	IOD_MODULE_ID       110

    #define SET_THREAD_PRIO_TIMER_THRESHOLD         4


	#define BSP_NUM_ISR_VECTORS	(SY_INT_COMMTX926 + 1)  	// see ertec_irq.h  definitions SY_INT_N_xxxxx

    PNIO_UINT32 toggleTimer = 0;
    extern PNIO_UINT32 OsInitialized;

    /**
     *  @brief Sets GPIO (0..31) to GPIO or alternate function 1..3
     *
     *  @param[in]   Gpio 		GPIO number (0..31)
     *  @param[in]   Function 	GPIO (0) or alternate function (1..3)
     *  @param[in]   InOut	 	1: in, 2: out
     *  @return      			PNIO_OK, PNIO_NOT_OK (invalid params)
     *
     *  It is not checked, if the alternate function is available!!
     *
     */
    PNIO_UINT32 Bsp_SetGpioMode (PNIO_UINT32     Gpio,
                                 PNIO_UINT32     Function,
                                 PNIO_GPIO_DIR   InOut)
    {
    	return 0;
    }

    /**
     *  @brief Init function, called during system startup
     *
     *  @return      			Void
     *
     *
     */
    PNIO_UINT32 Bsp_Init  (void)
    {
        BspLed_Open();

		#if ( IOD_INCLUDE_POF == 1 )
        Bsp_PofInit();
		#endif

     return (PNIO_OK);       // nothing to do here
    }

// Settings for Macaddress, one of them needs to be enabled, to switch address, simply comment out
    
    /**
     *  @brief Reads the device mac address
     *
     *  @param[out]  pDevMacAddr	Destination pointer to the device mac address
     *  @return      				PNIO_OK
     *
     *  Mac address is read from non-volatile data
     *
     */
    PNIO_UINT32 Bsp_GetMacAddr  (PNIO_UINT8* pDevMacAddr)
    {
    	PNIO_UINT8* pBootLine;
    	PNIO_UINT32 bootlinelength;

    	if(PNIO_OK == Bsp_nv_data_restore(PNIO_NVDATA_MAC_ADDR, (PNIO_VOID**)&pBootLine, &bootlinelength))
    	{
    		OsMemCpy (pDevMacAddr, pBootLine, 6);
    		Bsp_nv_data_memfree(pBootLine);
            return (PNIO_OK);
    	}

    	if(PNIO_OK == Bsp_nv_data_restore(PNIO_NVDATA_PRIVATE_DATA,(PNIO_VOID**)&pBootLine,&bootlinelength))
    	{
    	    OsMemCpy(pDevMacAddr,pBootLine + 0x103,  	// offset 0x103 for MAC Address (compatible to history...)
    				 6);
    	}else
    	{
    		pBootLine = NULL;
    	}

    	// no valid BootLine or invalid MAC (since Individual/Group Bit for Broadcast is set)
    	if((0 == pBootLine)||(0x00 != (pDevMacAddr[0] & 0x01)))
    	{
	    	pDevMacAddr[0] = 0x74;
	    	pDevMacAddr[1] = 0x90;
	    	pDevMacAddr[2] = 0x50;
	    	pDevMacAddr[3] = 0x10;
#ifdef UPD60620_PHY
	    	pDevMacAddr[4] = 0xe0;
#else
	    	pDevMacAddr[4] = 0xe9;
#endif

		#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB400)
	    	pDevMacAddr[5] = 0x00;
		#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200)
	    	pDevMacAddr[5] = 0x10;
        #elif (PNIOD_PLATFORM & (PNIOD_PLATFORM_ECOS_EB200P | PNIOD_PLATFORM_POSIX_EB200P))
            pDevMacAddr[5] = 0x10;
		#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_RZT2)
            pDevMacAddr[5] = 0x07;
		#else
			#error Unknown Platform
		#endif
    	}

    	Bsp_nv_data_memfree(pBootLine);
        //sysEnetAddrGet(0, pDevMacAddr);

        return (PNIO_OK);
    }



    /**
     *  @brief Reads the specified port mac address
     *
     *  @param[out]  pPortMacAddr 	Destination pointer to the port address
     *  @param[in]   PortNum 		Port number, must be 1... max. portnum
     *  @return      				PNIO_OK
     *
     *  In this simple example implementation the port mac address is build by
     *  incrementing the device mac address
     *
     */
    PNIO_UINT32 Bsp_GetPortMacAddr  (PNIO_UINT8* pPortMacAddr, PNIO_UINT32 PortNum)
    {
	    PNIO_VAR16 setVal;
	    PNIO_UINT8* port_mac_addr = (PNIO_UINT8*)pPortMacAddr;

        // **** get device MAC address ***
        Bsp_GetMacAddr (pPortMacAddr);

        // **** add the port number to the last 2 bytes of the mac address
        setVal.u8[0] = *(port_mac_addr+4);
        setVal.u8[1] = *(port_mac_addr+5);

        // ***** swap to machine endian format and add port-number ***
        setVal.u16 = OsNtohs (setVal.u16);
	    setVal.u16 += (PortNum);

        // ***** swap back to network endian format ***
        setVal.u16 = OsHtons (setVal.u16);

        // **** modify last 2 bytes of mac address
	    *(port_mac_addr+4) = (setVal.u8[0]);
	    *(port_mac_addr+5) = (setVal.u8[1]);

        return (PNIO_OK);
    }


    /**
     *  @brief Init GPIO for optical fiber communication
     *
     *  @return      					Void
     *
     *
     */
		void Bsp_PofInit (void)
		{

		}




#define BSP_WD_TIME	10		/*multiple of 100ms*/



		/**
		 *  @brief Prepare watchdog timer, set time before reset and reset method
		 *
		 *  @param[in]   wd_time 			Time multiplied by wd_granity
		 *  @param[in]   wd_granity 		Time base [BSP_WD_100MS, BSP_WD_10MS, BSP_WD_1MS, BSP_WD_100US]
		 *
		 *  @return      					Void
		 *
		 *
		 */
		void Bsp_hw_watchdog_init(PNIO_UINT32 wd_time, BSP_WD_GRANITY wd_granity)
		{
			/*all wd settings registers are write protect*/
			/*to write them, there have to be password 0x9876 in upper 4 bytes of register*/
			PNIO_UINT32 load_low, load_high, timer_base = 0xBEBC2;

			switch ( wd_granity )
			{
				case BSP_WD_100MS:
				{
					timer_base = 0xBEBC2;
					break;
				}
				case BSP_WD_10MS:
				{
					timer_base = 0x1312D;
					break;
				}
				case BSP_WD_1MS:
				{
					timer_base = 0x1E84;	//truncated
					break;
				}
				case BSP_WD_100US:
				{
					timer_base = 0x30D;		//truncated
					break;
				}
			}

			/*WD 1 have 36 bit counter on 125MHz clock*/
			/*100 ms = 0xBEBC20*/
			/*RELD1 high and low gives together 32 bits for 36 bit value - ignore lowest 4 bits*/
			/*==> 100 ms = 0xBEBC2 written to register*/
			load_high = timer_base * wd_time;
			load_low = load_high & 0x0000ffff;
			load_low |= 0x98760000;
			load_high = load_high >> 16;
			load_high &= 0x0000ffff;
			load_high |= 0x98760000;
#if 0
			REG32(U_WDOG__RELD1_LOW) =  load_low;   // The watchdog is raised to 251 ms
			REG32(U_WDOG__RELD1_HIGH) = load_high;
			REG32(U_WDOG__WD_CTRL_STATUS) = 0x98760004;	//password + reload

			REG32(U_SCRB__ASYN_RES_CTRL_REG) |= 0x01;  // unlock in SCRB
#endif
		}



		/**
		 *  @brief Deinitialize WDT, turn off reset by WDT
		 *
		 *
		 *  @return      					Void
		 *
		 *
		 */
		void Bsp_hw_watchdog_deinit( void )
		{

		}



		/**
		 *  @brief Start WDT. Have to be initialiyed by user before
		 *
		 *
		 *  @return      					Void
		 *
		 *
		 */
		void Bsp_hw_watchdog_start( void )
		{

		}



		/**
		 *  @brief Stop (pause) WDT
		 *
		 *
		 *  @return      					Void
		 *
		 *	Don't forget to trigger before restart
		 *
		 */
		void Bsp_hw_watchdog_stop( void )
		{

		}



		/**
		 *  @brief Trigger WDT - reset wd timer
		 *
		 *
		 *  @return      					Void
		 *
		 *
		 */
		void Bsp_hw_watchdog_trigger( void )
		{

		}

		void BSP_LockPinAccess(void)
		{
			if(PNIO_TRUE == OsInitialized)
			{
			//	OsEnterX(OS_MUTEX_USR_1);
			}
		}

		void BSP_UnlockPinAccess(void)
		{
			if(PNIO_TRUE == OsInitialized)
			{
			//	OsExitX(OS_MUTEX_USR_1);
			}
		}

		/**
		 *  @brief Register Interrupt routine for acyc. receive GPIO
		 *
		 *  @param          int_vector      Number of interrupt vector
         *  @param          gpio_isr        Function called as isr(have to contain acknowledge of interrupt)
         *  @param          gpio_dsr        Function called as dsr
		 *
		 *  @return         void
		 *
		 *  Settings of interrupt handling
		 *
		 */
		PNIO_VOID Bsp_GPIO_set_int_for_acyc(PNIO_UINT32 int_vector, PNIO_VOID* gpio_isr, PNIO_VOID* gpio_dsr)
		{


		}   /* Bsp_GPIO_set_int_for_acyc */


        /**
         *  @brief Register Interrupt routine for acyc. receive GPIO
         *
         *  @param          int_vector      Number of interrupt vector
         *  @param          gpio_isr        Function called as isr(have to contain acknowledge of interrupt)
         *  @param          gpio_dsr        Function called as dsr
         *
         *  @return         void
         *
         *  Settings of interrupt handling
         *
         */
        PNIO_VOID Bsp_GPIO_set_int_for_acyc_confirm(PNIO_UINT32 int_vector, PNIO_VOID* gpio_isr, PNIO_VOID* gpio_dsr)
        {


        }   /* Bsp_GPIO_set_int_for_acyc_confirm */


		/**
		 *  @brief Interrupt acknowledge for isr routine
		 *
		 *  @param          int_vector      Number of interrupt vector
		 *
		 *  @return         void
		 *
		 *  Acknowledge interrupt
		 *
		 */
		PNIO_VOID Bsp_GPIO_isr(PNIO_UINT32 int_vector)
		{

		}   /* PnUsr_xhif_acyc_isr */


		PNIO_UINT32 Bsp_SwiIntConnect  (PNIO_VOID*   pfSwiIntH, PNIO_VOID*   pfSwiIntL)
		{
			return 0;
		}

		PNIO_UINT32 Bsp_ErrIntConnect  (PNIO_VOID*   pfParityErrInt, PNIO_VOID*   pfAccessErrInt)
		{
			return 0;
		}

		PNIO_UINT32 Bsp_Tim1IntConnect (PNIO_VOID*   pfTim1Int)
		{
			return 0;
		}
		PNIO_UINT32 Bsp_Tim1Start      (PNIO_UINT32  Tim1PeriodUs)
		{
			return 0;
		}

		PNIO_VOID   Bsp_Timer1Int           (PNIO_UINT32                int_source)
		{

		}

		void Bsp_EVMA_register_ISR(PNIO_UINT32 vector, void (*isr)(void))
		{

		}

		void Bsp_EVMA_unregister_ISR(PNIO_UINT32 vector)
		{

		}

        void Bsp_EVMA_enable_ISR(PNIO_UINT32 vector)
        {
            R_BSP_IrqDetectTypeSet(vector, 1); // Set the int to rising edge detection
            R_BSP_IrqCfgEnable(vector, DEFAULT_INT_PRIORITY, NULL);
        }

        void Bsp_EVMA_disable_ISR(PNIO_UINT32 vector)
        {
            R_BSP_IrqDisable(vector);
        }
#endif


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
