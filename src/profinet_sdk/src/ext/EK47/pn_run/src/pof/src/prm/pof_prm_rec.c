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
/*  F i l e               &F: pof_prm_rec.c                             :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  POF PRM REC                                      */
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
#define LTRC_ACT_MODUL_ID  8
#define POF_MODULE_ID      LTRC_ACT_MODUL_ID

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/
#include "pof_inc.h"
#include "pof_dmi_type.h"
#include "pof_prm_type.h"
#include "pof_int.h"
#include "pof_prm_req.h"
#include "pof_prm_rec.h"
#include "pof_dmi.h"
#include "pof_glb.h"

POF_FILE_SYSTEM_EXTENSION(POF_MODULE_ID)

/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/
#define  POF_ADR_MANUFACTURER          20   
#define  POF_ADR_MANUFACTURER_OUI      37   
#define  POF_ADR_PARTNUMBER            40   
#define  POF_ADR_REVISION              56   
#define  POF_ADR_WAVELENGTH            60   
#define  POF_ADR_TRANSCEIVEROPTIONS    65   
#define  POF_ADR_SERIALNUMBER          68   
#define  POF_ADR_DATECODE              84   

#define  POF_LENGTH_MANUFACTURER       16UL  
#define  POF_LENGTH_MANUFACTURER_OUI   3UL   
#define  POF_LENGTH_PARTNUMBER         16UL  
#define  POF_LENGTH_REVISION           4UL   
#define  POF_LENGTH_WAVELENGTH         2UL   
#define  POF_LENGTH_TRANSCEIVEROPTIONS 1UL   
#define  POF_LENGTH_SERIALNUMBER       16UL  
#define  POF_LENGTH_DATECODE           8UL   

/*===========================================================================*/
/*                                global Variables                           */
/*===========================================================================*/

/*===========================================================================*/
/*                                typedefs                                   */
/*===========================================================================*/

/*===========================================================================*/
/*                            local function declaration                     */
/*===========================================================================*/

/*===========================================================================*/
/*                                 Macros                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                 functions                                 */
/*===========================================================================*/

/*=============================================================================
 * function name: POF_GetPowerBudget()
 *
 * parameters:    
 *
 * return value:  LSA_VOID
 *
 * description:
 *===========================================================================*/
LSA_VOID  POF_LOCAL_FCT_ATTR  POF_GetPowerBudget( LSA_UINT16                  const  PortIndex,
                                                  POF_LOCAL_MEM_U32_PTR_TYPE  const  pPowerBudget_0_1_dB,
                                                  POF_HDB_PTR_TYPE            const  pHDB )
{
  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_GetPowerBudget, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);

  if (POF_TransceiverIsPlugged(PortIndex, pHDB))
  {
    //A2_Mirror.u.ElementPOF.RealTime_Rx_OMA_Margin (= POF-Transceiver-Register) is decoded as an 8 bit signed twos compliment integer in increments of 0,2 dB
    LSA_UINT8  const  RealTime_Rx_OMA_Margin_0_2_dB = pHDB->pPCtrl[PortIndex].DMIMap.A2_Mirror.u.ElementPOF.RealTime_Rx_OMA_Margin;

    if //RealTime_Rx_OMA_Margin is not negative?
       (RealTime_Rx_OMA_Margin_0_2_dB < (LSA_UINT8)0x80)
    {
      LSA_UINT32  const  RealTime_Rx_OMA_Margin_0_1_dB = (LSA_UINT32)((LSA_UINT32)2 * RealTime_Rx_OMA_Margin_0_2_dB);

      if (RealTime_Rx_OMA_Margin_0_1_dB > (LSA_UINT32)POF_STRESSED_LINK_PENALTY_0_1_DB)
      {
        /* PowerBudget 0,1 dB */
        *pPowerBudget_0_1_dB = RealTime_Rx_OMA_Margin_0_1_dB - (LSA_UINT32)POF_STRESSED_LINK_PENALTY_0_1_DB;
      }
      else
      {
        /* PowerBudget 0,1 dB */
        *pPowerBudget_0_1_dB = 0;
      }
    }
    else //RealTime_Rx_OMA_Margin is negative
    {
      /* PowerBudget 0,1 dB */
      *pPowerBudget_0_1_dB = 0;
    }

    POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_GetPowerBudget, PowerBudget_0_1_dB:0x%X",
                         *pPowerBudget_0_1_dB);
  }
  else
  {
    POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_GetPowerBudget, no POF-Port");
    POF_FATAL("POF_GetPowerBudget, no POF-Port", POF_FATAL_ERR_EDD, 0, 0);
    //lint -unreachable
    return;
  }

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_GetPowerBudget, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);
}
/*---------------------- end [subroutine] ---------------------------------*/


