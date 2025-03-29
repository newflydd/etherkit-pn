#ifndef POF_PRM_REQ_H                   /* ----- reinclude-protection ----- */
#define POF_PRM_REQ_H

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
/*  C o m p o n e n t     &C: POF (Polymer Optical Fiber)               :C&  */
/*                                                                           */
/*  F i l e               &F: pof_prm_req.h                             :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Internal headerfile                                                      */
/*  Defines internal constants, types, data, macros and prototyping for      */
/*  POF.                                                                     */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
/*****************************************************************************/

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

#include "pnio_pck1_on.h"


/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/

/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* Opcode: POF_OPC_PRM_READ                                                  */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Opcode: POF_OPC_PRM_WRITE                                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* IO APDU substitutions                                                     */
/*---------------------------------------------------------------------------*/

/* Index (subslot specific) */
#define POF_PRM_FO_DATA_REAL_INDEX                  0x8060 /* PDPortFODataReal                         */
                                                           /* with subblock FiberOpticDiagnosisInfo    */
#define POF_PRM_FO_DATA_CHECK_INDEX                 0x8061 /* PDPortFODataCheck                        */
#define POF_PRM_FO_DATA_ADJUST_INDEX                0x8062 /* PDPortFODataAdjust                       */
#define POF_PRM_SFP_DATA_CHECK_INDEX                0x8063 /* PDPortSFPDataCheck                       */
#define POF_PRM_SFP_DIAG_TEST_INDEX                 0xB052 /* PDPortSFPDiagTest                        */

/* only for test purpose */
#define POF_PRM_FO_DATA_TRA_IND_TEST_INDEX          0xFFFF /* POF-Transceiver Identinformationen       */

/* BlockType */
#define POF_PRM_FO_DATA_REAL_BLOCKTYPE_1            0x0220 /* PDPortFODataReal                         */
#define POF_PRM_FO_DATA_REAL_BLOCKTYPE_3            0x0228 /* Subblock for reading                     */
                                                           /* FiberOpticDiagnosisInfo                  */
#define POF_PRM_FO_DATA_CHECK_BLOCKTYPE             0x0223 /* PDPortFODataCheck                        */
#define POF_PRM_FO_DATA_ADJUST_BLOCKTYPE            0x0222 /* PDPortFODataAdjust                       */
#define POF_PRM_SFP_DATA_CHECK_BLOCKTYPE            0x022E /* PDPortSFPDataCheck                       */
#define POF_PRM_SFP_DIAG_TEST_BLOCKTYPE             0xB052 /* PDPortSFPDiagTest                       */

/* only for test purpose */
#define POF_PRM_FO_DATA_TRA_IND_TEST_BLOCKTYPE      0xFFFF 

/* record_data_length */
#define POF_PRM_FO_DATA_REAL_LENGTH                 28UL   /* PDPortFODataReal                         */
#define POF_PRM_FO_DATA_REAL_SHORT_LENGTH           16UL   /* PDPortFODataReal(Fiber but not POF)      */
#define POF_PRM_FO_DATA_CHECK_LENGTH                20UL   /* PDPortFODataCheck                        */
#define POF_PRM_FO_DATA_ADJUST_LENGTH               16UL   /* PDPortFODataAdjust                       */
#define POF_PRM_SFP_DATA_CHECK_LENGTH               16UL   /* PDPortFOSFPDataCheck                     */
#define POF_PRM_SFP_DIAG_TEST_LENGTH                12UL   /* PDPortFOSFPDiagTest                      */
#define POF_PRM_FO_DATA_TRA_IND_TEST_LENGTH         76UL   /* only for test purpose                    */
                                                           /* POF-Transceiver Identinformationen       */

#define POF_PRM_FO_DATA_REAL_BLOCK_LENGTH_BLOCK_1   24     /* PDPortFODataReal, Block1 (= length inclusive all subblocks) */
#define POF_PRM_FO_DATA_REAL_BLOCK_LENGTH_BLOCK_3   8      /* PDPortFODataReal, Block3 (this subblock is mandatory)       */

#define POF_PRM_BLOCK_VERSION_HIGH                  0x01   /* BlockVersionHigh                                            */
#define POF_PRM_BLOCK_VERSION_LOW                   0x00   /* BlockVersionLow                                             */

