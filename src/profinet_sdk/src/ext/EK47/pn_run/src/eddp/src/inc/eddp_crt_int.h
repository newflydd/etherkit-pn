#ifndef EDDP_CRT_INT_H                  /* ----- reinclude-protection ----  */
#define EDDP_CRT_INT_H

#ifdef __cplusplus                      /* If C++ - compiler: Use C linkage */
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
/*  F i l e               &F: eddp_crt_int.h                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  System interface                                                         */
/*  Defines, constants, types, macros, structs for EDDP                      */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who         What                                             */
#ifdef EDDP_MESSAGE
/*  22.10.08    Sergeev     Erstellung                                       */
/*  14.11.14    Tristan     removed defines
                                EDDP_CRT_PROVIDER_CYCLEPHASE_MAX_BYTE_CNT_32 */
/*                 EDDP_CRT_PROVIDER_CYCLEPHASE_MAX_BYTE_CNT_DIVISION_FACTOR */
#endif
/*****************************************************************************/


/* ============================================================================ */
/*                            paremeter                                         */
/* ============================================================================ */

// Maximum of allowed reduction
// ---------------------------------------------------------------------------------------------------------------------------------------------
// Reduction:                                                   1   2   4   8   16  32  64  128 256 512 1.024   2.048   4.096   8.192   16.384
// ---------------------------------------------------------------------------------------------------------------------------------------------
#define EDDP_CRT_MAX_PHASE_U1024_U16384         64      // max. Phase for reductions ->                 x       x       x       x       x
// Maximum number of entries in TBase array -> [0..1342] = 1343 entries
#define EDDP_CRT_MAX_COUNT_TBASE                (((2*EDDP_CRT_REDUCTION_512)-1) + (5*EDDP_CRT_MAX_PHASE_U1024_U16384))

// Maximum number of Bits in FrameID use for index in RBase-Array   -> pRBaseEntry[RBaseIndex]
#define EDDP_CRT_MAX_RINDEX_SIZE                3                                               // use Bit(3:1) in FrameID
#define EDDP_CRT_MAX_COUNT_RBASE                (EDDP_GET_2_POTENZ(EDDP_CRT_MAX_RINDEX_SIZE))   // 2 ^ 3 = 8 Pointer
    
#define EDDP_R3_CRT_MAX_COUNT_RBASE             (EDDP_R3_CRT_FID_AREA_ENTRIES)

/* ---------------------------------------------------------------------------- */
/* Maximum number of Provider/Consumer in EDDP-CRT for one device (DDB)         */
/* ---------------------------------------------------------------------------- */
// max. number of pack frames
#define EDDP_CRT_MAX_PACK_FRAMES_ERTEC200P      8
/* ---------------------------------------------------------------------------- */
/* ERTEC200+ Provider                                                           */
/* ---------------------------------------------------------------------------- */
// max. count of all application providers in Provider-ADD
#define EDDP_CRT_MAX_PROVIDER_ERTEC200P         18

#ifdef EDDP_CRT_SUPPORT_ACW_WAIT_ON
    // max. count of MultiCast providers
    #define EDDP_CRT_MAX_MC_PROVIDER_ERTEC200P          ((EDDP_CRT_MAX_PROVIDER_ERTEC200P) / 2)
    // max. count of all provider ACWs (54)
    #define EDDP_CRT_MAX_PROVIDER_ACW_ERTEC200P                                                         \
                (  (4 * (EDDP_CRT_MAX_MC_PROVIDER_ERTEC200P))                                           \
                 + (2 * ((EDDP_CRT_MAX_PROVIDER_ERTEC200P - EDDP_CRT_MAX_MC_PROVIDER_ERTEC200P)))       \
                )
    // max. count of all provider ACW-DGs (18)
    #define EDDP_CRT_MAX_PROVIDER_ACW_DG_ERTEC200P      (EDDP_CRT_MAX_PROVIDER_ERTEC200P)
    // max. count of all provider Trogs (37): one more because of "ACW_move" command
    #define EDDP_CRT_MAX_PROVIDER_TROG_ERTEC200P                                                        \
                (  (2 * (EDDP_CRT_MAX_MC_PROVIDER_ERTEC200P))                                           \
                 + (2 * ((EDDP_CRT_MAX_PROVIDER_ERTEC200P - EDDP_CRT_MAX_MC_PROVIDER_ERTEC200P)))       \
                 + 1                                                                                    \
                )
#else
    // max. count of all provider ACWs (36)
    #define EDDP_CRT_MAX_PROVIDER_ACW_ERTEC200P         (2 * (EDDP_CRT_MAX_PROVIDER_ERTEC200P))
    // max. count of all provider ACW-DGs (18)
    #define EDDP_CRT_MAX_PROVIDER_ACW_DG_ERTEC200P      (EDDP_CRT_MAX_PROVIDER_ERTEC200P)
    // max. count of all provider Trogs (37): one more because of "ACW_move" command
    #define EDDP_CRT_MAX_PROVIDER_TROG_ERTEC200P        (EDDP_CRT_MAX_PROVIDER_ACW_ERTEC200P + 1)
#endif

// max. count of all provider pack groups (8)
#define EDDP_CRT_MAX_PROVIDER_PACK_GROUP_ERTEC200P      (EDDP_CRT_MAX_PACK_FRAMES_ERTEC200P)
// max. count of all provider DataStatus (36): all application providers * 2
// worst case: every ACW_DG needs one Pack-ACW - every ACW and ACW_DG needs a DataStatus
#define EDDP_CRT_MAX_PROVIDER_DATASTATUS_ERTEC200P      (2 * (EDDP_CRT_MAX_PROVIDER_ERTEC200P))
/* ---------------------------------------------------------------------------- */
/* ERTEC200+ Consumer                                                           */
/* ---------------------------------------------------------------------------- */
// max. count of all application consumers in Consumer-ADD
#define EDDP_CRT_MAX_CONSUMER_ERTEC200P                 (18)

// max. count of all consumer ACWs (18)
#define EDDP_CRT_MAX_CONSUMER_ACW_ERTEC200P             (EDDP_CRT_MAX_CONSUMER_ERTEC200P)
// max. count of all ACW containers used for ACW-DGs (8)
#define EDDP_CRT_MAX_CONSUMER_ACW_CONTAINER_ERTEC200P   (EDDP_CRT_MAX_PACK_FRAMES_ERTEC200P)
// max. count of all consumer ACW-DGs (18)
#define EDDP_CRT_MAX_CONSUMER_ACW_DG_ERTEC200P          (EDDP_CRT_MAX_CONSUMER_ERTEC200P)
#define EDDP_CRT_MAX_INBOUND_DG_CNT_ERTEC200P           (64)
#define EDDP_CRT_MAX_INBOUND_GRP_ERTEC200P              (8)
/* ---------------------------------------------------------------------------- */
/* HERA general                                                                 */
/* ---------------------------------------------------------------------------- */
// max. count of RTC3 legacy startup providers/consumers
#define EDDP_CRT_MAX_RTC3_LEGACY_STARTUP_HERA           (64)
// additional temporary AR for Configuration in Run (CiR)
#define EDDP_CRT_MAX_ADDITIONAL_CIR_AR_HERA             (1)
// max. count of pack groups in controller
#define EDDP_CRT_MAX_PACKGROUP_CONTROLLER_HERA          (64)
// max. count of pack groups in device
#define EDDP_CRT_MAX_PACKGROUP_DEVICE_HERA              (8)
/* ---------------------------------------------------------------------------- */
/* HERA Provider                                                                */
/* ---------------------------------------------------------------------------- */
// max. count of providers (without CiR)
#define EDDP_CRT_BASE_PROVIDER_CNT_HERA                 (512)
// max. count of providers (without CiR) for small send clocks
#define EDDP_CRT_BASE_PROVIDER_CNT_HERA_SMALL_SCF       (192)
// max. count of providers within one AR
#define EDDP_CRT_MAX_AR_PROVIDER_HERA                   (4)
// max. count of Multicast providers within one AR
#define EDDP_CRT_MAX_AR_MC_PROVIDER_HERA                (4)
// max. count of ports for RTC3 Multicast providers
#define EDDP_CRT_MAX_RTC3_MC_PROVIDER_MAX_PORT_HERA     (3)
// max. count of RTC3 Multicast providers
#define EDDP_CRT_MAX_RTC3_MC_PROVIDER_HERA              (64)
// max. count of MRPD providers
#define EDDP_CRT_MAX_RTC3_MRPD_PROVIDER_HERA            (256)
// max. count of providers
#define EDDP_CRT_MAX_PROVIDER_CNT_HERA                  (EDDP_CRT_BASE_PROVIDER_CNT_HERA + (EDDP_CRT_MAX_ADDITIONAL_CIR_AR_HERA * EDDP_CRT_MAX_AR_PROVIDER_HERA))
// max. count of providers for small send clocks
#define EDDP_CRT_MAX_PROVIDER_CNT_HERA_SMALL_SCF        (EDDP_CRT_BASE_PROVIDER_CNT_HERA_SMALL_SCF + (EDDP_CRT_MAX_ADDITIONAL_CIR_AR_HERA * EDDP_CRT_MAX_AR_PROVIDER_HERA))    
// max. count of all provider Trogs for IFA and IFB: one more because of "ACW_move" command
#define EDDP_CRT_MAX_PROVIDER_TROG_HERA_IFA             (EDDP_CRT_MAX_PROVIDER_CNT_HERA + \
                                                         EDDP_CRT_MAX_RTC3_LEGACY_STARTUP_HERA + \
                                                         EDDP_CRT_MAX_ADDITIONAL_CIR_AR_HERA * EDDP_CRT_MAX_AR_PROVIDER_HERA + \
                                                         EDDP_CRT_MAX_RTC3_MC_PROVIDER_HERA * (EDDP_CRT_MAX_RTC3_MC_PROVIDER_MAX_PORT_HERA - 1)\
                                                         + 1)
#define EDDP_CRT_MAX_PROVIDER_TROG_HERA_IFB             (EDDP_CRT_MAX_PROVIDER_CNT_HERA + \
                                                         EDDP_CRT_MAX_RTC3_LEGACY_STARTUP_HERA + \
                                                         EDDP_CRT_MAX_ADDITIONAL_CIR_AR_HERA * EDDP_CRT_MAX_AR_PROVIDER_HERA + \
                                                         EDDP_CRT_MAX_RTC3_MC_PROVIDER_HERA * 1 \
                                                         + 1)
// max. count of provider ACWs for IFA and IFB
#define EDDP_CRT_MAX_PROVIDER_ACW_HERA_IFA               (EDDP_CRT_MAX_RTC3_MC_PROVIDER_HERA * EDDP_CRT_MAX_RTC3_MC_PROVIDER_MAX_PORT_HERA + \
                                                         (EDDP_CRT_MAX_RTC3_LEGACY_STARTUP_HERA + EDDP_CRT_MAX_ADDITIONAL_CIR_AR_HERA * EDDP_CRT_MAX_AR_PROVIDER_HERA) * 2 + \
                                                          EDDP_CRT_BASE_PROVIDER_CNT_HERA - EDDP_CRT_MAX_RTC3_MC_PROVIDER_HERA - EDDP_CRT_MAX_RTC3_LEGACY_STARTUP_HERA)
#define EDDP_CRT_MAX_PROVIDER_ACW_HERA_IFB               (EDDP_CRT_MAX_RTC3_MC_PROVIDER_HERA * 2 + \
                                                         (EDDP_CRT_MAX_RTC3_LEGACY_STARTUP_HERA + EDDP_CRT_MAX_ADDITIONAL_CIR_AR_HERA * EDDP_CRT_MAX_AR_PROVIDER_HERA) * 2 + \
                                                          EDDP_CRT_BASE_PROVIDER_CNT_HERA - EDDP_CRT_MAX_RTC3_MC_PROVIDER_HERA - EDDP_CRT_MAX_RTC3_LEGACY_STARTUP_HERA)
// max. count of all provider ACW-DGs
#define EDDP_CRT_MAX_PROVIDER_ACW_DG_HERA                (EDDP_CRT_BASE_PROVIDER_CNT_HERA)
// max. count of all provider DataStatus
#define EDDP_CRT_MAX_PROVIDER_DATASTATUS_HERA            (EDDP_CRT_MAX_PROVIDER_CNT_HERA + \
                                                          EDDP_CRT_MAX_PACKGROUP_CONTROLLER_HERA + \
                                                          EDDP_CRT_MAX_PACKGROUP_DEVICE_HERA)
// max. hardware send list entries per port depending on used interface
// NOTE: it must be ensured that the list is large enough to hold all entries, round up!!!
// NOTE: +1 for maximum reduction count for each DG_CycleCount
#define EDDP_CRT_MAX_PROVIDER_HW_SEND_LIST_ENTRIES_IFA   ((EDDP_CRT_MAX_PROVIDER_ACW_HERA_IFA + 1 + EDDP_CRT_REDUCTION_NR_MAX) / 4)
#define EDDP_CRT_MAX_PROVIDER_HW_SEND_LIST_ENTRIES_IFB   ((EDDP_CRT_MAX_PROVIDER_ACW_HERA_IFB + 1 + EDDP_CRT_REDUCTION_NR_MAX) / 4)
/* ---------------------------------------------------------------------------- */
/* HERA Consumer                                                                */
/* ---------------------------------------------------------------------------- */
// max. count of consumers (without CiR)
#define EDDP_CRT_BASE_CONSUMER_CNT_HERA                 (512)
// max. count of consumers within one AR
#define EDDP_CRT_MAX_AR_CONSUMER_HERA                   (4)
// max. count of Multicast consumers within one AR
#define EDDP_CRT_MAX_AR_MC_CONSUMER_HERA                (4)
// max. count of RTC3 Multicast consumers
#define EDDP_CRT_MAX_RTC3_MC_CONSUMER_HERA              (128)
// max. count of MRPD consumers
#define EDDP_CRT_MAX_RTC3_MRPD_CONSUMER_HERA            (256)
// max. count of consumers
#define EDDP_CRT_MAX_CONSUMER_CNT_HERA                  (EDDP_CRT_BASE_CONSUMER_CNT_HERA + (EDDP_CRT_MAX_ADDITIONAL_CIR_AR_HERA * EDDP_CRT_MAX_AR_CONSUMER_HERA))
/* ---------------------------------------------------------------------------- */

// CycleLength(ns) = CycleBaseFactor * 31250 ns
#define EDDP_CRT_CYCLE_LENGTH_BASE_NS           31250

// Maximum number of provider groups
#define EDDP_CRT_MAX_PROVIDER_GROUP             7       // GroupId: 0..6 (Group7 is reserved for DFP)
// Number of GroupId
#define EDDP_CRT_GROUP_ID_0                     0
#define EDDP_CRT_GROUP_ID_1                     1
#define EDDP_CRT_GROUP_ID_2                     2
#define EDDP_CRT_GROUP_ID_3                     3
#define EDDP_CRT_GROUP_ID_4                     4
#define EDDP_CRT_GROUP_ID_5                     5
#define EDDP_CRT_GROUP_ID_6                     6
#define EDDP_CRT_GROUP_ID_7                     7       // only for DFP

// Minimum, Maximum CRNumber in Provider/Consumer-Add/Control for PERIF
#define EDDP_CRT_MIN_CRNUMBER_PERIF_ERTEC200P   1
#define EDDP_CRT_MAX_CRNUMBER_PERIF_ERTEC200P   27
// check macro: is CRNumber in range for PERIF ?
#define EDDP_CRT_IS_CRNUMBER_IN_PERIF_RANGE(CRNumber) \
            ( (CRNumber >= EDDP_CRT_MIN_CRNUMBER_PERIF_ERTEC200P) && (CRNumber <= EDDP_CRT_MAX_CRNUMBER_PERIF_ERTEC200P) )

// Maximum number of AppGroup for Provider
#define EDDP_CRT_PROV_MAX_APPGROUP              2
// Maximum number of AppGroup for Consumer (0..3)
#define EDDP_CRT_CONS_MAX_APPGROUP              3


/* -------------------------------------------------------- */
/*      min. length of RT frame                             */
/* -------------------------------------------------------- */
/*          Name                    Byte                    */
/* -------------------------------------------------------- */
/*          Dst-MAC                 6                       */
/*          Src-MAC                 6                       */
/*          VLAN-Tag                4 (not in RTC3)         */
/*          EtherType               2                       */
/*          FrameID                 2                       */
/*          IO-Data	min.            40                      */
/*          APDUStatus              4                       */
/*          FCS                     4                       */
/* -------------------------------------------------------- */
/*          FrameLength             64 (24 + 40)            */
/* -------------------------------------------------------- */
#define EDDP_CRT_RT_FRAME_LEN_HEADER_APDU_FCS   24      // length of Frame[Header+APDUStatus+FCS] (without VLAN-Tag)
#define EDDP_CRT_RT_FRAME_LEN_VLANTAG           4       // length of VLAN-Tag

// Minimum, Maximum DFP-SFPosition in Provider/Consumer-Add/Control
#define EDDP_CRT_MIN_DFP_SFPOSITION             1
#define EDDP_CRT_MAX_DFP_SFPOSITION             127
// check macro: is SFPosition in valid range ?
#define EDDP_CRT_IS_DFP_SFPOSITION_IN_VALID_RANGE(SFPosition) \
            ( (SFPosition >= EDDP_CRT_MIN_DFP_SFPOSITION) && (SFPosition <= EDDP_CRT_MAX_DFP_SFPOSITION) )

// number of ACWs in Provider-ADD for one provider
#define EDDP_CRT_PROV_ADD_NUMBER_OF_ACW_DEFAULT             1
#define EDDP_CRT_PROV_ADD_NUMBER_OF_ACW_RTC3_LEGACY         2
#define EDDP_CRT_PROV_ADD_NUMBER_OF_ACW_RTC3_MRPD           2
#define EDDP_CRT_PROV_ADD_NUMBER_OF_ACW_RTC3_DFP            0


/* ------------------------------------------------------------------------- */
/* CPM FrameID areas - link FrameID to ACW index entry, used by HW to        */ 
/*                     determine the corresponding ACW                       */   
/* ------------------------------------------------------------------------- */

#define EDDP_R3_CRT_FID_AREA_COUNT                 (6)

#define EDDP_R3_CRT_FID_AREA_DECODE_1024           (0)
#define EDDP_R3_CRT_FID_AREA_DECODE_2048           (1)
#define EDDP_R3_CRT_FID_AREA_DECODE_4096           (2)
#define EDDP_R3_CRT_FID_AREA_DECODE_OFF            (3)

#define EDDP_R3_CRT_FID_AREA_SIZE_0                (EDDP_R3_CRT_FID_AREA_DECODE_4096) /* 4096 decoded */
#define EDDP_R3_CRT_FID_AREA_SIZE_1                (EDDP_R3_CRT_FID_AREA_DECODE_1024) /* 1024 decoded */
#define EDDP_R3_CRT_FID_AREA_SIZE_2                (EDDP_R3_CRT_FID_AREA_DECODE_1024) /* 1024 decoded */
#define EDDP_R3_CRT_FID_AREA_SIZE_3                (EDDP_R3_CRT_FID_AREA_DECODE_1024) /* 1024 decoded */
#define EDDP_R3_CRT_FID_AREA_SIZE_4                (EDDP_R3_CRT_FID_AREA_DECODE_1024) /* 1024 decoded */
#define EDDP_R3_CRT_FID_AREA_SIZE_5                (EDDP_R3_CRT_FID_AREA_DECODE_OFF)  /* not used */

#define EDDP_R3_CRT_FID_AREA_ENTRIES_0             (4096) /* 4096 decoded */
#define EDDP_R3_CRT_FID_AREA_ENTRIES_1             (1024) /* 1024 decoded */
#define EDDP_R3_CRT_FID_AREA_ENTRIES_2             (1024) /* 1024 decoded */
#define EDDP_R3_CRT_FID_AREA_ENTRIES_3             (1024) /* 1024 decoded */
#define EDDP_R3_CRT_FID_AREA_ENTRIES_4             (1024) /* 1024 decoded */
#define EDDP_R3_CRT_FID_AREA_ENTRIES_5             (1024) /* not used, but reserved in API-RT-CTRL-RAM */

#define EDDP_R3_CRT_FID_AREA_RANGE_0               (0x00)
#define EDDP_R3_CRT_FID_AREA_RANGE_1               (0x3F)
#define EDDP_R3_CRT_FID_AREA_RANGE_2               (0x20)
#define EDDP_R3_CRT_FID_AREA_RANGE_3               (0x30)
#define EDDP_R3_CRT_FID_AREA_RANGE_4               (0x3E)
#define EDDP_R3_CRT_FID_AREA_RANGE_5               (0x00) /* don't care, not used */

#define EDDP_R3_CRT_FID_AREA_FRAMEID_CMP_0         (EDDP_R3_CRT_FID_AREA_RANGE_0 >> EDDP_R3_CRT_FID_AREA_SIZE_0)
#define EDDP_R3_CRT_FID_AREA_FRAMEID_CMP_1         (EDDP_R3_CRT_FID_AREA_RANGE_1 >> EDDP_R3_CRT_FID_AREA_SIZE_1)
#define EDDP_R3_CRT_FID_AREA_FRAMEID_CMP_2         (EDDP_R3_CRT_FID_AREA_RANGE_2 >> EDDP_R3_CRT_FID_AREA_SIZE_2)
#define EDDP_R3_CRT_FID_AREA_FRAMEID_CMP_3         (EDDP_R3_CRT_FID_AREA_RANGE_3 >> EDDP_R3_CRT_FID_AREA_SIZE_3)
#define EDDP_R3_CRT_FID_AREA_FRAMEID_CMP_4         (EDDP_R3_CRT_FID_AREA_RANGE_4 >> EDDP_R3_CRT_FID_AREA_SIZE_4)
#define EDDP_R3_CRT_FID_AREA_FRAMEID_CMP_5         (EDDP_R3_CRT_FID_AREA_RANGE_5 >> EDDP_R3_CRT_FID_AREA_SIZE_5) /* don't care, not used */

