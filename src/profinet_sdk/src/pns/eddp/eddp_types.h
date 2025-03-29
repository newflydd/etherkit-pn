#ifndef __EDDP_TYPES_H__                  /* ----- reinclude-protection ----- */
#define __EDDP_TYPES_H__

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

/*****************************************************************************/
/*  Copyright (C) 2021  All rights reserved.      */
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
/*  C o m p o n e n t     &C: EDDP (EthernetDeviceDriver for Std. MAC)  :C&  */
/*                                                                           */
/*  F i l e               &F: rzt2_edd_dat.h                              :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.17         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2021-09-15                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  Low Layer user header (TEMPLATE).                */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
/*****************************************************************************/
#define EDDP_SYS_HDL		0xDEU


#define EDDP_LL_MSG_IRQ_ETHSW			(1U)
#define EDDP_LL_MSG_IRQ_GMAC			(2U)
#define EDDP_LL_MSG_LINK_IND			(3U)
#define EDDP_LL_MSG_SET_LINK			(4U)
#define EDDP_LL_MSG_GET_LINK			(5U)
#define EDDP_LL_MSG_CHG_PORT			(6U)
#define EDDP_LL_MSG_MC_CTRL				(9U)
#define EDDP_LL_MSG_PHYCTRL				(10U)
#define EDDP_LL_MSG_CLOSE				(11U)

#define EDDP_LL_EV_LINK_SET			(0x0001U)
#define EDDP_LL_EV_LINK_CHG			(0x0002U)
#define EDDP_LL_EV_PORT_CHG			(0x0004U)
#define EDDP_LL_EV_SWI_PORT_STAT	(0x0008U)
#define EDDP_LL_EV_SWI_FWD			(0x0010U)
#define EDDP_LL_EV_SWI_FLUSH		(0x0020U)
#define EDDP_LL_EV_MC_CTRL			(0x0040U)
#define EDDP_LL_EV_LINK				(0x0100U)
#define EDDP_LL_EV_RCV				(0x0200U)
#define EDDP_LL_EV_SND				(0x0400U)
#define EDDP_LL_EV_CMD_REQ			(0x0800U)
#define EDDP_LL_EV_K32_REQ			(0x1000U)
#define EDDP_LL_EV_MAIL				(0x0800U)
#define EDDP_LL_EV_DHTEXPIRE		(0x2000U)


#define EDDP_LLIF_STATE_NONE			(0x0000U)
#define EDDP_LLIF_STATE_OPEN			(0x0001U)
#define EDDP_LLIF_STATE_SETUP			(0x0002U)
#define EDDP_LLIF_STATE_OPERATIONAL		(EDDP_LLIF_STATE_OPEN | EDDP_LLIF_STATE_SETUP)

#define EDDP_LL_RTCX_DATA_MIN			(40U)
#define EDDP_LL_ETHER_TYPE_FILTER_MAX	(10U)
#define EDDP_LL_ETHER_TYPE_FILTER_TIMER (1U) // Tiemr Periodic

/* ---------------------------------------------------------------------------------------------------- */
/* MultiCast MAC addresses                                                                              */
/* ---------------------------------------------------------------------------------------------------- */
#define EDDP_MC_MAC_ADR_RTC3        { 0x01, 0x0E, 0xCF, 0x00, 0x01, 0x01 }  // RTC3
#define EDDP_MC_MAC_ADR_FFW         { 0x03, 0x00, 0x00, 0x00, 0x00, 0x00 }  // FastForwarding
/* ---------------------------------------------------------------------------------------------------- */

/* PHYPower */
#define EDDP_PHY_POWER_ON        0  /* Switch the Phy on, -> Link-Ability    */
#define EDDP_PHY_POWER_OFF       1  /* Switch the Phy off -> No Link-Ability */
#define EDDP_PHY_POWER_UNCHANGED 2  /* No Changes on Phy                     */
/* for use with EDD_SRV_SETUP_PHY        */



#if 0
#define ETH_TYPE_VLAN		0x8100U
#define ETH_TYPE_IPV4		0x0800U
#define ETH_TYPE_ARP		0x0806U
#define ETH_TYPE_MRP		0x88E3U
#define ETH_TYPE_DLR		0x88E1U
#define ETH_TYPE_LLDP		0x88CCU
#define ETH_TYPE_PROFIENT 	0x8892U
#define ETH_TYPE_1588		0x88F7U
#define ETH_TYPE_LEN		0x0600U

