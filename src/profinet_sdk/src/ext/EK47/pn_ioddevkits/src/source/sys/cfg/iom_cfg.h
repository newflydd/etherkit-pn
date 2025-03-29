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
/*  F i l e               &F: iom_cfg.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  This file contains the configuration of the IO Manager                   */
/*                                                                           */
/*****************************************************************************/


#ifndef IOM_CFG_H_
#define IOM_CFG_H_

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200_400)
    #define IOM_CFG_IRTE    /* Ertec200/400  */
    #if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB400)
        #define IOM_CFG_IRTE400    /* Ertec400  */
    #endif
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P)
    #define IOM_CFG_PNIP  /* Ertec200p */
#ifdef ENABLE_S2_REDUNDANCY
	#define IOM_SYSTEM_REDUNDANCY_SUPPORT
#endif
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_DKSW)
    #define IOM_CFG_SWIF	/* STandart Ethernet Controller, MSVC   */
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_RZT2_EDDP)
    #define IOM_CFG_PNIP  /* Ertec200p */
#ifdef ENABLE_S2_REDUNDANCY
	#define IOM_SYSTEM_REDUNDANCY_SUPPORT
#endif
#else
    #error "invalid platform"
#endif


/*****************************************************************
 * general defines and declarations, valid for both PNIP and IRTE
 *****************************************************************/

 #if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200_400)
	/* locking to enable multiple tasks to access the module (PNIP) */
    #define IOM_ENTER()     OsIntDisable()
    #define IOM_EXIT()      OsIntEnable()
    /* special enter/exit macros for lock() and unlock() functions - can be left empty
     * if buffer locking is always done in interrupt context */
	#define IOM_ENTER_IO()  IOM_ENTER()
	#define IOM_EXIT_IO()   IOM_EXIT()
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P)
    /* locking to enable multiple tasks to access the module (PNIP) */
    #define IOM_ENTER()
    #define IOM_EXIT()
    /* special enter/exit macros for lock() and unlock() functions - can be left empty
     * if buffer locking is always done in interrupt context */
    #define IOM_ENTER_IO()  IOM_ENTER()
    #define IOM_EXIT_IO()   IOM_EXIT()
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_DKSW)

    /* locking to enable multiple tasks to access the module (PNIP) */
	#define IOM_ENTER() 	OsEnterX(OS_MUTEX_IOM) /// $$$bj
	#define IOM_EXIT() 		OsExitX(OS_MUTEX_IOM)  /// $$$bj
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_RZT2)

    /* locking to enable multiple tasks to access the module (PNIP) */
    #define IOM_ENTER()
    #define IOM_EXIT()
/* special enter/exit macros for lock() and unlock() functions - can be left empty
     * if buffer locking is always done in interrupt context */
    #define IOM_ENTER_IO()  IOM_ENTER()
    #define IOM_EXIT_IO()   IOM_EXIT()
#endif


#define IOM_MEMSET(_DST,_VAL,_LEN)  LSA_MEMSET(_DST,_VAL,_LEN)

    /* if defined : use the DTCM to store meta data */
#ifndef IOM_CFG_SWIF
#if(PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200P)
	#define IOM_CFG_LOCATE_DATA_TO_TCM
#endif
#endif
#ifdef IOM_CFG_LOCATE_DATA_TO_TCM
        /* specifies the name of the DTCM section. for example use ".data_d_tcm" */
    #define IOM_TCM_SECTION_NAME            OS_DATA_DTCM_SECTION_NAME
    #define IOM_DEFAULT_BSS_SECTION_NAME    ".bss_sys_iom"
#endif

    /* if defined : use the ITCM for time-critical instruction code (ISR) */
#ifdef IOM_CFG_PNIP
#if(PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200P)
	#define IOM_CFG_LOCATE_INSTRUCTIONS_TO_TCM
#endif
#endif
#ifdef IOM_CFG_LOCATE_INSTRUCTIONS_TO_TCM
    #define IOM_ITCM_SECTION_NAME           OS_ITCM_SECTION_NAME
    #define IOM_DEFAULT_TEXT_SECTION_NAME   ".text_sys_iom"
#endif

/* ---------------------------------------------------------------------------- */
/* error handling. if IOM_CFG_FATAL_ERROR_ON_FAILURE is defined, all asserts    */
/* will lead to fatal error. Otherwise you'll get a corresponding return value  */
/* ---------------------------------------------------------------------------- */
#define IOM_CFG_FATAL_ERROR_ON_FAILURE
    /* error handling */
#define IOM_LSA_COMPONENT_ID                 0x0000

#define IOM_FATAL_ERROR(_ERROR_DETAIL_PTR)        \
{                                                 \
    PnpbLsasFatalError (_ERROR_DETAIL_PTR);       \
}

