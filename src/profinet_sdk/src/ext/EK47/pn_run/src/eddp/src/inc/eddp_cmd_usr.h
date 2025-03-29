#ifndef EDDP_CMD_USR_H                   /* ----- reinclude-protection ----- */
#define EDDP_CMD_USR_H

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
/*  F i l e               &F: eddp_cmd_usr.h                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  EDDP PNIP Command management                                             */
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

/*===========================================================================*/
/*                                defines                                    */
/*===========================================================================*/
    
/*---------------------------------------------------------------------------*/
/* Bit masks                                                                 */
/*---------------------------------------------------------------------------*/
// mask of RequestStatus in RequestEntry
#define EDDP_CMD_REQUEST_STATUS_MASK            0xC0000000
#define EDDP_CMD_REQUEST_FREE                   0x00000000
#define EDDP_CMD_REQUEST_WAITING                0x40000000
#define EDDP_CMD_REQUEST_RUNNING                0x80000000
#define EDDP_CMD_REQUEST_CONFIRMED              0xC0000000

// mask of asyn Queues(0..8)
#define EDDP_CMD_CONFIRMINGQUEUE_HOST_MASK      0x000001FF

/* these defines are used for internal structure sizes and array sizes, take maximum */
#define EDDP_CMD_MAX_ACW_CNT_PER_CMD            4 /* Rev1/2 : MRPD im Stich bei 2 Ports --> 4 ACW's */

/* determine the maximum amount of ACW's within a single command depending on revision */
#define EDDP_CMD_MAX_ACW_CNT_PER_CMD_REV12      4
#define EDDP_CMD_MAX_ACW_CNT_PER_CMD_REV3       8

// EDDP_CMD_MAX_ACW_CNT_PER_CMD is used for internal structure sizes and array sizes independing of revision
// check against EDDP_CMD_MAX_ACW_CNT_PER_CMD_REV12 for ERTEC200P support
#ifdef EDDP_CFG_HW_ERTEC200P_SUPPORT
#if (EDDP_CMD_MAX_ACW_CNT_PER_CMD_REV12 > EDDP_CMD_MAX_ACW_CNT_PER_CMD)
#error "EDDP_CMD_MAX_ACW_CNT_PER_CMD_REV12 > EDDP_CMD_MAX_ACW_CNT_PER_CMD"
#endif
#endif
    
/* determine the maximum amount of commands that can be processed in parallel within HW */
#define EDDP_CMD_CPM_PPM_COMMAND_DEPTH_REV12     1
#define EDDP_CMD_CPM_PPM_COMMAND_DEPTH_REV3      10

/*---------------------------------------------------------------------------*/
/* Size of CMD-RAM structures                                                */
/*---------------------------------------------------------------------------*/
#define EDDP_CMD_QUEUE_MGMT_SIZE_8BYTE          32
#define EDDP_CMD_KRISC_SIZE_1BYTE               2048

/*---------------------------------------------------------------------------*/
/* Indices of queues: CmdQueue[Index]                                        */
/*---------------------------------------------------------------------------*/
#define EDDP_CMD_INDEX_HOST_ASYNC_FILTER        0
#define EDDP_CMD_INDEX_HOST_ASYNC_ARCV          1
#define EDDP_CMD_INDEX_HOST_ASYNC_ASND          2
#define EDDP_CMD_INDEX_HOST_ASYNC_CRT           3
#define EDDP_CMD_INDEX_HOST_ASYNC_PACK          5
#define EDDP_CMD_INDEX_HOST_ASYNC_FDB           6
#define EDDP_CMD_INDEX_HOST_ASYNC_VLAN          7
#define EDDP_CMD_INDEX_HOST_ASYNC_GSYNC         8
#define EDDP_CMD_INDEX_HOST_SYNC                9
#define EDDP_CMD_INDEX_USER_ASYNC_FDB           10
#define EDDP_CMD_INDEX_USER_ASYNC_GSYNC         11
#define EDDP_CMD_INDEX_USER_SYNC                12
#define EDDP_CMD_INDEX_KRISC_ASYNC_FDB          16
#define EDDP_CMD_INDEX_KRISC_ASYNC_GSYNC        17
#define EDDP_CMD_INDEX_KRISC_SYNC               18

/*---------------------------------------------------------------------------*/
/* User ID for Command Interface ParamStruct                                 */
/*---------------------------------------------------------------------------*/
#define EDDP_CMD_USER_ID_HOST_ASYNC				0
#define EDDP_CMD_USER_ID_HOST_SYNC				1
#define EDDP_CMD_USER_ID_USER_ASYNC				2
#define EDDP_CMD_USER_ID_USER_SYNC				3
#define EDDP_CMD_USER_ID_KRISC32_ASYNC			4
#define EDDP_CMD_USER_ID_KRISC32_SYNC			5
#define EDDP_CMD_USER_ID_RESERVED_ASYNC			6
#define EDDP_CMD_USER_ID_RESERVED_SYNC			7


/*---------------------------------------------------------------------------*/
/* Commands                                                                  */
/*---------------------------------------------------------------------------*/
#define EDDP_CMD_GROUP_MASK                0xFF00
#define EDDP_CMD_COMMAND_MASK              0x00FF

#define EDDP_CMD_MAX_PARAMS_FOR_SYNC_QUEUE      5

// ApiFilter
#define EDDP_CMD_FILTER_NOP                0x0000   // no operation
#define EDDP_CMD_FILTER_STOP               0x0001   // stop filter
#define EDDP_CMD_FILTER_CHANGE_BASE        0x0002   // change filtercodebase
#define EDDP_CMD_FILTER_ENABLE_STRING      0x0003   // enable stringdescriptor
#define EDDP_CMD_FILTER_CHANGE_STRING      0x0004   // change stringdescriptor
#define EDDP_CMD_FILTER_COUNT			   5		// total count of filter commands

// RcvDMACW 
#define EDDP_CMD_ARCV_NOP                  0x0100   // no operation
#define EDDP_CMD_ARCV_DISABLE              0x0101   // disable receive table
#define EDDP_CMD_ARCV_DISABLE_DSCR         0x0102   // disable receive descriptor
#define EDDP_CMD_ARCV_COPY_DSCR            0x0103   // copy RcvDMACW
#define EDDP_CMD_ARCV_COUNT				   4        // total count of ARCV commands

// SndDMACW 
#define EDDP_CMD_ASND_NOP                  0x0200   // no operation
#define EDDP_CMD_ASND_DISABLE              0x0201   // disable send table
#define EDDP_CMD_ASND_DISABLE_DSCR         0x0202   // disable send descriptor
#define EDDP_CMD_ASND_COUNT				   3		// total count of ASND commands

// CPM / AcwConsumer
#define EDDP_CMD_CPM_NOP                   0x0300   // no operation
#define EDDP_CMD_CPM_ON_OFF                0x0301   // CPM on/off
#define EDDP_CMD_CPM_ACW_ON_OFF            0x0302   // ACW on/off
#define EDDP_CMD_CPM_ACW_IN_OUT            0x0303   // ACW in/out
#define EDDP_CMD_CPM_ACW_SET_UNKNOWN       0x0304   // ACW set to unknown
#define EDDP_CMD_CPM_ACW_XCHANGE_BUFF      0x0305   // exchange buffer
#define EDDP_CMD_CPM_COUNT				   6		// total count of CPM commands

// PPM / AcwProvider
#define EDDP_CMD_PPM_NOP                   0x0306   // no operation
#define EDDP_CMD_PPM_ON_OFF                0x0307   // PPM on/off
#define EDDP_CMD_PPM_ACW_ON_OFF            0x0308   // ACW on/off
#define EDDP_CMD_PPM_ACW_IN_OUT            0x0309   // ACW in/out
#define EDDP_CMD_PPM_ACW_MOVE              0x030A   // ACW move
#define EDDP_CMD_PPM_ACW_XCHANGE_BUFF      0x030B   // exchange buffer
#define EDDP_CMD_PPM_ACW_DELAY             0x030C   // ACW in/out for delay command, only with HERA
#define EDDP_CMD_PPM_COUNT				   7		// total count of PPM commands
#define EDDP_CMD_CRT_COUNT                 (EDDP_CMD_CPM_COUNT + EDDP_CMD_PPM_COUNT)
    
// Pack
#define EDDP_CMD_PACK_NOP                  0x0500   // no operation
#define EDDP_CMD_PACK_DISABLE_GRP          0x0501   // disable packgroup
#define EDDP_CMD_PACK_ENABLE_GRP           0x0502   // enable packgroup
#define EDDP_CMD_PACK_DIS_ENA_GRP          0x0503   // disable/enable packgroup
#define EDDP_CMD_PACK_COUNT				   4		// total count of PACK commands

// FDB
#define EDDP_CMD_FDB_NOP                   0x0600   // no operation
#define EDDP_CMD_FDB_CLEAR_UC_TABLE        0x0601   // clear UC-table
#define EDDP_CMD_FDB_CLEAR_UC_ENTRY        0x0602   // clear UC-Entry
#define EDDP_CMD_FDB_INSERT_UC_ENTRY       0x0603   // insert UC-Entry
#define EDDP_CMD_FDB_SEARCH_MAC            0x0604   // Search MAC address
#define EDDP_CMD_FDB_CHANGE_MC_ENTRY       0x0605   // change MC-Entry
#define EDDP_CMD_FDB_CLEAR_UC_PORTS        0x0606   // clear UC-Tx-Ports
#define EDDP_CMD_FDB_PTCP_CHANGE           0x0607   // PTCP change
#define EDDP_CMD_FDB_COUNT				   8		// total count of FDB commands

