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

/*
  PTP Profile:
  IEEE Std 802.1ASTM-201x PTP profile for transport of timing over full-duplex, point-to-point links
  Version 1.0
  Profile identifier: 00-80-C2-00-01-00

  G.2 PTP attribute values
  The ranges and default values for time-aware system attributes covered by this profile are:

  a) The domain number is 0 (see 8.1)
  b) The default logAnnounceInterval (see 10.6.2.2) is 0. The value 127 is supported.
  c) The default logSyncInterval (see 11.5.2.3) is -3. The value 127 is supported.
  d) The default logPdelayReqInterval (see 11.5.2.2) is 0. The value 127 is supported.
  e) The default announceReceiptTimeout (see 10.6.3.2) is 2.
  f) The default values of priority1, for different media, are specified in 8.6.2.1, Table 8-2. The value of
     priority1 for a time-aware system that is not grandmaster-capable is 255.
  g) The default value of priority2 is 248 (see 8.6.2.5).
  h) The default observation interval for offsetScaledLogVariance is equal to the default sync interval,
  i.e., 0.125 s (see 8.6.2.4).

  802.1AS defines a profile according to 1588 Annex J with the following parameters: 
  � Device types: �end stations� and �bridges� (ordinary clocks and boundary-transparent clocks)
  � Multicast messages (no unicast messages) all using MAC address 01-80-C2-00-00-0E 
  � VLAN tagging prohibited
  � Peer-to-peer delay calculation (no end-to-end) 
  � 2-step only 

  � Default settings: 
    priority 1: slave-only = 255, master-capable = 128 
    priority 2 = 248 (default) 
*/

#ifndef GPTP_CMN_H
#define GPTP_CMN_H

#include "dep/datatypes_dep.h"

//EDDP/PNO includes
#include "compiler.h"
#include "lsa_cfg.h"
#include "edd_cfg.h"
#include "lsa_types.h"
#include "os.h"
#include "lsas_inc.h"
#include "lsas_int.h"
#include "lsas_dat.h"
#include "eddp_inc.h"
#include "eddp_int.h"           
#include "pnio_types.h"
#include "common_data.h"
#include "eddp_types.h"
#include "edd_usr.h"

#include "PTCP.h"
#include "gsy_int.h"

/* retuen codes */
#define R_OK     1
#define R_FAILED 0

/* Buffer sizes */
#define GPTP_TX_BUF_SIZE                  1024
#define GPTP_RX_BUF_SIZE                  4096
#define GPTP_CON_TS_BUF_SIZE              1024

/* List of timers */
#define GPTP_TIMER_DELAYREQ_RPT           0
#define GPTP_TIMER_DELAYREQ_TO            1
#define GPTP_TIMER_ANNOUNCE_RPT           2
#define GPTP_TIMER_ANNOUNCE_TO            3
#define GPTP_TIMER_SYNC_RPT               4
#define GPTP_TIMER_SYNC_TO                5
#define GPTP_TIMER_SYNC_TAKEOVER          6

#define GPTP_NUM_TIMERS                   7
#define GPTP_TIMER_INVALID                GPTP_NUM_TIMERS

/* Event destinations */
#define GPTP_EVT_DEST_MASK                0xffff0000
#define GPTP_EVT_DEST_ALL                 0x00000000
#define GPTP_EVT_DEST_DM                  0x00010000
#define GPTP_EVT_DEST_BMC                 0x00020000
#define GPTP_EVT_DEST_CS                  0x00040000

/* list of common events */
#define GPTP_EVT_NONE                     (GPTP_EVT_DEST_ALL | 0x0)
#define GPTP_EVT_STATE_ENTRY              (GPTP_EVT_DEST_ALL | 0x1)
#define GPTP_EVT_STATE_EXIT               (GPTP_EVT_DEST_ALL | 0x2)

