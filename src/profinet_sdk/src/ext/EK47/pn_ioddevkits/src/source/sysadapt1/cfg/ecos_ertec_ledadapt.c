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
/*  F i l e               &F: ecos_ertec_ledadapt.c                     :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Interface for LED control (if available) for PNIO stack and application  */
/*                                                                           */
/*  Note: Mandatory is to signal the BLINK event, but it is                  */
/*        up to the customer, which LED or LEDs are used for that function   */
/*                                                                           */
/*        All other LED signaling is optional, this example is only          */
/*        a recommendation.                                                  */
/*                                                                           */
/*        included signaling:                                                */
/*        RUN LED (green):    on:  IO data exchange running                  */
/*                            off: no IO data exchange                       */
/*                                                                           */
/*        ERROR LED (red):    error occured or diagnosis entry available     */
/*        MAINT LED (yellow): maintenance required or demanded               */
/*                                                                           */
/*        blink:       DCP blink request (from engineering)                  */
/*                     all LEDs (error LED, ) are blinking                   */
/*                                                                           */
/*****************************************************************************/

#include "compiler.h"

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_ERTEC)

    #include "glob_sys.h"
    #include "trc_if.h"
    #include "bspadapt.h"
    #include "bspledadapt.h"
    #include "ertec_inc.h"
    #include "ertec_x00.h"

	#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB400)
    #include "lsas_cfg.h"
    #include "edd_cfg.h"
    #include "eddi_cfg.h"
    #include "edd_usr.h"
    #include "eddi_usr.h"
    #include "eddi_sys.h"
    #include "lsas_sys.h"
	#endif

    // *------------ local data for LED blink  ----------*
    static  PNIO_UINT16     IdentTimerID  = 0;
    static  PNIO_BOOL       led_state 	  = 0;
    static  PNIO_UINT8      LedState[PNIO_LED_NUMOF_ENTRIES];


    typedef enum
    { // LED
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200)
        GPIO_LED_ERROR   = 2,  // red
        GPIO_LED_RUN     = 3,  // green
        GPIO_LED_LINKP1  = 4,  // green port1
        GPIO_LED_LINKP2  = 5,  // green port2
        GPIO_LED_ACTIVP1 = 6,  // yellow port 1
        GPIO_LED_ACTIVP2 = 7,  // yellow port 2
        GPIO_LED_FO1     = 11, // fo maintenance port1
        GPIO_LED_FO2     = 12, // fo maintenance port2
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB400)
        GPIO_LED_ERROR   = 6,  // red
        GPIO_LED_RUN     = 7,  // green
        GPIO_LED_LINKP1  = 3,  // green port0
        GPIO_LED_LINKP2  = 5,  // green port1
        GPIO_LED_LINKP3  = 27,  // green port2
        GPIO_LED_LINKP4  = 29,  // green port3
        GPIO_LED_ACTIVP1 = 2,  // yellow port0
        GPIO_LED_ACTIVP2 = 4,  // yellow port1
        GPIO_LED_ACTIVP3 = 26,  // yellow port2
        GPIO_LED_ACTIVP4 = 28,  // yellow port3
#elif (PNIOD_PLATFORM & (PNIOD_PLATFORM_ECOS_EB200P | PNIOD_PLATFORM_POSIX_EB200P))
        // User GPIO 0..7 = ERTEC GPIO(0)....GPIO(7)
        GPIO_LED_USER_00 = 00,
        GPIO_LED_USER_01 = 01,
        GPIO_LED_USER_02 = 02,
        GPIO_LED_USER_03 = 03,
        GPIO_LED_USER_04 = 04,
        GPIO_LED_USER_05 = 05,
        GPIO_LED_USER_06 = 06,
        GPIO_LED_USER_07 = 07,
        GPIO_LED_USER_08 = 16,
        GPIO_LED_USER_09 = 17,
        GPIO_LED_USER_10 = 18,
        GPIO_LED_USER_11 = 19,
        GPIO_LED_USER_12 = 20,
        GPIO_LED_USER_13 = 21,
        GPIO_LED_USER_14 = 22,
        GPIO_LED_USER_15 = 23,
        GPIO_LED_ERROR   = 25, // red
        GPIO_LED_MAINT   = 26, // yellow
        GPIO_LED_DIAG    = 27, // yellow
        GPIO_LED_PENERGY = 28, // green
        GPIO_LED_SYNC    = 29, // green
        GPIO_LED_FO1     = 30, // fo maintenance port1
        GPIO_LED_FO2     = 31, // fo maintenance port2
