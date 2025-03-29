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
/*  F i l e               &F: eddp_pck_off.h                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Headerfile to support packing within the EDDP                            */
/*                                                                           */
/*****************************************************************************/
/*
 * no re-include protection!!!
 * this header does a sanity check internally
 */

/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  22.04.09    SF    initial version.                                       */
#endif
/*****************************************************************************/


/*---------------------------------------------------------------------------*/
/*      1.  SANITY CHECK                                                     */
/*---------------------------------------------------------------------------*/


#ifdef EDDP_PCK_OFF_H
    #error "why defined --> check include packon/packoff pair !!!"
#endif


/*---------------------------------------------------------------------------*/
/*      2.  UNDEF PACKED DEFINES                                             */
/*---------------------------------------------------------------------------*/

#if defined (EDDP_PCK_ON_H)
	#undef  EDDP_PCK_ON_H
#endif

#if defined (EDDP_PACKED_ATTRIBUTE_PRE)
	#undef  EDDP_PACKED_ATTRIBUTE_PRE
#endif

#if defined (EDDP_PACKED_ATTRIBUTE_POST)
	#undef  EDDP_PACKED_ATTRIBUTE_POST
#endif


/*---------------------------------------------------------------------------*/
/*      3.  RESTORE LAST PACK                                                */
/*---------------------------------------------------------------------------*/

#if defined (TOOL_CHAIN_MICROSOFT)

    #ifdef PACK_LBL
        #pragma pack(pop, PACK_LBL /*, force_pack*/)
    #endif

    // keep warning disabled to avoid:
    // "alignment changed after including header, may be due to missing #pragma pack(pop)"
    // #pragma warning(default:PRAGMA_PCK_WARN)
    #undef PRAGMA_PCK_WARN

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
#elif defined (TOOL_CHAIN_GNU)

    #pragma pack (0)
#elif defined (TOOL_CHAIN_IAR)

    #pragma pack (0)
#else

	/* packing has to be defined in system adaption */
	#include "sys_unpck.h"

#endif


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
