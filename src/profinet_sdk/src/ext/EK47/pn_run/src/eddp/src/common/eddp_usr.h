    
#ifndef EDDP_USR_H                       /* ----- reinclude-protection ----- */
#define EDDP_USR_H

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
/*  F i l e               &F: eddp_usr.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  User Interface for EDDP (Ethernet Device Driver for PNIP)                */
/*  Defines constants, types, macros and prototyping specific for EDDP.      */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  12.08.08    JS    initial version.                                       */
/*  19.04.12    VS    added user API for the access to the PN-IP registers   */
/*                    added user API for command interface                   */
/*                    added user API for system redundancy                   */
/*  02.03.16    HM    extended UserCmd interface with TimeStamp Info         */
/*  05.04.16    SJ    Added HERA I/O Structures                              */
/*                                                                           */
#endif
/*****************************************************************************/


/*===========================================================================*/
/*                      Basic types for EDDP                                 */
/*===========================================================================*/

#define EDDP_LOWER_MEM_PTR_TYPE                 /* pointer to lower-memory */ \
    LSA_VOID                    EDDP_LOWER_MEM_ATTR *

#define EDDP_LOCAL_MEM_PTR_TYPE                 /* pointer to local-memory */ \
    LSA_VOID                    EDDP_LOCAL_MEM_ATTR *

#define EDDP_LOCAL_MEM_U8_PTR_TYPE              /* pointer to local-memory */ \
    LSA_UINT8                   EDDP_LOCAL_MEM_ATTR *

#define EDDP_LOCAL_MEM_U16_PTR_TYPE             /* pointer to local-memory */ \
    LSA_UINT16                  EDDP_LOCAL_MEM_ATTR *

#define EDDP_LOCAL_MEM_U32_PTR_TYPE             /* pointer to local-memory */ \
    LSA_UINT32                  EDDP_LOCAL_MEM_ATTR *


#define EDDP_DEV_MEM_PTR_TYPE                   /* pointer to device-memory */ \
    LSA_VOID                    EDDP_LOWER_MEM_ATTR *

#define EDDP_DEV_MEM_U8_PTR_TYPE                /* pointer to device-memory */ \
    LSA_UINT8                   EDDP_LOWER_MEM_ATTR *

#define EDDP_DEV_MEM_U32_PTR_TYPE               /* pointer to device-memory */ \
    LSA_UINT32                  EDDP_LOWER_MEM_ATTR *

#define EDDP_DEV_MEM_U64_PTR_TYPE               /* pointer to device-memory */ \
    LSA_UINT64                  EDDP_LOWER_MEM_ATTR *


/*===========================================================================*/
/*                          User defines                                     */
/*===========================================================================*/
// for user-cmd-handle
#define EDDP_USER_CMD_HANDLE_TYPE               LSA_UINT32
#define EDDP_USER_CMD_HANDLE_INVALID            0xFFFFFFFF

// for user-sysred-handle
#define EDDP_USER_SYSRED_HANDLE_TYPE            LSA_UINT32
#define EDDP_USER_SYSRED_HANDLE_INVALID         0xFFFFFFFF

// CmdOpcode
#define EDDP_USER_CMD_GSYNC_UPDATE_APPCYCL          0x0804  // Update_APPLCYCLE
#define EDDP_USER_CMD_GSYNC_CONNECT_FREE            0x080A  // Connect_FREE
#define EDDP_USER_CMD_GSYNC_CONNECT_SYNC_A          0x080B  // Connect_SYNC_A
#define EDDP_USER_CMD_GSYNC_TIME_STAMP_GET          0x080C  // TimeStampGet
#define EDDP_USER_CMD_GSYNC_TIME_SYNC_IND_GET       0x080D  // TimeSyncIndGet

// Mode in macro EDDP_SIGNAL_SENDCLOCK_CHANGE
#define EDDP_SENDCLOCK_CHANGE_MODE_STARTING     1       // EDDP will change the SendClock after the execution of this function
#define EDDP_SENDCLOCK_CHANGE_MODE_FINISHED     2       // EDDP has changed the SendClock

// for check in <eddp_pls.h>
#define EDDP_RX_FRAME_BUFFER_LENGTH             1536

/*===========================================================================*/
/*                          User types                                       */
/*===========================================================================*/
// parameters for command Update_APPLCYCLE
typedef struct _EDDP_USER_CMD_PARAM_GSYNC_UPDATE_APPCYCL_TYPE
{
    LSA_UINT8       ApplicationTimer1;
    LSA_UINT8       ApplicationTimer2;
    LSA_UINT8       ApplicationTimer3;
    LSA_UINT32      NewApplicationLength;
} EDDP_USER_CMD_PARAM_GSYNC_UPDATE_APPCYCL_TYPE;
/* ApplicationTimer1        : no Update(       0), Update(     !=0)            */
/* ApplicationTimer2        : no Update(       0), Update(     !=0)            */
/* ApplicationTimer3        : no Update(       0), Update(     !=0)            */
/* NewApplicationLength     : new application cycle length in ns                */

