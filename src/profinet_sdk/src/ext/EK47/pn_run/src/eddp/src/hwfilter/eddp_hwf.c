
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
/*  F i l e               &F: eddp_hwf.c                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP xxxx                                        */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  xx.xx.xx    xx    initial version.                                       */
/*  generated by: HwFilterCompiler.exe, V1.00.1C                             */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID  63                 
#define EDDP_MODULE_ID     LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_NRT_HWF */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"            /* edd headerfiles */
#include "eddp_int.h"            /* internal header */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID) 

/*===========================================================================*/
/*                                global Variables                           */
/*===========================================================================*/


LSA_UINT8 eddp_hwfiltercode[1552]= {
  // EDDP_API_TYPE_OTHER       ; Drop Filter !(EtherType: 0h-800h, 806h, 8892h, 88F7h, 88CCh, 88E3h)
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 0 | 1                                          ; drop frame
  // EDDP_API_TYPE_IP          ; Filter for IP EtherType:0x0800
  0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x01, 0x00, // --> SKIP | 6                                              ; move dataptr to VLAN/EtherType
  0x00, 0x81, 0xFF, 0xFF, 0x01, 0x02, 0x00, 0x80, // --> SV_EQU | 0 | 2 | 1 | ffffh | 8100h                    ; check VLAN_TYPE and skip VLAN until not found anymore 
  0x00, 0x45, 0x00, 0xFF, 0x04, 0x00, 0x16, 0x81, // --> SV_NEQ | Lbl_NO_IP_RT_OVER_UDP=22 | 0 | 4 | ff00h | 4500h ; check for Version(4)/IHL(5) and move dataptr to TTL/Protocol if 45
  0x01, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x05, 0x80, // --> SV_EQU | Lbl_IPICMP=5 | 0 | 0 | ffh | 1h              ; check ID for ICMP (1)
  0x02, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x05, 0x80, // --> SV_EQU | Lbl_IPIGMP=5 | 0 | 0 | ffh | 2h              ; check ID for IGMP (2)
  0x06, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x05, 0x80, // --> SV_EQU | Lbl_IPTCP=5 | 0 | 0 | ffh | 6h               ; check ID for TCP  (6)
  0x11, 0x00, 0xFF, 0x00, 0x00, 0x07, 0x05, 0x80, // --> SV_EQU | Lbl_IP_UDP=5 | 7 | 0 | ffh | 11h             ; check ID for UDP  (17)
  0x01, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 20 | 1                                         ; drop frame
  0x0A, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 16 | 10                                        ; pass frame ID = ICMP (1)
  0x0A, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 17 | 10                                        ; pass frame ID = IGMP (2)
  0x03, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 18 | 3                                         ; pass frame ID = TCP  (6)
  0x92, 0x88, 0xFF, 0xFF, 0x00, 0x03, 0x02, 0x80, // --> SV_EQU | Lbl_IP_UDP_RT=2 | 3 | 0 | ffffh | 8892h      ; Dst port 8892h ?
  0x04, 0x00, 0x13, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 19 | 4                                         ; standard UDP (17)
  0x00, 0xFC, 0xFF, 0xFF, 0x00, 0x00, 0x03, 0x84, // --> SV_GTE | Lbl_IP_UDP_MORE=3 | 0 | 0 | ffffh | fc00h    ; is grp: FC00-FEFFh ?
  0x00, 0x01, 0xFF, 0xFF, 0x00, 0x00, 0x06, 0x83, // --> SV_LT | Lbl_UDP_DROP=6 | 0 | 0 | ffffh | 100h         ; is not grp: 0100-FBFFh ?
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, // --> CYCLIC | 0 | 0                                        ; Cyclic: RT_FRAME: 0100-BFFFh, RT_FRAME_UDP: C000-FBFFh
  0x01, 0xFC, 0xFF, 0xFD, 0x00, 0x00, 0x05, 0x80, // --> SV_EQU | Lbl_IP_UDP_RTA=5 | 0 | 0 | fdffh | fc01h     ; is RTA i/o Alarm: FC01h(high)/FE01h(low)
  0xFC, 0xFE, 0xFF, 0xFF, 0x00, 0x00, 0x05, 0x80, // --> SV_EQU | Lbl_IP_UDP_DCP_HELLO=5 | 0 | 0 | ffffh | fefch ; is DCP-Hello: FEFCh  
  0xFD, 0xFE, 0xFF, 0xFF, 0x00, 0x00, 0x02, 0x83, // --> SV_LT | Lbl_UDP_DROP=2 | 0 | 0 | ffffh | fefdh        ; is frame <FEFD --> drop it
  0xFF, 0xFE, 0xFF, 0xFF, 0x00, 0x00, 0x04, 0x85, // --> SV_LTE | Lbl_IP_UDP_DCP=4 | 0 | 0 | ffffh | feffh     ; is frame FEFD-FEFFh --> pass it
  0x00, 0x00, 0x10, 0x80, 0x00, 0x00, 0x00, 0x10, // --> DROP | 32784 | 0                                      ; drop frame
  0x05, 0x00, 0x17, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 23 | 5                                         ; pass frame FC01h/FE01h
  0x06, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 22 | 6                                         ; pass frame FEFCh
  0x06, 0x00, 0x15, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 21 | 6                                         ; pass frame FEFD - FEFFh
  0x00, 0x40, 0x00, 0xF0, 0x04, 0x00, 0x0A, 0x81, // --> SV_NEQ | Lbl_IP_DROP=10 | 0 | 4 | f000h | 4000h       ; check for Version(4) and move dataptr to TTL/Protocol if 4
  0x01, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x05, 0x80, // --> SV_EQU | Lbl_IPICMP_2=5 | 0 | 0 | ffh | 1h            ; check ID for ICMP (1)     -> from here with Version(=4)/IHL(!=5)
  0x02, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x05, 0x80, // --> SV_EQU | Lbl_IPIGMP_2=5 | 0 | 0 | ffh | 2h            ; check ID for IGMP (2)
  0x06, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x05, 0x80, // --> SV_EQU | Lbl_IPTCP_2=5 | 0 | 0 | ffh | 6h             ; check ID for TCP  (6)
  0x11, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x05, 0x80, // --> SV_EQU | Lbl_IP_UDP_2=5 | 0 | 0 | ffh | 11h           ; check ID for UDP  (17)
  0x01, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 20 | 1                                         ; drop frame
  0x0A, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 16 | 10                                        ; pass frame ID = ICMP (1)
  0x0A, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 17 | 10                                        ; pass frame ID = IGMP (2)
  0x03, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 18 | 3                                         ; pass frame ID = TCP  (6)
  0x04, 0x00, 0x13, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 19 | 4                                         ; pass frame ID = UDP (17)
  0x00, 0x00, 0x10, 0x80, 0x00, 0x00, 0x00, 0x10, // --> DROP | 32784 | 0                                      ; drop frame
  // EDDP_API_TYPE_ARP         ; Filter for ARP EtherType:0x0806
  0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x01, 0x00, // --> SKIP | 6                                              ; move dataptr to VLAN/EtherType
  0x00, 0x81, 0xFF, 0xFF, 0x00, 0x02, 0x00, 0x80, // --> SV_EQU | 0 | 2 | 0 | ffffh | 8100h                    ; check VLAN_TYPE and skip VLAN until not found anymore 
  0x06, 0x08, 0xFF, 0xFF, 0x01, 0x00, 0x0C, 0x81, // --> SV_NEQ | Lbl_ARP_DROP=12 | 0 | 1 | ffffh | 806h       ; check that EtherType is ARP
  0x01, 0x00, 0xFF, 0xFF, 0x01, 0x00, 0x0B, 0x81, // --> SV_NEQ | Lbl_ARP_DROP=11 | 0 | 1 | ffffh | 1h         ; Hrd = 1?
  0x00, 0x08, 0xFF, 0xFF, 0x01, 0x00, 0x0A, 0x81, // --> SV_NEQ | Lbl_ARP_DROP=10 | 0 | 1 | ffffh | 800h       ; Prot = IP? 
  0x04, 0x06, 0xFF, 0xFF, 0x01, 0x00, 0x09, 0x81, // --> SV_NEQ | Lbl_ARP_DROP=9 | 0 | 1 | ffffh | 604h        ; hln (6), pln (4)
  0x01, 0x00, 0xFF, 0xFF, 0x04, 0x00, 0x07, 0x81, // --> SV_NEQ | Lbl_ARP_PASS=7 | 0 | 4 | ffffh | 1h          ; opcode (1) = request, skip to spa field
  0x4A, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x06, 0x0F, // --> SCMP | Lbl_ARP_PASS=6 | 0 | 0 | ffffh | 4ah           ; check spa field, stay on position.
  0x00, 0x00, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x87, // --> SM_STORE | 1 | 65535 | 0                              ; Store spa HiWord, move to LowByte (2Byte)
  0x01, 0x00, 0xFF, 0xFF, 0x04, 0x00, 0x00, 0x87, // --> SM_STORE | 4 | 65535 | 1                              ; Store spa LoWord, move to tpa field (8Byte)
  0x4A, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x03, 0x0F, // --> SCMP | Lbl_ARP_PASS=3 | 0 | 0 | ffffh | 4ah           ; check tpa field, stay on position.
  0x00, 0x00, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x89, // --> SM_NEQ | Lbl_ARP_DROP=3 | 0 | 1 | ffffh | idx:0h (680h) ; spa == tpa? -> HiWord
  0x01, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x02, 0x89, // --> SM_NEQ | Lbl_ARP_DROP=2 | 0 | 0 | ffffh | idx:1h (682h) ; spa == tpa? -> LoWord
  0x02, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 32 | 2                                         ; pass frame
  0x00, 0x00, 0x20, 0x80, 0x00, 0x00, 0x00, 0x10, // --> DROP | 32800 | 0                                      ; drop frame
  // EDDP_API_TYPE_MRP         ; filter for mrp ethertype:0x88e3, DLR ethertype:0x80E1
  0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x01, 0x00, // --> SKIP | 6                                              ; move dataptr to VLAN/EtherType
  0x00, 0x81, 0xFF, 0xFF, 0x00, 0x02, 0x00, 0x80, // --> SV_EQU | 0 | 2 | 0 | ffffh | 8100h                    ; check VLAN_TYPE and skip VLAN until not found anymore 
  0xE3, 0x88, 0xFF, 0xFF, 0x00, 0x00, 0x03, 0x80, // --> SV_EQU | Lbl_MRP_PASS=3 | 0 | 0 | ffffh | 88e3h       ; check that EtherType is MRP
  0xE1, 0x80, 0xFF, 0xFF, 0x00, 0x00, 0x02, 0x80, // --> SV_EQU | Lbl_MRP_PASS=2 | 0 | 0 | ffffh | 80e1h       ; check that EtherType is EIP/DLR
  0x00, 0x00, 0x30, 0x80, 0x00, 0x00, 0x00, 0x10, // --> DROP | 32816 | 0                                      ; drop frame
  0x09, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 48 | 9                                         ; pass frame of type 0x88E3/80E1
  // EDDP_API_TYPE_LLDP        ; Filter for LLDP EtherType:0x88CC
  0x08, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 64 | 8                                         ; pass frame of type 0x88CC
  // EDDP_API_TYPE_PN_DCP      ; Filter for DCP EtherType:0x8892
  0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x01, 0x00, // --> SKIP | 6                                              ; move dataptr to VLAN/EtherType
  0x00, 0x81, 0xFF, 0xFF, 0x00, 0x02, 0x00, 0x80, // --> SV_EQU | 0 | 2 | 0 | ffffh | 8100h                    ; check VLAN_TYPE and skip VLAN until not found anymore 
  0x92, 0x88, 0xFF, 0xFF, 0x01, 0x01, 0x0F, 0x81, // --> SV_NEQ | Lbl_DCP_DROP=15 | 1 | 1 | ffffh | 8892h      ; check that EtherType is DCP
  0xFC, 0xFE, 0xFF, 0xFF, 0x00, 0x05, 0x10, 0x80, // --> SV_EQU | Lbl_DCP_HELLO=16 | 5 | 0 | ffffh | fefch     ; FEFC: check for DCP-Hello 
  0xFE, 0xFE, 0xFF, 0xFF, 0x05, 0x00, 0x0E, 0x81, // --> SV_NEQ | Lbl_DCP_PASS=14 | 0 | 5 | ffffh | fefeh      ; !FEFE: check for not DCP multicast
  0x04, 0x00, 0xFF, 0xFF, 0x01, 0x00, 0x0D, 0x83, // --> SV_LT | Lbl_DCP_PASS=13 | 0 | 1 | ffffh | 4h          ; no TLV present. pass
  0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x06, 0x0F, // --> SCMP | Lbl_DCP_CHK_TLV1=6 | 0 | 0 | ffffh | 0h        ; index into string descr. table
  0x02, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x06, 0x0F, // --> SCMP | Lbl_DCP_CHK_TLV2=6 | 0 | 0 | ffffh | 2h        ; index into string descr. table
  0x04, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x06, 0x0F, // --> SCMP | Lbl_DCP_CHK_TLV3=6 | 0 | 0 | ffffh | 4h        ; index into string descr. table
  0x06, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x06, 0x0F, // --> SCMP | Lbl_DCP_CHK_TLV4=6 | 0 | 0 | ffffh | 6h        ; index into string descr. table
  0x08, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x06, 0x0F, // --> SCMP | Lbl_DCP_CHK_TLV5=6 | 0 | 0 | ffffh | 8h        ; index into string descr. table
  0x06, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 80 | 6                                         ; pass frame
  0x01, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x06, 0x0F, // --> SCMP | Lbl_DCP_PASS=6 | 0 | 0 | ffffh | 1h            ; index into string descr. table
  0x03, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x05, 0x0F, // --> SCMP | Lbl_DCP_PASS=5 | 0 | 0 | ffffh | 3h            ; index into string descr. table
  0x05, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x04, 0x0F, // --> SCMP | Lbl_DCP_PASS=4 | 0 | 0 | ffffh | 5h            ; index into string descr. table
  0x07, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x03, 0x0F, // --> SCMP | Lbl_DCP_PASS=3 | 0 | 0 | ffffh | 7h            ; index into string descr. table
  0x09, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x02, 0x0F, // --> SCMP | Lbl_DCP_PASS=2 | 0 | 0 | ffffh | 9h            ; index into string descr. table
  0x00, 0x00, 0x50, 0x80, 0x00, 0x00, 0x00, 0x10, // --> DROP | 32848 | 0                                      ; drop frame
  0x06, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 80 | 6                                         ; pass frame
  0x04, 0x00, 0xFF, 0xFF, 0x01, 0x00, 0x22, 0x83, // --> SV_LT | Lbl_DCPHello_DROP=34 | 0 | 1 | ffffh | 4h     ; no TLV present if smaller 4 --> drop it
  0x02, 0x02, 0xFF, 0xFF, 0x01, 0x00, 0x21, 0x81, // --> SV_NEQ | Lbl_DCPHello_DROP=33 | 0 | 1 | ffffh | 202h  ; test Option & Suboption
  0x0A, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV01=33 | 2 | 0 | ffffh | ah ; index into string descr. table
  0x0C, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV02=33 | 2 | 0 | ffffh | ch ; index into string descr. table
  0x0E, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV03=33 | 2 | 0 | ffffh | eh ; index into string descr. table
  0x10, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV04=33 | 2 | 0 | ffffh | 10h ; index into string descr. table
  0x12, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV05=33 | 2 | 0 | ffffh | 12h ; index into string descr. table
  0x14, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV06=33 | 2 | 0 | ffffh | 14h ; index into string descr. table
  0x16, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV07=33 | 2 | 0 | ffffh | 16h ; index into string descr. table
  0x18, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV08=33 | 2 | 0 | ffffh | 18h ; index into string descr. table
  0x1A, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV09=33 | 2 | 0 | ffffh | 1ah ; index into string descr. table
  0x1C, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV10=33 | 2 | 0 | ffffh | 1ch ; index into string descr. table
  0x1E, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV11=33 | 2 | 0 | ffffh | 1eh ; index into string descr. table
  0x20, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV12=33 | 2 | 0 | ffffh | 20h ; index into string descr. table
  0x22, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV13=33 | 2 | 0 | ffffh | 22h ; index into string descr. table
  0x24, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV14=33 | 2 | 0 | ffffh | 24h ; index into string descr. table
  0x26, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV15=33 | 2 | 0 | ffffh | 26h ; index into string descr. table
  0x28, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV16=33 | 2 | 0 | ffffh | 28h ; index into string descr. table
  0x2A, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV17=33 | 2 | 0 | ffffh | 2ah ; index into string descr. table
  0x2C, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV18=33 | 2 | 0 | ffffh | 2ch ; index into string descr. table
  0x2E, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV19=33 | 2 | 0 | ffffh | 2eh ; index into string descr. table
  0x30, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV20=33 | 2 | 0 | ffffh | 30h ; index into string descr. table
  0x32, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV21=33 | 2 | 0 | ffffh | 32h ; index into string descr. table
  0x34, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV22=33 | 2 | 0 | ffffh | 34h ; index into string descr. table
  0x36, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV23=33 | 2 | 0 | ffffh | 36h ; index into string descr. table
  0x38, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV24=33 | 2 | 0 | ffffh | 38h ; index into string descr. table
  0x3A, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV25=33 | 2 | 0 | ffffh | 3ah ; index into string descr. table
  0x3C, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV26=33 | 2 | 0 | ffffh | 3ch ; index into string descr. table
  0x3E, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV27=33 | 2 | 0 | ffffh | 3eh ; index into string descr. table
  0x40, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV28=33 | 2 | 0 | ffffh | 40h ; index into string descr. table
  0x42, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV29=33 | 2 | 0 | ffffh | 42h ; index into string descr. table
  0x44, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV30=33 | 2 | 0 | ffffh | 44h ; index into string descr. table
  0x46, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV31=33 | 2 | 0 | ffffh | 46h ; index into string descr. table
  0x48, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x21, 0x0F, // --> SCMP | Lbl_DCP_HELLO_CHK_TLV32=33 | 2 | 0 | ffffh | 48h ; index into string descr. table
  0x00, 0x00, 0x51, 0x00, 0x00, 0x00, 0x00, 0x10, // --> DROP | 81 | 0                                         ; drop frame
  0x0B, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x22, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS1=34 | 0 | 0 | ffffh | bh    ; index into string descr. table, Filter1
  0x0D, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x22, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS2=34 | 0 | 0 | ffffh | dh    ; index into string descr. table, Filter2
  0x0F, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x22, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS3=34 | 0 | 0 | ffffh | fh    ; index into string descr. table, Filter3
  0x11, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x1E, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=30 | 0 | 0 | ffffh | 11h    ; index into string descr. table
  0x13, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x1D, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=29 | 0 | 0 | ffffh | 13h    ; index into string descr. table
  0x15, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x1C, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=28 | 0 | 0 | ffffh | 15h    ; index into string descr. table
  0x17, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x1B, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=27 | 0 | 0 | ffffh | 17h    ; index into string descr. table
  0x19, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x1A, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=26 | 0 | 0 | ffffh | 19h    ; index into string descr. table
  0x1B, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x19, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=25 | 0 | 0 | ffffh | 1bh    ; index into string descr. table
  0x1D, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x18, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=24 | 0 | 0 | ffffh | 1dh    ; index into string descr. table
  0x1F, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x17, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=23 | 0 | 0 | ffffh | 1fh    ; index into string descr. table
  0x21, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x16, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=22 | 0 | 0 | ffffh | 21h    ; index into string descr. table
  0x23, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x15, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=21 | 0 | 0 | ffffh | 23h    ; index into string descr. table
  0x25, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x14, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=20 | 0 | 0 | ffffh | 25h    ; index into string descr. table
  0x27, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x13, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=19 | 0 | 0 | ffffh | 27h    ; index into string descr. table
  0x29, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x12, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=18 | 0 | 0 | ffffh | 29h    ; index into string descr. table
  0x2B, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x11, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=17 | 0 | 0 | ffffh | 2bh    ; index into string descr. table
  0x2D, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x10, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=16 | 0 | 0 | ffffh | 2dh    ; index into string descr. table
  0x2F, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x0F, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=15 | 0 | 0 | ffffh | 2fh    ; index into string descr. table
  0x31, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x0E, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=14 | 0 | 0 | ffffh | 31h    ; index into string descr. table
  0x33, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x0D, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=13 | 0 | 0 | ffffh | 33h    ; index into string descr. table
  0x35, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x0C, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=12 | 0 | 0 | ffffh | 35h    ; index into string descr. table
  0x37, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x0B, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=11 | 0 | 0 | ffffh | 37h    ; index into string descr. table
  0x39, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x0A, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=10 | 0 | 0 | ffffh | 39h    ; index into string descr. table
  0x3B, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x09, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=9 | 0 | 0 | ffffh | 3bh     ; index into string descr. table
  0x3D, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x08, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=8 | 0 | 0 | ffffh | 3dh     ; index into string descr. table
  0x3F, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x07, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=7 | 0 | 0 | ffffh | 3fh     ; index into string descr. table
  0x41, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x06, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=6 | 0 | 0 | ffffh | 41h     ; index into string descr. table
  0x43, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x05, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=5 | 0 | 0 | ffffh | 43h     ; index into string descr. table
  0x45, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x04, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=4 | 0 | 0 | ffffh | 45h     ; index into string descr. table
  0x47, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x03, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=3 | 0 | 0 | ffffh | 47h     ; index into string descr. table
  0x49, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x02, 0x0F, // --> SCMP | Lbl_DCP_HELLO_PASS=2 | 0 | 0 | ffffh | 49h     ; index into string descr. table
  0x00, 0x00, 0x51, 0x00, 0x00, 0x00, 0x00, 0x10, // --> DROP | 81 | 0                                         ; drop frame
  0x06, 0x00, 0x51, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 81 | 6                                         ; pass frame ; FDC for HelloFilterString #4...#31 ; Note that currently Filters #4...31 are disabled in ERTEC200P, so FDC is the same as Filter#1
  0x06, 0x00, 0x51, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 81 | 6                                         ; pass frame ; FDC for HelloFilterString #1
  0x06, 0x00, 0x51, 0x04, 0x00, 0x00, 0x00, 0x11, // --> PASS | 1105 | 6                                       ; pass frame ; FDC for HelloFilterString #2
  0x06, 0x00, 0x51, 0x08, 0x00, 0x00, 0x00, 0x11, // --> PASS | 2129 | 6                                       ; pass frame ; FDC for HelloFilterString #3
  // EDDP_API_TYPE_ALARM       ; Filter for Alarm EtherType:0x8892
  0x05, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 96 | 5                                         ; pass frame FC00-FCFF=high, FE00-FEFC=low
  // EDDP_API_TYPE_LEN         ; Filter for LEN EtherType: <0x0800
  0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x01, 0x00, // --> SKIP | 6                                              ; move dataptr to VLAN/EtherType
  0x00, 0x81, 0xFF, 0xFF, 0x00, 0x02, 0x00, 0x80, // --> SV_EQU | 0 | 2 | 0 | ffffh | 8100h                    ; check VLAN_TYPE and skip VLAN until not found anymore 
  0x00, 0x08, 0xFF, 0xFF, 0x01, 0x00, 0x10, 0x84, // --> SV_GTE | Lbl_LEN_DROP=16 | 0 | 1 | ffffh | 800h       ; check that EtherType is < 800h
  0xFD, 0xFD, 0xFF, 0xFF, 0x00, 0x01, 0x0B, 0x80, // --> SV_EQU | Lbl_Chk_LEN_SINEC_FWL=11 | 1 | 0 | ffffh | fdfdh ; check for DSAP (FD),SSAP (FD)
  0xAA, 0xAA, 0xFF, 0xFF, 0x01, 0x00, 0x0E, 0x81, // --> SV_NEQ | Lbl_LEN_DROP=14 | 0 | 1 | ffffh | aaaah      ; check for DSAP (AA),SSAP (AA)
  0x08, 0x03, 0xFF, 0xFF, 0x01, 0x00, 0x0E, 0x81, // --> SV_NEQ | Lbl_LEN_MORE=14 | 0 | 1 | ffffh | 308h       ; check for CTRL (03),OUI (08,[00,06])
  0x06, 0x00, 0xFF, 0xFF, 0x01, 0x00, 0x11, 0x81, // --> SV_NEQ | Lbl_LEN_OTHER=17 | 0 | 1 | ffffh | 6h        ; check for OUI (00,06)
  0xFD, 0x01, 0xFF, 0xFF, 0x00, 0x00, 0x05, 0x80, // --> SV_EQU | Lbl_LEN_DCP=5 | 0 | 0 | ffffh | 1fdh         ; check for DCP (cd7)  
  0x00, 0x01, 0xFF, 0xFF, 0x00, 0x00, 0x05, 0x80, // --> SV_EQU | Lbl_LEN_TSYNC=5 | 0 | 0 | ffffh | 100h       ; check for TSYNC 
  0x10, 0x01, 0xFF, 0xFF, 0x00, 0x00, 0x08, 0x80, // --> SV_EQU | Lbl_LEN_SINEC_FWL=8 | 0 | 0 | ffffh | 110h   ; check for SINEC_FWL 
  0xFB, 0x01, 0xFF, 0xFF, 0x00, 0x00, 0x0D, 0x81, // --> SV_NEQ | Lbl_LEN_OTHER=13 | 0 | 0 | ffffh | 1fbh      ; check for STDBY 
  0x09, 0x00, 0x72, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 114 | 9                                        ; pass frame 01FBh
  0x06, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 112 | 6                                        ; pass frame 01FDh
  0x0A, 0x00, 0x71, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 113 | 10                                       ; pass frame 0100h
  0x08, 0x03, 0xFF, 0xFF, 0x01, 0x00, 0x04, 0x81, // --> SV_NEQ | Lbl_LEN_DROP=4 | 0 | 1 | ffffh | 308h        ; check for CTRL (03),OUI (08,[00,06])
  0x06, 0x00, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x81, // --> SV_NEQ | Lbl_LEN_DROP=3 | 0 | 1 | ffffh | 6h          ; check for OUI (00,06)
  0x10, 0x01, 0xFF, 0xFF, 0x00, 0x00, 0x02, 0x81, // --> SV_NEQ | Lbl_LEN_DROP=2 | 0 | 0 | ffffh | 110h        ; check for SINEC_FWL 
  0x0A, 0x00, 0x74, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 116 | 10                                       ; pass frame 0110h
  0x00, 0x00, 0x70, 0x80, 0x00, 0x00, 0x00, 0x10, // --> DROP | 32880 | 0
  0x00, 0x03, 0xFF, 0xFF, 0x01, 0x00, 0x04, 0x81, // --> SV_NEQ | Lbl_LEN_OTHER=4 | 0 | 1 | ffffh | 300h       ; check for CTRL (03),OUI (00,[80,63])
  0x63, 0x80, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x81, // --> SV_NEQ | Lbl_LEN_OTHER=3 | 0 | 1 | ffffh | 8063h      ; check for OUI (80,63)
  0x00, 0x08, 0xFF, 0xFF, 0x00, 0x00, 0x02, 0x81, // --> SV_NEQ | Lbl_LEN_OTHER=2 | 0 | 0 | ffffh | 800h       ; check for HSR      
  0x09, 0x00, 0x73, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 115 | 9                                        ; pass frame 8000h
  0x01, 0x00, 0x75, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 117 | 1
  // EDDP_API_TYPE_PTCP        ; Filter for PTPC EtherType:0x8892
  0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x01, 0x00, // --> SKIP | 6                                              ; move dataptr to VLAN/EtherType
  0x00, 0x81, 0xFF, 0xFF, 0x00, 0x02, 0x00, 0x80, // --> SV_EQU | 0 | 2 | 0 | ffffh | 8100h                    ; check VLAN_TYPE and skip VLAN until not found anymore 
  0x92, 0x88, 0xFF, 0xFF, 0x01, 0x01, 0x0E, 0x81, // --> SV_NEQ | Lbl_PTCP_DROP=14 | 1 | 1 | ffffh | 8892h     ; check that EtherType is PROFINET
  0x00, 0xFF, 0xFE, 0xFF, 0x00, 0x00, 0x0B, 0x80, // --> SV_EQU | Lbl_PTCP_ANNOUNCE=11 | 0 | 0 | fffeh | ff00h ; FrameID = FF00+FF01   --> pass it ANNOUNCE
  0x40, 0xFF, 0xFC, 0xFF, 0x00, 0x00, 0x0B, 0x80, // --> SV_EQU | Lbl_PTCP_K32_DELAY=11 | 0 | 0 | fffch | ff40h ; FrameID = FF40-FF43   --> pass it DELAY
  0x20, 0x00, 0xFE, 0xFF, 0x00, 0x00, 0x06, 0x80, // --> SV_EQU | Lbl_PTCP_CHK_SYNC=6 | 0 | 0 | fffeh | 20h    ; FrameID = 0020, 0021  --> check Sync0/1
  0x80, 0x00, 0xFE, 0xFF, 0x00, 0x00, 0x05, 0x80, // --> SV_EQU | Lbl_PTCP_CHK_SYNC=5 | 0 | 0 | fffeh | 80h    ; FrameID = 0080, 0081  --> check Sync0/1
  0x20, 0xFF, 0xFE, 0xFF, 0x00, 0x00, 0x09, 0x81, // --> SV_NEQ | Lbl_PTCP_DROP=9 | 0 | 0 | fffeh | ff20h      ; FrameID = FF20, FF21  --> check SyncFU 0/1
  0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x80, // --> SV_EQU | Lbl_PTCP_K32_SYNC_FU_1=2 | 0 | 0 | 1h | 1h   ; LSB for Sync0/Sync1 switch
  0x1D, 0x00, 0x84, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 132 | 29                                       ; pass frame into KRISC RX ring
  0x1E, 0x00, 0x85, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 133 | 30                                       ; pass frame into KRISC RX ring
  0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x80, // --> SV_EQU | Lbl_PTCP_K32_SYNC_1=2 | 0 | 0 | 1h | 1h      ; LSB for Sync0/Sync1 switch
  0x1D, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 129 | 29                                       ; pass frame into KRISC RX ring
  0x1E, 0x00, 0x82, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 130 | 30                                       ; pass frame into KRISC RX ring    
  0x07, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 128 | 7                                        ; pass frame to EDDP  
  0x1F, 0x00, 0x83, 0x00, 0x00, 0x00, 0x00, 0x11, // --> PASS | 131 | 31                                       ; pass frame into KRISC RX ring
  0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x10, // --> DROP | 32896 | 0                                      ; drop frame     
  // EDDP_API_TYPE_RT_CT       ; Filter for RT EtherType:0x8892
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, // --> CYCLIC | 0 | 0                                        ; Cyclic: RT_FRAME: 0100-BFFFh, RT_FRAME_UDP: C000-FBFFh
  // EDDP_API_TYPE_RT_SF       ; Filter for RT EtherType:0x8892
  0x00, 0x00, 0xF0, 0x80, 0x00, 0x00, 0x00, 0x10, // --> DROP | 33008 | 0                                      ; drop frame
  // EDDP_HW_FILTER_VER        ; version string null terminated
  0x56, 0x31, 0x2E, 0x30, 0x30, 0x72, 0x31, 0x00, // --> CONST: 'V','1','.','0','0','r','1','\0'

};

/*===========================================================================*/
/*                                external functions                         */
/*===========================================================================*/

/*===========================================================================*/
/*                                 functions                                 */
/*===========================================================================*/



/*****************************************************************************/
/*  end of file eddp_hwf.c                                                   */
/*****************************************************************************/

