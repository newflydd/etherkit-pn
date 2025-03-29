
#ifndef EDDP_CORE_GLB_H                  /* ----- reinclude-protection ----- */
#define EDDP_CORE_GLB_H

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
/*  F i l e               &F: eddp_core_glb.h                           :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Internal headerfile with global defines/macros                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  14.08.08    JS    initial version.                                       */
#endif
/*****************************************************************************/

/*======================================================================================*/
/*                                  defines                                             */
/*======================================================================================*/
/* ---------------------------------------------------------------------------------------------------- */
/* switches                                                                                             */
/* ---------------------------------------------------------------------------------------------------- */
/* *** PRM *** */
//#define EDDP_PRM_TEST_RECORD_ON                                         // test PRM records ON/OFF (only for test)
//#define EDDP_PRM_PREAMBLELENGTH_HACK                                    // special test flag to force RED independent of Neighbour_TopoState
//#define EDDP_PRM_TRACE_BYTE_ARRAY_ON                                    // enable function EDDP_GlbTraceByteArray() for PRM
/* *** CRT *** */
//#define EDDP_CRT_SUPPORT_RT_OVER_UDP_ON                                 // EDDP supports the RToverUDP
#define EDDP_CRT_SUPPORT_ACW_WAIT_ON                                    // EDDP supports the ACW_Wait=1 in PPM-ACW
//#define EDDP_CRT_ISR_PPM_HOL_NOT_EMPTY_RESOURCETYPE6_RTC3_FATAL_ON      // activate fatal if this interrupt has appeared for ResourceType6-RTC3 (Port1)
//#define EDDP_CRT_ISR_PPM_HOL_NOT_EMPTY_RESOURCETYPE7_RTC3_FATAL_ON      // activate fatal if this interrupt has appeared for ResourceType7-RTC3 (Port2)
/* *** SWI *** */
#define EDDP_SWI_SUPPORT_FFW_ON                                         // EDDP supports the FastForwarding
//#define EDDP_SWI_ISR_FRAMEAGING_EXPIRED_QUEUEPRIO8_RTC3_FATAL_ON        // activate fatal if this interrupt has appeared for QueuePrio8-RTC3
//#define EDDP_SWI_ISR_FRAMEAGING_EXPIRED_QUEUEPRIO15_RTC3_FATAL_ON       // activate fatal if this interrupt has appeared for QueuePrio15-RTC3
#define EDDP_SWI_ISR_MACERR_MACTBUFFEROVERFLOWERR_FATAL_ON              // activate fatal if this interrupt has appeared for MACTBufferOverflowErr
#define EDDP_SWI_ISR_MACERR_MACTBUFFERUNDERFLOWERR_FATAL_ON             // activate fatal if this interrupt has appeared for MACTBufferUnderflowErr


/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* "Alignment" for function     EDDP_GlbIsAligned (pDDB, Value, Alignment)                              */
/* ---------------------------------------------------------------------------------------------------- */
// case 2 byte alignment
#define EDDP_GLB_ALIGNMENT_2_BYTE               2
#define EDDP_GLB_ALIGNMENT_2_MASK               0x1
// case 4 byte alignment
#define EDDP_GLB_ALIGNMENT_4_BYTE               4
#define EDDP_GLB_ALIGNMENT_4_MASK               0x3
// case 8 byte alignment
#define EDDP_GLB_ALIGNMENT_8_BYTE               8
#define EDDP_GLB_ALIGNMENT_8_MASK               0x7
// case 8 byte alignment
#define EDDP_GLB_ALIGNMENT_16_BYTE              16
#define EDDP_GLB_ALIGNMENT_16_MASK              0xF
// case 64 byte alignment
#define EDDP_GLB_ALIGNMENT_64_BYTE              64
#define EDDP_GLB_ALIGNMENT_64_MASK              0x3F
// shift
#define EDDP_GLB_ALIGNMENT_SHIFT                0
/* ---------------------------------------------------------------------------------------------------- */


/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* Resource-Type    using in MultiCast-/FrameClass-Table                                                */
/* ---------------------------------------------------------------------------------------------------- */
/*  0   OTHER       IP, DCP, ARP, Forwarding RToverUDP                                                  */
/*  1   RTA         PNIO - Alarme High/Low                                                              */
/*  2   MRP         MediumRedundancyProtocol (HSR)                                                      */
/*  3   LLDP        LinkLayerDiscoveryProtocol                                                          */
/*  4   PTCP        Sync: Clock, Time, Delay, Announce                                                  */
/*  5   RT_L        Inject      : RTC2, RTC1, RToverUDP (E200P: all Ports)                              */
/*                  Forwarding  : RTC2, RTC1                                                            */
/*      RT_L1       Inject      : RTC2, RTC1, RToverUDP (HERA: Port 1)                                  */
/*  6   RT_H        Forwarding  : RTC3                                                                  */
/*      RT_H1       Inject      : RTC3 Port1                                                            */
/*  7   RT_H2       Inject      : RTC3 Port2                                                            */
/*  8   RT_H3       Inject      : RTC3 Port3 (HERA: Port 3)                                             */
/*  9   RT_H4       Inject      : RTC3 Port3 (HERA: Port 4)                                             */
/*  10  RT_L2       Inject      : RTC2, RTC1, RToverUDP Port2 (HERA: Port 2)                            */
/*  11  RT_L3       Inject      : RTC2, RTC1, RToverUDP Port3 (HERA: Port 3)                            */
/*  12  RT_L4       Inject      : RTC2, RTC1, RToverUDP Port4 (HERA: Port 4)                            */    
/* ---------------------------------------------------------------------------------------------------- */
   
