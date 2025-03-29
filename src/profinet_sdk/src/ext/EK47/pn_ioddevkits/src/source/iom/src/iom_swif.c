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
/*  F i l e               &F: iom_swif.c                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/* software module accessing the EDDS software buffer modell                 */
/*                                                                           */
/*****************************************************************************/

#define IOM_LOCAL_ERR_MODULE IOM_MODULE_SWIF

#include "iom_inc.h"
#include "iom_com.h"
#include "iom_int.h"

#ifdef IOM_CFG_SWIF

#include "edds_int.h"

/* ------------------------------------- internal global variables ------------------------------------ */

typedef struct iom_data_s
{
    LSA_UINT16	consumer_cr_number[IOM_CFG_SWIF_NUM_AR];
    LSA_UINT16	provider_cr_number[IOM_CFG_SWIF_NUM_AR];

    LSA_UINT32	consumer_data_offset[IOM_CFG_SWIF_NUM_AR];
    LSA_UINT32	provider_data_offset[IOM_CFG_SWIF_NUM_AR];

    LSA_UINT32	consumer_apdu_offset[IOM_CFG_SWIF_NUM_AR];
    LSA_UINT32	provider_apdu_offset[IOM_CFG_SWIF_NUM_AR];

    LSA_UINT16	consumer_cr_cnt;
    LSA_UINT16	provider_cr_cnt;

	EDDS_IOBUFFER_HANDLE_TYPE	h_io_buffer;

} iom_data_t;

static iom_data_t iom_data;


/* ----------------------------------------- helper functions ----------------------------------------- */
static LSA_VOID   iom_swif_in_fatal_error( LSA_UINT16 error_module, LSA_UINT16 error_line, LSA_UINT32 error_code);



/* ----------------------------------------- user functions ----------------------------------------- */


/**************************************************************************************************
 * @brief   This function initializes all meta data fields describing ARs and CRs.
 * 			No external services are called from here (ISR registration for example).
 *************************************************************************************************/
LSA_VOID iom_swif_init(iom_init_struct_t *init_data)
{
	// init_data->h_io_buffer_management is returned by EDDS_SRV_DEV_COMP_SRT_INI
	LSA_UINT16 mngm_id = 0; 
	iom_data.h_io_buffer = EDDS_IOBuffer_Init ((EDD_COMMON_MEM_PTR_TYPE) init_data->h_io_buffer_management, mngm_id,
											   &iom_data.consumer_cr_cnt, &iom_data.provider_cr_cnt);
}

/**************************************************************************************************
 * @brief   This function allocates memory for a new CR.
 *          - only paged mode is used to prevent fragmentation
 *          - 3 buffer mode is used
 *
 * @param ar_idx        [in] contains the AR number for which a new CR shall be created. Zero based value.
 * @param iocr_ptr      [in] contains a pointer to a structure describing the CR. Evaluated fields are
 *                      io_buffer_size, iocr_type. The following fields are set: eddx_data_offset, eddp_cr_number, eddp_forward_mode
 * @return              returns true, if everything went well
 *************************************************************************************************/
LSA_BOOL iom_swif_allocate_iocr_memory (LSA_UINT32 ar_idx,
										LSA_UINT16 ar_set_nr,
										LSA_UINT16 session_key,
										CM_UPPER_SV_AR_CONNECT_IOCR_PTR_TYPE iocr_ptr)
{
	IOM_ENTER();

	IOM_ASSERT(ar_idx < IOM_CFG_SWIF_NUM_AR, ar_idx);

	switch (iocr_ptr->iocr_type)
	{
		case CM_IOCR_TYPE_INPUT:
		{
			LSA_UINT16 cr_nr = EDDS_IOBuffer_ProviderAlloc (iom_data.h_io_buffer, iocr_ptr->io_buffer_size,
															EDDS_CSRT_PROVIDER_TYPE_XRT, (IOD_INCLUDE_S2_REDUNDANCY));
			if (EDD_CRNUMBER_UNDEFINED == cr_nr)
			{
				IOM_EXIT_RETURN(FALSE);
			}
			iocr_ptr->eddx_cr_number 			  = cr_nr;
			iom_data.provider_cr_number  [ar_idx] = cr_nr;
			iom_data.provider_data_offset[ar_idx] = 0;
			iom_data.provider_apdu_offset[ar_idx] = iocr_ptr->io_buffer_size;
			// clean provider buffer with '0'
			{
				void* provider_buffer = EDDS_IOBuffer_ProviderLockNew (iom_data.h_io_buffer, iom_data.provider_cr_number[ar_idx]);
				IOM_MEMSET(provider_buffer, 0, iocr_ptr->io_buffer_size + 4 /*APDU*/);
				EDDS_IOBuffer_ProviderUnlock (iom_data.h_io_buffer, iom_data.provider_cr_number[ar_idx]);
			}
			break;
		}
		case CM_IOCR_TYPE_OUTPUT:
		{
			LSA_UINT16 cr_nr = EDDS_IOBuffer_ConsumerAlloc (iom_data.h_io_buffer,iocr_ptr->io_buffer_size,
															EDDS_CSRT_CONSUMER_TYPE_XRT, NULL);
			if (EDD_CRNUMBER_UNDEFINED == cr_nr)
			{
				IOM_EXIT_RETURN(FALSE);
			}
			iocr_ptr->eddx_cr_number 			  = cr_nr;
			iom_data.consumer_cr_number  [ar_idx] = cr_nr;
			iom_data.consumer_data_offset[ar_idx] = 0;
			iom_data.consumer_apdu_offset[ar_idx] = iocr_ptr->io_buffer_size;
			break;
		}
		default:
		{
			iom_swif_in_fatal_error(IOM_LOCAL_ERR_MODULE, (LSA_UINT16) __LINE__, iocr_ptr->iocr_type);
			break;
		}
	}

	IOM_EXIT_RETURN(TRUE);
}


