#ifndef MRP_PDU_H
#define MRP_PDU_H

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
/*  C o m p o n e n t     &C: MRP (Media Redundancy Protocol)           :C&  */
/*                                                                           */
/*  F i l e               &F: mrp_pdu.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Defines basic MRP PDU packed structures - applicable to MRC and MRM      */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef MRP_MESSAGE   
/*                                                                           */
/*  2016-10-12  ds    File introduced (mrp_pck.h -> mrp_pdu.h)               */
/*                    PNIO packing introduced                                */
#endif
/*****************************************************************************/


/* packing of structures starts now */
#include "pnio_pck2_on.h"


/*---------------------------------------------------------------------------*/
/*              MRP packet data unit (MRP PDU) basic definitions             */
/*---------------------------------------------------------------------------*/

/* MRP PDU types */
#define MRP_PACKET_VAL_TLV_TPLG_CHG         0x030A
#define MRP_PACKET_VAL_TLV_LNK_DWN          0x040E
#define MRP_PACKET_VAL_TLV_LNK_UP           0x050E
#define MRP_PACKET_VAL_TLV_TST              0x0212



#define MRP_PACKET_SIZE_LINK_CHG                64
#define MRP_PACKET_SIZE_TPLG_CHG                64

#define MRP_PACKET_VAL_PROTO_TAG_IDENT      0x8100
#define MRP_PACKET_VAL_VLAN_ID_TAG_MASK     0x0000FF00
#define MRP_PACKET_VAL_VLAN_ID_TAG_VALID    0x0000e000
#define MRP_PACKET_VAL_VLAN_ID_MRP_MASK     0xFFFF0000
#define MRP_PACKET_VAL_VLAN_ID_MRP          0x81000000

#define MRP_PACKET_VAL_LT_MRP               0x88E30000

#define MRP_PACKET_VAL_VERSION              0x0001    /* = MRP_Version */

#define MRP_PACKET_VAL_TLV_END              0x0000
#define MRP_PACKET_VAL_TLV_COMMON           0x0112

#define MRP_PACKET_VAL_PORT_ROLE_PRIMARY    0x0000
#define MRP_PACKET_VAL_PORT_ROLE_SECONDARY  0x0001

#define MRP_PACKET_VAL_DST_ADR_NSM_TST     \
  "\x01\x15\x4E\x00\x00\x01" /* dst */

#define MRP_PACKET_VAL_DST_ADR_NSM_WRK     \
  "\x01\x15\x4E\x00\x00\x02" /* dst */

#define MRP_PACKET_VAL_SRC_ADR_TST_PORT_A		\
  "\x08\x00\x06\x20\x48\x2F" /* src */

#define MRP_PACKET_VAL_SRC_ADR_TST_PORT_B		\
  "\x08\x00\x06\x20\x48\x30" /* src */

#define MRP_PACKET_VAL_DST_ADR_HA_VENDOR		\
  "\x01\x0E\xCF\x00\x05\x00" /* dst */

#define MRP_PACKET_VAL_DEFAULT_ZERRO_MAC_ADDRESS     \
  "\x00\x00\x00\x00\x00\x00" /* zerro mac address */


#define MRP_PACKET_VAL_DEFAULT \
  "\xFF\xFF\xFF\xFF\xFF\xFF"                                  /* LSA_UINT8  dst[6];                0 */\
  "\x00\x00\x00\x00\x00\x00"                                  /* LSA_UINT8  src[6];                6 */\
  "\x81\x00"                                                  /* LSA_UINT16 vlan_lt;              12 */\
  "\xE0\x00"                                                  /* LSA_UINT16 vlan_tagCtrlInfo;     14 */\
  "\x88\xE3"                                                  /* LSA_UINT16 lt;                   16 */\
  "\x00\x01"                                                  /* LSA_UINT16 ver(sion);            18 */\
  "\x00\x00"                                                  /* LSA_UINT16 tlv;                  20 */\
  "\x00\x00"                                                  /* LSA_UINT8  *payload;             22 */\
  "\x00\x00\x00\x00\x00\x00\x00\x00"\
  "\x00\x00\x00\x00\x00\x00\x00\x00"\
  "\x00\x00\x00\x00\x00\x00\x00\x00"\
  "\x00\x00\x00\x00\x00\x00\x00\x00"\
  "\x00\x00\x00\x00\x00\x00\x00\x00"\
  "\x00\x00\x00\x00\x00\x00\x00\x00"\
  "\x00\x00\x00\x00\x00\x00\x00\x00"\
  "\x00\x00\x00\x00\x00\x00\x00\x00"\
  "\x00\x00\x00\x00\x00\x00\x00\x00"\
  "\x00\x00\x00\x00\x00\x00\x00\x00"


