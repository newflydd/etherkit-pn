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
/*  F i l e               &F: lsas_cfg.c                                :F&  */
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
 * @file    lsas_cfg.c
 * @brief   brief description
 * @author  cn3dit09
 * @version
 * @date    27.02.2012
 *
 * long description
 *
 */

/*****************************************************************************/
#define LSAS_MODULE_ID  7

#define INTERFACE_EXT_ATTR

#include "compiler.h"

#if ((PNIOD_PLATFORM & PNIOD_PLATFORM_POSIX_EB200P) || \
     (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200P) || \
     (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200_400)   || \
     (PNIOD_PLATFORM & PNIOD_PLATFORM_DKSW) || \
	 (PNIOD_PLATFORM & PNIOD_PLATFORM_RZT2) )

    #include "lsas_inc.h"
    #include "lsas_int.h"

	#if PNIO_BIG_ENDIAN
		#undef _LITTLE_ENDIAN  /* insert here because "ertec200.h" takes use of it */
		#define  _BIG_ENDIAN
	#else
		#define _LITTLE_ENDIAN  /* insert here because "ertec200.h" takes use of it */
		#undef  _BIG_ENDIAN
	#endif

    typedef struct
    {
        LSA_UINT32  port_id;

    }   LSAS_EDDI_I2C_USR_PORT_SEL;

    LSAS_EDDI_I2C_USR_PORT_SEL    lsas_eddi_i2c_port_sel;


#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB400)
    LSAS_E400_LED_PARAM         lsas_eb400_led_param;
#endif

     volatile GLOB_COUPLING_INTERFACE  glob_coupling_interface;

     LSA_UINT32 lsas_handle_oha_sys;

    /*=============================================================================
     * function name:  LSAS_WAIT_10NS
     *
     * function:       waiting 10ns which is provided by asm nops
     *
     * parameters:     ticks_10ns : number of nanosecs
     *
     * return value:   LSA_VOID
     *===========================================================================*/
	#ifndef LSAS_WAIT_10NS
	LSA_VOID LSAS_WAIT_10NS(unsigned int ticks_10ns)
	{
		ticks_10ns  = ticks_10ns >> 2;
#ifdef __GNUC__
        for (;ticks_10ns > 0; ticks_10ns--) __asm volatile ("nop");
#else
		for (;ticks_10ns > 0; ticks_10ns--) asm volatile ("nop");
#endif
	}
	#endif

    /*=============================================================================
     * function name:  LSAS_ENTER
     *
     * function:       Protect sequence within LSAS from beeing interrupted.
     *                 (reentrance protection)
     *
     * parameters:     LSA_VOID
     *
     * return value:   LSA_VOID
     *===========================================================================*/

    #ifndef LSAS_ENTER
    LSA_VOID  LSAS_SYSTEM_OUT_FCT_ATTR  LSAS_ENTER(
        LSA_VOID
    )
    {
    /*
        if ( !AM_LOCK_CHECK_ACCESS pnio )
        {
            AM_LOCK_CHECK_ACCESS pnio = 1;
        }
        else
        {
            //LSAS_FATAL_ERROR( 0);
        }
    */
    }
    #endif

    /*=============================================================================
     * function name:  LSAS_EXIT
     *
     * function:       Finishes LSAS_ENTER-Sequence.
     *
     * parameters:     LSA_VOID
     *
     * return value:   LSA_VOID
     *===========================================================================*/
    #ifndef LSAS_EXIT
    LSA_VOID  LSAS_SYSTEM_OUT_FCT_ATTR  LSAS_EXIT(
        LSA_VOID
    )
    {
        /*
        AM_LOCK_CHECK_ACCESS pnio = 0;
        */
    }
    #endif

    /*=============================================================================
     * function name:  LSAS_ALLOC_LOCAL_MEM
     *
     * function:       allocate a local-memory
     *
     * parameters:     LSAS_LOCAL_MEM_PTR_TYPE  ...  *  local_mem_ptr_ptr:
     *                                   return value: pointer to local-memory-
     *                                                 pointer
     *                                                 or LSA_NULL: No memory
     *                                                 available
     *                 LSA_UINT32                      length: length of memory
     *
     * return value:   LSA_VOID
     *===========================================================================*/

    LSA_VOID  LSAS_SYSTEM_OUT_FCT_ATTR  LSAS_ALLOC_LOCAL_MEM(
        LSAS_LOCAL_MEM_PTR_TYPE  LSAS_LOCAL_MEM_ATTR *  local_mem_ptr_ptr,
        LSA_UINT32                                      length
    )
    {
        LSAS_ALLOC_COMP_MEM(local_mem_ptr_ptr, length, LSA_COMP_ID_LLDP);
    }

    /*=============================================================================
     * function name:  LSAS_FREE_LOCAL_MEM
     *
     * function:       frees local -memory
     *
     * parameters:              ...  *  ret_val_ptr:
     *                     return value: LSA_RET_OK         ok
     *                                   LSA_RET_ERR_PARAM  no deallocation because
     *                                                      of wrong pointer to
     *                                                      local-memory
     *                 LSAS_LOCAL_MEM_PTR_TYPE  local_mem_ptr:  pointer to local-
     *                                                            memory
     *
     * return value:   LSA_VOID
     *===========================================================================*/

    LSA_VOID  LSAS_SYSTEM_OUT_FCT_ATTR  LSAS_FREE_LOCAL_MEM(
        LSA_UINT16                 LSAS_LOCAL_MEM_ATTR *  ret_val_ptr,
        LSAS_LOCAL_MEM_PTR_TYPE                           local_mem_ptr
    )
    {
        LSAS_FREE_COMP_MEM(ret_val_ptr, local_mem_ptr, LSA_COMP_ID_LLDP);
    }

    LSA_VOID  LSAS_EDD_LOWER_OUT_FCT_ATTR LSAS_EDD_REQUEST_OPEN_CHANNEL (LSAS_EDD_RQB_PTR_TYPE  lower_rqb_ptr)
    {
    #ifdef LSAS_CFG_USE_EDDI
        LSAS_RQB_EDD_REQUEST(lower_rqb_ptr,eddi_open_channel);
    #endif
    #ifdef LSAS_CFG_USE_EDDP
        LSAS_RQB_EDD_REQUEST(lower_rqb_ptr,eddp_open_channel);
    #endif
    #ifdef LSAS_CFG_USE_EDDS
        LSAS_RQB_EDD_REQUEST(lower_rqb_ptr,edds_open_channel);
    #endif
    }

    LSA_VOID  LSAS_ACP_LOWER_OUT_FCT_ATTR LSAS_ACP_REQUEST_OPEN_CHANNEL (LSAS_ACP_RQB_PTR_TYPE  lower_rqb_ptr)
    {
        LSAS_RQB_ACP_REQUEST(lower_rqb_ptr, acp_open_channel);
    }