#define ETH_IPV4_H4500		0x4500U
#define ETH_IPV4_H4000		0x4000U
#else
// Little-Endian
#define ETH_TYPE_VLAN		0x0081U
#define ETH_TYPE_IPV4		0x0008U
#define ETH_TYPE_IPV6		0xDD86U
#define ETH_TYPE_ARP		0x0608U
#define ETH_TYPE_MRP		0xE388U
#define ETH_TYPE_DLR		0xE188U
#define ETH_TYPE_LLDP		0xCC88U
#define ETH_TYPE_PROFINET	0x9288U
#define ETH_TYPE_1588		0xF788U
#define ETH_TYPE_LEN		0x0006U

#define ETH_IPV4_H4500		0x0045U
#define ETH_IPV4_H4000		0x0004U

#endif


#define FRAMEID_DCP_HELLO			0xFEFCU
#define FRAMEID_DCP					0xFEFDU
#define FRAMEID_DCP_IDENT_REQ		0xFEFEU
#define FRAMEID_DCP_IDENT_RSP		0xFEFFU

#define FRAMEID_ALARM_HIGH			0xFC01U
#define FRAMEID_ALARM_LOW			0xFE01U
#define FRAMEID_EVENT_LOW			0xFE02U

#define FRAMEID_PTCP_DELAY_START	0xFF40U
#define FRAMEID_PTCP_DELAY_END		0xFF43U

#define FRAMEID_PTCP_DELAY_REQ		0xFF40U
#define FRAMEID_PTCP_DELAY_RSP		0xFF41U
#define FRAMEID_PTCP_FOLLOWUP_DLY	0xFF42U
#define FRAMEID_PTCP_FOLLOWUP_RSP	0xFF43U


#define FRAMEID_PTCP_SYNC			0x0080U // Cyclic PTCP
#define FRAMEID_PTCP_SYNC_81		0x0081U	// Cyclic PTCP

#define FRAMEID_PTCP_RTSYNC_CYCL	0xFF00U // Acyclic PTCP
#define FRAMEID_PTCP_RTSYNC_TIME	0xFF01U // Acyclic PTCP
#define FRAMEID_PTCP_FOLLOWUP_CYCL	0xFF20U // Acyclic PTCP FollowUp
#define FRAMEID_PTCP_FOLLOWUP_TIME	0xFF21U // Acyclic PTCP FollowUp

#define FRAMEID_RTC3_START			0x0100U	// CYCL
#define FRAMEID_RTC3_END			0x06FFU	// CYCL

#define FRAMEID_RTC2_START	0x8000U
#define FRAMEID_RTC2_END	0xBFFFU

#define FRAMEID_RTC1_START	0xC000U
#define FRAMEID_RTC1_END	0xFBFFU

#define FRAMEID_RTCX_START	0x0100U
#define FRAMEID_RTCX_END	0xFBFFU







#include "sys_pck1.h"
typedef struct _s_eth_hdr
{
	LSA_UINT8  dst[6];
	LSA_UINT8  src[6];
	LSA_UINT16 type;
}ETH_HDR, *ETH_HDR_PTR;

typedef struct _s_eth_hdr_mgmttag
{
	LSA_UINT8  dst[6];
	LSA_UINT8  src[6];
	LSA_UINT16 mgmt_tag[4];
	LSA_UINT16 type;
}ETH_HDR_MGMTTAG, *ETH_HDR_MGMTTAG_PTR;

typedef struct _eth_hdr_vlan
{
	LSA_UINT8  dst[6];
	LSA_UINT8  src[6];
	LSA_UINT16 vlan_tag;
	LSA_UINT16 vlan_id;
	LSA_UINT16 type;
}ETH_HDR_VLAN, *ETH_HDR_VLAN_PTR;

typedef struct _eth_hdr_vlan_mgmttag
{
	LSA_UINT8  dst[6];
	LSA_UINT8  src[6];
	LSA_UINT16 mgmt_tag[4];
	LSA_UINT16 vlan_tag;
	LSA_UINT16 vlan_id;
	LSA_UINT16 type;
}ETH_HDR_VLAN_MGMTTAG, *ETH_HDR_VLAN_MGMTTAG_PTR;



typedef struct _u_eth_hdr
{
	ETH_HDR 	 tStd;
	ETH_HDR_VLAN tVlan;
}TU_ETH_HDR_VLAN, *TU_ETH_HDR_VLAN_PTR;

