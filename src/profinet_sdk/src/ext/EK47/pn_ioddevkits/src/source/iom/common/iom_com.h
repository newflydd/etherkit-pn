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
/*  F i l e               &F: iom_com.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  component header for the IO Manager software module (KRAM / PERIF)       */
/*                                                                           */
/*****************************************************************************/


#ifndef IOM_COM_H_
#define IOM_COM_H_

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

#if defined IOM_CFG_PNIP
    /*
     * PERIF specific declarations
     */

    /** initialization parameters
     * @see iom_init
     * @see iom_perif_init
     * */
    typedef struct
    {
        EDD_COMMON_MEM_PTR_TYPE pnip_base_address;
        EDD_COMMON_MEM_PTR_TYPE gsharedmem_address;

    } iom_init_struct_t;

    #ifdef IOM_CFG_USE_GDMA
    /**
     * The function iom_perif_install_dma_transfer() expects this structure to install / initialize
     * a GDMA job in order to copy PERIF data to/from an external memory.
     *
     * @see iom_perif_install_dma_transfer
     */
    typedef union
    {
        /** output CR specific descriptor. data direction : controller -> device (IM) */
        struct
        {
            LSA_UINT32          *dest_addr;             /**< address of the destination buffer, to which the CR data will be copied */
            gdma_trigger_src_t  trigger;                /**< trigger source of the GDMA job to be created */
            LSA_UINT8           priority;               /**< priority of the GDMA job */
            LSA_VOID (*job_done_callback)(LSA_INT16 job_num, LSA_UINT32 cbf_arg);    /**< callback pointer : callback is executed after the GDMA job has finished */
            LSA_UINT32          cbf_arg;                /**< additional argument, that shall be passed to the callback */
        }out_cr;

        /** input CR specific descriptor. data direction : device (IM) -> controller */
        struct
        {
            LSA_UINT32          *src_addr;              /**< address of the source buffer, from which the CR data will be copied */
            gdma_trigger_src_t  trigger;
            LSA_UINT8           priority;
            LSA_VOID (*job_done_callback)(LSA_INT16 job_num, LSA_UINT32 cbf_arg);
            LSA_UINT32          cbf_arg;
        }in_cr;
    }iom_dma_request_t;
    #endif

