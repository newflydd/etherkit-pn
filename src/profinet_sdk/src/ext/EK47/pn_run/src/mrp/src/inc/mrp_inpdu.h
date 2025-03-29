#ifndef MRP_INPDU_H
#define MRP_INPDU_H

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
/*  F i l e               &F: mrp_inpdu.h                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Defines basic MRP interconnection PDU packed structures                  */
/*  - applicable to RSTP+ as well as MIC and MIM                             */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef MRP_MESSAGE   
/*                                                                           */
/*  2017-06-28  ds    File mrp_inpdu.h introduced                            */
/*                    PNIO packing introduced                                */
/*  2017-11-08  ds    frame definitions for MRP interconnection frames       */
/*                    introduced                                             */
#endif
/*****************************************************************************/


/* packing of structures starts now */
#include "pnio_pck2_on.h"



/*---------------------------------------------------------------------------*/
/*              MRP packet data unit (MRP INPDU) basic definitions           */
/*---------------------------------------------------------------------------*/

#define MRP_IN_PACKET_VAL_TLV_TPLG_CHG         0x070A
#define MRP_IN_PACKET_VAL_TLV_POLL             0x0A0A

#define MRP_IN_PACKET_VAL_TLV_LNK_DOWN         0x080E
#define MRP_IN_PACKET_VAL_TLV_LNK_UP           0x090E


/* valid values for field MRP_PortRole */
#define MRP_IN_PACKET_VAL_PORT_ROLE_PRM        0x0000
#define MRP_IN_PACKET_VAL_PORT_ROLE_SEC        0x0001
#define MRP_IN_PACKET_VAL_PORT_ROLE_IN         0x0002


/* mandatory for LC mode! linkRole */
/* defines for linkRole field of frame MRP_IN_PACKET_STRUCT_PAYLOAD_LNK_CHG_TYPE */
/* according to IEC62439-2 Ed.2 byte 34 and 35 of InLnkChg payload are defined as padding[2] */
/* mandatory for LC-mode only: Byte 34 and 35 of InLnkChg frame payload are used to signalize linkRole and link error information from MIC to MIM */
/* at least linkRole (PRM, SEC and RDI bit) have to be delivered here */

/* Bit 13 - Bit 15:                    reserved */
/* Bit 12:                      AddCheckEnabled */
/* Bit 8 - Bit 11:                 ChangeReason */

/* Bit 1 - Bit 7:                      reserved */
/* Bit 0:                 linkRole (PRM or SEC) */

/* Bit 0: linkRole info */
#define MRP_IN_PACKET_VAL_LNK_ROLE_BIT0_PRM     0x0000   
#define MRP_IN_PACKET_VAL_LNK_ROLE_BIT0_SEC     0x0001

#define MRP_IN_PACKET_VAL_BITMASK_BIT0_SET      0x0001

/* offsets for the bit positions of link change reasons */
/* acquired by additional link check protocols          */
#define MRP_IN_PACKET_VAL_LNK_CHANGE_REASON_BIT_OFFSET  8
#define MRP_IN_PACKET_VAL_LNK_ADD_CHECK_BIT_OFFSET     12

/* Bit 8 - Bit 11:                 ChangeReason */
#define MRP_IN_PACKET_VAL_LNK_CHANGE_REASON_NONE      (0x0 << MRP_IN_PACKET_VAL_LNK_CHANGE_REASON_BIT_OFFSET)
#define MRP_IN_PACKET_VAL_LNK_CHANGE_REASON_UNDEF     (0x1 << MRP_IN_PACKET_VAL_LNK_CHANGE_REASON_BIT_OFFSET)
#define MRP_IN_PACKET_VAL_LNK_CHANGE_REASON_RDI       (0x2 << MRP_IN_PACKET_VAL_LNK_CHANGE_REASON_BIT_OFFSET)
#define MRP_IN_PACKET_VAL_LNK_CHANGE_REASON_TIMEOUT   (0x4 << MRP_IN_PACKET_VAL_LNK_CHANGE_REASON_BIT_OFFSET)
#define MRP_IN_PACKET_VAL_LNK_CHANGE_REASON_PHY_PROBL (0x8 << MRP_IN_PACKET_VAL_LNK_CHANGE_REASON_BIT_OFFSET)