// parameters for command Connect_FREE
typedef struct _EDDP_USER_CMD_PARAM_GSYNC_CONNECT_FREE_TYPE
{
    LSA_UINT8       ApplicationTimer1;
    LSA_UINT8       ApplicationTimer2;
    LSA_UINT8       ApplicationTimer3;
} EDDP_USER_CMD_PARAM_GSYNC_CONNECT_FREE_TYPE;
/* ApplicationTimer1        : no Action(       0), Connect(     !=0)           */
/* ApplicationTimer2        : no Action(       0), Connect(     !=0)           */
/* ApplicationTimer3        : no Action(       0), Connect(     !=0)           */

// parameters for command Connect_SYNC_A
typedef struct _EDDP_USER_CMD_PARAM_GSYNC_CONNECT_SYNC_A_TYPE
{
    LSA_UINT8       ApplicationTimer1;
    LSA_UINT8       ApplicationTimer2;
    LSA_UINT8       ApplicationTimer3;
} EDDP_USER_CMD_PARAM_GSYNC_CONNECT_SYNC_A_TYPE;
/* ApplicationTimer1        : no Action(       0), Connect(     !=0)           */
/* ApplicationTimer2        : no Action(       0), Connect(     !=0)           */
/* ApplicationTimer3        : no Action(       0), Connect(     !=0)           */

/* ------------------------------------------------------------------------------------------------------------ */
/* PACKED(4) STRUCTS - ON                                                                                       */
/* ------------------------------------------------------------------------------------------------------------ */
#include "pnio_pck4_on.h"

