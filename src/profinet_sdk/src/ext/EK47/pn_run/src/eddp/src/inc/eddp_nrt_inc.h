
#ifndef EDDP_NRT_INC_H                   /* ----- reinclude-protection ----- */
#define EDDP_NRT_INC_H

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
/*  F i l e               &F: eddp_nrt_inc.h                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Internal NRT headerfile                                                  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  24.09.08    SF    initial version.                                       */
#endif
/*****************************************************************************/

#include "eddp_nrt.h"       // public NRT-API
#include "eddp_nrt_isr.h"   // interrupt handling
#include "eddp_nrt_usr.h"   // request handling
#include "eddp_nrt_rcv.h"   // receive handling
#include "eddp_nrt_snd.h"   // transmit handling


/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/

#define EDDP_NRT_USE_TX_BARRIER
/*===========================================================================*/
#define EDDP_NRT_TX_BARRIER_IDX_ALL         0       // index for global
#define EDDP_NRT_TX_BARRIER_IDX_DEF         1       // limit for: DEFAULT, TCP, UDP, MISC, DCP, SYNC
#define EDDP_NRT_TX_BARRIER_IDX_RTA         2       // limit for: ALARM
#define EDDP_NRT_TX_BARRIER_IDX_ORG         3       // limit for: MRP, LLDP, KRISC

#define EDDP_NRT_TX_BARRIER_PRIO_ALL_DEF    (8|1)   // limit for: DEFAULT, TCP, UDP, MISC, DCP, SYNC
#define EDDP_NRT_TX_BARRIER_PRIO_ALL_RTA    (8|2)   // limit for: ALARM
#define EDDP_NRT_TX_BARRIER_PRIO_ALL_ORG    (8|3)   // limit for: MRP, LLDP, KRISC

#define EDDP_NRT_TX_BARRIER_SIZE_OFF        12500   // (30D4h) bytes within 1ms
#define EDDP_NRT_TX_BARRIER_SIZE_FILL       3084    // (0C0Ch) bytes within 1ms
#define EDDP_NRT_TX_BARRIER_SIZE_ALL        EDDP_NRT_TX_BARRIER_SIZE_FILL    // see FILL
#define EDDP_NRT_TX_BARRIER_SIZE_DEF        EDDP_NRT_TX_BARRIER_SIZE_FILL    // see FILL
#define EDDP_NRT_TX_BARRIER_SIZE_RTA        1584    // (0630h) bytes within 1ms
#define EDDP_NRT_TX_BARRIER_SIZE_ORG        1584    // (0630h) bytes within 1ms

#define EDDP_NRT_TX_BARRIER_TIME_1MS        1000000 // 1ms = 0xF4240
    
#define EDDP_NRT_FRAME_OVERHEAD             (0x18)  // IFG+Preamble+SFD+FCS
    
// If a RQB with a Length less than 24 PNIP generate an TXError         
#define EDDP_NRT_TX_MIN_SND_LEN_REV12       24
#define EDDP_NRT_TX_MIN_SND_LEN_REV3        14

/*===========================================================================*/
#define EDDP_NRT_RX_DMACW_CNT   32  // 32 Rx-DMACW entries for every IFACE
#define EDDP_NRT_TX_DMACW_CNT   16  // 16 Tx-DMACW entries for every IFACE


