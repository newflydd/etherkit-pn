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
/*  F i l e               &F: eddp_swi_irt.c                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP SWITCH functions for IRT related things.    */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  25.06.09    AB    initial version                                        */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID   85                 
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_SWI_IRT */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"            /* eddp headerfiles */
#include "eddp_int.h"            /* internal header  */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID) 

/*===========================================================================*/
/*                                global Variables                           */
/*===========================================================================*/

/*===========================================================================*/
/*                                  Types                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                Constants                                  */
/*===========================================================================*/





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_PRM_Configure_IRT                  +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function sets up and enables IRT according to the    +*/
/*+               parameters of the PDIRData record.                        +*/
/*+               If this record does not exist nothing is done.            +*/
/*+                                                                         +*/
/*+               Take data always from active RecordsetA.                  +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_PRM_Configure_IRT(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_SWI_PRM_Configure_IRT(): SWI.IRTState=0x%X", pDDB->SWI.IRTState);

    EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_PRM_Configure_IRT(): PDSyncDataA=0x%X, PDIRDataA=0x%X, PDIRSubframeDataA=0x%X", 
        pDDB->PRM.pRecordsetA->PDSyncData_IsPresent, 
        pDDB->PRM.pRecordsetA->PDIRData_IsPresent, 
        pDDB->PRM.pRecordsetA->PDIRSubframeData_IsPresent);

    /* -------------------------------------------------------- */
    /* IRTtop: PDSyncData and PDIRData records                  */
    /* -------------------------------------------------------- */
    if ((pDDB->PRM.pRecordsetA->PDSyncData_IsPresent) && (pDDB->PRM.pRecordsetA->PDIRData_IsPresent))
    {
        // set AbsoluteTime- or RelativeTime-Mode
        if (pDDB->PRM.pPDIRData_A->AbsoluteTimeMode)
            pDDB->SWI.IRTtopFwMode = EDDP_SWI_IRTTOP_FW_MODE_ABSOLUTETIME;
        else
            pDDB->SWI.IRTtopFwMode = EDDP_SWI_IRTTOP_FW_MODE_RELATIVETIME;

        /* ------------------------------------------------ */
        /* IMPORTANT - The activate order of IRT in HW:     */
        /* ------------------------------------------------ */
        //  - configure phase list (it will be activated by RTC3-PortStatemachine)
        EDDP_SWI_Setup_Phases_RTC3 (pDDB);
        //  - configure and activate FastForwarding
        EDDP_SWI_Setup_FastForwarding (pDDB);
        //  - configure and activate TxFragmentation
        //      1) enable TxFragmentation
        //      2) configure TxFrameLength_Px
        EDDP_SWI_Setup_TxFragmentation (pDDB);
        //  - configure and activate IRT-Table
        EDDP_SWI_Setup_IRT_Table (pDDB);

        // IRTtop is active in HW
        EDDP_SET_BIT_VALUE16( (pDDB->SWI.IRTState), 
                                EDDP_SWI_IRTSTATE_BIT_ON, 
                                EDDP_SWI_IRTSTATE_IRT_TOP_MASK, 
                                EDDP_SWI_IRTSTATE_IRT_TOP_SHIFT)
    }

    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_SWI_PRM_Configure_IRT(): SWI.IRTState=0x%X", pDDB->SWI.IRTState);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_PRM_Disable_IRT                    +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function sets all IRT settings to default.           +*/
