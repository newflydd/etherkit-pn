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
/*  F i l e               &F: pof_dmi.c                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  POF DMI                                          */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
/*****************************************************************************/

/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/
#define LTRC_ACT_MODUL_ID  11
#define POF_MODULE_ID      LTRC_ACT_MODUL_ID

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/
#include "pof_inc.h"  
#include "pof_dmi_type.h"
#include "pof_prm_type.h"
#include "pof_int.h"
#include "pof_glb.h"
#include "pof_dmi.h"

POF_FILE_SYSTEM_EXTENSION(POF_MODULE_ID)

/*===========================================================================*/
/*                             compiler switches                             */
/*===========================================================================*/

#define POF_DMI_READ_ONLY_NECESSARY_VALUES

/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/
//POF_ADR_PAGE_A0:
#define POF_VENDOR_NAME_LENGTH          16UL

#define POF_ADR_PART_NUMBER_0           40
#define POF_PART_NUMBER_LENGTH          16UL

//POF_ADR_PAGE_A2:
#define POF_ADR_RX_OMA_MARGIN_L_ALARM   40      //for process OMA
#define POF_ADR_RX_OMA_MARGIN_L_WARN    41      //for process OMA

#define POF_SIZE_THRESHOLD_ALARM_OMA    1UL     //for process OMA
#define POF_SIZE_THRESHOLD_WARN_OMA     1UL     //for process OMA

#define POF_ADR_REALTIME_RX_OMA_MARGIN  106
//#define POF_ADR_STATUS_CONTROL        110
#define POF_ADR_FLAGBIT_113             113
#define POF_ADR_FLAGBIT_117             117


//SFP_ADR_PAGE_A0:
//#define I2C_TRANSCEIVER_TYPE              0
#define SFP_DiagnosticMonitoringType     92   //Bit 5: internally calibrated, Bit6: digital diagnostic
//#define SFP_ENHANCED_OPTIONS             93
//#define SFP_SFF_8472_Compliance          94   //!= 0xFF but >= 1
#define SFP_NUMBER_VALIDATION_BYTES       3   //92 through 94


//SFP_ADR_PAGE_A2:
#define SFP_ADR_STATUS_CONTROL          110
//#define SFP_ADR_ALARM_FLAGS             112
//#define SFP_ADR_RX_POWER_FLAGS          113
#define SFP_NUMBER_MONITORING_BYTES       4   // 110 through 113

// SFP Byte 0 
//#define SFP_TRANSCEIVER_TYPE_IS_SFP            0x03

// SFP Byte 92
//#define SFP_INTERNALLY_CALIBRATED              0x20
#define SFP_DIGITAL_DIAGNOSTIC_IMPLEMENTED     0x40

/*===========================================================================*/
/*                                global Variables                           */
/*===========================================================================*/
static const LSA_UINT8 PartNumberSoll[POF_VENDOR_NAME_LENGTH] = 
       {'Q', 'F', 'B', 'R', '-', '5', '9', '7', '8', 'A', 'Z', 'x', 'x', 'x', 'x', 'x'};


static POF_SFP_PORT_DIAG_TESTS sgSFP_DiagnosticTest =
{
    1       // NumberTestTranceivers
  , {       // TestTranceivers [ ]
        {   // TestTranceiver  [0]
            {"Diag Test Transc..."} // Signature[20];  "44 69 61 67 20 54 65 73 74 20 54 72 61 6E 73 63 2E 2E 2E"
          , 19       // SignatureLength;
          , 0x60     // SignatureAddress;  
          , 0x7B     // PasswordAddress; 
          ,  { 0x70, 0x80, 0x38, 0x48, 0x03, 0x00 }     // Password      [SFP_TRANCEIVER_MAX_PASSWORD_LENHGTH + 2]; +TableSelect +FillByte
          ,  { 0x00, 0x00, 0x00, 0x00, 0x03, 0x00 }     // ResetPassword [SFP_TRANCEIVER_MAX_PASSWORD_LENHGTH + 2]; +TableSelect +FillByte
          ,  5     // PasswordLength;  inclusive TableSelect
          ,  0     // CurrentManipulationStatus;
          ,  0     // SupportedManipulationMask;
          , 
          {
            {// Temperature
              //   ManipulationBitMask,            ManipulationWriteAddress, 
              //   |                               |     ManipulationByteNumber, 
              //   |                               |     |  ManipulationMask,
              //   |                               |     |  |        OriginalValue[2], 
              //   |                               |     |  |        |               ManipulationValue[2]
                {  SFP_DIAG_TRIGGER_TEMP,          0x00, 2, 0xFFFF, {{0x6E, 0x00}}, {{0x80, 0x01}} }    //lint !e708 !e651
              , {  SFP_DIAG_TRIGGER_NO_EFFECT,     0x00, 0, 0x0000, {{0x00, 0x00}}, {{0x00, 0x00}} }    //lint !e708
            }                                                                                     
          , {// TX_Bias
                {  SFP_DIAG_TRIGGER_TXBIAS_HIGH,   0x10, 2, 0xFFFF, {{0x1D, 0x4C}}, {{0x00, 0x00}} }    //lint !e708
              , {  SFP_DIAG_TRIGGER_TXBIAS_LOW,    0x12, 2, 0xFFFF, {{0x00, 0x32}}, {{0xFF, 0xFF}} }    //lint !e708
            }                                                                                     
          , {// TX_Power
                {  SFP_DIAG_TRIGGER_TXPOWER_HIGH,  0x18, 2, 0xFFFF, {{0x13, 0x94}}, {{0x00, 0x00}} }    //lint !e708
              , {  SFP_DIAG_TRIGGER_TXPOWER_LOW,   0x1A, 2, 0xFFFF, {{0x03, 0x7B}}, {{0xFF, 0xFF}} }    //lint !e708
            }                                                                                     
          , {// RX_Power
                {  SFP_DIAG_TRIGGER_RXPOWER_HIGH,  0x20, 2, 0xFFFF, {{0x18, 0xA7}}, {{0x00, 0x00}} }    //lint !e708
              , {  SFP_DIAG_TRIGGER_RXPOWER_LOW,   0x22, 2, 0xFFFF, {{0x00, 0x9E}}, {{0xFF, 0xFF}} }    //lint !e708
            }                                                                                     
          , {// TX_FAULT - Table03 has to be mapped!
                {  SFP_DIAG_TRIGGER_TXFAULT,       0x89, 1, 0x0002, {{0x00, 0x00}}, {{0x02, 0x00}} }    //lint !e708
              , {  SFP_DIAG_TRIGGER_NO_EFFECT,     0x00, 0, 0x0000, {{0x00, 0x00}}, {{0x00, 0x00}} }    //lint !e708
            }                                                                                     
          , {// RX_LOSS - Table03 has to be mapped!
                {  SFP_DIAG_TRIGGER_RXLOSS,        0x89, 1, 0x0001, {{0x00, 0x00}}, {{0x01, 0x00}} }    //lint !e708
              , {  SFP_DIAG_TRIGGER_NO_EFFECT,     0x00, 0, 0x0000, {{0x00, 0x00}}, {{0x00, 0x00}} }    //lint !e708
            }
          }
        }   // end TestTranceiver [0]
    }
};

/*===========================================================================*/
/*                                typedefs                                   */
/*===========================================================================*/

/*===========================================================================*/
/*                            local function declaration                     */
/*===========================================================================*/
static LSA_BOOL   POF_LOCAL_FCT_ATTR POF_DMI_Read_All_A0          ( LSA_UINT16                const  PortIndex,
                                                                    POF_HDB_PTR_TYPE          const  pHDB );

static LSA_BOOL   POF_LOCAL_FCT_ATTR POF_DMI_TestI2CBus            ( LSA_UINT16               const  PortIndex,
                                                                     POF_HDB_PTR_TYPE         const  pHDB );

static LSA_BOOL   POF_LOCAL_FCT_ATTR POF_DMI_TestTransceiver       ( LSA_UINT16               const  PortIndex,
                                                                     POF_HDB_PTR_TYPE         const  pHDB );

static LSA_BOOL   POF_LOCAL_FCT_ATTR SFP_DMI_TestTransceiver       ( LSA_UINT16               const  PortIndex,
                                                                     POF_HDB_PTR_TYPE         const  pHDB );

static LSA_BOOL   POF_LOCAL_FCT_ATTR POF_DMI_WriteThreshold        ( LSA_UINT16               const  PortIndex,
                                                                     POF_HDB_PTR_TYPE         const  pHDB );

static LSA_BOOL   POF_LOCAL_FCT_ATTR POF_DMI_IsThresholdCheckEnable( LSA_UINT16               const  PortIndex,
                                                                     POF_HDB_PTR_TYPE         const  pHDB );

static LSA_UINT32 POF_LOCAL_FCT_ATTR Check4SFP_TestTranceiver      ( LSA_UINT16               const  PortIndex,
                                                                     POF_HDB_PTR_TYPE         const  pHDB );
static LSA_VOID   POF_LOCAL_FCT_ATTR Check4SFP_TestTranceiverManipulations(
                                                                     LSA_UINT16               const  PortIndex,
                                                                     POF_HDB_PTR_TYPE         const  pHDB );

static LSA_UINT32 POF_LOCAL_FCT_ATTR POF_Write_SFP_DiagTestData2HW(  LSA_UINT16               const  PortIndex,
                                                                     POF_HDB_PTR_TYPE         const  pHDB,
                                                               const POF_SFP_DIAG_TEST_TYPE * const  pSFP_DiagTest,
                                                                     LSA_UINT8  *             const  pBuf);

static LSA_VOID   POF_LOCAL_FCT_ATTR POF_I2C_READ_OFFSET_Internal(   LSA_UINT16 *             const  ret_val_ptr,
                                                                     POF_HDB_PTR_TYPE         const  pHDB,
                                                                     LSA_UINT32               const  PortIndex,
                                                                     LSA_UINT8                const  I2CDevAddr,
                                                                     LSA_UINT8                const  I2COffsetCnt,
                                                                     LSA_UINT8                const  I2COffset1,
                                                                     LSA_UINT8                const  I2COffset2,
                                                                     LSA_UINT32               const  Size,
                                                                     LSA_UINT8  *             const  pBuf);
static LSA_VOID  POF_LOCAL_FCT_ATTR POF_I2C_WRITE_OFFSET_Internal(   LSA_UINT16 *             const  ret_val_ptr,
                                                                     POF_HDB_PTR_TYPE         const  pHDB,
                                                                     LSA_UINT32               const  PortIndex,
                                                                     LSA_UINT8                const  I2CDevAddr,
                                                                     LSA_UINT8                const  I2COffsetCnt,
                                                                     LSA_UINT8                const  I2COffset1,
                                                                     LSA_UINT8                const  I2COffset2,
                                                                     LSA_UINT32               const  Size,
                                                                     LSA_UINT8  *             const  pBuf);

/*===========================================================================*/
/*                                 Macros                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                 functions                                 */
/*===========================================================================*/

/*=============================================================================
 * function name:   Check4SFP_TestTranceiver()
 *
 * parameters:      
 *
 * return value:    LSA_UINT32
 *
 * description:
 *===========================================================================*/