#ifndef LSAS_CFG_USE_EXTERNAL_TCPIP_STACK
    LSA_VOID  LSAS_TCIP_LOWER_OUT_FCT_ATTR LSAS_TCIP_REQUEST_OPEN_CHANNEL (LSAS_TCIP_RQB_PTR_TYPE  lower_rqb_ptr)
    {
        LSAS_RQB_TCPIP_REQUEST(lower_rqb_ptr, tcip_open_channel);
    }
#endif
    LSA_VOID  LSAS_SOCK_LOWER_OUT_FCT_ATTR LSAS_SOCK_REQUEST_OPEN_CHANNEL (LSAS_SOCK_RQB_PTR_TYPE  lower_rqb_ptr)
    {
        LSAS_RQB_SOCK_REQUEST(lower_rqb_ptr, sock_open_channel);
    }

    LSA_VOID  LSAS_MRP_LOWER_OUT_FCT_ATTR LSAS_MRP_REQUEST_OPEN_CHANNEL (LSAS_MRP_RQB_PTR_TYPE  lower_rqb_ptr)
    {
        LSAS_RQB_MRP_REQUEST(lower_rqb_ptr, mrp_open_channel);
    }

    LSA_VOID  LSAS_OHA_LOWER_OUT_FCT_ATTR LSAS_OHA_REQUEST_OPEN_CHANNEL (LSAS_OHA_RQB_PTR_TYPE  lower_rqb_ptr)
    {
        LSAS_RQB_OHA_REQUEST(lower_rqb_ptr, oha_open_channel);
    }

    LSA_VOID  LSAS_CLRPC_LOWER_OUT_FCT_ATTR LSAS_CLRPC_REQUEST_OPEN_CHANNEL (LSAS_CLRPC_RQB_PTR_TYPE  lower_rqb_ptr)
    {
        LSAS_RQB_CLRPC_REQUEST(lower_rqb_ptr, clrpc_open_channel);
    }

    LSA_VOID  LSAS_NARE_LOWER_OUT_FCT_ATTR LSAS_NARE_REQUEST_OPEN_CHANNEL (LSAS_NARE_RQB_PTR_TYPE  lower_rqb_ptr)
    {
        LSAS_RQB_NARE_REQUEST(lower_rqb_ptr, nare_open_channel);
    }

    LSA_VOID  LSAS_GSY_LOWER_OUT_FCT_ATTR LSAS_GSY_REQUEST_OPEN_CHANNEL (LSAS_GSY_RQB_PTR_TYPE  lower_rqb_ptr)
    {
        LSAS_RQB_GSY_REQUEST(lower_rqb_ptr, gsy_open_channel);
    }

    LSA_VOID  LSAS_CM_LOWER_OUT_FCT_ATTR LSAS_CM_REQUEST_OPEN_CHANNEL (LSAS_CM_RQB_PTR_TYPE  lower_rqb_ptr)
    {
        LSAS_RQB_CM_REQUEST(lower_rqb_ptr, cm_open_channel);
    }

    LSA_VOID  LSAS_DCP_LOWER_OUT_FCT_ATTR LSAS_DCP_REQUEST_OPEN_CHANNEL (LSAS_DCP_RQB_PTR_TYPE  lower_rqb_ptr)
    {
        LSAS_RQB_DCP_REQUEST(lower_rqb_ptr, dcp_open_channel);
    }



    /*****************************************************************************/

    #ifdef LSAS_CFG_USE_EDDI

    LSA_VOID  lsas_set_pll_mode_out( LSA_RESULT                      * result,
                                     LSA_UINT32                        pIRTE,
                                     LSA_UINT32                        location,
                                     LSA_UINT16                        pllmod)
    {
    #define IRT_SYNC_INPUT_GPIO           24  /* Defined by ERTEC200/400                    */
    #define IRT_SYNC_OUTPUT_GPIO          25  /* ERTEC200: GPIO25..28 / ERTEC400: GPIO25    */

    #define ertec_local_port_mode_h_gpio  16  /* Defined by ERTEC200/400                    */
    #define ertec_mux_selector_gpio_base  25  /* Defined by ERTEC200/400                    */

    #if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200_400)
        volatile Unsigned32  *local_port_mode_h;
        volatile Unsigned32  *local_ioctrl;
        volatile Unsigned32  *mux_selector;
        IRTE_SW_MAKRO_T *_irte = (IRTE_SW_MAKRO_T *)LSAS_CFG_IRTE_START;

        if (!((EDDI_LOC_ERTEC200 == location) ||
              (EDDI_LOC_ERTEC400 == location)))
        {
            /* neither ERTEC200 nor ERTEC400 device */
            LSAS_FATAL(location);
        }
        local_port_mode_h    = &(*ERTEC200_GPIO).gpio_port_mode_h.reg;
        local_ioctrl         = &(*ERTEC200_GPIO).gpio_ioctrl.reg;
        mux_selector         = (Unsigned32 *)&_irte->s11000_Zyklussync.x11030_Mux_Selektor; /* IRTE register to set XPLLOut signal to pins */

        switch (pllmod)
        {
            case EDDI_PRM_DEF_PLL_MODE_OFF:
            {
                *local_ioctrl      &= ~(1 <<   IRT_SYNC_INPUT_GPIO);                                       /* GPIOxx Output              */
                *local_port_mode_h &= ~(3 << ((IRT_SYNC_INPUT_GPIO-ertec_local_port_mode_h_gpio)*2));      /* GPIOxx -> Clear bits       */

                *local_ioctrl      &= ~(1 <<   IRT_SYNC_OUTPUT_GPIO);                                      /* GPIOxx Output              */
                *local_port_mode_h &= ~(3 << ((IRT_SYNC_OUTPUT_GPIO-ertec_local_port_mode_h_gpio)*2));     /* GPIOxx -> Clear bits       */

                break;
            }
            case EDDI_PRM_DEF_PLL_MODE_XPLL_EXT_OUT:
            {
                *local_ioctrl      &= ~(1 <<   IRT_SYNC_OUTPUT_GPIO);                                     /* GPIOxx Output              */
                *local_port_mode_h &= ~(3 << ((IRT_SYNC_OUTPUT_GPIO-ertec_local_port_mode_h_gpio)*2));    /* GPIOxx -> Clear bits       */
                *local_port_mode_h |=  (1 << ((IRT_SYNC_OUTPUT_GPIO-ertec_local_port_mode_h_gpio)*2));    /* GPIOxx -> TGEN_OUT1        */

                /* select XPLL_Ext_Out for TGEN_OUTn */
                *mux_selector = (7 << ((IRT_SYNC_OUTPUT_GPIO-ertec_mux_selector_gpio_base)*4));           /* 7 = XPLL_Ext_Out           */

                break;
            }
            case EDDI_PRM_DEF_PLL_MODE_XPLL_EXT_IN:
            {
                *local_ioctrl      |= (1 <<    IRT_SYNC_INPUT_GPIO);                                     /* GPIOxx Input               */
                *local_port_mode_h &= ~(3 << ((IRT_SYNC_INPUT_GPIO-ertec_local_port_mode_h_gpio)*2));    /* GPIOxx -> Clear bits       */
                *local_port_mode_h |=  (1 << ((IRT_SYNC_INPUT_GPIO-ertec_local_port_mode_h_gpio)*2));    /* GPIOxx -> Mode XPLL IN     */

                break;
            }
            default:
            {
                /* Error */
                LSAS_FATAL(pllmod);
                break;
            }
        }
    #endif
        *(result) = EDD_STS_OK;
    }

    /*=============================================================================
     * function name:  EDDI_I2C_SCL_LOW_HIGH()
     *
     * function:       Sets the SCL signal of the selected I2C device to level low or high
     *
     * parameters:     ...
     *
     * return value:   LSA_VOID
     *===========================================================================*/
    #ifndef LSAS_EDDI_I2C_SCL_LOW_HIGH
    LSA_VOID  LSAS_EDDI_I2C_SCL_LOW_HIGH( LSA_BOOL          const  Level,
                                          LSA_UINT32  const  sys_ptr )
    {
        LSA_UNUSED_ARG( sys_ptr );
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200)
        Bsp_PofSclDir (lsas_eddi_i2c_port_sel.port_id, Level);
