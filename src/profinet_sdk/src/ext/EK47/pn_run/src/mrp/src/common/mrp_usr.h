#ifndef MRP_USR_H
#define MRP_USR_H

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
/*  C o m p o n e n t     &C: MRP (Media Redundancy Protocol)           :C&  */
/*                                                                           */
/*  F i l e               &F: mrp_usr.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  user interface                                                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef MRP_MESSAGE
/*  2008-02-29  mk    srv MRP_OPC_PRM_READ_ADMIN_DATA added.                 */
/*                    mrp_request_synchron added.                            */
/*  2008-10-20  mk    new diag state for service                             */
/*                    MRP_OPC_STATE_INDICATION: MRP_RED_DIAG_ROLE_MORPHING   */ 
/*  2008-10-28  mk    rename ROLE_MORPHING in MORPHING                       */
/*  2009-07-02  ds    include hierarchy changed                              */
/*  2009-10-15  ds    minor cleanups                                         */
/*  2010-05-25  ds    new data structure MRP_OPC_RINGSTATE_INDICATION_TYPE,  */
/*                    new opcode MRP_OPC_RINGSTATE_INDICATION                */
/*  2010-06-01  ds    structures and types for opcodes MRP_OPC_GET_STATUS and*/
/*                    MRP_OPC_SET_STATUS defined                             */
/*  2010-07-19  ds    new feature MRP MIB implemented according to           */
/*                    IEC 62439-2                                            */
/*  2010-07-22  ds    N interfaces: Upper callbacks called within macro, to  */
/*                    be able to operate in different systems                */
/*  2010-12-21  ds    N-IF: channel handling redesigned                      */
/*  2013-11-28  mk    RQ827954: erroneous connected ring line detection      */
/*  2016-10-21  ds    Replace LSA_BOOL by LSA_UINT8 at USR-IF                */
/*  2018-10-19  hh    introduction of N MRP interconnection instances        */
/*                                                                           */
#endif
/*****************************************************************************/


/*---------------------------------------------------------------------------*/
/*  OPCODES                                                                  */
/*---------------------------------------------------------------------------*/

#define MRP_OPC_OPEN_CHANNEL             1
#define MRP_OPC_CLOSE_CHANNEL            2
#define MRP_OPC_INTERNAL_TIMER           3 /* for internal use only ! */

#define MRP_OPC_PRM_PREPARE              4
#define MRP_OPC_PRM_WRITE                5
#define MRP_OPC_PRM_END                  6
#define MRP_OPC_PRM_COMMIT               7
#define MRP_OPC_PRM_READ                 8
#define MRP_OPC_PRM_INDICATION           9
#define MRP_OPC_PRM_CHANGE_PORT         10
#define MRP_OPC_PRM_READ_ADMIN_DATA     11

#define MRP_OPC_STATE_INDICATION        12
#define MRP_OPC_RINGSTATE_INDICATION    13
#define MRP_OPC_GET_STATUS              14
#define MRP_OPC_SET_STATUS              15
#define MRP_OPC_RED_STATE_INDICATION    16
/* MRP_RSTP+: trigger an MRP interconnection topology change frame */
#define MRP_OPC_RED_IN_TC_REQUEST       17
#define MRP_OPC_RED_IN_EVENT_INDICATION 18

#ifdef MRP_CFG_PLUGIN_2_HSR
#define MRP_OPC_IND_PROVIDE             21
#endif /* MRP_CFG_PLUGIN_2_HSR */

/*---------------------------------------------------------------------------*/
/*                                 defines                                   */
/*---------------------------------------------------------------------------*/

#define MRP_MAX_USER_CHANNELS_PER_IF      (MRP_MAX_CHANNELS_PER_IF - 1) /* at least one channel has to be system channel to EDD */

/*  operation states */
#define MRP_OPERATION_OFF                       0
#define MRP_OPERATION_ON                        1

/*  diag values for MULTIPLE_OTHER_MRMs */
#define MRP_DIAG_MULTIPLE_OTHER_RM_NO           0
#define MRP_DIAG_MULTIPLE_OTHER_RM_YES          1

/* operation states for MRP interconnection */
#define MRP_IN_OPERATION_OFF                    0
#define MRP_IN_OPERATION_ON                     1

#define MRP_MIB_DEFAULT_DOMAIN_NAME_SIZE_MAX    240

/*---------------------------------------------------------------------------*/
/*  ERROR_CODES                                                              */
/*---------------------------------------------------------------------------*/
#define MRP_RET_OK                        LSA_OK
#define MRP_RET_ERR                       (LSA_RET_ERR_OFFSET) /* 0xC0 */

#define MRP_RSP_OK                        LSA_OK
#define MRP_RSP_ERR_RESOURCE              LSA_RET_ERR_RESOURCE
#define MRP_RSP_ERR_PARAM                 LSA_RET_ERR_PARAM
#define MRP_RSP_ERR_SEQUENCE              LSA_RET_ERR_SEQUENCE
#define MRP_RSP_ERR_SYS_PATH              LSA_RET_ERR_SYS_PATH

#define MRP_RSP_ERR                       (LSA_RET_ERR_OFFSET) /* 0xC0 */
#define MRP_RSP_ERR_PRM_DATA              (LSA_RET_ERR_OFFSET + 1) /* 0xC1 */
#define MRP_RSP_ERR_PRM_BLOCK             (LSA_RET_ERR_OFFSET + 2) /* 0xC2 */
#define MRP_RSP_ERR_PRM_INDEX             (LSA_RET_ERR_OFFSET + 3) /* 0xC3 */
#define MRP_RSP_ERR_PRM_CONSISTENCY       (LSA_RET_ERR_OFFSET + 4) /* 0xC4 */
#define MRP_RSP_ERR_PRM_PORTID            (LSA_RET_ERR_OFFSET + 5) /* 0xC5 */
#define MRP_RSP_ERR_PRM_NOT_CONFIGURED    (LSA_RET_ERR_OFFSET + 6) /* 0xC6 */

#define MRP_RSP_OK_CANCEL                 (LSA_RET_OK_OFFSET)

/*---------------------------------------------------------------------------*/
/*  common types/typedefs for MRP                                            */
/*---------------------------------------------------------------------------*/

#define MRP_UPPER_MEM_UINT8_PTR_TYPE    LSA_UINT8*

/*---------------------------------------------------------------------------*/
/* upper CBF definitions                                                     */
/*---------------------------------------------------------------------------*/

/* open/close/request CBF */
typedef  LSA_VOID  LSA_FCT_PTR (/*ATTR*/, MRP_UPPER_CALLBACK_FCT_PTR_TYPE)(MRP_UPPER_RQB_PTR_TYPE  upper_rqb_ptr);

/*---------------------------------------------------------------------------*/
/* channel states (any kind of system or user channel)                       */
/*---------------------------------------------------------------------------*/
typedef enum 
{
    MRP_CH_STATE_FREE,
    MRP_CH_STATE_RESERVED,
    MRP_CH_STATE_READY,
    MRP_CH_STATE_CLOSING
}MRP_CH_STATE;


