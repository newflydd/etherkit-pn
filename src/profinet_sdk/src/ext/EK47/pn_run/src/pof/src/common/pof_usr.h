#ifndef POF_USR_H   /* ----- reinclude-protection ----- */
#define POF_USR_H

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
/*  F i l e               &F: pof_usr.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  User Interface                                                           */
/*  Defines constants, types, macros and prototyping for POF                 */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/* Date        Who   What                                                    */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/
/*---------------------------------------------------------------------------*/
/* Opcodes within RQB                                                        */
/*---------------------------------------------------------------------------*/
 
#define POF_OPC_OPEN_CHANNEL                (LSA_OPCODE_TYPE)   0x01
#define POF_OPC_CLOSE_CHANNEL               (LSA_OPCODE_TYPE)   0x02
#define POF_OPC_TIMER                       (LSA_OPCODE_TYPE)   0x03
/* Services */
#define POF_OPC_SET_RAW_HARDWARE            (LSA_OPCODE_TYPE)   0x04
#define POF_OPC_GET_RAW_HARDWARE            (LSA_OPCODE_TYPE)   0x05
#define POF_OPC_SET_DIAGNOSTIC              (LSA_OPCODE_TYPE)   0x06
#define POF_OPC_GET_PARAMS                  (LSA_OPCODE_TYPE)   0x07
#define POF_OPC_PRM_READ                    (LSA_OPCODE_TYPE)   0x08
#define POF_OPC_PRM_WRITE                   (LSA_OPCODE_TYPE)   0x09
#define POF_OPC_PRM_INDICATION              (LSA_OPCODE_TYPE)   0x0A
#define POF_OPC_PRM_PREPARE                 (LSA_OPCODE_TYPE)   0x0B
#define POF_OPC_PRM_END                     (LSA_OPCODE_TYPE)   0x0C
#define POF_OPC_PRM_COMMIT                  (LSA_OPCODE_TYPE)   0x0D
#define POF_OPC_PRM_CHANGE_PORT             (LSA_OPCODE_TYPE)   0x0E
#define POF_OPC_TEST_HARDWARE               (LSA_OPCODE_TYPE)   0x0F /* only for test */

/*---------------------------------------------------------------------------*/
/*  Response within RQB (Response)                                           */
/*---------------------------------------------------------------------------*/

#define POF_RSP  LSA_UINT16     /* don't change            */
#define POF_LOCAL_RSP_PTR_TYPE  /* pointer to local-memory */ \
    POF_RSP  POF_LOCAL_MEM_ATTR  *

/* LSA Codes */
#define POF_RSP_OK                          (POF_RSP)    LSA_OK
#define POF_RSP_OK_TIMER_RESTARTED          (POF_RSP)    LSA_RET_OK_TIMER_RESTARTED
#define POF_RSP_OK_TIMER_NOT_RUNNING        (POF_RSP)    LSA_RET_OK_TIMER_NOT_RUNNING
/* LSA Codes */                      
#define POF_RSP_ERR_RESOURCE                (POF_RSP)    LSA_RET_ERR_RESOURCE
#define POF_RSP_ERR_PARAM                   (POF_RSP)    LSA_RET_ERR_PARAM
#define POF_RSP_ERR_SEQUENCE                (POF_RSP)    LSA_RET_ERR_SEQUENCE
#define POF_RSP_ERR_SYS_PATH                (POF_RSP)    LSA_RET_ERR_SYS_PATH
#define POF_RSP_ERR_NO_TIMER                (POF_RSP)    LSA_RET_ERR_NO_TIMER

#define POF_RSP_OK_CANCEL                   (POF_RSP)    (LSA_RET_OK_OFFSET + 0x01)

