
#ifndef EDDP_HAL_INC_H                   /* ----- reinclude-protection ----- */
#define EDDP_HAL_INC_H

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
/*  F i l e               &F: eddp_hal_inc.h                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Internal headerfile for debugging                                        */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  20.10.08    SF    initial version.                                       */
#endif
/*****************************************************************************/

//#include "eddp_hal_pnip_reg.h"          /* HAL PNIP register defines        */
#include "pnip_xrsl.h"                  /* HAL PNIP register defines        */
#include "eddp_hal_pnip_struct.h"       /* HAL PNIP structures and defiens  */
#include "eddp_hal_pnip_val.h"          /* HAL PNIP values for registers    */

/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/

#ifdef EDDP_CFG_BIG_ENDIAN
    #define EDDP_HAL_RESET_CHK_A    0x52657365
    #define EDDP_HAL_RESET_CHK_B    0x7463686B
#else
    #define EDDP_HAL_RESET_CHK_A    0x65736552
    #define EDDP_HAL_RESET_CHK_B    0x6B686374
#endif
#define EDDP_HAL_RESET_CHK_SIZE 8

#define EDDP_HAL_WR_RESET_CHK(ptr)      { ptr[0]=EDDP_HAL_RESET_CHK_A; ptr[1]=EDDP_HAL_RESET_CHK_B; }
#define EDDP_HAL_IS_RESET_CHK_SET(ptr)  ( (ptr[0]==EDDP_HAL_RESET_CHK_A) && (ptr[1]==EDDP_HAL_RESET_CHK_B) )

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

// Arguments for: 
//  a) EDDP_HAL_RAMTYPE_XXX: in EDDP_HALGetRAMBaseAddress(a) 
//  b) EDDP_HAL_RAMSUB_XXX : in EDDP_HALGetRAMSubAddress(a,b)
#define EDDP_HAL_RAMTYPE_STATISTIC      (0)                             // 0
	//------------------------------------------------------------
    #define EDDP_HAL_RAMSUB_STATISTIC_COUNT                     1

#define EDDP_HAL_RAMTYPE_SYNC           (EDDP_HAL_RAMTYPE_STATISTIC+1)  // 1
	//------------------------------------------------------------
    #define EDDP_HAL_RAMSUB_SYNC_CMD_PARAM			            0
	//------------------------------------------------------------
    #define EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P1	            1
    #define EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P2	            2
    #define EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P3	            3
    #define EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P4	            4 
    //------------------------------------------------------------
    #define EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P1	            5
    #define EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P2	            6
    #define EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P3	            7
    #define EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P4	            8        
	//------------------------------------------------------------
    #define EDDP_HAL_RAMSUB_SYNC_TIME_LIST	                    9        
	//------------------------------------------------------------ 	    
    #define EDDP_HAL_RAMSUB_SYNC_COUNT                         10


#define EDDP_HAL_RAMTYPE_PACK_DATA      (EDDP_HAL_RAMTYPE_SYNC+1)       // 2
	//------------------------------------------------------------
    #define EDDP_HAL_RAMSUB_PACK_DATA_COUNT                     1

#define EDDP_HAL_RAMTYPE_CMD            (EDDP_HAL_RAMTYPE_PACK_DATA+1)  // 3
	//------------------------------------------------------------
    #define EDDP_HAL_RAMSUB_CMD_QUEUE_MGMT			            0
	//------------------------------------------------------------
    #define EDDP_HAL_RAMSUB_CMD_KRISC				            1
	//------------------------------------------------------------
    #define EDDP_HAL_RAMSUB_CMD_USER_CMD_INFO_BLOCK             2
    //------------------------------------------------------------
	#define EDDP_HAL_RAMSUB_CMD_REQUEST_HOST_ASYNC	            3
    #define EDDP_HAL_RAMSUB_CMD_COMMAND_HOST_ASYNC	            4
    #define EDDP_HAL_RAMSUB_CMD_CRT_COMMAND_HOST_ASYNC          5
	#define EDDP_HAL_RAMSUB_CMD_PARAM_HOST_ASYNC	            6
	//------------------------------------------------------------
	#define EDDP_HAL_RAMSUB_CMD_REQUEST_HOST_SYNC	            7
    #define EDDP_HAL_RAMSUB_CMD_COMMAND_HOST_SYNC	            8
	#define EDDP_HAL_RAMSUB_CMD_PARAM_HOST_SYNC		            9
	//------------------------------------------------------------
	#define EDDP_HAL_RAMSUB_CMD_REQUEST_USER_ASYNC	            10
    #define EDDP_HAL_RAMSUB_CMD_COMMAND_USER_ASYNC	            11
	#define EDDP_HAL_RAMSUB_CMD_PARAM_USER_ASYNC	            12
	//------------------------------------------------------------
    #define EDDP_HAL_RAMSUB_CMD_REQUEST_USER_SYNC	            13
    #define EDDP_HAL_RAMSUB_CMD_COMMAND_USER_SYNC	            14
	#define EDDP_HAL_RAMSUB_CMD_PARAM_USER_SYNC		            15
	//------------------------------------------------------------
	#define EDDP_HAL_RAMSUB_CMD_COUNT				            16

