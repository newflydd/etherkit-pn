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
/*  F i l e               &F: pnio_pck4_on.h                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Headerfile to support packing within PNIO components                     */
/*                                                                           */
/*  -- DEFINITIONS FOR 4-BYTE-PACKING --                                     */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
/*****************************************************************************/

/*
 * no re-include protection!!!
 * this header does a sanity check internally
 */




/*---------------------------------------------------------------------------*/
/*      0.  USAGE                                                            */
/*---------------------------------------------------------------------------*/

/*
There are some Toolchains (Microsoft, GNU, ...) supported in these packing headers. 
In combination with the attribute keywords the packing machanism of every compiler
should be supported. If the compiler does'nt use the attribute keywords, than
define these keywords as empty defines. Special pragma instructions can be placed
into the pack_on and pack_off-header the hide these settings internally.

1)
    Select at first one of the predefined Toolchain or add a new one if not existing here.
    --> #define TOOL_CHAIN_MICROSOFT
    Note:   For the development kits do NOT use TOOL_CHAIN_xxxxx, but use PNIOD_PLATFORM_xxxxx,
    to select one of the predefined platforms or add a new one if not existing here.

2)
    include the pnio_pckx_on.h at the beginning of the structure. "x" stands for 1,2 or 4
3)
    Use the defined keywords "PNIO_PACKED_ATTRIBUTE_PRE" and "PNIO_PACKED_ATTRIBUTE_POST"
    to encapsulate the structure.
4)
    include the pnio_pck_off.h at the end of the structure
5)
    example:
    ========

    #include "pnio_pck4_on.h"

    typedef PNIO_PACKED_ATTRIBUTE_PRE struct _PNIO_PRM_RECORD_HEADER_TYPE
    {
        LSA_UINT16          BlockType;
        LSA_UINT16          BlockLength; 
        LSA_UINT8           BlockVersionHigh;
        LSA_UINT8           BlockVersionLow;
        LSA_UINT8           Padding1_1;
        LSA_UINT8           Padding1_2;
    } PNIO_PRM_RECORD_HEADER_TYPE PNIO_PACKED_ATTRIBUTE_POST;

    #include "pnio_pck_off.h"

*/



/*---------------------------------------------------------------------------*/
/*      1.  SANITY CHECK                                                     */
/*---------------------------------------------------------------------------*/


#ifdef PNIO_PCK_ON_H
    #error "why defined --> check include packon/packoff pair !!!"
#endif

/*---------------------------------------------------------------------------*/
/*      2.  DEFINE DWORD-PACKED                                              */
/*---------------------------------------------------------------------------*/

#if defined (TOOL_CHAIN_MICROSOFT)

	#define PNIO_PACKED_ATTRIBUTE_PRE   /* nothing, see pragma below */
	#define PNIO_PACKED_ATTRIBUTE_POST  /* nothing, see pragma below */

	/*
	 * doing pack here gives a level 1 warning
     * --> disable this warning and keep it disabled to avoid "warning: pack changed within a header file"
	 */
    #define PRAGMA_PCK_WARN 4103
    #pragma warning( disable : PRAGMA_PCK_WARN )
    #define PACK_LBL PACK32_LBL
    #pragma pack(push, PACK_LBL, 4/*32bit*/)

#elif defined (TOOL_CHAIN_TASKING_TRICORE)

	#define PNIO_PACKED_ATTRIBUTE_PRE   /* nothing, see pragma below */
	#define PNIO_PACKED_ATTRIBUTE_POST  /* nothing, see pragma below */

	#pragma pack 4                /* tricore byte-packed      */

#elif defined (TOOL_CHAIN_GNU_PPC)

	#define PNIO_PACKED_ATTRIBUTE_PRE   /* no prefix */
	#define PNIO_PACKED_ATTRIBUTE_POST  __attribute__ ((packed))
	/* does not use pragma, see define above */

#elif defined (TOOL_CHAIN_CC386)

	#define PNIO_PACKED_ATTRIBUTE_PRE   /* nothing, see pragma below */
	#define PNIO_PACKED_ATTRIBUTE_POST  /* nothing, see pragma below */
	#pragma noalign

#elif defined (TOOL_CHAIN_GREENHILLS_ARM)

	#define PNIO_PACKED_ATTRIBUTE_PRE   /* nothing, see pragma below */
	#define PNIO_PACKED_ATTRIBUTE_POST  /* nothing, see pragma below */
	#pragma pack(4)               /* 1=byte 2=short 4=long alignment */

#elif defined (TOOL_CHAIN_GREENHILLS_MIPS)

    #define PNIO_PACKED_ATTRIBUTE_PRE   /* nothing, see pragma below */
    #define PNIO_PACKED_ATTRIBUTE_POST  /* nothing, see pragma below */
    #pragma pack(4)               /* 1=byte 2=short 4=long alignment */

#elif defined (TOOL_CHAIN_NRK) /* ARM/Thumb C/C++ Compiler */

	#define PNIO_PACKED_ATTRIBUTE_PRE   __packed
	#define PNIO_PACKED_ATTRIBUTE_POST  /* no postfix */
	/* does not use pragma, see define above */

#elif defined (TOOL_CHAIN_GNU)

	#define PNIO_PACKED_ATTRIBUTE_PRE   /* nothing, see pragma below */
	#define PNIO_PACKED_ATTRIBUTE_POST  /* nothing, see pragma below */
	#pragma pack (4)              /* 1=byte 2=short 4=long alignment */

#elif defined (TOOL_CHAIN_IAR)

	#define PNIO_PACKED_ATTRIBUTE_PRE   /* nothing, see pragma below */
	#define PNIO_PACKED_ATTRIBUTE_POST  /* nothing, see pragma below */
	#pragma pack (4)              /* 1=byte 2=short 4=long alignment */
#else

    /* packing has to be defined in system adaption */
	#include "sys_pck4.h"
    #define PNIO_PACKED_ATTRIBUTE_PRE   SYS_BYTE_PACKED_PRE
    #define PNIO_PACKED_ATTRIBUTE_POST  SYS_BYTE_PACKED

#endif


/*---------------------------------------------------------------------------*/
/*      3.  set MARKER for 32bit packing                                     */
/*---------------------------------------------------------------------------*/

#define PNIO_PCK_ON_H


/*---------------------------------------------------------------------------*/
/*      4.  undef MARKER for unpack header                                   */
/*---------------------------------------------------------------------------*/
#undef PNIO_PCK_OFF_H



/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