static LSA_UINT32 POF_LOCAL_FCT_ATTR  Check4SFP_TestTranceiver( LSA_UINT16       const  PortIndex,
                                                                POF_HDB_PTR_TYPE const  pHDB )
{
    EDD_RSP                         EDDRetVal;
    LSA_UINT32                      i;
    LSA_UINT8                       Signature[SFP_TRANCEIVER_MAX_SIGNATURE_LENHGTH];
    LSA_UINT32                      TransceiverFound = 0;

    for (i = 0; i < sgSFP_DiagnosticTest.NumberTestTranceivers; ++i)
    {

        POF_MEMSET_LOCAL(Signature, (LSA_UINT8)0, (LSA_UINT32) sizeof(Signature));

        //read signature
        EDDRetVal = EDD_STS_ERR_EXCP;
        POF_I2C_READ_OFFSET_Internal((LSA_UINT16 POF_LOCAL_MEM_ATTR *)&EDDRetVal, //ret_val_ptr
                                     pHDB,
                                     PortIndex,                                   //PortIndex
                                     POF_ADR_PAGE_A0,                             //I2CDevAddr
                                     POF_I2C_STD_OFFSET_CNT,                      //I2COffsetCnt
                                     sgSFP_DiagnosticTest.TestTranceivers[i].SignatureAddress, //I2COffset1
                                     0,                                           //I2COffset2
                                     sgSFP_DiagnosticTest.TestTranceivers[i].SignatureLength,  //Size
                                     Signature);                                   //pBuf

        if (EDDRetVal != EDD_STS_OK)
        {
            POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "Check4SFP_TestTranceiver, POF_I2C_READ_OFFSET with invalid ret_val:0x%X", EDDRetVal);
            continue;
        }

        if (0 == POF_MEMCMP_COMMON(sgSFP_DiagnosticTest.TestTranceivers[i].Signature, Signature, sgSFP_DiagnosticTest.TestTranceivers[i].SignatureLength))
        {
            TransceiverFound = i + 1;
            break;
        }
    }
    return TransceiverFound;
}


/*=============================================================================
 * function name:   Check4SFP_TestTranceiverManipulations()
 *
 * parameters:      
 *
 * return value:    LSA_UINT32
 *
 * description:
 *===========================================================================*/
static LSA_VOID POF_LOCAL_FCT_ATTR Check4SFP_TestTranceiverManipulations( LSA_UINT16       const  PortIndex,
                                                                          POF_HDB_PTR_TYPE const  pHDB )
{
    EDD_RSP                         EDDRetVal;
    LSA_UINT32                      i;
    union {
        LSA_UINT8   ByteArray[2];
        LSA_UINT16  DataRead;
    }                               TempValue;
    POF_EDD_PCTRL_PTR_TYPE  const   pPCtrl = &pHDB->pPCtrl[PortIndex];
    POF_SFP_DIAG_TEST_PAIR_TYPE *   pSFP_DiagTestData;

    pPCtrl->SFP_DiagTest.CurrentManipulationStatus = 0;

    // write password and table 3 select to page A2
    if (EDD_STS_OK != (EDDRetVal = POF_I2C_Write_ByteArray( PortIndex, pHDB,
                                                            pPCtrl->SFP_DiagTest.PasswordAddress,
                                                            pPCtrl->SFP_DiagTest.PasswordLength,
                                                            pPCtrl->SFP_DiagTest.Password)))
    {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,
                                "Check4SFP_TestTranceiverManipulations, POF_I2C_Write_ByteArray "
                                "writing password with invalid ret_val:0x%X", EDDRetVal);
        return;
    }

    for (i = 0; i < POF_SFP_NUMBER_CHECK_DIAGNOSTICS; ++i)
    {
        pSFP_DiagTestData = &pPCtrl->SFP_DiagTest.SFP_DiagTestData[i];

        if (pSFP_DiagTestData->HighValue.ManipulationByteNumber > 0)
        {
            TempValue.DataRead  = 0;
            EDDRetVal           = EDD_STS_ERR_EXCP;

            POF_I2C_READ_OFFSET_Internal((LSA_UINT16 POF_LOCAL_MEM_ATTR *)&EDDRetVal, //ret_val_ptr
                                         pHDB,
                                         PortIndex,                                   //PortIndex
                                         POF_ADR_PAGE_A2,                             //I2CDevAddr
                                         POF_I2C_STD_OFFSET_CNT,                      //I2COffsetCnt
                                         pSFP_DiagTestData->HighValue.ManipulationWriteAddress,
                                                                                      //I2COffset1
                                         0,                                           //I2COffset2
                                         2,                                           //Size
                                         TempValue.ByteArray);                        //pBuf

            if (EDDRetVal == EDD_STS_OK)
            {
                if ((TempValue.DataRead & pSFP_DiagTestData->HighValue.ManipulationMask)
                    != pSFP_DiagTestData->HighValue.Original.Value)
                {
                    if ((TempValue.DataRead & pSFP_DiagTestData->HighValue.ManipulationMask)
                        != pSFP_DiagTestData->HighValue.Manipulation.Value)
                    {     // even wrong manipulation value, so write orgiginal value
						LSA_UINT32 tempRetVal;
                        
						tempRetVal = POF_Write_SFP_DiagTestData2HW(PortIndex, pHDB, &pSFP_DiagTestData->HighValue,
                                                                        pSFP_DiagTestData->HighValue.Original.Value_8);

						if (tempRetVal)
						{
							POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,
								"Check4SFP_TestTranceiverManipulations, POF_I2C_READ_OFFSET "
								"(HighValue) POF_Write_SFP_DiagTestData2HW returned:0x%X", tempRetVal);
						}
                    }
                    else
                    {
                        pPCtrl->SFP_DiagTest.CurrentManipulationStatus 
                            |= pSFP_DiagTestData->HighValue.ManipulationBitMask;
                    }
                }
            }
            else
            {
                POF_PROGRAM_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,
                                     "Check4SFP_TestTranceiverManipulations, POF_I2C_READ_OFFSET "
                                     "(HighValue[%i]) with invalid ret_val:0x%X", i, EDDRetVal);
            }
        }

        if (pSFP_DiagTestData->LowValue.ManipulationByteNumber > 0)
        {
            TempValue.DataRead  = 0;
            EDDRetVal           = EDD_STS_ERR_EXCP;

            POF_I2C_READ_OFFSET_Internal((LSA_UINT16 POF_LOCAL_MEM_ATTR *)&EDDRetVal, //ret_val_ptr
                                         pHDB,
                                         PortIndex,                                   //PortIndex
                                         POF_ADR_PAGE_A2,                             //I2CDevAddr
                                         POF_I2C_STD_OFFSET_CNT,                      //I2COffsetCnt
                                         pSFP_DiagTestData->LowValue.ManipulationWriteAddress,
                                                                                      //I2COffset1
                                         0,                                           //I2COffset2
                                         2,                                           //Size
                                         TempValue.ByteArray);                        //pBuf

            if (EDDRetVal == EDD_STS_OK)
            {
                if ((TempValue.DataRead & pSFP_DiagTestData->LowValue.ManipulationMask)
                    != pSFP_DiagTestData->LowValue.Original.Value)
                {
                    if ((TempValue.DataRead & pSFP_DiagTestData->HighValue.ManipulationMask)
                        != pSFP_DiagTestData->LowValue.Manipulation.Value)
                    {     // even wrong manipulation value, so write orgiginal value
						LSA_UINT32 tempRetVal;
                        
						tempRetVal = POF_Write_SFP_DiagTestData2HW(PortIndex, pHDB, &pSFP_DiagTestData->LowValue,
                                                                        pSFP_DiagTestData->LowValue.Original.Value_8);

						if (tempRetVal)
						{
							POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,
								                 "Check4SFP_TestTranceiverManipulations, POF_I2C_READ_OFFSET "
								                 "(LowValue) POF_Write_SFP_DiagTestData2HW returned:0x%X", tempRetVal);
						}
                    }
                    else
                    {
                        pPCtrl->SFP_DiagTest.CurrentManipulationStatus 
                            |= pSFP_DiagTestData->LowValue.ManipulationBitMask;
                    }
                }
            }
            else
            {
                POF_PROGRAM_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,
                                     "Check4SFP_TestTranceiverManipulations, POF_I2C_READ_OFFSET "
                                     "(LowValue[%i]) with invalid ret_val:0x%X", i, EDDRetVal);
            }
        }
    }
    
/* keep the password written 
    // invalidate write access
    if (EDD_STS_OK != (EDDRetVal = POF_I2C_Write_ByteArray( PortIndex, pHDB,
                                                            pPCtrl->SFP_DiagTest.PasswordAddress,
                                                            pPCtrl->SFP_DiagTest.PasswordLength,
                                                            pPCtrl->SFP_DiagTest.ResetPassword)))
    {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,
                                "Check4SFP_TestTranceiverManipulations, POF_I2C_Write_ByteArray "
                                "resetting password with invalid ret_val:0x%X", EDDRetVal);
    }
*/
    return;
}


/*=============================================================================
 * function name:   Check4SFP_TestTranceiver()
 *
 * parameters:      
 *
 * return value:    LSA_UINT32
 *
 * description:
 *===========================================================================*/
EDD_RSP POF_LOCAL_FCT_ATTR  POF_I2C_Write_ByteArray( LSA_UINT16        const PortIndex,
                                                     POF_HDB_PTR_TYPE  const pHDB,
                                                     LSA_UINT8         const PageA2Address,
                                                     LSA_UINT32        const NumberBytes,
                                               const LSA_UINT8  *      const pBuf)

{
    EDD_RSP     EDDRetVal = EDD_STS_ERR_EXCP;
    LSA_UINT32  i;
    LSA_UINT8   LocalBuffer[256];

    if ((NumberBytes < 2) || (NumberBytes + PageA2Address >= 256))
        return EDDRetVal;

    POF_MEMCOPY_COMMON(LocalBuffer, (POF_COMMON_MEM_PTR_TYPE) pBuf, NumberBytes);

    for (i = 0; i < NumberBytes; i += 2)
    {
        LSA_UINT8   Offset = (LSA_UINT8) i;
        if (i + 1 == NumberBytes)
        {
            Offset -= 1;
        }

        EDDRetVal = EDD_STS_ERR_EXCP;

        POF_I2C_WRITE_OFFSET_Internal((LSA_UINT16 POF_LOCAL_MEM_ATTR *)&EDDRetVal, //ret_val_ptr
                                      pHDB,
                                      PortIndex,                                   //PortIndex
                                      POF_ADR_PAGE_A2,                             //I2CDevAddr
                                      POF_I2C_STD_OFFSET_CNT,                      //I2COffsetCnt
                                      PageA2Address + Offset,                      //I2COffset1
                                      0,                                           //I2COffset2
                                      2,                                           //Size  use 2 ALWAYS !!!! for now
                                      &LocalBuffer[Offset]);                       //pBuf
                                     
        if (EDDRetVal != EDD_STS_OK)
        {
            POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_I2C_Write_ByteArray, "
                                 "POF_I2C_WRITE_OFFSET with invalid ret_val:0x%X", EDDRetVal);
            break;
        }
    }
    return EDDRetVal;
}


/*=============================================================================
 * function name:   POF_DMI_IsThresholdCheckEnable()
 *
 * parameters:      
 *
 * return value:    LSA_BOOL
 *
 * description:
 *===========================================================================*/