/* Bit 12: AddCheckEnabled */
#define MRP_IN_PACKET_VAL_LNK_ADD_CHECK_DIS           (0x0 << MRP_IN_PACKET_VAL_LNK_ADD_CHECK_BIT_OFFSET)
#define MRP_IN_PACKET_VAL_LNK_ADD_CHECK_ENA           (0x1 << MRP_IN_PACKET_VAL_LNK_ADD_CHECK_BIT_OFFSET)


/* MRP interconnection packet size lengths: If the packet length is less than 64 Byte, at least 64 Bytes are sent. */
#define MRP_IN_PACKET_SIZE_DEFAULT                     64

/* MRP interconnection packet size for MRP InTC frames sent with Siemens option */
#define MRP_IN_PACKET_SIZE_IN_TPLG_CHG                 82


#define MRP_IN_PACKET_VAL_DST_ADR_TST     \
  "\x01\x15\x4E\x00\x00\x03" /* dst */

#define MRP_IN_PACKET_VAL_DST_ADR_WRK     \
  "\x01\x15\x4E\x00\x00\x04" /* dst */

#define MRP_IN_PACKET_VAL_DST_ADR_TRF     \
  "\x01\x15\x4E\x00\x00\x05" /* dst */


#define MRP_IN_PACKET_VAL_DEFAULT \
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

#define MRP_IN_PACKET_VAL_OPTION_COUPLED_UUID \
  "\x7F\x1A"                                                          /* LSA_UINT16  tlv_opt;                                           52 */\
  "\x08\x00\x06\x00"                                                  /* LSA_UINT8  oui_and_ed1Type[MRP_PACKET_VAL_OUI_LEN + MRP_PACKET_VAL_SO1_ED1TYPE_LEN]; 54 */\
  "\x00\x00"                                                          /* LSA_UINT16 so1Ed1ManData;                                      58 */\
  "\xF2\x12"                                                          /* LSA_UINT16 so2_tlv;                                            60 */\
  "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"  /* LSA_UINT8  coupledDomainUUID[MRP_PACKET_VAL_DOMAIN_UUID_LEN];  62 */\
  "\x00\x00"                                                          /* LSA_UINT8  padding[MRP_PACKET_VAL_PADDING_WORD_LEN];           78 */\
  "\x00\x00"                                                          /* LSA_UINT16 tlv_end;                                            80 */


#define MRP_IN_PACKET_VAL_NO_OUI               "\x00\x00\x00"


/*---------------------------------------------------------------------------*/
/* MRP interconnection packet data unit (MRP INPDU) basic length definitions */
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/* MRP interconnection packet data unit (MRP INPDU) structures               */
/*---------------------------------------------------------------------------*/

typedef PNIO_PACKED_ATTRIBUTE_PRE union _MRP_IN_PACKET_UNION_VLAN_TYPE
{
  LSA_UINT16 vlan_u16[2];                                /* 12: VLAN tag */
  LSA_UINT32 vlan_u32;                                   /* 12: VLAN tag complete */
} PNIO_PACKED_ATTRIBUTE_POST MRP_IN_PACKET_UNION_VLAN_TYPE;

/* pack this struct to word size */
typedef PNIO_PACKED_ATTRIBUTE_PRE struct _MRP_IN_PACKET_STRUCT_HEADER_TYPE
{
  LSA_UINT8  dst[MRP_PACKET_VAL_MAC_ADDR_LEN];           /*  0 */
  LSA_UINT8  src[MRP_PACKET_VAL_MAC_ADDR_LEN];           /*  6: source MAC address (port): PORT MAC address of sending device */
  MRP_PACKET_UNION_VLAN_TYPE vlan;                       /* 12: VLAN tag */
  LSA_UINT16 lt;                                         /* 16 */
                                                         /* 18 */
} PNIO_PACKED_ATTRIBUTE_POST MRP_IN_PACKET_STRUCT_HEADER_TYPE;