#define EDDP_HAL_RAMTYPE_API_CTRL       (EDDP_HAL_RAMTYPE_CMD+1)        // 4
    #define EDDP_HAL_RAMSUB_APICTRL_RESET_CHECK                 0
    // -----------------------------------------------------------
    #define EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_PROV_ACW            1
    #define EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_PROV_ACW_DG         2
    #define EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_CONS_ACW            3
    #define EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_CONS_APDU           4
    #define EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_CONS_ACW_DG         5
    #define EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_CONS_DG_STATUS      6
    #define EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_TBASE_LIST          7
    #define EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_TROG_LIST           8
    #define EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_DS_LIST             9
    #define EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_RBASE_LIST          10   /* ERTEC200P only! */
    #define EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_CONS_TSB            11
    #define EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_CONS_CSB0           12
    #define EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_CONS_CSB1           13
    #define EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_RECV_TABLE          14   /* HERA only! CPM receive table for hardware */
    #define EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_CONS_IO_NBUF        15   /* HERA only! CPM consumer IO NBuf table for hardware */
    #define EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_HW_SEND_LIST_M1     16   /* HERA only! PPM hardware send list M1 */
    #define EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_HW_SEND_LIST_M2     17   /* HERA only! PPM hardware send list M2 */
    #define EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_HW_SEND_LIST_M3     18   /* HERA only! PPM hardware send list M3 */
    #define EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_HW_SEND_LIST_M4     19   /* HERA only! PPM hardware send list M4 */
    #define EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_HW_SEND_LIST_N1     20   /* HERA only! PPM hardware send list N1 */
    #define EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_HW_SEND_LIST_N2     21   /* HERA only! PPM hardware send list N2 */
    #define EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_HW_SEND_LIST_N3     22   /* HERA only! PPM hardware send list N3 */
    #define EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_HW_SEND_LIST_N4     23   /* HERA only! PPM hardware send list N4 */
    // -----------------------------------------------------------
    #define EDDP_HAL_RAMSUB_APICTRL_NRT_COMMON_CODE             24
    #define EDDP_HAL_RAMSUB_APICTRL_NRT_IFA_MGM                 25
    #define EDDP_HAL_RAMSUB_APICTRL_NRT_IFA_DCPPORT_FILTER      26
    #define EDDP_HAL_RAMSUB_APICTRL_NRT_IFA_DCPHELLO_FILTER     27
    // -----------------------------------------------------------
    
    #define EDDP_HAL_RAMSUB_APICTRL_COMMON_CMD_ARGS             28
    // -----------------------------------------------------------
    #define EDDP_HAL_RAMSUB_API_CTRL_COUNT                      29
    
#define EDDP_HAL_RAMTYPE_API_NRT_CTRL   (EDDP_HAL_RAMTYPE_API_CTRL+1)   // 5
    #define EDDP_HAL_RAMSUB_API_NRT_CTRL_RESET_CHECK                 0
    // -----------------------------------------------------------
    #define EDDP_HAL_RAMSUB_API_NRT_CTRL_NRT_COMMON_CODE             1
    #define EDDP_HAL_RAMSUB_API_NRT_CTRL_NRT_IFA_MGM                 2
    // -----------------------------------------------------------
    #define EDDP_HAL_RAMSUB_API_NRT_CTRL_COMMON_CMD_ARGS             9
    // -----------------------------------------------------------
    #define EDDP_HAL_RAMSUB_API_NRT_CTRL_COUNT                       10
    