// VLAN
#define EDDP_CMD_VLAN_NOP                  0x0700   // no operation
#define EDDP_CMD_VLAN_CLEAR                0x0701   // clear VLAN entry
#define EDDP_CMD_VLAN_INSERT               0x0702   // insert VLAN entry
#define EDDP_CMD_VLAN_SEARCH               0x0703   // search VLAN entry
#define EDDP_CMD_VLAN_COUNT				   4		// total count of VLAN commands

// SYNC
#define EDDP_CMD_GSYNC_NOP                 0x0800   // no operation
#define EDDP_CMD_GSYNC_UPDATE_SYNC_A       0x0801   // update SyncA
#define EDDP_CMD_GSYNC_UPDATE_SYNC_B       0x0802   // update SyncB
#define EDDP_CMD_GSYNC_UPDATE_TIME         0x0803   // update Time
#define EDDP_CMD_GSYNC_UPDATE_APPCYCL      0x0804   // update ApplCycle
#define EDDP_CMD_GSYNC_UPDATE_COMCYCL      0x0805   // update ComCYCLE
#define EDDP_CMD_GSYNC_UPDATE_BASECYCL     0x0806   // update BaseCycle
#define EDDP_CMD_GSYNC_UPDATE_SYNCCYCL     0x0807   // update SyncCycle
#define EDDP_CMD_GSYNC_UPDATE_SYNCTIME     0x0808   // update SyncTime
#define EDDP_CMD_GSYNC_STAMP               0x0809   // read out STAMP
#define EDDP_CMD_GSYNC_CONNECT_FREE        0x080A   // connect free
#define EDDP_CMD_GSYNC_CONNECT_SYNC_A      0x080B   // connect SyncA
#define EDDP_CMD_GSYNC_CONNECT_SYNC_B      0x080C   // connect SyncB
#define EDDP_CMD_GSYNC_COUNT			   13		// total count of GSYNC commands
#define EDDP_CMD_GSYNC_STAMP_PARAM_SIZE    4

#define EDDP_CMD_TOTAL_COUNT_OF_COMMANDS    EDDP_CMD_FILTER_COUNT\
                                          + EDDP_CMD_ARCV_COUNT\
                                          + EDDP_CMD_ASND_COUNT\
                                          + EDDP_CMD_CPM_COUNT\
                                          + EDDP_CMD_PPM_COUNT\
                                          + EDDP_CMD_PACK_COUNT\
                                          + EDDP_CMD_FDB_COUNT\
                                          + EDDP_CMD_VLAN_COUNT\
                                          + EDDP_CMD_GSYNC_COUNT
    
#define EDDP_CMD_TOTAL_COUNT_OF_NON_CRT_COMMANDS    EDDP_CMD_FILTER_COUNT\
                                                  + EDDP_CMD_ARCV_COUNT\
                                                  + EDDP_CMD_ASND_COUNT\
                                                  + EDDP_CMD_PACK_COUNT\
                                                  + EDDP_CMD_FDB_COUNT\
                                                  + EDDP_CMD_VLAN_COUNT\
                                                  + EDDP_CMD_GSYNC_COUNT





/*---------------------------------------------------------------------------*/
/* Management structure of CMD stored in DDB                                 */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_QUEUE_INFO_TYPE    EDDP_LOCAL_MEM_ATTR *   EDDP_CMD_QUEUE_INFO_PTR_TYPE;
typedef struct _EDDP_CMD_QUEUE_INFO_TYPE
{
    LSA_UINT32*             pCurrentConfPtr;
    LSA_UINT32*             pNewReqPtr;
    LSA_UINT32*             pQueueStartAddress;
    LSA_UINT32              RunningCommand;
    LSA_UINT32              QueueLength;
    EDDP_QUEUE_TYPE         Queue_pParam;
    EDDP_QUEUE_TYPE         QueueConf_pParam;      /* HERA only - PPM/CPM commands only: asynchronous */
    LSA_UINT32              CmdRunningCount;       /* all       - all commands         : asynchronous */
    LSA_UINT32              CmdRunSeqCount;        /* HERA only - PPM/CPM commands only: asynchronous */
    LSA_UINT32              CmdReadyToBeAcknCount; /* HERA only - PPM/CPM commands only: asynchronous */
} EDDP_CMD_QUEUE_INFO_TYPE;





/*===========================================================================*/
/* Command-Control RAM structure definition for HAL-module                   */
/*===========================================================================*/

typedef struct _EDDP_CMD_RAM_QUEUE_MGMT_TYPE    EDDP_LOCAL_MEM_ATTR *   EDDP_CMD_RAM_QUEUE_MGMT_PTR_TYPE;
typedef struct _EDDP_CMD_RAM_QUEUE_MGMT_TYPE
{
    LSA_UINT64      QueueMGMT[EDDP_CMD_QUEUE_MGMT_SIZE_8BYTE];
} EDDP_CMD_RAM_QUEUE_MGMT_TYPE;

typedef struct _EDDP_CMD_COMMAND_TYPE
{
    LSA_UINT32	    Low_32Bit;
    LSA_UINT32	    High_32Bit;
} EDDP_CMD_COMMAND_TYPE;

typedef struct _EDDP_CMD_RAM_KRISC_TYPE
{
    LSA_UINT8       dummy[EDDP_CMD_KRISC_SIZE_1BYTE];
} EDDP_CMD_RAM_KRISC_TYPE;

/* ------------------------------------------------------------------------------------------------------------------------ */
/* UserCmd-Info-Block                                                                                                       */
/*  -> the struct has fix address (0x68904) in CMD-Ram                                                                      */
/*  -> see HAL index "EDDP_HAL_RAMSUB_CMD_USER_CMD_INFO_BLOCK"                                                              */
/* ------------------------------------------------------------------------------------------------------------------------ */
#define EDDP_CMD_USER_CMD_INFO_BLOCK_SIZE       64
typedef struct _EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_TYPE   EDDP_LOCAL_MEM_ATTR *   EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE;
typedef struct _EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_TYPE
{
    LSA_UINT32          CheckBytes;

    LSA_UINT32          CurrentConfPtrCmdRamAddr;
    LSA_UINT32          NewReqPtrCmdRamAddr;
    LSA_UINT32          QueueStartCmdRamAddr;

    LSA_UINT32          RequestEntryCmdRamAddr;
    LSA_UINT32          CblCmdRamAddr;
    LSA_UINT32          ParamCmdRamAddr;

    LSA_UINT32          TimeGetSequenceID;
    LSA_UINT32          TimeGetStatusSyncState;
    LSA_UINT32          TimeGetEpochUTCOffset;
    LSA_UINT32          TimeGetMACAddressLo;
    LSA_UINT32          TimeGetMACAddressHi;
    LSA_UINT32          TimeGetSequenceIDCopy;

	LSA_UINT32			TimeGetOctLowTimeOld;
	LSA_UINT32			TimeGetOctHighTimeOld;
	LSA_UINT32			TimeGetRctDelayOld;
} EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_TYPE;
/* CheckBytes               : to check the startup of eddp is done                                                          */
#define EDDP_CMD_USER_CMD_INFO_BLOCK_CHECK_BYTES            0XAABBCCDD
/* CurrentConfPtrCmdRamAddr : CurrentConfPtr                -> PNIP-CMD-Ram format                                          */
/* NewReqPtrCmdRamAddr      : NewReqPtr                     -> PNIP-CMD-Ram format                                          */
/* QueueStartCmdRamAddr     : QueueStart                    -> PNIP-CMD-Ram format                                          */
/* RequestEntryCmdRamAddr   : address of RequestEntry       -> PNIP-CMD-Ram format                                          */
/* CblCmdRamAddr            : address of command block      -> PNIP-CMD-Ram format                                          */
/* ParamCmdRamAddr          : address of parameters         -> PNIP-CMD-Ram format                                          */
/* TimeGetSequenceID        : SoE timestamp: SequenceID to keep changes of SyncStates apart                                 */
/* TimeGetStatusSyncState   : SoE timestamp: Status (high word) and PTCP SyncState (low word)                               */
/* TimeGetEpochUTCOffset    : SoE timestamp: SecondsHigh (high word) and PTCP UTCOffset (low word)                          */
/* TimeGetMACAddressLo      : SoE timestamp: MACAddr of the TimeMaster (lower four bytes)                                   */
/* TimeGetMACAddressHi      : SoE timestamp: MACAddr of the TimeMaster (the remaining two bytes)                            */
/* TimeGetSequenceIDCopy    : SoE timestamp: The Copy is not equal to SequenceID during update of TimeGet* data             */
/* TimeGetOctLowTimeOld     : SoE timestamp: Old time before the invalidation by incrementing the SequenceID                */
/* TimeGetOctHighTimeOld    : SoE timestamp: Old time before the invalidation by incrementing the SequenceID                */
/* TimeGetRctDelayOld       : SoE timestamp: Old time from PNIP_REG_RCT_DELAY register                                      */
/* ------------------------------------------------------------------------------------------------------------------------ */

