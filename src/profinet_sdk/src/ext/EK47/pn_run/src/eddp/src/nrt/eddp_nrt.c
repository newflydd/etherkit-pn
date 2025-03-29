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
/*  F i l e               &F: eddp_nrt.c                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP Debugging support functions                 */
/*                          (system dependend)                               */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  24.09.08    SF    initial version.                                       */
/*  15.10.12    MH	  AP01185150 - DCP Hello Filter "FrameFilterInfo"		 */
/*								   bei RCV ist nicht belegt (0)              */
#endif
/*****************************************************************************/



/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/

#define LTRC_ACT_MODUL_ID  61
#define EDDP_MODULE_ID     LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_NRT */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"               /* eddp headerfiles */
#include "eddp_int.h"               /* internal header  */
#include "eddp_hwc.h"               /* hardwarefilter code  */
#include "eddp_nrt_inc.h"


EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)


// ***** test for EDDP_HALMemset()
#define EDDP_NRT_MEMSET_CHK


//#define EDDP_CFG_LOCAL_MEM_STATIC

// Attention: HDBs[0..EDDP_CFG_MAX_CHANNELS] used in Openchannel() are over all devices/interfaces !!!

#ifdef EDDP_CFG_LOCAL_MEM_STATIC
    // Note: EDDP_CFG_MAX_CHANNELS is over all interfaces !!!
    EDDP_LOCAL_MEM_ATTR EDDP_HDB_NRT_TYPE       g_EDDP_HDB_NRT[EDDP_CFG_MAX_CHANNELS];
    //EDDP_LOCAL_MEM_ATTR EDDP_RQB_QUEUE_TYPE   g_EDDP_NRT_RqbChannelQueue[EDDP_CFG_MAX_CHANNELS]; --> is in g_EDDP_HDB_NRT[]
    //EDD_RQB_TYPE                              g_EDDP_HDB_NRT_CANCEL_RQB[EDD_CFG_MAX_CHANNELS];
    //EDD_RQB_DEV_INTERNAL_TYPE                 g_EDDP_HDB_NRT_CANCEL_RQB_PARAM[EDD_CFG_MAX_CHANNELS];
#endif


/*===========================================================================*/
/*                        private functions                                  */
/*===========================================================================*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_NRTInitMgmZero                        +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB         : Pointer to DeviceDescriptionBlock                       +*/
/*+                                                                         +*/
/*+  Result       : EDD_STS_OK                                              +*/
/*+                 EDD_STS_ERR_PARAM                                       +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: utility function for EDDP_NRTDeviceOpen()                 +*/
/*+               --> set internal NRT mgm data to valid defaults           +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_NRTInitMgmZero(
    EDDP_LOCAL_DDB_PTR_TYPE pDDB)
{
    EDDP_LOCAL_DDB_NRT_PTR_TYPE pNRTIface;
    LSA_UINT32   Idx;

    //EDDP_ASSERT_NO_NULL_PTR(pDDB);                                                                               // checked by caller

    pNRTIface                   = &pDDB->NRT;
    pNRTIface->UsedTxRingMask   = 0;
    pNRTIface->UsedRxRingMask   = 0;

    switch (pDDB->Hardware)
    {
        case EDDP_HW_ERTEC200P:
        case EDDP_HW_ERTEC200P_FPGA:
        {
            pNRTIface->DcpFilterCnt         = (LSA_UINT8)(1 + pDDB->SWI.PortCnt); // --> buffer for IF + Interface-Ports
            pNRTIface->DcpHelloFilterCnt    = 3;   // for testing only
        }
        break;
        case EDDP_HW_HERA_FPGA:
        {
            pNRTIface->DcpFilterCnt         = (LSA_UINT8)(1 + pDDB->SWI.PortCnt); // --> buffer for IF + Interface-Ports
            pNRTIface->DcpHelloFilterCnt    = 32;
        }
        break;

        default:
        {
            EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] _EDDP_NRTInitMgmZero(): ERROR -> unknown HW type(%d)!", pDDB->Hardware);
            EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
            return (EDD_STS_ERR_PARAM);
        }
        break;
    }

    // ***** currently no handles open
    for ( Idx=0; Idx < EDDP_NRT_MAX_FDC_CNT; Idx++)
    {
        pNRTIface->FilterInfo[Idx].pNrtHDB = LSA_NULL;
    }

    pNRTIface->HandleCnt = 0;

    // ***** build mapping filter information
    // 0
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DROP_IDX].UsrFrameFilter					= 0;                            // special ring for development
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DROP_IDX].HWFrameFilter					= EDDP_FDC_DROP_MASK;           // special bit in FDC for drop !!!
    // 1
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_OTHER_IDX].UsrFrameFilter					= EDD_NRT_FRAME_OTHER;          // ring for all other frames
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_OTHER_IDX].HWFrameFilter					= EDDP_FDC_OTHER;               // FDC in HW-Ring 0
    // 2
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_IP_ICMP_IDX].UsrFrameFilter				= EDD_NRT_FRAME_IP_ICMP;        // public EDD select filter ICMP
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_IP_ICMP_IDX].HWFrameFilter				= EDDP_FDC_IP_ICMP;             // FDC in HW-Ring
    // 3
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_IP_IGMP_IDX].UsrFrameFilter				= EDD_NRT_FRAME_IP_IGMP;        // public EDD select filter ICMP
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_IP_IGMP_IDX].HWFrameFilter				= EDDP_FDC_IP_IGMP;             // FDC in HW-Ring
    // 4
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_IP_TCP_IDX].UsrFrameFilter				= EDD_NRT_FRAME_IP_TCP;         // public EDD select filter TCP
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_IP_TCP_IDX].HWFrameFilter					= EDDP_FDC_IP_TCP;              // FDC in HW-Ring
    // 5
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_IP_UDP_IDX].UsrFrameFilter				= EDD_NRT_FRAME_IP_UDP;         // public EDD select filter UDP
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_IP_UDP_IDX].HWFrameFilter					= EDDP_FDC_IP_UDP;              // FDC in HW-Ring
    // 6
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_IP_OTHER_IDX].UsrFrameFilter				= EDD_NRT_FRAME_IP_OTHER;       // public EDD select filter IP_OTHER
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_IP_OTHER_IDX].HWFrameFilter				= EDDP_FDC_IP_OTHER;            // FDC in HW-Ring
    // 7
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_UDP_DCP_IDX].UsrFrameFilter				= EDD_NRT_FRAME_UDP_DCP;        // public EDD select filter UDP_DCP
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_UDP_DCP_IDX].HWFrameFilter				= EDDP_FDC_UDP_DCP;             // FDC in HW-Ring
    // 8
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_UDP_DCP_HELLO_IDX].UsrFrameFilter			= EDD_NRT_FRAME_UDP_DCP_HELLO;  // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_UDP_DCP_HELLO_IDX].HWFrameFilter			= EDDP_FDC_UDP_DCP_HELLO;       // FDC in HW-Ring
    // 9
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_UDP_ASRT_IDX].UsrFrameFilter				= EDD_NRT_FRAME_UDP_ASRT;       // public EDD select filter UDP_ASRT
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_UDP_ASRT_IDX].HWFrameFilter				= EDDP_FDC_UDP_ASRT;            // FDC in HW-Ring
    // 10
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_ARP_IDX].UsrFrameFilter					= EDD_NRT_FRAME_ARP;            // public EDD select filter ARP
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_ARP_IDX].HWFrameFilter					= EDDP_FDC_ARP;                 // FDC in HW-Ring
    // 11
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_MRP_IDX].UsrFrameFilter					= EDD_NRT_FRAME_MRP;            // public EDD select filter MRP
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_MRP_IDX].HWFrameFilter					= EDDP_FDC_MRP;                 // FDC in HW-Ring
    // 12
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_LLDP_IDX].UsrFrameFilter					= EDD_NRT_FRAME_LLDP;           // public EDD select filter LLDP
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_LLDP_IDX].HWFrameFilter					= EDDP_FDC_LLDP;                // FDC in HW-Ring
    // 13
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_IDX].UsrFrameFilter					= EDD_NRT_FRAME_DCP;            // public EDD select filter DCP
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_IDX].HWFrameFilter					= EDDP_FDC_DCP;                 // FDC in HW-Ring
    // 14
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX].UsrFrameFilter				= EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX].HWFrameFilter				= EDDP_FDC_DCP_HELLO;           // FDC in HW-Ring
    // 15
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_ASRT_IDX].UsrFrameFilter					= EDD_NRT_FRAME_ASRT;           // public EDD select filter ASRT (ALARM)
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_ASRT_IDX].HWFrameFilter					= EDDP_FDC_ASRT;                // FDC in HW-Ring
    // 16
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_LEN_DCP_IDX].UsrFrameFilter				= EDD_NRT_FRAME_LEN_DCP;        // public EDD select filter LEN_DCP
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_LEN_DCP_IDX].HWFrameFilter				= EDDP_FDC_LEN_DCP;             // FDC in HW-Ring
    // 17
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_LEN_TSYNC_IDX].UsrFrameFilter				= EDD_NRT_FRAME_LEN_TSYNC;      // public EDD select filter LEN_TSYNC
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_LEN_TSYNC_IDX].HWFrameFilter				= EDDP_FDC_LEN_TSYNC;           // FDC in HW-Ring
    // 18
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_LEN_STDBY_IDX].UsrFrameFilter				= EDD_NRT_FRAME_LEN_STDBY;      // public EDD select filter LEN_STDBY
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_LEN_STDBY_IDX].HWFrameFilter				= EDDP_FDC_LEN_STDBY;           // FDC in HW-Ring
    // 19
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_LEN_HSR_IDX].UsrFrameFilter				= EDD_NRT_FRAME_LEN_HSR;        // public EDD select filter LEN_HSR
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_LEN_HSR_IDX].HWFrameFilter				= EDDP_FDC_LEN_HSR;             // FDC in HW-Ring
    // 20
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_LEN_SINEC_FWL_IDX].UsrFrameFilter			= EDD_NRT_FRAME_LEN_SINEC_FWL;  // public EDD select filter for FW-Loader
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_LEN_SINEC_FWL_IDX].HWFrameFilter			= EDDP_FDC_LEN_SINEC_FWL;       // FDC in HW-Ring
    // 21
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_LEN_OTHER_IDX].UsrFrameFilter				= EDD_NRT_FRAME_LEN_OTHER;      // public EDD select filter LEN_OTHER
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_LEN_OTHER_IDX].HWFrameFilter				= EDDP_FDC_LEN_OTHER;           // FDC in HW-Ring
    // 22
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_PTCP_IDX].UsrFrameFilter					= EDD_NRT_FRAME_PTCP_ANNO;      // public EDD select filter PTCP (announce only in NRT !!!)
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_PTCP_IDX].HWFrameFilter					= EDDP_FDC_PTCP_ANNOUNCE;       // FDC in HW-Ring  (other frames to KRISC: sync0/1, delay)
    // 23
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_1].UsrFrameFilter	= EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_1].HWFrameFilter		= EDDP_FDC_DCP_HELLO;           // FDC in HW-Ring for HelloFilter String 1
    // 24
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_2].UsrFrameFilter	= EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_2].HWFrameFilter     = EDDP_FDC_DCP_HELLO2;          // FDC in HW-Ring for HelloFilter String 2
    // 25
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_3].UsrFrameFilter    = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_3].HWFrameFilter     = EDDP_FDC_DCP_HELLO3;          // FDC in HW-Ring for HelloFilter String 3
    //26
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_4].UsrFrameFilter    = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_4].HWFrameFilter     = EDDP_FDC_DCP_HELLO4;          // FDC in HW-Ring for HelloFilter String 4
    //27
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_5].UsrFrameFilter    = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_5].HWFrameFilter     = EDDP_FDC_DCP_HELLO5;          // FDC in HW-Ring for HelloFilter String 5
    //28
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_6].UsrFrameFilter    = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_6].HWFrameFilter     = EDDP_FDC_DCP_HELLO6;          // FDC in HW-Ring for HelloFilter String 6
    //29
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_7].UsrFrameFilter    = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_7].HWFrameFilter     = EDDP_FDC_DCP_HELLO7;          // FDC in HW-Ring for HelloFilter String 7
    //30
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_8].UsrFrameFilter    = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_8].HWFrameFilter     = EDDP_FDC_DCP_HELLO8;          // FDC in HW-Ring for HelloFilter String 8
    //31
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_9].UsrFrameFilter    = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_9].HWFrameFilter     = EDDP_FDC_DCP_HELLO9;          // FDC in HW-Ring for HelloFilter String 9
    //32
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_10].UsrFrameFilter   = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_10].HWFrameFilter    = EDDP_FDC_DCP_HELLO10;         // FDC in HW-Ring for HelloFilter String 10
    //33
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_11].UsrFrameFilter   = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_11].HWFrameFilter    = EDDP_FDC_DCP_HELLO11;         // FDC in HW-Ring for HelloFilter String 11
    //34
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_12].UsrFrameFilter   = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_12].HWFrameFilter    = EDDP_FDC_DCP_HELLO12;         // FDC in HW-Ring for HelloFilter String 12
    //35
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_13].UsrFrameFilter   = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_13].HWFrameFilter    = EDDP_FDC_DCP_HELLO13;         // FDC in HW-Ring for HelloFilter String 13
    //36
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_14].UsrFrameFilter   = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_14].HWFrameFilter    = EDDP_FDC_DCP_HELLO14;         // FDC in HW-Ring for HelloFilter String 14
    //37
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_15].UsrFrameFilter   = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_15].HWFrameFilter    = EDDP_FDC_DCP_HELLO15;         // FDC in HW-Ring for HelloFilter String 15
    //38
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_16].UsrFrameFilter   = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_16].HWFrameFilter    = EDDP_FDC_DCP_HELLO16;         // FDC in HW-Ring for HelloFilter String 16
    //39
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_17].UsrFrameFilter   = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_17].HWFrameFilter    = EDDP_FDC_DCP_HELLO17;         // FDC in HW-Ring for HelloFilter String 17
    //40
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_18].UsrFrameFilter   = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_18].HWFrameFilter    = EDDP_FDC_DCP_HELLO18;         // FDC in HW-Ring for HelloFilter String 18
    //41
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_19].UsrFrameFilter   = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_19].HWFrameFilter    = EDDP_FDC_DCP_HELLO19;         // FDC in HW-Ring for HelloFilter String 19
    //42
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_20].UsrFrameFilter   = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_20].HWFrameFilter    = EDDP_FDC_DCP_HELLO20;         // FDC in HW-Ring for HelloFilter String 20
    //43
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_21].UsrFrameFilter   = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_21].HWFrameFilter    = EDDP_FDC_DCP_HELLO21;         // FDC in HW-Ring for HelloFilter String 21
    //44
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_22].UsrFrameFilter   = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_22].HWFrameFilter    = EDDP_FDC_DCP_HELLO22;         // FDC in HW-Ring for HelloFilter String 22
    //45
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_23].UsrFrameFilter   = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_23].HWFrameFilter    = EDDP_FDC_DCP_HELLO23;         // FDC in HW-Ring for HelloFilter String 23
    //46
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_24].UsrFrameFilter   = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_24].HWFrameFilter    = EDDP_FDC_DCP_HELLO24;         // FDC in HW-Ring for HelloFilter String 24
    //47
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_25].UsrFrameFilter   = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_25].HWFrameFilter    = EDDP_FDC_DCP_HELLO25;         // FDC in HW-Ring for HelloFilter String 25
    //48
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_26].UsrFrameFilter   = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_26].HWFrameFilter    = EDDP_FDC_DCP_HELLO26;         // FDC in HW-Ring for HelloFilter String 26
    //49
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_27].UsrFrameFilter   = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_27].HWFrameFilter    = EDDP_FDC_DCP_HELLO27;         // FDC in HW-Ring for HelloFilter String 27
    //50
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_28].UsrFrameFilter   = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_28].HWFrameFilter    = EDDP_FDC_DCP_HELLO28;         // FDC in HW-Ring for HelloFilter String 28
    //51
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_29].UsrFrameFilter   = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_29].HWFrameFilter    = EDDP_FDC_DCP_HELLO29;         // FDC in HW-Ring for HelloFilter String 29
    //52
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_30].UsrFrameFilter   = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_30].HWFrameFilter    = EDDP_FDC_DCP_HELLO30;         // FDC in HW-Ring for HelloFilter String 30
    //53
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_31].UsrFrameFilter   = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_31].HWFrameFilter    = EDDP_FDC_DCP_HELLO31;         // FDC in HW-Ring for HelloFilter String 31
    //54
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_32].UsrFrameFilter   = EDD_NRT_FRAME_DCP_HELLO;      // public EDD select filter DCP_HELLO
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_32].HWFrameFilter    = EDDP_FDC_DCP_HELLO32;         // FDC in HW-Ring for HelloFilter String 32
    // 55 (only for validate NRTFrameFilter on OpenChannel)
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_PTCP_SYNC_IDX].UsrFrameFilter             = EDD_NRT_FRAME_PTCP_SYNC;      // public EDD select filter PTCP_SYNC
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_PTCP_SYNC_IDX].HWFrameFilter              = EDDP_FDC_INVALID;             // HWFrameFilter is not used, PTCP_SYNC is handled by KRISC
    // 56 (only for validate NRTFrameFilter on OpenChannel)
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_PTCP_DELAY_IDX].UsrFrameFilter            = EDD_NRT_FRAME_PTCP_DELAY;     // public EDD select filter PTCP_DELAY
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_PTCP_DELAY_IDX].HWFrameFilter             = EDDP_FDC_INVALID;             // HWFrameFilter is not used, PTCP_DELAY is handled by KRISC
    // 57
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DLR_IDX].UsrFrameFilter                   = EDD_NRT_FRAME_DLR;            // public EDD select filter MRP
    pNRTIface->FilterInfo[EDDP_NRT_FILTER_DLR_IDX].HWFrameFilter                    = EDDP_FDC_MRP;                 // FDC in HW-Ring
    
    // 57
    // RT_CT --> use drop counter only for debug
    // 58
    // RT_SF --> use drop counter only for debug

    pNRTIface->IfaceStatus = EDDP_NRT_IF_PREINIT;

    return (EDD_STS_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_NRTInitRcvCurrDmacwPtrs               +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_DDB_NRT_PTR_TYPE pNRTIface       +*/