/**************************************************************************************************
 * @brief   This function releases all IO buffer resources, allocated by the CRs of an AR synchronously.
 *
 *          AR-/CR meta data is reset.
 *
 * @param ar_idx    [in] AR that is to be released
 *************************************************************************************************/
LSA_VOID iom_swif_free_iocr_memory(LSA_UINT32 ar_idx)
{
	IOM_ASSERT(ar_idx < IOM_CFG_SWIF_NUM_AR, ar_idx);

	if (EDD_CRNUMBER_UNDEFINED != iom_data.provider_cr_number[ar_idx])
	{
		EDDS_IOBuffer_ProviderFree (iom_data.h_io_buffer, iom_data.provider_cr_number[ar_idx]);
		iom_data.provider_cr_number[ar_idx] = EDD_CRNUMBER_UNDEFINED;
	}
	if (EDD_CRNUMBER_UNDEFINED != iom_data.consumer_cr_number[ar_idx])
	{
		EDDS_IOBuffer_ConsumerFree (iom_data.h_io_buffer, iom_data.consumer_cr_number[ar_idx]);
		iom_data.consumer_cr_number[ar_idx] = EDD_CRNUMBER_UNDEFINED;
	}
}


/**************************************************************************************************
 * @brief This function retrieves and locks a buffer of an output CR.
 *        No other task can lock the buffer again (prior to that an unlock is needed).
 *
 * @param consumer_buffer   [out] address of the current PERIF-user-CR buffer
 * @param apdu_status_ptr   [out] address within the PNIP, where the APDU status can be found
 * @param ar_idx            [in]  index of the AR whose output buffer should be retrieved
 * @return                  IOM_INVALID_BUFFER if the output CR is not initialized,
 *                          IOM_BUFFER_LOCKED if the CR memory is already locked,
 *                          IOM_OK_NEW_BUFFER or IOM_OK_OLD_BUFFER if locking was successful
 *************************************************************************************************/
LSA_UINT16 iom_swif_consumer_lock(LSA_VOID** consumer_buffer, iom_apdu_status_t** apdu_status_ptr, LSA_UINT32 ar_idx)
{
	LSA_UINT32      buffer_start;

	IOM_ENTER();

	IOM_ASSERT(ar_idx < IOM_CFG_SWIF_NUM_AR, ar_idx);

	buffer_start = (LSA_UINT32) EDDS_IOBuffer_ConsumerLock (iom_data.h_io_buffer, iom_data.consumer_cr_number[ar_idx]);

	if (LSA_NULL == buffer_start)
	{
		*consumer_buffer  = NULL;
		*apdu_status_ptr  = NULL;
	    IOM_EXIT_RETURN(IOM_INVALID_BUFFER);
	}
	*consumer_buffer = (LSA_VOID*)(buffer_start + iom_data.consumer_data_offset[ar_idx]);
	*apdu_status_ptr = (LSA_VOID*)(buffer_start + iom_data.consumer_apdu_offset[ar_idx]);

    IOM_EXIT_RETURN(IOM_OK_NEW_BUFFER);
}


/**************************************************************************************************
 * @brief   This function unlocks a buffer previously locked by iom_swif_consumer_lock(). iom_swif_consumer_unlock()
 *          may not be called if a previous call to iom_swif_consumer_lock() returned with an error.
 *
 * @param ar_idx    [in] specifies the AR that contains the output-CR to be unlocked
 * @return          iom_swif_INVALID_BUFFER if the output CR is not initialized, iom_swif_BUFFER_UNLOCKED if the buffer
 *                  isn't in the locked state, else iom_swif_OK. Here you get no destinction between old and new buffers.
 *************************************************************************************************/