#endif

    }
    #endif

    /*=============================================================================
     * function name:  EDDI_I2C_SDA_LOW_HIGH()
     *
     * function:       Sets the SDA signal of the selected I2C device to level low or high
     *
     * parameters:     ...
     *
     * return value:   LSA_VOID
     *===========================================================================*/
    #ifndef LSAS_EDDI_I2C_SDA_LOW_HIGH
    LSA_VOID  LSAS_EDDI_I2C_SDA_LOW_HIGH( LSA_BOOL          const  Level,
                                          LSA_UINT32  const  sys_ptr )
    {
        LSA_UNUSED_ARG( sys_ptr );
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200)
        Bsp_PofSdaDir (lsas_eddi_i2c_port_sel.port_id, Level);
#endif
    }
    #endif

    /*=============================================================================
     * function name:  EDDI_I2C_SDA_READ()
     *
     * function:       Reads the SDA signal of the selected I2C device (0 or 1)
     *
     * parameters:     ...
     *
     * return value:   LSA_VOID
     *===========================================================================*/
    #ifndef LSAS_EDDI_I2C_SDA_READ
    LSA_VOID  LSAS_EDDI_I2C_SDA_READ( LSA_UINT8           *  const  value_ptr,
                                      LSA_UINT32             const  sys_ptr )
    {
        LSA_UNUSED_ARG( sys_ptr );
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200)
        *value_ptr = Bsp_PofSdaRead(lsas_eddi_i2c_port_sel.port_id);