#define EDDP_MCFCT_RESOURCE_TYPE_OTHER          0
#define EDDP_MCFCT_RESOURCE_TYPE_RTA            1
#define EDDP_MCFCT_RESOURCE_TYPE_MRP            2
#define EDDP_MCFCT_RESOURCE_TYPE_LLDP           3
#define EDDP_MCFCT_RESOURCE_TYPE_PTCP           4
#define EDDP_MCFCT_RESOURCE_TYPE_RT_L           5
#define EDDP_MCFCT_RESOURCE_TYPE_RT_L1          5
#define EDDP_MCFCT_RESOURCE_TYPE_RT_H           6
#define EDDP_MCFCT_RESOURCE_TYPE_RT_H1          6
#define EDDP_MCFCT_RESOURCE_TYPE_RT_H2          7
#define EDDP_MCFCT_RESOURCE_TYPE_RT_H3          8
#define EDDP_MCFCT_RESOURCE_TYPE_RT_H4          9
#define EDDP_MCFCT_RESOURCE_TYPE_RT_L2          10
#define EDDP_MCFCT_RESOURCE_TYPE_RT_L3          11
#define EDDP_MCFCT_RESOURCE_TYPE_RT_L4          12
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */

/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* API-Type         using in MultiCast-/FrameClass-Table                                                */
/* ---------------------------------------------------------------------------------------------------- */
/*  0   OTHER       EDD_NRT_FRAME_OTHER                                                                 */
/*  1   IP          EDD_NRT_FRAME_IP_xxx, EDD_NRT_FRAME_UDP_xxx                                         */
/*  2   ARP         EDD_NRT_FRAME_ARP                                                                   */
/*  3   MRP         EDD_NRT_FRAME_MRP                                                                   */
/*  4   LLDP        EDD_NRT_FRAME_LLDP                                                                  */
/*  5   DCP         EDD_NRT_FRAME_DCP                                                                   */
/*  6   ALARM       EDD_NRT_FRAME_ASRT                                                                  */
/*  7   LEN         EDD_NRT_FRAME_LEN_xxx                                                               */
/*  8   PTCP        EDD_NRT_FRAME_PTCP, PTCP-Frames, KRISC32                                            */
/*  9   IP6         EDD_NRT_FRAME_IP_xxx, EDD_NRT_FRAME_UDP_xxx                                         */  
/*  10..13          not used                                                                            */
/*  14  RT_SF       RT-Frames: Forwarding to Queue-14 (local API) with Store&Forward mode               */
/*  15  RT_CT       RT-Frames: Forwarding to Queue-15 (local API) with CutThrough mode                  */
/* ---------------------------------------------------------------------------------------------------- */
// MCFCT = MultiCast-/FrameClass-Table
#define EDDP_MCFCT_API_TYPE_OTHER               0
#define EDDP_MCFCT_API_TYPE_IP                  1
#define EDDP_MCFCT_API_TYPE_ARP                 2
#define EDDP_MCFCT_API_TYPE_MRP                 3
#define EDDP_MCFCT_API_TYPE_LLDP                4
#define EDDP_MCFCT_API_TYPE_DCP                 5
#define EDDP_MCFCT_API_TYPE_ALARM               6
#define EDDP_MCFCT_API_TYPE_LEN                 7
#define EDDP_MCFCT_API_TYPE_PTCP                8
#define EDDP_MCFCT_API_TYPE_IP6                 9
// 10..13    not used
#define EDDP_MCFCT_API_TYPE_RT_SF               14
#define EDDP_MCFCT_API_TYPE_RT_CT               15
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */

/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* FrameHandling    using in MultiCast-/FrameClass-Table                                                */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/*  00  NRT         NRT Frame                                                                           */
/*  01  RTA         RTA Frame                                                                           */
/*  10  RT_L        RT low Frame                                                                        */
/*  11  RT_H        RT high Frame                                                                       */
/*  1x  RT          RT Frame                                                                            */
/* ---------------------------------------------------------------------------------------------------- */
// MCFCT = MultiCast-/FrameClass-Table
#define EDDP_MCFCT_FRAME_HANDLING_NRT           0x0
#define EDDP_MCFCT_FRAME_HANDLING_RTA           0x1
#define EDDP_MCFCT_FRAME_HANDLING_RT_L          0x2
#define EDDP_MCFCT_FRAME_HANDLING_RT_H          0x3
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */

/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* UseType          using in MultiCast-/FrameClass-Table                                                */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/*  000             STANDARD                                                                            */
/*  001             PTCP_CLOCK                                                                          */
/*  010             PTCP_TIME                                                                           */
/*  011             PTCP_DELAY                                                                          */
/*  100             FUP_CLOCK                                                                           */
/*  101             FUP_TIME                                                                            */
/*  110             PTP_TIME                                                                            */
/*  111             PTP_DELAY                                                                           */
/* ---------------------------------------------------------------------------------------------------- */
// MCFCT = MultiCast-/FrameClass-Table
#define EDDP_MCFCT_USE_TYPE_STANDARD            0x0
#define EDDP_MCFCT_USE_TYPE_PTCP_CLOCK          0x1
#define EDDP_MCFCT_USE_TYPE_PTCP_TIME           0x2
#define EDDP_MCFCT_USE_TYPE_PTCP_DELAY          0x3
#define EDDP_MCFCT_USE_TYPE_FUP_CLOCK           0x4
#define EDDP_MCFCT_USE_TYPE_FUP_TIME            0x5
#define EDDP_MCFCT_USE_TYPE_PTP_TIME            0x6
#define EDDP_MCFCT_USE_TYPE_PTP_DELAY           0x7
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */


/* ---------------------------------------------------------------------------------------------------- */
/* Defines                                                                                              */
/* ---------------------------------------------------------------------------------------------------- */
// 8 ns resolution
#define EDDP_UNIT_RESOLUTION_8              8
// TimeTicks resolution in ns (1 Tick = 1 ns)
#define EDDP_TIME_TICKS_RESOLUTION_NS       1

// time-byte resolution for RT frame
#define EDDP_TIME_BYTE_RESOLUTION_RT_FRAME_NS                   80
// time-byte resolution only for IO-Data of RT-Frame
#define EDDP_TIME_BYTE_RESOLUTION_IO_DATA_NS_ERTEC200P          8
#define EDDP_TIME_BYTE_RESOLUTION_IO_DATA_NS_FPGA               32
/* ---------------------------------------------------------------------------------------------------- */


/* ---------------------------------------------------------------------------------------------------- */
/* MultiCast MAC addresses                                                                              */
/* ---------------------------------------------------------------------------------------------------- */
#define EDDP_MC_MAC_ADR_RTC3        { 0x01, 0x0E, 0xCF, 0x00, 0x01, 0x01 }  // RTC3
#define EDDP_MC_MAC_ADR_FFW         { 0x03, 0x00, 0x00, 0x00, 0x00, 0x00 }  // FastForwarding
/* ---------------------------------------------------------------------------------------------------- */





/*======================================================================================*/
/*                                   types                                              */
/*======================================================================================*/

/* ### state machine PhySTS ### */
/* States of statemachine PhySTS */
typedef enum EDDP_PHYSTS_SM_STATE_TYPE_
{
    EDDP_PHYSTS_SM_STATE_INIT_WF_PWRUP,
    EDDP_PHYSTS_SM_STATE_RDY,
    EDDP_PHYSTS_SM_STATE_RUN_WF_PWRUP
} EDDP_PHYSTS_SM_STATE_TYPE;

/* Trigger events of statemachine PhySTS */
typedef enum EDDP_PHYSTS_SM_TRIGGER_TYPE_
{
    EDDP_PHYSTS_SM_TRIGGER_INT_STATUS_CHANGE = 0,    
    EDDP_PHYSTS_SM_TRIGGER_PLUG_COMMIT = 1
} EDDP_PHYSTS_SM_TRIGGER_TYPE;

typedef struct _EDDP_SWI_PHY_STS_SM_TYPE  EDDP_LOCAL_MEM_ATTR *   _EDDP_SWI_PHY_STS_SM_PTR_TYPE;
typedef struct _EDDP_SWI_PHY_STS_SM_TYPE
{
    EDDP_PHYSTS_SM_STATE_TYPE       State;
    EDD_RSP                         Response;
    LSA_BOOL                        isAdjustLinkStateAvailable[EDDP_MAX_PORT_CNT];
} EDDP_SWI_PHY_STS_SM_TYPE;
/* ### end of PhySTS ### */

/* ### PortModul state machine ### */
typedef enum EDDP_PORT_MODULE_SM_STATE_TYPE_
{
    EDDP_PORT_MODULE_SM_STATE_ISPULLED,
    EDDP_PORT_MODULE_SM_STATE_PLUGGING,
    EDDP_PORT_MODULE_SM_STATE_PLUGINPROGRESS,
    EDDP_PORT_MODULE_SM_STATE_ISPLUGGED
} EDDP_PORT_MODULE_SM_STATE_TYPE;

typedef enum EDDP_PRM_PORT_MODULE_SM_TRIGGER_TYPE_
{
    EDDP_PRM_PORT_MODULE_SM_TRIGGER_PULL            = 0,
    EDDP_PRM_PORT_MODULE_SM_TRIGGER_PLUG_PREPARE    = 1,    
    EDDP_PRM_PORT_MODULE_SM_TRIGGER_PLUG_COMMIT     = 2,
    EDDP_PRM_PORT_MODULE_SM_TRIGGER_PHY_ISR         = 3
} EDDP_PRM_PORT_MODULE_SM_TRIGGER_TYPE;

typedef struct _EDDP_PRM_PORT_MODULE_SM_TYPE  EDDP_LOCAL_MEM_ATTR *   _EDDP_PRM_PORT_MODULE_SM_PTR_TYPE;
typedef struct _EDDP_PRM_PORT_MODULE_SM_TYPE
{
    EDDP_PORT_MODULE_SM_STATE_TYPE              State;
} EDDP_PRM_PORT_MODULE_SM_TYPE;
/* ### end of PortModul state machine ### */

/* forward declarations. see eddp_dev.h */    
typedef struct _EDDP_HDB_TYPE        EDDP_LOCAL_MEM_ATTR *    EDDP_LOCAL_HDB_PTR_TYPE;
typedef struct _EDDP_DDB_TYPE        EDDP_LOCAL_MEM_ATTR *    EDDP_LOCAL_DDB_PTR_TYPE;

