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
/*  F i l e               &F: eddp_core_gen.c                           :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP general functions                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  23.10.08    AB    initial version.                                       */
/*  24.02.09    lrg   EDDP_K32RemotePortState                                */
/*  05.05.09    JS    Removed PhyTxDelay100MBit, PhyRxDelay100MBit,          */
/*                            MACTxDelay100MBit and MACRxDelay100MBit from   */
/*                            EDD_SRV_GET_PORT_PARAMS                        */
/*  16.09.09    lrg   lrg0809: EDDP_LINK_IND_PROVIDE_EXTENDED, MAC_upper,    */
/*                             SyncId0_Topo                                  */
/*  18.08.10    JS    Added "TraceIdx" init.                                 */
/*  24.11.10    JS    changes to prevent compiler warnings                   */
/*  17.01.11    JS    changed EDD_SRV_GET_STATISTICS structure (sMIB)        */
/*                    added   EDD_SRV_GET_STATISTICS_ALL                     */
/*  17.01.11    JS    added MaxInterfaceCntOfAllEDD and MaxPortCntOfAllEDD   */
/*  06.02.15    TH    Added PhyStatus for EDD_SRV_GET_PORT_PARAMS            */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID  9                 
#define EDDP_MODULE_ID     LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_CORE_GEN */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"            /* edd headerfiles */
#include "eddp_int.h"            /* internal header */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID) 

/*===========================================================================*/
/*                                global Variables                           */
/*===========================================================================*/
#ifdef EDDP_CFG_LOCAL_MEM_STATIC
    // Note: EDDP_CFG_MAX_CHANNELS is over all interfaces !!!
    EDDP_LOCAL_MEM_ATTR EDDP_HDB_GEN_TYPE       g_EDDP_HDB_GEN[EDDP_CFG_MAX_CHANNELS];
#endif


/*===========================================================================*/
/*                                external functions                         */
/*===========================================================================*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GEN_GetPortStatistics                  +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE            pDDB     +*/
/*+                             EDDP_LOCAL_GET_STATISTICS_PTR_TYPE pStats   +*/
/*+                             LSA_UINT16                         HwPortID +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pStats     : destination for statistic                                 +*/
/*+  HwPortID   : HwPortID (1..EDDP_MAX_PORT_CNT)                           +*/
/*+                                                                         +*/
/*+  Results    : LSA_VOID                                                  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Utility function for EDDP_GEN_RequestXXXStatistics().     +*/
/*+                                                                         +*/
/*+               Note: values in pStats must be initialized or Clear=1 !!! +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GEN_GetPortStatistics(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_LOCAL_GET_STATISTICS_PTR_TYPE      pStats,
    LSA_UINT32                              HwPortID)
{
#if(PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200P)
    LSA_UINT32                              PortIDX;
    PNIP_STATISTIC_PORTX_STRUCT_PTR_TYPE    pStatistic;
    PNIP_STATISTIC_MEM_STRUCT_PTR_TYPE      pStatistic_Mem;
    LSA_UINT32                              RequestedCounters_loc;
    LSA_UINT32                              TmpMulticastPkts = 0;
    LSA_UINT32                              TmpBroadcastPkts = 0;

    EDDP_ASSERT_NO_NULL_PTR(pStats);

    if ((HwPortID > pDDB->SWI.PortCnt) || (HwPortID == 0))
    {
        EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_GEN_GetPortStatistics(): FATAL -> invalid HwPortID(%d), PortCnt(%d)!", 
            HwPortID, pDDB->SWI.PortCnt);
        EDDP_FATAL(EDD_STS_ERR_NOT_ALLOWED);
    }

    pStatistic = (PNIP_STATISTIC_PORTX_STRUCT_PTR_TYPE)EDDP_HALGetRAMBaseAddress(pDDB, EDDP_HAL_RAMTYPE_STATISTIC, LSA_NULL);
    EDDP_ASSERT_NO_NULL_PTR(pStatistic);

    pStatistic_Mem = (PNIP_STATISTIC_MEM_STRUCT_PTR_TYPE)EDDP_HALGetRAMBaseAddress(pDDB, EDDP_HAL_RAMTYPE_STATISTIC, LSA_NULL);
    EDDP_ASSERT_NO_NULL_PTR(pStatistic_Mem);

    // ***** map from HwPortID to index (e.g: 1 2 3 4 -> 0 1 2 3)
    PortIDX = (HwPortID-1);
    RequestedCounters_loc = pStats->RequestedCounters;
    switch (EDDP_HALGetPnipRevision(pDDB))
    {
        #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
        case EDDP_HAL_HW_PNIP_REV1:
        {
            if (RequestedCounters_loc == 0)
            {
                RequestedCounters_loc = EDDP_MIB2_SUPPORTED_COUNTERS_PORT_REV1;
            }
            if ((RequestedCounters_loc & EDD_MIB_SUPPORT_INOCTETS) != 0)
            {
                pStats->InOctets = EDDP_HAL_MEM32_READ(pDDB, &pStatistic[PortIDX].Good_Bytes_Received_Px);
            }
            if ((RequestedCounters_loc & EDD_MIB_SUPPORT_INUCASTPKTS) != 0)
            {
                pStats->InUcastPkts = EDDP_HAL_MEM32_READ(pDDB, &pStatistic[PortIDX].UC_Frames_Received_Px);
            }
            if ((RequestedCounters_loc & EDD_MIB_SUPPORT_INNUCASTPKTS) != 0)
            {
                TmpMulticastPkts = EDDP_HAL_MEM32_READ(pDDB, &pStatistic[PortIDX].MC_Frames_Received_Px);
                TmpBroadcastPkts = EDDP_HAL_MEM32_READ(pDDB, &pStatistic[PortIDX].BroadCast_Frames_Received_Px);
                pStats->InNUcastPkts = TmpBroadcastPkts + TmpMulticastPkts;
                if ((RequestedCounters_loc & EDD_MIB_SUPPORT_INMULTICASTPKTS) != 0)
                {
                    pStats->InMulticastPkts = TmpMulticastPkts;
                }
                if ((RequestedCounters_loc & EDD_MIB_SUPPORT_INBROADCASTPKTS) != 0)
                {
                    pStats->InBroadcastPkts = TmpBroadcastPkts;
                }
            }
            else
            {
                if ((RequestedCounters_loc & EDD_MIB_SUPPORT_INMULTICASTPKTS) != 0)
                {
                    pStats->InMulticastPkts = EDDP_HAL_MEM32_READ(pDDB, &pStatistic[PortIDX].MC_Frames_Received_Px);
                }
                if ((RequestedCounters_loc & EDD_MIB_SUPPORT_INBROADCASTPKTS) != 0)
                {
                    pStats->InBroadcastPkts = EDDP_HAL_MEM32_READ(pDDB, &pStatistic[PortIDX].BroadCast_Frames_Received_Px);
                }
            }
            if ((RequestedCounters_loc & EDD_MIB_SUPPORT_INDISCARDS) != 0)
            {
                pStats->InDiscards = EDDP_HAL_MEM32_READ(pDDB, &pStatistic[PortIDX].Frames_Dropped_L2_Px);
            }
            if ((RequestedCounters_loc & EDD_MIB_SUPPORT_INERRORS) != 0)
            {
                pStats->InErrors = EDDP_HAL_MEM32_READ(pDDB, &pStatistic[PortIDX].Bad_Frames_Received_Px);
            }
            pStats->InUnknownProtos = 0; // not supported
            if ((RequestedCounters_loc & EDD_MIB_SUPPORT_OUTOCTETS) != 0)
            {
                pStats->OutOctets = EDDP_HAL_MEM32_READ(pDDB, &pStatistic[PortIDX].Total_Bytes_Sent_Px);
            }
            if ((RequestedCounters_loc & EDD_MIB_SUPPORT_OUTUCASTPKTS) != 0)
            {
                pStats->OutUcastPkts = EDDP_HAL_MEM32_READ(pDDB, &pStatistic[PortIDX].UniCast_Frames_Sent_Px);
            }
            if ((RequestedCounters_loc & EDD_MIB_SUPPORT_OUTNUCASTPKTS) != 0)
            {
                TmpMulticastPkts = EDDP_HAL_MEM32_READ(pDDB, &pStatistic_Mem->PortStatistic[PortIDX].MultiCast_Frames_Sent_Px);
                TmpBroadcastPkts = EDDP_HAL_MEM32_READ(pDDB, &pStatistic_Mem->PortStatistic[PortIDX].BroadCast_Frames_Sent_Px);
                pStats->OutNUcastPkts = TmpBroadcastPkts + TmpMulticastPkts;
                if ((RequestedCounters_loc & EDD_MIB_SUPPORT_OUTMULTICASTPKTS) != 0)
                {
                    pStats->OutMulticastPkts = TmpMulticastPkts;
                }
                if ((RequestedCounters_loc & EDD_MIB_SUPPORT_OUTBROADCASTPKTS) != 0)
                {
                    pStats->OutBroadcastPkts = TmpBroadcastPkts;
                }
            }
            else
            {
                if ((RequestedCounters_loc & EDD_MIB_SUPPORT_OUTMULTICASTPKTS) != 0)
                {
                    pStats->OutMulticastPkts = EDDP_HAL_MEM32_READ(pDDB, &pStatistic_Mem->PortStatistic[PortIDX].MultiCast_Frames_Sent_Px);
                }
                if ((RequestedCounters_loc & EDD_MIB_SUPPORT_OUTBROADCASTPKTS) != 0)
                {
                    pStats->OutBroadcastPkts = EDDP_HAL_MEM32_READ(pDDB, &pStatistic_Mem->PortStatistic[PortIDX].BroadCast_Frames_Sent_Px);
                }
            }
            if ((RequestedCounters_loc & EDD_MIB_SUPPORT_OUTDISCARDS) != 0)
            {
                pStats->OutDiscards = EDDP_HAL_MEM32_READ(pDDB, &pStatistic[PortIDX].FrameAging_Crossed_Px);
                pStats->OutDiscards += EDDP_HAL_MEM32_READ(pDDB, &pStatistic[PortIDX].IRT_DelayTime_Crossed_Px);
            }
			pStats->InHCOctets = 0; // not supported
			pStats->InHCUcastPkts = 0; // not supported
			pStats->InHCMulticastPkts = 0; // not supported
			pStats->InHCBroadcastPkts = 0; // not supported
			pStats->OutHCOctets = 0; // not supported
			pStats->OutHCUcastPkts = 0; // not supported
			pStats->OutHCMulticastPkts = 0; // not supported
			pStats->OutHCBroadcastPkts = 0; // not supported
			pStats->OutErrors           = 0; // not supported
            //MIB2 calc for Rev1
            pStats->SupportedCounters   = EDDP_MIB2_SUPPORTED_COUNTERS_PORT_REV1;
        }
        break;
        #endif

        #ifdef EDDP_CFG_HW_PNIP_REV2_SUPPORT
        case EDDP_HAL_HW_PNIP_REV2:
        {
            if (RequestedCounters_loc == 0)
            {
                RequestedCounters_loc = EDDP_MIB2_SUPPORTED_COUNTERS_PORT_REV2;
            }
            if ((RequestedCounters_loc & EDD_MIB_SUPPORT_INOCTETS) != 0)
            {
                pStats->InOctets = EDDP_HAL_MEM32_READ(pDDB, &pStatistic[PortIDX].Good_Bytes_Received_Px);
            }
            if ((RequestedCounters_loc & EDD_MIB_SUPPORT_INUCASTPKTS) != 0)
            {
                pStats->InUcastPkts = EDDP_HAL_MEM32_READ(pDDB, &pStatistic[PortIDX].UC_Frames_Received_Px);
            }
            if ((RequestedCounters_loc & EDD_MIB_SUPPORT_INNUCASTPKTS) != 0)
            {
                TmpMulticastPkts = EDDP_HAL_MEM32_READ(pDDB, &pStatistic[PortIDX].MC_Frames_Received_Px);
                TmpBroadcastPkts = EDDP_HAL_MEM32_READ(pDDB, &pStatistic[PortIDX].BroadCast_Frames_Received_Px);
                pStats->InNUcastPkts = TmpBroadcastPkts + TmpMulticastPkts;
                if ((RequestedCounters_loc & EDD_MIB_SUPPORT_INMULTICASTPKTS) != 0)
                {
                    pStats->InMulticastPkts = TmpMulticastPkts;
                }
                if ((RequestedCounters_loc & EDD_MIB_SUPPORT_INBROADCASTPKTS) != 0)
                {
                    pStats->InBroadcastPkts = TmpBroadcastPkts;
                }
            }
            else
            {
                if ((RequestedCounters_loc & EDD_MIB_SUPPORT_INMULTICASTPKTS) != 0)
                {
                    pStats->InMulticastPkts = EDDP_HAL_MEM32_READ(pDDB, &pStatistic[PortIDX].MC_Frames_Received_Px);
                }
                if ((RequestedCounters_loc & EDD_MIB_SUPPORT_INBROADCASTPKTS) != 0)
                {
                    pStats->InBroadcastPkts = EDDP_HAL_MEM32_READ(pDDB, &pStatistic[PortIDX].BroadCast_Frames_Received_Px);
                }
            }
            if ((RequestedCounters_loc & EDD_MIB_SUPPORT_INDISCARDS) != 0)
            {
                pStats->InDiscards = EDDP_HAL_MEM32_READ(pDDB, &pStatistic[PortIDX].Frames_Dropped_L2_Px);
            }
            if ((RequestedCounters_loc & EDD_MIB_SUPPORT_INERRORS) != 0)
            {
                pStats->InErrors = EDDP_HAL_MEM32_READ(pDDB, &pStatistic[PortIDX].Bad_Frames_Received_Px);
            }
			pStats->InUnknownProtos = 0; // not supported
            if ((RequestedCounters_loc & EDD_MIB_SUPPORT_OUTOCTETS) != 0)
            {
                pStats->OutOctets = EDDP_HAL_MEM32_READ(pDDB, &pStatistic[PortIDX].Total_Bytes_Sent_Px);
            }
            if ((RequestedCounters_loc & EDD_MIB_SUPPORT_OUTUCASTPKTS) != 0)
            {
                pStats->OutUcastPkts = EDDP_HAL_MEM32_READ(pDDB, &pStatistic[PortIDX].UniCast_Frames_Sent_Px);
            }
            if ((RequestedCounters_loc & EDD_MIB_SUPPORT_OUTNUCASTPKTS) != 0)
            {
                TmpMulticastPkts = EDDP_HAL_MEM32_READ(pDDB, &pStatistic_Mem->PortStatistic[PortIDX].MultiCast_Frames_Sent_Px);
                TmpBroadcastPkts = EDDP_HAL_MEM32_READ(pDDB, &pStatistic_Mem->PortStatistic[PortIDX].BroadCast_Frames_Sent_Px);
                pStats->OutNUcastPkts = TmpBroadcastPkts + TmpMulticastPkts;
                if ((RequestedCounters_loc & EDD_MIB_SUPPORT_OUTMULTICASTPKTS) != 0)
                {
                    pStats->OutMulticastPkts = TmpMulticastPkts;
                }
                if ((RequestedCounters_loc & EDD_MIB_SUPPORT_OUTBROADCASTPKTS) != 0)
                {
                    pStats->OutBroadcastPkts = TmpBroadcastPkts;
                }
            }
            else
            {
                if ((RequestedCounters_loc & EDD_MIB_SUPPORT_OUTMULTICASTPKTS) != 0)
                {
                    pStats->OutMulticastPkts = EDDP_HAL_MEM32_READ(pDDB, &pStatistic_Mem->PortStatistic[PortIDX].MultiCast_Frames_Sent_Px);
                }
                if ((RequestedCounters_loc & EDD_MIB_SUPPORT_OUTBROADCASTPKTS) != 0)
                {
                    pStats->OutBroadcastPkts = EDDP_HAL_MEM32_READ(pDDB, &pStatistic_Mem->PortStatistic[PortIDX].BroadCast_Frames_Sent_Px);
                }
            }
            if ((RequestedCounters_loc & EDD_MIB_SUPPORT_OUTDISCARDS) != 0)
            {
                pStats->OutDiscards = EDDP_HAL_MEM32_READ(pDDB, &pStatistic[PortIDX].FrameAging_Crossed_Px);
                pStats->OutDiscards += EDDP_HAL_MEM32_READ(pDDB, &pStatistic[PortIDX].IRT_DelayTime_Crossed_Px);
            }
            pStats->InHCOctets = 0; // not supported
			pStats->InHCUcastPkts = 0; // not supported
			pStats->InHCMulticastPkts = 0; // not supported
			pStats->InHCBroadcastPkts = 0; // not supported
			pStats->OutHCOctets = 0; // not supported
			pStats->OutHCUcastPkts = 0; // not supported
			pStats->OutHCMulticastPkts = 0; // not supported
			pStats->OutHCBroadcastPkts = 0; // not supported
            if ((RequestedCounters_loc & EDD_MIB_SUPPORT_OUTERRORS) != 0)
            {
                pStats->OutErrors = EDDP_HAL_MEM32_READ(pDDB, &pStatistic[PortIDX].Total_bad_Frames_Sent_Px);
            }
            //MIB2 calc for Rev2
            pStats->SupportedCounters   = EDDP_MIB2_SUPPORTED_COUNTERS_PORT_REV2;
        }
        break;
        #endif
        default:
        {
            EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_GEN_GetPortStatistics(): FATAL -> PnipRevision(%d) is unknown", 
                EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }
    pStats->OutQLen             = 0; // not supported
#else
    EDDP_LL_GetPortStatistics(pDDB, pStats, HwPortID);
#endif
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GEN_GetIfaceStatistics                 +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE            pDDB     +*/
/*+                             EDDP_LOCAL_GET_STATISTICS_PTR_TYPE pStats   +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pStats     : destination for statistic                                 +*/
/*+                                                                         +*/
/*+  Results    : LSA_VOID                                                  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Utility function for EDDP_GEN_RequestXXXStatistics().     +*/
/*+                                                                         +*/
/*+               Note: values in pStats must be initialized or Clear=1 !!! +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GEN_GetIfaceStatistics(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_LOCAL_GET_STATISTICS_PTR_TYPE      pStats)
{
    EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE  pNrtApiCtrlIfaceMgm;
    PNIP_LOCAL_DMACW_RCV_DESCR_PTR_TYPE         pRcvPtr;
    LSA_UINT32                                  RegAddrDropCnt, Idx, Value;


    EDDP_ASSERT_NO_NULL_PTR(pStats);

    switch (EDDP_HALGetPnipRevision(pDDB))
    {
#if (defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT) || defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT))
#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
        case EDDP_HAL_HW_PNIP_REV1:
#endif
#ifdef EDDP_CFG_HW_PNIP_REV2_SUPPORT
        case EDDP_HAL_HW_PNIP_REV2:
#endif
        {
            #if (EDDP_NRT_RXTX_COUNTER_SUPPORT == 1)
            EDDP_NRT_STAT_TYPE_PTR pNRTIfaceStats;
            #endif

            pNrtApiCtrlIfaceMgm = (EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE)pDDB->NRT.pNrtApiCtrlIfaceMgm;

#if (EDDP_NRT_RXTX_COUNTER_SUPPORT == 1)
            pNRTIfaceStats = &pDDB->NRT.Stats;
            pStats->SupportedCounters = EDDP_MIB2_SUPPORTED_COUNTERS_IF;
            pStats->InOctets = 0;    // -
            pStats->InUcastPkts = pNRTIfaceStats->InUcastPkts;    // SW counter UC from DMACWs
            pStats->InNUcastPkts = pNRTIfaceStats->InNUcastPkts;   // SW counter BC/MC from DMACWs  
            pStats->InDiscards = 0;    // all 16bit HW DMACWDropCounter from RX-DMACW (entry into the ring)
            pStats->InErrors = 0;    // -
            pStats->InUnknownProtos = 0;    // sum of DropCount_IFx from HW-Regs (only 16 bit)
            pStats->OutOctets = 0;    // -
            pStats->OutUcastPkts = pNRTIfaceStats->OutUcastPkts;   // SW counter UC from DMACWs
            pStats->OutNUcastPkts = pNRTIfaceStats->OutNUcastPkts;  // SW counter BC/MC from DMACWs
            pStats->OutDiscards = pNRTIfaceStats->OutDiscards;    // SW counter Tx-Error (only TimeOut) from DMACWs
            pStats->OutErrors = pNRTIfaceStats->OutErrors;      // SW counter Tx-Error (no TimeOut) from DMACWs
            pStats->OutQLen = 0;    // -
            pStats->InMulticastPkts = pNRTIfaceStats->InMulticastPkts; // SW counter MC from DMACWs
            pStats->InBroadcastPkts = pNRTIfaceStats->InBroadcastPkts; // SW counter BC from DMACWs
            pStats->OutMulticastPkts = pNRTIfaceStats->OutMulticastPkts; // SW counter MC from DMACWs
            pStats->OutBroadcastPkts = pNRTIfaceStats->OutBroadcastPkts; // SW counter BC from DMACWs
            pStats->InHCOctets = 0; // not supported
            pStats->InHCUcastPkts = 0; // not supported
            pStats->InHCMulticastPkts = 0; // not supported
            pStats->InHCBroadcastPkts = 0; // not supported
            pStats->OutHCOctets = 0; // not supported
            pStats->OutHCUcastPkts = 0; // not supported
            pStats->OutHCMulticastPkts = 0; // not supported
            pStats->OutHCBroadcastPkts = 0; // not supported
#else
            EDDP_MEMSET_COMMON(pStats, 0, sizeof(EDDP_GET_STATISTICS_TYPE));    // clear rx/tx statistic
#endif

            /* There is a macro that checks PNIP_R2_REG_DROPCOUNT_IFA_00 and PNIP_R3_REG_DROPCOUNT_IFA_00 has the same value. */
            /* Checked in eddp_hal_pnip_val.h. We use the R2 version. */
            RegAddrDropCnt = PNIP_R2_REG_DROPCOUNT_IFA_00;

            for (Idx = 0; Idx < EDDP_NRT_RX_DMACW_CNT; Idx++)
            {
                if (EDDP_ARD_NRT_USED_RINGS & ((LSA_UINT32)1 << Idx))
                {
                    // ***** count all PNIP_REG_DROPCOUNT_IFx_XX / DMACWDropCounter used by Host-NRT (without KRISC32!!!)
                    pStats->InUnknownProtos += EDDP_HAL_REG32_READ(pDDB, RegAddrDropCnt);
                    // ***** count all DMACWDropCounter from RxRing used by Host-NRT (without KRISC32!!!)
                    pRcvPtr = &pNrtApiCtrlIfaceMgm->ApiCtrlRamRcvDmacw[Idx];
                    Value = (EDDP_HAL_MEM32_READ(pDDB, &pRcvPtr->RcvDropCntTsErrIntVal) >> PNIP_DMACW_RCV_DESCR_DROPCNT_SHIFT);
                    pStats->InDiscards += Value;
                }
                /* There is a macro that checks                                      */
                /* PNIP_R2_REG_DROPCOUNT_IFA_00 and PNIP_R3_REG_DROPCOUNT_IFA_00     */
                /* PNIP_R2_REG_DROPCOUNT_IFA_01 and PNIP_R3_REG_DROPCOUNT_IFA_01     */
                /* has the same value.                                               */
                /* Checked in eddp_hal_pnip_val.h. We use the R2 version.            */
                RegAddrDropCnt += (PNIP_R2_REG_DROPCOUNT_IFA_01 - PNIP_R2_REG_DROPCOUNT_IFA_00);  // offset b0etween registers
            }
            break;
        }
