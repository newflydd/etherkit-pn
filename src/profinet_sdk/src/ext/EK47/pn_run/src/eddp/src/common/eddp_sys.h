#ifndef EDDP_SYS_H                     /* ----- reinclude-protection ----- */
#define EDDP_SYS_H

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
/*  F i l e               &F: eddp_sys.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  System interface                                                         */
/*  Defines constants, types, macros and prototyping for EDDP.               */
/*                                                                           */
/*  Note: All output macros are in eddp_out.h                                */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  13.08.08    JS    initial version.                                       */
/*  18.10.10    JS    added K32TraceLevel                                    */
/*  17.01.11    JS    added MaxInterfaceCntOfAllEDD and MaxPortCntOfAllEDD   */
/*  08.02.12    VS    added ApplTimer parameters for TransferEnd mode in     */
/*                    struct EDDP_IPB_CRT_TYPE                               */
/*  09.03.12    VS    added Hardware configuration parameters                */
/*                    "HWCfg.IRQx_Control" in EDDP_DPB_HW_CFG_TYPE           */
/*  02.03.16    HM    UserCmdSharedMem_BaseAddr added to DPB                 */
/*  08.04.16    SJ    Added parameters:                                      */
/*                           pCPMBaseAddrFastMem                             */
/*                           pCPMBaseAddrSlowMem                             */
/*                           CPMBaseAddrFastMemPNIP                          */
/*                           CPMBaseAddrSlowMemPNIP                          */
/*                           CPM_NILOffsetFastMem                            */
/*                           CPM_NILOffsetSlowMem                            */
/*                           pPPMBaseAddrFastMem                             */
/*                           pPPMBaseAddrSlowMem                             */
/*                           PPMBaseAddrFastMemPNIP                          */
/*                           PPMBaseAddrSlowMemPNIP                          */
/*  29.08.16    LU    Removed parameters:                                    */
/*                           CPM_NILOffsetFastMem                            */
/*                           CPM_NILOffsetSlowMem                            */

#endif
/*****************************************************************************/


/*===========================================================================*/
/*                              constants                                    */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* Maximal number of ports the EDDP supports.                                */
/* Note: EDD_CFG_MAX_PORT_CNT used for EDD- API calls and must be greater or */
/*       equal to EDDP_MAX_PORT_CNT!! EDDP_MAX_PORT_CNT used for internal    */
/*       EDDP structures.                                                    */
/*                                                                           */
/* EDDP_MAX_PORT_CNT   set to 4                                              */
/*                                                                           */
#ifdef EDDP_MESSAGE
/* Important: It is not allowed to change the EDDP_MAX_PORT_CNT to a         */
/*            value <> 4 because of internal structure of CMD-interface      */
/*            and CRT must be adapted.                                       */
#endif
/*---------------------------------------------------------------------------*/

// max. port count currently supported by EDDP
#define EDDP_MAX_PORT_CNT       4

// max. port count currently supported by ERTEC200P
#define EDDP_MAX_PORT_CNT_ERTEC200P         2

// max. port count currently supported by HERA
#define EDDP_MAX_PORT_CNT_HERA              4    // maximum PortCnt (Max(IF A, IF B)))
#define EDDP_MAX_PORT_CNT_HERA_IF_A         4    // maximum PortCnt of IFA
#define EDDP_MAX_PORT_CNT_HERA_IF_B         2    // maximum PortCnt of IFB

/*---------------------------------------------------------------------------*/
/* fatal-errorcodes (used for EDDP_FatalError() or EDDP_FATAL())             */
/*---------------------------------------------------------------------------*/

#define EDDP_FATAL_ERROR_TYPE       LSA_UINT32

#define EDDP_FATAL_ERR_TIMER_RESOURCE                   (EDDP_FATAL_ERROR_TYPE)  0x01
#define EDDP_FATAL_ERR_DEV_OPEN                         (EDDP_FATAL_ERROR_TYPE)  0x02
#define EDDP_FATAL_ERR_DEV_SETUP                        (EDDP_FATAL_ERROR_TYPE)  0x03
#define EDDP_FATAL_ERR_DEV_SHUTDOWN                     (EDDP_FATAL_ERROR_TYPE)  0x04
#define EDDP_FATAL_ERR_DEV_IF_SETUP                     (EDDP_FATAL_ERROR_TYPE)  0x05
#define EDDP_FATAL_ERR_DEV_IF_SHUTDOWN                  (EDDP_FATAL_ERROR_TYPE)  0x06
#define EDDP_FATAL_ERR_OPEN_CHANNEL                     (EDDP_FATAL_ERROR_TYPE)  0x07
#define EDDP_FATAL_ERR_UNDO_INIT_FAILED                 (EDDP_FATAL_ERROR_TYPE)  0x08
#define EDDP_FATAL_ERR_NULL_PTR                         (EDDP_FATAL_ERROR_TYPE)  0x09
#define EDDP_FATAL_ERR_MEM_FREE                         (EDDP_FATAL_ERROR_TYPE)  0x0A
#define EDDP_FATAL_ERR_RELEASE_PATH_INFO                (EDDP_FATAL_ERROR_TYPE)  0x0B
#define EDDP_FATAL_ERR_INCONSISTENCE                    (EDDP_FATAL_ERROR_TYPE)  0x0C
#define EDDP_FATAL_ERR_CHANNEL_USE                      (EDDP_FATAL_ERROR_TYPE)  0x0D
#define EDDP_FATAL_ERR_REENTRANCE                       (EDDP_FATAL_ERROR_TYPE)  0x0E
#define EDDP_FATAL_ERR_ENDIANES                         (EDDP_FATAL_ERROR_TYPE)  0x0F
#define EDDP_FATAL_ERR_NOT_SUPPORTED                    (EDDP_FATAL_ERROR_TYPE)  0x10
#define EDDP_FATAL_ERR_PARAM                            (EDDP_FATAL_ERROR_TYPE)  0x11
#define EDDP_FATAL_ERR_INIACW_BITVALUE                  (EDDP_FATAL_ERROR_TYPE)  0x12
#define EDDP_FATAL_ERR_INIACW_RANGEVALUE                (EDDP_FATAL_ERROR_TYPE)  0x13
#define EDDP_FATAL_ERR_USER_INI_PARAM                   (EDDP_FATAL_ERROR_TYPE)  0x14
#define EDDP_FATAL_ERR_SERVICE                          (EDDP_FATAL_ERROR_TYPE)  0x15
#define EDDP_FATAL_ERR_ALIGNMENT                        (EDDP_FATAL_ERROR_TYPE)  0x16
#define EDDP_FATAL_ERR_UNEXP_INTERRUPT                  (EDDP_FATAL_ERROR_TYPE)  0x17
#define EDDP_FATAL_ERR_GET_APICTRLRAM                   (EDDP_FATAL_ERROR_TYPE)  0x18
#define EDDP_FATAL_ERR_KRISC32_WATCHDOG                 (EDDP_FATAL_ERROR_TYPE)  0x19
#define EDDP_FATAL_ERR_CMD_TIMEOUT                      (EDDP_FATAL_ERROR_TYPE)  0x1A



/*===========================================================================*/
/*                                 defines                                   */
/*===========================================================================*/

#define EDDP_HANDLE             LSA_VOID EDDP_LOCAL_MEM_ATTR *
                                            /* for device-handles  */
#define EDDP_DETAIL_PTR_TYPE                 /* pointer to EDD_DETAIL */ \
    struct eddp_detail_tag      EDDP_SYSTEM_MEM_ATTR *

#define EDDP_FATAL_ERROR_PTR_TYPE           /* pointer to LSA_FATAL_ERROR */ \
    struct lsa_fatal_error_tag  EDDP_LOCAL_MEM_ATTR *


/*---------------------------------------------------------------------------*/
/* usage of LSA_RQB_LOCAL_ERROR_TYPE                                         */
/* - lsa_component_id:                                                       */
/*    LSA_COMP_ID_EDD                                                        */
/* - error_code_id not used. always 0                                        */
/* - error_code[12] used for following format:                               */
/*---------------------------------------------------------------------------*/

typedef struct EDDP_LOCAL_ERROR_CODE_   EDD_UPPER_RQB_ATTR *    EDDP_LOCAL_ERROR_CODE_PTR_TYPE;
typedef struct EDDP_LOCAL_ERROR_CODE_
{
    EDD_SERVICE         Service;
    LSA_RESPONSE_TYPE   Response;
    LSA_UINT16          ModuleId;   /* 0 if not present */
    LSA_UINT16          LineNr;     /* 0 if not present */
} EDDP_LOCAL_ERROR_CODE_TYPE;


/*---------------------------------------------------------------------------*/
/* Service within RQB for EDD_OPC_SYSTEM                                     */
/*                                                                           */
/* Bits 08..15 used for decoding of service by EDD-ERTEC. Do not change!     */
/*                                                                           */
/* Range 0x0000..0x0FFF reserved for Device-Services                         */
/*---------------------------------------------------------------------------*/

/*=====  EDD_OPC_SYSTEM:  System-Services (0x0100..0x01FF) ===*/
#define EDDP_SRV_DEV_TYPE               (EDD_SERVICE)   0x0100

#define EDDP_SRV_DEV_OPEN               (EDD_SERVICE)   0x0100
#define EDDP_SRV_DEV_SETUP              (EDD_SERVICE)   0x0101

#define EDDP_SRV_DEV_IF_SETUP           (EDD_SERVICE)   0x0102
#define EDDP_SRV_DEV_IF_SHUTDOWN        (EDD_SERVICE)   0x0103

#define EDDP_SRV_DEV_SHUTDOWN           (EDD_SERVICE)   0x0104
#define EDDP_SRV_DEV_CLOSE              (EDD_SERVICE)   0x0105
#define EDDP_SRV_DEV_INTERRUPT          (EDD_SERVICE)   0x0106
#define EDDP_SRV_DEV_TIMER              (EDD_SERVICE)   0x0107
#define EDDP_SRV_DEV_TRIGGER            (EDD_SERVICE)   0x0108

/*---------------------------------------------------------------------------*/
/* Additional status within RQBs �                                           */
/* ERR-Codes from Offset 0x2F..0x3E reserved for EDD specific systemfunctions*/
/*---------------------------------------------------------------------------*/

/* EDD OK-Code (MaxOffset = 0x3F) */
/* LSA_RSP_OK_OFFSET = 0x40       */

/* EDD ERR-Codes (MaxOffset = 0x3E)*/
/* LSA_RSP_ERR_OFFSET = 0xC0       */
#define EDD_STS_ERR_PARAM_XXXX           (EDD_RSP)   (LSA_RSP_ERR_OFFSET + 0x2F)

