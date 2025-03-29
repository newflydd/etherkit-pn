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
/*  F i l e               &F: clrpc_pck.h                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Packing macros                                                           */
/*                                                                           */
/*****************************************************************************/
/*
 * no re-include protection!!!
 */
#ifdef CLRPC_PCK_H
#error "why defined?"
#endif


/*
 * included in clrpc_pdu.h which contains PDU definitions
 */

/*---------------------------------------------------------------------------*/
/*      1.  UNDEF PACKED DEFINES                                             */
/*---------------------------------------------------------------------------*/

#if defined (CLRPC_BYTE_PACKED)
	#error "why is it defined?"
#endif

#if defined (CLRPC_BYTE_PACKED_PRE)
	#error "why is it defined?"
#endif

#if defined (CLRPC_WORD_PACKED)
	#error "why is it defined?"
#endif

#if defined (CLRPC_PRAGMA_PACK)
	#error "why is it defined?"
#endif

/*---------------------------------------------------------------------------*/
/*      2.  DEFINE BYTE-PACKED                                               */
/*---------------------------------------------------------------------------*/

#if defined (TOOL_CHAIN_MICROSOFT)

	#define CLRPC_BYTE_PACKED /* nothing, see pragma below */
	#define CLRPC_BYTE_PACKED_PRE /* nothing, see pragma below */

#elif defined (TOOL_CHAIN_TASKING_TRICORE)

	#define CLRPC_BYTE_PACKED /* nothing, see pragma below */
	#define CLRPC_BYTE_PACKED_PRE /* nothing, see pragma below */

#elif defined (TOOL_CHAIN_GNU_PPC)

    #define CLRPC_BYTE_PACKED  __attribute__ ((packed))
	#define CLRPC_BYTE_PACKED_PRE /* no prefix */

#elif defined (TOOL_CHAIN_CC386)

	#define CLRPC_BYTE_PACKED /* nothing, see pragma below */
	#define CLRPC_BYTE_PACKED_PRE /* nothing, see pragma below */

#elif defined (TOOL_CHAIN_GREENHILLS_ARM)

	#define CLRPC_BYTE_PACKED /* nothing, see pragma below */
	#define CLRPC_BYTE_PACKED_PRE /* nothing, see pragma below */

#elif defined (TOOL_CHAIN_GREENHILLS_MIPS)

    #define CLRPC_BYTE_PACKED /* nothing, see pragma below */
    #define CLRPC_BYTE_PACKED_PRE /* nothing, see pragma below */

#elif defined (TOOL_CHAIN_NRK) /* ARM/Thumb C/C++ Compiler */

	#define CLRPC_BYTE_PACKED     /* no postfix */
	#define CLRPC_BYTE_PACKED_PRE __packed

#else

	/* packing has to be defined in system adaption */
	#include "sys_pck1.h"
	#define CLRPC_BYTE_PACKED       SYS_BYTE_PACKED
	#define CLRPC_BYTE_PACKED_PRE   SYS_BYTE_PACKED_PRE 

#endif

#ifdef CLRPC_WORD_PACKED
#error "why is it defined?"
#endif

/*----------------------------------------------------------------------*/
/*      3.  SET BYTE-ALIGNMENT                                          */
/*----------------------------------------------------------------------*/

#if defined (TOOL_CHAIN_MICROSOFT)

	/*
	 * doing pack here gives a level 1 warning
	 * but luckily "The argument to #pragma is subject to macro expansion"
	 */

	#define CLRPC_PRAGMA_PACK(label) pack( push, label, 1/*byte*/ )

#elif defined (TOOL_CHAIN_TASKING_TRICORE)

	#pragma pack 2 /* ! tricore cannot byte-packed ! */

#elif defined (TOOL_CHAIN_GNU_PPC)

	/* does not use pragma, see define above */

#elif defined (TOOL_CHAIN_CC386)

    #pragma noalign

#elif defined (TOOL_CHAIN_GREENHILLS_ARM)

	#pragma pack(1) /* 1=byte 2=short 4=long alignment */

#elif defined (TOOL_CHAIN_GREENHILLS_MIPS)

    #pragma pack(1) /* 1=byte 2=short 4=long alignment */

#elif defined (TOOL_CHAIN_NRK)

	/* does not use pragma, see define above */
#elif defined (TOOL_CHAIN_IAR)

    #pragma pack(1)
#else
	/* packing has to be defined in system adaption */
	/* including "sys_pck1.h" */

#endif

/*---------------------------------------------------------------------------*/
/*      4.  SANITY CHECK                                                     */
/*---------------------------------------------------------------------------*/

#if defined (CLRPC_BYTE_PACKED) && defined (CLRPC_WORD_PACKED)
	#error "both not possible! see pragmas"
#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
