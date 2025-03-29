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
/*  F i l e               &F: pnpb_sub_exp.h                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
#ifndef PNPB_SUB_EXP_H
#define PNPB_SUB_EXP_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif


// *-------------------------------------------------
// *	typedef for slot/Subslot Model
// *    and physical device (PDEV)
// *-------------------------------------------------
#define BIT_PLUGGED		1	// bit 0: module or submodule is plugged
#define BIT_IODATA		2	// bit 1: submodule has IO Data
#define BIT_DAP 		4	// bit 2: submodule is the DAP submodule
#define BIT_PDEV 		8	// bit 3: submodule is pdev port (0x8000, 0x8001,..)


// *-------------------------------------------------
// *	PNPB internal functions
// *-------------------------------------------------
void            pnpb_sub_exp_init(void);

PNIO_EXP_SUB*   pnpb_sub_exp_oneAr_getp (PNIO_UINT32   ArNum,
                                         PNIO_UINT32   ApiNum, 
                                         PNIO_UINT32   SlotNum, 
                                         PNIO_UINT32   SubNum);

PNIO_EXP_SUB*   pnpb_sub_exp_allAr_getp (PNIO_UINT32*  pArNum,
                                         PNIO_UINT32   ApiNum, 
                                         PNIO_UINT32   SlotNum, 
                                         PNIO_UINT32   SubNum);

// *-------------------------------------------------
// *	public functions
// *-------------------------------------------------
#if 1  // 2015-05-05 patch01.2
    PNIO_UINT32 pnpb_sub_exp_add  (PNIO_UINT32 ArInd,                // [in]
							       PNIO_UINT32 EntityIndSetCfg,      // [in]
							       PNDV_SET_CFG_ELEMENT_T*  pElem,   // [in]
                                   PNIO_UINT32 Flags,                // [in]
							       PNIO_EXP_SUB** ppSubExp,          // [in, out]
							       PNIO_BOOL  MoreFollows);          // [in]
#else
    PNIO_UINT32 pnpb_sub_exp_add  (PNIO_UINT32 ArInd,                // [in]
							       PNIO_UINT32 EntityIndSetCfg,      // [in]
							       PNDV_SET_CFG_ELEMENT_T*  pElem,   // [in]
							       PNIO_EXP_SUB** ppSubExp,          // [in, out]
							       PNIO_BOOL  MoreFollows);          // [in]
#endif

PNIO_BOOL pnpb_remove_all_sub  (PNIO_UINT32 ArNum);


#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif



#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
