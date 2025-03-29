#ifndef EDDP_HWF_VAL_H                   /* ----- reinclude-protection ----- */
#define EDDP_HWF_VAL_H

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
/*  F i l e               &F: eddp_hwf_val.h                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  xx.xx.xx    xx    initial version.                                       */
#endif
/*****************************************************************************/

#include "eddp_hwf.h"

#define EDDP_FDC_DROP_MASK 0x8000                     // ; 0=accept, 1=drop
#ifdef EDDP_HWF_FDC_DROP_MASK
    #if (EDDP_FDC_DROP_MASK != EDDP_HWF_FDC_DROP_MASK)
        #error "EDDP_FDC_DROP_MASK != EDDP_HWF_FDC_DROP_MASK"
    #endif
#endif

#define EDDP_FDC_GRP_MASK 0x3F0                       // ; mask for FDC group --> same with RX ring index
#ifdef EDDP_HWF_FDC_GRP_MASK
    #if (EDDP_FDC_GRP_MASK != EDDP_HWF_FDC_GRP_MASK)
        #error "EDDP_FDC_GRP_MASK != EDDP_HWF_FDC_GRP_MASK"
    #endif
#endif

#define EDDP_FDC_SUB_GRP_MASK 0xF                     // ; mask for FDC sub group
#ifdef EDDP_HWF_FDC_SUB_GRP_MASK
    #if (EDDP_FDC_SUB_GRP_MASK != EDDP_HWF_FDC_SUB_GRP_MASK)
        #error "EDDP_FDC_SUB_GRP_MASK != EDDP_HWF_FDC_SUB_GRP_MASK"
    #endif
#endif
// ********************************************************************************
#define EDDP_FDC_OTHER_GRP 0x0
#ifdef EDDP_HWF_FDC_OTHER_GRP
    #if (EDDP_FDC_OTHER_GRP != EDDP_HWF_FDC_OTHER_GRP)
        #error "EDDP_FDC_OTHER_GRP != EDDP_HWF_FDC_OTHER_GRP"
    #endif
#endif

#define EDDP_FDC_OTHER_DROP 0x8000                    // ; 1000.0000.0000.0000: FilterDecisionCode for OTHER drop
#ifdef EDDP_HWF_FDC_OTHER_DROP
    #if (EDDP_FDC_OTHER_DROP != EDDP_HWF_FDC_OTHER_DROP)
        #error "EDDP_FDC_OTHER_DROP != EDDP_HWF_FDC_OTHER_DROP"
    #endif
#endif

#define EDDP_FDC_OTHER 0x0                            // ; 0000.0000.0000.0000: FilterDecisionCode for OTHER (not used)
#ifdef EDDP_HWF_FDC_OTHER
    #if (EDDP_FDC_OTHER != EDDP_HWF_FDC_OTHER)
        #error "EDDP_FDC_OTHER != EDDP_HWF_FDC_OTHER"
    #endif
#endif

// ********************************************************************************
#define EDDP_FDC_IP_UDP_GRP 0x10
#ifdef EDDP_HWF_FDC_IP_UDP_GRP
    #if (EDDP_FDC_IP_UDP_GRP != EDDP_HWF_FDC_IP_UDP_GRP)
        #error "EDDP_FDC_IP_UDP_GRP != EDDP_HWF_FDC_IP_UDP_GRP"
    #endif
#endif

#define EDDP_FDC_IP_UDP_DROP 0x8010                   // ; 1000.0000.0001.0000: FilterDecisionCode for IP drop
#ifdef EDDP_HWF_FDC_IP_UDP_DROP
    #if (EDDP_FDC_IP_UDP_DROP != EDDP_HWF_FDC_IP_UDP_DROP)
        #error "EDDP_FDC_IP_UDP_DROP != EDDP_HWF_FDC_IP_UDP_DROP"
    #endif
#endif

#define EDDP_FDC_IP_ICMP 0x10                         // ; 0000.0000.0001.0000: FilterDecisionCode for IP_ICMP
#ifdef EDDP_HWF_FDC_IP_ICMP
    #if (EDDP_FDC_IP_ICMP != EDDP_HWF_FDC_IP_ICMP)
        #error "EDDP_FDC_IP_ICMP != EDDP_HWF_FDC_IP_ICMP"
    #endif
#endif

#define EDDP_FDC_IP_IGMP 0x11                         // ; 0000.0000.0001.0001: FilterDecisionCode for IP_IGMP (not used)
#ifdef EDDP_HWF_FDC_IP_IGMP
    #if (EDDP_FDC_IP_IGMP != EDDP_HWF_FDC_IP_IGMP)
        #error "EDDP_FDC_IP_IGMP != EDDP_HWF_FDC_IP_IGMP"
    #endif
#endif

#define EDDP_FDC_IP_TCP 0x12                          // ; 0000.0000.0001.0010: FilterDecisionCode for IP_TCP
#ifdef EDDP_HWF_FDC_IP_TCP
    #if (EDDP_FDC_IP_TCP != EDDP_HWF_FDC_IP_TCP)
        #error "EDDP_FDC_IP_TCP != EDDP_HWF_FDC_IP_TCP"
    #endif
#endif

#define EDDP_FDC_IP_UDP 0x13                          // ; 0000.0000.0001.0011: FilterDecisionCode for IP_UDP
#ifdef EDDP_HWF_FDC_IP_UDP
    #if (EDDP_FDC_IP_UDP != EDDP_HWF_FDC_IP_UDP)
        #error "EDDP_FDC_IP_UDP != EDDP_HWF_FDC_IP_UDP"
    #endif
#endif

#define EDDP_FDC_IP_OTHER 0x14                        // ; 0000.0000.0001.0100: FilterDecisionCode for IP_OTHER (not used)
#ifdef EDDP_HWF_FDC_IP_OTHER
    #if (EDDP_FDC_IP_OTHER != EDDP_HWF_FDC_IP_OTHER)
        #error "EDDP_FDC_IP_OTHER != EDDP_HWF_FDC_IP_OTHER"
    #endif