static  LSA_BOOL  POF_LOCAL_FCT_ATTR  POF_DMI_IsThresholdCheckEnable( LSA_UINT16        const  PortIndex,
                                                                      POF_HDB_PTR_TYPE  const  pHDB )
{
  POF_EDD_PCTRL_PTR_TYPE  const  pPCtrl = &pHDB->pPCtrl[PortIndex];

  if (pPCtrl->RecordSet_A.MRCheckEnable)
  {
      return LSA_TRUE;
  }
  if (pPCtrl->RecordSet_A.MDCheckEnable)
  {
      return LSA_TRUE;
  }

  return LSA_FALSE;
}
/*---------------------- end [subroutine] ---------------------------------*/

/*=============================================================================
 * function name:   POF_DMI_WriteThreshold()
 *
 * parameters:      
 *
 * return value:    LSA_BOOL
 *
 * description:
 *===========================================================================*/
static  LSA_BOOL  POF_LOCAL_FCT_ATTR  POF_DMI_WriteThreshold( LSA_UINT16        const  PortIndex,
                                                              POF_HDB_PTR_TYPE  const  pHDB )
{
  POF_EDD_PCTRL_PTR_TYPE  const  pPCtrl = &pHDB->pPCtrl[PortIndex];
  EDD_RSP                        EDDRetVal;
  LSA_UINT16                     Value_0_1_db;
  LSA_UINT8                      Value_0_2_db;
  LSA_UINT8                      OldValue_0_2_db;
  #if defined (POF_CFG_TRACE_MODE) && ((POF_CFG_TRACE_MODE == 1) || (POF_CFG_TRACE_MODE == 2)) //LSA-Trace
  LTRC_LEVEL_TYPE                tracelevel = LSA_TRACE_LEVEL_WARN;
  #endif

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_DMI_WriteThreshold, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);
  #if defined (POF_CFG_TRACE_MODE) && ((POF_CFG_TRACE_MODE == 1) || (POF_CFG_TRACE_MODE == 2)) //LSA-Trace
  if (POF_TransceiverIsPulled(PortIndex, pHDB))
  {
      tracelevel = LSA_TRACE_LEVEL_NOTE;
  }
  #endif

  //MaintenanceRequired (warning threshold)
  if ((pPCtrl->RecordSet_A.MRCheckEnable) && (!pPCtrl->RecordSet_A.MRThresholdWritten))
  {
      pPCtrl->RecordSet_A.MRThresholdWritten = LSA_TRUE;

      Value_0_1_db = pPCtrl->RecordSet_A.MRPowerBudget_0_1_dB;
      Value_0_1_db = Value_0_1_db + (LSA_UINT16)POF_STRESSED_LINK_PENALTY_0_1_DB;
      Value_0_2_db = (LSA_UINT8)((LSA_UINT16)((Value_0_1_db + 1) / 2));

      #if !defined (POF_CFG_SIMULATE_DIAGNOSTIC)
      //read current warning threshold
      EDDRetVal = EDD_STS_ERR_EXCP;
      POF_I2C_READ_OFFSET_Internal((LSA_UINT16 POF_LOCAL_MEM_ATTR *)&EDDRetVal, //ret_val_ptr
                                   pHDB,
                                   PortIndex,                                   //PortIndex
                                   POF_ADR_PAGE_A2,                             //I2CDevAddr
                                   POF_I2C_STD_OFFSET_CNT,                      //I2COffsetCnt
                                   POF_ADR_RX_OMA_MARGIN_L_WARN,                //I2COffset1
                                   0,                                           //I2COffset2
                                   POF_SIZE_THRESHOLD_WARN_OMA,                 //Size
                                   &OldValue_0_2_db);                           //pBuf
      if (EDDRetVal != EDD_STS_OK)
      {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, tracelevel, "POF_DMI_WriteThreshold, POF_I2C_READ_OFFSET with invalid ret_val:0x%X", EDDRetVal);
        return LSA_FALSE;
      }

      POF_PROGRAM_TRACE_04(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_DMI_WriteThreshold, Warning OMA, Value_0_1_db:0x%X StressedLinkPenalty_0_1_db:0x%X Value_0_2_db:0x%X OldValue_0_2_db:0x%X",
                           Value_0_1_db, (LSA_UINT16)POF_STRESSED_LINK_PENALTY_0_1_DB, Value_0_2_db, OldValue_0_2_db);

      if //difference exists between new and old value?
         (Value_0_2_db != OldValue_0_2_db)
      {
        //set warning threshold
        EDDRetVal = EDD_STS_ERR_EXCP;
        POF_I2C_WRITE_OFFSET_Internal((LSA_UINT16 POF_LOCAL_MEM_ATTR *)&EDDRetVal, //ret_val_ptr
                                      pHDB,
                                      PortIndex,                                   //PortIndex
                                      POF_ADR_PAGE_A2,                             //I2CDevAddr
                                      POF_I2C_STD_OFFSET_CNT,                      //I2COffsetCnt
                                      POF_ADR_RX_OMA_MARGIN_L_WARN,                //I2COffset1
                                      0,                                           //I2COffset2
                                      POF_SIZE_THRESHOLD_WARN_OMA,                 //Size
                                      &Value_0_2_db);                              //pBuf
        if (EDDRetVal != EDD_STS_OK)
        {
          POF_PROGRAM_TRACE_01(pHDB->TraceIdx, tracelevel, "POF_DMI_WriteThreshold, POF_I2C_WRITE_OFFSET with invalid ret_val:0x%X", EDDRetVal);
          return LSA_FALSE;
        }

        return LSA_TRUE; //threshold is written
      }
      #endif
  }

  //MaintenanceDemanded (alarm threshold)
  if ((pPCtrl->RecordSet_A.MDCheckEnable) && (!pPCtrl->RecordSet_A.MDThresholdWritten))
  {
      pPCtrl->RecordSet_A.MDThresholdWritten = LSA_TRUE;

      Value_0_1_db = pPCtrl->RecordSet_A.MDPowerBudget_0_1_dB;
      Value_0_1_db = Value_0_1_db + (LSA_UINT16)POF_STRESSED_LINK_PENALTY_0_1_DB;
      Value_0_2_db = (LSA_UINT8)((LSA_UINT16)((Value_0_1_db + 1) / 2));

      #if !defined (POF_CFG_SIMULATE_DIAGNOSTIC)
      //read current alarm threshold
      EDDRetVal = EDD_STS_ERR_EXCP;
      POF_I2C_READ_OFFSET_Internal((LSA_UINT16 POF_LOCAL_MEM_ATTR *)&EDDRetVal, //ret_val_ptr
                                   pHDB,
                                   PortIndex,                                   //PortIndex
                                   POF_ADR_PAGE_A2,                             //I2CDevAddr
                                   POF_I2C_STD_OFFSET_CNT,                      //I2COffsetCnt
                                   POF_ADR_RX_OMA_MARGIN_L_ALARM,               //I2COffset1
                                   0,                                           //I2COffset2
                                   POF_SIZE_THRESHOLD_ALARM_OMA,                //Size
                                   &OldValue_0_2_db);                           //pBuf
      if (EDDRetVal != EDD_STS_OK)
      {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, tracelevel, "POF_DMI_WriteThreshold, POF_I2C_READ_OFFSET with invalid ret_val:0x%X", EDDRetVal);
        return LSA_FALSE;
      }

      POF_PROGRAM_TRACE_04(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_DMI_WriteThreshold, Alarm OMA, Value_0_1_db:0x%X StressedLinkPenalty_0_1_db:0x%X Value_0_2_db:0x%X OldValue_0_2_db:0x%X",
                           Value_0_1_db, (LSA_UINT16)POF_STRESSED_LINK_PENALTY_0_1_DB, Value_0_2_db, OldValue_0_2_db);

      if //difference exists between new and old value?
         (Value_0_2_db != OldValue_0_2_db)
      {
        //set alarm threshold
        EDDRetVal = EDD_STS_ERR_EXCP;
        POF_I2C_WRITE_OFFSET_Internal((LSA_UINT16 POF_LOCAL_MEM_ATTR *)&EDDRetVal, //ret_val_ptr
                                      pHDB,
                                      PortIndex,                                   //PortIndex
                                      POF_ADR_PAGE_A2,                             //I2CDevAddr
                                      POF_I2C_STD_OFFSET_CNT,                      //I2COffsetCnt
                                      POF_ADR_RX_OMA_MARGIN_L_ALARM,               //I2COffset1
                                      0,                                           //I2COffset2
                                      POF_SIZE_THRESHOLD_ALARM_OMA,                //Size
                                      &Value_0_2_db);                              //pBuf
        if (EDDRetVal != EDD_STS_OK)
        {
          POF_PROGRAM_TRACE_01(pHDB->TraceIdx, tracelevel, "POF_DMI_WriteThreshold, POF_I2C_WRITE_OFFSET with invalid ret_val:0x%X", EDDRetVal);
          return LSA_FALSE;
        }

        return LSA_TRUE; //threshold is written
      }
      #endif
  }

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_DMI_WriteThreshold, PortIndex:0x%X pHDB:0x%X",
                        PortIndex, pHDB);

  return LSA_TRUE; //no error but no threshold is written
}
/*---------------------- end [subroutine] ---------------------------------*/


/*=============================================================================
 * function name:   POF_DMI_Validate_HW()
 *
 * parameters:      
 *
 * return value:    LSA_BOOL    Success
 *
 * description:
 *===========================================================================*/
LSA_BOOL  POF_LOCAL_FCT_ATTR  POF_DMI_Validate_HW( LSA_UINT16        const  PortIndex,
                                                   POF_HDB_PTR_TYPE  const  pHDB )
{
  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_DMI_Validate_HW, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);

  #if !defined (POF_CFG_SIMULATE_DIAGNOSTIC)
  //read complete DMI A0 MAP
  if (!POF_DMI_Read_All_A0(PortIndex, pHDB))
  {
    POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_DMI_Validate_HW, PortIndex:0x%X, POF_DMI_Read_All_A0 failed", PortIndex);
    return LSA_FALSE;
  }

  #if defined (POF_UNUSED)
  //get transceiver version
  POF_DMI_GetTransVersion(PortIndex, pHDB);
  #endif

  //test I2C bus
  if (!POF_DMI_TestI2CBus(PortIndex, pHDB))
  {
    POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_DMI_Validate_HW, PortIndex:0x%X, POF_DMI_TestI2CBus failed", PortIndex);
    return LSA_FALSE;
  }

  //check transceiver type
  if (!POF_DMI_TestTransceiver(PortIndex, pHDB))
  {
    POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_DMI_Validate_HW, PortIndex:0x%X, POF_DMI_TestTransceiver failed", PortIndex);
    return LSA_FALSE;
  }
  #endif //POF_CFG_SIMULATE_DIAGNOSTIC

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_DMI_Validate_HW, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);
  return LSA_TRUE;
}
/*---------------------- end [subroutine] ---------------------------------*/

/*=============================================================================
 * function name:   SFP_DMI_Validate_HW()
 *
 * parameters:      
 *
 * return value:    LSA_BOOL    Success
 *
 * description:
 *===========================================================================*/
LSA_BOOL  POF_LOCAL_FCT_ATTR  SFP_DMI_Validate_HW( LSA_UINT16        const  PortIndex,
                                                   POF_HDB_PTR_TYPE  const  pHDB )
{
  static const LSA_BOOL result = LSA_TRUE;

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: SFP_DMI_Validate_HW, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);

  //check transceiver type
  if (!SFP_DMI_TestTransceiver(PortIndex, pHDB))
  {
    POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "SFP_DMI_Validate_HW, PortIndex:0x%X, POF_DMI_TestTransceiver failed", PortIndex);
    return LSA_FALSE;
  }

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: SFP_DMI_Validate_HW, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);
  
  return result;
}
/*---------------------- end [subroutine] ---------------------------------*/