#define EDDP_HAL_RAMTYPE_FDB_TABLE      (EDDP_HAL_RAMTYPE_API_NRT_CTRL+1)   // 6
	#define EDDP_HAL_RAMSUB_FDB_TABLE_PARAM_STRUCT	            0
	#define EDDP_HAL_RAMSUB_FDB_TABLE_MC_TABLE_A	            1
	#define EDDP_HAL_RAMSUB_FDB_TABLE_MC_TABLE_B	            2
	#define EDDP_HAL_RAMSUB_FDB_TABLE_UC_TABLE_A	            3
	#define EDDP_HAL_RAMSUB_FDB_TABLE_IRT_TABLE_A	            4
	#define EDDP_HAL_RAMSUB_FDB_TABLE_UC_TABLE_B	            5
	#define EDDP_HAL_RAMSUB_FDB_TABLE_IRT_TABLE_B	            6    
	//------------------------------------------------------------
	#define EDDP_HAL_RAMSUB_FDB_TABLE_COUNT			            7

#define EDDP_HAL_RAMTYPE_VLAN_TABLE     (EDDP_HAL_RAMTYPE_FDB_TABLE+1)  // 7
	//------------------------------------------------------------
    #define EDDP_HAL_RAMSUB_VLAN_TABLE_COUNT                    1

#define EDDP_HAL_RAMTYPE_QUEUE_CTRL     (EDDP_HAL_RAMTYPE_VLAN_TABLE+1) // 8
	//------------------------------------------------------------
    #define EDDP_HAL_RAMSUB_QUEUE_CTRL_COUNT                    1

#define EDDP_HAL_RAMTYPE_QUEUE_DATA     (EDDP_HAL_RAMTYPE_QUEUE_CTRL+1) // 9
	//------------------------------------------------------------
    #define EDDP_HAL_RAMSUB_QUEUE_DATA_COUNT                    1

#define EDDP_HAL_RAMTYPE_API_PST     (EDDP_HAL_RAMTYPE_QUEUE_DATA+1)    // 10
	//------------------------------------------------------------
    #define EDDP_HAL_RAMSUB_API_PST_DCPHELLO_FILTER             0
    #define EDDP_HAL_RAMSUB_API_PST_DCPPORT_FILTER              1
    #define EDDP_HAL_RAMSUB_API_PST_COUNT                       2

#define EDDP_HAL_RAMTYPE_COMP_COUNT     (EDDP_HAL_RAMTYPE_API_PST+1) // 11: nr of components



/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                                                           */
/* Interrupt Management types                                                */
/*                                                                           */
/*===========================================================================*/

typedef struct _EDDP_HAL_ISR_GRP_REGS_TYPE  EDD_COMMON_MEM_ATTR *    EDDP_HAL_ISR_GRP_REGS_PTR_TYPE;
typedef struct _EDDP_HAL_ISR_GRP_REGS_TYPE
{
    LSA_UINT32  Low;
    LSA_UINT32  Mid;
    LSA_UINT32  High;
} EDDP_HAL_ISR_GRP_REGS_TYPE;


typedef struct _EDDP_HAL_ISR_TYPE 
{
	LSA_UINT32                  CPUSubys;       /* CPU-Subsystem to use     */
    LSA_UINT32                  EOI_WaitTime;
} EDDP_HAL_ISR_TYPE;

#define EDDP_ISR_MIN_EOI_WAITTIME  0x0002
#define EDDP_ISR_MAX_EOI_WAITTIME  0x3FFF
#define EDDP_ISR_MAX_IRQ_WAITTIME  0x01FF


#define EDDP_HAL_ISR_SUBSYS_CNT       3
#define EDDP_HAL_ISR_MUX_CNT         15

typedef struct _EDDP_HAL_ISR_MUX_REGISTER_MAPPING_TYPE
{
	LSA_UINT32                      IRQActivate;
    LSA_UINT32                      IRQControlMux[EDDP_HAL_ISR_MUX_CNT];
} EDDP_HAL_ISR_MUX_REGISTER_MAPPING_TYPE;

/* CPUSubsys (will be set depending on EDDPLocation from DPB/DDB!) */
#define EDDP_HAL_ISR_SUBSYS_KRISC     0  /* not used within EDDP! */
#define EDDP_HAL_ISR_SUBSYS_LOCAL     1
#define EDDP_HAL_ISR_SUBSYS_EXT_HOST  2



LSA_EXTERN const EDDP_HAL_ISR_MUX_REGISTER_MAPPING_TYPE EDDP_HalIsrMUXRegSubSys[EDDP_HAL_ISR_SUBSYS_CNT];