/*=============================================================================
 * function name: POF_Read_FO_DataReal()
 *
 * parameters:    
 *
 * return value:  LSA_VOID
 *
 * description:    
 *===========================================================================*/
LSA_VOID  POF_LOCAL_FCT_ATTR  POF_Read_FO_DataReal( LSA_UINT16                           const  PortIndex,
                                                    LSA_BOOL                             const  bLinkUp,
                                                    POF_UPPER_PRM_FO_DATA_REAL_PTR_TYPE  const  pRecordData,
                                                    POF_HDB_PTR_TYPE                     const  pHDB )
{
  LSA_UINT32                     PowerBudget_0_1_dB;
  LSA_UINT32                     N_PowerBudget_0_1_dB;
  POF_EDD_PCTRL_PTR_TYPE  const  pPCtrl = &pHDB->pPCtrl[PortIndex];

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_Read_FO_DataReal, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);

  //All Data in Big-Endian (N_...)

  /* FiberOpticType */
  pRecordData->N_FiberOpticType      = pPCtrl->RecordSet_A.LN_FiberOpticType;

  /* FiberOpticCableType */
  pRecordData->N_FiberOpticCableType = pPCtrl->RecordSet_A.LN_FiberOpticCableType;

  POF_PROGRAM_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_Read_FO_DataReal, N_FiberOpticType:0x%X N_FiberOpticCableType:0x%X",
                       pRecordData->N_FiberOpticType, pRecordData->N_FiberOpticCableType);

  /*** No Linkup exists ***/
  if (!bLinkUp)
  {
    pRecordData->N_FiberOpticPowerBudgetReal = 0;

    POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_Read_FO_DataReal, Linkdown, N_FiberOpticPowerBudgetReal:0x0");

    return;
  }

  POF_GetPowerBudget(PortIndex, &PowerBudget_0_1_dB, pHDB);
  N_PowerBudget_0_1_dB = (LSA_UINT32)POF_HTONT_32(PowerBudget_0_1_dB);

  /* FiberOpticPowerBudgetReal 0,1 dB */
  pRecordData->N_FiberOpticPowerBudgetReal = N_PowerBudget_0_1_dB;

  POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_Read_FO_DataReal, N_FiberOpticPowerBudgetReal:0x%X", pRecordData->N_FiberOpticPowerBudgetReal);

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_Read_FO_DataReal, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);
}
/*---------------------- end [subroutine] ---------------------------------*/


/*=============================================================================
 * function name: POF_Read_FO_DataRealShort()
 *
 * parameters:    
 *
 * return value:  LSA_VOID
 *
 * description:    
 *===========================================================================*/
LSA_VOID  POF_LOCAL_FCT_ATTR  POF_Read_FO_DataRealShort( LSA_UINT16                      const  PortIndex,
                                                    POF_UPPER_PRM_FO_DATA_REAL_PTR_TYPE  const  pRecordData,
                                                    POF_HDB_PTR_TYPE                     const  pHDB )
{
  POF_EDD_PCTRL_PTR_TYPE  const  pPCtrl = &pHDB->pPCtrl[PortIndex];

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_Read_FO_DataRealShort, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);

  //All Data in Big-Endian (N_...)

  /* FiberOpticType */
  pRecordData->N_FiberOpticType      = pPCtrl->RecordSet_A.LN_FiberOpticType;

  /* FiberOpticCableType */
  pRecordData->N_FiberOpticCableType = pPCtrl->RecordSet_A.LN_FiberOpticCableType;

  POF_PROGRAM_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_Read_FO_DataRealShort, N_FiberOpticType:0x%X N_FiberOpticCableType:0x%X",
                       pRecordData->N_FiberOpticType, pRecordData->N_FiberOpticCableType);

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_Read_FO_DataRealShort, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);
}
/*---------------------- end [subroutine] ---------------------------------*/


/*=============================================================================
 * function name: POF_Read_FO_DataCheck()
 *
 * parameters:    
 *
 * return value:  LSA_VOID
 *
 * description:
 *===========================================================================*/
LSA_VOID  POF_LOCAL_FCT_ATTR  POF_Read_FO_DataCheck( LSA_UINT16                            const  PortIndex,
                                                     POF_UPPER_PRM_FO_DATA_CHECK_PTR_TYPE  const  pRecordData,
                                                     POF_HDB_PTR_TYPE                      const  pHDB )
{
  POF_EDD_PCTRL_PTR_TYPE  const  pPCtrl = &pHDB->pPCtrl[PortIndex];

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_Read_FO_DataCheck, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);

  /* All Data in Big-Endian */
  pRecordData->N_MaintenanceRequiredPowerBudget = pPCtrl->RecordSet_A.LN_MRPowerBudget;

  pRecordData->N_MaintenanceDemandedPowerBudget = pPCtrl->RecordSet_A.LN_MDPowerBudget;  

  pRecordData->N_ErrorPowerBudget               = pPCtrl->RecordSet_A.LN_ErrorPowerBudget;  

  POF_PROGRAM_TRACE_03(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_Read_FO_DataCheck, N_MaintenanceRequiredPowerBudget:0x%X N_MaintenanceDemandedPowerBudget:0x%X N_ErrorPowerBudget:0x%X",
                       pRecordData->N_MaintenanceRequiredPowerBudget, pRecordData->N_MaintenanceDemandedPowerBudget, pRecordData->N_ErrorPowerBudget);

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_Read_FO_DataCheck, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);
}
/*---------------------- end [subroutine] ---------------------------------*/


