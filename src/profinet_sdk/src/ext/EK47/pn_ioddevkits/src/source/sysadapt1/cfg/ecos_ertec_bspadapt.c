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

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_ERTEC)
    #include "cm_inc.h"
    #include "compiler_stdlibs.h"
    #include "os.h"
	#include "bspadapt.h"
	#include <cyg/kernel/kapi.h>
	#include "nv_data.h"
	#include "nv_data.h"
    #include "ertec_inc.h"
    #if (PNIOD_PLATFORM & (PNIOD_PLATFORM_ECOS_EB200P | PNIOD_PLATFORM_POSIX_EB200P))
		#include "ertec_irq.h"
	#endif

	#include "ertec_x00.h"
    #include <cyg/hal/hal_io.h>
    #include "os_taskprio.h"


    #define LTRC_ACT_MODUL_ID   110
    #define	IOD_MODULE_ID       110

    #define SET_THREAD_PRIO_TIMER_THRESHOLD         4


	#define BSP_NUM_ISR_VECTORS	(SY_INT_COMMTX926 + 1)  	// see ertec_irq.h  definitions SY_INT_N_xxxxx

    PNIO_UINT32 toggleTimer = 0;


#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200_400)
    extern PNIO_UINT32   TskId_CycleIO;
#endif



    /**
     *  @brief Get general purpose port direction configuration
     *
     *  @param[in]   port	 	port to get direction setup from
     *  @return      			ERTECX00_GPIO_IN for input or ERTEC200_GPIO_OUT for output
     *
     * This routine gets the general purpose 'port' direction configuration and
     * returns it.
     * The return value could be either ERTECX00_GPIO_IN for input or
     * ERTEC200_GPIO_OUT for output.
     *
     */
    ERTECX00_GPIO_DIRECTION ertecx00GpioDirectionGet (PNIO_UINT32 port)    /* port to get direction setup from */
    {
	#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200_400)
        volatile PNIO_UINT32 gpioIoctrlVal = 0L;
        PNIO_UINT32 gpioPortPin;

        /* set port bit number */
        /* read gpio configuration register */
        if (port > 31)
        {
        #if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200)
            gpioPortPin = 1 << (port - 32);
            gpioIoctrlVal = READ_REG_ULONG (GPIO2_IOCTRL_REG);
        #else
            return(false); // port > 31 not supported
        #endif
        }
        else
  		{
    		gpioPortPin = 1 << port;
    		gpioIoctrlVal = READ_REG_ULONG (ERTEC_GPIO_IOCTRL_REG);
   		}
        /* return port IO configuration value */
        return ((0 == (gpioIoctrlVal & gpioPortPin)) ?
               ERTECX00_GPIO_OUT : ERTECX00_GPIO_IN);
        #elif (PNIOD_PLATFORM & (PNIOD_PLATFORM_ECOS_EB200P | PNIOD_PLATFORM_POSIX_EB200P))
              PNIO_printf ("##error: function not implemented\n");
              return(false); // port > 31 not supported
		#endif
    }



    /**
     *  @brief Configure general purpose port direction
     *
     *  @param[in]   port 		port to set direction for
     *  @param[in]   direction 	ERTECX00_GPIO_IN or ERTECX00_GPIO_OUT
     *  @return      			OK, always
     *
     *  This routine configures the general purpose 'port' direction to 'direction'
     *
     */
    PNIO_BOOL ertecx00GpioDirectionSet (PNIO_UINT32             port,       /* port to set direction for */
                                        ERTECX00_GPIO_DIRECTION direction)  /* ERTECX00_GPIO_IN or ERTECX00_GPIO_OUT */
    {
	#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200_400)
        volatile PNIO_UINT32 gpioIoctrlVal = 0L;
        PNIO_UINT32 gpioPortPin;

        /* read gpio configuration register */
    	if (port > 31)
    	{
        #if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200)
    		gpioIoctrlVal = READ_REG_ULONG (GPIO2_IOCTRL_REG);
    		gpioPortPin = 1 << (port - 32);
        #else
    		return(false); // port > 31 not supported
        #endif
        }
    	else
    	{
    		gpioIoctrlVal = READ_REG_ULONG (ERTEC_GPIO_IOCTRL_REG);
    		gpioPortPin = 1 << port;
    	}

        /* set gpio port direction (input or output) */
        if (ERTECX00_GPIO_OUT == direction)
        {
            ERTECX00_BIT_CLR (gpioIoctrlVal, gpioPortPin);
        }
        else if (ERTECX00_GPIO_IN == direction)
        {
            ERTECX00_BIT_SET (gpioIoctrlVal, gpioPortPin);
        }

        /* write back changed configuration register */
    	if (port > 31)
    	{
        #if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200)
    	    WRITE_REG_ULONG (GPIO2_IOCTRL_REG, gpioIoctrlVal);
        #else
    	    return(false); // port > 31 not supported
        #endif
    	}
    	else
    	{
    		WRITE_REG_ULONG (ERTEC_GPIO_IOCTRL_REG, gpioIoctrlVal);
    	}
        return(true);
    #elif (PNIOD_PLATFORM & (PNIOD_PLATFORM_ECOS_EB200P | PNIOD_PLATFORM_POSIX_EB200P))
        if (port > 31)
        {
            return (PNIO_NOT_OK);
        }

        // *  Setting to Output:
        if (ERTECX00_GPIO_IN == direction)
        {
            *((volatile LSA_UINT32*) U_GPIO__GPIO_IOCTRL_0) |=   (1UL << port); // set to input
        }
        else
        {
            *((volatile LSA_UINT32*) U_GPIO__GPIO_IOCTRL_0) &=  ~(1UL << port); // set to output
        }
        return (PNIO_OK);
	#endif
    }



    /**
     *  @brief get function of general purpose port
     *
     *  @param[in]   port	 	Port to get configured function for
     *  @return      			ERTECX00_GPIO_FUNCTION number,
     * 							ERTECX00_GPIO_FUNC_0 if port is out-of-range or GPIO function unknown.
     *
     *  This routine gets the general purpose 'port' function and returns it.
     *
	 * The return value is of type ERTECX00_GPIO_FUNCTION and can have one of the
	 * following values:
	 *
	 * \ts
	 * value                | | meaning
	 * ---------------------|-|-----------------
	 * ERTECX00_GPIO_FUNC_0 | | GPIO function 0
	 * ERTECX00_GPIO_FUNC_1 | | GPIO function 1
	 * ERTECX00_GPIO_FUNC_2 | | GPIO function 2
	 * ERTECX00_GPIO_FUNC_3 | | GPIO function 3
     *
     */
    ERTECX00_GPIO_FUNCTION ertecx00GpioFunctionGet (PNIO_UINT32 port /* port to get configured function for */ )
    {
	#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200_400)
        PNIO_UINT32 gpioFunction = 0xFFFFFFFF;
        PNIO_UINT32 gpioModeVal  = 0xFFFFFFFF;

        if (16 > port)
        {
            gpioModeVal = READ_REG_ULONG (ERTEC_GPIO_PORT_MODE_L_REG);
            port = port * 2;
        }
        else if (32 > port)
        {
            gpioModeVal = READ_REG_ULONG (ERTEC_GPIO_PORT_MODE_H_REG);
            port = (port - 16) * 2;
        }
        else
        {
           //PNIO_printf ("ERROR: port out-of-range [%2d]\n", port);
            return(ERTECX00_GPIO_FUNC_0);
        }
        gpioFunction = (gpioModeVal >> port) & 3;
        return(gpioFunction);
    #elif (PNIOD_PLATFORM & (PNIOD_PLATFORM_ECOS_EB200P | PNIOD_PLATFORM_POSIX_EB200P))
        PNIO_printf ("##error: function not implemented\n");
        return(false); // port > 31 not supported
	#endif
    }



    /**
     *  @brief Set general purpose port function
     *
     *  @param[in]   port 		Port to set function for
     *  @param[in]   function 	ERTECX00_GPIO_FUNC_N (N=0,1,2,3)
     *  @return      			OK, or ERROR if port is out-of-range
     *
     *  This routine sets the general purpose 'port' function to 'function'
     *
     * 'function' can have one of four different values of type
     * ERTECX00_GPIO_FUNCTION:
     *
     * value                | | meaning
     * ---------------------|-|-----------------
     * ERTECX00_GPIO_FUNC_0 | | GPIO function 0
     * ERTECX00_GPIO_FUNC_1 | | GPIO function 1
     * ERTECX00_GPIO_FUNC_2 | | GPIO function 2
     * ERTECX00_GPIO_FUNC_3 | | GPIO function 3
     *
     * The function definitions possible for the different ports are board specific
     * and are therefore defined in the board manual(s).
     */
    PNIO_BOOL ertecx00GpioFunctionSet ( PNIO_UINT32            port,       /* port to set function for */
    									ERTECX00_GPIO_FUNCTION function)   /* ERTECX00_GPIO_FUNC_N (N=0,1,2,3) */
  	{
	#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200_400)
        PNIO_UINT32 portBits = 0L;               /* bits associated to selected port */
        PNIO_UINT32 gpioFunctionMask = 0L;       /* mask selected port */
        PNIO_UINT32 gpioModeVal = 0xFFFFFFFF;    /* port mode register contents */

        /* read port mode register */
        if (16 > port)
        {
            gpioModeVal = READ_REG_ULONG (ERTEC_GPIO_PORT_MODE_L_REG);
            /* calculate bits associated to selected port */
            portBits = port * 2;
        }
        else if (32 > port)
        {
            gpioModeVal = READ_REG_ULONG (ERTEC_GPIO_PORT_MODE_H_REG);
            /* calculate bits associated to selected port */
            portBits = (port - 16) * 2;
        }
        else
        {
            return(false);
        }
        /* setup a mask to select only the requested port */
        gpioFunctionMask = 3 << portBits;
        /* clear port bits */
        ERTECX00_BIT_CLR (gpioModeVal, gpioFunctionMask);
        /* set requested port function */
        ERTECX00_BIT_SET (gpioModeVal, ((function & 3) << portBits));
        /* write back port mode register */
        if (16 > port)
        {
            WRITE_REG_ULONG (ERTEC_GPIO_PORT_MODE_L_REG, gpioModeVal);
        }
        else
        {
            WRITE_REG_ULONG (ERTEC_GPIO_PORT_MODE_H_REG, gpioModeVal);
        }
        return(true);
    #elif (PNIOD_PLATFORM & (PNIOD_PLATFORM_ECOS_EB200P | PNIOD_PLATFORM_POSIX_EB200P))
        PNIO_UINT32 RegAddr;
        PNIO_UINT32 GpioOfs;

        if (port < 16)
        {
            RegAddr = U_GPIO__GPIO_PORT_MODE_0_L;
            GpioOfs = port;
        }
        else if (port < 31)
        {
            RegAddr = U_GPIO__GPIO_PORT_MODE_0_H;
            GpioOfs = port - 16;
        }
        else
        {
            return (PNIO_NOT_OK);
        }

        if (function > 3)
        {
            return (PNIO_NOT_OK);
        }

        // ****alternate mode A:
        *((volatile LSA_UINT32*) RegAddr) = (*((volatile LSA_UINT32*) RegAddr) & ~(3UL << (2*GpioOfs))) | (function << (2*GpioOfs));

        return (PNIO_OK);
	#endif
    }



    /**
     *  @brief Read value from specific general purpose port
     *
     *  @param[in]   port 		Port to read input from
     *  @return      			Signal level on port (0 (FALSE, low) or 1 (TRUE, high))
     *
     *
     */
    PNIO_BOOL ertecx00GpioIn (PNIO_UINT32 port     /* port to read input from */ )
    {
	#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200_400)
        volatile PNIO_UINT32 gpioInVal = 0L;
        PNIO_UINT32 gpioPortPin;

        /* set port bit number */
        /* read gpio input register */
		#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200)
    	if (port > 31)
    	{
    		gpioPortPin = 1 << (port - 32);
    		gpioInVal = READ_REG_ULONG (GPIO2_IN_REG);
    	}
    	else
    	#endif
    	{
    		gpioPortPin = 1 << port;
    		gpioInVal = READ_REG_ULONG (ERTEC_GPIO_IN_REG);
    	}
        /* return port bit value */
        return((0 == (gpioInVal & gpioPortPin)) ? false : true);
    #elif (PNIOD_PLATFORM & (PNIOD_PLATFORM_ECOS_EB200P | PNIOD_PLATFORM_POSIX_EB200P))
        volatile PNIO_UINT32 gpioInVal = 0L;
        PNIO_UINT32 gpioPortPin;

        /* set port bit number */
        /* read gpio input register */

        gpioPortPin = 1 << port;
        gpioInVal = REG32(U_GPIO__GPIO_IN_0);

        /* return port bit value */
        return((0 == (gpioInVal & gpioPortPin)) ? false : true);
	#endif
    }



    /**
     *  @brief Write value to specific general purpose port
     *
     *  @param[in]   port 		Port to write output value to
     *  @param[out]  output 	Output value (0 or 1)
     *  @return      			GPIO output register value
     *
     *  This routine writes the 'output' value to specified general purpose output
     *  'port' and returns the new gpio output register value
     *
     */
    PNIO_UINT32 ertecx00GpioOut ( PNIO_UINT32 port,     /* port to write output value to */
    							  PNIO_BOOL   output )  /* output value (0 or 1) */
    {
	#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200_400)
        volatile PNIO_UINT32 gpioOutVal = 0L;
        PNIO_UINT32 gpioPortPin;
        /* read actual gpio output register */
		#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200) //clzs
    	if (port > 31)
  		{
    		gpioOutVal = READ_REG_ULONG (GPIO2_OUT_REG);
    		gpioPortPin =  1 << (port - 32);
   		}
    	else
		#endif
   		{
    		gpioOutVal = READ_REG_ULONG (ERTEC_GPIO_OUT_REG);
    		gpioPortPin = 1 << port;
   		}

        /* set gpio port output value (0 = FALSE, 1 = TRUE) */
        if (false == output)
        {
            ERTECX00_BIT_CLR (gpioOutVal, gpioPortPin);
        }
        else if (true == output)
        {
            ERTECX00_BIT_SET (gpioOutVal, gpioPortPin);
        }

        /* write back gpio output register */
		#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200)
    	if (port > 31)
   		{
    		WRITE_REG_ULONG (GPIO2_OUT_REG, gpioOutVal);
   		}
    	else
		#endif
   		{
    		WRITE_REG_ULONG (ERTEC_GPIO_OUT_REG, gpioOutVal);
   		}

        /* returns new gpio output register value */
        return(gpioOutVal);

    #elif (PNIOD_PLATFORM & (PNIOD_PLATFORM_ECOS_EB200P | PNIOD_PLATFORM_POSIX_EB200P))

        if (output)
        {
            // PNIO_printf (" ON\n");
            REG32(U_GPIO__GPIO_OUT_SET_0) = (1 << port);
        }
        else
        {
            // PNIO_printf (" OFF\n");
            REG32(U_GPIO__GPIO_OUT_CLEAR_0) = (1 << port);
        }
        return(0);  // read back not supported for E200P

    #endif
    }



    /**
     *  @brief Initialize the ERTECX00 GPIO
     *
     *  @return      			Void
     *
     *  Note: This routine does nothing. The input/output setup of the specific lines
     *  has to be done by the device driver using that GPIO
     *
     */
    void ertecX00GpioInit(void)
    {
        return;
    }



    /**
     *  @brief Configure general purpose port polarity
     *
     *  @param[in]   port 		Port to set polarity for
     *  @param[in]   polarity 	ERTEC200_GPIO_NORMAL_POLARITY or ERTEC200_GPIO_INVERTED_POLARITY
     *  @return      			OK, ERROR if invalid port
     *
     */
    PNIO_BOOL ertec200GpioPolaritySet ( PNIO_UINT32            port,      /* port to set polarity for */
    									ERTEC200_GPIO_POLARITY polarity)  /* ERTEC200_GPIO_NORMAL_POLARITY or ERTEC200_GPIO_INVERTED_POLARITY */
    {
	#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200)
        volatile PNIO_UINT32 gpioIoctrlVal = 0L;
        PNIO_UINT32 gpioPortPin;

    	switch (port)
    	{
    		case 0:
    			gpioPortPin = 1;
    			break;
    		case 1:
    			gpioPortPin = 2;
    			break;
    		case 30:
    			gpioPortPin = 4;
    			break;
    		case 31:
    			gpioPortPin = 8;
    			break;
    		default:
    			return(false);
    	}
        /* read gpio configuration register */
        gpioIoctrlVal = READ_REG_ULONG (GPIO_POLARITY_SELECT);
        /* set gpio port polarity (normal or inverted) */
        if (ERTEC200_GPIO_NORMAL_POLARITY == polarity)
        {
            ERTECX00_BIT_CLR (gpioIoctrlVal, gpioPortPin);
        }
        else if (ERTEC200_GPIO_INVERTED_POLARITY == polarity)
        {
            ERTECX00_BIT_SET (gpioIoctrlVal, gpioPortPin);
        }
    	else
    	{
    		return(false);
    	}
        /* write back changed configuration register */
        WRITE_REG_ULONG (GPIO_POLARITY_SELECT, gpioIoctrlVal);
        return(true);
	#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB400)
        return(false); // not supported
    #elif (PNIOD_PLATFORM & (PNIOD_PLATFORM_ECOS_EB200P | PNIOD_PLATFORM_POSIX_EB200P))
        return(false); // not supported
	#endif
    }



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
    #if (PNIOD_PLATFORM & (PNIOD_PLATFORM_ECOS_EB200P | PNIOD_PLATFORM_POSIX_EB200P))
		PNIO_UINT32 RegAddr;
		PNIO_UINT32 GpioOfs;

		if (Gpio < 16)
		{
			RegAddr = U_GPIO__GPIO_PORT_MODE_0_L;
			GpioOfs = Gpio;
		}
		else if (Gpio < 31)
		{
			RegAddr = U_GPIO__GPIO_PORT_MODE_0_H;
			GpioOfs = Gpio - 16;
		}
		else
		{
			return (PNIO_NOT_OK);
		}

		if (Function > 3)
		{
			return (PNIO_NOT_OK);
		}

		// ****alternate mode A:
		*((volatile LSA_UINT32*) RegAddr) = (*((volatile LSA_UINT32*) RegAddr) & ~(3UL << (2*GpioOfs))) | (Function << (2*GpioOfs));

		// *  put on exit:
		if (InOut == GPIO_DIR_IN)
		{
			*((volatile LSA_UINT32*) U_GPIO__GPIO_IOCTRL_0) |=   (1UL << Gpio); // set to input
		}
		else
		{
			*((volatile LSA_UINT32*) U_GPIO__GPIO_IOCTRL_0) &=  ~(1UL << Gpio); // set to output
		}
		return (PNIO_OK);

	#else
		// not supported yet
		return (PNIO_NOT_OK);
	#endif
    }

    #if (PNIOD_PLATFORM & (PNIOD_PLATFORM_ECOS_EB200P | PNIOD_PLATFORM_POSIX_EB200P))



    /**
     *  @brief Read GPIOin 0 to 31
     *
     *  @return      			32 bit Input Values
     *
     *  Note: To set the specified GPIOs to input mode, is in the
     *  responsibility of the user. It is not checked in this function
     *
     */
    PNIO_UINT32 Bsp_ReadGPIOin_0_to_31  (PNIO_VOID)
    {
        // ****alternate mode A:
         return (*((volatile LSA_UINT32*) U_GPIO__GPIO_IN_0));
    }



    /**
     *  @brief Sets all GPIO's 0...31, where the Mask bit is set
     *
     *  @param[in]   OutMsk 	Mask bits for GPIO 0..31.
     *  @return      			Void
     *
     *  All GPIO's, where the mask bit is not set, keep unchanged
     *
     */
    PNIO_VOID Bsp_SetGPIOout_0_to_31  (PNIO_UINT32 OutMsk)
    {
         REG32(U_GPIO__GPIO_OUT_SET_0) = OutMsk;
    }



    /**
     *  @brief Clears all GPIO's 0...31, where the Mask bit is set
     *
     *  @param[in]   OutMsk 	Mask bits for GPIO 0..31.
     *  @return      			Void
     *
     *  All GPIO's, where the mask bit is not set, keep unchanged
     *
     */
    PNIO_VOID Bsp_ClearGPIOout_0_to_31  (PNIO_UINT32 OutMsk)
    {
         REG32(U_GPIO__GPIO_OUT_CLEAR_0) = OutMsk;
    }