/*+                             LSA_UINT32                  RingMask        +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB         : Pointer to DeviceDescriptionBlock                       +*/
/*+  pNRTIface    : Pointer to internal Mgm structure                       +*/
/*+  RingMask     : selector for RX-Ring (bits 0..31)                       +*/
/*+                                                                         +*/
/*+  Result       : EDD_STS_OK                                              +*/
/*+                 EDD_STS_ERR_PARAM                                       +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: utility function for _EDDP_NRTInitInterface()             +*/
/*+               write entry (CurrDmacwPtr) for every HW-ring into ASIC    +*/
/*+               --> setup for HWFiltercode & RX-rings must be finished    +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_NRTInitRcvCurrDmacwPtrs(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_DDB_NRT_PTR_TYPE pNRTIface,
    LSA_UINT32                  RingMask)
{
    EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE  pNrtApiCtrlIfaceMgm;    // memory with the RcvDescrTable
    PNIP_LOCAL_DMACW_RCV_DESCR_PTR_TYPE         pRcvDescrTable;         // points to RcvDescrTable[32]
    PNIP_LOCAL_DMACW_RCV_ITEM_PTR_TYPE          pFirstRcvDmacw;         // first DMACW-item in the ring
    LSA_UINT32                                  Idx, AsicAddr, Value;

    pNrtApiCtrlIfaceMgm = (EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE)pNRTIface->pNrtApiCtrlIfaceMgm;
    EDDP_ASSERT_NO_NULL_PTR(pNrtApiCtrlIfaceMgm);
    pRcvDescrTable      = &(pNrtApiCtrlIfaceMgm->ApiCtrlRamRcvDmacw[0]);

    // ***** step through all rings and write CurrDMACWPtr (entry point) into ASIC if ring active configured
    for ( Idx=0; Idx < EDDP_NRT_RX_DMACW_CNT; Idx++)
    {
        if ( (1<<Idx) & RingMask )
        {
            // ***** ring selected by configuration
            pFirstRcvDmacw = pNRTIface->RxHwDescrRingMgm[Idx].pRxRing;

            if (pFirstRcvDmacw != LSA_NULL)
            {
                // ***** ring is active configured --> give ASIC the entry point for this ring (in Dev-MEM)
                EDDP_DEV_MEM_ADDR_UPPER_TO_PNIP(pDDB->hSysDev, pFirstRcvDmacw, &AsicAddr);
                EDDP_HAL_MEM32_WRITE(pDDB, (EDD_COMMON_MEM_U32_PTR_TYPE)&pRcvDescrTable[Idx].CurrentDmacwHwPtr, AsicAddr);

                Value = PNIP_DMACW_RCV_DESCR_TS_ON;

                switch (pDDB->EDDPLocation)
                {
                    case EDDP_LOCATION_EXT_HOST:
                        Value |= PNIP_DMACW_RCV_DESCR_INT_ICU3;
                    break;

                    case EDDP_LOCATION_LOCAL:	// ADAPTION_ARM_ICU
                        // use ICU2
                        Value |= PNIP_DMACW_RCV_DESCR_INT_ICU2;
                    break;

                    default:
                        EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] _EDDP_NRTInitRcvCurrDmacwPtrs(): ERROR -> EDDPLocation(%d)!",
                            pDDB->EDDPLocation);
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
                        return (EDD_STS_ERR_PARAM);
                    break;
                }

                EDDP_HAL_MEM32_WRITE(pDDB, &pRcvDescrTable[Idx].RcvDropCntTsErrIntVal, Value);
            }
        }
    }

    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_RCVENABLED_IFA, RingMask);

    if (pDDB->NRT.NrtIFCfg.RxDropLogging)
    {
        // ***** enable LOG that DROP works like PASS
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_LOGCONTROL_IFA, EDDP_HWF_ALL_CODE_MASK);
    }

    return (EDD_STS_OK);
}



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_NRTInitSndCurrDmacwPtrs               +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_DDB_NRT_PTR_TYPE pNRTIface       +*/
/*+                             LSA_UINT32                  RingMask        +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB         : Pointer to DeviceDescriptionBlock                       +*/
/*+  pNRTIface    : Pointer to internal Mgm structure                       +*/
/*+  RingMask     : selector for RX-Ring (bits 0..31)                       +*/
/*+                                                                         +*/
/*+  Result       : EDD_STS_OK                                              +*/
/*+                 EDD_STS_ERR_PARAM                                       +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: utility function for _EDDP_NRTInitInterface()             +*/
/*+               write entry (CurrDmacwPtr) for every HW-ring into ASIC    +*/
/*+               --> setup for TX-rings must be finished                   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_NRTInitSndCurrDmacwPtrs(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_DDB_NRT_PTR_TYPE pNRTIface,
    LSA_UINT32                  RingMask)
{
    EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE  pNrtApiCtrlIfaceMgm;    // memory with the RcvDescrTable
    PNIP_LOCAL_DMACW_SND_DESCR_PTR_TYPE         pSndDescrTable;         // points to SndDescrTable[16]
    PNIP_LOCAL_DMACW_SND_ITEM_PTR_TYPE          pFirstSndDmacw;         // first DMACW-item in the ring
    LSA_UINT32                                  Idx, AsicAddr, Value;

    pNrtApiCtrlIfaceMgm = (EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE)pNRTIface->pNrtApiCtrlIfaceMgm;
    EDDP_ASSERT_NO_NULL_PTR(pNrtApiCtrlIfaceMgm);
    pSndDescrTable      = &(pNrtApiCtrlIfaceMgm->ApiCtrlRamSndDmacw[0]);

    // ***** step through all rings and write CurrDMACWPtr (entry point) into ASIC if ring active configured
    for ( Idx=0; Idx < EDDP_NRT_TX_DMACW_CNT; Idx++)
    {
        if ( (1<<Idx) & RingMask )
        {
            // ***** ring selected by configuration
            pFirstSndDmacw = pNRTIface->TxHwDescrRingMgm[Idx].pTxRing;

            if (pFirstSndDmacw != LSA_NULL)
            {
                // ***** ring is active configured --> give ASIC the entry point for this ring (in Dev-MEM)
                EDDP_DEV_MEM_ADDR_UPPER_TO_PNIP(pDDB->hSysDev, pFirstSndDmacw, &AsicAddr);
                EDDP_HAL_MEM32_WRITE(pDDB, &pSndDescrTable[Idx].CurrentDmacwHwPtr, AsicAddr);

                // init: WDEn and INT
                Value = PNIP_DMACW_SND_DESCR_WDT_MSK | PNIP_DMACW_SND_DESCR_WDEN_ON;    // set WDT to FFFF to see in HW has used this ring anytime

                switch (pDDB->EDDPLocation)
                {
                    case EDDP_LOCATION_EXT_HOST:
                        Value |= PNIP_DMACW_SND_DESCR_INT_ICU3;
                    break;

                    case EDDP_LOCATION_LOCAL:
                        Value |= PNIP_DMACW_SND_DESCR_INT_ICU2;     // use ICU2 --> ADAPTION_ARM_ICU
                    break;

                    default:
                        EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] _EDDP_NRTInitSndCurrDmacwPtrs(): ERROR -> EDDPLocation(%d)!",
                            pDDB->EDDPLocation);
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
                        return (EDD_STS_ERR_PARAM);
                    break;
                }

                EDDP_HAL_MEM32_WRITE(pDDB, &pSndDescrTable[Idx].SndWdtErrIntVal, Value);
            }
        }
    }

    return (EDD_STS_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_NRTInitInterface                      +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_IPB_NRT_PTR_TYPE       pIPBNrt         +*/