#define POF_RSP_ERR_OPCODE                  (POF_RSP)    (LSA_RET_ERR_OFFSET + 0x01)
#define POF_RSP_ERR_CHANNEL_USE             (POF_RSP)    (LSA_RET_ERR_OFFSET + 0x02)
#define POF_RSP_ERR_LL                      (POF_RSP)    (LSA_RET_ERR_OFFSET + 0x03)
#define POF_RSP_ERR_PRM_DATA                (POF_RSP)    (LSA_RET_ERR_OFFSET + 0x04)
#define POF_RSP_ERR_PRM_BLOCK               (POF_RSP)    (LSA_RET_ERR_OFFSET + 0x05)
#define POF_RSP_ERR_PRM_INDEX               (POF_RSP)    (LSA_RET_ERR_OFFSET + 0x06)
#define POF_RSP_ERR_PRM_CONSISTENCY         (POF_RSP)    (LSA_RET_ERR_OFFSET + 0x07)
#define POF_RSP_ERR_PRM_PORTID              (POF_RSP)    (LSA_RET_ERR_OFFSET + 0x08)
#define POF_RSP_ERR_NOT_ALLOWED             (POF_RSP)    (LSA_RET_ERR_OFFSET + 0x09)
#define POF_RSP_ERR_EXCP                    (POF_RSP)    (LSA_RET_ERR_OFFSET + 0x0A)

//#define POF_RSP_ERR_TRANSCEIVER_NOT_SUPPORTED       (POF_RSP)    (LSA_RET_ERR_OFFSET + 0x0B)
//#define POF_RSP_ERR_TRANSCEIVER_NOT_PARAMETERIZED   (POF_RSP)    (LSA_RET_ERR_OFFSET + 0x0C)
//#define POF_RSP_ERR_DIAG_IS_ALREADY_ORIGINAL        (POF_RSP)    (LSA_RET_ERR_OFFSET + 0x0D)
//#define POF_RSP_ERR_DIAG_IS_ALREADY_MANIPULATED     (POF_RSP)    (LSA_RET_ERR_OFFSET + 0x0E)
//#define POF_RSP_ERR_HIGH_LOW_MANIPULATION           (POF_RSP)    (LSA_RET_ERR_OFFSET + 0x0F)
//#define POF_RSP_ERR_DIAG_NOT_SUPPORTED              (POF_RSP)    (LSA_RET_ERR_OFFSET + 0x10)

/*===========================================================================*/
/*                              basic types for POF                          */
/*===========================================================================*/
#define POF_UPPER_MEM_PTR_TYPE               /* pointer to upper-memory */ \
LSA_VOID                    POF_UPPER_MEM_ATTR *

#define POF_UPPER_MEM_U8_PTR_TYPE            /* pointer to upper-memory */ \
LSA_UINT8                   POF_UPPER_MEM_ATTR *

#define POF_UPPER_MEM_U16_PTR_TYPE           /* pointer to upper-memory */ \
LSA_UINT16                  POF_UPPER_MEM_ATTR *

#define POF_UPPER_MEM_U32_PTR_TYPE          /* pointer to upper-memory */ \
LSA_UINT32                  POF_UPPER_MEM_ATTR *

#define POF_UPPER_MEM_U64_PTR_TYPE              /* pointer to device-memory */ \
LSA_UINT64                  POF_UPPER_MEM_ATTR *

#define POF_EDD_LOWER_MEM_PTR_TYPE             /* pointer to lower-memory */ \
LSA_VOID                    POF_EDD_LOWER_MEM_ATTR *

#define POF_EDD_LOWER_MEM_U8_PTR_TYPE          /* pointer to lower-memory */ \
LSA_UINT8                   POF_EDD_LOWER_MEM_ATTR *

#define POF_LOCAL_MEM_PTR_TYPE                /* pointer to local-memory */ \
LSA_VOID                    POF_LOCAL_MEM_ATTR *

#define POF_LOCAL_MEM_U8_PTR_TYPE             /* pointer to local-memory */ \
LSA_UINT8                   POF_LOCAL_MEM_ATTR *

#define POF_LOCAL_MEM_U16_PTR_TYPE            /* pointer to local-memory */ \
LSA_UINT16                  POF_LOCAL_MEM_ATTR *

#define POF_LOCAL_MEM_I16_PTR_TYPE            /* pointer to local-memory */ \
LSA_INT16                   POF_LOCAL_MEM_ATTR *