#endif



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

        //FLASH_INIT;

        return (PNIO_OK);       // nothing to do here
    }



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
	    	pDevMacAddr[0] = 0x08;
	    	pDevMacAddr[1] = 0x00;
	    	pDevMacAddr[2] = 0x06;
	    	pDevMacAddr[3] = 0x02;
	    	pDevMacAddr[4] = 0x01;

		#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB400)
	    	pDevMacAddr[5] = 0x00;
		#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200)
	    	pDevMacAddr[5] = 0x10;
        #elif (PNIOD_PLATFORM & (PNIOD_PLATFORM_ECOS_EB200P | PNIOD_PLATFORM_POSIX_EB200P))
            pDevMacAddr[5] = 0x10;
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


    #if (PNIOD_PLATFORM & (PNIOD_PLATFORM_ECOS_EB200P | PNIOD_PLATFORM_POSIX_EB200P))
    /* ------------------------------------------------------------------- */
    /* interrupt handling for the EVMA (event manager) module (application */
    /* timer management)                                                   */
    /* ------------------------------------------------------------------- */

    #define   USER_CONTEXT_ISR       0   // 1: call user callback in ISR, else in DSR

    OS_BSS_FAST  static cyg_handle_t     bsp_evma_isr_handles [BSP_NUM_ISR_VECTORS];
    OS_BSS_FAST  static cyg_interrupt    bsp_evma_isr_memory  [BSP_NUM_ISR_VECTORS];
    OS_DATA_FAST static volatile int     bsp_evma_int_context = 0;



    /**
     *  @brief Interrupt isr routine
     *
     *  @param[in]   vector 	Interrupt vector
     *  @param[in]   data	 	Pointer or word of interrupt data
     *  @return      			CYG_ISR_CALL_DSR | CYG_ISR_HANDLED
     *
     *  Data should carry pointer to user interrupt functionality
     *  The function from data will be called in dsr
     *
     */
    OS_CODE_FAST static cyg_uint32 Bsp_EVMA_ISR(cyg_vector_t vector, cyg_addrword_t data)
	{
		#if (USER_CONTEXT_ISR)
			void (*evma_isr)(void) = (void (*)(void))(data);
			cyg_interrupt_disable();
			evma_isr ();
			cyg_interrupt_enable();
			cyg_interrupt_acknowledge(vector);
			return (CYG_ISR_HANDLED);
		#else
			cyg_interrupt_disable();
			bsp_evma_int_context++;
			cyg_interrupt_enable();
			cyg_interrupt_acknowledge(vector);
			return ((cyg_uint32)CYG_ISR_HANDLED | (cyg_uint32)CYG_ISR_CALL_DSR);
		#endif
	}



    #if (USER_CONTEXT_ISR == 0)
    /**
     *  @brief Interrupt dsr routine
     *
     *  @param[in]   vector 	Interrupt vector
     *  @param[in]   count	 	Pending interrupts counter
     *  @param[in]   data	 	Pointer or word of interrupt data
     *  @return      			Void
     *
     *  Data should carry pointer to user interrupt functionality
     *  The function from data will be called here
     *
     */
        OS_CODE_FAST static void Bsp_EVMA_DSR (cyg_vector_t vector, cyg_ucount32 count, cyg_addrword_t data)
        {
            /* note: count should be always 1 */
            void (*evma_dsr)(void) = (void (*)(void))(data);

            evma_dsr();
            cyg_interrupt_disable();
            bsp_evma_int_context-= count;
            if (bsp_evma_int_context < 0)
                bsp_evma_int_context = 0;
            cyg_interrupt_enable();
        }
    #endif



        /**
         *  @brief Register interrupt isr routine to interrupt handler
         *
         *  @param[in]   vector 	Interrupt vector
         *  @param[in]   *isr	 	Pointer to the isr
         *  @return      			Void
         *
         *
         */
    void Bsp_EVMA_register_ISR(PNIO_UINT32 vector, void (*isr)(void))
    {
    	if (isr)
    	{
			PNIO_INT32 handle_index = vector;  // - EVMA_FIRST_ISR_VECTOR_INDEX;

			if(handle_index >= 0 && handle_index < BSP_NUM_ISR_VECTORS)
			{
			#if (USER_CONTEXT_ISR)
			  cyg_interrupt_create(vector, 0, (cyg_addrword_t) isr, Bsp_EVMA_ISR, NULL, &bsp_evma_isr_handles[handle_index], &bsp_evma_isr_memory[handle_index]);
			#else
			  cyg_interrupt_create(vector, 0, (cyg_addrword_t) isr, Bsp_EVMA_ISR, Bsp_EVMA_DSR, &bsp_evma_isr_handles[handle_index], &bsp_evma_isr_memory[handle_index]);
			#endif
			cyg_interrupt_attach(bsp_evma_isr_handles[handle_index]);
			cyg_interrupt_unmask(vector);
			}
    	}
    	else /* unregister ISR when called with NULL pointer */
    	{
    		Bsp_EVMA_unregister_ISR(vector);
    	}
    }



    /**
     *  @brief Remove isr, mask its interrupt
     *
     *  @param[in]   vector 	Interrupt vector
     *  @return      			Void
     *
     *  Basically remove interrupt
     *
     */
    void Bsp_EVMA_unregister_ISR(PNIO_UINT32 vector)
    {
      PNIO_INT32 handle_index = vector;  //  - EVMA_FIRST_ISR_VECTOR_INDEX;

      if(handle_index >= 0 && handle_index < BSP_NUM_ISR_VECTORS)
      {
        cyg_interrupt_mask(vector);
        cyg_interrupt_detach(bsp_evma_isr_handles[handle_index]);
        cyg_interrupt_delete(bsp_evma_isr_handles[handle_index]);
      }
    }