/*===========================================================================*/
//                                                        see: edd_usr.h                 see: eddp_hwf_val.h
#define EDDP_NRT_FILTER_DROP_IDX                    0     // EDD_NRT_FRAME_DROP          (DROP)                      -->  Type-Frames not IP/DCP/LLDP/xRT      
#define EDDP_NRT_FILTER_OTHER_IDX                   1     // EDD_NRT_FRAME_OTHER         EDDP_FDC_OTHER              -->  Type-Frames not IP/DCP/LLDP/xRT      
#define EDDP_NRT_FILTER_IP_ICMP_IDX                 2     // EDD_NRT_FRAME_IP_ICMP       EDDP_FDC_IP_ICMP            -->  Internet Control Message Prot.      
#define EDDP_NRT_FILTER_IP_IGMP_IDX                 3     // EDD_NRT_FRAME_IP_IGMP       EDDP_FDC_IP_IGMP            -->  Internet Group Management Prot.      
#define EDDP_NRT_FILTER_IP_TCP_IDX                  4     // EDD_NRT_FRAME_IP_TCP        EDDP_FDC_IP_TCP             -->  Transmission Control Protocol      
#define EDDP_NRT_FILTER_IP_UDP_IDX                  5     // EDD_NRT_FRAME_IP_UDP        EDDP_FDC_IP_UDP             -->  User Datagram Protocol      
#define EDDP_NRT_FILTER_IP_OTHER_IDX                6     // EDD_NRT_FRAME_IP_OTHER      EDDP_FDC_IP_OTHER           -->  all other IP protocols      
#define EDDP_NRT_FILTER_UDP_DCP_IDX                 7     // EDD_NRT_FRAME_UDP_DCP       EDDP_FDC_UDP_DCP            -->  xRToverUDP DCP type frames      
#define EDDP_NRT_FILTER_UDP_DCP_HELLO_IDX           8     // EDD_NRT_FRAME_UDP_DCP_HELLO EDDP_FDC_UDP_DCP_HELLO      -->  DCP-HELLO Frame with UDP-Header  
#define EDDP_NRT_FILTER_UDP_ASRT_IDX                9     // EDD_NRT_FRAME_UDP_ASRT      EDDP_FDC_UDP_ASRT           -->  xRToverUDP ASRT type frames      
  //      UDP_RT                                    -     //                             (-)                         -->  ACW
#define EDDP_NRT_FILTER_ARP_IDX                     10    // EDD_NRT_FRAME_ARP           EDDP_FDC_ARP                -->  ARP 
  //#define EDDP_NRT_FILTER_RARP_IDX                -     // EDD_NRT_FRAME_RARP          (-)                         -->  reverse-ARP      