/*+                             EDD_RSP                     Response        +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB         : Pointer to DeviceDescriptionBlock                       +*/
/*+  pIPBNrt      : Pointer to parameter structure                          +*/
/*+  Response     : setup status in EDDP_NRTDeviceOpen()                    +*/
/*+                                                                         +*/
/*+  Result       : EDD_STS_OK                                              +*/
/*+                 EDD_STS_ERR_PARAM                                       +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: utility function for EDDP_NRTDeviceOpen()                 +*/
/*+               init HWFiltercode, build filter map, init empty           +*/
/*+               TX-RQB-queues, init valid RX-rings                        +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_NRTInitInterface(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_DPB_NRT_PTR_TYPE       pIPBNrt,
    EDD_RSP                     Response)
{
    EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE  pNrtApiCtrlIfaceMgm;
    EDDP_LOCAL_DDB_NRT_PTR_TYPE                 pNRTIface;
    LSA_UINT32                                  Idx, RegSndTable=0, RegRcvTable=0, RegSdTable=0, RegStoreTable=0, RegSndWdLimit = 0, AsicAddr, HostAddr;
    EDD_COMMON_MEM_U32_PTR_TYPE                 pCommon32;

    if (Response == EDD_STS_OK)
    {
        // EDDP_ASSERT_NO_NULL_PTR(pDDB);
        pNRTIface                   = &pDDB->NRT;
        pNRTIface->FilterTLVCnt     = 0;                        // no TLV's used
        pNRTIface->FilterTLVHandle  = EDDP_CFG_MAX_CHANNELS;    // set handle invalid

        // EDDP_IF_A
        /* There is a macro that checks                                             */
        /* PNIP_R2_REG_SNDTABLEBASE_IFA    and PNIP_R3_REG_SNDTABLEBASE_IFA         */
        /* PNIP_R2_REG_RCVTABLEBASE_IFA    and PNIP_R3_REG_RCVTABLEBASE_IFA         */
        /* PNIP_R2_REG_SDTABLEBASE_IFA     and PNIP_R3_REG_SDTABLEBASE_IFA          */
        /* PNIP_R2_REG_STORETABLEBASE_IFA  and PNIP_R3_REG_STORETABLEBASE_IFA       */
        /* has the same value.                                                      */
        /* Checked in eddp_hal_pnip_val.h. We use the R2 version.                   */
        RegSndTable     = PNIP_R2_REG_SNDTABLEBASE_IFA;
        RegRcvTable     = PNIP_R2_REG_RCVTABLEBASE_IFA;
        RegSdTable      = PNIP_R2_REG_SDTABLEBASE_IFA;
        RegStoreTable   = PNIP_R2_REG_STORETABLEBASE_IFA;
        RegSndWdLimit   = PNIP_REG_SNDWATCHDOGLIMIT_IFA;

        switch(EDDP_HALGetPnipRevision(pDDB))
        {
            #if ((defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT)) || (defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)))
            case EDDP_HAL_HW_PNIP_REV1:
            case EDDP_HAL_HW_PNIP_REV2:
            {
                pNRTIface->pNrtApiCtrlCommonCode            = (EDD_COMMON_MEM_U32_PTR_TYPE)EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_API_CTRL, EDDP_HAL_RAMSUB_APICTRL_NRT_COMMON_CODE,          LSA_NULL);
                pNRTIface->pNrtApiCtrlIfaceMgm              = (EDD_COMMON_MEM_U32_PTR_TYPE)EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_API_CTRL, EDDP_HAL_RAMSUB_APICTRL_NRT_IFA_MGM,              LSA_NULL);
                pNRTIface->pNrtApiCtrlIfaceDcpFilter        = (EDD_COMMON_MEM_U32_PTR_TYPE)EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_API_CTRL, EDDP_HAL_RAMSUB_APICTRL_NRT_IFA_DCPPORT_FILTER,   LSA_NULL);
                pNRTIface->pNrtApiCtrlIfaceDcpHelloFilter   = (EDD_COMMON_MEM_U32_PTR_TYPE)EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_API_CTRL, EDDP_HAL_RAMSUB_APICTRL_NRT_IFA_DCPHELLO_FILTER,  LSA_NULL);
            }
            break;
            #endif

            default:
            {
                EDDP_NRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] _EDDP_NRTInitInterface(): ERROR -> Hardware not supported!");
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            }
            break;
        }
        // ***** load filtercode into ASIC and set FilterFragmentRegister
        pCommon32 = pNRTIface->pNrtApiCtrlCommonCode;
        EDDP_ASSERT_NO_NULL_PTR(pCommon32);
        EDDP_HWCInitFilterCode(pDDB, (LSA_UINT32)pCommon32);

        _EDDP_NRTInitSndRingsEmpty(pDDB, EDDP_NRT_TX_DMACW_MASK);   // use lower 4 DMACW rings
        _EDDP_NRTInitRcvRingsEmpty(pDDB, EDDP_ARD_HWF_USED_RINGS);  // NRT + KRISC

        // ***** reset statistic/error registers
        if (Response == EDD_STS_OK)
        {
            Response = EDDP_HWCSetRegisterToDefaults (pDDB);
        }

        // ***** init SDT with disabled entries/strings (missmatch and zero length)
        if (Response == EDD_STS_OK)
        {
            pCommon32 = pNRTIface->pNrtApiCtrlIfaceMgm;
            EDDP_ASSERT_NO_NULL_PTR(pCommon32);
            HostAddr = (LSA_UINT32)(((EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE)pCommon32)->ApiCtrlRamSdt);
            Response = EDDP_HWCInitDisabledSdtEntries(pDDB, HostAddr);
            EDDP_HAL_REG32_WRITE(pDDB, RegSdTable, EDDP_HAL_H2P_ADDR(pDDB, HostAddr));      // Base for StringDescr.Table, 64 entries a 4Byte
            HostAddr = (LSA_UINT32)(((EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE)pCommon32)->ApiCtrlRamStore);
            Response = EDDP_HWC_InitStoreEntries(pDDB, HostAddr);
            EDDP_HAL_REG32_WRITE(pDDB, RegStoreTable, EDDP_HAL_H2P_ADDR(pDDB, HostAddr));   // Base for StoreTable, 4 entries a 2Byte
        }

        // ***** Setup TX/RX MGM for all interfaces & init empty Tx/Rx-Rings
        if (Response == EDD_STS_OK)
        {
            // ***** currently no handles open
            for ( Idx=0; Idx < EDDP_NRT_MAX_FDC_CNT; Idx++)
            {
                pNRTIface->FilterInfo[Idx].pNrtHDB = LSA_NULL;
            }

            pNRTIface->HandleCnt = 0;

            // ***** init empty TX-queues and setup limits for sending
            for ( Idx=0; Idx < EDDP_NRT_TX_GRP_CNT; Idx++)
            {
                pNRTIface->TxQueue[Idx].InProgCntMax = 1;
                EDDP_GlbRQBQueueInit (&pNRTIface->TxQueue[Idx].TxReqQueue);
            }

            // ***** init valid TX-rings
            if (Response == EDD_STS_OK)
            {
                Response = _EDDP_NRTInitSndRings (pDDB, pIPBNrt, EDDP_NRT_TX_DMACW_MASK);      // use 6 DMACW rings
            }
            if (Response == EDD_STS_OK)
            {
                Response = _EDDP_NRTInitSndCurrDmacwPtrs (pDDB, pNRTIface, EDDP_NRT_TX_DMACW_MASK);
            }

            // ***** init valid RX-rings
            if (Response == EDD_STS_OK)
            {
                Response = _EDDP_NRTInitRcvRings (pDDB, pIPBNrt, EDDP_ARD_NRT_USED_RINGS);  // all supported NRT rings
            }
            if (Response == EDD_STS_OK)
            {
                Response = _EDDP_NRTInitRcvCurrDmacwPtrs (pDDB, pNRTIface, pNRTIface->UsedRxRingMask & EDDP_ARD_NRT_USED_RINGS);
            }

            // ***** set Snd/RcvTableBase
            if (Response == EDD_STS_OK)
            {
                // ***** map addr from Host (ApiCtrlRAM) into ASIC view
                pNrtApiCtrlIfaceMgm = (EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE)(pNRTIface->pNrtApiCtrlIfaceMgm);
                AsicAddr            = EDDP_HAL_H2P_ADDR(pDDB, &pNrtApiCtrlIfaceMgm->ApiCtrlRamSndDmacw[0]);
                EDDP_HAL_REG32_WRITE(pDDB, RegSndTable, AsicAddr);

                AsicAddr = EDDP_HAL_H2P_ADDR(pDDB, &pNrtApiCtrlIfaceMgm->ApiCtrlRamRcvDmacw[0]);
                EDDP_HAL_REG32_WRITE(pDDB, RegRcvTable, AsicAddr);
                pNRTIface->IfaceStatus = EDDP_NRT_IF_INITDONE;
                // ***** HWF-setup complete but HW-Filter needs to be enable
                EDDP_HAL_REG32_WRITE(pDDB, RegSndWdLimit, pIPBNrt->TxSndTimeout);

            }
        }
    }

    return (Response);
}



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_NRTDeInitInterface                    +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB         : Pointer to DeviceDescriptionBlock                       +*/
/*+                                                                         +*/
/*+  Result       : EDD_STS_OK                                              +*/
/*+                 EDD_STS_ERR_PARAM                                       +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: utility function for EDDP_NRTDeviceClose()                +*/
/*+               deinit used TX-rings, deinit used RX-rings and its        +*/
/*+               RX-buffers                                                +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_NRTDeInitInterface(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDDP_LOCAL_DDB_NRT_PTR_TYPE     pNRTIface;
    EDD_RSP                         Response;

    // EDDP_ASSERT_NO_NULL_PTR(pDDB);
    pNRTIface = &pDDB->NRT;

    // ***** deinit used TX-rings --> keep first detected error code
    Response = _EDDP_NRTDeInitSndRings (pDDB, EDDP_NRT_TX_DMACW_MASK);  //  use 6 DMACW rings in NRT

    // ***** deinit used RX-rings/Buffers --> keep first detected error code
    if (Response == EDD_STS_OK)
    {
        Response = _EDDP_NRTDeInitRcvRings (pDDB, EDDP_ARD_NRT_USED_RINGS);
    }
    else
    {
        _EDDP_NRTDeInitRcvRings (pDDB, EDDP_ARD_NRT_USED_RINGS);
    }

    pNRTIface->IfaceStatus = EDDP_NRT_IF_PREINIT;

    return (Response);
}






/*===========================================================================*/
/*                        public  functions                                  */
/*===========================================================================*/