typedef struct _dcp_header
{
	LSA_UINT16 frameid;
	LSA_UINT8  service_id;
	LSA_UINT8  service_type;
	LSA_UINT32 xid;
	LSA_UINT16 res_delay;
	LSA_UINT16 dcpdata_length;
	LSA_UINT8  option;
	LSA_UINT8  suboption;
	LSA_UINT16 dcpblock_length;
	LSA_UINT8  data[0U];
}DCP_HDR, *DCP_HDR_PTR;

typedef struct _arp_header
{
	LSA_UINT16 hardware;
	LSA_UINT16 protocol;
	LSA_UINT8  hardware_length;
	LSA_UINT8  protocol_length;
	LSA_UINT16 opecode;
	LSA_UINT8  sha[6];
	LSA_UINT8  spa[4];
	LSA_UINT8  tha[6];
	LSA_UINT8  tpa[4];
}ARP_HDR, *ARP_HDR_PTR;

typedef struct _ipv4_header
{
	LSA_UINT16 VerLen;
	LSA_UINT16 length;
	LSA_UINT16 ident;
	LSA_UINT16 flags;
	LSA_UINT8  tlv;
	LSA_UINT8  protocol;
	LSA_UINT16 checksum;
	LSA_UINT8  ip_src[4];
	LSA_UINT8  ip_dst[4];
}IPV4_HDR, *IPV4_HDR_PTR;


typedef struct _rtcx_farame_hdr
{
	LSA_UINT8  dst[6];
	LSA_UINT8  src[6];
	LSA_UINT16 vlan_tag;
	LSA_UINT16 vlan_id;
	LSA_UINT16 eth_type;
	LSA_UINT16 frame_id;
	LSA_UINT8  Data[0U];
}RTCX_HDR, *RTCX_HDR_PTR;

typedef struct _rtcx_farame_hdr_no_vlan
{
	LSA_UINT8  dst[6];
	LSA_UINT8  src[6];
	LSA_UINT16 eth_type;
	LSA_UINT16 frame_id;
	LSA_UINT8  Data[0U];
}RTCX_HDR_NV, *RTCX_HDR_PTR_NV;

#define MGMT_TAG_SIZE 8
typedef struct _rtcx_snd_hdr
{
	LSA_UINT8  dst[6];
	LSA_UINT8  src[6];
	LSA_UINT16 mgmt_tag;
	LSA_UINT16 mgmt_tag_data;
	LSA_UINT16 mgmt_tag_data20;
	LSA_UINT16 mgmt_tag_data21;
	LSA_UINT16 vlan_tag;
	LSA_UINT16 vlan_id;
	LSA_UINT16 eth_type;
	LSA_UINT16 frame_id;
	LSA_UINT8  Data[0U];
}RTCX_SND_HDR, *RTCX_SND_HDR_PTR;

typedef struct _rtcx_snd_hdr_no_vlan
{
	LSA_UINT8  dst[6];
	LSA_UINT8  src[6];
	LSA_UINT16 mgmt_tag;
	LSA_UINT16 mgmt_tag_data;
	LSA_UINT16 mgmt_tag_data20;
	LSA_UINT16 mgmt_tag_data21;
	LSA_UINT16 eth_type;
	LSA_UINT16 frame_id;
	LSA_UINT8  Data[0U];
}RTCX_SND_HDR_NV, *RTCX_SND_HDR_PTR_NV;

typedef struct _s_rtx_apdu
{
    LSA_UINT16 cycle_counter;
    LSA_UINT8  data_status;
    LSA_UINT8  transfer_status;
}RTC_APDU, *RTC_APDU_PTR;

#include "sys_unpck.h"

#define RTCX_BUFF_SIZE 	EDD_CFG_RX_FRAME_BUFFER_LENGTH

#if 1
/** 
 * @ishikawa: 
 * Add CSB activate stage enum for controlling CSB interrupt.
 */
typedef enum _eddp_llif_csb_activate_stage
{
    EDDP_LLIF_CSB_NOT_ACTIVATE = 0,
    EDDP_LLIF_CSB_ACTIVATE_STAGE1 = 1,
    EDDP_LLIF_CSB_ACTIVATE_STAGE2 = 2,
} EDDP_LLIF_CSB_ACTIVATE_STAGE;
#endif