#define IOM_MODULE_PERIF                     0x01       /* firmware module id inside the IOM (iom_perif.c), used in fatal error routine */
#define IOM_MODULE_KRAM                      0x02       /* firmware module id inside the IOM (iom_kram.c), used in fatal error routine */
#define IOM_MODULE_SWIF                      0x03       /* firmware module id inside the IOM (iom_swif.c), used in fatal error routine */

    /* function that is called after the completion of deallocation of PERIF/KRAM memory */
#define IOM_IOCR_MEMORY_FREED(cur_ar)		/* nothing to do */

    /* allow unbuffered locking / unlocking for RTC3 connections. Buffer switching becomes unnecessary
     * because of the exact event scheduling in RTC3. The herewith activated ...lock_unbuffered/
     * ...unlock_unbuffered functions act differently compared to the "buffered" ones only in the KRAM submodule.*/
#define IOM_CFG_RTC3_UNBUFFERED

/********************************************
 * defines that are specific to KRAM or PERIF
 ********************************************/
#if defined IOM_CFG_PNIP
    /*
     * PERIF defines
     */

        /* if IOM_CFG_USE_GDMA is defined, the "GDMA reuseable module" can be accessed by
         * the Perif-module in order to install user data DMA jobs
         */
    // #define IOM_CFG_USE_GDMA

        /* number of supported ARs */
    #define IOM_CFG_PERIF_NUM_AR 8
        /* number of supported CRs by PERIF-hardware */
    #define IOM_CFG_PERIF_NUM_CR 27

        /* interrupt configuration */
    #define IOM_SET_INT_HANDLER(callback)     Bsp_EVMA_register_ISR(SY_INT_PER_IF_ARM_IRQ, callback) /* choose between IRQ / FIQ handler function */

	/* use the parameter structures defined in V5.3; otherwise it's compatible to V5.2 */
    #define IOM_USE_PROFINET_STACK_V5_3

#elif defined IOM_CFG_IRTE
    /*
     * KRAM defines
     */
    #define IOM_CFG_KRAM_PROCESS_IMAGE_SIZE                   (LSAS_IOCR_IN_MAX_BUFLEN + LSAS_IOCR_OUT_MAX_BUFLEN)
    #define IOM_CFG_KRAM_IRTE_REG_START                       0x10000000
    #define IOM_CFG_KRAM_DATA_START                           0x10100000     /* start address of the KRAM buffer */


    #if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200)
        #define EDDI_BUFFER_REQUEST_SETUP(ret_val, InstanceHandle, TraceIdx, pKRAM, pIRTE, pGSharedRAM, pPAEARAM, offset_ProcessImageEnd, pollTimeout_ns)    \
                eddi_BufferRequest_Init();                                                                      \
                ret_val = eddi_BufferRequest_Setup(InstanceHandle, TraceIdx, (LSA_VOID * ) pKRAM,               \
                                    (LSA_VOID * ) pIRTE, (LSA_VOID * ) pGSharedRAM, (LSA_VOID * ) pPAEARAM,  offset_ProcessImageEnd, pollTimeout_ns );
    #endif

    #if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB400)

      //#define EDDI_BUFFER_LOCK_TIMEOUT_NS             0x00000400UL  /* recommended value for  PollTimeout */
        #define EDDI_BUFFER_LOCK_TIMEOUT_NS             0x00040000UL  /* recommended value for PollTimeout */

        #define EDDI_BUFFER_REQUEST_SETUP(ret_val, InstanceHandle, TraceIdx, pKRAM, pIRTE, pGSharedRAM, pPAEARAM, offset_ProcessImageEnd, pollTimeout_ns)    \
                eddi_BufferRequest_Init();                                                                      \
                ret_val = eddi_BufferRequest_Setup(InstanceHandle, TraceIdx, (LSA_VOID * ) pKRAM,               \
                                    (LSA_VOID * ) pIRTE, (LSA_VOID * ) pGSharedRAM, (LSA_VOID * ) pPAEARAM,  IOM_CFG_KRAM_PROCESS_IMAGE_SIZE-1, EDDI_BUFFER_LOCK_TIMEOUT_NS);
    #endif

   /* perform the clean up of CR buffers in a synchronous way */
    #define IOM_SYNC_CR_RELEASE_BUFFERS

#elif defined IOM_CFG_SWIF

	/* number of supported ARs */
    #define IOM_CFG_SWIF_NUM_AR 	2


#elif defined IOM_CFG_IRTE

#else
    #error "config error: define IOM_CFG_PNIP OR IOM_CFG_IRTE or IOM_CFG_SWIF"
#endif

/* ****************************************************************** */
#include "iom_plau.h"
/* ****************************************************************** */

#endif /* IOM_CFG_H_ */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