/*---------------------------------------------------------------------------*/
/* RQB- service-specific paramteter blocks, pParam points to.                */
/*      Opcode and Service specify the Paramblock                            */
/*                                                                           */
/*   RQB-Header                                                              */
/*  +------------+                                                           */
/*  |            |                                                           */
/*  |            |                                                           */
/*  |            |                                                           */
/*  |            |                                                           */
/*  |            |                                                           */
/*  |            |                                                           */
/*  |            |                                                           */
/*  |            |                                                           */
/*  +------------+ service-       e.g. EDD_UPPER_DEV_OPEN_PTR_TYPE           */
/*  |   pParam   | ------------->+-----------+                               */
/*  +------------+ specific      |           |                               */
/*                               | +------------+                            */
/*                               | |            |                            */
/*                               | | +-------------+                         */
/*                               | | |             |                         */
/*                                                                           */
/*                                                                           */
/*---------------------------------------------------------------------------*/


/*===========================================================================*/
/*======================= Device open/setup services ========================*/
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* Opcode: EDD_OPC_SYSTEM   Service: EDDP_SRV_DEV_OPEN                       */
/*---------------------------------------------------------------------------*/

/* --------------------------------------------------------------------------*/
/* DPB: Hardware configuration                                               */
/* --------------------------------------------------------------------------*/

typedef struct _EDDP_DPB_HW_CFG_IRQX_CONTROL_TYPE
{
    LSA_UINT8       IRQ2_Level;
    LSA_UINT8       IRQ2_Length;
    LSA_UINT8       IRQ3_Level;
    LSA_UINT8       IRQ3_Length;
} EDDP_DPB_HW_CFG_IRQX_CONTROL_TYPE;
/* IRQx_Level       : For configuration of PNIP-Register "IRQx_Control.IRQ_Level"                           */
/*                      EDDP_IRQX_CONTROL_IRQ_LEVEL_CFG_LOW                                                 */
/*                          The interrupt outputs PN_IRQx are low active.                                   */
/*                                                                                                          */
/*                      EDDP_IRQX_CONTROL_IRQ_LEVEL_CFG_HIGH                                                */
/*                          The interrupt outputs PN_IRQx are high active.                                  */
#define EDDP_IRQX_CONTROL_IRQ_LEVEL_CFG_LOW             0
#define EDDP_IRQX_CONTROL_IRQ_LEVEL_CFG_HIGH            1
/* IRQx_Length      : For configuration of PNIP-Register "IRQx_Control.IRQx_Length"                         */
/*                    Valid range: 0..7                                                                     */

typedef struct _EDDP_DPB_HW_CFG_TYPE
{
    EDDP_DPB_HW_CFG_IRQX_CONTROL_TYPE       IRQx_Control;
    LSA_UINT32                              IRQAcycRcv;
    LSA_UINT32                              TxLocalApplicationFeedExtNs;
    LSA_UINT32                              RxLocalApplicationDelayExtNs;
} EDDP_DPB_HW_CFG_TYPE;

/* --------------------------------------------------------------------------*/
/* DPB: CRT-Configuration                                                    */
/* --------------------------------------------------------------------------*/

typedef struct _EDDP_DPB_CRT_TYPE       EDDP_LOCAL_MEM_ATTR *    EDDP_DPB_CRT_PTR_TYPE;
typedef struct _EDDP_DPB_CRT_TYPE
{
    LSA_UINT16                      ProviderMaxNumber;
    LSA_UINT16                      ConsumerMaxNumber;
    EDD_UPPER_MEM_U8_PTR_TYPE       pIOBaseAdrPerif;             // ERTEC200P only
    EDD_UPPER_MEM_U8_PTR_TYPE       pIOBaseAdrExtRam;            // ERTEC200P only
    EDD_UPPER_MEM_U8_PTR_TYPE       pCPMBaseAddrFastMem;         // HERA only
    EDD_UPPER_MEM_U8_PTR_TYPE       pCPMBaseAddrSlowMem;         // HERA only 
    LSA_UINT32                      CPMBaseAddrFastMemPNIP;      // HERA only
    LSA_UINT32                      CPMBaseAddrSlowMemPNIP;      // HERA only 
    EDD_UPPER_MEM_U8_PTR_TYPE       pPPMBaseAddrFastMem;         // HERA only
    EDD_UPPER_MEM_U8_PTR_TYPE       pPPMBaseAddrSlowMem;         // HERA only
    LSA_UINT32                      PPMBaseAddrFastMemPNIP;      // HERA only
    LSA_UINT32                      PPMBaseAddrSlowMemPNIP;      // HERA only 
    LSA_UINT32                      PPM_Lifetime64FastMem;       // HERA only
    LSA_UINT32                      PPM_Lifetime64SlowMem;       // HERA only
    LSA_UINT8                       ApplTimerMode;
    LSA_UINT16                      ApplTimerReductionRatio;
    LSA_UINT32                      TransferEndCorrectionValue;
    LSA_UINT8                       isTransferEndCorrectionPos;
} EDDP_DPB_CRT_TYPE;
/* ConsumerMaxNumber        : Maximum number of Consumer in EDDP-CRT for one device                             */
/* ProviderMaxNumber        : Maximum number of Provider in EDDP-CRT for one device                             */
/* pIOBaseAdrPerif              : Base address of IO-Buffer for IO-Data in PERIF                                */
/* pIOBaseAdrExtRam             : Base address of IO-Buffer for IO-Data in external RAM                         */
/* ApplTimerMode                : Mode for configuration of Application-Timer:                                  */
/* pCPMBaseAddrFastMem          : Base address of fast memory region w used for Consumer IO transferstructures  */
/*                                and buffers.                                                                  */
/*                                Note: This address must be 64 Byte aligned.                                   */                    
/*                                Only used with HERA.                                                          */
/* pCPMBaseAddrSlowMem          : Base address of slow memory region used for Consumer IO transferstructures    */
/*                                and buffers.                                                                  */
/*                                Note: This address must be 64 Byte aligned.                                   */                    
/*                                Only used with HERA.                                                          */
/* CPMBaseAddrFastMemPNIP       : PN-IP point of view of base address of fast memory region used for Consumer   */
/*                                IO transferstructures. This 32 Bit address is written into the PN-IP register */
/*                                CPM_BasePtr_FastMem                                                           */
/*                                Note: This address must be 64 Byte aligned.                                   */                    
/*                                Only used with HERA.                                                          */
/* CPMBaseAddrSlowMemPNIP       : PN-IP point of view of base address of slow memory region used for Consumer   */
/*                                IO transferstructures. This 32 Bit address is written into the PN-IP register */
/*                                CPM_BasePtr_SlowMem                                                           */
/*                                Note: This address must be 64 Byte aligned.                                   */                    
/*                                Only used with HERA.                                                          */
/* pPPMBaseAddrFastMem          : Base address of fast memory region used for Provider IO transferstructures    */
/*                                and buffers.                                                                  */
/*                                Note: This address must be 64 Byte aligned.                                   */                    
/*                                Only used with HERA.                                                          */
/* pPPMBaseAddrSlowMem          : Base address of slow memory region used for Provider IO transferstructures    */
/*                                and buffers                                                                   */
/*                                Note: This address must be 64 Byte aligned.                                   */                    
/*                                Only used with HERA.                                                          */
/*                                Note: This address must be 64 Byte aligned.                                   */                    
/*                                Only used with HERA.                                                          */
/* PPMBaseAddrFastMemPNIP       : PN-IP point of view of base address of fast memory region used for Provider   */
/*                                IO transferstructures. This 32 Bit address is written into the PN-IP register */
/*                                PPM_BasePtr_FastMem                                                           */
/*                                Note: This address must be 64 Byte aligned.                                   */                    
/*                                Only used with HERA.                                                          */
/* PPMBaseAddrSlowMemPNIP       : PN-IP point of view of base address of slow memory region used for Provider   */
/*                                IO transferstructures. This 32 Bit address is written into the PN-IP register */
/*                                PPM_BasePtr_SlowMem                                                           */
#define EDDP_HERA_PPM_LIFETIME64_FASTMEM_MIN        8
#define EDDP_HERA_PPM_LIFETIME64_FASTMEM_MAX        0x7FFFFFF8
/* PPM_Lifetime64FastMem        : Optional parameter used for PPM Lifetime monitoring for fast memory within    */
/*                                PN-IP.                                                                        */
/*                                This parameter defines the maximum time in ns (with 8ns granularity) allowed  */
/*                                for reading the bufferoffset and the frame io data per read (each read reads  */
/*                                up to 64 bytes). The lifetime is determined depending on the number of reads  */
/*                                necessary.                                                                    */
/*                                Only used with HERA.                                                          */
#define EDDP_HERA_PPM_LIFETIME64_SLOWMEM_MIN        8
#define EDDP_HERA_PPM_LIFETIME64_SLOWMEM_MAX        0x7FFFFFF8
/* PPM_Lifetime64SlowMem        : Optional parameter used for PPM Lifetime monitoring for slow memory within    */
/*                                PN-IP.                                                                        */
/*                                This parameter defines the maximum time in ns (with 8ns granularity) allowed  */
/*                                for reading the bufferoffset and the frame io data per read (each read reads  */
/*                                up to 64 bytes). The lifetime is determined depending on the number of reads  */
/*                                necessary.                                                                    */
/*                                Only used with HERA.                                                          */
/*                                  EDDP_APPL_TIMER_CFG_MODE_NONE                                               */
/*                                      No configuration of Application-Timer by EDDP.                          */
/*                                                                                                              */
/*                                  EDDP_APPL_TIMER_CFG_MODE_TRANSFER_END                                       */
/*                                      EDDP configures the Application-Timer for TransferEnd event.            */
/*                                      Configuration is in "ApplCompare_2_1" of "ApplTimer_2".                 */
#define EDDP_APPL_TIMER_CFG_MODE_NONE               0
#define EDDP_APPL_TIMER_CFG_MODE_TRANSFER_END       1
/* ApplTimerReductionRatio      : Reduction Ratio for Application-Timer.                                        */
/*                                Evaluate only if ApplTimerMode = EDDP_APPL_TIMER_CFG_MODE_TRANSFER_END        */
/*                                Valid range: 1..16                                                            */
/* TransferEndCorrectionValue   : Value in ns for correction of TransferEnd interrupt                           */
/*                                Evaluate only if ApplTimerMode = EDDP_APPL_TIMER_CFG_MODE_TRANSFER_END        */
/*                                Valid range: 0..100000                                                        */
/* isTransferEndCorrectionPos   : This parameter sets the "TransferEndCorrectionValue" to positive or negative. */
/*                                EDDP_DPB_TRANSFERENDCORRECTIONVALUE_IS_POS: "TransferEndCorrectionValue" is positive */
/*                                EDDP_DPB_TRANSFERENDCORRECTIONVALUE_IS_NEG: "TransferEndCorrectionValue" is negative */
#define EDDP_DPB_TRANSFERENDCORRECTIONVALUE_IS_NEG  0
#define EDDP_DPB_TRANSFERENDCORRECTIONVALUE_IS_POS  1

