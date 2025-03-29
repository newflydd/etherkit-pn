#ifndef EDDP_CRT_ISR_H                  /* ----- reinclude-protection ----- */
#define EDDP_CRT_ISR_H

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
/*  F i l e               &F: eddp_crt_isr.h                            :F&  */
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
/*  Defines, constants, types, macros, structs for EDDP                      */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who         What                                             */
#ifdef EDDP_MESSAGE
/*  09.07.09    Freitag     Erstellung                                       */
#endif
/*****************************************************************************/





/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/


/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/



/*===========================================================================*/
/*                                 macros                                    */
/*===========================================================================*/




/*===========================================================================*/
/*                                prototyping                                */
/*===========================================================================*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRT_ISR_Cpm_Err ( 
    EDDP_LOCAL_DDB_PTR_TYPE  pDDB,
    LSA_UINT32               EventNr );

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRT_ISR_CSB_changed_IFA(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRT_ISR_Ppm_Err(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRT_ISR_Ppm_not_finished_IFA(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRT_ISR_Ppm_HOL_not_empty_IFA(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRT_ISR_PhaseTransferError(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTPpmHolNotEmptyTimerPassed(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTPpmNotFinishedTimerPassed(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRT_ISR_DelayStatus(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRT_ISR_PhaseCrossed(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRT_ISR_InBound_Late_ERR(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTLateErrorTimerPassed(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRT_ISR_Pack_ERR(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr);

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif  /* of EDDP_CRT_ISR_H */