// parameters for command TimeStampGet
PNIO_PACKED_ATTRIBUTE_PRE struct _EDDP_PNIO_TIMESTAMP_TYPE
{               
    LSA_UINT16  Status;             // State of TimeStamp: 0=sync 1=local 2=free run
    LSA_UINT16  SecondsHigh;        // PTCP_EpochNumber
    LSA_UINT32  SecondsLow;         // PTCP_Seconds
    LSA_UINT32  Nanoseconds;        // PTCP_NanoSeconds
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _EDDP_PNIO_TIMESTAMP_TYPE    EDDP_PNIO_TIMESTAMP_TYPE;

// values for Status:
#define EDDP_PNIO_STATUS_MASTER_SYNC    0
#define EDDP_PNIO_STATUS_LOCAL_SYNC     1
#define EDDP_PNIO_STATUS_NOT_SYNC       2

// size of the Time Master Identification
#define EDDP_TIME_MASTER_IDENT_SIZE     8
// size of the Time Domain Identification
#define EDDP_TIME_DOMAIN_IDENT_SIZE     16

typedef struct _EDDP_TIMESTAMP_INFO_TYPE EDDP_SYSTEM_MEM_ATTR * EDDP_TIMESTAMP_INFO_PTR_TYPE;
PNIO_PACKED_ATTRIBUTE_PRE struct _EDDP_TIMESTAMP_INFO_TYPE
{
	EDDP_PNIO_TIMESTAMP_TYPE    TimesStamp; // PNIO_TimeStamp
    LSA_UINT16                  UTCOffset;  // current UTC offset
    LSA_UINT16                  SyncState;  // current state of time synchronisation
    LSA_UINT32                  SequenceID; // ID of a specific SyncState
    LSA_UINT8                   MasterIdent [EDDP_TIME_MASTER_IDENT_SIZE];      /* For RS_MasterIdentification  */
    LSA_UINT8                   DomainIdent [EDDP_TIME_DOMAIN_IDENT_SIZE];      /* For RS_DomainIdentification  */
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _EDDP_TIMESTAMP_INFO_TYPE    EDDP_TIMESTAMP_INFO_TYPE;

// Bit values for SyncState:
#define EDDP_PTCP_SYNC_STATE_OK						0
#define EDDP_PTCP_SYNC_STATE_JITTER_OUT_OF_BOUNDARY	1
#define EDDP_PTCP_SYNC_STATE_NO_MESSAGE_RECEIVED	2
#define EDDP_PTCP_SYNC_STATE_MASK					3
#define EDDP_PTCP_SYNC_STATE_OFF					0xffff

typedef struct _EDDP_USER_CMD_PARAM_GSYNC_TIME_STAMP_GET_TYPE
{
    EDDP_TIMESTAMP_INFO_PTR_TYPE pTimestamp;    // pointer to write back timestamp values
    EDDP_TIMESTAMP_INFO_PTR_TYPE pOldTimestamp; // pointer to write back timestamp values
    EDDP_TIMESTAMP_INFO_PTR_TYPE pNewTimestamp; // pointer to write back timestamp values
} EDDP_USER_CMD_PARAM_GSYNC_TIME_STAMP_GET_TYPE;

typedef struct _EDDP_USER_CMD_PARAM_GSYNC_TIME_DOMAIN_IDENT_TYPE
{
    LSA_UINT8         * pTimeDomainIdent;       // pointer to write back time domain ident
} EDDP_USER_CMD_PARAM_GSYNC_TIME_DOMAIN_IDENT_TYPE;

// main struct for User-CMD-API to call the functions: eddp_UserCmdXxx()
typedef struct _EDDP_USER_CMD_PARAM_TYPE    EDD_UPPER_MEM_ATTR *    EDDP_USER_CMD_PARAM_PTR_TYPE;
typedef struct _EDDP_USER_CMD_PARAM_TYPE
{               
    LSA_UINT32          CmdOpcode;
    LSA_UINT8           Async;

    union
    {
        EDDP_USER_CMD_PARAM_GSYNC_UPDATE_APPCYCL_TYPE       UpdateAppCycl;  /* CmdOpcode = EDDP_USER_CMD_GSYNC_UPDATE_APPCYCL   */
        EDDP_USER_CMD_PARAM_GSYNC_CONNECT_FREE_TYPE         ConnectFree;    /* CmdOpcode = EDDP_USER_CMD_GSYNC_CONNECT_FREE     */
        EDDP_USER_CMD_PARAM_GSYNC_CONNECT_SYNC_A_TYPE       ConnectSyncA;   /* CmdOpcode = EDDP_USER_CMD_GSYNC_CONNECT_SYNC_A   */
        EDDP_USER_CMD_PARAM_GSYNC_TIME_STAMP_GET_TYPE       TimeStampGet;   /* CmdOpcode = EDDP_USER_CMD_GSYNC_TIME_STAMP_GET or EDDP_USER_CMD_GSYNC_TIME_SYNC_IND_GET */
	} Param;

} EDDP_USER_CMD_PARAM_TYPE;
/* CmdOpcode        : defines the command opcode to specify the type of "Param" union       */
/* Async            : EDDP_USER_CMD_ASYNC_PROCESSING: asynchronous command processing       */
/*                    EDDP_USER_CMD_SYNC_PROCESSING : synchronous command processing        */
#define EDDP_USER_CMD_ASYNC_PROCESSING  1
#define EDDP_USER_CMD_SYNC_PROCESSING   0

// structs for User-CMD-API Shared Memory and the Circular List
PNIO_PACKED_ATTRIBUTE_PRE struct EDDP_CIRC_LIST_DESCR_TYPE_
{
    LSA_UINT32 SrcWriteIndex;     /* WriteIndex of the source        */
    LSA_UINT32 DstReadIndex;      /* ReadIndex of the destination    */
    LSA_UINT32 ListSize;          /* List size in elements           */
    LSA_UINT32 ListOffset;        /* Offset of the timestamp list    */
                                  /* from the start of Shared Mem    */
    LSA_UINT32 ElementSize;       /* Size of one list element        */
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct EDDP_CIRC_LIST_DESCR_TYPE_    EDDP_CIRC_LIST_DESCR_TYPE;

PNIO_PACKED_ATTRIBUTE_PRE struct EDDP_TIMESTAMP_INFO_PAIR_TYPE_
{
    EDDP_TIMESTAMP_INFO_TYPE   OldTimestamp;
    EDDP_TIMESTAMP_INFO_TYPE   NewTimestamp;
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct EDDP_TIMESTAMP_INFO_PAIR_TYPE_   EDDP_TIMESTAMP_INFO_PAIR_TYPE;

PNIO_PACKED_ATTRIBUTE_PRE struct EDDP_USERCMDSHAREDMEM_TYPE_
{
    EDDP_CIRC_LIST_DESCR_TYPE       CircListDescr;
    EDDP_TIMESTAMP_INFO_TYPE        TimestampInfoBackup;
    EDDP_TIMESTAMP_INFO_PAIR_TYPE   TimestampList[EDDP_CFG_USR_CMD_SHAREDMEM_MAXENTRIES];
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct EDDP_USERCMDSHAREDMEM_TYPE_  EDDP_USERCMDSHAREDMEM_TYPE;

/* ------------------------------------------------------------------------------------------------------------ */
/* PACKED(4) STRUCTS - OFF                                                                                      */
/* ------------------------------------------------------------------------------------------------------------ */
#include "pnio_pck_off.h"

/*===========================================================================*/
/*              PN-IP register defines for application                       */
/*      mapping of PN-IP defines to EDDP defines to use it for macros        */
/*              EDDP_USR_REG32_READ, EDDP_USR_REG32_WRITE                    */
/*===========================================================================*/
// sync control
#define EDDP_PNIP_REG_APPLSTATUS                (PNIP_REG_APPLSTATUS)
#define EDDP_PNIP_REG_APPLTIMER_2               (PNIP_REG_APPLTIMER_2)
#define EDDP_PNIP_REG_APPLLENGTH_2              (PNIP_REG_APPLLENGTH_2)
#define EDDP_PNIP_REG_APPLRCF_2                 (PNIP_REG_APPLRCF_2)
#define EDDP_PNIP_REG_APPLCOMPARE_2_1           (PNIP_REG_APPLCOMPARE_2_1)
#define EDDP_PNIP_REG_APPLCOMPARE_2_2           (PNIP_REG_APPLCOMPARE_2_2)
#define EDDP_PNIP_REG_APPLCOMPARE_2_3           (PNIP_REG_APPLCOMPARE_2_3)
#define EDDP_PNIP_REG_APPLCOMPARE_2_4           (PNIP_REG_APPLCOMPARE_2_4)
#define EDDP_PNIP_REG_APPLCOMPARE_2_5           (PNIP_REG_APPLCOMPARE_2_5)
#define EDDP_PNIP_REG_APPLCOMPARE_2_6           (PNIP_REG_APPLCOMPARE_2_6)
#define EDDP_PNIP_REG_APPLCOMPARE_2_7           (PNIP_REG_APPLCOMPARE_2_7)
#define EDDP_PNIP_REG_APPLTIMER_3               (PNIP_REG_APPLTIMER_3)
#define EDDP_PNIP_REG_APPLLENGTH_3              (PNIP_REG_APPLLENGTH_3)
#define EDDP_PNIP_REG_APPLRCF_3                 (PNIP_REG_APPLRCF_3)
#define EDDP_PNIP_REG_APPLCOMPARE_3_1           (PNIP_REG_APPLCOMPARE_3_1)
#define EDDP_PNIP_REG_APPLCOMPARE_3_2           (PNIP_REG_APPLCOMPARE_3_2)
#define EDDP_PNIP_REG_APPLCOMPARE_3_3           (PNIP_REG_APPLCOMPARE_3_3)
#define EDDP_PNIP_REG_APPLCOMPARE_3_4           (PNIP_REG_APPLCOMPARE_3_4)
#define EDDP_PNIP_REG_APPLCOMPARE_3_5           (PNIP_REG_APPLCOMPARE_3_5)
#define EDDP_PNIP_REG_APPLCOMPARE_3_6           (PNIP_REG_APPLCOMPARE_3_6)
#define EDDP_PNIP_REG_APPLCOMPARE_3_7           (PNIP_REG_APPLCOMPARE_3_7)
#define EDDP_PNIP_REG_PLL_OUT_CONTROL_1         (PNIP_REG_PLL_OUT_CONTROL_1)
#define EDDP_PNIP_REG_PLL_OUT_CONTROL_2         (PNIP_REG_PLL_OUT_CONTROL_2)
#define EDDP_PNIP_REG_PLL_OUT_CONTROL_3         (PNIP_REG_PLL_OUT_CONTROL_3)
#define EDDP_PNIP_REG_PLL_OUT_CONTROL_4         (PNIP_REG_PLL_OUT_CONTROL_4)
#define EDDP_PNIP_REG_PLL_OUT_CONTROL_5         (PNIP_REG_PLL_OUT_CONTROL_5)
#define EDDP_PNIP_REG_PLL_OUT_CONTROL_6         (PNIP_REG_PLL_OUT_CONTROL_6)
#define EDDP_PNIP_REG_PLL_OUT_CONTROL_7         (PNIP_REG_PLL_OUT_CONTROL_7)
#define EDDP_PNIP_REG_PLL_OUT_CONTROL_8         (PNIP_REG_PLL_OUT_CONTROL_8)
#define EDDP_PNIP_REG_PLL_OUT_CONTROL_9         (PNIP_REG_PLL_OUT_CONTROL_9)
#define EDDP_PNIP_REG_PLL_OUT_CONTROL_10        (PNIP_REG_PLL_OUT_CONTROL_10)
#define EDDP_PNIP_REG_PLL_OUT_CONTROL_11        (PNIP_REG_PLL_OUT_CONTROL_11)
#define EDDP_PNIP_REG_PLL_OUT_CONTROL_12        (PNIP_REG_PLL_OUT_CONTROL_12)
#define EDDP_PNIP_REG_PLL_OUT_CONTROL_13        (PNIP_REG_PLL_OUT_CONTROL_13)
#define EDDP_PNIP_REG_PLL_OUT_CONTROL_14        (PNIP_REG_PLL_OUT_CONTROL_14)
#define EDDP_PNIP_REG_PLL_OUT_CONTROL_15        (PNIP_REG_PLL_OUT_CONTROL_15)
#define EDDP_PNIP_REG_PLL_OUT_CONTROL_16        (PNIP_REG_PLL_OUT_CONTROL_16)
#define EDDP_PNIP_REG_PLL_OUT_CONTROL_17        (PNIP_REG_PLL_OUT_CONTROL_17)
#define EDDP_PNIP_REG_PLL_OUT_CONTROL_18        (PNIP_REG_PLL_OUT_CONTROL_18)
#define EDDP_PNIP_REG_PLL_OUT_CONTROL_19        (PNIP_REG_PLL_OUT_CONTROL_19)
#define EDDP_PNIP_REG_PLL_OUT_CONTROL_20        (PNIP_REG_PLL_OUT_CONTROL_20)
#define EDDP_PNIP_REG_IRQ_CONTROL_4             (PNIP_REG_IRQ_CONTROL_4)
#define EDDP_PNIP_REG_IRQ_CONTROL_5             (PNIP_REG_IRQ_CONTROL_5)
#define EDDP_PNIP_REG_IRQ_CONTROL_6             (PNIP_REG_IRQ_CONTROL_6)
#define EDDP_PNIP_REG_IRQ_CONTROL_7             (PNIP_REG_IRQ_CONTROL_7)
#define EDDP_PNIP_REG_IRQ_CONTROL_8             (PNIP_REG_IRQ_CONTROL_8)
#define EDDP_PNIP_REG_IRQ_CONTROL_9             (PNIP_REG_IRQ_CONTROL_9)
// interrupt control
#define EDDP_PNIP_REG_IRQ2EVENT_LOW_1           (PNIP_REG_IRQ2EVENT_LOW_1)
#define EDDP_PNIP_REG_IRQ2EVENT_MID_1           (PNIP_REG_IRQ2EVENT_MID_1)
#define EDDP_PNIP_REG_IRQ2EVENT_HIGH_1          (PNIP_REG_IRQ2EVENT_HIGH_1)
#define EDDP_PNIP_REG_IRQ2MASK_LOW_1            (PNIP_REG_IRQ2MASK_LOW_1)
#define EDDP_PNIP_REG_IRQ2MASK_MID_1            (PNIP_REG_IRQ2MASK_MID_1)
#define EDDP_PNIP_REG_IRQ2MASK_HIGH_1           (PNIP_REG_IRQ2MASK_HIGH_1)
#define EDDP_PNIP_REG_IRQ2_LOW_1                (PNIP_REG_IRQ2_LOW_1)
#define EDDP_PNIP_REG_IRQ2_MID_1                (PNIP_REG_IRQ2_MID_1)
#define EDDP_PNIP_REG_IRQ2_HIGH_1               (PNIP_REG_IRQ2_HIGH_1)
#define EDDP_PNIP_REG_IRQ2ACK_LOW_1             (PNIP_REG_IRQ2ACK_LOW_1)
#define EDDP_PNIP_REG_IRQ2ACK_MID_1             (PNIP_REG_IRQ2ACK_MID_1)
#define EDDP_PNIP_REG_IRQ2ACK_HIGH_1            (PNIP_REG_IRQ2ACK_HIGH_1)
#define EDDP_PNIP_REG_IRQ2_EOI_1                (PNIP_REG_IRQ2_EOI_1)
#define EDDP_PNIP_REG_IRQ3EVENT_LOW_1           (PNIP_REG_IRQ3EVENT_LOW_1)
#define EDDP_PNIP_REG_IRQ3EVENT_MID_1           (PNIP_REG_IRQ3EVENT_MID_1)
#define EDDP_PNIP_REG_IRQ3EVENT_HIGH_1          (PNIP_REG_IRQ3EVENT_HIGH_1)
#define EDDP_PNIP_REG_IRQ3MASK_LOW_1            (PNIP_REG_IRQ3MASK_LOW_1)
#define EDDP_PNIP_REG_IRQ3MASK_MID_1            (PNIP_REG_IRQ3MASK_MID_1)
#define EDDP_PNIP_REG_IRQ3MASK_HIGH_1           (PNIP_REG_IRQ3MASK_HIGH_1)
#define EDDP_PNIP_REG_IRQ3_LOW_1                (PNIP_REG_IRQ3_LOW_1)
#define EDDP_PNIP_REG_IRQ3_MID_1                (PNIP_REG_IRQ3_MID_1)
#define EDDP_PNIP_REG_IRQ3_HIGH_1               (PNIP_REG_IRQ3_HIGH_1)
#define EDDP_PNIP_REG_IRQ3ACK_LOW_1             (PNIP_REG_IRQ3ACK_LOW_1)
#define EDDP_PNIP_REG_IRQ3ACK_MID_1             (PNIP_REG_IRQ3ACK_MID_1)
#define EDDP_PNIP_REG_IRQ3ACK_HIGH_1            (PNIP_REG_IRQ3ACK_HIGH_1)
#define EDDP_PNIP_REG_IRQ3_EOI_1                (PNIP_REG_IRQ3_EOI_1)
// cycle API
#define EDDP_PNIP_REG_PPMGROUPSTART_G1          (PNIP_REG_PPMGROUPSTART_G1)
#define EDDP_PNIP_REG_PPMGROUPSTART_G2          (PNIP_REG_PPMGROUPSTART_G2)
#define EDDP_PNIP_REG_PPMTRANSFEREND_G1         (PNIP_REG_PPMTRANSFEREND_G1)
#define EDDP_PNIP_REG_PPMTRANSFEREND_G2         (PNIP_REG_PPMTRANSFEREND_G2)
#define EDDP_PNIP_REG_FRAMECOUNT_G1             (PNIP_REG_FRAMECOUNT_G1)
#define EDDP_PNIP_REG_FRAMECOUNT_G2             (PNIP_REG_FRAMECOUNT_G2)
#define EDDP_PNIP_REG_FRAMECOUNT_G3             (PNIP_REG_FRAMECOUNT_G3)
#define EDDP_PNIP_REG_CPMGROUPCYCLE_G1          (PNIP_REG_CPMGROUPCYCLE_G1)
#define EDDP_PNIP_REG_CPMGROUPCYCLE_G2          (PNIP_REG_CPMGROUPCYCLE_G2)
#define EDDP_PNIP_REG_CPMGROUPCYCLE_G3          (PNIP_REG_CPMGROUPCYCLE_G3)
#define EDDP_PNIP_REG_CPMTRANSFEREND_G1         (PNIP_REG_CPMTRANSFEREND_G1)
#define EDDP_PNIP_REG_CPMTRANSFEREND_G2         (PNIP_REG_CPMTRANSFEREND_G2)
#define EDDP_PNIP_REG_CPMTRANSFEREND_G3         (PNIP_REG_CPMTRANSFEREND_G3)
#define EDDP_PNIP_REG_CPM_OUTDATACOUNT          (PNIP_REG_CPM_OUTDATACOUNT)
//IP Verion/Development
#define EDDP_PNIP_REG_IP_VERSION                (PNIP_REG_IP_VERSION)
#define EDDP_PNIP_REG_IP_DEVELOPMENT            (PNIP_REG_IP_DEVELOPMENT)

/*===========================================================================*/
/*                      Define for System Redundancy API                     */
/*===========================================================================*/

#define EDDP_MAX_NR_APPL_PROVIDERS 18

/*===========================================================================*/
/*                      EDDP Swapping macros                                 */
/*===========================================================================*/

/* swaps a byte pattern of 0102 to 0201 */
#ifndef EDDP_SWAP16
#define EDDP_SWAP16(C) ((LSA_UINT16)((((LSA_UINT16)(C) & 0xFF00) >> 8) | \
                                     (((LSA_UINT16)(C) & 0x00FF) << 8)))
#endif

#ifndef EDDP_SWAP32
/* swaps a byte pattern of 01020304 to 04030201 */
#define EDDP_SWAP32(C) ((LSA_UINT32)((((LSA_UINT32)(C) & 0xFF000000) >> 24) | \
                                     (((LSA_UINT32)(C) & 0x00FF0000) >>  8) | \
                                     (((LSA_UINT32)(C) & 0x0000FF00) <<  8) | \
                                     (((LSA_UINT32)(C) & 0x000000FF) << 24)))
#endif

#ifndef EDDP_SWAP64
/* swaps a byte pattern of 0102030405060708 to 0807060504030201 */
#define EDDP_SWAP64(C) ((LSA_UINT64)((((LSA_UINT64)(C) & 0xFF00000000000000) >> 56) | \
                                     (((LSA_UINT64)(C) & 0x00FF000000000000) >> 40) | \
                                     (((LSA_UINT64)(C) & 0x0000FF0000000000) >> 24) | \
                                     (((LSA_UINT64)(C) & 0x000000FF00000000) >>  8) | \
                                     (((LSA_UINT64)(C) & 0x00000000FF000000) <<  8) | \
                                     (((LSA_UINT64)(C) & 0x0000000000FF0000) << 24) | \
                                     (((LSA_UINT64)(C) & 0x000000000000FF00) << 40) | \
                                     (((LSA_UINT64)(C) & 0x00000000000000FF) << 56))) 
