#ifndef EDDP_HAL_PNIP_STRUCT_H           /* ----- reinclude-protection ----- */
#define EDDP_HAL_PNIP_STRUCT_H
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
/*  F i l e               &F: eddp_hal_pnip_struct.h                    :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  structures, defines of PNIP-RAM (DMACW, ACW, ...)                        */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  05.03.09    JS    initial version                                        */
#endif
/*****************************************************************************/


/* ---------------------------------------------------------------------------------------------------------------------------- */
/* Rules for packing of structs: <pnio_pck1/2/4_on.h>                                                                           */
/* ---------------------------------------------------------------------------------------------------------------------------- */
/* 1) Parameters in structs must lie naturally aligned (perhaps it can be added with padding parameters if it is possible)      */
/* 2) The size of struct must be multiple of its greatest parameter                                                             */
/* 3) The value of pack(x) orientates itself after greatest parameter in the struct:                                            */
/*           8 Bit  -> pack(1)                                                                                                  */
/*          16 Bit  -> pack(2)                                                                                                  */
/*          32 Bit  -> pack(4)                                                                                                  */
/*          64 Bit  -> pack(4), pack(8) only on 64-Bit-System necessary                                                         */
/* 4) The size of struct must be multiple of its pack(x). It means: with pack(4) the multiple of 4 bytes                        */
/* ---------------------------------------------------------------------------------------------------------------------------- */


#ifndef K32_KRISC32OS
/* the following structs are only for EDDP (not for KRISC32OS) */

/* ------------------------------------------------------------------------------------------------------------ */
/* PACKED(1) STRUCTS - ON                                                                                       */
/* ------------------------------------------------------------------------------------------------------------ */
/* Rules for packing of structs see <eddp_hal_pnip_struct.h> */
#include "pnio_pck1_on.h"