/*---------------------------------------------------------------------------*/
/* struct MRP_CH_USR_DB_TYPE                                                 */
/*---------------------------------------------------------------------------*/
typedef struct 
{
    LSA_VOID *pUsr;
    LSA_VOID *pRec;
    LSA_VOID *pOha;
}MRP_CH_USR_DB_TYPE;


#include "sys_pck1.h"
/*---------------------------------------------------------------------------*/
/*  struct for every MRP channel type: MRP_HDB_TYPE                          */
/*---------------------------------------------------------------------------*/
typedef struct 
{
    MRP_CH_STATE                    state;            /* internal state */
    LSA_HANDLE_TYPE                 myHandle;         /* own MRP handle <=> array index of channel table */
    LSA_HANDLE_TYPE                 handle_upper;     /* handle of upper component         */
    EDD_HANDLE_LOWER_TYPE           handle_lower_edd; /* the handle of lower EDD component */
    MRP_UPPER_CALLBACK_FCT_PTR_TYPE pCbf;             /* function pointer of user callback-function*/
    LSA_SYS_PTR_TYPE                sysptr;           /* lsa specifically, must match with lower->sysptr */
    MRP_DETAIL_PTR_TYPE             detailptr;        /* pointer to channel-details */
    LSA_SYS_PATH_TYPE               sys_path;         /* this target used by system */
    MRP_UPPER_RQB_PTR_TYPE          pRQB;             /* queued requests */

    MRP_CH_SYS_DB_TYPE             *pChSysDB;         /* consider mrp_open_channel from now on !!! */
                                                      /* any USR- and SYS-channel has a reference to MRP_CH_SYS_DB_TYPE */
    MRP_CH_USR_DB_TYPE             *pChUsrDB;         /* consider mrp_open_channel from now on !!! */
                                                      /* only USR-channels have a reference to MRP_CH_USR_DB_TYPE       */
}MRP_HDB_TYPE;


/*---------------------------------------------------------------------------*/
/*  MRP channel types                                                        */
/*---------------------------------------------------------------------------*/
typedef MRP_HDB_TYPE *MRP_CHANNEL_TYPE_COMMON;  /* for System- or User-Channel */
typedef MRP_HDB_TYPE *MRP_CHANNEL_TYPE_USER;    /* for User-Channel */
typedef MRP_HDB_TYPE *MRP_CHANNEL_TYPE_SYSTEM;  /* for System-Channel */

typedef MRP_HDB_TYPE const *MRP_CONST_CHANNEL_TYPE_COMMON;  /* for System- or User-Channel */
typedef MRP_HDB_TYPE const *MRP_CONST_CHANNEL_TYPE_USER;    /* for User-Channel */
typedef MRP_HDB_TYPE const *MRP_CONST_CHANNEL_TYPE_SYSTEM;  /* for System-Channel */

/*===========================================================================*/
/*                                 USR RQ mapping type                       */
/*===========================================================================*/
typedef LSA_UINT16 LSA_FCT_PTR(/*ATTR*/, MRP_USR_RQ_FCT_PTR_TYPE)(const MRP_CHANNEL_TYPE_USER, MRP_UPPER_RQB_PTR_TYPE);

typedef struct 
{
  MRP_USR_RQ_FCT_PTR_TYPE  pUserReqCbf;
  LSA_OPCODE_TYPE          opcode;
  LSA_UINT16               pathType;
}MRP_USR_RQ_MAPPING_TBL_TYPE;


/*---------------------------------------------------------------------------*/
/*  global MRP cyclic component timer                                        */
/*---------------------------------------------------------------------------*/
typedef struct  
{
  LSA_UINT32                 currTicks;

  LSA_UINT16                 timerId;
  LSA_USER_ID_TYPE           uid;
  
  LSA_UINT16                 type;
  MRP_TIMER_STS_T            state;
  LSA_UINT32                 durationTicks;
} MRP_BASE_TIMER_PARA_DB_TYPE;

/*---------------------------------------------------------------------------*/
/*  global MRP database structure                                            */
/*---------------------------------------------------------------------------*/
typedef struct  
{
  LSA_UINT8               init;                                   /* init flag */
  MRP_HDB_TYPE            channels[MRP_MAX_CHANNELS];            /* list of all LSA channels to/from LSA-MRP */

  MRP_CHANNEL_TYPE_SYSTEM pChSys[MRP_MAX_NO_OF_INTERFACES];      /* array of pointers to system channels */
  MRP_CHANNEL_TYPE_USER   pChPrmUsr[MRP_MAX_NO_OF_INTERFACES];   /* array of pointers to prm user channels */
  MRP_CHANNEL_TYPE_USER   pChUsr[MRP_MAX_NO_OF_INTERFACES];      /* array of pointers to ordinary user channels */
  MRP_CHANNEL_TYPE_USER   pChPrivUsr[MRP_MAX_NO_OF_INTERFACES];  /* array of pointers to private user channels */

  LSA_UINT16                  usrReqCnt;                          /* max. number of applicable user requests of different types */
  MRP_BASE_TIMER_PARA_DB_TYPE cyclicTimer;                        /* cyclic MRP component timer data */
  MRP_USR_RQ_MAPPING_TBL_TYPE *pUsrReqMap;                        /* pointer to list of allowed user request types */
}MRP_DB_TYPE;
#include "sys_unpck.h"


#define MRP_INIT_COMPLETED       1
#define MRP_INIT_NOT_COMPLETED   0


/*---------------------------------------------------------------------------*/
/*                            forward declaration                            */
/*---------------------------------------------------------------------------*/

LSA_EXTERN MRP_DB_TYPE             g_Mrp;


/*---------------------------------------------------------------------------*/
/*  MRP common types                                                         */
/*---------------------------------------------------------------------------*/

typedef LSA_UINT8 * MRP_COMMON_MEM_U8_PTR_TYPE;


#ifdef MRP_CFG_PLUGIN_2_HSR
/*===========================================================================*/
/*  PLUGIN HSR                                                               */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/*  Opcode: MRP_OPC_IND_PROVIDE                                              */
/*---------------------------------------------------------------------------*/
/*  manager states */
#define MRP_RED_STATE_OFF                          0
#define MRP_RED_STATE_ACTIVE                       1
#define MRP_RED_STATE_PASSIVE                      2 

/*  MRP diagnosis states for RM  */
/*  values are bitmasks because states can occur together !  */
#define MRP_RED_DIAG_RM_OTHER                      0x00000001U
#define MRP_RED_DIAG_RM_SINGLE_SIDE_RCV            0x00000002U
#define MRP_RED_DIAG_RM_ECORI_DE                   0x00000004U

