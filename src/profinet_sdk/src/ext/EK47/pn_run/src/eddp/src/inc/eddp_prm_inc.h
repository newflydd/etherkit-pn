
#ifndef EDDP_PRM_INC_H                   /* ----- reinclude-protection ----- */
#define EDDP_PRM_INC_H

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
/*  F i l e               &F: eddp_prm_inc.h                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Device/Handle                                                            */
/*  Defines constants, types, macros and prototyping for PRM                 */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  04.12.08    JS    initial version.                                       */
#endif
/*****************************************************************************/

/*===========================================================================*/
/*                                switches                                   */
/*===========================================================================*/

/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/

/*===========================================================================*/
/*                                defines                                    */
/*===========================================================================*/
#define EDDP_PRM_MAX_IRFRAMEDATA_ENTRIES_ERTEC_200          1024
#define EDDP_PRM_MAX_ASSIGNMENTS                            5
#define EDDP_PRM_MAX_PHASES                                 16
#define EDDP_PRM_MAX_PTCPSUBDOMAINNAMELENGTH                240

#define EDDP_PRM_PDNCDATACHECK_MAX_SIZE                     (8 /*[BlockHeader, Padding, Padding]*/\
                                                            +4 /*[MaintenanceRequiredDropBudget]*/\
                                                            +4 /*[MaintenanceDemandedDropBudget]*/\
                                                            +4)/*[ErrorDropBudget]*/
                                                        /* =20*/


// PDIRDATA:
//    [BlockHeader, Padding, Padding, SlotNumber, SubslotNumber] 12 (constant)
//
//    PDIRGlobalData:
//                    [BlockHeader, Padding, Padding] 8 (constant)
//                    [IRDataUUID] 16 (constant)
//                    [MaxBridgeDelay] 4 (constant)
//                    [NumberOfPorts] 4 (constant)
//                    (MaxPortTxDelay 4, MaxPortRxDelay 4, MaxLineRxDelay 4, YellowTime 4)* = 16 (multiplied by NumberOfPorts)
//
//    PDIRFrameData:
//                    [BlockHeader, Padding, Padding] 8 (constant)
//                    [FrameDataProperties] 4 (constant)
//                    (FrameSendOffset 4, DataLength 2, ReductionRatio 2, Phase 2, FrameID 2, EtherType 2, RxPort 1, FrameDetails 1, TxPortGroup 2, Padding 2)* = 20 (equals EDDP_PRM_PDIRFRAMEDATA_ONE_ENTRY_SIZE)
//    
//    PDIRBeginEndData:
//                    [BlockHeader, Padding, Padding] 8 (constant)
//                    [RedGuard] 4 (constant)
//                    [NumberOfPorts] 4 (constant)
//                    (NumberOfAssignments 4, ( TXBeginEndAssignment 12, RXBeginEndAssignment 12 )*5 ,NumberOfPhases 4, (TXPhaseAssignment 2, RXPhaseAssignment 2 )*16 )* = 192 (multiplied by NumberOfPorts)
#define EDDP_PRM_PDIRDATA_CONSTANT_1                        208 /* sum of all (multiplied by NumberOfPorts) */
#define EDDP_PRM_PDIRDATA_CONSTANT_2                        20  /* equals to EDDP_PRM_PDIRFRAMEDATA_ONE_ENTRY_SIZE */
#define EDDP_PRM_PDIRDATA_CONSTANT_3                        72  /* sum of all (constant) values */


#define EDDP_PRM_PDSYNCDATA_MAX_SIZE                         (8/*[BlockHeader, Padding, Padding]*/\
                                                            +16/*[PTCP_SubdomainUUID]*/\
                                                             +4/*[ReservedIntervalBegin]*/\
                                                             +4/*[ReservedIntervalEnd]*/\
                                                             +4/*[PLLWindow]*/\
                                                             +4/*[SyncSendFactor]*/\
                                                             +2/*[SendClockFactor]*/\
                                                             +2/*[PTCPTimeoutFactor]*/\
                                                             +2/*[PTCPTakeoverTimeoutFactor]*/\
                                                             +2/*[PTCPMasterStartupTime]*/\
                                                             +2/*[SyncProperties]*/\
                                                             +1/*[PTCP_MasterPriority1]*/\
                                                             +1/*[PTCP_MasterPriority2]*/\
                                                             +1/*[PTCPLengthSubdomainName]*/\
                                                           +240/*[PTCPSubdomainName]*/\
                                                             +3)/*[Padding]*/
                                                        /* =296*/


#define EDDP_PRM_PDINTERFACEMRPDATAADJUST_MAX_SIZE			  (8 /*[BlockHeader, Padding, Padding]*/\
                                                             +16 /*[MRP_DomainUUID]*/\
                                                              +2 /*[MRP_Role]*/\
                                                              +2 /*[Padding]*/\
                                                              +1 /*[MRP_LengthDomainName]*/\
                                                            +240 /*[MRP_DomainName]*/\
                                                              +3 /*[Padding]*/\
                                                             +18 /*[(MrpManagerParams)(18) ^ (MrpClientParams)(12)]*/\
                                                              +2) /*[Padding]*/
                                                         /* =292 */

#define EDDP_PRM_PDPORTMRPDATAADJUST_MAX_SIZE                (8 /*[BlockHeader, Padding, Padding]*/\
                                                            +16)/*[MRP_DomainUUID]*/
                                                         /* =24 */

#define EDDP_PRM_PDPORTDATAADJUST_MAX_SIZE                  (12 /*[BlockHeader, Padding, Padding, SlotNumber, SubslotNumber]*/\
                                                            +20 /*[AdjustDomainBoundary]*/\
                                                            +16 /*[AdjustMulticastBoundary]*/\
                                                            +12 /*[AdjustMAUType ^ AdjustLinkState]*/\
                                                            +16 /*[AdjustPeerToPeerBoundary]*/\
                                                            +16 /*[AdjustDCPBoundary]*/\
                                                            +12)/*[AdjustPreambleLength]*/
                                                        /* =104 */


// counter for MAC-Adr[Byte 5]
#define EDDP_PRM_DOMAIN_BOUNDARY_COUNT                      1
// MC Address   0x01:0x0E:0xCF:0x00:0x04:0x00   -> only this is used by EDDP currently
//              0x01:0x0E:0xCF:0x00:0x04:0x01

#define EDDP_PRM_DOMAIN_BOUNDARY_MASK                       0xFFFFFFFC

#define EDDP_PRM_DOMAIN_BOUNDARY_NOT_BLOCKED                0

#define EDDP_PRM_SUPPORTED_MAUTYPE_100BASETXFD              0x0010

#define EDDP_PRM_LINK_STATE_DOWN                            0x0002

#define EDDP_PRM_INVALID_VALUE_8                            0xFF
#define EDDP_PRM_INVALID_VALUE_16                           0xFFFF
#define EDDP_PRM_INVALID_VALUE_32                           0xFFFFFFFF

#define EDDP_PRM_LIMIT_LSA_INT32_MAX                        0x7FFFFFFFL
    
/* ============================================================================ */
/*                              structs for all                                 */
/* ============================================================================ */

typedef struct _EDDP_PRM_CHAIN_TYPE
{
    LSA_UINT16      Prev;       // prev index in used list
    LSA_UINT16      Next;       // next index in used list
} EDDP_PRM_CHAIN_TYPE;


typedef struct _EDDP_PRM_CHAIN_BASE_TYPE
{
    LSA_UINT16      BeginIdx;       // begin index in list
    LSA_UINT16      EndIdx;         // end index in list
} EDDP_PRM_CHAIN_BASE_TYPE;


#define EDDP_PRM_CHAIN_IDX_END          (EDDP_PRM_INVALID_VALUE_16)

/*===========================================================================*/
/*                              Macros                                       */
/*===========================================================================*/
/** macro to set values for EDD_RQB_PRM_END_TYPE
 *
 * @param   pPrmEnd         pointer to struct EDD_RQB_PRM_END_TYPE
 * @param   _err_port_id    LSA_UINT16 value for err_port_id
 * @param   _err_index      LSA_UINT32 value for err_index
 * @param   _err_offset     LSA_UINT32 value for err_offset
 * @param   _err_fault      LSA_BOOL value for err_fault
 */
#define EDDP_RQB_PRM_END_SET(pPrmEnd, _err_port_id, _err_index, _err_offset, _err_fault) \
{                                                                                       \
    ((EDD_UPPER_PRM_END_PTR_TYPE)(pPrmEnd))->err_port_id  = (_err_port_id);       \
    ((EDD_UPPER_PRM_END_PTR_TYPE)(pPrmEnd))->err_index    = (_err_index);         \
    ((EDD_UPPER_PRM_END_PTR_TYPE)(pPrmEnd))->err_offset   = (_err_offset);        \
    ((EDD_UPPER_PRM_END_PTR_TYPE)(pPrmEnd))->err_fault    = ((_err_fault) ? EDD_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS : EDD_PRM_ERR_FAULT_NO_OR_TOO_LESS_PARAMS);   \
}

/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/

/* ---------------------------------------------------------------------------- */
/* PRM/Diagnosis (configured by PRM-services)                                   */
/* ---------------------------------------------------------------------------- */

/* ---------------------------------------------------------------------------- */
/* PDU-Format:                                                                  */
/* ---------------------------------------------------------------------------- */

/* ---------------------------------------------------------------------------- */
/*   BlockHeader                                                                */
/* ---------------------------------------------------------------------------- */
/*    LSA_UINT16          BlockType                                     2       */
/*    LSA_UINT16          BlockLength        (without Type/Length)      2       */
/*    LSA_UINT8           BlockVersionHigh                              1       */  
/*    LSA_UINT8           BlockVersionLow                               1       */
/*                                                                     ---      */
/*                                                                      6       */
/* ---------------------------------------------------------------------------- */
#define EDDP_PRM_RECORD_HEADER_SIZE           6
#define EDDP_PRM_BLOCK_VERSION_HIGH           0x01
#define EDDP_PRM_BLOCK_VERSION_LOW            0x00
#define EDDP_PRM_STATISTIC_BLOCK_VERSION_LOW  0x01


/* ---------------------------------------------------------------------------- */
/*   PDNCDataCheck (indication barriers for dropped frames)                     */
/* ---------------------------------------------------------------------------- */
/*                                                                              */
/*    BlockHeader                                                       6       */
/*    LSA_UINT8           Padding                                       1       */
/*    LSA_UINT8           Padding                                       1       */
/*    LSA_UINT32          MaintenanceRequiredDropBudget                 4       */
/*    LSA_UINT32          MaintenanceDemandedDropBudget                 4       */
/*    LSA_UINT32          ErrorDropBudget                               4       */
/*                                                                    -----     */
/*                                                                     20       */
/* ---------------------------------------------------------------------------- */
#define EDDP_PRM_PDNC_DATA_CHECK_RECORD_INDEX           0x8070

#define EDDP_PRM_PDNC_DATA_CHECK_BLOCK_TYPE             0x0230
#define EDDP_PRM_PDNC_DATA_CHECK_BLOCK_SIZE             (EDDP_PRM_RECORD_HEADER_SIZE+14)

#define EDDP_PRM_PDNC_DATA_CHECK_BUDGET_VALUE_MSK       0x7FFFFFFF

#define EDDP_PRM_PDNC_DATA_CHECK_BUDGET_CHECK_MSK       0x80000000
#define EDDP_PRM_PDNC_DATA_CHECK_BUDGET_CHECK_ON        0x80000000
#define EDDP_PRM_PDNC_DATA_CHECK_BUDGET_CHECK_OFF       0x00000000

#define EDDP_PRM_PDNC_DROP_BUDGET_MIN                   0x1
#define EDDP_PRM_PDNC_DROP_BUDGET_MAX                   0x3E7

/* ---------------------------------------------------------------------------- */
/* Record for ExtChannelErrorType "Frame dropped"                               */
/* Note: will be set to 0 at startup -> default record                          */
/* ---------------------------------------------------------------------------- */
typedef struct _EDDP_PRM_BUDGET_TYPE           
{
    LSA_BOOL        Enabled;
    LSA_UINT32      Value;
} EDDP_PRM_BUDGET_TYPE;

typedef struct _EDDP_NC_DATA_CHECK_RECORD_SET_TYPE  EDDP_LOCAL_MEM_ATTR *   EDDP_NC_DATA_CHECK_RECORD_SET_PTR_TYPE;
typedef struct _EDDP_NC_DATA_CHECK_RECORD_SET_TYPE
{
    EDDP_PRM_BUDGET_TYPE        ErrorDropBudget;
    EDDP_PRM_BUDGET_TYPE        MaintenanceDemandedDropBudget;
    EDDP_PRM_BUDGET_TYPE        MaintenanceRequiredDropBudget;    
} EDDP_NC_DATA_CHECK_RECORD_SET_TYPE;


typedef struct _EDDP_PRM_NC_DATA_TYPE   EDDP_LOCAL_MEM_ATTR *   EDDP_PRM_NC_DATA_PTR_TYPE;
typedef struct _EDDP_PRM_NC_DATA_TYPE            
{
    LSA_UINT64                          DropCnt;                      /* drop count last interval */
    LSA_BOOL                            CheckEnable;                  /* Check Enable             */
    LSA_UINT8                           ErrorStatus;                  /* Error Status             */
    LSA_UINT8                           ErrorStatusIndicated;         /* Error Status indicated   */
    EDDP_NC_DATA_CHECK_RECORD_SET_TYPE  Budgets;                      /* Dropped frames budgets   */
} EDDP_PRM_NC_DATA_TYPE;          


/* CheckEnable (if record set A is present and something to check) */
/* ErrorStatus          (actual Error Status)                      */
/* ErrorStatusIndicated (actual Error Status indicated to user)    */
#define EDDP_DIAG_FRAME_DROP_OK                             0   /* dont change */
#define EDDP_DIAG_FRAME_DROP_ERROR                          1
#define EDDP_DIAG_FRAME_DROP_MAINTENANCE_DEMANDED           2
#define EDDP_DIAG_FRAME_DROP_MAINTENANCE_REQUIRED           3


/* ---------------------------------------------------------------------------- */
/*   PDPortDataAdjust                                                           */
/* ---------------------------------------------------------------------------- */
/*                                                                              */
/*    BlockHeader                                                       6       */
/*    LSA_UINT8           Padding                                       1       */
/*    LSA_UINT8           Padding                                       1       */
/*    LSA_UINT16          SlotNumber                                    2       */
/*    LSA_UINT16          SubslotNumber                                 2       */
/*                                                                    -----     */
/*                                                                     12       */
/*    [AdjustDomainBoundary]                                                    */
/*    [AdjustMulticastBoundary]                                                 */
/*    [AdjustMAUType ^ AdjustLinkState]                                         */
/*    [AdjustPeerToPeerBoundary]                                                */
/*    [AdjustDCPBoundary]                                                       */
/*    [AdjustFastForwardingBoundary]                                            */
/*    [AdjustPreambleLength]                                                    */
/*                                                                              */
/*    AdjustDomainBoundary:                                                     */
/*                                                                              */
/*    BlockHeader                                                       6       */
/*    LSA_UINT8           Padding                                       1       */
/*    LSA_UINT8           Padding                                       1       */
/*    LSA_UINT32          DomainBoundaryIngress                         4       */
/*    LSA_UINT32          DomainBoundaryEgress                          4       */
/*    LSA_UINT16          AdjustProperties                              2       */
/*    LSA_UINT8           Padding                                       1       */
/*    LSA_UINT8           Padding                                       1       */
/*                                                                    -----     */
/*                                                                     20       */
/*    AdjustMulticastBoundary:                                                  */
/*                                                                              */
/*    BlockHeader                                                       6       */
/*    LSA_UINT8           Padding                                       1       */
/*    LSA_UINT8           Padding                                       1       */
/*    LSA_UINT32          MulticastBoundary                             4       */
/*    LSA_UINT16          AdjustProperties                              2       */
/*    LSA_UINT8           Padding                                       1       */
/*    LSA_UINT8           Padding                                       1       */
/*                                                                    -----     */
/*                                                                     16       */
/*    AdjustMAUType                                                             */
/*                                                                              */
/*    BlockHeader                                                       6       */
/*    LSA_UINT8           Padding                                       1       */
/*    LSA_UINT8           Padding                                       1       */
/*    LSA_UINT16          MAUType                                       2       */
/*    LSA_UINT16          AdjustProperties                              2       */
/*                                                                    -----     */
/*                                                                     12       */
/*                                                                              */
/*    AdjustLinkState                                                           */
/*                                                                              */
/*    BlockHeader                                                       6       */
/*    LSA_UINT8           Padding                                       1       */
/*    LSA_UINT8           Padding                                       1       */
/*    LSA_UINT16          LinkState                                     2       */
/*    LSA_UINT16          AdjustProperties                              2       */
/*                                                                    -----     */
/*                                                                     12       */
/*    AdjustPeerToPeerBoundary                                                  */
/*                                                                              */
/*    BlockHeader                                                       6       */
/*    LSA_UINT8           Padding                                       1       */
/*    LSA_UINT8           Padding                                       1       */
/*    LSA_UINT32          PeerToPeerBoundary                            4       */
/*    LSA_UINT16          AdjustProperties                              2       */
/*    LSA_UINT8           Padding                                       1       */
/*    LSA_UINT8           Padding                                       1       */
/*                                                                    -----     */
/*                                                                     16       */
/*                                                                              */
/*    AdjustDCPBoundary                                                         */
/*                                                                              */
/*    BlockHeader                                                       6       */
/*    LSA_UINT8           Padding                                       1       */
/*    LSA_UINT8           Padding                                       1       */
/*    LSA_UINT32          DCPBoundary                                   4       */
/*    LSA_UINT16          AdjustProperties                              2       */
/*    LSA_UINT8           Padding                                       1       */
/*    LSA_UINT8           Padding                                       1       */
/*                                                                    -----     */
/*                                                                     16       */
/*    AdjustPreambleLength                                                      */
/*                                                                              */
/*    BlockHeader                                                       6       */
/*    LSA_UINT8           Padding                                       1       */
/*    LSA_UINT8           Padding                                       1       */
/*    LSA_UINT16          PreambleLength                                2       */
/*    LSA_UINT16          AdjustProperties                              2       */
/*                                                                    -----     */
/*                                                                     12       */
/* ---------------------------------------------------------------------------- */
#define EDDP_PRM_PDPORT_DATA_ADJUST_RECORD_INDEX                0x802F

