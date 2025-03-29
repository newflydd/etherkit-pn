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
/*  F i l e               &F: lsas_cfg.h                                :F&  */
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
 * @file    lsas_cfg.h
 * @brief   brief description
 * @author  cn3dit09
 * @version
 * @date    24.02.2012
 *
 * long description
 *
 */

/* 2017-01-23      fix06.6                                                   */
/*****************************************************************************/

#ifndef LSAS_CFG_H_
#define LSAS_CFG_H_

#include "pniousrd.h"

/**
 *  @brief Enable the use of lsa eddi component
 *
 */

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200_400)  /* for EDDI use this */
    #define LSAS_CFG_USE_EDDI
	#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200)
		#define LSAS_CFG_EDDI_ERTEC200
	#else
		#define LSAS_CFG_EDDI_ERTEC400
	#endif

#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P)   /* for EDDP use this */
    #define LSAS_CFG_USE_EDDP
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_DKSW)     /* for EDDS use this */
    #define LSAS_CFG_USE_EDDS
	/* a timer-overload should result in a unexpected-error-message.  */
	#define LSAS_CFG_EDDS_CSRT_OVERLOAD_TRACE_LEVEL		LSA_TRACE_LEVEL_OFF // LSA_TRACE_LEVEL_UNEXP
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_RZT2_EDDP)     /* for EDDS use this */
	#define LSAS_CFG_USE_EDDP
	#define LSAS_CFG_USE_EDDR      // AG   Added for EDDR(enesas)

#include "pnip_reg.h"
#else
    #error ("no valid platform selected")
#endif

#if ((defined (PNIOD_PLATFORM)) && (PNIOD_PLATFORM & PNIOD_PLATFORM_WIN))
	#define LSAS_CFG_EDDS_ON_WINDOWS
#endif

#ifdef LSAS_CFG_USE_EDDP
#include "edd_cfg.h"
#include "edd_usr.h"
#include "eddp_cfg.h"
#include "eddp_usr.h"
#endif
/**
 *  @brief Enable the use of lsa pof component
 *
 */
//#define LSAS_CFG_USE_POF  // EDDS: Integration!

/**
 *  @brief Enable S2 system redundancy in whole project - all other S2 switches are dependant on this
 *
 */
#if( IOD_INCLUDE_S2_REDUNDANCY )
#define ENABLE_S2_REDUNDANCY
#endif

#if( IOD_INCLUDE_DR)
#define ENABLE_DR
#endif

/**
 *  @brief Enable the use of fs component
 *
 */
//#define LSAS_CFG_USE_FS
#include "xx_mem.h"
#include "pnpb_nvdata.h"
/*****************************************************************************
 *
 *      process image related defines
 *
 *****************************************************************************/

#ifdef LSAS_CFG_USE_EDDI
    #ifdef LSAS_CFG_EDDI_ERTEC200
        #define LSAS_IOCR_IN_MAX_LENGTH              256        // max. number of bytes of an iocr
        #define LSAS_IOCR_IN_MAX_BUFLEN              784        // (IOCR_IN_MAX_BUFLEN = 3*(IOCR_IN_MAX_LENGTH + 4) + Fillbytes(Align16)
        #define LSAS_IOCR_OUT_MAX_LENGTH             256        // max. number of bytes of an iocr
        #define LSAS_IOCR_OUT_MAX_BUFLEN             272        // (IOCR_OUT_MAX_BUFLEN = 1*(IOCR_OUT_MAX_LENGTH + 4) + Fillbytes(Align16)

    #else //LSAS_CFG_EDDI_ERTEC400
        #define LSAS_IOCR_IN_MAX_LENGTH              1440        // max. number of bytes of an iocr
        #define LSAS_IOCR_IN_MAX_BUFLEN              1456        // (IOCR_IN_MAX_BUFLEN = (IOCR_IN_MAX_LENGTH + 4) + Fillbytes(Align16)
        #define LSAS_IOCR_OUT_MAX_LENGTH             1440        // max. number of bytes of an iocr
        #define LSAS_IOCR_OUT_MAX_BUFLEN             1456        // (IOCR_OUT_MAX_BUFLEN = (IOCR_OUT_MAX_LENGTH + 4) + Fillbytes(Align16)
    #endif
    #define LSAS_PROCESS_INPUT_SIZE         LSAS_IOCR_IN_MAX_LENGTH     
    #define LSAS_PROCESS_OUTPUT_SIZE        LSAS_IOCR_OUT_MAX_BUFLEN	/* include APDU and alignment */
    #define LSAS_PROCESS_IMAGE_SIZE         (LSAS_IOCR_IN_MAX_BUFLEN + LSAS_IOCR_OUT_MAX_BUFLEN)

    /* @brief Absolute adress of the IRTE*/
    #define LSAS_CFG_IRTE_START 0x10000000
    /* @brief Absolut adresw of SDRAM Base */
    #define LSAS_CFG_SDRAM_BASE_ADDR 0x20000000
    /* @brief EDD Interface ID */
    #define LSAS_CFG_INTERFACE_ID EDD_INTERFACE_ID_MIN /* 0x1 */

    #define LSAS_EDD_NRT_RCV_MAX_FRAME_TO_LOOK_UP_B0 10
    #define LSAS_EDD_NRT_RCV_MAX_FRAME_SEND_TO_USER_B0 10
#endif
#define LSAS_SIGNAL_TIME_SYNC_STATE_CHANGE()	/*dummy to be able to compile new LSAS*/
#ifdef LSAS_CFG_USE_EDDP
    #define LSAS_CFG_IF_CNT 1

    #define LSAS_CFG_INTERFACE_ID EDD_INTERFACE_ID_MIN /* 0x1 */

#if( 1 == IOD_INCLUDE_MRPD )
    #define LSAS_PU_FEATURE_MRPD_SUPPORT
#endif

    /* Based on definitions IEC 61158 IRT ForwardingMode, followings are mandatory 
     * used in parameterization of PDIRData record ;otherwise RTC3 is not enabled  
     */
    #define LSAS_PU_FEATURE_ABSOLUTE_FWD_SUPPORTED
    #define LSAS_PU_FEATURE_RELATIVE_FWD_SUPPORTED

#if( 1 == IOD_CFG_PU_FEATURES_SUPPORT )
    #define LSAS_PU_FEATURE_SHORTPREAMBLE_SUPPORT
    #define LSAS_PU_FEATURE_FRAGTYPE_DYNAMIC_SUPPORTED
    #define LSAS_PU_FEATURE_DFP_SUPPORTED
#endif

#endif

#ifdef LSAS_CFG_USE_EDDS
    #define LSAS_CFG_IF_CNT 1

	#ifndef IOD_INCLUDE_MRP
		#error "IOD_INCLUDE_MRP is undefined"
    #endif

    #if (IOD_CFG_PDEV_NUMOF_PORTS > 1)
        #define LLIF_CFG_SWITCH_SUPPORT
    #endif

#endif

#if (IOD_INCLUDE_MRP == 1)
    #define LSAS_CFG_USE_MRP    1
#else
    #define LSAS_CFG_USE_MRP    0
#endif

/** @brief I2C configuration parameters
 *
 *  Defines the I2C hardware type to the POF transceivers.
 */
#ifdef LSAS_CFG_USE_EDDI
	#if ( IOD_INCLUDE_POF == 1 )
		#define LSAS_EDDI_I2C_TYPE 	EDDI_I2C_TYPE_GPIO
	#endif
#endif

/** @brief MDIX Setting for all odd ports
 *
 *  Defines the configuration for the media dependent interface crossing.
 *
 *  if LSA_TRUE Port is a crossed Port (MDI-X)
 *  if LSA_FALSE Port is not crossed (MDI)
 *
 *  Only applicable if Auto-MDI(X) is disabled, ignored otherwise.
 */
#if ( IOD_INCLUDE_POF == 1 )
	#define LSAS_CFG_EDD_ODD_PORT_MDIX LSA_FALSE
#else
	#define LSAS_CFG_EDD_ODD_PORT_MDIX LSA_FALSE
#endif
/** @brief MDIX Setting for all even ports
 *
 *  Defines the configuration for the media dependent interface crossing.
 *
 *  if LSA_TRUE Port is a crossed Port (MDI-X)
 *  if LSA_FALSE Port is not crossed (MDI)
 *
 *  Only applicable if Auto-MDI(X) is disabled, ignored otherwise.
 */
#if ( IOD_INCLUDE_POF == 1 )
	#define LSAS_CFG_EDD_EVN_PORT_MDIX LSA_FALSE
#else
	#define LSAS_CFG_EDD_EVN_PORT_MDIX LSA_TRUE
#endif

#define LSAS_MAX_ARS_RTC1_RTC2  IOD_CONSUMER_C1_FRAME_ID_COUNT
#define LSAS_MAX_ARS_RTC3       IOD_CONSUMER_C3_FRAME_ID_COUNT

/*****************************************************************************
 *
 *      fs path related config
 *
 *****************************************************************************/

#ifdef LSAS_CFG_USE_FS

#define LSAS_FS_PARTITION_LOC_DATA          ((Unsigned8)0x00)
#define LSAS_FS_PARTITION_FWUP              ((Unsigned8)0x01)

#define LSAS_FS_LOC_DATA_ACTIVE_BLOCK()  (IM_FLASH_LOC_DATA_PART1_OFFSET | IM_FLASH_BASE_ADDRESS)
#define LSAS_FS_LOC_DATA_PASSIVE_BLOCK() (IM_FLASH_LOC_DATA_PART2_OFFSET | IM_FLASH_BASE_ADDRESS)
#define LSAS_FS_LOC_DATA_LENGTH()        IM_FLASH_LOC_DATA_PART_LENGTH

extern BOOT_STRUC_STATIC_DATA boot_static_data;

#define LSAS_FS_FWUP_ACTIVE_BLOCK()         boot_static_data.fs_firmware_block_ptr_used
#define LSAS_FS_FWUP_PASSIVE_BLOCK()        boot_static_data.fs_firmware_block_ptr_unused
#define LSAS_FS_FWUP_LENGTH()               IM_FLASH_FW_DATA_PART_LENGTH

#endif

/*****************************************************************************
 *
 *      other path related config
 *
 *****************************************************************************/

#define LSAS_PDB_MAX_RPC_DATA_LEN   IOD_CFG_MAX_RECORD_LENGTH /*8192    TCPIP frame size ; lt. LSA_CM_Speicherbedarf.xls 4652 Byte for connect.req, records */
                                                              /* 8k max pdev-record size, IRT top with 64 devices */


#define LSAS_CFG_TCIP_PATH_TYPE_TCP_RECV_RES_COUNT 10 /* if not defined receiving tcip frames is turned off */
/*===========================================================================*/
/*                              basic attributes                             */
/*===========================================================================*/

#define LSAS_FCTATTR_FLAT  /* nothing */
#define LSAS_MEMATTR_FLAT  /* nothing */