// set TimeGetMACAddressLo/Hi: (11-22-33-44-55-66) --> high (0x11223344) and low (0x55660000)
#define EDDP_TIME_GET_MAC_ADDR_SET(_addr, _p_hi32, _p_lo32)	\
{ \
	*(_p_lo32) = (((LSA_UINT32)(_addr[5])) << 16) | \
                 (((LSA_UINT32)(_addr[4])) << 24);  \
	*(_p_hi32) = (((LSA_UINT32)(_addr[3])))       | \
                 (((LSA_UINT32)(_addr[2])) << 8)  | \
                 (((LSA_UINT32)(_addr[1])) << 16) | \
                 (((LSA_UINT32)(_addr[0])) << 24);  \
}
#define EDDP_TIME_GET_MAC_ADDR_GET(_addr, _hi32, _lo32)	\
{ \
        _addr[0] = (LSA_UINT8)((_hi32 >> 24) & 0xFF); \
        _addr[1] = (LSA_UINT8)((_hi32 >> 16) & 0xFF); \
        _addr[2] = (LSA_UINT8)((_hi32 >> 8)  & 0xFF); \
        _addr[3] = (LSA_UINT8)( _hi32        & 0xFF); \
        _addr[4] = (LSA_UINT8)((_lo32 >> 24) & 0xFF); \
        _addr[5] = (LSA_UINT8)((_lo32 >> 16) & 0xFF); \
}

// IEEE 802 MAC Address formatting for Clock Identity
//   EUI-48: 6 octets
//   EUI-64: 8 octets, the conversation assigns values 255 and 254 to octets 3 and 4 respectively.
// set MasterIdent to MACAddressLo/Hi: (11-22-33-FF FE-44-55-66) --> high (0x11223344) and low (0x55660000)
#define EDDP_MAC_ADDR_EUI64_TO_HI_LO(_addr, _p_hi32, _p_lo32)	\
{ \
	*(_p_lo32) = (((LSA_UINT32)(_addr[7])) << 16) | \
                 (((LSA_UINT32)(_addr[6])) << 24);  \
	*(_p_hi32) = (((LSA_UINT32)(_addr[5])))       | \
                 (((LSA_UINT32)(_addr[2])) << 8)  | \
                 (((LSA_UINT32)(_addr[1])) << 16) | \
                 (((LSA_UINT32)(_addr[0])) << 24);  \
}
// set MACAddressLo/Hi to MasterIdent: high (0x11223344) and low (0x55660000) --> (11-22-33-FF FE-44-55-66)
#define EDDP_MAC_ADDR_HI_LO_TO_EUI64(_addr, _hi32, _lo32)	\
{ \
        _addr[0] = (LSA_UINT8)((_hi32 >> 24) & 0xFF); \
        _addr[1] = (LSA_UINT8)((_hi32 >> 16) & 0xFF); \
        _addr[2] = (LSA_UINT8)((_hi32 >> 8)  & 0xFF); \
        _addr[3] = (LSA_UINT8)(0xFF);                 \
        _addr[4] = (LSA_UINT8)(0xFE);                 \
        _addr[5] = (LSA_UINT8)( _hi32        & 0xFF); \
        _addr[6] = (LSA_UINT8)((_lo32 >> 24) & 0xFF); \
        _addr[7] = (LSA_UINT8)((_lo32 >> 16) & 0xFF); \
}

/* -------------------- REQUEST -------------------- */

typedef struct _EDDP_CMD_RAM_REQUEST_HOST_SYNC_TYPE
{
    LSA_UINT32	    dummy[1];
} EDDP_CMD_RAM_REQUEST_HOST_SYNC_TYPE;

typedef struct _EDDP_CMD_RAM_REQUEST_USER_ASYNC_TYPE
{
    LSA_UINT32	    dummy[EDDP_CMD_FDB_COUNT + EDDP_CMD_GSYNC_COUNT];
} EDDP_CMD_RAM_REQUEST_USER_ASYNC_TYPE;

typedef struct _EDDP_CMD_RAM_REQUEST_USER_SYNC_TYPE
{
    LSA_UINT32	    dummy[1];
} EDDP_CMD_RAM_REQUEST_USER_SYNC_TYPE;

/* -------------------- COMMAND BLOCK -------------------- */

typedef struct _EDDP_CMD_COMMAND_BLOCKS_CRT_HOST_ASYNC_TYPE     EDDP_LOCAL_MEM_ATTR *   EDDP_CMD_COMMAND_BLOCKS_CRT_HOST_ASYNC_PTR_TYPE;
typedef struct _EDDP_CMD_COMMAND_BLOCKS_CRT_HOST_ASYNC_TYPE
{
    EDDP_CMD_COMMAND_TYPE       Crt[EDDP_CMD_MAX_ACW_CNT_PER_CMD];
} EDDP_CMD_COMMAND_BLOCKS_CRT_HOST_ASYNC_TYPE;

typedef struct _EDDP_CMD_COMMAND_BLOCK_GENERIC_TYPE     EDDP_LOCAL_MEM_ATTR *   EDDP_CMD_COMMAND_BLOCK_GENERIC_PTR_TYPE;
typedef struct _EDDP_CMD_COMMAND_BLOCK_GENERIC_TYPE
{
    EDDP_CMD_COMMAND_TYPE       Generic[EDDP_CMD_MAX_ACW_CNT_PER_CMD];
} EDDP_CMD_COMMAND_BLOCK_GENERIC_TYPE;

typedef struct _EDDP_CMD_RAM_COMMAND_BLOCKS_HOST_ASYNC_TYPE     EDDP_LOCAL_MEM_ATTR *   EDDP_CMD_RAM_COMMAND_BLOCKS_HOST_ASYNC_PTR_TYPE;
typedef struct _EDDP_CMD_RAM_COMMAND_BLOCKS_HOST_ASYNC_TYPE
{
    /* API Filter */
    EDDP_CMD_COMMAND_TYPE     Filter_Nop;                                   // no operation
    EDDP_CMD_COMMAND_TYPE     Filter_Stop;                                  // stop filter
    EDDP_CMD_COMMAND_TYPE     Filter_ChangeBase;                            // change filtercodebase
    EDDP_CMD_COMMAND_TYPE     Filter_EnableBase;                            // enable stringdescriptor
    EDDP_CMD_COMMAND_TYPE     Filter_ChangeString[2];                       // change stringdescriptor    
                                                                            
    /* Receive DMACW */                                                     
    EDDP_CMD_COMMAND_TYPE     Arcv_Nop;                                     // operation
    EDDP_CMD_COMMAND_TYPE     Arcv_Disable;                                 // disable receive table
    EDDP_CMD_COMMAND_TYPE     Arcv_DisableDscr;                             // disable receive descriptor
    EDDP_CMD_COMMAND_TYPE     Arcv_CopyDscr[2];                             // copy RcvDMACW
                                                                            
    /* Send DMACW */                                                        
    EDDP_CMD_COMMAND_TYPE     Asnd_Nop;                                     // operation
    EDDP_CMD_COMMAND_TYPE     Asnd_Disable;                                 // disable send table
    EDDP_CMD_COMMAND_TYPE     Asnd_DisableDscr;                             // disable send descriptor    
                                                                            
    /* PACK */                                                              
    EDDP_CMD_COMMAND_TYPE     Pack_Nop;                                     // operation
    EDDP_CMD_COMMAND_TYPE     Pack_DisableGrp;                              // disable packgroup
    EDDP_CMD_COMMAND_TYPE     Pack_EnableGrp;                               // enable packgroup
    EDDP_CMD_COMMAND_TYPE     Pack_DisableEnableGrp;                        // disable/enable packgroup
                                                                            
    /* FDB */                                                               
    EDDP_CMD_COMMAND_TYPE     Fdb_Nop;                                      // no operation
    EDDP_CMD_COMMAND_TYPE     Fdb_ClearUcTable;                             // clear UC-table
    EDDP_CMD_COMMAND_TYPE     Fdb_ClearUcEntry[2];                          // clear UC-Entry
    EDDP_CMD_COMMAND_TYPE     Fdb_InsertUcEntry[2];                         // insert UC-Entry
    EDDP_CMD_COMMAND_TYPE     Fdb_SearchMac[2];                             // search MAC address
    EDDP_CMD_COMMAND_TYPE     Fdb_ChangeMacEntry[2];                        // change MC-Entry
    EDDP_CMD_COMMAND_TYPE     Fdb_ClearUcPorts;                             // clear UC-Tx-Ports
    EDDP_CMD_COMMAND_TYPE     Fdb_PtcpChange;                               // PTCP change
                                                                            
    /* VLAN */                                                              
    EDDP_CMD_COMMAND_TYPE     Vlan_Nop;                                     // no operation
    EDDP_CMD_COMMAND_TYPE     Vlan_Clear;                                   // clear VLAN entry
    EDDP_CMD_COMMAND_TYPE     Vlan_Insert;                                  // insert VLAN entry
    EDDP_CMD_COMMAND_TYPE     Vlan_Search;                                  // search VLAN entry
                                                                            
    /* SYNC */                                                              
    EDDP_CMD_COMMAND_TYPE     Gsync_Nop;                                    // no operation
    EDDP_CMD_COMMAND_TYPE     Gsync_UpdateSyncA;                            // update SyncA
    EDDP_CMD_COMMAND_TYPE     Gsync_UpdateSyncB;                            // update SyncB
    EDDP_CMD_COMMAND_TYPE     Gsync_UpdateTime;                             // update Time
    EDDP_CMD_COMMAND_TYPE     Gsync_UpdateAppCycl[2];                       // update ApplCycle
    EDDP_CMD_COMMAND_TYPE     Gsync_UpdateComCycl[2];                       // update ComCycle
    EDDP_CMD_COMMAND_TYPE     Gsync_UpdateBaseCycl[2];                      // update BaseCycle
    EDDP_CMD_COMMAND_TYPE     Gsync_UpdateSyncCycl;                         // update SyncCycle
    EDDP_CMD_COMMAND_TYPE     Gsync_UpdateSyncTime;                         // update SyncTime
    EDDP_CMD_COMMAND_TYPE     Gsync_Stamp;                                  // read out STAMP
    EDDP_CMD_COMMAND_TYPE     Gsync_ConnectFree;                            // connect free
    EDDP_CMD_COMMAND_TYPE     Gsync_ConnectSyncA[2];                        // connect SyncA
    EDDP_CMD_COMMAND_TYPE     Gsync_ConnectSyncB[2];                        // connect SyncB
} EDDP_CMD_RAM_COMMAND_BLOCKS_HOST_ASYNC_TYPE;

