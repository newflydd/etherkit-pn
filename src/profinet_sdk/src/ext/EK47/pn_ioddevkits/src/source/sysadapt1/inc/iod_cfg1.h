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
/*  F i l e               &F: iod_cfg1.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  device configuration file                                                */
/*                                                                           */
/*****************************************************************************/
#ifndef IOD_CFG1_H
#define IOD_CFG1_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

#ifndef LOG_LSA_PACK_ID
  #define LOG_LSA_PACK_ID		LSA_COMP_ID_IODAPI
  #define LOG_LSA_MOD_ID		IOD_MODULE_ID
#endif

#include "iod_cfg2.h"



// *===========================================================
//       additional defines (don't change)
// *===========================================================


// *-----------------------------------------
// *		FRAME IDs, IOCR-length
// *   (recommendation: keep unchanged)
// *-----------------------------------------
#define IOD_CONSUMER_C1_FRAME_ID_BASE	0xC000		// first supported frame ID number  // towe org: 0xC010
#define IOD_CONSUMER_C2_FRAME_ID_BASE	0x8000		// first supported frame ID number	// towe org: 0x8010
#define IOD_CONSUMER_C3_FRAME_ID_BASE	0x0100	    // first supported frame ID number

#if (PNIOD_PLATFORM &  (PNIOD_PLATFORM_EBx00 | PNIOD_PLATFORM_RZT2_EDDP) )

	// **** RT Class 1 ***
    #define IOD_CONSUMER_C1_FRAME_ID_COUNT	4			// number of consumer frame IDs
    #define IOD_PROVIDER_C1_FRAME_ID_COUNT	2			// number of consumer frame IDs

    // **** RT Class 2 ***
    #define IOD_CONSUMER_C2_FRAME_ID_COUNT	4			// number of consumer frame IDs
    #define IOD_PROVIDER_C2_FRAME_ID_COUNT	2			// number of consumer frame IDs

    #define IOD_CONSUMER_C1_2_COUNT	        4			// c1 an c2 together
    #define IOD_PROVIDER_C1_2_COUNT	        2			// c1 an c2 together

    // **** RT Class 3 ***
    #define IOD_CONSUMER_C3_FRAME_ID_COUNT	4		    // number of consumer frame IDs
    #define IOD_PROVIDER_C3_FRAME_ID_COUNT	2		    // number of consumer frame IDs

#elif (PNIOD_PLATFORM &  PNIOD_PLATFORM_DKSW)
    // **** RT Class 1 ***
    #define IOD_CONSUMER_C1_FRAME_ID_COUNT	2			// number of consumer frame IDs
    #define IOD_PROVIDER_C1_FRAME_ID_COUNT	2			// number of consumer frame IDs

    // **** RT Class 2 ***
    #define IOD_CONSUMER_C2_FRAME_ID_COUNT	2			// number of consumer frame IDs   // towe old:0
    #define IOD_PROVIDER_C2_FRAME_ID_COUNT	2			// number of consumer frame IDs	  // towe old:0

    // **** RT Class 3 ***
    #define IOD_CONSUMER_C3_FRAME_ID_COUNT	0		    // number of consumer frame IDs
    #define IOD_PROVIDER_C3_FRAME_ID_COUNT	0		    // number of consumer frame IDs
#else
	#error "no valid platform selected"
#endif


#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