#define EDDP_R3_CRT_FID_AREA_FRAMEID_SHIFT_0       (10 + EDDP_R3_CRT_FID_AREA_SIZE_0)
#define EDDP_R3_CRT_FID_AREA_FRAMEID_SHIFT_1       (10 + EDDP_R3_CRT_FID_AREA_SIZE_1)
#define EDDP_R3_CRT_FID_AREA_FRAMEID_SHIFT_2       (10 + EDDP_R3_CRT_FID_AREA_SIZE_2)
#define EDDP_R3_CRT_FID_AREA_FRAMEID_SHIFT_3       (10 + EDDP_R3_CRT_FID_AREA_SIZE_3)
#define EDDP_R3_CRT_FID_AREA_FRAMEID_SHIFT_4       (10 + EDDP_R3_CRT_FID_AREA_SIZE_4)
#define EDDP_R3_CRT_FID_AREA_FRAMEID_SHIFT_5       (10 + EDDP_R3_CRT_FID_AREA_SIZE_5) /* don't care, not used */

#define EDDP_R3_CRT_FID_AREA_FRAMEID_MASK_0        (0x1FFF)
#define EDDP_R3_CRT_FID_AREA_FRAMEID_MASK_1        (0x07FF)
#define EDDP_R3_CRT_FID_AREA_FRAMEID_MASK_2        (0x07FF)
#define EDDP_R3_CRT_FID_AREA_FRAMEID_MASK_3        (0x07FF)
#define EDDP_R3_CRT_FID_AREA_FRAMEID_MASK_4        (0x07FF)
#define EDDP_R3_CRT_FID_AREA_FRAMEID_MASK_5        (0x07FF) /* don't care, not used */

#define EDDP_R3_CRT_FID_AREA_IDX_START_0           (0)
#define EDDP_R3_CRT_FID_AREA_IDX_START_1           (EDDP_R3_CRT_FID_AREA_ENTRIES_0)
#define EDDP_R3_CRT_FID_AREA_IDX_START_2           (EDDP_R3_CRT_FID_AREA_IDX_START_1     + \
                                                    EDDP_R3_CRT_FID_AREA_ENTRIES_1)
#define EDDP_R3_CRT_FID_AREA_IDX_START_3           (EDDP_R3_CRT_FID_AREA_IDX_START_2     + \
                                                    EDDP_R3_CRT_FID_AREA_ENTRIES_2)
#define EDDP_R3_CRT_FID_AREA_IDX_START_4           (EDDP_R3_CRT_FID_AREA_IDX_START_3     + \
                                                    EDDP_R3_CRT_FID_AREA_ENTRIES_3)
#define EDDP_R3_CRT_FID_AREA_IDX_START_5           (EDDP_R3_CRT_FID_AREA_IDX_START_4     + \
                                                    EDDP_R3_CRT_FID_AREA_ENTRIES_4)
#define EDDP_R3_CRT_FID_AREA_ENTRIES               (EDDP_R3_CRT_FID_AREA_IDX_START_5     + \
                                                    EDDP_R3_CRT_FID_AREA_ENTRIES_5)


/* ============================================================================ */
/*                              defines                                         */
/* ============================================================================ */

#define EDDP_R3_CRT_PPM_IO_BUFFER_DFP_HCS               2
#define EDDP_R3_CRT_PPM_IO_BUFFER_SEQ_ID_LEN            2
#define EDDP_R3_CRT_PPM_IO_BUFFER_DS_LEN                1
#define EDDP_R3_CRT_PPM_IO_BUFFER_OVERHEAD             (EDDP_R3_CRT_PPM_IO_BUFFER_SEQ_ID_LEN + \
                                                        EDDP_R3_CRT_PPM_IO_BUFFER_SEQ_ID_LEN + \
                                                        EDDP_R3_CRT_PPM_IO_BUFFER_DS_LEN)
  
#define EDDP_R3_CRT_PPM_IO_BUFFER_DFP_SF_POS_LEN        1
#define EDDP_R3_CRT_PPM_IO_BUFFER_DFP_SF_LENGTH_LEN     1
#define EDDP_R3_CRT_PPM_IO_BUFFER_DFP_SF_CC_LEN         1
#define EDDP_R3_CRT_PPM_IO_BUFFER_DFP_SF_DS_LEN         1
#define EDDP_R3_CRT_PPM_IO_BUFFER_DFP_SF_CRC_LEN        2
#define EDDP_R3_CRT_PPM_IO_BUFFER_DFP_SF_HEADER        (EDDP_R3_CRT_PPM_IO_BUFFER_DFP_SF_POS_LEN + \
                                                        EDDP_R3_CRT_PPM_IO_BUFFER_DFP_SF_LENGTH_LEN + \
                                                        EDDP_R3_CRT_PPM_IO_BUFFER_DFP_SF_CC_LEN + \
                                                        EDDP_R3_CRT_PPM_IO_BUFFER_DFP_SF_DS_LEN + \
                                                        EDDP_R3_CRT_PPM_IO_BUFFER_DFP_SF_CRC_LEN)
#define EDDP_R3_CRT_PPM_IO_BUFFER_DFP_TERM              2
    
//    SFPosition, SFDataLength, SFCycleCounter, DataStatus and SFCRC16
    
// invalid value
#define EDDP_CRT_INVALID_VALUE_8                0xFF
#define EDDP_CRT_INVALID_VALUE_16               0xFFFF
#define EDDP_CRT_INVALID_VALUE_32               0xFFFFFFFF
#define EDDP_CRT_INVALID_VALUE_64               0xFFFFFFFFFFFFFFFF

// Defines for Prov/Cons.Param.RTClass
#define EDDP_CRT_RTCLASS_UNKNOWN                0x0
#define EDDP_CRT_RTCLASS_RT                     0x1
#define EDDP_CRT_RTCLASS_UDP                    0x2

// Defines for RTClasses dependent of FrameID range: RTClass1/2/3
#define EDDP_CRT_FRAMEID_RTCLASS_INVALID        (EDDP_CRT_INVALID_VALUE_8)
#define EDDP_CRT_FRAMEID_RTCLASS_1              0x1     // RTClass1 (Green)     : RT, RToverUDP
#define EDDP_CRT_FRAMEID_RTCLASS_2              0x2     // RTClass2 (Orange)    : IRTflex
#define EDDP_CRT_FRAMEID_RTCLASS_3              0x3     // RTClass3 (Red)       : IRTtop

// Defines for FrameID ranges
#define EDDP_CRT_FRAMEID_RT_CLASS1_START        (EDD_SRT_FRAMEID_CSRT_START2)       // RTClass1 (Green)     : RT, RToverUDP
#define EDDP_CRT_FRAMEID_RT_CLASS1_STOP         (EDD_SRT_FRAMEID_CSRT_STOP2)
#define EDDP_CRT_FRAMEID_RT_CLASS2_START        (EDD_SRT_FRAMEID_CSRT_START1)       // RTClass2 (Orange)    : IRTflex
#define EDDP_CRT_FRAMEID_RT_CLASS2_STOP         (EDD_SRT_FRAMEID_CSRT_STOP1)
#define EDDP_CRT_FRAMEID_RT_CLASS3_START        (EDD_SRT_FRAMEID_IRT_START)         // RTClass3 (Red)       : IRTtop
#define EDDP_CRT_FRAMEID_RT_CLASS3_STOP         (EDD_SRT_FRAMEID_IRT_STOP)
// Min, Max of FrameID
#define EDDP_CRT_FRAMEID_MIN                    (EDDP_CRT_FRAMEID_RT_CLASS3_START)
#define EDDP_CRT_FRAMEID_MAX                    (EDDP_CRT_FRAMEID_RT_CLASS1_STOP)

#define EDDP_CRT_CHAIN_IDX_END                  (EDDP_CRT_INVALID_VALUE_16)

// States for EDDP-Provider/Consumer
// NOTE: States must be defined in ascending order !!!
#define EDDP_CRT_STATE_FREE                     0x0     // in FreeList chained and unused
#define EDDP_CRT_STATE_SW_USED                  0x1     // used in SW management: chained with ADD in UsedList but no ACW in HW
#define EDDP_CRT_STATE_HW_PASSIVE               0x2     // used in SW/HW: ACW in HW initialized and chained, ACW in HW is passive
#define EDDP_CRT_STATE_HW_ACTIVE                0x3     // used in SW/HW: ACW in HW is active

// CRT commandos: PPM
#define EDDP_CRT_CMD_PPM_NOP                            0x0100      // PPM-NOP
#define EDDP_CRT_CMD_PPM_ON                             0x0101      // PPM activate
#define EDDP_CRT_CMD_PPM_OFF                            0x0102      // PPM deactivate
#define EDDP_CRT_CMD_PPM_ACW_IN                         0x0105      // Provider-ACW chain in
#define EDDP_CRT_CMD_PPM_ACW_OUT                        0x0106      // Provider-ACW chain out
#define EDDP_CRT_CMD_PPM_ACW_MOVE                       0x0107      // Provider-ACW move to new phase
#define EDDP_CRT_CMD_PPM_ACW_XCHANGE_BUFF               0x0108      // exchange buffer
//      ==>> PPM for pack group
#define EDDP_CRT_CMD_PPM_PACK_ACW_IN                    0x010B      // Pack Provider-ACW chain in
#define EDDP_CRT_CMD_PPM_PACK_ACW_OUT                   0x010C      // Pack Provider-ACW chain out
#define EDDP_CRT_CMD_PPM_PACK_ACW_DG_ON                 0x010D      // Pack Provider-ACW-DG activate
#define EDDP_CRT_CMD_PPM_PACK_ACW_DG_OFF                0x010E      // Pack Provider-ACW-DG deactivate
#define EDDP_CRT_CMD_PPM_PACK_ACW_DG_ON_ACW_IN          0x010F      // Pack Provider-ACW-DG -> ACW of this ACW_DG is chained in
#define EDDP_CRT_CMD_PPM_PACK_ACW_DG_OFF_ACW_OUT        0x0110      // Pack Provider-ACW-DG -> ACW of this ACW_DG is chained out
//      ==>> PPM other
#define EDDP_CRT_CMD_PPM_STARTTIME                      0x0111      // set register "PPM_Starttime"
#define EDDP_CRT_CMD_PPM_DELAY                          0x0112      // delay command
// CRT commandos: CPM
#define EDDP_CRT_CMD_CPM_NOP                            0x0200      // CPM-NOP
#define EDDP_CRT_CMD_CPM_ON                             0x0201      // CPM activate
#define EDDP_CRT_CMD_CPM_OFF                            0x0202      // CPM deactivate
#define EDDP_CRT_CMD_CPM_ACW_ON                         0x0203      // Consumer-ACW activate
#define EDDP_CRT_CMD_CPM_ACW_OFF                        0x0204      // Consumer-ACW deactivate
#define EDDP_CRT_CMD_CPM_ACW_IN                         0x0205      // Consumer-ACW chain in
#define EDDP_CRT_CMD_CPM_ACW_OUT                        0x0206      // Consumer-ACW chain out
#define EDDP_CRT_CMD_CPM_ACW_SET_UNKNOWN                0x0207      // Set Consumer to Unknown
#define EDDP_CRT_CMD_CPM_ACW_XCHANGE_BUFF               0x0208      // exchange buffer
// CRT commandos: ApplTimer
#define EDDP_CRT_CMD_APPL_TIMER_CONNECT_SYNC            0x0300      // Connect_SYNC_A/B
#define EDDP_CRT_CMD_APPL_TIMER_UPDATE_APPLCYCLE        0x0301      // Update_APPLCYCLE

// CRT-Reductions for Register PNIP_REG_ACW_TREDUCTIONSELECT_IFA/IFB
#define EDDP_CRT_REDUCTION_1                    1
#define EDDP_CRT_REDUCTION_2                    2
#define EDDP_CRT_REDUCTION_4                    4
#define EDDP_CRT_REDUCTION_8                    8
#define EDDP_CRT_REDUCTION_16                   16
#define EDDP_CRT_REDUCTION_32                   32
#define EDDP_CRT_REDUCTION_64                   64
#define EDDP_CRT_REDUCTION_128                  128
#define EDDP_CRT_REDUCTION_256                  256
#define EDDP_CRT_REDUCTION_512                  512
#define EDDP_CRT_REDUCTION_1024                 1024
#define EDDP_CRT_REDUCTION_2048                 2048
#define EDDP_CRT_REDUCTION_4096                 4096
#define EDDP_CRT_REDUCTION_8192                 8192
#define EDDP_CRT_REDUCTION_16384                16384
// CRT-Reductions: Mask/Shift
#define EDDP_CRT_REDUCTION_1_MASK               0x00000001
#define EDDP_CRT_REDUCTION_1_SHIFT              0
#define EDDP_CRT_REDUCTION_2_MASK               0x00000002
#define EDDP_CRT_REDUCTION_2_SHIFT              1
#define EDDP_CRT_REDUCTION_4_MASK               0x00000004
#define EDDP_CRT_REDUCTION_4_SHIFT              2
#define EDDP_CRT_REDUCTION_8_MASK               0x00000008
#define EDDP_CRT_REDUCTION_8_SHIFT              3
#define EDDP_CRT_REDUCTION_16_MASK              0x00000010
#define EDDP_CRT_REDUCTION_16_SHIFT             4
#define EDDP_CRT_REDUCTION_32_MASK              0x00000020
#define EDDP_CRT_REDUCTION_32_SHIFT             5
#define EDDP_CRT_REDUCTION_64_MASK              0x00000040
#define EDDP_CRT_REDUCTION_64_SHIFT             6
#define EDDP_CRT_REDUCTION_128_MASK             0x00000080
#define EDDP_CRT_REDUCTION_128_SHIFT            7
#define EDDP_CRT_REDUCTION_256_MASK             0x00000100
#define EDDP_CRT_REDUCTION_256_SHIFT            8
#define EDDP_CRT_REDUCTION_512_MASK             0x00000200
#define EDDP_CRT_REDUCTION_512_SHIFT            9
#define EDDP_CRT_REDUCTION_1024_MASK            0x00000400
#define EDDP_CRT_REDUCTION_1024_SHIFT           10
#define EDDP_CRT_REDUCTION_2048_MASK            0x00000800
#define EDDP_CRT_REDUCTION_2048_SHIFT           11
#define EDDP_CRT_REDUCTION_4096_MASK            0x00001000
#define EDDP_CRT_REDUCTION_4096_SHIFT           12
#define EDDP_CRT_REDUCTION_8192_MASK            0x00002000
#define EDDP_CRT_REDUCTION_8192_SHIFT           13
#define EDDP_CRT_REDUCTION_16384_MASK           0x00004000
#define EDDP_CRT_REDUCTION_16384_SHIFT          14
// Min, Max of CRT-Reduction
#define EDDP_CRT_REDUCTION_MIN                  (EDDP_CRT_REDUCTION_1)
#define EDDP_CRT_REDUCTION_MAX                  (EDDP_CRT_REDUCTION_16384)
// Min, Max of CRT-ReductionNr
#define EDDP_CRT_REDUCTION_NR_MIN               1
#define EDDP_CRT_REDUCTION_NR_MAX               15
// check macro: is ReductionNr in valid range ?
#define EDDP_CRT_IS_REDUCTIONNR_IN_VALID_RANGE(ReductionNr)     \
            ((ReductionNr >= EDDP_CRT_REDUCTION_NR_MIN) && (ReductionNr <= EDDP_CRT_REDUCTION_NR_MAX))

#define EDDP_CRT_TREDUCTIONSELECT_1_TO_512      0x3FF       // Bit(9:0)
#define EDDP_CRT_TREDUCTIONSELECT_1_TO_16384    0x7FFF      // Bit(14:0)

// init values                                             for PNIP-Register:
#define EDDP_CRT_ACW_T_PO1                      0       // PNIP_REG_ACW_T_PO1K_IFA
#define EDDP_CRT_ACW_T_PO2                      1       // PNIP_REG_ACW_T_PO2K_IFA
#define EDDP_CRT_ACW_T_PO4                      2       // PNIP_REG_ACW_T_PO4K_IFA
#define EDDP_CRT_ACW_T_PO8                      3       // PNIP_REG_ACW_T_PO8K_IFA
#define EDDP_CRT_ACW_T_PO16                     4       // PNIP_REG_ACW_T_PO16K_IFA
// "Phase_offset" for calculating of TBaseIndex
#define EDDP_CRT_PHASE_OFFSET_ACW_T_PO1         16      // Reduction 1024
#define EDDP_CRT_PHASE_OFFSET_ACW_T_PO2         32      // Reduction 2048
#define EDDP_CRT_PHASE_OFFSET_ACW_T_PO4         64      // Reduction 4096
#define EDDP_CRT_PHASE_OFFSET_ACW_T_PO8         128     // Reduction 8192
#define EDDP_CRT_PHASE_OFFSET_ACW_T_PO16        256     // Reduction 16384

// even CycleBaseFactor
#define EDDP_CRT_CYCLE_BASE_FACTOR_1            1       // 1   * 31,25 us   = 31,25 us
#define EDDP_CRT_CYCLE_BASE_FACTOR_2            2       // 2   * 31,25 us   = 62,5 us
#define EDDP_CRT_CYCLE_BASE_FACTOR_4            4       // 4   * 31,25 us   = 125 us
#define EDDP_CRT_CYCLE_BASE_FACTOR_6            6       // 6   * 31,25 us   = 187.5 us
#define EDDP_CRT_CYCLE_BASE_FACTOR_8            8       // 8   * 31,25 us   = 250 us
#define EDDP_CRT_CYCLE_BASE_FACTOR_16           16      // 16  * 31,25 us   = 500 us
#define EDDP_CRT_CYCLE_BASE_FACTOR_32           32      // 32  * 31,25 us   = 1000 us
#define EDDP_CRT_CYCLE_BASE_FACTOR_64           64      // 64  * 31,25 us   = 2000 us
#define EDDP_CRT_CYCLE_BASE_FACTOR_128          128     // 128 * 31,25 us   = 4000 us
// Min, Max of CycleBaseFactor
#define EDDP_CRT_CYCLE_BASE_FACTOR_MIN          (EDDP_CRT_CYCLE_BASE_FACTOR_1)
#define EDDP_CRT_CYCLE_BASE_FACTOR_MAX          (EDDP_CRT_CYCLE_BASE_FACTOR_128)

#define EDDP_CRT_API_TYPE_RT_SF                 14      // API-Type for Store&Forward RT Frames. See FrameClassTable
#define EDDP_CRT_API_TYPE_RT_CT                 15      // API-Type for CutThrough RT Frames.    See FrameClassTable

// DataStatus in Pack-Frame-ACW
#define EDDP_CRT_PACK_FRAME_DSTAT_LOCAL_INIT_VALUE      0x35
#define EDDP_CRT_PACK_FRAME_GROUP_ID                    7
// Bit "DataStatus.Ignore" in Pack-Frame-ACW
#define EDDP_CRT_PACK_FRAME_DSTAT_LOCAL_IGNORE_MASK     (EDD_CSRT_DSTAT_BIT_IGNORE)
#define EDDP_CRT_PACK_FRAME_DSTAT_LOCAL_IGNORE_SHIFT    7
// DataStatus in register
#define EDDP_CRT_REG_APPLDATASTATUS_7_INIT              0x35
#define EDDP_CRT_REG_DG_DATASTATUS_0_6_INIT             0x10
#define EDDP_CRT_REG_DG_DATASTATUS_7_INIT               0x35
#define EDDP_CRT_REG_DG_DATASTATUS_MASK_INIT            0xEF

// init value for PNIP-Register PNIP_REG_AUTOSTOP_BASETIME
#define EDDP_CRT_REG_AUTOSTOP_BASETIME_INIT             0x7A1794    // 0x7A1794 = 64011424ns = 64.01ms

// EtherType
#define EDDP_CRT_ETHER_TYPE         0x8892

// value of TTL (Time To Live)  -> field of IP-Header for RToverUDP frame
#define EDDP_CRT_IP_TTL_VALUE       (EDD_DEFAULT_TTL)

// values for PPM_Starttime
// PPM_Starttime can be only changed with command "Update_BASECYCLE" -> the HW stores the value in register PNIP_R1_REG_PPM_STARTTIME_A
// ERTEC200P
#define EDDP_CRT_PPM_STARTTIME_ERTEC200P_PNIP_REV1      2800    // in ns
#define EDDP_CRT_PPM_STARTTIME_ERTEC200P_PNIP_REV2      3700    // in ns
// FPGA
#define EDDP_CRT_PPM_STARTTIME_FPGA_PNIP_REV1           5200    // in ns
#define EDDP_CRT_PPM_STARTTIME_FPGA_PNIP_REV2           5200    // in ns
#define EDDP_CRT_PPM_STARTTIME_FPGA_PNIP_REV3           5400    // in ns

// values for PPM_FSO_Leadtime
// ERTEC200P
#define EDDP_CRT_PPM_FSO_LEADTIME_ERTEC200P_PNIP_REV1   500     // in ns
#define EDDP_CRT_PPM_FSO_LEADTIME_ERTEC200P_PNIP_REV2   0       // in ns
// FPGA
#define EDDP_CRT_PPM_FSO_LEADTIME_FPGA_PNIP_REV1        4000    // in ns
#define EDDP_CRT_PPM_FSO_LEADTIME_FPGA_PNIP_REV2        4000    // in ns
   
// values for PPM_FetchTimeDelta_FastMEM and PPM_FetchTimeDelta_SlowMem
// HERA - FPGA worstcase values
#define EDDP_CRT_PPM_FETCHTIME_DELTA_FPGA_REV3     8600    // in ns
#define EDDP_CRT_PPM_FETCHTIME_DELTA_ASIC_REV3     8600    // in ns 

// init value for CPM transfer end delay for PN-IP Rev2
#define EDDP_CRT_CPM_TRANSFEREND_DELAY_ASIC_REV2        2300
#define EDDP_CRT_CPM_TRANSFEREND_DELAY_FPGA_REV2        4100

// init value for CPM transfer end delay for PN-IP Rev3
#define EDDP_CRT_CPM_TRANSFEREND_DELAY_ASIC_REV3        2300
#define EDDP_CRT_CPM_TRANSFEREND_DELAY_FPGA_REV3        4100
    
// RxLocalApplicationDelay and TxLocalApplicationFeed for PN-IP Rev3
#define EDDP_CRT_CPM_LOCAL_APPL_DELAY_PNIP_INTERNAL_REV3   862   /* first approach use Tx.LAF.PNIPInternal */
#define EDDP_CRT_PPM_LOCAL_APPL_FEED_PNIP_INTERNAL_REV3    862   /* same value for ASIC and FPGA */
    
#define EDDP_CRT_PPM_OCTET_COPY_TIME_12_BYTE_ASIC_REV3     24   /* reading next 12 byte IO data, asic value */
#define EDDP_CRT_PPM_OCTET_COPY_TIME_12_BYTE_FPGA_REV3     48   /* reading next 12 byte IO data, fpga value */
    
