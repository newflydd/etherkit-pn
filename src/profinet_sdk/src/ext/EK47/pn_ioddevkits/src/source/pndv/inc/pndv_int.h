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
/*  F i l e               &F: pndv_int.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  internal arrangements                                                    */
/*                                                                           */
/*****************************************************************************/



/*****************************************************************************/
/* contents:

    - internal state machine defines
    - internal error sub-module defines
    - comparing operations
    - dummy operations

*/
/*****************************************************************************/
/* reinclude protection */


#ifndef PNDV_INT_H
#define PNDV_INT_H

#include "pndv_list.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/**********                                                         **********/
/**********                   FEATURES                              **********/
/**********                                                         **********/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

//#define PNDV_DPM_SIM_USED_TERM
//#define PNDV_SIMU_ZYKL_OUTP


/*****************************************************************************/
/* internal state machine defines */

typedef enum PNDV_SM_STATE_E
{
TRC_SUBLABEL_LIST_OPEN("TRC_PNDV_SM_STATE")
    PNDV_SM_CLOSED                                          = 1, TRC_EXPLAIN("STATE_CLOSED")                    //!< pndv initialized but not opend yet
    PNDV_SM_OPEN                                            = 3, TRC_EXPLAIN("STATE_OPEN")                      //!< pndv has been opend, ready for start
    PNDV_SM_W_OPEN_CHANNELS_DONE                            = 4, TRC_EXPLAIN("STATE_W_OPEN_CHANNELS_DONE")      //!< start was initiated, start opening channels to cm and sending resources

    PNDV_SM_W_CREATE_PD_SERVER_CNF                          = 8, TRC_EXPLAIN("STATE_W_CREATE_PD_SERVER_CNF")    //!< wait for physical device server to be created
    PNDV_SM_W_CREAT_SERVER_CNF                              = 10, TRC_EXPLAIN("STATE_W_CREAT_SERVER_CNF")       //!< wait for server to be created
    PNDV_SM_W_ADD_DEVICE_CNF                                = 12, TRC_EXPLAIN("STATE_W_ADD_DEVICE_CNF")         //!< wait for device instance to be created
    PNDV_SM_W_ADD_IM_SUBMODULES_DONE                        = 16, TRC_EXPLAIN("STATE_W_ADD_IM_SUBMODULES_DONE") //!< device is ready and waiting for the first modules to be added (pdev subs required now)
    PNDV_SM_PRM_PDEV                                        = 17, TRC_EXPLAIN("STATE_PRM_PDEV")                 //!< adding first subs is done, parameters where send to pdev

    PNDV_SM_DEVICE_ACTIVE                                   = 20, TRC_EXPLAIN("STATE_DEVICE_ACTIVE")            //!< paramters for pdev are done, device has been set to active state, startup done

    PNDV_SM_W_REMOVE_DEVICE_CNF                             = 81, TRC_EXPLAIN("STATE_W_REMOVE_DEVICE_CNF")      //!< pndv is going to be stopped, waiting for removing the device instance
    PNDV_SM_W_DELETE_SERVER_CNF                             = 82, TRC_EXPLAIN("STATE_W_DELETE_SERVER_CNF")      //!< waiting for deleting the server
    PNDV_SM_W_DELETE_PD_SERVER_CNF                          = 83, TRC_EXPLAIN("STATE_W_DELETE_PD_SERVER_CNF")   //!< waiting for deleting the physical device server
    PNDV_SM_W_CLOSE_CHANNELS_DONE                           = 84, TRC_EXPLAIN("STATE_W_CLOSE_CHANNELS_DONE")    //!< waiting for channels to be closed
    PNDV_SM_STOP_W_CM_CNF                                   = 85, TRC_EXPLAIN("STATE_STOP_W_CM_CNF")            //!< pndv is going to be stopped while startup is underway, wait for the last request to be finished
    PNDV_SM_W_CLOSE_ACP_CHANNEL_CNF                         = 86, TRC_EXPLAIN("CLOSE_ACP_CHANNEL_CNF")
    PNDV_SM_W_CLOSE_PD_SV_CHANNEL_CNF                       = 87, TRC_EXPLAIN("CLOSE_PD_SV_CHANNEL_CNF")

TRC_SUBLABEL_LIST_CLOSE("TRC_PNDV_SM_STATE")
    PNDV_SM_NOT_USED                                        = 0xFF

}   PNDV_SM_STATE_E_T;


typedef enum PNDV_SM_EVENT_E
{
TRC_SUBLABEL_LIST_OPEN("TRC_PNDV_SM_EVENT")
    PNDV_SM_EVENT_IDLE                                      = 1, TRC_EXPLAIN("EVENT_IDLE")
    PNDV_SM_EVENT_OPEN                                      = 2, TRC_EXPLAIN("EVENT_OPEN")
    PNDV_SM_EVENT_START                                     = 3, TRC_EXPLAIN("EVENT_START")
    PNDV_SM_EVENT_CHANNELS_OPEN_DONE                        = 4, TRC_EXPLAIN("EVENT_CHANNELS_OPEN_DONE")
    PNDV_SM_EVENT_CREATE_PD_SERVER_DONE                     = 5, TRC_EXPLAIN("EVENT_CREATE_PD_SERVER_DONE")
    PNDV_SM_EVENT_CREATE_CM_SERVER_DONE                     = 6, TRC_EXPLAIN("EVENT_CREATE_CM_SERVER_DONE")
    PNDV_SM_EVENT_DEVICE_ADD_DONE                           = 7, TRC_EXPLAIN("EVENT_DEVICE_ADD_DONE")
    PNDV_SM_EVENT_DEVICE_CONTROL_DONE                       = 8, TRC_EXPLAIN("EVENT_DEVICE_CONTROL_DONE")
    PNDV_SM_EVENT_DEVICE_REMOVE_DONE                        = 9, TRC_EXPLAIN("EVENT_DEVICE_REMOVE_DONE")
    PNDV_SM_EVENT_CM_SV_DELETE_DONE                         = 10, TRC_EXPLAIN("EVENT_CM_SV_DELETE_DONE")
    PNDV_SM_EVENT_ADD_IM_SUBMODULES_DONE                    = 11, TRC_EXPLAIN("EVENT_ADD_IM_SUBMODULES_DONE")
    PNDV_SM_EVENT_PRM_PDEV_DONE                             = 12, TRC_EXPLAIN("EVENT_PRM_PDEV_DONE")

    PNDV_SM_EVENT_DELETE_PD_SERVER_DONE                     = 13, TRC_EXPLAIN("EVENT_DELETE_PD_SERVER_DONE")
    PNDV_SM_EVENT_STOP                                      = 14, TRC_EXPLAIN("EVENT_STOP")
    PNDV_SM_EVENT_STOP_DONE                                 = 15, TRC_EXPLAIN("EVENT_STOP_DONE")
    PNDV_SM_EVENT_ACP_CLOSE_CHANNEL_DONE                    = 16, TRC_EXPLAIN("EVENT_ACP_CLOSE_CHANNEL_DONE")
    PNDV_SM_EVENT_PD_SV_CLOSE_CHANNEL_DONE                  = 17, TRC_EXPLAIN("EVENT_PD_SV_CLOSE_CHANNEL_DONE")
    PNDV_SM_EVENT_EDD_CLRPC_CLOSE_CHANNEL_DONE              = 18, TRC_EXPLAIN("EVENT_EDD_CLRPC_CLOSE_CHANNEL_DONE")

    PNDV_SM_EVENT_ERROR                                     = 99, TRC_EXPLAIN("EVENT_ERROR")
    TRC_SUBLABEL_LIST_CLOSE("TRC_PNDV_SM_EVENT")
    PNDV_SM_EVENT_NOT_USED                                  = 255
} PNDV_SM_EVENT_E_T;


typedef enum PNDV_AR_SM_STATE_E
{
TRC_SUBLABEL_LIST_OPEN("TRC_PNDV_AR_SM_STATE")
    PNDV_AR_SM_OFFLINE                                  = 10, TRC_EXPLAIN("OFFLINE")/**< ar is not established*/
    PNDV_AR_SM_SESSION_START                            = 11, TRC_EXPLAIN("STATE_SESSION_START")
    PNDV_AR_SM_W_CONNECT_IND_PERI_DONE                  = 12, TRC_EXPLAIN("STATE_W_CONNECT_IND_PERI_DONE")/**< connect_ind received ar is prepared for further services, connect_ind was send to peri*/
    PNDV_AR_SM_W_OWNERSHIP_IND                          = 13, TRC_EXPLAIN("STATE_W_OWNERSHIP_IND")/**< connect_rsp was send to cm, ar is waiting f�r ownership ind*/
    PNDV_AR_SM_W_OWNERSHIP_IND_DONE                     = 14, TRC_EXPLAIN("STATE_W_OWNERSHIP_IND_DONE")/**< ownership_ind was received from cm, list of submodules was send to peri*/
    PNDV_AR_SM_W_PRM_END_IND                            = 15, TRC_EXPLAIN("STATE_W_PRM_END_IND")/**< list of submodules of owenership ind was processed by peri, ownership_rsp was send to cm, waiting for end of parameter process*/
    PNDV_AR_SM_W_PRM_END_IND_DONE                       = 16, TRC_EXPLAIN("STATE_W_PRM_END_IND_DONE")/**< prm_end ind was received from cm and send to peri, waiting for peri response*/

    /**! RTC3*/
    PNDV_AR_SM_W_RIR_IND                                = 22, TRC_EXPLAIN("STATE_W_RIR_IND")/**< appl_ready.cnf was received from cm, wait for rir ind (receive in red)*/
    PNDV_AR_SM_W_INPUT_UPDATE_RTC3                      = 23, TRC_EXPLAIN("STATE_W_INPUT_UPDATE_RTC3")/**< rir_ind received from cm, ready for input update was signaled to peri, rir_rsp was send*/
    PNDV_AR_SM_W_RIR_ACK_CNF                            = 24, TRC_EXPLAIN("STATE_W_RIR_ACK_CNF")/**< inputs updated, appl in sync (if needed), rir_ack was send to cm*/
    PNDV_AR_SM_W_IN_DATA_IND_RTC3                       = 25, TRC_EXPLAIN("STATE_W_IN_DATA_IND_RTC3")/**< rir_ack.cnf received from cm, now wait for in_data.ind*/
    PNDV_AR_SM_W_IN_DATA_IND_DONE_RTC3                  = 26, TRC_EXPLAIN("STATE_W_IN_DATA_IND_DONE_RTC3")/**<*/
    PNDV_AR_SM_IN_DATA_RTC3                             = 27, TRC_EXPLAIN("STATE_IN_DATA_RTC3")/**< in_data ind received from cm, ar is fully established now*/

    /**! RTC1/2*/
    PNDV_AR_SM_W_INPUT_UPDATE                           = 30, TRC_EXPLAIN("STATE_W_INPUT_UPDATE")/**< prm_end rsp was received from peri, ar is non RTClass3, ready for input update was signaled to peri*/
    PNDV_AR_SM_W_APP_READY_RSP                          = 31, TRC_EXPLAIN("STATE_W_APP_READY_RSP")/**< inputs updated, appl_ready.req was send to cm*/
    PNDV_AR_SM_W_IN_DATA_IND                            = 32, TRC_EXPLAIN("STATE_W_IN_DATA_IND")/**< appl_ready.cnf was received from cm,*/
    PNDV_AR_SM_W_IN_DATA_IND_DONE                       = 33, TRC_EXPLAIN("STATE_W_IN_DATA_IND_DONE")/**<*/
    PNDV_AR_SM_IN_DATA                                  = 34, TRC_EXPLAIN("STATE_IN_DATA")/**< ar_in_data ind received from cm, ar is fully established now*/

    /**! AR-Abort/Offline*/
    PNDV_AR_SM_DISCONNECT_IND_W_DONE                    = 35, TRC_EXPLAIN("STATE_DISCONNECT_IND_W_DONE")/**< OFFLINE.ind was received while a peri service was active, wait for finishing this service*/
    PNDV_AR_SM_W_DISCONNECT_IND_DONE                    = 36, TRC_EXPLAIN("STATE_W_DISCONNECT_IND_DONE")/**< process disconnect, cleanup ar informations and interface*/
    PNDV_AR_SM_W_DISCONNECT_IND_DONE__CONTEXT_CONNECT   = 37, TRC_EXPLAIN("STATE_W_DISCONNECT_IND_DONE__CONTEXT_CONNECT")/**< internal disconnect after rejected Connect.ind*/
TRC_SUBLABEL_LIST_CLOSE("TRC_PNDV_AR_SM_STATE")
    PNDV_AR_SM_NOT_USED                                 = 255
}   PNDV_AR_SM_STATE_T;

#ifdef PNDV_TRACE_MSG
#define PNDV_TRACE_MSG_AR_STATE_STR(_p_deb_str, _deb_code)    \
{                                                            \
    char* _pstr1;                                            \
    switch (_deb_code)                                         \
    {                                                       \
        case PNDV_AR_SM_OFFLINE                                 : _pstr1 = "OFFLINE";                     break; \
        case PNDV_AR_SM_SESSION_START                           : _pstr1 = "SESSION_START";             break; \
        case PNDV_AR_SM_W_CONNECT_IND_PERI_DONE                 : _pstr1 = "W_CONNECT_IND_PERI_DONE";     break; \
        case PNDV_AR_SM_W_OWNERSHIP_IND                         : _pstr1 = "W_OWNERSHIP_IND";             break; \
        case PNDV_AR_SM_W_OWNERSHIP_IND_DONE                    : _pstr1 = "W_OWNERSHIP_IND_DONE";         break; \
        case PNDV_AR_SM_W_PRM_END_IND                           : _pstr1 = "W_PRM_END_IND";             break; \
        case PNDV_AR_SM_W_PRM_END_IND_DONE                      : _pstr1 = "W_PRM_END_IND_DONE";         break; \
        case PNDV_AR_SM_W_RIR_IND                               : _pstr1 = "W_RIR_IND";                 break; \
        case PNDV_AR_SM_W_INPUT_UPDATE_RTC3                     : _pstr1 = "W_INPUT_UPDATE_RTC3";         break; \
        case PNDV_AR_SM_W_RIR_ACK_CNF                           : _pstr1 = "W_RIR_ACK_CNF";             break; \
        case PNDV_AR_SM_W_IN_DATA_IND_RTC3                      : _pstr1 = "W_IN_DATA_IND_RTC3";         break; \
        case PNDV_AR_SM_W_IN_DATA_IND_DONE_RTC3                 : _pstr1 = "W_IN_DATA_IND_DONE_RTC3";     break; \
        case PNDV_AR_SM_IN_DATA_RTC3                            : _pstr1 = "IN_DATA_RTC3";                 break; \
        case PNDV_AR_SM_W_INPUT_UPDATE                          : _pstr1 = "W_INPUT_UPDATE";             break; \
        case PNDV_AR_SM_W_APP_READY_RSP                         : _pstr1 = "W_APP_READY_RSP";             break; \
        case PNDV_AR_SM_W_IN_DATA_IND                           : _pstr1 = "W_IN_DATA_IND";             break; \
        case PNDV_AR_SM_W_IN_DATA_IND_DONE                      : _pstr1 = "W_IN_DATA_IND_DONE";         break; \
        case PNDV_AR_SM_IN_DATA                                 : _pstr1 = "IN_DATA";                     break; \
        case PNDV_AR_SM_DISCONNECT_IND_W_DONE                   : _pstr1 = "DISCONNECT_IND_W_DONE";     break; \
        case PNDV_AR_SM_W_DISCONNECT_IND_DONE                   : _pstr1 = "W_DISCONNECT_IND_DONE";     break; \
        case PNDV_AR_SM_NOT_USED                                : _pstr1 = "NOT USED";                     break; \
        default                                                 : _pstr1 = "UNDEFINED";                 break; \
    }                        \
    *_p_deb_str = _pstr1;     \
}
#endif /* PNDV_TRACE_MSG */

typedef enum PNDV_AR_SM_EVENT_E
{
    TRC_SUBLABEL_LIST_OPEN("TRC_PNDV_AR_SM_EVENT")
    PNDV_AR_SM_EVENT_CONNECT_IND                                = 1,  TRC_EXPLAIN("EVENT_CONNECT_IND")
    PNDV_AR_SM_EVENT_CONNECT_IND_OK_DONE                        = 2,  TRC_EXPLAIN("EVENT_CONNECT_IND_OK_DONE")
    PNDV_AR_SM_EVENT_CONNECT_IND_ERROR_DONE                     = 3,  TRC_EXPLAIN("EVENT_CONNECT_IND_ERROR_DONE")
    PNDV_AR_SM_EVENT_CONNECT_IND_PERI_OK_DONE                   = 4,  TRC_EXPLAIN("EVENT_CONNECT_IND_PERI_OK_DONE")
    PNDV_AR_SM_EVENT_CONNECT_IND_PERI_ERROR_DONE                = 5,  TRC_EXPLAIN("EVENT_CONNECT_IND_PERI_ERROR_DONE")
    PNDV_AR_SM_EVENT_CONNECT_IND_PERI_RESOURCE_ERROR_DONE       = 6,  TRC_EXPLAIN("EVENT_CONNECT_IND_PERI_RESOURCE_ERROR_DONE")
    PNDV_AR_SM_EVENT_OWNERSHIP_IND                              = 7,  TRC_EXPLAIN("EVENT_OWNERSHIP_IND")
    PNDV_AR_SM_EVENT_OWNERSHIP_IND_DONE                         = 8,  TRC_EXPLAIN("EVENT_OWNERSHIP_IND_DONE")
    PNDV_AR_SM_EVENT_PRM_END_IND                                = 9,  TRC_EXPLAIN("EVENT_PRM_END_IND")
    PNDV_AR_SM_EVENT_PRM_END_IND_DONE                           = 10,  TRC_EXPLAIN("EVENT_PRM_END_IND_DONE")
    PNDV_AR_SM_EVENT_FIRST_INPUT_UPDATE_DONE                    = 11, TRC_EXPLAIN("EVENT_FIRST_INPUT_UPDATE_DONE")
    PNDV_AR_SM_EVENT_APP_READY_RSP                              = 12, TRC_EXPLAIN("EVENT_APP_READY_RSP")
    PNDV_AR_SM_EVENT_AR_IN_DATA                                 = 13, TRC_EXPLAIN("EVENT_AR_IN_DATA")
    PNDV_AR_SM_EVENT_AR_IN_DATA_DONE                            = 14, TRC_EXPLAIN("EVENT_AR_IN_DATA_DONE")
    PNDV_AR_SM_EVENT_RIR_IND                                    = 15, TRC_EXPLAIN("EVENT_RIR_IND")
    PNDV_AR_SM_EVENT_RIR_ACK_CNF                                = 16, TRC_EXPLAIN("EVENT_RIR_ACK_CNF")
    PNDV_AR_SM_EVENT_DISCONNECT_IND                             = 17, TRC_EXPLAIN("EVENT_DISCONNECT_IND")
    PNDV_AR_SM_EVENT_DISCONNECT_IND_DONE                        = 18, TRC_EXPLAIN("EVENT_DISCONNECT_IND_DONE")
    TRC_SUBLABEL_LIST_CLOSE("TRC_PNDV_AR_SM_EVENT")
    PNDV_AR_SM_EVENT_NOT_USED                                   = 255

}PNDV_AR_SM_EVENT_E_T;

#ifdef PNDV_TRACE_MSG
#define PNDV_TRACE_MSG_AR_EVENT_STR(_p_deb_str, _deb_code)    \
{                                                            \
    char* _pstr2;                                            \
    switch (_deb_code)                                         \
    {                                                       \
        case PNDV_AR_SM_EVENT_CONNECT_IND                       : _pstr2 = "CONNECT_IND";                 break; \
        case PNDV_AR_SM_EVENT_CONNECT_IND_OK_DONE               : _pstr2 = "CONNECT_IND_OK_DONE";         break; \
        case PNDV_AR_SM_EVENT_CONNECT_IND_ERROR_DONE            : _pstr2 = "CONNECT_IND_ERROR_DONE";     break; \
        case PNDV_AR_SM_EVENT_CONNECT_IND_PERI_OK_DONE          : _pstr2 = "CONNECT_IND_PERI_OK_DONE";     break; \
        case PNDV_AR_SM_EVENT_CONNECT_IND_PERI_ERROR_DONE       : _pstr2 = "CONNECT_IND_PERI_ERROR_DONE";break; \
        case PNDV_AR_SM_EVENT_OWNERSHIP_IND                     : _pstr2 = "OWNERSHIP_IND";             break; \
        case PNDV_AR_SM_EVENT_OWNERSHIP_IND_DONE                : _pstr2 = "OWNERSHIP_IND_DONE";         break; \
        case PNDV_AR_SM_EVENT_PRM_END_IND                       : _pstr2 = "PRM_END_IND";                 break; \
        case PNDV_AR_SM_EVENT_PRM_END_IND_DONE                  : _pstr2 = "PRM_END_IND_DONE";             break; \
        case PNDV_AR_SM_EVENT_FIRST_INPUT_UPDATE_DONE           : _pstr2 = "FIRST_INPUT_UPDATE_DONE";     break; \
        case PNDV_AR_SM_EVENT_APP_READY_RSP                     : _pstr2 = "APP_READY_RSP";             break; \
        case PNDV_AR_SM_EVENT_AR_IN_DATA                        : _pstr2 = "AR_IN_DATA";                 break; \
        case PNDV_AR_SM_EVENT_AR_IN_DATA_DONE                   : _pstr2 = "AR_IN_DATA_DONE";             break; \
        case PNDV_AR_SM_EVENT_RIR_IND                           : _pstr2 = "RIR_IND";                     break; \
        case PNDV_AR_SM_EVENT_RIR_ACK_CNF                       : _pstr2 = "RIR_ACK_CNF";                 break; \
        case PNDV_AR_SM_EVENT_DISCONNECT_IND                    : _pstr2 = "DISCONNECT_IND";            break; \
        case PNDV_AR_SM_EVENT_DISCONNECT_IND_DONE               : _pstr2 = "DISCONNECT_IND_DONE";         break; \
        default                                                 : _pstr2 = "UNDEFINED";                 break; \
    }                        \
    *_p_deb_str = _pstr2;     \
}
#endif /* PNDV_TRACE_MSG */


typedef enum
{
    PNDV_SM_MODUL_PLUG_NON          = 1,
    PNDV_SM_MODUL_PLUG_DIM_RUN      = 2,
    PNDV_SM_MODUL_PLUG_DIM          = 3,
    PNDV_SM_MODUL_PLUG_PERI_RUN     = 4,
    PNDV_SM_MODUL_PLUG_PERI_DONE    = 5,
    PNDV_SM_MODUL_RMV_PERI_PENDING  = 6,
    PNDV_SM_MODUL_RMV_PERI_RUN      = 7

}   PNDV_SM_MODUL_STATE;

#define PNDV_SOL_EVENT_FLAG_AR_00           (1 << 0)
#define PNDV_SOL_EVENT_FLAG_AR_01           (1 << 1)
#define PNDV_SOL_EVENT_FLAG_AR_02           (1 << 2)
#define PNDV_SOL_EVENT_FLAG_AR_03           (1 << 3)

#define PNDV_SOL_EVENT_FLAG_AR_04           (1 << 4)
#define PNDV_SOL_EVENT_FLAG_AR_05           (1 << 5)
#define PNDV_SOL_EVENT_FLAG_AR_06           (1 << 6)
#define PNDV_SOL_EVENT_FLAG_AR_07           (1 << 7)

#define PNDV_SOL_EVENT_FLAG_AR_08           (1 << 8)
#define PNDV_SOL_EVENT_FLAG_AR_09           (1 << 9)
#define PNDV_SOL_EVENT_FLAG_AR_10           (1 << 10)
#define PNDV_SOL_EVENT_FLAG_AR_11           (1 << 11)

#define PNDV_SOL_EVENT_FLAG_LOCKED_BY_ISOM  (1 << 12)
#define PNDV_SOL_EVENT_FLAG_PDEV_FAULT      (1 << 13)
#define PNDV_SOL_EVENT_FLAG_14              (1 << 14)
#define PNDV_SOL_EVENT_FLAG_15              (1 << 15)

#define PNDV_SOL_EVENT_FLAG_MOD_ID_HEAD     (1 << 16)
#define PNDV_SOL_EVENT_FLAG_PERIBUS_STATE   (1 << 17)
#define PNDV_SOL_EVENT_FLAG_ISOM_LOCKED     (1 << 18)
#define PNDV_SOL_EVENT_FLAG_LOCKED_BY_STACK (1 << 19)