/*===========================================================================*/
/*                         code- and data-attributes                         */
/*===========================================================================*/

/*=====  code attributes  =====*/

#define LSAS_UPPER_IN_FCT_ATTR      LSAS_FCTATTR_FLAT   /* rema-user to rema */
#define LSAS_UPPER_OUT_FCT_ATTR     LSAS_FCTATTR_FLAT   /* rema to rema-user */
#define LSAS_SYSTEM_IN_FCT_ATTR     LSAS_FCTATTR_FLAT   /* system to rema    */
#define LSAS_SYSTEM_OUT_FCT_ATTR    LSAS_FCTATTR_FLAT   /* mac_rw to system  */
#define LSAS_LOCAL_FCT_ATTR         LSAS_FCTATTR_FLAT   /* local function    */
#define LSAS_EDD_LOWER_OUT_FCT_ATTR
#define LSAS_CM_LOWER_OUT_FCT_ATTR
#define LSAS_ACP_LOWER_OUT_FCT_ATTR
#define LSAS_TCIP_LOWER_OUT_FCT_ATTR
#define LSAS_SOCK_LOWER_OUT_FCT_ATTR
#define LSAS_MRP_LOWER_OUT_FCT_ATTR
#define LSAS_OHA_LOWER_OUT_FCT_ATTR
#define LSAS_CLRPC_LOWER_OUT_FCT_ATTR
#define LSAS_NARE_LOWER_OUT_FCT_ATTR
#define LSAS_GSY_LOWER_OUT_FCT_ATTR
#define LSAS_DCP_LOWER_OUT_FCT_ATTR


/*=====  data attributes  =====*/

#define LSAS_UPPER_RQB_ATTR         LSAS_MEMATTR_FLAT   /* upper-rqb data    */
#define LSAS_UPPER_MEM_ATTR         LSAS_MEMATTR_FLAT   /* upper-memory data */
#define LSAS_SYSTEM_MEM_ATTR        LSAS_MEMATTR_FLAT   /* system-memory data*/
#define LSAS_LOCAL_MEM_ATTR         LSAS_MEMATTR_FLAT   /* local data        */
#define LSAS_COMMON_MEM_ATTR        LSAS_MEMATTR_FLAT   /* greatest common
                                                              data attribute */
#define LSAS_LOCAL_MEM_PTR_TYPE LSA_VOID LSAS_MEMATTR_FLAT *

    /* RQB handling */

#define LSAS_RQB_HEADER   LSA_RQB_HEADER(LSAS_RQB_PTR_TYPE)
#define LSAS_RQB_TRAILER  LSA_RQB_TRAILER

#define LSAS_RQB_GET_OPCODE(_RQB_PTR)             LSA_RQB_GET_OPCODE(_RQB_PTR)
#define LSAS_RQB_SET_OPCODE(_RQB_PTR, _VALUE)     LSA_RQB_SET_OPCODE(_RQB_PTR,_VALUE)

#define LSAS_RQB_GET_RESPONSE(_RQB_PTR)           LSA_RQB_GET_RESPONSE(_RQB_PTR)
#define LSAS_RQB_SET_RESPONSE(_RQB_PTR, _VALUE)   LSA_RQB_SET_RESPONSE(_RQB_PTR, _VALUE)

#define LSAS_RQB_GET_HANDLE(_RQB_PTR)             LSA_RQB_GET_HANDLE(_RQB_PTR)
#define LSAS_RQB_SET_HANDLE(_RQB_PTR, _VALUE)     LSA_RQB_SET_HANDLE(_RQB_PTR, _VALUE)


#define LSAS_REQUEST_LOCAL(rqb_ptr)\
{\
    LSA_UINT32 own_thread_id = OsGetThreadId ();\
    IoxRequestSync( own_thread_id,     /* task id of self*/\
                     lsas_request,     /* function to call*/\
                    (void*) rqb_ptr,   /* RQB pointer*/\
                    (void (*)(void*))_iox_dummy_fct_done);\
}


#define LSAS_ACP_RQB_TYPE       ACP_RQB_TYPE
#define LSAS_CLRPC_RQB_TYPE     CLRPC_RQB_TYPE
#define LSAS_CM_RQB_TYPE        CM_RQB_TYPE
#define LSAS_DCP_RQB_TYPE       DCP_RQB_TYPE
#define LSAS_EDD_RQB_TYPE       EDD_RQB_TYPE
#define LSAS_GSY_RQB_TYPE       GSY_RQB_TYPE
#define LSAS_LLDP_RQB_TYPE      LLDP_RQB_TYPE
#define LSAS_MRP_RQB_TYPE       MRP_RQB_TYPE
#define LSAS_NARE_RQB_TYPE      NARE_RQB_TYPE
#define LSAS_OHA_RQB_TYPE       OHA_RQB_TYPE
#define LSAS_POF_RQB_TYPE       POF_RQB_TYPE
#define LSAS_SOCK_RQB_TYPE      SOCK_RQB_TYPE
#define LSAS_TCIP_RQB_TYPE      TCIP_RQB_TYPE


/*===========================================================================*/
/*                                 macros                                    */
/*===========================================================================*/

#if defined LSAS_CFG_USE_EDDI
	#define LSAS_SET_SENDCLOCK_REDUCTION(_CycleBaseFactor)		\
	{													        \
	}
#elif defined LSAS_CFG_USE_EDDP
	#define LSAS_SET_SENDCLOCK_REDUCTION(_CycleBaseFactor)		\
	{													        \
	}
#endif


#define IM_HW_FLASH_32BIT

#ifndef IM_HW_FLASH_32BIT
/* @brief Put Hardware related pll settings here */
#define LSAS_CFG_OUT_CHANGE_PLL_SETTINGS()                  \
{                                                           \
                                                            \
    /* correction of GPIO_PORT_MODE_H GPIO24   */           \
    /* -> function XPLL_SetPLLMode_OUT in edd_sys.c set GPIO24 to alternate mode 2 - XPLL_EXT_IN  */ \
    /* -> for controlling ON_LED by GPIO24 set GPIO24 to output again, as EDD has set it to input (AP00439633) */ \
                                                            \
    (*ERTEC200_GPIO).gpio_port_mode_h.bits.gpio_24 = 0;     \
    hama_port_set_direction(0x00, HAMA_PORT_ID_A_24_31 | HAMA_GPIO_LED_PWR);     \
}
#else
 #define LSAS_CFG_OUT_CHANGE_PLL_SETTINGS()
#endif

#define SET_THREAD_PRIO_TIMER_VALUE             400

/* @brief install the irte/pnip isr here */
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200_400)
#define LSAS_INIT_ISR(_INT_NR)                                  \
{                                                               \
    Bsp_ErtecSwiIntConnect  (tskma_isr_00, tskma_isr_01);       \
    Bsp_ErtecTim1IntConnect (Bsp_Timer1Int);                    \
    Bsp_ErtecTim1Start      (SET_THREAD_PRIO_TIMER_VALUE);      \
}
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P)
#define LSAS_INIT_ISR(_INT_NR)                                  \
{                                                               \
    Bsp_ErtecSwiIntConnect  (tskma_isr_00, tskma_isr_01);       \
    Bsp_ErtecErrIntConnect  (tskma_parity_error_isr, tskma_access_error_isr);     \
}
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_RZT2_EDDP)
#define LSAS_INIT_ISR(_INT_NR)                                  \
{                                                               \
    Bsp_SwiIntConnect  ((PNIO_VOID*)tskma_isr_00, (PNIO_VOID*)tskma_isr_01);       \
    Bsp_ErrIntConnect  ((PNIO_VOID*)tskma_parity_error_isr, (PNIO_VOID*)tskma_access_error_isr);     \
}
#endif


#define LSAS_CFG_PORT_CNT		IOD_CFG_PDEV_NUMOF_PORTS


/* @brief   the macro returns true if the port is of the fiber optic type.
 *
 * @param   hwport_id   refers to the hardware port (point of view of the Profinet ASIC)
 *                      and is 1-based (not 0-based)
 */

#if ( IOD_INCLUDE_POF == 1 )
	#define LSAS_PORT_IS_FO(_HWPORT_ID)         (_HWPORT_ID == _HWPORT_ID) /*TRUE*/
    #define LSAS_PORT_IS_POF(_HWPORT_ID)        (_HWPORT_ID == _HWPORT_ID) /*TRUE*/
#else
	#define LSAS_PORT_IS_FO(_HWPORT_ID)         (_HWPORT_ID != _HWPORT_ID) /*FALSE*/
    #define LSAS_PORT_IS_POF(_HWPORT_ID)        (_HWPORT_ID != _HWPORT_ID) /*FALSE*/
#endif


#define LSAS_SET_IPSUITE(_interface_id, _ip_addr, _net_mask, _gateway ) 	\
{																			\
	Bsp_SetIpSuite(_interface_id, _ip_addr, _net_mask, _gateway);			\
}

/* @brief implement a function that returns the MAC-Adress of the interface
 * to _DST_MAC_ADDR_PTR (don't copy more than 6 byte!)
 */
#define LSAS_CFG_GET_MAC_ADDR(_DST_MAC_ADDR_PTR)            \
{                                                           \
    Bsp_GetMacAddr((PNIO_UINT8*)_DST_MAC_ADDR_PTR);                      \
}

/* @brief implement a function that returns the MAC-Adress of port with number _PORT_NR
 * to _MAC_ADDR_DST_PTR (don't copy more than 6 byte!)
 */
#define LSAS_CFG_GET_PORT_MAC_ADDR(_PORT_NR,_MAC_ADDR_DST_PTR)      \
{                                                                   \
    Bsp_GetPortMacAddr(_MAC_ADDR_DST_PTR, _PORT_NR + 1);\
}

/* @brief initialze the process image memory management here */
#ifdef LSAS_CFG_USE_EDDI
	#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200)
		#define LSAS_CFG_INIT_COM_RAM()                                     	    \
		{                                                                       	\
			{                                                                       \
			iom_init_struct_t init_data;                                    		\
			init_data.unkramed_buffer_address = &lsas_process_shadow_buffer[0];     \
			init_data.unkramed_buffer_size    = sizeof(lsas_process_shadow_buffer); \
			init_data.properties              = 0;                                  \
			iom_init(&init_data);                                                   \
			}                                                                       \
		}
	#endif
	#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB400)
		#define LSAS_CFG_INIT_COM_RAM()                                     	    \
		{                                                                       	\
			{                                                                       \
			iom_init_struct_t init_data;                                    		\
			init_data.unkramed_buffer_address = &lsas_process_shadow_buffer[0];     \
			init_data.unkramed_buffer_size    = sizeof(lsas_process_shadow_buffer); \
			init_data.properties              = IOM_KRAM_PROP_USE_CONS_EX_BUFFERS;  \
			iom_init(&init_data);                                                   \
			}                                                                       \
		}
	#endif