typedef struct _EDDP_CMD_RAM_COMMAND_BLOCK_HOST_SYNC_TYPE
{
    EDDP_CMD_COMMAND_BLOCK_GENERIC_TYPE     Generic;
} EDDP_CMD_RAM_COMMAND_BLOCK_HOST_SYNC_TYPE;

typedef struct _EDDP_CMD_RAM_COMMAND_BLOCKS_USER_ASYNC_TYPE
{
    EDDP_CMD_COMMAND_BLOCK_GENERIC_TYPE     CBL[EDDP_CMD_FDB_COUNT + EDDP_CMD_GSYNC_COUNT];
} EDDP_CMD_RAM_COMMAND_BLOCKS_USER_ASYNC_TYPE;

typedef struct _EDDP_CMD_RAM_COMMAND_BLOCK_USER_SYNC_TYPE
{
    EDDP_CMD_COMMAND_BLOCK_GENERIC_TYPE     Generic;
} EDDP_CMD_RAM_COMMAND_BLOCK_USER_SYNC_TYPE;

/* -------------------- PARAM -------------------- */

typedef struct _EDDP_CMD_CBL_PARAM_TYPE
{
    LSA_UINT32      Low_32Bit;
    LSA_UINT32      High_32Bit;
} EDDP_CMD_CBL_PARAM_TYPE;

typedef struct _EDDP_CMD_PARAM_GENERIC_TYPE     EDDP_LOCAL_MEM_ATTR *       EDDP_CMD_PARAM_GENERIC_PTR_TYPE;
typedef struct _EDDP_CMD_PARAM_GENERIC_TYPE
{
    EDDP_CMD_CBL_PARAM_TYPE     Generic[3];
} EDDP_CMD_PARAM_GENERIC_TYPE;

typedef struct _EDDP_CMD_RAM_PARAM_HOST_ASYNC_TYPE  EDDP_LOCAL_MEM_ATTR *  EDDP_CMD_RAM_PARAM_HOST_ASYNC_PTR_TYPE;
typedef struct _EDDP_CMD_RAM_PARAM_HOST_ASYNC_TYPE
{
    EDDP_CMD_CBL_PARAM_TYPE     Fdb_InsertUcEntry;
    EDDP_CMD_CBL_PARAM_TYPE     Gsync_UpdateComCycl;
    EDDP_CMD_CBL_PARAM_TYPE     Gsync_UpdateAppCycl;
    EDDP_CMD_CBL_PARAM_TYPE     Gsync_UpdateBaseCycl;
    EDDP_CMD_CBL_PARAM_TYPE     Stamp_Param[EDDP_CMD_GSYNC_STAMP_PARAM_SIZE];
} EDDP_CMD_RAM_PARAM_HOST_ASYNC_TYPE;

typedef struct _EDDP_CMD_RAM_PARAM_HOST_SYNC_TYPE
{
    EDDP_CMD_CBL_PARAM_TYPE      Param[EDDP_CMD_MAX_PARAMS_FOR_SYNC_QUEUE];
} EDDP_CMD_RAM_PARAM_HOST_SYNC_TYPE;

typedef struct _EDDP_CMD_RAM_PARAM_USER_ASYNC_TYPE
{
    EDDP_CMD_PARAM_GENERIC_TYPE     Param[EDDP_CMD_FDB_COUNT + EDDP_CMD_GSYNC_COUNT];
    EDDP_CMD_CBL_PARAM_TYPE         Stamp_Param[EDDP_CMD_GSYNC_STAMP_PARAM_SIZE];
} EDDP_CMD_RAM_PARAM_USER_ASYNC_TYPE;

typedef struct _EDDP_CMD_RAM_PARAM_USER_SYNC_TYPE
{
    LSA_UINT64      dummy[EDDP_CMD_MAX_PARAMS_FOR_SYNC_QUEUE];
} EDDP_CMD_RAM_PARAM_USER_SYNC_TYPE;





/*===========================================================================*/
/* 0. Filter commands                                                        */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_FILTER_NOP                                                       */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_FILTER_NOP_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_FILTER_NOP_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_FILTER_STOP                                                      */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_FILTER_STOP_TYPE
{
    LSA_UINT16      StopFilterMask;
} EDDP_CMD_PARAM_FILTER_STOP_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_FILTER_CHANGE_BASE                                               */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_FILTER_CHANGE_BASE_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_FILTER_CHANGE_BASE_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_FILTER_ENABLE_STRING                                             */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_FILTER_ENABLE_STRING_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_FILTER_ENABLE_STRING_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_FILTER_CHANGE_STRING                                             */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_FILTER_CHANGE_STRING_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_FILTER_CHANGE_STRING_TYPE;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
typedef union _EDDP_CMD_PARAM_FILTER_TYPE
{
    EDDP_CMD_PARAM_FILTER_NOP_TYPE              Nop;            /* EDDP_CMD_FILTER_NOP              */
    EDDP_CMD_PARAM_FILTER_STOP_TYPE             StopFilter;     /* EDDP_CMD_FILTER_STOP             */
    EDDP_CMD_PARAM_FILTER_CHANGE_BASE_TYPE      ChangeBase;     /* EDDP_CMD_FILTER_CHANGE_BASE      */
    EDDP_CMD_PARAM_FILTER_ENABLE_STRING_TYPE    EnableString;   /* EDDP_CMD_FILTER_ENABLE_STRING    */
    EDDP_CMD_PARAM_FILTER_CHANGE_STRING_TYPE    ChangeString;   /* EDDP_CMD_FILTER_CHANGE_STRING    */
} EDDP_CMD_PARAM_FILTER_TYPE;





/*===========================================================================*/
/* 1. ARCV commands                                                          */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_ARCV_NOP                                                         */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_ARCV_NOP_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_ARCV_NOP_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_ARCV_DISABLE                                                     */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_ARCV_ACW_DISABLE_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_ARCV_ACW_DISABLE_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_ARCV_DISABLE_DSCR                                                */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_ARCV_DISABLE_DSCR_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_ARCV_ACW_DISABLE_DSCR_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_ARCV_COPY_DSCR                                                   */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_ARCV_COPY_DSCR_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_ARCV_ACW_COPY_DSCR_TYPE;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
typedef union _EDDP_CMD_PARAM_ARCV_TYPE
{
    EDDP_CMD_PARAM_ARCV_NOP_TYPE                Nop;            /* EDDP_CMD_ARCV_NOP            */
    EDDP_CMD_PARAM_ARCV_ACW_DISABLE_TYPE        Disable;        /* EDDP_CMD_ARCV_DISABLE        */
    EDDP_CMD_PARAM_ARCV_ACW_DISABLE_DSCR_TYPE   DisableDscr;    /* EDDP_CMD_ARCV_DISABLE_DSCR   */
    EDDP_CMD_PARAM_ARCV_ACW_COPY_DSCR_TYPE      CopyDscr;       /* EDDP_CMD_ARCV_COPY_DSCR      */
} EDDP_CMD_PARAM_ARCV_TYPE;





/*===========================================================================*/
/* 2. ASND commands                                                          */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_ASND_NOP                                                         */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_ASND_NOP_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_ASND_NOP_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_ASND_DISABLE                                                     */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_ASND_ACW_DISABLE_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_ASND_ACW_DISABLE_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_ASND_DISABLE_DSCR                                                */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_ASND_DISABLE_DSCR_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_ASND_DISABLE_DSCR_TYPE;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
typedef union _EDDP_CMD_PARAM_ASND_TYPE
{
    EDDP_CMD_PARAM_ASND_NOP_TYPE            Nop;            /* EDDP_CMD_ASND_NOP            */
    EDDP_CMD_PARAM_ASND_ACW_DISABLE_TYPE    Disable;        /* EDDP_CMD_ASND_DISABLE        */
    EDDP_CMD_PARAM_ASND_DISABLE_DSCR_TYPE   DisableDscr;    /* EDDP_CMD_ASND_DISABLE_DSCR   */
} EDDP_CMD_PARAM_ASND_TYPE;





