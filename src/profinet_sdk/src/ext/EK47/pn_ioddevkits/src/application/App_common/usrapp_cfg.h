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
/*  F i l e               &F: usrapp_cfg.h                              :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  configuration file for the application example                           */
/*  has to be modified by the user.                                          */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*                                                                           */
/*  Date        Version        Who  What                                     */
/*                                                                           */
/*****************************************************************************/

#ifndef _USR_APP_CFG_H
#define _USR_APP_CFG_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

#include "iod_cfg.h"
#include "iod_cfg2.h"


// *-------------------------------------------------
// *  EXAMPL_DEV_CONFIG_VERSION
// *
// *  specifies, application example shall be compiled.
// *  Possible values are
// *
// *    1      RT or IRT example for ERTEC200/400 platforms with standard io modules,
// *            - includes MRP-client, shared device, FSU, PROFIenergy-support
// *            - optional support for IRT io modules, that handle  record 8030 support for RT Class3
// *
// *    2      DBAI example (direct buffer access interface),based on RT and IRT
// *            - no record 8030 support for IRT Class3
// *
// *    3      IRT example with synchronized application - uses special io modules
// *            - configures PLL and assigns to GPIO 5, 7 for T-input, T-output
// *
// *    4      XHIF RT or IRT example for ERTEC200P platforms with standard io modules,
// *            - includes MRP-client, shared device, PROFIenergy-support
// *-------------------------------------------------
#ifndef EXAMPL_DEV_CONFIG_VERSION
#define EXAMPL_DEV_CONFIG_VERSION 1
#endif


#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P)
#define MODULE_ID_DAP_NO_PDEV          0x01       // DAP 1  no PDEV, for old pnio controller only
#define MODULE_ID_DAP_2PORT_NO_MRP     0x02       // DAP 2  standard, no MRP capability, but Fast Startup
#define MODULE_ID_DAP_SWITCH           0x03       // DAP 3, including switch with more than one port
#define MODULE_ID_DAP_1PORT            0x04       // DAP 4  standard, no MRP capability, but Fast Startup
#define MODULE_ID_DAP_POF_1PORT        0x05       // DAP 5, including physical device for 1 fiber optic (POF)
#define MODULE_ID_DAP_POF              0x06       // DAP 6, including physical device for fiber optic (POF)
#define MODULE_ID_DAP_S2			   0x07		  // DAP 7, including switch with more than one port, S2 redundancy enabled
#define MODULE_ID_DAP_DR               0x08       // DAP 8, S2 redundancy with DR (CiR)
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_RZT2)
#define MODULE_ID_DAP_NO_PDEV          0x01       // DAP 1  no PDEV, for old pnio controller only
#define MODULE_ID_DAP_2PORT_NO_MRP     0x02       // DAP 2  standard, no MRP capability, but Fast Startup
#define MODULE_ID_DAP_SWITCH           0x03       // DAP 3, including switch with more than one port
#define MODULE_ID_DAP_1PORT            0x04       // DAP 4  standard, no MRP capability, but Fast Startup
#define MODULE_ID_DAP_POF_1PORT        0x05       // DAP 5, including physical device for 1 fiber optic (POF)
#define MODULE_ID_DAP_POF              0x06       // DAP 6, including physical device for fiber optic (POF)
#define MODULE_ID_DAP_S2			   0x07		  // DAP 7, including switch with more than one port, S2 redundancy enabled
#define MODULE_ID_DAP_DR               0x08       // DAP 8, S2 redundancy with DR (CiR)
#else
#define MODULE_ID_DAP_NO_PDEV          0x01       // DAP 1  no PDEV, for old pnio controller only
#define MODULE_ID_DAP_1PORT            0x02       // DAP 2  standard, no MRP capability, but Fast Startup
#endif

/* Consistency check - Some config combinations are invalid -> inform user */
#if ((1 == IOD_USED_WITH_XHIF_HOST) && (1 == EXAMPL_DEV_CONFIG_VERSION))
#error "Application example 1 is not intended to be used as xhif host"
#endif
#if ((1 == IOD_USED_WITH_XHIF_HOST) && (2 == EXAMPL_DEV_CONFIG_VERSION))
#error "Application example 2 is not intended to be used as xhif host"
#endif
#if ((1 == IOD_USED_WITH_XHIF_HOST) && (3 == EXAMPL_DEV_CONFIG_VERSION))
#error "Application example 3 is not intended to be used as xhif host"
#endif
#if ((0 == IOD_USED_WITH_XHIF_HOST) && (4 == EXAMPL_DEV_CONFIG_VERSION))
#error "Application example 4 is intended to be used with xhif"
#endif