#endif


    volatile int ECOS_intContext = 0;



    /**
     *  @brief Interrupt isr routine
     *
     *  @param[in]   vector 	Interrupt vector
     *  @param[in]   data	 	Pointer or word of interrupt data
     *  @return      			CYG_ISR_CALL_DSR | CYG_ISR_HANDLED
     *
     *  Data should carry pointer to user interrupt functionality
     *  The function from data will be called in dsr
     *
     */
    OS_CODE_FAST static cyg_uint32 Bsp_ISR(cyg_vector_t vector, cyg_addrword_t data)
    {
    	cyg_interrupt_disable();
    	++ECOS_intContext;
    	cyg_interrupt_enable();
    	cyg_interrupt_acknowledge(vector);
    	return ((cyg_uint32)CYG_ISR_CALL_DSR | (cyg_uint32)CYG_ISR_HANDLED);
    }



    /**
     *  @brief Interrupt dsr routine
     *
     *  @param[in]   vector 	Interrupt vector
     *  @param[in]   count	 	Pending interrupts counter
     *  @param[in]   data	 	Pointer or word of interrupt data
     *  @return      			Void
     *
     *  Data should carry pointer to user interrupt functionality
     *  The function from data will be called here
     *
     */
    OS_CODE_FAST static void Bsp_DSR(cyg_vector_t vector, cyg_ucount32 count, cyg_addrword_t data)
    {
    	/* note: count should be always 1 */
    	void (*DSRCall)(void) = (void (*)(void))(data);
    	DSRCall();
    	cyg_interrupt_disable();
    	ECOS_intContext-= count;
    	cyg_interrupt_enable();
    	if (ECOS_intContext < 0)
    		ECOS_intContext = 0;
    }



    /**
     *  @brief Assigns interrupts to interrupt handler
     *
     *  @param[in]   pErtecSwiIntH 	High priority switch interrupts
     *  @param[in]   pErtecSwiIntL 	Low priority switch interrupts
     *  @return      				PNIO_OK
     *
     *
     */
    PNIO_UINT32 Bsp_ErtecSwiIntConnect (PNIO_CBF_ERTEC_SWI_INT_H pErtecSwiIntH,
										PNIO_CBF_ERTEC_SWI_INT_L pErtecSwiIntL)
	{
		static cyg_interrupt intH;
		static cyg_interrupt intL;
		static cyg_handle_t hIntH;
		static cyg_handle_t hIntL;
#if (PNIOD_PLATFORM & (PNIOD_PLATFORM_ECOS_EB200P | PNIOD_PLATFORM_POSIX_EB200P))
        cyg_interrupt_create(SY_INT_PN_IRQ2_2,0,(cyg_addrword_t)pErtecSwiIntH,Bsp_ISR,Bsp_DSR,&hIntH,&intH);
        cyg_interrupt_create(SY_INT_PN_IRQ2_0,0,(cyg_addrword_t)pErtecSwiIntL,Bsp_ISR,Bsp_DSR,&hIntL,&intL);
        cyg_interrupt_attach(hIntL);
        cyg_interrupt_attach(hIntH);
        cyg_interrupt_unmask(SY_INT_PN_IRQ2_2);
        cyg_interrupt_unmask(SY_INT_PN_IRQ2_0);
#else
		cyg_interrupt_create(INT_VEC_IRQ0_SP,0,(cyg_addrword_t)pErtecSwiIntH,Bsp_ISR,Bsp_DSR,&hIntH,&intH);
		cyg_interrupt_create(INT_VEC_IRQ1_SP,0,(cyg_addrword_t)pErtecSwiIntL,Bsp_ISR,Bsp_DSR,&hIntL,&intL);
		cyg_interrupt_attach(hIntL);
		cyg_interrupt_attach(hIntH);
		cyg_interrupt_unmask(INT_VEC_IRQ0_SP);
		cyg_interrupt_unmask(INT_VEC_IRQ1_SP);
#endif
		return (PNIO_OK);
	}



    /**
     *  @brief Assigns error interrupts to interrupt handlers
     *
     *  @param[in]   pErtecParityErrInt 	Parity error interrupt
     *  @param[in]   pErtecAccessErrInt 	Access error interrupt
     *  @return      						PNIO_OK
     *
     *
     */
    PNIO_UINT32 Bsp_ErtecErrIntConnect  (PNIO_CBF_ERTEC_PARITY_ERR_INT   pErtecParityErrInt,
    									 PNIO_CBF_ERTEC_ACCESS_ERR_INT   pErtecAccessErrInt)
	{
#if (PNIOD_PLATFORM & (PNIOD_PLATFORM_ECOS_EB200P | PNIOD_PLATFORM_POSIX_EB200P))
        static cyg_interrupt intParityErr;
        static cyg_interrupt intAhbAccessErr;
        static cyg_interrupt intApbAccessErr;
        static cyg_interrupt intEmcAccessErr;

        static cyg_handle_t hIntParityErr;
        static cyg_handle_t hIntAhbAccessErr;
        static cyg_handle_t hIntApbAccessErr;
        static cyg_handle_t hIntEmcAccessErr;

        cyg_interrupt_create(SY_INT_EDC_EVENT,         0, (cyg_addrword_t)pErtecParityErrInt, Bsp_ISR, Bsp_DSR, &hIntParityErr,    &intParityErr);
        cyg_interrupt_create(SY_INT_AHB_ADDRESS_ERROR, 0, (cyg_addrword_t)pErtecAccessErrInt, Bsp_ISR, Bsp_DSR, &hIntAhbAccessErr, &intAhbAccessErr);
        cyg_interrupt_create(SY_INT_APB_ADDRESS_ERROR, 0, (cyg_addrword_t)pErtecAccessErrInt, Bsp_ISR, Bsp_DSR, &hIntApbAccessErr, &intApbAccessErr);
        cyg_interrupt_create(SY_INT_EMC_ADDRESS_ERROR, 0, (cyg_addrword_t)pErtecAccessErrInt, Bsp_ISR, Bsp_DSR, &hIntEmcAccessErr, &intEmcAccessErr);

        cyg_interrupt_attach(hIntParityErr);
        cyg_interrupt_attach(hIntAhbAccessErr);
        cyg_interrupt_attach(hIntApbAccessErr);
        cyg_interrupt_attach(hIntEmcAccessErr);

        cyg_interrupt_unmask(SY_INT_EDC_EVENT);
        cyg_interrupt_unmask(SY_INT_AHB_ADDRESS_ERROR);
        cyg_interrupt_unmask(SY_INT_APB_ADDRESS_ERROR);
        cyg_interrupt_unmask(SY_INT_EMC_ADDRESS_ERROR);
#endif
		return (PNIO_OK);
	}



    /**
     *  @brief Assigns Timer1 interrupt to interrupt handlers
     *
     *  @param[in]   pErtecTim1Int 		Timer1 interrupt
     *  @return      					PNIO_OK
     *
     *
     */
    PNIO_UINT32 Bsp_ErtecTim1IntConnect (PNIO_CBF_ERTEC_TIM1_INT pErtecTim1Int)
    {
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200_400)
        static cyg_interrupt intTim1;
        static cyg_handle_t hintTim1;

        cyg_interrupt_create(INT_VEC_TIMER_1, 0, (cyg_addrword_t)pErtecTim1Int, Bsp_ISR, Bsp_DSR, &hintTim1, &intTim1);

        cyg_interrupt_attach(hintTim1);
        cyg_interrupt_unmask(INT_VEC_TIMER_1);