/*===========================================================================*/
/*                                                                           */
/* Memory Management types                                                   */
/*                                                                           */
/*===========================================================================*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Note: typedef for alignment selection in  EDDP_HAL_RAM_SUB_COMP_TYPE 
typedef enum
{
    EDDP_ALIGMNT_32BIT   = 0,   //   4 Byte
    EDDP_ALIGMNT_64BIT   = 1,   //   8 Byte
    EDDP_ALIGMNT_128BIT  = 2,   //  16 Byte
    EDDP_ALIGMNT_256BIT  = 3,   //  32 Byte
    EDDP_ALIGMNT_512BIT  = 4,   //  64 Byte
    EDDP_ALIGMNT_1024BIT = 5    // 128 Byte
} EDDP_HAL_ARG_ALIGMNT_TYPE;


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Note: typedef to handle subcomponents 
//       Start of this RAM can be forced to different alignments but 
//       alignment within this RAM part must be placed internally
typedef struct _EDDP_HAL_RAM_SUB_COMP_TYPE        EDDP_LOCAL_MEM_ATTR *    EDDP_HAL_LOCAL_RAM_SUB_COMP_PTR_TYPE;
typedef struct _EDDP_HAL_RAM_SUB_COMP_TYPE
{
    EDDP_HAL_ARG_ALIGMNT_TYPE   RamSubComponentBitAlignmt;          // see EDDP_HAL_ARG_ALIGMNT_TYPE
    EDD_COMMON_MEM_PTR_TYPE     pSubComponentRamBase;               // Base Address for RamComponent (Host view)
    LSA_UINT32                  SubComponentRamSize;                // full length of RAM-structure in bytes reserved for appl. usage !!!
    LSA_BOOL                    RamSubComponentUseBorderCheck;      // FALSE: no support, TRUE: append special value to test for overwriting
    EDD_COMMON_MEM_U32_PTR_TYPE pSubComponentBorderCheck;           // points to special border value if enabled
    LSA_UINT32                  SubComponentUsedSize;               // full length needed incl. alignment/border cell in bytes !!!
} EDDP_HAL_RAM_SUB_COMP_TYPE;


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Note: This structure is a template only to have a common access to the specific RAMtypes
//       Common members in this structure must be are before ComponentRamArray[] !!!!
typedef struct _EDDP_HAL_RAM_COMP_COMMON_TYPE   EDDP_LOCAL_MEM_ATTR *    EDDP_HAL_LOCAL_RAM_COMP_COMMON_PTR_TYPE;
typedef struct _EDDP_HAL_RAM_COMP_COMMON_TYPE
{
    EDD_COMMON_MEM_PTR_TYPE     pComponentRamBase;                  // Base Address for RamComponent (Host view)
    LSA_UINT32                  ComponentRamSize;                   // full length of RamComponent
    LSA_UINT32                  ComponentRamFreeSpace;              // space be left
    LSA_BOOL                    Is8BitAccessAllowed;                // true if 8bit access supported on this RAM

    LSA_UINT32                  ComponentRamCount;                  // number of SubComponents
    EDDP_HAL_RAM_SUB_COMP_TYPE  ComponentRamArray[1];               // number of items depends on specific RAMtype
} EDDP_HAL_RAM_COMP_COMMON_TYPE;



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Note: Start of this RAM is defined by ASIC
typedef struct _EDDP_HAL_RAM_COMP_QUEUEDATA_TYPE  // ::EDDP_HAL_RAM_COMP_COMMON_TYPE
{
    EDD_COMMON_MEM_PTR_TYPE     pComponentRamBase;                  // Base Address for RamComponent (Host view)
    LSA_UINT32                  ComponentRamSize;                   // full length of RamComponent
    LSA_UINT32                  ComponentRamFreeSpace;              // space be left
    LSA_BOOL                    Is8BitAccessAllowed;                // true if 8bit access supported on this RAM

    LSA_UINT32                  ComponentRamCount;                  // number of SubComponents
    EDDP_HAL_RAM_SUB_COMP_TYPE  ComponentRamArray[EDDP_HAL_RAMSUB_QUEUE_DATA_COUNT];
} EDDP_HAL_RAM_COMP_QUEUEDATA_TYPE;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Note: Start of this RAM is defined by ASIC
typedef struct _EDDP_HAL_RAM_COMP_QUEUECTRL_TYPE  // ::EDDP_HAL_RAM_COMP_COMMON_TYPE
{
    EDD_COMMON_MEM_PTR_TYPE     pComponentRamBase;                  // Base Address for RamComponent (Host view)
    LSA_UINT32                  ComponentRamSize;                   // full length of RamComponent
    LSA_UINT32                  ComponentRamFreeSpace;              // space be left
    LSA_BOOL                    Is8BitAccessAllowed;                // true if 8bit access supported on this RAM

    LSA_UINT32                  ComponentRamCount;                  // number of SubComponents
    EDDP_HAL_RAM_SUB_COMP_TYPE  ComponentRamArray[EDDP_HAL_RAMSUB_QUEUE_CTRL_COUNT];
} EDDP_HAL_RAM_COMP_QUEUECTRL_TYPE;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Note: Start of this RAM is defined by ASIC
typedef struct _EDDP_HAL_RAM_COMP_VLANTABLE_TYPE  // ::EDDP_HAL_RAM_COMP_COMMON_TYPE
{
    EDD_COMMON_MEM_PTR_TYPE     pComponentRamBase;                  // Base Address for RamComponent (Host view)
    LSA_UINT32                  ComponentRamSize;                   // full length of RamComponent
    LSA_UINT32                  ComponentRamFreeSpace;              // space be left
    LSA_BOOL                    Is8BitAccessAllowed;                // true if 8bit access supported on this RAM

    LSA_UINT32                  ComponentRamCount;                  // number of SubComponents
    EDDP_HAL_RAM_SUB_COMP_TYPE  ComponentRamArray[EDDP_HAL_RAMSUB_VLAN_TABLE_COUNT];
} EDDP_HAL_RAM_COMP_VLANTABLE_TYPE;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Note: Start of this RAM is defined by ASIC
typedef struct _EDDP_HAL_RAM_COMP_FDBTABLE_TYPE   // ::EDDP_HAL_RAM_COMP_COMMON_TYPE
{
    EDD_COMMON_MEM_PTR_TYPE     pComponentRamBase;                  // Base Address for RamComponent (Host view)
    LSA_UINT32                  ComponentRamSize;                   // full length of RamComponent
    LSA_UINT32                  ComponentRamFreeSpace;              // space be left
    LSA_BOOL                    Is8BitAccessAllowed;                // true if 8bit access supported on this RAM

    LSA_UINT32                  ComponentRamCount;                  // number of SubComponents
    EDDP_HAL_RAM_SUB_COMP_TYPE  ComponentRamArray[EDDP_HAL_RAMSUB_FDB_TABLE_COUNT];
} EDDP_HAL_RAM_COMP_FDBTABLE_TYPE;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Note: Start of this RAM is defined by ASIC
typedef struct _EDDP_HAL_RAM_COMP_APICTRL_TYPE    // ::EDDP_HAL_RAM_COMP_COMMON_TYPE
{
    EDD_COMMON_MEM_PTR_TYPE     pComponentRamBase;                  // Base Address for RamComponent (Host view)
    LSA_UINT32                  ComponentRamSize;                   // full length of RamComponent
    LSA_UINT32                  ComponentRamFreeSpace;              // space be left
    LSA_BOOL                    Is8BitAccessAllowed;                // true if 8bit access supported on this RAM

    LSA_UINT32                  ComponentRamCount;                  // number of SubComponents
    EDDP_HAL_RAM_SUB_COMP_TYPE  ComponentRamArray[EDDP_HAL_RAMSUB_API_CTRL_COUNT];
} EDDP_HAL_RAM_COMP_APICTRL_TYPE;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Note: Start of this RAM is defined by ASIC
typedef struct _EDDP_HAL_RAM_COMP_CMD_TYPE        // ::EDDP_HAL_RAM_COMP_COMMON_TYPE
{
    EDD_COMMON_MEM_PTR_TYPE     pComponentRamBase;                  // Base Address for RamComponent (Host view)
    LSA_UINT32                  ComponentRamSize;                   // full length of RamComponent
    LSA_UINT32                  ComponentRamFreeSpace;              // space be left
    LSA_BOOL                    Is8BitAccessAllowed;                // true if 8bit access supported on this RAM

    LSA_UINT32                  ComponentRamCount;                  // number of SubComponents
    EDDP_HAL_RAM_SUB_COMP_TYPE  ComponentRamArray[EDDP_HAL_RAMSUB_CMD_COUNT];
} EDDP_HAL_RAM_COMP_CMD_TYPE;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Note: Start of this RAM is defined by ASIC
typedef struct _EDDP_HAL_RAM_COMP_PACKDATA_TYPE   // ::EDDP_HAL_RAM_COMP_COMMON_TYPE
{
    EDD_COMMON_MEM_PTR_TYPE     pComponentRamBase;                  // Base Address for RamComponent (Host view)
    LSA_UINT32                  ComponentRamSize;                   // full length of RamComponent
    LSA_UINT32                  ComponentRamFreeSpace;              // space be left
    LSA_BOOL                    Is8BitAccessAllowed;                // true if 8bit access supported on this RAM

    LSA_UINT32                  ComponentRamCount;                  // number of SubComponents
    EDDP_HAL_RAM_SUB_COMP_TYPE  ComponentRamArray[EDDP_HAL_RAMSUB_PACK_DATA_COUNT];
} EDDP_HAL_RAM_COMP_PACKDATA_TYPE;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Note: Start of this RAM is defined by ASIC
typedef struct _EDDP_HAL_RAM_COMP_SYNC_TYPE       // ::EDDP_HAL_RAM_COMP_COMMON_TYPE
{
    EDD_COMMON_MEM_PTR_TYPE     pComponentRamBase;                  // Base Address for RamComponent (Host view)
    LSA_UINT32                  ComponentRamSize;                   // full length of RamComponent
    LSA_UINT32                  ComponentRamFreeSpace;              // space be left
    LSA_BOOL                    Is8BitAccessAllowed;                // true if 8bit access supported on this RAM

    LSA_UINT32                  ComponentRamCount;                  // number of SubComponents
    EDDP_HAL_RAM_SUB_COMP_TYPE  ComponentRamArray[EDDP_HAL_RAMSUB_SYNC_COUNT];
} EDDP_HAL_RAM_COMP_SYNC_TYPE;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Note: Start of this RAM is defined by ASIC
typedef struct _EDDP_HAL_RAM_COMP_STATISTIC_TYPE  // ::EDDP_HAL_RAM_COMP_COMMON_TYPE
{
    EDD_COMMON_MEM_PTR_TYPE     pComponentRamBase;                  // Base Address for RamComponent (Host view)
    LSA_UINT32                  ComponentRamSize;                   // full length of RamComponent
    LSA_UINT32                  ComponentRamFreeSpace;              // space be left
    LSA_BOOL                    Is8BitAccessAllowed;                // true if 8bit access supported on this RAM

    LSA_UINT32                  ComponentRamCount;                  // number of SubComponents
    EDDP_HAL_RAM_SUB_COMP_TYPE  ComponentRamArray[EDDP_HAL_RAMSUB_STATISTIC_COUNT];
} EDDP_HAL_RAM_COMP_STATISTIC_TYPE;


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Note: Start of this RAM is defined by ASIC
typedef struct _EDDP_HAL_RAM_COMP_APIPST_TYPE    // ::EDDP_HAL_RAM_COMP_COMMON_TYPE
{
    EDD_COMMON_MEM_PTR_TYPE     pComponentRamBase;                  // Base Address for RamComponent (Host view)
    LSA_UINT32                  ComponentRamSize;                   // full length of RamComponent
    LSA_UINT32                  ComponentRamFreeSpace;              // space be left
    LSA_BOOL                    Is8BitAccessAllowed;                // true if 8bit access supported on this RAM

    LSA_UINT32                  ComponentRamCount;                  // number of SubComponents
    EDDP_HAL_RAM_SUB_COMP_TYPE  ComponentRamArray[EDDP_HAL_RAMSUB_API_PST_COUNT];
} EDDP_HAL_RAM_COMP_APIPST_TYPE;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
typedef struct _EDDP_HAL_RAMLAYOUT_TYPE    EDDP_LOCAL_MEM_ATTR *    EDDP_HAL_RAMLAYOUT_PTR_TYPE;
typedef struct _EDDP_HAL_RAMLAYOUT_TYPE
{
    EDD_COMMON_MEM_PTR_TYPE                 pPnipRamBase;                       // Host Base Address for PNIP mapped memory, NULL if error on Startup !!!
    LSA_BOOL                                SetupDone;                          // true if HALSetup() executed and WrChecker initialized 
    EDDP_HAL_RAM_COMP_QUEUEDATA_TYPE        RamComponentQueueData;
    EDDP_HAL_RAM_COMP_QUEUECTRL_TYPE        RamComponentQueueCtrl;
    EDDP_HAL_RAM_COMP_VLANTABLE_TYPE        RamComponentVlanTable;
    EDDP_HAL_RAM_COMP_FDBTABLE_TYPE         RamComponentFdbTable;
    EDDP_HAL_RAM_COMP_APICTRL_TYPE          RamComponentApiCtrl;                // with Rev1/2 of PNIP this element is for both. RT and NRT. With Rev3 this is for RT
    EDDP_HAL_RAM_COMP_APICTRL_TYPE          RamComponentApiNRTCtrl;             // is not used with Rev1/2
    EDDP_HAL_RAM_COMP_CMD_TYPE              RamComponentCmd;
    EDDP_HAL_RAM_COMP_PACKDATA_TYPE         RamComponentPackData;
    EDDP_HAL_RAM_COMP_SYNC_TYPE             RamComponentSync;
    EDDP_HAL_RAM_COMP_STATISTIC_TYPE        RamComponentStatistic;
    EDDP_HAL_RAM_COMP_APIPST_TYPE           RamComponentApiPst;                 // only with Rev3
    EDDP_HAL_LOCAL_RAM_COMP_COMMON_PTR_TYPE RamCommonArrayPtr[EDDP_HAL_RAMTYPE_COMP_COUNT]; // pointer to RamComponentQueueData .. RamComponentStatistic
} EDDP_HAL_RAMLAYOUT_TYPE;


/*===========================================================================*/
/*                                                                           */
/* HAL Management main infos (located within DDB)                            */
/*                                                                           */
/*===========================================================================*/