#define EDDP_NRT_FILTER_MRP_IDX                     11    // EDD_NRT_FRAME_MRP           EDDP_FDC_MRP                -->  MRP frames      
#define EDDP_NRT_FILTER_LLDP_IDX                    12    // EDD_NRT_FRAME_LLDP          EDDP_FDC_LLDP               -->  LLDP-Frames      
#define EDDP_NRT_FILTER_DCP_IDX                     13    // EDD_NRT_FRAME_DCP           EDDP_FDC_DCP                -->  DCP-Frames with xRT-Header      
#define EDDP_NRT_FILTER_DCP_HELLO_IDX               14    // EDD_NRT_FRAME_DCP_HELLO     EDDP_FDC_DCP_HELLO          -->  DCP-HELLO Frame with xRT-Header 
#define EDDP_NRT_FILTER_ASRT_IDX                    15    // EDD_NRT_FRAME_ASRT          EDDP_FDC_ASRT               -->  ASRT-Frames      
#define EDDP_NRT_FILTER_LEN_DCP_IDX                 16    // EDD_NRT_FRAME_LEN_DCP       EDDP_FDC_LEN_DCP            -->  DCP with SNAP-Header      
#define EDDP_NRT_FILTER_LEN_TSYNC_IDX               17    // EDD_NRT_FRAME_LEN_TSYNC     EDDP_FDC_LEN_TSYNC          -->  TSYNC with SNAP-Header     
#define EDDP_NRT_FILTER_LEN_STDBY_IDX               18    // EDD_NRT_FRAME_LEN_STDBY     EDDP_FDC_LEN_STDBY          -->  MRP-Standby SNAP Frame  
#define EDDP_NRT_FILTER_LEN_HSR_IDX                 19    // EDD_NRT_FRAME_LEN_HSR       EDDP_FDC_LEN_HSR            -->  LEN_HSR SNAP Frame  
#define EDDP_NRT_FILTER_LEN_SINEC_FWL_IDX           20    // EDD_NRT_FRAME_LEN_SINEC_FWL EDDP_FDC_LEN_LEN_SINEC_FWL  -->  FimewareLoader uses SNAP-Header     
#define EDDP_NRT_FILTER_LEN_OTHER_IDX               21    // EDD_NRT_FRAME_LEN_OTHER     EDDP_FDC_LEN_OTHER          -->  LEN_HSR SNAP FrameLEN-Type Frames not DCP/TSYNC/HSR  
#define EDDP_NRT_FILTER_PTCP_IDX                    22    // EDD_NRT_FRAME_PTCP_ANNO     EDDP_FDC_PTCP_ANNOUNCE      -->  xRT PTCP frames (KRISC: sync0/1, delay)
  //      RT_CT                                     -     //                             (-)                         -->  ACW -> dropping always
  //      RT_SF                                     -     //                             (-)                         -->  ACW -> dropping always
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_1      23    // EDD_NRT_FRAME_DCP_HELLO1     EDDP_FDC_DCP_HELLO         -->  DCP-HELLO with station name stored in Filter1 
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_2      24    // EDD_NRT_FRAME_DCP_HELLO2     EDDP_FDC_DCP_HELLO2        -->  DCP-HELLO with station name stored in Filter2  
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_3      25    // EDD_NRT_FRAME_DCP_HELLO3     EDDP_FDC_DCP_HELLO3        -->  DCP-HELLO with station name stored in Filter3
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_4      26    // EDD_NRT_FRAME_DCP_HELLO4     EDDP_FDC_DCP_HELLO4        -->  DCP-HELLO with station name stored in Filter4
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_5      27    // EDD_NRT_FRAME_DCP_HELLO5     EDDP_FDC_DCP_HELLO5        -->  DCP-HELLO with station name stored in Filter5
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_6      28    // EDD_NRT_FRAME_DCP_HELLO6     EDDP_FDC_DCP_HELLO6        -->  DCP-HELLO with station name stored in Filter6
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_7      29    // EDD_NRT_FRAME_DCP_HELLO7     EDDP_FDC_DCP_HELLO7        -->  DCP-HELLO with station name stored in Filter7
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_8      30    // EDD_NRT_FRAME_DCP_HELLO8     EDDP_FDC_DCP_HELLO8        -->  DCP-HELLO with station name stored in Filter8
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_9      31    // EDD_NRT_FRAME_DCP_HELLO9     EDDP_FDC_DCP_HELLO9        -->  DCP-HELLO with station name stored in Filter9
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_10     32    // EDD_NRT_FRAME_DCP_HELLO10    EDDP_FDC_DCP_HELLO10       -->  DCP-HELLO with station name stored in Filter10
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_11     33    // EDD_NRT_FRAME_DCP_HELLO11    EDDP_FDC_DCP_HELLO11       -->  DCP-HELLO with station name stored in Filter11
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_12     34    // EDD_NRT_FRAME_DCP_HELLO12    EDDP_FDC_DCP_HELLO12       -->  DCP-HELLO with station name stored in Filter12
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_13     35    // EDD_NRT_FRAME_DCP_HELLO13    EDDP_FDC_DCP_HELLO13       -->  DCP-HELLO with station name stored in Filter13
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_14     36    // EDD_NRT_FRAME_DCP_HELLO14    EDDP_FDC_DCP_HELLO14       -->  DCP-HELLO with station name stored in Filter14
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_15     37    // EDD_NRT_FRAME_DCP_HELLO15    EDDP_FDC_DCP_HELLO15       -->  DCP-HELLO with station name stored in Filter15
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_16     38    // EDD_NRT_FRAME_DCP_HELLO16    EDDP_FDC_DCP_HELLO16       -->  DCP-HELLO with station name stored in Filter16
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_17     39    // EDD_NRT_FRAME_DCP_HELLO17    EDDP_FDC_DCP_HELLO17       -->  DCP-HELLO with station name stored in Filter17 
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_18     40    // EDD_NRT_FRAME_DCP_HELLO18    EDDP_FDC_DCP_HELLO18       -->  DCP-HELLO with station name stored in Filter18 
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_19     41    // EDD_NRT_FRAME_DCP_HELLO19    EDDP_FDC_DCP_HELLO19       -->  DCP-HELLO with station name stored in Filter19
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_20     42    // EDD_NRT_FRAME_DCP_HELLO20    EDDP_FDC_DCP_HELLO20       -->  DCP-HELLO with station name stored in Filter10
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_21     43    // EDD_NRT_FRAME_DCP_HELLO21    EDDP_FDC_DCP_HELLO21       -->  DCP-HELLO with station name stored in Filter11
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_22     44    // EDD_NRT_FRAME_DCP_HELLO22    EDDP_FDC_DCP_HELLO22       -->  DCP-HELLO with station name stored in Filter12
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_23     45    // EDD_NRT_FRAME_DCP_HELLO23    EDDP_FDC_DCP_HELLO23       -->  DCP-HELLO with station name stored in Filter13
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_24     46    // EDD_NRT_FRAME_DCP_HELLO24    EDDP_FDC_DCP_HELLO24       -->  DCP-HELLO with station name stored in Filter14
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_25     47    // EDD_NRT_FRAME_DCP_HELLO25    EDDP_FDC_DCP_HELLO25       -->  DCP-HELLO with station name stored in Filter15
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_26     48    // EDD_NRT_FRAME_DCP_HELLO26    EDDP_FDC_DCP_HELLO26       -->  DCP-HELLO with station name stored in Filter16
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_27     49    // EDD_NRT_FRAME_DCP_HELLO27    EDDP_FDC_DCP_HELLO27       -->  DCP-HELLO with station name stored in Filter17
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_28     50    // EDD_NRT_FRAME_DCP_HELLO28    EDDP_FDC_DCP_HELLO28       -->  DCP-HELLO with station name stored in Filter18
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_29     51    // EDD_NRT_FRAME_DCP_HELLO29    EDDP_FDC_DCP_HELLO29       -->  DCP-HELLO with station name stored in Filter19
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_30     52    // EDD_NRT_FRAME_DCP_HELLO30    EDDP_FDC_DCP_HELLO30       -->  DCP-HELLO with station name stored in Filter30
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_31     53    // EDD_NRT_FRAME_DCP_HELLO31    EDDP_FDC_DCP_HELLO31       -->  DCP-HELLO with station name stored in Filter31
#define EDDP_NRT_FILTER_DCP_HELLO_IDX_FILTER_32     54    // EDD_NRT_FRAME_DCP_HELLO32    EDDP_FDC_DCP_HELLO32       -->  DCP-HELLO with station name stored in Filter32
#define EDDP_NRT_FILTER_PTCP_SYNC_IDX               55    // EDD_NRT_FRAME_PTCP_SYNC      EDDP_FDC_DCP_HELLO2        -->  xRT PTCP frames Sync/FU Frames (only for validate NRTFrameFilter on OpenChannel) 
#define EDDP_NRT_FILTER_PTCP_DELAY_IDX              56    // EDD_NRT_FRAME_PTCP_DELAY     EDDP_FDC_DCP_HELLO3        -->  xRT PTCP frames Delay Frames (only for validate NRTFrameFilter on OpenChannel)
#define EDDP_NRT_FILTER_DLR_IDX                     57    // EDD_NRT_FRAME_DLR            EDDP_FDC_MRP               -->  DLR frames

