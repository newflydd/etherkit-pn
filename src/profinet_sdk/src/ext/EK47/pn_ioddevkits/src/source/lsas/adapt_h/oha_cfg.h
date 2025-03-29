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
/*  F i l e               &F: oha_cfg.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Configuration for OHA:                                                   */
/*  Defines constants, types and macros for OHA.                             */
/*                                                                           */
/*  This file has to be overwritten during system integration, because       */
/*  some definitions depend on the different system, compiler or             */
/*  operating system.                                                        */
/*                                                                           */
/*****************************************************************************/
#ifndef OHA_CFG_H                        /* ----- reinclude-protection ----- */
#define OHA_CFG_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
/*  20.07.04    VE    initial version.                                       */
/*****************************************************************************/

#if 0
#include "pcpnio_sys.h"

#if PCPNIO_PLATFORM != 0 /* not ifdef... */

void oha_pcpnio_startstop (int start);

/* LTRC-IF */

#define TRACE_SUBSYS_OHA_UPPER     oha
#define TRACE_SUBSYS_OHA_LOWER     oha
#define TRACE_SUBSYS_OHA_SYSTEM    oha
#define TRACE_SUBSYS_OHA_FUNCTION  oha
#define TRACE_SUBSYS_OHA_PROGRAM   oha

#define TRACE_SUBSYS_OHA_UPPER_IDX00     oha
#define TRACE_SUBSYS_OHA_LOWER_IDX00     oha
#define TRACE_SUBSYS_OHA_SYSTEM_IDX00    oha
#define TRACE_SUBSYS_OHA_FUNCTION_IDX00  oha
#define TRACE_SUBSYS_OHA_PROGRAM_IDX00   oha

#define oha_log_get_fct            "oha" /* see LTRC_TRACE_01 */

extern int oha_log_level;          /* see LTRC_LEVEL_TYPE */

#else
#error "no platform"
#endif
#endif

/*===========================================================================*/
/*                              defines                                      */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/*             Components EDD, DCP, NARE, LLDP, MRP and SOCK                 */
/*                        OHA_CFG_NO_NARE, OHA_CFG_NO_MRP, OHA_CFG_NO_SOCK   */
/*---------------------------------------------------------------------------*/

/* Component NARE not used (no NARE-channel is openend): the request         */
/* OHA_OPC_RESOLVE_ADDRESS is therfore not supported !                       */
/*** #define OHA_CFG_NO_NARE ***/

/* Component MRP not used (no MRP-channel is openend): the request           */
/* OHA_OPC_CONTROL_MRP_OID is therfore not supported !                       */
#if ( LSAS_CFG_USE_MRP == 0 )
#ifndef OHA_CFG_NO_MRP
    #define OHA_CFG_NO_MRP
#endif
#endif

/* OHA_SOCK_SNMP_OFF: No AutoActivate/AutoDeactivate of the OHA subagent in SOCK.*/
/* OHA activates its agent in SOCK by OhaSystemOpenChannel and deactivates   */
/* his agent by OhaSystemCloseChannel "automatically". There is no external  */
/* (user-) activation in SOCK necessary, neither an activation of the        */
/* internal agent nor an activation of the (external) OHA agent!             */
/* LSA_SOCK_P04.01.00.00_00.01.04.01 can do AutoActivation.                  */

/* The MRP-MIB (IEC-62439-2-MIB) is not supported.                           */
#if ( LSAS_CFG_USE_MRP == 0 )
    #define OHA_CFG_NO_MRP_MIB    /* removed to IM_ENABLE_V2_3_FEATURES        */
#endif
/*---------------------------------------------------------------------------*/
/*                         OHA_CFG_NO_REMA                                   */
/*---------------------------------------------------------------------------*/
/* The PDEV parameterization must also be stored in "remote" parameterization*/
/* and it should be possible to load it during the next restart.             */
/* For this purpose, the Remanence Manager (REMA) is required.               */
/* The use of REMA in OHA can be turned off.                                 */
/*** #define OHA_CFG_NO_REMA  ***/

/*---------------------------------------------------------------------------*/
/* Maximum number of user-channels (max. 128, because of handle-management)  */
/*---------------------------------------------------------------------------*/

