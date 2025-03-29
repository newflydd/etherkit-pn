
#ifndef EDDP_NRT_RCV_H                   /* ----- reinclude-protection ----- */
#define EDDP_NRT_RCV_H

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
/*  F i l e               &F: eddp_nrt_rcv.h                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Internal NRT headerfile                                                  */
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



/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/

// *****  see: hal\eddp_hal_nrt.h


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
EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_NRTDeInitRcvRings(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  RingMask);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_NRTInitRcvRings(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_DPB_NRT_PTR_TYPE       pIPBNrt,
    LSA_UINT32                  RingMask);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_NRTInitRcvRingsEmpty(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  RingMask);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_NRTDeInitSingleRcvRingAndBuffers(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  RingIdx);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_NRTInitSingleRcvRingWithBuffers(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  RingIdx,
    LSA_UINT16                  Count,
    LSA_UINT16                  RxBuf32Size);


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_NRTRecv(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB);




/*****************************************************************************/
/*  end of file EDDP_NRT_RCV.H                                               */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_NRT_RCV_H */