#define EDDP_NRT_MAX_FDC_CNT                        58    // we have currently 27 FiltersDecisionCodes + DropRxQueue

/*===========================================================================*/
//      DMACW-Queue                           EDDP-ID used with Channel
#define EDDP_NRT_TX_GRP_DEFAULT     0       // - EDD_NRT_TX_GRP_DEFAULT
                                            // - EDD_NRT_TX_GRP_LEN_TSYNC
#define EDDP_NRT_TX_GRP_TCP         1       // - EDD_NRT_TX_GRP_IP_TCP
#define EDDP_NRT_TX_GRP_UDP         2       // - EDD_NRT_TX_GRP_IP_UDP
#define EDDP_NRT_TX_GRP_IP_MISC     3       // - EDD_NRT_TX_GRP_IP_IGMP
                                            // - EDD_NRT_TX_GRP_IP_ICMP
                                            // - EDD_NRT_TX_GRP_ARP
#define EDDP_NRT_TX_GRP_DCP         4       // - EDD_NRT_TX_GRP_DCP
#define EDDP_NRT_TX_GRP_ALARM       5       // - EDD_NRT_TX_GRP_ASRT
#define EDDP_NRT_TX_GRP_MRP         6       // - EDD_NRT_TX_GRP_MRP
                                            // - EDD_NRT_TX_GRP_LEN_STDBY
                                            // - EDD_NRT_TX_GRP_LEN_HSR
