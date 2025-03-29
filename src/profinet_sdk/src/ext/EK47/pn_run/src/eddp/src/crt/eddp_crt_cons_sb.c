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
/*  F i l e               &F: eddp_crt_cons_sb.c                        :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  CRT (Cycle Real Time) for EDDP  -> Score Board                           */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who         What                                             */
#ifdef EDDP_MESSAGE
/*  23.10.08    Sergeev     Erstellung                                       */
#endif
/*****************************************************************************/


/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/

#define LTRC_ACT_MODUL_ID   24
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_CRT_CONS_SB */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"           /* eddp headerfiles */
#include "eddp_int.h"           /* internal header  */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTScoreboardSetupEntry                +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB        +*/
/*+                             EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer   +*/
/*+  Result                :    ---                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DDB                                       (in) +*/
/*+  pConsumer  : Pointer to Consumer                                  (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Setup TSB and CSB Entrys to initial values for the        +*/
/*+               Consumer. Init the Consumer actual/indicated states       +*/
/*+               to defaults.                                              +*/
/*+                                                                         +*/
/*+               Called on activating the consumer                         +*/
/*+                                                                         +*/
/*+   DataStatus:                                                           +*/
/*+            ---+------------------+-----------------+----------------+   +*/
/*+            Bit|  Meaning         |       0         |        1       |   +*/
/*+            ---+------------------+-----------------+----------------+   +*/
/*+            0  |  State           |     BACKUP      |#    PRIMARY   #|   +*/
/*+            1  |  SystemRedundancy|# NO_REDUNDANCY #|    REDUNDANCY  |   +*/
/*+            2  |  DataValid       |#   INVALID     #|      VALID     |1) +*/
/*+            3  |  -----           |#               #|                |1) +*/
/*+            4  |  ProviderState   |     STOP        |#      RUN     #|   +*/
/*+            5  |  ProblemIndicator|    PROBLEM      |#     NORMAL   #|   +*/
/*+            6  |  -----           |#               #|                |1) +*/
/*+            7  |  Surrogate       |# NO_SURROGATE  #|    SURROGATE   |1) +*/
/*+            ---+------------------+-----------------+----------------+   +*/
/*+                                                                         +*/
/*+   Defaults marked with #. see ScoreBoard.EventStatusDefault             +*/
/*+   1) dont care with CSB (not checked)                                   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTScoreboardSetupEntry(
	EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
	EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer)
{
    LSA_UINT64      TSB_Word1, TSB_Word2;
    LSA_UINT32      CSB_Word1;
    LSA_UINT32      APICtrlRAMOffs;

    EDDP_ASSERT_NO_NULL_PTR(pConsumer);

    /* -------------------------------------------------------------------------------- */
    /* init struct Consumer.ScoreBoard (defaults for ConsumerEvents)                    */
    /* -------------------------------------------------------------------------------- */
    if (pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_SRD_ITEM)
        pConsumer->ScoreBoard.EventStatusDefault = EDDP_CRT_CONS_STATE_DEFAULT_SRD;
    else
        pConsumer->ScoreBoard.EventStatusDefault = EDDP_CRT_CONS_STATE_DEFAULT_NORMAL;

    pConsumer->ScoreBoard.ActEvents         = pConsumer->ScoreBoard.EventStatusDefault;
    pConsumer->ScoreBoard.IndEvents         = pConsumer->ScoreBoard.EventStatusDefault;
    pConsumer->ScoreBoard.WaitSetToUnknown  = LSA_FALSE;
    pConsumer->ScoreBoard.DebugInfo         = 0;

    /* -------------------------------------------------------------------------------- */
    /* init struct Consumer.MrtDiag                                                     */
    /* -------------------------------------------------------------------------------- */
    pConsumer->MrtDiag.IsPrmMRPDWatchDogActive  = LSA_FALSE;
    pConsumer->MrtDiag.LastMrtExpired           = LSA_FALSE;
    pConsumer->MrtDiag.PrmPDIRFrameDataIdx      = EDDP_CRT_CHAIN_IDX_END;
    /* -------------------------------------------------------------------------------- */
    /* set "IsPrmMRPDWatchDogActive" from PRM only for MRPD consumer                    */
    /* MRPD-Consumer-ADD has always the FrameID(x)                                      */
    /* FrameID(x+1) is not possibly here - it was checked in CheckConsumerAddParam      */
    /* -------------------------------------------------------------------------------- */
    switch (EDDP_HALGetPnipRevision(pDDB))
    {
        #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
        case EDDP_HAL_HW_PNIP_REV1:
        {
            //  not supported with PN-IP Rev1
        }
        break;
        #endif

        #if defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
        case EDDP_HAL_HW_PNIP_REV2:
        case EDDP_HAL_HW_PNIP_REV3:
        {
            if (EDDP_IS_IRT_REDUNDANT_FRAMEID(pConsumer->Param.FrameID))
            {
                EDDP_ASSERT_FALSE((pConsumer->Param.FrameID & 0x1));
                pConsumer->MrtDiag.IsPrmMRPDWatchDogActive = EDDP_PRMPDIRDataIsMRPDWatchDogActive (pDDB, pConsumer->Param.FrameID, &pConsumer->MrtDiag.PrmPDIRFrameDataIdx, LSA_TRUE);
            }
        }
        break;
        #endif
        

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTScoreboardSetupEntry(): FATAL -> Version(%d) of PNIP is unknown", 
                EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }

    /* -------------------------------------------------------------------------------- */
    /* Setup the TSB entry                                                              */
    /* -------------------------------------------------------------------------------- */
    TSB_Word1 = PNIP_CPM_TSB_WORD1_DEF_VALUE; /* init all with 0 as default */
    TSB_Word2 = PNIP_CPM_TSB_WORD2_DEF_VALUE; /* init all with 0 as default */

    /* -------------------------------------------------------------------------------- */
    /* TSB_Word1                                                                        */
    /* -------------------------------------------------------------------------------- */
    /*      - Bit(0-18)  : TSBRR set to CycleReductionRatio-1               */
    EDDP_SET_BIT_VALUE64(TSB_Word1,pConsumer->Param.HwCycleReductionRatio, PNIP_CPM_TSB_WORD1_TSBRR_COUNTER_MASK, PNIP_CPM_TSB_WORD1_TSBRR_COUNTER_SHIFT);

    /*      - Bit(19)    : DS_State_changed              remains 0          */
    /*      - Bit(20)    : DS_SystemRedundancy_changed   remains 0          */
    /*      - Bit(21)    : DS_RunStop_changed            remains 0          */
    /*      - Bit(22)    : DS_ProblemIndicator_changed   remains 0          */
    /*      - Bit(23)    : ReceivePhase_changed          remains 0          */

    /*      - Bit(24)    : DS_State                                         */
    if (pConsumer->ScoreBoard.EventStatusDefault & EDD_CSRT_CONS_EVENT_PRIMARY)
    {
        TSB_Word1 |= PNIP_CPM_TSB_WORD1_DS_STATE_MASK;
    }
    /*      - Bit(25)    : DS_Systemredundancy                              */
    if (pConsumer->ScoreBoard.EventStatusDefault & EDD_CSRT_CONS_EVENT_NO_PRIMARY_AR_EXISTS)
    {
        TSB_Word1 |= PNIP_CPM_TSB_WORD1_DS_SYS_RED_MASK;
    }
    /*      - Bit(26)    : DS_RunStop                                       */
    if (pConsumer->ScoreBoard.EventStatusDefault & EDD_CSRT_CONS_EVENT_RUN)
    {
        TSB_Word1 |= PNIP_CPM_TSB_WORD1_DS_RUNSTOP_MASK;
    }
    /*      - Bit(27)    : DS_ProblemIndicator                              */
    if (pConsumer->ScoreBoard.EventStatusDefault & EDD_CSRT_CONS_EVENT_STATION_OK)
    {
        TSB_Word1 |= PNIP_CPM_TSB_WORD1_DS_PROBL_IND_MASK;
    }
    /*      - Bit(28)    : Received_in_Red                                  */
    if (pConsumer->ScoreBoard.EventStatusDefault & EDD_CSRT_CONS_EVENT_ENTERING_RED)
    {
        TSB_Word1 |= PNIP_CPM_TSB_WORD1_RCVD_IN_DP2_MASK;
    }

    /*      - Bit(29..30): reserved           :    -                                                        */

    /*      - Bit(31)    : Autostop_en = 0    :    set if frame received, without old/new detection         */
    /*      - Bit(32)    : WDT_unknown = 0    :    flag for WatchDog-Timer                                  */
    /*      - Bit(33)    : DHT_unknown = 0    :    flag for DataHold-Timer                                  */
    /*      - Bit(34)    : MRT_unknown = 0    :    flag for MediaRedundancy-Timer                           */
    /*      - Bit(35)    : WDT_expired = 0    :    state of WatchDog-Timer                                  */
    //TSB_Word1 |= PNIP_CPM_TSB_WORD1_WDT_EXPIRED_MASK;
    /*      - Bit(36)    : DHT_expired = 0    :    state of DataHold-Timer                                  */
    if (pConsumer->ScoreBoard.EventStatusDefault & (EDD_CSRT_CONS_EVENT_MISS | EDD_CSRT_CONS_EVENT_MISS_NOT_STOPPED))
    {
        TSB_Word1 |= PNIP_CPM_TSB_WORD1_DHT_EXPIRED_MASK;
    }
    /*      - Bit(37)    : MRT_expired =0     :    state of MediaRedundancy--Timer                          */
    /*      - Bit(38)    : Frame_received                remains 0                                          */
    /*      - Bit(39)    : VALID    remains 0 -> will be set with Command ACW_ON                            */
    /*      - Bit(40-47) : WD_Timer remains 0 -> will be set with Command ACW_ON or SetToUnknown            */
    /*      - Bit(48-55) : DH_Timer remains 0 -> will be set with Command ACW_ON or SetToUnknown            */
    /*      - Bit(56-63) : MR_Timer remains 0 -> will be set with Command ACW_ON or SetToUnknown            */

    /* -------------------------------------------------------------------------------- */
    /* TSB_Word2                                                                        */
    /* -------------------------------------------------------------------------------- */
    /*      - Bit(0-18)  : TSBRR-Reload set to CycleReductionRatio-1        */
    EDDP_SET_BIT_VALUE64(TSB_Word2,pConsumer->Param.HwCycleReductionRatio, PNIP_CPM_TSB_WORD2_TSBRR_RELOAD_MASK , PNIP_CPM_TSB_WORD2_TSBRR_RELOAD_SHIFT);
    /*      - Bit(16..31): reserved           :    -                        */
    /*      - Bit(32..47): ACW_offset used for autostop                     */
    if (pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_DFP_ITEM)
    {
        // APICtrlRAMOffs = Offset >> 3
        EDDP_ASSERT_FALSE(pConsumer->DgIdx == EDDP_CRT_CHAIN_IDX_END);
        APICtrlRAMOffs = EDDP_CRTGetPnipAddrShift (pDDB, pDDB->CRT.Cons.pConsDgEntry[pConsumer->DgIdx].pHwAcwDgApi, EDDP_CRT_SHIFT_VALUE_3); // Address must be 64Bit aligned!
    }
    else
    {
        // APICtrlRAMOffs = Offset >> 3
        APICtrlRAMOffs = EDDP_CRTGetPnipAddrShift (pDDB, pConsumer->ConsAcwTypeInfo.pHwAcwApi, EDDP_CRT_SHIFT_VALUE_3); // Address must be 64Bit aligned!
    }
    EDDP_SET_BIT_VALUE64(TSB_Word2,APICtrlRAMOffs, PNIP_CPM_TSB_WORD2_ACW_OFFSET_MASK, PNIP_CPM_TSB_WORD2_ACW_OFFSET_SHIFT);
    /*      - Bit(48..63): reserved for SW    :    -                        */
    /* -------------------------------------------------------------------------------- */

    EDDP_HAL_MEM64_WRITE(pDDB, &pConsumer->pHwTSBApi->TSB_Word1, TSB_Word1);
    EDDP_HAL_MEM64_WRITE(pDDB, &pConsumer->pHwTSBApi->TSB_Word2, TSB_Word2);

    /* -------------------------------------------------------------------------------- */
    /* Setup the CSB entrys                                                             */
    /* -------------------------------------------------------------------------------- */
    CSB_Word1 = PNIP_CPM_CSB_WORD1_DEF_VALUE;

    /* WDT_expired=0                                                        */
    /* DHT_expired=0                                                        */
    /* MRT_expired=0                                                        */
    /* DS_State (1=Primary)                                                 */
    if (pConsumer->ScoreBoard.EventStatusDefault & EDD_CSRT_CONS_EVENT_PRIMARY)
    {
        CSB_Word1 |= PNIP_CPM_CSB_WORD1_DS_STATE_MASK;
    }
    /* DS_SystemRedundancy (0=no red)                                       */
    if (pConsumer->ScoreBoard.EventStatusDefault & EDD_CSRT_CONS_EVENT_NO_PRIMARY_AR_EXISTS)
    {
        CSB_Word1 |= PNIP_CPM_CSB_WORD1_DS_SYS_RED_MASK;
    }
    /* DS_RunStop (1=Run)                                                   */
    if (pConsumer->ScoreBoard.EventStatusDefault & EDD_CSRT_CONS_EVENT_RUN)
    {
        CSB_Word1 |= PNIP_CPM_CSB_WORD1_DS_RUNSTOP_MASK;
    }
    /* DS_ProblemIndicator (1=ok)                                           */
    if (pConsumer->ScoreBoard.EventStatusDefault & EDD_CSRT_CONS_EVENT_STATION_OK)
    {
        CSB_Word1 |= PNIP_CPM_CSB_WORD1_DS_PROBL_IND_MASK;
    }
    /* Received_in_Red (0=no dp2)                                           */
    if (pConsumer->ScoreBoard.EventStatusDefault & EDD_CSRT_CONS_EVENT_ENTERING_RED)
    {
        CSB_Word1 |= PNIP_CPM_CSB_WORD1_RCVD_IN_DP2_MASK;
    }
    /* FrameReceived=0                                                      */
    /* All Changedbits=0                                                    */

    EDDP_HAL_MEM32_WRITE(pDDB, &pConsumer->pHwCSB0Api->CSB_Word1,CSB_Word1);
    EDDP_HAL_MEM32_WRITE(pDDB, &pConsumer->pHwCSB1Api->CSB_Word1,CSB_Word1);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :  _EDDP_CRTConsumerQuitState                    +*/