#endif
        default:
        {
            EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_GEN_GetPortStatistics(): FATAL -> PnipRevision(%d) is unknown",
                EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }
}



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GEN_SubStatistics                      +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE            pDDB     +*/
/*+                             EDDP_LOCAL_GET_STATISTICS_PTR_TYPE pNew     +*/
/*+                             EDDP_LOCAL_GET_STATISTICS_PTR_TYPE pSnap    +*/
/*+                             LSA_BOOL                           Reset    +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pNew       : new statistic data                                        +*/
/*+  pSnap      : snapshot of statistic                                     +*/
/*+  Reset      : if TRUE --> reset snapshot                                +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Utility function for EDDP_GEN_RequestGetStatistics().     +*/
/*+                                                                         +*/
/*+               Note: Build difference to get real values !!!             +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GEN_SubStatistics(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_LOCAL_GET_STATISTICS_PTR_TYPE      pNew,
    EDDP_LOCAL_GET_STATISTICS_PTR_TYPE      pSnap,
    LSA_UINT8                               Reset)
{
    LSA_UNUSED_ARG(pDDB);

    if (pNew && pSnap)
    {
        // ***** build difference to get real counter values (beginning from virtual zero)
        pNew->InOctets        -= pSnap->InOctets ;
        pNew->InUcastPkts     -= pSnap->InUcastPkts;
        pNew->InNUcastPkts    -= pSnap->InNUcastPkts;
        pNew->InDiscards      -= pSnap->InDiscards;
        pNew->InErrors        -= pSnap->InErrors;
        pNew->InUnknownProtos -= pSnap->InUnknownProtos;
        pNew->OutOctets       -= pSnap->OutOctets;
        pNew->OutUcastPkts    -= pSnap->OutUcastPkts;
        pNew->OutNUcastPkts   -= pSnap->OutNUcastPkts;
        pNew->OutDiscards     -= pSnap->OutDiscards;
        pNew->OutErrors       -= pSnap->OutErrors;
        pNew->OutQLen         -= pSnap->OutQLen;
        pNew->InMulticastPkts    -= pSnap->InMulticastPkts;
        pNew->InBroadcastPkts    -= pSnap->InBroadcastPkts;
        pNew->OutMulticastPkts   -= pSnap->OutMulticastPkts;
        pNew->OutBroadcastPkts   -= pSnap->OutBroadcastPkts;
        pNew->InHCOctets         -= pSnap->InHCOctets;
        pNew->InHCUcastPkts      -= pSnap->InHCUcastPkts;
        pNew->InHCMulticastPkts  -= pSnap->InHCMulticastPkts;
        pNew->InHCBroadcastPkts  -= pSnap->InHCBroadcastPkts;
        pNew->OutHCOctets        -= pSnap->OutHCOctets;
        pNew->OutHCUcastPkts     -= pSnap->OutHCUcastPkts;
        pNew->OutHCMulticastPkts -= pSnap->OutHCMulticastPkts;
        pNew->OutHCBroadcastPkts -= pSnap->OutHCBroadcastPkts;

        if (Reset == EDD_STATISTICS_RESET_VALUES)
        {
            // ***** take over current counter values to set the snapshot counters to virtual zero 
            pSnap->InOctets        += pNew->InOctets;
            pSnap->InUcastPkts     += pNew->InUcastPkts;
            pSnap->InNUcastPkts    += pNew->InNUcastPkts;
            pSnap->InDiscards      += pNew->InDiscards;
            pSnap->InErrors        += pNew->InErrors;
            pSnap->InUnknownProtos += pNew->InUnknownProtos;
            pSnap->OutOctets       += pNew->OutOctets;
            pSnap->OutUcastPkts    += pNew->OutUcastPkts;
            pSnap->OutNUcastPkts   += pNew->OutNUcastPkts;
            pSnap->OutDiscards     += pNew->OutDiscards;
            pSnap->OutErrors       += pNew->OutErrors;
            pSnap->OutQLen         += pNew->OutQLen;
            pSnap->InMulticastPkts    += pNew->InMulticastPkts;
            pSnap->InBroadcastPkts    += pNew->InBroadcastPkts;
            pSnap->OutMulticastPkts   += pNew->OutMulticastPkts;
            pSnap->OutBroadcastPkts   += pNew->OutBroadcastPkts;
            pSnap->InHCOctets         += pNew->InHCOctets;
            pSnap->InHCUcastPkts      += pNew->InHCUcastPkts;
            pSnap->InHCMulticastPkts  += pNew->InHCMulticastPkts;
            pSnap->InHCBroadcastPkts  += pNew->InHCBroadcastPkts;
            pSnap->OutHCOctets        += pNew->OutHCOctets;
            pSnap->OutHCUcastPkts     += pNew->OutHCUcastPkts;
            pSnap->OutHCMulticastPkts += pNew->OutHCMulticastPkts;
            pSnap->OutHCBroadcastPkts += pNew->OutHCBroadcastPkts;
        }
        return (EDD_STS_OK);
    }

    return (EDD_STS_ERR_PARAM);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_GEN_CopyStatistics                    +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE                pDDB +*/
/*+                             EDD_UPPER_GET_STATISTICS_MIB_PTR_TYPE  pDst +*/
/*+                             EDDP_LOCAL_GET_STATISTICS_PTR_TYPE     pSrc +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pDst       : destination for MIB statistic data                        +*/
/*+  pSrc       : current statistic data                                    +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Utility function for EDDP_GEN_RequestGetStatistics().     +*/
/*+                                                                         +*/
/*+               Note: Copy from internal members into upper data struct ! +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_GEN_CopyStatistics(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDD_UPPER_GET_STATISTICS_MIB_PTR_TYPE   pDst,
    EDDP_LOCAL_GET_STATISTICS_PTR_TYPE      pSrc)
{
    LSA_UNUSED_ARG(pDDB);

    if (pDst && pSrc)
    {
        pDst->SupportedCounters = pSrc->SupportedCounters;
        pDst->RequestedCounters = pSrc->RequestedCounters;
        pDst->InOctets          = pSrc->InOctets ;
        pDst->InUcastPkts       = pSrc->InUcastPkts;
        pDst->InNUcastPkts      = pSrc->InNUcastPkts;
        pDst->InDiscards        = pSrc->InDiscards;
        pDst->InErrors          = pSrc->InErrors;
        pDst->InUnknownProtos   = pSrc->InUnknownProtos;
        pDst->OutOctets         = pSrc->OutOctets;
        pDst->OutUcastPkts      = pSrc->OutUcastPkts;
        pDst->OutNUcastPkts     = pSrc->OutNUcastPkts;
        pDst->OutDiscards       = pSrc->OutDiscards;
        pDst->OutErrors         = pSrc->OutErrors;
        pDst->OutQLen           = pSrc->OutQLen;
        pDst->InMulticastPkts    = pSrc->InMulticastPkts;
        pDst->InBroadcastPkts    = pSrc->InBroadcastPkts;
        pDst->OutMulticastPkts   = pSrc->OutMulticastPkts;
        pDst->OutBroadcastPkts   = pSrc->OutBroadcastPkts;
        pDst->InHCOctets         = pSrc->InHCOctets;
        pDst->InHCUcastPkts      = pSrc->InHCUcastPkts;
        pDst->InHCMulticastPkts  = pSrc->InHCMulticastPkts;
        pDst->InHCBroadcastPkts  = pSrc->InHCBroadcastPkts;
        pDst->OutHCOctets        = pSrc->OutHCOctets;
        pDst->OutHCUcastPkts     = pSrc->OutHCUcastPkts;
        pDst->OutHCMulticastPkts = pSrc->OutHCMulticastPkts;
        pDst->OutHCBroadcastPkts = pSrc->OutHCBroadcastPkts;
        return (EDD_STS_OK);
    }
    return (EDD_STS_ERR_PARAM);
}





/*===========================================================================*/
/*                                 functions                                 */
/*===========================================================================*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname  :    EDDP_GENSetLinkStatusRQB                            +*/
/*+  Input/Output  :    EDD_UPPER_LINK_STATUS_IND_PROVIDE_PTR_TYPE   pParam +*/
/*+                :    EDDP_DDB_SWI_LINK_STATUS_PTR_TYPE       pLinkStatus +*/
/*+                                                                         +*/
/*+  Result        :    LSA_BOOL                                            +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function copies current link parameters into the RQB.+*/
/*+               In case the parameters are already equal, nothing is done +*/
/*+               and LSA_FALSE is returned.                                +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_GENSetLinkStatusRQB( 
    EDD_UPPER_LINK_STATUS_IND_PROVIDE_PTR_TYPE  pParam,
    EDDP_DDB_SWI_LINK_STATUS_PTR_TYPE           pLinkStatus)
{
    LSA_BOOL        bNewParameters;

    if ((pParam->Status                   != pLinkStatus->LinkStatus              ) ||
        (pParam->Mode                     != pLinkStatus->LinkDuplexity           ) ||
        (pParam->Speed                    != pLinkStatus->LinkSpeed               ) ||
        (pParam->LineDelayInTicksMeasured != pLinkStatus->LineDelayInTicksMeasured) ||
        (pParam->CableDelayInNsMeasured   != pLinkStatus->CableDelayInNsMeasured  ) ||
        (pParam->PortTxDelay              != pLinkStatus->RealPortTxDelay         ) ||
        (pParam->PortRxDelay              != pLinkStatus->RealPortRxDelay         ) ||
        (pParam->PortStatus               != pLinkStatus->PortStatus              ) ||
        (pParam->Autoneg                  != pLinkStatus->Autoneg                 ) ||
        (pParam->IRTPortStatus            != pLinkStatus->IRTPortStatus           ) ||
        (pParam->RTClass2_PortStatus      != pLinkStatus->RTClass2_PortStatus     ) ||
        (pParam->PortState                != pLinkStatus->PortState               ) ||
        (pParam->MediaType                != pLinkStatus->MediaType               ) ||
        (pParam->OpticalTransType         != pLinkStatus->OpticalTransType        ) ||
        (pParam->ShortPreamble            != pLinkStatus->ShortPreamble           ) ||
        (pParam->TxFragmentation          != pLinkStatus->TxFragmentation         ) ||
        (pParam->AddCheckEnabled          != pLinkStatus->AddCheckEnabled         ) ||
        (pParam->ChangeReason             != pLinkStatus->ChangeReason            ) ||
        (pParam->PhyStatus 				  != pLinkStatus->PhyStatus               ) ||
        (pParam->MAUType				  != pLinkStatus->MAUType                 ) ||
        (pParam->SyncId0_TopoOk           != pLinkStatus->SyncId0_TopoOk          ))        
    {                  
        pParam->Status                   = pLinkStatus->LinkStatus;
        pParam->Speed                    = pLinkStatus->LinkSpeed;
        pParam->Mode                     = pLinkStatus->LinkDuplexity;
        pParam->Autoneg                  = pLinkStatus->Autoneg;
        pParam->PortStatus               = pLinkStatus->PortStatus;
        pParam->PhyStatus				 = pLinkStatus->PhyStatus;
        pParam->MAUType					 = pLinkStatus->MAUType;
        pParam->MediaType				 = pLinkStatus->MediaType;
        pParam->OpticalTransType		 = pLinkStatus->OpticalTransType;
        pParam->ShortPreamble            = pLinkStatus->ShortPreamble;
        pParam->TxFragmentation          = pLinkStatus->TxFragmentation;
        pParam->AddCheckEnabled          = pLinkStatus->AddCheckEnabled;
        pParam->ChangeReason             = pLinkStatus->ChangeReason;
        pParam->IRTPortStatus            = pLinkStatus->IRTPortStatus;
        pParam->RTClass2_PortStatus      = pLinkStatus->RTClass2_PortStatus;
        pParam->LineDelayInTicksMeasured = pLinkStatus->LineDelayInTicksMeasured;
        pParam->CableDelayInNsMeasured   = pLinkStatus->CableDelayInNsMeasured;
        pParam->PortTxDelay              = pLinkStatus->RealPortTxDelay;
        pParam->PortRxDelay              = pLinkStatus->RealPortRxDelay;
        pParam->PortState                = pLinkStatus->PortState;
        pParam->SyncId0_TopoOk           = pLinkStatus->SyncId0_TopoOk;

        bNewParameters = LSA_TRUE;
    }
    else
    {
        bNewParameters = LSA_FALSE;
    }

    return (bNewParameters);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GENIndicateLinkChange_Ext              +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_UINT32                  PortID          +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  PortID     : Port ID                                              (in) +*/