#endif

#ifdef LSAS_CFG_USE_EDDS
#define LSAS_CFG_INIT_COM_RAM()                                     			    		\
{                                                                       					\
	{                                                    				                   	\
    iom_init_struct_t init_data;                                    						\
    init_data.h_io_buffer_management = (LSA_UINT32) lsas_data.edds.h_io_buffer_management;	\
    iom_init(&init_data);                                                   				\
	}                                                                       				\
}
#endif

#ifdef LSAS_CFG_USE_EDDP

typedef EDDP_TIMESTAMP_INFO_PTR_TYPE LSAS_PTCP_TIMESTAMP_TYPE;

#define INIT_MEMORY_FOR_EDDP
#ifdef INIT_MEMORY_FOR_EDDP
#define LSAS_CFG_INIT_COM_RAM()                                        	 									\
{     																										\
    iom_init_struct_t init_data;                                    										\
    init_data.gsharedmem_address = (EDD_COMMON_MEM_PTR_TYPE) &lsas_data.eddp.gshared_mem;								\
    init_data.pnip_base_address = (EDD_COMMON_MEM_PTR_TYPE) lsas_data.eddp.eddp_param_ptr->dpb_pparam.pDPB->pPnipBase;	\
    iom_init(&init_data);                                                 									\
    evma_init();                                                        									\
}
#else
#define LSAS_CFG_INIT_COM_RAM()                                        	 									\
{     																										\
    iom_init(LSA_NULL);                                                 									\
    evma_init();                                                        									\
}
#endif	/*INIT_MEMORY_FOR_EDDP*/

#endif

#ifndef LSAS_CFG_INIT_COM_RAM
	#error  LSAS_CFG_INIT_COM_RAM undefined !
#endif

#ifdef LSAS_CFG_USE_EDDP
#ifdef ENABLE_S2_REDUNDANCY

    #define  LSAS_CFG_GET_PRIMARY_AR_SESSION_KEY(_SRSHM_INDEX, _SRSHM_VALUE_PTR) 	\
	{																					\
		if( (_SRSHM_INDEX) != 0 )															\
		{																				\
			CM_FATAL(); /* currently only 1 AR-set */									\
		}																				\
																						\
	   *(_SRSHM_VALUE_PTR) = iom_provider_get_primary_arid(_SRSHM_INDEX);			\
	}
#else
    #define  LSAS_CFG_GET_PRIMARY_AR_SESSION_KEY(_AR_SET_NR, _AR_SESSION_KEY_RET_PTR)
#endif
#endif

#ifdef LSAS_CFG_USE_EDDI
#ifdef ENABLE_S2_REDUNDANCY
	#define  LSAS_CFG_GET_PRIMARY_AR_SESSION_KEY(_AR_SET_NR, _AR_SESSION_KEY_RET_PTR) 	\
	{																					\
		if( (_AR_SET_NR) != 1 )															\
		{																				\
			CM_FATAL(); /* currently only 1 AR-set */									\
		}																				\
																						\
	   *(_AR_SESSION_KEY_RET_PTR) = iom_provider_get_primary_arid(_AR_SET_NR);			\
	}
#else
	#define  LSAS_CFG_GET_PRIMARY_AR_SESSION_KEY(_AR_SET_NR, _AR_SESSION_KEY_RET_PTR)
#endif
#endif

#ifdef LSAS_CFG_USE_EDDS
	#define  LSAS_CFG_GET_PRIMARY_AR_SESSION_KEY(_AR_SET_NR, _AR_SESSION_KEY_RET_PTR)
#endif


#define LSAS_CFG_INIT_DONE_IND(_RQB_PTR)\
{\
    LSAS_RQB_STARTUP_REQUEST(&lsas_data.tskma_rqb, &tskma_task_stp_perform_cold_start_sm);\
}

#define LSAS_CFG_FS_OPEN_DONE()\
{\
    LSAS_RQB_STARTUP_REQUEST(&lsas_data.tskma_rqb, &tskma_task_stp_perform_cold_start_sm);\
}

#define LSAS_CFG_EDD_OPEN_DONE()\
{\
    LSAS_RQB_STARTUP_REQUEST(&lsas_data.tskma_rqb, &tskma_task_stp_perform_cold_start_sm);\
}


#define LSAS_CFG_PNIO_OPEN_DONE()\
{\
    LSAS_RQB_STARTUP_REQUEST(&lsas_data.tskma_rqb, &tskma_task_stp_perform_cold_start_sm);\
}


#define LSAS_RQB_STARTUP_REQUEST(_RQB_PTR, _METHODE_PTR)\
{\
    LSA_RQB_SET_METHOD_PTR(_RQB_PTR, _METHODE_PTR);\
    OsSendMessage  ( \
                        TSKMA_TASK_ID_STARTUP,         /* id of the  task (references the task message queue) */\
                        _RQB_PTR,                     /* parameter 2 (request block)*/ \
                        OS_MBX_PRIO_NORM);      /* message priority */ \
    /*AM_QUEUE_SEND(APMA_ID_PNIO_TASK, _RQB_PTR);*/ \
}

#define LSAS_RQB_LSAS_REQUEST(_RQB_PTR, _METHODE_PTR)\
{\
    LSA_RQB_SET_METHOD_PTR(_RQB_PTR, _METHODE_PTR);\
    OsSendMessage  ( \
                        TSKMA_TASK_ID_STARTUP,         /* id of the  task (references the task message queue) */\
                        _RQB_PTR,                     /* parameter 2 (request block)*/ \
                        OS_MBX_PRIO_NORM);      /* message priority */ \
    /*AM_QUEUE_SEND(APMA_ID_PNIO_TASK, _RQB_PTR);*/ \
}

#define LSAS_RQB_ACP_REQUEST(_RQB_PTR, _METHODE_PTR)\
{\
    LSA_RQB_SET_METHOD_PTR(_RQB_PTR, _METHODE_PTR);\
    OsSendMessage  ( \
                        TSKMA_TASK_ID_PNIO,             /* id of the  task (references the task message queue) */\
                        _RQB_PTR,                     /* parameter 2 (request block)*/ \
                        OS_MBX_PRIO_NORM);      /* message priority */ \
    /*AM_QUEUE_SEND(APMA_ID_PNIO_TASK, _RQB_PTR);*/ \
}

#define LSAS_RQB_CLRPC_REQUEST(_RQB_PTR, _METHODE_PTR)\
{\
    LSA_RQB_SET_METHOD_PTR(_RQB_PTR, _METHODE_PTR);\
    OsSendMessage  ( \
                        TSKMA_TASK_ID_PNIO,             /* id of the  task (references the task message queue) */\
                        _RQB_PTR,                     /* parameter 2 (request block)*/ \
                        OS_MBX_PRIO_NORM);      /* message priority */ \
    /* AM_QUEUE_SEND(APMA_ID_PNIO_TASK, _RQB_PTR);*/\
}

#define LSAS_RQB_CM_REQUEST(_RQB_PTR, _METHODE_PTR)\
{\
    LSA_RQB_SET_METHOD_PTR(_RQB_PTR, _METHODE_PTR);\
    OsSendMessage  ( \
                        TSKMA_TASK_ID_PNIO,             /* id of the  task (references the task message queue) */\
                        _RQB_PTR,                     /* parameter 2 (request block)*/ \
                        OS_MBX_PRIO_NORM);      /* message priority */ \
    /*AM_QUEUE_SEND(APMA_ID_PNIO_TASK, _RQB_PTR);*/\
}

#define LSAS_RQB_DCP_REQUEST(_RQB_PTR, _METHODE_PTR)\
{\
    LSA_RQB_SET_METHOD_PTR(_RQB_PTR, _METHODE_PTR);\
    OsSendMessage  ( \
                        TSKMA_TASK_ID_PNIO,             /* id of the  task (references the task message queue) */\
                        _RQB_PTR,                     /* parameter 2 (request block)*/ \
                        OS_MBX_PRIO_NORM);      /* message priority */ \
    /*AM_QUEUE_SEND(APMA_ID_PNIO_TASK, _RQB_PTR);*/\
}

#define LSAS_RQB_EDD_REQUEST(_RQB_PTR, _METHODE_PTR)\
{\
    LSA_RQB_SET_METHOD_PTR(_RQB_PTR, _METHODE_PTR);\
    OsSendMessage  ( \
                        TSKMA_TASK_ID_EDD,             /* id of the  task (references the task message queue) */\
                        _RQB_PTR,                     /* parameter 2 (request block)*/ \
                        OS_MBX_PRIO_NORM);      /* message priority */ \
    /*AM_QUEUE_SEND(APMA_ID_EDD_TASK, _RQB_PTR);*/\
}

#define LSAS_RQB_EDD_REQUEST_HIGH(_RQB_PTR, _METHODE_PTR)\
{\
    LSA_RQB_SET_METHOD_PTR(_RQB_PTR, _METHODE_PTR);\
    OsSendMessage  ( \
                        TSKMA_TASK_ID_EDD,             /* id of the  task (references the task message queue) */\
                        _RQB_PTR,                     /* parameter 2 (request block)*/ \
                        OS_MBX_PRIO_NORM);      /* message priority */ \
    /*AM_QUEUE_SEND_HIGH(APMA_ID_EDD_TASK, _RQB_PTR);*/\
}

#define LSAS_RQB_GSY_REQUEST(_RQB_PTR, _METHODE_PTR)\
{\
    LSA_RQB_SET_METHOD_PTR(_RQB_PTR, _METHODE_PTR);\
    OsSendMessage  ( \
                        TSKMA_TASK_ID_ORG,             /* id of the  task (references the task message queue) */\
                        _RQB_PTR,                     /* parameter 2 (request block)*/ \
                        OS_MBX_PRIO_NORM);      /* message priority */ \
    /*AM_QUEUE_SEND(APMA_ID_ORG_TASK, _RQB_PTR);*/\
}

#define LSAS_RQB_LLDP_REQUEST(_RQB_PTR, _METHODE_PTR)\
{\
    LSA_RQB_SET_METHOD_PTR(_RQB_PTR, _METHODE_PTR);\
    OsSendMessage  ( \
                        TSKMA_TASK_ID_ORG,             /* id of the  task (references the task message queue) */\
                        _RQB_PTR,                     /* parameter 2 (request block)*/ \
                        OS_MBX_PRIO_NORM);      /* message priority */ \
    /*AM_QUEUE_SEND(APMA_ID_ORG_TASK, _RQB_PTR);*/\
}

#define LSAS_RQB_MRP_REQUEST(_RQB_PTR, _METHODE_PTR)\
{\
    LSA_RQB_SET_METHOD_PTR(_RQB_PTR, _METHODE_PTR);\
    OsSendMessage  ( \
                        TSKMA_TASK_ID_ORG,             /* id of the  task (references the task message queue) */\
                        _RQB_PTR,                     /* parameter 2 (request block)*/ \
                        OS_MBX_PRIO_NORM);      /* message priority */ \
    /*AM_QUEUE_SEND(APMA_ID_ORG_TASK, _RQB_PTR);*/\
}