/* --------------------------------------------------------------------------*/
/* DPB: SWI-Configuration                                                    */
/* --------------------------------------------------------------------------*/

typedef struct _EDDP_DPB_SWI_PORT_MAP_TYPE
{
	LSA_UINT32         PortID_x_to_HardwarePort_y[EDDP_MAX_PORT_CNT];
	LSA_UINT32         PortID_x_to_ASICPort_y[EDDP_MAX_PORT_CNT];
	LSA_UINT32         PortID_x_to_SMIPort_y[EDDP_MAX_PORT_CNT];

} EDDP_DPB_SWI_PORT_MAP_TYPE;

#define EDDP_PORT_NOT_CONNECTED   0xFFFFFFFFUL

/* ------------------------------------------------------------------------------------------------------------ */
/*                                                                                                              */
/* PortID_x_to_HardwarePort_y[] : First element maps PortID 1 to Hw Port X; second element maps Port ID 2;..    */
/*          Note: If a Port ID is not mapped to a Hardware Port it must be set to EDDP_PORT_NOT_CONNECTED.      */
/*                                                                                                              */
/* PortID_x_to_AsicPort_y[]     : First element maps PortID 1 to ASIC Port X; second element maps Port ID 2;... */
/*                                                                                                              */
/* PortID_x_to_SMIPort_y[]      : First element maps PortID 1 to SMI Port X; second element maps Port ID 2;...  */
/*                                                                                                              */
/* ------------------------------------------------------------------------------------------------------------ */

typedef struct _EDDP_DPB_SWI_TYPE       EDDP_LOCAL_MEM_ATTR *    EDDP_DPB_SWI_PTR_TYPE;
typedef struct _EDDP_DPB_SWI_TYPE
{
    LSA_UINT8                       Multicast_Bridge_IEEE802RSTP_Forward;
    EDDP_DPB_SWI_PORT_MAP_TYPE      PortMap;
    LSA_UINT8                       MaxMRP_Instances;
    LSA_UINT8                       MRPDefaultRoleInstance0; 
    LSA_UINT8                       MRPSupportedRole;
    LSA_UINT8                       MRPSupportedMultipleRole;
    LSA_UINT8                       MaxMRPInterconn_Instances;
    LSA_UINT8                       SupportedMRPInterconnRole;
    LSA_UINT8                       MRPRingPort[EDDP_MAX_PORT_CNT];
    LSA_UINT8                       SupportsMRPInterconnPortConfig[EDDP_MAX_PORT_CNT]; 
    LSA_UINT8                       MRAEnableLegacyMode;
    LSA_UINT32                      PhyAddressOffset;
} EDDP_DPB_SWI_TYPE;
/* Multicast_Bridge_IEEE802RSTP_Forward :EDD_FEATURE_ENABLE : Forward rapid spanning tree protokoll frames */
/*                                       EDD_FEATURE_DISABLE: Block rapid spanning tree protokoll frames   */
/* PortMap                    : Array of 4 elements in order to map Port IDs to Hardware Ports          */
/* MaxMRP_Instances           : Number of MRP instances supported for this (out)                        */
/*                              interface. This parameter is configured                                 */
/*                              within EDD startup.                                                     */
/*                              0: none. MRP not supported.                                             */
/*                              1.. EDD_CFG_MAX_MRP_INSTANCE_CNT                                        */
/* MRPDefaultRoleInstance0    : If MRP is supported but no MRP             (out)                        */
/*                              parameterization is present it is assumed                               */
/*                              that only one MRP instance is present.                                  */
/*                              In this case this parameter defines the MRP                             */
/*                              role for MRP instance 0.                                                */
/*                              The parameter is configured within EDD on                               */
/*                              startup and only forwarded to the user (MRP)                            */
/* MRPSupportedRole           : This parameter defines the possible MRP    (out)                        */
/*                              roles if only one MRP instance is present                               */
/*                              within parameterization (PRM).                                          */
/*                              The parameter is configured within EDD on                               */
/*                              startup and only forwarded to the user (MRP)                            */
/*                              Values can be ORed!                                                     */
/* MRPSupportedMultipleRole   : This parameter defines the possible MRP    (out)                        */
/*                              roles if multiple  MRP instance are present                             */
/*                              within parameterization (PRM).                                          */
/*                              The parameter is configured within EDD on                               */
/*                              startup and only forwarded to the user (MRP)                            */
/*                              Values can be ORed!                                                     */
/* SupportedMRPInterconnRole  : This parameter defines the possible MRP interconnect   (out)            */
/*                              roles.                                                                  */
/*                              The parameter is configured within EDD on                               */
/*                              startup and only forwarded to the user (MRP)                            */
/*                              Values can be ORed!                                                     */
/* MaxMRPInterconn_Instances  : Number of MRP interconnect instances supported for this (out)          */
/*                              interface. This parameter is configured                                 */
/*                              within EDD startup.                                                     */
/*                              0: none. MRP not supported.                                             */
/*                              1.. EDD_CFG_MAX_MRP_IN_INSTANCE_CNT                                     */
/* MRPRingPort                 : For each port of the interface this parameter defines if the port is   */
/*                               a ring port used with MRP and if this port is a default ring port on   */
/*                               startup                                                                */
/*                               -> possible defines for this parameter see <edd_usr.h>                 */
/* SupportsMRPInterconnPortConfig: For each port of the interface this parameter defines if the port is */
/*                               an interconnect ring port used with MRP.                               */
/*                               -> possible defines for this parameter see <edd_usr.h>                 */
/* MRAEnableLegacyMode        : MRA2.0 support: This parameter is only                                  */
/*                              forwarded to the user (MRP)                                             */
/* PhyAddressOffset           : This parameter defines the offset of the phy addresses.                 */
/*                              The eddp calculates the phy address as follows:                         */
/*                                  Port 1: PhyAddress = PhyAddressOffset                               */
/*                                  Port 2: PhyAddress = PhyAddressOffset + 1                           */
/*                                  Port 3: PhyAddress = PhyAddressOffset + 2                           */
/*                                  ....                                                                */
/*                              Range: 0 ... (31 - number of used ports)                                */

/* --------------------------------------------------------------------------*/
/* DPB: NRT-Configuration                                                    */
/* --------------------------------------------------------------------------*/

#define EDDP_HW_MAX_TX_RINGS      16      // the ASIC has upto 16 physical TX Rings
#define EDDP_HW_MAX_RX_RINGS      32      // the ASIC has upto 32 physical RX Rings

typedef struct _EDDP_DPB_NRT_TYPE       EDDP_LOCAL_MEM_ATTR *    EDDP_DPB_NRT_PTR_TYPE;
typedef struct _EDDP_DPB_NRT_TYPE
{   
    LSA_UINT16      TxItemsInDefaultRing;
    LSA_UINT16      TxItemsInTcpRing;
    LSA_UINT16      TxItemsInUdpRing;
    LSA_UINT16      TxItemsInIpMiscRing;
    LSA_UINT16      TxItemsInDcpRing;
    LSA_UINT16      TxItemsInAlarmRing;
    LSA_UINT16      TxItemsInMrpRing;
    LSA_UINT16      TxItemsInLldpRing;
    LSA_UINT16      TxItemsInPtcpRing;
    LSA_UINT16      TxSndTimeout;

    LSA_UINT8       FeedInLoadLimitationActive;
    LSA_UINT8       IO_Configured;

    LSA_UINT16      RxItemsInOtherRing;
    LSA_UINT16      RxItemsInIpMiscRing;
    LSA_UINT16      RxItemsInTcpRing;
    LSA_UINT16      RxItemsInUdpRing;
    LSA_UINT16      RxItemsInAlarmRing;
    LSA_UINT16      RxItemsInDcpRing;
    LSA_UINT16      RxItemsInPtcpRing;
    LSA_UINT16      RxItemsInLldpRing;
    LSA_UINT16      RxItemsInMrpRing;
    LSA_UINT16      RxItemsInMiscRing;
    LSA_UINT8       RxDropLogging;
    LSA_UINT8       RxFilterUDP_Unicast;
    LSA_UINT8       RxFilterUDP_Broadcast;
} EDDP_DPB_NRT_TYPE;
/* TxItemsInDefaultRing: Specify no of TX-items for default transmit.        */
/*                       --> this pool is used with: EDD_NRT_TX_GRP_DEFAULT  */
/* TxItemsInTcpRing    : Specify no of TX-items for TCP frames.              */
/*                       --> this pool is used with: EDD_NRT_TX_GRP_IP_TCP   */
/* TxItemsInUdpRing    : Specify no of TX-items for UDP frames.              */
/*                       --> this pool is used with: EDD_NRT_TX_GRP_IP_UDP   */
/* TxItemsInIpMiscRing : Specify no of TX-items for IP frames.               */
/*                       --> this pool is used with: EDD_NRT_TX_GRP_IP_ICMP  */
/*                                                   EDD_NRT_TX_GRP_IP_IGMP  */
/*                                                   EDD_NRT_TX_GRP_ARP      */
/* TxItemsInDcpRing    : Specify no of TX-items for DCP frames.              */
/*                       --> this pool is used with: EDD_NRT_TX_GRP_DCP      */
/* TxItemsInAlarmRing  : Specify no of TX-items for Alarm frames.            */
/*                       --> this pool is used with: EDD_NRT_TX_GRP_ASRT     */
/* TxItemsInMrpRing    : Specify no of TX-items for MRP frames.              */
/*                       --> this pool is used with: EDD_NRT_TX_GRP_MRP      */
/*                                                   EDD_NRT_TX_GRP_LEN_HSR  */
/*                                                   EDD_NRT_TX_GRP_LEN_STDBY*/
/* TxItemsInLldpRing   : Specify no of TX-items for LLDP frames.             */
/*                       --> this pool is used with: EDD_NRT_TX_GRP_LLDP     */
/* TxItemsInPtcpRing   : Specify no of TX-items for PTCP frames.             */
/*                       --> this pool is used with: EDD_NRT_TX_GRP_PTCP     */
/* TxSndTimeout        : Specify the TX-watchdog timeout in 31,25 us ticks   */
/*                       --> 0 disables the TX watchdog and the default will */
/*                           be set with 1000ms to finish transmission over  */
/*                           timeout if e.g. no port link exists             */