#endif


/*===========================================================================*/
/*                              User macros                                  */
/*===========================================================================*/

/*=============================================================================
 * macro name   : EDDP_USR_REG32_READ
 * function     : read value from the PN-IP register
 * parameters   : EDD_COMMON_MEM_PTR_TYPE   pPNIPBase   : pointer to internal memory of the PN-IP from HOST view
 *                LSA_UINT32			 	RegOffs     : PN-IP register offset
 * return value : register value
 *===========================================================================*/
#ifndef EDDP_USR_REG32_READ
#ifdef EDDP_CFG_BIG_ENDIAN
    #define EDDP_USR_REG32_READ(pPNIPBase, RegOffs) \
                EDDP_SWAP32( *((volatile EDD_COMMON_MEM_U32_PTR_TYPE)(((volatile EDD_COMMON_MEM_U8_PTR_TYPE)(pPNIPBase)) + (RegOffs))) );
#else
    #define EDDP_USR_REG32_READ(pPNIPBase, RegOffs) \
                *((volatile EDD_COMMON_MEM_U32_PTR_TYPE)(((volatile EDD_COMMON_MEM_U8_PTR_TYPE)(pPNIPBase)) + (RegOffs)));
#endif
#endif

/*=============================================================================
 * macro name   : EDDP_USR_REG32_WRITE
 * function     : write value to the PN-IP register
 * parameters   : EDD_COMMON_MEM_PTR_TYPE   pPNIPBase   : pointer to internal memory of the PN-IP from HOST view
 *                LSA_UINT32                RegOffs     : PN-IP register offset
 *                LSA_UINT32                Value       : value to write to the PN-IP Register
 * return value : no
 *===========================================================================*/