#endif
        GPIO_LED_NUMOF_ENTRIES
    } GPIO_LEDTYPE;


    static PNIO_VOID BspLed_IdentTimerCbf(void)
    {
        led_state ^= 1;
	#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200_400)
        Bsp_EbSetLed(PNIO_LED_RUN, led_state);
    #elif (PNIOD_PLATFORM & (PNIOD_PLATFORM_ECOS_EB200P | PNIOD_PLATFORM_POSIX_EB200P))
        Bsp_EbSetLed(PNIO_LED_MAINT, led_state);
	#endif
        Bsp_EbSetLed(PNIO_LED_ERROR, led_state);
    }

    PNIO_VOID  BspLed_Open (PNIO_VOID)
    {
        OsMemSet (LedState, 0, sizeof (LedState));
        BspLed_InitLedBlink ();

        // config LED gpio
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200_400)
        ertecx00GpioDirectionSet(GPIO_LED_RUN,     ERTECX00_GPIO_OUT);
        ertecx00GpioFunctionSet (GPIO_LED_RUN,     ERTECX00_GPIO_FUNC_0);
        ertecx00GpioDirectionSet(GPIO_LED_ERROR,   ERTECX00_GPIO_OUT);
        ertecx00GpioFunctionSet (GPIO_LED_ERROR,   ERTECX00_GPIO_FUNC_0);
        // enable both LEDs
        Bsp_EbSetLed(PNIO_LED_RUN,   1);
        Bsp_EbSetLed(PNIO_LED_ERROR, 1);
#endif

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200)
        ertecx00GpioDirectionSet(GPIO_LED_LINKP1,  ERTECX00_GPIO_OUT);
        ertecx00GpioFunctionSet (GPIO_LED_LINKP1,  ERTECX00_GPIO_FUNC_1);
        ertecx00GpioDirectionSet(GPIO_LED_LINKP2,  ERTECX00_GPIO_OUT);
        ertecx00GpioFunctionSet (GPIO_LED_LINKP2,  ERTECX00_GPIO_FUNC_1);
        ertecx00GpioDirectionSet(GPIO_LED_ACTIVP1, ERTECX00_GPIO_OUT);
        ertecx00GpioFunctionSet (GPIO_LED_ACTIVP1, ERTECX00_GPIO_FUNC_3);
        ertecx00GpioDirectionSet(GPIO_LED_ACTIVP2, ERTECX00_GPIO_OUT);
        ertecx00GpioFunctionSet (GPIO_LED_ACTIVP2, ERTECX00_GPIO_FUNC_3);
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB400)
        ertecx00GpioDirectionSet(GPIO_LED_LINKP1,  ERTECX00_GPIO_OUT);
        ertecx00GpioFunctionSet (GPIO_LED_LINKP1,  ERTECX00_GPIO_FUNC_0);
        ertecx00GpioDirectionSet(GPIO_LED_LINKP2,  ERTECX00_GPIO_OUT);
        ertecx00GpioFunctionSet (GPIO_LED_LINKP2,  ERTECX00_GPIO_FUNC_0);
        ertecx00GpioDirectionSet(GPIO_LED_LINKP3,  ERTECX00_GPIO_OUT);
        ertecx00GpioFunctionSet (GPIO_LED_LINKP3,  ERTECX00_GPIO_FUNC_0);
        ertecx00GpioDirectionSet(GPIO_LED_LINKP4,  ERTECX00_GPIO_OUT);
        ertecx00GpioFunctionSet (GPIO_LED_LINKP4,  ERTECX00_GPIO_FUNC_0);
        ertecx00GpioDirectionSet(GPIO_LED_ACTIVP1, ERTECX00_GPIO_OUT);
        ertecx00GpioFunctionSet (GPIO_LED_ACTIVP1, ERTECX00_GPIO_FUNC_0);
        ertecx00GpioDirectionSet(GPIO_LED_ACTIVP2, ERTECX00_GPIO_OUT);
        ertecx00GpioFunctionSet (GPIO_LED_ACTIVP2, ERTECX00_GPIO_FUNC_0);
        ertecx00GpioDirectionSet(GPIO_LED_ACTIVP3, ERTECX00_GPIO_OUT);
        ertecx00GpioFunctionSet (GPIO_LED_ACTIVP3, ERTECX00_GPIO_FUNC_0);
        ertecx00GpioDirectionSet(GPIO_LED_ACTIVP4, ERTECX00_GPIO_OUT);
        ertecx00GpioFunctionSet (GPIO_LED_ACTIVP4, ERTECX00_GPIO_FUNC_0);
        ertecx00GpioOut (GPIO_LED_LINKP1, 1);
        ertecx00GpioOut (GPIO_LED_LINKP2, 1);
        ertecx00GpioOut (GPIO_LED_LINKP3, 1);
        ertecx00GpioOut (GPIO_LED_LINKP4, 1);
        ertecx00GpioOut (GPIO_LED_ACTIVP1, 1);
        ertecx00GpioOut (GPIO_LED_ACTIVP2, 1);
        ertecx00GpioOut (GPIO_LED_ACTIVP3, 1);
        ertecx00GpioOut (GPIO_LED_ACTIVP4, 1);