/*=============================================================================
 * function name: POF_Read_FO_DataAdjust()
 *
 * parameters:    
 *
 * return value:  LSA_VOID
 *
 * description:    
 *===========================================================================*/
LSA_VOID  POF_LOCAL_FCT_ATTR  POF_Read_FO_DataAdjust( LSA_UINT16                             const  PortIndex,
                                                      POF_UPPER_PRM_FO_DATA_ADJUST_PTR_TYPE  const  pRecordData,
                                                      POF_HDB_PTR_TYPE                       const  pHDB )
{
  POF_EDD_PCTRL_PTR_TYPE  const  pPCtrl = &pHDB->pPCtrl[PortIndex];

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_Read_FO_DataAdjust, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);

  /* All Data in Big-Endian */
  pRecordData->N_FiberOpticType      = pPCtrl->RecordSet_A.LN_FiberOpticType;  

  pRecordData->N_FiberOpticCableType = pPCtrl->RecordSet_A.LN_FiberOpticCableType;  

  POF_PROGRAM_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_Read_FO_DataAdjust, N_FiberOpticType:0x%X N_FiberOpticCableType:0x%X", 
                       pRecordData->N_FiberOpticType, pRecordData->N_FiberOpticCableType);

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_Read_FO_DataAdjust, PortIndex:0x%X pHDB:0x%X", 
                        PortIndex, pHDB);
}
/*---------------------- end [subroutine] ---------------------------------*/


/*=============================================================================
 * function name: POF_Read_SFP_DataCheck()
 *
 * parameters:    
 *
 * return value:  LSA_VOID
 *
 * description:
 *===========================================================================*/
LSA_VOID  POF_LOCAL_FCT_ATTR  POF_Read_SFP_DataCheck( LSA_UINT16                             const  PortIndex,
                                                      POF_UPPER_PRM_SFP_DATA_CHECK_PTR_TYPE  const  pRecordData,
                                                      POF_HDB_PTR_TYPE                       const  pHDB )
{
  POF_EDD_PCTRL_PTR_TYPE  const  pPCtrl = &pHDB->pPCtrl[PortIndex];

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_Read_SFP_DataCheck, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);

  /* All Data in Big-Endian */
  pRecordData->N_MaintenanceDemandedAdminStatus = pPCtrl->RecordSet_A.SFP_MaintenanceDemandedAdminStatus;
  pRecordData->N_ErrorAdminStatus               = pPCtrl->RecordSet_A.SFP_ErrorAdminStatus;

  POF_PROGRAM_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_Read_SFP_DataCheck, N_MaintenanceDemandedAdminStatus:0x%X, N_ErrorAdminStatus:0x%X",
                       pRecordData->N_MaintenanceDemandedAdminStatus, pRecordData->N_ErrorAdminStatus);

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_Read_SFP_DataCheck, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);
}
/*---------------------- end [subroutine] ---------------------------------*/


/*=============================================================================
 * function name: POF_Read_SFP_DiagTest()
 *
 * parameters:    
 *
 * return value:  LSA_VOID
 *
 * description:
 *===========================================================================*/
LSA_VOID  POF_LOCAL_FCT_ATTR  POF_Read_SFP_DiagTest( LSA_UINT16                             const  PortIndex,
                                                     POF_UPPER_PRM_SFP_DIAG_TEST_PTR_TYPE   const  pRecordData,
                                                     POF_HDB_PTR_TYPE                       const  pHDB )
{
  POF_EDD_PCTRL_PTR_TYPE  const  pPCtrl = &pHDB->pPCtrl[PortIndex];

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_Read_SFP_DiagTest, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);

  /* All Data in Big-Endian */
  pRecordData->N_DiagTrigger = POF_NTOH_32(pPCtrl->SFP_DiagTest.CurrentManipulationStatus);
  
  POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_Read_SFP_DiagTest, N_DiagTrigger:0x%X",
                       pRecordData->N_DiagTrigger);

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_Read_SFP_DiagTest, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);
}
/*---------------------- end [subroutine] ---------------------------------*/


/*=============================================================================
 * function name: POF_Read_Transceiver_Ident()
 *
 * parameters:    
 *
 * return value:  LSA_VOID
 *
 * description:    
 *===========================================================================*/
