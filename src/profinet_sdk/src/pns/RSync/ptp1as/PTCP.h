/***********************************************************************************************************************
 * Copyright [2021-2024] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics Corporation and/or its affiliates and may only
 * be used with products of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.
 * Renesas products are sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for
 * the selection and use of Renesas products and Renesas assumes no liability.  No license, express or implied, to any
 * intellectual property right is granted by Renesas.  This software is protected under all applicable laws, including
 * copyright laws. Renesas reserves the right to change or discontinue this software and/or this documentation.
 * THE SOFTWARE AND DOCUMENTATION IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND
 * TO THE FULLEST EXTENT PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY,
 * INCLUDING WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE
 * SOFTWARE OR DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR
 * DOCUMENTATION (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER,
 * INCLUDING, WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY
 * LOST PROFITS, OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

#ifndef GPTP_PTCP_H
#define GPTP_PTCP_H

/* Time for receiving check of announce frames. Default value 10s */
#define PTCP_ANNO_TIMEOUT    10000
#define PTCP_ANNOUNCE_FACTOR     3
#define PDU_HDR_SIZE_NO_VLAN    16
#define MASTER_STATE_PRIMARY     6
#define SLAVE_STATE_SYNC         6
#define MAC_ADR_LENGTH           6
#define UUID_LENGTH             16
#define PTCP_SYNCID_MASK      0x1f

/* 030707lrg002: PDU structure and size with(out) VLAN tag 
*/
#ifdef USE_VLAN
#define	PDU_HDR			  PDU_HDR_VLAN
#define	PDU_HDR_SIZE	PDU_HDR_SIZE_VLAN
#else
#define	PDU_HDR			  PDU_HDR_NO_VLAN
#define	PDU_HDR_SIZE	PDU_HDR_SIZE_NO_VLAN
#endif

#define SWAP32(val)            \
	 (  ((( (val)) & 0xFF000000L) >> 24)   \
		+ ((( (val)) & 0x00FF0000L) >>  8)   \
		+ ((( (val)) & 0x0000FF00L) <<  8)   \
		+ ((( (val)) & 0x000000FFL) << 24))  \

#define TLV_DELAYPARAM_SIZE		8
#define TLV_PORTTIME_SIZE		  8
#define TLV_END_SIZE			    2
#define TLV_PORTPARAM_SIZE		12

#define DELAY_RSP_PDU_SIZE \
		(PDU_HDR_SIZE  + \
		 PTCP_HDR_SIZE + \
		 TLV_DELAYPARAM_SIZE + \
		 TLV_PORTPARAM_SIZE  + \
		 TLV_PORTTIME_SIZE   + \
		 TLV_END_SIZE)
			
#define DELAY_REQ_PDU_SIZE \
		(PDU_HDR_SIZE  + \
		 PTCP_HDR_SIZE + \
		 TLV_DELAYPARAM_SIZE + \
		 TLV_END_SIZE)
			
#define DELAY_FU_PDU_SIZE \
		(PDU_HDR_SIZE  + \
		 PTCP_HDR_SIZE + \
		 TLV_DELAYPARAM_SIZE + \
		 TLV_END_SIZE)
			
#define SYNC_PDU_SIZE \
		(GSY_PDU_HDR_SIZE  + \
		 GSY_PTCP_HDR_SIZE + \
		 GSY_TLV_SUBDOMAIN_SIZE + \
		 GSY_TLV_TIME_SIZE    + \
		 GSY_TLV_TIMEEXT_SIZE + \
		 GSY_TLV_MASTER_SIZE  + \
		 GSY_TLV_END_SIZE)
			
#define SYNC_FU_PDU_SIZE \
		(PDU_HDR_SIZE  + \
		 PTCP_HDR_SIZE + \
		 TLV_SUBDOMAIN_SIZE + \
		 TLV_TIME_SIZE + \
		 TLV_END_SIZE)
			
#define ANNOUNCE_PDU_SIZE \
		(PDU_HDR_SIZE_VLAN  + \
		 PTCP_HDR_SIZE      + \
		 TLV_SUBDOMAIN_SIZE + \
		 TLV_MASTER_SIZE    + \
		 TLV_END_SIZE)
		
#define PTCP_HDR_SIZE			     20
#define PTCP_DELAY10_OFFSET		 10	/* mit Frame-ID */
#define PTCP_DELAY1_OFFSET		 16	/* mit Frame-ID */
#define PTCP_DelayNS_OFFSET		 18	/* mit Frame-ID */
#define UUID_BYTE_LENGTH	      8
#define TLV_MASTER_SIZE			    8
#define TLV_SUBDOMAIN_SIZE     24
#define TLV_TIME_SIZE			     12
#define PDU_HDR_SIZE_VLAN		   20

