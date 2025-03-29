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
/*  F i l e               &F: cm_cfg.h                                  :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Implements system integration of CM using the pcPNIO framework           */
/*                                                                           */
/*****************************************************************************/
#ifndef CM_CFG_H
#define CM_CFG_H


/*
 * included by "cm_inc.h"
 */

/*===========================================================================*/
/*                             compiler-switches                             */
/*===========================================================================*/


#define CM_DEBUG                   0


/*===========================================================================*/
/*                         code- and data-attributes                         */
/*===========================================================================*/

/*
 * all known platforms have a "flat" memory-model
 */


/*===========================================================================*/
/* CM-HEADER and CM-TRAILER                                                  */
/*===========================================================================*/

#define CM_RQB_HEADER                           LSA_RQB_HEADER(CM_UPPER_RQB_PTR_TYPE)
#define CM_RQB_TRAILER                          LSA_RQB_TRAILER


#define CM_RQB_SET_NEXT_RQB_PTR(rb_, val_)      LSA_RQB_SET_NEXT_RQB_PTR(rb_, val_)
#define CM_RQB_GET_NEXT_RQB_PTR(rb_)            LSA_RQB_GET_NEXT_RQB_PTR(rb_)

#define CM_RQB_SET_PREV_RQB_PTR(rb_, val_)      LSA_RQB_SET_PREV_RQB_PTR(rb_, val_)
#define CM_RQB_GET_PREV_RQB_PTR(rb_)            LSA_RQB_GET_PREV_RQB_PTR(rb_)

#define CM_RQB_SET_OPCODE(rb_, val_)            LSA_RQB_SET_OPCODE(rb_, val_)
#define CM_RQB_GET_OPCODE(rb_)                  LSA_RQB_GET_OPCODE(rb_)

#define CM_RQB_SET_HANDLE(rb_, val_)            LSA_RQB_SET_HANDLE(rb_, val_)
#define CM_RQB_GET_HANDLE(rb_)                  LSA_RQB_GET_HANDLE(rb_)

#define CM_RQB_SET_USER_ID_PTR(rb_, val_)       LSA_RQB_SET_USER_ID_PTR(rb_, val_)
#define CM_RQB_GET_USER_ID_PTR(rb_)             LSA_RQB_GET_USER_ID_PTR(rb_)

#define CM_RQB_SET_RESPONSE(rqb_, v_)           LSA_RQB_SET_RESPONSE(rqb_, v_)
#define CM_RQB_GET_RESPONSE(rqb_)               LSA_RQB_GET_RESPONSE(rqb_)


/*===========================================================================*/
/* LOWER LAYER MACROS                                                        */
/*===========================================================================*/

/*
 * ACP
 */

#define CM_ACP_LOWER_RQB_TYPE       ACP_RQB_TYPE

#define CM_ACP_SET_NEXT_RQB_PTR(rb_, val_)  ACP_RQB_SET_NEXT_RQB_PTR(rb_, val_)
#define CM_ACP_SET_PREV_RQB_PTR(rb_, val_)  ACP_RQB_SET_PREV_RQB_PTR(rb_, val_)
#define CM_ACP_SET_OPCODE(rb_, val_)        ACP_RQB_SET_OPCODE(rb_, val_)
#define CM_ACP_SET_HANDLE(rb_, val_)        ACP_RQB_SET_HANDLE(rb_, val_)
#define CM_ACP_SET_USER_ID_PTR(rb_, val_)   ACP_RQB_SET_USER_ID_PTR(rb_, val_)
#define CM_ACP_SET_RESPONSE(rb_, val_)      ACP_RQB_SET_RESPONSE(rb_, val_)

#define CM_ACP_GET_NEXT_RQB_PTR(rb_)        ACP_RQB_GET_NEXT_RQB_PTR(rb_)
#define CM_ACP_GET_PREV_RQB_PTR(rb_)        ACP_RQB_GET_PREV_RQB_PTR(rb_)
#define CM_ACP_GET_OPCODE(rb_)              ACP_RQB_GET_OPCODE(rb_)
#define CM_ACP_GET_HANDLE(rb_)              ACP_RQB_GET_HANDLE(rb_)
#define CM_ACP_GET_USER_ID_PTR(rb_)         ACP_RQB_GET_USER_ID_PTR(rb_)
#define CM_ACP_GET_RESPONSE(rb_)            ACP_RQB_GET_RESPONSE(rb_)

#define CM_ACP_SET_ARGS(rb_, val_)          ACP_RQB_SET_ARGS(rb_, val_)
#define CM_ACP_GET_ARGS(rb_)                ACP_RQB_GET_ARGS(rb_)

/*
 * EDD
 */

#define CM_EDD_INVALID_HANDLE   0xFF /* EDD doesn't export one */

#define CM_EDD_LOWER_RQB_TYPE   EDD_RQB_TYPE