#define EDDP_PRM_PDPORT_DATA_ADJUST_BLOCK_TYPE                  0x0202

/* AdjustDomainBoundary  */
#define EDDP_PRM_PDPORT_DATA_ADJUST_D_BOUNDARY_BLOCK_TYPE       0x0209

#define EDDP_PRM_PDPORT_DATA_ADJUST_D_ADJUST_PROPERTIES         0

/* AdjustMulticastBoundary */
#define EDDP_PRM_PDPORT_DATA_ADJUST_MC_BOUNDARY_BLOCK_TYPE      0x0210

#define EDDP_PRM_PDPORT_DATA_MC_BOUNDARY_BLOCK                  1 
#define EDDP_PRM_PDPORT_DATA_MC_BOUNDARY_NOT_BLOCK              0 

#define EDDP_PRM_PDPORT_DATA_MC_ADJUST_PROPERTIES               0 

/* AdjustMAUType */
#define EDDP_PRM_PDPORT_DATA_ADJUST_MAUTYPE_BLOCK_TYPE          0x020E

#define EDDP_PRM_PDPORT_DATA_MAUTYPE_10BASETXHD                 0x000A
#define EDDP_PRM_PDPORT_DATA_MAUTYPE_10BASETXFD                 0x000B
#define EDDP_PRM_PDPORT_DATA_MAUTYPE_10BASEFLHD                 0x000C
#define EDDP_PRM_PDPORT_DATA_MAUTYPE_10BASEFLFD                 0x000D
#define EDDP_PRM_PDPORT_DATA_MAUTYPE_100BASETXHD                0x000F  
#define EDDP_PRM_PDPORT_DATA_MAUTYPE_100BASETXFD                0x0010
#define EDDP_PRM_PDPORT_DATA_MAUTYPE_100BASEFXHD                0x0011
#define EDDP_PRM_PDPORT_DATA_MAUTYPE_100BASEFXFD                0x0012
#define EDDP_PRM_PDPORT_DATA_MAUTYPE_1000BASEXHD                0x0015 
#define EDDP_PRM_PDPORT_DATA_MAUTYPE_1000BASEXFD                0x0016 
#define EDDP_PRM_PDPORT_DATA_MAUTYPE_1000BASELXHD               0x0017 
#define EDDP_PRM_PDPORT_DATA_MAUTYPE_1000BASELXFD               0x0018
#define EDDP_PRM_PDPORT_DATA_MAUTYPE_1000BASESXHD               0x0019
#define EDDP_PRM_PDPORT_DATA_MAUTYPE_1000BASESXFD               0x001A
#define EDDP_PRM_PDPORT_DATA_MAUTYPE_1000BASETHD                0x001D
#define EDDP_PRM_PDPORT_DATA_MAUTYPE_1000BASETFD                0x001E
#define EDDP_PRM_PDPORT_DATA_MAUTYPE_10GigBASEFX                0x001F
#define EDDP_PRM_PDPORT_DATA_MAUTYPE_100BASELX10                0x002E
#define EDDP_PRM_PDPORT_DATA_MAUTYPE_1000BASELX10               0x0031
#define EDDP_PRM_PDPORT_DATA_MAUTYPE_100BASEPXFD                0x0036

#define EDDP_PRM_PDPORT_DATA_MAUTYPE_ADJUST_PROPERTIES          0 

/* AdjustPortState */
#define EDDP_PRM_PDPORT_DATA_ADJUST_LINKSTATE_BLOCK_TYPE        0x021B

#define EDDP_PRM_PDPORT_DATA_LINKSTATE_UP                       1 
#define EDDP_PRM_PDPORT_DATA_LINKSTATE_DOWN                     2   /* only allowed value with PDPortDataAdjust */
#define EDDP_PRM_PDPORT_DATA_LINKSTATE_TESTING                  3 
#define EDDP_PRM_PDPORT_DATA_LINKSTATE_UNKNOWN                  4 

#define EDDP_PRM_PDPORT_DATA_LINKSTATE_ADJUST_PROPERTIES        0 

/* AdjustPeerToPeerBoundary */
#define EDDP_PRM_PDPORT_DATA_ADJUST_P2P_BOUNDARY_BLOCK_TYPE     0x0224

#define EDDP_PDPORT_DATA_ADJUST_P2P_ADJUST_PROPERTIES           0

/* AdjustDCPBoundary */
#define EDDP_PRM_PDPORT_DATA_ADJUST_DCP_BOUNDARY_BLOCK_TYPE     0x0225

#define EDDP_PRM_PDPORT_DATA_DCP_BOUNDARY_BLOCK                 1 
#define EDDP_PRM_PDPORT_DATA_DCP_BOUNDARY_NOT_BLOCK             0 

#define EDDP_PRM_PDPORT_DATA_DCP_ADJUST_PROPERTIES              0 

/* AdjustPreambleLength */
#define EDDP_PRM_PDPORT_DATA_ADJUST_PREAMBLELENGTH_BLOCK_TYPE   0x0226
#define EDDP_PRM_PDPORT_DATA_PREAMBLELENGTH_SHORT               1   // One octet Preamble shall be used 
#define EDDP_PRM_PDPORT_DATA_PREAMBLELENGTH_LONG                0   // Seven octets Preamble shall be used
#define EDDP_PRM_PDPORT_DATA_ADJUST_PREAMBLELENGTH_PROPERTIES   0 

typedef struct _EDDP_PORT_DATA_ADJUST_RECORD_SET_TYPE   EDDP_LOCAL_MEM_ATTR *   EDDP_PORT_DATA_ADJUST_RECORD_SET_PTR_TYPE;
typedef struct _EDDP_PORT_DATA_ADJUST_RECORD_SET_TYPE
{                                                      
    LSA_UINT16             SlotNumber;
    LSA_UINT16             SubslotNumber;

    LSA_BOOL               DomainBoundaryIngressEgress_IsPresent;
    LSA_BOOL               MulticastBoundary_IsPresent;
    LSA_BOOL               MAUType_IsPresent;
    LSA_BOOL               LinkState_IsPresent;
    LSA_BOOL               PeerToPeerBoundary_IsPresent;
    LSA_BOOL               DCPBoundary_IsPresent;
    LSA_BOOL               PreambleLength_IsPresent;    // TRUE if written by PDPORT_DATA_ADJUST_PREAMBLELENGTH

    LSA_UINT32             DomainBoundaryIngress;
    LSA_UINT32             DomainBoundaryEgress;
    LSA_UINT32             MulticastBoundary;
    LSA_UINT32             PeerToPeerBoundary;
    LSA_UINT32             DCPBoundary;

    LSA_UINT16             MAUType;
    LSA_UINT16             LinkState;
    LSA_UINT16             PreambleLength;              // long(0) or short(1) preamble length

    LSA_UINT32             Speed;
    LSA_UINT32             Duplexity;
    LSA_UINT8              MediaType;
    LSA_UINT8              OpticalTransType;
} EDDP_PORT_DATA_ADJUST_RECORD_SET_TYPE;

typedef struct _EDDP_PRM_PORT_DATA_ADJUST_TYPE  EDDP_LOCAL_MEM_ATTR *    EDDP_PRM_PORT_DATA_ADJUST_PTR_TYPE;
typedef struct _EDDP_PRM_PORT_DATA_ADJUST_TYPE            
{      
    LSA_BOOL                                LesserCheckQuality;                 /* Flag from CDB.                 */
    EDDP_PORT_DATA_ADJUST_RECORD_SET_TYPE   Parameters[EDDP_MAX_PORT_CNT +1];   /* +1 to ease access with Port ID */ 
} EDDP_PRM_PORT_DATA_ADJUST_TYPE;          


/* ---------------------------------------------------------------------------- */
/*   PDSCFDataCheck (= SendClock factor check)                                  */
/* ---------------------------------------------------------------------------- */
/*                                                                              */
/*    BlockHeader                                                       6       */
/*    LSA_UINT8           Padding                                       1       */
/*    LSA_UINT8           Padding                                       1       */
/*    LSA_UINT16          SendClockFactor                               2       */
/*    LSA_UINT16          SendClockProperties                           2       */
/*    LSA_UINT16          Reserved                                      2       */
/*    LSA_UINT8           Data[x]                                       x       */
/*                                                                    -----     */
/*                                                                      8+x     */
/* ---------------------------------------------------------------------------- */
#define EDDP_PRM_PDSCF_DATA_CHECK_RECORD_INDEX          0x10000

#define EDDP_PRM_PDSCF_DATA_CHECK_BLOCK_TYPE            0xF000
#define EDDP_PRM_PDSCF_DATA_CHECK_BLOCKVERSIONHIGH      (EDDP_PRM_BLOCK_VERSION_HIGH)
#define EDDP_PRM_PDSCF_DATA_CHECK_BLOCKVERSIONLOW       (EDDP_PRM_BLOCK_VERSION_LOW)
// MinSize(PDSCFDataCheck)  : BlockHeader(6) + SendClockFactor(2) + SendClockProperties(2) + Reserved(2)
#define EDDP_PRM_PDSCF_DATA_CHECK_BLOCK_MIN_SIZE        (EDDP_PRM_RECORD_HEADER_SIZE + 2 + 2 + 2)
#define EDDP_PRM_PDSCF_DATA_CHECK_BLOCK_SIZE            (EDDP_PRM_PDSCF_DATA_CHECK_BLOCK_MIN_SIZE + 4)

// struct of PDSCFDataCheck
typedef struct _EDDP_PRM_PDSCFDATACHECK_TYPE
{
    EDD_COMMON_MEM_PTR_TYPE     pPDSCFDataCheck;
    LSA_UINT32                  PDSCFDataCheck_Size;
    LSA_UINT32                  PDSCFDataCheck_Length;
    LSA_BOOL                    PDSCFDataCheck_IsPresent;
} EDDP_PRM_PDSCFDATACHECK_TYPE;


/* ---------------------------------------------------------------------------- */
/*   PDSetDefaultPortState (= ?????)                                            */
/* ---------------------------------------------------------------------------- */
/*                                                                              */
/*    BlockHeader                                                       6       */
/*    LSA_UINT8           Padding                                       1       */
/*    LSA_UINT8           Padding                                       1       */
/*    LSA_UINT8           PortId                                        1       */
/*    LSA_UINT8           PortState                                     1       */
/*                                                                    -----     */
/*                                                                     10       */
/* ---------------------------------------------------------------------------- */
/*                                                                              */
/* PortID: 1..255 (a valid PortId)                                              */
/* each PortId must be present Port and only appears once within record!        */
/* Multiple PortId settings may be present within record.                       */
/*                                                                              */
/********************************************************************************/

/* defines for PortState */
#define EDDP_PRM_RECORD_PD_SET_DEFAULT_PORT_STATES_DISABLE                  0x01
#define EDDP_PRM_RECORD_PD_SET_DEFAULT_PORT_STATES_BLOCKING                 0x02

#define EDDP_PRM_RECORD_PD_SET_DEFAULT_PORT_STATES_OFFSET_PortID            0
#define EDDP_PRM_RECORD_PD_SET_DEFAULT_PORT_STATES_OFFSET_PortState         1


#define EDDP_PRM_PD_SET_DEFAULT_PORT_STATES_RECORD_INDEX          0x10001

#define EDDP_PRM_PD_SET_DEFAULT_PORT_STATES_BLOCK_TYPE            0xF001
#define EDDP_PRM_PD_SET_DEFAULT_PORT_STATES_BLOCKVERSIONHIGH      (EDDP_PRM_BLOCK_VERSION_HIGH)
#define EDDP_PRM_PD_SET_DEFAULT_PORT_STATES_BLOCKVERSIONLOW       (EDDP_PRM_BLOCK_VERSION_LOW)
#define EDDP_PRM_PD_SET_DEFAULT_PORT_STATES_BLOCK_MIN_SIZE        (EDDP_PRM_RECORD_HEADER_SIZE + 2 + 4)
#define EDDP_PRM_PD_SET_DEFAULT_PORT_STATES_BLOCK_SIZE            (EDDP_PRM_PDSCF_DATA_CHECK_BLOCK_MIN_SIZE)

// struct of PDSetDefaultPortState
typedef struct _EDDP_PRM_PDSetDefaultPortState_TYPE
{
    LSA_UINT8           PortId;
    LSA_UINT8           PortState;
    LSA_UINT8           Padding_1;
    LSA_UINT8           Padding_2;
} EDDP_PRM_PDSetDefaultPortState_TYPE;

#define EDDP_PRM_PDSETDEFAULTPORTSTATE_BLOCKSIZE                4


/* ---------------------------------------------------------------------------- */
/*   PDIRApplicationData                                                        */
/*      -> only if (PDIRGlobalData.BlockVersionLow <= 0x2)                      */
/* ---------------------------------------------------------------------------- */
/*    BlockHeader                                                       6       */
/*    LSA_UINT16          ConsumerCnt                                   2       */
/*    LSA_UINT16          ConsumerIODataLength                          2       */
/*    LSA_UINT16          Padding                                       2       */
/* ---------------------------------------------------------------------------- */
#define EDDP_PRM_PDIRAPPLICATIONDATA_RECORD_INDEX       0x10002

#define EDDP_PRM_PDIRAPPLICATIONDATA_BLOCK_TYPE         0xF002
#define EDDP_PRM_PDIRAPPLICATIONDATA_BLOCKVERSIONHIGH   (EDDP_PRM_BLOCK_VERSION_HIGH)
#define EDDP_PRM_PDIRAPPLICATIONDATA_BLOCKVERSIONLOW    (EDDP_PRM_BLOCK_VERSION_LOW)
#define EDDP_PRM_PDIRAPPLICATIONDATA_BLOCK_SIZE         (EDDP_PRM_RECORD_HEADER_SIZE + 2 + 2 + 2)

typedef struct _EDDP_PRM_PDIRAPPLICATIONDATA_TYPE   EDDP_LOCAL_MEM_ATTR *   EDDP_PRM_PDIRAPPLICATIONDATA_PTR_TYPE;
typedef struct _EDDP_PRM_PDIRAPPLICATIONDATA_TYPE
{
    LSA_UINT16      ConsumerCnt;
    LSA_UINT16      ConsumerIODataLength;
} EDDP_PRM_PDIRAPPLICATIONDATA_TYPE;
/* ConsumerCnt              : Max number of Consumers active within this device. If set to 0 the EDD uses the WorstCase.            */
/* ConsumerIODataLength     : Sum of all C_SDU data of all Consumers (RTC1/RTC2/RTC3/RToverUPD) this device (IOD or IOC)            */
/*                            receives locally. If set to 0 the EDD uses the WorstCase.                                             */
// max value of "ConsumerIODataLength"
#define EDDP_PRM_PDIRAPPLICATIONDATA_MAX_CONSUMER_IODATA_LENGTH         1440

/* ---------------------------------------------------------------------------- */
/*   PDPortMrpDataAdjust                                                        */
/* ---------------------------------------------------------------------------- */
/*   Note: We only support WRITE ! and only check for Record to be written      */
/*   (no validation!)                                                           */
/* ---------------------------------------------------------------------------- */
typedef struct _EDDP_PRM_MRP_DATA_TYPE            
{
    LSA_UINT32      RecordCountInSetB;
} EDDP_PRM_MRP_PORT_DATA_TYPE;          
     

