
#ifndef EDDP_K32_CFG_H                        /* ----- reinclude-protection ----- */
#define EDDP_K32_CFG_H

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
/*  F i l e               &F: eddp_k32_cfg.h                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Configuration for GSY KRISC32 access                                     */
/*  Defines constants, types and macros...                                   */
/*                                                                           */
/*****************************************************************************/
#ifdef EDDP_MESSAGE /*********************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Version                  Who  What                           */
/*  2008-11-13  P04.01.00.00_00.02.02.02 lrg  initial version                */
/*  2008-12-04  P04.01.00.00_00.02.03.01 lrg: Package Version (PNIO)         */
/*  2009-01-07  P04.01.00.00_00.02.03.02 lrg: gsy_ -> k32gsy_                */
/*  2009-01-09  P04.01.00.00_00.02.03.03 lrg: K32 slave (msm)                */
/*  2009-03-18  P04.01.00.00_00.03.03.02 180309lrg001: K32_OPC_GSY_SYNC_IND  */
/*  2009-03-31  P04.01.00.00_00.03.03.03 lrg: K32GSY_FWD_TX/RX_SET_CLOCK/TIME*/
/*  2009-04-24  P04.01.00.00_00.03.04.02 lrg: k32_cfg.h -> eddp_k32_cfg.h    */
/*  2009-09-24  P04.02.00.00_00.01.07.01 lrg: K32_MASTER_IN_FW               */
/*  2009-11-19  P05.00.00.00_00.01.01.02 lrg: K32_SYNC_UPDATE                */
/*  2009-12-02  P05.00.00.00_00.01.01.03 lrg: K32_TXRESULT_ALIGN             */
/*  2010-20-10  P05.00.00.00_00.01.10.01 ma:  fixed K32_INTERFACE_VALID      */
/*  2011-02-16  P05.00.00.00_00.04.07.02 lrg: AP01123956 Index-Trace         */
/*  2011-07-25  P05.00.00.00_00.05.38.02 lrg: AP01215492 K32_RSP_OK_ACTIVE   */
/*                                            K32_OPC_GSY_DELAY_CNF          */
/*  2013-01-08  P05.02.00.00_00.04.12.02 lrg: AP01482568:EDDP/KRISC32 RRQB   */
/*              Queue Handling: set K32_OPC_TO_DO_BIT to get/ack RRQB        */
/*  2016-03-02  P06.01.00.00_00.         hm:  K32_PARAMS_TYPE_SYNC_IND change*/
/*                                                                           */
/*****************************************************************************/
#endif /* EDDP_MESSAGE */


/* ------------------------------------------------------------------------------------------------------------ */
/* PACKED(4) STRUCTS - ON                                                                                       */
/* Rules for packing of structs see <eddp_hal_pnip_struct.h>                                                    */
/* IMPORTANT: All struct elements in this header must lie/packed naturally aligned (4 bytes)                    */
/* ------------------------------------------------------------------------------------------------------------ */
#ifdef K32_KRISC32OS
    #include "pnio_k32_pck4_on.h"
#else
    #include "pnio_pck4_on.h"
#endif

// #define K32_MASTER_IN_FW		// Compile option for GSY master code in KRISC

/*****************************************************************************/
/* Attribute of modul internal functions
*/
#define K32_STATIC				static

/* Module IDs
*/
#define K32_MODULE_ID_GSY_DEL	10
#define K32_MODULE_ID_GSY_MSM	11


/*===========================================================================*/
/*                         basic constants of K32_BOOL                       */
/*===========================================================================*/
#define K32_BOOL        LSA_UINT8       /* !!! IMPORTANT: LSA_UINT8 may not be changed */

#define K32_FALSE       ((K32_BOOL) 0)  /* for variables of type KRISC32::LSA_BOOL and K32_BOOL must be 0! */
#define K32_TRUE        (!K32_FALSE)    /* for variables of type KRISC32::LSA_BOOL and K32_BOOL must be (!FALSE) */


/*===========================================================================*/
/*                              basic attributes                             */
/*===========================================================================*/

#define K32_ATTR_NEAR
#define K32_ATTR_FAR
#define K32_ATTR_HUGE
#define K32_ATTR_SHUGE
#define K32_MEM_ATTR			K32_ATTR_HUGE

/*===========================================================================*/
/*                         code- and data-attributes                         */
/*===========================================================================*/

/*=====  code attributes  =====
*/
#define K32_SYS_IN_FCT_ATTR		K32_ATTR_FAR         /* system to GSY */
#define K32_SYS_OUT_FCT_ATTR	K32_ATTR_FAR         /* GSY to system */
#define K32_LOCAL_FCT_ATTR		K32_ATTR_NEAR        /* local GSY function */

#ifdef GSY_INCLUDE_FOR_KRISC32