/*+                                                                         +*/
/*+  Results    : LSA_VOID                                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called whenever an event occurs that     +*/
/*+               changes port parameters which are only relevant for       +*/
/*+               service EDD_SRV_LINK_STATUS_IND_PROVIDE_EXT. If there are +*/
/*+               pending RQBs, they are filled with updated data and sent  +*/
/*+               back to the user in order to indicate the new parameters. +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GENIndicateLinkChange_Ext(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  PortID)
{
    EDD_RSP                     Status;
    LSA_HANDLE_TYPE             Handle;
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB;
    EDD_UPPER_RQB_PTR_TYPE      pRQB;

    Status = EDD_STS_OK;
    Handle = 0;

    while (Status == EDD_STS_OK)
    {
        Status = EDDP_CoreDevHandleGetHDB(Handle, &pHDB);

        if (    (Status == EDD_STS_OK) 
            &&  (pHDB->InUse) 
            &&  (LSA_HOST_PTR_ARE_EQUAL(pHDB->pDDB, pDDB)) // handle belongs to same interface
           )   
        {
            if ( (pHDB->LinkIndReq[PortID].Count != 0) && (EDDP_LINK_IND_PROVIDE_EXTENDED == pHDB->LinkIndReqType[PortID]) )
            {
                // ***** Note: There is only one RQB in the QUEUE --> compare with the RQB is ok !!!
                pRQB = EDDP_GlbRQBQueueRemoveFromBegin( &(pHDB->LinkIndReq[PortID]) );

                if ( EDDP_GENSetLinkStatusRQB( (EDD_UPPER_LINK_STATUS_IND_PROVIDE_PTR_TYPE) pRQB->pParam, &(pDDB->SWI.LinkStatus[PortID]) ) )
                {
                    // only fire indication when at least one parameter has actually changed
                    EDDP_GlbRequestFinish(pHDB, pRQB, EDD_STS_OK);

                    if (pHDB->LinkIndReq[PortID].Count == 0)
                    {
                        // reset the type of link indication for this port when there are no more RQBs left in EDDP
                        pHDB->LinkIndReqType[PortID] = EDDP_LINK_IND_PROVIDE_UNDEFINED;
                    }
                }
                else
                {
                    // put RQB back to queue            
                    EDDP_GlbRQBQueueAddToEnd (&(pHDB->LinkIndReq[PortID]), pRQB);
                }
            }
        }
        Handle++;
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GENIndicateLinkChange                  +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_UINT32                  PortID          +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  PortID     : Port ID                                              (in) +*/
/*+                                                                         +*/
/*+  Results    : LSA_VOID                                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called whenever an event occurs that     +*/
/*+               changes port parameters which are relevant for			+*/
/*+               service EDD_SRV_LINK_STATUS_IND_PROVIDE and 				+*/
/*+			      EDD_SRV_LINK_STATUS_IND_PROVIDE. If there are				+*/
/*+               pending RQBs, they are filled with updated data and sent  +*/
/*+               back to the user in order to indicate the new parameters. +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GENIndicateLinkChange(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  PortID)
{
    EDD_RSP                     Status;
    LSA_HANDLE_TYPE             Handle;
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB;
    EDD_UPPER_RQB_PTR_TYPE      pRQB;

    Status = EDD_STS_OK;
    Handle = 0;

    while (Status == EDD_STS_OK)
    {
        Status = EDDP_CoreDevHandleGetHDB(Handle, &pHDB);

        if (    (Status == EDD_STS_OK) 
            &&  (pHDB->InUse)
            &&  (LSA_HOST_PTR_ARE_EQUAL(pHDB->pDDB, pDDB)) // handle belongs to same interface
           )
        {
            if (pHDB->LinkIndReq[PortID].Count != 0)
            {
                pRQB = EDDP_GlbRQBQueueRemoveFromBegin( &(pHDB->LinkIndReq[PortID]) );

                if ( EDDP_GENSetLinkStatusRQB( (EDD_UPPER_LINK_STATUS_IND_PROVIDE_PTR_TYPE) pRQB->pParam, &(pDDB->SWI.LinkStatus[PortID]) ) )
                {
                    // only fire indication when at least one parameter has actually changed
                    EDDP_GlbRequestFinish(pHDB, pRQB, EDD_STS_OK);

                    if (pHDB->LinkIndReq[PortID].Count == 0)
                    {
                        // reset the type of link indication for this port when there are no more RQBs left in EDDP
                        pHDB->LinkIndReqType[PortID] = EDDP_LINK_IND_PROVIDE_UNDEFINED;
                    }
                }
                else
                {
                    // put RQB back to queue            
                    EDDP_GlbRQBQueueAddToEnd (&(pHDB->LinkIndReq[PortID]), pRQB);
                }
            }
        }
        Handle++;
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname    :    EDDP_GENFindFastestPort                           +*/
/*+  Input/Output    :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                  +*/
/*+  Result          :    LSA_BOOL                                          +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB            : Pointer to DeviceDescriptionBlock         (in)/(out) +*/
/*+                                                                         +*/
/*+  Result          : LSA_TRUE / LSA_FALSE                                 +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function finds the fastest port and stores its       +*/
/*+               parameters into pDDB->SWI.LinkStatus[0].                  +*/
/*+               It returns LSA_TRUE if parameters for the fastest port    +*/
/*+               had changed; otherwise it returns LSA_FALSE.              +*/
/*+                                                                         +*/
/*+  Priority ranking for determining fastest port                          +*/
/*+  (shown only for 10/100 Mbit):                                          +*/
/*+                                                                         +*/
/*+              Status               Speed            Mode                 +*/
/*+   best       LINK_UP              100              Full                 +*/
/*+              LINK_UP              100              Half                 +*/
/*+              LINK_UP              10               Full                 +*/
/*+              LINK_UP              10               Half                 +*/
/*+     |        LINK_UP_CLOSED       100              Full                 +*/
/*+     |        LINK_UP_CLOSED       100              Half                 +*/
/*+     |        LINK_UP_CLOSED       10               Full                 +*/
/*+     |        LINK_UP_CLOSED       10               Half                 +*/
/*+     V        LINK_UP_DISABLED     100              Full                 +*/
/*+              LINK_UP_DISABLED     100              Half                 +*/
/*+              LINK_UP_DISABLED     10               Full                 +*/
/*+              LINK_UP_DISABLED     10               Half                 +*/
/*+   worst      LINK_DOWN            UNKNOWN          UNKNOWN              +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_GENFindFastestPort(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT32                      i;
    LSA_BOOL                        bCheckSpeedDuplexity;
    LSA_BOOL                        bParametersChanged;
    EDDP_DDB_SWI_LINK_STATUS_TYPE   FastestPortStatus;

    FastestPortStatus.LinkStatus    = EDD_LINK_DOWN;
    FastestPortStatus.LinkSpeed     = EDD_LINK_UNKNOWN;
    FastestPortStatus.LinkDuplexity = EDD_LINK_UNKNOWN;
    bParametersChanged              = LSA_FALSE;

    for ( i=1; i<= pDDB->SWI.PortCnt; i++)
    {
        if (pDDB->SWI.LinkStatus[i].LinkStatus == EDD_LINK_UP)
        {
            if (FastestPortStatus.LinkStatus != EDD_LINK_UP)
            {
                FastestPortStatus.LinkStatus    = EDD_LINK_UP;
                FastestPortStatus.LinkSpeed     = EDD_LINK_UNKNOWN;
                FastestPortStatus.LinkDuplexity = EDD_LINK_UNKNOWN;
            }
            bCheckSpeedDuplexity = LSA_TRUE;
        }
        else if ((pDDB->SWI.LinkStatus[i].LinkStatus == EDD_LINK_UP_CLOSED) && (FastestPortStatus.LinkStatus != EDD_LINK_UP))
        {
            if (FastestPortStatus.LinkStatus != EDD_LINK_UP_CLOSED)
            {
                FastestPortStatus.LinkStatus    = EDD_LINK_UP_CLOSED;
                FastestPortStatus.LinkSpeed     = EDD_LINK_UNKNOWN;
                FastestPortStatus.LinkDuplexity = EDD_LINK_UNKNOWN;
            }
            bCheckSpeedDuplexity = LSA_TRUE;
        }
        else if (   (pDDB->SWI.LinkStatus[i].LinkStatus == EDD_LINK_UP_DISABLED) 
                 && (FastestPortStatus.LinkStatus != EDD_LINK_UP) 
                 && (FastestPortStatus.LinkStatus != EDD_LINK_UP_CLOSED)
                )
        {
            if (FastestPortStatus.LinkStatus != EDD_LINK_UP_DISABLED)
            {
                FastestPortStatus.LinkStatus    = EDD_LINK_UP_DISABLED;
                FastestPortStatus.LinkSpeed     = EDD_LINK_UNKNOWN;
                FastestPortStatus.LinkDuplexity = EDD_LINK_UNKNOWN;
            }
            bCheckSpeedDuplexity = LSA_TRUE;
        }
        else
        {   
            bCheckSpeedDuplexity = LSA_FALSE;
        }

        if (bCheckSpeedDuplexity)
        {
            if (pDDB->SWI.LinkStatus[i].LinkSpeed > FastestPortStatus.LinkSpeed)
            {
                FastestPortStatus.LinkSpeed     = pDDB->SWI.LinkStatus[i].LinkSpeed;
                FastestPortStatus.LinkDuplexity = pDDB->SWI.LinkStatus[i].LinkDuplexity;
            }
            else
            {   
                if (   (pDDB->SWI.LinkStatus[i].LinkSpeed == FastestPortStatus.LinkSpeed) 
                    && (pDDB->SWI.LinkStatus[i].LinkDuplexity > FastestPortStatus.LinkDuplexity)
                   )
                {
                    FastestPortStatus.LinkDuplexity = pDDB->SWI.LinkStatus[i].LinkDuplexity;
                }
            }
        }
    }//for

    if (   (pDDB->SWI.LinkStatus[EDD_PORT_ID_AUTO].LinkStatus    != FastestPortStatus.LinkStatus)
        || (pDDB->SWI.LinkStatus[EDD_PORT_ID_AUTO].LinkSpeed     != FastestPortStatus.LinkSpeed)
        || (pDDB->SWI.LinkStatus[EDD_PORT_ID_AUTO].LinkDuplexity != FastestPortStatus.LinkDuplexity)
       )
    {       
        pDDB->SWI.LinkStatus[EDD_PORT_ID_AUTO].LinkStatus       = FastestPortStatus.LinkStatus;
        pDDB->SWI.LinkStatus[EDD_PORT_ID_AUTO].LinkSpeed        = FastestPortStatus.LinkSpeed;
        pDDB->SWI.LinkStatus[EDD_PORT_ID_AUTO].LinkDuplexity    = FastestPortStatus.LinkDuplexity;
        
        bParametersChanged = LSA_TRUE;
    }

    return (bParametersChanged);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GENLinkIndProvide                      +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+                             LSA_BOOL                   *bIndicate       +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters          (in) +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                    (in) +*/
/*+  bIndicate  : Indicates whether or not the RQB was queued.         (out)+*/
/*+               If bIndicate is LSA_TRUE the caller has to return         +*/
/*+               the RQB back to the user.                                 +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function determines whether the passed RQB parameters+*/
/*+               are identical to the interal EDDP ones. If so, the RQB is +*/
/*+               queued until an event occurs that changes one or more     +*/
/*+               parameters. Otherwise the RQB (filled with the current    +*/
/*+               parameters) is returned immediately by the caller.        +*/ 
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static EDD_RSP EDDP_LOCAL_FCT_ATTR  EDDP_GENLinkIndProvide(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB,
    LSA_BOOL                    *bIndicate)
{
    EDD_RSP                                         Status;
    EDD_UPPER_LINK_STATUS_IND_PROVIDE_PTR_TYPE      pParam;

    EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_GENLinkIndProvide(pRQB=0x%X)", pHDB->Handle, pRQB);

    pParam = (EDD_UPPER_LINK_STATUS_IND_PROVIDE_PTR_TYPE) pRQB->pParam;
    
    *bIndicate = LSA_TRUE; // Set default behaviour.

    if (pParam->PortID > pDDB->SWI.PortCnt)
    {
        Status = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR(pHDB->pDDB, Status);
        EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:%2X] EDDP_GENLinkIndProvide(): ERROR -> PortID(%d) > PortCnt(%d)", 
            pHDB->Handle, pParam->PortID, pDDB->SWI.PortCnt);
    }
    else
    {
        if (EDDP_LINK_IND_PROVIDE_UNDEFINED != pHDB->LinkIndReqType[pParam->PortID])
        {
            Status = EDD_STS_ERR_SEQUENCE;
            EDDP_DBG_SET_DETAIL_ERR(pHDB->pDDB, Status);
            EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:%2X] EDDP_GENLinkIndProvide(): ERROR -> Only one RQB at a time is allowed with EDD_SRV_LINK_STATUS_IND_PROVIDE! PortID=%d", 
                pHDB->Handle, pParam->PortID);
        }
        else
        {            
            if (   (pParam->Status != pDDB->SWI.LinkStatus[pParam->PortID].LinkStatus)
                || (pParam->Mode   != pDDB->SWI.LinkStatus[pParam->PortID].LinkDuplexity)
                || (pParam->Speed  != pDDB->SWI.LinkStatus[pParam->PortID].LinkSpeed)
               )
            {   
                // Parameters from RQB differ from current parameters. Copy current params into RQB.
                EDDP_GENSetLinkStatusRQB (pParam, &(pDDB->SWI.LinkStatus[pParam->PortID]));
                Status = EDD_STS_OK;
            }
            else
            {
                *bIndicate = LSA_FALSE;
                EDDP_GlbRQBQueueAddToEnd (&(pHDB->LinkIndReq[pParam->PortID]), pRQB);
                pHDB->LinkIndReqType[pParam->PortID] = EDDP_LINK_IND_PROVIDE_STANDARD;
                Status = EDD_STS_OK;
            }
        }
    }

    EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] OUT: EDDP_GENLinkIndProvide(bIndicate=%d)", pHDB->Handle, *bIndicate);

    return (Status);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GENLinkIndProvide_Ext                  +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+                             LSA_BOOL                   *bIndicate       +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters          (in) +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                    (in) +*/
/*+  bIndicate  : Indicates whether or not the RQB was queued.         (out)+*/
/*+               If bIndicate is LSA_TRUE the caller has to return         +*/
/*+               the RQB back to the user.                                 +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function determines whether the passed RQB parameters+*/
/*+               are identical to the interal EDDP ones. If so, the RQB is +*/
/*+               queued until an event occurs that changes one or more     +*/
/*+               parameters. Otherwise the RQB (filled with the current    +*/
/*+               parameters) is returned immediately by the caller.        +*/
/*+               Difference to EDDP_GENLinkIndProvide: more parameters are +*/
/*+               compared and PordID EDD_PORT_ID_AUTO is not allowed.      +*/ 
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static EDD_RSP EDDP_LOCAL_FCT_ATTR  EDDP_GENLinkIndProvide_Ext(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB,
    LSA_BOOL                    *bIndicate)
{
    EDD_RSP                                         Status;
    EDD_UPPER_LINK_STATUS_IND_PROVIDE_PTR_TYPE      pParam;

    EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_GENLinkIndProvide_Ext(pRQB=0x%X)", pHDB->Handle, pRQB);

    pParam = (EDD_UPPER_LINK_STATUS_IND_PROVIDE_PTR_TYPE) pRQB->pParam;
    
    *bIndicate = LSA_TRUE; // Set default behaviour.
    
    if (pParam->PortID > pDDB->SWI.PortCnt)
    {
        Status = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR(pHDB->pDDB, Status);
        EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:%2X] EDDP_GENLinkIndProvide_Ext(): ERROR -> PortID(%d) > PortCnt(%d)", 
            pHDB->Handle, pParam->PortID, pDDB->SWI.PortCnt);
    }
    else
    {
        if (EDDP_LINK_IND_PROVIDE_UNDEFINED != pHDB->LinkIndReqType[pParam->PortID])
        {
            Status = EDD_STS_ERR_SEQUENCE;
            EDDP_DBG_SET_DETAIL_ERR(pHDB->pDDB, Status);
            EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:%2X] EDDP_GENLinkIndProvide_Ext(): ERROR -> Only one RQB at a time is allowed with EDD_SRV_LINK_STATUS_IND_PROVIDE_EXT! PortID=%d", 
                pHDB->Handle, pParam->PortID);
        }
        else if (EDD_PORT_ID_AUTO == pParam->PortID)    // EDD_PORT_ID_AUTO is not allowed for the _EXT type of this service.
        {
            Status = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR(pHDB->pDDB, Status);
            EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:%2X] EDDP_GENLinkIndProvide_Ext(): ERROR -> EDD_PORT_ID_AUTO is not allowed for the _EXT type of this service", 
                pHDB->Handle);
        }
        else
        {                        
            if ( EDDP_GENSetLinkStatusRQB(pParam, &(pDDB->SWI.LinkStatus[pParam->PortID])) )      
            {   
                //Parameters from RQB differ from current parameters. Indicate current link state immediately.
                Status = EDD_STS_OK;
            }
            else
            {
                *bIndicate = LSA_FALSE;
                EDDP_GlbRQBQueueAddToEnd (&(pHDB->LinkIndReq[pParam->PortID]), pRQB);
                pHDB->LinkIndReqType[pParam->PortID] = EDDP_LINK_IND_PROVIDE_EXTENDED;
                Status = EDD_STS_OK;
            }
        }

        EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] OUT: EDDP_GENLinkIndProvide_Ext (.., bIndicate = %d )", 
            pHDB->Handle, *bIndicate);
    }

    return (Status);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GENRequest                             +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters          (in) +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         Opcode:     EDD_OPC_REQUEST                 +*/
/*+     LSA_HANDLE_TYPE         Handle:                                     +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user (not used)           +*/
/*+     EDD_SERVICE             Service:    EDD_SRV_xxx                     +*/
/*+                                         EDD_SRV_xxx                     +*/
/*+                                         EDD_SRV_xxx                     +*/
/*+     LSA_RESULT              Status:     Return status                   +*/
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:     Depends on service              +*/
/*+                                                                         +*/
/*+  RQB-return values via callback-function depend on service              +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function handles the following gobal services:       +*/
/*+                                                                         +*/
/*+               EDD_SRV_LED_BLINK                                         +*/
/*+                                                                         +*/
/*+               If there's a parameter error we will set EDD_STS_ERR_PARAM+*/
/*+               and call the handle-callbackfunction and abort the request+*/
/*+                                                                         +*/
/*+               EDD_STS_ERR_SERVICE is set if we get an invalid service.  +*/
/*+               EDD_STS_ERR_CHANNEL_USE is set if the service is invalid  +*/
/*+               for this channel (i.e. not supported by this channel)     +*/
/*+                                                                         +*/
/*+               If the request is valid, it is queued or executed.        +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GENRequest(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP         Response;
    LSA_BOOL        bIndicate;

    EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] >>> EDDP_GENRequest(pRQB=0x%X)", pHDB->Handle, pRQB);

    Response    = EDD_STS_OK;
    bIndicate   = LSA_TRUE; 

    if (LSA_HOST_PTR_ARE_EQUAL(pRQB->pParam, LSA_NULL))
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR(pHDB->pDDB, Response);
        EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_GENRequest(): ERROR -> RQB->pParam=NULL, Response=0x%X", 
            pHDB->Handle, Response);
    }

    if (Response == EDD_STS_OK)
    {
        switch (EDD_RQB_GET_SERVICE(pRQB))
        {
            //-------------------------------------------------------------
            case EDD_SRV_LED_BLINK:
            //-------------------------------------------------------------
            {
                // LED Blink request. Starts blinking and confirms the request immediately.   
                EDDP_UPPER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_LED_BLINK", pHDB->Handle);
                Response = EDDP_GENLEDBlink (pRQB, pDDB, pHDB);
            }
            break;

            //-------------------------------------------------------------
            case EDD_SRV_GET_LINK_STATUS:
            //-------------------------------------------------------------
            {  
                #if (EDDP_CFG_TRACE_MODE != 0)
                EDD_UPPER_GET_LINK_STATUS_PTR_TYPE      pRqbParam = (EDD_UPPER_GET_LINK_STATUS_PTR_TYPE) pRQB->pParam;
                #endif
                EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_GET_LINK_STATUS (PortID=%d)", 
                    pHDB->Handle, pRqbParam->PortID);

                Response = EDDP_GENGetLinkStatus (pRQB, pDDB);
            }
            break;

            //-------------------------------------------------------------
            case EDD_SRV_GET_PARAMS:
            //-------------------------------------------------------------
            {
                EDDP_UPPER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_GET_PARAMS", pHDB->Handle);
                EDDP_GEN_Get_Params (pRQB, pDDB, pHDB);
            }
            break;

            //-------------------------------------------------------------
            case EDD_SRV_SET_IP:
            //-------------------------------------------------------------
            {  
                EDDP_UPPER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_SET_IP", pHDB->Handle);                          
                Response = EDDP_GEN_Set_IP_Address (pRQB, pDDB);
            }
            break;

            //-------------------------------------------------------------
            case EDD_SRV_LINK_STATUS_IND_PROVIDE:
            //-------------------------------------------------------------
            {
                #if (EDDP_CFG_TRACE_MODE != 0)
                EDD_UPPER_LINK_STATUS_IND_PROVIDE_PTR_TYPE      pRqbParam = (EDD_UPPER_LINK_STATUS_IND_PROVIDE_PTR_TYPE) pRQB->pParam;
                #endif
                EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_LINK_STATUS_IND_PROVIDE (PortID=%d)", 
                    pHDB->Handle, pRqbParam->PortID);

                Response = EDDP_GENLinkIndProvide (pRQB, pDDB, pHDB, &bIndicate);
            }
            break;

            //-------------------------------------------------------------
            case EDD_SRV_LINK_STATUS_IND_PROVIDE_EXT:
            //-------------------------------------------------------------
            {
                #if (EDDP_CFG_TRACE_MODE != 0)
                EDD_UPPER_LINK_STATUS_IND_PROVIDE_PTR_TYPE      pRqbParam = (EDD_UPPER_LINK_STATUS_IND_PROVIDE_PTR_TYPE) pRQB->pParam;
                #endif
                EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_LINK_STATUS_IND_PROVIDE_EXT (PortID=%d)", 
                    pHDB->Handle, pRqbParam->PortID);

                Response = EDDP_GENLinkIndProvide_Ext (pRQB, pDDB, pHDB, &bIndicate);
            }
            break;

            //-------------------------------------------------------------
            case EDD_SRV_GET_PORT_PARAMS:
            //-------------------------------------------------------------
            {    
                #if (EDDP_CFG_TRACE_MODE != 0)
                EDD_UPPER_GET_PORT_PARAMS_PTR_TYPE      pRqbParam = (EDD_UPPER_GET_PORT_PARAMS_PTR_TYPE) pRQB->pParam;
                #endif
                EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_GET_PORT_PARAMS (PortID=%d)", 
                    pHDB->Handle, pRqbParam->PortID);

                Response = EDDP_GEN_Get_Port_Params (pRQB, pDDB);
            }
            break;

            //-------------------------------------------------------------
            case EDD_SRV_SET_REMOTE_PORT_STATE:
            //-------------------------------------------------------------
            {
                #if (EDDP_CFG_TRACE_MODE != 0)
                EDD_UPPER_SET_REMOTE_PORT_STATE_PTR_TYPE        pRqbParam = (EDD_UPPER_SET_REMOTE_PORT_STATE_PTR_TYPE) pRQB->pParam;
                #endif
                EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_SET_REMOTE_PORT_STATE (PortID=%d)", 
                    pHDB->Handle, pRqbParam->PortID);

                // first notify KRISC32
                EDDP_K32RemotePortState (pRQB, pDDB, pHDB);
                Response = EDDP_GENSetRemotePortState (pRQB, pDDB);
                
#if 1   /** RZ/T2M additional code */
                EDDP_LL_CoreRequest(pRQB, pHDB);    // discard response