/*---------------------------------------------------------------------------*/
/*  MRP_IN_TC frame definition                                               */
/*---------------------------------------------------------------------------*/

/* MRP InTC frame extension: MRP domainUUID of coupled MRP ring */
/* pack this struct to word size */
typedef PNIO_PACKED_ATTRIBUTE_PRE struct _MRP_IN_PACKET_STRUCT_OPTION_UUID_TYPE
{
  LSA_UINT16 tlv_opt;                                           /* 52 */
  LSA_UINT8  oui_and_ed1Type[MRP_PACKET_VAL_OUI_LEN + MRP_PACKET_VAL_SO1_ED1TYPE_LEN];
                                                                /* 54 */         /* Byte 54 - 56: OUI   */
                                                                /* 57 */         /* Byte 57: so1Ed1Type */
  LSA_UINT16 so1Ed1ManData;                                     /* 58 */
  LSA_UINT16 so2_tlv;                                           /* 60 */
  LSA_UINT8  coupledDomainUUID[MRP_PACKET_VAL_DOMAIN_UUID_LEN]; /* 62 */
  LSA_UINT8  padding[MRP_PACKET_VAL_PADDING_WORD_LEN];          /* 78 */
  LSA_UINT16 tlv_end;                                           /* 80 */
} PNIO_PACKED_ATTRIBUTE_POST MRP_IN_PACKET_STRUCT_OPTION_UUID_TYPE;


/* MRP InTC frame extension: either frame uSuffix or domain UUID of coupled MRP ring */
/* pack this struct to word size */
typedef PNIO_PACKED_ATTRIBUTE_PRE union _MRP_IN_PACKET_UNION_PAYLOAD_EXT_TPLG_CHG_TYPE
{
  LSA_UINT16                                 tlv_end;          /* 52 */
  MRP_IN_PACKET_STRUCT_OPTION_UUID_TYPE      inTcOption;       /* 52 */
} PNIO_PACKED_ATTRIBUTE_POST MRP_IN_PACKET_UNION_PAYLOAD_EXT_TPLG_CHG_TYPE;



/* pack this struct to word size */
typedef PNIO_PACKED_ATTRIBUTE_PRE struct _MRP_IN_PACKET_STRUCT_PAYLOAD_TPLG_CHG_TYPE
{
  LSA_UINT16 tlv;                                            /* 20 */
  LSA_UINT8  sa[MRP_PACKET_VAL_MAC_ADDR_LEN];                /* 22 */  /* source MAC address (interface): Interface MAC Address of sending device */
  LSA_UINT16 inId;                                           /* 28 */
  LSA_UINT16 interval;                                       /* 30 */
  LSA_UINT16 tlv_common;                                     /* 32 */
  LSA_UINT16 sequenceID;                                     /* 34 */
  LSA_UINT8  domainUUID[MRP_PACKET_VAL_DOMAIN_UUID_LEN];     /* 36 */

  MRP_IN_PACKET_UNION_PAYLOAD_EXT_TPLG_CHG_TYPE uSuffix;     /* 52 */
                                                             /* without coupled UUID*/   /* with coupled UUID */
                                                             /* 54 */                    /* 82 */
} PNIO_PACKED_ATTRIBUTE_POST MRP_IN_PACKET_STRUCT_PAYLOAD_TPLG_CHG_TYPE;

/* pack this struct to word size */
typedef PNIO_PACKED_ATTRIBUTE_PRE struct _MRP_IN_PACKET_STRUCT_TPLG_CHG_TYPE
{
  MRP_IN_PACKET_STRUCT_HEADER_TYPE           hdr;            /* the following offset values are valid for VLAN tagged frames only, decrement by 4, if no VLAN tag is present */
  LSA_UINT16                                 ver;            /* 18 */
  MRP_IN_PACKET_STRUCT_PAYLOAD_TPLG_CHG_TYPE payload;        /* 20 */
} PNIO_PACKED_ATTRIBUTE_POST MRP_IN_PACKET_STRUCT_TPLG_CHG_TYPE;



/*---------------------------------------------------------------------------*/
/*  MRP_IN_POLL frame definition                                             */
/*---------------------------------------------------------------------------*/