#define POF_LOCAL_MEM_U32_PTR_TYPE            /* pointer to local-memory */ \
LSA_UINT32                  POF_LOCAL_MEM_ATTR *

#define POF_LOCAL_MEM_U8_CONST_PTR_TYPE       /* pointer to local-memory */ \
const LSA_UINT8             POF_LOCAL_MEM_ATTR *

#define POF_UPPER_RQB_MEM_PTR_TYPE            /* pointer to upper RQB-mem*/ \
LSA_VOID                    POF_UPPER_RQB_ATTR *

#define POF_COMMON_MEM_PTR_TYPE               /* pointer to common-memory */ \
LSA_VOID                    POF_COMMON_MEM_ATTR *

#define POF_COMMON_MEM_U8_PTR_TYPE            /* pointer to common-memory */ \
LSA_UINT8                   POF_COMMON_MEM_ATTR *

#define POF_COMMON_MEM_U16_PTR_TYPE            /* pointer to common-memory */ \
LSA_UINT16                  POF_COMMON_MEM_ATTR *

#define POF_COMMON_MEM_U32_PTR_TYPE            /* pointer to common-memory */ \
LSA_UINT32                  POF_COMMON_MEM_ATTR *


/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/
typedef struct _POF_RQB_TYPE     POF_UPPER_RQB_ATTR *    POF_UPPER_RQB_PTR_TYPE;

/*===========================================================================*/
/*                       Args for RQB (Request Block)                        */
/*===========================================================================*/

#define POF_PORT_ID_ALL  0xFFFF

/*---------------------------------------------------------------------------*/
/* Opcode: POF_OPC_OPEN_CHANNEL                                              */
/*---------------------------------------------------------------------------*/

typedef  LSA_VOID  LSA_FCT_PTR( POF_UPPER_OUT_FCT_ATTR, POF_UPPER_CALLBACK_FCT_PTR_TYPE )( POF_UPPER_RQB_PTR_TYPE  pRQB );
#include "sys_pck1.h"
typedef struct _POF_RQB_OPEN_CHANNEL_TYPE
{
  LSA_HANDLE_TYPE                   handle;
  /* req: -                                */
  /* cnf: channel-handle of POF            */
  /* At all later RQBs user will write it  */
  /* into handle of POF_RQB_HEADER         */
  LSA_HANDLE_TYPE                   handle_upper;
  /* req: channel-handle of user           */
  /* cnf: -                                */
  /* At all confirmations POF writes it    */
  /* into handle of POF_RQB_HEADER.        */
  LSA_SYS_PATH_TYPE                 sys_path;
  /* req: system-path of channel           */
  /* cnf: -                                */
  POF_UPPER_CALLBACK_FCT_PTR_TYPE   Cbf;
  /* req: pointer to callback-function     */
  /* cnf: -                                */

} POF_RQB_OPEN_CHANNEL_TYPE;

typedef struct _POF_RQB_OPEN_CHANNEL_TYPE   POF_UPPER_MEM_ATTR *    POF_UPPER_OPEN_CHANNEL_PTR_TYPE;

/*---------------------------------------------------------------------------*/
/* Opcode: POF_OPC_CLOSE_CHANNEL                                            */
/*---------------------------------------------------------------------------*/

/* no args */

/*---------------------------------------------------------------------------*/
/* Opcode: POF_OPC_PRM_READ                                                  */
/*---------------------------------------------------------------------------*/
typedef struct _POF_RQB_PRM_READ_TYPE
{
  LSA_UINT16                  slot_number;
  LSA_UINT16                  subslot_number;
  LSA_UINT16                  edd_port_id;
  LSA_UINT32                  record_index;
  LSA_UINT32                  record_data_length;
  POF_UPPER_MEM_U8_PTR_TYPE   record_data;

} POF_RQB_PRM_READ_TYPE;

typedef struct _POF_RQB_PRM_READ_TYPE   POF_UPPER_MEM_ATTR *   POF_UPPER_PRM_READ_PTR_TYPE;