#ifndef EDDP_USR_REG32_WRITE
#ifdef EDDP_CFG_BIG_ENDIAN
    #define EDDP_USR_REG32_WRITE(pPNIPBase, RegOffs, Value) \
                *((volatile EDD_COMMON_MEM_U32_PTR_TYPE)((volatile EDD_COMMON_MEM_U8_PTR_TYPE)(pPNIPBase) + (RegOffs))) = EDDP_SWAP32((LSA_UINT32)(Value));
#else
    #define EDDP_USR_REG32_WRITE(pPNIPBase, RegOffs, Value) \
                *((volatile EDD_COMMON_MEM_U32_PTR_TYPE)((volatile EDD_COMMON_MEM_U8_PTR_TYPE)(pPNIPBase) + (RegOffs))) = (LSA_UINT32)(Value);
#endif
#endif

/*===========================================================================*/
/*  Conversion of  byteorder from/to Hostbyteorder                          */
/*===========================================================================*/
#ifdef EDDP_CFG_BIG_ENDIAN

#ifndef LE_TO_H_S
#define LE_TO_H_S(Value) ( EDDS_SWAP16(Value) )  /* Little Endian to Host Short */
#endif

#ifndef LE_TO_H_L
#define LE_TO_H_L(Value) ( EDDS_SWAP32(Value) )  /* Little Endian to Host Long  */
#endif