#endif
        return (PNIO_OK);
    }



    /**
     *  @brief Enable Timer1 interrupt, set time amount
     *
     *  @param[in]   ErtecTim1PeriodUs 	Time until interrupt [us]
     *  @return      					PNIO_OK
     *
     *
     */
    PNIO_UINT32 Bsp_ErtecTim1Start (PNIO_UINT32 ErtecTim1PeriodUs)
    {
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200_400)
        HAL_WRITE_UINT32(ERTEC_TIMER_1, 0);
        /* set up in periodic mode, disable timer */
        HAL_WRITE_UINT32(ERTEC_TIMER_CTRL_STAT_1, ERTEC_TIMER_DISABLE);
        /* Load Timer Prescale register */
        HAL_WRITE_UINT32 (ERTEC_TIMER_RELD_PREDIV, 0);
        /* stop any prescale */
        HAL_WRITE_UINT32 (ERTEC_TIMER_CTRL_PREDIV, 0);
        /* Load Timer Reload value into reload registers */
        HAL_WRITE_UINT32 (ERTEC_TIMER_RELD_1, ErtecTim1PeriodUs * 50);
        /* set up in periodic mode, load timer register with preload, enable timer */
        HAL_WRITE_UINT32(ERTEC_TIMER_CTRL_STAT_1, ERTEC_TIMER_ENABLE);