/* ------------------------------------------------------------------------------------ */
/* this structure is  used for internal request initiated by interrupt or               */
/* timeout-handler. pParam in pRQB points to EDDP_RQB_DEV_INTERNAL_TYPE                 */
/* InUse is set if the RQB is underway (from Int-Handler to eddp_system(). if           */
/* it arrives eddp_system() InUse is reset to LSA_FALSE. So only one RQB is             */
/* used for every event.                                                                */
/* ------------------------------------------------------------------------------------ */
typedef volatile struct _EDDP_INTERNAL_REQUEST_STRUCT
{
    LSA_BOOL                    InUse;  /* LSA_TRUE if in rqb-queue */
    EDD_UPPER_RQB_PTR_TYPE      pRQB;           
} EDDP_INTERNAL_REQUEST_TYPE;
typedef EDDP_INTERNAL_REQUEST_TYPE  EDDP_LOCAL_MEM_ATTR *   EDDP_INTERNAL_REQUEST_PTR_TYPE;


/*======================================================================================*/
/* Queuing of RQBs or internal management blocks                                        */
/* Used with general queuing functions.                                                 */
/*======================================================================================*/
/* for queueing RQBs */
typedef struct _EDDP_RQB_QUEUE_TYPE EDDP_LOCAL_MEM_ATTR *   EDDP_RQB_QUEUE_PTR_TYPE;
typedef struct _EDDP_RQB_QUEUE_TYPE
{
    EDD_UPPER_RQB_PTR_TYPE      pFirst;
    EDD_UPPER_RQB_PTR_TYPE      pLast;
    LSA_UINT32                  Count;
} EDDP_RQB_QUEUE_TYPE;


/* for queueing internal blocks */

// predefine
typedef struct _EDDP_QUEUE_ELEMENT_TYPE EDDP_LOCAL_MEM_ATTR *   EDDP_QUEUE_ELEMENT_PTR_TYPE;
typedef struct _EDDP_QUEUE_ELEMENT_TYPE
{
    EDDP_QUEUE_ELEMENT_PTR_TYPE     next_ptr;
    EDDP_QUEUE_ELEMENT_PTR_TYPE     prev_ptr;
} EDDP_QUEUE_ELEMENT_TYPE;

typedef struct _EDDP_QUEUE_TYPE EDDP_LOCAL_MEM_ATTR *   EDDP_QUEUE_PTR_TYPE;
typedef struct _EDDP_QUEUE_TYPE
{
    EDDP_QUEUE_ELEMENT_PTR_TYPE     pFirst;
    EDDP_QUEUE_ELEMENT_PTR_TYPE     pLast;
    LSA_UINT32                      Count;
} EDDP_QUEUE_TYPE;





/*======================================================================================*/
/*                                  macros                                              */
/*======================================================================================*/

// hDDB is a pointer to the DDB
#define EDDP_GETDDB(hDDB)   ((EDD_LOCAL_DDB_PTR_TYPE) (hDDB))

// check for: FrameID is in IRT range [0x0100..0x0FFF]
#define EDDP_IS_IRT_FRAMEID(FrameID) \
            ((FrameID >= EDD_SRT_FRAMEID_IRT_START) && (FrameID <= EDD_SRT_FRAMEID_IRT_STOP))

// check for: FrameID is in IRT redundat range [0x0700..0x0FFF]
#define EDDP_IS_IRT_REDUNDANT_FRAMEID(FrameID) \
            ((FrameID >= EDD_SRT_FRAMEID_IRT_REDUNDANT_START) && (FrameID <= EDD_SRT_FRAMEID_IRT_REDUNDANT_STOP))


/*=============================================================================
 * function name:  EDDP_FATAL
 *
 * function:       issue a fatal error. for error codes see <eddp_sys.h>
 *
 * parameters:     Error         .....
 *
 * return value:   none 
 *===========================================================================*/
#define EDDP_FATAL(Error)                                        \
        EDDP_GlbFatalError (Error, EDDP_MODULE_ID, __LINE__)

/*=============================================================================
 * function name:  EDDP_ALLOC_FAIL_FATAL
 *
 * function:       check if memory alloc failed 
 *
 * parameters:     pPtr         .....
 *
 * return value:   none 
 *===========================================================================*/
#define EDDP_ALLOC_FAIL_FATAL(pPtr)                                                                                 \
{                                                                                                                   \
    if ((LSA_HOST_PTR_ARE_EQUAL(pPtr, LSA_NULL)))                                                                   \
    {                                                                                                               \
        EDDP_FATAL(EDDP_FATAL_ERR_NULL_PTR);                                                                        \
    }                                                                                                               \
}

/*=============================================================================
 * function name:  EDDP_FREE_CHECK
 *
 * function:       check if memory free failed 
 *
 * parameters:     RetVal (LSA/EDD-Response code) of free call
 *
 * return value:   none 
 *===========================================================================*/
#ifdef EDDP_CFG_FREE_ERROR_IS_FATAL

    #define EDDP_FREE_CHECK(RetVal)                                                                                                                             \
    {                                                                                                                                                           \
        if ((RetVal) != LSA_RET_OK)                                                                                                                             \
        {                                                                                                                                                       \
            EDDP_FATAL(EDDP_FATAL_ERR_MEM_FREE);                                                                                                                \
        }                                                                                                                                                       \
    }

#else

    #define EDDP_FREE_CHECK(RetVal)                                                                                                                             \
    {                                                                                                                                                           \
        LSA_UNUSED_ARG((RetVal));                                                                                                                                                    \
    }