/* ---------------------------------------------------------------------------- */
/*   PDSyncData                                                       bytes     */
/* ---------------------------------------------------------------------------- */
/*    BlockHeader                                                       6       */
/*    LSA_UINT8                      Padding                            1       */
/*    LSA_UINT8                      Padding                            1       */
/*    EDDP_PRM_PTCPSUBDOMAINID_TYPE  PTCP_SubdomainUUID                16       */
/*    LSA_UINT32                     ReservedIntervalBegin              4       */     
/*    LSA_UINT32                     ReservedIntervalEnd                4       */
/*    LSA_UINT32                     PLLWindow                          4       */
/*    LSA_UINT32                     SyncSendFactor                     4       */
/*    LSA_UINT32                     SendClockFactor                    2       */
/*    LSA_UINT16                     PTCPTimeoutFactor                  2       */
/*    LSA_UINT16                     PTCPTakeoverTimeoutFactor          2       */
/*    LSA_UINT16                     PTCPMasterStartupTime              2       */
/*    LSA_UINT16                     SyncProperties                     2       */
/*    LSA_UINT8                      PTCP_MasterPriority1               1       */
/*    LSA_UINT8                      PTCP_MasterPriority2               1       */
/*    LSA_UINT8                      PTCPLengthSubdomainName            1       */
/*    (                                                                         */
/*      LSA_UINT8                    PTCPSubdomainName                  1       */
/*    )*                                                                        */       
/* ---------------------------------------------------------------------------- */
#define EDDP_PRM_PDSYNCDATA_RECORD_INDEX            0x802D
#define EDDP_PRM_PDSYNCDATA_BLOCKTYPE               0x0203
#define EDDP_PRM_PDSYNCDATA_BLOCKVERSIONHIGH        (EDDP_PRM_BLOCK_VERSION_HIGH)
#define EDDP_PRM_PDSYNCDATA_BLOCKVERSIONLOW         (0x02)

/* Format of UUID: XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX */
typedef struct _EDDP_PRM_PTCPSUBDOMAINID_TYPE
{
    LSA_UINT32      UUID_part_1;
    LSA_UINT16      UUID_part_2;
    LSA_UINT16      UUID_part_3;
    LSA_UINT16      UUID_part_4;
    LSA_UINT8       UUID_part_5_array[6];
} EDDP_PRM_PTCPSUBDOMAINID_TYPE;

typedef struct _EDDP_PRM_PDSYNCDATA_TYPE
{
    EDDP_PRM_PTCPSUBDOMAINID_TYPE       PTCPSubdomainID;               
    LSA_UINT32                          ReservedIntervalBegin;
    LSA_UINT32                          ReservedIntervalEnd;      
    LSA_UINT32                          PLLWindow;                
    LSA_UINT32                          SyncSendFactor;           
    LSA_UINT32                          SendClockFactor;          
    LSA_UINT16                          PTCPTimeoutFactor;        
    LSA_UINT16                          PTCPTakeoverTimeoutFactor;
    LSA_UINT16                          PTCPMasterStartupTime;    
    LSA_UINT16                          SyncProperties;           
    LSA_UINT8                           PTCP_MasterPriority1;     
    LSA_UINT8                           PTCP_MasterPriority2;     
    LSA_UINT8                           PTCPLengthSubdomainName;                                                       
    LSA_UINT8                           PTCPSubdomainName[EDDP_PRM_MAX_PTCPSUBDOMAINNAMELENGTH];                                                            
} EDDP_PRM_PDSYNCDATA_TYPE;
/* ---------------------------------------------------------------------------- */


/* ---------------------------------------------------------------------------- */
/*   PDIRData                                                         bytes     */
/* ---------------------------------------------------------------------------- */
/*    BlockHeader                                                       6       */
/*    LSA_UINT8                 Padding                                 1       */
/*    LSA_UINT8                 Padding                                 1       */
/*    LSA_UINT16                SlotNumber                              2       */
/*    LSA_UINT16                SubslotNumber                           2       */
/*    ...................................................................       */
/*    BlockHeader (PDIRGlobalData)                                      6       */
/*    LSA_UINT8                 Padding                                 1       */
/*    LSA_UINT8                 Padding                                 1       */
/*    EDDP_PRM_IRDATAUUID_TYPE  IRDataUUID                             16       */     
/*    LSA_UINT32                MaxBridgeDelay                          4       */
/*    LSA_UINT32                NumberOfPorts                           4       */
/*    (                                                                         */
/*      LSA_UINT32                MaxPortTxDelay                        4       */
/*      LSA_UINT32                MaxPortRxDelay                        4       */
/*     [LSA_UINT32                MaxLineRxDelay   BlockVersionLow = 2  4       */
/*      LSA_UINT32                YellowTime]      BlockVersionLow = 2  4       */
/*    )*                                                                        */
/*    ...................................................................       */
/*   [BlockHeader (PDIRFrameData)                                       6       */
/*    LSA_UINT8                 Padding                                 1       */
/*    LSA_UINT8                 Padding                                 1       */
/*    LSA_UINT32                FrameDataProperties                     4       (only with BlockVersion V1.1 of PDIRFrameData)*/
/*    (                                                                         */
/*      LSA_UINT32              FrameSendOffset                         4       */
/*      LSA_UINT16              DataLength                              2       */
/*      LSA_UINT16              ReductionRatio                          2       */
/*      LSA_UINT16              Phase                                   2       */
/*      LSA_UINT16              FrameID                                 2       */
/*      LSA_UINT16              EtherType                               2       */
/*      LSA_UINT8               RxPort                                  1       */
/*      LSA_UINT8               FrameDetails                            1       */
/*      LSA_UINT8               NumberOfTxPortGroups                    1       */
/*      LSA_UINT8[..]           TxPortGroupArray                        1       */
/*     [LSA_UINT8               Padding]*                               2       */
/*    )*]                                                                       */
/*    ...................................................................       */
/*    BlockHeader (PDIRBeginEndData)                                    6       */
/*    LSA_UINT8                 Padding                                 1       */
/*    LSA_UINT8                 Padding                                 1       */
/*    LSA_UINT16                StartOfRedFrameID                       2       */
/*    LSA_UINT16                EndOfRedFrameID                         2       */
/*    LSA_UINT32                NumberOfPorts                           4       */
/*    (                                                                         */
/*      LSA_UINT32              NumberOfAssignments                     4       */
/*      (                                                                       */
/*        LSA_UINT32            RedOrangePeriodBegin    Tx              4       */
/*        LSA_UINT32            OrangePeriodBegin       Tx              4       */
/*        LSA_UINT32            GreenPeriodBegin        Tx              4       */
/*        LSA_UINT32            RedOrangePeriodBegin    Rx              4       */
/*        LSA_UINT32            OrangePeriodBegin       Rx              4       */
/*        LSA_UINT32            GreenPeriodBegin        Rx              4       */
/*      )*                                                                      */
/*      LSA_UINT32              NumberOfPhases                          4       */
/*      (                                                                       */
/*        LSA_UINT16            PhaseAssignment         Tx              2       */
/*        LSA_UINT16            PhaseAssignment         Rx              2       */
/*      )*                                                                      */
/*    )*                                                                        */ 
/* ---------------------------------------------------------------------------- */
#define EDDP_PRM_PDIRDATA_RECORD_INDEX                  0x802C

#define EDDP_PRM_PDIRDATA_BLOCKTYPE                     0x0205
#define EDDP_PRM_PDIRDATA_BLOCKVERSIONHIGH              (EDDP_PRM_BLOCK_VERSION_HIGH)
#define EDDP_PRM_PDIRDATA_BLOCKVERSIONLOW               (EDDP_PRM_PDIRDATA_BLOCKVERSIONHIGH)

#define EDDP_PRM_PDIRGLOBALDATA_BLOCKTYPE               0x0206
#define EDDP_PRM_PDIRGLOBALDATA_BLOCKVERSIONHIGH        (EDDP_PRM_BLOCK_VERSION_HIGH)
#define EDDP_PRM_PDIRGLOBALDATA_BLOCKVERSIONLOW         (EDDP_PRM_PDIRGLOBALDATA_BLOCKVERSIONHIGH)

#define EDDP_PRM_PDIRFRAMEDATA_BLOCKTYPE                0x0207
#define EDDP_PRM_PDIRFRAMEDATA_ONE_ENTRY_SIZE           20

#define EDDP_PRM_PDIRBEGINENDDATA_BLOCKTYPE             0x0208
#define EDDP_PRM_PDIRBEGINENDDATA_BLOCKVERSIONHIGH      (EDDP_PRM_BLOCK_VERSION_HIGH)
#define EDDP_PRM_PDIRBEGINENDDATA_BLOCKVERSIONLOW       (EDDP_PRM_BLOCK_VERSION_LOW)

/* Format of UUID: XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX */
typedef struct _EDDP_PRM_IRDATAUUID_TYPE
{
    LSA_UINT32          UUID_part_1;
    LSA_UINT16          UUID_part_2;
    LSA_UINT16          UUID_part_3;
    LSA_UINT16          UUID_part_4;
    LSA_UINT8           UUID_part_5_array[6];
} EDDP_PRM_IRDATAUUID_TYPE;

typedef struct _EDDP_PRM_DELAY_VALUES_TYPE
{
    LSA_UINT32          MaxPortTxDelay;
    LSA_UINT32          MaxPortRxDelay;
    LSA_UINT32          MaxLineRxDelay;     // only with PDIRGlobalData V1.2
    LSA_BOOL            YellowTime_IsPresent;
    LSA_UINT32          YellowTime;         // only with PDIRGlobalData V1.2
} EDDP_PRM_DELAY_VALUES_TYPE;
/* MaxPortTxDelay           :                                                                           */
// Minimum, Maximum MaxPortTxDelay
#define EDDP_PRM_PDIRGLOBALDATA_MAXPORTTXDELAY_MIN      0x00000001
#define EDDP_PRM_PDIRGLOBALDATA_MAXPORTTXDELAY_MAX      0x0000FFFF
// check macro: is MaxPortTxDelay in valid range
#define EDDP_PRM_IS_MAXPORTTXDELAY_IN_VALID_RANGE(MaxPortTxDelay, PortTxDelay) \
            ((MaxPortTxDelay >= PortTxDelay) && (MaxPortTxDelay <= EDDP_PRM_PDIRGLOBALDATA_MAXPORTTXDELAY_MAX))
/* MaxPortRxDelay           :                                                                           */
// Minimum, Maximum MaxPortRxDelay
#define EDDP_PRM_PDIRGLOBALDATA_MAXPORTRXDELAY_MIN      0x00000001
#define EDDP_PRM_PDIRGLOBALDATA_MAXPORTRXDELAY_MAX      0x0000FFFF
// check macro: is MaxPortRxDelay in valid range
#define EDDP_PRM_IS_MAXPORTRXDELAY_IN_VALID_RANGE(MaxPortRxDelay, PortRxDelay) \
            ((MaxPortRxDelay >= PortRxDelay) && (MaxPortRxDelay <= EDDP_PRM_PDIRGLOBALDATA_MAXPORTRXDELAY_MAX))
/* MaxLineRxDelay           :                                                                           */
// Minimum, Maximum MaxLineRxDelay
#define EDDP_PRM_PDIRGLOBALDATA_MAXLINERXDELAY_MIN      0x00000000
#define EDDP_PRM_PDIRGLOBALDATA_MAXLINERXDELAY_MAX      0x3B9AC9FF
// check macro: is MaxLineRxDelay in valid range
//check for MIN is obsolete (UINT)
#define EDDP_PRM_IS_MAXLINERXDELAY_IN_VALID_RANGE(MaxLineRxDelay) \
            (/*(MaxLineRxDelay >= EDDP_PRM_PDIRGLOBALDATA_MAXLINERXDELAY_MIN) && */(MaxLineRxDelay <= EDDP_PRM_PDIRGLOBALDATA_MAXLINERXDELAY_MAX))
/* YellowTime_IsPresent     : LSA_FALSE     if PDIRGlobalData V1.1                                      */
/*                            LSA_TRUE      if PDIRGlobalData V1.2                                      */
/* YellowTime               : for Fragmentation                                                         */
#define EDDP_PRM_PDIRGLOBALDATA_YELLOWTIME_MIN_PNIP_REV1        10720   // in ns
#define EDDP_PRM_PDIRGLOBALDATA_YELLOWTIME_MIN_PNIP_REV2        10240   // in ns
#define EDDP_PRM_PDIRGLOBALDATA_YELLOWTIME_MAX                  125000  // in ns (default value if fragmentation is OFF)


typedef struct _EDDP_PRM_PDIRGLOBALDATA_TYPE
{
    LSA_UINT8                       BlockVersionLow;
    EDDP_PRM_IRDATAUUID_TYPE        IRDataUUID;
    LSA_UINT32                      MaxBridgeDelay;
    LSA_UINT32                      NumberOfPorts;
    EDDP_PRM_DELAY_VALUES_TYPE      DelayValues[EDDP_MAX_PORT_CNT];
} EDDP_PRM_PDIRGLOBALDATA_TYPE;
/* MaxBridgeDelay           :                                                                           */
// Minimum, Maximum MaxBridgeDelay
#define EDDP_PRM_PDIRGLOBALDATA_MAXBRIDGEDELAY_MIN      0x00000001
#define EDDP_PRM_PDIRGLOBALDATA_MAXBRIDGEDELAY_MAX      0x0000FFFF
// check macro: is MaxBridgeDelay in valid range
#define EDDP_PRM_IS_MAXBRIDGEDELAY_IN_VALID_RANGE(MaxBridgeDelay) \
            ( (MaxBridgeDelay >= EDDP_PRM_PDIRGLOBALDATA_MAXBRIDGEDELAY_MIN) && (MaxBridgeDelay <= EDDP_PRM_PDIRGLOBALDATA_MAXBRIDGEDELAY_MAX) )


/* -------------------------------------------------------------------------------- */
/* Defines for min. MaxBridgeDelay in ns                                            */
/*      Forwarding Mode     : Standard, FastForwarding                              */
/*      Preamble            : long, short                                           */
/* -------------------------------------------------------------------------------- */
typedef struct _EDDP_PRM_CFG_PARAM_MIN_MAXBRIDGEDELAY_TYPE
{
    // AbsoluteTime-Mode
    LSA_UINT32      AbsoluteStandardLong;
    LSA_UINT32      AbsoluteStandardShort;
    LSA_UINT32      AbsoluteFFWLong;
    LSA_UINT32      AbsoluteFFWShort;
    // RelativeTime-Mode
    LSA_UINT32      RelativeStandardLong;
    LSA_UINT32      RelativeStandardShort;
    LSA_UINT32      RelativeFFWLong;
    LSA_UINT32      RelativeFFWShort;
} EDDP_PRM_CFG_PARAM_MIN_MAXBRIDGEDELAY_TYPE;
/* -------------------------------------------------------------------------------- */
/* MaxBridgeDelay - ERTEC200+ - FPGA                                                */
/* -------------------------------------------------------------------------------- */
// AbsoluteTime-Mode
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_STANDARD_LONG_FPGA             2144
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_STANDARD_SHORT_FPGA            1664
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_FFW_LONG_FPGA                  1024
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_FFW_SHORT_FPGA                 544
// RelativeTime-Mode = AbsoluteTime-Mode + Compensation
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_STANDARD_LONG_FPGA             \
            (EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_STANDARD_LONG_FPGA        + EDDP_SWI_IRT_DELAYTIME_COMPENSATION)
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_STANDARD_SHORT_FPGA            \
            (EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_STANDARD_SHORT_FPGA       + EDDP_SWI_IRT_DELAYTIME_COMPENSATION)
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_FFW_LONG_FPGA                  \
            (EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_FFW_LONG_FPGA             + EDDP_SWI_IRT_DELAYTIME_COMPENSATION)
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_FFW_SHORT_FPGA                 \
            (EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_FFW_SHORT_FPGA            + EDDP_SWI_IRT_DELAYTIME_COMPENSATION)
/* -------------------------------------------------------------------------------- */
/* MaxBridgeDelay - ERTEC200+ - ASIC                                                */
/* -------------------------------------------------------------------------------- */
// AbsoluteTime-Mode
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_STANDARD_LONG_ERTEC200P        2088
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_STANDARD_SHORT_ERTEC200P       1608
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_FFW_LONG_ERTEC200P             1024
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_FFW_SHORT_ERTEC200P            544
// RelativeTime-Mode = AbsoluteTime-Mode + Compensation
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_STANDARD_LONG_ERTEC200P        \
            (EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_STANDARD_LONG_ERTEC200P   + EDDP_SWI_IRT_DELAYTIME_COMPENSATION)
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_STANDARD_SHORT_ERTEC200P       \
            (EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_STANDARD_SHORT_ERTEC200P  + EDDP_SWI_IRT_DELAYTIME_COMPENSATION)
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_FFW_LONG_ERTEC200P             \
            (EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_FFW_LONG_ERTEC200P        + EDDP_SWI_IRT_DELAYTIME_COMPENSATION)
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_FFW_SHORT_ERTEC200P            \
            (EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_FFW_SHORT_ERTEC200P       + EDDP_SWI_IRT_DELAYTIME_COMPENSATION)