/* Multicast addresses for PTCP telegrams
*/
#define PTCP_MULTICAST_ADDR_DELAY		 {0x01, 0x80, 0xC2, 0x00, 0x00, 0x0E}
#define PTCP_MULTICAST_ADDR_SYNC_IRT {0x01, 0x0E, 0xCF, 0x00, 0x01, 0x02}
#define PTCP_MULTICAST_ADDR_SYNC		 {0x01, 0x0E, 0xCF, 0x00, 0x04, 0x80}
#define PTCP_MULTICAST_ADDR_TIME		 {0x01, 0x0E, 0xCF, 0x00, 0x04, 0x81}
#define PTCP_MULTICAST_ADDR_FUSYNC	 {0x01, 0x0E, 0xCF, 0x00, 0x04, 0x20}
#define PTCP_MULTICAST_ADDR_FUTIME	 {0x01, 0x0E, 0xCF, 0x00, 0x04, 0x21}
#define PTCP_MULTICAST_ADDR_SYNC_FU	 {0x01, 0x0E, 0xCF, 0x00, 0x04, 0x40}
#define PTCP_MULTICAST_ADDR_TIME_FU	 {0x01, 0x0E, 0xCF, 0x00, 0x04, 0x41}
#define PTCP_MULTICAST_ADDR_ANNOUNCE {0x01, 0x0E, 0xCF, 0x00, 0x04, 0x00}
#define PTCP_MULTICAST_ADDR_ANNO_MAX {0x01, 0x0E, 0xCF, 0x00, 0x04, 0x1F}
#define PTCP_NO_MASTER_ADDR					 {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#define PTCP_EMPTY_ADDR							 {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

#define ETHERTYPE							 0x8892
#define TLV_PORTPARAM_TYPELEN	 0x0A0A
#define TLV_DELAYPARAM_TYPELEN 0x0C06
#define TLV_PORTTIME_TYPELEN	 0x0E06
#define TLV_END_TYPELEN			   0x0000
#define TLV_SUBDOMAIN_TYPELEN	 0x0216
#define TLV_MASTER_TYPELEN		 0x0806
#define TLV_TIMEEXT_TYPELEN		 0x0606
#define TLV_TIME_TYPELEN		   0x040A

#pragma pack(push, 1)

typedef union _WORD_TYPE
{
	LSA_UINT8       Octet[2];
	LSA_UINT16      Word;
} WORD_TYPE;

typedef union _DWORD_TYPE
{               
	LSA_UINT8       Octet[4];
	LSA_UINT16      Word[2];
	LSA_UINT32      Dword;
} DWORD_TYPE;

/* PTCP-Header
*/
typedef struct _PTCP_HDR
{
	DWORD_TYPE	ApiRecvTS;			/* Local ApiTimeStamp */
	DWORD_TYPE	ReceiveTS;			/* LocalReciveTimeStamp */
	DWORD_TYPE	Delay10ns;			/* Delay */
	WORD_TYPE	  SequenceID;			/* SequenceNumber */
	LSA_UINT8		Delay1ns;			  /* Sign Flags*/
	LSA_UINT8		Reserved3;			/* Reserved  */
	DWORD_TYPE	DelayNS;			  /* 32 Bit Nanoseconds */
} PTCP_HDR;

typedef struct _MAC_ADR_TYPE
{
	LSA_UINT8       MacAdr[MAC_ADR_LENGTH];
} MAC_ADR_TYPE;

/* PTCP-DelayParameter TLV for Delay PDUs
*/
typedef struct _TLV_DELAYPARAM
{
	WORD_TYPE		TypeLen;	/* 7 Bit Type, 9 Bit Length */
	MAC_ADR_TYPE	RequestSourceAddress; 
} TLV_DELAYPARAM;

/* PTCP-PortParameter TLV
*/
typedef struct _TLV_PORTPARAM
{
	WORD_TYPE	 TypeLen;		/* 7 Bit Type, 9 Bit Length */
	LSA_UINT8  Reserved1; 
	LSA_UINT8  Reserved2;
	DWORD_TYPE T2PortRxDelay; 
	DWORD_TYPE T3PortTxDelay; 
} TLV_PORTPARAM;

/* PTCP-DelayParameter TLV for Delay PDUs
*/
typedef struct _TLV_PORTTIME
{
	WORD_TYPE		TypeLen;	/* 7 Bit Type, 9 Bit Length */
	LSA_UINT8   Reserved1; 
	LSA_UINT8   Reserved2;
	DWORD_TYPE	T2TimeStamp; 
} TLV_PORTTIME;

/* PTCP-End TLV
*/
typedef struct _TLV_END
{
	/* 7 Bit Type, 9 Bit Length */
	WORD_TYPE	TypeLen;
} TLV_END;