#endif

/*=============================================================================
 * function name:  EDDP_ASSERT_NO_NULL_PTR
 *
 * function:       check for No NULL ptr assertion
 *
 * parameters:     pPtr
 *
 * return value:   none 
 *===========================================================================*/
#ifdef EDDP_CFG_DEBUG_ASSERT
    #define EDDP_ASSERT_NO_NULL_PTR(pPtr)                                                                                   \
    {                                                                                                                       \
        if ( (LSA_HOST_PTR_ARE_EQUAL(pPtr, LSA_NULL)) )                                                                     \
        {                                                                                                                   \
            EDDP_FATAL(EDDP_FATAL_ERR_NULL_PTR);                                                                            \
        }                                                                                                                   \
    }

    #define EDDP_ASSERT_NO_NULL_FCT_PTR(pPtr)                                                                                   \
    {                                                                                                                       \
        if (0 == pPtr)                                                                     \
        {                                                                                                                   \
            EDDP_FATAL(EDDP_FATAL_ERR_NULL_PTR);                                                                            \
        }                                                                                                                   \
    }
#else   
    #define EDDP_ASSERT_NO_NULL_PTR(pPtr) {LSA_UNUSED_ARG((pPtr));}

    #define EDDP_ASSERT_NO_NULL_FCT_PTR(pPtr) {LSA_UNUSED_ARG((pPtr));}
#endif

/*=============================================================================
 * function name:  EDDP_ASSERT_NULL_PTR
 *
 * function:       check for NULL ptr assertion
 *
 * parameters:     pPtr
 *
 * return value:   none 
 *===========================================================================*/
#ifdef EDDP_CFG_DEBUG_ASSERT
    #define EDDP_ASSERT_NULL_PTR(pPtr)                                                                                  \
    {                                                                                                                   \
        if ( !(LSA_HOST_PTR_ARE_EQUAL(pPtr, LSA_NULL)) )                                                                \
        {                                                                                                               \
            EDDP_FATAL(EDDP_FATAL_ERR_NULL_PTR);                                                                        \
        }                                                                                                               \
    }
#else   
    #define EDDP_ASSERT_NULL_PTR(pPtr) {LSA_UNUSED_ARG((pPtr));}
#endif

/*=============================================================================
 * function name:  EDDP_ASSERT
 *
 * function:       check for FALSE condition assertion
 *
 * parameters:     Cond_
 *
 * return value:   none 
 *===========================================================================*/
#ifdef EDDP_CFG_DEBUG_ASSERT
    #define EDDP_ASSERT(Cond_)                                                                                      \
    {                                                                                                               \
        if (!(Cond_))                                                                                               \
        {                                                                                                           \
            EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);                                                               \
        }                                                                                                           \
    }
#else   
    #define EDDP_ASSERT(Cond_) {LSA_UNUSED_ARG((Cond_));}
#endif

/*=============================================================================
 * function name:  EDDP_ASSERT_FALSE
 *
 * function:       check for TRUE condition assertion
 *
 * parameters:     Cond_
 *
 * return value:   none 
 *===========================================================================*/
#ifdef EDDP_CFG_DEBUG_ASSERT
    #define EDDP_ASSERT_FALSE(Cond_)                                                                                \
    {                                                                                                               \
        if ((Cond_))                                                                                                \
        {                                                                                                           \
            EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);                                                               \
        }                                                                                                           \
    }
#else   
    #define EDDP_ASSERT_FALSE(Cond_) {LSA_UNUSED_ARG((Cond_));}
#endif


/*=====================================================================================================================*/
/*  Bit range operations (get/set) within a variable                                                                   */
/*                                                                                                                     */
/*  Msk     : Masks bit area within Val (e.g. 0x0070 -> bits 4,5,6)                                                    */
/*  Shift   : Position (starting with 0) of first bit of Msk (e.g. 4)                                                  */
/*                                                                                                                     */
/*=====================================================================================================================*/

#define EDDP_GET_BIT_VALUE(Val, Msk, Shift)     (((Val) & (Msk)) >> Shift)

/*=====================================================================================================================*/
/*=====================================================================================================================*/

#define EDDP_SET_BIT_VALUE8(Target, Val, Msk, Shift)                                                       \
{                                                                                                          \
    Target = ((Target) &~ ((LSA_UINT8)(Msk))) | ((((LSA_UINT8)(Val)) << (Shift)) & ((LSA_UINT8)(Msk)));    \
}

#define EDDP_SET_BIT_VALUE16(Target, Val, Msk, Shift)                                                      \
{                                                                                                          \
    Target = ((Target) &~ ((LSA_UINT16)(Msk))) | ((((LSA_UINT16)(Val)) << (Shift)) & ((LSA_UINT16)(Msk))); \
}

#define EDDP_SET_BIT_VALUE32(Target, Val, Msk, Shift)                                                      \
{                                                                                                          \
    Target = ((Target) &~ ((LSA_UINT32)(Msk))) | ((((LSA_UINT32)(Val)) << (Shift)) & ((LSA_UINT32)(Msk))); \
}

#define EDDP_SET_BIT_VALUE64(Target, Val, Msk, Shift)                                                      \
{                                                                                                          \
    Target = ((Target) &~ ((LSA_UINT64)(Msk))) | ((((LSA_UINT64)(Val)) << (Shift)) & ((LSA_UINT64)(Msk))); \
}