// init value for PPM_Ressource_Lack_IFA
#define EDDP_CRT_REG_PPM_RESSOURCE_LACK_INIT            0xFFFF  // all Resource-Types(15..0)=1
#define EDDP_CRT_REG_PPM_RESSOURCE_LACK_INIT_PNIP_REV3  0xFC3F  // Resource-Types(9..6)=0, other=1


#define EDDP_CRT_PROVIDER_ACW_DG_STATE_MACHINE_DEPTH     (EDDP_CMD_CPM_PPM_COMMAND_DEPTH_REV3)


/* ============================================================================ */
/*                              masks / bits                                    */
/* ============================================================================ */

// shifting of x bits
#define EDDP_CRT_SHIFT_VALUE_1          1
#define EDDP_CRT_SHIFT_VALUE_2          2
#define EDDP_CRT_SHIFT_VALUE_3          3


/* -------------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------------- */
/* mask for DebugInfo of consumer MISS indication                                               */
/* -------------------------------------------------------------------------------------------- */
/* IndParam.DebugInfo[0..11]                                                                    */
/* -------------------------------------------------------------------------------------------- */
//  DebugInfo.Global.Valid (Bit[0])
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_GLOBAL_VALID_MASK                            0x1
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_GLOBAL_VALID_SHIFT                           0x0
//  DebugInfo.Global.SyncOk (Bit[1])
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_GLOBAL_SYNCOK_MASK                           0x2
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_GLOBAL_SYNCOK_SHIFT                          0x1
//  DebugInfo.PortStatus1.RTC3PortStatus (Bit[0..1])
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC3PST_MASK                     0x3
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC3PST_SHIFT                    0x0
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC3PST_NOT_SUPPORTED            0
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC3PST_INACTIVE                 1
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC3PST_TX                       2
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC3PST_RX_TX                    3
//  DebugInfo.PortStatus1.RTC2PortStatus (Bit[2..3])
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC2PST_MASK                     0xc
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC2PST_SHIFT                    0x2
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC2PST_NOT_SUPPORTED            0
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC2PST_INACTIVE                 1
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC2PST_ACTIVE                   2
//  DebugInfo.PortStatus1.RtClass3_RemoteState (Bit[4..5])
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC3REMOTESTATE_MASK             0x30
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC3REMOTESTATE_SHIFT            0x4
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC3REMOTESTATE_OFF              0
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC3REMOTESTATE_TX               1
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC3REMOTESTATE_RX_TX            2
//  DebugInfo.PortStatus1.Preamble (Bit[6])
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_PREAMBLE_MASK                    0x40
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_PREAMBLE_SHIFT                   0x6
//  DebugInfo.PortStatus2.RtClass2_TopoState (Bit[0])
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS2_RTC2TOPOST_MASK                  0x1
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS2_RTC2TOPOST_SHIFT                 0x0
//  DebugInfo.PortStatus2.RtClass3_TopoState (Bit[1])
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS2_RTC3TOPOST_MASK                  0x2
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS2_RTC3TOPOST_SHIFT                 0x1
//  DebugInfo.PortStatus2.RtClass3_PDEVTopoState (Bit[2])
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS2_RTC3PDEVTOPOST_MASK              0x4
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS2_RTC3PDEVTOPOST_SHIFT             0x2
//  DebugInfo.PortStatus2.Neighbour_TopoState (Bit[3])
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS2_NBTOPOST_MASK                    0x8
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS2_NBTOPOST_SHIFT                   0x3
//  DebugInfo.PortStatus2.PortState (Bit[4..7])
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS2_PORTSTATE_MASK                   0xF0
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS2_PORTSTATE_SHIFT                  0x4
/* -------------------------------------------------------------------------------------------- */
/* IndParam.Data[Count].DebugInfo[0..3]                                                         */
/* -------------------------------------------------------------------------------------------- */
//  DebugInfo.DFP_Subframe (Bit[0])
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_DFP_SUBFRAME_MASK                            0x1
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_DFP_SUBFRAME_SHIFT                           0x0
//  DebugInfo.DFP_Subframe_CRC (Bit[1])
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_DFP_SUBFRAME_CRC_MASK                        0x2
#define EDDP_CRT_IND_PROVIDE_DEBUGINFO_DFP_SUBFRAME_CRC_SHIFT                       0x1
/* -------------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------------- */





/* ============================================================================ */
/*                              structs for all                                 */
/* ============================================================================ */

typedef struct _EDDP_CRT_CHAIN_TYPE
{
    LSA_UINT16      Prev;       // prev index in used list
    LSA_UINT16      Next;       // next index in used list
} EDDP_CRT_CHAIN_TYPE;


typedef struct _EDDP_CRT_CHAIN_BASE_TYPE
{
    LSA_UINT16      BeginIdx;       // begin index in list
    LSA_UINT16      EndIdx;         // end index in list
} EDDP_CRT_CHAIN_BASE_TYPE;





/* ============================================================================ */
/*                      structs consumer score board - DDB                      */
/* ============================================================================ */
// Consumer Score Board management
// typedef struct _EDDP_CRT_CONSUMER_SB_MNGM_TYPE      EDDP_LOCAL_MEM_ATTR *       EDDP_CRT_CONSUMER_SB_MNGM_PTR_TYPE;
typedef struct _EDDP_CRT_CONSUMER_SB_MNGM_TYPE
{
    LSA_UINT32              Dummy;
} EDDP_CRT_CONSUMER_SB_MNGM_TYPE;
/* Dummy                    : tbd --> structure reserved for future use !!!                                         */





// contains Param-Data for adding a Consumer
typedef struct _EDDP_CRT_CONSUMER_PARAM_TYPE        EDDP_LOCAL_MEM_ATTR *       EDDP_CRT_CONSUMER_PARAM_PTR_TYPE;
typedef struct _EDDP_CRT_CONSUMER_PARAM_TYPE
{
    LSA_UINT16                          FrameID;
    EDD_MAC_ADR_TYPE                    SrcMAC;
    EDD_IP_ADR_TYPE                     SrcIP;
    LSA_UINT16                          DataLen;
    LSA_UINT16                          Partial_DataOffset;
    LSA_UINT16                          Partial_DataLen;
    LSA_UINT16                          CycleReductionRatio;
    LSA_UINT32                          HwCycleReductionRatio;
    LSA_UINT16                          CyclePhase;
    LSA_UINT16                          WatchdogFactor;
    LSA_UINT16                          DataHoldFactor;
    LSA_UINT8                           HwDataHoldFactor;
    LSA_UINT16                          MediaRedFactor;
    LSA_UINT32                          UserID;
    LSA_UINT32                          DataOffset;
    LSA_UINT8                           ForwardMode;
    LSA_UINT32                          CutThrough_OutDataStart;
    LSA_UINT16                          CRNumber;
    LSA_UINT8                           ClearOnMISS;
    LSA_UINT8                           AppGroup;
    LSA_UINT8                           RTClass;
    LSA_UINT8                           SFPosition;
    LSA_UINT32                          SFOffset;
    LSA_UINT16                          Properties;
} EDDP_CRT_CONSUMER_PARAM_TYPE;
/* FrameId                  : valid FrameID. Range depends on SRT-Init-parameters                                   */
/* SrcMAC                   : Source  MAC address to check for if not 00.00.00.00.00.00.00. (only XRT)              */
/* SrcIP                    : Source  IP address of received frame to be checked. (only xRTOverUDP)                 */
/* DataLen                  :   DataLen of I/O-area of consumer. Not Frame length!                                  */
/* Partial_DataOffset       : PartialData access. dataoffset.                                                       */
/* Partial_DataLen          : PartialData access. Datalen.                                                          */
/* CycleReductionRatio      : Reduction                                                                             */
#define EDDP_CRT_CYCLE_REDUCTION_RATIO_LEGACY    128    // used in 2CR mode for the green (legacy) frame
/* HwCycleReductionRatio    : normally CycleReductionRatio-1                                                        */
/* CyclePhase               : Phase                                                                                 */
/* WatchdogFactor           : for ACW.WD_Timer (factor to check for received consumer)                              */
#define EDDP_CRT_CONSUMER_WDT_OFF           0xFF    // in HW off: no WDT
#define EDDP_CRT_CONSUMER_WDT_MAX_SW        253     // 253+1 is the maximum
/* DataHoldFactor           : for ACW.DH_Timer (factor to check for DataHold)                                       */
#define EDDP_CRT_CONSUMER_DHT_OFF           0xFF    // in HW off: no DHT
#define EDDP_CRT_CONSUMER_DHT_MAX_SW        253     // 253+1 is the maximum
/* HwDataHoldFactor         : normally DataHoldFactor+1 used for WDT and DHT                                        */
/* MediaRedFactor           : for ACW.MR_ReloadValue (factor to check for received MRPD frames)                     */
#define EDDP_CRT_CONSUMER_MRT_OFF           0xFF    // in HW off: no MRPD frame with FrameID(x+1)
#define EDDP_CRT_CONSUMER_MRT_MAX_SW        253     // 253+1 is the maximum
#define EDDP_CRT_CONSUMER_MRT_PNIP_REV2     64      // constant value since PN-IP Rev2
/* UserID                   : UserID used for status-indications                                                    */
/* DataOffset               : Dataoffset of I/O-Buffer to start of I/O-Data RAM                                     */
/* ForwardMode              : store-and-forward mode / CutThrough-Mode                                              */
/* CutThrough_OutDataStart  : only evaluated if ForwardMode set to EDD_FORWARDMODE_CT                               */
#define EDDP_CRT_MAX_CUTTHROUGH_OUTDATASTART        0x7FF
/* CRNumber                 : IO-CR this provider belongs to. (EDD_CRNUMBER_NOT_USED)                               */
/* ClearOnMISS              : If CRNumber set then clear data by HW on real MISS when ClearOnMISS is set to TRUE    */
/* AppGroup                 : Application-group this consumer belongs to. 0=no group, 1/2=group1 or 2               */
/* RTClass                  : EDDP_CRT_RTCLASS_RT or EDDP_CRT_RTCLASS_UDP. No User-parameter. set by EDDP!          */
/* SFPosition               : Subframeposition: 1..127 if DFP used                                                  */
/* SFOffset                 : Offset of the datagramm data (DG-C_SDU) in the packframe if DFP used                  */
/* Properties               : Properties from CONSUMER_ADD                                                          */

/* Scoreboard Management for Consumer */
typedef struct _EDDP_CRT_CONSUMER_SB_TYPE      EDDP_LOCAL_MEM_ATTR *       EDDP_CRT_CONSUMER_SB_PTR_TYPE;
typedef struct _EDDP_CRT_CONSUMER_SB_TYPE
{
    LSA_UINT32                          ActEvents;
    LSA_UINT32                          IndEvents;
    LSA_BOOL                            WaitSetToUnknown;
    LSA_UINT32                          EventStatusDefault;
    LSA_UINT32                          DebugInfo;
} EDDP_CRT_CONSUMER_SB_TYPE;
/* ActEvents                : Consumer Events of actual ConsumerScoreboard.                                         */
/* IndEvents                : Consumer Events on last Indication to user. Typically the same as                     */
/*                            ActEvents, but if no indication resources present it will be an old one.              */  
/*                            For ActEvents and IndEvents defines see EDD_USR.H                                     */
/*                             EDD_CSRT_CONS_EVENT_MISS (EDD_CSRT_CONS_EVENT_MISS_NOT_STOPPED)                      */
/*                             EDD_CSRT_CONS_EVENT_AGAIN                                                            */
/*                             EDD_CSRT_CONS_EVENT_RUN                                                              */
/*                             EDD_CSRT_CONS_EVENT_STOP                                                             */
/*                             EDD_CSRT_CONS_EVENT_STATION_OK                                                       */
/*                             EDD_CSRT_CONS_EVENT_STATION_FAILURE                                                  */
/*                             EDD_CSRT_CONS_EVENT_PRIMARY                                                          */
/*                             EDD_CSRT_CONS_EVENT_BACKUP                                                           */
/*                             EDD_CSRT_CONS_EVENT_ENTERING_RED                                                     */
/*                             etc.                                                                                 */
/* WaitSetToUnknown         : TRUE:  Wait for a new DHT / MRT indication from CSB becaus of SetToUnknown            */
/*                            FALSE: dont wait.                                                                     */
/* EventStatusDefault       : Default of EventStatus on Activate for normal or SRD-Consumer                         */
#define EDDP_CRT_CONS_STATE_DEFAULT_NORMAL          \
    (EDD_CSRT_CONS_EVENT_MISS_NOT_STOPPED           | \
     EDD_CSRT_CONS_EVENT_RUN                        | \
     EDD_CSRT_CONS_EVENT_STATION_OK                 | \
     EDD_CSRT_CONS_EVENT_PRIMARY                    | \
     EDD_CSRT_CONS_EVENT_PRIMARY_AR_EXISTS)
#define EDDP_CRT_CONS_STATE_DEFAULT_SRD             \
    (EDD_CSRT_CONS_EVENT_MISS_NOT_STOPPED           | \
     EDD_CSRT_CONS_EVENT_RUN                        | \
     EDD_CSRT_CONS_EVENT_STATION_OK                 | \
     EDD_CSRT_CONS_EVENT_BACKUP                     | \
     EDD_CSRT_CONS_EVENT_PRIMARY_AR_EXISTS)
/* DebugInfo                : DebugInfo for indication RQB (IndParam.Data[Count].DebugInfo[0..3])                   */
/*                              DebugInfo [3 2 1 0]                                                                 */
/*                              DebugInfo[0]    : DFP_Subframe, DFP_Subframe_CRC                                    */
/*                              DebugInfo[1]    : reserved                                                          */
/*                              DebugInfo[2]    : CycleCounter (high)                                               */
/*                              DebugInfo[3]    : CycleCounter (low)                                                */


/* MRT Diagnosis Management for MRPD-Consumer - init on activate */
typedef struct _EDDP_CRT_CONSUMER_MRT_DIAG_TYPE
{
    LSA_BOOL        IsPrmMRPDWatchDogActive;
    LSA_BOOL        LastMrtExpired;
    LSA_UINT16      PrmPDIRFrameDataIdx;
} EDDP_CRT_CONSUMER_MRT_DIAG_TYPE;
/* IsPrmMRPDWatchDogActive  : flag MRT diagnosis is ON/OFF                                                          */
/* LastMrtExpired           : last state of MRT_EXPIRED (LSA_FALSE: not expired / LSA_TRUE: expired)                */
/* PrmPDIRFrameDataIdx      : chained index in PDIRData.PDIRFrameData[x] if MRT diagnosis is ON                     */


// Management structure for 1 single consumer ACW   -> pConsEntry[ConsumerId].ConsAcwTypeInfo
typedef struct _EDDP_CRT_CONS_ACW_INFO_TYPE     EDDP_LOCAL_MEM_ATTR *   EDDP_CRT_CONS_ACW_INFO_PTR_TYPE;
typedef struct _EDDP_CRT_CONS_ACW_INFO_TYPE
{
    LSA_UINT16                  AcwIdx;
    EDDP_CRT_CHAIN_TYPE         RBaseLink;
    LSA_BOOL                    IsAcwContainer;
    LSA_UINT16                  FrameID;
    LSA_VOID_PTR_TYPE           pHwAcwApi;
    PNIP_ADPU_RCV_PTR_TYPE      pHwApduApi;
}EDDP_CRT_CONS_ACW_INFO_TYPE;
/* AcwIdx                   : logical index for PTR array RBaseLink                                                 */
/* RBaseLink                : chain element in RBaseEntry list                                                      */
/* IsAcwContainer           : TRUE for AcwIdx  MAX_CONSUMER..(MAX_CONSUMER + 8)                                     */
/* FrameID                  : used for inserting into RBase                                                         */
/* pHwAcwApi                : address to Receive- or Container-ACW in APICtrl-RAM                                   */
/* pHwApduApi               : address to APDU status in APICtrl-RAM (in DFP: used for DGs without Checksum)         */
typedef EDDP_CRT_CONS_ACW_INFO_PTR_TYPE     EDDP_LOCAL_MEM_ATTR *       EDDP_CRT_CONS_ACW_INFO_PTR_PTR_TYPE;


// Management structure for 1 single consumer   -> pConsEntry[ConsumerId]
typedef struct _EDDP_CRT_CONSUMER_TYPE      EDDP_LOCAL_MEM_ATTR *       EDDP_CRT_CONSUMER_PTR_TYPE;
typedef struct _EDDP_CRT_CONSUMER_TYPE
{
    EDDP_CRT_CONS_ACW_INFO_TYPE         ConsAcwTypeInfo;    // NOTE: must be first element here !!!!
    // -----------------------------------------------------------
    LSA_UINT16                          ConsumerId;
    LSA_UINT8                           ConsumerFlags;
    LSA_UINT8                           State;
    LSA_UINT32                          UpperUserId;
    EDDP_CRT_CHAIN_TYPE                 UsedFreeLink;
    EDDP_CRT_CONSUMER_PARAM_TYPE        Param;
    LSA_UINT16                          DgIdx;
    LSA_UINT16                          DgGrpIdx;
    EDDP_CRT_CONSUMER_SB_TYPE           ScoreBoard;
    EDDP_CRT_CONSUMER_MRT_DIAG_TYPE     MrtDiag;
    PNIP_CPM_TSB_PTR_TYPE               pHwTSBApi;
    PNIP_CPM_CSB_PTR_TYPE               pHwCSB0Api;
    PNIP_CPM_CSB_PTR_TYPE               pHwCSB1Api;
#if 1 // for Renesas RZ/T2M
    LSA_UINT32                          DHTExpireCount;
    LSA_BOOL                            DHTExpireCheckEnable;
    LSA_BOOL                            FirstPacket;
#endif
} EDDP_CRT_CONSUMER_TYPE;
/* ConsAcwTypeInfo          : capsulates the RBaseLink and type of ACW (normal / container)                         */
/* ConsumerId               : unique ident in a Consumer-List, needed to Control and to Remove a Consumer           */
/* ConsumerFlags            : different flags see EDDP_CONS_FLAG_xxx                                                */
#define EDDP_CONS_FLAG_IS_2CR_MODE                  1   // 0 = for 1CR mode, 1 = for 2CR mode
#define EDDP_CONS_FLAG_IS_DP2ACW                    2   // 0 = no DP2 need, 1 = DP2 relevant
#define EDDP_CONS_FLAG_PROV_AUTOSTOP_ENABLED        4   // hold user setting for EDD_CONS_CONTROL_MODE_PROV_AUTOSTOP_ENABLE / DISABLE
#define EDDP_CONS_FLAG_PENDING_PROV_AUTOSTOP_ON     8   // hold flag to fire PROV_AUTOSTOP_ENABLE-indication
#define EDDP_CONS_FLAG_IS_DFP_ITEM                  16  // 0: normal ACW consumer, 1: DFP item uses entry from EDDP_CRT_CONSUMER_DG_TYPE
#define EDDP_CONS_FLAG_IS_SRD_ITEM                  32  // 0: normal consumer, 1: SRD consumer
#define EDDP_CONS_FLAG_MISS_RCVD                    64  // used for handling of MISS and MISS_NOT_STOPPED
#define EDDP_CONS_FLAG_AGAIN_RCVD                   128 // used for handling: frame was received once
/* State                    : state free, reserved in SW and in HW active/passive                                   */
/* UpperUserId              : special ID for user (indications)                                                     */
/* UsedFreeLink             : chain element in used/free list                                                       */
/* Param                    : contains consumer parameters                                                          */
/* DgIdx                    : Index of DG item if FLAG DFP set                                                      */
/* DgGrpIdx                 : Index of AcwContainerEntry if FLAG DFP set                                            */
/* ScoreBoard               : Scoreboard management                                                                 */
/* MrtDiag                  : MRT diagnosis management for MRPD-Consumer                                            */
/* pHwTSBApi                : address to TimerScoreBoard-entry in APICtrl-RAM                                       */
/* pHwCSB0Api               : address to ConsumeScoreBoard-entry within CSB #0 in APICtrl-RAM                       */
/* pHwCSB1Api               : address to ConsumeScoreBoard-entry within CSB #1 in APICtrl-RAM                       */


// Management structure for 1 single pack frame     -> pConsContainerEntry[DgGrpIdx]
typedef struct _EDDP_CRT_CONSUMER_CONTAINER_TYPE      EDDP_LOCAL_MEM_ATTR *       EDDP_CRT_CONSUMER_CONTAINER_PTR_TYPE;
typedef struct _EDDP_CRT_CONSUMER_CONTAINER_TYPE
{
    EDDP_CRT_CONS_ACW_INFO_TYPE         ConsAcwTypeInfo;    // NOTE: must be first element here !!!!
    // -----------------------------------------------------------
    LSA_UINT16                          DgGrpIdx;
    EDDP_CRT_CHAIN_BASE_TYPE            ConsDgList;
    LSA_UINT16                          DgCount;
    EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE     pPDIRFrameData;
    LSA_BOOL                            HasChksum;
    LSA_BOOL                            IsRedundant;
    LSA_UINT8                           Acw_DG_Order;
} EDDP_CRT_CONSUMER_CONTAINER_TYPE;
/* ConsAcwTypeInfo          : capsulates the RBaseLink and type of ACW (normal / container)                         */
/* DgGrpIdx                 : Index of AcwContainerEntry                                                            */
/* ConsDgList               : List of DGs connected with this container                                             */
/* DgCount                  : number of DGs in list ConsDgLink                                                      */
/* pPDIRFrameData           : pointer to PRM data                                                                   */
/* HasChksum                : LSA_FALSE=no, LSA_TRUE=yes                                                            */
/* IsRedundant              : LSA_FALSE=no, LSA_TRUE=yes                                                            */
/* Acw_DG_Order             : 0=first DG(UP), 1=last DG(DOWN)                                                       */


