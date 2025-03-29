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
#include "pnioerrx.h"
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_RZT2)

    #include "glob_sys.h"
    #include "trc_if.h"
    #include "bspadapt.h"
    #include "bspledadapt.h"
	#include "bsp_api.h"
	#include "r_ioport.h"

    // *------------ local data for LED blink  ----------*
    static  PNIO_UINT16     BspTimerID  = 0;
    static  PNIO_BOOL       led_state 	  = 0;
    static  PNIO_BOOL       led_Blink_ctrl = 0;
    LSA_UINT32 BspTaskId = 0;
    static  PNIO_UINT8      LedState[PNIO_LED_NUMOF_ENTRIES];

    // *------------ external data for LED blink  ----------*
    extern ioport_instance_ctrl_t g_ioport_ctrl;

#ifdef _RENESAS_RZT_
#define RZT2_LED_GREEN_1	BSP_IO_PORT_19_PIN_6 // RLED0
#define RZT2_LED_GREEN_2	BSP_IO_PORT_19_PIN_4 // RLED1
#define RZT2_LED_GREEN_3	BSP_IO_PORT_20_PIN_0 // RLED2
#define RZT2_LED_MOD_GREEN	BSP_IO_PORT_20_PIN_4 // ETH_LED3
#define RZT2_LED_BUS_GREEN	BSP_IO_PORT_21_PIN_0 // ETH_LED5
#define RZT2_LED_BUS_RED	BSP_IO_PORT_20_PIN_3 // ETH_LED1
#define RZT2_LED_YELLOW		BSP_IO_PORT_19_PIN_3 // ETH_LED7
#define RZT2_LED_RED 		BSP_IO_PORT_08_PIN_2 // ETH_LED6
#endif
#ifdef _RENESAS_RZN_
#define RZT2_LED_GREEN_1	BSP_IO_PORT_18_PIN_2 // RLED0
#define RZT2_LED_GREEN_2	BSP_IO_PORT_22_PIN_3 // RLED1
#define RZT2_LED_GREEN_3	BSP_IO_PORT_04_PIN_2 // RLED2
#define RZT2_LED_MOD_GREEN	BSP_IO_PORT_20_PIN_4 // ETH_LED3
#define RZT2_LED_BUS_GREEN	BSP_IO_PORT_22_PIN_1 // ETH_LED5
#define RZT2_LED_BUS_RED	BSP_IO_PORT_20_PIN_3 // ETH_LED1
#define RZT2_LED_YELLOW		BSP_IO_PORT_05_PIN_0 // ETH_LED7
#define RZT2_LED_RED 		BSP_IO_PORT_04_PIN_4 // ETH_LED6
#endif


PNIO_VOID BspLed_Restore (PNIO_VOID);

PNIO_UINT32 RZT2_PNS_GetLedStat (int Led)
{
	bsp_io_level_t level;
	PNIO_UINT32 nLedStat = 0;

	switch (Led)
	{
	case PNIO_LED_ERROR:
	case BSP_LED_FAULT:
	case BSP_LED_MOD_ERR:
	case BSP_LED_BUS_ERR:
		R_IOPORT_PinRead(&g_ioport_ctrl, (bsp_io_port_pin_t) RZT2_LED_RED, (bsp_io_level_t*) &nLedStat);
		break;
	case BSP_LED_WARNING:
		R_IOPORT_PinRead(&g_ioport_ctrl, (bsp_io_port_pin_t) RZT2_LED_YELLOW, (bsp_io_level_t*) &nLedStat);
		break;
	case BSP_LED_MOD_RUN:
		R_IOPORT_PinRead(&g_ioport_ctrl, (bsp_io_port_pin_t) RZT2_LED_MOD_GREEN, (bsp_io_level_t*) &nLedStat);
		break;
	case BSP_LED_BUS_RUN:
	case PNIO_LED_RUN:
		R_IOPORT_PinRead(&g_ioport_ctrl, (bsp_io_port_pin_t) RZT2_LED_BUS_GREEN, (bsp_io_level_t*) &nLedStat);
		break;
	case BSP_LED_1:
		R_IOPORT_PinRead(&g_ioport_ctrl, (bsp_io_port_pin_t) RZT2_LED_GREEN_1, (bsp_io_level_t*) &nLedStat);
		break;
	case BSP_LED_2:
		R_IOPORT_PinRead(&g_ioport_ctrl, (bsp_io_port_pin_t) RZT2_LED_GREEN_2, (bsp_io_level_t*) &nLedStat);
		break;
	case BSP_LED_3:
		R_IOPORT_PinRead(&g_ioport_ctrl, (bsp_io_port_pin_t) RZT2_LED_GREEN_3, (bsp_io_level_t*) &nLedStat);
		break;
	case BSP_LED_GROUP:
		R_IOPORT_PinRead(&g_ioport_ctrl, (bsp_io_port_pin_t) RZT2_LED_GREEN_1, &level);
		nLedStat = level;
		R_IOPORT_PinRead(&g_ioport_ctrl, (bsp_io_port_pin_t) RZT2_LED_GREEN_2, &level);
		nLedStat |= (uint32_t) (level << 1);
		R_IOPORT_PinRead(&g_ioport_ctrl, (bsp_io_port_pin_t) RZT2_LED_GREEN_3, &level);
		nLedStat |= (uint32_t) (level << 2);
		break;
	case PNIO_LED_MAINT   :
	case PNIO_LED_SYNC    :
	case PNIO_LED_FO1     :
	case PNIO_LED_FO2     :
	case PNIO_LED_DIAG    :
	case PNIO_LED_PENERGY :
	default:
			break;
	}
	return nLedStat;
}