#endif
        return (PNIO_OK);
    }



#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200_400)
    /**
     *  @brief User reaction to Timer1 interrupt
     *
     *  @param[in]   int_source 		Source of interrupt
     *  @return      					Void
     *
     *
     */
    PNIO_VOID Bsp_Timer1Int(PNIO_UINT32 int_source)
    {
        if (toggleTimer == 0)
          {
              // SET_THREAD_PRIO_TIMER_VALUE expired --> set prio low
              OsSetThreadPrio (TskId_CycleIO, TASK_PRIO_APPL_CYCLE_IO_LOW);
              toggleTimer++;
          } else if (toggleTimer >= SET_THREAD_PRIO_TIMER_THRESHOLD)
          {
              //SET_THREAD_PRIO_TIMER_THRESHOLD * SET_THREAD_PRIO_TIMER_VALUE expired --> set prio high
              OsSetThreadPrio (TskId_CycleIO, TASK_PRIO_APPL_CYCLE_IO);
              toggleTimer = 0;
          } else
          {
              toggleTimer++;
          }
    }
#endif



	#if ((PNIOD_PLATFORM & (PNIOD_PLATFORM_ECOS_EB200P | PNIOD_PLATFORM_POSIX_EB200P)) && (IOD_INCLUDE_POF == 1))
    /**
     *  @brief Init GPIO for optical fiber communication
     *
     *  @return      					Void
     *
     *
     */
		void Bsp_PofInit (void)
		{
			Bsp_SetGpioMode (GPIO_POF1_SCL, 3, GPIO_DIR_OUT);
			Bsp_ClearGPIOout_0_to_31 (1<<GPIO_POF1_SCL);
			Bsp_SetGpioMode (GPIO_POF1_SDA, 3, GPIO_DIR_OUT);
			Bsp_ClearGPIOout_0_to_31 (1<<GPIO_POF1_SDA);

			Bsp_SetGpioMode (GPIO_POF2_SCL, 3, GPIO_DIR_OUT);
			Bsp_ClearGPIOout_0_to_31 (1<<GPIO_POF2_SCL);
			Bsp_SetGpioMode (GPIO_POF2_SDA, 3, GPIO_DIR_OUT);
			Bsp_ClearGPIOout_0_to_31 (1<<GPIO_POF2_SDA);
		}



	#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200)
	    /**
	     *  @brief Init GPIO for optical fiber communication
	     *
	     *  @return      					Void
	     *
	     *
	     */
		void Bsp_PofInit (void)
		{
			ertecx00GpioDirectionSet(GPIO_POF1_SCL,   ERTECX00_GPIO_OUT);
			ertecx00GpioFunctionSet (GPIO_POF1_SCL,   ERTECX00_GPIO_FUNC_0);
			ertecx00GpioOut			(GPIO_POF1_SCL,   0);
			ertecx00GpioDirectionSet(GPIO_POF1_SDA,   ERTECX00_GPIO_OUT);
			ertecx00GpioFunctionSet (GPIO_POF1_SDA,   ERTECX00_GPIO_FUNC_0);
			ertecx00GpioOut			(GPIO_POF1_SDA,   0);
			ertecx00GpioDirectionSet(GPIO_POF2_SCL,   ERTECX00_GPIO_OUT);
			ertecx00GpioFunctionSet (GPIO_POF2_SCL,   ERTECX00_GPIO_FUNC_0);
			ertecx00GpioOut			(GPIO_POF2_SCL,   0);
			ertecx00GpioDirectionSet(GPIO_POF2_SDA,   ERTECX00_GPIO_OUT);
			ertecx00GpioFunctionSet (GPIO_POF2_SDA,   ERTECX00_GPIO_FUNC_0);
			ertecx00GpioOut			(GPIO_POF2_SDA,   0);
		}



	    /**
	     *  @brief Set POF port SCL direction
	     *
	     *  @param[in]   portId 			POF porn number [1 or 2]
	     *  @param[in]   setInput 			Input or output
	     *
	     *  @return      					Void
	     *
	     *
	     */
		void Bsp_PofSclDir (PNIO_UINT8 portId, PNIO_UINT8 setInput)
		{
			if ((portId >=1 ) && (portId <= 2))
			{
				// setIOnput=1  => High -> GPIO Tristate, set to Input
				// setIOnput=0  => Low  -> no Tristate, set to Output
				ertecx00GpioDirectionSet ( (portId == 1) ? GPIO_POF1_SCL : GPIO_POF2_SCL,
										   (setInput)    ? ERTECX00_GPIO_IN : ERTECX00_GPIO_OUT);
				ertecx00GpioOut( (portId == 1) ? GPIO_POF1_SCL : GPIO_POF2_SCL, 0);
			}
		}



		/**
		 *  @brief Set POF port SDA direction
		 *
		 *  @param[in]   portId 			POF porn number [1 or 2]
		 *  @param[in]   setInput 			Input or output
		 *
		 *  @return      					Void
		 *
		 *
		 */
		void Bsp_PofSdaDir (PNIO_UINT8 portId, PNIO_UINT8 setInput)
		{
			if ((portId >=1 ) && (portId <= 2))
			{
				// setIOnput=1  => High -> GPIO Tristate, set to Input
				// setIOnput=0  => Low  -> no Tristate, set to Output
				ertecx00GpioDirectionSet ( (portId == 1) ? GPIO_POF1_SDA : GPIO_POF2_SDA,
										   (setInput)    ? ERTECX00_GPIO_IN : ERTECX00_GPIO_OUT);
				ertecx00GpioOut( (portId == 1) ? GPIO_POF1_SDA : GPIO_POF2_SDA, 0);
			}
		}



		/**
		 *  @brief Read status of POF port SDA line
		 *
		 *  @param[in]   portId 			POF porn number [1 or 2]
		 *
		 *  @return      					Status of GPIO pin
		 *
		 *
		 */
		PNIO_UINT8 Bsp_PofSdaRead (PNIO_UINT8 portId)
		{
			if ((portId >=1 ) && (portId <= 2))
			{
				return ertecx00GpioIn( (portId == 1) ? GPIO_POF1_SDA : GPIO_POF2_SDA);
			}
			return 0;
		}