/* RxItemsInOtherRing  : Specify no of RX-items for OTHER frames.            */
/*                       --> this pool is used with: EDD_NRT_FRAME_IP_OTHER  */
/*                                                   EDD_NRT_FRAME_LEN_OTHER */
/*                                                   EDD_NRT_FRAME_OTHER     */
/* RxItemsInIpMiscRing : Specify no of RX-items for IP frames.               */
/*                       --> this pool is used with: EDD_NRT_FRAME_IP_IGMP   */
/*                                                   EDD_NRT_FRAME_IP_ICMP   */
/*                                                   EDD_NRT_FRAME_ARP       */
/* RxItemsInTcpRing    : Specify no of RX-items for TCP frames.              */
/*                       --> this pool is used with: EDD_NRT_FRAME_IP_TCP    */
/* RxItemsInUdpRing    : Specify no of RX-items for UDP frames.              */
/*                       --> this pool is used with: EDD_NRT_FRAME_IP_UDP    */
/* RxItemsInAlarmRing  : Specify no of RX-items for Alarm frames.            */
/*                       --> this pool is used with: EDD_NRT_FRAME_ASRT      */
/*                                                   EDD_NRT_FRAME_UDP_ASRT  */
/* RxItemsInDcpRing    : Specify no of RX-items for Alarm frames.            */
/*                       --> this pool is used with: EDD_NRT_FRAME_DCP       */
/*                                                   EDD_NRT_FRAME_DCP_HELLO */
/*                                                   EDD_NRT_FRAME_UDP_DCP   */
/*                                                EDD_NRT_FRAME_UDP_DCP_HELLO*/
/*                                                   EDD_NRT_FRAME_LEN_DCP   */
/* RxItemsInPtcpRing   : Specify no of RX-items for PTCP frames.             */
/*                       --> this pool is used with: EDD_NRT_FRAME_PTCP_ANNO */
/* RxItemsInLldpRing   : Specify no of RX-items for LLDP frames.             */
/*                       --> this pool is used with: EDD_NRT_FRAME_LLDP      */
/* RxItemsInMrpRing    : Specify no of RX-items for MRP frames.              */
/*                       --> this pool is used with: EDD_NRT_FRAME_LEN_STDBY */
/*                                                   EDD_NRT_FRAME_LEN_HSR   */
/*                                                   EDD_NRT_FRAME_MRP       */
/* RxItemsInMiscRing   : Specify no of RX-items for Tsync/FWLoader frames.   */
/*                       --> this pool is used with: EDD_NRT_FRAME_LEN_TSYNC */
/*                                           and EDD_NRT_FRAME_LEN_SINEC_FWL */
/* RxDropLogging       : EDD_FEATURE_ENABLE enables logging of dropping frames */
/*                       --> this feature is for testing/verification only!  */
/* RxFilterUDP_Unicast  : EDD_SYS_UDP_WHITELIST_FILTER_ON enables filtering  */
/*                        of UDP Unicast frames on reception by EDD          */
/* RxFilterUDP_Broadcast: EDD_SYS_UDP_WHITELIST_FILTER_ON enables filtering  */
/*                        of UDP Broadcast frames on reception by EDD        */

/* -------------------------------------------------------------------------- */
/* DPB: Device Parameter block                                                */
/* -------------------------------------------------------------------------- */

typedef struct _EDDP_GSHAREDMEM_TYPE
{
  LSA_UINT16    SRSHM[EDD_CFG_MAX_SRSHM_SIZE];
} EDDP_GSHAREDMEM_TYPE;
/* Shared memory */

typedef struct _EDDP_DPB_FEATURE_TYPE
{
    LSA_UINT8     bShortPreambleSupported;
    LSA_UINT8     bMRPDSupported;
    LSA_UINT8     FragmentationtypeSupported;
    LSA_UINT8     IRTForwardingModeSupported;
    LSA_UINT16    MaxDFPFrames;
    LSA_UINT8     AdditionalForwardingRulesSupported;   /* HSYNC mac supported as (static) mac address in MC table */
    LSA_UINT8     ApplicationExist;                     /* HSYNC APPLICATION SUPPORT */
    LSA_UINT8     ClusterIPSupport;                     // currently not used
    LSA_UINT8     MRPInterconnFwdRulesSupported;
    LSA_UINT8     MRPInterconnOriginatorSupported;
    LSA_UINT8     EIPExtensions;
    LSA_UINT8     DLRExtensions;
} EDDP_DPB_FEATURE_TYPE;

typedef struct _EDDP_DPB_TYPE    EDD_UPPER_MEM_ATTR *    EDDP_UPPER_DPB_PTR_TYPE;
typedef struct _EDDP_DPB_TYPE
{
    LSA_UINT32                          InterfaceID;
    LSA_UINT32                          TraceIdx;
    EDDP_SYS_HANDLE                     hSysDev;
    EDD_COMMON_MEM_PTR_TYPE             pPnipBase;
    EDD_COMMON_MEM_PTR_TYPE             pK32TCMBase;
    EDD_COMMON_MEM_PTR_TYPE             pK32ATCMBase;
    EDD_COMMON_MEM_PTR_TYPE             pK32BTCMBase;
    EDD_COMMON_MEM_PTR_TYPE             pK32DDR3Base;
    EDD_COMMON_MEM_PTR_TYPE             GSharedRAM_BaseAddr; 
    EDD_COMMON_MEM_PTR_TYPE             UserCmdSharedMem_BaseAddr; 
    LSA_UINT16                          K32TraceLevel;
    LSA_UINT16                          EDDPLocation;
    LSA_UINT16                          Hardware;
    LSA_UINT16                          HardwareIF;
    LSA_UINT16                          MaxPortCntOfAllEDD;
    LSA_UINT16                          MaxInterfaceCntOfAllEDD;
    EDDP_DPB_HW_CFG_TYPE                HWCfg;
    EDDP_DPB_CRT_TYPE                   CRT;
    EDDP_DPB_SWI_TYPE                   SWI;
    EDDP_DPB_NRT_TYPE                   NRT;
    EDDP_DPB_FEATURE_TYPE               FeatureSupport;
} EDDP_DPB_TYPE;
/* InterfaceID        :   InterfaceID of this Interface                 (in) */

#define EDDP_INTERFACE_ID_MIN  1
#define EDDP_INTERFACE_ID_MAX  16

/* TraceIdx           : Trace index useable with LSA-Trace              (in) */
/*                0   : 	no special index or unknown                      */
/*                1..x:	typically the EDD interface index                    */
/* Note: 	    This parameter will also be forwarded to the EDD User within */
/*              EDD_SRV_GET_PARAMS for use with indexed LSA-Traces           */

/* hSysDev            :   System Handle of Device                       (in) */
/* pPnipBase          :   Pointer to PNIP memory space (2MByte)         (in) */
/* pK32TCMBase        :   Pointer to KRISC32 TCM memory space           (in) */
/* pK32ATCMBase       :   Pointer to KRISC32 ATCM memory space (HERA)   (in) */
/* pK32BTCMBase       :   Pointer to KRISC32 BTCM memory space (HERA)   (in) */
/* pK32DDR3Base       :   Pointer to KRISC32 DDR3 memory space (HERA)   (in) */
/* GSharedRAM_BaseAddr:   Base Addr of 2proc shared mem (virt.)         (in) */
/*                              allocated memory must be                     */
/*                              sizeof(EDDP_GSHAREDMEM_TYPE)                 */
/* K32TraceLevel      :   Tracelevel for KRISC32 traces tranfered to    (in) */
/*                        EDDP.                                              */
/*                                                                           */
/*                        LSA_TRACE_LEVEL_OFF       : off (no traces)        */
/*                        LSA_TRACE_LEVEL_FATAL     : fatal errors           */
/*                        LSA_TRACE_LEVEL_ERROR     : errors                 */
/*                        LSA_TRACE_LEVEL_WARN      : warning                */
/*                        LSA_TRACE_LEVEL_UNEXP     : unexpected situations  */
/*                        LSA_TRACE_LEVEL_NOTE_HIGH : note high              */
/*                        LSA_TRACE_LEVEL_NOTE      : note                   */
/*                        LSA_TRACE_LEVEL_NOTE_LOW  : note low               */
/*                        LSA_TRACE_LEVEL_CHAT      : chat                   */
/*                                                                           */
/* EDDPLocation       :   Defines the location the EDDP is running      (in) */

#define EDDP_LOCATION_LOCAL       0  /* EDDP is running on local cpu (e.g. ARM) */
#define EDDP_LOCATION_EXT_HOST    1  /* EDDP is running on exernal HOST         */
                                     /* e.g. PC                                 */                            

/* Hardware           :   Defines the hardware the EDDP is running on   (in) */

#define EDDP_HW_ERTEC200P         0
#define EDDP_HW_ERTEC200P_FPGA    1
#define EDDP_HW_HERA              2
#define EDDP_HW_HERA_FPGA         3

/* Interface          :   Defines which Interface is used. (in)    */
/*                        Interface A and B from HERA can be used. */

#define EDDP_HW_IF_A                 1
#define EDDP_HW_IF_B                 2

/* MaxPortCntOfAllEDD :    Number of all interfaces of all EDDs         (in) */
/*                         max: EDD_CFG_MAX_INTERFACE_CNT                    */
/* MaxInterfaceCntOfAllEDD Number of all ports of all EDDs              (in) */
/*                         max: EDD_CFG_MAX_PORT_CNT_ALL_INTERFACE           */
/* HWCfg              : General hardware parameter for device           (in) */
/* CRT                : Subcomponent CRT                                (in) */
/* SWI                : Subcomponent SWI                                (in) */
/* NRT                : Subcomponent NRT                                (in) */


/* --------------------------------------------------------------------------*/

typedef struct _EDDP_RQB_DEV_OPEN_TYPE    EDD_UPPER_MEM_ATTR *    EDDP_UPPER_DEV_OPEN_PTR_TYPE;
typedef struct _EDDP_RQB_DEV_OPEN_TYPE
{
    EDDP_HANDLE                 hDDB;
    EDDP_UPPER_DPB_PTR_TYPE     pDPB;
    LSA_VOID                    LSA_FCT_PTR(EDD_UPPER_OUT_FCT_ATTR, Cbf)
                                           (EDD_UPPER_RQB_PTR_TYPE pRQB);
} EDDP_RQB_DEV_OPEN_TYPE;
/* hDDB               :   EDDP handle of Device                         (out)*/
/* pDPB               :   pointer to DPB                                (in) */
/* Cbf                :   Optional call back function                   (in) */

