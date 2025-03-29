#ifndef MRP_SYS_H
#define MRP_SYS_H

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
/*  F i l e               &F: mrp_sys.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  system interface                                                         */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef MRP_MESSAGE
/*  2008-01-07  mk    new MRP_MAX_CHANNEL_SET.                               */
/*  2008-02-29  mk    default role for ARD removed.                          */
/*  2009-07-02  ds    include hierarchy changed                              */
/*  2010-05-25  ds    MRP_PATH_TYPE_USER_OHA introduced                      */
/*  2010-06-11  ds    common user channel functionality implemented          */
/*                    new user channel detail: isPrmInterface                */
/*  2010-07-19  ds    new feature MRP MIB implemented according to           */
/*                    IEC 62439-2                                            */
/*  2010-12-10  ds    initial implementation steps for N-IF feature          */
/*  2010-12-13  ds    timer handling reworked for N-IF feature               */
/*  2010-12-21  ds    N-IF: channel handling redesigned                      */
/*  2012-04-04  ds    introduction of N MRP instances per interface          */
/*  2016-10-25  ds    Replace LSA_BOOL by LSA_UINT8 at USR interface         */
/*  2017-11-14  ds    MRP interconnection timer definitions introduced       */
/*                                                                           */
#endif
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/*                                 defines                                   */
/*---------------------------------------------------------------------------*/

/* number of channels per nic */
#define MRP_MAX_CHANNELS_PER_IF        4  /* 1 sys-chan to EDD   + 1 prm-user (CM-PD) +              */
                                          /* 1 normal user (OHA) + 1 priv. user (switch-appl. (HSR)) */

/* maximum number of communication channels */
#define MRP_MAX_CHANNELS  (MRP_MAX_NO_OF_INTERFACES * MRP_MAX_CHANNELS_PER_IF)

/* maximum number of timer IDs, that might be associated to one EDD interface */ 
#define MRP_MAX_NO_OF_TIMER_IDS_PER_INTERFACE    256

#define MRP_FATAL_ERROR_PTR_TYPE          /* pointer to LSA_FATAL_ERROR */ \
  struct lsa_fatal_error_tag*

/*---------------------------------------------------------------------------*/
/*                                  types                                    */
/*---------------------------------------------------------------------------*/
#define MRP_LOCAL_MEM_PTR_TYPE                  /* pointer to local-memory */\
  LSA_VOID*


typedef enum 
{
  MRP_TIMER_STS_T_NOT_INIT
  ,MRP_TIMER_STS_T_STOPPED
  ,MRP_TIMER_STS_T_STOP_PENDING
  ,MRP_TIMER_STS_T_RUNNING
}MRP_TIMER_STS_T;

typedef enum _MRP_ENUM_TIMER_USE_T
{
  MRP_TIMER_ENUM_NO_TIMER           = -3
  , MRP_TIMER_ENUM_NEXT_TIMER       = -2
  , MRP_TIMER_ENUM_NOT_DEFINED      = -1

  /* MRP interface timer */
  , MRP_TIMER_ENUM_MRP_EDD_CYCLIC   = 0

  , MRP_TIMER_ENUM_MRP_LINKUP_PORT1 = 1
  , MRP_TIMER_ENUM_MRP_LINKUP_PORT2 = 2
  , MRP_TIMER_ENUM_MRP_LINKUP_PORT3 = 3
  , MRP_TIMER_ENUM_MRP_LINKUP_PORT4 = 4

  /* MRP instance timer */
  , MRP_TIMER_ENUM_MRP_TPLG_CHG_EDD = 5
  , MRP_TIMER_ENUM_MRP_LINKUP_MRC   = 6
  , MRP_TIMER_ENUM_MRP_LINKDOWN_MRC = 7
  , MRP_TIMER_ENUM_MRP_TEST         = 8
  , MRP_TIMER_ENUM_MRP_TPLG_CHG_REQ = 9
  , MRP_TIMER_ENUM_MRP_OTHER_MRM    = 10
  , MRP_TIMER_ENUM_MRP_MORPHING     = 11

  , MRP_TIMER_ENUM_MRP_ALARM        = 12

  /* MRP interconnection instance timer */
  , MRP_TIMER_ENUM_MRP_RSTPP_IN_TC_SEND   = 13

  , MRP_TIMER_ENUM_MRP_IN_TC_SEND         = 14
  , MRP_TIMER_ENUM_MRP_IN_POLL_SEND       = 15
  , MRP_TIMER_ENUM_MRP_IN_LNK_CHG_DOWN_SEND= 16
  , MRP_TIMER_ENUM_MRP_IN_LNK_CHG_UP_SEND = 17

  /* HRP/Standby timer */
  , MRP_TIMER_ENUM_HSR_TPLG_CHG           = 18
  , MRP_TIMER_ENUM_HSR_SND_TPLG_CHG       = 19
  , MRP_TIMER_ENUM_HSR_SND_STDBY_TPLG_CHG = 20
  , MRP_TIMER_ENUM_HSR_MGR_SND            = 21
  , MRP_TIMER_ENUM_HSR_STDBY_SND          = 22
  , MRP_TIMER_ENUM_HSR_STDBY_ERROR        = 23
  , MRP_TIMER_ENUM_HSR_STDBY_TPLG_CHG_TL  = 24
  , MRP_TIMER_ENUM_HSR_STDBY_WAIT4PARTNER = 25
  , MRP_TIMER_ENUM_HSR_HSR_ARD            = 26

  , MRP_TIMER_ENUM_MRP_NEW_TIMER1         = 27    /* to be changed, if new timer is introduced */
  , MRP_TIMER_ENUM_MRP_NEW_TIMER2         = 28    /* to be changed, if new timer is introduced */
}MRP_ENUM_TIMER_USE_T;