#define LSAS_RQB_NARE_REQUEST(_RQB_PTR, _METHODE_PTR)\
{\
    LSA_RQB_SET_METHOD_PTR(_RQB_PTR, _METHODE_PTR);\
    OsSendMessage  ( \
                        TSKMA_TASK_ID_PNIO,             /* id of the  task (references the task message queue) */\
                        _RQB_PTR,                     /* parameter 2 (request block)*/ \
                        OS_MBX_PRIO_NORM);      /* message priority */ \
    /*AM_QUEUE_SEND(APMA_ID_PNIO_TASK, _RQB_PTR);*/\
}

#define LSAS_RQB_OHA_REQUEST(_RQB_PTR, _METHODE_PTR)\
{\
    LSA_RQB_SET_METHOD_PTR(_RQB_PTR, _METHODE_PTR);\
    OsSendMessage  ( \
                        TSKMA_TASK_ID_PNIO,             /* id of the  task (references the task message queue) */\
                        _RQB_PTR,                     /* parameter 2 (request block)*/ \
                        OS_MBX_PRIO_NORM);      /* message priority */ \
    /*AM_QUEUE_SEND(APMA_ID_PNIO_TASK, _RQB_PTR);*/\
}

#define LSAS_RQB_POF_REQUEST(_RQB_PTR, _METHODE_PTR)\
{\
    LSA_RQB_SET_METHOD_PTR(_RQB_PTR, _METHODE_PTR);\
    OsSendMessage  ( \
                        TSKMA_TASK_ID_POF,             /* id of the  task (references the task message queue) */\
                        _RQB_PTR,                     /* parameter 2 (request block)*/ \
                        OS_MBX_PRIO_NORM);      /* message priority */ \
    /*AM_QUEUE_SEND(APMA_ID_POF_TASK, _RQB_PTR);*/\
}

#define LSAS_RQB_SOCK_REQUEST(_RQB_PTR, _METHODE_PTR)\
{\
    LSA_RQB_SET_METHOD_PTR(_RQB_PTR, _METHODE_PTR);\
    OsSendMessage  ( \
                        TSKMA_TASK_ID_TCPIP,             /* id of the  task (references the task message queue) */\
                        _RQB_PTR,                     /* parameter 2 (request block)*/ \
                        OS_MBX_PRIO_NORM);      /* message priority */ \
    /*AM_QUEUE_SEND(APMA_ID_TCPIP_TASK, _RQB_PTR);*/\
}

#define LSAS_RQB_SOCK_REQUEST_HIGH(_RQB_PTR, _METHODE_PTR)\
{\
    LSA_RQB_SET_METHOD_PTR(_RQB_PTR, _METHODE_PTR);\
    OsSendMessage  ( \
                        TSKMA_TASK_ID_TCPIP,             /* id of the  task (references the task message queue) */\
                        _RQB_PTR,                     /* parameter 2 (request block)*/ \
                        OS_MBX_PRIO_NORM);      /* message priority */ \
    /*AM_QUEUE_SEND_HIGH(APMA_ID_TCPIP_TASK, _RQB_PTR);*/\
}

#define LSAS_RQB_TCPIP_REQUEST(_RQB_PTR, _METHODE_PTR)\
{\
    LSA_RQB_SET_METHOD_PTR(_RQB_PTR, _METHODE_PTR);\
    OsSendMessage  ( \
                        TSKMA_TASK_ID_TCPIP,             /* id of the  task (references the task message queue) */\
                        _RQB_PTR,                     /* parameter 2 (request block)*/ \
                        OS_MBX_PRIO_NORM);      /* message priority */ \
    /*AM_QUEUE_SEND(APMA_ID_TCPIP_TASK, _RQB_PTR);*/\
}

#define LSAS_RQB_APPL_REQUEST(_RQB_PTR, _METHODE_PTR)\
{\
    LSA_RQB_SET_METHOD_PTR(_RQB_PTR, _METHODE_PTR);\
    OsSendMessage  ( \
                        TSKMA_TASK_ID_APPLICATION,             /* id of the  task (references the task message queue) */\
                        _RQB_PTR,                     /* parameter 2 (request block)*/ \
                        OS_MBX_PRIO_NORM);      /* message priority */ \
    /*AM_QUEUE_SEND(APMA_ID_APPLICATION_TASK, _RQB_PTR);*/\
}

#define LSAS_RQB_PNDV_REQUEST(_RQB_PTR, _METHODE_PTR)\
{\
    LSA_RQB_SET_METHOD_PTR(_RQB_PTR, _METHODE_PTR);\
    OsSendMessage  ( \
                        TSKMA_TASK_ID_APPLICATION,             /* id of the  task (references the task message queue) */\
                        _RQB_PTR,                     /* parameter 2 (request block)*/ \
                        OS_MBX_PRIO_NORM);      /* message priority */ \
    /*AM_QUEUE_SEND(APMA_ID_APPLICATION_TASK, _RQB_PTR);*/\
}

    /* @brief   the macro is called for FO ports and it has to return the transceiver type.
     *          for example: - EDD_FX_TRANSCEIVER_QFBR5978 (old POF transceivers)
     *                       - EDD_FX_TRANSCEIVER_QFBR5978_2 (new POF transceivers (10/2017))
     *                       - EDD_FX_TRANSCEIVER_AFBR59E4APZ (LC)
     * @param   hwport_id   refers to the hardware port (point of view of the Profinet ASIC)
     *                      and is 1-based (not 0-based)
     */
#define LSAS_PORT_GET_FO_TRANSCEIVER_TYPE(_HW_PORT_ID)  (LSAS_PORT_IS_POF(_HW_PORT_ID) ? EDD_FX_TRANSCEIVER_QFBR5978 : EDD_FX_TRANSCEIVER_AFBR59E4APZ)

/**
 *  @brief Lock edd com
 *
 *  Provides access to lock edd com sema
 *
 */
#define LSAS_OS_SEM_TAKE_EDD_LOCK_COM()\
{\
    OsTakeSemB(TSKMA_SEM_RES_EDD_COM_ACCESS);\
    /*am_Sem_Take(AM_SEMA_ACCESS id_edd_lock_com, WAIT_FOREVER);*/\
}

/**
 *  @brief Unlock edd com
 *
 *  Provides access to unlock edd com sema
 *
 */
#define LSAS_OS_SEM_GIVE_EDD_LOCK_COM()\
{\
    OsGiveSemB(TSKMA_SEM_RES_EDD_COM_ACCESS);\
    /*am_Sem_Give(AM_SEMA_ACCESS id_edd_lock_com);*/\
}


/**
 *  @brief lock edd io
 *
 *
 *
 */
#define LSAS_OS_SEM_TAKE_EDD_LOCK_IO()\
{\
    /* no semaphore needed and functions are called within interrupt */ \
    /*am_Sem_Take(AM_SEMA_ACCESS id_edd_lock_io, WAIT_FOREVER); */ \
    OsIntDisable();\
   /* hama_lock(HAMA_SEMA_EDD);  anhe temporarily until pnio base has improved datastate handling */\
}

/**
 *  @brief unlock edd io
 *
 *
 */
#define LSAS_OS_SEM_GIVE_EDD_LOCK_IO()\
{\
    /* no semaphore needed and functions are called within interrupt */ \
    /* am_Sem_Give(AM_SEMA_ACCESS id_edd_lock_io); */ \
    OsIntEnable();\
    /* hama_unlock(HAMA_SEMA_EDD); anhe temporarily until pnio base has improved datastate handling */\
}
/**
 *  @brief lock edd i2c
 *
 *
 *
 */
#define LSAS_OS_SEM_TAKE_EDD_LOCK_I2C()

/**
 *  @brief unlock edd i2c
 *
 *
 */
#define LSAS_OS_SEM_GIVE_EDD_LOCK_I2C()

#define LSAS_EDDP_I2C_SELECT(_RET_VAL_PTR, _H_SYS_DEV, _PORT_ID, _I2C_MUX_SELECT )

/**
 *  @brief Check and lock task context
 *
 *  This macro is called within several enter macros.
 *  Here you can check if it is called in the context of the
 *  PNIO_TASK or use it to take a semaphore (should not be needed if everything
 *  is implemented the right way). Best ist to use it as an assert only during
 *  development.
 */
#define LSAS_OS_LOCK_ON_PNIO_TASK()\
{\
	if ( !TSKMA_LOCK_CHECK_PND )	\
    {								\
        TSKMA_LOCK_SET_PND(1);		\
    }								\
    else							\
    {								\
        LSAS_ASSERT(0);				\
    }								\
}

/**
 *  @brief Release task context
 *
 *  This macro is called within several exit macros.
 *  Here you can e.g. release a previously taken semaphore.
 */
#define LSAS_OS_RELEASE_PNIO_TASK()	\
{									\
    TSKMA_LOCK_SET_PND(0);			\
}

/**
 *  @brief Check and lock task context
 *
 *  This macro is called within several enter macros.
 *  Here you can check if it is called in the context of the
 *  POF_TASK or use it to take a semaphore (should not be needed if everything
 *  is implemented the right way). Best ist to use it as an assert only during
 *  development.
 */
#define LSAS_OS_LOCK_ON_POF_TASK()\
{\
    if ( !TSKMA_LOCK_CHECK_POF )\
    {\
    	TSKMA_LOCK_SET_POF(1);\
    }\
    else	\
    {\
        LSAS_ASSERT(0);\
    }\
}

/**
 *  @brief Release task context
 *
 *  This macro is called within several exit macros.
 *  Here you can e.g. release a previously taken semaphore.
 */
#define LSAS_OS_RELEASE_POF_TASK()\
{\
    TSKMA_LOCK_SET_POF(0);\
}

/**
 *  @brief Check and lock task context
 *
 *  This macro is called within several enter macros.
 *  Here you can check if it is called in the context of the
 *  GSY_TASK or use it to take a semaphore (should not be needed if everything
 *  is implemented the right way). Best ist to use it as an assert only during
 *  development.
 */
#define LSAS_GSY_ENTER()\
{\
    OsEnterX(OS_MUTEX_GSY);\
}


#define LSAS_GSY_EXIT()\
{\
    OsExitX(OS_MUTEX_GSY);\
}

/**
 *  @brief Check and lock task context
 *
 *  This macro is called within several enter macros.
 *  Here you can check if it is called in the context of the
 *  LLDP_TASK or use it to take a semaphore (should not be needed if everything
 *  is implemented the right way). Best ist to use it as an assert only during
 *  development.
 */
