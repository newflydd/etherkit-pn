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
/*  F i l e               &F: clrpc_unpck.h                             :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Unpacking macros                                                         */
/*                                                                           */
/*****************************************************************************/
/*
 * no re-include protection!!!
 */
#ifdef CLRPC_UNPCK_H
#error "why defined?"
#endif


/*
 * included in clrpc_pdu.h which contains PDU definitions
 */

/*---------------------------------------------------------------------------*/
/*      1.  UNDEF PACKED DEFINES                                             */
/*---------------------------------------------------------------------------*/

#if defined (CLRPC_BYTE_PACKED) && defined (CLRPC_WORD_PACKED)
    #error "how this?"
#endif

#if defined (CLRPC_BYTE_PACKED)
    #undef  CLRPC_BYTE_PACKED
#endif

#if defined (CLRPC_BYTE_PACKED_PRE)
	#undef  CLRPC_BYTE_PACKED_PRE
#endif

#if defined (CLRPC_WORD_PACKED)
    #undef  CLRPC_WORD_PACKED
#endif

#if defined (CLRPC_PRAGMA_PACK)
	#undef CLRPC_PRAGMA_PACK
	#undef CLRPC_PRAGMA_UNPACK /* see below */
#endif

/*---------------------------------------------------------------------------*/
/*      2.  SET NO-ALIGNMENT                                                 */
/*---------------------------------------------------------------------------*/

#if defined (TOOL_CHAIN_MICROSOFT)

	/*
	 * doing pack here gives a level 1 warning
	 * but luckily "The argument to #pragma is subject to macro expansion"
	 */

	#define CLRPC_PRAGMA_UNPACK(label) pack( pop, label)

#elif defined (TOOL_CHAIN_TASKING_TRICORE)

	#pragma pack 0

#elif defined (TOOL_CHAIN_GNU_PPC)

	/* does not use pragma */

#elif defined (TOOL_CHAIN_CC386)

    #pragma align

#elif defined (TOOL_CHAIN_GREENHILLS_ARM)

	#pragma pack()

#elif defined (TOOL_CHAIN_GREENHILLS_MIPS)

    #pragma pack()

#elif defined (TOOL_CHAIN_NRK)

	/* does not use pragma */
#elif defined(TOOL_CHAIN_IAR)
	#pragma pack()
#else

	/* packing has to be defined in system adaption */
	#include "sys_unpck.h"

#endif


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
