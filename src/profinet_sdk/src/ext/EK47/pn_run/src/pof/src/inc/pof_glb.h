#ifndef POF_GLB_H                   /* ----- reinclude-protection ----- */
#define POF_GLB_H

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
/*  F i l e               &F: pof_glb.h                                 :F&  */
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
/*                                   macros                                  */
/*===========================================================================*/
#ifdef POF_CFG_NO_FATAL_FILE_INFO
#define POF_FATAL(sErr, Error, DW_0, DW_1)                                  \
  POF_FatalError((LSA_UINT32)__LINE__, (LSA_UINT8 *)(void *)0, (LSA_UINT32)POF_MODULE_ID, \
                (LSA_UINT8  *)(void *)0, (LSA_UINT32)Error, (LSA_UINT32)DW_0, (LSA_UINT32)DW_1)
#else
#define POF_FATAL(sErr, Error, DW_0, DW_1)                                  \
  POF_FatalError((LSA_UINT32)__LINE__, (LSA_UINT8 *)(void *)__FILE__, (LSA_UINT32)POF_MODULE_ID, \
                (LSA_UINT8  *)(void *)sErr, (LSA_UINT32)Error, (LSA_UINT32)DW_0, (LSA_UINT32)DW_1)
#endif
   
#define POF_MEM_FREE_FAIL_CHECK(RetVal)                                     \
{                                                                           \
  if (RetVal != LSA_RET_OK)                                                 \
  {                                                                         \
    POF_FATAL("POF_MEM_FREE_FAIL_CHECK", POF_FATAL_ERR_MEM_FREE, 0, 0);     \
  }                                                                         \
}

#define POF_CHECK_NULL_PTR(Ptr)                                             \
{                                                                           \
    if (LSA_HOST_PTR_ARE_EQUAL((Ptr), LSA_NULL))                            \
    {                                                                       \
        POF_FATAL("POF_CHECK_NULL_PTR", POF_FATAL_ERR_NULL_PTR, Ptr, 0);    \
    }                                                                       \
}

/*=============================================================================
 * macro name: POF_IS_POF_PORT()
 *
 * return:     LSA_BOOL
 *===========================================================================*/
#define POF_IS_POF_PORT(PortIndex, pHDB)                                                           \
    (  (EDD_MEDIATYPE_FIBER_OPTIC_CABLE == (pHDB)->pPCtrl[PortIndex].LinkStatus.MediaType)         \
    && (EDD_PORT_OPTICALTYPE_ISPOF      == (pHDB)->pPCtrl[PortIndex].LinkStatus.OpticalTransType)  )

/*=============================================================================
 * macro name: POF_IS_SFP_PORT()
 *
 * return:     LSA_BOOL
 *===========================================================================*/
#define POF_IS_SFP_PORT(PortIndex, pHDB)                                                           \
    (  (EDD_MEDIATYPE_FIBER_OPTIC_CABLE == (pHDB)->pPCtrl[PortIndex].LinkStatus.MediaType)         \
    && (EDD_PORT_OPTICALTYPE_ISSFP      == (pHDB)->pPCtrl[PortIndex].LinkStatus.OpticalTransType)  )

/*=============================================================================
 * macro name: POF_IS_FIBER_PORT()
 *
 * return:     LSA_BOOL
 *===========================================================================*/
#ifndef POF_IS_FIBER_PORT
#define POF_IS_FIBER_PORT(PortIndex, pHDB)                                   \
    (EDD_MEDIATYPE_FIBER_OPTIC_CABLE == (pHDB)->pPCtrl[PortIndex].LinkStatus.MediaType)
#endif

/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                prototyping                                */
/*===========================================================================*/
LSA_VOID POF_LOCAL_FCT_ATTR   POF_FatalError( LSA_UINT32         const Line,
                                              LSA_UINT8        * const sFile,
                                              LSA_UINT32         const ModuleID,
                                              LSA_UINT8        * const sErr,
                                              LSA_UINT32         const Error,    
                                              LSA_UINT32         const DW_0,    
                                              LSA_UINT32         const DW_1 );

LSA_VOID POF_LOCAL_FCT_ATTR POF_CallCbf( LSA_UINT32                      const TraceIdx,
                                         POF_UPPER_CALLBACK_FCT_PTR_TYPE const pCbf,
                                         POF_UPPER_RQB_PTR_TYPE          const pRQB,
                                         LSA_SYS_PTR_TYPE                const pSys );

LSA_VOID POF_LOCAL_FCT_ATTR POF_UserRequestFinish( POF_HDB_PTR_TYPE       const pHDB,
                                                   POF_UPPER_RQB_PTR_TYPE const pRQB,
                                                   POF_RSP                const Status );

POF_HDB_PTR_TYPE  POF_LOCAL_FCT_ATTR POF_GetUserHDBFromHandle( LSA_HANDLE_TYPE const Handle );

LSA_VOID POF_LOCAL_FCT_ATTR POF_QueueAddToEnd( POF_QUEUE_PTR_TYPE         const pQueue,
                                               POF_QUEUE_ELEMENT_PTR_TYPE const pNewElement );

POF_QUEUE_ELEMENT_PTR_TYPE POF_LOCAL_FCT_ATTR POF_QueueRemoveFromBegin( POF_QUEUE_PTR_TYPE const pQueue );

LSA_VOID  POF_LOCAL_FCT_ATTR  POF_UserChannelStateMachine( POF_HDB_PTR_TYPE  const  pHDB,
                                                           POF_RSP                  Response );


/*****************************************************************************/
/*  end of file pof_glb.h                                                    */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of POF_GLB_H */