// *-------------------------------------------------
// *  default submodule ID configuration
// *  note:  DAP-submodule, PDEV-IF and PDEV-port must
// *         have different submodule IDs, if they belong
// *         to the same module (e.g. DAP-module).
// *-------------------------------------------------
#ifndef IOD_CFG_PDEV_NUMOF_PORTS
    #error "IOD_CFG_PDEV_NUMOF_PORTS is not defined"
#endif
#if (IOD_INCLUDE_S2_REDUNDANCY == 1)
    #if (IOD_INCLUDE_DR == 1)
        #define MODULE_ID_DAP          MODULE_ID_DAP_DR
    #else
    	#define MODULE_ID_DAP		   MODULE_ID_DAP_S2
    #endif
#else
#if (IOD_CFG_PDEV_NUMOF_PORTS == 1)
    #if ( IOD_INCLUDE_POF == 0 )
        #define MODULE_ID_DAP          MODULE_ID_DAP_1PORT
    #else
        #define MODULE_ID_DAP          MODULE_ID_DAP_POF_1PORT
    #endif
#else
	#if ( IOD_INCLUDE_POF == 0 )
        #if (IOD_INCLUDE_MRP == 1)
            #define MODULE_ID_DAP      MODULE_ID_DAP_SWITCH
        #else
            #define MODULE_ID_DAP      MODULE_ID_DAP_2PORT_NO_MRP
        #endif
	#else
        #define MODULE_ID_DAP      MODULE_ID_DAP_POF
	#endif
#endif
#endif

#define SUBMOD_ID_DEFAULT      1       // we use only one ID for all submodules here, except PDEV ports
#define SUBMOD_ID_PDEV_IF      2       // ID for PDEV interface submodule
#define SUBMOD_ID_PDEV_PORT    3       // ID for PDEV port submodules

#define SUBMOD_ID_MULTI1       1
#define SUBMOD_ID_MULTI2       2

// *-----------------------------
// *    define IO module IDs
// *    (must fit to GSD file)
// *-----------------------------
#define IO_MODULE_1_BYTE_INOUT      0x20    // 1 byte I + 1 byte O
#define IO_MODULE_1_BYTE_IN         0x21    // 1 byte I
#define IO_MODULE_1_BYTE_OUT        0x22    // 1 byte O
#define IO_MODULE_64_BYTE_INOUT     0x29    // 64 byte I
#define IO_MODULE_64_BYTE_IN        0x30    // 64 byte I
#define IO_MODULE_64_BYTE_OUT       0x31    // 64 byte O
#define IO_MODULE_64_BYTE_IN_IRT    0x50    // 64 byte I, synchronized application (IRT only)
#define IO_MODULE_64_BYTE_OUT_IRT   0x51    // 64 byte O, synchronized application (IRT only)
#define IO_MODULE_1_BYTE_IN_IRT     0x52    // 1 byte I, synchronized application (IRT only)
#define IO_MODULE_1_BYTE_OUT_IRT    0x53    // 1 byte O, synchronized application (IRT only)
#define IO_MODULE_MULTISUBSLOT      0x60    // 1 byte I + 1 byte I + 1 byte O + 1 byte O

#define IO_MODULE_250_BYTE_INOUT    0x2D
#define IO_MODULE_250_BYTE_IN       0x2E
#define IO_MODULE_250_BYTE_OUT      0x2F


/*IN OUT Data sizes*/
#define IO_MODULE_ZERO_DATA_SIZE                0
#define IO_MODULE_1_BYTE_INOUT_INDATA_SIZE      1
#define IO_MODULE_1_BYTE_INOUT_OUTDATA_SIZE     1
#define IO_MODULE_1_BYTE_IN_INDATA_SIZE         1
#define IO_MODULE_1_BYTE_OUT_OUTDATA_SIZE       1
#define IO_MODULE_64_BYTE_INOUT_INDATA_SIZE     64
#define IO_MODULE_64_BYTE_INOUT_OUTDATA_SIZE    64
#define IO_MODULE_64_BYTE_IN_INDATA_SIZE        64
#define IO_MODULE_64_BYTE_OUT_OUTDATA_SIZE      64