#define OHA_CFG_MAX_CHANNELS    (3/*ernst*/ + 2/*cm-pd*/ +1)   /* max number of user-channels */

/*---------------------------------------------------------------------------*/
/* Maximum number of NIC's (max. 32, because of handle-management)           */
/*---------------------------------------------------------------------------*/

#define OHA_CFG_MAX_NICS         1            /* max. number of Nic's        */

/*---------------------------------------------------------------------------*/
/* Maximum number of Ports of a Channel                                      */
/* must be >= MaxPortCnt                                                     */
/*---------------------------------------------------------------------------*/

#define OHA_CFG_MAX_PORTS        LSAS_CFG_PORT_CNT            /* max Ports   */

/*---------------------------------------------------------------------------*/
/* Additional hello delay by LinkUp in 10ms (20ms default)                   */
/*---------------------------------------------------------------------------*/

/* OHA does a HelloDelay(LinkUp) := "AddDelay" + HelloDelay from the record */

#define OHA_ADDITIONAL_HELLO_DELAY_BY_LINK_UP   2

/*===========================================================================*/
/*                              basic attributes                             */
/*===========================================================================*/

/*===========================================================================*/
/*                         code- and data-attributes                         */
/*===========================================================================*/


/*---------------------------------------------------------------------------*/
/* Definition of RQB-HEADER:                                                 */
/*                                                                           */
/* The order of the parameters can be changed!                               */
/* The length of the RQB-HEADER must be 2/4-byte-aligned according to the    */
/* system!                                                                   */
/* If necessary, dummy-bytes must be added!                                  */
/* If necessary during system integration, additional parameters can be added*/
/*                                                                           */
/* For example:                                                              */
/*   LSA_COMP_ID_TYPE                comp_id; // for lsa-component-id        */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#define OHA_RQB_HEADER      LSA_RQB_HEADER(OHA_UPPER_RQB_PTR_TYPE)


/*---------------------------------------------------------------------------*/
/* Definition of RQB-TRAILER                                                 */
/*                                                                           */
/* If necessary during system integration, additional parameters can be      */
/* added!                                                                    */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#define OHA_RQB_TRAILER     LSA_RQB_TRAILER
    /* nothing by default */


/*---------------------------------------------------------------------------*/
/* set and get parameter of the rqb-header                                   */
/*---------------------------------------------------------------------------*/

#define OHA_RQB_SET_NEXT_RQB_PTR      LSA_RQB_SET_NEXT_RQB_PTR
#define OHA_RQB_GET_NEXT_RQB_PTR      LSA_RQB_GET_NEXT_RQB_PTR
#define OHA_RQB_SET_PREV_RQB_PTR      LSA_RQB_SET_PREV_RQB_PTR
#define OHA_RQB_GET_PREV_RQB_PTR      LSA_RQB_GET_PREV_RQB_PTR
#define OHA_RQB_SET_OPCODE            LSA_RQB_SET_OPCODE
#define OHA_RQB_GET_OPCODE            LSA_RQB_GET_OPCODE
#define OHA_RQB_SET_HANDLE            LSA_RQB_SET_HANDLE
#define OHA_RQB_GET_HANDLE            LSA_RQB_GET_HANDLE
#define OHA_RQB_SET_USERID_UVAR32     LSA_RQB_SET_USER_ID_UVAR32
#define OHA_RQB_GET_USERID_UVAR32     LSA_RQB_GET_USER_ID_UVAR32
#define OHA_RQB_SET_USERID_UVAR16_1   LSA_RQB_SET_USER_ID_UVAR16_ARRAY_LOW
#define OHA_RQB_GET_USERID_UVAR16_1   LSA_RQB_GET_USER_ID_UVAR16_ARRAY_LOW
#define OHA_RQB_SET_USERID_UVAR16_2   LSA_RQB_SET_USER_ID_UVAR16_ARRAY_HIGH
#define OHA_RQB_GET_USERID_UVAR16_2   LSA_RQB_GET_USER_ID_UVAR16_ARRAY_HIGH
#define OHA_RQB_SET_RESPONSE          LSA_RQB_SET_RESPONSE
#define OHA_RQB_GET_RESPONSE          LSA_RQB_GET_RESPONSE
#define OHA_RQB_SET_USER_ID_PTR       LSA_RQB_SET_USER_ID_PTR
#define OHA_RQB_GET_USER_ID_PTR       LSA_RQB_GET_USER_ID_PTR