#define LSAS_OS_LOCK_ON_ORG_TASK()\
{\
    if ( !TSKMA_LOCK_CHECK_LLD )\
    {\
        TSKMA_LOCK_SET_LLD(1);\
    }\
    else\
    {\
        LSAS_ASSERT(0);\
    }   \
}


#define LSAS_OS_RELEASE_ORG_TASK()\
{\
    TSKMA_LOCK_SET_LLD(0);\
}

/**
 *  @brief Check and lock task context
 *
 *  This macro is called within several enter macros.
 *  Here you can check if it is called in the context of the
 *  POF_TASK or use it to take a semaphore (should not be needed if everything
 *  is implemented the right way). Best ist to use it as an assert only during
 *  development.
 */
#define LSAS_OS_LOCK_ON_TCPIP_TASK()\
{\
    if ( !TSKMA_LOCK_CHECK_TCP )\
    {\
        TSKMA_LOCK_SET_TCP(1);\
    }\
    else\
    {\
        LSAS_ASSERT(0);\
    }\
}

/**
 *  @brief Release task context
 *
 *  This macro is called within several exit macros.
 *  Here you can e.g. release a previously taken semaphore.
 */
#define LSAS_OS_RELEASE_TCPIP_TASK()\
{\
    TSKMA_LOCK_SET_TCP(0);\
}

#define LSAS_OHA_ENTER()\
{\
    if ( !TSKMA_LOCK_CHECK_PNO )\
    {\
        TSKMA_LOCK_SET_PNO(1);\
    }\
    else\
    {\
        LSAS_ASSERT( 0);\
    }\
}

#define LSAS_OHA_EXIT()\
{\
    TSKMA_LOCK_SET_PNO(0);\
}



#if 1  // fix06.6  2017-01-23
    #define LSAS_OS_LOCK_ON_TCPIP_RESOURCE(res) OsEnterX(OS_MUTEX_TCPIP_NET_RESID+res)
    #define LSAS_OS_RELEASE_TCPIP_RESOURCE(res) OsExitX(OS_MUTEX_TCPIP_NET_RESID+res)
#else
    #define LSAS_OS_LOCK_ON_TCPIP_RESOURCE(res)\
    {\
        if(    (TSKMA_TASK_ID_TCPIP != OsGetThreadId())\
            && (TSKMA_TASK_ID_PNIO != OsGetThreadId())\
          )\
        {\
            LSAS_ASSERT(0);\
        }\
    }

    #define LSAS_OS_RELEASE_TCPIP_RESOURCE(res)\
    {\
        if(    (TSKMA_TASK_ID_TCPIP != OsGetThreadId())\
            && (TSKMA_TASK_ID_PNIO != OsGetThreadId())\
           )\
        {\
            LSAS_ASSERT(0);\
        }\
    }
#endif

/* memory allocation */

/**
 *  @brief Allocate general memory
 *
 *  @param[out] _MEM_PTR_PTR pointer that gets the pointer to the allocated memory
 *  @param[in]  _LENGTH lenght of the memory that should be allocated
 *  @param[in]  _COMP_ID id of the component that allocates the memory
 *
 *  This macro should adapt to the system specific function that allocates memory
 *  for general purposes (RAM).
 *
 */
#define LSAS_ALLOC_COMP_MEM(_MEM_PTR_PTR, _LENGTH, _COMP_ID) \
{\
    OsAllocX((void **)_MEM_PTR_PTR,0,(LSA_UINT32)_LENGTH,MEMPOOL_DEFAULT); \
    /*LSAS_ALLOC_PACKET_MEM(_MEM_PTR_PTR, _LENGTH, _COMP_ID);*/\
}


/**
 *  @brief Free general memory
 *
 *  @param[out]  _RET_VAL_PTR pointer for returning the result
 *  @param[in]   _MEM_PTR pointer to the memory that should be freed
 *  @param[in]   _COMP_ID component that requests the free
 *
 *  This macro should adapt to the system specific function that frees memory
 *  for general purposes (RAM).
 *
 */
#define LSAS_FREE_COMP_MEM(_RET_VAL_PTR, _MEM_PTR, _COMP_ID) \
{\
   *(_RET_VAL_PTR) = (LSA_UINT16) OsFreeX(_MEM_PTR, MEMPOOL_DEFAULT);    \
   /* LSAS_FREE_PACKET_MEM(_RET_VAL_PTR, _MEM_PTR, _COMP_ID);*/\
}

/**
 *  @brief Allocate communication memory
 *
 *  @param[out] _MEM_PTR_PTR pointer that gets the pointer to the allocated memory
 *  @param[in]  _LENGTH lenght of the memory that should be allocated
 *  @param[in]  _COMP_ID id of the component that allocates the memory
 *
 *  This macro should adapt to the system specific function that allocates memory
 *  for receiving or sending packets (RX/TX Mem). In most common cases this can be the same
 *  memory like COMP_MEM.
 *
 */

extern LSA_UINT8* pRxTxMem;

#define LSAS_ALLOC_PACKET_MEM(_MEM_PTR_PTR, _LENGTH, _COMP_ID) \
{\
    /*OsAllocX((void**)_MEM_PTR_PTR,0,_LENGTH,MEMPOOL_DEFAULT);*//* pool may also bee MEMPOOL_RX_TX_BUF */ \
    XX_MEM_alloc((void**)_MEM_PTR_PTR, _LENGTH, pRxTxMem);\
}

 /**
  *  @brief Free communication memory
  *
  *  @param[out]  _RET_VAL_PTR pointer for returning the result
  *  @param[in]   _MEM_PTR pointer to the memory that should be freed
  *  @param[in]   _COMP_ID component that requests the free
  *
  *  This macro should adapt to the system specific function that frees memory
  *  for receiving or sending packets (RX/TX Mem).
  *
  */
#define LSAS_FREE_PACKET_MEM(_RET_VAL_PTR, _MEM_PTR, _COMP_ID) \
{\
    /**(_RET_VAL_PTR) = (LSA_UINT16) OsFreeX(_MEM_PTR, MEMPOOL_DEFAULT); */    \
    if (XX_MEM_free(_MEM_PTR))\
    {\
        *(_RET_VAL_PTR) = LSA_OK;\
    }\
    else\
    {\
        *(_RET_VAL_PTR) = LSA_NULL;\
    }\
}

/**
 *  @brief Allocate a timer resource
 *
 *  @param[out]  _ret_val_ptr   return value pointer
 *  @param[out]  _timer_id_ptr  pointer for returning the timer id
 *  @param[in]   _timer_type    timer type (cyclic or one shot)
 *  @param[in]   _time_base     time base (see LSA_TIME_xxx)
 *  @param[in]   _cbf           function to be called if timer runs out
 *  @param[in]   _comp_id       id of the component that requests the timer
 *
 *  This macro should adapt to the system specific function for allocating
 *  software timers.
 *
 */
#define LSAS_ALLOC_TIMER(_ret_val_ptr, _timer_id_ptr, _timer_type, _time_base, _cbf, _comp_id)	 \
{                                                                              					 \
    *(_ret_val_ptr) = (LSA_UINT16) OsAllocTimer ((PNIO_UINT16*)_timer_id_ptr, _timer_type, _time_base, (PNIO_VOID*)_cbf);	 \
}

/**
 *  @brief Starts a timer
 *
 *  @param[out]  _ret_val_ptr   return value pointer
 *  @param[in]   _timer_id      id of the timer
 *  @param[in]   _time_base     time base (see LSA_TIME_xxx)
 *  @param[in]   _cbf           function to be called if timer runs out
 *  @param[in]   _comp_id       id of the component that requested the timer
 *
 *  This macro should adapt to the system specific function that starts the
 *  previously allocated timer.
 *
 */
#define LSAS_START_TIMER(_ret_val_ptr, _timer_id, _user_id, _time, _comp_id)    		\
{                                                                               		\
    * (_ret_val_ptr) = (LSA_UINT16) OsStartTimer (_timer_id, _user_id.uvar32, _time);   \
}

/**
 *  @brief Stops a timer
 *
 *  @param[out]  _ret_val_ptr   return value pointer
 *  @param[in]   _timer_id      id of the timer
 *  @param[in]   _comp_id       id of the component that requested the timer
 *
 *  This macro should adapt to the system specific function that stops a
 *  previously started timer.
 *
 */
#define LSAS_STOP_TIMER(_ret_val_ptr, _timer_id, _comp_id)                      \
{                                                                               \
    * (_ret_val_ptr) = (LSA_UINT16) OsStopTimer (_timer_id);                    \
}

 /**
  *  @brief Resets a stopped timer
  *
  *  @param[out]  _ret_val_ptr   return value pointer
  *  @param[in]   _timer_id      id of the timer
  *  @param[in]   _comp_id       id of the component that requested the timer
  *
  *  This macro should adapt to the system specific function that resets a
  *  stopped timer.
  *
  */
#define LSAS_RESET_TIMER(_ret_val_ptr, _timer_id, _comp_id)                      \
{                                                                               \
    * (_ret_val_ptr) = (LSA_UINT16) OsResetTimer (_timer_id);                    \
}


/**
 *  @brief Free a timer
 *
 *  @param[out]  _ret_val_ptr   return value pointer
 *  @param[in]   _timer_id      id of the timer
 *  @param[in]   _comp_id       id of the component that requested the timer
 *
 *  This macro should adapt to the system specific function that frees
 *  a not longer needed timer
 *
 */
#define LSAS_FREE_TIMER(_ret_val_ptr, _timer_id, _comp_id)                      \
{                                                                               \
    * (_ret_val_ptr) = (LSA_UINT16) OsFreeTimer (_timer_id);                    \
}

/*****************************************************************************/

#define LSAS_SWAP_U16(short_ptr)                                                                 \
{                                                                                               \
    *short_ptr = ((*short_ptr & 0xFF00) >> 8)                                                   \
               | ((*short_ptr & 0x00FF) << 8);                                                  \
}

#define LSAS_SWAP_U32(long_ptr)                                                                  \
{                                                                                               \
    *long_ptr = ((*long_ptr & 0xFF000000) >> 24)                                                \
              | ((*long_ptr & 0x00FF0000) >>  8)                                                \
              | ((*long_ptr & 0x0000FF00) <<  8)                                                \
              | ((*long_ptr & 0x000000FF) << 24);                                               \
}

#define LSAS_MEMSET(dst,val,len)                                                                 \
{                                                                                               \
    OsMemSet((void*)(dst),val,len);                                                                        \
}

#define LSAS_MEMCPY(dst,src,len)                                                                 \
{                                                                                               \
    OsMemCpy((void*)(dst),(void*)(src),len);                                                                      \
}

#define LSAS_MEMCMP(_a,_b,_len)                                                                 \
(                                                                                               \
        OsMemCmp((void*)(_a), (void*)(_b), _len)                                                \
)

