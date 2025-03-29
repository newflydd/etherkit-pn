#ifndef POF_PRM_STATE_DIAG_H             /* ----- reinclude-protection ----- */
#define POF_PRM_STATE_DIAG_H

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
/*  F i l e               &F: pof_prm_state_diag.h                      :F&  */
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
/*                                  prototypes                               */
/*===========================================================================*/
LSA_VOID POF_LOCAL_FCT_ATTR POF_PRM_StateDiag_Start(              LSA_UINT16              const PortIndex,
                                                                  POF_HDB_PTR_TYPE        const pHDB,
                                                                  POF_PRM_DIAG_EVENT      const AppearEvent,
                                                                  LSA_UINT32              const AddValue );
//?? erstmal
LSA_BOOL   POF_LOCAL_FCT_ATTR DMI_ReadHW_MarginDummy(             LSA_UINT16              const PortIndex,
                                                                  POF_HDB_PTR_TYPE        const pHDB );
LSA_BOOL   POF_LOCAL_FCT_ATTR POF_DMI_ReadHW_Margin(              LSA_UINT16              const PortIndex,
                                                                  POF_HDB_PTR_TYPE        const pHDB );
LSA_BOOL   POF_LOCAL_FCT_ATTR POF_DMI_ReadSFP_Margin(             LSA_UINT16              const PortIndex,
                                                                  POF_HDB_PTR_TYPE        const pHDB );
LSA_VOID   POF_LOCAL_FCT_ATTR HW_Validate_TimeOutDummy(           LSA_UINT16              const PortIndex,
                                                                  POF_HDB_PTR_TYPE        const pHDB,
                                                                  LSA_BOOL                const bCommitCall );
LSA_VOID   POF_LOCAL_FCT_ATTR POF_Validate_TimeOut(               LSA_UINT16              const PortIndex,
                                                                  POF_HDB_PTR_TYPE        const pHDB,
                                                                  LSA_BOOL                const bCommitCall );
LSA_VOID   POF_LOCAL_FCT_ATTR SFP_Validate_TimeOut(               LSA_UINT16              const PortIndex,
                                                                  POF_HDB_PTR_TYPE        const pHDB,
                                                                  LSA_BOOL                const bCommitCall );

LSA_VOID   POF_LOCAL_FCT_ATTR POF_PRM_StateDiag_Validate_TimeOut( POF_HDB_PTR_TYPE        const pHDB,
                                                                  LSA_BOOL                const bCommitCall );

LSA_VOID POF_LOCAL_FCT_ATTR POF_PrmSendIndication(                LSA_UINT16              const PortIndex,
                                                                  POF_HDB_PTR_TYPE        const pHDB,
                                                                  POF_PRM_DIAG_EVENT      const AppearEvent,
                                                                  POF_PRM_DIAG_EVENT      const DisappearEvent,
                                                                  LSA_UINT32              const AddValue );

LSA_VOID POF_LOCAL_FCT_ATTR POF_SFP_IndicateChange(               LSA_UINT16                      const PortIndex, 
                                                                  POF_UPPER_RQB_PTR_TYPE          const pIndRQB, 
                                                                  POF_SFP_DIAGNOSTIC_TYPE const * const pChangeValue,
                                                                  LSA_UINT16                      const ChannelPropBase);



/*****************************************************************************/
/*  end of file pof_prm_state_diag.h                                         */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of POF_PRM_STATE_DIAG_H */

