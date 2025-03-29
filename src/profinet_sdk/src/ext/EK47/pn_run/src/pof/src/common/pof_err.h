#ifndef POF_ERR_H                    /* ----- reinclude-protection ----- */
#define POF_ERR_H

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
/*  F i l e               &F: pof_err.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Version of prefix                                                        */
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

/* Detail ERROR Codes */

/*** Common ***/
#define POF_ERR_SEQUENCE                                0x00000008UL

/*** Detail-Error ***/
#define POF_ERR_PRM_CHECK_RECORD_DATA_LENGTH            0x00000001UL
#define POF_ERR_PRM_CHECK_BLOCK_1_LENGTH                0x00000002UL
#define POF_ERR_PRM_CHECK_BLOCK_TYPE                    0x00000004UL
#define POF_ERR_PRM_CHECK_VERSION_HIGH                  0x00000005UL
#define POF_ERR_PRM_CHECK_VERSION_LOW                   0x00000006UL
#define POF_ERR_PORTID                                  0x00000007UL
#if defined (POF_CFG_SIMULATE_DIAGNOSTIC)
#define POF_ERR_DIAGNOSTIC_TYPE                         0x00000008UL
#endif
#define POF_ERR_DIAGNOSTIC_MAIN_REQ_POWER_VALUE         0x00000009UL
#define POF_ERR_DIAGNOSTIC_MAIN_DEM_POWER_VALUE         0x0000000AUL
#define POF_ERR_DIAGNOSTIC_ERROR_POWER_ENABLE           0x0000000CUL
//#define POF_ERR_PORT_TRANSCEIVER_NOT_INIT               0x0000000DUL
#define POF_ERR_DIAGNOSTIC_FIBEROPTICTYPE               0x0000000EUL
#define POF_ERR_DIAGNOSTIC_FIBEROPTICCABLE              0x0000000FUL
#define POF_ERR_PRM_CHECK_PADDING_1                     0x00000010UL
#define POF_ERR_PRM_CHECK_PADDING_2                     0x00000011UL
//#define POF_ERR_PORT_NO_FIBER                           0x00000012UL
#define POF_ERR_PORT_NO_POF                             0x00000013UL
#define POF_ERR_PORT_NO_SFP                             0x00000014UL
#define POF_ERR_TEST_DIAG                               0x00000015UL


/*****************************************************************************/
/*  end of file pof_err.h                                                    */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of POF_ERR_H */