int RZT2_PNS_SetLed (int Led, int Val)
{
	PNIO_UINT32 nRslt = PNIO_OK;

	R_BSP_PinAccessEnable();

	switch (Led)
	{
	case PNIO_LED_ERROR:
	case BSP_LED_FAULT:
	case BSP_LED_MOD_ERR:
	case BSP_LED_BUS_ERR:
		R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) RZT2_LED_RED, Val);
		break;
	case BSP_LED_WARNING:
		R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) RZT2_LED_YELLOW, Val);
		break;
	case BSP_LED_MOD_RUN:
		R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) RZT2_LED_MOD_GREEN, Val);
		break;
	case BSP_LED_BUS_RUN:
	case PNIO_LED_RUN:
		R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) RZT2_LED_BUS_GREEN, Val);
		R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) RZT2_LED_BUS_RED, ((~Val) & 0x0001));
		break;
	case BSP_LED_1:
		R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) RZT2_LED_GREEN_1, Val);
		break;
	case BSP_LED_2:
		R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) RZT2_LED_GREEN_2, Val);
		break;
	case BSP_LED_3:
		R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) RZT2_LED_GREEN_3, Val);
		break;
	case PNIO_LED_BLINK:
		R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) RZT2_LED_MOD_GREEN, Val);
		R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) RZT2_LED_BUS_GREEN, Val);
		R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) RZT2_LED_BUS_RED, Val);
		R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) RZT2_LED_YELLOW, Val);
		R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) RZT2_LED_RED, Val);
		break;
	case PNIO_LED_MAINT   :
	case PNIO_LED_SYNC    :
	case PNIO_LED_FO1     :
	case PNIO_LED_FO2     :
	case PNIO_LED_DIAG    :
	case PNIO_LED_PENERGY :
			break;
		default:
			//Error PNIO_NOT_OK;
			break;
	}

	R_BSP_PinAccessDisable();

	return nRslt;
}