LSA_VOID  POF_LOCAL_FCT_ATTR  POF_Read_Transceiver_Ident( LSA_UINT16                                 const  PortIndex,
                                                          POF_UPPER_PRM_TRANSCEIVER_INDENT_PTR_TYPE  const  pRecordData,
                                                          POF_HDB_PTR_TYPE                           const  pHDB )
{
  /* All Data in Big-Endian */
  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_Read_Transceiver_Ident, PortIndex:0x%X pHDB:0x%X", 
                        PortIndex, pHDB);

  POF_DMI_ReadOffset_A0(PortIndex, (LSA_UINT8)POF_ADR_MANUFACTURER,       POF_LENGTH_MANUFACTURER,        
                        &pRecordData->N_Manufacturer[0], pHDB);

  POF_DMI_ReadOffset_A0(PortIndex, (LSA_UINT8)POF_ADR_MANUFACTURER_OUI,   POF_LENGTH_MANUFACTURER_OUI,    
                        &pRecordData->N_ManufacturerOUI[0], pHDB);

  pRecordData->N_ManufacturerOUI[3] = 0;

  POF_DMI_ReadOffset_A0(PortIndex, (LSA_UINT8)POF_ADR_PARTNUMBER,         POF_LENGTH_PARTNUMBER,          
                        &pRecordData->N_PartNumber[0], pHDB);

  POF_DMI_ReadOffset_A0(PortIndex, (LSA_UINT8)POF_ADR_REVISION,           POF_LENGTH_REVISION,            
                        &pRecordData->N_Revision[0], pHDB);

  POF_DMI_ReadOffset_A0(PortIndex, (LSA_UINT8)POF_ADR_WAVELENGTH,         POF_LENGTH_WAVELENGTH,          
                        &pRecordData->N_Wavelength[0], pHDB);

  POF_DMI_ReadOffset_A0(PortIndex, (LSA_UINT8)POF_ADR_TRANSCEIVEROPTIONS, POF_LENGTH_TRANSCEIVEROPTIONS,  
                        &pRecordData->N_TransceiverOptions, pHDB);
  
  POF_DMI_ReadOffset_A0(PortIndex, (LSA_UINT8)POF_ADR_SERIALNUMBER,       POF_LENGTH_SERIALNUMBER,        
                        &pRecordData->N_SerialNumber[0], pHDB);

  POF_DMI_ReadOffset_A0(PortIndex, (LSA_UINT8)POF_ADR_DATECODE,           POF_LENGTH_DATECODE,            
                        &pRecordData->N_Datecode[0], pHDB);

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_Read_Transceiver_Ident, PortIndex:0x%X pHDB:0x%X",
                        PortIndex, pHDB);
}
/*---------------------- end [subroutine] ---------------------------------*/


/*=============================================================================
 * function name: POF_Write_FO_DataCheck()
 *
 * parameters:    
 *
 * return value:  POF_RSP
 *
 * description:
 *===========================================================================*/