#define EDDP_NRT_TX_GRP_LLDP        7       // - EDD_NRT_TX_GRP_LLDP
#define EDDP_NRT_TX_GRP_SYNC        8       // - EDD_NRT_TX_GRP_PTCP

#define EDDP_NRT_TX_DMACW_MASK      0x1FF   // use 9 DMACW rings in NRT
#define EDDP_NRT_TX_DMACW_MIN       0       // use rings 0-5 in NRT
#define EDDP_NRT_TX_DMACW_MAX       8     
#define EDDP_NRT_TX_GRP_CNT         ((EDDP_NRT_TX_DMACW_MAX-EDDP_NRT_TX_DMACW_MIN)+1)    // 6 queues from IDX 0..5

#define EDDP_K32_TX_DMACW_MASK      0xC000  // use 2 DMACW rings in Krisc32
#define EDDP_K32_TX_DMACW_MIN       14      // use rings 14-15 in Krisc32
#define EDDP_K32_TX_DMACW_MAX       15
#define EDDP_K32_TX_GRP_CNT         ((EDDP_K32_TX_DMACW_MAX-EDDP_K32_TX_DMACW_MIN)+1)    // 2 queues from IDX 6..7


/*===========================================================================*/
// defines for UDP destination ports
// DHCP Client
#define EDDP_NRT_UDP_DST_PORT_DHCP_CLIENT       68
//  NTP/SNTP
#define EDDP_NRT_UDP_DST_PORT_NTP_SNTP          123
//  SNMP
#define EDDP_NRT_UDP_DST_PORT_SNMP_MIN          161
#define EDDP_NRT_UDP_DST_PORT_SNMP_MAX          162
//  IANA_PNIO_EPM_PORT
#define EDDP_NRT_UDP_DST_PORT_IANA_EPM          0x8894
//  IANA_FREE_PORT
#define EDDP_NRT_UDP_DST_PORT_FREE_MIN          0xC000
#define EDDP_NRT_UDP_DST_PORT_FREE_MAX          0xFFFF
//  Ethernet/IP Ports
#define EDDP_NRT_UDP_DST_PORT_EIP_DLR           2222
#define EDDP_NRT_UDP_DST_PORT_EIP               44818

/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/



/* -------------------------------------------------------------------------*/
/* for DCP-Filtering of xRT-DCP-Frames (Hello)                              */
/* -------------------------------------------------------------------------*/
typedef struct _EDDP_NRT_DCP_HELLO_FILTER_MGM_TYPE    EDDP_LOCAL_MEM_ATTR *   EDDP_NRT_DCP_HELLO_FILTER_MGM_PTR;
typedef struct _EDDP_NRT_DCP_HELLO_FILTER_MGM_TYPE
{
    LSA_BOOL                    InUse;          /* Filter in Use?           */
    LSA_HANDLE_TYPE             Handle;         /* Handle of channel        */
    LSA_UINT32                  UserID;         /* UserID if in use         */
    LSA_UINT8                   StationName[EDD_DCP_MAX_DCP_HELLO_NAME_OF_STATION];
    LSA_UINT32                  StationNameLen; /* Length of StationName    */
} EDDP_NRT_DCP_HELLO_FILTER_MGM_TYPE;


/* -------------------------------------------------------------------------*/
/* managing rx-filtering                                                    */
/* -------------------------------------------------------------------------*/
typedef struct _EDDP_HDB_NRT_TYPE       EDDP_LOCAL_MEM_ATTR *    EDDP_LOCAL_HDB_NRT_PTR_TYPE;

typedef struct _EDDP_NRT_RX_FILTER_MGM_TYPE  EDDP_LOCAL_MEM_ATTR *    EDDP_NRT_RX_FILTER_MGM_TYPE_PTR;
typedef struct _EDDP_NRT_RX_FILTER_MGM_TYPE
{
    LSA_UINT16                      HWFrameFilter;      // FrameFilter (FDC) set by HW-FilterCode in RX-frame info
    LSA_UINT32                      UsrFrameFilter;     // public FrameFilter selector in EDDP-API
    EDDP_LOCAL_HDB_NRT_PTR_TYPE     pNrtHDB;            // binding mgm-information for opened channel
} EDDP_NRT_RX_FILTER_MGM_TYPE;


