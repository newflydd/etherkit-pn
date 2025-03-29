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
/*  C o m p o n e n t     &C: TCPIP (TCIP for Interniche Stack)         :C&  */
/*                                                                           */
/*  F i l e               &F: tcip_sys.c                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Implements the LSA system-interface                                      */
/*                                                                           */
/*****************************************************************************/
#define LTRC_ACT_MODUL_ID  1
#define TCIP_MODULE_ID     1

#include "tcip_int.h"
#include "anyip_sys.h"
#include "anyip_tcip.h"

#if TCIP_CFG_SNMP_ON && (TCIP_CFG_OPEN_BSD || TCIP_CFG_USE_OBSD_SNMPD)
#include "obsd_snmpd_snmpdext.h"
#include "obsd_platform_mibal.h"
#endif

TCIP_FILE_SYSTEM_EXTENSION(TCIP_MODULE_ID) /* no semicolon */

/*===========================================================================*/

TCIP_DATA tcip_data; /* global data of TCIP */

/*===========================================================================*/


/*------------------------------------------------------------------------------
//	func: tcip_init
//	work: initialises the module, must be called on startup
//----------------------------------------------------------------------------*/
LSA_UINT16 tcip_init ( LSA_VOID )
{
	TCIP_LOG_FCT("tcip_init")
	LSA_INT must;

#ifdef TCIP_MESSAGE
#pragma TCIP_MESSAGE("LSA_FALSE is defined as '" TCIP_STRINGIFY(LSA_FALSE) "'")
#pragma TCIP_MESSAGE("LSA_TRUE is defined as '" TCIP_STRINGIFY(LSA_TRUE) "'")
#endif /* TCIP_MESSAGE */


	must = ( LSA_FALSE == 0 && LSA_TRUE != LSA_FALSE );

	if( ! must )
	{
		TCIP_UPPER_TRACE_00(0, LSA_TRACE_LEVEL_FATAL , "strange TRUE/FALSE" );
		TCIP_FATAL();
	}

	/*
	 * but: LSA_NULL needs not to be 0. some may set it to -1 or some other special value!
	 *
	 * don't: if( LSA_NULL == 0 ) ...
	 */

	#if 0 /* show #define of LSA_NULL */
	#ifdef TCIP_MESSAGE
	#pragma TCIP_MESSAGE("LSA_NULL is defined as '" TCIP_STRINGIFY(LSA_NULL) "'")
	#endif /* TCIP_MESSAGE */
	#endif

	#ifdef TCIP_MESSAGE
	/*#pragma TCIP_MESSAGE("VV: test with LSA_NULL != 0")*/
	/*currently no one dares to do this (because of lack of a clean c-lib)*/
	#endif /* TCIP_MESSAGE */

	/***/

	if (anyip_init() != 0)
	{
		TCIP_UPPER_TRACE_00(0, LSA_TRACE_LEVEL_FATAL, "Error: anyip_init() failed!");
		TCIP_FATAL();
	}

#if (ANYIP_CFG_USE_TCPKEEPTIME == 1)
#ifdef TCIP_CFG_KEEPALIVE_IDLE_TIMEOUT
	if (anyip_tcip_set_tcp_keepidle(TCIP_CFG_KEEPALIVE_IDLE_TIMEOUT) != 0)
	{
		TCIP_UPPER_TRACE_00(0, LSA_TRACE_LEVEL_FATAL, "Error: anyip_set_tcp_keepidle failed!");
		TCIP_FATAL();
	}
#endif

#ifdef TCIP_CFG_KEEPALIVE_PROBE_INTERVAL
	if (anyip_tcip_set_tcp_keepintvl(TCIP_CFG_KEEPALIVE_PROBE_INTERVAL) != 0)
	{
		TCIP_UPPER_TRACE_00(0, LSA_TRACE_LEVEL_FATAL, "Error: anyip_set_tcp_keepintvl failed!");
		TCIP_FATAL();
	}
#endif

#ifdef TCIP_CFG_KEEPALIVE_PROBE_COUNT
	if (anyip_tcip_set_tcp_keepcount(TCIP_CFG_KEEPALIVE_PROBE_COUNT) != 0)
	{
		TCIP_UPPER_TRACE_00(0, LSA_TRACE_LEVEL_FATAL, "Error: anyip_set_tcp_keepcount failed!");
		TCIP_FATAL();
	}
#endif

#ifdef TCIP_CFG_CONNECTION_TIMEOUT
	if (anyip_tcip_set_tcp_keepinittime(TCIP_CFG_CONNECTION_TIMEOUT) != 0)
	{
		TCIP_UPPER_TRACE_00(0, LSA_TRACE_LEVEL_FATAL, "Error: anyip_tcip_set_tcp_keepinittime failed!");
		TCIP_FATAL();
	}
#endif
#endif /* (ANYIP_CFG_USE_TCPKEEPTIME == 1) */

	/***/

	tcip_data.tcip_channels_opened = 0;

	{
	LSA_UINT16 i;

	/* init the channel table */
	for (i = 0; i < TCIP_MAX_CHANNELS; ++i)
	{
		tcip_data.tcip_channels[i].state = TCIP_CH_FREE;
	}
	}

	/*
	 * alloc timer
	 */

	{
	TCIP_UPPER_RQB_PTR_TYPE tim = &tcip_data.tcip_TimerRQB;

	TCIP_RQB_SET_OPCODE(tim, TCIP_OPC_TIMER);
	TCIP_RQB_SET_HANDLE(tim, TCIP_INVALID_HANDLE); /* not used */
	TCIP_RQB_SET_USER_ID_PTR(tim, 0); /* not used */

	TCIP_EXCHANGE_LONG(&tim->args.timer.in_use, 0);
	}

	{
	LSA_UINT16 retVal;
	TCIP_ALLOC_TIMER(&retVal, &tcip_data.tcip_TimerID, LSA_TIMER_TYPE_CYCLIC, LSA_TIME_BASE_100MS); /* see too TCIP_START_TIMER() */
	TCIP_ASSERT(LSA_RET_OK == retVal);
	}

	/***/

	tcip_data.get_statistics_pending = 0;

	/***/

#if TCIP_CFG_SNMP_ON && (TCIP_CFG_OPEN_BSD || TCIP_CFG_USE_OBSD_SNMPD) /* OpenBSD SNMPD uses TCIP Memory --> must be initialized first */
	snmpd_main(0, LSA_NULL); /* initialize obsd snmp "daemon" */
#endif

	return LSA_RET_OK;
}