/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_NRTDeviceOpen                          +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             EDDP_UPPER_DPB_PTR_TYPE        pDPB         +*/
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
/*+               Initializes all NRT management structures within DDB      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_NRTDeviceOpen(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    EDDP_UPPER_DPB_PTR_TYPE            pDPB)
{
    LSA_UINT32                      i;
    EDDP_LOCAL_DDB_NRT_PTR_TYPE     pNRTIface;
    EDD_RSP                         Response = EDD_STS_OK;

    EDDP_ASSERT_NO_NULL_PTR(pDDB);
    EDDP_ASSERT_NO_NULL_PTR(pDPB);

    EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_NRTDeviceOpen(pDDB=0x%X, pDPB=0x%X)", 
        pDDB, pDPB);

    _EDDP_NRTInitMgmZero(pDDB);   // sets pNRTIface->DcpHelloFilterCnt !!!
    pNRTIface = &pDDB->NRT;
    // alloc Mgmt-data in Heap
    pNRTIface->pDCPHelloFilter = (EDDP_NRT_DCP_HELLO_FILTER_MGM_PTR) EDDP_GlbAllocLocalMem ((pNRTIface->DcpHelloFilterCnt) * sizeof(EDDP_NRT_DCP_HELLO_FILTER_MGM_TYPE));

    if (LSA_HOST_PTR_ARE_EQUAL(pNRTIface->pDCPHelloFilter, LSA_NULL))
    {
        Response = EDD_STS_ERR_RESOURCE;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_NRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_NRTDeviceOpen(): ERROR -> Alloc pDCPHelloFilter!");
    }
    else
    {
        for ( i=0; i < pNRTIface->DcpHelloFilterCnt; i++ )
        {   // ***** minimum initialization to "not used"
            pNRTIface->pDCPHelloFilter[i].InUse = LSA_FALSE;
        }
    }

    pDDB->NRT.NrtIFCfg = pDPB->NRT;      // copy CFG settings

    if (Response != EDD_STS_OK)
    {
        // ***** error on alloc --> free memory again
        pNRTIface = &pDDB->NRT;

        if (!LSA_HOST_PTR_ARE_EQUAL(pNRTIface->pDCPHelloFilter, LSA_NULL))
        {
            EDDP_GlbFreeLocalMem (pNRTIface->pDCPHelloFilter);  // free Mgmt-data in Heap
        }
    }
    else
    {
        // ***** ok
        pDDB->NRT.CfgFilled = LSA_TRUE;
        // configure NRT/ASIC later in EDDP_NRTDeviceSetup()
    }

    EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_NRTDeviceOpen(): Response=0x%X", Response);

    if( EDD_STS_OK == Response)
    {
        /* IO_Configured must be set with FILL = Active */
        if (   (EDD_SYS_IO_CONFIGURED_ON == pDPB->NRT.IO_Configured)
            && (EDD_SYS_FILL_ACTIVE != pDPB->NRT.FeedInLoadLimitationActive) )
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_NRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_NRTDeviceOpen(): ERROR -> IO_Configured=True && FeedInLoadLimitation=False is not allowed!");
        }
        else
        {
            /* init fill values */
            pDDB->NRT.FILL.FeedInLoadLimitationActiveStartup = (EDD_SYS_FILL_ACTIVE == pDPB->NRT.FeedInLoadLimitationActive)?LSA_TRUE:LSA_FALSE;
            pDDB->NRT.FILL.FeedInLoadLimitationActive = pDDB->NRT.FILL.FeedInLoadLimitationActiveStartup;
            /* IO_Configured is undefined but deactivated fill means no provider or consumer are allowed */
            pDDB->NRT.FILL.IO_ConfiguredStartup = (EDD_SYS_IO_CONFIGURED_ON == pDPB->NRT.IO_Configured)?LSA_TRUE:LSA_FALSE;
            pDDB->NRT.FILL.IO_Configured = pDDB->NRT.FILL.IO_ConfiguredStartup;
        }
    }

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_NRTDeviceSetup                         +*/
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
/*+               Setup Device for NRT                                      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_NRTDeviceSetup(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    EDDP_UPPER_DSB_PTR_TYPE            pDSB)
{
    EDD_RSP                 Response = EDD_STS_OK;

    EDDP_ASSERT_NO_NULL_PTR(pDDB);
    EDDP_ASSERT_NO_NULL_PTR(pDSB);

    EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_NRTDeviceSetup(pDDB=0x%X, pDSB=0x%X)", 
        pDDB, pDSB);

    if (pDDB->NRT.CfgFilled)
    {
        // ***** do basic initialization for interface
        Response = _EDDP_NRTInitInterface(pDDB, &pDDB->NRT.NrtIFCfg, Response);  // set interface status to EDDP_NRT_IF_INITDONE on success

        if (Response != EDD_STS_OK)
        {
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_NRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_NRTDeviceSetup(): ERROR -> _EDDP_NRTInitInterface()!");

            // ***** do de-initialization on error
            _EDDP_NRTDeInitInterface (pDDB);
        }
    }
    else
    {
        Response = EDD_STS_ERR_SEQUENCE;
    }

    EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_NRTDeviceSetup(): Response=0x%X", Response);

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_NRTDeviceIFSetup                       +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE           pDDB      +*/
/*+                             EDDP_UPPER_DSB_PTR_TYPE           pDSB      +*/
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
/*+               Setup Interface or NRT                                    +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_NRTDeviceIFSetup(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    EDDP_UPPER_DSB_PTR_TYPE            pDSB)
{
    EDDP_LOCAL_DDB_NRT_PTR_TYPE pNRTIface;
    EDD_RSP                     Response = EDD_STS_OK;

    // EDDP_ASSERT_NO_NULL_PTR(pDDB);
    pNRTIface = &pDDB->NRT;

#if (EDDP_CFG_TRACE_MODE ==  0)
    LSA_UNUSED_ARG(pDSB);
#endif

    EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_NRTDeviceIFSetup(pDDB=0x%X, pDSB=0x%X)",
        pDDB, pDSB);

    EDDP_ASSERT_NO_NULL_PTR(pNRTIface);

    // ***** check index if TX rings: DEFAULT(0), TCP(1), UDP(2), MISC(3), DCP(4), ALARM(5), MRP(6), LLDP(7), SYNC(8), KRISC(14)
#if EDDP_NRT_TX_GRP_DEFAULT != 0
    #error "EDDP_NRTDeviceIFSetup(): index for EDDP_NRT_TX_GRP_DEFAULT != 0"
#endif
#if EDDP_NRT_TX_GRP_TCP != 1
    #error "EDDP_NRTDeviceIFSetup(): index for EDDP_NRT_TX_GRP_TCP != 1"
#endif
#if EDDP_NRT_TX_GRP_UDP != 2
    #error "EDDP_NRTDeviceIFSetup(): index for EDDP_NRT_TX_GRP_UDP != 2"
#endif
#if EDDP_NRT_TX_GRP_IP_MISC != 3
    #error "EDDP_NRTDeviceIFSetup(): index for EDDP_NRT_TX_GRP_IP_MISC != 3"
#endif
#if EDDP_NRT_TX_GRP_DCP != 4
    #error "EDDP_NRTDeviceIFSetup(): index for EDDP_NRT_TX_GRP_DCP != 4"
#endif
#if EDDP_NRT_TX_GRP_ALARM != 5
    #error "EDDP_NRTDeviceIFSetup(): index for EDDP_NRT_TX_GRP_ALARM != 5"
#endif
#if EDDP_NRT_TX_GRP_MRP != 6
    #error "EDDP_NRTDeviceIFSetup(): index for EDDP_NRT_TX_GRP_MRP != 6"
#endif
#if EDDP_NRT_TX_GRP_LLDP != 7
    #error "EDDP_NRTDeviceIFSetup(): index for EDDP_NRT_TX_GRP_LLDP != 7"
#endif
#if EDDP_NRT_TX_GRP_SYNC != 8
    #error "EDDP_NRTDeviceIFSetup(): index for EDDP_NRT_TX_GRP_SYNC != 8"
#endif
#if EDDP_K32_TX_DMACW_MIN != 14
    #error "EDDP_NRTDeviceIFSetup(): index for EDDP_K32_TX_DMACW_MIN != 14"
#endif

    if ( pNRTIface->IfaceStatus == EDDP_NRT_IF_INITDONE )
    {
#if (EDDP_NRT_RXTX_COUNTER_SUPPORT == 1)
        EDDP_MEMSET_COMMON(&pNRTIface->Stats, 0, sizeof(EDDP_NRT_STAT_TYPE));    // clear rx/tx statistic
#endif

        /* init feed in load limitation */
        EDDP_NRTFillSetup(pDDB,LSA_TRUE,
                    pDDB->NRT.FILL.FeedInLoadLimitationActiveStartup,
                    pDDB->NRT.FILL.FeedInLoadLimitationActiveStartup);/* default value for IO_Configured is LoadLimitationActive*/
        switch(EDDP_HALGetPnipRevision(pDDB))
        {
#if defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT) || defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
            case EDDP_HAL_HW_PNIP_REV1:
            case EDDP_HAL_HW_PNIP_REV2:
            {
                EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_FILTERENABLED_IFA, EDDP_HWF_ALL_CODE_MASK);
            }
            break;
                