#endif

#define EDDP_FDC_UDP_DCP 0x15                         // ; 0000.0000.0001.0101: FilterDecisionCode for UDP_DCP
#ifdef EDDP_HWF_FDC_UDP_DCP
    #if (EDDP_FDC_UDP_DCP != EDDP_HWF_FDC_UDP_DCP)
        #error "EDDP_FDC_UDP_DCP != EDDP_HWF_FDC_UDP_DCP"
    #endif
#endif

#define EDDP_FDC_UDP_DCP_HELLO 0x16                   // ; 0000.0000.0001.0110: FilterDecisionCode for UDP_DCP_HELLO
#ifdef EDDP_HWF_FDC_UDP_DCP_HELLO
    #if (EDDP_FDC_UDP_DCP_HELLO != EDDP_HWF_FDC_UDP_DCP_HELLO)
        #error "EDDP_FDC_UDP_DCP_HELLO != EDDP_HWF_FDC_UDP_DCP_HELLO"
    #endif
#endif

#define EDDP_FDC_UDP_ASRT 0x17                        // ; 0000.0000.0001.0111: FilterDecisionCode for UDP_ASRT
#ifdef EDDP_HWF_FDC_UDP_ASRT
    #if (EDDP_FDC_UDP_ASRT != EDDP_HWF_FDC_UDP_ASRT)
        #error "EDDP_FDC_UDP_ASRT != EDDP_HWF_FDC_UDP_ASRT"
    #endif
#endif

// ********************************************************************************
#define EDDP_FDC_ARP_GRP 0x20
#ifdef EDDP_HWF_FDC_ARP_GRP
    #if (EDDP_FDC_ARP_GRP != EDDP_HWF_FDC_ARP_GRP)
        #error "EDDP_FDC_ARP_GRP != EDDP_HWF_FDC_ARP_GRP"
    #endif
#endif

#define EDDP_FDC_ARP_DROP 0x8020                      // ; 1000.0000.0010.0000: FilterDecisionCode for ARP drop
#ifdef EDDP_HWF_FDC_ARP_DROP
    #if (EDDP_FDC_ARP_DROP != EDDP_HWF_FDC_ARP_DROP)
        #error "EDDP_FDC_ARP_DROP != EDDP_HWF_FDC_ARP_DROP"
    #endif
#endif

#define EDDP_FDC_ARP 0x20                             // ; 0000.0000.0010.0000: FilterDecisionCode for ARP pass
#ifdef EDDP_HWF_FDC_ARP
    #if (EDDP_FDC_ARP != EDDP_HWF_FDC_ARP)
        #error "EDDP_FDC_ARP != EDDP_HWF_FDC_ARP"
    #endif
#endif

// ********************************************************************************
#define EDDP_FDC_MRP_GRP 0x30
#ifdef EDDP_HWF_FDC_MRP_GRP
    #if (EDDP_FDC_MRP_GRP != EDDP_HWF_FDC_MRP_GRP)
        #error "EDDP_FDC_MRP_GRP != EDDP_HWF_FDC_MRP_GRP"
    #endif
#endif

#define EDDP_FDC_MRP_DROP 0x8030                      // ; 1000.0000.0011.0000: FilterDecisionCode for MRP drop
#ifdef EDDP_HWF_FDC_MRP_DROP
    #if (EDDP_FDC_MRP_DROP != EDDP_HWF_FDC_MRP_DROP)
        #error "EDDP_FDC_MRP_DROP != EDDP_HWF_FDC_MRP_DROP"
    #endif
#endif

#define EDDP_FDC_MRP 0x30                             // ; 0000.0000.0011.0000: FilterDecisionCode for MRP pass
#ifdef EDDP_HWF_FDC_MRP
    #if (EDDP_FDC_MRP != EDDP_HWF_FDC_MRP)
        #error "EDDP_FDC_MRP != EDDP_HWF_FDC_MRP"
    #endif
#endif

// ********************************************************************************
#define EDDP_FDC_LLDP_GRP 0x40
#ifdef EDDP_HWF_FDC_LLDP_GRP
    #if (EDDP_FDC_LLDP_GRP != EDDP_HWF_FDC_LLDP_GRP)
        #error "EDDP_FDC_LLDP_GRP != EDDP_HWF_FDC_LLDP_GRP"
    #endif
#endif

#define EDDP_FDC_LLDP 0x40                            // ; 0000.0000.0100.0000: FilterDecisionCode for LLDP pass
#ifdef EDDP_HWF_FDC_LLDP
    #if (EDDP_FDC_LLDP != EDDP_HWF_FDC_LLDP)
        #error "EDDP_FDC_LLDP != EDDP_HWF_FDC_LLDP"
    #endif
#endif

// ********************************************************************************
#define EDDP_FDC_DCP_GRP 0x50
#ifdef EDDP_HWF_FDC_DCP_GRP
    #if (EDDP_FDC_DCP_GRP != EDDP_HWF_FDC_DCP_GRP)
        #error "EDDP_FDC_DCP_GRP != EDDP_HWF_FDC_DCP_GRP"
    #endif
#endif

#define EDDP_FDC_DCP_DROP 0x8050                      // ; 1000.0000.0101.0000: FilterDecisionCode for DCP drop
#ifdef EDDP_HWF_FDC_DCP_DROP
    #if (EDDP_FDC_DCP_DROP != EDDP_HWF_FDC_DCP_DROP)
        #error "EDDP_FDC_DCP_DROP != EDDP_HWF_FDC_DCP_DROP"
    #endif
#endif