LSA_UINT16 iom_swif_consumer_unlock(LSA_UINT32 ar_idx)
{
	IOM_ENTER();

	IOM_ASSERT(ar_idx < IOM_CFG_SWIF_NUM_AR, ar_idx);

	EDDS_IOBuffer_ConsumerUnlock (iom_data.h_io_buffer, iom_data.consumer_cr_number[ar_idx]);

	IOM_EXIT_RETURN(IOM_OK);
}


/**************************************************************************************************
 * @brief  This function retrieves and locks a buffer of an input CR
 *
 * @param provider_buffer   [out] provider_buffer is set according to the buffer address of the AR's input CR
 * @param ar_idx            [in]  ar_idx specifies the AR whose input-CR-buffer shall be locked
 *************************************************************************************************/
LSA_VOID iom_swif_provider_lock(void **provider_buffer, LSA_UINT32 ar_idx)
{
	IOM_ENTER();

	IOM_ASSERT(ar_idx < IOM_CFG_SWIF_NUM_AR, ar_idx);
	IOM_ASSERT(provider_buffer, (LSA_UINT32) ar_idx);

	*provider_buffer = EDDS_IOBuffer_ProviderLockNew (iom_data.h_io_buffer, iom_data.provider_cr_number[ar_idx]);

	IOM_EXIT();
}


/**************************************************************************************************
 * @brief   This function unlocks a buffer previously locked provider buffer.
 *
 * @param ar_idx    [in] specifies the AR whose input-CR-buffer is ready for transmission
 * @return          IOM_INVALID_BUFFER if the output CR is not initialized, IOM_BUFFER_UNLOCKED if the buffer
 *                  isn't in the locked state. IOM_OK_NEW_BUFFER if none of the above errors occured (the Perif can
 *                  always update the next provider buffer in contrast to the EDDI)
 *                  In DMA-mode, IOM_OK is always returned.
 *************************************************************************************************/
LSA_UINT16 iom_swif_provider_unlock(LSA_UINT32 ar_idx)
{
	IOM_ENTER();

	IOM_ASSERT(ar_idx < IOM_CFG_SWIF_NUM_AR, ar_idx);

	EDDS_IOBuffer_ProviderUnlock (iom_data.h_io_buffer, iom_data.provider_cr_number[ar_idx]);

	IOM_EXIT_RETURN(IOM_OK);
}


/**************************************************************************************************
 * @brief   This function returns the input- and output-CR buffer sizes (4 byte granularity)
 *
 * @param ar_idx                [in] AR for which the buffer sizes shall be returned
 * @param input_cr_buffer_size  [out] buffer size of the input CR
 * @param output_cr_buffer_size [out] buffer size of the output CR
 *
 * @return  LSA_TRUE if no problems occured; LSA_FALSE otherwise
 *************************************************************************************************/
LSA_BOOL iom_swif_get_iocr_buffer_sizes(LSA_UINT32 ar_idx, LSA_UINT32 *input_cr_buffer_size, LSA_UINT32 *output_cr_buffer_size)
{
    IOM_ENTER();

	IOM_ASSERT(ar_idx < IOM_CFG_SWIF_NUM_AR, ar_idx);

    *input_cr_buffer_size  = iom_data.provider_apdu_offset[ar_idx];
	*output_cr_buffer_size = iom_data.consumer_apdu_offset[ar_idx];

    IOM_EXIT_RETURN(LSA_TRUE);
}


static LSA_FATAL_ERROR_TYPE iom_error_descriptor;

/**************************************************************************************************
 * @brief   Fatal error logging.
 *
 * @param error_module      [in]	Module ID
 * @param error_line  		[in]	Line the error occurred
 * @param error_code 		[in]	Error code
 *
 * @return  LSA_TRUE if no problems occured; LSA_FALSE otherwise
 *************************************************************************************************/
static LSA_VOID iom_swif_in_fatal_error( LSA_UINT16 error_module, LSA_UINT16 error_line, LSA_UINT32 error_code)
{
    iom_error_descriptor.lsa_component_id = IOM_LSA_COMPONENT_ID;

    iom_error_descriptor.module_id        = error_module;
    iom_error_descriptor.line             = error_line;

    iom_error_descriptor.error_code[ 0]   = error_code;
    iom_error_descriptor.error_code[ 1]   =
    iom_error_descriptor.error_code[ 2]   =
    iom_error_descriptor.error_code[ 3]   = 0;

    iom_error_descriptor.error_data_length = sizeof( iom_data);
    iom_error_descriptor.error_data_ptr    =        &iom_data;

    IOM_FATAL_ERROR( &iom_error_descriptor);
}

#endif // #ifdef IOM_CFG_SWIF

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