/*------------------------------------------------------------------------------
//	func: tcip_undo_init
//	work: de-initialises the module, must be call on shutdown
//----------------------------------------------------------------------------*/
LSA_UINT16
tcip_undo_init(
	LSA_VOID
) {
	TCIP_LOG_FCT ("tcip_undo_init")

#if TCIP_CFG_SNMP_ON && (TCIP_CFG_OPEN_BSD || TCIP_CFG_USE_OBSD_SNMPD) /* OpenBSD SNMPD uses TCIP Memory --> must be initialized first */
	obsd_snmpd_shutdown();
#endif

	if( tcip_data.tcip_channels_opened != 0 )
	{
		TCIP_UPPER_TRACE_00(0, LSA_TRACE_LEVEL_FATAL, "not all channels closed" );
		TCIP_FATAL();
	}

	/*
	 *   check state: all channels closed ?
	 */
	{
	LSA_UINT16 i;

	for (i = 0; i < TCIP_MAX_CHANNELS; ++i)
	{
		TCIP_ASSERT(tcip_data.tcip_channels[i].state == TCIP_CH_FREE);

	}
	}

	/***/

	anyip_deinit();

	/***/

	/*
	 *  free timer
	 */
	{
	LSA_UINT16 retVal;
	TCIP_FREE_TIMER(&retVal, tcip_data.tcip_TimerID);
	TCIP_ASSERT(LSA_RET_OK == retVal);
	}

	/***/

	return LSA_RET_OK;
}


/*------------------------------------------------------------------------------
//	func: tcip_timeout
//	work:
//----------------------------------------------------------------------------*/
LSA_VOID
tcip_timeout ( LSA_TIMER_ID_TYPE  timer_id, LSA_USER_ID_TYPE  user_id )
{
	/* runs in context of the LSA timer thread */

	if( timer_id != tcip_data.tcip_TimerID )
	{
		TCIP_FATAL();
	}
	else {

		LSA_UNUSED_ARG(user_id);

		if( TCIP_EXCHANGE_LONG(&tcip_data.tcip_TimerRQB.args.timer.in_use, 1) != 0 )
		{
			/* skip this tick */
		}
		else
		{
			TCIP_REQUEST_LOCAL(&tcip_data.tcip_TimerRQB);
		}
	}
}


/*------------------------------------------------------------------------------
//	func: tcip_fatal
//----------------------------------------------------------------------------*/

static LSA_FATAL_ERROR_TYPE tcip_fatal_var; /* for some debuggers that have problems with the stack-trace */

LSA_VOID
tcip_fatal(
	LSA_UINT16 tcip_module_id,
	LSA_INT line
) {
	tcip_fatal1(tcip_module_id, line, 0);
}


LSA_VOID
tcip_fatal1(
	LSA_UINT16 tcip_module_id,
	LSA_INT line,
	LSA_UINT32 ec_0
) {
	TCIP_LOG_FCT("tcip_fatal1")

	TCIP_UPPER_TRACE_03(0, LSA_TRACE_LEVEL_FATAL
		, "module_id(%u) line(%u) ec_0(0x%x)"
		, tcip_module_id, line, ec_0
		);

	tcip_fatal_var.lsa_component_id = LSA_COMP_ID_TCIP;
	tcip_fatal_var.module_id        = tcip_module_id;
	tcip_fatal_var.line             = (LSA_UINT16)line;

	tcip_fatal_var.error_code[0]    = ec_0;
	tcip_fatal_var.error_code[1]    = 0;
	tcip_fatal_var.error_code[2]    = 0;
	tcip_fatal_var.error_code[3]    = 0;

	tcip_fatal_var.error_data_length = 0;
	tcip_fatal_var.error_data_ptr    = LSA_NULL;

	TCIP_FATAL_ERROR(sizeof(tcip_fatal_var), &tcip_fatal_var);
	/* not reached */
}

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