#define OHA_RQB_SET_PRIORITY(rb_, val_)     {(rb_)->_priority = (val_);}
#define OHA_RQB_GET_PRIORITY(_pRQB)         ((rb_)->_priority)


/*===========================================================================*/
/*             Acess to error_code[12] of LSA_RQB_LOCAL_ERROR_TYPE           */
/*===========================================================================*/

#define OHA_ERROR_SET_LL_OPCODE(_pRQB, _Value) {  \
OHA_LOCAL_ERROR_CODE_PTR_TYPE pErrorCode = (OHA_LOCAL_ERROR_CODE_PTR_TYPE)&(_pRQB)->err.error_code[0]; \
pErrorCode->opcode   = (_Value);                  \
}

#define OHA_ERROR_GET_LL_OPCODE(_pRQB)            \
((OHA_LOCAL_ERROR_CODE_PTR_TYPE)&(_pRQB)->err.error_code[0])->opcode    /* must be 32-Bit aligned!!!*/

#define OHA_ERROR_SET_LL_RESPONSE(_pRQB, _Value){ \
OHA_LOCAL_ERROR_CODE_PTR_TYPE pErrorCode = (OHA_LOCAL_ERROR_CODE_PTR_TYPE)&(_pRQB)->err.error_code[0]; \
pErrorCode->response = (_Value);                  \
}

#define OHA_ERROR_GET_LL_RESPONSE(_pRQB)          \
((OHA_LOCAL_ERROR_CODE_PTR_TYPE)&(_pRQB)->err.error_code[0])->response  /* must be 32-Bit aligned!!!*/


/*===========================================================================*/
/* LOWER LAYER RQB MACROS to EDD                                             */
/*===========================================================================*/

#define OHA_EDD_RQB_SET_NEXT_RQB_PTR(rb_, val_)   LSA_RQB_SET_NEXT_RQB_PTR(rb_, val_)
#define OHA_EDD_RQB_SET_PREV_RQB_PTR(rb_, val_)   LSA_RQB_SET_PREV_RQB_PTR(rb_, val_)
#define OHA_EDD_RQB_SET_OPCODE(rb_, val_)         LSA_RQB_SET_OPCODE(rb_, val_)
#define OHA_EDD_RQB_SET_SERVICE(rb_, val_)        EDD_RQB_SET_SERVICE(rb_, val_)
#define OHA_EDD_RQB_SET_HANDLE(rb_, val_)         LSA_RQB_SET_HANDLE(rb_, val_)
#define OHA_EDD_RQB_SET_USERID_UVAR16(rb_, val_)  LSA_RQB_SET_USER_ID_UVAR16(rb_, val_)
#define OHA_EDD_RQB_SET_USERID_UVAR32(rb_, val_)  LSA_RQB_SET_USER_ID_UVAR32(rb_, val_)
#define OHA_EDD_RQB_SET_RESPONSE(rb_, val_)       LSA_RQB_SET_REPONSE(rb_, val_)

#define OHA_EDD_RQB_GET_NEXT_RQB_PTR(rb_)         LSA_RQB_GET_NEXT_RQB_PTR(rb_)
#define OHA_EDD_RQB_GET_PREV_RQB_PTR(rb_)         LSA_RQB_GET_PREV_RQB_PTR(rb_)
#define OHA_EDD_RQB_GET_OPCODE(rb_)               LSA_RQB_GET_OPCODE(rb_)
#define OHA_EDD_RQB_GET_SERVICE(rb_)              EDD_RQB_GET_SERVICE(rb_)
#define OHA_EDD_RQB_GET_HANDLE(rb_)               LSA_RQB_GET_HANDLE(rb_)
#define OHA_EDD_RQB_GET_USERID_UVAR16(rb_)        LSA_RQB_GET_USER_ID_UVAR16(rb_)
#define OHA_EDD_RQB_GET_USERID_UVAR32(rb_)        LSA_RQB_GET_USER_ID_UVAR32(rb_)
#define OHA_EDD_RQB_GET_RESPONSE(rb_)             LSA_RQB_GET_RESPONSE(rb_)