/*===========================================================================*/
/* 3. CPM commands                                                           */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_CPM_NOP                                                          */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_CPM_NOP_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_CPM_NOP_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_CPM_ON_OFF                                                       */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_CPM_ON_OFF_TYPE
{
    LSA_BOOL        CpmOn;
} EDDP_CMD_PARAM_CPM_ON_OFF_TYPE;
/* ------------------------------------------------------------------------------------------------------------------------ */
/* CpmOn            : LSA_TRUE  = CPM ON                                                                                    */
/*                    LSA_FALSE = CPM OFF                                                                                   */
/* ------------------------------------------------------------------------------------------------------------------------ */

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_CPM_ACW_ON_OFF                                                   */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_CPM_ACW_ON_OFF_TYPE
{
    LSA_UINT16      AcwID;
    LSA_BOOL        Acw_On;
    LSA_BOOL        Acw_DG;
    LSA_UINT32      ApiAdr_Acw;
} EDDP_CMD_PARAM_CPM_ACW_ON_OFF_TYPE;
/* ------------------------------------------------------------------------------------------------------------------------ */
/* AcwId            : only for CRT: to remember the consumer/container to which this data belong                            */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* Acw_On           : LSA_TRUE  = Acw_On                                                                                    */
/*                    LSA_FALSE = Acw_Off                                                                                   */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* Acw_DG           : LSA_TRUE  = ACW_DG                                                                                    */
/*                    LSA_FALSE = ACW                                                                                       */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* Set_To_Unknown   : LSA_TRUE  = execute Set-to-unknown on AGAIN-Bit                                                       */
/*                    LSA_FALSE = no modify                                                                                 */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* ApiAdr_Acw       : APICtrl-RAM-address of ACW    -> offset to PNIP_RAM_APICTRL_START (64Bit aligned)                     */
/* ------------------------------------------------------------------------------------------------------------------------ */

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_PARAM_CPM_ACW_IN_OUT_WORD0_TYPE                                  */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_CPM_ACW_IN_OUT_WORD0_TYPE
{
    LSA_BOOL        Acw_In;
    LSA_BOOL        Acw_DG;
    LSA_UINT32      ApiAdr_NextPointer_Prev;
} EDDP_CMD_PARAM_CPM_ACW_IN_OUT_WORD0_TYPE;
/* ------------------------------------------------------------------------------------------------------------------------ */
/* Acw_In                   : LSA_TRUE  = Acw_In                                                                            */
/*                            LSA_FALSE = Acw_Out                                                                           */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* Acw_DG                   : LSA_TRUE  = ACW_DG                                                                            */
/*                            LSA_FALSE = ACW                                                                               */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* ApiAdr_NextPointer_Prev  : APICtrl-RAM-address of "ACW.ACW_nextoffset" of prev. ACW, if new ACW in the middle of list    */
/*                            APICtrl-RAM-address of "ACW_List_Ptr", if new ACW the first ACW of list                       */
/*                              -> offset to PNIP_RAM_APICTRL_START (16Bit aligned)                                         */
/* ------------------------------------------------------------------------------------------------------------------------ */

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_PARAM_CPM_ACW_IN_OUT_WORD1_TYPE                                  */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_CPM_ACW_IN_OUT_WORD1_TYPE
{
    LSA_UINT32      ApiAdr_Acw_New;
} EDDP_CMD_PARAM_CPM_ACW_IN_OUT_WORD1_TYPE;
/* ------------------------------------------------------------------------------------------------------------------------ */
/* ApiAdr_Acw_New           : APICtrl-RAM-address of new ACW        -> offset to PNIP_RAM_APICTRL_START (64Bit aligned)     */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* reserved                 : -                                                                                             */
/* ------------------------------------------------------------------------------------------------------------------------ */

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_CPM_ACW_IN_OUT                                                   */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_CPM_ACW_IN_OUT_TYPE
{
    LSA_UINT16                                  AcwID;
    EDDP_CMD_PARAM_CPM_ACW_IN_OUT_WORD0_TYPE    Word0;
    EDDP_CMD_PARAM_CPM_ACW_IN_OUT_WORD1_TYPE    Word1;
} EDDP_CMD_PARAM_CPM_ACW_IN_OUT_TYPE;
/* ------------------------------------------------------------------------------------------------------------------------ */
/* AcwID (ConsumerId) : only for CRT: to remember the consumer to which this data belong                                    */
/* ------------------------------------------------------------------------------------------------------------------------ */

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_CPM_ACW_SET_UNKNOWN                                              */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_CPM_ACW_SET_UNKNOWN_TYPE
{
    LSA_UINT16      ConsumerId;
    LSA_BOOL        Acw_DG;
    LSA_UINT32      ApiAdr_Acw;
} EDDP_CMD_PARAM_CPM_ACW_SET_UNKNOWN_TYPE;
/* ------------------------------------------------------------------------------------------------------------------------ */
/* ConsumerId       : only for CRT: to remember the consumer to which this data belong                                      */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* Acw_DG           : LSA_TRUE  = ACW_DG                                                                                    */
/*                    LSA_FALSE = ACW                                                                                       */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* ApiAdr_Acw       : APICtrl-RAM-address of ACW    -> offset to PNIP_RAM_APICTRL_START (64Bit aligned)                     */
/* ------------------------------------------------------------------------------------------------------------------------ */

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_CPM_ACW_XCHANGE_BUFF                                             */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_CPM_ACW_XCHANGE_BUFF_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_CPM_ACW_XCHANGE_BUFF_TYPE;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
typedef union _EDDP_CMD_PARAM_CPM_TYPE
{
    EDDP_CMD_PARAM_CPM_NOP_TYPE                 Nop;            /* EDDP_CMD_CPM_NOP                 */
    EDDP_CMD_PARAM_CPM_ON_OFF_TYPE              OnOff;          /* EDDP_CMD_CPM_ON_OFF              */
    EDDP_CMD_PARAM_CPM_ACW_ON_OFF_TYPE          ACW_OnOff;      /* EDDP_CMD_CPM_ACW_ON_OFF          */
    EDDP_CMD_PARAM_CPM_ACW_IN_OUT_TYPE          ACW_InOut;      /* EDDP_CMD_CPM_ACW_IN_OUT          */
    EDDP_CMD_PARAM_CPM_ACW_SET_UNKNOWN_TYPE     SetUnknown;     /* EDDP_CMD_CPM_ACW_SET_UNKNOWN     */
    EDDP_CMD_PARAM_CPM_ACW_XCHANGE_BUFF_TYPE    XchangeBuff;    /* EDDP_CMD_CPM_ACW_XCHANGE_BUFF    */
} EDDP_CMD_PARAM_CPM_TYPE;





/*===========================================================================*/
/* 4. PPM commands                                                           */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_PPM_NOP                                                          */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_PPM_NOP_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_PPM_NOP_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_PPM_ON_OFF                                                       */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_PPM_ON_OFF_TYPE
{
    LSA_BOOL        PpmOn;
} EDDP_CMD_PARAM_PPM_ON_OFF_TYPE;
/* ------------------------------------------------------------------------------------------------------------------------ */
/* PpmOn            : LSA_TRUE  = PPM ON                                                                                    */
/*                    LSA_FALSE = PPM OFF                                                                                   */
/* ------------------------------------------------------------------------------------------------------------------------ */

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_PPM_ACW_ON_OFF                                                   */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_PPM_ACW_ON_OFF_TYPE
{
    LSA_UINT16      ProviderId;
    LSA_UINT16      AcwId;
    LSA_BOOL        Acw_On;
    LSA_BOOL        Acw_DG;
    LSA_BOOL        FirstSend;
    LSA_UINT32      ApiAdr_Acw[EDDP_CMD_MAX_ACW_CNT_PER_CMD];
    LSA_UINT32      Acw_Count;
} EDDP_CMD_PARAM_PPM_ACW_ON_OFF_TYPE;
/* ------------------------------------------------------------------------------------------------------------------------ */
/* ProviderId       : only for CRT: to remember the provider to which this data belong                                      */
/* AcwId            : only for CRT: AcwId                   : only one ACW is passivated/activated                          */
/*                                                          : AcwDGId, if Acw_DG(TRUE)                                      */
/*                                  EDDP_CRT_CHAIN_IDX_END  : all ACWs of this provider are passivated/activated            */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* Acw_On           : LSA_TRUE  = Acw_On                                                                                    */
/*                    LSA_FALSE = Acw_Off                                                                                   */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* Acw_DG           : LSA_TRUE  = ACW_DG (AcwDGId is stored in AcwId)                                                       */
/*                    LSA_FALSE = ACW                                                                                       */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* FirstSend        : FirstSend-Bit see also ACW.FirstSend                                                                  */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* ApiAdr_Acw       : APICtrl-RAM-address of ACW (ACW_DG)   -> offset to PNIP_RAM_APICTRL_START (64Bit aligned)             */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* Acw_Count        : Number of ACWs                                                                                        */
/* ------------------------------------------------------------------------------------------------------------------------ */

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_PPM_ACW_IN_OUT                                                   */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_PPM_ACW_IN_OUT_TYPE
{
    LSA_UINT16      ProviderId;
    LSA_UINT16      AcwId;
    LSA_BOOL        Acw_In;
    LSA_BOOL        Acw_DG;
    LSA_UINT32      ApiAdr_Acw_New[EDDP_CMD_MAX_ACW_CNT_PER_CMD];
    LSA_UINT32      ApiAdr_Acw_DG_Next[EDDP_CMD_MAX_ACW_CNT_PER_CMD];
    LSA_UINT32      ApiAdr_NextPointer_Prev[EDDP_CMD_MAX_ACW_CNT_PER_CMD];
    LSA_UINT32      Acw_Count;
} EDDP_CMD_PARAM_PPM_ACW_IN_OUT_TYPE;
/* ------------------------------------------------------------------------------------------------------------------------ */
/* ProviderId               : only for CRT: to remember the provider to which this data belong                              */
/* AcwId                    : only for CRT: AcwId                   : only one ACW is added/removed                         */
/*                                          EDDP_CRT_CHAIN_IDX_END  : all ACWs of this provider are added/removed           */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* Acw_In                   : LSA_TRUE  = Acw_In                                                                            */
/*                            LSA_FALSE = Acw_Out                                                                           */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* Acw_DG                   : LSA_TRUE  = ACW_DG                                                                            */
/*                            LSA_FALSE = ACW                                                                               */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* ApiAdr_Acw_New           : APICtrl-RAM-address of new ACW        -> offset to PNIP_RAM_APICTRL_START (64Bit aligned)     */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* ApiAdr_Acw_DG_Next       : APICtrl-RAM-address of next ACW_DG    -> offset to PNIP_RAM_APICTRL_START (64Bit aligned)     */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* ApiAdr_NextPointer_Prev  : APICtrl-RAM-address of "ACW.ACW_nextoffset" of prev. ACW, if new ACW in the middle of list    */
/*                            APICtrl-RAM-address of "ACW_X_Ptr", if new ACW the first ACW of list                          */
/*                              -> offset to PNIP_RAM_APICTRL_START (16Bit aligned)                                         */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* Acw_Count                : Number of ACWs                                                                                */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* withDelayConf            : true  : command confirmation is delayed                                                       */
/*                            false : command confirmation immediately                                                      */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* isBlockCmd               : true  : following command word contains another command                                       */
/*                            false : this is the last command word                                                         */
/* ------------------------------------------------------------------------------------------------------------------------ */