/*  stdby state values  */
#define MRP_RED_STATE_STBY_OFF                     0
#define MRP_RED_STATE_STBY_MASTER_ACTIVE           1
#define MRP_RED_STATE_STBY_MASTER_PASSIVE          2
#define MRP_RED_STATE_STBY_MASTER_WAITING          3
#define MRP_RED_STATE_STBY_SLAVE_ACTIVE            4
#define MRP_RED_STATE_STBY_SLAVE_PASSIVE           5
#define MRP_RED_STATE_STBY_SLAVE_WAITING           6

/*  stdby diagnosis states  */
/*  values are bitmasks because states can occur together ! */
#define MRP_RED_DIAG_STBY_PARTNER_ALIVE            0x00000001U
#define MRP_RED_DIAG_STBY_PARTNER_AMBIGUOUS        0x00000002U
#define MRP_RED_DIAG_STBY_VERSION_MISMATCH         0x00000004U


#endif /* MRP_CFG_PLUGIN_2_HSR */

/*===========================================================================*/
/*  PLUGIN HSR Opcode: MRP_OPC_IND_PROVIDE                                   */
/*===========================================================================*/

#include "sys_pck1.h"
typedef struct 
{
  LSA_UINT16     hsrInstance;   /* this parameter is newly introduced to separate one HSR instance and has to be set to 1 */

  LSA_UINT16     stdbyState;
  LSA_UINT16     stdbyDiag;
  LSA_UINT16     stdbyPartnerPort;
  LSA_UINT8      stdbyPartnerID[6];
  LSA_UINT16     stdbyChangeOver;

  LSA_UINT16     mgrState;
  LSA_UINT16     ssrAddInfo;   /* additional information in case of single side receive -> RPort, which doesn't receive HRP test frames */
  LSA_UINT16     mgrDiag;
  LSA_UINT8      mgrDiagMAC[6];
  LSA_UINT16     res1;

}MRP_OPC_IND_PROVIDE_TYPE;

typedef MRP_OPC_IND_PROVIDE_TYPE *MRP_OPC_IND_PROVIDE_PTR_TYPE;

/*---------------------------------------------------------------------------*/
/*  Opcode: MRP_OPC_OPEN_CHANNEL                                             */
/*---------------------------------------------------------------------------*/
#define MRP_INVALID_HANDLE  0xFF

typedef struct 
{
  LSA_HANDLE_TYPE                   handle;           /* the new open handle from mrp, input for all other requests */
  LSA_HANDLE_TYPE                   handle_upper;   /* every confirmation of an request set this handle to rqb.handle */
  LSA_SYS_PATH_TYPE             sys_path;         /* this target used by system */
  MRP_UPPER_CALLBACK_FCT_PTR_TYPE pCbf;   /* the callback-function */
}MRP_OPEN_CHANNEL_TYPE;

typedef MRP_OPEN_CHANNEL_TYPE *MRP_UPPER_OPEN_CHANNEL_PTR_TYPE;


/*---------------------------------------------------------------------------*/
/*  Opcode: MRP_OPC_PRM_PREPARE                                              */
/*---------------------------------------------------------------------------*/
typedef struct 
{
  LSA_INT    dummy;
}MRP_PRM_PREPARE_TYPE;

typedef MRP_PRM_PREPARE_TYPE *MRP_PRM_PREPARE_PTR_TYPE;

/*---------------------------------------------------------------------------*/
/*  Opcode: MRP_OPC_PRM_WRITE                                                */
/*---------------------------------------------------------------------------*/
typedef struct 
{
  LSA_UINT16    slot_number;
  LSA_UINT16    subslot_number;
  LSA_UINT16    edd_port_id;
  LSA_UINT32    record_index;
  LSA_UINT32    record_data_length;
  MRP_UPPER_MEM_UINT8_PTR_TYPE record_data;
  LSA_UINT32    err_offset;
}MRP_PRM_WRITE_TYPE;

typedef MRP_PRM_WRITE_TYPE *MRP_PRM_WRITE_PTR_TYPE;


#define MRP_PORT_IS_NOT_MODULAR  0
#define MRP_PORT_IS_MODULAR      1

/*---------------------------------------------------------------------------*/
/*  Opcode: MRP_OPC_PRM_END                                                  */
/*---------------------------------------------------------------------------*/
typedef struct 
{
  LSA_UINT8     isModularPort[EDD_CFG_MAX_PORT_CNT];

  LSA_UINT16    err_port_id;
  LSA_UINT32    err_index;
  LSA_UINT32    err_offset;
  LSA_UINT8     err_fault;

  LSA_UINT8     portparamsNotApplicable[EDD_CFG_MAX_PORT_CNT];
}MRP_PRM_END_TYPE;

typedef MRP_PRM_END_TYPE *MRP_PRM_END_PTR_TYPE;


#define MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS    1
#define MRP_PRM_ERR_FAULT_NO_OR_TOO_LESS_PARAMS       0

/* error defaults for PRM_WRITE and PRM_END services */
#define MRP_PRM_ERR_PORT_ID_DEFAULT                     0
#define MRP_PRM_ERR_INDEX_DEFAULT                       0xFFFF
#define MRP_PRM_ERR_OFFSET_DEFAULT                    0
#define MRP_PRM_ERR_FAULT_DEFAULT                       MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS


/*---------------------------------------------------------------------------*/
/*  Opcode: MRP_OPC_PRM_COMMIT                                               */
/*---------------------------------------------------------------------------*/
typedef struct 
{
  LSA_UINT8    applyDefaultPortparams[EDD_CFG_MAX_PORT_CNT];
}MRP_PRM_COMMIT_TYPE;

typedef MRP_PRM_COMMIT_TYPE *MRP_PRM_COMMIT_PTR_TYPE;

/*---------------------------------------------------------------------------*/
/*  Opcode: MRP_OPC_PRM_READ                                                 */
/*          MRP_OPC_PRM_READ_ADMIN_DATA                                      */
/*---------------------------------------------------------------------------*/
typedef struct 
{
  LSA_UINT16  slot_number;
  LSA_UINT16  subslot_number;
  LSA_UINT16  edd_port_id;
  LSA_UINT32  record_index;
  LSA_UINT32  record_data_length;
  MRP_UPPER_MEM_UINT8_PTR_TYPE record_data;
}MRP_PRM_READ_TYPE;

typedef MRP_PRM_READ_TYPE *MRP_PRM_READ_PTR_TYPE;



/* defines for the state of the port module (=modul_state) */
#define MRP_PRM_PORTMODULE_PLUG_PREPARE     1
#define MRP_PRM_PORTMODULE_PLUG_COMMIT      2
#define MRP_PRM_PORTMODULE_PULLED           3

/* defines for apply_default_portparams */
#define MRP_PRM_APPLY_RECORD_PORT_PARAMS             0
#define MRP_PRM_APPLY_DEFAULT_PORT_PARAMS            1

/* defines for port_params_not_applicable*/
#define MRP_PRM_PORT_PARAMS_APPLICABLE         0
#define MRP_PRM_PORT_PARAMS_NOT_APPLICABLE     1

