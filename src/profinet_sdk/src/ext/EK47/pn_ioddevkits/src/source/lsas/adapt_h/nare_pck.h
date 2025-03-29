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
/*  C o m p o n e n t     &C: PnIODDevkits                              :C&  */
/*                                                                           */
/*  F i l e               &F: nare_pck.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*                       Frame for file "nare_pck.h".                        */
/*                       ==============================                      */
/*                                                                           */
/*  Internal headerfile                                                      */
/*  Defines internal types and data (and constants) for prefix,              */
/*  which must be used with packing alignment.                               */
/*                                                                           */
/*  This file has to be overwritten during system integration, because       */
/*  the packing alignment instructions depend on the compiler.               */
/*                                                                           */
/*****************************************************************************/

#ifndef NARE_PCK_H                     /* ----- reinclude-protection ----- */
#define NARE_PCK_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif


/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef NARE_MESSAGE
/*  24.06.03    JS    initial version.                                       */
/*  19.02.04    JS    added TLV-PDUs for DCP                                 */
/*                                                                           */
#endif
/*****************************************************************************/

/* MSC     : #pragma pack(2)*/
/* Tasking : #pragma pack 2 */
#include "pnio_pck2_on.h"

/*===========================================================================*/
/*                           ARP-Defines                                     */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Ethernet transmission layer                                    For IPv4   */
/*                                                              and Ethernet */
/*                                                                  Bytes    */
/*   48.bit: Ethernet address of destination                          6      */
/*   48.bit: Ethernet address of sender                               6      */
/*   16.bit: Protocol type = ether_type$ADDRESS_RESOLUTION  = 0x806   2      */
/*                                                                           */
/* Ethernet packet data:                                                     */
/*   16.bit: (ar$hrd) Hardware address space (e.g., Ethernet,         2      */
/*                    Packet Radio Net.)                                     */
/*   16.bit: (ar$pro) Protocol address space.  For Ethernet           2      */
/*                    hardware, this is from the set of type                 */
/*                    fields ether_typ$&lt;protocol&gt;.                     */
/*    8.bit: (ar$hln) byte length of each hardware address            1      */
/*    8.bit: (ar$pln) byte length of each protocol address            1      */
/*   16.bit: (ar$op)  opcode (ares_op$REQUEST | ares_op$REPLY)        2      */
/*   nbytes: (ar$sha) Hardware address of sender of this              6      */
/*                      packet, n from the ar$hln field.                       */
/*   mbytes: (ar$spa) Protocol address of sender of this              4      */
/*                    packet, m from the ar$pln field.                       */
/*   nbytes: (ar$tha) Hardware address of target of this              6      */
/*                    packet (if known).                                     */
/*   mbytes: (ar$tpa) Protocol address of target.                     4      */
/*                                                                  -----    */
/*                                                                   42      */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#define NARE_ARP_TYPE             0x0806
#define NARE_VLAN_TYPE            0x8100

/* field ar$hdr */
#define NARE_ARP_HRD_ETHERNET     1

/* field ar$pro */
#define NARE_ARP_PRO_IP           0x800

/* field ar$op*/
#define NARE_ARP_OP_REQUEST       1
#define NARE_ARP_OP_RESPONSE      2

/* field ar$hln */
#define NARE_ARP_HLN              NARE_MAC_ADDR_SIZE /* 6 */

/* field ar$pln */
#define NARE_ARP_PLN              NARE_IP_ADDR_SIZE  /* 4 */

/* Frame offsets */
#define NARE_ARP_DST_OFFSET       (0)
#define NARE_ARP_SRC_OFFSET       (NARE_ARP_DST_OFFSET+NARE_MAC_ADDR_SIZE)
#define NARE_ARP_TYPE_OFFSET      (NARE_ARP_SRC_OFFSET+NARE_MAC_ADDR_SIZE)
#define NARE_ARP_HRD_OFFSET       (NARE_ARP_TYPE_OFFSET+2)
#define NARE_ARP_PRO_OFFSET       (NARE_ARP_HRD_OFFSET+2)
#define NARE_ARP_HLN_OFFSET       (NARE_ARP_PRO_OFFSET+2)
#define NARE_ARP_PLN_OFFSET       (NARE_ARP_HLN_OFFSET+1)
#define NARE_ARP_OP_OFFSET        (NARE_ARP_PLN_OFFSET+1)
#define NARE_ARP_SHA_OFFSET       (NARE_ARP_OP_OFFSET+2)
#define NARE_ARP_SPA_OFFSET       (NARE_ARP_OP_OFFSET+NARE_ARP_HLN)
#define NARE_ARP_THA_OFFSET       (NARE_ARP_OP_OFFSET+NARE_ARP_PLN)
#define NARE_ARP_TPA_OFFSET       (NARE_ARP_OP_OFFSET+NARE_ARP_HLN)