/* ------------------------------------------------------------------------- */
/* ACW_DG Receive - DataStatus                                               */
/* ------------------------------------------------------------------------- */
typedef struct _PNIP_DG_STATUS_RCV_TYPE     EDD_COMMON_MEM_ATTR *   PNIP_DG_STATUS_RCV_PTR_TYPE;
PNIO_PACKED_ATTRIBUTE_PRE struct _PNIP_DG_STATUS_RCV_TYPE
{
    LSA_UINT8           CycleCounter;
    LSA_UINT8           DataStatus;
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _PNIP_DG_STATUS_RCV_TYPE     PNIP_DG_STATUS_RCV_TYPE;
/* CycleCounter     : used for old/new detection                                                */
/* DataStatus       : some indication flags                                                     */

/* ------------------------------------------------------------------------- */
/* ACW Send - DataStatus Revision 1                                          */
/* ------------------------------------------------------------------------- */
#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
typedef struct _PNIP_R1_ACW_SND_DS_TYPE    EDD_COMMON_MEM_ATTR *   PNIP_R1_ACW_SND_DS_PTR_TYPE;
PNIO_PACKED_ATTRIBUTE_PRE struct _PNIP_R1_ACW_SND_DS_TYPE
{
    LSA_UINT8           DataStatus;
    LSA_UINT8           DS_Selector;
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _PNIP_R1_ACW_SND_DS_TYPE    PNIP_R1_ACW_SND_DS_TYPE;
/* DataStatus       : DataStatus to create the APDU.DataStatus                                  */
/* DS_Selector      : Index (GroupId of provider) to select the register                        */
/*                      - ApplDataStatus_0-7        -> for ACW                                  */
/*                      - DG_DataStatus_0-7         -> for ACW_DG                               */
#endif

/* ------------------------------------------------------------------------- */
/* ACW Send - DataStatus Revision 2 and 3                                         */
/* ------------------------------------------------------------------------- */
#if defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
typedef struct _PNIP_R2_ACW_SND_DS_TYPE    EDD_COMMON_MEM_ATTR *   PNIP_R2_ACW_SND_DS_PTR_TYPE;
PNIO_PACKED_ATTRIBUTE_PRE struct _PNIP_R2_ACW_SND_DS_TYPE
{
    LSA_UINT8           DataStatus;
    LSA_UINT8           DS_Selector;
    LSA_UINT8           DataStatus_Saved;
    LSA_UINT8           Reserved;
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _PNIP_R2_ACW_SND_DS_TYPE    PNIP_R2_ACW_SND_DS_TYPE;
/* DataStatus       : DataStatus to create the APDU.DataStatus                                  */
/* DS_Selector      : Index (GroupId of provider) to select the register                        */
/*                      - ApplDataStatus_0-7        -> for ACW                                  */
/*                      - DG_DataStatus_0-7         -> for ACW_DG                               */
/* DataStatus_Saved : to save old-new Datastatus-> access for HW                                */
/* Reserved         : Reserved                                                                  */
#endif

/* ------------------------------------------------------------------------------------------------------------ */
/* PACKED(1) STRUCTS - OFF                                                                                      */
/* ------------------------------------------------------------------------------------------------------------ */
#include "pnio_pck_off.h"


/* ------------------------------------------------------------------------------------------------------------ */
/* PACKED(2) STRUCTS - ON                                                                                       */
/* ------------------------------------------------------------------------------------------------------------ */
/* Rules for packing of structs see <eddp_hal_pnip_struct.h> */
#include "pnio_pck2_on.h"

/* ------------------------------------------------------------------------- */
/* ACW Receive - RBase-Entry: one ACW_List_Ptr in RBase array                */
/* ------------------------------------------------------------------------- */
typedef struct _PNIP_ACW_RCV_RBASE_TYPE     EDD_COMMON_MEM_ATTR *   PNIP_ACW_RCV_RBASE_PTR_TYPE;
PNIO_PACKED_ATTRIBUTE_PRE struct _PNIP_ACW_RCV_RBASE_TYPE
{
    LSA_UINT16          ACW_List_Ptr;       // ACW_List_Ptr -> ACW 1 -> ACW 2 -> ... -> ACW 0xFFFF
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _PNIP_ACW_RCV_RBASE_TYPE     PNIP_ACW_RCV_RBASE_TYPE;
/* ACW_List_Ptr     : beginn of ACW chain - address (offset) of first ACW in APICtrl-RAM, 64Bit alined - last 3 bits are cut off      */

/* ------------------------------------------------------------------------- */
/* ACW Send - TBase: one TBaseElement in ACW reduction list                  */
/* ------------------------------------------------------------------------- */
typedef struct _PNIP_ACW_SND_TBASE_TYPE     EDD_COMMON_MEM_ATTR *   PNIP_ACW_SND_TBASE_PTR_TYPE;
PNIO_PACKED_ATTRIBUTE_PRE struct _PNIP_ACW_SND_TBASE_TYPE
{
    LSA_UINT16          TBaseElement;
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _PNIP_ACW_SND_TBASE_TYPE     PNIP_ACW_SND_TBASE_TYPE;
/* TBaseElement:                                                                                        */
/*      - Bit(15)   : Red_enable                                                                        */
/*      - Bit(14)   : Orange_enable                                                                     */
/*      - Bit(13)   : Green_enable                                                                      */
/*      - Bit(12)   : DG_CC_enable                                                                      */
/*      - Bit(11:0) : ROG_offset    - Offset to address in Reg. "PNIP_REG_ACW_TROGPTR_IFA/IFB"          */
/*                                  - 64Bit alined last 3 bits are cut off                              */
/* Masks for TBaseElement (LSA_UINT16)                                                                  */
#define PNIP_ACW_SND_TBASE_ROG_OFFSET_MASK              0x0FFF          // Bit(11:0)
#define PNIP_ACW_SND_TBASE_ROG_OFFSET_SHIFT             0
#define PNIP_ACW_SND_TBASE_DG_CC_ENABLE_MASK            0x1000          // Bit(12)
#define PNIP_ACW_SND_TBASE_DG_CC_ENABLE_SHIFT           12
#define PNIP_ACW_SND_TBASE_GREEN_ENABLE_MASK            0x2000          // Bit(13)
#define PNIP_ACW_SND_TBASE_GREEN_ENABLE_SHIFT           13
#define PNIP_ACW_SND_TBASE_ORANGE_ENABLE_MASK           0x4000          // Bit(14)
#define PNIP_ACW_SND_TBASE_ORANGE_ENABLE_SHIFT          14
#define PNIP_ACW_SND_TBASE_RED_ENABLE_MASK              0x8000          // Bit(15)
#define PNIP_ACW_SND_TBASE_RED_ENABLE_SHIFT             15
#define PNIP_ACW_SND_TBASE_BIT_OFF                      0x0             // Bit in TBaseElement activate
#define PNIP_ACW_SND_TBASE_BIT_ON                       0x1             // Bit in TBaseElement deactivate

/* ------------------------------------------------------------------------------------------------------------ */
/* PACKED(2) STRUCTS - OFF                                                                                      */
/* ------------------------------------------------------------------------------------------------------------ */
#include "pnio_pck_off.h"

#endif  // K32_KRISC32OS



/* ------------------------------------------------------------------------------------------------------------ */
/* PACKED(4) STRUCTS - ON                                                                                       */
/* ------------------------------------------------------------------------------------------------------------ */
/* Rules for packing of structs see <eddp_hal_pnip_struct.h> */
#ifdef K32_KRISC32OS
    #include "pnio_k32_pck4_on.h"
#else
    #include "pnio_pck4_on.h"
#endif


/********************************************************************************************************************/
/* SYNC IF Control ParamStruct and Phase/Time List                                                                  */
/********************************************************************************************************************/
#define PNIP_SWI_SYNC_PARAM_STRUCT_STAMP_SIZE_32        4
#define PNIP_SWI_SYNC_MAX_PHASES                        16

typedef struct _PNIP_SWI_RAMSUB_SYNC_PARAM_STRUCT_TYPE     EDD_COMMON_MEM_ATTR *       PNIP_SWI_RAMSUB_SYNC_PARAM_STRUCT_PTR_TYPE;
PNIO_PACKED_ATTRIBUTE_PRE struct _PNIP_SWI_RAMSUB_SYNC_PARAM_STRUCT_TYPE
{
    /* ParamStruct = SYNCParamStruct_Base + 8 * ParamStruct_offset(UserId) */
    LSA_UINT64				Param_User_ID_0;    // for EDDP_CMD_USER_ID_HOST_ASYNC
	LSA_UINT64				Param_User_ID_1;    // for EDDP_CMD_USER_ID_HOST_SYNC
	LSA_UINT64				Param_User_ID_2;    // for EDDP_CMD_USER_ID_USER_ASYNC
	LSA_UINT64				Param_User_ID_3;    // for EDDP_CMD_USER_ID_USER_SYNC
	LSA_UINT64				Param_User_ID_4;    // for EDDP_CMD_USER_ID_KRISC32_ASYNC
	LSA_UINT64				Param_User_ID_5;    // for EDDP_CMD_USER_ID_KRISC32_SYNC
	LSA_UINT64				Param_User_ID_6;    // for EDDP_CMD_USER_ID_RESERVED_ASYNC
	LSA_UINT64				Param_User_ID_7;    // for EDDP_CMD_USER_ID_RESERVED_SYNC
	
	LSA_UINT32              ParamStructStamp[PNIP_SWI_SYNC_PARAM_STRUCT_STAMP_SIZE_32];
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _PNIP_SWI_RAMSUB_SYNC_PARAM_STRUCT_TYPE  PNIP_SWI_RAMSUB_SYNC_PARAM_STRUCT_TYPE;

typedef struct _PNIP_SWI_RAMSUB_SYNC_PHASE_LIST_TYPE     EDD_COMMON_MEM_ATTR *       PNIP_SWI_RAMSUB_SYNC_PHASE_LIST_PTR_TYPE;
PNIO_PACKED_ATTRIBUTE_PRE struct _PNIP_SWI_RAMSUB_SYNC_PHASE_LIST_TYPE
{
    LSA_UINT32      PhaseEntry[PNIP_SWI_SYNC_MAX_PHASES];
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _PNIP_SWI_RAMSUB_SYNC_PHASE_LIST_TYPE    PNIP_SWI_RAMSUB_SYNC_PHASE_LIST_TYPE;

typedef struct _PNIP_SWI_RAMSUB_SYNC_TIME_LIST_ENTRY_TYPE     EDD_COMMON_MEM_ATTR *       PNIP_SWI_RAMSUB_SYNC_TIME_LIST_ENTRY_PTR_TYPE;
PNIO_PACKED_ATTRIBUTE_PRE struct _PNIP_SWI_RAMSUB_SYNC_TIME_LIST_ENTRY_TYPE
{
    LSA_UINT32      TimeEntry;
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _PNIP_SWI_RAMSUB_SYNC_TIME_LIST_ENTRY_TYPE   PNIP_SWI_RAMSUB_SYNC_TIME_LIST_ENTRY_TYPE;





/********************************************************************************************************************/
/* DMACW = NRT                                                                                                      */
/********************************************************************************************************************/

/******************************************************************************************************************************/
/** Snd defines ***************************************************************************************************************/
/**             ***************************************************************************************************************/
/******************************************************************************************************************************/


/*---------------------------------------------------------------------------*/
/* SndDescriptor structure depends on HW (PNIP-ASIC)                         */
/*---------------------------------------------------------------------------*/

/*
16 TX-Rings

SndTableBasePtr IFA/B  -->  PNIP_DMACW_SND_DESCR_TYPE[1] --> PNIP_DMACW_SND_ITEM_TYPE --> ..  --> PNIP_DMACW_SND_ITEM_TYPE
                            PNIP_DMACW_SND_DESCR_TYPE[2]              A                                   |
                            ...                                       |                                   |
                            PNIP_DMACW_SND_DESCR_TYPE[15]             *-----------------------------------*

*/



/*
Layout of PNIP_DMACW_SND_DESCR_TYPE:
====================================
Bits:   31                  23                  15                  7               0
n   :                           CurrentDMACWPtr (4Byte aligned)
n+1 :             WatchdogStartTime(15:0),          -, Err, -,   INT,     -
*/

typedef struct _PNIP_DMACW_SND_DESCR_TYPE   EDDP_LOCAL_MEM_ATTR *   PNIP_LOCAL_DMACW_SND_DESCR_PTR_TYPE;
PNIO_PACKED_ATTRIBUTE_PRE struct _PNIP_DMACW_SND_DESCR_TYPE
{
    LSA_UINT32      CurrentDmacwHwPtr;      // 31..00: CurrentDMACWPtr (current processing DMACW)
    LSA_UINT32      SndWdtErrIntVal;        // 31..16: WatchdogStartTime: for timeout check in ASIC
                                            // 15    : WDEn: Enable Watchdog for this DMACW
                                            // 13..12: Err:  ring error status
                                            // 10..08: INT:  interrupt mask (3 IRQ lines)
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _PNIP_DMACW_SND_DESCR_TYPE   PNIP_DMACW_SND_DESCR_TYPE;

// PNIP_DMACW_SND_DESCR_TYPE.SndWdtErrIntVal
#define PNIP_DMACW_SND_DESCR_WDT_MSK          0xFFFF0000              // 31..16:  WatchdogStartTime
    #define PNIP_DMACW_SND_DESCR_WDT_SHIFT        16                  //          Shift Value
    #define PNIP_DMACW_SND_DESCR_WDT_OFF          ((LSA_UINT32)0<<16) //          watchdog off
#define PNIP_DMACW_SND_DESCR_WDEN_MSK         0x00008000              // 15:      Dis/Enable Watchdog for this DMACW
    #define PNIP_DMACW_SND_DESCR_WDEN_SHIFT       15                  //          Shift Value
    #define PNIP_DMACW_SND_DESCR_WDEN_ON         ((LSA_UINT32)1<<15)  //          Enable Watchdog for this DMACW
    #define PNIP_DMACW_SND_DESCR_WDEN_OFF        ((LSA_UINT32)0<<15)  //          Disable Watchdog for this DMACW
#define PNIP_DMACW_SND_DESCR_RSVD3_MSK        0x00004000              // 14:      reserved
    #define PNIP_DMACW_SND_DESCR_RSVD3_SHIFT      14                  //          Shift Value
#define PNIP_DMACW_SND_DESCR_ERR_MSK          0x00003000              // 13..12:  Err:    ring error status
    #define PNIP_DMACW_SND_DESCR_ERR_SHIFT        12                  //          Shift Value
    #define PNIP_DMACW_SND_DESCR_ERR_RESERVED     ((LSA_UINT32)3<<12) //          Reserved
    #define PNIP_DMACW_SND_DESCR_ERR_ALIGNMENT    ((LSA_UINT32)2<<12) //          AlignmentError
    #define PNIP_DMACW_SND_DESCR_ERR_AHB_READ     ((LSA_UINT32)1<<12) //          AHBReadErr
    #define PNIP_DMACW_SND_DESCR_ERR_OK           ((LSA_UINT32)0<<12) //          no error
#define PNIP_DMACW_SND_DESCR_RSVD2_MSK        0x00000800              // 11:      reserved
    #define PNIP_DMACW_SND_DESCR_RSVD2_SHIFT      11                  //          Shift Value
#define PNIP_DMACW_SND_DESCR_INT_MSK          0x00000700              // 10..08:  INT:    interrupt mask (3 IRQ lines)
    #define PNIP_DMACW_SND_DESCR_INT_SHIFT        8                   //          Shift Value
    #define PNIP_DMACW_SND_DESCR_INT_ICU3         ((LSA_UINT32)4<<8)  //          interrupt mask ICU3
    #define PNIP_DMACW_SND_DESCR_INT_ICU2         ((LSA_UINT32)2<<8)  //          interrupt mask ICU2
    #define PNIP_DMACW_SND_DESCR_INT_ICU1         ((LSA_UINT32)1<<8)  //          interrupt mask ICU1
    #define PNIP_DMACW_SND_DESCR_INT_NONE         ((LSA_UINT32)0<<8)  //          no interrupt on
#define PNIP_DMACW_SND_DESCR_RSVD1_MSK        0x000000FF              // 07..00:  reserved
    #define PNIP_DMACW_SND_DESCR_RSVD1_SHIFT      0

#define PNIP_DMACW_SND_DESCR_SET_CURRPTR(stru, ptr)   (stru.CurrentDmacwHwPtr = ptr)
#define PNIP_DMACW_SND_DESCR_GET_CURRPTR(stru)        (stru.CurrentDmacwHwPtr)
// i/o as LSB-Value
#define PNIP_DMACW_SND_DESCR_SET_WDT(stru, wdt)       stru.SndWdtErrIntVal &= (~PNIP_DMACW_SND_DESCR_WDT_MSK); stru.SndWdtErrIntVal |= (wdt<<PNIP_DMACW_SND_DESCR_WDT_SHIFT)
#define PNIP_DMACW_SND_DESCR_GET_WDT(stru)            ((stru.SndWdtErrIntVal & PNIP_DMACW_SND_DESCR_WDT_MSK)>>PNIP_DMACW_SND_DESCR_WDT_SHIFT)
#define PNIP_DMACW_SND_DESCR_SET_WDEN_ON(stru)   stru.SndWdtErrIntVal &= (~PNIP_DMACW_SND_DESCR_WDEN_MSK); stru.SndWdtErrIntVal |= (PNIP_DMACW_SND_DESCR_WDEN_ON)
#define PNIP_DMACW_SND_DESCR_SET_WDEN_OFF(stru)  stru.SndWdtErrIntVal &= (~PNIP_DMACW_SND_DESCR_WDEN_MSK); stru.SndWdtErrIntVal |= (PNIP_DMACW_SND_DESCR_WDEN_OFF)
#define PNIP_DMACW_SND_DESCR_GET_WDEN(stru)           ((stru.SndWdtErrIntVal & PNIP_DMACW_SND_DESCR_WDEN_MSK)>>PNIP_DMACW_SND_DESCR_WDEN_SHIFT)
// ErrorStatus
#define PNIP_DMACW_SND_DESCR_CLR_ERR(stru)            (stru.SndWdtErrIntVal &= (~PNIP_DMACW_SND_DESCR_ERR_MSK))
#define PNIP_DMACW_SND_DESCR_GET_ERR(stru)            (stru.SndWdtErrIntVal & PNIP_DMACW_SND_DESCR_ERR_MSK)
// Interrupt Selector
#define PNIP_DMACW_SND_DESCR_SET_INT_ICU1(stru)       stru.SndWdtErrIntVal &= (~PNIP_DMACW_SND_DESCR_INT_MSK); stru.SndWdtErrIntVal |= PNIP_DMACW_SND_DESCR_INT_ICU1
#define PNIP_DMACW_SND_DESCR_SET_INT_ICU2(stru)       stru.SndWdtErrIntVal &= (~PNIP_DMACW_SND_DESCR_INT_MSK); stru.SndWdtErrIntVal |= PNIP_DMACW_SND_DESCR_INT_ICU2
#define PNIP_DMACW_SND_DESCR_SET_INT_ICU3(stru)       stru.SndWdtErrIntVal &= (~PNIP_DMACW_SND_DESCR_INT_MSK); stru.SndWdtErrIntVal |= PNIP_DMACW_SND_DESCR_INT_ICU3
#define PNIP_DMACW_SND_DESCR_GET_INT(stru)            (stru.SndWdtErrIntVal & PNIP_DMACW_SND_DESCR_INT_MSK)

// default init.-value for CFG1 --> owner=SW
#define PNIP_DMACW_SND_DESCR_CFG_OFF                 (   PNIP_DMACW_SND_DESCR_WDT_OFF           |  /* watchdog off                                     */ \
                                                         PNIP_DMACW_SND_DESCR_ERR_OK            |  /* clr error code                                   */ \
                                                         PNIP_DMACW_SND_DESCR_INT_NONE             /* no interrupt on                                  */ \
                                                     )


/*---------------------------------------------------------------------------*/
/* Snd-DMACW structure depends on HW (PNIP-ASIC)                             */
/*---------------------------------------------------------------------------*/

/*
Layout of PNIP_DMACW_SND_ITEM_TYPE:
===================================
Bits:   31                  23                  15                  7               0
n   :   O,RES, RTH, LD, SR, CRM, --, SPP, PP,   DRM, DestSel,      FrameLength
n+1 :                                   NextDMAWC
n+2 :                                   TimeStamp
n+3 :                                 FrameBufferPtr
n+4 :   ---, SMP, PM,       -,SPF,P,SRT,Res-,   SF,Fr,SR,Re,ST,Traf,SUT,Use ,-,SFH,FH
           ,    ,   ,        ,   ,F,   ,Type,   R ,ag,D ,d ,C ,clas,   ,Type, ,
*/

typedef struct _PNIP_DMACW_SND_ITEM_TYPE    EDDP_LOCAL_MEM_ATTR *   PNIP_LOCAL_DMACW_SND_ITEM_PTR_TYPE;
PNIO_PACKED_ATTRIBUTE_PRE struct _PNIP_DMACW_SND_ITEM_TYPE
{
    LSA_UINT32      SndCfg1;                                        //     31: 0:       Owner-Bit
                                                                    // 30..28: RES:     ResultStatus
                                                                    // 27..26: RTH:     RT-Handling
                                                                    //     25: LD:      Learning Disable
                                                                    //     24: SR:      SendResponse
                                                                    //     23: CRM:     Classification-Raw-Mode
                                                                    // 20..19: SPP:     SelectPhasePrio
                                                                    // 18..16: PP:      Phase and Prio
                                                                    //     15: DRM:     Destination-Raw-Mode
                                                                    // 14..11: DestSel: Destination_select (TX-Port)
                                                                    // 10..00: FrameLength

    LSA_UINT32      NextDMACWPtr;                                   // 31..00: NextDMACWPtr     (4byte aligned)
    LSA_UINT32      TimeStamp;                                      // 31..00: TimeStamp        transmit time if SPP="11" + PP="000"
    LSA_UINT32      FrameBufferPtr;                                 // 31..00: FrameBufferPtr   (telegramm buffer)
    
    LSA_UINT32      SndCfg2;                                        //     25: SPM:     SelectPriorityMapping
                                                                    //     24: PM:      PriorityMapping
                                                                    //     22: SPF:     SelectPackFrame
                                                                    //     21: PF:      PackFrame
                                                                    //     20: SRT:     SelectRessourceType
                                                                    // 19..16: ResType: ResourceType
                                                                    //     15: SFR:     SelectFragmentation
                                                                    //     14: Frag:    Fragmentation
                                                                    //     13: SRD:     SelectRedundancy
                                                                    //     12: RED:     Redundancy
                                                                    //     11: STC:     SelectTrafficClass
                                                                    // 10..08: TrafClas:TrafficClass
                                                                    //     07: SUT:     SelectUseType
                                                                    // 06..04: UseType: rule for UseType
                                                                    //     02: SFH:     SelectFrameHandling
                                                                    // 01..00: FH:      FrameHandling
    // --> 
    EDD_COMMON_MEM_U32_PTR_TYPE     pTxRqb;                         // Not part of ASIC !!! EDD_UPPER_RQB_PTR_TYPE, special pointer to store RQB pointer
    EDD_COMMON_MEM_U32_PTR_TYPE     pNrtHDB;                        // Not part of ASIC !!! EDDP_LOCAL_HDB_NRT_PTR_TYPE, special pointer to store NrtHDB pointer
    // <--
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _PNIP_DMACW_SND_ITEM_TYPE    PNIP_DMACW_SND_ITEM_TYPE;

#define PNIP_DMACW_SND_ITEM_SET_CFG1(stru, cfg)      (stru.SndCfg1 = cfg)
#define PNIP_DMACW_SND_ITEM_GET_CFG1(stru)           (stru.SndCfg1)

#define PNIP_DMACW_SND_ITEM_SET_NEXTPTR(stru, next)  (stru.NextDMACWPtr = next)
#define PNIP_DMACW_SND_ITEM_GET_NEXTPTR(stru)        (stru.NextDMACWPtr)

#define PNIP_DMACW_SND_ITEM_SET_TS(stru, ts)         (stru.TimeStamp = ts)
#define PNIP_DMACW_SND_ITEM_GET_TS(stru)             (stru.TimeStamp)

#define PNIP_DMACW_SND_ITEM_SET_TXBUF(stru, buf)     (stru.FrameBufferPtr = buf)
#define PNIP_DMACW_SND_ITEM_GET_TXBUF(stru)          (stru.FrameBufferPtr)

#define PNIP_DMACW_SND_ITEM_SET_CFG2(stru, cfg)      (stru.SndCfg2 = cfg)
#define PNIP_DMACW_SND_ITEM_GET_CFG2(stru)           (stru.SndCfg2)

#define PNIP_DMACW_SND_ITEM_SET_TXRQB(stru, rqb)     (stru.pTxRqb = rqb)
#define PNIP_DMACW_SND_ITEM_GET_TXRQB(stru)          (stru.pTxRqb)
#define PNIP_DMACW_SND_ITEM_SET_NRTHDB(stru, hdb)    (stru.pNrtHDB = hdb)
#define PNIP_DMACW_SND_ITEM_GET_NRTHDB(stru)         (stru.pNrtHDB)


// PNIP_DMACW_SND_ITEM_TYPE.SndCfg1
#define PNIP_DMACW_SND_ITEM_CFG1_OWN_MSK             0x80000000               // 31:      0:       Owner-Bit
    #define PNIP_DMACW_SND_ITEM_CFG1_OWN_SHIFT           31                   //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG1_OWN_HW              ((LSA_UINT32)0<<31)  //          0:PN-IP (HW)
    #define PNIP_DMACW_SND_ITEM_CFG1_OWN_SW              ((LSA_UINT32)1<<31)  //          1:Software
#define PNIP_DMACW_SND_ITEM_CFG1_RESULT_MSK          0x70000000               // 30..28:  RES:     ResultStatus
    #define PNIP_DMACW_SND_ITEM_CFG1_RESULT_SHIFT        28                   //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG1_RESULT_OK           ((LSA_UINT32)0<<28)  //          000: transfer ok
    #define PNIP_DMACW_SND_ITEM_CFG1_RESULT_OK_WARN      ((LSA_UINT32)1<<28)  //          001: ok but NextDMACWNotAligned
    #define PNIP_DMACW_SND_ITEM_CFG1_RESULT_ERR_PORT     ((LSA_UINT32)2<<28)  //          010: SndWrongPort
    #define PNIP_DMACW_SND_ITEM_CFG1_RESULT_ERR_RESP     ((LSA_UINT32)3<<28)  //          011: SndResponseErr
    #define PNIP_DMACW_SND_ITEM_CFG1_RESULT_ERR_WDT      ((LSA_UINT32)4<<28)  //          100: SndWatchdogExpired
    #define PNIP_DMACW_SND_ITEM_CFG1_RESULT_ERR_TX_LEN   ((LSA_UINT32)5<<28)  //          101: SndLenghtErr
    #define PNIP_DMACW_SND_ITEM_CFG1_RESULT_ERR_ALIGN    ((LSA_UINT32)6<<28)  //          110: FrameBufferNotAligned
    #define PNIP_DMACW_SND_ITEM_CFG1_RESULT_ERR_AHB_READ ((LSA_UINT32)7<<28)  //          111: AHBReadErr
#define PNIP_DMACW_SND_ITEM_CFG1_RTH_MSK             0x0C000000               // 27..26:  RTH:     RT-Handling
    #define PNIP_DMACW_SND_ITEM_CFG1_RTH_SHIFT           26                   //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG1_RTH_NO_RT           ((LSA_UINT32)0<<26)  //          0?: no RT-/RToverUDP-Frame
    #define PNIP_DMACW_SND_ITEM_CFG1_RTH_NO_RT0          ((LSA_UINT32)0<<26)  //          0x: no RT-/RToverUDP-Frame
    #define PNIP_DMACW_SND_ITEM_CFG1_RTH_NO_RT1          ((LSA_UINT32)1<<26)  //          0x: 
    #define PNIP_DMACW_SND_ITEM_CFG1_RTH_RT              ((LSA_UINT32)2<<26)  //          10: RT-Frame
    #define PNIP_DMACW_SND_ITEM_CFG1_RTH_RT_UDP          ((LSA_UINT32)3<<26)  //          11: RToverUDP-Frame
#define PNIP_DMACW_SND_ITEM_CFG1_LD_MSK              0x02000000               // 25:      LD:      Learning Disable
    #define PNIP_DMACW_SND_ITEM_CFG1_LD_SHIFT            25                   //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG1_LD_LERN_ON          ((LSA_UINT32)0<<25)  //          0: Learning enabled
    #define PNIP_DMACW_SND_ITEM_CFG1_LD_LERN_OFF         ((LSA_UINT32)1<<25)  //          1: Learning disabled
#define PNIP_DMACW_SND_ITEM_CFG1_SR_MSK              0x01000000               // 24:      SR:      SendResponse
    #define PNIP_DMACW_SND_ITEM_CFG1_SR_SHIFT            24                   //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG1_SR_OFF              ((LSA_UINT32)0<<24)  //          0: no response
    #define PNIP_DMACW_SND_ITEM_CFG1_SR_ON               ((LSA_UINT32)1<<24)  //          1: need MAC response
#define PNIP_DMACW_SND_ITEM_CFG1_CRM_MSK             0x00800000               // 23:      CRM:     Classification-Raw-Mode
    #define PNIP_DMACW_SND_ITEM_CFG1_CRM_SHIFT           23                   //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG1_CRM_AUTO            ((LSA_UINT32)0<<23)  //          0: use results from frame classificaton
    #define PNIP_DMACW_SND_ITEM_CFG1_CRM_FIELD           ((LSA_UINT32)1<<23)  //          1: use settings from DMACW-field(s)
#define PNIP_DMACW_SND_ITEM_CFG1_RSVD1_MSK           0x00400000               // 22:      reserved
    #define PNIP_DMACW_SND_ITEM_CFG1_RSVD1_SHIFT         22                   //          Shift Value
#define PNIP_DMACW_SND_ITEM_CFG1_WDR_MSK             0x00200000               // 21:      WatchdogReason
    #define PNIP_DMACW_SND_ITEM_CFG1_WDR_SHIFT           21                   //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG1_WDR_BARRIER          ((LSA_UINT32)0<<21) //          0: because of prio/traffic limit 
    #define PNIP_DMACW_SND_ITEM_CFG1_WDR_TX_QCW           ((LSA_UINT32)1<<21) //          1: missing resources
#define PNIP_DMACW_SND_ITEM_CFG1_SPP_MSK             0x00180000               // 20-19:   SPP:     SelectPhasePrio
    #define PNIP_DMACW_SND_ITEM_CFG1_SPP_SHIFT           19                   //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG1_SPP_NONE            ((LSA_UINT32)0<<19)  //          00: no rule
    #define PNIP_DMACW_SND_ITEM_CFG1_SPP_PRIO            ((LSA_UINT32)1<<19)  //          01: Prio
    #define PNIP_DMACW_SND_ITEM_CFG1_SPP_SLOT            ((LSA_UINT32)2<<19)  //          10: Phase/Time
    #define PNIP_DMACW_SND_ITEM_CFG1_SPP_EXACT           ((LSA_UINT32)3<<19)  //          11: exact Phase/Time
#define PNIP_DMACW_SND_ITEM_CFG1_PP_MSK              0x00070000               // 18-16:   PP:      Phase and Prio
    #define PNIP_DMACW_SND_ITEM_CFG1_PP_SHIFT            16                   //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG1_PP_PRIO_NONE        ((LSA_UINT32)0<<16)  //          SPP="01" --> "00x": no rule
    #define PNIP_DMACW_SND_ITEM_CFG1_PP_PRIO_LOW         ((LSA_UINT32)2<<16)  //          SPP="01" --> "01x": Mngm_low-TrafficClass
    #define PNIP_DMACW_SND_ITEM_CFG1_PP_PRIO_HIGH        ((LSA_UINT32)4<<16)  //          SPP="01" --> "1xx": Mngm_high-TrafficClass
    #define PNIP_DMACW_SND_ITEM_CFG1_PP_SLOT_TIME        ((LSA_UINT32)0<<16)  //          SPP="10" --> "000": use field "TimeStamp"
    #define PNIP_DMACW_SND_ITEM_CFG1_PP_SLOT_RED         ((LSA_UINT32)1<<16)  //          SPP="10" --> "xx1": send in red phase
    #define PNIP_DMACW_SND_ITEM_CFG1_PP_SLOT_ORA         ((LSA_UINT32)2<<16)  //          SPP="10" --> "x1x": send in orange phase
    #define PNIP_DMACW_SND_ITEM_CFG1_PP_SLOT_GRE         ((LSA_UINT32)4<<16)  //          SPP="10" --> "1xx": send in green phase
    #define PNIP_DMACW_SND_ITEM_CFG1_PP_SLOT_ALL         ((LSA_UINT32)7<<16)  //          SPP="10" --> "111": send in red, orange or green phase
    #define PNIP_DMACW_SND_ITEM_CFG1_PP_EXACT_TIME       ((LSA_UINT32)0<<16)  //          SPP="11" --> "000": use field "TimeStamp"
    #define PNIP_DMACW_SND_ITEM_CFG1_PP_EXACT_RED        ((LSA_UINT32)1<<16)  //          SPP="11" --> "xx1": send in red phase
    #define PNIP_DMACW_SND_ITEM_CFG1_PP_EXACT_ORA        ((LSA_UINT32)2<<16)  //          SPP="11" --> "x1x": send in orange phase
    #define PNIP_DMACW_SND_ITEM_CFG1_PP_EXACT_GRE        ((LSA_UINT32)4<<16)  //          SPP="11" --> "1xx": send in green phase
    #define PNIP_DMACW_SND_ITEM_CFG1_PP_EXACT_ALL        ((LSA_UINT32)7<<16)  //          SPP="11" --> "111": send in red, orange or green phase
#define PNIP_DMACW_SND_ITEM_CFG1_DRM_MSK             0x00008000               // 15:      DRM:     Destination-Raw-Mode
    #define PNIP_DMACW_SND_ITEM_CFG1_DRM_SHIFT           20                   //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG1_DRM_OFF             ((LSA_UINT32)0<<15)  //          0: use DestSel or auto select if DestSel=0
    #define PNIP_DMACW_SND_ITEM_CFG1_DRM_ON              ((LSA_UINT32)1<<15)  //          1: use DestSel or reject frame if DestSel=0
#define PNIP_DMACW_SND_ITEM_CFG1_DSTSEL_MSK          0x00007800               // 14..11:  DestSel: Destination_select (TX-Port)
    #define PNIP_DMACW_SND_ITEM_CFG1_DSTSEL_SHIFT        11                   //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG1_DSTSEL_NONE         ((LSA_UINT32)0<<11)  //          0000 : no port for auto with DRM=0
    #define PNIP_DMACW_SND_ITEM_CFG1_DSTSEL_P1           ((LSA_UINT32)1<<11)  //          xxx1 : Port 1
    #define PNIP_DMACW_SND_ITEM_CFG1_DSTSEL_P2           ((LSA_UINT32)2<<11)  //          xx1x : Port 2
    #define PNIP_DMACW_SND_ITEM_CFG1_DSTSEL_P3           ((LSA_UINT32)4<<11)  //          x1xx : Port 3 
    #define PNIP_DMACW_SND_ITEM_CFG1_DSTSEL_P4           ((LSA_UINT32)8<<11)  //          1xxx : Port 4
    #define PNIP_DMACW_SND_ITEM_CFG1_DSTSEL_P1234        ((LSA_UINT32)0xF<<11)//          1111 : Port 1+2+3+4
#define PNIP_DMACW_SND_ITEM_CFG1_FRMLEN_MSK          0x000007FF               // 10-0:    FrameLength in Bytes
    #define PNIP_DMACW_SND_ITEM_CFG1_FRMLEN_SHIFT        0                    //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG1_FRMLEN_ZERO         0                    //          default init value
    #define PNIP_DMACW_SND_ITEM_CFG1_FRMLEN_MIN          24                   //          minimum 24 byte
    #define PNIP_DMACW_SND_ITEM_CFG1_FRMLEN_MAX          1532                 //          maximum 1532 byte (without CRC)

// PNIP_DMACW_SND_ITEM_TYPE.SndCfg2
#define PNIP_DMACW_SND_ITEM_CFG2_RSVD3_MSK           0xFC000000               // 31..26:  reserved, set 0
    #define PNIP_DMACW_SND_ITEM_CFG2_RSVD3_SHIFT         26                   //          Shift Value
#define PNIP_DMACW_SND_ITEM_CFG2_SPM_MSK             0x02000000               //     25:  SPM:     SelectPriorityMapping
    #define PNIP_DMACW_SND_ITEM_CFG2_SPM_SHIFT           25                   //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG2_SPM_AUTO            ((LSA_UINT32)0<<25)  //          0: use result of frame classification 
    #define PNIP_DMACW_SND_ITEM_CFG2_SPM_FIELD           ((LSA_UINT32)1<<25)  //          1: use field "PriorityMapping"
#define PNIP_DMACW_SND_ITEM_CFG2_PM_MSK              0x01000000               //     24:  PM:      PriorityMapping
    #define PNIP_DMACW_SND_ITEM_CFG2_PM_SHIFT            24                   //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG2_PM_PRIOMAP_A        ((LSA_UINT32)0<<24)  //          0: IEEE_PriorityMapping_A
    #define PNIP_DMACW_SND_ITEM_CFG2_PM_PRIOMAP_B        ((LSA_UINT32)1<<24)  //          1: IEEE_PriorityMapping_B
#define PNIP_DMACW_SND_ITEM_CFG2_RSVD2_MSK           0x00800000               //     23:  reserved, set 0
    #define PNIP_DMACW_SND_ITEM_CFG2_RSVD2_SHIFT         23                   //          Shift Value
#define PNIP_DMACW_SND_ITEM_CFG2_SPF_MSK             0x00400000               //     22:  SPF:     SelectPackFrame
    #define PNIP_DMACW_SND_ITEM_CFG2_SPF_SHIFT           22                   //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG2_SPF_NONE            ((LSA_UINT32)0<<22)  //          0: do not use the field setting  
    #define PNIP_DMACW_SND_ITEM_CFG2_SPF_FIELD           ((LSA_UINT32)1<<22)  //          1: use field "PackFrame"
#define PNIP_DMACW_SND_ITEM_CFG2_PF_MSK              0x00200000               //     21:  PF:      PackFrame
    #define PNIP_DMACW_SND_ITEM_CFG2_PF_SHIFT            21                   //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG2_PF_OFF              ((LSA_UINT32)0<<21)  //          0: -
    #define PNIP_DMACW_SND_ITEM_CFG2_PF_ON               ((LSA_UINT32)1<<21)  //          1: OutBound-Pack Frame
#define PNIP_DMACW_SND_ITEM_CFG2_SRT_MSK             0x00100000               //     20:  SRT:     SelectRessourceType
    #define PNIP_DMACW_SND_ITEM_CFG2_SRT_SHIFT           20                   //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG2_SRT_AUTO            ((LSA_UINT32)0<<20)  //          0: use result of frame classification  
    #define PNIP_DMACW_SND_ITEM_CFG2_SRT_FIELD           ((LSA_UINT32)1<<20)  //          1: use field "RescourceType"
#define PNIP_DMACW_SND_ITEM_CFG2_RESTYPE_MSK         0x000F0000               // 19..16:  ResType: ResourceType
    #define PNIP_DMACW_SND_ITEM_CFG2_RESTYPE_SHIFT       16                   //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG2_RESTYPE_DEFPOOL     ((LSA_UINT32)0<<16)  //          0: def. pool  
    #define PNIP_DMACW_SND_ITEM_CFG2_RESTYPE_POOL(x)     (((LSA_UINT32)x&15)<<16)//       1-15 specific pool  
#define PNIP_DMACW_SND_ITEM_CFG2_SFR_MSK             0x00008000               //     15:  SFR:     SelectFragmentation
    #define PNIP_DMACW_SND_ITEM_CFG2_SFR_SHIFT           15                   //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG2_SFR_AUTO            ((LSA_UINT32)0<<15)  //          0: use result of frame classification  
    #define PNIP_DMACW_SND_ITEM_CFG2_SFR_FIELD           ((LSA_UINT32)1<<15)  //          1: use field "Fragmentation"
#define PNIP_DMACW_SND_ITEM_CFG2_FRAG_MSK            0x00004000               //     14:  Frag:    Fragmentation
    #define PNIP_DMACW_SND_ITEM_CFG2_FRAG_SHIFT          14                   //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG2_FRAG_OFF            ((LSA_UINT32)0<<14)  //          0: -
    #define PNIP_DMACW_SND_ITEM_CFG2_FRAG_ON             ((LSA_UINT32)1<<14)  //          1: on if Port_Control_Px.TxD_Fragmentation = ‘1’
#define PNIP_DMACW_SND_ITEM_CFG2_SRD_MSK             0x00002000               //     13:  SRD:     SelectRedundancy
    #define PNIP_DMACW_SND_ITEM_CFG2_SRD_SHIFT           13                   //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG2_SRD_AUTO            ((LSA_UINT32)0<<13)  //          0: use result of frame classification 
    #define PNIP_DMACW_SND_ITEM_CFG2_SRD_FIELD           ((LSA_UINT32)1<<13)  //          1: use field "Redundancy"
#define PNIP_DMACW_SND_ITEM_CFG2_RED_MSK             0x00001000               //     12:  RED:     Redundancy
    #define PNIP_DMACW_SND_ITEM_CFG2_RED_SHIFT           12                   //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG2_RED_OFF             ((LSA_UINT32)0<<12)  //          0: -
    #define PNIP_DMACW_SND_ITEM_CFG2_RED_ON              ((LSA_UINT32)1<<12)  //          1: use Redundancy machanism
#define PNIP_DMACW_SND_ITEM_CFG2_STC_MSK             0x00000800               //     11:  STC:     SelectTrafficClass
    #define PNIP_DMACW_SND_ITEM_CFG2_STC_SHIFT           11                   //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG2_STC_AUTO            ((LSA_UINT32)0<<11)  //          0: use result of frame classification
    #define PNIP_DMACW_SND_ITEM_CFG2_STC_FIELD           ((LSA_UINT32)1<<11)  //          1: use field "TrafficClass"  
#define PNIP_DMACW_SND_ITEM_CFG2_TRAFCLASS_MSK       0x00000700               // 10..08:  TrafClas:TrafficClass
    #define PNIP_DMACW_SND_ITEM_CFG2_TRAFCLASS_SHIFT     8                    //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG2_TRAFCLASS_SEL(x)    (((LSA_UINT32)x&7)<<8)//         0-7: Groups: a) 0-4,7  b) 5,6  c) 5,6
#define PNIP_DMACW_SND_ITEM_CFG2_SUT_MSK             0x00000080               //     07:  SUT:     SelectUseType
    #define PNIP_DMACW_SND_ITEM_CFG2_SUT_SHIFT           7                    //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG2_SUT_AUTO            ((LSA_UINT32)0<<7)   //          0: use result of frame classification
    #define PNIP_DMACW_SND_ITEM_CFG2_SUT_FIELD           ((LSA_UINT32)1<<7)   //          1: use field "UseType"
#define PNIP_DMACW_SND_ITEM_CFG2_USETYPE_MSK         0x00000070               // 06..04:  UseType: rule for UseType
    #define PNIP_DMACW_SND_ITEM_CFG2_USETYPE_SHIFT       4                    //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG2_USETYPE_STD         ((LSA_UINT32)0<<4)   //          000: Std
    #define PNIP_DMACW_SND_ITEM_CFG2_USETYPE_PTCP_CLOCK  ((LSA_UINT32)1<<4)   //          001: PTCP Clock
    #define PNIP_DMACW_SND_ITEM_CFG2_USETYPE_PTCP_TIME   ((LSA_UINT32)2<<4)   //          010: PTCP Time
    #define PNIP_DMACW_SND_ITEM_CFG2_USETYPE_PTCP_DELAY  ((LSA_UINT32)3<<4)   //          011: PTCP Delay
    #define PNIP_DMACW_SND_ITEM_CFG2_USETYPE_FUP_CLK     ((LSA_UINT32)4<<4)   //          100: FUP Clock
    #define PNIP_DMACW_SND_ITEM_CFG2_USETYPE_FUP_TIME    ((LSA_UINT32)5<<4)   //          101: FUP Time
    #define PNIP_DMACW_SND_ITEM_CFG2_USETYPE_PTP         ((LSA_UINT32)6<<4)   //          110: PTP
    #define PNIP_DMACW_SND_ITEM_CFG2_USETYPE_PTP_DELAY   ((LSA_UINT32)7<<4)   //          111: PTP Delay
#define PNIP_DMACW_SND_ITEM_CFG2_RSVD1_MSK           0x00000008               //     03:  reserved, set 0
    #define PNIP_DMACW_SND_ITEM_CFG2_RSVD1_SHIFT         3                    //          Shift Value
#define PNIP_DMACW_SND_ITEM_CFG2_SFH_MSK             0x00000004               //     02:  SFH:     SelectFrameHandling
    #define PNIP_DMACW_SND_ITEM_CFG2_SFH_SHIFT           2                    //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG2_SFH_AUTO            ((LSA_UINT32)0<<2)   //          0: use result of frame classification
    #define PNIP_DMACW_SND_ITEM_CFG2_SFH_FIELD           ((LSA_UINT32)1<<2)   //          1: use field "FH"
#define PNIP_DMACW_SND_ITEM_CFG2_FH_MSK              0x00000003               // 01..00:  FH:      FrameHandling
    #define PNIP_DMACW_SND_ITEM_CFG2_FH_SHIFT            0                    //          Shift Value
    #define PNIP_DMACW_SND_ITEM_CFG2_FH_NRT              ((LSA_UINT32)0<<0)   //          00: NRT
    #define PNIP_DMACW_SND_ITEM_CFG2_FH_RTA              ((LSA_UINT32)1<<0)   //          01: RTA         TrafficMode=0+1
    #define PNIP_DMACW_SND_ITEM_CFG2_FH_RT_LOW           ((LSA_UINT32)2<<0)   //          10: RT low      TrafficMode=0+1
    #define PNIP_DMACW_SND_ITEM_CFG2_FH_RT_HIGH          ((LSA_UINT32)3<<0)   //          11: RT high
    #define PNIP_DMACW_SND_ITEM_CFG2_FH_RT               ((LSA_UINT32)3<<0)   //          1x: RT          1x: TrafficMode=2

// default init.-value for CFG1 --> PNIP_DMACW_SND_ITEM_SET_CFG1(stru, PNIP_DMACW_SND_ITEM_CFG1_DEF_HW);
#define PNIP_DMACW_SND_ITEM_CFG1_DEF_HW              (      PNIP_DMACW_SND_ITEM_CFG1_OWN_HW       |  /* Owner = HW                                       */ \
                                                            PNIP_DMACW_SND_ITEM_CFG1_RESULT_OK    |  /* clr result code                                  */ \
                                                            PNIP_DMACW_SND_ITEM_CFG1_RTH_NO_RT    |  /* no RT-/RToverUDP-Frame                           */ \
                                                            PNIP_DMACW_SND_ITEM_CFG1_LD_LERN_OFF  |  /* Learning disabled                                */ \
                                                            PNIP_DMACW_SND_ITEM_CFG1_SR_OFF       |  /* no response (from MAC/Port)                      */ \
                                                            PNIP_DMACW_SND_ITEM_CFG1_CRM_AUTO     |  /* use results from frame classificaton             */ \
                                                            PNIP_DMACW_SND_ITEM_CFG1_SPP_NONE     |  /* no rule for SelectPhasePrio                      */ \
                                                            PNIP_DMACW_SND_ITEM_CFG1_PP_PRIO_NONE |  /* no rule for Phase and Prio                       */ \
                                                            PNIP_DMACW_SND_ITEM_CFG1_DRM_OFF      |  /* Destination-Raw-Mode with auto search TX-Port    */ \
                                                            PNIP_DMACW_SND_ITEM_CFG1_DSTSEL_NONE  |  /* -                                                */ \
                                                            PNIP_DMACW_SND_ITEM_CFG1_FRMLEN_ZERO     /* no TX length in default init.-value              */ \
                                                        )
// default init.-value for CFG1 --> owner=SW
#define PNIP_DMACW_SND_ITEM_CFG1_DEF_SW              (      PNIP_DMACW_SND_ITEM_CFG1_OWN_SW       |  /* Owner = SW                                       */ \
                                                            PNIP_DMACW_SND_ITEM_CFG1_RESULT_OK    |  /* clr result code                                  */ \
                                                            PNIP_DMACW_SND_ITEM_CFG1_RTH_NO_RT    |  /* no RT-/RToverUDP-Frame                           */ \
                                                            PNIP_DMACW_SND_ITEM_CFG1_LD_LERN_OFF  |  /* Learning disabled                                */ \
                                                            PNIP_DMACW_SND_ITEM_CFG1_SR_OFF       |  /* no response (from MAC/Port)                      */ \
                                                            PNIP_DMACW_SND_ITEM_CFG1_CRM_AUTO     |  /* use results from frame classificaton             */ \
                                                            PNIP_DMACW_SND_ITEM_CFG1_SPP_NONE     |  /* no rule for SelectPhasePrio                      */ \
                                                            PNIP_DMACW_SND_ITEM_CFG1_PP_PRIO_NONE |  /* no rule for Phase and Prio                       */ \
                                                            PNIP_DMACW_SND_ITEM_CFG1_DRM_OFF      |  /* Destination-Raw-Mode with auto search TX-Port    */ \
                                                            PNIP_DMACW_SND_ITEM_CFG1_DSTSEL_NONE  |  /* -                                                */ \
                                                            PNIP_DMACW_SND_ITEM_CFG1_FRMLEN_ZERO     /* no TX length in default init.-value              */ \
                                                        )
// 0:       Owner-Bit           0:HW, 1:SW
#define PNIP_DMACW_SND_ITEM_SET_OWNER_HW(stru)       (stru.SndCfg1 &= (~PNIP_DMACW_SND_ITEM_CFG1_OWN_MSK))
#define PNIP_DMACW_SND_ITEM_SET_OWNER_SW(stru)       (stru.SndCfg1 |= PNIP_DMACW_SND_ITEM_CFG1_OWN_SW)
#define PNIP_DMACW_SND_ITEM_GET_OWNER(stru)          (stru.SndCfg1 & PNIP_DMACW_SND_ITEM_CFG1_OWN_MSK)
// RES:     ResultStatus
#define PNIP_DMACW_SND_ITEM_CLR_RESULT(stru)         (stru.SndCfg1 &= (~PNIP_DMACW_SND_ITEM_CFG1_RESULT_MSK))
#define PNIP_DMACW_SND_ITEM_GET_RESULT(stru)         (stru.SndCfg1 & PNIP_DMACW_SND_ITEM_CFG1_RESULT_MSK)
// RTH:     RT-Handling         0x:none, 10:RT, 11:RT_UDP
#define PNIP_DMACW_SND_ITEM_SET_RTH_NONE(stru)       (stru.SndCfg1 &= (~PNIP_DMACW_SND_ITEM_CFG1_RTH_MSK))
#define PNIP_DMACW_SND_ITEM_SET_RTH_RT(stru)         stru.SndCfg1 &= (~PNIP_DMACW_SND_ITEM_CFG1_RTH_MSK); stru.SndCfg1 |= PNIP_DMACW_SND_ITEM_CFG1_RTH_RT
#define PNIP_DMACW_SND_ITEM_SET_RTH_RT_UDP(stru)     (stru.SndCfg1 |= PNIP_DMACW_SND_ITEM_CFG1_RTH_RT_UDP)
#define PNIP_DMACW_SND_ITEM_GET_RTH(stru)            (stru.SndCfg1 & PNIP_DMACW_SND_ITEM_CFG1_RTH_MSK)
// LD:      Learning Disable     0:Learning enabled, 1:Learning disabled
#define PNIP_DMACW_SND_ITEM_SET_LD_LERN_ON(stru)     (stru.SndCfg1 &= (~PNIP_DMACW_SND_ITEM_CFG1_LD_MSK))
#define PNIP_DMACW_SND_ITEM_SET_LD_LERN_OFF(stru)    (stru.SndCfg1 |= PNIP_DMACW_SND_ITEM_CFG1_LD_LERN_OFF)
#define PNIP_DMACW_SND_ITEM_GET_LD(stru)             (stru.SndCfg1 & PNIP_DMACW_SND_ITEM_CFG1_LD_MSK)
// SR:      SendResponse
#define PNIP_DMACW_SND_ITEM_SET_SR_OFF(stru)         (stru.SndCfg1 &= (~PNIP_DMACW_SND_ITEM_CFG1_SR_MSK))
#define PNIP_DMACW_SND_ITEM_SET_SR_ON(stru)          (stru.SndCfg1 |= PNIP_DMACW_SND_ITEM_CFG1_SR_ON)
#define PNIP_DMACW_SND_ITEM_GET_SR(stru)             (stru.SndCfg1 & PNIP_DMACW_SND_ITEM_CFG1_SR_MSK)
// CRM:     Classification-Raw-Mode 0:auto, 1:field
#define PNIP_DMACW_SND_ITEM_SET_CRM_AUTO(stru)       (stru.SndCfg1 &= (~PNIP_DMACW_SND_ITEM_CFG1_CRM_MSK))
#define PNIP_DMACW_SND_ITEM_SET_CRM_FIELD(stru)      (stru.SndCfg1 |= PNIP_DMACW_SND_ITEM_CFG1_CRM_FIELD)
#define PNIP_DMACW_SND_ITEM_GET_CRM(stru)            (stru.SndCfg1 & PNIP_DMACW_SND_ITEM_CFG1_CRM_MSK)
// WDR:     WatchdogReason 0:prio/traffic limit, 1:missing resources
#define PNIP_DMACW_SND_ITEM_GET_WDR(stru)            (stru.SndCfg1 & PNIP_DMACW_SND_ITEM_CFG1_WDR_MSK)
#define PNIP_DMACW_SND_ITEM_IS_WDR_BARRIER(stru)     ((stru.SndCfg1 & PNIP_DMACW_SND_ITEM_CFG1_WDR_MSK) == PNIP_DMACW_SND_ITEM_CFG1_WDR_BARRIER)
#define PNIP_DMACW_SND_ITEM_IS_WDR_TX_QCW(stru)      ((stru.SndCfg1 & PNIP_DMACW_SND_ITEM_CFG1_WDR_MSK) == PNIP_DMACW_SND_ITEM_CFG1_WDR_TX_QCW)
// SPP:     SelectPhasePrio 00:none, 01:prio, 10:slot, 11:exact
#define PNIP_DMACW_SND_ITEM_SET_SPP_NONE(stru)       (stru.SndCfg1 &= (~PNIP_DMACW_SND_ITEM_CFG1_SPP_MSK))
#define PNIP_DMACW_SND_ITEM_SET_SPP_PRIO(stru)       stru.SndCfg1 &= (~PNIP_DMACW_SND_ITEM_CFG1_SPP_MSK); stru.SndCfg1 |= PNIP_DMACW_SND_ITEM_CFG1_SPP_PRIO
#define PNIP_DMACW_SND_ITEM_SET_SPP_SLOT(stru)       stru.SndCfg1 &= (~PNIP_DMACW_SND_ITEM_CFG1_SPP_MSK); stru.SndCfg1 |= PNIP_DMACW_SND_ITEM_CFG1_SPP_SLOT
#define PNIP_DMACW_SND_ITEM_SET_SPP_EXACT(stru)      (stru.SndCfg1 |= PNIP_DMACW_SND_ITEM_CFG1_SPP_EXACT)
#define PNIP_DMACW_SND_ITEM_GET_SPP(stru)            (stru.SndCfg1 & PNIP_DMACW_SND_ITEM_CFG1_SPP_MSK)
// PP:      Phase and Prio  depends on "SPP"
#define PNIP_DMACW_SND_ITEM_SET_PP_PRIO_NONE(stru)   (stru.SndCfg1 &= (~PNIP_DMACW_SND_ITEM_CFG1_PP_MSK))
#define PNIP_DMACW_SND_ITEM_SET_PP_PRIO_LOW(stru)    stru.SndCfg1 &= (~PNIP_DMACW_SND_ITEM_CFG1_PP_MSK); stru.SndCfg1 |= PNIP_DMACW_SND_ITEM_CFG1_PP_PRIO_LOW
#define PNIP_DMACW_SND_ITEM_SET_PP_PRIO_HIGH(stru)   stru.SndCfg1 &= (~PNIP_DMACW_SND_ITEM_CFG1_PP_MSK); stru.SndCfg1 |= PNIP_DMACW_SND_ITEM_CFG1_PP_PRIO_HIGH
#define PNIP_DMACW_SND_ITEM_SET_PP_SLOT_TIME(stru)   (stru.SndCfg1 &= (~PNIP_DMACW_SND_ITEM_CFG1_PP_MSK))
#define PNIP_DMACW_SND_ITEM_SET_PP_SLOT_RED(stru)    stru.SndCfg1 &= (~PNIP_DMACW_SND_ITEM_CFG1_PP_MSK); stru.SndCfg1 |= PNIP_DMACW_SND_ITEM_CFG1_PP_SLOT_RED
#define PNIP_DMACW_SND_ITEM_SET_PP_SLOT_ORA(stru)    stru.SndCfg1 &= (~PNIP_DMACW_SND_ITEM_CFG1_PP_MSK); stru.SndCfg1 |= PNIP_DMACW_SND_ITEM_CFG1_PP_SLOT_ORA
#define PNIP_DMACW_SND_ITEM_SET_PP_SLOT_GRE(stru)    stru.SndCfg1 &= (~PNIP_DMACW_SND_ITEM_CFG1_PP_MSK); stru.SndCfg1 |= PNIP_DMACW_SND_ITEM_CFG1_PP_SLOT_GRE
#define PNIP_DMACW_SND_ITEM_SET_PP_SLOT_ALL(stru)    (stru.SndCfg1 |= PNIP_DMACW_SND_ITEM_CFG1_PP_SLOT_ALL)
#define PNIP_DMACW_SND_ITEM_SET_PP_EXACT_TIME(stru)  (stru.SndCfg1 &= (~PNIP_DMACW_SND_ITEM_CFG1_PP_MSK))
#define PNIP_DMACW_SND_ITEM_SET_PP_EXACT_RED(stru)   stru.SndCfg1 &= (~PNIP_DMACW_SND_ITEM_CFG1_PP_MSK); stru.SndCfg1 |= PNIP_DMACW_SND_ITEM_CFG1_PP_EXACT_RED
#define PNIP_DMACW_SND_ITEM_SET_PP_EXACT_ORA(stru)   stru.SndCfg1 &= (~PNIP_DMACW_SND_ITEM_CFG1_PP_MSK); stru.SndCfg1 |= PNIP_DMACW_SND_ITEM_CFG1_PP_EXACT_ORA
#define PNIP_DMACW_SND_ITEM_SET_PP_EXACT_GRE(stru)   stru.SndCfg1 &= (~PNIP_DMACW_SND_ITEM_CFG1_PP_MSK); stru.SndCfg1 |= PNIP_DMACW_SND_ITEM_CFG1_PP_EXACT_GRE
#define PNIP_DMACW_SND_ITEM_SET_PP_EXACT_ALL(stru)   (stru.SndCfg1 |= PNIP_DMACW_SND_ITEM_CFG1_PP_EXACT_ALL)
#define PNIP_DMACW_SND_ITEM_GET_PP(stru)             (stru.SndCfg1 & PNIP_DMACW_SND_ITEM_CFG1_PP_MSK)
// DRM:     Destination-Raw-Mode    0:off/auto, 1:on
#define PNIP_DMACW_SND_ITEM_SET_DRM_OFF(stru)        (stru.SndCfg1 &= (~PNIP_DMACW_SND_ITEM_CFG1_DRM_MSK))
#define PNIP_DMACW_SND_ITEM_SET_DRM_ON(stru)         (stru.SndCfg1 |= PNIP_DMACW_SND_ITEM_CFG1_DRM_ON)
#define PNIP_DMACW_SND_ITEM_GET_DRM(stru)            (stru.SndCfg1 & PNIP_DMACW_SND_ITEM_CFG1_DRM_MSK)
// DestSel: Destination_select (TX-Port)    xxx1:Port0, .. , 1xxx:Port3
#define PNIP_DMACW_SND_ITEM_SET_DSTSEL(stru, port)   (stru.SndCfg1 &= (~PNIP_DMACW_SND_ITEM_CFG1_DSTSEL_MSK)); stru.SndCfg1 |= (port & PNIP_DMACW_SND_ITEM_CFG1_DSTSEL_MSK)
#define PNIP_DMACW_SND_ITEM_GET_DSTSEL(stru)         (stru.SndCfg1 & PNIP_DMACW_SND_ITEM_CFG1_DSTSEL_MSK)
// FrameLength in Bytes
#define PNIP_DMACW_SND_ITEM_SET_FRMLEN(stru, len)    (stru.SndCfg1 &= (~PNIP_DMACW_SND_ITEM_CFG1_FRMLEN_MSK)); stru.SndCfg1 |= (len & PNIP_DMACW_SND_ITEM_CFG1_FRMLEN_MSK)
#define PNIP_DMACW_SND_ITEM_GET_FRMLEN(stru)         (stru.SndCfg1 & PNIP_DMACW_SND_ITEM_CFG1_FRMLEN_MSK)


// default init.-value for CFG2 --> PNIP_DMACW_SND_ITEM_SET_CFG2(stru, PNIP_DMACW_SND_ITEM_CFG2_DEF);
#define PNIP_DMACW_SND_ITEM_CFG2_DEF                 (      PNIP_DMACW_SND_ITEM_CFG2_SPM_AUTO        |  /* use result of frame classification               */ \
                                                            PNIP_DMACW_SND_ITEM_CFG2_PM_PRIOMAP_A    |  /* PriorityMapping                                  */ \
                                                            PNIP_DMACW_SND_ITEM_CFG2_SPF_NONE        |  /* use result of frame classification               */ \
                                                            PNIP_DMACW_SND_ITEM_CFG2_PF_OFF          |  /* PackFrame                                        */ \
                                                            PNIP_DMACW_SND_ITEM_CFG2_SRT_AUTO        |  /* use result of frame classification               */ \
                                                            PNIP_DMACW_SND_ITEM_CFG2_RESTYPE_DEFPOOL |  /* ResourceType                                     */ \
                                                            PNIP_DMACW_SND_ITEM_CFG2_SFR_AUTO        |  /* use result of frame classification               */ \
                                                            PNIP_DMACW_SND_ITEM_CFG2_FRAG_OFF        |  /* Fragmentation                                    */ \
                                                            PNIP_DMACW_SND_ITEM_CFG2_SRD_AUTO        |  /* use result of frame classification               */ \
                                                            PNIP_DMACW_SND_ITEM_CFG2_RED_OFF         |  /* Redundancy                                       */ \
                                                            PNIP_DMACW_SND_ITEM_CFG2_STC_AUTO        |  /* use result of frame classification               */ \
                                                            PNIP_DMACW_SND_ITEM_CFG2_TRAFCLASS_SEL(0)|  /* TrafficClass                                     */ \
                                                            PNIP_DMACW_SND_ITEM_CFG2_SUT_AUTO        |  /* use result of frame classification               */ \
                                                            PNIP_DMACW_SND_ITEM_CFG2_USETYPE_STD     |  /* UseType                                          */ \
                                                            PNIP_DMACW_SND_ITEM_CFG2_SFH_AUTO        |  /* use result of frame classification               */ \
                                                            PNIP_DMACW_SND_ITEM_CFG2_FH_NRT             /* FrameHandling                                    */ \
                                                        )
// SPM:     SelectPriorityMapping   0:auto, 1:field
#define PNIP_DMACW_SND_ITEM_SET_SPM_AUTO(stru)       (stru.SndCfg2 &= (~PNIP_DMACW_SND_ITEM_CFG2_SPM_MSK))
#define PNIP_DMACW_SND_ITEM_SET_SPM_FIELD(stru)      (stru.SndCfg2 |= PNIP_DMACW_SND_ITEM_CFG2_SPM_FIELD)
#define PNIP_DMACW_SND_ITEM_GET_SPM(stru)            (stru.SndCfg2 & PNIP_DMACW_SND_ITEM_CFG2_SPM_MSK)
// PM:      PriorityMapping         0:MapA, 1:MapB
#define PNIP_DMACW_SND_ITEM_SET_PM_PRIOMAP_A(stru)   (stru.SndCfg2 &= (~PNIP_DMACW_SND_ITEM_CFG2_PM_MSK))
#define PNIP_DMACW_SND_ITEM_SET_PM_PRIOMAP_B(stru)   (stru.SndCfg2 |= PNIP_DMACW_SND_ITEM_CFG2_PM_PRIOMAP_B)
#define PNIP_DMACW_SND_ITEM_GET_PM(stru)             (stru.SndCfg2 & PNIP_DMACW_SND_ITEM_CFG2_PM_MSK)
// SPF:     SelectPackFrame         0:none, 1:field
#define PNIP_DMACW_SND_ITEM_SET_SPF_NONE(stru)       (stru.SndCfg2 &= (~PNIP_DMACW_SND_ITEM_CFG2_SPF_MSK))
#define PNIP_DMACW_SND_ITEM_SET_SPF_FIELD(stru)      (stru.SndCfg2 |= PNIP_DMACW_SND_ITEM_CFG2_SPF_FIELD)
#define PNIP_DMACW_SND_ITEM_GET_SPF(stru)            (stru.SndCfg2 & PNIP_DMACW_SND_ITEM_CFG2_SPF_MSK)
// PF:      PackFrame               0:noOutband, 1:As OutbandPack-Frame
#define PNIP_DMACW_SND_ITEM_SET_PF_OFF(stru)         (stru.SndCfg2 &= (~PNIP_DMACW_SND_ITEM_CFG2_PF_MSK))
#define PNIP_DMACW_SND_ITEM_SET_PF_ON(stru)          (stru.SndCfg2 |= PNIP_DMACW_SND_ITEM_CFG2_PF_ON)
#define PNIP_DMACW_SND_ITEM_GET_PF(stru)             (stru.SndCfg2 & PNIP_DMACW_SND_ITEM_CFG2_PF_MSK)
// SRT:     SelectRessourceType     0:auto, 1:field
#define PNIP_DMACW_SND_ITEM_SET_SRT_AUTO(stru)       (stru.SndCfg2 &= (~PNIP_DMACW_SND_ITEM_CFG2_SRT_MSK))
#define PNIP_DMACW_SND_ITEM_SET_SRT_FIELD(stru)      (stru.SndCfg2 |= PNIP_DMACW_SND_ITEM_CFG2_SRT_FIELD)
#define PNIP_DMACW_SND_ITEM_GET_SRT(stru)            (stru.SndCfg2 & PNIP_DMACW_SND_ITEM_CFG2_SRT_MSK)
// ResType: ResourceType --> PNIP_DMACW_SND_ITEM_SET_RESTYPE(stru, PNIP_DMACW_SND_ITEM_CFG2_RESTYPE_POOL(x))
#define PNIP_DMACW_SND_ITEM_SET_RESTYPE(stru, pool)  stru.SndCfg2 &= (~PNIP_DMACW_SND_ITEM_CFG2_RESTYPE_MSK); stru.SndCfg2 |= (pool&PNIP_DMACW_SND_ITEM_CFG2_RESTYPE_MSK)
#define PNIP_DMACW_SND_ITEM_GET_RESTYPE(stru)        (stru.SndCfg2 & PNIP_DMACW_SND_ITEM_CFG2_RESTYPE_MSK)
// SFR:     SelectFragmentation     0:auto, 1:field
#define PNIP_DMACW_SND_ITEM_SET_SFR_AUTO(stru)       (stru.SndCfg2 &= (~PNIP_DMACW_SND_ITEM_CFG2_SFR_MSK))
#define PNIP_DMACW_SND_ITEM_SET_SFR_FIELD(stru)      (stru.SndCfg2 |= PNIP_DMACW_SND_ITEM_CFG2_SFR_FIELD)
#define PNIP_DMACW_SND_ITEM_GET_SFR(stru)            (stru.SndCfg2 & PNIP_DMACW_SND_ITEM_CFG2_SFR_MSK)
// Frag:    Fragmentation           0:no (active)fragmentation, 1:fragmentation is allowed
#define PNIP_DMACW_SND_ITEM_SET_FRAG_OFF(stru)       (stru.SndCfg2 &= (~PNIP_DMACW_SND_ITEM_CFG2_FRAG_MSK))
#define PNIP_DMACW_SND_ITEM_SET_FRAG_ON(stru)        (stru.SndCfg2 |= PNIP_DMACW_SND_ITEM_CFG2_FRAG_ON)
#define PNIP_DMACW_SND_ITEM_GET_FRAG(stru)           (stru.SndCfg2 & PNIP_DMACW_SND_ITEM_CFG2_FRAG_MSK)
// SRD:     SelectRedundancy        0:auto, 1:field
#define PNIP_DMACW_SND_ITEM_SET_SRD_AUTO(stru)       (stru.SndCfg2 &= (~PNIP_DMACW_SND_ITEM_CFG2_SRD_MSK))
#define PNIP_DMACW_SND_ITEM_SET_SRD_FIELD(stru)      (stru.SndCfg2 |= PNIP_DMACW_SND_ITEM_CFG2_SRD_FIELD)
#define PNIP_DMACW_SND_ITEM_GET_SRD(stru)            (stru.SndCfg2 & PNIP_DMACW_SND_ITEM_CFG2_SRD_MSK)
// RED:     Redundancy              0:no (active)redundancy, 1:redundancy is allowed
#define PNIP_DMACW_SND_ITEM_SET_RED_OFF(stru)        (stru.SndCfg2 &= (~PNIP_DMACW_SND_ITEM_CFG2_RED_MSK))
#define PNIP_DMACW_SND_ITEM_SET_RED_ON(stru)         (stru.SndCfg2 |= PNIP_DMACW_SND_ITEM_CFG2_RED_ON)
#define PNIP_DMACW_SND_ITEM_GET_RED(stru)            (stru.SndCfg2 & PNIP_DMACW_SND_ITEM_CFG2_RED_MSK)
// STC:     SelectTrafficClass      0:auto, 1:field
#define PNIP_DMACW_SND_ITEM_SET_STC_AUTO(stru)       (stru.SndCfg2 &= (~PNIP_DMACW_SND_ITEM_CFG2_STC_MSK))
#define PNIP_DMACW_SND_ITEM_SET_STC_FIELD(stru)      (stru.SndCfg2 |= PNIP_DMACW_SND_ITEM_CFG2_STC_FIELD)
#define PNIP_DMACW_SND_ITEM_GET_STC(stru)            (stru.SndCfg2 & PNIP_DMACW_SND_ITEM_CFG2_STC_MSK)
// ResType: ResourceType --> PNIP_DMACW_SND_ITEM_SET_DSTSEL(stru, PNIP_DMACW_SND_ITEM_CFG2_TRAFCLASS_SEL(x))
#define PNIP_DMACW_SND_ITEM_SET_TRAFCLASS(stru, sel) stru.SndCfg2 &= (~PNIP_DMACW_SND_ITEM_CFG2_TRAFCLASS_MSK); stru.SndCfg2 |= (sel&PNIP_DMACW_SND_ITEM_CFG2_TRAFCLASS_MSK)
#define PNIP_DMACW_SND_ITEM_GET_TRAFCLASS(stru)      (stru.SndCfg2 & PNIP_DMACW_SND_ITEM_CFG2_TRAFCLASS_MSK)
// SUT:     SelectUseType           0:auto, 1:field
#define PNIP_DMACW_SND_ITEM_SET_SUT_AUTO(stru)       (stru.SndCfg2 &= (~PNIP_DMACW_SND_ITEM_CFG2_SUT_MSK))
#define PNIP_DMACW_SND_ITEM_SET_SUT_FIELD(stru)      (stru.SndCfg2 |= PNIP_DMACW_SND_ITEM_CFG2_SUT_FIELD)
#define PNIP_DMACW_SND_ITEM_GET_SUT(stru)            (stru.SndCfg2 & PNIP_DMACW_SND_ITEM_CFG2_SUT_MSK)
// UseType: rule for UseType --> PNIP_DMACW_SND_ITEM_SET_USETYPE(stru, PNIP_DMACW_SND_ITEM_CFG2_USETYPE_???)
#define PNIP_DMACW_SND_ITEM_SET_USETYPE(stru, sel)   stru.SndCfg2 &= (~PNIP_DMACW_SND_ITEM_CFG2_USETYPE_MSK); stru.SndCfg2 |= (sel&PNIP_DMACW_SND_ITEM_CFG2_USETYPE_MSK)
#define PNIP_DMACW_SND_ITEM_GET_USETYPE(stru)        (stru.SndCfg2 & PNIP_DMACW_SND_ITEM_CFG2_USETYPE_MSK)
// SFH:     SelectFrameHandling     0:auto, 1:field
#define PNIP_DMACW_SND_ITEM_SET_SFH_AUTO(stru)       (stru.SndCfg2 &= (~PNIP_DMACW_SND_ITEM_CFG2_SFH_MSK))
#define PNIP_DMACW_SND_ITEM_SET_SFH_FIELD(stru)      (stru.SndCfg2 |= PNIP_DMACW_SND_ITEM_CFG2_SFH_FIELD)
#define PNIP_DMACW_SND_ITEM_GET_SFH(stru)            (stru.SndCfg2 & PNIP_DMACW_SND_ITEM_CFG2_SFH_MSK)
// FH:      FrameHandling --> PNIP_DMACW_SND_ITEM_SET_FH(stru, PNIP_DMACW_SND_ITEM_CFG2_FH_???)
#define PNIP_DMACW_SND_ITEM_SET_FH(stru, sel)        stru.SndCfg2 &= (~PNIP_DMACW_SND_ITEM_CFG2_FH_MSK); stru.SndCfg2 |= (sel&PNIP_DMACW_SND_ITEM_CFG2_FH_MSK)
#define PNIP_DMACW_SND_ITEM_GET_FH(stru)             (stru.SndCfg2 & PNIP_DMACW_SND_ITEM_CFG2_FH_MSK)



/******************************************************************************************************************************/
/** Rcv defines ***************************************************************************************************************/
/**             ***************************************************************************************************************/
/******************************************************************************************************************************/


/*---------------------------------------------------------------------------*/
/* RcvDescriptor structure depends on HW (PNIP-ASIC)                         */
/*---------------------------------------------------------------------------*/

/*
32 RX-Rings

RcvTableBasePtr IFA/B  -->  PNIP_DMACW_RCV_DESCR_TYPE[0] --> PNIP_DMACW_RCV_ITEM_TYPE --> ..  --> PNIP_DMACW_RCV_ITEM_TYPE
                            PNIP_DMACW_RCV_DESCR_TYPE[1]              A                                   |
                            ...                                       |                                   |
                            PNIP_DMACW_RCV_DESCR_TYPE[31]             *-----------------------------------*

*/



/*
Layout:
=======
Bits:   31                  23                  15                  7               0
n   :                           CurrentDMACWPtr (4Byte aligned)
n+1 :             DMACWDropCounter(15:0),       TS,-,  Err,-,    INT,    reserved
*/

/*---------------------------------------------------------------------------*/

typedef struct _PNIP_DMACW_RCV_DESCR_TYPE   EDDP_LOCAL_MEM_ATTR *    PNIP_LOCAL_DMACW_RCV_DESCR_PTR_TYPE;
PNIO_PACKED_ATTRIBUTE_PRE struct _PNIP_DMACW_RCV_DESCR_TYPE
{
    LSA_UINT32  CurrentDmacwHwPtr;              // 31..00: CurrentDMACWPtr (current processing DMACW)
    LSA_UINT32  RcvDropCntTsErrIntVal;          // 31..16: DMACWDropCounter (for this DMACW ring)
                                                //     15: TS: TagStripping
                                                // 13..12: Err: ring error status
                                                // 10..08: INT: interrupt mask (3 IRQ lines)
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _PNIP_DMACW_RCV_DESCR_TYPE   PNIP_DMACW_RCV_DESCR_TYPE;

// PNIP_DMACW_RCV_DESCR_TYPE.RcvDropCntErrIntVal
#define PNIP_DMACW_RCV_DESCR_DROPCNT_MSK      0xFFFF0000              // 31..16:  DMACWDropCounter
    #define PNIP_DMACW_RCV_DESCR_DROPCNT_SHIFT    16                  //          Shift Value
    #define PNIP_DMACW_RCV_DESCR_DROPCNT_ZERO     ((LSA_UINT32)0<<16) //          special init. value
#define PNIP_DMACW_RCV_DESCR_TS_MSK           0x00008000              //     15:  TS:     TagStripping
    #define PNIP_DMACW_RCV_DESCR_TS_SHIFT         15                  //          Shift Value
    #define PNIP_DMACW_RCV_DESCR_TS_ON            ((LSA_UINT32)1<<15) //          TagStripping on
    #define PNIP_DMACW_RCV_DESCR_TS_OFF           ((LSA_UINT32)0<<15) //          TagStripping off
#define PNIP_DMACW_RCV_DESCR_RSVD3_MSK        0x00004000              //     14:  reserved
    #define PNIP_DMACW_RCV_DESCR_RSVD3_SHIFT      14                  //          Shift Value
#define PNIP_DMACW_RCV_DESCR_ERR_MSK          0x00003000              // 13..12:  Err:    ring error status
    #define PNIP_DMACW_RCV_DESCR_ERR_SHIFT        12                  //          Shift Value
    #define PNIP_DMACW_RCV_DESCR_ERR_RESERVED     ((LSA_UINT32)3<<12) //          Reserved
    #define PNIP_DMACW_RCV_DESCR_ERR_ALIGNMENT    ((LSA_UINT32)2<<12) //          AlignmentError
    #define PNIP_DMACW_RCV_DESCR_ERR_AHB_READ     ((LSA_UINT32)1<<12) //          AHBReadErr
    #define PNIP_DMACW_RCV_DESCR_ERR_OK           ((LSA_UINT32)0<<12) //          no error
#define PNIP_DMACW_RCV_DESCR_RSVD2_MSK        0x00000800              // 11:      reserved
    #define PNIP_DMACW_RCV_DESCR_RSVD2_SHIFT      11                  //          Shift Value
#define PNIP_DMACW_RCV_DESCR_INT_MSK          0x00000700              // 10..08:  INT:    interrupt mask (3 IRQ lines)
    #define PNIP_DMACW_RCV_DESCR_INT_SHIFT        8                   //          Shift Value
    #define PNIP_DMACW_RCV_DESCR_INT_ICU3         ((LSA_UINT32)4<<8)  //          interrupt mask ICU3
    #define PNIP_DMACW_RCV_DESCR_INT_ICU2         ((LSA_UINT32)2<<8)  //          interrupt mask ICU2
    #define PNIP_DMACW_RCV_DESCR_INT_ICU1         ((LSA_UINT32)1<<8)  //          interrupt mask ICU1
    #define PNIP_DMACW_RCV_DESCR_INT_NONE         ((LSA_UINT32)0<<8)  //          no interrupt
#define PNIP_DMACW_RCV_DESCR_RSVD1_MSK        0x000000FF              // 07..00:  reserved
    #define PNIP_DMACW_RCV_DESCR_RSVD1_SHIFT      0                   //          Shift Value

#define PNIP_DMACW_RCV_DESCR_CFG_DEF_SW               (   PNIP_DMACW_RCV_DESCR_DROPCNT_ZERO      |  /* special init. value                              */ \
                                                            PNIP_DMACW_RCV_DESCR_TS_ON             |  /* default for TagStripping                         */ \
                                                            PNIP_DMACW_RCV_DESCR_ERR_OK            |  /* clr error code                                   */ \
                                                            PNIP_DMACW_RCV_DESCR_INT_NONE             /* no interrupt                                     */ \
                                                        )


#define PNIP_DMACW_RCV_DESCR_SET_CURRPTR(stru, ptr)   (stru.CurrentDmacwHwPtr = ptr)
#define PNIP_DMACW_RCV_DESCR_GET_CURRPTR(stru)        (stru.CurrentDmacwHwPtr)
// i/o as LSB-Value
#define PNIP_DMACW_RCV_DESCR_SET_DROPCNT(stru, cnt)   stru.RcvDropCntTsErrIntVal &= (~PNIP_DMACW_RCV_DESCR_DROPCNT_MSK); stru.RcvDropCntTsErrIntVal |= (cnt<<PNIP_DMACW_RCV_DESCR_DROPCNT_SHIFT)
#define PNIP_DMACW_RCV_DESCR_GET_DROPCNT(stru)        ((stru.RcvDropCntTsErrIntVal & PNIP_DMACW_RCV_DESCR_DROPCNT_MSK)>>PNIP_DMACW_RCV_DESCR_DROPCNT_SHIFT)
// TagStripping
#define PNIP_DMACW_RCV_DESCR_SET_TAGSTRIP_ON(stru)    (stru.RcvDropCntTsErrIntVal |= PNIP_DMACW_RCV_DESCR_TS_MSK)
#define PNIP_DMACW_RCV_DESCR_SET_TAGSTRIP_OFF(stru)   (stru.RcvDropCntTsErrIntVal &= (~PNIP_DMACW_RCV_DESCR_TS_MSK))
#define PNIP_DMACW_RCV_DESCR_GET_TAGSTRIP(stru)       (stru.RcvDropCntTsErrIntVal & PNIP_DMACW_RCV_DESCR_TS_MSK)
// ErrorStatus
#define PNIP_DMACW_RCV_DESCR_CLR_ERR(stru)            (stru.RcvDropCntTsErrIntVal &= (~PNIP_DMACW_RCV_DESCR_ERR_MSK))
#define PNIP_DMACW_RCV_DESCR_GET_ERR(stru)            (stru.RcvDropCntTsErrIntVal & PNIP_DMACW_RCV_DESCR_ERR_MSK)
// Interrupt Selector
#define PNIP_DMACW_RCV_DESCR_SET_INT_ICU1(stru)       stru.RcvDropCntTsErrIntVal &= (~PNIP_DMACW_RCV_DESCR_INT_MSK); stru.RcvDropCntTsErrIntVal |= PNIP_DMACW_RCV_DESCR_INT_ICU1
#define PNIP_DMACW_RCV_DESCR_SET_INT_ICU2(stru)       stru.RcvDropCntTsErrIntVal &= (~PNIP_DMACW_RCV_DESCR_INT_MSK); stru.RcvDropCntTsErrIntVal |= PNIP_DMACW_RCV_DESCR_INT_ICU2
#define PNIP_DMACW_RCV_DESCR_SET_INT_ICU3(stru)       stru.RcvDropCntTsErrIntVal &= (~PNIP_DMACW_RCV_DESCR_INT_MSK); stru.RcvDropCntTsErrIntVal |= PNIP_DMACW_RCV_DESCR_INT_ICU3
#define PNIP_DMACW_RCV_DESCR_GET_INT(stru)            (stru.RcvDropCntTsErrIntVal & PNIP_DMACW_RCV_DESCR_INT_MSK)



/*---------------------------------------------------------------------------*/
/* Rcv-DMACW structure depends on HW (PNIP-ASIC)                             */
/*---------------------------------------------------------------------------*/

/*
Layout:
=======
Bits:   31                  23                  15                  7               0
n   :   O,-,SYN,RES,--, FrmBufLen,              -, Source,         FrameLength
n+1 :                                   NextDMACW
n+2 :                                   TimeStamp
n+3 :                                 FrameBufferPtr
n+4 :   -----------,                      UseType,          FilterDecisionCode
*/

typedef struct _PNIP_DMACW_RCV_ITEM_TYPE    EDDP_LOCAL_MEM_ATTR *   PNIP_LOCAL_DMACW_RCV_ITEM_PTR_TYPE;
PNIO_PACKED_ATTRIBUTE_PRE struct _PNIP_DMACW_RCV_ITEM_TYPE
{
    LSA_UINT32      RcvInf1;                //     31: 0:       Owner-Bit
                                            //     29: SYN:     Sync-Frame-Bit
                                            // 28..27: RES:     Result-Status
                                            // 24..16: Frame-Buffer-Length in 32-Bit parts
                                            // 14..11: Source:  Rx-Port
                                            // 10..00: FrameLength: received data count
    LSA_UINT32      NextDMACWPtr;           // NextDMACWPtr (4byte aligned)
    LSA_UINT32      TimeStamp;              // time for transmit/sending --> if SndCfg.SendPhase 1xxx: use Time-Stamp-Field
    LSA_UINT32      FrameBufferPtr;         // FrameBufferPtr (telegramm buffer)
    LSA_UINT32      RcvInf2;                // 18..16: Use-Type: result build frame class
                                            // 15..00: FilterDecisionCode:
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _PNIP_DMACW_RCV_ITEM_TYPE    PNIP_DMACW_RCV_ITEM_TYPE;


#define PNIP_DMACW_RCV_ITEM_SET_INF1(stru, inf)      (stru.RcvInf1 = inf)
#define PNIP_DMACW_RCV_ITEM_GET_INF1(stru)           (stru.RcvInf1)

#define PNIP_DMACW_RCV_ITEM_SET_NEXTPTR(stru, next)  (stru.NextDMACWPtr = next)
#define PNIP_DMACW_RCV_ITEM_GET_NEXTPTR(stru)        (stru.NextDMACWPtr)

#define PNIP_DMACW_RCV_ITEM_SET_TS(stru, ts)         (stru.TimeStamp = ts)
#define PNIP_DMACW_RCV_ITEM_GET_TS(stru)             (stru.TimeStamp)

#define PNIP_DMACW_RCV_ITEM_SET_RXBUF(stru, buf)     (stru.FrameBufferPtr = buf)
#define PNIP_DMACW_RCV_ITEM_GET_RXBUF(stru)          (stru.FrameBufferPtr)

#define PNIP_DMACW_RCV_ITEM_SET_INF2(stru, inf)      (stru.RcvInf2 = inf)
#define PNIP_DMACW_RCV_ITEM_GET_INF2(stru)           (stru.RcvInf2)


// PNIP_DMACW_RCV_ITEM_TYPE.RcvInfo1
#define PNIP_DMACW_RCV_ITEM_INF1_OWN_MSK             0x80000000              // 31:      0:       Owner-Bit
    #define PNIP_DMACW_RCV_ITEM_INF1_OWN_SHIFT           31                  //          Shift Value
    #define PNIP_DMACW_RCV_ITEM_INF1_OWN_HW              ((LSA_UINT32)0<<31) //          0:PN-IP (HW)
    #define PNIP_DMACW_RCV_ITEM_INF1_OWN_SW              ((LSA_UINT32)1<<31) //          1:Software
#define PNIP_DMACW_RCV_ITEM_INF1_RSVD3_MSK           0x40000000              // 30:      reserved, set 0
    #define PNIP_DMACW_RCV_ITEM_INF1_RSVD3_SHIFT         25                  //          Shift Value
#define PNIP_DMACW_RCV_ITEM_INF1_SYN_MSK             0x20000000              // 29:      SYN:    Sync-Frame-Bit
    #define PNIP_DMACW_RCV_ITEM_INF1_SYN_SHIFT           29                  //          Shift Value
    #define PNIP_DMACW_RCV_ITEM_INF1_CFG_SYN_NO          ((LSA_UINT32)0<<29) //          0:no Sync-Frame
//    #define PNIP_DMACW_RCV_ITEM_INF1_CFG_SYN_YES         (1<<29)             //          1:Sync-Frame (8byte timestamp included)
#define PNIP_DMACW_RCV_ITEM_INF1_RESULT_MSK          0x18000000              // 28..27:  RES:     ResultStatus
    #define PNIP_DMACW_RCV_ITEM_INF1_RESULT_SHIFT        27                  //          Shift Value
    #define PNIP_DMACW_RCV_ITEM_INF1_RESULT_OK           ((LSA_UINT32)0<<27) //          00: transfer ok
    #define PNIP_DMACW_RCV_ITEM_INF1_RESULT_OK_WARN      ((LSA_UINT32)1<<27) //          01: ok but NextDMACWNotAligned
    #define PNIP_DMACW_RCV_ITEM_INF1_RESULT_ERR_BUFSHORT ((LSA_UINT32)2<<27) //          10: FrameBufferTooSmall
    #define PNIP_DMACW_RCV_ITEM_INF1_RESULT_ERR_BUFALIGN ((LSA_UINT32)3<<27) //          11: FrameBufferNotAligned
#define PNIP_DMACW_RCV_ITEM_INF1_RSVD2_MSK           0x06000000              // 26..25:  reserved, set 0
    #define PNIP_DMACW_RCV_ITEM_INF1_RSVD2_SHIFT         25                  //          Shift Value
#define PNIP_DMACW_RCV_ITEM_INF1_FBUFLEN_MSK         0x01FF0000              // 24..16:  Frame-Buffer-Length in 32-Bit parts
    #define PNIP_DMACW_RCV_ITEM_INF1_FBUFLEN_SHIFT       16                  //          Shift Value
    #define PNIP_DMACW_RCV_ITEM_INF1_FBUFLEN_ZERO        0                   //          special default value
#define PNIP_DMACW_RCV_ITEM_INF1_RSVD1_MSK           0x00008000              // 15:      reserved, set 0
    #define PNIP_DMACW_RCV_ITEM_INF1_RSVD1_SHIFT         15                  //          Shift Value
#define PNIP_DMACW_RCV_ITEM_INF1_SOURCE_MSK          0x00007800              // 14..11:  Source:     Source-Port
    #define PNIP_DMACW_RCV_ITEM_INF1_SOURCE_SHIFT        11                  //          Shift Value
    #define PNIP_DMACW_RCV_ITEM_INF1_SOURCE_NONE         ((LSA_UINT32)0<<11) //          0000: special value for init.
    #define PNIP_DMACW_RCV_ITEM_INF1_SOURCE_P1           ((LSA_UINT32)1<<11) //          0001: Port 1
    #define PNIP_DMACW_RCV_ITEM_INF1_SOURCE_P2           ((LSA_UINT32)2<<11) //          0010: Port 2
    #define PNIP_DMACW_RCV_ITEM_INF1_SOURCE_P3           ((LSA_UINT32)4<<11) //          0100: Port 3
    #define PNIP_DMACW_RCV_ITEM_INF1_SOURCE_P4           ((LSA_UINT32)8<<11) //          1000: Port 4
#define PNIP_DMACW_RCV_ITEM_INF1_FRMLEN_MSK          0x000007FF              // 10..00:  (received) FrameLength
    #define PNIP_DMACW_RCV_ITEM_INF1_FRMLEN_SHIFT        0                   //          Shift Value
    #define PNIP_DMACW_RCV_ITEM_INF1_FRMLEN_ZERO         0                   //          special default value

// PNIP_DMACW_RCV_ITEM_TYPE.RcvInfo2
#define PNIP_DMACW_RCV_ITEM_INF2_RSVD1_MSK           0xFFF80000              // 31..19:  reserved, set 0
    #define PNIP_DMACW_RCV_ITEM_INF2_RSVD1_SHIFT         19                  //          Shift Value
#define PNIP_DMACW_RCV_ITEM_INF2_USETYPE_MSK         0x00070000              // 18..16:  Use-Type
    #define PNIP_DMACW_RCV_ITEM_INF2_USETYPE_SHIFT       16                  //          Shift Value
    #define PNIP_DMACW_RCV_ITEM_INF2_USETYPE_STD         ((LSA_UINT32)0<<16) //          000: Std
    #define PNIP_DMACW_RCV_ITEM_INF2_USETYPE_PTCP_CLOCK  ((LSA_UINT32)1<<16) //          001: PTCP Clock
    #define PNIP_DMACW_RCV_ITEM_INF2_USETYPE_PTCP_TIME   ((LSA_UINT32)2<<16) //          010: PTCP Time
    #define PNIP_DMACW_RCV_ITEM_INF2_USETYPE_PTCP_DELAY  ((LSA_UINT32)3<<16) //          011: PTCP Delay
    #define PNIP_DMACW_RCV_ITEM_INF2_USETYPE_FUP_CLK     ((LSA_UINT32)4<<16) //          100: FUP Clock
    #define PNIP_DMACW_RCV_ITEM_INF2_USETYPE_FUP_TIME    ((LSA_UINT32)5<<16) //          101: FUP Time
    #define PNIP_DMACW_RCV_ITEM_INF2_USETYPE_PTP         ((LSA_UINT32)6<<16) //          110: PTP
    #define PNIP_DMACW_RCV_ITEM_INF2_USETYPE_PTP_DELAY   ((LSA_UINT32)7<<16) //          111: PTP Delay
#define PNIP_DMACW_RCV_ITEM_INF2_FDC_MSK             0x0000FFFF              // 15..00:  FilterDecisionCode:
    #define PNIP_DMACW_RCV_ITEM_INF2_FDC_SHIFT           0                   //          Shift Value
    #define PNIP_DMACW_RCV_ITEM_INF2_FDC_ZERO            0                   //          special default value

// default init.-value for INF1 --> PNIP_DMACW_RCV_ITEM_SET_INF1(stru, PNIP_DMACW_RCV_ITEM_INF1_DEF_HW);
#define PNIP_DMACW_RCV_ITEM_INF1_DEF_HW(len)         (      PNIP_DMACW_RCV_ITEM_INF1_OWN_HW       |  /* Owner = HW                                       */ \
                                                            PNIP_DMACW_RCV_ITEM_INF1_CFG_SYN_NO   |  /* dummy for sync bit                               */ \
                                                            PNIP_DMACW_RCV_ITEM_INF1_RESULT_OK    |  /* clr result code                                  */ \
                                                          ( (len<<PNIP_DMACW_RCV_ITEM_INF1_FBUFLEN_SHIFT) & \
                                                            PNIP_DMACW_RCV_ITEM_INF1_FBUFLEN_MSK)|  /* set buffer length                              */ \
                                                            PNIP_DMACW_RCV_ITEM_INF1_SOURCE_NONE  |  /* dummy for Port                                   */ \
                                                            PNIP_DMACW_RCV_ITEM_INF1_FRMLEN_ZERO     /* no RX length in default init.-value              */ \
                                                        )
#define PNIP_DMACW_RCV_ITEM_INF1_DEF_SW              (      PNIP_DMACW_RCV_ITEM_INF1_OWN_SW       |  /* Owner = SW                                       */ \
                                                            PNIP_DMACW_RCV_ITEM_INF1_CFG_SYN_NO   |  /* dummy for sync bit                               */ \
                                                            PNIP_DMACW_RCV_ITEM_INF1_RESULT_OK    |  /* clr result code                                  */ \
                                                            PNIP_DMACW_RCV_ITEM_INF1_FBUFLEN_ZERO |  /* set buffer length 0                              */ \
                                                            PNIP_DMACW_RCV_ITEM_INF1_SOURCE_NONE  |  /* dummy for Port                                   */ \
                                                            PNIP_DMACW_RCV_ITEM_INF1_FRMLEN_ZERO     /* no RX length in default init.-value              */ \
                                                        )
// 0:       Owner-Bit           0:HW, 1:SW
#define PNIP_DMACW_RCV_ITEM_SET_OWNER_HW(stru)       (stru.RcvInf1 &= (~PNIP_DMACW_RCV_ITEM_INF1_OWN_MSK))
#define PNIP_DMACW_RCV_ITEM_SET_OWNER_SW(stru)       (stru.RcvInf1 |= PNIP_DMACW_RCV_ITEM_INF1_OWN_SW)
#define PNIP_DMACW_RCV_ITEM_GET_OWNER(stru)          (stru.RcvInf1 & PNIP_DMACW_RCV_ITEM_INF1_OWN_MSK)
// SYN:     Sync Bit
#define PNIP_DMACW_RCV_ITEM_GET_SYN(stru)            (stru.RcvInf1 & PNIP_DMACW_RCV_ITEM_INF1_SYN_MSK)
// RES:     ResultStatus
#define PNIP_DMACW_RCV_ITEM_CLR_RESULT(stru)         (stru.RcvInf1 &= (~PNIP_DMACW_RCV_ITEM_INF1_RESULT_MSK))
#define PNIP_DMACW_RCV_ITEM_GET_RESULT(stru)         (stru.RcvInf1 & PNIP_DMACW_RCV_ITEM_INF1_RESULT_MSK)
// Frame-Buffer-Length in 32-Bit parts as LSB-Value
#define PNIP_DMACW_RCV_ITEM_SET_FBUFLEN(stru,len)    (stru.RcvInf1 &= (~PNIP_DMACW_RCV_ITEM_INF1_FBUFLEN_MSK); stru.RcvInf1 |= ((len<<PNIP_DMACW_RCV_ITEM_INF1_FBUFLEN_SHIFT) & PNIP_DMACW_RCV_ITEM_INF1_FBUFLEN_MSK))
#define PNIP_DMACW_RCV_ITEM_GET_FBUFLEN(stru)        ((stru.RcvInf1 & PNIP_DMACW_RCV_ITEM_INF1_FBUFLEN_MSK)>>PNIP_DMACW_RCV_ITEM_INF1_FBUFLEN_SHIFT)
// Source:     Source-Port
#define PNIP_DMACW_RCV_ITEM_CLR_SOURCE(stru)         (stru.RcvInf1 &= (~PNIP_DMACW_RCV_ITEM_INF1_SOURCE_MSK))
#define PNIP_DMACW_RCV_ITEM_GET_SOURCE(stru)         (stru.RcvInf1 & PNIP_DMACW_RCV_ITEM_INF1_SOURCE_MSK)
// (received) FrameLength as LSB-Value
#define PNIP_DMACW_RCV_ITEM_CLR_RXFRMLEN(stru)       (stru.RcvInf1 &= (~PNIP_DMACW_RCV_ITEM_INF1_FRMLEN_MSK))
#define PNIP_DMACW_RCV_ITEM_GET_RXFRMLEN(stru)       ((stru.RcvInf1 & PNIP_DMACW_RCV_ITEM_INF1_FRMLEN_MSK)>>PNIP_DMACW_RCV_ITEM_INF1_FRMLEN_SHIFT)



// default init.-value for INF2 --> PNIP_DMACW_RCV_ITEM_SET_INF2(stru, PNIP_DMACW_RCV_ITEM_INF2_DEF);
#define PNIP_DMACW_RCV_ITEM_INF2_DEF                 (   PNIP_DMACW_RCV_ITEM_INF2_USETYPE_STD  |  /* dummy for UseType                                */ \
                                                         PNIP_DMACW_RCV_ITEM_INF2_FDC_ZERO        /* dummy for FDC                                    */ \
                                                     )

#define PNIP_DMACW_RCV_ITEM_INF2_DEF_FDC(Fdc)        (   PNIP_DMACW_RCV_ITEM_INF2_USETYPE_STD  |  /* dummy for UseType                                */ \
                                                         PNIP_DMACW_RCV_ITEM_INF2_FDC_ZERO     |  /* dummy for FDC                                    */ \
                                                         ( (Fdc<<PNIP_DMACW_RCV_ITEM_INF2_FDC_SHIFT) & PNIP_DMACW_RCV_ITEM_INF2_FDC_MSK )                \
                                                     )


// Use-Type
#define PNIP_DMACW_RCV_ITEM_CLR_USETYPE(stru)        (stru.RcvInf2 &= (~PNIP_DMACW_RCV_ITEM_INF2_USETYPE_MSK))
#define PNIP_DMACW_RCV_ITEM_GET_USETYPE(stru)        (stru.RcvInf2 & PNIP_DMACW_RCV_ITEM_INF2_USETYPE_MSK)
// FilterDecisionCode as LSB-Value
#define PNIP_DMACW_RCV_ITEM_CLR_FDC(stru)            (stru.RcvInf2 &= (~PNIP_DMACW_RCV_ITEM_INF2_FDC_MSK))
#define PNIP_DMACW_RCV_ITEM_SET_FDC(stru, Fdc)       stru.RcvInf2 &= (~PNIP_DMACW_RCV_ITEM_INF2_FDC_MSK); stru.RcvInf2 |= ((Fdc<<PNIP_DMACW_RCV_ITEM_INF2_FDC_SHIFT) & PNIP_DMACW_RCV_ITEM_INF2_FDC_MSK)
#define PNIP_DMACW_RCV_ITEM_GET_FDC(stru)            ((stru.RcvInf2 & PNIP_DMACW_RCV_ITEM_INF2_FDC_MSK)>>PNIP_DMACW_RCV_ITEM_INF2_FDC_SHIFT)



/*---------------------------------------------------------------------------*/
/* SndDescriptor structure depends on HW (PNIP-ASIC)                         */
/*---------------------------------------------------------------------------*/

/*
64 String descriptors

SDTableBase IFA/B      -->  PNIP_FILTER_STRING_DESCR_TYPE[0] 
                            PNIP_FILTER_STRING_DESCR_TYPE[1]
                            ...
                            PNIP_FILTER_STRING_DESCR_TYPE[63]

Layout:
=======
Bits:   31                  23                  15                  7               0
n   :   EN   ,   -   ,     Len                  ,          Offset to SDTableBase
*/

typedef struct _PNIP_FILTER_STRING_TYPE     EDDP_LOCAL_MEM_ATTR *   PNIP_LOCAL_FILTER_STRING_PTR_TYPE;
PNIO_PACKED_ATTRIBUTE_PRE struct _PNIP_FILTER_STRING_TYPE
{
    LSA_UINT32      StringDescriptor;       // 31..30: EN:            Action for SCMP opcode
                                            // 24..16: StringLength:  Length in Bytes 0...511
                                            // 15..00: StringPtr:     Offset to SDTableBase
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _PNIP_FILTER_STRING_TYPE     PNIP_FILTER_STRING_TYPE;


#define PNIP_FILTER_STRING_DESCR_EN_MSK              0xC0000000               // 31..30:  EN: Action for SCMP opcode
    #define PNIP_FILTER_STRING_DESCR_EN_SHIFT            30                   //          Shift Value
    #define PNIP_FILTER_STRING_DESCR_EN_MATCH            ((LSA_UINT32)0<<30)  // 00:      „Wildcard“ match
    #define PNIP_FILTER_STRING_DESCR_EN_MISS             ((LSA_UINT32)1<<30)  // 01+10:   „Wildcard“ missmatch
    #define PNIP_FILTER_STRING_DESCR_EN_CMP              ((LSA_UINT32)3<<30)  // 11:      string compare
#define PNIP_FILTER_STRING_DESCR_RSVD1_MSK           0x3E000000               // 29..25:  reserved, set 0
    #define PNIP_FILTER_STRING_DESCR_RSVD1_SHIFT         25                   //          Shift Value
#define PNIP_FILTER_STRING_DESCR_LEN_MSK             0x01FF0000               // 24..16:  StringLength:  Length in Bytes 0...511
    #define PNIP_FILTER_STRING_DESCR_LEN_SHIFT           16                   //          Shift Value
#define PNIP_FILTER_STRING_DESCR_OFFS_MSK            0x0000FFFF               // 15..0:   StringPtr:     Offset to SDTableBase
    #define PNIP_FILTER_STRING_DESCR_OFFS_SHIFT          0                    //          Shift Value

// default init.-value for disabled StringDescriptor --> set StringLength > 0 to avoid sideffect for "Wildcard" settings
#define PNIP_FILTER_STRING_DESCR_DEF_OFF               (    PNIP_FILTER_STRING_DESCR_EN_MISS    |  /* EN = Miss         */ \
                                                            0                                   |  /* reserved, set 0   */ \
                                                            0x10000                             |  /* StringLength = 1  */ \
                                                            0                                      /* StringPtr = 0     */ \
                                                       )

// default init.-value for enabled StringDescriptor --> set StringLength = 0 to force MATCH
#define PNIP_FILTER_STRING_DESCR_DEF_ON                (    PNIP_FILTER_STRING_DESCR_EN_MATCH   |  /* EN = Match        */ \
                                                            0                                   |  /* reserved, set 0   */ \
                                                            0x00000                             |  /* StringLength = 1  */ \
                                                            0                                      /* StringPtr = 0     */ \
                                                       )

#define PNIP_FILTER_STRING_DESCR_SET_EN(var, val)    var &= ~(PNIP_FILTER_STRING_DESCR_EN_MSK); var |= (val & PNIP_FILTER_STRING_DESCR_EN_MSK)
#define PNIP_FILTER_STRING_DESCR_GET_EN(var)         var & (PNIP_FILTER_STRING_DESCR_EN_MSK)


#define PNIP_FILTER_STRING_DESCR_SET_LEN(var, len)   var &= ~(PNIP_FILTER_STRING_DESCR_LEN_MSK); var |= ( ((LSA_UINT32)len<<PNIP_FILTER_STRING_DESCR_LEN_SHIFT) & PNIP_FILTER_STRING_DESCR_LEN_MSK )
#define PNIP_FILTER_STRING_DESCR_GET_LEN(var)        ((var & (PNIP_FILTER_STRING_DESCR_LEN_MSK)) >> PNIP_FILTER_STRING_DESCR_LEN_SHIFT)


#define PNIP_FILTER_STRING_DESCR_SET_OFFS(var, offs) var &= ~(PNIP_FILTER_STRING_DESCR_OFFS_MSK); var |= (offs & PNIP_FILTER_STRING_DESCR_OFFS_MSK)
#define PNIP_FILTER_STRING_DESCR_GET_OFFS(var)       var & (PNIP_FILTER_STRING_DESCR_OFFS_MSK)


#define PNIP_FILTER_PSTRING_DESCR_OFFSET_MSK                0x000001F8
    #define PNIP_FILTER_PSTRING_DESCR_OFFSET_SHIFT          3
#define PNIP_FILTER_PSTRING_DESCR_PSTABLE_NUMBER_MSK        0x00002000
    #define PNIP_FILTER_PSTRING_DESCR_PSTABLE_NUMBER_SHIFT  13
    #define PNIP_FILTER_PSTRING_DESCR_PSTABLE_NUMBER_1      ((LSA_UINT32)0<<PNIP_FILTER_PSTRING_DESCR_PSTABLE_NUMBER_SHIFT)
    //#define PNIP_FILTER_PSTRING_DESCR_PSTABLE_NUMBER_2      ((LSA_UINT32)1<<PNIP_FILTER_PSTRING_DESCR_PSTABLE_NUMBER_SHIFT)
#define PNIP_FILTER_PSTRING_DESCR_EN_01_MSK                 0x00010000
    #define PNIP_FILTER_PSTRING_DESCR_EN_01_SHIFT           16
    #define PNIP_FILTER_PSTRING_DESCR_EN_01_ENABLE          ((LSA_UINT32)1<<PNIP_FILTER_PSTRING_DESCR_EN_01_SHIFT)
    #define PNIP_FILTER_PSTRING_DESCR_EN_01_DISABLE         ((LSA_UINT32)0<<PNIP_FILTER_PSTRING_DESCR_EN_01_SHIFT)
#define PNIP_FILTER_PSTRING_DESCR_EN_02_MSK                 0x00020000
    #define PNIP_FILTER_PSTRING_DESCR_EN_02_SHIFT           17
    #define PNIP_FILTER_PSTRING_DESCR_EN_02_ENABLE          ((LSA_UINT32)1<<PNIP_FILTER_PSTRING_DESCR_EN_02_SHIFT)
    #define PNIP_FILTER_PSTRING_DESCR_EN_02_DISABLE         ((LSA_UINT32)0<<PNIP_FILTER_PSTRING_DESCR_EN_02_SHIFT)
#define PNIP_FILTER_PSTRING_DESCR_EN_03_MSK                 0x00040000
    #define PNIP_FILTER_PSTRING_DESCR_EN_03_SHIFT           18
    #define PNIP_FILTER_PSTRING_DESCR_EN_03_ENABLE          ((LSA_UINT32)1<<PNIP_FILTER_PSTRING_DESCR_EN_03_SHIFT)
    #define PNIP_FILTER_PSTRING_DESCR_EN_03_DISABLE         ((LSA_UINT32)0<<PNIP_FILTER_PSTRING_DESCR_EN_03_SHIFT)
#define PNIP_FILTER_PSTRING_DESCR_EN_04_MSK                 0x00080000
    #define PNIP_FILTER_PSTRING_DESCR_EN_04_SHIFT           19
    #define PNIP_FILTER_PSTRING_DESCR_EN_04_ENABLE          ((LSA_UINT32)1<<PNIP_FILTER_PSTRING_DESCR_EN_04_SHIFT)
    #define PNIP_FILTER_PSTRING_DESCR_EN_04_DISABLE         ((LSA_UINT32)0<<PNIP_FILTER_PSTRING_DESCR_EN_04_SHIFT)
#define PNIP_FILTER_PSTRING_DESCR_EN_05_MSK                 0x00100000
    #define PNIP_FILTER_PSTRING_DESCR_EN_05_SHIFT           20
    #define PNIP_FILTER_PSTRING_DESCR_EN_05_ENABLE          ((LSA_UINT32)1<<PNIP_FILTER_PSTRING_DESCR_EN_05_SHIFT)
    #define PNIP_FILTER_PSTRING_DESCR_EN_05_DISABLE         ((LSA_UINT32)0<<PNIP_FILTER_PSTRING_DESCR_EN_05_SHIFT)
#define PNIP_FILTER_PSTRING_DESCR_EN_06_MSK                 0x00200000
    #define PNIP_FILTER_PSTRING_DESCR_EN_06_SHIFT           21
    #define PNIP_FILTER_PSTRING_DESCR_EN_06_ENABLE          ((LSA_UINT32)1<<PNIP_FILTER_PSTRING_DESCR_EN_06_SHIFT)
    #define PNIP_FILTER_PSTRING_DESCR_EN_06_DISABLE         ((LSA_UINT32)0<<PNIP_FILTER_PSTRING_DESCR_EN_06_SHIFT)
#define PNIP_FILTER_PSTRING_DESCR_EN_07_MSK                 0x00400000
    #define PNIP_FILTER_PSTRING_DESCR_EN_07_SHIFT           22
    #define PNIP_FILTER_PSTRING_DESCR_EN_07_ENABLE          ((LSA_UINT32)1<<PNIP_FILTER_PSTRING_DESCR_EN_07_SHIFT)
    #define PNIP_FILTER_PSTRING_DESCR_EN_07_DISABLE         ((LSA_UINT32)0<<PNIP_FILTER_PSTRING_DESCR_EN_07_SHIFT)
#define PNIP_FILTER_PSTRING_DESCR_EN_08_MSK                 0x00800000
    #define PNIP_FILTER_PSTRING_DESCR_EN_08_SHIFT           23
    #define PNIP_FILTER_PSTRING_DESCR_EN_08_ENABLE          ((LSA_UINT32)1<<PNIP_FILTER_PSTRING_DESCR_EN_08_SHIFT)
    #define PNIP_FILTER_PSTRING_DESCR_EN_08_DISABLE         ((LSA_UINT32)0<<PNIP_FILTER_PSTRING_DESCR_EN_08_SHIFT)
#define PNIP_FILTER_PSTRING_DESCR_EN_09_MSK                 0x01000000
    #define PNIP_FILTER_PSTRING_DESCR_EN_09_SHIFT           24
    #define PNIP_FILTER_PSTRING_DESCR_EN_09_ENABLE          ((LSA_UINT32)1<<PNIP_FILTER_PSTRING_DESCR_EN_09_SHIFT)
    #define PNIP_FILTER_PSTRING_DESCR_EN_09_DISABLE         ((LSA_UINT32)0<<PNIP_FILTER_PSTRING_DESCR_EN_09_SHIFT)
#define PNIP_FILTER_PSTRING_DESCR_EN_10_MSK                 0x02000000
    #define PNIP_FILTER_PSTRING_DESCR_EN_10_SHIFT           25
    #define PNIP_FILTER_PSTRING_DESCR_EN_10_ENABLE          ((LSA_UINT32)1<<PNIP_FILTER_PSTRING_DESCR_EN_10_SHIFT)
    #define PNIP_FILTER_PSTRING_DESCR_EN_10_DISABLE         ((LSA_UINT32)0<<PNIP_FILTER_PSTRING_DESCR_EN_10_SHIFT)
#define PNIP_FILTER_PSTRING_DESCR_EN_11_MSK                 0x04000000
    #define PNIP_FILTER_PSTRING_DESCR_EN_11_SHIFT           26
    #define PNIP_FILTER_PSTRING_DESCR_EN_11_ENABLE          ((LSA_UINT32)1<<PNIP_FILTER_PSTRING_DESCR_EN_11_SHIFT)
    #define PNIP_FILTER_PSTRING_DESCR_EN_11_DISABLE         ((LSA_UINT32)0<<PNIP_FILTER_PSTRING_DESCR_EN_11_SHIFT)
#define PNIP_FILTER_PSTRING_DESCR_EN_12_MSK                 0x08000000
    #define PNIP_FILTER_PSTRING_DESCR_EN_12_SHIFT           27
    #define PNIP_FILTER_PSTRING_DESCR_EN_12_ENABLE          ((LSA_UINT32)1<<PNIP_FILTER_PSTRING_DESCR_EN_12_SHIFT)
    #define PNIP_FILTER_PSTRING_DESCR_EN_12_DISABLE         ((LSA_UINT32)0<<PNIP_FILTER_PSTRING_DESCR_EN_12_SHIFT)
#define PNIP_FILTER_PSTRING_DESCR_EN_13_MSK                 0x10000000
    #define PNIP_FILTER_PSTRING_DESCR_EN_13_SHIFT           28
    #define PNIP_FILTER_PSTRING_DESCR_EN_13_ENABLE          ((LSA_UINT32)1<<PNIP_FILTER_PSTRING_DESCR_EN_13_SHIFT)
    #define PNIP_FILTER_PSTRING_DESCR_EN_13_DISABLE         ((LSA_UINT32)0<<PNIP_FILTER_PSTRING_DESCR_EN_13_SHIFT)
#define PNIP_FILTER_PSTRING_DESCR_EN_14_MSK                 0x20000000
    #define PNIP_FILTER_PSTRING_DESCR_EN_14_SHIFT           29
    #define PNIP_FILTER_PSTRING_DESCR_EN_14_ENABLE          ((LSA_UINT32)1<<PNIP_FILTER_PSTRING_DESCR_EN_14_SHIFT)
    #define PNIP_FILTER_PSTRING_DESCR_EN_14_DISABLE         ((LSA_UINT32)0<<PNIP_FILTER_PSTRING_DESCR_EN_14_SHIFT)
#define PNIP_FILTER_PSTRING_DESCR_EN_15_MSK                 0x40000000
    #define PNIP_FILTER_PSTRING_DESCR_EN_15_SHIFT           30
    #define PNIP_FILTER_PSTRING_DESCR_EN_15_ENABLE          ((LSA_UINT32)1<<PNIP_FILTER_PSTRING_DESCR_EN_15_SHIFT)
    #define PNIP_FILTER_PSTRING_DESCR_EN_15_DISABLE         ((LSA_UINT32)0<<PNIP_FILTER_PSTRING_DESCR_EN_15_SHIFT)
#define PNIP_FILTER_PSTRING_DESCR_EN_16_MSK                 0x80000000
    #define PNIP_FILTER_PSTRING_DESCR_EN_16_SHIFT           31
    #define PNIP_FILTER_PSTRING_DESCR_EN_16_ENABLE          ((LSA_UINT32)1<<PNIP_FILTER_PSTRING_DESCR_EN_16_SHIFT)
    #define PNIP_FILTER_PSTRING_DESCR_EN_16_DISABLE         ((LSA_UINT32)0<<PNIP_FILTER_PSTRING_DESCR_EN_16_SHIFT)

// default init.-value for enabled StringDescriptor --> set StringLength = 0 to force MATCH
#define PNIP_FILTER_PSTRING_DESCR_DEF_OFF                   (0)

/********************************************************************************************************************/
/* ACW = CRT                                                                                                        */
/********************************************************************************************************************/

// Crt-CPM-SW-Data
PNIO_PACKED_ATTRIBUTE_PRE struct _PNIP_ACW_RCV_CRT_DATA_TYPE
{
    LSA_UINT16          AcwId;
    LSA_UINT16          reserved1;
    LSA_UINT16          reserved2;
    LSA_UINT16          reserved3;
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _PNIP_ACW_RCV_CRT_DATA_TYPE  PNIP_ACW_RCV_CRT_DATA_TYPE;
/* AcwId       : Consumer/Dg/Container to which this ACW belongs                                   */


/********************************************************************************************************************/
/* ACW_DG Receive                                                                                                   */
/********************************************************************************************************************/

/* ------------------------------------------------------------------------- */
/* ACW_DG Receive - ACW_Word1                                                */
/* ------------------------------------------------------------------------- */
#define PNIP_ACWDG_RCV_WORD1_DG_POS_T_MASK                            0x000000000000007FULL     // 6...0
#define PNIP_ACWDG_RCV_WORD1_DG_POS_T_SHIFT                           0
                                                                                                // 7 reserved
#define PNIP_ACWDG_RCV_WORD1_DG_LEN_MASK                              0x000000000000FF00ULL     // 15...8
#define PNIP_ACWDG_RCV_WORD1_DG_LEN_SHIFT                             8
#define PNIP_R2_ACWDG_RCV_WORD1_ACW_DG_NEXT_OFFSET0_MASK              0x00000000FFFF0000ULL     // 31...16
#define PNIP_R3_ACWDG_RCV_WORD1_ACW_DG_NEXT_OFFSET0_MASK              0x000000007FFF0000ULL     // 30...16
#define PNIP_ACWDG_RCV_WORD1_ACW_DG_NEXT_OFFSET0_SHIFT                16
																	    						// 31 reserved in HERA
#define PNIP_R2_ACWDG_RCV_WORD1_ACW_DG_NEXT_OFFSET1_MASK              0x0000FFFF00000000ULL     // 47...32
#define PNIP_R3_ACWDG_RCV_WORD1_ACW_DG_NEXT_OFFSET1_MASK              0x00007FFF00000000ULL     // 46...32
#define PNIP_ACWDG_RCV_WORD1_ACW_DG_NEXT_OFFSET1_SHIFT                32
																	    						// 47 reserved in HERA
#define PNIP_ACWDG_RCV_WORD1_OPCODE_MASK                              0x0003000000000000ULL     // 49...48
#define PNIP_ACWDG_RCV_WORD1_OPCODE_SHIFT                             48
#define PNIP_ACWDG_RCV_WORD1_ACTIVE_MASK                              0x0004000000000000ULL     // 50
#define PNIP_ACWDG_RCV_WORD1_ACTIVE_SHIFT                             50
#define PNIP_ACWDG_RCV_WORD1_ONESHOT_BIT_MASK                         0x0008000000000000ULL     // 51
#define PNIP_ACWDG_RCV_WORD1_ONESHOT_BIT_SHIFT                        51
																	    						// 52..63 reserved
// constant value                                                     
#define PNIP_ACWDG_RCV_WORD1_DG_POS_T_INIT_VALUE                      0x0       // init PosT
#define PNIP_ACWDG_RCV_WORD1_DG_LEN_INIT_VALUE                        0x0       // init Len
#define PNIP_R2_ACWDG_RCV_WORD1_ACW_DG_NEXT_OFFSET_INIT_VALUE         0xFFFF    // init for ACW_DG_nextoffset
#define PNIP_R3_ACWDG_RCV_WORD1_ACW_DG_NEXT_OFFSET_INIT_VALUE         0x7FFF    // init for ACW_DG_nextoffset
#define PNIP_ACWDG_RCV_WORD1_OPCODE_ACW_DG_VALUE                      0x1       // opcode for receive ACWDG
#define PNIP_ACWDG_RCV_WORD1_ACTIVE_INIT_VALUE                        0x0       // init for Active-Bit
#define PNIP_ACWDG_RCV_WORD1_ONESHOT_INIT_VALUE                       0x0       // init for OneShot-Bit

/* ------------------------------------------------------------------------- */
/* ACW_DG Receive - ACW_Word2                                                */
/* ------------------------------------------------------------------------- */
#define PNIP_R2_ACWDG_RCV_WORD2_SCOREBOARD_OFFSET_MASK                0x000000000000FFFFULL     // 15...0
#define PNIP_R3_ACWDG_RCV_WORD2_SCOREBOARD_OFFSET_MASK                0x0000000000007FFFULL     // 14...0
#define PNIP_ACWDG_RCV_WORD2_SCOREBOARD_OFFSET_SHIFT                  0
#define PNIP_R2_ACWDG_RCV_WORD2_DG_STATUS_OFFSET_MASK                 0x00000003FFFF0000ULL     // 33...16
#define PNIP_R3_ACWDG_RCV_WORD2_DG_STATUS_OFFSET_MASK                 0x00000001FFFF0000ULL     // 32...16
#define PNIP_ACWDG_RCV_WORD2_DG_STATUS_OFFSET_SHIFT                   16
                                                                                                // 38...34 reserved
#define PNIP_ACWDG_RCV_WORD2_DP2_CHECK_MASK                           0x0000008000000000ULL     // 39
#define PNIP_ACWDG_RCV_WORD2_DP2_CHECK_SHIFT                          39
#define PNIP_ACWDG_RCV_WORD2_WD_RELOADVALUE_MASK                      0x0000FF0000000000ULL     // 47...40
#define PNIP_ACWDG_RCV_WORD2_WD_RELOADVALUE_SHIFT                     40
#define PNIP_ACWDG_RCV_WORD2_DH_RELOADVALUE_MASK                      0x00FF000000000000ULL     // 55...48
#define PNIP_ACWDG_RCV_WORD2_DH_RELOADVALUE_SHIFT                     48
#define PNIP_ACWDG_RCV_WORD2_MR_RELOADVALUE_MASK                      0xFF00000000000000ULL     // 63...56
#define PNIP_ACWDG_RCV_WORD2_MR_RELOADVALUE_SHIFT                     56
// constant value                                                
#define PNIP_R2_ACWDG_RCV_WORD2_SCOREBOARD_OFFSET_OFF_VALUE           0xFFFF    // no evaluating of SB                                                
#define PNIP_R3_ACWDG_RCV_WORD2_SCOREBOARD_OFFSET_OFF_VALUE           0x7FFF    // no evaluating of SB
#define PNIP_R2_ACWDG_RCV_WORD2_DG_STATUS_OFFSET_OFF_VALUE            0x3FFFF   // no evaluating of DG-Status
#define PNIP_R3_ACWDG_RCV_WORD2_DG_STATUS_OFFSET_OFF_VALUE            0x1FFFF   // no evaluating of DG-Status
#define PNIP_ACWDG_RCV_WORD2_DP2_CHECK_OFF_VALUE                      0x0       // monitoring of Consumer-Scoreboard OFF
#define PNIP_ACWDG_RCV_WORD2_DP2_CHECK_ON_VALUE                       0x1       // monitoring of Consumer-Scoreboard ON
#define PNIP_ACWDG_RCV_WORD2_MR_RELOADVALUE_OFF_VALUE                 0xFF      // MediaRedundancy-Timer in Timer-Scoreboard is off
#define PNIP_ACWDG_RCV_WORD2_WD_RELOADVALUE_OFF_VALUE                 0xFF      // Watchdog-Timer in Timer-Scoreboard is off

/* ------------------------------------------------------------------------- */
/* ACW_DG Receive - ACW_Word3                                                */
/* ------------------------------------------------------------------------- */
#define PNIP_R2_ACWDG_RCV_WORD3_DATA_PTR_MASK                             0x00000000FFFFFFFFULL     // 31...0
#define PNIP_R2_ACWDG_RCV_WORD3_DATA_PTR_SHIFT                            0
#define PNIP_R3_ACWDG_RCV_WORD3_MEM_TYPE_MASK                             0x0000000000000003ULL     // 1...0
#define PNIP_R3_ACWDG_RCV_WORD3_MEM_TYPE_SHIFT                            0
#define PNIP_R3_ACWDG_RCV_WORD3_LAST_RX_MASK                              0x0000000000000004ULL     // 2
#define PNIP_R3_ACWDG_RCV_WORD3_LAST_RX_SHIFT                             2
#define PNIP_R3_ACWDG_RCV_WORD3_BUFFER_TRANSFER_STRUCTURE_OFFSET_MASK     0x00000000FFFFFFF8ULL     // 31...3
#define PNIP_R3_ACWDG_RCV_WORD3_BUFFER_TRANSFER_STRUCTURE_OFFSET_SHIFT    3
#define PNIP_ACWDG_RCV_WORD3_DATASTART_OFFSET_MASK                        0x000000FF00000000ULL     // 39...32
#define PNIP_ACWDG_RCV_WORD3_DATASTART_OFFSET_SHIFT                       32
                                                                                                    // 40...42 reserved
#define PNIP_ACWDG_RCV_WORD3_DATALENGTH_MASK                              0x0007F80000000000ULL     // 50...43
#define PNIP_ACWDG_RCV_WORD3_DATALENGTH_SHIFT                             43
																									// 51...53 reserved
#define PNIP_R2_ACWDG_RCV_WORD3_CT_TO_RWB_MASK                            0x0040000000000000ULL     // 54
#define PNIP_R2_ACWDG_RCV_WORD3_CT_TO_RWB_SHIFT                           54
#define PNIP_ACWDG_RCV_WORD3_EN_ZERO_DATA_MASK                            0x0080000000000000ULL     // 55
#define PNIP_ACWDG_RCV_WORD3_EN_ZERO_DATA_SHIFT                           55
#define PNIP_R2_ACWDG_RCV_WORD3_CR_NUMBER_MASK                            0x1F00000000000000ULL     // 60...56
#define PNIP_R2_ACWDG_RCV_WORD3_CR_NUMBER_SHIFT                           56
#define PNIP_R3_ACWDG_RCV_WORD3_BUFFER_NUMBER_MASK                        0xFF00000000000000ULL     // 63...56
#define PNIP_R3_ACWDG_RCV_WORD3_BUFFER_NUMBER_SHIFT                       56
#define PNIP_ACWDG_RCV_WORD3_BUFFER_MODE_MASK                             0x2000000000000000ULL     // 61
#define PNIP_ACWDG_RCV_WORD3_BUFFER_MODE_SHIFT                            61
#define PNIP_R2_ACWDG_RCV_WORD3_APP_GROUP_MASK                            0xC000000000000000ULL     // 63...62
#define PNIP_R2_ACWDG_RCV_WORD3_APP_GROUP_SHIFT                           62

// constant value
#define PNIP_ACWDG_RCV_WORD3_BUFFER_MODE_NORMAL_VALUE                     0x0       // Normal Mode
#define PNIP_ACWDG_RCV_WORD3_BUFFER_MODE_EXTENSION_VALUE                  0x1       // Extension Mode
#define PNIP_ACWDG_RCV_WORD3_EN_ZERO_DATA_OFF_VALUE                       0x0       // Zero-Data in PerIF disable
#define PNIP_ACWDG_RCV_WORD3_EN_ZERO_DATA_ON_VALUE                        0x1       // Zero-Data in PerIF enable
#define PNIP_ACWDG_RCV_WORD3_LAST_RX_ZERO_VALUE                           0x0       // LastRx init value

/* ------------------------------------------------------------------------- */
/* ACW_DG Receive - ACW_Word4                                                */
/* ------------------------------------------------------------------------- */
#define PNIP_R3_ACWDG_RCV_WORD4_BUFFERLENGTH64_MASK                       0x000000000000001FULL     // 4...0
#define PNIP_R3_ACWDG_RCV_WORD4_BUFFERLENGTH64_SHIFT                      0
#define PNIP_R3_ACWDG_RCV_WORD4_DATA_BUFFER_START_OFFSET_MASK             0x00000000FFFFFFC0ULL     // 31...6
#define PNIP_R3_ACWDG_RCV_WORD4_DATA_BUFFER_START_OFFSET_SHIFT            6
#define PNIP_R3_ACWDG_RCV_WORD4_NEXTOUT_NR_P1_MASK                        0x000000FF00000000ULL     // 39...32
#define PNIP_R3_ACWDG_RCV_WORD4_NEXTOUT_NR_P1_SHIFT                       32
#define PNIP_R3_ACWDG_RCV_WORD4_NEXTOUT_NR_P2_MASK                        0x0000FF0000000000ULL     // 47...40
#define PNIP_R3_ACWDG_RCV_WORD4_NEXTOUT_NR_P2_SHIFT                       40
#define PNIP_R3_ACWDG_RCV_WORD4_NEXTOUT_NR_P3_MASK                        0x00FF000000000000ULL     // 55...48
#define PNIP_R3_ACWDG_RCV_WORD4_NEXTOUT_NR_P3_SHIFT                       48
#define PNIP_R3_ACWDG_RCV_WORD4_NEXTOUT_NR_P4_MASK                        0xFF00000000000000ULL     // 63...56
#define PNIP_R3_ACWDG_RCV_WORD4_NEXTOUT_NR_P4_SHIFT                       56

/********************************************************************************************************************/
/* ACW_DG Send                                                                                                      */
/********************************************************************************************************************/

// ACW-Words, for Rev1 - 3
typedef struct _PNIP_ACWDG_SND_TYPE     EDD_COMMON_MEM_ATTR *       PNIP_ACWDG_SND_PTR_TYPE;
PNIO_PACKED_ATTRIBUTE_PRE struct _PNIP_ACWDG_SND_TYPE
{
    LSA_UINT64                      ACW_Word1;
    LSA_UINT64                      ACW_Word2;
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _PNIP_ACWDG_SND_TYPE     PNIP_ACWDG_SND_TYPE;
/* ACWDG_Word1      : PNIP HW-struct word 1                                                         */
/* ACWDG_Word2      : PNIP HW-struct word 2                                                         */

/* ------------------------------------------------------------------------- */
/* ACW_DG Send - ACW_Word1                                                   */
/* ------------------------------------------------------------------------- */
#define PNIP_ACWDG_SND_WORD1_DG_POST_MASK                       0x00000000000000FFULL   // 7...0
#define PNIP_ACWDG_SND_WORD1_DG_POST_SHIFT                      0
#define PNIP_ACWDG_SND_WORD1_DG_LEN_MASK                        0x000000000000FF00ULL   // 15...8
#define PNIP_ACWDG_SND_WORD1_DG_LEN_SHIFT                       8
#define PNIP_R2_ACWDG_SND_WORD1_ACW_DG_NEXTOFFSET0_MASK         0x00000000FFFF0000ULL   // 31...16
#define PNIP_R3_ACWDG_SND_WORD1_ACW_DG_NEXTOFFSET0_MASK         0x000000007FFF0000ULL   // 30...16
#define PNIP_ACWDG_SND_WORD1_ACW_DG_NEXTOFFSET0_SHIFT           16
#define PNIP_R2_ACWDG_SND_WORD1_ACW_DG_NEXTOFFSET1_MASK         0x0000FFFF00000000ULL   // 47...32
#define PNIP_R3_ACWDG_SND_WORD1_ACW_DG_NEXTOFFSET1_MASK         0x00007FFF00000000ULL   // 47...32
#define PNIP_ACWDG_SND_WORD1_ACW_DG_NEXTOFFSET1_SHIFT           32
#define PNIP_ACWDG_SND_WORD1_OPCODE_MASK                        0x0003000000000000ULL   // 49...48
#define PNIP_ACWDG_SND_WORD1_OPCODE_SHIFT                       48
#define PNIP_ACWDG_SND_WORD1_ACTIVE_MASK                        0x0004000000000000ULL   // 50
#define PNIP_ACWDG_SND_WORD1_ACTIVE_SHIFT                       50
#define PNIP_ACWDG_SND_WORD1_FIRSTSEND_MASK                     0x0008000000000000ULL   // 51
#define PNIP_ACWDG_SND_WORD1_FIRSTSEND_SHIFT                    51
#define PNIP_R1_ACWDG_SND_WORD1_SCATTER_SELECT_MASK             0x0010000000000000ULL   // 52
#define PNIP_R1_ACWDG_SND_WORD1_SCATTER_SELECT_SHIFT            52
#define PNIP_R3_ACWDG_SND_WORD1_DATASTATUS_C_HANDLING_MASK      0x0010000000000000ULL   // 52
#define PNIP_R3_ACWDG_SND_WORD1_DATASTATUS_C_HANDLING_SHIFT     52
#define PNIP_ACWDG_SND_WORD1_DATASTATUS_OFFSET_MASK             0xFFE0000000000000ULL   // 63...53
#define PNIP_ACWDG_SND_WORD1_DATASTATUS_OFFSET_SHIFT            53
// constant value
#define PNIP_R2_ACWDG_SND_WORD1_ACW_DG_NEXTOFFSET_INIT_VALUE    0xFFFF      // init for ACW_DG_nextoffset0/1
#define PNIP_R3_ACWDG_SND_WORD1_ACW_DG_NEXTOFFSET_INIT_VALUE    0x7FFF      // init for ACW_DG_nextoffset0/1
#define PNIP_ACWDG_SND_WORD1_OPCODE_ACWDG_SND_VALUE             0x3         // opcode for send ACW_DG
#define PNIP_ACWDG_SND_WORD1_ACTIVE_INIT_VALUE                  0x0         // init for Active-Bit
#define PNIP_ACWDG_SND_WORD1_ACTIVE_OFF_VALUE                   0x0         // Active-Bit is OFF
#define PNIP_ACWDG_SND_WORD1_FIRSTSEND_INIT_VALUE               0x1         // init for FirstSend-Bit
#define PNIP_R1_ACWDG_SND_WORD1_SCATTER_SELECT_OFF_VALUE        0x0         // Scatter_select is OFF
#define PNIP_R3_ACWDG_SND_WORD1_DATASTATUS_C_HANDLING_OFF_VALUE 0x0         // DataStatus_C_Handling is OFF
#define PNIP_R3_ACWDG_SND_WORD1_DATASTATUS_C_HANDLING_ON_VALUE  0x1         // DataStatus_C_Handling is ON
//#define PNIP_ACWDG_SND_WORD1_DATASTATUS_OFFSET_INIT_VALUE       0xFFFF      // index to DataStatus struct is invalid
#define PNIP_ACWDG_SND_WORD1_DATASTATUS_OFFSET_INIT_VALUE       0x07FF      // index to DataStatus struct is invalid
#define PNIP_ACWDG_SND_WORD1_SCATTER_SELECT_ON_VALUE            0x1         // Scatter_select is ON

/* ------------------------------------------------------------------------- */
/* ACW_DG Send - ACW_Word2                                                   */
/* ------------------------------------------------------------------------- */
#define PNIP_R2_ACWDG_SND_WORD2_SCOREBOARD_OFFSET_MASK      0x000000000000FFFFULL   // 15...0
#define PNIP_R3_ACWDG_SND_WORD2_SCOREBOARD_OFFSET_MASK      0x0000000000007FFFULL   // 14...0
#define PNIP_ACWDG_SND_WORD2_SCOREBOARD_OFFSET_SHIFT        0
#define PNIP_R2_ACWDG_SND_WORD2_DATA_PTR_MASK               0x0000FFFFFFFF0000ULL   // 47...16
#define PNIP_R2_ACWDG_SND_WORD2_DATA_PTR_SHIFT              16
#define PNIP_R2_ACWDG_SND_WORD2_DATALENGTH_MASK             0x00FF000000000000ULL   // 55...48
#define PNIP_R2_ACWDG_SND_WORD2_DATALENGTH_SHIFT            48
#define PNIP_R2_ACWDG_SND_WORD2_CR_NUMBER_MASK              0x1F00000000000000ULL   // 60...56
#define PNIP_R2_ACWDG_SND_WORD2_CR_NUMBER_SHIFT             56
#define PNIP_R2_ACWDG_SND_WORD2_BUFFER_MODE_MASK            0x2000000000000000ULL   // 61
#define PNIP_R2_ACWDG_SND_WORD2_BUFFER_MODE_SHIFT           61
#define PNIP_R2_ACWDG_SND_WORD2_APP_GROUP_MASK              0xC000000000000000ULL   // 63...62
#define PNIP_R2_ACWDG_SND_WORD2_APP_GROUP_SHIFT             62
// constant value
#define PNIP_R2_ACWDG_SND_WORD2_SCOREBOARD_OFFSET_OFF_VALUE 0xFFFF      // control of Timer-Scoreboard is off
#define PNIP_R3_ACWDG_SND_WORD2_SCOREBOARD_OFFSET_OFF_VALUE 0x7FFF      // control of Timer-Scoreboard is off
#define PNIP_ACWDG_SND_WORD2_BUFFER_MODE_NORMAL_VALUE       0x0         // BufferMode Normal


/********************************************************************************************************************/
/* ACW Receive                                                                                                      */
/********************************************************************************************************************/

typedef struct _PNIP_ACW_RCV_TYPE   EDD_COMMON_MEM_ATTR *   PNIP_ACW_RCV_PTR_TYPE;
PNIO_PACKED_ATTRIBUTE_PRE struct _PNIP_ACW_RCV_TYPE
{
    LSA_UINT64                  ACW_Word1;
    LSA_UINT64                  ACW_Word2;
    LSA_UINT64                  ACW_Word3;
    LSA_UINT64                  ACW_Word4;
    PNIP_ACW_RCV_CRT_DATA_TYPE  CpmSwData;
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _PNIP_ACW_RCV_TYPE   PNIP_ACW_RCV_TYPE;
/* ACW_Word1        : PNIP HW-struct word 1                                                         */
/* ACW_Word2        : PNIP HW-struct word 2                                                         */
/* ACW_Word3        : PNIP HW-struct word 3                                                         */
/* ACW_Word4        : PNIP HW-struct word 4                                                         */
/* CpmSwData        : SW data for internal SW mgmt refencing                                        */

/* ------------------------------------------------------------------------- */
/* ACW Receive - ACW_Word1                                                   */
/* ------------------------------------------------------------------------- */
#define PNIP_ACW_RCV_WORD1_FRAMEID_MASK                         0x000000000000FFFFULL   // 15...0
#define PNIP_ACW_RCV_WORD1_FRAMEID_SHIFT                        0
#define PNIP_R2_ACW_RCV_WORD1_ACW_NEXTOFFSET_MASK               0x00000000FFFF0000ULL   // 31...16
#define PNIP_R3_ACW_RCV_WORD1_ACW_NEXTOFFSET_MASK               0x000000007FFF0000ULL   // 30...16
#define PNIP_ACW_RCV_WORD1_ACW_NEXTOFFSET_SHIFT                 16
                                                                                        // 31 reserved in HERA
#define PNIP_R2_ACW_RCV_WORD1_ACW_DG_OFFSET_MASK                0x0000FFFF00000000ULL   // 47...32
#define PNIP_R3_ACW_RCV_WORD1_ACW_DG_OFFSET_MASK                0x00007FFF00000000ULL   // 46...32
#define PNIP_ACW_RCV_WORD1_ACW_DG_OFFSET_SHIFT                  32
                                                                                        // 47 reserved in HERA
#define PNIP_ACW_RCV_WORD1_OPCODE_MASK                          0x0003000000000000ULL   // 49...48
#define PNIP_ACW_RCV_WORD1_OPCODE_SHIFT                         48
#define PNIP_ACW_RCV_WORD1_ACTIVE_MASK                          0x0004000000000000ULL   // 50
#define PNIP_ACW_RCV_WORD1_ACTIVE_SHIFT                         50
#define PNIP_ACW_RCV_WORD1_ONESHOT_BIT_MASK                     0x0008000000000000ULL   // 51
#define PNIP_ACW_RCV_WORD1_ONESHOT_BIT_SHIFT                    51
#define PNIP_ACW_RCV_WORD1_DP2_AFTER_RED_BIT_MASK               0x0010000000000000ULL   // 52
#define PNIP_ACW_RCV_WORD1_DP2_AFTER_RED_BIT_SHIFT              52
#define PNIP_ACW_RCV_WORD1_DP2_ONLY_BIT_MASK                    0x0020000000000000ULL	// 53
#define PNIP_ACW_RCV_WORD1_DP2_ONLY_BIT_SHIFT                   53
#define PNIP_ACW_RCV_WORD1_ETHERTYPE_SELECT_MASK                0x00C0000000000000ULL   // 55...54
#define PNIP_ACW_RCV_WORD1_ETHERTYPE_SELECT_SHIFT               54
#define PNIP_ACW_RCV_WORD1_FID_REDUNDANT_MASK                   0x0100000000000000ULL   // 56
#define PNIP_ACW_RCV_WORD1_FID_REDUNDANT_SHIFT                  56
#define PNIP_ACW_RCV_WORD1_DG_RCV_MODE_MASK                     0x0200000000000000ULL   // 57
#define PNIP_ACW_RCV_WORD1_DG_RCV_MODE_SHIFT                    57
#define PNIP_ACW_RCV_WORD1_EN_CSA_MASK                          0x0400000000000000ULL   // 58
#define PNIP_ACW_RCV_WORD1_EN_CSA_SHIFT                         58
#define PNIP_ACW_RCV_WORD1_RT_CLASS_MASK                        0x0800000000000000ULL   // 59
#define PNIP_ACW_RCV_WORD1_RT_CLASS_SHIFT                       59
#define PNIP_ACW_RCV_WORD1_ACW_DG_ORDER_MASK                    0x1000000000000000ULL   // 60
#define PNIP_ACW_RCV_WORD1_ACW_DG_ORDER_SHIFT                   60
#define PNIP_R3_ACW_RCV_WORD1_IPV6_MASK                         0x1000000000000000ULL   // 60
#define PNIP_R3_ACW_RCV_WORD1_IPV6_SHIFT                        60
#define PNIP_ACW_RCV_WORD1_PACK_WO_DGCC_MASK                    0x2000000000000000ULL   // 61
#define PNIP_ACW_RCV_WORD1_PACK_WO_DGCC_SHIFT                   61
                                                                                    // 62..63 reserved
// constant value
#define PNIP_R2_ACW_RCV_WORD1_ACW_NEXTOFFSET_INIT_VALUE         0xFFFF      // init for ACW_nextoffset
#define PNIP_R3_ACW_RCV_WORD1_ACW_NEXTOFFSET_INIT_VALUE         0x7FFF      // init for ACW_nextoffset
#define PNIP_R2_ACW_RCV_WORD1_ACW_DG_OFFSET_INIT_VALUE          0xFFFF      // init for ACW_DG_offset
#define PNIP_R3_ACW_RCV_WORD1_ACW_DG_OFFSET_INIT_VALUE          0x7FFF      // init for ACW_DG_offset
#define PNIP_ACW_RCV_WORD1_OPCODE_ACW_VALUE                     0x0         // opcode for receive ACW
#define PNIP_ACW_RCV_WORD1_ACTIVE_INIT_VALUE                    0x0         // init for Active-Bit
#define PNIP_ACW_RCV_WORD1_ONESHOT_BIT_INIT_VALUE               0x0         // init for Oneshot-Bit
#define PNIP_ACW_RCV_WORD1_DP2_AFTER_RED_OFF_VALUE              0x0         // no auto sync red
#define PNIP_ACW_RCV_WORD1_DP2_AFTER_RED_ON_VALUE               0x1         // HW sets DP2_ONLY on first red frame
#define PNIP_ACW_RCV_WORD1_DP2_ONLY_OFF_VALUE                   0x0         // red + green frames
#define PNIP_ACW_RCV_WORD1_DP2_ONLY_ON_VALUE                    0x1         // only red frames
#define PNIP_ACW_RCV_WORD1_ETHERTYPE_SELECT_INDEX0_VALUE        0x0         // Index 0 to select the Register "Local_SPort_Etype_0"
#define PNIP_ACW_RCV_WORD1_ETHERTYPE_SELECT_INDEX1_VALUE        0x1         // Index 1 to select the Register "Local_SPort_Etype_1"
#define PNIP_ACW_RCV_WORD1_ETHERTYPE_SELECT_INDEX2_VALUE        0x2         // Index 2 to select the Register "Local_SPort_Etype_2"
#define PNIP_ACW_RCV_WORD1_ETHERTYPE_SELECT_INDEX3_VALUE        0x3         // Index 3 to select the Register "Local_SPort_Etype_3"
#define PNIP_ACW_RCV_WORD1_FID_REDUNDANT_OFF_VALUE              0x0         // deactivate the redundant frames
#define PNIP_ACW_RCV_WORD1_FID_REDUNDANT_ON_VALUE               0x1         // activate the redundant frames
#define PNIP_ACW_RCV_WORD1_DG_RCV_MODE_OFF_VALUE                0x0         // pack frame contains more datagrams than ACW_DGs
#define PNIP_ACW_RCV_WORD1_DG_RCV_MODE_ON_VALUE                 0x1         // pack frame contains equal or less datagrams than ACW_DGs
#define PNIP_ACW_RCV_WORD1_EN_CSA_OFF_VALUE                     0x0         // disable check source address
#define PNIP_ACW_RCV_WORD1_EN_CSA_ON_VALUE                      0x1         // enable check source address
#define PNIP_ACW_RCV_WORD1_RT_CLASS_RT_VALUE                    0x0         // RT_class = RT-Frame
#define PNIP_ACW_RCV_WORD1_RT_CLASS_UDP_VALUE                   0x1         // RT_class = RToverUDP-Frame
#define PNIP_ACW_RCV_WORD1_ACW_DG_ORDER_UP_VALUE                0x0         // ascending order for sending of ACW_DGs
#define PNIP_ACW_RCV_WORD1_ACW_DG_ORDER_DOWN_VALUE              0x1         // descending order for sending of ACW_DGs
#define PNIP_ACW_RCV_WORD1_IPV4_VALUE                           0x0         // IPv4
#define PNIP_ACW_RCV_WORD1_IPV6_VALUE                           0x1         // IPv6
#define PNIP_ACW_RCV_WORD1_PACK_WO_DGCC_OFF_VALUE               0x1         // without Chksum
#define PNIP_ACW_RCV_WORD1_PACK_WO_DGCC_ON_VALUE                0x0         // with Chksum

/* ------------------------------------------------------------------------- */
/* ACW Receive - ACW_Word2                                                   */
/* ------------------------------------------------------------------------- */
//------------------------------------------------------------------------------------------------------------------------------------
#define PNIP_ACW_RCV_WORD2_MAC_SA_ALL_MASK                  0x0000FFFFFFFFFFFFULL		// 47...0   | 47...0  MAC_SA_5_0
#define PNIP_ACW_RCV_WORD2_MAC_SA_ALL_SHIFT                 0							//          |
#define PNIP_ACW_RCV_WORD2_MAC_SA_0_MASK                    0x00000000000000FFULL		// 7...0    | 47...0  MAC_SA_5_0
#define PNIP_ACW_RCV_WORD2_MAC_SA_0_SHIFT                   0							//          |
#define PNIP_ACW_RCV_WORD2_MAC_SA_1_MASK                    0x000000000000FF00ULL		// 15...8   |
#define PNIP_ACW_RCV_WORD2_MAC_SA_1_SHIFT                   8							//          |
#define PNIP_ACW_RCV_WORD2_MAC_SA_2_MASK                    0x0000000000FF0000ULL		// 23...16  |
#define PNIP_ACW_RCV_WORD2_MAC_SA_2_SHIFT                   16							//          |
#define PNIP_ACW_RCV_WORD2_MAC_SA_3_MASK                    0x00000000FF000000ULL		// 31...24  |
#define PNIP_ACW_RCV_WORD2_MAC_SA_3_SHIFT                   24							//          |
#define PNIP_ACW_RCV_WORD2_MAC_SA_4_MASK                    0x000000FF00000000ULL		// 39...32  |
#define PNIP_ACW_RCV_WORD2_MAC_SA_4_SHIFT                   32							//          |
#define PNIP_ACW_RCV_WORD2_MAC_SA_5_MASK                    0x0000FF0000000000ULL		// 47...40  |
#define PNIP_ACW_RCV_WORD2_MAC_SA_5_SHIFT                   40							//         \|/
//------------------------------------------------------------------------------------------------------------------------------------
#define PNIP_ACW_RCV_WORD2_SRCIP_ADDRESS_0_MASK             0x00000000000000FFULL		// 7...0    | 31...0  SrcIP_Address_3_0
#define PNIP_ACW_RCV_WORD2_SRCIP_ADDRESS_0_SHIFT            0							//          |
#define PNIP_ACW_RCV_WORD2_SRCIP_ADDRESS_1_MASK             0x000000000000FF00ULL		// 15...8   |
#define PNIP_ACW_RCV_WORD2_SRCIP_ADDRESS_1_SHIFT            8							//          |
#define PNIP_ACW_RCV_WORD2_SRCIP_ADDRESS_2_MASK             0x0000000000FF0000ULL		// 23...16  |
#define PNIP_ACW_RCV_WORD2_SRCIP_ADDRESS_2_SHIFT            16							//          |
#define PNIP_ACW_RCV_WORD2_SRCIP_ADDRESS_3_MASK             0x00000000FF000000ULL		// 31...24  |
#define PNIP_ACW_RCV_WORD2_SRCIP_ADDRESS_3_SHIFT            24							//         \|/
//------------------------------------------------------------------------------------------------------------------------------------
#define PNIP_ACW_RCV_WORD2_UDP_SP_MASK                      0x0000FFFF00000000ULL		// 47...32  | 47...32 UDP_SP
#define PNIP_ACW_RCV_WORD2_UDP_SP_SHIFT                     32							//         \|/
//------------------------------------------------------------------------------------------------------------------------------------
#define PNIP_ACW_RCV_WORD2_LENGTH_MASK                      0x07FF000000000000ULL		// 58...48
#define PNIP_ACW_RCV_WORD2_LENGTH_SHIFT                     48
#define PNIP_ACW_RCV_WORD2_IP_DA_SELECT_MASK                0xF800000000000000ULL       // 63...59
#define PNIP_ACW_RCV_WORD2_IP_DA_SELECT_SHIFT               59
#define PNIP_R3_ACW_RCV_WORD2_RXPORT_PACK_MASK              0x7800000000000000ULL       // 62...59
#define PNIP_R3_ACW_RCV_WORD2_RXPORT_PACK_SHIFT             59
// constant value
#define PNIP_ACW_RCV_WORD2_IP_DA_SELECT_INDEX0_VALUE        0x0     // Index of Register Local_IP_SA_0-3

/* ------------------------------------------------------------------------- */
/* ACW Receive - ACW_Word3                                                   */
/* ------------------------------------------------------------------------- */
#define PNIP_R2_ACW_RCV_WORD3_SCOREBOARD_OFFSET_MASK                0x000000000000FFFFULL   // 15...0
#define PNIP_R3_ACW_RCV_WORD3_SCOREBOARD_OFFSET_MASK                0x0000000000007FFFULL   // 14...0
#define PNIP_ACW_RCV_WORD3_SCOREBOARD_OFFSET_SHIFT                  0
																							// 15 is reserved with HERA
#define PNIP_R2_ACW_RCV_WORD3_APDUSTATUS_OFFSET_MASK                0x00000001FFFF0000ULL   // 32...16
#define PNIP_R3_ACW_RCV_WORD3_APDUSTATUS_OFFSET_MASK                0x00000000FFFF0000ULL   // 31...16
#define PNIP_ACW_RCV_WORD3_APDUSTATUS_OFFSET_SHIFT                  16
																							// 32 is reserved with HERA
#define PNIP_R3_ACW_RCV_WORD3_RXPORT_NON_PACK_MASK                  0x0000001E00000000ULL   // 36...33
#define PNIP_R3_ACW_RCV_WORD3_RXPORT_NON_PACK_SHIFT                 33
																    					    // 38...33 reserved for ERTEC200P
																							// 38...37 reserved for HERA

#define PNIP_ACW_RCV_WORD3_DP2_CHECK_MASK                           0x0000008000000000ULL   // 39
#define PNIP_ACW_RCV_WORD3_DP2_CHECK_SHIFT                          39
#define PNIP_ACW_RCV_WORD3_WD_RELOADVALUE_MASK                      0x0000FF0000000000ULL   // 47...40
#define PNIP_ACW_RCV_WORD3_WD_RELOADVALUE_SHIFT                     40
#define PNIP_ACW_RCV_WORD3_DH_RELOADVALUE_MASK                      0x00FF000000000000ULL   // 55...48
#define PNIP_ACW_RCV_WORD3_DH_RELOADVALUE_SHIFT                     48
#define PNIP_ACW_RCV_WORD3_MR_RELOADVALUE_MASK                      0xFF00000000000000ULL   // 63...56
#define PNIP_ACW_RCV_WORD3_MR_RELOADVALUE_SHIFT                     56
// constant value											    
#define PNIP_R2_ACW_RCV_WORD3_SCOREBOARD_OFFSET_OFF_VALUE           0xFFFF      // no evaluating of SB
#define PNIP_R3_ACW_RCV_WORD3_SCOREBOARD_OFFSET_OFF_VALUE           0x7FFF      // no evaluating of SB
#define PNIP_R2_ACW_RCV_WORD3_APDUSTATUS_OFFSET_OFF_VALUE           0x1FFFF     // no evaluating of APDU-Status
#define PNIP_R3_ACW_RCV_WORD3_APDUSTATUS_OFFSET_OFF_VALUE           0xFFFF      // no evaluating of APDU-Status
#define PNIP_R3_ACW_RCV_WORD3_PXPORT1_ON_VALUE                      0x1         // port1 receive enabled
#define PNIP_R3_ACW_RCV_WORD3_PXPORT2_ON_VALUE                      0x2         // port2 receive enabled
#define PNIP_R3_ACW_RCV_WORD3_PXPORT3_ON_VALUE                      0x4         // port3 receive enabled
#define PNIP_R3_ACW_RCV_WORD3_PXPORT4_ON_VALUE                      0x8         // port4 receive enabled
#define PNIP_R3_ACW_RCV_WORD3_PXPORTALL_OFF_VALUE                   0x0         // port4 receive enabled

#define PNIP_ACW_RCV_WORD3_DP2_CHECK_OFF_VALUE                      0x0         // monitoring of Consumer-Scoreboard OFF
#define PNIP_ACW_RCV_WORD3_DP2_CHECK_ON_VALUE                       0x1         // monitoring of Consumer-Scoreboard ON
#define PNIP_ACW_RCV_WORD3_MR_RELOADVALUE_OFF_VALUE                 0xFF        // MediaRedundancy-Timer in Timer-Scoreboard is off

/* ------------------------------------------------------------------------- */
/* ACW Receive - ACW_Word4                                                   */
/* ------------------------------------------------------------------------- */
#define PNIP_R2_ACW_RCV_WORD4_DATA_PTR_MASK                     0x00000000FFFFFFFFULL   // 31...0
#define PNIP_R2_ACW_RCV_WORD4_DATA_PTR_SHIFT                    0
#define PNIP_R3_ACW_RCV_WORD4_MEM_TYPE_MASK                     0x0000000000000001ULL   // 0
#define PNIP_R3_ACW_RCV_WORD4_MEM_TYPE_SHIFT                    0
#define PNIP_R3_ACW_RCV_WORD4_LAST_RX_MASK                      0x0000000000000006ULL   // 2...1
#define PNIP_R3_ACW_RCV_WORD4_LAST_RX_SHIFT                     1
#define PNIP_R3_ACW_RCV_WORD4_BUFFERTRANS_STRUCT_OFFSET_MASK    0x00000000FFFFFFF8ULL   // 31...3
#define PNIP_R3_ACW_RCV_WORD4_BUFFERTRANS_STRUCT_OFFSET_SHIFT   3
#define PNIP_ACW_RCV_WORD4_DATASTART_OFFSET_MASK                0x000007FF00000000ULL   // 42...32
#define PNIP_ACW_RCV_WORD4_DATASTART_OFFSET_SHIFT               32
#define PNIP_ACW_RCV_WORD4_DATALENGTH_MASK                      0x003FF80000000000ULL   // 53...43
#define PNIP_ACW_RCV_WORD4_DATALENGTH_SHIFT                     43
#define PNIP_R2_ACW_RCV_WORD4_CT_TO_RWB_MASK                    0x0040000000000000ULL   // 54
#define PNIP_R2_ACW_RCV_WORD4_CT_TO_RWB_SHIFT                   54
#define PNIP_ACW_RCV_WORD4_EN_ZERO_DATA_MASK                    0x0080000000000000ULL   // 55
#define PNIP_ACW_RCV_WORD4_EN_ZERO_DATA_SHIFT                   55
#define PNIP_R2_ACW_RCV_WORD4_CR_NUMBER_MASK                    0x1F00000000000000ULL   // 60...56
#define PNIP_R2_ACW_RCV_WORD4_CR_NUMBER_SHIFT                   56
#define PNIP_R3_ACW_RCV_WORD4_BUFFER_NUMBER_MASK                0xFF00000000000000ULL   // 63..56
#define PNIP_R3_ACW_RCV_WORD4_BUFFER_NUMBER_SHIFT               56
#define PNIP_ACW_RCV_WORD4_BUFFER_MODE_MASK                     0x2000000000000000ULL   // 61
#define PNIP_ACW_RCV_WORD4_BUFFER_MODE_SHIFT                    61
#define PNIP_ACW_RCV_WORD4_APP_GROUP_MASK                       0xC000000000000000ULL   // 63...62
#define PNIP_ACW_RCV_WORD4_APP_GROUP_SHIFT                      62
// constant value                                               
#define PNIP_ACW_RCV_WORD4_BUFFER_MODE_NORMAL_VALUE             0x0     // Normal Mode
#define PNIP_ACW_RCV_WORD4_BUFFER_MODE_EXTENSION_VALUE          0x1     // Extension Mode
#define PNIP_ACW_RCV_WORD4_EN_ZERO_DATA_OFF_VALUE               0x0     // Zero-Data in PerIF disable
#define PNIP_ACW_RCV_WORD4_EN_ZERO_DATA_ON_VALUE                0x1     // Zero-Data in PerIF enable

/* ------------------------------------------------------------------------- */
/* ACW Receive - ACW_Word5 for HERA                                          */
/* ------------------------------------------------------------------------- */
#define PNIP_R3_ACW_RCV_WORD5_BUFFER_LENGTH_MASK               0x000000000000001FULL   // 4...0
#define PNIP_R3_ACW_RCV_WORD5_BUFFER_LENGTH_SHIFT              0
#define PNIP_R3_ACW_RCV_WORD5_DATA_BUFFER_START_OFFSET_MASK    0x00000000FFFFFFC0ULL   // 31...6
#define PNIP_R3_ACW_RCV_WORD5_DATA_BUFFER_START_OFFSET_SHIFT   6
#define PNIP_R3_ACW_RCV_WORD5_NEXTOUT_NR_P1_MASK               0x000000FF00000000ULL   // 39...32
#define PNIP_R3_ACW_RCV_WORD5_NEXTOUT_NR_P1_SHIFT              32
#define PNIP_R3_ACW_RCV_WORD5_NEXTOUT_NR_P2_MASK               0x0000FF0000000000ULL   // 47...40
#define PNIP_R3_ACW_RCV_WORD5_NEXTOUT_NR_P2_SHIFT              40
#define PNIP_R3_ACW_RCV_WORD5_NEXTOUT_NR_P3_MASK               0x00FF000000000000ULL   // 55...48
#define PNIP_R3_ACW_RCV_WORD5_NEXTOUT_NR_P3_SHIFT              48
#define PNIP_R3_ACW_RCV_WORD5_NEXTOUT_NR_P4_MASK               0xFF00000000000000ULL   // 63...56
#define PNIP_R3_ACW_RCV_WORD5_NEXTOUT_NR_P4_SHIFT              56

/* ------------------------------------------------------------------------- */
/* ACW Receive - ACW_Word6 for HERA                                          */
/* ------------------------------------------------------------------------- */

//------------------------------------------------------------------------------------------------------------------------------------
#define PNIP_R3_ACW_RCV_WORD6_SOURCEC_IP_ADDRESS2_MASK          0xFFFFFFFFFFFFFFFFULL   // 63...0
#define PNIP_R3_ACW_RCV_WORD6_SOURCEC_IP_ADDRESS2_SHIFT         0

//------------------------------------------------------------------------------------------------------------------------------------

#define PNIP_R3_ACW_RCV_WORD6_SRCIP_ADDRESS2_0_MASK             0x00000000000000FFULL		// 7...0    | 63...0  SrcIP_Address2_7_0
#define PNIP_R3_ACW_RCV_WORD6_SRCIP_ADDRESS2_0_SHIFT            0							//          |
#define PNIP_R3_ACW_RCV_WORD6_SRCIP_ADDRESS2_1_MASK             0x000000000000FF00ULL		// 15...8   |
#define PNIP_R3_ACW_RCV_WORD6_SRCIP_ADDRESS2_1_SHIFT            8							//          |
#define PNIP_R3_ACW_RCV_WORD6_SRCIP_ADDRESS2_2_MASK             0x0000000000FF0000ULL		// 23...16  |
#define PNIP_R3_ACW_RCV_WORD6_SRCIP_ADDRESS2_2_SHIFT            16							//          |
#define PNIP_R3_ACW_RCV_WORD6_SRCIP_ADDRESS2_3_MASK             0x00000000FF000000ULL		// 31...24  |
#define PNIP_R3_ACW_RCV_WORD6_SRCIP_ADDRESS2_3_SHIFT            24							//          |
#define PNIP_R3_ACW_RCV_WORD6_SRCIP_ADDRESS2_4_MASK             0x000000FF00000000ULL		// 39...32  |
#define PNIP_R3_ACW_RCV_WORD6_SRCIP_ADDRESS2_4_SHIFT            32							//          |
#define PNIP_R3_ACW_RCV_WORD6_SRCIP_ADDRESS2_5_MASK             0x0000FF0000000000ULL		// 47...40  |
#define PNIP_R3_ACW_RCV_WORD6_SRCIP_ADDRESS2_5_SHIFT            40							//          |
#define PNIP_R3_ACW_RCV_WORD6_SRCIP_ADDRESS2_6_MASK             0x00FF000000000000ULL		// 55...48  |
#define PNIP_R3_ACW_RCV_WORD6_SRCIP_ADDRESS2_6_SHIFT            48							//          |
#define PNIP_R3_ACW_RCV_WORD6_SRCIP_ADDRESS2_7_MASK             0xFF00000000000000ULL		// 63...56  |
#define PNIP_R3_ACW_RCV_WORD6_SRCIP_ADDRESS2_7_SHIFT            56							//         \|/

/* ------------------------------------------------------------------------- */
/* ACW Receive - ACW_Word7 for HERA                                          */
/* ------------------------------------------------------------------------- */

//------------------------------------------------------------------------------------------------------------------------------------
#define PNIP_R3_ACW_RCV_WORD7_SOURCEC_IP_ADDRESS3_MASK         0x00000000FFFFFFFFULL   // 31...0
#define PNIP_R3_ACW_RCV_WORD7_SOURCEC_IP_ADDRESS3_SHIFT        0


//------------------------------------------------------------------------------------------------------------------------------------
#define PNIP_R3_ACW_RCV_WORD7_SRCIP_ADDRESS3_0_MASK             0x00000000000000FFULL		// 7...0    | 63...0  SrcIP_Address3_7_0
#define PNIP_R3_ACW_RCV_WORD7_SRCIP_ADDRESS3_0_SHIFT            0							//          |
#define PNIP_R3_ACW_RCV_WORD7_SRCIP_ADDRESS3_1_MASK             0x000000000000FF00ULL		// 15...8   |
#define PNIP_R3_ACW_RCV_WORD7_SRCIP_ADDRESS3_1_SHIFT            8							//          |
#define PNIP_R3_ACW_RCV_WORD7_SRCIP_ADDRESS3_2_MASK             0x0000000000FF0000ULL		// 23...16  |
#define PNIP_R3_ACW_RCV_WORD7_SRCIP_ADDRESS3_2_SHIFT            16							//          |
#define PNIP_R3_ACW_RCV_WORD7_SRCIP_ADDRESS3_3_MASK             0x00000000FF000000ULL		// 31...24  |
#define PNIP_R3_ACW_RCV_WORD7_SRCIP_ADDRESS3_3_SHIFT            24							//         \|/

/* ------------------------------------------------------------------------- */
/* ACW APDU - the link to this memory is in ACW receive entry                */
/* ------------------------------------------------------------------------- */
typedef struct _PNIP_ADPU_RCV_TYPE     EDD_COMMON_MEM_ATTR *   PNIP_ADPU_RCV_PTR_TYPE;
PNIO_PACKED_ATTRIBUTE_PRE struct _PNIP_ADPU_RCV_TYPE
{
    LSA_UINT16          CycleCounter;       // used for old/new detection
    LSA_UINT8           DataStatus;         // some indication flags
    LSA_UINT8           TransferStatus;     // nonzero for error
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _PNIP_ADPU_RCV_TYPE  PNIP_ADPU_RCV_TYPE;
/* CycleCounter     : used for old/new detection                                                        */
/* DataStatus       : some indication flags                                                             */
/* DataStatus:                                                                                          */
/*      - Bit(7)    : Ignore:                 1=do'nt use APDU (from packed frames)                     */
/*      - Bit(6)    : -                                                                                 */
/*      - Bit(5)    : ProblemIndicatior:      Normal Operation (=1) or Problem detected (=0)            */
/*      - Bit(4)    : ProviderState:          Run (=1) oder Stop (=0)                                   */
/*      - Bit(3)    : -                                                                                 */
/*      - Bit(2)    : DataValid:              valid(=1) or invalid(=0)                                  */
/*      - Bit(1)    : SystemRedundancy:       guarding in scorboard                                     */
/*      - Bit(0)    : State:                  Primary(=1) or Backup(=0)                                 */
/* Masks for DataStatus (LSA_UINT8)                                                                     */
#define PNIP_ADPU_RCV_IGNORE_MASK                   0x80        // Bit(7)
#define PNIP_ADPU_RCV_IGNORE_SHIFT                  7
  #define PNIP_ADPU_RCV_IGNORE_APDU                   0x1       // do'nt use APDU
#define PNIP_ADPU_RCV_PROBLEMIND_MASK               0x20        // Bit(5)
#define PNIP_ADPU_RCV_PROBLEMIND_SHIFT              5
  #define PNIP_ADPU_RCV_PROBLEMIND_NORMAL             0x1       // Normal Operation
  #define PNIP_ADPU_RCV_PROBLEMIND_DETECTED           0x0       // Problem detected
#define PNIP_ADPU_RCV_PROVSTATE_MASK                0x10        // Bit(4)
#define PNIP_ADPU_RCV_PROVSTATE_SHIFT               4
  #define PNIP_ADPU_RCV_PROVSTATE_RUN                 0x1       // Run
  #define PNIP_ADPU_RCV_PROVSTATE_STOP                0x0       // Stop
#define PNIP_ADPU_RCV_DATAVALID_MASK                0x4         // Bit(2)
#define PNIP_ADPU_RCV_DATAVALID_SHIFT               2
  #define PNIP_ADPU_RCV_DATAVALID_YES                 0x1       // valid
  #define PNIP_ADPU_RCV_DATAVALID_NO                  0x0       // invalid
#define PNIP_ADPU_RCV_SYSRED_MASK                   0x2         // Bit(1)
#define PNIP_ADPU_RCV_SYSRED_SHIFT                  1
#define PNIP_ADPU_RCV_STATE_MASK                    0x1         // Bit(0)
#define PNIP_ADPU_RCV_STATE_SHIFT                   0
  #define PNIP_ADPU_RCV_STATE_PRIMARY                 0x1       // Primary
  #define PNIP_ADPU_RCV_STATE_BACKUP                  0x0       // Backup
/* TransferStatus   : nonzero for error                                                                 */


/* ------------------------------------------------------------------------- */
/* ACW_DG Receive                                                            */
/* ------------------------------------------------------------------------- */
typedef struct _PNIP_ACW_DG_RCV_TYPE   EDD_COMMON_MEM_ATTR *   PNIP_ACW_DG_RCV_PTR_TYPE;
PNIO_PACKED_ATTRIBUTE_PRE struct _PNIP_ACW_DG_RCV_TYPE
{
    LSA_UINT64                  ACWDG_Word1;
    LSA_UINT64                  ACWDG_Word2;
    LSA_UINT64                  ACWDG_Word3;
    PNIP_ACW_RCV_CRT_DATA_TYPE  CpmSwData;
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _PNIP_ACW_DG_RCV_TYPE    PNIP_ACW_DG_RCV_TYPE;


/* ------------------------------------------------------------------------- */
/* CPM TSB - the link to this memory is in pTSB entry                        */
/* ------------------------------------------------------------------------- */
typedef struct _PNIP_CPM_TSB_TYPE     EDD_COMMON_MEM_ATTR *   PNIP_CPM_TSB_PTR_TYPE;
PNIO_PACKED_ATTRIBUTE_PRE struct _PNIP_CPM_TSB_TYPE
{
    LSA_UINT64          TSB_Word1;  // used for old/new detection
    LSA_UINT64          TSB_Word2;  // used for old/new detection
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _PNIP_CPM_TSB_TYPE   PNIP_CPM_TSB_TYPE;
/* TSB_Word1    : configuration word 1                                                                  */
/* TSB_Word2    : configuration word 2                                                                  */

/* TSB_Word1:                                                                                           */
/*  - Bit( 0..18)   : TSBRR_Counter         : Timer-Scoreboard-Reduction-Ratio                          */
/*  - Bit(19)       : DS_State_changed      : indication for change to TSB.DS_State                     */
/*  - Bit(20)       : DS_SystemRedundancy   : indication for change to TSB.DS_State                     */
/*                     _changed                                                                         */
/*  - Bit(21)       : DS_RunStop_changed    : indication for change to TSB.DS_State                     */
/*  - Bit(22)       : DS_ProblemIndicator   : indication for change to TSB.DS_State                     */
/*                     _changed                                                                         */
/*  - Bit(23)       : Received_in_DP2       : indication for changing of red phase (DP2)                */
/*                     _changed                                                                         */
/*  - Bit(24)       : DS_State              : bit from APDU.DataStatus                                  */
/*  - Bit(25)       : DS_SystemRedundancy   : bit from APDU.DataStatus                                  */
/*  - Bit(26)       : DS_RunStop            : bit from APDU.DataStatus                                  */
/*  - Bit(27)       : DS_ProblemIndicator   : bit from APDU.DataStatus                                  */
/*  - Bit(28)       : Received_in_DP2       : set if received in red phase (DP2)                        */
/*  - Bit(29..30)   : reserved              : -                                                         */
/*  - Bit(31)       : Autostop_en           : set if frame received, without old/new detection          */
/*  - Bit(32)       : WDT_unknown           : flag for WatchDog-Timer                                   */
/*  - Bit(33)       : DHT_unknown           : flag for DataHold-Timer                                   */
/*  - Bit(34)       : MRT_unknown           : flag for MediaRedundancy-Timer                            */
/*  - Bit(35)       : WDT_expired           : state of WatchDog-Timer                                   */
/*  - Bit(36)       : DHT_expired           : state of DataHold-Timer                                   */
/*  - Bit(37)       : MRT_expired           : state of MediaRedundancy--Timer                           */
/*  - Bit(38)       : Frame_received        : set if frame received, without old/new detection          */
/*  - Bit(39)       : Valid                 : Timer-Scoreboard valid                                    */
/*  - Bit(40..47)   : WD_Timer              : Watchdog-Timer                                            */
/*  - Bit(48..55)   : DH_Timer              : DataHold-Timer                                            */
/*  - Bit(56..63)   : MR_Timer              : MediaRedundancy-Timer                                     */

/* ------------------------------------------------------------------------- */
/* CPM_TSB - masks for TSB_Word1 (LSA_UINT64)                                */
/* ------------------------------------------------------------------------- */
#define PNIP_CPM_TSB_WORD1_MR_TIMER_MASK            0xFF00000000000000ULL   // Bit(56..63)
  #define PNIP_CPM_TSB_WORD1_MR_TIMER_SHIFT           56
  #define PNIP_CPM_TSB_WORD1_MR_TIMER_OFF             0xFF
#define PNIP_CPM_TSB_WORD1_DH_TIMER_MASK            0x00FF000000000000ULL   // Bit(48..55)
  #define PNIP_CPM_TSB_WORD1_DH_TIMER_SHIFT           48
  #define PNIP_CPM_TSB_WORD1_DH_TIMER_OFF             0xFF
#define PNIP_CPM_TSB_WORD1_WD_TIMER_MASK            0x0000FF0000000000ULL   // Bit(40..47)
  #define PNIP_CPM_TSB_WORD1_WD_TIMER_SHIFT           40
  #define PNIP_CPM_TSB_WORD1_WD_TIMER_OFF             0xFF
#define PNIP_CPM_TSB_WORD1_VALID_MASK               0x0000008000000000ULL   // Bit(39)
  #define PNIP_CPM_TSB_WORD1_VALID_SHIFT              39
#define PNIP_CPM_TSB_WORD1_FRM_RCVD_MASK            0x0000004000000000ULL   // Bit(38)
  #define PNIP_CPM_TSB_WORD1_FRM_RCVD_SHIFT           38
#define PNIP_CPM_TSB_WORD1_MRT_EXPIRED_MASK         0x0000002000000000ULL   // Bit(37)
  #define PNIP_CPM_TSB_WORD1_MRT_EXPIRED_SHIFT        37
#define PNIP_CPM_TSB_WORD1_DHT_EXPIRED_MASK         0x0000001000000000ULL   // Bit(36)
  #define PNIP_CPM_TSB_WORD1_DHT_EXPIRED_SHIFT        36
#define PNIP_CPM_TSB_WORD1_WDT_EXPIRED_MASK         0x0000000800000000ULL   // Bit(35)
  #define PNIP_CPM_TSB_WORD1_WDT_EXPIRED_SHIFT        35
#define PNIP_CPM_TSB_WORD1_MRT_UNKNOWN_MASK         0x0000000400000000ULL   // Bit(34)
  #define PNIP_CPM_TSB_WORD1_MRT_UNKNOWN_SHIFT        34
#define PNIP_CPM_TSB_WORD1_DHT_UNKNOWN_MASK         0x0000000200000000ULL   // Bit(33)
  #define PNIP_CPM_TSB_WORD1_DHT_UNKNOWN_SHIFT        33
#define PNIP_CPM_TSB_WORD1_WDT_UNKNOWN_MASK         0x0000000100000000ULL   // Bit(32)
  #define PNIP_CPM_TSB_WORD1_WDT_UNKNOWN_SHIFT        32
#define PNIP_CPM_TSB_WORD1_AUTOSTOP_EN_MASK         0x0000000080000000ULL   // Bit(31)
  #define PNIP_CPM_TSB_WORD1_AUTOSTOP_EN_SHIFT        31
																		    // Bit(29..30) reserved
#define PNIP_CPM_TSB_WORD1_RCVD_IN_DP2_MASK         0x0000000010000000ULL   // Bit(28)
  #define PNIP_CPM_TSB_WORD1_RCVD_IN_DP2_SHIFT        28
#define PNIP_CPM_TSB_WORD1_DS_PROBL_IND_MASK        0x0000000008000000ULL   // Bit(27)
  #define PNIP_CPM_TSB_WORD1_DS_PROBL_IND_SHIFT       27
#define PNIP_CPM_TSB_WORD1_DS_RUNSTOP_MASK          0x0000000004000000ULL   // Bit(26)
  #define PNIP_CPM_TSB_WORD1_DS_RUNSTOP_SHIFT         26
#define PNIP_CPM_TSB_WORD1_DS_SYS_RED_MASK          0x0000000002000000ULL   // Bit(25)
  #define PNIP_CPM_TSB_WORD1_DS_SYS_RED_SHIFT         25
#define PNIP_CPM_TSB_WORD1_DS_STATE_MASK            0x0000000001000000ULL   // Bit(24)
  #define PNIP_CPM_TSB_WORD1_DS_STATE_SHIFT           24
#define PNIP_CPM_TSB_WORD1_RCVD_IN_DP2_CHD_MASK     0x0000000000800000ULL   // Bit(23)
  #define PNIP_CPM_TSB_WORD1_RCVD_IN_DP2_CHD_SHIFT     23
#define PNIP_CPM_TSB_WORD1_DS_PROBL_IND_CHD_MASK    0x0000000000400000ULL   // Bit(22)
  #define PNIP_CPM_TSB_WORD1_DS_PROBL_IND_CHD_SHIFT   22
#define PNIP_CPM_TSB_WORD1_DS_RUNSTOP_CHD_MASK      0x0000000000200000ULL   // Bit(21)
  #define PNIP_CPM_TSB_WORD1_DS_RUNSTOP_CHD_SHIFT     21
#define PNIP_CPM_TSB_WORD1_DS_SYS_RED_CHD_MASK      0x0000000000100000ULL   // Bit(20)
  #define PNIP_CPM_TSB_WORD1_DS_SYS_RED_CHD_SHIFT     20
#define PNIP_CPM_TSB_WORD1_DS_STATE_CHD_MASK        0x0000000000080000ULL   // Bit(19)
  #define PNIP_CPM_TSB_WORD1_DS_STATE_CHD_SHIFT       19
#define PNIP_CPM_TSB_WORD1_TSBRR_COUNTER_MASK       0x000000000007FFFFULL   // Bit(0..18)
  #define PNIP_CPM_TSB_WORD1_TSBRR_COUNTER_SHIFT      0

#define PNIP_CPM_TSB_IS_VALID(TSB_Word1)            (TSB_Word1 & PNIP_CPM_TSB_WORD1_VALID_MASK)


/* TSB_Word2:                                                                                           */
/*      - Bit( 0..18): TSBRR_ReloadValue  :    Relaod value for TSBRR_Counter                           */
/*      - Bit(19..31): reserved           :    -                                                        */
/*      - Bit(32..47): ACW_offset         :    connected Rcv-ACW                                        */
/*      - Bit(48..63): free               :    free for SW                                              */


/* ------------------------------------------------------------------------- */
/* CPM_TSB - masks for TSB_Word2 (LSA_UINT64)                                */
/* ------------------------------------------------------------------------- */
#define PNIP_CPM_TSB_WORD2_SW_SPACE_MASK            0xFFFF000000000000ULL   // Bit(48..63)
  #define PNIP_CPM_TSB_WORD2_SW_SPACE_SHIFT           48
#define PNIP_CPM_TSB_WORD2_ACW_OFFSET_MASK          0x0000FFFF00000000ULL   // Bit(32..47)
  #define PNIP_CPM_TSB_WORD2_ACW_OFFSET_SHIFT         32
  #define PNIP_CPM_TSB_WORD2_ACW_OFFSET_INVALID       0xFFFF
                                                                            // Bit(19..31) reserved
#define PNIP_CPM_TSB_WORD2_TSBRR_RELOAD_MASK        0x000000000007FFFFULL   // Bit(0..18)
  #define PNIP_CPM_TSB_WORD2_TSBRR_RELOAD_SHIFT       0

#define PNIP_CPM_TSB_SET_ACW_OFFS(stru, offs)       (stru.TSB_Word2 &= PNIP_CPM_TSB_WORD2_ACW_OFFSET_MASK; stru.TSB_Word2 |= ((offs << PNIP_CPM_TSB_WORD2_ACW_OFFSET_SHIFT) & PNIP_CPM_TSB_WORD2_ACW_OFFSET_MASK) )
#define PNIP_CPM_TSB_SET_TSBRR_RELOAD(stru, brr)    (stru.TSB_Word2 &= PNIP_CPM_TSB_WORD2_TSBRR_RELOAD_MASK; stru.TSB_Word2 |= ((brr << PNIP_CPM_TSB_WORD2_TSBRR_RELOAD_SHIFT) & PNIP_CPM_TSB_WORD2_TSBRR_RELOAD_MASK) )


#define PNIP_CPM_TSB_WORD1_DEF_VALUE                0x0  // Bit(0..63)
#define PNIP_CPM_TSB_WORD2_DEF_VALUE                0x0  // Bit(0..63)



/* ------------------------------------------------------------------------- */
/* CPM CSB - the link to this memory is in pCSB entry                        */
/* ------------------------------------------------------------------------- */
typedef struct _PNIP_CPM_CSB_TYPE     EDD_COMMON_MEM_ATTR *   PNIP_CPM_CSB_PTR_TYPE;
PNIO_PACKED_ATTRIBUTE_PRE struct _PNIP_CPM_CSB_TYPE
{
    LSA_UINT32          CSB_Word1;       // used for old/new detection
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _PNIP_CPM_CSB_TYPE   PNIP_CPM_CSB_TYPE;


/* CSB_Word1     : configuration word                                                                   */

/* CSB_Word1:                                                                                           */
/*      - Bit( 0): WDT_expired:         result of Timer-Scoreboard.WD_Timer                             */
/*      - Bit( 1): DHT_expired:         result of Timer-Scoreboard.DHT_Timer                            */
/*      - Bit( 2): MRT_expired:         result of Timer-Scoreboard.MRT_Timer                            */
/*      - Bit( 3): DS_State   :         DS_State.Bit_0                                                  */
/*      - Bit( 4): DS_SystemRedundancy: DS_State.Bit_1                                                  */
/*      - Bit( 5): DS_RunStop:          DS_State.Bit_4                                                  */
/*      - Bit( 6): DS_ProblemIndicator: DS_State.Bit_5                                                  */
/*      - Bit( 7): Received_in_DP2:                                                                     */
/*      - Bit( 8): Frame_received:                                                                      */
/*      - Bit( 9-15): reserved                                                                          */
/*      - Bit(16): WDT_expired_changed:                                                                 */
/*      - Bit(17): DHT_expired_changed:                                                                 */
/*      - Bit(18): MRT_expired_changed:                                                                 */
/*      - Bit(19): DS_State_changed:                                                                    */
/*      - Bit(20): DS_SystemRedundancy_changed:                                                         */
/*      - Bit(21): DS_RunStop_changed:                                                                  */
/*      - Bit(22): DS_ProblemIndicator_changed:                                                         */
/*      - Bit(23): Received_in_DP2_changed:                                                             */
/*      - Bit(24-31): reserved                                                                          */

/* ------------------------------------------------------------------------- */
/* CSB_Word1 - masks for CSB_Word1 (LSA_UINT32)                              */
/* ------------------------------------------------------------------------- */
                                                                // Bit(24..31) reserved
#define PNIP_CPM_CSB_WORD1_RCVD_IN_DP2_CHD_MASK     0x00800000  // Bit(23)
  #define PNIP_CPM_CSB_WORD1_RCVD_IN_DP2_CHD_SHIFT    23
#define PNIP_CPM_CSB_WORD1_DS_PROBL_IND_CHD_MASK    0x00400000  // Bit(22)
  #define PNIP_CPM_CSB_WORD1_DS_PROBL_IND_CHD_SHIFT   22
#define PNIP_CPM_CSB_WORD1_DS_RUNSTOP_CHD_MASK      0x00200000  // Bit(21)
  #define PNIP_CPM_CSB_WORD1_DS_RUNSTOP_CHD_SHIFT     21
#define PNIP_CPM_CSB_WORD1_DS_SYS_RED_CHD_MASK      0x00100000  // Bit(20)
  #define PNIP_CPM_CSB_WORD1_DS_SYS_RED_CHD_SHIFT     20
#define PNIP_CPM_CSB_WORD1_DS_STATE_CHD_MASK        0x00080000  // Bit(19)
  #define PNIP_CPM_CSB_WORD1_DS_STATE_CHD_SHIFT       19
#define PNIP_CPM_CSB_WORD1_MRT_EXPIRED_CHD_MASK     0x00040000  // Bit(18)
  #define PNIP_CPM_CSB_WORD1_MRT_EXPIRED_CHD_SHIFT    18
#define PNIP_CPM_CSB_WORD1_DHT_EXPIRED_CHD_MASK     0x00020000  // Bit(17)
  #define PNIP_CPM_CSB_WORD1_DHT_EXPIRED_CHD_SHIFT    17
#define PNIP_CPM_CSB_WORD1_WDT_EXPIRED_CHD_MASK     0x00010000  // Bit(16)
  #define PNIP_CPM_CSB_WORD1_WDT_EXPIRED_CHD_SHIFT    16

                                                                // Bit(9..15) reserved
#define PNIP_CPM_CSB_WORD1_FRM_RCVD_MASK            0x00000100  // Bit(8)
  #define PNIP_CPM_CSB_WORD1_FRM_RCVD_SHIFT           8
#define PNIP_CPM_CSB_WORD1_RCVD_IN_DP2_MASK         0x00000080  // Bit(7)
  #define PNIP_CPM_CSB_WORD1_RCVD_IN_DP2_SHIFT        7
#define PNIP_CPM_CSB_WORD1_DS_PROBL_IND_MASK        0x00000040  // Bit(6)
  #define PNIP_CPM_CSB_WORD1_DS_PROBL_IND_SHIFT       6
#define PNIP_CPM_CSB_WORD1_DS_RUNSTOP_MASK          0x00000020  // Bit(5)
  #define PNIP_CPM_CSB_WORD1_DS_RUNSTOP_SHIFT         5
#define PNIP_CPM_CSB_WORD1_DS_SYS_RED_MASK          0x00000010  // Bit(4)
  #define PNIP_CPM_CSB_WORD1_DS_SYS_RED_SHIFT         4
#define PNIP_CPM_CSB_WORD1_DS_STATE_MASK            0x00000008  // Bit(3)
  #define PNIP_CPM_CSB_WORD1_DS_STATE_SHIFT           3
#define PNIP_CPM_CSB_WORD1_MRT_EXPIRED_MASK         0x00000004  // Bit(2)
  #define PNIP_CPM_CSB_WORD1_MRT_EXPIRED_SHIFT        2
#define PNIP_CPM_CSB_WORD1_DHT_EXPIRED_MASK         0x00000002  // Bit(1)
  #define PNIP_CPM_CSB_WORD1_DHT_EXPIRED_SHIFT        1
#define PNIP_CPM_CSB_WORD1_WDT_EXPIRED_MASK         0x00000001  // Bit(0)
  #define PNIP_CPM_CSB_WORD1_WDT_EXPIRED_SHIFT        0

#define PNIP_CPM_CSB_WORD1_CHANGED_MASK             0x00FF0000  // Bit(16..23)
  #define PNIP_CPM_CSB_WORD1_CHANGED_SHIFT            16

#define PNIP_CPM_CSB_WORD1_DEF_VALUE                0x00000000  // Bit(0..31)


/********************************************************************************************************************/
/* ACW Send                                                                                                         */
/********************************************************************************************************************/
// Crt-PPM-SW-Data
PNIO_PACKED_ATTRIBUTE_PRE struct _PNIP_ACW_SND_CRT_DATA_TYPE
{
    LSA_UINT16          ProviderId;
    LSA_UINT16          reserved1;
    LSA_UINT16          reserved2;
    LSA_UINT16          reserved3;
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _PNIP_ACW_SND_CRT_DATA_TYPE  PNIP_ACW_SND_CRT_DATA_TYPE;
/* ProviderId       : Provider to which this ACW belongs                                            */

// ACW-Words
typedef struct _PNIP_ACW_SND_TYPE   EDD_COMMON_MEM_ATTR *   PNIP_ACW_SND_PTR_TYPE;
PNIO_PACKED_ATTRIBUTE_PRE struct _PNIP_ACW_SND_TYPE
{
    LSA_UINT64                      ACW_Word1;
    LSA_UINT64                      ACW_Word2;
    LSA_UINT64                      ACW_Word3;
    LSA_UINT64                      ACW_Word4;
    LSA_UINT64                      ACW_Word5;
    PNIP_ACW_SND_CRT_DATA_TYPE      PpmSwData;
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _PNIP_ACW_SND_TYPE   PNIP_ACW_SND_TYPE;
/* ACW_Word1        : PNIP HW-struct word 1                                                         */
/* ACW_Word2        : PNIP HW-struct word 2                                                         */
/* ACW_Word3        : PNIP HW-struct word 3                                                         */
/* ACW_Word4        : PNIP HW-struct word 4                                                         */
/* ACW_Word5        : PNIP HW-struct word 5                                                         */
/* PpmSwData        : SW data for internal SW mgmt refencing                                        */

/* ------------------------------------------------------------------------- */
/* ACW Send - ACW_Word1                                                      */
/* ------------------------------------------------------------------------- */
// mask and shift
#define PNIP_ACW_SND_WORD1_FRAMEID_MASK                     0x000000000000FFFFULL      // 15:0
#define PNIP_ACW_SND_WORD1_FRAMEID_SHIFT                    0
#define PNIP_R2_ACW_SND_WORD1_ACW_NEXTOFFSET_MASK           0x00000000FFFF0000ULL      // 31:16
#define PNIP_R3_ACW_SND_WORD1_ACW_NEXTOFFSET_MASK           0x000000007FFF0000ULL      // 30:16
#define PNIP_ACW_SND_WORD1_ACW_NEXTOFFSET_SHIFT             16
#define PNIP_R2_ACW_SND_WORD1_ACW_DG_OFFSET_MASK            0x0000FFFF00000000ULL      // 47:32
#define PNIP_R3_ACW_SND_WORD1_ACW_DG_OFFSET_MASK            0x00007FFF00000000ULL      // 46:32
#define PNIP_ACW_SND_WORD1_ACW_DG_OFFSET_SHIFT              32
#define PNIP_ACW_SND_WORD1_OPCODE_MASK                      0x0003000000000000ULL      // 49:48
#define PNIP_ACW_SND_WORD1_OPCODE_SHIFT                     48
#define PNIP_ACW_SND_WORD1_ACTIVE_MASK                      0x0004000000000000ULL      // 50
#define PNIP_ACW_SND_WORD1_ACTIVE_SHIFT                     50
#define PNIP_ACW_SND_WORD1_FIRSTSEND_MASK                   0x0008000000000000ULL      // 51
#define PNIP_ACW_SND_WORD1_FIRSTSEND_SHIFT                  51
#define PNIP_ACW_SND_WORD1_DP_RAW_MODE_MASK                 0x0010000000000000ULL      // 52
#define PNIP_ACW_SND_WORD1_DP_RAW_MODE_SHIFT                52
#define PNIP_ACW_SND_WORD1_DP2_TYPE_MASK                    0x0020000000000000ULL      // 53
#define PNIP_ACW_SND_WORD1_DP2_TYPE_SHIFT                   53
#define PNIP_ACW_SND_WORD1_ETHERTYPE_SELECT_MASK            0x00C0000000000000ULL      // 55:54
#define PNIP_ACW_SND_WORD1_ETHERTYPE_SELECT_SHIFT           54
#define PNIP_ACW_SND_WORD1_INBOUND_PACK_MASK                0x0100000000000000ULL      // 56
#define PNIP_ACW_SND_WORD1_INBOUND_PACK_SHIFT               56
#define PNIP_ACW_SND_WORD1_RT_CLASS_MASK                    0x0200000000000000ULL      // 57
#define PNIP_ACW_SND_WORD1_RT_CLASS_SHIFT                   57
#define PNIP_ACW_SND_WORD1_ACW_DG_ORDER_MASK                0x0400000000000000ULL      // 58
#define PNIP_ACW_SND_WORD1_ACW_DG_ORDER_SHIFT               58
#define PNIP_ACW_SND_WORD1_DRM_MASK                         0x0800000000000000ULL      // 59
#define PNIP_ACW_SND_WORD1_DRM_SHIFT                        59
#define PNIP_ACW_SND_WORD1_DESTINATION_MASK                 0xF000000000000000ULL      // 63:60
#define PNIP_ACW_SND_WORD1_DESTINATION_SHIFT                60
// constant value
#define PNIP_R2_ACW_SND_WORD1_ACW_NEXTOFFSET_INIT_VALUE     0xFFFF  // init for ACW_nextoffset
#define PNIP_R3_ACW_SND_WORD1_ACW_NEXTOFFSET_INIT_VALUE     0x7FFF  // init for ACW_nextoffset
#define PNIP_R2_ACW_SND_WORD1_ACW_DG_OFFSET_INIT_VALUE      0xFFFF  // init for ACW_DG_offset
#define PNIP_R3_ACW_SND_WORD1_ACW_DG_OFFSET_INIT_VALUE      0x7FFF  // init for ACW_DG_offset
#define PNIP_ACW_SND_WORD1_OPCODE_ACW_SND_VALUE             0x2     // opcode for send ACW
#define PNIP_ACW_SND_WORD1_ACTIVE_INIT_VALUE                0x1     // init for Active-Bit (it is allowed to call the command "ACW_in_out" with set Active-Bit)
#define PNIP_ACW_SND_WORD1_FIRSTSEND_INIT_VALUE             0x1     // init for FirstSend-Bit
#define PNIP_ACW_SND_WORD1_DP_RAW_MODE_OFF_VALUE            0x0     // DP_Raw_Mode is off   -> do not use DataPlane (no IRTtop)
#define PNIP_ACW_SND_WORD1_DP_RAW_MODE_ON_VALUE             0x1     // DP_Raw_Mode is on    -> use DataPlane (IRTtop)
#define PNIP_ACW_SND_WORD1_DP2_TYPE_DP1_VALUE               0x0     // DP1
#define PNIP_ACW_SND_WORD1_DP2_TYPE_DP2_VALUE               0x1     // DP2 - send only if Red is active
#define PNIP_ACW_SND_WORD1_ETHERTYPE_SELECT_INDEX0_VALUE    0x0     // Index 0 to select a Register "Local_SPort_Etype_0"
#define PNIP_ACW_SND_WORD1_ETHERTYPE_SELECT_INDEX1_VALUE    0x1     // Index 1 to select a Register "Local_SPort_Etype_1"
#define PNIP_ACW_SND_WORD1_ETHERTYPE_SELECT_INDEX2_VALUE    0x2     // Index 2 to select a Register "Local_SPort_Etype_2"
#define PNIP_ACW_SND_WORD1_ETHERTYPE_SELECT_INDEX3_VALUE    0x3     // Index 3 to select a Register "Local_SPort_Etype_3"
#define PNIP_ACW_SND_WORD1_INBOUND_PACK_OFF_VALUE           0x0     // the pack frame is a not InBound-Pack-Frame
#define PNIP_ACW_SND_WORD1_INBOUND_PACK_ON_VALUE            0x1     // the pack frame is a InBound-Pack-Frame
#define PNIP_ACW_SND_WORD1_RT_CLASS_RT_VALUE                0x0     // RT_class = RT-Frame
#define PNIP_ACW_SND_WORD1_RT_CLASS_UDP_VALUE               0x1     // RT_class = RToverUDP-Frame
#define PNIP_ACW_SND_WORD1_ACW_DG_ORDER_HIGH_VALUE          0x0     // ascending order for sending of ACW_DGs
#define PNIP_ACW_SND_WORD1_ACW_DG_ORDER_DOWN_VALUE          0x1     // descending order for sending of ACW_DGs
#define PNIP_ACW_SND_WORD1_ACW_DG_ORDER_UDP_IPV4            0x0     // RToverUDP (IPv4)
#define PNIP_ACW_SND_WORD1_ACW_DG_ORDER_UDP_IPV6            0x1     // RToverUDP (IPv6)
#define PNIP_ACW_SND_WORD1_DRM_OFF_VALUE                    0x0     // Destination Raw Mode for port is off
#define PNIP_ACW_SND_WORD1_DRM_ON_VALUE                     0x1     // Destination Raw Mode for port is on
#define PNIP_ACW_SND_WORD1_DESTINATION_OFF_VALUE            0x0     // "Destination"            -> no Port specification
#define PNIP_ACW_SND_WORD1_DESTINATION_P1_VALUE             0x1     // Bit 0: "Destination"     -> Port1 specification
#define PNIP_ACW_SND_WORD1_DESTINATION_P2_VALUE             0x2     // Bit 1: "Destination"     -> Port2 specification
#define PNIP_ACW_SND_WORD1_DESTINATION_P3_VALUE             0x4     // Bit 2: "Destination"     -> Port3 specification
#define PNIP_ACW_SND_WORD1_DESTINATION_P4_VALUE             0x8     // Bit 3: "Destination"     -> Port4 specification

/* ------------------------------------------------------------------------- */
/* ACW Send - ACW_Word2                                                      */
/* ------------------------------------------------------------------------- */
// mask and shift
#define PNIP_ACW_SND_WORD2_MAC_DA_0_MASK                    0x00000000000000FFULL      // 7:0      47:0  MAC_DA_5_0
#define PNIP_ACW_SND_WORD2_MAC_DA_0_SHIFT                   0
#define PNIP_ACW_SND_WORD2_MAC_DA_1_MASK                    0x000000000000FF00ULL      // 15:8
#define PNIP_ACW_SND_WORD2_MAC_DA_1_SHIFT                   8
#define PNIP_ACW_SND_WORD2_MAC_DA_2_MASK                    0x0000000000FF0000ULL      // 23:16
#define PNIP_ACW_SND_WORD2_MAC_DA_2_SHIFT                   16
#define PNIP_ACW_SND_WORD2_MAC_DA_3_MASK                    0x00000000FF000000ULL      // 31:24
#define PNIP_ACW_SND_WORD2_MAC_DA_3_SHIFT                   24
#define PNIP_ACW_SND_WORD2_MAC_DA_4_MASK                    0x000000FF00000000ULL      // 39:32
#define PNIP_ACW_SND_WORD2_MAC_DA_4_SHIFT                   32
#define PNIP_ACW_SND_WORD2_MAC_DA_5_MASK                    0x0000FF0000000000ULL      // 47:40
#define PNIP_ACW_SND_WORD2_MAC_DA_5_SHIFT                   40
#define PNIP_ACW_SND_WORD2_Q_SELECT_MASK                    0x0003000000000000ULL      // 49:48
#define PNIP_ACW_SND_WORD2_Q_SELECT_SHIFT                   48
#define PNIP_ACW_SND_WORD2_PHASENINFO_MASK                  0x003C000000000000ULL      // 53:50
#define PNIP_ACW_SND_WORD2_PHASENINFO_SHIFT                 50
#define PNIP_ACW_SND_WORD2_TX_TIMEMODE_MASK                 0x0040000000000000ULL      // 54
#define PNIP_ACW_SND_WORD2_TX_TIMEMODE_SHIFT                54
#define PNIP_ACW_SND_WORD2_DA_SELECT_MASK                   0x0080000000000000ULL      // 55
#define PNIP_ACW_SND_WORD2_DA_SELECT_SHIFT                  55
#define PNIP_ACW_SND_WORD2_VLAN_TAG_MASK                    0x0100000000000000ULL      // 56
#define PNIP_ACW_SND_WORD2_VLAN_TAG_SHIFT                   56
#define PNIP_ACW_SND_WORD2_VLAN_PRIO_MASK                   0x0E00000000000000ULL      // 59:57
#define PNIP_ACW_SND_WORD2_VLAN_PRIO_SHIFT                  57
#define PNIP_ACW_SND_WORD2_USETYPE_RAW_MODE_MASK            0x1000000000000000ULL      // 60
#define PNIP_ACW_SND_WORD2_USETYPE_RAW_MODE_SHIFT           60
#define PNIP_ACW_SND_WORD2_USETYPE_MASK                     0xE000000000000000ULL      // 63:61
#define PNIP_ACW_SND_WORD2_USETYPE_SHIFT                    61
// constant value
#define PNIP_ACW_SND_WORD2_Q_SELECT_PRIO_VALUE                      0x0     // put frame to Priority Queue 0-11
#define PNIP_ACW_SND_WORD2_Q_SELECT_PHASE_VALUE                     0x1     // put frame to Phase-Queue-14
#define PNIP_ACW_SND_WORD2_Q_SELECT_TIME_VALUE                      0x2     // put frame to Time-Queue-15
#define PNIP_ACW_SND_WORD2_Q_SELECT_RESERVED_VALUE                  0x3     // Q_Select reserved
#define PNIP_ACW_SND_WORD2_PHASENINFO_RED_ON_VALUE                  0x1     // Bit 0 in PhasenInfo - red phase
#define PNIP_ACW_SND_WORD2_PHASENINFO_ORANGE_ON_VALUE               0x2     // Bit 1 in PhasenInfo - orange phase
#define PNIP_ACW_SND_WORD2_PHASENINFO_GREEN_ON_VALUE                0x4     // Bit 2 in PhasenInfo - green phase
#define PNIP_ACW_SND_WORD2_PHASENINFO_TRANSFERTYPE_OFF_VALUE        0x0     // Bit 3 in PhasenInfo - TransferType OFF
#define PNIP_ACW_SND_WORD2_PHASENINFO_TRANSFERTYPE_ON_VALUE         0x8     // Bit 3 in PhasenInfo - TransferType ON
#define PNIP_ACW_SND_WORD2_TX_TIMEMODE_MINIMUM                      0x0     // Tx_TimeMode = Minimum
#define PNIP_ACW_SND_WORD2_TX_TIMEMODE_EXACT                        0x1     // Tx_TimeMode = Exact
#define PNIP_ACW_SND_WORD2_DA_SELECT_ACW_VALUE                      0x0     // get DstMAC-Address from ACW.MAC_DA
#define PNIP_ACW_SND_WORD2_DA_SELECT_REG_VALUE                      0x1     // get DstMAC-Address from Register "Router_MAC_DA"
#define PNIP_ACW_SND_WORD2_VLAN_TAG_OFF_VALUE                       0x0     // frame without VLAN_TAG
#define PNIP_ACW_SND_WORD2_VLAN_TAG_ON_VALUE                        0x1     // frame with VLAN_TAG
#define PNIP_ACW_SND_WORD2_VLAN_PRIO_INIT_VALUE                     0x6     // init for VLAN_Prio
#define PNIP_ACW_SND_WORD2_USETYPE_RAW_MODE_FRAME_CLASS_VALUE       0x0     // get "UseType" by frame classification
#define PNIP_ACW_SND_WORD2_USETYPE_RAW_MODE_ACW_VALUE               0x1     // get "UseType" by ACW.UseType
#define PNIP_ACW_SND_WORD2_USETYPE_STD_VALUE                        (EDDP_MCFCT_USE_TYPE_STANDARD)  // standard UseType

/* ------------------------------------------------------------------------- */
/* ACW Send - ACW_Word3                                                      */
/* ------------------------------------------------------------------------- */
// mask and shift
#define PNIP_R2_ACW_SND_WORD3_SCOREBOARD_OFFSET_MASK			0x000000000000FFFFULL      // 15:0
#define PNIP_R3_ACW_SND_WORD3_SCOREBOARD_OFFSET_MASK			0x0000000000007FFFULL      // 14:0
#define PNIP_ACW_SND_WORD3_SCOREBOARD_OFFSET_SHIFT				0
#define PNIP_R3_ACW_SND_WORD3_RESERVED1_MASK_PNIP				0x0000000000008000ULL      // 15
#define PNIP_R3_ACW_SND_WORD3_RESERVED1_SHIFT_PNIP				15
#define PNIP_ACW_SND_WORD3_DATASTATUS_OFFSET_MASK				0x0000000007FF0000ULL      // 26:16
#define PNIP_ACW_SND_WORD3_DATASTATUS_OFFSET_SHIFT				16
#define PNIP_ACW_SND_WORD3_ACW_WAIT_MASK						0x0000000008000000ULL      // 27
#define PNIP_ACW_SND_WORD3_ACW_WAIT_SHIFT						27
#define PNIP_ACW_SND_WORD3_CT_SCHWELLE_MASK						0x00000000F0000000ULL      // 31:28
#define PNIP_ACW_SND_WORD3_CT_SCHWELLE_SHIFT					28
#define PNIP_ACW_SND_WORD3_TX_TIME_MASK							0x00FFFFFF00000000ULL      // 55:32
#define PNIP_ACW_SND_WORD3_TX_TIME_SHIFT						32
// PN-IP Rev1 -----------------------------------------------------------------
#define PNIP_R1_ACW_SND_WORD3_RESERVED2_MASK_PNIP               0x0700000000000000ULL      // 58:56
#define PNIP_R1_ACW_SND_WORD3_RESERVED2_SHIFT_PNIP              56
// PN-IP Rev2 -----------------------------------------------------------------
#define PNIP_R2_ACW_SND_WORD3_RESERVED2_MASK_PNIP               0x0300000000000000ULL      // 57:56
#define PNIP_R2_ACW_SND_WORD3_RESERVED2_SHIFT_PNIP              56
#define PNIP_R2_ACW_SND_WORD3_DS_HANDLING_MASK_PNIP             0x0400000000000000ULL      // 58
#define PNIP_R2_ACW_SND_WORD3_DS_HANDLING_SHIFT_PNIP            58
// // PN-IP Rev3---------------------------------------------------------------
// HERADevMark Speichereinstellungen - für R3 hinzugefügt - vorerst lassen - wurde angepasst, sollte ok sein
#define PNIP_R3_ACW_SND_WORD3_RESERVED2_MASK_PNIP				0x0100000000000000ULL      // 56
#define PNIP_R3_ACW_SND_WORD3_RESERVED2_SHIFT_PNIP				56
#define PNIP_R3_ACW_SND_WORD3_DATASTATUS_C_HANDLING_MASK_PNIP   0x0200000000000000ULL      // 57
#define PNIP_R3_ACW_SND_WORD3_DATASTATUS_C_HANDLING_SHIFT_PNIP  57
#define PNIP_R3_ACW_SND_WORD3_RESERVED3_MASK_PNIP				0x0400000000000000ULL      // 58
#define PNIP_R3_ACW_SND_WORD3_RESERVED3_SHIFT_PNIP				58
// ----------------------------------------------------------------------------
#define PNIP_ACW_SND_WORD3_RESSOURCE_RAW_MODE_MASK              0x0800000000000000ULL      // 59
#define PNIP_ACW_SND_WORD3_RESSOURCE_RAW_MODE_SHIFT             59
#define PNIP_R2_ACW_SND_WORD3_RESSOURCE_TYPE_MASK               0xF000000000000000ULL      // 63:60
#define PNIP_R2_ACW_SND_WORD3_RESSOURCE_TYPE_SHIFT              60
// constant value
#define PNIP_R2_ACW_SND_WORD3_SCOREBOARD_OFFSET_OFF_VALUE       0xFFFF  // control of Timer-Scoreboard (Provider-AutoStop) is off
#define PNIP_R3_ACW_SND_WORD3_SCOREBOARD_OFFSET_OFF_VALUE       0x7FFF  // control of Timer-Scoreboard (Provider-AutoStop) is off
#define PNIP_ACW_SND_WORD3_ACW_WAIT_OFF_VALUE                   0x0     // ACW_Wait is off
#define PNIP_ACW_SND_WORD3_ACW_WAIT_ON_VALUE                    0x1     // ACW_Wait is on
#define PNIP_ACW_SND_WORD3_CT_SCHWELLE_0_VALUE                  0x0
#define PNIP_ACW_SND_WORD3_CT_SCHWELLE_1_VALUE                  0x1
#define PNIP_ACW_SND_WORD3_CT_SCHWELLE_15_VALUE                 0xF
#define PNIP_ACW_SND_WORD3_TX_TIME_OFF_VALUE                    0x0     // sending time (only for Queue-15)
#define PNIP_ACW_SND_WORD3_RESERVED_VALUE                       0x0     // reserved
#define PNIP_R2_ACW_SND_WORD3_DS_HANDLING_NOT_SAVE_VALUE        0x0     // do not save the DataStatus
#define PNIP_R2_ACW_SND_WORD3_DS_HANDLING_SAVE_VALUE            0x1     // save the DataStatus
#define PNIP_R3_ACW_SND_WORD3_DATASTATUS_C_HANDLING_OFF_VALUE   0x0     // DataStatus_C_Handling is OFF
#define PNIP_R3_ACW_SND_WORD3_DATASTATUS_C_HANDLING_ON_VALUE    0x1     // DataStatus_C_Handling is ON
#define PNIP_ACW_SND_WORD3_RESSOURCE_RAW_MODE_FRAME_CLASS_VALUE 0x0     // get "ResourceType" by frame classification
#define PNIP_ACW_SND_WORD3_RESSOURCE_RAW_MODE_ACW_VALUE         0x1     // get "ResourceType" by ACW.ResourceType
#define PNIP_ACW_SND_WORD3_RESSOURCE_TYPE_RT_L_VALUE            (EDDP_MCFCT_RESOURCE_TYPE_RT_L)     // "ResourceType" RT_L
#define PNIP_ACW_SND_WORD3_RESSOURCE_TYPE_RT_H1_VALUE           (EDDP_MCFCT_RESOURCE_TYPE_RT_H1)    // "ResourceType" RT_H1
#define PNIP_ACW_SND_WORD3_RESSOURCE_TYPE_RT_H2_VALUE           (EDDP_MCFCT_RESOURCE_TYPE_RT_H2)    // "ResourceType" RT_H2
#define PNIP_ACW_SND_WORD3_RESSOURCE_TYPE_RT_H3_VALUE           (EDDP_MCFCT_RESOURCE_TYPE_RT_H3)    // "ResourceType" RT_H3
#define PNIP_ACW_SND_WORD3_RESSOURCE_TYPE_RT_H4_VALUE           (EDDP_MCFCT_RESOURCE_TYPE_RT_H4)    // "ResourceType" RT_H4

/* ------------------------------------------------------------------------- */
/* ACW Send - ACW_Word4                                                      */
/* ------------------------------------------------------------------------- */
// mask and shift
#define PNIP_R2_ACW_SND_WORD4_DATA_PTR_MASK                             0x00000000FFFFFFFFULL  // 31:0
#define PNIP_R2_ACW_SND_WORD4_DATA_PTR_SHIFT                            0
#define PNIP_R3_ACW_SND_WORD4_MEM_TYPE_MASK                             0x0000000000000001ULL  // 0
#define PNIP_R3_ACW_SND_WORD4_MEM_TYPE_SHIFT                            0
#define PNIP_R3_ACW_SND_WORD4_RESERVED1_MASK                            0x0000000000000006ULL  // 2:1
#define PNIP_R3_ACW_SND_WORD4_RESERVED1_SHIFT                           1
#define PNIP_R3_ACW_SND_WORD4_BUFFER_TRANSFER_STRUCTURE_OFFSET_MASK     0x00000000FFFFFFF8ULL  // 31:3
#define PNIP_R3_ACW_SND_WORD4_BUFFER_TRANSFER_STRUCTURE_OFFSET_SHIFT    3
#define PNIP_R3_ACW_SND_WORD4_RESERVED2_MASK							0x000007FF00000000ULL  // 42:32
#define PNIP_R3_ACW_SND_WORD4_RESERVED2_SHIFT							32

#define PNIP_R2_ACW_SND_WORD4_DATASTART_OFFSET_MASK                     0x000007FF00000000ULL  // 42:32
#define PNIP_R2_ACW_SND_WORD4_DATASTART_OFFSET_SHIFT                    32
#define PNIP_ACW_SND_WORD4_DATALENGTH_DATASTART_OFFSET2_MASK            0x003FF80000000000ULL  // 53:43
#define PNIP_ACW_SND_WORD4_DATALENGTH_DATASTART_OFFSET2_SHIFT           43
#define PNIP_R2_ACW_SND_WORD4_RESERVED1_MASK                            0x0040000000000000ULL  // 54
#define PNIP_R2_ACW_SND_WORD4_RESERVED1_SHIFT                           54
#define PNIP_R3_ACW_SND_WORD4_RESERVED3_MASK                            0xFFC0000000000000ULL  // 63:54
#define PNIP_R3_ACW_SND_WORD4_RESERVED3_SHIFT                           54

#define PNIP_R2_ACW_SND_WORD4_SCATTER_SELECT_MASK                       0x0080000000000000ULL  // 55
#define PNIP_R2_ACW_SND_WORD4_SCATTER_SELECT_SHIFT                      55
#define PNIP_R2_ACW_SND_WORD4_CR_NUMBER_MASK                            0x1F00000000000000ULL  // 60:56
#define PNIP_R2_ACW_SND_WORD4_CR_NUMBER_SHIFT                           56
#define PNIP_R2_ACW_SND_WORD4_BUFFER_MODE_MASK                          0x2000000000000000ULL  // 61
#define PNIP_R2_ACW_SND_WORD4_BUFFER_MODE_SHIFT                         61
#define PNIP_R2_ACW_SND_WORD4_APP_GROUP_MASK                            0xC000000000000000ULL  // 63:62
#define PNIP_R2_ACW_SND_WORD4_APP_GROUP_SHIFT                           62
// constant value												        
#define PNIP_ACW_SND_WORD4_MEM_TYPE_SLOW_VALUE                          0x0     // BufferTransferStructureOffset refers to SlowMem
#define PNIP_ACW_SND_WORD4_MEM_TYPE_FAST_VALUE                          0x1     // BufferTransferStructureOffset refers to FastMem
#define PNIP_ACW_SND_WORD4_DATASTART_OFFSET_VALUE                       0x0     // tbd. for IRTtop
#define PNIP_ACW_SND_WORD4_RESERVED_VALUE                               0x0     // reserved
#define PNIP_ACW_SND_WORD4_SCATTER_SELECT_OFF_VALUE                     0x0     // Scatter_select is OFF
#define PNIP_ACW_SND_WORD4_SCATTER_SELECT_ON_VALUE                      0x1     // Scatter_select is ON
#define PNIP_ACW_SND_WORD4_BUFFER_MODE_NORMAL_VALUE                     0x0     // BufferMode Normal
#define PNIP_ACW_SND_WORD4_BUFFER_MODE_EXTENSION_VALUE                  0x1     // BufferMode Extension

/* ------------------------------------------------------------------------- */
/* ACW Send - ACW_Word4 for RToverUDP IPv6                                   */
/* ------------------------------------------------------------------------- */
#define PNIP_R3_ACW_SND_WORD4_IPV6_MEM_TYPE_MASK                                0x0000000000000001ULL  // 0
#define PNIP_R3_ACW_SND_WORD4_IPV6_MEM_TYPE_SHIFT								0

#define PNIP_R3_ACW_SND_WORD4_IPV6_RESERVED1_MASK                               0x0000000000000006ULL  // 2:1
#define PNIP_R3_ACW_SND_WORD4_IPV6_RESERVED1_SHIFT                              1

#define PNIP_R3_ACW_SND_WORD4_IPV6_BUFFER_TRANSFER_STRUCTURE_OFFSET_MASK        0x00000000FFFFFFF8ULL  // 31:3
#define PNIP_R3_ACW_SND_WORD4_IPV6_BUFFER_TRANSFER_STRUCTURE_OFFSET_SHIFT       3

#define PNIP_R3_ACW_SND_WORD4_IPV6_DATA_LENGTH_MASK							    0x000007FF00000000ULL  // 42:32
#define PNIP_R3_ACW_SND_WORD4_IPV6_DATA_LENGTH_SHIFT							32

#define PNIP_R3_ACW_SND_WORD4_IPV6_RESERVED2_MASK							    0x0000080000000000ULL  // 43
#define PNIP_R3_ACW_SND_WORD4_IPV6_RESERVED2_SHIFT							    43

#define PNIP_R3_ACW_SND_WORD4_IPV6_IP_SA_SELECT_MASK						    0x0000300000000000ULL  // 45:44
#define PNIP_R3_ACW_SND_WORD4_IPV6_IP_SA_SELECT_SHIFT							44

#define PNIP_R3_ACW_SND_WORD4_IPV6_UDP_SP_SELECT_MASK						    0x0000C00000000000ULL  // 47:46
#define PNIP_R3_ACW_SND_WORD4_IPV6_UDP_SP_SELECT_SHIFT							46

#define PNIP_R3_ACW_SND_WORD4_IPV6_UDP_DP_MASK									0xFFFF000000000000ULL  // 63:48
#define PNIP_R3_ACW_SND_WORD4_IPV6_UDP_DP_SHIFT									48


/* ------------------------------------------------------------------------- */
/* ACW Send - ACW_Word5                                                      */
/* ------------------------------------------------------------------------- */
// mask and shift
#define PNIP_ACW_SND_WORD5_RESERVED_MASK                    0x0000000000000FFFULL      // 11:0
#define PNIP_ACW_SND_WORD5_RESERVED_SHIFT                   0
#define PNIP_ACW_SND_WORD5_IP_SA_SELECT_MASK                0x0000000000003000ULL      // 13:12
#define PNIP_ACW_SND_WORD5_IP_SA_SELECT_SHIFT               12
#define PNIP_ACW_SND_WORD5_UDP_SP_SELECT_MASK               0x000000000000C000ULL      // 15:14
#define PNIP_ACW_SND_WORD5_UDP_SP_SELECT_SHIFT              14
#define PNIP_ACW_SND_WORD5_IP_DA_0_MASK                     0x0000000000FF0000ULL      // 23:16    47:16 IP_DA
#define PNIP_ACW_SND_WORD5_IP_DA_0_SHIFT                    16
#define PNIP_ACW_SND_WORD5_IP_DA_1_MASK                     0x00000000FF000000ULL      // 31:24
#define PNIP_ACW_SND_WORD5_IP_DA_1_SHIFT                    24
#define PNIP_ACW_SND_WORD5_IP_DA_2_MASK                     0x000000FF00000000ULL     // 39:32
#define PNIP_ACW_SND_WORD5_IP_DA_2_SHIFT                    32
#define PNIP_ACW_SND_WORD5_IP_DA_3_MASK                     0x0000FF0000000000ULL     // 47:40
#define PNIP_ACW_SND_WORD5_IP_DA_3_SHIFT                    40
#define PNIP_ACW_SND_WORD5_UDP_DP_MASK                      0xFFFF000000000000ULL     // 63:48
#define PNIP_ACW_SND_WORD5_UDP_DP_SHIFT                     48
// constant value
#define PNIP_ACW_SND_WORD5_RESERVED_VALUE                   0x0     // reserved
#define PNIP_ACW_SND_WORD5_UDP_SP_SELECT_INDEX0_VALUE       0x0     // UDP-SrcPort from Register "Local_SPort_Etype_0"
#define PNIP_ACW_SND_WORD5_UDP_SP_SELECT_INDEX1_VALUE       0x1     // UDP-SrcPort from Register "Local_SPort_Etype_1"
#define PNIP_ACW_SND_WORD5_UDP_SP_SELECT_INDEX2_VALUE       0x2     // UDP-SrcPort from Register "Local_SPort_Etype_2"
#define PNIP_ACW_SND_WORD5_UDP_SP_SELECT_INDEX3_VALUE       0x3     // UDP-SrcPort from Register "Local_SPort_Etype_3"
#define PNIP_ACW_SND_WORD5_UDP_DP_VALUE                     (EDDP_CRT_ETHER_TYPE)   // UDP Destination-Port
#define PNIP_ACW_SND_WORD5_IP_SA_SELECT_INDEX0_VALUE_VALUE  0x0     // Source-IP-Address from Register "Local_IP_SA_0"
#define PNIP_ACW_SND_WORD5_IP_SA_SELECT_INDEX1_VALUE_VALUE  0x1     // Source-IP-Address from Register "Local_IP_SA_1"
#define PNIP_ACW_SND_WORD5_IP_SA_SELECT_INDEX2_VALUE_VALUE  0x2     // Source-IP-Address from Register "Local_IP_SA_2"
#define PNIP_ACW_SND_WORD5_IP_SA_SELECT_INDEX3_VALUE_VALUE  0x3     // Source-IP-Address from Register "Local_IP_SA_3"

/* ------------------------------------------------------------------------- */
/* ACW Send - ACW_Word5 for RToverUDP IPv6                                   */
/* ------------------------------------------------------------------------- */
#define PNIP_R3_ACW_SND_WORD5_IPV6_DA1_MASK									0xFFFFFFFFFFFFFFFFULL  // 63:0
#define PNIP_R3_ACW_SND_WORD5_IPV6_DA1_SHIFT						        0

//------------------------------------------------------------------------------------------------------------------------------------

#define PNIP_R3_ACW_SND_WORD5_IPV6_DA1_0_MASK		0x00000000000000FFULL		// 7...0    | 63...0  DstIP_Address1
#define PNIP_R3_ACW_SND_WORD5_IPV6_DA1_0_SHIFT		0							//          |
#define PNIP_R3_ACW_SND_WORD5_IPV6_DA1_1_MASK		0x000000000000FF00ULL		// 15...8   |
#define PNIP_R3_ACW_SND_WORD5_IPV6_DA1_1_SHIFT		8							//          |
#define PNIP_R3_ACW_SND_WORD5_IPV6_DA1_2_MASK		0x0000000000FF0000ULL		// 23...16  |
#define PNIP_R3_ACW_SND_WORD5_IPV6_DA1_2_SHIFT		16							//          |
#define PNIP_R3_ACW_SND_WORD5_IPV6_DA1_3_MASK		0x00000000FF000000ULL		// 31...24  |
#define PNIP_R3_ACW_SND_WORD5_IPV6_DA1_3_SHIFT		24							//          |
#define PNIP_R3_ACW_SND_WORD5_IPV6_DA1_4_MASK		0x000000FF00000000ULL		// 39...32  |
#define PNIP_R3_ACW_SND_WORD5_IPV6_DA1_4_SHIFT		32							//          |
#define PNIP_R3_ACW_SND_WORD5_IPV6_DA1_5_MASK		0x0000FF0000000000ULL		// 47...40  |
#define PNIP_R3_ACW_SND_WORD5_IPV6_DA1_5_SHIFT		40							//          |
#define PNIP_R3_ACW_SND_WORD5_IPV6_DA1_6_MASK		0x00FF000000000000ULL		// 55...48  |
#define PNIP_R3_ACW_SND_WORD5_IPV6_DA1_6_SHIFT		48							//          |
#define PNIP_R3_ACW_SND_WORD5_IPV6_DA1_7_MASK		0xFF00000000000000ULL		// 63...56  |
#define PNIP_R3_ACW_SND_WORD5_IPV6_DA1_7_SHIFT		56							//         \|/


/* ------------------------------------------------------------------------- */
/* ACW Send - ACW_Word6 for RToverUDP IPv6                                   */
/* ------------------------------------------------------------------------- */
#define PNIP_R3_ACW_SND_WORD6_IPV6_DA2_MASK			0xFFFFFFFFFFFFFFFFULL       // 63:0
#define PNIP_R3_ACW_SND_WORD6_IPV6_DA2_SHIFT		0

//------------------------------------------------------------------------------------------------------------------------------------

#define PNIP_R3_ACW_SND_WORD6_IPV6_DA2_0_MASK       0x00000000000000FFULL		// 7...0    | 63...0  DstIP_Address2
#define PNIP_R3_ACW_SND_WORD6_IPV6_DA2_0_SHIFT      0							//          |
#define PNIP_R3_ACW_SND_WORD6_IPV6_DA2_1_MASK       0x000000000000FF00ULL		// 15...8   |
#define PNIP_R3_ACW_SND_WORD6_IPV6_DA2_1_SHIFT      8							//          |
#define PNIP_R3_ACW_SND_WORD6_IPV6_DA2_2_MASK       0x0000000000FF0000ULL		// 23...16  |
#define PNIP_R3_ACW_SND_WORD6_IPV6_DA2_2_SHIFT      16							//          |
#define PNIP_R3_ACW_SND_WORD6_IPV6_DA2_3_MASK       0x00000000FF000000ULL		// 31...24  |
#define PNIP_R3_ACW_SND_WORD6_IPV6_DA2_3_SHIFT      24							//          |
#define PNIP_R3_ACW_SND_WORD6_IPV6_DA2_4_MASK       0x000000FF00000000ULL		// 39...32  |
#define PNIP_R3_ACW_SND_WORD6_IPV6_DA2_4_SHIFT      32							//          |
#define PNIP_R3_ACW_SND_WORD6_IPV6_DA2_5_MASK       0x0000FF0000000000ULL		// 47...40  |
#define PNIP_R3_ACW_SND_WORD6_IPV6_DA2_5_SHIFT      40							//          |
#define PNIP_R3_ACW_SND_WORD6_IPV6_DA2_6_MASK       0x00FF000000000000ULL		// 55...48  |
#define PNIP_R3_ACW_SND_WORD6_IPV6_DA2_6_SHIFT      48							//          |
#define PNIP_R3_ACW_SND_WORD6_IPV6_DA2_7_MASK       0xFF00000000000000ULL		// 63...56  |
#define PNIP_R3_ACW_SND_WORD6_IPV6_DA2_7_SHIFT      56							//         \|/

/* ------------------------------------------------------------------------- */
/* ACW Send - Trog: Red-Orange-Green-Pointer with ACW lists                  */
/* ------------------------------------------------------------------------- */
typedef struct _PNIP_ACW_SND_TROG_TYPE      EDD_COMMON_MEM_ATTR *   PNIP_ACW_SND_TROG_PTR_TYPE;
PNIO_PACKED_ATTRIBUTE_PRE struct _PNIP_ACW_SND_TROG_TYPE
{
    LSA_UINT16          ACW_R_Ptr;      // ACW_R_Ptr -> ACW 1 -> ACW 2 -> ... -> ACW 0xFFFF
    LSA_UINT16          ACW_O_Ptr;
    LSA_UINT16          ACW_G_Ptr;
    LSA_UINT8           DG_CycleCount;
    LSA_UINT8           reserved;
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _PNIP_ACW_SND_TROG_TYPE  PNIP_ACW_SND_TROG_TYPE;
/* ACW_R_Ptr        : beginn of ACW list for Red phase      - address (offset) of first ACW in APICtrl-RAM, 64Bit alined        */
/* ACW_O_Ptr        : beginn of ACW list for Orange phase   - address (offset) of first ACW in APICtrl-RAM, 64Bit alined        */
/* ACW_G_Ptr        : beginn of ACW list for Green phase    - address (offset) of first ACW in APICtrl-RAM, 64Bit alined        */
/* DG_CycleCount    : CycleCount for Pack-Frames (ACW_DG)                                                                       */
/* reserved         : for 8 byte alignment                                                                                      */



/* ------------------------------------------------------------------------- */
/* statistic                                                                 */
/* ------------------------------------------------------------------------- */
typedef struct _PNIP_STATISTIC_PORTX_STRUCT_TYPE     EDD_COMMON_MEM_ATTR *  PNIP_STATISTIC_PORTX_STRUCT_PTR_TYPE;
PNIO_PACKED_ATTRIBUTE_PRE struct _PNIP_STATISTIC_PORTX_STRUCT_TYPE
{
	LSA_UINT32				Good_Bytes_Received_Px;             //0x000
	LSA_UINT32				Bad_Bytes_Received_Px;              //0x004
	LSA_UINT32				Total_Bytes_Received_Px;            //0x008
	LSA_UINT32				Good_Frames_Received_Px;            //0x00C
	LSA_UINT32				Bad_Frames_Received_Px;             //0x010
	LSA_UINT32				Total_Frames_Received_Px;           //0x014
	LSA_UINT32				UC_Frames_Received_Px;              //0x018
	LSA_UINT32				UC_Frames_Received_unknown_Px;      //0x01C
	LSA_UINT32				MC_Frames_Received_Px;              //0x020
	LSA_UINT32				MC_Frames_Received_unknown_Px;      //0x024
	LSA_UINT32				BroadCast_Frames_Received_Px;       //0x028
	LSA_UINT32				CRC_Alignment_Err_Px;               //0x02C
	LSA_UINT32				Frames_Dropped_L1_Px;               //0x030
	LSA_UINT32				Frames_Dropped_L2_Px;               //0x034
	LSA_UINT32				Frames_Dropped_L3_Px;               //0x038
	LSA_UINT32				HOL_Crossed_Px;                     //0x03C
	LSA_UINT32				QCW_Crossed_Px;                     //0x040
	LSA_UINT32				UC_Default_Forwarding_Px;           //0x044
	LSA_UINT32				MC_Default_Forwarding_Px;           //0x048
	LSA_UINT32				FrameAging_Crossed_Px;              //0x04C
	LSA_UINT32				Total_Bytes_Sent_Px;                //0x050
	LSA_UINT32				Total_Frames_Sent_Px;               //0x054
	LSA_UINT32				UniCast_Frames_Sent_Px;             //0x058
	LSA_UINT32				MultiCast_Frames_Sent_Px;           //0x05C
	LSA_UINT32				BroadCast_Frames_Sent_Px;           //0x060
	LSA_UINT32				IRT_DelayTime_Crossed_Px;           //0x064
	LSA_UINT32				Frames_less_64Bytes_Px;             //0x068
	LSA_UINT32				Frames_with_64Bytes_Px;             //0x06C
	LSA_UINT32				Frames_between_65to127Bytes_Px;     //0x070
	LSA_UINT32				Frames_between_128to255Bytes_Px;    //0x074
	LSA_UINT32				Frames_between_256to511Bytes_Px;    //0x078
	LSA_UINT32				Frames_between_512to1023Bytes_Px;   //0x07C
	LSA_UINT32				Frames_between_1024to1536Bytes_Px;  //0x080
	LSA_UINT32				Oversize_Frames_Px;                 //0x084
	LSA_UINT32				ErrCode_RxErr_Px;                   //0x088
	LSA_UINT32				ErrCode_FCSErr_Px;                  //0x08C
	LSA_UINT32				ErrCode_LongErr_Px;                 //0x090
	LSA_UINT32				ErrCode_ShortErr_Px;                //0x094
	LSA_UINT32				ErrCode_DgFCSErr_Px;                //0x098
	LSA_UINT32				ErrCode_InPackShortErr_Px;          //0x09C
	LSA_UINT32				ErrCode_InPackPosLengthErr_Px;      //0x0A0
	LSA_UINT32				ErrCode_NoResourceErr_Px;           //0x0A4
	LSA_UINT32				ErrCode_MACBufferErr_Px;            //0x0A8
	LSA_UINT32				ErrCode_MACSAErr_Px;                //0x0AC
	LSA_UINT32				ErrCode_DisableErr_Px;              //0x0B0
	LSA_UINT32				ErrCode_PackParErr_Px;              //0x0B4
	LSA_UINT32				ErrCode_PTCPErr_Px;                 //0x0B8
#if defined EDDP_CFG_HW_PNIP_REV2_SUPPORT
    LSA_UINT32              Total_bad_Frames_Sent_Px;           //0x0BC 
    LSA_UINT32				not_used[16];                       //Rev2: 0x0C0–0x0FF
#else
    LSA_UINT32				not_used[17];                       //Rev1: 0x0BC–0x0FF
#endif
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _PNIP_STATISTIC_PORTX_STRUCT_TYPE    PNIP_STATISTIC_PORTX_STRUCT_TYPE;


typedef struct _PNIP_STATISTIC_64_PORTX_STRUCT_TYPE     EDD_COMMON_MEM_ATTR *  PNIP_STATISTIC_64_PORTX_STRUCT_PTR_TYPE;
PNIO_PACKED_ATTRIBUTE_PRE struct _PNIP_STATISTIC_64_PORTX_STRUCT_TYPE
{
    LSA_UINT32				Good_Bytes_Received_64_Px;             //0x800
    LSA_UINT32				Bad_Bytes_Received_64_Px;              //0x804
    LSA_UINT32				Total_Bytes_Received_64_Px;            //0x808
    LSA_UINT32				Good_Frames_Received_64_Px;            //0x80C
    LSA_UINT32				Bad_Frames_Received_64_Px;             //0x810
    LSA_UINT32				Total_Frames_Received_64_Px;           //0x814
    LSA_UINT32				UC_Frames_Received_64_Px;              //0x818
    LSA_UINT32				UC_Frames_Received_64_unknown_Px;      //0x81C
    LSA_UINT32				MC_Frames_Received_64_Px;              //0x820
    LSA_UINT32				MC_Frames_Received_unknown_64_Px;      //0x824
    LSA_UINT32				BroadCast_Frames_Received_64_Px;       //0x828
    LSA_UINT32				CRC_Alignment_Err_64_Px;               //0x82C
    LSA_UINT32				Frames_Dropped_L1_64_Px;               //0x830
    LSA_UINT32				Frames_Dropped_L2_64_Px;               //0x834
    LSA_UINT32				Frames_Dropped_L3_64_Px;               //0x838
    LSA_UINT32				HOL_Crossed_64_Px;                     //0x83C
    LSA_UINT32				QCW_Crossed_64_Px;                     //0x840
    LSA_UINT32				UC_Default_Forwarding_64_Px;           //0x844
    LSA_UINT32				MC_Default_Forwarding_64_Px;           //0x848
    LSA_UINT32				FrameAging_Crossed_UC_64_Px;           //0x84C
    LSA_UINT32				Total_Bytes_Sent_64_Px;                //0x050
    LSA_UINT32				Total_Frames_Sent_64_Px;               //0x054
    LSA_UINT32				UniCast_Frames_Sent_64_Px;             //0x058
    LSA_UINT32				MultiCast_Frames_Sent_64_Px;           //0x05C
    LSA_UINT32				BroadCast_Frames_Sent_64_Px;           //0x060
    LSA_UINT32				IRT_DelayTime_Crossed_UC_64_Px;        //0x064
    LSA_UINT32				Frames_less_64Bytes_64_Px;             //0x068
    LSA_UINT32				Frames_with_64Bytes_64_Px;             //0x06C
    LSA_UINT32				Frames_between_65to127Bytes_64_Px;     //0x070
    LSA_UINT32				Frames_between_128to255Bytes_64_Px;    //0x074
    LSA_UINT32				Frames_between_256to511Bytes_64_Px;    //0x078
    LSA_UINT32				Frames_between_512to1023Bytes_64_Px;   //0x07C
    LSA_UINT32				Frames_between_1024to1536Bytes_64_Px;  //0x080
    LSA_UINT32				Oversize_Frames_64_Px;                 //0x084
    LSA_UINT32				ErrCode_RxErr_64_Px;                   //0x088
    LSA_UINT32				ErrCode_FCSErr_64_Px;                  //0x08C
    LSA_UINT32				ErrCode_LongErr_64_Px;                 //0x090
    LSA_UINT32				ErrCode_ShortErr_64_Px;                //0x094
    LSA_UINT32				ErrCode_DgFCSErr_64_Px;                //0x098
    LSA_UINT32				ErrCode_InPackShortErr_64_Px;          //0x09C
    LSA_UINT32				ErrCode_InPackPosLengthErr_64_Px;      //0x0A0
    LSA_UINT32				ErrCode_NoResourceErr_64_Px;           //0x0A4
    LSA_UINT32				ErrCode_MACBufferErr_64_Px;            //0x0A8
    LSA_UINT32				ErrCode_MACSAErr_64_Px;                //0x0AC
    LSA_UINT32				ErrCode_DisableErr_64_Px;              //0x0B0
    LSA_UINT32				ErrCode_PackParErr_64_Px;              //0x0B4
    LSA_UINT32				ErrCode_PTCPErr_64_Px;                 //0x0B8
    LSA_UINT32              Total_bad_Frames_Sent_64_Px;           //0x0BC
    LSA_UINT32              FrameAging_Crossed_MC_64_Px;           //0x0C0
    LSA_UINT32              FrameAging_Crossed_BC_64_Px;           //0x0C4
    LSA_UINT32              IRT_DelayTime_Crossed_MC_64_Px;        //0x0C8
    LSA_UINT32              IRT_DelayTime_Crossed_BC_64_Px;        //0x0CC
    LSA_UINT32              Good_Bytes_Sent_64_Px;                 //0x0D0
    LSA_UINT32              OutUCDiscards_API_RT_64_Px;            //0x0D4
    LSA_UINT32              OutMCDiscards_API_RT_64_Px;            //0x0D8
    LSA_UINT32              OutBCDiscards_API_RT_64_Px;            //0x0DC
    LSA_UINT32              NRT_Lim_UC_Bytes_Discarded_64;         //0x0E0
    LSA_UINT32              NRT_Lim_MCBC_Bytes_Discarded_64;       //0x0E4
    LSA_UINT32              HOL_netload_1_Overruns_64;             //0x0E8
    LSA_UINT32              HOL_netload_2_Overruns_64;             //0x0EC
    LSA_UINT32				not_used[4];                           //Rev3: 0x0F0–0x0FF
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _PNIP_STATISTIC_64_PORTX_STRUCT_TYPE    PNIP_STATISTIC_64_PORTX_STRUCT_TYPE;

typedef struct _PNIP_STATISTIC_MEM_STRUCT_TYPE     EDD_COMMON_MEM_ATTR *  PNIP_STATISTIC_MEM_STRUCT_PTR_TYPE;
PNIO_PACKED_ATTRIBUTE_PRE struct _PNIP_STATISTIC_MEM_STRUCT_TYPE
{
    PNIP_STATISTIC_PORTX_STRUCT_TYPE PortStatistic[4];     //Port1 (0x000-0x0FF) - Port4 (0x300-0x3FF)
    LSA_UINT32 unused[256];            //unused (0x400-0x7FF)
    PNIP_STATISTIC_64_PORTX_STRUCT_TYPE PortStatistic_64[4];  //Port1 (0x800) - Port4 (0xA00-0xAFF)
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _PNIP_STATISTIC_MEM_STRUCT_TYPE    PNIP_STATISTIC_MEM_STRUCT_TYPE;

/* ------------------------------------------------------------------------------------------------------------ */
/* PACKED(4) STRUCTS - OFF                                                                                      */
/* ------------------------------------------------------------------------------------------------------------ */
#ifdef K32_KRISC32OS
    #include "pnio_k32_pck_off.h"
#else
    #include "pnio_pck_off.h"
#endif

/*****************************************************************************/
/*  end of file eddp_hal_pnip_struct.h                                       */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  // of EDDP_HAL_PNIP_STRUCT_H