/*=============================================================================
 * function name:   POF_DMI_TestTransceiver()
 *
 * parameters:      
 *
 * return value:    LSA_BOOL    Success
 *
 * description:
 *===========================================================================*/
static LSA_BOOL POF_LOCAL_FCT_ATTR POF_DMI_TestTransceiver( LSA_UINT16        const  PortIndex,
                                                            POF_HDB_PTR_TYPE  const  pHDB )
{
  EDD_RSP                        EDDRetVal;
  LSA_UINT8                      PartNumber0;
  #if defined (POF_CFG_TRACE_MODE) && ((POF_CFG_TRACE_MODE == 1) || (POF_CFG_TRACE_MODE == 2)) //LSA-Trace
  LTRC_LEVEL_TYPE                tracelevel = LSA_TRACE_LEVEL_WARN;
  #endif

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_DMI_TestTransceiver, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);
  #if defined (POF_CFG_TRACE_MODE) && ((POF_CFG_TRACE_MODE == 1) || (POF_CFG_TRACE_MODE == 2)) //LSA-Trace
  if (POF_TransceiverIsPulled(PortIndex, pHDB))
  {
      tracelevel = LSA_TRACE_LEVEL_NOTE;
  }
  #endif

  /* check transceiver type:                     */
  /* - Avago QFBR-5978AZ                  => 'Q' */
  /* - broad market chip Avago AFBR-5978Z => 'A' */
  EDDRetVal = EDD_STS_ERR_EXCP;
  POF_I2C_READ_OFFSET_Internal((LSA_UINT16 POF_LOCAL_MEM_ATTR *)&EDDRetVal, //ret_val_ptr
                               pHDB,
                               PortIndex,                                   //PortIndex
                               POF_ADR_PAGE_A0,                             //I2CDevAddr
                               POF_I2C_STD_OFFSET_CNT,                      //I2COffsetCnt
                               POF_ADR_PART_NUMBER_0,                       //I2COffset1
                               0,                                           //I2COffset2
                               1UL, //1 byte                                //Size
                               &PartNumber0);                               //pBuf
  if (EDDRetVal != EDD_STS_OK)
  {
    POF_PROGRAM_TRACE_01(pHDB->TraceIdx, tracelevel, "POF_DMI_TestTransceiver, POF_I2C_READ_OFFSET with invalid ret_val:0x%X", EDDRetVal);
    return LSA_FALSE;
  }
  if (PartNumber0 != 'Q')
  {
    POF_PROGRAM_TRACE_02(pHDB->TraceIdx, tracelevel, "POF_DMI_TestTransceiver, invalid TransType, PortIndex:0x%X PartNumber0:0x%X",
                         PortIndex, PartNumber0);
    return LSA_FALSE;
  }

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_DMI_TestTransceiver, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);
  return LSA_TRUE;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*=============================================================================
 * function name:   POF_DMI_GetTransVersion()
 *
 * parameters:      
 *
 * return value:    LSA_VOID
 *
 * description:
 *===========================================================================*/
#if defined (POF_UNUSED)
static  LSA_VOID  POF_LOCAL_FCT_ATTR  POF_DMI_GetTransVersion( LSA_UINT16        const  PortIndex,
                                                               POF_HDB_PTR_TYPE  const  pHDB )
{
  POF_EDD_PCTRL_PTR_TYPE  const  pPCtrl = &pHDB->pPCtrl[PortIndex];
  LSA_UINT8                      year, month, day;

  //transceiver before 01/06/2006 are Beta3_3 ?
  //transceiver after  01/06/2006 are Beta4_3 ?
  //example: 21.06.2006 -> Beta4_3
  //   year      month       date
  // 84   85    86   87    88   89
  // 0x30 0x36  0x30 0x36  0x32 0x31
  //year 06 -> 2006

  year  = (LSA_UINT8)(((pPCtrl->DMIMap.A0_Mirror[84] & 0x0F) * 10) +
                       (pPCtrl->DMIMap.A0_Mirror[85] & 0x0F));

  month = (LSA_UINT8)(((pPCtrl->DMIMap.A0_Mirror[86] & 0x0F) * 10) +
                       (pPCtrl->DMIMap.A0_Mirror[87] & 0x0F));

  day   = (LSA_UINT8)(((pPCtrl->DMIMap.A0_Mirror[88] & 0x0F) * 10) +
                       (pPCtrl->DMIMap.A0_Mirror[89] & 0x0F));
}
#endif
/*---------------------- end [subroutine] ---------------------------------*/


/*=============================================================================
 * function name:   POF_DMI_TestI2CBus()
 *
 * parameters:      
 *
 * return value:    LSA_BOOL    Success
 *
 * description:
 *===========================================================================*/
static  LSA_BOOL  POF_LOCAL_FCT_ATTR  POF_DMI_TestI2CBus( LSA_UINT16        const  PortIndex,
                                                          POF_HDB_PTR_TYPE  const  pHDB )
{
  LSA_UINT32  TestCount, Ctr;
  LSA_UINT8   PartNumberRead[POF_PART_NUMBER_LENGTH];
  #if defined (POF_CFG_TRACE_MODE) && ((POF_CFG_TRACE_MODE == 1) || (POF_CFG_TRACE_MODE == 2)) //LSA-Trace
  LTRC_LEVEL_TYPE                tracelevel = LSA_TRACE_LEVEL_WARN;
  #endif

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_DMI_TestI2CBus, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);
  #if defined (POF_CFG_TRACE_MODE) && ((POF_CFG_TRACE_MODE == 1) || (POF_CFG_TRACE_MODE == 2)) //LSA-Trace
  if (POF_TransceiverIsPulled(PortIndex, pHDB))
  {
      tracelevel = LSA_TRACE_LEVEL_NOTE;
  }
  #endif

  for (TestCount = 0; TestCount < 40UL; TestCount++)
  {
    EDD_RSP  EDDRetVal;

    POF_MEMSET_LOCAL(PartNumberRead, (LSA_UINT8)0, (LSA_UINT32)POF_PART_NUMBER_LENGTH);

    //read vendor name
    EDDRetVal = EDD_STS_ERR_EXCP;
    POF_I2C_READ_OFFSET_Internal((LSA_UINT16 POF_LOCAL_MEM_ATTR *)&EDDRetVal, //ret_val_ptr
                                 pHDB,
                                 PortIndex,                                   //PortIndex
                                 POF_ADR_PAGE_A0,                             //I2CDevAddr
                                 POF_I2C_STD_OFFSET_CNT,                      //I2COffsetCnt
                                 POF_ADR_PART_NUMBER_0,                       //I2COffset1
                                 0,                                           //I2COffset2
                                 POF_PART_NUMBER_LENGTH,                      //Size
                                 PartNumberRead);                             //pBuf
    if (EDDRetVal != EDD_STS_OK)
    {
      POF_PROGRAM_TRACE_01(pHDB->TraceIdx, tracelevel, "POF_DMI_TestI2CBus, POF_I2C_READ_OFFSET with invalid ret_val:0x%X", EDDRetVal);
      return LSA_FALSE;
    }

    //compare vendor name
    for (Ctr = 0; Ctr < (LSA_UINT32)11; Ctr++)  //Only the first 11 Characters of the Part-Number is tested
    {
      if (PartNumberRead[Ctr] != PartNumberSoll[Ctr])
      {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, tracelevel, "POF_DMI_TestI2CBus, invalid VendorName read, PortIndex:0x%X", PortIndex);
        return LSA_FALSE;
      }
    }
  } //end of for-loop

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_DMI_TestI2CBus, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);
  return LSA_TRUE;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*=============================================================================
 * function name:   SFP_DMI_TestTransceiver()
 *
 * parameters:      
 *
 * return value:    LSA_BOOL    Success
 *
 * description:
 *===========================================================================*/
static LSA_BOOL POF_LOCAL_FCT_ATTR SFP_DMI_TestTransceiver( LSA_UINT16        const  PortIndex,
                                                            POF_HDB_PTR_TYPE  const  pHDB )
{
    EDD_RSP     EDDRetVal;
    LSA_UINT8   DataRead[SFP_NUMBER_VALIDATION_BYTES];

    #if defined (POF_CFG_TRACE_MODE) && ((POF_CFG_TRACE_MODE == 1) || (POF_CFG_TRACE_MODE == 2)) //LSA-Trace
    LTRC_LEVEL_TYPE                tracelevel = LSA_TRACE_LEVEL_WARN;
    #endif

    POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN:SFP_DMI_TestTransceiver, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);
    #if defined (POF_CFG_TRACE_MODE) && ((POF_CFG_TRACE_MODE == 1) || (POF_CFG_TRACE_MODE == 2)) //LSA-Trace
    if (POF_TransceiverIsPulled(PortIndex, pHDB))
    {
        tracelevel = LSA_TRACE_LEVEL_NOTE;
    }
    #endif

    POF_MEMSET_LOCAL(DataRead, (LSA_UINT8)0, (LSA_UINT32) SFP_NUMBER_VALIDATION_BYTES);

    //read vendor name
    EDDRetVal = EDD_STS_ERR_EXCP;
    POF_I2C_READ_OFFSET_Internal((LSA_UINT16 POF_LOCAL_MEM_ATTR *)&EDDRetVal, //ret_val_ptr
                                 pHDB,
                                 PortIndex,                                   //PortIndex
                                 POF_ADR_PAGE_A0,                             //I2CDevAddr
                                 POF_I2C_STD_OFFSET_CNT,                      //I2COffsetCnt
                                 SFP_DiagnosticMonitoringType,                //I2COffset1
                                 0,                                           //I2COffset2
                                 SFP_NUMBER_VALIDATION_BYTES,                 //Size
                                 DataRead);                                   //pBuf

    if (EDDRetVal != EDD_STS_OK)
    {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, tracelevel, "SFP_DMI_TestTransceiver, POF_I2C_READ_OFFSET with invalid ret_val:0x%X", EDDRetVal);
        return LSA_FALSE;
    }

    if (!(DataRead[0] & SFP_DIGITAL_DIAGNOSTIC_IMPLEMENTED))
    {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, tracelevel, "SFP_DMI_TestTransceiver, Digital Diagnostic Monitoring not implemented, PortIndex:0x%X", PortIndex);
        return LSA_FALSE;
    }

    if (!(DataRead[1] & 0xB0))  // at least one of the bits 4,5 or 7 is set
    {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, tracelevel, "SFP_DMI_TestTransceiver, no diagnosis at all is implemented, PortIndex:0x%X", PortIndex);
        return LSA_FALSE;
    }

    if ((DataRead[2] == 0) || (DataRead[2] == 0xFF))  //0xFF: not implemented or 0
    {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, tracelevel, "SFP_DMI_TestTransceiver, SFF 8472 Revision invalid, PortIndex:0x%X", PortIndex);
        return LSA_FALSE;
    }

    pHDB->pPCtrl[PortIndex].DMIMap.SFP_AlarmFlagsSupported = (DataRead[1] & 0x80) ? LSA_TRUE : LSA_FALSE;
    pHDB->pPCtrl[PortIndex].DMIMap.SFP_TX_FaultSupported   = (DataRead[1] & 0x20) ? LSA_TRUE : LSA_FALSE;
    pHDB->pPCtrl[PortIndex].DMIMap.SFP_RX_LossSupported    = (DataRead[1] & 0x10) ? LSA_TRUE : LSA_FALSE;

    POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: SFP_DMI_TestTransceiver, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);
    return LSA_TRUE;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*=============================================================================
 * function name:   POF_DMI_Read_All_A0
 *
 * parameters:      
 *
 * return value:    LSA_BOOL    Success
 *
 * description:
 *===========================================================================*/