/*===========================================================================*/
/* LOWER LAYER RQB MACROS to DCP                                             */
/*===========================================================================*/

#define OHA_DCP_RQB_SET_NEXT_RQB_PTR(rb_, val_)   LSA_RQB_SET_NEXT_RQB_PTR(rb_, val_)
#define OHA_DCP_RQB_SET_PREV_RQB_PTR(rb_, val_)   LSA_RQB_SET_PREV_RQB_PTR(rb_, val_)
#define OHA_DCP_RQB_SET_OPCODE(rb_, val_)         LSA_RQB_SET_OPCODE(rb_, val_)
#define OHA_DCP_RQB_SET_HANDLE(rb_, val_)         LSA_RQB_SET_HANDLE(rb_, val_)
#define OHA_DCP_RQB_SET_USERID_UVAR16(rb_, val_)  LSA_RQB_SET_USER_ID_UVAR16(rb_, val_)
#define OHA_DCP_RQB_SET_USERID_UVAR32(rb_, val_)  LSA_RQB_SET_USER_ID_UVAR32(rb_, val_)
#define OHA_DCP_RQB_SET_RESPONSE(rb_, val_)       LSA_RQB_SET_REPONSE(rb_, val_)

#define OHA_DCP_RQB_GET_NEXT_RQB_PTR(rb_)         LSA_RQB_GET_NEXT_RQB_PTR(rb_)
#define OHA_DCP_RQB_GET_PREV_RQB_PTR(rb_)         LSA_RQB_GET_PREV_RQB_PTR(rb_)
#define OHA_DCP_RQB_GET_OPCODE(rb_)               LSA_RQB_GET_OPCODE(rb_)
#define OHA_DCP_RQB_GET_HANDLE(rb_)               LSA_RQB_GET_HANDLE(rb_)
#define OHA_DCP_RQB_GET_USERID_UVAR16(rb_)        LSA_RQB_GET_USER_ID_UVAR16(rb_)
#define OHA_DCP_RQB_GET_USERID_UVAR32(rb_)        LSA_RQB_GET_USER_ID_UVAR32(rb_)
#define OHA_DCP_RQB_GET_RESPONSE(rb_)             LSA_RQB_GET_RESPONSE(rb_)

/*===========================================================================*/
/* LOWER LAYER RQB MACROS to LLDP                                            */
/*===========================================================================*/

#define OHA_LLDP_RQB_SET_NEXT_RQB_PTR(rb_, val_)   LSA_RQB_SET_NEXT_RQB_PTR(rb_, val_)
#define OHA_LLDP_RQB_SET_PREV_RQB_PTR(rb_, val_)   LSA_RQB_SET_PREV_RQB_PTR(rb_, val_)
#define OHA_LLDP_RQB_SET_OPCODE(rb_, val_)         LSA_RQB_SET_OPCODE(rb_, val_)
#define OHA_LLDP_RQB_SET_HANDLE(rb_, val_)         LSA_RQB_SET_HANDLE(rb_, val_)
#define OHA_LLDP_RQB_SET_USERID_UVAR16(rb_, val_)  LSA_RQB_SET_USER_ID_UVAR16(rb_, val_)
#define OHA_LLDP_RQB_SET_USERID_UVAR32(rb_, val_)  LSA_RQB_SET_USER_ID_UVAR32(rb_, val_)
#define OHA_LLDP_RQB_SET_RESPONSE(rb_, val_)       LSA_RQB_SET_REPONSE(rb_, val_)

#define OHA_LLDP_RQB_GET_NEXT_RQB_PTR(rb_)         LSA_RQB_GET_NEXT_RQB_PTR(rb_)
#define OHA_LLDP_RQB_GET_PREV_RQB_PTR(rb_)         LSA_RQB_GET_PREV_RQB_PTR(rb_)
#define OHA_LLDP_RQB_GET_OPCODE(rb_)               LSA_RQB_GET_OPCODE(rb_)
#define OHA_LLDP_RQB_GET_HANDLE(rb_)               LSA_RQB_GET_HANDLE(rb_)
#define OHA_LLDP_RQB_GET_USERID_UVAR16(rb_)        LSA_RQB_GET_USER_ID_UVAR16(rb_)
#define OHA_LLDP_RQB_GET_USERID_UVAR32(rb_)        LSA_RQB_GET_USER_ID_UVAR32(rb_)
#define OHA_LLDP_RQB_GET_RESPONSE(rb_)             LSA_RQB_GET_RESPONSE(rb_)