#define EDDP_PRM_MAXBRIDGEDELAY_ABSOLUTE_STANDARD_LONG_ERTEC200P_SPEC       (1944 + 480)
/* -------------------------------------------------------------------------------- */
/* MaxBridgeDelay - HERA - FPGA                                                     */
/* -------------------------------------------------------------------------------- */
// AbsoluteTime-Mode
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_STANDARD_LONG_FPGA_HERA        2176
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_STANDARD_SHORT_FPGA_HERA       1696
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_FFW_LONG_FPGA_HERA             1048
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_FFW_SHORT_FPGA_HERA            568
// RelativeTime-Mode = AbsoluteTime-Mode + Compensation
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_STANDARD_LONG_FPGA_HERA        EDDP_PRM_INVALID_VALUE_32 /* currently not supported */
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_STANDARD_SHORT_FPGA_HERA       EDDP_PRM_INVALID_VALUE_32 /* currently not supported */
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_FFW_LONG_FPGA_HERA             EDDP_PRM_INVALID_VALUE_32 /* currently not supported */
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_FFW_SHORT_FPGA_HERA            EDDP_PRM_INVALID_VALUE_32 /* currently not supported */
/* HERADevMark - when asic values are present ... */
#if 0
/* -------------------------------------------------------------------------------- */
/* MaxBridgeDelay - HERA - ASIC                                                     */
/* -------------------------------------------------------------------------------- */
// AbsoluteTime-Mode
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_STANDARD_LONG_HERA             2104
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_STANDARD_SHORT_HERA            1624
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_FFW_LONG_HERA                  1024
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_FFW_SHORT_HERA                 544
// RelativeTime-Mode = AbsoluteTime-Mode + Compensation
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_STANDARD_LONG_HERA             EDDP_PRM_INVALID_VALUE_32 /* currently not supported */
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_STANDARD_SHORT_HERA            EDDP_PRM_INVALID_VALUE_32 /* currently not supported */
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_FFW_LONG_HERA                  EDDP_PRM_INVALID_VALUE_32 /* currently not supported */
#define EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_FFW_SHORT_HERA                 EDDP_PRM_INVALID_VALUE_32 /* currently not supported */
#endif
/* -------------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------------- */
/* MRT Diagnosis Management for one MRPD-Consumer from PDIRFrameData                */
/* -------------------------------------------------------------------------------- */
typedef struct _EDDP_PRM_PDIRFRAMEDATA_MRT_DIAG_TYPE
{
    EDDP_PRM_CHAIN_TYPE     PendingGroupLink;
    LSA_UINT16              Flags;
} EDDP_PRM_PDIRFRAMEDATA_MRT_DIAG_TYPE;
/* PendingGroupLink     : chain element in MrtDiagPendingGroup list                                                         */
/* Flags                : MrtDiag-Flags                                                                                     */
/*                          -> MRPD_WATCHDOG_ACTIVE - MRT diagnosis is (0:OFF / 1:ON)                                       */
#define EDDP_PRM_PDIRFRAMEDATA_MRT_FLAGS_MRPD_WATCHDOG_ACTIVE               0x0001
/*                          -> LAST_FIRED_DIAG (0:DisAppear / 1:Appear)                                                     */
#define EDDP_PRM_PDIRFRAMEDATA_MRT_FLAGS_LAST_FIRED_DIAG                    0x0002
/*                          -> INDICATION_IS_PENDING (0:notPending / 1:Pending)                                             */
#define EDDP_PRM_PDIRFRAMEDATA_MRT_FLAGS_INDICATION_IS_PENDING              0x0004
/*                          -> LAST_REQUESTED_DIAG_BY_PENDING (0:DisAppear / 1:Appear)                                      */
/*                          -> if no PrmIndRessource was present (evaluate only if Bit "INDICATION_IS_PENDING" is set)      */
#define EDDP_PRM_PDIRFRAMEDATA_MRT_FLAGS_LAST_REQUESTED_DIAG_BY_PENDING     0x0008

// Management structure for MrtDiagPendingGroup - list of all PDIRFrameData entries that have MrtDiagPending
typedef struct _EDDP_PRM_PDIRFRAMEDATA_MRT_DIAG_PENDING_GROUP_TYPE      EDDP_LOCAL_MEM_ATTR *   EDDP_PRM_PDIRFRAMEDATA_MRT_DIAG_PENDING_GROUP_PTR_TYPE;
typedef struct _EDDP_PRM_PDIRFRAMEDATA_MRT_DIAG_PENDING_GROUP_TYPE
{
    EDDP_PRM_CHAIN_BASE_TYPE        List;
    LSA_UINT32                      PDIRFrameDataCnt;
} EDDP_PRM_PDIRFRAMEDATA_MRT_DIAG_PENDING_GROUP_TYPE;
/* List                 : beginn/end of MrtDiagPendingList                          */
/* PDIRFrameDataCnt     : number of PDIRFrameData entries in this list              */
/* -------------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------------- */
/* LateError Diagnosis Management                                                   */
/* -------------------------------------------------------------------------------- */
typedef struct _EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_LATE_ERR_DIAG_TYPE
{
    EDDP_PRM_CHAIN_TYPE     PendingGroupLink;
    LSA_UINT16              Flags;
} EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_LATE_ERR_DIAG_TYPE;
/* PendingGroupLink     : chain element in LateErrDiagPendingGroup list                                                     */
/* Flags                : LateErrDiag-Flags                                                                                 */
/*                          -> ACTIVE - LateError diagnosis is (0:OFF / 1:ON)                                               */
//#define EDDP_PRM_PDIRFRAMEDATA_LATE_ERR_FLAGS_DIAG_ACTIVE                           0x0001
/*                          -> LAST_FIRED_DIAG (0:DisAppear / 1:Appear)                                                     */
#define EDDP_PRM_PDIRFRAMEDATA_LATE_ERR_FLAGS_LAST_FIRED_DIAG                       0x0002
/*                          -> INDICATION_IS_PENDING (0:notPending / 1:Pending)                                             */
#define EDDP_PRM_PDIRFRAMEDATA_LATE_ERR_FLAGS_INDICATION_IS_PENDING                 0x0004
/*                          -> LAST_REQUESTED_DIAG_BY_PENDING (0:DisAppear / 1:Appear)                                      */
/*                          -> if no PrmIndRessource was present (evaluate only if Bit "INDICATION_IS_PENDING" is set)      */
#define EDDP_PRM_PDIRFRAMEDATA_LATE_ERR_FLAGS_LAST_REQUESTED_DIAG_BY_PENDING        0x0008

// Management structure for LateErrDiagPendingGroup - list of all InBound pack groups that have LateErrDiagPending
typedef struct _EDDP_PRM_PDIRFRAMEDATA_LATE_ERR_DIAG_PENDING_GROUP_TYPE     EDDP_LOCAL_MEM_ATTR *   EDDP_PRM_PDIRFRAMEDATA_LATE_ERR_DIAG_PENDING_GROUP_PTR_TYPE;
typedef struct _EDDP_PRM_PDIRFRAMEDATA_LATE_ERR_DIAG_PENDING_GROUP_TYPE
{
    EDDP_PRM_CHAIN_BASE_TYPE        List;
    LSA_UINT32                      PackGrpCnt;
} EDDP_PRM_PDIRFRAMEDATA_LATE_ERR_DIAG_PENDING_GROUP_TYPE;
/* List                 : beginn/end of LateErrDiagPendingGroupList                 */
/* PackGrpCnt           : number of InBound pack groups in this list                */
/* -------------------------------------------------------------------------------- */


typedef enum _EDDP_PRM_PDIRFRAMEDATA_SND_RCV_ROLE_TYPE
{
    EDDP_PRM_PDIRFRAMEDATA_ROLE_INVALID             = 0,
    EDDP_PRM_PDIRFRAMEDATA_ROLE_PROVIDER            = 1,
    EDDP_PRM_PDIRFRAMEDATA_ROLE_CONSUMER            = 2,
    EDDP_PRM_PDIRFRAMEDATA_ROLE_FORWARDER           = 3,
    EDDP_PRM_PDIRFRAMEDATA_ROLE_CONSUMER_FORWARDER  = 4
} EDDP_PRM_PDIRFRAMEDATA_SND_RCV_ROLE_TYPE;

typedef struct _EDDP_PRM_PDIRFRAMEDATA_TYPE     EDDP_LOCAL_MEM_ATTR *   EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE;
typedef struct _EDDP_PRM_PDIRFRAMEDATA_TYPE
{
    LSA_UINT32                                                      FrameSendOffset;
    LSA_UINT16                                                      DataLength;
    LSA_UINT16                                                      ReductionRatio;
    LSA_UINT16                                                      Phase;
    LSA_UINT16                                                      FrameID;
    LSA_UINT16                                                      EtherType;
    LSA_UINT8                                                       RxPort;
    LSA_UINT8                                                       FrameDetails;
    LSA_UINT8                                                       NumberOfTxPortGroups;
    LSA_UINT8                                                       TxPortGroupArray;
    LSA_UINT16                                                      TxPort;
    EDDP_PRM_PDIRFRAMEDATA_SND_RCV_ROLE_TYPE                        SndRcvRole;
    LSA_BOOL                                                        isDoubleFrameID;
    LSA_UINT16                                                      SubframeBlockIdx;
    EDDP_PRM_PDIRFRAMEDATA_MRT_DIAG_TYPE                            MrtDiag;
    EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_LATE_ERR_DIAG_TYPE      LateErrDiag;
    LSA_UINT32                                                      OffsetFrameID;
    LSA_UINT32                                                      OffsetFrameSendOffset;
} EDDP_PRM_PDIRFRAMEDATA_TYPE;
/* FrameSendOffset          :                                                                           */
// Minimum, Maximum FrameSendOffset
#define EDDP_PRM_PDIRFRAMEDATA_FRAMESENDOFFSET_MIN_ERTEC200P_PNIP_REV1      3360
#define EDDP_PRM_PDIRFRAMEDATA_FRAMESENDOFFSET_MIN_ERTEC200P_PNIP_REV2      1760
#define EDDP_PRM_PDIRFRAMEDATA_FRAMESENDOFFSET_MIN_FPGA_PNIP_REV1           4480
#define EDDP_PRM_PDIRFRAMEDATA_FRAMESENDOFFSET_MIN_FPGA_PNIP_REV2           4480
#define EDDP_PRM_PDIRFRAMEDATA_FRAMESENDOFFSET_MIN_FPGA_PNIP_REV3           5000
#define EDDP_PRM_PDIRFRAMEDATA_FRAMESENDOFFSET_SHORTENED_VALUE              480         // if a short preamble is used (MinFSO - 480)
#define EDDP_PRM_PDIRFRAMEDATA_FRAMESENDOFFSET_MAX                          0x003B28FF  // 3.877.119 ns
/* DataLength               :                                                                           */
// Minimum, Maximum DataLength
#define EDDP_PRM_PDIRFRAMEDATA_DATALENGTH_MIN               (EDD_CSRT_DATALEN_MIN)
#define EDDP_PRM_PDIRFRAMEDATA_DATALENGTH_MAX               (EDD_CSRT_DATALEN_MAX)
// check macro: is DataLength in valid range
#define EDDP_PRM_IS_DATALENGTH_IN_VALID_RANGE(DataLength) \
            ( (DataLength >= EDDP_PRM_PDIRFRAMEDATA_DATALENGTH_MIN) && (DataLength <= EDDP_PRM_PDIRFRAMEDATA_DATALENGTH_MAX) )
/* ReductionRatio                                                                                       */
// check macro: is ReductionRatio in valid range
#define EDDP_PRM_IS_REDUCTIONRATIO_RTC3_IN_VALID_RANGE(ReductionRatio) \
            ( (ReductionRatio >= EDD_CSRT_CYCLE_REDUCTION_RATIO_RTC3_MIN) && (ReductionRatio <= EDD_CSRT_CYCLE_REDUCTION_RATIO_RTC3_MAX) )
/* FrameDetails             : FrameDetails.SyncFrame                                                    */
#define EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_SYNCFRAME_MASK                          0x03    // 1:0
#define EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_SYNCFRAME_SHIFT                         0
//                              : value
#define EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_SYNCFRAME_NO_SYNC                       0x0
#define EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_SYNCFRAME_PRIMARY_SYNC                  0x1
#define EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_SYNCFRAME_SECONDARY_SYNC                0x2
/*                          : FrameDetails.MeaningFrameSendOffset                                       */
#define EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_MEANINGFRAMESENDOFFSET_MASK             0x0C    // 3:2
#define EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_MEANINGFRAMESENDOFFSET_SHIFT            2
//                              : value
#define EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_MEANINGFRAMESENDOFFSET_POINT_OF_TIME    0x0
#define EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_MEANINGFRAMESENDOFFSET_START_OF_RED     0x1
#define EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_MEANINGFRAMESENDOFFSET_END_OF_RED       0x2
/*                          : FrameDetails.MediaRedundancyWatchDog                                      */
#define EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_MEDIAREDUNDANCYWATCHDOG_MASK            0x80    // 7
#define EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_MEDIAREDUNDANCYWATCHDOG_SHIFT           7
//                              : value
#define EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_MEDIAREDUNDANCYWATCHDOG_MRT_OFF         0x0
#define EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_MEDIAREDUNDANCYWATCHDOG_MRT_ON          0x1
/* NumberOfTxPortGroups     : must be 1                                                                 */
/* TxPortGroupArray         : EDDP supports only 4 ports                                                */
/* TxPort                   : TxPortID from TxPortGroupArray (only for DFP)                             */
/* SndRcvRole               : Provider / Consumer / Forwarder / Consumer+Forwarder                      */
/* isDoubleFrameID          : this FrameID is double in PDIRFrameData (only for DFP allowed)            */
/* SubframeBlockIdx         : this PDIRFrameData is a DFP frame and has an entry in PDIRSubframeData    */
/* MrtDiag                  : MRT diagnosis management for MRPD-Consumer                                */
/* LateErrDiag              : LateError diagnosis management for InBound pack group                     */


typedef struct _EDDP_PRM_PDIRBEGINENDDATA_ASSIGNMENTS_TYPE
{
    LSA_UINT32          TxRedOrangePeriodBegin;
    LSA_UINT32          TxOrangePeriodBegin;
    LSA_UINT32          TxGreenPeriodBegin;
    LSA_UINT32          RxRedOrangePeriodBegin;
    LSA_UINT32          RxOrangePeriodBegin;
    LSA_UINT32          RxGreenPeriodBegin;
} EDDP_PRM_PDIRBEGINENDDATA_ASSIGNMENTS_TYPE;
// Minimum, Maximum PeriodBegin
#define EDDP_PRM_PDIRBEGINENDDATA_PERIODBEGIN_MIN       0x00000000
#define EDDP_PRM_PDIRBEGINENDDATA_PERIODBEGIN_MAX       0x003D08FF
//check for MIN is obsolete (UINT)
// check macro: is PeriodBegin in valid range
#define EDDP_PRM_IS_PERIODBEGIN_IN_VALID_RANGE(PeriodBegin) \
            (/* (PeriodBegin >= EDDP_PRM_PDIRBEGINENDDATA_PERIODBEGIN_MIN) && */(PeriodBegin <= EDDP_PRM_PDIRBEGINENDDATA_PERIODBEGIN_MAX) )

typedef struct _EDDP_PRM_PHASE_ASSIGNMENT_TYPE
{
    LSA_UINT16          TxPhaseAssignment;
    LSA_UINT16          RxPhaseAssignment;
} EDDP_PRM_PHASE_ASSIGNMENT_TYPE;

typedef struct _EDDP_PRM_PDIRBEGINENDDATA_PORT_TYPE
{
    LSA_UINT32                                  NumberOfAssignments;
    EDDP_PRM_PDIRBEGINENDDATA_ASSIGNMENTS_TYPE  Assignments[EDDP_PRM_MAX_ASSIGNMENTS];
    LSA_UINT32                                  NumberOfPhases;
    EDDP_PRM_PHASE_ASSIGNMENT_TYPE              PhaseAssignmentArray[EDDP_PRM_MAX_PHASES];
    LSA_BOOL                                    isRedPort;
} EDDP_PRM_PDIRBEGINENDDATA_PORT_TYPE;
/* NumberOfAssignments      :                                                                           */
// Minimum, Maximum NumberOfAssignments
#define EDDP_PRM_PDIRBEGINENDDATA_NUMBEROFASSIGNMENTS_MIN       1
#define EDDP_PRM_PDIRBEGINENDDATA_NUMBEROFASSIGNMENTS_MAX       (EDDP_PRM_MAX_ASSIGNMENTS)
// check macro: is NumberOfAssignments in valid range
#define EDDP_PRM_IS_NUMBEROFASSIGNMENTS_IN_VALID_RANGE(NumberOfAssignments) \
            ( (NumberOfAssignments >= EDDP_PRM_PDIRBEGINENDDATA_NUMBEROFASSIGNMENTS_MIN) && (NumberOfAssignments <= EDDP_PRM_PDIRBEGINENDDATA_NUMBEROFASSIGNMENTS_MAX) )