#ifdef EDDP_CFG_HW_HERA_SUPPORT
/*---------------------------------------------------------------------------*/
/* EDDP_CMD_PPM_ACW_DELAY                                                    */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_PPM_ACW_DELAY_TYPE
{
    LSA_BOOL        Acw_In;
    LSA_UINT32      ApiAdr_Acw_New;
    LSA_UINT32      ApiAdr_NextPointer_Prev;
    LSA_BOOL        withDelayConf;
    LSA_BOOL        isBlockCmd;
} EDDP_CMD_PARAM_PPM_ACW_DELAY_TYPE;
/* ------------------------------------------------------------------------------------------------------------------------ */
/* Acw_In                   : LSA_TRUE  = Acw_In                                                                            */
/*                            LSA_FALSE = Acw_Out                                                                           */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* ApiAdr_Acw_New           : APICtrl-RAM-address of new ACW        -> offset to PNIP_RAM_APICTRL_START (64Bit aligned)     */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* ApiAdr_NextPointer_Prev  : APICtrl-RAM-address of "ACW.ACW_nextoffset" of prev. ACW, if new ACW in the middle of list    */
/*                            APICtrl-RAM-address of "ACW_X_Ptr", if new ACW the first ACW of list                          */
/*                              -> offset to PNIP_RAM_APICTRL_START (16Bit aligned)                                         */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* withDelayConf            : true  : command confirmation is delayed                                                       */
/*                            false : command confirmation immediately                                                      */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* isBlockCmd               : true  : following command word contains another command                                       */
/*                            false : this is the last command word                                                         */
/* ------------------------------------------------------------------------------------------------------------------------ */
#endif

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_PPM_ACW_MOVE                                                     */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_PPM_ACW_MOVE_TYPE
{
    LSA_UINT16      ProviderId;
    LSA_UINT16      CyclePhase;
    LSA_UINT32      CyclePhaseSequence;
    LSA_UINT32      ApiAdr_NextPointer_New_Prev;
    LSA_UINT32      ApiAdr_NextPointer_Old_Prev;
    LSA_UINT32      ApiAdr_Acw;
} EDDP_CMD_PARAM_PPM_ACW_MOVE_TYPE;
/* -------------------------------------------------------------------------------------------------------------------------------- */
/* ProviderId                   : only for CRT: to remember the provider to which this data belong                                  */
/* CyclePhase                   : only for CRT                                                                                      */
/* CyclePhaseSequence           : only for CRT                                                                                      */
/* -------------------------------------------------------------------------------------------------------------------------------- */
/* ApiAdr_NextPointer_New_Prev  : APICtrl-RAM-address of "ACW.ACW_nextoffset" of new prev. ACW, if new ACW in the middle of list    */
/*                                APICtrl-RAM-address of "ACW_X_Ptr", if new ACW the first ACW of list                              */
/*                                  -> offset to PNIP_RAM_APICTRL_START (16Bit aligned)                                             */
/* -------------------------------------------------------------------------------------------------------------------------------- */
/* ApiAdr_NextPointer_Old_Prev  : APICtrl-RAM-address of "ACW.ACW_nextoffset" of old prev. ACW, if old ACW in the middle of list    */
/*                                APICtrl-RAM-address of "ACW_X_Ptr", if old ACW the first ACW of list                              */
/*                                  -> offset to PNIP_RAM_APICTRL_START (16Bit aligned)                                             */
/* -------------------------------------------------------------------------------------------------------------------------------- */
/* ApiAdr_Acw                   : APICtrl-RAM-address of ACW    -> offset to PNIP_RAM_APICTRL_START (64Bit aligned)                 */
/* -------------------------------------------------------------------------------------------------------------------------------- */
/* withDelayConf                : true  : command confirmation is delayed                                                           */
/*                                false : command confirmation immediately                                                          */
/* -------------------------------------------------------------------------------------------------------------------------------- */
/* isBlockCmd                   : true  : following command word contains another command                                           */
/*                                false : this is the last command word                                                             */
/* -------------------------------------------------------------------------------------------------------------------------------- */

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_PPM_ACW_XCHANGE_BUFF                                             */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_PPM_ACW_XCHANGE_BUFF_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_PPM_ACW_XCHANGE_BUFF_TYPE;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
typedef union _EDDP_CMD_PARAM_PPM_TYPE
{
    EDDP_CMD_PARAM_PPM_NOP_TYPE                 Nop;            /* EDDP_CMD_PPM_NOP                 */
    EDDP_CMD_PARAM_PPM_ON_OFF_TYPE              OnOff;          /* EDDP_CMD_PPM_ON_OFF              */
    EDDP_CMD_PARAM_PPM_ACW_ON_OFF_TYPE          ACW_OnOff;      /* EDDP_CMD_PPM_ACW_ON_OFF          */
    EDDP_CMD_PARAM_PPM_ACW_IN_OUT_TYPE          ACW_InOut;      /* EDDP_CMD_PPM_ACW_IN_OUT          */
    EDDP_CMD_PARAM_PPM_ACW_MOVE_TYPE            ACW_Move;       /* EDDP_CMD_PPM_ACW_MOVE            */
    EDDP_CMD_PARAM_PPM_ACW_XCHANGE_BUFF_TYPE    XchangeBuff;    /* EDDP_CMD_PPM_ACW_XCHANGE_BUFF    */
} EDDP_CMD_PARAM_PPM_TYPE;





/*===========================================================================*/
/* 5. PACK commands                                                          */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_PACK_NOP                                                         */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_PACK_NOP_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_PACK_NOP_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_PACK_DISABLE_GRP                                                 */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_PACK_DISABLE_GRP_TYPE
{
    LSA_BOOL        IsInGrp;
    LSA_UINT8       DisPackGrp;
} EDDP_CMD_PARAM_PACK_DISABLE_GRP_TYPE;
/* ------------------------------------------------------------------------------------------------------------------------ */
/* IsInGrp                  : LSA_TRUE  = In                                                                                */
/*                            LSA_FALSE = Out                                                                               */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* DisPackGrp               : Nr of the Pack group                                                                          */
/* ------------------------------------------------------------------------------------------------------------------------ */

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_PACK_ENABLE_GRP                                                  */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_PACK_ENABLE_GRP_TYPE
{
    LSA_BOOL        IsInGrp;
    LSA_UINT8       EnPackGrp;
} EDDP_CMD_PARAM_PACK_ENABLE_GRP_TYPE;
/* ------------------------------------------------------------------------------------------------------------------------ */
/* IsInGrp                  : LSA_TRUE  = In                                                                                */
/*                            LSA_FALSE = Out                                                                               */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* EnPackGrp                : Nr of the Pack group                                                                          */
/* ------------------------------------------------------------------------------------------------------------------------ */

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_PACK_DIS_ENA_GRP                                                 */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_PACK_DIS_ENA_TYPE
{
    LSA_BOOL        IsInGrp;
    LSA_UINT8       DisPackGrp;
    LSA_UINT8       EnPackGrp;
} EDDP_CMD_PARAM_PACK_DIS_ENA_TYPE;
/* ------------------------------------------------------------------------------------------------------------------------ */
/* IsInGrp                  : LSA_TRUE  = In                                                                                */
/*                            LSA_FALSE = Out                                                                               */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* DisPackGrp               : Nr of the Pack group (for disable)                                                            */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* EnPackGrp                : Nr of the Pack group (for enable)                                                             */
/* ------------------------------------------------------------------------------------------------------------------------ */


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
typedef union _EDDP_CMD_PARAM_PACK_TYPE
{
    EDDP_CMD_PARAM_PACK_NOP_TYPE            Nop;            /* EDDP_CMD_PACK_NOP            */
    EDDP_CMD_PARAM_PACK_DISABLE_GRP_TYPE    DisableGrp;     /* EDDP_CMD_PACK_DISABLE_GRP    */
    EDDP_CMD_PARAM_PACK_ENABLE_GRP_TYPE     EnableGrp;      /* EDDP_CMD_PACK_ENABLE_GRP     */
    EDDP_CMD_PARAM_PACK_DIS_ENA_TYPE        DisEnaGrp;      /* EDDP_CMD_PACK_DIS_ENA_GRP    */
} EDDP_CMD_PARAM_PACK_TYPE;