#define MRP_PACKET_VAL_NO_OUI               "\x00\x00\x00"


/*---------------------------------------------------------------------------*/
/*              MRP packet data unit (MRP PDU) basic length definitions      */
/*---------------------------------------------------------------------------*/


#define MRP_PACKET_VAL_MAC_ADDR_LEN              6 /* SRC/DST MAC addr length     */
#define MRP_PACKET_VAL_MRP_VERSION_LEN           2 /* length of MRP version field */
#define MRP_PACKET_VAL_PADDING_BYTE_LEN          1 /* length of one padding byte  */
#define MRP_PACKET_VAL_PADDING_WORD_LEN          2 /* length of two padding bytes */
#define MRP_PACKET_VAL_DOMAIN_UUID_LEN          16 /* length of domain UUID field */

#define MRP_PACKET_VAL_TLV_OPTION_LEN            2 /* length of TLV Option field (Type + Length) */
#define MRP_PACKET_VAL_OUI_LEN                   3 /* length of OUI in MRP frame */
#define MRP_PACKET_VAL_SO1_ED1TYPE_LEN           1 /* length of MRP_Ed1Type in MRP frame */

#define MRP_PACKET_DATA_OFFSET_VER_TO_OUI        (MRP_PACKET_VAL_MRP_VERSION_LEN + MRP_PACKET_VAL_TLV_OPTION_LEN)
#define MRP_PACKET_DATA_OFFSET_VER_TO_SO1ED1TYPE (MRP_PACKET_VAL_MRP_VERSION_LEN + MRP_PACKET_VAL_TLV_OPTION_LEN + MRP_PACKET_VAL_OUI_LEN)
#define MRP_PACKET_VAL_OFFSET_VER_TO_PAYLOAD_EXT (MRP_PACKET_VAL_MRP_VERSION_LEN + MRP_PACKET_VAL_TLV_OPTION_LEN + MRP_PACKET_VAL_OUI_LEN + MRP_PACKET_VAL_SO1_ED1TYPE_LEN)

#define MRP_PACKET_VAL_PAYLOAD_NORM_MRM_TST_LEN  42


#define MRP_PACKET_TST_EXT_ADD_BYTES_ED1           6        /* auto MRM extension MRA 1.0: MRP_PACKET_TST_EXT_TLV_ADD_BYTES_ED1 =6 Bytes */
#define MRP_PACKET_TST_EXT_ADD_BYTES_ED1_ED2_COMP 10        /* auto MRM extension MRA 2.0 siemens compatible */
#define MRP_PACKET_TST_EXT_ADD_BYTES_ED2_ONLY      6        /* auto MRM extension MRA 2.0 edition 2 only     */


#define MRP_PACKET_VAL_ED1TYPE4_MAN_DATA_LEN      26 /* length of special manufacturer data for the case that the suboption1 ed1Type is four (so1Ed1Type = 4) */



/*---------------------------------------------------------------------------*/
/*                MRP packet data unit (MRP PDU) structures                  */
/*---------------------------------------------------------------------------*/

typedef PNIO_PACKED_ATTRIBUTE_PRE union _MRP_PACKET_UNION_VLAN_TYPE
{
  LSA_UINT16 vlan_u16[2];                                /* 12: VLAN tag */
  LSA_UINT32 vlan_u32;                                   /* 12: VLAN tag complete */
} PNIO_PACKED_ATTRIBUTE_POST MRP_PACKET_UNION_VLAN_TYPE;

/* pack this struct to word size */
typedef PNIO_PACKED_ATTRIBUTE_PRE struct _MRP_PACKET_STRUCT_HEADER_TYPE
{
  LSA_UINT8  dst[MRP_PACKET_VAL_MAC_ADDR_LEN];           /*  0 */
  LSA_UINT8  src[MRP_PACKET_VAL_MAC_ADDR_LEN];           /*  6: source MAC address (port): PORT MAC address of sending device */
  MRP_PACKET_UNION_VLAN_TYPE vlan;                       /* 12: VLAN tag */
  LSA_UINT16 lt;                                         /* 16 */
                                                         /* 18 */
} PNIO_PACKED_ATTRIBUTE_POST MRP_PACKET_STRUCT_HEADER_TYPE;