/*+                                                                         +*/
/*+               IMPORTANT - The passivate order of IRT in HW:             +*/
/*+                 - disable IRT-Table                                     +*/
/*+                 - disable phase control                                 +*/
/*+                 - set TxFrameLength_Px to default                       +*/
/*+                 - set Rx/Tx-Boundary-DP2 active                         +*/
/*+                 - disable FastForwarding                                +*/
/*+                 - disable TxFragmentation                               +*/
/*+                     1) set register TxFrameLength_Px to default         +*/
/*+                     2) disable TxFragmentation in register PortControl_Px+*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_PRM_Disable_IRT(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT32          PortID;
    LSA_UINT32          HwPortID;
    LSA_UINT32          reg_val, reg_address;

    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_PRM_Disable_IRT(): Disable all IRT settings! current SWI.IRTState=0x%X", 
        pDDB->SWI.IRTState);

    /* -------------------------------------------- */
    /* disable IRT flags                            */
    /* -------------------------------------------- */
    pDDB->SWI.IRTtopFwMode = EDDP_SWI_IRTTOP_FW_MODE_INVALID;
    // IRTtop
    EDDP_SET_BIT_VALUE16( (pDDB->SWI.IRTState), 
                          EDDP_SWI_IRTSTATE_BIT_OFF, 
                          EDDP_SWI_IRTSTATE_IRT_TOP_MASK, 
                          EDDP_SWI_IRTSTATE_IRT_TOP_SHIFT)

    /* -------------------------------------------- */
    /* disable IRT-Table                            */
    /* -------------------------------------------- */
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_IRT_TABLE_BASE2_A, PNIP_REG_IRT_TABLE_BASE2_A_RST__VAL);

    /* -------------------------------------------- */
    /* port specific parameter                      */
    /* -------------------------------------------- */
    for ( PortID=1; PortID <= pDDB->SWI.PortCnt; PortID++ )
    {   
        HwPortID = EDDP_SWI_get_HwPortID(pDDB, PortID);

        /* -------------------------------------------- */
        /* disable phase control                        */
        /* -------------------------------------------- */
        reg_address = (HwPortID==1 ? PNIP_REG_PHASEENABLE_P1 : 
                      (HwPortID==2 ? PNIP_REG_PHASEENABLE_P2 : 
                      (HwPortID==3 ? PNIP_REG_PHASEENABLE_P3 : 
                      (HwPortID==4 ? PNIP_REG_PHASEENABLE_P4 : 0))));
        EDDP_HAL_REG32_WRITE(pDDB, reg_address, EDDP_SWI_PHASE_ENABLE_OFF);

        /* -------------------------------------------- */
        /* reset RTC3 and RTC2 Port State               */
        /* -------------------------------------------- */
        pDDB->SWI.LinkStatus[PortID].IRTPortStatus          = EDD_IRT_NOT_SUPPORTED;
        pDDB->SWI.LinkStatus[PortID].RTClass2_PortStatus    = EDD_RTCLASS2_NOT_SUPPORTED;

        /* -------------------------------------------- */
        /* reset YellowTime                             */
        /* in register TxFrameLength_Px                 */
        /* -------------------------------------------- */
        reg_address = (HwPortID==1 ? PNIP_REG_TXFRAMELENGTH_P1 : 
                      (HwPortID==2 ? PNIP_REG_TXFRAMELENGTH_P2 : 
                      (HwPortID==3 ? PNIP_REG_TXFRAMELENGTH_P3 : 
                      (HwPortID==4 ? PNIP_REG_TXFRAMELENGTH_P4 : 0))));
        EDDP_HAL_REG32_WRITE(pDDB, reg_address, PNIP_REG_TXFRAMELENGTH_P1_RST__VAL);

        /* -------------------------------------------- */
        /* set Rx/Tx-Boundary-DP2 active                */
        /* in register FlowControl_Px                   */
        /* -------------------------------------------- */
        reg_address = (HwPortID==1 ? PNIP_REG_FLOWCONTROL_P1 : 
                      (HwPortID==2 ? PNIP_REG_FLOWCONTROL_P2 : 
                      (HwPortID==3 ? PNIP_REG_FLOWCONTROL_P3 : 
                      (HwPortID==4 ? PNIP_REG_FLOWCONTROL_P4 : 0))));
        reg_val = EDDP_HAL_REG32_READ(pDDB, reg_address);
        EDDP_SET_BIT_VALUE32(reg_val, 0x00000001, PNIP_REG_FLOWCONTROL_P1__MSK_RXBOUNDARY_DP2, PNIP_REG_FLOWCONTROL_P1_SHFT_RXBOUNDARY_DP2);
        EDDP_SET_BIT_VALUE32(reg_val, 0x00000001, PNIP_REG_FLOWCONTROL_P1__MSK_TXBOUNDARY_DP2, PNIP_REG_FLOWCONTROL_P1_SHFT_TXBOUNDARY_DP2);
        EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_val);

        /* -------------------------------------------- */
        /* disable SuperFastForwarding                  */
        /* disable TxFragmentation                      */
        /* disable Forwarding-Mode "Store-and-Forward"  */
        /*      -> set it to Cut-Through                */
        /* in register PortControl_Px                   */
        /* -------------------------------------------- */
        reg_address = (HwPortID==1 ? PNIP_REG_PORTCONTROL_P1 : 
                      (HwPortID==2 ? PNIP_REG_PORTCONTROL_P2 : 
                      (HwPortID==3 ? PNIP_REG_PORTCONTROL_P3 : 
                      (HwPortID==4 ? PNIP_REG_PORTCONTROL_P4 : 0))));
        reg_val = EDDP_HAL_REG32_READ(pDDB, reg_address);
        EDDP_SET_BIT_VALUE32(reg_val, 0x00000000, PNIP_REG_PORTCONTROL_P1__MSK_SUPERFASTFORWARDING, PNIP_REG_PORTCONTROL_P1_SHFT_SUPERFASTFORWARDING);
        EDDP_SET_BIT_VALUE32(reg_val, 0x00000000, PNIP_REG_PORTCONTROL_P1__MSK_TXD_FRAGMENTATION, PNIP_REG_PORTCONTROL_P1_SHFT_TXD_FRAGMENTATION);
        EDDP_SET_BIT_VALUE32(reg_val, 0x00000000, PNIP_REG_PORTCONTROL_P1__MSK_FORWARDING_MODE, PNIP_REG_PORTCONTROL_P1_SHFT_FORWARDING_MODE);
        EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_val);

        // LinkStatus.TxFragmentation is OFF
        pDDB->SWI.LinkStatus[PortID].TxFragmentation = EDD_PORT_NO_TX_FRAGMENTATION;

        // reset RsvIntervalRed and RsvIntervalOrange
        EDDP_SWI_Reset_RsvInterval_Red_Orange_Param (pDDB, PortID);

        // notify user of port status changes
        EDDP_GENIndicateLinkChange_Ext (pDDB, PortID);
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_Setup_Phases_RTC3                  +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function sets up and enables phase control for RTC3  +*/
/*+                                                                         +*/
/*+               Take data always from active RecordsetA.                  +*/
/*+               So call this function only after EDDP_PRMCopySetB2A()!    +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Setup_Phases_RTC3(
	EDDP_LOCAL_DDB_PTR_TYPE         pDDB)
{
    LSA_UINT32                      i, j, HwPortID, phase_repeat;
    LSA_UINT32                      phase_entry;
    LSA_UINT32                      time_list_index_base = 0;
    LSA_UINT32                      time_list_index = 0;
    LSA_UINT32                      phase_list_tx_ram_index;
    LSA_UINT32                      phase_list_rx_ram_index;
    LSA_UINT32                      red_index, orange_index, green_index;
    LSA_UINT32                      green_phase_assignment;
    EDD_COMMON_MEM_PTR_TYPE         time_list;
    EDD_COMMON_MEM_PTR_TYPE         phase_list_address_tx;    
    EDD_COMMON_MEM_PTR_TYPE         phase_list_address_rx;      
    EDDP_PRM_PDIRDATA_PTR_TYPE      pPDIRData;
    LSA_UINT32                      maxIndex; 

    EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Setup_Phases_RTC3()");

    // is PDIRData present in RecordsetA ?
    if ( !(EDDP_PRMPDIRDataIsPDIRDataPresent(pDDB, LSA_TRUE)) )
    {
        EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_Setup_Phases_RTC3(): FATAL -> The record PDIRData(A) is not present!");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    pPDIRData   = pDDB->PRM.pPDIRData_A;
    time_list   = EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_SYNC, EDDP_HAL_RAMSUB_SYNC_TIME_LIST, 0);

    for ( i=0; i < pPDIRData->PDIRBeginEndData.NumberOfPorts; i++ )
    {
        HwPortID = EDDP_SWI_get_HwPortID(pDDB, i+1);
        
        EDDP_ASSERT(pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].NumberOfAssignments <= EDDP_PRM_MAX_ASSIGNMENTS);

        for ( j=0; j < pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].NumberOfAssignments; j++ )
        {  
            maxIndex = (pDDB->SWI.PortCnt * EDDP_SWI_SYNC_MAX_TIME_LIST_ENTRIES_PER_PORT) -1;

            EDDP_ASSERT(maxIndex >= time_list_index + 5);

            EDDP_HAL_MEM32_WRITE(pDDB, &((((PNIP_SWI_RAMSUB_SYNC_TIME_LIST_ENTRY_PTR_TYPE)time_list)[time_list_index]).TimeEntry), 
                                    pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxRedOrangePeriodBegin);
            time_list_index++;  // move index to next time entry
            EDDP_HAL_MEM32_WRITE(pDDB, &((((PNIP_SWI_RAMSUB_SYNC_TIME_LIST_ENTRY_PTR_TYPE)time_list)[time_list_index]).TimeEntry), 
                                    pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxOrangePeriodBegin);
            time_list_index++;  // move index to next time entry
            EDDP_HAL_MEM32_WRITE(pDDB, &((((PNIP_SWI_RAMSUB_SYNC_TIME_LIST_ENTRY_PTR_TYPE)time_list)[time_list_index]).TimeEntry), 
                                    pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxGreenPeriodBegin);
            time_list_index++;  // move index to next time entry
            EDDP_HAL_MEM32_WRITE(pDDB, &((((PNIP_SWI_RAMSUB_SYNC_TIME_LIST_ENTRY_PTR_TYPE)time_list)[time_list_index]).TimeEntry), 
                                    pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxRedOrangePeriodBegin);
            time_list_index++;  // move index to next time entry
            EDDP_HAL_MEM32_WRITE(pDDB, &((((PNIP_SWI_RAMSUB_SYNC_TIME_LIST_ENTRY_PTR_TYPE)time_list)[time_list_index]).TimeEntry), 
                                    pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxOrangePeriodBegin);
            time_list_index++;  // move index to next time entry
            EDDP_HAL_MEM32_WRITE(pDDB, &((((PNIP_SWI_RAMSUB_SYNC_TIME_LIST_ENTRY_PTR_TYPE)time_list)[time_list_index]).TimeEntry), 
                                    pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxGreenPeriodBegin);
            time_list_index++;  // move index to next time entry 

            // calculate "RsvIntervalRed.Tx" for SWI.LinkStatus[PortID]
            if (pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxRedOrangePeriodBegin != pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxOrangePeriodBegin)
            {
                // there is a Red-Phase
                if (   (pDDB->SWI.LinkStatus[i+1].RsvIntervalRed.Tx.BeginNs == 0) 
                    && (pDDB->SWI.LinkStatus[i+1].RsvIntervalRed.Tx.EndNs == 0)
                   )
                {
                    // if RsvIntervalRed.Tx.BeginNs and RsvIntervalRed.Tx.EndNs are still at their initial values -> just set
                    pDDB->SWI.LinkStatus[i+1].RsvIntervalRed.Tx.BeginNs = pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxRedOrangePeriodBegin;
                    pDDB->SWI.LinkStatus[i+1].RsvIntervalRed.Tx.EndNs   = pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxOrangePeriodBegin;
                }
                else
                {           
                    if (pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxRedOrangePeriodBegin < pDDB->SWI.LinkStatus[i+1].RsvIntervalRed.Tx.BeginNs)
                    {
                        pDDB->SWI.LinkStatus[i+1].RsvIntervalRed.Tx.BeginNs = pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxRedOrangePeriodBegin;
                    }
                    if (pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxOrangePeriodBegin > pDDB->SWI.LinkStatus[i+1].RsvIntervalRed.Tx.EndNs)
                    {
                        pDDB->SWI.LinkStatus[i+1].RsvIntervalRed.Tx.EndNs = pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxOrangePeriodBegin;
                    }
                }
            }

            // calculate "RsvIntervalRed.Rx" for SWI.LinkStatus[PortID]
            if (pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxRedOrangePeriodBegin != pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxOrangePeriodBegin)
            {
                // there is a Red-Phase
                if (   (pDDB->SWI.LinkStatus[i+1].RsvIntervalRed.Rx.BeginNs == 0) 
                    && (pDDB->SWI.LinkStatus[i+1].RsvIntervalRed.Rx.EndNs == 0)
                   )
                {
                    // if RsvIntervalRed.Rx.BeginNs and RsvIntervalRed.Rx.EndNs are still at their initial values -> just set
                    pDDB->SWI.LinkStatus[i+1].RsvIntervalRed.Rx.BeginNs = pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxRedOrangePeriodBegin;
                    pDDB->SWI.LinkStatus[i+1].RsvIntervalRed.Rx.EndNs   = pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxOrangePeriodBegin;
                }
                else
                {           
                    if (pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxRedOrangePeriodBegin < pDDB->SWI.LinkStatus[i+1].RsvIntervalRed.Rx.BeginNs)
                    {
                        pDDB->SWI.LinkStatus[i+1].RsvIntervalRed.Rx.BeginNs = pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxRedOrangePeriodBegin;
                    }
                    if (pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxOrangePeriodBegin > pDDB->SWI.LinkStatus[i+1].RsvIntervalRed.Rx.EndNs)
                    {
                        pDDB->SWI.LinkStatus[i+1].RsvIntervalRed.Rx.EndNs = pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxOrangePeriodBegin;
                    }
                }
            }
        }

        phase_list_tx_ram_index = (HwPortID==1 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P1 : 
                                  (HwPortID==2 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P2 : 
                                  (HwPortID==3 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P3 : 
                                  (HwPortID==4 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P4 : 0))));

        phase_list_rx_ram_index = (HwPortID==1 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P1 : 
                                  (HwPortID==2 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P2 : 
                                  (HwPortID==3 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P3 : 
                                  (HwPortID==4 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P4 : 0))));

        phase_list_address_tx = EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_SYNC, phase_list_tx_ram_index, 0);
        phase_list_address_rx = EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_SYNC, phase_list_rx_ram_index, 0);

        phase_repeat = 0;

        do
        {
            for ( j=0; j < pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].NumberOfPhases; j++ )
            {


                red_index       = time_list_index_base + ((((pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].PhaseAssignmentArray[j].TxPhaseAssignment >> 0) & 0xF) * 6) + 0);
                orange_index    = time_list_index_base + ((((pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].PhaseAssignmentArray[j].TxPhaseAssignment >> 4) & 0xF) * 6) + 1);
                green_index     = time_list_index_base + ((((pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].PhaseAssignmentArray[j].TxPhaseAssignment >> 8) & 0xF) * 6) + 2);

                // write phase list entry (Tx)        
                phase_entry =     EDDP_SWI_SET_PHASE_ENTRY_RED(LSA_TRUE, red_index) 
                                | EDDP_SWI_SET_PHASE_ENTRY_ORANGE(LSA_FALSE, orange_index) 
                                | EDDP_SWI_SET_PHASE_ENTRY_GREEN(LSA_TRUE, green_index);

                // check if this is a green-only phase
                green_phase_assignment = ((pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].PhaseAssignmentArray[j].TxPhaseAssignment >> 8) & 0xF);

                if (pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[green_phase_assignment].TxGreenPeriodBegin == 0x00000000)
                {
                    EDDP_SWI_SET_PHASE_ENTRY_TO_ALL_GREEN(phase_entry);
                }

                EDDP_HAL_MEM32_WRITE(pDDB, phase_list_address_tx, phase_entry);

                EDDP_SWI_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_SWI_Setup_Phases_RTC3(): Wrote TX phase_entry: 0x%X to phase_list_address_tx:0x%X for Phase:0x%X on portnumber:0x%X", 
                                  phase_entry, phase_list_address_tx, j, i);

                phase_list_address_tx = (LSA_UINT32*)phase_list_address_tx + 1;     // move pointer to next tx phase list entry

                red_index       = time_list_index_base + ((((pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].PhaseAssignmentArray[j].RxPhaseAssignment >> 0) & 0xF) * 6) + 3);
                orange_index    = time_list_index_base + ((((pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].PhaseAssignmentArray[j].RxPhaseAssignment >> 4) & 0xF) * 6) + 4);
                green_index     = time_list_index_base + ((((pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].PhaseAssignmentArray[j].RxPhaseAssignment >> 8) & 0xF) * 6) + 5);

                // write phase list entry (Rx)            
                phase_entry =     EDDP_SWI_SET_PHASE_ENTRY_RED(LSA_TRUE, red_index) 
                                | EDDP_SWI_SET_PHASE_ENTRY_ORANGE(LSA_FALSE, orange_index) 
                                | EDDP_SWI_SET_PHASE_ENTRY_GREEN(LSA_TRUE, green_index);

                // check if this is a green-only phase
                green_phase_assignment = ((pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].PhaseAssignmentArray[j].RxPhaseAssignment >> 8) & 0xF);

                if (pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[green_phase_assignment].RxGreenPeriodBegin == 0x00000000)
                {
                    EDDP_SWI_SET_PHASE_ENTRY_TO_ALL_GREEN(phase_entry);
                }

                EDDP_HAL_MEM32_WRITE(pDDB, phase_list_address_rx, phase_entry);

                EDDP_SWI_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_SWI_Setup_Phases_RTC3(): Wrote RX phase_entry:0x%X to phase_list_address_rx:0x%X for Phase:0x%X on portnumber:0x%X", 
                                  phase_entry, phase_list_address_rx, j, i);

                phase_list_address_rx = (LSA_UINT32*)phase_list_address_rx + 1;     // move pointer to next rx phase list entry
            }
            phase_repeat++;
        } while ( (phase_repeat * pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].NumberOfPhases) != pPDIRData->PDIRBeginEndData.NumberOfPhases_max );

        // set index base to next assignment
        time_list_index_base = time_list_index;

        if (   (pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].NumberOfAssignments == 1) 
            && (pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[0].TxRedOrangePeriodBegin == 0x0) 
            && (pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[0].TxOrangePeriodBegin == 0x0) 
            && (pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[0].TxGreenPeriodBegin == 0x0) 
            && (pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[0].RxRedOrangePeriodBegin == 0x0) 
            && (pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[0].RxOrangePeriodBegin == 0x0) 
            && (pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[0].RxGreenPeriodBegin == 0x0)
           )
        {
            // if this is a "green-only" port the IRT port status remains at EDD_IRT_NOT_SUPPORTED
            pDDB->SWI.LinkStatus[i+1].IRTPortStatus = EDD_IRT_NOT_SUPPORTED;
        }
        else
        {
            pDDB->SWI.LinkStatus[i+1].IRTPortStatus = EDD_IRT_PORT_INACTIVE;
        }

        // copy parameters from PDIRData_A
        pDDB->SWI.LinkStatus[i+1].RsvIntervalRed.Rx.EndLocalNs          = pPDIRData->LastLocalIRTFrameArray[i].RxEndLocalNs;
        pDDB->SWI.LinkStatus[i+1].RsvIntervalRed.Rx.LastLocalFrameLen   = pPDIRData->LastLocalIRTFrameArray[i].RxLastLocalFrameLen;
        pDDB->SWI.LinkStatus[i+1].RsvIntervalRed.Tx.EndLocalNs          = pPDIRData->LastLocalIRTFrameArray[i].TxEndLocalNs;
        pDDB->SWI.LinkStatus[i+1].RsvIntervalRed.Tx.LastLocalFrameLen   = pPDIRData->LastLocalIRTFrameArray[i].TxLastLocalFrameLen;
    }

    // set PhaseRange register to maximum number of phases
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_PHASERANGE_A, (pPDIRData->PDIRBeginEndData.NumberOfPhases_max - 1));
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_Setup_IRT_Table                    +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function creates and activates the IRT-Table         +*/
/*+               according to the given PDIRData record for                +*/
/*+               AbsoluteTime- or RelativeTime-Mode.                       +*/
/*+                                                                         +*/
/*+               Take data always from active RecordsetA.                  +*/
/*+               So call this function only after EDDP_PRMCopySetB2A()!    +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Setup_IRT_Table(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT32                              reg_val;
    LSA_UINT32                              i, iPDIRFrData;
    LSA_UINT32                              irt_entry_32;       // for AbsoluteTime-Mode    : sizeof(irt_entry) = 4 byte
    LSA_UINT8                               irt_entry_8;        // for RelativeTime-Mode    : sizeof(irt_entry) = 1 byte
    LSA_UINT16                              FrameID_low     = 0;
    LSA_UINT16                              FrameID_high    = 0;
    LSA_UINT16                              FrameID_next    = 0;
    LSA_UINT8                               TxPortIF        = 0;
    LSA_UINT32                              PortID          = 0;
    LSA_UINT32                              Max_IRT_Entries = 0;
    LSA_UINT8                               TxPortGroupArrayUser, TxPortGroupArrayHw;
    LSA_BOOL                                RTM_IRT_Entry_In_PDIRFrameData_Found = LSA_FALSE;   // for RelativeTime-Mode (RTM)
    LSA_UINT32                              IRT_Table_Size;
    EDD_COMMON_MEM_PTR_TYPE                 irt_table_base_address_host_32;     // for AbsoluteTime-Mode
    EDD_COMMON_MEM_U8_PTR_TYPE              irt_table_base_address_host_8 = 0;  // for RelativeTime-Mode
    LSA_UINT32                              irt_table_base_address_pnip;
    EDDP_PRM_PDIRDATA_PTR_TYPE              pPDIRData;
    EDDP_PRM_PDIRSUBFRAMEDATA_PTR_TYPE      pPDIRSubframeData;

    // is PDIRData present in RecordsetA ?
    if ( !(EDDP_PRMPDIRDataIsPDIRDataPresent(pDDB, LSA_TRUE)) )
    {
        EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_Setup_IRT_Table(): FATAL -> The record PDIRData(A) is not present!");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    pPDIRData           = pDDB->PRM.pPDIRData_A;
    pPDIRSubframeData   = pDDB->PRM.pPDIRSubframeData_A;

    // get base address of IRT-Table
    irt_table_base_address_host_32 = EDDP_HALGetRAMSubAddress( pDDB, 
                                                               EDDP_HAL_RAMTYPE_FDB_TABLE,
                                                               EDDP_HAL_RAMSUB_FDB_TABLE_IRT_TABLE_A, 
                                                               0);
    irt_table_base_address_pnip = EDDP_HAL_H2P_ADDR(pDDB, irt_table_base_address_host_32);

    switch (pDDB->SWI.IRTtopFwMode)
    {
        /* -------------------------------------------------------------------------------- */
        /* AbsoluteTime-Mode                                                                */
        /* -------------------------------------------------------------------------------- */
        /*  - FrameID range only from PDIRFrameData                                         */
        /*  - it contains all FrameIDs from PDIRFrameData-Array, also those Frames          */
        /*    which only are forwarded                                                      */
        /*  - sizeof(irt_entry) = 4 byte                                                    */
        /* -------------------------------------------------------------------------------- */
        case EDDP_SWI_IRTTOP_FW_MODE_ABSOLUTETIME:
        {
            FrameID_low     = pPDIRData->FrameID_low;
            FrameID_high    = pPDIRData->FrameID_high;
            Max_IRT_Entries = EDDP_SWI_FDB_SIZE_OF_IRT_TABLE_A_32BIT;

            EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Setup_IRT_Table(): Init IRT-Table -> AbsoluteTimeMode, FrameID range [0x%X...0x%X]", 
                FrameID_low, FrameID_high);

            // set AbsoluteTime-Mode in IRT_Control register
            reg_val = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_IRT_CONTROL);
            EDDP_HAL_REG32_WRITE(pDDB,  PNIP_REG_IRT_CONTROL, (reg_val | (PNIP_REG_IRT_CONTROL__MSK_TOPFW_MODE_A)));

            // init IRT_Table_Size
            IRT_Table_Size = FrameID_high - FrameID_low + 1;
            if (IRT_Table_Size > Max_IRT_Entries)
            {
                EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_Setup_IRT_Table(): FATAL -> IRT_Table_Size(%d) > Max_IRT_Entries(%d)", 
                    IRT_Table_Size, Max_IRT_Entries);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            else
            {
                // delete IRT-Table before writing new entries
                for ( i=0; i < IRT_Table_Size; i++ )
                {
                    EDDP_HAL_MEM32_WRITE(pDDB, ((LSA_UINT32*)irt_table_base_address_host_32) + i, 0x00000000);        
                }
            }

            /* -------------------------------------------------------- */
            /* fill IRT-Table with all FrameIDs from PDIRFrameData      */
            /* -------------------------------------------------------- */
            for ( iPDIRFrData=0; iPDIRFrData < pPDIRData->IRFrameDataArray_Count_All; iPDIRFrData++ )
            {  
                // only valid FrameID range
                if (   (pPDIRData->pIRFrameDataArray[iPDIRFrData].FrameID >= FrameID_low) 
                    && (pPDIRData->pIRFrameDataArray[iPDIRFrData].FrameID <= FrameID_high)
                   )
                {
                    /* First read current IRT entry in case there has been already written an IRT entry with same FrameID.          */
                    /* TxPorts are then combined (OR). This scenario can only happen when there are more than one providers         */
                    /* with the same FrameID but different FrameSendOffset. Because one IRT entry can only hold one FrameSendOffset */
                    /* it is not used. The FrameSendOffset for providers are actually determined by the PPM itself!                 */
                    irt_entry_32 = EDDP_HAL_MEM32_READ( pDDB, 
                                                        ((LSA_UINT32*)irt_table_base_address_host_32) 
                                                        + (pPDIRData->pIRFrameDataArray[iPDIRFrData].FrameID - FrameID_low));

                    // Provider (with ACW in PPM)   : RxPort = 0
                    // Forwarder                    : RxPort != 0
                    if (   (EDDP_PRMPDIRSubframeDataIsDfpPresent(pDDB, LSA_TRUE)) 
                        && (pPDIRData->pIRFrameDataArray[iPDIRFrData].isDoubleFrameID)
                       )
                    {
                        // this is DFP and there is double FrameID (Provider and Consumer) in PDIRFrameData
                        LSA_UINT32                                          FrameSendOffsetTemp = 0;
                        EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_PTR_TYPE    pSubframeBlock;
                        EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE                     pPDIRFrameDataTemp;

                        // pointer to SubframeBlock of this PDIRFrameData
                        EDDP_ASSERT_FALSE(pPDIRData->pIRFrameDataArray[iPDIRFrData].SubframeBlockIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
                        pSubframeBlock = &pPDIRSubframeData->SubframeBlockArray[pPDIRData->pIRFrameDataArray[iPDIRFrData].SubframeBlockIdx];

                        if (pSubframeBlock->SFIOCRProperties.DFPDirection == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPDIRECTION_INBOUND)
                        {
                            // InBound: IRT entry as Consumer
                            EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataConsIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
                            pPDIRFrameDataTemp      = &pPDIRData->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataConsIdx];
                            TxPortGroupArrayUser    = pPDIRFrameDataTemp->TxPortGroupArray;
                        }
                        else
                        {
                            // OutBound: IRT entry = TxPort(Consumer+Provider) and FrameSendOffset(Provider)
                            // Consumer
                            EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataConsIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
                            pPDIRFrameDataTemp      = &pPDIRData->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataConsIdx];
                            TxPortGroupArrayUser    = pPDIRFrameDataTemp->TxPortGroupArray;
                            // Provider
                            EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataProvIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
                            pPDIRFrameDataTemp      = &pPDIRData->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataProvIdx];
                            TxPortGroupArrayUser   |= pPDIRFrameDataTemp->TxPortGroupArray;
                            FrameSendOffsetTemp     = pPDIRFrameDataTemp->FrameSendOffset;
                        }

                        // convert UserTxPorts to HwTxPorts
                        TxPortGroupArrayHw = EDDP_PRM_Map_UserTxPortGroupArray_To_HwTxPortGroupArray (pDDB, TxPortGroupArrayUser);
                        // set field    : TxPort, ExactTime=1, Valid=1, TxTime
                        irt_entry_32 |= EDDP_SWI_SET_IRT_ENTRY_ABSOLUTE( TxPortGroupArrayHw, 
                                                                         EDDP_SWI_IRT_ENTRY_EXACT_TIME, 
                                                                         FrameSendOffsetTemp);
                    }
                    else
                    {
                        // no DFP or no double FrameID
                        if (pPDIRData->pIRFrameDataArray[iPDIRFrData].SndRcvRole == EDDP_PRM_PDIRFRAMEDATA_ROLE_PROVIDER)
                        {
                            // set field    : TxPort=0, ExactTime=1, Valid=1, TxTime=0
                            irt_entry_32 = EDDP_SWI_SET_IRT_ENTRY_ABSOLUTE(0, EDDP_SWI_IRT_ENTRY_EXACT_TIME, 0);
                        }
                        else
                        {
                            // convert UserTxPorts to HwTxPorts
                            TxPortGroupArrayHw = EDDP_PRM_Map_UserTxPortGroupArray_To_HwTxPortGroupArray (pDDB, pPDIRData->pIRFrameDataArray[iPDIRFrData].TxPortGroupArray);
                            // set field    : TxPort, ExactTime=1, Valid=1, TxTime
                            irt_entry_32 |= EDDP_SWI_SET_IRT_ENTRY_ABSOLUTE( TxPortGroupArrayHw, 
                                                                             EDDP_SWI_IRT_ENTRY_EXACT_TIME, 
                                                                             pPDIRData->pIRFrameDataArray[iPDIRFrData].FrameSendOffset);
                        }
                    }

                    // write IRT entry to IRT-Table
                    EDDP_HAL_MEM32_WRITE( pDDB, 
                                          ((LSA_UINT32*)irt_table_base_address_host_32) 
                                          + (pPDIRData->pIRFrameDataArray[iPDIRFrData].FrameID - FrameID_low), 
                                          irt_entry_32);
                }
            }
        }
        break;

        /* -------------------------------------------------------------------------------- */
        /* RelativeTime-Mode                                                                */
        /* -------------------------------------------------------------------------------- */
        /*  - the whole FrameID range from PDIRBeginEndData (RedGuard)                      */
        /*  - FrameID with an entry in PDIRFrameData        : TxPort = from PDIRFrameData   */
        /*  - FrameID without an entry in PDIRFrameData     : TxPort = all Ports of IF      */
        /*  - sizeof(irt_entry) = 1 byte                                                    */
        /* -------------------------------------------------------------------------------- */
        case EDDP_SWI_IRTTOP_FW_MODE_RELATIVETIME:
        {
            irt_table_base_address_host_8   = (EDD_COMMON_MEM_U8_PTR_TYPE) irt_table_base_address_host_32;
            FrameID_low                     = pPDIRData->PDIRBeginEndData.StartOfRedFrameID;
            FrameID_high                    = pPDIRData->PDIRBeginEndData.EndOfRedFrameID;
            Max_IRT_Entries                 = EDDP_SWI_FDB_SIZE_OF_IRT_TABLE_A_32BIT * 4;

            EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Setup_IRT_Table(): Init IRT-Table -> RelativeTimeMode, FrameID range [0x%X...0x%X]", 
                FrameID_low, FrameID_high);

            // set RelativeTime-Mode in IRT_Control register
            reg_val = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_IRT_CONTROL);
            EDDP_HAL_REG32_WRITE(pDDB,  PNIP_REG_IRT_CONTROL, (reg_val & (~PNIP_REG_IRT_CONTROL__MSK_TOPFW_MODE_A)));

            // init IRT_Table_Size
            IRT_Table_Size = FrameID_high - FrameID_low + 1;
            if (IRT_Table_Size > Max_IRT_Entries)
            {
                EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_Setup_IRT_Table(): FATAL -> IRT_Table_Size(%d) > Max_IRT_Entries(%d)", 
                    IRT_Table_Size, Max_IRT_Entries);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            else
            {
                // delete IRT-Table before writing new entries
                for ( i=0; i < IRT_Table_Size; i++ )
                {
                    EDDP_HAL_MEM8_WRITE(pDDB, (irt_table_base_address_host_8 + i), 0x00);
                }
            }

            // store new MaxBridgeDelay (from PRM record) to global DDB parameters
            pDDB->SWI.PRM_MaxBridgeDelay = pPDIRData->PDIRGlobalData.MaxBridgeDelay;

            // port specific parameter
            for ( i=1; i <= pPDIRData->PDIRGlobalData.NumberOfPorts; i++ )
            {
                // store new DelayValues (MaxPortTxDelay, MaxPortRxDelay, MaxLineRxDelay) to global DDB parameters
                pDDB->SWI.LinkStatus[i].PRM_MaxLineRxDelay = pPDIRData->PDIRGlobalData.DelayValues[i-1].MaxLineRxDelay;

                // set register "IRT_DelayTime_Px" for this port
                EDDP_SWI_Set_IRT_DelayTime (pDDB, i);
            }

            /* -------------------------------------------------------- */
            /* fill IRT-Table with all FrameIDs from PDIRBeginEndData   */
            /* -------------------------------------------------------- */
            FrameID_next = FrameID_low;
            while ((FrameID_next <= FrameID_high) && (FrameID_next <= EDD_SRT_FRAMEID_IRT_STOP))
            {
                RTM_IRT_Entry_In_PDIRFrameData_Found = LSA_FALSE;
                // search, whether this FrameID is contained in PDIRFrameData
                for ( iPDIRFrData=0; iPDIRFrData < pPDIRData->IRFrameDataArray_Count_Used; iPDIRFrData++ )
                {
                    if (FrameID_next == pPDIRData->pIRFrameDataArray[iPDIRFrData].FrameID)
                    {
                        // one entry of RelativeTime-Mode is found in PDIRFrameData
                        RTM_IRT_Entry_In_PDIRFrameData_Found = LSA_TRUE;
                        break;
                    }
                }

                // First read current IRT entry in case there has been already written an IRT entry with same FrameID
                irt_entry_8 = EDDP_HAL_MEM8_READ(pDDB, irt_table_base_address_host_8 + (FrameID_next - FrameID_low));

                // set field    : TxPort, ExactTime, Valid
                if (RTM_IRT_Entry_In_PDIRFrameData_Found)
                {
                    // entry in PDIRFrameData found
                    // Provider (with ACW in PPM)   : RxPort = 0
                    // Forwarder                    : RxPort != 0
                    if (   (EDDP_PRMPDIRSubframeDataIsDfpPresent(pDDB, LSA_TRUE)) 
                        && (pPDIRData->pIRFrameDataArray[iPDIRFrData].isDoubleFrameID)
                       )
                    {
                        // this is DFP and there is double FrameID (Provider and Consumer) in PDIRFrameData
                        EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_PTR_TYPE    pSubframeBlock;
                        EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE                     pPDIRFrameDataTemp;

                        // pointer to SubframeBlock of this PDIRFrameData
                        EDDP_ASSERT_FALSE(pPDIRData->pIRFrameDataArray[iPDIRFrData].SubframeBlockIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
                        pSubframeBlock = &pPDIRSubframeData->SubframeBlockArray[pPDIRData->pIRFrameDataArray[iPDIRFrData].SubframeBlockIdx];

                        if (pSubframeBlock->SFIOCRProperties.DFPDirection == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPDIRECTION_INBOUND)
                        {
                            /* -------------------------------------------- */
                            /* InBound: IRT entry as Consumer               */
                            /* -------------------------------------------- */
                            // is this the redundant FrameID(x+1)
                            if ( (EDDP_IS_IRT_REDUNDANT_FRAMEID(pSubframeBlock->FrameID)) && ((pSubframeBlock->FrameID+1) == FrameID_next) )
                            {
                                // only for redundant FrameID(x+1)
                                EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataConsIdxRed1 == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
                                pPDIRFrameDataTemp      = &pPDIRData->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataConsIdxRed1];
                                TxPortGroupArrayUser    = pPDIRFrameDataTemp->TxPortGroupArray;
                            }
                            else
                            {
                                // all other FrameIDs
                                EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataConsIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
                                pPDIRFrameDataTemp      = &pPDIRData->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataConsIdx];
                                TxPortGroupArrayUser    = pPDIRFrameDataTemp->TxPortGroupArray;
                            }
                        }
                        else
                        {
                            /* -------------------------------------------- */
                            /* OutBound: IRT entry as Consumer+Provider     */
                            /* -------------------------------------------- */
                            // is this the redundant FrameID(x+1)
                            if ( (EDDP_IS_IRT_REDUNDANT_FRAMEID(pSubframeBlock->FrameID)) && ((pSubframeBlock->FrameID+1) == FrameID_next) )
                            {
                                // only for redundant FrameID(x+1)
                                // Consumer
                                EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataConsIdxRed1 == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
                                pPDIRFrameDataTemp      = &pPDIRData->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataConsIdxRed1];
                                TxPortGroupArrayUser    = pPDIRFrameDataTemp->TxPortGroupArray;
                                // Provider
                                EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataProvIdxRed1 == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
                                pPDIRFrameDataTemp      = &pPDIRData->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataProvIdxRed1];
                                TxPortGroupArrayUser   |= pPDIRFrameDataTemp->TxPortGroupArray;
                            }
                            else
                            {
                                // all other FrameIDs
                                // Consumer
                                EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataConsIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
                                pPDIRFrameDataTemp      = &pPDIRData->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataConsIdx];
                                TxPortGroupArrayUser    = pPDIRFrameDataTemp->TxPortGroupArray;
                                // Provider
                                EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataProvIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
                                pPDIRFrameDataTemp      = &pPDIRData->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataProvIdx];
                                TxPortGroupArrayUser   |= pPDIRFrameDataTemp->TxPortGroupArray;
                            }
                        }

                        // convert UserTxPorts to HwTxPorts
                        TxPortGroupArrayHw = EDDP_PRM_Map_UserTxPortGroupArray_To_HwTxPortGroupArray (pDDB, TxPortGroupArrayUser);
                        // TxPort   : from PDIRFrameData
                        irt_entry_8 |= EDDP_SWI_SET_IRT_ENTRY_RELATIVE(TxPortGroupArrayHw, EDDP_SWI_IRT_ENTRY_MINIMUM_TIME);
                    }
                    else
                    {
                        // no DFP or no double FrameID
                        if (pPDIRData->pIRFrameDataArray[iPDIRFrData].SndRcvRole == EDDP_PRM_PDIRFRAMEDATA_ROLE_PROVIDER)
                        {
                            irt_entry_8 = EDDP_SWI_SET_IRT_ENTRY_RELATIVE(0, EDDP_SWI_IRT_ENTRY_EXACT_TIME);
                        }
                        else
                        {
                            // convert UserTxPorts to HwTxPorts
                            TxPortGroupArrayHw = EDDP_PRM_Map_UserTxPortGroupArray_To_HwTxPortGroupArray (pDDB, pPDIRData->pIRFrameDataArray[iPDIRFrData].TxPortGroupArray);
                            // TxPort   : from PDIRFrameData
                            irt_entry_8 |= EDDP_SWI_SET_IRT_ENTRY_RELATIVE( TxPortGroupArrayHw, 
                                                                            EDDP_SWI_IRT_ENTRY_MINIMUM_TIME);
                        }
                    }
                }
                else
                {
                    // no entry in PDIRFrameData found
                    // TxPort   : all TxPorts of this interface as forwarder
                    TxPortIF = 0;

                    for ( PortID=1; PortID <= pDDB->SWI.PortCnt; PortID++ )
                    {
                        switch (EDDP_SWI_get_HwPortID(pDDB, PortID))
                        {
                            case 1:     TxPortIF |= EDDP_SWI_IRT_ENTRY_TXPORT_1;    break;
                            case 2:     TxPortIF |= EDDP_SWI_IRT_ENTRY_TXPORT_2;    break;
                            case 3:     TxPortIF |= EDDP_SWI_IRT_ENTRY_TXPORT_3;    break;
                            case 4:     TxPortIF |= EDDP_SWI_IRT_ENTRY_TXPORT_4;    break;
                            default:
                            {
                                EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_Setup_IRT_Table(): FATAL -> Invalid Hw-PortID(%d)!", 
                                    EDDP_SWI_get_HwPortID(pDDB, PortID));
                                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                            }
                            break;
                        }
                    }

                    irt_entry_8 |= EDDP_SWI_SET_IRT_ENTRY_RELATIVE(TxPortIF, EDDP_SWI_IRT_ENTRY_MINIMUM_TIME);
                }

                // write IRT entry to IRT-Table
                EDDP_HAL_MEM8_WRITE( pDDB, irt_table_base_address_host_8 + (FrameID_next - FrameID_low), irt_entry_8);

                // next FrameID of PDIRBeginEndData
                FrameID_next++;
            }
        }
        break;

        /* -------------------------------------------------------------------------------- */
        /* unknown mode                                                                     */
        /* -------------------------------------------------------------------------------- */
        default:
        {
            EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_Setup_IRT_Table(): FATAL -> IRTtopFwMode(0x%X) is unknown!", 
                pDDB->SWI.IRTtopFwMode);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }

    // set IRT_FrameID_Range2_A register
    reg_val = (FrameID_low | (FrameID_high << PNIP_REG_IRT_FRAMEID_RANGE2_A_SHFT_FRAMEID_HIGH));
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_IRT_FRAMEID_RANGE2_A, reg_val);

    // activate IRT-Table: set base address of IRT-Table
    EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Setup_IRT_Table(): Activate IRT-Table");
    EDDP_HAL_REG32_WRITE( pDDB, PNIP_REG_IRT_TABLE_BASE2_A, irt_table_base_address_pnip);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_Setup_FastForwarding               +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function activates the FastForwarding                +*/