/* -------------------------------------------------------------------------*/
/* managing rx-dmacw-rings                                                  */
/* -------------------------------------------------------------------------*/
typedef struct _EDDP_NRT_RX_FRAMEBUFFER_ARRAY  EDDP_LOCAL_MEM_ATTR *   EDDP_NRT_RX_FRAMEBUFFER_ARRAY_PTR;
typedef struct _EDDP_NRT_RX_FRAMEBUFFER_ARRAY
{
    LSA_UINT32                              NrOfElements;               //Nr of elements
    LSA_UINT32                              SizeOfStruct;
    EDD_UPPER_MEM_PTR_TYPE                  FrameBufferUpperAddr[1];
} EDDP_NRT_RX_FRAMEBUFFER_ARRAY;

typedef struct _EDDP_NRT_RX_RING_TYPE  EDDP_LOCAL_MEM_ATTR *           EDDP_NRT_RX_RING_TYPE_PTR;
typedef struct _EDDP_NRT_RX_RING_TYPE
{
    EDD_UPPER_MEM_PTR_TYPE                  pAllocAddr;
    EDDP_NRT_RX_FRAMEBUFFER_ARRAY_PTR       pFrameBufferArray;
    PNIP_LOCAL_DMACW_RCV_ITEM_PTR_TYPE      pRxRing;
    LSA_UINT16                              ItemCount;
    LSA_UINT16                              DoneIdx;
    LSA_UINT16                              ExpBuf32Size;
} EDDP_NRT_RX_RING_TYPE;
/* pAllocAddr               : by EDDP real addr of alloced memory                                                                   */
/* FrameBufferUpperAddr     : Array of frame buffer addresses                                                                       */
/* pRxRing                  : head of DMACW ring                                                                                    */
/* ItemCount                : number of items in ring                                                                               */
/* DoneIdx                  : index position in DMACW ring for processed requests                                                   */
/* ExpBuf32Size             : compare value for buffer size when binding RX buffer to item frame buffers must be 32bit aligned      */


/* -------------------------------------------------------------------------*/
/* managing tx-requests                                                     */
/* -------------------------------------------------------------------------*/

typedef struct _EDDP_NRT_TX_MGM_TYPE  EDDP_LOCAL_MEM_ATTR *           EDDP_NRT_TX_MGM_TYPE_PTR;
typedef struct _EDDP_NRT_TX_MGM_TYPE
{
    EDDP_RQB_QUEUE_TYPE         TxReqQueue;     // Tx-RQB-Queue
    LSA_UINT32                  InProgCntMax;   // Max Number of Tx allowed to be in progress
} EDDP_NRT_TX_MGM_TYPE;


/* -------------------------------------------------------------------------*/
/* managing tx-dmacw-rings                                                  */
/* -------------------------------------------------------------------------*/

typedef struct _EDDP_NRT_TX_RING_TYPE  EDDP_LOCAL_MEM_ATTR *           EDDP_NRT_TX_RING_TYPE_PTR;
typedef struct _EDDP_NRT_TX_RING_TYPE
{
    EDD_UPPER_MEM_PTR_TYPE                  pAllocAddr ;    // real addr of alloced memory 
    PNIP_LOCAL_DMACW_SND_ITEM_PTR_TYPE      pTxRing;        // head of DMACW ring (32bit aligned)
    LSA_UINT16                              ItemCount;      // number of items in ring
    LSA_UINT16                              FreeCount;      // number of free items in ring (for transmit request)
    LSA_UINT16                              InsertIdx;      // index position in DMACW ring for insert transmit request 
    LSA_UINT16                              DoneIdx;        // index position in DMACW ring for processed requests 
} EDDP_NRT_TX_RING_TYPE;