#endif
    }
    #endif

    /*=============================================================================
     * function name:  EDDI_I2C_SDA_IN_OUT()
     *
     * function:       Sets the SDA signal of the selected I2C device to direction input or output
     *
     * parameters:     ...
     *
     * return value:   LSA_VOID
     *===========================================================================*/
    #ifndef LSAS_EDDI_I2C_SDA_IN_OUT
    LSA_VOID  LSAS_EDDI_I2C_SDA_IN_OUT( LSA_BOOL          const  OutPut,
                                        LSA_UINT32        const  sys_ptr )
    {
        LSA_UNUSED_ARG( sys_ptr );

        //Bsp_PofSdaDir (lsas_eddi_i2c_port_sel.port_id, !OutPut);

		/* do not toggle between input and output with following reason:    */
		/* this function is designed for setting the sda direction          */
		/* set to input is used to receive an acknowledge                   */
		/* set to output would cause sending 0 -> this is to be avoided     */
		/* mind implementation of function POF_I2C_SDA_LOW_HIGH too         */
    }
    #endif

    /*=============================================================================
     * function name:  EDDI_I2C_SCL_IN_OUT()
     *
     * function:       Sets the SCL signal of the selected I2C device to direction input or output
     *
     * parameters:     ...
     *
     * return value:   LSA_VOID
     *===========================================================================*/
    #ifndef LSAS_EDDI_I2C_SCL_IN_OUT
    LSA_VOID LSAS_EDDI_I2C_SCL_IN_OUT( LSA_BOOL          const  OutPut,
                                       LSA_UINT32        const  sys_ptr )
    {
        LSA_UNUSED_ARG( sys_ptr );
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200)
        Bsp_PofSclDir (lsas_eddi_i2c_port_sel.port_id, !OutPut);
#endif
    }
    #endif

    /*=============================================================================
     * function name:  EDDI_I2C_SELECT()
     *
     * function:       Selects a port of the I2C device for the following I2C accesses
     *
     * parameters:     ...
     *
     * return value:   LSA_VOID
     *===========================================================================*/

    #ifndef LSAS_EDDI_I2C_SELECT
    LSA_VOID LSAS_EDDI_I2C_SELECT( LSA_BOOL            *  const  ret_val_ptr,
                                   LSA_UINT32             const  sys_ptr,
                                   LSA_UINT16             const  PortId,
                                   LSA_UINT16             const  I2CMuxSelect)
    {
        *ret_val_ptr = LSA_TRUE;

        LSA_UNUSED_ARG( sys_ptr );
        LSA_UNUSED_ARG( I2CMuxSelect );

        switch (PortId)
        {
            case 1:
            {
                lsas_eddi_i2c_port_sel.port_id = 1;
                break;
            }
            case 2:
            {
                lsas_eddi_i2c_port_sel.port_id = 2;
                break;
            }
            default:
            {
                *ret_val_ptr = LSA_FALSE;

                break;
            }
        }
    }
    #endif


    /*=============================================================================
     * function name:  EDDI_LL_LED_BLINK_BEGIN
     *
     * function:       The EDD calls this function within the service
     *                   EDDI_SRV_LED_BLINK() before the LED(s) start to blink.
     *                 Can be used e.g. to disable the link/activity LED function of
     *                   a PHY if necessary.
     *
     * parameters:     hDDB:        handle to EDD
     *                   HwPortIndex: Hardware Port Index
     *
     * return value:   LSA_VOID
     *===========================================================================*/

    LSA_VOID  LSAS_EDDI_LL_LED_BLINK_BEGIN( EDDI_SYS_HANDLE             hSysDev,
                                            EDDI_HANDLE                 hDDB,
                                            LSA_UINT32                  HwPortIndex
                                            )
    {
        LSA_UINT32      frequency   = 1;

        #if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB400)
            LSAS_SET_PARAM_LED_BLINK(hSysDev, hDDB, HwPortIndex);
        #endif

        if ( !(PNIO_OK == PNIO_cbf_start_led_blink (hSysDev, HwPortIndex, frequency)) )
        {
            LSAS_ASSERT(0);
        }
    }

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB400)
    /*=============================================================================
     * function name:  LSAS_SET_PARAM_LED_BLINK
     *
     * function:       Stores the EDDI parameters.
     *
     * parameters:     hSysDev:     device handle
     *                 hDDB:        handle to EDD
     *                 HwPortIndex: Hardware Port Index
     *
     * return value:   LSA_VOID
     *===========================================================================*/
    LSA_VOID LSAS_SET_PARAM_LED_BLINK( EDDI_SYS_HANDLE             hSysDev,
                                       EDDI_HANDLE                 hDDB,
                                       LSA_UINT32                  HwPortIndex)

    {
        lsas_eb400_led_param.hSysDev        = hSysDev;
        lsas_eb400_led_param.hDDB           = hDDB;
        lsas_eb400_led_param.HwPortIndex    = HwPortIndex;
    }

    /*=============================================================================
     * function name:  LSAS_GET_PARAM_LED_BLINK
     *
     * function:       Gets the EDDI parameters.
     *
     * parameters:     lsas_eb400_led_param_bsp:     EDDI parameters
     *
     * return value:   LSA_VOID
     *===========================================================================*/
    LSA_VOID LSAS_GET_PARAM_LED_BLINK( LSAS_E400_LED_PARAM *     lsas_eb400_led_param_bsp)

    {
        *lsas_eb400_led_param_bsp =   lsas_eb400_led_param;
    }