/*---------------------------------------------------------------------------*/
/* Opcode: EDD_OPC_SYSTEM   Service: EDDP_SRV_DEV_SETUP                      */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Opcode: EDD_OPC_SYSTEM   Service: EDDP_SRV_DEV_IF_SETUP                   */
/*---------------------------------------------------------------------------*/

/* --------------------------------------------------------------------------*/
/* DSB: Port Params Configuration                                            */
/* --------------------------------------------------------------------------*/

typedef struct _EDDP_DSB_PORT_PARAMS_TYPE
{
    EDD_MAC_ADR_TYPE        Port_MAC_Address;
    LSA_UINT8               IsWireless;
    LSA_UINT8               IsMDIX;
    LSA_UINT8               IsPulled;
    LSA_UINT8               GBitSupport;
} EDDP_DSB_PORT_PARAMS_TYPE;
/* Port_MAC_Address  : Port specific MAC address                        (in) */
/* GBitSupport       : Feature of HERA                                       */ 
/*             Values: EDD_FEATURE_ENABLE                                    */
/*                     EDD_FEATURE_DISABLE                                   */

/* --------------------------------------------------------------------------*/
/* DSB: Interrupt-Configuration                                              */
/* --------------------------------------------------------------------------*/

typedef struct _EDDP_DSB_INTERRUPT_HANDLING_TYPE       EDDP_LOCAL_MEM_ATTR *    EDDP_DSB_INTERRUPT_HANDLING_PTR_TYPE;
typedef struct _EDDP_DSB_INTERRUPT_HANDLING_TYPE
{
    LSA_UINT32      EOI_WaitTime;
    LSA_UINT32      IRQ_WaitTime_AcycRcv;
} EDDP_DSB_INTERRUPT_HANDLING_TYPE;
/* EOI_WaitTime : Wait time when next group interrupt may occur.            */
/*                Actual time value = EOI_WaitTime x 1024ns                 */ 
/*                Max: ~16,8ms -> EOI_WaitTime = 0x3FFF                     */
/* IRQ_WaitTime_AcycRcv : Wait time when next Acyc-receive interrupt may occur.     */
/*                        Actual time value = IRQ_WaitTime_AcycRcv x 32,768us       */
/*                        Max: ~16,7ms -> IRQ_WaitTime_AcycRcv = 0x1FF              */

typedef struct _EDDP_DSB_TYPE    EDD_UPPER_MEM_ATTR *    EDDP_UPPER_DSB_PTR_TYPE;
typedef struct _EDDP_DSB_TYPE
{
    LSA_UINT32                          physicalK32PNIPBase;        // physical PNIP base address in K32 system 
	LSA_UINT32                          physicalPNIPK32DTCMBase;       // physical K32 DTCM (or BTCM) base address in PNIP system 
    EDDP_DSB_PORT_PARAMS_TYPE           PortParams[EDDP_MAX_PORT_CNT];
    EDDP_DSB_INTERRUPT_HANDLING_TYPE    InterruptHandling;
    EDD_MAC_ADR_TYPE                    IF_MAC_Address;
} EDDP_DSB_TYPE;
/* PortParams   : Port parameters (the index into the array is the UserPortID).         */
/*                Which port is actualle mapped to a interface logical ports is         */
/*                defined by the portmapping within DPB.SWI.PortMap                     */


typedef struct _EDDP_RQB_DEV_SETUP_TYPE  EDD_UPPER_MEM_ATTR *    EDDP_UPPER_DEV_SETUP_PTR_TYPE;
typedef struct _EDDP_RQB_DEV_SETUP_TYPE
{
    EDDP_HANDLE                 hDDB;
    EDDP_UPPER_DSB_PTR_TYPE     pDSB;
    LSA_VOID                    LSA_FCT_PTR(EDD_UPPER_OUT_FCT_ATTR, Cbf)
                                           (EDD_UPPER_RQB_PTR_TYPE pRQB);
} EDDP_RQB_DEV_SETUP_TYPE;


/*---------------------------------------------------------------------------*/
/* Opcode: EDD_OPC_SYSTEM   Service: EDDP_SRV_DEV_SHUTDOWN                    */
/*---------------------------------------------------------------------------*/

typedef struct _EDDP_RQB_DEV_SHUTDOWN_TYPE   EDD_UPPER_MEM_ATTR *    EDDP_UPPER_DEV_SHUTDOWN_PTR_TYPE;
typedef struct _EDDP_RQB_DEV_SHUTDOWN_TYPE
{
    EDDP_HANDLE     hDDB;
    LSA_VOID        LSA_FCT_PTR(EDD_UPPER_OUT_FCT_ATTR, Cbf)
                               (EDD_UPPER_RQB_PTR_TYPE pRQB);
} EDDP_RQB_DEV_SHUTDOWN_TYPE;


/*---------------------------------------------------------------------------*/
/* Opcode: EDD_OPC_SYSTEM   Service: EDDP_SRV_DEV_CLOSE                      */
/*---------------------------------------------------------------------------*/

typedef struct _EDDP_RQB_DEV_CLOSE_TYPE    EDD_UPPER_MEM_ATTR *    EDDP_UPPER_DEV_CLOSE_PTR_TYPE;
typedef struct _EDDP_RQB_DEV_CLOSE_TYPE
{
    EDDP_HANDLE     hDDB;
    LSA_VOID        LSA_FCT_PTR(EDD_UPPER_OUT_FCT_ATTR, Cbf)
                               (EDD_UPPER_RQB_PTR_TYPE pRQB);
} EDDP_RQB_DEV_CLOSE_TYPE;



/*===========================================================================*/
/*======================= INTERNAL & SERVICES   =============================*/
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* Filled and used edd internal!!!                                           */
/*---------------------------------------------------------------------------*/
/* Opcode: EDD_OPC_SYSTEM   Service: EDDP_SRV_DEV_INTERRUPT                  */
/*                          Service: EDDP_SRV_DEV_TIMER                      */
/*                          Service: EDDP_SRV_DEV_TRIGGER                    */
/*---------------------------------------------------------------------------*/

typedef struct _EDDP_RQB_DEV_INTERNAL_TYPE   EDD_UPPER_MEM_ATTR *    EDDP_UPPER_DEV_INTERNAL_PTR_TYPE;
typedef struct _EDDP_RQB_DEV_INTERNAL_TYPE
{
    EDDP_HANDLE     hDDB;
    LSA_UINT32      UserID;
    LSA_UINT32      Param;
} EDDP_RQB_DEV_INTERNAL_TYPE;
/* EDDP_SRV_DEV_INTERRUPT (Param not used)              */
/* UserID   : Sub-ID                                    */
/* Param    : service-dependend parameter               */

#define EDDP_ISR_RQB_USER_ID_GROUP      0
#define EDDP_ISR_RQB_USER_ID_SINGLE     2

/* param: 0 for Group                       */
/*        2..15 for Single                  */

/* EDDP_SRV_DEV_TIMER (Param used for EDDPTimerID!) */
/* UserIDs:                                         */

#define EDDP_TIMER_BLINK                    0
#define EDDP_TIMER_FRAME_DROP               1
#define EDDP_TIMER_PPM_HOL_NOT_EMPTY        2
#define EDDP_TIMER_PPM_NOT_FINISHED         3
#define EDDP_TIMER_INBOUND_LATE_ERR         4
#define EDDP_TIMER_WRKAR_PACKGRP_DISABLE    5

/* EDDP_SRV_DEV_TRIGGER  */
/* UserIDs:              */

#define EDDP_TRIGGER_xxxx    0      /*                             */


/*===========================================================================*/
/*                   CDB (Channel Description Block)                         */
/*                     (used by EDDP_GET_PATH_INFO)                          */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* CDB- Parameter for NRT                                                    */
/*---------------------------------------------------------------------------*/

typedef struct _EDDP_CDB_NRT_TYPE    EDD_UPPER_MEM_ATTR * EDDP_UPPER_CDB_NRT_PTR_TYPE;
typedef struct _EDDP_CDB_NRT_TYPE
{
    LSA_UINT32      FrameFilter;
} EDDP_CDB_NRT_TYPE;
/* FrameFilter  : defines NRT-Frames to be receive for this channel.        */
/*                Can be ORed. see EDD_SRV_NRT_RECV in <edd_usr.h>          */


/*---------------------------------------------------------------------------*/
/* CDB- Parameter for CRT                                                    */
/*---------------------------------------------------------------------------*/

typedef struct _EDDP_CDB_CRT_TYPE   EDD_UPPER_MEM_ATTR *    EDDP_UPPER_CDB_CRT_PTR_TYPE;
typedef struct _EDDP_CDB_CRT_TYPE
{
    LSA_UINT8       Dummy;
} EDDP_CDB_CRT_TYPE;


/*---------------------------------------------------------------------------*/
/* CDB- Parameter for PRM                                                    */
/*---------------------------------------------------------------------------*/

typedef struct _EDDP_CDB_PRM_TYPE EDD_UPPER_MEM_ATTR * EDDP_UPPER_CDB_PRM_PTR_TYPE;
typedef struct _EDDP_CDB_PRM_TYPE
{
    LSA_UINT8       PortDataAdjustLesserCheckQuality;
} EDDP_CDB_PRM_TYPE;
/*---------------------------------------------------------------------------*/
/* PortDataAdjustLesserCheckQuality:                                         */
/*                                                                           */
/* EDD_SYS_PDPORTDATAADJUST_LESSER_CHECK_QUALITY: Do a lesser strict         */
/*                                                check (see table below)    */
/* EDD_SYS_PDPORTDATAADJUST_CHECK_FOR_PNIO_STANDARD:  No special handling.   */
/*                                                                           */
/* PDPortDataAdjust checking rules:                                          */
/*                                                                           */
/* --------+--------------------+----------------------+-------------------- */
/*  PRM    |     IEC61158       |  LesserCheckQuality  |     Local-Flag      */
/* --------+--------------------+----------------------+-------------------- */
/*  WRITE  |                         PortState = OFF                         */
/*         |                                                                 */
/*         |                             100 FD                              */
/*         |                           Autoneg 1)                            */
/*         |                 (PNIP supports 100 FD only!!)                   */
/* --------+--------------------+----------------------+-------------------- */
/*  END    | At least one port has to be set to        |  no restrictions    */
/*         | Autoneg or 100 FD.                        |                     */
/* --------+-------------------------------------------+-------------------- */
/*                                                                           */
/* IEC61158           : Checking rule according to IEC61158.                 */
/* LesserCheckQuality : Rule if PortDataAdjustLesserCheckQuality is set      */
/* Local-Flag         : Rule if "Local"  flag set with PRM-request by CMPD.  */
/*                                                                           */
/*  1) Autoneg is used if no MAUType or PortState is present for this port.  */
/*     This is the default setting.                                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* CDB- Channel Description Block                                            */
/*      (used as detail-ptr in EDD_GET_PATH_INFO)                            */
/*                                                                           */
/*---------------------------------------------------------------------------*/

