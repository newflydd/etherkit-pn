#ifndef POF_PRM_REC_H                   /* ----- reinclude-protection ----- */
#define POF_PRM_REC_H

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
/*  C o m p o n e n t     &C: POF (Polymer Optical Fiber)               :C&  */
/*                                                                           */
/*  F i l e               &F: pof_prm_rec.h                             :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Internal headerfile                                                      */
/*  Defines internal constants, types, data, macros and prototyping for      */
/*  POF.                                                                     */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
/*****************************************************************************/

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/

/*===========================================================================*/
/*                                prototyping                                */
/*===========================================================================*/

LSA_VOID    POF_LOCAL_FCT_ATTR POF_GetPowerBudget(        LSA_UINT16                                   const PortIndex,
                                                          POF_LOCAL_MEM_U32_PTR_TYPE                   const pPowerBudget_0_1_dB, 
                                                          POF_HDB_PTR_TYPE                             const pHDB );

LSA_VOID    POF_LOCAL_FCT_ATTR POF_Read_FO_DataReal(      LSA_UINT16                                   const PortIndex,
                                                          LSA_BOOL                                     const bLinkUp,     
                                                          POF_UPPER_PRM_FO_DATA_REAL_PTR_TYPE          const pRecordData, 
                                                          POF_HDB_PTR_TYPE                             const pHDB );

LSA_VOID    POF_LOCAL_FCT_ATTR POF_Read_FO_DataRealShort( LSA_UINT16                                   const PortIndex,
                                                          POF_UPPER_PRM_FO_DATA_REAL_PTR_TYPE          const pRecordData, 
                                                          POF_HDB_PTR_TYPE                             const pHDB );

LSA_VOID    POF_LOCAL_FCT_ATTR POF_Read_FO_DataCheck(     LSA_UINT16                                   const PortIndex,
                                                          POF_UPPER_PRM_FO_DATA_CHECK_PTR_TYPE         const pRecordData, 
                                                          POF_HDB_PTR_TYPE                             const pHDB );

LSA_VOID    POF_LOCAL_FCT_ATTR POF_Read_FO_DataAdjust(    LSA_UINT16                                   const PortIndex,
                                                          POF_UPPER_PRM_FO_DATA_ADJUST_PTR_TYPE        const pRecordData, 
                                                          POF_HDB_PTR_TYPE                             const pHDB );

LSA_VOID    POF_LOCAL_FCT_ATTR POF_Read_SFP_DataCheck(    LSA_UINT16                                   const PortIndex,
                                                          POF_UPPER_PRM_SFP_DATA_CHECK_PTR_TYPE        const pRecordData, 
                                                          POF_HDB_PTR_TYPE                             const pHDB );

LSA_VOID    POF_LOCAL_FCT_ATTR  POF_Read_SFP_DiagTest(    LSA_UINT16                                   const  PortIndex,
                                                          POF_UPPER_PRM_SFP_DIAG_TEST_PTR_TYPE         const  pRecordData,
                                                          POF_HDB_PTR_TYPE                             const  pHDB );

LSA_VOID    POF_LOCAL_FCT_ATTR POF_Read_Transceiver_Ident(LSA_UINT16                                   const PortIndex,
                                                          POF_UPPER_PRM_TRANSCEIVER_INDENT_PTR_TYPE    const pRecordData,
                                                          POF_HDB_PTR_TYPE                             const pHDB );

POF_RSP     POF_LOCAL_FCT_ATTR POF_Write_FO_DataCheck(    LSA_UINT16                                   const PortIndex,
                                                          POF_UPPER_PRM_FO_DATA_CHECK_CONST_PTR_TYPE   const pRecordData,
                                                          POF_HDB_PTR_TYPE                             const pHDB );

POF_RSP     POF_LOCAL_FCT_ATTR POF_Write_FO_DataAdjust(   LSA_UINT16                                   const PortIndex,
                                                          POF_UPPER_PRM_FO_DATA_ADJUST_CONST_PTR_TYPE  const pRecordData,
                                                          POF_HDB_PTR_TYPE                             const pHDB );

POF_RSP     POF_LOCAL_FCT_ATTR POF_Write_SFP_DataCheck(   LSA_UINT16                                   const PortIndex,
                                                          POF_UPPER_PRM_SFP_DATA_CHECK_CONST_PTR_TYPE  const pRecordData,
                                                          POF_HDB_PTR_TYPE                             const pHDB );

POF_RSP     POF_LOCAL_FCT_ATTR POF_Write_SFP_DiagTest (   LSA_UINT16                                   const PortIndex,
                                                          POF_UPPER_PRM_SFP_DIAG_TEST_CONST_PTR_TYPE   const pRecordData,
                                                          POF_HDB_PTR_TYPE                             const pHDB );


/*****************************************************************************/
/*  end of file pof_prm_rec.h                                                */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of POF_PRM_REC_H */