#define PNDV_SOL_EVENT_FLAG_SHARED_NO_REMA  (1 << 20)


#define PNDV_SOL_NO_EVENT      0x00000000
#define PNDV_SOL_AR_EVENTS(_events) (_events & 0x00000FFF)
#define PNDV_SOL_ALL_ARS  0xFFFFFFFF


typedef enum PNDV_SM_CONNECT_SERVICE_STATE_E
{
    PNDV_SM_CONNECT_SERVICE_STATE_UNKNOWN           = 0,    /* initial state, connect service not initialized */
    PNDV_SM_CONNECT_SERVICE_STATE_DISABLED          = 1,    /* connect service is disabled */
    PNDV_SM_CONNECT_SERVICE_STATE_DISABLED_DELAY    = 2,    /* connect service is disabled, but connect is parked befor response */
    PNDV_SM_CONNECT_SERVICE_STATE_ENABLED           = 3,    /* connect service is enabled */
    PNDV_SM_CONNECT_SERVICE_STATE_ENABLED_DELAY     = 4     /* connect service is enabled, but connect is parked befor response */

}PNDV_SM_CONNECT_SERVICE_STATE_T;

typedef enum PNDV_SM_CONNECT_SERVICE_EVENT_E
{
    PNDV_SM_CONNECT_SERVICE_EVENT_ENABLE                  = 0,    //!< connect service is enabled
    PNDV_SM_CONNECT_SERVICE_EVENT_DISABLE                 = 1,    //!< connect service is disabled
    PNDV_SM_CONNECT_SERVICE_EVENT_DISABLE_DELAY           = 2,    //!< connect service is disabled, but connects are parked befor response
    PNDV_SM_CONNECT_SERVICE_EVENT_DELAY_TIMEOUT           = 3,    //!< timeout for a parked connect request
    PNDV_SM_CONNECT_SERVICE_EVENT_CONNECT_REQ             = 4     //!< a connect request

}PNDV_SM_CONNECT_SERVICE_EVENT_T;


#define PNDV_AR_IDX_NOT_USED             ((UnsignedOpt)0xFFFF)
#define PNDV_AR_NR_NOT_USED              ((Unsigned32)0xFFFFFFFF)
#define PNDV_SR_SET_IDX_NOT_USED         ((UnsignedOpt)0xFFFF)
#define PNDV_SR_SET_NR_NOT_USED          ((Unsigned16)0xFFFF)

/*****************************************************************************/
/* internal error sub-module defines */


#define PNDV_ERR_MODULE                  ((Unsigned8)0x01)
#define PNDV_ERR_MODULE_AL               ((Unsigned8)0x02)
#define PNDV_ERR_MODULE_C0               ((Unsigned8)0x03)
#define PNDV_ERR_MODULE_CM               ((Unsigned8)0x04)
#define PNDV_ERR_MODULE_DAT              ((Unsigned8)0x05)
#define PNDV_ERR_MODULE_DS               ((Unsigned8)0x06)
#define PNDV_ERR_MODULE_IO               ((Unsigned8)0x07)
#define PNDV_ERR_MODULE_PERI             ((Unsigned8)0x08)
#define PNDV_ERR_MODULE_OH               ((Unsigned8)0x09)
#define PNDV_ERR_MODULE_AR               ((Unsigned8)0x0A)
#define PNDV_ERR_MODULE_PP               ((Unsigned8)0x0B)
#define PNDV_ERR_MODULE_R1_BRIDGE        ((Unsigned8)0x0C)
#define PNDV_ERR_MODULE_R1               ((Unsigned8)0x0D)


/*****************************************************************************/
/* pndv_peri defines                                                         */
/*****************************************************************************/


/*****************************************************************************/
/* quatity structure */

/* 4 channel diagnosis per submodule  + 2 (K,G) Pack-err-diag -> allocate more if needed */
/* count of max possible submodules * (channel diags per submodule + 2 (coming and going )) + all going */
#define PNDV_DIAG_CHANNEL_ANZ  ( (PNDV_MAX_SV_SUBSLOTS * (PNDV_MAX_CHN_DIAG_PER_SLOT_AND_MOMENT + 2)) + PNDV_MAX_SV_SUBSLOTS )
#define PNDV_PP_AL_ANZ         (  PNDV_MAX_SV_SUBSLOTS * 2 )
#define PNDV_PRAL_ANZ          (  PNDV_MAX_SV_SUBSLOTS )


/*****************************************************************************/
/* these are the number communication ports */

/*****************************************************************************/

#define PNDV_SUBMODUL_1                  ((Unsigned16)0x1)
#define PNDV_SUBMODUL_UNKNOWN            ((Unsigned32)0xFFFFFFFF)

#define PNDV_PORT_HANDLE_RE                 1
#define PNDV_PORT_HANDLE_LI                 0

#define PNDV_EDD_PORT_ID_RE                 2
#define PNDV_EDD_PORT_ID_LI                 1

/*****************************************************************************/
/* internal macro */

#define PNDV_SUBMOD_ID_DEFAULT      0
#define PNDV_SUBMOD_ID_INVALID      0xFFFF

#define PNDV_SUB_IDX_INPUT     0
#define PNDV_SUB_IDX_OUTPUT    1

/* IOxS */
#define PNDV_IOXS_GB_MASK               0x80
#define PNDV_IOXS_GOOD                  0x80
#define PNDV_IOXS_BAD                   0x00

#define PNDV_IOXS_LOC_MASK              0x60
#define PNDV_IOXS_BAD_LOC_SUBSLOT       0x00
#define PNDV_IOXS_BAD_LOC_SLOT          0x20
#define PNDV_IOXS_BAD_LOC_DEVICE        0x40
#define PNDV_IOXS_BAD_LOC_CONTROLLER    0x60


#ifdef IM_SUPPORT_PN_RX_REDUNDANCY

#define PNDV_IS_PDEV(_slot_nr, _subslot_nr)              \
    (   (   (_slot_nr == PNDV_IM_SLOT_NO)                \
         || (_slot_nr == PNDV_IM_SLOT_NO_PARTNER)        \
        )                                                \
     && (   ((_subslot_nr & 0xF000) == 0x8000)           \
         || ((_subslot_nr & 0xF000) == 0x9000)           \
        )                                                \
    )

#else

#define PNDV_IS_PDEV(_slot_nr, _subslot_nr)              \
    (   (_slot_nr == PNDV_IM_SLOT_NO)                    \
     && ((_subslot_nr & 0xF000) == 0x8000)               \
    )

#endif


/* record structure for read input/read output */
/* ------------------------------------------------------------------------- */

/* 1. Common */
/* ------------------------------------------------------------------------- */

#define PNDV_RD_REC_POS_BLOCK_TYPE_H   0
#define PNDV_RD_REC_POS_BLOCK_TYPE_L   1
#define PNDV_RD_REC_POS_BLOCK_LEN_H    2
#define PNDV_RD_REC_POS_BLOCK_LEN_L    3  /* len counts from block version */
#define PNDV_RD_REC_POS_BLOCK_VERS_H   4
#define PNDV_RD_REC_POS_BLOCK_VERS_L   5


#define PNDV_IO_RECORD_VERSION_HIGH         1
#define PNDV_IO_RECORD_VERSION_LOW          0

/* 1. Input */
/* ------------------------------------------------------------------------- */

    /* according 65C_3xx_NP_PROFINET_IO_v20_i282.doc

       length in brackets

       RecordInputDataObjectElement(12 + netto_src_data_len):
           BlockHeader(6), LengthIOCS(1), IOCS(1), LengthIOPS(1), IOPS(1), LengthData(2), Data(netto_src_data_len)

               BlockHeader:
                    BlockType(2), BlockLength(2), BlockVersionHigh(1), BlockVersionLow(1)
    */


#define PNDV_RD_INP_REC_POS_LENGTH_IOCS    6
#define PNDV_RD_INP_REC_POS_IOCS           7
#define PNDV_RD_INP_REC_POS_LENGTH_IOPS    8
#define PNDV_RD_INP_REC_POS_IOPS           9
#define PNDV_RD_INP_REC_POS_LENGTH_DATA_H 10
#define PNDV_RD_INP_REC_POS_LENGTH_DATA_L 11
#define PNDV_RD_INP_REC_POS_DATA          12

#define PNDV_RD_INP_REC_BLOCK_LEN_FIX     8  /* PNDV_RD_INP_REC_POS_BLOCK_VERS_H bis PNDV_RD_INP_REC_POS_LENGTH_DATA_L */

#define PNDV_IO_RECORD_BLK_TYPE_INPUT  0x0015

/* 2. Output */
/* ------------------------------------------------------------------------- */

    /* according 65C_3xx_NP_PROFINET_IO_v20_i282.doc

       length in brackets

       RecordOutputDataObjectElement
           BlockHeader(6), SubstituteActiveFlag(2), LengthIOCS(1), LengthIOPS(1),
           LengthData(2), DataItem(netto_src_data_len+2), SubstituteValue

                BlockHeader(6):
                    BlockType(2), BlockLength(2), BlockVersionHigh(1), BlockVersionLow(1)

                SubstituteActiveFlag:   0 - OPERATION
                                        1 - SUBSTITUTE

                DataItem:
                    IOCS(1), DataObjectElement(netto_src_data_len + 1)

                        DataObjectElement   Data(netto_src_data_len), IOPS(1)

                SubstituteValue:
                    BlockHeader(6), SubstitutionMode(2), DataItem(netto_src_data_len+2)

               because there are no substitude values available
                -> SubstitutionMode = 0
                   DataItem.DataObjectElement.IOPS = BAD
                   DataItem.IOCS                   = RecordOutputDataObjectElement.DataItem.IOCS
    */

#define PNDV_RD_OUTP_REC_POS_SUBST_ACT_FLAG_H   6
#define PNDV_RD_OUTP_REC_POS_SUBST_ACT_FLAG_L   7
#define PNDV_RD_OUTP_REC_POS_LENGTH_IOCS        8
#define PNDV_RD_OUTP_REC_POS_LENGTH_IOPS        9
#define PNDV_RD_OUTP_REC_POS_LENGTH_DATA_H     10
#define PNDV_RD_OUTP_REC_POS_LENGTH_DATA_L     11
#define PNDV_RD_OUTP_REC_POS_DATA_ITEM         12

#define PNDV_RD_OUTP_REC_BLOCK_LEN_FIX     16  /* PNDV_RD_REC_POS_BLOCK_VERS_H bis PNDV_RD_OUTP_REC_POS_LENGTH_DATA_L + fixed part of SubstituteValue*/

#define PNDV_RD_OUTP_REC_DATA_ITEM_LEN_FIX 2  /* IOCS + IOPS */

#define PNDV_IO_RECORD_BLK_TYPE_OUTPUT     0x0016

/* 3.SubstituteValue  */
/* ------------------------------------------------------------------------- */

    /* according 65C_3xx_NP_PROFINET_IO_v20_i282.doc

       length in brackets

        SubstituteValue:
            BlockHeader(6), SubstitutionMode(2), DataItem(netto_src_data_len+2)

        because there are no substitude values available
        -> SubstitutionMode = 0
           DataItem.DataObjectElement.IOPS = BAD
           DataItem.IOCS                   = RecordOutputDataObjectElement.DataItem.IOCS
    */

#define PNDV_RD_SUBST_REC_BLOCK_LEN_FIX    4 /* PNDV_RD_REC_POS_BLOCK_VERS_H to SubstitutionMode */

#define PNDV_IO_RECORD_BLK_TYPE_SUBSTITUDE 0x0014

#define PNDV_MAX_RD_INP_DATA_LEN              (PNDV_MAX_RD_IO_DATA_LEN + PNDV_RD_INP_REC_POS_DATA)
#define PNDV_MAX_RD_OUTP_DATA_LEN            ((PNDV_RD_OUTP_REC_BLOCK_LEN_FIX+4) + 2 * (PNDV_MAX_RD_IO_DATA_LEN + PNDV_RD_OUTP_REC_DATA_ITEM_LEN_FIX))



/* prm record defines */

#define PNDV_RM_REC_BLOCK_VERS_H    0x01
#define PNDV_RM_REC_BLOCK_VERS_L    0x00

#define PNDV_PRM_REC_BLOCK_HEADER_LEN           0x04
#define PNDV_PRM_REC_BLOCK_TYPE_NAME_OF_STATION 0xA201
#define PNDV_PRM_REC_BLOCK_LEN_NAME_OF_STATION  0x08 /* length without name length */
#define PNDV_PRM_REC_BLOCK_TYPE_IP_SUITE        0x3000
#define PNDV_PRM_REC_BLOCK_LEN_IP_SUITE         0x04 /* length without ip data length */

#define PNDV_PRM_REC_BLOCK_LEN_SNMP             0x14 /* length of a single snmp element without snmp data */



/*****************************************************************************/
/* internal data */

/* saved modul states */

typedef enum PNDV_SUBMODULE_STATE_E
{
    PNDV_SUBMODULE_STATE_NO_MODULE           = 0,
    PNDV_SUBMODULE_STATE_NO_OWNER            = 1,
    PNDV_SUBMODULE_STATE_VALID_MODUL         = 2,
    PNDV_SUBMODULE_STATE_WRONG_MODUL         = 3

}   PNDV_SUBMODULE_STATE_T;

#define PNDV_MAX_PACK_MEMBERS 4


/* channel diagnosis */

typedef enum PNDV_DIAG_CH_STATE_E
{
    /* see sm_diag.vsd */

    PNDV_DIAG_CH_STATE_FREE            = 0,
    PNDV_DIAG_CH_STATE_REQ_AL             ,
    PNDV_DIAG_CH_STATE_W_DIAG_DONE_AL     ,
    PNDV_DIAG_CH_STATE_DIAG_DONE

}   PNDV_DIAG_CH_STATE_T;


#define PNDV_PARA_GET_FIRST_DS       1
#define PNDV_PARA_GET_NEXT_DS        2


/*
    for cm_dev_req_running and cm_dev_req_pending
*/

#define PNDV_CM_DEV_REQ_DEVICE_ADD          0x0001
#define PNDV_CM_DEV_REQ_DEVICE_RMV          0x0002
#define PNDV_CM_DEV_REQ_AP_ADD              0x0004
#define PNDV_CM_DEV_REQ_AP_REMOVE           0x0008
#define PNDV_CM_DEV_REQ_DEVICE_CONTROL      0x0010
#define PNDV_CM_DEV_REQ_MODULE_ADD          0x0020
#define PNDV_CM_DEV_REQ_MODULE_RMV          0x0040
#define PNDV_CM_DEV_REQ_SUBMODULE_ADD       0x0080
#define PNDV_CM_DEV_REQ_SUBMODULE_RMV       0x0100


/* ------------------------------------------------------------------------- */


typedef enum PNDV_MODUL_PP_SRC_E
{
    PNDV_MODUL_PP_SRC_NIX  = 0,
    PNDV_MODUL_PP_SRC_PNDV,
    PNDV_MODUL_PP_SRC_PERI_PLUG,
    PNDV_MODUL_PP_SRC_PERI_PULL

}   PNDV_MODUL_PP_SRC_T;


typedef enum PNDV_SUBMODUL_PLUG_STATE_E
{
    PNDV_SUBMODUL_PLUG_STATE_NIX  = 0,
    PNDV_SUBMODUL_PLUG_STATE_PEND,       /* req from peri has arrived */
    PNDV_SUBMODUL_PLUG_STATE_ADD,        /* submodul_add to CM */
    PNDV_SUBMODUL_PLUG_STATE_PARA_DONE,  /* paramter setting is done */

}   PNDV_SUBMODUL_PLUG_STATE_T;

/************************************
    enum describing the type of an AR
*************************************/
typedef enum PNDV_ENUM_AR_E
{
    PNDV_AR_TYPE_UNKNOWN,                       /* an unknown / not used ar type */
    PNDV_AR_TYPE_OLD_PROJECTING,                /* this is a legacy ar */
    PNDV_AR_TYPE_PRIMARY_WITH_PDEV,             /* this ar is primary (owns subslot 1 of the head) and owns the pdev submodules */
    PNDV_AR_TYPE_PRIMARY_WITHOUT_PDEV,          /* this ar is primary (owns subslot 1 of the head) and does not own the pdev submodules */
    PNDV_AR_TYPE_SHARED_WITH_PDEV,              /* this ar is not primary (not owns subslot 1 of the head) but owns the pdev submodules */
    PNDV_AR_TYPE_SHARED_WITHOUT_PDEV,           /* this ar is not primary (not owns subslot 1 of the head) and does not own the pdev submodules */
    PNDV_AR_TYPE_SYSRED,                        /* this ar is a system redundancy ar and can not be combined with sharing */
    PNDV_AR_TYPE_SUPERVISOR_DEVICE_ACCESS

}   PNDV_ENUM_AR_T;

/****************************************
    enum describing the check_ind ret_val
*****************************************/
typedef enum PNDV_CONNECT_IND_RET_VAL_E
{
    PNDV_CONNECT_IND_RET_OK       = 1,
    PNDV_CONNECT_IND_RET_ERROR_RESOURCE,
    PNDV_CONNECT_IND_RET_ABORT

}   PNDV_CONNECT_IND_RET_VAL_T;


#define PNDV_RD_IO_RECORD_STATE_NONE      1 /* there is no task */
#define PNDV_RD_IO_RECORD_STATE_COPY_REQ  2 /* data has to be copied */
#define PNDV_RD_IO_RECORD_STATE_COPY_DONE 3 /* data is copied */
#define PNDV_RD_IO_RECORD_STATE_QUIT_REQ  4 /* record can be quit */



/* structure ChannelDial and ExtChannelDial */
/* ------------------------------------------------------------------------- */

/* (Ext)ChannelDiagnosisData.ChannelNumber  */
#define PNDV_AL_DIAL_INFO_OFFS_CHANNEL_NO_H  0
#define PNDV_AL_DIAL_INFO_OFFS_CHANNEL_NO_L  1

/* (Ext)ChannelDiagnosisData.ChannelProperties; Bit 0..7 for type*/
#define PNDV_AL_DIAL_INFO_OFFS_CHANNEL_SPEC  2
#define PNDV_AL_DIAL_INFO_OFFS_CHANNEL_TYPE  3

/* (Ext)ChannelDiagnosisData.ChannelErrorType */
#define PNDV_AL_DIAL_INFO_OFFS_ERR_TYPE_H    4
#define PNDV_AL_DIAL_INFO_OFFS_ERR_TYPE_L    5

/* (Ext)ChannelDiagnosisData.ExtChannelErrorType */
#define PNDV_AL_DIAL_INFO_OFFS_XERR_TYPE_H    6
#define PNDV_AL_DIAL_INFO_OFFS_XERR_TYPE_L    7

/* (Ext)ChannelDiagnosisData.ExtChannelAddValue */
#define PNDV_AL_DIAL_INFO_OFFS_ADD_VALUE_B3    8
#define PNDV_AL_DIAL_INFO_OFFS_ADD_VALUE_B2    9
#define PNDV_AL_DIAL_INFO_OFFS_ADD_VALUE_B1    10
#define PNDV_AL_DIAL_INFO_OFFS_ADD_VALUE_B0    11

#define PNDV_AL_DIAL_CHN_INFO_LEN            6
#define PNDV_AL_DIAL_XCH_INFO_LEN            12

#if PNDV_MAX_GENERIC_DIAG_DATA_LENGTH > 12
 #define PNDV_AL_DIAL_MAX_INFO_LEN            PNDV_MAX_GENERIC_DIAG_DATA_LENGTH
#else
 #define PNDV_AL_DIAL_MAX_INFO_LEN            12
#endif


#define PNDV_AL_ROSAL_INFO_LEN               1


/*****************************************************************************/
/* internal structures */

/*****************************************************************************/
/* debug */

TRC_DECLARE_OPEN("TRC_ID_PNDV")
    TRC_EVENT_LIST_LINK("TRC_PNDV_EVENT")
TRC_DECLARE_CLOSE("TRC_ID_PNDV")

#ifdef TRC_ONLY_TRACE_SCANNER

TRC_STRUCT_OPEN("TRC_PNDV_STRUCT_0")
    /* nothing */
TRC_STRUCT_CLOSE("TRC_PNDV_STRUCT_0")

TRC_STRUCT_OPEN("TRC_PNDV_STRUCT_1")
    BYTE "_fill_1"
    WORD "info_1"
TRC_STRUCT_CLOSE("TRC_PNDV_STRUCT_1")

TRC_STRUCT_OPEN("TRC_PNDV_STRUCT_2")
    BYTE "_fill_1"
    WORD "info_1"
    WORD "info_2"
TRC_STRUCT_CLOSE("TRC_PNDV_STRUCT_2")

TRC_STRUCT_OPEN("TRC_PNDV_STRUCT_3")
    BYTE  "_fill_1"
    WORD  "info_1"
    WORD  "info_2"
    DWORD "info_3"
TRC_STRUCT_CLOSE("TRC_PNDV_STRUCT_3")

TRC_STRUCT_OPEN("TRC_PNDV_STRUCT_AL_ACK")
    BYTE      "_fill_1"
    WORD      "entity"
    uint16_t  "sequence"  /* take the decimal form - CM use the same in the trace */
    DWORD     "response"
TRC_STRUCT_CLOSE("TRC_PNDV_STRUCT_AL_ACK")

TRC_STRUCT_OPEN("TRC_PNDV_STRUCT_SM")
    BYTE "_fill_1"
    WORD "PNDV_SM_EVENT", TRC_SUBLABEL_LIST_LINK ("TRC_PNDV_SM_EVENT")
    WORD "PNDV_SM_STATE", TRC_SUBLABEL_LIST_LINK ("TRC_PNDV_SM_STATE")
TRC_STRUCT_CLOSE("TRC_PNDV_STRUCT_SM")

TRC_STRUCT_OPEN("TRC_PNDV_STRUCT_SR_SM")
    BYTE "_fill_1"
    WORD "PNDV_SM_EVENT", TRC_SUBLABEL_LIST_LINK ("TRC_PNDV_SR_SM_EVENT")
    WORD "PNDV_SM_STATE", TRC_SUBLABEL_LIST_LINK ("TRC_PNDV_SR_SM_STATE")
    DWORD "SR-Set Index"
TRC_STRUCT_CLOSE("TRC_PNDV_STRUCT_SR_SM")

TRC_STRUCT_OPEN("TRC_PNDV_STRUCT_AR_SM")
    BYTE  "_fill_1"
    WORD  "PNDV_AR_SM_EVENT", TRC_SUBLABEL_LIST_LINK ("TRC_PNDV_AR_SM_EVENT")
    WORD  "PNDV_AR_SM_STATE", TRC_SUBLABEL_LIST_LINK ("TRC_PNDV_AR_SM_STATE")
    DWORD "AR Index"
TRC_STRUCT_CLOSE("TRC_PNDV_STRUCT_AR_SM")

TRC_STRUCT_OPEN("TRC_PNDV_STRUCT_PP_SM")
    BYTE "_fill_1"
    WORD  "PNDV_SM_EVENT"    , TRC_SUBLABEL_LIST_LINK ("TRC_PNDV_PP_SM_EVENT")
    BYTE  "PNDV_SM_STATE_OLD", TRC_SUBLABEL_LIST_LINK ("TRC_PNDV_PP_SM_STATE")
    BYTE  "PNDV_SM_STATE"    , TRC_SUBLABEL_LIST_LINK ("TRC_PNDV_PP_SM_STATE")
    DWORD "RES_PTR"
TRC_STRUCT_CLOSE("TRC_PNDV_STRUCT_PP_SM")

TRC_STRUCT_OPEN("TRC_PNDV_STRUCT_COUPLE_EVENT")
    BYTE  "_fill_1"
    WORD  "PNDV_PERI_EVENT", TRC_SUBLABEL_LIST_LINK ("TRC_PNDV_PERI_EVENT")
    WORD  "ADD Info 1"
    DWORD "ADD Info 2"
TRC_STRUCT_CLOSE("TRC_PNDV_STRUCT_COUPLE_EVENT")

TRC_STRUCT_OPEN("TRC_PNDV_STRUCT_ERROR")
    BYTE "_fill_1"
    WORD "error_code"
TRC_STRUCT_CLOSE("TRC_PNDV_STRUCT_ERROR")