typedef struct _EDDP_DDB_CORE_HAL_TYPE	EDDP_LOCAL_MEM_ATTR *	EDDP_LOCAL_DDB_CORE_HAL_PTR_TYPE;
typedef struct _EDDP_DDB_CORE_HAL_TYPE 
{
    EDDP_HAL_RAMLAYOUT_TYPE		    HalRamLayout;
	EDDP_HAL_ISR_TYPE               Isr;
	LSA_UINT16					    PNIP_Revision;
    LSA_UINT16                      HardwareIF;                 /* internal HW identifier    */
                                                                /* #defines see DPB.         */
                                                                /* e.g. EDDP_HW_ERTEC200P    */
    LSA_UINT16                      HardwareVersion;            /* Revision of Chip          */
                                                                /* ERTEC200+ Step1 = EDDP_HAL_HW_PNIP_REV1    */
                                                                /* ERTEC200+ Step2 = EDDP_HAL_HW_PNIP_REV2    */
                                                                /* HERA Step1      = EDDP_HAL_HW_PNIP_REV3    */
    LSA_UINT16                      HardwareStep;               /* Step of Chip              */
                                                                /* Step1 = ERTEC200+ Step1, HERA Step1 */
                                                                /* Step2 = ERTEC200+ Step2,  */
    LSA_UINT16                      HardwareChipType;           /* Type of Chip wich PNIP contains */
                                                                /* ASIC/FPGA                 */
    LSA_UINT16                      HardwareChipVersion;        /* Version of Chip wich PNIP contains */
                                                                /* ERTEC200P/HERA            */
    LSA_UINT16                      HardwarePortCntType;        /* Version of interface 2 or 4 ports */
    LSA_UINT16                      AllocHwResourceCrtCmd;      /* allocation size of CRT command HW resources */

} EDDP_DDB_CORE_HAL_TYPE;
/* HalRamLayout		: see <eddp_hal_ram.c>                                  */
/* Isr				: Isr data of HAL										*/
/* PNIP_Revision	: PNIP-Revision is set in HALDeviceOpen                 */
#define EDDP_HAL_HW_PNIP_R1A1       1   // ERTEC200P (Rev1)
#define EDDP_HAL_HW_PNIP_R1F1       2   // FPGA (Rev1)
#define EDDP_HAL_HW_PNIP_R2A1       3   // ERTEC200P (Rev2)
#define EDDP_HAL_HW_PNIP_R2F1       4   // FPGA (Rev2)
#define EDDP_HAL_HW_PNIP_R3A1       5   // HERA ASIC (Rev3)
#define EDDP_HAL_HW_PNIP_R3F1       6   // HERA FPGA (Rev3)
/*					: register "IP_Development.Inkr(15:11)"					*/
//#define EDDP_HAL_REG_IP_DEVELOPMENT_INKR_REV1       0x4     // Inkr = "00100" (Rev1)
//#define EDDP_HAL_REG_IP_DEVELOPMENT_INKR_REV2       0x5     // Inkr = "00101" (Rev2)
/*					: register "IP_Version.Version(15:8)"					*/
#define EDDP_HAL_REG_IP_VERSION_VERSION_REV1        0x1     // Rev1 = "00000001"
#define EDDP_HAL_REG_IP_VERSION_VERSION_REV2        0x2     // Rev2 = "00000010"
#define EDDP_HAL_REG_IP_VERSION_VERSION_REV3        0x3     // Rev3 = "00000011"