# define CM_EDD_SET_NEXT_RQB_PTR(rb_, val_) EDD_RQB_SET_NEXT_RQB_PTR(rb_, val_)
# define CM_EDD_SET_PREV_RQB_PTR(rb_, val_) EDD_RQB_SET_PREV_RQB_PTR(rb_, val_)
# define CM_EDD_SET_OPCODE(rb_, opc_, svc_) { EDD_RQB_SET_OPCODE(rb_, opc_); EDD_RQB_SET_SERVICE(rb_, svc_); }
# define CM_EDD_SET_HANDLE(rb_, val_)       EDD_RQB_SET_HANDLE(rb_, val_)
# define CM_EDD_SET_USER_ID_PTR(rb_, val_)  EDD_RQB_SET_USERID_PTR(rb_, val_)
# define CM_EDD_SET_RESPONSE(rb_, val_)     EDD_RQB_SET_RESPONSE(rb_, val_)

# define CM_EDD_GET_NEXT_RQB_PTR(rb_)       EDD_RQB_GET_NEXT_RQB_PTR(rb_)
# define CM_EDD_GET_PREV_RQB_PTR(rb_)       EDD_RQB_GET_PREV_RQB_PTR(rb_)
# define CM_EDD_GET_OPCODE(rb_)             EDD_RQB_GET_OPCODE(rb_)
# define CM_EDD_GET_SERVICE(rb_)            EDD_RQB_GET_SERVICE(rb_)
# define CM_EDD_GET_HANDLE(rb_)             EDD_RQB_GET_HANDLE(rb_)
# define CM_EDD_GET_USER_ID_PTR(rb_)        EDD_RQB_GET_USERID_PTR(rb_)
# define CM_EDD_GET_RESPONSE(rb_)           EDD_RQB_GET_RESPONSE(rb_)

#define CM_EDD_SET_USER_ID_HANDLE_DEV_AR(rb_, h_, dev_, ar_) { /* store channel_handle, device_nr and ar_nr */ \
    LSA_RQB_SET_USER_ID_UVAR16_ARRAY_LOW(rb_, dev_);                                                    \
    LSA_RQB_SET_USER_ID_UVAR8_ARRAY_2(rb_, h_);                                                         \
    LSA_RQB_SET_USER_ID_UVAR8_ARRAY_3(rb_, (LSA_UINT8)(ar_)); /*see CM_CFG_MAX_CL/SV_ARS*/              \
    }

#define CM_EDD_GET_USER_ID_DEV(rb_) ( /* get device_nr */ \
    LSA_RQB_GET_USER_ID_UVAR16_ARRAY_LOW(rb_) \
    )

#define CM_EDD_GET_USER_ID_HANDLE(rb_) ( /* get channel_handle */ \
    LSA_RQB_GET_USER_ID_UVAR8_ARRAY_2(rb_) \
    )

#define CM_EDD_GET_USER_ID_AR(rb_) ( /* get ar_nr */ \
    LSA_RQB_GET_USER_ID_UVAR8_ARRAY_3(rb_) \
    )


/*
 * GSY
 */
# define CM_CFG_USE_GSY  1 /* 0.. no GSY, 1..use GSY */
#ifndef CM_CFG_USE_GSY
# error "CM_CFG_USE_GSY not defined" /* 0 = no GSY, 1 = use GSY */
#endif

#if CM_CFG_USE_GSY

#define CM_GSY_INVALID_HANDLE   0xFF /* GSY doesn't export one */

#define CM_GSY_LOWER_RQB_TYPE   GSY_RQB_TYPE

# define CM_GSY_SET_NEXT_RQB_PTR(rb_, val_) GSY_RQB_SET_NEXT_RQB_PTR(rb_, val_)
# define CM_GSY_SET_PREV_RQB_PTR(rb_, val_) GSY_RQB_SET_PREV_RQB_PTR(rb_, val_)
# define CM_GSY_SET_OPCODE(rb_, val_)       GSY_RQB_SET_OPCODE(rb_, val_)
# define CM_GSY_SET_HANDLE(rb_, val_)       GSY_RQB_SET_HANDLE(rb_, val_)
# define CM_GSY_SET_USER_ID_PTR(rb_, val_)  GSY_RQB_SET_USER_ID_PTR(rb_, val_)
# define CM_GSY_SET_RESPONSE(rb_, val_)     GSY_RQB_SET_RESPONSE(rb_, val_)

# define CM_GSY_GET_NEXT_RQB_PTR(rb_)       GSY_RQB_GET_NEXT_RQB_PTR(rb_)
# define CM_GSY_GET_PREV_RQB_PTR(rb_)       GSY_RQB_GET_PREV_RQB_PTR(rb_)
# define CM_GSY_GET_OPCODE(rb_)             GSY_RQB_GET_OPCODE(rb_)
# define CM_GSY_GET_HANDLE(rb_)             GSY_RQB_GET_HANDLE(rb_)
# define CM_GSY_GET_USER_ID_PTR(rb_)        GSY_RQB_GET_USER_ID_PTR(rb_)
# define CM_GSY_GET_RESPONSE(rb_)           GSY_RQB_GET_RESPONSE(rb_)