/*---------------------------------------------------------------------------*/
/*  Opcode: MRP_OPC_PRM_CHANGE_PORT                                          */
/*---------------------------------------------------------------------------*/
typedef struct 
{
  LSA_UINT16  edd_port_id;
  LSA_UINT8   module_state;
  LSA_UINT8   apply_default_portparams;
  LSA_UINT8   port_params_not_applicable;
}MRP_PRM_CHANGE_PORT_TYPE;

typedef MRP_PRM_CHANGE_PORT_TYPE *MRP_PRM_CHANGE_PORT_PTR_TYPE;

/*---------------------------------------------------------------------------*/
/*  Opcode: MRP_OPC_GET_STATUS                                               */
/*---------------------------------------------------------------------------*/

typedef enum
{
  MRP_MIB_DOMAIN_ROLE_DISABLE,
  MRP_MIB_DOMAIN_ROLE_CLIENT,
  MRP_MIB_DOMAIN_ROLE_MANAGER,
  MRP_MIB_DOMAIN_ROLE_AUTO_MANAGER_COMP
}MRP_MIB_ENUM_DOMAIN_ROLE;

typedef enum
{
  MRP_MIB_RPORT_STATE_MRP_OFF             = 0,   /* ringport state in MRP-MIB, if MRP is deactivated */
  MRP_MIB_RPORT_STATE_DISABLED            = 1,   /* port is disabled (only applicable, if port is connected) */
  MRP_MIB_RPORT_STATE_BLOCKED             = 2,   /* port is BLOCKED (only applicable, if port is connected) */
  MRP_MIB_RPORT_STATE_FORWARDING          = 3,   /* port is in FORWARDING state (only applicable, if port is connected) */
  MRP_MIB_RPORT_STATE_NOT_CONNECTED       = 4,   /* port is NOT connected (has LINK_DOWN) */

  /* INIT/DEINIT settings of MRP port states */
  MRP_MIB_RPORT_STATE_UNCHANGED           = 254, /* port state isn't affected directly by deinit of MRP, but will be changed to */
                                                 /* FORWARDING afterwards, should never be visible in MRP MIB variable          */                                             
  MRP_MIB_RPORT_STATE_UNKNOWN             = 255  /* during init of MRP, port has an unknown state temporarily, which couldn't be mapped */
}MRP_MIB_ENUM_RPORT_STATE;


typedef enum
{
  MRP_MIB_DOMAIN_STATE_RINGCLOSED         = 0x0000,   /* MRP ring is ok and closed */

  MRP_MIB_DOMAIN_STATE_DISABLED           = 0x0001,   /* MRP is switched off */
  MRP_MIB_DOMAIN_STATE_UNDEFINED          = 0x0002,   /* from MRP client's point of view, the ringstate is UNDEFINED */
  MRP_MIB_DOMAIN_STATE_RINGOPEN           = 0x0004,   /* MRP ring redundancy lost */
  MRP_MIB_DOMAIN_STATE_RESERVED           = 0x0008    /* currently not set, because NO further extension */
}MRP_MIB_ENUM_DOMAIN_STATE;

typedef enum
{
  MRP_MIB_DOMAIN_NO_ERR                   = 0x0001,   /* Bit 0=1: operational state == admin state */
                                                      
  MRP_MIB_DOMAIN_ERR_INVALID_VLANID       = 0x0002,   /* Bit 1 */
  MRP_MIB_DOMAIN_ERR_INVALID              = 0x0004,   /* Bit 2 */
  MRP_MIB_DOMAIN_ERR_MULTIPLE_MRM         = 0x0008,   /* Bit 3 */
  MRP_MIB_DOMAIN_ERR_SINGLE_SIDE_RECEIVE  = 0x0010    /* Bit 4 */
}MRP_MIB_ENUM_DOMAIN_ERROR;


typedef struct _MRP_GET_STATUS_TYPE
{
  /* IN parameter */
  /* identification of one specific MRP instance  */
  LSA_UINT16                                mrpInstance;                /* MRP instance within one interface */
                                                                        /* (valid range 1 ... EDD_CFG_MAX_MRP_INSTANCE_CNT) */
  /* IN/OUT parameter */
  MRP_COMMON_MEM_U8_PTR_TYPE                pDomainName;                /* IN/OUT parameter */
  LSA_UINT32                                domainNameLength;           /* IN/OUT parameter */
  
  /* OUT parameter */
  MRP_MIB_ENUM_DOMAIN_ROLE                  operRole;
  LSA_UINT16                                rPort1;                      
  LSA_UINT16                                rPort2;                      
  MRP_MIB_ENUM_RPORT_STATE                  rPort1State;
  MRP_MIB_ENUM_RPORT_STATE                  rPort2State;

  LSA_UINT16                                domainState;                /* see enum MRP_MIB_ENUM_DOMAIN_STATE */
  LSA_UINT16                                domainError;                /* see enum MRP_MIB_ENUM_DOMAIN_ERROR, values may be or-ed */
  
  LSA_UINT32                                domainRingOpenCount;        /* only applicable if MRP manager role */
  LSA_UINT32                                domainLastRingOpenChange;   /* only applicable if MRP manager role */
  LSA_UINT32                                domainRoundTripDelayMax;    /* only applicable if MRP manager role */
  LSA_UINT32                                domainRoundTripDelayMin;    /* only applicable if MRP manager role */
  LSA_UINT32                                domainResetRoundTripDelays; /* only applicable if MRP manager role */

}MRP_GET_STATUS_TYPE;

typedef MRP_GET_STATUS_TYPE *MRP_GET_STATUS_PTR_TYPE;

/*---------------------------------------------------------------------------*/
/*  Opcode: MRP_OPC_SET_STATUS                                               */
/*---------------------------------------------------------------------------*/
typedef struct _MRP_SET_STATUS_TYPE
{
  /* IN parameter */
  /* identification of one specific MRP instance  */
  LSA_UINT16                                    mrpInstance;                /* MRP instance within one interface */
                                                                            /* (valid range 1 ... EDD_CFG_MAX_MRP_INSTANCE_CNT) */
  LSA_UINT32                                    domainResetRoundTripDelays; /* reset any delays = 1 */
}MRP_SET_STATUS_TYPE;

typedef MRP_SET_STATUS_TYPE *MRP_SET_STATUS_PTR_TYPE;


/*---------------------------------------------------------------------------*/
/* struct MRP_IN_TC_REQUEST_TYPE                                             */
/*---------------------------------------------------------------------------*/

/* Defines for different kinds of originators */
#define MRP_IN_TC_ORIGINATOR_DEFAULT                         0
#define MRP_IN_TC_ORIGINATOR_RSTP                            1
#define MRP_IN_TC_ORIGINATOR_OTHER                           2

/* Defines for reasons to request an MRP interconnection topology change request */
#define MRP_IN_TC_REASON_DEFAULT                             0
#define MRP_IN_TC_REASON_INIT                                1
#define MRP_IN_TC_REASON_RSTP_RECONFIG                       2
#define MRP_IN_TC_REASON_OTHER                               3