/* Note: with NARE_MAC_TYPE = 6 and NARE_IP_TYPE = 4 all 16-Bit */
/*       values are propably aligned at a 16-Bit boundary if    */
/*       the frame starts at a 16-Bit boundary!                 */

#define ARP_FRAME_HEADER \
    NARE_MAC_TYPE    DstMacAddr;        /* Destination MAC address */ \
    NARE_MAC_TYPE    SrcMacAddr;        /* Source MAC address      */ \
    LSA_UINT16        Type;              /* Protocol Type (0x806)   */

#define ARP_FRAME_VLAN_HEADER \
    NARE_MAC_TYPE    DstMacAddr;        /* Destination MAC address */ \
    NARE_MAC_TYPE    SrcMacAddr;        /* Source MAC address      */ \
    LSA_UINT16        VLANType;         /* VLAN-Type               */ \
    LSA_UINT16        VLANCtrl;        /* VLAN-Control            */ \
    LSA_UINT16        Type;              /* Protocol Type (0x806)   */

#define ARP_FRAME_VLAN \
    LSA_UINT16        VLANType;         /* VLAN-Type               */ \
    LSA_UINT16        VLANCtrl;        /* VLAN-Control            */

#define ARP_FRAME_CONTENT \
    LSA_UINT16      Hrd;            /* Headertype    (1)       */ \
    LSA_UINT16      Pro;            /* Protocoltype  (0x800)   */ \
    LSA_UINT8       Hln;            /* Hardware addr. len (6)  */ \
    LSA_UINT8       Pln;            /* Protocol len (4)        */ \
    LSA_UINT16      Op;             /* Opcode (1,2)            */ \
    NARE_MAC_TYPE   Sha;            /* Sender Hardware addr.   */ \
    NARE_IP_TYPE    Spa;            /* Sender Protocol addr    */ \
    NARE_MAC_TYPE   Tha;            /* Target Hardware addr.   */ \
    NARE_IP_TYPE    Tpa;            /* Target Protocol addr.   */

typedef struct _ARP_FRAME_HEADER_TYPE
{
    ARP_FRAME_HEADER
} NARE_ARP_FRAME_HEADER_TYPE;

typedef struct _ARP_FRAME_HEADER_VLAN_TYPE
{
    ARP_FRAME_VLAN_HEADER
} NARE_ARP_FRAME_HEADER_VLAN_TYPE;

typedef struct _ARP_FRAME_CONTENT_TYPE
{
    ARP_FRAME_CONTENT
} NARE_ARP_FRAME_CONTENT_TYPE;

typedef struct _ARP_FRAME_TYPE
{
    ARP_FRAME_HEADER
    ARP_FRAME_CONTENT
} NARE_ARP_FRAME_TYPE;

typedef struct _ARP_FRAME_VLAN_TYPE
{
    ARP_FRAME_VLAN_HEADER
    ARP_FRAME_CONTENT
} NARE_ARP_FRAME_VLAN_TYPE;


typedef NARE_ARP_FRAME_HEADER_VLAN_TYPE   NARE_EDD_LOWER_MEM_ATTR * NARE_ARP_FRAME_HEADER_VLAN_PTR_TYPE;
typedef NARE_ARP_FRAME_CONTENT_TYPE       NARE_EDD_LOWER_MEM_ATTR * NARE_ARP_FRAME_CONTENT_PTR_TYPE;
typedef NARE_ARP_FRAME_TYPE               NARE_EDD_LOWER_MEM_ATTR * NARE_ARP_FRAME_PTR_TYPE;

#define NARE_ARP_TX_FRAME_LEN          sizeof(NARE_ARP_FRAME_TYPE)
#define NARE_ARP_TX_FRAME_VLAN_LEN     sizeof(NARE_ARP_FRAME_VLAN_TYPE)

#if NARE_MAC_ADDR_SIZE != 6
#error "IP-Addr size != 6 not supported! "
#endif

typedef union _NARE_UNION_MAC_TYPE
{
    struct
    {
        LSA_UINT32  MAC1234;
        LSA_UINT16  MAC56;
    } w;

    struct
    {
        LSA_UINT8   MACAddr[NARE_MAC_ADDR_SIZE];
    } b;
} NARE_UNION_MAC_TYPE;