// Management structure for 1 single ACW_DG     -> pConsDgEntry[DgIdx]
typedef struct _EDDP_CRT_CONSUMER_DG_TYPE      EDDP_LOCAL_MEM_ATTR *       EDDP_CRT_CONSUMER_DG_PTR_TYPE;
typedef struct _EDDP_CRT_CONSUMER_DG_TYPE
{
    LSA_UINT16                          DgIdx;
    LSA_UINT16                          DgGrpIdx;
    EDDP_CRT_CHAIN_TYPE                 UsedFreeDgLink;
    EDDP_CRT_CHAIN_TYPE                 DgLink;
    LSA_UINT16                          ConnectedConsumerId;
    EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SUBFRAMEDATA_PTR_TYPE  pSubFrameData;
    LSA_VOID_PTR_TYPE                   pHwAcwDgApi;
    PNIP_DG_STATUS_RCV_PTR_TYPE         pHwDgStatusApi;
} EDDP_CRT_CONSUMER_DG_TYPE;
/* DgIdx                    : Index of DG item                                                                      */
/* DgGrpIdx                 : Index to AcwContainerEntry (ACW container)                                            */
/* UsedFreeDgLink           : chain element in used/free list                                                       */
/* DgLink                   : chain element used from Container item to store list of DG items                      */
/* ConnectedConsumerId      : unique ident in a Consumer-List and the entry will be used like a proxy item          */
/* pSubFrameData            : pointer to PRM data                                                                   */
/* pHwAcwDgApi              : address to DG-ACW in APICtrl-RAM                                                      */
/* pHwDgStatusApi           : address to DG-status in APICtrl-RAM                                                   */


// Management structure for 1 RBase-Entry   -> pRBaseEntry[RBaseIndex]
typedef struct _EDDP_CRT_CONSUMER_RBASE_TYPE    EDDP_LOCAL_MEM_ATTR *       EDDP_CRT_CONSUMER_RBASE_PTR_TYPE;
typedef struct _EDDP_CRT_CONSUMER_RBASE_TYPE
{
    EDDP_CRT_CHAIN_BASE_TYPE            ConsRBaseList;   // ConsRBaseList.BeginIdx --> Cons1 <-> Cons2 <-> ... <-> ConsN
                                                         // ConsRBaseList.EndIdx   ----------------------------------^
    LSA_UINT16                          AcwCnt;
    LSA_UINT16                          ContainerCnt;
    PNIP_ACW_RCV_RBASE_PTR_TYPE         pHwRBaseApi;
} EDDP_CRT_CONSUMER_RBASE_TYPE;
/* ConsRBaseList    : beginn/end of consumer RBase list                                 */
/* AcwCnt           : Number of consumers/containers in ConsRBaseList of this RBaseEntry*/
/* ContainerCnt     : Number of containers in ConsRBaseList of this RBaseEntry          */
/* pHwRBaseApi      : RBase-Entry: one ACW_List_Ptr in APICtrl-RAM                      */


/* ============================================================================ */
/*          signature of CRT cbf                                                */
/* ============================================================================ */
typedef LSA_VOID LSA_FCT_PTR(EDD_UPPER_OUT_FCT_ATTR, EDDP_CRT_CBF_PTR_TYPE)(EDDP_LOCAL_DDB_PTR_TYPE, EDD_RSP); 


/* ============================================================================ */
/*          signature of CRT provider ACW_DG cbf                                */
/* ============================================================================ */
typedef LSA_VOID LSA_FCT_PTR(EDD_UPPER_OUT_FCT_ATTR, EDDP_CRT_PROV_DG_CBF_PTR_TYPE)(EDDP_LOCAL_DDB_PTR_TYPE, LSA_UINT16, EDD_RSP); 


/* ============================================================================ */
/*          signature of CRT SM-function                                        */
/* ============================================================================ */
typedef EDD_RSP LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, EDDP_CRT_SMF_PTR_TYPE)(EDDP_LOCAL_DDB_PTR_TYPE, LSA_BOOL, EDDP_CRT_CBF_PTR_TYPE); 


/* ============================================================================ */
/*          signature of CRT RBase-functions                                    */
/* ============================================================================ */
typedef LSA_VOID   LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, EDDP_CRT_ADD_ACW_TO_RBASE_PTR_TYPE)(EDDP_LOCAL_DDB_PTR_TYPE, LSA_UINT16);
typedef LSA_VOID   LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, EDDP_CRT_REMOVE_ACW_FROM_RBASE_PTR_TYPE)(EDDP_LOCAL_DDB_PTR_TYPE, LSA_UINT16);
typedef LSA_UINT16 LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, EDDP_CRT_GET_RBASE_INDEX_PTR_TYPE)(EDDP_LOCAL_DDB_PTR_TYPE, LSA_UINT16);


/* ============================================================================ */
/*          signature of CRT CPM ACW-functions                                  */
/* ============================================================================ */
typedef LSA_VOID   LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, EDDP_CRT_CONS_ACW_UPDATE_SRC_ADDR_CSA_PTR_TYPE)(EDDP_LOCAL_DDB_PTR_TYPE, LSA_UINT16);
typedef LSA_VOID   LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, EDDP_CRT_CONS_ACW_INIT_PTR_TYPE)(EDDP_LOCAL_DDB_PTR_TYPE, LSA_UINT16);
typedef LSA_VOID   LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, EDDP_CRT_CONS_ACW_DG_BASIC_INIT_PTR_TYPE)(EDDP_LOCAL_DDB_PTR_TYPE, LSA_UINT16, LSA_UINT32, LSA_UINT32);
typedef LSA_VOID   LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, EDDP_CRT_CONS_ACW_DG_FINAL_INIT_PTR_TYPE)(EDDP_LOCAL_DDB_PTR_TYPE, EDDP_CRT_CONSUMER_PTR_TYPE, EDDP_CRT_CONSUMER_DG_PTR_TYPE);
typedef LSA_VOID   LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, EDDP_CRT_CONS_ACW_CONTAINER_INIT_PTR_TYPE)(EDDP_LOCAL_DDB_PTR_TYPE, LSA_UINT16);
typedef LSA_VOID   LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, EDDP_CRT_CONS_ACW_UPDATE_NEXT_OFFSET_PTR_TYPE)(EDDP_LOCAL_DDB_PTR_TYPE, LSA_UINT16);
typedef LSA_VOID   LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, EDDP_CRT_CONS_ACW_SETUP_LEGACY_PTR_TYPE)(EDDP_LOCAL_DDB_PTR_TYPE, EDDP_CRT_CONSUMER_PTR_TYPE);


/* ============================================================================ */
/*          struct consumer RBase handler functions                             */
/* ============================================================================ */
typedef struct _EDDP_CRT_CONSUMER_RBASE_HANDLER_TYPE
{
    EDDP_CRT_ADD_ACW_TO_RBASE_PTR_TYPE       FctAddAcwToRBase;
    EDDP_CRT_REMOVE_ACW_FROM_RBASE_PTR_TYPE  FctRemoveAcwFromRBase;
    EDDP_CRT_GET_RBASE_INDEX_PTR_TYPE        FctGetRBaseIndex;
} EDDP_CRT_CONSUMER_RBASE_HANDLER_TYPE;
/* FctAddAcwToRBase           : add ACW to RBase management                               */
/* FctRemoveAcwFromRBase      : remove ACW from RBase management                          */
/* FctGetRBaseIndex           : get index within RBase for given FrameID                  */


/* ============================================================================ */
/*          struct consumer ACW handler functions                               */
/* ============================================================================ */
typedef struct _EDDP_CRT_CONSUMER_ACW_HANDLER_TYPE
{
    EDDP_CRT_CONS_ACW_UPDATE_SRC_ADDR_CSA_PTR_TYPE FctUpdateSrcAddrCsaAcwHwApi;
    EDDP_CRT_CONS_ACW_INIT_PTR_TYPE                FctInitAcwHwApi;
    EDDP_CRT_CONS_ACW_DG_BASIC_INIT_PTR_TYPE       FctBasicInitAcwDgHwApi;
    EDDP_CRT_CONS_ACW_DG_FINAL_INIT_PTR_TYPE       FctFinalInitAcwDgHwApi;
    EDDP_CRT_CONS_ACW_CONTAINER_INIT_PTR_TYPE      FctInitContainerAcwHwApi;
    EDDP_CRT_CONS_ACW_UPDATE_NEXT_OFFSET_PTR_TYPE  FctUpdateAcwNextOffsetAcwHwApi;
    EDDP_CRT_CONS_ACW_SETUP_LEGACY_PTR_TYPE        FctSetupLegacyStartupAcwHwApi;
} EDDP_CRT_CONSUMER_ACW_HANDLER_TYPE;
/* FctUpdateSrcAddrCsaInConsAcwHwApi  : setup src address within ACW                               */
/* FctInitAcwHwApi                    : initialize ACW                                             */
/* FctBasicInitAcwDgHwApi             : initialize ACW-DG with basic settings                      */
/* FctFinalInitAcwDgHwApi             : finish ACW-DG settings                                     */
/* FctInitContainerAcwHwApi           : initialize container ACW                                   */
/* FctUpdateAcwNextOffsetAcwHwApi     : update next offset within ACW to next ACW                  */
/* FctSetupLegacyStartupAcwHwApi      : setup ACW with legacy startup                              */


/* ============================================================================ */
/*          structs consumer statemchine RcvDg,In + Outbound                    */
/* ============================================================================ */
typedef enum
{
    EDDP_CRT_RCVDG_SM_IDLE = 0,     // RcvDg ON + OFF
    EDDP_CRT_RCVDG_SM_ACWOFF,       // RcvDg OFF
    EDDP_CRT_RCVDG_SM_ACWOUT,       // RcvDg OFF
    EDDP_CRT_RCVDG_SM_ACWIN,        // RcvDg ON
    EDDP_CRT_RCVDG_SM_ACWON         // RcvDg ON
} EDDP_CRT_RCVDG_SM_STATE;

typedef struct _EDDP_CRT_RCVDG_SM_TYPE
{
    EDDP_CRT_RCVDG_SM_STATE     State;
    LSA_UINT16                  CurrContainerIdx;   
} EDDP_CRT_RCVDG_SM_TYPE;
// State:            current state
// CurrContainerIdx: curent container grp for RcvDg ON

typedef enum
{
    EDDP_CRT_INBOUND_SM_IDLE = 0,     // Inbound En
    EDDP_CRT_INBOUND_SM_EN_RED_GRP    // enable redundant Inbound grp
} EDDP_CRT_INBOUND_SM_STATE;

typedef struct _EDDP_CRT_INBOUND_SM_TYPE
{
    EDDP_CRT_INBOUND_SM_STATE   State;
    LSA_UINT8                   InBoundIdx;
    LSA_UINT32                  RegInboundPackCtl_0_X;
    LSA_UINT32                  RegInboundPackCtl_1_X;
    LSA_UINT32                  RegInboundPack_X;
} EDDP_CRT_INBOUND_SM_TYPE;
// State:                   current state
// InBoundIdx:              index of current or next free pack grp
// RegOutboundPack_X:       outbound pack reg. for controller/multi dev
// RegInboundPackCtl_0_X:   inbound reg. of current or next free pack grp
// RegInboundPackCtl_1_X:   inbound reg. of current or next free pack grp
// RegInboundPack_X:        inbound reg. of current or next free pack grp

typedef enum
{
    EDDP_CRT_OUTBOUND_SM_IDLE = 0,     // Outbound En
    EDDP_CRT_OUTBOUND_SM_EN_RED_GRP    // enable redundant Outbound grp
} EDDP_CRT_OUTBOUND_SM_STATE;

typedef struct _EDDP_CRT_OUTBOUND_SM_TYPE
{
    EDDP_CRT_OUTBOUND_SM_STATE  State;
    LSA_UINT8                   OutBoundIdx;
    LSA_UINT32                  RegOutboundPack_X;
} EDDP_CRT_OUTBOUND_SM_TYPE;
// State:                   current state
// OutBoundIdx:             index of current or next free pack grp
// RegOutboundPack_X:       outbound reg. of current or next free pack grp


typedef struct _EDDP_CRT_PACK_SM_TYPE    EDDP_LOCAL_MEM_ATTR *       EDDP_CRT_PACK_SM_PTR_TYPE;
typedef struct _EDDP_CRT_PACK_SM_TYPE
{
    LSA_UINT16                          CurrIdx;            
    LSA_UINT16                          CurrEndCnt;         
    LSA_BOOL                            ExecuteOff;         
    EDDP_CRT_CBF_PTR_TYPE               Cbf;                
    EDDP_CRT_SMF_PTR_TYPE               Smf;                
    EDDP_PRM_PDIRDATA_PTR_TYPE          pPDIRData;          
    EDDP_PRM_PDIRSUBFRAMEDATA_PTR_TYPE  pPDIRSubFrameData;  
    EDDP_CMD_PARAM_TYPE                 CmdParam;
    union
    {
        EDDP_CRT_RCVDG_SM_TYPE          RcvDg;
        EDDP_CRT_INBOUND_SM_TYPE        Inbound;
        EDDP_CRT_OUTBOUND_SM_TYPE       Outbound;
    }SM;
} EDDP_CRT_PACK_SM_TYPE;

// CurrIdx:             current index
// CurrEndCnt:          end detection for CurrIdx
// ExecuteOff:          stored input parameter
// Cbf:                 stored input parameter
// Smf:                 address of own SM function (OutBound, Inbound or RcvDG)
// pPDIRData:           for faster access
// pPDIRSubFrameData:   for faster access
// CmdParam:            parameter struct for asynchronous CMD
// SM:                  special parameter for the SM (only one active from call until callback)



// contains Param-Data for adding a Provider
typedef struct _EDDP_CRT_PROVIDER_ADD_PARAM_TYPE
{
    LSA_UINT16                          FrameID;
    LSA_UINT16                          Properties;
    EDD_IP_ADR_TYPE                     DstIP;
    LSA_UINT16                          DataLen;
    LSA_UINT32                          BufferProperties;
    LSA_UINT16                          CRNumber;
    LSA_UINT8                           AppGroup;
    LSA_UINT32                          DataOffset;
    EDDP_PRM_RTC3_ALL_PORTS_INFO_TYPE   PortInfo;
} EDDP_CRT_PROVIDER_ADD_PARAM_TYPE;
/* FrameID                  : FrameID from PROVIDER_ADD                                                             */
/* Properties               : Properties from PROVIDER_ADD                                                          */
/* DataLen                  : DataLen of IO-Data                                                                    */
/* DataOffset               : Offset to pIOBaseAdr im IO-Buffer for ACW.Data_Ptr                                    */
/* PortInfo                 : PortInfo for RTClass3 provider                                                        */


// contains Param-Data for DFP-Provider
typedef struct _EDDP_CRT_PROVIDER_DFP_PARAM_TYPE
{
    LSA_UINT16              PackGrId;
    LSA_UINT8               SFPosition;
    LSA_UINT32              SFOffset;
    LSA_UINT16              AcwDGId;
} EDDP_CRT_PROVIDER_DFP_PARAM_TYPE;
/* PackGrId                 : PackGrId of connected pack group in PackGroupUsedList                                 */
/* SFPosition               : SFPosition from Provider-ADD/CONTROL                                                  */
/* SFOffset                 : SFOffset from Provider-ADD/CONTROL                                                    */
/* AcwDGId                  : ACW_DG with SFPosition                                                                */


// contains additional Param-Data for DFP-Provider which are checked when activating a Provider (only for later checks)
typedef struct _EDDP_CRT_PROVIDER_DFP_PARAM_CHECK_TYPE
{
    LSA_UINT16              CycleReductionRatio;
    LSA_UINT16              CyclePhase;
    LSA_UINT32              DataOffset; // only relevant for HERA
} EDDP_CRT_PROVIDER_DFP_PARAM_CHECK_TYPE;
/* CycleReductionRatio      : CycleReductionRatio from Provider-ADD/CONTROL                                         */
/* CyclePhase               : CyclePhase from Provider-ADD/CONTROL                                                  */
/* DataOffset               : DataOffset from Provider-ADD/CONTROL                                                  */


// Management structure for 1 single provider   -> pProvEntry[ProviderId]
typedef struct _EDDP_CRT_PROVIDER_TYPE      EDDP_LOCAL_MEM_ATTR *       EDDP_CRT_PROVIDER_PTR_TYPE;
typedef struct _EDDP_CRT_PROVIDER_TYPE
{
    EDDP_CRT_CHAIN_TYPE                       UsedFreeLink;
    EDDP_CRT_CHAIN_TYPE                       GroupLink;
    EDDP_CRT_CHAIN_TYPE                       ASGroupLink;
    EDDP_CRT_CHAIN_BASE_TYPE                  AcwList;
    LSA_UINT16                                ProviderId;
    LSA_UINT8                                 State;
    LSA_UINT16                                ProviderFlags;
    LSA_UINT8                                 ROGPhase;
    LSA_UINT8                                 RTClass;
    LSA_UINT16                                DataStatusId;
    LSA_UINT32                                AcwCnt;
    LSA_UINT32                                AcwActCnt;
    LSA_UINT16                                AutoStopConsumerID;
    LSA_UINT16                                AuxAcwId;
    EDDP_CRT_PROVIDER_ADD_PARAM_TYPE          AddParam;
    EDDP_CRT_PROVIDER_DFP_PARAM_TYPE          DfpParam;
    EDDP_CRT_PROVIDER_DFP_PARAM_CHECK_TYPE    DfpParamCheck;
} EDDP_CRT_PROVIDER_TYPE;
/* UsedFreeLink             : chain element in used/free list                                                       */
/* GroupLink                : chain element in group list                                                           */
/* ASGroupLink              : chain element in AutoStop group list                                                  */
/* AcwList                  : beginn/end of ACW list, which belong to this provider                                 */
/* ProviderId               : Index in provider array                                                               */
/* State                    : state free, reserved in SW and in HW active/passive                                   */
/* ProviderFlags            : ProviderFlags:                                                                        */
#define EDDP_CRT_PROV_FLAGS_IS_REDUNDANT        0x0001      // Redundant-Provider
#define EDDP_CRT_PROV_FLAGS_IS_DFP              0x0002      // DFP-Provider
#define EDDP_CRT_PROV_FLAGS_IS_MCCR             0x0004      // MultiCast-CR-Provider
#define EDDP_CRT_PROV_FLAGS_IS_SYSRED           0x0008      // System-Redundancy-Provider
/* ROGPhase                 : Red, Orange or Green Provider                                                         */
/* RTClass                  : RT or RToverUDP                                                                       */
/* DataStatusId             : Index in DataStatus array                                                             */
/* AddParam                 : contains provider-add parameters                                                      */
/* AcwCnt                   : Number of ACWs which belong to this provider                                          */
/* AcwActCnt                : Number of active ACWs which belong to this provider                                   */
/* AutoStopConsumerID       : ConsumerID of the consumer which will be used for Provider-AutoStop                   */
/* AuxAcwId                 : AuxAcwId of this provider (only for RTC3_MODE_LEGACY)                                 */
/* DfpParam                 : parameters for DFP-Provider                                                           */
/* DfpParamCheck            : additional parameters for DFP-Provider (only for later checks)                        */


// contains Param-Data for adding a ACW to PhaseList
typedef struct _EDDP_CRT_PROVIDER_ACW_ADD_PARAM_TYPE
{
    LSA_UINT16              FrameID;
    EDD_MAC_ADR_TYPE        DstMAC;
    LSA_UINT16              CycleReductionRatio;
    LSA_UINT16              CyclePhase;
    LSA_UINT32              CyclePhaseSequence;
    LSA_UINT16              TxPortID;
    LSA_UINT32              FrameSendOffset;
} EDDP_CRT_PROVIDER_ACW_ADD_PARAM_TYPE;
/* FrameID                  : FrameID from PROVIDER_ADD or from PDIRData                                            */
/* DstMAC                   : if FastForwarding + MRPD, so every ACW has different FFW-DstMAC because of FrameID    */
/* CycleReductionRatio      : Reduction                                                                             */
/* CyclePhase               : Phase                                                                                 */
/* CyclePhaseSequence       : send sequence or position in sending order for phase Green/Orange                     */
/* TxPortID                 : PortID to send this ACW (only for RTClass3 provider)                                  */
/* FrameSendOffset          : "FrameSendOffset" in ns - time offset within a red phase to send this ACW             */
/*                            position in sending order of red ACWs (only for RTClass3 provider)                    */
#define EDDP_CRT_FRAME_SEND_OFFSET_UNDEFINED        (EDDP_CRT_INVALID_VALUE_32)


// contains Param-Data for DFP-ACW
typedef struct _EDDP_CRT_PROVIDER_ACW_DFP_PARAM_TYPE
{
    LSA_UINT16              PrmSubframeBlockIdx;
    LSA_UINT16              PackGrId;
    LSA_UINT32              AcwDGCnt;
    LSA_UINT32              AcwDGActCnt;
    LSA_UINT8               ACW_DG_order;
    LSA_UINT16              All_DG_Len;
} EDDP_CRT_PROVIDER_ACW_DFP_PARAM_TYPE;
/* PrmSubframeBlockIdx      : connected index in array PDIRSubframeData.SubframeBlock[x] (only for ACW_DG)          */
/* PackGrId                 : PackGrId of connected pack group in PackGroupUsedList                                 */
/* AcwDGCnt                 : Number of ACW_DGs which belong to this ACW                                            */
/* AcwDGActCnt              : Number of active ACW_DGs which belong to this ACW                                     */
/* ACW_DG_order             : 0x0 = ascending order for sending of ACW_DGs                                          */
/*                            0x1 = descending order for sending of ACW_DGs                                         */
/* All_DG_Len               : Sum of all ACW_DG lengths of this pack group                                          */


