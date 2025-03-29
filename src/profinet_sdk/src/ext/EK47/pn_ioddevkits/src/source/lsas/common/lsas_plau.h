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
/*  C o m p o n e n t     &C: PnIODDevkits                              :C&  */
/*                                                                           */
/*  F i l e               &F: lsas_plau.h                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/

/**
 * @file    lsas_plau.h
 * @brief   brief description
 * @author  cn3dit09
 * @version
 * @date    24.02.2012
 *
 * long description
 *
 */

/*****************************************************************************/

#ifndef LSAS_PLAU_H_
#define LSAS_PLAU_H_


#ifdef LSAS_CFG_USE_EDDI
    #ifdef LSAS_CFG_USE_EDDP
        #error It is not possible to use eddi and eddp
    #endif
    #ifdef LSAS_CFG_USE_EDDS
        #error It is not possible to use eddi and edds
    #endif
#endif

#ifdef LSAS_CFG_USE_EDDI
    #if defined LSAS_CFG_EDDI_ERTEC200 && defined LSAS_CFG_EDDI_ERTEC400
        #error do not define LSAS_CFG_EDDI_ERTEC200 and LSAS_CFG_EDDI_ERTEC400 simultaneously
    #endif
    #if defined LSAS_CFG_EDDI_ERTEC200 && defined LSAS_CFG_EDDI_SOC
        #error do not define LSAS_CFG_EDDI_ERTEC200 and LSAS_CFG_EDDI_SOC simultaneously
    #endif
    #if defined LSAS_CFG_EDDI_ERTEC400 && defined LSAS_CFG_EDDI_SOC
        #error do not define LSAS_CFG_EDDI_ERTEC400 and LSAS_CFG_EDDI_SOC simultaneously
    #endif

    #if !defined LSAS_CFG_EDDI_ERTEC200 && !defined LSAS_CFG_EDDI_ERTEC400 && !defined LSAS_CFG_EDDI_SOC
        #error if you use EDDI you have to define either LSAS_CFG_EDDI_ERTEC200 or LSAS_CFG_EDDI_ERTEC400 or LSAS_CFG_EDDI_SOC
    #endif
#endif


#ifdef LSAS_CFG_USE_EDDI
#define LSAS_EDD_HANDLE EDDI_HANDLE
//typedef EDDI_HANDLE LSAS_EDD_HANDLE
#ifndef LSAS_EDDI_I2C_TYPE
 #define LSAS_EDDI_I2C_TYPE EDDI_I2C_TYPE_NONE
#endif
#endif

#ifdef LSAS_CFG_USE_EDDP
#define LSAS_EDD_HANDLE EDDP_HANDLE
	#ifdef LSAS_CFG_USE_EDDS
		#error It is not possible to use eddp and edds
	#endif
#endif

#ifdef LSAS_CFG_USE_EDDS
	#define LSAS_EDD_HANDLE EDDS_HANDLE
#endif

