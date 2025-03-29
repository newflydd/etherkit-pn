#ifndef EDDP_K32_FW_H                    /* ----- reinclude-protection ----- */
#define EDDP_K32_FW_H


#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

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
/*  C o m p o n e n t     &C: EDDP (EthernetDeviceDriver for PN-IP)     :C&  */
/*                                                                           */
/*  F i l e               &F: eddp_k32_fw.h                             :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP KRISC32 firmware                            */
/*                                                                           */
/*****************************************************************************/
#ifdef EDDP_MESSAGE
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
/*  2008-12-22  lrg   initial version.                                       */
/*  2011-02-14  lrg   Remove global edd_usr.h                                */
#endif
/*****************************************************************************/

/*===========================================================================*/
/*                              constants                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                 defines                                   */
/*===========================================================================*/
/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/


/* ------------------------------------------------------------------------------------------------------------ */
/* PACKED(4) STRUCTS - ON                                                                                       */
/* Rules for packing of structs see <eddp_hal_pnip_struct.h>                                                    */
/* IMPORTANT: All struct elements in this header must lie/packed naturally aligned (4 bytes)                    */
/* ------------------------------------------------------------------------------------------------------------ */
#ifdef K32_KRISC32OS
    #include "pnio_k32_pck4_on.h"
#else
    #include "pnio_pck4_on.h"
#endif

#include "lsa_cfg.h"

#ifndef K32_KRISC32OS
#include "eddp_k32_image_rev1.h"
#include "eddp_k32_image_rev2.h"
#include "eddp_k32_image_r4_ifa.h"
#include "eddp_k32_image_r4_ifb.h"
#endif /* K32_KRISC32OS */

#ifdef K32_KRISC32OS
	/* normal access in krisc32 firmware (krisc32 and pnip have same endianess) */
#define EDDP_HAL_MEM8_READ(hSysDev_ignore, pMem8)				*(pMem8)
#define EDDP_HAL_MEM16_READ(hSysDev_ignore, pMem16)				*(pMem16)
#define EDDP_HAL_MEM32_READ(hSysDev_ignore, pMem32)				*(pMem32)
#define EDDP_HAL_MEM64_READ(hSysDev_ignore, pMem64)				*(pMem64)
#define EDDP_HAL_MEM8_WRITE(hSysDev_ignore, pMem8, value8)		*(pMem8)  = (value8)
#define EDDP_HAL_MEM16_WRITE(hSysDev_ignore, pMem16, value16)	*(pMem16) = (value16)
#define EDDP_HAL_MEM32_WRITE(hSysDev_ignore, pMem32, value32)	*(pMem32) = (value32)
#define EDDP_HAL_MEM64_WRITE(hSysDev_ignore, pMem64, value64)	*(pMem64) = (value64)
#endif


#ifdef K32_KRISC32OS
	/* normal access in krisc32 firmware (krisc32 and pnip have same endianess) */
#define EDDP_HAL_MEM8_READ(hSysDev_ignore, pMem8)				*(pMem8)
#define EDDP_HAL_MEM16_READ(hSysDev_ignore, pMem16)				*(pMem16)
#define EDDP_HAL_MEM32_READ(hSysDev_ignore, pMem32)				*(pMem32)
#define EDDP_HAL_MEM64_READ(hSysDev_ignore, pMem64)				*(pMem64)
#define EDDP_HAL_MEM8_WRITE(hSysDev_ignore, pMem8, value8)		*(pMem8)  = (value8)
#define EDDP_HAL_MEM16_WRITE(hSysDev_ignore, pMem16, value16)	*(pMem16) = (value16)
#define EDDP_HAL_MEM32_WRITE(hSysDev_ignore, pMem32, value32)	*(pMem32) = (value32)
#define EDDP_HAL_MEM64_WRITE(hSysDev_ignore, pMem64, value64)	*(pMem64) = (value64)
#endif