#define CM_GSY_SET_USER_ID_HANDLE_DEV_AR(rb_, h_, dev_, ar_) { /* store channel_handle, device_nr and ar_nr */ \
    LSA_RQB_SET_USER_ID_UVAR16_ARRAY_LOW(rb_, dev_);                                                    \
    LSA_RQB_SET_USER_ID_UVAR8_ARRAY_2(rb_, h_);                                                         \
    LSA_RQB_SET_USER_ID_UVAR8_ARRAY_3(rb_, (LSA_UINT8)(ar_)); /*see CM_CFG_MAX_CL/SV_ARS*/              \
    }

#define CM_GSY_GET_USER_ID_DEV(rb_) ( /* get device_nr */ \
    LSA_RQB_GET_USER_ID_UVAR16_ARRAY_LOW(rb_) \
    )

#define CM_GSY_GET_USER_ID_HANDLE(rb_) ( /* get channel_handle */ \
    LSA_RQB_GET_USER_ID_UVAR8_ARRAY_2(rb_) \
    )

#define CM_GSY_GET_USER_ID_AR(rb_) ( /* get ar_nr */ \
    LSA_RQB_GET_USER_ID_UVAR8_ARRAY_3(rb_) \
    )

#endif


/*
 * NARE
 */
#define CM_NARE_INVALID_HANDLE  0xFF /* with very high propability, see nare_int.h */

#define CM_NARE_LOWER_RQB_TYPE  NARE_RQB_TYPE

# define CM_NARE_SET_NEXT_RQB_PTR(rb_, val_)    LSA_RQB_SET_NEXT_RQB_PTR(rb_, val_)
# define CM_NARE_SET_PREV_RQB_PTR(rb_, val_)    LSA_RQB_SET_PREV_RQB_PTR(rb_, val_)
# define CM_NARE_SET_OPCODE(rb_, val_)          LSA_RQB_SET_OPCODE(rb_, val_)
# define CM_NARE_SET_HANDLE(rb_, val_)          LSA_RQB_SET_HANDLE(rb_, val_)
# define CM_NARE_SET_USER_ID_PTR(rb_, val_)     LSA_RQB_SET_USER_ID_PTR(rb_, val_)
# define CM_NARE_SET_RESPONSE(rb_, val_)        LSA_RQB_SET_RESPONSE(rb_, val_)

# define CM_NARE_GET_NEXT_RQB_PTR(rb_)          LSA_RQB_GET_NEXT_RQB_PTR(rb_)
# define CM_NARE_GET_PREV_RQB_PTR(rb_)          LSA_RQB_GET_PREV_RQB_PTR(rb_)
# define CM_NARE_GET_OPCODE(rb_)                LSA_RQB_GET_OPCODE(rb_)
# define CM_NARE_GET_HANDLE(rb_)                LSA_RQB_GET_HANDLE(rb_)
# define CM_NARE_GET_USER_ID_PTR(rb_)           LSA_RQB_GET_USER_ID_PTR(rb_)
# define CM_NARE_GET_RESPONSE(rb_)              LSA_RQB_GET_RESPONSE(rb_)

#define CM_NARE_SET_USER_ID_HANDLE_DEV_AR(rb_, h_, dev_, ar_) { /* store channel_handle, device_nr and ar_nr */ \
    LSA_RQB_SET_USER_ID_UVAR16_ARRAY_LOW(rb_, dev_);                                        \
    LSA_RQB_SET_USER_ID_UVAR8_ARRAY_2(rb_, h_);                                             \
    LSA_RQB_SET_USER_ID_UVAR8_ARRAY_3(rb_, (LSA_UINT8)(ar_)); /*see CM_CFG_MAX_CL/SV_ARS*/  \
    }

#define CM_NARE_GET_USER_ID_DEV(rb_) ( /* get device_nr */ \
    LSA_RQB_GET_USER_ID_UVAR16_ARRAY_LOW(rb_)                                               \
    )

#define CM_NARE_GET_USER_ID_HANDLE(rb_) ( /* get channel_handle */ \
    LSA_RQB_GET_USER_ID_UVAR8_ARRAY_2(rb_)                                                  \
    )

#define CM_NARE_GET_USER_ID_AR(rb_) ( /* get ar_nr */ \
    LSA_RQB_GET_USER_ID_UVAR8_ARRAY_3(rb_)                                                  \
    )

/*
 * OHA
 */

#define CM_OHA_INVALID_HANDLE   OHA_INVALID_HANDLE

#define CM_OHA_LOWER_RQB_TYPE   OHA_RQB_TYPE