typedef struct _MRP_IN_TC_REQUEST_TYPE
{
  /* IN parameter */
  /* identification of one specific MRP instance  */
  LSA_UINT16                   mrpInstance;            /* MRP instance within one interface  */
                                                       /* (valid range 1 ... EDD_CFG_MAX_MRP_INSTANCE_CNT)   */
                                                       /* has to be always 1 for RSTP+ applications */
  LSA_UINT16                   inId;
  LSA_UINT16                   originator;
  LSA_UINT16                   origPort;
  LSA_UINT16                   reason;

}MRP_IN_TC_REQUEST_TYPE;


/*---------------------------------------------------------------------------*/
/* struct MRP_OPC_RED_IN_EVENT_INDICATION_TYPE                               */
/*---------------------------------------------------------------------------*/

/* Defines for opcode MRP_OPC_RED_IN_EVENT_INDICATION, structure element inDiag */
/* Diagnostic events for MRP interconnection (bit mask), events can be O_RED:   */
#define MRP_IN_RED_DIAG_IN_OPEN_SEC_MD       0x00000001U
#define MRP_IN_RED_DIAG_IN_OPEN_SEC_ERR      0x00000002U
#define MRP_IN_RED_DIAG_IN_OPEN_PRM_MD       0x00000010U
#define MRP_IN_RED_DIAG_IN_OPEN_PRM_ERR      0x00000020U
#define MRP_IN_RED_DIAG_IN_MIC_MISS_SEC      0x00000100U
#define MRP_IN_RED_DIAG_IN_MIC_MISS_PRM      0x00000200U
#define MRP_IN_RED_DIAG_IN_MIC_MULTIPLE      0x00000400U
#define MRP_IN_RED_DIAG_IN_MIM_ROLE_FAIL     0x00001000U
#define MRP_IN_RED_DIAG_IN_MIM_RECONFIG_REJ  0x00002000U
#define MRP_IN_RED_DIAG_IN_MIM_MULTIPLE      0x00004000U

#define MRP_IN_RED_EVT_PARTNER_ROLE_MISMATCH 0x10000000U /* Role mismatch */
#define MRP_IN_RED_EVT_CFM_DISABLED          0x20000000U /* CFM disabled */
#define MRP_IN_RED_EVT_INID_MISMATCH         0x40000000U /* InID mismatch */
#define MRP_IN_RED_EVT_IPORT_MISMATCH        0x80000000U /* Port mismatch */

#define MRP_IN_RED_DIAG_IN_NONE              0x00000000U
#define MRP_IN_RED_DIAG_IN_ALL               0xFFFFFFFFU

#define MRP_IN_RED_EVT_PARA_DONT_CARE        0xFFFF

typedef enum _MRP_IN_OPER_ROLE_ENUM_T
{
  MRP_IN_OPER_ROLE_OFF  = 0x0000
 ,MRP_IN_OPER_ROLE_MIC  = 0x0001
 ,MRP_IN_OPER_ROLE_MIM  = 0x0002
} MRP_IN_OPER_ROLE_ENUM_T;


typedef struct _MRP_OPC_RED_IN_EVENT_INDICATION_TYPE
{
  /* IN/OUT parameter */
  /* identification of one specific MRP instance */
  LSA_UINT16              mrpInstance;
  /* MRP instance within one interface */
  LSA_UINT16              mrpInInstance;
  /* MRP interconnection instance within one interface */
  /* OUT parameter */
  LSA_UINT16              inId;
  LSA_UINT16              inPortId;
  MRP_IN_OPER_ROLE_ENUM_T inOperRole;
  /* optional for future releases: LSA_UINT16 inState; */
  /* report the current state of the MIM/MIC state machine */
  LSA_UINT32              inDiag;

  LSA_UINT16              inMICPosition;
  LSA_UINT16              inRxPort;
  LSA_UINT16              inRxInID;
  LSA_UINT16              inPeerOperRole;
  LSA_UINT16              inPeerMICPosition;

  LSA_UINT8               inPeerMacAddress[6];
} MRP_OPC_RED_IN_EVENT_INDICATION_TYPE;


/*---------------------------------------------------------------------------*/
/*  Opcode: MRP_OPC_PRM_INDICATION                                           */
/*---------------------------------------------------------------------------*/
#define MRP_PRM_INDICATION_DIAG_MAX_ENTRIES  20  /* 5 * 4: max. 5 diag alarms per instance and max. 4 MRP instances */

typedef struct _MRP_PRM_INDICATION_DIAG_ENTRY_TYPE
{
  LSA_UINT16 ChannelProperties;   /* ChannelProperties.Type_Bit0_7 := 0, */
                                  /* ChannelProperties.Accumulative_Bit8 := 0, */
                                  /* ChannelProperties.MaintenanceRequired_Bit9  := component specific, */
                                  /* ChannelProperties.MaintenanceDemanded_Bit10 := component specific, */
                                  /* ChannelProperties.Specifier_Bit11_12 := Appearing(1) or Disappearing(2) */
                                  /* ChannelProperties.Direction_Bit13_15 =: 0 */
  LSA_UINT16 ChannelErrorType;    /* 0x8002 */
  LSA_UINT16 ExtChannelErrorType; /* 0x8000..0x8003 */
  LSA_UINT32 ExtChannelAddValue;  /* 0 */
}MRP_PRM_INDICATION_DIAG_ENTRY_TYPE;

typedef struct _MRP_PRM_INDICATION_TYPE
{
  LSA_UINT16 edd_port_id;         /* 0 (=Interface) */
  LSA_UINT16 diag_cnt;            /* 0..MRP_PRM_INDICATION_DIAG_MAX_ENTRIES */
  MRP_PRM_INDICATION_DIAG_ENTRY_TYPE diag[MRP_PRM_INDICATION_DIAG_MAX_ENTRIES];
}MRP_PRM_INDICATION_TYPE;

typedef MRP_PRM_INDICATION_TYPE *MRP_PRM_INDICATION_PTR_TYPE;

/*---------------------------------------------------------------------------*/
/* Opcode: MRP_OPC_STATE_INDICATION                                          */
/*---------------------------------------------------------------------------*/
#define MRP_RED_DIAG_MRM_RING_OPEN        0x00000001U
#define MRP_RED_DIAG_MRM_OTHER            0x00000002U
#define MRP_RED_DIAG_MRM_SINGLE_SIDE_RCV  0x00000004U
#define MRP_RED_DIAG_MRM_MORPHING         0x00000008U
#define MRP_RED_DIAG_ROLE_MRM             0x00000010U
#define MRP_RED_DIAG_ROLE_MRC             0x00000020U
#define MRP_RED_DIAG_ROLE_HSR             0x00000040U
#define MRP_RED_DIAG_ROLE_OFF             0x00000080U
#define MRP_RED_DIAG_ROLE_MRM_AUTO        0x00010000U