/*---------------------------------------------------------------------------*/
/* Opcode: POF_OPC_PRM_WRITE                                                 */
/*---------------------------------------------------------------------------*/
typedef struct _POF_RQB_PRM_WRITE_TYPE
{
  LSA_UINT16                  slot_number;
  LSA_UINT16                  subslot_number;
  LSA_UINT16                  edd_port_id;
  LSA_UINT32                  record_index;
  LSA_UINT32                  record_data_length;
  POF_UPPER_MEM_U8_PTR_TYPE   record_data;
  LSA_UINT32                  err_offset;

} POF_RQB_PRM_WRITE_TYPE;

typedef struct _POF_RQB_PRM_WRITE_TYPE   POF_UPPER_MEM_ATTR *   POF_UPPER_PRM_WRITE_PTR_TYPE;

/** macro to set values for POF_RQB_PRM_WRITE_TYPE
 *
 * @param   pPrmWrite       pointer to struct POF_RQB_PRM_WRITE_TYPE
 * @param   _err_offset     LSA_UINT32 value for err_offset
 */
#define POF_RQB_PRM_WRITE_SET(pPrmWrite, _err_offset)                           \
{                                                                               \
    ((POF_UPPER_PRM_WRITE_PTR_TYPE)(pPrmWrite))->err_offset = (_err_offset);    \
}

/*---------------------------------------------------------------------------*/
/* Opcode: POF_OPC_PRM_INDICATION                                            */
/*---------------------------------------------------------------------------*/
#define POF_PRM_INDICATION_DIAG_MAX_ENTRIES  10

typedef struct _POF_PRM_IND_DATA_TYPE
{               
  LSA_UINT16   ChannelProperties;   /* ChannelProperties.Type_Bit0_7 := 0,                                     */
                                    /* ChannelProperties.Accumulative_Bit8 := 0,                               */
                                    /* Channelproperties.MaintenanceRequired_Bit9  := component-specific,      */
                                    /* Channelproperties.MaintenanceDemanded_Bit10 := component-specific,      */
                                    /* ChannelProperties.Specifier_Bit11_12 := Appearing(1) or Disappearing(2) */
                                    /* ChannelProperties.Direction_Bit13_15 := 0                               */
  LSA_UINT16   ChannelErrorType;    /* 0x8007 */
  LSA_UINT16   ExtChannelErrorType; /* 0x8000 */
  LSA_UINT32   ExtChannelAddValue;  /* 0...   */

} POF_PRM_IND_DATA_TYPE;

typedef struct _POF_RQB_PRM_INDICATION_TYPE
{
  LSA_UINT16                edd_port_id;           /* 1..MaxPortCnt                          */
  LSA_UINT16                diag_cnt;              /* 0..POF_PRM_INDICATION_DIAG_MAX_ENTRIES */                   
  POF_PRM_IND_DATA_TYPE     diag[POF_PRM_INDICATION_DIAG_MAX_ENTRIES];

} POF_RQB_PRM_INDICATION_TYPE;

typedef struct _POF_RQB_PRM_INDICATION_TYPE   POF_UPPER_MEM_ATTR *    POF_UPPER_PRM_INDICATION_PTR_TYPE;

/*---------------------------------------------------------------------------*/
/* Opcode: POF_OPC_GET_RAW_HARDWARE                                          */
/*---------------------------------------------------------------------------*/
typedef enum _POF_GET_RAW_HARDWARE_MEMORY_TYPE
{
  POF_GET_RAW_A0 = 1,
  POF_GET_RAW_A2
    
} POF_GET_RAW_HARDWARE_MEMORY_TYPE;

typedef struct _POF_RQB_GET_RAW_HARDWARE_TYPE
{
  LSA_UINT16                            edd_port_id;
  POF_GET_RAW_HARDWARE_MEMORY_TYPE      Memory;
  LSA_UINT8                             Address; /* Range: 0 - 255 */
  LSA_UINT8                             Value;   /* Range: 0 - 255 */
  
} POF_RQB_GET_RAW_HARDWARE_TYPE;