/* list of delay events */
#define GPTP_EVT_DM_ENABLE                (GPTP_EVT_DEST_DM | 0x0)
#define GPTP_EVT_DM_PDELAY_REQ            (GPTP_EVT_DEST_DM | 0x1)
#define GPTP_EVT_DM_PDELAY_RESP           (GPTP_EVT_DEST_DM | 0x2)
#define GPTP_EVT_DM_PDELAY_RESP_FLWUP     (GPTP_EVT_DEST_DM | 0x3)
#define GPTP_EVT_DM_PDELAY_REQ_RPT        (GPTP_EVT_DEST_DM | 0x4)
#define GPTP_EVT_DM_PDELAY_REQ_TO         (GPTP_EVT_DEST_DM | 0x5)
#define GPTP_EVT_DM_PDELAY_FURESP         (GPTP_EVT_DEST_DM | 0x6)

/* list of best master clock events */
#define GPTP_EVT_BMC_ENABLE               (GPTP_EVT_DEST_BMC | 0x0)
#define GPTP_EVT_BMC_ANNOUNCE_RPT         (GPTP_EVT_DEST_BMC | 0x4)
#define GPTP_EVT_BMC_ANNOUNCE_TO          (GPTP_EVT_DEST_BMC | 0x5)
#define GPTP_EVT_BMC_ANNOUNCE_MSG         (GPTP_EVT_DEST_BMC | 0x6)

/* list of sync events */
#define GPTP_EVT_CS_ENABLE                (GPTP_EVT_DEST_CS | 0x0)
#define GPTP_EVT_CS_SYNC_RPT              (GPTP_EVT_DEST_CS | 0x4)
#define GPTP_EVT_CS_SYNC_TO               (GPTP_EVT_DEST_CS | 0x5)
#define GPTP_EVT_CS_SYNC_MSG              (GPTP_EVT_DEST_CS | 0x6)
#define GPTP_EVT_CS_SYNC_FLWUP_MSG        (GPTP_EVT_DEST_CS | 0x7)
#define GPTP_EVT_CS_SYNCFU_MSG						(GPTP_EVT_DEST_CS | 0x8)
#define GPTP_EVT_CS_SYNC_TAKEOVER         (GPTP_EVT_DEST_CS | 0x9)

/* GPTP message types */
#define GPTP_MSG_TYPE_SYNC                0x00
#define GPTP_MSG_TYPE_PDELAY_REQ          0x02
#define GPTP_MSG_TYPE_PDELAY_RESP         0x03
#define GPTP_MSG_TYPE_SYNC_FLWUP          0x08
#define GPTP_MSG_TYPE_PDELAY_RESP_FLWUP   0x0A
#define GPTP_MSG_TYPE_ANNOUNCE            0x0B
#define GPTP_MSG_TYPE_SYNCFU              0x0C

/* GPTP constants */
#define GPTP_VERSION_NO                   0x02
#define GPTP_TRANSPORT_L2                 0x10

#define GPTP_CONTROL_SYNC                 0x00
#define GPTP_CONTROL_SYNC_FLWUP           0x02
#define GPTP_CONTROL_DELAY_ANNOUNCE       0x05

#define GPTP_TLV_TYPE_ORG_EXT             0x0003
#define GPTP_TLV_TYPE_PATH_TRACE          0x0008

#define GPTP_LOG_MSG_INT_NOCHANGE         0x80
#define GPTP_LOG_MSG_INT_INIT             0x7E
#define GPTP_LOG_MSG_INT_MAX              0x7F

#define SLAVE_ONLY                        255
#define MASTER_ONLY                        4

