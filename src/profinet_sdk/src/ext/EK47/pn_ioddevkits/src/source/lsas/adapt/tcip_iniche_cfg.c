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
/*  F i l e               &F: tcip_iniche_cfg.c                         :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Implements system integration of TCIP                                    */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
/*                                                                           */
/*****************************************************************************/
#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

#define LSAS_MODULE_ID  31
#define TCIP_MODULE_ID 31

#include "sock_int.h"

#if (SOCK_CFG_USE_TCIP == 1)
#include "tcip_int.h"
#if (TCIP_CFG_INTERNICHE)

#include "ipport.h"
#include "q.h"
#include "netbuf.h"
#include "net.h"
#include "ether.h"
#include "ip.h"
#include "ip_reasm.h"

#if ETHHDR_BIAS != 0
#error "ETHHDR_BIAS must be 0, EDD_SRV_NRT_RECV needs 4 byte alignment"
#endif

#if (ETHHDR_BIAS + EDD_IEEE_FRAME_LENGTH) > EDD_FRAME_BUFFER_LENGTH
#error "check this!"
#endif

char* post_task_setup(void)
{
	/* do nothing */
	return NULL;
}

void clock_c(void)
{
	/* do nothing */
}

void clock_init(void)
{
	/* do nothing */
}

/*===========================================================================*/

char *npalloc(unsigned size)
{
	void *ptr;
	TCIP_ALLOC_LOCAL_MEM(&ptr, (LSA_UINT16)size);
	if (is_not_null(ptr))
	{
		TCIP_MEMSET(ptr, 0, size); /* documentation: buffers returned from npalloc() are assumed to be pre-initialized to all zeros */
	}
	return((char*)ptr);
}

/*---------------------------------------------------------------------------*/

void npfree(void *ptr)
{
	LSA_UINT16  retval;
	TCIP_FREE_LOCAL_MEM(&retval, ptr);
	TCIP_ASSERT(retval == LSA_RET_OK);
}

/*===========================================================================*/

/*===========================================================================*/

#if TCIP_CFG_COPY_ON_SEND == 0

char * npalloc_ethmem(unsigned size) /* alloc & return memory that is accessible by the ethernet controller */
{
	LSA_SYS_PTR_TYPE sys_ptr = { 0 }; /* user must not use it */
	TCIP_EDD_LOWER_MEM_PTR_TYPE ptr;
	TCIP_EDD_ALLOC_TX_TRANSFER_BUFFER_MEM(&ptr, (LSA_UINT16)size, sys_ptr);
	return (char*)ptr;
}

void npfree_ethmem(void* ptr) /* free memory allocated with npalloc_ethmem() */
{
	LSA_SYS_PTR_TYPE sys_ptr = { 0 }; /* user must not use it */
	LSA_UINT16  retval;
	TCIP_EDD_FREE_TX_TRANSFER_BUFFER_MEM(&retval, (TCIP_EDD_LOWER_MEM_PTR_TYPE)ptr, sys_ptr);
	TCIP_ASSERT(retval == LSA_RET_OK);
}

#endif

/*===========================================================================*/

static LSA_FATAL_ERROR_TYPE tcip_iniche_fatal_var; /* for some debuggers that have problems with the stack-trace */

void tcip_iniche_panic(char *file, int line)
{
	TCIP_LOG_FCT("tcip_iniche_panic")

	tcip_iniche_fatal_var.lsa_component_id = LSA_COMP_ID_CM;
	tcip_iniche_fatal_var.module_id        = TCIP_MODULE_ID;
	tcip_iniche_fatal_var.line             = (LSA_UINT16)__LINE__;

	tcip_iniche_fatal_var.error_code[0]    = (LSA_UINT32)file;
	tcip_iniche_fatal_var.error_code[1]    = (LSA_UINT32)line;
	tcip_iniche_fatal_var.error_code[2]    = 0;
	tcip_iniche_fatal_var.error_code[3]    = 0;

	tcip_iniche_fatal_var.error_data_length = 0;
	tcip_iniche_fatal_var.error_data_ptr    = LSA_NULL;

	TCIP_FATAL_ERROR(sizeof(tcip_iniche_fatal_var), &tcip_iniche_fatal_var);
	/* not reached */
}

#ifdef IP_FRAGMENTS
/*----------------------------------------------------------------------------
// delete content of reassembly cache
//--------------------------------------------------------------------------*/
extern IREP h_ireq;
void tcip_iniche_flush_reasm_cache(void *my_channel)
{
	TCIP_CHANNEL_PTR_TYPE channel = (TCIP_CHANNEL_PTR_TYPE)my_channel;
	IREP tmpp;
	IREP nxt_tmpp;

	channel->edd.get_statistics_pending++;

	/* Explanation: if pk_free() is called by ip_reasm_delete_ire(), tcip_iniche_recv_done() will be called.
	Since state of channel == TCIP_CH_CLOSING, tcip_iniche_recv_done() calls tcip_edd_close_channel().
	Incrementing get_statistics_pending here prevents tcip_edd_close_channel() from returning the close rqb.
	*/

	LOCK_NET_RESOURCE(NET_RESID);

	for (tmpp = h_ireq; tmpp; tmpp = nxt_tmpp)
	{
		nxt_tmpp = tmpp->next; /* save the next pointer for the IRE that may be deleted */

		ip_reasm_delete_ire(tmpp);

		++ip_mib.ipReasmFails;
	}

	UNLOCK_NET_RESOURCE(NET_RESID);

	if (channel->edd.get_statistics_pending > 0)
	{
		/* To enable tcip_edd_close_channel(), decrement again. See explanation above. */

		channel->edd.get_statistics_pending--;
	}
	else
	{
		TCIP_FATAL();
	}
}
#endif

#endif // TCIP_CFG_INTERNICHE
#endif 

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