#elif defined IOM_CFG_IRTE
    /*
     * KRAM specific declarations
     */

    /** initialization parameters
     *  @see iom_init
     *  @see iom_kram_init */
    typedef struct iom_init_struct_s
    {
        Unsigned8 * unkramed_buffer_address;        /**< specifies the address of SDRAM buffer, allocated for the software module to realize IO data handling */
        LSA_UINT32  unkramed_buffer_size;           /**< specifies the size of SDRAM buffer, allocated for the software module to realize IO data handling */
        LSA_UINT32  properties;                     /**< see defines IOM_KRAM_PROP_USE_CONS_EX_BUFFERS, ... */
        #ifndef EDDI_CFG_USE_SW_IIIB_IF_PROV
        EDD_HANDLE                  hDDB;           /**< only for old 3 buffer provider model */
        #endif

    } iom_init_struct_t;

    /** initialization parameters (see the .properties field of struct iom_init_struct)
     *  @{*/
    #define IOM_KRAM_PROP_USE_CONS_EX_BUFFERS      0x00000001       /**< if set, there will be 2 destination buffers for a consumer to ensure consistency
                                                                         (a "next"-buffer and a "user"-buffer outside KRAM. Switching them is done in
                                                                         iom_kram_consumer_lock() ). Otherwise the KRAM data will be
                                                                         written directly to the "next" buffer (the user accesses the "next buffer directly in this case) */
    #define IOM_KRAM_PROP_FORBID_SECOND_AR         0x00000002       /**< a second AR won't be accepted by the KRAM submodule*/
    /** @}*/

#elif defined IOM_CFG_SWIF
    // initialization parameters
    typedef struct
    {
        LSA_UINT32 h_io_buffer_management;

    } iom_init_struct_t;

#else
    #error "config error : define IOM_CFG_PNIP OR IOM_CFG_IRTE or IOM_CFG_SWIF"
#endif
#include "sys_pck1.h"
typedef struct iom_apdu_status_s
{
    LSA_UINT16 cycle_counter;
    LSA_UINT8  data_status;
    LSA_UINT8  transfer_status;
} iom_apdu_status_t;
#include "sys_unpck.h"
typedef LSA_VERSION_TYPE  * iom_version_info_ptr_t;

/*
 * function declarations for both KRAM and PERIF
 */
LSA_VOID iom_init(iom_init_struct_t *init_data);

LSA_BOOL iom_allocate_iocr_memory(LSA_UINT32 ar_idx, LSA_UINT16 ar_set_nr, LSA_UINT16 session_key, CM_UPPER_SV_AR_CONNECT_IOCR_PTR_TYPE iocr_ptr);

LSA_VOID iom_free_iocr_memory(LSA_UINT32 ar_idx);

LSA_UINT16 iom_consumer_lock(LSA_VOID **buffer, iom_apdu_status_t** apdu_status_ptr, LSA_UINT32 ar_idx);

LSA_UINT16 iom_consumer_unlock(LSA_UINT32 ar_idx);

LSA_VOID   iom_provider_lock(LSA_VOID **buffer, LSA_UINT32 ar_idx);

LSA_VOID   iom_provider_lock_section(LSA_VOID** buffer, LSA_UINT32 ar_idx, LSA_UINT32 offset, LSA_UINT32 length);

LSA_UINT16 iom_provider_unlock(LSA_UINT32 ar_idx);

#ifdef IOM_CFG_RTC3_UNBUFFERED
LSA_UINT16 iom_consumer_lock_unbuffered(LSA_VOID **buffer, iom_apdu_status_t** apdu_status_ptr, LSA_UINT32 ar_idx);

LSA_UINT16 iom_consumer_unlock_unbuffered(LSA_UINT32 ar_idx);

LSA_VOID   iom_provider_lock_unbuffered(LSA_VOID **buffer, LSA_UINT32 ar_idx);

LSA_UINT16 iom_provider_unlock_unbuffered(LSA_UINT32 ar_idx);
#endif

LSA_BOOL iom_get_iocr_buffer_sizes(LSA_UINT32 ar_idx, LSA_UINT32 *input_cr_buffer_size, LSA_UINT32 *output_cr_buffer_size);

LSA_VOID iom_update_consumer_data(LSA_VOID);

LSA_VOID iom_update_provider_data(LSA_VOID);

#ifdef IOM_SYSTEM_REDUNDANCY_SUPPORT
LSA_UINT32  iom_provider_set_data_state     (LSA_UINT32 ar_idx, LSA_UINT8 value, LSA_UINT8 mask);
LSA_VOID    iom_set_session_key_primary_arid(LSA_UINT32 ar_idx);
LSA_UINT16  iom_provider_get_primary_arid   (LSA_UINT16 ARSetID);
#endif

LSA_UINT16 iom_version(LSA_UINT16 version_len, iom_version_info_ptr_t version_ptr);

#if defined IOM_CFG_PNIP

    #ifdef IOM_CFG_USE_GDMA
    LSA_UINT16 iom_install_dma_transfer(LSA_UINT32 ar_idx, LSA_UINT16 iocr_type, iom_dma_request_t *dma_request, LSA_INT16 *gdma_handle);
    #endif

    LSA_UINT16 iom_install_consumer_data_ready_isr(LSA_UINT32 ar_idx, LSA_VOID (*new_data_cbf)(LSA_UINT8));

#elif defined IOM_CFG_IRTE

    LSA_UINT32 iom_provider_set_data_state(LSA_UINT32 ar_idx, LSA_UINT8 value, LSA_UINT8 mask);

    LSA_VOID iom_set_session_key_primary_arid(LSA_UINT32 ar_idx);

#elif defined IOM_CFG_SWIF
    
#endif

/** @name return codes
 *  return values of IO manager functions
 *  @{*/
#define IOM_OK               0          /**< function call returned without error */
#define IOM_OK_NEW_BUFFER    0          /**< specific to lock() calls : -> new data buffer is returned */
#define IOM_OK_OLD_BUFFER    1          /**< specific to lock() calls : -> a data buffer that has been returned before is given back */
#define IOM_BUFFER_UNLOCKED  0xFFFD     /**< the buffer to be unlocked is already unlocked */
#define IOM_BUFFER_LOCKED    0xFFFE     /**< the buffer to be locked is already locked */
#define IOM_INVALID_BUFFER   0xFFFF     /**< the buffer that has been addressed doesn't exist */
#define IOM_PARAMETER_ERROR  0xFF00     /**< a function parameter is wrong */
#define IOM_INTERNAL_ERROR   0xFF01
/** @}*/

#ifdef __cplusplus
}
#endif

#endif /* IOM_COM_H_ */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