/*===========================================================================*/
/* LOWER LAYER RQB MACROS to NARE                                            */
/*===========================================================================*/

#define OHA_NARE_RQB_SET_NEXT_RQB_PTR(rb_, val_)   LSA_RQB_SET_NEXT_RQB_PTR(rb_, val_)
#define OHA_NARE_RQB_SET_PREV_RQB_PTR(rb_, val_)   LSA_RQB_SET_PREV_RQB_PTR(rb_, val_)
#define OHA_NARE_RQB_SET_OPCODE(rb_, val_)         LSA_RQB_SET_OPCODE(rb_, val_)
#define OHA_NARE_RQB_SET_HANDLE(rb_, val_)         LSA_RQB_SET_HANDLE(rb_, val_)
#define OHA_NARE_RQB_SET_USERID_UVAR16(rb_, val_)  LSA_RQB_SET_USER_ID_UVAR16(rb_, val_)
#define OHA_NARE_RQB_SET_USERID_UVAR32(rb_, val_)  LSA_RQB_SET_USER_ID_UVAR32(rb_, val_)
#define OHA_NARE_RQB_SET_RESPONSE(rb_, val_)       LSA_RQB_SET_REPONSE(rb_, val_)

#define OHA_NARE_RQB_GET_NEXT_RQB_PTR(rb_)         LSA_RQB_GET_NEXT_RQB_PTR(rb_)
#define OHA_NARE_RQB_GET_PREV_RQB_PTR(rb_)         LSA_RQB_GET_PREV_RQB_PTR(rb_)
#define OHA_NARE_RQB_GET_OPCODE(rb_)               LSA_RQB_GET_OPCODE(rb_)
#define OHA_NARE_RQB_GET_HANDLE(rb_)               LSA_RQB_GET_HANDLE(rb_)
#define OHA_NARE_RQB_GET_USERID_UVAR16(rb_)        LSA_RQB_GET_USER_ID_UVAR16(rb_)
#define OHA_NARE_RQB_GET_USERID_UVAR32(rb_)        LSA_RQB_GET_USER_ID_UVAR32(rb_)
#define OHA_NARE_RQB_GET_RESPONSE(rb_)             LSA_RQB_GET_RESPONSE(rb_)
/*===========================================================================*/
/* LOWER LAYER RQB MACROS to MRP                                             */
/*===========================================================================*/

#define OHA_MRP_RQB_SET_NEXT_RQB_PTR(rb_, val_)   MRP_RQB_SET_NEXT_RQB_PTR(rb_, val_)
#define OHA_MRP_RQB_SET_PREV_RQB_PTR(rb_, val_)   MRP_RQB_SET_PREV_RQB_PTR(rb_, val_)
#define OHA_MRP_RQB_SET_OPCODE(rb_, val_)         MRP_RQB_SET_OPCODE(rb_, val_)
#define OHA_MRP_RQB_SET_HANDLE(rb_, val_)         MRP_RQB_SET_HANDLE(rb_, val_)
#define OHA_MRP_RQB_SET_USERID_UVAR16(rb_, val_)  MRP_RQB_SET_USER_ID_UVAR16(rb_, val_)
#define OHA_MRP_RQB_SET_RESPONSE(rb_, val_)       MRP_RQB_SET_RESPONSE(rb_, val_)

#define OHA_MRP_RQB_GET_NEXT_RQB_PTR(rb_)         MRP_RQB_GET_NEXT_RQB_PTR(rb_)
#define OHA_MRP_RQB_GET_PREV_RQB_PTR(rb_)         MRP_RQB_GET_PREV_RQB_PTR(rb_)
#define OHA_MRP_RQB_GET_OPCODE(rb_)               MRP_RQB_GET_OPCODE(rb_)
#define OHA_MRP_RQB_GET_HANDLE(rb_)               MRP_RQB_GET_HANDLE(rb_)
#define OHA_MRP_RQB_GET_USERID_UVAR16(rb_)        MRP_RQB_GET_USER_ID_UVAR16(rb_)
#define OHA_MRP_RQB_GET_RESPONSE(rb_)             MRP_RQB_GET_RESPONSE(rb_)