/*---------------------------------------------------------------------------*/
/*                   forward declarations / common types                     */
/*---------------------------------------------------------------------------*/
typedef struct mrp_rqb_tag *MRP_UPPER_RQB_PTR_TYPE;
typedef struct mrp_rqb_tag const *MRP_CONST_UPPER_RQB_PTR_TYPE;

typedef struct _MRP_CH_SYS_DB_TYPE
{
  LSA_UINT32 traceIdx;      /* traceIdx of this DB (acquired by EDD_SRV_GET_PARAMS) */

  LSA_VOID *pInterfacePara; /* DB for HW related data or detail parameters of an EDD interface */
  LSA_VOID *pInterfaceTimer;/* DB for timer data, related to one interface */
  LSA_VOID *pEdd;           /* DB for MRP-EDD interface functionality */
  LSA_VOID *pMrc;           /* DB for MRP client functionality  */
  LSA_VOID *pMrpMIB;        /* DB for MRP-MIB variables according to IEC62439-2 */
  LSA_VOID *pMrpIn;         /* DB for MRP interconnection functionality  */
#ifdef MRP_CFG_PLUGIN_0_MRM
  LSA_VOID *pMrm;           /* DB for MRP manager functionality */
#endif
#ifdef MRP_CFG_PLUGIN_2_HSR
  LSA_VOID *pArd;           /* DB for ARD mode functionality    */
  LSA_VOID *pHsrB;          /* DB for HSR base functionality    */
  LSA_VOID *pHsrC;          /* DB for HSR client functionality  */
  LSA_VOID *pHsrM;          /* DB for HSR manager functionality */
  LSA_VOID *pHsrS;          /* DB for HSR standby functionality */
#endif
}MRP_CH_SYS_DB_TYPE;

/*---------------------------------------------------------------------------*/
/*                              detail types                                 */
/*---------------------------------------------------------------------------*/

typedef enum _MRP_CAPABILITY_ROLE_ENUM_T
{
   MRP_CAPABILITY_ROLE_OFF      = 0x0000
  ,MRP_CAPABILITY_ROLE_CLIENT   = 0x0001
  ,MRP_CAPABILITY_ROLE_MGR      = 0x0002
  ,MRP_CAPABILITY_ROLE_AUTO_MGR = 0x0004
} MRP_CAPABILITY_ROLE_ENUM_T;

typedef enum 
{
   MRP_ADMIN_ROLE_OFF
  ,MRP_ADMIN_ROLE_CLIENT
  ,MRP_ADMIN_ROLE_MGR
  ,MRP_ADMIN_ROLE_AUTO_MGR
}MRP_ADMIN_ROLE_ENUM_T;

typedef enum
{
  MRP_OPER_ROLE_OFF
  ,MRP_OPER_ROLE_CLIENT
  ,MRP_OPER_ROLE_MGR
  ,MRP_OPER_ROLE_AUTO_MGR
}MRP_OPER_ROLE_ENUM_T;