/* Structure of Delay response messages
*/
typedef struct _DELAY_RSP_PDU
{
	PTCP_HDR			  PtcpHdr;
	TLV_DELAYPARAM	DelayParam;
	TLV_PORTPARAM		PortParam;
	TLV_PORTTIME		PortTime;
	TLV_END				  End;
} DELAY_RSP_PDU;

/* Structure of Delay-FollowUp messages
*/
typedef struct _DELAY_FU_PDU
{
	PTCP_HDR			 PtcpHdr;
	TLV_DELAYPARAM DelayParam;
	TLV_END				 End;
} DELAY_FU_PDU;

/* Structure of Delay request messages
*/
typedef struct _DELAY_REQ_PDU
{
	PTCP_HDR			 PtcpHdr;
	TLV_DELAYPARAM DelayParam;
	TLV_END				 End;
} DELAY_REQ_PDU;

typedef struct _PTCP_UUID
{
	DWORD_TYPE  Data1;
	WORD_TYPE   Data2;
	WORD_TYPE   Data3;
	LSA_UINT8   Data4[UUID_BYTE_LENGTH];
} PTCP_UUID;

/* PTCP-Subdomain TLV
*/
typedef struct _TLV_SUBDOMAIN
{
	WORD_TYPE		 TypeLen;	/* 7 Bit Type, 9 Bit Length */
	MAC_ADR_TYPE MasterSourceAddress; 
	PTCP_UUID		 SubdomainUUID;
} TLV_SUBDOMAIN;

/* PTCP-Time TLV
*/
typedef struct _TLV_TIME
{
	WORD_TYPE	 TypeLen;		/* 7 Bit Type, 9 Bit Length */
	WORD_TYPE	 EpochNumber; 
	DWORD_TYPE Seconds; 
	DWORD_TYPE NanoSeconds;
} TLV_TIME;

/* PTCP-TimeExtension TLV
*/
typedef struct _TLV_TIMEEXT
{
	WORD_TYPE	TypeLen;		/* 7 Bit Type, 9 Bit Length */
	WORD_TYPE	Flags; 
	WORD_TYPE	CurrentUTCOffset; 
	LSA_UINT8	Reserved1;
	LSA_UINT8	Reserved2;
} TLV_TIMEEXT;

/* PTCP-Grandmaster TLV
*/
typedef struct _TLV_MASTER
{
	WORD_TYPE	TypeLen;		/* 7 Bit Type, 9 Bit Length */
	LSA_UINT8	MasterPriority1; 
	LSA_UINT8	MasterPriority2; 
	LSA_UINT8	ClockClass; 
	LSA_UINT8	ClockAccuracy; 
	WORD_TYPE	ClockVariance; 
} TLV_MASTER;

/* Structure of Sync messages
*/
typedef struct _TLV_K32
{
	TLV_SUBDOMAIN	Subdomain;
	TLV_TIME		  Time;
	TLV_TIMEEXT		TimeExt;
	TLV_MASTER		Master;
	TLV_END			  End;
} TLV_K32;

typedef struct _SYNC_PDU
{
	PTCP_HDR		PtcpHdr;
	TLV_K32     TLV;
} SYNC_PDU;

/* Structure of Sync-FollowUp messages
*/
typedef struct _SYNC_FU_PDU
{
	PTCP_HDR		  PtcpHdr;
	TLV_SUBDOMAIN	Subdomain;
	TLV_TIME		  Time;
	TLV_END			  End;
} SYNC_FU_PDU;

/* Structure of Announce messages
*/
typedef struct _ANNOUNCE_PDU
{
	PTCP_HDR		  PtcpHdr;
	TLV_SUBDOMAIN	Subdomain;
	TLV_MASTER		Master;
	TLV_END			  End;
} ANNOUNCE_PDU;

/* Union of all PTCP messages
*/
typedef union _PTCP_DATA //das sind die buffer
{
	SYNC_PDU			 Sync;
	SYNC_FU_PDU		 SyncFu;
	ANNOUNCE_PDU	 Announce;
	DELAY_REQ_PDU	 DelayReq;
	DELAY_RSP_PDU	 DelayRsp;
	DELAY_FU_PDU	 DelayFu;
} PTCP_DATA;

/* PROFINET-Header without VLAN tag
*/
typedef struct _PDU_HDR_NO_VLAN
{
	MAC_ADR_TYPE	DstMacAdr; /* Destination MAC address */
	MAC_ADR_TYPE	SrcMacAdr; /* Source MAC address */
	WORD_TYPE		EthType;		 /* Ethertype */
	WORD_TYPE		FrameId;		 /* Frame-ID */
} PDU_HDR_NO_VLAN;

/* Structure of all PTCP messages
*/
typedef struct _RSYNC_PTCP
{
	PDU_HDR				PduHdr;
	PTCP_DATA			PtcpData;
} RSYNC_PTCP;

#pragma pack(pop)

#endif