POF_RSP  POF_LOCAL_FCT_ATTR  POF_Write_FO_DataCheck( LSA_UINT16                                 const  PortIndex,
                                                     POF_UPPER_PRM_FO_DATA_CHECK_CONST_PTR_TYPE const  pRecordData,
                                                     POF_HDB_PTR_TYPE                           const  pHDB )
{
  /* All Data in Record in Big-Endian (N_...) */
  LSA_UINT32                     Value_0_1_db;
  LSA_UINT32              const  MaxValue_0_1_db = 238UL; //= 23,8 dB - strict check according to register size (Transceiver-Register max. 127)
  POF_EDD_PCTRL_PTR_TYPE  const  pPCtrl          = &pHDB->pPCtrl[PortIndex];

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_Write_FO_DataCheck, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);

  POF_PROGRAM_TRACE_03(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_Write_FO_DataCheck, N_ErrorPowerBudget:0x%X N_MaintenanceDemandedPowerBudget:0x%X N_MaintenanceRequiredPowerBudget:0x%X",
                       pRecordData->N_ErrorPowerBudget, pRecordData->N_MaintenanceDemandedPowerBudget, pRecordData->N_MaintenanceRequiredPowerBudget);

  pPCtrl->RecordSet_B.RecordTypeWritten &= ~POF_RECORD_TYPE_DATA_CHECK_8061;

  /*** ErrorPowerBudget (error threshold) ***/
  Value_0_1_db = POF_NTOH_32(pRecordData->N_ErrorPowerBudget);

  /* ErrorPowerBudget.CheckEnable */
  if (Value_0_1_db & 0x80000000UL)
  {
    POF_SET_DETAIL_ERR(POF_ERR_DIAGNOSTIC_ERROR_POWER_ENABLE);
    POF_PRM_SET_ERR_OFFSET(pHDB, POF_PRM_RECORD_FO_DATA_CHECK_OFFSET_N_ErrorPowerBudget);
    return POF_RSP_ERR_PRM_BLOCK;
  }
  else
  {
    pPCtrl->RecordSet_B.ErrorCheckEnable = LSA_FALSE;
  }

  /*** MaintenanceDemandedPowerBudget (alarm threshold) ***/
  Value_0_1_db = POF_NTOH_32(pRecordData->N_MaintenanceDemandedPowerBudget);

  /* FiberOpticPowerBudgetType.CheckEnable */
  if (Value_0_1_db & 0x80000000UL)
  {
    Value_0_1_db = Value_0_1_db & 0x7FFFFFFFUL;

    /* check range for maintenance demanded */
    if (Value_0_1_db > MaxValue_0_1_db)
    {
      POF_SET_DETAIL_ERR(POF_ERR_DIAGNOSTIC_MAIN_DEM_POWER_VALUE);
      POF_PRM_SET_ERR_OFFSET(pHDB, POF_PRM_RECORD_FO_DATA_CHECK_OFFSET_N_MaintenanceDemandedPowerBudget);
      return POF_RSP_ERR_PRM_BLOCK;
    }

    pPCtrl->RecordSet_B.MDPowerBudget_0_1_dB = (LSA_UINT16)Value_0_1_db;

    pPCtrl->RecordSet_B.MDCheckEnable = LSA_TRUE;
  }
  else
  {
    pPCtrl->RecordSet_B.MDCheckEnable = LSA_FALSE;
  }

  /*** MaintenanceRequiredPowerBudget (warning threshold) ***/
  Value_0_1_db = POF_NTOH_32(pRecordData->N_MaintenanceRequiredPowerBudget);

  /* FiberOpticPowerBudgetType.CheckEnable */
  if ((Value_0_1_db & 0x80000000UL) != 0)
  {
    Value_0_1_db = Value_0_1_db & 0x7FFFFFFFUL;

    /* check range for maintenance required */
    if (Value_0_1_db > MaxValue_0_1_db)
    {
      POF_SET_DETAIL_ERR(POF_ERR_DIAGNOSTIC_MAIN_REQ_POWER_VALUE);
      POF_PRM_SET_ERR_OFFSET(pHDB, POF_PRM_RECORD_FO_DATA_CHECK_OFFSET_N_MaintenanceRequiredPowerBudget);
      return POF_RSP_ERR_PRM_BLOCK;
    }

    pPCtrl->RecordSet_B.MRPowerBudget_0_1_dB = (LSA_UINT16)Value_0_1_db;

    pPCtrl->RecordSet_B.MRCheckEnable = LSA_TRUE;
  }
  else
  {
    pPCtrl->RecordSet_B.MRCheckEnable = LSA_FALSE;
  }

  /* Data ok -> POF_RECORD_TYPE_POWER_BUDGET */
  pPCtrl->RecordSet_B.RecordTypeWritten   |= POF_RECORD_TYPE_DATA_CHECK_8061;
  pPCtrl->RecordSet_B.LN_MRPowerBudget     = pRecordData->N_MaintenanceRequiredPowerBudget;
  pPCtrl->RecordSet_B.LN_MDPowerBudget     = pRecordData->N_MaintenanceDemandedPowerBudget;
  pPCtrl->RecordSet_B.LN_ErrorPowerBudget  = pRecordData->N_ErrorPowerBudget;

  POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_Write_FO_DataCheck, RecordTypeWritten:0x%X",
                       pPCtrl->RecordSet_B.RecordTypeWritten);

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_Write_FO_DataCheck, PortIndex:0x%X pHDB:0x%X",
                        PortIndex, pHDB);

  return POF_RSP_OK;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*=============================================================================
 * function name: POF_Write_FO_DataAdjust()
 *
 * parameters:    
 *
 * return value:  POF_RSP
 *
 * description:
 *===========================================================================*/