/*=====================================================================================================================*/
/*=====================================================================================================================*/

#define EDDP_CLR_BIT_VALUE8(Target, Msk)            \
{                                                   \
    Target &= (~((LSA_UINT8)(Msk)));                \
}

#define EDDP_CLR_BIT_VALUE16(Target, Msk)           \
{                                                   \
    Target &= (~((LSA_UINT16)(Msk)));               \
}

#define EDDP_CLR_BIT_VALUE32(Target, Msk)           \
{                                                   \
    Target &= (~((LSA_UINT32)(Msk)));               \
}

#define EDDP_CLR_BIT_VALUE64(Target, Msk)           \
{                                                   \
    Target &= (~((LSA_UINT64)(Msk)));               \
}

/*=====================================================================================================================*/
/*=====================================================================================================================*/

#define EDDP_SHIFT_VALUE_RIGHT(Target, Shift)       \
{                                                   \
    Target = ((Target) >> (Shift));                 \
}

#define EDDP_SHIFT_VALUE_LEFT(Target, Shift)        \
{                                                   \
    Target = ((Target) << (Shift));                 \
}

/*=====================================================================================================================*/
/*=====================================================================================================================*/


/*===========================================================================*/
/*   Check if given value is odd or even                                     */
/*===========================================================================*/
#define EDDP_NUMBER_IS_EVEN(Number)     (((Number) & 0x1) == 0)
#define EDDP_NUMBER_IS_ODD(Number)      (((Number) & 0x1) == 1)


/*===========================================================================*/
/*  Description: Calculate 2^Exponent.                                       */
/*===========================================================================*/

#define EDDP_GET_2_POTENZ(Exponent)     (1l << (Exponent))


/* ============================================================================ */
/*                  Macros for MAC/IP addresses                                 */
/* ============================================================================ */

// is broadcom MAC(FF:FF:FF:FF:FF:FF)
#define EDDP_IS_BC_MAC(SrcMAC)      \
        ( (SrcMAC.MacAdr[0] & SrcMAC.MacAdr[1] & SrcMAC.MacAdr[2] & SrcMAC.MacAdr[3] & SrcMAC.MacAdr[4] & SrcMAC.MacAdr[5]) == 0xFF )

// is MultiCast MAC(01:xx:xx:xx:xx:xx)
#define EDDP_IS_MC_MAC(SrcMAC)      ( (SrcMAC.MacAdr[0] & 0x01) )

// is zero MAC(00:00:00:00:00:00)
#define EDDP_IS_ZERO_MAC(SrcMAC)    \
        ( (SrcMAC.MacAdr[0] | SrcMAC.MacAdr[1] | SrcMAC.MacAdr[2] | SrcMAC.MacAdr[3] | SrcMAC.MacAdr[4] | SrcMAC.MacAdr[5]) == 0x00 )

// is broadcom IP(FF:FF:FF:FF)
#define EDDP_IS_BC_IP(SrcIP)        \
        ( (SrcIP.IPAdr[0] & SrcIP.IPAdr[1] & SrcIP.IPAdr[2] & SrcIP.IPAdr[3]) == 0xFF )

// is zero IP(00:00:00:00)
#define EDDP_IS_ZERO_IP(SrcIP)      \
        ( (SrcIP.IPAdr[0] | SrcIP.IPAdr[1] | SrcIP.IPAdr[2] | SrcIP.IPAdr[3]) == 0x00 )


/*===========================================================================*/
/*  Conversion of Network byteorder to Host byteorder                        */
/*===========================================================================*/

#ifdef EDDP_CFG_BIG_ENDIAN

    #ifndef EDDP_NTOHS
    #define EDDP_NTOHS(Value)   (Value)     /* Network TO Host Short */
    #endif

    #ifndef EDDP_NTOHL
    #define EDDP_NTOHL(Value)   (Value)     /* Network TO Host Long */
    #endif

    #ifndef EDDP_HTONS
    #define EDDP_HTONS(Value)   (Value)     /* Host To Network Short */
    #endif

    #ifndef EDDP_NTONL
    #define EDDP_HTONL(Value)   (Value)     /* Host TO Network Long */
    #endif

#else

    #ifndef EDDP_NTOHS
    #define EDDP_NTOHS(Value)   (EDDP_SWAP16(Value))    /* Network TO Host Short */
    #endif

    #ifndef EDDP_NTOHL
    #define EDDP_NTOHL(Value)   (EDDP_SWAP32(Value))    /* Network TO Host Long */
    #endif

    #ifndef EDDP_HTONS
    #define EDDP_HTONS(Value)   (EDDP_SWAP16(Value))    /* Host To Network Short */
    #endif

    #ifndef EDDP_NTONL
    #define EDDP_HTONL(Value)   (EDDP_SWAP32(Value))    /* Host TO Network Long */
    #endif

#endif


/*===========================================================================*/
/*  access to Network byte ordered unaligned variables                       */
/*===========================================================================*/

/* pValue has to be a UINT8 - Ptr! */
#define EDDP_GET_U16(pValue)    ((LSA_UINT16) (((LSA_UINT16)(*(pValue)) << 8 ) + *((pValue)+1)))

/* pValue has to be a UINT8 - Ptr! */
#define EDDP_GET_U32(pValue)    ((LSA_UINT32) ( ((LSA_UINT32)(*(pValue))     << 24) +   \
                                                ((LSA_UINT32)(*((pValue)+1)) << 16) +   \
                                                ((LSA_UINT32)(*((pValue)+2)) << 8 ) +   \
                                               *((pValue)+3) ) )