#ifdef LSAS_CFG_USE_EDDS
    #ifndef LSAS_EDDS_SINGLE_DEVICE_HNDL
        #ifdef PNIO_SINGLE_DEVICE_HNDL
            #define LSAS_EDDS_SINGLE_DEVICE_HNDL PNIO_SINGLE_DEVICE_HNDL     /* for devkit compatibility */
        #else
            #define LSAS_EDDS_SINGLE_DEVICE_HNDL 1                           /* default value */
        #endif
    #endif
    #ifndef LSAS_EDDS_CFG_CYCLE_BASE_FACTOR
        #ifdef EDD_CFG_CYCLE_BASE_FACTOR  /*32: = 1ms, 64: = 2ms, 128: =4ms */
            #define LSAS_EDDS_CFG_CYCLE_BASE_FACTOR EDD_CFG_CYCLE_BASE_FACTOR /* for devkit compatibility */
        #else
            #define LSAS_EDDS_CFG_CYCLE_BASE_FACTOR 32                      /* default value */
        #endif
    #endif
    #ifndef LSAS_EDDS_CONSUMER_C1_FRAME_ID_BASE
        #ifdef IOD_CONSUMER_C1_FRAME_ID_BASE
            #define LSAS_EDDS_CONSUMER_C1_FRAME_ID_BASE IOD_CONSUMER_C1_FRAME_ID_BASE /* for devkit compatibility */
        #else
            #define LSAS_EDDS_CONSUMER_C1_FRAME_ID_BASE 0xC000              /* default value */
        #endif
    #endif
    #ifndef LSAS_EDDS_CONSUMER_C1_FRAME_ID_COUNT
        #ifdef IOD_CONSUMER_C1_FRAME_ID_COUNT
            #define LSAS_EDDS_CONSUMER_C1_FRAME_ID_COUNT IOD_CONSUMER_C1_FRAME_ID_COUNT /* for devkit compatibility */
        #else
            #define LSAS_EDDS_CONSUMER_C1_FRAME_ID_COUNT 2                 /* default value */
        #endif
    #endif
    #ifndef LSAS_EDDS_CONSUMER_C2_FRAME_ID_BASE
        #ifdef IOD_CONSUMER_C2_FRAME_ID_BASE
            #define LSAS_EDDS_CONSUMER_C2_FRAME_ID_BASE IOD_CONSUMER_C2_FRAME_ID_BASE /* for devkit compatibility */
        #else
            #define LSAS_EDDS_CONSUMER_C2_FRAME_ID_BASE 0x8000              /* default value */
        #endif
    #endif
    #ifndef LSAS_EDDS_CONSUMER_C2_FRAME_ID_COUNT
        #ifdef IOD_CONSUMER_C2_FRAME_ID_COUNT
            #define LSAS_EDDS_CONSUMER_C2_FRAME_ID_COUNT IOD_CONSUMER_C2_FRAME_ID_COUNT /* for devkit compatibility */
        #else
            #define LSAS_EDDS_CONSUMER_C2_FRAME_ID_COUNT 2                 /* default value */
        #endif
    #endif
    #ifndef LSAS_EDDS_ALLOC_MEM
        #define LSAS_EDDS_ALLOC_MEM(LOCAL_MEM_PTR_PTR, LENGTH, MEMORY_CLASS) OsAllocFX (LOCAL_MEM_PTR_PTR, LENGTH, MEMORY_CLASS)
    #endif
    #ifndef LSAS_EDDS_FREE_MEM
        #define LSAS_EDDS_FREE_MEM(RET_VAL_PTR, LOCAL_MEM_PTR, MEMORY_CLASS) *RET_VAL_PTR = OsFreeX   (LOCAL_MEM_PTR, MEMORY_CLASS)
    #endif
    #ifndef LSAS_EDDS_PRINT
        #define LSAS_EDDS_PRINT(_STRING)    PNIO_printf(_STRING)
    #endif
#endif

#ifndef LSAS_CFG_TCIP_PATH_TYPE_TCP_RECV_RES_COUNT
    #define LSAS_CFG_TCIP_PATH_TYPE_TCP_RECV_RES_COUNT 0
#endif

/* default for MRP is on */
#ifndef LSAS_CFG_USE_MRP
#ifndef LSAS_CFG_PORT_CNT
    #error LSAS_CFG_PORT_CNT is undefined
#else
    #if LSAS_CFG_PORT_CNT == 1
        #define LSAS_CFG_USE_MRP 0
    #else
        #define LSAS_CFG_USE_MRP 1
    #endif
#endif
#endif


#ifndef LSAS_EDD_NRT_RCV_MAX_FRAME_TO_LOOK_UP_B0
   #define LSAS_EDD_NRT_RCV_MAX_FRAME_TO_LOOK_UP_B0 10
#endif
#ifndef LSAS_EDD_NRT_RCV_MAX_FRAME_SEND_TO_USER_B0
   #define LSAS_EDD_NRT_RCV_MAX_FRAME_SEND_TO_USER_B0 4
#endif

#ifndef LSAS_CFG_GET_HW_PORT_IDX
    #define LSAS_CFG_GET_HW_PORT_IDX(_PORT_ID) _PORT_ID
#endif

#ifndef LSAS_ATTR_CODE_FAST
    #define LSAS_ATTR_CODE_FAST
#endif

#endif /* LSAS_PLAU_H_ */

/*** end of file *************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