//N_... = Network format (= Big Endian)

PNIO_PACKED_ATTRIBUTE_PRE struct _POF_PRM_RECORD_HEADER_TYPE
{
  /* 00 */ LSA_UINT16          N_BlockType;
  /* 02 */ LSA_UINT16          N_BlockLength;
  /* 04 */ LSA_UINT8           N_BlockVersionHigh;
  /* 05 */ LSA_UINT8           N_BlockVersionLow;
  /* 06 */ LSA_UINT8           N_Padding1_1; 
  /* 07 */ LSA_UINT8           N_Padding1_2; 

  /* 08 Length */

}  PNIO_PACKED_ATTRIBUTE_POST;

typedef struct _POF_PRM_RECORD_HEADER_TYPE POF_PRM_RECORD_HEADER_TYPE;

typedef struct _POF_PRM_RECORD_HEADER_TYPE   POF_UPPER_MEM_ATTR *    POF_UPPER_PRM_RECORD_HEADER_PTR_TYPE;

#define POF_PRM_BLOCKHEADER_OFFSET_BlockType           0
#define POF_PRM_BLOCKHEADER_OFFSET_BlockLength         2
#define POF_PRM_BLOCKHEADER_OFFSET_BlockVersionHigh    4
#define POF_PRM_BLOCKHEADER_OFFSET_BlockVersionLow     5
#define POF_PRM_BLOCKHEADER_OFFSET_Padding1_1          6
#define POF_PRM_BLOCKHEADER_OFFSET_Padding1_2          7


/* PDPortFODataReal, Read */
PNIO_PACKED_ATTRIBUTE_PRE struct _POF_PRM_RECORD_FO_DATA_REAL_TYPE
{
  POF_PRM_RECORD_HEADER_TYPE   N_Header1;

  /* 08 */ LSA_UINT32          N_FiberOpticType;
  /* 12 */ LSA_UINT32          N_FiberOpticCableType;
  /* 16 Length is shorten in case of Fiber but no POF*/

  //subblock "FiberOpticDiagnosisInfo":
  /* 16 */ LSA_UINT16          N_BlockType3;
  /* 18 */ LSA_UINT16          N_BlockLength3;
  /* 20 */ LSA_UINT8           N_BlockVersionHigh3;
  /* 21 */ LSA_UINT8           N_BlockVersionLow3;

  /* 22 */ LSA_UINT8           N_Padding3_1;
  /* 23 */ LSA_UINT8           N_Padding3_2;
  /* 24 */ LSA_UINT32          N_FiberOpticPowerBudgetReal; /* 0,1 dB */

  /* 28 Length */

}  PNIO_PACKED_ATTRIBUTE_POST;

typedef struct _POF_PRM_RECORD_FO_DATA_REAL_TYPE POF_PRM_RECORD_FO_DATA_REAL_TYPE;

typedef struct _POF_PRM_RECORD_FO_DATA_REAL_TYPE   POF_UPPER_MEM_ATTR *    POF_UPPER_PRM_FO_DATA_REAL_PTR_TYPE;

/* CheckEnable-Flag for MaintenanceRequiredPowerBudget */ 
/*                      MaintenanceDemandedPowerBudget */
/*                      ErrorPowerBudget               */
/* Bit 31: FiberOpticPowerBudgetType.CheckEnable       */
/*#define POF_PRM_FO_DATA_CHECK_FIBER_OPTIC_POWER_BUDGET_TYPE 0x80000000*/ 


/* PDPortFODataCheck, Read and Write */
PNIO_PACKED_ATTRIBUTE_PRE struct _POF_PRM_RECORD_FO_DATA_CHECK_TYPE
{
  POF_PRM_RECORD_HEADER_TYPE   N_Header1;

  /* 08 */ LSA_UINT32          N_MaintenanceRequiredPowerBudget; 
  /* 12 */ LSA_UINT32          N_MaintenanceDemandedPowerBudget; 
  /* 16 */ LSA_UINT32          N_ErrorPowerBudget; 

  /* 20 Length  */

}  PNIO_PACKED_ATTRIBUTE_POST;