#define EDDP_FDC_DCP 0x50                             // ; 0000.0000.0101.0000: FilterDecisionCode for DCP
#ifdef EDDP_HWF_FDC_DCP
    #if (EDDP_FDC_DCP != EDDP_HWF_FDC_DCP)
        #error "EDDP_FDC_DCP != EDDP_HWF_FDC_DCP"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO 0x51                       // ; 0000.0000.0101.0001: FilterDecisionCode for DCP_HELLO, HelloFilter 1 
#ifdef EDDP_HWF_FDC_DCP_HELLO
    #if (EDDP_FDC_DCP_HELLO != EDDP_HWF_FDC_DCP_HELLO)
        #error "EDDP_FDC_DCP_HELLO != EDDP_HWF_FDC_DCP_HELLO"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO2 0x451                     // ; 0000.0100.0101.0010: FilterDecisionCode for DCP_HELLO, HelloFilter 2 
#ifdef EDDP_HWF_FDC_DCP_HELLO2
    #if (EDDP_FDC_DCP_HELLO2 != EDDP_HWF_FDC_DCP_HELLO2)
        #error "EDDP_FDC_DCP_HELLO2 != EDDP_HWF_FDC_DCP_HELLO2"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO3 0x851                     // ; 0000.1000.0101.0011: FilterDecisionCode for DCP_HELLO, HelloFilter 3 
#ifdef EDDP_HWF_FDC_DCP_HELLO3
    #if (EDDP_FDC_DCP_HELLO3 != EDDP_HWF_FDC_DCP_HELLO3)
        #error "EDDP_FDC_DCP_HELLO3 != EDDP_HWF_FDC_DCP_HELLO3"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO4 0xC51                     // ; 0000.1100.0101.0100: FilterDecisionCode for DCP_HELLO, HelloFilter 4 
#ifdef EDDP_HWF_FDC_DCP_HELLO4
    #if (EDDP_FDC_DCP_HELLO4 != EDDP_HWF_FDC_DCP_HELLO4)
        #error "EDDP_FDC_DCP_HELLO4 != EDDP_HWF_FDC_DCP_HELLO4"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO5 0x1051                    // ; 0001.0000.0101.0100: FilterDecisionCode for DCP_HELLO, HelloFilter 5
#ifdef EDDP_HWF_FDC_DCP_HELLO5
    #if (EDDP_FDC_DCP_HELLO5 != EDDP_HWF_FDC_DCP_HELLO5)
        #error "EDDP_FDC_DCP_HELLO5 != EDDP_HWF_FDC_DCP_HELLO5"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO6 0x1451                    // ; 0001.0100.0101.0100: FilterDecisionCode for DCP_HELLO, HelloFilter 6 
#ifdef EDDP_HWF_FDC_DCP_HELLO6
    #if (EDDP_FDC_DCP_HELLO6 != EDDP_HWF_FDC_DCP_HELLO6)
        #error "EDDP_FDC_DCP_HELLO6 != EDDP_HWF_FDC_DCP_HELLO6"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO7 0x1851                    // ; 0001.1000.0101.0100: FilterDecisionCode for DCP_HELLO, HelloFilter 7
#ifdef EDDP_HWF_FDC_DCP_HELLO7
    #if (EDDP_FDC_DCP_HELLO7 != EDDP_HWF_FDC_DCP_HELLO7)
        #error "EDDP_FDC_DCP_HELLO7 != EDDP_HWF_FDC_DCP_HELLO7"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO8 0x1C51                    // ; 0001.1100.0101.0100: FilterDecisionCode for DCP_HELLO, HelloFilter 8 
#ifdef EDDP_HWF_FDC_DCP_HELLO8
    #if (EDDP_FDC_DCP_HELLO8 != EDDP_HWF_FDC_DCP_HELLO8)
        #error "EDDP_FDC_DCP_HELLO8 != EDDP_HWF_FDC_DCP_HELLO8"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO9 0x2051                    // ; 0010.0000.0101.0100: FilterDecisionCode for DCP_HELLO, HelloFilter 9 
#ifdef EDDP_HWF_FDC_DCP_HELLO9
    #if (EDDP_FDC_DCP_HELLO9 != EDDP_HWF_FDC_DCP_HELLO9)
        #error "EDDP_FDC_DCP_HELLO9 != EDDP_HWF_FDC_DCP_HELLO9"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO10 0x2451                   // ; 0010.0100.0101.0100: FilterDecisionCode for DCP_HELLO, HelloFilter 10 
#ifdef EDDP_HWF_FDC_DCP_HELLO10
    #if (EDDP_FDC_DCP_HELLO10 != EDDP_HWF_FDC_DCP_HELLO10)
        #error "EDDP_FDC_DCP_HELLO10 != EDDP_HWF_FDC_DCP_HELLO10"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO11 0x2851                   // ; 0010.1000.0101.0100: FilterDecisionCode for DCP_HELLO, HelloFilter 11 
#ifdef EDDP_HWF_FDC_DCP_HELLO11
    #if (EDDP_FDC_DCP_HELLO11 != EDDP_HWF_FDC_DCP_HELLO11)
        #error "EDDP_FDC_DCP_HELLO11 != EDDP_HWF_FDC_DCP_HELLO11"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO12 0x2C51                   // ; 0010.1100.0101.0100: FilterDecisionCode for DCP_HELLO, HelloFilter 12 
#ifdef EDDP_HWF_FDC_DCP_HELLO12
    #if (EDDP_FDC_DCP_HELLO12 != EDDP_HWF_FDC_DCP_HELLO12)
        #error "EDDP_FDC_DCP_HELLO12 != EDDP_HWF_FDC_DCP_HELLO12"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO13 0x3051                   // ; 0011.0000.0101.0100: FilterDecisionCode for DCP_HELLO, HelloFilter 13 