/*===========================================================================*/
/* LOWER LAYER RQB MACROS to SOCK                                            */
/*===========================================================================*/

#define OHA_SOCK_RQB_SET_NEXT_RQB_PTR(rb_, val_)   LSA_RQB_SET_NEXT_RQB_PTR(rb_, val_)
#define OHA_SOCK_RQB_SET_PREV_RQB_PTR(rb_, val_)   LSA_RQB_SET_PREV_RQB_PTR(rb_, val_)
#define OHA_SOCK_RQB_SET_OPCODE(rb_, val_)         LSA_RQB_SET_OPCODE(rb_, val_)
#define OHA_SOCK_RQB_SET_HANDLE(rb_, val_)         LSA_RQB_SET_HANDLE(rb_, val_)
#define OHA_SOCK_RQB_SET_USERID_UVAR16(rb_, val_)  LSA_RQB_SET_USER_ID_UVAR16(rb_, val_)
#define OHA_SOCK_RQB_SET_USERID_UVAR32(rb_, val_)  LSA_RQB_SET_USER_ID_UVAR32(rb_, val_)
#define OHA_SOCK_RQB_SET_RESPONSE(rb_, val_)       LSA_RQB_SET_REPONSE(rb_, val_)

#define OHA_SOCK_RQB_GET_NEXT_RQB_PTR(rb_)         LSA_RQB_GET_NEXT_RQB_PTR(rb_)
#define OHA_SOCK_RQB_GET_PREV_RQB_PTR(rb_)         LSA_RQB_GET_PREV_RQB_PTR(rb_)
#define OHA_SOCK_RQB_GET_OPCODE(rb_)               LSA_RQB_GET_OPCODE(rb_)
#define OHA_SOCK_RQB_GET_HANDLE(rb_)               LSA_RQB_GET_HANDLE(rb_)
#define OHA_SOCK_RQB_GET_USERID_UVAR16(rb_)        LSA_RQB_GET_USER_ID_UVAR16(rb_)
#define OHA_SOCK_RQB_GET_USERID_UVAR32(rb_)        LSA_RQB_GET_USER_ID_UVAR32(rb_)
#define OHA_SOCK_RQB_GET_RESPONSE(rb_)             LSA_RQB_GET_RESPONSE(rb_)


/*===========================================================================*/
/* LOWER LAYER RQB MACROS to SNMPX                                           */
/*===========================================================================*/

#define OHA_SNMPX_RQB_SET_NEXT_RQB_PTR(rb_, val_)  LSA_RQB_SET_NEXT_RQB_PTR(rb_, val_)
#define OHA_SNMPX_RQB_SET_PREV_RQB_PTR(rb_, val_)  LSA_RQB_SET_PREV_RQB_PTR(rb_, val_)
#define OHA_SNMPX_RQB_SET_OPCODE(rb_, val_)        LSA_RQB_SET_OPCODE(rb_, val_)
#define OHA_SNMPX_RQB_SET_HANDLE(rb_, val_)        LSA_RQB_SET_HANDLE(rb_, val_)
#define OHA_SNMPX_RQB_SET_USERID_UVAR32(rb_, val_) LSA_RQB_SET_USER_ID_UVAR32(rb_, val_)
#define OHA_SNMPX_RQB_SET_RESPONSE(rb_, val_)      LSA_RQB_SET_REPONSE(rb_, val_)

#define OHA_SNMPX_RQB_GET_NEXT_RQB_PTR(rb_)        LSA_RQB_GET_NEXT_RQB_PTR(rb_)
#define OHA_SNMPX_RQB_GET_PREV_RQB_PTR(rb_)        LSA_RQB_GET_PREV_RQB_PTR(rb_)
#define OHA_SNMPX_RQB_GET_OPCODE(rb_)              LSA_RQB_GET_OPCODE(rb_)
#define OHA_SNMPX_RQB_GET_HANDLE(rb_)              LSA_RQB_GET_HANDLE(rb_)
#define OHA_SNMPX_RQB_GET_USERID_UVAR32(rb_)       LSA_RQB_GET_USER_ID_UVAR32(rb_)
#define OHA_SNMPX_RQB_GET_RESPONSE(rb_)            LSA_RQB_GET_RESPONSE(rb_)