/* GPTP Clock values */
/*
  8.6.2.1 priority1 of 250 are for:
  Devices that can go away (physically or otherwise) at any time. This includes devices 
  that are designed to be transient to the network. For example, these include laptop com-
  puters, cell phones, and battery powered speakers.

  The value of priority1 shall be 255 for a PTP Instance that is not grandmaster-capable. 
  The value of priority1 shall be less than 255 for a PTP Instance that is grandmaster-capable.
*/
#define GPTP_DEFAULT_CLOCK_PRIO1          128
// set to slave only for testing
//#define GPTP_DEFAULT_CLOCK_PRIO1          SLAVE_ONLY
#define GPTP_DEFAULT_CLOCK_CLASS          248
#define GPTP_DEFAULT_CLOCK_ACCURACY       249
/* 
  8.6.2.4
  This value corresponds to the value of PTPDEV for observation interval equal to 
  the default Sync message transmission interval (i.e., observation interval of 0.125)
*/
#define GPTP_DEFAULT_OFFSET_VARIANCE      0x436a
/*
  8.5.2.3 Port number
  The portNumber values for the ports on a time-aware system shall be 
  distinct in the range 1, 2, �, 0xFFFE.
*/
#define GPTP_DEFAULT_PORT1_ID              0x0100
#define GPTP_DEFAULT_PORT2_ID              0x0200
/*
  8.6.2.5 priority2
  The default value of priority2 shall be 248.
*/
#define GPTP_DEFAULT_CLOCK_PRIO2          248
#define GPTP_DEFAULT_STEPS_REMOVED        0

/* Clock types */
#define GPTP_CLOCK_TYPE_INT_OSC           0xA0

/* UTC offset (https://de.wikipedia.org/wiki/Schaltsekunde) */
#define TAIUTC                            0x2500

/* GPTP flags */
#define GPTP_FLAGS_NONE                   0x0000
#define GPTP_FLAGS_PTP_TIMESCALE          0x0008                   
#define GPTP_FLAGS_UNICAST                (1 << 10) 
#define GPTP_FLAGS_TWO_STEP               (1 << 9)

/* GPTP sizes */
#define GPTP_TS_LEN                       10
#define GPTP_PORT_IDEN_LEN                10
#define GPTP_CLOCK_IDEN_LEN               8
#define GPTP_ETHEDR_HDR_LEN               14
#define GPTP_HEADER_LEN                   34
// Next depends on size of time_t
#define GPTP_BODY_OFFSET                  (GPTP_ETHEDR_HDR_LEN + GPTP_HEADER_LEN)     

/* Default timeouts */
#ifdef gPTP
#define GPTP_PDELAY_REQ_INTERVAL          1000
#define GPTP_PDELAY_REQ_TIMEOUT           4000
#else //PTCP
#define GPTP_PDELAY_REQ_INTERVAL          200
#define GPTP_PDELAY_REQ_TIMEOUT           8000
#define GPTP_PDELAY_REPEAT                5
#endif
#define GPTP_ANNOUNCE_INTERVAL            1000
#define GPTP_ANNOUNCE_TIMEOUT             4000
#define GPTP_TAKEOVER_TIMEOUT_FACTOR      2
#define GPTP_SYNC_TIMEOUT_FACTOR          6
#define GPTP_SYNC_INTERVAL                30 // 30ms

//GPTP_SYNC_INTERVAL_LOG is: log(base 2) of (GPTP_SYNC_INTERVAL / 1000)
//#define GPTP_SYNC_INTERVAL_LOG            -3
#define GPTP_SYNC_INTERVAL_LOG            1
#define GPTP_ANNOUNCE_INTERVAL_LOG        1u
#define GPTP_PDELAY_REQ_INTERVAL_LOG      0u
#define GPTP_SYNC_TIMEOUT                 3000  
/* timeout for TX timestamp in mS */
#define TXTS_TIMOUT_MS                    50

/* IEEE 1588 PTP */
#define ETH_P_1588	0x88F7
/* Number of HW ports supported */
#define NUM_PORTS     2
#define PORT_DISABLED 0
#define PORT_ENABLED  1
#define ANY_PORT			0

typedef unsigned long long u64;
typedef signed   long long s64;

typedef signed   int   s32;
typedef signed   short s16;
typedef signed   char  s8;
typedef unsigned int   u32;
typedef unsigned short u16;
typedef unsigned char  u8;