#ifdef EDDP_HWF_FDC_DCP_HELLO13
    #if (EDDP_FDC_DCP_HELLO13 != EDDP_HWF_FDC_DCP_HELLO13)
        #error "EDDP_FDC_DCP_HELLO13 != EDDP_HWF_FDC_DCP_HELLO13"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO14 0x3451                   // ; 0011.0100.0101.0100: FilterDecisionCode for DCP_HELLO, HelloFilter 14 
#ifdef EDDP_HWF_FDC_DCP_HELLO14
    #if (EDDP_FDC_DCP_HELLO14 != EDDP_HWF_FDC_DCP_HELLO14)
        #error "EDDP_FDC_DCP_HELLO14 != EDDP_HWF_FDC_DCP_HELLO14"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO15 0x3851                   // ; 0011.1000.0101.0100: FilterDecisionCode for DCP_HELLO, HelloFilter 15 
#ifdef EDDP_HWF_FDC_DCP_HELLO15
    #if (EDDP_FDC_DCP_HELLO15 != EDDP_HWF_FDC_DCP_HELLO15)
        #error "EDDP_FDC_DCP_HELLO15 != EDDP_HWF_FDC_DCP_HELLO15"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO16 0x3C51                   // ; 0011.1100.0101.0100: FilterDecisionCode for DCP_HELLO, HelloFilter 16 
#ifdef EDDP_HWF_FDC_DCP_HELLO16
    #if (EDDP_FDC_DCP_HELLO16 != EDDP_HWF_FDC_DCP_HELLO16)
        #error "EDDP_FDC_DCP_HELLO16 != EDDP_HWF_FDC_DCP_HELLO16"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO17 0x4051                   // ; 0100.0000.0101.0100: FilterDecisionCode for DCP_HELLO, HelloFilter 17 
#ifdef EDDP_HWF_FDC_DCP_HELLO17
    #if (EDDP_FDC_DCP_HELLO17 != EDDP_HWF_FDC_DCP_HELLO17)
        #error "EDDP_FDC_DCP_HELLO17 != EDDP_HWF_FDC_DCP_HELLO17"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO18 0x4451                   // ; 0100.0100.0101.0010: FilterDecisionCode for DCP_HELLO, HelloFilter 18 
#ifdef EDDP_HWF_FDC_DCP_HELLO18
    #if (EDDP_FDC_DCP_HELLO18 != EDDP_HWF_FDC_DCP_HELLO18)
        #error "EDDP_FDC_DCP_HELLO18 != EDDP_HWF_FDC_DCP_HELLO18"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO19 0x4851                   // ; 0100.1000.0101.0011: FilterDecisionCode for DCP_HELLO, HelloFilter 19 
#ifdef EDDP_HWF_FDC_DCP_HELLO19
    #if (EDDP_FDC_DCP_HELLO19 != EDDP_HWF_FDC_DCP_HELLO19)
        #error "EDDP_FDC_DCP_HELLO19 != EDDP_HWF_FDC_DCP_HELLO19"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO20 0x4C51                   // ; 0100.1100.0101.0100: FilterDecisionCode for DCP_HELLO, HelloFilter 20 
#ifdef EDDP_HWF_FDC_DCP_HELLO20
    #if (EDDP_FDC_DCP_HELLO20 != EDDP_HWF_FDC_DCP_HELLO20)
        #error "EDDP_FDC_DCP_HELLO20 != EDDP_HWF_FDC_DCP_HELLO20"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO21 0x5051                   // ; 0101.0000.0101.0100: FilterDecisionCode for DCP_HELLO, HelloFilter 21
#ifdef EDDP_HWF_FDC_DCP_HELLO21
    #if (EDDP_FDC_DCP_HELLO21 != EDDP_HWF_FDC_DCP_HELLO21)
        #error "EDDP_FDC_DCP_HELLO21 != EDDP_HWF_FDC_DCP_HELLO21"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO22 0x5451                   // ; 0101.0100.0101.0100: FilterDecisionCode for DCP_HELLO, HelloFilter 22 
#ifdef EDDP_HWF_FDC_DCP_HELLO22
    #if (EDDP_FDC_DCP_HELLO22 != EDDP_HWF_FDC_DCP_HELLO22)
        #error "EDDP_FDC_DCP_HELLO22 != EDDP_HWF_FDC_DCP_HELLO22"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO23 0x5851                   // ; 0101.1000.0101.0100: FilterDecisionCode for DCP_HELLO, HelloFilter 23
#ifdef EDDP_HWF_FDC_DCP_HELLO23
    #if (EDDP_FDC_DCP_HELLO23 != EDDP_HWF_FDC_DCP_HELLO23)
        #error "EDDP_FDC_DCP_HELLO23 != EDDP_HWF_FDC_DCP_HELLO23"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO24 0x5C51                   // ; 0101.1100.0101.0100: FilterDecisionCode for DCP_HELLO, HelloFilter 24 
#ifdef EDDP_HWF_FDC_DCP_HELLO24
    #if (EDDP_FDC_DCP_HELLO24 != EDDP_HWF_FDC_DCP_HELLO24)
        #error "EDDP_FDC_DCP_HELLO24 != EDDP_HWF_FDC_DCP_HELLO24"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO25 0x6051                   // ; 0110.0000.0101.0100: FilterDecisionCode for DCP_HELLO, HelloFilter 25
#ifdef EDDP_HWF_FDC_DCP_HELLO25
    #if (EDDP_FDC_DCP_HELLO25 != EDDP_HWF_FDC_DCP_HELLO25)
        #error "EDDP_FDC_DCP_HELLO25 != EDDP_HWF_FDC_DCP_HELLO25"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO26 0x6451                   // ; 0110.0100.0101.0100: FilterDecisionCode for DCP_HELLO, HelloFilter 26