#endif
            default:
            {
                
            }
            break;
        }

        if (pNRTIface != LSA_NULL)
        {
            pNRTIface->IfaceStatus = EDDP_NRT_IF_ACTIVE;    // setup complete and HW-Filter enabled
        }
    }
    else
    {
        Response = EDD_STS_ERR_SEQUENCE;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_NRTDeviceIFSetup(): ERROR -> expect other interface status(%d)!!!",
            pNRTIface->IfaceStatus);
    }

    EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_NRTDeviceIFSetup(): Response=0x%X", Response);

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_NRTDeviceIFShutdown                    +*/
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
/*+               Shutdown Interface of NRT                                 +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_NRTDeviceIFShutdown(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDDP_LOCAL_DDB_NRT_PTR_TYPE     pNRTIface;
    EDDP_CMD_PARAM_TYPE             StopCmd;
    EDD_RSP                         Response;

    EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_NRTDeviceIFShutdown(pDDB=0x%X)", pDDB);

    // EDDP_ASSERT_NO_NULL_PTR(pDDB);
    pNRTIface = &pDDB->NRT;

    if (pNRTIface->IfaceStatus == EDDP_NRT_IF_ACTIVE)
    {
        StopCmd.param_union.Filter.StopFilter.StopFilterMask    = EDDP_ARD_NRT_USED_RINGS;
        StopCmd.CmdOpcode                                       = EDDP_CMD_FILTER_STOP;
        StopCmd.InUse                                           = LSA_FALSE;
        StopCmd.Cbf                                             = 0;

        Response = EDDP_CmdRequest(pDDB, &StopCmd);

        if (Response == EDD_STS_OK)
        {
            pNRTIface->IfaceStatus = EDDP_NRT_IF_INITDONE;
        }
    }
    else
    {
        Response = EDD_STS_ERR_SEQUENCE;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_NRTDeviceIFShutdown(): ERROR -> expect other interface status(%d)!!!",
            pNRTIface->IfaceStatus);
    }

    EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_NRTDeviceIFShutdown(): Response=0x%X", Response);

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_NRTDeviceShutdown                      +*/
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
/*+               Shutdown Interface of NRT                                 +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_NRTDeviceShutdown(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDDP_LOCAL_DDB_NRT_PTR_TYPE     pNRTIface;
//  EDDP_CMD_PARAM_TYPE             StopCmd;
    EDD_RSP                         Response = EDD_STS_OK;

    EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_NRTDeviceShutdown(pDDB=0x%X)", pDDB);

    pNRTIface = &pDDB->NRT;

    if (pNRTIface->IfaceStatus > EDDP_NRT_IF_INITDONE)
    {
        Response = EDD_STS_ERR_SEQUENCE;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_NRTDeviceShutdown(): ERROR -> expect other interface IfaceStatus(%d)!!!",
            pNRTIface->IfaceStatus);
    }

    EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_NRTDeviceShutdown(): Response=0x%X", Response);

    return (Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_NRTDeviceClose                         +*/
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
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDDP_SRV_DEV_CLOSE or EDDP_SRV_DEV_OPEN (only if          +*/
/*+               previous OPEN shall be aborted)                           +*/
/*+               Close device for NRT                                      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_NRTDeviceClose(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP                         Response = EDD_STS_OK;
    EDDP_LOCAL_DDB_NRT_PTR_TYPE     pNRTIface;

    EDDP_ASSERT_NO_NULL_PTR(pDDB);

    EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_NRTDeviceClose(pDDB=0x%X)", pDDB);

    // ***** do de-initialization
    pNRTIface = &pDDB->NRT;

    if (!LSA_HOST_PTR_ARE_EQUAL(pNRTIface->pDCPHelloFilter, LSA_NULL))
    {
        // ***** free alloced memory for DcpHelloMgmArray
        EDDP_GlbFreeLocalMem (pNRTIface->pDCPHelloFilter);  // free Mgmt-data in Heap
    }

    if (Response != EDD_STS_OK)
    {
        // ***** set interface status to EDDP_NRT_IF_PREINIT
        _EDDP_NRTDeInitInterface(pDDB);
    }
    else
    {
        // ***** set interface status to EDDP_NRT_IF_PREINIT
        Response = _EDDP_NRTDeInitInterface(pDDB);

        if (Response != EDD_STS_OK)
        {
            EDDP_NRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "EDDP_NRTDeviceClose(): ERROR -> _EDDP_NRTDeInitInterface()!");
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        }
    }

#ifdef EDDP_NRT_MEMSET_CHK
    if (Response == EDD_STS_OK)
    {
        switch(EDDP_HALGetPnipRevision(pDDB))
        {
            #if ((defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT)) || (defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)))
            case EDDP_HAL_HW_PNIP_REV1:
            case EDDP_HAL_HW_PNIP_REV2:
            {
                EDDP_HALMemset(pDDB, EDDP_HAL_RAMTYPE_API_CTRL, EDDP_HAL_RAMSUB_APICTRL_NRT_COMMON_CODE,        0, 0xFFFFFFFF, 0x11);
                EDDP_HALMemset(pDDB, EDDP_HAL_RAMTYPE_API_CTRL, EDDP_HAL_RAMSUB_APICTRL_NRT_IFA_MGM,            0, 0xFFFFFFFF, 0x22);
                EDDP_HALMemset(pDDB, EDDP_HAL_RAMTYPE_API_CTRL, EDDP_HAL_RAMSUB_APICTRL_NRT_IFA_DCPPORT_FILTER, 0, 0xFFFFFFFF, 0x33);
                EDDP_HALMemset(pDDB, EDDP_HAL_RAMTYPE_API_CTRL, EDDP_HAL_RAMSUB_APICTRL_NRT_IFA_DCPHELLO_FILTER,0, 0xFFFFFFFF, 0x44);
            }
            break;
            #endif
            default:
            {
                EDDP_NRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_NRTDeviceClose(): ERROR -> Hardware not supported!");
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            }
            break;

        }
    }
#endif

    EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_NRTDeviceClose(): Response=0x%X", Response);

    return (Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_NRTOpenChannel                         +*/
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
/*+  Description: Opens Channel for NRT. The Handle will specify the Channel+*/
/*+               and pDDB the Device. pCDB has all parameters              +*/
/*+               for this channel                                          +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_NRTOpenChannel(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE         pHDB,
    LSA_HANDLE_TYPE                 Handle,
    EDDP_SYSTEM_CDB_PTR_TYPE        pCDB)
{
    LSA_UINT32                      Idx;
    EDD_RSP                         Response;
    EDDP_LOCAL_DDB_NRT_PTR_TYPE     pNRTIface;
    EDDP_LOCAL_HDB_NRT_PTR_TYPE     pHDB_NRT_PoolItem;

    EDDP_NRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_NRTOpenChannel()");

    EDDP_NRT_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:%2X] EDDP_NRTOpenChannel(pDDB=0x%X, pHDB=0x%X, Handle=0x%X, pCDB=0x%X)",
        pHDB->Handle, pDDB, pHDB, Handle, pCDB);

    /* ----------------------------------------------------------------------*/
    /* If HandleCnt too high return with error                               */
    /* ----------------------------------------------------------------------*/
    if (Handle >= EDDP_CFG_MAX_CHANNELS)
    {
        EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_NRTOpenChannel(): ERROR -> Handle(%d) >= Max(%d)!",
            Handle, EDDP_CFG_MAX_CHANNELS);
        EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
        return(EDD_STS_ERR_PARAM);
    }

    /* ----------------------------------------------------------------------*/
    /* If pNRT's not valid return with error                                 */
    /* ----------------------------------------------------------------------*/
    if (LSA_HOST_PTR_ARE_EQUAL(pCDB->pNRT, LSA_NULL))
    {
        EDDP_NRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] ERROR -> EDDP_NRTOpenChannel(): pCDB->pNRT is NULL!");
        EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
        return(EDD_STS_ERR_PARAM);
    }

    Response            = EDD_STS_OK;
    pNRTIface           = &pDDB->NRT;
    pHDB_NRT_PoolItem   = LSA_NULL;

    /* ----------------------------------------------------------------------*/
    /* check if all selected filters are free if one/more RX filter selected */
    /* ----------------------------------------------------------------------*/
    if (pCDB->pNRT->FrameFilter)
    {
        if (   (pCDB->pNRT->FrameFilter & EDD_NRT_FRAME_DLR)
            && (pDDB->FeatureSupport.DLRExtensions != EDD_FEATURE_ENABLE))
        {
            Response = EDD_STS_ERR_NOT_SUPPORTED;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] ERROR -> EDDP_NRTOpenChannel(filter:%8Xh): filter[%d] selected but DLRExtensions not ENABLED!",
                pCDB->pNRT->FrameFilter, EDDP_NRT_FILTER_DLR_IDX);
        }
        else if (   (pCDB->pNRT->FrameFilter & EDD_NRT_FRAME_MRP)
                 && (pDDB->FeatureSupport.DLRExtensions == EDD_FEATURE_ENABLE))
        {
            Response = EDD_STS_ERR_NOT_SUPPORTED;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] ERROR -> EDDP_NRTOpenChannel(filter:%8Xh): filter[%d] not supported with DLRExtensions ENABLED!",
                pCDB->pNRT->FrameFilter, EDDP_NRT_FILTER_MRP_IDX);
        }
        
        if (Response == EDD_STS_OK)
        {
            for ( Idx=0; Idx < EDDP_NRT_MAX_FDC_CNT; Idx++ )
            {
                if (pCDB->pNRT->FrameFilter & pNRTIface->FilterInfo[Idx].UsrFrameFilter)
                {
                    if (pNRTIface->FilterInfo[Idx].pNrtHDB)
                    {
                        Response = EDD_STS_ERR_CHANNEL_USE;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] ERROR -> EDDP_NRTOpenChannel(filter:%8Xh): filter[%d] used!",
                            pCDB->pNRT->FrameFilter, Idx);
                        break;
                    }
                }
            }
        }
    }

    // ***** alloc handle management-memory
    if (Response == EDD_STS_OK)
    {
        #ifdef EDDP_CFG_LOCAL_MEM_STATIC
        pHDB_NRT_PoolItem = &g_EDDP_HDB_NRT[Handle];    // use static RAM
        #else
        pHDB_NRT_PoolItem = (EDDP_LOCAL_HDB_NRT_PTR_TYPE) EDDP_GlbAllocLocalMem (sizeof(EDDP_HDB_NRT_TYPE));    // alloc Mgmt-data in Heap
        if (LSA_HOST_PTR_ARE_EQUAL(pHDB_NRT_PoolItem, LSA_NULL))
        {
            EDDP_NRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] ERROR -> EDDP_NRTOpenChannel(): error alloc pHDB_NRT_PoolItem!");
            EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_RESOURCE);
            Response = EDD_STS_ERR_RESOURCE;
        }
        #endif
        pHDB->pNRT = pHDB_NRT_PoolItem; // assign NRT managment for this channel
    }

    // ***** init handle management for this channel
    if (Response == EDD_STS_OK)
    {
        pHDB_NRT_PoolItem->UsrFrameFilter   = pCDB->pNRT->FrameFilter;  // user filter for this channel
        EDDP_GlbRQBQueueInit(&pHDB_NRT_PoolItem->RxUsrQueue);           // no RQBs for this channel in RX-queue
        pHDB_NRT_PoolItem->TxQueueReqCnt    = 0;                        // no RQBs for this channel in TX-queue(s)
        pHDB_NRT_PoolItem->TxInProgCnt      = 0;                        // no TX requests for this channel in progress
        pHDB_NRT_PoolItem->CancelInProgress = LSA_FALSE;                // no cancel
        EDDP_GlbRQBQueueInit(&pHDB_NRT_PoolItem->TxCancelQueue);        // set cancel-queue empty
        pHDB_NRT_PoolItem->pUserCancelRQB   = LSA_NULL;                 // no cancel in progress --> no RQB
        pHDB_NRT_PoolItem->pNrtDDB          = pNRTIface;                // reference to global NRT mgm
//      pHDB_NRT_PoolItem->Handle           = Handle;                   // index open channel
        pHDB_NRT_PoolItem->EDD_Handle       = pHDB;                     // handle open channel
        pNRTIface->HandleCnt++;                                         // cnt of currently open handles

        // ***** assign HW-RX-Queues(Filter) with channel mgm if RX-filter selected
        if (pHDB_NRT_PoolItem->UsrFrameFilter)
        {
            for ( Idx=0; Idx < EDDP_NRT_MAX_FDC_CNT; Idx++ )
            {
                if (pHDB_NRT_PoolItem->UsrFrameFilter & pNRTIface->FilterInfo[Idx].UsrFrameFilter)
                {
                    pNRTIface->FilterInfo[Idx].pNrtHDB = pHDB_NRT_PoolItem; // reference to this userchannel
                }
            }
        }
    }

    // ***** release memory on error
    if (Response != EDD_STS_OK)
    {
        #ifndef EDDP_CFG_LOCAL_MEM_STATIC
        if ( pHDB_NRT_PoolItem != LSA_NULL )
        {
            EDDP_GlbFreeLocalMem(pHDB_NRT_PoolItem);
        }
        #endif
        pHDB->pNRT = LSA_NULL;
    }

    EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_NRTOpenChannel(): Response=0x%X", Response);

    return (Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_NRTRequest                             +*/
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
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:     Depend on service               +*/
/*+                                                                         +*/
/*+  RQB-return values via callback-function depends on service             +*/
/*+                                                                         +*/
/*+  Results    : LSA_VOID                                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function handles the following services for NRT:     +*/
/*+                                                                         +*/
/*+               EDD_SRV_xxx_yyy                                           +*/
/*+                                                                         +*/
/*+               If we have a parameter error we will set EDD_STS_ERR_PARAM+*/
/*+               and call the handle-callbackfunction and abort the request+*/
/*+                                                                         +*/
/*+               EDD_STS_ERR_SERVICE is set if we get an invalid Service.  +*/
/*+               EDD_STS_ERR_CHANNEL_USE is set if the service is invalid  +*/
/*+               for this channel (i.e. not used by this channel)          +*/
/*+                                                                         +*/
/*+               If the request is valid, it is queued or executed.        +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_NRTRequest(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP                             Response;
    LSA_BOOL                            IndicateRqb;
    EDD_UPPER_NRT_CANCEL_PTR_TYPE       pRQBCancel;

    EDDP_NRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_NRTRequest(pRQB=0x%X, pDDB=0x%X, pHDB=0x%X)",
        pHDB->Handle, pRQB, pDDB, pHDB);

    IndicateRqb = LSA_TRUE;
    Response    = EDD_STS_OK;

    //---------------------------------------------------------------------------
    // Check if management is initialized, --> test private NRT pointer
    //---------------------------------------------------------------------------
    if (LSA_HOST_PTR_ARE_EQUAL(pHDB->pNRT, LSA_NULL))
    {
        Response = EDD_STS_ERR_CHANNEL_USE;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_NRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "[H:--] EDDP_NRTRequest(): NRT not initialized yet");
    }

    //---------------------------------------------------------------------------
    // every request needs a param-ptr, so check it.
    //---------------------------------------------------------------------------
    if (Response == EDD_STS_OK)
    {
        if  (LSA_HOST_PTR_ARE_EQUAL(pRQB->pParam, LSA_NULL))
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_NRTRequest(): ERROR -> Service(0x%X), RQB->pParam is NULL",
                EDD_RQB_GET_SERVICE(pRQB));
        }
    }

    if (Response == EDD_STS_OK)
    {
        Response = EDD_STS_ERR_NOT_SUPPORTED;   // default error

        switch (EDD_RQB_GET_SERVICE(pRQB))
        {
            //-------------------------------------------------------------
            case EDD_SRV_NRT_RECV:
            //-------------------------------------------------------------
            {
                // Receive-Request. Queue the Request within Rx-Queue used by handle
                EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_NRT_RECV (RxUsrQueue.Count=%d)",
                    pHDB->Handle, pHDB->pNRT->RxUsrQueue.Count);

                if ((Response = EDDP_NRTRecv(pRQB,pDDB,pHDB)) == EDD_STS_OK)
                {
                    IndicateRqb = LSA_FALSE;    // Request is queued
                }
            }
            break;

            //-------------------------------------------------------------
            case EDD_SRV_NRT_SEND:
            //-------------------------------------------------------------
            case EDD_SRV_NRT_SEND_TIMESTAMP:
            {
                EDDP_UPPER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_NRT_SEND", pHDB->Handle);

                // is tx-cancel running ?
                // tx-requests only allowed if no tx-cancel currently running
                if (!(pHDB->pNRT->CancelInProgress))
                {
                    // no Cancel is running
                    Response = EDDP_NRTSend (pRQB, pDDB, pHDB);

                    if (Response == EDD_STS_OK)
                    {
                        IndicateRqb = LSA_FALSE;    // Request is queued
                    }
                }
                else
                {
                    Response = EDD_STS_ERR_SEQUENCE;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_NRTRequest(): ERROR -> EDD_SRV_NRT_SEND: Cancel in progress",
                        pHDB->Handle);
                }
            }
            break;

            //-------------------------------------------------------------
            case EDD_SRV_NRT_CANCEL:
            //-------------------------------------------------------------
            {
                pRQBCancel = (EDD_UPPER_NRT_CANCEL_PTR_TYPE) pRQB->pParam;

                EDDP_UPPER_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_NRT_CANCEL (Mode=%d, RequestID=0x%X): RxUsrQueue.Count=%d, TxQueueReqCnt(%d), TxInProgCnt(%d)",
                    pHDB->Handle, pRQBCancel->Mode, pRQBCancel->RequestID, pHDB->pNRT->RxUsrQueue.Count, pHDB->pNRT->TxQueueReqCnt, pHDB->pNRT->TxInProgCnt);

                if (!(pHDB->pNRT->CancelInProgress))
                {
                    // no Cancel is running
                    Response = EDDP_NRTCancel (pRQB, pDDB, pHDB, pRQBCancel);

                    if (Response == EDD_STS_OK)
                    {
                        IndicateRqb = LSA_FALSE;    // Request is queued
                    }
                }
                else
                {
                    // Cancel is already running
                    Response = EDD_STS_ERR_SEQUENCE;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_NRTRequest(): ERROR -> EDD_SRV_NRT_CANCEL: Cancel is already running",
                        pHDB->Handle);
                }
            }
            break;

            //-------------------------------------------------------------
            case EDD_SRV_NRT_SET_DCP_FILTER:
            //-------------------------------------------------------------
            {
                // !!! ATTENTION: special development filter code for DCP/Hello and ARP !!!"
                EDDP_UPPER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_NRT_SET_DCP_FILTER", pHDB->Handle);

                Response = EDDP_NRTDCPFilter (pRQB, pDDB, pHDB);
            }
            break;

            //-------------------------------------------------------------
            case EDD_SRV_NRT_SET_DCP_HELLO_FILTER:
            //-------------------------------------------------------------
            {
                // !!! ATTENTION: special development filter code for DCP/Hello and ARP !!!"
                EDDP_UPPER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_NRT_SET_DCP_HELLO_FILTER", pHDB->Handle);

                Response = EDDP_NRTDCPHelloFilter (pRQB, pDDB, pHDB);
            }
            break;

            //-------------------------------------------------------------
            case EDD_SRV_NRT_SET_ARP_FILTER:
            //-------------------------------------------------------------
            {
                // !!! ATTENTION: special development filter code for DCP/Hello and ARP !!!"
                EDDP_UPPER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_NRT_SET_ARP_FILTER", pHDB->Handle);

                Response = EDDP_NRTARPFilter (pRQB, pDDB, pHDB);
            }
            break;

            //-------------------------------------------------------------
            default:
            //-------------------------------------------------------------
            {
                Response = EDD_STS_ERR_SERVICE;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_NRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_NRTRequest(): ERROR -> Unknown NRT-Service (pRQB=0x%X, Service=0x%X) !!!",
                    pHDB->Handle, pRQB, EDD_RQB_GET_SERVICE(pRQB));
            }
            break;
        }
    }

    if (IndicateRqb)
    {
        EDDP_GlbRequestFinish (pHDB, pRQB, Response);
    }

    EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] OUT: EDDP_NRTRequest(): Response=0x%X", pHDB->Handle, Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_NRTCloseChannel                        +*/
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
/*+               EDD_STS_OK_PENDING                                        +*/
/*+               EDD_STS_ERR_xxx                                           +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Close Channel for NRT.                                    +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_NRTCloseChannel(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB,
    LSA_HANDLE_TYPE             Handle)
{
    EDD_RSP                                         Response;
    LSA_UINT32                                      Tmp;
    EDDP_LOCAL_DDB_NRT_PTR_TYPE                     pNRTIface;
    EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE      pNrtApiCtrlIfaceMgm;

#if (EDDP_CFG_TRACE_MODE ==  0)
    LSA_UNUSED_ARG(Handle);
#endif

    EDDP_NRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_NRTCloseChannel()");

    EDDP_NRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:%2X] EDDP_NRTCloseChannel(pDDB=0x%X, pHDB=0x%X, Handle=0x%X)",
        pHDB->Handle, pDDB, pHDB, Handle);

    /* -------------------------------------------------------------------- */
    /* If handle dont uses NRT return ok.                                   */
    /* -------------------------------------------------------------------- */
    if (LSA_HOST_PTR_ARE_EQUAL(pHDB->pNRT, LSA_NULL))
    {
        return (EDD_STS_OK);
    }

    /* -------------------------------------------------------------------- */
    /* Next we check if cancel is in progress or any tx/rx-request present  */
    /* -------------------------------------------------------------------- */
    if (   (pHDB->pNRT->CancelInProgress)   /* cancel running in background */
        || (pHDB->pNRT->RxUsrQueue.Count)   /* Rx-RQB existing in Queue but expected empty queue (RQB returned to USER) */
        || (pHDB->pNRT->TxQueueReqCnt)      /* Tx-RQB existing in Queue but expected empty queue (RQB returned to USER) */
        || (pHDB->pNRT->TxInProgCnt)        /* Tx-RQB running in background in HW */
       )
    {
        Response = EDD_STS_ERR_SEQUENCE;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_NRT_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_NRTCloseChannel(): ERROR -> Cannot close channel: CancelInProgress(0x%X), RxUsrQueue.Count(%d), TxQueueReqCnt(%d), TxInProgCnt(%d)",
            pHDB->Handle, pHDB->pNRT->CancelInProgress, pHDB->pNRT->RxUsrQueue.Count, pHDB->pNRT->TxQueueReqCnt, pHDB->pNRT->TxInProgCnt);
    }
    else
    {
        Response    = EDD_STS_OK;
        pNRTIface   = &pDDB->NRT;

        /* -------------------------------------------------------------------- */
        /* disable all DCP-Identify filters, that were set by this channel      */
        /* -------------------------------------------------------------------- */
        switch (EDDP_HALGetPnipRevision(pDDB))
        {
#if defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT) || defined (EDDP_CFG_HW_PNIP_REV2_SUPPORT)
            case EDDP_HAL_HW_PNIP_REV1:
            case EDDP_HAL_HW_PNIP_REV2:
                if (   (Response == EDD_STS_OK)
                    && (pNRTIface->FilterTLVHandle == pHDB->Handle)
                    && (pNRTIface->FilterTLVCnt > 0)
                   )
                {
                    LSA_UINT8 Idx, LogicalTlvCnt;
                    
                    LogicalTlvCnt       = pNRTIface->DcpFilterCnt;
                    pNrtApiCtrlIfaceMgm = (EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE) pNRTIface->pNrtApiCtrlIfaceMgm;
                    EDDP_ASSERT_NO_NULL_PTR(pNrtApiCtrlIfaceMgm);

                    for ( Idx=0; Idx < (LogicalTlvCnt<<1); Idx++ )
                    {
                        if (Idx < (pNRTIface->FilterTLVCnt << 1))
                        {
                            // ***** Note: EDDP_HWCWriteSdtEntry() returns EDD_STS_OK always
                            EDDP_HWCWriteSdtEntry (pDDB, (LSA_UINT32)&pNrtApiCtrlIfaceMgm->ApiCtrlRamSdt[0], Idx+EDDP_SDT_IDX_DCP_MIN, PNIP_FILTER_STRING_DESCR_DEF_OFF);
                        }
                    }

                    pNRTIface->FilterTLVCnt     = 0;
                    pNRTIface->FilterTLVHandle  = EDDP_CFG_MAX_CHANNELS;
                }
                break;
#endif
            default:
                EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_NRTCloseChannel(): FATAL -> Version(%d) of PNIP is unknown",
                    EDDP_HALGetPnipRevision(pDDB));
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                break;
        }


        /* -------------------------------------------------------------------- */
        /* release the filters (and mgm memory)                                 */
        /* -------------------------------------------------------------------- */
        if (Response == EDD_STS_OK)
        {
            // *****  Release all filters which are used by the handle.
            for ( Tmp=0; Tmp < EDDP_NRT_MAX_FDC_CNT; Tmp++ )
            {
                if (LSA_HOST_PTR_ARE_EQUAL(pHDB->pNRT, pNRTIface->FilterInfo[Tmp].pNrtHDB))
                {
                    pNRTIface->FilterInfo[Tmp].pNrtHDB = LSA_NULL;
                }
            }

#ifndef EDDP_CFG_LOCAL_MEM_STATIC
            // *****  Release all memory used by the handle.
            if ( EDDP_GlbFreeLocalMem(pHDB->pNRT) != LSA_RET_OK )
            {
                EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_NRTCloseChannel(): ERROR -> EDDP_GlbFreeLocalMem(pHDB->pNRT)!",
                    pHDB->Handle);
                Response = EDD_STS_ERR_RESOURCE;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            }
            pHDB->pNRT = LSA_NULL;          // we always release the handle