typedef struct _rtcx_pkt
{
	LSA_UINT16 State;
	LSA_UINT16 PortID;
	LSA_UINT16 HwPortID;
	LSA_UINT32 TimeStamp;
	LSA_UINT16 IoDataSize;
	LSA_VOID*  pResource;
	LSA_VOID*  pDescriptor;
	RTC_APDU_PTR  pApdu;
	LSA_UINT32 Status;
	LSA_UINT16 BuffSize;
	LSA_UINT8  Buffer[RTCX_BUFF_SIZE];
#if 1   
    /** 
     * @ishikawa: 
     * Add CSB stage for controlling CSB interrupt.
     */
    EDDP_LLIF_CSB_ACTIVATE_STAGE CSBstage;
#endif
}RTCX_PKT, *RTCX_PKT_PTR;


#if 0
typedef struct _rtcx_snd_pkt
{
	LSA_BOOL   bBusy;
	LSA_UINT16 PortID;
	LSA_UINT32 TimeStamp;
	LSA_UINT16 BufferSize;
	LSA_UINT16 DataSize;
	LSA_UINT8* pBuffer;
	LSA_VOID*  pApdu;
	LSA_VOID*  hProvider;
	LSA_VOID*  hResource;
}RTCX_SND_PKT, *RTCX_SND_PKT_PTR;

typedef struct _rtcx_rcv_pkt
{
	LSA_UINT16 PortID;
	LSA_UINT32 TimeStamp;
	LSA_UINT16 DataSize;
	LSA_UINT8  Buffer[RTCX_BUFF_SIZE];
}RTCX_RCV_PKT, *RTCX_RCV_PKT_PTR;
#endif

typedef struct _pnio_timing
{
	LSA_UINT32 SendClock;
	LSA_UINT32 CycleTime;
	LSA_UINT32 RedPhase;
	LSA_UINT32 GreenPhase;
	LSA_UINT32 YellowPhase;
}PNIO_TIMING, *PNIO_TIMING_PTR;

/*---------------------------------------------------------------------------*/
/* actual LinkStatus/Speed and Mode of PHY                                   */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_LINK_CHANGED_STRUCT
{
    LSA_UINT8   LinkStateChanged;   /* indicates a pending change of link state */
    LSA_UINT8   PortStateChanged;   /* indicates a pending change of port state */
}EDDP_LINK_CHANGE_TYPE;

#define EDDP_LINK_STATE_NO_CHANGE       0 /* no pending change */
#define EDDP_LINK_STATE_CHANGE_PENDING  1 /* a pending change */

typedef struct _EDDP_LINK_STAT_STRUCT
{
    LSA_UINT8               Status;
    LSA_UINT8               Speed;
    LSA_UINT8               Mode;
    LSA_UINT8               Autoneg;        /* Autoneg configuration: EDD_AUTONEG_ON,  EDD_AUTONEG_OFF                         */
    LSA_UINT16              MAUType;        /* MAU Type                                */
    LSA_UINT8               MediaType;      /* Media Type                              */
    LSA_UINT8               OpticalTransType; /* is POF/SFP/OTHER                      */
    LSA_UINT8               PortStatus;     /* Status of Port                          */
    LSA_UINT8               PhyStatus;      /* PHYStatus of Port                       */
    LSA_UINT8               IsPulled;       /* ChangeMediaTypeinRun value              */
    LSA_UINT16              PortState;      /* State  of Port                          */
    LSA_UINT32              LineDelay;      /* LineDelay set by EDD_SRV_SET_LINE_DELAY */
    LSA_UINT32              CableDelayNs;   /* CableDel. set by EDD_SRV_SET_LINE_DELAY */
    LSA_UINT32              AutonegCapAdvertised;      /* autoneg advertised capabil.  */
    LSA_UINT8               AddCheckEnabled;
    LSA_UINT8               ChangeReason;
    EDDP_LINK_CHANGE_TYPE   LinkChange;     /* sub structure of pending changes in states */
} EDDP_LINK_STAT_TYPE;

typedef struct _EDDP_LINK_STAT_STRUCT EDDP_LOCAL_MEM_ATTR *         EDDP_LINK_STAT_PTR_TYPE;
typedef struct _EDDP_LINK_STAT_STRUCT EDDP_LOCAL_MEM_ATTR const *   EDDP_LINK_STAT_PTR_TO_CONST_TYPE;