#ifdef EDDP_HWF_FDC_DCP_HELLO26
    #if (EDDP_FDC_DCP_HELLO26 != EDDP_HWF_FDC_DCP_HELLO26)
        #error "EDDP_FDC_DCP_HELLO26 != EDDP_HWF_FDC_DCP_HELLO26"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO27 0x6851                   // ; 0110.1000.0101.0100: FilterDecisionCode for DCP_HELLO, HelloFilter 27 
#ifdef EDDP_HWF_FDC_DCP_HELLO27
    #if (EDDP_FDC_DCP_HELLO27 != EDDP_HWF_FDC_DCP_HELLO27)
        #error "EDDP_FDC_DCP_HELLO27 != EDDP_HWF_FDC_DCP_HELLO27"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO28 0x6C51                   // ; 0110.1100.0101.0100: FilterDecisionCode for DCP_HELLO, HelloFilter 28 
#ifdef EDDP_HWF_FDC_DCP_HELLO28
    #if (EDDP_FDC_DCP_HELLO28 != EDDP_HWF_FDC_DCP_HELLO28)
        #error "EDDP_FDC_DCP_HELLO28 != EDDP_HWF_FDC_DCP_HELLO28"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO29 0x7051                   // ; 0111.0000.0101.0100: FilterDecisionCode for DCP_HELLO, HelloFilter 29 
#ifdef EDDP_HWF_FDC_DCP_HELLO29
    #if (EDDP_FDC_DCP_HELLO29 != EDDP_HWF_FDC_DCP_HELLO29)
        #error "EDDP_FDC_DCP_HELLO29 != EDDP_HWF_FDC_DCP_HELLO29"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO30 0x7451                   // ; 0111.0100.0101.0100: FilterDecisionCode for DCP_HELLO, HelloFilter 30 
#ifdef EDDP_HWF_FDC_DCP_HELLO30
    #if (EDDP_FDC_DCP_HELLO30 != EDDP_HWF_FDC_DCP_HELLO30)
        #error "EDDP_FDC_DCP_HELLO30 != EDDP_HWF_FDC_DCP_HELLO30"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO31 0x7851                   // ; 0111.1000.0101.0100: FilterDecisionCode for DCP_HELLO, HelloFilter 31 
#ifdef EDDP_HWF_FDC_DCP_HELLO31
    #if (EDDP_FDC_DCP_HELLO31 != EDDP_HWF_FDC_DCP_HELLO31)
        #error "EDDP_FDC_DCP_HELLO31 != EDDP_HWF_FDC_DCP_HELLO31"
    #endif
#endif

#define EDDP_FDC_DCP_HELLO32 0x7C51                   // ; 0111.1100.0101.0100: FilterDecisionCode for DCP_HELLO, HelloFilter 32 
#ifdef EDDP_HWF_FDC_DCP_HELLO32
    #if (EDDP_FDC_DCP_HELLO32 != EDDP_HWF_FDC_DCP_HELLO32)
        #error "EDDP_FDC_DCP_HELLO32 != EDDP_HWF_FDC_DCP_HELLO32"
    #endif
#endif

// ********************************************************************************
#define EDDP_FDC_ASRT_GRP 0x60
#ifdef EDDP_HWF_FDC_ASRT_GRP
    #if (EDDP_FDC_ASRT_GRP != EDDP_HWF_FDC_ASRT_GRP)
        #error "EDDP_FDC_ASRT_GRP != EDDP_HWF_FDC_ASRT_GRP"
    #endif
#endif

#define EDDP_FDC_ASRT 0x60                            // ; 0000.0000.0110.0000: FilterDecisionCode for ASRT pass (AlarmSoftRealTime)
#ifdef EDDP_HWF_FDC_ASRT
    #if (EDDP_FDC_ASRT != EDDP_HWF_FDC_ASRT)
        #error "EDDP_FDC_ASRT != EDDP_HWF_FDC_ASRT"
    #endif
#endif

// ********************************************************************************
#define EDDP_FDC_LEN_GRP 0x70
#ifdef EDDP_HWF_FDC_LEN_GRP
    #if (EDDP_FDC_LEN_GRP != EDDP_HWF_FDC_LEN_GRP)
        #error "EDDP_FDC_LEN_GRP != EDDP_HWF_FDC_LEN_GRP"
    #endif
#endif

#define EDDP_FDC_LEN_DROP 0x8070                      // ; 1000.0000.0111.0000: FilterDecisionCode for LEN drop
#ifdef EDDP_HWF_FDC_LEN_DROP
    #if (EDDP_FDC_LEN_DROP != EDDP_HWF_FDC_LEN_DROP)
        #error "EDDP_FDC_LEN_DROP != EDDP_HWF_FDC_LEN_DROP"
    #endif
#endif

#define EDDP_FDC_LEN_DCP 0x70                         // ; 0000.0000.0111.0000: FilterDecisionCode for LEN_DCP
#ifdef EDDP_HWF_FDC_LEN_DCP
    #if (EDDP_FDC_LEN_DCP != EDDP_HWF_FDC_LEN_DCP)
        #error "EDDP_FDC_LEN_DCP != EDDP_HWF_FDC_LEN_DCP"
    #endif
#endif

#define EDDP_FDC_LEN_TSYNC 0x71                       // ; 0000.0000.0111.0001: FilterDecisionCode for LEN_TSYNC
#ifdef EDDP_HWF_FDC_LEN_TSYNC
    #if (EDDP_FDC_LEN_TSYNC != EDDP_HWF_FDC_LEN_TSYNC)
        #error "EDDP_FDC_LEN_TSYNC != EDDP_HWF_FDC_LEN_TSYNC"
    #endif
#endif

#define EDDP_FDC_LEN_STDBY 0x72                       // ; 0000.0000.0111.0010: FilterDecisionCode for LEN_STDBY
#ifdef EDDP_HWF_FDC_LEN_STDBY
    #if (EDDP_FDC_LEN_STDBY != EDDP_HWF_FDC_LEN_STDBY)
        #error "EDDP_FDC_LEN_STDBY != EDDP_HWF_FDC_LEN_STDBY"
    #endif