static  LSA_BOOL  POF_LOCAL_FCT_ATTR  POF_DMI_Read_All_A0( LSA_UINT16        const  PortIndex,
                                                           POF_HDB_PTR_TYPE  const  pHDB )
{
    EDD_RSP  EDDRetVal;
    #if defined (POF_CFG_TRACE_MODE) && ((POF_CFG_TRACE_MODE == 1) || (POF_CFG_TRACE_MODE == 2)) //LSA-Trace
    LTRC_LEVEL_TYPE                tracelevel = LSA_TRACE_LEVEL_WARN;
    #endif

    POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_DMI_Read_All_A0, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);
    #if defined (POF_CFG_TRACE_MODE) && ((POF_CFG_TRACE_MODE == 1) || (POF_CFG_TRACE_MODE == 2)) //LSA-Trace
    if (POF_TransceiverIsPulled(PortIndex, pHDB))
    {
        tracelevel = LSA_TRACE_LEVEL_NOTE;
    }
    #endif

    //read whole A0 Map
    EDDRetVal = EDD_STS_ERR_EXCP;
    POF_I2C_READ_OFFSET_Internal((LSA_UINT16 POF_LOCAL_MEM_ATTR *)&EDDRetVal, //ret_val_ptr
                                 pHDB,
                                 PortIndex,                                   //PortIndex
                                 POF_ADR_PAGE_A0,                             //I2CDevAddr
                                 POF_I2C_STD_OFFSET_CNT,                      //I2COffsetCnt
                                 0,                                           //I2COffset1
                                 0,                                           //I2COffset2
                                 I2C_DMI_MAP_A0_SIZE_8,                       //Size
                                 pHDB->pPCtrl[PortIndex].DMIMap.A0_Mirror);   //pBuf
    if (EDDRetVal != EDD_STS_OK)
    {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, tracelevel, "POF_DMI_Read_All_A0, POF_I2C_READ_OFFSET with invalid ret_val:0x%X", EDDRetVal);
        return LSA_FALSE;
    }

    POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_DMI_Read_All_A0, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);
    return LSA_TRUE;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*=============================================================================
 * function name:   POF_DMI_ReadHW_TimeOut()
 *
 * parameters:      
 *
 * return value:    LSA_VOID
 *
 * description:
 *===========================================================================*/
LSA_VOID  POF_LOCAL_FCT_ATTR  POF_DMI_ReadHW_TimeOut( POF_HDB_PTR_TYPE  const  pHDB )
{
    LSA_UINT16  PortIndex;

    POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_DMI_ReadHW_TimeOut, pHDB:0x%X", pHDB);

    for (PortIndex = 0; PortIndex < pHDB->PortCnt; PortIndex++)
    {
        //If we get the cyclic tigger and no I2C_AVAILABLE (which should not happen) the SM is in the state POF_TRANSCEIVER_STATE_STOPPED and nothing happens.
        POF_TransceiverSMTransition(PortIndex, POF_TRANSCEIVER_TRIGGER_CYCLIC, pHDB);
    } //end of for-loop

    POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_DMI_ReadHW_TimeOut, pHDB:0x%X", pHDB);
}
/*---------------------- end [subroutine] ---------------------------------*/


LSA_BOOL POF_LOCAL_FCT_ATTR DMI_ReadHW_MarginDummy( LSA_UINT16        const PortIndex,
                                                    POF_HDB_PTR_TYPE  const pHDB )
{
    LSA_UNUSED_ARG(PortIndex);

    POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: DMI_ReadHW_MarginDummy, pHDB:0x%X", pHDB);
    return LSA_FALSE;
}

/*=============================================================================
 * function name:   POF_DMI_ReadHW_Margin()
 *
 * parameters:      
 *
 * return value:    LSA_BOOL    Success
 *
 * description:
 *===========================================================================*/
LSA_BOOL POF_LOCAL_FCT_ATTR POF_DMI_ReadHW_Margin( LSA_UINT16        const PortIndex,
                                                   POF_HDB_PTR_TYPE  const pHDB )
{
    POF_EDD_PCTRL_PTR_TYPE  const  pPCtrl = &pHDB->pPCtrl[PortIndex];
    #if !defined (POF_DMI_READ_ONLY_NECESSARY_VALUES)
    LSA_UINT16  Ctr;
    #endif
    EDD_RSP     EDDRetVal;
    #if defined (POF_CFG_TRACE_MODE) && ((POF_CFG_TRACE_MODE == 1) || (POF_CFG_TRACE_MODE == 2)) //LSA-Trace
    LTRC_LEVEL_TYPE             tracelevel = LSA_TRACE_LEVEL_WARN;
    #endif

    POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_DMI_ReadHW_Margin, pHDB:0x%X", pHDB);
    #if defined (POF_CFG_TRACE_MODE) && ((POF_CFG_TRACE_MODE == 1) || (POF_CFG_TRACE_MODE == 2)) //LSA-Trace
    if (POF_TransceiverIsPulled(PortIndex, pHDB))
    {
        tracelevel = LSA_TRACE_LEVEL_NOTE;
    }
    #endif

    #if defined (POF_DMI_READ_ONLY_NECESSARY_VALUES)
    //Read only the necessary transceiver-values

    //total execution-time (I2C):
    //  597us

    #if defined (POF_UNUSED)
    //Real time Rx Pwr, 2 * 253us
    if (POF_DMI_ReadConsistent_A2(PortIndex, (LSA_UINT8)104, pHDB))
    {
        pPCtrl->DMIMap.A2_Mirror.u.ElementPOF.RealTime_Rx_Pwr = pPCtrl->DMIMap.A2_Hardware.u.ElementPOF.RealTime_Rx_Pwr;
    }
    #endif

    //Real time Rx OMA Margin, 1 * 199us
    EDDRetVal = EDD_STS_ERR_EXCP;
    POF_I2C_READ_OFFSET_Internal((LSA_UINT16 POF_LOCAL_MEM_ATTR *)&EDDRetVal,  //ret_val_ptr
                                 pHDB,
                                 PortIndex,                                    //PortIndex
                                 POF_ADR_PAGE_A2,                              //I2CDevAddr
                                 POF_I2C_STD_OFFSET_CNT,                       //I2COffsetCnt
                                 POF_ADR_REALTIME_RX_OMA_MARGIN,               //I2COffset1
                                 0,                                            //I2COffset2
                                 1UL, //1 byte                                 //Size
                                 &pPCtrl->DMIMap.A2_Hardware.u.ElementPOF.RealTime_Rx_OMA_Margin); //pBuf
    if (EDDRetVal != EDD_STS_OK)
    {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, tracelevel, "POF_DMI_ReadHW_Margin, POF_I2C_READ_OFFSET with invalid ret_val:0x%X", EDDRetVal);
        return LSA_FALSE;
    }
    pPCtrl->DMIMap.A2_Mirror.u.ElementPOF.RealTime_Rx_OMA_Margin = pPCtrl->DMIMap.A2_Hardware.u.ElementPOF.RealTime_Rx_OMA_Margin;

    //Flag 113, 1 * 199us
    EDDRetVal = EDD_STS_ERR_EXCP;
    POF_I2C_READ_OFFSET_Internal((LSA_UINT16 POF_LOCAL_MEM_ATTR *)&EDDRetVal,      //ret_val_ptr
                                 pHDB,
                                 PortIndex,                                        //PortIndex
                                 POF_ADR_PAGE_A2,                                  //I2CDevAddr
                                 POF_I2C_STD_OFFSET_CNT,                           //I2COffsetCnt
                                 POF_ADR_FLAGBIT_113,                              //I2COffset1
                                 0,                                                //I2COffset2
                                 1UL, //1 byte                                     //Size
                                 &pPCtrl->DMIMap.A2_Hardware.u.ElementPOF.Flagbit_113); //pBuf
    if (EDDRetVal != EDD_STS_OK)
    {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, tracelevel, "POF_DMI_ReadHW_Margin, POF_I2C_READ_OFFSET with invalid ret_val:0x%X", EDDRetVal);
        return LSA_FALSE;
    }
    pPCtrl->DMIMap.A2_Mirror.u.ElementPOF.Flagbit_113 = pPCtrl->DMIMap.A2_Hardware.u.ElementPOF.Flagbit_113;

    //Flag 117, 1 * 199us
    EDDRetVal = EDD_STS_ERR_EXCP;
    POF_I2C_READ_OFFSET_Internal((LSA_UINT16 POF_LOCAL_MEM_ATTR *)&EDDRetVal,      //ret_val_ptr
                                 pHDB,
                                 PortIndex,                                             //PortIndex
                                 POF_ADR_PAGE_A2,                                       //I2CDevAddr
                                 POF_I2C_STD_OFFSET_CNT,                           //I2COffsetCnt
                                 POF_ADR_FLAGBIT_117,                              //I2COffset1
                                 0,                                                //I2COffset2
                                 1UL, //1 byte                                     //Size
                                 &pPCtrl->DMIMap.A2_Hardware.u.ElementPOF.Flagbit_117); //pBuf
    if (EDDRetVal != EDD_STS_OK)
    {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, tracelevel, "POF_DMI_ReadHW_Margin, POF_I2C_READ_OFFSET with invalid ret_val:0x%X", EDDRetVal);
        return LSA_FALSE;
    }
    pPCtrl->DMIMap.A2_Mirror.u.ElementPOF.Flagbit_117 = pPCtrl->DMIMap.A2_Hardware.u.ElementPOF.Flagbit_117;

    #if defined (POF_UNUSED)
    /* Status control, 1 * 199us */
    EDDRetVal = EDD_STS_ERR_EXCP;
    POF_I2C_READ_OFFSET((LSA_UINT16 POF_LOCAL_MEM_ATTR *)&EDDRetVal,        //ret_val_ptr
                        pHDB->Params.pSys,                                  //sys_ptr
                        PortIndex + 1UL, //PortId=PortIndex+1               //PortId
                        pHDB->I2C[PortIndex].I2CMuxSelect,                  //I2CMuxSelect
                        POF_ADR_PAGE_A2,                                    //I2CDevAddr
                        POF_I2C_STD_OFFSET_CNT,                             //I2COffsetCnt
                        POF_ADR_STATUS_CONTROL,                             //I2COffset1
                        0,                                                  //I2COffset2
                        1UL, //1 byte                                       //Size
                        &pPCtrl->DMIMap.A2_Hardware.u.ElementPOF.StatusControl); //pBuf
    if (EDDRetVal != EDD_STS_OK)
    {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, tracelevel, "POF_DMI_ReadHW_Margin, POF_I2C_READ_OFFSET with invalid ret_val:0x%X", EDDRetVal);
        return LSA_FALSE;
    }
    pPCtrl->DMIMap.A2_Mirror.u.ElementPOF.StatusControl = pPCtrl->DMIMap.A2_Hardware.u.ElementPOF.StatusControl;

    POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_DMI_ReadHW_Margin, StatusControl:0x%X",
                            pPCtrl->DMIMap.A2_Hardware.u.ElementPOF.StatusControl);
    #endif

    POF_PROGRAM_TRACE_03(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_DMI_ReadHW_Margin, RealTime_Rx_OMA_Margin:0x%X Flagbit_113:0x%X Flagbit_117:0x%X",
                            pPCtrl->DMIMap.A2_Hardware.u.ElementPOF.RealTime_Rx_OMA_Margin,
                            pPCtrl->DMIMap.A2_Hardware.u.ElementPOF.Flagbit_113,
                            pPCtrl->DMIMap.A2_Hardware.u.ElementPOF.Flagbit_117);
    #elif !defined (POF_DMI_READ_ONLY_NECESSARY_VALUES)
    //read whole transceiver A2-Memory
    if (pHDB->I2C[PortIndex].I2CAvailable != POF_I2C_AVAILABLE)   //This should never happen!!
    {
      POF_FATAL("POF_I2C_READ_OFFSET without I2C_AVAILABLE!", POF_FATAL_ERR_EDD, 0, 0);
    }
    EDDRetVal = EDD_STS_ERR_EXCP;
    POF_I2C_READ_OFFSET((LSA_UINT16 POF_LOCAL_MEM_ATTR *)&EDDRetVal, //ret_val_ptr
                        pHDB->Params.pSys,                           //sys_ptr
                        PortIndex + 1UL, //PortId=PortIndex+1        //PortId
                        pHDB->I2C[PortIndex].I2CMuxSelect,           //I2CMuxSelect
                        POF_ADR_PAGE_A2,                             //I2CDevAddr
                        POF_I2C_STD_OFFSET_CNT,                      //I2COffsetCnt
                        0,                                           //I2COffset1
                        0,                                           //I2COffset2
                        I2C_DMI_MAP_A2_SIZE_8,                       //Size
                        &pPCtrl->DMIMap.A2_Hardware.Array_8[0]);     //pBuf
    if (EDDRetVal != EDD_STS_OK)
    {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, tracelevel, "POF_DMI_ReadHW_Margin, POF_I2C_READ_OFFSET with invalid ret_val:0x%X", EDDRetVal);
        return LSA_FALSE;
    }

    for (Ctr = 0; Ctr < I2C_DMI_MAP_A2_SIZE_16; Ctr++)
    {
        //read only even address
        pPCtrl->DMIMap.A2_Mirror.Array_16[Ctr] = pPCtrl->DMIMap.A2_Hardware.Array_16[Ctr];
    }
    #endif //POF_DMI_READ_ONLY_NECESSARY_VALUES

    POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_DMI_ReadHW_Margin, pHDB:0x%X", pHDB);

    return LSA_TRUE;
}