#endif
            }
            break;

            //-------------------------------------------------------------
            case EDD_SRV_MULTICAST:
            //-------------------------------------------------------------
            {
                #if (EDDP_CFG_TRACE_MODE != 0)
                EDD_UPPER_MULTICAST_PTR_TYPE        pRqbParam = (EDD_UPPER_MULTICAST_PTR_TYPE) pRQB->pParam;
                #endif
                EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_MULTICAST (Mode=0x%X)", 
                    pHDB->Handle, pRqbParam->Mode);

                Response = EDDP_GEN_Add_Remove_Multicast_Address (pRQB, pDDB);
            }
            break;

            //-------------------------------------------------------------
            case EDD_SRV_MULTICAST_FWD_CTRL:
            //-------------------------------------------------------------
            {
                EDDP_UPPER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_MULTICAST_FWD_CTRL", pHDB->Handle);
                Response = EDDP_GEN_Multicast_Fwd_Ctrl ((EDD_UPPER_MULTICAST_FWD_CTRL_PTR_TYPE)(pRQB->pParam), pDDB);
            }
            break;

            //-------------------------------------------------------------
            case EDD_SRV_SENDCLOCK_CHANGE:
            //-------------------------------------------------------------
            {
                // it will be traced in EDDP_GlbRequestFromGlbCrtRQBHandler()
                //EDDP_UPPER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"[H:%2X] >>> Request: EDD_SRV_SENDCLOCK_CHANGE", pHDB->Handle);
                // put this RQB to Queue
                EDDP_GlbRQBQueueAddToEnd (&(pDDB->GlbCrtRQBHandler.RqbQueue), pRQB);
                pHDB->pSWI->RunningRQBCount++;
                // and call RQB-Handler
                EDDP_GlbCrtRQBHandler (pDDB);
                // request is queued
                bIndicate = LSA_FALSE;
            }
            break;

            //-------------------------------------------------------------
            case EDD_SRV_GET_STATISTICS:
            //-------------------------------------------------------------
            {
                #if (EDDP_CFG_TRACE_MODE != 0)
                EDD_UPPER_GET_STATISTICS_PTR_TYPE       pRqbParam = (EDD_UPPER_GET_STATISTICS_PTR_TYPE) pRQB->pParam;
                #endif
                EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_GET_STATISTICS (PortID=%d)", 
                    pHDB->Handle, pRqbParam->PortID);

                Response = EDDP_GEN_RequestGetStatistics (pRQB, pDDB, pHDB);
            }
            break;

            //-------------------------------------------------------------
            case EDD_SRV_RESET_STATISTICS:
            //-------------------------------------------------------------
            {
                #if (EDDP_CFG_TRACE_MODE != 0)
                EDD_UPPER_RESET_STATISTICS_PTR_TYPE     pRqbParam = (EDD_UPPER_RESET_STATISTICS_PTR_TYPE) pRQB->pParam;
                #endif
                EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_RESET_STATISTICS (PortID=%d)", 
                    pHDB->Handle, pRqbParam->PortID);

                Response = EDDP_GEN_RequestResetStatistics (pRQB, pDDB, pHDB);
            }
            break;

            //-------------------------------------------------------------
            case EDD_SRV_GET_STATISTICS_ALL:
            //-------------------------------------------------------------
            {
                #if (EDDP_CFG_TRACE_MODE != 0)
                EDD_UPPER_GET_STATISTICS_ALL_PTR_TYPE       pRqbParam = (EDD_UPPER_GET_STATISTICS_ALL_PTR_TYPE) pRQB->pParam;
                #endif
                EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_GET_STATISTICS_ALL (Reset=0x%X)", 
                    pHDB->Handle, pRqbParam->Reset);

                Response = EDDP_GEN_RequestGetStatisticsAll (pRQB, pDDB, pHDB);
            }
            break;
            
#if 1	/** RZ/T2M additional codes */
			
			//-------------------------------------------------------------
			case EDD_SRV_SET_LINE_DELAY:
			//-------------------------------------------------------------
            {
                Response = EDDP_RequestSetLineDelay (pRQB, pDDB, pHDB);
            }
            break;
			
			//-------------------------------------------------------------
            case EDD_SRV_SET_SYNC_STATE:
			//-------------------------------------------------------------
			{
				Response = EDDP_GEN_RequestSetSyncState( pRQB, pDDB, pHDB );
			}
			break;	
			
			//-------------------------------------------------------------
            case EDD_SRV_SET_SYNC_TIME:
			//-------------------------------------------------------------
            {   
                /**
                 * RZ/T2M EDDP:
                 *   This operation will be implemented by another way. 
                 *   Here is no operation and does not return error code. 
                 */
            }
            break;
			
#endif 

            //-------------------------------------------------------------
            default:
            //-------------------------------------------------------------
            {
                Response = EDD_STS_ERR_SERVICE;
                EDDP_DBG_SET_DETAIL_ERR(pHDB->pDDB, Response);
                EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_GENRequest(): ERROR -> Unknown/Unsupported Service(0x%X)!", 
                    pHDB->Handle, EDD_RQB_GET_SERVICE(pRQB));
            }
            break;
        }
    }

    if (bIndicate)
    {
        EDDP_GlbRequestFinish (pHDB, pRQB, Response);
    }

    EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] <<< EDDP_GENRequest(): Response=0x%X", pHDB->Handle, Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GENLEDBlink                            +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters          (in) +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function invokes the timer for LED blink and sets    +*/
/*+               corresponding parameters.                                 +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GENLEDBlink(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP		                    Response = EDD_STS_OK;
    LSA_UINT32                      PortID, HwPortID;
    LSA_UINT16          const       TotalBlinkDurationInSeconds = 3;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pRQB);
#endif

    EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_GENLEDBlink(pRQB=0x%X)", pHDB->Handle, pRQB);

    EDDP_CORE_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:%2X] EDDP_GenLEDBlink(): TotalBlinkDurationInSeconds=%d, TotalBlinkCountIn500ms=%d, isBlinking=%d", 
        pHDB->Handle, TotalBlinkDurationInSeconds, pDDB->SWI.LEDBlink.TotalBlinkCountIn500ms, pDDB->SWI.LEDBlink.isBlinking);

    /* In case blinking is still running, the blinking duration will be set to TotalBlinkDurationInSeconds = 3 */
    if (pDDB->SWI.LEDBlink.isBlinking)
    {
        pDDB->SWI.LEDBlink.TotalBlinkDurationInSeconds = TotalBlinkDurationInSeconds;
        pDDB->SWI.LEDBlink.TotalBlinkCountIn500ms   = 0;

        EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:%2X] EDDP_GenLEDBlink(): The blinking duration is extended to TotalBlinkDurationInSeconds=%d", 
            pHDB->Handle, pDDB->SWI.LEDBlink.TotalBlinkDurationInSeconds);
    }
    else
    {
        pDDB->SWI.LEDBlink.TotalBlinkDurationInSeconds = TotalBlinkDurationInSeconds;

        /* Invoke init things before starting to blink and turn all LEDs on */
        for ( PortID = 1; PortID <= pDDB->SWI.PortCnt; PortID++ )
        {   
            HwPortID = EDDP_SWI_get_HwPortID (pDDB, PortID);

            /* start blinking */
            Response = pHDB->pDDB->SWI.PHY_FunctionPointers[HwPortID].EDDP_PHY_LED_BlinkBegin(pHDB->pDDB->hDDB, pHDB->pDDB->hSysDev, HwPortID);
            EDDP_ASSERT_FALSE(Response != EDD_STS_OK);
            /* LED ON */
            Response = pHDB->pDDB->SWI.PHY_FunctionPointers[HwPortID].EDDP_PHY_LED_BlinkSetMode(pHDB->pDDB->hDDB, pHDB->pDDB->hSysDev, HwPortID, EDDP_LED_MODE_ON); 
            EDDP_ASSERT_FALSE(Response != EDD_STS_OK);
        } 

        pDDB->SWI.LEDBlink.isLED_ON                 = LSA_TRUE;
        pDDB->SWI.LEDBlink.TotalBlinkCountIn500ms   = 0;
        pDDB->SWI.LEDBlink.isBlinking               = LSA_TRUE;

        /* Start cyclic timer which then toggles LEDs */
        EDDP_CoreTimerStart (pDDB->SWI.LEDBlink.Timer, (LSA_UINT16) EDDP_LED_BLINK_ON_OFF_DURATION_IN_100ms);
    }

    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] OUT: EDDP_GENLEDBlink()", pHDB->Handle);

    return (EDD_STS_OK);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GENLEDBlink_Toggle                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called by a cyclic timer and toggles     +*/
/*+               LEDs for service EDD_SRV_LED_BLINK.                       +*/
/*+               TimerID: EDDP_TIMER_BLINK                                 +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GENLEDBlink_Toggle( 
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP			Response = EDD_STS_OK;
    LSA_UINT32      PortID, HwPortID;

    /* Since start of the timer or last call of this function there are 500ms elapsed. */
    pDDB->SWI.LEDBlink.TotalBlinkCountIn500ms++;
    
    EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_GENLEDBlink_Toggle(): TotalBlinkCountIn500ms=%d, TotalBlinkDurationInSeconds=%d", 
        pDDB->SWI.LEDBlink.TotalBlinkCountIn500ms, pDDB->SWI.LEDBlink.TotalBlinkDurationInSeconds);

    if (pDDB->SWI.LEDBlink.TotalBlinkCountIn500ms < (pDDB->SWI.LEDBlink.TotalBlinkDurationInSeconds * 2))
    {
        /* Toggle all LEDs of this interface */
        for ( PortID = 1; PortID <= pDDB->SWI.PortCnt; PortID++ )
        {   
            HwPortID = EDDP_SWI_get_HwPortID (pDDB, PortID);
            /* LED ON/OFF */
            Response = pDDB->SWI.PHY_FunctionPointers[HwPortID].EDDP_PHY_LED_BlinkSetMode(pDDB->hDDB, pDDB->hSysDev, HwPortID, pDDB->SWI.LEDBlink.isLED_ON ? EDDP_LED_MODE_OFF : EDDP_LED_MODE_ON);
            EDDP_ASSERT_FALSE(Response != EDD_STS_OK);
        }
        pDDB->SWI.LEDBlink.isLED_ON = pDDB->SWI.LEDBlink.isLED_ON ? LSA_FALSE : LSA_TRUE;
    }
    else
    {
        /* Total blink time elapsed -> Stop cyclic timer. */
        EDDP_CoreTimerStop (pDDB->SWI.LEDBlink.Timer);

        pDDB->SWI.LEDBlink.isBlinking                   = LSA_FALSE;
        pDDB->SWI.LEDBlink.TotalBlinkCountIn500ms       = 0;
        pDDB->SWI.LEDBlink.TotalBlinkDurationInSeconds  = 0;
        
        /* Restore LEDs to initial state after blinking. */
        for ( PortID = 1; PortID <= pDDB->SWI.PortCnt; PortID++ )
        {   
            HwPortID = EDDP_SWI_get_HwPortID (pDDB, PortID);
            /* stop blinking */
            Response = pDDB->SWI.PHY_FunctionPointers[HwPortID].EDDP_PHY_LED_BlinkEnd(pDDB->hDDB, pDDB->hSysDev, HwPortID);
            EDDP_ASSERT_FALSE(Response != EDD_STS_OK);
        }       
    }
    
    EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_GENLEDBlink_Toggle()");
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_GEN_ReadPhyStatusRegs                 +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32					   HWPortID     +*/
/*+                             EDDP_PHY_LINK_STATUS_PTR_TYPE  pLinkStatus  +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  HWPortID   : Hardware PortID 1..EDDP_MAX_PORT_CNT                (in)  +*/
/*+  pLinkStatus: points to a struct for returning data               (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Utility function for EDDP_GEN_ISR_LinkChange()            +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR _EDDP_GEN_ReadPhyStatusRegs(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_UINT32			            HWPortID,
    EDDP_PHY_LINK_STATUS_PTR_TYPE   pLinkStatus)
{
    LSA_UINT32  reg_address;

    EDDP_ASSERT_FALSE((LSA_HOST_PTR_ARE_EQUAL(pLinkStatus, LSA_NULL)));

    // select the right register address for current HwPortID "i"
    reg_address = (HWPortID==1 ? PNIP_REG_PHYSTATUS1_P1 : 
                  (HWPortID==2 ? PNIP_REG_PHYSTATUS1_P2 : 
                  (HWPortID==3 ? PNIP_REG_PHYSTATUS1_P3 : 
                  (HWPortID==4 ? PNIP_REG_PHYSTATUS1_P4 : 0))));
    pLinkStatus->Link = (EDDP_HAL_REG32_READ(pDDB, reg_address) & PNIP_REG_PHYSTATUS1_P1__MSK_PHY_STATUS1);

    // select the right register address for current HwPortID "i"
    reg_address = (HWPortID==1 ? PNIP_REG_PHYSTATUS2_P1 : 
                  (HWPortID==2 ? PNIP_REG_PHYSTATUS2_P2 : 
                  (HWPortID==3 ? PNIP_REG_PHYSTATUS2_P3 :
                  (HWPortID==4 ? PNIP_REG_PHYSTATUS2_P4 : 0))));
    pLinkStatus->Speed = (EDDP_HAL_REG32_READ(pDDB, reg_address) & PNIP_REG_PHYSTATUS2_P1__MSK_PHY_STATUS2);

    // select the right register address for current HwPortID "i"
    reg_address = (HWPortID==1 ? PNIP_REG_PHYSTATUS3_P1 : 
                  (HWPortID==2 ? PNIP_REG_PHYSTATUS3_P2 : 
                  (HWPortID==3 ? PNIP_REG_PHYSTATUS3_P3 : 
                  (HWPortID==4 ? PNIP_REG_PHYSTATUS3_P4 : 0))));
    pLinkStatus->Duplexity = (EDDP_HAL_REG32_READ(pDDB, reg_address) & PNIP_REG_PHYSTATUS3_P1__MSK_PHY_STATUS3);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GEN_ISR_LinkChange                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_UINT32                  EventNr         +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  EventNr    : Event Number (Interrupt Number)                     (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE after a link status		+*/
/*+				  change interrupt occurred. It is executed within			+*/
/*+               LOW context and determines the current link status of all	+*/
/*+				  ports. Also the finite state machine for controlling		+*/
/*+				  the ports is triggerd. Changes are stored in the DDB		+*/
/*+				  structure and are indicated to users if appropiate RQBs	+*/
/*+				  are present.												+*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#if 0
#define EDDP_STATUSCHANGE_MSK_PWRUP     (PNIP_REG_STATUSCHANGE__MSK_PHY_PWRUP_P1)
#define EDDP_STATUSCHANGE_MSK_STATUS    (PNIP_REG_STATUSCHANGE__MSK_STATUSCHANGE1_P1|PNIP_REG_STATUSCHANGE__MSK_STATUSCHANGE2_P1|PNIP_REG_STATUSCHANGE__MSK_STATUSCHANGE3_P1)
#define EDDP_STATUSCHANGE_MSK_ALL       (EDDP_STATUSCHANGE_MSK_STATUS | EDDP_STATUSCHANGE_MSK_PWRUP)
#else
#define EDDP_STATUSCHANGE_MSK_PWRUP     (PNIP_REG_STATUSCHANGE__MSK_PHY_PWRUP_P1 | PNIP_REG_STATUSCHANGE__MSK_PHY_PWRUP_P2 | PNIP_REG_STATUSCHANGE__MSK_PHY_PWRUP_P3)
#define EDDP_STATUSCHANGE_MSK_STATUS    ( (PNIP_REG_STATUSCHANGE__MSK_STATUSCHANGE1_P1|PNIP_REG_STATUSCHANGE__MSK_STATUSCHANGE2_P1|PNIP_REG_STATUSCHANGE__MSK_STATUSCHANGE3_P1) |\
										  (PNIP_REG_STATUSCHANGE__MSK_STATUSCHANGE1_P2|PNIP_REG_STATUSCHANGE__MSK_STATUSCHANGE2_P2|PNIP_REG_STATUSCHANGE__MSK_STATUSCHANGE3_P2) |\
										  (PNIP_REG_STATUSCHANGE__MSK_STATUSCHANGE1_P3|PNIP_REG_STATUSCHANGE__MSK_STATUSCHANGE2_P3|PNIP_REG_STATUSCHANGE__MSK_STATUSCHANGE3_P3) )
#define EDDP_STATUSCHANGE_MSK_ALL       (EDDP_STATUSCHANGE_MSK_STATUS | EDDP_STATUSCHANGE_MSK_PWRUP)

#endif
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GEN_ISR_LinkChange(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr)
{
    EDD_RSP                     Response = EDD_STS_OK;
    LSA_UINT32			        RegStatusChange;
    LSA_UINT32			        PortID, HwPortID;
    LSA_UINT32			        IF_Number;
    LSA_UINT32                  reg_val, reg_address;
    EDDP_PHY_LINK_STATUS_TYPE   PhyLinkStatus; 

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(EventNr);
#endif

    LSA_UNUSED_ARG(EventNr);

    EDDP_LL_LINK_CHANGE_DONE(pDDB);

    // read STATUSCHANGE to get reason(s) of this interrupt
    RegStatusChange = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_STATUSCHANGE);
    // clear change indication bit(s)
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_STATUSCHANGE, RegStatusChange);

    EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_GEN_ISR_LinkChange(EventNr=0x%X): RegStatusChange=0x%X", 
        EventNr, RegStatusChange);

    // ***** detect changes in PortStatus for all HW-Ports
    // ***** loop over all port of this DDB
    for ( HwPortID=1; HwPortID <= pDDB->SWI.PortCnt; HwPortID++ )
    { 
        // ***** somethings changed ?
        if (RegStatusChange & EDDP_STATUSCHANGE_MSK_ALL)
        {
            // ***** convert HwPortID into PortID and IF number
            PortID = EDDP_SWI_get_PortID_and_IF (pDDB, HwPortID, &IF_Number);
            // get PhyLinkStatus
            _EDDP_GEN_ReadPhyStatusRegs (pDDB, HwPortID, &PhyLinkStatus);

            if (0 == pDDB->SWI.PHY_FunctionPointers[HwPortID].EDDP_PHY_GetLinkStatus)
            {
            	 EDDP_PHY_TYPE PHYType = EDDP_PHY_TYPE_UNKNOWN;
            	//EDDP_SWI_FinishPHYSetup (pDDB, HwPortID, LSA_TRUE /*bKeepPhyStatus*/);
            	EDDP_PHY_GetFunctionPointers(pDDB->hDDB, pDDB->hSysDev, HwPortID, reg_val, &(pDDB->SWI.PHY_FunctionPointers[HwPortID]), &PHYType);
            }

            // poll state on HwPortID has changed
            if (RegStatusChange & EDDP_STATUSCHANGE_MSK_PWRUP)
            {
                EDDP_SWI_GEN_PhySTS_StateMachine(pDDB, EDDP_PHYSTS_SM_TRIGGER_INT_STATUS_CHANGE, HwPortID-1);
            }

            if (0 == pDDB->SWI.PHY_FunctionPointers[HwPortID].EDDP_PHY_GetLinkStatus)
            {
                EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_GEN_ISR_LinkChange(): FATAL -> PHY function pointer EDDP_PHY_GetLinkStatus is NULL! UsrPortID(%d), HwPortID(%d)", 
                    PortID, HwPortID);
                EDDP_FATAL(EDDP_FATAL_ERR_NULL_PTR);
            }

            // convert register values into EDD defines (done by system adaption)
            Response = pDDB->SWI.PHY_FunctionPointers[HwPortID].EDDP_PHY_GetLinkStatus(pDDB->hDDB, pDDB->hSysDev, pDDB->Hardware, HwPortID, &PhyLinkStatus);
            EDDP_ASSERT_FALSE(Response != EDD_STS_OK);
            EDDP_ASSERT_FALSE((EDD_PORT_MDIX_DISABLED != PhyLinkStatus.IsMDIX) && (EDD_PORT_MDIX_ENABLED != PhyLinkStatus.IsMDIX));
            EDDP_ASSERT_FALSE((EDD_PORT_IS_NOT_WIRELESS != PhyLinkStatus.IsWireless) && (EDD_PORT_IS_WIRELESS != PhyLinkStatus.IsWireless));
            EDDP_ASSERT_FALSE((EDD_PORT_OPTICALTYPE_ISOTHER != PhyLinkStatus.OpticalTransType) && (EDD_PORT_OPTICALTYPE_ISPOF != PhyLinkStatus.OpticalTransType));

            // either link, speed or duplexity on HwPortID "i" have changed
            if (RegStatusChange & EDDP_STATUSCHANGE_MSK_STATUS)
            {
                // PNIP only supports 100Mbit/Fullduplex -> we go into FATAL because system adaption did something wrong
                if (   (PhyLinkStatus.Link == EDD_LINK_UP)
                    && (PhyLinkStatus.Speed != EDD_LINK_SPEED_100)
                    /*&& (PhyLinkStatus.Duplexity != EDD_LINK_MODE_FULL)  note: without duplexity check to support 100MBit HalfDuplex with some restrictions !!!*/
                   )
                {
                    EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H:--] EDDP_GEN_ISR_LinkChange(): LinkUP but no 100Mbit! UsrPortID(%d), HwPortID(%d)", 
                        PortID, HwPortID);
                }

                EDDP_CORE_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] EDDP_GEN_ISR_LinkChange(UsrPortID=%d, HwPortID=%d): Link(1=up,2=down): %d, Speed(1=10,2=100): %d, Duplexity(1=half,2=full): %d", 
                    PortID, HwPortID, PhyLinkStatus.Link, PhyLinkStatus.Speed, PhyLinkStatus.Duplexity);
                EDDP_CORE_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] EDDP_GEN_ISR_LinkChange(UsrPortID=%d, HwPortID=%d): MediaType(1=cop,2=fiber,3=radio): %d, MAUType: %d", 
                    PortID, HwPortID, PhyLinkStatus.MediaType, PhyLinkStatus.MAUType);

                if (PhyLinkStatus.Link == EDD_LINK_UP)
                {
                    /* ---------------- */
                    /* Link-Up          */
                    /* ---------------- */
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P)
                    // enable MACControl_Px.Transmit/Receive
                    reg_address = (HwPortID==1 ? PNIP_REG_MACCONTROL_P1 : 
                                  (HwPortID==2 ? PNIP_REG_MACCONTROL_P2 : 
                                  (HwPortID==3 ? PNIP_REG_MACCONTROL_P3 : 
                                  (HwPortID==4 ? PNIP_REG_MACCONTROL_P4 : 0))));
                    reg_val = (PNIP_REG_MACCONTROL_P1__MSK_TRANSMIT_ENABLE | PNIP_REG_MACCONTROL_P1__MSK_RECEIVE_ENABLE);
                    EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_val);