/* pack this struct to word size */
typedef PNIO_PACKED_ATTRIBUTE_PRE struct _MRP_IN_PACKET_STRUCT_PAYLOAD_POLL_TYPE
{
  LSA_UINT16 tlv;                                        /* 20 */
  LSA_UINT8  sa[MRP_PACKET_VAL_MAC_ADDR_LEN];            /* 22 */  /* source MAC address (interface): Interface MAC Address of sending device */
  LSA_UINT16 portRole;                                   /* 28 */
  LSA_UINT16 inId;                                       /* 30 */
  LSA_UINT16 tlv_common;                                 /* 32 */
  LSA_UINT16 sequenceID;                                 /* 34 */
  LSA_UINT8  domainUUID[MRP_PACKET_VAL_DOMAIN_UUID_LEN]; /* 36 */
  LSA_UINT16 tlv_end;                                    /* 52 */
                                                         /* 54 */
} PNIO_PACKED_ATTRIBUTE_POST MRP_IN_PACKET_STRUCT_PAYLOAD_POLL_TYPE;

/* pack this struct to word size */
typedef PNIO_PACKED_ATTRIBUTE_PRE struct _MRP_IN_PACKET_STRUCT_POLL_TYPE
{
  MRP_IN_PACKET_STRUCT_HEADER_TYPE           hdr;        /* the following offset values are valid for VLAN tagged frames only, decrement by 4, if no VLAN tag is present */
  LSA_UINT16                                 ver;        /* 18 */
  MRP_IN_PACKET_STRUCT_PAYLOAD_POLL_TYPE     payload;    /* 20 */
} PNIO_PACKED_ATTRIBUTE_POST MRP_IN_PACKET_STRUCT_POLL_TYPE;


/*---------------------------------------------------------------------------*/
/*  MRP_IN_LNK_CHG frame definition                                          */
/*---------------------------------------------------------------------------*/

/* pack this struct to word size */
typedef PNIO_PACKED_ATTRIBUTE_PRE struct _MRP_IN_PACKET_STRUCT_PAYLOAD_LNK_CHG_TYPE
{
  LSA_UINT16 tlv;                                        /* 20 */
  LSA_UINT8  sa[MRP_PACKET_VAL_MAC_ADDR_LEN];            /* 22 */  /* source MAC address (interface): Interface MAC Address of sending device */
  LSA_UINT16 portRole;                                   /* 28 */
  LSA_UINT16 inId;                                       /* 30 */
  LSA_UINT16 interval;                                   /* 32 */
  LSA_UINT16 linkRole;                                   /* 34 */  /* according to IEC62439-2 Ed.2 this field is defined as padding[2]                     */
                                                                   /* mandatory for LC-mode only: linkRole (PRM, SEC and RDI bit) have to be delivered here */
  LSA_UINT16 tlv_common;                                 /* 36 */
  LSA_UINT16 sequenceID;                                 /* 38 */
  LSA_UINT8  domainUUID[MRP_PACKET_VAL_DOMAIN_UUID_LEN]; /* 40 */
  LSA_UINT16 tlv_end;                                    /* 56 */
                                                         /* 58 */
} PNIO_PACKED_ATTRIBUTE_POST MRP_IN_PACKET_STRUCT_PAYLOAD_LNK_CHG_TYPE;

/* pack this struct to word size */
typedef PNIO_PACKED_ATTRIBUTE_PRE struct _MRP_IN_PACKET_STRUCT_LNK_CHG_TYPE
{
  MRP_IN_PACKET_STRUCT_HEADER_TYPE           hdr;        /* the following offset values are valid for VLAN tagged frames only, decrement by 4, if no VLAN tag is present */
  LSA_UINT16                                 ver;        /* 18 */
  MRP_IN_PACKET_STRUCT_PAYLOAD_LNK_CHG_TYPE  payload;    /* 20 */
} PNIO_PACKED_ATTRIBUTE_POST MRP_IN_PACKET_STRUCT_LNK_CHG_TYPE;



/* packing of structures ends now */
#include "pnio_pck_off.h"



/*----------------------------------------------------------------------------*/
#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif  /* of MRP_INPDU_H */
