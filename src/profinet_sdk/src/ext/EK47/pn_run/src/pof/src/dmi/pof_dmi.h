#ifndef POF_DMI_H                     /* ----- reinclude-protection ----- */
#define POF_DMI_H

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
/*  F i l e               &F: pof_dmi.h                                 :F&  */
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

LSA_VOID    POF_LOCAL_FCT_ATTR POF_DMI_ReadOffset_A0(      LSA_UINT16               const PortIndex,
                                                           LSA_UINT8                const Offset, 
                                                           LSA_UINT32               const Size,   
                                                           POF_COMMON_MEM_PTR_TYPE        pAdr, 
                                                           POF_HDB_PTR_TYPE         const pHDB );

LSA_VOID    POF_LOCAL_FCT_ATTR POF_DMI_ReadOffset_A2(      LSA_UINT16               const PortIndex,
                                                           LSA_UINT8                const Offset, 
                                                           LSA_UINT32               const Size,   
                                                           POF_COMMON_MEM_PTR_TYPE        pAdr, 
                                                           POF_HDB_PTR_TYPE         const pHDB );

LSA_VOID    POF_LOCAL_FCT_ATTR POF_DMI_ReadHW_TimeOut(     POF_HDB_PTR_TYPE         const pHDB );

LSA_BOOL    POF_LOCAL_FCT_ATTR POF_DMI_Validate_HW(        LSA_UINT16               const PortIndex,
                                                           POF_HDB_PTR_TYPE         const pHDB );
LSA_BOOL    POF_LOCAL_FCT_ATTR SFP_DMI_Validate_HW(        LSA_UINT16               const PortIndex,
                                                           POF_HDB_PTR_TYPE         const pHDB );

EDD_RSP     POF_LOCAL_FCT_ATTR POF_I2C_Write_ByteArray(    LSA_UINT16               const PortIndex,
                                                           POF_HDB_PTR_TYPE         const pHDB,
                                                           LSA_UINT8                const PageA2Address,
                                                           LSA_UINT32               const NumberBytes,
                                                     const LSA_UINT8  *             const pBuf);

LSA_UINT32  POF_LOCAL_FCT_ATTR POF_Write_SFP_DiagTestData( LSA_UINT16               const  PortIndex,
                                                           POF_HDB_PTR_TYPE         const  pHDB,
                                                           LSA_UINT32               const  ManipulationRequestFlags);

/*****************************************************************************/
/*  end of file pof_dmi.h                                                    */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of POF_DMI_H */