POF_RSP  POF_LOCAL_FCT_ATTR  POF_Write_FO_DataAdjust( LSA_UINT16                                    const  PortIndex,
                                                      POF_UPPER_PRM_FO_DATA_ADJUST_CONST_PTR_TYPE   const  pRecordData,
                                                      POF_HDB_PTR_TYPE                              const  pHDB )
{
  LSA_UINT32                     Value;
  POF_EDD_PCTRL_PTR_TYPE  const  pPCtrl = &pHDB->pPCtrl[PortIndex];

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_Write_FO_DataAdjust, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);

  POF_PROGRAM_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_Write_FO_DataAdjust, N_FiberOpticType:0x%X N_FiberOpticCableType:0x%X",
                       pRecordData->N_FiberOpticType, pRecordData->N_FiberOpticCableType);

  /* All Data in Big-Endian */
  pPCtrl->RecordSet_B.RecordTypeWritten &= ~POF_RECORD_TYPE_DATA_ADJUST_8062;

  Value = POF_NTOH_32(pRecordData->N_FiberOpticType);

  /* reserved */
  if ((Value >= 0x00000008UL) && (Value <= 0x0000007FUL))
  {
      POF_SET_DETAIL_ERR(POF_ERR_DIAGNOSTIC_FIBEROPTICTYPE);
      POF_PRM_SET_ERR_OFFSET(pHDB, POF_PRM_RECORD_FO_DATA_ADJUST_OFFSET_N_FiberOpticType);
      return POF_RSP_ERR_PRM_BLOCK;
  }

  /* reserved */
  if (Value >= 0x00000100UL)
  {
      POF_SET_DETAIL_ERR(POF_ERR_DIAGNOSTIC_FIBEROPTICTYPE);
      POF_PRM_SET_ERR_OFFSET(pHDB, POF_PRM_RECORD_FO_DATA_ADJUST_OFFSET_N_FiberOpticType);
      return POF_RSP_ERR_PRM_BLOCK;
  }

  /* check FiberOpticCableType, 0x0004 - 0xFFFFFFFF -> reserved */
  Value = POF_NTOH_32(pRecordData->N_FiberOpticCableType);

  /* reserved */
  if (Value >= 0x00000004UL)
  {
    POF_SET_DETAIL_ERR(POF_ERR_DIAGNOSTIC_FIBEROPTICCABLE);
    POF_PRM_SET_ERR_OFFSET(pHDB, POF_PRM_RECORD_FO_DATA_ADJUST_OFFSET_N_FiberOpticCableType);
    return POF_RSP_ERR_PRM_BLOCK;
  }

  /* Data ok -> POF_RECORD_TYPE_FIBER_OPTIC */
  pPCtrl->RecordSet_B.RecordTypeWritten      |= POF_RECORD_TYPE_DATA_ADJUST_8062;
  pPCtrl->RecordSet_B.LN_FiberOpticType       = pRecordData->N_FiberOpticType;
  pPCtrl->RecordSet_B.LN_FiberOpticCableType  = pRecordData->N_FiberOpticCableType;

  POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_Write_FO_DataAdjust, RecordTypeWritten:0x%X",
                       pPCtrl->RecordSet_B.RecordTypeWritten);

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_Write_FO_DataAdjust, PortIndex:0x%X pHDB:0x%X",
                        PortIndex, pHDB);

  return POF_RSP_OK;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*=============================================================================
 * function name: POF_Write_SFP_DataCheck()
 *
 * parameters:    
 *
 * return value:  POF_RSP
 *
 * description:
 *===========================================================================*/