/*=============================================================================
 * function name:   POF_DMI_ReadSFP_Margin()
 *
 * parameters:      
 *
 * return value:    LSA_BOOL    Success
 *
 * description:
 *===========================================================================*/
LSA_BOOL POF_LOCAL_FCT_ATTR POF_DMI_ReadSFP_Margin( LSA_UINT16        const PortIndex,
                                                    POF_HDB_PTR_TYPE  const pHDB )
{
    POF_EDD_PCTRL_PTR_TYPE  const  pPCtrl = &pHDB->pPCtrl[PortIndex];
    EDD_RSP                        EDDRetVal;

    #if defined (POF_CFG_TRACE_MODE) && ((POF_CFG_TRACE_MODE == 1) || (POF_CFG_TRACE_MODE == 2)) //LSA-Trace
        LTRC_LEVEL_TYPE             tracelevel = LSA_TRACE_LEVEL_WARN;
    #endif

    POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_DMI_ReadSFP_Margin, pHDB:0x%X", pHDB);
    #if defined (POF_CFG_TRACE_MODE) && ((POF_CFG_TRACE_MODE == 1) || (POF_CFG_TRACE_MODE == 2)) //LSA-Trace
        if (POF_TransceiverIsPulled(PortIndex, pHDB))
        {
            tracelevel = LSA_TRACE_LEVEL_NOTE;
        }
    #endif

    #if defined (POF_DMI_READ_ONLY_NECESSARY_VALUES)
        //Read only the necessary transceiver-values
    
    
        //Read diagnostic bytes
        EDDRetVal = EDD_STS_ERR_EXCP;
        POF_I2C_READ_OFFSET_Internal((LSA_UINT16 POF_LOCAL_MEM_ATTR *)&EDDRetVal,             //ret_val_ptr
                                     pHDB,
                                     PortIndex,                                               //PortIndex
                                     POF_ADR_PAGE_A2,                                         //I2CDevAddr
                                     POF_I2C_STD_OFFSET_CNT,                                  //I2COffsetCnt
                                     SFP_ADR_STATUS_CONTROL,                                  //I2COffset1
                                     0,                                                       //I2COffset2
                                     SFP_NUMBER_MONITORING_BYTES,                             //Size
                                     &pPCtrl->DMIMap.A2_Hardware.u.ElementSFP.StatusControl); //pBuf
        if (EDDRetVal != EDD_STS_OK)
        {
            POF_PROGRAM_TRACE_01(pHDB->TraceIdx, tracelevel, "POF_DMI_ReadSFP_Margin, POF_I2C_READ_OFFSET with invalid ret_val:0x%X", EDDRetVal);
            return LSA_FALSE;
        }
        POF_MEMCOPY_COMMON(&pPCtrl->DMIMap.A2_Mirror.u.ElementSFP.StatusControl, &pPCtrl->DMIMap.A2_Hardware.u.ElementSFP.StatusControl, 4);
    
    
    
        POF_PROGRAM_TRACE_03(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_DMI_ReadSFP_Margin, StatusControl:0x%X Flagbit_112:0x%X Flagbit_113:0x%X",
                                pPCtrl->DMIMap.A2_Hardware.u.ElementSFP.StatusControl,
                                pPCtrl->DMIMap.A2_Hardware.u.ElementSFP.Flagbit_112,
                                pPCtrl->DMIMap.A2_Hardware.u.ElementSFP.Flagbit_113);
    
    #elif !defined (POF_DMI_READ_ONLY_NECESSARY_VALUES)
        //read whole transceiver A2-Memory
        if (pHDB->I2C[PortIndex].I2CAvailable != POF_I2C_AVAILABLE)     //This should never happen!!
        {
          POF_FATAL("POF_I2C_READ_OFFSET without I2C_AVAILABLE!", POF_FATAL_ERR_EDD, 0, 0);
        }
        EDDRetVal = EDD_STS_ERR_EXCP;
        POF_I2C_READ_OFFSET((LSA_UINT16 POF_LOCAL_MEM_ATTR *)&EDDRetVal, //ret_val_ptr
                            pHDB->Params.pSys,                           //sys_ptr
                            PortIndex + 1UL, //PortId=PortIndex+1        //PortId
                            pHDB->I2C[PortIndex].I2CMuxSelect,           //I2CMuxSelect
                            POF_ADR_PAGE_A2,                             //I2CDevAddr
                            POF_I2C_STD_OFFSET_CNT,                      //I2COffsetCnt
                            0,                                           //I2COffset1
                            0,                                           //I2COffset2
                            I2C_DMI_MAP_A2_SIZE_8,                       //Size
                            &pPCtrl->DMIMap.A2_Hardware.Array_8[0]);     //pBuf
        if (EDDRetVal != EDD_STS_OK)
        {
            POF_PROGRAM_TRACE_01(pHDB->TraceIdx, tracelevel, "POF_DMI_ReadSFP_Margin, POF_I2C_READ_OFFSET with invalid ret_val:0x%X", EDDRetVal);
            return LSA_FALSE;
        }
    
        POF_MEMCOPY_COMMON(p.A2_Mirror.pPCtrl->DMIMap.A2_Mirror.Array_8, p.A2_Hardware.Array_8, I2C_DMI_MAP_A2_SIZE_8);
    #endif //POF_DMI_READ_ONLY_NECESSARY_VALUES

    POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_DMI_ReadSFP_Margin, pHDB:0x%X", pHDB);

    return LSA_TRUE;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*=============================================================================
 * function name: POF_DMI_ReadOffset_A0()
 *
 * parameters:    
 *
 * return value:  LSA_VOID
 *
 * description:
 *===========================================================================*/
LSA_VOID  POF_LOCAL_FCT_ATTR  POF_DMI_ReadOffset_A0( LSA_UINT16               const  PortIndex,
                                                     LSA_UINT8                const  Offset,
                                                     LSA_UINT32               const  Size,
                                                     POF_COMMON_MEM_PTR_TYPE         pAdr,
                                                     POF_HDB_PTR_TYPE         const  pHDB )
{
  //check range

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_DMI_ReadOffset_A0, pHDB:0x%X", pHDB);

  if ((Offset + Size) > I2C_DMI_MAP_A0_SIZE_8)
  {
    POF_PROGRAM_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_DMI_ReadOffset_A0, Offset:0x%X Size:0x%X", Offset, Size);
    POF_FATAL("POF_DMI_ReadOffset_A0, Offset: Size:", POF_FATAL_ERR_PRM_READ_RECORD_INDEX, Offset, Size);
    //lint -unreachable
    return;
  }

  if ((Size > I2C_DMI_MAP_A0_SIZE_8) && (Size < 1UL))
  {
    POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_DMI_ReadOffset_A0, Size:0x%X", Size);
    POF_FATAL("POF_DMI_ReadOffset_A0, Size:", POF_FATAL_ERR_PRM_READ_RECORD_INDEX, Size, 0);
    //lint -unreachable
    return;
  }

  POF_MEMCOPY_COMMON((POF_COMMON_MEM_PTR_TYPE)pAdr,
                     (POF_COMMON_MEM_PTR_TYPE)(&pHDB->pPCtrl[PortIndex].DMIMap.A0_Mirror[Offset]),
                     Size);

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_DMI_ReadOffset_A0, pHDB:0x%X", pHDB);
}
/*---------------------- end [subroutine] ---------------------------------*/


/*=============================================================================
 * function name: POF_DMI_ReadOffset_A2()
 *
 * parameters:    
 *
 * return value:  LSA_VOID
 *
 * description:
 *===========================================================================*/
LSA_VOID  POF_LOCAL_FCT_ATTR  POF_DMI_ReadOffset_A2( LSA_UINT16               const  PortIndex,
                                                     LSA_UINT8                const  Offset,
                                                     LSA_UINT32               const  Size,
                                                     POF_COMMON_MEM_PTR_TYPE         pAdr,
                                                     POF_HDB_PTR_TYPE         const  pHDB )
{
  //check range

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_DMI_ReadOffset_A2, pHDB:0x%X", pHDB);

  if ((Offset + Size) > I2C_DMI_MAP_A0_SIZE_8)
  {
    POF_PROGRAM_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_DMI_ReadOffset_A2, Offset:0x%X Size:0x%X", Offset, Size);
    POF_FATAL("POF_DMI_ReadOffset_A2, Offset: Size:", POF_FATAL_ERR_PRM_READ_RECORD_INDEX, Offset, Size);
    //lint -unreachable
    return;
  }

  if ((Size > I2C_DMI_MAP_A0_SIZE_8) && (Size < 1UL))
  {
    POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_DMI_ReadOffset_A2, Size:0x%X", Size);
    POF_FATAL("POF_DMI_ReadOffset_A2, Size:", POF_FATAL_ERR_PRM_READ_RECORD_INDEX, Size, 0);
    //lint -unreachable
    return;
  }

  POF_MEMCOPY_COMMON((POF_COMMON_MEM_PTR_TYPE)pAdr,
                     (POF_COMMON_MEM_PTR_TYPE)(&pHDB->pPCtrl[PortIndex].DMIMap.A2_Mirror.Array_8[Offset]),
                     Size);

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_DMI_ReadOffset_A2, pHDB:0x%X", pHDB);
}
/*---------------------- end [subroutine] ---------------------------------*/

static LSA_UINT32 POF_LOCAL_FCT_ATTR POF_Write_SFP_DiagTestData2HW(LSA_UINT16                const  PortIndex,
                                                              POF_HDB_PTR_TYPE         const  pHDB,
                                                        const POF_SFP_DIAG_TEST_TYPE * const  pSFP_DiagTest,
                                                              LSA_UINT8  *             const  pBuf)
{
    union
    {
        LSA_UINT8   TempDataBuffer[2];
        LSA_UINT16  TempValue;
    } u                     = {{0}};    //lint !e708
    LSA_UINT8 * pData       = pBuf;
    EDD_RSP     EDDRetVal   = EDD_STS_ERR_EXCP;

    if (0xFFFF != pSFP_DiagTest->ManipulationMask)
    {
        EDDRetVal = EDD_STS_ERR_EXCP;

        POF_I2C_READ_OFFSET_Internal((LSA_UINT16 POF_LOCAL_MEM_ATTR *)&EDDRetVal, //ret_val_ptr
                                     pHDB,
                                     PortIndex,                                   //PortIndex
                                     POF_ADR_PAGE_A2,                             //I2CDevAddr
                                     POF_I2C_STD_OFFSET_CNT,                      //I2COffsetCnt
                                     pSFP_DiagTest->ManipulationWriteAddress,     //I2COffset1
                                     0,                                           //I2COffset2
                                     2UL,                                         //Size  use 2 ALWAYS !!!! for now
                                     u.TempDataBuffer);                           //pBuf

        if (EDDRetVal != EDD_STS_OK)
        {
            POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_Write_SFP_DiagTestData2HW, "
                                 "POF_I2C_READ_OFFSET with invalid ret_val:0x%X", EDDRetVal);
            u.TempValue = 0;
        }

        u.TempValue         &= ~(pSFP_DiagTest->ManipulationMask);
        u.TempDataBuffer[0] |=   pBuf[0];
        u.TempDataBuffer[1] |=   pBuf[1];

        pData = u.TempDataBuffer;
    }

    EDDRetVal = EDD_STS_ERR_EXCP;

    POF_I2C_WRITE_OFFSET_Internal((LSA_UINT16 POF_LOCAL_MEM_ATTR *)&EDDRetVal, //ret_val_ptr
                                  pHDB,
                                  PortIndex,                                   //PortIndex
                                  POF_ADR_PAGE_A2,                             //I2CDevAddr
                                  POF_I2C_STD_OFFSET_CNT,                      //I2COffsetCnt
                                  pSFP_DiagTest->ManipulationWriteAddress,     //I2COffset1
                                  0,                                           //I2COffset2
                                  2,                                           //Size  use 2 ALWAYS !!!! for now
                                  pData);                                      //pBuf

    if (EDDRetVal != EDD_STS_OK)
    {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_Write_SFP_DiagTestData2HW, "
                             "POF_I2C_WRITE_OFFSET with invalid ret_val:0x%X", EDDRetVal);
        return 0;
    }

    return pSFP_DiagTest->ManipulationBitMask;
}