# define CM_OHA_SET_NEXT_RQB_PTR(rb_, val_) OHA_RQB_SET_NEXT_RQB_PTR(rb_, val_)
# define CM_OHA_SET_PREV_RQB_PTR(rb_, val_) OHA_RQB_SET_PREV_RQB_PTR(rb_, val_)
# define CM_OHA_SET_OPCODE(rb_, val_)       OHA_RQB_SET_OPCODE(rb_, val_)
# define CM_OHA_SET_HANDLE(rb_, val_)       OHA_RQB_SET_HANDLE(rb_, val_)
# define CM_OHA_SET_USER_ID_PTR(rb_, val_)  OHA_RQB_SET_USER_ID_PTR(rb_, val_)
# define CM_OHA_SET_RESPONSE(rb_, val_)     OHA_RQB_SET_RESPONSE(rb_, val_)

# define CM_OHA_GET_NEXT_RQB_PTR(rb_)       OHA_RQB_GET_NEXT_RQB_PTR(rb_)
# define CM_OHA_GET_PREV_RQB_PTR(rb_)       OHA_RQB_GET_PREV_RQB_PTR(rb_)
# define CM_OHA_GET_OPCODE(rb_)             OHA_RQB_GET_OPCODE(rb_)
# define CM_OHA_GET_HANDLE(rb_)             OHA_RQB_GET_HANDLE(rb_)
# define CM_OHA_GET_USER_ID_PTR(rb_)        OHA_RQB_GET_USER_ID_PTR(rb_)
# define CM_OHA_GET_RESPONSE(rb_)           OHA_RQB_GET_RESPONSE(rb_)

#define CM_OHA_SET_USER_ID_HANDLE_DEV_AR(rb_, h_, dev_, ar_) { /* store channel_handle, device_nr and ar_nr */ \
    LSA_RQB_SET_USER_ID_UVAR16_ARRAY_LOW(rb_, dev_);                                        \
    LSA_RQB_SET_USER_ID_UVAR8_ARRAY_2(rb_, h_);                                             \
    LSA_RQB_SET_USER_ID_UVAR8_ARRAY_3(rb_, (LSA_UINT8)(ar_)); /*see CM_CFG_MAX_CL/SV_ARS*/  \
    }

#define CM_OHA_GET_USER_ID_DEV(rb_) ( /* get device_nr */ \
    LSA_RQB_GET_USER_ID_UVAR16_ARRAY_LOW(rb_) \
    )

#define CM_OHA_GET_USER_ID_HANDLE(rb_) ( /* get channel_handle */ \
    LSA_RQB_GET_USER_ID_UVAR8_ARRAY_2(rb_) \
    )

#define CM_OHA_GET_USER_ID_AR(rb_) ( /* get ar_nr */ \
    LSA_RQB_GET_USER_ID_UVAR8_ARRAY_3(rb_) \
    )


/*
 * RPC
 */

#define CM_RPC_INVALID_HANDLE       CLRPC_INVALID_HANDLE
#define CM_RPC_INVALID_CLIENT_ID    CLRPC_INVALID_CLIENT_ID

#define CM_RPC_LOWER_RQB_TYPE       CLRPC_RQB_TYPE

#define CM_RPC_SET_NEXT_RQB_PTR(rb_, val_)  CLRPC_RQB_SET_NEXT_RQB_PTR(rb_, val_)
#define CM_RPC_SET_PREV_RQB_PTR(rb_, val_)  CLRPC_RQB_SET_PREV_RQB_PTR(rb_, val_)
#define CM_RPC_SET_OPCODE(rb_, val_)        CLRPC_RQB_SET_OPCODE(rb_, val_)
#define CM_RPC_SET_HANDLE(rb_, val_)        CLRPC_RQB_SET_HANDLE(rb_, val_)

#define CM_RPC_GET_NEXT_RQB_PTR(rb_)        CLRPC_RQB_GET_NEXT_RQB_PTR(rb_)
#define CM_RPC_GET_PREV_RQB_PTR(rb_)        CLRPC_RQB_GET_PREV_RQB_PTR(rb_)
#define CM_RPC_GET_OPCODE(rb_)              CLRPC_RQB_GET_OPCODE(rb_)
#define CM_RPC_GET_HANDLE(rb_)              CLRPC_RQB_GET_HANDLE(rb_)

#define CM_RPC_SET_USER_ID_PTR(rb_, val_)   CLRPC_RQB_SET_USER_ID_PTR(rb_, val_)
#define CM_RPC_GET_USER_ID_PTR(rb_)         CLRPC_RQB_GET_USER_ID_PTR(rb_)

#define CM_RPC_SET_RESPONSE(rb_, val_)      CLRPC_RQB_SET_RESPONSE(rb_, val_)
#define CM_RPC_GET_RESPONSE(rb_)            CLRPC_RQB_GET_RESPONSE(rb_)

#define CM_RPC_SET_ARGS(rb_, val_)          CLRPC_RQB_SET_ARGS(rb_, val_)
#define CM_RPC_GET_ARGS(rb_)                CLRPC_RQB_GET_ARGS(rb_)