#endif
                    // set port state according to saved state in SWI.LinkStatus[PortID].PortState
                    EDDP_SWI_Set_Port_State_HW (pDDB, pDDB->SWI.LinkStatus[PortID].PortState, HwPortID);
                }
                else
                {
                    /* ---------------- */
                    /* Link-Down        */
                    /* ---------------- */
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P)
                    // Disable Port in FlowControl_Px.DisablePort
                    reg_address = (HwPortID==1 ? PNIP_REG_FLOWCONTROL_P1 : 
                                  (HwPortID==2 ? PNIP_REG_FLOWCONTROL_P2 : 
                                  (HwPortID==3 ? PNIP_REG_FLOWCONTROL_P3 : 
                                  (HwPortID==4 ? PNIP_REG_FLOWCONTROL_P4 : 0))));
                    reg_val = EDDP_HAL_REG32_READ(pDDB, reg_address);
                    EDDP_SET_BIT_VALUE32(reg_val, 0x00000001, PNIP_REG_FLOWCONTROL_P1__MSK_DISABLEPORT, PNIP_REG_FLOWCONTROL_P1_SHFT_DISABLEPORT);
                    EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_val);

                    // enable MACControl_Px.Transmit/Transmit_HaltRequest
                    // select the right register address for current HwPortID "i"
                    reg_address = (HwPortID==1 ? PNIP_REG_MACCONTROL_P1 : 
                                  (HwPortID==2 ? PNIP_REG_MACCONTROL_P2 : 
                                  (HwPortID==3 ? PNIP_REG_MACCONTROL_P3 : 
                                  (HwPortID==4 ? PNIP_REG_MACCONTROL_P4 : 0))));
                    reg_val = (PNIP_REG_MACCONTROL_P1__MSK_TRANSMIT_ENABLE | PNIP_REG_MACCONTROL_P1__MSK_TRANSMIT_HALTREQUEST);
                    EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_val);
#endif
                }

                // update LinkStatus: store current link information in DDB
                if (   ((LSA_UINT8)PhyLinkStatus.Link == EDD_LINK_UP)
                    && (   (pDDB->SWI.LinkStatus[PortID].PortState == EDD_PORT_STATE_BLOCKING) 
                        || (pDDB->SWI.LinkStatus[PortID].PortState == EDD_PORT_STATE_LISTENING) 
                        || (pDDB->SWI.LinkStatus[PortID].PortState == EDD_PORT_STATE_LEARNING)
                       )
                   )
                {
                    // Link is Up and port is set to "Blocking"
                    pDDB->SWI.LinkStatus[PortID].LinkStatus = EDD_LINK_UP_CLOSED;
                }
                else
                {
                    if (   ((LSA_UINT8)PhyLinkStatus.Link == EDD_LINK_UP) 
                        && (pDDB->SWI.LinkStatus[PortID].PortState == EDD_PORT_STATE_DISABLE)
                       )
                    {
                        // Link is Up and port is set to "Disable"
                        pDDB->SWI.LinkStatus[PortID].LinkStatus = EDD_LINK_UP_DISABLED;
                    }
                    else
                    {      
                        // LinkStatus is from PHY
                        pDDB->SWI.LinkStatus[PortID].LinkStatus = (LSA_UINT8)PhyLinkStatus.Link;
                    }
                }
                EDDP_CORE_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_GEN_ISR_LinkChange(UsrPortID=%d, HwPortID=%d): current PortState=0x%X, new LinkStatus=0x%X", 
                    PortID, HwPortID, pDDB->SWI.LinkStatus[PortID].PortState, pDDB->SWI.LinkStatus[PortID].LinkStatus);

                // store other parameters in DDB
                EDDP_SWIFillPHYLinkStatus(&PhyLinkStatus,&( pDDB->SWI.LinkStatus[PortID]));
                pDDB->SWI.LinkStatus[PortID].LinkSpeedModeConfigured    = PhyLinkStatus.LinkSpeedModeConfigured;

                // fire link indication for this PortID
                EDDP_GENIndicateLinkChange (pDDB, PortID);

                // check if "fastest link" status has changed
                if (EDDP_GENFindFastestPort(pDDB))
                {
                    EDDP_GENIndicateLinkChange (pDDB, EDD_PORT_ID_AUTO);
                }

                // trigger PreambleLength
                EDDP_SWI_Trigger_PreambleLength (pDDB, PortID);
                // trigger RTC3 port state machine
                EDDP_SWI_Trigger_RTC3_PortState (pDDB, PortID);
            }
        }

        // shift to Port2 within StatusChange register
        EDDP_SHIFT_VALUE_RIGHT (RegStatusChange, PNIP_REG_STATUSCHANGE_SHFT_STATUSCHANGE1_P2);
    }

    EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_GEN_ISR_LinkChange()");
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GEN_Get_Params                         +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function fills all paramters requested by service    +*/
/*+               EDD_SRV_GET_PARAMS.                                       +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GEN_Get_Params(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_UPPER_GET_PARAMS_PTR_TYPE       pRqbParam;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pHDB);
#endif

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_GEN_Get_Params(pRQB=0x%X)", pHDB->Handle, pRQB);

    pRqbParam = (EDD_UPPER_GET_PARAMS_PTR_TYPE) pRQB->pParam;

    /* -------------------------------------------- */
    /* parameters for SWI                           */
    /* -------------------------------------------- */
    pRqbParam->InterfaceID                          = pDDB->InterfaceID;
    pRqbParam->HardwareType                         = pDDB->HardwareType;
    pRqbParam->TraceIdx                             = pDDB->TraceIdx, 
    pRqbParam->TimeResolutionInNs                   = EDDP_TIME_TICKS_RESOLUTION_NS;
    pRqbParam->MACAddr                              = pDDB->SWI.IF_MAC_Address;
    pRqbParam->LocalIP                              = pDDB->SWI.IF_IP_Address;
    pRqbParam->MaxPortCnt                           = (LSA_UINT16)pDDB->SWI.PortCnt;
    pRqbParam->MaxPortCntOfAllEDD                   = pDDB->MaxPortCntOfAllEDD;
    pRqbParam->MaxInterfaceCntOfAllEDD              = pDDB->MaxInterfaceCntOfAllEDD;
    pRqbParam->MaxMRP_Instances                     = pDDB->SWI.MaxMRP_Instances;
    pRqbParam->MRPDefaultRoleInstance0              = pDDB->SWI.MRPDefaultRoleInstance0;
    pRqbParam->MRPSupportedRole                     = pDDB->SWI.MRPSupportedRole;
    pRqbParam->MRPSupportedMultipleRole             = pDDB->SWI.MRPSupportedMultipleRole;
    pRqbParam->MRAEnableLegacyMode                  = pDDB->SWI.MRAEnableLegacyMode;
    pRqbParam->MaxMRPInterconn_Instances            = pDDB->SWI.MaxMRPInterconn_Instances;
    pRqbParam->SupportedMRPInterconnRole            = pDDB->SWI.SupportedMRPInterconnRole;

    pRqbParam->FeatureMask = 0x0;

    if (EDD_FEATURE_ENABLE == pDDB->FeatureSupport.AdditionalForwardingRulesSupported)
    {
      pRqbParam->FeatureMask |= EDD_FEATURE_SUPPORT_ADD_FWD_RULES_MSK;
    }
    if (EDD_FEATURE_ENABLE == pDDB->FeatureSupport.ApplicationExist)
    {
      pRqbParam->FeatureMask |= EDD_FEATURE_SUPPORT_APPL_EXIST_MSK;
    }
    if (EDD_FEATURE_ENABLE == pDDB->FeatureSupport.MRPInterconnFwdRulesSupported)
    {
        pRqbParam->FeatureMask |= EDD_FEATURE_SUPPORT_MRP_INTERCONN_FWD_RULES;
    }

    if (pRqbParam->MaxPortCnt > EDD_CFG_MAX_PORT_CNT)
    {
        EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_GEN_Get_Params(): FATAL -> MaxPortCnt(0x%x) > EDD_CFG_MAX_PORT_CNT(0x%x)", 
                           pRqbParam->MaxPortCnt, 
                           EDD_CFG_MAX_PORT_CNT);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    /* -------------------------------------------- */
    /* parameters for cyclical communication        */
    /* -------------------------------------------- */
    pRqbParam->CycleBaseFactor                      = pDDB->SWI.CycleInfo.CycleBaseFactor;

    pRqbParam->ConsumerFrameIDBaseClass3            = EDDP_CRT_FRAMEID_RT_CLASS3_START;
    pRqbParam->ConsumerCntClass3                    = pDDB->CRT.CfgParam.ConsumerMaxNumber;
    pRqbParam->ConsumerFrameIDBaseClass2            = EDDP_CRT_FRAMEID_RT_CLASS2_START;
    pRqbParam->ConsumerCntClass2                    = pDDB->CRT.CfgParam.ConsumerMaxNumber;
    pRqbParam->ConsumerFrameIDBaseClass1            = EDDP_CRT_FRAMEID_RT_CLASS1_START;
    pRqbParam->ConsumerCntClass1                    = pDDB->CRT.CfgParam.ConsumerMaxNumber;
    pRqbParam->ConsumerCntClass12Combined           = EDD_CONSUMERCNT_CLASS12_COMBINED;

    pRqbParam->ProviderCnt                          = pDDB->CRT.CfgParam.ProviderMaxNumber;
    pRqbParam->ProviderCntClass3                    = pDDB->CRT.CfgParam.ProviderMaxNumber;
    pRqbParam->ProviderGroupCnt                     = pDDB->CRT.CfgParam.ProviderGroupMaxNumber;

    pRqbParam->DataLenMin                           = EDD_CSRT_DATALEN_MIN;
    pRqbParam->DataLenMax                           = EDD_CSRT_DATALEN_MAX;
    pRqbParam->DataLenMinUDP                        = EDD_CSRT_UDP_DATALEN_MIN;
    pRqbParam->DataLenMaxUDP                        = EDD_CSRT_UDP_DATALEN_MAX;

    /* -------------------------------------------- */
    /* other parameters                             */
    /* -------------------------------------------- */
    pRqbParam->KRAMInfo.KRAMBaseAddr                = 0;
    pRqbParam->KRAMInfo.offset_ProcessImageStart    = 0;
    pRqbParam->KRAMInfo.offset_async_in_start       = 0;
    pRqbParam->KRAMInfo.size_async_in               = 0;
    pRqbParam->KRAMInfo.offset_async_out_start      = 0;
    pRqbParam->KRAMInfo.size_async_out              = 0;
    pRqbParam->KRAMInfo.offset_ProcessImageEnd      = 0;

    /* -------------------------------------------- */
    /* parameters of RsvIntervalRed                 */
    /* -------------------------------------------- */
    EDDP_PRMPDIRDataSetupRsvIntervalRedValues (pDDB, &(pRqbParam->RsvIntervalRed), LSA_TRUE, LSA_TRUE);

    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_GEN_Get_Params()");
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GEN_Get_Port_Params                    +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters          (in) +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function fills all paramters requested by service    +*/
/*+               EDD_SRV_GET_PORT_PARAMS.                                  +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GEN_Get_Port_Params(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP                                 Status;
    LSA_UINT32                              PortID, HwPortID;
    EDD_UPPER_GET_PORT_PARAMS_PTR_TYPE      pParam;
    EDDP_DDB_SWI_LINK_STATUS_PTR_TYPE       pSwiLinkStatus;

    Status  = EDD_STS_OK;
    pParam  = (EDD_UPPER_GET_PORT_PARAMS_PTR_TYPE) pRQB->pParam;
    PortID  = pParam->PortID;

    if ((PortID < 1) || (PortID > pDDB->SWI.PortCnt))
    {
        Status = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Status);
        EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_GEN_Get_Port_Params(): ERROR -> RQB-PortID(%d) is out of range [1...%d]", 
            PortID, pDDB->SWI.PortCnt);
    }
    else
    {
        // convert PortID into Hardware PortID
        HwPortID = EDDP_SWI_get_HwPortID (pDDB, PortID);

        pSwiLinkStatus = &pDDB->SWI.LinkStatus[PortID];

        pParam->MediaType                               = pSwiLinkStatus->MediaType;
        pParam->OpticalTransType                        = pSwiLinkStatus->OpticalTransType;
        pParam->ShortPreamble                           = pSwiLinkStatus->ShortPreamble;
        pParam->TxFragmentation                         = pSwiLinkStatus->TxFragmentation;
        pParam->MACAddr                                 = pDDB->SWI.PortParams[HwPortID].Port_MAC_Address;
        pParam->PortStatus                              = pSwiLinkStatus->PortStatus;
        pParam->PhyStatus                               = pSwiLinkStatus->PhyStatus;
        pParam->IRTPortStatus                           = pSwiLinkStatus->IRTPortStatus;

        pParam->LineDelayInTicksMeasured                = pSwiLinkStatus->LineDelayInTicksMeasured;
        pParam->CableDelayInNsMeasured                  = pSwiLinkStatus->CableDelayInNsMeasured;
        pParam->RTClass2_PortStatus                     = pSwiLinkStatus->RTClass2_PortStatus;
        pParam->MRPRingPort                             = pSwiLinkStatus->MRPRingPort;
        pParam->SupportsMRPInterconnPortConfig          = pSwiLinkStatus->SupportsMRPInterconnPortConfig;
        pParam->LinkSpeedModeConfigured                 = pSwiLinkStatus->LinkSpeedModeConfigured;
        pParam->AutonegCapAdvertised                    = pSwiLinkStatus->AutonegCapAdvertised;

        pParam->RsvIntervalRed.Rx.BeginNs               = pSwiLinkStatus->RsvIntervalRed.Rx.BeginNs;
        pParam->RsvIntervalRed.Rx.EndNs                 = pSwiLinkStatus->RsvIntervalRed.Rx.EndNs;
        pParam->RsvIntervalRed.Rx.EndLocalNs            = pSwiLinkStatus->RsvIntervalRed.Rx.EndLocalNs;
        pParam->RsvIntervalRed.Rx.LastLocalFrameLen     = pSwiLinkStatus->RsvIntervalRed.Rx.LastLocalFrameLen;

        pParam->RsvIntervalRed.Tx.BeginNs               = pSwiLinkStatus->RsvIntervalRed.Tx.BeginNs;
        pParam->RsvIntervalRed.Tx.EndNs                 = pSwiLinkStatus->RsvIntervalRed.Tx.EndNs;
        pParam->RsvIntervalRed.Tx.EndLocalNs            = pSwiLinkStatus->RsvIntervalRed.Tx.EndLocalNs;
        pParam->RsvIntervalRed.Tx.LastLocalFrameLen     = pSwiLinkStatus->RsvIntervalRed.Tx.LastLocalFrameLen;

        pParam->RsvIntervalOrange.Rx                    = pSwiLinkStatus->RsvIntervalOrange.Rx;
        pParam->RsvIntervalOrange.Tx                    = pSwiLinkStatus->RsvIntervalOrange.Tx;

        pParam->IsWireless                              = pSwiLinkStatus->IsWireless;
        pParam->IsMDIX                                  = pSwiLinkStatus->IsMDIX;

        pParam->IsPulled                                = pSwiLinkStatus->IsPulled;
    }
    return (Status);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GEN_Add_Remove_Multicast_Address       +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters          (in) +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+             : EDD_STS_ERR_OPCODE                                        +*/
/*+             : EDD_STS_ERR_PARAM                                         +*/
/*+             : EDD_STS_ERR_RESOURCE                                      +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function adds/removes a specified multicast          +*/
/*+               address to/from the multicast table.                      +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GEN_Add_Remove_Multicast_Address(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP                         Status;
    EDD_UPPER_MULTICAST_PTR_TYPE    pParam;

    pParam = (EDD_UPPER_MULTICAST_PTR_TYPE) pRQB->pParam;

    if (pParam->Mode == EDD_MULTICAST_ENABLE)
    {
        Status = EDDP_GEN_Add_Multicast_Address (pDDB, pParam->MACAddr, &pParam->RefCnt);
    }
    else if (pParam->Mode == EDD_MULTICAST_DISABLE)
    {
        Status = EDDP_GEN_Remove_Multicast_Address (pDDB, pParam->MACAddr, &pParam->RefCnt);
    }
    else
    {
        Status = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Status);
        EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_GEN_Add_Remove_Multicast_Address(): ERROR -> RQB-Mode(0x%X) is neither EDD_MULTICAST_ENABLE nor EDD_MULTICAST_DISABLE!", 
            pParam->Mode);
    }

    return (Status);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GEN_Remove_Multicast_Address           +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             EDD_MAC_ADR_TYPE               MAC_Address  +*/