#endif



    /*=============================================================================
     * function name:  EDDI_LL_LED_BLINK_END
     *
     * function:       The EDD calls this function within the service
     *                   EDDI_SRV_LED_BLINK() after blinking of LED(s) has finished.
     *                 Can be used e.g. to re-enable the link/activity LED function of
     *                   a PHY if necessary.
     *
     * parameters:     hDDB:        handle to EDD
     *                   HwPortIndex: Hardware Port Index
     *
     * return value:   LSA_VOID
     *===========================================================================*/
    LSA_VOID  LSAS_EDDI_LL_LED_BLINK_END( EDDI_SYS_HANDLE                 hSysDev,
                                          EDDI_HANDLE                     hDDB,
                                          LSA_UINT32                      HwPortIndex
                                          )
    {

        #if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB400)
            LSAS_SET_PARAM_LED_BLINK(hSysDev, hDDB, HwPortIndex);
        #endif

        if ( !(PNIO_OK == PNIO_cbf_stop_led_blink (hSysDev, HwPortIndex)) )
         {
             LSAS_ASSERT(0);
         }
    }


    /*=============================================================================
     * function name:  EDDI_LL_LED_BLINK_SET_MODE
     *
     * function:       The EDD calls this function repeatedly within the service
     *                   EDDI_SRV_LED_BLINK() in order to turn on and
     *                 turn off the LED(s) alternately.
     *
     * parameters:     hDDB:        handle to EDD
     *                   HwPortIndex  Hardware port index
     *                   LEDMode        EDDI_LED_MODE_ON / EDDI_LED_MODE_OFF
     *
     * return value:   LSA_VOID
     *===========================================================================*/
    LSA_VOID LSAS_EDDI_LL_LED_BLINK_SET_MODE( EDDI_SYS_HANDLE                   hSysDev,
                                              EDDI_HANDLE                       hDDB,
                                              LSA_UINT32                        HwPortIndex,
                                              LSA_UINT16                        LEDMode
                                              )
    {
        switch (HwPortIndex)
        {
            case 0:
            {
                if (EDDI_LED_MODE_ON == LEDMode)
                {
                    Bsp_EbSetLed (PNIO_LED_LINKP1,  1);
                }
                else
                {
                    Bsp_EbSetLed (PNIO_LED_LINKP1, 0);
                }
                break;
            }
            case 1:
            {

                if (EDDI_LED_MODE_ON == LEDMode)
                {
                    Bsp_EbSetLed (PNIO_LED_LINKP2,  1);
                }
                else
                {
                    Bsp_EbSetLed (PNIO_LED_LINKP2,  0);
                }

                break;
            }
            case 2:
            {

                if (EDDI_LED_MODE_ON == LEDMode)
                {
                    Bsp_EbSetLed (PNIO_LED_LINKP3,  1);
                }
                else
                {
                    Bsp_EbSetLed (PNIO_LED_LINKP3,  0);
                }

                break;
            }
            case 3:
            {

                if (EDDI_LED_MODE_ON == LEDMode)
                {
                    Bsp_EbSetLed (PNIO_LED_LINKP4,  1);
                }
                else
                {
                    Bsp_EbSetLed (PNIO_LED_LINKP4,  0);
                }

                break;
            }
            default:
            {
                /* Error, only 4 Ports available */
                LSAS_FATAL(HwPortIndex);//EDDI_Excp("Exception", EDDI_FATAL_ERR_EXCP, HwPortIndex, 0);
                break;
            }
        }
    }

    #endif /* LSAS_CFG_USE_EDDI */


	#ifdef  LSAS_CFG_USE_EDDS
		LSA_VOID  LSAS_POF_FO_LED( LSA_UINT16        const PortID,
								   LSA_UINT8          const ON,
								   LSA_SYS_PTR_TYPE  sys_ptr)
		{
			LSA_UNUSED_ARG( sys_ptr );

			switch (PortID)
			{
				case 1:
				{
					if ( ON)
					{
						PNIO_printf ("Dummy:  LSAS_POF1_LED_ON\n");
					}
					else
					{
						PNIO_printf ("Dummy:  LSAS_POF1_LED_OFF\n");
					}

					break;
				}
				case 2:
				{
					if ( ON)
					{
						PNIO_printf ("Dummy:  LSAS_POF2_LED_ON\n");
					}
					else
					{
						PNIO_printf ("Dummy:  LSAS_POF2_LED_OFF\n");
					}

					break;
				}
				default:
				{

					break;
				}
			}
		}
	#else
    LSA_VOID  LSAS_POF_FO_LED( LSA_UINT16        const PortID,
                               LSA_UINT8          const ON,
                               LSA_SYS_PTR_TYPE  sys_ptr)
    {
        LSA_UNUSED_ARG( sys_ptr );

        switch (PortID)
        {
            case 1:
            {
                Bsp_EbSetLed (PNIO_LED_FO1, ON);
                break;
            }
            case 2:
            {
                Bsp_EbSetLed (PNIO_LED_FO2, ON);
                break;
            }
            default:
            {
                break;
            }
        }
    }