/*---------------------- end [subroutine] ---------------------------------*/

LSA_UINT32  POF_LOCAL_FCT_ATTR POF_Write_SFP_DiagTestData( LSA_UINT16        const  PortIndex,
                                                           POF_HDB_PTR_TYPE  const  pHDB,
                                                           LSA_UINT32        const  ManipulationRequestFlags)
{
  //EDD_RSP                       EDDRetVal = EDD_STS_ERR_EXCP;
  LSA_UINT32                    PropIndex;
  POF_EDD_PCTRL_PTR_TYPE  const pPCtrl                     = &pHDB->pPCtrl[PortIndex];
  LSA_UINT32                    ManipulationStatus         =  pPCtrl->SFP_DiagTest.CurrentManipulationStatus;
  LSA_UINT32                    ManipulationActionsNeeded  = (ManipulationStatus ^ ManipulationRequestFlags);

  if (0 == ManipulationActionsNeeded)        // everything already set as requested
  {
      return ManipulationRequestFlags;
  }

/*
  // write password and table 3 select to page A2
  if (EDD_STS_OK != (EDDRetVal = POF_I2C_Write_ByteArray( PortIndex, pHDB,
                                                          pPCtrl->SFP_DiagTest.PasswordAddress,
                                                          pPCtrl->SFP_DiagTest.PasswordLength,
                                                          pPCtrl->SFP_DiagTest.Password)))
  {
      POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,
                              "Check4SFP_TestTranceiverManipulations, POF_I2C_Write_ByteArray "
                              "writing password with invalid ret_val:0x%X", EDDRetVal);
      return pPCtrl->SFP_DiagTest.CurrentManipulationStatus;
  }
*/

  for (PropIndex = 0; PropIndex < POF_SFP_NUMBER_CHECK_DIAGNOSTICS; ++PropIndex)
  {
      POF_SFP_DIAG_TEST_PAIR_TYPE *  const  pDiagnosticPair = &pPCtrl->SFP_DiagTest.SFP_DiagTestData[PropIndex];
      LSA_UINT8                             CurrentDiagnosticStatus = 0;

      if (0 == (ManipulationActionsNeeded & ( pDiagnosticPair->HighValue.ManipulationBitMask
                                            | pDiagnosticPair->LowValue.ManipulationBitMask)))
      {
          continue;
      }

      if (ManipulationStatus & pDiagnosticPair->HighValue.ManipulationBitMask)
      {
          CurrentDiagnosticStatus = 2;
      }
      else if (ManipulationStatus & pDiagnosticPair->LowValue.ManipulationBitMask)
      {
          CurrentDiagnosticStatus = 1;
      }

      switch (CurrentDiagnosticStatus)
      {
          case 0:   // currently this diagnostic is not manipulated
              {
                  if (ManipulationRequestFlags & pDiagnosticPair->HighValue.ManipulationBitMask)
                  {
                      ManipulationStatus |= POF_Write_SFP_DiagTestData2HW(PortIndex, pHDB, &pDiagnosticPair->HighValue,
                                                                          pDiagnosticPair->HighValue.Manipulation.Value_8);
                  }
                  else
                  {
                      ManipulationStatus |= POF_Write_SFP_DiagTestData2HW(PortIndex, pHDB, &pDiagnosticPair->LowValue,
                                                                          pDiagnosticPair->LowValue.Manipulation.Value_8);
                  }
              }
              break;

          case 1:   // currently the low value of this diagnostic is manipulated - reset it
              {
                  ManipulationStatus &= ~(POF_Write_SFP_DiagTestData2HW(PortIndex, pHDB, &pDiagnosticPair->LowValue,
                                                                        pDiagnosticPair->LowValue.Original.Value_8));

                  if (ManipulationRequestFlags & pDiagnosticPair->HighValue.ManipulationBitMask)
                  {
                      ManipulationStatus |= POF_Write_SFP_DiagTestData2HW(PortIndex, pHDB, &pDiagnosticPair->HighValue,
                                                                          pDiagnosticPair->HighValue.Manipulation.Value_8);
                  }
              }
              break;

          case 2:   // currently the high value of this diagnostic is manipulated - reset it
              {
                  ManipulationStatus &= ~(POF_Write_SFP_DiagTestData2HW(PortIndex, pHDB, &pDiagnosticPair->HighValue,
                                                                        pDiagnosticPair->HighValue.Original.Value_8));

                  if (ManipulationRequestFlags & pDiagnosticPair->LowValue.ManipulationBitMask)
                  {
                      ManipulationStatus |= POF_Write_SFP_DiagTestData2HW(PortIndex, pHDB, &pDiagnosticPair->LowValue,
                                                                          pDiagnosticPair->LowValue.Manipulation.Value_8);
                  }
              }
              break;

          default: break;
      }
  }

/*
  if (EDD_STS_OK != (EDDRetVal = POF_I2C_Write_ByteArray( PortIndex, pHDB,
                                                          pPCtrl->SFP_DiagTest.PasswordAddress,
                                                          pPCtrl->SFP_DiagTest.PasswordLength,
                                                          pPCtrl->SFP_DiagTest.ResetPassword)))
  {
      POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,
                              "POF_Write_SFP_DiagTestData, POF_I2C_Write_ByteArray "
                              "resetting password with invalid ret_val:0x%X", EDDRetVal);
  }
*/

  return ManipulationStatus;
}


/*=============================================================================
 * function name: POF_TransceiverSMTransition()
 *
 * parameters:    
 *
 * return value:  LSA_VOID
 *
 * description:
 *===========================================================================*/