/* pValue has to be a UINT8 - Ptr! */
#define EDDP_GET_U16_INCR(pValue, Value)    \
{                                           \
	Value = EDDP_GET_U16(pValue);           \
	pValue += sizeof(LSA_UINT16);           \
}

/* pValue has to be a UINT8 - Ptr! */
#define EDDP_GET_U32_INCR(pValue, Value)    \
{                                           \
	Value = EDDP_GET_U32(pValue);           \
	pValue += sizeof(LSA_UINT32);           \
}

/* pValue has to be a UINT8 - Ptr! */     
#define EDDP_SET_U8_INCR(pValue, Value)     \
{                                           \
	*(pValue)++ = (LSA_UINT8) ((Value));    \
}

/* pValue has to be a UINT8 - Ptr! */     
#define EDDP_SET_U16_INCR(pValue, Value)            \
{                                                   \
	*(pValue)++ = (LSA_UINT8) ((Value) >> 8);       \
	*(pValue)++ = (LSA_UINT8) ((Value));            \
}

/* pValue has to be a UINT8 - Ptr! */     
#define EDDP_SET_U32_INCR(pValue, Value)            \
{                                                   \
	*(pValue)++ = (LSA_UINT8) ((Value) >> 24);      \
	*(pValue)++ = (LSA_UINT8) ((Value) >> 16);      \
	*(pValue)++ = (LSA_UINT8) ((Value) >> 8 );      \
	*(pValue)++ = (LSA_UINT8) ((Value));            \
}


/*===========================================================================*/
/*                               miscellaneous                               */
/*===========================================================================*/

/* check if a MAC address is 0. _s1 must be of type EDD_MAC_ADR_TYPE */
#define EDDP_MACADDR_IS_0(_s1)                                  \
        ((_s1.MacAdr[0] == 0) && (_s1.MacAdr[1] == 0) &&        \
         (_s1.MacAdr[2] == 0) && (_s1.MacAdr[3] == 0) &&        \
         (_s1.MacAdr[4] == 0) && (_s1.MacAdr[5] == 0))


/*===========================================================================*/
/* Protection macros with optional reentrance check                          */
/*                                                                           */
/* EDD intern we use this macros instead of the direct output macro!         */
/*                                                                           */
/*===========================================================================*/

/*=============================================================================
 * function name:  EDDP_ENTER_LOW_S, EDDP_EXIT_LOW_S
 *
 * function:       LOW context (no Systemoutput macro for protection. 
 *                              design requirement!)
 *
 * parameters:     none 
 *
 * return value:   none 
 *===========================================================================*/
#ifdef EDDP_CFG_REENTRANCE_CHECK

    #define EDDP_ENTER_LOW_S()                              \
    {                                                       \
        if (!pEDDPInfo->Locking.EDDPLockSemaLOW)            \
        {                                                   \
            pEDDPInfo->Locking.EDDPLockSemaLOW++;           \
        }                                                   \
        else                                                \
        {                                                   \
            EDDP_FATAL(EDDP_FATAL_ERR_REENTRANCE);          \
        }                                                   \
    }

    #define EDDP_EXIT_LOW_S()                               \
    {                                                       \
        pEDDPInfo->Locking.EDDPLockSemaLOW--;               \
        if (pEDDPInfo->Locking.EDDPLockSemaLOW != 0 )       \
        {                                                   \
            EDDP_FATAL(EDDP_FATAL_ERR_REENTRANCE);          \
        }                                                   \
    }

#else

    #define EDDP_ENTER_LOW_S() {}
    #define EDDP_EXIT_LOW_S()  {}

#endif

/*=============================================================================
 * function name:  EDDP_ENTER_MAIN_S, EDDP_ENTER_MAIN_S
 *
 * function:       MAIN context protection                            
 *
 * parameters:     none 
 *
 * return value:   none 
 *===========================================================================*/
#ifdef EDDP_CFG_REENTRANCE_CHECK

    #define EDDP_ENTER_MAIN_S()                             \
    {                                                       \
        EDDP_ENTER_MAIN();                                  \
        if (!pEDDPInfo->Locking.EDDPLockSemaMAIN)           \
        {                                                   \
            pEDDPInfo->Locking.EDDPLockSemaMAIN++;          \
        }                                                   \
        else                                                \
        {                                                   \
            EDDP_FATAL(EDDP_FATAL_ERR_REENTRANCE);          \
        }                                                   \
    }

    #define EDDP_EXIT_MAIN_S()                              \
    {                                                       \
        pEDDPInfo->Locking.EDDPLockSemaMAIN--;              \
        if (pEDDPInfo->Locking.EDDPLockSemaMAIN != 0)       \
        {                                                   \
            EDDP_FATAL(EDDP_FATAL_ERR_REENTRANCE);          \
        }                                                   \
        EDDP_EXIT_MAIN();                                   \
    }

#else

    #define EDDP_ENTER_MAIN_S()     EDDP_ENTER_MAIN()
    #define EDDP_EXIT_MAIN_S()      EDDP_EXIT_MAIN()

#endif

/*=============================================================================
 * function name:  EDDP_ENTER_INT_S, EDDP_EXIT_INT_S
 *
 * function:       INT context protection for outside interrupthandler. 
 *                 LockSemaInt must be 0 outside of Interrupthandler if ENTER
 *                 was successful. 
 *
 * parameters:     none 
 *
 * return value:   none 
 *===========================================================================*/
