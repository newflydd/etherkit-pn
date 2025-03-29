#ifndef EDDP_CMD_INT_H                   /* ----- reinclude-protection ----- */
#define EDDP_CMD_INT_H

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
/*  F i l e               &F: eddp_cmd_inc.h                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  EDDP PNIP Command management internal header file                        */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDD_MESSAGE
/*  12.08.08    JS    initial version.                                       */
#endif
/*****************************************************************************/

#include "eddp_cmd_usr.h"

/*===========================================================================*/
/*                                defines                                    */
/*===========================================================================*/

// F_Code parameter defines
// PPM
#define EDDP_CMD_PPM_PPM_DEACTIVATE                                  0x00000000
#define EDDP_CMD_PPM_PPM_ACTIVATE                                    0x00000001

#define EDDP_CMD_PPM_ACW_DEACTIVATE                                  0x00000000
#define EDDP_CMD_PPM_ACW_ACTIVATE                                    0x00000001

#define EDDP_CMD_PPM_IS_ACW                                          0x00000000
#define EDDP_CMD_PPM_IS_ACW_DG                                       0x00000002

#define EDDP_CMD_PPM_FIRST_SEND_DEACTIVATE                           0x00000000
#define EDDP_CMD_PPM_FIRST_SEND_ACTIVATE                             0x00000004
    
#define EDDP_CMD_PPM_PARAM_ACW_OUT                                   0x00000000
#define EDDP_CMD_PPM_PARAM_ACW_IN                                    0x00000100
    
#define EDDP_CMD_PPM_SHIFT_ACW_ADDRESS                               8

#define EDDP_CMD_PPM_SHIFT_PARAMETERBLOCK_ADDR                       13

#define EDDP_CMD_PPM_SHIFT_PARAM_NEXT_POINTER                        14
#define EDDP_CMD_PPM_SHIFT_PARAM_NEXT_ACW_DG                         16

#define EDDP_CMD_PPM_PARAMBLOCK_COUNT_32BIT                          2

#define EDDP_CMD_PPM_NEW_NEXT_PART_1_BITMASK                         0x00000003
#define EDDP_CMD_PPM_NEW_NEXT_PART_1_SHIFT                           12
#define EDDP_CMD_PPM_NEW_NEXT_PART_2_SHIFT                           14

#define EDDP_CMD_PPM_MOVE_NEXT_OLD_SHIFT                             14




// CPM
#define EDDP_CMD_CPM_CPM_DEACTIVATE                                  0x00000000
#define EDDP_CMD_CPM_CPM_ACTIVATE                                    0x00000001

#define EDDP_CMD_CPM_ACW_DEACTIVATE                                  0x00000000
#define EDDP_CMD_CPM_ACW_ACTIVATE                                    0x00000001

#define EDDP_CMD_CPM_IS_ACW                                          0x00000000
#define EDDP_CMD_CPM_IS_ACW_DG                                       0x00000002

#define EDDP_CMD_CPM_SETTOUNKNOWN_DEACTIVATE                         0x00000000
#define EDDP_CMD_CPM_SETTOUNKNOWN_ACTIVATE                           0x00000004

#define EDDP_CMD_CPM_PARAM_ACW_OUT                                   0x00000000
#define EDDP_CMD_CPM_PARAM_ACW_IN                                    0x00000100

#define EDDP_CMD_CPM_PARAM_IS_ACW                                    0x00000000  
#define EDDP_CMD_CPM_PARAM_IS_ACW_DG                                 0x00000200

#define EDDP_CMD_CPM_SHIFT_ACW_ADDRESS                               8

#define EDDP_CMD_CPM_SHIFT_PARAMETERBLOCK_ADDR                       13

#define EDDP_CMD_CPM_SHIFT_PARAM_NEXT_POINTER                        14

#define EDDP_CMD_CPM_PARAMBLOCK_COUNT_32BIT                          2  

//API
#define EDDP_CMD_API_SHIFT_STOPFILTERMASK                            16

//FDB
#define EDDP_CMD_FDB_DELETE_ONLY_DYNAMIC_ENTRIES                     0x00000000
#define EDDP_CMD_FDB_DELETE_ONLY_DYNAMIC_ENTRIES_ALL_PORT            0x00000000
#define EDDP_CMD_FDB_DELETE_ALL_ENTRIES                              0x00000001
#define EDDP_CMD_FDB_DELETE_ONLY_DYNAMIC_ENTRIES_PORTX               0x00000001

#define EDDP_CMD_FDB_PARAM_STATIC_DEACTIVATE                         0x00000000
#define EDDP_CMD_FDB_PARAM_STATIC_ACTIVATE                           0x00000040

#define EDDP_CMD_FDB_PARAM_AGING_DEACTIVATE                          0x00000000
#define EDDP_CMD_FDB_PARAM_AGING_ACTIVATE                            0x00000080

#define EDDP_CMD_FDB_PARAM_LEARNING_DISABLE_DEACTIVATE               0x00000000
#define EDDP_CMD_FDB_PARAM_LEARNING_DISABLE_ACTIVATE                 0x00002000
  
