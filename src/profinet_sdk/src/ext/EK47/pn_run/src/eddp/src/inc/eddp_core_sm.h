
#ifndef EDDP_CORE_SM_H                   /* ----- reinclude-protection ----- */
#define EDDP_CORE_SM_H

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
/*  F i l e               &F: eddp_core_sm.h                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Device/Handle                                                            */
/*  Defines constants, types, macros and prototyping for Device/Channel      */
/*  Statemachines of EDDP Core                                               */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  15.08.08    JS    initial version.                                       */
#endif
/*****************************************************************************/

/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/

/*===========================================================================*/
/*                              DEVICE_SETUP Satemachine                     */
/*===========================================================================*/

typedef struct _EDDP_DDB_CORE_SM_SETUP_TYPE
{
    LSA_UINT32              State;
    EDD_RSP                 Response;
    LSA_UINT32              FctTblIdx;
    EDD_UPPER_RQB_PTR_TYPE  pRQB;
    EDDP_UPPER_DSB_PTR_TYPE pDSB;   
} EDDP_DDB_CORE_SM_SETUP_TYPE;

typedef struct _EDDP_DDB_CORE_SM_SETUP_TYPE    EDDP_LOCAL_MEM_ATTR *    EDDP_LOCAL_DDB_CORE_SM_SETUP_PTR_TYPE;

/* States for devices setup */
#define EDDP_DDB_STATE_SETUP_IDLE            0
#define EDDP_DDB_STATE_SETUP_START           1
#define EDDP_DDB_STATE_SETUP_DO_SETUP        2 
#define EDDP_DDB_STATE_SETUP_DO_SHUTDOWN     3 

/*===========================================================================*/
/*                          DEVICE_SHUTDOWN Statemachine                     */
/*===========================================================================*/

typedef struct _EDDP_DDB_CORE_SM_SHUTDOWN_TYPE
{
    LSA_UINT32              State;
    EDD_RSP                 Response;
    LSA_UINT32              FctTblIdx;
    EDD_UPPER_RQB_PTR_TYPE  pRQB;
} EDDP_DDB_CORE_SM_SHUTDOWN_TYPE;

typedef struct _EDDP_DDB_CORE_SM_SHUTDOWN_TYPE    EDDP_LOCAL_MEM_ATTR *    EDDP_LOCAL_DDB_CORE_SM_SHUTDOWN_PTR_TYPE;

/* States for devices shutdown */
#define EDDP_DDB_STATE_SHUTDOWN_IDLE        0
#define EDDP_DDB_STATE_SHUTDOWN_START       1
#define EDDP_DDB_STATE_SHUTDOWN_DO_SHUTDOWN 2

/*****************************************************************************/
/*  end of file eddp_core_sm.h                                               */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_CORE_SM_H */