#endif


    LSA_VOID LSAS_GET_UTC100NS_CLKSEQ(  LSA_UINT32 * utc_low_ptr,
                                        LSA_UINT32 * utc_high_ptr,
                                        LSA_UINT16 * clkseq_ptr
                                      )
    {
        static LSA_UINT32 Old_Time_100nsec = 0;
        static LSA_UINT32 Old_Time_Diff = 0;

        LSA_UINT32 Time_100nsec;        // time value in 100 nsec steps

        Time_100nsec = OsGetTime_us () * 10;

        /*** check if values are unequal *****/
        if (Old_Time_100nsec == Time_100nsec)
        {
            Old_Time_Diff++;
        }
        else
        {
            Old_Time_Diff = 0;
            Old_Time_100nsec = Time_100nsec;
        }



        *utc_low_ptr  = (LSA_UINT32)Time_100nsec + Old_Time_Diff;
    //    *utc_high_ptr = (LSA_UINT32)(Time_100nsec >> 32); // $$$ will be 0 because 32 bit instead of 64 bit
        *utc_high_ptr = 0;

        *clkseq_ptr = 0;
    }

    #ifdef LSAS_CFG_USE_EDDP
    LSA_VOID EDD_UPPER_OUT_FCT_ATTR lsas_eddp_req_open_lower_done(EDD_UPPER_RQB_PTR_TYPE rqb_ptr)
    {

    }
    #endif



    /**
     * @brief Sends new address data to pn-stack (OHA)
     *
     * @param[in]		*pStationData	pointer to data, which should be changed
     * @param[in]		param_to_change	which of parameters should be changed
     *
     * @return			void			no return value
     *
     */
    LSA_VOID LSAS_SET_ADDRESS_TO_OHA( LSA_UINT32 param_to_change, void* pStationData )
	{
    	OHA_UPPER_RQB_PTR_TYPE new_rqb_ptr;

    	LSAS_ALLOC_COMP_MEM( &new_rqb_ptr, sizeof(OHA_RQB_TYPE), LSA_COMP_ID_LSAS);
        if( new_rqb_ptr == LSA_NULL )
        {
           LSAS_FATAL(0);
        }
        new_rqb_ptr->args.Address.AccessMode = OHA_READ_WRITE;
        new_rqb_ptr->args.Address.Options = param_to_change;
        new_rqb_ptr->args.Address.RemaOptions = param_to_change;
        new_rqb_ptr->args.Address.pStationData = ( OHA_UPPER_STATION_PTR_TYPE )pStationData;

		LSA_RQB_SET_HANDLE(new_rqb_ptr, lsas_handle_oha_sys);
		LSA_RQB_SET_OPCODE(new_rqb_ptr, OHA_OPC_SET_ADDRESS);

		LSAS_RQB_OHA_REQUEST(new_rqb_ptr, oha_request);

	}

#ifdef LSAS_CFG_USE_EDDP

    LSA_VOID LSAS_CFG_SYSTEM_RAM_INIT(LSA_VOID)
    {
        LSA_UINT32 i;
        /* reset PNIP ================================================*/
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P)
        *((LSA_UINT32*)U_SCRB__ASYN_RES_CTRL_REG) |= U_SCRB__ASYN_RES_CTRL_REG__RES_SOFT_PN;
#endif
        /* reset PNIP-memories =======================================*/

        // reset StatisticRam
        for (i = 0; i < PNIP_RAM__SIZE_STATISTIC; i+=4)
        {
            WRITE_ULONG(   (U_PNIP__BASE + PNIP_RAM__OFFSET_STATISTIC + i), 0x00000000);
        }

        // reset SyncRam
        for (i = 0; i < PNIP_RAM__SIZE_SYNC; i+=4)
        {
            WRITE_ULONG(   (U_PNIP__BASE + PNIP_RAM__OFFSET_SYNC + i), 0x00000000);
        }
        /*  reset is done by hardware
        // reset PackCtrlRam
        for (i = 0; i < PNIP_RAM__SIZE_PACK_CTRL; i+=4)
        {
            WRITE_ULONG(   (U_PNIP__BASE + PNIP_RAM__OFFSET_PACK_CTRL + i), 0x00000000);
        }

        // reset PackDataRam
        for (i = 0; i < PNIP_RAM__SIZE_PACK_DATA; i+=4)
        {
            WRITE_ULONG(   (U_PNIP__BASE + PNIP_RAM__OFFSET_PACK_DATA + i), 0x00000000);
        }
        */
        // reset CmdRam
        for (i = 0; i < PNIP_RAM__SIZE_CMD; i+=4)
        {
            WRITE_ULONG(   (U_PNIP__BASE + PNIP_RAM__OFFSET_CMD + i), 0x00000000);
        }

        // reset ApiCtrlRam
        for (i = 0; i < PNIP_RAM__SIZE_API_CTRL; i+=4)
        {
            WRITE_ULONG(   (U_PNIP__BASE + PNIP_RAM__OFFSET_API_CTRL + i), 0x00000000);
        }
    }