// Management structure for 1 single ACW        -> pAcwEntry[AcwId]
typedef struct _EDDP_CRT_PROVIDER_ACW_TYPE      EDDP_LOCAL_MEM_ATTR *       EDDP_CRT_PROVIDER_ACW_PTR_TYPE;
typedef struct _EDDP_CRT_PROVIDER_ACW_TYPE
{
    EDDP_CRT_CHAIN_TYPE                     UsedFreeLink;
    EDDP_CRT_CHAIN_TYPE                     AcwProvLink;
    EDDP_CRT_CHAIN_TYPE                     AcwPhaseLink;
    EDDP_CRT_CHAIN_BASE_TYPE                AcwDGList;
    LSA_UINT16                              AcwId;
    LSA_UINT16                              State;
    LSA_UINT8                               ROGPhase;
    LSA_UINT16                              DataStatusId;
    EDDP_CRT_PROVIDER_ACW_ADD_PARAM_TYPE    AddParam;
    EDDP_CRT_PROVIDER_ACW_DFP_PARAM_TYPE    DfpParam;
    LSA_VOID_PTR_TYPE                       pHwAcwApi;
} EDDP_CRT_PROVIDER_ACW_TYPE;
/* UsedFreeLink             : chain element in used/free ACW list                                                   */
/* AcwProvLink              : chain element in ACW list, which belong to one single provider                        */
/* AcwPhaseLink             : chain element in ACW list, which belong to one phase in Trog                          */
/* AcwDGList                : beginn/end of ACW_DG list, which belong to this ACW                                   */
/* AcwId                    : Index in ACW array                                                                    */
/* State                    : ACW-State                                                                             */
#define EDDP_CRT_PROV_ACW_STATE_IN_SW_MASK              0x0001      // ACW is used in SW
#define EDDP_CRT_PROV_ACW_STATE_IN_SW_SHIFT             0
#define EDDP_CRT_PROV_ACW_STATE_IN_HW_MASK              0x0002      // ACW is used in HW (available APICtrl-RAM)
#define EDDP_CRT_PROV_ACW_STATE_IN_HW_SHIFT             1
#define EDDP_CRT_PROV_ACW_STATE_IN_PHASELIST_MASK       0x0004      // ACW is added and chained to PhaseList in SW-TBase
#define EDDP_CRT_PROV_ACW_STATE_IN_PHASELIST_SHIFT      2
#define EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK             0x0008      // ACW is active in HW
#define EDDP_CRT_PROV_ACW_STATE_ACTIVE_SHIFT            3
#define EDDP_CRT_PROV_ACW_STATE_TREDUCTIONSELECT_MASK   0x0010      // ACW is counted for ACW_Treductionselect
#define EDDP_CRT_PROV_ACW_STATE_TREDUCTIONSELECT_SHIFT  4
#define EDDP_CRT_PROV_ACW_STATE_BIT_OFF                 0x0         // Bit in State activate
#define EDDP_CRT_PROV_ACW_STATE_BIT_ON                  0x1         // Bit in State deactivate
/* ROGPhase                 : Red, Orange or Green Phase                                                            */
#define EDDP_CRT_PROV_ACW_PHASE_INVALID     (EDDP_CRT_FRAMEID_RTCLASS_INVALID)
#define EDDP_CRT_PROV_ACW_PHASE_G           (EDDP_CRT_FRAMEID_RTCLASS_1)    // RTClass1 (Green)     "ACW_G_Ptr" of TrogEntry
#define EDDP_CRT_PROV_ACW_PHASE_O           (EDDP_CRT_FRAMEID_RTCLASS_2)    // RTClass2 (Orange)    "ACW_O_Ptr" of TrogEntry
#define EDDP_CRT_PROV_ACW_PHASE_R           (EDDP_CRT_FRAMEID_RTCLASS_3)    // RTClass3 (Red)       "ACW_R_Ptr" of TrogEntry
/* DataStatusId             : Index in DataStatus array (only for pack frame)                                       */
/* AddParam                 : parameters for adding a ACW to PhaseList                                              */
/* DfpParam                 : parameters for DFP-ACW                                                                */
/* pHwAcwApi                : pointer to this ACW in APICtrl-RAM                                                    */


// only relevant for HERA: contains additional Param-Data for ACW_DG which are checked when activating a Provider (only for later checks)
typedef struct _EDDP_CRT_PROVIDER_ACW_DG_PARAM_CHECK_TYPE
{
    LSA_UINT32          DataOffset;
} EDDP_CRT_PROVIDER_ACW_DG_PARAM_CHECK_TYPE;
/* DataOffset   : DataOffset from Provider-ADD/CONTROL                                                              */

// contains Param-Data for adding a ACW_DG to ACW
typedef struct _EDDP_CRT_PROVIDER_ACW_DG_ADD_PARAM_TYPE
{
    LSA_UINT16          DG_PosT;
    LSA_UINT16          DG_Len;
    LSA_UINT32          DG_Offset;
} EDDP_CRT_PROVIDER_ACW_DG_ADD_PARAM_TYPE;
/* DG_PosT      : DataGramm Pos/T   (from PRM PDIRSubframeData.SubframeBlock.SubframeData)                          */
/* DG_Len       : DataGramm length  (from PRM PDIRSubframeData.SubframeBlock.SubframeData)                          */
/* DG_Offset    : DataGramm offset  (from PRM PDIRSubframeData.SubframeBlock.SubframeData)                          */


// Management structure for 1 single ACW_DG     -> pAcwDGEntry[AcwDGId]
typedef struct _EDDP_CRT_PROVIDER_ACW_DG_TYPE   EDDP_LOCAL_MEM_ATTR *       EDDP_CRT_PROVIDER_ACW_DG_PTR_TYPE;
typedef struct _EDDP_CRT_PROVIDER_ACW_DG_TYPE
{
    EDDP_CRT_CHAIN_TYPE                         UsedFreeLink;
    EDDP_CRT_CHAIN_TYPE                         AcwDGAcwLink;
    EDDP_CRT_CHAIN_TYPE                         AcwDGAcwLinkRed1;
    LSA_UINT16                                  AcwDGId;
    LSA_UINT16                                  AcwId;
    LSA_UINT16                                  AcwIdRed1;
    LSA_UINT16                                  State;
    EDDP_CRT_PROVIDER_ACW_DG_ADD_PARAM_TYPE     AddParam;
    EDDP_CRT_PROVIDER_ACW_DG_PARAM_CHECK_TYPE   DfpParamCheck; // only relevant for HERA
    PNIP_ACWDG_SND_PTR_TYPE                     pHwAcwDGApi;
} EDDP_CRT_PROVIDER_ACW_DG_TYPE;
/* UsedFreeLink             : chain element in used/free ACW_DG list                                                                */
/* AcwDGAcwLink             : chain element in ACW_DG list, which belong to one single ACW                                          */
/* AcwDGAcwLinkRed1         : redundant reversed chain element in ACW_DG list, which belong to one single ACW with FrameID(x+1)     */
/* AcwDGId                  : Index in ACW_DG array                                                                                 */
/* AcwId                    : ACW, to which this ACW_DG belongs                                                                     */
/* AcwIdRed1                : redundant ACW with FrameID(x+1), to which this ACW_DG belongs                                         */
/* State                    : see ACW                                                                                               */
/* AddParam                 : parameters for adding a ACW_DG to ACW                                                                 */
/* DfpParamCheck            : additional parameters for ACW_DG (only for later checks)                                              */
/* pHwAcwDGApi              : pointer to this ACW_DG in APICtrl-RAM                                                                 */


// Management structure for 1 single pack group     -> pProvPackGroupEntry[PackGrId]
typedef struct _EDDP_CRT_PROVIDER_PACK_GROUP_TYPE   EDDP_LOCAL_MEM_ATTR *   EDDP_CRT_PROVIDER_PACK_GROUP_PTR_TYPE;
typedef struct _EDDP_CRT_PROVIDER_PACK_GROUP_TYPE
{
    EDDP_CRT_CHAIN_TYPE             UsedFreeLink;
    LSA_UINT16                      PackGrId;
    LSA_UINT16                      AcwId;
    LSA_BOOL                        isRedundant1;
} EDDP_CRT_PROVIDER_PACK_GROUP_TYPE;
/* UsedFreeLink         : chain element in used/free PackGroupList                                                  */
/* PackGrId             : Index in pack group array                                                                 */
/* AcwId                : AcwId of connected ACW                                                                    */
/* isRedundant1         : is this pack group redundant with FrameID(x+1) ?                                          */


// Management structure for 1 single provider group pProvGroupEntry[GroupId]
typedef struct _EDDP_CRT_PROVIDER_GROUP_TYPE    EDDP_LOCAL_MEM_ATTR *       EDDP_CRT_PROVIDER_GROUP_PTR_TYPE;
typedef struct _EDDP_CRT_PROVIDER_GROUP_TYPE
{
    EDDP_CRT_CHAIN_BASE_TYPE        GroupList;
    LSA_UINT8                       GroupDataStatus;
    LSA_UINT32                      ProviderCnt;
} EDDP_CRT_PROVIDER_GROUP_TYPE;
/* GroupList            : beginn/end of provider list in this group                         */
/* GroupDataStatus      : DataStatus of provider group                                      */
/* ProviderCnt          : Number of providers in this group                                 */


// Management structure for 1 single AutoStop provider group pASGroupEntry[ConsumerId]
typedef struct _EDDP_CRT_PROVIDER_ASGROUP_TYPE  EDDP_LOCAL_MEM_ATTR *       EDDP_CRT_PROVIDER_ASGROUP_PTR_TYPE;
typedef struct _EDDP_CRT_PROVIDER_ASGROUP_TYPE
{
    EDDP_CRT_CHAIN_BASE_TYPE        ASGroupList;
    LSA_UINT32                      ProviderCnt;
} EDDP_CRT_PROVIDER_ASGROUP_TYPE;
/* GroupList            : beginn/end of provider list in this AutoStop group                */
/* ProviderCnt          : Number of providers in this AutoStop group                        */


// Trog: Red-Orange-Green send lists
typedef struct _EDDP_CRT_PROVIDER_TROG_ACW_LIST_TYPE
{
    EDDP_CRT_CHAIN_BASE_TYPE        AcwPhaseList_R;     // Acw1 -> Acw2 -> ... -> AcwN
    LSA_UINT32                      AcwPhaseCnt_R;
    LSA_UINT32                      AcwPhaseActCnt_R;
    LSA_UINT32                      AcwDfpPhaseCnt_R;

    EDDP_CRT_CHAIN_BASE_TYPE        AcwPhaseList_O;
    LSA_UINT32                      AcwPhaseCnt_O;
    LSA_UINT32                      AcwPhaseActCnt_O;

    EDDP_CRT_CHAIN_BASE_TYPE        AcwPhaseList_G;
    LSA_UINT32                      AcwPhaseCnt_G;
    LSA_UINT32                      AcwPhaseActCnt_G;
} EDDP_CRT_PROVIDER_TROG_ACW_LIST_TYPE;
/* AcwPhaseList_R       : beginn/end of ACW list of Red phase                                           */
/* AcwPhaseCnt_R        : Number of all ACWs in list of Red phase                                       */
/* AcwPhaseActCnt_R     : Number of all active ACWs in list of Red phase                                */
/* AcwDfpPhaseCnt_R     : Number of all DFP-ACWs in list of Red phase                                   */
/* AcwPhaseList_O       : beginn/end of ACW list of Orange phase                                        */
/* AcwPhaseCnt_O        : Number of all ACWs in list of Orange phase                                    */
/* AcwPhaseActCnt_O     : Number of all active ACWs in list of Orange phase                             */
/* AcwPhaseList_G       : beginn/end of ACW list of Green phase                                         */
/* AcwPhaseCnt_G        : Number of all ACWs in list of Green phase                                     */
/* AcwPhaseActCnt_G     : Number of all active ACWs in list of Green phase                              */


// Management structure for 1 Trog-Entry    -> pTrogEntry[TrogId]
typedef struct _EDDP_CRT_PROVIDER_TROG_TYPE     EDDP_LOCAL_MEM_ATTR *       EDDP_CRT_PROVIDER_TROG_PTR_TYPE;
typedef struct _EDDP_CRT_PROVIDER_TROG_TYPE
{
    EDDP_CRT_CHAIN_TYPE                     UsedFreeLink;
    LSA_UINT16                              TrogId;
    PNIP_ACW_SND_TROG_PTR_TYPE              pHwTrogApi;
    EDDP_CRT_PROVIDER_TROG_ACW_LIST_TYPE    TrogAcwList;
} EDDP_CRT_PROVIDER_TROG_TYPE;
/* UsedFreeLink     : LinkList of used Trog-Entries                                                 */
/* TrogId           : Id in Trog array                                                              */
/* pHwTrogApi       : pointer to Trog (Red-Orange-Green-Pointer) in APICtrl-RAM                     */
/* TrogAcwList      : struct of Red-Orange-Green send lists                                         */


// Management structure for 1 TBase-Entry   -> pTBaseEntry[Reduction + Phase]
typedef struct _EDDP_CRT_PROVIDER_TBASE_TYPE     EDDP_LOCAL_MEM_ATTR *       EDDP_CRT_PROVIDER_TBASE_PTR_TYPE;
typedef struct _EDDP_CRT_PROVIDER_TBASE_TYPE
{
    LSA_BOOL                                TBaseElementInHw;
    LSA_UINT16                              TBaseElement;
    LSA_UINT8                               ReductionNr;
    EDDP_CRT_PROVIDER_TROG_PTR_TYPE         pTrogEntry;
    PNIP_ACW_SND_TBASE_PTR_TYPE             pHwTBaseApi;
} EDDP_CRT_PROVIDER_TBASE_TYPE;
/* TBaseElementInHw : Flag - "TBaseElement" is already initialized in HW                            */
/* TBaseElement     : is identical with TBaseElement in PNIP_ACW_SND_TBASE_PTR_TYPE                 */
/* ReductionNr      : ReductionNr(1..15) of this TBase-Entry                                        */
/* pTrogEntry       : pointer to array of Trogs -> 1 single Trog: pTrogEntry[TrogId]                */
/* pHwTBaseApi      : pointer to TBase (one TBaseElement in ACW reduction list) in APICtrl-RAM      */


// Management structure for 1 DataStatus-Entry      -> pDSEntry[Idx]
typedef struct _EDDP_CRT_PROVIDER_DS_TYPE       EDDP_LOCAL_MEM_ATTR *       EDDP_CRT_PROVIDER_DS_PTR_TYPE;
typedef struct _EDDP_CRT_PROVIDER_DS_TYPE
{
    EDDP_CRT_CHAIN_TYPE             UsedFreeLink;
    LSA_UINT8                       LocalDataStatus;
    LSA_UINT8                       DS_Selector;
    #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
    PNIP_R1_ACW_SND_DS_PTR_TYPE     pHwDSApiPnipR1;
    #endif
    #if defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
    PNIP_R2_ACW_SND_DS_PTR_TYPE     pHwDSApiPnipR2;
    #endif
} EDDP_CRT_PROVIDER_DS_TYPE;
/* UsedFreeLink     : chain element in used/free DataStatus list                                    */
/* LocalDataStatus  : DataStatus to create the APDU.DataStatus                                      */
/* DS_Selector      : GroupId (from PROVIDER_ADD) to select the register ApplDataStatus_0-7         */
/* pHwDSApiPnipR1   : pointer to this DataStatus in APICtrl-RAM                                     */
/* pHwDSApiPnipR2   : pointer to this DataStatus in APICtrl-RAM                                     */

// Management structure for 1 Treductionselect-Entry    -> Treductionselect[ReductionNr]
// it is used for PNIP register "ACW_Treductionselect_IFA/B"
typedef struct _EDDP_CRT_PROVIDER_TREDUCTIONSELECT_TYPE
{
    LSA_UINT32          AcwCnt;
} EDDP_CRT_PROVIDER_TREDUCTIONSELECT_TYPE;
/* AcwCnt           : Number of all ACWs (active/passive) in HW-TBase that are chained in the same reduction    */
/*                    The counter is modified with commands ACW_in/ACW_out                                      */


/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */
/* State machines for Provider, Consumer + PackCtrl                                     */
/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------------------ */
/* State machine "Provider-PackGroupActivate-All"                                       */
/* ------------------------------------------------------------------------------------ */
typedef enum _EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_TYPE
{
    EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_IDLE                = 0,
    EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_START               = 1,
    EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_NEXT_PACK_GROUP     = 2,
    EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_ACW_IN              = 3,
    EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_ACW_ON              = 4,
    EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_FINISH              = 5
} EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_TYPE;

typedef struct _EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_SM_TYPE   EDDP_LOCAL_MEM_ATTR *   EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_SM_PTR_TYPE;
typedef struct _EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_SM_TYPE
{
    EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_TYPE    State;
    EDD_RSP                                                 Response;
    LSA_UINT16                                              ActPackGrId;
    EDDP_CRT_CBF_PTR_TYPE                                   Cbf;
} EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_SM_TYPE;
/* ActPackGrId  : PackGrId of actual state                                              */


/* ------------------------------------------------------------------------------------ */
/* State machine "Provider-PackGroupPassivate-All"                                      */
/* ------------------------------------------------------------------------------------ */
typedef enum _EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_TYPE
{
    EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_IDLE               = 0,
    EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_START              = 1,
    EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_NEXT_PACK_GROUP    = 2,
    EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_ACW_OFF            = 3,
    EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_ACW_OUT            = 4,
    EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_FINISH             = 5
} EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_TYPE;

typedef struct _EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_SM_TYPE  EDDP_LOCAL_MEM_ATTR *   EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_SM_PTR_TYPE;
typedef struct _EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_SM_TYPE
{
    EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_TYPE   State;
    EDD_RSP                                                 Response;
    LSA_UINT16                                              ActPackGrId;
    EDDP_CRT_CBF_PTR_TYPE                                   Cbf;
} EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_SM_TYPE;
/* ActPackGrId  : PackGrId of actual state                                              */


/* ------------------------------------------------------------------------------------ */
/* State machine "CRT-Activate-Pack"                                                    */
/* ------------------------------------------------------------------------------------ */
typedef enum _EDDP_CRT_PACK_ACTIVATE_STATE_TYPE
{
    EDDP_CRT_PACK_ACTIVATE_STATE_IDLE               = 0,
    EDDP_CRT_PACK_ACTIVATE_STATE_START              = 1,
    EDDP_CRT_PACK_ACTIVATE_STATE_SND_ACW_ON         = 2,
    EDDP_CRT_PACK_ACTIVATE_STATE_RCV_ACW_ON         = 3,
    EDDP_CRT_PACK_ACTIVATE_STATE_INBOUND_PACK_ON    = 4,
    EDDP_CRT_PACK_ACTIVATE_STATE_OUTBOUND_PACK_ON   = 5,
    EDDP_CRT_PACK_ACTIVATE_STATE_FINISH             = 6
} EDDP_CRT_PACK_ACTIVATE_STATE_TYPE;

typedef struct _EDDP_CRT_PACK_ACTIVATE_SM_TYPE  EDDP_LOCAL_MEM_ATTR *   EDDP_CRT_PACK_ACTIVATE_SM_PTR_TYPE;
typedef struct _EDDP_CRT_PACK_ACTIVATE_SM_TYPE
{
    EDDP_CRT_PACK_ACTIVATE_STATE_TYPE       State;
    EDD_RSP                                 Response;
    EDDP_CRT_CBF_PTR_TYPE                   Cbf;
} EDDP_CRT_PACK_ACTIVATE_SM_TYPE;


/* ------------------------------------------------------------------------------------ */
/* State machine "CRT-Passivate-Pack"                                                   */
/* ------------------------------------------------------------------------------------ */
typedef enum _EDDP_CRT_PACK_PASSIVATE_STATE_TYPE
{
    EDDP_CRT_PACK_PASSIVATE_STATE_IDLE                  = 0,
    EDDP_CRT_PACK_PASSIVATE_STATE_START                 = 1,
    EDDP_CRT_PACK_PASSIVATE_STATE_WAITING_BREAK         = 2,
    EDDP_CRT_PACK_PASSIVATE_STATE_INBOUND_PACK_OFF      = 3,
    EDDP_CRT_PACK_PASSIVATE_STATE_OUTBOUND_PACK_OFF     = 4,
    EDDP_CRT_PACK_PASSIVATE_STATE_SND_ACW_OFF           = 5,
    EDDP_CRT_PACK_PASSIVATE_STATE_RCV_ACW_OFF           = 6,
    EDDP_CRT_PACK_PASSIVATE_STATE_FINISH                = 7
} EDDP_CRT_PACK_PASSIVATE_STATE_TYPE;

typedef struct _EDDP_CRT_PACK_PASSIVATE_SM_TYPE     EDDP_LOCAL_MEM_ATTR *   EDDP_CRT_PACK_PASSIVATE_SM_PTR_TYPE;
typedef struct _EDDP_CRT_PACK_PASSIVATE_SM_TYPE
{
    EDDP_CRT_PACK_PASSIVATE_STATE_TYPE      State;
    EDD_RSP                                 Response;
    EDDP_CRT_CBF_PTR_TYPE                   Cbf;
} EDDP_CRT_PACK_PASSIVATE_SM_TYPE;


/* ------------------------------------------------------------------------------------ */
/* State machine "Provider-ACW_DG-Activate"                                             */
/* ------------------------------------------------------------------------------------ */
typedef enum _EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_TYPE
{
    EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_IDLE            = 0,
    EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_START           = 1,
    EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_ACW_DG_ON       = 2,
    EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_ACW_IN          = 3,
    EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_ACWRED1_IN      = 4,
    EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_FINISH          = 5
} EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_TYPE;

typedef struct _EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_SM_TYPE   EDDP_LOCAL_MEM_ATTR *   EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_SM_PTR_TYPE;
typedef struct _EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_SM_TYPE
{
    EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_TYPE        State;
    EDD_RSP                                             Response;
    LSA_UINT16                                          ProviderId;
    EDDP_CRT_PROV_DG_CBF_PTR_TYPE                       Cbf;
    LSA_BOOL                                            InUse;
    EDD_UPPER_RQB_PTR_TYPE                              pRQB;
    LSA_UINT16                                          CmdCnt;
    LSA_UINT16                                          Index;
} EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_SM_TYPE;


/* ------------------------------------------------------------------------------------ */
/* State machine "Provider-ACW_DG-Passivate"                                            */
/* ------------------------------------------------------------------------------------ */
typedef enum _EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_TYPE
{
    EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_IDLE           = 0,
    EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_START          = 1,
    EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_ACW_OUT        = 2,
    EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_ACWRED1_OUT    = 3,
    EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_ACW_DG_OFF     = 4,
    EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_FINISH         = 5
} EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_TYPE;

typedef struct _EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_SM_TYPE  EDDP_LOCAL_MEM_ATTR *   EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_SM_PTR_TYPE;
typedef struct _EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_SM_TYPE
{
    EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_TYPE       State;
    EDD_RSP                                             Response;
    LSA_UINT16                                          ProviderId;
    EDDP_CRT_PROV_DG_CBF_PTR_TYPE                       Cbf;
    LSA_BOOL                                            InUse;
    EDD_UPPER_RQB_PTR_TYPE                              pRQB;
    LSA_UINT16                                          CmdCnt;
    LSA_UINT16                                          Index;
} EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_SM_TYPE;