typedef enum 
{
  MRP_PROJ_ROLE_OFF
  ,MRP_PROJ_ROLE_CLIENT
  ,MRP_PROJ_ROLE_MGR
  ,MRP_PROJ_ROLE_AUTO_MGR
}MRP_PROJ_ROLE_ENUM_T;


typedef enum
{
  MRP_MRM_EDITION_NONE             /* initial value */
  ,MRP_MRM_EDITION_1               /* siemens MRM/MRA V1.0 implementation */ 
  ,MRP_MRM_EDITION_2_COMPATIBLE    /* siemens MRM/MRA V2.0 implementation */
  ,MRP_MRM_EDITION_2_ONLY          /* not yet implemented! */
}MRP_MRM_EDITION_ENUM_T;


typedef enum
{
  MRP_HA_ADMIN_NONE
  ,MRP_HA_ADMIN_FORWARDER
  ,MRP_HA_ADMIN_APPLICATION
}MRP_HA_ADMIN_ROLE_ENUM_T;

enum mrp_path_type_enum 
{
  MRP_PATH_TYPE_EDD     = 69,   /* 0x45 */
  MRP_PATH_TYPE_USER    = 85,   /* 0x55 */
  MRP_PATH_TYPE_USER_2  = 50,   /* 0x32 */
  MRP_PATH_TYPE_INVALID = 255
};

enum mrp_ic_capability_role_enum
{
  MRP_IC_CAPABILITY_ROLE_OFF     = 0x0000
  ,MRP_IC_CAPABILITY_ROLE_CLIENT = 0x0001
  ,MRP_IC_CAPABILITY_ROLE_MGR    = 0x0002
};

typedef enum
{
  MRP_IC_PROJ_ROLE_OFF
  ,MRP_IC_PROJ_ROLE_CLIENT
  ,MRP_IC_PROJ_ROLE_MGR
}MRP_IC_PROJ_ROLE_ENUM_T;


typedef struct 
{
  LSA_UINT16  NicId;             /* reference to the network device which must be used */
  LSA_UINT16  PathType;          /* type of channel (see mrp_path_type_enum) */

  union mrp_detail_union 
  {
    struct mrp_detail_edd_sys_tag 
    {
      LSA_INT dummy;                     /* not used up to now. */
    }edd;

    struct mrp_detail_usr_tag 
    {
      LSA_UINT8  isPrmInterface;         /* MRP_IS_PRM_INTERFACE_ON:  channel is used for PRM */
                                         /* MRP_IS_PRM_INTERFACE_OFF: channel is not used for PRM */
      LSA_UINT16 PrioDefault;            /* default mrp priority */
    }user;

    struct mrp_detail_user2_tag 
    {
      LSA_INT dummy;                     /* not used up to now. */
    }user2;
  }type;
}MRP_DETAIL_TYPE;

typedef MRP_DETAIL_TYPE *MRP_DETAIL_PTR_TYPE;
typedef MRP_DETAIL_TYPE const *MRP_CONST_DETAIL_PTR_TYPE;


/*****************************************************************************/
/* Defines for channel detail isPrmInterface                                 */
/*****************************************************************************/
#define MRP_IS_PRM_INTERFACE_ON                 1
#define MRP_IS_PRM_INTERFACE_OFF                0


/*===========================================================================*/
/*                                prototyping                                */
/*===========================================================================*/

/*====  in functions  =====*/

/*=============================================================================
 * function name:  mrp_init
 *
 * function:       initialize mrp
 *
 * parameters:     ....
 *
 * return value:   LSA_UINT16:  LSA_RET_OK
 *                              possibly:
 *                              LSA_RET_ERR_RESOURCE
 *                              or others
 *===========================================================================*/
LSA_UINT16 mrp_init (LSA_VOID);


/*=============================================================================
 * function name:  mrp_undo_init
 *
 * function:       undo the initialize of mrp
 *
 * parameters:     ....
 *
 * return value:   LSA_UINT16:  LSA_RET_OK
 *                              possibly:
 *                              LSA_RET_ERR_RESOURCE
 *                              or others
 *===========================================================================*/
LSA_UINT16 mrp_undo_init (LSA_VOID);


/*=============================================================================
 * function name:  mrp_timeout          
 *
 * function:       time out
 *
 * parameters:		LSA_UINT16 timer_id     
 *					      LSA_USER_ID_TYPE user_id 
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID mrp_timeout (LSA_UINT16 timer_id, LSA_USER_ID_TYPE user_id);

/*----------------------------------------------------------------------------*/
#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif


#endif  /* of MRP_SYS_H */