POF_RSP  POF_LOCAL_FCT_ATTR  POF_Write_SFP_DataCheck( LSA_UINT16                                  const  PortIndex,
                                                      POF_UPPER_PRM_SFP_DATA_CHECK_CONST_PTR_TYPE const  pRecordData,
                                                      POF_HDB_PTR_TYPE                            const  pHDB )
{
  /* All Data in Record in Big-Endian (N_...) */
  LSA_UINT32                     TempValue;
  POF_EDD_PCTRL_PTR_TYPE  const  pPCtrl   = &pHDB->pPCtrl[PortIndex];

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_Write_SFP_DataCheck, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);

  POF_PROGRAM_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_Write_SFP_DataCheck, N_MaintenanceDemandedAdminStatus:0x%X, N_ErrorAdminStatus:0x%X",
                       pRecordData->N_MaintenanceDemandedAdminStatus, pRecordData->N_ErrorAdminStatus);

  pPCtrl->RecordSet_B.RecordTypeWritten &= ~POF_RECORD_TYPE_SFP_DATA_CHECK_8063;

  if ((POF_TRANSCEIVER_STATE_PLUGGED != pPCtrl->TransceiverState)  &&  SFP_DMI_Validate_HW(PortIndex, pHDB))
  {
      POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_Write_SFP_DataCheck, calling SFP_DMI_TestTransceiver() for PortIndex:0x%X", PortIndex);
  }

  /*** MaintenanceDemandedDiagFlags (alarm threshold) ***/
  TempValue = POF_NTOH_32(pRecordData->N_MaintenanceDemandedAdminStatus);

  if ((TempValue &  ~(  SFP_TEMPERATURE_CHECK_ON | SFP_TX_BIAS_CHECK_ON
                      | SFP_TX_POWER_CHECK_ON    | SFP_RX_POWER_CHECK_ON)) != 0)
  {
      POF_SET_DETAIL_ERR(POF_ERR_DIAGNOSTIC_MAIN_DEM_POWER_VALUE);
      POF_PRM_SET_ERR_OFFSET(pHDB, POF_PRM_RECORD_SFP_DATA_CHECK_OFFSET_N_MaintenanceDemandedAdminStatus);

      POF_PROGRAM_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_Write_SFP_DataCheck(): check of SFP flags failed! TempValue=0x%X, N_MaintenanceDemandedAdminStatus=0x%X", 
          TempValue, pRecordData->N_MaintenanceDemandedAdminStatus);

      return POF_RSP_ERR_PRM_BLOCK;
  }

  //Flags have to be checked bitwise
  if (TempValue && !pPCtrl->DMIMap.SFP_AlarmFlagsSupported)
  {
      POF_SET_DETAIL_ERR(POF_ERR_DIAGNOSTIC_MAIN_DEM_POWER_VALUE);
      POF_PRM_SET_ERR_OFFSET(pHDB, POF_PRM_RECORD_SFP_DATA_CHECK_OFFSET_N_MaintenanceDemandedAdminStatus);

      POF_PROGRAM_TRACE_03(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_Write_SFP_DataCheck(): check of SFP_AlarmFlagsSupported failed! TempValue=0x%X, SFP_AlarmFlagsSupported=0x%X, N_MaintenanceDemandedAdminStatus=0x%X", 
          TempValue, pPCtrl->DMIMap.SFP_AlarmFlagsSupported, pRecordData->N_MaintenanceDemandedAdminStatus);

      return POF_RSP_ERR_PRM_BLOCK;
  }

  /*** ErrorDiagFlags ***/
  TempValue = POF_NTOH_32(pRecordData->N_ErrorAdminStatus);

  if ((TempValue &  ~(SFP_TX_FAULT_CHECK_ON | SFP_RX_LOSS_FAULT_CHECK_ON)) != 0)
  {
      POF_SET_DETAIL_ERR(POF_ERR_DIAGNOSTIC_MAIN_REQ_POWER_VALUE);
      POF_PRM_SET_ERR_OFFSET(pHDB, POF_PRM_RECORD_SFP_DATA_CHECK_OFFSET_N_ErrorAdminStatus);

      POF_PROGRAM_TRACE_03(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_Write_SFP_DataCheck(): check of ErrorDiag flags failed! TempValue=0x%X, N_ErrorAdminStatus=0x%X, N_MaintenanceDemandedAdminStatus=0x%X", 
          TempValue, pRecordData->N_ErrorAdminStatus, pRecordData->N_MaintenanceDemandedAdminStatus);

      return POF_RSP_ERR_PRM_BLOCK;
  }

  if ((TempValue & SFP_TX_FAULT_CHECK_ON)  &&  !pPCtrl->DMIMap.SFP_TX_FaultSupported)
  {
      POF_SET_DETAIL_ERR(POF_ERR_DIAGNOSTIC_MAIN_REQ_POWER_VALUE);
      POF_PRM_SET_ERR_OFFSET(pHDB, POF_PRM_RECORD_SFP_DATA_CHECK_OFFSET_N_ErrorAdminStatus);

      POF_PROGRAM_TRACE_04(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_Write_SFP_DataCheck(): check of ErrorDiag flags failed! TempValue=0x%X, SFP_TX_FaultSupported=0x%X, N_ErrorAdminStatus=0x%X, N_MaintenanceDemandedAdminStatus=0x%X", 
          TempValue, pPCtrl->DMIMap.SFP_TX_FaultSupported, pRecordData->N_ErrorAdminStatus, pRecordData->N_MaintenanceDemandedAdminStatus);

      return POF_RSP_ERR_PRM_BLOCK;
  }

  if ((TempValue & SFP_RX_LOSS_FAULT_CHECK_ON)  &&  !pPCtrl->DMIMap.SFP_RX_LossSupported)
  {
      POF_SET_DETAIL_ERR(POF_ERR_DIAGNOSTIC_MAIN_REQ_POWER_VALUE);
      POF_PRM_SET_ERR_OFFSET(pHDB, POF_PRM_RECORD_SFP_DATA_CHECK_OFFSET_N_ErrorAdminStatus);

      POF_PROGRAM_TRACE_04(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_Write_SFP_DataCheck(): check of ErrorDiag flags failed! TempValue=0x%X, SFP_RX_LossSupported=0x%X, N_ErrorAdminStatus=0x%X, N_MaintenanceDemandedAdminStatus=0x%X", 
          TempValue, pPCtrl->DMIMap.SFP_RX_LossSupported, pRecordData->N_ErrorAdminStatus, pRecordData->N_MaintenanceDemandedAdminStatus);

      return POF_RSP_ERR_PRM_BLOCK;
  }

  pPCtrl->RecordSet_B.RecordTypeWritten                  |= POF_RECORD_TYPE_SFP_DATA_CHECK_8063;
  pPCtrl->RecordSet_B.SFP_MaintenanceDemandedAdminStatus  = pRecordData->N_MaintenanceDemandedAdminStatus;
  pPCtrl->RecordSet_B.SFP_ErrorAdminStatus                = pRecordData->N_ErrorAdminStatus;

  POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_Write_SFP_DataCheck, RecordTypeWritten:0x%X",
                       pPCtrl->RecordSet_B.RecordTypeWritten);

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_Write_SFP_DataCheck, PortIndex:0x%X pHDB:0x%X",
                        PortIndex, pHDB);

  //pPCtrl->RecordSet_B.RecordTypeWritten   |= POF_RECORD_TYPE_SFP_DATA_CHECK_8063;
  return POF_RSP_OK;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*=============================================================================
 * function name: POF_Write_SFP_DiagTest()
 *
 * parameters:    
 *
 * return value:  POF_RSP
 *
 * description:
 *===========================================================================*/