// *-------------------------------------------------
// *  set number of slots and subslots, that is used
// *  in this example application
// * (defines allocated memory inside the application
// * for managing slots, subslots and IO data)
// *-------------------------------------------------
#define NUMOF_SLOTS				        IOD_CFG_MAX_NUMOF_SUBSLOTS//17		// must be lower or equal IOD_CFG_NUMOF_SLOTS
#define NUMOF_SUBSLOTS			        IOD_CFG_MAX_NUMOF_SUBSL_PER_SLOT//6		// must be lower or equal IOD_CFG_NUMOF_SUBSLOTS_PER_SLOT
#define NUMOF_BYTES_PER_SUBSLOT	        IOD_CFG_NUMOF_BYTES_PER_SUBSLOT     // see example modules, plugged in usriod_main.c


// *-----------------------------------------------------------------------
// *    define a default configuration for SI and DBAI - example
// *-----------------------------------------------------------------------
#define EXAMPL_MOD_ID_SLOT1             IO_MODULE_64_BYTE_IN        // standard input module, usable in RT or IRT
#define EXAMPL_MOD_ID_SLOT2             IO_MODULE_64_BYTE_OUT       // standard output module, usable in RT or IRT

#define EXAMPL_1_SLOTNUM		        1                           // slot number
#define EXAMPL_1_SUBSLOTNUM	            1                           // subslot number
#define EXAMPL_1_DIAG_TAG		        0x2345					    // free defined by the user
#define EXAMPL_1_USER_STRUCT_IDENT      0x1234                      // user defined (0...0x7fff)

// ***** EXAMPLE 2:  channel diagnostic alarm ********
#define EXAMPL_2_SLOTNUM		        1                           // slot number
#define EXAMPL_2_SUBSLOTNUM	            1                           // subslot number
#define EXAMPL_2_CHANNELNUM	            2                           // channel number
#define EXAMPL_2_CHAN_ERR_TYPE          DIAG_CHAN_ERR_LINE_BREAK	// see IEC 61158
#define EXAMPL_2_DIAG_TAG		        0x2346					    // free defined by the user
#define EXAMPL_2_USER_STRUCT_IDENT      0x1234                      // user defined (0...0x7fff)
#define EXAMPL_2_IO_DIRECTION           DIAG_CHANPROP_DIR_IN        // input/output/input-output
#define EXAMPL_2_IO_DATA_TYPE           DIAG_CHANPROP_TYPE_BYTE     // data type (..., byte, word,.. )

// ***** EXAMPLE 3:  extended channel diagnostic alarm ********
#define EXAMPL_3_SLOTNUM		        2                           // slot number
#define EXAMPL_3_SUBSLOTNUM	            1                           // subslot number
#define EXAMPL_3_CHANNELNUM	            3                           // channel number
#define EXAMPL_3_CHAN_ERR_TYPE          DIAG_CHAN_ERR_LINE_BREAK	// see IEC 61158
#define EXAMPL_3_EXT_CHAN_ERR_TYPE      0x0000                  	// see IEC 61158
#define EXAMPL_3_EXT_CHAN_ADD_VALUE     0x00000000              	// see IEC 61158
#define EXAMPL_3_DIAG_TAG		        0x2347					    // free defined by the user
#define EXAMPL_3_USER_STRUCT_IDENT      0x1234                      // user defined (0...0x7fff)
#define EXAMPL_3_IO_DIRECTION           DIAG_CHANPROP_DIR_OUT       // input/output/input-output
#define EXAMPL_3_IO_DATA_TYPE           DIAG_CHANPROP_TYPE_BYTE     // data type (..., byte, word,.. )

// ***** EXAMPLE 4:  generic diagnostic alarm ********
#define EXAMPL_4_SLOTNUM		        0                           // slot number
#define EXAMPL_4_SUBSLOTNUM	            1                           // subslot number
#define EXAMPL_4_CHANNELNUM	            4                           // channel number
#define EXAMPL_4_DIAG_TAG		        0x2348					    // free defined by the user
#define EXAMPL_4_USER_STRUCT_IDENT      0x1234                      // user defined (0...0x7fff)
#define EXAMPL_4_IO_DIRECTION           DIAG_CHANPROP_DIR_IN        // input/output/input-output
#define EXAMPL_4_IO_DATA_TYPE           DIAG_CHANPROP_TYPE_BYTE     // data type (..., byte, word,.. )


#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