#define LSAS_MEMMOVE(_a,_b,_len)                                                                 \
(                                                                                               \
        OsMemMove((void*)(_a), (void*)(_b), _len)                                                \
)

#define LSAS_STRCPY(dst,src)                                                                     \
{                                                                                               \
    OsStrCpy((void*)(dst),(void*)(src));                                                                            \
}

#define LSAS_STRLEN(str)                                                                         \
(                                                                                               \
    OsStrLen ((void*)(str))                                                                                 \
)

#define LSAS_SPRINTF                                                                            \
(                                                                                               \
  PNIO_sprintf                                                                                  \
)                                                                                               \

#define LSAS_SNPRINTF                                                                            \
(                                                                                               \
  PNIO_snprintf                                                                                  \
)                                                                                               \

#define LSAS_SSCANF		ret;//

#define LSAS_LOG_10(val)                                                                        \
(                                                                                               \
  PNIO_log10(val)                                                                               \
)                                                                                               \

#define LSAS_POW(val_1, val_2)                                                                  \
(                                                                                               \
  PNIO_pow(val_1, val_2)                                                                        \
)                                                                                               \


#define LSAS_HTONS(host_short)                                                                   \
(                                                                                               \
    OsHtons(host_short)                                                                         \
)

#define LSAS_HTONL(host_long)                                                                    \
(                                                                                               \
    OsHtonl(host_long)                                                                         \
)

#define LSAS_NTOHS(network_short)                                                                \
(                                                                                               \
    OsNtohs(network_short)                                                                         \
)

#define LSAS_NTOHL(network_long)                                                                 \
(                                                                                               \
    OsNtohl(network_long)                                                                         \
)

#if 0  // check if needed
#ifndef max
    #define max(a,b)    (((a) > (b)) ? (a) : (b))
#else
	#undef max
	#define max(a,b)    (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
    #define min(a,b)    (((a) < (b)) ? (a) : (b))
#else
	#undef min
	#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif
#endif

extern void PNIO_Log  (LSA_UINT32   DevHndl,
                LSA_UINT32   ErrLevel,
                LSA_UINT32   PackId,
                LSA_UINT32   ModId,
                LSA_UINT32   LineNum);

extern LSA_VOID   PnpbLsasFatalError (LSA_FATAL_ERROR_TYPE* pLsaErr);

#define LSAS_FATAL_ERROR( _ERROR_PTR)                                                             \
{                                                                                                \
    /*LSA_TRACE_01(TRACE_SUBSYS_PNDV_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "LSAS fatal pErr 0x%x\n", _ERROR_PTR)*/ \
    PnpbLsasFatalError (_ERROR_PTR); \
}

#if 0
    #define LSAS_PUT8_HTON(base,offset,val)                                                       \
    {/* put and convert from host byte order to network byte order */                           \
        apma_put8_hton(base,offset,val);                                                        \
    }                                                                                           \

    #define LSAS_GET8_NTOH(base,offset)                                                           \
    (/* get and convert from network byte order to host byte order */                           \
        apma_get8_ntoh(base,offset)                                                             \
    )

    #define LSAS_PUT16_HTON(base,offset,val)                                                      \
    {/* put and convert from host byte order to network byte order */                           \
        apma_put16_hton(base,offset,val);                                                       \
    }                                                                                           \

    #define LSAS_GET16_NTOH(base,offset)                                                          \
    (/* get and convert from network byte order to host byte order */                           \
        apma_get16_ntoh(base,offset)                                                            \
    )

    #define LSAS_PUT32_HTON(base,offset,val)                                                      \
    {/* put and convert from host byte order to network byte order */                           \
        apma_put32_hton(base,offset,val);                                                       \
    }                                                                                           \

    #define LSAS_GET32_NTOH(base,offset)                                                          \
    (/* get and convert from network byte order to host byte order */                           \
        apma_get32_ntoh(base,offset)                                                            \
    )
#endif


#if (PNIOD_PLATFORM)    // any development kit based platform ?
    #define LSAS_TCPIP_ICMP_RESPONSE(buf_ptr, buf_length)
#else
    #define LSAS_TCPIP_ICMP_RESPONSE(buf_ptr, buf_length)   trace_response(buf_ptr, buf_length)
#endif

#define LSAS_START_SOCK_SELECT()

// EDD receive resource queueing
#define LSAS_EDD_RCV_OVERTAKE_LANE

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_DKSW)
	#ifdef LSAS_EDD_RCV_OVERTAKE_LANE
		#define LSAS_EDD_CHECK_RCV(_RQB_PTR, _METHODE_PTR) (tskma_task_pno_queue_nrt_resource((LSA_VOID*)_RQB_PTR, 0, (LSA_VOID*)_METHODE_PTR) == FALSE)
		#define LSAS_EDD_SERVE_NRT_RESOURCE_LIST(_RQB_PTR)								\
			LSA_RQB_SET_METHOD_PTR(_RQB_PTR, tskma_task_pno_serve_nrt_resource_list);	\
			TSKMA_OS_SEND_MAIL(TSKMA_TASK_NUM_EDD, _RQB_PTR);
	#else
		#define LSAS_EDD_CHECK_RCV(_RQB_PTR, _METHODE_PTR) 	(TRUE)
		#define LSAS_EDD_SERVE_NRT_RESOURCE_LIST(_RQB_PTR)	LSAS_RQB_EDD_REQUEST(_RQB_PTR, edds_system)
	#endif
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200_400)
	#ifdef LSAS_EDD_RCV_OVERTAKE_LANE
		#define LSAS_EDD_CHECK_RCV(_RQB_PTR, _METHODE_PTR) (tskma_task_pno_queue_nrt_resource((LSA_VOID*)_RQB_PTR, 0, (LSA_VOID*)_METHODE_PTR) == FALSE)
		#define LSAS_EDD_SERVE_NRT_RESOURCE_LIST(_RQB_PTR)								\
			LSA_RQB_SET_METHOD_PTR(_RQB_PTR, tskma_task_pno_serve_nrt_resource_list);	\
			TSKMA_OS_SEND_MAIL(TSKMA_TASK_NUM_EDD, _RQB_PTR);
	#else
		#define LSAS_EDD_CHECK_RCV(_RQB_PTR, _METHODE_PTR) 	(TRUE)
		#define LSAS_EDD_SERVE_NRT_RESOURCE_LIST(_RQB_PTR)	LSAS_RQB_EDD_REQUEST(_RQB_PTR, eddi_system)
	#endif
#else
	#define LSAS_EDD_CHECK_RCV(_RQB_PTR, _METHODE_PTR) 		(LSA_TRUE)
	#define LSAS_EDD_SERVE_NRT_RESOURCE_LIST()
#endif



#define LSAS_MEDIA_TYPE_FIBER_OPTIC        0x00
#define LSAS_MEDIA_TYPE_WIRED              0x01
#define LSAS_MEDIA_TYPE_WLAN               0x02
/*
 * @brief Provide the type of media
 */
#if ( IOD_INCLUDE_POF == 1 )
	#define LSAS_EDD_SYS_GET_MEDIA_TYPE() LSAS_MEDIA_TYPE_FIBER_OPTIC
#else
	#define LSAS_EDD_SYS_GET_MEDIA_TYPE() LSAS_MEDIA_TYPE_WIRED
#endif

#define LSAS_EDD_DO_EV_NRT_RCV_IF_A0(_RQB_PTR)		LSAS_EDD_SERVE_NRT_RESOURCE_LIST(_RQB_PTR)

#define LSAS_EDDI_INT_PRIO04_NRT_LOW(_RQB_PTR)		LSAS_EDD_SERVE_NRT_RESOURCE_LIST(_RQB_PTR)

#define LSAS_EDDI_DETECTED_RECEIVE_LIMIT_WARN_ONLY_NEW
#define LSAS_EDDI_DETECTED_RECEIVE_LIMIT(_HDDB, _HSYSDEV, _NRT_CHANNEL) \
{\
	PNIO_printf("WARNING: EDDI receive limit detected (hDDB=%p, hSysDev=%p, NRTChannel=%p)\n",   \
    		                               (void*)_HDDB, (void*)_HSYSDEV, (void*)_NRT_CHANNEL ); \
}


#define LSAS_EDD_PHY_LED_BLINK_BEGIN(_PORT_ID) PNIO_cbf_start_led_blink(PNIO_SINGLE_DEVICE_HNDL, (PNIO_UINT32)_PORT_ID, 1)
#define LSAS_EDD_PHY_LED_BLINK_END(_PORT_ID) PNIO_cbf_stop_led_blink(PNIO_SINGLE_DEVICE_HNDL, (PNIO_UINT32)_PORT_ID)
#define LSAS_EDD_PHY_LED_SET_MODE(_PORT_ID, _LED_MODE)

#define LSAS_EDDI_XPLL_SETPLLMODE_OUT(_RESULT, _p_IRTE, _LOCATION, _PLLMODE)\
{\
    lsas_set_pll_mode_out(_RESULT, _p_IRTE, _LOCATION, _PLLMODE);\
}

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_LINUX_PC)
// $$$BJ    assert leads to a FATAL_ERROR
#define LSAS_ASSERT(_cond) 	/* empty-block in release version */																		  \
{   																																	  \
    PNIO_printf ("%d caused synchronization issue %d in %s\n", OsGetThreadId(),  __LINE__, __FILE__);                                     \
}
#else
#define LSAS_ASSERT(_cond) 	/* empty-block in release version */
#endif

/**
 *  @brief Check for edd task
 *
 *  checks if called in context of the edd task
 *
 */
#define LSAS_ASSERT_EDD_CONTEXT()\
{\
    if( TSKMA_TASK_ID_EDD != OsGetThreadId() )\
    {\
        EDDI_Excp("Exception", EDDI_FATAL_ERR_EXCP, OsGetThreadId(), 0);\
    }\
}

#define LSAS_EDDI_ENTER_CRITICAL()\
{\
    OsIntDisable();\
    /*hama_lock(HAMA_SEMA_EDD);*/\
}

#define LSAS_EDDI_EXIT_CRITICAL()\
{\
    OsIntEnable();\
    /*hama_unlock(HAMA_SEMA_EDD);*/\
}

#define LSAS_EDDI_ENTER_SII_CRITICAL()\
{\
    OsIntDisable();\
    /*hama_lock(HAMA_SEMA_EDD);*/\
}

#define LSAS_EDDI_EXIT_SII_CRITICAL()\
{\
    OsIntEnable();\
    /*hama_unlock(HAMA_SEMA_EDD);*/\
}

#define LSAS_GET_MS_COUNT(_COUNT_VAL_PTR)\
{\
        *_COUNT_VAL_PTR = OsGetTime_ms();\
}

#define LSAS_OS_GET_UPTIME_MS(_VAL_PTR)\
{\
   *(_VAL_PTR) = OsGetTime_ms();\
}