#ifndef H_TO_LE_S
#define H_TO_LE_S(Value) ( EDDS_SWAP16(Value) )  /* Host to Little Endian Short */
#endif

#ifndef H_TO_LE_L
#define H_TO_LE_L(Value) ( EDDS_SWAP32(Value) )  /* Host to Little Endian Long  */
#endif

#else //EDDP_CFG_BIG_ENDIAN is not defined, we assume that we are on littel endian system

#ifndef LE_TO_H_S
#define LE_TO_H_S(Value) (Value)  /* Little Endian to Host Short */
#endif

#ifndef LE_TO_H_L
#define LE_TO_H_L(Value) (Value)  /* Little Endian to Host Long  */
#endif

#ifndef H_TO_LE_S
#define H_TO_LE_S(Value) (Value)  /* Host to Little Endian Short */
#endif

#ifndef H_TO_LE_L
#define H_TO_LE_L(Value) (Value)  /* Host to Little Endian Long  */
#endif

#endif


/*===========================================================================*/
/*                          User Prototypes                                  */
/*===========================================================================*/

EDD_RSP EDD_UPPER_IN_FCT_ATTR eddp_UserCmdInit(
	LSA_UINT16              EDDPLocation, 
    EDD_COMMON_MEM_PTR_TYPE pSharedMem);