#define CM_RPC_SET_USER_ID_HANDLE_DEV_AR(rb_, h_, dev_, ar_) { /* store channel_handle, device_nr and ar_nr */ \
    LSA_RQB_SET_USER_ID_UVAR16_ARRAY_LOW(rb_, dev_);                                        \
    LSA_RQB_SET_USER_ID_UVAR8_ARRAY_2(rb_, h_);                                             \
    LSA_RQB_SET_USER_ID_UVAR8_ARRAY_3(rb_, (LSA_UINT8)(ar_)); /*see CM_CFG_MAX_CL/SV_ARS*/  \
    }

#define CM_RPC_GET_USER_ID_DEV(rb_) ( /* get device_nr */ \
    LSA_RQB_GET_USER_ID_UVAR16_ARRAY_LOW(rb_)                                               \
    )

#define CM_RPC_GET_USER_ID_HANDLE(rb_) ( /* get channel_handle */ \
    LSA_RQB_GET_USER_ID_UVAR8_ARRAY_2(rb_)                                                  \
    )

#define CM_RPC_GET_USER_ID_AR(rb_) ( /* get ar_nr */ \
    LSA_RQB_GET_USER_ID_UVAR8_ARRAY_3(rb_)                                                  \
    )


/*
 * MRP
 */
#if (defined LSAS_CFG_USE_MRP) && (LSAS_CFG_USE_MRP)
# define CM_CFG_USE_MRP  1 /* 0.. no MRP, 1..use MRP */
#else
# define CM_CFG_USE_MRP  0 /* 0.. no MRP, 1..use MRP */
#endif

#if CM_CFG_USE_MRP

#define CM_MRP_INVALID_HANDLE   0xFF /* with very high propability, see mrp_int.h */

#define CM_MRP_LOWER_RQB_TYPE   MRP_RQB_TYPE

# define CM_MRP_SET_NEXT_RQB_PTR(rb_, val_)     MRP_RQB_SET_NEXT_RQB_PTR(rb_, val_)
# define CM_MRP_SET_PREV_RQB_PTR(rb_, val_)     MRP_RQB_SET_PREV_RQB_PTR(rb_, val_)
# define CM_MRP_SET_OPCODE(rb_, val_)           MRP_RQB_SET_OPCODE(rb_, val_)
# define CM_MRP_SET_HANDLE(rb_, val_)           MRP_RQB_SET_HANDLE(rb_, val_)
# define CM_MRP_SET_USER_ID_PTR(rb_, val_)      MRP_RQB_SET_USER_ID_PTR(rb_, val_)
# define CM_MRP_SET_RESPONSE(rb_, val_)         MRP_RQB_SET_RESPONSE(rb_, val_)

# define CM_MRP_GET_NEXT_RQB_PTR(rb_)           MRP_RQB_GET_NEXT_RQB_PTR(rb_)
# define CM_MRP_GET_PREV_RQB_PTR(rb_)           MRP_RQB_GET_PREV_RQB_PTR(rb_)
# define CM_MRP_GET_OPCODE(rb_)                 MRP_RQB_GET_OPCODE(rb_)
# define CM_MRP_GET_HANDLE(rb_)                 MRP_RQB_GET_HANDLE(rb_)
# define CM_MRP_GET_USER_ID_PTR(rb_)            MRP_RQB_GET_USER_ID_PTR(rb_)
# define CM_MRP_GET_RESPONSE(rb_)               MRP_RQB_GET_RESPONSE(rb_)

#define CM_MRP_SET_USER_ID_HANDLE_DEV_AR(rb_, h_, dev_, ar_) { /* store channel_handle, device_nr and ar_nr */ \
    LSA_RQB_SET_USER_ID_UVAR16_ARRAY_LOW(rb_, dev_);                                        \
    LSA_RQB_SET_USER_ID_UVAR8_ARRAY_2(rb_, h_);                                             \
    LSA_RQB_SET_USER_ID_UVAR8_ARRAY_3(rb_, (LSA_UINT8)(ar_)); /*see CM_CFG_MAX_CL/SV_ARS*/  \
    }

#define CM_MRP_GET_USER_ID_DEV(rb_) ( /* get device_nr */ \
    LSA_RQB_GET_USER_ID_UVAR16_ARRAY_LOW(rb_) \
    )

#define CM_MRP_GET_USER_ID_HANDLE(rb_) ( /* get channel_handle */ \
    LSA_RQB_GET_USER_ID_UVAR8_ARRAY_2(rb_) \
    )

#define CM_MRP_GET_USER_ID_AR(rb_) ( /* get ar_nr */ \
    LSA_RQB_GET_USER_ID_UVAR8_ARRAY_3(rb_) \
    )

#else

#endif


/*
 * POF
 */
#ifdef LSAS_CFG_USE_POF
    #define CM_CFG_USE_POF  1 /* 1..use POF */
#else
    #define CM_CFG_USE_POF  0 /* 0..no POF */
#endif

#ifndef CM_CFG_USE_POF
   #error "CM_CFG_USE_POF not defined" /* 0 = no POF, 1 = use POF */
#endif

#if CM_CFG_USE_POF

#define CM_POF_INVALID_HANDLE   0xFF /* with very high propability, see POF_GetUserHDBFromHandle() */