TRC_STRUCT_OPEN("TRC_PNDV_STRUCT_DS_RW")
    BYTE "_fill_1"
    WORD "slot_nr"
    WORD "subslot_nr"
    DWORD "index"
TRC_STRUCT_CLOSE("TRC_PNDV_STRUCT_DS_RW")

TRC_STRUCT_OPEN("TRC_PNDV_STRUCT_AR_IP")
    BYTE "_fill_1"
    WORD "ar_idx"
    uint16_t "datalen"
    IP_ADDR   "IP"
TRC_STRUCT_CLOSE("TRC_PNDV_STRUCT_AR_IP")

TRC_STRUCT_OPEN("TRC_PNDV_STRUCT_AR_OWNERSHIP_RESP")
    BYTE  "_fill_1"
    WORD  "PNDV_CMP_RES", TRC_SUBLABEL_LIST_LINK ("TRC_PNDV_CMP_RES")
    WORD  "session key"
    DWORD "flags"
TRC_STRUCT_CLOSE("TRC_PNDV_STRUCT_AR_OWNERSHIP_RESP")

TRC_STRUCT_OPEN("TRC_PNDV_STRUCT_COMMON_MODULE_INFO")
    BYTE  "_fill_1"
    WORD  "slot"
    WORD  "subslot"
    DWORD "sub id/mod id (H/L)"
TRC_STRUCT_CLOSE("TRC_PNDV_STRUCT_COMMON_MODULE_INFO")

TRC_PNDV_STRUCT_LINE_INFO
TRC_STRUCT_OPEN("TRC_PNDV_STRUCT_LINE_INFO")
    BYTE "_fill_1"
    uint16_t "module_id"
    uint16_t "line number"
TRC_STRUCT_CLOSE("TRC_PNDV_STRUCT_LINE_INFO")
#endif /*TRC_ONLY_TRACE_SCANNER*/


typedef enum PNDV_DEBUG_CODE_E
{
TRC_EVENT_LIST_OPEN("TRC_PNDV_EVENT")
        DUMMY                                                        = 0, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("DUMMY")
        PNDV_DC_ALL_OPEN                                             = 1, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("ALL_OPEN")
        PNDV_DC_ALL_START                                            = 2, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("ALL_START")
        PNDV_DC_ALL_STOP                                             = 3, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("ALL_STOP")
        PNDV_DC_CM_OPEN_CH                                           = 4, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_OPEN_CH")
        PNDV_DC_CM_CLOSE_CH                                          = 5, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_CLOSE_CH")
        PNDV_DC_CM_RES_PROV                                          = 6, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_RES_PROV")
        PNDV_DC_CM_RES_BACK                                          = 7, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_RES_BACK")
/*
 * 8
 */
        PNDV_DC_CM_LINK_STATUS_IND                                   = 9, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_LINK_STATUS_IND")
        PNDV_DC_CM_SV_CREATE                                         = 10, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_SV_CREATE")
        PNDV_DC_CM_SV_DELETE                                         = 11, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_SV_DELETE")
        PNDV_DC_CM_DV_ADD                                            = 12, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_DV_ADD")
        PNDV_DC_CM_DV_REMOVE                                         = 13, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_DV_REMOVE")
        PNDV_DC_CM_DV_AKT                                            = 14, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_DV_AKT")
        PNDV_DC_CM_CONNECT_IND                                       = 15, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_CONNECT_IND")/* sm, ar_idx */

        // PNDV SM's
        PNDV_DC_PNDV_SM                                              = 16, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_SM") TRC_EXPLAIN("PNDV_SM")
        PNDV_DC_PNDV_AR_SM                                           = 17, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_AR_SM")  TRC_EXPLAIN("PNDV_AR_SM")
        PNDV_DC_PNDV_SR_SM                                           = 18, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_SR_SM")  TRC_EXPLAIN("PNDV_SR_SM")
        PNDV_DC_PNDV_PP_SM                                           = 19, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_PP_SM")  TRC_EXPLAIN("PNDV_PP_SM")

        PNDV_DC_CM_ARSET_ABORT                                       = 20, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_ARSET_ABORT")
        PNDV_DC_CM_ARSET_TRIGGER                                     = 21, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_ARSET_TRIGGER")
        PNDV_DC_CM_AR_ABORT_DONE                                     = 22, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_AR_ABORT_DONE")/* ar_idx */
        PNDV_DC_CM_AR_DISCONNECT_IND                                 = 23, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_AR_DISCONNECT_IND")/* reason, ar_idx */

        PNDV_DC_CM_AR_R1_OWNERSHIP_PASSIVE                           = 24, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("AR_R1_OWNERSHIP_PASSIVE")

        PNDV_DC_CM_AR_ABORT                                          = 25, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_AR_ABORT")/* ar_idx */
        PNDV_DC_CM_ADD_CHANNEL_DIAG_REQ                              = 26, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_ADD_CHANNEL_DIAG_REQ")
        PNDV_DC_CM_RMV_CHANNEL_DIAG_REQ                              = 27, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_RMV_CHANNEL_DIAG_REQ")
        PNDV_DC_CM_RMV_CHANNEL_DIAG_ALL_REQ                          = 28, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_RMV_CHANNEL_DIAG_ALL_REQ")
        PNDV_DC_CM_ADD_CHANNEL_DIAG                                  = 29, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_ADD_CHANNEL_DIAG")
        PNDV_DC_CM_RMV_CHANNEL_DIAG                                  = 30, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_RMV_CHANNEL_DIAG")
        PNDV_DC_CM_ADD_CHANNEL_DIAG_CBF                              = 31, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_ADD_CHANNEL_DIAG_CBF")
        PNDV_DC_CM_ADD_CHANNEL_DIAG_DONE_CBF                         = 32, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_ADD_CHANNEL_DIAG_DONE_CBF")
        PNDV_DC_CM_RMV_CHANNEL_DIAG_CBF                              = 33, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_RMV_CHANNEL_DIAG_CBF")
        PNDV_DC_CM_DIAG_ADD_DONE_ERR_ELEM                            = 34, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_DIAG_ADD_DONE_ERR_ELEM")
        PNDV_DC_CM_DIAG_RMV_DONE_ERR_ELEM                            = 35, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_DIAG_RMV_DONE_ERR_ELEM")
        PNDV_DC_SF_OFF                                               = 36, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("SF_OFF")
        PNDV_DC_SF_ON                                                = 37, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("SF_ON")
        PNDV_DC_CM_PRAL_REQ                                          = 38, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_PRAL_REQ")

        PNDV_DC_AR_OWNERSHIP_RESP                                    = 39, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_AR_OWNERSHIP_RESP") TRC_EXPLAIN("AR_OWNERSHIP_RESP")

        PNDV_DC_CM_UPAL_REQ                                          = 40, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_UPAL_REQ")
        PNDV_DC_CM_URAL_REQ                                          = 41, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_URAL_REQ")
        PNDV_DC_CM_STAL_REQ                                          = 42, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_STAL_REQ")
        PNDV_DC_CM_ADD_EXT_CHANNEL_DIAG_REQ                          = 43, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_ADD_EXT_CHANNEL_DIAG_REQ")
        PNDV_DC_CM_RMV_EXT_CHANNEL_DIAG_REQ                          = 44, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_RMV_EXT_CHANNEL_DIAG_REQ")
        PNDV_DC_CM_RMV_EXT_CHANNEL_DIAG_ALL_REQ                      = 45, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_RMV_EXT_CHANNEL_DIAG_ALL_REQ")
        PNDV_DC_CM_RMV_EXT_CHANNEL_DIAG_ALL_REQ_PNDV                 = 46, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_RMV_EXT_CHANNEL_DIAG_ALL_REQ_PNDV")
        PNDV_DC_CM_ADD_EXT_CHANNEL_DIAG                              = 47, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_ADD_EXT_CHANNEL_DIAG")
        PNDV_DC_CM_RMV_EXT_CHANNEL_DIAG                              = 48, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_RMV_EXT_CHANNEL_DIAG")
        PNDV_DC_CM_ADD_EXT_CHANNEL_DIAG_CBF                          = 49, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_ADD_EXT_CHANNEL_DIAG_CBF")
        PNDV_DC_CM_ADD_EXT_CHANNEL_DIAG_DONE_CBF                     = 50, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_ADD_EXT_CHANNEL_DIAG_DONE_CBF")
        PNDV_DC_CM_RMV_EXT_CHANNEL_DIAG_CBF                          = 51, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_RMV_EXT_CHANNEL_DIAG_CBF")

/*
 * 52
 */
        PNDV_DC_CM_SV_DEVICE_PROVIDE_EVENT_DONE                          = 53, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_SV_DEVICE_PROVIDE_EVENT_DONE")
        PNDV_DC_CM_SV_DEVICE_PROVIDE_ALARM_DONE                          = 54, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_SV_DEVICE_PROVIDE_ALARM_DONE")
		
/*
 * 55
 * -
 * 67
 */
        PNDV_DC_LINE_INFO                                            = 68, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_LINE_INFO")  TRC_EXPLAIN("LINE_INFO")
        PNDV_DC_PERI_BUSAUSFALL                                      = 69, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("PERI_BUSAUSFALL")
        PNDV_DC_PERI_BUSWIEDERKEHR                                   = 70, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("PERI_BUSWIEDERKEHR")

    /* PNDV_ENUM_AL_DETAIL_1: */

/*
 * 71
 */
        PNDV_DC_CM_AL_RES_PROV                                       = 72, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")       TRC_EXPLAIN("CM_AL_RES_PROV")
        PNDV_DC_CM_AL_RES_BACK                                       = 73, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")       TRC_EXPLAIN("CM_AL_RES_BACK")
        PNDV_DC_CM_AL_IND                                            = 74, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")       TRC_EXPLAIN("CM_AL_IND")
        PNDV_DC_CM_AL_ACK_IND                                        = 75, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")       TRC_EXPLAIN("CM_AL_ACK_IND")
        PNDV_DC_CM_DIAL_SEND                                         = 76, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")       TRC_EXPLAIN("CM_DIAL_SEND")
        PNDV_DC_CM_DIAL_ACK                                          = 77, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_AL_ACK")  TRC_EXPLAIN("CM_DIAL_ACK")
        PNDV_DC_CM_PRAL_SEND                                         = 78, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")       TRC_EXPLAIN("CM_PRAL_SEND")
        PNDV_DC_CM_PRAL_ACK                                          = 79, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_AL_ACK")  TRC_EXPLAIN("CM_PRAL_ACK")
        PNDV_DC_CM_UPAL_SEND                                         = 80, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")       TRC_EXPLAIN("CM_UPAL_SEND")
        PNDV_DC_CM_UPAL_ACK                                          = 81, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_AL_ACK")  TRC_EXPLAIN("CM_UPAL_ACK")
        PNDV_DC_CM_URAL_SEND                                         = 82, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")       TRC_EXPLAIN("CM_URAL_SEND")
        PNDV_DC_CM_URAL_ACK                                          = 83, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_AL_ACK")  TRC_EXPLAIN("CM_URAL_ACK")
        PNDV_DC_CM_STAL_SEND                                         = 84, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")       TRC_EXPLAIN("CM_STAL_SEND")
        PNDV_DC_CM_STAL_ACK                                          = 85, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_AL_ACK")  TRC_EXPLAIN("CM_STAL_ACK")

        PNDV_DC_DS_WRITE                                             = 86, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("DS_WRITE")
        PNDV_DC_DS_WRITE_DONE                                        = 87, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("DS_WRITE_DONE")
        PNDV_DC_DS_READ_DONE                                         = 88, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("DS_READ_DONE")
        PNDV_DC_PERI_DS_RW                                           = 89, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_DS_RW")  TRC_EXPLAIN("PERI_DS_RW")
        PNDV_DC_PERI_DS_RW_DONE                                      = 90, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("PERI_DS_RW_DONE")
        PNDV_DC_DFC_DS_REQ                                           = 91, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("DFC_DS_REQ")/* not used */
        PNDV_DC_DFC_DS_DONE                                          = 92, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("DFC_DS_DONE")/* slot, err */

        PNDV_DC_COMMON_MODUL_INFO                                    = 93, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_COMMON_MODULE_INFO")  TRC_EXPLAIN("COMMON_MODULE_INFO")/* slot, err */

        PNDV_DC_CM_UPAL_DONT_SEND                                    = 94, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_UPAL_DONT_SEND")
        PNDV_DC_CM_URAL_DONT_SEND                                    = 95, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_URAL_DONT_SEND")
        PNDV_DC_CM_STAL_DONT_SEND                                    = 96, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_STAL_DONT_SEND")
        PNDV_DC_CM_PRAL_DONT_SEND                                    = 97, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_PRAL_DONT_SEND")
        PNDV_DC_CM_ADD_GENERIC_DIAG                                  = 98, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_ADD_GENERIC_DIAG")
        PNDV_DC_CM_ADD_GENERIC_DIAG_CBF                              = 99, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_ADD_GENERIC_DIAG_CBF")
        PNDV_DC_CM_ADD_GENERIC_DIAG_DONE_CBF                         = 100, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_ADD_GENERIC_DIAG_DONE_CBF")

        PNDV_DC_PERI_WRITE_EVENT_LOW                                 = 101, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_COUPLE_EVENT")  TRC_EXPLAIN("PERI_WRITE_EVENT_LOW")
        PNDV_DC_PERI_WRITE_EVENT_HIGH                                = 102, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_COUPLE_EVENT")  TRC_EXPLAIN("PERI_WRITE_EVENT_HIGH")
        PNDV_DC_PERI_READ_EVENT_LOW                                  = 103, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_COUPLE_EVENT")  TRC_EXPLAIN("PERI_READ_EVENT_LOW")
        PNDV_DC_PERI_READ_EVENT_HIGH                                 = 104, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_COUPLE_EVENT")  TRC_EXPLAIN("PERI_READ_EVENT_HIGH")
        PNDV_DC_CIR_RMV                                              = 105, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_COMMON_MODULE_INFO")  TRC_EXPLAIN("CIR_RMV")/* slot, err */

        PNDV_DC_STORE_PARA_DS                                        = 106, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("STORE_PARA_DS")
        PNDV_DC_STORE_PARA_DS_CHANGE                                 = 107, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("STORE_PARA_DS_CHANGE")
        PNDV_DC_GET_PARA_DS                                          = 108, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("GET_PARA_DS")
        PNDV_DC_GET_NON_PARA_DS                                      = 109, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("GET_NON_PARA_DS")
        PNDV_DC_DFC_CM_DS_RD_REQ                                     = 110, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_DS_RW")  TRC_EXPLAIN("DFC_CM_DS_RD_REQ")
        PNDV_DC_DFC_CM_DS_WR_REQ                                     = 111, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_DS_RW")  TRC_EXPLAIN("DFC_CM_DS_WR_REQ")
        PNDV_DC_CM_CONTROL_STATION_DONE                              = 112, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_CONTROL_STATION_DONE")
        PNDV_DC_CM_RMV_GENERIC_DIAG                                  = 113, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_RMV_GENERIC_DIAG")
        PNDV_DC_CM_RMV_GENERIC_DIAG_CBF                              = 114, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_RMV_GENERIC_DIAG_CBF")
        PNDV_DC_CM_AL_ACK                                            = 115, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_AL_ACK")
        PNDV_DC_DFC_CM_DS_RW_REQ                                     = 116, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_AR_IP")  TRC_EXPLAIN("DFC_CM_DS_RW_REQ")
        PNDV_DC_CM_CONNECT_RSP_SYNC                                  = 117, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_CONNECT_RSP_SYNC")
        PNDV_DC_CM_CONNECT_RSP_ASYNC                                 = 118, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_CONNECT_RSP_ASYNC")
        PNDV_DC_IO_ADD_MODULE_SPAETER                                = 119, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("IO_ADD_MODULE_SPAETER")
        PNDV_DC_IO_ADD_SUBMODULE_SPAETER                             = 120, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("IO_ADD_SUBMODULE_SPAETER")
        PNDV_DC_IO_REM_MODULE_SPAETER                                = 121, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("IO_REM_MODULE_SPAETER")
        PNDV_DC_IO_REM_SUBMODULE_SPAETER                             = 122, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("IO_REM_SUBMODULE_SPAETER")
        PNDV_DC_CM_DELETE_PP_REQ                                     = 123, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_DELETE_PP_REQ")
        PNDV_DC_CM_AP_ADD                                            = 124, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_AP_ADD")
        PNDV_DC_CM_OVERLOAD_RES_PROV                                 = 125, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_OVERLOAD_RES_PROV")
        PNDV_DC_CM_LINK_STATUS_RES_PROV                              = 126, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_LINK_STATUS_RES_PROV")
        PNDV_DC_CM_ROSAL_REQ                                         = 127, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_ROSAL_REQ")
        PNDV_DC_CM_ROSAL_SEND                                        = 128, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_ROSAL_SEND")
        PNDV_DC_CM_ROSAL_ACK                                         = 129, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_ROSAL_ACK")
        PNDV_DC_CM_NOT_USED_130                                      = 130, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_NOT_USED_130")/* not used */
        PNDV_DC_CM_PLUG_REQ_PNDV                                     = 131, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_PLUG_REQ_PNDV")
        PNDV_DC_CM_RMV_DIAG_CBF_ERR                                  = 132, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_RMV_CHANNEL_DIAG_CBF_ERR")
        PNDV_DC_CM_SLOT_NOT_READY                                    = 133, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_SLOT_NOT_READY")
        PNDV_DC_CM_NOT_USED_134                                      = 134, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_NOT_USED_134")
        PNDV_DC_CM_OPEN_CH_DONE                                      = 135, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_OPEN_CH_DONE")
        PNDV_DC_CM_CLOSE_CH_DONE                                     = 136, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_CLOSE_CH_DONE")
/*
 * 137
 */
        PNDV_DC_START_DONE                                           = 138, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("START_DONE")
        PNDV_DC_STOP_DONE                                            = 139, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("STOP_DONE")
        PNDV_DC_CM_ADD_PACK_ERR_DIAG_REQ                             = 140, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_ADD_PACK_ERR_DIAG_REQ")
        PNDV_DC_CM_RMV_PACK_ERR_DIAG_REQ                             = 141, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_RMV_PACK_ERR_DIAG_REQ")
        PNDV_DC_CM_DEL_PACK_ERR_DIAG_REQ                             = 142, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_DEL_PACK_ERR_DIAG_REQ")
        PNDV_DC_CM_PDSV_CREATE                                       = 143, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_PDSV_CREATE")
        PNDV_DC_CM_PDSV_DELETE                                       = 144, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_PDSV_DELETE")
/*
145
..
150
*/
        PNDV_DC_IOCR_MAX_NUM_EXCEED                                  = 151, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("IOCR_MAX_NUM_EXCEED")
        PNDV_DC_I2C_SM                                               = 152, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("I2C_SM")
        PNDV_DC_CM_ADD_OPAQUE_DATA_REQ                               = 153, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_ADD_OPAQUE_DATA_REQ")
        PNDV_DC_CM_ADD_OPAQUE_DATA_DONE                              = 154, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_ADD_OPAQUE_DATA_DONE")
        PNDV_DC_CM_DELETE_ROSAL_REQ                                  = 155, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_DELETE_ROSAL_REQ")

        PNDV_DC_CHK_IND_PROPER_MODULE                                = 156, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CHK_IND_PROPER_MODULE")
        PNDV_DC_CHK_IND_PROPER_SUBMODULE                             = 157, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CHK_IND_PROPER_SUBMODULE")

        PNDV_DC_CM_SYNC_LED_INFO_IND                                 = 158, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_SYNC_LED_INFO_IND")

        PNDV_DC_PERI_WREQ_SDS_MODULE_DS245                           = 159, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("PERI_WREQ_SDS_MODULE_DS245")
        PNDV_DC_PERI_WRES_SDS_MODULE_DS245                           = 160, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("PERI_WRES_SDS_MODULE_DS245")
/*
161,
162
*/
        PNDV_DC_XS_DIFF_GOOD                                         = 163, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("XS_DIFF_GOOD")/* dword_nr, bitfield (32bit) */
        PNDV_DC_XS_DIFF_BAD                                          = 164, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("XS_DIFF_BAD")/* dword_nr, bitfield (32bit) */

        PNDV_DC_CM_DEV_LED_INFO_IND                                  = 165, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_DEV_LED_INFO_IND")/* diag_info | mcons_info<<8, maint_info */
        PNDV_DC_CM_LED_INFO_REQ                                      = 166, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_LED_INFO_REQ")/* diag_info | mcons_info<<8, maint_info */

        PNDV_DC_PERI_MODUL_OPERATE                                   = 167, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("PERI_MODUL_OPERATE")/* slot , server or don't server */

        PNDV_DC_CM_DIAL_REPEAT_REQ                                   = 168, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_DIAL_REPEAT_REQ")

/* 169 */

        PNDV_DC_CM_RTC3_IND                                          = 170, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_RTC3_IND")/* ar_idx */
        PNDV_DC_CM_RTC3_RES                                          = 171, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_RTC3_RES")/* ar_idx */
        PNDV_DC_CM_AR_DISCONNECT_IND_DONE                            = 172, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_AR_ABORT_IND_DONE")/* ar_idx */

        PNDV_DC_DS_PATCH_ISOM_PARA                                   = 173, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("DS_PATCH_ISOM_PARA")/* ds-nr */
        PNDV_DC_DS_REPATCH_ISOM_PARA                                 = 174, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("DS_REPATCH_ISOM_PARA")/* ds-nr */
        PNDV_DC_DS_QUIT_DS_XX30                                      = 175, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("DS_QUIT_DS_XX30")/* ds-nr */

        PNDV_DC_CM_PD_PRM_PREPARE                                    = 176, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_PD_PRM_PREPARE")/* prepare locale PDEV parameter setting */
        PNDV_DC_CM_PD_PRM_PREPARE_DONE                               = 177, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_PD_PRM_PREPARE_DONE")/* prepare done */
        PNDV_DC_CM_PD_PRM_WRITE                                      = 178, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_PD_PRM_WRITE")/* write PDEV-DS */
        PNDV_DC_CM_PD_PRM_WRITE_DONE                                 = 179, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_PD_PRM_WRITE_DONE")/* writing a PDEV record done */
        PNDV_DC_CM_PD_PRM_END                                        = 180, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_PD_PRM_END")/* no more PDEV records */
        PNDV_DC_CM_PD_PRM_END_DONE                                   = 181, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_PD_PRM_END_DONE")/* prm end confirmation */
        PNDV_DC_CM_PD_EVENT_APPL_READY_IND                           = 182, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_PD_EVENT_APPL_READY_IND")/* indication finished PDEV parameter setting */
        PNDV_DC_CM_PD_RES_PROV                                       = 183, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_PD_RES_PROV")/* event resources CM-PD */
        PNDV_DC_PERI_ISOM_UNVALID                                    = 184, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("PERI_ISOM_UNVALID")/* nr of comparison, para value */

        PNDV_DC_AL_PS_STATE                                          = 185, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("AL_PS_STATE")/* slot, mode */
        PNDV_DC_AL_ALL_MODUL_RUNNING                                 = 186, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("AL_ALL_MODUL_RUNNING")/* mode (YES der NO) */

        PNDV_DC_PERI_EXT_AR_ABORT_REQ                                = 187, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("PERI_EXT_AR_ABORT_REQ")/*not used -> PNDV_DC_COUPLE_EVENT */
        PNDV_DC_SM_AR_GLOB_CHANGE                                    = 188, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("SM_AR_GLOB_CHANGE")/* new SM state */

        PNDV_DC_CM_RMV_CHANNEL_DIAG_ALL_REQ_PNDV                     = 189, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_RMV_CHANNEL_DIAG_ALL_REQ_PNDV")/* slot, channel << 8 | error number */
        PNDV_DC_CM_MAINT                                             = 190, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_MAINT")/* maintenance event reported by CM */
/*
 * 191
 */
        PNDV_DC_CM_CONNECT_IND_1                                     = 192, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_CONNECT_IND_1")/* sm, ar_idx */
        PNDV_DC_CM_CONNECT_IND_2                                     = 193, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_CONNECT_IND_2")/* sm, ar_idx */
        PNDV_DC_CM_CONNECT_IND_3                                     = 194, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_CONNECT_IND_3")/* sm, ar_idx */
        PNDV_DC_CM_CONNECT_IND_4                                     = 195, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_CONNECT_IND_4")/* sm, ar_idx */

        PNDV_DC_CM_AP_ADD_DONE                                       = 196, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_AP_ADD_DONE")
        PNDV_DC_CM_AP_REMOVE                                         = 197, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_AP_REMOVE")
        PNDV_DC_CM_AP_REMOVE_DONE                                    = 198, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_AP_REMOVE_DONE")
        PNDV_DC_CM_NOT_USED_199                                      = 199, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_NOT_USED_199")

    // begin: used by PN / PN couplers
        PNDV_DC_CM_PROCESS_CHK_IND                                   = 200, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_PROCESS_CHK_IND")
        PNDV_DC_CM_SEND_NEW_PARTNER_CFG                              = 201, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_SEND_NEW_PARTNER_CFG")
        PNDV_DC_CM_RECV_NEW_PARTNER_CFG                              = 202, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_RECV_NEW_PARTNER_CFG")
        PNDV_DC_CM_AR_OFFLINE_PARTNER                                = 203, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_AR_OFFLINE_PARTNER")

        PNDV_DC_DIAL_STATE_FREE                                      = 210, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("DIAL_STATE_FREE")
        PNDV_DC_DIAL_STATE_SENT                                      = 211, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("DIAL_STATE_SENT")
        PNDV_DC_CHECK_MODUL_ERROR                                    = 212, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CHECK_MODUL_ERROR")
        PNDV_DC_CHECK_SYM_CFG                                        = 213, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CHECK_SYM_CFG")
    // end: used by PN / PN couplers

        PNDV_DC_CM_CONNECT_RSP_ASYNC_SPAETER                         = 220, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_CHK_RSP_ASYNC_SPAETER")
        PNDV_DC_CM_CHK_IND_AR_ABORT                                  = 221, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_CHK_IND_AR_ABORT")
        PNDV_DC_CM_EXCHANGE_PP_REQ                                   = 222, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_EXCHANGE_PP_REQ")
        PNDV_DC_CM_PRM_END_RSP                                       = 223, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_PRM_END_RSP")/* slot, ar_idx */
        PNDV_DC_SO_LOCKED                                            = 224, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("SO_LOCKED")/* event, last paramter: */
    /* 0xppppppti   p: still active flags 0..23  t_1: appear/disappear TRUE/FALSE t_0: new SO_Locked_State TRUE/FALSE i: ar_idx  */

        PNDV_DC_CM_ADD_GENERIC_DIAG_REQ                              = 225, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_ADD_GENERIC_DIAG_REQ")/**/
        PNDV_DC_CM_RMV_GENERIC_DIAG_REQ                              = 226, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("CM_RMV_GENERIC_DIAG_REQ")/**/

        PNDV_DC_NEW_OUT_START                                        = 230, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("NEW_OUT_START")
        PNDV_DC_NEW_OUT_END                                          = 231, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("NEW_OUT_END")
        PNDV_DC_NEW_IN_START                                         = 232, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("NEW_IN_START")
        PNDV_DC_NEW_IN_END                                           = 233, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("NEW_IN_END")

        PNDV_DC_EXT_CODE_BEGIN                                       = 240, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("EXT_CODE_BEGIN")
        PNDV_DC_EXT_OPERATE                                          = 241, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("EXT_OPERATE")/* 0xF1 = 0,xF0 | PERI_ENABLE   , slot */
        PNDV_DC_EXT_CLEAR                                            = 242, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("EXT_CLEAR")/* 0xF2 = 0,xF0 | PERI_DISABLE  , slot */
        PNDV_DC_EXT_CM_MAINT                                         = 243, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("EXT_CM_MAINT")/* Maint-LED control */
        PNDV_DC_EXT_CODE_END                                         = 250, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("EXT_CODE_END")

        PNDV_DC_FATAL                                                = 254, TRC_STRUCT_LINK("TRC_PNDV_STRUCT_3")  TRC_EXPLAIN("FATAL")
TRC_EVENT_LIST_CLOSE("TRC_PNDV_EVENT")
        PNDV_DC_NOT_USED                                             = 0xFF

}   PNDV_DEBUG_CODE_E_T;