EDD_RSP EDD_UPPER_IN_FCT_ATTR eddp_UserCmdDeinit(LSA_VOID);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_UserCmdInternDeinitHandleMngm(LSA_VOID);

EDD_RSP EDD_UPPER_IN_FCT_ATTR eddp_UserCmdInitHandle(
	EDD_COMMON_MEM_PTR_TYPE         pPnipBase,
	EDDP_USER_CMD_HANDLE_TYPE      *pHandle);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_UserCmdInternFreeHandle(
	EDDP_USER_CMD_HANDLE_TYPE       Handle);

EDD_RSP EDD_UPPER_IN_FCT_ATTR eddp_UserCmdFreeHandle(
	EDDP_USER_CMD_HANDLE_TYPE       Handle);

EDD_RSP EDD_UPPER_IN_FCT_ATTR eddp_UserCmdRequest(
	EDDP_USER_CMD_HANDLE_TYPE       Handle,
 	EDDP_USER_CMD_PARAM_PTR_TYPE    pParam);

EDD_RSP EDD_UPPER_IN_FCT_ATTR eddp_UserCmdReadyTest(
	EDDP_USER_CMD_HANDLE_TYPE       Handle);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_UserCmdInternInitHandleMngm(
    LSA_UINT16              EDDPLocation, 
    EDD_COMMON_MEM_PTR_TYPE pSharedMem);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_UserCmdInternGetNewHandle(
    EDD_COMMON_MEM_PTR_TYPE         pPnipBase,
    EDDP_USER_CMD_HANDLE_TYPE      *pHandle);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_UserCmdInternRequest(
	EDDP_USER_CMD_HANDLE_TYPE       Handle,
 	EDDP_USER_CMD_PARAM_PTR_TYPE    pParam);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_UserCmdInternReadyTest(
	EDDP_USER_CMD_HANDLE_TYPE       Handle);