/* pack this struct to word size */
typedef PNIO_PACKED_ATTRIBUTE_PRE struct _MRP_PACKET_STRUCT_PAYLOAD_LINK_CHG_TYPE
{
  LSA_UINT16 tlv;                                        /* 20 */
  LSA_UINT8  sa[MRP_PACKET_VAL_MAC_ADDR_LEN];            /* 22 */  /* source MAC address (interface): Interface MAC Address of sending device */
  LSA_UINT16 portRole;                                   /* 28 */
  LSA_UINT16 interval;                                   /* 30 */
  LSA_UINT16 blocked;                                    /* 32 */
  LSA_UINT8  padding[MRP_PACKET_VAL_PADDING_WORD_LEN];   /* 34 */
  LSA_UINT16 tlv_common;                                 /* 36 */
  LSA_UINT16 sequenceID;                                 /* 38 */
  LSA_UINT8  domainUUID[MRP_PACKET_VAL_DOMAIN_UUID_LEN]; /* 40 */
  LSA_UINT16 tlv_end;                                    /* 56 */
                                                         /* 58 */
} PNIO_PACKED_ATTRIBUTE_POST MRP_PACKET_STRUCT_PAYLOAD_LINK_CHG_TYPE;

/* pack this struct to word size */
typedef PNIO_PACKED_ATTRIBUTE_PRE struct _MRP_PACKET_STRUCT_LINK_CHG_TYPE
{
  MRP_PACKET_STRUCT_HEADER_TYPE           hdr;           /* the following offset values are valid for VLAN tagged frames only, decrement by 4, if no VLAN tag is present */
  LSA_UINT16                              ver;           /* 18 */
  MRP_PACKET_STRUCT_PAYLOAD_LINK_CHG_TYPE payload;       /* 20 */
} PNIO_PACKED_ATTRIBUTE_POST MRP_PACKET_STRUCT_LINK_CHG_TYPE;

/* pack this struct to word size */
typedef PNIO_PACKED_ATTRIBUTE_PRE struct _MRP_PACKET_STRUCT_PAYLOAD_TPLG_CHG_TYPE
{
  LSA_UINT16 tlv;                                        /* 20 */
  LSA_UINT16 prio;                                       /* 22 */
  LSA_UINT8  sa[MRP_PACKET_VAL_MAC_ADDR_LEN];            /* 24 */  /* source MAC address (interface): Interface MAC Address of sending device */
  LSA_UINT16 interval;                                   /* 30 */
  LSA_UINT16 tlv_common;                                 /* 32 */
  LSA_UINT16 sequenceID;                                 /* 34 */
  LSA_UINT8  domainUUID[MRP_PACKET_VAL_DOMAIN_UUID_LEN]; /* 36 */
  LSA_UINT16 tlv_end;                                    /* 52 */
                                                         /* 54 */
} PNIO_PACKED_ATTRIBUTE_POST MRP_PACKET_STRUCT_PAYLOAD_TPLG_CHG_TYPE;

/* pack this struct to word size */
typedef PNIO_PACKED_ATTRIBUTE_PRE struct _MRP_PACKET_STRUCT_TPLG_CHG_TYPE
{
  MRP_PACKET_STRUCT_HEADER_TYPE           hdr;           /* the following offset values are valid for VLAN tagged frames only, decrement by 4, if no VLAN tag is present */
  LSA_UINT16                              ver;           /* 18 */
  MRP_PACKET_STRUCT_PAYLOAD_TPLG_CHG_TYPE payload;       /* 20 */
} PNIO_PACKED_ATTRIBUTE_POST MRP_PACKET_STRUCT_TPLG_CHG_TYPE;



/* packing of structures ends now */
#include "pnio_pck_off.h"

#ifdef MRP_CFG_PLUGIN_2_HSR
#include "mrp2pdu.h"
#endif

#ifdef MRP_CFG_PLUGIN_0_MRM
#include "mrp0pdu.h"
#endif
#ifdef MRP_CFG_PLUGIN_1_MRM_EXT
#include "mrp1pdu.h"
#endif
/* MRP_IN: MRP interconnection PDUs */
#include "mrp_inpdu.h"

/*----------------------------------------------------------------------------*/
#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif  /* of MRP_PDU_H */