LSA_VOID POF_LOCAL_FCT_ATTR POF_TransceiverSMTransition(LSA_UINT16              const   PortIndex,
                                                        POF_TRANSCEIVER_TRIGGER_TYPE    Trigger,
                                                        POF_HDB_PTR_TYPE        const   pHDB)
{
    POF_EDD_PCTRL_PTR_TYPE  const  pPCtrl = &pHDB->pPCtrl[PortIndex];

    POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_TransceiverSMTransition, pHDB:0x%X", pHDB);

    switch (Trigger)
    {
        case POF_TRANSCEIVER_TRIGGER_START:
            {
                if (POF_TRANSCEIVER_STATE_STOPPED == pPCtrl->TransceiverState)
                {
                    pPCtrl->TransceiverState = POF_TRANSCEIVER_STATE_PULLED;
                    POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_TransceiverSMTransition, port:%d, POF_TRANSCEIVER_STATE_STOPPED -> POF_TRANSCEIVER_STATE_PULLED", PortIndex);
                }
            }
            break;

        case POF_TRANSCEIVER_TRIGGER_STOP:
            {
                pPCtrl->TestTranceiverDetected = LSA_FALSE;
                pPCtrl->TransceiverState = POF_TRANSCEIVER_STATE_STOPPED;
        
                POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_TransceiverSMTransition, port:%d, any state -> POF_TRANSCEIVER_STATE_STOPPED", PortIndex);
            }
            break;
        
        case POF_TRANSCEIVER_TRIGGER_CYCLIC:
            {
                //If we get the cyclic tigger and no I2C_AVAILABLE (which should not happen) the SM is in the state POF_TRANSCEIVER_STATE_STOPPED and nothing happens.
                switch (pPCtrl->TransceiverState)
                {
                    case POF_TRANSCEIVER_STATE_PULLED:
                        if (EDD_PORT_OPTICALTYPE_ISSFP != pPCtrl->LinkStatus.OpticalTransType)
                        {
                            if  /* valid transceiver exists at I2C port */
                                (POF_DMI_Validate_HW(PortIndex, pHDB))
                            {
                                pPCtrl->DiagStateFct(PortIndex, pHDB, POF_PRM_EV_GOOD, 0 /*AddValue*/);
                                pPCtrl->TransceiverState = POF_TRANSCEIVER_STATE_WRITE_THR;
                                POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_TransceiverSMTransition, port:%d, POF_TRANSCEIVER_STATE_PULLED -> POF_TRANSCEIVER_STATE_WRITE_THR", PortIndex);
                            }
                        }
                        else
                        {
                            if  /* valid transceiver exists at I2C port */
                                (SFP_DMI_Validate_HW(PortIndex, pHDB))
                            {
                                LSA_UINT32   TestableTranceiver;

                                // ignore pPCtrl->DiagStateFct(PortIndex, pHDB, POF_PRM_EV_GOOD, 0 /*AddValue*/);
                                pPCtrl->TransceiverState = POF_TRANSCEIVER_STATE_PLUGGED;
                                POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_TransceiverSMTransition, port:%d, POF_TRANSCEIVER_STATE_PULLED(SFP) -> POF_TRANSCEIVER_STATE_PLUGGED", PortIndex);

                                if ((TestableTranceiver = Check4SFP_TestTranceiver(PortIndex, pHDB)) > 0)
                                {
                                    POF_MEMCOPY_COMMON(&pPCtrl->SFP_DiagTest, &sgSFP_DiagnosticTest.TestTranceivers[TestableTranceiver - 1],
                                                        sizeof(POF_SFP_PORT_DIAG_TEST_TYPE));
                                    pPCtrl->TestTranceiverDetected                 = LSA_TRUE;
                                    pPCtrl->SFP_DiagTest.SupportedManipulationMask = SFP_DIAG_TRIGGER_SET_ALL;   //test tranceiver must!! support all diagnoses

                                    Check4SFP_TestTranceiverManipulations(PortIndex, pHDB);
                                }
                            }
                        }

                        break;
                    case POF_TRANSCEIVER_STATE_WRITE_THR:
                        if  /* PDPortFODataCheck does not exist */
                            (!POF_DMI_IsThresholdCheckEnable(PortIndex, pHDB))
                        {
                            if  /* "real" POF port */
                                (   (EDD_MEDIATYPE_FIBER_OPTIC_CABLE == pPCtrl->LinkStatus.MediaType)
                                 && (EDD_PORT_OPTICALTYPE_ISPOF      == pPCtrl->LinkStatus.OpticalTransType)     )
                            {
                                /* Start polling of transceiver */
                                pPCtrl->TransceiverState = POF_TRANSCEIVER_STATE_PLUGGED;
                                POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_TransceiverSMTransition, port:%d, POF_TRANSCEIVER_STATE_WRITE_THR -> POF_TRANSCEIVER_STATE_PLUGGED, no PDPortFODataCheck", PortIndex);
                            }
                            else
                            {
                                /* Do not start polling of transceiver */
                                POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_TransceiverSMTransition, port:%d, keep POF_TRANSCEIVER_STATE_WRITE_THR", PortIndex);
                            }
                        }
                        else if /* PDPortFODataCheck exists and successful transfered to transceiver */
                                (POF_DMI_WriteThreshold(PortIndex, pHDB))
                        {
                            /* Start polling of transceiver */
                            pPCtrl->TransceiverState = POF_TRANSCEIVER_STATE_PLUGGED;
                            POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_TransceiverSMTransition, port:%d, POF_TRANSCEIVER_STATE_WRITE_THR -> POF_TRANSCEIVER_STATE_PLUGGED, WriteThreshold ok", PortIndex);
                        }
                        else
                        {
                            /* PDPortFODataCheck exists, but could not be transfered to transceiver  */
                            pPCtrl->DiagStateFct(PortIndex, pHDB, POF_PRM_EV_ERROR, 0 /*AddValue*/);
                            pPCtrl->TransceiverState = POF_TRANSCEIVER_STATE_PULLED;
                            POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_TransceiverSMTransition, port:%d, POF_TRANSCEIVER_STATE_WRITE_THR -> POF_TRANSCEIVER_STATE_PULLED", PortIndex);
                        }
                        break;
                    case POF_TRANSCEIVER_STATE_PLUGGED:
                        if  /* polling of tranceiver failed */
                            (!pPCtrl->ReadHW_MarginFct(PortIndex, pHDB))  // POF_DMI_ReadHW_Margin
                        {
                            if (POF_IS_POF_PORT(PortIndex, pHDB))
                            {
                                if  /* PDPortFODataCheck exists */
                                    (POF_DMI_IsThresholdCheckEnable(PortIndex, pHDB))
                                {
                                    /* error diagnosis */
                                    pPCtrl->DiagStateFct(PortIndex, pHDB, POF_PRM_EV_ERROR, 0 /*AddValue*/);
                                }
                            }
                            // dont care for SFP ports

                            pPCtrl->TestTranceiverDetected = LSA_FALSE;
                            pPCtrl->TransceiverState       = POF_TRANSCEIVER_STATE_PULLED;

                            POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_TransceiverSMTransition, port:%d, POF_TRANSCEIVER_STATE_PLUGGED -> POF_TRANSCEIVER_STATE_PULLED", PortIndex);
                        }
                        break;
                    case POF_TRANSCEIVER_STATE_STOPPED:
                        //Do nothing if pulled by ChangePort Service
                        break;
                    default:
                        /* Should not occur! Fatal error! */
                        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_TransceiverSMTransition, Unsupported State enum:0x%X", pPCtrl->TransceiverState);
                        POF_FATAL("POF_TransceiverSMTransition, Unsupported State enum:", POF_FATAL_ERR_EDD, pPCtrl->TransceiverState, 0);
                        break;
                }
            }
            break;

        case POF_TRANSCEIVER_TRIGGER_PRM_WRITTEN:
            {
                if (POF_TRANSCEIVER_STATE_PLUGGED == pPCtrl->TransceiverState)
                {
                    if (EDD_PORT_OPTICALTYPE_ISSFP != pPCtrl->LinkStatus.OpticalTransType)
                    {
                        pPCtrl->TransceiverState = POF_TRANSCEIVER_STATE_WRITE_THR;
                        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_TransceiverSMTransition, port:%d, POF_TRANSCEIVER_STATE_PLUGGED -> POF_TRANSCEIVER_STATE_WRITE_THR", PortIndex);
                    }
                }
            }
            break;

        default:
            {
                /* Should not occur! Fatal error! */
                POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_TransceiverSMTransition, Unsupported Trigger enum:0x%X", Trigger);
                POF_FATAL("POF_TransceiverSMTransition, Unsupported Trigger enum:", POF_FATAL_ERR_EDD, Trigger, 0);
            }            
            break;
    }

    POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_TransceiverSMTransition, pHDB:0x%X", pHDB);
}
/*---------------------- end [subroutine] ---------------------------------*/

/*=============================================================================
 * function name: POF_TransceiverSMTransition()
 *
 * parameters:    
 *
 * return value:  LSA_BOOL
 *
 * description:
 *===========================================================================*/
LSA_BOOL POF_LOCAL_FCT_ATTR POF_TransceiverIsPulled(  LSA_UINT16       const PortIndex,
                                                      POF_HDB_PTR_TYPE const pHDB)
{
    POF_EDD_PCTRL_PTR_TYPE  const  pPCtrl = &pHDB->pPCtrl[PortIndex];
    return ((POF_TRANSCEIVER_STATE_PULLED == pPCtrl->TransceiverState) || (POF_TRANSCEIVER_STATE_STOPPED == pPCtrl->TransceiverState));
}
/*---------------------- end [subroutine] ---------------------------------*/

/*=============================================================================
 * function name: POF_TransceiverSMTransition()
 *
 * parameters:    
 *
 * return value:  LSA_BOOL
 *
 * description:
 *===========================================================================*/
LSA_BOOL POF_LOCAL_FCT_ATTR POF_TransceiverIsPlugged( LSA_UINT16       const PortIndex,
                                                      POF_HDB_PTR_TYPE const pHDB)
{
    POF_EDD_PCTRL_PTR_TYPE  const  pPCtrl = &pHDB->pPCtrl[PortIndex];
    return (POF_TRANSCEIVER_STATE_PLUGGED == pPCtrl->TransceiverState);
}
/*---------------------- end [subroutine] ---------------------------------*/


static LSA_VOID   POF_LOCAL_FCT_ATTR POF_I2C_READ_OFFSET_Internal(LSA_UINT16 *     const  ret_val_ptr,
                                                                  POF_HDB_PTR_TYPE const  pHDB,
                                                                  LSA_UINT32       const  PortIndex,
                                                                  LSA_UINT8        const  I2CDevAddr,
                                                                  LSA_UINT8        const  I2COffsetCnt,
                                                                  LSA_UINT8        const  I2COffset1,
                                                                  LSA_UINT8        const  I2COffset2,
                                                                  LSA_UINT32       const  Size,
                                                                  LSA_UINT8  *     const  pBuf)
{
    LSA_UINT32  i;

    //This should never happen!!
    if (pHDB->I2C[PortIndex].I2CAvailable != POF_I2C_AVAILABLE)
    {
        POF_FATAL("POF_I2C_READ_OFFSET without I2C_AVAILABLE!", POF_FATAL_ERR_EDD, 0, 0);
    }

    for (i = 0; i < 3; ++i)
    {
        *ret_val_ptr = EDD_STS_ERR_EXCP;
    
        POF_WAIT_US(10000);

        POF_I2C_READ_OFFSET(ret_val_ptr, pHDB->Params.pSys, (PortIndex + 1UL), pHDB->I2C[PortIndex].I2CMuxSelect, I2CDevAddr, I2COffsetCnt,   /* PortId = PortIndex+1 */
                            I2COffset1, I2COffset2, Size, pBuf);

        if (EDD_STS_OK == *ret_val_ptr)
        {
            break;
        }
    }

    return;
}


static LSA_VOID  POF_LOCAL_FCT_ATTR POF_I2C_WRITE_OFFSET_Internal(LSA_UINT16 *     const  ret_val_ptr,
                                                                  POF_HDB_PTR_TYPE const  pHDB,
                                                                  LSA_UINT32       const  PortIndex,
                                                                  LSA_UINT8        const  I2CDevAddr,
                                                                  LSA_UINT8        const  I2COffsetCnt,
                                                                  LSA_UINT8        const  I2COffset1,
                                                                  LSA_UINT8        const  I2COffset2,
                                                                  LSA_UINT32       const  Size,
                                                                  LSA_UINT8  *     const  pBuf)
{
    LSA_UINT32  i;

    //This should never happen!!
    if (pHDB->I2C[PortIndex].I2CAvailable != POF_I2C_AVAILABLE)
    {
      POF_FATAL("POF_I2C_READ_OFFSET without I2C_AVAILABLE!", POF_FATAL_ERR_EDD, 0, 0);
    }

    for (i = 0; i < 3; ++i)
    {
        *ret_val_ptr = EDD_STS_ERR_EXCP;

        POF_WAIT_US(10000);
        POF_I2C_WRITE_OFFSET(ret_val_ptr, pHDB->Params.pSys, (PortIndex + 1UL), pHDB->I2C[PortIndex].I2CMuxSelect, I2CDevAddr, I2COffsetCnt,   /* PortId = PortIndex+1 */
                             I2COffset1, I2COffset2, Size, pBuf);

        if (EDD_STS_OK == *ret_val_ptr)
        {
            break;
        }
    }
    POF_WAIT_US(10000);

    return;
}


/*****************************************************************************/
/*  end of file pof_dmi.c                                                    */
/*****************************************************************************/