#if NARE_IP_ADDR_SIZE != 4
#error "IP-Addr size != 4 not supported! "
#endif

typedef union _NARE_UNION_IP_TYPE
{
    struct
    {
        LSA_UINT32  IP1234;
    } w;

    struct
    {
        LSA_UINT8   IPAddr[NARE_IP_ADDR_SIZE];
    } b;
} NARE_UNION_IP_TYPE;


/*===========================================================================*/
/*                           DCP-Defines                                     */
/*===========================================================================*/

#if 0
#if NARE_MAC_ADDR_SIZE != DCP_MAC_ADR_SIZE
#error "MAC-Addr size mismatch with DCP! "
#endif

#if NARE_IP_ADDR_SIZE != DCP_IP_ADR_SIZE
#error "IP-Addr size mismatch with DCP! "
#endif
#endif

#define NARE_DCP_IP_TYPE    NARE_IP_TYPE

/* --------------------------------------------------------------------------*/
/* DataSets used with Identify or Set (needed/used with NARE)                */
/* --------------------------------------------------------------------------*/

/* Dataset for Type: DCP_TYPE_TCP_IP */
typedef struct _NARE_DCP_DS_TCPIP_TYPE
{
    NARE_DCP_IP_TYPE     IpAddr;
    NARE_DCP_IP_TYPE     SubnetMask;
    NARE_DCP_IP_TYPE     DefaultRouter;
} NARE_DCP_DS_TCPIP_TYPE;

#define NARE_DCP_DS_TCPIP_SIZE    12

/* Dataset for Type: DCP_TYPE_DEVICE_NAME */
typedef struct _NARE_DCP_DS_DEVICE_NAME_TYPE
{
    LSA_UINT8   Name[2];    /* Name. variable length, only low-letters! */
                            /*       not 0 terminated.                  */
} NARE_DCP_DS_DEVICE_NAME_TYPE;



/* DataSet-Union */
/* Data (part of V-field). Depends on "T"-Field (Type) */

typedef union NARE_DCP_DS_DATA_
{                                                   /* used with Type (T):      */
    NARE_DCP_DS_TCPIP_TYPE            TcpIp;            /* DCP_TYPE_TCP_IP           */
    NARE_DCP_DS_DEVICE_NAME_TYPE    NameOfStation;  /* DCP_TYPE_DEVICE_NAME      */
} NARE_DCP_DS_DATA;


/* --------------------------------------------------------------------------*/
/* TLVs                                                                      */
/* --------------------------------------------------------------------------*/
/* Important: Every TLV has to be even in size, so every chained TLV starts  */
/*            on a even address. So after a TLV a padding byte may be added. */
/*            This padding byte is not counted in "Len". So if Len has a odd */
/*            value we have one padding-byte to add (req) or take care of    */
/*            (rsp).                                                         */
/*                                                                           */
/*            With this Type/Len/Status and start of Data is always WORD     */
/*            aligned!                                                       */
/*                                                                           */
/* 1) Swapping:  The Type and all other 16/32-Bit values have to be swapped  */
/*               with NARE_HTONS() or NARE_NTOS() on writing/reading.        */
/* --------------------------------------------------------------------------*/

#define NARE_DCP_TYPE_LEN \
    LSA_UINT16            Type;            /* Option + Suboption         */ \
    LSA_UINT16            Len;            /* Length of "V"            */

#define NARE_DCP_STATUS \
    LSA_UINT16            Status;


typedef struct _NARE_DCP_TL_TYPE
{
    NARE_DCP_TYPE_LEN
} NARE_DCP_TL_TYPE;

typedef struct _NARE_DCP_TLS_TYPE
{
    NARE_DCP_TYPE_LEN
    NARE_DCP_STATUS
} NARE_DCP_TLS_TYPE;

/* --------------------------------------------------------------------------*/
/* TLV for DCP-Identify-Request/Response (=Filter-TLV)                       */
/* --------------------------------------------------------------------------*/

/* Request:                                 */
/* Note: T = Type (Option + Subotion)       */
/*       L = Len                            */
/*       V = Data                           */

typedef struct _NARE_DCP_IDENT_REQ_TLV_TYPE    /* without Status!        */
{
    NARE_DCP_TYPE_LEN
    NARE_DCP_DS_DATA    Data;            /* Variable data part        */

} NARE_DCP_IDENT_REQ_TLV_TYPE;