#endif

#define EDDP_FDC_LEN_HSR 0x73                         // ; 0000.0000.0111.0011: FilterDecisionCode for LEN_HSR SNAP header
#ifdef EDDP_HWF_FDC_LEN_HSR
    #if (EDDP_FDC_LEN_HSR != EDDP_HWF_FDC_LEN_HSR)
        #error "EDDP_FDC_LEN_HSR != EDDP_HWF_FDC_LEN_HSR"
    #endif
#endif

#define EDDP_FDC_LEN_SINEC_FWL 0x74                   // ; 0000.0000.0111.0100: FilterDecisionCode for LEN_SINEC_FWL SNAP header
#ifdef EDDP_HWF_FDC_LEN_SINEC_FWL
    #if (EDDP_FDC_LEN_SINEC_FWL != EDDP_HWF_FDC_LEN_SINEC_FWL)
        #error "EDDP_FDC_LEN_SINEC_FWL != EDDP_HWF_FDC_LEN_SINEC_FWL"
    #endif
#endif

#define EDDP_FDC_LEN_OTHER 0x75                       // ; 0000.0000.0111.0101: FilterDecisionCode for LEN_OTHER (not used)
#ifdef EDDP_HWF_FDC_LEN_OTHER
    #if (EDDP_FDC_LEN_OTHER != EDDP_HWF_FDC_LEN_OTHER)
        #error "EDDP_FDC_LEN_OTHER != EDDP_HWF_FDC_LEN_OTHER"
    #endif
#endif

// ********************************************************************************
#define EDDP_FDC_PTCP_GRP 0x80
#ifdef EDDP_HWF_FDC_PTCP_GRP
    #if (EDDP_FDC_PTCP_GRP != EDDP_HWF_FDC_PTCP_GRP)
        #error "EDDP_FDC_PTCP_GRP != EDDP_HWF_FDC_PTCP_GRP"
    #endif
#endif

#define EDDP_FDC_PTCP_DROP 0x8080                     // ; 1000.0000.1000.0000: EDDP FilterDecisionCode for PTCP drop
#ifdef EDDP_HWF_FDC_PTCP_DROP
    #if (EDDP_FDC_PTCP_DROP != EDDP_HWF_FDC_PTCP_DROP)
        #error "EDDP_FDC_PTCP_DROP != EDDP_HWF_FDC_PTCP_DROP"
    #endif
#endif

#define EDDP_FDC_PTCP_ANNOUNCE 0x80                   // ; 0000.0000.1000.0000: EDDP FilterDecisionCode for PTCP_ANNOUNCE
#ifdef EDDP_HWF_FDC_PTCP_ANNOUNCE
    #if (EDDP_FDC_PTCP_ANNOUNCE != EDDP_HWF_FDC_PTCP_ANNOUNCE)
        #error "EDDP_FDC_PTCP_ANNOUNCE != EDDP_HWF_FDC_PTCP_ANNOUNCE"
    #endif
#endif

#define EDDP_FDC_PTCP_K32_SYNC_0 0x81                 // ; 0000.0000.1000.0001: K32  FilterDecisionCode for PTCP_SYNC Clock 
#ifdef EDDP_HWF_FDC_PTCP_K32_SYNC_0
    #if (EDDP_FDC_PTCP_K32_SYNC_0 != EDDP_HWF_FDC_PTCP_K32_SYNC_0)
        #error "EDDP_FDC_PTCP_K32_SYNC_0 != EDDP_HWF_FDC_PTCP_K32_SYNC_0"
    #endif
#endif

#define EDDP_FDC_PTCP_K32_SYNC_1 0x82                 // ; 0000.0000.1000.0010: K32  FilterDecisionCode for PTCP_SYNC Time
#ifdef EDDP_HWF_FDC_PTCP_K32_SYNC_1
    #if (EDDP_FDC_PTCP_K32_SYNC_1 != EDDP_HWF_FDC_PTCP_K32_SYNC_1)
        #error "EDDP_FDC_PTCP_K32_SYNC_1 != EDDP_HWF_FDC_PTCP_K32_SYNC_1"
    #endif
#endif

#define EDDP_FDC_PTCP_K32_DELAY 0x83                  // ; 0000.0000.1000.0011: K32  FilterDecisionCode for PTCP_DELAY
#ifdef EDDP_HWF_FDC_PTCP_K32_DELAY
    #if (EDDP_FDC_PTCP_K32_DELAY != EDDP_HWF_FDC_PTCP_K32_DELAY)
        #error "EDDP_FDC_PTCP_K32_DELAY != EDDP_HWF_FDC_PTCP_K32_DELAY"
    #endif
#endif

#define EDDP_FDC_PTCP_K32_SYNC_0_FU 0x84              // ; 0000.0000.1000.0100: K32  FilterDecisionCode for Sync0 Follow Up Frames
#ifdef EDDP_HWF_FDC_PTCP_K32_SNC_0_FU
    #if (EDDP_FDC_PTCP_K32_SYNC_0_FU != EDDP_HWF_FDC_PTCP_K32_SNC_0_FU)
        #error "EDDP_FDC_PTCP_K32_SYNC_0_FU != EDDP_HWF_FDC_PTCP_K32_SNC_0_FU"
    #endif
#endif

#define EDDP_FDC_PTCP_K32_SYNC_1_FU 0x85              // ; 0000.0000.1000.0101: K32  FilterDecisionCode for Sync1 Follow Up Frames
#ifdef EDDP_HWF_FDC_PTCP_K32_SNC_1_FU
    #if (EDDP_FDC_PTCP_K32_SYNC_1_FU != EDDP_HWF_FDC_PTCP_K32_SNC_1_FU)
        #error "EDDP_FDC_PTCP_K32_SYNC_1_FU != EDDP_HWF_FDC_PTCP_K32_SNC_1_FU"
    #endif