/*+  Input                 :  EDDP_LOCAL_DDB_PTR_TYPE        pDDB           +*/
/*+                           EDDP_CRT_CONSUMER_PTR_TYPE     pCons          +*/
/*+                           LSA_UINT32                     Events         +*/
/*+                                                                         +*/
/*+  Result                :  LSA_VOID                                      +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DDB                                       (in) +*/
/*+  pCons      : Pointer to Consumer                                  (in) +*/
/*+  Events     : Eventflags which was indicatated to user by RQB      (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function sets all ActEvents to IndEvents (all events +*/
/*+               indicated to user. If SetToUnkown Flag was set we reset it+*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_VOID EDDP_LOCAL_FCT_ATTR _EDDP_CRTConsumerQuitState(
	EDDP_LOCAL_DDB_PTR_TYPE        pDDB,
    EDDP_CRT_CONSUMER_PTR_TYPE     pCons,
    LSA_UINT32                     Events)
{
    LSA_BOOL    bClrUnknown;

    EDDP_ASSERT_NO_NULL_PTR(pCons);
    LSA_UNUSED_ARG(pDDB);

    if (Events & (EDD_CSRT_CONS_EVENT_MISS | EDD_CSRT_CONS_EVENT_AGAIN | EDD_CSRT_CONS_EVENT_MISS_NOT_STOPPED))
    {
        pCons->ScoreBoard.WaitSetToUnknown = LSA_FALSE;
        bClrUnknown = LSA_TRUE;
    }
    else
    {
        bClrUnknown = LSA_FALSE;
    }

    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] _EDDP_CRTConsumerQuitState(ConsID=%d): Event=0x%X, ClrSetToUnknown=%d", 
        pCons->ConsumerId, Events, bClrUnknown);

    // ***** expect that current indication are signaled to the user and
    //       we can reject these events
    pCons->ScoreBoard.IndEvents = pCons->ScoreBoard.ActEvents;
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :  _EDDP_CRTConsumerActualizeEvents              +*/
/*+  Input                 :  EDDP_LOCAL_DDB_PTR_TYPE        pDDB           +*/
/*+                           EDDP_CRT_CONSUMER_PTR_TYPE     pCons          +*/
/*+                           LSA_UINT32                     CSBValue       +*/
/*+                                                                         +*/
/*+  Result                :  LSA_VOID                                      +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DDB                                       (in) +*/
/*+  pCons      : Pointer to Consumer                                  (in) +*/
/*+  CSBValue   : Value of Scoreboard entry from PNIP (CSB_Word1)      (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function actualizes the "ActEvents" within Consumer  +*/
/*+               management depending on Scoreboard values.                +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_VOID EDDP_LOCAL_FCT_ATTR _EDDP_CRTConsumerActualizeEvents(
	EDDP_LOCAL_DDB_PTR_TYPE        pDDB,
    EDDP_CRT_CONSUMER_PTR_TYPE     pCons,
    LSA_UINT32                     CSBValue,
    EDD_COMMON_MEM_U32_PTR_TYPE    pEvents)
{
    LSA_UINT32              EventStatus, Events;
    LSA_UINT16              FrameID;
    LSA_UINT64              TSB_Word1;

    EDDP_ASSERT_NO_NULL_PTR(pCons);
    EDDP_ASSERT_NO_NULL_PTR(pEvents);

    Events   = 0;
    FrameID = pCons->Param.FrameID;

    EDDP_CRT_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: _EDDP_CRTConsumerActualizeEvents(ConsumerID=%d, Consumer-State=0x%X, FrameID=0x%X, ConsumerFlags=0x%X, ScoreBoard-ActEvents=0x%X)", 
        pCons->ConsumerId, pCons->State, FrameID, pCons->ConsumerFlags, pCons->ScoreBoard.ActEvents);

    /* -------------------------------------------------------------------- */
    /* get actual Events. We change the changed ones within ActEvents        */
    /* -------------------------------------------------------------------- */
    EventStatus = pCons->ScoreBoard.ActEvents;

    /* -------------------------------------------------------------------- */
    /* MISS/AGAIN                                                            */
    /* -------------------------------------------------------------------- */
    /* Note: it is possible that both, AGAIN and DHT expired are set. If so  */
    /*       we use MISS. This may occur if the Scoreboard handling is       */
    /*       delayed for some reason.                                        */
    /* Note: We always use Autostop of Consumer. So after a MISS no more     */
    /*       consumer will be received! But if someone issues a SetToUnknown */
    /*       at this time we will get the following behaviour:               */
    /*       1. DHT not expired (because of reload of DHT by Hardware)       */
    /*       2. DHT expired (because no more frames come in)                 */
    /* -------------------------------------------------------------------- */
    // ***** check AGAIN change (AGAIN can only transit from 0->1)
    TSB_Word1 = EDDP_HAL_MEM64_READ(pDDB, &pCons->pHwTSBApi->TSB_Word1);

    /* -------------------------------------------------------------------- */
    /* AGAIN, MISS, MISS_NOT_STOPPED, DHT_EXPIRED                           */
    /* -------------------------------------------------------------------- */
    if (CSBValue & PNIP_CPM_CSB_WORD1_DHT_EXPIRED_CHD_MASK)
    {   // ***** AGAIN, MISS or MISS_NOT_STOPPED
        if (CSBValue & PNIP_CPM_CSB_WORD1_DHT_EXPIRED_MASK)
        {   // ***** MISS or MISS_NOT_STOPPED                
            if (PNIP_CPM_TSB_IS_VALID(TSB_Word1))
            {   // ***** MISS but not stopped --> MISS_NOT_STOPPED
                if (pCons->ScoreBoard.WaitSetToUnknown)
                {   // ***** fire event only on SetToUnknown
                    EventStatus &= ~(EDD_CSRT_CONS_EVENT_AGAIN | EDD_CSRT_CONS_EVENT_MISS);
                    EventStatus |=  EDD_CSRT_CONS_EVENT_MISS_NOT_STOPPED;
                    Events      |=  EDD_CSRT_CONS_EVENT_MISS_NOT_STOPPED;

                    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] _EDDP_CRTConsumerActualizeEvents(ConsumerID=%d, CSBValue=0x%X, FrameID=0x%X): EVENT_MISS_NOT_STOPPED",
                        pCons->ConsumerId, CSBValue, FrameID);
                }
            }
            else
            {   // ***** MISS detected by HW (DHT expired --> Autostop)
                EventStatus &= ~(EDD_CSRT_CONS_EVENT_AGAIN | EDD_CSRT_CONS_EVENT_MISS_NOT_STOPPED);
                EventStatus |=  EDD_CSRT_CONS_EVENT_MISS;
                Events      |=  EDD_CSRT_CONS_EVENT_MISS;

                // ***** Because of Autostop we set the Consumer to passive
                if /* no "old" SB event */
                    (pCons->State != EDDP_CRT_STATE_HW_PASSIVE)
                {
                    pCons->State = EDDP_CRT_STATE_HW_PASSIVE;
                    EDDP_ASSERT(pDDB->CRT.Cons.ConsumerActCnt);
                    pDDB->CRT.Cons.ConsumerActCnt--;
                }

                //if (pCons->ConsumerFlags & EDDP_CONS_FLAG_IS_SRD_ITEM)
                //{   // ***** SRD-Consumer: patching DataStatus to BACKUP
                //    CSBValue |= PNIP_CPM_CSB_WORD1_DS_STATE_CHD_MASK;
                //    CSBValue &= ~PNIP_CPM_CSB_WORD1_DS_STATE_MASK; // change to force EDD_CSRT_CONS_EVENT_BACKUP
                //    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H:--] _EDDP_CRTConsumerActualizeEvents(ConsumerID=%d, CSBValue=0x%X, FrameID=0x%X): EVENT_MISS (DHT_EXPIRED - SRD-Consumer: patching DataStatus to BACKUP)",
                //        pCons->ConsumerId, CSBValue, FrameID);
                //    // ***** write BACKUP into ConsumerScoreBoard (HW had disabled this entry)
                //    EDDP_HAL_MEM32_WRITE(pDDB, (EDD_COMMON_MEM_U32_PTR_TYPE) pCons->pHwCSB0Api, CSBValue);
                //    EDDP_HAL_MEM32_WRITE(pDDB, (EDD_COMMON_MEM_U32_PTR_TYPE) pCons->pHwCSB1Api, CSBValue);
                //    // ***** patching DataStatus to BACKUP within ApiCtrlRAM
                //    EDDP_CRTConsForceBackupDataStatusHwApi (pDDB, pCons);
                //}
                //else
                //{
                //    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H:--] _EDDP_CRTConsumerActualizeEvents(ConsumerID=%d, CSBValue=0x%X, FrameID=0x%X): EVENT_MISS (DHT_EXPIRED)",
                //        pCons->ConsumerId, CSBValue, FrameID);
                //}
                pCons->ConsumerFlags |= EDDP_CONS_FLAG_MISS_RCVD;  // store MISS information for: MISS and MISS_NOT_STOPPED event generated by SW

                // trigger MRT diagnosis only if this is MRPD consumer and IsPrmMRPDWatchDogActive is ON
                if ((EDDP_IS_IRT_REDUNDANT_FRAMEID(pCons->Param.FrameID)) && (pCons->MrtDiag.IsPrmMRPDWatchDogActive))
                {
                    // this consumer must be connected to PRM -> PDIRData.PDIRFrameData[x]
                    EDDP_ASSERT_FALSE(pCons->MrtDiag.PrmPDIRFrameDataIdx == EDDP_CRT_CHAIN_IDX_END);
                    // consumer is passivated, so trigger MRT diagnosis for this case:
                    // if a MRT diagnosis was fired as Appear, it must be fired again as DisAppear
                    EDDP_PRMDiagTriggerMRTDiagSingle (pDDB, pCons->MrtDiag.PrmPDIRFrameDataIdx, LSA_FALSE, LSA_FALSE, LSA_FALSE);
                }
            }
        }
        else
        {
            // ***** AGAIN
            EventStatus &= ~(EDD_CSRT_CONS_EVENT_MISS | EDD_CSRT_CONS_EVENT_MISS_NOT_STOPPED);
            EventStatus |=  EDD_CSRT_CONS_EVENT_AGAIN;
            Events      |=  EDD_CSRT_CONS_EVENT_AGAIN;

            pCons->ConsumerFlags |= EDDP_CONS_FLAG_AGAIN_RCVD;  // store information: frame was received once

            EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] _EDDP_CRTConsumerActualizeEvents(ConsumerID=%d, CSBValue=0x%X, FrameID=0x%X): EVENT_AGAIN",
                pCons->ConsumerId, CSBValue, FrameID);
        }
    }

    /* -------------------------------------------------------------------- */
    /* MRT_EXPIRED                                                          */
    /* -------------------------------------------------------------------- */
    if (CSBValue & PNIP_CPM_CSB_WORD1_MRT_EXPIRED_CHD_MASK)
    {
        LSA_BOOL        NewMrtExpired;

        EDDP_ASSERT_FALSE(!(EDDP_IS_IRT_REDUNDANT_FRAMEID(FrameID)));

        if (CSBValue & PNIP_CPM_CSB_WORD1_MRT_EXPIRED_MASK)
            NewMrtExpired = LSA_TRUE;   // expired
        else
            NewMrtExpired = LSA_FALSE;  // not expired

        EDDP_CRT_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] _EDDP_CRTConsumerActualizeEvents(ConsumerID=%d, CSBValue=0x%X, FrameID=0x%X, IsMrtDiagActive=0x%X): MRT_EXPIRED (old=0x%X, new=0x%X)", 
            pCons->ConsumerId, CSBValue, FrameID, pCons->MrtDiag.IsPrmMRPDWatchDogActive, pCons->MrtDiag.LastMrtExpired, NewMrtExpired);

        // trigger MRT diagnosis only if IsPrmMRPDWatchDogActive is ON and MRT_EXPIRED has changed
        if ((pCons->MrtDiag.IsPrmMRPDWatchDogActive) && (pCons->MrtDiag.LastMrtExpired != NewMrtExpired))
        {
            // this consumer must be connected to PRM -> PDIRData.PDIRFrameData[x]
            EDDP_ASSERT_FALSE(pCons->MrtDiag.PrmPDIRFrameDataIdx == EDDP_CRT_CHAIN_IDX_END);
            EDDP_PRMDiagTriggerMRTDiagSingle (pDDB, pCons->MrtDiag.PrmPDIRFrameDataIdx, NewMrtExpired, LSA_FALSE, LSA_FALSE);
        }

        // store state of NewMrtExpired in consumer
        pCons->MrtDiag.LastMrtExpired = NewMrtExpired;
    }

    /* -------------------------------------------------------------------- */
    /* DataStatus                                                           */
    /* ***** STATE: BACKUP(0) PRIMARY(1)                                    */
    /* -------------------------------------------------------------------- */
    if (CSBValue & PNIP_CPM_CSB_WORD1_DS_STATE_CHD_MASK)
    {
        if (CSBValue & PNIP_CPM_CSB_WORD1_DS_STATE_MASK)
        {
            EventStatus &= ~EDD_CSRT_CONS_EVENT_BACKUP;
            EventStatus |=  EDD_CSRT_CONS_EVENT_PRIMARY;
            Events      |=  EDD_CSRT_CONS_EVENT_PRIMARY;
            EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] _EDDP_CRTConsumerActualizeEvents(ConsumerID=%d, CSBValue=0x%X, FrameID=0x%X): EVENT_PRIMARY",
                pCons->ConsumerId, CSBValue, FrameID);
        }
        else
        {
            EventStatus &= ~EDD_CSRT_CONS_EVENT_PRIMARY;
            EventStatus |=  EDD_CSRT_CONS_EVENT_BACKUP;
            Events      |=  EDD_CSRT_CONS_EVENT_BACKUP;
            EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] _EDDP_CRTConsumerActualizeEvents(ConsumerID=%d, CSBValue=0x%X, FrameID=0x%X): EVENT_BACKUP",
                pCons->ConsumerId, CSBValue, FrameID);
        }
    }

    /* -------------------------------------------------------------------- */
    /* DataStatus                                                           */
    /* ***** Provider State: STOP(0) RUN(1)                                 */
    /* -------------------------------------------------------------------- */
    if (CSBValue & PNIP_CPM_CSB_WORD1_DS_RUNSTOP_CHD_MASK)
    {
        if (CSBValue & PNIP_CPM_CSB_WORD1_DS_RUNSTOP_MASK)
        {
            EventStatus &= ~EDD_CSRT_CONS_EVENT_STOP;
            EventStatus |=  EDD_CSRT_CONS_EVENT_RUN;
            Events      |=  EDD_CSRT_CONS_EVENT_RUN;
            EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] _EDDP_CRTConsumerActualizeEvents(ConsumerID=%d, CSBValue=0x%X, FrameID=0x%X): EVENT_RUN",
                pCons->ConsumerId, CSBValue, FrameID);
        }
        else
        {
            EventStatus &= ~EDD_CSRT_CONS_EVENT_RUN;
            EventStatus |=  EDD_CSRT_CONS_EVENT_STOP;
            Events      |=  EDD_CSRT_CONS_EVENT_STOP;
            EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] _EDDP_CRTConsumerActualizeEvents(ConsumerID=%d, CSBValue=0x%X, FrameID=0x%X): EVENT_STOP",
                pCons->ConsumerId, CSBValue, FrameID);
        }
    }

    /* -------------------------------------------------------------------- */
    /* DataStatus                                                           */
    /* ***** ProblemIndicator: PROBLEM(0) NORMAL(1)                         */
    /* -------------------------------------------------------------------- */
    if (CSBValue & PNIP_CPM_CSB_WORD1_DS_PROBL_IND_CHD_MASK)
    {
        if (CSBValue & PNIP_CPM_CSB_WORD1_DS_PROBL_IND_MASK)
        {
            EventStatus &= ~EDD_CSRT_CONS_EVENT_STATION_FAILURE;
            EventStatus |=  EDD_CSRT_CONS_EVENT_STATION_OK;
            Events      |=  EDD_CSRT_CONS_EVENT_STATION_OK;
            EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] _EDDP_CRTConsumerActualizeEvents(ConsumerID=%d, CSBValue=0x%X, FrameID=0x%X): EVENT_STATION_OK",
                pCons->ConsumerId, CSBValue, FrameID);
        }
        else
        {
            EventStatus &= ~EDD_CSRT_CONS_EVENT_STATION_OK;
            EventStatus |=  EDD_CSRT_CONS_EVENT_STATION_FAILURE;
            Events      |=  EDD_CSRT_CONS_EVENT_STATION_FAILURE;
            EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] _EDDP_CRTConsumerActualizeEvents(ConsumerID=%d, CSBValue=0x%X, FrameID=0x%X): EVENT_STATION_FAILURE",
                pCons->ConsumerId, CSBValue, FrameID);
        }
    }

    /* -------------------------------------------------------------------- */
    /* DataStatus                                                           */
    /* ***** SystemRedundancy: NO_REDUNANCY(0) REDUNDANCY(1)                */
    /* -------------------------------------------------------------------- */
    if (CSBValue & PNIP_CPM_CSB_WORD1_DS_SYS_RED_CHD_MASK)
    {
        if (CSBValue & PNIP_CPM_CSB_WORD1_DS_SYS_RED_MASK)
        {
            EventStatus &= ~EDD_CSRT_CONS_EVENT_PRIMARY_AR_EXISTS;
            EventStatus |=  EDD_CSRT_CONS_EVENT_NO_PRIMARY_AR_EXISTS;
            Events      |=  EDD_CSRT_CONS_EVENT_NO_PRIMARY_AR_EXISTS;
            EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] _EDDP_CRTConsumerActualizeEvents(ConsumerID=%d, CSBValue=0x%X, FrameID=0x%X): EVENT_NO_PRIMARY_AR_EXISTS",
                pCons->ConsumerId, CSBValue, FrameID);
        }
        else
        {
            EventStatus &= ~EDD_CSRT_CONS_EVENT_NO_PRIMARY_AR_EXISTS;
            EventStatus |=  EDD_CSRT_CONS_EVENT_PRIMARY_AR_EXISTS;
            Events      |=  EDD_CSRT_CONS_EVENT_PRIMARY_AR_EXISTS;
            EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] _EDDP_CRTConsumerActualizeEvents(ConsumerID=%d, CSBValue=0x%X, FrameID=0x%X): EVENT_PRIMARY_AR_EXISTS",
                pCons->ConsumerId, CSBValue, FrameID);
        }
    }

    /* -------------------------------------------------------------------- */
    /* Receive: ENTERING_RED			                                    */
    /* -------------------------------------------------------------------- */
    if (CSBValue & PNIP_CPM_CSB_WORD1_RCVD_IN_DP2_CHD_MASK)
    {
        if (CSBValue & PNIP_CPM_CSB_WORD1_RCVD_IN_DP2_MASK)
        {
            EventStatus |=  EDD_CSRT_CONS_EVENT_ENTERING_RED;
            Events      |=  EDD_CSRT_CONS_EVENT_ENTERING_RED;
            EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] _EDDP_CRTConsumerActualizeEvents(ConsumerID=%d, CSBValue=0x%X, FrameID=0x%X): EVENT_ENTERING_RED",
                pCons->ConsumerId, CSBValue, FrameID);
        }
        else
        {
			/* Event LEAVING_RED does not longer exist and this may not be reported by PN-IP to EDDP.                               */
            /* This is guaranteed by the following setting of Rcv-ACW: ACW.DP2Only=0 and ACW.DP2only_set_after_RiDP=1.              */
            /* This has the consequence that at first the consumer can receive both in the green and in the red phase.              */
            /* As soon as the frame arrives in the red phase and additional in the DP2 once (even if he was already                 */
            /* received in green), so the frame is only received by the PN-IP in the DP2 from this time on.                         */
            /* For the 2-CR consumer this state change is possibly: ENTERING_RED -> LEAVING_RED -> ENTERING_RED                     */
            /* This case is only possibly if the green and red frames are arriving on different ports (in ring), usually this is    */
            /* a wrong parametrization - MRPD and 2-CR is not supported.                                                            */
            EventStatus &= ~EDD_CSRT_CONS_EVENT_ENTERING_RED;
            EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CRTConsumerActualizeEvents(): ERROR -> EVENT_LEAVING_RED no longer supported (ConsumerID=%d, CSBValue=0x%X, FrameID=0x%X)",
				pCons->ConsumerId, CSBValue, FrameID);
        }
    }

    /* -------------------------------------------------------------------- */
    /* fill DebugInfo of this consumer if MISS is present                   */
    /* -------------------------------------------------------------------- */
    if (EventStatus & EDD_CSRT_CONS_EVENT_MISS)
    {
        /* ---------------------------------------------------------------- */
        /* DFP_Subframe and DFP_Subframe_CRC                                */
        /* DebugInfo[0] : Bit(0) = DFP_Subframe (yes/no)                    */
        /* DebugInfo[0] : Bit(1) = DFP_Subframe_CRC (yes/no)                */
        /* ---------------------------------------------------------------- */
        /* CycleCounter:                                                    */
        /* - not DFP         : APDU-CycleCounter(16Bit) from frame          */
        /* - DFP without CRC : APDU-CycleCounter(16Bit) from pack frame     */
        /* - DFP with CRC    : DG-CycleCounter(8Bit) from Subframe          */
        /* DebugInfo[2]      : CycleCounter (high)                          */
        /* DebugInfo[3]      : CycleCounter (low)                           */
        /* ---------------------------------------------------------------- */
        LSA_UINT32                              DebugInfoTemp   = 0;
        LSA_UINT16                              CycleCounter    = 0;
        LSA_UINT16                              AcwContainerIdx, AcwMaxNumber;
        EDDP_CRT_CONSUMER_CONTAINER_PTR_TYPE    pConsContainerEntry;
        EDDP_CRT_CONS_ACW_INFO_PTR_TYPE         pAcwPtr;
        EDDP_CRT_CONSUMER_DG_PTR_TYPE           pDgEntry;

        // DFP or not DFP
        if (pCons->ConsumerFlags & EDDP_CONS_FLAG_IS_DFP_ITEM)
        {
            /* ---------------------------------------------------------------- */
            /* DFP_Subframe or DFP_Subframe_CRC                                 */
            /* ---------------------------------------------------------------- */
            // this DFP consumer must be connected to DG
            EDDP_ASSERT_FALSE(pCons->DgIdx >= pDDB->CRT.CfgParam.ConsumerDGMaxNumber);
            // init pointer to this DG
            pDgEntry = &pDDB->CRT.Cons.pConsDgEntry[pCons->DgIdx];
            // this DG must be connected to a pack group (ACW container)
            EDDP_ASSERT_FALSE(pDgEntry->DgGrpIdx >= pDDB->CRT.CfgParam.ConsumerContainerMaxNumber);
            // init pointer to this pack group (ACW container)
            pConsContainerEntry = &pDDB->CRT.Cons.pConsContainerEntry[pDgEntry->DgGrpIdx];
            // check whether this ACW is a container (pack group)
            pAcwPtr = (EDDP_CRT_CONS_ACW_INFO_PTR_TYPE) pConsContainerEntry;
            EDDP_ASSERT_FALSE(!(pAcwPtr->IsAcwContainer));

            // DFP_Subframe (yes)
            pCons->ScoreBoard.DebugInfo |= EDDP_CRT_IND_PROVIDE_DEBUGINFO_DFP_SUBFRAME_MASK;

            // with CRC ?
            if (pConsContainerEntry->HasChksum)
            {
                // DFP with CRC: DG-CycleCounter(8Bit) from Subframe
                pCons->ScoreBoard.DebugInfo |= EDDP_CRT_IND_PROVIDE_DEBUGINFO_DFP_SUBFRAME_CRC_MASK;
                CycleCounter = EDDP_HAL_MEM8_READ(pDDB, &pDgEntry->pHwDgStatusApi->CycleCounter);
            }
            else
            {
                // DFP without CRC: APDU-CycleCounter(16Bit) from pack frame
                // get and check the AcwContainerIdx
                AcwContainerIdx = pConsContainerEntry->ConsAcwTypeInfo.AcwIdx;
                AcwMaxNumber    = pDDB->CRT.CfgParam.ConsumerMaxNumber;
                AcwMaxNumber   += pDDB->CRT.CfgParam.ConsumerContainerMaxNumber;
                EDDP_ASSERT_FALSE(AcwContainerIdx >= AcwMaxNumber);
                CycleCounter = EDDP_SWAP16(EDDP_HAL_MEM16_READ(pDDB, &pDDB->CRT.Cons.pHwBaseApduApi[AcwContainerIdx].CycleCounter));
            }
        }
        else
        {
            /* ---------------------------------------------------------------- */
            /* Not DFP: APDU-CycleCounter(16Bit) from frame                     */
            /* ---------------------------------------------------------------- */
            CycleCounter = EDDP_SWAP16(EDDP_HAL_MEM16_READ(pDDB, &pCons->ConsAcwTypeInfo.pHwApduApi->CycleCounter));
        }

        /* ---------------------------------------------------------------- */
        /* store CycleCounter in DebugInfo                                  */
        /* ---------------------------------------------------------------- */
        // DebugInfo[2] : CycleCounter (high)
        DebugInfoTemp = (LSA_UINT8) EDDP_GET_BIT_VALUE(CycleCounter, 0xFF00, 8);
        EDDP_SHIFT_VALUE_LEFT(DebugInfoTemp, 16);
        pCons->ScoreBoard.DebugInfo |= DebugInfoTemp;
        // DebugInfo[3] : CycleCounter (low)
        DebugInfoTemp = (LSA_UINT8) EDDP_GET_BIT_VALUE(CycleCounter, 0x00FF, 0);
        EDDP_SHIFT_VALUE_LEFT(DebugInfoTemp, 24);
        pCons->ScoreBoard.DebugInfo |= DebugInfoTemp;
    }

    /* -------------------------------------------------------------------- */
    /* Write back the acutal EventStatus                                    */
    /* -------------------------------------------------------------------- */
    pCons->ScoreBoard.ActEvents = EventStatus;
    
    /* -------------------------------------------------------------------- */
    /* return Events                                                        */
    /* -------------------------------------------------------------------- */
    *pEvents = Events;
    
    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: _EDDP_CRTConsumerActualizeEvents(): new ScoreBoard-ActEvents=0x%X",
        pCons->ScoreBoard.ActEvents);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :  EDDP_CRTConsumerCheckState                    +*/