#define EDDP_CMD_FDB_PARAM_MGMT_ENABLE_DEACTIVATE                    0x00000000
#define EDDP_CMD_FDB_PARAM_MGMT_ENABLE_ACTIVATE                      0x00004000                         

#define EDDP_CMD_FDB_PARAM_MGMT_PRIO_DEACTIVATE                      0x00000000
#define EDDP_CMD_FDB_PARAM_MGMT_PRIO_ACTIVATE                        0x00008000 

#define EDDP_CMD_FDB_PARAM_SHIFT_MAC_ADDR_5                          16
#define EDDP_CMD_FDB_PARAM_SHIFT_MAC_ADDR_4                          24
#define EDDP_CMD_FDB_PARAM_SHIFT_MAC_ADDR_2                          8
#define EDDP_CMD_FDB_PARAM_SHIFT_MAC_ADDR_1                          16
#define EDDP_CMD_FDB_PARAM_SHIFT_MAC_ADDR_0                          24

#define EDDP_CMD_FDB_PARAM_SHIFT_RXPORT                              8 

#define EDDP_CMD_FDB_UC_INSERT_PARAMBLOCK_COUNT_32BIT                2

//SYNC
#define EDDP_CMD_SYNC_UPDATE_COMCYCLE_PARAM_COUNT_32BIT              1
#define EDDP_CMD_SYNC_UPDATE_APPLCYCLE_PARAM_COUNT_32BIT             1
#define EDDP_CMD_SYNC_UPDATE_BASECYCL_PARAM_COUNT_32BIT              1

#define EDDP_CMD_SYNC_RETURNVALUE_INSERT_UC_ENTRY_ERROR              0x00000000

#define EDDP_CMD_CBL_ERROR_MASK                                      0x80000000   
#define EDDP_CMD_IF_STATUS_CONFERROR_MASK                            0x00000008                                                                 



/*===========================================================================*/
/*                                macros                                     */
/*===========================================================================*/
// Single-Data Commands (High)
#define EDDP_SET_CBL_HIGH_32_WORD_WCRC(variable, target_address, last_flag) (variable =\
                                                   (last_flag ? 0x40000000 : 0x0) | /*last flag*/\
                                                   0x30000000 | /*write 32Bit*/\
                                                   0x03000000 | /*WCRC*/\
                                                   target_address) 
// Block-Transfer Commands (Low)
#define EDDP_SET_CBL_LOW_32_BWRI(variable, data_ptr, data_length) (variable =\
                                                   data_ptr >> 2 | /* word address*/\
                                                   data_length << 16)
// Block-Transfer Commands (High)
#define EDDP_SET_CBL_HIGH_32_BWRI(variable, target_addr) (variable =\
                                                   target_addr |\
                                                   0x05000000 /*Block Write*/)


/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/
typedef struct _EDDP_DDB_CMD_TYPE
{   
    LSA_UINT16                  MaxAcwCountPerCmd;
    LSA_UINT16                  CrtCmdDepth;
    LSA_UINT16                  CrtCmdDepthAlloc;
    EDDP_CMD_QUEUE_INFO_TYPE    Queue[EDDP_CMD_QUEUE_MGMT_SIZE_8BYTE];	
} EDDP_DDB_CMD_TYPE;

typedef struct _EDDP_DDB_CMD_TYPE  EDDP_LOCAL_MEM_ATTR *  EDDP_LOCAL_DDB_CMD_PTR_TYPE;

/*===========================================================================*/
/*                                prototyping                                */
/*===========================================================================*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CMDDeviceSetup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DSB_PTR_TYPE     pDSB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CMDDeviceShutdown(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);
													
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CMDDeviceOpen(	
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DPB_PTR_TYPE     pDPB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CMDDeviceClose(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CMD_ISR_ReqDone_IFA(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr);

LSA_VOID _EDDP_CMD_Write_QueueMgmt_Entry(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  QueueIndex,
    LSA_UINT32                  queue_start_address,
    LSA_UINT32                  queue_length);

LSA_VOID _EDDP_CMD_Setup_QueueMgmtTable(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID _EDDP_CMD_Delete_QueueMgmt_Entry(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  QueueIndex);

LSA_VOID _EDDP_CMD_Delete_QueueMgmtTable(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

EDD_COMMON_MEM_PTR_TYPE _EDDP_CMD_FillCommandBlockAndParam(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_CMD_PARAM_PTR_TYPE     pParam);

EDD_RSP _EDDP_CMD_ReadCommandReturnValue(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_CMD_PARAM_PTR_TYPE     pParam);

EDD_RSP _EDDP_CMD_CheckCmdOpcodeParams(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  CmdOpcode);

EDD_RSP _EDDP_CMD_IsCmdOpcodeValid(
    LSA_UINT32      CmdOpcode);

LSA_VOID _EDDP_CMD_FinishCommandQueue(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  QueueIndex);

LSA_VOID _EDDP_CMD_FinishCrtCommandQueue(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  QueueIndex);

/*****************************************************************************/
/*  end of file eddp_cmd_int.h                                               */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_CMD_INT_H */