POF_RSP  POF_LOCAL_FCT_ATTR POF_Write_SFP_DiagTest( LSA_UINT16                                   const  PortIndex,
                                                    POF_UPPER_PRM_SFP_DIAG_TEST_CONST_PTR_TYPE   const  pRecordData,
                                                    POF_HDB_PTR_TYPE                             const  pHDB )
{
  /* All Data in Record in Big-Endian (N_...) */
  LSA_UINT32                     ManipulationRequestFlags;
  LSA_UINT32                     PropIndex;
  POF_EDD_PCTRL_PTR_TYPE  const  pPCtrl   = &pHDB->pPCtrl[PortIndex];

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_Write_SFP_DiagTest, PortIndex:0x%X pHDB:0x%X", PortIndex, pHDB);

  POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_Write_SFP_DiagTest, N_DiagTrigger:0x%X",
                       pRecordData->N_DiagTrigger);

  ///*** suitable Test receiver  ***/
  if (!pPCtrl->TestTranceiverDetected)
  {
      POF_SET_DETAIL_ERR(POF_ERR_TEST_DIAG);
      POF_PRM_SET_ERR_OFFSET(pHDB, POF_PRM_RECORD_SFP_DIAG_TEST_OFFSET_N_DiagTrigger);
      return POF_RSP_ERR_PRM_INDEX;
  }
  
  ///*** Is Diag parametrized ***/
  if (0 == (pPCtrl->RecordSet_A.RecordTypeWritten & POF_RECORD_TYPE_SFP_DATA_CHECK_8063))
  {
      POF_SET_DETAIL_ERR(POF_ERR_TEST_DIAG);
      POF_PRM_SET_ERR_OFFSET(pHDB, POF_PRM_RECORD_SFP_DIAG_TEST_OFFSET_N_DiagTrigger);
      return POF_RSP_ERR_PRM_INDEX;
  }
  
  ManipulationRequestFlags = POF_NTOH_32(pRecordData->N_DiagTrigger);

  ///*** only hardware supported diags requested ***/
  if ((ManipulationRequestFlags  &  pPCtrl->SFP_DiagTest.SupportedManipulationMask)  != ManipulationRequestFlags)
  {
      POF_SET_DETAIL_ERR(POF_ERR_TEST_DIAG);
      POF_PRM_SET_ERR_OFFSET(pHDB, POF_PRM_RECORD_SFP_DIAG_TEST_OFFSET_N_DiagTrigger);
      return POF_RSP_ERR_PRM_BLOCK;
  }

  /*** check for simultaneous SET of High and Low values ***/
  for (PropIndex = 0; PropIndex < POF_SFP_NUMBER_CHECK_DIAGNOSTICS; ++PropIndex)
  {
    if ((ManipulationRequestFlags & pPCtrl->SFP_DiagTest.SFP_DiagTestData[PropIndex].HighValue.ManipulationBitMask)  &&
        (ManipulationRequestFlags & pPCtrl->SFP_DiagTest.SFP_DiagTestData[PropIndex].LowValue. ManipulationBitMask)    )
    {
      POF_SET_DETAIL_ERR(POF_ERR_TEST_DIAG);
      POF_PRM_SET_ERR_OFFSET(pHDB, POF_PRM_RECORD_SFP_DIAG_TEST_OFFSET_N_DiagTrigger);
      return POF_RSP_ERR_PRM_BLOCK;
    }
  }
  /*** no error found, so do/undo the requested manipulations ***/

  pPCtrl->SFP_DiagTest.CurrentManipulationStatus = POF_Write_SFP_DiagTestData(PortIndex,
                                                                              pHDB,
                                                                              ManipulationRequestFlags);

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_Write_SFP_DiagTest, PortIndex:0x%X pHDB:0x%X",
                        PortIndex, pHDB);

  return POF_RSP_OK;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*****************************************************************************/
/*  end of file pof_prm_rec.c                                                */
/*****************************************************************************/