#ifdef PNDV_TRACE_MSG
#define PNDV_TRACE_MSG_GET_DEBUG_STR(_p_deb_str, _deb_code, _detail_1, _detail_2, _detail_3)  \
{                                                        \
    char* _pstr = NULL;                                    \
    char* _pdst = (char*)_p_deb_str;                    \
    switch (_deb_code)                                     \
    {                                                   \
        case DUMMY                                              : _pstr = "PNDV DUMMY";                                                     break; \
        case PNDV_DC_ALL_OPEN                                   : _pstr = "PNDV ALL_OPEN %d";                                            break; \
        case PNDV_DC_ALL_START                                  : _pstr = "PNDV ALL_START %d";                                           break; \
        case PNDV_DC_ALL_STOP                                   : _pstr = "PNDV ALL_STOP %d";                                            break; \
        case PNDV_DC_CM_OPEN_CH                                 : _pstr = "PNDV CM_OPEN_CH Channel=%d";                                  break; \
        case PNDV_DC_CM_CLOSE_CH                                : _pstr = "PNDV CM_CLOSE_CH Channel=%d";                                 break; \
        case PNDV_DC_CM_RES_PROV                                : _pstr = "PNDV CM_RES_PROV %d";                                         break; \
        case PNDV_DC_CM_RES_BACK                                : _pstr = "PNDV CM_RES_BACK %d";                                         break; \
        case PNDV_DC_CM_LINK_STATUS_IND                         : _pstr = "PNDV CM_LINK_STATUS_IND Status=%d (%d) Mode|Speed|PortId=%p"; break; \
        case PNDV_DC_CM_SV_CREATE                               : _pstr = "PNDV CM_SV_CREATE %d";                                        break; \
        case PNDV_DC_CM_SV_DELETE                               : _pstr = "PNDV CM_SV_DELETE %d";                                        break; \
        case PNDV_DC_CM_DV_ADD                                  : _pstr = "PNDV CM_DV_ADD %d";                                           break; \
        case PNDV_DC_CM_DV_REMOVE                               : _pstr = "PNDV CM_DV_REMOVE %d";                                        break; \
        case PNDV_DC_CM_DV_AKT                                  : _pstr = "PNDV CM_DV_AKT DeviceControl=%d";                             break; \
        case PNDV_DC_CM_CONNECT_IND                             : _pstr = "PNDV CM_CONNECT_IND 0x%04X 0x%04X 0x%08X";                    break; \
        case PNDV_DC_PNDV_SM                                    : _pstr = "PNDV SM Event=%d State=%d (%d)";                              break; \
        case PNDV_DC_PNDV_AR_SM                                 : \
        {                                                        \
            char* _d1str;                                          \
            char* _d2str;                                          \
            char* _t1str = "PNDV AR_SM Event=";                    \
            char* _t2str = "[%d] State=";                        \
            char* _t3str = "[%d] ArIdx=%d";                     \
            PNDV_TRACE_MSG_AR_EVENT_STR(&_d1str, _detail_1);    \
            PNDV_TRACE_MSG_AR_STATE_STR(&_d2str, _detail_2);    \
            while (*_t1str) { *_pdst++ = *_t1str++; }            \
            while (*_d1str) { *_pdst++ = *_d1str++; }            \
            while (*_t2str) { *_pdst++ = *_t2str++; }            \
            while (*_d2str) { *_pdst++ = *_d2str++; }            \
            while (*_t3str) { *_pdst++ = *_t3str++; }            \
            break; \
        }\
        case PNDV_DC_PNDV_SR_SM                                 : _pstr = "PNDV SR_SM Event=%d State=%d SrSet=%d";                       break; \
        case PNDV_DC_PNDV_PP_SM                                 : \
        {                                                        \
            char* _d1str;                                          \
            char* _d2str;                                          \
            char* _t1str = "PNDV PP_SM Event=";                    \
            char* _t2str = "[%d] State=";                        \
            char* _t3str = "[%d] ResPtr=%p";                     \
            PNDV_TRACE_MSG_PP_EVENT_STR(&_d1str, _detail_1);    \
            PNDV_TRACE_MSG_PP_STATE_STR(&_d2str, _detail_2);    \
            while (*_t1str) { *_pdst++ = *_t1str++; }            \
            while (*_d1str) { *_pdst++ = *_d1str++; }            \
            while (*_t2str) { *_pdst++ = *_t2str++; }            \
            while (*_d2str) { *_pdst++ = *_d2str++; }            \
            while (*_t3str) { *_pdst++ = *_t3str++; }            \
            break; \
        }\
        case PNDV_DC_CM_ARSET_ABORT                             : _pstr = "PNDV CM_ARSET_ABORT ArSetNr=%d";                              break; \
        case PNDV_DC_CM_ARSET_TRIGGER                           : _pstr = "PNDV CM_ARSET_TRIGGER ArSetNr=%d";                            break; \
        case PNDV_DC_CM_AR_ABORT_DONE                           : _pstr = "PNDV CM_AR_ABORT_DONE ArIdx=%d";                              break; \
        case PNDV_DC_CM_AR_DISCONNECT_IND                       : _pstr = "PNDV CM_AR_DISCONNECT_IND Reason=%d (%d) ArIdx=%d";                         break; \
        case PNDV_DC_CM_AR_ABORT                                : _pstr = "PNDV CM_AR_ABORT ArIdx=%d (%0) CallLine=%d";                                break; \
        case PNDV_DC_CM_ADD_CHANNEL_DIAG_REQ                    : _pstr = "PNDV CM_ADD_CHANNEL_DIAG_REQ Entity=%d (%0) Channel|Error=%p";              break; \
        case PNDV_DC_CM_RMV_CHANNEL_DIAG_REQ                    : _pstr = "PNDV CM_RMV_CHANNEL_DIAG_REQ Entity=%d (%0) Channel|Error=%p";              break; \
        case PNDV_DC_CM_RMV_CHANNEL_DIAG_ALL_REQ                : _pstr = "PNDV CM_RMV_CHANNEL_DIAG_ALL_REQ Entity=%d (%0) Channel|Error=%p";          break; \
        case PNDV_DC_CM_ADD_CHANNEL_DIAG                        : _pstr = "PNDV CM_ADD_CHANNEL_DIAG Entity=%d (%0) Channel|Error=%p";                  break; \
        case PNDV_DC_CM_RMV_CHANNEL_DIAG                        : _pstr = "PNDV CM_RMV_CHANNEL_DIAG Entity=%d (%0) Channel|Error=%p";                  break; \
        case PNDV_DC_CM_ADD_CHANNEL_DIAG_CBF                    : _pstr = "PNDV CM_ADD_CHANNEL_DIAG_CBF Entity=%d";                                    break; \
        case PNDV_DC_CM_ADD_CHANNEL_DIAG_DONE_CBF               : _pstr = "PNDV CM_ADD_CHANNEL_DIAG_DONE_CBF Entity=%d";                               break; \
        case PNDV_DC_CM_RMV_CHANNEL_DIAG_CBF                    : _pstr = "PNDV CM_RMV_CHANNEL_DIAG_CBF Entity=%d";                                    break; \
        case PNDV_DC_CM_DIAG_ADD_DONE_ERR_ELEM                  : _pstr = "PNDV CM_DIAG_ADD_DONE_ERR_ELEM Slot=%d (%d) Subslot=%d";                    break; \
        case PNDV_DC_CM_DIAG_RMV_DONE_ERR_ELEM                  : _pstr = "PNDV CM_DIAG_RMV_DONE_ERR_ELEM Slot=%d (%d) Subslot=%d";                    break; \
        case PNDV_DC_SF_OFF                                     : _pstr = "PNDV SF_OFF %d";                                                            break; \
        case PNDV_DC_SF_ON                                      : _pstr = "PNDV SF_ON %d";                                                             break; \
        case PNDV_DC_CM_PRAL_REQ                                : _pstr = "PNDV CM_PRAL_REQ Entity=%d";                                                break; \
        case PNDV_DC_AR_OWNERSHIP_RESP                          : _pstr = "PNDV AR_OWNERSHIP_RESP CmpRes=%d SessionKey=0x%04X Flags=%p";                break; \
        case PNDV_DC_CM_UPAL_REQ                                : _pstr = "PNDV CM_UPAL_REQ Entity=%d";                                                break; \
        case PNDV_DC_CM_URAL_REQ                                : _pstr = "PNDV CM_URAL_REQ Entity=%d";                                                break; \
        case PNDV_DC_CM_STAL_REQ                                : _pstr = "PNDV CM_STAL_REQ Entity=%d";                                                break; \
        case PNDV_DC_CM_ADD_EXT_CHANNEL_DIAG_REQ                : _pstr = "PNDV CM_ADD_EXT_CHANNEL_DIAG_REQ Entity=%d (%0) Channel|Error=%p";          break; \
        case PNDV_DC_CM_RMV_EXT_CHANNEL_DIAG_REQ                : _pstr = "PNDV CM_RMV_EXT_CHANNEL_DIAG_REQ Entity=%d (%0) Channel|Error=%p";          break; \
        case PNDV_DC_CM_RMV_EXT_CHANNEL_DIAG_ALL_REQ            : _pstr = "PNDV CM_RMV_EXT_CHANNEL_DIAG_ALL_REQ Entity=%d (%0) Channel|Error=%p";      break; \
        case PNDV_DC_CM_RMV_EXT_CHANNEL_DIAG_ALL_REQ_PNDV       : _pstr = "PNDV CM_RMV_EXT_CHANNEL_DIAG_ALL_REQ_PNDV Entity=%d (%0) Channel|Error=%p"; break; \
        case PNDV_DC_CM_ADD_EXT_CHANNEL_DIAG                    : _pstr = "PNDV CM_ADD_EXT_CHANNEL_DIAG Entity=%d (%0) DiagTag=%p";                    break; \
        case PNDV_DC_CM_RMV_EXT_CHANNEL_DIAG                    : _pstr = "PNDV CM_RMV_EXT_CHANNEL_DIAG Entity=%d (%0) DiagTag=%p";                    break; \
        case PNDV_DC_CM_ADD_EXT_CHANNEL_DIAG_CBF                : _pstr = "PNDV CM_ADD_EXT_CHANNEL_DIAG_CBF Entity=%d";                                break; \
        case PNDV_DC_CM_ADD_EXT_CHANNEL_DIAG_DONE_CBF           : _pstr = "PNDV CM_ADD_EXT_CHANNEL_DIAG_DONE_CBF Entity=%d";                           break; \
        case PNDV_DC_CM_RMV_EXT_CHANNEL_DIAG_CBF                : _pstr = "PNDV CM_RMV_EXT_CHANNEL_DIAG_CBF Entity=%d";                                break; \
        case PNDV_DC_LINE_INFO                                  : _pstr = "PNDV LINE_INFO Mod=%d Line=%d";                            break; \
        case PNDV_DC_PERI_BUSAUSFALL                            : _pstr = "PNDV PERI_BUSAUSFALL";                                     break; \
        case PNDV_DC_PERI_BUSWIEDERKEHR                         : _pstr = "PNDV PERI_BUSWIEDERKEHR";                                  break; \
        case PNDV_DC_CM_AL_RES_PROV                             : _pstr = "PNDV CM_AL_RES_PROV %d";                                   break; \
        case PNDV_DC_CM_AL_RES_BACK                             : _pstr = "PNDV CM_AL_RES_BACK %d";                                   break; \
        case PNDV_DC_CM_AL_IND                                  : _pstr = "PNDV CM_AL_IND %d";                                        break; \
        case PNDV_DC_CM_AL_ACK_IND                              : _pstr = "PNDV CM_AL_ACK_IND %d";                                    break; \
        case PNDV_DC_CM_DIAL_SEND                               : _pstr = "PNDV CM_DIAL_SEND Slot=%d";                                break; \
        case PNDV_DC_CM_DIAL_ACK                                : _pstr = "PNDV CM_DIAL_ACK Entity|AlarmSequence=0x%04X Response=%p"; break; \
        case PNDV_DC_CM_PRAL_SEND                               : _pstr = "PNDV CM_PRAL_SEND Entity=%d";                              break; \
        case PNDV_DC_CM_PRAL_ACK                                : _pstr = "PNDV CM_PRAL_ACK Entity|AlarmSequence=0x%04X Response=%p"; break; \
        case PNDV_DC_CM_UPAL_SEND                               : _pstr = "PNDV CM_UPAL_SEND Entity=%d";                              break; \
        case PNDV_DC_CM_UPAL_ACK                                : _pstr = "PNDV CM_UPAL_ACK Entity|AlarmSequence=0x%04X Response=%p"; break; \
        case PNDV_DC_CM_URAL_SEND                               : _pstr = "PNDV CM_URAL_SEND Entity=%d";                              break; \
        case PNDV_DC_CM_URAL_ACK                                : _pstr = "PNDV CM_URAL_ACK Entity|AlarmSequence=0x%04X Response=%p"; break; \
        case PNDV_DC_CM_STAL_SEND                               : _pstr = "PNDV CM_STAL_SEND Entity=%d";                              break; \
        case PNDV_DC_CM_STAL_ACK                                : _pstr = "PNDV CM_STAL_ACK Entity|AlarmSequence=0x%04X Response=%p"; break; \
        case PNDV_DC_DS_WRITE                                   : _pstr = "PNDV DS_WRITE RecIndex=0x%04X%04X";                        break; \
        case PNDV_DC_DS_WRITE_DONE                              : _pstr = "PNDV DS_WRITE_DONE 0x%04X%04X";                            break; \
        case PNDV_DC_DS_READ_DONE                               : _pstr = "PNDV DS_READ_DONE Slot=%d (%d) Result=%p";                 break; \
        case PNDV_DC_PERI_DS_RW                                 : _pstr = "PNDV PERI_DS_RW Slot=%d Subslot=%d Index=%p";              break; \
        case PNDV_DC_PERI_DS_RW_DONE                            : _pstr = "PNDV PERI_DS_RW_DONE 0x%04X%04X";                          break; \
        case PNDV_DC_DFC_DS_REQ                                 : _pstr = "PNDV DFC_DS_REQ <<<not used>>>";                           break; \
        case PNDV_DC_DFC_DS_DONE                                : _pstr = "PNDV DFC_DS_DONE Slot=%d CmPnioErr=%p";                    break; \
        case PNDV_DC_COMMON_MODUL_INFO                          : _pstr = "PNDV COMMON_MODULE_INFO Slot=%d Subslot=%d SubModId=%p";   break; \
        case PNDV_DC_CM_UPAL_DONT_SEND                          : _pstr = "PNDV CM_UPAL_DONT_SEND Entity=%d";                         break; \
        case PNDV_DC_CM_URAL_DONT_SEND                          : _pstr = "PNDV CM_URAL_DONT_SEND Entity=%d";                         break; \
        case PNDV_DC_CM_STAL_DONT_SEND                          : _pstr = "PNDV CM_STAL_DONT_SEND Entity=%d";                         break; \
        case PNDV_DC_CM_PRAL_DONT_SEND                          : _pstr = "PNDV CM_PRAL_DONT_SEND Entity=%d";                         break; \
        case PNDV_DC_CM_ADD_GENERIC_DIAG                        : _pstr = "PNDV CM_ADD_GENERIC_DIAG USI=%d (%d) Slot=%d";             break; \
        case PNDV_DC_CM_ADD_GENERIC_DIAG_CBF                    : _pstr = "PNDV CM_ADD_GENERIC_DIAG_CBF USI=%d (%d) Channel=%d";      break; \
        case PNDV_DC_CM_ADD_GENERIC_DIAG_DONE_CBF               : _pstr = "PNDV CM_ADD_GENERIC_DIAG_DONE_CBF USI=%d";                 break; \
        case PNDV_DC_PERI_WRITE_EVENT_LOW                       : _pstr = "PNDV PERI_WRITE_EVENT_LOW Event=%d Add1=%d Add=%d";        break; \
        case PNDV_DC_PERI_WRITE_EVENT_HIGH                      : \
        {                                                        \
            char* _d1str;                                          \
            char* _t1str = "PNDV PERI_WRITE_EVENT_HIGH Event=";    \
            char* _t2str = "[%d] Add1=%d Add=%d";                \
            PNDV_TRACE_MSG_EVENTS_STR(&_d1str, _detail_1);        \
            while (*_t1str) { *_pdst++ = *_t1str++; }            \
            while (*_d1str) { *_pdst++ = *_d1str++; }            \
            while (*_t2str) { *_pdst++ = *_t2str++; }            \
            break; \
        }\
        case PNDV_DC_PERI_READ_EVENT_LOW                        : _pstr = "PNDV PERI_READ_EVENT_LOW Event=%d Add1=%d Add=%d";         break; \
        case PNDV_DC_PERI_READ_EVENT_HIGH                       : \
        {                                                        \
            char* _d1str;                                          \
            char* _t1str = "PNDV PERI_READ_EVENT_HIGH Event=";    \
            char* _t2str = "[%d] Add1=%d Add=%d";                \
            PNDV_TRACE_MSG_EVENTS_STR(&_d1str, _detail_1);        \
            while (*_t1str) { *_pdst++ = *_t1str++; }            \
            while (*_d1str) { *_pdst++ = *_d1str++; }            \
            while (*_t2str) { *_pdst++ = *_t2str++; }            \
            break; \
        }\
        case PNDV_DC_STORE_PARA_DS                              : _pstr = "PNDV STORE_PARA_DS";                                       break; \
        case PNDV_DC_STORE_PARA_DS_CHANGE                       : _pstr = "PNDV STORE_PARA_DS_CHANGE";                                break; \
        case PNDV_DC_GET_PARA_DS                                : _pstr = "PNDV GET_PARA_DS";                                         break; \
        case PNDV_DC_GET_NON_PARA_DS                            : _pstr = "PNDV GET_NON_PARA_DS";                                     break; \
        case PNDV_DC_DFC_CM_DS_RD_REQ                           : _pstr = "PNDV DFC_CM_DS_RD_REQ Slot=%d Subslot=%d Index=%p";        break; \
        case PNDV_DC_DFC_CM_DS_WR_REQ                           : _pstr = "PNDV DFC_CM_DS_WR_REQ Slot=%d Subslot=%d Index=%p";        break; \
        case PNDV_DC_DFC_CM_DS_RW_REQ                           : _pstr = "PNDV DFC_CM_DS_RW_REQ AR=%d DataLen=%d IP=%p";             break; \
        case PNDV_DC_CM_CONTROL_STATION_DONE                    : _pstr = "PNDV CM_CONTROL_STATION_DONE %d";                          break; \
        case PNDV_DC_CM_RMV_GENERIC_DIAG                        : _pstr = "PNDV CM_RMV_GENERIC_DIAG USI=%d (%d) Slot=%d";             break; \
        case PNDV_DC_CM_RMV_GENERIC_DIAG_CBF                    : _pstr = "PNDV CM_RMV_GENERIC_DIAG_CBF Tag=%d (%d) Interface=%d";    break; \
        case PNDV_DC_CM_AL_ACK                                  : _pstr = "PNDV CM_AL_ACK Type|Sequence=0x%04X";                      break; \
        case PNDV_DC_CM_CONNECT_RSP_SYNC                        : _pstr = "PNDV CM_CONNECT_RSP_SYNC State=%d";                        break; \
        case PNDV_DC_CM_CONNECT_RSP_ASYNC                       : _pstr = "PNDV CM_CONNECT_RSP_ASYNC State=%d";                       break; \
        case PNDV_DC_IO_ADD_MODULE_SPAETER                      : _pstr = "PNDV IO_ADD_MODULE_SPAETER";                               break; \
        case PNDV_DC_IO_ADD_SUBMODULE_SPAETER                   : _pstr = "PNDV IO_ADD_SUBMODULE_SPAETER";                            break; \
        case PNDV_DC_IO_REM_MODULE_SPAETER                      : _pstr = "PNDV IO_REM_MODULE_SPAETER";                               break; \
        case PNDV_DC_IO_REM_SUBMODULE_SPAETER                   : _pstr = "PNDV IO_REM_SUBMODULE_SPAETER";                            break; \
        case PNDV_DC_CM_DELETE_PP_REQ                           : _pstr = "PNDV CM_DELETE_PP_REQ";                                    break; \
        case PNDV_DC_CM_AP_ADD                                  : _pstr = "PNDV CM_AP_ADD";                                           break; \
        case PNDV_DC_CM_OVERLOAD_RES_PROV                       : _pstr = "PNDV CM_OVERLOAD_RES_PROV %d";                             break; \
        case PNDV_DC_CM_LINK_STATUS_RES_PROV                    : _pstr = "PNDV CM_LINK_STATUS_RES_PROV %d";                          break; \
        case PNDV_DC_CM_ROSAL_REQ                               : _pstr = "PNDV CM_ROSAL_REQ Slot=%d";                                break; \
        case PNDV_DC_CM_ROSAL_SEND                              : _pstr = "PNDV CM_ROSAL_SEND Entity=%d";                             break; \
        case PNDV_DC_CM_ROSAL_ACK                               : _pstr = "PNDV CM_ROSAL_ACK Entity|AlarmSequence=0x%04X";            break; \
        case PNDV_DC_CM_NOT_USED_130                            : _pstr = "PNDV CM_NOT_USED_130";                                     break; \
        case PNDV_DC_CM_PLUG_REQ_PNDV                           : _pstr = "PNDV CM_PLUG_REQ_PNDV";                                    break; \
        case PNDV_DC_CM_RMV_DIAG_CBF_ERR                        : _pstr = "PNDV CM_RMV_CHANNEL_DIAG_CBF_ERR Error|Entity=0x%04X";     break; \
        case PNDV_DC_CM_SLOT_NOT_READY                          : _pstr = "PNDV CM_SLOT_NOT_READY";                                   break; \
        case PNDV_DC_CM_NOT_USED_134                            : _pstr = "PNDV CM_NOT_USED_134";                                     break; \
        case PNDV_DC_CM_OPEN_CH_DONE                            : _pstr = "PNDV CM_OPEN_CH_DONE Handle=0x%04X";                       break; \
        case PNDV_DC_CM_CLOSE_CH_DONE                           : _pstr = "PNDV CM_CLOSE_CH_DONE Handle=0x%04X";                      break; \
        case PNDV_DC_START_DONE                                 : _pstr = "PNDV START_DONE %d";                                       break; \
        case PNDV_DC_STOP_DONE                                  : _pstr = "PNDV STOP_DONE %d";                                        break; \
        case PNDV_DC_CM_ADD_PACK_ERR_DIAG_REQ                   : _pstr = "PNDV CM_ADD_PACK_ERR_DIAG_REQ";                            break; \
        case PNDV_DC_CM_RMV_PACK_ERR_DIAG_REQ                   : _pstr = "PNDV CM_RMV_PACK_ERR_DIAG_REQ";                            break; \
        case PNDV_DC_CM_DEL_PACK_ERR_DIAG_REQ                   : _pstr = "PNDV CM_DEL_PACK_ERR_DIAG_REQ";                            break; \
        case PNDV_DC_CM_PDSV_CREATE                             : _pstr = "PNDV CM_PDSV_CREATE %d";                                   break; \
        case PNDV_DC_CM_PDSV_DELETE                             : _pstr = "PNDV CM_PDSV_DELETE %d";                                   break; \
        case PNDV_DC_I2C_SM                                     : _pstr = "PNDV I2C_SM";                                              break; \
        case PNDV_DC_CM_ADD_OPAQUE_DATA_REQ                     : _pstr = "PNDV CM_ADD_OPAQUE_DATA_REQ";                              break; \
        case PNDV_DC_CM_ADD_OPAQUE_DATA_DONE                    : _pstr = "PNDV CM_ADD_OPAQUE_DATA_DONE";                             break; \
        case PNDV_DC_CM_DELETE_ROSAL_REQ                        : _pstr = "PNDV CM_DELETE_ROSAL_REQ";                                 break; \
        case PNDV_DC_CHK_IND_PROPER_MODULE                      : _pstr = "PNDV CHK_IND_PROPER_MODULE";                               break; \
        case PNDV_DC_CHK_IND_PROPER_SUBMODULE                   : _pstr = "PNDV CHK_IND_PROPER_SUBMODULE";                            break; \
        case PNDV_DC_CM_SYNC_LED_INFO_IND                       : _pstr = "PNDV CM_SYNC_LED_INFO_IND LedInfo=0x%04X PortId=0x%04X";   break; \
        case PNDV_DC_PERI_WREQ_SDS_MODULE_DS245                 : _pstr = "PNDV PERI_WREQ_SDS_MODULE_DS245";                          break; \
        case PNDV_DC_PERI_WRES_SDS_MODULE_DS245                 : _pstr = "PNDV PERI_WRES_SDS_MODULE_DS245";                          break; \
        case PNDV_DC_XS_DIFF_GOOD                               : _pstr = "PNDV XS_DIFF_GOOD";                                        break; \
        case PNDV_DC_XS_DIFF_BAD                                : _pstr = "PNDV XS_DIFF_BAD";                                         break; \
        case PNDV_DC_CM_DEV_LED_INFO_IND                        : _pstr = "PNDV CM_DEV_LED_INFO_IND LedInfo=0x%04X MaintInfo=0x%04X"; break; \
        case PNDV_DC_CM_LED_INFO_REQ                            : _pstr = "PNDV CM_LED_INFO_REQ %d";                                  break; \
        case PNDV_DC_PERI_MODUL_OPERATE                         : _pstr = "PNDV PERI_MODUL_OPERATE";                                  break; \
        case PNDV_DC_CM_DIAL_REPEAT_REQ                         : _pstr = "PNDV CM_DIAL_REPEAT_REQ Entity=%d (%d) RetVal=%p";         break; \
        case PNDV_DC_CM_RTC3_IND                                : _pstr = "PNDV CM_RTC3_IND";                                         break; \
        case PNDV_DC_CM_RTC3_RES                                : _pstr = "PNDV CM_RTC3_RES";                                         break; \
        case PNDV_DC_CM_AR_DISCONNECT_IND_DONE                  : _pstr = "PNDV CM_AR_ABORT_IND_DONE ArIdx=%d";                       break; \
        case PNDV_DC_DS_PATCH_ISOM_PARA                         : _pstr = "PNDV DS_PATCH_ISOM_PARA";                                  break; \
        case PNDV_DC_DS_REPATCH_ISOM_PARA                       : _pstr = "PNDV DS_REPATCH_ISOM_PARA";                                break; \
        case PNDV_DC_DS_QUIT_DS_XX30                            : _pstr = "PNDV DS_QUIT_DS_XX30";                                     break; \
        case PNDV_DC_CM_PD_PRM_PREPARE                          : _pstr = "PNDV CM_PD_PRM_PREPARE %d";                                break; \
        case PNDV_DC_CM_PD_PRM_PREPARE_DONE                     : _pstr = "PNDV CM_PD_PRM_PREPARE_DONE CmpdPrmDefault=%d (%d) Reason=%p"; break; \
        case PNDV_DC_CM_PD_PRM_WRITE                            : _pstr = "PNDV CM_PD_PRM_WRITE (%d) (%d) RecordIndex=%p";            break; \
        case PNDV_DC_CM_PD_PRM_WRITE_DONE                       : _pstr = "PNDV CM_PD_PRM_WRITE_DONE %d";                             break; \
        case PNDV_DC_CM_PD_PRM_END                              : _pstr = "PNDV CM_PD_PRM_END %d";                                    break; \
        case PNDV_DC_CM_PD_PRM_END_DONE                         : _pstr = "PNDV CM_PD_PRM_END_DONE %d";                               break; \
        case PNDV_DC_CM_PD_EVENT_APPL_READY_IND                 : _pstr = "PNDV CM_PD_EVENT_APPL_READY_IND Response=%d";              break; \
        case PNDV_DC_CM_PD_RES_PROV                             : _pstr = "PNDV CM_PD_RES_PROV %d";                                   break; \
        case PNDV_DC_PERI_ISOM_UNVALID                          : _pstr = "PNDV PERI_ISOM_UNVALID";                                   break; \
        case PNDV_DC_AL_PS_STATE                                : _pstr = "PNDV AL_PS_STATE";                                         break; \
        case PNDV_DC_AL_ALL_MODUL_RUNNING                       : _pstr = "PNDV AL_ALL_MODUL_RUNNING";                                break; \
        case PNDV_DC_PERI_EXT_AR_ABORT_REQ                      : _pstr = "PNDV PERI_EXT_AR_ABORT_REQ";                               break; \
        case PNDV_DC_SM_AR_GLOB_CHANGE                          : _pstr = "PNDV SM_AR_GLOB_CHANGE";                                   break; \
        case PNDV_DC_CM_RMV_CHANNEL_DIAG_ALL_REQ_PNDV           : _pstr = "PNDV CM_RMV_CHANNEL_DIAG_ALL_REQ_PNDV Entity=%d (%0) Channel|Error=%p"; break; \
        case PNDV_DC_CM_MAINT                                   : _pstr = "PNDV CM_MAINT";                                     break; \
        case PNDV_DC_CM_CONNECT_IND_1                           : _pstr = "PNDV CM_CONNECT_IND_1";                             break; \
        case PNDV_DC_CM_CONNECT_IND_2                           : _pstr = "PNDV CM_CONNECT_IND_2";                             break; \
        case PNDV_DC_CM_CONNECT_IND_3                           : _pstr = "PNDV CM_CONNECT_IND_3 0x%04X %p";                   break; \
        case PNDV_DC_CM_CONNECT_IND_4                           : _pstr = "PNDV CM_CONNECT_IND_4";                             break; \
        case PNDV_DC_CM_AP_ADD_DONE                             : _pstr = "PNDV CM_AP_ADD_DONE";                               break; \
        case PNDV_DC_CM_AP_REMOVE                               : _pstr = "PNDV CM_AP_REMOVE";                                 break; \
        case PNDV_DC_CM_AP_REMOVE_DONE                          : _pstr = "PNDV CM_AP_REMOVE_DONE";                            break; \
        case PNDV_DC_CM_NOT_USED_199                            : _pstr = "PNDV CM_NOT_USED_199";                              break; \
        case PNDV_DC_CM_PROCESS_CHK_IND                         : _pstr = "PNDV CM_PROCESS_CHK_IND";                           break; \
        case PNDV_DC_CM_SEND_NEW_PARTNER_CFG                    : _pstr = "PNDV CM_SEND_NEW_PARTNER_CFG";                      break; \
        case PNDV_DC_CM_RECV_NEW_PARTNER_CFG                    : _pstr = "PNDV CM_RECV_NEW_PARTNER_CFG";                      break; \
        case PNDV_DC_CM_AR_OFFLINE_PARTNER                      : _pstr = "PNDV CM_AR_OFFLINE_PARTNER";                        break; \
        case PNDV_DC_DIAL_STATE_FREE                            : _pstr = "PNDV DIAL_STATE_FREE";                              break; \
        case PNDV_DC_DIAL_STATE_SENT                            : _pstr = "PNDV DIAL_STATE_SENT";                              break; \
        case PNDV_DC_CHECK_MODUL_ERROR                          : _pstr = "PNDV CHECK_MODUL_ERROR";                            break; \
        case PNDV_DC_CHECK_SYM_CFG                              : _pstr = "PNDV CHECK_SYM_CFG";                                break; \
        case PNDV_DC_CM_CONNECT_RSP_ASYNC_SPAETER               : _pstr = "PNDV CM_CHK_RSP_ASYNC_SPAETER";                     break; \
        case PNDV_DC_CM_CHK_IND_AR_ABORT                        : _pstr = "PNDV CM_CHK_IND_AR_ABORT Line=%d (%d) Info=%p";     break; \
        case PNDV_DC_CM_EXCHANGE_PP_REQ                         : _pstr = "PNDV CM_EXCHANGE_PP_REQ";                           break; \
        case PNDV_DC_CM_PRM_END_RSP                             : _pstr = "PNDV CM_PRM_END_RSP";                               break; \
        case PNDV_DC_SO_LOCKED                                  : _pstr = "PNDV SO_LOCKED Event=0x%04d Info=%p";               break; \
        case PNDV_DC_CM_ADD_GENERIC_DIAG_REQ                    : _pstr = "PNDV CM_ADD_GENERIC_DIAG_REQ Interface=%d (%d) %d"; break; \
        case PNDV_DC_CM_RMV_GENERIC_DIAG_REQ                    : _pstr = "PNDV CM_RMV_GENERIC_DIAG_REQ Interface=%d (%d) %d"; break; \
        case PNDV_DC_NEW_OUT_START                              : _pstr = "PNDV NEW_OUT_START";                                break; \
        case PNDV_DC_NEW_OUT_END                                : _pstr = "PNDV NEW_OUT_END";                                  break; \
        case PNDV_DC_NEW_IN_START                               : _pstr = "PNDV NEW_IN_START";                                 break; \
        case PNDV_DC_NEW_IN_END                                 : _pstr = "PNDV NEW_IN_END";                                   break; \
        case PNDV_DC_EXT_CODE_BEGIN                             : _pstr = "PNDV EXT_CODE_BEGIN";                               break; \
        case PNDV_DC_EXT_OPERATE                                : _pstr = "PNDV EXT_OPERATE";                                  break; \
        case PNDV_DC_EXT_CLEAR                                  : _pstr = "PNDV EXT_CLEAR";                                    break; \
        case PNDV_DC_EXT_CM_MAINT                               : _pstr = "PNDV EXT_CM_MAINT";                                 break; \
        case PNDV_DC_EXT_CODE_END                               : _pstr = "PNDV EXT_CODE_END";                                 break; \
        case PNDV_DC_FATAL                                      : _pstr = "PNDV FATAL";                                        break; \
        case PNDV_DC_NOT_USED                                   : _pstr = "PNDV NOT_USED";                                     break; \
        default                                                 : _pstr = "PNDV UNDEFINED";                                    break; \
    }                                                  \
    while (_pstr && *_pstr) { *_pdst++ = *_pstr++; }; \
    *_pdst = 0;                                       \
}
#endif /* PNDV_TRACE_MSG */


