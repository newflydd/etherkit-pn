
#ifndef EDDP_NRT_H                       /* ----- reinclude-protection ----- */
#define EDDP_NRT_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

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
/*  C o m p o n e n t     &C: EDDP (EthernetDeviceDriver for PN-IP)     :C&  */
/*                                                                           */
/*  F i l e               &F: eddp_nrt.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  public NRT headerfile                                                    */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  24.09.08    SF    initial version.                                       */
#endif
/*****************************************************************************/


/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/

// simple detection for setting of the FDC-field by the PNIP
// 0: (default) off
// 1: fill FDC with EDDP_FDC_INVALID to detect HW bug if found EDDP_FDC_INVALID in ISR-handler 
#define EDDP_NRT_CHECK_FDC_FIELD    0

// CFG switch to force off SW Rx/Tx-counter
// 0: off
// 1: (default) SW counter enabled
#define EDDP_NRT_RXTX_COUNTER_SUPPORT   1

/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/



/*===========================================================================*/
/*                                  data                                     */
/*===========================================================================*/


/*===========================================================================*/
/*                                 macros                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                prototyping                                */
/*===========================================================================*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_NRTDeviceOpen(
	EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
	EDDP_UPPER_DPB_PTR_TYPE            pDPB);


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_NRTDeviceSetup(
	EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
	EDDP_UPPER_DSB_PTR_TYPE            pDSB);


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_NRTDeviceIFSetup(
	EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
	EDDP_UPPER_DSB_PTR_TYPE            pDSB);


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_NRTDeviceIFShutdown(
	EDDP_LOCAL_DDB_PTR_TYPE            pDDB);


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_NRTDeviceShutdown(
	EDDP_LOCAL_DDB_PTR_TYPE            pDDB);


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_NRTDeviceClose(
	EDDP_LOCAL_DDB_PTR_TYPE            pDDB);


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_NRTOpenChannel(
	EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
	EDDP_LOCAL_HDB_PTR_TYPE            pHDB,
	LSA_HANDLE_TYPE                    Handle,
	EDDP_SYSTEM_CDB_PTR_TYPE           pCDB);


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_NRTRequest(
	EDD_UPPER_RQB_PTR_TYPE             pRQB,
	EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
	EDDP_LOCAL_HDB_PTR_TYPE            pHDB);


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_NRTCloseChannel(
	EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
	EDDP_LOCAL_HDB_PTR_TYPE            pHDB,
	LSA_HANDLE_TYPE                    Handle);


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_NRTUpperAdaptAlignment(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB, 
    EDD_UPPER_MEM_PTR_TYPE      pAddr);


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_NRTFillSetup(
    EDDP_LOCAL_DDB_PTR_TYPE pDDB,
    LSA_BOOL                resetToDefault,
    LSA_BOOL                LoadLimitationActive,
    LSA_BOOL                IO_Configured);


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_NRTFillIsIOAllowed(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_NRTFillIsFeedInLoadLimitationActive(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_NRTFillGetStartupParameter(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB);

/*****************************************************************************/
/*  end of file EDDP_NRT.H                                                   */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_NRT_H */