#endif
            EDDP_ASSERT(pNRTIface->HandleCnt);
            pNRTIface->HandleCnt--;         // cnt of currently open handles
        }
    }

    EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_NRTCloseChannel(): Response=0x%X", Response);

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_NRTUpperAdaptAlignment                 +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB    +*/
/*+                             LSA_UINT32   EDD_UPPER_MEM_ATTR *   pAddr   +*/
/*+                             LSA_UINT32                          MemType +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+  Result                :    LSA_UINT32                                  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       :   pointer to DeviceDescriptionBlock               (in)    +*/
/*+  pAddr      :   pointer to upper address                        (in)    +*/
/*+  MemType    :   selected MemType                                (in)    +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+  Results    : aligned asic adress and adapted upper                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This utility function is used to adapt upper addres to    +*/
/**               the selected aligned asic address and return a aligned    +*/
/*+               asic adress.                                              +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_NRTUpperAdaptAlignment(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_UPPER_MEM_PTR_TYPE      pAddr)
{
    LSA_UINT32                  AsicAddr = 0;
    LSA_UINT32                  AlignmentAsicAddr, UdaptedUpperAddr;

    UdaptedUpperAddr = (LSA_UINT32)pAddr;
    EDDP_DEV_MEM_ADDR_UPPER_TO_PNIP(pDDB->hSysDev, pAddr, &AsicAddr);
    AlignmentAsicAddr = EDDP_HALTrimAlignment(AsicAddr, EDDP_HAL_32BIT_ALIGNMENT);

    EDDP_ASSERT_FALSE(AlignmentAsicAddr < AsicAddr);

    if ((AlignmentAsicAddr - AsicAddr) != 0)
    {
        UdaptedUpperAddr = UdaptedUpperAddr + (AlignmentAsicAddr - AsicAddr);
    }
    return UdaptedUpperAddr;
}

/*===========================================================================*/
/*                        public  functions                                  */
/*===========================================================================*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_NRTFillSetAndMapHWBarrier              +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_UINT32                  BarrierSizeALL  +*/
/*+                             LSA_UINT32                  BarrierSizeDEF  +*/
/*+                             LSA_UINT32                  BarrierSizeRTA  +*/
/*+                             LSA_UINT32                  BarrierSizeORG  +*/
/*+  Result                :    ---                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                     +*/
/*+  BarrierSizeAll : Overall barrier size (in bytes / ms )                 +*/
/*+  BarrierSizeDEF : barrier size of default frames  (in bytes / ms )      +*/
/*+  BarrierSizeRTA : barrier size of rta frames  (in bytes / ms )          +*/
/*+  BarrierSizeORG : barrier size of org frames  (in bytes / ms )          +*/
/*+                                                                         +*/
/*+  Result     : ---                                                       +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: helper function for setting hardware register             +*/
/*+                             of feed in load limitation                  +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_NRTFillSetAndMapHWBarrier(
    EDDP_LOCAL_DDB_PTR_TYPE pDDB,
    LSA_UINT32              BarrierSizeAll,
    LSA_UINT32              BarrierSizeDEF,
    LSA_UINT32              BarrierSizeRTA,
    LSA_UINT32              BarrierSizeORG)
{
    LSA_UINT32          Value0to7;
    LSA_UINT32          Value8to15;
    LSA_UINT32          RegValueTemp_DmacwCount;
    LSA_UINT16          HwType;

    // pDDB   checked by function caller !

    HwType = EDDP_HALGetChipVersion(pDDB);
    
    /* hardware type must either ERTEC200P or HERA, assured on HAL setup */
    EDDP_ASSERT((EDDP_HAL_HW_ERTEC200P == HwType)||(EDDP_HAL_HW_HERA == HwType));
    
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_DMACW_BARRIER_0_IFA, BarrierSizeAll);     // additive barrier limit for all TX rings
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_DMACW_BARRIER_1_IFA, BarrierSizeDEF);     // additive barrier limit for rings: Default, Tcp, Udp, IpMisc, Dcp, Ptcp
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_DMACW_BARRIER_2_IFA, BarrierSizeRTA);     // additive barrier limit for rings: Alarm
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_DMACW_BARRIER_3_IFA, BarrierSizeORG);     // additive barrier limit for rings: Mrp, Lldp, [Krisc]

    {
        // ***** mapping for EDDP_NRT_TX_BARRIER_IDX_ALL: -
        Value0to7  = 0;
        Value8to15 = 0;
        // ***** mapping for EDDP_NRT_TX_BARRIER_IDX_DEF: DEFAULT(0), TCP(1), UDP(2), MISC(3), DCP(4), SYNC(8)
        Value0to7 |= ( ((LSA_UINT32)EDDP_NRT_TX_BARRIER_PRIO_ALL_DEF<<PNIP_REG_DMACW_MAPPING_0TO7_IFA_SHFT_DMACW0_MAP) |
                       ((LSA_UINT32)EDDP_NRT_TX_BARRIER_PRIO_ALL_DEF<<PNIP_REG_DMACW_MAPPING_0TO7_IFA_SHFT_DMACW1_MAP) |
                       ((LSA_UINT32)EDDP_NRT_TX_BARRIER_PRIO_ALL_DEF<<PNIP_REG_DMACW_MAPPING_0TO7_IFA_SHFT_DMACW2_MAP) |
                       ((LSA_UINT32)EDDP_NRT_TX_BARRIER_PRIO_ALL_DEF<<PNIP_REG_DMACW_MAPPING_0TO7_IFA_SHFT_DMACW3_MAP) |
                       ((LSA_UINT32)EDDP_NRT_TX_BARRIER_PRIO_ALL_DEF<<PNIP_REG_DMACW_MAPPING_0TO7_IFA_SHFT_DMACW4_MAP) );
        Value8to15 |= ( (LSA_UINT32)EDDP_NRT_TX_BARRIER_PRIO_ALL_DEF<<PNIP_REG_DMACW_MAPPING_8TO15_IFA_SHFT_DMACW8_MAP );
        // ***** mapping for EDDP_NRT_TX_BARRIER_IDX_RTA: ALARM(5)
        Value0to7 |=  ( (LSA_UINT32)EDDP_NRT_TX_BARRIER_PRIO_ALL_RTA<<PNIP_REG_DMACW_MAPPING_0TO7_IFA_SHFT_DMACW5_MAP );
        // ***** mapping for EDDP_NRT_TX_BARRIER_IDX_ORG: MRP(6), LLDP(7), KRISC(14)
        Value0to7 |= ( ( (LSA_UINT32)EDDP_NRT_TX_BARRIER_PRIO_ALL_ORG<<PNIP_REG_DMACW_MAPPING_0TO7_IFA_SHFT_DMACW6_MAP ) |
                       ( (LSA_UINT32)EDDP_NRT_TX_BARRIER_PRIO_ALL_ORG<<PNIP_REG_DMACW_MAPPING_0TO7_IFA_SHFT_DMACW7_MAP ) );
        Value8to15 |=  ( (LSA_UINT32)EDDP_NRT_TX_BARRIER_PRIO_ALL_ORG<<PNIP_REG_DMACW_MAPPING_8TO15_IFA_SHFT_DMACW14_MAP );
    
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_DMACW_MAPPING_0TO7_IFA, Value0to7);
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_DMACW_MAPPING_8TO15_IFA, Value8to15);
        
        /* There are macros which are checking PNIP_REG_DMACW_COUNT_0_IFA__MSK_ENABLECOUNT and PNIP_REG_DMACW_COUNT_0_IFA__MSK_ENABLEPRIORITY 
        have the same value for 0..3. Checked in eddp_hal_pnip_val.h. */
        RegValueTemp_DmacwCount = PNIP_REG_DMACW_COUNT_0_IFA__MSK_ENABLECOUNT |
                                  PNIP_R2_REG_DMACW_COUNT_0_IFA__MSK_ENABLEPRIORITY;
    }
    
        // ***** enable EDDP_NRT_TX_BARRIER_IDX_ALL
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_DMACW_COUNT_0_IFA, RegValueTemp_DmacwCount);
    // ***** enable EDDP_NRT_TX_BARRIER_IDX_DEF
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_DMACW_COUNT_1_IFA, RegValueTemp_DmacwCount);
    // ***** enable EDDP_NRT_TX_BARRIER_IDX_RTA
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_DMACW_COUNT_2_IFA, RegValueTemp_DmacwCount);
    // ***** enable EDDP_NRT_TX_BARRIER_IDX_ORG
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_DMACW_COUNT_3_IFA, RegValueTemp_DmacwCount);
    
    // ***** set barrier length with 1ms
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_DMACW_TRIGGER_IFA, EDDP_NRT_TX_BARRIER_TIME_1MS);  // timewindow for a barrier cycle is 1ms
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_NRTFillActivate                        +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+  Result                :    ---                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+                                                                         +*/
/*+  Result     : ---                                                       +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: helper function for activate feed in load limitation      +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_NRTFillActivate(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB)
{
    // pDDB   checked by function caller !

    /* use FILL values for send limitation */
    EDDP_NRTFillSetAndMapHWBarrier(pDDB,
                                   EDDP_NRT_TX_BARRIER_SIZE_ALL,
                                   EDDP_NRT_TX_BARRIER_SIZE_DEF,
                                   EDDP_NRT_TX_BARRIER_SIZE_RTA,
                                   EDDP_NRT_TX_BARRIER_SIZE_ORG);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_NRTFillDeactivate                      +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+  Result                :    ---                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+                                                                         +*/