typedef struct _POF_RQB_GET_RAW_HARDWARE_TYPE   POF_UPPER_MEM_ATTR *    POF_UPPER_GET_RAW_HARDWARE_PTR_TYPE;

/*---------------------------------------------------------------------------*/
/* Opcode: POF_OPC_SET_RAW_HARDWARE                                          */
/*---------------------------------------------------------------------------*/
typedef struct _POF_RQB_SET_RAW_HARDWARE_TYPE
{
  LSA_UINT16   edd_port_id;
  LSA_UINT8    Address; /* Range: 0 - 255 */
  LSA_UINT8    Value;   /* Range: 0 - 255 */
  
} POF_RQB_SET_RAW_HARDWARE_TYPE;

typedef struct _POF_RQB_SET_RAW_HARDWARE_TYPE   POF_UPPER_MEM_ATTR *    POF_UPPER_SET_RAW_HARDWARE_PTR_TYPE;

/*---------------------------------------------------------------------------*/
/* Opcode: POF_OPC_SET_DIAGNOSTIC                                            */
/*---------------------------------------------------------------------------*/
#define POF_DIAGNOSTIC_RX_PWR_GOOD      0x0000 
#define POF_DIAGNOSTIC_RX_PWR_REQUIRED  0x0001         
#define POF_DIAGNOSTIC_RX_PWR_DEMANDED  0x0002 
#define POF_DIAGNOSTIC_RX_PWR_ERROR     0x0003 

typedef struct _POF_RQB_SET_DIAGNOSTIC_TYPE
{
  LSA_UINT16  edd_port_id;
  LSA_UINT16  DiagnosticType;
  
} POF_RQB_SET_DIAGNOSTIC_TYPE;

typedef struct _POF_RQB_SET_DIAGNOSTIC_TYPE   POF_UPPER_MEM_ATTR *    POF_UPPER_SET_DIAGNOSTIC_PTR_TYPE;

/*---------------------------------------------------------------------------*/
/*  Opcode: POF_OPC_PRM_PREPARE                                              */
/*---------------------------------------------------------------------------*/
typedef struct _POF_RQB_PRM_PREPARE_TYPE
{
  LSA_UINT32  dummy;
  
} POF_RQB_PRM_PREPARE_TYPE;

typedef struct _POF_RQB_PRM_PREPARE_TYPE   POF_UPPER_MEM_ATTR *    POF_UPPER_PRM_PREPARE_PTR_TYPE;

#define POF_RQB_PRM_COMMIT_APPLYDEFAULTPORTPARAMS_GET(_pRbu,_pVal,_Cnt)                                 \
{   LSA_UINT32  j;                                                                                      \
    for (j = 0; j < (_Cnt); ++j) { ((_pVal[j]) = (_pRbu)->args.PrmCommit.ApplyDefaultPortparams[j]); }  \
}

/*---------------------------------------------------------------------------*/
/*  Opcode: POF_OPC_PRM_END....                                              */
/*---------------------------------------------------------------------------*/
typedef struct _POF_RQB_PRM_END_TYPE
{
  LSA_UINT16  err_port_id;
  LSA_UINT32  err_index;
  LSA_UINT32  err_offset;
  LSA_UINT8   err_fault;
  LSA_UINT8   PortparamsNotApplicable [EDD_CFG_MAX_PORT_CNT];
  LSA_UINT8   IsModularPort           [EDD_CFG_MAX_PORT_CNT];
 
} POF_RQB_PRM_END_TYPE;

typedef struct _POF_RQB_PRM_END_TYPE   POF_UPPER_MEM_ATTR *    POF_UPPER_PRM_END_PTR_TYPE;

/** macro to set values for POF_RQB_PRM_END_TYPE
 *
 * @param   pPrmEnd                   pointer to struct POF_RQB_PRM_END_TYPE
 * @param   _err_port_id              LSA_UINT16 value for err_port_id
 * @param   _err_index                LSA_UINT32 value for err_index
 * @param   _err_offset               LSA_UINT32 value for err_offset
 * @param   _err_fault                LSA_UINT8  value for err_fault
 */