#define K32_MAX_RRQB			8
/* Number of 16 KB Tiles (ERTEC200+, may change in ERTEC400+!). 
This value is divided into Instruction-TCM and Data-TCM depending on the size of the K32 Firmware code. The ratio is written into the register KRISC_TCM_MAP */
/* REV1: 8 Tiles with 16 kb = 128 KB */
#define K32_TCM_MAP_SIZE_REV1   8 
/* REV2: 16 Tiles with 16 KB = 256 KB */
#define K32_TCM_MAP_SIZE_REV2   16
/* Size of memory blocks reserved for code and data segments of KRISC32 firmware in context of HERA */
#define K32_HERA_ATCM_SIZE        4096   /* 4 KB */
#define K32_HERA_ATCM_TEXT_SIZE   1024   /* 1 KB */
#define K32_HERA_BTCM_SIZE        32768  /* 32 KB */
#define K32_HERA_DDR3_SIZE        512000 /* 500 KB */
#define K32_HERA_DDR3_TEXT_SIZE   262144 /* 256 KB */
#define K32_HERA_DDR3_DATA_SIZE   262144 /* 256 KB */

#define K32_FIRMWARE_REV1       1
#define K32_FIRMWARE_REV2       2
#define K32_FIRMWARE_REV3       3
#define K32_FIRMWARE_DLL_REV1   3
#define K32_FIRMWARE_DLL_REV2   4