#endif

#define EDDP_FDC_K32_RESERVED_2 0x86                  // ; 0000.0000.1000.0101: K32  FilterDecisionCode, reserved for gPTP (not used in K32)
#ifdef EDDP_HWF_FDC_K32_RESERVED_2
    #if (EDDP_FDC_K32_RESERVED_2 != EDDP_HWF_FDC_K32_RESERVED_2)
        #error "EDDP_FDC_K32_RESERVED_2 != EDDP_HWF_FDC_K32_RESERVED_2"
    #endif
#endif

// ********************************************************************************
#define EDDP_FDC_RT_CT_GRP 0xE0
#ifdef EDDP_HWF_FDC_RT_CT_GRP
    #if (EDDP_FDC_RT_CT_GRP != EDDP_HWF_FDC_RT_CT_GRP)
        #error "EDDP_FDC_RT_CT_GRP != EDDP_HWF_FDC_RT_CT_GRP"
    #endif
#endif

#define EDDP_FDC_RT_CT_DROP 0x80E0                    // ; 1000.0000.1110.0000: FilterDecisionCode for RT_CT drop
#ifdef EDDP_HWF_FDC_RT_CT_DROP
    #if (EDDP_FDC_RT_CT_DROP != EDDP_HWF_FDC_RT_CT_DROP)
        #error "EDDP_FDC_RT_CT_DROP != EDDP_HWF_FDC_RT_CT_DROP"
    #endif
#endif

// ********************************************************************************
#define EDDP_FDC_RT_SF_GRP 0xF0
#ifdef EDDP_HWF_FDC_RT_SF_GRP
    #if (EDDP_FDC_RT_SF_GRP != EDDP_HWF_FDC_RT_SF_GRP)
        #error "EDDP_FDC_RT_SF_GRP != EDDP_HWF_FDC_RT_SF_GRP"
    #endif
#endif

#define EDDP_FDC_RT_SF_DROP 0x80F0                    // ; 1000.0000.1111.0000: FilterDecisionCode for RT_SF drop
#ifdef EDDP_HWF_FDC_RT_SF_DROP
    #if (EDDP_FDC_RT_SF_DROP != EDDP_HWF_FDC_RT_SF_DROP)
        #error "EDDP_FDC_RT_SF_DROP != EDDP_HWF_FDC_RT_SF_DROP"
    #endif
#endif

// ********************************************************************************
#define EDDP_FDC_INVALID 0xFFFF                       // ; reserved for initialization of FDC field for debuging/testing/ect...
#ifdef EDDP_HWF_FDC_INVALID
    #if (EDDP_FDC_INVALID != EDDP_HWF_FDC_INVALID)
        #error "EDDP_FDC_INVALID != EDDP_HWF_FDC_INVALID"
    #endif
#endif


// *** AcyclicRead-IDX for PASS (DROP in monitor mode) ****************************
// *** CYCLE puts frame into ACW list without FDC,IDX  ****************************
// ********************************************************************************
#define EDDP_ARD_INDEX_DROP 0x0                       // ; DMA-CW Idx for DROP
#ifdef EDDP_HWF_ARD_INDEX_DROP
    #if (EDDP_ARD_INDEX_DROP != EDDP_HWF_ARD_INDEX_DROP)
        #error "EDDP_ARD_INDEX_DROP != EDDP_HWF_ARD_INDEX_DROP"
    #endif
#endif

#define EDDP_ARD_INDEX_OTHER 0x1                      // ; DMA-CW Idx for ?_OTHER
#ifdef EDDP_HWF_ARD_INDEX_OTHER
    #if (EDDP_ARD_INDEX_OTHER != EDDP_HWF_ARD_INDEX_OTHER)
        #error "EDDP_ARD_INDEX_OTHER != EDDP_HWF_ARD_INDEX_OTHER"
    #endif
#endif

#define EDDP_ARD_INDEX_IP_MISC 0x2                    // ; DMA-CW Idx for IP:    ICMP, IGMP, ARP
#ifdef EDDP_HWF_ARD_INDEX_IP_MISC
    #if (EDDP_ARD_INDEX_IP_MISC != EDDP_HWF_ARD_INDEX_IP_MISC)
        #error "EDDP_ARD_INDEX_IP_MISC != EDDP_HWF_ARD_INDEX_IP_MISC"
    #endif
#endif

#define EDDP_ARD_INDEX_IP_TCP 0x3                     // ; DMA-CW Idx for IP:    TCP
#ifdef EDDP_HWF_ARD_INDEX_IP_TCP
    #if (EDDP_ARD_INDEX_IP_TCP != EDDP_HWF_ARD_INDEX_IP_TCP)
        #error "EDDP_ARD_INDEX_IP_TCP != EDDP_HWF_ARD_INDEX_IP_TCP"
    #endif
#endif

#define EDDP_ARD_INDEX_IP_UDP 0x4                     // ; DMA-CW Idx for IP:    UDP
#ifdef EDDP_HWF_ARD_INDEX_IP_UDP
    #if (EDDP_ARD_INDEX_IP_UDP != EDDP_HWF_ARD_INDEX_IP_UDP)
        #error "EDDP_ARD_INDEX_IP_UDP != EDDP_HWF_ARD_INDEX_IP_UDP"
    #endif
#endif

#define EDDP_ARD_INDEX_ALARM 0x5                      // ; DMA-CW Idx for ALARM: ASRT, UDP_ASRT
#ifdef EDDP_HWF_ARD_INDEX_ALARM
    #if (EDDP_ARD_INDEX_ALARM != EDDP_HWF_ARD_INDEX_ALARM)
        #error "EDDP_ARD_INDEX_ALARM != EDDP_HWF_ARD_INDEX_ALARM"
    #endif
#endif