/*+                             LSA_UINT32*                    RefCnt       +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB         : Pointer to DeviceDescriptionBlock                 (in)  +*/
/*+  MAC_Address  : MAC address to be removed/disabled                (in)  +*/
/*+  RefCnt       : Reference counter for this MAC address            (out) +*/
/*+                                                                         +*/
/*+  Results      : EDD_STS_OK                                              +*/
/*+               : EDD_STS_ERR_PARAM                                       +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function removes a MC address from the MC table. 	+*/
/*+				  For each MC address entry there is a counter (RefCnt)     +*/
/*+               which stores how many users do currently need this MC     +*/
/*+				  address. When the couter reaches '0' the MC is then       +*/
/*+				  actually deleted from the MC table. Otherwise the counter +*/
/*+				  (RefCnt) is only decremented. If the MC address is a      +*/
/*+				  static one, it is not deleted but the default forwarding  +*/
/*+               settings are restored.                                    +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GEN_Remove_Multicast_Address(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_MAC_ADR_TYPE            MAC_Address,
    LSA_UINT32*                 RefCnt)
{
    EDD_RSP         Status;
    LSA_UINT32      i;
    LSA_UINT32      MAC_upper;
    LSA_UINT16      MAC_lower;
    LSA_BOOL        MC_entry_found;

    Status          = EDD_STS_OK;
    MC_entry_found  = LSA_FALSE;

    EDDP_CORE_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_GEN_Remove_Multicast_Address(): MAC(%2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X)", 
        MAC_Address.MacAdr[0], 
        MAC_Address.MacAdr[1], 
        MAC_Address.MacAdr[2], 
        MAC_Address.MacAdr[3], 
        MAC_Address.MacAdr[4], 
        MAC_Address.MacAdr[5]);

    MAC_lower = ((LSA_UINT32)MAC_Address.MacAdr[5])       | 
                ((LSA_UINT32)MAC_Address.MacAdr[4]) << 8  ;

    MAC_upper = ((LSA_UINT32)MAC_Address.MacAdr[3])       | 
                ((LSA_UINT32)MAC_Address.MacAdr[2]) << 8  | 
                ((LSA_UINT32)MAC_Address.MacAdr[1]) << 16 | 
                ((LSA_UINT32)MAC_Address.MacAdr[0]) << 24 ;

    for ( i = 0; i < pDDB->SWI.MC_Table_Shadow_Entries_Count; i++ )
    {
        if (   (MAC_upper == pDDB->SWI.pMC_Table_Shadow[i].MAC)
            && (MAC_lower == pDDB->SWI.pMC_Table_Shadow[i].HighMAC)
            && (MAC_lower == pDDB->SWI.pMC_Table_Shadow[i].LowMAC)
            && (pDDB->SWI.pMC_Table_Shadow[i].RefCnt != 0)
           )
        {
            // MC entry found
            if (pDDB->SWI.pMC_Table_Shadow[i].RefCnt == 1)
            { 
                if (!(pDDB->SWI.pMC_Table_Shadow[i].Static))
                {
                    // remove MC entry from MC table
                    Status = EDDP_GEN_Move_MC_Table_Up_Down (pDDB, i, LSA_TRUE);
                    // copy modified MC table to PNIP; new entry is now active
                    _EDDP_SWI_switch_MC_table (pDDB);
                    // return 'RefCnt'
                    *RefCnt = 0;
                }
                else
                {
                    // this is a static MC entry and must not be deleted
                    // set 'RefCnt' for static entry to 0 (must be done before EDDP_SWI_MC_Entry_Set_To_Default(..))
                    pDDB->SWI.pMC_Table_Shadow[i].RefCnt = 0;
                    // restore default parameters for this static MC entry 
                    //EDDP_SWI_MC_Entry_Set_To_Default (pDDB, i);

                    // disable receiving on local API (change forwarding parameters in shadow MC table)
                    pDDB->SWI.pMC_Table_Shadow[i].TxPort_1 &= ~1;
                    pDDB->SWI.pMC_Table_Shadow[i].TxPort_2 &= ~1;
                    pDDB->SWI.pMC_Table_Shadow[i].TxPort_3 &= ~1;
                    pDDB->SWI.pMC_Table_Shadow[i].TxPort_4 &= ~1;

                    // copy modified MC table to PNIP; new entry is now active
                    _EDDP_SWI_switch_MC_table (pDDB);
                    // return 'RefCnt'
                    *RefCnt = 0;
                }
            }
            else
            {
                // RefCnt > 1   : decrement 'RefCnt'
                pDDB->SWI.pMC_Table_Shadow[i].RefCnt--;
                // return 'RefCnt'
                *RefCnt = pDDB->SWI.pMC_Table_Shadow[i].RefCnt;
            }

            MC_entry_found = LSA_TRUE;
            break;  // exit for loop
        }
    }

    if (!(MC_entry_found))
    {
        Status = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Status);
        EDDP_CORE_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_GEN_Remove_Multicast_Address(): ERROR -> MC entry not found: MAC(%2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X)", 
            MAC_Address.MacAdr[0], 
            MAC_Address.MacAdr[1], 
            MAC_Address.MacAdr[2], 
            MAC_Address.MacAdr[3], 
            MAC_Address.MacAdr[4], 
            MAC_Address.MacAdr[5]);
    }

    return (Status);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GEN_Add_Multicast_Address              +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDD_MAC_ADR_TYPE            MAC_Address     +*/
/*+                             LSA_UINT32*                 RefCnt          +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB         : Pointer to DeviceDescriptionBlock                 (in)  +*/
/*+  MAC_Address  : MAC address to be removed/disabled                (in)  +*/
/*+  RefCnt       : Reference counter for this MAC address            (out) +*/
/*+                                                                         +*/
/*+  Results      : EDD_STS_OK                                              +*/
/*+               : EDD_STS_ERR_PARAM                                       +*/
/*+               : EDD_STS_ERR_RESOURCE                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function adds a MC address to the MC table. 	        +*/
/*+				  For each MC address entry there is a counter (RefCnt)     +*/
/*+               which stores how many users do currently need this MC     +*/
/*+				  address. If the MC address is a static one, receiving on  +*/
/*+				  local API is enabled. If the MC address is not already    +*/
/*+				  listed in the MC table, all forwarding settings are taken +*/
/*+				  from the broadcast entry which is always at the end of the+*/
/*+               MC table. However, in both cases RefCnt is incremented.   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GEN_Add_Multicast_Address(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_MAC_ADR_TYPE            MAC_Address,
    LSA_UINT32*                 RefCnt)
{
    EDD_RSP         Status;
    LSA_UINT32      i;
    LSA_UINT32      MAC_upper;
    LSA_UINT16      MAC_lower;

    Status = EDD_STS_OK;

    EDDP_CORE_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_GEN_Add_Multicast_Address(): MAC(%2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X)", 
        MAC_Address.MacAdr[0], 
        MAC_Address.MacAdr[1], 
        MAC_Address.MacAdr[2], 
        MAC_Address.MacAdr[3], 
        MAC_Address.MacAdr[4], 
        MAC_Address.MacAdr[5]);

    if (MAC_Address.MacAdr[0] & 0x01)   // is it a multicast address?
    {
        MAC_lower = ((LSA_UINT32)MAC_Address.MacAdr[5])       | 
                    ((LSA_UINT32)MAC_Address.MacAdr[4]) << 8  ;

        MAC_upper = ((LSA_UINT32)MAC_Address.MacAdr[3])       | 
                    ((LSA_UINT32)MAC_Address.MacAdr[2]) << 8  | 
                    ((LSA_UINT32)MAC_Address.MacAdr[1]) << 16 | 
                    ((LSA_UINT32)MAC_Address.MacAdr[0]) << 24 ;

        for ( i = 0; i < pDDB->SWI.MC_Table_Shadow_Entries_Count; i++ )
        {
            if (MAC_upper == pDDB->SWI.pMC_Table_Shadow[i].MAC)
            {
                if ((MAC_lower == pDDB->SWI.pMC_Table_Shadow[i].HighMAC) && (MAC_lower == pDDB->SWI.pMC_Table_Shadow[i].LowMAC))
                {
                    // enable receiving on local API (change forwarding parameters in shadow MC table)
                    pDDB->SWI.pMC_Table_Shadow[i].TxPort_1 |= 0x001;
                    pDDB->SWI.pMC_Table_Shadow[i].TxPort_2 |= 0x001;
                    pDDB->SWI.pMC_Table_Shadow[i].TxPort_3 |= 0x001;
                    pDDB->SWI.pMC_Table_Shadow[i].TxPort_4 |= 0x001;

                    // enable receiving on local API (change forwarding parameters in MC table on PNIP)
                    EDDP_SWI_Change_MC_Entry_Parameters(pDDB, &pDDB->SWI.pMC_Table_Shadow[i], i); 
                    // increment RefCnt
                    pDDB->SWI.pMC_Table_Shadow[i].RefCnt++;
                    // return RefCnt
                    *RefCnt = pDDB->SWI.pMC_Table_Shadow[i].RefCnt;
                    break;  // exit for loop
                }
                else if ((MAC_lower <= pDDB->SWI.pMC_Table_Shadow[i].HighMAC) && (MAC_lower >= pDDB->SWI.pMC_Table_Shadow[i].LowMAC))
                {
                    // MAC address is between an existing MAC address band -> not supported
                    Status = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Status);
                    EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_GEN_Add_Multicast_Address(): ERROR -> Splitting of static MC address intervals is not supported!");
                    break; // exit for loop
                }
                else if ( MAC_lower < pDDB->SWI.pMC_Table_Shadow[i].LowMAC )
                {
                    // move all following addresses downwards 
                    Status = EDDP_GEN_Move_MC_Table_Up_Down (pDDB, i, LSA_FALSE);

                    if (Status == EDD_STS_OK)
                    {
                        // copy MC properties from broadcast entry (which is always the last one)
                        pDDB->SWI.pMC_Table_Shadow[i] = pDDB->SWI.pMC_Table_Shadow[pDDB->SWI.MC_Table_Shadow_Entries_Count - 1];

                        // add MC address on current index
                        pDDB->SWI.pMC_Table_Shadow[i].MAC       = MAC_upper;
                        pDDB->SWI.pMC_Table_Shadow[i].HighMAC   = MAC_lower;
                        pDDB->SWI.pMC_Table_Shadow[i].LowMAC    = MAC_lower;
                        // change 'Static' flag to 'non static' -> can be removed
                        pDDB->SWI.pMC_Table_Shadow[i].Static    = LSA_FALSE;
                        // set 'RefCnt' to 1 because it's the first time this MAC address was added
                        pDDB->SWI.pMC_Table_Shadow[i].RefCnt    = 1;
                        // return RefCnt
                        *RefCnt = pDDB->SWI.pMC_Table_Shadow[i].RefCnt;
                        // copy modified MC table to PNIP; new entry is now active
                        _EDDP_SWI_switch_MC_table (pDDB);
                    }
                    break;  // exit for loop
                }
                else
                {
                    continue;   // continue for loop
                }
            }
            else if (MAC_upper > pDDB->SWI.pMC_Table_Shadow[i].MAC)
            {
                continue;   // continue for loop
            }
            else
            {
                // move all following addresses downwards 
                Status = EDDP_GEN_Move_MC_Table_Up_Down (pDDB, i, LSA_FALSE);

                if (Status == EDD_STS_OK)
                {
                    // copy MC properties from broadcast entry (which is always the last one)
                    pDDB->SWI.pMC_Table_Shadow[i] = pDDB->SWI.pMC_Table_Shadow[ pDDB->SWI.MC_Table_Shadow_Entries_Count - 1 ];
                    // add MC address on current index
                    pDDB->SWI.pMC_Table_Shadow[i].MAC     = MAC_upper;
                    pDDB->SWI.pMC_Table_Shadow[i].HighMAC = MAC_lower;
                    pDDB->SWI.pMC_Table_Shadow[i].LowMAC  = MAC_lower;
                    // change 'Static' flag to 'non static' -> can be removed
                    pDDB->SWI.pMC_Table_Shadow[i].Static  = LSA_FALSE;
                    // set 'RefCnt' to 1 because it's the first time this MAC address was added
                    pDDB->SWI.pMC_Table_Shadow[i].RefCnt  = 1;
                    // return RefCnt
                    *RefCnt = pDDB->SWI.pMC_Table_Shadow[i].RefCnt;
                    // copy modified MC table to PNIP; new entry is now active
                    _EDDP_SWI_switch_MC_table(pDDB);
                }
                break;  // exit for loop
            }
        }
    }
    else
    {
        // this is no multicast address
        Status = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Status);
        EDDP_CORE_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_GEN_Add_Multicast_Address(): ERROR -> This is no multicast address: MAC(%2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X)", 
            MAC_Address.MacAdr[0], 
            MAC_Address.MacAdr[1], 
            MAC_Address.MacAdr[2], 
            MAC_Address.MacAdr[3], 
            MAC_Address.MacAdr[4], 
            MAC_Address.MacAdr[5]);
    }

    return (Status);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GEN_Move_MC_Table_Up_Down              +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_UINT32                  Index           +*/
/*+                             LSA_BOOL                    MoveUp          +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB         : Pointer to DeviceDescriptionBlock                 (in)  +*/
/*+  Index        : Index in shadow MC table                          (in)  +*/
/*+  MoveUp       : LSA_TRUE = delete this entry                      (in)  +*/
/*+               : LSA_FALSE = make room for a new entry                   +*/
/*+                                                                         +*/
/*+  Results      : EDD_STS_OK                                              +*/
/*+               : EDD_STS_ERR_RESOURCE                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function either deletes a MC entry or makes room for +*/
/*+               a new one depending on the input paramter 'MoveUp'.       +*/
/*+				  When deleting a entry all MC entries below the selected   +*/
/*+				  index are shifted one entry up in the shadow MC table.    +*/
/*+               When making room for a new entry all MC entries are       +*/
/*+				  shifted one entry down (below the selected index and      +*/
/*+               including this index).                                    +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GEN_Move_MC_Table_Up_Down(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  Index,
    LSA_BOOL                    MoveUp)
{
    EDD_RSP         Status;
    LSA_UINT32      i;

    Status = EDD_STS_OK;

    EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDD_GEN_Move_MC_Table_Up_Down(Index=%d, MoveUp=0x%X)", 
        Index, MoveUp);

    if (MoveUp)
    {
        if (Index != pDDB->SWI.MC_Table_Shadow_Entries_Count - 1)
        {
            for (i = Index; i < pDDB->SWI.MC_Table_Shadow_Entries_Count; i++)
            {
                pDDB->SWI.pMC_Table_Shadow[i] = pDDB->SWI.pMC_Table_Shadow[i + 1];
            }
        }
        pDDB->SWI.MC_Table_Shadow_Entries_Count--;
    }
    else
    {
        if (pDDB->SWI.MC_Table_Shadow_Entries_Count == pDDB->SWI.MC_Table_Max_Entries)
        {
            // MC table is full
            Status = EDD_STS_ERR_RESOURCE;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Status);
            EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDD_GEN_Move_MC_Table_Up_Down(Index=%d, MoveUp=0x%X): ERROR -> No more space in MC table!", 
                Index, MoveUp);
        }
        else
        {
            for ( i = pDDB->SWI.MC_Table_Shadow_Entries_Count; i > Index; i-- )
            {
                pDDB->SWI.pMC_Table_Shadow[i] = pDDB->SWI.pMC_Table_Shadow[i-1]; 
            }
            pDDB->SWI.MC_Table_Shadow_Entries_Count++;
        }
    }

    return (Status);
}





/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                            +*/
/*+  Functionname          :    EDDP_GEN_Multicast_Fwd_Ctrl                    +*/
/*+  Input/Output          :    EDD_UPPER_MULTICAST_FWD_CTRL_PTR_TYPE   pParam +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE                 pDDB   +*/
/*+  Result                :    EDD_RSP                                        +*/
/*+                                                                            +*/
/*+----------------------------------------------------------------------------+*/
/*+                                                                            +*/
/*+  Input/Output:                                                             +*/
/*+                                                                            +*/
/*+  pParam : Pointer to Upper-RQB with Requestparameters          (in)        +*/
/*+  pDDB   : Pointer to DeviceDescriptionBlock                    (in)        +*/
/*+                                                                            +*/
/*+  Results    : EDD_STS_OK                                                   +*/
/*+             : EDD_STS_ERR_OPCODE                                           +*/
/*+             : EDD_STS_ERR_PARAM                                            +*/
/*+             : EDD_STS_ERR_RESOURCE                                         +*/
/*+----------------------------------------------------------------------------+*/
/*+  Description: This function controls the forwarding rules for some         +*/
/*+               special multicast addresses.                                 +*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GEN_Multicast_Fwd_Ctrl(
    EDD_UPPER_MULTICAST_FWD_CTRL_PTR_TYPE   pParam,
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB)
{
    EDD_RSP         Status;
    LSA_UINT32      i,k,m;
    LSA_UINT8       SpecialHandling;
    LSA_BOOL        bFound;
    LSA_BOOL        bForward;
             
    bFound = LSA_FALSE;
    Status = EDD_STS_OK;

    if (pParam->PortIDModeCnt <= pDDB->SWI.PortCnt) // check number of ports which have to be changed
    {
        if (pParam->MACAddrGroup == EDD_MAC_ADDR_GROUP_RT_2_QUER)
        {
            for ( i=0; i < pParam->PortIDModeCnt; i++ )
            {
                // check if the mode for all ports is EDD_MULTICAST_FWD_BITMASK
                if (pParam->pPortIDModeArray[i].Mode != EDD_MULTICAST_FWD_BITMASK)
                {
                    Status = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Status);
                    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_GEN_Multicast_Fwd_Ctrl(): ERROR -> Mode(%d) is not EDD_MULTICAST_FWD_BITMASK (EDD_MAC_ADDR_GROUP_RT_2_QUER is selected)!", 
                        pParam->pPortIDModeArray[i].Mode);
                }
            }

            if (Status == EDD_STS_OK)
            {
                // search for the corresponding MC entry
                for ( k=0; k < pDDB->SWI.MC_Table_Shadow_Entries_Count; k++ )
                {
                    if (pDDB->SWI.pMC_Table_Shadow[k].SpecialHandling == EDDP_SWI_FDB_GROUP_RT_2_QUER)
                    {
                        bFound = LSA_TRUE;
                        break; // exit for loop
                    }
                }

                if (bFound)
                {
                    for (i=0; i < EDDP_GEN_QEUER_ENTRIES_COUNT; i++)
                    {
                        // determine if forwarding for current MC entry has to be enabled or disabled
                        bForward = ((pParam->MACAddrLow >> i) & 0x1) ? LSA_TRUE : LSA_FALSE;
                        
                        if (pDDB->SWI.pMC_Table_Shadow[k+i].SpecialHandling == EDDP_SWI_FDB_GROUP_RT_2_QUER)
                        {
                            for (m=0; m < pParam->PortIDModeCnt; m++)
                            {
                              Status = EDDP_SWI_Change_Forwarding (pDDB, k + i, pParam->pPortIDModeArray[m], bForward);
                              if (Status != EDD_STS_OK) { break; }  // exit loop on error
                            }
                            if (Status != EDD_STS_OK) {break;}  // exit loop on error
                        }
                        else
                        {
                            EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_GEN_Multicast_Fwd_Ctrl(): FATAL -> SpecialHandling(%d) is not EDDP_SWI_FDB_GROUP_RT_2_QUER!", 
                                pDDB->SWI.pMC_Table_Shadow[k+i].SpecialHandling);
                            EDDP_FATAL(EDDP_FATAL_ERR_SERVICE);
                        }
                    }
                }
                else
                {
                    // MC entry not found
                    EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_GEN_Multicast_Fwd_Ctrl(): FATAL -> Could not find corresponding MC Entry!");
                    EDDP_FATAL(EDDP_FATAL_ERR_SERVICE);
                }
            }
        }
        else
        {
            SpecialHandling = EDDP_SWI_FDB_NO_SPECIAL_HANDLING;

            if (   ((pParam->MACAddrGroup == EDD_MAC_ADDR_GROUP_MRP_1) || (pParam->MACAddrGroup == EDD_MAC_ADDR_GROUP_MRP_2))
                 && (EDD_FEATURE_DISABLE  == pDDB->FeatureSupport.DLRExtensions))
            {
                // select either MRP 1 or MRP 2 
                SpecialHandling = (pParam->MACAddrGroup == EDD_MAC_ADDR_GROUP_MRP_1) ? EDDP_SWI_FDB_GROUP_MRP_1 : EDDP_SWI_FDB_GROUP_MRP_2;
            }
            /* MRP interconnect can only be selected if supported - otherwise there are no mc mac table entry */
            else if (   ((pParam->MACAddrGroup == EDD_MAC_ADDR_GROUP_MRP_IN_1) || (pParam->MACAddrGroup == EDD_MAC_ADDR_GROUP_MRP_IN_2) || (pParam->MACAddrGroup == EDD_MAC_ADDR_GROUP_MRP_IN_3))
                     && (EDD_FEATURE_ENABLE    == pDDB->FeatureSupport.MRPInterconnFwdRulesSupported)
                     && (EDD_FEATURE_DISABLE   == pDDB->FeatureSupport.DLRExtensions))
            {
                /* set special handling for MRP IC */
                if (EDD_MAC_ADDR_GROUP_MRP_IN_3 == pParam->MACAddrGroup)
                {
                    SpecialHandling = EDDP_SWI_FDB_GROUP_MRP_IC_3;
                }
                else if (EDD_MAC_ADDR_GROUP_MRP_IN_2 == pParam->MACAddrGroup)
                {
                    SpecialHandling = EDDP_SWI_FDB_GROUP_MRP_IC_2;
                }
                else
                {
                    SpecialHandling = EDDP_SWI_FDB_GROUP_MRP_IC_1;
                }
            }
            else if (   ((pParam->MACAddrGroup == EDD_MAC_ADDR_GROUP_DLR_1) || (pParam->MACAddrGroup == EDD_MAC_ADDR_GROUP_DLR_2) || (pParam->MACAddrGroup == EDD_MAC_ADDR_GROUP_DLR_3))
                     && (EDD_FEATURE_ENABLE    == pDDB->FeatureSupport.DLRExtensions) )
            {
                /* set special handling for DLR */
                if (EDD_MAC_ADDR_GROUP_DLR_1 == pParam->MACAddrGroup)
                {
                    SpecialHandling = EDDP_SWI_FDB_GROUP_DLR_1;
                }
                else if (EDD_MAC_ADDR_GROUP_DLR_2 == pParam->MACAddrGroup)
                {
                    SpecialHandling = EDDP_SWI_FDB_GROUP_DLR_2;
                }
                else
                {
                    SpecialHandling = EDDP_SWI_FDB_GROUP_DLR_3;
                }
            }
            /* HSYNC can only be selected if supported - otherwise there is no mc mac table entry */
            else if (     EDD_MAC_ADDR_GROUP_HSYNC == pParam->MACAddrGroup
                     && ((EDD_FEATURE_ENABLE       == pDDB->FeatureSupport.AdditionalForwardingRulesSupported) || (EDD_FEATURE_ENABLE == pDDB->FeatureSupport.ApplicationExist)))
            {
                /* set special handling for hsync */
                SpecialHandling = EDDP_SWI_FDB_GROUP_HSYNC;
            }                

            if (EDDP_SWI_FDB_NO_SPECIAL_HANDLING != SpecialHandling)
            {
                // search for the corresponding MC entry
                for ( k = 0; k < pDDB->SWI.MC_Table_Shadow_Entries_Count; k++ )
                {
                    if (pDDB->SWI.pMC_Table_Shadow[k].SpecialHandling == SpecialHandling)
                    {
                        bFound = LSA_TRUE;
                        break; // exit for loop
                    }
                }

                if (bFound)
                {
                    for ( i = 0; i < pParam->PortIDModeCnt; i++ )
                    {
                        if (   (pParam->pPortIDModeArray[i].Mode == EDD_MULTICAST_FWD_ENABLE)
                            || (pParam->pPortIDModeArray[i].Mode == EDD_MULTICAST_FWD_DISABLE)
                           )
                        {
                            Status = EDDP_SWI_Change_Forwarding (pDDB, k, pParam->pPortIDModeArray[i], (pParam->pPortIDModeArray[i].Mode == EDD_MULTICAST_FWD_ENABLE) ? LSA_TRUE : LSA_FALSE);
                            if (Status != EDD_STS_OK)
                            {
                                break;   // exit loop on error
                            }
                        }
                        else
                        {
                            Status = EDD_STS_ERR_PARAM;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Status);
                            EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_GEN_Multicast_Fwd_Ctrl(): ERROR -> Mode(0x%X) is neither EDD_MULTICAST_FWD_ENABLE nor EDD_MULTICAST_FWD_DISABLE!", 
                                pParam->pPortIDModeArray[i].Mode);
                        }
                    }
                }
                else
                {
                    // MC entry not found
                    EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_GEN_Multicast_Fwd_Ctrl(): FATAL -> Could not find corresponding MC Entry!");
                    EDDP_FATAL(EDDP_FATAL_ERR_SERVICE);
                }
            }
            else
            {
                Status = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Status);
                EDDP_CORE_TRACE_01 (pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_GEN_Multicast_Fwd_Ctrl(): ERROR -> MACAddrGroup(%d) not supported!",
                                    pParam->MACAddrGroup);
            }
        }
    }
    else
    {
        Status = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Status);
        EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_GEN_Multicast_Fwd_Ctrl(): ERROR -> pParam->PortIDModeCnt(%d) > pDDB->SWI.PortCnt(%d)!", 
            pParam->PortIDModeCnt, pDDB->SWI.PortCnt);
    }

    return (Status);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GENGetLinkStatus                       +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters          (in) +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function returns the current link status of a given  +*/