typedef enum
{
    EDDP_NRT_IF_UNKNOWN     = 0,    // unknown                      --> e.g memset(0) in core
                                    //                              --> EDDP_NRTDeviceClose()
    EDDP_NRT_IF_PREINIT     = 1,    // zero setup of mgmt variables --> EDDP_NRTDeviceOpen->_EDDP_NRTInitMgmZero()
                                    //                              --> EDDP_NRTDeviceShutdown()
    EDDP_NRT_IF_INITDONE    = 2,    // init complete but disabled   --> EDDP_NRTDeviceSetup()->_EDDP_NRTInitInterface()
                                    //                              --> EDDP_NRTDeviceIFShutdown()
    EDDP_NRT_IF_ACTIVE      = 3     // HW-Filter enabled            --> EDDP_NRTDeviceIFSetup()
} EDDP_IFSTAT_NRT_TYPE;


/* -------------------------------------------------------------------------*/
/* some interface statistics                                                */
/* -------------------------------------------------------------------------*/

#if (EDDP_NRT_RXTX_COUNTER_SUPPORT == 1)
typedef struct _EDDP_NRT_STAT_TYPE       EDDP_LOCAL_MEM_ATTR *   EDDP_NRT_STAT_TYPE_PTR;
typedef struct _EDDP_NRT_STAT_TYPE
{
    LSA_UINT32      InUcastPkts;        // counts UC rx-packets
    LSA_UINT32      InNUcastPkts;       // counts BC/MC rx-packets
    LSA_UINT32      OutUcastPkts;       // counts UC tx-packets
    LSA_UINT32      OutNUcastPkts;      // counts BC/MC tx-packets
    LSA_UINT32      OutDiscards;        // counts tx-packets with TX errors (only timeout error!!!)
    LSA_UINT32      OutErrors;          // counts tx-packets with TX errors (with timeout error!!!)
    LSA_UINT32      InMulticastPkts;    // counts MC rx-packets
    LSA_UINT32      InBroadcastPkts;    // counts BC rx-packets
    LSA_UINT32      OutMulticastPkts;   // counts MC tx-packets
    LSA_UINT32      OutBroadcastPkts;   // counts BC tx-packets
} EDDP_NRT_STAT_TYPE;
#endif


/*===========================================================================*/
/* NRT handle (channel) management-structures                                */
/*===========================================================================*/

/* FILL management */
typedef struct _EDDP_NRT_FILL_TYPE
{
    LSA_BOOL                            FeedInLoadLimitationActive;             // current state of fill 
    LSA_BOOL                            FeedInLoadLimitationActiveStartup;      // default state of fill (startup)
    LSA_BOOL                            IO_Configured;                          // state of io_configured
    LSA_BOOL                            IO_ConfiguredStartup;                   // state of io_configured (startup)
}EDDP_NRT_FILL_TYPE;
/*---------------------------------------------------------------------------*/
/* Managment information for device                                          */
/* in eddp_dev.h  EDDP_DDB_TYPE.EDDP_DDB_NRT_TYPE (NRT)                      */
/*---------------------------------------------------------------------------*/

#ifndef EDDP_HW_MAX_TX_RINGS
    #define EDDP_HW_MAX_TX_RINGS      16      // the ASIC has upto 16 physical TX Rings
    #define EDDP_HW_MAX_RX_RINGS      32      // the ASIC has upto 32 physical RX Rings
#endif