/* NumberOfPhases           :                                                                           */
// Minimum, Maximum NumberOfPhases
#define EDDP_PRM_PDIRBEGINENDDATA_NUMBEROFPHASES_MIN            1
#define EDDP_PRM_PDIRBEGINENDDATA_NUMBEROFPHASES_MAX            (EDDP_PRM_MAX_PHASES)
// check macro: is NumberOfPhases in valid range
#define EDDP_PRM_IS_NUMBEROFPHASES_IN_VALID_RANGE(NumberOfPhases) \
            ( (NumberOfPhases >= EDDP_PRM_PDIRBEGINENDDATA_NUMBEROFPHASES_MIN) && (NumberOfPhases <= EDDP_PRM_PDIRBEGINENDDATA_NUMBEROFPHASES_MAX) )
/* isRedPort                : this port has red phase                                                   */

typedef struct _EDDP_PRM_PDIRBEGINENDDATA_TYPE
{
    LSA_UINT16                              StartOfRedFrameID;
    LSA_UINT16                              EndOfRedFrameID; 
    LSA_UINT32                              NumberOfPorts;
    LSA_UINT32                              NumberOfPhases_max;
    EDDP_PRM_PDIRBEGINENDDATA_PORT_TYPE     BeginEndDataArray[EDDP_MAX_PORT_CNT];
} EDDP_PRM_PDIRBEGINENDDATA_TYPE;

typedef struct _EDDP_PRM_LAST_LOCAL_IRT_FRAME_PORT_TYPE
{
    LSA_UINT32      RxEndLocalNs;
    LSA_UINT32      RxLastLocalFrameLen;
    LSA_UINT32      TxEndLocalNs;
    LSA_UINT32      TxLastLocalFrameLen;
} EDDP_PRM_LAST_LOCAL_IRT_FRAME_PORT_TYPE;

/* for EDD_SRV_GET_PARAMS, TransferEnd(ApplicationTimer) and PrmCommit pParam */
typedef struct _EDDP_PRM_LOCAL_TRANSFER_START_END_TYPE
{
    /* Transfer-Start-End values */
    LSA_UINT32      RxLocalTransferEndNs;
    LSA_INT32       TxLocalTransferStartNs;
    LSA_UINT32      TxLocalTransferEndNs;
    /* Transfer-Start-End intermidiate values */
    LSA_UINT32      RxMaxFSOWithFrameLengthNs;
    LSA_UINT32      TxMinFSONs;
    LSA_UINT32      TxMaxFSOWithFrameLengthNs;
} EDDP_PRM_LOCAL_TRANSFER_START_END_TYPE;
/* RxLocalTransferEndNs         : realy TransferEnd for Consumer                                                                                                        */
/* TxLocalTransferStartNs       : realy TransferStart for Provider                                                                                                      */
/* TxLocalTransferEndNs         : realy TransferEnd for Provider                                                                                                        */
/* RxMaxFSOWithFrameLengthNs    : Time in ns of last locally received red frame (consumer) with frame length - intermidiate value to calculate RxLocalTransferEndNs     */
/* TxMinFSONs                   : FSO in ns of first locally send red frame (provider) - intermidiate value to calculate TxLocalTransferStartNs                         */
/* TxMaxFSOWithFrameLengthNs    : Maximum transfer end time (worst case) in ns with frame length of last locally send red frame (provider)                              */
/*                                  -> intermidiate value to calculate TxLocalTransferEndNs                                                                             */

// values for EndOfRedPhase (interface) of all ports
typedef struct _EDDP_PRM_END_OF_RED_PHASE_IF_TYPE
{
    LSA_UINT32      RxNs;       // for all local received frames
    LSA_UINT32      TxNs;       // for all local sent frames
    LSA_UINT32      RxTxNs;     // for all local received and sent frames
} EDDP_PRM_END_OF_RED_PHASE_IF_TYPE;


typedef struct _EDDP_PRM_PDIRDATA_TYPE  EDDP_LOCAL_MEM_ATTR *   EDDP_PRM_PDIRDATA_PTR_TYPE;
typedef struct _EDDP_PRM_PDIRDATA_TYPE
{
    LSA_UINT16                                      SlotNumber;
    LSA_UINT16                                      SubslotNumber;

    EDDP_PRM_PDIRGLOBALDATA_TYPE                    PDIRGlobalData;

    LSA_BOOL                                        AbsoluteTimeMode;
    LSA_BOOL                                        FrameDataProperties_IsPresent;
    LSA_UINT32                                      FrameDataProperties;

    EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE                 pIRFrameDataArray;

    EDDP_PRM_LAST_LOCAL_IRT_FRAME_PORT_TYPE         LastLocalIRTFrameArray[EDDP_MAX_PORT_CNT];
    EDDP_PRM_LOCAL_TRANSFER_START_END_TYPE          LocalTransferStartEnd;
    EDDP_PRM_END_OF_RED_PHASE_IF_TYPE               EndOfRedPhaseIFPorts;

    LSA_BOOL                                        Sync_IsPresent;
    LSA_BOOL                                        MRPDConsumerWithOddFrameID_IsPresent;
    LSA_UINT32                                      IRFrameDataArray_Max_Entries;
    LSA_UINT32                                      IRFrameDataArray_Count_All;
    LSA_UINT32                                      IRFrameDataArray_Count_Used;    
    LSA_UINT16                                      FrameID_low;
    LSA_UINT16                                      FrameID_high;

    EDDP_PRM_PDIRBEGINENDDATA_TYPE                  PDIRBeginEndData;
} EDDP_PRM_PDIRDATA_TYPE;
/* AbsoluteTimeMode             : using only during the prm phase (PrmWrite, PrmEnd, PrmCommit)                     */
/*                                after the prm phase use the parameter SWI.IRTtopFwMode                            */
/*                                  - LSA_TRUE  : AbsoluteTime-Mode                                                 */
/*                                  - LSA_FALSE : RelativeTime-Mode                                                 */
/* FrameDataProperties_IsPresent: PDIRFrameData V1.0 field "FrameDataProperties" is not present                     */
/*                                PDIRFrameData V1.1 field "FrameDataProperties" is present                         */
/*                                  -> evalute only if PDIRData is present                                          */
/* FrameDataProperties          : FrameDataProperties.ForwardingMode (AbsoluteTime/RelativeTime-Mode)               */
#define EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FORWARDINGMODE_MASK                       0x01    // 0
#define EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FORWARDINGMODE_SHIFT                      0
//                              : value
#define EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FORWARDINGMODE_ABSOLUTE                   0x0
#define EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FORWARDINGMODE_RELATIVE                   0x1
/* FrameDataProperties          : FrameDataProperties.FastForwardingMulticastMACAdd                                 */
#define EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_MASK        0x06    // 2:1
#define EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_SHIFT       1
//                              : value
#define EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_LEGACY      0x0
#define EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_RTC3        0x1
#define EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_FFW         0x2
/* FrameDataProperties          : FrameDataProperties.FragmentationMode                                             */
#define EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_MASK                    0x18    // 4:3
#define EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_SHIFT                   3
//                              : value
#define EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_OFF                     0x0
#define EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_ON_FRAG_SIZE_128        0x1
#define EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_ON_FRAG_SIZE_256        0x2
/* ---------------------------------------------------------------------------- */
// values for MinFragTime
// ERTEC200P
#define EDDP_PRM_PDIRDATA_MIN_FRAG_TIME_ERTEC200P_PNIP_REV1         11920
#define EDDP_PRM_PDIRDATA_MIN_FRAG_TIME_ERTEC200P_PNIP_REV2         11440
// FPGA
#define EDDP_PRM_PDIRDATA_MIN_FRAG_TIME_FPGA_PNIP_REV1              20160
#define EDDP_PRM_PDIRDATA_MIN_FRAG_TIME_FPGA_PNIP_REV2              19680
#define EDDP_PRM_PDIRDATA_MIN_FRAG_TIME_FPGA_PNIP_REV3              14480
/* ---------------------------------------------------------------------------- */
/* Sync_IsPresent               : If the Sync-FrameID(0x0080) is present                                            */
/* MRPDConsumerWithOddFrameID_IsPresent: at least one MRPD consumer with odd FrameID(x+1) is present                */
/* IRFrameDataArray_Count_All   : Number of all IRT entries from PDIRFrameData record                               */
/* IRFrameDataArray_Count_Used  : Number of IRT entries counted within FrameDataArray of PDIRFrameData              */
/* FrameID_low, FrameID_high    : FrameID range only from PDIRFrameData                                             */


/* ---------------------------------------------------------------------------- */
/*  PDIRSubframeData                                                   bytes    */
/* ---------------------------------------------------------------------------- */
/*      BlockHeader                                                     6       */
/*      NumberOfSubframeBlocks                                          2       */
/*      (                                                                       */
/*          BlockHeader (SubframeBlock)                                 6       */
/*          FrameID                                                     2       */
/*          SFIOCRProperties                                            4       */
/*          (                                                                   */
/*              SubframeData                                            4       */
/*          )*                                                                  */
/*      )*                                                                      */
/* ---------------------------------------------------------------------------- */
#define EDDP_PRM_PDIRSUBFRAMEDATA_RECORD_INDEX          0x8020

#define EDDP_PRM_PDIRSUBFRAMEDATA_BLOCKTYPE             0x022A
#define EDDP_PRM_PDIRSUBFRAMEDATA_BLOCKVERSIONHIGH      (EDDP_PRM_BLOCK_VERSION_HIGH)
#define EDDP_PRM_PDIRSUBFRAMEDATA_BLOCKVERSIONLOW       (EDDP_PRM_BLOCK_VERSION_LOW)
#define EDDP_PRM_PDIRSUBFRAMEBLOCK_BLOCKTYPE            0x022B
#define EDDP_PRM_PDIRSUBFRAMEBLOCK_BLOCKVERSIONHIGH     (EDDP_PRM_BLOCK_VERSION_HIGH)
#define EDDP_PRM_PDIRSUBFRAMEBLOCK_BLOCKVERSIONLOW      (EDDP_PRM_BLOCK_VERSION_LOW)

// max. number of InBound pack frames
#define EDDP_PRM_PDIRSUBFRAMEDATA_MAX_INBOUND_PACK_FRAMES       8
// max. number of OutBound pack frames
#define EDDP_PRM_PDIRSUBFRAMEDATA_MAX_OUTBOUND_PACK_FRAMES      8

// max. number of InBound DataGramms
#define EDDP_PRM_PDIRSUBFRAMEDATA_MAX_INBOUND_DATA_GRAMM        64
// max. number of OutBound DataGramms
#define EDDP_PRM_PDIRSUBFRAMEDATA_MAX_OUTBOUND_DATA_GRAMM       127

// max. number of pack groups (SubframeBlocks) within one PDIRSubframeData record (InBound + OutBound)
#define EDDP_PRM_PDIRSUBFRAMEDATA_MAX_SUBFRAMEBLOCK \
            (EDDP_PRM_PDIRSUBFRAMEDATA_MAX_INBOUND_PACK_FRAMES + EDDP_PRM_PDIRSUBFRAMEDATA_MAX_OUTBOUND_PACK_FRAMES)

// max. number of DataGramms (SubframeData) for every FrameID
//  InBound     : 64 (the array "SubframeData" is limited for InBound)
//  OutBound    : 127
#define EDDP_PRM_PDIRSUBFRAMEDATA_MAX_SUBFRAMEBLOCK_SUBFRAMEDATA        (EDDP_PRM_PDIRSUBFRAMEDATA_MAX_INBOUND_DATA_GRAMM)

// max. sizeof(PDIRSubframeData record)
// BlockHeader(6) + NumberOfSubframeBlocks(2) + 26*SubframeBlock(6+2+4+(64*SubframeData(4)))
#define EDDP_PRM_MAX_SIZE_OF_PDIRSUBFRAMEDATA_RECORD                                                                                        \
            (   6                                                                                                                           \
              + 2                                                                                                                           \
              + (EDDP_PRM_PDIRSUBFRAMEDATA_MAX_SUBFRAMEBLOCK * (6 + 2 + 4 + (EDDP_PRM_PDIRSUBFRAMEDATA_MAX_SUBFRAMEBLOCK_SUBFRAMEDATA*4)))  \
            )

// length of (SubframeHeader + SFCRC16) in one subframe: SFPosition(1) + SFDataLength(1) + SFCycleCounter(1) + DataStatusSubframe(1) + SFCRC16(2)
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEHEADER_AND_SFCRC16_LENGTH         6

// length of (SFHeaderCRC16(2) + SFEndDelimiter(2)) in one pack frame
#define EDDP_PRM_PDIRSUBFRAMEDATA_SFHEADERCRC16_SFENDDELIMITER_LENGTH       4


/* ------------------------------------------------------------ */
/* PDIRSubframeData.SubframeBlock.SFIOCRProperties              */
/* ------------------------------------------------------------ */
typedef struct _EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_TYPE
{
    LSA_UINT16          DistributedWatchDogFactor;
    LSA_UINT16          RestartFactorForDistributedWD;
    LSA_UINT16          DFPmode;
    LSA_UINT8           DFPDirection;
    LSA_UINT8           DFPRedundantPathLayout;
    LSA_UINT8           SFCRC16;
} EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_TYPE;
/* DistributedWatchDogFactor    :                                                                                           */
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DIST_WD_FACTOR_MASK            0x000000FF  // 7:0
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DIST_WD_FACTOR_SHIFT           0
//                              : value
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DIST_WD_FACTOR_OFF             0x0
/*      Minimum, Maximum DistributedWatchDogFactor in record PDIRSubframeData                                               */
#define EDDP_PRM_PDIRSUBFRAMEDATA_MIN_DISTRIBUTEDWATCHDOGFACTOR     3
#define EDDP_PRM_PDIRSUBFRAMEDATA_MAX_DISTRIBUTEDWATCHDOGFACTOR     31
/*      Maximum DistributedWatchDogFactor in PNIP                                                                  */
#define EDDP_PRM_PNIP_MAX_DISTRIBUTEDWATCHDOGFACTOR                 127
// check macro: is DistributedWatchDogFactor in valid range ?
#define EDDP_PRM_IS_DFP_DISTRIBUTEDWATCHDOGFACTOR_IN_VALID_RANGE(DistributedWatchDogFactor)                 \
            (   (DistributedWatchDogFactor >= EDDP_PRM_PDIRSUBFRAMEDATA_MIN_DISTRIBUTEDWATCHDOGFACTOR)      \
             && (DistributedWatchDogFactor <= EDDP_PRM_PDIRSUBFRAMEDATA_MAX_DISTRIBUTEDWATCHDOGFACTOR)      \
            )
/* RestartFactorForDistributedWD:                                                                                           */
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_REST_FACTOR_DIST_WD_MASK       0x0000FF00  // 15:8
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_REST_FACTOR_DIST_WD_SHIFT      8
//                              : value
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_REST_FACTOR_DIST_WD_OFF        0x0
/*      Minimum, Maximum RestartFactorForDistributedWD in record PDIRSubframeData                                           */
#define EDDP_PRM_PDIRSUBFRAMEDATA_MIN_RESTARTFACTORFORDISTRIBUTEDWD     0
#define EDDP_PRM_PDIRSUBFRAMEDATA_MAX_RESTARTFACTORFORDISTRIBUTEDWD     80
//check for MIN is obsolete (UINT)
// check macro: is RestartFactorForDistributedWD in valid range ?
#define EDDP_PRM_IS_DFP_RESTARTFACTORFORDISTRIBUTEDWD_IN_VALID_RANGE(RestartFactorForDistributedWD)                 \
            (/*   (RestartFactorForDistributedWD >= EDDP_PRM_PDIRSUBFRAMEDATA_MIN_RESTARTFACTORFORDISTRIBUTEDWD) && */     \
                (RestartFactorForDistributedWD <= EDDP_PRM_PDIRSUBFRAMEDATA_MAX_RESTARTFACTORFORDISTRIBUTEDWD)      \
            )
