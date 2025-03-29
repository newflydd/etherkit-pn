#ifndef EDDP_CRT_USR_H                  /* ----- reinclude-protection ----- */
#define EDDP_CRT_USR_H

#ifdef __cplusplus                      /* If C++ - compiler: Use C linkage */
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
/*  F i l e               &F: eddp_crt_usr.h                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  System interface                                                         */
/*  Prototyping for EDDP                                                     */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who         What                                             */
#ifdef EDDP_MESSAGE
/*  03.03.09    Sergeev     Erstellung                                       */
#endif
/*****************************************************************************/





/* ============================================================================ */
/*                              prototyping                                     */
/* ============================================================================ */

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTDeviceOpen(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DPB_PTR_TYPE     pDPB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTDeviceSetup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DSB_PTR_TYPE     pDSB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTDeviceIFSetup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DSB_PTR_TYPE     pDSB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTOpenChannel(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB,
    LSA_HANDLE_TYPE             Handle,
    EDDP_SYSTEM_CDB_PTR_TYPE    pCDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTRequest(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCloseChannel(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB,
    LSA_HANDLE_TYPE             Handle);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTDeviceIFShutdown(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTDeviceShutdown(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTDeviceClose(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetNumberActiveConsumerProvider(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif  /* of EDDP_CRT_USR_H */
