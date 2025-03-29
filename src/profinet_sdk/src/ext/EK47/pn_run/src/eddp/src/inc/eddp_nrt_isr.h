
#ifndef EDDP_NRT_ISR_H                   /* ----- reinclude-protection ----- */
#define EDDP_NRT_ISR_H

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
/*  F i l e               &F: eddp_nrt_isr.h                            :F&  */
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



/*===========================================================================*/
/*                                  data                                     */
/*===========================================================================*/


/*===========================================================================*/
/*                                 macros                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                prototyping                                */
/*===========================================================================*/


LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_NRT_ISR_Acyc_Snd_IFA(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_NRT_ISR_Acyc_Rcv_IFA(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_NRT_ISR_Acyc_Snd_ERR(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_NRT_ISR_Acyc_Rcv_ERR(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_NRT_ISR_APIFilter_ERR(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr);


/*****************************************************************************/
/*  end of file EDDP_NRT_ISR.H                                               */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_NRT_ISR_H */