/*+               in register PortControl_Px if:                            +*/
/*+                 - it is active in PDIRData.FrameDataProperties          +*/
/*+                 - the port has RED phase                                +*/
/*+                                                                         +*/
/*+               Take data always from active RecordsetA.                  +*/
/*+               So call this function only after EDDP_PRMCopySetB2A()!    +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Setup_FastForwarding(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT32                      PortID, HwPortID;
    LSA_UINT32                      FastForwardingMulticastMACAdd;
    LSA_UINT32                      reg_val, reg_address;
    EDDP_PRM_PDIRDATA_PTR_TYPE      pPDIRData;

    // is PDIRData present in RecordsetA ?
    if ( !(EDDP_PRMPDIRDataIsPDIRDataPresent(pDDB, LSA_TRUE)) )
    {
        EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_Setup_FastForwarding(): FATAL -> The record PDIRData(A) is not present!");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    pPDIRData = pDDB->PRM.pPDIRData_A;

    // is "FrameDataProperties" present in PDIRData ?
    if ( EDDP_PRMPDIRDataIsFrameDataPropertiesPresent(pDDB, LSA_TRUE) )
    {
        // get parameter PDIRData.FrameDataProperties.FastForwardingMulticastMACAdd
        FastForwardingMulticastMACAdd = 
            EDDP_PRMPDIRDataGetFrameDataProperties (pDDB, EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_SHIFT, LSA_TRUE);

        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Setup_FastForwarding(): PDIRData.FrameDataProperties.FastForwardingMulticastMACAdd(0x%X)", 
            FastForwardingMulticastMACAdd);

        // check if FastForwarding is active in PDIRData
        if (FastForwardingMulticastMACAdd == EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_FFW)
        {
            // FastForwarding is active in PDIRData, so activate it in HW for all ports with RED phase
            for ( PortID=1; PortID <= pPDIRData->PDIRGlobalData.NumberOfPorts; PortID++ )
            {
                if (pPDIRData->PDIRBeginEndData.BeginEndDataArray[PortID-1].isRedPort)
                {
                    // this port has RED phase: activate "SuperFastForwarding" in register PortControl_Px
                    HwPortID = EDDP_SWI_get_HwPortID (pDDB, PortID);

                    reg_address = (HwPortID==1 ? PNIP_REG_PORTCONTROL_P1 : 
                                  (HwPortID==2 ? PNIP_REG_PORTCONTROL_P2 : 
                                  (HwPortID==3 ? PNIP_REG_PORTCONTROL_P3 : 
                                  (HwPortID==4 ? PNIP_REG_PORTCONTROL_P4 : 0))));
                    reg_val = EDDP_HAL_REG32_READ(pDDB, reg_address);
                    #ifdef EDDP_SWI_SUPPORT_FFW_ON
                    EDDP_SET_BIT_VALUE32(reg_val, 0x00000001, PNIP_REG_PORTCONTROL_P1__MSK_SUPERFASTFORWARDING, PNIP_REG_PORTCONTROL_P1_SHFT_SUPERFASTFORWARDING);
                    EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_val);

                    EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Setup_FastForwarding(UsrPortID=%d): enable FastForwarding in REG_PORTCONTROL(HwPortID=%d)", 
                        PortID, HwPortID);
                    #else
                    EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H:--] EDDP_SWI_Setup_FastForwarding(): FastForwarding is not supported!");
                    #endif
                }
            }
        }
    }
    else
    {
        EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Setup_FastForwarding(): PDIRData.FrameDataProperties is not present");
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_Setup_TxFragmentation              +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function activates the TxFragmentation               +*/
/*+               in register PortControl_Px if:                            +*/
/*+                 - the port has RED phase                                +*/
/*+                 - FrameDataProperties.FragmentationMode is ON           +*/
/*+               else the function deactivates the TxFragmentation.        +*/
/*+                                                                         +*/
/*+               Calculate the TxFrameLength from YellowTime if:           +*/
/*+                 - YellowTime is present in PDIRGlobalData               +*/
/*+                 - FragmentationMode = ON                                +*/
/*+                 - the port has RED phase                                +*/
/*+                 - YellowTime < YellowTimeMax                            +*/
/*+                                                                         +*/
/*+               IMPORTANT - The activate order of TxFragmentation:        +*/
/*+                 1) enable TxFragmentation in register PortControl_Px    +*/
/*+                 2) set YellowTime to register TxFrameLength_Px          +*/
/*+               IMPORTANT - The passivate order of TxFragmentation:       +*/
/*+                 1) set YellowTime to register TxFrameLength_Px          +*/
/*+                 2) disable TxFragmentation in register PortControl_Px   +*/
/*+                                                                         +*/
/*+               Take PrmData always from active RecordsetA.               +*/
/*+               So call this function only after EDDP_PRMCopySetB2A()!    +*/
/*+               The valid YellowTime must be present in PDIRGlobalData!   +*/
/*+                                                                         +*/
/*+               This function triggers the activating of Domain-Boundary-Port+*/
/*+               for Fragmentation "Store-and-Forward"                     +*/
/*+               in register PortControl_Px.Forwarding_Mode if:            +*/
/*+                 - PDIRData is present                                   +*/
/*+                 - port has only green phase                             +*/
/*+                 - FrameDataProperties.FragmentationMode is ON           +*/
/*+               else set "Cut-Through" in register                        +*/
/*+               PortControl_Px.Forwarding_Mode                            +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Setup_TxFragmentation(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT32                      PortID, HwPortID;
    LSA_UINT32                      reg_val, reg_address;
    LSA_BOOL                        EnableTxFragmentation, isRedPhasePresent, InActiveRecord_A;
    LSA_UINT32                      TxFrameLength, YellowTime, FragmentationMode;
    EDDP_PRM_PDIRDATA_PTR_TYPE      pPDIRData;

    InActiveRecord_A = LSA_TRUE;

    // is PDIRData present in RecordsetA ?
    if ( !(EDDP_PRMPDIRDataIsPDIRDataPresent(pDDB, InActiveRecord_A)) )
    {
        EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_Setup_TxFragmentation(): FATAL -> The record PDIRDataA is not present!");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    pPDIRData           = pDDB->PRM.pPDIRData_A;
    FragmentationMode   = EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_OFF;

    // is "FrameDataProperties" present in PDIRData ?
    if ( EDDP_PRMPDIRDataIsFrameDataPropertiesPresent(pDDB, InActiveRecord_A) )
    {
        // get parameter PDIRData.FrameDataProperties.FragmentationMode
        FragmentationMode = EDDP_PRMPDIRDataGetFrameDataProperties (pDDB, EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_SHIFT, InActiveRecord_A);
    }

    for ( PortID=1; PortID <= pPDIRData->PDIRGlobalData.NumberOfPorts; PortID++ )
    {
        HwPortID                = EDDP_SWI_get_HwPortID(pDDB, PortID);
        YellowTime              = 0;
        EnableTxFragmentation   = LSA_FALSE;
        isRedPhasePresent       = pPDIRData->PDIRBeginEndData.BeginEndDataArray[PortID-1].isRedPort;
        TxFrameLength           = PNIP_REG_TXFRAMELENGTH_P1_RST__VAL;

        /* ---------------------------------------------------------------------------- */
        /* Set EnableTxFragmentation if:                                                */
        /*  - it is a RED port                                                          */
        /*  - FragmentationMode is ON                                                   */
        /* ---------------------------------------------------------------------------- */
        if (   (isRedPhasePresent)
            && (   (FragmentationMode == EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_ON_FRAG_SIZE_128) 
                || (FragmentationMode == EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_ON_FRAG_SIZE_256) 
               )
           )
        {
            EnableTxFragmentation = LSA_TRUE;
        }

        /* ---------------------------------------------------------------------------- */
        /* Calculate the TxFrameLength from YellowTime if:                              */
        /*  - YellowTime is present in PDIRGlobalData                                   */
        /*  - FragmentationMode = ON                                                    */
        /*  - the port has RED phase                                                    */
        /*  - YellowTime < YellowTimeMax                                                */
        /* ---------------------------------------------------------------------------- */
        if (pPDIRData->PDIRGlobalData.DelayValues[PortID-1].YellowTime_IsPresent)
        {
            // get YellowTime from PDIRGlobalData
            YellowTime = pPDIRData->PDIRGlobalData.DelayValues[PortID-1].YellowTime;
            EDDP_ASSERT_FALSE(!(EDDP_PRMPDIRDataChekYellowTimeInValidRange(pDDB, YellowTime)));

            if ((EnableTxFragmentation) && (isRedPhasePresent) && (YellowTime < EDDP_PRM_PDIRGLOBALDATA_YELLOWTIME_MAX))
            {
                #ifdef EDDP_MESSAGE
                #ifdef EDDP_PRAGMA_MESSAGE
                #pragma EDDP_PRAGMA_MESSAGE("YellowSafetyMargin is depending PNIP revision")
                #endif /* EDDP_PRAGMA_MESSAGE */
                #endif /* EDDP_MESSAGE */
                // switch to current PNIP revision
                switch (EDDP_HALGetPnipRevision(pDDB))
                {
                    #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
                    case EDDP_HAL_HW_PNIP_REV1:
                    {
                        TxFrameLength = ((YellowTime / EDDP_TIME_BYTE_RESOLUTION_RT_FRAME_NS) - EDDP_SWI_REG_TXFRAMELENGTH_YELLOW_SAFETY_MARGIN_PNIP_REV1);
                    }
                    break;
                    #endif

                    #if defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
                    case EDDP_HAL_HW_PNIP_REV2:
                    case EDDP_HAL_HW_PNIP_REV3:
                    {
                        TxFrameLength = ((YellowTime / EDDP_TIME_BYTE_RESOLUTION_RT_FRAME_NS) - EDDP_SWI_REG_TXFRAMELENGTH_YELLOW_SAFETY_MARGIN_PNIP_REV2); 
                    }
                    break;
                    #endif
                    
                    default:
                    {
                        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_Setup_TxFragmentation(): FATAL -> Version(%d) of PNIP is unknown", 
                            EDDP_HALGetPnipRevision(pDDB));
                        EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                    }
                    break;
                }
                // patch TxFrameLength to maximum if it is greater than maximum
                if (TxFrameLength > PNIP_REG_TXFRAMELENGTH_P1_RST__VAL)
                {
                    TxFrameLength = PNIP_REG_TXFRAMELENGTH_P1_RST__VAL;
                }
                // if (TxFrameLength < Fragmentation_Control.Min_FrameLength), FATAL
                EDDP_ASSERT_FALSE(TxFrameLength < PNIP_REG_FRAGMENTATION_CONTROL_RST__VAL);
            }
        }

        /* ---------------------------------------------------------------------------- */
        /* Setup TxFragmentation                                                        */
        /* ---------------------------------------------------------------------------- */
        if (EnableTxFragmentation)
        {
            /* ---------------------------- */
            /* enable TxFragmentation       */
            /* ---------------------------- */
            // enable TxFragmentation in PortControl_Px.TxD_Fragmentation
            reg_address = (HwPortID==1 ? PNIP_REG_PORTCONTROL_P1 : 
                          (HwPortID==2 ? PNIP_REG_PORTCONTROL_P2 : 
                          (HwPortID==3 ? PNIP_REG_PORTCONTROL_P3 : 
                          (HwPortID==4 ? PNIP_REG_PORTCONTROL_P4 : 0))));
            reg_val = EDDP_HAL_REG32_READ(pDDB, reg_address);
            EDDP_SET_BIT_VALUE32(reg_val, 0x00000001, PNIP_REG_PORTCONTROL_P1__MSK_TXD_FRAGMENTATION, PNIP_REG_PORTCONTROL_P1_SHFT_TXD_FRAGMENTATION);
            EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_val);

            // set TxFrameLength to register TxFrameLength_Px
            reg_address = (HwPortID==1 ? PNIP_REG_TXFRAMELENGTH_P1 : 
                          (HwPortID==2 ? PNIP_REG_TXFRAMELENGTH_P2 : 
                          (HwPortID==3 ? PNIP_REG_TXFRAMELENGTH_P3 : 
                          (HwPortID==4 ? PNIP_REG_TXFRAMELENGTH_P4 : 0))));
            EDDP_HAL_REG32_WRITE(pDDB, reg_address, TxFrameLength);

            // LinkStatus.TxFragmentation is ON
            pDDB->SWI.LinkStatus[PortID].TxFragmentation = EDD_PORT_TX_FRAGMENTATION;

            EDDP_SWI_TRACE_07(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Setup_TxFragmentation(UsrPortID=%d): enable TxFragmentation in REG_PORTCONTROL(HwPortID=%d), FragmentationMode(0x%X), YellowTime(%d), isRedPhasePresent(0x%X), TxFrameLength=%d/0x%X", 
                PortID, HwPortID, FragmentationMode, YellowTime, isRedPhasePresent, TxFrameLength, TxFrameLength);
        }
        else
        {
            /* ---------------------------- */
            /* disable TxFragmentation      */
            /* ---------------------------- */
            // set TxFrameLength to register TxFrameLength_Px
            reg_address = (HwPortID==1 ? PNIP_REG_TXFRAMELENGTH_P1 : 
                          (HwPortID==2 ? PNIP_REG_TXFRAMELENGTH_P2 : 
                          (HwPortID==3 ? PNIP_REG_TXFRAMELENGTH_P3 : 
                          (HwPortID==4 ? PNIP_REG_TXFRAMELENGTH_P4 : 0))));
            EDDP_HAL_REG32_WRITE(pDDB, reg_address, TxFrameLength);

            // disable TxFragmentation in PortControl_Px.TxD_Fragmentation
            reg_address = (HwPortID==1 ? PNIP_REG_PORTCONTROL_P1 : 
                          (HwPortID==2 ? PNIP_REG_PORTCONTROL_P2 : 
                          (HwPortID==3 ? PNIP_REG_PORTCONTROL_P3 : 
                          (HwPortID==4 ? PNIP_REG_PORTCONTROL_P4 : 0))));
            reg_val = EDDP_HAL_REG32_READ(pDDB, reg_address);
            EDDP_SET_BIT_VALUE32(reg_val, 0x00000000, PNIP_REG_PORTCONTROL_P1__MSK_TXD_FRAGMENTATION, PNIP_REG_PORTCONTROL_P1_SHFT_TXD_FRAGMENTATION);
            EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_val);

            // LinkStatus.TxFragmentation is OFF
            pDDB->SWI.LinkStatus[PortID].TxFragmentation = EDD_PORT_NO_TX_FRAGMENTATION;

            EDDP_SWI_TRACE_07(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Setup_TxFragmentation(UsrPortID=%d): disable TxFragmentation in REG_PORTCONTROL(HwPortID=%d), FragmentationMode(0x%X), YellowTime(%d), isRedPhasePresent(0x%X), TxFrameLength=%d/0x%X", 
                PortID, HwPortID, FragmentationMode, YellowTime, isRedPhasePresent, TxFrameLength, TxFrameLength);
        }

        /* ---------------------------------------------------------------------------- */
        /* Setup Domain-Boundary-Port (Forwarding-Mode) for "Store-and-Forward"         */
        /*  - for green port    : no more changes                                       */
        /*  - for red port      : it will be changed on RTC3-PortStatemachine           */
        /* ---------------------------------------------------------------------------- */
        EDDP_SWI_Setup_DomainBoundaryPort (pDDB, PortID, LSA_FALSE);
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_Setup_DomainBoundaryPort           +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_UINT32                  PortID          +*/
/*+                             LSA_BOOL                    isIRTPortStatusRun+*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                 +*/
/*+  PortID             : User Port ID (1..4)                               +*/
/*+  isIRTPortStatusRun : the RTC3PortState of given Port is RUN or not RUN +*/
/*+                       or it is green or not green port                  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function activates the Domain-Boundary-Port          +*/
/*+               for Fragmentation "Store-and-Forward"                     +*/
/*+               in register PortControl_Px.Forwarding_Mode if:            +*/
/*+                 - PDIRData is present                                   +*/
/*+                 - YellowTime is present in PDIRGlobalData               +*/
/*+                 - YellowTime < YellowTimeMax                            +*/
/*+                 - given IRTPortStatus ist not RUN                       +*/
/*+               else set "Cut-Through" in register                        +*/
/*+               PortControl_Px.Forwarding_Mode                            +*/
/*+                                                                         +*/
/*+               Take PrmData always from active RecordsetA.               +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Setup_DomainBoundaryPort(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  PortID,
    LSA_BOOL                    isIRTPortStatusRun)
{
    LSA_UINT32                      HwPortID;
    LSA_UINT32                      reg_val, reg_address;
    LSA_UINT32                      YellowTime;
    LSA_BOOL                        InActiveRecord_A, SetDomainBoundary;
    EDDP_PRM_PDIRDATA_PTR_TYPE      pPDIRData;

    InActiveRecord_A = LSA_TRUE;

    if ((PortID > 0) && (PortID <= pDDB->SWI.PortCnt))
    {
        HwPortID            = EDDP_SWI_get_HwPortID(pDDB, PortID);
        YellowTime          = 0;
        SetDomainBoundary   = LSA_FALSE;

        // is PDIRData present ?
        if (EDDP_PRMPDIRDataIsPDIRDataPresent(pDDB, InActiveRecord_A))
        {
            pPDIRData = pDDB->PRM.pPDIRData_A;

            // is YellowTime present in PDIRGlobalData ?
            if (pPDIRData->PDIRGlobalData.DelayValues[PortID-1].YellowTime_IsPresent)
            {
                // get YellowTime from PDIRGlobalData
                YellowTime = pPDIRData->PDIRGlobalData.DelayValues[PortID-1].YellowTime;
                EDDP_ASSERT_FALSE(!(EDDP_PRMPDIRDataChekYellowTimeInValidRange(pDDB, YellowTime)));

                // activate the Domain-Boundary-Port for Fragmentation, if (YellowTime < YellowTimeMax) and (IRTPortStatus ist not RUN)
                if ( (YellowTime < EDDP_PRM_PDIRGLOBALDATA_YELLOWTIME_MAX) && (!(isIRTPortStatusRun)) )
                {
                    SetDomainBoundary = LSA_TRUE;
                }
            }
        }

        reg_address = (HwPortID==1 ? PNIP_REG_PORTCONTROL_P1 : 
                      (HwPortID==2 ? PNIP_REG_PORTCONTROL_P2 : 
                      (HwPortID==3 ? PNIP_REG_PORTCONTROL_P3 : 
                      (HwPortID==4 ? PNIP_REG_PORTCONTROL_P4 : 0))));
        reg_val = EDDP_HAL_REG32_READ(pDDB, reg_address);

        if (SetDomainBoundary)
        {
            /* -------------------------------------------------------- */
            /* PortControl_Px.Forwarding_Mode = Store-and-Forward       */
            /* -------------------------------------------------------- */
            EDDP_SET_BIT_VALUE32(reg_val, 0x00000001, PNIP_REG_PORTCONTROL_P1__MSK_FORWARDING_MODE, PNIP_REG_PORTCONTROL_P1_SHFT_FORWARDING_MODE);

            EDDP_SWI_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Setup_DomainBoundaryPort(UsrPortID=%d, HwPortID=%d, isIRTPortStatusRun=0x%X): set REG_PORTCONTROL.ForwardingMode(Store-and-Forward), YellowTime(%d), TxFragmentation(0x%X)", 
                PortID, HwPortID, isIRTPortStatusRun, YellowTime, pDDB->SWI.LinkStatus[PortID].TxFragmentation);
        }
        else
        {
            /* -------------------------------------------------------- */
            /* PortControl_Px.Forwarding_Mode = Cut-Through             */
            /* -------------------------------------------------------- */
            EDDP_SET_BIT_VALUE32(reg_val, 0x00000000, PNIP_REG_PORTCONTROL_P1__MSK_FORWARDING_MODE, PNIP_REG_PORTCONTROL_P1_SHFT_FORWARDING_MODE);

            EDDP_SWI_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Setup_DomainBoundaryPort(UsrPortID=%d, HwPortID=%d, isIRTPortStatusRun=0x%X): set REG_PORTCONTROL.ForwardingMode(Cut-Through), YellowTime(%d), TxFragmentation(0x%X)", 
                PortID, HwPortID, isIRTPortStatusRun, YellowTime, pDDB->SWI.LinkStatus[PortID].TxFragmentation);
        }

        EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_val);
    }
    else
    {
        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_SWI_Setup_DomainBoundaryPort(PortID=%d): given PortID is out of range!", 
            PortID);
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_Setup_Phase_List_Base_Addr         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function sets up all PNIP_REG_PHASEBASE_RXD_PX and   +*/
/*+               PNIP_REG_PHASEBASE_TXD_PX registers according to          +*/
/*+               UserPortID-to-HwPortID-mapping                            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Setup_Phase_List_Base_Addr(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT32                  pnip_address;
    LSA_UINT32                  phase_list_rx_ram_index;
    LSA_UINT32                  phase_list_tx_ram_index;
    LSA_UINT32                  phase_list_rx_reg_address;
    LSA_UINT32                  phase_list_tx_reg_address;
    LSA_UINT32                  i;
    LSA_UINT32                  HwPortID;
    EDD_COMMON_MEM_PTR_TYPE     host_address;

    // set up PNIP_REG_PHASEBASE_RX/TXD_PX register according to UserPortID-to-HwPortID-mapping
    for ( i=1; i <= pDDB->SWI.PortCnt; i++ )
    {
        HwPortID = EDDP_SWI_get_HwPortID(pDDB, i);

        phase_list_rx_ram_index   = (HwPortID==1 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P1 : 
                                    (HwPortID==2 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P2 : 
                                    (HwPortID==3 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P3 : 
                                    (HwPortID==4 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P4 : 0))));
        phase_list_tx_ram_index   = (HwPortID==1 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P1 : 
                                    (HwPortID==2 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P2 : 
                                    (HwPortID==3 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P3 : 
                                    (HwPortID==4 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P4 : 0))));

        phase_list_rx_reg_address = (HwPortID==1 ? PNIP_REG_PHASEBASE_RXD_P1 : 
                                    (HwPortID==2 ? PNIP_REG_PHASEBASE_RXD_P2 : 
                                    (HwPortID==3 ? PNIP_REG_PHASEBASE_RXD_P3 : 
                                    (HwPortID==4 ? PNIP_REG_PHASEBASE_RXD_P4 : 0))));
        phase_list_tx_reg_address = (HwPortID==1 ? PNIP_REG_PHASEBASE_TXD_P1 : 
                                    (HwPortID==2 ? PNIP_REG_PHASEBASE_TXD_P2 : 
                                    (HwPortID==3 ? PNIP_REG_PHASEBASE_TXD_P3 : 
                                    (HwPortID==4 ? PNIP_REG_PHASEBASE_TXD_P4 : 0))));

        // write PNIP_REG_PHASEBASE_RXD_PX register
        host_address = EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_SYNC, phase_list_rx_ram_index, 0);
        pnip_address = EDDP_HALGetPnipRAMOffsetToRAM(pDDB, EDDP_HAL_RAMTYPE_SYNC, host_address);
        EDDP_HAL_REG32_WRITE(pDDB, phase_list_rx_reg_address, pnip_address);

        // write PNIP_REG_PHASEBASE_TXD_PX register
        host_address = EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_SYNC, phase_list_tx_ram_index, 0);
        pnip_address = EDDP_HALGetPnipRAMOffsetToRAM(pDDB, EDDP_HAL_RAMTYPE_SYNC, host_address);
        EDDP_HAL_REG32_WRITE(pDDB, phase_list_tx_reg_address, pnip_address);         
     }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_Set_IRT_DelayTime                  +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                        :    LSA_UINT32                  PortID          +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  PortID     : User Port ID (1..4)                                  (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called at PRM_Commit after set up of     +*/