#define POF_RQB_PRM_END_SET(pPrmEnd, _err_port_id, _err_index, _err_offset, _err_fault) \
{                                                                                       \
    ((POF_UPPER_PRM_END_PTR_TYPE)(pPrmEnd))->err_port_id  = (_err_port_id);             \
    ((POF_UPPER_PRM_END_PTR_TYPE)(pPrmEnd))->err_index    = (_err_index);               \
    ((POF_UPPER_PRM_END_PTR_TYPE)(pPrmEnd))->err_offset   = (_err_offset);              \
    ((POF_UPPER_PRM_END_PTR_TYPE)(pPrmEnd))->err_fault    = (_err_fault);               \
}


 /* default values for structure POF_RQB_PRM_END_TYPE */
#define POF_PRM_ERR_FAULT_NO_OR_TOO_LESS_PARAMS     0
#define POF_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS  1

#define POF_PRM_PORT_PARAMS_APPLICABLE              0
#define POF_PRM_PORT_PARAMS_NOT_APPLICABLE          1

#define POF_PRM_PORT_IS_COMPACT                     0
#define POF_PRM_PORT_IS_MODULAR                     1


#define POF_PRM_ERR_OFFSET_DEFAULT          0
#define POF_PRM_ERR_INDEX_DEFAULT           0x0000FFFF
#define POF_PRM_ERR_PORT_ID_DEFAULT         0
#define POF_PRM_ERR_FAULT_DEFAULT           POF_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS


/*---------------------------------------------------------------------------*/
/*  Opcode: POF_OPC_PRM_COMMIT                                               */
/*---------------------------------------------------------------------------*/
typedef struct _POF_RQB_PRM_COMMIT_TYPE
{
  LSA_UINT8   ApplyDefaultPortparams[EDD_CFG_MAX_PORT_CNT];
  
} POF_RQB_PRM_COMMIT_TYPE;

typedef struct _POF_RQB_PRM_COMMIT_TYPE   POF_UPPER_MEM_ATTR *    POF_UPPER_PRM_COMMIT_PTR_TYPE;


/*---------------------------------------------------------------------------*/
/*  Opcode: POF_OPC_PRM_CHANGE_PORT                                          */
/*---------------------------------------------------------------------------*/
typedef struct _POF_RQB_PRM_CHANGE_PORT_TYPE
{
    LSA_UINT16     edd_port_id;
    LSA_UINT8      module_state;
    LSA_UINT8      port_params_not_applicable;
    LSA_UINT8      apply_default_portparams;
} POF_RQB_PRM_CHANGE_PORT_TYPE;

typedef struct _POF_RQB_PRM_CHANGE_PORT_TYPE   POF_UPPER_MEM_ATTR *    POF_UPPER_PRM_CHANGE_PORT_PTR_TYPE;

#define POF_PRM_PORTMODULE_PLUG_PREPARE     1
#define POF_PRM_PORTMODULE_PLUG_COMMIT      2
#define POF_PRM_PORTMODULE_PULLED           3

#define POF_PRM_APPLY_RECORD_PORT_PARAMS    0
#define POF_PRM_APPLY_DEFAULT_PORT_PARAMS   1


/*---------------------------------------------------------------------------*/
/* Opcode: POF_OPC_GET_PARAMS                                                */
/*---------------------------------------------------------------------------*/
typedef struct _POF_RQB_GET_PARAMS_TYPE
{
  LSA_UINT16    MaxPortCnt;  /* out */
  
} POF_RQB_GET_PARAMS_TYPE; 

typedef struct _POF_RQB_GET_PARAMS_TYPE   POF_UPPER_MEM_ATTR *    POF_UPPER_GET_PARAMS_PTR_TYPE;

/*---------------------------------------------------------------------------*/
/* Opcode: POF_OPC_TIMER  (for POF internal use only!)                       */
/*---------------------------------------------------------------------------*/

typedef struct _POF_RQB_TIMER_TYPE        
{
  LSA_TIMER_ID_TYPE       POFTimerIndex; /* POF-TimerIndex */
  LSA_UINT32              UserID; 

} POF_RQB_TIMER_TYPE;  