typedef NARE_DCP_IDENT_REQ_TLV_TYPE   NARE_DCP_LOWER_MEM_ATTR * NARE_DCP_IDENT_REQ_TLV_PTR_TYPE;

/* Type   : e.g. DCP_TYPE_DEVICE_NAME used as filter.              */

/* Response:                                */
/* Note: T = Type (Option + Subotion)       */
/*       L = Len                            */
/*       V = Status+Data                    */

typedef struct _NARE_DCP_IDENT_RSP_TLV_TYPE
{
    NARE_DCP_TYPE_LEN
    NARE_DCP_STATUS
    NARE_DCP_DS_DATA    Data;            /* Variable data part        */

} NARE_DCP_IDENT_RSP_TLV_TYPE;

typedef NARE_DCP_IDENT_RSP_TLV_TYPE   NARE_DCP_LOWER_MEM_ATTR * NARE_DCP_IDENT_RSP_TLV_PTR_TYPE;

/* Type   : Databasis returned (in addition to filter-types)       */
/*          Identify always returns the following Datasets:        */
/*          DCP_TYPE_TCP_IP            1)                          */
/*          DCP_TYPE_DEVICE_NAME       1)                          */
/*          DCP_TYPE_DEVICE_ID         1)                          */
/*          DCP_TYPE_DEVICE_ROLE       1)                          */
/*          DCP_TYPE_DEVICE_OPTIONS    1)                          */
/* Status : depends on Type                                        */

/* STATUS (e.g. BlockInfo) of DCP_TYPE_TCP_IP */
#define NARE_DCP_IDENTIFY_STATUS_IP_ADDR_CONFLICT   0x0080  /* Bit 7 */

/* --------------------------------------------------------------------------*/
/* TLV for DCP-Set-Request/Response                                          */
/* --------------------------------------------------------------------------*/

/* Request:                                 */
/* Note: T = Type (Option + Subotion)       */
/*       L = Len                            */
/*       V = Status+Data                    */

typedef struct _NARE_DCP_SET_REQ_TLV_TYPE
{
    NARE_DCP_TYPE_LEN
    NARE_DCP_STATUS
    NARE_DCP_DS_DATA    Data;            /* Variable data part        */
} NARE_DCP_SET_REQ_TLV_TYPE;

typedef NARE_DCP_SET_REQ_TLV_TYPE   NARE_DCP_LOWER_MEM_ATTR * NARE_DCP_SET_REQ_TLV_PTR_TYPE;

/* Type   : e.g. DCP_TYPE_TCP_IP used to set IP-parameters          */
/* Status : remanent or not remanent storage of parameters          */

#define NARE_DCP_STATUS_NOT_REMANENT    0x0 /* save values not remanent */
#define NARE_DCP_STATUS_REMANENT        0x1 /* save values remanent     */


/* Response:                                */
/* Note: T = Type (Option + Subotion)       */
/*       L = Len                            */
/*       V = Status+Result+AddOn            */

typedef struct _NARE_DCP_SET_RSP_TLV_TYPE
{
    NARE_DCP_TYPE_LEN
    NARE_DCP_STATUS
    LSA_UINT8           Result;            /* Result dataset            */
   LSA_UINT8           AddOn[2];       /* Variable AddOn, depends  */
                                       /* on Len                   */
} NARE_DCP_SET_RSP_TLV_TYPE;


typedef NARE_DCP_SET_RSP_TLV_TYPE   NARE_DCP_LOWER_MEM_ATTR * NARE_DCP_SET_RSP_TLV_PTR_TYPE;

/* Type   : shall be: DCP_TYPE_CTRL_RESULT  1)                    */
/* Status : shall be  option+suboption. eg. DCP_TYPE_TCP_IP  1)   */
/* Result : shall be:                                             */
/*          DCP_RESULT_OK            0                              */
/*          DCP_RESULT_NO_DATABASE    1                              */
/*          DCP_RESULT_NO_DATASET    2                              */
/*          DCP_RESULT_DS_NOT_SET    3                              */
/*          DCP_RESULT_UNKNOWN        4                              */
/*          DCP_RESULT_NO_ACTION    5                              */
/*          DCP_RESULT_NO_RESOURCE    6                              */
/* AddOn  : Dont care.                                            */

/* --------------------------------------------------------------------------*/


/* MSC:     #pragma pack() */
/* Tasking: #pragma pack 0 */
#include "pnio_pck_off.h"


/*****************************************************************************/
/*  end of file NARE_PCK.H                                                   */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of NARE_PCK_H */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