/*+  Result     : ---                                                       +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: helper function for deactivate feed in load limitation    +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_NRTFillDeactivate(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB)
{
    // pDDB   checked by function caller !

    /* use default values for send limitation */
    EDDP_NRTFillSetAndMapHWBarrier(pDDB,
                                   EDDP_NRT_TX_BARRIER_SIZE_OFF,
                                   EDDP_NRT_TX_BARRIER_SIZE_OFF,
                                   EDDP_NRT_TX_BARRIER_SIZE_OFF,
                                   EDDP_NRT_TX_BARRIER_SIZE_OFF);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_NRTFillActivate                        +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                        :    LSA_BOOL resetToDefault                     +*/
/*+                        :    LSA_BOOL LoadLimitationActive               +*/
/*+                        :    LSA_BOOL IO_Configured                      +*/
/*+  Result                :    ---                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB                 : Pointer to DeviceDescriptionBlock               +*/
/*+  resetToDefault       : reset fill setting to default                   +*/
/*+  LoadLimitationActive : state for fill ( active:TRUE / inactive:FALSE ) +*/
/*+  IO_Configured        : state of IO_Configured ( yes:TRUE / no:FALSE )  +*/
/*+                                                                         +*/
/*+  Result     : ---                                                       +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: setup fill with new settings:                             +*/
/*+                 resetToDefault = TRUE:                                  +*/
/*+                           setup FILL with default parameter of startup  +*/
/*+                 resetToDefault = FALSE:                                 +*/
/*+                           setup FILL with given values                  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_NRTFillSetup(
    EDDP_LOCAL_DDB_PTR_TYPE pDDB,
    LSA_BOOL                resetToDefault,
    LSA_BOOL                LoadLimitationActive,
    LSA_BOOL                IO_Configured)
{
    if(resetToDefault)
    {
        /* restore startup values */
        pDDB->NRT.FILL.FeedInLoadLimitationActive = pDDB->NRT.FILL.FeedInLoadLimitationActiveStartup;
        pDDB->NRT.FILL.IO_Configured = pDDB->NRT.FILL.IO_ConfiguredStartup;
    }
    else
    {
        /* values of prm record */
        pDDB->NRT.FILL.FeedInLoadLimitationActive = LoadLimitationActive;
        pDDB->NRT.FILL.IO_Configured = IO_Configured;
    }

    if(pDDB->NRT.FILL.FeedInLoadLimitationActive)
    {
        EDDP_NRTFillActivate(pDDB);
    }
    else
    {
        EDDP_NRTFillDeactivate(pDDB);
    }

    EDDP_NRT_TRACE_02(pDDB->TraceIdx,LSA_TRACE_LEVEL_NOTE_HIGH,
                "EDDP_NRTFillSetup: LoadLimitationActive(%u), IO_Configured(%u)",
                pDDB->NRT.FILL.FeedInLoadLimitationActive,pDDB->NRT.FILL.IO_Configured);
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_NRTFillIsIOAllowed                      +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+  Result                :    LSA_BOOL                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_NOT_ALLOWED                                   +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: function returns OK if prov / cons could be added         +*/
/*+                otherwise ERR_NOT_ALLOWED                                +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_NRTFillIsIOAllowed(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB)
{
    EDD_RSP isIOAllowed = EDD_STS_ERR_NOT_ALLOWED;
    // pDDB   checked by function caller !

    if(pDDB->NRT.FILL.IO_Configured)
    {
        /* IO_Configured has to be set for adding consumer / provider */
        isIOAllowed = EDD_STS_OK;
    }
    return(isIOAllowed);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_NRTFillIsFeedInLoadLimitationActive    +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+  Result                :    LSA_BOOL                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_NOT_ALLOWED                                   +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: function returns OK if prov / cons could be added         +*/
/*+                otherwise ERR_NOT_ALLOWED                                +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_NRTFillIsFeedInLoadLimitationActive(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB)
{
    EDD_RSP isIOAllowed = EDD_STS_ERR_NOT_ALLOWED;
    // pDDB   checked by function caller !

    if(pDDB->NRT.FILL.FeedInLoadLimitationActive)
    {
        /* FILL has to be set for adding consumer / provider */
        isIOAllowed = EDD_STS_OK;
    }
    return(isIOAllowed);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_NRTFillGetStartupParameter             +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+  Result                :    LSA_BOOL                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+                                                                         +*/
/*+  Result     : LSA_TRUE                                                  +*/
/*+               LSA_FALSE                                                 +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: function returns the default parameter of fill            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_NRTFillGetStartupParameter(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB)
{
    return (pDDB->NRT.FILL.FeedInLoadLimitationActiveStartup);
}