typedef struct _POF_RQB_TIMER_TYPE   POF_UPPER_MEM_ATTR *    POF_UPPER_TIMER_PTR_TYPE;

#define POF_TIMER_USER_ID_CYLIC    0UL   /* TTL-Check and Send base timer */

/*---------------------------------------------------------------------------*/
/* Opcode: POF_OPC_TEST_RAW_HARDWARE (!! only for test !!                    */
/*---------------------------------------------------------------------------*/
typedef enum _POF_TEST_HARDWARE_ACTION_TYPE
{
  POF_ACTION_READ = 1,
  POF_ACTION_WRITE
    
} POF_TEST_HARDWARE_ACTION_TYPE;

typedef struct _POF_RQB_TEST_RAW_HARDWARE_TYPE
{
  LSA_UINT16                            edd_port_id;
  POF_GET_RAW_HARDWARE_MEMORY_TYPE      Memory;
  POF_TEST_HARDWARE_ACTION_TYPE         Action;
  LSA_UINT8                             Address; /* Range: 0 - 255 */
  LSA_UINT16                            Length;  /* Range: 1 - 256 */
  POF_UPPER_MEM_U8_PTR_TYPE             pData;
  
} POF_RQB_TEST_RAW_HARDWARE_TYPE;

typedef struct _POF_RQB_TEST_RAW_HARDWARE_TYPE   POF_UPPER_MEM_ATTR *    POF_UPPER_TEST_RAW_HARDWARE_PTR_TYPE;

/*---------------------------------------------------------------------------*/
/*                           RQB- Header for Services                        */
/*---------------------------------------------------------------------------*/

typedef union _POF_RQB_ARGS_TYPE
{
  POF_RQB_OPEN_CHANNEL_TYPE           channel;          /* open channel             */
  POF_RQB_GET_PARAMS_TYPE             GetParams;        /* get params               */
  POF_RQB_TIMER_TYPE                  Timer;            /* TimerEvent               */
  POF_RQB_GET_RAW_HARDWARE_TYPE       GetRawHardware;   /* get raw diagnostic       */
  POF_RQB_SET_RAW_HARDWARE_TYPE       SetRawHardware;   /* set raw diagnostic       */
  POF_RQB_SET_DIAGNOSTIC_TYPE         SetDiagnostic;    /* set diagnostic           */
  POF_RQB_PRM_READ_TYPE               PrmRead;          /* read diagnostic record   */
  POF_RQB_PRM_WRITE_TYPE              PrmWrite;         /* write diagnostic record  */
  POF_RQB_PRM_CHANGE_PORT_TYPE        PrmChangePort;    /* change port              */
  POF_RQB_PRM_INDICATION_TYPE         PrmIndication;    /* indication diagnostic    */
  POF_RQB_PRM_COMMIT_TYPE             PrmCommit;        /* commit ......            */
  POF_RQB_PRM_END_TYPE                PrmEnd;           /* end                      */
  POF_RQB_PRM_PREPARE_TYPE            PrmPrepare;       /* prepare                  */
  POF_RQB_TEST_RAW_HARDWARE_TYPE      TestHardware;     /* only for test            */

} POF_RQB_ARGS_TYPE;

typedef struct _POF_RQB_TYPE
{
  POF_RQB_HEADER

  POF_RQB_ARGS_TYPE         args;                       /* IN/OUT: Args of Request  */
  LSA_UINT32                internal_context_1;

  POF_RQB_TRAILER

} POF_RQB_TYPE;
#include "sys_unpck.h"
/*===========================================================================*/
/*===========================================================================*/

/*===========================================================================*/
/*                                 macros                                    */
/*===========================================================================*/

/*==== set and get parameter of the nare-part of the rqb ===*/

/*===========================================================================*/
/*                                prototyping                                */
/*===========================================================================*/

/*====  in functions  =====*/