#pragma pack(push, 1)

/* common header for all gPTP frame types */
struct gPTPHdr {
	union tbf {
		struct th {
			union tb1 {
				u8 tsSpec;
				u8 msgType;
			} b1;
			union tb2 {
				u8 res1;
				u8 ptpVer;
			} b2;
			u16 msgLen;
			u8 domNo;
			u8 res2;
			u16 flags;
			u64 corrF;
			u32 res3;
			u8  srcPortIden[GPTP_CLOCK_IDEN_LEN];
			u16 sourceportid;
			u16 seqNo;
			u8 ctrl;
			u8 logMsgInt;
		} f;
		unsigned char byte[GPTP_HEADER_LEN];
	} h;
};

/* transport layer time stamp */
struct gPTPTS {
	struct s48b {
		u16 msb;
		u32 lsb;
	} s;
	u32 ns;
};

/* best master */
struct gPTPPrioVec {
  u8  originTimestamp[10];
	u16 currUTCOff;
	u8  res2;
	u8  prio1;
	
	struct clockQual {
		u8  clockClass;
		u8  clockAccuracy;
		u16 offsetScaledLogVariance;
	}clockQual;
	
	u8  prio2;
	u8  iden[GPTP_CLOCK_IDEN_LEN];
	u16 stepsRem;
	u8  clockSrc;
};

/* TLV gPTP */
struct gPTPscaledNs {
	u8 ns[12];
};

/* TLV gPTP */
struct gPTPtlv {
	u16 type;
	u16 len;
};

/* TLV gPTP */
struct gPTPOrgExt {
	u8     orgType[3];
	u8     orgSubType[3];
	u32    csRateOff;
	u16    gmTBInd;
	struct gPTPscaledNs lastGMPhChg;
	u32    gmFreqChg;
};

#pragma pack(pop)

/* event timer */
struct timer {
	u64 lastTS;
	u32 timeInterval;
	u32 timerEvt;
};

/* delay request frame gPTP/PTCP */
struct dmst {
	int state;
	u16 rxSeqNo;
	u16 txSeqNo;
	u32 delayReqInterval;
	u32 delayReqTimeOut;
	u8  reqPortIden[GPTP_PORT_IDEN_LEN];
};

/* announce frame gPTP/PTCP */
struct bmcst {
	int state;
	u16 annoSeqNo;
	u32 announceInterval;
	u32 announceTimeout;
	struct gPTPPrioVec portPrio;
	struct gPTPPrioVec gmPrio;
};

/* sync frame gPTP/PTCP */
struct csst {
	int state;
	u16 syncSeqNo;
	u32 syncInterval;
	u32 syncTimeout;
	u32 syncTakeoverTimeout;
	u16 padding;
};

/* port state handling (per port) gPTP/PTCP */
struct gPTPd {

	/* prepared send buffer */
	uint8_t  txBufSync[GPTP_TX_BUF_SIZE];
	uint8_t  txBufSyFu[GPTP_TX_BUF_SIZE];
	uint8_t  txBufDeRq[GPTP_TX_BUF_SIZE];
	uint8_t  txBufDeRp[GPTP_TX_BUF_SIZE];
	uint8_t  txBufReFu[GPTP_TX_BUF_SIZE];	
	uint8_t  txBufAnno[GPTP_TX_BUF_SIZE];
	uint8_t* rxBuf;
	
	void * txTsQueue[6];
	
	/* por number of this port instance */
	uint16_t port;
	/* port enabled by TIA-Portal project */
	uint16_t enabled;
	/* are we master at this port 
		 (as device we are never master) */
	uint16_t isMasterPort;
	/* SyncID from PRM data */
	uint16_t syncID;
	/* sync frame is one/two step */
	uint16_t twoStep;
	/* sync/no sync reported to CM */
	uint16_t SyncState;
	/* error counter foe BSP API eth write 
     most likely there is no descriptor left */
	uint32_t sendError;
	