typedef struct _EDDP_LL_PORTCTRL_TYPE
{
	LSA_UINT8 			 PortID;
    LSA_UINT8            PhyPowerMode;
    LSA_UINT8            LinkSpeedModeCfg;
    LSA_UINT8            SetLinkPending;
    LSA_UINT32           dwAutonegCap;      /* Autoneg capabil. */
    LSA_UINT32           TxLatency;
    LSA_UINT32     		 RxLatency;
	EDDP_LINK_STAT_TYPE  tLinkStat;
	EDDP_GET_STATISTICS_TYPE tStatistic;
}EDDP_LL_PORTCTRL_TYPE, *EDDP_LL_PORTCTRL_PTR_TYPE;

typedef struct _EDDP_LL_MCFWD_TYPE
{
	EDD_RQB_PORTID_MODE_TYPE tPortIDModeArray[EDD_CFG_MAX_PORT_CNT];
	LSA_UINT16               PortIDModeCnt;
	LSA_UINT16               MACAddrGroup;
	LSA_UINT16               MACAddrPrio;
	LSA_UINT32               MACAddrLow;
}EDDP_LL_MCFWD_TYPE, *EDDP_LL_MCFWD_PTR_TYPE;

#define EDDP_MAX_RX_PKT		NUM_RX_DESCRIPTOR
typedef struct _EDDP_PKT_TYPE
{
	LSA_UINT16 nId;
	LSA_UINT16 nIfId;
	LSA_UINT16 nPort;
	LSA_UINT32 nTimeNsec;
	LSA_UINT32 nTimeSec;
	LSA_UINT16 nDataSize;
	LSA_UINT8  abData[ETHER_BUFFER_SIZE];
}EDDP_PKT_TYPE, *EDDP_PKT_PTR_TYPE;

typedef struct _EDDP_TX_INFO_TYPE
{
	LSA_VOID  *hUpper;
	LSA_VOID  *pDescriptor;
} EDDP_TX_INFO_TYPE, *EDDP_TX_INFO_PTR_TYPE;

typedef struct _EDDP_LL_HANDLE_TYPE
{
	LSA_UINT32 				InterfaceID;
	LSA_UINT16 				u16State;
	LSA_UINT32              TraceIdx;
	LSA_UINT32 			    dwLinkEvent;
	LSA_UINT32  			nLastErr;
	LSA_UINT32              IfCtrlLock;
	LSA_UINT32				IfSendLock;
	LSA_UINT32				IfCtrlTaskId;
	LSA_UINT32              dwEddpIrqLow;
	LSA_UINT32              dwEddpIrqMid;
	LSA_UINT32              dwEddpIrqHigh;
	LSA_UINT32				dwLinkStatusAll;
	LSA_UINT32 				dwRxStatReg;
	LSA_UINT16 				u16LinkTimerId;
	LSA_UINT16              Rtc3TxCnt;
	LSA_UINT16              RtcxTxCnt;
	LSA_UINT16              Rtc3TxDone;
	LSA_UINT16              RtcxTxDone;
	LSA_UINT16              NrtTxReq;
	LSA_BOOL				bEddpIrqGrp;
	LSA_UINT32				FlowCtrl;
	LSA_VOID_PTR_TYPE       hDDB;
	LSA_VOID_PTR_TYPE       hDPB;
	EDDP_SYS_HANDLE       	hSysDev;
	LSA_VOID_PTR_TYPE 		hQCmdRsp;
	LSA_VOID_PTR_TYPE 		hQK32Rsp;
	LSA_VOID_PTR_TYPE 		pRxBuf;
	LSA_VOID_PTR_TYPE  		pIfHandle;
	LSA_UINT8 				MacAddrIF[6];
	EDDP_LL_PORTCTRL_TYPE   tPortCtrl[EDD_CFG_MAX_PORT_CNT +1U]; /* +1: Switch Internal Port--> Port 4!!! */
}EDDP_LL_HANDLE_TYPE, *EDDP_LL_HANDLE_PTR_TYPE;

typedef struct _EDDP_ETHER_TYPE_FILTER_TYPE
{
	LSA_UINT16				EtherType;
	LSA_UINT16				FilterPeriod;
	LSA_UINT32				FrameLimit;
	LSA_UINT32				FrameCounter;
} EDDP_ETHER_TYPE_FILTER_TYPE, *EDDP_ETHER_TYPE_FILTER_PTR_TYPE;

//=================================================================================================


#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif  /* __EDDP_TYPES_H__ */