/*=============================================================================
* function name:  pof_open_channel()
*
* function:       open a communication channel
*
* parameters:     POF_UPPER_RQB_PTR_TYPE  pRQB:  pointer to Upper-RQB
*
* return value:   LSA_VOID
*
* RQB-Header:
*      LSA_OPCODE_TYPE         Opcode:         POF_OPC_OPEN_CHANNEL
*      LSA_HANDLE_TYPE         Handle:         channel-handle of user
*      LSA_USER_ID_TYPE        UserId:         Id of user
*      POF_RSP                 Response:       -
* RQB-Parameter:
*      LSA_HANDLE_TYPE         HandleUpper:    channel-handle of user
*      LSA_SYS_PATH_TYPE       SysPath:        system-path of channel                 
*      LSA_VOID                LSA_FCT_PTR(POF_UPPER_OUT_FCT_ATTR, Cbf)
*                                         (POF_UPPER_RQB_PTR_TYPE pRQB)
*                                              callback-function
*
* RQB-return values via callback-function:
*      LSA_HANDLE_TYPE         Handle:         channel-handle of nare
*      POF_RSP                 Response:       POF_RSP_OK
*                                              POF_RSP_ERR_PARAM 
*                                              POF_RSP_ERR_SEQUENCE
*                                              or others
*     All other RQB-parameters are unchanged.
*===========================================================================*/
LSA_VOID  POF_UPPER_IN_FCT_ATTR  pof_open_channel( POF_UPPER_RQB_PTR_TYPE  pRQB );

/*=============================================================================
* function name:  pof_close_channel()
*
* function:       close a communication channel
*
* parameters:     POF_UPPER_RQB_PTR_TYPE  pRQB:  pointer to Upper-RQB
*
* return value:   LSA_VOID
*
* RQB-Header:
*      LSA_OPCODE_TYPE         Opcode:         POF_OPC_CLOSE_CHANNEL
*      LSA_HANDLE_TYPE         Handle:         channel-handle of user
*      LSA_USER_ID_TYPE        UserId:         Id of user
*      POF_RSP                 Response:       -
*
* RQB-return values via callback-function:
*      LSA_HANDLE_TYPE         Handle:         HandleUpper from Open-Request
*      POF_RSP                 Response:       POF_RSP_OK
*                                              POF_RSP_ERR_PARAM 
*                                              POF_RSP_ERR_SEQUENCE
*                                              or others
*
*     All other RQB-parameters are unchanged.
*===========================================================================*/
LSA_VOID  POF_UPPER_IN_FCT_ATTR  pof_close_channel( POF_UPPER_RQB_PTR_TYPE  pRQB );

/*=============================================================================
* function name:  pof_request()
*
* function:       send a request to a communication channel
*
* parameters:     POF_UPPER_RQB_PTR_TYPE  pRQB:  pointer to Upper-RQB
*
* return value:   LSA_VOID
*
* RQB-parameters: 
*      RQB-header:
*      LSA_OPCODE_TYPE         Opcode:     POF_OPC_REQUEST
*      LSA_HANDLE_TYPE         Handle:     channel-handle of nare
*      LSA_USER_ID_TYPE        UserId:     ID of user
*      POF_RSP                 Response:   -
*      POF_ARGS_TYPE           Args:       Depend on kind of request.
*     
* RQB-return values via callback-function:
*      LSA_OPCODE_TYPE         opcode:     POF_OPC_REQUEST
*      LSA_HANDLE_TYPE         Handle:     HandleUpper from Open-Request
*      LSA_USER_ID_TYPE        UserId:     ID of user
*      POF_RSP                 Response:   POF_RSP_OK
*                                          POF_RSP_ERR_PARAM 
*                                          POF_RSP_ERR_SEQUENCE
*                                          or others
*
*     All other RQB-parameters depend on kind of request.
*===========================================================================*/
LSA_VOID  POF_UPPER_IN_FCT_ATTR  pof_request( POF_UPPER_RQB_PTR_TYPE  upper_rqb_ptr );

/*****************************************************************************/
/*                                                                           */
/* Output-functions are in pof_out.h                                         */
/*                                                                           */
/*****************************************************************************/


/*****************************************************************************/
/*  end of file pof_usr.h                                                    */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of POF_USR_H */