#define LSAS_OS_GET_UNIX_TIME(_BOOT_TIME_PTR)\
{\
    *(_BOOT_TIME_PTR) = OsGetUnixTime();\
}


/**
 *  @brief common lock macro
 *
 *  Common lock macro to get interrupts disabled.
 *  No other checks.
 *
 */
#define LSAS_OS_ENTER_CRITICAL()\
{\
    OsIntDisable();\
}

/**
 *  @brief common unlock macro
 *
 *  Common lock macro to get interrupts enabled.
 *  No other checks.
 *
 */
#define LSAS_OS_EXIT_CRITICAL()\
{\
    OsIntEnable();\
}

/**
 *  @brief reset to factory indication
 *
 *
 *  This macro is called if the stack indicates a reset to factory
 *  settings. A user function should be called that resets all relevant
 *  data.
 *
 */
#define LSAS_RTF_IND()\
{\
    PnpbNvResetToFactorySettings(PNIO_RTF_RES_ALL); \
}


/**
 *  @brief reset communication parameter
 *
 *  This macro is called if the stack indicates a reset communication indication.
 *  A user function should be called that resets all relevant data.
 */
#define LSAS_RESET_COMM_IND()\
{\
    PnpbNvResetToFactorySettings(PNIO_RTF_RES_COMM_PAR); \
}


/**
 *  @brief indicates new ip settings for the device
 *
 *  @param[in]  _IP_ADDRESS IP address as LSA_UINT32
 *  @param[in]  _SUBNET_MASK subnet mask as LSA_UINT32
 *  @param[in]  _DEFAULT_GATEWAY default gateway as LSA_UINT32
 *  @param[in]  _REMANENT remantent flag as LSA_BOOL
 *
 *  This macro is used to indicate a change of the ip suit.
 *  If _REMANENT == LSA_TRUE the data should be safed to remanent memory and
 *  set with the next device start.
 *  If _REMANENT == LSA_FALSE the data should not be safed and even allready safed
 *  data must be deleted.
 *
 */
#define LSAS_IP_CHANGE_IND(_IP_ADDRESS, _SUBNET_MASK, _DEFAULT_GATEWAY, _REMANENT)\
{\
    PnpbNvSaveIpSuite (_IP_ADDRESS, _SUBNET_MASK, _DEFAULT_GATEWAY, _REMANENT); \
}

/**
 *  @brief delete the name of station
 *
 *  This macro is used to request a deletion of remanently safed name of station.
 *
 */
#define LSAS_DELETE_NOS()\
{\
    /*LSA_UINT8 *name_ptr=0;*/\
    /*LSA_UINT16 name_len=0;*/\
    PnpbNvSaveStationName (NULL, 0, PNIO_TRUE); \
}

/**
 *  @brief safe the name of station
 *
 *  @param[in]  _NAME_PTR pointer to the name of station as OHA_COMMON_MEM_U8_PTR_TYPE
 *  @param[in]  _NAME_LEN length of the name of station as LSA_INT
 *  @param[in]  _REMANENT remanent flag as LSA_BOOL
 *
 *  This macro is used to indicate a change of the name of station.
 *  If _REMANENT == LSA_TRUE the data should be safed to remanent memory and
 *  set with the next device start.
 *  If _REMANENT == LSA_FALSE the data should not be safed and even allready safed
 *  data must be deleted.
 *
 */
#define LSAS_NOS_CHANGE_IND(_NAME_PTR, _NAME_LEN, _REMANENT)\
{\
    PnpbNvSaveStationName ((PNIO_INT8*)_NAME_PTR, _NAME_LEN, _REMANENT); \
}


/**
 *  @brief provide remanent pdev data
 *
 *  @param[out] _DATA_PTR_PTR
 *  @param[out] _DATA_LENGTH_PTR
 *
 *  This macro should provide the pointer and the length to remanent
 *  pdev data. The provided buffer should be static as it is used
 *  asynchronous.
 *
 */
#define LSAS_CFG_REMA_READ_PDEV_DATA(_DATA_PTR_PTR, _DATA_LENGTH_PTR )\
{\
    PnpbNvRestoreRemaMem (_DATA_LENGTH_PTR, (PNIO_UINT8**)_DATA_PTR_PTR); \
}

/**
 *  @brief safe the pdev_data
 *
 *  @param[in]  _DATA_PTR pointer to the pdev data that should be written
 *  @param[in]  _DATA_LEN length of the pdev data to be writen
 *
 *  This macro is used to indicate pdev data to be safed for later use.
 *  A pointer to the data buffer and the length is given.
 *  _DATA_LEN is allways bigger then zero (see LSAS_CFG_REMA_DELETE_PDEV_DATA()) .
 *  It may considered to check the indicated buffer if it is equal to an already
 *  present remanent pdev data, to avoid writing the same data over and over again.
 *  It must be assured that the data has been written on exiting this macro therefor
 *  writing the data should be done synchron.
 *
 */
#define LSAS_CFG_REMA_WRITE_PDEV_DATA(_DATA_PTR, _DATA_LEN)\
{\
    PnpbNvStoreRemaMem (_DATA_LEN, _DATA_PTR); \
}

/**
 *  @brief delete remanent pdev data
 *
 *
 *  This macro is used to indicate pdev data to be deleted for remanent media.
 *  It must be assured that the data has been deleted on exiting this macro therefor
 *  deleting the data should be done synchron.
 *
 *
 */
#define LSAS_CFG_REMA_DELETE_PDEV_DATA()\
{\
    PnpbNvStoreRemaMem (0, 0);  \
}


/**
 *  @brief indicate a change in the hello-mode setting
 *
 *  @param[in]  _HELLO_MODE_ENABLE Enable hello-mode if LSA_TRUE, otherwise disable it
 *
 *  _HELLO_MODE_ENABLE == LSA_TRUE turn hello-mode on
 *  _HELLO_MODE_ENABLE == LSA_FALSE turn hello-mode off
 *
 */
#define LSAS_CFG_REMA_INDICATE_HELLO_MODE(_HELLO_MODE_ENABLE)\
{\
}

/**
 *  @brief return the if hello-mode is on
 *
 *  @return     return_value_description
 *
 *  This macro should return or be equal LSA_TRUE if
 *  hello-mode is enabled otherwise LSA_FALSE.
 *
 */
#define LSAS_CFG_REMA_GET_HELLO_MODE()

/**
 *  @brief safe snmp sys data
 *
 *  @param[in]  _SYS_NAME_PTR       pointer to sysname
 *  @param[in]  _SYS_NAME_LEN       length of sysname
 *  @param[in]  _SYS_CONTACT_PTR    pointer to syscontact
 *  @param[in]  _SYS_CONTACT_LEN    length of syscontact
 *  @param[in]  _SYS_LOCATION_PTR   pointer to syslocation
 *  @param[in]  _SYS_LOCATION_LEN   length of syslocation
 *
 *  This macro is used to indicate new data for snmp sys elements to be safed for later use.
 *  All buffers contain none zero terminated strings so don't use strcpy!
 *  If one of the length is zero, the corresponding remanent data has to be deleted.
 *
 */
#define LSAS_CFG_REMA_SNMP_SYS_INFO(_SYS_NAME_PTR, _SYS_NAME_LEN, _SYS_CONTACT_PTR, _SYS_CONTACT_LEN, _SYS_LOCATION_PTR, _SYS_LOCATION_LEN)\
{\
   PnpbNvStoreSnmpData (_SYS_NAME_LEN, (PNIO_UINT8*)_SYS_NAME_PTR, _SYS_CONTACT_LEN, (PNIO_UINT8*)_SYS_CONTACT_PTR, _SYS_LOCATION_LEN, (PNIO_UINT8*)_SYS_LOCATION_PTR); \
}

/**
 *  @brief safe snmp sys data
 *
 *  @param[out] _SYS_NAME_PTR       pointer to sysname
 *  @param[in]  _SYS_NAME_LEN       length of sysname
 *  @param[out] _SYS_CONTACT_PTR    pointer to syscontact
 *  @param[in]  _SYS_CONTACT_LEN    length of syscontact
 *  @param[out] _SYS_LOCATION_PTR   pointer to syslocation
 *  @param[in]  _SYS_LOCATION_LEN   length of syslocation
 *  @param[out] _RET_VAL_PTR        set LSA_FALSE if one of the lenght is to short
 *
 *  This macro is used to indicate new data for snmp sys elements to be safed for later use.
 *  All buffers contain none zero terminated strings so don't use strcpy!
 *  If one of the length is zero, the corresponding remanent data has to be deleted.
 *
 */

#define LSAS_CFG_REMA_GET_SNMP_SYS_INFO(_SYS_NAME_PTR, _SYS_NAME_LEN, _SYS_CONTACT_PTR, _SYS_CONTACT_LEN, _SYS_LOCATION_PTR, _SYS_LOCATION_LEN, _RET_VAL_PTR)\
{\
    PNPB_SNMP_DATA* pSnmp = PnpbNvGetpSnmpData (); \
    if ((_SYS_NAME_LEN <= pSnmp->SysNameLen)|| (_SYS_NAME_LEN <= pSnmp->SysNameLen) || (_SYS_NAME_LEN <= pSnmp->SysNameLen)) \
    { \
        *_RET_VAL_PTR = LSA_FALSE; \
    } \
    else \
    { \
        OsMemCpy (_SYS_NAME_PTR,     pSnmp->SysName, pSnmp->SysNameLen);\
        OsMemCpy (_SYS_CONTACT_PTR,  pSnmp->SysCont, pSnmp->SysContLen);\
        OsMemCpy (_SYS_LOCATION_PTR, pSnmp->SysLoc,  pSnmp->SysLocLen);\
    } \
}


/**
 *  @brief provide remanent snmp data
 *
 *  @param[out] _DATA_PTR_PTR
 *  @param[out] _DATA_LENGTH_PTR
 *
 *  This macro should provide the pointer and the length to remanent
 *  snmp data. The provided buffer should be static as it is used
 *  asynchronous.
 *
 */
#define LSAS_CFG_REMA_READ_SNMP_DATA( _NAME_PTR_PTR, _NAME_LENGTH_PTR, _CONTACT_PTR_PTR, _CONTACT_LENGTH_PTR, _LOCATION_PTR_PTR, _LOCATION_LENGTH_PTR )\
{\
	PnpbNvpSnmpDatas( (Unsigned8**)_NAME_PTR_PTR, _NAME_LENGTH_PTR, (Unsigned8**)_CONTACT_PTR_PTR, _CONTACT_LENGTH_PTR, (Unsigned8**)_LOCATION_PTR_PTR, _LOCATION_LENGTH_PTR );\
    /*PnpbNvRestoreRemaMem (_DATA_LENGTH_PTR, (PNIO_UINT8**)_DATA_PTR_PTR);*/ \
}