/*---------------------------------------------------------------------------*/
/* struct MRP_OPC_STATE_INDICATION_TYPE                                      */
/*---------------------------------------------------------------------------*/
typedef struct _MRP_OPC_STATE_INDICATION_TYPE
{
  /* IN parameter */
  /* identification of one specific MRP instance  */
  LSA_UINT16 mrpInstance;                              /* MRP instance within one interface  */
                                                       /* (valid range 1 ... EDD_CFG_MAX_MRP_INSTANCE_CNT) */
  LSA_UINT16 ssrAddInfo;                                  /* additional information in case of single side receive -> RPort, which doesn't receive MRP test frames */
  LSA_UINT32 mrpDiag;
  /* additional information in case of other MRM detected. */
  LSA_UINT8 mrpDiagMACAddrOtherMRM[6];
  /* additional information in case of multiple other MRM detected. */
  LSA_UINT8 mrpDiagMultipleOtherMRMs;
}MRP_OPC_STATE_INDICATION_TYPE;



/*---------------------------------------------------------------------------*/
/* Opcode: MRP_OPC_RED_STATE_INDICATION                                      */
/*---------------------------------------------------------------------------*/

typedef enum _MRP_USR_ENUM_RED_STATE_TYPE
{
  MRP_USR_ENUM_STATE_OFF       = 0x0000
  
  ,MRP_USR_ENUM_STATE_MRC      = 0x0001
  ,MRP_USR_ENUM_STATE_MRM      = 0x0002
  ,MRP_USR_ENUM_STATE_AUTO_MRM = 0x0010
#ifdef MRP_CFG_PLUGIN_2_HSR
  ,MRP_USR_ENUM_STATE_HSR_RC   = 0x0100
  ,MRP_USR_ENUM_STATE_HSR_MGR  = 0x0200
  ,MRP_USR_ENUM_STATE_STBY     = 0x0400
  ,MRP_USR_ENUM_STATE_ARD      = 0x1000
#endif
  ,MRP_USR_ENUM_STATE_IN       = 0x4000 /* at least one MRP interconnection instance is available */

  ,MRP_USR_ENUM_STATE_ALL      = 0xFFFF
} MRP_USR_ENUM_RED_STATE_TYPE;


typedef enum _MRP_USR_ENUM_RED_IN_STATE_TYPE
{
  MRP_USR_ENUM_STATE_IN_OFF    = 0x0000

  ,MRP_USR_ENUM_STATE_IN_MIC   = 0x0001
  ,MRP_USR_ENUM_STATE_IN_MIM   = 0x0002

  ,MRP_USR_ENUM_STATE_IN_ALL   = 0xFFFF
} MRP_USR_ENUM_RED_IN_STATE_TYPE;


/*---------------------------------------------------------------------------*/
/* struct MRP_RED_IN_STATE_TYPE                                              */
/*---------------------------------------------------------------------------*/
typedef struct _MRP_RED_IN_STATE_TYPE
{
  LSA_UINT16 initState;     /* contains ORED enums of MRP_USR_ENUM_RED_IN_STATE_TYPE */
  LSA_UINT16 startState;    /* contains ORED enums of MRP_USR_ENUM_RED_IN_STATE_TYPE */
  LSA_UINT16 inId;          /* MRP interconnection ID belonging to one instance   */
  LSA_UINT16 inPortId;      /* MRP interconnection port belonging to one instance */

} MRP_RED_IN_STATE_TYPE;



/* maximum number of HRP/Standby ports */
#define MRP_MAX_NO_OF_STDBY_PORTS     5

/*---------------------------------------------------------------------------*/
/* struct MRP_OPC_RED_STATE_INDICATION_TYPE                                  */
/*---------------------------------------------------------------------------*/
typedef struct _MRP_OPC_RED_STATE_INDICATION_TYPE
{
  /* IN parameter */
  /* identification of one specific MRP instance  */
  LSA_UINT16                   mrpInstance;            /* MRP instance within one interface  */
                                                       /* (valid range 1 ... EDD_CFG_MAX_MRP_INSTANCE_CNT)   */
  LSA_UINT16                   initState;              /* contains ORED enums of MRP_USR_ENUM_RED_STATE_TYPE */
  LSA_UINT16                   startState;             /* contains ORED enums of MRP_USR_ENUM_RED_STATE_TYPE */
  LSA_UINT16                   rport1;
  LSA_UINT16                   rport2;

  LSA_UINT16                   stdbyPorts[MRP_MAX_NO_OF_STDBY_PORTS];
  /* To avoid compiler errors (EDD_CFG_MAX_MRP_IN_INSTANCE_CNT + 1): in case of EDD_CFG_MAX_MRP_IN_INSTANCE_CNT = 0 the array has at least the size of 1! Still all internal checks are done with EDD_CFG_MAX_MRP_IN_INSTANCE_CNT. */
  MRP_RED_IN_STATE_TYPE        inState[EDD_CFG_MAX_MRP_IN_INSTANCE_CNT + 1];

}MRP_OPC_RED_STATE_INDICATION_TYPE;


/*---------------------------------------------------------------------------*/
/* Opcode: MRP_OPC_RINGSTATE_INDICATION                                      */
/*---------------------------------------------------------------------------*/
typedef enum _MRP_USR_ENUM_TOPO_CTRL
{
  MRP_USR_ENUM_TOPO_CTRL_NONE       = 0,  /* former OHA_MRP_REMOVE */
  MRP_USR_ENUM_TOPO_CTRL_ADD,             /* former OHA_MRP_ADD */
  MRP_USR_ENUM_TOPO_CTRL_ADD_CHECK,       /* former OHA_MRP_ADD_CHECK */
  MRP_USR_ENUM_TOPO_CTRL_MAX              /* for range checking, init value */
}MRP_USR_ENUM_TOPO_CTRL;

typedef enum _MRP_MIB_ENUM_DOMAIN_BLOCKED
{
  MRP_MIB_DOMAIN_BLOCKED_ENABLED    = 1,
  MRP_MIB_DOMAIN_BLOCKED_DISABLED   = 2
}MRP_MIB_ENUM_DOMAIN_BLOCKED;

typedef enum _MRP_MIB_ENUM_DOMAIN_MRM_REACT_ON_LINK_CHANGE
{
  MRP_MIB_DOMAIN_MRM_REACT_ON_LINK_CHANGE_ENABLED  = 1,
  MRP_MIB_DOMAIN_MRM_REACT_ON_LINK_CHANGE_DISABLED = 2
}MRP_MIB_ENUM_DOMAIN_MRM_REACT_ON_LINK_CHANGE;