typedef struct _EDDP_DDB_NRT_TYPE       EDDP_LOCAL_MEM_ATTR *    EDDP_LOCAL_DDB_NRT_PTR_TYPE;
typedef struct _EDDP_DDB_NRT_TYPE
{
    LSA_BOOL                            CfgFilled;
    EDDP_DPB_NRT_TYPE                   NrtIFCfg;

    // use EDD_COMMON_MEM_U32_PTR_TYPE here because problem with forward declaration !!!!
    EDD_COMMON_MEM_U32_PTR_TYPE         pNrtApiCtrlCommonCode;                  // EDDP_HAL_LOCAL_APICTRL_NRT_COMMON_PTR_TYPE        ApiCtrlRAM HW filtercode 
    EDD_COMMON_MEM_U32_PTR_TYPE         pNrtApiCtrlIfaceMgm;                    // EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE        ApiCtrlRAM static parts for Mgm
    EDD_COMMON_MEM_U32_PTR_TYPE         pNrtApiCtrlIfaceDcpFilter;              // host address for Dcp Filter Strings in ApiCtrlRAM 
    EDD_COMMON_MEM_U32_PTR_TYPE         pNrtApiCtrlIfaceDcpHelloFilter;         // host address for DcpHello Filter Strings in ApiCtrlRAM
    LSA_UINT8                           DcpFilterCnt;                           // nr of DCP strings available in HAL-Layout for ports
    LSA_UINT8                           DcpHelloFilterCnt;                      // nr of DCP-HELLO strings available in HAL-Layout
    LSA_UINT32                          UsedTxRingMask;                         // '1' in the mask reprasents used ring
    EDDP_NRT_TX_RING_TYPE               TxHwDescrRingMgm[EDDP_HW_MAX_TX_RINGS]; // mgmt data for Send DMACW ring
    LSA_UINT32                          UsedRxRingMask;                         // '1' in the mask reprasents used ring
    EDDP_NRT_RX_RING_TYPE               RxHwDescrRingMgm[EDDP_HW_MAX_RX_RINGS]; // mgmt data for Receive DMACW ring
    LSA_UINT32                          HandleCnt;                              // nr of handles using this NRT interface
    EDDP_NRT_TX_MGM_TYPE                TxQueue[EDDP_NRT_TX_GRP_CNT];           // TX Queues
    EDDP_NRT_RX_FILTER_MGM_TYPE         FilterInfo[EDDP_NRT_MAX_FDC_CNT];       // Filter infos
    EDDP_IFSTAT_NRT_TYPE                IfaceStatus;                            // current status of this interface

    EDDP_NRT_DCP_HELLO_FILTER_MGM_PTR   pDCPHelloFilter;                        // DCP-Hello Filters for (for EDD_NRT_FRAME_DCP_HELLO)
#if (EDDP_NRT_RXTX_COUNTER_SUPPORT == 1)
    EDDP_NRT_STAT_TYPE                   Stats;                                  // some interface rx/tx statistics
#endif
    LSA_UINT16                          FilterTLVCnt;                           // nr of TLVs used by EDDP
    LSA_HANDLE_TYPE                     FilterTLVHandle;                        // handle that activates the FilterTLV's
    EDDP_NRT_FILL_TYPE                  FILL;                                   // FILL management
} EDDP_DDB_NRT_TYPE;




/*---------------------------------------------------------------------------*/
/* Managment information for channel                                         */
/* in eddp_dev.h  EDDP_HDB_TYPE.EDDP_LOCAL_HDB_NRT_PTR_TYPE (pNRT)           */
/*---------------------------------------------------------------------------*/

//typedef struct _EDDP_HDB_NRT_TYPE       EDDP_LOCAL_MEM_ATTR *    EDDP_LOCAL_HDB_NRT_PTR_TYPE;
typedef struct _EDDP_HDB_NRT_TYPE
{   
    LSA_UINT32                  UsrFrameFilter;     // FrameFilter selected by OpenChannel()
    EDDP_RQB_QUEUE_TYPE         RxUsrQueue;         // Receive queue --> has internal an item Count
    LSA_UINT32                  TxQueueReqCnt;      // only items of this channel pending for transmit progressing (in SW queue)
    LSA_UINT32                  TxInProgCnt;        // only items of this channel in transmit progress (in HW queue)
    LSA_BOOL                    CancelInProgress;   // cancel for this channel
    EDDP_RQB_QUEUE_TYPE         TxCancelQueue;      // Tx to be canceled queue
    //EDD_INTERNAL_REQUEST_TYPE CancelRQB;        // Internal CANCEL-RQB
    EDD_UPPER_RQB_PTR_TYPE      pUserCancelRQB;     // User     CANCEL-RQB ptr
    EDDP_LOCAL_DDB_NRT_PTR_TYPE pNrtDDB;            // common nrt mgm
    LSA_HANDLE_TYPE             __Handle;             // idx of this ptr in core
    EDD_HANDLE_LOWER_TYPE       EDD_Handle;         // 
} EDDP_HDB_NRT_TYPE;




/*===========================================================================*/
/*                                  data                                     */
/*===========================================================================*/


/*===========================================================================*/
/*                                 macros                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                prototyping                                */
/*===========================================================================*/




/*****************************************************************************/
/*  end of file EDDP_NRT_INC.H                                               */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_NRT_INC_H */