/*+               the IRT-Table and is also called                          +*/
/*+               every time GSY provides a new cable delay. This function  +*/
/*+               sets the register REG_IRT_DELAYTIME_Px.                   +*/
/*+                                                                         +*/
/*+               If "RelativeTime-Mode"                                    +*/
/*+                     - calculate new IRT_DelayTime and set it            +*/
/*+                       to REG_IRT_DELAYTIME_Px                           +*/
/*+               Other mode                                                +*/
/*+                     - set REG_IRT_DELAYTIME_Px to default               +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Set_IRT_DelayTime(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT32                  PortID)
{
    LSA_UINT32          reg_address, reg_val;
    LSA_UINT32          IRT_DelayTime;
    LSA_UINT32          PRM_MaxLineRxDelay;
    LSA_UINT32          PRM_MaxBridgeDelay;
    LSA_UINT32          LineRxDelay;
    LSA_UINT32          HwPortID;

    if ((PortID > 0) && (PortID <= pDDB->SWI.PortCnt))
    {
        IRT_DelayTime       = 0;
        HwPortID            = EDDP_SWI_get_HwPortID(pDDB, PortID);
        LineRxDelay         = pDDB->SWI.LinkStatus[PortID].LineDelayInTicksMeasured * (EDDP_TIME_TICKS_RESOLUTION_NS);
        PRM_MaxLineRxDelay  = pDDB->SWI.LinkStatus[PortID].PRM_MaxLineRxDelay;
        PRM_MaxBridgeDelay  = pDDB->SWI.PRM_MaxBridgeDelay;

        reg_address = (HwPortID==1 ? PNIP_REG_IRT_DELAYTIME_P1 : 
                      (HwPortID==2 ? PNIP_REG_IRT_DELAYTIME_P2 : 
                      (HwPortID==3 ? PNIP_REG_IRT_DELAYTIME_P3 : 
                      (HwPortID==4 ? PNIP_REG_IRT_DELAYTIME_P4 : 0))));

        switch (pDDB->SWI.IRTtopFwMode)
        {
            case EDDP_SWI_IRTTOP_FW_MODE_RELATIVETIME:
            {
                /* ------------------------------------------------------------------------------------ */
                /* IRT_DelayTime = MaxBridgeDelay + (MaxLineRxDelay – LineRxDelay) - Compensation       */
                /* ------------------------------------------------------------------------------------ */
                // set DelayTime always at least to PRM_MaxBridgeDelay
                IRT_DelayTime = ( PRM_MaxBridgeDelay + (0) - EDDP_SWI_IRT_DELAYTIME_COMPENSATION );

                /* ------------------------------------------------------------------------------------ */
                /* set the new DelayTime to IRT_DelayTime_Px register only if                           */
                /*  - there is a valid MaxLineRxDelay from PRM                                          */
                /*  - there is a valid LineRxDelay from GSY                                             */
                /* ------------------------------------------------------------------------------------ */
                // check (PRM_MaxLineRxDelay != 0x0) because it is possibly
                // that the PRM_MaxLineRxDelay is not present for port without RED phase
                if ((PRM_MaxLineRxDelay != 0x0) && (LineRxDelay != 0x0))
                {
                    if (LineRxDelay > PRM_MaxLineRxDelay)
                    {
                        // LineRxDelay is greater than planned
                        EDDP_SWI_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H:--] EDDP_SWI_Set_IRT_DelayTime(UsrPortID=%d, HwPortID=%d): LineRxDelay(%d) > MaxLineRxDelay(%d)", 
                            PortID, HwPortID, LineRxDelay, PRM_MaxLineRxDelay);
                    }
                    else
                    {
                        IRT_DelayTime = ( PRM_MaxBridgeDelay + (PRM_MaxLineRxDelay - LineRxDelay) - EDDP_SWI_IRT_DELAYTIME_COMPENSATION );
                    }
                }
            }
            break;

            default:
                // nothing to do
            break;
        }

        EDDP_SWI_TRACE_08(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_SWI_Set_IRT_DelayTime(UsrPortID=%d, HwPortID=%d): IRT_DelayTime=%d, PRM_MaxBridgeDelay=%d, PRM_MaxLineRxDelay=%d, LineRxDelay=%d, SWI.IRTState=0x%X, IRTtopFwMode=0x%X", 
            PortID, HwPortID, IRT_DelayTime, PRM_MaxBridgeDelay, PRM_MaxLineRxDelay, LineRxDelay, 
            pDDB->SWI.IRTState, pDDB->SWI.IRTtopFwMode);

        // check max. IRT_DelayTime
        if (IRT_DelayTime > EDDP_SWI_IRT_DELAYTIME_MAX)
        {
            EDDP_SWI_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_Set_IRT_DelayTime(UsrPortID=%d, HwPortID=%d): ERROR -> calculated IRT_DelayTime(%d) > IRT_DelayTimeMax(%d)! Patch it to IRT_DelayTimeMax", 
                PortID, HwPortID, IRT_DelayTime, EDDP_SWI_IRT_DELAYTIME_MAX);
            // set it to maximum
            IRT_DelayTime = EDDP_SWI_IRT_DELAYTIME_MAX;
        }

        // write new IRT_DelayTime to register IRT_DELAYTIME_Px
        reg_val = EDDP_HAL_REG32_READ(pDDB, reg_address);
        EDDP_SET_BIT_VALUE32(reg_val, IRT_DelayTime, PNIP_REG_IRT_DELAYTIME_P1__MSK_DELAY_TIME, PNIP_REG_IRT_DELAYTIME_P1_SHFT_DELAY_TIME);
        EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_val);
    }
    else
    {
        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_SWI_Set_IRT_DelayTime(PortID=%d): given PortID is out of range!", 
            PortID);
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_Trigger_PreambleLength             +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called whenever a state has changed which+*/
/*+               has effect on the PreableLength parameters.               +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Trigger_PreambleLength(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  PortID)
{
    LSA_UINT8                           HwShortPreambleLen;
    LSA_BOOL                            ExpPreamble;
    LSA_UINT32                          HwPortID;
    LSA_UINT32                          reg_val, reg_address;
    EDDP_DDB_SWI_LINK_STATUS_PTR_TYPE   pLinkStatus;

    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_SWI_Trigger_PreambleLength(PortID=%d)", PortID);

/*
    Bedingungen                                                | Preamble        | Expected?
       AdjustPreambleLength    LinkUp/FullDuplex   Neighbor    |                 | (siehe RTC3PSM)
       record present                                          |                 |
    -----------------------------------------------------------|-----------------|----------------
    a) „0“                     don´t care          don´t care  | long Preamble   | OK
    b) „1“                     no                  don´t care  | long Preamble   | NOT_OK
    c) „1“                     don´t care          NOT_OK      | long Preamble   | NOT_OK
    d) „1“                     yes                 OK          | short Preamble  | OK   (to a PNIP Port)
    e) „1“                     yes                 OK          | long Preamble   | OK   (e.g. to a CP1616 Port)
*/
    if ((PortID > 0) && (PortID <= pDDB->SWI.PortCnt))
    {
        HwShortPreambleLen  = EDD_PORT_LONG_PREAMBLE;
        HwPortID            = EDDP_SWI_get_HwPortID(pDDB, PortID);
        pLinkStatus         = &pDDB->SWI.LinkStatus[PortID];
  
        EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Trigger_PreambleLength(UsrPortID=%d, HwPortID=%d): PreambleLength_IsPresent(0x%X)", 
            PortID, HwPortID, pDDB->SWI.PrmPreabmleCfg[PortID].PreambleLength_IsPresent);
        // do not delete the empty spaces in this trace text
        EDDP_SWI_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--]    Preamble-Params: PreambleLength(0x%X), Neighbour_TopoState(0x%X), LinkStatus(0x%X), LinkDuplexity(0x%X)", 
            pDDB->SWI.PrmPreabmleCfg[PortID].PreambleLength, 
            pLinkStatus->Neighbour_TopoState, 
            pLinkStatus->LinkStatus, 
            pLinkStatus->LinkDuplexity);

        if (!(pDDB->SWI.PrmPreabmleCfg[PortID].PreambleLength_IsPresent))
        {   // ***** a) „0“                     don´t care          don´t care  | long Preamble   | OK
            HwShortPreambleLen  = EDD_PORT_LONG_PREAMBLE;             // long Preamble
            ExpPreamble         = EDDP_SWI_PREAMBLE_OK;  // OK
        }
        else
        {
            if (   (   (pDDB->SWI.LinkStatus[PortID].LinkStatus == EDD_LINK_UP) 
                    || (pDDB->SWI.LinkStatus[PortID].LinkStatus == EDD_LINK_UP_CLOSED) 
                    || (pDDB->SWI.LinkStatus[PortID].LinkStatus == EDD_LINK_UP_DISABLED)
                   )
                && (pLinkStatus->LinkDuplexity == EDD_LINK_MODE_FULL) 
                && (pLinkStatus->Neighbour_TopoState == EDD_SET_REMOTE_PORT_STATE_TOPO_OK)
               )
            {   // ***** d) „1“                     yes                 OK          | short Preamble  | OK   (to a PNIP Port)
                // ***** e) „1“                     yes                 OK          | long Preamble   | OK   (e.g. to a CP1616 Port)
                if (pDDB->SWI.PrmPreabmleCfg[PortID].PreambleLength /* !=0 */)
                {
                    HwShortPreambleLen = EDD_PORT_SHORT_PREAMBLE;      // short Preamble
                }
                else
                {
                    HwShortPreambleLen = EDD_PORT_LONG_PREAMBLE;     // long Preamble
                }
                ExpPreamble = EDDP_SWI_PREAMBLE_OK;     // Ok
            }
            else
            {   // ***** b) „1“                     no                  don´t care  | long Preamble   | NOT_OK
                // ***** c) „1“                     don´t care          NOT_OK      | long Preamble   | NOT_OK
                HwShortPreambleLen  = EDD_PORT_LONG_PREAMBLE;                    // long Preamble
                ExpPreamble         = EDDP_SWI_PREAMBLE_NOT_OK;     // NOT_OK
            }
        }

        if (pDDB->SWI.RTC3_PortState[PortID].Preamble != ExpPreamble)
        {
            EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Trigger_PreambleLength(UsrPortID=%d, HwPortID=%d): ExpectedPreamble has changed to: 0x%X", 
                PortID, HwPortID, ExpPreamble);
            pDDB->SWI.RTC3_PortState[PortID].Preamble = ExpPreamble; 
        }

        if (pLinkStatus->ShortPreamble == HwShortPreambleLen)
        {
            // nothing to do
            EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Trigger_PreambleLength(UsrPortID=%d, HwPortID=%d): HwPreamble(0x%X) has not changed", 
                PortID, HwPortID, HwShortPreambleLen);
        }
        else
        {
            EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Trigger_PreambleLength(UsrPortID=%d, HwPortID=%d): HwPreamble has changed to 0x%X: (1=short/0=long))", 
                PortID, HwPortID, HwShortPreambleLen);

            reg_address = (HwPortID==1 ? PNIP_REG_PREAMBLELENGTH_P1 : 
                          (HwPortID==2 ? PNIP_REG_PREAMBLELENGTH_P2 : 
                          (HwPortID==3 ? PNIP_REG_PREAMBLELENGTH_P3 : 
                          (HwPortID==4 ? PNIP_REG_PREAMBLELENGTH_P4 : 0))));

            if (reg_address == 0)
            {
                EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_Trigger_PreambleLength(): FATAL -> HwPortID(%d) unexpected value! reg_address=0, UsrPortID=%d", 
                    HwPortID, PortID);
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            }

            // ***** change preamble
            if (HwShortPreambleLen)
            {   // ***** setup short preamble
                reg_val = EDDP_SWI_REG_PREAMBLELENGTH_SHORT_PREAMBLE;
            }
            else
            {   // ***** setup long preamble
                reg_val = EDDP_SWI_REG_PREAMBLELENGTH_LONG_PREAMBLE;
            }
            EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_val);
            // ***** set new setting
            pLinkStatus->ShortPreamble = HwShortPreambleLen;

            // **** notify user of port status changes
            EDDP_GENIndicateLinkChange_Ext(pDDB, PortID);
        }
    }
    else
    {
        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_SWI_Trigger_PreambleLength(PortID=%d): given PortID is out of range!", 
            PortID);
    }

    EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_SWI_Trigger_PreambleLength()");
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_Trigger_RTC3_PortState             +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_UINT32                  PortID          +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  PortID     : PortID                                               (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called whenever a state has changed which+*/
/*+               has effect on the RTC3 port state.                        +*/
/*+                                                                         +*/
/*+               Switch to:                                                +*/
/*+               - Trigger_RTC3_PortState_Rev(X): for PNIP-Revision(X)     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Trigger_RTC3_PortState(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  PortID)
{
    // switch to current PNIP revision
    switch (EDDP_HALGetPnipRevision(pDDB))
    {
        #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
        case EDDP_HAL_HW_PNIP_REV1:
        {
            // set "isRTC3PSMNewParamPresentMask"
            EDDP_SWI_Setup_RTC3PSMNewParamPresentMask (pDDB, PortID, LSA_TRUE);
            EDDP_SWI_Trigger_RTC3_PortState_Rev1 (pDDB, PortID);
        }
        break;
        #endif

        #if  defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
        case EDDP_HAL_HW_PNIP_REV2:
        case EDDP_HAL_HW_PNIP_REV3:
        {
            EDDP_SWI_Trigger_RTC3_PortState_RevOther (pDDB, PortID);
        }
        break;
        #endif

        default:
        {
            EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_Trigger_RTC3_PortState(): FATAL -> Version(%d) of PNIP is unknown", 
                EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_Trigger_RTC3_PortState_Rev1        +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_UINT32                  PortID          +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  PortID     : PortID                                               (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called whenever a state has changed which+*/
/*+               has effect on the RTC3 port state. The RTC3 port state is +*/
/*+               set based on this truth-table:                            +*/
/*+                                                                         +*/
/*+               This function is only for PNIP-Revision(1)                +*/
/*+                                                                         +*/
/*+ +-------------------+-----------------+--------------------------------++*/  
/*+ | CHECK_LOCAL_OK    |                 |                                |+*/
/*+ | CHECK_NEIGHBOR_OK |                 |                                |+*/
/*+ | CHECK_REMOTE_OK   |                 |                                |+*/ 
/*+ | CHECK_SYNC_OK     | REMOTEPORTSTATE | RTClass3 PortState             |+*/
/*+ +-------------------+-----------------+--------------------------------++*/
/*+ |at least one not OK| OFF             | OFF                            |+*/
/*+ |all OK             | OFF             | TX   :RED starts at time 0     |+*/
/*+ |all OK             | TX              | RXTX :RED starts at time 0     |+*/
/*+ |all OK             | TXRX            | RXTX :RED starts at "real" time|+*/
/*+ +-------------------+-----------------+--------------------------------++*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Trigger_RTC3_PortState_Rev1(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  PortID)
{
    EDD_RSP                                 Response;
    LSA_UINT32                              HwPortID;
    LSA_UINT32                              reg_val, reg_address;
    LSA_BOOL                                isIRTPortStatusRun;
    LSA_UINT16                              InBoundIdx;
    LSA_UINT8                               InboundUsedMask, InboundActiveMask;
    LSA_UINT32                              InboundPackRegAddr, InboundRegVal;
    LSA_UINT16                              TimerUserIdx;
    EDDP_CRT_INBOUND_MNGM_PTR_TYPE      pInboundMgmt;

    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_SWI_Trigger_RTC3_PortState_Rev1(PortID=%d)", PortID);

    EDDP_ASSERT_FALSE(EDDP_HAL_HW_PNIP_REV1 != EDDP_HALGetPnipRevision(pDDB));

    Response        = EDD_STS_OK;
    TimerUserIdx    = EDDP_CRT_WRKAR_PACKGRP_DISABLE_TIMER_USER_MAX_NUMBER;

    if ((PortID > 0) && (PortID <= pDDB->SWI.PortCnt))
    {
        HwPortID            = EDDP_SWI_get_HwPortID (pDDB, PortID);
        reg_val             = EDDP_SWI_PHASE_ENABLE_OFF;
        isIRTPortStatusRun  = LSA_FALSE;

        reg_address = (HwPortID==1 ? PNIP_REG_PHASEENABLE_P1 : 
                      (HwPortID==2 ? PNIP_REG_PHASEENABLE_P2 : 0));

        EDDP_SWI_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Trigger_RTC3_PortState_Rev1(UsrPortID=%d, HwPortID=%d): IRTPortStatus=0x%X, isPrmCommitRunning=0x%X, isRTC3PSMRunningAsynMask=0x%X", 
            PortID, HwPortID, pDDB->SWI.LinkStatus[PortID].IRTPortStatus, pDDB->PRM.isPrmCommitRunning, pDDB->SWI.isRTC3PSMRunningAsynMask);
        // do not delete the empty spaces in this trace text
        EDDP_SWI_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--]    RTC3-Params: SyncOk(0x%X), RTC3PST-TopoState(0x%X), RTC3PST-PDEVTopoState(0x%X), RTC3PST-RemoteState(0x%X), LinkStatus(0x%X), Preamble(%d)", 
            pDDB->SWI.SyncOk,                               pDDB->SWI.RTC3_PortState[PortID].TopoState, 
            pDDB->SWI.RTC3_PortState[PortID].PDEVTopoState, pDDB->SWI.RTC3_PortState[PortID].RemoteState, 
            pDDB->SWI.LinkStatus[PortID].LinkStatus,        pDDB->SWI.RTC3_PortState[PortID].Preamble);

        /* ---------------------------------------------------------------------------- */
        /* only trigger RTC3PSM if:                                                     */
        /*  - this port is active IRT port (PDIRData record has been written at PRM)    */
        /*  - no PrmCommit is running currently                                         */
        /*  - no RTC3PSM is already running asynchron for this port                     */
        /* ---------------------------------------------------------------------------- */
        if (   (pDDB->SWI.LinkStatus[PortID].IRTPortStatus != EDD_IRT_NOT_SUPPORTED) 
            && (!(pDDB->PRM.isPrmCommitRunning)) 
            && (!(EDDP_SWI_IsSet_RTC3PSMRunningAsynMask(pDDB, PortID))) 
           )
        {
            // delete "isRTC3PSMNewParamPresentMask"
            EDDP_SWI_Setup_RTC3PSMNewParamPresentMask (pDDB, PortID, LSA_FALSE);

            if (   (pDDB->SWI.SyncOk)
                && (pDDB->SWI.RTC3_PortState[PortID].PDEVTopoState   == EDD_SET_REMOTE_PORT_STATE_TOPO_OK)
                && (pDDB->SWI.RTC3_PortState[PortID].TopoState       == EDD_SET_REMOTE_PORT_STATE_TOPO_OK)
                && (   (pDDB->SWI.LinkStatus[PortID].LinkStatus     == EDD_LINK_UP) 
                    || (pDDB->SWI.LinkStatus[PortID].LinkStatus     == EDD_LINK_UP_CLOSED) 
                    || (pDDB->SWI.LinkStatus[PortID].LinkStatus     == EDD_LINK_UP_DISABLED)
                   )
                && (pDDB->SWI.LinkStatus[PortID].LinkSpeed           == EDD_LINK_SPEED_100)
                && (pDDB->SWI.LinkStatus[PortID].LinkDuplexity       == EDD_LINK_MODE_FULL)
                && (pDDB->SWI.RTC3_PortState[PortID].Preamble        == EDDP_SWI_PREAMBLE_OK)
               )
            {
                switch (pDDB->SWI.RTC3_PortState[PortID].RemoteState)
                {
                    case EDD_SET_REMOTE_PORT_STATE_RT_CLASS3_OFF:
                    {
                        reg_val = (EDDP_SWI_PHASE_ENABLE_START_TX_RED | EDDP_SWI_PHASE_ENABLE_TX_RED);
                        // store new RTC3 state
                        pDDB->SWI.LinkStatus[PortID].IRTPortStatus = EDD_IRT_PORT_ACTIVE_TX_UP;

                        EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Trigger_RTC3_PortState_Rev1(UsrPortID=%d, HwPortID=%d): new IRTPortStatus(0x%X) = UP (ACTIVE_TX_UP Start at 0)", 
                            PortID, HwPortID, pDDB->SWI.LinkStatus[PortID].IRTPortStatus);
                    }
                    break;

                    case EDD_SET_REMOTE_PORT_STATE_RT_CLASS3_TX:
                    {
                        reg_val = (EDDP_SWI_PHASE_ENABLE_START_TX_RED | EDDP_SWI_PHASE_ENABLE_TX_RED | EDDP_SWI_PHASE_ENABLE_RX_RED);
                        // store new RTC3 state
                        pDDB->SWI.LinkStatus[PortID].IRTPortStatus = EDD_IRT_PORT_ACTIVE_TX_RX;

                        EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Trigger_RTC3_PortState_Rev1(UsrPortID=%d, HwPortID=%d): new IRTPortStatus(0x%X) = RUN (ACTIVE_TX_RX Start at 0)", 
                            PortID, HwPortID, pDDB->SWI.LinkStatus[PortID].IRTPortStatus);
                    }
                    break;

                    case EDD_SET_REMOTE_PORT_STATE_RT_CLASS3_RXTX:
                    {
                        reg_val = (EDDP_SWI_PHASE_ENABLE_TX_RED | EDDP_SWI_PHASE_ENABLE_RX_RED);
                        // store new RTC3 state
                        pDDB->SWI.LinkStatus[PortID].IRTPortStatus = EDD_IRT_PORT_ACTIVE_TX_RX;
                        // IRT-Port is RUN
                        isIRTPortStatusRun = LSA_TRUE;

                        EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Trigger_RTC3_PortState_Rev1(UsrPortID=%d, HwPortID=%d): new IRTPortStatus(0x%X) = RUN (ACTIVE_TX_RX)", 
                            PortID, HwPortID, pDDB->SWI.LinkStatus[PortID].IRTPortStatus);
                    }
                    break;

                    default:
                    {
                        EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_Trigger_RTC3_PortState_Rev1(UsrPortID=%d, HwPortID=%d): FATAL -> invalid RTC3PST-RemoteState(0x%X)", 
                            PortID, HwPortID, pDDB->SWI.RTC3_PortState[PortID].RemoteState);
                        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                    }
                    break;
                }
            }
            else
            {
                // set RTC3-PortState to OFF for this port -> disable RED-Phase for Tx/Rx
                reg_val = EDDP_SWI_PHASE_ENABLE_OFF;
                // store current RTC3 state
                pDDB->SWI.LinkStatus[PortID].IRTPortStatus = EDD_IRT_PORT_INACTIVE;

                EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Trigger_RTC3_PortState_Rev1(UsrPortID=%d, HwPortID=%d): new IRTPortStatus(0x%X) = OFF (INACTIVE)", 
                    PortID, HwPortID, pDDB->SWI.LinkStatus[PortID].IRTPortStatus);
            }

            /* -------------------------------------------------------------------------------- */
            /* enable/disable full RED Rx/Tx                                                    */
            /* -------------------------------------------------------------------------------- */
            if (isIRTPortStatusRun)
            {
                /* -------------------------------------------------------- */
                /* enable full RED Rx/Tx                                    */
                /* -------------------------------------------------------- */
                // IMPORTANT - The activate order of RED-Phase in HW
                //      1) activate the RED-Phase
                EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_val);
                //      2) setup DomainBoundaryPort for "Cut-Through"
                EDDP_SWI_Setup_DomainBoundaryPort (pDDB, PortID, LSA_TRUE);
                //      3) set Rx/Tx-Boundary-DP2 inactive
                EDDP_CRTSetRxTxDP2BoundaryAndInbound (pDDB, HwPortID, LSA_FALSE);
            }
            else
            {
                /* -------------------------------------------------------- */
                /* disable full RED Rx/Tx                                   */
                /*  - synchron, if no active inbound groups are present     */
                /*  - asynchron, if active inbound groups are present       */
                /* -------------------------------------------------------- */
                pInboundMgmt        = &pDDB->CRT.Inbound;
                InboundUsedMask     = pInboundMgmt->InboundUsedMask[HwPortID];              // all used inbound groups for given HwPortID
                InboundActiveMask   = (InboundUsedMask & pInboundMgmt->InboundActiveMask);  // all active inbound groups for given HwPortID

                if (InboundActiveMask != 0)
                {
                    // active inbound groups for given HwPortID are present - the RTC3PSM will be asynchron
                    Response = EDD_STS_OK_PENDING;
                }

                switch (Response)
                {
                    case EDD_STS_OK:
                    {
                        // the RTC3PSM is synchron
                        // IMPORTANT - The passivate order of RED-Phase in HW
                        //      1) set Rx/Tx-Boundary-DP2 active
                        EDDP_CRTSetRxTxDP2BoundaryAndInbound (pDDB, HwPortID, LSA_TRUE);
                        //      2) setup DomainBoundaryPort for "Store-and-Forward"
                        EDDP_SWI_Setup_DomainBoundaryPort (pDDB, PortID, LSA_FALSE);
                        //      3) passivate the RED-Phase
                        EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_val);
                    }
                    break;

                    case EDD_STS_OK_PENDING:
                    {
                        #ifdef EDDP_MESSAGE
                        #ifdef EDDP_PRAGMA_MESSAGE
                        #pragma EDDP_PRAGMA_MESSAGE("Workaround CRT.WrkarPackGrpDisable for PNIP-Revision(1) - AP01372025")
                        #endif /* EDDP_PRAGMA_MESSAGE */
                        #endif /* EDDP_MESSAGE */
                        // the RTC3PSM is asynchron
                        EDDP_SWI_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_SWI_Trigger_RTC3_PortState_Rev1(UsrPortID=%d, HwPortID=%d): workaround PackGrp-Disable - InboundActiveMask=0x%X, Response=0x%X", 
                            PortID, HwPortID, InboundActiveMask, Response);

                        // set "isRTC3PSMRunningAsynMask"
                        EDDP_SWI_Setup_RTC3PSMRunningAsynMask (pDDB, PortID, LSA_TRUE);

                        // for all active inbound groups set invalid data for workaround PackGrp-Disable
                        InboundPackRegAddr = PNIP_REG_INBOUNDPACK_0;
                        for ( InBoundIdx=0; InBoundIdx < EDDP_CRT_MAX_PACK_FRAMES_ERTEC200P; InBoundIdx++ )
                        {
                            if (InboundActiveMask & (1<<InBoundIdx))
                            {
                                EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] EDDP_SWI_Trigger_RTC3_PortState_Rev1(UsrPortID=%d, HwPortID=%d): workaround PackGrp-Disable - PackGrp(%d) set invalid data", 
                                    PortID, HwPortID, InBoundIdx);

                                // disable interrupt "ISR_InBound_Late_ERR" of this PackGrp
                                EDDP_CRTLateErrorSetupLateErrMask (pDDB, (LSA_UINT8)InBoundIdx, LSA_TRUE);
                                // set invalid "InBoundPack_x.TxPort = 4 (0x3)"
                                InboundRegVal = EDDP_HAL_REG32_READ(pDDB, InboundPackRegAddr);
                                EDDP_SET_BIT_VALUE32 (InboundRegVal, 0x3, PNIP_REG_INBOUNDPACK_0__MSK_INBOUND_TXPORT, PNIP_REG_INBOUNDPACK_0_SHFT_INBOUND_TXPORT);
                                EDDP_HAL_REG32_WRITE(pDDB, InboundPackRegAddr, InboundRegVal);
                                // set invalid "ACW.DRM/Destination"
                                EDDP_ASSERT_FALSE(pInboundMgmt->InboundGrps[InBoundIdx].WrkarPackGrpDisable.AcwId == EDDP_CRT_CHAIN_IDX_END);
                                EDDP_CRTProvAcwSetupDrmAndDestination (pDDB, pInboundMgmt->InboundGrps[InBoundIdx].WrkarPackGrpDisable.AcwId, LSA_FALSE);
                                // if redundant FrameID(x+1) is present
                                if (pInboundMgmt->InboundGrps[InBoundIdx].WrkarPackGrpDisable.AcwIdRed1 != EDDP_CRT_CHAIN_IDX_END)
                                {
                                    EDDP_CRTProvAcwSetupDrmAndDestination (pDDB, pInboundMgmt->InboundGrps[InBoundIdx].WrkarPackGrpDisable.AcwIdRed1, LSA_FALSE);
                                }
                            }
                            InboundPackRegAddr += 4;
                        }

                        // get TimerUserIdx for this PortID
                        switch (PortID)
                        {
                            case 1: TimerUserIdx = EDDP_CRT_WRKAR_PACKGRP_DISABLE_TIMER_USER_IDX_RTC3PSM_P1;    break;
                            case 2: TimerUserIdx = EDDP_CRT_WRKAR_PACKGRP_DISABLE_TIMER_USER_IDX_RTC3PSM_P2;    break;
                            default:
                            {
                                EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_Trigger_RTC3_PortState_Rev1(): FATAL -> PortID(%d) is invalid!", 
                                    PortID);
                                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                            }
                            break;
                        }
                        // store the new value for PNIP_REG_PHASEENABLE_Px
                        pDDB->CRT.WrkarPackGrpDisable.TimerUser[TimerUserIdx].RegValuePhaseEnable = reg_val;
                        // start the timer of this PortID
                        EDDP_CRTWrkarPackGrpDisableTimerStart (pDDB, TimerUserIdx);
                    }
                    break;

                    default:
                    {
                        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_Trigger_RTC3_PortState_Rev1(): FATAL -> Response(%d) is invalid!", 
                            Response);
                        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                    }
                    break;
                }
            }
        }

        if (Response == EDD_STS_OK)
        {
            // notify user of port status changes
            EDDP_GENIndicateLinkChange_Ext (pDDB, PortID);
            // call RQB-Handler if new RQBs are present in queue
            EDDP_GlbCrtRQBHandler (pDDB);
        }
    }
    else
    {
        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_SWI_Trigger_RTC3_PortState_Rev1(PortID=%d): given PortID is out of range!", 
            PortID);
    }

    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_SWI_Trigger_RTC3_PortState_Rev1(): Response=0x%X", Response);
}
#endif

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_Trigger_RTC3_PortState_RevOther    +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_UINT32                  PortID          +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  PortID     : PortID                                               (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called whenever a state has changed which+*/
/*+               has effect on the RTC3 port state. The RTC3 port state is +*/
/*+               set based on this truth-table:                            +*/
/*+                                                                         +*/
/*+               This function is for PNIP-Revision(2) and (3)             +*/
/*+                                                                         +*/
/*+ +-------------------+-----------------+--------------------------------++*/  
/*+ | CHECK_LOCAL_OK    |                 |                                |+*/
/*+ | CHECK_NEIGHBOR_OK |                 |                                |+*/
/*+ | CHECK_REMOTE_OK   |                 |                                |+*/ 
/*+ | CHECK_SYNC_OK     | REMOTEPORTSTATE | RTClass3 PortState             |+*/
/*+ +-------------------+-----------------+--------------------------------++*/
/*+ |at least one not OK| OFF             | OFF                            |+*/
/*+ |all OK             | OFF             | TX   :RED starts at time 0     |+*/
/*+ |all OK             | TX              | RXTX :RED starts at time 0     |+*/
/*+ |all OK             | TXRX            | RXTX :RED starts at "real" time|+*/
/*+ +-------------------+-----------------+--------------------------------++*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#if  defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Trigger_RTC3_PortState_RevOther(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  PortID)
{
    LSA_UINT32      HwPortID;
    LSA_UINT32      reg_val, reg_address;
    LSA_BOOL        isIRTPortStatusRun;

    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_SWI_Trigger_RTC3_PortState_Rev2(PortID=%d)", PortID);

    EDDP_ASSERT_FALSE(    ( EDDP_HAL_HW_PNIP_REV2 != EDDP_HALGetPnipRevision(pDDB) )
                       && ( EDDP_HAL_HW_PNIP_REV3 != EDDP_HALGetPnipRevision(pDDB) )    );

    if ((PortID > 0) && (PortID <= pDDB->SWI.PortCnt))
    {
        HwPortID            = EDDP_SWI_get_HwPortID (pDDB, PortID);
        reg_val             = EDDP_SWI_PHASE_ENABLE_OFF;
        isIRTPortStatusRun  = LSA_FALSE;

        reg_address = (HwPortID==1 ? PNIP_REG_PHASEENABLE_P1 : 
                      (HwPortID==2 ? PNIP_REG_PHASEENABLE_P2 : 
                      (HwPortID==3 ? PNIP_REG_PHASEENABLE_P3 : 
                      (HwPortID==4 ? PNIP_REG_PHASEENABLE_P4 : 0))));

        EDDP_SWI_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Trigger_RTC3_PortState_Rev2(UsrPortID=%d, HwPortID=%d): IRTPortStatus=0x%X, isPrmCommitRunning=0x%X", 
            PortID, HwPortID, pDDB->SWI.LinkStatus[PortID].IRTPortStatus, pDDB->PRM.isPrmCommitRunning);
        // do not delete the empty spaces in this trace text
        EDDP_SWI_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--]    RTC3-Params: SyncOk(0x%X), RTC3PST-TopoState(0x%X), RTC3PST-PDEVTopoState(0x%X), RTC3PST-RemoteState(0x%X), LinkStatus(0x%X), Preamble(%d)", 
            pDDB->SWI.SyncOk,                               pDDB->SWI.RTC3_PortState[PortID].TopoState, 
            pDDB->SWI.RTC3_PortState[PortID].PDEVTopoState, pDDB->SWI.RTC3_PortState[PortID].RemoteState, 
            pDDB->SWI.LinkStatus[PortID].LinkStatus,        pDDB->SWI.RTC3_PortState[PortID].Preamble);

        /* ---------------------------------------------------------------------------- */
        /* only trigger RTC3PSM if:                                                     */
        /*  - this port is active IRT port (PDIRData record has been written at PRM)    */
        /*  - no PrmCommit is running currently                                         */
        /* ---------------------------------------------------------------------------- */
        if ((pDDB->SWI.LinkStatus[PortID].IRTPortStatus != EDD_IRT_NOT_SUPPORTED) && (!(pDDB->PRM.isPrmCommitRunning)))
        {
            if (   (pDDB->SWI.SyncOk)
                && (pDDB->SWI.RTC3_PortState[PortID].PDEVTopoState   == EDD_SET_REMOTE_PORT_STATE_TOPO_OK)
                && (pDDB->SWI.RTC3_PortState[PortID].TopoState       == EDD_SET_REMOTE_PORT_STATE_TOPO_OK)
                && (   (pDDB->SWI.LinkStatus[PortID].LinkStatus     == EDD_LINK_UP) 
                    || (pDDB->SWI.LinkStatus[PortID].LinkStatus     == EDD_LINK_UP_CLOSED) 
                    || (pDDB->SWI.LinkStatus[PortID].LinkStatus     == EDD_LINK_UP_DISABLED)
                   )
                && (pDDB->SWI.LinkStatus[PortID].LinkSpeed           == EDD_LINK_SPEED_100)
                && (pDDB->SWI.LinkStatus[PortID].LinkDuplexity       == EDD_LINK_MODE_FULL)
                && (pDDB->SWI.RTC3_PortState[PortID].Preamble        == EDDP_SWI_PREAMBLE_OK)
               )
            {
                switch (pDDB->SWI.RTC3_PortState[PortID].RemoteState)
                {
                    case EDD_SET_REMOTE_PORT_STATE_RT_CLASS3_OFF:
                    {
                        reg_val = (EDDP_SWI_PHASE_ENABLE_START_TX_RED | EDDP_SWI_PHASE_ENABLE_TX_RED);
                        // store new RTC3 state
                        pDDB->SWI.LinkStatus[PortID].IRTPortStatus = EDD_IRT_PORT_ACTIVE_TX_UP;

                        EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Trigger_RTC3_PortState_Rev2(UsrPortID=%d, HwPortID=%d): new IRTPortStatus(0x%X) = UP (ACTIVE_TX_UP Start at 0)", 
                            PortID, HwPortID, pDDB->SWI.LinkStatus[PortID].IRTPortStatus);
                    }
                    break;

                    case EDD_SET_REMOTE_PORT_STATE_RT_CLASS3_TX:
                    {
                        reg_val = (EDDP_SWI_PHASE_ENABLE_START_TX_RED | EDDP_SWI_PHASE_ENABLE_TX_RED | EDDP_SWI_PHASE_ENABLE_RX_RED);
                        // store new RTC3 state
                        pDDB->SWI.LinkStatus[PortID].IRTPortStatus = EDD_IRT_PORT_ACTIVE_TX_RX;

                        EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Trigger_RTC3_PortState_Rev2(UsrPortID=%d, HwPortID=%d): new IRTPortStatus(0x%X) = RUN (ACTIVE_TX_RX Start at 0)", 
                            PortID, HwPortID, pDDB->SWI.LinkStatus[PortID].IRTPortStatus);
                    }
                    break;

                    case EDD_SET_REMOTE_PORT_STATE_RT_CLASS3_RXTX:
                    {
                        reg_val = (EDDP_SWI_PHASE_ENABLE_TX_RED | EDDP_SWI_PHASE_ENABLE_RX_RED);
                        // store new RTC3 state
                        pDDB->SWI.LinkStatus[PortID].IRTPortStatus = EDD_IRT_PORT_ACTIVE_TX_RX;
                        // IRT-Port is RUN
                        isIRTPortStatusRun = LSA_TRUE;

                        EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Trigger_RTC3_PortState_Rev2(UsrPortID=%d, HwPortID=%d): new IRTPortStatus(0x%X) = RUN (ACTIVE_TX_RX)", 
                            PortID, HwPortID, pDDB->SWI.LinkStatus[PortID].IRTPortStatus);
                    }
                    break;

                    default:
                    {
                        EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_Trigger_RTC3_PortState_Rev2(UsrPortID=%d, HwPortID=%d): FATAL -> invalid RTC3PST-RemoteState(0x%X)", 
                            PortID, HwPortID, pDDB->SWI.RTC3_PortState[PortID].RemoteState);
                        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                    }
                    break;
                }
            }
            else
            {
                // set RTC3-PortState to OFF for this port -> disable RED-Phase for Tx/Rx
                reg_val = EDDP_SWI_PHASE_ENABLE_OFF;
                // store current RTC3 state
                pDDB->SWI.LinkStatus[PortID].IRTPortStatus = EDD_IRT_PORT_INACTIVE;

                EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Trigger_RTC3_PortState_Rev2(UsrPortID=%d, HwPortID=%d): new IRTPortStatus(0x%X) = OFF (INACTIVE)", 
                    PortID, HwPortID, pDDB->SWI.LinkStatus[PortID].IRTPortStatus);
            }

            /* -------------------------------------------------------- */
            /* enable/disable full RED Rx/Tx                            */
            /* -------------------------------------------------------- */
            if (isIRTPortStatusRun)
            {
                /* ------------------------ */
                /* enable full RED Rx/Tx    */
                /* ------------------------ */
                // IMPORTANT - The activate order of RED-Phase in HW
                //      1) activate the RED-Phase
                EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_val);
                //      2) setup DomainBoundaryPort for "Cut-Through"
                EDDP_SWI_Setup_DomainBoundaryPort (pDDB, PortID, LSA_TRUE);
                //      3) set Rx/Tx-Boundary-DP2 inactive
                EDDP_CRTSetRxTxDP2BoundaryAndInbound (pDDB, HwPortID, LSA_FALSE);
            }
            else
            {
                /* ------------------------ */
                /* disable full RED Rx/Tx   */
                /* ------------------------ */
                // IMPORTANT - The passivate order of RED-Phase in HW
                //      1) set Rx/Tx-Boundary-DP2 active
                EDDP_CRTSetRxTxDP2BoundaryAndInbound (pDDB, HwPortID, LSA_TRUE);
                //      2) setup DomainBoundaryPort for "Store-and-Forward"
                EDDP_SWI_Setup_DomainBoundaryPort (pDDB, PortID, LSA_FALSE);
                //      3) passivate the RED-Phase
                EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_val);
            }
        }

        // notify user of port status changes
        EDDP_GENIndicateLinkChange_Ext (pDDB, PortID);
    }
    else
    {
        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_SWI_Trigger_RTC3_PortState_Rev2(PortID=%d): given PortID is out of range!", 
            PortID);
    }

    EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_SWI_Trigger_RTC3_PortState_Rev2()");
}
#endif


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_Check_PDIRData                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function validates the record PDIRData after the     +*/
/*+               data of this record has been parsed.                      +*/
/*+                 -> call only after EDDP_PRMPDIRDataRecord_Write()       +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWI_Check_PDIRData(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP             Response;    
    LSA_UINT16          FrameID_low;
    LSA_UINT16          FrameID_high;
    LSA_UINT32          Count_IRT_Entries_PDIRFrameData;    // count IRT entries with a entry in PDIRFrameData
    LSA_UINT32          Count_IRT_Entries_PDIRBeginEndData; // count IRT entries with a entry in PDIRBeginEndData
    LSA_UINT32          i, j, k;
    LSA_UINT32          Max_IRT_Entries;

    Response                            = EDD_STS_OK;
    FrameID_low                         = EDD_SRT_FRAMEID_IRT_STOP;
    FrameID_high                        = EDD_SRT_FRAMEID_IRT_START;
    Count_IRT_Entries_PDIRFrameData     = 0;
    Count_IRT_Entries_PDIRBeginEndData  = 0;
    Max_IRT_Entries                     = 0;

    /* ---------------------------------------------------------------------------- */
    /* determine lowest and highest FrameID within all PDIRFrameData                */
    /* ---------------------------------------------------------------------------- */
    for ( i=0; i < pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All; i++ )
    {
        // only for IRT FrameID range
        if (EDDP_IS_IRT_FRAMEID(pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].FrameID))
        {
            // checks only for RelativeTimeMode
            if (!(pDDB->PRM.pPDIRData_B->AbsoluteTimeMode))
            {
                // check whether the PDIRFrameData-FrameID is in Red-Guard range
                if (   (pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].FrameID < pDDB->PRM.pPDIRData_B->PDIRBeginEndData.StartOfRedFrameID)
                    || (pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].FrameID > pDDB->PRM.pPDIRData_B->PDIRBeginEndData.EndOfRedFrameID)
                   )
                {
                    Response = EDD_STS_ERR_PRM_BLOCK;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_SWI_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_Check_PDIRData(): ERROR -> PDIRFrameData-FrameID(0x%X) is out of Red-Guard range [0x%X...0x%X]! FrameDataArray-Entry=%d", 
                        pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].FrameID, pDDB->PRM.pPDIRData_B->PDIRBeginEndData.StartOfRedFrameID, pDDB->PRM.pPDIRData_B->PDIRBeginEndData.EndOfRedFrameID, i);
                    // exit FOR loop
                    i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                }
            }

            if (Response == EDD_STS_OK)
            {
                if (pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].FrameID < FrameID_low)
                {
                    FrameID_low = pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].FrameID;
                }
                if (pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].FrameID > FrameID_high)
                {
                    FrameID_high = pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].FrameID;
                }
                Count_IRT_Entries_PDIRFrameData++;

                // init "SndRcvRole" of this FrameID
                EDDP_PRMPDIRDataInitSndRcvRole (pDDB, (LSA_UINT16)i, LSA_FALSE);

                // calculate data for last local sent/received IRT frame (always after init "SndRcvRole")
                EDDP_PRMPDIRDataSetupLastLocalIRTFrame (pDDB, (LSA_UINT16)i, LSA_FALSE);

                // calculate the intermediate values for local TransferStartEnd (always after init "SndRcvRole")
                EDDP_PRMPDIRDataCalculateLocalTransferStartEndIntermediateValues (pDDB, (LSA_UINT16)i, LSA_FALSE);

                // is this FrameID redundant ?
                if (EDDP_IS_IRT_REDUNDANT_FRAMEID(pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].FrameID))
                {
                    // so check this FrameID
                    Response = EDDP_PRMPDIRDataCheckRedundantFrameID_B (pDDB, (LSA_UINT16)i);

                    if (Response != EDD_STS_OK)
                    {
                        i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;    // exit FOR loop
                    }

                    // is MRPD consumer with odd FrameID(x+1) present ?
                    if (   (EDDP_NUMBER_IS_ODD(pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].FrameID))
                        && (   (pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].SndRcvRole == EDDP_PRM_PDIRFRAMEDATA_ROLE_CONSUMER)
                            || (pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].SndRcvRole == EDDP_PRM_PDIRFRAMEDATA_ROLE_CONSUMER_FORWARDER)
                           )
                       )
                    {
                        // check if MRPD is supported
                        if(EDD_FEATURE_DISABLE == pDDB->FeatureSupport.bMRPDSupported)
                        {
                            Response = EDD_STS_ERR_PRM_BLOCK;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_Check_PDIRData(): ERROR -> FeatureSupport.bMRPDSupported is 0, Odd FrameID(0x%X) is not allowed!", 
                                            pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].FrameID);// exit FOR loop
                            i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                        }
                        else
                        {
                            // at least one MRPD consumer with odd FrameID(x+1) is present
                            pDDB->PRM.pPDIRData_B->MRPDConsumerWithOddFrameID_IsPresent = LSA_TRUE;
                        }
                    }
                }
            }
        }
        else
        {
            // not IRT FrameID range - if SyncFrameID it is no error
            if (pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].FrameID == EDD_SRT_FRAMEID_PTCP_SYNC_RT_CLASS3)
            {
                EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Check_PDIRData(): SyncFrameID(0x%X) is found in PDIRFrameData", 
                    pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].FrameID);
            }
            else
            {
                Response = EDD_STS_ERR_PRM_BLOCK;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_Check_PDIRData(): ERROR -> FrameID(0x%X) is out of IRT range!", 
                    pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].FrameID);
                // exit FOR loop
                i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
            }
        }
    }

    /* ---------------------------------------------------------------------------- */
    /* check "LocalTransferStartEnd.TxMinFSONs"                                     */
    /* this must always be bigger than "FSO_Leadtime" of PPM                        */
    /* ---------------------------------------------------------------------------- */
    if (Response == EDD_STS_OK)
    { 
        switch (EDDP_HALGetChipVersion(pDDB))
        {
#ifdef EDDP_CFG_HW_ERTEC200P_SUPPORT
            case EDDP_HAL_HW_ERTEC200P:
            {
                LSA_UINT32 FSO_Leadtime = EDDP_CRTGetFSOLeadtimeOrFetchtimeDelta(pDDB);
                
                // TxMinFSONs = 0: if no local provider are present in PDIRData
                if (   (pDDB->PRM.pPDIRData_B->LocalTransferStartEnd.TxMinFSONs > 0)
                    && (pDDB->PRM.pPDIRData_B->LocalTransferStartEnd.TxMinFSONs < FSO_Leadtime)
                   )
                {
                    Response = EDD_STS_ERR_PRM_BLOCK;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_Check_PDIRData(): ERROR -> TxMinFSONs(%d) < FSO_Leadtime(%d)!", 
                        pDDB->PRM.pPDIRData_B->LocalTransferStartEnd.TxMinFSONs, FSO_Leadtime);
                }
            }
            break;
#endif

            default:
            {
                EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_Check_PDIRData(): FATAL -> Version(%d) of chip is unknown!", 
                    EDDP_HALGetChipVersion(pDDB));
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            }
            break;
        }
    }

    /* ---------------------------------------------------------------------------- */
    /* check "Max_IRT_Entries"                                                      */
    /* ---------------------------------------------------------------------------- */
    if (Response == EDD_STS_OK)
    {
        if (pDDB->PRM.pPDIRData_B->AbsoluteTimeMode)
        {
            Max_IRT_Entries = EDDP_SWI_FDB_SIZE_OF_IRT_TABLE_A_32BIT;        // one entry = 4 byte

            if (Count_IRT_Entries_PDIRFrameData > Max_IRT_Entries)
            {
                // number of IRT Entries do not fit into IRT-Table
                Response = EDD_STS_ERR_PRM_BLOCK;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_Check_PDIRData(): ERROR -> Number-IRT-Entries(%d) > Max-IRT-Entries(%d) for AbsoluteTimeMode!", 
                    Count_IRT_Entries_PDIRFrameData, Max_IRT_Entries);
            }
        }
        else
        {
            Max_IRT_Entries = EDDP_SWI_FDB_SIZE_OF_IRT_TABLE_A_32BIT * 4;    // one entry = 1 byte

            Count_IRT_Entries_PDIRBeginEndData = (pDDB->PRM.pPDIRData_B->PDIRBeginEndData.EndOfRedFrameID - pDDB->PRM.pPDIRData_B->PDIRBeginEndData.StartOfRedFrameID) + 1;

            if (Count_IRT_Entries_PDIRBeginEndData > Max_IRT_Entries)
            {
                // number of IRT Entries do not fit into IRT-Table
                Response = EDD_STS_ERR_PRM_BLOCK;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_SWI_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_Check_PDIRData(): ERROR -> Number of IRT Entries (%d) in PDIRBeginEndData do not fit into IRT-Table for RelativeTimeMode! Max_IRT_Entries=%d, StartOfRedFrameID=0x%X, EndOfRedFrameID=0x%X", 
                    Count_IRT_Entries_PDIRBeginEndData, Max_IRT_Entries, 
                    pDDB->PRM.pPDIRData_B->PDIRBeginEndData.StartOfRedFrameID, 
                    pDDB->PRM.pPDIRData_B->PDIRBeginEndData.EndOfRedFrameID);
            }
        }
    }

    /* ---------------------------------------------------------------------------- */
    /* store FrameID data                                                           */
    /* ---------------------------------------------------------------------------- */
    if (Response == EDD_STS_OK)
    {
        // store data
        pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_Used    = Count_IRT_Entries_PDIRFrameData;
        pDDB->PRM.pPDIRData_B->FrameID_low                    = FrameID_low;
        pDDB->PRM.pPDIRData_B->FrameID_high                   = FrameID_high;

        // check if there is no 'Orange-Phase' defined in PDIRBeginEndData and determine maximum number of phases  
        for ( i=0; i < pDDB->PRM.pPDIRData_B->PDIRBeginEndData.NumberOfPorts; i++ )
        {   
            for ( j=0; j < pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].NumberOfAssignments; j++ )
            {        
                if (   (pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxOrangePeriodBegin != pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxGreenPeriodBegin) 
                    || (pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxOrangePeriodBegin != pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxGreenPeriodBegin)
                   )
                {
                    Response = EDD_STS_ERR_PRM_BLOCK;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_Check_PDIRData(): ERROR -> There is an Orange-Phase within PDIRBeginEndData! Not supported by EDDP!");
                }     
            }

            for ( k=1; k <= EDDP_PRM_MAX_PHASES; k = k << 1 )
            {
                if (pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].NumberOfPhases == k)
                {
                    break;  // valid NumberOfPhases found
                }
            }

            if (k > EDDP_PRM_MAX_PHASES)
            {
                Response = EDD_STS_ERR_PRM_BLOCK;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_Check_PDIRData(): ERROR -> NumberOfPhases is not a value of 2^n!");
            }

            if (pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].NumberOfPhases > pDDB->PRM.pPDIRData_B->PDIRBeginEndData.NumberOfPhases_max)
            {
                pDDB->PRM.pPDIRData_B->PDIRBeginEndData.NumberOfPhases_max = 
                    pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].NumberOfPhases;
            }
        }
    }

    /* ---------------------------------------------------------------------------- */
    /* checks for "FragmentationMode" and "YellowTime"                              */
    /* ---------------------------------------------------------------------------- */
    if (Response == EDD_STS_OK)
    {
        LSA_UINT32      FragmentationMode   = EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_OFF;
        LSA_UINT32      SendClockNs         = (pDDB->SWI.CycleInfo.CycleBaseFactor * EDDP_CRT_CYCLE_LENGTH_BASE_NS);

        // is "FrameDataProperties" present in PDIRData ?
        if ( EDDP_PRMPDIRDataIsFrameDataPropertiesPresent(pDDB, LSA_FALSE) )
        {
            // get parameter PDIRData.FrameDataProperties.FragmentationMode
            FragmentationMode = EDDP_PRMPDIRDataGetFrameDataProperties (pDDB, EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_SHIFT, LSA_FALSE);
        }

        // loop through all ports
        for ( i=0; i < pDDB->PRM.pPDIRData_B->PDIRGlobalData.NumberOfPorts; i++ )
        {
            LSA_UINT32      MinFragTime = 0;
            LSA_UINT32      MaxRxTxGreenPeriodBegin = 0;    // maximum value of GreenPeriodBegin in all Tx/Rx
                                                            // MinGreen = SendClockNs - MaxRxTxGreenPeriodBegin
            // calculate MaxRxTxGreenPeriodBegin
            for ( j=0; j < pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].NumberOfAssignments; j++ )
            {
                // Rx
                if (pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxGreenPeriodBegin > MaxRxTxGreenPeriodBegin)
                {
                    MaxRxTxGreenPeriodBegin = pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxGreenPeriodBegin;
                }
                // Tx
                if (pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxGreenPeriodBegin > MaxRxTxGreenPeriodBegin)
                {
                    MaxRxTxGreenPeriodBegin = pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxGreenPeriodBegin;
                }
            }
            EDDP_ASSERT_FALSE(MaxRxTxGreenPeriodBegin > SendClockNs);

            // is YellowTime present in PDIRGlobalData ?
            if (pDDB->PRM.pPDIRData_B->PDIRGlobalData.DelayValues[i].YellowTime_IsPresent)
            {
                // checks with PDIRGlobalData.YellowTime
                // check for: FragmentationMode(OFF) and YellowTime < 125000
                if (   (FragmentationMode == EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_OFF)
                    && (pDDB->PRM.pPDIRData_B->PDIRGlobalData.DelayValues[i].YellowTime < EDDP_PRM_PDIRGLOBALDATA_YELLOWTIME_MAX)
                   )
                {
                    Response = EDD_STS_ERR_PRM_BLOCK;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_Check_PDIRData(): ERROR -> FragmentationMode(OFF) and YellowTime(%d) < %d! Port(%d)", 
                        pDDB->PRM.pPDIRData_B->PDIRGlobalData.DelayValues[i].YellowTime, EDDP_PRM_PDIRGLOBALDATA_YELLOWTIME_MAX, i+1);
                    // exit FOR loop "NumberOfPorts"
                    i = pDDB->PRM.pPDIRData_B->PDIRGlobalData.NumberOfPorts;
                }
                else
                {
                    // check for: minimum of green phase < YellowTime
                    if ((SendClockNs - MaxRxTxGreenPeriodBegin) < pDDB->PRM.pPDIRData_B->PDIRGlobalData.DelayValues[i].YellowTime)
                    {
                        Response = EDD_STS_ERR_PRM_BLOCK;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_Check_PDIRData(): ERROR -> MinGreen(%d) < YellowTime(%d)! Port(%d)", 
                            (SendClockNs - MaxRxTxGreenPeriodBegin), pDDB->PRM.pPDIRData_B->PDIRGlobalData.DelayValues[i].YellowTime, i+1);
                        // exit FOR loop "NumberOfPorts"
                        i = pDDB->PRM.pPDIRData_B->PDIRGlobalData.NumberOfPorts;
                    }
                }
            }

            if (Response == EDD_STS_OK)
            {
                // check for: FragmentationMode(OFF) and minimum of green phase < 125000
                if (   (FragmentationMode == EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_OFF)
                    && ((SendClockNs - MaxRxTxGreenPeriodBegin) < EDDP_PRM_PDIRGLOBALDATA_YELLOWTIME_MAX)
                    )
                {
                    Response = EDD_STS_ERR_PRM_BLOCK;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_Check_PDIRData(): ERROR -> FragmentationMode(OFF) and MinGreen(%d) < %d! Port(%d)", 
                        (SendClockNs - MaxRxTxGreenPeriodBegin), EDDP_PRM_PDIRGLOBALDATA_YELLOWTIME_MAX, i+1);
                    // exit FOR loop "NumberOfPorts"
                    i = pDDB->PRM.pPDIRData_B->PDIRGlobalData.NumberOfPorts;
                }
            }

            // checks for: minimum of green phase against MinFragTime
            #ifdef EDDP_MESSAGE
            #ifdef EDDP_PRAGMA_MESSAGE
            #pragma EDDP_PRAGMA_MESSAGE("MinFragTime is depending PNIP revision")
            #endif /* EDDP_PRAGMA_MESSAGE */
            #endif /* EDDP_MESSAGE */
            if (Response == EDD_STS_OK)
            {
                // init MinFragTime
                switch (pDDB->Hardware)
                {
                    case EDDP_HW_ERTEC200P:
                    {
                        // switch to current PNIP revision
                        switch (EDDP_HALGetPnipRevision(pDDB))
                        {
                            #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
                            case EDDP_HAL_HW_PNIP_REV1:
                            {
                                MinFragTime = EDDP_PRM_PDIRDATA_MIN_FRAG_TIME_ERTEC200P_PNIP_REV1;
                            }
                            break;
                            #endif

                            #ifdef EDDP_CFG_HW_PNIP_REV2_SUPPORT
                            case EDDP_HAL_HW_PNIP_REV2:
                            {
                                MinFragTime = EDDP_PRM_PDIRDATA_MIN_FRAG_TIME_ERTEC200P_PNIP_REV2;
                            }
                            break;
                            #endif

                            default:
                            {
                                EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_Check_PDIRData(): FATAL -> Version(%d) of PNIP is unknown", 
                                    EDDP_HALGetPnipRevision(pDDB));
                                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                            }
                            break;
                        }
                    }
                    break;

                    case EDDP_HW_ERTEC200P_FPGA:
                    {
                        // switch to current PNIP revision
                        switch (EDDP_HALGetPnipRevision(pDDB))
                        {
                            #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
                            case EDDP_HAL_HW_PNIP_REV1:
                            {
                                MinFragTime = EDDP_PRM_PDIRDATA_MIN_FRAG_TIME_FPGA_PNIP_REV1;
                            }
                            break;
                            #endif

                            #ifdef EDDP_CFG_HW_PNIP_REV2_SUPPORT
                            case EDDP_HAL_HW_PNIP_REV2:
                            {
                                MinFragTime = EDDP_PRM_PDIRDATA_MIN_FRAG_TIME_FPGA_PNIP_REV2;
                            }
                            break;
                            #endif

                            default:
                            {
                                EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_Check_PDIRData(): FATAL -> Version(%d) of PNIP is unknown", 
                                    EDDP_HALGetPnipRevision(pDDB));
                                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                            }
                            break;
                        }
                    }
                    break;

                    default:
                    {
                        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_Check_PDIRData(): FATAL -> Hardware(0x%X) is not supported!", 
                            pDDB->Hardware);
                        EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                    }
                    break;
                }

                // check minimum of green phase against MinFragTime
                if ((SendClockNs - MaxRxTxGreenPeriodBegin) < MinFragTime)
                {
                    Response = EDD_STS_ERR_PRM_BLOCK;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_Check_PDIRData(): ERROR -> MinGreen(%d) < MinFragTime(%d)! Port(%d)", 
                        (SendClockNs - MaxRxTxGreenPeriodBegin), MinFragTime, i+1);
                    // exit FOR loop "NumberOfPorts"
                    i = pDDB->PRM.pPDIRData_B->PDIRGlobalData.NumberOfPorts;
                }
            }
        }
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_Reset_RsvInterval_Red_Orange_Param +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_UINT32                  PortID          +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                 +*/
/*+  PortID             : User PortID                                       +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function resets the RsvIntervalRed and               +*/
/*+               RsvIntervalOrange parameters of given port.               +*/
/*+                                                                         +*/
/*+               Valid PortID:                                             +*/
/*+                 - 0             : interface                             +*/
/*+                 - 1..PortCnt    : port                                  +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Reset_RsvInterval_Red_Orange_Param(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  PortID)
{
    // only if given PortID is in valid range
    if (/*(PortID >= 0) &&*/ (PortID <= pDDB->SWI.PortCnt))
    {
        // RsvIntervalRed.Rx
        pDDB->SWI.LinkStatus[PortID].RsvIntervalRed.Rx.BeginNs              = 0;
        pDDB->SWI.LinkStatus[PortID].RsvIntervalRed.Rx.EndNs                = 0;
        pDDB->SWI.LinkStatus[PortID].RsvIntervalRed.Rx.EndLocalNs           = 0;
        pDDB->SWI.LinkStatus[PortID].RsvIntervalRed.Rx.LastLocalFrameLen    = 0;
        // RsvIntervalRed.Tx
        pDDB->SWI.LinkStatus[PortID].RsvIntervalRed.Tx.BeginNs              = 0;
        pDDB->SWI.LinkStatus[PortID].RsvIntervalRed.Tx.EndNs                = 0;
        pDDB->SWI.LinkStatus[PortID].RsvIntervalRed.Tx.EndLocalNs           = 0;
        pDDB->SWI.LinkStatus[PortID].RsvIntervalRed.Tx.LastLocalFrameLen    = 0;

        // RsvIntervalOrange.Rx
        pDDB->SWI.LinkStatus[PortID].RsvIntervalOrange.Rx.BeginNs           = 0;
        pDDB->SWI.LinkStatus[PortID].RsvIntervalOrange.Rx.EndNs             = 0;    
        // RsvIntervalOrange.Tx
        pDDB->SWI.LinkStatus[PortID].RsvIntervalOrange.Tx.BeginNs           = 0;
        pDDB->SWI.LinkStatus[PortID].RsvIntervalOrange.Tx.EndNs             = 0;
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_Setup_RTC3PSMNewParamPresentMask   +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_UINT32                  PortID          +*/
/*+                             LSA_BOOL                    isSet           +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                 +*/
/*+  PortID             : User PortID                                       +*/
/*+  isSet              : Set or Delete                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Set/Delete the bit in "isRTC3PSMNewParamPresentMask"      +*/
/*+               for given PortID.                                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Setup_RTC3PSMNewParamPresentMask(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  PortID,
    LSA_BOOL                    isSet)
{
    EDDP_ASSERT_FALSE(EDDP_HAL_HW_PNIP_REV1 != EDDP_HALGetPnipRevision(pDDB));

    EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_SWI_Setup_RTC3PSMNewParamPresentMask(PortID=%d, isSet=0x%X)", 
        PortID, isSet);

    // only if given PortID is in valid range
    if ((PortID >= 1) && (PortID <= pDDB->SWI.PortCnt))
    {
        switch (PortID)
        {
            // PortID = 1
            case 1:
            {
                if (isSet)
                {
                    EDDP_SET_BIT_VALUE32(pDDB->SWI.isRTC3PSMNewParamPresentMask, 1, EDDP_SWI_RTC3PSM_ASYN_PORT1_MASK, EDDP_SWI_RTC3PSM_ASYN_PORT1_SHIFT);
                }
                else
                {
                    EDDP_SET_BIT_VALUE32(pDDB->SWI.isRTC3PSMNewParamPresentMask, 0, EDDP_SWI_RTC3PSM_ASYN_PORT1_MASK, EDDP_SWI_RTC3PSM_ASYN_PORT1_SHIFT);
                }
            }
            break;

            // PortID = 2
            case 2:
            {
                if (isSet)
                {
                    EDDP_SET_BIT_VALUE32(pDDB->SWI.isRTC3PSMNewParamPresentMask, 1, EDDP_SWI_RTC3PSM_ASYN_PORT2_MASK, EDDP_SWI_RTC3PSM_ASYN_PORT2_SHIFT);
                }
                else
                {
                    EDDP_SET_BIT_VALUE32(pDDB->SWI.isRTC3PSMNewParamPresentMask, 0, EDDP_SWI_RTC3PSM_ASYN_PORT2_MASK, EDDP_SWI_RTC3PSM_ASYN_PORT2_SHIFT);
                }
            }
            break;

            default:
            {
                EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_Setup_RTC3PSMNewParamPresentMask(): FATAL -> PortID(%d) is invalid!", 
                    PortID);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }
}
#endif





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_IsSet_RTC3PSMNewParamPresentMask   +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_UINT32                  PortID          +*/
/*+                                                                         +*/
/*+  Result                :    LSA_BOOL                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                 +*/
/*+  PortID             : User PortID                                       +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Is bit in "isRTC3PSMNewParamPresentMask"                  +*/
/*+               set for given PortID?                                     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_SWI_IsSet_RTC3PSMNewParamPresentMask(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  PortID)
{
    LSA_BOOL        isSet       = LSA_FALSE;
	LSA_UINT16      ValueTemp   = 0;

    EDDP_ASSERT_FALSE(EDDP_HAL_HW_PNIP_REV1 != EDDP_HALGetPnipRevision(pDDB));

    // only if given PortID is in valid range
    if ((PortID >= 1) && (PortID <= pDDB->SWI.PortCnt))
    {
        switch (PortID)
        {
            // PortID = 1
            case 1:
            {
                ValueTemp = EDDP_GET_BIT_VALUE(pDDB->SWI.isRTC3PSMNewParamPresentMask, EDDP_SWI_RTC3PSM_ASYN_PORT1_MASK, EDDP_SWI_RTC3PSM_ASYN_PORT1_SHIFT);
            }
            break;

            // PortID = 2
            case 2:
            {
                ValueTemp = EDDP_GET_BIT_VALUE(pDDB->SWI.isRTC3PSMNewParamPresentMask, EDDP_SWI_RTC3PSM_ASYN_PORT2_MASK, EDDP_SWI_RTC3PSM_ASYN_PORT2_SHIFT);
            }
            break;

            default:
            {
                EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_IsSet_RTC3PSMNewParamPresentMask(): FATAL -> PortID(%d) is invalid!", 
                    PortID);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }

    if (ValueTemp == 1)
        isSet = LSA_TRUE;

    return (isSet);
}
#endif





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_Setup_RTC3PSMRunningAsynMask       +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_UINT32                  PortID          +*/
/*+                             LSA_BOOL                    isSet           +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                 +*/
/*+  PortID             : User PortID                                       +*/
/*+  isSet              : Set or Delete                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Set/Delete the bit in "isRTC3PSMRunningAsynMask"          +*/
/*+               for given PortID.                                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Setup_RTC3PSMRunningAsynMask(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  PortID,
    LSA_BOOL                    isSet)
{
    EDDP_ASSERT_FALSE(EDDP_HAL_HW_PNIP_REV1 != EDDP_HALGetPnipRevision(pDDB));

    EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_SWI_Setup_RTC3PSMRunningAsynMask(PortID=%d, isSet=0x%X)", 
        PortID, isSet);

    // only if given PortID is in valid range
    if ((PortID >= 1) && (PortID <= pDDB->SWI.PortCnt))
    {
        switch (PortID)
        {
            // PortID = 1
            case 1:
            {
                if (isSet)
                {
                    EDDP_SET_BIT_VALUE32(pDDB->SWI.isRTC3PSMRunningAsynMask, 1, EDDP_SWI_RTC3PSM_ASYN_PORT1_MASK, EDDP_SWI_RTC3PSM_ASYN_PORT1_SHIFT);
                }
                else
                {
                    EDDP_SET_BIT_VALUE32(pDDB->SWI.isRTC3PSMRunningAsynMask, 0, EDDP_SWI_RTC3PSM_ASYN_PORT1_MASK, EDDP_SWI_RTC3PSM_ASYN_PORT1_SHIFT);
                }
            }
            break;

            // PortID = 2
            case 2:
            {
                if (isSet)
                {
                    EDDP_SET_BIT_VALUE32(pDDB->SWI.isRTC3PSMRunningAsynMask, 1, EDDP_SWI_RTC3PSM_ASYN_PORT2_MASK, EDDP_SWI_RTC3PSM_ASYN_PORT2_SHIFT);
                }
                else
                {
                    EDDP_SET_BIT_VALUE32(pDDB->SWI.isRTC3PSMRunningAsynMask, 0, EDDP_SWI_RTC3PSM_ASYN_PORT2_MASK, EDDP_SWI_RTC3PSM_ASYN_PORT2_SHIFT);
                }
            }
            break;

            default:
            {
                EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_Setup_RTC3PSMRunningAsynMask(): FATAL -> PortID(%d) is invalid!", 
                    PortID);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }
}
#endif





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_IsSet_RTC3PSMRunningAsynMask       +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_UINT32                  PortID          +*/
/*+                                                                         +*/
/*+  Result                :    LSA_BOOL                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                 +*/
/*+  PortID             : User PortID                                       +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Is bit in "isRTC3PSMRunningAsynMask"                      +*/
/*+               set for given PortID?                                     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_SWI_IsSet_RTC3PSMRunningAsynMask(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  PortID)
{
    LSA_BOOL        isSet       = LSA_FALSE;
	LSA_UINT16      ValueTemp   = 0;

    EDDP_ASSERT_FALSE(EDDP_HAL_HW_PNIP_REV1 != EDDP_HALGetPnipRevision(pDDB));

    // only if given PortID is in valid range
    if ((PortID >= 1) && (PortID <= pDDB->SWI.PortCnt))
    {
        switch (PortID)
        {
            // PortID = 1
            case 1:
            {
                ValueTemp = EDDP_GET_BIT_VALUE(pDDB->SWI.isRTC3PSMRunningAsynMask, EDDP_SWI_RTC3PSM_ASYN_PORT1_MASK, EDDP_SWI_RTC3PSM_ASYN_PORT1_SHIFT);
            }
            break;

            // PortID = 2
            case 2:
            {
                ValueTemp = EDDP_GET_BIT_VALUE(pDDB->SWI.isRTC3PSMRunningAsynMask, EDDP_SWI_RTC3PSM_ASYN_PORT2_MASK, EDDP_SWI_RTC3PSM_ASYN_PORT2_SHIFT);
            }
            break;

            default:
            {
                EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_IsSet_RTC3PSMRunningAsynMask(): FATAL -> PortID(%d) is invalid!", 
                    PortID);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }

    if (ValueTemp == 1)
        isSet = LSA_TRUE;

    return (isSet);
}
#endif





#ifdef EDDP_PRM_TEST_RECORD_ON
/* ------------------------------------------------------------------------ */
/* These are just the test functions (for the internal use only)            */
/* ------------------------------------------------------------------------ */

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_TEST_Enable_Only_Red_Phase         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function sets enable the RED phase only              +*/
/*+               on given HwPortID.                                        +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_SWI_TEST_Enable_Only_Red_Phase(
    EDDP_LOCAL_DDB_PTR_TYPE      pDDB,
    LSA_UINT32                   HwPortID)
{
    LSA_UINT32              reg_val, reg_address;
    LSA_UINT32              phase_entry;    
    EDD_COMMON_MEM_PTR_TYPE time_list;
    EDD_COMMON_MEM_PTR_TYPE phase_list_address_tx;    
    EDD_COMMON_MEM_PTR_TYPE phase_list_address_rx;
    LSA_UINT32              phase_list_tx_ram_index;
    LSA_UINT32              phase_list_rx_ram_index;         

    phase_list_tx_ram_index = (HwPortID==1 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P1 : 
                              (HwPortID==2 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P2 : 
                              (HwPortID==3 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P3 : 
                              (HwPortID==4 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P4 : 0))));
    phase_list_rx_ram_index = (HwPortID==1 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P1 : 
                              (HwPortID==2 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P2 : 
                              (HwPortID==3 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P3 : 
                              (HwPortID==4 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P4 : 0))));

    phase_list_address_tx = EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_SYNC, phase_list_tx_ram_index, 0);
    phase_list_address_rx = EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_SYNC, phase_list_rx_ram_index, 0);   

    time_list = EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_SYNC, EDDP_HAL_RAMSUB_SYNC_TIME_LIST, 0); 

    // set first time list entry to "0" (start of RED)
    EDDP_HAL_MEM32_WRITE(pDDB, &((((PNIP_SWI_RAMSUB_SYNC_TIME_LIST_ENTRY_PTR_TYPE)time_list)[0]).TimeEntry), 0/*start of RED*/);

    // write phase list entry (Tx)        
    phase_entry = EDDP_SWI_SET_PHASE_ENTRY_RED(LSA_TRUE, 0) | EDDP_SWI_SET_PHASE_ENTRY_ORANGE(LSA_FALSE, 0) | EDDP_SWI_SET_PHASE_ENTRY_GREEN(LSA_FALSE, 0);
    EDDP_HAL_MEM32_WRITE(pDDB, phase_list_address_tx, phase_entry);

    // write phase list entry (Rx)            
    phase_entry = EDDP_SWI_SET_PHASE_ENTRY_RED(LSA_TRUE, 0) | EDDP_SWI_SET_PHASE_ENTRY_ORANGE(LSA_FALSE, 0) | EDDP_SWI_SET_PHASE_ENTRY_GREEN(LSA_FALSE, 0);
    EDDP_HAL_MEM32_WRITE(pDDB, phase_list_address_rx, phase_entry);    

    // set PhaseRange_X register to 0 /* := 1 phase entry*/
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_PHASERANGE_A 0);

    // enable phase control
    reg_address = (HwPortID==1 ? PNIP_REG_PHASEENABLE_P1 : 
                  (HwPortID==2 ? PNIP_REG_PHASEENABLE_P2 : 
                  (HwPortID==3 ? PNIP_REG_PHASEENABLE_P3 : 
                  (HwPortID==4 ? PNIP_REG_PHASEENABLE_P4 : 0))));
    EDDP_HAL_REG32_WRITE(pDDB, reg_address, EDDP_SWI_PHASE_ENABLE_TX_RED | EDDP_SWI_PHASE_ENABLE_RX_RED);                

    // enable RxBoundary_DP2 in register FlowControl_Px
    // enable TxBoundary_DP2 in register FlowControl_Px
    reg_address = (HwPortID==1 ? PNIP_REG_FLOWCONTROL_P1 : 
                  (HwPortID==2 ? PNIP_REG_FLOWCONTROL_P2 : 
                  (HwPortID==3 ? PNIP_REG_FLOWCONTROL_P3 : 
                  (HwPortID==4 ? PNIP_REG_FLOWCONTROL_P4 : 0))));
    reg_val = EDDP_HAL_REG32_READ(pDDB, reg_address);
    EDDP_SET_BIT_VALUE32(reg_val, 0x00000000, PNIP_REG_FLOWCONTROL_P1__MSK_RXBOUNDARY_DP2, PNIP_REG_FLOWCONTROL_P1_SHFT_RXBOUNDARY_DP2);
    EDDP_SET_BIT_VALUE32(reg_val, 0x00000000, PNIP_REG_FLOWCONTROL_P1__MSK_TXBOUNDARY_DP2, PNIP_REG_FLOWCONTROL_P1_SHFT_TXBOUNDARY_DP2);
    EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_val);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_TEST_Enable_Red_and_Green_Phase    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function sets enable the RED and GREEN phase         +*/
