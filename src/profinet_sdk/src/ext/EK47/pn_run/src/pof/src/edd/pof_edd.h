#ifndef POF_EDD_H   /* ----- reinclude-protection ----- */
#define POF_EDD_H

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
/*  F i l e               &F: pof_edd.h                                 :F&  */
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

#ifdef __cplusplus  /* If C++ - compiler: Use C linkage */
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
POF_RSP POF_LOCAL_FCT_ATTR POF_EDDCloseChannel( POF_HDB_PTR_TYPE const pHDB );

POF_RSP POF_LOCAL_FCT_ATTR POF_EDDOpenChannel( POF_HDB_PTR_TYPE const pHDB );

POF_RSP POF_LOCAL_FCT_ATTR POF_EDDInitPortMgm( POF_HDB_PTR_TYPE const pHDB,
                                               LSA_UINT32       const PortCnt );

LSA_VOID POF_LOCAL_FCT_ATTR POF_EDDFreePortMgm( POF_HDB_PTR_TYPE const pHDB );

POF_RSP POF_LOCAL_FCT_ATTR POF_EDDGetParams( POF_HDB_PTR_TYPE const pHDB );

POF_RSP POF_LOCAL_FCT_ATTR POF_EDDLinkIndProvide( POF_HDB_PTR_TYPE const pHDB,  
                                                  LSA_UINT32       const PortCnt );

POF_RSP POF_LOCAL_FCT_ATTR POF_EDDGetPortParams( POF_HDB_PTR_TYPE const pHDB );

POF_RSP POF_LOCAL_FCT_ATTR POF_Get_Port_Index( LSA_UINT16                      const PortId,
                                               LSA_UINT16 POF_LOCAL_MEM_ATTR * const pPort_Index,
                                               LSA_BOOL                        const bPrmService,
                                               POF_HDB_PTR_TYPE                const pHDB );

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif


/*****************************************************************************/
/*  end of file pof_edd.h                                                    */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
#endif //POF_EDD_H