/* ------------------------------------------------------------------------------------ */
/* signature of CRT PPM ACW-functions                                                   */
/* ------------------------------------------------------------------------------------ */
typedef LSA_VOID   LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, EDDP_CRT_PROV_ACW_INIT_RTFRAME_PTR_TYPE)(EDDP_LOCAL_DDB_PTR_TYPE, LSA_UINT16, LSA_UINT16);
typedef LSA_VOID   LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, EDDP_CRT_PROV_ACW_INIT_PACKFRAME_PTR_TYPE)(EDDP_LOCAL_DDB_PTR_TYPE, LSA_UINT16, LSA_BOOL);
typedef LSA_VOID   LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, EDDP_CRT_PROV_ACW_UPDATE_NEXT_OFFSET_PTR_TYPE)(EDDP_LOCAL_DDB_PTR_TYPE, LSA_UINT16);
typedef LSA_VOID   LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, EDDP_CRT_PROV_ACW_WRITE_SB_OFFSET_PTR_TYPE)(EDDP_LOCAL_DDB_PTR_TYPE, LSA_UINT16, LSA_UINT16);
typedef LSA_VOID   LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, EDDP_CRT_PROV_ACW_DG_UPDATE_NEXT_OFFSET_PTR_TYPE)(EDDP_LOCAL_DDB_PTR_TYPE, LSA_UINT16, LSA_UINT16);
typedef LSA_VOID   LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, EDDP_CRT_PROV_ACW_DG_INIT_HW_API_FROM_PRM_COMMIT_PTR_TYPE)(EDDP_LOCAL_DDB_PTR_TYPE, LSA_UINT16);
typedef LSA_VOID   LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, EDDP_CRT_PROV_ACW_DG_INIT_HW_API_FROM_PROVIDER_PTR_TYPE)(EDDP_LOCAL_DDB_PTR_TYPE, LSA_UINT16, LSA_UINT16);


/* ------------------------------------------------------------------------------------ */
/* struct provider ACW handler functions                                                */
/* ------------------------------------------------------------------------------------ */
typedef struct _EDDP_CRT_PROVIDER_ACW_HANDLER_TYPE
{
    EDDP_CRT_PROV_ACW_INIT_RTFRAME_PTR_TYPE                      FctInitHwApiRTFrame;
    EDDP_CRT_PROV_ACW_INIT_PACKFRAME_PTR_TYPE                    FctInitHwApiPackFrame;
    EDDP_CRT_PROV_ACW_UPDATE_NEXT_OFFSET_PTR_TYPE                FctUpdateAcwNextOffsetAcwHwApi;
    EDDP_CRT_PROV_ACW_WRITE_SB_OFFSET_PTR_TYPE                   FctWriteSBOffset;
    EDDP_CRT_PROV_ACW_DG_UPDATE_NEXT_OFFSET_PTR_TYPE             FctUpdateAcwDGNextOffsetAcwHwApi;
	EDDP_CRT_PROV_ACW_DG_INIT_HW_API_FROM_PRM_COMMIT_PTR_TYPE    FctDGInitHwApiFromPrmCommit;
	EDDP_CRT_PROV_ACW_DG_INIT_HW_API_FROM_PROVIDER_PTR_TYPE      FctDGInitHwApiFromProvider;
} EDDP_CRT_PROVIDER_ACW_HANDLER_TYPE;
/* FctUpdateAcwNextOffsetAcwHwApi  : update next offset within ACW to next ACW          */
/* FctWriteSBOffset                : write SB offset to ACW(_DG)                        */


/* ------------------------------------------------------------------------------------ */
/* State machine "ApplTimerIfSetup" for TransferEnd                                     */
/* ------------------------------------------------------------------------------------ */
typedef enum _EDDP_CRT_APPL_TIMER_STATE_TYPE
{
    EDDP_CRT_APPL_TIMER_STATE_IDLE                     = 0,
    EDDP_CRT_APPL_TIMER_STATE_START                    = 1,
    EDDP_CRT_APPL_TIMER_STATE_SETUP_TRANSFER_END_ON    = 2,
    EDDP_CRT_APPL_TIMER_STATE_UPDATE_APPL_LENGTH_ON    = 3,
    EDDP_CRT_APPL_TIMER_STATE_CONNECT_SYNC             = 4,
    EDDP_CRT_APPL_TIMER_STATE_FINISH                   = 5
} EDDP_CRT_APPL_TIMER_STATE_TYPE;

typedef struct _EDDP_CRT_APPL_TIMER_SM_TYPE    EDDP_LOCAL_MEM_ATTR *   EDDP_CRT_APPL_TIMER_SM_PTR_TYPE;
typedef struct _EDDP_CRT_APPL_TIMER_SM_TYPE
{
    EDDP_CRT_APPL_TIMER_STATE_TYPE     State;
    EDD_RSP                            Response;
    EDDP_CRT_CBF_PTR_TYPE              Cbf;
} EDDP_CRT_APPL_TIMER_SM_TYPE;


// Minimum, Maximum ApplTimerReductionRatio
#define EDDP_CRT_APPL_TIMER_REDUCTION_RATIO_MIN         1
#define EDDP_CRT_APPL_TIMER_REDUCTION_RATIO_MAX         16
// check macro: is ApplTimerReductionRatio in valid range
#define EDDP_CRT_IS_APPL_TIMER_REDUCTION_RATIO_IN_VALID_RANGE(ApplTimerReductionRatio) \
            ( (ApplTimerReductionRatio >= EDDP_CRT_APPL_TIMER_REDUCTION_RATIO_MIN) && (ApplTimerReductionRatio <= EDDP_CRT_APPL_TIMER_REDUCTION_RATIO_MAX) )
/* TransferEndCorrectionValue   : Value in ns for correction of TransferEnd interrupt   see <eddp_sys.h>                                */
// Minimum, Maximum TransferEndCorrectionValue
//#define EDDP_CRT_TRANSFEREND_CORRECTION_VALUE_MIN       0 //produces compiler warnings if 0
#define EDDP_CRT_TRANSFEREND_CORRECTION_VALUE_MAX       100000
// check macro: is TransferEndCorrectionValue in valid range
#define EDDP_CRT_IS_TRANSFEREND_CORRECTION_VALUE_IN_VALID_RANGE(TransferEndCorrectionValue, isTransferEndCorrectionPos)                                                         \
            ( /*(TransferEndCorrectionValue >= EDDP_CRT_TRANSFEREND_CORRECTION_VALUE_MIN) &&*/ (TransferEndCorrectionValue <= EDDP_CRT_TRANSFEREND_CORRECTION_VALUE_MAX)        \
                && ((EDDP_DPB_TRANSFERENDCORRECTIONVALUE_IS_NEG == isTransferEndCorrectionPos) || (EDDP_DPB_TRANSFERENDCORRECTIONVALUE_IS_POS == isTransferEndCorrectionPos))   \
            )
/* isTransferEndCorrectionPos     : This parameter set the "TransferEndCorrectionValue" to positive or negative     see <eddp_sys.h>    */




/* ============================================================================ */
/*                 Management for internal command elements                     */
/* ============================================================================ */
typedef struct _EDDP_CRT_INTERNAL_CMD_PARAM_TYPE    EDDP_LOCAL_MEM_ATTR *       EDDP_CRT_INTERNAL_CMD_PARAM_PTR_TYPE;
typedef struct _EDDP_CRT_INTERNAL_CMD_PARAM_TYPE
{
    EDDP_CMD_PARAM_PTR_TYPE     pInternalCmdParam;
    EDDP_CMD_PARAM_PTR_TYPE     pNextFreeCmdParam;
    EDDP_CMD_PARAM_PTR_TYPE     pNextToBeReleasedCmdParam;
    LSA_UINT16                  CmdParamCnt;
    LSA_UINT16                  UsageCnt;
} EDDP_CRT_INTERNAL_CMD_PARAM_TYPE;
/* pInternalCmdParam         : pointer to internal CRT command parameter elements                 */
/* pNextFreeCmdParam         : pointer to next free CRT command parameter element                 */
/* pNextToBeReleasedCmdParam : pointer to next to be released CRT command parameter element       */
/* CmdParamCnt               : amount of internal CRT command parameter elements                  */
/* InUseCnt                  : amount of internal CRT command parameter elements currently in use */




/* ============================================================================ */
/*                  command parameter for CMD-Interface                         */
/* ============================================================================ */
typedef struct _EDDP_CRT_CMD_PARAM_TYPE
{
    /* CPM and PPM internal CMD param */
    EDDP_CRT_INTERNAL_CMD_PARAM_TYPE  CrtInternalCmdParam;
    
    /* ApplTimer */
    EDDP_CMD_PARAM_TYPE         Connect_Sync_A_B;
    EDDP_CMD_PARAM_TYPE         Update_ApplCycle;
    EDDP_CMD_PARAM_TYPE         Ppm_Starttime;
} EDDP_CRT_CMD_PARAM_TYPE;





/* ============================================================================ */
/*                      Management for indications                              */
/* ============================================================================ */
typedef struct _EDDP_CRT_INDPROVIDE_TYPE    EDDP_LOCAL_MEM_ATTR *       EDDP_CRT_INDPROVIDE_PTR_TYPE;
typedef struct _EDDP_CRT_INDPROVIDE_TYPE
{
    LSA_BOOL                    ConsIndicationPending;
    EDDP_RQB_QUEUE_TYPE         RqbQueue;
} EDDP_CRT_INDPROVIDE_TYPE;
/* ConsIndicationPending    : TRUE if consumer indications pend.                                */
/* RqbQueue                 : RQB-Queue for indications                                         */





#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
/* ============================================================================ */
/*      Management for workaround "PackGrp Disable" in PN-IP Rev2               */
/* ============================================================================ */
/* TimerUser[x]                                                                                 */
/*  0 - RTC3PSM is asynchron for PortID-1 (not HwPortID)                                        */
/*  1 - RTC3PSM is asynchron for PortID-2 (not HwPortID)                                        */
/*  2 - CRT-Passivate-Pack state machine                                                        */
#define EDDP_CRT_WRKAR_PACKGRP_DISABLE_TIMER_USER_IDX_RTC3PSM_P1                0
#define EDDP_CRT_WRKAR_PACKGRP_DISABLE_TIMER_USER_IDX_RTC3PSM_P2                1
#define EDDP_CRT_WRKAR_PACKGRP_DISABLE_TIMER_USER_IDX_CRT_PASSIVE_PACK_SM       2
#define EDDP_CRT_WRKAR_PACKGRP_DISABLE_TIMER_USER_MAX_NUMBER                    3

typedef struct _EDDP_CRT_WRKAR_PACKGRP_DISABLE_TIMER_USER_TYPE
{
    LSA_BOOL            isTimerUsed;
    LSA_UINT32          TimeTicks;
    LSA_UINT32          RegValuePhaseEnable;
} EDDP_CRT_WRKAR_PACKGRP_DISABLE_TIMER_USER_TYPE;
/* isTimerUsed          : TRUE if the timer is used by this user currently                      */
/* TimeTicks            : count the numer of LSA-Timer-Ticks                                    */
#define EDDP_CRT_WRKAR_PACKGRP_DISABLE_MAX_TIME_TICKS   2   // 2 * 100ms = 200ms
/* RegValuePhaseEnable  : store/restore the value of PNIP_REG_PHASEENABLE_Px                    */

typedef struct _EDDP_CRT_WRKAR_PACKGRP_DISABLE_MNGM_TYPE
{
    LSA_UINT32                                          LSATimer;
    LSA_BOOL                                            isLSATimerRunning;
    LSA_UINT16                                          TimerUserCnt;
    EDDP_CRT_WRKAR_PACKGRP_DISABLE_TIMER_USER_TYPE      TimerUser[EDDP_CRT_WRKAR_PACKGRP_DISABLE_TIMER_USER_MAX_NUMBER];
} EDDP_CRT_WRKAR_PACKGRP_DISABLE_MNGM_TYPE;
/* LSATimer             : LSA-Timer                                                             */
#define EDDP_CRT_WRKAR_PACKGRP_DISABLE_TIME     1   // 1 * 100ms = 100ms
/* isLSATimerRunning    : Flag - the LSATimer is current running                                */
/* TimerUserCnt         : counter of users who use the timer currently                          */
/* TimerUser            : array of timer users                                                  */
#endif


#define EDDP_CRT_BITFLAG_INIT                   0               // init value
#define EDDP_CRT_BITFLAG_DEV_OPEN_MASK          0x00000001      // EDD_SRV_DEV_OPEN is ready in CRT
#define EDDP_CRT_BITFLAG_DEV_OPEN_SHIFT         0
#define EDDP_CRT_BITFLAG_DEV_SETUP_MASK         0x00000002      // EDDP_SRV_DEV_SETUP is ready in CRT
#define EDDP_CRT_BITFLAG_DEV_SETUP_SHIFT        1
#define EDDP_CRT_BITFLAG_DEV_IF_SETUP_MASK      0x00000004      // EDDP_SRV_DEV_IF_SETUP is ready in CRT
#define EDDP_CRT_BITFLAG_DEV_IF_SETUP_SHIFT     2
#define EDDP_CRT_BITFLAG_OPEN_CHANNEL_MASK      0x00000008      // OpenChannel is ready in CRT
#define EDDP_CRT_BITFLAG_OPEN_CHANNEL_SHIFT     3
/* Prov                 : Provider management                                                                           */
/* Cons                 : Consumer management                                                                           */
/* ConsSB               : Consumer Score-Board management                                                               */
/* IndProvide           : Management for indications                                                                    */
/* CmdParam             : Command parameter for CMD-Interface                                                           */
/* CfgParam             : Initialization/configuration parameter for CRT                                                */
/* WrkarPackGrpDisable  : Management for workaround "PackGrp Disable" in PN-IP Rev2                                     */
/* pIOBaseAdrPerif      : Base address of IO-Buffer for IO-Data in PERIF                                                */
/* pIOBaseAdrExtRam     : Base address of IO-Buffer for IO-Data in external RAM                                         */
/* PackActivateSm       : state machine "CRT-Activate-Pack"                                                             */
/* PackPassivateSm      : state machine "CRT-Passivate-Pack"                                                            */
/* ApplTimerIfSetupSm   : state machine "ApplTimerIfSetup"                                                              */


/* ============================================================================ */
/*                          structs Inbound - DDB                               */
/* ============================================================================ */
typedef struct _EDDP_CRT_INBOUND_LATE_ERR_DIAG_TYPE
{
    LSA_UINT8       TimerType;
    LSA_UINT32      TimeTicks;
    LSA_UINT16      PrmPDIRFrameDataIdx;
} EDDP_CRT_INBOUND_LATE_ERR_DIAG_TYPE;
/* TimerType            : TimerType                                                                                 */
#define EDDP_CRT_INBOUND_LATE_ERR_TIMER_TYPE_INVALID                (EDDP_CRT_INVALID_VALUE_8)
#define EDDP_CRT_INBOUND_LATE_ERR_TIMER_TYPE_PACKGRP_ACTIVATE       0x1
#define EDDP_CRT_INBOUND_LATE_ERR_TIMER_TYPE_PACKGRP_LATE_ERROR     0x2
/* TimeTicks            : count the numer of LSA-Timer-Ticks                                                        */
#define EDDP_CRT_INBOUND_LATE_ERR_MAX_TIME_TICKS                    5   // 5 * 200ms = 1s
/* PrmPDIRFrameDataIdx  : connected index in PDIRData.PDIRFrameData[x]                                              */

typedef struct _EDDP_CRT_INBOUND_DG_TYPE    EDDP_LOCAL_MEM_ATTR *   EDDP_CRT_INBOUND_DG_PTR_TYPE;
typedef struct _EDDP_CRT_INBOUND_DG_TYPE
{
    LSA_UINT8       PackPosT;
    LSA_UINT8       PackLen;
    LSA_UINT16      Offset_DG;
} EDDP_CRT_INBOUND_DG_TYPE;
/* PackPosT             : PackPosT of this DG (SubPackFrame) within PackRAM_0                                       */
/* PackLen              : PackLen of this DG (SubPackFrame) within PackRAM_0                                        */
/* Offset_DG            : Offset_DG (SubPackFrame) within PackRAM_0                                                 */

#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
typedef struct _EDDP_CRT_INBOUND_WRKAR_PACKGRP_DISABLE_TYPE
{
    LSA_UINT8       TxPort;
    LSA_UINT16      AcwId;
    LSA_UINT16      AcwIdRed1;
} EDDP_CRT_INBOUND_WRKAR_PACKGRP_DISABLE_TYPE;
/* TxPort       : for register InBoundPack_x.TxPort (it is HwPortID)                                                */
#define EDDP_CRT_INBOUND_TXPORT_INVALID     (EDDP_CRT_INVALID_VALUE_8)
/* AcwId        : Index in Snd-ACW array as connection to this InBound-Pack-ACW                                     */
/* AcwIdRed1    : redundant ACW with FrameID(x+1), only if 1 redundant InBound-Pack and 2 Send-ACWs on PPM          */
#endif

typedef struct _EDDP_CRT_INBOUND_GRP_TYPE   EDDP_LOCAL_MEM_ATTR *   EDDP_CRT_INBOUND_GRP_PTR_TYPE;
typedef struct _EDDP_CRT_INBOUND_GRP_TYPE
{
    LSA_UINT32                                      IfIdx;
    EDD_COMMON_MEM_U8_PTR_TYPE                      pPackRam1Start;
    LSA_UINT16                                      RxDisPort;
    LSA_UINT8                                       ReInitAsValue;
    LSA_UINT8                                       PackGrpId;
    LSA_UINT8                                       InboundDgCnt;
    EDDP_CRT_INBOUND_DG_TYPE                        InboundDGs[EDDP_CRT_MAX_INBOUND_DG_CNT_ERTEC200P];
    EDDP_CRT_INBOUND_LATE_ERR_DIAG_TYPE             LateErrDiag;
    #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
    EDDP_CRT_INBOUND_WRKAR_PACKGRP_DISABLE_TYPE     WrkarPackGrpDisable;
    #endif
} EDDP_CRT_INBOUND_GRP_TYPE;
/* IfIdx                : which interface uses this inbound group                                                   */
/* pPackRam1Start       : points to RAM1 for current Inbound group                                                  */
/* RxDisPort            : value for PNIP_REG_INBOUNDPACK_CONTROL_0_x.INBOUND_RXPORT_DIS (it is HwPortID)            */
/* ReInitAsValue        : init value for the AsWd-field within RAM1                                                 */
#define EDDP_CRT_INBOUND_REINITASVALUE_INIT             0x80
/* PackGrpId            : index in InboundGrps array                                                                */
/* InboundDgCnt         : number of valid entries within InboundDg[x]                                               */
/* InboundDGs           : Array for SubPackFrame                                                                    */
/* LateErrDiag          : LateError-Diagnosis management                                                            */
/* WrkarPackGrpDisable  : Management for workaround "PackGrp Disable" in PN-IP Rev2                                 */

typedef struct _EDDP_CRT_INBOUND_MNGM_TYPE     EDDP_LOCAL_MEM_ATTR *       EDDP_CRT_INBOUND_MNGM_PTR_TYPE;
typedef struct _EDDP_CRT_INBOUND_MNGM_TYPE
{
    LSA_UINT8                       InboundUsedMask[EDDP_MAX_PORT_CNT + 1];     // index 0 = all ports (it is HwPortID)
    LSA_UINT8                       InboundActiveMask;
    EDDP_CRT_INBOUND_GRP_TYPE       InboundGrps[EDDP_CRT_MAX_INBOUND_GRP_ERTEC200P];
    LSA_UINT32	                    LateErrLSATimer;
    LSA_BOOL                        isLateErrLSATimerRunning;
    LSA_UINT8                       LateErrPackGrpTimerUsedMask;
} EDDP_CRT_INBOUND_MNGM_TYPE;
/* InboundUsedMask              : mask to detect used inbound groups                                                */
/* InboundActiveMask            : mask to detect used + active inbound groups                                       */
/* InboundGrps                  : Array of all Inbound groups -> InboundGrps[InboundIdx]                            */
/* LateErrLSATimer              : LSA-Timer for InBound activate and interrupt "InBound_Late_ERR"                   */
#define EDDP_CRT_INBOUND_LATE_ERR_TIME      2   // 2 * 100ms = 200ms
/* isLateErrLSATimerRunning     : Flag - the LateErrLSATimer is current running                                     */
/* LateErrPackGrpTimerUsedMask  : Bit-Flag: LateErrTimer Bit(7:0) used by one PackGrp                               */





/* ============================================================================ */
/*                          structs consumer - DDB                              */
/* ============================================================================ */
typedef struct _EDDP_CRT_CONSUMER_MNGM_TYPE     EDDP_LOCAL_MEM_ATTR *       EDDP_CRT_CONSUMER_MNGM_PTR_TYPE;
typedef struct _EDDP_CRT_CONSUMER_MNGM_TYPE
{
    EDDP_CRT_CONS_ACW_INFO_PTR_TYPE         *pAcwPtrArray;
    LSA_UINT32                              ConsumerUsedCnt;
    LSA_UINT32                              ConsumerActCnt;
    EDDP_CRT_CHAIN_BASE_TYPE                ConsumerFreeList;
    EDDP_CRT_CONSUMER_PTR_TYPE              pConsEntry;
    LSA_UINT32                              ConsumerDgUsedCnt;
    EDDP_CRT_CHAIN_BASE_TYPE                ConsumerDgFreeList;
    EDDP_CRT_CONSUMER_CONTAINER_PTR_TYPE    pConsContainerEntry;
    EDDP_CRT_CONSUMER_DG_PTR_TYPE           pConsDgEntry;
    LSA_VOID_PTR_TYPE                       pHwBaseAcwApi;
    PNIP_ADPU_RCV_PTR_TYPE                  pHwBaseApduApi;
    LSA_VOID_PTR_TYPE                       pHwBaseAcwDgApi;
    PNIP_DG_STATUS_RCV_PTR_TYPE             pHwBaseDgStatusApi;

    LSA_UINT16                              RequestFinishConsumerID;
    LSA_UINT16                              ConsumerControlMode;

    EDDP_CRT_CHAIN_BASE_TYPE                ConsumerUsedList;
    EDDP_CRT_CHAIN_BASE_TYPE                ConsumerDgUsedList;

    EDDP_CRT_CONSUMER_RBASE_PTR_TYPE        pRBaseEntry;

    PNIP_ACW_RCV_RBASE_PTR_TYPE             pHwBaseRBaseApi;
    PNIP_CPM_TSB_PTR_TYPE                   pHwBaseTSBApi;
    PNIP_CPM_CSB_PTR_TYPE                   pHwBaseCSB0Api;  
    PNIP_CPM_CSB_PTR_TYPE                   pHwBaseCSB1Api;
    LSA_UINT32                              PNIPCSBControlReg;

    LSA_BOOL                                SBEventPending;
    LSA_UINT16                              CT_ConsumerID;
    EDDP_CRT_PACK_SM_TYPE                   CrtPackSm;
        
    EDDP_CRT_CONSUMER_RBASE_HANDLER_TYPE    RBaseHandler;
    EDDP_CRT_CONSUMER_ACW_HANDLER_TYPE      AcwHandler;
} EDDP_CRT_CONSUMER_MNGM_TYPE;
/* pAcwPtrArray         : pointer array of consumer/container ACWs for RBase linkage (used for common access)       */
/* ConsumerUsedCnt      : Number of used consumers per device                                                       */
/* ConsumerFreeList     : LinkList of free consumer                                                                 */
/* pConsEntry           : Array of all consumer -> 1 single consumer: pConsEntry[ConsumerId]                        */
/* ConsumerDgUsedCnt    : Number of used (DG) consumers per device                                                  */
/* ConsumerDgFreeList   : LinkList of free (DG) consumer                                                            */
/* pConsContainerEntry  : Array of all consumer -> 1 single consumer: pConsEntry[ConsumerId]                        */
/* pConsDgEntry         : Array of all (DG) consumer -> 1 single (DG) consumer: pConsDgEntry[DgId]                  */
/* pHwBaseAcwApi        : base pointer to array of Receive-ACWs (incl container ACW) in APICtrl-RAM                 */
/* pHwBaseApduApi       : base pointer to array of APDUs (incl container APDU) in APICtrl-RAM                       */
/* pHwBaseAcwDgApi      : base pointer to array of Receive-DG-ACWs in APICtrl-RAM                                   */
/* pHwBaseDgStatusApi   : base pointer to array of DG_Status in APICtrl-RAM                                         */
/* RequestFinishConsumerID: ConsumerID for activities after ConsumerRequest confirmation                            */
/* ConsumerControlMode  : Parameter "Mode" of ConsumerControl service                                               */