/*+  Input                 :  EDDP_CRT_CONSUMER_PTR_TYPE     pCons          +*/
/*+  Output                   EDDP_COMMON_MEM_U32_PTR_TYPE  *pEvents        +*/
/*+                                                                         +*/
/*+  Result                :  LSA_BOOL                                      +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pCons      : Pointer to Consumer                                       +*/
/*+  pEvents    : Address of variable for new consumer events detected      +*/
/*+                                                                         +*/
/*+  Result     : LSA_FALSE if no event was present.                        +*/
/*+               LSA_TRUE  if event was present.                           +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Looks if consumer indication events are present which     +*/
/*+               have not been signaled to user yet.                       +*/
/*+                                                                         +*/
/*+               The function uses ActEvents und IndEvents within Consumer +*/
/*+               management.                                               +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerCheckState(
    EDDP_CRT_CONSUMER_PTR_TYPE          pCons,
    EDD_COMMON_MEM_U32_PTR_TYPE         pEvents)
{
    LSA_UINT32      ActEvents, DiffEvents;
    LSA_BOOL        Status;
    LSA_UINT32      Events;

    EDDP_ASSERT_NO_NULL_PTR(pCons);

    Status      = LSA_FALSE;
    ActEvents   = pCons->ScoreBoard.ActEvents;
    DiffEvents  = (ActEvents ^ pCons->ScoreBoard.IndEvents);
    Events      = 0;

    if ((DiffEvents) || (pCons->ScoreBoard.WaitSetToUnknown))   // some changes ?
    {
        // ***** check MISS/AGAIN change
        /* If SetToUnknown we have to signal MISS or AGAIN independend of   */
        /* actual State of Consumer but only if we got a new MISS/AGAIN     */
        /* event after issuing SetToUnknown. So we have to wait till we     */
        /* got a new MISS/AGAIN and dont signal an "old" event.             */
        if (pCons->ScoreBoard.WaitSetToUnknown)
        {   // ***** if settounknown active look to active events (miss/again) only !!!!
            if (ActEvents & (EDD_CSRT_CONS_EVENT_MISS | EDD_CSRT_CONS_EVENT_AGAIN | EDD_CSRT_CONS_EVENT_MISS_NOT_STOPPED))
            {   // ***** detect event change
                Events |= ActEvents & (EDD_CSRT_CONS_EVENT_MISS | EDD_CSRT_CONS_EVENT_AGAIN | EDD_CSRT_CONS_EVENT_MISS_NOT_STOPPED);
                Status = LSA_TRUE;
            }
        }
        else
        {
            if (DiffEvents & (EDD_CSRT_CONS_EVENT_MISS | EDD_CSRT_CONS_EVENT_AGAIN | EDD_CSRT_CONS_EVENT_MISS_NOT_STOPPED))
            {   // ***** detect event change
                Events |= ActEvents & (EDD_CSRT_CONS_EVENT_MISS | EDD_CSRT_CONS_EVENT_AGAIN | EDD_CSRT_CONS_EVENT_MISS_NOT_STOPPED);
                Status = LSA_TRUE;
            }
        }

        // ***** check STOP/RUN change
        if (DiffEvents & (EDD_CSRT_CONS_EVENT_RUN | EDD_CSRT_CONS_EVENT_STOP))
        {   // ***** detect event change
            Events |= ActEvents & (EDD_CSRT_CONS_EVENT_RUN | EDD_CSRT_CONS_EVENT_STOP);
            Status = LSA_TRUE;
        }

        // ***** check StationFailure change
        if (DiffEvents & (EDD_CSRT_CONS_EVENT_STATION_OK | EDD_CSRT_CONS_EVENT_STATION_FAILURE))
        {   // ***** detect event change
            Events |= ActEvents & (EDD_CSRT_CONS_EVENT_STATION_OK | EDD_CSRT_CONS_EVENT_STATION_FAILURE);
            Status = LSA_TRUE;
        }

        // ***** check PRIMARY/BACKUP change
        if (DiffEvents & (EDD_CSRT_CONS_EVENT_PRIMARY | EDD_CSRT_CONS_EVENT_BACKUP))
        {
            Events |= ActEvents & (EDD_CSRT_CONS_EVENT_PRIMARY | EDD_CSRT_CONS_EVENT_BACKUP);
            Status = LSA_TRUE;
        }

        // ***** check ENTERING_RED change
        if (DiffEvents & (EDD_CSRT_CONS_EVENT_ENTERING_RED))
        {   // ***** detect event change
            Events |= ActEvents & (EDD_CSRT_CONS_EVENT_ENTERING_RED);
            Status = LSA_TRUE;
        }

        // ***** check SYSTEM_REDUNDANCY change
        if (DiffEvents & (EDD_CSRT_CONS_EVENT_NO_PRIMARY_AR_EXISTS | EDD_CSRT_CONS_EVENT_PRIMARY_AR_EXISTS))
        {   // ***** detect event change
            Events |= ActEvents & (EDD_CSRT_CONS_EVENT_NO_PRIMARY_AR_EXISTS | EDD_CSRT_CONS_EVENT_PRIMARY_AR_EXISTS);
            Status = LSA_TRUE;
        }
    }

    *pEvents = Events;

    EDDP_CRT_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTConsumerCheckState(ConsumerID=%d, Events=0x%X)", 
        pCons->ConsumerId, Events);

    return (Status);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTConsumerCheckIndication             +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+  Result                :    --                                          +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DDB                                       (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Checks all consumers for pending indications and indicate +*/
/*+               Called after providing new indication resources if        +*/
/*+               ConsIndicationPending is set.                             +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerCheckIndication(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDDP_CRT_CONSUMER_PTR_TYPE              pCons;
    LSA_UINT16                              ConsIdx;
    LSA_BOOL                                IndResourcePresent;
    LSA_UINT32                              Events;

    if (!pDDB->CRT.IndProvide.ConsIndicationPending) return;

    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CRTConsumerCheckIndication()");

    IndResourcePresent = (pDDB->CRT.IndProvide.RqbQueue.Count ? LSA_TRUE: LSA_FALSE);

    if (IndResourcePresent)
    {
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTConsumerCheckIndication(): Consumer event is pending");

        // ***** we have at least one Indication-RQB and can clear our indication flag
        pDDB->CRT.IndProvide.ConsIndicationPending = LSA_FALSE;

        /* get index of first consumer */
        ConsIdx = pDDB->CRT.Cons.ConsumerUsedList.BeginIdx;

        /*------------------------------------------------------------------------*/
        /* loop until all used consumers which are active are checked or we have  */
        /* no more indication resources..                                         */
        /*------------------------------------------------------------------------*/
        while ((ConsIdx != EDDP_CRT_CHAIN_IDX_END) && (IndResourcePresent))
        {
            EDDP_ASSERT_FALSE(ConsIdx >= pDDB->CRT.CfgParam.ConsumerMaxNumber);

            pCons = &pDDB->CRT.Cons.pConsEntry[ConsIdx];

            /* Now check Scoreboard for all active consumer, include passive consumer for pending MISS event */
            if ((pCons->State == EDDP_CRT_STATE_HW_ACTIVE) || (pCons->State == EDDP_CRT_STATE_HW_PASSIVE))
            {
                /* check if we have events to be signaled to user.. */
                if (EDDP_CRTConsumerCheckState(pCons, &Events))
                {
                    /* fill indication into requestblock (function will indicate */
                    /* it to user if the rqb is full!                            */
                    if ( EDDP_CRTFillIndication( pDDB,
                                                 pCons->Param.UserID,
                                                 Events,
                                                 pCons->ScoreBoard.ActEvents,
                                                 EDDP_CRTGetConsCycleCounterForIndication(pDDB, ConsIdx),
                                                 pCons->ScoreBoard.DebugInfo))
                    {
                        /* Indication was written into the Ind.-RQB. actualize state */
                        _EDDP_CRTConsumerQuitState (pDDB, pCons, Events);
                    }
                    else
                    {
                        /* no more indication resources present, skip further tests */
                        IndResourcePresent = LSA_FALSE;
                        /* if we havent enough resources we set a flag that indications pending..*/
                        pDDB->CRT.IndProvide.ConsIndicationPending = LSA_TRUE;
                    }
                }
            }
            else
            {
                if (pCons->State == EDDP_CRT_STATE_SW_USED)
                {   // ***** special use case for event MISS + NOT_STOPPED
                    Events = (pCons->ScoreBoard.ActEvents ^ pCons->ScoreBoard.IndEvents) & (EDD_CSRT_CONS_EVENT_MISS | EDD_CSRT_CONS_EVENT_MISS_NOT_STOPPED);

                    if (Events)
                    {
                        /* fill indication into requestblock (function will indicate */
                        /* it to user if the rqb is full!                            */
                        if ( EDDP_CRTFillIndication( pDDB,
                                                     pCons->Param.UserID,
                                                     Events,
                                                     pCons->ScoreBoard.ActEvents,
                                                     EDDP_CRTGetConsCycleCounterForIndication(pDDB, ConsIdx),
                                                     pCons->ScoreBoard.DebugInfo))
                        {
                            /* Indication was written into the Ind.-RQB. actualize state */
                            _EDDP_CRTConsumerQuitState (pDDB, pCons, Events);
                        }
                        else
                        {
                            /* no more indication resources present, skip further tests */
                            IndResourcePresent = LSA_FALSE;
                            /* if we havent enough resources we set a flag that indications pending..*/
                            pDDB->CRT.IndProvide.ConsIndicationPending = LSA_TRUE;
                        }
                    }
                }
            }

            /* Next Consumer to check */
            ConsIdx = pCons->UsedFreeLink.Next;

        } /* while */

        /* ----------------------------------------------------------------------*/
        /* Finish the indication request handling. This will indicate the rqb if */
        /* something is filled (necessary if the rqb is not full)                */
        /* ----------------------------------------------------------------------*/
        EDDP_CRTIndProvideFinishSingle(pDDB);
    }
    else
    {
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTConsumerCheckIndication(): missing Indication-RQB!");
    }

    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTConsumerCheckIndication()");
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTSetConsumerToUnknown                +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             EDDP_CRT_CONSUMER_PTR_TYPE     pCons        +*/
/*+                             LSA_BOOL                       WithSBC      +*/
/*+  Result                :    ---                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DDB                                       (in) +*/
/*+  pCons      : Pointer to Consumer                                  (in) +*/
/*+  WithSBC    : 0: without ScoreBoardChange, 1: with ScoreBoardChange(in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Function called for Consumer (pCons) to set internal      +*/
/*+               members for service SetToUnknown.                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTSetConsumerToUnknown(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_CRT_CONSUMER_PTR_TYPE  pConsumer,
    LSA_BOOL                    WithSBC)
{
    EDDP_ASSERT_NO_NULL_PTR(pConsumer);

    if (WithSBC)
    {
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTSetConsumerToUnknown(): clr/process old events with CSB-change" );
        EDDP_CRTScoreboardHandler (pDDB);
    }

    // ***** clear indicated events DHT
    pConsumer->ScoreBoard.IndEvents &= ~(EDD_CSRT_CONS_EVENT_AGAIN | EDD_CSRT_CONS_EVENT_MISS | EDD_CSRT_CONS_EVENT_MISS_NOT_STOPPED);

    if ((pConsumer->State != EDDP_CRT_STATE_HW_ACTIVE) && WithSBC)
    {   // ***** create miss/miss_not_stopped event by SW
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTSetConsumerToUnknown(ConsumerFlags=0x%X): SRV_UNKNOWN miss (off)",
            pConsumer->ConsumerFlags);
        pConsumer->ScoreBoard.WaitSetToUnknown = LSA_TRUE;

        // ***** set event depending on earlier MISS event
        if (pConsumer->ConsumerFlags & EDDP_CONS_FLAG_MISS_RCVD)
        {   // ***** retrun MISS because received from HW some thimes before
            pConsumer->ScoreBoard.ActEvents &= ~(EDD_CSRT_CONS_EVENT_AGAIN | EDD_CSRT_CONS_EVENT_MISS_NOT_STOPPED);
            pConsumer->ScoreBoard.ActEvents |=  EDD_CSRT_CONS_EVENT_MISS;
        }
        else
        {   // ***** return NOT_STOPPED because no MISS seen before
            pConsumer->ScoreBoard.ActEvents &= ~(EDD_CSRT_CONS_EVENT_AGAIN | EDD_CSRT_CONS_EVENT_MISS);
            pConsumer->ScoreBoard.ActEvents |=  EDD_CSRT_CONS_EVENT_MISS_NOT_STOPPED;
        }

        // ***** set flag for pending indication events
        pDDB->CRT.IndProvide.ConsIndicationPending      = LSA_TRUE;
        // ***** the SetToUnknown indication will be sent after ConsumerControl confirmation
        pDDB->CRT.Cons.RequestFinishConsumerID    = pConsumer->ConsumerId;
    }
    else
    {
        pConsumer->ScoreBoard.WaitSetToUnknown = LSA_TRUE;
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTScoreboardHandler                   +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+  Result                :    ---                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DDB                                       (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Function called on Consumer Scorboard events to check for +*/
/*+               Consumer state changes.                                   +*/
/*+                                                                         +*/
/*+               If a change of consumer State is detected it will be      +*/
/*+               indicated to the user. If no indication resource is       +*/
/*+               present, the event will be signaled if a new resource is  +*/
/*+               provided to the edd. The actual consumer state and the    +*/
/*+               last indicated consumer state is stored within consumer   +*/
/*+               management.                                               +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define EDDP_CRT_AGAIN_IND_DELAY_IN_NS  3584
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTScoreboardHandler(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDDP_CRT_CONSUMER_PTR_TYPE              pCons;
    LSA_UINT32                              CSBSelectOld,CSBSelectNew;
    PNIP_CPM_CSB_PTR_TYPE                   pCSB;
    LSA_UINT16                              ConsIdx;
    LSA_UINT32                              CSBValue;
    LSA_BOOL                                IndResourcePresent;
    LSA_UINT32                              Events;
#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
    LSA_UINT32                              HwNsTimer = 0;
    LSA_BOOL                                AgainFromHW = LSA_FALSE;
#endif
    LSA_BOOL                                SomethingChanged;
    LSA_UINT64                              HW_Word64;

    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CRTScoreboardHandler()");

    // **** if scoreboard handler was blocked the IRQ-Handler
    //      than we can clear this flag here because we handle the ScoreBoardEvent now !!!
    pDDB->CRT.Cons.SBEventPending = LSA_FALSE;

    /*-----------------------------------------------------------------------*/
    /* Check if new scoreboard events present by issuing a scoreboard change */
    /*-----------------------------------------------------------------------*/
    /* read Control_Reg CSBSelect before change */
    CSBSelectOld = EDDP_HAL_REG32_READ(pDDB, pDDB->CRT.Cons.PNIPCSBControlReg) & PNIP_REG_CSB_CONTROL_IFA__MSK_CSB_SELECT;
    /* initiate Scoreboard change     */
    EDDP_HAL_REG32_WRITE (pDDB, pDDB->CRT.Cons.PNIPCSBControlReg,PNIP_REG_CSB_CONTROL_IFA__MSK_CSB_CHANGE_ENABLE);
    /* read Control_Reg CSBSelect after change */
    CSBSelectNew = EDDP_HAL_REG32_READ(pDDB, pDDB->CRT.Cons.PNIPCSBControlReg) & PNIP_REG_CSB_CONTROL_IFA__MSK_CSB_SELECT;  

    /*-----------------------------------------------------------------------*/
    /* if CSBSelect change we have a new scoreboard                          */
    /*-----------------------------------------------------------------------*/
    if (CSBSelectNew == CSBSelectOld)
    {
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTScoreboardHandler(): CSBSelect has not changed: CSBSelectOld=0x%X, CSBSelectNew=0x%X", 
            CSBSelectOld, CSBSelectNew);
    }
    else
    {
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTScoreboardHandler(): CSBSelect has changed: CSBSelectOld=0x%X, CSBSelectNew=0x%X", 
            CSBSelectOld, CSBSelectNew);