/**
 *  @brief safe the snmp_data
 *
 *  @param[in]  _DATA_PTR pointer to the snmp data that should be written
 *  @param[in]  _DATA_LEN length of the snmp data to be writen
 *
 *  This macro is used to indicate snmp data to be safed for later use.
 *  A pointer to the data buffer and the length is given.
 *  _DATA_LEN is allways bigger then zero (see LSAS_CFG_REMA_DELETE_SNMP_DATA()) .
 *  It may considered to check the indicated buffer if it is equal to an already
 *  present remanent snmp data, to avoid writing the same data over and over again.
 *  It must be assured that the data has been written on exiting this macro therefor
 *  writing the data should be done synchron.
 *
 */
#define LSAS_CFG_REMA_WRITE_SNMP_DATA(_DATA_PTR, _DATA_LEN)\
{\
	LSA_UINT16 write_snmp_data_Response;\
	LSA_UINT16 SysNameLen     = 0;\
	LSA_UINT16 SysContactLen  = 0;\
	LSA_UINT16 SysLocationLen = 0;\
	LSA_UINT32 err_offset = 0;\
	OHA_COMMON_MEM_U8_PTR_TYPE pSysName     = LSA_NULL;\
	OHA_COMMON_MEM_U8_PTR_TYPE pSysContact  = LSA_NULL;\
	OHA_COMMON_MEM_U8_PTR_TYPE pSysLocation = LSA_NULL;\
\
    write_snmp_data_Response = OHA_Get_PrmSNMPData((_DATA_PTR),        /* get the record data */\
								                   (_DATA_LEN),\
								                   &pSysName,\
								                   &SysNameLen,\
								                   &pSysContact,\
								                   &SysContactLen,\
								                   &pSysLocation,\
								                   &SysLocationLen,\
								                   &err_offset);\
	if (write_snmp_data_Response == OHA_OK)\
	{\
		PnpbNvStoreSnmpData (SysNameLen, (PNIO_UINT8*)pSysName, SysContactLen, (PNIO_UINT8*)pSysContact, SysLocationLen, (PNIO_UINT8*)pSysLocation); \
	}\
}

/**
 *  @brief delete remanent snmp data
 *
 *
 *  This macro is used to indicate snmp data to be deleted for remanent media.
 *  It must be assured that the data has been deleted on exiting this macro therefor
 *  deleting the data should be done synchron.
 *
 *
 */
#define LSAS_CFG_REMA_DELETE_SNMP_DATA()\
{\
 \
}

/**
 *  @brief provide remanent ip data
 *
 *  @param[out] _DATA_PTR_PTR
 *  @param[out] _DATA_LENGTH_PTR
 *
 *  This macro should provide the pointer and the length to remanent
 *  ip data. The provided buffer should be static as it is used
 *  asynchronous.
 *
 */
#define LSAS_CFG_REMA_READ_IP_DATA(_DATA_PTR_PTR, _DATA_LENGTH_PTR )\
{\
	PnpbNvGetpIpSuit((Unsigned8**)(_DATA_PTR_PTR), _DATA_LENGTH_PTR);\
}

/**
 *  @brief safe the ip_data
 *
 *  @param[in]  _DATA_PTR pointer to the ip data that should be written
 *  @param[in]  _DATA_LEN length of the ip data to be writen
 *
 *  This macro is used to indicate ip data to be safed for later use.
 *  A pointer to the data buffer and the length is given.
 *  _DATA_LEN is allways bigger then zero (see LSAS_CFG_REMA_DELETE_IP_DATA()) .
 *  It may considered to check the indicated buffer if it is equal to an already
 *  present remanent ip data, to avoid writing the same data over and over again.
 *  It must be assured that the data has been written on exiting this macro therefor
 *  writing the data should be done synchron.
 *
 */
#define LSAS_CFG_REMA_WRITE_IP_DATA(_DATA_PTR, _DATA_LEN)\
{\
    OHA_UPPER_MEM_U8_PTR_TYPE    pIpAddr;\
    LSA_UINT16					 Length;\
    LSA_UINT32 err_offset = 0;\
\
	Response = OHA_Get_PrmIpSuiteData((_DATA_PTR),        /* get the record data */\
                                      (_DATA_LEN),\
                                      &pIpAddr,\
                                      &Length,\
                                      &err_offset\
                                      );\
\
	if (Response == OHA_OK)\
	{\
		PnpbNvSaveIpSuite (*((PNIO_UINT32*)pIpAddr), *((PNIO_UINT32*)(pIpAddr+4)), *((PNIO_UINT32*)(pIpAddr+8)), LSA_TRUE); \
	}\
}

/**
 *  @brief send ip data to user level
 *
 *
 *  Reporting new ip to user, so it may be used for upper level (usrapp) functionality
 *
 *
 */
#define LSAS_CFG_REPORT_NEW_IP_DATA(_IP_PTR)\
{\
	Pnpb_report_new_ip_data (*((PNIO_UINT32*)_IP_PTR), *((PNIO_UINT32*)(_IP_PTR+1)), *((PNIO_UINT32*)(_IP_PTR+2))); \
}

/**
 *  @brief delete remanent ip data
 *
 *
 *  This macro is used to indicate ip data to be deleted for remanent media.
 *  It must be assured that the data has been deleted on exiting this macro therefor
 *  deleting the data should be done synchron.
 *
 *
 */
#define LSAS_CFG_REMA_DELETE_IP_DATA()\
{\
	PnpbNvSaveIpSuite (0, 0, 0, LSA_FALSE);\
}

/**
 *  @brief provide remanent nos data
 *
 *  @param[out] _DATA_PTR_PTR
 *  @param[out] _DATA_LENGTH_PTR
 *
 *  This macro should provide the pointer and the length to remanent
 *  name of station data. The provided buffer should be static as it is used
 *  asynchronous.
 *
 */
#define LSAS_CFG_REMA_READ_NOS_DATA(_DATA_PTR_PTR, _DATA_LENGTH_PTR )\
{\
	PnpbNvGetpStationName((Unsigned8**)(_DATA_PTR_PTR), _DATA_LENGTH_PTR);\
}

/**
 *  @brief safe the nos_data
 *
 *  @param[in]  _DATA_PTR pointer to the nos data that should be written
 *  @param[in]  _DATA_LEN length of the nos data to be writen
 *
 *  This macro is used to indicate name of station data to be safed for later use.
 *  A pointer to the data buffer and the length is given.
 *  _DATA_LEN is allways bigger then zero (see LSAS_CFG_REMA_DELETE_NOS_DATA()) .
 *  It may considered to check the indicated buffer if it is equal to an already
 *  present remanent nos data, to avoid writing the same data over and over again.
 *  It must be assured that the data has been written on exiting this macro therefor
 *  writing the data should be done synchron.
 *
 */
#define LSAS_CFG_REMA_WRITE_NOS_DATA(_DATA_PTR, _DATA_LEN)\
{\
    OHA_UPPER_MEM_U8_PTR_TYPE    pNosAddr;\
    LSA_UINT16					 Length;\
    LSA_UINT32 err_offset = 0;\
\
	Response = OHA_Get_PrmNameOfStationData((_DATA_PTR),        /* get the record data */\
                                      (_DATA_LEN),\
                                      &pNosAddr,\
                                      &Length,\
                                      &err_offset\
                                      );\
\
	if (Response == OHA_OK)\
	{\
		PnpbNvSaveStationName ((PNIO_INT8*)pNosAddr, Length, (Length == 0) ? LSA_FALSE:LSA_TRUE); \
	}\
}

/**
 *  @brief delete remanent nos data
 *
 *
 *  This macro is used to indicate nos data to be deleted for remanent media.
 *  It must be assured that the data has been deleted on exiting this macro therefor
 *  deleting the data should be done synchron.
 *
 *
 */
#define LSAS_CFG_REMA_DELETE_NOS_DATA()\
{\
	PnpbNvSaveStationName ((PNIO_INT8*)0, 0, LSA_FALSE); \
}

/**
 * @brief interface description (interface)
 *
 * define snmp mib2 strings for the internal interface and ports
 */
#define LSAS_CFG_OHA_IF_DESCR_INTERFACE  	IOD_CFG_SNMP_INTERFACE_NAME

#define LSAS_CFG_OHA_IF_DESCR_PORT_01   	IOD_CFG_SNMP_PORT1_NAME

#define LSAS_CFG_OHA_IF_DESCR_PORT_02   	IOD_CFG_SNMP_PORT2_NAME

#define LSAS_CFG_OHA_IF_DESCR_PORT_03   	IOD_CFG_SNMP_PORT3_NAME

#define LSAS_CFG_OHA_IF_DESCR_PORT_04   	IOD_CFG_SNMP_PORT4_NAME

/**
 * @brief snprintf marcos
 */
#ifndef OHA_SNPRINTF
    #define OHA_SNPRINTF(_str, _fmt, _argptr)   LSAS_SNPRINTF(_str, _fmt, _argptr)
#endif

#ifndef CLRPC_SPRINTF
    #define CLRPC_SPRINTF    LSAS_SPRINTF
#endif

#ifndef CLRPC_SSCANF
	#define CLRPC_SSCANF 	/* not used */
#endif

#define LSAS_SOCK_ENTER() 	                        OsEnterX(OS_MUTEX_SOCK)
#define LSAS_TCIP_ENTER()                           OsEnterX(OS_MUTEX_TCPIP)

#define LSAS_SOCK_EXIT() 	                        OsExitX(OS_MUTEX_SOCK)
#define LSAS_TCIP_EXIT()                            OsExitX(OS_MUTEX_TCPIP)

#define LSAS_ATTR_CODE_FAST		OS_CODE_FAST

#if ( PNIOD_PLATFORM & (PNIOD_PLATFORM_ECOS_EB200P | PNIOD_PLATFORM_POSIX_EB200P ))
#define LSAS_HW_WATCHDOG_INIT(_time, _granity)      Bsp_hw_watchdog_init(_time, _granity)
#define LSAS_HW_WATCHDOG_DEINIT()                   Bsp_hw_watchdog_deinit()
#define LSAS_HW_WATCHDOG_START()                    Bsp_hw_watchdog_start()
#define LSAS_HW_WATCHDOG_STOP()                     Bsp_hw_watchdog_stop()
#define LSAS_HW_WATCHDOG_TRIGGER()                  Bsp_hw_watchdog_trigger()
#else
#define LSAS_HW_WATCHDOG_INIT(_time, _granity)
#define LSAS_HW_WATCHDOG_DEINIT()
#define LSAS_HW_WATCHDOG_START()
#define LSAS_HW_WATCHDOG_STOP()
#define LSAS_HW_WATCHDOG_TRIGGER()
#endif

#define LSAS_SERVE_WATCHDOG()

LSA_VOID LSAS_SET_ADDRESS_TO_OHA(  LSA_UINT32 param_to_change, void* pStationData  );

#include "lsas_plau.h"

#endif /* LSAS_CFG_H_ */

/*** end of file *************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
