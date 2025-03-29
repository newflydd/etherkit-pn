#ifndef POF_PRM_CHECK_H                     /* ----- reinclude-protection ----- */
#define POF_PRM_CHECK_H

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
/*  F i l e               &F: pof_prm_check.h                           :F&  */
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
/*                                  types                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                  prototypes                               */
/*===========================================================================*/
POF_RSP POF_LOCAL_FCT_ATTR  POF_CheckPrmRecord_Data_Read( POF_HDB_PTR_TYPE           const pHDB,
                                                          LSA_UINT32                 const RecordIndex,
                                                          POF_UPPER_MEM_U32_PTR_TYPE const pRecordDataLength );

POF_RSP POF_LOCAL_FCT_ATTR  POF_CheckPrmRecord_Data_Write( POF_HDB_PTR_TYPE const pHDB,
                                                           LSA_UINT32       const RecordIndex,
                                                           LSA_UINT32       const RecordDataLength );

POF_RSP POF_LOCAL_FCT_ATTR  POF_CheckPrmHeader( POF_HDB_PTR_TYPE        const pHDB,
                                                LSA_UINT32              const RecordIndex,
                                                POF_COMMON_MEM_PTR_TYPE const pRecordData );


/*****************************************************************************/
/*  end of file pof_prm_check.h                                              */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of POF_PRM_CHECK_H */