typedef struct _EDDP_COMP_USE_TYPE
{
    LSA_UINT8    UseNRT;        /*EDD_CDB_CHANNEL_USE_ON: NRT-Services  used by channel */
    LSA_UINT8    UseCRT;        /*EDD_CDB_CHANNEL_USE_ON: CRT-Services  used by channel */
    LSA_UINT8    UsePRM;        /*EDD_CDB_CHANNEL_USE_ON: PRM-Services  used by channel */
    LSA_UINT8    UseK32;        /*EDD_CDB_CHANNEL_USE_ON: K32-Services  used by channel */
} EDDP_COMP_USE_TYPE;

typedef struct eddp_detail_tag
{
    EDDP_COMP_USE_TYPE              UsedComp;
    EDDP_HANDLE                     hDDB;
    EDDP_UPPER_CDB_NRT_PTR_TYPE     pNRT;   /* required if "UseNRT" */
    EDDP_UPPER_CDB_PRM_PTR_TYPE     pPRM;  
    EDDP_UPPER_CDB_CRT_PTR_TYPE     pCRT;
} EDDP_CDB_TYPE;

typedef EDDP_DETAIL_PTR_TYPE EDDP_SYSTEM_CDB_PTR_TYPE;

/*===========================================================================*/
/*                                 macros                                    */
/*===========================================================================*/


/*===========================================================================*/
/*                              external functions                           */
/*===========================================================================*/


/*===========================================================================*/
/*                              local functions                              */
/*===========================================================================*/

/*====  in functions  =====*/

/*=============================================================================
 * function name:  eddp_init
 *
 * function:       initialize EDD
 *
 * parameters:     ....
 *
 * return value:   LSA_RESULT:  LSA_RET_OK
 *                              possibly:
 *                              LSA_RET_ERR_PARAM
 *                              LSA_RET_ERR_RESOURCE
 *                              or others
 *===========================================================================*/

EDD_RSP  EDDP_SYSTEM_IN_FCT_ATTR eddp_init(LSA_VOID);

/*=============================================================================
 * function name:  eddp_undo_init
 *
 * function:       undo the initialization of EDD
 *
 * parameters:     LSA_VOID
 *
 * return value:   LSA_RESULT:  LSA_RET_OK
 *                              possibly:
 *                              LSA_RET_ERR_SEQUENCE
 *                              or others
 *===========================================================================*/

EDD_RSP  EDDP_SYSTEM_IN_FCT_ATTR  eddp_undo_init(LSA_VOID);

/*=============================================================================
 * function name:  eddp_system
 *
 * function:       send a request to a communication channel
 *
 * parameters:     EDD_UPPER_RQB_PTR_TYPE  pRQB:  pointer to Upper-RQB
 *
 * return value:   LSA_VOID
 *
 * RQB-parameters: 
 *      RQB-header:
 *      LSA_OPCODE_TYPE         Opcode:     EDD_OPC_SYSTEM
 *      LSA_HANDLE_TYPE         Handle:     channel-handle of edd
 *      LSA_USER_ID_TYPE        UserId:     ID of user
 *      LSA_COMP_ID_TYPE        CompId:     Cmponent-ID
 *      EDD_SERVICE             Service:    EDDP_SRV_*
 *      LSA_RESULT              Response:
 *      EDD_UPPER_MEM_PTR_TYPE  pParam:     Depend on kind of request.
 *     
 * RQB-return values via callback-function:
 *      LSA_OPCODE_TYPE         opcode:     EDD_OPC_REQUEST
 *      LSA_HANDLE_TYPE         Handle:     channel-handle of edd
 *      LSA_USER_ID_TYPE        UserId:     ID of user
 *      LSA_COMP_ID_TYPE        CompId:
 *      EDD_SERVICE             Service:
 *      LSA_RESULT              Response:   EDD_STS_OK
 *                                          EDD_STS_ERR_PARAM 
 *                                          EDD_STS_ERR_SEQUENCE
 *                                          or others
 *
 *     All other RQB-parameters depend on kind of request.
 *===========================================================================*/

LSA_VOID EDD_UPPER_IN_FCT_ATTR eddp_system(
    EDD_UPPER_RQB_PTR_TYPE      pRQB);


/*=============================================================================
 * function name:  eddp_open_channel
 *
 * function:       open a communication channel
 *
 * parameters:     EDD_UPPER_RQB_PTR_TYPE  pRQB:  pointer to Upper-RQB
 *
 * return value:   LSA_VOID
 *
 * RQB-Header:
 *      LSA_OPCODE_TYPE         Opcode:         EDD_OPC_OPEN_CHANNEL
 *      LSA_HANDLE_TYPE         Handle:         channel-handle of user
 *      LSA_USER_ID_TYPE        UserId:         Id of user
 *      LSA_COMP_ID_TYPE        CompId:         Cmponent-ID
 *      EDD_SERVICE             Service:
 *      LSA_RESULT              Response:
 * RQB-Parameter:
 *      LSA_HANDLE_TYPE         HandleUpper:    channel-handle of user
 *      LSA_SYS_PATH_TYPE       SysPath:        system-path of channel                 
 *      LSA_VOID                LSA_FCT_PTR(EDD_UPPER_OUT_FCT_ATTR, Cbf)
 *                                         (EDD_UPPER_RQB_PTR_TYPE pRQB)
 *                                              callback-function
 *
 * RQB-return values via callback-function:
 *      LSA_HANDLE_TYPE         Handle:         channel-handle of edd
 *      LSA_RESULT              Response:       EDD_STS_OK
 *                                              EDD_STS_ERR_PARAM 
 *                                              EDD_STS_ERR_SEQUENCE
 *                                              or others
 *     All other RQB-parameters are unchanged.
 *===========================================================================*/

LSA_VOID EDD_UPPER_IN_FCT_ATTR eddp_open_channel(
    EDD_UPPER_RQB_PTR_TYPE      pRQB);

/*=============================================================================
 * function name:  eddp_close_channel
 *
 * function:       close a communication channel
 *
 * parameters:     EDD_UPPER_RQB_PTR_TYPE  pRQB:  pointer to Upper-RQB
 *
 * return value:   LSA_VOID
 *
 * RQB-Header:
 *      LSA_OPCODE_TYPE         Opcode:         EDD_OPC_CLOS_CHANNEL
 *      LSA_HANDLE_TYPE         Handle:         channel-handle of user
 *      LSA_USER_ID_TYPE        UserId:         Id of user
 *      LSA_COMP_ID_TYPE        CompId:         Cmponent-ID
 *      EDD_SERVICE             Service:
 *      LSA_RESULT              Response:
 *
 * RQB-return values via callback-function:
 *      LSA_HANDLE_TYPE         Handle:         HandleUpper from Open-Request
 *      LSA_RESULT              Response:       EDD_STS_OK
 *                                              EDD_STS_ERR_PARAM 
 *                                              EDD_STS_ERR_SEQUENCE
 *                                              or others
 *
 *     All other RQB-parameters are unchanged.
 *===========================================================================*/

LSA_VOID EDD_UPPER_IN_FCT_ATTR eddp_close_channel(
    EDD_UPPER_RQB_PTR_TYPE      pRQB);


/*=============================================================================
 * function name:  eddp_request
 *
 * function:       send a request to a communication channel
 *
 * parameters:     EDD_UPPER_RQB_PTR_TYPE  pRQB:  pointer to Upper-RQB
 *
 * return value:   LSA_VOID
 *
 * RQB-parameters: 
 *      RQB-header:
 *      LSA_OPCODE_TYPE         Opcode:     EDD_OPC_REQUEST
 *      LSA_HANDLE_TYPE         Handle:     channel-handle of edd
 *      LSA_USER_ID_TYPE        UserId:     ID of user
 *      LSA_COMP_ID_TYPE        CompId:     Cmponent-ID
 *      EDD_SERVICE             Service:    EDD_SRV_*
 *      LSA_RESULT              Response:
 *      EDD_UPPER_MEM_PTR_TYPE  pParam:     Depend on kind of request.
 *     
 * RQB-return values via callback-function:
 *      LSA_OPCODE_TYPE         opcode:     EDD_OPC_REQUEST
 *      LSA_HANDLE_TYPE         Handle:     HandleUpper from Open-Request
 *      LSA_USER_ID_TYPE        UserId:     ID of user
 *      LSA_COMP_ID_TYPE        CompId:
 *      EDD_SERVICE             Service:
 *      LSA_RESULT              Response:   EDD_STS_OK
 *                                          EDD_STS_ERR_PARAM 
 *                                          EDD_STS_ERR_SEQUENCE
 *                                          or others
 *
 *     All other RQB-parameters depend on kind of request.
 *===========================================================================*/

LSA_VOID EDD_UPPER_IN_FCT_ATTR eddp_request(
    EDD_UPPER_RQB_PTR_TYPE      pRQB);

/*=============================================================================
 * function name:  eddp_timeout
 *
 * function:       expiration of the running time of a timer
 *
 * parameters:	   LSA_TIMER_ID_TYPE timer_id:  id of timer
 *				   LSA_USER_ID_TYPE  user_id:   id of prefix
 *				   
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID EDDP_SYSTEM_IN_FCT_ATTR eddp_timeout(
	LSA_TIMER_ID_TYPE       timer_id,
	LSA_USER_ID_TYPE        user_id);


/*=============================================================================
 * function name:  eddp_interrupt
 *
 * function:       Edd-ISR-Handler called in isr 
 *
 * parameters:     EDD_HANDLE           hDDB
 *                 LSA_UINT32           IntSource
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID EDDP_SYSTEM_IN_FCT_ATTR eddp_interrupt(
	EDDP_HANDLE     hDDB,
    LSA_UINT32      IntSource);

/* IntSource */
#define  EDDP_INT_SOURCE_GROUP           0    /* group interrupt        */
                                              /* 2..15 single interrupt */
  