/*                  : register "IP_Development.Type(20:19)"                   */
#define EDDP_HAL_REG_IP_DEVELOPMENT_TYPE_ASIC       0x0
#define EDDP_HAL_REG_IP_DEVELOPMENT_TYPE_FPGA       0x1
#define EDDP_HAL_REG_IP_DEVELOPMENT_TYPE_RESERVED   0x2
#define EDDP_HAL_REG_IP_DEVELOPMENT_TYPE_USER_DEF   0x3

/*                  : interpretation of register "IP_Version.Configuration (31:16)"                   */
#define EDDP_HAL_HW_PNIP_CFG_2PORT                  0x1
#define EDDP_HAL_HW_PNIP_CFG_4PORT                  0x2

#define EDDP_HAL_HW_PNIP_REV1                       EDDP_HAL_REG_IP_VERSION_VERSION_REV1
#define EDDP_HAL_HW_PNIP_REV2                       EDDP_HAL_REG_IP_VERSION_VERSION_REV2
#define EDDP_HAL_HW_PNIP_REV3                       EDDP_HAL_REG_IP_VERSION_VERSION_REV3

#define EDDP_HAL_HW_TYPE_ASIC                       1  //ASIC Version
#define EDDP_HAL_HW_TYPE_FPGA                       2  //FPGA Version