#ifdef EDDP_CFG_REENTRANCE_CHECK

    #define EDDP_ENTER_INT_S()                              \
    {                                                       \
        EDDP_ENTER_INT();                                   \
        if (!pEDDPInfo->Locking.EDDPLockSemaINT)            \
        {                                                   \
            pEDDPInfo->Locking.EDDPLockSemaINT++;           \
        }                                                   \
        else                                                \
        {                                                   \
            EDDP_FATAL(EDDP_FATAL_ERR_REENTRANCE);          \
        }                                                   \
    }

    #define EDDP_EXIT_INT_S()                               \
    {                                                       \
        pEDDPInfo->Locking.EDDPLockSemaINT--;               \
        if (pEDDPInfo->Locking.EDDPLockSemaINT != 0)        \
        {                                                   \
            EDDP_FATAL(EDDP_FATAL_ERR_REENTRANCE);          \
        }                                                   \
        EDDP_EXIT_INT();                                    \
    }

#else

    #define EDDP_ENTER_INT_S()      EDDP_ENTER_INT()
    #define EDDP_EXIT_INT_S()       EDDP_EXIT_INT()

#endif

/*=============================================================================
 * function name:  EDDP_ENTER_INT_I, EDDP_EXIT_INT_I
 *
 * function:       INT context protection for inside interrupthandler     
 *                 The interrupthandler is reentrant for different "IntSource"
 *                 so reentrance can not be checked here globally. We only count the 
 *                 number of LOCKS for use with EDDP_EXIT_INT_S
 *
 * parameters:     none 
 *
 * return value:   none 
 *===========================================================================*/
#ifdef EDDP_CFG_REENTRANCE_CHECK

    #define EDDP_ENTER_INT_I()                              \
    {                                                       \
        EDDP_ENTER_INT();                                   \
        pEDDPInfo->Locking.EDDPLockSemaINT++;               \
    }

    #define EDDP_EXIT_INT_I()                               \
    {                                                       \
        pEDDPInfo->Locking.EDDPLockSemaINT--;               \
        EDDP_EXIT_INT();                                    \
    }

#else

    #define EDDP_ENTER_INT_I()      EDDP_ENTER_INT()
    #define EDDP_EXIT_INT_I()       EDDP_EXIT_INT()

#endif

/*=============================================================================
 * function name:  EDDP_ENTER_TIME_S, EDDP_EXIT_TIME_S
 *
 * function:       TIME context protection                            
 *
 * parameters:     none 
 *
 * return value:   none 
 *===========================================================================*/
#ifdef EDDP_CFG_REENTRANCE_CHECK

    #define EDDP_ENTER_TIME_S()                             \
    {                                                       \
        EDDP_ENTER_TIME();                                  \
        if (!pEDDPInfo->Locking.EDDPLockSemaTIME)           \
        {                                                   \
            pEDDPInfo->Locking.EDDPLockSemaTIME++;          \
        }                                                   \
        else                                                \
        {                                                   \
            EDDP_FATAL(EDDP_FATAL_ERR_REENTRANCE);          \
        }                                                   \
    }

    #define EDDP_EXIT_TIME_S()                              \
    {                                                       \
        pEDDPInfo->Locking.EDDPLockSemaTIME--;              \
        if (pEDDPInfo->Locking.EDDPLockSemaTIME != 0)       \
        {                                                   \
            EDDP_FATAL(EDDP_FATAL_ERR_REENTRANCE);          \
        }                                                   \
        EDDP_EXIT_TIME();                                   \
    }

#else

    #define EDDP_ENTER_TIME_S()     EDDP_ENTER_TIME()
    #define EDDP_EXIT_TIME_S()      EDDP_EXIT_TIME()

#endif

/*=============================================================================
 * function name:  EDDP_ENTER_I2C_S, EDDP_EXIT_I2C_S
 *
 * function:       I2C context protection                            
 *
 * parameters:     none 
 *
 * return value:   none 
 *===========================================================================*/
#ifdef EDDP_CFG_REENTRANCE_CHECK

    #define EDDP_ENTER_I2C_S()                              \
    {                                                       \
        EDDP_ENTER_I2C();                                   \
        if (!pEDDPInfo->Locking.EDDPLockSemaI2C)            \
        {                                                   \
            pEDDPInfo->Locking.EDDPLockSemaI2C++;           \
        }                                                   \
        else                                                \
        {                                                   \
            EDDP_FATAL(EDDP_FATAL_ERR_REENTRANCE);          \
        }                                                   \
    }

    #define EDDP_EXIT_I2C_S()                               \
    {                                                       \
        pEDDPInfo->Locking.EDDPLockSemaI2C--;               \
        if (pEDDPInfo->Locking.EDDPLockSemaI2C)             \
        {                                                   \
            EDDP_FATAL(EDDP_FATAL_ERR_REENTRANCE);          \
        }                                                   \
        EDDP_EXIT_I2C();                                    \
    }

#else

    #define EDDP_ENTER_I2C_S()      EDDP_ENTER_I2C()
    #define EDDP_EXIT_I2C_S()       EDDP_EXIT_I2C()

#endif


/*****************************************************************************/
/*  end of file EDDP_CORE_GLB.H                                              */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_CORE_GLB_H */