/* ConsumerUsedList     : LinkList of used consumer                                                                 */
/* ConsumerDgUsedCnt    : Number of used DG consumer per interface                                                  */
/* ConsumerDgUsedList   : LinkList of used DG consumer                                                              */
/* pRBaseEntry          : Array of all RBase -> 1 single RBase: pRBaseEntry[RBaseIndex]                             */
/* pHwBaseRBaseApi      : base address to RBase array in APICtrl-RAM                                                */
/* pHwBaseTSBApi        : base pointer to array of TimerScoreBoards in APICtrl-RAM                                  */
/* pHwBaseCSB0Api       : base pointer to array of ConsumerScoreBoards #0 in APICtrl-RAM                            */
/* pHwBaseCSB1Api       : base pointer to array of ConsumerScoreBoards #1 in APICtrl-RAM                            */
/* PNIPCSBControlReg    : Register address of PNIP_REG_CSB_CONTROL_IFA or PNIP_REG_CSB_CONTROL_IFB                  */
/* SBEventPending       : TRUE:  a Consumer Scoreboard check has to be called after finishing of                    */
/*                               current consumer request. Set if a SB Event occurs while a CPM asynchronis command */    
/*                               is running                                                                         */   
/*                        FALSE: Nothing to do.        .                                                            */    
/* CT_ConsumerID        : Consumer with CutThrough-Mode (only one Consumer with CT is allowed in CPM)               */
/* CrtPackSm            : Holds mgnt data for Inbound/Outbound and RcvDG statemachine for PRM-Commit                */
/* RBaseHandler         : virtualize RBase handler functions for different revisions                                */
/* AcwHandler           : virtualize ACW handler functions for different revisions                                  */





/* ============================================================================ */
/*                          structs provider - DDB                              */
/* ============================================================================ */
// Provider management
typedef struct _EDDP_CRT_PROVIDER_MNGM_TYPE     EDDP_LOCAL_MEM_ATTR *       EDDP_CRT_PROVIDER_MNGM_PTR_TYPE;
typedef struct _EDDP_CRT_PROVIDER_MNGM_TYPE
{
    LSA_UINT32                                              ProviderUsedCnt;
    EDDP_CRT_CHAIN_BASE_TYPE                                ProviderFreeList;
    EDDP_CRT_PROVIDER_PTR_TYPE                              pProvEntry;

    LSA_UINT32                                              AcwUsedCnt;
    EDDP_CRT_CHAIN_BASE_TYPE                                AcwFreeList;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE                          pAcwEntry;

    LSA_UINT32                                              AcwDGUsedCnt;
    EDDP_CRT_CHAIN_BASE_TYPE                                AcwDGFreeList;
    EDDP_CRT_PROVIDER_ACW_DG_PTR_TYPE                       pAcwDGEntry;

    EDDP_CRT_CHAIN_BASE_TYPE                                PackGroupFreeList;
    EDDP_CRT_PROVIDER_PACK_GROUP_PTR_TYPE                   pProvPackGroupEntry;

    LSA_VOID_PTR_TYPE                                       pHwBaseAcwApi;
    PNIP_ACWDG_SND_PTR_TYPE                                 pHwBaseAcwDGApi;

    LSA_UINT32                                              AcwActCnt;
    LSA_UINT32                                              AcwActCntProvCntrlRTC12;
    LSA_UINT32                                              AcwActCntProvCntrlRTC3;
    LSA_UINT32                                              AcwActCntProvCntrl;
    EDDP_CRT_CHAIN_BASE_TYPE                                ProviderUsedList;
    EDDP_CRT_CHAIN_BASE_TYPE                                AcwUsedList;
    EDDP_CRT_CHAIN_BASE_TYPE                                AcwDGUsedList;
    LSA_UINT32                                              PackGroupUsedCnt;
    EDDP_CRT_CHAIN_BASE_TYPE                                PackGroupUsedList;

    LSA_UINT32                                              TrogUsedCnt;
    EDDP_CRT_CHAIN_BASE_TYPE                                TrogUsedList;
    EDDP_CRT_CHAIN_BASE_TYPE                                TrogFreeList;
    EDDP_CRT_PROVIDER_TROG_PTR_TYPE                         pTrogEntry;

    EDDP_CRT_PROVIDER_GROUP_PTR_TYPE                        pProvGroupEntry;
    EDDP_CRT_PROVIDER_ASGROUP_PTR_TYPE                      pASGroupEntry;
    EDDP_CRT_PROVIDER_TBASE_PTR_TYPE                        pTBaseEntry;

    EDDP_CRT_CHAIN_BASE_TYPE                                DSUsedList;
    EDDP_CRT_CHAIN_BASE_TYPE                                DSFreeList;
    EDDP_CRT_PROVIDER_DS_PTR_TYPE                           pDSEntry;

    LSA_UINT32                                              ACW_Treductionselect;
    EDDP_CRT_PROVIDER_TREDUCTIONSELECT_TYPE                 Treductionselect[EDDP_CRT_REDUCTION_NR_MAX];

    PNIP_ACW_SND_TBASE_PTR_TYPE                             pHwBaseTBaseApi;
    PNIP_ACW_SND_TROG_PTR_TYPE                              pHwBaseTrogApi;
    #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
    PNIP_R1_ACW_SND_DS_PTR_TYPE                             pHwBaseDSApiPnipR1;
    #endif
    #if defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
    PNIP_R2_ACW_SND_DS_PTR_TYPE                             pHwBaseDSApiPnipR2; // used since PN-IP Rev2
    #endif

    /* Timer */
    LSA_UINT32	                                            PpmHolNotEmptyTimer;
    LSA_UINT32	                                            PpmNotFinishedTimer;

    /* Provider State Machines */
    EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_SM_TYPE       PackGroupActivateAllSm;
    EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_SM_TYPE      PackGroupPassivateAllSm;
    EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_SM_TYPE               AcwDGActivateSm[EDDP_CRT_PROVIDER_ACW_DG_STATE_MACHINE_DEPTH];
    EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_SM_TYPE              AcwDGPassivateSm[EDDP_CRT_PROVIDER_ACW_DG_STATE_MACHINE_DEPTH];
    
    EDDP_CRT_PROVIDER_ACW_HANDLER_TYPE                      AcwHandler;
} EDDP_CRT_PROVIDER_MNGM_TYPE;
/* ProviderUsedCnt          : Number of used providers per device                                                                   */
/* ProviderFreeList         : LinkList of free provider                                                                             */
/* pProvEntry               : Array of all provider         -> 1 single provider        : pProvEntry[ProviderId]                    */
/* AcwUsedCnt               : Number of used ACWs per device                                                                        */
/* AcwFreeList              : LinkList of free ACWs                                                                                 */
/* pAcwEntry                : Array of all ACWs             -> 1 single ACW             : pAcwEntry[AcwId]                          */
/* AcwDGUsedCnt             : Number of used ACW_DGs per device                                                                     */
/* AcwDGFreeList            : LinkList of free ACW_DGs                                                                              */
/* pAcwDGEntry              : Array of all ACW_DGs          -> 1 single ACW_DG          : pAcwDGEntry[AcwDGId]                      */
/* PackGroupFreeList        : LinkList of free pack groups                                                                          */
/* pProvPackGroupEntry      : Array of all pack groups      -> 1 single pack group      : pProvPackGroupEntry[PackGrId]             */
/* pHwBaseAcwApi            : base pointer to array of Send-ACWs in APICtrl-RAM                                                     */
/* pHwBaseAcwDGApi          : base pointer to array of Send-ACW_DGs in APICtrl-RAM                                                  */

/* AcwActCnt                : Number of all active Provider-ACWs per interface                                                      */
/* AcwActCntProvCntrlRTC12  : Number of active RTC1/2-Provider-ACWs per interface (activated by user with Provider-Control)        */
/* AcwActCntProvCntrlRTC3   : Number of active RTC3-Provider-ACWs per interface (activated by user with Provider-Control)           */
/* AcwActCntProvCntrl       : Number of all active Provider-ACWs per interface (activated by user with Provider-Control)            */
/* ProviderUsedList         : LinkList of used provider                                                                             */
/* AcwUsedList              : LinkList of used ACWs in APICtrl-RAM                                                                  */
/* AcwDGUsedList            : LinkList of used ACW_DGs in APICtrl-RAM                                                               */
/* PackGroupUsedCnt         : Number of used pack groups                                                                            */
/* PackGroupUsedList        : LinkList of used pack groups                                                                          */
/* TrogUsedCnt              : Number of used Trog entries per interface                                                             */
/* TrogUsedList             : LinkList of used Trog elements                                                                        */
/* TrogFreeList             : LinkList of free Trog elements                                                                        */
/* pTrogEntry               : Array of all Trogs            -> 1 single Trog            : pTrogEntry[TrogId]                        */
/* pProvGroupEntry          : Array of all provider groups  -> 1 single provider group  : pProvGroupEntry[GroupId]                  */
/* pASGroupEntry            : Array of all AutoStop provider groups                     : pASGroupEntry[ConsumerId]                 */
/* pTBaseEntry              : Array of all TBase elements   -> 1 single TBase           : pTBaseEntry[Reduction + Phase]            */
/* DSUsedList               : LinkList of used DataStatus elements                                                                  */
/* DSFreeList               : LinkList of free DataStatus elements                                                                  */
/* pDSEntry                 : Array of all DataStatus       -> 1 single DataStatus      : pDSEntry[DataStatusId]                    */
/* ACW_Treductionselect     : is identical with PNIP register "ACW_Treductionselect_IFA/B"                                          */
/* Treductionselect         : Array of all reductions       -> 1 single Treductionselect                                            */
/* pHwBaseTBaseApi          : base address to TBase array in APICtrl-RAM                                                            */
/* pHwBaseTrogApi           : base address to TROG array in APICtrl-RAM                                                             */
/* pHwBaseDSApiPnipR1       : base address to DataStatus array in APICtrl-RAM                                                       */
/* pHwBaseDSApiPnipR2       : base address to DataStatus array in APICtrl-RAM                                                       */

/* PpmHolNotEmptyTimer      : Timer for interrupt "Ppm_HOL_not_empty"                                                               */
#define EDDP_CRT_PPM_HOL_NOT_EMPTY_TIME     5   // in seconds
/* PpmNotFinishedTimer      : Timer for interrupt "Ppm_not_finished"                                                                */
#define EDDP_CRT_PPM_NOT_FINISHED_TIME      5   // in seconds
/* PackGroupActivateAllSm   : state machine for activate of ALL pack groups in "ProviderPackGroupList"                              */
/* PackGroupPassivateAllSm  : state machine for passivate of ALL pack groups in "ProviderPackGroupList"                             */
/* AcwDGActivateSm          : state machine for activate of one ACW_DG                                                              */
/* AcwDGPassivateSm         : state machine for passivate of one ACW_DG                                                             */
/* AcwHandler               : virtualize ACW handler functions for different revisions                                              */





/* ============================================================================ */
/*              configuration parameter for CRT - DDB                           */
/* ============================================================================ */
typedef struct _EDDP_CRT_CFG_PARAM_TYPE
{
    LSA_UINT16              ProviderMaxNumber;
    LSA_UINT16              ProviderAcwMaxNumber;
    LSA_UINT16              ProviderAcwDGMaxNumber;
    LSA_UINT16              ProviderPackGroupMaxNumber;

    LSA_UINT16              ConsumerMaxNumber;
    LSA_UINT16              ConsumerContainerMaxNumber;
    LSA_UINT16              ConsumerDGMaxNumber;
    LSA_UINT16              ConsumerRBaseInvalidIndicator;

    LSA_UINT16              ProviderGroupMaxNumber;
    LSA_UINT16              ProviderTrogMaxNumber;
    LSA_UINT16              ProviderDataStatusMaxNumber;

    LSA_UINT8               ApplTimerMode;
    LSA_UINT16              ApplTimerReductionRatio;
    LSA_UINT32              TransferEndCorrectionValue;
    LSA_BOOL                isTransferEndCorrectionPos;
} EDDP_CRT_CFG_PARAM_TYPE;
/* ProviderMaxNumber                : Maximum number of Provider in EDDP-CRT for one device                         */
/* ProviderAcwMaxNumber             : Maximum number of Provider-ACWs in EDDP-CRT for one device                    */
/* ProviderAcwDGMaxNumber           : Maximum number of Provider-ACW-DGs in EDDP-CRT for one device                 */
/* ProviderPackGroupMaxNumber       : Maximum number of Provider-Pack-Groups in EDDP-CRT for one device             */
/* ConsumerMaxNumber                : Maximum number of Consumer in EDDP-CRT for one device                         */
/* ConsumerContainerMaxNumber       : Maximum number of ACW-Container used for DGs for one device                   */
/* ConsumerDGMaxNumber              : Maximum number of Consumer-DGs in EDDP-CRT for one device                     */
/* ConsumerRBaseInvalidIndicator    : RBase invalid indicator, means RBase entry not in use                         */
/* Note: ConsumerAcwMaxNumber = ConsumerMaxNumber + ConsumerContainerMaxNumber                                      */





/* ============================================================================ */
/*            HW configuration parameter for CRT - DDB                          */
/*                  for alloc in APICtrl-RAM                                    */
/* ============================================================================ */
typedef struct _EDDP_CRT_CFG_HW_ALLOC_PARAM_TYPE
{
    /* PPM */
    LSA_UINT16          MaxHWPpmAcw;
    LSA_UINT16          MaxHWPpmAcwDG;
    LSA_UINT16          MaxHWPpmTrog;
    LSA_UINT16          MaxHWPpmDataStatus;
    LSA_UINT16          HWPpmAcwSize;
    /* CPM */
    LSA_UINT16          MaxHWCpmAcw;
    LSA_UINT16          MaxHWCpmAcwContainer;
    LSA_UINT16          MaxHWCpmAllAcw;
    LSA_UINT16          MaxHWCpmAcwDG;
    LSA_UINT16          MaxHWCpmRBaseEntries;
    LSA_UINT16          HWCpmAcwSize;
    LSA_UINT16          HWCpmAcwDgSize;
} EDDP_CRT_CFG_HW_ALLOC_PARAM_TYPE;
/* MaxHWPpmAcw          : maximum supported PPM-ACWs (reserved in HW-RAM)                                               */
/* MaxHWPpmAcwDG        : maximum supported PPM-ACW_DGs (reserved in HW-RAM)                                            */
/* MaxHWPpmTrog         : maximum supported Trog-Entries (reserved in HW-RAM)                                           */
/* MaxHWPpmDataStatus   : maximum supported DataStatus-Entries (reserved in HW-RAM)                                     */
/* HWPpmAcwSize         : size of ACW within API-CTRL-RAM for specific hardware revision                                */
/* MaxHWCpmAcw          : maximum supported CPM-ACWs (reserved in HW-RAM)                                               */
/* MaxHWCpmAcwContainer : maximum supported ACW Container for DGs (reserved in HW-RAM)                                  */
/* MaxHWCpmAllAcw       : MaxHWCpmAcw + MaxHWCpmAcwContainer                                                            */
/* MaxHWCpmAcwDG        : maximum supported CPM-ACW_DGs (reserved in HW-RAM)                                            */
/* HWCpmAcwSize         : size of ACW within API-CTRL-RAM for specific hardware revision                                */
/* HWCpmAcwDgSize       : size of ACW within API-CTRL-RAM for specific hardware revision                                */





/* ============================================================================ */
/*                          main struct for EDDP-CRT                            */
/*          DDB-CRT Device management structure for one Device                  */
/* ============================================================================ */
typedef struct _EDDP_DDB_CRT_TYPE       EDDP_LOCAL_MEM_ATTR *       EDDP_DDB_CRT_PTR_TYPE;
typedef struct _EDDP_DDB_CRT_TYPE
{
    EDDP_CRT_PROVIDER_MNGM_TYPE                     Prov;
    EDDP_CRT_CONSUMER_MNGM_TYPE                     Cons;
    EDDP_CRT_INBOUND_MNGM_TYPE                      Inbound;
    EDDP_CRT_CFG_PARAM_TYPE                         CfgParam;
    EDDP_CRT_CFG_HW_ALLOC_PARAM_TYPE                CfgHwAllocParamApi;

    LSA_UINT32                                      BitFlag;
    EDDP_CRT_CONSUMER_SB_MNGM_TYPE                  ConsSB;
    EDDP_CRT_INDPROVIDE_TYPE                        IndProvide;
    EDDP_CRT_CMD_PARAM_TYPE                         CmdParam;

    #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
    EDDP_CRT_WRKAR_PACKGRP_DISABLE_MNGM_TYPE        WrkarPackGrpDisable;
    #endif

    EDD_UPPER_MEM_U8_PTR_TYPE                       pIOBaseAdrPerif;             // ERTEC200P only
    EDD_UPPER_MEM_U8_PTR_TYPE                       pIOBaseAdrExtRam;            // ERTEC200P only
    EDD_UPPER_MEM_U8_PTR_TYPE                       pCPMBaseAddrFastMem;         // HERA only
    EDD_UPPER_MEM_U8_PTR_TYPE                       pCPMBaseAddrSlowMem;         // HERA only 
    LSA_UINT32                                      CPMBaseAddrFastMemPNIP;      // HERA only
    LSA_UINT32                                      CPMBaseAddrSlowMemPNIP;      // HERA only 
    LSA_UINT32                                      CPM_NILOffsetFastMem;        // HERA only
    LSA_UINT32                                      CPM_NILOffsetSlowMem;        // HERA only 
    EDD_UPPER_MEM_U8_PTR_TYPE                       pPPMBaseAddrFastMem;         // HERA only
    EDD_UPPER_MEM_U8_PTR_TYPE                       pPPMBaseAddrSlowMem;         // HERA only
    LSA_UINT32                                      PPMBaseAddrFastMemPNIP;      // HERA only
    LSA_UINT32                                      PPMBaseAddrSlowMemPNIP;      // HERA only 
    LSA_UINT32                                      PPM_Lifetime64FastMem;       // HERA only
    LSA_UINT32                                      PPM_Lifetime64SlowMem;       // HERA only

    EDDP_CRT_PACK_ACTIVATE_SM_TYPE                  PackActivateSm;
    EDDP_CRT_PACK_PASSIVATE_SM_TYPE                 PackPassivateSm;

    EDDP_CRT_APPL_TIMER_SM_TYPE                     ApplTimerIfSetupSm;
} EDDP_DDB_CRT_TYPE;
/* Prov                 : Provider management                                                                           */
/* Cons                 : Consumer management                                                                           */
/* Inbound              : Inbound management                                                                            */
/* CfgParam             : initialization/configuration parameter for CRT                                                */
/* CfgHwAllocParamApi   : hardware configuration parameter for CRT - for alloc in APICtrl-RAM                           */





/* ============================================================================ */
/*                          main struct for EDDP-CRT                            */
/*                    HDB-CRT Handle management structure                       */
/* ============================================================================ */
typedef struct _EDDP_HDB_CRT_TYPE       EDDP_LOCAL_MEM_ATTR *       EDDP_LOCAL_HDB_CRT_PTR_TYPE;
typedef struct _EDDP_HDB_CRT_TYPE
{
    LSA_BOOL            UseCRT;
    LSA_UINT32          RunningRQBCount;
} EDDP_HDB_CRT_TYPE;
/* UseCRT               : LSA_TRUE CRT-Services used by channel                                     */
/* RunningRQBCount      : count of current running RQBs in GlbCrt queue of this CRT channel         */





/* ============================================================================ */
/*                    internal prototyping                                      */
/* ============================================================================ */

