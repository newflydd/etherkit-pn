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
/*  F i l e               &F: hamaport.c                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  control processor ports for ERTEC 200P                                   */
/*                                                                           */
/*****************************************************************************/


#include "compiler.h"
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P)
    #include "hama_com.h"
    #include "ertec200p_reg.h"
    #include "lsa_cfg.h"

    #define LTRC_ACT_MODUL_ID   118
    #define IOD_MODULE_ID       118

    /*****************************************************************************/
    /*****************************************************************************/
    /*****************************************************************************/
    /**********                                                         **********/
    /**********                   INTERNAL FUNCTIONS                    **********/
    /**********                                                         **********/
    /*****************************************************************************/
    /*****************************************************************************/
    /*****************************************************************************/


    /*****************************************************************************/

    /*****************************************************************************/
    /*****************************************************************************/
    /*****************************************************************************/
    /**********                                                         **********/
    /**********               SYSTEM INTERFACE FUNCTIONS                **********/
    /**********                                                         **********/
    /*****************************************************************************/
    /*****************************************************************************/
    /*****************************************************************************/

    /**
     * @brief   read the input level of pin >pin_number< (0-95)
     */
    LSA_BOOL hama_port_get_pin_value(LSA_UINT16 pin_number)
    {
        volatile LSA_UINT32 *port_in_reg;

        if(pin_number < 32)
        {
            port_in_reg = (LSA_UINT32*) U_GPIO__GPIO_IN_0;
        }
        else if(pin_number < 64)
        {
            port_in_reg = (LSA_UINT32*) U_GPIO__GPIO_IN_1;
        }
        else if(pin_number < 96)
        {
            port_in_reg = (LSA_UINT32*) U_GPIO__GPIO_IN_2;
        }
        else
        {
            for(;;);
        }

        if(*port_in_reg & (1 << (pin_number % 32)))
        {
            return LSA_TRUE;
        }
        else
        {
            return LSA_FALSE;
        }
    }

    /**
     * @brief   set the output level of pin >pin_number< (0-95). Call this function after you've set the pin mode to GPIO
     */
    void hama_port_set_pin_value(LSA_UINT16 pin_number, LSA_BOOL value)
    {
        volatile LSA_UINT32 *port_modify_reg;

        if(value)
        {
            if(pin_number < 32)
            {
                port_modify_reg = (LSA_UINT32*) U_GPIO__GPIO_OUT_SET_0;
            }
            else if(pin_number < 64)
            {
                port_modify_reg = (LSA_UINT32*) U_GPIO__GPIO_OUT_SET_1;
            }
            else if(pin_number < 96)
            {
                port_modify_reg = (LSA_UINT32*) U_GPIO__GPIO_OUT_SET_2;
            }
            else
            {
        	    for(;;);
            }
        }
        else
        {
            if(pin_number < 32)
            {
                port_modify_reg = (LSA_UINT32*) U_GPIO__GPIO_OUT_CLEAR_0;
            }
            else if(pin_number < 64)
            {
                port_modify_reg = (LSA_UINT32*) U_GPIO__GPIO_OUT_CLEAR_1;
            }
            else if(pin_number < 96)
            {
                port_modify_reg = (LSA_UINT32*) U_GPIO__GPIO_OUT_CLEAR_2;
            }
            else
            {
        	    for(;;);
            }
        }

        *port_modify_reg |= (1 << (pin_number % 32));
    }

    /*****************************************************************************/
    /**
     * @brief ERTEC200P port direction setting.
     *
     * @param pin_number   index of the GPIO, range 0 to 95
     * @param direction    enum : HAMA_PORT_DIRECTION_INPUT / HAMA_PORT_DIRECTION_OUTPUT
     */
    void hama_port_set_direction(LSA_UINT16 pin_number, hama_port_direction_t direction)
    {
        volatile LSA_UINT32 *port_direction_reg;

        if(pin_number < 32)
        {
            port_direction_reg = (LSA_UINT32*) U_GPIO__GPIO_IOCTRL_0;
        }
        else if(pin_number < 64)
        {
            port_direction_reg = (LSA_UINT32*) U_GPIO__GPIO_IOCTRL_1;
        }
        else if(pin_number < 96)
        {
            port_direction_reg = (LSA_UINT32*) U_GPIO__GPIO_IOCTRL_2;
        }
        else
        {
    	    for(;;);
        }

        switch(direction)
        {
            case HAMA_PORT_DIRECTION_INPUT:
                *port_direction_reg |= (1UL << (pin_number % 32));
                break;
            case HAMA_PORT_DIRECTION_OUTPUT:
                *port_direction_reg &= ~(1UL << (pin_number % 32));
                break;
           default:
        	    for(;;);
        }
    }


    /**
     * @brief   This function assigns one of 4 functions to the specified GPIO pin.
     *
     * @param   pin_number  value in the range 0-95
     * @param   mode        choose between HAMA_PORT_FUNCTION_GPIO, HAMA_PORT_FUNCTION_ALTERNATE_A, HAMA_PORT_FUNCTION_ALTERNATE_B, HAMA_PORT_FUNCTION_ALTERNATE_C
     */
    void hama_port_set_pin_mode(LSA_UINT16 pin_number, hama_port_pin_mode_t mode)
    {
        volatile LSA_UINT32 *port_mode_reg;

        if(pin_number < 16)
        {
            port_mode_reg = (LSA_UINT32*) U_GPIO__GPIO_PORT_MODE_0_L;
        }
        else if(pin_number < 32)
        {
            port_mode_reg = (LSA_UINT32*) U_GPIO__GPIO_PORT_MODE_0_H;
        }
        else if(pin_number < 48)
        {
            port_mode_reg = (LSA_UINT32*) U_GPIO__GPIO_PORT_MODE_1_L;
        }
        else if(pin_number < 64)
        {
            port_mode_reg = (LSA_UINT32*) U_GPIO__GPIO_PORT_MODE_1_H;
        }
        else if(pin_number < 80)
        {
            port_mode_reg = (LSA_UINT32*) U_GPIO__GPIO_PORT_MODE_2_L;
        }
        else if(pin_number < 96)
        {
            port_mode_reg = (LSA_UINT32*) U_GPIO__GPIO_PORT_MODE_2_H;
        }
        else
        {
    	    for(;;);
        }

        switch(mode)
        {
            case HAMA_PORT_FUNCTION_GPIO:
                *port_mode_reg = (*port_mode_reg & ~(3UL << ((pin_number % 16) * 2))) | (0UL << ((pin_number % 16) * 2));
                break;
            case HAMA_PORT_FUNCTION_ALTERNATE_A:
                *port_mode_reg = (*port_mode_reg & ~(3UL << ((pin_number % 16) * 2))) | (1UL << ((pin_number % 16) * 2));
                break;
            case HAMA_PORT_FUNCTION_ALTERNATE_B:
                *port_mode_reg = (*port_mode_reg & ~(3UL << ((pin_number % 16) * 2))) | (2UL << ((pin_number % 16) * 2));
                break;
            case HAMA_PORT_FUNCTION_ALTERNATE_C:
                *port_mode_reg = (*port_mode_reg & ~(3UL << ((pin_number % 16) * 2))) | (3UL << ((pin_number % 16) * 2));
                break;
            default:
        	    for(;;);
        }
    }
#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