#pragma pack(1)
typedef struct PNDV_STRUC_DEBUG_BUFFER_ITEM_S
{
    Unsigned8                           debug_code;
    Unsigned8                           fill;
    Unsigned16                          detail_1;
    Unsigned16                          detail_2;
    Unsigned32                          detail_3;

}   PNDV_STRUC_DEBUG_BUFFER_ITEM_T;
#pragma pack()

#define PNDV_INT_DEBUG_BUFFER_ITEM_PTR   PNDV_STRUC_DEBUG_BUFFER_ITEM_T *

typedef struct PNDV_STRUC_DEBUG_BUFFER_S
{
    PNDV_TRC_STRUC( PNDV_STRUC_DEBUG_BUFFER_ITEM_T, PNDV_CFG_DEBUG_ELEMENT_NUMBER)/* no ";" here! */
}   PNDV_STRUC_DEBUG_BUFFER_T;


typedef struct PNDV_TRIGGER_S
{
    UnsignedOpt     read_record_ind;
    UnsignedOpt     read_record_rsp;
    UnsignedOpt     write_record_ind;
    UnsignedOpt     write_record_rsp;

    UnsignedOpt     al_send;
    UnsignedOpt     al_send_done;
    UnsignedOpt     al_ack;

}   PNDV_TRIGGER_T;



typedef enum PNDV_CHANNEL_DIAG_REQ_E
{
    PNDV_CHANNEL_DIAG_ADD_REQ,
    PNDV_CHANNEL_DIAG_RMV_REQ,
    PNDV_CHANNEL_DIAG_RMV_ALL_REQ,
    PNDV_CHANNEL_DIAG_RMV_ALL_REQ_PNDV,
    PNDV_EXT_CHANNEL_DIAG_ADD_REQ,
    PNDV_EXT_CHANNEL_DIAG_RMV_REQ,
    PNDV_EXT_CHANNEL_DIAG_RMV_ALL_REQ,
    PNDV_EXT_CHANNEL_DIAG_RMV_ALL_REQ_PNDV,
    PNDV_GENERIC_DIAG_ADD_REQ,
    PNDV_GENERIC_DIAG_RMV_REQ,

    PNDV_DIAG_REQ_NISCHT            = 0xFF

}   PNDV_CHANNEL_DIAG_REQ_T;


typedef struct PNDV_CHAN_DIAG_S
{
    PNDV_LIST_ENTRY_TYPE    link;

    PNDV_CHANNEL_DIAG_REQ_T req;
    PNDV_DIAG_CH_STATE_T    ch_diag_sm;
    PNDV_AL_USI_T           usi;
    Unsigned16              entity_nr;
    Unsigned16              ist_slot;
    Unsigned16              slot;
    Unsigned16              subslot;

    union
    {
        /* channel diagnosis */
        struct
        {
            Unsigned16      kanal;
            Unsigned16      fehlernummer;
            Unsigned16      properties;
            Unsigned16      peri_index;
            Unsigned8       quit_to_peri;

        } ch;

        /* ext channel diagnosis */
        struct
        {
            Unsigned16      kanal;
            Unsigned16      fehlernummer;
            Unsigned16      properties;
            Unsigned16      peri_index;
            Unsigned8       quit_to_peri;
            Unsigned16      error_type;
            Unsigned32      add_value;
        } xch ;

    } as;

}   PNDV_CHAN_DIAG_T;


typedef PNDV_CHAN_DIAG_T * PNDV_CHAN_DIAG_PTR;

/* pull / plug */

typedef struct PNDV_AL_PP_S
{
    PNDV_LIST_ENTRY_TYPE   link;

    PNDV_AL_PP_REQ         req;
    Unsigned16             slot_or_ar_idx;
    Unsigned16             ist_slot;
    UnsignedOpt            clear_dial_if_pulled;

}   PNDV_AL_PP_T;


typedef PNDV_AL_PP_T * PNDV_AL_PP_TYPE_PTR;


typedef struct PNDV_DIAG_DATA_S
{
    /* only one add(Sub)Module, rmv(Sub)Module, addDiag, rmvDiag at same time */
    UnsignedOpt                     cm_diag_req_running;
    Boolean                         cm_dev_req_ignore_done;   /* ignore Modul-Req-done from CM (meanwhile the peri bus has a break down) */
    Unsigned16                      cm_dev_req_running;

    struct
    {
        Unsigned16                  pending;

        struct
        {
            Unsigned32              number;

        } add_api;

        struct
        {
            Unsigned32              number;

        } rmv_api;

        struct
        {
            Unsigned16              soll_slot;
            Unsigned16              ist_slot;
            PNDV_AL_PP_REQ          pp_src;

        } add_mod;

        struct
        {
            Unsigned16              soll_slot;
            Unsigned16              ist_slot;
            PNDV_AL_PP_REQ          pp_src;

        } rmv_mod;

        struct
        {
            Unsigned16              soll_slot;
            Unsigned16              ist_slot;

        } add_sub;

        struct
        {
            Unsigned16              soll_slot;
            Unsigned16              ist_slot;

        } rmv_sub;

    }cm_dev_req;

    PNDV_LIST_ENTRY_TYPE              channel_diag_free_list;
    PNDV_LIST_ENTRY_TYPE              channel_diag_new_list;

    PNDV_CHAN_DIAG_PTR              channel_diag_new_list_bookmark;

    PNDV_CHAN_DIAG_T                channel_diag[PNDV_DIAG_CHANNEL_ANZ];
    Boolean                         dial_repeat_req[PNDV_MAX_SV_ENTITY];

    Unsigned8                       modul_err[PNDV_MAX_SV_ENTITY];
    Unsigned16                      submodul_rmv_done_err_count;

    UnsignedOpt                     wait_for_ch_diag_quit_cnt[PNDV_MAX_SV_ENTITY];
    UnsignedOpt                     wait_for_xch_diag_quit_cnt[PNDV_MAX_SV_ENTITY];

    UnsignedOpt                     ch_diag_to_cm_count[PNDV_MAX_SV_ENTITY];

}   PNDV_DIAG_DATA_T;

typedef enum PNDV_CFG_SUBMODULE_RES_STATE_E
{
    TRC_SUBLABEL_LIST_OPEN("TRC_PNDV_PP_SM_STATE")
    PNDV_CFG_SUBMODULE_RES_STATE_UNKOWN         = 0, TRC_EXPLAIN("RES_STATE_UNKOWN")    /**< resource is not initialized yet*/
    PNDV_CFG_SUBMODULE_RES_STATE_FREE           = 1, TRC_EXPLAIN("RES_STATE_FREE")      /**< the resource is free to be used and not bound to a submodule*/
    PNDV_CFG_SUBMODULE_RES_STATE_W_ADD          = 2, TRC_EXPLAIN("RES_STATE_W_ADD")     /**< the resource is bound, a plug for the bound submodule is supposed to be done next*/
//    PNDV_CFG_SUBMODULE_RES_STATE_ADD_RUN        = 3, TRC_EXPLAIN("RES_STATE_ADD_RUN")   /**< the resource is bound, a plug for the bound submodule is on the way to cm*/
    PNDV_CFG_SUBMODULE_RES_STATE_W_REM          = 4, TRC_EXPLAIN("RES_STATE_W_REM")     /**< the resource is bound, a pull for the bound submodule is supposed to be done next*/
    PNDV_CFG_SUBMODULE_RES_STATE_REM_RUN        = 5, TRC_EXPLAIN("RES_STATE_REM_RUN")   /**< the resource is bound, a pull for the bound submodule is on the way to cm*/

    PNDV_CFG_SUBMODULE_RES_STATE_W_OWN          = 6, TRC_EXPLAIN("RES_STATE_W_OWN")     /**< the resource is bound, the bound submodule is waiting for the ownership indication*/
    PNDV_CFG_SUBMODULE_RES_STATE_W_OWN_PASSIV   = 7, TRC_EXPLAIN("RES_STATE_W_OWN_PASSIV")/**< the resource is bound, the bound submodule is waiting for taking ownership from a partner r1 ar*/
    PNDV_CFG_SUBMODULE_RES_STATE_W_PRM_END      = 8, TRC_EXPLAIN("RES_STATE_W_PRM_END") /**< the resource is bound, the bound submodule is waiting for the end of the parameter phase*/
    PNDV_CFG_SUBMODULE_RES_STATE_W_IN_DATA      = 9, TRC_EXPLAIN("RES_STATE_W_IN_DATA") /**< the resource is bound, the bound submodule is waiting for the in_data indication*/
    PNDV_CFG_SUBMODULE_RES_STATE_W_IN_DATA_DONE = 10, TRC_EXPLAIN("RES_STATE_W_IN_DATA_DONE") /**< the resource is bound, the bound submodule is waiting for the in_data confirmation from peri*/

    PNDV_CFG_SUBMODULE_RES_STATE_IN_DATA        = 11, TRC_EXPLAIN("RES_STATE_IN_DATA")  /**< the resource is bound, the bound submodule is fully working*/
    TRC_SUBLABEL_LIST_CLOSE("TRC_PNDV_PP_SM_STATE")
    PNDV_CFG_SUBMODULE_RES_STATE_NOT_USED       = 255

}PNDV_CFG_SUBMODULE_RES_STATE_T;

#ifdef PNDV_TRACE_MSG
#define PNDV_TRACE_MSG_PP_STATE_STR(_p_deb_str, _deb_code)    \
{                                                            \
    char* _pstr3;                                            \
    switch (_deb_code)                                         \
    {                                                       \
        case PNDV_CFG_SUBMODULE_RES_STATE_UNKOWN         : _pstr3 = "UNKOWN";         break; \
        case PNDV_CFG_SUBMODULE_RES_STATE_FREE           : _pstr3 = "FREE";         break; \
        case PNDV_CFG_SUBMODULE_RES_STATE_W_ADD          : _pstr3 = "W_ADD";         break; \
        case PNDV_CFG_SUBMODULE_RES_STATE_W_REM          : _pstr3 = "W_REM";         break; \
        case PNDV_CFG_SUBMODULE_RES_STATE_REM_RUN        : _pstr3 = "REM_RUN";         break; \
        case PNDV_CFG_SUBMODULE_RES_STATE_W_OWN          : _pstr3 = "W_OWN";         break; \
        case PNDV_CFG_SUBMODULE_RES_STATE_W_PRM_END      : _pstr3 = "W_PRM_END";    break; \
        case PNDV_CFG_SUBMODULE_RES_STATE_W_IN_DATA      : _pstr3 = "W_IN_DATA";     break; \
        case PNDV_CFG_SUBMODULE_RES_STATE_W_IN_DATA_DONE : _pstr3 = "W_IN_DATA";     break; \
        case PNDV_CFG_SUBMODULE_RES_STATE_IN_DATA        : _pstr3 = "IN_DATA";         break; \
        case PNDV_CFG_SUBMODULE_RES_STATE_NOT_USED       : _pstr3 = "NOT USED";        break; \
        default                                          : _pstr3 = "UNDEFINED";    break; \
    }                        \
    *_p_deb_str = _pstr3;     \
}
#endif /* PNDV_TRACE_MSG */