#define EDDP_ARD_INDEX_DCP 0x6                        // ; DMA-CW Idx for DCP:   DCP, DCP_HELLO , UDP_DCP, UDP_DCP_HELLO, LEN_DCP
#ifdef EDDP_HWF_ARD_INDEX_DCP
    #if (EDDP_ARD_INDEX_DCP != EDDP_HWF_ARD_INDEX_DCP)
        #error "EDDP_ARD_INDEX_DCP != EDDP_HWF_ARD_INDEX_DCP"
    #endif
#endif

#define EDDP_ARD_INDEX_PTCP_ANN 0x7                   // ; DMA-CW Idx for PTCP?: PTCP_ANNOUNCE
#ifdef EDDP_HWF_ARD_INDEX_PTCP_ANN
    #if (EDDP_ARD_INDEX_PTCP_ANN != EDDP_HWF_ARD_INDEX_PTCP_ANN)
        #error "EDDP_ARD_INDEX_PTCP_ANN != EDDP_HWF_ARD_INDEX_PTCP_ANN"
    #endif
#endif

#define EDDP_ARD_INDEX_LLDP 0x8                       // ; DMA-CW Idx for LLDP
#ifdef EDDP_HWF_ARD_INDEX_LLDP
    #if (EDDP_ARD_INDEX_LLDP != EDDP_HWF_ARD_INDEX_LLDP)
        #error "EDDP_ARD_INDEX_LLDP != EDDP_HWF_ARD_INDEX_LLDP"
    #endif
#endif

#define EDDP_ARD_INDEX_MRP 0x9                        // ; DMA-CW Idx for MRP:   LEN_STDBY, LEN_HSR, MRP
#ifdef EDDP_HWF_ARD_INDEX_MRP
    #if (EDDP_ARD_INDEX_MRP != EDDP_HWF_ARD_INDEX_MRP)
        #error "EDDP_ARD_INDEX_MRP != EDDP_HWF_ARD_INDEX_MRP"
    #endif
#endif

#define EDDP_ARD_INDEX_MISC 0xA                       // ; DMA-CW Idx for TSYNC/FWL: LEN_TSYNC, LEN_SINEC_FWL 
#ifdef EDDP_HWF_ARD_INDEX_MISC
    #if (EDDP_ARD_INDEX_MISC != EDDP_HWF_ARD_INDEX_MISC)
        #error "EDDP_ARD_INDEX_MISC != EDDP_HWF_ARD_INDEX_MISC"
    #endif
#endif

#define EDDP_ARD_INDEX_K32_SYNC_CLOCK 0x1D            // ; DMA-CW Idx for PTCP?: KRISC SYNC_0 (PTCP clock sync)
#ifdef EDDP_HWF_ARD_IDX_K32_SNC_CLOCK
    #if (EDDP_ARD_INDEX_K32_SYNC_CLOCK != EDDP_HWF_ARD_IDX_K32_SNC_CLOCK)
        #error "EDDP_ARD_INDEX_K32_SYNC_CLOCK != EDDP_HWF_ARD_IDX_K32_SNC_CLOCK"
    #endif
#endif

#define EDDP_ARD_INDEX_K32_SYNC_TIME 0x1E             // ; DMA-CW Idx for PTCP?: KRISC SYNC_1 (gPTP time sync tbd)
#ifdef EDDP_HWF_ARD_IDX_K32_SNC_TIME
    #if (EDDP_ARD_INDEX_K32_SYNC_TIME != EDDP_HWF_ARD_IDX_K32_SNC_TIME)
        #error "EDDP_ARD_INDEX_K32_SYNC_TIME != EDDP_HWF_ARD_IDX_K32_SNC_TIME"
    #endif
#endif

#define EDDP_ARD_INDEX_K32_SYNC_DELAY 0x1F            // ; DMA-CW Idx for PTCP?: KRISC SYNC_DELAY
#ifdef EDDP_HWF_ARD_IDX_K32_SNC_DELAY
    #if (EDDP_ARD_INDEX_K32_SYNC_DELAY != EDDP_HWF_ARD_IDX_K32_SNC_DELAY)
        #error "EDDP_ARD_INDEX_K32_SYNC_DELAY != EDDP_HWF_ARD_IDX_K32_SNC_DELAY"
    #endif
#endif

// ***** the PNIP supports 32 rings and next mask defines used rings **************
#define EDDP_ARD_HWF_USED_RINGS 0xE00007FF            // ; all RX-Rings used by this filter code
#ifdef EDDP_HWF_ARD_HWF_USED_RINGS
    #if (EDDP_ARD_HWF_USED_RINGS != EDDP_HWF_ARD_HWF_USED_RINGS)
        #error "EDDP_ARD_HWF_USED_RINGS != EDDP_HWF_ARD_HWF_USED_RINGS"
    #endif
#endif

#define EDDP_ARD_K32_USED_RINGS 0xE0000000            // ; all RX-rings handled by KRISC used by EDDP_API_TYPE_PTCP
#ifdef EDDP_HWF_ARD_K32_USED_RINGS
    #if (EDDP_ARD_K32_USED_RINGS != EDDP_HWF_ARD_K32_USED_RINGS)
        #error "EDDP_ARD_K32_USED_RINGS != EDDP_HWF_ARD_K32_USED_RINGS"
    #endif
#endif

#define EDDP_ARD_NRT_USED_RINGS 0xE00007FF                 // ; all RX-rings handled by NRT
#ifdef EDDP_HWF_ARD_NRT_USED_RINGS
    #if (EDDP_ARD_NRT_USED_RINGS != EDDP_HWF_ARD_NRT_USED_RINGS)
        #error "EDDP_ARD_NRT_USED_RINGS != EDDP_HWF_ARD_NRT_USED_RINGS"
    #endif
#endif


/*****************************************************************************/
/*  end of file eddp_hwf_val.h                                               */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_HWF_VAL_H */