/*=====  data attributes  ===== in GSY header files
*/
#define GSY_UPPER_RQB_ATTR       K32_ATTR_HUGE        /* upper-rqb data */
#define GSY_UPPER_MEM_ATTR       K32_ATTR_NEAR        /* upper-memory data */
#define GSY_LOWER_RQB_ATTR       K32_MEM_ATTR         /* lower-rqb data */
#define GSY_LOWER_MEM_ATTR       K32_MEM_ATTR         /* lower-memory data */
#define GSY_LOWER_TXMEM_ATTR     K32_MEM_ATTR         /* lower-memory data */
#define GSY_LOWER_RXMEM_ATTR     K32_MEM_ATTR         /* lower-memory data */
#define GSY_SYSTEM_MEM_ATTR      K32_MEM_ATTR         /* system-memory data */
#define GSY_LOCAL_MEM_ATTR       K32_MEM_ATTR         /* local data */
#define GSY_COMMON_MEM_ATTR      K32_ATTR_SHUGE       /* greatest common */
                                                      /* data attribute  */
#endif // GSY_INCLUDE_FOR_KRISC32

/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/

#define K32_INTERFACE_ID_A						0
#define K32_INTERFACE_ID_B						1
#define K32_INTERFACE_FIRST						(K32_INTERFACE_ID_A)
#define K32_INTERFACE_LAST						(K32_INTERFACE_ID_A)
#define K32_INTERFACE_COUNT						((K32_INTERFACE_LAST) - (K32_INTERFACE_FIRST) +1)
#define K32_INTERFACE_VALID(interfaceID)		((K32_INTERFACE_FIRST <= interfaceID)&&(K32_INTERFACE_LAST >= interfaceID))

#define K32_RSP_OK				1       // LSA_RSP_OK
#define K32_RSP_OK_ACTIVE       0x41    // (LSA_RSP_OK_OFFSET+1)

#ifndef K32GSY_RXTXNANOS		// defined in GSY module test
#define K32GSY_RXTXNANOS		1
#endif

#define K32GSY_FRAME_PTR \
	LSA_UINT8					K32_MEM_ATTR *

#define K32GSY_TXMEM_ZERO(_p, _l) \
	{LSA_INT _i; for (_i = 0; _i < (_l); _i++) *(((K32GSY_FRAME_PTR)_p)+_i) = 0;}

/* Use types for PN-IP send/receive
*/
#define K32_USETYPE_CLOCK		1
#define K32_USETYPE_TIME		2
#define K32_USETYPE_DELAY		3
#define K32_USETYPE_CLOCKFU		4
#define K32_USETYPE_TIMEFU		5

/* Tx results from PN-IP send
*/
#define K32_TXRESULT_OK			0
#define K32_RXRESULT_NEXT		1
#define K32_TXRESULT_PORT		2
#define K32_TXRESULT_RSP		3
#define K32_TXRESULT_TIME		4
#define K32_TXRESULT_LEN		5
#define K32_TXRESULT_ALIGN		6
#define K32_TXRESULT_AHB		7

/* Rx results from PN-IP receive
*/
#define K32_RXRESULT_OK			0
#define K32_RXRESULT_NEXT		1
#define K32_RXRESULT_LEN		2
#define K32_RXRESULT_ALIGN		3

/* Masks for PN-IP OCF/RCF compensation
*/
#define K32_COMP_INTERVAL		0x3FFFFFFF
#define K32_COMP_PLUS			0x40000000
#define K32_COMP_VALID			0x80000000

#define K32_INITIAL_TOPO_STATE	LSA_FALSE

/******************************************************************************
 * Opcodes and Structures of the KRISC32 RQB
 * AP01482568:EDDP/KRISC32 RRQB Queue Handling: set K32_OPC_TO_DO_BIT to get/ack RRQB
*/
#define K32_OPC_TO_DO_BIT		0x80
#define K32_OPC_LOG				0x7F
#define K32_OPC_NO_OPERATION	0x42
#define K32_OPC_SETUP			1
#define K32_OPC_START			2
#define K32_OPC_STOP			3
#define K32_OPC_GSY_SYNC		4
#define K32_OPC_GSY_DIAG		5
#define K32_OPC_GSY_DELAY_REQ	6
#define K32_OPC_GSY_DELAY_IND	7
#define K32_OPC_GSY_TOPO_STATE	8
#define K32_OPC_GSY_SYNC_IND	9
#define K32_OPC_GSY_DELAY_CNF	10