/*===========================================================================*/
/* 6. FDB commands                                                           */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_FDB_NOP                                                          */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_FDB_NOP_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_FDB_NOP_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_FDB_CLEAR_UC_TABLE                                               */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_FDB_CLEAR_UC_TABLE_TYPE
{
    LSA_BOOL        isCmdRunning;
    LSA_BOOL        Delete_Only_Dynamic_Entries;
    LSA_BOOL        Delete_Only_Dynamic_Entries_Port1;
    LSA_BOOL        Delete_Only_Dynamic_Entries_Port2;
    LSA_BOOL        Delete_Only_Dynamic_Entries_Port3;
    LSA_BOOL        Delete_Only_Dynamic_Entries_Port4;
    LSA_UINT32      UC_Learning_Enable;
    LSA_UINT32      UC_Learning_Enable_R;
    LSA_UINT32      MC_Learning_Enable;
    LSA_UINT32      MC_Learning_Enable_R;
    LSA_UINT32      IRT_Learning_Enable;
} EDDP_CMD_PARAM_FDB_CLEAR_UC_TABLE_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_FDB_CLEAR_UC_ENTRY                                               */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_FDB_CLEAR_UC_ENTRY_TYPE
{
    LSA_UINT64      UC_MAC_Address;
    LSA_UINT32      Result;
} EDDP_CMD_PARAM_FDB_CLEAR_UC_ENTRY_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_FDB_INSERT_UC_ENTRY                                              */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_FDB_INSERT_UC_ENTRY_TYPE
{
    LSA_UINT8           TxPort;
    LSA_BOOL	        Static;
    LSA_BOOL	        Aging;
    LSA_UINT8	        RxPort;
    LSA_BOOL	        Learning_Disable;
    LSA_BOOL	        Mgmt_Enable;
    LSA_BOOL	        Mgmt_Prio;    
    EDD_MAC_ADR_TYPE    UC_MAC_Address;  
    LSA_UINT32	        Result;
} EDDP_CMD_PARAM_FDB_INSERT_UC_ENTRY_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_FDB_SEARCH_MAC                                                   */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_FDB_SEARCH_MAC_TYPE
{
    LSA_UINT64      MAC_Address;
    LSA_UINT32      Result;
} EDDP_CMD_PARAM_FDB_SEARCH_MAC_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_FDB_CHANGE_MC_ENTRY                                              */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_FDB_CHANGE_MC_ENTRY_TYPE
{
    LSA_UINT16      HighMAC;
    LSA_UINT16      LowMAC;
    LSA_UINT32	    MAC;
    LSA_UINT8	    Valid;
    LSA_UINT8	    FrameHandling;
    LSA_UINT8	    Redundancy;
    LSA_UINT8	    Fragmentation;
    LSA_UINT8	    BridgeDelay_Enable; /* only used in PN-IP Rev1 */
    LSA_UINT8	    PriorityMapping;
    LSA_UINT8	    TxPort_API;
    LSA_UINT8	    LD_API;
    LSA_UINT8	    MgmtENable_API;
    LSA_UINT8	    MgmtPrio_API;
    LSA_UINT8	    TxPort_1;
    LSA_UINT8	    LD_1;
    LSA_UINT8	    MgmtENable_1;
    LSA_UINT8	    MgmtPrio_1;
    LSA_UINT8	    TxPort_2;
    LSA_UINT8	    LD_2;
    LSA_UINT8	    MgmtENable_2;
    LSA_UINT8	    MgmtPrio_2;
    LSA_UINT8	    TxPort_3;
    LSA_UINT8	    LD_3;
    LSA_UINT8	    MgmtENable_3;
    LSA_UINT8	    MgmtPrio_3;
    LSA_UINT8	    TxPort_4;
    LSA_UINT8	    LD_4;
    LSA_UINT8	    MgmtENable_4;
    LSA_UINT8	    MgmtPrio_4;
    LSA_UINT8	    UseType;
    LSA_UINT8	    DefaultTrafficClass;
    LSA_UINT8	    APIType;
    LSA_UINT8	    ResourceType;
    LSA_UINT8	    FastForwarding;

    LSA_UINT32	    Result;
} EDDP_CMD_PARAM_FDB_CHANGE_MC_ENTRY_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_FDB_CLEAR_UC_PORTS                                               */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_FDB_CLEAR_UC_PORTS_TYPE
{
    LSA_BOOL        ClearAPI;
    LSA_BOOL        ClearPort1;
    LSA_BOOL	    ClearPort2;
    LSA_BOOL	    ClearPort3;
    LSA_BOOL	    ClearPort4;
} EDDP_CMD_PARAM_FDB_CLEAR_UC_PORTS_TYPE;


/*---------------------------------------------------------------------------*/
/* EDDP_CMD_FDB_PTCP_CHANGE                                                  */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_FDB_PTCP_CHANGE_TYPE
{
    LSA_BOOL        PTCP_Clock_IFA;
    LSA_BOOL	    PTCP_Time;
    LSA_BOOL	    PTCP_INIT;
} EDDP_CMD_PARAM_FDB_PTCP_CHANGE_TYPE;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
typedef union _EDDP_CMD_PARAM_FDB_TYPE
{
    EDDP_CMD_PARAM_FDB_NOP_TYPE                 Nop;                /* EDDP_CMD_FDB_NOP                 */
    EDDP_CMD_PARAM_FDB_CLEAR_UC_TABLE_TYPE      ClearUCTable;       /* EDDP_CMD_FDB_CLEAR_UC_TABLE      */
    EDDP_CMD_PARAM_FDB_CLEAR_UC_ENTRY_TYPE      ClearUCEntry;       /* EDDP_CMD_FDB_CLEAR_UC_ENTRY      */
    EDDP_CMD_PARAM_FDB_INSERT_UC_ENTRY_TYPE     InsertUCEntry;      /* EDDP_CMD_FDB_INSERT_UC_ENTRY     */
    EDDP_CMD_PARAM_FDB_SEARCH_MAC_TYPE          SearchMAC;          /* EDDP_CMD_FDB_SEARCH_MAC          */
    EDDP_CMD_PARAM_FDB_CHANGE_MC_ENTRY_TYPE     ChangeMCEntry;      /* EDDP_CMD_FDB_CHANGE_MC_ENTRY     */
    EDDP_CMD_PARAM_FDB_CLEAR_UC_PORTS_TYPE      ClearUCPorts;       /* EDDP_CMD_FDB_CLEAR_UC_PORTS      */
    EDDP_CMD_PARAM_FDB_PTCP_CHANGE_TYPE         PTCPChange;         /* EDDP_CMD_FDB_PTCP_CHANGE         */
} EDDP_CMD_PARAM_FDB_TYPE;





/*===========================================================================*/
/* 7. VLAN commands                                                          */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_VLAN_NOP                                                         */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_VLAN_NOP_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_VLAN_NOP_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_VLAN_CLEAR                                                       */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_VLAN_CLEAR_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_VLAN_CLEAR_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_VLAN_INSERT                                                      */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_VLAN_INSERT_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_VLAN_INSERT_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_VLAN_SEARCH                                                      */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_VLAN_SEARCH_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_VLAN_SEARCH_TYPE;


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
typedef union _EDDP_CMD_PARAM_VLAN_TYPE
{
    EDDP_CMD_PARAM_VLAN_NOP_TYPE        Nop;        /* EDDP_CMD_VLAN_NOP        */
    EDDP_CMD_PARAM_VLAN_CLEAR_TYPE      Clear;      /* EDDP_CMD_VLAN_CLEAR      */
    EDDP_CMD_PARAM_VLAN_INSERT_TYPE     Insert;     /* EDDP_CMD_VLAN_INSERT     */
    EDDP_CMD_PARAM_VLAN_SEARCH_TYPE     Search;     /* EDDP_CMD_VLAN_SEARCH     */
} EDDP_CMD_PARAM_VLAN_TYPE;