/*+               Port ID. If Port ID = 0 the parameters of the currently   +*/
/*+               fastest port is returned.                                 +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GENGetLinkStatus(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP                                 Status;
    EDD_UPPER_GET_LINK_STATUS_PTR_TYPE      pParam;

    pParam  = (EDD_UPPER_GET_LINK_STATUS_PTR_TYPE) pRQB->pParam;
    Status  = EDD_STS_OK;

    if (pParam->PortID == EDD_PORT_ID_AUTO)
    {
        pParam->Status  = pDDB->SWI.LinkStatus[0].LinkStatus;
        pParam->Mode    = pDDB->SWI.LinkStatus[0].LinkDuplexity;
        pParam->Speed   = pDDB->SWI.LinkStatus[0].LinkSpeed;
        pParam->AddCheckEnabled = EDD_LINK_ADD_CHECK_DISABLED;
        pParam->ChangeReason    = EDD_LINK_CHANGE_REASON_NONE;
    }
    else if (pParam->PortID <= pDDB->SWI.PortCnt)
    {
        pParam->Status  = pDDB->SWI.LinkStatus[pParam->PortID].LinkStatus;
        pParam->Mode    = pDDB->SWI.LinkStatus[pParam->PortID].LinkDuplexity;
        pParam->Speed   = pDDB->SWI.LinkStatus[pParam->PortID].LinkSpeed;    
        pParam->AddCheckEnabled = EDD_LINK_ADD_CHECK_DISABLED;
        pParam->ChangeReason    = EDD_LINK_CHANGE_REASON_NONE;
    }
    else
    {
        Status = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Status);
        EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_GENGetLinkStatus(): ERROR -> pParam->PortID(%d) > pDDB->SWI.PortCnt(%d)!", 
            pParam->PortID, pDDB->SWI.PortCnt);
    }

    return (Status);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GENOpenChannel                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB           +*/
/*+                        :    EDDP_LOCAL_HDB_PTR_TYPE      pHDB           +*/
/*+                             LSA_HANDLE_TYPE              Handle         +*/
/*+                             EDDP_SYSTEM_CDB_PTR_TYPE     pCDB           +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                    (in) +*/
/*+  Handle     : Handle for this Channel (<  EDD_CFG_MAX_CHANNELS)    (in) +*/
/*+  pCDB       : Pointer to ChannelDescriptionBlock                   (in) +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_CHANNEL_USE                                   +*/
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Opens Channel for GEN. The Handle will specify the Channel+*/
/*+               and pDDB the Device. pCDB has all parameters              +*/
/*+               for this channel                                          +*/
/*+                                                                         +*/
/*+       Note:   EDD_STS_OK_PENDING not allowed as response. This call     +*/
/*+               must be synchronously!                                    +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GENOpenChannel(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB,
    LSA_HANDLE_TYPE             Handle,
    EDDP_SYSTEM_CDB_PTR_TYPE    pCDB)
{
    EDDP_LOCAL_HDB_GEN_PTR_TYPE     pHDB_GEN_PoolItem;
    LSA_UINT32                      PortID;

    LSA_UNUSED_ARG(pCDB);

    /* ----------------------------------------------------------------------*/
    /* If HandleCnt too high return with error                               */
    /* ----------------------------------------------------------------------*/
    if (Handle >= EDDP_CFG_MAX_CHANNELS)
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_GENOpenChannel(): ERROR -> Handle(%d) >= EDDP_CFG_MAX_CHANNELS(%d)!", 
            Handle, EDDP_CFG_MAX_CHANNELS);
        return (EDD_STS_ERR_PARAM);
    }

    #ifdef EDDP_CFG_LOCAL_MEM_STATIC
    pHDB_GEN_PoolItem = &g_EDDP_HDB_GEN[Handle];                            // use static RAM
    #else
    pHDB_GEN_PoolItem = EDDP_GlbAllocLocalMem(sizeof(EDDP_HDB_GEN_TYPE));   // alloc Mgmt-data in Heap
    if (LSA_HOST_PTR_ARE_EQUAL(pHDB_GEN_PoolItem, LSA_NULL))
    {
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_GENOpenChannel(): ERROR -> error alloc pHDB_GEN_PoolItem!");
        EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_RESOURCE);
        return(EDD_STS_ERR_RESOURCE);
    }
    #endif
    pHDB->pGEN          = pHDB_GEN_PoolItem;    // assign GEN managment for this channel
    pHDB->pGEN->Handle  = Handle;               // index open channel

    for ( PortID=0; PortID<=pDDB->SWI.PortCnt; PortID++ )
    {
        // ***** 0: Iface, 1: Port1, 2: Port2 ...
        // init port/if statistics to 0 for this channel
        EDDP_MEMSET_COMMON(&pHDB_GEN_PoolItem->Statistic[PortID], 0, sizeof(EDDP_GET_STATISTICS_TYPE)); 
    }

    return (EDD_STS_OK);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_AllocateShadowStatisticMem            +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This a helpfer function to allocate memory for statistic  +*/
/*+               counters. This memory is needed to implement the reset    +*/
/*+               functionality.                                            +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static EDD_RSP _EDDP_AllocateShadowStatisticMem(
        EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP  Status                        = EDD_STS_OK;
    //statistics for number of ports and for the interface
    LSA_UINT32 const SizeOfShadowStatistic = sizeof(EDDP_GET_STATISTICS_TYPE)*(pDDB->SWI.PortCnt+1);
    
    EDDP_LOCAL_GET_STATISTICS_PTR_TYPE   pShadowStatistic = EDDP_GlbAllocLocalMem(SizeOfShadowStatistic);
    
    if (LSA_HOST_PTR_ARE_EQUAL(pShadowStatistic, LSA_NULL))
    {
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] _EDDP_AllocateShadowStatisticMem(): ERROR -> error alloc pShadowStatistic!");
        EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_RESOURCE);
        Status = EDD_STS_ERR_RESOURCE;
    }
    else
    {
        pDDB->Core.pShadowStatistic = pShadowStatistic;
        EDDP_MEMSET_COMMON(pShadowStatistic, 0, SizeOfShadowStatistic);
    }
    
    return Status;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GENDeviceOpen                          +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_UPPER_DPB_PTR_TYPE     pDPB            +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  pDPB       : Pointer to DeviceParameterBlock                     (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDD_SRV_DEV_OPEN                                          +*/