/* DFPmode                      :   0x00    - end node (Controller/MultiDevice)                                             */
/*                                  !0x00   - Subframe position (Device)                                                    */
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPMODE_MASK                   0x00FF0000  // 23:16
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPMODE_SHIFT                  16
//                              : value
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_DFPMODE_END_NODE                                0x0
/* DFPDirection                 :   0x00    - DFP InBound Frame                                                             */
/*                                  0x01    - DFP OutBound Frame                                                            */
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPDIRECTION_MASK              0x20000000  // 29
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPDIRECTION_SHIFT             29
//                              : value
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPDIRECTION_INBOUND           0x00
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPDIRECTION_OUTBOUND          0x01
/* DFPRedundantPathLayout       :                                                                                           */
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_MASK    0x40000000  // 30
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_SHIFT   30
//                              : value
//      HIGH: sending order of ACW_DGs like in SubframeData
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_HIGH    0x0
//      DOWN: inverse sending order of ACW_DGs like in SubframeData
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_DOWN    0x1
/* SFCRC16                      :                                                                                           */
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_SFCRC16_MASK                   0x80000000  // 31
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_SFCRC16_SHIFT                  31
//                              : value
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_SFCRC16_0                      0x0
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_SFCRC16_1                      0x1

/* ------------------------------------------------------------ */
/* PDIRSubframeData.SubframeBlock.SubframeData (one ACW_DG)     */
/* ------------------------------------------------------------ */
typedef struct _EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SUBFRAMEDATA_TYPE   EDDP_LOCAL_MEM_ATTR *   EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SUBFRAMEDATA_PTR_TYPE;
typedef struct _EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SUBFRAMEDATA_TYPE
{
    LSA_UINT16          Position;
    LSA_UINT16          DataLength;
    LSA_UINT32          Offset;
} EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SUBFRAMEDATA_TYPE;
/* Position     : Position of DataGramm             */
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SUBFRAMEDATA_POSITION_MASK              0x0000007F  // 6:0
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SUBFRAMEDATA_POSITION_SHIFT             0
/*      Minimum, Maximum Position in record PDIRSubframeData                                                                */
/*      PositionNr=0 is reserved, so the Position is in range   [1..127] for IO-Controller                                  */
/*                                                              [1..63]  for IO-Device                                      */
#define EDDP_PRM_PDIRSUBFRAMEDATA_MIN_POSITION              1
#define EDDP_PRM_PDIRSUBFRAMEDATA_MAX_POSITION_IOC          (EDDP_PRM_PDIRSUBFRAMEDATA_MAX_OUTBOUND_DATA_GRAMM)
#define EDDP_PRM_PDIRSUBFRAMEDATA_MAX_POSITION_IOD          (EDDP_PRM_PDIRSUBFRAMEDATA_MAX_INBOUND_DATA_GRAMM - 1)
// check macro: is Position in valid range for IOC ?
#define EDDP_PRM_IS_DFP_POSITION_IN_VALID_RANGE_IOC(Position) \
            ( (Position >= EDDP_PRM_PDIRSUBFRAMEDATA_MIN_POSITION) && (Position <= EDDP_PRM_PDIRSUBFRAMEDATA_MAX_POSITION_IOC) )
// check macro: is Position in valid range for IOD ?
#define EDDP_PRM_IS_DFP_POSITION_IN_VALID_RANGE_IOD(Position) \
            ( (Position >= EDDP_PRM_PDIRSUBFRAMEDATA_MIN_POSITION) && (Position <= EDDP_PRM_PDIRSUBFRAMEDATA_MAX_POSITION_IOD) )
/* DataLength   : DataLength of DataGramm           */
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SUBFRAMEDATA_DATALENGTH_MASK            0x0000FF00  // 15:8
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SUBFRAMEDATA_DATALENGTH_SHIFT           8
/*      Minimum, Maximum DataLength in record PDIRSubframeData                                                              */
#define EDDP_PRM_PDIRSUBFRAMEDATA_MIN_DATALENGTH            1
#define EDDP_PRM_PDIRSUBFRAMEDATA_MAX_DATALENGTH            255
// check macro: is DataLength in valid range ?
#define EDDP_PRM_IS_DFP_DATALENGTH_IN_VALID_RANGE(DataLength)                   \
            (   (DataLength >= EDDP_PRM_PDIRSUBFRAMEDATA_MIN_DATALENGTH)        \
             && (DataLength <= EDDP_PRM_PDIRSUBFRAMEDATA_MAX_DATALENGTH)        \
            )
/* Offset       : Offset of the DataGramm data (DG-C_SDU) in the packframe relativ to the beginning of the packframe C_SDU  */
/*                  -> is calculated by EDDP                                                                                */

/* ------------------------------------------------------------ */
/* PDIRSubframeData.SubframeBlock (one pack group)              */
/* ------------------------------------------------------------ */
typedef struct _EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_TYPE    EDDP_LOCAL_MEM_ATTR *   EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_PTR_TYPE;
typedef struct _EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_TYPE
{
    LSA_UINT16                                                      NumberOfSubframeData;
    LSA_UINT16                                                      FrameID;
    EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_TYPE   SFIOCRProperties;
    LSA_UINT16                                                      HwConfig;
    LSA_UINT16                                                      PDIRFrameDataProvIdx;
    LSA_UINT16                                                      PDIRFrameDataConsIdx;
    LSA_UINT16                                                      PDIRFrameDataProvIdxRed1;
    LSA_UINT16                                                      PDIRFrameDataConsIdxRed1;
    EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SUBFRAMEDATA_TYPE       SubframeDataArray[EDDP_PRM_PDIRSUBFRAMEDATA_MAX_SUBFRAMEBLOCK_SUBFRAMEDATA];
} EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_TYPE;
/* NumberOfSubframeData         : Number of SubframeData (ACW_DGs)                                                          */
/*      Minimum, Maximum NumberOfSubframeData in record PDIRSubframeData                                                    */
/*      PositionNr=0 is reserved, so the NumberOfSubframeData is in range[1..63]                                            */
#define EDDP_PRM_PDIRSUBFRAMEDATA_MIN_NUMBER_OF_SUBFRAMEDATA        1
#define EDDP_PRM_PDIRSUBFRAMEDATA_MAX_NUMBER_OF_SUBFRAMEDATA        (EDDP_PRM_PDIRSUBFRAMEDATA_MAX_SUBFRAMEBLOCK_SUBFRAMEDATA - 1)
// check macro: is NumberOfSubframeData in valid range ?
#define EDDP_PRM_IS_DFP_NUMBER_OF_SUBFRAMEDATA_IN_VALID_RANGE(NumberOfSubframeData)                 \
            (   (NumberOfSubframeData >= EDDP_PRM_PDIRSUBFRAMEDATA_MIN_NUMBER_OF_SUBFRAMEDATA)      \
             && (NumberOfSubframeData <= EDDP_PRM_PDIRSUBFRAMEDATA_MAX_NUMBER_OF_SUBFRAMEDATA)      \
            )
/* FrameID                      : FrameID                                                                                   */
/* SFIOCRProperties             : see above                                                                                 */
/* HwConfig                     : bits - what has to be configured in HW for this pack group at PrmCommit ?                 */
/* HwConfig for FrameID(x)                                                                                                  */
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_INBOUND_PACK_MASK          0x0001  // InBound-Pack in Pack-Control
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_INBOUND_PACK_SHIFT         0
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_OUTBOUND_PACK_MASK         0x0002  // OutBound-Pack in Pack-Control
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_OUTBOUND_PACK_SHIFT        1
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_SND_ACW_MASK               0x0004  // Send ACW with ACW_DG in PPM
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_SND_ACW_SHIFT              2
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_RCV_ACW_MASK               0x0008  // Receive ACW with ACW_DG in CPM
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_RCV_ACW_SHIFT              3
/* HwConfig for FrameID(x+1) if redundant                                                                                   */
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_INBOUND_PACK_RED1_MASK     0x0010  // InBound-Pack in Pack-Control for FrameID(x+1)
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_INBOUND_PACK_RED1_SHIFT    4
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_OUTBOUND_PACK_RED1_MASK    0x0020  // OutBound-Pack in Pack-Control for FrameID(x+1)
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_OUTBOUND_PACK_RED1_SHIFT   5
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_SND_ACW_RED1_MASK          0x0040  // Send ACW with ACW_DG in PPM for FrameID(x+1)
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_SND_ACW_RED1_SHIFT         6
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_RCV_ACW_RED1_MASK          0x0080  // Receive ACW with ACW_DG in CPM for FrameID(x+1)
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_RCV_ACW_RED1_SHIFT         7
/* HwConfig other information                                                                                               */
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_SND_ACW_INBOUND_PACK_MASK          0x0100  // Send ACW.InBound_Pack = 1
#define EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_SND_ACW_INBOUND_PACK_SHIFT         8
/* PDIRFrameDataProvIdx         : Index in PDIRFrameData[x] of connected provider                                           */
/* PDIRFrameDataConsIdx         : Index in PDIRFrameData[x] of connected consumer                                           */
/* PDIRFrameDataProvIdxRed1     : only for MRPD - Index in PDIRFrameData[x] of connected provider with FrameID(x+1)         */
/* PDIRFrameDataConsIdxRed1     : only for MRPD - Index in PDIRFrameData[x] of connected consumer with FrameID(x+1)         */
#define EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED                       (EDDP_PRM_INVALID_VALUE_16)
/* SubframeDataArray            : Array of SubframeData (ACW_DGs)                                                           */

/* ------------------------------------------------------------ */
/* PDIRSubframeData                                             */
/* ------------------------------------------------------------ */
typedef struct _EDDP_PRM_PDIRSUBFRAMEDATA_TYPE  EDDP_LOCAL_MEM_ATTR *   EDDP_PRM_PDIRSUBFRAMEDATA_PTR_TYPE;
typedef struct _EDDP_PRM_PDIRSUBFRAMEDATA_TYPE
{
    LSA_UINT16                                          SlotNumber;
    LSA_UINT16                                          SubslotNumber;
    LSA_UINT16                                          NumberOfSubframeBlocks;
    EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_TYPE        SubframeBlockArray[EDDP_PRM_PDIRSUBFRAMEDATA_MAX_SUBFRAMEBLOCK];
} EDDP_PRM_PDIRSUBFRAMEDATA_TYPE;
/* SlotNumber                   :                                                                               */
/* SubslotNumber                :                                                                               */
/* NumberOfSubframeBlocks       : Number of SubframeBlocks (number of pack groups)                              */
/*      Minimum, Maximum NumberOfSubframeBlocks in record PDIRSubframeData  :   range[1..16]                    */
#define EDDP_PRM_PDIRSUBFRAMEDATA_MIN_NUMBER_OF_SUBFRAMEBLOCKS      1
#define EDDP_PRM_PDIRSUBFRAMEDATA_MAX_NUMBER_OF_SUBFRAMEBLOCKS      (EDDP_PRM_PDIRSUBFRAMEDATA_MAX_SUBFRAMEBLOCK)
// check macro: is NumberOfSubframeBlocks in valid range ?
#define EDDP_PRM_IS_DFP_NUMBER_OF_SUBFRAMEBLOCKS_IN_VALID_RANGE(NumberOfSubframeBlocks)                 \
            (   (NumberOfSubframeBlocks >= EDDP_PRM_PDIRSUBFRAMEDATA_MIN_NUMBER_OF_SUBFRAMEBLOCKS)      \
             && (NumberOfSubframeBlocks <= EDDP_PRM_PDIRSUBFRAMEDATA_MAX_NUMBER_OF_SUBFRAMEBLOCKS)      \
            )
/* SubframeBlockArray           : Array of SubframeBlocks                                                       */



typedef struct _EDDP_PRM_RTC3_PORT_INFO_TYPE    EDDP_LOCAL_MEM_ATTR *   EDDP_PRM_RTC3_PORT_INFO_PTR_TYPE;
typedef struct _EDDP_PRM_RTC3_PORT_INFO_TYPE
{
    /* Standard     */
    LSA_UINT16      FrameID;
    LSA_UINT16      ReductionRatio;
    LSA_UINT16      Phase;
    LSA_UINT32      FrameSendOffset;
    /* Multi-Cast   */
    LSA_UINT16      MCFrameID;
    LSA_UINT16      MCReductionRatio;
    LSA_UINT16      MCPhase;
    LSA_UINT32      MCFrameSendOffset;
} EDDP_PRM_RTC3_PORT_INFO_TYPE;
/* FrameID              : FrameID                                                   */
/* ReductionRatio       : ReductionRatio                                            */
/* Phase                : Phase                                                     */
/* FrameSendOffset      : FrameSendOffset                                           */
/* MCFrameID            : Multi-Cast FrameID                                        */
/* MCReductionRatio     : Multi-Cast ReductionRatio                                 */
/* MCPhase              : Multi-Cast Phase                                          */
/* MCFrameSendOffset    : Multi-Cast FrameSendOffset                                */


typedef struct _EDDP_PRM_RTC3_ALL_PORTS_INFO_TYPE   EDDP_LOCAL_MEM_ATTR *   EDDP_PRM_RTC3_ALL_PORTS_INFO_PTR_TYPE;
typedef struct _EDDP_PRM_RTC3_ALL_PORTS_INFO_TYPE
{
    EDDP_PRM_RTC3_PORT_INFO_TYPE        PortInfoArray[EDDP_MAX_PORT_CNT];
} EDDP_PRM_RTC3_ALL_PORTS_INFO_TYPE;


/*---------------------------------------------------------------------------*/
/*   PDNRTFeedInLoadLimitation (= Feed-In Load Limitation)                   */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*    BlockHeader                                                       6    */
/*    LSA_UINT16          Reserved                                      2    */
/*    LSA_UINT8           LoadLimitationActive                          1    */
/*    LSA_UINT8           IO_Configured                                 1    */
/*    LSA_UINT16          Reserved                                      2    */
/*                                                                    -----  */
/*                                                                     12    */
/*---------------------------------------------------------------------------*/

#define EDDP_PRM_PDNRT_FEED_IN_LOAD_LIMITATION_RECORD_INDEX        0x10003

#define EDDP_PRM_PDNRT_FEED_IN_LOAD_LIMITATION_BLOCK_TYPE          0xF004
#define EDDP_PRM_PDNRT_FEED_IN_LOAD_LIMITATION_BLOCK_SIZE          (EDDP_PRM_RECORD_HEADER_SIZE+6)
#define EDDP_PRM_PDNRT_FEED_IN_LOAD_LIMITATION_BLOCK_LENGTH        (EDDP_PRM_PDNRT_FEED_IN_LOAD_LIMITATION_BLOCK_SIZE-4)

#define EDDP_PRM_PDNRT_FEED_IN_LOAD_LIMITATION_INACTIVE            0x00
#define EDDP_PRM_PDNRT_FEED_IN_LOAD_LIMITATION_ACTIVE              0x01

#define EDDP_PRM_PDNRT_FEED_IN_LOAD_LIMITATION_IO_NOT_CONFIGURED   0x00
#define EDDP_PRM_PDNRT_FEED_IN_LOAD_LIMITATION_IO_CONFIGURED       0x01

typedef struct _EDDP_PDNRT_FILL_RECORD_SET_TYPE
{
    LSA_BOOL             Present;              /* record set present ? */
    LSA_BOOL             LoadLimitationActive; /* FILL active: FALSE: INACTIVE, TRUE: ACTIVE */
    LSA_BOOL             IO_Configured;        /* IO_Configured: FALSE: NO, TRUE: YES */
} EDDP_PDNRT_FILL_RECORD_SET_TYPE;

typedef struct _EDDP_PDNRT_FILL_RECORD_SET_TYPE EDDP_LOCAL_MEM_ATTR * EDDP_PDNRT_FILL_RECORD_SET_PTR_TYPE;

/* ---------------------------------------------------------------------------- */
/*   PDSetDefaultPortStates                                                     */
/* ---------------------------------------------------------------------------- */
/*    BlockHeader                                                       6       */
/*    LSA_UINT8           Padding                                       1       */
/*    LSA_UINT8           Padding                                       1       */
/*    (                                                                         */
/*      LSA_UINT8           PortId                                      1       */
/*      LSA_UINT8           PortState                                   1       */
/*      LSA_UINT8           Padding                                     1       */
/*      LSA_UINT8           Padding                                     1       */
/*    )*                                                                        */
/*                                                                              */
/* PortID: 1..255 (a valid PortId)                                              */
/* each PortId must be present Port and only appears once within record!        */
/* Multiple PortId settings may be present within record.                       */
/*                                                                              */
/* ---------------------------------------------------------------------------- */
#define EDDP_PRM_PDSET_DEF_PORTSTATES_BLOCK_TYPE            0xF001
#define EDDP_PRM_PDSET_DEF_PORTSTATES_BLOCK_MIN_SIZE        (EDDP_PRM_RECORD_HEADER_SIZE + 2)
#define EDDP_PRM_PDSET_DEF_PORTSTATES_BLOCK_MAX_SIZE        (EDDP_PRM_PDSET_DEF_PORTSTATES_BLOCK_MIN_SIZE + 4 * EDDP_MAX_PORT_CNT)

typedef struct _EDDP_PRM_RECORD_SET_PORT_STATES_TYPE
{
    LSA_BOOL        PortStatePresent; /* is PortState present? */
    LSA_UINT8       PortState; 
} EDDP_PRM_RECORD_SET_PORT_STATES_TYPE;