/*===========================================================================*/
/* BTRACE - Support                                                          */
/*===========================================================================*/

#ifndef OHA_FILE_SYSTEM_EXTENSION
#define OHA_FILE_SYSTEM_EXTENSION(module_id_)
#endif

/*===========================================================================*/
/* LTRC - Support (LSA-Trace)                                                */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* define the Trace mode for OHA                                             */
/* 0: no traces or external traces (default)                                 */
/* 1: enable Traces and use LTRC (LSA-Trace module) see OHA_ltrc.h           */
/*---------------------------------------------------------------------------*/

#define OHA_CFG_TRACE_MODE   1

/*=============================================================================
 * If the LSA component LTRC isn't used for trace in the target system, then
 * the OHA trace macros can be defined here. On default they are empty.
 *
 * See also files oha_trc.h and lsa_cfg.h/txt.
 *
 *===========================================================================*/

/*===========================================================================*/
/* Debugging (for developer use)                                             */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* If a a memory-free call error should result in an fatal-error call        */
/* with OHA_FATAL_ERR_MEM_FREE define OHA_CFG_FREE_ERROR_IS_FATAL. If not    */
/* memory-free errors will be ignored.                                       */
/*---------------------------------------------------------------------------*/

#define OHA_CFG_FREE_ERROR_IS_FATAL

/*---------------------------------------------------------------------------*/
/* define to enable spezial additonal assertion-checks (e.g. NULL-Ptr)       */
/* This assertion will cause an fatal-error.                                 */
/*---------------------------------------------------------------------------*/

#define OHA_CFG_DEBUG_ASSERT    0

/*----------------------------------------------------------------------------*/
/*  Assert                                                                    */
/*                                                                            */
/*  ==> MUST BE EMPTY in Release-Version!                                     */
/*                                                                            */
/*  check if condition holds or call fatal error otherwise (programming error)*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

#if OHA_CFG_DEBUG_ASSERT

    #define OHA_ASSERT(Cond_)                                 \
    {                                                         \
        if( ! (Cond_) )                                       \
        {                                                     \
            OHA_FatalError( OHA_FATAL_ERR_INCONSISTENZ,       \
                            OHA_MODULE_ID,                    \
                            __LINE__);                        \
        }                                                     \
    }
    #define OHA_ASSERT_NULL_PTR(pPtr)                         \
    {                                                         \
        if( LSA_HOST_PTR_ARE_EQUAL(pPtr, LSA_NULL) )          \
        {                                                     \
            OHA_FatalError( OHA_FATAL_ERR_NULL_PTR,           \
                            OHA_MODULE_ID,                    \
                            __LINE__);                        \
        }                                                     \
    }

    #define OHA_ASSERT_FALSE(Cond_)                           \
    {                                                         \
        if( ! (Cond_) )                                       \
        {                                                     \
            OHA_FatalError( OHA_FATAL_ERR_INCONSISTENZ,       \
                            OHA_MODULE_ID,                    \
                            __LINE__);                        \
        }                                                     \
    }
#else
    #define OHA_ASSERT(Condition)  LSA_UNUSED_ARG(Condition)
    #define OHA_ASSERT_NULL_PTR(pPtr) LSA_UNUSED_ARG(pPtr)
    #define OHA_ASSERT_FALSE(Condition) LSA_UNUSED_ARG(Condition)
#endif

/*---------------------------------------------------------------------------*/

/*===========================================================================*/
/*                          macros/function                                  */
/*===========================================================================*/

#define OHA_FATAL_ERROR(_length, _error_ptr)                                \
{                                                                           \
    LSAS_FATAL_ERROR( _error_ptr);                 \
}                                                                           \

/*****************************************************************************/
/*  end of file OHA_CFG.H                                                    */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of OHA_CFG_H */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