	/* static phy delays reported by neighbor (PLC) */
	uint32_t remoteRXDelay;
	uint32_t remoteTXDelay;
	
	/* MAC address of master we are syncing to */
	MAC_ADR_TYPE masterMAC;
	
	/* eth cable un-pluged? */
	uint16_t portDown;
	
	/* delay req. burst counter */
	uint16_t delayRepeat;
	
	/* response timout timer */
	uint16_t respTimeout;

	/* GSY interfacing */
	GSY_CH_USR_PTR pChUsr;
	
	/* physical port delays */
	uint32_t RealPortTxDelay;
	uint32_t RealPortRxDelay;
	uint32_t MaxPortTxDelay;
	uint32_t MaxPortRxDelay;
	
	/* claculated delays */
	//uint32_t average_val[7];
	//uint32_t average_cnt;
	int32_t  cableDelay;
	uint32_t portDelay;
	uint32_t RemotePortTimeT2_del;
    uint32_t LocalPortTimeT1_del;
  uint32_t DelayDriftAdjustDone; 
  LSA_UINT64 rcfPeerx1M; 
  
	
	/* send/receive queue data types */
	EDD_RQB_NRT_SEND_TYPE RSyncSendTime[6];
	EDD_RQB_NRT_RECV_TYPE RSyncReceive;
	EDD_RQB_NRT_RECV_TYPE * RSyncReceiveAddr;
  
	/* timer for periodic frame sending */
	struct timer timers[GPTP_NUM_TIMERS];
	/* collected timestamps (ts)*/
	struct timespec ts[13];
  
  uint32_t max_sync_diff_adj;
  uint32_t max_sync_drift_adj;
  int32_t act_sync_drift_adj; 
	
	/* state machine states for this port instance */
	struct dmst  dm;
	struct bmcst bmc;
	struct csst  cs;
};

/* get time ticks */
u64  gptp_getCurrMilliSecTS(void);

/* frame resource management */
void gptp_initTxBuf(struct gPTPd* gPTPd);
void gptp_initRxBuf(struct gPTPd* gPTPd);

/* base timer handling */
void gptp_startTimer(struct gPTPd* gPTPd, u32 timerId, u16 timeInterval, u32 timerEvt);
void gptp_stopTimer (struct gPTPd* gPTPd, u32 timerId);
void gptp_resetTimer(struct gPTPd* gPTPd, u32 timerId);
void eventLoop      (struct gPTPd* gPTPd);

/* event related */
void gptp_handleEvent(int evt, uint16_t port);
int  gptp_parseMsg   (uint16_t port);
void bmcSetAllPortsToSlave(void);

/* timer utility functions */
int  gptp_timespec_absdiff(struct timespec *start, struct timespec *stop, struct timespec *result);
void gptp_timespec_diff   (struct timespec *start, struct timespec *stop, struct timespec *result);
void gptp_timespec_sum    (struct timespec *start, struct timespec *stop, struct timespec *result);

/* getting, setting timestamp from/to frame */
void gptp_copyTSFromBuf(struct timespec *ts, u8 *src);
void gptp_copyTSToBuf  (struct timespec *ts, u8 *dest);

/* send/receive timstamp capture */
uint8_t getTxTS(struct gPTPd* gPTPd, struct timespec* ts, uint16_t idx);
void    getRxTS(struct gPTPd* gPTPd, struct timespec* ts);

/* task related */
static void syncFollowUpTask(void);

void startPTP  (GSY_CH_USR_PTR	pChUsr);
void gptp_init (uint16_t port);
void gptp_setup(void);
void gptp_start(void);
void gptp_clearMasterMAC(struct gPTPd * gPTPd);

/* PNIO interface related */
void setLLDPMasterMAC(int16_t diagIndId, struct gPTPd* gPTPd, uint8_t hasSubDomain);

#endif