typedef enum PNDV_CFG_SUBMODULE_RES_EVENT_E
{
    TRC_SUBLABEL_LIST_OPEN("TRC_PNDV_PP_SM_EVENT")
    PNDV_CFG_SUBMODULE_RES_EVENT_PERI_ADD_REQ           =  0, TRC_EXPLAIN("RES_EVENT_PERI_ADD_REQ")         /**< peri requested to add a submodule, this binds the resource to the submodule and puts it to add_wait list*/
    PNDV_CFG_SUBMODULE_RES_EVENT_ADD_REQ                =  1, TRC_EXPLAIN("RES_EVENT_ADD_REQ")              /**< indicates that the add req was send to cm, this puts the resource in the add_run list*/
    PNDV_CFG_SUBMODULE_RES_EVENT_ADD_CM_DONE            =  2, TRC_EXPLAIN("RES_EVENT_ADD_CM_DONE")          /**< add confirmation was received from cm, this puts the resource in the in_use list*/
    PNDV_CFG_SUBMODULE_RES_EVENT_CM_OWN_IND             =  3, TRC_EXPLAIN("RES_EVENT_CM_OWN_IND")           /**< an ownership indication for this submodule was received from cm*/
    PNDV_CFG_SUBMODULE_RES_EVENT_CM_PRM_END_IND         =  4, TRC_EXPLAIN("RES_EVENT_CM_PRM_END_IND")       /**< a prm_end indication for this submodule was received from cm*/
    PNDV_CFG_SUBMODULE_RES_EVENT_CM_IN_DATA_IND         =  5, TRC_EXPLAIN("RES_EVENT_CM_IN_DATA_IND")       /**< a in_data indication for this submodule was received from cm*/
    PNDV_CFG_SUBMODULE_RES_EVENT_PERI_IN_DATA_IND_DONE  =  6, TRC_EXPLAIN("RES_EVENT_CM_IN_DATA_IND_DONE")  /**< a in_data confirmation for this submodule was received from peri */
    PNDV_CFG_SUBMODULE_RES_EVENT_PERI_REM_REQ           =  7, TRC_EXPLAIN("RES_EVENT_PERI_REM_REQ")         /**< peri requested to remove a submodule, this puts the resource in the rem_wait list*/
    PNDV_CFG_SUBMODULE_RES_EVENT_REM_REQ                =  8, TRC_EXPLAIN("RES_EVENT_REM_REQ")              /**< indicates that the rem req was send to cm, this puts the resource in the rem_run list*/
    PNDV_CFG_SUBMODULE_RES_EVENT_REM_CM_DONE            =  9, TRC_EXPLAIN("RES_EVENT_REM_CM_DONE")          /**< rem confirmation was received from cm, this puts the resource in the free list and unbinds it from a submodule*/
    PNDV_CFG_SUBMODULE_RES_EVENT_RELEASE                = 10, TRC_EXPLAIN("RES_EVENT_RELEASE")              /**< owner ar has been disconnected, submodule is released from ownership*/
    PNDV_CFG_SUBMODULE_RES_EVENT_CHANGE_OWNER           = 11, TRC_EXPLAIN("RES_EVENT_CHANGE_OWNER")         /**< change the owner of a used module (only allowed for system redundancy) */
    PNDV_CFG_SUBMODULE_RES_EVENT_R1_PASSIV_OWNER        = 12, TRC_EXPLAIN("RES_EVENT_R1_PASSIV_OWNER")      /**< prepare a submodule in a R1 backup ar to take ownership later */
    TRC_SUBLABEL_LIST_CLOSE("TRC_PNDV_PP_SM_EVENT")
    PNDV_CFG_SUBMODULE_RES_EVENT_NOT_USED               = 255
}PNDV_CFG_SUBMODULE_RES_EVENT_T;

#ifdef PNDV_TRACE_MSG
#define PNDV_TRACE_MSG_PP_EVENT_STR(_p_deb_str, _deb_code)    \
{                                                            \
    char* _pstr4;                                            \
    switch (_deb_code)                                         \
    {                                                       \
        case PNDV_CFG_SUBMODULE_RES_EVENT_PERI_ADD_REQ          : _pstr4 = "PERI_ADD_REQ";             break; \
        case PNDV_CFG_SUBMODULE_RES_EVENT_ADD_REQ               : _pstr4 = "ADD_REQ";                 break; \
        case PNDV_CFG_SUBMODULE_RES_EVENT_ADD_CM_DONE           : _pstr4 = "ADD_CM_DONE";             break; \
        case PNDV_CFG_SUBMODULE_RES_EVENT_CM_OWN_IND            : _pstr4 = "CM_OWN_IND";             break; \
        case PNDV_CFG_SUBMODULE_RES_EVENT_CM_PRM_END_IND        : _pstr4 = "CM_PRM_END_IND";         break; \
        case PNDV_CFG_SUBMODULE_RES_EVENT_CM_IN_DATA_IND        : _pstr4 = "CM_IN_DATA_IND";         break; \
        case PNDV_CFG_SUBMODULE_RES_EVENT_PERI_IN_DATA_IND_DONE : _pstr4 = "CM_IN_DATA_IND_DONE";     break; \
        case PNDV_CFG_SUBMODULE_RES_EVENT_PERI_REM_REQ          : _pstr4 = "PERI_REM_REQ";             break; \
        case PNDV_CFG_SUBMODULE_RES_EVENT_REM_REQ               : _pstr4 = "REM_REQ";                 break; \
        case PNDV_CFG_SUBMODULE_RES_EVENT_REM_CM_DONE           : _pstr4 = "REM_CM_DONE";             break; \
        case PNDV_CFG_SUBMODULE_RES_EVENT_RELEASE               : _pstr4 = "RELEASE";                 break; \
        case PNDV_CFG_SUBMODULE_RES_EVENT_CHANGE_OWNER          : _pstr4 = "CHANGE_OWNER";             break; \
        case PNDV_CFG_SUBMODULE_RES_EVENT_NOT_USED              : _pstr4 = "NOT USED";                 break; \
        default                                                 : _pstr4 = "UNDEFINED";             break; \
    }                        \
    *_p_deb_str = _pstr4;     \
}
#endif /* PNDV_TRACE_MSG */

/*****************************************************************************/
/* internal state machine defines */

typedef enum PNDV_SM_PERIBUS_STATE_E
{
    PNDV_SM_PERIBUS_STATE_NOT_USED          = 0,    /**< initial state if sm is not used */
    PNDV_SM_PERIBUS_STATE_UNKNOWN           = 1,    /**< initial state if sm is used, peri-bus state is not known yet */
    PNDV_SM_PERIBUS_STATE_OK                = 2,    /**< this is the "good" state at normal work */
    PNDV_SM_PERIBUS_STATE_NOTOK             = 3     /**< this is the "bad" state at normal work */

}PNDV_SM_PERIBUS_STATE;

typedef enum PNDV_SM_PERIBUS_EVENT_E
{
    PNDV_SM_PERIBUS_EVENT_OK                   = 0,    /* signal from peri-bus to indicate "good" bus sate */
    PNDV_SM_PERIBUS_EVENT_BUSY                 = 1,    /* signal from peri-bus to indicate that ther is still something to do */
    PNDV_SM_PERIBUS_EVENT_ERROR                = 2,    /* signal from peri-bus to indicate "good" bus sate */
    PNDV_SM_PERIBUS_EVENT_ERROR_DELAY_TIMEOUT  = 3     /* signal from timer to switch error state delayed (only at startup) */

}PNDV_SM_PERIBUS_EVENT;

typedef struct PNDV_STRUCT_CFG_SUBMODULE_RES_S
{
    PNDV_LIST_ENTRY_TYPE              link;              //!< link header to make the element linkable to lists
    Unsigned32                        my_field_index;    //!< index of this element within the array, maybe usefull

    CM_SV_SUBMODULE_ADD_ELEMENT_TYPE  sub_module;        //!< information needed to add this submodule to cm

    Boolean                           para_error;        //!< this module has a parameter error
    Unsigned32                        own_ar_idx;        //!< ar which currently owns this submodule
    Unsigned16                        ar_set_nr;         //!< ar set number of the ar which currently owns this submodule (sysred)

    Unsigned16                        entity;            //!< the entity is filled in with the submodule add by peri, it is an index to address the submodule inside the interface
    PNDV_SUBMODUL_PLUG_STATE_T        plug_cm;           //!< indicates the status of a submodule regarding cm, information also included in res_state
    PNDV_CFG_SUBMODULE_RES_STATE_T    res_state;         //!< indicates in which state this submodule (and this resource) is in
    PNDV_SUBMODULE_STATE_T            submodule_state;   //!< main state of a submodule

}   PNDV_STRUCT_CFG_SUBMODULE_RES_T;

typedef PNDV_STRUCT_CFG_SUBMODULE_RES_T * PNDV_STRUCT_CFG_SUBMODULE_RES_PTR_T;

#define PNDV_CFG_UNBOUND_SUB 0xFFFF
#define PNDV_CFG_UNUSED_ENTITY 0xFFFF

typedef struct PNDV_PERI_CFG_S
{

        PNDV_LIST_ENTRY_TYPE                free_list;        //!< list with all free submodule resources
        PNDV_LIST_ENTRY_TYPE                in_use_list;      //!< list with all submodules currently added to cm
        PNDV_LIST_ENTRY_TYPE                cm_add_list;      //!< list with all submodules currently on the way to be added to cm
        PNDV_LIST_ENTRY_TYPE                cm_add_rem_wait_list; //!< list with all submodules currently waiting to be added or removed from cm
        PNDV_LIST_ENTRY_TYPE                cm_rem_list;      //!< list with all submodules currently on the way to be removed from cm

    PNDV_STRUCT_CFG_SUBMODULE_RES_T   sub_res[PNDV_MAX_SV_ENTITY]; //!< PNDV_MAX_SV_ENTITY sind die maximal m�glichen Slot/Subslot P�rchen

    /**
     * @brief fast find index array and other informations
     *
     * array to quickly find the index of a slot/subslot combo in .data
     * first index is used for slot, second index is used for subslot.
     * If a resources is taken out of the free_list, slot and subslot of the
     * submodule which the resource is used for are used to address an element
     * of the ff_index_array. This element is filled with the index of the
     * resources in the data array.
     * If the resource is put back to the free_list, this element has to be cleared.
     *
     */
    Unsigned16           ff_index_array[PNDV_MAX_SV_ENTITY][PNDV_MAX_SV_ENTITY];


    /** following values are determined during pndv_open */
    Unsigned32                        first_peri_sub_idx;   //!< idx of the first real peri subslot
    Unsigned32                        im_idx_sub_1;         //!< idx of subslot 1 of im
    Unsigned32                        im_idx_sub_iface;     //!< idx of pdev interace subslot
    Unsigned32                        im_idx_sub_port1;     //!< idx of pdev port 1 subslot
    Unsigned32                        im_idx_sub_port2;     //!< idx of pdev port 2 subslot

    PNDV_SM_PERIBUS_STATE             sm_state;
} PNDV_PERI_CFG_T;


typedef struct PNDV_ENTITY_ADMIN_ELEM_S
{
    PNDV_LIST_ENTRY_TYPE              link;
    Unsigned16                        slot;
    Unsigned16                        sub_slot;
    UnsignedOpt                       ar_idx;
    UnsignedOpt                       entity_idx;
}PNDV_ENTITY_ADMIN_ELEM_T;

typedef PNDV_ENTITY_ADMIN_ELEM_T * PNDV_ENTITY_ADMIN_ELEM_PTR_T;

typedef struct PNDV_ENTITY_ADMIN_S
{
    PNDV_LIST_ENTRY_TYPE                free_list;                      // free elements for this ar
    PNDV_LIST_ENTRY_TYPE                in_use_list;                    // occupied elements for this ar
    Unsigned32                          in_use_count;                   // count of occupied elements
    PNDV_ENTITY_ADMIN_ELEM_T            entities[PNDV_MAX_SV_ENTITY];   // one element for ever possible entity (max sub_slots per ar)
} PNDV_ENTITY_ADMIN_T;

typedef enum PNDV_PERI_SERVICE_IDENT_E
{
    PNDV_PERI_SERVICE_IDENT_CON     = 1,
    PNDV_PERI_SERVICE_IDENT_OWN     = 2,
    PNDV_PERI_SERVICE_IDENT_PRM     = 3,
    PNDV_PERI_SERVICE_IDENT_IDA     = 4

}PNDV_PERI_SERVICE_IDENT_T;

typedef struct PNDV_PERI_SERVICE_ELEMENT_S
{
        PNDV_LIST_ENTRY_TYPE              link;

    CM_UPPER_RQB_PTR_TYPE             rqb_ptr;
}   PNDV_PERI_SERVICE_ELEMENT_T;

typedef PNDV_PERI_SERVICE_ELEMENT_T * PNDV_PERI_SERVICE_ELEMENT_PTR_T;

typedef struct PNDV_PERI_SERVICE_QUEUES_S
{

        PNDV_LIST_ENTRY_TYPE              free_list;
        PNDV_LIST_ENTRY_TYPE              con_wait_list;    //! list with waiting connect.ind
        PNDV_LIST_ENTRY_TYPE              own_wait_list;    //! list with waiting ownership.ind
        PNDV_LIST_ENTRY_TYPE              prm_wait_list;    //! list with waiting prm_end.ind
        PNDV_LIST_ENTRY_TYPE              ida_wait_list;    //! list with waiting in_data.ind

    Boolean                           con_serv_run;     //! if TRUE, service is in use, queue all following indications
    Boolean                           own_serv_run;     //! if TRUE, service is in use, queue all following indications
    Boolean                           prm_serv_run;     //! if TRUE, service is in use, queue all following indications
    Boolean                           ida_serv_run;      //! if TRUE, service is in use, queue all following indications

    PNDV_PERI_SERVICE_ELEMENT_T       elem[PNDV_MAX_SV_ENTITY];

}PNDV_PERI_SERVICE_QUEUES_T;

typedef struct PNDV_PERI_RQB_QUEUE_S
{
    PNDV_LIST_ENTRY_TYPE              free_list;                  //! free list elements queue
    PNDV_LIST_ENTRY_TYPE              in_use;                     //! list with waiting list elements

    PNDV_PERI_SERVICE_ELEMENT_T       elem[PNDV_MAX_SV_ENTITY]; //! a bunch of list elements, one record request per subslot ist possible at a time
}PNDV_PERI_RQB_QUEUE_T;

typedef struct PNDV_PERI_ALARM_ELEMENT_S
{
        PNDV_LIST_ENTRY_TYPE              link;

        Unsigned16                        entity_nr;
}   PNDV_PERI_ALARM_ELEMENT_T;

typedef PNDV_PERI_ALARM_ELEMENT_T * PNDV_PERI_ALARM_ELEMENT_PTR_T;

typedef struct PNDV_PERI_ALARM_QUEUE_S
{
    PNDV_LIST_ENTRY_TYPE              free_list;                  //! free list elements queue
    PNDV_LIST_ENTRY_TYPE              in_use;                     //! list with waiting list elements

    PNDV_PERI_ALARM_ELEMENT_T         elem[PNDV_MAX_SV_ENTITY];   //! a bunch of list elements, one alarm request per subslot ist possible at a time
}PNDV_PERI_ALARM_QUEUE_T;

typedef struct
{

    LSA_UINT16 device_nr; /* device number */

    LSA_UINT16 nr_of_elems; /* number of array elements (1..n) */

    CM_SV_SUBMODULE_ADD_ELEMENT_TYPE elem[1];

    /* elem[2..n] follow here */

} PNDV_SV_SUBMODULE_ADD_TYPE;

/* value for rcv_record */

#define PNDV_RCV_RECORD_PARA   0x00000002
#define PNDV_RCV_RECORD_245    0x00000001

typedef struct PNDV_USR_DATA_S
{
    struct
    {
        Unsigned32              service_running;
        PNDV_RQB_PTR_TYPE       usr_rqb;
    }dev_read;
}PNDV_USR_DATA_T;

typedef struct PNDV_CFG_DATA_S
{
// real config
    PNDV_PERI_CFG_T                     peri_cfg;                     //!< real config administration

    UnsignedOpt                         max_modul_idx;                //!< last usable module index of current config
    PNDV_SM_CONNECT_SERVICE_STATE_T     sm_connect_service;           //!< Statemachine fuer connect service

// expected config
    PNDV_ENTITY_ADMIN_T                 entity_admin[PNDV_CM_AR_NO];  //!< administration for entity index per possible ar

//isom - cycle synchrony
    Boolean                     mode_isom;   /* config info for a successful 0x8030 record for the im */
    Unsigned32                  akt_isom_ar_idx;
    Unsigned32                  prev_isom_ar_idx; /* is used if a isom ar goes offline */
    Boolean                     all_modul_running;

    LSA_UINT16                  device_id;                           //! own device id

    Unsigned16                  first_real_cfg_modul_index;          //! first peri module
    //Signed16                    submodul_count;
    Unsigned16                  slot_no_running_cm_modul_req;        //! bitfield to indicate non-parralel running cm request

    PNDV_PD_CFG_S_T             pd;

}   PNDV_CFG_DATA_T;

typedef struct PNDV_IO_RECORD_OFFSET_S
{
    Unsigned16 pos_iocs_len;
    Unsigned16 pos_iocs;
    Unsigned16 pos_iops_len;
    Unsigned16 pos_iops;
    Unsigned16 pos_data_len;
    Unsigned16 pos_data;

}   PNDV_IO_RECORD_OFFSET_T;

typedef struct PNDV_RD_IO_RECORD_REQ_S
{
    UnsignedOpt            state;
    CM_UPPER_RQB_PTR_TYPE  rqb_ptr;
    Unsigned16             inp_slot;   /* applies for both slots: */
    Unsigned16             outp_slot;  /* only valid for state = PNDV_RD_IO_RECORD_STATE_COPY_REQ otherwise 0xFFFF */
    Unsigned16             subslot;
    Unsigned8 *            data_ptr;
    Unsigned16             data_len;

}   PNDV_RD_IO_RECORD_REQ_T;

typedef struct PNDV_AR_DATA_S
{
    PNDV_AR_SM_STATE_T         sm_state;         //!< state variable for ar state machine

    /* ar information */
    Unsigned32                 ar_nr;
    Unsigned16                 ar_type;
    Unsigned32                 ar_properties; /* see cm_ar_properties_enum */
    Unsigned32                 sr_rdht_msec; /* redundancy data hold time, in milliseconds */
    Unsigned32                 sr_properties; /* see CM_SR_PROP_INP_VALID_ON_BACKUP_AR, etc. */
    UnsignedOpt                sr_firstAR;
    Unsigned16                 session_key;   /* cl:offline_counter   sv:connect_counter */
    Unsigned16                 nr_of_iocrs;   /* nr of iocr associated with this ar */
    Unsigned16                 input_cr_io_data_length; /* sum of SubmoduleDataLength of all submodules within this CR */
    Unsigned16                 input_cr_ioxs_length;    /* sum of IOPS/IOCS of all submodules contained in this CR. DiscardIOXS not considered! */
    Unsigned16                 output_cr_io_data_length;
    Unsigned16                 output_cr_ioxs_length;
    Unsigned16                 ar_set_nr;
    CLRPC_UUID_TYPE            ar_uuid;
    CLRPC_UUID_TYPE            cmi_obj_uuid;  /* CMInitiatorObjectUUID to identify the connection (not the ar) */
    Unsigned32                 host_ip;       /* network byte order, ip-address of IOC */
    Unsigned8*                 host_name;     /* zero-terminated string */
    Unsigned8                  ar_fsu_enable;
    Unsigned8                  ar_fsu_uuid[16];

    Unsigned16                 akt_def_para_slot_nr;
    Unsigned16                 max_slot_nr;

    CM_UPPER_RQB_PTR_TYPE           con_ind_rqb_ptr;

    CM_UPPER_RQB_PTR_TYPE           own_ind_rqb_ptr;
    UnsignedOpt                     own_rsp_count;

    CM_UPPER_RQB_PTR_TYPE           prm_end_ind_rqb_ptr;
    UnsignedOpt                     prm_end_rsp_count;
    UnsignedOpt                     prm_end_rsp_deleted;
    Unsigned16                      prm_end_elem_cnt;
    CM_SV_AR_PRM_END_ELEMENT_TYPE   prm_end_elem_list[PNDV_MAX_SV_ENTITY];

    UnsignedOpt                     ar_set_trigger_running;
    UnsignedOpt                     ar_set_trigger_new_event;
    UnsignedOpt                     ar_set_trigger_safed_is_primary;

    CM_UPPER_RQB_PTR_TYPE           indata_ind_rqb_ptr;


    CM_UPPER_RQB_PTR_TYPE           disconnect_ind_rqb_ptr;



    /* the next variables point to the last byte of the respective frame element
       -> number of frame bytes until then = max_offset + 1
       Example: Data from slot 4 are at offset 8 len = 2 bytes + IPS
             = 8 + 2 = 10 = offset of the IPS (last byte of the io-data-object)
             number of bytes until then = 11 (8 before the data for slot 4 + 2 + 1) */
    Unsigned16                  icr_max_offset;
    Unsigned16                  ocr_max_offset;

    UnsignedOpt                 no_prm_end_ind;          // set TRUE during ownership_ind_done if the ar ownes no good module or is locked
    UnsignedOpt                 empty_prm_end_ind;       // set TRUE during prm_end_ind if an empty prm_end_ind is given by cm --> no appl. ready, take shortcut

    Unsigned32                  current_rt_class;        /* rt class for peripheral data    */

    UnsignedOpt                 ar_abort_requested;

    PNDV_ENUM_AR_T              pndv_ar_type;

    Unsigned16                  send_clock;
    Unsigned16                  reduction_ratio[PNDV_MAX_IOCR_PER_AR];

    UnsignedOpt                 pdev_8000_locked;
    UnsignedOpt                 pdev_9000_locked;

    #ifdef PNDV_CFG_ENABLE_RS_INFO_BLOCK_SUPPORT
    UnsignedOpt                 has_RSInfoBlock;
    #endif

}   PNDV_AR_DATA_T;

typedef enum PNDV_SR_EVENT_E
{
    TRC_SUBLABEL_LIST_OPEN("TRC_PNDV_SR_SM_EVENT")
    PNDV_SR_EVENT_NONE                   = 0x00,     TRC_EXPLAIN("SR EVENT: NOTHING")                   /**< should not be used */
    PNDV_SR_EVENT_ADD_TO_ARSET           = 0x01,     TRC_EXPLAIN("SR EVENT: ADD TO AR-SET")             /**< add an ar to a sysred ar set */
    PNDV_SR_EVENT_REMOVE_FROM_ARSET      = 0x02,     TRC_EXPLAIN("SR EVENT: REMOVE FROM AR-SET")        /**< ar of a systed ar set has been disconnected, remove it from the set */
    PNDV_SR_EVENT_REMOVE_DONE            = 0x03,     TRC_EXPLAIN("SR EVENT: REMOVE DONE")               /**< removing an ar from the set has been done */
    PNDV_SR_EVENT_RDHT_TIMEOUT           = 0x04,     TRC_EXPLAIN("SR EVENT: RDHT TIMEOUT")              /**< a data hold timeout occured */
    PNDV_SR_EVENT_RDHT_TIMEOUT_DONE      = 0x05,     TRC_EXPLAIN("SR EVENT: RDHT TIMEOUT DONE")         /**< data hold timeout has been processed */

    TRC_SUBLABEL_LIST_CLOSE("TRC_PNDV_SR_SM_EVENT")

    PNDV_SR_EVENT_NOT_USED               = 0xFF

}PNDV_SR_EVENT_T;

typedef enum PNDV_SR_STATE_E
{
    TRC_SUBLABEL_LIST_OPEN("TRC_PNDV_SR_SM_STATE")

    PNDV_SR_STATE_NONE           = 0x00,     TRC_EXPLAIN("SR -") /**< should never occur */
    PNDV_SR_STATE_OFFLINE        = 0x01,     TRC_EXPLAIN("SR OFFLINE")          //!< ar-set is not existent, the resource is free
    PNDV_SR_STATE_ESTABLISH      = 0x02,     TRC_EXPLAIN("SR ESTABLISHED")      //!< first ar of set is currently established (automaticly becomes the leading ar)
    PNDV_SR_STATE_ONLINE         = 0x03,     TRC_EXPLAIN("SR ONLINE")           //!< all ars of a set are established stable redundancy state reached
    PNDV_SR_STATE_ABORT          = 0x04,     TRC_EXPLAIN("SR ABORT")            //!< the whole ar-set is going down
    PNDV_SR_STATE_W_REMOVE_DONE  = 0x05,     TRC_EXPLAIN("SR WAIT REMOVE DONE") //!< a single ar of a set is beeing removed
    PNDV_SR_STATE_W_OFFLINE_DONE = 0x06,     TRC_EXPLAIN("SR WAIT OFFLINE DONE")//!< the last ar of aset is beeing removed

    TRC_SUBLABEL_LIST_CLOSE("TRC_PNDV_SR_SM_STATE")

    PNDV_SR_STATE_NOT_USED      = 0xFF

}PNDV_SR_STATE_T;