/*---------------------------------------------------------------------------*/
/* Opcode: MRP_OPC_RINGSTATE_INDICATION_TYPE                                 */
/*---------------------------------------------------------------------------*/
typedef struct _MRP_OPC_RINGSTATE_INDICATION_TYPE
{
  /* IN parameter */
  /* identification of one specific MRP instance  */
  LSA_UINT16                                    mrpInstance;                /* MRP instance within one interface */
                                                                            /* (valid range 1 ... EDD_CFG_MAX_MRP_INSTANCE_CNT) */
  /* IN/OUT parameter */                                                        
  /* parameters of the former service OHA_MRP_DATA_WRITE */
  LSA_UINT8                                     domainUUID[16];             /* former MRP_DomainUUID */
  MRP_USR_ENUM_TOPO_CTRL                        topoCtrlMode;               /* former OHA_MRP_ADD, etc. */
  LSA_UINT32                                    mrpStatus;                  /* former MRP_PortStatus (can be applied to both ringports) */
  
  /* MRP MIB related and former service OHA_MRP_DATA_WRITE related */
  LSA_UINT16                                    rPort1;                     /* former PortId 1 */
  LSA_UINT16                                    rPort2;                     /* former PortId 2 */

  /* MRP MIB related parameters */
  MRP_MIB_ENUM_DOMAIN_ROLE                      adminRole;
  MRP_MIB_ENUM_DOMAIN_ROLE                      operRole;

  MRP_MIB_ENUM_DOMAIN_BLOCKED                   domainBlocked;
  LSA_UINT32                                    domainVlanId;
  LSA_UINT16                                    domainManagerPrio;           /* only applicable if operRole =MRP manager */
  LSA_UINT16                                    domainMrmReactOnLinkChange;  /* only applicable if operRole =MRP manager */
                                                                             /* see MRP_MIB_ENUM_DOMAIN_MRM_REACT_ON_LINK_CHANGE */
}MRP_OPC_RINGSTATE_INDICATION_TYPE;

/*---------------------------------------------------------------------------*/
/* union MRP_STATE_INDICATION                                                */
/*---------------------------------------------------------------------------*/
typedef union _MRP_STATE_INDICATION_TYPE
{
  MRP_OPC_RED_STATE_INDICATION_TYPE    redStateInd;
  MRP_OPC_STATE_INDICATION_TYPE        usrStateInd;
  MRP_OPC_RINGSTATE_INDICATION_TYPE    ohaStateInd;
  MRP_OPC_RED_IN_EVENT_INDICATION_TYPE inStateInd;
}MRP_STATE_INDICATION_TYPE;

typedef  MRP_STATE_INDICATION_TYPE   *MRP_USR_STATE_INDICATION_PTR_TYPE;


/*---------------------------------------------------------------------------*/
/*  common defines for MRP resource availability                             */
/*---------------------------------------------------------------------------*/
#define MRP_IND_RESOURCE_MISSING_ON             1
#define MRP_IND_RESOURCE_MISSING_OFF            0


/*---------------------------------------------------------------------------*/
/* struct MRP_IN_STATE_INST_EVENT_DATA                                       */
/*---------------------------------------------------------------------------*/

/* instance related MRP interconnection event data */
typedef struct _MRP_IN_INST_EVENT_DATA
{
  /* service MRP_OPC_RED_IN_EVENT_INDICATION_TYPE */
  MRP_UPPER_RQB_PTR_TYPE        pInEventIndRqbB;
  MRP_UPPER_RQB_PTR_TYPE        pInEventIndRqbT;

  MRP_STATE_INDICATION_TYPE     InEventIndLastSignaled;     /* last indicated state */
  MRP_STATE_INDICATION_TYPE     InEventIndCurrent;          /* current state */
  LSA_UINT8                     InEventIndOutOfResource;
}MRP_IN_INST_EVENT_DATA;



/*---------------------------------------------------------------------------*/
/* struct MRP_USR_INST_DATA                                                  */
/*---------------------------------------------------------------------------*/

/* instance related USR data */
typedef struct _MRP_USR_INST_DATA
{
  /* service MRP_OPC_STATE_INDICATION */
  MRP_UPPER_RQB_PTR_TYPE        pStateIndRqbB;
  MRP_UPPER_RQB_PTR_TYPE        pStateIndRqbT;

  MRP_STATE_INDICATION_TYPE     StateIndLastSignaled;    /* last indicated state */
  MRP_STATE_INDICATION_TYPE     StateIndCurrent;         /* current state */
  LSA_UINT8                     StateIndOutOfResource;
  
  /* service MRP_OPC_REDSTATE_INDICATION */
  MRP_UPPER_RQB_PTR_TYPE        pRedStateIndRqbB;
  MRP_UPPER_RQB_PTR_TYPE        pRedStateIndRqbT;

  MRP_STATE_INDICATION_TYPE     RedStateIndLastSignaled;    /* last indicated state */
  MRP_STATE_INDICATION_TYPE     RedStateIndCurrent;         /* current state */
  LSA_UINT8                     RedStateIndOutOfResource;

  /* MRP interconnection event data which are related to one MRP interconnection instance */
  MRP_IN_INST_EVENT_DATA        *pInInstEventData;

#ifdef MRP_CFG_PLUGIN_2_HSR
  MRP_UPPER_RQB_PTR_TYPE        pHsrIndProvideRqbB;
  MRP_UPPER_RQB_PTR_TYPE        pHsrIndProvideRqbT;
#endif
}MRP_USR_INST_DATA;

/*---------------------------------------------------------------------------*/
/* struct MRP_OHA_INST_DATA                                                  */
/*---------------------------------------------------------------------------*/
/* instance related OHA data */
typedef struct _MRP_OHA_INST_DATA
{
  /* service MRP_OPC_RINGSTATE_INDICATION */
  MRP_UPPER_RQB_PTR_TYPE        pRingstateIndRqbB;
  MRP_UPPER_RQB_PTR_TYPE        pRingstateIndRqbT;

  MRP_STATE_INDICATION_TYPE     RingstateIndLastSignaled; /* last indicated state */
  MRP_STATE_INDICATION_TYPE     RingstateIndCurrent;      /* current state */
  LSA_UINT8                     RingstateIndOutOfResource;
}MRP_OHA_INST_DATA;

/*---------------------------------------------------------------------------*/
/* struct MRP_USR_DB_TYPE                                                    */
/*---------------------------------------------------------------------------*/

/* channel related USR database */
typedef struct _MRP_USR_DB_TYPE
{
  MRP_UPPER_RQB_PTR_TYPE        pUsrPendingRqbB;
  MRP_UPPER_RQB_PTR_TYPE        pUsrPendingRqbT;
  MRP_USR_INST_DATA             *pInstData;
}MRP_USR_DB_TYPE;

/*---------------------------------------------------------------------------*/
/* struct MRP_OHA_DB_TYPE                                                    */
/*---------------------------------------------------------------------------*/

/* channel related OHA database */
typedef struct _MRP_OHA_DB_TYPE
{
  MRP_UPPER_RQB_PTR_TYPE        pUsrPendingRqbB;
  MRP_UPPER_RQB_PTR_TYPE        pUsrPendingRqbT;
  MRP_OHA_INST_DATA             *pInstData;
}MRP_OHA_DB_TYPE;