#endif

    #define LSAS_CFG_REMA_DATA_SIZE 0x1000

    #ifndef LSAS_HANDLE_OHA_REMA_REQUESTS
    /**
         * @brief Sends new address data to pn-stack (OHA)
         *
         * @param[in]		rqb_ptr			pointer to rqb with parameters
         *
         * @return			void			no return value
         *
         */
    void LSAS_HANDLE_OHA_REMA_REQUESTS(OHA_UPPER_RQB_PTR_TYPE rqb_ptr)
    {

        LSA_UINT16  Response;

    #if 1
        if ( rqb_ptr->response != OHA_OK )
        {
            LSAS_FATAL(rqb_ptr->response);
        }

        if (OHA_OPC_OPEN_CHANNEL==LSA_RQB_GET_OPCODE(rqb_ptr))
        {
            OHA_UPPER_RQB_PTR_TYPE new_rqb_ptr;
            Unsigned32 * data_ptr;
            /* channel is open now, give indication resource to oha */

            /* at this place rqb_ptr is the rqb recource for all channel open requests and
               must not be used here for new requests.
               Allocate a new resource (new_rqb_ptr) for further use */
            LSAS_ALLOC_COMP_MEM( &new_rqb_ptr, sizeof(OHA_RQB_TYPE), LSA_COMP_ID_LSAS);

            if( new_rqb_ptr == LSA_NULL )
            {
               LSAS_FATAL(0);
            }

            /* alloc data buffer for rqb */
            LSAS_ALLOC_COMP_MEM( &data_ptr, LSAS_CFG_REMA_DATA_SIZE, LSA_COMP_ID_LSAS);

            if( data_ptr == LSA_NULL )
            {
                LSAS_FATAL(0);
            }



            //apma_data.remanence.rema_handle = rqb_ptr->args.channel.handle;

    //        new_rqb_ptr->args.RemaData.index = 0;
    //        new_rqb_ptr->args.RemaData.data_tag = 0;
            new_rqb_ptr->args.RemaData.data = (Unsigned8 *)data_ptr;
            new_rqb_ptr->args.RemaData.data_length = LSAS_CFG_REMA_DATA_SIZE;
            new_rqb_ptr->args.RemaData.data_size = LSAS_CFG_REMA_DATA_SIZE;

            /* remember handle, so it might be used later */
            lsas_handle_oha_sys = rqb_ptr->args.channel.handle;

            LSA_RQB_SET_HANDLE(new_rqb_ptr, rqb_ptr->args.channel.handle);
            LSA_RQB_SET_OPCODE(new_rqb_ptr, OHA_OPC_REMA_INDICATION);

            LSAS_RQB_OHA_REQUEST(new_rqb_ptr, oha_request);

        }
        else
        {
            /* at this place rqb_ptr is the rqb recource for all common requests
               It was allocated above as new_rqb_ptr and is never released. */

            switch (LSA_RQB_GET_OPCODE(rqb_ptr))
            {
                case OHA_OPC_REMA_INDICATION:
                {
                    /* handle remanence indication */

                    if ( OHA_OK == LSA_RQB_GET_RESPONSE(rqb_ptr) )
                    {
                        switch (rqb_ptr->args.RemaData.index)
                        {
                            case OHA_RECORD_INDEX_MULTIPLE_PDEV:
                            {
                                OHA_UPPER_MEM_U8_PTR_TYPE  pPDevData;
                                LSA_UINT32                 PDevLen;
                                LSA_UINT32                 new_projecting_is_fsu=FALSE;
                                LSA_UINT32				   err_offset;

                                LSA_UINT32  FSHelloMode     = 0;
                                LSA_UINT32  FSHelloInterval = 0;
                                LSA_UINT32  FSHelloRetry    = 0;
                                LSA_UINT32  FSHelloDelay    = 0;

                                /* neue PDEV-Daten */
                            	Response = OHA_Get_PrmPDEVData( rqb_ptr->args.RemaData.data, rqb_ptr->args.RemaData.data_length,
                                                                OHA_RECORD_INDEX_FAST_STARTUP, &pPDevData, &PDevLen, &err_offset);

                                if ( (OHA_OK == Response) && ( pPDevData != NULL ) )
                                {

                                    Response = OHA_Get_FastStartUpData(pPDevData, PDevLen,
                                                                       &FSHelloMode, &FSHelloInterval,
                                                                       &FSHelloRetry, &FSHelloDelay, &err_offset);

                                    if (OHA_OK == Response)
                                    {
                                        if( (FSHelloMode == OHA_DCP_HELLO_ON) || (FSHelloMode == OHA_DCP_HELLO_ON_DELAY) )
                                        {
                                            /* we have a FSU project */
                                            new_projecting_is_fsu = TRUE;
                                        }
                                    }
                                }

                                /* Vergleich neue PDEV-Daten mit geflashten und ggf. schreiben */
                                if (rqb_ptr->args.RemaData.data_length)
                                {
                                    LSAS_CFG_REMA_WRITE_PDEV_DATA(rqb_ptr->args.RemaData.data, rqb_ptr->args.RemaData.data_length);

                                    if( new_projecting_is_fsu == TRUE )
                                    {
                                        LSAS_CFG_REMA_INDICATE_HELLO_MODE(LSA_TRUE);
                                    }
                                    else
                                    {
                                        LSAS_CFG_REMA_INDICATE_HELLO_MODE(LSA_FALSE);
                                    }
                                }
                                else
                                {
                                    /* if an empty buffer is indicated, a reset to factory is in progress       */
                                    /* Bumples RTF needs this file to be deleted here*/
                                    LSAS_CFG_REMA_DELETE_PDEV_DATA();
                                }

                                break;
                            }

                            case OHA_RECORD_INDEX_MULTIPLE_STATION:
                            {
                                /* this is done on a different way
                                 * see OHA_DB_WRITE_STATION_NAME and OHA_DB_WRITE_STATION_NAME
                                 */
                                break;
                            }
                            case OHA_RECORD_INDEX_MULTIPLE_SNMP:
                            {

                                /* Vergleich neue Daten mit geflashten und ggf. schreiben */
                                if (rqb_ptr->args.RemaData.data_length)
                                {
                                    LSAS_CFG_REMA_WRITE_SNMP_DATA(rqb_ptr->args.RemaData.data, rqb_ptr->args.RemaData.data_length);

                                }
                                else
                                {
                                    /* if an empty buffer is indicated, a reset to factory is in progress       */
                                    /* Bumples RTF needs this file to be deleted here*/
                                    LSAS_CFG_REMA_DELETE_SNMP_DATA();
                                }


                                break;
                            }

                            case OHA_RECORD_INDEX_IP_SUITE:
                            {
                                /* Vergleich neue Daten mit geflashten und ggf. schreiben */
                                if (rqb_ptr->args.RemaData.data_length)
                                {
                                    LSAS_CFG_REMA_WRITE_IP_DATA(rqb_ptr->args.RemaData.data, rqb_ptr->args.RemaData.data_length);

                                }
                                else
                                {
                                    /* if an empty buffer is indicated, a reset to factory is in progress       */
                                    /* Bumples RTF needs this file to be deleted here*/
                                    LSAS_CFG_REMA_DELETE_IP_DATA();
                                }

                            	break;

                            }
                            case OHA_RECORD_INDEX_NAME_OF_STATION:
                            {
                                /* Vergleich neue Daten mit geflashten und ggf. schreiben */
                                if (rqb_ptr->args.RemaData.data_length)
                                {
                                    LSAS_CFG_REMA_WRITE_NOS_DATA(rqb_ptr->args.RemaData.data, rqb_ptr->args.RemaData.data_length);

                                }
                                else
                                {
                                    /* if an empty buffer is indicated, a reset to factory is in progress       */
                                    /* Bumples RTF needs this file to be deleted here*/
                                    LSAS_CFG_REMA_DELETE_NOS_DATA();
                                }
                            	break;
                            }

                            default:
                            {
                                LSAS_FATAL( 0);

                                break;
                            }
                        }

                        /* Ressourcen mittels Request wieder zu OHA */
                        rqb_ptr->args.RemaData.data_length  = LSAS_CFG_REMA_DATA_SIZE;
                        rqb_ptr->args.RemaData.data_size  = LSAS_CFG_REMA_DATA_SIZE;

                        //rqb is reused, handle is still valid
                        //LSA_RQB_SET_HANDLE(rqb_ptr, apma_data.remanence.rema_handle);
                        LSA_RQB_SET_OPCODE(rqb_ptr, OHA_OPC_REMA_INDICATION);

                        LSAS_RQB_OHA_REQUEST(rqb_ptr, oha_request);

                    }
                    else
                    {
                        LSAS_FATAL( 0);
                    }

                    break;
                }

                default:
                {
                    /* un-known/wanted opcode */
                    LSAS_FATAL(rqb_ptr->response);
                    break;
                }

            }
        }
    #else
        if ( rqb_ptr->response != OHA_OK )
        {
            LSAS_FATAL(rqb_ptr->response);
        }

        if (OHA_OPC_OPEN_CHANNEL==LSA_RQB_GET_OPCODE(rqb_ptr))
        {
            OHA_UPPER_RQB_PTR_TYPE new_rqb_ptr;
            Unsigned32 * data_ptr;
            /* channel is open now, give indication resource to oha */

            /* at this place rqb_ptr is the rqb recource for all channel open requests and
               must not be used here for new requests.
               Allocate a new resource (new_rqb_ptr) for further use */
            LSAS_ALLOC_COMP_MEM( &new_rqb_ptr, sizeof(OHA_RQB_TYPE), LSA_COMP_ID_LSAS);

            if( new_rqb_ptr == LSA_NULL )
            {
               LSAS_FATAL(0);
            }

            /* alloc data buffer for rqb */
            LSAS_ALLOC_COMP_MEM( &data_ptr, 1000, LSA_COMP_ID_LSAS);

            if( data_ptr == LSA_NULL )
            {
               LSAS_FATAL(0);
            }



            //apma_data.remanence.rema_handle = rqb_ptr->args.channel.handle;

    //        new_rqb_ptr->args.RemaData.index = 0;
    //        new_rqb_ptr->args.RemaData.data_tag = 0;
            new_rqb_ptr->args.RemaData.data = (Unsigned8 *)data_ptr;
            new_rqb_ptr->args.RemaData.data_length = 300;
            new_rqb_ptr->args.RemaData.data_size = 300;

            LSA_RQB_SET_HANDLE(new_rqb_ptr, rqb_ptr->args.channel.handle);
            LSA_RQB_SET_OPCODE(new_rqb_ptr, OHA_OPC_REMA_INDICATION);

            LSAS_RQB_OHA_REQUEST(new_rqb_ptr, oha_request);

        }
        else
        {
            /* Ressourcen mittels Request wieder zu OHA */
            rqb_ptr->args.RemaData.data_length  = 1000;
            rqb_ptr->args.RemaData.data_size  = 1000;
            //LSA_RQB_SET_HANDLE(rqb_ptr, );
            LSA_RQB_SET_OPCODE(rqb_ptr, OHA_OPC_REMA_INDICATION);

            LSAS_RQB_OHA_REQUEST(rqb_ptr, oha_request);
        }
    #endif
    }
    #endif
#endif

/* end of file *************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