/*===========================================================================*/
/*                          User Prototypes - System redundancy              */
/*===========================================================================*/

LSA_VOID   eddp_SysRed_Init( 
           LSA_VOID);

EDD_RSP    eddp_SysRed_Setup ( 
           LSA_UINT32 const    InstanceHandle, 
           LSA_UINT32 const    TraceIdx, 
           LSA_VOID_PTR_TYPE   pPnipBase, 
           LSA_VOID_PTR_TYPE   pGSharedRAM,
           LSA_UINT16          const   Hardware);

EDD_RSP    eddp_SysRed_UndoSetup ( 
           LSA_UINT32    const   InstanceHandle);

EDD_RSP    eddp_SysRed_Provider_Add ( 
           LSA_UINT32      const   InstanceHandle,
           LSA_UINT16*     const   pApplProviderID,
           LSA_UINT32      const   DataStatusOffset,
           LSA_UINT8       const   ProvType);

EDD_RSP    eddp_SysRed_Provider_Remove( 
           LSA_UINT32    const   InstanceHandle,
           LSA_UINT16    const   ApplProviderID);

EDD_RSP    eddp_SysRed_ProviderDataStatus_Set ( 
           LSA_UINT32    const   InstanceHandle,
           LSA_UINT16    const   ApplProviderID,
           LSA_UINT8     const   Status,
           LSA_UINT8     const   Mask);

EDD_RSP    eddp_SysRed_ShM_Array_Set( 
           LSA_UINT32    const   InstanceHandle,
           LSA_UINT16    const   ShMIndex,
           LSA_UINT16    const   ShMValue);

LSA_VOID   eddp_SysRed_Deinit( LSA_VOID);





/*===========================================================================*/
/*                              User RQBs                                    */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* Service within RQB for EDD_OPC_REQUEST and EDD_OPC_REQUEST_SRT_BUFFER     */
/*                                                                           */
/* Bits 08..15 used for decoding of service by EDD-ERTEC. Do not change!     */
/*                                                                           */
/* Range 0x8000..0x8FFF can be used for EDD depended services!               */
/* other special service values maybe reserved within edd_usr.h              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* RQB- service-specific paramteter blocks, pParam points to.                */
/*      Opcode and Service specify the Paramblock                            */
/*                                                                           */
/*   RQB-Header                                                              */
/*  +------------+                                                           */
/*  |            |                                                           */
/*  |            |                                                           */
/*  |            |                                                           */
/*  |            |                                                           */
/*  |            |                                                           */
/*  |            |                                                           */
/*  |            |                                                           */
/*  |            |                                                           */
/*  +------------+ service-       e.g. EDDP_UPPER_GET_STATS_PTR_TYPE         */
/*  |   pParam   | ------------->+-----------+                               */
/*  +------------+ specific      |           |                               */
/*                               | +------------+                            */
/*                               | |            |                            */
/*                               | | +-------------+                         */
/*                               | | |             |                         */
/*                                                                           */
/*                                                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Opcode: EDD_OPC_REQUEST  Service: xxx                                     */
/*---------------------------------------------------------------------------*/


/*===========================================================================*/
/*===========================================================================*/



/*****************************************************************************/
/*                                                                           */
/* Output-functions are in eddp_out.h                                        */
/*                                                                           */
/*****************************************************************************/


/*****************************************************************************/
/*  end of file eddp_usr.h                                                   */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_USR_H */
