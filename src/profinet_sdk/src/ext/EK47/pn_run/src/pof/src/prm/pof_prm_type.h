#ifndef POF_PRM_TYPE_H                     /* ----- reinclude-protection ----- */
#define POF_PRM_TYPE_H

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
/*  F i l e               &F: pof_prm_type.h                            :F&  */
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
/* RecordTypeWritten */
#define POF_RECORD_TYPE_NONE                 0x00
#define POF_RECORD_TYPE_DATA_ADJUST_8062     0x01
#define POF_RECORD_TYPE_DATA_CHECK_8061      0x02
#define POF_RECORD_TYPE_SFP_DATA_CHECK_8063  0x04

/* CheckEnable */
/* #define POF_RECORD_CHECK_ENABLE_NONE          0x00 */
/* #define POF_RECORD_CHECK_ENABLE_MAIN_DEMANDED 0x01 */
/* #define POF_RECORD_CHECK_ENABLE_MAIN_REQUIRED 0x02 */

/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/
typedef enum _POF_PRM_DIAG_EVENT
{
  POF_PRM_EV_GOOD,
  POF_PRM_EV_REQUIRED,
  POF_PRM_EV_DEMANDED,
  POF_PRM_EV_ERROR,     
  POF_PRM_EV_START

} POF_PRM_DIAG_EVENT;

/* RecordSet for _prm_ Services */
typedef struct _POF_RECORD_SET_TYPE
{
  LSA_UINT32  LN_FiberOpticType;
  LSA_UINT32  LN_FiberOpticCableType;
  LSA_UINT32  LN_MRPowerBudget;
  LSA_UINT32  LN_MDPowerBudget;
  LSA_UINT32  LN_ErrorPowerBudget;
  LSA_UINT16  MRPowerBudget_0_1_dB;
  LSA_UINT16  MDPowerBudget_0_1_dB;
  LSA_BOOL    MRCheckEnable;           /* diagnostic type is enable */
  LSA_BOOL    MDCheckEnable;           /* diagnostic type is enable */
  LSA_BOOL    ErrorCheckEnable;        /* diagnostic type is enable */
  LSA_UINT8   RecordTypeWritten;       /* which record type has written */
  LSA_BOOL    MRThresholdWritten;      /* Threshold is written */
  LSA_BOOL    MDThresholdWritten;      /* Threshold is written */
  LSA_UINT32  SFP_MaintenanceDemandedAdminStatus;
  LSA_UINT32  SFP_ErrorAdminStatus;

} POF_RECORD_SET_TYPE;

typedef struct _POF_PRM_DETAIL_ERR
{
    LSA_BOOL        ErrValid;
    LSA_BOOL        ErrFault;
    LSA_UINT16      ErrPortID;
    LSA_UINT32      ErrIndex;
    LSA_UINT32      ErrOffset;
 
    /* special offsets for consistency checks */
//    LSA_UINT32      EDDI_PRM_ADJUST_PORT_STATE_Offset[EDD_CFG_MAX_PORT_CNT];
//    LSA_UINT32      EDDI_PRM_PDSYNC_DATA_Record_Offset;
//    LSA_UINT32      EDDI_PRM_RECORD_PDSYNC_DATA_ReservedIntervalEnd_OFFSET;
//    LSA_UINT32      EDDI_PRM_RECORD_PDSYNC_DATA_PTCP_MasterPriority1_OFFSET;
//    LSA_UINT32      EDDI_PRM_RECORD_PDSYNC_DATA_SyncProperties_OFFSET;

} POF_PRM_DETAIL_ERR;

typedef struct _POF_PRM_DETAIL_ERR         POF_LOCAL_MEM_ATTR *    POF_PRM_DETAIL_ERR_PTR_TYPE;


/*===========================================================================*/
/*                                  prototypes                               */
/*===========================================================================*/

/*****************************************************************************/
/*  end of file pof_prm_type.h                                               */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of POF_PRM_TYPE_H */