#define EDDP_HAL_HW_ERTEC200P                       1  //ERTED200P Step1/Step2 ASIC/FPGA
#define EDDP_HAL_HW_HERA                            2  //HERA FPGA

#define EDDP_HAL_HW_STEP1                           1  // ERTEC200P Step 1, HERA Step1
#define EDDP_HAL_HW_STEP2                           2  // ERTEC200P Step 2

#define EDDP_HAL_HW_IF_A                            1  // IF A: ERTEC200P, HERA ASIC/FPGA
#define EDDP_HAL_HW_IF_B                            2  // IF B: HERA ASIC/FPGA

/*  */
/**
@ingroup        IP_VERSION
@param Address  0x00000000
@param Mask     0xFFFF0000
@param Shift    16
@param Access   r,
@param Reset    0x00000021
*/
#define EDDP_HAL_R3_REG_IP_VERSION__MSK_CONFIGURATION                                                   0xFFFF0000
#define EDDP_HAL_R3_REG_IP_VERSION_SHFT_CONFIGURATION                                                   16
#define EDDP_HAL_R3_REG_IP_VERSION_RSTV_CONFIGURATION                                                   0x00000021

#define EDDP_HAL_REG_IP_VERSION_VALUE_CONFIGURATION_2IF                                                 0x00000000
#define EDDP_HAL_REG_IP_VERSION_VALUE_CONFIGURATION_4P_R2F0_1IF                                         0x00000001
#define EDDP_HAL_REG_IP_VERSION_VALUE_CONFIGURATION_2P_R2F0_1IF                                         0x00000002