#define CM_POF_LOWER_RQB_TYPE   POF_RQB_TYPE

# define CM_POF_SET_NEXT_RQB_PTR(rb_, val_)     POF_RQB_SET_NEXT_RQB_PTR(rb_, val_)
# define CM_POF_SET_PREV_RQB_PTR(rb_, val_)     POF_RQB_SET_PREV_RQB_PTR(rb_, val_)
# define CM_POF_SET_OPCODE(rb_, val_)           POF_RQB_SET_OPCODE(rb_, val_)
# define CM_POF_SET_HANDLE(rb_, val_)           POF_RQB_SET_HANDLE(rb_, val_)
# define CM_POF_SET_USER_ID_PTR(rb_, val_)      POF_RQB_SET_USER_ID_PTR(rb_, val_)
# define CM_POF_SET_RESPONSE(rb_, val_)         POF_RQB_SET_RESPONSE(rb_, val_)

# define CM_POF_GET_NEXT_RQB_PTR(rb_)           POF_RQB_GET_NEXT_RQB_PTR(rb_)
# define CM_POF_GET_PREV_RQB_PTR(rb_)           POF_RQB_GET_PREV_RQB_PTR(rb_)
# define CM_POF_GET_OPCODE(rb_)                 POF_RQB_GET_OPCODE(rb_)
# define CM_POF_GET_HANDLE(rb_)                 POF_RQB_GET_HANDLE(rb_)
# define CM_POF_GET_USER_ID_PTR(rb_)            POF_RQB_GET_USER_ID_PTR(rb_)
# define CM_POF_GET_RESPONSE(rb_)               POF_RQB_GET_RESPONSE(rb_)

#define CM_POF_SET_USER_ID_HANDLE_DEV_AR(rb_, h_, dev_, ar_) { /* store channel_handle, device_nr and ar_nr */ \
    LSA_RQB_SET_USER_ID_UVAR16_ARRAY_LOW(rb_, dev_);                                                    \
    LSA_RQB_SET_USER_ID_UVAR8_ARRAY_2(rb_, h_);                                                         \
    LSA_RQB_SET_USER_ID_UVAR8_ARRAY_3(rb_, (LSA_UINT8)(ar_)); /*see CM_CFG_MAX_CL/SV_ARS*/              \
    }

#define CM_POF_GET_USER_ID_DEV(rb_) ( /* get device_nr */ \
    LSA_RQB_GET_USER_ID_UVAR16_ARRAY_LOW(rb_) \
    )

#define CM_POF_GET_USER_ID_HANDLE(rb_) ( /* get channel_handle */ \
    LSA_RQB_GET_USER_ID_UVAR8_ARRAY_2(rb_) \
    )

#define CM_POF_GET_USER_ID_AR(rb_) ( /* get ar_nr */ \
    LSA_RQB_GET_USER_ID_UVAR8_ARRAY_3(rb_) \
    )

#endif


/*=============================================================================
 *       set local_mem static or dynamic:
 * static (no use of CM_ALLOC_LOCAL_MEM):
 * #define CM_LOCAL_MEM_STATIC
 * or dynamic (use of CM_ALLOC_LOCAL_MEM):
 * #define CM_LOCAL_MEM_DYNAMIC
 *===========================================================================*/
#define CM_LOCAL_MEM_DYNAMIC /* don't change. there is no choice! */


/*=============================================================================
 *       clear global data:
 * clear (CM_DATA_TYPE cm_data = {0};):
 * #define CM_CLEAR_GLOBAL_DATA
 * or no clear:
 * #define CM_NO_CLEAR_GLOBAL_DATA
 *===========================================================================*/
#define CM_NO_CLEAR_GLOBAL_DATA /* don't change. there is no choice! */


/*===========================================================================*/
/* configurable things                                                       */
/*===========================================================================*/

 #define CM_CFG_USE_CASTS                      1   /* 1 = use casts to avoid some warnings */

#ifndef CM_CFG_USE_CASTS
# define CM_CFG_USE_CASTS 0 /* 1 = use casts to avoid some warnings */
#endif

/*---------------------------------------------------------------------------*/
#define CM_CFG_MAX_CLIENTS                     0   /* 0 = no client functionality */

#if CM_CFG_MAX_CLIENTS

 #define CM_CFG_MAX_CL_DEVICES 1 /* 1 .. 2048 devices per channel, real count will assigned with cm_cl_create() (per client-channel) */

 #define CM_CFG_MAX_CL_LOG_ENTRIES 12 /* current session + IN_DATA/OFFLINE for old session, see cm_cl_logbook_lbentry_type_enum (per client-device) */

 #ifndef CM_CFG_MAX_CL_LOG_ENTRIES
# error "CM_CFG_MAX_CL_LOG_ENTRIES not defined" /* current-session + IN_DATA/OFFLINE for old-session, see cm_cl_logbook_lbentry_type_enum (per client-device) */
 #endif

 #define CM_CFG_MAX_CL_IMPLICIT_READ 1 /* 0..n: max. number of concurrently running implicit reads (per client-channel) */

 #define CM_CFG_MAX_CL_FSU_DEVICES EDD_DCP_MAX_DCP_HELLO_FILTER /* 0..EDD_DCP_MAX_DCP_HELLO_FILTER: max. number of supported Fast-StartUp-devices */

 #ifndef CM_CFG_MAX_CL_FSU_DEVICES