/*+               on given HwPortID.                                        +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_SWI_TEST_Enable_Red_and_Green_Phase(
    EDDP_LOCAL_DDB_PTR_TYPE      pDDB,
    LSA_UINT32                   HwPortID)
{
    LSA_UINT32              reg_val, reg_address;
    LSA_UINT32              phase_entry;    
    EDD_COMMON_MEM_PTR_TYPE time_list;
    EDD_COMMON_MEM_PTR_TYPE phase_list_address_tx;    
    EDD_COMMON_MEM_PTR_TYPE phase_list_address_rx;
    LSA_UINT32              phase_list_tx_ram_index;
    LSA_UINT32              phase_list_rx_ram_index;         

    phase_list_tx_ram_index = (HwPortID==1 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P1 : 
                              (HwPortID==2 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P2 : 
                              (HwPortID==3 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P3 : 
                              (HwPortID==4 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P4 : 0))));
    phase_list_rx_ram_index = (HwPortID==1 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P1 : 
                              (HwPortID==2 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P2 : 
                              (HwPortID==3 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P3 : 
                              (HwPortID==4 ? EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P4 : 0))));

    phase_list_address_tx = EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_SYNC, phase_list_tx_ram_index, 0);
    phase_list_address_rx = EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_SYNC, phase_list_rx_ram_index, 0);   

    time_list = EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_SYNC, EDDP_HAL_RAMSUB_SYNC_TIME_LIST, 0); 

    // set first time list entry to "0" (start of RED)
    EDDP_HAL_MEM32_WRITE(pDDB, &((((PNIP_SWI_RAMSUB_SYNC_TIME_LIST_ENTRY_PTR_TYPE)time_list)[0]).TimeEntry), 0);

    // set first time list entry to "500.000ns" (start of GREEN)
    EDDP_HAL_MEM32_WRITE(pDDB, &((((PNIP_SWI_RAMSUB_SYNC_TIME_LIST_ENTRY_PTR_TYPE)time_list)[2]).TimeEntry), 500000);

    // write phase list entry (Tx)
    phase_entry = EDDP_SWI_SET_PHASE_ENTRY_RED(LSA_TRUE, 0) | EDDP_SWI_SET_PHASE_ENTRY_ORANGE(LSA_FALSE, 0) | EDDP_SWI_SET_PHASE_ENTRY_GREEN(LSA_TRUE, 1);
    EDDP_HAL_MEM32_WRITE(pDDB, phase_list_address_tx, phase_entry);

    // write phase list entry (Rx)
    phase_entry = EDDP_SWI_SET_PHASE_ENTRY_RED(LSA_TRUE, 0) | EDDP_SWI_SET_PHASE_ENTRY_ORANGE(LSA_FALSE, 0) | EDDP_SWI_SET_PHASE_ENTRY_GREEN(LSA_TRUE, 1);
    EDDP_HAL_MEM32_WRITE(pDDB, phase_list_address_rx, phase_entry);    

    // set PhaseRange_X register to 0 /* := 1 phase entry*/
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_PHASERANGE_A, 0);

    // enable phase control
    reg_address = (HwPortID==1 ? PNIP_REG_PHASEENABLE_P1 : 
                  (HwPortID==2 ? PNIP_REG_PHASEENABLE_P2 : 
                  (HwPortID==3 ? PNIP_REG_PHASEENABLE_P3 : 
                  (HwPortID==4 ? PNIP_REG_PHASEENABLE_P4 : 0))));
    EDDP_HAL_REG32_WRITE(pDDB, reg_address, EDDP_SWI_PHASE_ENABLE_TX_RED | EDDP_SWI_PHASE_ENABLE_RX_RED);                

    // enable RxBoundary_DP2 in register FlowControl_Px
    // enable TxBoundary_DP2 in register FlowControl_Px
    reg_address = (HwPortID==1 ? PNIP_REG_FLOWCONTROL_P1 : 
                  (HwPortID==2 ? PNIP_REG_FLOWCONTROL_P2 : 
                  (HwPortID==3 ? PNIP_REG_FLOWCONTROL_P3 : 
                  (HwPortID==4 ? PNIP_REG_FLOWCONTROL_P4 : 0))));
    reg_val = EDDP_HAL_REG32_READ(pDDB, reg_address);
    EDDP_SET_BIT_VALUE32(reg_val, 0x00000000, PNIP_REG_FLOWCONTROL_P1__MSK_RXBOUNDARY_DP2, PNIP_REG_FLOWCONTROL_P1_SHFT_RXBOUNDARY_DP2);
    EDDP_SET_BIT_VALUE32(reg_val, 0x00000000, PNIP_REG_FLOWCONTROL_P1__MSK_TXBOUNDARY_DP2, PNIP_REG_FLOWCONTROL_P1_SHFT_TXBOUNDARY_DP2);
    EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_val);
}

#endif

/*****************************************************************************/
/*  end of file eddp_swi_irt.c                                               */
/*****************************************************************************/