#endif

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

			REG32(U_WDOG__RELD1_LOW) =  load_low;   // The watchdog is raised to 251 ms
			REG32(U_WDOG__RELD1_HIGH) = load_high;
			REG32(U_WDOG__WD_CTRL_STATUS) = 0x98760004;	//password + reload

			REG32(U_SCRB__ASYN_RES_CTRL_REG) |= 0x01;  // unlock in SCRB
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
			REG32(U_WDOG__WD_CTRL_STATUS) = 0x98760000;  // password
			REG32(U_SCRB__ASYN_RES_CTRL_REG) &= ( ~( 0x01 ) );  // turn off wd reset
			REG32(U_WDOG__RELD1_LOW) =  0;
			REG32(U_WDOG__RELD1_HIGH) = 0;
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
			REG32(U_WDOG__WD_CTRL_STATUS) = 0x98760006;  // password + trigger + WD1RUN
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
			REG32(U_WDOG__WD_CTRL_STATUS) = 0x98760000;  // password
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
			REG32(U_WDOG__WD_CTRL_STATUS) |= 0x98760004;  // password + trigger + WD1RUN
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

		    static cyg_interrupt intH;
		    static cyg_handle_t hIntH;
            //lint -e{611} Suspicious cast
		    cyg_interrupt_create(
		            int_vector,        /*vector*/
		            0,                      /*priority*/
		            0,                      /*data ptr*/
		            ( cyg_ISR_t * )gpio_isr, /* *isr*/
		            ( cyg_DSR_t * )gpio_dsr, /* *dsr*/
		            &hIntH,                 /* *return handle*/
		            &intH);
		    /* *interrupt*/
		    cyg_interrupt_attach( hIntH );
		    cyg_interrupt_unmask( int_vector );
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

            static cyg_interrupt intH;
            static cyg_handle_t hIntH;
            //lint -e{611} Suspicious cast
            cyg_interrupt_create(
                    int_vector,        /*vector*/
                    0,                      /*priority*/
                    0,                      /*data ptr*/
                    ( cyg_ISR_t * )gpio_isr, /* *isr*/
                    ( cyg_DSR_t * )gpio_dsr, /* *dsr*/
                    &hIntH,                 /* *return handle*/
                    &intH);
            /* *interrupt*/
            cyg_interrupt_attach( hIntH );
            cyg_interrupt_unmask( int_vector );
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
		    cyg_interrupt_acknowledge(int_vector);
		}   /* PnUsr_xhif_acyc_isr */


#endif


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