/*===========================================================================*/
/* 8. GSYNC commands                                                         */
/*===========================================================================*/
typedef LSA_VOID LSA_FCT_PTR(EDD_UPPER_OUT_FCT_ATTR, EDDP_CMD_GSYNC_CBF_PTR_TYPE)(EDDP_LOCAL_DDB_PTR_TYPE, EDD_RSP); 

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_GSYNC_NOP                                                        */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_GSYNC_NOP_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_GSYNC_NOP_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_GSYNC_UPDATE_SYNC_A                                              */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_GSYNC_UPDATE_SYNC_A_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_GSYNC_UPDATE_SYNC_A_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_GSYNC_UPDATE_SYNC_B                                              */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_GSYNC_UPDATE_SYNC_B_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_GSYNC_UPDATE_SYNC_B_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_GSYNC_UPDATE_TIME                                                */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_GSYNC_UPDATE_TIME_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_GSYNC_UPDATE_TIME_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_GSYNC_UPDATE_APPCYCL                                             */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_GSYNC_UPDATE_APPCYCL_TYPE
{
    LSA_BOOL                        isApplTimerOn;
    LSA_UINT8                       ApplTimerNr;
    LSA_UINT32                      ApplicationLengthNs;
    EDDP_CMD_GSYNC_CBF_PTR_TYPE     SmCbf;
} EDDP_CMD_PARAM_GSYNC_UPDATE_APPCYCL_TYPE;
/* isApplTimerOn            : ApplTimer is ON/OFF                                       */
/* ApplTimerNr              : number(1,2,3) of ApplTimer                                */
/* ApplicationLengthNs      : cycle length (SendClock) in ns                            */
/* SmCbf                    : state machine call back function to call after command    */

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_GSYNC_UPDATE_COMCYCL                                             */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_GSYNC_UPDATE_COMCYCL_TYPE
{
    LSA_UINT32      CommunicationLength_1ns;
} EDDP_CMD_PARAM_GSYNC_UPDATE_COMCYCL_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_GSYNC_UPDATE_BASECYCL                                            */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_GSYNC_UPDATE_BASECYCL_TYPE
{
    LSA_UINT16      PPM_Starttime_Ns;
} EDDP_CMD_PARAM_GSYNC_UPDATE_BASECYCL_TYPE;
/* PPM_Starttime_Ns     : PPM_Starttime in ns                                           */

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_GSYNC_UPDATE_SYNCCYCL                                            */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_GSYNC_UPDATE_SYNCCYCL_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_GSYNC_UPDATE_SYNCCYCL_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_GSYNC_UPDATE_SYNCTIME                                            */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_GSYNC_UPDATE_SYNCTIME_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_GSYNC_UPDATE_SYNCTIME_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_GSYNC_STAMP                                                      */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_GSYNC_STAMP_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_GSYNC_STAMP_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_GSYNC_CONNECT_FREE                                               */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_GSYNC_CONNECT_FREE_TYPE
{
    LSA_UINT32      Dummy;
} EDDP_CMD_PARAM_GSYNC_CONNECT_FREE_TYPE;

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_GSYNC_CONNECT_SYNC_A                                             */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_GSYNC_CONNECT_SYNC_A_TYPE
{
    LSA_UINT8                       ApplTimerNr;
    EDDP_CMD_GSYNC_CBF_PTR_TYPE     SmCbf;
} EDDP_CMD_PARAM_GSYNC_CONNECT_SYNC_A_TYPE;
/* ApplTimerNr              : number(1,2,3) of ApplTimer                                */
/* SmCbf                    : state machine call back function to call after command    */

/*---------------------------------------------------------------------------*/
/* EDDP_CMD_GSYNC_CONNECT_SYNC_B                                             */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_CMD_PARAM_GSYNC_CONNECT_SYNC_B_TYPE
{
    LSA_UINT8                       ApplTimerNr;
    EDDP_CMD_GSYNC_CBF_PTR_TYPE     SmCbf;
} EDDP_CMD_PARAM_GSYNC_CONNECT_SYNC_B_TYPE;
/* ApplTimerNr              : number(1,2,3) of ApplTimer                                */
/* SmCbf                    : state machine call back function to call after command    */

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
typedef union _EDDP_CMD_PARAM_GSYNC_TYPE
{
    EDDP_CMD_PARAM_GSYNC_NOP_TYPE               Nop;                /* EDDP_CMD_GSYNC_UPDATE_SYNC_A     */
    EDDP_CMD_PARAM_GSYNC_UPDATE_SYNC_A_TYPE     UpdateSyncA;        /* EDDP_CMD_GSYNC_NOP               */
    EDDP_CMD_PARAM_GSYNC_UPDATE_SYNC_B_TYPE     UpdateSyncB;        /* EDDP_CMD_GSYNC_UPDATE_SYNC_B     */
    EDDP_CMD_PARAM_GSYNC_UPDATE_TIME_TYPE       UpdateTime;         /* EDDP_CMD_GSYNC_UPDATE_TIME       */
    EDDP_CMD_PARAM_GSYNC_UPDATE_APPCYCL_TYPE    UpdateAppCycl;      /* EDDP_CMD_GSYNC_UPDATE_APPCYCL    */
    EDDP_CMD_PARAM_GSYNC_UPDATE_COMCYCL_TYPE    UpdateComCycl;      /* EDDP_CMD_GSYNC_UPDATE_COMCYCL    */
    EDDP_CMD_PARAM_GSYNC_UPDATE_BASECYCL_TYPE   UpdateBaseCycl;     /* EDDP_CMD_GSYNC_UPDATE_BASECYCL   */
    EDDP_CMD_PARAM_GSYNC_UPDATE_SYNCCYCL_TYPE   UpdateSyncCycl;     /* EDDP_CMD_GSYNC_UPDATE_SYNCCYCL   */
    EDDP_CMD_PARAM_GSYNC_UPDATE_SYNCTIME_TYPE   UpdateSyncTime;     /* EDDP_CMD_GSYNC_UPDATE_SYNCTIME   */
    EDDP_CMD_PARAM_GSYNC_STAMP_TYPE             Stamp;              /* EDDP_CMD_GSYNC_STAMP             */
    EDDP_CMD_PARAM_GSYNC_CONNECT_FREE_TYPE      ConnectFree;        /* EDDP_CMD_GSYNC_CONNECT_FREE      */
    EDDP_CMD_PARAM_GSYNC_CONNECT_SYNC_A_TYPE    ConnectSyncA;       /* EDDP_CMD_GSYNC_CONNECT_SYNC_A    */
    EDDP_CMD_PARAM_GSYNC_CONNECT_SYNC_B_TYPE    ConnectSyncB;       /* EDDP_CMD_GSYNC_CONNECT_SYNC_B    */
} EDDP_CMD_PARAM_GSYNC_TYPE;





/*===========================================================================*/
/* Command parameter block                                                   */
/*===========================================================================*/

typedef struct _EDDP_CMD_PARAM_CRT_TYPE
{
    LSA_UINT32*                         pExternalCmdParam_ApiCtrlRam; /* external CMD param wihtin ApiCtrlRam */
    EDDP_CMD_COMMAND_TYPE*              pCBL;                         /* command blocks */
    EDDP_CMD_PARAM_CPM_TYPE             Cpm;                          /* CPM commands */
    EDDP_CMD_PARAM_PPM_TYPE             Ppm;                          /* PPM commands */
    LSA_BOOL                            InUse;                        /* is internal command parameter in use */
    LSA_UINT16                          PpmAcwDgSmIdx;        /* index of activation sate machine, invalid idx: 0xFFFF */
} EDDP_CMD_PARAM_CRT_TYPE;

// ***** CallbackFct for async command
typedef struct _EDDP_CMD_PARAM_TYPE  EDDP_LOCAL_MEM_ATTR *   EDDP_CMD_PARAM_PTR_TYPE;
typedef LSA_VOID LSA_FCT_PTR(EDD_UPPER_OUT_FCT_ATTR, EDDP_CMD_CBF_PTR_TYPE)(EDDP_LOCAL_DDB_PTR_TYPE, EDDP_CMD_PARAM_PTR_TYPE, EDD_RSP); 
typedef struct _EDDP_CMD_PARAM_TYPE
{
    EDDP_QUEUE_ELEMENT_TYPE             Queue;          /* for queuing                                                  */
    LSA_UINT32                          CmdOpcode;      /* command opcode to specify type of param_union                */
    EDDP_CMD_CBF_PTR_TYPE               Cbf;            /* CallbackFct when using async commands                        */
    //LSA_VOID                             LSA_FCT_PTR(EDD_UPPER_OUT_FCT_ATTR, Cbf)  
    //                                            ( EDDP_LOCAL_DDB_PTR_TYPE       pDDB,
    //                                              EDDP_CMD_PARAM_PTR_TYPE       pParam,
    //                                              EDD_RSP                       Response );
    LSA_BOOL                            InUse;       /* Flag used by caller - controlled by CMD                         */
    EDD_RSP                             Response;    /* Response from command handling                                  */
    EDDP_LOCAL_DDB_PTR_TYPE             pDDB;        /* filled and used by CMD to remember the calling interface        */
    union
    {
        EDDP_CMD_PARAM_FILTER_TYPE          Filter;     /* Filter commands  */
        EDDP_CMD_PARAM_ARCV_TYPE            Arcv;       /* ARCV   commands  */
        EDDP_CMD_PARAM_ASND_TYPE            Asnd;       /* ASND   commands  */
        EDDP_CMD_PARAM_CRT_TYPE             Crt;        /* CRT    commands  */
        EDDP_CMD_PARAM_PACK_TYPE            Pack;       /* PACK   commands  */
        EDDP_CMD_PARAM_FDB_TYPE             Fdb;        /* FDB    commands  */
        EDDP_CMD_PARAM_VLAN_TYPE            Vlan;       /* VLAN   commands  */
        EDDP_CMD_PARAM_GSYNC_TYPE           Gsync;      /* GSYNC  commands  */
    }param_union;
} EDDP_CMD_PARAM_TYPE;





/*===========================================================================*/
/*                               prototypes                                  */
/*===========================================================================*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CmdRequest(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_CMD_PARAM_PTR_TYPE     pParam);
    
LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CMD_Is_Command_Running(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  CmdOpCode);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CMD_ISR_ReqQueue_ERR(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr);
    
/*****************************************************************************/
/*  end of file eddp_cmd_usr.h                                               */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_CMD_USR_H */