typedef struct _EDDP_PRM_RECORD_SET_PORT_STATES_TYPE EDDP_LOCAL_MEM_ATTR * EDDP_PRM_RECORD_SET_PORT_STATES_PTR_TYPE;

/* PortState : */
#define EDDP_PRM_RECORD_PD_SET_DEF_PORTSTATES_DISABLE  0x01
#define EDDP_PRM_RECORD_PD_SET_DEF_PORTSTATES_BLOCKING 0x02

typedef struct _EDDP_PRM_SET_DEF_PORTSTATES_TYPE
{
    LSA_BOOL                             Present;
//    LSA_BOOL                             Present_B;
    EDDP_PRM_RECORD_SET_PORT_STATES_TYPE RecordSet[EDDP_MAX_PORT_CNT + 1];
//    EDDP_PRM_RECORD_SET_PORT_STATES_TYPE RecordSet_B[EDDP_MAX_PORT_CNT];
} EDDP_PRM_SET_DEF_PORTSTATES_TYPE;

typedef struct _EDDP_PRM_SET_DEF_PORTSTATES_TYPE EDDP_LOCAL_MEM_ATTR * EDDP_PRM_SET_DEF_PORTSTATES_PTR_TYPE;


/* ---------------------------------------------------------------------------- */
typedef struct _EDDP_PRM_SINGLE_RECORD_TYPE
{
    EDD_COMMON_MEM_PTR_TYPE         pRecord;
    LSA_UINT32                      Record_Size;
    LSA_UINT32                      Record_Length;
    LSA_BOOL                        Record_IsPresent;
    LSA_BOOL                        Record_Skip;    
} EDDP_PRM_SINGLE_RECORD_TYPE;

typedef struct _EDDP_PRM_RECORDSET_TYPE     EDDP_LOCAL_MEM_ATTR *   EDDP_PRM_RECORDSET_PTR_TYPE;
typedef struct _EDDP_PRM_RECORDSET_TYPE
{
    EDD_COMMON_MEM_PTR_TYPE         pPDNCDataCheck;
    LSA_UINT32                      PDNCDataCheck_Size;
    LSA_UINT32                      PDNCDataCheck_Length;
    LSA_BOOL                        PDNCDataCheck_IsPresent;
    LSA_BOOL                        PDNCDataCheck_Skip;

    EDD_COMMON_MEM_PTR_TYPE         pPDIRData;
    LSA_UINT32                      PDIRData_Size;
    LSA_UINT32                      PDIRData_Length;
    LSA_BOOL                        PDIRData_IsPresent;
    LSA_BOOL                        PDIRData_Skip;

    EDD_COMMON_MEM_PTR_TYPE         pPDIRApplicationData;
    LSA_UINT32                      PDIRApplicationData_Size;
    LSA_UINT32                      PDIRApplicationData_Length;
    LSA_BOOL                        PDIRApplicationData_IsPresent;
    LSA_BOOL                        PDIRApplicationData_Skip;

    EDD_COMMON_MEM_PTR_TYPE         pPDIRSubframeData;
    LSA_UINT32                      PDIRSubframeData_Size;
    LSA_UINT32                      PDIRSubframeData_Length;
    LSA_BOOL                        PDIRSubframeData_IsPresent;
    LSA_BOOL                        PDIRSubframeData_Skip;

    EDD_COMMON_MEM_PTR_TYPE         pPDSyncData;
    LSA_UINT32                      PDSyncData_Size;
    LSA_UINT32                      PDSyncData_Length;
    LSA_BOOL                        PDSyncData_IsPresent;
    LSA_BOOL                        PDSyncData_Skip;

    EDD_COMMON_MEM_PTR_TYPE         pPDInterfaceMrpDataAdjust;
    LSA_UINT32                      PDInterfaceMrpDataAdjust_Size;
    LSA_UINT32                      PDInterfaceMrpDataAdjust_Length;    
    LSA_BOOL                        PDInterfaceMrpDataAdjust_IsPresent;
    LSA_BOOL                        PDInterfaceMrpDataAdjust_Skip;

    EDD_COMMON_MEM_PTR_TYPE         pPDNRTFeedInLoadLimitation;
    LSA_UINT32                      PDNRTFeedInLoadLimitation_Size;
    LSA_UINT32                      PDNRTFeedInLoadLimitation_Length;
    LSA_BOOL                        PDNRTFeedInLoadLimitation_IsPresent;
    LSA_BOOL                        PDNRTFeedInLoadLimitation_Skip;

    EDD_COMMON_MEM_PTR_TYPE         pPDSetDefaultPortStates;
    LSA_UINT32                      PDSetDefaultPortStates_SizeMin;
    LSA_UINT32                      PDSetDefaultPortStates_SizeMax;
    LSA_UINT32                      PDSetDefaultPortStates_Length;
    LSA_BOOL                        PDSetDefaultPortStates_IsPresent;
    LSA_BOOL                        PDSetDefaultPortStates_Skip;

    EDDP_PRM_SINGLE_RECORD_TYPE     PDPortMrpDataAdjust[EDDP_MAX_PORT_CNT +1];      //+1 to ease access with Port ID */
//  EDDP_PRM_SINGLE_RECORD_TYPE     PDPortMrpIcDataAdjust[EDDP_MAX_PORT_CNT +1];    //+1 to ease access with Port ID */
    EDDP_PRM_SINGLE_RECORD_TYPE     PDPortDataAdjust[EDDP_MAX_PORT_CNT +1];         //+1 to ease access with Port ID */
} EDDP_PRM_RECORDSET_TYPE;


/*===========================================================================*/
/*                          PrmCommit state machine                          */
/*===========================================================================*/
// states for state machine of PrmCommit
typedef enum _EDDP_PRM_COMMIT_STATE_TYPE
{
    EDDP_PRM_COMMIT_STATE_IDLE                      = 0,
    EDDP_PRM_COMMIT_STATE_START                     = 1,
    EDDP_PRM_COMMIT_STATE_PASSIVATE_IRT             = 2,
    EDDP_PRM_COMMIT_STATE_PASSIVATE_CRT_PACK        = 3,
    EDDP_PRM_COMMIT_STATE_ACTIVATE_CRT_PACK         = 4,
    EDDP_PRM_COMMIT_STATE_ACTIVATE_IRT              = 5,
    EDDP_PRM_COMMIT_STATE_SETUP_TRANSFER_END_ON     = 6,
    EDDP_PRM_COMMIT_STATE_FINISH                    = 7
} EDDP_PRM_COMMIT_STATE_TYPE;

typedef struct _EDDP_PRM_COMMIT_SM_TYPE     EDDP_LOCAL_MEM_ATTR *   EDDP_PRM_COMMIT_SM_PTR_TYPE;
typedef struct _EDDP_PRM_COMMIT_SM_TYPE
{
    EDDP_PRM_COMMIT_STATE_TYPE      State;
    EDD_RSP                         Response;
    LSA_BOOL                        IsDfpPresentA;
    LSA_BOOL                        IsDfpPresentB;
} EDDP_PRM_COMMIT_SM_TYPE;


/*===========================================================================*/
/*                  cases for setup of TransferEnd                           */
/*===========================================================================*/
typedef enum _EDDP_PRM_TRANSFER_END_CASE_TYPE
{
    EDDP_PRM_TRANSFER_END_CASE_OFF                  = 0,    /* TransferEnd trigger is OFF */
    EDDP_PRM_TRANSFER_END_CASE_PDIRDATA_RED         = 1,    /* PDIRData with red phase */
    EDDP_PRM_TRANSFER_END_CASE_NO_PARAMETRIZATION   = 2,    /* no PDIRData and no PDSyncData */
} EDDP_PRM_TRANSFER_END_CASE_TYPE;


/*===========================================================================*/
/*                  modes for setup of RsvIntervalRed                        */
/*===========================================================================*/
typedef enum _EDDP_PRM_RSV_INTERVAL_RED_MODE_TYPE
{
    EDDP_PRM_RSV_INTERVAL_RED_MODE_RX       = 1,    /* RsvIntervalRed for all local received frames */
    EDDP_PRM_RSV_INTERVAL_RED_MODE_TX       = 2,    /* RsvIntervalRed for all local sent frames */
    EDDP_PRM_RSV_INTERVAL_RED_MODE_RX_TX    = 3     /* RsvIntervalRed for all local received and sent frames */
} EDDP_PRM_RSV_INTERVAL_RED_MODE_TYPE;

/*===========================================================================*/
/*                  modes for setup of LocalTransferStartEnd                 */
/*===========================================================================*/
typedef enum _EDDP_PRM_LOCAL_TRANSFER_START_END_MODE_TYPE
{
    EDDP_PRM_LOCAL_TRANSFER_START_END_MODE_RX_END       = 1,    /* RxLocalTransferEndNs, see EDDP_PRMPDIRDataGetLocalTransferEndNs */
    EDDP_PRM_LOCAL_TRANSFER_START_END_MODE_TX_START     = 2,    /* TxLocalTransferStartNs, see EDDP_PRMPDIRDataGetLocalTransferStartNs */
    EDDP_PRM_LOCAL_TRANSFER_START_END_MODE_TX_END       = 3,    /* TxLocalTransferEndNs, EDDP_PRMPDIRDataGetLocalTransferEndNs */
} EDDP_PRM_LOCAL_TRANSFER_START_END_MODE_TYPE;


/*===========================================================================*/
/*              configuration parameter for PRM                              */
/*===========================================================================*/
typedef struct _EDDP_PRM_CFG_PARAM_TYPE
{
    EDDP_PRM_CFG_PARAM_MIN_MAXBRIDGEDELAY_TYPE      Min_MaxBridgeDelay;
} EDDP_PRM_CFG_PARAM_TYPE;


/*===========================================================================*/
/*                              PRM-DDB                                      */
/*===========================================================================*/
typedef struct _EDDP_DDB_PRM_TYPE   EDDP_LOCAL_MEM_ATTR *   EDDP_DDB_PRM_PTR_TYPE;
typedef struct _EDDP_DDB_PRM_TYPE            
{
    LSA_UINT32                                                  HandleCnt;
    LSA_HANDLE_TYPE                                             Handle;
    EDD_HANDLE_LOWER_TYPE                                       EDD_Handle;
    LSA_UINT32                                                  PrmState;

    LSA_UINT32                                                  Timer;
    LSA_BOOL                                                    Timer_isRunning;

    LSA_UINT32                                                  PrmCommitIndDiagEntryCntIF;
    EDDP_RQB_QUEUE_TYPE                                         PrmIndReq [EDDP_MAX_PORT_CNT+1];     /* Interface(0) + Ports(1..Max) */
    LSA_BOOL                                                    DoIndicate[EDDP_MAX_PORT_CNT+1];    /* Interface(0) + Ports(1..Max) */

    LSA_BOOL                                                    FirstCommitExecuted;

    EDDP_PRM_NC_DATA_TYPE                                       FrameDrop;
    EDDP_PRM_PORT_DATA_ADJUST_TYPE                              PortDataAdjust;
    EDDP_PRM_MRP_PORT_DATA_TYPE                                 MRPPort;

    EDDP_PRM_PDIRDATA_TYPE                                      PDIRData_A;
    EDDP_PRM_PDIRDATA_TYPE                                      PDIRData_B;
    EDDP_PRM_PDIRDATA_PTR_TYPE                                  pPDIRData_A;
    EDDP_PRM_PDIRDATA_PTR_TYPE                                  pPDIRData_B;

    EDDP_PRM_PDIRAPPLICATIONDATA_TYPE                           PDIRApplicationData_A;
    EDDP_PRM_PDIRAPPLICATIONDATA_TYPE                           PDIRApplicationData_B;
    EDDP_PRM_PDIRAPPLICATIONDATA_PTR_TYPE                       pPDIRApplicationData_A;
    EDDP_PRM_PDIRAPPLICATIONDATA_PTR_TYPE                       pPDIRApplicationData_B;

    EDDP_PRM_PDIRSUBFRAMEDATA_TYPE                              PDIRSubframeData_A;
    EDDP_PRM_PDIRSUBFRAMEDATA_TYPE                              PDIRSubframeData_B;
    EDDP_PRM_PDIRSUBFRAMEDATA_PTR_TYPE                          pPDIRSubframeData_A;
    EDDP_PRM_PDIRSUBFRAMEDATA_PTR_TYPE                          pPDIRSubframeData_B;

    EDDP_PRM_PDSYNCDATA_TYPE                                    PDSyncData;
    EDDP_PRM_PDSCFDATACHECK_TYPE                                PDSCFDataCheck;

    EDDP_PDNRT_FILL_RECORD_SET_TYPE                             PDNRTFeedInLoadLimitation_A;
    EDDP_PDNRT_FILL_RECORD_SET_TYPE                             PDNRTFeedInLoadLimitation_B;
    EDDP_PDNRT_FILL_RECORD_SET_PTR_TYPE                         pPDNRTFeedInLoadLimitation_A;
    EDDP_PDNRT_FILL_RECORD_SET_PTR_TYPE                         pPDNRTFeedInLoadLimitation_B;
    
    EDDP_PRM_SET_DEF_PORTSTATES_TYPE                            PDSetDefaultPortStates_A;
    EDDP_PRM_SET_DEF_PORTSTATES_TYPE                            PDSetDefaultPortStates_B;
    EDDP_PRM_SET_DEF_PORTSTATES_PTR_TYPE                        pPDSetDefaultPortStates_A;
    EDDP_PRM_SET_DEF_PORTSTATES_PTR_TYPE                        pPDSetDefaultPortStates_B;

    EDDP_PRM_RECORDSET_TYPE                                     RecordsetA;
    EDDP_PRM_RECORDSET_TYPE                                     RecordsetB;
    EDDP_PRM_RECORDSET_PTR_TYPE                                 pRecordsetA;
    EDDP_PRM_RECORDSET_PTR_TYPE                                 pRecordsetB;

    EDDP_PRM_PDIRFRAMEDATA_MRT_DIAG_PENDING_GROUP_TYPE          MrtDiagPendingGroup;
    EDDP_PRM_PDIRFRAMEDATA_LATE_ERR_DIAG_PENDING_GROUP_TYPE     LateErrDiagPendingGroup;

    LSA_BOOL                                                    isPrmCommitRunning;
    LSA_BOOL                                                    isTriggerPrmCommitFinish;
    EDDP_PRM_COMMIT_SM_TYPE                                     PrmCommitSm;

    LSA_UINT32                                                  TransferEndNs;

    EDDP_PRM_CFG_PARAM_TYPE                                     CfgParam;

    LSA_UINT8                       PortparamsNotApplicable[EDD_CFG_MAX_PORT_CNT];  //Accessed by UsrPortIndex
    LSA_UINT8                       ApplyDefaultPortParams[EDD_CFG_MAX_PORT_CNT];   //Accessed by UsrPortIndex
    LSA_UINT8                       IsModularPort[EDD_CFG_MAX_PORT_CNT];            //Accessed by UsrPortIndex
} EDDP_DDB_PRM_TYPE;
/* HandleCnt                    : Number of Handles(only 1)                                                         */
/* Handle                       : Channel handle                                                                    */
/* PrmState                     : Actual State                                                                      */
/* Timer                        : cylic One second timer                                                            */
/* Timer_isRunning              : LSA_TRUE when Timer has been started                                              */
/* PrmCommitIndDiagEntryCntIF   : Number of entries in indication RQB for Interface(Port=0) on PrmCommit            */
/* PrmIndReq                    : Indication queues. 0 = Interface                                                  */
/* DoIndicate                   : Indicate for port. 0 = Interface                                                  */
/* FirstCommitExecuted          : LSA_TRUE after first PRM_COMMIT after startup was executed                        */
/* FrameDrop                    : PDNC Record                                                                       */
/* PortDataAdjust               : PDPort Record                                                                     */
/* MRPPort                      : PDPortMrpDataAdjust Record                                                        */
/* PDIRData_A                   : storage of PDIRData Records, first set                                            */    
/* PDIRData_B                   : storage of PDIRData Records, second set                                           */
/* pPDIRData_A                  : currently active PDIRData Record, pointer to first or 2nd set                     */                                        
/* pPDIRData_B                  : new PDIRData Record, pointer to first or 2nd set                                  */                                        
/* PDIRSubframeData_A           : storage of PDIRDataSubframeData Records, first set                                */
/* PDIRSubframeData_B           : storage of PDIRDataSubframeData Records, second set                               */
/* pPDIRSubframeData_A          : currently active PDIRSubframeData Record, pointer to first or 2nd set             */
/* pPDIRSubframeData_B          : new PDIRSubframeData Record, pointer to first or 2nd set                          */
/* PDSyncData                   : PDSyncData Record                                                                 */
/* PDSCFDataCheck               : PDSCFDataCheck Record                                                             */
/* RecordsetA                   : storage of Records, first set                                                     */
/* RecordsetB                   : storage of Records, second set                                                    */
/* pRecordsetA                  : currently active parameter set, pointer to first or 2nd set                       */
/* pRecordsetB                  : temporary storage for new parameter set,pointer to first or 2nd set               */
/* MrtDiagPendingGroup          : Management structure for MrtDiagPendingGroup                                      */
/* LateErrDiagPendingGroup      : Management structure for LateErrDiagPendingGroup                                  */
/* isPrmCommitRunning           : the PrmCommit is running currently                                                */
/* isTriggerPrmCommitFinish     : trigger the PrmCommit finish after PrmCommit confirmation                         */
/* PrmCommitSm                  : PrmCommit state machine                                                           */
/* TransferEndNs                : TransferEnd time in ns                                                            */
#define EDDP_PRM_TRANSFER_END_TRIGGER_OFF_VALUE     0xFFFFFFFF
/* CfgParam                     : Initialization/configuration parameter for PRM                                    */