typedef struct _POF_PRM_RECORD_FO_DATA_CHECK_TYPE POF_PRM_RECORD_FO_DATA_CHECK_TYPE;

typedef struct _POF_PRM_RECORD_FO_DATA_CHECK_TYPE  POF_UPPER_MEM_ATTR       *    POF_UPPER_PRM_FO_DATA_CHECK_PTR_TYPE;
typedef struct _POF_PRM_RECORD_FO_DATA_CHECK_TYPE  POF_UPPER_MEM_ATTR const *    POF_UPPER_PRM_FO_DATA_CHECK_CONST_PTR_TYPE;


#define POF_PRM_RECORD_FO_DATA_CHECK_OFFSET_N_MaintenanceRequiredPowerBudget         8
#define POF_PRM_RECORD_FO_DATA_CHECK_OFFSET_N_MaintenanceDemandedPowerBudget        12
#define POF_PRM_RECORD_FO_DATA_CHECK_OFFSET_N_ErrorPowerBudget                      16


/* PDPortFODataAdjust, Read and Write */
PNIO_PACKED_ATTRIBUTE_PRE struct _POF_PRM_RECORD_FO_DATA_ADJUST_TYPE
{
  POF_PRM_RECORD_HEADER_TYPE   N_Header1;

  /* 08 */ LSA_UINT32          N_FiberOpticType;
  /* 12 */ LSA_UINT32          N_FiberOpticCableType;

  /* 16 Length */
  
}  PNIO_PACKED_ATTRIBUTE_POST;

typedef struct _POF_PRM_RECORD_FO_DATA_ADJUST_TYPE POF_PRM_RECORD_FO_DATA_ADJUST_TYPE;

typedef struct _POF_PRM_RECORD_FO_DATA_ADJUST_TYPE  POF_UPPER_MEM_ATTR       *    POF_UPPER_PRM_FO_DATA_ADJUST_PTR_TYPE;
typedef struct _POF_PRM_RECORD_FO_DATA_ADJUST_TYPE  POF_UPPER_MEM_ATTR const *    POF_UPPER_PRM_FO_DATA_ADJUST_CONST_PTR_TYPE;


#define POF_PRM_RECORD_FO_DATA_ADJUST_OFFSET_N_FiberOpticType            8
#define POF_PRM_RECORD_FO_DATA_ADJUST_OFFSET_N_FiberOpticCableType      12


/* PDPortFOSFPDataCheck, Read and Write */
PNIO_PACKED_ATTRIBUTE_PRE struct _POF_PRM_RECORD_SFP_DATA_CHECK_TYPE
{
  POF_PRM_RECORD_HEADER_TYPE   N_Header1;

  /* 08 */ LSA_UINT32          N_MaintenanceDemandedAdminStatus; 
  /* 12 */ LSA_UINT32          N_ErrorAdminStatus; 

  /* 16 Length  */

}  PNIO_PACKED_ATTRIBUTE_POST;

typedef struct _POF_PRM_RECORD_SFP_DATA_CHECK_TYPE POF_PRM_RECORD_SFP_DATA_CHECK_TYPE;

typedef struct _POF_PRM_RECORD_SFP_DATA_CHECK_TYPE  POF_UPPER_MEM_ATTR       *    POF_UPPER_PRM_SFP_DATA_CHECK_PTR_TYPE;
typedef struct _POF_PRM_RECORD_SFP_DATA_CHECK_TYPE  POF_UPPER_MEM_ATTR const *    POF_UPPER_PRM_SFP_DATA_CHECK_CONST_PTR_TYPE;


#define POF_PRM_RECORD_SFP_DATA_CHECK_OFFSET_N_MaintenanceDemandedAdminStatus       8
#define POF_PRM_RECORD_SFP_DATA_CHECK_OFFSET_N_ErrorAdminStatus                    12

#define SFP_TEMPERATURE_CHECK_ON                        0x0001
#define SFP_TX_BIAS_CHECK_ON                            0x0002
#define SFP_TX_POWER_CHECK_ON                           0x0004
#define SFP_RX_POWER_CHECK_ON                           0x0008

#define SFP_TX_FAULT_CHECK_ON                           0x0001
#define SFP_RX_LOSS_FAULT_CHECK_ON                      0x0002