#define EDDP_HAL_REG_IP_VERSION_VALUE_CONFIGURATION_4P_R3F0_1IFA_INC4                                   0x00000004
#define EDDP_HAL_REG_IP_VERSION_VALUE_CONFIGURATION_4P_R3F0_1IFA_INC10                                  0x0000000A
#define EDDP_HAL_REG_IP_VERSION_VALUE_CONFIGURATION_4P_R3F0_1IFA_INC8                                   0x00000008
#define EDDP_HAL_REG_IP_VERSION_VALUE_CONFIGURATION_4P_R3F0_1IFA_INC12                                  0x0000000C
#define EDDP_HAL_REG_IP_VERSION_VALUE_CONFIGURATION_2P_R3F0_1IFB_INC14                                  0x0000000E

#define EDDP_HAL_REG_IP_VERSION_VALUE_CONFIGURATION_2P_R2_1IF                                           0x00000012

#define EDDP_HAL_REG_IP_VERSION_VALUE_CONFIGURATION_4P_R3A0_1IFA                                        0x00000021
#define EDDP_HAL_REG_IP_VERSION_VALUE_CONFIGURATION_2P_R3A0_1IFB                                        0x00000022


/*===========================================================================*/
/*                                prototyping                                */
/*===========================================================================*/


/*****************************************************************************/
/*  end of file EDDP_HAL_INC.H                                               */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_HAL_INC_H */