# error "CM_CFG_MAX_CL_FSU_DEVICES not defined"  /* 0 = no Fast-StartUp-devices, 1..EDD_DCP_MAX_DCP_HELLO_FILTER = max. number of supported Fast-StartUp-devices */
 #endif

#endif

/*---------------------------------------------------------------------------*/

#define CM_CFG_MAX_SERVERS                     1   /* 0 = no server functionality */

#if CM_CFG_MAX_SERVERS

 #define CM_CFG_MAX_SV_DEVICES                  1   /* 1 .. 2048 per server */

 #define CM_CFG_MAX_SV_LOG_ENTRIES              127

#define CM_CFG_MAX_SV_ARSETS_PER_DEVICE 1

 #ifndef CM_CFG_MAX_SV_LOG_ENTRIES
 # error "CM_CFG_MAX_SV_LOG_ENTRIES not defined"  /* per device (min=16, see part 6)*/
 #endif

 #define CM_CFG_MAX_SV_ARS                      CLRPC_CFG_MAX_CLIENTS /* 2 .. 16 per device */

 #define CM_CFG_SV_AR_HAS_MCONS                 0 /* 0 = AR has no m-consumers, else must CM_CFG_USE_NARE too! */

 #ifndef CM_CFG_SV_AR_HAS_MCONS
 # error "CM_CFG_SV_AR_HAS_MCONS not defined" /* 0 = AR has no m-consumers, else must CM_CFG_USE_NARE too! */
 #endif

#endif

/*---------------------------------------------------------------------------*/

#define CM_CFG_MAX_MULTICAST_CHANNELS          0

/*------------------------------------------------------------------------------
//  Assert
//
//  ==> MUST BE EMPTY in Release-Version!
//
//  check if condition holds or call fatal error otherwise (programming error)
//
//----------------------------------------------------------------------------*/

#if CM_DEBUG

#define CM_TEST_POINTER(ptr_)  (ptr_) /* dont test */

    /* check if condition holds or call fatal (programming error) */
#define CM_ASSERT(cond_)                                                                                 \
{                                                                                                        \
    LSA_BOOL tmp_cond = (LSA_BOOL)(cond_);                                                               \
                                                                                                         \
    if (!(tmp_cond))                                                                                     \
    {                                                                                                    \
        CM_FATAL();\
    }                                                                                                    \
}                                                                                                        \

#else

#define CM_TEST_POINTER(ptr_)  (ptr_) /* don't test */

#define CM_ASSERT(cond_) { /* empty-block in release version */ }

#endif

/*------------------------------------------------------------------------------*/
/*  System Layer (prototypes in cm_sys.h)                                       */
/*------------------------------------------------------------------------------*/

/***/

#define CM_FATAL_ERROR(length,error_ptr)                                                    \
{                                                                                           \
    LSAS_FATAL_ERROR( error_ptr);                                   \
}                                                                                           \

/*------------------------------------------------------------------------------*/
/*  Upper Layer (prototypes in cm_usr.h)                                        */
/*------------------------------------------------------------------------------*/

#define CM_HTON16(host_short)                                                               \
(                                                                                           \
    LSAS_HTONS(host_short)                                                                   \
)                                                                                           \

#define CM_HTON32(host_long)                                                                \
(                                                                                           \
    LSAS_HTONL(host_long)                                                                    \
)                                                                                           \

#define CM_NTOH16(network_short)                                                            \
(                                                                                           \
    LSAS_NTOHS(network_short)                                                                \
)                                                                                           \

#define CM_NTOH32(network_long)                                                             \
(                                                                                           \
    LSAS_NTOHL(network_long)                                                                 \
)                                                                                           \


/*------------------------------------------------------------------------------
// Library
//----------------------------------------------------------------------------*/

#define CM_MEMSET(ptr,val,len)                                                              \
{                                                                                           \
    LSAS_MEMSET(ptr,val,len);                                                                \
}                                                                                           \

#define CM_MEMCPY(dst,src,len)                                                              \
{                                                                                           \
    LSAS_MEMCPY(dst,src,len);                                                                \
}                                                                                           \


#define CM_MEMCMP(_a,_b,len) LSAS_MEMCMP(_a,_b,len)


#define CM_MEMMOVE(dst,src,len)\
{\
    LSAS_MEMMOVE(dst,src,len);\
}\


/*------------------------------------------------------------------------------
// Put / Get
//----------------------------------------------------------------------------*/
#if 0
#define CM_PUT8_HTON(base,offset,val)                                                       \
{/* put and convert from host byte order to network byte order */                           \
    LSAS_PUT8_HTON(base,offset,val);                                                        \
}                                                                                           \