/*+               Initializes all GEN management structures within DDB      +*/
/*+                                                                         +*/
/*+       Note:   EDD_STS_OK_PENDING not allowed as response. This call     +*/
/*+               must be synchronously!                                    +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GENDeviceOpen(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DPB_PTR_TYPE     pDPB)
{
    EDD_RSP         Status;
    
    LSA_UNUSED_ARG(pDPB);
    Status = EDD_STS_OK;

    /* Alloc one timer for service EDD_SRV_LED_BLINK */      
    Status = EDDP_CoreTimerAlloc (pDDB, LSA_TIMER_TYPE_CYCLIC, LSA_TIME_BASE_100MS, EDDP_TIMER_BLINK, LSA_NULL, &pDDB->SWI.LEDBlink.Timer);

    pDDB->Core.pShadowStatistic = LSA_NULL;
    
    if( pDDB->FeatureSupport.EIPExtensions == EDD_FEATURE_ENABLE )
    {
       if( EDDP_HALGetPnipRevision(pDDB) == EDDP_HAL_HW_PNIP_REV2 )//only for ERTEC200P-Rev2 is EIP supported
       {
            Status = _EDDP_AllocateShadowStatisticMem(pDDB);
       }
       else
       {
            Status = EDD_STS_ERR_PARAM;
       }
    }
    return (Status);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GENDeviceSetup                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             EDDP_UPPER_DSB_PTR_TYPE        pDSB         +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  pDSB       : Pointer to DeviceSetupBlock                         (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_OK_PENDING                                        +*/
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDDP_SRV_DEV_SETUP                                        +*/
/*+               Setup Device for GEN                                      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GENDeviceSetup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DSB_PTR_TYPE     pDSB)
{
    LSA_UNUSED_ARG(pDDB);
    LSA_UNUSED_ARG(pDSB);

    return (EDD_STS_OK);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GENDeviceIFSetup                       +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_UPPER_DSB_PTR_TYPE     pDSB            +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  pDSB       : Pointer to DeviceSetupBlock                         (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_OK_PENDING                                        +*/
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDDP_SRV_DEV_IF_SETUP                                     +*/
/*+               Setup Interface of GEN                                    +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GENDeviceIFSetup( 
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DSB_PTR_TYPE     pDSB)
{
    LSA_UNUSED_ARG(pDDB);
    LSA_UNUSED_ARG(pDSB);

    return (EDD_STS_OK);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GENDeviceIFShutdown                    +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE           pDDB      +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  pDSB       : Pointer to DeviceSetupBlock                         (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_OK_PENDING                                        +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDDP_SRV_DEV_IF_SHUTDOWN or EDDP_SRV_DEV_IF_SETUP (only if+*/
/*+               previous setup shall be aborted)                          +*/
/*+               Shutdown Interface of GEN                                 +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GENDeviceIFShutdown(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    /* Stop cyclic timer for service EDD_SRV_LED_BLINK */
    EDDP_CoreTimerStop (pDDB->SWI.LEDBlink.Timer);
    
    return (EDD_STS_OK);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GENDeviceShutdown                      +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE           pDDB      +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_OK_PENDING                                        +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDDP_SRV_DEV_SHUTDOWN or EDDP_SRV_DEV_SETUP (only if      +*/
/*+               previous setup shall be aborted)                          +*/
/*+               Shutdown Interface of GEN                                 +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GENDeviceShutdown(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UNUSED_ARG(pDDB);

    return (EDD_STS_OK);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GENDeviceClose                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE           pDDB      +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDDP_SRV_DEV_CLOSE or EDDP_SRV_DEV_OPEN (only if          +*/
/*+               previous OPEN shall be aborted)                           +*/
/*+               Close device for GEN                                      +*/
/*+                                                                         +*/
/*+       Note:   EDD_STS_OK_PENDING not allowed as response. This call     +*/
/*+               must be synchronous!                                      +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GENDeviceClose(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP         Status = EDD_STS_OK;
 
    /* Free all timers for service EDD_SRV_LED_BLINK */
    Status = EDDP_CoreTimerFree (pDDB->SWI.LEDBlink.Timer);

    if(    (pDDB->FeatureSupport.EIPExtensions == EDD_FEATURE_ENABLE   )
        && (EDDP_HALGetPnipRevision(pDDB)      == EDDP_HAL_HW_PNIP_REV2)
      )
    {
        EDDP_ASSERT(pDDB->Core.pShadowStatistic != LSA_NULL);
        EDDP_GlbFreeLocalMem(pDDB->Core.pShadowStatistic);
    }

    return (Status);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GENCloseChannel                        +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB           +*/
/*+                        :    EDDP_LOCAL_HDB_PTR_TYPE      pHDB           +*/
/*+                             LSA_HANDLE_TYPE              Handle         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                    (in) +*/
/*+  Handle     : Handle for this Channel (<  EDD_CFG_MAX_CHANNELS)    (in) +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Close Channel for GEN.                                    +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GENCloseChannel(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB,
    LSA_HANDLE_TYPE             Handle)
{
    EDD_RSP                 Status;
    LSA_UINT32              PortID;
    EDD_UPPER_RQB_PTR_TYPE  pRQB;

    Status = EDD_STS_OK;
    PortID = 0;

    LSA_UNUSED_ARG(pDDB);
    LSA_UNUSED_ARG(Handle);

    // We loop through all PortIDs and look if there are LinkIndication RQBs which 
    // have to be returned to the user on CloseChannel. These RQBs are returned with EDD_STS_OK_CANCEL.
    for ( PortID = 0; PortID <= EDDP_MAX_PORT_CNT; PortID++ )
    {
        while (pHDB->LinkIndReq[PortID].Count != 0)
        {
            pRQB = EDDP_GlbRQBQueueRemoveFromBegin( &(pHDB->LinkIndReq[PortID]) );
            EDDP_GlbRequestFinish(pHDB, pRQB, EDD_STS_OK_CANCEL);
        }
    }

    /* ----------------------------------------------------------------------*/
    /* If handle used GEN --> free alloced RAM                               */
    /* ----------------------------------------------------------------------*/
    if ( !(LSA_HOST_PTR_ARE_EQUAL(pHDB->pGEN, LSA_NULL)) )
    {
        #ifndef EDDP_CFG_LOCAL_MEM_STATIC
        // *****  Release all memory used by the handle.
        if (EDDP_GlbFreeLocalMem(pHDB->pGEN) != LSA_RET_OK)
        {
            Status = EDD_STS_ERR_RESOURCE;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Status);
            EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_GENCloseChannel(): ERROR -> error EDDP_GlbFreeLocalMem(pHDB->pGEN)!");
        }
        pHDB->pGEN = LSA_NULL;  // we always release the handle
        #endif
    }

    return (Status);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GENSetRemotePortState                  +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters          (in) +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function stores the given remote port states and     +*/
/*+               triggers the RTC2/3-PortStatemachine.                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GENSetRemotePortState(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT16                                  PortID;
    EDD_RSP                                     Status;
    EDD_UPPER_SET_REMOTE_PORT_STATE_PTR_TYPE    pParam;

    Status  = EDD_STS_OK;
    pParam  = (EDD_UPPER_SET_REMOTE_PORT_STATE_PTR_TYPE) pRQB->pParam;
    PortID  = pParam->PortID;

    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_GENSetRemotePortState(PortID=%d)", PortID);

    if ((PortID >= 1) && (PortID <= pDDB->SWI.PortCnt))
    {
        // do not delete the empty spaces in this trace text, the parameters have to match with trace text below
        EDDP_CORE_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW,"[H:--]    pRQB->pParam: SyncId0_TopoState=%d, RTC2-TopoState=%d, RTC3-TopoState=%d, RTC3-PDEVTopoState=%d, RTC3-RemoteState=%d, Neighbour_TopoState=%d", 
                                pParam->SyncId0_TopoState,
                                pParam->RtClass2_TopoState, 
                                pParam->RtClass3_TopoState,
                                pParam->RtClass3_PDEVTopoState,
                                pParam->RtClass3_RemoteState,
                                pParam->Neighbour_TopoState);

        if (pParam->RtClass3_PDEVTopoState != EDD_SET_REMOTE_PORT_STATE_IGNORE)
        {
            pDDB->SWI.RTC3_PortState[PortID].PDEVTopoState = pParam->RtClass3_PDEVTopoState; 
        }

        if (pParam->RtClass3_RemoteState != EDD_SET_REMOTE_PORT_STATE_IGNORE)
        {
            pDDB->SWI.RTC3_PortState[PortID].RemoteState = pParam->RtClass3_RemoteState;
        }

        if (pParam->RtClass3_TopoState != EDD_SET_REMOTE_PORT_STATE_IGNORE)
        {        
            pDDB->SWI.RTC3_PortState[PortID].TopoState = pParam->RtClass3_TopoState;
        }

        if (pParam->RtClass2_TopoState != EDD_SET_REMOTE_PORT_STATE_IGNORE)
        {
            pDDB->SWI.RtClass2_TopoState[PortID] = pParam->RtClass2_TopoState;
        }

        if (pParam->SyncId0_TopoState != EDD_SET_REMOTE_PORT_STATE_IGNORE)
        {
            if (EDD_SET_REMOTE_PORT_STATE_TOPO_OK == pParam->SyncId0_TopoState)
            {   // 1
                pDDB->SWI.LinkStatus[PortID].SyncId0_TopoOk = EDD_PORT_SYNCID0_TOPO_OK;
            }
            else if (EDD_SET_REMOTE_PORT_STATE_TOPO_MISMATCH == pParam->SyncId0_TopoState)
            {   // 0
                pDDB->SWI.LinkStatus[PortID].SyncId0_TopoOk = EDD_PORT_SYNCID0_TOPO_NOT_OK;
            }
            else
            {
                Status = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Status);
                EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_GENSetRemotePortState(): ERROR -> pParam->SyncId0_TopoState(0x%X) is unknown state!", 
                    pParam->SyncId0_TopoState);
            }	
        }

        // ***** store Neighbour_TopoState for EDDP_SWI_Trigger_PreambleLength()
        #ifdef EDDP_PRM_PREAMBLELENGTH_HACK
        pDDB->SWI.LinkStatus[PortID].Neighbour_TopoState = EDD_SET_REMOTE_PORT_STATE_TOPO_OK;
        #else
        pDDB->SWI.LinkStatus[PortID].Neighbour_TopoState = pParam->Neighbour_TopoState;
        #endif

        // do not delete the empty spaces in this trace text, the parameters have to match with trace text above
        EDDP_CORE_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW,"[H:--]    SWI         : SyncId0_TopoOk=%d,    RTC2-TopoState=%d, RTC3-TopoState=%d, RTC3-PDEVTopoState=%d, RTC3-RemoteState=%d, Neighbour_TopoState=%d", 
                            pDDB->SWI.LinkStatus[PortID].SyncId0_TopoOk, 
                            pDDB->SWI.RtClass2_TopoState[PortID], 
                            pDDB->SWI.RTC3_PortState[PortID].TopoState, 
                            pDDB->SWI.RTC3_PortState[PortID].PDEVTopoState, 
                            pDDB->SWI.RTC3_PortState[PortID].RemoteState, 
                            pDDB->SWI.LinkStatus[PortID].Neighbour_TopoState);

        // trigger PreambleLength
        EDDP_SWI_Trigger_PreambleLength (pDDB, PortID);
        // trigger RTC3 port state machine
        EDDP_SWI_Trigger_RTC3_PortState (pDDB, PortID);
    }
    else
    {
        Status = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Status);
        EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_GENSetRemotePortState(): ERROR -> RQB-PortID(%d) is out of range [1...%d]", 
            PortID, pDDB->SWI.PortCnt);
    }

    return (Status);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GEN_Set_IP_Address                     +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters          (in) +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function writes the local IP address to the PNIP     +*/
/*+               register(s) and stores it in DDB.                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GEN_Set_IP_Address(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP                     Status;
    LSA_UINT32                  reg_address;
    LSA_UINT32                  reg_value;
    EDD_UPPER_SET_IP_PTR_TYPE   pParam;

    Status  = EDD_STS_OK;
    pParam  = (EDD_UPPER_SET_IP_PTR_TYPE) pRQB->pParam;

    reg_address = PNIP_REG_LOCAL_IP_SA_0;
    reg_value = ((LSA_INT32)pParam->LocalIP.IPAdr[0]) << 24 |
                ((LSA_INT32)pParam->LocalIP.IPAdr[1]) << 16 |
                ((LSA_INT32)pParam->LocalIP.IPAdr[2]) << 8  |
                ((LSA_INT32)pParam->LocalIP.IPAdr[3]);

    EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_value);

    // store new interface IP address in DDB
    pDDB->SWI.IF_IP_Address = pParam->LocalIP;

    EDDP_CORE_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_GEN_Set_IP_Address(): IP address is set to: %d.%d.%d.%d", 
        pParam->LocalIP.IPAdr[0], pParam->LocalIP.IPAdr[1], pParam->LocalIP.IPAdr[2], pParam->LocalIP.IPAdr[3] );

    return (Status);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GEN_RequestResetStatistics             +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                         +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         Opcode:     EDD_OPC_REQUEST                 +*/
/*+     LSA_HANDLE_TYPE         Handle:                                     +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user (not used)           +*/
/*+     LSA_COMP_ID_TYPE        CompId:     Cmponent-ID                     +*/
/*+     EDD_SERVICE             Service:    EDD_SRV_RESET_STATISTICS        +*/
/*+     LSA_RESULT              Status:     Return status                   +*/
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:                                     +*/
/*+                                                                         +*/
/*+  pParam points to EDD_UPPER_RESET_STATISTICS_PTR_TYPE                   +*/
/*+                                                                         +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         opcode:     unchanged                       +*/
/*+     LSA_HANDLE_TYPE         Handle:     unchanged                       +*/
/*+     LSA_USER_ID_TYPE        UserId:     unchanged                       +*/
/*+     LSA_COMP_ID_TYPE        CompId:     unchanged                       +*/
/*+     EDD_SERVICE             Service:    unchanged                       +*/
/*+     LSA_RESULT              Status:     unchanged                       +*/
/*+     EDD_UPPER_RESET_STATISTICS_PTR_TYPE   pParam: filled with statistics+*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function handles the EDD_SRV_RESET_STATISTICS request+*/
/*+               It will reset the statistics of the IF and/or Port. This  +*/
/*+               is done by setting the "resetvalue" to the actual values  +*/
/*+               (because the reset is done per if/port and per handle!)   +*/
/*+                                                                         +*/
/*+               Note: Opcode/Service not checked!                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GEN_RequestResetStatistics(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP                                 Status;
    LSA_UINT32                              ID, HwPortID;
    EDD_UPPER_RESET_STATISTICS_PTR_TYPE     pResStats;

    pResStats = (EDD_UPPER_RESET_STATISTICS_PTR_TYPE) pRQB->pParam;

    if (   (LSA_HOST_PTR_ARE_EQUAL(pResStats, LSA_NULL))
        || (LSA_HOST_PTR_ARE_EQUAL(pHDB->pGEN, LSA_NULL))
        || (   (pResStats->PortID > pDDB->SWI.PortCnt)
            && (pResStats->PortID != EDD_STATISTICS_RESET_ALL)
           )
       )
    {
        Status = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Status);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_GEN_RequestResetStatistics(): ERROR -> parameter error!");
    }
    else
    {
        Status = EDD_STS_OK;

        // ***** reset statistic
        if (pResStats->PortID == EDD_STATISTICS_RESET_ALL)
        {
            // ***** reset all statistics
            for ( ID=1; ID <= pDDB->SWI.PortCnt; ID++ )
            {
                // ***** reset single port statistic
                HwPortID = EDDP_SWI_get_HwPortID (pDDB, ID);
                EDDP_GEN_GetPortStatistics (pDDB, &pHDB->pGEN->Statistic[ID], HwPortID);
            }
            // ***** reset interface statistic
            EDDP_GEN_GetIfaceStatistics (pDDB, &pHDB->pGEN->Statistic[0]);
        }
        else
        {
            // ***** reset single statistic
            if (pResStats->PortID)
            {
                // ***** reset single port statistic
                HwPortID = EDDP_SWI_get_HwPortID (pDDB, pResStats->PortID);
                EDDP_GEN_GetPortStatistics (pDDB, &pHDB->pGEN->Statistic[pResStats->PortID], (LSA_UINT16)HwPortID);
            }
            else
            {   // ***** reset interface statistic
                EDDP_GEN_GetIfaceStatistics (pDDB, &pHDB->pGEN->Statistic[0]);
            }
        }
    }

    return (Status);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GEN_RequestGetStatistics               +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                         +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         Opcode:     EDD_OPC_REQUEST                 +*/
/*+     LSA_HANDLE_TYPE         Handle:                                     +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user (not used)           +*/
/*+     LSA_COMP_ID_TYPE        CompId:     Cmponent-ID                     +*/
/*+     EDD_SERVICE             Service:    EDD_SRV_GET_STATISTICS          +*/
/*+     LSA_RESULT              Status:     Return status                   +*/
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:                                     +*/
/*+                                                                         +*/
/*+  pParam points to EDD_UPPER_GET_STATISTICS_PTR_TYPE                     +*/
/*+                                                                         +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         opcode:     unchanged                       +*/
/*+     LSA_HANDLE_TYPE         Handle:     unchanged                       +*/
/*+     LSA_USER_ID_TYPE        UserId:     unchanged                       +*/
/*+     LSA_COMP_ID_TYPE        CompId:     unchanged                       +*/
/*+     EDD_SERVICE             Service:    unchanged                       +*/
/*+     LSA_RESULT              Status:     unchanged                       +*/
/*+     EDD_UPPER_GET_STATISTICS_PTR_TYPE    pParam: filled with statistics +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function handles the EDD_SRV_GET_STATISTICS request. +*/
/*+               It fills the pParam structure with the actual statistics  +*/
/*+                                                                         +*/
/*+               The statistics will be per channel and IF/Port.           +*/
/*+                                                                         +*/
/*+               Note: Opcode/Service not checked!                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GEN_RequestGetStatistics(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP                                 Status;
    EDD_UPPER_GET_STATISTICS_PTR_TYPE       pStats;
    LSA_UINT32                              HwPortID;
    EDDP_GET_STATISTICS_TYPE                Statistic;

    pStats = (EDD_UPPER_GET_STATISTICS_PTR_TYPE) pRQB->pParam;

    if (   (LSA_HOST_PTR_ARE_EQUAL(pStats, LSA_NULL))
        || (LSA_HOST_PTR_ARE_EQUAL(pHDB->pGEN, LSA_NULL))
        || (pStats->PortID > pDDB->SWI.PortCnt)
        || (   (EDD_STATISTICS_DO_NOT_RESET_VALUES != pStats->Reset)
            && (EDD_STATISTICS_RESET_VALUES != pStats->Reset)) 
       )
    {
        Status = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Status);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_GEN_RequestGetStatistics(): ERROR -> parameter error!");
    }
    else
    {
        Status = EDD_STS_OK;

        // use RequestedCounters from RQB
        Statistic.RequestedCounters = pStats->sMIB.RequestedCounters;

        // ***** fill statistic for Interface (pStats->PortID=0) or Port (pStats->PortID=1..n)
        if (pStats->PortID)
        {
            // ***** read statistic snapshot for diff. result 
            HwPortID = EDDP_SWI_get_HwPortID (pDDB, pStats->PortID);
            EDDP_GEN_GetPortStatistics (pDDB, &Statistic, HwPortID);
        }
        else
        {
            // ***** read statistic snapshot for diff. result 
            EDDP_GEN_GetIfaceStatistics (pDDB, &Statistic);
        }

        // ***** build diff. result and copy real counters into RQB->pParam
        EDDP_GEN_SubStatistics (pDDB, &Statistic, &pHDB->pGEN->Statistic[pStats->PortID], pStats->Reset);
        _EDDP_GEN_CopyStatistics (pDDB, &pStats->sMIB, &Statistic);
    }

    return (Status);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GEN_RequestGetStatisticsAll            +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                         +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         Opcode:     EDD_OPC_REQUEST                 +*/
/*+     LSA_HANDLE_TYPE         Handle:                                     +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user (not used)           +*/
/*+     LSA_COMP_ID_TYPE        CompId:     Cmponent-ID                     +*/
/*+     EDD_SERVICE             Service:    EDD_SRV_GET_STATISTICS_ALL      +*/
/*+     LSA_RESULT              Status:     Return status                   +*/
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:                                     +*/
/*+                                                                         +*/
/*+  pParam points to EDD_UPPER_GET_STATISTICS_ALL_PTR_TYPE                 +*/
/*+                                                                         +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         opcode:     unchanged                       +*/
/*+     LSA_HANDLE_TYPE         Handle:     unchanged                       +*/
/*+     LSA_USER_ID_TYPE        UserId:     unchanged                       +*/
/*+     LSA_COMP_ID_TYPE        CompId:     unchanged                       +*/
/*+     EDD_SERVICE             Service:    unchanged                       +*/
/*+     LSA_RESULT              Status:     unchanged                       +*/
/*+     EDD_UPPER_GET_STATISTICS_ALL_PTR_TYPE pParam: filled with statistics+*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function handles the EDD_SRV_GET_STATISTICS_ALL req. +*/
/*+               It fills the pParam structure with the actual statistics  +*/
/*+                                                                         +*/
/*+               The statistics will be per channel.                       +*/
/*+                                                                         +*/
/*+               Note: Opcode/Service not checked!                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GEN_RequestGetStatisticsAll(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP                                 Response;
    LSA_UINT32                              HwPortID, PortID;
    EDD_UPPER_GET_STATISTICS_ALL_PTR_TYPE   pStats;
    EDDP_GET_STATISTICS_TYPE                Statistic;

    pStats = (EDD_UPPER_GET_STATISTICS_ALL_PTR_TYPE) pRQB->pParam;

    if (   (LSA_HOST_PTR_ARE_EQUAL(pStats, LSA_NULL))
        || (LSA_HOST_PTR_ARE_EQUAL(pHDB->pGEN, LSA_NULL))
        || ((EDD_STATISTICS_DO_NOT_RESET_VALUES != pStats->Reset)
            && (EDD_STATISTICS_RESET_VALUES != pStats->Reset))
        )
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_GEN_RequestGetStatisticsAll(): ERROR -> parameter error!");
    }
    else
    {
        Response    = EDD_STS_OK;
        PortID      = 0;

        // fill statistic for Ports and Interface
        // PortID=0     : interface
        // PortID=1..n  : Port
        while ((PortID <= pDDB->SWI.PortCnt) )
        {
            // use RequestedCounters from RQB
            Statistic.RequestedCounters = pStats->sMIB[PortID].RequestedCounters;

            if (PortID)
            {
                // ***** read statistic snapshot for diff. result
                HwPortID = EDDP_SWI_get_HwPortID (pDDB,PortID);
                EDDP_GEN_GetPortStatistics (pDDB, &Statistic, HwPortID);
            }
            else
            {
                // ***** read statistic snapshot for diff. result
                EDDP_GEN_GetIfaceStatistics (pDDB, &Statistic);
            }

            // ***** build diff. result and copy real counters into RQB->pParam
            EDDP_GEN_SubStatistics (pDDB, &Statistic, &pHDB->pGEN->Statistic[PortID], pStats->Reset);
            _EDDP_GEN_CopyStatistics (pDDB, &pStats->sMIB[PortID], &Statistic);

            PortID++;
        }
    }

    return (Response);
}

#if 1 /** RZ/T2M additional codes */
void EDDP_Set_IRT_DelayTime(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT32                  PortID);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_RequestSetLineDelay                            +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters          (in) +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function handles the EDD_SRV_SET_LINE_DELAY request  +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_RequestSetLineDelay(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    LSA_RESULT                              Status;
	LSA_UINT32				                EddpDelayTicks;
	LSA_UINT32	                			EddpCableDelay;
	LSA_UINT16		                        PortID;
    LSA_BOOL                                Changed;

    Status = EDD_STS_OK;

    PortID = (((EDD_RQB_SET_LINE_DELAY_TYPE *)((pRQB)->pParam))->PortID);
	EddpDelayTicks = (((EDD_RQB_SET_LINE_DELAY_TYPE *)((pRQB)->pParam))->LineDelayInTicksMeasured);
	EddpCableDelay = (((EDD_RQB_SET_LINE_DELAY_TYPE *)((pRQB)->pParam))->CableDelayInNsMeasured);

    if (( PortID == 0) || (PortID > pDDB->SWI.PortCnt))
    {
        Status = EDD_STS_ERR_PARAM;
    }
    else
    {
        Changed = LSA_FALSE;
        if (( pDDB->SWI.LinkStatus[PortID].LineDelayInTicksMeasured    != EddpDelayTicks ) ||
            ( pDDB->SWI.LinkStatus[PortID].CableDelayInNsMeasured != EddpCableDelay   ))
        {
            Changed = LSA_TRUE;
            EDDP_Set_IRT_DelayTime(pDDB,PortID);   // Axel 
        }

        
        pDDB->SWI.LinkStatus[PortID].LineDelayInTicksMeasured    = EddpDelayTicks;
        pDDB->SWI.LinkStatus[PortID].CableDelayInNsMeasured = EddpCableDelay;
        if (Changed) EDDP_GENIndicateLinkChange_Ext(pDDB,PortID);
        Status = EDD_STS_OK;  /* no error */
    }

    return(Status);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GEN_RequestSetSyncState                +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters          (in) +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function handles the EDD_SRV_SET_SYNC_STATE request  +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GEN_RequestSetSyncState(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    /** Unused parameter */
    LSA_UNUSED_ARG(pHDB);
    
    /** Error codes */
    LSA_RESULT	Status = EDD_STS_OK;
    
    /** PortID */
    LSA_UINT32 PortID = 0;
    
    /** Resolve the type of RQB parameter block */
    EDD_RQB_SET_SYNC_STATE_TYPE * pRqbParam = (EDD_RQB_SET_SYNC_STATE_TYPE*) pRQB->pParam;

    /** Check sync state from upper module (GSY) */
    switch ( pRqbParam->SyncState )
    {
    case EDD_SYNC_STATE_GOOD:	/** Synchronized */
        
        /** 
         * This implementation refers to EDDP_K32SyncSet().
         * The function indicates KRISC32 sync state to RTClass3 state machine on SWI.
         */
        
        /** Set SynkOK */
        pDDB->SWI.SyncOk = LSA_TRUE;
        
        /** Apply SynkOk to the RTC3 PortStates of all ports */
        for ( PortID=1; PortID <= pDDB->SWI.PortCnt; PortID++ )
        {
            EDDP_SWI_Trigger_RTC3_PortState(pDDB, PortID);
        }
        
        break;
        
    case EDD_SYNC_STATE_BAD:	/** Not synchronized or unknown */
        
        /** Unset SyncOK */
        pDDB->SWI.SyncOk = LSA_FALSE;
        
        /** Apply SynkOk to the RTC3 PortStates of all ports */
        for ( PortID=1; PortID <= pDDB->SWI.PortCnt; PortID++ )
        {
            EDDP_SWI_Trigger_RTC3_PortState(pDDB, PortID);
        }
        
        break;
        
    default: /** Invalid argument */

        Status = EDD_STS_ERR_PARAM;

        break;
    }
    
    /** Return error codes */
    return( Status );
}

#endif

/*****************************************************************************/
/*  end of file eddp_core_gen.c                                              */
/*****************************************************************************/