#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
        if ( EDDP_HAL_HW_PNIP_REV1 == EDDP_HALGetPnipRevision(pDDB) )
        {
            HwNsTimer           = EDDP_HALGetNsCounter(pDDB);
        }
#endif
        
        IndResourcePresent  = (pDDB->CRT.IndProvide.RqbQueue.Count ? LSA_TRUE:LSA_FALSE);
        SomethingChanged    = LSA_FALSE;

        /* Select CSB depending on CSBSelect */
        if (CSBSelectNew == 0) 
            pCSB = pDDB->CRT.Cons.pHwBaseCSB1Api; /* PNIP uses CSB0, so we use CSB1 */
        else
            pCSB = pDDB->CRT.Cons.pHwBaseCSB0Api; /* PNIP uses CSB1, so we use CSB0 */

        /* Now check Scoreboard for all active consumer */
        ConsIdx = pDDB->CRT.Cons.ConsumerUsedList.BeginIdx;

        while (ConsIdx != EDDP_CRT_CHAIN_IDX_END)
        {
            EDDP_ASSERT_FALSE(ConsIdx >= pDDB->CRT.CfgParam.ConsumerMaxNumber);
            pCons = &pDDB->CRT.Cons.pConsEntry[ConsIdx];

            /* Read value from CSB. The Consumer IDX is index into the table */
            CSBValue = EDDP_HAL_MEM32_READ(pDDB, ((EDD_COMMON_MEM_U32_PTR_TYPE)&pCSB[ConsIdx]));

            EDDP_CRT_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTScoreboardHandler(): ConsIdx=%d, Consumer-State=0x%X, FrameID=0x%X, ConsumerFlags=0x%X, CSBValue=0x%X", 
                ConsIdx, pCons->State, pCons->Param.FrameID, pCons->ConsumerFlags, CSBValue);

            if (pCons->State == EDDP_CRT_STATE_HW_ACTIVE)
            {
                /* is something changed ? */
                if (CSBValue & PNIP_CPM_CSB_WORD1_CHANGED_MASK)
                {
                    SomethingChanged = LSA_TRUE; 

                    /* Actualize the EventStatus for this Consumer in pCons->ScoreBoard.ActEvents */
                    _EDDP_CRTConsumerActualizeEvents (pDDB, pCons, CSBValue, &Events);

                    // ***** check falling into RED if running in 2CR mode 
                    if (pCons->ConsumerFlags & EDDP_CONS_FLAG_IS_2CR_MODE)
                    {
                        if (CSBValue & PNIP_CPM_CSB_WORD1_RCVD_IN_DP2_CHD_MASK)
                        {
                            if (CSBValue & PNIP_CPM_CSB_WORD1_RCVD_IN_DP2_MASK)
                            {
                                HW_Word64 = EDDP_HAL_MEM64_READ(pDDB, &pCons->pHwTSBApi->TSB_Word2);
                                /*      - Bit(0-18)  : TSBRR-Reload set to CycleReductionRatio-1         */
                                EDDP_SET_BIT_VALUE64(HW_Word64,pCons->Param.HwCycleReductionRatio, PNIP_CPM_TSB_WORD2_TSBRR_RELOAD_MASK , PNIP_CPM_TSB_WORD2_TSBRR_RELOAD_SHIFT);
                                EDDP_HAL_MEM64_WRITE(pDDB, &pCons->pHwTSBApi->TSB_Word2, HW_Word64);
                                EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTScoreboardHandler(RED, 2-CR): ConsumerID=%d, HwCycleReductionRatio=%d, Time=%d us)", 
                                    pCons->ConsumerId, pCons->Param.HwCycleReductionRatio, pDDB->SWI.CycleInfo.CycleBaseFactor * (pCons->Param.HwDataHoldFactor-1) * (pCons->Param.HwCycleReductionRatio+1));
                            }
                        }
                    }

                    if (IndResourcePresent && (0 != Events))
                    {
#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
                        if ( EDDP_HAL_HW_PNIP_REV1 == EDDP_HALGetPnipRevision(pDDB) )
                        {
                            if (Events & EDD_CSRT_CONS_EVENT_AGAIN)
                            {
                                AgainFromHW = LSA_TRUE;
                            }
                        }
#endif
                        /* Consumer Events present. Try signaling to User */
                        if ( EDDP_CRTFillIndication( pDDB,
                                                     pCons->Param.UserID,
                                                     Events,
                                                     pCons->ScoreBoard.ActEvents,
                                                     EDDP_CRTGetConsCycleCounterForIndication(pDDB, ConsIdx),
                                                     pCons->ScoreBoard.DebugInfo))
                        {
                            /* Indication was signaled to user --> clear current event flags */
                            _EDDP_CRTConsumerQuitState (pDDB, pCons, Events);
                        }
                        else
                        {
                            /* No more free Indication Resources --> set later ConsIndicationPending */
                            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTScoreboardHandler(): No more indication Resources!");
                            IndResourcePresent = LSA_FALSE;
                        }
                    }
                }
            }

            /* Next Consumer to check */
            ConsIdx = pCons->UsedFreeLink.Next;
        } // while()

        /* --------------------------------------------------------------------------*/
        /* Finish the indication request handling. This will indicate the rqb if     */
        /* something is filled.                                                      */
        /* --------------------------------------------------------------------------*/
        if (SomethingChanged)
        {
            #ifdef EDDP_MESSAGE
            #ifdef EDDP_PRAGMA_MESSAGE
            #pragma EDDP_PRAGMA_MESSAGE("Workaround for PNIP-Revision(1) - AP01170064: Konsistenz der CSB-Meldung und den zugehoerigen IO-Daten (Again-Meldung)")
            #endif /* EDDP_PRAGMA_MESSAGE */
            #endif /* EDDP_MESSAGE */
            #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
            if ( EDDP_HAL_HW_PNIP_REV1 == EDDP_HALGetPnipRevision(pDDB) )
            {
                if (AgainFromHW)
                {   // ***** wait upto 3584ns after ScoreboardChange for AGAIN event only
                    //          --> 24ns + (2 PPM 8Bursts mit je 6 Waits im PeriF) AHB-Waits = 300ns 
                    //          --> 300ns + (448ns je 32 Byte beim max. 255 Bytes) AHB-Waits = 3584ns 
                    //       Note: delay of 20460ns not relevant here, because no support for RToverUDP
                    //          --> 300ns + (448ns je 32 Byte beim max. 1440 Bytes) AHB-Waits = 20460ns 
                    HwNsTimer = EDDP_HALDiffNsCounter(pDDB, HwNsTimer);

                    if (HwNsTimer < EDDP_CRT_AGAIN_IND_DELAY_IN_NS)
                    {   
                        EDDP_HALWaitNsTicks(pDDB, EDDP_CRT_AGAIN_IND_DELAY_IN_NS - HwNsTimer);
                    }
                }
            }
            #endif

            EDDP_CRTIndProvideFinishSingle (pDDB);

            if (!IndResourcePresent)
            {
                /* if we havent enough resources we set a flag that indications pending ..   */
                pDDB->CRT.IndProvide.ConsIndicationPending = LSA_TRUE;
            }
        }
    }

    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTScoreboardHandler()");
}