#define CM_GET8_NTOH(base,offset)                                                           \
(/* get and convert from network byte order to host byte order */                           \
    LSAS_GET8_NTOH(base,offset)                                                             \
)

#define CM_PUT16_HTON(base,offset,val)                                                      \
{/* put and convert from host byte order to network byte order */                           \
    LSAS_PUT16_HTON(base,offset,val);                                                       \
}                                                                                           \

#define CM_GET16_NTOH(base,offset)                                                          \
(/* get and convert from network byte order to host byte order */                           \
    LSAS_GET16_NTOH(base,offset)                                                            \
)

#define CM_PUT32_HTON(base,offset,val)                                                      \
{/* put and convert from host byte order to network byte order */                           \
    LSAS_PUT32_HTON(base,offset,val);                                                       \
}                                                                                           \

#define CM_GET32_NTOH(base,offset)                                                          \
(/* get and convert from network byte order to host byte order */                           \
    LSAS_GET32_NTOH(base,offset)                                                            \
)
#endif

#define CM_PUT_UUID_HTON(base,offset,uuid)                                                  \
{/* put and convert from host byte order to network byte order */                           \
    /* LSA_UINT32      time_low;                   */                                       \
    /* LSA_UINT16      time_mid;                   */                                       \
    /* LSA_UINT16      time_hi_and_version;        */                                       \
    /* LSA_UINT8       clock_seq_hi_and_reserved;  */                                       \
    /* LSA_UINT8       clock_seq_low;              */                                       \
    /* LSA_UINT8       node[6];                    */                                       \
                                                                                            \
    CM_PUT32_HTON((base), (offset) +  0, (uuid)->time_low);                                 \
    CM_PUT16_HTON((base), (offset) +  4, (uuid)->time_mid);                                 \
    CM_PUT16_HTON((base), (offset) +  6, (uuid)->time_hi_and_version);                      \
    CM_PUT8_HTON ((base), (offset) +  8, (uuid)->clock_seq_hi_and_reserved);                \
    CM_PUT8_HTON ((base), (offset) +  9, (uuid)->clock_seq_low);                            \
    CM_PUT8_HTON ((base), (offset) + 10, (uuid)->node[0]);                                  \
    CM_PUT8_HTON ((base), (offset) + 11, (uuid)->node[1]);                                  \
    CM_PUT8_HTON ((base), (offset) + 12, (uuid)->node[2]);                                  \
    CM_PUT8_HTON ((base), (offset) + 13, (uuid)->node[3]);                                  \
    CM_PUT8_HTON ((base), (offset) + 14, (uuid)->node[4]);                                  \
    CM_PUT8_HTON ((base), (offset) + 15, (uuid)->node[5]);                                  \
}                                                                                           \

#define CM_GET_UUID_NTOH(uuid,base,offset)                                                  \
{/* get and convert from network byte order to host byte order */                           \
    (uuid)->time_low                  = CM_GET32_NTOH((base), (offset) +  0);               \
    (uuid)->time_mid                  = CM_GET16_NTOH((base), (offset) +  4);               \
    (uuid)->time_hi_and_version       = CM_GET16_NTOH((base), (offset) +  6);               \
    (uuid)->clock_seq_hi_and_reserved = CM_GET8_NTOH ((base), (offset) +  8);               \
    (uuid)->clock_seq_low             = CM_GET8_NTOH ((base), (offset) +  9);               \
    (uuid)->node[0]                   = CM_GET8_NTOH ((base), (offset) + 10);               \
    (uuid)->node[1]                   = CM_GET8_NTOH ((base), (offset) + 11);               \
    (uuid)->node[2]                   = CM_GET8_NTOH ((base), (offset) + 12);               \
    (uuid)->node[3]                   = CM_GET8_NTOH ((base), (offset) + 13);               \
    (uuid)->node[4]                   = CM_GET8_NTOH ((base), (offset) + 14);               \
    (uuid)->node[5]                   = CM_GET8_NTOH ((base), (offset) + 15);               \
}                                                                                           \

/*------------------------------------------------------------------------------
//    Lower Layer (prototypes in cm_low.h)
//----------------------------------------------------------------------------*/

/*
 * all "macros" are implemented as functions in cm_cfg.c as an example
 */

 /*------------------------------------------------------------------------------
// interface to LSA trace
//    0 = no LSA trace
//  1 = LSA trace
//  2 = LSA index trace
//----------------------------------------------------------------------------*/

#ifndef CM_CFG_TRACE_MODE
# define CM_CFG_TRACE_MODE  1
#endif

#if (CM_CFG_TRACE_MODE == 1)

#define CM_TRACE_LEVEL_TYPE  LSA_UINT16

#endif


/*------------------------------------------------------------------------------
// interface to BTRACE
//----------------------------------------------------------------------------*/

#ifndef CM_FILE_SYSTEM_EXTENSION
# define CM_FILE_SYSTEM_EXTENSION(module_id_)   /* nothing */
#endif


#endif  /* of CM_CFG_H */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