/*=============================================================================
 * function name:  eddp_ll_PhyRegisterWrite
 *
 * function:       Writes 'Value' to PHY register 'RegisterNumber' on Port
 *				  'HwPortID'.
 *
 * parameters:     EDDP_HANDLE		hDDB
 *				   LSA_UINT32		HwPortID
 *				   LSA_UINT32		RegisterNumber
 *				   LSA_UINT16		Value 
 *
 * return value:   EDD_RSP
 *===========================================================================*/
EDD_RSP EDDP_LOCAL_FCT_ATTR eddp_ll_PhyRegisterWrite(
    EDDP_HANDLE     hDDB,
    LSA_UINT32		HwPortID,
    LSA_UINT32		RegisterNumber,
    LSA_UINT32		Value);

/*=============================================================================
 * function name:  eddp_ll_PhyRegisterRead
 *
 * function:       Reads PHY register 'RegisterNumber' on Port 'HwPortID'	
 *				   into *pValue.
 *
 * parameters:     EDDP_HANDLE		hDDB
 *				   LSA_UINT32		HwPortID
 *				   LSA_UINT32		RegisterNumber
 *				   LSA_UINT16	   *pValue 
 *
 * return value:   EDD_RSP
 *===========================================================================*/
EDD_RSP EDDP_LOCAL_FCT_ATTR eddp_ll_PhyRegisterRead(
    EDDP_HANDLE     hDDB,
    LSA_UINT32		HwPortID,
    LSA_UINT32		RegisterNumber,
    LSA_UINT32	   *pValue);



/*=============================================================================
 * function name:  eddp_InitDPBWithDefaults
 *
 * function:       Write default settings into pDPB
 *
 * parameters:     EDDP_UPPER_DPB_PTR_TYPE		pDPB
 *
 * return value:   EDD_RSP
 *===========================================================================*/
EDD_RSP EDDP_LOCAL_FCT_ATTR eddp_InitDPBWithDefaults(
    EDDP_UPPER_DPB_PTR_TYPE     pDPB,
    LSA_UINT16                  Hardware,
    LSA_UINT16                  EDDPLocation,
    LSA_UINT16                  Interface,
    EDD_UPPER_SWICHAR_PTR_TYPE  pSwiChar);


/*=============================================================================
 * function name:  eddp_InitDSBWithDefaults
 *
 * function:       Write default settings into pDSB	
 *
 * parameters:     EDDP_UPPER_DPB_PTR_TYPE		pDPB
 *                 EDDP_UPPER_DSB_PTR_TYPE		pDSB
 *
 * return value:   EDD_RSP
 *===========================================================================*/
EDD_RSP EDDP_LOCAL_FCT_ATTR eddp_InitDSBWithDefaults(
    EDDP_UPPER_DPB_PTR_TYPE     pDPB,
    EDDP_UPPER_DSB_PTR_TYPE     pDSB);


/*=============================================================================
 * function name:  eddp_SetIRQ_WaitTime_to_MAX
 *
 * function:       Sets IRQ_WaitTime for single interrupts to maximum value.	
 *
 * parameters:     EDDP_HANDLE             hDDB,
 *                 LSA_UINT32              IRQ_Number
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID EDDP_LOCAL_FCT_ATTR eddp_SetIRQ_WaitTime_to_MAX(
    EDDP_HANDLE     hDDB,
    LSA_UINT32      IRQ_Number);


/*=============================================================================
 * function name:  eddp_K32GetData
 *
 * function:       Get pointer and length of KRISC32 internal data.
 *
 * parameters:     EDDP_HANDLE				hDDB,
 *                 LSA_UINT8**				pData,
 *                 LSA_INT*					pLength
 *
 * return value:   EDD_RSP
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_IN_FCT_ATTR eddp_K32GetData(
	EDDP_HANDLE         hDDB,
	LSA_UINT8**			pData,
	LSA_INT*			pLength);
/*=============================================================================
 * function name:  eddp_GetSyncTime
 *
 * function:       Get current PNIO TimeStamp, UTCOffset and SyncState
 *                 Read PTCP time from PN-IP register OCT_Time
 *                 Read EpochNumber, UTCOffset and SyncState from KRISC32 module
 *                 The function may be called from interrupt context
 *
 * parameters:     EDDP_HANDLE                      hDDB
 *                 EDDP_TIMESTAMP_INFO_PTR_TYPE     pTimestampInfo
 *
 * return value:   EDD_RSP
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_IN_FCT_ATTR eddp_GetSyncTime (
    EDDP_HANDLE						hDDB,
    EDDP_TIMESTAMP_INFO_PTR_TYPE	pTimestampInfo);

/*=============================================================================
 *+                                                                         
 *+  Functionname:    eddp_SysRed_ShM_Array_Get                        
 *+  function:        read a value from the sr shared memory array element.
 *+                   the index of the element in the array is ShMIndex.
 *+  parameters:      EDDP_HANDLE         hDDB 
 *+                   LSA_UINT16          ShMIndex 
 *+                                         Range: 
 *+                                             0... EDD_CFG_MAX_SRSHM_SIZE-1 
 *+                   LSA_UINT16*         pShMValue: pointer to where, 
 *+                                                  the read value shall be
 *+                                                  written to.
 *+  return value:    EDD_RSP                                     
 *===========================================================================*/

EDD_RSP eddp_SysRed_ShM_Array_Get (
    EDDP_HANDLE         hDDB,
    LSA_UINT16          const ShMIndex,
    LSA_UINT16*         const pShMValue);
 

/*=========================================================================================================
 *+
 *+  Functionname:    eddp_GetMIB2Counters 
 *+  function:        read or read+reset synchronously MIB2 counters for this instance.
 *+  parameters:      EDDP_HANDLE                              hDDB 
 *+                   LSA_UINT16                               PortID: Port number (0= local interface,
 *+                                                            1 = first port, 2 = second) 
 *+                                                               Range: 
 *+                                                                   0... MaxportCnt 
 *+                   LSA_UINT8                                Reset
 *+                                                               Possible Values:
 *+                                                                  EDD_STATISTICS_RESET_VALUES
 *+                                                                  EDD_STATISTICS_DO_NOT_RESET_VALUES
 *+                   EDD_UPPER_GET_STATISTICS_MIB_PTR_TYPE    pMIB
 *+  return value:    EDD_RSP                                     
 *=======================================================================================================*/


EDD_RSP eddp_GetMIB2Counters (
        EDDP_HANDLE                           const hDDB,
        LSA_UINT32                            const PortID,
        LSA_UINT8                             const Reset,
        EDD_UPPER_GET_STATISTICS_MIB_PTR_TYPE const pMIB);

/*****************************************************************************/
/*                                                                           */
/* Output-functions are in eddp_out.h                                        */
/*                                                                           */
/*****************************************************************************/



/*****************************************************************************/
/*                                                                           */
/* Typedefs and defines for PHY interface                                    */
/*                                                                           */
/*****************************************************************************/


#define EDDP_LED_MODE_ON        1 
#define EDDP_LED_MODE_OFF       0 
/* EDDP_LED_MODE_ON / EDDP_LED_MODE_OFF are used by service EDD_SRV_LED_BLINK */


typedef struct _EDDP_PHY_LINK_STATUS_TYPE   EDDP_LOWER_MEM_ATTR         *   EDDP_PHY_LINK_STATUS_PTR_TYPE;
typedef struct _EDDP_PHY_LINK_STATUS_TYPE   EDDP_LOWER_MEM_ATTR const   *   EDDP_PHY_LINK_STATUS_PTR_CONST_TYPE;
typedef struct _EDDP_PHY_LINK_STATUS_TYPE
{
    LSA_UINT32          Link;
    LSA_UINT32          Speed;
    LSA_UINT32          Duplexity;
    LSA_UINT16          MAUType;
    LSA_UINT8           MediaType;
    LSA_UINT8           OpticalTransType;
    LSA_UINT8           FxTransceiverType;
    LSA_UINT8           LinkSpeedModeConfigured;
    LSA_UINT32          AutonegCapAdvertised;
    LSA_UINT32          RealPortTxDelay;
    LSA_UINT32          RealPortRxDelay;
    LSA_UINT32          MaxPortTxDelay;
    LSA_UINT32          MaxPortRxDelay;
    LSA_UINT8           IsWireless;
    LSA_UINT8           IsMDIX;
} EDDP_PHY_LINK_STATUS_TYPE;

/* ----------------------------------------------------------------------------------------------------------*/
/*      Link                    : OUT: PHY register containing link information                              */
/*                                IN : Link status (EDD_LINK_UP, EDD_LINK_DOWN)                              */
/*      Speed                   : OUT: PHY register containing speed information                             */
/*                                IN : Current speed status (e.g. EDD_LINK_SPEED_100)                        */
/*      Duplexity               : OUT: PHY register containing duplexity information                         */
/*                                IN : Current duplexity status ( EDD_LINK_MODE_FULL, EDD_LINK_MODE_HALF)    */
/*      MAUType                 : Current MAUType                                                            */
/*      MediaType               : Current MediaType                                                          */
/*      LinkSpeedModeConfigured : Link speed and link mode configured in the phy, it usually corresponds     */
/*                                to the input parameter below Speed, Duplexity and Autoneg, just in case    */
/*                                of POF media, no auto-negotiation is supported, therefore                  */
/*                                pLinkSpeedModeConfigured refers to no autoneg modes                        */
/*      AutonegCapAdvertised    : Current link capabilites which are advertised by transceiver               */
/*      RealPortTxDelay         : real Transmit port delay in nano seconds (ns)                              */
/*      RealPortRxDelay         : real Receive port delay in nano seconds (ns)                               */
/*      MaxPortTxDelay          : maximum Transmit port delay in nano seconds (ns) - only for checking in Prm*/
/*      MaxPortRxDelay          : maximum Receive port delay in nano seconds (ns) - only for checking in Prm */
/*      IsWireless              : Is this port a wireless port? (EDD_PORT_IS_NOT_WIRELESS / EDD_PORT_IS_WIRELESS) */
/*      IsMDIX                  : Is this a "Crossed Port" ? (EDD_PORT_MDIX_DISABLED / EDD_PORT_MDIX_ENABLED)     */
/* ---------------------------- ---------------------------------------------------------------------------- */