/*---------------------------------------------------------------------------*/
/* Definition of ARGS sent within MRP RQB: MRP_ARGS_TYPE                     */
/*---------------------------------------------------------------------------*/
typedef union _MRP_ARGS_TYPE
{
  union 
  {
    MRP_OPEN_CHANNEL_TYPE             open;
  } channel;

  union 
  {
    MRP_PRM_PREPARE_TYPE              prm_prepare;
    MRP_PRM_WRITE_TYPE                prm_write;
    MRP_PRM_END_TYPE                  prm_end;
    MRP_PRM_COMMIT_TYPE               prm_commit;
    MRP_PRM_CHANGE_PORT_TYPE          prm_change_port;
    MRP_PRM_READ_TYPE                 prm_read;
    MRP_GET_STATUS_TYPE               get_status;
    MRP_SET_STATUS_TYPE               set_status;
    MRP_IN_TC_REQUEST_TYPE            in_tc_request;
  } ctrl;

  union 
  {
    MRP_PRM_INDICATION_TYPE           prm_ind;
    MRP_STATE_INDICATION_TYPE         state_ind;
    MRP_OPC_IND_PROVIDE_TYPE          ind;
  } state;
} MRP_ARGS_TYPE;

typedef  MRP_ARGS_TYPE   *MRP_UPPER_ARGS_PTR_TYPE;


/*---------------------------------------------------------------------------*/
/* Definition of RQB used by MRP: MRP_RQB_TYPE                               */
/*---------------------------------------------------------------------------*/

typedef struct mrp_rqb_tag 
{
  MRP_RQB_HEADER
  MRP_ARGS_TYPE args;
  MRP_RQB_TRAILER
}MRP_RQB_TYPE;
#include "sys_unpck.h"


/*===========================================================================*/
/*                                prototyping                                */
/*===========================================================================*/

/*====  in functions  =====*/
/*=============================================================================
 * function name:  mrp_open_channel
 *
 * function:       open a communication channel
 *
 * parameters:     MRP_UPPER_RQB_PTR_TYPE  upper_rqb_ptr:  pointer to upper-
 *                                                            RQB
 *
 * return value:   LSA_VOID
 *
 * RQB-parameters:
 *     RQB-header:
 *     LSA_OPCODE_TYPE    opcode:        *_OPC_OPEN_CHANNEL
 *     LSA_HANDLE_TYPE    handle:        channel-handle of user
 *     LSA_USER_ID_TYPE   user-id:       id of user
 *     RQB-args:
 *     LSA_HANDLE_TYPE    handle_upper:  channel-handle of user
 *     LSA_SYS_PATH_TYPE  sys_path:      system-path of channel
 *     LSA_VOID  LSA_FCT_PTR(*_request_upper_done_ptr)
 *                                    (MRP_UPPER_RQB_PTR_TYPE upper_rqb_ptr)
 *                                       callback-function
 *
 * RQB-return values via callback-function:
 *     LSA_UINT16         response:      *_RSP_OK
 *                                       *_RSP_ERR
 *     RQB-args:
 *     LSA_HANDLE_TYPE    handle:        channel-handle of prefix
 *     All other RQB-parameters are unchanged.
 *===========================================================================*/
LSA_VOID mrp_open_channel(MRP_UPPER_RQB_PTR_TYPE  pRQB);


/*=============================================================================
 * function name:  mrp_close_channel
 *
 * function:       close a communication channel
 *
 * parameters:     MRP_UPPER_RQB_PTR_TYPE  upper_rqb_ptr:  pointer to upper-
 *                                                            RQB
 *
 * return value:   LSA_VOID
 *
 * RQB-parameters:
 *     RQB-header:
 *     LSA_OPCODE_TYPE   opcode:   *_OPC_CLOSE_CHANNEL
 *     LSA_HANDLE_TYPE   handle:   channel-handle of acp
 *     LSA_USER_ID_TYPE  user-id:  id of user
 *
 * RQB-return values via callback-function:
 *     RQB-header:
 *     LSA_HANDLE_TYPE   handle:    channel-handle of user
 *     RQB-args:         response:  *_RSP_OK
 *                                  *_RSP_ERR
 *     All other RQB-parameters are unchanged.
 *===========================================================================*/
LSA_VOID   mrp_close_channel(MRP_UPPER_RQB_PTR_TYPE  pRQB);


/*=============================================================================
 * function name:  mrp_request
 *
 * function:       send a request to a communication channel
 *
 * parameters:     MRP_UPPER_RQB_PTR_TYPE  upper_rqb_ptr:  pointer to upper-
 *                                                            RQB
 *
 * return value:   LSA_VOID
 *
 * RQB-parameters:
 *     RQB-header:
 *     LSA_OPCODE_TYPE   opcode:    *_OPC_*
 *     LSA_HANDLE_TYPE   handle:    channel-handle of acp
 *     LSA_USER_ID_TYPE  user-id:   id of user
 *     RQB-args:                    Depend on kind of request.
 *
 * RQB-return values via callback-function:
 *     RQB-header:
 *     LSA_OPCODE_TYPE   opcode:    *_OPC_*
 *     LSA_HANDLE_TYPE   handle:    channel-handle of user
 *     LSA_USER_ID_TYPE  user-id:   id of user
 *     RQB-args:         response:  *_RSP_OK
 *                                  *_RSP_ERR
 *                                  or others
 *     All other RQB-parameters depend on kind of request.
 *===========================================================================*/
LSA_VOID mrp_request(MRP_UPPER_RQB_PTR_TYPE  pRQB);


/*=============================================================================
 * function name:  mrp_request_synchron
 *
 * function:       send a synchronous request to a communication channel
 *
 * parameters:     MRP_UPPER_RQB_PTR_TYPE  upper_rqb_ptr:  pointer to upper-
 *                                                            RQB
 *
 * return value:   LSA_VOID
 *
 * RQB-parameters:
 *     RQB-header:
 *     LSA_OPCODE_TYPE   opcode:    *_OPC_*
 *     LSA_HANDLE_TYPE   handle:    channel-handle of acp
 *     LSA_USER_ID_TYPE  user-id:   id of user
 *     RQB-args:                    Depend on kind of request.
 *
 * RQB-return values via callback-function:
 *     RQB-header:
 *     LSA_OPCODE_TYPE   opcode:    *_OPC_*
 *     LSA_HANDLE_TYPE   handle:    channel-handle of user
 *     LSA_USER_ID_TYPE  user-id:   id of user
 *     RQB-args:         response:  *_RSP_OK
 *                                  *_RSP_ERR
 *                                  or others
 *     All other RQB-parameters depend on kind of request.
 *===========================================================================*/
LSA_VOID mrp_request_synchron(MRP_UPPER_RQB_PTR_TYPE  pRQB);



/*----------------------------------------------------------------------------*/
#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif  /* of MRP_USR_H */