/* PrmState */
#define EDDP_PRM_STATE_PREPARED     0       /* PREPARED for new param sequence */
#define EDDP_PRM_STATE_WRITE        1       /* at least one WRITE after PREPARED */
#define EDDP_PRM_STATE_END          2       /* END arrived (await commit) */


/* ---------------------------------------------------------------------------- */
/*   PDInterfaceMrpDataAdjust, PDPortMrpDataAdjust, PDPortMrpIcDataAdjust       */
/* ---------------------------------------------------------------------------- */
#define EDDP_PRM_PDINTERFACE_MRP_DATA_ADJUST_RECORD_INDEX           0x8052
#define EDDP_PRM_PDPORT_MRP_DATA_ADJUST_RECORD_INDEX                0x8053
//#define EDDP_PRM_PDPORT_MRP_IC_DATA_ADJUST_RECORD_INDEX             0x8055 


/* ---------------------------------------------------------------------------- */
/*   PDPortStatistic                                                            */
/* ---------------------------------------------------------------------------- */
/*      BlockHeader                                                     6       */
/*      Padding                                                         1       */
/*      Padding                                                         1       */
/*      ifInOctets                                                      4       */
/*      ifOutOctets                                                     4       */
/*      ifInDiscards                                                    4       */
/*      ifOutDiscards                                                   4       */
/*      ifInErrors                                                      4       */
/*      ifOutErrors                                                     4       */
/*                                                                  ---------   */
/*                                                                      32      */
/*      BlockHeader.BlockLength = RecordLength without BlockType+BlockLength    */
/*      BlockHeader.BlockLength =                                       28      */
/* ---------------------------------------------------------------------------- */
#define EDDP_PRM_PDPORT_STATISTIC_RECORD_INDEX                      0x8072
#define EDDP_PRM_PDPORT_STATISTIC_RECORD_LENGTH                     32
#define EDDP_PRM_PDPORT_STATISTIC_BLOCKTYPE                         0x0251
#define EDDP_PRM_PDPORT_STATISTIC_BLOCKLENGTH                       (EDDP_PRM_PDPORT_STATISTIC_RECORD_LENGTH - 4)
#define EDDP_PRM_PDPORT_STATISTIC_BLOCKVERSIONHIGH                  (EDDP_PRM_BLOCK_VERSION_HIGH)
#define EDDP_PRM_PDPORT_STATISTIC_BLOCKVERSIONLOW                   (EDDP_PRM_STATISTIC_BLOCK_VERSION_LOW)


/* ---------------------------------------------------------------------------- */
/*   User specific records                                                      */
/*   The following records are not used by EDDP but they must be known          */
/* ---------------------------------------------------------------------------- */
#define EDDP_PRM_PDCONTROL_PLL_RECORD_INDEX                     0xB050
#define EDDP_PRM_PDTRACE_UNIT_CONTROL_RECORD_INDEX              0xB060
#define EDDP_PRM_PDTRACE_UNIT_DATA_RECORD_INDEX                 0xB061
#define EDDP_PRM_PDSET_DEFAULT_PORT_STATES_RECORD_INDEX         0x10001





/* ============================================================================ */
/*                    HDB-PRM Handle management structure                       */
/* ============================================================================ */
typedef struct _EDDP_HDB_PRM_TYPE   EDDP_LOCAL_MEM_ATTR *   EDDP_LOCAL_HDB_PRM_PTR_TYPE;
typedef struct _EDDP_HDB_PRM_TYPE
{
    LSA_UINT32          RunningRQBCount;
} EDDP_HDB_PRM_TYPE;
/* RunningRQBCount      : count of current running RQBs in GlbCrt queue of this PRM channel         */





/*===========================================================================*/
/*                                prototypes                                 */
/*===========================================================================*/

LSA_EXTERN EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMCloseChannel(
	EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
	EDDP_LOCAL_HDB_PTR_TYPE         pHDB,
	LSA_HANDLE_TYPE                 Handle);

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMRequest(
	EDD_UPPER_RQB_PTR_TYPE          pRQB,
	EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
	EDDP_LOCAL_HDB_PTR_TYPE         pHDB);

LSA_EXTERN EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMOpenChannel(
	EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
	EDDP_LOCAL_HDB_PTR_TYPE         pHDB,
	LSA_HANDLE_TYPE                 Handle,
	EDDP_SYSTEM_CDB_PTR_TYPE        pCDB);

LSA_EXTERN EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMDeviceClose(
	EDDP_LOCAL_DDB_PTR_TYPE         pDDB);

LSA_EXTERN EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMDeviceOpen(
	EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
	EDDP_UPPER_DPB_PTR_TYPE         pDPB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMDeviceIFSetup( 
	EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDDP_UPPER_DSB_PTR_TYPE         pDSB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMDeviceIFShutdown( 
    EDDP_LOCAL_DDB_PTR_TYPE         pFDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataRecord_CheckMaxPortDelays(
   	EDDP_LOCAL_DDB_PTR_TYPE         const pDDB,
   	EDDP_PRM_PDIRGLOBALDATA_TYPE  * const pPDIRGlobalData,
    LSA_UINT32                      const HWPortIndex,
    LSA_UINT32                    * const pErrOffset);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataRecord_Write(
   	EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
   	EDD_UPPER_MEM_U8_PTR_TYPE       pRecord);
   	
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDSyncDataRecord_Write(
   	EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
   	EDD_UPPER_MEM_U8_PTR_TYPE       pRecord);
   	
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDPortDataAdjustRecord_Write(
   	EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
   	EDD_UPPER_MEM_U8_PTR_TYPE       pRecord,
   	LSA_UINT16                      PortID);
   	
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDNCDataCheckRecord_Write(
   	EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
   	EDD_UPPER_MEM_U8_PTR_TYPE       pRecord);
   	
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMCheckFrameDrop(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB );   	
   	   	  	 	
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataGetRTC3PortInfoTx(
    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB,
    LSA_UINT16                                  FrameID,
    LSA_BOOL                                    isRedundant,
    LSA_BOOL                                    InActiveRecord_A,
    EDDP_PRM_RTC3_ALL_PORTS_INFO_PTR_TYPE       pPortInfoArray);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataCheckFrameDetails(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_UINT16                      PDIRFrameDataIdx);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataCheckRedPhaseAvailable(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_UINT32                      TxRedOrangePeriodBegin,
	LSA_UINT32                      TxOrangePeriodBegin,
    LSA_UINT32                      TxGreenPeriodBegin,
    LSA_UINT32                      RxRedOrangePeriodBegin,
    LSA_UINT32                      RxOrangePeriodBegin,
    LSA_UINT32                      RxGreenPeriodBegin);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataCheckOrangePhaseAvailable(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_UINT32                      TxRedOrangePeriodBegin,
	LSA_UINT32                      TxOrangePeriodBegin,
    LSA_UINT32                      TxGreenPeriodBegin,
    LSA_UINT32                      RxRedOrangePeriodBegin,
    LSA_UINT32                      RxOrangePeriodBegin,
    LSA_UINT32                      RxGreenPeriodBegin);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataIsRedundantFrameIDCoupleAvailable(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_UINT16                      RedundFrameID,
    LSA_BOOL                        InActiveRecord_A);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataCheckRedundantFrameID_B(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_UINT16                      PDIRFrameDataIdx);

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_PRMGetPortCountFromTxPortGroupArray(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT8                   TxPortGroupArray);

LSA_UINT8 EDDP_LOCAL_FCT_ATTR EDDP_PRM_Map_UserTxPortGroupArray_To_HwTxPortGroupArray(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT8                   UserTxPortGroupArray);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataIsMRPDWatchDogActive(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_UINT16                      RedundFrameID,
    EDD_COMMON_MEM_U16_PTR_TYPE     pPDIRFrameDataIdx,
    LSA_BOOL                        InActiveRecord_A);

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataGetDataLen(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_UINT16                      FrameID,
    LSA_BOOL                        InActiveRecord_A);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataIsFrameDataPropertiesPresent(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_BOOL                        InActiveRecord_A);

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataGetFrameDataProperties(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_UINT16                      Properties,
    LSA_BOOL                        InActiveRecord_A);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataCheckMaxBridgeDelay(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_UINT32                      MaxBridgeDelay);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataIsPDIRDataPresent(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_BOOL                        InActiveRecord_A);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMRequestPrmRead(
	EDD_UPPER_RQB_PTR_TYPE      pRQB,
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMRequestPrmPrepare(
	EDD_UPPER_RQB_PTR_TYPE      pRQB,
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMRequestPrmWrite(
	EDD_UPPER_RQB_PTR_TYPE      pRQB,
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMRequestPrmIndication(
	EDD_UPPER_RQB_PTR_TYPE      pRQB,
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMRequestPrmEnd(
	EDD_UPPER_RQB_PTR_TYPE      pRQB,
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMRequestPrmCommit(
	EDD_UPPER_RQB_PTR_TYPE      pRQB,
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMRequestPrmChangePort(
	EDD_UPPER_RQB_PTR_TYPE      pRQB,
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMChangePortStateMachine(
	EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_PRM_PORT_MODULE_SM_TRIGGER_TYPE    Trigger,
    LSA_UINT32                              HwPortIndex,
    EDD_RQB_PRM_CHANGE_PORT_TYPE*           pRQBParams);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMChangePortState_IsPlugged(
	EDDP_LOCAL_DDB_PTR_TYPE const           pDDB,
    LSA_UINT32                              PortIndex);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMChangePortState_IsPulled(
	EDDP_LOCAL_DDB_PTR_TYPE const           pDDB,
    LSA_UINT32                              PortIndex);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMChangePortState_IsAnySequenceRunning(
	EDDP_LOCAL_DDB_PTR_TYPE const           pDDB,
    LSA_UINT32 *                            pPortIndex);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMDiagIndicateDiagAllAfterPrmCommit(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRSubframeDataRecord_Write(
   	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
   	EDD_UPPER_MEM_U8_PTR_TYPE   pRecord);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRSubframeDataCheckAndValidate(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRSubframeDataGetHwConfigFromSubframeBlock(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  SubframeBlockIdx);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataCheckDoubleFrameID(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PDIRFrameDataIdx);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataInitSndRcvRole(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PDIRFrameDataIdx,
    LSA_BOOL                    InActiveRecord_A);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataIsSameIrtActive(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataIsSameIRApplicationDataActive(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataCalculateEndOfRedPhaseNsIF(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_BOOL                        InActiveRecord_A);

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataGetEndOfRedPhaseNsIF(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_PRM_RSV_INTERVAL_RED_MODE_TYPE     Mode,
    LSA_BOOL                                InActiveRecord_A);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataSetupLastLocalIRTFrame(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PDIRFrameDataIdx,
    LSA_BOOL                    InActiveRecord_A);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMCommitStateMachine(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDD_RSP                     Response);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMCommitStateMachineCbf(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDD_RSP                     Response);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMTriggerPrmCommitFinish(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRSubframeDataIsDfpPresent(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_BOOL                        InActiveRecord_A);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRSubframeDataIsSameDfpActive(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB);

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRSubframeDataGetTxPortIDFromTxPortGroupArray(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT8                   TxPortGroupArray);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRSubframeDataCheckSubframeDataLength(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  SubframeBlockIdx);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMCopySetB2A(
	EDDP_LOCAL_DDB_PTR_TYPE      pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMSetBToNotPresent(
	EDDP_LOCAL_DDB_PTR_TYPE      pDDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDPortStatisticRecord_Read(
	EDD_UPPER_RQB_PTR_TYPE      pRQB,
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDSCFDataCheckRecord_Write(
   	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_UPPER_MEM_U8_PTR_TYPE   pRecord);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRApplicationDataRecord_Write(
   	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
   	EDD_UPPER_MEM_U8_PTR_TYPE   pRecord);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMDiagSetupPrmIndEntryFrameDrop(
	EDD_SRV_PRM_IND_DATA_PTR_TYPE       pPrmIndData,
	LSA_UINT32                          ErrorStatus,
	LSA_BOOL                            isAppear);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMDiagTriggerMRTDiagSingle(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PDIRFrameDataIdx,
    LSA_BOOL                    isAppear,
    LSA_BOOL                    isEmptyDiag,
    LSA_BOOL                    isPrmCommit);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMDiagTriggerMRTDiagAllPending(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_BOOL                    isPrmCommit);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMDiagTriggerLateErrorDiagSingle(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PDIRFrameDataIdx,
    LSA_BOOL                    isAppear,
    LSA_BOOL                    isEmptyDiag,
    LSA_BOOL                    isPrmCommit);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMDiagTriggerLateErrorDiagAllPending(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_BOOL                    isPrmCommit);

#define EDDP_PRM_IRFRAMEDATA_DIAG_TYPE_MRT              0x1
#define EDDP_PRM_IRFRAMEDATA_DIAG_TYPE_DFP_LATE_ERR     0x2
LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMDiagIndicateIRDataDiag(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  DiagType,
    LSA_UINT16                  FrameID,
    LSA_BOOL                    isAppear,
    LSA_BOOL                    isEmptyDiag,
    LSA_BOOL                    isPrmCommit);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMDiagIndicateFinish(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT16                  PortID,
    LSA_UINT32                  IndDiagEntryCnt);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMDiagIndicateDiagOnePort(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT16                  PortID,
    LSA_BOOL                    isPrmCommit);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMDiagPDIRFrameDataAddToMrtDiagPendingList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PDIRFrameDataIdx);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMDiagPDIRFrameDataRemoveFromMrtDiagPendingList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PDIRFrameDataIdx);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMDiagPDIRFrameDataAddToLateErrDiagPendingList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PDIRFrameDataIdx);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMDiagPDIRFrameDataRemoveFromLateErrDiagPendingList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PDIRFrameDataIdx);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMSetupTransferEnd(
    EDDP_LOCAL_DDB_PTR_TYPE             pDDB,
    EDDP_PRM_TRANSFER_END_CASE_TYPE     TransferEndCase);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataCalculateLocalTransferStartEndNs(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB, 
    LSA_BOOL                    InActiveRecord_A);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataCalculateLocalTransferStartEndIntermediateValues(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PDIRFrameDataIdx,
    LSA_BOOL                    InActiveRecord_A);

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataGetLocalTransferEndNs(
    EDDP_LOCAL_DDB_PTR_TYPE                         pDDB,
    EDDP_PRM_LOCAL_TRANSFER_START_END_MODE_TYPE     Mode,
    LSA_BOOL                                        InActiveRecord_A);

LSA_INT32 EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataGetLocalTransferStartNs(
    EDDP_LOCAL_DDB_PTR_TYPE                         pDDB,
    EDDP_PRM_LOCAL_TRANSFER_START_END_MODE_TYPE     Mode,
    LSA_BOOL                                        InActiveRecord_A);

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataCalculateLocalRxApplicationDelay(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_BOOL                    isMRPDConsOddPresent,
    LSA_BOOL                    InActiveRecord_A);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataCheckFrameSendOffset(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PDIRFrameDataIdx);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataCheckRR(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PDIRFrameDataIdx);

    LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataChekYellowTimeInValidRange(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
   	LSA_UINT32                  YellowTime);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataSetupRsvIntervalRedValues(
    EDDP_LOCAL_DDB_PTR_TYPE             pDDB,
    EDD_RESERVED_INTERVAL_IF_RED_TYPE   *pRsvIntervalRed,
    LSA_BOOL                            isValid,
    LSA_BOOL                            InActiveRecord_A);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataIsFrameIDPresent(
    EDDP_LOCAL_DDB_PTR_TYPE                          pDDB,
    LSA_UINT16                                       FrameID,
    LSA_BOOL                                         isCrtProv,
    EDD_COMMON_MEM_U16_PTR_TYPE                      pCycleReductionRatio,
    EDD_COMMON_MEM_U16_PTR_TYPE                      pCyclePhase,
    LSA_BOOL                                         InActiveRecord_A);

/*****************************************************************************/
/*  end of file eddp_prm_inc.h                                               */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_PRM_INC_H */