typedef struct _EDDP_PHY_INIT_TYPE  EDDP_LOWER_MEM_ATTR *   EDDP_PHY_INIT_PTR_TYPE;
typedef struct _EDDP_PHY_INIT_TYPE
{
        LSA_UINT32 LinkStatusRegAddress;
        LSA_UINT32 LinkStatusBitMask;
        LSA_UINT32 SpeedRegAddress;
        LSA_UINT32 SpeedBitMask;
        LSA_UINT32 DuplexityRegAddress;
        LSA_UINT32 DuplexityBitMask;
} EDDP_PHY_INIT_TYPE;
/* -------------------------------------------------------------------------------------------------------- */
/*      LinkStatusRegAddress            : PHY register address that contains link status information        */
/*      LinkStatusBitMask               : Bit mask to mark relevant bits for link status information        */
/*      SpeedRegAddress                 : PHY register address that contains speed status information       */
/*      SpeedBitMask                    : Bit mask to mark relevant bits for speed status information       */
/*      DuplexityRegAddress             : PHY register address that contains duplexity status information   */
/*      DuplexityBitMask                : Bit mask to mark relevant bits for duplexity status information   */
/* -------------------------------------------------------------------------------------------------------- */

typedef enum _EDDP_PHY_TYPE
{
    EDDP_PHY_TYPE_UNKNOWN = 0,
    EDDP_PHY_TYPE_NSC,
    EDDP_PHY_TYPE_NEC,
    EDDP_PHY_TYPE_BCM5461S,
    EDDP_PHY_TYPE_TI,
	EDDP_PHY_TYPE_RZT2
} EDDP_PHY_TYPE;

typedef struct _EDDP_PHY_FCT_TYPE   EDDP_LOWER_MEM_ATTR *   EDDP_PHY_FCT_PTR_TYPE;
typedef struct _EDDP_PHY_FCT_TYPE
{               
    EDD_RSP LSA_FCT_PTR(EDDP_LOWER_OUT_FCT_ATTR, EDDP_PHY_Init)(
                        EDDP_HANDLE             hDDB,
                        EDDP_SYS_HANDLE         hSysDev,        
	                    LSA_UINT32              Hardware,
                        LSA_UINT32              HwPortID,
                        LSA_UINT8  	            IsWireless,
                        LSA_UINT8  	            IsMIDX,
                        EDDP_PHY_INIT_PTR_TYPE  pInitData);

	EDD_RSP LSA_FCT_PTR(EDDP_LOWER_OUT_FCT_ATTR, EDDP_PHY_LoadDelayValues)(
						EDDP_HANDLE				        hDDB,
	                    LSA_UINT32                      Hardware,
                        EDDP_PHY_LINK_STATUS_PTR_TYPE   pLinkStatus);

    EDD_RSP LSA_FCT_PTR(EDDP_LOWER_OUT_FCT_ATTR, EDDP_PHY_GetLinkStatus)(
						EDDP_HANDLE				        hDDB,
						EDDP_SYS_HANDLE			        hSysDev,
	                    LSA_UINT32                      Hardware,
                        LSA_UINT32				        HwPortID,
                        EDDP_PHY_LINK_STATUS_PTR_TYPE   pLinkStatus);

    EDD_RSP LSA_FCT_PTR(EDDP_LOWER_OUT_FCT_ATTR, EDDP_PHY_SetPowerDown)(
                        EDDP_HANDLE             hDDB,
                        EDDP_SYS_HANDLE         hSysDev,
                        LSA_UINT32              HwPortID, 
                        LSA_UINT8               PowerDown );

    EDD_RSP LSA_FCT_PTR(EDDP_LOWER_OUT_FCT_ATTR, EDDP_PHY_CheckPowerDown)(
                        EDDP_HANDLE             hDDB,
                        EDDP_SYS_HANDLE         hSysDev,
                        LSA_UINT32              HwPortID,
                        LSA_UINT8 *             pIsPowerDown);

    EDD_RSP LSA_FCT_PTR(EDDP_LOWER_OUT_FCT_ATTR, EDDP_PHY_SetSpeedDuplexityOrAutoneg )(
                        EDDP_HANDLE             hDDB,
                        EDDP_SYS_HANDLE         hSysDev,
                        LSA_UINT32              HwPortID,
                        LSA_UINT8               *pLinkSpeedModeConfigured,
                        LSA_UINT32              Speed,
                        LSA_UINT32              Duplexity,
                        LSA_UINT8               Autoneg );
/* Autoneg uses EDD_AUTONEG_ON/EDD_AUTONEG_OFF */

    EDD_RSP LSA_FCT_PTR(EDDP_LOWER_OUT_FCT_ATTR, EDDP_PHY_CheckMAUType )(
                        EDDP_HANDLE             hDDB,
                        EDDP_SYS_HANDLE         hSysDev,
                        LSA_UINT32              HwPortID,
                        LSA_UINT16              MAUType,
                        LSA_UINT32               *pSpeed,
                        LSA_UINT32               *pDuplexity );    
    
    LSA_VOID LSA_FCT_PTR(EDDP_LOWER_OUT_FCT_ATTR, EDDP_PHY_Get_Media_Type )(
                        EDDP_HANDLE             hDDB,
                        EDDP_SYS_HANDLE         hSysDev,
                        LSA_UINT32              HwPortID,
                        LSA_UINT8               *pMediaType,
                        LSA_UINT8               *pOpticalTransType,
                        LSA_UINT8               *pFxTransceiverType);

    LSA_VOID LSA_FCT_PTR(EDDP_LOWER_OUT_FCT_ATTR, EDDP_PHY_Get_MAU_Type )(
                        EDDP_HANDLE             hDDB,
                        EDDP_SYS_HANDLE         hSysDev,
                        LSA_UINT32              HwPortID,
                        LSA_UINT32              Speed,
                        LSA_UINT32              Duplexity,
                        LSA_UINT16              *pMauType);

    EDD_RSP LSA_FCT_PTR(EDDP_LOWER_OUT_FCT_ATTR, EDDP_PHY_LED_BlinkBegin)(
                        EDDP_HANDLE             hDDB,
                        EDDP_SYS_HANDLE         hSysDev,        
                        LSA_UINT32              HwPortID );

    EDD_RSP LSA_FCT_PTR(EDDP_LOWER_OUT_FCT_ATTR, EDDP_PHY_LED_BlinkSetMode)(
                        EDDP_HANDLE             hDDB,
                        EDDP_SYS_HANDLE         hSysDev,        
                        LSA_UINT32              HwPortID,
                        LSA_UINT32              LEDMode );

    EDD_RSP LSA_FCT_PTR(EDDP_LOWER_OUT_FCT_ATTR, EDDP_PHY_LED_BlinkEnd)(
                        EDDP_HANDLE             hDDB,
                        EDDP_SYS_HANDLE         hSysDev,        
                        LSA_UINT32              HwPortID );
} EDDP_PHY_FCT_TYPE;

#define EDDP_PHY_POWERUP            0
#define EDDP_PHY_POWERDOWN          1
#define EDDP_PHY_CLOSE          	2

/*=============================================================================
 * function name:  eddp_SRTProviderGetCycleCounter
 *
 * function:       Get SRT-provider CycleCounter (31.25us ticks)
 *
 * parameters:     EDDP_HANDLE                      hDDB
 *                 EDDP_CYCLE_COUNTER_PTR_TYPE      pCycleCounter
 *
 * return value:   EDD_RSP
 *===========================================================================*/
typedef struct _EDDP_CYCLE_COUNTER_TYPE     EDD_UPPER_MEM_ATTR *    EDDP_CYCLE_COUNTER_PTR_TYPE;
typedef struct _EDDP_CYCLE_COUNTER_TYPE
{               
    LSA_UINT32      High;
    LSA_UINT32      Low;
} EDDP_CYCLE_COUNTER_TYPE;

EDD_RSP EDDP_SYSTEM_IN_FCT_ATTR eddp_SRTProviderGetCycleCounter(
    EDDP_HANDLE			      	    hDDB,
    EDDP_CYCLE_COUNTER_PTR_TYPE     pCycleCounter);

/*=============================================================================
 * function name: eddp_I2C_Read_Offset()
 *
 * function:      API-function: reads n bytes from the I2C device of the selected port
 *
 * parameters:    ...
 *
 * return value:  EDD_RSP
 *
 *===========================================================================*/
EDD_RSP  EDD_UPPER_IN_FCT_ATTR  eddp_I2C_Read_Offset( EDDP_HANDLE  		   	         const  hDDB,          //DDB handle (refer to service EDDP_SRV_DEV_OPEN)
	                                                  LSA_UINT32	                 const  PortId,        //User-PortId 1...4
	                                                  LSA_UINT16		             const  I2CMuxSelect,  //can be used for HW-multiplexer-control
	                                                  LSA_UINT8			             const  I2CDevAddr,    //0...127
	                                                  LSA_UINT8			             const  I2COffsetCnt,  //0...2
	                                                  LSA_UINT8			             const  I2COffset1,    //optional: 0...255
	                                                  LSA_UINT8			             const  I2COffset2,    //optional: 0...255
                                                      LSA_UINT16                     const  RetryCnt,      //0: disabled,    1..65535
                                                      LSA_UINT16                     const  RetryTime_us,  //0: no waiting,  1..65535
	                                                  LSA_UINT32                     const  Size,      	   //in bytes, 1...300
	                                                  LSA_UINT8  EDD_UPPER_MEM_ATTR      *  pBuf );    	   //pointer to buffer

/*=============================================================================
 * function name: eddp_I2C_Write_Offset()
 *
 * function:      API-function: writes n bytes to the I2C device of the selected port
 *
 * parameters:    ...
 *
 * return value:  EDD_RSP
 *
 *===========================================================================*/
EDD_RSP  EDD_UPPER_IN_FCT_ATTR  eddp_I2C_Write_Offset( EDDP_HANDLE  		   	      const  hDDB,          //DDB handle (refer to service EDDP_SRV_DEV_OPEN)
	                                                   LSA_UINT32	                  const  PortId,        //User-PortId 1...4
	                                                   LSA_UINT16		              const  I2CMuxSelect,  //can be used for HW-multiplexer-control
	                                                   LSA_UINT8			          const  I2CDevAddr,    //0...127
	                                                   LSA_UINT8			          const  I2COffsetCnt,  //0...2
	                                                   LSA_UINT8			          const  I2COffset1,    //optional: 0...255
	                                                   LSA_UINT8			          const  I2COffset2,    //optional: 0...255
                                                       LSA_UINT16                     const  RetryCnt,      //0: disabled,    1..65535
                                                       LSA_UINT16                     const  RetryTime_us,  //0: no waiting,  1..65535
	                                                   LSA_UINT32                     const  Size,      	//in bytes, 1...300
	                                                   LSA_UINT8  EDD_UPPER_MEM_ATTR      *  pBuf );   	    //pointer to buffer


/*****************************************************************************/
/*  end of file EDDP_SYS.H                                                   */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_SYS_H */