/* PDPortFOSFPDiagTest, Read and Write */
PNIO_PACKED_ATTRIBUTE_PRE struct _POF_PRM_RECORD_SFP_DIAG_TEST_TYPE
{
  POF_PRM_RECORD_HEADER_TYPE   N_Header1;
  /* 08 */ LSA_UINT32          N_DiagTrigger; 
}  PNIO_PACKED_ATTRIBUTE_POST;

typedef struct _POF_PRM_RECORD_SFP_DIAG_TEST_TYPE POF_PRM_RECORD_SFP_DIAG_TEST_TYPE;

typedef struct _POF_PRM_RECORD_SFP_DIAG_TEST_TYPE  POF_UPPER_MEM_ATTR       *    POF_UPPER_PRM_SFP_DIAG_TEST_PTR_TYPE;
typedef struct _POF_PRM_RECORD_SFP_DIAG_TEST_TYPE  POF_UPPER_MEM_ATTR const *    POF_UPPER_PRM_SFP_DIAG_TEST_CONST_PTR_TYPE;

#define POF_PRM_RECORD_SFP_DIAG_TEST_OFFSET_N_DiagTrigger       8


/* POF-Transceiver Identinformationen, Read */
PNIO_PACKED_ATTRIBUTE_PRE struct _POF_PRM_RECORD_TRANSCEIVER_INDENT_TYPE
{
  POF_PRM_RECORD_HEADER_TYPE   N_Header1;

  /* 08 */ LSA_UINT8           N_Manufacturer   [16];
  /* 24 */ LSA_UINT8           N_ManufacturerOUI[4];
  /* 28 */ LSA_UINT8           N_PartNumber     [16];
  /* 44 */ LSA_UINT8           N_Revision       [4]; 
  /* 48 */ LSA_UINT8           N_Wavelength     [2]; 
  /* 50 */ LSA_UINT8           N_TransceiverOptions;
  /* 51 */ LSA_UINT8           N_SerialNumber   [16];
  /* 67 */ LSA_UINT8           N_Datecode       [8];
  /* 75 */ LSA_UINT8           N_Reserved2;

  /* 76 Length */
  
}  PNIO_PACKED_ATTRIBUTE_POST;

typedef struct _POF_PRM_RECORD_TRANSCEIVER_INDENT_TYPE POF_PRM_RECORD_TRANSCEIVER_INDENT_TYPE;

typedef struct _POF_PRM_RECORD_TRANSCEIVER_INDENT_TYPE   POF_UPPER_MEM_ATTR *    POF_UPPER_PRM_TRANSCEIVER_INDENT_PTR_TYPE;

#if defined (POF_UNUSED)

/* Index (device specific) */
#define POF_PRM_EXPECTED_DATA_INDEX             0xF842 /* PDExpectedData =                   */
                                                       /* PDPortDataAdjust + PDPortDataCheck */   
#define POF_PRM_EXPECTED_DATA_LENGTH                36 /* PDExpectedData                     */

/* PDExpectedData, Read */
PNIO_PACKED_ATTRIBUTE_PRE struct _POF_PRM_RECORD_EXPECTED_DATA_TYPE
{
  POF_PRM_RECORD_HEADER_TYPE   N_Header1;

  /* 08 */ LSA_UINT32          N_FiberOpticType;
  /* 12 */ LSA_UINT32          N_FiberOpticCableType;

  POF_PRM_RECORD_HEADER_TYPE   N_Header2;

  /* 24 */ LSA_UINT32          N_MaintenanceRequiredPowerBudget; 
  /* 28 */ LSA_UINT32          N_MaintenanceDemandedPowerBudget; 
  /* 32 */ LSA_UINT32          N_ErrorPowerBudget; 
  
  /* 36 Length  */
  
}  PNIO_PACKED_ATTRIBUTE_POST;

typedef struct _POF_PRM_RECORD_EXPECTED_DATA_TYPE POF_PRM_RECORD_EXPECTED_DATA_TYPE;

typedef struct _POF_PRM_RECORD_EXPECTED_DATA_TYPE   POF_UPPER_MEM_ATTR *    POF_UPPER_PRM_EXPECTED_DATA_PTR_TYPE;

#endif