typedef struct PNDV_SR_DATA_S
{
    PNDV_SR_STATE_T     sm;             //!< state of this sm resource
    Unsigned16          primary_ar_idx; //!< this is the index of the leading (mostly identical to the primary) ar
    Unsigned16          ar_set_nr;      //!< number of the ar-set related to this resource
}PNDV_SR_DATA_T;

typedef struct PNDV_RD_IO_DATA_S
{
    PNDV_RD_IO_RECORD_REQ_T    rd_io_req;
    Unsigned8                  rd_input_buffer[PNDV_MAX_RD_INP_DATA_LEN];   
    Unsigned8                  rd_output_buffer[PNDV_MAX_RD_OUTP_DATA_LEN];

}   PNDV_RD_IO_DATA_T;

typedef struct PNDV_STATIC_RQBS_S
{
    CM_RQB_TYPE                   dev_control;                     /* 1 device */
    CM_SV_DEVICE_CONTROL_TYPE     dev_control_args;

    CM_RQB_TYPE                   app_ready[PNDV_CM_AR_NO+1];        /* PNDV_CM_AR_NO ar */
    struct {

        LSA_UINT16 device_nr;   /* device number */

        LSA_UINT16 nr_of_elems; /* number of array elements (1..n) */

        CM_SV_AR_APPL_READY_ELEMENT_TYPE elem[PNDV_MAX_SV_ENTITY];


    } /*CM_SV_AR_APPL_READY_TYPE*/ app_ready_args[PNDV_CM_AR_NO+1];

    CM_RQB_TYPE                   ar_abort[PNDV_CM_AR_NO+1];         /* PNDV_CM_AR_NO ar */
    CM_SV_AR_ABORT_TYPE           ar_abort_args[PNDV_CM_AR_NO+1];

    CM_RQB_TYPE                   diag_add;                        /* 1 device */
    struct
    {
        LSA_UINT16                  device_nr;      /* device number */

        LSA_UINT16                  nr_of_elems;    /* number of array elements (1..n) */

        CM_SV_DIAG_ADD_ELEMENT_TYPE elem[PNDV_MAX_SV_ENTITY];

    }/*CM_SV_DIAG_ADD_TYPE*/    diag_add_args;

    CM_RQB_TYPE                   diag_remove;                      /* 1 device */
    CM_SV_DIAG_REMOVE_TYPE        diag_remove_args;                 /* needed for DIAG_REMOVE */
    CM_SV_DIAG_ADD_TYPE           diag_remove_keys;                 /* needed for DIAG_REMOVE2 */

    CM_RQB_TYPE                   ium_rqb;
    CM_EVENT_TYPE                 ium_rqb_event;
    Unsigned8                     ium_rqb_event_record_data[CM_RECORD_OFFSET + 60];

    CM_RQB_TYPE                   device_led_info;
    CM_SV_DEVICE_LED_INFO_TYPE    device_led_info_args;

    CM_RQB_TYPE                   sync_led_info;
    CM_PD_SYNC_LED_INFO_TYPE      sync_led_info_args;

    CM_UPPER_RQB_PTR_TYPE         ar_info_ind_ptr;

    CM_RQB_TYPE                   rir_rqb[PNDV_CM_AR_NO+1];
    CM_SV_AR_RIR_ACK_TYPE         rir_ack_args[PNDV_CM_AR_NO+1];

    CM_UPPER_RQB_PTR_TYPE         store_ds_ptr_for_recopy; /* If a DS-Req has to be copied into the interface 
                                                             (because it is not there and the slave could not access it), 
                                                              the original DS is saved here */
    //CM_UPPER_RQB_PTR_TYPE         pending_connect_indication_rqb_ptr; // no longer used see pndv_data.serv
    PNDV_RQB_TYPE                 connect_indication_timeout_rqb;

    CM_UPPER_RQB_PTR_TYPE         stored_rtc3_ind_rqb_ptr[PNDV_CM_AR_NO+1];

    CM_UPPER_RQB_PTR_TYPE         stored_prm_end_ind_rqb_ptr[PNDV_CM_AR_NO+1];

    CM_RQB_TYPE                   sub_add_rem;             /* RQB to CM for pull/plug-alarms */
    struct
    {
        LSA_UINT16 device_nr; /* device number */

        LSA_UINT16 nr_of_elems; /* number of array elements (1..n) */

        CM_SV_SUBMODULE_ADD_ELEMENT_TYPE elem[PNDV_MAX_SV_ENTITY];
    }/*CM_SV_SUBMODULE_ADD_TYPE*/ sub_add_args;

    Unsigned32                    sub_add_pd_count;

    struct
    {
        LSA_UINT16 device_nr; /* device number */

        LSA_UINT16 nr_of_elems; /* number of array elements (1..n) */

        CM_SV_SUBMODULE_REMOVE_ELEMENT_TYPE elem[PNDV_MAX_SV_ENTITY];
    }/*CM_SV_SUBMODULE_REMOVE_TYPE*/ sub_rem_args;


}   PNDV_STATIC_RQBS_T;


/* Alarme */
/* ------------------------------------------------------------------------- */

typedef struct PNDV_AL_MODUL_DATA_S
{
    CM_RQB_TYPE         dial_rqb;
    CM_ALARM_TYPE       dial_data;
    UnsignedOpt         dial_state;
    Unsigned8           dial_info[PNDV_AL_DIAL_MAX_INFO_LEN];

    CM_RQB_TYPE         pral_rqb;
    CM_ALARM_TYPE       pral_data;
    UnsignedOpt         pral_state;
    Unsigned8           pral_info[PNDV_AL_PRAL_INFO_MAX_LEN];

    CM_RQB_TYPE         upal_rqb;
    CM_ALARM_TYPE       upal_data;
    UnsignedOpt         upal_state;
    Unsigned8           upal_info[PNDV_AL_UPAL_INFO_LEN];

    CM_RQB_TYPE         ural_rqb;
    CM_ALARM_TYPE       ural_data;
    UnsignedOpt         ural_state;
    Unsigned8           ural_info[PNDV_AL_URAL_INFO_LEN];

    CM_RQB_TYPE         stal_rqb;
    CM_ALARM_TYPE       stal_data;
    UnsignedOpt         stal_state;
    Unsigned8           stal_info[PNDV_AL_STAL_INFO_LEN];

    CM_RQB_TYPE         rosal_rqb;
    CM_ALARM_TYPE       rosal_data;
    UnsignedOpt         rosal_state;
    Unsigned8           rosal_info[PNDV_AL_ROSAL_INFO_LEN];

}   PNDV_AL_SUBMODUL_DATA_T;

typedef struct PNDV_AL_DATA_S
{
    PNDV_AL_SUBMODUL_DATA_T submodul[PNDV_MAX_SV_ENTITY];

    PNDV_PERI_ALARM_QUEUE_T q_rosal;

}   PNDV_AL_DATA_T;


/* DS-Plexer */
/* ------------------------------------------------------------------------- */

#define PNDV_DS_RES_IDX_RD_IO      0

#define PNDV_DFC_CON_ID_RD_INP_OUTP_BASIS    1
/* PNDV_DFC_CON_ID_RD_INP_OUTP_BASIS must be the last number */


/* rd input rd output */
/* --------------------------------------- */

typedef struct PNDV_RD_INP_OUTP_S
{
    PNDV_RQB_DS_PTR             ds_struc_ptr;

}   PNDV_RD_INP_OUTP_T;

#define PNDV_RD_INP_OUTP_IOPS_OFFS  0 /* relative by ioxs_ptr */
#define PNDV_RD_INP_OUTP_IOCS_OFFS  1 /* relative by ioxs_ptr */

#define PNDV_IOXS_OFFS				2 /* Peripheral Record IOXs Offset */

/* prm service queue */

typedef struct PNDV_PRM_REQUEST_ELEMENT_S
{
        PNDV_LIST_ENTRY_TYPE              link;

    CM_UPPER_RQB_PTR_TYPE             rqb_ptr;
}   PNDV_PRM_REQUEST_ELEMENT_T;

typedef PNDV_PRM_REQUEST_ELEMENT_T * PNDV_PRM_REQUEST_ELEMENT_PTR_T;

typedef struct PNDV_PDEV_PRM_RQB_QUEUE_S
{
    PNDV_LIST_ENTRY_TYPE              free_list;                  //! free list elements queue
    PNDV_LIST_ENTRY_TYPE              in_use;                     //! list with waiting list elements

    PNDV_PRM_REQUEST_ELEMENT_T       elem[PNDV_PD_MAX_PRM_REC_COUNT];     //! 4
}PNDV_PDEV_PRM_RQB_QUEUE_T;

typedef PNDV_PDEV_PRM_RQB_QUEUE_T * PNDV_PDEV_PRM_RQB_QUEUE_PTR_T;

/* queue for serialization of events of one specific service (like ready-for-input-update) to user */
typedef struct PNDV_EVENT_SERIALIZATION_S
{
    struct
    {
        PNDV_IFACE_CMD_ENTRY_T  event;
        Boolean                 in_use;
        Boolean                 sent_to_user;
    }event_array[PNDV_CM_AR_NO];
    Unsigned8 in_use_count;
}PNDV_PERI_EVENT_SERIALIZATION_T;

/* int data struct */
/* ------------------------------------------------------------------------- */

typedef struct PNDV_DATA_S
{
    // state machines

    PNDV_SM_STATE_E_T                       sm;                             //!< pndv main state machine

    /* parameter */

    LSA_HANDLE_TYPE                     cm_handle[PNDV_MAX_CHANNEL];    /* IO device (my productive handle) is always index (PNDV_MAX_CHANNEL-1)) */

    LSA_SYS_PATH_TYPE                   sys_path_cm[PNDV_MAX_CHANNEL];  /* index see  PNDV_OPEN_CHANNEL_TYPE */

    LSA_USER_ID_TYPE                    null_user_id;
    Unsigned32                         *annotation_ptr;
    Unsigned32                         *oem_data_ptr;

    Unsigned8                           port_link_status[PNDV_CFG_MAX_PORT_CNT];

    Unsigned32                          reset_reason;

    UnsignedOpt                         stop_req;

    PNDV_STATIC_RQBS_T                  rqb;

    PNDV_AR_DATA_T                      ar[PNDV_CM_AR_NO+1];     /* +1 so that too many AR can be terminated cleanly */

    PNDV_SR_DATA_T                      sr[PNDV_MAX_AR_SET+1];   //!< system redundancy data

    PNDV_RD_IO_DATA_T                   rd_io;

    PNDV_AL_DATA_T                      al;

    PNDV_CFG_DATA_T                     cfg;

    PNDV_PERI_SERVICE_QUEUES_T          serv;                   //!< service list structure for queueing own, prm_end ind, etc.

    PNDV_USR_DATA_T                     usr;

#ifdef PNDV_CFG_PERI_QUEUE_DS_REQ
    PNDV_PERI_RQB_QUEUE_T               rqb_queue;
#endif

    PNDV_PDEV_PRM_RQB_QUEUE_T           prm_queue;

    PNDV_PERI_EVENT_SERIALIZATION_T     ev_queue_ready_for_input_update;

    PNDV_DIAG_DATA_T                    diag;

    PNDV_RD_INP_OUTP_T                  rd_in_out;

    #ifdef PNDV_CFG_DEBUG_ENABLE

    PNDV_STRUC_DEBUG_BUFFER_ITEM_T      debug_element;

    PNDV_DEBUG_MODE_TYPE                debug_control;

    #endif


    #ifdef PNDV_CFG_DEBUG_COUNTER

    PNDV_TRIGGER_T                 trigger;

    #endif

    UnsignedOpt                         ar_in_data_led_used;
    UnsignedOpt                         akt_clear_state;

    Unsigned8                           tmp_io_record[1500];

    Signed32                            alloc_mem_counter;
    Signed32                            alloc_rqb_counter;

    Boolean                             cmpd_prm_default;

    LSA_UINT16                          connect_indication_timeout_timer_id;

    Unsigned8                           device_control;

    Unsigned32                          ar_so_locked_state[PNDV_MAX_ARS_RT];
    CLRPC_UUID_TYPE                     so_locked_isom_uuid;

    UnsignedOpt                         block_ar_while_rtf;     /* ResetToFactory Block peri-AR */

    PNDV_IFACE_STRUCT_PTR               iface_ptr;              /* pointer to pndv area inside of glob_coupling_interface */

    PNDV_RQB_DS1                        * rema_station_prm_ptr;   /* pointer to apma_data.rema_station_prm */
    Unsigned16                          ds_rw_soll_slot_stored;

    struct
    {
        CM_RQB_TYPE                     rqb;
        CM_EVENT_TYPE                   event_type;
        Unsigned8                       record_data[CM_RECORD_OFFSET + 8];
    } rema_local_station_prm;

}   PNDV_DATA_T;



/*****************************************************************************/
/* debug-buffer macros */

#ifdef PNDV_CFG_DEBUG_ENABLE
#ifdef    PNDV_CFG_DEBUG_USE_IM_TRC
    #error 
    #define pndv_in_count_debug_buffer_cnt__(_detail_1, _detail_2)            \
    {                                                                         \
        PNDV_INT_DEBUG_BUFFER_ITEM_PTR tmp_db_ptr;                            \
                                                                              \
        tmp_db_ptr = pndv_in_get_last_debug_buffer();                         \
                                                                              \
        if (   (tmp_db_ptr->as.all.detail_1 == _detail_1 )                    \
            && (tmp_db_ptr->as.all.detail_2 == _detail_2 )                    \
           )                                                                  \
        {                                                                     \
            /* the last entry is the same                             \
               -> Increase counters */                                         \
                                                                              \
            tmp_db_ptr->counter++;                                            \
        }                                                                     \
        else                                                                  \
        {                                                                     \
            /* Entry is new */                                             \
            pndv_in_write_debug_buffer_all__(_detail_1, _detail_2);           \
        }                                                                     \
    }
#else
    #define pndv_in_count_debug_buffer_cnt__(_detail_1, _detail_2) pndv_in_write_debug_buffer_all__(_detail_1, _detail_2);
#endif

    #define pndv_in_write_debug_buffer_all__(_debug_code,_detail_1)                         pndv_in_write_debug_buffer( _detail_1,0,0,_debug_code, 0)
    #define pndv_in_write_debug_buffer_1__(_debug_code,_detail_1)                           pndv_in_write_debug_buffer( _detail_1,0,0,_debug_code, 0)
    #define pndv_in_write_debug_buffer_2__(_debug_code,_detail_1,_detail_2)                 pndv_in_write_debug_buffer( _detail_1,_detail_2,0,_debug_code, 0)
    #define pndv_in_write_debug_buffer_3__(_debug_code,_detail_1,_detail_2,_detail_3)       pndv_in_write_debug_buffer( _detail_1,_detail_2,_detail_3,_debug_code, 0)

    #define pndv_in_write_debug_buffer_all_add__(_debug_code,_detail_1,_add)     pndv_in_write_debug_buffer( _detail_1,0, _add,_debug_code, 0)



    #define pndv_in_write_debug_buffer_io__(_detail_1,_detail_2)     pndv_in_write_debug_buffer_all__(_detail_1,_detail_2)
    #define pndv_in_write_debug_buffer_al__(_detail_1,_detail_2)     pndv_in_write_debug_buffer_all__(_detail_1,_detail_2)
    #define pndv_in_write_debug_buffer_ds__(_debug_code,_detail_1)   pndv_in_write_debug_buffer_2__(_debug_code,(Unsigned16)(_detail_1>>16),(Unsigned16)_detail_1)
    #define pndv_in_write_debug_buffer_dpr__(_detail_1,_detail_2)    pndv_in_write_debug_buffer_all__(_detail_1,_detail_2)

#else

#define pndv_in_count_debug_buffer_cnt__(_detail_1, _detail_2)

#define pndv_in_write_debug_buffer_all__(_debug_code,_detail_1)
#define pndv_in_write_debug_buffer_1__(_debug_code,_detail_1)
#define pndv_in_write_debug_buffer_2__(_debug_code,_detail_1,_detail_2)
#define pndv_in_write_debug_buffer_3__(_debug_code,_detail_1,_detail_2,_detail_3)

#define pndv_in_write_debug_buffer_all_add__(_debug_code,_detail_1,_add)



#define pndv_in_write_debug_buffer_io__(_detail_1,_detail_2)
#define pndv_in_write_debug_buffer_al__(_detail_1,_detail_2)
#define pndv_in_write_debug_buffer_ds__(_debug_code,_detail_1)
#define pndv_in_write_debug_buffer_dpr__(_detail_1,_detail_2)

#endif


#ifdef PNDV_CFG_DEBUG_COUNTER
    #define pndv_in_trigger__(_item)         pndv_data.trigger._item += 1;
#else
    #define pndv_in_trigger__(_item)
#endif

#define PNDV_STORE_PP_SRC_IN_RQB(_rb, _value)               PNDV_RQB_SET_USER_ID_UVAR16_ARRAY_HIGH(_rb, _value)
#define PNDV_STORE_CHANNEL_DIAG_QUIT_IN_RQB(_rb, _value)    PNDV_RQB_SET_USER_ID_UVAR16_ARRAY_HIGH(_rb, _value)

#define PNDV_LOAD_PP_SRC_FROM_RQB(_rb)                      PNDV_RQB_GET_USER_ID_UVAR16_ARRAY_HIGH(_rb)
#define PNDV_LOAD_CHANNEL_DIAG_QUIT_FROM_RQB(_rb)           PNDV_RQB_GET_USER_ID_UVAR16_ARRAY_HIGH(_rb)

/*****************************************************************************/
/* internal macros */

#define pndv_min__(_a,_b)                        ( (_a) < (_b) ? (_a) : (_b) )
#define pndv_max__(_a,_b)                        ( (_a) > (_b) ? (_a) : (_b) )

#define pndv_host_ptr_is_nil__(_ptr1)            ( (PNDV_HOST_PTR_COMPARE_TYPE) NIL     == (PNDV_HOST_PTR_COMPARE_TYPE) (_ptr1) )
#define pndv_host_ptr_are_equal__(_ptr1,_ptr2)   ( (PNDV_HOST_PTR_COMPARE_TYPE) (_ptr2) == (PNDV_HOST_PTR_COMPARE_TYPE) (_ptr1) )

// Optional submodule numbers for 2nd head, e.g PN Coupler +0x100
#ifndef IM_SUBMODULE_NR_SLAVE_OFFSET
  #define CASE_IM_SUBMODULE_NR_INTERFACE_SLAVE
  #define CASE_IM_SUBMODULE_NR_PORT_1_SLAVE
  #define CASE_IM_SUBMODULE_NR_PORT_2_SLAVE
#else
  #define CASE_IM_SUBMODULE_NR_INTERFACE_SLAVE case IM_SUBMODULE_NR_INTERFACE + IM_SUBMODULE_NR_SLAVE_OFFSET:
  #define CASE_IM_SUBMODULE_NR_PORT_1_SLAVE    case IM_SUBMODULE_NR_PORT_1    + IM_SUBMODULE_NR_SLAVE_OFFSET:
  #define CASE_IM_SUBMODULE_NR_PORT_2_SLAVE    case IM_SUBMODULE_NR_PORT_2    + IM_SUBMODULE_NR_SLAVE_OFFSET:
#endif

#define pndv_get_subslot_idx__(_subslot_idx, _slot_nr, _subslot_nr, _error_ptr)                   \
{                                                                                             \
    Unsigned16 _tmp_subslot_nr;                                                               \
    Unsigned16 _tmp_slot_nr;                                                                  \
                                                                                              \
    (*_error_ptr) = FALSE;                                                                    \
                                                                                              \
    _tmp_slot_nr = _slot_nr;                                                                  \
    _tmp_subslot_nr = _subslot_nr;                                                            \
                                                                                              \
    if (_slot_nr == PNDV_IM_SLOT_NO)                                                          \
    {                                                                                         \
        switch(_subslot_nr)                                                                   \
        {                                                                                     \
            case DEV_CFG_IM_SUBMODULE_NR_INTERFACE:                                           \
            CASE_IM_SUBMODULE_NR_INTERFACE_SLAVE                                              \
            {                                                                                 \
                _tmp_subslot_nr = PNDV_MAX_SV_ENTITY - 1;                                     \
                break;                                                                        \
            }                                                                                 \
            case DEV_CFG_IM_SUBMODULE_NR_PORT_1:                                              \
            CASE_IM_SUBMODULE_NR_PORT_1_SLAVE                                                 \
            {                                                                                 \
                _tmp_subslot_nr = PNDV_MAX_SV_ENTITY - 2;                                     \
                break;                                                                        \
            }                                                                                 \
            case DEV_CFG_IM_SUBMODULE_NR_PORT_2:                                              \
            CASE_IM_SUBMODULE_NR_PORT_2_SLAVE                                                 \
            {                                                                                 \
                _tmp_subslot_nr = PNDV_MAX_SV_ENTITY - 3;                                     \
                break;                                                                        \
            }                                                                                 \
            default:                                                                          \
            {                                                                                 \
                _tmp_subslot_nr = _subslot_nr;                                                \
                break;                                                                        \
            }                                                                                 \
        }                                                                                     \
    }                                                                                         \
                                                                                              \
    if (  ((_tmp_slot_nr )    >= PNDV_MAX_SV_ENTITY )                                         \
        ||((_tmp_subslot_nr ) >= PNDV_MAX_SV_ENTITY )                                         \
        )                                                                                     \
    {                                                                                         \
        /* slot or subslot out of range */                                                    \
                                                                                              \
        (*_error_ptr) = TRUE;                                                                 \
    }                                                                                         \
                                                                                              \
    if (!_error)                                                                              \
    {                                                                                         \
        _subslot_idx = pndv_data.cfg.peri_cfg.ff_index_array[_tmp_slot_nr][_tmp_subslot_nr];  \
    }                                                                                         \
}

#define pndv_set_subslot_idx__(_subslot_idx, _slot_nr, _subslot_nr, _error_ptr)               \
{                                                                                             \
    Unsigned16 _tmp_subslot_nr;                                                               \
    Unsigned16 _tmp_slot_nr;                                                                  \
                                                                                              \
   (*_error_ptr) = FALSE;                                                                     \
                                                                                              \
    /* DAP is always 0, 1 for us */                                                    \
    _tmp_subslot_nr = (_subslot_nr == 0xFFFF) ? 1 : _subslot_nr;                              \
    _tmp_slot_nr    = (_slot_nr    == 0xFFFF) ? PNDV_IM_SLOT_NO : _slot_nr;                   \
                                                                                              \
    if (_slot_nr == PNDV_IM_SLOT_NO)                                                          \
    {                                                                                         \
        switch(_subslot_nr)                                                                   \
        {                                                                                     \
            case DEV_CFG_IM_SUBMODULE_NR_INTERFACE:                                           \
            CASE_IM_SUBMODULE_NR_INTERFACE_SLAVE                                              \
            {                                                                                 \
                _tmp_subslot_nr = PNDV_MAX_SV_ENTITY - 1;                                     \
                break;                                                                        \
            }                                                                                 \
            case DEV_CFG_IM_SUBMODULE_NR_PORT_1:                                              \
            CASE_IM_SUBMODULE_NR_PORT_1_SLAVE                                                 \
            {                                                                                 \
                _tmp_subslot_nr = PNDV_MAX_SV_ENTITY - 2;                                     \
                break;                                                                        \
            }                                                                                 \
            case DEV_CFG_IM_SUBMODULE_NR_PORT_2:                                              \
            CASE_IM_SUBMODULE_NR_PORT_2_SLAVE                                                 \
            {                                                                                 \
                _tmp_subslot_nr = PNDV_MAX_SV_ENTITY - 3;                                     \
                break;                                                                        \
            }                                                                                 \
            default:                                                                          \
            {                                                                                 \
                _tmp_subslot_nr = _subslot_nr;                                                \
                break;                                                                        \
            }                                                                                 \
        }                                                                                     \
    }                                                                                         \
                                                                                              \
    if (  ((_tmp_slot_nr )    >= PNDV_MAX_SV_ENTITY )                                         \
        ||((_tmp_subslot_nr ) >= PNDV_MAX_SV_ENTITY )                                         \
        )                                                                                     \
    {                                                                                         \
        /* slot or subslot out of range */                                                    \
                                                                                              \
        (*_error_ptr) = TRUE;                                                                 \
    }                                                                                         \
                                                                                              \
    if (!_error)                                                                              \
    {                                                                                         \
        pndv_data.cfg.peri_cfg.ff_index_array[_tmp_slot_nr][_tmp_subslot_nr] = _subslot_idx;  \
    }                                                                                         \
}