/* !!! IMPORTANT: The structure elements must lie/packed naturally aligned (4 bytes) !!! */
PNIO_PACKED_ATTRIBUTE_PRE struct RRQB_QUEUE_STRUCT
{
	LSA_INT32				iBegin;		/* begin index of queue */
	LSA_INT32				iNextAck;	/* index next acknowlegeable element in queue */
	LSA_INT32				iFree;	/* index of next free element in queue */
	K32_RQB_TYPE            slot[K32_MAX_RRQB];	/* RRQB slot */
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct RRQB_QUEUE_STRUCT    RRQB_QUEUE;


/* !!! IMPORTANT: The structure elements must lie/packed naturally aligned (4 bytes) !!! */
PNIO_PACKED_ATTRIBUTE_PRE struct _K32_QUEUE_PARTITION_IN_DATA_QUEUE_RAM_STRUCT
{
	PNIO_PACKED_ATTRIBUTE_PRE struct _K32_BOOT_CONFIG
    {
		LSA_UINT32      partitionSize;
		LSA_UINT32		logMask;
	} PNIO_PACKED_ATTRIBUTE_POST config;
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

	RRQB_QUEUE		eddp;
	RRQB_QUEUE		krisc32;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
} PNIO_PACKED_ATTRIBUTE_POST;
typedef volatile struct _K32_QUEUE_PARTITION_IN_DATA_QUEUE_RAM_STRUCT  K32_QUEUE_PARTITION_IN_DATA_QUEUE_RAM;

/*===========================================================================*/
/*                                  macros                                   */
/*===========================================================================*/

/*===========================================================================*/
/*                        public  functions                                  */
/*===========================================================================*/



/*=============================================================================
 * function name:  K32_HAL_RRQB_SEND
 *
 * function:       Send a RRQB.
 *
 * parameters:
 * pDDB       : Pointer to DeviceDescriptionBlock                   (in)
 * ppRRQB     : Pointer to Reduced RQB                             (out)
 * pRRQBQ     : void Pointer to RRQB-Queue                          (in)
 *
 * return value:
 *     LSA_TRUE if insert into queue,
 *     LSA_FALSE if queue is temporary full.
 *===========================================================================*/
LSA_BOOL K32_HAL_RRQB_SEND(
#ifndef K32_KRISC32OS
			EDDP_LOCAL_DDB_PTR_TYPE pDDB, 
#endif /* K32_KRISC32OS */
			K32_RQB_PTR_TYPE pRRQB,
			volatile LSA_VOID_PTR_TYPE pRRQBQ
		);


/*=============================================================================
 * function name:  K32_HAL_RRQB_GET
 *
 * function:       get next element from RRQB queue that should be processed
 *                 and acknowledged.
 *
 * parameters:
 * pDDB       : Pointer to DeviceDescriptionBlock                   (in)
 * ppRRQB     : Pointer to Reduced RQB                             (out)
 * pRRQBQ     : void Pointer to RRQB-Queue                          (in)
 *
 * return value:
 *     LSA_TRUE if more RRQBs are available in RRQB queue,
 *     LSA_FALSE otherwise.
 *===========================================================================*/
LSA_BOOL K32_HAL_RRQB_GET(
#ifndef K32_KRISC32OS
			EDDP_LOCAL_DDB_PTR_TYPE pDDB, 
#endif /* K32_KRISC32OS */
			K32_RQB_PTR_TYPE* ppRRQB,
			volatile LSA_VOID_PTR_TYPE pRRQBQ
		);


/*=============================================================================
 * function name:  K32_HAL_RRQB_ACK
 *
 * function:       Acknowledge a processed RRQB.
 *
 * parameters:
 * pDDB       : Pointer to DeviceDescriptionBlock                   (in)
 * ppRRQB     : Pointer to Reduced RQB                             (out)
 * pRRQBQ     : void Pointer to RRQB-Queue                          (in)
 *
 * return value:
 *     the number of remaining elements in the RRQBs queue.
 *===========================================================================*/
LSA_INT32 K32_HAL_RRQB_ACK(
#ifndef K32_KRISC32OS
			EDDP_LOCAL_DDB_PTR_TYPE pDDB, 
#endif /* K32_KRISC32OS */
			K32_RQB_PTR_TYPE pRRQB,
			volatile LSA_VOID_PTR_TYPE pRRQBQ
		);


/*=============================================================================
 * function name:  K32_HAL_RRQB_GET_ACK
 *
 * function:       get next ACK-element from RRQB queue that should
 *                 be processed and finalized.
 *
 * parameters:
 * pDDB       : Pointer to DeviceDescriptionBlock                   (in)
 * ppRRQB     : Pointer to Reduced RQB                             (out)
 * pRRQBQ     : void Pointer to RRQB-Queue                          (in)
 *
 * return value:
 *     LSA_TRUE if more ACK-RRQBs are available in RRQB queue,
 *     LSA_FALSE otherwise.
 *===========================================================================*/
LSA_BOOL K32_HAL_RRQB_GET_ACK(
#ifndef K32_KRISC32OS
			EDDP_LOCAL_DDB_PTR_TYPE pDDB, 
#endif /* K32_KRISC32OS */
			K32_RQB_PTR_TYPE* ppRRQB,
			volatile LSA_VOID_PTR_TYPE pRRQBQ
		);


/*=============================================================================
 * function name:  K32_HAL_RRQB_FINALIZE
 *
 * function:       Finalize a processed ACK-RRQB.
 *
 * parameters:
 * pDDB       : Pointer to DeviceDescriptionBlock                   (in)
 * ppRRQB     : Pointer to Reduced RQB                             (out)
 * pRRQBQ     : void Pointer to RRQB-Queue                          (in)
 *
 * return value:
 *     the number of remaining ACK-elements in the RRQBs queue.
 *===========================================================================*/
LSA_INT32 K32_HAL_RRQB_FINALIZE(
#ifndef K32_KRISC32OS
			EDDP_LOCAL_DDB_PTR_TYPE pDDB, 
#endif /* K32_KRISC32OS */
			K32_RQB_PTR_TYPE pRRQB,
			volatile LSA_VOID_PTR_TYPE pRRQBQ
		);

#ifndef K32_KRISC32OS
/* set configuration after downloading data */
LSA_VOID EDDP_K32_FW_SetConfigData(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32      physicalPNIPK32Address,
	LSA_UINT32      physicalK32DTCMPNIPAddress	
    /* LSA_UINT32      xxx more config data maybe later... */
);
#endif

/*=============================================================================
 * function name:  EDDP_K32_FW_DOWNLOAD
 *
 * function:       Download firmware into KRISC32.
 *
 * parameters:     fw_image is the complete firmware image as a byte array.
 *                 length is the length of the firmware image in byte.
 *
 * return value:   Zero on success, otherwise the address (of the firmware
 *                 image) where the download error occurs.
 *===========================================================================*/
LSA_UINT32 EDDP_K32_FW_DOWNLOAD(
#ifndef K32_KRISC32OS
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
#endif
    LSA_UINT8                 * code,
    LSA_UINT32                  code_length,
    LSA_UINT8                 * data,
    LSA_UINT32                  data_length);

/*=============================================================================
 * function name:  EDDP_K32_FW_DOWNLOAD_HERA
 *
 * function:       Download firmware into KRISC32.
 *
 * parameters:     fw_image is the complete firmware image as a byte array.
 *                 length is the length of the firmware image in byte.
 *                 dst destination address.
 *
 * return value:   Zero on success, otherwise the address (of the firmware
 *                 image) where the download error occurs.
 *===========================================================================*/
LSA_UINT32 EDDP_K32_FW_DOWNLOAD_HERA(
   const LSA_UINT8*         fw_image,
   LSA_UINT32               length,
   volatile LSA_UINT8*      dst);

/*=============================================================================
* function name:  EDDP_K32_FW_DOWNLOAD_HERA_64BIT
*
* function:       Download firmware into KRISC32.
*
* parameters:     fw_image is the complete firmware image as a byte array.
*                 length is the length of the firmware image in byte.
*                 dst destination address.
*
* return value:   Zero on success, otherwise the address (of the firmware
*                 image) where the download error occurs.
*===========================================================================*/
LSA_UINT32 EDDP_K32_FW_DOWNLOAD_HERA_64BIT(
    const LSA_UINT8*         fw_image,
    LSA_UINT32               length,
    volatile LSA_UINT8*      dst);


#ifdef _WIN32
/**@note this section is part of krisc32_hal.h;
 * we need this copied section to avoid reference to krisc32_hal.h
 *
 * KRISC32.DLL exported symbols: KRISC32OS_API
 */
#ifndef K32_KRISC32OS

#ifdef KRISC32OS_EXPORTS
	#define KRISC32OS_API __declspec(dllexport)
#else
	#define KRISC32OS_API __declspec(dllimport)
#endif

/*=============================================================================
 * function name:  K32_RESET
 *
 * function:       Reset KRISC32.
 *                 This function should be used by EDDP.
 *                 The TCM is not affected by this function.
 *
 * parameters:     none
 *
 * return value:   none 
 *===========================================================================*/
KRISC32OS_API LSA_VOID K32_RESET();


/*=============================================================================
 * function name:  K32_RESET_RELEASE
 *
 * function:       Release reset of KRISC32.
 *                 This function should be used by EDDP.
 *                 Downloaded firmware will be executed.
 *
 * parameters:     none
 *
 * return value:   none 
 *===========================================================================*/
KRISC32OS_API LSA_VOID K32_RESET_RELEASE();


KRISC32OS_API LSA_VOID K32_ATTACH_PNIP_MEMORY(volatile LSA_UINT32* PNIP_MEMORY_AREA, volatile LSA_UINT32* DMACW_MEMORY_AREA, volatile LSA_UINT32* DMACW_PHYSICAL_MEMORY_AREA);


#endif /* K32_KRISC32OS */
#endif /* _WIN32 */


/* ------------------------------------------------------------------------------------------------------------ */
/* PACKED(4) STRUCTS - OFF                                                                                      */
/* ------------------------------------------------------------------------------------------------------------ */
#ifdef K32_KRISC32OS
    #include "pnio_k32_pck_off.h"
#else
    #include "pnio_pck_off.h"
#endif

/*****************************************************************************/
/*  end of file eddp_k32_fw.h                                                */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif  /* of EDDP_K32_FW_H */

