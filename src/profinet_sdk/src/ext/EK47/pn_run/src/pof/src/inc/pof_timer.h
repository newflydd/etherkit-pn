#ifndef POF_TIMER_H                     /* ----- reinclude-protection ----- */
#define POF_TIMER_H

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
/*  F i l e               &F: pof_timer.h                               :F&  */
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
/*                                prototyping                                */
/*===========================================================================*/
POF_RSP  POF_LOCAL_FCT_ATTR POF_TimerAlloc( LSA_UINT16                       const TimerType,
                                            LSA_UINT16                       const TimerBase,
                                            LSA_UINT32                       const UserID,
                                            LSA_VOID                         LSA_FCT_PTR(POF_LOCAL_FCT_ATTR, const Cbf) 
                                                                                         (POF_INTERNAL_REQUEST_PTR_TYPE pIntRQB),
                                            LSA_TIMER_ID_TYPE POF_LOCAL_MEM_ATTR * pPOFTimerIndex );

POF_RSP   POF_LOCAL_FCT_ATTR POF_TimerFree( LSA_TIMER_ID_TYPE const POFTimerIndex );

LSA_VOID  POF_LOCAL_FCT_ATTR POF_TimerMgmSetup( LSA_VOID );

LSA_VOID  POF_LOCAL_FCT_ATTR POF_TimerStop( LSA_TIMER_ID_TYPE const POFTimerIndex );

LSA_VOID  POF_LOCAL_FCT_ATTR POF_TimerStart( LSA_TIMER_ID_TYPE const POFTimerIndex,
                                             LSA_UINT16        const Time );

LSA_VOID  POF_LOCAL_FCT_ATTR POF_TimerRQBFree( LSA_TIMER_ID_TYPE const POFTimerIndex );

LSA_VOID  POF_LOCAL_FCT_ATTR POF_CylicTimerCbf( POF_INTERNAL_REQUEST_PTR_TYPE pIntRQB );


/*****************************************************************************/
/*  end of file pof_timer.h                                                  */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of POF_TIMER_H */
