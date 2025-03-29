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
/*  C o m p o n e n t     &C: ACP (Alarm Consumer Provider)             :C&  */
/*                                                                           */
/*  F i l e               &F: acp_sys.c                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Implements the system LSA interface                                      */
/*                                                                           */
/*****************************************************************************/
#define LTRC_ACT_MODUL_ID  1
#define ACP_MODULE_ID      LTRC_ACT_MODUL_ID

#include "acp_int.h"

ACP_FILE_SYSTEM_EXTENSION(ACP_MODULE_ID)

/*------------------------------------------------------------------------------
//	globals
//----------------------------------------------------------------------------*/
ACP_CHANNEL						acp_channels[ACP_CFG_MAX_CHANNELS];
ACP_LOWER_TYPE					acp_lowers[ACP_CFG_MAX_CHANNELS];
LSA_USER_ID_TYPE				acp_null_usr_id;

/*------------------------------------------------------------------------------
//	initialise the module ACP, must call once on startup
//----------------------------------------------------------------------------*/
LSA_UINT16
acp_init (
	ACP_INIT_CONST_PTR_TYPE  init
	)
{
	LSA_INT  i;

	if (acp_is_null(init))
	{
		ACP_FATAL(0);
		return LSA_RET_ERR_PARAM;
	}

	{
		/* --e{#} For entire current {region} */
		/*lint --e{413} likely use of null pointer */

		/* check RQB vs double-linked-list */
		if ((LSA_INT)& ACP_RQB_GET_NEXT_RQB_PTR((ACP_UPPER_RQB_PTR_TYPE)0) != ACP_OFFS(ACP_LIST_ENTRY_TYPE, Flink))
		{
			ACP_FATAL(0);
		}

		if ((LSA_INT)& ACP_RQB_GET_PREV_RQB_PTR((ACP_UPPER_RQB_PTR_TYPE)0) != ACP_OFFS(ACP_LIST_ENTRY_TYPE, Blink))
		{
			ACP_FATAL(0);
		}

	}

	acp_null_usr_id.void_ptr = LSA_NULL;

	for (i = 0; i < ACP_CFG_MAX_CHANNELS; ++i)
	{
		acp_channels[i].state = ACP_CHANNEL_STATE_FREE;
		acp_lowers[i].count_of_open_channel = 0;
	}

	acp_oneshot_sys_init (init->rta_time_base, init->rta_time_factor);

	return LSA_RET_OK;
}


/*------------------------------------------------------------------------------
//	uninitialise the module ACP
//----------------------------------------------------------------------------*/
LSA_UINT16
acp_undo_init (
    LSA_VOID
	)
{
	LSA_UINT16  i;


	for (i = 0; i < ACP_CFG_MAX_CHANNELS; ++i) {
		ACP_ASSERT (acp_channels[i].state == 0);
	}

	acp_oneshot_sys_undo_init();

    return LSA_RET_OK;
}


/*------------------------------------------------------------------------------
//	fills the global fatal-error structure and call ACP_FATAL_ERROR
//----------------------------------------------------------------------------*/
static LSA_FATAL_ERROR_TYPE  acp_glob_fatal_error;

LSA_VOID
acp_fatal_error (
	LSA_UINT16         module_id,
	LSA_UINT16         line,
	LSA_UINT32         error_code_0,
	LSA_UINT32         error_code_1,
	LSA_UINT32         error_code_2,
	LSA_UINT32         error_code_3,
	LSA_UINT16         error_data_len,
	LSA_VOID_PTR_TYPE  error_data
	)
{
	acp_glob_fatal_error.lsa_component_id	= LSA_COMP_ID_ACP;
	acp_glob_fatal_error.module_id			= module_id;
	acp_glob_fatal_error.line				= line;

	acp_glob_fatal_error.error_code[0]		= error_code_0;
	acp_glob_fatal_error.error_code[1]		= error_code_1;
	acp_glob_fatal_error.error_code[2]		= error_code_2;
	acp_glob_fatal_error.error_code[3]		= error_code_3;

	acp_glob_fatal_error.error_data_length	= error_data_len;
	acp_glob_fatal_error.error_data_ptr		= error_data;

	ACP_FATAL_ERROR ((LSA_UINT16)(sizeof(acp_glob_fatal_error)), &acp_glob_fatal_error);
}


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
