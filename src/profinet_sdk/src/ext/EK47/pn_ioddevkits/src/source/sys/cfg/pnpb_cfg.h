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
/*  F i l e               &F: pnpb_cfg.h                                :F&  */
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
#ifndef PNPB_CFG_H
#define PNPB_CFG_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif



	#define PNPB_COPY_BYTE(_DEST_PTR,_SRC_PTR,_LEN)                              \
	{                                                                            \
		LSAS_MEMCPY((void*)_DEST_PTR,_SRC_PTR,_LEN);                             \
	}

	#define PNPB_CMPR_BYTE(_DEST_PTR,_SRC_PTR,_LEN)                              \
	{                                                                            \
		LSAS_MEMCMP((void*)_DEST_PTR,(void*)_SRC_PTR,_LEN);                             \
	}

 	// return TRUE if submodule supports ProfiEnergy
	#define PNPB_ENABLE_PROFIENERGY(_SLOT_NR, _SUBSLOT_NR, _MODUL_IDENT, _SUBMODUL_IDENT)		\
		IOD_ENABLE_PROFIENERGY(_SLOT_NR, _SUBSLOT_NR, _MODUL_IDENT, _SUBMODUL_IDENT)

	#define PNPB_ENABLE_IM5                         IOD_ENABLE_IM5

	#define PNPB_TRIGGER_PNDV()                      {tskma_task_app_send_pndv_trigger();}
    #define PNPB_WAIT_ON_TRIGGER_PNDV()              {OsReadMessageBlocked ((void**)&pDat, taskID);}

    PNIO_VOID*  PnpbCfg_GetpIface(PNIO_VOID);

    #define PNPB_BSS_FAST                           OS_BSS_FAST
    #define PNPB_DATA_FAST                          OS_DATA_FAST
    #define PNPB_CODE_FAST                          OS_CODE_FAST

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif


#endif


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