void RZT2_PNS_LedCtrl (void * pvParameters)
{
	static uint32_t nCnt = 0;
	static bsp_io_level_t pin_level = BSP_IO_LEVEL_HIGH;

	if(NULL != pvParameters)
	{
		/* LED type structure */
		bsp_leds_t *ptLeds = (bsp_leds_t*)pvParameters;//g_app_leds;
		//R_BSP_PinAccessEnable();

	//	R_BSP_PinAccessDisable();
		nCnt++;
		if(nCnt >= 4)//ptLeds->led_count)
		{
			nCnt = 0;
			//nDir ^= 1;
			pin_level ^= 1;
		}

		if(0 < pin_level)
		{
			R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) ptLeds->p_leds[4-nCnt][0], pin_level);
		}
		else
		{
			R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) ptLeds->p_leds[nCnt][0], pin_level);
		}

	}
}

    PNIO_VOID BspLed_IdentTimerCbf(PNIO_UINT16 timer_id, PNIO_UINT32 user_id)
    {
     //   led_state ^= 1;
    //    Bsp_EbSetLed(PNIO_LED_BLINK, led_state);
   //     Bsp_EbSetLed(PNIO_LED_ERROR, led_state);

        OsSendMessageX(BspTaskId, (void*) BSP_LED_CYCL_CTRL, (void*)user_id, 0);
    }

    void  BspLed_CtrlTask (void)
    {
    	void* pLedCtrl = LSA_NULL;
    	void* pVal = LSA_NULL;
    	int nLedCtrl = 0;
    	PNIO_UINT8 nVal = 0;

    	while(1)
    	{
    		pLedCtrl = LSA_NULL;
    		pVal = LSA_NULL;
    		nLedCtrl = 0;
    		nVal = 0;
    		if(PNIO_OK == OsReadMessageBlockedX (&pLedCtrl, &pVal, BspTaskId))
    		{
    			nLedCtrl = (int)pLedCtrl;
    			nVal = (int)pVal;
    		}

    		switch(nLedCtrl)
    		{
    		case BSP_LED_CYCL_CTRL:
    			led_state ^= 1;
			//	RZT2_PNS_LedCtrl((bsp_leds_t*)&g_app_leds);
    			break;
    		case BSP_LED_BLINK_START:
    			led_Blink_ctrl = 1;
    			break;
    		case BSP_LED_BLINK_STOP:
    			led_state = LSA_FALSE;
    			led_Blink_ctrl = 0;
    			BspLed_Restore();
    			break;
    		case PNIO_LED_BLINK:
    			if(0 < led_Blink_ctrl)
				{
    				RZT2_PNS_SetLed(nLedCtrl, nVal);
				}
    			break;
    		case BSP_LED_GROUP:
    			if(0 < (nVal & 0x01))
    			{
    				RZT2_PNS_SetLed(BSP_LED_1, 1);
    			}
    			else
    			{
    				RZT2_PNS_SetLed(BSP_LED_1, 0);
    			}
    			if(0 < (nVal & 0x02))
				{
					RZT2_PNS_SetLed(BSP_LED_2, 1);
				}
				else
				{
					RZT2_PNS_SetLed(BSP_LED_2, 0);
				}
    			if(0 < (nVal & 0x04))
				{
					RZT2_PNS_SetLed(BSP_LED_3, 1);
				}
				else
				{
					RZT2_PNS_SetLed(BSP_LED_3, 0);
				}
    			break;
    		case PNIO_LED_ERROR:
    		case PNIO_LED_RUN:
    		case BSP_LED_FAULT:
			case BSP_LED_MOD_ERR:
			case BSP_LED_BUS_ERR:
			case BSP_LED_WARNING:
			case BSP_LED_MOD_RUN:
			case BSP_LED_BUS_RUN:
				LedState[nLedCtrl] = nVal;
				if(0 == led_Blink_ctrl)
				{
					RZT2_PNS_SetLed(nLedCtrl, nVal);
				}
				break;
    		default:
				if(nLedCtrl < PNIO_LED_NUMOF_ENTRIES)
				{
					LedState[nLedCtrl] = nVal;
					RZT2_PNS_SetLed(nLedCtrl, nVal);
				}
    			break;
    		}
    	}
    }

	PNIO_VOID  BspLed_InitLedBlink (PNIO_VOID)
	{
		PNIO_INT32 Status  = 0;
		/* init timer for identify blinking */
		if(0U == BspTaskId)
		{
			OsMemSet (LedState, 0, sizeof (LedState));

			Status = OsCreateThread ((void  (*)(void))BspLed_CtrlTask, (PNIO_UINT8*) "BspLed_Ctrl",  TASK_PRIO_LED_CTRL, (PNIO_VOID*)&BspTaskId);

			if(PNIO_OK == Status)
			{
				Status = OsCreateMsgQueue (BspTaskId);
			}

			if(PNIO_OK == Status)
			{
				Status = OsStartThread (BspTaskId);
			}

			if(PNIO_OK == Status)
			{
				Status = OsAllocTimer(&BspTimerID, LSA_TIMER_TYPE_CYCLIC ,LSA_TIME_BASE_100MS , (PNIO_VOID*)BspLed_IdentTimerCbf);
			}

			if(PNIO_OK == Status)
			{
				OsStartTimer (BspTimerID, 0, 5);
			}
		}
	}

    PNIO_VOID  BspLed_Open (PNIO_VOID)
    {
        BspLed_InitLedBlink ();
    }



    PNIO_VOID BspLed_Restore (PNIO_VOID)
	{
    	Bsp_EbSetLed(PNIO_LED_RUN,    LedState[PNIO_LED_RUN]);
    	Bsp_EbSetLed(PNIO_LED_ERROR,  LedState[PNIO_LED_ERROR]);
    	Bsp_EbSetLed(BSP_LED_MOD_RUN, LedState[BSP_LED_MOD_RUN]);
    	Bsp_EbSetLed(BSP_LED_FAULT,   LedState[BSP_LED_FAULT]);
    	Bsp_EbSetLed(BSP_LED_MOD_ERR, LedState[BSP_LED_MOD_ERR]);
    	Bsp_EbSetLed(BSP_LED_BUS_ERR, LedState[BSP_LED_BUS_ERR]);
    	Bsp_EbSetLed(BSP_LED_WARNING, LedState[BSP_LED_WARNING]);
	}


    PNIO_VOID BspLed_StartLedBlink (PNIO_UINT32 frequency)
    {
       // PNIO_printf ("START BLINK LED \n");
      //  OsStartTimer(BspTimerID, 0, (PNIO_UINT16)(5/frequency)); /* half the period normalized on 100ms */
        //led_Blink_ctrl = 1;
        OsSendMessageX(BspTaskId, (void*) BSP_LED_BLINK_START, (void*)frequency,0);
    }

    PNIO_VOID BspLed_StopLedBlink (PNIO_VOID)
    {
		//PNIO_printf ("STOP BLINK LED \n");
		//OsStopTimer(BspTimerID);
		//led_state = LSA_FALSE;
		//led_Blink_ctrl = 0;
		//BspLed_Restore();
    	OsSendMessageX(BspTaskId, (void*) BSP_LED_BLINK_STOP, (void*)0,0);
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
    	return OsSendMessageX(BspTaskId, (void*) Led, (void*)Val,0);
    }




#endif // PNIOD_PLATFORM


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