/* KRISC32 setup control parameter structure
*/
/* !!! IMPORTANT: The structure elements must lie/packed naturally aligned (4 bytes) !!! */
PNIO_PACKED_ATTRIBUTE_PRE struct _K32_PARAMS_TYPE_SETUP
{
	LSA_UINT16			PP0_PortID;			// logical port ID of pysical port 0 (zero == not used)
	LSA_UINT16			PP1_PortID;			// logical port ID of pysical port 1 (zero == not used)
	LSA_UINT16			PP2_PortID;			// logical port ID of pysical port 2 (zero == not used)
	LSA_UINT16			PP3_PortID;			// logical port ID of pysical port 3 (zero == not used)
	LSA_UINT32			TraceIdx;			// AP01123956 Index-Trace
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _K32_PARAMS_TYPE_SETUP   K32_PARAMS_TYPE_SETUP;


/* !!! IMPORTANT: The structure elements must lie/packed naturally aligned (4 bytes) !!! */
PNIO_PACKED_ATTRIBUTE_PRE struct _K32_TRACE_BUFFER
{
    LSA_UINT32      sequenceID;
    LSA_UINT32      elements;
    LSA_UINT32      data[0x185];	/* 12+1+2=15 elements minimum!
								    * (trace with 12 elements + 1 for hash + 2 for timestamp )
								    * 1.) We use 240 (0xF0) elements (no frame drops in normal operation, no array dump).
								    * 2.) We use 389 (0x185) elements (1556 byte).
								    * (384 for max Ethernet Frame + 2 for PNIP-timestamp  + 1 for hash + 2 for timestamp)
								    */
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _K32_TRACE_BUFFER    K32_TRACE_BUFFER;

/* KRISC32 logging parameter structure
*/
/* !!! IMPORTANT: The structure elements must lie/packed naturally aligned (4 bytes) !!! */
PNIO_PACKED_ATTRIBUTE_PRE struct _K32_PARAMS_TYPE_LOGGING
{
	LSA_UINT32			logBufferOffset;
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _K32_PARAMS_TYPE_LOGGING     K32_PARAMS_TYPE_LOGGING;

/* Sync control parameter structure of the KRISC32 RQB
*/
/* !!! IMPORTANT: The structure elements must lie/packed naturally aligned (4 bytes) !!! */
PNIO_PACKED_ATTRIBUTE_PRE struct _K32_PARAMS_TYPE_GSY_SYNC
{
	LSA_UINT8	SyncId;			// 0 = clock, 1 = time
	LSA_UINT8	SlaveState;		// START, OFF
	LSA_UINT8	MasterState;	// START, ACTIVE, SLEEP, OFF
	LSA_UINT8	LengthTLV;		// Length of TLV chain in SyncFrame (24 or 52)
	LSA_UINT32	SyncSendFactor;	// Master send interval in milliseconds
	LSA_UINT16	TimeoutFactor;	// Slave timeout als  as multiple of master send interval
	LSA_UINT16	TakeoverFactor;	// Master takeover timeout as multiple of master send interval
	LSA_UINT32  PLLWindow;		// max. offset of slave time from master in nanoseconds
	LSA_UINT8 	TLV[52];		// SyncFrame TLVs: Subdomain [,Time,TimeExt, Master]
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _K32_PARAMS_TYPE_GSY_SYNC    K32_PARAMS_TYPE_GSY_SYNC;

/* !!! IMPORTANT: The structure elements must lie/packed naturally aligned (4 bytes) !!! */
PNIO_PACKED_ATTRIBUTE_PRE struct _K32_PARAMS_TYPE_GSY_SLAVE
{
	LSA_UINT8	SyncId;			// 0 = clock, 1 = time
	LSA_UINT8	SlaveState;		// START, OFF
	LSA_UINT8	MasterState;	// START, ACTIVE, SLEEP, OFF
	LSA_UINT8	LengthTLV;		// Length of TLV chain in SyncFrame (24)
	LSA_UINT32	SyncSendFactor;	// Master send interval as multiple of 10ms
	LSA_UINT16	TimeoutFactor;	// Slave timeout als  as multiple of master send interval
	LSA_UINT16	TakeoverFactor;	// Master takeover timeout as multiple of master send interval
	LSA_UINT32	PLLWindow;		// max. offset of slave time from master in nanoseconds
	LSA_UINT8 	TLV[24];		// SyncFrame TLV Subdomain
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _K32_PARAMS_TYPE_GSY_SLAVE   K32_PARAMS_TYPE_GSY_SLAVE;

#define K32_SYNC_OFF			0
#define K32_SYNC_START			1
#define K32_SYNC_UPDATE			2
#define K32_SYNC_ACTIVE			3
#define K32_SYNC_SLEEP			4

/* Diagnosis parameter structure of the KRISC32 RQB
*/
/* !!! IMPORTANT: The structure elements must lie/packed naturally aligned (4 bytes) !!! */
PNIO_PACKED_ATTRIBUTE_PRE struct _K32_PARAMS_TYPE_GSY_DIAG
{
    LSA_UINT8	        MasterMacAddr[EDD_MAC_ADDR_SIZE];   // 2: Master interface MAC address (MacAddrSize = 6 byte)
	LSA_UINT8			SyncId;			                    // 0 = clock, 1 = time
	LSA_UINT8			DiagSrc;		                    // 1...4
	LSA_INT32			RateInterval;	                    // 4: Rate to the master clock
	LSA_UINT16			SequenceId;		                    // of the last received SyncFrame
	LSA_UINT8			SlaveState;		                    // 1: Sync state of the slave in FW
    LSA_UINT8			RcvSyncPrio;	                    // 3: Master Priority1

} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _K32_PARAMS_TYPE_GSY_DIAG    K32_PARAMS_TYPE_GSY_DIAG;

/* Delay control parameter structure of the KRISC32 RQB
*/
/* !!! IMPORTANT: The structure elements must lie/packed naturally aligned (4 bytes) !!! */
PNIO_PACKED_ATTRIBUTE_PRE struct _K32_PARAMS_TYPE_GSY_DELAY_REQ
{
    LSA_UINT8			ReqActive;		                // 1: Delay requestor active, 0: inaktive
	LSA_UINT8			RspActive;		                // 1: Delay responder active, 0: inaktive
    LSA_UINT8           dummy1[2];                      // for naturally alignment (32bit)
	LSA_UINT8           PortMacAddr[EDD_MAC_ADDR_SIZE]; // Port MAC address of requestor (MacAddrSize = 6 byte)
	LSA_UINT16			PortID;			                // Port number to control (1, 2)
	LSA_UINT32			RxPortDelay;	                // Local receive delay in nanoseconds
	LSA_UINT32			TxPortDelay;	                // Local send delay in nanoseconds

} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _K32_PARAMS_TYPE_GSY_DELAY_REQ   K32_PARAMS_TYPE_GSY_DELAY_REQ;

#define K32_DELAY_INACTIVE		0
#define K32_DELAY_ACTIVE		1

/* Delay indication parameter structure of the KRISC32 RQB
*/
/* !!! IMPORTANT: The structure elements must lie/packed naturally aligned (4 bytes) !!! */
PNIO_PACKED_ATTRIBUTE_PRE struct _K32_PARAMS_TYPE_GSY_DELAY_IND
{
	LSA_UINT32	LineDelay;		// Average in nanoseconds (=0 at stop oder cancel by error)
	LSA_UINT32	CableDelay;		// Average in nanoseconds
	LSA_UINT16	PortID;			// Port number (1, 2)
    LSA_UINT8   dummy1[2];      // for naturally alignment (32bit)
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _K32_PARAMS_TYPE_GSY_DELAY_IND   K32_PARAMS_TYPE_GSY_DELAY_IND;

/* Implicite FWD boundary control parameter structure of the KRISC32 RQB
*/
/* !!! IMPORTANT: The structure elements must lie/packed naturally aligned (4 bytes) !!! */
PNIO_PACKED_ATTRIBUTE_PRE struct _K32_PARAMS_TYPE_GSY_TOPO_STATE
{
	LSA_UINT16			PortID;			// Port number to control (1, 2)
	K32_BOOL            TopoStateOk;	// Neighbourhood state OK or not
    LSA_UINT8           dummy1[1];      // for naturally alignment (32bit)

} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _K32_PARAMS_TYPE_GSY_TOPO_STATE  K32_PARAMS_TYPE_GSY_TOPO_STATE;

/* Indication of synchronisation
*/
/* !!! IMPORTANT: The structure elements must lie/packed naturally aligned (4 bytes) !!! */
PNIO_PACKED_ATTRIBUTE_PRE struct _K32_PARAMS_TYPE_SYNC_IND
{
	LSA_UINT8			SyncId;			// 0 = clock, 1 = time
	K32_BOOL            SyncOk;			// K32_TRUE if synchronized
	LSA_UINT16			SyncState;		// current state of time synchronisation (FT1204141)
	LSA_UINT32			OctLowTimeOld;	// old time before change
	LSA_UINT32			OctHighTimeOld;	// old time before change
	LSA_UINT32			OctLowTimeNew;	// new time after change
	LSA_UINT32			OctHighTimeNew;	// new time after change
	LSA_UINT32			SequenceID;	    // ID of a specific SyncState
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _K32_PARAMS_TYPE_SYNC_IND    K32_PARAMS_TYPE_SYNC_IND;

// Bit values for SyncState:
#define EDDP_K32_SYNC_STATE_OK						0
#define EDDP_K32_SYNC_STATE_JITTER_OUT_OF_BOUNDARY	1
#define EDDP_K32_SYNC_STATE_NO_MESSAGE_RECEIVED		2
#define EDDP_K32_SYNC_STATE_MASK					3
#define EDDP_K32_SYNC_STATE_OFF					0xffff

/* !!! IMPORTANT: The structure elements must lie/packed naturally aligned (4 bytes) !!! */
PNIO_PACKED_ATTRIBUTE_PRE union _K32_RQB_TYPE_PARAMS
{
	K32_PARAMS_TYPE_SETUP			Setup;
	K32_PARAMS_TYPE_LOGGING			Logging;
	K32_PARAMS_TYPE_GSY_SYNC		SyncReq;
	K32_PARAMS_TYPE_SYNC_IND		SyncInd;
	K32_PARAMS_TYPE_GSY_SLAVE		SlaveReq;
	K32_PARAMS_TYPE_GSY_DIAG		DiagInd;
	K32_PARAMS_TYPE_GSY_DELAY_REQ	DelayReq;
	K32_PARAMS_TYPE_GSY_DELAY_IND	DelayInd;
	K32_PARAMS_TYPE_GSY_TOPO_STATE	TopoState;

} PNIO_PACKED_ATTRIBUTE_POST;
typedef union _K32_RQB_TYPE_PARAMS  K32_RQB_TYPE_PARAMS;

/* !!! IMPORTANT: The structure elements must lie/packed naturally aligned (4 bytes) !!! */
PNIO_PACKED_ATTRIBUTE_PRE struct _K32_RQB_TYPE_HEADER
{
	LSA_UINT8					InterfaceID;
	LSA_UINT8					Opcode;
	LSA_UINT16					Response;
	LSA_USER_ID_TYPE			UserId;

} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _K32_RQB_TYPE_HEADER     K32_RQB_TYPE_HEADER;

/* !!! IMPORTANT: The structure elements must lie/packed naturally aligned (4 bytes) !!! */
PNIO_PACKED_ATTRIBUTE_PRE struct _K32_RQB_TYPE_DELAY_REQ
{
	K32_RQB_TYPE_HEADER				Hdr;
	K32_PARAMS_TYPE_GSY_DELAY_REQ	DelayReq;

} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _K32_RQB_TYPE_DELAY_REQ  K32_RQB_TYPE_DELAY_REQ;

/* !!! IMPORTANT: The structure elements must lie/packed naturally aligned (4 bytes) !!! */
PNIO_PACKED_ATTRIBUTE_PRE struct _K32_RQB_TYPE_SLAVE_REQ
{
	K32_RQB_TYPE_HEADER				Hdr;
	K32_PARAMS_TYPE_GSY_SLAVE		SlaveReq;

} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _K32_RQB_TYPE_SLAVE_REQ  K32_RQB_TYPE_SLAVE_REQ;

/* !!! IMPORTANT: The structure elements must lie/packed naturally aligned (4 bytes) !!! */
PNIO_PACKED_ATTRIBUTE_PRE struct _K32_RQB_TYPE_DIAG_REQ
{
	K32_RQB_TYPE_HEADER				Hdr;
	K32_PARAMS_TYPE_GSY_DIAG		DiagInd;

} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _K32_RQB_TYPE_DIAG_REQ   K32_RQB_TYPE_DIAG_REQ;

/* !!! IMPORTANT: The structure elements must lie/packed naturally aligned (4 bytes) !!! */
PNIO_PACKED_ATTRIBUTE_PRE struct _K32_RQB_TYPE
{
	K32_RQB_TYPE_HEADER	Hdr;
	K32_RQB_TYPE_PARAMS	Params;
} PNIO_PACKED_ATTRIBUTE_POST;
typedef struct _K32_RQB_TYPE    K32_RQB_TYPE;

#define K32_UPPER_RQB_ATTR		EDD_ATTR_HUGE

typedef K32_RQB_TYPE	EDD_UPPER_MEM_ATTR *    EDD_UPPER_KRISC32_RQB_PTR_TYPE;
typedef K32_RQB_TYPE	K32_UPPER_RQB_ATTR *    K32_RQB_PTR_TYPE;

/*****************************************************************************/
/* Macros                                                                    */
/*****************************************************************************/

/** @note This function is only for temporary variables!
 */
#define K32_CIRCULAR_INC(index, elements)\
	if( ((index)+1) >= (elements) )\
	{\
	    (index) = 0;\
	}else\
	{\
		++(index);\
	}

/** @note this is faster and smaller code on ARM9 than modulo.
 * @note elements must be greater than zero.
 * @note This function is only for temporary variables!
 */
#define K32_CIRCULAR_ADJUST(index, elements) \
	while(0 > (index))\
	{\
		(index) += (elements);\
	}\
	while((index) >= (elements))\
	{\
		(index) -= (elements);\
	}

/** This is the common adjust function (modulo is slower on ARM9!).
 * @note elements must be greater than zero.
 * @note This function is only for temporary variables,
 *       for atomic operations directly on circular list index elements,
 *       use K32_CIRCULAR_INC !
 */
/*
#define K32_CIRCULAR_ADJUST(index, elements) \
	if((index) < 0)\
	{\
		(index) = elements - ((-(index)) % (elements));\
	}\
	else\
	if((index) >= (elements))\
	{\
		(index) %= (elements);\
	}
*/


#define NUMBER_OF_ELEMENTS(_array_) (sizeof(_array_) / sizeof(_array_[0]))

/** LiveTrace structure.
 */
/* !!! IMPORTANT: The structure elements must lie/packed naturally aligned (4 bytes) !!! */
PNIO_PACKED_ATTRIBUTE_PRE struct _K32_LIVE_TRACE_STRUCT
{
	LSA_UINT32		liveTraceMask;              /* 0000 */ 
	LSA_UINT32		lockedTraceBufferOffset;    /* 0004 */
	LSA_UINT32		firstTraceBufferOffset;     /* 0008 */
	LSA_UINT32		secondTraceBufferOffset;    /* 000C */
	LSA_UINT32		thirdTraceBufferOffset;     /* 0010 */
	LSA_UINT32 		fatalModuleID;              /* 0014 */
	LSA_UINT32  	fatalLineNumber;            /* 0018 */
	LSA_UINT32		K32RRQBTrace;               /* 001C */
	LSA_UINT32  	const K32Version[2];        /* 0020..0024 */
	LSA_UINT32      const K32VersionHASH[2];    /* 0028..002C */
	LSA_UINT32      const startUpConfig[16];    /* 0030..003C */
} PNIO_PACKED_ATTRIBUTE_POST;
typedef volatile struct _K32_LIVE_TRACE_STRUCT  _K32_LIVE_TRACE;

/** LiveTrace structure instance.
 */
extern _K32_LIVE_TRACE K32_LIVE_TRACE;

/** extra post-mortem structure.
 * @see startup.S
 */
/* !!! IMPORTANT: The structure elements must lie/packed naturally aligned (4 bytes) !!! */
PNIO_PACKED_ATTRIBUTE_PRE struct _K32_POST_MORTEM_STRUCT
{
	LSA_UINT32			state;		/* post mortem processor state */
	struct PNIO_PACKED_ATTRIBUTE_PRE
    {
		LSA_UINT32		r[16];		/* processor registers r0..r15 */
		LSA_UINT32		cpsr;		/* processor state */
	} PNIO_PACKED_ATTRIBUTE_POST system;
	/**@note	we do not need all banked registers since there
	 *			is only one transition from user-mode to an exception
 	 *			(no use of irq/fiq/svc modes in K32 firmware).
 	 */
	//struct {
	//	LSA_UINT32		r[7];		/* processor registers r8..r14 */
	//	LSA_UINT32		spsr;		/* processor state */
	//} fiq;
	//struct {
	//	LSA_UINT32		r[2];		/* processor registers r13..r14 */
	//	LSA_UINT32		spsr;		/* processor state */
	//} irq;
	//struct {
	//	LSA_UINT32		r[2];		/* processor registers r13..r14 */
	//	LSA_UINT32		spsr;		/* processor state */
	//} svc;
	//struct {
	//	LSA_UINT32		r[2];		/* processor registers r13..r14 */
	//	LSA_UINT32		spsr;		/* processor state */
	//} undef;
	//struct {
	//	LSA_UINT32		r[2];		/* processor registers r13..r14 */
	//	LSA_UINT32		spsr;		/* processor state */
	//} abt;
} PNIO_PACKED_ATTRIBUTE_POST;
typedef volatile struct _K32_POST_MORTEM_STRUCT _K32_POST_MORTEM;

/** extra post-mortem structure instance
 */
extern _K32_POST_MORTEM K32_POST_MORTEM;


/*****************************************************************************/
/* Prototypes                                                                */
/*****************************************************************************/

/* k32_gsy_del.c
*/
LSA_UINT16	K32_SYS_IN_FCT_ATTR k32gsy_Init (LSA_VOID);
LSA_UINT16	K32_SYS_IN_FCT_ATTR k32gsy_UndoInit (LSA_VOID);
LSA_VOID	K32_SYS_IN_FCT_ATTR k32gsy_Timeout (LSA_UINT16, LSA_USER_ID_TYPE);
LSA_VOID	K32_SYS_IN_FCT_ATTR k32gsy_DelayReq (K32_RQB_PTR_TYPE);
LSA_VOID	K32_SYS_IN_FCT_ATTR k32gsy_DelayInd (K32GSY_FRAME_PTR,LSA_UINT16,LSA_UINT8,LSA_UINT16,LSA_UINT8,LSA_UINT8,LSA_UINT32);
LSA_VOID	K32_SYS_IN_FCT_ATTR k32gsy_SendConf (K32GSY_FRAME_PTR,LSA_UINT8,LSA_UINT16,LSA_UINT8,LSA_BOOL,LSA_UINT32);

/* k32_gsy_msm.c
*/
LSA_VOID	K32_SYS_IN_FCT_ATTR k32gsy_SyncReq (K32_RQB_PTR_TYPE);
LSA_VOID 	K32_SYS_IN_FCT_ATTR k32gsy_TopoStateSet (K32_RQB_PTR_TYPE);
LSA_VOID	K32_SYS_IN_FCT_ATTR k32gsy_SyncInd (K32GSY_FRAME_PTR,LSA_UINT16,LSA_UINT8,LSA_UINT16,LSA_UINT8,LSA_UINT8,LSA_UINT32);
LSA_VOID	K32_SYS_IN_FCT_ATTR k32gsy_SyncFuInd (K32GSY_FRAME_PTR,LSA_UINT16,LSA_UINT8,LSA_UINT16,LSA_UINT8,LSA_UINT8,LSA_UINT32);
LSA_VOID	K32_SYS_IN_FCT_ATTR k32gsy_ChangeCbfClockA(LSA_VOID);
LSA_VOID	K32_SYS_IN_FCT_ATTR k32gsy_ChangeCbfClockB(LSA_VOID);
LSA_VOID	K32_SYS_IN_FCT_ATTR k32gsy_ChangeCbfTime(LSA_VOID);

/* Krisk32 OS
*/
#ifndef K32GSY_CALLBACK
LSA_VOID			K32_SYS_OUT_FCT_ATTR K32GSY_CALLBACK (K32_RQB_PTR_TYPE);
#endif
#ifndef	 K32GSY_TIME_GET
LSA_VOID 			K32_SYS_OUT_FCT_ATTR K32GSY_TIME_GET(LSA_UINT32*,LSA_UINT32*);
#endif
#ifndef K32GSY_RCF_SET_CLOCK_A
LSA_VOID			K32_SYS_OUT_FCT_ATTR K32GSY_RCF_SET_CLOCK_A (LSA_UINT32);
#endif
#ifndef K32GSY_RCF_SET_CLOCK_B
LSA_VOID			K32_SYS_OUT_FCT_ATTR K32GSY_RCF_SET_CLOCK_B (LSA_UINT32);
#endif
#ifndef K32GSY_RCF_SET_TIME
LSA_VOID			K32_SYS_OUT_FCT_ATTR K32GSY_RCF_SET_TIME (LSA_UINT32);
#endif
#ifndef K32GSY_RCF_SET_DELAY
LSA_VOID			K32_SYS_OUT_FCT_ATTR K32GSY_RCF_SET_DELAY (LSA_UINT32);
#endif
#ifndef K32GSY_OCF_SET_CLOCK_A
LSA_VOID			K32_SYS_OUT_FCT_ATTR K32GSY_OCF_SET_CLOCK_A (LSA_UINT32);
#endif
#ifndef K32GSY_OCF_SET_CLOCK_B
LSA_VOID			K32_SYS_OUT_FCT_ATTR K32GSY_OCF_SET_CLOCK_B (LSA_UINT32);
#endif
#ifndef K32GSY_OCF_SET_TIME
LSA_VOID			K32_SYS_OUT_FCT_ATTR K32GSY_OCF_SET_TIME (LSA_UINT32);
#endif
#ifndef K32GSY_UPDATE_CLOCK_A
LSA_VOID			K32_SYS_OUT_FCT_ATTR K32GSY_UPDATE_CLOCK_A (LSA_INT64,
					LSA_VOID LSA_FCT_PTR(K32_SYS_IN_FCT_ATTR,Cbf)(LSA_VOID));
#endif
#ifndef K32GSY_UPDATE_CLOCK_B
LSA_VOID			K32_SYS_OUT_FCT_ATTR K32GSY_UPDATE_CLOCK_B (LSA_INT64,
					LSA_VOID LSA_FCT_PTR(K32_SYS_IN_FCT_ATTR,Cbf)(LSA_VOID));
#endif
#ifndef K32GSY_UPDATE_TIME
LSA_VOID			K32_SYS_OUT_FCT_ATTR K32GSY_UPDATE_TIME (LSA_INT64,
					LSA_VOID LSA_FCT_PTR(K32_SYS_IN_FCT_ATTR,Cbf)(LSA_VOID));
#endif
#ifndef K32GSY_DELAY_SET
LSA_VOID			K32_SYS_OUT_FCT_ATTR K32GSY_DELAY_SET (LSA_UINT8,LSA_UINT16,LSA_UINT32);
#endif
#ifndef K32GSY_SEND_FRAME
LSA_VOID			K32_SYS_OUT_FCT_ATTR K32GSY_SEND_FRAME (K32GSY_FRAME_PTR,LSA_UINT16,LSA_UINT8,LSA_UINT8,LSA_UINT16,LSA_BOOL);
#endif
#ifndef K32GSY_RECV_FRAME
LSA_VOID			K32_SYS_OUT_FCT_ATTR K32GSY_RECV_FRAME (K32GSY_FRAME_PTR,LSA_UINT16,LSA_UINT8,LSA_UINT8);
#endif
#ifndef K32GSY_LINK_GET
LSA_BOOL			K32_SYS_OUT_FCT_ATTR K32GSY_LINK_GET (LSA_UINT8,LSA_UINT16);
#endif
#ifndef K32GSY_PORT_COUNT_GET
LSA_UINT16			K32_SYS_OUT_FCT_ATTR K32GSY_PORT_COUNT_GET (LSA_UINT8);
#endif
#ifndef K32GSY_TRACE_IDX_GET
LSA_UINT32			K32_SYS_OUT_FCT_ATTR K32GSY_TRACE_IDX_GET (LSA_UINT8);
#endif
#ifndef K32GSY_TIMER_ALLOC_DELAY
LSA_UINT16			K32_SYS_OUT_FCT_ATTR K32GSY_TIMER_ALLOC_DELAY (LSA_VOID);
#endif
#ifndef K32GSY_TIMER_ALLOC_SYNC
LSA_UINT16			K32_SYS_OUT_FCT_ATTR K32GSY_TIMER_ALLOC_SYNC (LSA_VOID);
#endif
#ifndef K32GSY_TIMER_START
LSA_UINT16			K32_SYS_OUT_FCT_ATTR K32GSY_TIMER_START (LSA_UINT16,LSA_USER_ID_TYPE,LSA_UINT32);
#endif
#ifndef K32GSY_TIMER_STOP
LSA_UINT16			K32_SYS_OUT_FCT_ATTR K32GSY_TIMER_STOP (LSA_UINT16);
#endif
#ifndef K32GSY_TIMER_FREE
LSA_UINT16			K32_SYS_OUT_FCT_ATTR K32GSY_TIMER_FREE (LSA_UINT16);
#endif
#ifndef K32GSY_WD_START_CLOCK_A
LSA_VOID			K32_SYS_OUT_FCT_ATTR K32GSY_WD_START_CLOCK_A (LSA_UINT32,
					LSA_VOID LSA_FCT_PTR(K32_SYS_IN_FCT_ATTR,Cbf)(LSA_VOID));
#endif
#ifndef K32GSY_WD_START_CLOCK_B
LSA_VOID			K32_SYS_OUT_FCT_ATTR K32GSY_WD_START_CLOCK_B (LSA_UINT32,
					LSA_VOID LSA_FCT_PTR(K32_SYS_IN_FCT_ATTR,Cbf)(LSA_VOID));
#endif
#ifndef K32GSY_WD_START_TIME
LSA_VOID			K32_SYS_OUT_FCT_ATTR K32GSY_WD_START_TIME (LSA_UINT32,
					LSA_VOID LSA_FCT_PTR(K32_SYS_IN_FCT_ATTR,Cbf)(LSA_VOID));
#endif
#ifndef K32GSY_WD_STOP_CLOCK_A
LSA_VOID			K32_SYS_OUT_FCT_ATTR K32GSY_WD_STOP_CLOCK_A (LSA_VOID);
#endif
#ifndef K32GSY_WD_STOP_CLOCK_B
LSA_VOID			K32_SYS_OUT_FCT_ATTR K32GSY_WD_STOP_CLOCK_B (LSA_VOID);
#endif
#ifndef K32GSY_WD_STOP_TIME
LSA_VOID			K32_SYS_OUT_FCT_ATTR K32GSY_WD_STOP_TIME (LSA_VOID);
#endif
#ifndef K32GSY_CHANGE_CLOCK_A
LSA_VOID			K32_SYS_OUT_FCT_ATTR K32GSY_CHANGE_CLOCK_A (LSA_UINT8 *,LSA_UINT8 *,LSA_BOOL,
					LSA_VOID LSA_FCT_PTR(K32_SYS_IN_FCT_ATTR,Cbf)(LSA_VOID));
#endif
#ifndef K32GSY_CHANGE_CLOCK_B
LSA_VOID			K32_SYS_OUT_FCT_ATTR K32GSY_CHANGE_CLOCK_B (LSA_UINT8 *,LSA_UINT8 *,LSA_BOOL,
					LSA_VOID LSA_FCT_PTR(K32_SYS_IN_FCT_ATTR,Cbf)(LSA_VOID));
#endif
#ifndef K32GSY_CHANGE_TIME
LSA_VOID			K32_SYS_OUT_FCT_ATTR K32GSY_CHANGE_TIME (LSA_UINT8 *,LSA_UINT8 *,LSA_BOOL,
					LSA_VOID LSA_FCT_PTR(K32_SYS_IN_FCT_ATTR,Cbf)(LSA_VOID));
#endif
#ifndef K32GSY_FWD_TX_SET_CLOCK
LSA_VOID			K32_SYS_OUT_FCT_ATTR K32GSY_FWD_TX_SET_CLOCK (LSA_UINT8,LSA_UINT16,LSA_BOOL);
#endif
#ifndef K32GSY_FWD_TX_SET_TIME
LSA_VOID			K32_SYS_OUT_FCT_ATTR K32GSY_FWD_TX_SET_TIME (LSA_UINT8,LSA_UINT16,LSA_BOOL);
#endif
#ifndef K32GSY_FWD_RX_SET_CLOCK
LSA_VOID			K32_SYS_OUT_FCT_ATTR K32GSY_FWD_RX_SET_CLOCK (LSA_UINT8,LSA_UINT16,LSA_BOOL);
#endif
#ifndef K32GSY_FWD_RX_SET_TIME
LSA_VOID			K32_SYS_OUT_FCT_ATTR K32GSY_FWD_RX_SET_TIME (LSA_UINT8,LSA_UINT16,LSA_BOOL);
#endif
#ifndef K32GSY_RRQB_ALLOC
K32_RQB_PTR_TYPE	K32_SYS_OUT_FCT_ATTR K32GSY_RRQB_ALLOC (LSA_VOID);
#endif
#ifndef K32GSY_RRQB_FREE
LSA_BOOL			K32_SYS_OUT_FCT_ATTR K32GSY_RRQB_FREE (K32_RQB_PTR_TYPE);
#endif


/* ------------------------------------------------------------------------------------------------------------ */
/* PACKED(4) STRUCTS - OFF                                                                                      */
/* ------------------------------------------------------------------------------------------------------------ */
#ifdef K32_KRISC32OS
    #include "pnio_k32_pck_off.h"
#else
    #include "pnio_pck_off.h"
#endif


/*****************************************************************************/
/*  end of file EDDP_K32_CFG.H                                               */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_K32_CFG_H */