/* ---------------------------------------------------------------------------- */
/* common                                                                       */
/* ---------------------------------------------------------------------------- */
LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetPnipAddrShift(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_COMMON_MEM_PTR_TYPE     pAddrHost,
    LSA_UINT16                  Shift);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTIndicationProvide(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTInitDevOpen(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_UPPER_DPB_PTR_TYPE     pDPB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTInitCfgParam(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_UPPER_DPB_PTR_TYPE     pDPB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTInitDevOpen(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DPB_PTR_TYPE     pDPB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTAllocLocalMemDDB(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTFreeLocalMemDDB(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTInitPnipRegDevSetup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_UPPER_DSB_PTR_TYPE     pDSB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTAllocLocalMemHDB(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTFreeLocalMemHDB(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTIndProvideFinishAll(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_RSP                     ResponseFinish);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTIndProvideFinishSingle(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTFillIndication(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  UserID,
    LSA_UINT32                  Event,
    LSA_UINT32                  EventStatus,
    LSA_UINT16                  CycleCounter,
    LSA_UINT32                  DebugInfo);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTDeleteHwApiDDB(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTAllocTimer(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTFreeTimer(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTInitStateMachine(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

#ifdef EDDP_CFG_DETAIL_ERROR_INFO
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTFillDebugInfoError(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    LSA_UINT32                  ModuleId,
    LSA_UINT32                  LineNr);
#endif

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTFillDebugInfoMissIndication(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDD_UPPER_CSRT_IND_PROVIDE_PTR_TYPE     pIndParam);

/* ---------------------------------------------------------------------------- */
/* Consumer                                                                     */
/* ***** eddp_crt_cons.c                                                        */
/* ---------------------------------------------------------------------------- */
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTInitConsumerDevOpen(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_UPPER_DPB_PTR_TYPE     pDPB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTInitConsumerDevSetup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_UPPER_DSB_PTR_TYPE     pDSB);

/* ---------------------------------------------------------------------------- */
/* Consumer                                                                     */
/* ***** eddp_crt_cons_acw.c                                                    */
/* ---------------------------------------------------------------------------- */

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsForceBackupDataStatusHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_CRT_CONSUMER_PTR_TYPE  pConsumer);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsDeInitConsAcwHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ConsumerID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsDeInitConsAcwDgHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  DgID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsDeInitContainerAcwHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID);

#if defined (EDDP_CFG_HW_ERTEC200P_SUPPORT)
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsUpdateSrcAddrCsaInConsAcwHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ConsumerID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsInitConsAcwHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ConsumerID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsBasicInitConsAcwDgHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  DgID,
    LSA_UINT32                  NextOffs_0,
    LSA_UINT32                  NextOffs_1);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsFinalInitConsAcwDgHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer,
    EDDP_CRT_CONSUMER_DG_PTR_TYPE   pDgEntry);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsInitContainerAcwHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsUpdateAcwNextOffsetAcwHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsSetupLegacyStartupAcwHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer);
#endif

/* ---------------------------------------------------------------------------- */
/* Consumer                                                                     */
/* ***** eddp_crt_cons.c                                                        */
/* ---------------------------------------------------------------------------- */

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerSetCtRwb(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerClrCtRwb(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ConsumerID);

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_CRTCountActiveDP2Consumer(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_CRTCountAllActiveConsumer(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerAdd(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerRemove(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerControl(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerAddToCRTMngm(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDD_UPPER_CSRT_CONSUMER_ADD_PTR_TYPE    pRqbParam);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerRemoveFromCRTMngm(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ConsumerID,
    LSA_BOOL                    RemoveFromeRBaseList);

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerGetFreeConsumer(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerReleaseConsumer(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ConsumerID);

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerGetFreeDgItem(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerAppendDgItem(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_CRT_CONSUMER_CONTAINER_PTR_TYPE    pContainer,
    LSA_UINT16                              DgID);

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerRemoveFirstDgItem(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_CRT_CONSUMER_CONTAINER_PTR_TYPE    pContainer);

#if defined(EDDP_CFG_HW_ERTEC200P_SUPPORT)
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerAddAcwToRBaseSWList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID);
#endif

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerRemoveAcwFromRBaseSWList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID);

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetConsumerState(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT16                  ConsumerID);

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetShiftedTSBOffset(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT16                  ConsumerID);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTConsIsProvAutoStopEnabled(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT16                  ConsumerID);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTConsExistFrameID(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT16                  FrameID,
    LSA_BOOL                    IgnoreDfp);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerFirstActivateControlReq(
    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB,
    EDD_UPPER_CSRT_CONSUMER_CONTROL_PTR_TYPE    pRqbParam);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerDoAsynAcwInCmd(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID,
    EDDP_CMD_CBF_PTR_TYPE       pCbPtr);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerDoAsynAcwOutCmd(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID,
    EDDP_CMD_CBF_PTR_TYPE       pCbPtr);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerDoAsynAcwOnOffCmd(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID,
    LSA_BOOL                    SetOn,
    EDDP_CMD_CBF_PTR_TYPE       pCbPtr);

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerGetConsDgEntry(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer,
    LSA_UINT8                       SFPosition);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerConnectToConsDgEntry(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer,
    LSA_UINT16                      DgIdx);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerDisconnectFromConsDgEntry(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer);

LSA_VOID  EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerEventForProvAutoStop(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer,
    LSA_UINT16                      Mode,
    LSA_BOOL                        bPendingProvAutoStopOn);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerDoControlReq(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_UPPER_RQB_PTR_TYPE      pRQB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTScoreboardSetupEntry(
	EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
	EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerCheckIndication(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTSetConsumerToUnknown(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_CRT_CONSUMER_PTR_TYPE  pConsumer,
    LSA_BOOL                    WithSBC);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTScoreboardHandler(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTTriggerConsumerControlFinish(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsMrtTriggerAppearDiagAllActiveMRPDConsumer(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_BOOL                    isPrmCommit);

/* ---------------------------------------------------------------------------- */
/* Provider                                                                     */
/* ***** eddp_crt_prov.c                                                        */
/* ***** eddp_crt_prov_acw.c                                                    */
/* ***** eddp_crt_prov_dfp.c                                                    */
/* ---------------------------------------------------------------------------- */
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTInitProviderDevOpen(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_UPPER_DPB_PTR_TYPE     pDPB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTInitProviderDevSetup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_UPPER_DSB_PTR_TYPE     pDSB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderAdd(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderRemove(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderControl(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderSetState(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderChangePhase(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderAddToCRTMngm(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
	EDD_UPPER_CSRT_PROVIDER_ADD_PTR_TYPE    pRqbParam,
	LSA_UINT8                               ProviderAcwPhase);

#if defined (EDDP_CFG_HW_ERTEC200P_SUPPORT)
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDGInitHwApiFromProvider(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID,
    LSA_UINT16                  AcwDGID);
#endif

#if defined (EDDP_CFG_HW_ERTEC200P_SUPPORT)
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDGInitHwApiFromPrmCommit(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwDGID);
#endif

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderRemoveFromCRTMngm(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderAddToGroupList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderRemoveFromGroupList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderAddToASGroupList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID,
    LSA_UINT16                  AutoStopConsumerID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderRemoveFromASGroupList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID);

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderAddToUsedList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwAddToUsedList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwAddToProvider(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID,
    LSA_UINT16                  AcwID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwRemoveFromProvider(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID,
    LSA_UINT16                  AcwID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwRemoveFromUsedList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderRemoveFromUsedList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwPhaseListUpdate_O_G(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AddRemove,
    LSA_UINT16                  TrogID,
    LSA_UINT16                  AcwID);

#define EDDP_CRT_ADD            0x1
#define EDDP_CRT_REMOVE         0x2
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwPhaseListUpdate_R(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AddRemove,
    LSA_UINT16                  TrogID,
    LSA_UINT16                  AcwID,
    LSA_BOOL                    isDFP);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwAddToTBase(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID,
    LSA_BOOL                    isDFP);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwRemoveFromTBase(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID,
    LSA_BOOL                    isDFP);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwIsFirstAcwAddedToTBase(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  TBaseIndex);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwIsLastAcwRemovedFromTBase(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  TBaseIndex);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwSetActiveRTFrame(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID,
    LSA_UINT16                  AcwID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwSetPassiveRTFrame(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID,
    LSA_UINT16                  AcwID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwSetActivePackFrame(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwSetPassivePackFrame(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID,
    LSA_BOOL                    DeleteAddParam);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDGSetActive(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwDGID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDGSetPassive(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwDGID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDGDeleteActiveBitHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwDGID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwSetActiveBitHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDeleteActiveBitHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID);

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTTrogEntryAddToUsedList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTTrogEntryRemoveFromUsedList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  TrogUsedId);

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTDataStatusAddToUsedList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTDataStatusRemoveFromUsedList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  DataStatusUsedId);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderChangeDataStatus(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID,
    LSA_UINT8                   LocalDataStatus,
    LSA_UINT8                   LocalMask);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderChangeGroupDataStatus(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  GroupId,
    LSA_UINT8                   GroupDataStatus,
    LSA_UINT8                   GroupMask);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwROGBitAddHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwROGBitRemoveHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderInitTBaseElementHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  TBaseIndex);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDeleteHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDGDeleteHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwDGId);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderDeleteTBaseElementHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  TBaseIndex);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderDeleteTrogHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  TrogId);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderAutoStopControl(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ConsumerID,
    LSA_BOOL                    IsEnable);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderDeleteDataStatusHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  DataStatusId);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderIOParamUpdateForActivate(
    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB,
    LSA_UINT16                                  ProviderID,
    EDD_UPPER_CSRT_PROVIDER_CONTROL_PTR_TYPE    pRqbParam);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwSetDstMACFromPDIRData(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID,
    LSA_UINT16                  FastForwardingMulticastMACAdd);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwCheckAndSetDstMACFromProviderControlRQB(
    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB,
    LSA_UINT16                                  ProviderID,
    EDD_UPPER_CSRT_PROVIDER_CONTROL_PTR_TYPE    pRqbParam);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwChainSendOrderToProvider(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID,
    LSA_UINT16                  AcwID);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTProvDfpPackGroupActivateFromPDIRSubframeData(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_BOOL                    InActiveRecord_A);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvDfpPackGroupAddToCRTMngmFromPDIRSubframeData(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  SubframeBlockIdx,
    LSA_BOOL                    isRedundant1,
    LSA_BOOL                    InActiveRecord_A);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvDfpAcwAcwDGInitAndChainHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID,
    LSA_BOOL                    isRedundant1);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvDfpPackGroupRemoveFromCRTMngm(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PackGrId);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTProvDfpPackGroupPassivate(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDGAddToUsedList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDGRemoveFromUsedList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwDGID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDGAddToAcw(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID,
    LSA_UINT16                  AcwDGID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDGRemoveFromAcw(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwDGID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDGAddToAcwRed1(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwIDRed,
    LSA_UINT16                  AcwIDRed1,
    LSA_UINT16                  AcwDGID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDGRemoveFromAcwRed1(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwIDRed1,
    LSA_UINT16                  AcwDGID);

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTProvDfpPackGroupAddToUsedList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvDfpPackGroupRemoveFromUsedList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PackGrId);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvDfpAcwDGControlDoneCbf(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT16                  ProviderId,
	EDD_RSP                     Response);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvDfpAcwDGRemoveDoneCbf(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderId,
    EDD_RSP                     Response);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvDfpDisconnectFromPackGroup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID);

#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTProvDfpGetAcwIDFromInBoundPackGroupFrameID(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  FrameID,
    LSA_BOOL                    isFIDRedundantSet);
#endif

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwAddToTreductionselect(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwRemoveFromTreductionselect(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID);

#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwSetupDrmAndDestination(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID,
    LSA_BOOL                    isValid);
#endif

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwAcwDGDeleteHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID);

#if defined (EDDP_CFG_HW_ERTEC200P_SUPPORT)
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwInitHwApiRTFrame(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID,
    LSA_UINT16                  AcwID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwInitHwApiPackFrame(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID,
    LSA_BOOL                    InActiveRecord_A);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwUpdateAcwNextOffsetAcwHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwWriteSBOffset(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID,
    LSA_UINT16                  ACW_SB_offset);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDGUpdateAcwDGNextOffsetAcwHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID,
    LSA_UINT16                  AcwDGID);
#endif

/* ---------------------------------------------------------------------------- */
/* CRT-CMD inteface                                                             */
/* ***** eddp_crt_cmd.c                                                         */
/* ---------------------------------------------------------------------------- */
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCmdRequestCpm(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  CrtCommand,
    LSA_UINT16                  AcwId,
    EDDP_CMD_CBF_PTR_TYPE       pCbPtr);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTCmdDoneCbfCpm(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_CMD_PARAM_PTR_TYPE     pCmdParam,
    EDD_RSP                     ResponseCmd);

#define EDDP_CRT_CMD_SYNCHRON       0x1
#define EDDP_CRT_CMD_ASYNCHRON      0x2
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCmdRequestPpm(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  CrtCommand,
    LSA_UINT16                  SynAsyn,
    EDDP_CMD_PARAM_PTR_TYPE     pCmdParam);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTCmdDoneCbfPpm(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_CMD_PARAM_PTR_TYPE     pCmdParam,
    EDD_RSP                     ResponseCmd);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTCmdDoneCbfPpmPackGroup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_CMD_PARAM_PTR_TYPE     pCmdParam,
    EDD_RSP                     ResponseCmd);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTCmdDoneCbfPpmAcwDG(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_CMD_PARAM_PTR_TYPE     pCmdParam,
    EDD_RSP                     ResponseCmd);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCmdRequestApplTimer(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  CrtCommand,
    LSA_UINT16                  SynAsyn);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTCmdDoneCbfApplTimer(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_CMD_PARAM_PTR_TYPE     pCmdParam,
    EDD_RSP                     ResponseCmd);

EDDP_CMD_PARAM_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_CRTCmdGetInternalCmdParamElement(
    EDDP_LOCAL_DDB_PTR_TYPE           pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTCmdReleaseInternalCmdParamElement(
    EDDP_LOCAL_DDB_PTR_TYPE           pDDB);


LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTCmdTriggerCrtQueue(
    EDDP_LOCAL_DDB_PTR_TYPE           pDDB);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTCmdIsSequenceInWork(
    EDDP_LOCAL_DDB_PTR_TYPE           pDDB);

/* ---------------------------------------------------------------------------- */
/* CRT info                                                                     */
/* ***** eddp_crt_info.c                                                        */
/* ---------------------------------------------------------------------------- */
LSA_UINT8 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetNewDataStatus(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT8                   CurrentDataStatus,
    LSA_UINT8                   DataStatusMask,
    LSA_UINT8                   NewDataStatus);

LSA_UINT8 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetProvAPDUDataStatus(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTGetAPDUStatus(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTGetConsAPDUStatus(
    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB,
    EDD_UPPER_CSRT_GET_APDU_STATUS_PTR_TYPE     pGetApduStatus);

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetConsCycleCounterForIndication(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ConsumerID);

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetHWCycleCounter(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT8                   RTClass);

LSA_UINT8 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetProviderAcwPhase(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  Properties,
    LSA_UINT16                  FrameID);

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetTBaseIndex(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ReductionUser,
    LSA_UINT16                  PhaseUser);

LSA_UINT8 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetReductionNrFromReductionRatio(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ReductionRatio);

#if defined(EDDP_CFG_HW_ERTEC200P_SUPPORT)
LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetRBaseIndex(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  FrameID);
#endif

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetProviderMaxNumber(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetConsumerMaxNumber(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetProviderAcwDGMaxNumber(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetConsumerAcwDGMaxNumber(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_UINT8 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetApplTimerMode(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetTransferEndCorrectionValue(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTIsTransferEndCorrectionPos(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetFSOLeadtimeOrFetchtimeDelta(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

#if defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetConsMrtRrReloadValue(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);
#endif

/* ---------------------------------------------------------------------------- */
/* CRT check                                                                    */
/* ***** eddp_crt_check.c                                                       */
/* ---------------------------------------------------------------------------- */
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckCloseChannel(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckProvAutoStopReference(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ConsumerID);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckProviderAddParam(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
	EDD_UPPER_CSRT_PROVIDER_ADD_PTR_TYPE    pRqbParam,
	LSA_UINT8                               ProviderAcwPhase);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckProviderRemoveParam(
    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB,
	EDD_UPPER_CSRT_PROVIDER_REMOVE_PTR_TYPE     pRqbParam);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckProviderControlParam(
    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB,
	EDD_UPPER_CSRT_PROVIDER_CONTROL_PTR_TYPE    pRqbParam);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckProviderSetStateParam(
    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB,
	EDD_UPPER_CSRT_PROVIDER_SET_STATE_PTR_TYPE  pRqbParam,
	EDD_SERVICE                                 Service);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckConsumerAddParam(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
	EDD_UPPER_CSRT_CONSUMER_ADD_PTR_TYPE    pRqbParam);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckProviderChangePhaseParam(
    EDDP_LOCAL_DDB_PTR_TYPE                         pDDB,
	EDD_UPPER_CSRT_PROVIDER_CHANGE_PHASE_PTR_TYPE   pRqbParam);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckConsumerControlParam(
    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB,
	EDD_UPPER_CSRT_CONSUMER_CONTROL_PTR_TYPE    pRqbParam,
    LSA_UINT16                                  *pDgIdx);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckConsumerRemoveParam(
    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB,
	EDD_UPPER_CSRT_CONSUMER_REMOVE_PTR_TYPE     pRqbParam);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckGetAPDUStatusParam(
    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB,
	EDD_UPPER_CSRT_GET_APDU_STATUS_PTR_TYPE     pRqbParam,
    EDD_SERVICE                                 Service);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckAndGetProviderPortParam(
    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB,
    EDD_UPPER_CSRT_PROVIDER_CONTROL_PTR_TYPE    pRqbParam);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckIsActiveRTC3ProviderConsumerPresent(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckIsActiveRTC12ProviderPresent(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckIsActiveProviderConsumerPresent(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckRangeOfCycleReductionRatio(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT8                   RTClass,
    LSA_UINT16                  Properties,
    LSA_UINT16                  CycleReductionRatio,
    LSA_BOOL                    UndefAllowed);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckIsProviderConsumerPresent(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckIsBCAddress(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_MAC_ADR_TYPE            *pMac);

LSA_UINT8 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetRTClass(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  FrameID);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckProviderExistFrameID(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  FrameID);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckIsCycleBaseFactorEven(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT16                  CycleBaseFactor);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTProvDfpConnectToPackGroup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID,
    LSA_UINT8                   SFPosition);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckIsCycleReductionRatioPowerOfTwo(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT16                  CycleReductionRatio);

/* ---------------------------------------------------------------------------- */
/* State machine                                                                */
/* ***** eddp_crt_sm.c                                                          */
/* ---------------------------------------------------------------------------- */
EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_SM_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_CRTGetProvAcwDGActivateStateMachine(
    EDDP_LOCAL_DDB_PTR_TYPE       pDDB,
    LSA_UINT16                    ProviderId,
    EDDP_CRT_PROV_DG_CBF_PTR_TYPE Cbf,
    EDD_UPPER_RQB_PTR_TYPE        pRQB);

EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_SM_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_CRTGetProvAcwDGPassivateStateMachine(
    EDDP_LOCAL_DDB_PTR_TYPE       pDDB,
    LSA_UINT16                    ProviderId,
    EDDP_CRT_PROV_DG_CBF_PTR_TYPE Cbf,
    EDD_UPPER_RQB_PTR_TYPE        pRQB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTReleaseProvAcwDGActivateStateMachine(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  Index);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTReleaseProvAcwDGPassivateStateMachine(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  Index);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderPackGroupActivateAllStateMachine(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDD_RSP                     ResponseSm);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderPackGroupActivateAllStateMachineCbf(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDD_RSP                     Response);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTActivatePackStateMachine(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDD_RSP                     ResponseSm);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTActivatePackStateMachineCbf(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDD_RSP                     Response);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTPassivatePackStateMachine(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDD_RSP                     ResponseSm);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTPassivatePackStateMachineCbf(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDD_RSP                     Response);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderPackGroupPassivateAllStateMachine(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDD_RSP                     ResponseSm);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderPackGroupPassivateAllStateMachineCbf(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDD_RSP                     Response);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderAcwDGActivateStateMachine(
    EDDP_LOCAL_DDB_PTR_TYPE                       pDDB,
    EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_SM_PTR_TYPE pStateMachine,
    EDD_RSP                                       ResponseSm);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderAcwDGActivateStateMachineCbf(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_CMD_PARAM_PTR_TYPE     pCmdParam,
	EDD_RSP                     Response);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderAcwDGPassivateStateMachine(
    EDDP_LOCAL_DDB_PTR_TYPE                        pDDB,
    EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_SM_PTR_TYPE pStateMachine,
    EDD_RSP                                        ResponseSm);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderAcwDGPassivateStateMachineCbf(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_CMD_PARAM_PTR_TYPE     pCmdParam,
	EDD_RSP                     Response);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTApplTimerIfSetupStateMachine(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDD_RSP                     ResponseSm);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTApplTimerIfSetupStateMachineCbf(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDD_RSP                     Response);

/* ---------------------------------------------------------------------------- */
/* State machine outbound/inbound/DGs                                           */
/* ***** eddp_crt_cons_dfp.c                                                    */
/* ---------------------------------------------------------------------------- */
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTSetRxTxDP2BoundaryAndInbound(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32				    HwPortID,
    LSA_BOOL                    isSetBoundary);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTInitInboundDevOpen(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DPB_PTR_TYPE     pDPB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTInitInboundDevSetup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_UPPER_DSB_PTR_TYPE     pDSB);

#define EDDP_CRT_SET_INBOUND_ON     LSA_FALSE
#define EDDP_CRT_SET_INBOUND_OFF    LSA_TRUE
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTInboundStateMachine(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_BOOL                    ExecuteOff,
    EDDP_CRT_CBF_PTR_TYPE       Cbf);

#define EDDP_CRT_SET_OUTBOUND_ON     LSA_FALSE
#define EDDP_CRT_SET_OUTBOUND_OFF    LSA_TRUE
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTOutboundStateMachine(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_BOOL                    ExecuteOff,
    EDDP_CRT_CBF_PTR_TYPE       Cbf);

#define EDDP_CRT_SET_RCV_ACWDG_ON     LSA_FALSE
#define EDDP_CRT_SET_RCV_ACWDG_OFF    LSA_TRUE
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTRcvAcwDGStateMachine(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_BOOL                    ExecuteOff,
    EDDP_CRT_CBF_PTR_TYPE       Cbf);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTLateErrorStartLSATimer(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTLateErrorStopLSATimer(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTLateErrorTimerStartPackGrp(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT8                   PackGrpId,
    LSA_UINT8                   TimerType);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTLateErrorTimerStopPackGrp(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT8                   PackGrpId);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTLateErrorSetupLateErrMask(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT8                   PackGrpId,
    LSA_BOOL                    isMask);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTLateErrorDeleteLateErrStatus(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT8                   PackGrpId);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTLateErrorTriggerAppearDiagAllActiveInBound(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_BOOL                    isPrmCommit);

#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTWrkarPackGrpDisableTimerPassed(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);
#endif

#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTWrkarPackGrpDisableStartLSATimer(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);
#endif

#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTWrkarPackGrpDisableStopLSATimer(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);
#endif

#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTWrkarPackGrpDisableTimerStart(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  TimerUserIdx);
#endif

#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTWrkarPackGrpDisableTimerStop(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  TimerUserIdx);
#endif


#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif  /* of EDDP_CRT_INT_H */