/*---------------------------------------------------------------------------*/
/* Opcode: POF_OPC_PRM_INDICATION                                            */
/*---------------------------------------------------------------------------*/
/*** ChannelProperties ***/
#define POF_PRM_CH_PROP_NONE            0x0000  

/* Bit  9: MaintenanceRequired */
#define POF_PRM_CH_PROP_MAIN_REQUIRED   0x0200  

/* Bit 10: MaintenanceDemanded */
#define POF_PRM_CH_PROP_MAIN_DEMANDED   0x0400  

/* Bit  9: MaintenanceRequired  AND  Bit 10: MaintenanceDemanded  are 0 */
#define POF_PRM_CH_PROP_ERROR           0x0000  

/* Bit 11 - 12: ChannelProperties.Specifier */
#define POF_PRM_CH_PROP_APPEARS         0x0800  
#define POF_PRM_CH_PROP_DISAPPEARS      0x1000  

/*** ChannelErrorType ***/
#define POF_PRM_CHANNEL_ERROR_TYPE      0x8007  /* Fiber optics mismatch */

/*** ExtChannelErrorType ***/
#define POF_PRM_EXT_CHANNEL_ERROR_TYPE                     0x8000  /* Power       budget mismatch */
#define POF_PRM_EXT_CHANNEL_TEMPERATURE_HIGH_ERROR_TYPE    0x8001  /* Temperature budget mismatch */
#define POF_PRM_EXT_CHANNEL_TX_BIAS_HIGH_ERROR_TYPE        0x8002  /* TX_Bias     budget mismatch */
#define POF_PRM_EXT_CHANNEL_TX_BIAS_LOW_ERROR_TYPE         0x8003  /* TX_Bias     budget mismatch */
#define POF_PRM_EXT_CHANNEL_TX_POWER_HIGH_ERROR_TYPE       0x8004  /* TX_Power    budget mismatch */
#define POF_PRM_EXT_CHANNEL_TX_POWER_LOW_ERROR_TYPE        0x8005  /* TX_Power    budget mismatch */
#define POF_PRM_EXT_CHANNEL_RX_POWER_HIGH_ERROR_TYPE       0x8006  /* RX_Power    budget mismatch */
#define POF_PRM_EXT_CHANNEL_RX_POWER_LOW_ERROR_TYPE        0x8007  /* RX_Power    budget mismatch */
#define POF_PRM_EXT_CHANNEL_TX_FAULT_ERROR_TYPE            0x8008  /* TX_Fault    budget mismatch */
#define POF_PRM_EXT_CHANNEL_RX_LOSS_ERROR_TYPE             0x8009  /* RX_LOSS     budget mismatch */

/*** ExtChannelAddValue ***/
/* #define POF_PRM_EXT_CHANNEL_ADD_NONE    0x0000*/  /* No info */

/*---------------------------------------------------------------------------*/
/* defines                                                                   */
/*---------------------------------------------------------------------------*/


/*===========================================================================*/
/*                                  prototypes                               */
/*===========================================================================*/

POF_RSP POF_LOCAL_FCT_ATTR POF_RequestPrmRead(       POF_UPPER_RQB_PTR_TYPE const pRQB,
                                                     POF_HDB_PTR_TYPE       const pHDB );

POF_RSP POF_LOCAL_FCT_ATTR POF_RequestPrmWrite(      POF_UPPER_RQB_PTR_TYPE const pRQB,
                                                     POF_HDB_PTR_TYPE       const pHDB );

POF_RSP POF_LOCAL_FCT_ATTR POF_RequestPrmChangePort( POF_UPPER_RQB_PTR_TYPE const pRQB,
                                                     POF_HDB_PTR_TYPE       const pHDB );

POF_RSP POF_LOCAL_FCT_ATTR POF_RequestPrmIndication( POF_LOCAL_MEM_BOOL_PTR_TYPE const pbIndicate,
                                                     POF_UPPER_RQB_PTR_TYPE      const pRQB,
                                                     POF_HDB_PTR_TYPE            const pHDB );

/*****************************************************************************/
/*  end of file pof_prm_req.h                                                */
/*****************************************************************************/
#include "pnio_pck_off.h"

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of POF_PRM_H */