#elif (PNIOD_PLATFORM & (PNIOD_PLATFORM_ECOS_EB200P | PNIOD_PLATFORM_POSIX_EB200P))

#endif

#if ( IOD_INCLUDE_POF == 1 )
        ertecx00GpioDirectionSet(GPIO_LED_FO1,     ERTECX00_GPIO_OUT);
        ertecx00GpioFunctionSet (GPIO_LED_FO1,     ERTECX00_GPIO_FUNC_0);
        ertecx00GpioDirectionSet(GPIO_LED_FO2,     ERTECX00_GPIO_OUT);
        ertecx00GpioFunctionSet (GPIO_LED_FO2,     ERTECX00_GPIO_FUNC_0);
        // disable FO maintenance LEDs
        Bsp_EbSetLed(PNIO_LED_FO1, 0);
        Bsp_EbSetLed(PNIO_LED_FO2, 0);
#endif
    }


    PNIO_VOID BspLed_StartLedBlink (PNIO_UINT32 frequency)
    {
        #if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200)

            //enable Firmware to access the Link-LEDs
            ertecx00GpioFunctionSet (GPIO_LED_LINKP1,  ERTECX00_GPIO_FUNC_0);
            ertecx00GpioFunctionSet (GPIO_LED_LINKP2,  ERTECX00_GPIO_FUNC_0);

        #elif (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB400)

            LSA_UINT32              PhyAuxModReg;
            LSAS_E400_LED_PARAM     eb400_led_param_bsp;

            LSAS_GET_PARAM_LED_BLINK(&eb400_led_param_bsp);

            //Set LinkLEDDisable in auxiliary mode register of PHY BCM5221 to override PHY with GPIO
            eddi_ll_PhyRead(eb400_led_param_bsp.hSysDev, eb400_led_param_bsp.hDDB, eb400_led_param_bsp.HwPortIndex, EB400_SER_PHY_AUX_MODE_ADR, &PhyAuxModReg);
            PhyAuxModReg |= (1 << EB400_SER_PHY_AUXILIARY_MODE_BIT__LinkLEDDisable);
            eddi_ll_PhyWrite(eb400_led_param_bsp.hSysDev, eb400_led_param_bsp.hDDB, eb400_led_param_bsp.HwPortIndex, EB400_SER_PHY_AUX_MODE_ADR, &PhyAuxModReg);


        #elif (PNIOD_PLATFORM & (PNIOD_PLATFORM_ECOS_EB200P | PNIOD_PLATFORM_POSIX_EB200P))
            PNIO_printf ("START BLINK LED \n");
            OsStartTimer(IdentTimerID, 0, (PNIO_UINT16)(5/frequency)); /* half the period normalized on 100ms */

        #endif
    }

    PNIO_VOID BspLed_StopLedBlink (PNIO_VOID)
    {
        #if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200)

            //give control back to PHY
            ertecx00GpioFunctionSet (GPIO_LED_LINKP1,  ERTECX00_GPIO_FUNC_1);
            ertecx00GpioFunctionSet (GPIO_LED_LINKP2,  ERTECX00_GPIO_FUNC_1);

        #elif (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB400)

            LSA_UINT32              PhyAuxModReg;
            LSAS_E400_LED_PARAM    eb400_led_param_bsp;

            LSAS_GET_PARAM_LED_BLINK(&eb400_led_param_bsp);
            //Reset LinkLEDDisable in auxiliary mode register of PHY BCM5221
            eddi_ll_PhyRead(eb400_led_param_bsp.hSysDev, eb400_led_param_bsp.hDDB, eb400_led_param_bsp.HwPortIndex, EB400_SER_PHY_AUX_MODE_ADR, &PhyAuxModReg);
            PhyAuxModReg &= ~(1 << EB400_SER_PHY_AUXILIARY_MODE_BIT__LinkLEDDisable);
            eddi_ll_PhyWrite(eb400_led_param_bsp.hSysDev, eb400_led_param_bsp.hDDB, eb400_led_param_bsp.HwPortIndex, EB400_SER_PHY_AUX_MODE_ADR, &PhyAuxModReg);

        #elif (PNIOD_PLATFORM & (PNIOD_PLATFORM_ECOS_EB200P | PNIOD_PLATFORM_POSIX_EB200P))
            PNIO_printf ("STOP BLINK LED \n");
            OsStopTimer(IdentTimerID);
            led_state = PNIO_FALSE;
            Bsp_EbSetLed(PNIO_LED_RUN,  LedState[PNIO_LED_RUN]);
            Bsp_EbSetLed(PNIO_LED_ERROR,LedState[PNIO_LED_ERROR]);
        #endif
    }


    // *-----------------------------------------------------------*
    // *  Bsp_EbSetLed ()
    // *
    // *  switch on/off the green/red LED on the EB
    // *  example implementation the port mac address is build by
    // *  incrementing the device mac address.
    // *
    // *
    // *  input:  Led               PNIO_LED_...
    // *          Val               1: switch on, 0: switch off
    // *  output: return            PNIO_OK
    // *                            PNIO_NOT_OK   invalid params
    // *-----------------------------------------------------------*
    PNIO_UINT32 Bsp_EbSetLed (PNIO_LEDTYPE Led, PNIO_UINT32 Val)
    {
		#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200_400)
        switch (Led)
        {
            case PNIO_LED_RUN:
            {
                LedState[PNIO_LED_RUN] = !Val;
                ertecx00GpioOut(GPIO_LED_RUN, !Val);
                break;
            }
            case PNIO_LED_MAINT:
            {
                break;
            }
            case PNIO_LED_ERROR:
            {
                LedState[PNIO_LED_ERROR] = !Val;
                ertecx00GpioOut(GPIO_LED_ERROR, !Val);
                break;
            }
            case PNIO_LED_LINKP1:
            {
                LedState[PNIO_LED_LINKP1] = !Val;
                ertecx00GpioOut(GPIO_LED_LINKP1, !Val);
                break;
            }
            case PNIO_LED_LINKP2:
            {
                LedState[PNIO_LED_LINKP2] = !Val;
                ertecx00GpioOut(GPIO_LED_LINKP2, !Val);
                break;
            }
            case PNIO_LED_LINKP3:
            {
			#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB400)
                LedState[PNIO_LED_LINKP3] = !Val;
                ertecx00GpioOut(GPIO_LED_LINKP3, !Val);
                break;
			#else
                return PNIO_NOT_OK;
			#endif
            }
            case PNIO_LED_LINKP4:
            {
			#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB400)
                LedState[PNIO_LED_LINKP4] = !Val;
                ertecx00GpioOut(GPIO_LED_LINKP4, !Val);
                break;
			#else
                return PNIO_NOT_OK;
			#endif
            }
            case PNIO_LED_ACTIVP1:
            {
                LedState[PNIO_LED_ACTIVP1] = !Val;
                ertecx00GpioOut(GPIO_LED_ACTIVP1, !Val);
                break;
            }
            case PNIO_LED_ACTIVP2:
            {
                LedState[PNIO_LED_ACTIVP2] = !Val;
                ertecx00GpioOut(GPIO_LED_ACTIVP2, !Val);
                break;
            }
            case PNIO_LED_ACTIVP3:
            {
			#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB400)
                LedState[PNIO_LED_ACTIVP3] = !Val;
                ertecx00GpioOut(GPIO_LED_ACTIVP3, !Val);
                break;
			#else
                return PNIO_NOT_OK;
			#endif
            }
            case PNIO_LED_ACTIVP4:
            {
			#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB400)
                LedState[PNIO_LED_ACTIVP4] = !Val;
                ertecx00GpioOut(GPIO_LED_ACTIVP4, !Val);
                break;
			#else
                return PNIO_NOT_OK;
			#endif
            }
            case PNIO_LED_FO1:
            {
			#if ( IOD_INCLUDE_POF == 1 )
                LedState[PNIO_LED_FO1] = !Val;
				#ifdef IOD_ENABLE_POF_LED
                	ertecx00GpioOut(GPIO_LED_FO1, !Val);
				#endif
                break;
			#else
                return PNIO_NOT_OK;
			#endif
            }
            case PNIO_LED_FO2:
            {
			#if ( IOD_INCLUDE_POF == 1 )
                LedState[PNIO_LED_FO2] = !Val;
				#ifdef IOD_ENABLE_POF_LED
                	ertecx00GpioOut(GPIO_LED_FO2, !Val);
				#endif
                break;
			#else
                return PNIO_NOT_OK;
			#endif
            }
            case PNIO_LED_SYNC:
            case PNIO_LED_POWER:
            case PNIO_LED_DIAG:
            case PNIO_LED_PENERGY:
            case PNIO_LED_BLINK:
			case PNIO_LED_USER_00:
			case PNIO_LED_USER_01:
			case PNIO_LED_USER_02:
			case PNIO_LED_USER_03:
			case PNIO_LED_USER_04:
			case PNIO_LED_USER_05:
			case PNIO_LED_USER_06:
			case PNIO_LED_USER_07:
			case PNIO_LED_USER_08:
			case PNIO_LED_USER_09:
			case PNIO_LED_USER_10:
			case PNIO_LED_USER_11:
			case PNIO_LED_USER_12:
			case PNIO_LED_USER_13:
			case PNIO_LED_USER_14:
			case PNIO_LED_USER_15:
            default:
            {
                return PNIO_NOT_OK;
            }
        }
        #elif (PNIOD_PLATFORM & (PNIOD_PLATFORM_ECOS_EB200P | PNIOD_PLATFORM_POSIX_EB200P))
        PNIO_UINT32 gpioNr = 0xFFFF;
        switch (Led)
        {
		/*	note: SPI boot uses GPIO 26, 27 and 31.. so this ones have to be freed from LED duty..	*/
		/*		  three new leds are present on SPI adapter.. use GPIO 8, 10 and 11 instead 		*/
			case PNIO_LED_MAINT   :
			{
				if( MODE_SPI_0x03 == ( REG32( U_SCRB__BOOT_REG ) &0xf )  ||
						( 1 == spi_in_use ) )
					{
					gpioNr = 8;
					break;
					}
				else
				{
					gpioNr = GPIO_LED_MAINT;
					break;
				}
			}
			case PNIO_LED_ERROR   :	gpioNr = GPIO_LED_ERROR  ; break;
			case PNIO_LED_SYNC    : gpioNr = GPIO_LED_SYNC   ; break;
			case PNIO_LED_FO1     : gpioNr = GPIO_LED_FO1    ; break;
			case PNIO_LED_FO2     :
			{
				if( MODE_SPI_0x03 == ( REG32( U_SCRB__BOOT_REG ) &0xf ) ||
						( 1 == spi_in_use ) )
					{
					gpioNr = 10;
					break;
					}
				else
				{
					gpioNr = GPIO_LED_FO2;
					break;
				}
			}
			case PNIO_LED_DIAG    :
			{
				if( MODE_SPI_0x03 == ( REG32( U_SCRB__BOOT_REG ) &0xf )  ||
						( 1 == spi_in_use ) )
					{
					gpioNr = 11;
					break;
					}
				else
				{
					gpioNr = GPIO_LED_DIAG;
					break;
				}
			}
			case PNIO_LED_PENERGY : gpioNr = GPIO_LED_PENERGY; break;
			case PNIO_LED_USER_00 : gpioNr = GPIO_LED_USER_00; break;
			case PNIO_LED_USER_01 : gpioNr = GPIO_LED_USER_01; break;
			case PNIO_LED_USER_02 : gpioNr = GPIO_LED_USER_02; break;
			case PNIO_LED_USER_03 : gpioNr = GPIO_LED_USER_03; break;
			case PNIO_LED_USER_04 : gpioNr = GPIO_LED_USER_04; break;
			case PNIO_LED_USER_05 : gpioNr = GPIO_LED_USER_05; break;
			case PNIO_LED_USER_06 : gpioNr = GPIO_LED_USER_06; break;
			case PNIO_LED_USER_07 : gpioNr = GPIO_LED_USER_07; break;
			case PNIO_LED_USER_08 : gpioNr = GPIO_LED_USER_08; break;
			case PNIO_LED_USER_09 : gpioNr = GPIO_LED_USER_09; break;
			case PNIO_LED_USER_10 : gpioNr = GPIO_LED_USER_10; break;
			case PNIO_LED_USER_11 : gpioNr = GPIO_LED_USER_11; break;
			case PNIO_LED_USER_12 : gpioNr = GPIO_LED_USER_12; break;
			case PNIO_LED_USER_13 : gpioNr = GPIO_LED_USER_13; break;
			case PNIO_LED_USER_14 : gpioNr = GPIO_LED_USER_14; break;
			case PNIO_LED_USER_15 : gpioNr = GPIO_LED_USER_15; break;
			case PNIO_LED_BLINK:
			case PNIO_LED_RUN:
			case PNIO_LED_LINKP1:
			case PNIO_LED_LINKP2:
			case PNIO_LED_LINKP3:
			case PNIO_LED_LINKP4:
			case PNIO_LED_ACTIVP1:
			case PNIO_LED_ACTIVP2:
			case PNIO_LED_ACTIVP3:
			case PNIO_LED_ACTIVP4:
			case PNIO_LED_POWER:
            default:
            {
                return PNIO_NOT_OK;
            }
        }
        if (0xFFFF != gpioNr)
        {
            if (Val)
            {
                // PNIO_printf (" ON\n");
                REG32(U_GPIO__GPIO_OUT_SET_0) = (1<<gpioNr);
            }
            else
            {
                // PNIO_printf (" OFF\n");
                REG32(U_GPIO__GPIO_OUT_CLEAR_0) = (1<<gpioNr);
            }
        }
		#endif
        return PNIO_OK;
    }

    PNIO_VOID  BspLed_InitLedBlink (PNIO_VOID)
    {
        /* init timer for identify blinking */
        OsAllocTimer(&IdentTimerID, LSA_TIMER_TYPE_CYCLIC ,LSA_TIME_BASE_100MS ,BspLed_IdentTimerCbf);
    }


#endif // PNIOD_PLATFORM


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