#ifdef PNDV_CFG_USE_FAST_INDEX_SEARCH
#define pndv_get_submod_resource_ptr(_res_ptr_ptr,  _slot_nr, _subslot_nr, _error_ptr)\
{\
    Unsigned16 subslot_idx;\
    \
    pndv_get_subslot_idx__(subslot_idx, _slot_nr, _subslot_nr, _error_ptr);\
    if((!(*_error_ptr))&&(subslot_idx != PNDV_CFG_UNBOUND_SUB))\
    {\
        (*local_sub_res) = &pndv_data.cfg.peri_cfg.sub_res[subslot_idx];\
    }\
    else\
    {\
        (*local_sub_res) = 0;\
    }\
}

#define pndv_lock_submod_resource(_res_ptr,  _slot_nr, _subslot_nr, _error_ptr)\
{\
    pndv_set_subslot_idx__((_res_ptr->my_field_index), _slot_nr, _subslot_nr, _error_ptr);\
}

#define pndv_unlock_submod_resource(_res_ptr,  _slot_nr, _subslot_nr, _error_ptr)\
{\
    pndv_set_subslot_idx__((PNDV_CFG_UNBOUND_SUB), _slot_nr, _subslot_nr, _error_ptr);\
}
#else

#ifdef __cplusplus
extern "C"
{
#endif

    void pndv_get_submod_resource_ptr(PNDV_STRUCT_CFG_SUBMODULE_RES_PTR_T *res_ptr_ptr, Unsigned16 slot, Unsigned16 subslot, UnsignedOpt *error);

#ifdef __cplusplus
}
#endif


// not needed with this setup
  #define pndv_lock_submod_resource(_res_ptr,  _slot_nr, _subslot_nr, _error_ptr)       *_error_ptr = 0
  #define pndv_unlock_submod_resource(_res_ptr,  _slot_nr, _subslot_nr, _error_ptr)     *_error_ptr = 0
#endif

#define PndvListLast(ListHead_, Type_) (\
    (Type_)(CmListIsEmpty(ListHead_) ? LSA_NULL : (ListHead_)->Blink)\
    )

#define PndvListPrev(ListHead_, Entry_, Type_) (\
    (Type_)(LSA_HOST_PTR_ARE_EQUAL((Entry_)->Blink, (ListHead_)) ? LSA_NULL : (Entry_)->Blink)\
    )

/*****************************************************************************/
/* dummy operations */


#define pndv_unused__(_a)                        ( (_a)=(_a) )


/*****************************************************************************/
/* internal function prototypes */

/*lint -sem( pndv_in_fatal_error, r_no ) the function does not return */

#ifdef __cplusplus
extern "C"
{
#endif

/* pndv.c */

void        pndv_in_start                           (void);
void        pndv_in_stop                            (void);
UnsignedOpt pndv_in_get_api_idx                     ( Unsigned32 api, UnsignedOpt* api_ix );
void        pndv_in_compress_api_list               ( UnsignedOpt tmp_api_ix);
void        pndv_in_check_led_info                  (void);
void        pndv_in_check_sync_info                 (void);
void        pndv_in_init_local_sub_res              (PNDV_STRUCT_CFG_SUBMODULE_RES_PTR_T local_sub_res);
UnsignedOpt pndv_in_debug_control                   (PNDV_DEBUG_MODE_TYPE    debug_control);
void        pndv_in_write_debug_buffer              (Unsigned16 detail_1, Unsigned16 detail_2, Unsigned32 detail_3, PNDV_DEBUG_CODE_E_T deb_code, Unsigned32 add);
void        pndv_in_fatal_error                     (Unsigned8 error_module, Unsigned16 error_line, Unsigned32 error_code);

/* pndv_al.c */

void        pndv_in_al_init                      (void);
void        pndv_in_al_init_submodul             (PNDV_AL_SUBMODUL_DATA_T *al_submodul_ptr);
void        pndv_al_peri_check_dial              (Unsigned16 entity_nr, PNDV_REAL_CFG_T *real_cfg_channel_diag_ptr);
void        pndv_al_peri_check_xdial             (Unsigned16 entity_nr, PNDV_REAL_CFG_T *real_cfg_channel_diag_ptr);
void        pndv_al_peri_dial_rmv_all            (UnsignedOpt ar_idx);
void        pndv_al_peri_xdial_rmv_all           (UnsignedOpt ar_idx);
void        pndv_al_peri_pral_req                (Unsigned16 entity_nr, PNDV_SENDMSG_PRALINFO_PTR pral_ptr);
void        pndv_al_peri_upal_req                (Unsigned16 entity_nr, PNDV_SENDMSG_UPALINFO_PTR upal_ptr);
void        pndv_al_peri_ural_req                (Unsigned16 entity_nr, PNDV_SENDMSG_URALINFO_PTR ural_ptr);
void        pndv_al_peri_stal_req                (Unsigned16 entity_nr, PNDV_SENDMSG_STALINFO_PTR stal_ptr);
void        pndv_al_peri_rosal_req               (Unsigned16 slot_nr);
void        pndv_in_al_diag_req                  (PNDV_AL_USI_T usi, Unsigned16 entity_nr, Unsigned16 kanal, Unsigned16 fehlernummer, Unsigned16 properties,
                                                 Unsigned16 ext_fehler, Unsigned32 ext_wert, PNDV_CHANNEL_DIAG_REQ_T  req );
void        pndv_al_peri_alarm_reported_after_swo(void);
void        pndv_al_peri_alarm_reported_after_swo_done(void);
void        pndv_in_al_check_diag                (void);
void        pndv_in_al_check_dial_continue       (void);
void        pndv_in_al_check_rosal_queue         (void);
UnsignedOpt pndv_al_search_clear_rosal           (Unsigned16 slot, Unsigned16 subslot);
UnsignedOpt pndv_in_al_search_clear_diag         (Unsigned16 slot, Unsigned16 subslot, PNDV_CHANNEL_DIAG_REQ_T req);
UnsignedOpt pndv_in_al_quit_one_pp_req           (Unsigned16 soll_slot, Unsigned16 ist_slot, PNDV_AL_PP_REQ req, UnsignedOpt line);

Unsigned16  pndv_in_al_diag_branch               (PNDV_CHAN_DIAG_PTR diag_channel_ptr);
Unsigned16  pndv_in_al_channel_diag              (PNDV_CHAN_DIAG_PTR diag_channel_ptr);
Unsigned16  pndv_in_al_generic_diag              (PNDV_CHAN_DIAG_PTR diag_channel_ptr);
void        pndv_in_al_diag_add_done             (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_al_diag_remove_done          (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_al_diag_remove2_done         (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_al_diag_channel_add_done     (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_al_diag_channel_remove_done  (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
Unsigned16  pndv_in_al_generic_diag_peri_err     (UnsignedOpt req);
void        pndv_in_al_diag_generic_add_done     (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_al_diag_generic_remove_done  (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_al_diag_done                 (Unsigned16 entity_nr, UnsignedOpt purge);
Unsigned16  pndv_in_al_dial_send                 (PNDV_CHAN_DIAG_PTR diag_channel_ptr);
void        pndv_in_al_al_ack                    (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_al_sv_alarm_ind              (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_al_sv_alarm_ack_done    (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_al_set_critical_slot         (Unsigned8 critical_slot);
#if 0
void        pndv_in_al_set_ms                    (UnsignedOpt slot, UnsignedOpt subslot, PNDV_SUBMODULE_STATE_T modul_status);
void        pndv_in_al_clear_modul_error         (void);
void        pndv_in_al_set_all_modul_error       (void);
void        pndv_in_al_set_modul_error           (Boolean error, Unsigned16 slot, Unsigned16 subslot);
#endif
Boolean     pndv_al_check_ar_is_ready            (UnsignedOpt ar_idx);
Boolean     pndv_al_al_possible_within_state     (PNDV_STRUCT_CFG_SUBMODULE_RES_PTR_T sub_res_ptr );
Boolean     pndv_al_diag_possible_within_state   (PNDV_STRUCT_CFG_SUBMODULE_RES_PTR_T sub_res_ptr, PNDV_CHANNEL_DIAG_REQ_T req );
void        pndv_in_al_check_all_soll_modul_running(void);
void        pndv_in_al_ar_abort_if_ps_lost       (Unsigned16 slot);




/* pndv_c0.c */

void        pndv_ar_offline_cleanup                 (Unsigned32 ar_nr, UnsignedOpt ar_idx);
void        pndv_ar_tool_disconnect_ind_control_bf_led (void);
void        pndv_ar_cm_ar_offline_ind            (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_ar_cm_ar_offline_ind_done       (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
UnsignedOpt pndv_ar_abort_req                 ( UnsignedOpt ar_idx, UnsignedOpt call_line );
void        pndv_ar_abort_shared_ar           (void);
void        pndv_in_put_all_module               (void);
void        pndv_in_rmv_all_module               (Boolean first_run);
UnsignedOpt pndv_in_cm_check_module_add_remove   (void);
void        pndv_in_get_ium_cap                  (Unsigned16 slot);
void        pndv_in_get_ium_cap_done             (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
UnsignedOpt pndv_in_cm_module_add                (Unsigned16 slot, Unsigned16 ist_slot, Boolean wdh_intern, PNDV_AL_PP_REQ req);
void        pndv_in_cm_module_add_done           (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_cm_plug_next_submodule       (Unsigned16 slot_nr);
UnsignedOpt pndv_in_cm_submodule_add             (Unsigned16 slot);
void        pndv_in_cm_submodule_add_done        (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
UnsignedOpt pndv_in_cm_module_remove             (Unsigned16 slot, Unsigned16 ist_slot, Boolean wdh_intern, PNDV_AL_PP_REQ req);
void        pndv_in_cm_module_remove_done        (CM_UPPER_RQB_PTR_TYPE rqb_ptr);

PNDV_CONNECT_IND_RET_VAL_T pndv_ar_check_ind_check_excpected_cfg_1(UnsignedOpt ar_idx, CM_UPPER_EVENT_PTR_TYPE event_ptr);
Boolean     pndv_ar_check_ind_check_excpected_cfg_2(UnsignedOpt ar_idx, CM_UPPER_EVENT_PTR_TYPE event_ptr);
PNDV_CONNECT_IND_RET_VAL_T pndv_ar_tool_connect_ind_find_ar_idx        (UnsignedOpt *ar_idx_ptr, CM_UPPER_EVENT_PTR_TYPE event_ptr);
void        pndv_ar_tool_connect_ind_store_ar_data      (UnsignedOpt ar_idx, CM_UPPER_EVENT_PTR_TYPE event_ptr);
void        pndv_ar_check_ind                    (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
UnsignedOpt pndv_ar_check_rsp                    (UnsignedOpt ar_idx);
void        pndv_ar_set_cfg                      (UnsignedOpt ar_idx);


/* pndv_cm.c */

void        pndv_in_cm_open_channel              (Unsigned16 channel_idx);
void        pndv_in_cm_open_channel_done         (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_cm_close_channel             (Unsigned16 channel_idx);
void        pndv_in_cm_close_channel_done        (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_cm_put_resource              (Unsigned32 num_event_resources, Unsigned32 num_alarm_resources);
void        pndv_in_cm_event_res_done            (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_cm_al_res_done               (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
Boolean     pndv_in_cm_link_status_info          (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_cm_provide_led_info_rqb      (void);
Boolean     pndv_in_cm_dev_led_info              (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
Boolean     pndv_in_cm_sync_led_info             (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_cm_pdsv_create               (void);
void        pndv_in_cm_pdsv_create_done          (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_cm_pdsv_delete               (void);
void        pndv_in_cm_pdsv_delete_done          (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_cm_sv_create                 (void);
void        pndv_in_cm_sv_create_done            (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_cm_sv_delete                 (void);
void        pndv_in_cm_sv_delete_done            (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_cm_sv_control_done           (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_cm_check_dev_req             (void);
void        pndv_in_cm_sv_device_add             (void);
void        pndv_in_cm_sv_device_add_done        (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_cm_sv_device_remove          (void);
void        pndv_in_cm_sv_device_remove_done     (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_cm_sv_device_read_done       (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_cm_pd_prm_begin              (void);
void        pndv_in_cm_pd_build_prm_list         (PNDV_PDEV_PRM_RQB_QUEUE_PTR_T prm_queue_ptr);
void        pndv_in_cm_pd_prm_begin_done         (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_cm_pd_prm_write              (void);
void        pndv_in_cm_pd_prm_write_done         (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_cm_pd_prm_end                (void);
void        pndv_in_cm_pd_prm_end_done           (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_cm_pd_event_appl_ready_ind   (CM_UPPER_RQB_PTR_TYPE rqb_ptr);

void        pndv_in_cm_sv_device_provide_event_done(CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_cm_sv_device_provide_alarm_done(CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_cm_in_device_read_req           (PNDV_RQB_PTR_TYPE rqb_ptr);

void        pndv_in_device_control               (Unsigned8 device_control);
void        pndv_in_cm_sv_device_control_done    (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_cm_sv_appl_ready             (Unsigned16 slot, UnsignedOpt ar_idx);
void        pndv_in_cm_sv_appl_ready_done        (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_cm_sv_ar_control_done        (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_ar_cm_ar_abort_done          (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_cm_sv_event_prm_end_ind      (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_cm_sv_event_prm_end_continue (UnsignedOpt ar_idx);
void        pndv_in_cm_sv_event_ar_rir_ind      (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_cm_sv_ar_rtc3_res            (UnsignedOpt ar_idx);
void        pndv_in_cm_sv_event_apdu_status_ind  (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_ar_cm_ar_in_data_ind            (CM_UPPER_RQB_PTR_TYPE rqb_ptr);


/* pndv_peri.c */

void        pndv_in_peri_init                    (void);
void        pndv_in_peri_peribus_statemachine    (PNDV_SM_PERIBUS_EVENT event);
void        pndv_in_peri_check_event             (PNDV_IFACE_CMD_ENTRY_T event);
PNDV_IFACE_CMD_ENTRY_T  pndv_in_peri_read_coupl_event        (void);
void        pndv_in_peri_write_coupl_event       (PNDV_IFACE_CMD_ENTRY_T event);
void        pndv_in_peri_put_cfg_to_interface    (UnsignedOpt ar_idx);
void        pndv_in_peri_put_ds_to_interface     (CM_UPPER_RQB_PTR_TYPE ds_ptr);
UnsignedOpt pndv_in_peri_def_para_one_slot       (UnsignedOpt ar_idx, Unsigned16 soll_slot, Unsigned16 ist_slot);
void        pndv_in_peri_def_para                (UnsignedOpt ar_idx);
void        pndv_in_peri_dial_quit               (Unsigned16 slot, Unsigned8 ret_val_to_peri);
void        pndv_in_peri_xdial_quit              (Unsigned16 slot, Unsigned8 ret_val_to_peri);
void        pndv_in_peri_generic_dial_quit       (Unsigned16 interface_idx, Unsigned8 ret_val_to_peri);
void        pndv_in_peri_pral_quit               (Unsigned16 slot, Unsigned8 ret_val_to_peri);
void        pndv_in_peri_upal_quit               (Unsigned16 slot);
void        pndv_in_peri_ural_quit               (Unsigned16 slot);
void        pndv_in_peri_stal_quit               (Unsigned16 entity_nr, Unsigned8 ret_val_to_peri);
void        pndv_in_peri_ros_al_quit             (Unsigned16 slot);
void        pndv_in_peri_pull_al_quit            (Unsigned16 slot);
void        pndv_in_peri_plug_al_quit            (Unsigned16 slot, UnsignedOpt line);
void        pndv_in_peri_write_rema_station_prm_data (void);


/* pndv_ds.c */

void        pndv_in_write_record                 (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_read_record                  (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
Unsigned8   pndv_in_store_para                   (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_in_dfc_rd_io_record_fill        (Unsigned8 *data_ptr, UnsignedOpt data_len, CM_UPPER_RQB_PTR_TYPE rqb_ptr, Unsigned8 *ioxs_ptr);
void        pndv_in_dfc_rd_io_record_done        (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
UnsignedOpt pndv_in_ds_build_rd_input_data       (Unsigned8  *src_data_ptr,
                                                  UnsignedOpt netto_src_data_len,
                                                  Unsigned16  slot,
                                                  Unsigned16  subslot,
                                                  UnsignedOpt ar_idx,
                                                  Unsigned8  *ioxs_ptr);
UnsignedOpt pndv_in_ds_build_rd_output_data      (Unsigned8  *src_data_ptr,
                                                  UnsignedOpt netto_src_data_len,
                                                  Unsigned16  slot,
                                                  Unsigned16  subslot,
                                                  UnsignedOpt ar_idx,
                                                  Unsigned8  *ioxs_ptr);

void        pndv_in_ds_rd_input_data_init        (void);
void        pndv_in_ds_rd_output_data_init       (void);
Unsigned16  pndv_ds_get_slot_nr                  (CM_UPPER_SV_RECORD_PTR_TYPE record_ptr);
Unsigned16  pndv_ds_get_subslot_nr               (CM_UPPER_SV_RECORD_PTR_TYPE record_ptr);
void        pndv_in_cm_read_write_ind            (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_set_rqb_err                     (CM_UPPER_RQB_PTR_TYPE rqb_ptr, Unsigned8 err_code);


/* pndv_io.c */

void        pndv_in_ar_info                      (CM_UPPER_RQB_PTR_TYPE rqb_ptr);


void pndv_sm(PNDV_SM_EVENT_E_T event);

/* pndv_ar.c */

void                            pndv_ar_sm                                      (Unsigned32 ar_idx, PNDV_AR_SM_EVENT_E_T event);
void                            pndv_ar_sr_sm                                   (Unsigned32 sr_set_idx, PNDV_SR_EVENT_T event, Unsigned32 ar_idx);
void                            pndv_ar_cm_connect_ind                          (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void                            pndv_ar_peri_connect_ind_done                   (Unsigned16 ar_idx, Unsigned16 response);
PNDV_CONNECT_IND_RET_VAL_T      pndv_ar_tool_connect_ind_find_ar_idx            (UnsignedOpt *ar_idx_ptr, CM_UPPER_EVENT_PTR_TYPE event_ptr);
void                            pndv_ar_tool_connect_ind_store_ar_data          (UnsignedOpt ar_idx, CM_UPPER_EVENT_PTR_TYPE event_ptr);
PNDV_CONNECT_IND_RET_VAL_T      pndv_ar_tool_connect_ind_handle_iocr            (UnsignedOpt ar_idx, Unsigned16 ar_set_nr, Unsigned16 session_key, CM_UPPER_EVENT_PTR_TYPE event_ptr);
void                            pndv_ar_tool_connect_ind_process_parked         (void);
void                            pndv_ar_sys_connect_ind_timeout                 (void);
void                            pndv_ar_cm_ar_disconnect_ind                    (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void                            pndv_ar_peri_disconnect_ind_done                (Unsigned8 ar_idx);
void                            pndv_ar_tool_disconnect_ind_control_bf_led      ( void );
void                            pndv_ar_tool_disconnect_ind_cleanup             (Unsigned32 ar_idx);
void                            pndv_ar_tool_disconnect_ind_reset_set_cfg       ( Unsigned32 ar_nr );
void                            pndv_ar_init_set_cfg                            ( void );
UnsignedOpt                     pndv_ar_abort_req                               ( UnsignedOpt ar_idx, UnsignedOpt call_line);
void                            pndv_ar_cm_ar_abort_done                        (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void                            pndv_ar_abort_shared_ar                         (void);
void                            pndv_ar_cm_sv_ownership_ind                     ( CM_UPPER_RQB_PTR_TYPE rqb_ptr );
Unsigned32                      pndv_ar_tool_ownership_ind_check_plausibility   (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void                            pndv_ar_peri_ownership_ind_done                 (Unsigned8 ar_idx, Unsigned16 entity_nr, Boolean more_follows);
void                            pndv_ar_peri_ownership_ind_done_check_pdev      (Unsigned8 ar_idx, CM_UPPER_SV_AR_OWNERSHIP_PTR_TYPE local_ar_ownership);
void                            pndv_ar_cm_sv_event_prm_end_ind                 (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void                            pndv_ar_peri_prm_end_ind_done                   (Unsigned8 ar_idx, Unsigned16 entity_nr, Boolean more_follows);
void                            pndv_ar_peri_prm_end_ind_done_continue          (Unsigned8 ar_idx, Unsigned16 slot_nr, Unsigned16 subslot_nr, Boolean more_follows);
void                            pndv_ar_peri_ready_for_input_update_done        (Unsigned8 ar_idx);
void                            pndv_ar_do_appl_ready_req                       ( UnsignedOpt ar_idx );
void                            pndv_ar_cm_sv_appl_ready_done                   (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void                            pndv_ar_cm_ar_in_data_ind                       (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void                            pndv_ar_peri_in_data_done                       (Unsigned8 ar_idx, Unsigned16 entity_nr, Boolean more_follows);
void                            pndv_ar_cm_sv_event_ar_rir_ind                  (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void                            pndv_ar_do_rir_ack_req                          (UnsignedOpt ar_idx);
void                            pndv_ar_cm_sv_rir_ack_cnf                       (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void                            pndv_ar_peri_sr_rdht_timeout                    (Unsigned32 ar_idx);
void                            pndv_ar_cm_sr_rdht_timeout_cnf                  (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void                            pndv_ar_peri_sr_edge_indication                 (Unsigned32 ar_idx, UnsignedOpt is_primary);
void                            pndv_ar_peri_sr_owner_takeover_ind              (Unsigned32 ar_idx, UnsignedOpt is_owner);
void                            pndv_ar_cm_sr_ar_set_trigger_cnf                (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
UnsignedOpt                     pndv_ar_do_service_queue                        (CM_UPPER_RQB_PTR_TYPE rqb_ptr, PNDV_PERI_SERVICE_IDENT_T serv_id);
void                            pndv_ar_peri_service_get_next                   (CM_UPPER_RQB_PTR_TYPE *rqb_ptr_ptr, PNDV_PERI_SERVICE_IDENT_T serv_id);
UnsignedOpt                     pndv_ar_set_so_state                            ( Unsigned32 event, Boolean lock, Unsigned32 ar_idx );
Unsigned8                       pndv_get_num_connected_ars                      (void);
UnsignedOpt                     pndv_ar_get_ar_idx_by_ar_nr                     ( Unsigned32 ar_nr );
UnsignedOpt                     pndv_ar_get_ar_idx_by_session_key               ( Unsigned16 session_key );
UnsignedOpt                     pndv_ar_get_sr_idx_by_ar_set_nr                 ( Unsigned16 sr_set_nr);

/* pndv_pp.c */

void        pndv_pp_sm                                  (PNDV_STRUCT_CFG_SUBMODULE_RES_PTR_T res_ptr, PNDV_CFG_SUBMODULE_RES_EVENT_T event);
void        pndv_pp_peri_submodule_remove               (Unsigned16 entity, Boolean more_follows);
void        pndv_pp_cm_submodule_remove_done            (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_pp_peri_submodule_add                  (Unsigned16 entity, Boolean more_follows);
void        pndv_pp_cm_submodule_add_done               (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_pp_cm_pd_submodule_add_done            (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_pp_tool_submodule_add_remove_do_request(void);
Boolean     pndv_pp_peri_check_trigger_cm_add_remove    (Boolean more_follows);
UnsignedOpt pndv_pp_tool_check_sub_is_local_pd          (PNDV_STRUCT_CFG_SUBMODULE_RES_PTR_T res_ptr);

#ifdef __cplusplus
}
#endif

/*****************************************************************************/
/* reinclude-protection */


#else
    #pragma message ("The header PNDV_INT.H is included twice or more !")
#endif


/*****************************************************************************/
/*  end of file.                                                             */
/*****************************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
