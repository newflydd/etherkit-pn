/** @file eddp_prm.c 
 */
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
/*  F i l e               &F: eddp_prm.c                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP PRM handling                                */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  04.12.08    js    initial version.                                       */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID   91                 
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_PRM */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"            /* edd headerfiles */
#include "eddp_int.h"            /* internal header */
#include "eddp_phy_nec.h"        /* is used by function EDDP_PRMRequestPrmChangePort */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)

#ifdef EDDP_CFG_LOCAL_MEM_STATIC
    EDDP_LOCAL_MEM_ATTR     EDDP_HDB_PRM_TYPE       g_EDDP_HDB_PRM[EDDP_CFG_MAX_CHANNELS];
#endif


/*===========================================================================*/
/*                                defines                                    */
/*===========================================================================*/



/*===========================================================================*/
/*                                global Variables                           */
/*===========================================================================*/
EDD_MAC_ADR_TYPE  EDDP_MAC_DomainBoundary_1 = {{ 0x01, 0x0E, 0xCF, 0x00, 0x04, 0x00 }};   // DomainBoundaryAnnounce
EDD_MAC_ADR_TYPE  EDDP_MAC_DomainBoundary_2 = {{ 0x01, 0x0E, 0xCF, 0x00, 0x04, 0x20 }};   // DomainBoundaryIngress, DomainBoundaryEgress
EDD_MAC_ADR_TYPE  EDDP_MAC_DomainBoundary_3 = {{ 0x01, 0x0E, 0xCF, 0x00, 0x04, 0x40 }};   // DomainBoundaryIngress, DomainBoundaryEgress
EDD_MAC_ADR_TYPE  EDDP_MAC_DomainBoundary_4 = {{ 0x01, 0x0E, 0xCF, 0x00, 0x04, 0x80 }};   // DomainBoundaryIngress, DomainBoundaryEgress

EDD_MAC_ADR_TYPE  EDDP_MAC_DCPBoundary_Identify = {{ 0x01, 0x0E, 0xCF, 0x00, 0x00, 0x00 }};
EDD_MAC_ADR_TYPE  EDDP_MAC_DCPBoundary_Hello    = {{ 0x01, 0x0E, 0xCF, 0x00, 0x00, 0x01 }};

/*===========================================================================*/
/*                                external functions                         */
/*===========================================================================*/

/*===========================================================================*/
/*                                 functions                                 */
/*===========================================================================*/





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMFreeLocalMem                        +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB           +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB        : Pointer to Interface Description Block                   +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function frees memory within PRM structure. It checks+*/
/*+              if there is memory to free so it can be called at any time.+*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMFreeLocalMem(
    EDDP_LOCAL_DDB_PTR_TYPE      pDDB)
{
    LSA_UINT32      i;

    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMFreeLocalMem()");

    if (!(LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetA->pPDNCDataCheck, LSA_NULL)))
    {    
        EDDP_GlbFreeLocalMem (pDDB->PRM.pRecordsetA->pPDNCDataCheck);
        pDDB->PRM.pRecordsetA->pPDNCDataCheck       = LSA_NULL;
        pDDB->PRM.pRecordsetA->PDNCDataCheck_Size   = 0;
    }
    if (!(LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetB->pPDNCDataCheck, LSA_NULL)))
    {    
        EDDP_GlbFreeLocalMem (pDDB->PRM.pRecordsetB->pPDNCDataCheck);
        pDDB->PRM.pRecordsetB->pPDNCDataCheck       = LSA_NULL;
        pDDB->PRM.pRecordsetB->PDNCDataCheck_Size   = 0;
    }

    if (!(LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.PDSCFDataCheck.pPDSCFDataCheck, LSA_NULL)))
    {
        EDDP_GlbFreeLocalMem (pDDB->PRM.PDSCFDataCheck.pPDSCFDataCheck);
        pDDB->PRM.PDSCFDataCheck.pPDSCFDataCheck        = LSA_NULL;
        pDDB->PRM.PDSCFDataCheck.PDSCFDataCheck_Size    = 0;
    }

    if (!(LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pPDIRData_A->pIRFrameDataArray, LSA_NULL)))
    {    
        EDDP_GlbFreeLocalMem (pDDB->PRM.pPDIRData_A->pIRFrameDataArray);
        pDDB->PRM.pPDIRData_A->pIRFrameDataArray            = LSA_NULL;
        pDDB->PRM.pPDIRData_A->IRFrameDataArray_Max_Entries = 0;
        pDDB->PRM.pPDIRData_A->Sync_IsPresent               = LSA_FALSE;
    }
    if (!(LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pPDIRData_B->pIRFrameDataArray, LSA_NULL)))
    {    
        EDDP_GlbFreeLocalMem (pDDB->PRM.pPDIRData_B->pIRFrameDataArray);
        pDDB->PRM.pPDIRData_B->pIRFrameDataArray            = LSA_NULL;
        pDDB->PRM.pPDIRData_B->IRFrameDataArray_Max_Entries = 0;
        pDDB->PRM.pPDIRData_B->Sync_IsPresent               = LSA_FALSE;
    }	

    if (!(LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetA->pPDIRData, LSA_NULL)))
    {
        EDDP_GlbFreeLocalMem (pDDB->PRM.pRecordsetA->pPDIRData);
        pDDB->PRM.pRecordsetA->pPDIRData        = LSA_NULL;
        pDDB->PRM.pRecordsetA->PDIRData_Size    = 0;
    }
    if (!(LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetB->pPDIRData, LSA_NULL)))
    {	
        EDDP_GlbFreeLocalMem (pDDB->PRM.pRecordsetB->pPDIRData);
        pDDB->PRM.pRecordsetB->pPDIRData        = LSA_NULL;
        pDDB->PRM.pRecordsetB->PDIRData_Size    = 0;
    }

    if (!(LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetA->pPDIRApplicationData, LSA_NULL)))
    {
        EDDP_GlbFreeLocalMem (pDDB->PRM.pRecordsetA->pPDIRApplicationData);
        pDDB->PRM.pRecordsetA->pPDIRApplicationData     = LSA_NULL;
        pDDB->PRM.pRecordsetA->PDIRApplicationData_Size = 0;
    }
    if (!(LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetB->pPDIRApplicationData, LSA_NULL)))
    {	
        EDDP_GlbFreeLocalMem (pDDB->PRM.pRecordsetB->pPDIRApplicationData);
        pDDB->PRM.pRecordsetB->pPDIRApplicationData     = LSA_NULL;
        pDDB->PRM.pRecordsetB->PDIRApplicationData_Size = 0;
    }

    if (!(LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetA->pPDIRSubframeData, LSA_NULL)))
    {
        EDDP_GlbFreeLocalMem (pDDB->PRM.pRecordsetA->pPDIRSubframeData);
        pDDB->PRM.pRecordsetA->pPDIRSubframeData        = LSA_NULL;
        pDDB->PRM.pRecordsetA->PDIRSubframeData_Size    = 0;
    }
    if (!(LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetB->pPDIRSubframeData, LSA_NULL)))
    {	
        EDDP_GlbFreeLocalMem (pDDB->PRM.pRecordsetB->pPDIRSubframeData);
        pDDB->PRM.pRecordsetB->pPDIRSubframeData        = LSA_NULL;
        pDDB->PRM.pRecordsetB->PDIRSubframeData_Size    = 0;
    }

    if (!(LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetA->pPDSyncData, LSA_NULL)))
    {
        EDDP_GlbFreeLocalMem (pDDB->PRM.pRecordsetA->pPDSyncData);
        pDDB->PRM.pRecordsetA->pPDSyncData      = LSA_NULL;
        pDDB->PRM.pRecordsetA->PDSyncData_Size  = 0;
                                    
    }
    if (!(LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetB->pPDSyncData, LSA_NULL)))
    {	
        EDDP_GlbFreeLocalMem (pDDB->PRM.pRecordsetB->pPDSyncData);
        pDDB->PRM.pRecordsetB->pPDSyncData      = LSA_NULL;
        pDDB->PRM.pRecordsetB->PDSyncData_Size  = 0;
    }		

    if (!(LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetA->pPDInterfaceMrpDataAdjust, LSA_NULL)))
    {	
        EDDP_GlbFreeLocalMem (pDDB->PRM.pRecordsetA->pPDInterfaceMrpDataAdjust);
        pDDB->PRM.pRecordsetA->pPDInterfaceMrpDataAdjust        = LSA_NULL;
        pDDB->PRM.pRecordsetA->PDInterfaceMrpDataAdjust_Size    = 0;
    }	
    if (!(LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetB->pPDInterfaceMrpDataAdjust, LSA_NULL)))
    {	
        EDDP_GlbFreeLocalMem (pDDB->PRM.pRecordsetB->pPDInterfaceMrpDataAdjust);
        pDDB->PRM.pRecordsetB->pPDInterfaceMrpDataAdjust        = LSA_NULL;
        pDDB->PRM.pRecordsetB->PDInterfaceMrpDataAdjust_Size    = 0;
    }			

    if (!(LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetA->pPDNRTFeedInLoadLimitation, LSA_NULL)))
    {   
        EDDP_GlbFreeLocalMem (pDDB->PRM.pRecordsetA->pPDNRTFeedInLoadLimitation);
        pDDB->PRM.pRecordsetA->pPDNRTFeedInLoadLimitation        = LSA_NULL;
        pDDB->PRM.pRecordsetA->PDNRTFeedInLoadLimitation_Size    = 0;
    }   
    if (!(LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetB->pPDNRTFeedInLoadLimitation, LSA_NULL)))
    {   
        EDDP_GlbFreeLocalMem (pDDB->PRM.pRecordsetB->pPDNRTFeedInLoadLimitation);
        pDDB->PRM.pRecordsetB->pPDNRTFeedInLoadLimitation        = LSA_NULL;
        pDDB->PRM.pRecordsetB->PDNRTFeedInLoadLimitation_Size    = 0;
    }
        
    if (!(LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetA->pPDSetDefaultPortStates, LSA_NULL)))
    {   
        EDDP_GlbFreeLocalMem (pDDB->PRM.pRecordsetA->pPDSetDefaultPortStates);
        pDDB->PRM.pRecordsetA->pPDSetDefaultPortStates        = LSA_NULL;
        pDDB->PRM.pRecordsetA->PDSetDefaultPortStates_SizeMax = 0;
    }   
    if (!(LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetB->pPDSetDefaultPortStates, LSA_NULL)))
    {   
        EDDP_GlbFreeLocalMem (pDDB->PRM.pRecordsetB->pPDSetDefaultPortStates);
        pDDB->PRM.pRecordsetB->pPDSetDefaultPortStates        = LSA_NULL;
        pDDB->PRM.pRecordsetB->PDSetDefaultPortStates_SizeMax = 0;
    }
        
    for ( i=1; i <= pDDB->SWI.PortCnt; i++ )
    {
        if (!(LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetA->PDPortMrpDataAdjust[i].pRecord, LSA_NULL)))
        {	
            EDDP_GlbFreeLocalMem (pDDB->PRM.pRecordsetA->PDPortMrpDataAdjust[i].pRecord);
            pDDB->PRM.pRecordsetA->PDPortMrpDataAdjust[i].pRecord       = LSA_NULL;
            pDDB->PRM.pRecordsetA->PDPortMrpDataAdjust[i].Record_Size   = 0;
        }   

        if (!(LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetB->PDPortMrpDataAdjust[i].pRecord, LSA_NULL)))
        {	
            EDDP_GlbFreeLocalMem (pDDB->PRM.pRecordsetB->PDPortMrpDataAdjust[i].pRecord);
            pDDB->PRM.pRecordsetB->PDPortMrpDataAdjust[i].pRecord       = LSA_NULL;
            pDDB->PRM.pRecordsetB->PDPortMrpDataAdjust[i].Record_Size   = 0;
        }    	
        
        if (!(LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetA->PDPortDataAdjust[i].pRecord, LSA_NULL)))
        {	
            EDDP_GlbFreeLocalMem (pDDB->PRM.pRecordsetA->PDPortDataAdjust[i].pRecord);
            pDDB->PRM.pRecordsetA->PDPortDataAdjust[i].pRecord      = LSA_NULL;
            pDDB->PRM.pRecordsetA->PDPortDataAdjust[i].Record_Size  = 0;
        }   

        if (!(LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetB->PDPortDataAdjust[i].pRecord, LSA_NULL)))
        {	
            EDDP_GlbFreeLocalMem (pDDB->PRM.pRecordsetB->PDPortDataAdjust[i].pRecord);
            pDDB->PRM.pRecordsetB->PDPortDataAdjust[i].pRecord      = LSA_NULL;
            pDDB->PRM.pRecordsetB->PDPortDataAdjust[i].Record_Size  = 0;
        }  	    
    }
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMAllocLocalMem                       +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB           +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB        : Pointer to Interface Description Block                   +*/
/*+                                                                         +*/
/*+  Result:      EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_RESOURCE                                      +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Allocates memory within PRM structure. PRM structure      +*/
/*+               has to be zero on entry!                                  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMAllocLocalMem(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP         Status = EDD_STS_OK;
    LSA_UINT32      mem_size, i;

    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMAllocLocalMem()");

    // Initialize the pointers to Set A and Set B
    pDDB->PRM.pRecordsetA                   = &(pDDB->PRM.RecordsetA);
    pDDB->PRM.pRecordsetB                   = &(pDDB->PRM.RecordsetB);
    pDDB->PRM.pPDIRData_A                   = &(pDDB->PRM.PDIRData_A);
    pDDB->PRM.pPDIRData_B                   = &(pDDB->PRM.PDIRData_B);
    pDDB->PRM.pPDIRApplicationData_A        = &(pDDB->PRM.PDIRApplicationData_A);
    pDDB->PRM.pPDIRApplicationData_B        = &(pDDB->PRM.PDIRApplicationData_B);
    pDDB->PRM.pPDIRSubframeData_A           = &(pDDB->PRM.PDIRSubframeData_A);
    pDDB->PRM.pPDIRSubframeData_B           = &(pDDB->PRM.PDIRSubframeData_B);
    pDDB->PRM.pPDNRTFeedInLoadLimitation_A  = &(pDDB->PRM.PDNRTFeedInLoadLimitation_A);
    pDDB->PRM.pPDNRTFeedInLoadLimitation_B  = &(pDDB->PRM.PDNRTFeedInLoadLimitation_B);
    pDDB->PRM.pPDSetDefaultPortStates_A     = &(pDDB->PRM.PDSetDefaultPortStates_A);
    pDDB->PRM.pPDSetDefaultPortStates_B     = &(pDDB->PRM.PDSetDefaultPortStates_B);

    switch (pDDB->Hardware)
    {
        case EDDP_HW_ERTEC200P:
        case EDDP_HW_ERTEC200P_FPGA:
        case EDDP_HW_HERA_FPGA:    
        {
            // Alloc memory for IRFrameDataArray
            pDDB->PRM.pPDIRData_A->pIRFrameDataArray = 
                (EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE)EDDP_GlbAllocLocalMem (EDDP_PRM_MAX_IRFRAMEDATA_ENTRIES_ERTEC_200 * sizeof(EDDP_PRM_PDIRFRAMEDATA_TYPE));
            pDDB->PRM.pPDIRData_A->IRFrameDataArray_Max_Entries = EDDP_PRM_MAX_IRFRAMEDATA_ENTRIES_ERTEC_200;
            pDDB->PRM.pPDIRData_B->pIRFrameDataArray = 
                (EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE)EDDP_GlbAllocLocalMem (EDDP_PRM_MAX_IRFRAMEDATA_ENTRIES_ERTEC_200 * sizeof(EDDP_PRM_PDIRFRAMEDATA_TYPE));
            pDDB->PRM.pPDIRData_B->IRFrameDataArray_Max_Entries = EDDP_PRM_MAX_IRFRAMEDATA_ENTRIES_ERTEC_200;

            // PDNCDataCheck record
            mem_size = EDDP_PRM_PDNC_DATA_CHECK_BLOCK_SIZE;
            pDDB->PRM.pRecordsetA->pPDNCDataCheck       = EDDP_GlbAllocLocalMem(mem_size);
            pDDB->PRM.pRecordsetB->pPDNCDataCheck       = EDDP_GlbAllocLocalMem(mem_size);
            pDDB->PRM.pRecordsetA->PDNCDataCheck_Size   = mem_size;
            pDDB->PRM.pRecordsetB->PDNCDataCheck_Size   = mem_size;

            // PDSCFDataCheck record
            mem_size = EDDP_PRM_PDSCF_DATA_CHECK_BLOCK_SIZE;
            pDDB->PRM.PDSCFDataCheck.pPDSCFDataCheck        = EDDP_GlbAllocLocalMem(mem_size);
            pDDB->PRM.PDSCFDataCheck.PDSCFDataCheck_Size    = mem_size;

            // PDIRData record
            mem_size = (pDDB->SWI.PortCnt * EDDP_PRM_PDIRDATA_CONSTANT_1) + (EDDP_PRM_PDIRDATA_CONSTANT_2 * EDDP_PRM_MAX_IRFRAMEDATA_ENTRIES_ERTEC_200) + EDDP_PRM_PDIRDATA_CONSTANT_3;
            pDDB->PRM.pRecordsetA->pPDIRData        = EDDP_GlbAllocLocalMem(mem_size);
            pDDB->PRM.pRecordsetB->pPDIRData        = EDDP_GlbAllocLocalMem(mem_size);
            pDDB->PRM.pRecordsetA->PDIRData_Size    = mem_size;
            pDDB->PRM.pRecordsetB->PDIRData_Size    = mem_size;

            // PDIRApplicationData record
            mem_size = EDDP_PRM_PDIRAPPLICATIONDATA_BLOCK_SIZE;
            pDDB->PRM.pRecordsetA->pPDIRApplicationData     = EDDP_GlbAllocLocalMem(mem_size);
            pDDB->PRM.pRecordsetB->pPDIRApplicationData     = EDDP_GlbAllocLocalMem(mem_size);
            pDDB->PRM.pRecordsetA->PDIRApplicationData_Size = mem_size;
            pDDB->PRM.pRecordsetB->PDIRApplicationData_Size = mem_size;

            // PDIRSubframeData record
            mem_size = EDDP_PRM_MAX_SIZE_OF_PDIRSUBFRAMEDATA_RECORD;
            pDDB->PRM.pRecordsetA->pPDIRSubframeData        = EDDP_GlbAllocLocalMem(mem_size);
            pDDB->PRM.pRecordsetB->pPDIRSubframeData        = EDDP_GlbAllocLocalMem(mem_size);
            pDDB->PRM.pRecordsetA->PDIRSubframeData_Size    = mem_size;
            pDDB->PRM.pRecordsetB->PDIRSubframeData_Size    = mem_size;

            // PDSyncData record
            mem_size = EDDP_PRM_PDSYNCDATA_MAX_SIZE;
            pDDB->PRM.pRecordsetA->pPDSyncData      = EDDP_GlbAllocLocalMem(mem_size);
            pDDB->PRM.pRecordsetB->pPDSyncData      = EDDP_GlbAllocLocalMem(mem_size);
            pDDB->PRM.pRecordsetA->PDSyncData_Size  = mem_size;
            pDDB->PRM.pRecordsetB->PDSyncData_Size  = mem_size;            

            // PDInterfaceMrpDataAdjust record
            mem_size = EDDP_PRM_PDINTERFACEMRPDATAADJUST_MAX_SIZE;
            pDDB->PRM.pRecordsetA->pPDInterfaceMrpDataAdjust        = EDDP_GlbAllocLocalMem(mem_size);
            pDDB->PRM.pRecordsetB->pPDInterfaceMrpDataAdjust        = EDDP_GlbAllocLocalMem(mem_size);
            pDDB->PRM.pRecordsetA->PDInterfaceMrpDataAdjust_Size    = mem_size;
            pDDB->PRM.pRecordsetB->PDInterfaceMrpDataAdjust_Size    = mem_size;  
            
            // PDNRTFeedInLoadLimitation record
            mem_size = EDDP_PRM_PDNRT_FEED_IN_LOAD_LIMITATION_BLOCK_SIZE;
            pDDB->PRM.pRecordsetA->pPDNRTFeedInLoadLimitation       = EDDP_GlbAllocLocalMem(mem_size);
            pDDB->PRM.pRecordsetB->pPDNRTFeedInLoadLimitation       = EDDP_GlbAllocLocalMem(mem_size);
            pDDB->PRM.pRecordsetA->PDNRTFeedInLoadLimitation_Size   = mem_size;
            pDDB->PRM.pRecordsetB->PDNRTFeedInLoadLimitation_Size   = mem_size;

            // PDSetDefaultPortStates record
            mem_size = EDDP_PRM_PDSET_DEF_PORTSTATES_BLOCK_MAX_SIZE;
            pDDB->PRM.pRecordsetA->pPDSetDefaultPortStates          = EDDP_GlbAllocLocalMem(mem_size);
            pDDB->PRM.pRecordsetB->pPDSetDefaultPortStates          = EDDP_GlbAllocLocalMem(mem_size);
            pDDB->PRM.pRecordsetA->PDSetDefaultPortStates_SizeMin   = EDDP_PRM_PDSET_DEF_PORTSTATES_BLOCK_MIN_SIZE;
            pDDB->PRM.pRecordsetA->PDSetDefaultPortStates_SizeMax   = mem_size;
            pDDB->PRM.pRecordsetB->PDSetDefaultPortStates_SizeMin   = EDDP_PRM_PDSET_DEF_PORTSTATES_BLOCK_MIN_SIZE;
            pDDB->PRM.pRecordsetB->PDSetDefaultPortStates_SizeMax   = mem_size;

            // check the allocated pointers
            if (   (LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pPDIRData_A->pIRFrameDataArray,            LSA_NULL))
                || (LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pPDIRData_B->pIRFrameDataArray,            LSA_NULL))
                || (LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetA->pPDNCDataCheck,               LSA_NULL))
                || (LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetB->pPDNCDataCheck,               LSA_NULL))
                || (LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.PDSCFDataCheck.pPDSCFDataCheck,            LSA_NULL))
                || (LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetA->pPDIRData,                    LSA_NULL))
                || (LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetB->pPDIRData,                    LSA_NULL))
                || (LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetA->pPDIRApplicationData,         LSA_NULL))
                || (LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetB->pPDIRApplicationData,         LSA_NULL))
                || (LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetA->pPDIRSubframeData,            LSA_NULL))
                || (LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetB->pPDIRSubframeData,            LSA_NULL))
                || (LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetA->pPDSyncData,                  LSA_NULL))
                || (LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetB->pPDSyncData,                  LSA_NULL))
                || (LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetA->pPDInterfaceMrpDataAdjust,    LSA_NULL))
                || (LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetB->pPDInterfaceMrpDataAdjust,    LSA_NULL))
                || (LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetA->pPDNRTFeedInLoadLimitation,   LSA_NULL))
                || (LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetB->pPDNRTFeedInLoadLimitation,   LSA_NULL))
                || (LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetA->pPDSetDefaultPortStates,      LSA_NULL))
                || (LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetB->pPDSetDefaultPortStates,      LSA_NULL))
               )
            {
                Status = EDD_STS_ERR_RESOURCE;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Status);
                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMAllocLocalMem(): ERROR -> Alloc memory for PRM records!");
                break;            
            }    

            for ( i=1; i <= pDDB->SWI.PortCnt; i++ )
            {
                // PDPortMrpDataAdjust record
                mem_size = EDDP_PRM_PDPORTMRPDATAADJUST_MAX_SIZE;
                pDDB->PRM.pRecordsetA->PDPortMrpDataAdjust[i].pRecord       = EDDP_GlbAllocLocalMem(mem_size);
                pDDB->PRM.pRecordsetB->PDPortMrpDataAdjust[i].pRecord       = EDDP_GlbAllocLocalMem(mem_size);
                pDDB->PRM.pRecordsetA->PDPortMrpDataAdjust[i].Record_Size   = mem_size;
                pDDB->PRM.pRecordsetB->PDPortMrpDataAdjust[i].Record_Size   = mem_size;                 	        

                if (   (LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetA->PDPortMrpDataAdjust[i].pRecord, LSA_NULL)) 
                    || (LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetB->PDPortMrpDataAdjust[i].pRecord, LSA_NULL))
                   )
                {
                    Status = EDD_STS_ERR_RESOURCE;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Status); 
                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMAllocLocalMem(): ERROR -> Alloc memory for PRM record PDPortMrpDataAdjust!");
                    break;           
                }

                // PDPortDataAdjust record
                mem_size = EDDP_PRM_PDPORTDATAADJUST_MAX_SIZE;
                pDDB->PRM.pRecordsetA->PDPortDataAdjust[i].pRecord      = EDDP_GlbAllocLocalMem(mem_size);
                pDDB->PRM.pRecordsetB->PDPortDataAdjust[i].pRecord      = EDDP_GlbAllocLocalMem(mem_size);
                pDDB->PRM.pRecordsetA->PDPortDataAdjust[i].Record_Size  = mem_size;
                pDDB->PRM.pRecordsetB->PDPortDataAdjust[i].Record_Size  = mem_size;                 	        

                if (   (LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetA->PDPortDataAdjust[i].pRecord, LSA_NULL)) 
                    || (LSA_HOST_PTR_ARE_EQUAL(pDDB->PRM.pRecordsetB->PDPortDataAdjust[i].pRecord, LSA_NULL))
                   )        
                {
                    Status = EDD_STS_ERR_RESOURCE;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Status);
                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMAllocLocalMem(): ERROR -> Alloc memory for PRM record PDPortDataAdjust!");
                    break;
                }
            }
        }
        break;

        default:
        {
            Status = EDDP_FATAL_ERR_NOT_SUPPORTED;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Status);
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMAllocLocalMem(): FATAL -> HardwareType(0x%X) not supported!", 
                pDDB->Hardware);
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }

    if (Status != EDD_STS_OK)
    {
        // if something goes wrong in this function -> free all allocated memory
        EDDP_PRMFreeLocalMem (pDDB);
    }

    return (Status);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMInit                                +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB           +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB        : Pointer to Interface Description Block                   +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Initializes PRM structures. Structure has to be zero on   +*/
/*+               entry and memory has to be allocated within structures!   +*/
/*+               Called on DeviceOpen                                      +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMInit(
    EDDP_LOCAL_DDB_PTR_TYPE      pDDB)
{
    LSA_UINT32                  j;
    EDDP_DDB_PRM_PTR_TYPE       pPrm;

    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PrmInit()");

    pPrm = &pDDB->PRM;

    pPrm->PrmState                          = EDDP_PRM_STATE_PREPARED; 
    pPrm->FirstCommitExecuted               = LSA_FALSE;
    pPrm->MRPPort.RecordCountInSetB         = 0;

    pPrm->Timer_isRunning                   = LSA_FALSE;

    pPrm->FrameDrop.CheckEnable             = LSA_FALSE;
    pPrm->FrameDrop.ErrorStatus             = EDDP_DIAG_FRAME_DROP_OK;
    pPrm->FrameDrop.ErrorStatusIndicated    = EDDP_DIAG_FRAME_DROP_OK;
    pPrm->FrameDrop.DropCnt                 = 0;	

    for ( j=0; j <= EDDP_MAX_PORT_CNT; j++ )
    {
        pPrm->DoIndicate[j] = LSA_FALSE;

        pPrm->pRecordsetA->PDPortMrpDataAdjust[j].Record_IsPresent  = LSA_FALSE;
        pPrm->pRecordsetB->PDPortMrpDataAdjust[j].Record_IsPresent  = LSA_FALSE;
        pPrm->pRecordsetA->PDPortMrpDataAdjust[j].Record_Skip       = LSA_FALSE;
        pPrm->pRecordsetB->PDPortMrpDataAdjust[j].Record_Skip       = LSA_FALSE;

        pPrm->pRecordsetA->PDPortDataAdjust[j].Record_IsPresent     = LSA_FALSE;
        pPrm->pRecordsetB->PDPortDataAdjust[j].Record_IsPresent     = LSA_FALSE;
        pPrm->pRecordsetA->PDPortDataAdjust[j].Record_Skip          = LSA_FALSE;
        pPrm->pRecordsetB->PDPortDataAdjust[j].Record_Skip          = LSA_FALSE;

        //pPrm->PDSetDefaultPortStates_A.RecordSet[j].PortStatePresent = LSA_FALSE;
        //pPrm->PDSetDefaultPortStates_B.RecordSet[j].PortStatePresent = LSA_FALSE;

        // init RQB-Queue for indications
        EDDP_GlbRQBQueueInit (&pPrm->PrmIndReq[j]);
    }

    pPrm->PDSCFDataCheck.PDSCFDataCheck_IsPresent               = LSA_FALSE;

    pPrm->pRecordsetA->PDInterfaceMrpDataAdjust_IsPresent       = LSA_FALSE;
    pPrm->pRecordsetA->PDIRData_IsPresent                       = LSA_FALSE;
    pPrm->pRecordsetA->PDIRApplicationData_IsPresent            = LSA_FALSE;
    pPrm->pRecordsetA->PDIRSubframeData_IsPresent               = LSA_FALSE;
    pPrm->pRecordsetA->PDNCDataCheck_IsPresent                  = LSA_FALSE;
    pPrm->pRecordsetA->PDSyncData_IsPresent                     = LSA_FALSE;
    pPrm->pRecordsetA->PDNRTFeedInLoadLimitation_IsPresent      = LSA_FALSE;

    pPrm->pRecordsetB->PDInterfaceMrpDataAdjust_IsPresent       = LSA_FALSE;
    pPrm->pRecordsetB->PDIRData_IsPresent                       = LSA_FALSE;
    pPrm->pRecordsetB->PDIRApplicationData_IsPresent            = LSA_FALSE;
    pPrm->pRecordsetB->PDIRSubframeData_IsPresent               = LSA_FALSE;
    pPrm->pRecordsetB->PDNCDataCheck_IsPresent                  = LSA_FALSE;
    pPrm->pRecordsetB->PDSyncData_IsPresent                     = LSA_FALSE;
    pPrm->pRecordsetB->PDNRTFeedInLoadLimitation_IsPresent      = LSA_FALSE;

    pPrm->pRecordsetA->PDInterfaceMrpDataAdjust_Skip            = LSA_FALSE;
    pPrm->pRecordsetA->PDIRData_Skip                            = LSA_FALSE;
    pPrm->pRecordsetA->PDIRApplicationData_Skip                 = LSA_FALSE;
    pPrm->pRecordsetA->PDIRSubframeData_Skip                    = LSA_FALSE;
    pPrm->pRecordsetA->PDNCDataCheck_Skip                       = LSA_FALSE;
    pPrm->pRecordsetA->PDSyncData_Skip                          = LSA_FALSE;
    pPrm->pRecordsetA->PDNRTFeedInLoadLimitation_Skip           = LSA_FALSE;

    pPrm->pRecordsetB->PDInterfaceMrpDataAdjust_Skip            = LSA_FALSE;
    pPrm->pRecordsetB->PDIRData_Skip                            = LSA_FALSE;
    pPrm->pRecordsetB->PDIRApplicationData_Skip                 = LSA_FALSE;
    pPrm->pRecordsetB->PDIRSubframeData_Skip                    = LSA_FALSE;
    pPrm->pRecordsetB->PDNCDataCheck_Skip                       = LSA_FALSE;
    pPrm->pRecordsetB->PDSyncData_Skip                          = LSA_FALSE;
    pPrm->pRecordsetB->PDNRTFeedInLoadLimitation_Skip           = LSA_FALSE;

    pPrm->pPDIRData_A->Sync_IsPresent                           = LSA_FALSE;
    pPrm->pPDIRData_A->MRPDConsumerWithOddFrameID_IsPresent     = LSA_FALSE;
    pPrm->pPDIRData_A->IRFrameDataArray_Count_All               = 0;
    pPrm->pPDIRData_A->IRFrameDataArray_Count_Used              = 0;
    pPrm->pPDIRData_A->FrameID_high                             = 0;
    pPrm->pPDIRData_A->FrameID_low                              = 0; 
    pPrm->pPDIRData_A->PDIRBeginEndData.NumberOfPhases_max      = 0;   

    pPrm->pPDIRData_B->Sync_IsPresent                           = LSA_FALSE;
    pPrm->pPDIRData_B->MRPDConsumerWithOddFrameID_IsPresent     = LSA_FALSE;
    pPrm->pPDIRData_B->IRFrameDataArray_Count_All               = 0;
    pPrm->pPDIRData_B->IRFrameDataArray_Count_Used              = 0;
    pPrm->pPDIRData_B->FrameID_high                             = 0;
    pPrm->pPDIRData_B->FrameID_low                              = 0; 
    pPrm->pPDIRData_B->PDIRBeginEndData.NumberOfPhases_max      = 0;    
    
    pPrm->pPDNRTFeedInLoadLimitation_A->Present                 = LSA_FALSE;
    pPrm->pPDNRTFeedInLoadLimitation_B->Present                 = LSA_FALSE;

    pPrm->pPDSetDefaultPortStates_A->Present                     = LSA_FALSE;
    pPrm->pPDSetDefaultPortStates_B->Present                     = LSA_FALSE;

    pPrm->MrtDiagPendingGroup.List.BeginIdx                     = EDDP_PRM_CHAIN_IDX_END;
    pPrm->MrtDiagPendingGroup.List.EndIdx                       = EDDP_PRM_CHAIN_IDX_END;
    pPrm->MrtDiagPendingGroup.PDIRFrameDataCnt                  = 0;

    pPrm->LateErrDiagPendingGroup.List.BeginIdx                 = EDDP_PRM_CHAIN_IDX_END;
    pPrm->LateErrDiagPendingGroup.List.EndIdx                   = EDDP_PRM_CHAIN_IDX_END;
    pPrm->LateErrDiagPendingGroup.PackGrpCnt                    = 0;

    pPrm->FrameDrop.Budgets.ErrorDropBudget.Enabled                 = LSA_FALSE;
    pPrm->FrameDrop.Budgets.MaintenanceDemandedDropBudget.Enabled   = LSA_FALSE;
    pPrm->FrameDrop.Budgets.MaintenanceRequiredDropBudget.Enabled   = LSA_FALSE;    

    pPrm->isPrmCommitRunning        = LSA_FALSE;
    pPrm->isTriggerPrmCommitFinish  = LSA_FALSE;
    pPrm->PrmCommitSm.State         = EDDP_PRM_COMMIT_STATE_IDLE;

    /* ---------------------------------------- */
    /* Init min. MaxBridgeDelay values          */
    /* ---------------------------------------- */
    switch (pDDB->Hardware)
    {
        case EDDP_HW_ERTEC200P:
        {
            // AbsoluteTime-Mode
            pPrm->CfgParam.Min_MaxBridgeDelay.AbsoluteStandardLong      = EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_STANDARD_LONG_ERTEC200P;
            pPrm->CfgParam.Min_MaxBridgeDelay.AbsoluteStandardShort     = EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_STANDARD_SHORT_ERTEC200P;
            pPrm->CfgParam.Min_MaxBridgeDelay.AbsoluteFFWLong           = EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_FFW_LONG_ERTEC200P;
            pPrm->CfgParam.Min_MaxBridgeDelay.AbsoluteFFWShort          = EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_FFW_SHORT_ERTEC200P;
            // RelativeTime-Mode
            pPrm->CfgParam.Min_MaxBridgeDelay.RelativeStandardLong      = EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_STANDARD_LONG_ERTEC200P;
            pPrm->CfgParam.Min_MaxBridgeDelay.RelativeStandardShort     = EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_STANDARD_SHORT_ERTEC200P;
            pPrm->CfgParam.Min_MaxBridgeDelay.RelativeFFWLong           = EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_FFW_LONG_ERTEC200P;
            pPrm->CfgParam.Min_MaxBridgeDelay.RelativeFFWShort          = EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_FFW_SHORT_ERTEC200P;
        }
        break;

        case EDDP_HW_ERTEC200P_FPGA: 
        {
            // AbsoluteTime-Mode
            pPrm->CfgParam.Min_MaxBridgeDelay.AbsoluteStandardLong      = EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_STANDARD_LONG_FPGA;
            pPrm->CfgParam.Min_MaxBridgeDelay.AbsoluteStandardShort     = EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_STANDARD_SHORT_FPGA;
            pPrm->CfgParam.Min_MaxBridgeDelay.AbsoluteFFWLong           = EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_FFW_LONG_FPGA;
            pPrm->CfgParam.Min_MaxBridgeDelay.AbsoluteFFWShort          = EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_FFW_SHORT_FPGA;
            // RelativeTime-Mode
            pPrm->CfgParam.Min_MaxBridgeDelay.RelativeStandardLong      = EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_STANDARD_LONG_FPGA;
            pPrm->CfgParam.Min_MaxBridgeDelay.RelativeStandardShort     = EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_STANDARD_SHORT_FPGA;
            pPrm->CfgParam.Min_MaxBridgeDelay.RelativeFFWLong           = EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_FFW_LONG_FPGA;
            pPrm->CfgParam.Min_MaxBridgeDelay.RelativeFFWShort          = EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_FFW_SHORT_FPGA;
        }
        break;

        case EDDP_HW_HERA_FPGA:    
        {
            // AbsoluteTime-Mode
            pPrm->CfgParam.Min_MaxBridgeDelay.AbsoluteStandardLong      = EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_STANDARD_LONG_FPGA_HERA;
            pPrm->CfgParam.Min_MaxBridgeDelay.AbsoluteStandardShort     = EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_STANDARD_SHORT_FPGA_HERA;
            pPrm->CfgParam.Min_MaxBridgeDelay.AbsoluteFFWLong           = EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_FFW_LONG_FPGA_HERA;
            pPrm->CfgParam.Min_MaxBridgeDelay.AbsoluteFFWShort          = EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_FFW_SHORT_FPGA_HERA;
            // RelativeTime-Mode
            pPrm->CfgParam.Min_MaxBridgeDelay.RelativeStandardLong      = EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_STANDARD_LONG_FPGA_HERA;
            pPrm->CfgParam.Min_MaxBridgeDelay.RelativeStandardShort     = EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_STANDARD_SHORT_FPGA_HERA;
            pPrm->CfgParam.Min_MaxBridgeDelay.RelativeFFWLong           = EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_FFW_LONG_FPGA_HERA;
            pPrm->CfgParam.Min_MaxBridgeDelay.RelativeFFWShort          = EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_FFW_SHORT_FPGA_HERA;
        }
        break;
        
        /* HERADevMark - when asic values are present ... */
        #if 0
        case EDDP_HW_HERA:    
        {
            // AbsoluteTime-Mode
            pPrm->CfgParam.Min_MaxBridgeDelay.AbsoluteStandardLong      = EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_STANDARD_LONG_HERA;
            pPrm->CfgParam.Min_MaxBridgeDelay.AbsoluteStandardShort     = EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_STANDARD_SHORT_HERA;
            pPrm->CfgParam.Min_MaxBridgeDelay.AbsoluteFFWLong           = EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_FFW_LONG_HERA;
            pPrm->CfgParam.Min_MaxBridgeDelay.AbsoluteFFWShort          = EDDP_PRM_MIN_MAXBRIDGEDELAY_ABSOLUTE_FFW_SHORT_HERA;
            // RelativeTime-Mode
            pPrm->CfgParam.Min_MaxBridgeDelay.RelativeStandardLong      = EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_STANDARD_LONG_HERA;
            pPrm->CfgParam.Min_MaxBridgeDelay.RelativeStandardShort     = EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_STANDARD_SHORT_HERA;
            pPrm->CfgParam.Min_MaxBridgeDelay.RelativeFFWLong           = EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_FFW_LONG_HERA;
            pPrm->CfgParam.Min_MaxBridgeDelay.RelativeFFWShort          = EDDP_PRM_MIN_MAXBRIDGEDELAY_RELATIVE_FFW_SHORT_HERA;
        }
        break;
        #endif
        
        default:
        {
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PrmInit(): FATAL -> Hardware(0x%X) is not supported!", 
                pDDB->Hardware);
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :  EDDP_PRMPDNRTFeedInLoadLimitationRecord_Write +*/
/*+  Input/Output          :  EDDP_LOCAL_DDB_PTR_TYPE      pDDB             +*/
/*+                           EDD_UPPER_MEM_U8_PTR_TYPE    pRecord          +*/
/*+                                                                         +*/
/*+  Result                :  EDD_RSP                                       +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pRecord    : Pointer to PRM record                                     +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PRM_BLOCK                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function parses the PDNRTFeedInLoadLimitation        +*/
/*+               record (pRecord)  and stores the extracted data into DDB. +*/
/*+               Moreover, the original record is stored in record set B.  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDNRTFeedInLoadLimitationRecord_Write(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord)
{
    EDD_RSP                         Response = EDD_STS_OK;
    LSA_UINT16                      val_16;
    LSA_UINT32                      record_length = 0;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_Begin;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_Tmp;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_RecordLength;
    EDDP_PRM_DETAIL_ERR_PTR_TYPE    pDetailErr = &pDDB->PrmDetailErr;
    LSA_UINT8                       LoadLimitationActive;
    LSA_UINT8                       IO_Configured;
    
    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_PRMPDNRTFeedInLoadLimitationRecord_Write()");

    LoadLimitationActive = 0;
    IO_Configured = 0;
    
    //remember the pointer to the begin of this record
    pRecord_Begin           = pRecord;
    pRecord_RecordLength    = pRecord;
    
    // check BlockType (record index) for PDNRTFeedInLoadLimitation
    pRecord_Tmp = pRecord;
    EDDP_GET_U16_INCR(pRecord, val_16);
    if (val_16 != EDDP_PRM_PDNRT_FEED_IN_LOAD_LIMITATION_BLOCK_TYPE)
    {
        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDNRTFeedInLoadLimitationRecord_Write(): ERROR -> (%d) != EDDP_PRM_PDNC_DATA_CHECK_BLOCK_TYPE", val_16);
        Response =  EDD_STS_ERR_PRM_BLOCK; 
    }
    else
    {
        // check BlockLength
        pRecord_RecordLength = pRecord;
        pRecord_Tmp = pRecord;
        EDDP_GET_U16_INCR(pRecord, val_16);
        record_length = (val_16 + 4);
        if (record_length > pDDB->PRM.pRecordsetB->PDNRTFeedInLoadLimitation_Length)
        {

            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDNRTFeedInLoadLimitationRecord_Write(): ERROR -> Record length does not match with length in RQB!");
            Response = EDD_STS_ERR_PRM_BLOCK; 
        }
        else
        {
            // check BlockVersion
            // BlockVersionHigh
            pRecord_Tmp = pRecord;
            if (*pRecord++ != 0x01)
            { 
                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDNRTFeedInLoadLimitationRecord_Write(): ERROR -> Block version does not match!");
                Response = EDD_STS_ERR_PRM_VERSION_CONFLICT; 
            }
            else
            {
                // BlockVersionLow
                pRecord_Tmp = pRecord;
                if (*pRecord++ != 0x00)
                { 
                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDNRTFeedInLoadLimitationRecord_Write(): ERROR -> Block version does not match!");
                    Response = EDD_STS_ERR_PRM_VERSION_CONFLICT; 
                }
                else
                {
                    // check padding bytes
                    pRecord_Tmp = pRecord;
                    if (*pRecord++ != 0x00)
                    { 
                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDNRTFeedInLoadLimitationRecord_Write(): ERROR -> Padding is not '0'!");
                        Response = EDD_STS_ERR_PRM_BLOCK; 
                    }
                    else
                    {
                        pRecord_Tmp = pRecord;
                        if (*pRecord++ != 0x00)
                        { 
                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDNRTFeedInLoadLimitationRecord_Write(): ERROR -> Padding is not '0'!");
                            Response = EDD_STS_ERR_PRM_BLOCK; 
                        }
                        else
                        {
                           
                        }
                    }
                }
            }
        }
    }
    
    if (EDD_STS_OK == Response)
    {
        // check LoadLimitationActive
        pRecord_Tmp = pRecord;
        LoadLimitationActive = *pRecord++;
        if( (EDDP_PRM_PDNRT_FEED_IN_LOAD_LIMITATION_ACTIVE != LoadLimitationActive) &&
                    (EDDP_PRM_PDNRT_FEED_IN_LOAD_LIMITATION_INACTIVE != LoadLimitationActive))
        {
            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, 
                        "[H:--] EDDP_PRMPDNRTFeedInLoadLimitationRecord_Write(): ERROR -> LoadLimitationActive is invalid (0x%x)!",
                        LoadLimitationActive);
            Response = EDD_STS_ERR_PRM_BLOCK; 
        }
    }
    
    if (EDD_STS_OK == Response)
    {
        // check IO_Configured
        pRecord_Tmp = pRecord;
        IO_Configured = *pRecord++;
        if( (EDDP_PRM_PDNRT_FEED_IN_LOAD_LIMITATION_IO_CONFIGURED != IO_Configured) &&
                    (EDDP_PRM_PDNRT_FEED_IN_LOAD_LIMITATION_IO_NOT_CONFIGURED != IO_Configured))
        {
            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, 
                        "[H:--] EDDP_PRMPDNRTFeedInLoadLimitationRecord_Write(): ERROR -> IO_Configured is invalid (0x%x)!",
                        IO_Configured);
            Response = EDD_STS_ERR_PRM_BLOCK; 
        }
    }
    
    if(EDD_STS_OK == Response)
    {
        /* IO_Configured and not LoadLimitation is not allowed */
        if( (EDDP_PRM_PDNRT_FEED_IN_LOAD_LIMITATION_IO_CONFIGURED == IO_Configured) &&
                    (EDDP_PRM_PDNRT_FEED_IN_LOAD_LIMITATION_INACTIVE == LoadLimitationActive))
        {
            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, 
                        "[H:--] EDDP_PRMPDNRTFeedInLoadLimitationRecord_Write(): ERROR -> LoadLimitation inactive and IO_Configured is not allowed!");
            Response = EDD_STS_ERR_PRM_BLOCK;
        }
    }
    
    if (EDD_STS_OK == Response)
    {
        // check padding
        pRecord_Tmp = pRecord;
        EDDP_GET_U16_INCR(pRecord, val_16);
        if( (0x000 != val_16) )
        {
            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, 
                        "[H:--] EDDP_PRMPDNRTFeedInLoadLimitationRecord_Write(): ERROR -> second padding is invalid (0x%x)!",
                        val_16);
            Response = EDD_STS_ERR_PRM_BLOCK; 
        }
    }

    if (Response == EDD_STS_OK)
    {
        // store fill values
        pDDB->PRM.pPDNRTFeedInLoadLimitation_B->Present = LSA_TRUE;
        if(EDDP_PRM_PDNRT_FEED_IN_LOAD_LIMITATION_ACTIVE == LoadLimitationActive)
        {
            pDDB->PRM.pPDNRTFeedInLoadLimitation_B->LoadLimitationActive = LSA_TRUE;
        }
        else
        {
            pDDB->PRM.pPDNRTFeedInLoadLimitation_B->LoadLimitationActive = LSA_FALSE;
        }
        if(EDDP_PRM_PDNRT_FEED_IN_LOAD_LIMITATION_IO_CONFIGURED == IO_Configured)
        {
            pDDB->PRM.pPDNRTFeedInLoadLimitation_B->IO_Configured = LSA_TRUE;
        }
        else
        {
            pDDB->PRM.pPDNRTFeedInLoadLimitation_B->IO_Configured = LSA_FALSE;
        }
    }

    // check if number of read bytes is same as record length
    if ( ((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_Begin) != record_length )
    {
        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_RecordLength - pRecord_Begin);
        Response = EDD_STS_ERR_PRM_BLOCK;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDNRTFeedInLoadLimitationRecord_Write(): ERROR -> ERROR -> number of read bytes(%d) != record_length(%d)", 
            ((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_Begin), record_length);
    }
    
    if (Response == EDD_STS_OK)
    {
        // store record in record set B
        EDDP_MEMCOPY_COMMON(pDDB->PRM.pRecordsetB->pPDNRTFeedInLoadLimitation, pRecord_Begin, record_length);
        pDDB->PRM.pRecordsetB->PDNRTFeedInLoadLimitation_Length     = record_length;
        pDDB->PRM.pRecordsetB->PDNRTFeedInLoadLimitation_IsPresent  = LSA_TRUE;
    }
            
    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMPDNRTFeedInLoadLimitationRecord_Write(): Response=0x%X", Response);

    return (Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDNRTFeedInLoadLimitationConsistencyCheck_End   +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Consistency checks for PDNRTFeedInLoadLimitation records              +*/
/*+                          for PRM_END                                                +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDNRTFeedInLoadLimitationConsistencyCheck_End(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDDP_PRM_DETAIL_ERR_PTR_TYPE    pDetailErr)
{
    EDD_RSP Response;
    LSA_UINT32 ActiveProvConsCnt;
    LSA_BOOL loadLimitationActiveStartup;

    Response = EDD_STS_OK;
    ActiveProvConsCnt = EDDP_CRTGetNumberActiveConsumerProvider(pDDB);
    loadLimitationActiveStartup = EDDP_NRTFillGetStartupParameter(pDDB);
    /* consistency errors */
    /* record not present & added prov/cons & fill not active on startup */
    if (!(pDDB->PRM.pPDNRTFeedInLoadLimitation_B->Present) &&
                (0 != ActiveProvConsCnt) &&
                !(loadLimitationActiveStartup))
    {
        EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT,
                    EDD_PRM_ERR_INDEX_DEFAULT, EDD_PRM_ERR_OFFSET_DEFAULT,
                    EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
        Response = EDD_STS_ERR_PRM_CONSISTENCY;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,
                    "[H:--] EDDP_PRMPDNRTFeedInLoadLimitationConsistencyCheck_End(): ERROR -> reset to default settings (FILL inactive) with added prov / cons");
    }

    if (EDD_STS_OK == Response)
    {
        /* record present & added prov/cons & not LoadLimitationActive */
        if ((pDDB->PRM.pPDNRTFeedInLoadLimitation_B->Present) &&
                    (0 != ActiveProvConsCnt) &&
                    (!pDDB->PRM.pPDNRTFeedInLoadLimitation_B->LoadLimitationActive))
        {
            EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT,
                        EDDP_PRM_PDNRT_FEED_IN_LOAD_LIMITATION_RECORD_INDEX,
                        EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT,
                        LSA_TRUE);
            Response = EDD_STS_ERR_PRM_CONSISTENCY;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,
                        "[H:--] EDDP_PRMPDNRTFeedInLoadLimitationConsistencyCheck_End(): ERROR -> deactivate FILL with added prov / cons");
        }
    }

    if (EDD_STS_OK == Response)
    {
        /* record present & added prov/cons & not IO_configured */
        if ((pDDB->PRM.pPDNRTFeedInLoadLimitation_B->Present) &&
                    (0 != ActiveProvConsCnt) &&
                    (!pDDB->PRM.pPDNRTFeedInLoadLimitation_B->IO_Configured))
        {
            EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT,
                        EDDP_PRM_PDNRT_FEED_IN_LOAD_LIMITATION_RECORD_INDEX,
                        EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT,
                        LSA_TRUE);
            Response = EDD_STS_ERR_PRM_CONSISTENCY;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,
                        "[H:--] EDDP_PRMPDNRTFeedInLoadLimitationConsistencyCheck_End(): ERROR -> No IO_Configured with added prov / cons");
        }
    }

    return (Response);
}
    
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDNRTFeedInLoadLimitationUpdate_Commit          +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Updates FILL by PDNRTFeedInLoadLimitation record                      +*/
/*+                          for PRM_COMMIT                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDNRTFeedInLoadLimitationUpdate_Commit(
    EDDP_LOCAL_DDB_PTR_TYPE             pDDB)
{
    EDD_RSP Response;
    LSA_BOOL resetFill;
    LSA_BOOL loadLimitationActive;
    LSA_BOOL io_Configured;
    
    Response = EDD_STS_OK;
    resetFill = (!(pDDB->PRM.pRecordsetA->PDNRTFeedInLoadLimitation_IsPresent));
    loadLimitationActive = pDDB->PRM.pPDNRTFeedInLoadLimitation_A->LoadLimitationActive;
    io_Configured = pDDB->PRM.pPDNRTFeedInLoadLimitation_A->IO_Configured;
                     
    EDDP_NRTFillSetup(pDDB, resetFill, loadLimitationActive, io_Configured);
    
    return (Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :  EDDP_PRMPDSetDefaultPortStatesRecord_Write    +*/
/*+  Input/Output          :  EDDP_LOCAL_DDB_PTR_TYPE      pDDB             +*/
/*+                           EDD_UPPER_MEM_U8_PTR_TYPE    pRecord          +*/
/*+                                                                         +*/
/*+  Result                :  EDD_RSP                                       +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pRecord    : Pointer to PRM record                                     +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PRM_BLOCK                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function parses the PDSetDefaultPortStates           +*/
/*+               record (pRecord)  and stores the extracted data into DDB. +*/
/*+               Moreover, the original record is stored in record set B.  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDSetDefaultPortStatesRecord_Write(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord)
{
    LSA_UINT32                      i;
    EDD_RSP                         Response = EDD_STS_OK;
    LSA_UINT16                      val_16;
    LSA_UINT32                      record_length = 0;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_Begin;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_Tmp;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_RecordLength;
    EDDP_PRM_DETAIL_ERR_PTR_TYPE    pDetailErr = &pDDB->PrmDetailErr;

    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_PRMPDSetDefaultPortStatesRecord_Write()");

    pDDB->PRM.PDSetDefaultPortStates_B.Present               = LSA_FALSE;
    pDDB->PRM.pRecordsetB->PDSetDefaultPortStates_IsPresent  = LSA_FALSE;

    /* Set all entrys to not present*/
    for (i = 1; i <= pDDB->SWI.PortCnt; i++)
    {
        pDDB->PRM.PDSetDefaultPortStates_B.RecordSet[i].PortStatePresent = LSA_FALSE;
    }

    //remember the pointer to the begin of this record
    pRecord_Begin           = pRecord;
    pRecord_RecordLength    = pRecord;
    
    /* check record header */
    /***********************/

    // check BlockType (record index) for PDSetDefaultPortStates
    pRecord_Tmp = pRecord;
    EDDP_GET_U16_INCR(pRecord, val_16);
    if (val_16 != EDDP_PRM_PD_SET_DEFAULT_PORT_STATES_BLOCK_TYPE)
    {
        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDSetDefaultPortStatesRecord_Write(): ERROR -> (%d) != EDDP_PRM_PD_SET_DEFAULT_PORT_STATES_BLOCK_TYPE", val_16);
        Response =  EDD_STS_ERR_PRM_BLOCK; 
    }
    else
    {
        // check BlockLength
        pRecord_RecordLength = pRecord;
        pRecord_Tmp          = pRecord;
        EDDP_GET_U16_INCR(pRecord, val_16);
        record_length        = (val_16 + 4);

        if (record_length > pDDB->PRM.pRecordsetB->PDSetDefaultPortStates_SizeMax)  //although already checked
        {

            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDSetDefaultPortStatesRecord_Write(): ERROR -> Record length does not match with length in RQB!");
            Response = EDD_STS_ERR_PRM_BLOCK; 
        }
        else
        {
            // check BlockVersion
            // BlockVersionHigh
            pRecord_Tmp = pRecord;
            if (*pRecord++ != 0x01)
            { 
                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDSetDefaultPortStatesRecord_Write(): ERROR -> Block version does not match!");
                Response = EDD_STS_ERR_PRM_VERSION_CONFLICT; 
            }
            else
            {
                // BlockVersionLow
                pRecord_Tmp = pRecord;
                if (*pRecord++ != 0x00)
                { 
                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDSetDefaultPortStatesRecord_Write(): ERROR -> Block version does not match!");
                    Response = EDD_STS_ERR_PRM_VERSION_CONFLICT; 
                }
                else
                {
                    // check padding bytes
                    pRecord_Tmp = pRecord;
                    if (*pRecord++ != 0x00)
                    { 
                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDSetDefaultPortStatesRecord_Write(): ERROR -> Padding is not '0'!");
                        Response = EDD_STS_ERR_PRM_BLOCK; 
                    }
                    else
                    {
                        pRecord_Tmp = pRecord;
                        if (*pRecord++ != 0x00)
                        { 
                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDSetDefaultPortStatesRecord_Write(): ERROR -> Padding is not '0'!");
                            Response = EDD_STS_ERR_PRM_BLOCK; 
                        }
                        else
                        {
                           
                        }
                    }
                }
            }
        }
    }

    /*---------------------------------------------------------------------------*/
    /*  EDDP_PRM_INDEX_PDSET_DEFAULT_PORT_STATES, record_index:0x10001           */
    /*---------------------------------------------------------------------------*/
    /*                                                                           */
    /*    BlockHeader                                                       6    */
    /*    LSA_UINT8           Padding                                       1    */
    /*    LSA_UINT8           Padding                                       1    */
    /*    (                                                                      */
    /*      LSA_UINT8           PortId                                      1    */
    /*      LSA_UINT8           PortState                                   1    */
    /*      LSA_UINT8           Padding                                     1    */
    /*      LSA_UINT8           Padding                                     1    */
    /*    )*                                                                     */
    /*                                                                           */
    /* PortID: 1..255 (a valid PortId)                                           */
    /* each PortId must be a present Port and only appears once within record!   */
    /* Multiple PortId settings may be present within record.                    */
    /*                                                                           */
    /*---------------------------------------------------------------------------*/

    /*---------------------------------------------------------------------------*/
    /* if header ok. continue with content..                                     */
    /*---------------------------------------------------------------------------*/

    if (Response == EDD_STS_OK)
    {
        LSA_UINT8  PortId;
        LSA_UINT8  PortState;

        while (((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_Begin + EDDP_PRM_PDSETDEFAULTPORTSTATE_BLOCKSIZE) <= record_length )
        {
            PortId    = * pRecord;
            PortState = *(pRecord + 1);

            /* check for valid PortId */
            if ((PortId > 0) && (PortId <= pDDB->SWI.PortCnt))
            {
                /* check for duplicate entry */
                if ( ! pDDB->PRM.PDSetDefaultPortStates_B.RecordSet[PortId].PortStatePresent )
                {
                    /* check for valid PortState */
                    if (( PortState == EDDP_PRM_RECORD_PD_SET_DEFAULT_PORT_STATES_DISABLE ) ||
                        ( PortState == EDDP_PRM_RECORD_PD_SET_DEFAULT_PORT_STATES_BLOCKING))
                    {
                        /* all ok. store values.. */
                        pDDB->PRM.PDSetDefaultPortStates_B.RecordSet[PortId].PortStatePresent = LSA_TRUE;
                        pDDB->PRM.PDSetDefaultPortStates_B.RecordSet[PortId].PortState        = PortState;

                        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "EDDP_PrmCheckPDSetDefaultPortStates: PortID: %d, PortState: %d",PortId,PortState);
                    }
                    else
                    {
                        /* Invalid PortState! */
                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, (LSA_UINT32)pRecord - (LSA_UINT32)pRecord_Begin
                                              + EDDP_PRM_RECORD_PD_SET_DEFAULT_PORT_STATES_OFFSET_PortState);
                        Response = EDD_STS_ERR_PRM_BLOCK;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "EDDP_PrmCheckPDSetDefaultPortStates, Invalid PortState: 0x%X for ID 0x%X",
                                          PortState, PortId);
                    }
                }
                else
                {
                    /* multiple PortId */
                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr,  (LSA_UINT32)pRecord - (LSA_UINT32)pRecord_Begin
                                          + EDDP_PRM_RECORD_PD_SET_DEFAULT_PORT_STATES_OFFSET_PortID);
                    Response = EDD_STS_ERR_PRM_BLOCK;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "EDDP_PrmCheckPDSetDefaultPortStates, Multiple PortId: 0x%X",
                                      PortId);
                }
            }
            else
            {
                /* invalid PortId */
                EDDP_PRM_SET_ERR_OFFSET(pDetailErr,  (LSA_UINT32)pRecord - (LSA_UINT32)pRecord_Begin
                                        + EDDP_PRM_RECORD_PD_SET_DEFAULT_PORT_STATES_OFFSET_PortID);
                Response = EDD_STS_ERR_PRM_BLOCK;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "EDDP_PrmCheckPDSetDefaultPortStates, Invalid PortId: 0x%X",
                                  PortId);
            }

            pRecord += EDDP_PRM_PDSETDEFAULTPORTSTATE_BLOCKSIZE;
        }

        /* if ok check if Blocklength matches (no bytes left) */
        if ((Response == EDD_STS_OK) && (((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_Begin) != record_length ))
        {
            /* invalid BlockLength (dont matches content) */
            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_RecordLength - pRecord_Begin);
            Response = EDD_STS_ERR_PRM_BLOCK;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "EDDP_PrmCheckPDSetDefaultPortStates, Invalid BlockLength:0x%X :0x%X",
                              ((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_Begin), record_length);
        }
    }

    //// check if number of read bytes is same as record length
    //if ( ((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_Begin) != record_length )
    //{
    //    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_RecordLength - pRecord_Begin);
    //    Response = EDD_STS_ERR_PRM_BLOCK;
    //    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
    //    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDSetDefaultPortStatesRecord_Write(): ERROR -> ERROR -> number of read bytes(%d) != record_length(%d)", 
    //        ((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_Begin), record_length);
    //}
    
    if (Response == EDD_STS_OK)
    {
        // store record in record set B
        EDDP_MEMCOPY_COMMON(pDDB->PRM.pRecordsetB->pPDSetDefaultPortStates, pRecord_Begin, record_length);
        pDDB->PRM.pRecordsetB->PDSetDefaultPortStates_Length     = record_length;
        pDDB->PRM.pRecordsetB->PDSetDefaultPortStates_IsPresent  = LSA_TRUE;
        pDDB->PRM.PDSetDefaultPortStates_B.Present               = LSA_TRUE;
    }

    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMPDSetDefaultPortStatesRecord_Write(): Response=0x%X", Response);

    return (Response);
}

#if 0
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDSetDefaultPortStatesConsistencyCheck_End      +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Consistency checks for PDSetDefaultPortStates record                  +*/
/*+                          for PRM_END                                                +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDSetDefaultPortStatesConsistencyCheck_End(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDDP_PRM_DETAIL_ERR_PTR_TYPE    pDetailErr)
{
    EDD_RSP Response;

    //?????  Und nu ?????
    
    Response = EDD_STS_OK;

    return (Response);
}
    
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDSetDefaultPortStatesUpdate_Commit             +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Updates FILL by PDSetDefaultPortStates record                         +*/
/*+                          for PRM_COMMIT                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDSetDefaultPortStatesUpdate_Commit(
    EDDP_LOCAL_DDB_PTR_TYPE             pDDB)
{
    EDD_RSP Response;
    
    Response = EDD_STS_OK;

    return (Response);
}
#endif

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMDeviceOpen                          +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             EDDP_UPPER_DPB_PTR_TYPE        pDPB         +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDD_SRV_DEV_OPEN                                          +*/
/*+               Initializes all PRM management structures within DDB      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMDeviceOpen(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    EDDP_UPPER_DPB_PTR_TYPE            pDPB)
{
    EDD_RSP          Response = EDD_STS_OK;
    
    LSA_UNUSED_ARG(pDPB);

    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_PRMDeviceOpen(pDDB=0x%X, pDPB=0x%X)", pDDB, pDPB);

    Response = EDDP_PRMAllocLocalMem (pDDB);
    if (Response == EDD_STS_OK)
    {
         Response = EDDP_CoreTimerAlloc (pDDB, LSA_TIMER_TYPE_CYCLIC, LSA_TIME_BASE_1S, /*pDDB->hDDB,*/ EDDP_TIMER_FRAME_DROP, LSA_NULL, &pDDB->PRM.Timer);
         if (Response == EDD_STS_OK)
         {
             EDDP_PRMInit (pDDB);
         }
    }	

    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMDeviceOpen(): Response=0x%X", Response);

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMDeviceClose                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE           pDDB      +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDDP_SRV_DEV_CLOSE or EDDP_SRV_DEV_OPEN (only if          +*/
/*+               previous OPEN shall be aborted)                           +*/
/*+               Close device for PRM                                      +*/
/*+                                                                         +*/
/*+       Note:   EDD_STS_OK_PENDING not allowed as response. This call     +*/
/*+               must be synchronously!                                    +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMDeviceClose(
    EDDP_LOCAL_DDB_PTR_TYPE    pDDB)
{
    EDD_RSP         Response;

    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_PRMDeviceClose(pDDB=0x%X)", pDDB);

    Response = EDD_STS_OK;

    EDDP_PRMFreeLocalMem (pDDB);

    Response = EDDP_CoreTimerFree (pDDB->PRM.Timer);

    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMDeviceClose(): Response=0x%X", Response);

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMDeviceIFShutdown                    +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE           pDDB      +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_OK_PENDING                                        +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDDP_SRV_DEV_SHUTDOWN or EDDP_SRV_DEV_SETUP (only if      +*/
/*+               previous setup shall be aborted)                          +*/
/*+               Shutdown Interface of PRM                                 +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMDeviceIFShutdown(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_PRMDeviceIFShutdown(pDDB=0x%X)", pDDB);

    // only stop timer if it is running
    if (pDDB->PRM.Timer_isRunning)
    {
        // stop frame drop timer
        EDDP_CoreTimerStop (pDDB->PRM.Timer);
        pDDB->PRM.Timer_isRunning = LSA_FALSE;
    }

    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMDeviceShutdown(): Response=0x%X", EDD_STS_OK);

    return (EDD_STS_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMDeviceIFSetup                       +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE           pDDB      +*/
/*+                             EDDP_UPPER_DSB_PTR_TYPE           pDSB      +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  pDSB       : Pointer to DeviceSetupBlock                         (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_OK_PENDING                                        +*/
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDDP_SRV_DEV_SETUP                                        +*/
/*+               Setup Device for PRM                                      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMDeviceIFSetup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DSB_PTR_TYPE     pDSB)
{
#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pDDB);
    LSA_UNUSED_ARG(pDSB);
#endif

    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_PRMDeviceSetup(pDDB=0x%X, pDSB=0x%X)", pDDB, pDSB);

    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMDeviceSetup(): Response=0x%X", EDD_STS_OK);

    return (EDD_STS_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMSetBToNotPresent                    +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB           +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Inits record set B to "not present"                       +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMSetBToNotPresent(
    EDDP_LOCAL_DDB_PTR_TYPE      pDDB)
{
    LSA_UINT32                  i;
    EDDP_DDB_PRM_PTR_TYPE       pPrm;

    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMSetBToNotPresent()");

    pPrm = &pDDB->PRM;

    for ( i=0; i <= EDDP_MAX_PORT_CNT; i++ )
    {	
        pPrm->pRecordsetB->PDPortMrpDataAdjust[i].Record_IsPresent  = LSA_FALSE;
        pPrm->pRecordsetB->PDPortMrpDataAdjust[i].Record_Skip       = LSA_FALSE;
        pPrm->pRecordsetB->PDPortMrpDataAdjust[i].Record_Length     = 0;

        pPrm->pRecordsetB->PDPortDataAdjust[i].Record_IsPresent     = LSA_FALSE;
        pPrm->pRecordsetB->PDPortDataAdjust[i].Record_Skip          = LSA_FALSE;	    
        pPrm->pRecordsetB->PDPortDataAdjust[i].Record_Length        = 0;

        pPrm->PortDataAdjust.Parameters[i].DCPBoundary_IsPresent                    = LSA_FALSE;
        pPrm->PortDataAdjust.Parameters[i].DomainBoundaryIngressEgress_IsPresent    = LSA_FALSE;
        pPrm->PortDataAdjust.Parameters[i].LinkState_IsPresent                      = LSA_FALSE;
        pPrm->PortDataAdjust.Parameters[i].MAUType_IsPresent                        = LSA_FALSE;	    	    	    				
        pPrm->PortDataAdjust.Parameters[i].MulticastBoundary_IsPresent              = LSA_FALSE;	    
        pPrm->PortDataAdjust.Parameters[i].PeerToPeerBoundary_IsPresent             = LSA_FALSE;
        pPrm->PortDataAdjust.Parameters[i].PreambleLength_IsPresent                 = LSA_FALSE;

        if (i < EDDP_MAX_PORT_CNT)
        {
            pPrm->pPDIRData_B->PDIRGlobalData.DelayValues[i].YellowTime_IsPresent = LSA_FALSE;

            pPrm->pPDIRData_B->LastLocalIRTFrameArray[i].RxEndLocalNs           = 0;
            pPrm->pPDIRData_B->LastLocalIRTFrameArray[i].RxLastLocalFrameLen    = 0;
            pPrm->pPDIRData_B->LastLocalIRTFrameArray[i].TxEndLocalNs           = 0;
            pPrm->pPDIRData_B->LastLocalIRTFrameArray[i].TxLastLocalFrameLen    = 0;
        }
    }

    pPrm->pRecordsetB->PDInterfaceMrpDataAdjust_IsPresent   = LSA_FALSE;
    pPrm->pRecordsetB->PDInterfaceMrpDataAdjust_Skip        = LSA_FALSE;
    pPrm->pRecordsetB->PDInterfaceMrpDataAdjust_Length      = 0;

    pPrm->pRecordsetB->PDIRData_IsPresent                   = LSA_FALSE;
    pPrm->pRecordsetB->PDIRData_Skip                        = LSA_FALSE;	
    pPrm->pRecordsetB->PDIRData_Length                      = 0;

    pPrm->pRecordsetB->PDIRApplicationData_IsPresent        = LSA_FALSE;
    pPrm->pRecordsetB->PDIRApplicationData_Skip             = LSA_FALSE;
    pPrm->pRecordsetB->PDIRApplicationData_Length           = 0;

    pPrm->pRecordsetB->PDIRSubframeData_IsPresent           = LSA_FALSE;
    pPrm->pRecordsetB->PDIRSubframeData_Skip                = LSA_FALSE;
    pPrm->pRecordsetB->PDIRSubframeData_Length              = 0;

    pPrm->pRecordsetB->PDNCDataCheck_IsPresent              = LSA_FALSE;
    pPrm->pRecordsetB->PDNCDataCheck_Skip                   = LSA_FALSE;	
    pPrm->pRecordsetB->PDNCDataCheck_Length                 = 0;

    pPrm->pRecordsetB->PDSyncData_IsPresent                 = LSA_FALSE;
    pPrm->pRecordsetB->PDSyncData_Skip                      = LSA_FALSE;	
    pPrm->pRecordsetB->PDSyncData_Length                    = 0;

    pPrm->pPDIRData_B->Sync_IsPresent                       = LSA_FALSE;
    pPrm->pPDIRData_B->MRPDConsumerWithOddFrameID_IsPresent = LSA_FALSE;
    pPrm->pPDIRData_B->IRFrameDataArray_Count_Used          = 0;
    pPrm->pPDIRData_B->FrameDataProperties_IsPresent        = LSA_FALSE;

    pPrm->pPDIRData_B->LocalTransferStartEnd.RxLocalTransferEndNs       = 0;
    pPrm->pPDIRData_B->LocalTransferStartEnd.TxLocalTransferStartNs     = 0;
    pPrm->pPDIRData_B->LocalTransferStartEnd.TxLocalTransferEndNs       = 0;
    pPrm->pPDIRData_B->LocalTransferStartEnd.RxMaxFSOWithFrameLengthNs  = 0;
    pPrm->pPDIRData_B->LocalTransferStartEnd.TxMinFSONs                 = 0;
    pPrm->pPDIRData_B->LocalTransferStartEnd.TxMaxFSOWithFrameLengthNs  = 0;

    pPrm->pPDIRData_B->EndOfRedPhaseIFPorts.RxNs    = 0;
    pPrm->pPDIRData_B->EndOfRedPhaseIFPorts.TxNs    = 0;
    pPrm->pPDIRData_B->EndOfRedPhaseIFPorts.RxTxNs  = 0;
    
    pPrm->pPDNRTFeedInLoadLimitation_B->Present             = LSA_FALSE;
    pPrm->pRecordsetB->PDNRTFeedInLoadLimitation_IsPresent  = LSA_FALSE;
    pPrm->pRecordsetB->PDNRTFeedInLoadLimitation_Skip       = LSA_FALSE;
    pPrm->pRecordsetB->PDNRTFeedInLoadLimitation_Length     = 0;

    pPrm->pPDSetDefaultPortStates_B->Present                = LSA_FALSE;
    pPrm->pRecordsetB->PDSetDefaultPortStates_IsPresent     = LSA_FALSE;
    pPrm->pRecordsetB->PDSetDefaultPortStates_Skip          = LSA_FALSE;
    pPrm->pRecordsetB->PDSetDefaultPortStates_Length        = 0;

    pPrm->MRPPort.RecordCountInSetB = 0;
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMCopySetB2A                          +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB           +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Copy all records from set B to set A. A will be activated.+*/
/*+               Will be called on PRM_COMMIT.                             +*/
/*+                                                                         +*/
/*+  PDInterfaceMrpDataAdjust / PDPortMrpDataAdjust:                        +*/
/*+   1) If there is a PDInterfaceMrpDataAdjust record and no               +*/
/*+      PDPortMrpDataAdjust record then set all ports to 'Forwarding'.     +*/
/*+   2) If there is a PDInterfaceMrpDataAdjust record and two              +*/
/*+      PDPortMrpDataAdjust records then set those two ports to 'Blocking'.+*/
/*+   3) If there is no PDInterfaceMrpDataAdjust record then set the        +*/
/*+      default ring ports to 'Blocking'.                                  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMCopySetB2A(
    EDDP_LOCAL_DDB_PTR_TYPE      pDDB)
{
    LSA_UINT32                                  i, j, HwPortID, Index;
    EDD_RSP                                     Response;
    LSA_BOOL                                    MRP_active;
    LSA_BOOL                                    bEgress, bIngress, bSkip;
    EDDP_DDB_PRM_PTR_TYPE                       pPrm;
    EDD_RQB_MULTICAST_FWD_CTRL_TYPE             MC_FWD_CTRL_Param;
    EDD_RQB_PORTID_MODE_TYPE                    PortIDModeArray[EDDP_MAX_PORT_CNT];
    EDD_MAC_ADR_TYPE	                        MAC_Address_Temp;
    EDDP_PRM_RECORDSET_PTR_TYPE                 pRecordset_Temp;
    EDDP_PRM_PDIRDATA_PTR_TYPE                  pPDIRData_Temp;
    EDDP_PRM_PDIRAPPLICATIONDATA_PTR_TYPE       pPDIRApplicationData_Temp;
    EDDP_PRM_PDIRSUBFRAMEDATA_PTR_TYPE          pPDIRSubframeData_Temp;
    EDDP_PDNRT_FILL_RECORD_SET_PTR_TYPE         pPDNRTFeedInLoadLimitation_Temp;
    LSA_UINT8                                   LinkSpeedModeConfigured;

    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_PRMCopySetB2A()");

    Response                            = EDD_STS_OK;
    pPrm                                = &pDDB->PRM;
    MC_FWD_CTRL_Param.pPortIDModeArray  = PortIDModeArray;
    MRP_active                          = LSA_FALSE;

    /* ------------------------------------------------------------------------ */
    /* Set flag which Indications have to be triggered after PrmCommit.         */
    /* Currently all.                                                           */
    /* ------------------------------------------------------------------------ */
    // ***** Note: pDDB->SWI.PrmPreabmleCfg[0] not used !!!
    for ( i=0; i <= pDDB->SWI.PortCnt; i++ )
    {
        // ***** always indicate all Ports(1..EDDP_MAX_PORT_CNT) and the Interface(0)
        pPrm->DoIndicate[i] = LSA_TRUE;

        // ***** store preamble CFG from PRM
        pDDB->SWI.PrmPreabmleCfg[i].PreambleLength_IsPresent = pDDB->PRM.PortDataAdjust.Parameters[i].PreambleLength_IsPresent;
        pDDB->SWI.PrmPreabmleCfg[i].PreambleLength           = pDDB->PRM.PortDataAdjust.Parameters[i].PreambleLength;

        // ***** Note: Call this function here at first to set the ShortPreamble for the RTC3PSM
        EDDP_SWI_Trigger_PreambleLength (pDDB, i);
    }

    // set port states for default ring ports on first PrmCommit
    if (!(pDDB->PRM.FirstCommitExecuted))
    {
        // it is the first parametrization

        /* ---------------------------------------------------------------------- */
        /* Handle PDSetDefaultPortStates BEFORE MRP records, although there       */
        /* is no dependency: Either DLR or MRP is active (unlike EDDI and EDDS!)! */
        /* ---------------------------------------------------------------------- */

        if (pDDB->PRM.PDSetDefaultPortStates_B.Present)
        {
            if (pDDB->SWI.MaxMRP_Instances != 0)
            {
                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMCopySetB2A(): FATAL -> DLR AND MRP-rings parametrized!");
                EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
            }
            else
            {
                // PDSetDefaultPortStates  record is present
                for ( i = 1; i <= pDDB->SWI.PortCnt; i++ ) //Iterate by UsrPortID
                {
                    HwPortID = EDDP_SWI_get_HwPortID (pDDB, i);

                    if (pDDB->PRM.PDSetDefaultPortStates_B.RecordSet[i].PortStatePresent)
                    {
                        if (EDDP_PRM_RECORD_PD_SET_DEFAULT_PORT_STATES_BLOCKING == pDDB->PRM.PDSetDefaultPortStates_B.RecordSet[i].PortState)
                        {
                            //might be a DLR ringport -> BLOCKING
                            pDDB->SWI.LinkStatus[i].PortState = EDD_PORT_STATE_BLOCKING;
                        }
                        else
                        {
                            //Portstate can either be BLOCKING or DISABLE
                            pDDB->SWI.LinkStatus[i].PortState = EDD_PORT_STATE_DISABLE;
                        }
                    }
                    else
                    {
                        // normal port -> initially set to 'Forwarding'
                        pDDB->SWI.LinkStatus[i].PortState = EDD_PORT_STATE_FORWARDING;
                    }
                    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMCopySetB2A(UsrPortID=%d, HwPortID=%d): new PortState=0x%X", 
                        i, HwPortID, pDDB->SWI.LinkStatus[i].PortState);
                } //end loop

#if 0
                // _???????
                MC_FWD_CTRL_Param.PortIDModeCnt = (LSA_UINT16)pDDB->SWI.PortCnt;
                for ( i=0; i < pDDB->SWI.PortCnt; i++ )
                {
                    MC_FWD_CTRL_Param.pPortIDModeArray[i].Mode      = EDD_MULTICAST_FWD_DISABLE;
                    MC_FWD_CTRL_Param.pPortIDModeArray[i].DstPortID = (LSA_UINT16)i+1;
                    MC_FWD_CTRL_Param.pPortIDModeArray[i].SrcPortID = (LSA_UINT16) EDD_PORT_ID_AUTO;
                }

                // DLR Group 1
                MC_FWD_CTRL_Param.MACAddrGroup = EDD_MAC_ADDR_GROUP_DLR_1;
                Response = EDDP_GEN_Multicast_Fwd_Ctrl((EDD_UPPER_MULTICAST_FWD_CTRL_PTR_TYPE)&MC_FWD_CTRL_Param, pDDB);
                if (Response != EDD_STS_OK)
                {
                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMCopySetB2A(): FATAL -> DLR 1-frame forwarding could not be changed!");
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);        
                } 

                // DLR Group 2
                MC_FWD_CTRL_Param.MACAddrGroup = EDD_MAC_ADDR_GROUP_DLR_2;
                Response = EDDP_GEN_Multicast_Fwd_Ctrl((EDD_UPPER_MULTICAST_FWD_CTRL_PTR_TYPE)&MC_FWD_CTRL_Param, pDDB);
                if (Response != EDD_STS_OK)
                {
                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMCopySetB2A(): FATAL -> DSL 2-frame forwarding could not be changed!");
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                }                     

                // DLR Group 3
                MC_FWD_CTRL_Param.MACAddrGroup = EDD_MAC_ADDR_GROUP_DLR_3;
                Response = EDDP_GEN_Multicast_Fwd_Ctrl((EDD_UPPER_MULTICAST_FWD_CTRL_PTR_TYPE)&MC_FWD_CTRL_Param, pDDB);
                if (Response != EDD_STS_OK)
                {
                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMCopySetB2A(): FATAL -> DLR 3-frame forwarding could not be changed!");
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                }
#endif
            }
        }
        else
        {
            /* ---------------------------------------------------------------- */
            /* Handle MRP records                                               */
            /* ---------------------------------------------------------------- */
            if (pDDB->PRM.pRecordsetB->PDInterfaceMrpDataAdjust_IsPresent)
            {
                // MRP record is present
                for ( i=1; i <= pDDB->SWI.PortCnt; i++ )
                {
                    HwPortID = EDDP_SWI_get_HwPortID (pDDB, i);

                    if (pDDB->PRM.pRecordsetB->PDPortMrpDataAdjust[i].Record_IsPresent)
                    {
                        // this is a ring port -> set to 'Blocking'   
                        pDDB->SWI.LinkStatus[i].PortState = EDD_PORT_STATE_BLOCKING;
                        MRP_active = LSA_TRUE;
                    }
                    else
                    {
                        // this is not a ring port -> set to 'Forwarding'
                        pDDB->SWI.LinkStatus[i].PortState = EDD_PORT_STATE_FORWARDING;
                    }

                    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMCopySetB2A(UsrPortID=%d, HwPortID=%d): new PortState=0x%X", 
                        i, HwPortID, pDDB->SWI.LinkStatus[i].PortState);
                }
            }    
            else
            {
                // MRP record is not present: set default ring ports
                for ( i=1; i <= pDDB->SWI.PortCnt; i++ )
                {
                    HwPortID = EDDP_SWI_get_HwPortID (pDDB, i);

                    if (pDDB->SWI.LinkStatus[i].MRPRingPort == EDD_MRP_RING_PORT_DEFAULT)
                    {
                        // this is a ring port -> set to 'Blocking'   
                        pDDB->SWI.LinkStatus[i].PortState = EDD_PORT_STATE_BLOCKING;
                        MRP_active = LSA_TRUE;
                    }
                    else
                    {
                        // this is not a ring port -> set to 'Forwarding'
                        pDDB->SWI.LinkStatus[i].PortState = EDD_PORT_STATE_FORWARDING;
                    }

                    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMCopySetB2A(UsrPortID=%d, HwPortID=%d): new PortState=0x%X", 
                        i, HwPortID, pDDB->SWI.LinkStatus[i].PortState);
                }
            }

            // set forwarding for MRP multicast frames
            // there is no need for enabling forwarding because this is done at creating the MC-table anyway
            if (MRP_active)
            {
                MC_FWD_CTRL_Param.PortIDModeCnt = (LSA_UINT16)pDDB->SWI.PortCnt;
                for ( i=0; i < pDDB->SWI.PortCnt; i++ )
                {
                    MC_FWD_CTRL_Param.pPortIDModeArray[i].Mode      = EDD_MULTICAST_FWD_DISABLE;
                    MC_FWD_CTRL_Param.pPortIDModeArray[i].DstPortID = (LSA_UINT16)i+1;
                    MC_FWD_CTRL_Param.pPortIDModeArray[i].SrcPortID = (LSA_UINT16) EDD_PORT_ID_AUTO;
                }

                // MRP Group 1
                MC_FWD_CTRL_Param.MACAddrGroup = EDD_MAC_ADDR_GROUP_MRP_1;
                Response = EDDP_GEN_Multicast_Fwd_Ctrl((EDD_UPPER_MULTICAST_FWD_CTRL_PTR_TYPE)&MC_FWD_CTRL_Param, pDDB);
                if (Response != EDD_STS_OK)
                {
                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMCopySetB2A(): FATAL -> MRP MC-frame forwarding could not be changed!");
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);        
                } 

                // MRP Group 2
                MC_FWD_CTRL_Param.MACAddrGroup = EDD_MAC_ADDR_GROUP_MRP_2;
                Response = EDDP_GEN_Multicast_Fwd_Ctrl((EDD_UPPER_MULTICAST_FWD_CTRL_PTR_TYPE)&MC_FWD_CTRL_Param, pDDB);
                if (Response != EDD_STS_OK)
                {
                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMCopySetB2A(): FATAL -> MRP MC-frame forwarding could not be changed!");
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                }                     
            }

            // set forwarding for MRP interconnect multicast frames
            // there is no need for enabling forwarding because this is done at creating the MC-table anyway
            if (EDD_FEATURE_ENABLE == pDDB->FeatureSupport.MRPInterconnFwdRulesSupported)
            {
                MC_FWD_CTRL_Param.PortIDModeCnt = (LSA_UINT16)pDDB->SWI.PortCnt;
                for ( i=0; i < pDDB->SWI.PortCnt; i++ )
                {
                    MC_FWD_CTRL_Param.pPortIDModeArray[i].Mode      = EDD_MULTICAST_FWD_DISABLE;
                    MC_FWD_CTRL_Param.pPortIDModeArray[i].DstPortID = (LSA_UINT16)i+1;
                    MC_FWD_CTRL_Param.pPortIDModeArray[i].SrcPortID = (LSA_UINT16) EDD_PORT_ID_AUTO;
                }

                // MRP Interconnection Group 1
                MC_FWD_CTRL_Param.MACAddrGroup = EDD_MAC_ADDR_GROUP_MRP_IN_1;
                Response = EDDP_GEN_Multicast_Fwd_Ctrl((EDD_UPPER_MULTICAST_FWD_CTRL_PTR_TYPE)&MC_FWD_CTRL_Param, pDDB);
                if (Response != EDD_STS_OK)
                {
                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMCopySetB2A(): FATAL -> MRP IC MC-frame forwarding could not be changed!");
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);        
                } 

                // MRP Interconnection Group 2
                MC_FWD_CTRL_Param.MACAddrGroup = EDD_MAC_ADDR_GROUP_MRP_IN_2;
                Response = EDDP_GEN_Multicast_Fwd_Ctrl((EDD_UPPER_MULTICAST_FWD_CTRL_PTR_TYPE)&MC_FWD_CTRL_Param, pDDB);
                if (Response != EDD_STS_OK)
                {
                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMCopySetB2A(): FATAL -> MRP IC MC-frame forwarding could not be changed!");
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                }              

                // MRP Interconnection Group 3
                MC_FWD_CTRL_Param.MACAddrGroup = EDD_MAC_ADDR_GROUP_MRP_IN_3;
                Response = EDDP_GEN_Multicast_Fwd_Ctrl((EDD_UPPER_MULTICAST_FWD_CTRL_PTR_TYPE)&MC_FWD_CTRL_Param, pDDB);
                if (Response != EDD_STS_OK)
                {
                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMCopySetB2A(): FATAL -> MRP IC MC-frame forwarding could not be changed!");
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);        
                } 
            }
        }
    }

    /* ---------------------------------------------------------------- */
    /* Handle PDPortDataAjust record (has to be after MRP records)      */
    /* ---------------------------------------------------------------- */
    bSkip = LSA_TRUE;
    for ( i=1; i <= pDDB->SWI.PortCnt; i++ )
    {
        /* logic behind next if:
            bSkip will be LSA_TRUE in case that for all ports this rule applies:
            - skip if PDPortDataAdjust in Recordset A is equal to the one in Recordset B or
            - if there is no PDPortDataAdjust in Recordset A and no PDPortDataAdjust in Recordset B  
        */
        if (   (pDDB->PRM.pRecordsetA->PDPortDataAdjust[i].Record_IsPresent == pDDB->PRM.pRecordsetB->PDPortDataAdjust[i].Record_IsPresent)
            && (pDDB->PRM.pRecordsetB->PDPortDataAdjust[i].Record_IsPresent == pDDB->PRM.pRecordsetB->PDPortDataAdjust[i].Record_Skip) 
           )
        {
            // skip: same PortDataAdjust configuration for this port is already active or it has not changed
        }
        else
        {
            bSkip = LSA_FALSE;
        }
    }   

    if (bSkip)
    {
        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMCopySetB2A(): Skip PDPortDataAdjust configuration - it has not changed");
    }
    else
    {    
        // set all domain boundary filters back to default
        for ( j=0; j < EDDP_PRM_DOMAIN_BOUNDARY_COUNT; j++ )
        {            
            MAC_Address_Temp            = EDDP_MAC_DomainBoundary_1;
            MAC_Address_Temp.MacAdr[5]  = MAC_Address_Temp.MacAdr[5] + (LSA_UINT8)j;
            Index                       = EDDP_SWI_MC_Entry_Get_Index (pDDB, MAC_Address_Temp);
            EDDP_SWI_MC_Entry_Set_To_Default (pDDB, Index);

            MAC_Address_Temp            = EDDP_MAC_DomainBoundary_2;
            MAC_Address_Temp.MacAdr[5]  = MAC_Address_Temp.MacAdr[5] + (LSA_UINT8)j;
            Index                       = EDDP_SWI_MC_Entry_Get_Index (pDDB, MAC_Address_Temp);                
            EDDP_SWI_MC_Entry_Set_To_Default (pDDB, Index);

            MAC_Address_Temp            = EDDP_MAC_DomainBoundary_3;
            MAC_Address_Temp.MacAdr[5]  = MAC_Address_Temp.MacAdr[5] + (LSA_UINT8)j;
            Index = EDDP_SWI_MC_Entry_Get_Index (pDDB, MAC_Address_Temp);                
            EDDP_SWI_MC_Entry_Set_To_Default (pDDB, Index);

            MAC_Address_Temp            = EDDP_MAC_DomainBoundary_4;
            MAC_Address_Temp.MacAdr[5]  = MAC_Address_Temp.MacAdr[5] + (LSA_UINT8)j;
            Index                       = EDDP_SWI_MC_Entry_Get_Index (pDDB, MAC_Address_Temp);
            EDDP_SWI_MC_Entry_Set_To_Default (pDDB, Index);

            for ( i=1; i <= pDDB->SWI.PortCnt; i++ )
            {
                /* No announce boundary at any port
                */
                pDDB->SWI.AnnounceBoundary[i][j] = LSA_FALSE;
            }
        }

        /* ----------------------------------------------------------------------*/     
        /* set domain boundary                                                   */
        /* ----------------------------------------------------------------------*/     
        for ( i=1; i <= pDDB->SWI.PortCnt; i++ )
        {
            if (pDDB->PRM.PortDataAdjust.Parameters[i].DomainBoundaryIngressEgress_IsPresent)
            {
                for ( j=0; j < EDDP_PRM_DOMAIN_BOUNDARY_COUNT; j++ )
                {
                    bIngress = ( (pDDB->PRM.PortDataAdjust.Parameters[i].DomainBoundaryIngress >> j & 0x1) ? LSA_TRUE : LSA_FALSE );
                    bEgress  = ( (pDDB->PRM.PortDataAdjust.Parameters[i].DomainBoundaryEgress  >> j & 0x1) ? LSA_TRUE : LSA_FALSE );

                    MAC_Address_Temp            = EDDP_MAC_DomainBoundary_1;    // DomainBoundaryAnnounce
                    MAC_Address_Temp.MacAdr[5]  = MAC_Address_Temp.MacAdr[5] + (LSA_UINT8)j;
                    Index                       = EDDP_SWI_MC_Entry_Get_Index (pDDB, MAC_Address_Temp);
                    EDDP_SWI_MC_Entry_Set_DomainBoundary (pDDB, Index, i, bIngress, bEgress, LSA_TRUE);

                    MAC_Address_Temp            = EDDP_MAC_DomainBoundary_2;
                    MAC_Address_Temp.MacAdr[5]  = MAC_Address_Temp.MacAdr[5] + (LSA_UINT8)j;
                    Index                       = EDDP_SWI_MC_Entry_Get_Index (pDDB, MAC_Address_Temp);
                    EDDP_SWI_MC_Entry_Set_DomainBoundary (pDDB, Index, i, bIngress, bEgress, LSA_FALSE);

                    MAC_Address_Temp            = EDDP_MAC_DomainBoundary_3;
                    MAC_Address_Temp.MacAdr[5]  = MAC_Address_Temp.MacAdr[5] + (LSA_UINT8)j;
                    Index                       = EDDP_SWI_MC_Entry_Get_Index (pDDB, MAC_Address_Temp);
                    EDDP_SWI_MC_Entry_Set_DomainBoundary (pDDB, Index, i, bIngress, bEgress, LSA_FALSE);

                    MAC_Address_Temp            = EDDP_MAC_DomainBoundary_4;
                    MAC_Address_Temp.MacAdr[5]  = MAC_Address_Temp.MacAdr[5] + (LSA_UINT8)j;
                    Index                       = EDDP_SWI_MC_Entry_Get_Index (pDDB, MAC_Address_Temp);
                    EDDP_SWI_MC_Entry_Set_DomainBoundary (pDDB, Index, i, bIngress, bEgress, LSA_FALSE);
                    if (bIngress && bEgress)
                    {
                    	/* Set announce boundary for SyncId <j> at port <i>
                    	*/
                    	pDDB->SWI.AnnounceBoundary[i][j] = LSA_TRUE;
                    }
                }

                EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMCopySetB2A(UsrPortID=%d): PortDataAdjust.DomainBoundaryIngressEgress is present", i);
            }
        }
        for ( i = 1; i <= pDDB->SWI.PortCnt; i++ )
        {
            for ( j=0; j < EDDP_PRM_DOMAIN_BOUNDARY_COUNT; j++ )
            {
                if ((!pDDB->SWI.AnnounceBoundary[i][j]) && (0 != pDDB->SWI.LinkStatus[i].LineDelayInTicksMeasured))
                {
                    /* No announce boundary for SyncId <j> at port <i> and delay measured: (re)activate forwarding
                    */
                    MAC_Address_Temp            = EDDP_MAC_DomainBoundary_1;    // DomainBoundaryAnnounce
                    MAC_Address_Temp.MacAdr[5]  = MAC_Address_Temp.MacAdr[5] + (LSA_UINT8)j;
                    Index                       = EDDP_SWI_MC_Entry_Get_Index (pDDB, MAC_Address_Temp);
                    EDDP_SWI_ChangeAnnounceFwd(pDDB, Index, i, LSA_TRUE, (LSA_UINT8)j);
                    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMCopySetB2A(UsrPortID=%d): PortDataAdjust: Announce forwarding enabled for SyncId=%u", i, j);
                }
            }
        }

        /* ----------------------------------------------------------------------*/  
        /* set multicast boundary                                                */
        /* ----------------------------------------------------------------------*/     
        for ( i=1; i <= pDDB->SWI.PortCnt; i++ )
        {  
            // we set up each port separately
            MC_FWD_CTRL_Param.PortIDModeCnt                    = 1;
            MC_FWD_CTRL_Param.pPortIDModeArray[i-1].Mode       = EDD_MULTICAST_FWD_BITMASK;
            MC_FWD_CTRL_Param.pPortIDModeArray[i-1].DstPortID  = (LSA_UINT16)i;
            MC_FWD_CTRL_Param.pPortIDModeArray[i-1].SrcPortID  = (LSA_UINT16) EDD_PORT_ID_AUTO;
            MC_FWD_CTRL_Param.MACAddrGroup                     = EDD_MAC_ADDR_GROUP_RT_2_QUER;     
            
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMCopySetB2A(UsrPortID=%d): PortDataAdjust.MulticastBoundary_IsPresent(0x%X)", 
                i, pDDB->PRM.PortDataAdjust.Parameters[i].MulticastBoundary_IsPresent);

            if (pDDB->PRM.PortDataAdjust.Parameters[i].MulticastBoundary_IsPresent)
            {   
                // invert bitfield! '1' in record means "block"; '1' in service parameter means "forward"
                MC_FWD_CTRL_Param.MACAddrLow = ~pDDB->PRM.PortDataAdjust.Parameters[i].MulticastBoundary;                     
            }
            else
            {
                // 0xFFFFFFFF := forward all frames with MAC addresses of group EDD_MAC_ADDR_GROUP_RT_2_QUER to this port
                MC_FWD_CTRL_Param.MACAddrLow = 0xFFFFFFFF;                             
            }

            Response = EDDP_GEN_Multicast_Fwd_Ctrl ((EDD_UPPER_MULTICAST_FWD_CTRL_PTR_TYPE)&MC_FWD_CTRL_Param, pDDB);

            if (Response != EDD_STS_OK)
            {
                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMCopySetB2A(): FATAL -> PDPortDataAdjust: MC-boundary could not be set!");
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
        }

        // set all DCP boundary filters back to default
        MAC_Address_Temp    = EDDP_MAC_DCPBoundary_Identify;
        Index               = EDDP_SWI_MC_Entry_Get_Index (pDDB, MAC_Address_Temp);
        EDDP_SWI_MC_Entry_Set_To_Default (pDDB, Index);

        MAC_Address_Temp    = EDDP_MAC_DCPBoundary_Hello;
        Index               = EDDP_SWI_MC_Entry_Get_Index (pDDB, MAC_Address_Temp);
        EDDP_SWI_MC_Entry_Set_To_Default (pDDB, Index);                                             

        /* ----------------------------------------------------------------------*/     
        /* set DCP boundary                                                      */
        /* ----------------------------------------------------------------------*/     
        for ( i=1; i <= pDDB->SWI.PortCnt; i++ )
        {
            if (pDDB->PRM.PortDataAdjust.Parameters[i].DCPBoundary_IsPresent)
            {
                bIngress = LSA_FALSE;   // DCP has no ingress filter

                // set DCP boundary for 'Identify' MC-frames (Bit 0 in DCPBoundary)
                bEgress = ( (pDDB->PRM.PortDataAdjust.Parameters[i].DCPBoundary >> 0 & 0x1) ? LSA_TRUE : LSA_FALSE );

                MAC_Address_Temp    = EDDP_MAC_DCPBoundary_Identify;
                Index               = EDDP_SWI_MC_Entry_Get_Index (pDDB, MAC_Address_Temp);
                EDDP_SWI_MC_Entry_Set_DomainBoundary (pDDB, Index, i, bIngress, bEgress, LSA_FALSE);

                // set DCP boundary for 'Hello' MC-frames (Bit 1 in DCPBoundary)
                bEgress = ( (pDDB->PRM.PortDataAdjust.Parameters[i].DCPBoundary >> 1 & 0x1) ? LSA_TRUE : LSA_FALSE );

                MAC_Address_Temp    = EDDP_MAC_DCPBoundary_Hello;
                Index               = EDDP_SWI_MC_Entry_Get_Index (pDDB, MAC_Address_Temp);
                EDDP_SWI_MC_Entry_Set_DomainBoundary (pDDB, Index, i, bIngress, bEgress, LSA_FALSE);
                
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMCopySetB2A(UsrPortID=%d): PortDataAdjust.DCPBoundary(0x%X) is present", 
                    i, pDDB->PRM.PortDataAdjust.Parameters[i].DCPBoundary);
            }
        }
    }

    /* ---------------------------------------------------------------- */
    /* set MAUType and/or Link State                                    */
    /* ---------------------------------------------------------------- */
    //index is UsrPortIndex
    for ( i=1; i <= pDDB->SWI.PortCnt; i++ )
    {
        LSA_UINT8  LocalLinkSpeedModeConfigured = 0;

        //The transceiver gets setup regardless of its pulled/plugged state
        HwPortID = EDDP_SWI_get_HwPortID (pDDB, i);

        if (pDDB->PRM.PortDataAdjust.Parameters[i].MAUType_IsPresent)
        {
            //the only valid MAUType for PNIP is 100MBFD!
            pDDB->SWI.LinkStatus[i].MAUTypeParam = pDDB->PRM.PortDataAdjust.Parameters[i].MAUType;
            pDDB->SWI.LinkStatus[i].LinkSpeedModeConfiguredParam = EDD_LINK_100MBIT_FULL;
        }
        else
        {
            pDDB->SWI.LinkStatus[i].LinkSpeedModeConfiguredParam = EDD_LINK_AUTONEG;
        }

        if (0 != pDDB->PRM.ApplyDefaultPortParams[i-1 /*UsrPortIndex*/]) 
        {
            //Apply default port params is requested by CM
            LocalLinkSpeedModeConfigured = (EDD_MEDIATYPE_FIBER_OPTIC_CABLE == pDDB->PRM.PortDataAdjust.Parameters[i].MediaType)?EDD_LINK_100MBIT_FULL:EDD_LINK_AUTONEG;
        }
        else
        {
            //replace AUTONEG by 100MBFD for optical ports!
            LocalLinkSpeedModeConfigured = (EDD_MEDIATYPE_FIBER_OPTIC_CABLE == pDDB->PRM.PortDataAdjust.Parameters[i].MediaType)?EDD_LINK_100MBIT_FULL:pDDB->SWI.LinkStatus[i].LinkSpeedModeConfiguredParam;
        }

        // first COMMIT or phy setting changed => set PHY
        if (EDD_LINK_AUTONEG != LocalLinkSpeedModeConfigured)
        {
            // set Speed and Duplexity (always 100Mbit, Full Duplex)
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMCopySetB2A(UsrPortID=%d, HwPortID=%d): PortDataAdjust.MAUType has changed - set Link(100MBit), Duplexity(Full), Autoneg(OFF)", 
                i, HwPortID);

            Response = pDDB->SWI.PHY_FunctionPointers[HwPortID].EDDP_PHY_SetSpeedDuplexityOrAutoneg(pDDB->hDDB, pDDB->hSysDev, HwPortID, 
                                                                                                            &LinkSpeedModeConfigured,EDD_LINK_SPEED_100,
                                                                                                            EDD_LINK_MODE_FULL, LSA_FALSE);
            EDDP_ASSERT_FALSE(Response != EDD_STS_OK);

            pDDB->SWI.LinkStatus[i].LinkSpeedModeConfigured      = LinkSpeedModeConfigured;
            pDDB->SWI.LinkStatus[i].Autoneg = EDD_AUTONEG_OFF;
        }
        else 
        {
            // set PHY to Autonegotiation
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMCopySetB2A(UsrPortID=%d, HwPortID=%d): PortDataAdjust.MAUType has changed - set Link(Autoneg), Autoneg(ON)", 
                i, HwPortID);

            Response = pDDB->SWI.PHY_FunctionPointers[HwPortID].EDDP_PHY_SetSpeedDuplexityOrAutoneg(pDDB->hDDB, pDDB->hSysDev, HwPortID,
                                                                                                            &LinkSpeedModeConfigured,EDD_LINK_UNKNOWN,
                                                                                                            EDD_LINK_UNKNOWN, LSA_TRUE);
            EDDP_ASSERT_FALSE(Response != EDD_STS_OK);

            pDDB->SWI.LinkStatus[i].LinkSpeedModeConfigured = LinkSpeedModeConfigured;
            pDDB->SWI.LinkStatus[i].Autoneg                 = EDD_AUTONEG_ON;
        }
    }

    /* -------------------------------------------------------------------- */
    /* Set ports and PHYs according to parameterisation or default values   */
    /* -------------------------------------------------------------------- */
    for ( i=1; i <= pDDB->SWI.PortCnt; i++ )
    {
        LSA_UINT32 HwPortIndex;

        HwPortID = EDDP_SWI_get_HwPortID (pDDB, i);
        HwPortIndex = HwPortID - 1;

        // Check if the PHY has to be disabled/enabled (check LinkState in PortDataAdjust)
        if (   (pDDB->PRM.PortDataAdjust.Parameters[i].LinkState_IsPresent)
            && (pDDB->PRM.PortDataAdjust.Parameters[i].LinkState == EDDP_PRM_LINK_STATE_DOWN)
           )
        {
            // disable PHY (turn power off)
            // check if PHY is already disabled. Always set PHY on first PrmCommit.
            if ((EDD_PHY_STATUS_DISABLED != pDDB->SWI.LinkStatus[i].PhyStatus) || (!pPrm->FirstCommitExecuted))
            {
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMCopySetB2A(UsrPortID=%d, HwPortID=%d): PortDataAdjust.PHY has changed - disable PHY", 
                    i, HwPortID);

                if (EDDP_PRMChangePortState_IsPlugged(pDDB, HwPortIndex))
                {
                    //Transceiver is only set in PowerDown State if plugged
                    Response = pDDB->SWI.PHY_FunctionPointers[HwPortID].EDDP_PHY_SetPowerDown(pDDB->hDDB, pDDB->hSysDev, HwPortID, LSA_TRUE);
                    EDDP_ASSERT_FALSE(Response != EDD_STS_OK);
                }

                pDDB->SWI.LinkStatus[i].PhyStatus   = EDD_PHY_STATUS_DISABLED;
                pDDB->SWI.LinkStatus[i].LinkStatus  = EDD_LINK_DOWN;
            }
            else
            {
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMCopySetB2A(UsrPortID=%d, HwPortID=%d): PortDataAdjust.PHY has not changed - PHY is already disabled", 
                    i, HwPortID);
            }
        }
        else
        {
            // enable PHY (turn power on)
            // check if PHY is already enabled. Always set PHY on first PrmCommit.
            if ((EDD_PHY_STATUS_ENABLED != pDDB->SWI.LinkStatus[i].PhyStatus) || (!pPrm->FirstCommitExecuted))
            {
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMCopySetB2A(UsrPortID=%d, HwPortID=%d): PortDataAdjust.PHY has changed - enable PHY", 
                    i, HwPortID);

                if (EDDP_PRMChangePortState_IsPlugged(pDDB, HwPortIndex))
                {
                    //Transceiver is only set in PowerUp State if plugged
                    Response = pDDB->SWI.PHY_FunctionPointers[HwPortID].EDDP_PHY_SetPowerDown(pDDB->hDDB, pDDB->hSysDev, HwPortID, LSA_FALSE);
                    if(Response != EDD_STS_OK)
                    {
                    	EDDP_ASSERT_FALSE(Response != EDD_STS_OK);
                    }
                }

                pDDB->SWI.LinkStatus[i].PhyStatus = EDD_PHY_STATUS_ENABLED;
                // LinkStatus = LinkUP will be set on ISR_LinkChange() if Link is present!
            }
            else
            {
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMCopySetB2A(UsrPortID=%d, HwPortID=%d): PortDataAdjust.PHY has not changed - PHY is already enabled", 
                    i, HwPortID);
            }
        }

        // fire link indication for this PortID
        EDDP_GENIndicateLinkChange (pDDB, i);
    }         

    // check if "fastest link" status has changed
    if (EDDP_GENFindFastestPort(pDDB))
    {
        EDDP_GENIndicateLinkChange (pDDB, EDD_PORT_ID_AUTO);
    }

    /* -------------------------------------------------------------------- */
    /* delete DiagPendingList                                               */
    /* CM-PD deletes all fired Appear-Diagnosis                             */
    /* -------------------------------------------------------------------- */
    // MrtDiagPendingList
    pPrm->MrtDiagPendingGroup.List.BeginIdx     = EDDP_PRM_CHAIN_IDX_END;
    pPrm->MrtDiagPendingGroup.List.EndIdx       = EDDP_PRM_CHAIN_IDX_END;
    pPrm->MrtDiagPendingGroup.PDIRFrameDataCnt  = 0;
    // LateErrDiagPendingGroup
    pPrm->LateErrDiagPendingGroup.List.BeginIdx = EDDP_PRM_CHAIN_IDX_END;
    pPrm->LateErrDiagPendingGroup.List.EndIdx   = EDDP_PRM_CHAIN_IDX_END;
    pPrm->LateErrDiagPendingGroup.PackGrpCnt    = 0;

    /* -------------------------------------------------------------------- */
    /* Swap the content of Record(B) and Record(A)                          */
    /* From now only the Record(A) is valid and the Record(B) is free       */
    /* for new parametrization                                              */
    /* -------------------------------------------------------------------- */
    // PDIRData - swap the pointers pPDIRData_A and pPDIRData_B
    if (pDDB->PRM.pRecordsetB->PDIRData_IsPresent)
    {
        pPDIRData_Temp          = pDDB->PRM.pPDIRData_A;
        pDDB->PRM.pPDIRData_A   = pDDB->PRM.pPDIRData_B;
        pDDB->PRM.pPDIRData_B   = pPDIRData_Temp;
    }
    // PDIRApplicationData - swap the pointers pPDIRApplicationData_A and pPDIRApplicationData_B
    if (pDDB->PRM.pRecordsetB->PDIRApplicationData_IsPresent)
    {
        pPDIRApplicationData_Temp           = pDDB->PRM.pPDIRApplicationData_A;
        pDDB->PRM.pPDIRApplicationData_A    = pDDB->PRM.pPDIRApplicationData_B;
        pDDB->PRM.pPDIRApplicationData_B    = pPDIRApplicationData_Temp;
    }
    // PDIRSubframeData- swap the pointers pPDIRSubframeData_A and pPDIRSubframeData_B
    if (pDDB->PRM.pRecordsetB->PDIRSubframeData_IsPresent)
    {
        pPDIRSubframeData_Temp           = pDDB->PRM.pPDIRSubframeData_A;
        pDDB->PRM.pPDIRSubframeData_A    = pDDB->PRM.pPDIRSubframeData_B;
        pDDB->PRM.pPDIRSubframeData_B    = pPDIRSubframeData_Temp;
    }
    // PDNRTFeedInLoadLimitation- swap the pointers pPDNRTFeedInLoadLimitation_A and pPDNRTFeedInLoadLimitation_B
    if (pDDB->PRM.pRecordsetB->PDNRTFeedInLoadLimitation_IsPresent)
    {
        pPDNRTFeedInLoadLimitation_Temp         = pDDB->PRM.pPDNRTFeedInLoadLimitation_A;
        pDDB->PRM.pPDNRTFeedInLoadLimitation_A  = pDDB->PRM.pPDNRTFeedInLoadLimitation_B;
        pDDB->PRM.pPDNRTFeedInLoadLimitation_B  = pPDNRTFeedInLoadLimitation_Temp;
    }
    // Recordset: always after swap of "pPDIRData_A" and "pPDIRSubframeData_A" - swap the pointers pRecordsetA and pRecordsetB
    pRecordset_Temp          = pDDB->PRM.pRecordsetA;
    pDDB->PRM.pRecordsetA    = pDDB->PRM.pRecordsetB;
    pDDB->PRM.pRecordsetB    = pRecordset_Temp;

    // at least the first PrmCommit is executed
    pPrm->FirstCommitExecuted = LSA_TRUE;

    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMCopySetB2A()");
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMOpenChannel                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB           +*/
/*+                        :    EDDP_LOCAL_HDB_PTR_TYPE      pHDB           +*/
/*+                             LSA_HANDLE_TYPE              Handle         +*/
/*+                             EDDP_SYSTEM_CDB_PTR_TYPE     pCDB           +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                    (in) +*/
/*+  Handle     : Handle for this Channel (<  EDD_CFG_MAX_CHANNELS)    (in) +*/
/*+  pCDB       : Pointer to ChannelDescriptionBlock                   (in) +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_CHANNEL_USE                                   +*/
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Opens Channel for PRM. The Handle will specify the Channel+*/
/*+               and pDDB the Device. pCDB has all parameters              +*/
/*+               for this channel                                          +*/
/*+                                                                         +*/
/*+       Note:   EDD_STS_OK_PENDING not allowed as response. This call     +*/
/*+               must be synchronously!                                    +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMOpenChannel(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB,
    LSA_HANDLE_TYPE             Handle,
    EDDP_SYSTEM_CDB_PTR_TYPE    pCDB)
{
    EDD_RSP                         Response;
    EDDP_LOCAL_HDB_PRM_PTR_TYPE     pHDB_PRM_PoolItem;

#if (EDDP_CFG_TRACE_MODE ==  0)
    LSA_UNUSED_ARG(pHDB);
#endif

    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_PRMOpenChannel(Handle=0x%X, pCDB=0x%X)", 
        pHDB->Handle, Handle, pCDB);

    Response = EDD_STS_OK;

    /* Param check */
    if (LSA_HOST_PTR_ARE_EQUAL(pCDB->pPRM, LSA_NULL))
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMOpenChannel(): ERROR -> pCDB->pPRM is NULL! Response=0x%X", 
            Handle, Response);
    }
    else
    {
        /* Only one channel can use PRM services! */
        if (pDDB->PRM.HandleCnt == 0)
        {
            pDDB->PRM.HandleCnt                         = 1;
            pDDB->PRM.Handle                            = Handle;
            pDDB->PRM.EDD_Handle                        = pHDB;
            pDDB->PRM.PortDataAdjust.LesserCheckQuality = (EDD_SYS_PDPORTDATAADJUST_LESSER_CHECK_QUALITY == pCDB->pPRM->PortDataAdjustLesserCheckQuality)?LSA_TRUE:LSA_FALSE;

            #ifdef EDDP_CFG_LOCAL_MEM_STATIC
            pHDB_PRM_PoolItem = &g_EDDP_HDB_PRM[pHDB->Handle];                          // use static RAM
            #else
            pHDB_PRM_PoolItem = EDDP_GlbAllocLocalMem (sizeof(EDDP_HDB_PRM_TYPE));      // alloc Mgmt-data in Heap

            // check Alloc successful ?
            if (LSA_HOST_PTR_ARE_EQUAL(pHDB_PRM_PoolItem, LSA_NULL))
            {
                Response = EDD_STS_ERR_RESOURCE;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMOpenChannel(): ERROR -> AllocLocalMem of pHDB->pPRM(0x%X) is not Ok! Response=0x%X", 
                    pHDB->Handle, pHDB_PRM_PoolItem, Response);
            }
            #endif

            pHDB->pPRM = pHDB_PRM_PoolItem;

            if (Response == EDD_STS_OK)
            {
                // init RunningRQBCount of this new channel: it will be checked at PRMCloseChannel
                pHDB->pPRM->RunningRQBCount = 0;
            }
        }
        else
        {
            Response = EDD_STS_ERR_CHANNEL_USE;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMOpenChannel(): ERROR -> PRM is already in use by another Channel(0x%X)! Response=0x%X",
                Handle, pDDB->PRM.Handle, Response);
        }
    }

    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMOpenChannel(): Response=0x%X", Response);

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMRequestPrmRead                      +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                         +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         Opcode:     EDD_OPC_REQUEST                 +*/
/*+     LSA_HANDLE_TYPE         Handle:                                     +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user (not used)           +*/
/*+     EDD_SERVICE             Service:    EDD_SRV_PRM_READ                +*/
/*+     LSA_RESPONSE_TYPE       Response:   Return status                   +*/
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:                                     +*/
/*+                                                                         +*/
/*+  pParam points to EDD_UPPER_PRM_WRITE_PTR_TYPE                          +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_SEQUENCE                                      +*/
/*+               EDD_STS_ERR_PRM_INDEX                                     +*/
/*+               EDD_STS_ERR_PRM_DATA                                      +*/
/*+               EDD_STS_ERR_PRM_BLOCK                                     +*/
/*+               EDD_STS_ERR_PRM_PORTID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function handles the EDD_SRV_PRM_READ request        +*/
/*+                                                                         +*/
/*+               Note: Opcode/Service not checked!                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMRequestPrmRead(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP	                        Response;
    EDD_UPPER_PRM_READ_PTR_TYPE     pPrmRead;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pHDB);
#endif

    Response    = EDD_STS_OK;
    pPrmRead    = (EDD_UPPER_PRM_READ_PTR_TYPE) pRQB->pParam;

    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_PRMRequestPrmRead(pRQB=0x%X)", pHDB->Handle, pRQB);

    if (LSA_HOST_PTR_ARE_EQUAL(pRQB->pParam, LSA_NULL))
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmRead(): ERROR -> RQB->pParam=NULL, Response=0x%X", 
            pHDB->Handle, Response);
    }
    else if ((pPrmRead->edd_port_id != EDD_PORT_ID_AUTO) && (pPrmRead->edd_port_id > pDDB->SWI.PortCnt))
    {
        Response = EDD_STS_ERR_PRM_PORTID;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmRead(): ERROR ->   edd_port_id(%d) is neither EDD_PORT_ID_AUTO nor < %d!",
            pHDB->Handle, pPrmRead->edd_port_id, pDDB->SWI.PortCnt);
    }
    else if ((pPrmRead->edd_port_id != EDD_PORT_ID_AUTO) && EDDP_PRMChangePortState_IsPulled(pDDB, EDDP_SWI_get_HwPortID(pDDB, pPrmRead->edd_port_id) - 1))
    {
        //Reads from portspecific records are rejected if port submodule is pulled.
        Response = EDD_STS_ERR_PRM_PORTID;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H:%2X] EDDP_PRMRequestPrmRead():  Record 0x%X from UsrPortid %u not readable for pulled submodule", 
            pHDB->Handle, pPrmRead->record_index, pPrmRead->edd_port_id);
    }
    else
    {
        switch (pPrmRead->record_index)
        {
            /* ---------------------------------------- */
            /* PDNCDataCheck                            */
            /* ---------------------------------------- */
            case EDDP_PRM_PDNC_DATA_CHECK_RECORD_INDEX:
            {
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmRead(): PDNCDataCheck(0x%X)", 
                    pHDB->Handle, pPrmRead->record_index);

                if (pPrmRead->edd_port_id == EDD_PORT_ID_AUTO)
                {
                    if (pDDB->PRM.pRecordsetA->PDNCDataCheck_IsPresent)
                    {
                        if (pPrmRead->record_data_length < pDDB->PRM.pRecordsetA->PDNCDataCheck_Length)
                        {
                            Response = EDD_STS_ERR_PRM_DATA;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmRead(): ERROR -> PrmRecord PDNCDataCheck: record_data_length(%d) < PDNCDataCheck_Length(%d)", 
                                pHDB->Handle, pPrmRead->record_data_length, pDDB->PRM.pRecordsetA->PDNCDataCheck_Length);
                        }
                        else
                        {
                            // copy record into RQB
                            EDDP_MEMCOPY_COMMON(pPrmRead->record_data, pDDB->PRM.pRecordsetA->pPDNCDataCheck, pDDB->PRM.pRecordsetA->PDNCDataCheck_Length);
                            pPrmRead->record_data_length = pDDB->PRM.pRecordsetA->PDNCDataCheck_Length;
                        }
                    }
                    else
                    {
                        // no record of this type has been written previously
                        pPrmRead->record_data_length = 0;
                        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmRead(): PDNCDataCheck(0x%X) is not present!", 
                            pHDB->Handle, pPrmRead->record_index);
                    }
                }
                else
                {
                    Response = EDD_STS_ERR_PRM_PORTID;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmRead(): ERROR -> PrmRecord PDNCDataCheck: edd_port_id(%d) is not EDD_PORT_ID_AUTO(%d)", 
                        pHDB->Handle, pPrmRead->edd_port_id, EDD_PORT_ID_AUTO);
                }
            }
            break;

            /* ---------------------------------------- */
            /* PDIRData                                 */
            /* ---------------------------------------- */
            case EDDP_PRM_PDIRDATA_RECORD_INDEX:
            {
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmRead(): PDIRData(0x%X)", 
                    pHDB->Handle, pPrmRead->record_index);

                if (pPrmRead->edd_port_id == EDD_PORT_ID_AUTO)
                {
                    if (pDDB->PRM.pRecordsetA->PDIRData_IsPresent)
                    {
                        if (pPrmRead->record_data_length < pDDB->PRM.pRecordsetA->PDIRData_Length)
                        {
                            Response = EDD_STS_ERR_PRM_DATA;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmRead(): ERROR -> PrmRecord PDIRData: record_data_length(%d) < PDIRData_Length(%d)", 
                                pHDB->Handle, pPrmRead->record_data_length, pDDB->PRM.pRecordsetA->PDIRData_Length);
                        }
                        else
                        {
                            // copy record into RQB
                            EDDP_MEMCOPY_COMMON(pPrmRead->record_data, pDDB->PRM.pRecordsetA->pPDIRData, pDDB->PRM.pRecordsetA->PDIRData_Length);
                            pPrmRead->record_data_length = pDDB->PRM.pRecordsetA->PDIRData_Length;
                        }
                    }
                    else
                    {
                        // no record of this type has been written previously
                        pPrmRead->record_data_length = 0;
                        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmRead(): PDIRData(0x%X) is not present!", 
                            pHDB->Handle, pPrmRead->record_index);
                    }
                }
                else
                {
                    Response = EDD_STS_ERR_PRM_PORTID;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmRead(): ERROR -> PrmRecord PDIRData: edd_port_id(%d) is not EDD_PORT_ID_AUTO(%d)", 
                        pHDB->Handle, pPrmRead->edd_port_id, EDD_PORT_ID_AUTO);
                }
            }
            break;

            /* ---------------------------------------- */
            /* PDIRSubframeData                         */
            /* ---------------------------------------- */
            case EDDP_PRM_PDIRSUBFRAMEDATA_RECORD_INDEX:
            {
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmRead(): PDIRSubframeData(0x%X)", 
                    pHDB->Handle, pPrmRead->record_index);

                if (pPrmRead->edd_port_id == EDD_PORT_ID_AUTO)
                {
                    if (pDDB->PRM.pRecordsetA->PDIRSubframeData_IsPresent)
                    {
                        if (pPrmRead->record_data_length < pDDB->PRM.pRecordsetA->PDIRSubframeData_Length)
                        {
                            Response = EDD_STS_ERR_PRM_DATA;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmRead(): ERROR -> PrmRecord PDIRSubframeData: record_data_length(%d) < PDIRSubframeData_Length(%d)", 
                                pHDB->Handle, pPrmRead->record_data_length, pDDB->PRM.pRecordsetA->PDIRSubframeData_Length);
                        }
                        else
                        {
                            // copy record into RQB
                            EDDP_MEMCOPY_COMMON(pPrmRead->record_data, pDDB->PRM.pRecordsetA->pPDIRSubframeData, pDDB->PRM.pRecordsetA->PDIRSubframeData_Length);
                            pPrmRead->record_data_length = pDDB->PRM.pRecordsetA->PDIRSubframeData_Length;
                        }
                    }
                    else
                    {
                        // no record of this type has been written previously
                        pPrmRead->record_data_length = 0;
                        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmRead(): PDIRSubframeData(0x%X) is not present!", 
                            pHDB->Handle, pPrmRead->record_index);
                    }
                }
                else
                {
                    Response = EDD_STS_ERR_PRM_PORTID;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmRead(): ERROR -> PrmRecord PDIRSubframeData: edd_port_id(%d) is not EDD_PORT_ID_AUTO(%d)", 
                        pHDB->Handle, pPrmRead->edd_port_id, EDD_PORT_ID_AUTO);
                }
            }
            break;

            /* ---------------------------------------- */
            /* PDSyncData                               */
            /* ---------------------------------------- */
            case EDDP_PRM_PDSYNCDATA_RECORD_INDEX:
            {
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmRead(): PDSyncData(0x%X)", 
                    pHDB->Handle, pPrmRead->record_index);

                if (pPrmRead->edd_port_id == EDD_PORT_ID_AUTO)
                {
                    if (pDDB->PRM.pRecordsetA->PDSyncData_IsPresent)
                    {	        
                        if (pPrmRead->record_data_length < pDDB->PRM.pRecordsetA->PDSyncData_Length)
                        {
                            Response = EDD_STS_ERR_PRM_DATA;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmRead(): ERROR -> PrmRecord PDSyncData: record_data_length(%d) < PDSyncData_Length(%d)", 
                                pHDB->Handle, pPrmRead->record_data_length, pDDB->PRM.pRecordsetA->PDSyncData_Length);	            
                        }
                        else
                        {
                            // copy record into RQB
                            EDDP_MEMCOPY_COMMON(pPrmRead->record_data, pDDB->PRM.pRecordsetA->pPDSyncData, pDDB->PRM.pRecordsetA->PDSyncData_Length);
                            pPrmRead->record_data_length = pDDB->PRM.pRecordsetA->PDSyncData_Length;
                        }
                    }
                    else
                    {
                        // no record of this type has been written previously
                        pPrmRead->record_data_length = 0;
                        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmRead(): PDSyncData(0x%X) is not present!", 
                            pHDB->Handle, pPrmRead->record_index);
                    }
                }
                else
                {
                    Response = EDD_STS_ERR_PRM_PORTID;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmRead(): ERROR -> PrmRecord PDSyncData: edd_port_id(%d) is not EDD_PORT_ID_AUTO(%d)!", 
                        pHDB->Handle, pPrmRead->edd_port_id, EDD_PORT_ID_AUTO);
                }
            }
            break;

            /* ---------------------------------------- */
            /* PDPortDataAdjust                         */
            /* ---------------------------------------- */
            case EDDP_PRM_PDPORT_DATA_ADJUST_RECORD_INDEX:
            {
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmRead(): PDPortDataAdjust(0x%X)", 
                    pHDB->Handle, pPrmRead->record_index);

                if ((pPrmRead->edd_port_id > EDD_PORT_ID_AUTO) && (pPrmRead->edd_port_id <= pDDB->SWI.PortCnt))
                {
                    if (pDDB->PRM.pRecordsetA->PDPortDataAdjust[pPrmRead->edd_port_id].Record_IsPresent)
                    {
                        if (pPrmRead->record_data_length < pDDB->PRM.pRecordsetA->PDPortDataAdjust[pPrmRead->edd_port_id].Record_Length)
                        {
                            Response = EDD_STS_ERR_PRM_DATA;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmRead(): ERROR -> PrmRecord PDPortDataAdjust: record_data_length(%d) < Record_Length(%d)", 
                                pHDB->Handle, pPrmRead->record_data_length, pDDB->PRM.pRecordsetA->PDPortDataAdjust[pPrmRead->edd_port_id].Record_Length);
                        }
                        else
                        {
                            // copy record into RQB
                            EDDP_MEMCOPY_COMMON(pPrmRead->record_data, pDDB->PRM.pRecordsetA->PDPortDataAdjust[pPrmRead->edd_port_id].pRecord, pDDB->PRM.pRecordsetA->PDPortDataAdjust[pPrmRead->edd_port_id].Record_Length);
                            pPrmRead->record_data_length = pDDB->PRM.pRecordsetA->PDPortDataAdjust[pPrmRead->edd_port_id].Record_Length;
                        }
                    }
                    else
                    {
                        // no record of this type has been written previously
                        pPrmRead->record_data_length = 0;
                        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmRead(): PDPortDataAdjust(0x%X) is not present!", 
                            pHDB->Handle, pPrmRead->record_index);
                    }
                }
                else
                {
                    Response = EDD_STS_ERR_PRM_PORTID;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmRead(): ERROR -> PrmRecord PDPortDataAdjust: edd_port_id(%d) is not between 1..%d!", 
                        pHDB->Handle, pPrmRead->edd_port_id, pDDB->SWI.PortCnt);
                }
            }
            break;

            /* ---------------------------------------- */
            /* PDPortStatistic                          */
            /* ---------------------------------------- */
            case EDDP_PRM_PDPORT_STATISTIC_RECORD_INDEX:
            {
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmRead(): PDPortStatistic(0x%X)", 
                    pHDB->Handle, pPrmRead->record_index);

                if (pPrmRead->record_data_length < EDDP_PRM_PDPORT_STATISTIC_RECORD_LENGTH)
                {
                    Response = EDD_STS_ERR_PRM_DATA;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmRead(): ERROR -> PrmRecord PDPortStatistic: record_data_length(%d) < EDDP_PRM_PDPORT_STATISTIC_RECORD_LENGTH(%d)", 
                        pHDB->Handle, pPrmRead->record_data_length, EDDP_PRM_PDPORT_STATISTIC_RECORD_LENGTH);
                }
                else
                {
                    // copy record into RQB
                    Response = EDDP_PRMPDPortStatisticRecord_Read (pRQB, pDDB, pHDB);
                    pPrmRead->record_data_length = EDDP_PRM_PDPORT_STATISTIC_RECORD_LENGTH;
                }
            }
            break;

            /* ---------------------------------------- */
            /* PDSCFDataCheck                           */
            /* ---------------------------------------- */
            case EDDP_PRM_PDSCF_DATA_CHECK_RECORD_INDEX:
            {
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmRead(): PDSCFDataCheck(0x%X)", 
                    pHDB->Handle, pPrmRead->record_index);

                if (pDDB->PRM.PDSCFDataCheck.PDSCFDataCheck_IsPresent)
                {
                    if (pPrmRead->record_data_length < pDDB->PRM.PDSCFDataCheck.PDSCFDataCheck_Length)
                    {
                        Response = EDD_STS_ERR_PRM_DATA;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmRead(): ERROR -> PrmRecord PDSCFDataCheck: record_data_length(%d) < PDSCFDataCheck_Length(%d)", 
                            pHDB->Handle, pPrmRead->record_data_length, pDDB->PRM.PDSCFDataCheck.PDSCFDataCheck_Length);
                    }
                    else
                    {
                        // copy record into RQB
                        EDDP_MEMCOPY_COMMON(pPrmRead->record_data, pDDB->PRM.PDSCFDataCheck.pPDSCFDataCheck, pDDB->PRM.PDSCFDataCheck.PDSCFDataCheck_Length);
                        pPrmRead->record_data_length = pDDB->PRM.PDSCFDataCheck.PDSCFDataCheck_Length;
                    }
                }
                else
                {
                    // no record of this type has been written previously
                    pPrmRead->record_data_length = 0;
                    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmRead(): PDSCFDataCheck(0x%X) is not present!", 
                        pHDB->Handle, pPrmRead->record_index);
                }
            }
            break;

            /* ---------------------------------------- */
            /* PDIRApplicationData                      */
            /* ---------------------------------------- */
            case EDDP_PRM_PDIRAPPLICATIONDATA_RECORD_INDEX:
            {
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmRead(): PDIRApplicationData(0x%X)", 
                    pHDB->Handle, pPrmRead->record_index);

                if (pDDB->PRM.pRecordsetA->PDIRApplicationData_IsPresent)
                {
                    if (pPrmRead->record_data_length < pDDB->PRM.pRecordsetA->PDIRApplicationData_Length)
                    {
                        Response = EDD_STS_ERR_PRM_DATA;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmRead(): ERROR -> PrmRecord PDIRApplicationData: record_data_length(%d) < PDIRApplicationData_Length(%d)", 
                            pHDB->Handle, pPrmRead->record_data_length, pDDB->PRM.pRecordsetA->PDIRApplicationData_Length);
                    }
                    else
                    {
                        // copy record into RQB
                        EDDP_MEMCOPY_COMMON(pPrmRead->record_data, pDDB->PRM.pRecordsetA->pPDIRApplicationData, pDDB->PRM.pRecordsetA->PDIRApplicationData_Length);
                        pPrmRead->record_data_length = pDDB->PRM.pRecordsetA->PDIRApplicationData_Length;
                    }
                }
                else
                {
                    // no record of this type has been written previously
                    pPrmRead->record_data_length = 0;
                    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmRead(): PDIRApplicationData(0x%X) is not present!", 
                        pHDB->Handle, pPrmRead->record_index);
                }
            }
            break;
            
            /* ---------------------------------------- */
            /* PDNRTFeedInLoadLimitation                */
            /* ---------------------------------------- */
            case EDDP_PRM_PDNRT_FEED_IN_LOAD_LIMITATION_RECORD_INDEX:
            {
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmRead(): PDNRTFeedInLoadLimitation(0x%X)", 
                    pHDB->Handle, pPrmRead->record_index);

                if (pDDB->PRM.pRecordsetA->PDNRTFeedInLoadLimitation_IsPresent)
                {
                    // check "edd_port_id" - only interface allowed
                    if( 0 != pPrmRead->edd_port_id )
                    {
                        Response = EDD_STS_ERR_PRM_PORTID;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,
                                    "[H:--] EDDP_PRMRequestPrmRead(): ERROR -> PrmRecord PDNRTFeedInLoadLimitation: only interface port (0) allowed (edd_port_id == 0x%x)",
                                    pPrmRead->edd_port_id);
                    }
                    // check "record_data_length
                    if(EDD_STS_OK == Response)
                    {
                        if (pPrmRead->record_data_length < pDDB->PRM.pRecordsetA->PDNRTFeedInLoadLimitation_Length)
                        {
                            Response = EDD_STS_ERR_PRM_DATA;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, 
                                        "[H:%2X] EDDP_PRMRequestPrmRead(): ERROR -> PrmRecord PDNRTFeedInLoadLimitation: record_data_length(%d) < PDNRTFeedInLoadLimitation_Length(%d)", 
                                        pHDB->Handle, pPrmRead->record_data_length, pDDB->PRM.pRecordsetA->PDNRTFeedInLoadLimitation_Length);
                        }
                    }
                    if(EDD_STS_OK == Response)
                    {
                        // copy record into RQB
                        EDDP_MEMCOPY_COMMON(pPrmRead->record_data, pDDB->PRM.pRecordsetA->pPDNRTFeedInLoadLimitation, pDDB->PRM.pRecordsetA->PDNRTFeedInLoadLimitation_Length);
                        pPrmRead->record_data_length = pDDB->PRM.pRecordsetA->PDNRTFeedInLoadLimitation_Length;
                    }
                }
                else
                {
                    // no record of this type has been written previously
                    pPrmRead->record_data_length = 0;
                    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmRead(): PDNRTFeedInLoadLimitation(0x%X) is not present!", 
                        pHDB->Handle, pPrmRead->record_index);
                }
            }
            break;

            /* ---------------------------------------- */
            /* User specific records                    */
            /* These records are not used by EDDP       */
            /* ---------------------------------------- */
            case EDDP_PRM_PDCONTROL_PLL_RECORD_INDEX:
            case EDDP_PRM_PDSET_DEFAULT_PORT_STATES_RECORD_INDEX:
            case EDDP_PRM_PDTRACE_UNIT_CONTROL_RECORD_INDEX:
            case EDDP_PRM_PDTRACE_UNIT_DATA_RECORD_INDEX:
            {
                pPrmRead->record_data_length = 0;
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmRead(): PrmRecord(0x%X) is not used by EDDP", 
                    pHDB->Handle, pPrmRead->record_index);
            }
            break;

            /* ---------------------------------------- */
            /* Unknown Records                          */
            /* ---------------------------------------- */
            default:
            {
                Response = EDD_STS_ERR_PRM_INDEX;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMRequestPrmRead(): ERROR -> unknown record_index(0x%X)!", 
                    pPrmRead->record_index);
            }
            break;
        }
    }

    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMRequestPrmRead(): Response=0x%X", Response);

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMRequestPrmPrepare                   +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                         +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         Opcode:     EDD_OPC_REQUEST                 +*/
/*+     LSA_HANDLE_TYPE         Handle:                                     +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user (not used)           +*/
/*+     EDD_SERVICE             Service:    EDD_SRV_PRM_PREPARE             +*/
/*+     LSA_RESPONSE_TYPE       Response:   Return status                   +*/
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:                                     +*/
/*+                                                                         +*/
/*+  pParam points to EDD_UPPER_PRM_WRITE_PTR_TYPE                          +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_SEQUENCE                                      +*/
/*+               EDD_STS_ERR_PRM_INDEX                                     +*/
/*+               EDD_STS_ERR_PRM_DATA                                      +*/
/*+               EDD_STS_ERR_PRM_BLOCK                                     +*/
/*+               EDD_STS_ERR_PRM_PORTID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function handles the EDD_SRV_PRM_PREPARE request     +*/
/*+                                                                         +*/
/*+               Note: Opcode/Service not checked!                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMRequestPrmPrepare(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP     Response;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pRQB);
    LSA_UNUSED_ARG(pHDB);
#endif

    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_PRMRequestPrmPrepare(pRQB=0x%X)", pHDB->Handle, pRQB);

    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmPrepare(PrmState=0x%X)", 
        pHDB->Handle, pDDB->PRM.PrmState);

    Response = EDD_STS_OK;

    EDDP_PRMSetBToNotPresent (pDDB);
    pDDB->PRM.PrmState = EDDP_PRM_STATE_PREPARED;

    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMRequestPrmPrepare(): Response=0x%X", Response);

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMEndConsistencyCheck                 +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_BOOL                    IsLocal         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  IsLocal    : is this a 'local' PRM sequence?                           +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PRM_CONSISTENCY                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function checks if the current record set B is       +*/
/*+               consistent and valid for a subsequent PRM_Commit.         +*/
/*+               Parameter 'IsLocal' defines if it is allowed to close all +*/
/*+               ethernet ports.                                           +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMEndConsistencyCheck(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_BOOL                    IsLocal)
{
    EDD_RSP                         Response;
    EDDP_PRM_DETAIL_ERR_PTR_TYPE    pDetailErr;
    LSA_UINT32                      i;
    LSA_UINT32                      UsrPortIndex, HwPortIndex;
    LSA_UINT32                      Closed_Ports_Count;
    LSA_UINT32                      FastForwardingMulticastMACAdd;

    Response            = EDD_STS_OK;
    pDetailErr          = &pDDB->PrmDetailErr;
    Closed_Ports_Count  = 0;

    //Prepare PortparamsNotApplicable array
    for ( UsrPortIndex=0; UsrPortIndex < pDDB->SWI.PortCnt; UsrPortIndex++ )
    {
        pDDB->PRM.PortparamsNotApplicable[UsrPortIndex] = EDD_PRM_PORT_PARAMS_APPLICABLE;
    }

    /* ---------------------------------------------------- */
    /* consistency check for active RTC3 provider/consumer  */
    /* ---------------------------------------------------- */
    // not for DFP (for DFP it will be checked below)
    if (   (!(EDDP_PRMPDIRSubframeDataIsDfpPresent(pDDB, LSA_FALSE))) 
        && (!(EDDP_PRMPDIRSubframeDataIsDfpPresent(pDDB, LSA_TRUE))) 
       )
    {
        // check only if this is a new IRT configuration
        if ( !(EDDP_PRMPDIRDataIsSameIrtActive(pDDB)) )
        {
            if ( EDDP_CRTCheckIsActiveRTC3ProviderConsumerPresent(pDDB) )
            {
                EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDIRDATA_RECORD_INDEX, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE); 
                Response = EDD_STS_ERR_PRM_CONSISTENCY;
                EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMEndConsistencyCheck(): ERROR -> CRT contains active RTC3 provider or consumer!");
            }
        }
    }

    /* ---------------------------------------------------- */
    /* consistency check for PDIRData                       */
    /* ---------------------------------------------------- */
    if (Response == EDD_STS_OK)
    {
        // check the combination: if PDIRData is present, so PDSyncData must be present too
        if ((pDDB->PRM.pRecordsetB->PDIRData_IsPresent) && (!(pDDB->PRM.pRecordsetB->PDSyncData_IsPresent)))
        {
            EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDSYNCDATA_RECORD_INDEX, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE); 
            Response = EDD_STS_ERR_PRM_CONSISTENCY;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMEndConsistencyCheck(): ERROR -> PDIRData is present but PDSyncData is not present!");
        }
        else
        {
            // check the combination MRP + SyncInRed not allowed
            //      MRP         = at least 1 PDPortMrpDataAdjust-Record is present
            //      SyncInRed   = FrameID(0x0080) in PDIRData is present
            // the new PDIRData is in Buffer "PDIRData_B"
            if ((pDDB->PRM.pPDIRData_B->Sync_IsPresent) && (pDDB->PRM.MRPPort.RecordCountInSetB > 0))
            {
                EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDD_PRM_ERR_INDEX_DEFAULT, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
                Response = EDD_STS_ERR_PRM_CONSISTENCY;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMEndConsistencyCheck(): ERROR -> combination MRP + SyncInRed(0x0080) is not allowed!");
            }
            else
            {
                // check MaxBridgeDelay in PDIRFrameData
                /* Set PrmDetailError in EDDP_PRMDIRDataCheckMaxBridgeDelay() */
                Response = EDDP_PRMPDIRDataCheckMaxBridgeDelay (pDDB, pDDB->PRM.pPDIRData_B->PDIRGlobalData.MaxBridgeDelay);
            }
        }

        // checks for parameters in PDIRFrameData array
        if ((Response == EDD_STS_OK) && (pDDB->PRM.pRecordsetB->PDIRData_IsPresent))
        {
            for ( i=0; i < pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All; i++ )
            {
                // check double FrameIDs in PDIRFrameData
                /* Set PrmDetailError in EDDP_PRMDIRDataCheckDoubleFrameID() */
                Response = EDDP_PRMPDIRDataCheckDoubleFrameID (pDDB, (LSA_UINT16)i);

                if (Response == EDD_STS_OK)
                {
                    // check the FrameSendOffset is in valid range
                    /* Set PrmDetailError in EDDP_PRMPDIRDataCheckFrameSendOffset */
                    Response = EDDP_PRMPDIRDataCheckFrameSendOffset (pDDB, (LSA_UINT16)i);
                }

#if defined (EDDP_APPLY_IRTPU_RESTRICTIONS)
                if (Response == EDD_STS_OK)
                {
                    // check the ReductionRatio is in valid range
                    /* Set PrmDetailError in EDDP_PRMPDIRDataCheckRR */
                    Response = EDDP_PRMPDIRDataCheckRR (pDDB, (LSA_UINT16)i);
                }
#endif
                if (Response != EDD_STS_OK)
                {
                    // exit FOR loop
                    i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                }
            }
        }

        /* ---------------------------------------------------- */
        /* PN-IP Rev1: "Running in a Squeeze" is not allowed    */
        /*  - Fragmentation is ON in PDIRData                   */
        /*  - active RTC1/2/RToverUDP provider in CRT present   */
        /* ---------------------------------------------------- */
        #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
        if (   (Response == EDD_STS_OK)
            && (EDDP_HAL_HW_PNIP_REV1 == EDDP_HALGetPnipRevision(pDDB))
           )
        {
            if (pDDB->PRM.pRecordsetB->PDIRData_IsPresent)
            {
                LSA_UINT32      FragmentationMode = EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_OFF;

                // is "FrameDataProperties" present in PDIRData ?
                if (EDDP_PRMPDIRDataIsFrameDataPropertiesPresent(pDDB, LSA_FALSE))
                {
                    // get parameter PDIRData.FrameDataProperties.FragmentationMode
                    FragmentationMode = EDDP_PRMPDIRDataGetFrameDataProperties (pDDB, EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_SHIFT, LSA_FALSE);
                }

                // check the combination: Fragmentation is ON and active RTC1/2/RToverUDP provider
                if (   (FragmentationMode != EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_OFF) 
                    && (EDDP_CRTCheckIsActiveRTC12ProviderPresent(pDDB))
                   )
                {
                    EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDIRDATA_RECORD_INDEX, pDetailErr->OffsetFragmentationOn, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
                    Response = EDD_STS_ERR_PRM_CONSISTENCY;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMEndConsistencyCheck(): ERROR -> with PN-IP Rev1 the <Running in a Squeeze> is not allowed: Fragmentation=ON in PDIRData and active RTC1/2/RToverUDP provider!");
                }
            }
        }
        #endif
    }

    
    /* ---------------------------------------------------- */
    /* check for MaxPortDelays                              */
    /* ---------------------------------------------------- */
    if ((Response == EDD_STS_OK) && (pDDB->PRM.pRecordsetB->PDIRData_IsPresent))
    {
        for ( UsrPortIndex=0; UsrPortIndex < pDDB->PRM.pPDIRData_B->PDIRGlobalData.NumberOfPorts; UsrPortIndex++ )
        {
            LSA_UINT32 ErrOffset;
            HwPortIndex = EDDP_SWI_get_HwPortID(pDDB, UsrPortIndex + 1/*UsrPortID*/) - 1;

            Response = EDDP_PRMPDIRDataRecord_CheckMaxPortDelays(pDDB, &pDDB->PRM.pPDIRData_B->PDIRGlobalData, HwPortIndex, &ErrOffset);

            if (   (EDD_STS_ERR_PARAM == Response)
                && (1 == pDDB->PRM.IsModularPort[UsrPortIndex]) ) 
            {
                pDDB->PRM.PortparamsNotApplicable[UsrPortIndex] = EDD_PRM_PORT_PARAMS_NOT_APPLICABLE;
            }
            else if (EDD_STS_OK != Response)
            {
                //consistency check failed
                EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMEndConsistencyCheck(): ERROR -> EDDP_PRMPDIRDataRecord_CheckMaxPortDelays returned 0x%X for HWPortIndex %u, USrPortIndex %u",
                    Response, HwPortIndex, UsrPortIndex);
                EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDIRDATA_RECORD_INDEX, ErrOffset, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
                Response = EDD_STS_ERR_PRM_CONSISTENCY;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                //consistency check failed, exit for loop
                break;
            }
        }
    }

    /* ---------------------------------------------------- */
    /* consistency check for PDIRSubframeData               */
    /* ---------------------------------------------------- */
    if (Response == EDD_STS_OK)
    {
        // is DFP record present in Recordset B or Recordset A ?
        if (   (pDDB->PRM.pRecordsetB->PDIRSubframeData_IsPresent)
            || (pDDB->PRM.pRecordsetA->PDIRSubframeData_IsPresent)
           )
        {
            // check of active RTC3 provider/consumer only if new DFP+IRT configuration
            if ( !(EDDP_PRMPDIRSubframeDataIsSameDfpActive(pDDB)) )
            {
                if ( EDDP_CRTCheckIsActiveRTC3ProviderConsumerPresent(pDDB) )
                {
                    EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDIRSUBFRAMEDATA_RECORD_INDEX, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
                    Response = EDD_STS_ERR_PRM_CONSISTENCY;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMEndConsistencyCheck(): ERROR -> CRT contains active RTC3 provider or consumer!");
                }
            }

            // check following parameters only if new PDIRSubframeData is present
            if ((Response == EDD_STS_OK) && (pDDB->PRM.pRecordsetB->PDIRSubframeData_IsPresent))
            {
                // PDIRData must be present too
                if ( !(pDDB->PRM.pRecordsetB->PDIRData_IsPresent) )
                {
                    EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDIRDATA_RECORD_INDEX, EDD_PRM_ERR_OFFSET_DEFAULT, LSA_FALSE, LSA_TRUE);
                    Response = EDD_STS_ERR_PRM_CONSISTENCY;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMEndConsistencyCheck(): ERROR -> PDIRSubframeData is present but PDIRData is not present!");
                }
                else
                {
                    // "FrameDataProperties" must be present in PDIRData
                    if ( !(EDDP_PRMPDIRDataIsFrameDataPropertiesPresent(pDDB, LSA_FALSE)) )
                    {
                        EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDIRDATA_RECORD_INDEX, EDD_PRM_ERR_OFFSET_DEFAULT, LSA_FALSE, LSA_TRUE);
                        Response = EDD_STS_ERR_PRM_CONSISTENCY;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMEndConsistencyCheck(): ERROR -> PDIRSubframeData is present but PDIRData.FrameDataProperties is not present!");
                    }
                    else
                    {
                        // FrameDataProperties(LEGACY) is not permitted
                        // get parameter PDIRData.FrameDataProperties.FastForwardingMulticastMACAdd
                        FastForwardingMulticastMACAdd = 
                            EDDP_PRMPDIRDataGetFrameDataProperties (pDDB, EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_SHIFT, LSA_FALSE);

                        if (FastForwardingMulticastMACAdd == EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_LEGACY)
                        {
                            EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDIRDATA_RECORD_INDEX, EDD_PRM_ERR_OFFSET_DEFAULT, LSA_FALSE, LSA_TRUE);
                            Response = EDD_STS_ERR_PRM_CONSISTENCY;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMEndConsistencyCheck(): ERROR -> PDIRSubframeData is present but PDIRData.FrameDataProperties is Legacy!");
                        }
                    }
                }

                if (Response == EDD_STS_OK)
                {
                    // check the PDIRSubframeData contains valid data and set connection to PDIRData
                    /* Set PrmDetailError in EDDP_PRMDIRSubframeDataCheckAndValidate() */
                    Response = EDDP_PRMPDIRSubframeDataCheckAndValidate (pDDB);
                }
            }
        }
    }

    /* ---------------------------------------------------- */
    /* consistency check for PDSyncData                     */
    /* ---------------------------------------------------- */
    if ((Response == EDD_STS_OK) && (pDDB->PRM.pRecordsetB->PDSyncData_IsPresent))
    {
        // PDSyncData.SendClockFactor must be identical with SendClockFactor in Hardware
        if (pDDB->PRM.PDSyncData.SendClockFactor != pDDB->SWI.CycleInfo.CycleBaseFactor)
        {
            EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDSYNCDATA_RECORD_INDEX, pDetailErr->OffsetSendClockFactor, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
            Response = EDD_STS_ERR_PRM_CONSISTENCY;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMEndConsistencyCheck(): ERROR -> PDSyncData.SendClockFactor(%d) != SWI.CycleBaseFactor(%d)", 
                pDDB->PRM.PDSyncData.SendClockFactor, pDDB->SWI.CycleInfo.CycleBaseFactor);
        }
    }

    /* ---------------------------------------------------- */
    /* consistency check for PortDataAdjust                 */
    /* ---------------------------------------------------- */
    if (Response == EDD_STS_OK)
    {
        for ( UsrPortIndex=0; UsrPortIndex < pDDB->SWI.PortCnt; UsrPortIndex++ )
        {
            LSA_UINT32 HwPortID;
            LSA_UINT32 UsrPortID = UsrPortIndex + 1;
            EDDP_PHY_LINK_STATUS_TYPE PhyLinkStatus;

            HwPortID    = EDDP_SWI_get_HwPortID (pDDB, UsrPortID);
            HwPortIndex = HwPortID-1;

            //Get MediaType
            pDDB->SWI.PHY_FunctionPointers[HwPortID].EDDP_PHY_Get_Media_Type(pDDB->hDDB, pDDB->hSysDev, HwPortID, &PhyLinkStatus.MediaType, &PhyLinkStatus.OpticalTransType, &PhyLinkStatus.FxTransceiverType);
            pDDB->PRM.PortDataAdjust.Parameters[UsrPortID].MediaType        = PhyLinkStatus.MediaType;
            pDDB->PRM.PortDataAdjust.Parameters[UsrPortID].OpticalTransType = PhyLinkStatus.OpticalTransType;

            //Check subrecord AdjustMAUType
            //if(EDD_PORT_NOT_PRESENT == pDDB->SWI.LinkStatus[i].PortStatus)
            if (EDDP_PRMChangePortState_IsPulled(pDDB, HwPortIndex))
            {
                if(EDD_PRM_PORT_IS_COMPACT == pDDB->PRM.IsModularPort[UsrPortIndex])
                {
                    /* Exception */
                    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMEndConsistencyCheck(): FATAL -> ChangePortState is IsPullen and IsModular is 0 --> Throw an exection. HwPortID: 0x%X",
                        HwPortID);
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                }

                /* Port is NOT present */
                pDDB->PRM.PortDataAdjust.Parameters[UsrPortID].Duplexity    = EDD_LINK_UNKNOWN;
                pDDB->PRM.PortDataAdjust.Parameters[UsrPortID].Speed        = EDD_LINK_UNKNOWN;
                pDDB->PRM.PortparamsNotApplicable[UsrPortIndex]             = EDD_PRM_PORT_PARAMS_NOT_APPLICABLE;
            }
            else if (pDDB->PRM.PortDataAdjust.Parameters[UsrPortID].MAUType_IsPresent)
            {
                LSA_UINT32 Speed;
                LSA_UINT32 Duplexity;

                //Decode MAUType (MAUType from record is used here)
                Response = pDDB->SWI.PHY_FunctionPointers[HwPortID].EDDP_PHY_CheckMAUType(pDDB->hDDB, pDDB->hSysDev, HwPortID, pDDB->PRM.PortDataAdjust.Parameters[UsrPortID].MAUType, &Speed, &Duplexity);

                if (EDD_STS_OK != Response)
                {
                    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H:--] EDDP_PRMEndConsistencyCheck(): EDDP_PHY_CheckMAUType() returned != EDD_STS_OK Response=0x%X",
                        Response);
                    if (   (EDD_STS_ERR_PARAM != Response)
                        || (EDD_PRM_PORT_IS_COMPACT == pDDB->PRM.IsModularPort[UsrPortIndex])
                        )
                    {
                        //consistency check failed
                        EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDPORT_DATA_ADJUST_RECORD_INDEX, pDDB->PrmDetailErr.OffsetMAUType, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
                        Response = EDD_STS_ERR_PRM_CONSISTENCY;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMEndConsistencyCheck(): ERROR -> (PDPortDataAdjust) It is not allowed to disable all ports when 'IsLocal' == LSA_FALSE!");
                        //consistency check failed, exit for loop
                        break;
                    }
                }

                if(    (EDD_MEDIATYPE_UNKNOWN == PhyLinkStatus.MediaType)
                    || (EDD_STS_OK != Response)
                    || (EDD_LINK_UNKNOWN == Duplexity)
                    || (EDD_LINK_UNKNOWN == Speed) )
                {
                    pDDB->PRM.PortparamsNotApplicable[UsrPortIndex] = EDD_PRM_PORT_PARAMS_NOT_APPLICABLE;
                }

                pDDB->PRM.PortDataAdjust.Parameters[UsrPortID].Duplexity    = Duplexity;
                pDDB->PRM.PortDataAdjust.Parameters[UsrPortID].Speed        = Speed;
                Response = EDD_STS_OK;
            }
            else
            {
                //MAUType not present ==> AUTONEG
                pDDB->PRM.PortDataAdjust.Parameters[UsrPortID].Duplexity    = EDD_LINK_UNKNOWN;
                pDDB->PRM.PortDataAdjust.Parameters[UsrPortID].Speed        = EDD_LINK_UNKNOWN;
                pDDB->PRM.PortparamsNotApplicable[UsrPortIndex]             = EDD_PRM_PORT_PARAMS_APPLICABLE;
            }

            //Check subrecord AdjustLinkState
            if (pDDB->PRM.PortDataAdjust.Parameters[UsrPortID].LinkState_IsPresent)
            {
                    Closed_Ports_Count++;
            }

            //Check for PNIO conformity
            if (   (Closed_Ports_Count == pDDB->SWI.PortCnt)
                && !IsLocal)
            {
                //if isLocal is set, all ports can be closed. MAUType has not to be taken into account, as PNIP supports 100MBFD only
                EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDPORT_DATA_ADJUST_RECORD_INDEX, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
                Response = EDD_STS_ERR_PRM_CONSISTENCY;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMEndConsistencyCheck(): ERROR -> (PDPortDataAdjust) It is not allowed to disable all ports when 'IsLocal' == LSA_FALSE!");
            }
        }
    }


    /* ---------------------------------------------------- */
    /* PDSetDefaultPortStates                               */
    /* ---------------------------------------------------- */
    // nothing todo
    //if(EDD_STS_OK == Response)
    //{
    //    Response = EDDP_PRMPDSetDefaultPortStatesConsistencyCheck_End(pDDB,pDetailErr);
    //}


    /* ---------------------------------------------------- */
    /* consistency check for PDInterfaceMrpDataAdjust       */
    /* ---------------------------------------------------- */
    if (Response == EDD_STS_OK)
    {
        if (!(((pDDB->PRM.pRecordsetB->PDInterfaceMrpDataAdjust_IsPresent) &&
               (pDDB->PRM.MRPPort.RecordCountInSetB == 0 || pDDB->PRM.MRPPort.RecordCountInSetB == 2 || pDDB->PRM.MRPPort.RecordCountInSetB == 4)) ||
              ((!(pDDB->PRM.pRecordsetB->PDInterfaceMrpDataAdjust_IsPresent)) &&
               (pDDB->PRM.MRPPort.RecordCountInSetB == 0)))
           )
        {
            if (pDDB->PRM.pRecordsetB->PDInterfaceMrpDataAdjust_IsPresent)
            {
                //not 0 or NrOfPorts PDPortMrpDataAjdust records written
                EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDPORT_MRP_DATA_ADJUST_RECORD_INDEX, EDD_PRM_ERR_OFFSET_DEFAULT, LSA_FALSE, LSA_TRUE);
                Response = EDD_STS_ERR_PRM_CONSISTENCY;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMEndConsistencyCheck(): ERROR -> Number of PDPortMrpDataAdjust records (%d) is not 0 or 2 or 4!", 
                    pDDB->PRM.MRPPort.RecordCountInSetB);
            }
            else
            {
                //PDPortMrpDataAjdust written without PDInterfaceMrpDataAdjust written
                EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDINTERFACE_MRP_DATA_ADJUST_RECORD_INDEX, EDD_PRM_ERR_OFFSET_DEFAULT, LSA_FALSE, LSA_TRUE);
                Response = EDD_STS_ERR_PRM_CONSISTENCY;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMEndConsistencyCheck(): ERROR -> Number of PDPortMrpDataAdjust records (%d) is not 0!", 
                    pDDB->PRM.MRPPort.RecordCountInSetB);
            }
        }

        if (Response == EDD_STS_OK)
        {
            if ((pDDB->SWI.MaxMRP_Instances == 0) && (pDDB->PRM.pRecordsetB->PDInterfaceMrpDataAdjust_IsPresent))
            {
                EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDD_PRM_ERR_INDEX_DEFAULT, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
                Response = EDD_STS_ERR_PRM_CONSISTENCY;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);    
                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMEndConsistencyCheck(): ERROR -> MaxMRP_Instances(no MRP) but MRP record PDInterfaceMrpDataAdjust is present");
            }
        }
    }

    /* ---------------------------------------------------- */
    /* PDNCDataCheck                                        */
    /* ---------------------------------------------------- */
    if (!(pDDB->PRM.pRecordsetB->PDNCDataCheck_IsPresent))
    {
        // there is no NCDataCheck record with this new parameter set -> disable frame drop diagnoses
        pDDB->PRM.FrameDrop.Budgets.ErrorDropBudget.Enabled                 = LSA_FALSE;
        pDDB->PRM.FrameDrop.Budgets.MaintenanceDemandedDropBudget.Enabled   = LSA_FALSE;
        pDDB->PRM.FrameDrop.Budgets.MaintenanceRequiredDropBudget.Enabled   = LSA_FALSE;
    }
    
    /* ---------------------------------------------------- */
    /* PDNRTFeedInLoadLimitation                            */
    /* ---------------------------------------------------- */
    if(EDD_STS_OK == Response)
    {
        Response = EDDP_PRMPDNRTFeedInLoadLimitationConsistencyCheck_End(pDDB,pDetailErr);
    }

    return (Response);                
}  


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMRequestPrmEnd                       +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                         +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         Opcode:     EDD_OPC_REQUEST                 +*/
/*+     LSA_HANDLE_TYPE         Handle:                                     +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user (not used)           +*/
/*+     EDD_SERVICE             Service:    EDD_SRV_PRM_END                 +*/
/*+     LSA_RESPONSE_TYPE       Response:   Return status                   +*/
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:                                     +*/
/*+                                                                         +*/
/*+  pParam points to EDD_UPPER_PRM_WRITE_PTR_TYPE                          +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_SEQUENCE                                      +*/
/*+               EDD_STS_ERR_PRM_INDEX                                     +*/
/*+               EDD_STS_ERR_PRM_DATA                                      +*/
/*+               EDD_STS_ERR_PRM_BLOCK                                     +*/
/*+               EDD_STS_ERR_PRM_PORTID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function handles the EDD_SRV_PRM_END request         +*/
/*+                                                                         +*/
/*+               END is only allowed after PREPARE or WRITE                +*/
/*+                                                                         +*/
/*+               Note: Opcode/Service not checked!                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMRequestPrmEnd(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
	  EDD_RSP	                        Response;
	  EDD_UPPER_PRM_END_PTR_TYPE      pPrmEnd;
    EDDP_PRM_DETAIL_ERR_PTR_TYPE    pDetailErr;
    LSA_UINT32                      PortIndex;

    pDetailErr  = &pDDB->PrmDetailErr;

    EDDP_PRM_INIT_DETAIL_ERR(pDetailErr);

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pHDB);
#endif

    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_PRMRequestPrmEnd(pRQB=0x%X)", pHDB->Handle, pRQB);

    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmEnd(PrmState=0x%X)", 
        pHDB->Handle, pDDB->PRM.PrmState);

    Response    = EDD_STS_OK; 
    pPrmEnd     = (EDD_UPPER_PRM_END_PTR_TYPE) pRQB->pParam;

    /* -------------------------------- */
    /* common checks                    */
    /* -------------------------------- */
    if (LSA_HOST_PTR_ARE_EQUAL(pPrmEnd, LSA_NULL))
    {
        Response = EDD_STS_ERR_PARAM; 
        EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmEnd(): ERROR -> RQB->pParam=NULL!",
            pHDB->Handle);
    }

    if (   (EDD_PRM_PARAMS_ASSIGNED_REMOTE != pPrmEnd->Local)
        && (EDD_PRM_PARAMS_ASSIGNED_LOCALLY != pPrmEnd->Local) )
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmEnd(): ERROR -> Local (%d)!",
            pHDB->Handle, pPrmEnd->Local);
    }

    for(PortIndex = 0 ; PortIndex < pDDB->SWI.PortCnt ; PortIndex++)
    {
        if(EDDP_PRMChangePortState_IsAnySequenceRunning(pDDB, &PortIndex))
        {
            Response = EDD_STS_ERR_SEQUENCE; 
            EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmEnd(): ERROR -> DDP_PRMChangePortState with PortIndex: 0x%X _IsAnySequenceRunning is LSA_TRUE",
                              pHDB->Handle,
                              PortIndex);
            break;
        }

        if (   (EDD_PRM_PORT_IS_MODULAR != pPrmEnd->IsModularPort[PortIndex])
            && (EDD_PRM_PORT_IS_COMPACT != pPrmEnd->IsModularPort[PortIndex]))
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmEnd(): ERROR -> IsModularPort (%d) at PortIndex %d invalid",
                pHDB->Handle, pPrmEnd->IsModularPort[PortIndex], PortIndex);
            break;
        }
    }

    /* -------------------------------- */
    /* execute PrmEnd                   */
    /* -------------------------------- */
    if (Response == EDD_STS_OK)
    {
        if ((pDDB->PRM.PrmState == EDDP_PRM_STATE_PREPARED) || (pDDB->PRM.PrmState == EDDP_PRM_STATE_WRITE))
        {
            LSA_UINT8 LocPortIndex;

            for(LocPortIndex = 0 ; LocPortIndex < pDDB->SWI.PortCnt ; LocPortIndex++)
            {
                pDDB->PRM.IsModularPort[LocPortIndex] = pPrmEnd->IsModularPort[LocPortIndex];
            }            

            Response = EDDP_PRMEndConsistencyCheck (pDDB, (EDD_PRM_PARAMS_ASSIGNED_LOCALLY == pPrmEnd->Local));
            if (Response == EDD_STS_OK)
            {
                for(LocPortIndex = 0 ; LocPortIndex < pDDB->SWI.PortCnt ; LocPortIndex++)
                {
                    pPrmEnd->PortparamsNotApplicable[LocPortIndex] = pDDB->PRM.PortparamsNotApplicable[LocPortIndex];
                }

                // calculate parameters of "LocalTransferStartEnd"
                EDDP_PRMPDIRDataCalculateLocalTransferStartEndNs (pDDB, LSA_FALSE);
                // calculate parameters of "EndOfRedPhaseIFPorts"
                EDDP_PRMPDIRDataCalculateEndOfRedPhaseNsIF (pDDB, LSA_FALSE);
            }
        }
        else
        {
            EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDD_PRM_ERR_INDEX_DEFAULT, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
            Response = EDD_STS_ERR_SEQUENCE;
            EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmEnd(): ERROR -> Wrong sequence! PrmState=0x%X",
                pHDB->Handle, pDDB->PRM.PrmState);
        }
    }

    /* -------------------------------- */
    /* check Response                   */
    /* -------------------------------- */
    if (Response == EDD_STS_OK)
    {
        LSA_UINT32  j;
        for (j = 0; j < EDD_CFG_MAX_PORT_CNT; ++j)
        {
            ((EDD_UPPER_PRM_END_PTR_TYPE)(pRQB->pParam))->PortparamsNotApplicable[j] = pDDB->PRM.PortparamsNotApplicable[j];
        }

        // wait for PrmCommit or PrmPrepare
        pDDB->PRM.PrmState = EDDP_PRM_STATE_END;
    }
    else
    {
        if (/* pDetailErr->ErrValid == LSA_TRUE && */ !(LSA_HOST_PTR_ARE_EQUAL(pPrmEnd, LSA_NULL)) && !(LSA_HOST_PTR_ARE_EQUAL(pDetailErr, LSA_NULL)))
        {
            EDDP_RQB_PRM_END_SET(pPrmEnd, pDetailErr->ErrPortID, pDetailErr->ErrIndex, pDetailErr->ErrOffset, pDetailErr->ErrFault); 
            EDDP_PRM_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMRequestPrmEnd(): ERROR -> err_port_id=%d err_index=%d err_offset=%d err_fault=%d",
                pPrmEnd->err_port_id, /*  */
                pPrmEnd->err_index, /*  */
                pPrmEnd->err_offset, /*  */
                pPrmEnd->err_fault); /*  */
        }
        // PrmCommit is not permitted
        EDDP_PRMSetBToNotPresent (pDDB);
        pDDB->PRM.PrmState = EDDP_PRM_STATE_PREPARED;
    }

    EDDP_PRM_INIT_DETAIL_ERR(pDetailErr);

    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMRequestPrmEnd(): Response=0x%X", Response);

    return (Response);
}
  

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMRequestPrmChangePort                +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                         +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         Opcode:     EDD_OPC_REQUEST                 +*/
/*+     LSA_HANDLE_TYPE         Handle:                                     +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user (not used)           +*/
/*+     EDD_SERVICE             Service:    EDD_SRV_PRM_CHANGE_PORT         +*/
/*+     LSA_RESPONSE_TYPE       Response:   Return status                   +*/
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:     EDD_RQB_PRM_CHANGE_PORT_TYPE    +*/
/*+                                                                         +*/
/*+  pParam points to EDD_UPPER_PRM_WRITE_PTR_TYPE                          +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_OPCODE                                        +*/
/*+               EDD_STS_ERR_SERVICE                                       +*/
/*+               EDD_STS_ERR_SEQUENCE                                      +*/
/*+               EDD_STS_ERR_HW                                            +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function handles the EDD_SRV_PRM_COMMIT request      +*/
/*+                                                                         +*/
/*+               Note: Opcode/Service not checked!                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMRequestPrmChangePort(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB
    )
{
    EDD_RSP                         Response;
    LSA_UINT32                      HwPortID;
    LSA_UINT32                      HwPortIndex;
    EDD_RQB_PRM_CHANGE_PORT_TYPE*   pParam;

    pParam                          = (EDD_RQB_PRM_CHANGE_PORT_TYPE*)pRQB->pParam;
    HwPortID                        = EDDP_SWI_get_HwPortID(pDDB, pParam->PortID);
    HwPortIndex                     = HwPortID - 1;
    Response                        = EDD_STS_OK;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pHDB);
#endif
    LSA_UNUSED_ARG(pDDB);
    LSA_UNUSED_ARG(pHDB);

    if ((pDDB->PRM.PrmState == EDDP_PRM_STATE_PREPARED) || (pDDB->PRM.PrmState == EDDP_PRM_STATE_WRITE))
    {
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P)
        if  (  (EDDP_PHY_TYPE_NEC == pDDB->SWI.PortParams[HwPortID].PhyType) 
            || (EDD_PRM_PORTMODULE_PULLED == pParam->ModuleState)  )    //RQ 1722727: Pull must be allowed for any PHY
        {
            Response = EDD_STS_OK;
        }
        else
        {
            EDDP_PRM_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "EDDP_PRMRequestPrmChangePort(pParam->PortID=%d, PhyType=0x%X): ERROR -> This PHY is not supported HwPortID=%d, HwPortIndex=%d", 
                    pParam->PortID, 
                    pDDB->SWI.PortParams[pParam->PortID].PhyType,
                    HwPortID,
                    HwPortIndex);
            Response = EDD_STS_ERR_PARAM;
        }
#endif
        if (EDD_STS_OK == Response)
        {

            /* PortID starts with 1, the -1 modification to get right array element */

            /* statemachine for pulling and plugging port modules */

            if(EDD_PRM_PORTMODULE_PULLED == pParam->ModuleState)
            {
                Response = EDDP_PRMChangePortStateMachine(pDDB, EDDP_PRM_PORT_MODULE_SM_TRIGGER_PULL, HwPortIndex, pParam);
            }
            else if (EDD_PRM_PORTMODULE_PLUG_PREPARE == pParam->ModuleState)
            {
                Response = EDDP_PRMChangePortStateMachine(pDDB, EDDP_PRM_PORT_MODULE_SM_TRIGGER_PLUG_PREPARE, HwPortIndex, pParam);
            }
            else if (EDD_PRM_PORTMODULE_PLUG_COMMIT == pParam->ModuleState)
            {
                if (   (EDD_PRM_APPLY_RECORD_PORT_PARAMS != pParam->ApplyDefaultPortparams)
                    && (EDD_PRM_APPLY_DEFAULT_PORT_PARAMS != pParam->ApplyDefaultPortparams))
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H: ] EDDP_PRMRequestPrmChangePort(): ERROR -> ApplyDefaultPortparams (%d), HwPortID: 0x%X illegal value!",
                        pParam->ApplyDefaultPortparams,
                        HwPortID);
                }
                else
                {
                    Response = EDDP_PRMChangePortStateMachine(pDDB, EDDP_PRM_PORT_MODULE_SM_TRIGGER_PLUG_COMMIT, HwPortIndex, pParam);
                }
            }
            else
            {
                // ModulState unknown
                Response = EDD_STS_ERR_PARAM;
                EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H: ] EDDP_PRMRequestPrmChangePort(): ERROR -> Unknown ModuleState. Response=0x%X, HwPortID: 0x%X, ModuleState: 0x%X", 
                                  Response, 
                                  HwPortID,
                                  pParam->ModuleState);
            }
        }

        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMRequestPrmChangePort(): Response=0x%X, HwPortID: 0x%X", 
                          Response, 
                          HwPortID);
    }
    else
    {
        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] OUT: EDDP_PRMRequestPrmChangePort(): PrmState=0x%X, HwPortID: 0x%X", 
                          pDDB->PRM.PrmState,
                          HwPortID);

        Response = EDD_STS_ERR_SEQUENCE;
    }

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMChangePortStateMachine_DoPull       +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+  Input                 :    LSA_UINT32                       PortID     +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock             (in/out) +*/
/*+  PortID        : Describes wich Port should be handeled        (in)     +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:    Helperfunction for EDDP_PRMChangePortStateMachine      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMChangePortStateMachine_DoPull(
	EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    LSA_UINT32                              HwPortIndex)
{
    /* Deklarations */
    EDD_RSP    Response;
    LSA_UINT32 HwPortID;

    /* Initalisation */
    Response = EDD_STS_OK;
    HwPortID = HwPortIndex + 1;

    /* set PHY to PowerDown */
    // PULL:
    Response = pDDB->SWI.PHY_FunctionPointers[HwPortID].EDDP_PHY_SetPowerDown(pDDB->hDDB, pDDB->hSysDev, HwPortID, LSA_TRUE);
    EDDP_ASSERT_FALSE(Response != EDD_STS_OK);

    // pDDB->SWI.LinkStatus[i].PhyStatus is not set on purpose!

    /* Don't change PhyStatus */
    
    /* Set IsPulled to 1, only in state IsPlugged is IsPulled 0 */
    pDDB->SWI.LinkStatus[HwPortID].IsPulled = EDD_PORTMODULE_IS_PULLED;

    /* Set PortStatus to EDD_PORT_NOT_PRESENT */
    pDDB->SWI.LinkStatus[HwPortID].PortStatus = EDD_PORT_NOT_PRESENT;

    /* Trigger ExtLinkIndication */
    EDDP_GENIndicateLinkChange_Ext(pDDB, HwPortID);

    return;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMChangePortStateMachine_DoPrepare    +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+  Input                 :    LSA_UINT32                       PortID     +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock             (in/out) +*/
/*+  PortID        : Describes wich Port should be handeled        (in)     +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:    Helperfunction for EDDP_PRMChangePortStateMachine      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMChangePortStateMachine_DoPrepare(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    LSA_UINT32                              HwPortIndex)
{
    /* Deklarations */
    EDD_RSP     Response;
    LSA_UINT8   OpticalTransType;
    LSA_UINT8   MediaType;
    LSA_UINT8   FxTransceiverType;
    LSA_UINT32  HwPortID;
    LSA_UINT8   IsPowerDown; /* 1 = Power is down */
    LSA_UINT32  Speed;
    LSA_UINT32  Duplexity;

    /* Initalisation */
    Response = EDD_STS_OK;
    HwPortID = HwPortIndex + 1;
    
    /* check PHY PowerDown, if not --> exception */
    Response = pDDB->SWI.PHY_FunctionPointers[HwPortID].EDDP_PHY_CheckPowerDown(pDDB->hDDB, pDDB->hSysDev, HwPortID, &IsPowerDown);

    if(IsPowerDown && (EDD_STS_OK == Response))
    {
        pDDB->SWI.PHY_FunctionPointers[HwPortID].EDDP_PHY_Get_Media_Type(pDDB->hDDB, pDDB->hSysDev, HwPortID, &MediaType, &OpticalTransType, &FxTransceiverType);

        switch (MediaType) 
        {
            case EDD_MEDIATYPE_COPPER_CABLE:
            case EDD_MEDIATYPE_FIBER_OPTIC_CABLE:
            case EDD_MEDIATYPE_RADIO_COMMUNICATION:
                break;
            default:
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H: ] EDDP_PRMChangePortStateMachine_DoPrepare(): FATAL ERROR -> Unknown MediaType!, HwPortID:0x%X, MediaType: 0x%X", 
                    HwPortID,
                    MediaType);
                   EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                   break;
        }

        pDDB->SWI.LinkStatus[HwPortID].OpticalTransType         = OpticalTransType;
        pDDB->SWI.LinkStatus[HwPortID].MediaType                = MediaType;
//        pDDB->SWI.LinkStatus[HwPortID].FxTransceiverType        = FxTransceiverType;



        /*----------------------------------*/
        /* Check PDPortDataAdjust (MAUType) */
        /*----------------------------------*/
        if (EDD_LINK_AUTONEG != pDDB->SWI.LinkStatus[HwPortID].LinkSpeedModeConfiguredParam)
        {
            //MAUType present: Check stores MAUType
            Response = pDDB->SWI.PHY_FunctionPointers[HwPortID].EDDP_PHY_CheckMAUType(pDDB->hDDB, pDDB->hSysDev, HwPortID, pDDB->SWI.LinkStatus[HwPortID].MAUTypeParam, &Speed, &Duplexity);
        
            if (EDD_STS_OK == Response)
            {
                //PNIP currently supports 100MB only
                if(EDD_LINK_SPEED_100 == Speed)
                {
                    pDDB->SWI.LinkStatus[HwPortID].PortparamsNotApplicable  = EDD_PRM_PORT_PARAMS_APPLICABLE;
                    pDDB->SWI.LinkStatus[HwPortID].LinkSpeed                = (LSA_UINT8)Speed;
                    pDDB->SWI.LinkStatus[HwPortID].LinkDuplexity            = (LSA_UINT8)Duplexity;
                }
                else
                {
                    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H: ] EDDP_PRMChangePortStateMachine_DoPrepare(): WARNING -> No autoneg, CheckMAU returns ok, speed is not 100, HwPortID:0x%X, Speed: 0x%X, MAUType: 0x%X", 
                                        HwPortID,
                                        Speed,
                                        pDDB->SWI.LinkStatus[HwPortID].MAUType);
                    pDDB->SWI.LinkStatus[HwPortID].PortparamsNotApplicable  = EDD_PRM_PORT_PARAMS_NOT_APPLICABLE;
                }
            }
            else if (EDD_STS_ERR_PARAM == Response)
            {
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H: ] EDDP_PRMChangePortStateMachine_DoPrepare(): WARNING -> No autoneg, CheckMAU returns EDD_STS_ERR_PARAM, HwPortID:0x%X, MauType: 0x%X", 
                                    HwPortID,
                                    pDDB->SWI.LinkStatus[HwPortID].MAUType);
                pDDB->SWI.LinkStatus[HwPortID].PortparamsNotApplicable  = EDD_PRM_PORT_PARAMS_NOT_APPLICABLE;
                Response = EDD_STS_OK;
            }
            else
            {
                EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H: ] EDDP_PRMChangePortStateMachine_DoPrepare(): WARNING -> No autoneg, CheckMAU returns an error, HwPortID:0x%X, MauType: 0x%X, Response: 0c%X", 
                                    HwPortID,
                                    pDDB->SWI.LinkStatus[HwPortID].MAUType,
                                    Response);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
        }
        else
        {
            //MAUType not present
            pDDB->SWI.LinkStatus[HwPortID].PortparamsNotApplicable  = EDD_PRM_PORT_PARAMS_APPLICABLE;
        }        

        /*----------------------------------*/
        /* Check PDIRData (Portdelays)      */
        /*----------------------------------*/
        if (EDD_STS_OK == Response) 
        {
            EDD_RSP                     ResponseLocal;
            EDDP_PHY_LINK_STATUS_TYPE   PhyLinkStatus; 

            // load delay values
            PhyLinkStatus.MediaType         = MediaType;
            PhyLinkStatus.OpticalTransType  = OpticalTransType;
            PhyLinkStatus.FxTransceiverType = FxTransceiverType;
            ResponseLocal = pDDB->SWI.PHY_FunctionPointers[HwPortID].EDDP_PHY_LoadDelayValues(pDDB->hDDB, pDDB->Hardware, &PhyLinkStatus);
            EDDP_ASSERT_FALSE(ResponseLocal != EDD_STS_OK);

            pDDB->SWI.LinkStatus[HwPortID].RealPortTxDelay  = PhyLinkStatus.RealPortTxDelay;
            pDDB->SWI.LinkStatus[HwPortID].RealPortRxDelay  = PhyLinkStatus.RealPortRxDelay;
            pDDB->SWI.LinkStatus[HwPortID].MaxPortTxDelay   = PhyLinkStatus.MaxPortTxDelay;
            pDDB->SWI.LinkStatus[HwPortID].MaxPortRxDelay   = PhyLinkStatus.MaxPortRxDelay;

            if (pDDB->PRM.pRecordsetA->PDIRData_IsPresent)
            {
                LSA_UINT32 ErrOffset;
                ResponseLocal = EDDP_PRMPDIRDataRecord_CheckMaxPortDelays(pDDB, &pDDB->PRM.pPDIRData_A->PDIRGlobalData, HwPortIndex, &ErrOffset);

                if (EDD_STS_ERR_PARAM == ResponseLocal)
                {
                    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H: ] EDDP_PRMChangePortStateMachine_DoPrepare(): EDDP_PRMPDIRDataRecord_CheckMaxPortDelays returned 0x%X for HwPortIndex %u", 
                                        ResponseLocal,
                                        HwPortIndex);
                    pDDB->SWI.LinkStatus[HwPortID].PortparamsNotApplicable  = EDD_PRM_PORT_PARAMS_NOT_APPLICABLE;
                }
                else if (EDD_STS_OK != ResponseLocal)
                {
                    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H: ] EDDP_PRMChangePortStateMachine_DoPrepare(): EDDP_PRMPDIRDataRecord_CheckMaxPortDelays returned 0x%X for HwPortIndex %u", 
                                        ResponseLocal,
                                        HwPortIndex);
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                }
            }
        }
    }
    else
    {
        EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H: ] EDDP_PRMChangePortStateMachine_DoPrepare(): FATAL ERROR -> PHY is not PowerDown or EDDP_PHY_CheckPowerDown returns not OK, HwPortID:0x%X, IsPowerDown: 0x%X, Response: 0x%X", 
            HwPortID,
            IsPowerDown,
            Response);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    return Response;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_PRM_ReadPhyStatusRegs                 +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32					   HWPortID     +*/
/*+                             EDDP_PHY_LINK_STATUS_PTR_TYPE  pLinkStatus  +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  HWPortID   : Hardware PortID 1..EDDP_MAX_PORT_CNT                (in)  +*/
/*+  pLinkStatus: points to a struct for returning data               (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Utility function for EDDP_PRMChangePortStateMachine_DoPhyUp()       +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#if 0
LSA_VOID EDDP_LOCAL_FCT_ATTR _EDDP_PRM_ReadPhyStatusRegs(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_UINT16			            HWPortID,
    EDDP_PHY_LINK_STATUS_PTR_TYPE   pLinkStatus)
{
    LSA_UINT32  reg_address;

    EDDP_ASSERT_FALSE((LSA_HOST_PTR_ARE_EQUAL(pLinkStatus, LSA_NULL)));

    // select the right register address for current HwPortID "i"
    reg_address = (HWPortID==1 ? PNIP_REG_PHYSTATUS1_P1 : 
                  (HWPortID==2 ? PNIP_REG_PHYSTATUS1_P2 : 
                  (HWPortID==3 ? PNIP_REG_PHYSTATUS1_P3 : 
                  (HWPortID==4 ? PNIP_REG_PHYSTATUS1_P4 : 0))));
    pLinkStatus->Link = (EDDP_HAL_REG32_READ(pDDB, reg_address) & PNIP_REG_PHYSTATUS1_P1__MSK_PHY_STATUS1);

    // select the right register address for current HwPortID "i"
    reg_address = (HWPortID==1 ? PNIP_REG_PHYSTATUS2_P1 : 
                  (HWPortID==2 ? PNIP_REG_PHYSTATUS2_P2 : 
                  (HWPortID==3 ? PNIP_REG_PHYSTATUS2_P3 : 
                  (HWPortID==4 ? PNIP_REG_PHYSTATUS2_P4 : 0))));
    pLinkStatus->Speed = (EDDP_HAL_REG32_READ(pDDB, reg_address) & PNIP_REG_PHYSTATUS2_P1__MSK_PHY_STATUS2);

    // select the right register address for current HwPortID "i"
    reg_address = (HWPortID==1 ? PNIP_REG_PHYSTATUS3_P1 : 
                  (HWPortID==2 ? PNIP_REG_PHYSTATUS3_P2 : 
                  (HWPortID==3 ? PNIP_REG_PHYSTATUS3_P3 : 
                  (HWPortID==4 ? PNIP_REG_PHYSTATUS3_P4 : 0))));
    pLinkStatus->Duplexity = (EDDP_HAL_REG32_READ(pDDB, reg_address) & PNIP_REG_PHYSTATUS3_P1__MSK_PHY_STATUS3);
}
#endif

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMChangePortStateMachine_DoCommit     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+  Input                 :    LSA_UINT32                       PortID     +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock             (in/out) +*/
/*+  PortID        : Describes wich Port should be handeled        (in)     +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:    Helperfunction for EDDP_PRMChangePortStateMachine      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMChangePortStateMachine_DoCommit(
	EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    LSA_UINT32                              HwPortIndex,
    LSA_UINT8                               ApplyDefaultPortparams)
{
    /* Deklarations */
    LSA_UINT32 HwPortID;

    /* Initalisation */
    HwPortID = HwPortIndex + 1;

    /* evaluate default params and MediaType */
    if (   (EDD_PRM_APPLY_DEFAULT_PORT_PARAMS == ApplyDefaultPortparams)
        || (EDD_LINK_AUTONEG == pDDB->SWI.LinkStatus[HwPortID].LinkSpeedModeConfiguredParam) )
    {
        EDDP_PRM_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H: ] EDDP_PRMChangePortStateMachine_DoCommit(): NOTE -> ApplyDefaultPortparams: 0x%X or  LinkSpeedModeConfiguredParam: 0x%X is autoneg, HwPortID: 0x%X, MediaType: 0x%X",
            ApplyDefaultPortparams,              
            pDDB->SWI.LinkStatus[HwPortID].LinkSpeedModeConfiguredParam,
            HwPortID,
            pDDB->SWI.LinkStatus[HwPortID].MediaType);

        //apply default parameters
        if (EDD_MEDIATYPE_FIBER_OPTIC_CABLE == pDDB->SWI.LinkStatus[HwPortID].MediaType)
        {
            pDDB->SWI.LinkStatus[HwPortID].LinkSpeed     = EDD_LINK_SPEED_100;
            pDDB->SWI.LinkStatus[HwPortID].LinkDuplexity = EDD_LINK_MODE_FULL;
            pDDB->SWI.LinkStatus[HwPortID].Autoneg       = EDD_AUTONEG_OFF;
        }
        else
        {
            pDDB->SWI.LinkStatus[HwPortID].LinkSpeed     = EDD_LINK_UNKNOWN;
            pDDB->SWI.LinkStatus[HwPortID].LinkDuplexity = EDD_LINK_UNKNOWN;
            pDDB->SWI.LinkStatus[HwPortID].Autoneg       = EDD_AUTONEG_ON;
        }
    }
    else
    {
        //MAUType present
        pDDB->SWI.LinkStatus[HwPortID].LinkSpeed     = EDD_LINK_SPEED_100;
        pDDB->SWI.LinkStatus[HwPortID].LinkDuplexity = EDD_LINK_MODE_FULL;
        pDDB->SWI.LinkStatus[HwPortID].Autoneg       = EDD_AUTONEG_OFF;
    }

    EDDP_SWI_GEN_PhySTS_StateMachine(pDDB, EDDP_PHYSTS_SM_TRIGGER_PLUG_COMMIT, HwPortIndex);

    return EDD_STS_OK_PENDING;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMChangePortStateMachine_DoTransFinish+*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+  Input                 :    LSA_UINT32                       PortID     +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock             (in/out) +*/
/*+  PortID        : Describes wich Port should be handeled        (in)     +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:    Helperfunction for EDDP_PRMChangePortStateMachine      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMChangePortStateMachine_DoTransFinish(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    LSA_UINT32                              HwPortIndex)
{
    LSA_UINT32 HwPortID;
    EDD_RSP    Response;

    HwPortID = HwPortIndex + 1;
    
    /* Set IsPulled to 0, only in state IsPlugged is IsPulled 0 */
    pDDB->SWI.LinkStatus[HwPortID].IsPulled = EDD_PORTMODULE_IS_PLUGGED;

    pDDB->SWI.LinkStatus[HwPortID].PortStatus = EDD_PORT_PRESENT;

    // fire link change indication
    EDDP_GENIndicateLinkChange_Ext (pDDB, HwPortID);

    if (EDD_PHY_STATUS_ENABLED == pDDB->SWI.LinkStatus[HwPortID].PhyStatus)
    {
        //No AdjustLinkState record exists
        Response = pDDB->SWI.PHY_FunctionPointers[HwPortID].EDDP_PHY_SetPowerDown(pDDB->hDDB, pDDB->hSysDev, HwPortID, LSA_FALSE);
        EDDP_ASSERT_FALSE(Response != EDD_STS_OK);
    }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMChangePortStateMachine              +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+  Input                 :    EDDP_PORT_MODULE_SM_STATE_TYPE   Trigger    +*/
/*+  Input                 :    LSA_UINT32                       PortID     +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock             (in/out) +*/
/*+  Trigger       : Trigger to control the statemachine           (in)     +*/
/*+  PortID        : Describes wich Port should be handeled        (in)     +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:    Controls PHY up an down configuration                  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMChangePortStateMachine(
	EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_PRM_PORT_MODULE_SM_TRIGGER_TYPE    Trigger,
    LSA_UINT32                              HwPortIndex,
    EDD_RQB_PRM_CHANGE_PORT_TYPE*           pRQBParams)
{
    EDD_RSP                         Response;
    LSA_UINT32                      HwPortID;
    EDDP_PORT_MODULE_SM_STATE_TYPE  SaveState;

    HwPortID       = HwPortIndex + 1;
    Response       = EDD_STS_OK;
    SaveState      = pDDB->PortModuleSm[HwPortIndex].State;

    switch(pDDB->PortModuleSm[HwPortIndex].State)
    {
        case EDDP_PORT_MODULE_SM_STATE_ISPULLED:
        {
            if(EDDP_PRM_PORT_MODULE_SM_TRIGGER_PULL == Trigger)
            {
                // state persists
            }
            else if(EDDP_PRM_PORT_MODULE_SM_TRIGGER_PLUG_PREPARE == Trigger)
            {
                /* transition is only allowed if PRM Statemachine is in State _PREPARED or _WRITE */
                if ((EDDP_PRM_STATE_PREPARED == pDDB->PRM.PrmState) || (EDDP_PRM_STATE_WRITE == pDDB->PRM.PrmState))
                {
                    /* change state */
                    pDDB->PortModuleSm[HwPortIndex].State = EDDP_PORT_MODULE_SM_STATE_PLUGGING;

                    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H: ] EDDP_PRMChangePortStateMachine(): NOTE -> State transition - old state: 0x%X, new state: 0x%X",
                                        SaveState,
                                        pDDB->PortModuleSm[HwPortIndex].State);

                    /* do things for transition */
                    Response = EDDP_PRMChangePortStateMachine_DoPrepare(pDDB, HwPortIndex);
                    if (EDD_STS_OK == Response)
                    {
                        pRQBParams->PortparamsNotApplicable = pDDB->SWI.LinkStatus[HwPortID].PortparamsNotApplicable;
                    }
                }
                else
                {
                    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H: ] EDDP_PRMChangePortStateMachine(): ERROR -> PRM state machine in wrong state! PrmState=0x%X, Trigger=0x%X, HwPortID: 0x%X",
                                      pDDB->PRM.PrmState,
                                      Trigger,
                                      HwPortID);
                    Response = EDD_STS_ERR_SEQUENCE;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);  
                }
            }
            else
            {
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H: ] EDDP_PRMChangePortStateMachine(): ERROR -> Wrong Trigger for State! PrmState=0x%X, Trigger=0x%X",
                                    pDDB->PRM.PrmState,
                                    Trigger);
                Response = EDD_STS_ERR_SEQUENCE;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);    
            }
        }
        break;

        case EDDP_PORT_MODULE_SM_STATE_PLUGGING:
        {
            if(EDDP_PRM_PORT_MODULE_SM_TRIGGER_PLUG_PREPARE == Trigger)
            {
                // state persists
                // PortParamsNotApplicable is still valid.
            }
            else if(EDDP_PRM_PORT_MODULE_SM_TRIGGER_PULL == Trigger)
            {
                /* do things for transition */
                /* change state */
                pDDB->PortModuleSm[HwPortIndex].State = EDDP_PORT_MODULE_SM_STATE_ISPULLED;

                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H: ] EDDP_PRMChangePortStateMachine(): NOTE -> State transition - old state: 0x%X, new state: 0x%X",
                                    SaveState,
                                    pDDB->PortModuleSm[HwPortIndex].State);
                //Port submodule is still pulled...
            }
            else if (EDDP_PRM_PORT_MODULE_SM_TRIGGER_PLUG_COMMIT == Trigger)
            {
                /* change state */
                pDDB->PortModuleSm[HwPortIndex].State = EDDP_PORT_MODULE_SM_STATE_PLUGINPROGRESS;

                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H: ] EDDP_PRMChangePortStateMachine(): NOTE -> State transition - old state: 0x%X, new state: 0x%X",
                                    SaveState,
                                    pDDB->PortModuleSm[HwPortIndex].State);

                /* do things for transition */
                Response = EDDP_PRMChangePortStateMachine_DoCommit(pDDB, HwPortIndex, pRQBParams->ApplyDefaultPortparams);
            }
            else
            {
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H: ] EDDP_PRMChangePortStateMachine(): ERROR -> Wrong Trigger for State! PrmState=0x%X, Trigger=0x%X",
                                    pDDB->PRM.PrmState,
                                    Trigger);

                Response = EDD_STS_ERR_SEQUENCE;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            }
        }
        break;

        case EDDP_PORT_MODULE_SM_STATE_PLUGINPROGRESS:
        {
            if(EDDP_PRM_PORT_MODULE_SM_TRIGGER_PHY_ISR == Trigger)
            {
                EDDP_GLB_CRT_RQB_HANDLER_PTR_TYPE const pRQBHandler = &pDDB->GlbCrtRQBHandler;
                EDDP_LOCAL_HDB_PTR_TYPE                 pHDB;
                EDD_UPPER_RQB_PTR_TYPE                  pRQB_from_queue;
                    
                /* remove first RQB */
                pRQB_from_queue = EDDP_GlbRQBQueueRemoveFromBegin(&pRQBHandler->RqbInProgressQueue);
                
                if (   (LSA_HOST_PTR_ARE_EQUAL(pRQB_from_queue, LSA_NULL)                  /* valid RQB? */
                    || (EDD_SRV_PRM_CHANGE_PORT != EDD_RQB_GET_SERVICE(pRQB_from_queue))   /* valid service? */
                    || (0 != pRQBHandler->RqbInProgressQueue.Count) )                      /* exclusive RQB? */
                   )
                {
                    EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMChangePortStateMachine(): FATAL -> INCONSISTENCE! RQB wrong or sequence error!");
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                }
                
                /* this happens when transceiver has generated an interrupt */
                /* change state */
                pDDB->PortModuleSm[HwPortIndex].State = EDDP_PORT_MODULE_SM_STATE_ISPLUGGED;

                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H: ] EDDP_PRMChangePortStateMachine(): NOTE -> State transition - old state: 0x%X, new state: 0x%X",
                                    SaveState,
                                    pDDB->PortModuleSm[HwPortIndex].State);

                /* do things for transition */
                EDDP_PRMChangePortStateMachine_DoTransFinish(pDDB, HwPortIndex);

                /* ---------------------------------------------------- */
                /* get Handle for this RQB                              */
                /* ---------------------------------------------------- */
                Response = EDDP_GlbHandleGetHDB (EDD_RQB_GET_HANDLE_LOWER(pRQB_from_queue), &pHDB);

                if (Response != EDD_STS_OK)
                {
                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "EDDP_PRMCommitStateMachine(): Invalid handle lower in RQB!");
                    EDD_RQB_SET_RESPONSE(pRQB_from_queue, EDD_STS_ERR_PARAM);
                    EDDP_RQB_ERROR(pRQB_from_queue);
                }

                /* ---------------------------------------------------- */
                /* finish the PLUG_COMMIT request                       */
                /* ---------------------------------------------------- */
                EDDP_GlbRequestFinish (pHDB, pRQB_from_queue, EDD_STS_OK);

                // trigger the finish events of this Service
                EDDP_GlbTriggerFinishEventAfterRQBConfirmation (pDDB, pHDB, EDD_SRV_PRM_COMMIT);

                /* ---------------------------------------------------- */
                /* call RQB-Handler for other RQBs in Queue             */
                /* ---------------------------------------------------- */
                EDDP_GlbCrtRQBHandler (pDDB);
            }
            else
            {
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H: ] EDDP_PRMChangePortStateMachine(): ERROR -> Wrong Trigger for State! PrmState=0x%X, Trigger=0x%X",
                                    pDDB->PRM.PrmState,
                                    Trigger);
                Response = EDD_STS_ERR_SEQUENCE;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            }
        }
        break;

        case EDDP_PORT_MODULE_SM_STATE_ISPLUGGED:
        {
            if(EDDP_PRM_PORT_MODULE_SM_TRIGGER_PULL == Trigger)
            {
                pDDB->PortModuleSm[HwPortIndex].State = EDDP_PORT_MODULE_SM_STATE_ISPULLED;

                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H: ] EDDP_PRMChangePortStateMachine(): NOTE -> State transition - old state: 0x%X, new state: 0x%X",
                                    SaveState,
                                    pDDB->PortModuleSm[HwPortIndex].State);

                /* do things for transition */
                EDDP_PRMChangePortStateMachine_DoPull(pDDB, HwPortIndex);
            }
            else
            {
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H: ] EDDP_PRMChangePortStateMachine(): ERROR -> Wrong Trigger for State! PrmState=0x%X, Trigger=0x%X",
                                    pDDB->PRM.PrmState,
                                    Trigger);
                Response = EDD_STS_ERR_SEQUENCE;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            }
        }
        break;

        default:
        {
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H: ] EDDP_PRMChangePortStateMachine(): ERROR -> Wrong State! PrmState=0x%X",
                                pDDB->PRM.PrmState);


            Response = EDD_STS_ERR_SEQUENCE;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        }
        break;


    }
    return Response;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMChangePortStateMachine_isPlugged    +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+  Input                 :    LSA_UINT32                       PortIndex  +*/
/*+                                                                         +*/
/*+  Result                :    LSA_BOOL                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock             (in)     +*/
/*+  PortID        : Describes wich Port should be handeled        (in)     +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:    Is the statemachine in state isPlugged                 +*/
/*+                  this function returns LSA_TRUE                         +*/
/*+                  otherwise it returns LSA_FALSE                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMChangePortState_IsPlugged(
	EDDP_LOCAL_DDB_PTR_TYPE const           pDDB,
    LSA_UINT32                              PortIndex)
{
    if(EDDP_PORT_MODULE_SM_STATE_ISPLUGGED == pDDB->PortModuleSm[PortIndex].State)
    {
        return LSA_TRUE;
    }
    else
    {
        return LSA_FALSE;
    }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMChangePortStateMachine_isPulled     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+  Input                 :    LSA_UINT32                       PortIndex  +*/
/*+                                                                         +*/
/*+  Result                :    LSA_BOOL                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock             (in)     +*/
/*+  PortID        : Describes wich Port should be handeled        (in)     +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:    Is the statemachine in state isPulled                  +*/
/*+                  this function returns LSA_TRUE                         +*/
/*+                  otherwise it returns LSA_FALSE                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMChangePortState_IsPulled(
	EDDP_LOCAL_DDB_PTR_TYPE const           pDDB,
    LSA_UINT32                              PortIndex)
{
    if(EDDP_PORT_MODULE_SM_STATE_ISPULLED == pDDB->PortModuleSm[PortIndex].State)
    {
        return LSA_TRUE;
    }
    else
    {
        return LSA_FALSE;
    }
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMChangePortState_IsAnySequenceRunning+*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+  Input                 :    LSA_UINT32 *                     pPortIndex +*/
/*+                                                                         +*/
/*+  Result                :    LSA_BOOL                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock             (in)     +*/
/*+  PortID        : Describes wich Port should be handeled        (out)    +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:    Is the statemachine NOT in state isPulled              +*/
/*+                  and NOT in State isPlugged                             +*/
/*+                  this function returns LSA_TRUE                         +*/
/*+                  otherwise it returns LSA_FALSE                         +*/
/*+                  Function returns with first found Index in             +*/
/*+                  in parameter pPortIndex.                               +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMChangePortState_IsAnySequenceRunning(
	EDDP_LOCAL_DDB_PTR_TYPE const           pDDB,
    LSA_UINT32 *                            pPortIndex)
{
    LSA_UINT32 PortIndex;
    
    for (PortIndex = 0; PortIndex < pDDB->SWI.PortCnt; PortIndex++)
    {
        if((!EDDP_PRMChangePortState_IsPulled(pDDB,PortIndex)) &&
           (!EDDP_PRMChangePortState_IsPlugged(pDDB,PortIndex)))
        {
            if(pPortIndex)
            {
                *pPortIndex = PortIndex;
                return LSA_TRUE;
            }
            else
            {
                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H: ] EDDP_PRMChangePortState_IsAnySequenceRunning(): ERROR -> pPortIndex is 0");
            }
            
        }

    }
    return LSA_FALSE;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMRequestPrmCommit                    +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                         +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         Opcode:     EDD_OPC_REQUEST                 +*/
/*+     LSA_HANDLE_TYPE         Handle:                                     +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user (not used)           +*/
/*+     EDD_SERVICE             Service:    EDD_SRV_PRM_COMMIT              +*/
/*+     LSA_RESPONSE_TYPE       Response:   Return status                   +*/
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:                                     +*/
/*+                                                                         +*/
/*+  pParam points to EDD_UPPER_PRM_WRITE_PTR_TYPE                          +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_SEQUENCE                                      +*/
/*+               EDD_STS_ERR_PRM_INDEX                                     +*/
/*+               EDD_STS_ERR_PRM_DATA                                      +*/
/*+               EDD_STS_ERR_PRM_BLOCK                                     +*/
/*+               EDD_STS_ERR_PRM_PORTID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function handles the EDD_SRV_PRM_COMMIT request      +*/
/*+               We copy record set B to record set A                      +*/
/*+               For all present records we have to indicate the current   +*/
/*+               ErrorStatus. If no error present we have to indicate an   +*/
/*+               empty message for ports some record was written. So all   +*/
/*+               written ports get a flag (doIndicate) to force a indicat. +*/
/*+                                                                         +*/
/*+               COMMIT is only allowed after an END. COMMIT shall only    +*/
/*+               be called if all WRITE and END were successful! CM does.  +*/
/*+                                                                         +*/
/*+               Note: Opcode/Service not checked!                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMRequestPrmCommit(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP                         Response;
    LSA_BOOL                        IsSameIrtActive, IsSameDfpActive, IsSameIRApplicationDataActive;
    EDD_UPPER_PRM_COMMIT_PTR_TYPE   pRqbParam;

#if (EDDP_CFG_TRACE_MODE == 0) 
    LSA_UNUSED_ARG(pRQB);
    LSA_UNUSED_ARG(pHDB);
#endif

    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_PRMRequestPrmCommit(pRQB=0x%X)", pHDB->Handle, pRQB);

    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_PRMRequestPrmCommit(PrmState=0x%X): SWI.IRTState=0x%X", 
        pDDB->PRM.PrmState, pDDB->SWI.IRTState);
    // do not delete the empty spaces in this trace text
    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--]    Recordset: PDSyncDataB=0x%X, PDSyncDataA=0x%X", 
        pDDB->PRM.pRecordsetB->PDSyncData_IsPresent, 
        pDDB->PRM.pRecordsetA->PDSyncData_IsPresent);
    // do not delete the empty spaces in this trace text
    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--]    Recordset: PDIRDataB=0x%X, PDIRDataA=0x%X", 
        pDDB->PRM.pRecordsetB->PDIRData_IsPresent, 
        pDDB->PRM.pRecordsetA->PDIRData_IsPresent);
    // do not delete the empty spaces in this trace text
    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--]    Recordset: PDIRSubframeDataB=0x%X, PDIRSubframeDataA=0x%X", 
        pDDB->PRM.pRecordsetB->PDIRSubframeData_IsPresent, 
        pDDB->PRM.pRecordsetA->PDIRSubframeData_IsPresent);
    // do not delete the empty spaces in this trace text
    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--]    Recordset: PDIRApplicationDataB=0x%X, PDIRApplicationDataA=0x%X", 
        pDDB->PRM.pRecordsetB->PDIRApplicationData_IsPresent, 
        pDDB->PRM.pRecordsetA->PDIRApplicationData_IsPresent);
    // do not delete the empty spaces in this trace text
    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--]    Recordset: PDNRTFeedInLoadLimitationB=0x%X, PDNRTFeedInLoadLimitationA=0x%X", 
        pDDB->PRM.pRecordsetB->PDNRTFeedInLoadLimitation_IsPresent, 
        pDDB->PRM.pRecordsetA->PDNRTFeedInLoadLimitation_IsPresent);

    Response                                = EDD_STS_OK;
    IsSameIrtActive                         = LSA_FALSE;
    IsSameDfpActive                         = LSA_FALSE;
    IsSameIRApplicationDataActive           = LSA_FALSE;
    pDDB->PRM.PrmCommitSm.IsDfpPresentA     = LSA_FALSE;
    pDDB->PRM.PrmCommitSm.IsDfpPresentB     = LSA_FALSE;
    pRqbParam                               = (EDD_UPPER_PRM_COMMIT_PTR_TYPE) pRQB->pParam;

    /* ------------------------------------ */
    /* Flag isPrmCommitRunning              */
    /* ------------------------------------ */
    // set flag PrmCommit is running
    // if PrmCommit is synchron, this flag will be deleted after PrmCommit
    // if PrmCommit is asynchron (state machine) it will be deleted in last state of PrmCommit state machine 
    // after EDDP_GlbRequestFinish() of PrmCommit-RQB
    pDDB->PRM.isPrmCommitRunning = LSA_TRUE;

    // trigger the PrmCommit finish after PrmCommit confirmation
    pDDB->PRM.isTriggerPrmCommitFinish = LSA_TRUE;

    /* -------------------------------------------------------- */
    /* delete RsvIntervalRed values in PrmCommit-RQB            */
    /* they will be updated before PrmCommit confirmation       */
    /* -------------------------------------------------------- */
    EDDP_PRMPDIRDataSetupRsvIntervalRedValues (pDDB, &(pRqbParam->RsvIntervalRed), LSA_FALSE, LSA_FALSE);

    switch (pDDB->PRM.PrmState)
    {
        case EDDP_PRM_STATE_END:
        {
            /* copy values in pDDB - it is needed in EDDP_PRMCopySetB2A */
            LSA_UINT16 PortIterator;

            for (PortIterator = 0; PortIterator < pDDB->SWI.PortCnt; PortIterator++)
            {
                if ((EDD_PRM_APPLY_RECORD_PORT_PARAMS != pRqbParam->ApplyDefaultPortparams[PortIterator])
                    && (EDD_PRM_APPLY_DEFAULT_PORT_PARAMS != pRqbParam->ApplyDefaultPortparams[PortIterator]))
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmCommit(): ApplyDefaultPortparams (%d) illegal value for UsrPortIndex %d",
                        pHDB->Handle, pRqbParam->ApplyDefaultPortparams[PortIterator], PortIterator);
                    break;
                }
            }
            if (EDD_STS_OK != Response)
            {
                break;
            }

            /* ------------------------------------------------ */
            /* set flags before CopySetB2A                      */
            /* ------------------------------------------------ */
            // is same IRT configuration already active ?
            if ( EDDP_PRMPDIRDataIsSameIrtActive(pDDB) )
            {
                IsSameIrtActive = LSA_TRUE;
            }

            // is same DFP configuration already active ?
            if ( EDDP_PRMPDIRSubframeDataIsSameDfpActive(pDDB) )
            {
                IsSameDfpActive = LSA_TRUE;
            }

            // is same IRApplicationData configuration already active ?
            if ( EDDP_PRMPDIRDataIsSameIRApplicationDataActive(pDDB) )
            {
                IsSameIRApplicationDataActive = LSA_TRUE;
            }

            // is DFP present in new Recordset B ?
            if ( EDDP_PRMPDIRSubframeDataIsDfpPresent(pDDB, LSA_FALSE) )
            {
                pDDB->PRM.PrmCommitSm.IsDfpPresentB = LSA_TRUE;
            }

            // is DFP already active in Recordset A ?
            if ( EDDP_PRMPDIRSubframeDataIsDfpPresent(pDDB, LSA_TRUE) )
            {
                pDDB->PRM.PrmCommitSm.IsDfpPresentA = LSA_TRUE;
            }

            /* ------------------------------------------------ */
            /* copy Record(B) to Record(A)                      */
            /*  - always before PrmCommit-SM                    */
            /*  - PrmCommit-SM uses the Record(A)               */
            /* ------------------------------------------------ */

            /* copy values in pDDB - it is needed in EDDP_PRMCopySetB2A */
            for (PortIterator = 0; PortIterator < pDDB->SWI.PortCnt; PortIterator++)
            {
                pDDB->PRM.ApplyDefaultPortParams[PortIterator] = pRqbParam->ApplyDefaultPortparams[PortIterator];
            }

            EDDP_PRMCopySetB2A (pDDB);
            EDDP_PRMSetBToNotPresent (pDDB);
            pDDB->PRM.PrmState = EDDP_PRM_STATE_PREPARED;

            /* ------------------------------------------------ */
            /* actualize PDNRTFeedInLoadLimitation              */
            /* ------------------------------------------------ */
            if(EDD_STS_OK == Response)
            {
                 Response = EDDP_PRMPDNRTFeedInLoadLimitationUpdate_Commit(pDDB);
            }
            
            /* ------------------------------------------------ */
            /* actualize PDSetDefaultPortStates                 */
            /* ------------------------------------------------ */
            // nothing todo
            //if(EDD_STS_OK == Response)
            //{
            //     Response = EDDP_PRMPDSetDefaultPortStatesUpdate_Commit(pDDB);
            //}
            
            /* ------------------------------------------------ */
            /* PrmCommit state machine                          */
            /*  - is only for IRT parametrization               */
            /* ------------------------------------------------ */
            if ((IsSameIrtActive) && (IsSameDfpActive) && (IsSameIRApplicationDataActive))
            {
                // skip: same IRT configuration is already active
                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_PRMRequestPrmCommit(): Skip IRT configuration - it has not changed");

                // set RsvIntervalRed values in PrmCommit-RQB to valid
                EDDP_PRMPDIRDataSetupRsvIntervalRedValues (pDDB, &(pRqbParam->RsvIntervalRed), LSA_TRUE, LSA_TRUE);
            }
            else
            {
                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_PRMRequestPrmCommit(): IRT configuration has changed - start PrmCommit state machine");

                // start the PrmCommit state machine
                EDDP_ASSERT_FALSE(pDDB->PRM.PrmCommitSm.State != EDDP_PRM_COMMIT_STATE_IDLE);
                pDDB->PRM.PrmCommitSm.State = EDDP_PRM_COMMIT_STATE_START;
                EDDP_PRMCommitStateMachine (pDDB, EDD_STS_OK);
                // set Response for RQBHandler
                Response = EDD_STS_OK_PENDING;
            }
        }
        break;

        case EDDP_PRM_STATE_PREPARED:
        case EDDP_PRM_STATE_WRITE:
        default:
        {
            Response = EDD_STS_ERR_SEQUENCE;
            EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmCommit(): ERROR -> Wrong sequence! PrmState=0x%X",
                pHDB->Handle, pDDB->PRM.PrmState);
        }
        break;
    }

    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMRequestPrmCommit(): Response=0x%X", Response);

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMRequestPrmWrite                     +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                         +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         Opcode:     EDD_OPC_REQUEST                 +*/
/*+     LSA_HANDLE_TYPE         Handle:                                     +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user (not used)           +*/
/*+     EDD_SERVICE             Service:    EDD_SRV_PRM_WRITE               +*/
/*+     LSA_RESPONSE_TYPE       Response:   Return status                   +*/
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:                                     +*/
/*+                                                                         +*/
/*+  pParam points to EDD_UPPER_PRM_WRITE_PTR_TYPE                          +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_SEQUENCE                                      +*/
/*+               EDD_STS_ERR_PRM_INDEX                                     +*/
/*+               EDD_STS_ERR_PRM_DATA                                      +*/
/*+               EDD_STS_ERR_PRM_BLOCK                                     +*/
/*+               EDD_STS_ERR_PRM_PORTID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function handles the EDD_SRV_PRM_WRITE request       +*/
/*+                                                                         +*/
/*+               WRITE is only allowed after PREPARE or WRITE              +*/
/*+                                                                         +*/
/*+               Note: A PRM error will not cause a state-change!          +*/
/*+               Note: Opcode/Service not checked!                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMRequestPrmWrite(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP	                            Response;
    EDDP_DDB_PRM_PTR_TYPE               pPrm;
    EDD_UPPER_PRM_WRITE_PTR_TYPE        pPrmWrite;
    EDDP_PRM_DETAIL_ERR_PTR_TYPE        pDetailErr;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pHDB);
#endif

	Response    = EDD_STS_OK; 
    pDetailErr  = &pDDB->PrmDetailErr;
    pPrm        = &pDDB->PRM;
    pPrmWrite   = (EDD_UPPER_PRM_WRITE_PTR_TYPE) pRQB->pParam;

    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_PRMRequestPrmWrite(pRQB=0x%X): PrmState=0x%X", 
        pHDB->Handle, pRQB, pDDB->PRM.PrmState);

    EDDP_PRM_INIT_DETAIL_ERR(pDetailErr);

    if (LSA_HOST_PTR_ARE_EQUAL(pRQB->pParam, LSA_NULL))
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmWrite(): ERROR -> RQB->pParam=NULL, Response=0x%X", 
            pHDB->Handle, Response);
    }
    else
    {
        if (   (pPrmWrite->edd_port_id > pDDB->SWI.PortCnt) 
            || (LSA_HOST_PTR_ARE_EQUAL(pPrmWrite->record_data, LSA_NULL))
            || (   (EDD_PRM_PARAMS_ASSIGNED_REMOTE != pPrmWrite->Local)
                && (EDD_PRM_PARAMS_ASSIGNED_LOCALLY != pPrmWrite->Local) )
           )
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
            EDDP_PRM_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmWrite(): ERROR -> RQB-Param error! record_index=0x%X, edd_port_id(%d), SWI.PortCnt(%d), record_data(0x%X), Local(%d)", 
                pHDB->Handle, pPrmWrite->record_index, pPrmWrite->edd_port_id, pDDB->SWI.PortCnt, pPrmWrite->record_data, pPrmWrite->Local);
        }
        else
        {
            if ((pPrm->PrmState == EDDP_PRM_STATE_PREPARED) || (pPrm->PrmState == EDDP_PRM_STATE_WRITE))
            {	
                switch (pPrmWrite->record_index)
                {
                    #ifdef EDDP_PRM_TEST_RECORD_ON
                    /* ---------------------------------------- */
                    /* only for test                            */
                    /* ---------------------------------------- */
                    case 0x00011111: // Enable RED Tx and Rx on HwPortID 1
                        EDDP_SWI_TEST_Enable_Only_Red_Phase(pDDB, 1);
                    break;

                    case 0x00011112: // Enable RED Tx and Rx on HwPortID 2
                        EDDP_SWI_TEST_Enable_Only_Red_Phase(pDDB, 2);
                    break;
                    
                    case 0x00011121: // Enable RED and GREEN Tx and Rx on HwPortID 1                
                        EDDP_SWI_TEST_Enable_Red_and_Green_Phase(pDDB, 1);
                    break;
                    
                    case 0x00011122: // Enable RED and GREEN Tx and Rx on HwPortID 2                  
                        EDDP_SWI_TEST_Enable_Red_and_Green_Phase(pDDB, 2);           
                    break;                     
                    #endif

                    /* ---------------------------------------- */
                    /* PDIRData                                 */
                    /* ---------------------------------------- */
                    case EDDP_PRM_PDIRDATA_RECORD_INDEX:
                    {
                        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmWrite(): PDIRData(0x%X)", 
                            pHDB->Handle, pPrmWrite->record_index);

                        if (pPrmWrite->edd_port_id == EDD_PORT_ID_AUTO)
                        {
                            if (   (pPrmWrite->record_data_length == pDDB->PRM.pRecordsetA->PDIRData_Length)
                                && (EDDP_MEMCMP_COMMON(pPrmWrite->record_data, pDDB->PRM.pRecordsetA->pPDIRData, pPrmWrite->record_data_length)) 
                               )
                            {
                                // same record is already active -> potentially nothing to do at PRM_Commit
                                pDDB->PRM.pRecordsetB->PDIRData_Skip = LSA_TRUE;
                            }
                            else
                            {
                                pDDB->PRM.pRecordsetB->PDIRData_Skip = LSA_FALSE; 
                            }  		            

                            // check "record_data_length"
                            if (pPrmWrite->record_data_length > pDDB->PRM.pRecordsetB->PDIRData_Size)
                            {                  
                                Response = EDD_STS_ERR_PRM_DATA;
                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMRequestPrmWrite(): ERROR -> PrmRecord PDIRData: record_data_length(%d) > PDIRData_Size(%d)", 
                                    pPrmWrite->record_data_length, pDDB->PRM.pRecordsetB->PDIRData_Size);
                            }
                            else
                            {
                                // store SlotNumber and SubslotNumber from RQB for later check with values stored in the actual record
                                pDDB->PRM.pPDIRData_B->SlotNumber         = pPrmWrite->slot_number;
                                pDDB->PRM.pPDIRData_B->SubslotNumber      = pPrmWrite->subslot_number;
                                // store record length from RQB in order to check it later against record length from record 
                                pDDB->PRM.pRecordsetB->PDIRData_Length    = pPrmWrite->record_data_length;

                                Response = EDDP_PRMPDIRDataRecord_Write (pDDB, pPrmWrite->record_data);

                                if (Response == EDD_STS_OK)
                                {
                                    Response = EDDP_SWI_Check_PDIRData (pDDB);
                                }

                                // IsPresent will be set in Write(), but will not cleared in Check() if needed
                                if (Response != EDD_STS_OK)
                                {
                                    pDDB->PRM.pRecordsetB->PDIRData_IsPresent = LSA_FALSE;
                                }
                            }
                        }
                        else
                        {
                            Response = EDD_STS_ERR_PRM_PORTID;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmWrite(): ERROR -> PrmRecord PDIRData: edd_port_id(%d) is not EDD_PORT_ID_AUTO(%d)", 
                                pHDB->Handle, pPrmWrite->edd_port_id, EDD_PORT_ID_AUTO);
                        }
                    }
                    break;

                    /* ---------------------------------------- */
                    /* PDIRSubframeData                         */
                    /* ---------------------------------------- */
                    case EDDP_PRM_PDIRSUBFRAMEDATA_RECORD_INDEX:
                    {
                        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"[H:%2X] EDDP_PRMRequestPrmWrite(): PDIRSubframeData(0x%X)", 
                            pHDB->Handle, pPrmWrite->record_index);

                        if (pPrmWrite->edd_port_id == EDD_PORT_ID_AUTO)
                        {
                            if (   (pPrmWrite->record_data_length == pDDB->PRM.pRecordsetA->PDIRSubframeData_Length)
                                && (EDDP_MEMCMP_COMMON(pPrmWrite->record_data, pDDB->PRM.pRecordsetA->pPDIRSubframeData, pPrmWrite->record_data_length)) 
                               )
                            {
                                // same record is already active -> potentially nothing to do at PRM_Commit
                                pDDB->PRM.pRecordsetB->PDIRSubframeData_Skip = LSA_TRUE;
                            }
                            else
                            {
                                pDDB->PRM.pRecordsetB->PDIRSubframeData_Skip = LSA_FALSE; 
                            }  		            

                            // check "record_data_length"
                            if (pPrmWrite->record_data_length > pDDB->PRM.pRecordsetB->PDIRSubframeData_Size)
                            {
                                Response = EDD_STS_ERR_PRM_DATA;
                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMRequestPrmWrite(): ERROR -> PrmRecord PDIRSubframeData: record_data_length(%d) > PDIRSubframeData_Size(%d)", 
                                    pPrmWrite->record_data_length, pDDB->PRM.pRecordsetB->PDIRSubframeData_Size);
                            }
                            // check "MaxNrPackframesX" == MaxDFPFrames
                            else if (0 == pDDB->FeatureSupport.MaxDFPFrames)
                            {
                                Response = EDD_STS_ERR_PRM_INDEX;
                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMRequestPrmWrite(): ERROR -> PrmRecord PDIRSubframeData: MaxDFPFrames = 0");                            
							}
                            else
                            {
                                // store SlotNumber and SubslotNumber from RQB for later check with values stored in the actual record
                                pDDB->PRM.pPDIRSubframeData_B->SlotNumber         = pPrmWrite->slot_number;
                                pDDB->PRM.pPDIRSubframeData_B->SubslotNumber      = pPrmWrite->subslot_number;
                                // store record length from RQB in order to check it later against record length from record 
                                pDDB->PRM.pRecordsetB->PDIRSubframeData_Length    = pPrmWrite->record_data_length;

                                Response = EDDP_PRMPDIRSubframeDataRecord_Write (pDDB, pPrmWrite->record_data);
                            }
                        }
                        else
                        {
                            Response = EDD_STS_ERR_PRM_PORTID;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmWrite(): ERROR -> PrmRecord PDIRSubframeData: edd_port_id(%d) is not EDD_PORT_ID_AUTO(%d)", 
                                pHDB->Handle, pPrmWrite->edd_port_id, EDD_PORT_ID_AUTO);
                        }
                    }
                    break;

                    /* ---------------------------------------- */
                    /* PDSyncData                               */
                    /* ---------------------------------------- */
                    case EDDP_PRM_PDSYNCDATA_RECORD_INDEX:
                    {
                        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"[H:%2X] EDDP_PRMRequestPrmWrite(): PDSyncData(0x%X)", 
                            pHDB->Handle, pPrmWrite->record_index);

                        if (pPrmWrite->edd_port_id == EDD_PORT_ID_AUTO)
                        {
                            if (   (pPrmWrite->record_data_length == pDDB->PRM.pRecordsetA->PDSyncData_Length)
                                && (EDDP_MEMCMP_COMMON(pPrmWrite->record_data, pDDB->PRM.pRecordsetA->pPDSyncData, pPrmWrite->record_data_length)) 
                               )
                            {
                                // same record is already active -> potentially nothing to do at PRM_Commit
                                pDDB->PRM.pRecordsetB->PDSyncData_Skip = LSA_TRUE;
                            }
                            else
                            {
                                pDDB->PRM.pRecordsetB->PDSyncData_Skip = LSA_FALSE;                    
                            }

                            // check "record_data_length"
                            if (pPrmWrite->record_data_length > pDDB->PRM.pRecordsetB->PDSyncData_Size)
                            {			        
                                Response = EDD_STS_ERR_PRM_DATA;
                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);                            
                                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMRequestPrmWrite(): ERROR -> PrmRecord PDSyncData: record_data_length(%d) > PDSyncData_Size(%d)", 
                                    pPrmWrite->record_data_length, pDDB->PRM.pRecordsetB->PDSyncData_Size);
                            }
                            else
                            {
                                // store record length from RQB in order to check it later against record length from record 
                                pDDB->PRM.pRecordsetB->PDSyncData_Length = pPrmWrite->record_data_length;
                                
                                Response = EDDP_PRMPDSyncDataRecord_Write (pDDB, pPrmWrite->record_data);
                            }
                        }
                        else
                        {
                            Response = EDD_STS_ERR_PRM_PORTID;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmWrite(): ERROR -> PrmRecord PDSyncData: edd_port_id(%d) is not EDD_PORT_ID_AUTO(%d)!", 
                                pHDB->Handle, pPrmWrite->edd_port_id, EDD_PORT_ID_AUTO);
                        }
                    }
                    break;

                    /* ---------------------------------------- */
                    /* PDInterfaceMrpDataAdjust                 */
                    /* ---------------------------------------- */
                    case EDDP_PRM_PDINTERFACE_MRP_DATA_ADJUST_RECORD_INDEX:
                    {
                        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"[H:%2X] EDDP_PRMRequestPrmWrite(): PDInterfaceMrpDataAdjust(0x%X)", 
                            pHDB->Handle, pPrmWrite->record_index);

                        if (   (pPrmWrite->record_data_length == pDDB->PRM.pRecordsetA->PDInterfaceMrpDataAdjust_Length)
                            && (EDDP_MEMCMP_COMMON(pPrmWrite->record_data, pDDB->PRM.pRecordsetA->pPDInterfaceMrpDataAdjust, pPrmWrite->record_data_length))
                           )
                        {
                            // same record is already active -> potentially nothing to do at PRM_Commit
                            pDDB->PRM.pRecordsetB->PDInterfaceMrpDataAdjust_Skip = LSA_TRUE;
                        }
                        else
                        {
                            pDDB->PRM.pRecordsetB->PDInterfaceMrpDataAdjust_Skip = LSA_FALSE;                    
                        }

                        // check "record_data_length"
                        if (pPrmWrite->record_data_length > pDDB->PRM.pRecordsetB->PDInterfaceMrpDataAdjust_Size)
                        {
                            Response = EDD_STS_ERR_PRM_DATA;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMRequestPrmWrite(): ERROR -> PrmRecord PDInterfaceMrpDataAdjust: record_data_length(%d) > PDInterfaceMrpDataAdjust_Size(%d)", 
                                pPrmWrite->record_data_length, pDDB->PRM.pRecordsetB->PDInterfaceMrpDataAdjust_Size);
                        }
                        else
                        {
                            if (pPrmWrite->edd_port_id == EDD_PORT_ID_AUTO)
                            {
                                // store record in record set B                   
                                EDDP_MEMCOPY_COMMON(pDDB->PRM.pRecordsetB->pPDInterfaceMrpDataAdjust, pPrmWrite->record_data, pPrmWrite->record_data_length);
                                pDDB->PRM.pRecordsetB->PDInterfaceMrpDataAdjust_Length    = pPrmWrite->record_data_length;
                                pDDB->PRM.pRecordsetB->PDInterfaceMrpDataAdjust_IsPresent = LSA_TRUE; 			    
                            }
                            else
                            {
                                Response = EDD_STS_ERR_PRM_PORTID;
                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMRequestPrmWrite(): ERROR -> PrmRecord PDInterfaceMrpDataAdjust: edd_port_id(%d) is not EDD_PORT_ID_AUTO(%d)!", 
                                    pPrmWrite->edd_port_id, EDD_PORT_ID_AUTO);
                            }
                        }
                    }
                    break;

                    /* ---------------------------------------- */
                    /* PDPortMrpDataAdjust                      */
                    /* ---------------------------------------- */
                    case EDDP_PRM_PDPORT_MRP_DATA_ADJUST_RECORD_INDEX:
                    {
                        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmWrite(): PDPortMrpDataAdjust(0x%X)", 
                            pHDB->Handle, pPrmWrite->record_index);

                        if ((pPrmWrite->edd_port_id != EDD_PORT_ID_AUTO) && (pPrmWrite->edd_port_id <= pDDB->SWI.PortCnt))
                        {
                            if (   (pPrmWrite->record_data_length == pDDB->PRM.pRecordsetA->PDPortMrpDataAdjust[pPrmWrite->edd_port_id].Record_Length)
                                && (EDDP_MEMCMP_COMMON(pPrmWrite->record_data, pDDB->PRM.pRecordsetA->PDPortMrpDataAdjust[pPrmWrite->edd_port_id].pRecord, pPrmWrite->record_data_length))
                               )
                            {
                                // same record is already active -> potentially nothing to do at PRM_Commit
                                pDDB->PRM.pRecordsetB->PDPortMrpDataAdjust[pPrmWrite->edd_port_id].Record_Skip = LSA_TRUE;
                            }
                            else
                            {
                                pDDB->PRM.pRecordsetB->PDPortMrpDataAdjust[pPrmWrite->edd_port_id].Record_Skip = LSA_FALSE;                        
                            }

                            // check "record_data_length"
                            if (pPrmWrite->record_data_length > pDDB->PRM.pRecordsetB->PDPortMrpDataAdjust[pPrmWrite->edd_port_id].Record_Size)
                            {
                                Response = EDD_STS_ERR_PRM_DATA;
                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMRequestPrmWrite(): ERROR -> PrmRecord PDPortMrpDataAdjust: record_data_length(%d) > Record_Size(%d)", 
                                    pPrmWrite->record_data_length, pDDB->PRM.pRecordsetB->PDPortMrpDataAdjust[pPrmWrite->edd_port_id].Record_Size);
                            }
                            else
                            {                        
                                // store record in record set B
                                EDDP_MEMCOPY_COMMON(pDDB->PRM.pRecordsetB->PDPortMrpDataAdjust[pPrmWrite->edd_port_id].pRecord, pPrmWrite->record_data, pPrmWrite->record_data_length);
                                // evalute counter only if not yet present for this PortID
                                if ( !(pDDB->PRM.pRecordsetB->PDPortMrpDataAdjust[pPrmWrite->edd_port_id].Record_IsPresent) )
                                    pDDB->PRM.MRPPort.RecordCountInSetB++;
                                pDDB->PRM.pRecordsetB->PDPortMrpDataAdjust[pPrmWrite->edd_port_id].Record_Length      = pPrmWrite->record_data_length;   
                                pDDB->PRM.pRecordsetB->PDPortMrpDataAdjust[pPrmWrite->edd_port_id].Record_IsPresent   = LSA_TRUE; 
                            }                        
                        }
                        else
                        {
                            Response = EDD_STS_ERR_PRM_PORTID;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMRequestPrmWrite(): ERROR -> PrmRecord PDPortMrpDataAdjust: edd_port_id(%d) is not valid!", 
                                pPrmWrite->edd_port_id);
                        }			        
                    }
                    break;

                    /* ---------------------------------------- */
                    /* PDPortDataAdjust                         */
                    /* ---------------------------------------- */
                    case EDDP_PRM_PDPORT_DATA_ADJUST_RECORD_INDEX:
                    {
                        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmWrite(): PDPortDataAdjust(0x%X)", 
                            pHDB->Handle, pPrmWrite->record_index);

                        if ((pPrmWrite->edd_port_id > EDD_PORT_ID_AUTO) && (pPrmWrite->edd_port_id <= pDDB->SWI.PortCnt))
                        {
                            if (   (pPrmWrite->record_data_length == pDDB->PRM.pRecordsetA->PDPortDataAdjust[pPrmWrite->edd_port_id].Record_Length)
                                && (EDDP_MEMCMP_COMMON(pPrmWrite->record_data, pDDB->PRM.pRecordsetA->PDPortDataAdjust[pPrmWrite->edd_port_id].pRecord, pPrmWrite->record_data_length))
                               )
                            {
                                // same record is already active -> potentially nothing to do at PRM_Commit
                                pDDB->PRM.pRecordsetB->PDPortDataAdjust[pPrmWrite->edd_port_id].Record_Skip = LSA_TRUE;
                            }
                            else
                            {
                                pDDB->PRM.pRecordsetB->PDPortDataAdjust[pPrmWrite->edd_port_id].Record_Skip = LSA_FALSE;                        
                            }                    

                            // store SlotNumber and SubslotNumber from RQB for later check with values stored in the actual record
                            pDDB->PRM.PortDataAdjust.Parameters[pPrmWrite->edd_port_id].SlotNumber    = pPrmWrite->slot_number;
                            pDDB->PRM.PortDataAdjust.Parameters[pPrmWrite->edd_port_id].SubslotNumber = pPrmWrite->subslot_number;

                            Response = EDDP_PRMPDPortDataAdjustRecord_Write (pDDB, pPrmWrite->record_data, pPrmWrite->edd_port_id);			             		    

                            if (Response != EDD_STS_OK)
                            {
                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMRequestPrmWrite(): ERROR -> PDPortDataAdjustRecord_Write is not ok!");
                            }
                        }
                        else
                        {                        
                            Response = EDD_STS_ERR_PRM_PORTID;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMRequestPrmWrite(): ERROR -> PrmRecord PDPortDataAdjust: edd_port_id(%d) is not valid!", 
                                pPrmWrite->edd_port_id);
                        }
                    }
                    break;

                    /* ---------------------------------------- */
                    /* PDNCDataCheck                            */
                    /* ---------------------------------------- */
                    case EDDP_PRM_PDNC_DATA_CHECK_RECORD_INDEX:
                    {
                        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmWrite(): PDNCDataCheck(0x%X)", 
                            pHDB->Handle, pPrmWrite->record_index);

                        if (   (pPrmWrite->record_data_length == pDDB->PRM.pRecordsetA->PDNCDataCheck_Length)
                            && (EDDP_MEMCMP_COMMON(pPrmWrite->record_data, pDDB->PRM.pRecordsetA->pPDNCDataCheck, pPrmWrite->record_data_length))
                           )
                        {
                            // same record is already active -> do nothing
                            pDDB->PRM.pRecordsetB->PDNCDataCheck_Skip = LSA_TRUE;
                        }
                        else
                        {
                            pDDB->PRM.pRecordsetB->PDNCDataCheck_Skip = LSA_FALSE;                    
                        }

                        // check "record_data_length"
                        if (pPrmWrite->record_data_length > pDDB->PRM.pRecordsetB->PDNCDataCheck_Size)
                        {			        
                            Response = EDD_STS_ERR_PRM_DATA;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMRequestPrmWrite(): ERROR -> PrmRecord PDNCDataCheck: record_data_length(%d) > PDNCDataCheck_Size(%d)", 
                                pPrmWrite->record_data_length, pDDB->PRM.pRecordsetB->PDNCDataCheck_Size);
                        }
                        else
                        {
                            if (pPrmWrite->edd_port_id == EDD_PORT_ID_AUTO)
                            {   
                                // store record length from RQB in order to check it later against record length from record 
                                pDDB->PRM.pRecordsetB->PDNCDataCheck_Length = pPrmWrite->record_data_length;

                                Response = EDDP_PRMPDNCDataCheckRecord_Write (pDDB, pPrmWrite->record_data);

                                if (Response != EDD_STS_OK)
                                {
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMRequestPrmWrite(): ERROR -> PDNCDataCheckRecord_Write is not ok!");
                                }
                            }
                            else
                            {
                                Response = EDD_STS_ERR_PRM_PORTID;
                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMRequestPrmWrite(): ERROR -> PrmRecord PDNCDataCheck: edd_port_id(%d) is not EDD_PORT_ID_AUTO(%d)!", 
                                    pPrmWrite->edd_port_id, EDD_PORT_ID_AUTO);
                            }			            
                        }	
                    }
                    break;

                    /* ---------------------------------------- */
                    /* PDSCFDataCheck                           */
                    /* ---------------------------------------- */
                    case EDDP_PRM_PDSCF_DATA_CHECK_RECORD_INDEX:
                    {
                        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmWrite(): PDSCFDataCheck(0x%X)", 
                            pHDB->Handle, pPrmWrite->record_index);

                        if (pPrmWrite->record_data_length > pDDB->PRM.PDSCFDataCheck.PDSCFDataCheck_Size)
                        {
                            Response = EDD_STS_ERR_PRM_DATA;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMRequestPrmWrite(): ERROR -> PrmRecord PDSCFDataCheck: record_data_length(%d) > PDSCFDataCheck_Size(%d)", 
                                pPrmWrite->record_data_length, pDDB->PRM.PDSCFDataCheck.PDSCFDataCheck_Size);
                        }
                        else
                        {
                            // store record length from RQB in order to check it later against record length from record
                            pDDB->PRM.PDSCFDataCheck.PDSCFDataCheck_Length = pPrmWrite->record_data_length;

                            Response = EDDP_PRMPDSCFDataCheckRecord_Write (pDDB, pPrmWrite->record_data);
                        }
                    }
                    break;

                    /* ---------------------------------------- */
                    /* PDIRApplicationData                      */
                    /* ---------------------------------------- */
                    case EDDP_PRM_PDIRAPPLICATIONDATA_RECORD_INDEX:
                    {
                        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmWrite(): PDIRApplicationData(0x%X)", 
                            pHDB->Handle, pPrmWrite->record_index);

                        if (   (pPrmWrite->record_data_length == pDDB->PRM.pRecordsetA->PDIRApplicationData_Length)
                            && (EDDP_MEMCMP_COMMON(pPrmWrite->record_data, pDDB->PRM.pRecordsetA->pPDIRApplicationData, pPrmWrite->record_data_length)) 
                           )
                        {
                            // same record is already active -> potentially nothing to do at PRM_Commit
                            pDDB->PRM.pRecordsetB->PDIRApplicationData_Skip = LSA_TRUE;
                        }
                        else
                        {
                            pDDB->PRM.pRecordsetB->PDIRApplicationData_Skip = LSA_FALSE; 
                        }  		            

                        // check "record_data_length"
                        if (pPrmWrite->record_data_length > pDDB->PRM.pRecordsetB->PDIRApplicationData_Size)
                        {
                            Response = EDD_STS_ERR_PRM_DATA;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMRequestPrmWrite(): ERROR -> PrmRecord PDIRApplicationData: record_data_length(%d) > PDIRApplicationData_Size(%d)", 
                                pPrmWrite->record_data_length, pDDB->PRM.pRecordsetB->PDIRApplicationData_Size);
                        }
                        else
                        {
                            // store record length from RQB in order to check it later against record length from record
                            pDDB->PRM.pRecordsetB->PDIRApplicationData_Length = pPrmWrite->record_data_length;

                            Response = EDDP_PRMPDIRApplicationDataRecord_Write (pDDB, pPrmWrite->record_data);
                        }
                    }
                    break;
                    
                    /* ---------------------------------------- */
                    /* PDNRTFeeInLoadLimitation                 */
                    /* ---------------------------------------- */
                    case EDDP_PRM_PDNRT_FEED_IN_LOAD_LIMITATION_RECORD_INDEX:
                    {
                        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, 
                                    "[H:%2X] EDDP_PRMRequestPrmWrite(): PDNRTFeedInLoadLimitation(0x%X)", 
                            pHDB->Handle, pPrmWrite->record_index);                 

                        // check "edd_port_id" - only interface allowed
                        if( 0 != pPrmWrite->edd_port_id )
                        {
                            Response = EDD_STS_ERR_PRM_PORTID;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,
                                        "[H:--] EDDP_PRMRequestPrmWrite(): ERROR -> PrmRecord PDNRTFeedInLoadLimitation: only interface port (0) allowed (edd_port_id == 0x%x)",
                                        pPrmWrite->edd_port_id);
                        }
                        // check "record_data_length"
                        if(EDD_STS_OK == Response)
                        {
                            if (pPrmWrite->record_data_length > pDDB->PRM.pRecordsetB->PDNRTFeedInLoadLimitation_Size)
                            {
                                Response = EDD_STS_ERR_PRM_DATA;
                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, 
                                            "[H:--] EDDP_PRMRequestPrmWrite(): ERROR -> PrmRecord PDNRTFeedInLoadLimitation: record_data_length(%d) > PDNRTFeedInLoadLimitation_Size(%d)", 
                                    pPrmWrite->record_data_length, pDDB->PRM.pRecordsetB->PDNRTFeedInLoadLimitation_Size);
                            }
                        }
                        if(EDD_STS_OK == Response)
                        {
                            // store record length from RQB in order to check it later against record length from record
                            pDDB->PRM.pRecordsetB->PDNRTFeedInLoadLimitation_Length = pPrmWrite->record_data_length;

                            Response = EDDP_PRMPDNRTFeedInLoadLimitationRecord_Write(pDDB, pPrmWrite->record_data);
                        }
                    }
                    break;

                    /* ---------------------------------------- */
                    /* PDSetDefaultPortStates                   */
                    /* ---------------------------------------- */
                    case EDDP_PRM_PDSET_DEFAULT_PORT_STATES_RECORD_INDEX:
                    {
                        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, 
                                    "[H:%2X] EDDP_PRMRequestPrmWrite(): PDSetDefaultPortStates(0x%X)", 
                            pHDB->Handle, pPrmWrite->record_index);                 

                        if (EDD_FEATURE_ENABLE != pDDB->FeatureSupport.DLRExtensions)
                        {
                            Response = EDD_STS_ERR_PRM_INDEX; /* special case we use Index error here */;  //!!! adjust error code ???
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,
                                        "[H:--] EDDP_PRMRequestPrmWrite(): ERROR -> PDSetDefaultPortStates DLRExtensions not ENABLED == 0x%x)",
                                        pDDB->FeatureSupport.DLRExtensions);
                        }
                        else if (pDDB->PRM.FirstCommitExecuted) /* request only allowed before first commit */
                        {
                            Response = EDD_STS_ERR_PRM_INDEX; /* special case we use Index error here */
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,
                                        "[H:--] EDDP_PRMRequestPrmWrite(): ERROR -> PDSetDefaultPortStates, Request invalid. First COMMIT already done.)");
                        }
                        else if( 0 != pPrmWrite->edd_port_id )  // check "edd_port_id" - only interface allowed
                        {
                            Response = EDD_STS_ERR_PRM_PORTID;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,
                                        "[H:--] EDDP_PRMRequestPrmWrite(): ERROR -> PrmRecord PDSetDefaultPortStates: only interface port (0) allowed (edd_port_id == 0x%x)",
                                        pPrmWrite->edd_port_id);
                        }
                        else if (   (pPrmWrite->record_data_length < pDDB->PRM.pRecordsetB->PDSetDefaultPortStates_SizeMin)   // check "record_data_length"
                                 || (pPrmWrite->record_data_length > pDDB->PRM.pRecordsetB->PDSetDefaultPortStates_SizeMax))
                        {
                            Response = EDD_STS_ERR_PRM_DATA;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, 
                                        "[H:--] EDDP_PRMRequestPrmWrite(): ERROR -> PrmRecord PDSetDefaultPortStates: record_data_length(%d) invalid", 
                                pPrmWrite->record_data_length);
                        }
                        else
                        {
                            // store record length from RQB in order to check it later against record length from record
                            pDDB->PRM.pRecordsetB->PDSetDefaultPortStates_Length = pPrmWrite->record_data_length;

                            Response = EDDP_PRMPDSetDefaultPortStatesRecord_Write(pDDB, pPrmWrite->record_data);
                        }
                    }
                    break;

                    /* ---------------------------------------- */
                    /* User specific records                    */
                    /* These records are not used by EDDP       */
                    /* ---------------------------------------- */
                    case EDDP_PRM_PDCONTROL_PLL_RECORD_INDEX:
                    case EDDP_PRM_PDTRACE_UNIT_CONTROL_RECORD_INDEX:
                    case EDDP_PRM_PDTRACE_UNIT_DATA_RECORD_INDEX:
                    {
                        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmWrite(): PrmRecord(0x%X) is not used by EDDP", 
                            pHDB->Handle, pPrmWrite->record_index);
                    }
                    break;

                    /* ---------------------------------------- */
                    /* Unknown Records                          */
                    /* ---------------------------------------- */
                    default:
                    {
                        Response = EDD_STS_ERR_PRM_INDEX;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMRequestPrmWrite(): ERROR -> unknown record_index(0x%X)!", 
                            pPrmWrite->record_index);
                    }
                    break;
                }
            }
            else
            {
                Response = EDD_STS_ERR_SEQUENCE;
                EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
                EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmWrite(): ERROR -> Wrong sequence! PrmState=0x%X",
                    pHDB->Handle, pDDB->PRM.PrmState);
            }
        }
    }


    switch(Response)
    {
    case EDD_STS_OK:
        pPrm->PrmState = EDDP_PRM_STATE_WRITE;
        break;
    case EDD_STS_ERR_PRM_BLOCK:
    case EDD_STS_ERR_PRM_VERSION_CONFLICT:
    case EDD_STS_ERR_PRM_CONSISTENCY:
    case EDD_STS_ERR_PARAM:
    case EDD_STS_ERR_PRM_DATA:
    case EDD_STS_ERR_PRM_PORTID:
    case EDD_STS_ERR_PRM_INDEX:
    case EDD_STS_ERR_SEQUENCE:
    default:
        if ( !(LSA_HOST_PTR_ARE_EQUAL(pRQB->pParam, LSA_NULL)) )
        {
            EDD_RQB_PRM_WRITE_SET(pPrmWrite,pDetailErr->ErrOffset); /* Offset into pPrmWrite->record_data */
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMRequestWrite(): ERROR -> ErrOffset=%d",
                pDDB->PrmDetailErr.ErrOffset);
        }
        break;
        break;
    }

    if (Response == EDD_STS_OK)
    {
        pPrm->PrmState = EDDP_PRM_STATE_WRITE;
    }

    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMRequestPrmWrite(): Response=0x%X", Response);
    EDDP_PRM_INIT_DETAIL_ERR(pDetailErr); /* sanity */
    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMPDNCDataCheckRecord_Write           +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB           +*/
/*+                             EDD_UPPER_MEM_U8_PTR_TYPE    pRecord        +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pRecord    : Pointer to PRM record                                     +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PRM_BLOCK                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function parses the PDNCDataCheck record (pRecord)   +*/
/*+               and stores the extracted data into DDB.                   +*/
/*+               Moreover, the original record is stored in record set B.  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDNCDataCheckRecord_Write(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
   	EDD_UPPER_MEM_U8_PTR_TYPE       pRecord)
{
    EDD_RSP                         Response = EDD_STS_OK;
    LSA_UINT16                      val_16;
    LSA_UINT32                      val_32;
    LSA_UINT32                      record_length = 0;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_Begin;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_Tmp;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_RecordLength;
    EDDP_PRM_DETAIL_ERR_PTR_TYPE    pDetailErr = &pDDB->PrmDetailErr;
    
    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_PRMPDNCDataCheckRecord_Write()");

    //remember the pointer to the begin of this record
    pRecord_Begin           = pRecord;
    pRecord_RecordLength    = pRecord;
    
    // check BlockType (record index) for PDNCDataCheck
    pRecord_Tmp = pRecord;
    EDDP_GET_U16_INCR(pRecord, val_16);
    if (val_16 != EDDP_PRM_PDNC_DATA_CHECK_BLOCK_TYPE)
    {
        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDNCDataCheckRecord_Write(): ERROR -> (%d) != EDDP_PRM_PDNC_DATA_CHECK_BLOCK_TYPE", val_16);
        Response =  EDD_STS_ERR_PRM_BLOCK; 
    }
    else
    {
        // check BlockLength
        pRecord_RecordLength = pRecord;
        pRecord_Tmp = pRecord;
        EDDP_GET_U16_INCR(pRecord, val_16);
        record_length = (val_16 + 4);
        if (record_length > pDDB->PRM.pRecordsetB->PDNCDataCheck_Length)
        {

            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDNCDataCheckRecord_Write(): ERROR -> Record length does not match with length in RQB!");
            Response = EDD_STS_ERR_PRM_BLOCK; 
        }
        else
        {
            // check BlockVersion
            // BlockVersionHigh
            pRecord_Tmp = pRecord;
            if (*pRecord++ != 0x01)
            { 
                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDNCDataCheckRecord_Write(): ERROR -> Block version does not match!");
                Response = EDD_STS_ERR_PRM_VERSION_CONFLICT; 
            }
            else
            {
                // BlockVersionLow
                pRecord_Tmp = pRecord;
                if (*pRecord++ != 0x00)
                { 
                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDNCDataCheckRecord_Write(): ERROR -> Block version does not match!");
                    Response = EDD_STS_ERR_PRM_VERSION_CONFLICT; 
                }
                else
                {
                    // check padding bytes
                    pRecord_Tmp = pRecord;
                    if (*pRecord++ != 0x00)
                    { 
                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDNCDataCheckRecord_Write(): ERROR -> Padding is not '0'!");
                        Response = EDD_STS_ERR_PRM_BLOCK; 
                    }
                    else
                    {
                        pRecord_Tmp = pRecord;
                        if (*pRecord++ != 0x00)
                        { 
                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDNCDataCheckRecord_Write(): ERROR -> Padding is not '0'!");
                            Response = EDD_STS_ERR_PRM_BLOCK; 
                        }
                        else
                        {

                        }
                    }
                }
            }
        }
    }
    
    if (Response == EDD_STS_OK)
    {
        // store MaintenanceRequiredDropBudget   
        EDDP_GET_U32_INCR(pRecord, val_32);
        if ((val_32 & EDDP_PRM_PDNC_DATA_CHECK_BUDGET_CHECK_MSK) == EDDP_PRM_PDNC_DATA_CHECK_BUDGET_CHECK_ON)
        {
            pDDB->PRM.FrameDrop.Budgets.MaintenanceRequiredDropBudget.Enabled   = LSA_TRUE;    
            pDDB->PRM.FrameDrop.Budgets.MaintenanceRequiredDropBudget.Value     = val_32 & EDDP_PRM_PDNC_DATA_CHECK_BUDGET_VALUE_MSK;
        }
        else
        {
            pDDB->PRM.FrameDrop.Budgets.MaintenanceRequiredDropBudget.Enabled   = LSA_FALSE;
        } 

        // store MaintenanceDemandedDropBudget
        EDDP_GET_U32_INCR(pRecord, val_32);
        if ((val_32 & EDDP_PRM_PDNC_DATA_CHECK_BUDGET_CHECK_MSK) == EDDP_PRM_PDNC_DATA_CHECK_BUDGET_CHECK_ON)
        {
            pDDB->PRM.FrameDrop.Budgets.MaintenanceDemandedDropBudget.Enabled   = LSA_TRUE;    
            pDDB->PRM.FrameDrop.Budgets.MaintenanceDemandedDropBudget.Value     = val_32 & EDDP_PRM_PDNC_DATA_CHECK_BUDGET_VALUE_MSK;
        }
        else
        {
            pDDB->PRM.FrameDrop.Budgets.MaintenanceDemandedDropBudget.Enabled   = LSA_FALSE;
        }        
    
        // store ErrorDropBudget
        EDDP_GET_U32_INCR(pRecord, val_32);
        if ((val_32 & EDDP_PRM_PDNC_DATA_CHECK_BUDGET_CHECK_MSK) == EDDP_PRM_PDNC_DATA_CHECK_BUDGET_CHECK_ON)
        {
            pDDB->PRM.FrameDrop.Budgets.ErrorDropBudget.Enabled = LSA_TRUE;    
            pDDB->PRM.FrameDrop.Budgets.ErrorDropBudget.Value   = val_32 & EDDP_PRM_PDNC_DATA_CHECK_BUDGET_VALUE_MSK;
        }
        else
        {
            pDDB->PRM.FrameDrop.Budgets.ErrorDropBudget.Enabled = LSA_FALSE;
        }
    }

    // check if number of read bytes is same as record length
    if ( ((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_Begin) != record_length )
    {
        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_RecordLength - pRecord_Begin);
        Response = EDD_STS_ERR_PRM_BLOCK;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDNCDataCheckRecord_Write(): ERROR -> ERROR -> number of read bytes(%d) != record_length(%d)", 
            ((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_Begin), record_length);
    }
    
    if (Response == EDD_STS_OK)
    {
        // store record in record set B
        EDDP_MEMCOPY_COMMON(pDDB->PRM.pRecordsetB->pPDNCDataCheck, pRecord_Begin, record_length);
        pDDB->PRM.pRecordsetB->PDNCDataCheck_Length     = record_length;
        pDDB->PRM.pRecordsetB->PDNCDataCheck_IsPresent  = LSA_TRUE;
    }
            
    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMPDNCDataCheckRecord_Write(): Response=0x%X", Response);

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMPDSyncDataRecord_Write              +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB           +*/
/*+                             EDD_UPPER_MEM_U8_PTR_TYPE    pRecord        +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pRecord    : Pointer to PRM record                                     +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PRM_BLOCK                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function parses the PDSyncData record (pRecord)      +*/
/*+               and stores the extracted data into DDB.                   +*/
/*+               Moreover, the original record is stored in record set B.  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDSyncDataRecord_Write(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord)
{
    EDD_RSP                         Response;
    LSA_UINT16                      val_16;
    LSA_UINT32                      val_32;
    LSA_UINT32                      i;
    LSA_UINT8                       BlockVersionLow, BlockVersionHigh;
    LSA_UINT32                      record_length;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_Begin;
    LSA_UINT32                      Padding;

    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_Tmp;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_RecordLength;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_ReservedIntervalBegin;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_ReservedIntervalEnd;
    EDDP_PRM_DETAIL_ERR_PTR_TYPE    pDetailErr;
    
    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_PRMPDSyncDataRecord_Write()");

    Response                = EDD_STS_OK;
    pDetailErr              = &pDDB->PrmDetailErr;
    pRecord_Begin           = pRecord;
    pRecord_RecordLength    = pRecord;
    record_length           = 0;

    // check BlockType for PDSyncData
    pRecord_Tmp = pRecord;
    EDDP_GET_U16_INCR(pRecord, val_16);
    if (val_16 != EDDP_PRM_PDSYNCDATA_BLOCKTYPE)
    {
        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
        Response = EDD_STS_ERR_PRM_BLOCK;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDSyncDataRecord_Write(): ERROR -> PDSyncData.BlockType(0x%X) != EDDP_PRM_PDSYNCDATA_BLOCKTYPE(0x%X)", 
            val_16, EDDP_PRM_PDSYNCDATA_BLOCKTYPE);
    }
    else
    {
        // check BlockLength
        pRecord_RecordLength = pRecord;
        pRecord_Tmp = pRecord;
        EDDP_GET_U16_INCR(pRecord, val_16);
        record_length = (val_16 + 4);
        if (record_length > pDDB->PRM.pRecordsetB->PDSyncData_Length)
        {
            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
            Response = EDD_STS_ERR_PRM_BLOCK;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDSyncDataRecord_Write(): ERROR -> record_length(%d) > PDSyncData_Length(%d)!", 
                record_length, pDDB->PRM.pRecordsetB->PDSyncData_Length);
        }
        else
        {
            // check BlockVersionHigh
            pRecord_Tmp = pRecord;
            BlockVersionHigh = *pRecord++;
            if (BlockVersionHigh != EDDP_PRM_PDSYNCDATA_BLOCKVERSIONHIGH)
            {
                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDSyncDataRecord_Write(): ERROR -> PDSyncData.BlockVersionHigh(0x%X) does not match!", 
                    BlockVersionHigh);
            }
            else
            {
                // check BlockVersionLow
                pRecord_Tmp = pRecord;
                BlockVersionLow = *pRecord++;
                if (BlockVersionLow != EDDP_PRM_PDSYNCDATA_BLOCKVERSIONLOW)
                {
                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                    Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDSyncDataRecord_Write(): ERROR -> PDSyncData.BlockVersionLow(0x%X) does not match!", 
                        BlockVersionLow);
                }
                else
                {
                    // check padding bytes
                    pRecord_Tmp = pRecord;
                    if (*pRecord++ != 0)
                    {
                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                        Response = EDD_STS_ERR_PRM_BLOCK;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDSyncDataRecord_Write(): ERROR -> PDSyncData.Padding is not '0'!");
                    }
                    else
                    {
                        // check padding bytes
                        pRecord_Tmp = pRecord;
                        if (*pRecord++ != 0)
                        {
                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                            Response = EDD_STS_ERR_PRM_BLOCK;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDSyncDataRecord_Write(): ERROR -> PDSyncData.Padding is not '0'!");
                        }
                    }
                }
            }
        }
    }

    /* ------------------------------------------------------------------------ */
    /* store data of PDSyncData                                                 */
    /* ------------------------------------------------------------------------ */
    if (Response == EDD_STS_OK)
    {
        // store PTCPSubdomainID
        EDDP_GET_U32_INCR(pRecord, val_32);
        pDDB->PRM.PDSyncData.PTCPSubdomainID.UUID_part_1            =  val_32;
        EDDP_GET_U16_INCR(pRecord, val_16);
        pDDB->PRM.PDSyncData.PTCPSubdomainID.UUID_part_2            =  val_16;
        EDDP_GET_U16_INCR(pRecord, val_16);
        pDDB->PRM.PDSyncData.PTCPSubdomainID.UUID_part_3            =  val_16;
        EDDP_GET_U16_INCR(pRecord, val_16);
        pDDB->PRM.PDSyncData.PTCPSubdomainID.UUID_part_4            =  val_16;    
        
        pDDB->PRM.PDSyncData.PTCPSubdomainID.UUID_part_5_array[0]   = *pRecord++;
        pDDB->PRM.PDSyncData.PTCPSubdomainID.UUID_part_5_array[1]   = *pRecord++;
        pDDB->PRM.PDSyncData.PTCPSubdomainID.UUID_part_5_array[2]   = *pRecord++;
        pDDB->PRM.PDSyncData.PTCPSubdomainID.UUID_part_5_array[3]   = *pRecord++;
        pDDB->PRM.PDSyncData.PTCPSubdomainID.UUID_part_5_array[4]   = *pRecord++;
        pDDB->PRM.PDSyncData.PTCPSubdomainID.UUID_part_5_array[5]   = *pRecord++;
        
        // store ReservedIntervalBegin
        pRecord_ReservedIntervalBegin = pRecord;
        EDDP_GET_U32_INCR(pRecord, val_32);
        pDDB->PRM.PDSyncData.ReservedIntervalBegin      = val_32;
        // store ReservedIntervalEnd
        pRecord_ReservedIntervalEnd = pRecord;
        EDDP_GET_U32_INCR(pRecord, val_32);
        pDDB->PRM.PDSyncData.ReservedIntervalEnd        = val_32;
        // store PLLWindow
        EDDP_GET_U32_INCR(pRecord, val_32);
        pDDB->PRM.PDSyncData.PLLWindow                  = val_32;
        // store SyncSendFactor
        EDDP_GET_U32_INCR(pRecord, val_32);
        pDDB->PRM.PDSyncData.SyncSendFactor             = val_32;
        // store SendClockFactor
        pRecord_Tmp = pRecord;
        EDDP_GET_U16_INCR(pRecord, val_16);
        pDetailErr->OffsetSendClockFactor = pRecord_Tmp - pRecord_Begin;
        pDDB->PRM.PDSyncData.SendClockFactor            = val_16;
        // store PTCPTimeoutFactor
        EDDP_GET_U16_INCR(pRecord, val_16);
        pDDB->PRM.PDSyncData.PTCPTimeoutFactor          = val_16;
        // store PTCPTakeoverTimeoutFactor
        EDDP_GET_U16_INCR(pRecord, val_16);
        pDDB->PRM.PDSyncData.PTCPTakeoverTimeoutFactor  = val_16;
        // store PTCPMasterStartupTime
        EDDP_GET_U16_INCR(pRecord, val_16);
        pDDB->PRM.PDSyncData.PTCPMasterStartupTime      = val_16;
        // store SyncProperties
        EDDP_GET_U16_INCR(pRecord, val_16);
        pDDB->PRM.PDSyncData.SyncProperties             = val_16;
        // store PTCP_MasterPriority1
        pDDB->PRM.PDSyncData.PTCP_MasterPriority1       = *pRecord++;
        // store PTCP_MasterPriority2
        pDDB->PRM.PDSyncData.PTCP_MasterPriority2       = *pRecord++;

        // store PTCPLengthSubdomainName
        pRecord_Tmp = pRecord;
        pDDB->PRM.PDSyncData.PTCPLengthSubdomainName    = *pRecord++;
        // check PTCPLengthSubdomainName
        if (pDDB->PRM.PDSyncData.PTCPLengthSubdomainName > EDDP_PRM_MAX_PTCPSUBDOMAINNAMELENGTH)
        {
            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
            Response = EDD_STS_ERR_PRM_BLOCK;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDSyncDataRecord_Write(): ERROR -> PDSyncData.PTCPLengthSubdomainName(%d) > PTCPLengthSubdomainNameMax(%d)!", 
                pDDB->PRM.PDSyncData.PTCPLengthSubdomainName, EDDP_PRM_MAX_PTCPSUBDOMAINNAMELENGTH);
        }

        // store PTCPSubdomainName
        if (Response == EDD_STS_OK)
        {
            for ( i=0; i < pDDB->PRM.PDSyncData.PTCPLengthSubdomainName; i++ )
            {
                pDDB->PRM.PDSyncData.PTCPSubdomainName[i] = *pRecord++;
            }
        }

        // check ReservedIntervalBegin / ReservedIntervalEnd
        //      Orange phase    : ReservedIntervalBegin < ReservedIntervalEnd       (Reserved-Interval is not supported)
        //      Default         : ReservedIntervalBegin = ReservedIntervalEnd = 0   (only this combination is valid)
        //      Not used        : ReservedIntervalBegin > ReservedIntervalEnd       (parametrization error)
        if (   (Response == EDD_STS_OK)
            && ((pDDB->PRM.PDSyncData.ReservedIntervalBegin != 0) || (pDDB->PRM.PDSyncData.ReservedIntervalEnd != 0))
           )
        {
            if(pDDB->PRM.PDSyncData.ReservedIntervalBegin != 0)
            {
                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_ReservedIntervalBegin - pRecord_Begin);
            }
            else 
            {
                if (pDDB->PRM.PDSyncData.ReservedIntervalEnd != 0)
                {
                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_ReservedIntervalEnd - pRecord_Begin);
                }
            }

            Response = EDD_STS_ERR_PRM_BLOCK;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDSyncDataRecord_Write(): ERROR -> PDSyncData.ReservedIntervalBegin(%d) or PDSyncData.ReservedIntervalEnd(%d) is not '0'!", 
                pDDB->PRM.PDSyncData.ReservedIntervalBegin, pDDB->PRM.PDSyncData.ReservedIntervalEnd);
        }
    }

    // skip padding (always 4 byte aligned)
    if (Response == EDD_STS_OK)
    {
        Padding = (pRecord - pRecord_Begin) % 4;

        if (Padding > 0)
        {
            Padding = 4 - Padding;

            for ( i=0; i < Padding; i++ )
            {
                pRecord_Tmp = pRecord;
                if (*pRecord++ != 0)
			    { 
                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                    Response = EDD_STS_ERR_PRM_BLOCK;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDSyncDataRecord_Write(): ERROR -> PDSyncData.Padding is not '0'!");
                }
            }
        }
    }

    if (Response == EDD_STS_OK)
    {
        // check if number of read bytes is same as record length
        if ( ((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_Begin) != record_length)
        {
            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_RecordLength - pRecord_Begin);
            Response = EDD_STS_ERR_PRM_BLOCK;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDSyncDataRecord_Write(): ERROR -> number of read bytes(%d) != record_length(%d)", 
                ((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_Begin), record_length);

        }
        else
        {
            // store record in record set B
            EDDP_MEMCOPY_COMMON(pDDB->PRM.pRecordsetB->pPDSyncData, pRecord_Begin, record_length);
            pDDB->PRM.pRecordsetB->PDSyncData_Length    = record_length;
            pDDB->PRM.pRecordsetB->PDSyncData_IsPresent = LSA_TRUE;
        }
    }

    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMPDSyncDataRecord_Write(): Response=0x%X", Response);

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMPDPortDataAdjustRecord_Write        +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB           +*/
/*+                             EDD_UPPER_MEM_U8_PTR_TYPE    pRecord        +*/
/*+                             LSA_UINT16                   PortID         +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pRecord    : Pointer to PRM record                                     +*/
/*+  PortID     : Port ID of which this record belongs to                   +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PRM_BLOCK                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function parses the PDPortDataAdjust record (pRecord)+*/
/*+               and stores the extracted data into DDB.                   +*/
/*+               Moreover, the original record is stored in record set B.  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDPortDataAdjustRecord_Write(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord,
    LSA_UINT16                      PortID)
{
    EDD_RSP                         Response = EDD_STS_OK;
    LSA_UINT16                      val_16;
    LSA_UINT32                      val_32;
    LSA_UINT32                      record_length = 0;
    LSA_UINT32                      Speed;
    LSA_UINT32                      Duplexity;
    LSA_UINT32                      HwPortID;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_Begin;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_BlockType;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_Tmp;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_RecordLength;
    EDDP_PRM_DETAIL_ERR_PTR_TYPE    pDetailErr = &pDDB->PrmDetailErr;
    LSA_BOOL                        EndOfRecordFlage = LSA_FALSE;
    LSA_BOOL                        AdjustMauTypeIsPresent = LSA_FALSE;
	LSA_BOOL						AdjustLinkStateIsPresent = LSA_FALSE;

    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_PRMPDPortDataAdjustRecord_Write(PortID=%d)", PortID);

    // ***** PDPortDataAdjust = BlockHeader, Padding, Padding, SlotNumber, SubslotNumber, { [AdjustDomainBoundary], [AdjustMulticastBoundary], [AdjustMAUType ^ AdjustLinkState], [AdjustPeerToPeerBoundary], [AdjustDCPBoundary], [AdjustPreambleLength] }
    pRecord_Begin           = pRecord;  // remember the pointer to the begin of this record
    pRecord_RecordLength    = pRecord;

    // ***** BlockHeader = BlockType, BlockLength, BlockVersionHigh, BlockVersionLow
    // check BlockType (record index) for PDPortDataAdjust
    pRecord_Tmp = pRecord;
    EDDP_GET_U16_INCR(pRecord, val_16);
    // ***** check BlockType
    if (val_16 != EDDP_PRM_PDPORT_DATA_ADJUST_BLOCK_TYPE)
    { 
        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
        Response = EDD_STS_ERR_PRM_BLOCK;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> BlockType(0x%X) != EDDP_PRM_PDPORT_DATA_ADJUST_BLOCK_TYPE(0x%X)!", 
            val_16, EDDP_PRM_PDPORT_DATA_ADJUST_BLOCK_TYPE);
    }
    else
    {
        // ***** check BlockLength
        pRecord_Tmp = pRecord;
        pRecord_RecordLength = pRecord;
        EDDP_GET_U16_INCR(pRecord, val_16);
        #ifdef EDDP_PRM_TRACE_BYTE_ARRAY_ON
        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): >>> TraceByteArray PDPortDataAdjust");
        EDDP_GlbTraceByteArray (pDDB, pRecord, val_16);
        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): <<< TraceByteArray PDPortDataAdjust");
        #endif
        record_length = (val_16 + 4);
        if (record_length > pDDB->PRM.pRecordsetB->PDPortDataAdjust[PortID].Record_Size)
        { 
            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
            Response = EDD_STS_ERR_PRM_BLOCK;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Record length does not match with length in RQB!");
        }
        else
        {
            // ***** check BlockVersion
            // BlockVersionHigh
            pRecord_Tmp = pRecord;
            if (*pRecord++ != 0x01)
            { 
                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Block version does not match (PDPortDataAdjust)!");
            }
            else
            {
                // BlockVersionLow
                pRecord_Tmp = pRecord;
                if (*pRecord++ != 0x00)
                { 
                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                    Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Block version does not match (PDPortDataAdjust)!");
                }
                else
                {
                    // ***** check padding bytes
                    pRecord_Tmp = pRecord;
                    if (*pRecord++ != 0x00)
                    { 
                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                        Response = EDD_STS_ERR_PRM_BLOCK;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Padding is not '0'!");
                    }
                    else
                    {
                        // ***** check padding bytes
                        pRecord_Tmp = pRecord;
                        if (*pRecord++ != 0x00)
                        { 
                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                            Response = EDD_STS_ERR_PRM_BLOCK;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Padding is not '0'!");
                        }
                        else
                        {
                            // ***** check SlotNumber
                            pRecord_Tmp = pRecord;
                            EDDP_GET_U16_INCR(pRecord, val_16);
                            if (val_16 != pDDB->PRM.PortDataAdjust.Parameters[PortID].SlotNumber)
                            { 
                                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                Response = EDD_STS_ERR_PRM_BLOCK;
                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Slotnumber does not match!");
                            }
                            else
                            {
                                // ***** check SubslotNumber
                                pRecord_Tmp = pRecord;
                                EDDP_GET_U16_INCR(pRecord, val_16);
                                if (val_16 != pDDB->PRM.PortDataAdjust.Parameters[PortID].SubslotNumber)
                                { 
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Subslotnumber does not match!");
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (Response == EDD_STS_OK)
    {
        while ( (((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_Begin) < record_length) && (Response == EDD_STS_OK) ) //break on ERR
        {
            pRecord_BlockType = pRecord;
            EDDP_GET_U16_INCR(pRecord, val_16);
    
            switch (val_16)
            {
                case EDDP_PRM_PDPORT_DATA_ADJUST_D_BOUNDARY_BLOCK_TYPE:
                    // ***** AdjustDomainBoundary_Vxx01 = BlockHeader, Padding, Padding, DomainBoundaryIngress, DomainBoundaryEgress, AdjustProperties, [Padding*]
                    // -------------------------------------------------
                    // check BlockType for optional AdjustDomainBoundary 
                    // -------------------------------------------------
                    // ***** BlockHeader = BlockType, BlockLength, BlockVersionHigh, BlockVersionLow
                    {
                        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): parse AdjustDomainBoundary");
                        // ***** skip BlockLength
                        pRecord += 2;    
                        // ***** check BlockVersion for AdjustDomainBoundary
                        // BlockVersionHigh
                        pRecord_Tmp = pRecord;
                        if (*pRecord++ != 0x01)
                        { 
                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                            Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[|H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Block version does not match (AdjustDomainBoundary)!");
                            break; 
                        }
                        else
                        {
                            // BlockVersionLow 
                            pRecord_Tmp = pRecord;
                            if (*pRecord++ != 0x01)
                            { 
                                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Block version does not match (AdjustDomainBoundary)!");
                                break; 
                            }
                            else
                            {
                                // ***** check padding bytes
                                pRecord_Tmp = pRecord;
                                if (*pRecord++ != 0x00)
                                { 
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Padding is not '0'!");
                                    break;
                                }
                                else
                                {
                                    // ***** check padding bytes
                                    pRecord_Tmp = pRecord;
                                    if (*pRecord++ != 0x00)
                                    { 
                                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                        Response = EDD_STS_ERR_PRM_BLOCK;
                                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Padding is not '0'!");
                                        break;
                                    }
                                    else
                                    {
                                        // ***** store DomainBoundaryIngress
                                        pRecord_Tmp = pRecord;
                                        EDDP_GET_U32_INCR(pRecord, val_32);
                                        pDDB->PRM.PortDataAdjust.Parameters[PortID].DomainBoundaryIngress = val_32;
                                        // ***** check if reserved bits are set to "do not block"
                                        if ((val_32 & EDDP_PRM_DOMAIN_BOUNDARY_MASK) != EDDP_PRM_DOMAIN_BOUNDARY_NOT_BLOCKED)
                                        {
                                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                            Response = EDD_STS_ERR_PRM_BLOCK;
                                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Reserved DomainBoundaryIngress bits are not %d!", 
                                                EDDP_PRM_DOMAIN_BOUNDARY_NOT_BLOCKED);
                                            break;
                                        }
                                        else
                                        {
                                            // ***** store DomainBoundaryEgress
                                            pRecord_Tmp = pRecord;
                                            EDDP_GET_U32_INCR(pRecord, val_32);        
                                            pDDB->PRM.PortDataAdjust.Parameters[PortID].DomainBoundaryEgress = val_32;
                                            // check if reserved bits are set to "do not block"
                                            if ((val_32 & EDDP_PRM_DOMAIN_BOUNDARY_MASK) != EDDP_PRM_DOMAIN_BOUNDARY_NOT_BLOCKED)
                                            {
                                                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                                Response = EDD_STS_ERR_PRM_BLOCK;
                                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                                EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Reserved DomainBoundaryEgress bits are not %d!", 
                                                    EDDP_PRM_DOMAIN_BOUNDARY_NOT_BLOCKED);
                                                break;
                                            }
                                            else
                                            {
                                                pDDB->PRM.PortDataAdjust.Parameters[PortID].DomainBoundaryIngressEgress_IsPresent = LSA_TRUE;    
                                                // ***** check AdjustProperties
                                                pRecord_Tmp = pRecord;
                                                EDDP_GET_U16_INCR(pRecord, val_16);        
                                                if (val_16 != EDDP_PRM_PDPORT_DATA_ADJUST_D_ADJUST_PROPERTIES)
                                                { 
                                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                                    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> (%d) != EDDP_PRM_PDPORT_DATA_ADJUST_D_ADJUST_PROPERTIES!", 
                                                        val_16);
                                                    break;
                                                }
                                                else
                                                {
                                                    // ***** check padding bytes --> padding to 32bit
                                                    pRecord_Tmp = pRecord;
                                                    if (*pRecord++ != 0x00)
                                                    { 
                                                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                                        Response = EDD_STS_ERR_PRM_BLOCK;
                                                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                                        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Padding is not '0'!");
                                                        break;
                                                    }
                                                    else
                                                    {
                                                        pRecord_Tmp = pRecord;
                                                        if (*pRecord++ != 0x00)
                                                        { 
                                                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                                            Response = EDD_STS_ERR_PRM_BLOCK;
                                                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                                            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Padding is not '0'!");
                                                            break;
                                                        }
                                                        else
                                                        {
                                                            if ( ((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_Begin) == record_length )
                                                            {
                                                                /* end of record reached*/
                                                                EndOfRecordFlage = LSA_TRUE;
                                                                break;
                                                            } 
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }    
                        break;
                    }
    
                case EDDP_PRM_PDPORT_DATA_ADJUST_MC_BOUNDARY_BLOCK_TYPE:
                    // ***** AdjustMulticastBoundary = BlockHeader, Padding, Padding, MulticastBoundary, AdjustProperties, Padding, Padding
                    // -------------------------------------------
                    // check BlockType for AdjustMulticastBoundary 
                    // ------------------------------------------- 
                    // ***** BlockHeader = BlockType, BlockLength, BlockVersionHigh, BlockVersionLow
                    {
                        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): parse AdjustMulticastBoundary");
                        // ***** skip BlockLength
                        pRecord += 2;    
                        // ***** check BlockVersion for AdjustMulticastBoundary
                        //BlockVersionHigh
                        pRecord_Tmp = pRecord;
                        if (*pRecord++ != 0x01)
                        { 
                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                            Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Block version does not match (AdjustMulticastBoundary)!");
                            break;
                        }
                        else
                        {
                            //BlockVersionLow
                            pRecord_Tmp = pRecord;
                            if (*pRecord++ != 0x00)
                            { 
                                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Block version does not match (AdjustMulticastBoundary)!");
                                break;
                            }
                            else
                            {
                                // ***** check padding bytes
                                pRecord_Tmp = pRecord;
                                if (*pRecord++ != 0x00)
                                { 
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Padding is not '0'!");
                                    break;
                                }
                                else
                                {
                                    // ***** check padding bytes
                                    pRecord_Tmp = pRecord;
                                    if (*pRecord++ != 0x00)
                                    { 
                                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                        Response = EDD_STS_ERR_PRM_BLOCK;
                                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Padding is not '0'!");
                                        break;
                                    }
                                    else
                                    {
                                        // ***** store MulticastBoundary
                                        EDDP_GET_U32_INCR(pRecord, val_32);        
                                        pDDB->PRM.PortDataAdjust.Parameters[PortID].MulticastBoundary           = val_32;
                                        pDDB->PRM.PortDataAdjust.Parameters[PortID].MulticastBoundary_IsPresent = LSA_TRUE;
                                        // ***** check AdjustProperties
                                        pRecord_Tmp = pRecord;
                                        EDDP_GET_U16_INCR(pRecord, val_16);        
                                        if (val_16 != EDDP_PRM_PDPORT_DATA_MC_ADJUST_PROPERTIES)
                                        { 
                                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                            Response = EDD_STS_ERR_PRM_BLOCK;
                                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> (%d) != EDDP_PRM_PDPORT_DATA_MC_ADJUST_PROPERTIES!", 
                                                val_16);
                                            break;
                                        }
                                        else
                                        {
                                            // ***** check 2x padding bytes --> padding to 32bit
                                            pRecord_Tmp = pRecord;
                                            if (*pRecord++ != 0x00)
                                            { 
                                                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                                Response = EDD_STS_ERR_PRM_BLOCK;
                                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Padding is not '0'!");
                                                break;
                                            }
                                            else
                                            {
                                                pRecord_Tmp = pRecord;
                                                if (*pRecord++ != 0x00)
                                                { 
                                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Padding is not '0'!");
                                                    break;
                                                }
                                                else
                                                {
                                                    if ( ((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_Begin) == record_length )
                                                    { 
                                                        /*end of record reached*/ 
                                                        EndOfRecordFlage = LSA_TRUE;
                                                        break;
                                                    } 
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    }
    
                case EDDP_PRM_PDPORT_DATA_ADJUST_MAUTYPE_BLOCK_TYPE:
                    // ***** AdjustMAUType = BlockHeader, Padding, Padding, MAUType, AdjustProperties
                    // -------------------------------------------
                    // check BlockType for AdjustMAUType 
                    // -------------------------------------------    
                    // ***** BlockHeader = BlockType, BlockLength, BlockVersionHigh, BlockVersionLow
                    {
						if(AdjustLinkStateIsPresent)
						{
							Response = EDD_STS_ERR_PRM_BLOCK;						
							EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
							EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> EDDP_PRM_PDPORT_DATA_ADJUST_MAUTYPE_BLOCK_TYPE called after EDDP_PRM_PDPORT_DATA_ADJUST_LINKSTATE_BLOCK_TYPE!");
							break;
						}
						else
						{
							EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): parse AdjustMAUType");
							// skip BlockLength
							pRecord += 2;    
							// ***** check BlockVersion for AdjustMAUType
							// BlockVersionHigh
							pRecord_Tmp = pRecord;
							if (*pRecord++ != 0x01)
							{ 
								EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
								Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
								EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
								EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Block version does not match (AdjustMAUType)!");
								break;
							}
							else
							{
								// BlockVersionLow
								pRecord_Tmp = pRecord;
								if (*pRecord++ != 0x00)
								{ 
									EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
									Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
									EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
									EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Block version does not match (AdjustMAUType)!");
									break;
								}
								else
								{
									// ***** check 2x padding bytes
									pRecord_Tmp = pRecord;
									if (*pRecord++ != 0x00)
									{ 
										EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
										Response = EDD_STS_ERR_PRM_BLOCK;
										EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
										EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Padding is not '0'!");
										break;
									}
									else
									{
										pRecord_Tmp = pRecord;
										if (*pRecord++ != 0x00)
										{ 
											EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
											Response = EDD_STS_ERR_PRM_BLOCK;
											EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
											EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Padding is not '0'!");
											break;
										}
										else
										{
											EDD_RSP LocalResult;
											pRecord_Tmp = pRecord;
    
											EDDP_GET_U16_INCR(pRecord, val_16);
    
											// ***** check MAUType
											HwPortID    = EDDP_SWI_get_HwPortID (pDDB, PortID);  
    
											LocalResult = pDDB->SWI.PHY_FunctionPointers[HwPortID].EDDP_PHY_CheckMAUType(pDDB->hDDB, pDDB->hSysDev, HwPortID, val_16, &Speed, &Duplexity);
    
											if (   (EDD_STS_OK != LocalResult)
												&& (EDD_STS_ERR_PARAM != LocalResult))
											{ 
												EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
												Response = EDD_STS_ERR_PRM_BLOCK;
												EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
												EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> MAU-Type(0x%x) rejected from PHY-API (EDDP_PHY_CheckMAUType)!", 
													val_16);
												break;
											}
											else
											{
												// ***** PNIP (MAC) supports only 100Mbit and FullDuplex
												if (   ((Speed != EDD_LINK_SPEED_100)     && (Speed != EDD_LINK_UNKNOWN))
													|| ((Duplexity != EDD_LINK_MODE_FULL) && (Duplexity != EDD_LINK_UNKNOWN)) )
												{
													EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
													Response = EDD_STS_ERR_PRM_BLOCK;
													EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
													EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> MAU-Type(0x%x) does not support 100Mbit and FullDuplex!", 
														val_16);
													break;                  
												}
												else
												{
													// store position of value
													pDDB->PrmDetailErr.OffsetMAUType = pRecord_Tmp - pRecord_Begin;
    
													// ***** store MAUType
													pDDB->PRM.PortDataAdjust.Parameters[PortID].MAUType           = val_16;
													pDDB->PRM.PortDataAdjust.Parameters[PortID].MAUType_IsPresent = LSA_TRUE;
													// ***** check AdjustProperties
													pRecord_Tmp = pRecord;
													EDDP_GET_U16_INCR(pRecord, val_16);        
													if (val_16 != EDDP_PRM_PDPORT_DATA_MAUTYPE_ADJUST_PROPERTIES)
													{ 
														EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
														Response = EDD_STS_ERR_PRM_BLOCK;
														EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
														EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> (%d) != EDDP_PRM_PDPORT_DATA_MAUTYPE_ADJUST_PROPERTIES!", 
															val_16);
														break;
													}
													else
													{
														if ( ((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_Begin) == record_length )
														{ 
															/*end of record reached*/ 
															EndOfRecordFlage = LSA_TRUE;
															break;
														}
														else
														{
															AdjustMauTypeIsPresent = LSA_TRUE;
														}
													}
												}
											}
										}
									}
								}
							}
                        }         
                        break; 
                    }
    
                    case EDDP_PRM_PDPORT_DATA_ADJUST_LINKSTATE_BLOCK_TYPE:
					{
						if(AdjustMauTypeIsPresent)
						{
							Response = EDD_STS_ERR_PRM_BLOCK;						
							EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
							EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> EDDP_PRM_PDPORT_DATA_ADJUST_LINKSTATE_BLOCK_TYPE called after EDDP_PRM_PDPORT_DATA_ADJUST_MAUTYPE_BLOCK_TYPE!");
							break;
						}
						else
						{
							// ***** AdjustLinkState = BlockHeader, Padding, Padding, LinkState, AdjustProperties
							// -------------------------------------------
							// check BlockType for AdjustLinkState 
							// -------------------------------------------    
							// ***** BlockHeader = BlockType, BlockLength, BlockVersionHigh, BlockVersionLow
							{
								EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): parse AdjustLinkState");
								// ***** skip BlockLength
								pRecord += 2;    
								// ***** check BlockVersion for AdjustLinkState
								// BlockVersionHigh
								pRecord_Tmp = pRecord;
								if (*pRecord++ != 0x01)
								{ 
									EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
									Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
									EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
									EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Block version does not match (AdjustLinkState)!");
									break;
								}
								else
								{
									// BlockVersionLow
									pRecord_Tmp = pRecord;
									if (*pRecord++ != 0x00)
									{ 
										EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
										Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
										EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
										EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Block version does not match (AdjustLinkState)!");
										break;
									}
									else
									{
										// ***** check 2x padding bytes
										pRecord_Tmp = pRecord;
										if (*pRecord++ != 0x00)
										{ 
											EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
											Response = EDD_STS_ERR_PRM_BLOCK;
											EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
											EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Padding is not '0'!"); 
											break;
										}
										else
										{
											pRecord_Tmp = pRecord;
											if (*pRecord++ != 0x00)
											{ 
												EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
												Response = EDD_STS_ERR_PRM_BLOCK;
												EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
												EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Padding is not '0'!");
												break;
											}
											else
											{
												pRecord_Tmp = pRecord;
												EDDP_GET_U16_INCR(pRecord, val_16); 
												// ***** check LinkState     
												if (val_16 != EDDP_PRM_LINK_STATE_DOWN)
												{ 
													EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
													Response = EDD_STS_ERR_PRM_BLOCK;
													EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
													EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> (%d) != EDDP_PRM_LINK_STATE_DOWN!", 
														val_16);
													break;
												}
												else
												{
													// ***** store LinkState
													pDDB->PRM.PortDataAdjust.Parameters[PortID].LinkState             = val_16;
													pDDB->PRM.PortDataAdjust.Parameters[PortID].LinkState_IsPresent   = LSA_TRUE;
													// ***** check AdjustProperties
													pRecord_Tmp = pRecord;
													EDDP_GET_U16_INCR(pRecord, val_16);        
													if (val_16 != EDDP_PRM_PDPORT_DATA_LINKSTATE_ADJUST_PROPERTIES)
													{ 
														EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
														Response = EDD_STS_ERR_PRM_BLOCK;
														EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
														EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> (%d) != EDDP_PRM_PDPORT_DATA_LINKSTATE_ADJUST_PROPERTIES!", val_16);
														break;
													}
													else
													{
														if ( ((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_Begin) == record_length )
														{ 
															/*end of record reached*/ 
															EndOfRecordFlage = LSA_TRUE;
															break;
														}
														else
														{
															AdjustLinkStateIsPresent = LSA_TRUE;
														}
													}
												}
											}
										}
									}
								}
							}
						}
						break;
					}
    
                case EDDP_PRM_PDPORT_DATA_ADJUST_P2P_BOUNDARY_BLOCK_TYPE:
                    // ***** AdjustPeerToPeerBoundary = BlockHeader, Padding, Padding, PeerToPeerBoundary, AdjustProperties, Padding, Padding
                    // -------------------------------------------
                    // check BlockType for AdjustPeerToPeerBoundary 
                    // -------------------------------------------    
                    // ***** BlockHeader = BlockType, BlockLength, BlockVersionHigh, BlockVersionLow
                    {
                        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): parse AdjustPeerToPeerBoundary");
                        // ***** skip BlockLength
                        pRecord += 2;        
                        // ***** check BlockVersion for AdjustPeerToPeerBoundary
                        // BlockVersionHigh
                        pRecord_Tmp = pRecord;
                        if (*pRecord++ != 0x01)
                        { 
                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                            Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Block version does not match (AdjustPeerToPeerBoundary)!");
                            break;
                        }
                        else
                        {
                            // BlockVersionLow
                            pRecord_Tmp = pRecord;
                            if (*pRecord++ != 0x00)
                            { 
                                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Block version does not match (AdjustPeerToPeerBoundary)!");
                                break;
                            }
                            else
                            {
                                // ***** check 2x padding bytes
                                pRecord_Tmp = pRecord;
                                if (*pRecord++ != 0x00)
                                { 
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Padding is not '0'!"); 
                                    break;
                                }
                                else
                                {
                                    pRecord_Tmp = pRecord;
                                    if (*pRecord++ != 0x00)
                                    { 
                                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                        Response = EDD_STS_ERR_PRM_BLOCK;
                                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Padding is not '0'!"); 
                                        break;
                                    }
                                    else
                                    {
                                        // ***** store PeerToPeerBoundary
                                        EDDP_GET_U32_INCR(pRecord, val_32);        
                                        pDDB->PRM.PortDataAdjust.Parameters[PortID].PeerToPeerBoundary           = val_32;
                                        pDDB->PRM.PortDataAdjust.Parameters[PortID].PeerToPeerBoundary_IsPresent = LSA_TRUE;
                                        // ***** check AdjustProperties
                                        pRecord_Tmp = pRecord;
                                        EDDP_GET_U16_INCR(pRecord, val_16);        
                                        if (val_16 != EDDP_PDPORT_DATA_ADJUST_P2P_ADJUST_PROPERTIES)
                                        { 
                                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                            Response = EDD_STS_ERR_PRM_BLOCK;
                                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> (%d) != EDDP_PDPORT_DATA_ADJUST_P2P_ADJUST_PROPERTIES!", 
                                                val_16);
                                            break;
                                        }
                                        else
                                        {
                                            // ***** check 2x padding bytes --> padding to 32bit
                                            pRecord_Tmp = pRecord;
                                            if (*pRecord++ != 0x00)
                                            { 
                                                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                                Response = EDD_STS_ERR_PRM_BLOCK;
                                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Padding is not '0'!"); 
                                                break;
                                            }
                                            else
                                            {
                                                pRecord_Tmp = pRecord;
                                                if (*pRecord++ != 0x00)
                                                { 
                                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Padding is not '0'!"); 
                                                    break;
                                                }
                                                else
                                                {
                                                    if ( ((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_Begin) == record_length )
                                                    { 
                                                        /*end of record reached*/ 
                                                        EndOfRecordFlage = LSA_TRUE;
                                                        break;
                                                    }   
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }  
                        break;
                    }
    
                case EDDP_PRM_PDPORT_DATA_ADJUST_DCP_BOUNDARY_BLOCK_TYPE:
    
                    // ***** AdjustDCPBoundary = BlockHeader, Padding, Padding, DCPBoundary, AdjustProperties, Padding, Padding
                    // -------------------------------------------
                    // check BlockType for AdjustDCPBoundary 
                    // -------------------------------------------    
                    // ***** BlockHeader = BlockType, BlockLength, BlockVersionHigh, BlockVersionLow
                    {
                        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): parse AdjustDCPBoundary");
                        // ***** skip BlockLength
                        pRecord += 2;    
                        // ***** check BlockVersion for AdjustDCPBoundary
                        // BlockVersionHigh
                        pRecord_Tmp = pRecord;
                        if (*pRecord++ != 0x01)
                        { 
                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                            Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Block version does not match (AdjustDCPBoundary)!");
                            break;
                        }
                        else
                        {
                            // BlockVersionLow
                            pRecord_Tmp = pRecord;
                            if (*pRecord++ != 0x00)
                            { 
                                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Block version does not match (AdjustDCPBoundary)!");
                                break;
                            }
                            else
                            {
                                // ***** check 2x padding bytes
                                pRecord_Tmp = pRecord;
                                if (*pRecord++ != 0x00)
                                { 
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Padding is not '0'!"); 
                                    break;
                                }
                                else
                                {
                                    pRecord_Tmp = pRecord;
                                    if (*pRecord++ != 0x00)
                                    { 
                                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                        Response = EDD_STS_ERR_PRM_BLOCK;
                                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Padding is not '0'!"); 
                                        break;
                                    }
                                    else
                                    {
                                        // ***** store DCPBoundary
                                        EDDP_GET_U32_INCR(pRecord, val_32);        
                                        pDDB->PRM.PortDataAdjust.Parameters[PortID].DCPBoundary           = val_32;
                                        pDDB->PRM.PortDataAdjust.Parameters[PortID].DCPBoundary_IsPresent = LSA_TRUE;
                                        // ***** check AdjustProperties
                                        pRecord_Tmp = pRecord;
                                        EDDP_GET_U16_INCR(pRecord, val_16);        
                                        if (val_16 != EDDP_PRM_PDPORT_DATA_DCP_ADJUST_PROPERTIES)
                                        { 
                                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                            Response = EDD_STS_ERR_PRM_BLOCK;
                                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> (%d) != EDDP_PRM_PDPORT_DATA_DCP_ADJUST_PROPERTIES!", 
                                                val_16);
                                            break;
                                        }
                                        else
                                        {
                                            // ***** check 2x padding bytes --> padding to 32bit
                                            pRecord_Tmp = pRecord;
                                            if (*pRecord++ != 0x00)
                                            { 
                                                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                                Response = EDD_STS_ERR_PRM_BLOCK;
                                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Padding is not '0'!"); 
                                                break;
                                            }
                                            else
                                            {
                                                pRecord_Tmp = pRecord;
                                                if (*pRecord++ != 0x00)
                                                { 
                                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Padding is not '0'!"); 
                                                    break;
                                                }
                                                else
                                                {
                                                    if ( ((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_Begin) == record_length )
                                                    { 
                                                        /*end of record reached*/ 
                                                        EndOfRecordFlage = LSA_TRUE;
                                                        break;
                                                    }  
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }     
                        break;
                    }
    
                case EDDP_PRM_PDPORT_DATA_ADJUST_PREAMBLELENGTH_BLOCK_TYPE:
                    // ***** AdjustPreambleLength = BlockHeader, Padding, Padding, PreambleLength, AdjustProperties, [Padding*]
                    // -------------------------------------------
                    // check BlockType for AdjustPreambleLength 
                    // -------------------------------------------    
                    // ***** BlockHeader = BlockType, BlockLength, BlockVersionHigh, BlockVersionLow
                    {
                        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): parse AdjustPreambleLength");
                        // ***** skip BlockLength
                        pRecord += 2;    
                        // ***** check BlockVersion for AdjustDCPBoundary
                        // BlockVersionHigh
                        pRecord_Tmp = pRecord;
                        if (*pRecord++ != 0x01)
                        { 
                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                            Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Block version does not match (AdjustPreambleLength)!");
                            break;
                        }
                        else
                        {
                            // BlockVersionLow 
                            pRecord_Tmp = pRecord;
                            if (*pRecord++ != 0x00)
                            { 
                                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Block version does not match (AdjustPreambleLength)!");
                                break;
                            }
                            else
                            {
                                // ***** check 2x padding bytes
                                pRecord_Tmp = pRecord;
                                if (*pRecord++ != 0x00)
                                { 
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Padding is not '0'!"); 
                                    break;
                                }
                                else
                                {
                                    pRecord_Tmp = pRecord;
                                    if (*pRecord++ != 0x00)
                                    { 
                                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                        Response = EDD_STS_ERR_PRM_BLOCK;
                                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Padding is not '0'!"); 
                                        break;
                                    }
                                    else
                                    {
                                        // ***** check PreambleLength (in Bit.0)
                                        pRecord_Tmp = pRecord;
                                        EDDP_GET_U16_INCR(pRecord, val_16);
                                        if( (EDDP_PRM_PDPORT_DATA_PREAMBLELENGTH_SHORT & val_16) && (0 == pDDB->FeatureSupport.bShortPreambleSupported) )
                                        {
                                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                            Response = EDD_STS_ERR_PRM_BLOCK;
                                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);                                            
                                            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> short preamble not supported!");
											break;
                                        }
                                        pDDB->PRM.PortDataAdjust.Parameters[PortID].PreambleLength = val_16;
                                        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(PortID=%d): PreambleLength=%d",
                                            PortID, val_16);
                                        pDDB->PRM.PortDataAdjust.Parameters[PortID].PreambleLength_IsPresent = LSA_TRUE;
                                        // ***** check AdjustProperties
                                        pRecord_Tmp = pRecord;
                                        EDDP_GET_U16_INCR(pRecord, val_16);        
                                        if (val_16 != EDDP_PRM_PDPORT_DATA_ADJUST_PREAMBLELENGTH_PROPERTIES)
                                        { 
                                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                            Response = EDD_STS_ERR_PRM_BLOCK;
                                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> (%d) != EDDP_PRM_PDPORT_DATA_ADJUST_PREAMBLELENGTH_PROPERTIES!", 
                                                val_16);
                                            break;
                                        }
                                        else
                                        {
                                            if ( ((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_Begin) == record_length )
                                            { 
                                                /*end of record reached*/ 
                                                EndOfRecordFlage = LSA_TRUE;
                                                break;
                                            }  
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    }
    
                /* Unknown BlockType */
                default:
                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_BlockType - pRecord_Begin);
                    Response = EDD_STS_ERR_PRM_BLOCK;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> BlockType not defined");
                    break;
            }
        }
    }

    if (EndOfRecordFlage)
    {
        // ***** check if number of read bytes is same as record length
        if ( ((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_Begin) != record_length )
        { 
            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_RecordLength - pRecord_Begin);
            Response = EDD_STS_ERR_PRM_BLOCK;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDPortDataAdjustRecord_Write(): ERROR -> Record length does not match with number of read bytes!");
        }
    }

    if (Response == EDD_STS_OK)
    {
        // ***** store record in record set B
        EDDP_MEMCOPY_COMMON(pDDB->PRM.pRecordsetB->PDPortDataAdjust[PortID].pRecord, pRecord_Begin, record_length);
        pDDB->PRM.pRecordsetB->PDPortDataAdjust[PortID].Record_Length     = record_length;
        pDDB->PRM.pRecordsetB->PDPortDataAdjust[PortID].Record_IsPresent  = LSA_TRUE;
    }

    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMPDPortDataAdjustRecord_Write(): Response=0x%X", Response);

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMRequestPrmIndication                +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                         +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         Opcode:     EDD_OPC_REQUEST                 +*/
/*+     LSA_HANDLE_TYPE         Handle:                                     +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user (not used)           +*/
/*+     EDD_SERVICE             Service:    EDD_SRV_PRM_INDICATION          +*/
/*+     LSA_RESPONSE_TYPE       Response:   Return status                   +*/
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:                                     +*/
/*+                                                                         +*/
/*+  pParam points to EDD_UPPER_PRM_WRITE_PTR_TYPE                          +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_SEQUENCE                                      +*/
/*+               EDD_STS_ERR_PRM_INDEX                                     +*/
/*+               EDD_STS_ERR_PRM_DATA                                      +*/
/*+               EDD_STS_ERR_PRM_BLOCK                                     +*/
/*+               EDD_STS_ERR_PRM_PORTID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function handles the EDD_SRV_PRM_INDICATION request  +*/
/*+                                                                         +*/
/*+               PortID 0 = Interface                                      +*/
/*+                                                                         +*/
/*+               Note: Opcode/Service not checked!                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMRequestPrmIndication(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP								Response;
    LSA_UINT16							PortID;
    EDD_UPPER_PRM_INDICATION_PTR_TYPE   pIndParam;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pHDB);
#endif

    Response    = EDD_STS_OK;
    pIndParam   = (EDD_UPPER_PRM_INDICATION_PTR_TYPE) pRQB->pParam;

    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_PRMRequestPrmIndication(pRQB=0x%X)", pHDB->Handle, pRQB);

    /* -------------------------------- */
    /* check parameters                 */
    /* -------------------------------- */
    if (LSA_HOST_PTR_ARE_EQUAL(pIndParam, LSA_NULL))
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmIndication(): ERROR -> RQB->pParam=NULL", 
            pHDB->Handle);
    }
    else
    {
        if (pIndParam->edd_port_id > pDDB->SWI.PortCnt)
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMRequestPrmIndication(): ERROR -> RQB.edd_port_id(%d) > SWI.PortCnt(%d)", 
                pHDB->Handle, pIndParam->edd_port_id, pDDB->SWI.PortCnt);
        }
    }

    /* -------------------------------- */
    /* try to indicate                  */
    /* -------------------------------- */
    if (Response == EDD_STS_OK)
    {
        PortID = pIndParam->edd_port_id;

        // put this RQB to Indication Queue
        EDDP_GlbRQBQueueAddToEnd (&pDDB->PRM.PrmIndReq[PortID], pRQB);
        // and init with 0
        pIndParam->diag_cnt = 0;

        EDDP_PRM_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_PRMRequestPrmIndication(PortID=%d): new RqbQueue.Count=%d, isPrmCommitRunning=0x%X", 
            pHDB->Handle, PortID, pDDB->PRM.PrmIndReq[PortID].Count, pDDB->PRM.isPrmCommitRunning);

        // try to indicate only if no PrmCommit is running currently
        if ( !(pDDB->PRM.isPrmCommitRunning) )
        {
            // Port diagnosis
            EDDP_PRMDiagIndicateDiagOnePort (pDDB, PortID, LSA_FALSE);

            // only for PortID=0 (Interface)
            if (PortID == 0)
            {
                // is indication resource still present ?
                if (pDDB->PRM.PrmIndReq[PortID].Count > 0)
                {
                    // trigger MRT diagnosis of MRPD consumers in CRT if something is pending
                    EDDP_PRMDiagTriggerMRTDiagAllPending (pDDB, LSA_FALSE);

                    // is indication resource still present ?
                    if (pDDB->PRM.PrmIndReq[PortID].Count > 0)
                    {
                        // trigger the LateError diagnosis 
                        EDDP_PRMDiagTriggerLateErrorDiagAllPending (pDDB, LSA_FALSE);
                    }
                }
            }
        }
    }

    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMRequestPrmIndication(): Response=0x%X", Response);

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMRequest                             +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters          (in) +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         Opcode:     EDD_OPC_REQUEST                 +*/
/*+     LSA_HANDLE_TYPE         Handle:                                     +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user (not used)           +*/
/*+     EDD_SERVICE             Service:    EDD_SRV_xxx                     +*/
/*+                                         EDD_SRV_xxx                     +*/
/*+                                         EDD_SRV_xxx                     +*/
/*+     LSA_RESPONSE_TYPE       Response:   Return status                   +*/
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:     Depend on service               +*/
/*+                                                                         +*/
/*+  RQB-return values via callback-function depends on service             +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function handles the following services for PRM:     +*/
/*+                                                                         +*/
/*+               EDD_SRV_xxx_yyy                                           +*/
/*+                                                                         +*/
/*+               If we have a parameter error we will set EDD_STS_ERR_PARAM+*/
/*+               and call the handle-callbackfunction and abort the request+*/
/*+                                                                         +*/
/*+               EDD_STS_ERR_SERVICE is set if we get an invalid Service.  +*/
/*+               EDD_STS_ERR_CHANNEL_USE is set if the service is invalid  +*/
/*+               for this channel (i.e. not used by this channel)          +*/
/*+                                                                         +*/
/*+               If the request is valid, it is queued or executed.        +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMRequest(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP         Response;
    EDD_SERVICE     Service;
    LSA_BOOL        isRequestFinish;

    Response            = EDD_STS_OK;
    Service             = EDD_RQB_GET_SERVICE(pRQB);
    isRequestFinish     = LSA_TRUE;

    EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] >>> EDDP_PRMRequest(pRQB=0x%X)", pHDB->Handle, pRQB);

    /* -------------------------------------------- */
    /* do not check the "pRQB->pParam=NULL"         */
    /* it is allowed here                           */
    /* -------------------------------------------- */

    switch (Service)
    {
        /* ---------------------------------------------------------------- */
        /* PrmRead                                                          */
        /* PrmPrepare                                                       */
        /* PrmWrite                                                         */
        /* PrmEnd                                                           */
        /* ---------------------------------------------------------------- */
        case EDD_SRV_PRM_READ:
        case EDD_SRV_PRM_PREPARE:
        case EDD_SRV_PRM_WRITE:
        case EDD_SRV_PRM_END:
        case EDD_SRV_PRM_CHANGE_PORT:
        {
            // put this RQB to Queue
            EDDP_GlbRQBQueueAddToEnd (&(pDDB->GlbCrtRQBHandler.RqbQueue), pRQB);
            pHDB->pPRM->RunningRQBCount++;
            // and call RQB-Handler
            EDDP_GlbCrtRQBHandler (pDDB);
            // request is queued
            isRequestFinish = LSA_FALSE;
        }
        break;

        /* ---------------------------------------------------------------- */
        /* PrmIndication                                                    */
        /* ---------------------------------------------------------------- */
        case EDD_SRV_PRM_INDICATION:
        {
            EDDP_UPPER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_PRM_INDICATION", pHDB->Handle);

            Response = EDDP_PRMRequestPrmIndication (pRQB, pDDB, pHDB);

            if (Response == EDD_STS_OK)
            {
                isRequestFinish = LSA_FALSE;
            }
        }
        break;

        /* ---------------------------------------------------------------- */
        /* PrmCommit                                                        */
        /* ---------------------------------------------------------------- */
        case EDD_SRV_PRM_COMMIT:
        {
            // these flags may not be deleted here, because if a PrmCommit is current running asynchronous, these flags are set and they would be deleted by the new PrmCommit here
            //pDDB->PRM.isPrmCommitRunning        = LSA_FALSE;
            //pDDB->PRM.isTriggerPrmCommitFinish  = LSA_FALSE;

            // put this RQB to Queue
            EDDP_GlbRQBQueueAddToEnd (&(pDDB->GlbCrtRQBHandler.RqbQueue), pRQB);
            pHDB->pPRM->RunningRQBCount++;
            // and call RQB-Handler
            EDDP_GlbCrtRQBHandler (pDDB);
            // request is queued
            isRequestFinish = LSA_FALSE;
        }
        break;

        /* ---------------------------------------------------------------- */
        /* Unknown/Unsupported Request                                      */
        /* ---------------------------------------------------------------- */
        default:
        {
            Response = EDD_STS_ERR_SERVICE;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] >>> EDDP_PRMRequest(): ERROR -> Unknown/Unsupported Service(0x%X)", 
                pHDB->Handle, Service);
        }
        break;
    }

    if (isRequestFinish)
    {
        EDDP_GlbRequestFinish (pHDB, pRQB, Response);
    }

    EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] <<< EDDP_PRMRequest(): Response=0x%X", pHDB->Handle, Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMCloseChannel                        +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB           +*/
/*+                        :    EDDP_LOCAL_HDB_PTR_TYPE      pHDB           +*/
/*+                             LSA_HANDLE_TYPE              Handle         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                    (in) +*/
/*+  Handle     : Handle for this Channel (<  EDD_CFG_MAX_CHANNELS)    (in) +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_OK_PENDING                                        +*/
/*+               EDD_STS_ERR_xxx                                           +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Close Channel for PRM.                                    +*/
/*+                                                                         +*/
/*+       Note:   EDD_STS_OK_PENDING not allowed as response. This call     +*/
/*+               must be synchronously!                                    +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMCloseChannel(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB,
    LSA_HANDLE_TYPE             Handle)
{
    EDD_RSP                     Response;
    LSA_UINT32                  i;
    EDD_UPPER_RQB_PTR_TYPE      pRQB;

    LSA_UNUSED_ARG(Handle);

    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_PRMCloseChannel(Handle=0x%X)", pHDB->Handle, Handle);

    Response = EDD_STS_OK;

    // is IRT still active?
    if (pDDB->SWI.IRTState != 0)
    {
        Response = EDD_STS_ERR_SEQUENCE;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMCloseChannel(): ERROR -> IRT is still active in HW! SWI.IRTState=0x%X", 
            pDDB->SWI.IRTState);
    }
    else
    {
        if (pDDB->PRM.HandleCnt == 1)
        {
            /* ------------------------------------------------------------ */
            /* check if there are some RQBs in processing of this channel   */
            /* ------------------------------------------------------------ */
            if (pHDB->pPRM->RunningRQBCount > 0)
            {
                Response = EDD_STS_ERR_SEQUENCE;
                EDDP_DBG_SET_DETAIL_ERR(pHDB->pDDB, Response);
                EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMCloseChannel(): ERROR -> There are still some RQBs in processing of this channel! PRM-RunningRQBCount=%d, Response=0x%X", 
                    pHDB->Handle, pHDB->pPRM->RunningRQBCount, Response);
            }
            else
            {
                /* ---------------------------------------------------------------- */
                /* Release all Prm-Indication RQBs with CANCEL                      */
                /*----------------------------------------------------------------- */
                for ( i=0; i <= pDDB->SWI.PortCnt; i++ )
                {
                    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:%2X] EDDP_PRMCloseChannel(PortID=%d): Indication-RqbQueue.Count=%d", 
                        pHDB->Handle, i, pDDB->PRM.PrmIndReq[i].Count);

                    do
                    {
                        pRQB = EDDP_GlbRQBQueueRemoveFromBegin (&pDDB->PRM.PrmIndReq[i]);
                                                
                        if (!(LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL)))
                        {
                            EDDP_GlbRequestFinish (pHDB, pRQB, EDD_STS_OK_CANCEL);
                        }                        
                    } while (!(LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL)));
                }

                pDDB->PRM.HandleCnt = 0;
                pDDB->PRM.Handle    = 0;

                /* ------------------------------------------------------------ */
                /* now free local memory of PRM management of this channel      */
                /* ------------------------------------------------------------ */
                #ifndef EDDP_CFG_LOCAL_MEM_STATIC
                if (LSA_HOST_PTR_ARE_EQUAL(pHDB->pPRM, LSA_NULL))
                {
                    // the pointer is already deleted !
                }
                else
                {
                    EDDP_GlbFreeLocalMem (pHDB->pPRM);
                }
                #endif

                pHDB->pPRM = LSA_NULL;
            }
        }
        else
        {
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMCloseChannel(): FATAL -> PRM.HandleCnt(%d) != 1",
                pDDB->PRM.HandleCnt);
            EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
        }
    }

    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMCloseChannel(): Response=0x%X", Response);

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMCheckFrameDrop                      +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB           +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called by a cyclic timer and checks if   +*/
/*+               one of the frame drop limits have been reached since the  +*/
/*+               last call of this function. If so, a PRM_INDICATION       +*/
/*+               signals this diagnosis.                                   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMCheckFrameDrop(
    EDDP_LOCAL_DDB_PTR_TYPE      pDDB)
{
    LSA_UINT32      reg_val, reg_address;
    LSA_UINT32      i;
    LSA_UINT32      HwPortID;
    LSA_UINT64      dropped_frames_delta = 0;

    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMCheckFrameDrop()");

    // sum up dropped frames from all ports of this interface
    for ( i=1; i <= pDDB->SWI.PortCnt; i++ )
    {
        HwPortID    = EDDP_SWI_get_HwPortID (pDDB, i);
        reg_address = (HwPortID==1 ? PNIP_REG_FRAMES_DROPPED_L2_P1 : 
                      (HwPortID==2 ? PNIP_REG_FRAMES_DROPPED_L2_P2 : 
                      (HwPortID==3 ? PNIP_REG_FRAMES_DROPPED_L2_P3 : 
                      (HwPortID==4 ? PNIP_REG_FRAMES_DROPPED_L2_P4 : 0))));
        reg_val     = EDDP_HAL_REG32_READ(pDDB, reg_address);

        if (reg_val < pDDB->SWI.DroppedFramesCounter[i])
        {
            // there has been an overflow of the frame drop counter
             dropped_frames_delta += (0xFFFFFFFF - pDDB->SWI.DroppedFramesCounter[i]) + reg_val;
        }
        else
        {
            dropped_frames_delta += reg_val - pDDB->SWI.DroppedFramesCounter[i];
        }

        pDDB->SWI.DroppedFramesCounter[i] = reg_val;
    }

    if (dropped_frames_delta != 0)
    {
        // there have been some dropped frames since last call
        if (   (pDDB->PRM.FrameDrop.Budgets.ErrorDropBudget.Enabled)
            && (dropped_frames_delta >= pDDB->PRM.FrameDrop.Budgets.ErrorDropBudget.Value)
           )
        {
            pDDB->PRM.FrameDrop.ErrorStatus = EDDP_DIAG_FRAME_DROP_ERROR;
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H:--] EDDP_PRMCheckFrameDrop(): EDDP_DIAG_FRAME_DROP_ERROR | %d Frames dropped in the last second!", 
                dropped_frames_delta);
        }
        else if (   (pDDB->PRM.FrameDrop.Budgets.MaintenanceDemandedDropBudget.Enabled)
                 && (dropped_frames_delta >= pDDB->PRM.FrameDrop.Budgets.MaintenanceDemandedDropBudget.Value)
                )
        {
            pDDB->PRM.FrameDrop.ErrorStatus = EDDP_DIAG_FRAME_DROP_MAINTENANCE_DEMANDED;
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H:--] EDDP_PRMCheckFrameDrop(): EDDP_DIAG_FRAME_DROP_MAINTENANCE_DEMANDED | %d Frames dropped in the last second!", 
                dropped_frames_delta);            
        }
        else if (   (pDDB->PRM.FrameDrop.Budgets.MaintenanceRequiredDropBudget.Enabled)
                 && (dropped_frames_delta >= pDDB->PRM.FrameDrop.Budgets.MaintenanceRequiredDropBudget.Value)
                )
        {
            pDDB->PRM.FrameDrop.ErrorStatus = EDDP_DIAG_FRAME_DROP_MAINTENANCE_REQUIRED;
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H:--] EDDP_PRMCheckFrameDrop(): EDDP_DIAG_FRAME_DROP_MAINTENANCE_REQUIRED | %d Frames dropped in the last second!", 
                dropped_frames_delta);            
        }
        else
        {
            pDDB->PRM.FrameDrop.ErrorStatus = EDDP_DIAG_FRAME_DROP_OK;
        }

        // store new total drop counter
        pDDB->PRM.FrameDrop.DropCnt += dropped_frames_delta;
    }
    else
    {
        // no dropped frames since last call
        pDDB->PRM.FrameDrop.ErrorStatus = EDDP_DIAG_FRAME_DROP_OK;                
    }

    // try to indicate only if no PrmCommit is running currently
    if ( !(pDDB->PRM.isPrmCommitRunning) )
    {
        // indicate diagnosis change
        EDDP_PRMDiagIndicateDiagOnePort (pDDB, EDD_PORT_ID_AUTO, LSA_FALSE);
    }
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_PRMGetPortCountFromTxPortGroupArray               +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                       +*/
/*+                      LSA_UINT8                   TxPortGroupArray           +*/
/*+                                                                             +*/
/*+  Result         :    LSA_UINT32                                             +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                     +*/
/*+  TxPortGroupArray   : TxPortGroupArray from PRM-Record                      +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function counts the number of TxPorts coded in           +*/
/*+               TxPortGroupArray of PRM-Record.                               +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_PRMGetPortCountFromTxPortGroupArray(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT8                   TxPortGroupArray)
{
    LSA_UINT32      TxPortCount;

    LSA_UNUSED_ARG(pDDB);

    TxPortCount = 0;
    EDDP_SHIFT_VALUE_RIGHT(TxPortGroupArray, 1);  // skip Bit(0) API

    while (TxPortGroupArray != 0)
    {
        if (TxPortGroupArray & 0x1)
        {
            TxPortCount++;  // count TxPorts
        }
        EDDP_SHIFT_VALUE_RIGHT(TxPortGroupArray, 1); // next port of this TxPortGroupArray
    }

    return (TxPortCount);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_PRM_Map_UserTxPortGroupArray_To_HwTxPortGroupArray+*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                       +*/
/*+                      LSA_UINT8                   UserTxPortGroupArray       +*/
/*+                                                                             +*/
/*+  Result         :    LSA_UINT8                                              +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB                   : Pointer to DeviceDescriptionBlock                 +*/
/*+  UserTxPortGroupArray   : UserTxPortGroupArray from PRM-Record              +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function converts the UserTxPortGroupArray to            +*/
/*+               HwTxPortGroupArray.                                           +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_UINT8 EDDP_LOCAL_FCT_ATTR EDDP_PRM_Map_UserTxPortGroupArray_To_HwTxPortGroupArray(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT8                   UserTxPortGroupArray)
{
    LSA_UINT8       HwTxPortGroupArray;
    LSA_UINT32      PortID;

    HwTxPortGroupArray  = 0;
    PortID              = 0;

    // Bit(0) API
    if (UserTxPortGroupArray & 0x1)
    {
        HwTxPortGroupArray |= 0x1;
    }
    EDDP_SHIFT_VALUE_RIGHT(UserTxPortGroupArray, 1);  // skip Bit(0) API

    // Bit(1..x) TxPorts
    // loop through the UserTxPortGroupArray
    while (UserTxPortGroupArray != 0)
    {
        PortID++;

        if (UserTxPortGroupArray & 0x1)
        {
            // map this UserPort to HwPort and store it in HwTxPortGroupArray
            switch (EDDP_SWI_get_HwPortID(pDDB, PortID))
            {
                case 1:     HwTxPortGroupArray |= EDDP_SWI_IRT_ENTRY_TXPORT_1;  break;
                case 2:     HwTxPortGroupArray |= EDDP_SWI_IRT_ENTRY_TXPORT_2;  break;
                case 3:     HwTxPortGroupArray |= EDDP_SWI_IRT_ENTRY_TXPORT_3;  break;
                case 4:     HwTxPortGroupArray |= EDDP_SWI_IRT_ENTRY_TXPORT_4;  break;
                default:
                {
                    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRM_Map_UserTxPortGroupArray_To_HwTxPortGroupArray(): FATAL -> Invalid Hw-PortID(%d)!", 
                        EDDP_SWI_get_HwPortID(pDDB, PortID));
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                }
                break;
            }
        }

        EDDP_SHIFT_VALUE_RIGHT(UserTxPortGroupArray, 1); // next port of this UserTxPortGroupArray
    }

    return (HwTxPortGroupArray);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMPDPortStatisticRecord_Read          +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                         +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PRM_BLOCK                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function reads the Port/Inteface-Statistic           +*/
/*+               and stores it to PrmRead RQB.                             +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDPortStatisticRecord_Read(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP                         Response;
    LSA_UINT32                      HwPortID;
    EDDP_GET_STATISTICS_TYPE        StatisticData;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecordData;
    EDD_UPPER_PRM_READ_PTR_TYPE     pPrmReadParam;
    LSA_UINT16                      CounterStatus = ((LSA_UINT16)(~(EDDP_RECORD_SUPPORTED_COUNTERS_ALL))) & EDD_RECORD_SUPPORT_MASK;

    #if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pHDB);
    #endif

    Response        = EDD_STS_OK;
    pPrmReadParam   = (EDD_UPPER_PRM_READ_PTR_TYPE) pRQB->pParam;
    pRecordData     = pPrmReadParam->record_data;

    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_PRMPDPortStatisticRecord_Read(pRQB=0x%X): PortID=%d",
        pHDB->Handle, pRQB, pPrmReadParam->edd_port_id);

    /* ------------------------------------ */
    /* check PortID                         */
    /* ------------------------------------ */
    // PortID=0     : interface
    // PortID=1..n  : Port
    if (pPrmReadParam->edd_port_id > pDDB->SWI.PortCnt)
    {
        Response = EDD_STS_ERR_PRM_PORTID;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_PRMPDPortStatisticRecord_Read(): ERROR -> PortID(%d) is not between 0..%d!", 
            pHDB->Handle, pPrmReadParam->edd_port_id, pDDB->SWI.PortCnt);
    }
    else
    {
        /* ------------------------------------ */
        /* get current StatisticData            */
        /* ------------------------------------ */
        if (pPrmReadParam->edd_port_id == 0)
        {
            // Interface Statistic
            StatisticData.RequestedCounters = (EDDP_HAL_HW_PNIP_REV3 == EDDP_HALGetPnipRevision(pDDB))?EDDP_RECORD_REQUESTABLE_COUNTERS_IF_REV3:EDDP_RECORD_REQUESTABLE_COUNTERS_IF;
            EDDP_GEN_GetIfaceStatistics (pDDB, &StatisticData);

            if (EDDP_HALGetPnipRevision(pDDB) != EDDP_HAL_HW_PNIP_REV3)
            {
                //"CounterStatus" is coded in negative logic (0=supported)
                CounterStatus = ((LSA_UINT16)(~(EDDP_RECORD_SUPPORTED_COUNTERS_IF_REV1_2))) & EDD_RECORD_SUPPORT_MASK;
            }
        }
        else
        {
            // Port Statistic
            StatisticData.RequestedCounters = (EDDP_HAL_HW_PNIP_REV1 == EDDP_HALGetPnipRevision(pDDB))?EDDP_RECORD_REQUESTABLE_COUNTERS_PORT_REV1:EDDP_RECORD_REQUESTABLE_COUNTERS_PORT_REV2;
            HwPortID = EDDP_SWI_get_HwPortID (pDDB, pPrmReadParam->edd_port_id);
            EDDP_GEN_GetPortStatistics (pDDB, &StatisticData, HwPortID);
            if (EDDP_HALGetPnipRevision(pDDB) == EDDP_HAL_HW_PNIP_REV1)
            {
                //"CounterStatus" is coded in negative logic (0=supported)
                CounterStatus = ((LSA_UINT16)(~(EDDP_RECORD_SUPPORTED_COUNTERS_PORT_REV1))) & EDD_RECORD_SUPPORT_MASK;
            }
        }

        if (Response != EDD_STS_OK)
        {
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:%2X] EDDP_PRMPDPortStatisticRecord_Read(): FATAL -> Response(0x%X) of GEN-GetStatistic is not ok!", 
                pHDB->Handle, Response);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        else
        {
            /* ------------------------------------ */
            /* write StatisticData to RecordData    */
            /* ------------------------------------ */
            // BlockType
            EDDP_SET_U16_INCR(pRecordData, EDDP_PRM_PDPORT_STATISTIC_BLOCKTYPE);
            // BlockLength
            EDDP_SET_U16_INCR(pRecordData, EDDP_PRM_PDPORT_STATISTIC_BLOCKLENGTH);
            // BlockVersionHigh
            EDDP_SET_U8_INCR(pRecordData, EDDP_PRM_PDPORT_STATISTIC_BLOCKVERSIONHIGH);
            // BlockVersionLow
            EDDP_SET_U8_INCR(pRecordData, EDDP_PRM_PDPORT_STATISTIC_BLOCKVERSIONLOW);

            /* ------------------------------------------------------------------------ */
            /* >>> CounterStatus                                                        */
            /* ------------------------------------------------------------------------ */
            // CounterStatus
            EDDP_SET_U16_INCR(pRecordData, CounterStatus);

            /* ------------------------------------------------------------------------ */
            /* >>> StatisticData                                                        */
            /* ------------------------------------------------------------------------ */
            // ifInOctets
            EDDP_SET_U32_INCR(pRecordData, StatisticData.InOctets);
            // ifOutOctets
            EDDP_SET_U32_INCR(pRecordData, StatisticData.OutOctets);
            // ifInDiscards
            EDDP_SET_U32_INCR(pRecordData, StatisticData.InDiscards);
            // ifOutDiscards
            EDDP_SET_U32_INCR(pRecordData, StatisticData.OutDiscards);
            // ifInErrors
            EDDP_SET_U32_INCR(pRecordData, StatisticData.InErrors);
            // ifOutErrors
            EDDP_SET_U32_INCR(pRecordData, StatisticData.OutErrors);

            // Padding: only for alignment (the block must be UINT32 aligned)
            //EDDP_SET_U8_INCR(pRecordData, 0x00);
            /* ------------------------------------------------------------------------ */
            /* <<< StatisticData                                                        */
            /* ------------------------------------------------------------------------ */
        }
    }

    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMPDPortStatisticRecord_Read(): Response=0x%X", Response);

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMPDSCFDataCheckRecord_Write          +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDD_UPPER_MEM_U8_PTR_TYPE   pRecord         +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pRecord    : Pointer to PRM record                                     +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PRM_BLOCK                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function parses the PDSCFDataCheck record (pRecord)  +*/
/*+               and stores the extracted data into DDB.                   +*/
/*+                                                                         +*/
/*+               Check for valid SendClockFactor.                          +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDSCFDataCheckRecord_Write(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_UPPER_MEM_U8_PTR_TYPE   pRecord)
{
    EDD_RSP                         Response;
    LSA_UINT16                      val_16;
    LSA_UINT32                      record_length;
    LSA_UINT8                       BlockVersionHigh, BlockVersionLow;
    LSA_UINT16                      SendClockFactor;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_Begin;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_Tmp;
    EDDP_PRM_DETAIL_ERR_PTR_TYPE    pDetailErr;

    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_PRMPDSCFDataCheckRecord_Write()");

    Response        = EDD_STS_OK;
    pDetailErr      = &pDDB->PrmDetailErr;
    pRecord_Begin   = pRecord;
    record_length   = 0;

    // check BlockType
    pRecord_Tmp = pRecord;
    EDDP_GET_U16_INCR(pRecord, val_16);
    if (val_16 != EDDP_PRM_PDSCF_DATA_CHECK_BLOCK_TYPE)
    {
        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
        Response = EDD_STS_ERR_PRM_BLOCK;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDSCFDataCheckRecord_Write(): ERROR -> BlockType(0x%X) != EDDP_PRM_PDSCF_DATA_CHECK_BLOCK_TYPE(0x%X)!", 
            val_16, EDDP_PRM_PDSCF_DATA_CHECK_BLOCK_TYPE);
    }
    else
    {
        // check BlockLength
        pRecord_Tmp = pRecord;
        EDDP_GET_U16_INCR(pRecord, val_16);
        record_length = (val_16 + 4);
        if (record_length > pDDB->PRM.PDSCFDataCheck.PDSCFDataCheck_Length)
        {
            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
            Response = EDD_STS_ERR_PRM_BLOCK;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDSCFDataCheckRecord_Write(): ERROR -> Record length(%d) does not match with length in RQB->record_data_length(%d)!", 
                record_length, pDDB->PRM.PDSCFDataCheck.PDSCFDataCheck_Length);
        }
        else
        {
            // check BlockVersionHigh
            pRecord_Tmp = pRecord;
            BlockVersionHigh = *pRecord++;
            if (BlockVersionHigh != EDDP_PRM_PDSCF_DATA_CHECK_BLOCKVERSIONHIGH)
            {
                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDSCFDataCheckRecord_Write(): ERROR -> BlockVersionHigh(0x%X) does not match (PDSCFDataCheck)!", 
                    BlockVersionHigh);
            }
            else
            {
                // check BlockVersionLow
                pRecord_Tmp = pRecord;
                BlockVersionLow = *pRecord++;
                if (BlockVersionLow != EDDP_PRM_PDSCF_DATA_CHECK_BLOCKVERSIONLOW)
                {
                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                    Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDSCFDataCheckRecord_Write(): ERROR -> BlockVersionLow(0x%X) does not match (PDSCFDataCheck)!", 
                        BlockVersionLow);
                }
            }
        }
    }

    if (Response == EDD_STS_OK)
    {
        // SendClockFactor
        pRecord_Tmp = pRecord;
        EDDP_GET_U16_INCR(pRecord, SendClockFactor);

        if (SendClockFactor != pDDB->SWI.CycleInfo.CycleBaseFactor)
        {
            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
            Response = EDD_STS_ERR_PRM_BLOCK;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDSCFDataCheckRecord_Write(): ERROR -> PDSCFDataCheck.SendClockFactor(%d) != SWI.CycleBaseFactor(%d)", 
                SendClockFactor, pDDB->SWI.CycleInfo.CycleBaseFactor);
        }
    }

    if (Response == EDD_STS_OK)
    {
        // store record in PRM
        EDDP_MEMCOPY_COMMON(pDDB->PRM.PDSCFDataCheck.pPDSCFDataCheck, pRecord_Begin, record_length);
        pDDB->PRM.PDSCFDataCheck.PDSCFDataCheck_Length      = record_length;
        pDDB->PRM.PDSCFDataCheck.PDSCFDataCheck_IsPresent   = LSA_TRUE;
    }

    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMPDSCFDataCheckRecord_Write(): Response=0x%X", Response);

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMTriggerPrmCommitFinish              +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB           +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function contains some activities for finish of      +*/
/*+               PrmCommit after PrmCommit confirmation.                   +*/
/*+                 -> after EDDP_GlbRequestFinish() of PrmCommit-RQB !!!   +*/
/*+                                                                         +*/
/*+               Do the PrmCommitFinish only if:                           +*/
/*+                 - no PrmCommit is running currently                     +*/
/*+                 - PrmCommit finish is still requested                   +*/
/*+                                                                         +*/
/*+               Call this function only after PrmCommit confirmation      +*/
/*+                 -> after EDDP_GlbRequestFinish() of PrmCommit-RQB !!!   +*/
/*+                 -> EDDP_PRMDiagIndicateDiagAllAfterPrmCommit()          +*/
/*+                    is allowed after PrmCommit confirmation              +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMTriggerPrmCommitFinish(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT32      PortID;

    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_PRMTriggerPrmCommitFinish()");

    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMTriggerPrmCommitFinish(): isPrmCommitRunning=0x%X, isTriggerPrmCommitFinish=0x%X, SWI.IRTState=0x%X", 
        pDDB->PRM.isPrmCommitRunning, pDDB->PRM.isTriggerPrmCommitFinish, pDDB->SWI.IRTState);

    if ( (!(pDDB->PRM.isPrmCommitRunning)) && (pDDB->PRM.isTriggerPrmCommitFinish) )
    {
        /* -------------------------------------------------------------------- */
        /* Delete flag                                                          */
        /* -------------------------------------------------------------------- */
        pDDB->PRM.isTriggerPrmCommitFinish = LSA_FALSE;

        /* -------------------------------------------------------------------- */
        /* Indicate all empty diagnosis (needed by CM-PD)                       */
        /* -------------------------------------------------------------------- */
        EDDP_PRMDiagIndicateDiagAllAfterPrmCommit (pDDB);

        /* -------------------------------------------------------------------- */
        /* Trigger FrameDrop timer                                              */
        /* -------------------------------------------------------------------- */
        if (   (pDDB->PRM.FrameDrop.Budgets.ErrorDropBudget.Enabled) 
            || (pDDB->PRM.FrameDrop.Budgets.MaintenanceDemandedDropBudget.Enabled)
            || (pDDB->PRM.FrameDrop.Budgets.MaintenanceRequiredDropBudget.Enabled)
           )
        {   
            // only start timer if not already startet from previous PRM sequence
            if ( !(pDDB->PRM.Timer_isRunning) )
            {
                // start frame drop timer AFTER first (empty) indication
                EDDP_CoreTimerStart (pDDB->PRM.Timer, 1/*every second*/);
                pDDB->PRM.Timer_isRunning = LSA_TRUE;
            }
        }
        else
        {   
            // only stop timer if it is running
            if (pDDB->PRM.Timer_isRunning)
            {
                // stop frame drop timer because new parameter set does not need it
                EDDP_CoreTimerStop (pDDB->PRM.Timer);
            }
        }

        /* -------------------------------------------------------------------- */
        /* Trigger RTC3 port state machine                                      */
        /* -------------------------------------------------------------------- */
        if (pDDB->SWI.IRTState != 0)
        {
            for ( PortID=1; PortID <= pDDB->SWI.PortCnt; PortID++ )
            {
                EDDP_SWI_Trigger_RTC3_PortState (pDDB, PortID);
            }
        }
    }

    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMTriggerPrmCommitFinish()");
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMSetupTransferEnd                                +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                +*/
/*+                             EDDP_PRM_TRANSFER_END_CASE_TYPE     TransferEndCase     +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  TransferEndCase    : case of TransferEnd                                           +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function sets the parameter TransferEnd.                         +*/
/*+                                                                                     +*/
/*+               Store new TransferEnd in:                                             +*/
/*+                 - pDDB->PRM.TransferEndNs                                           +*/
/*+                 - in register ApplCompare_2_1.                                      +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMSetupTransferEnd(
    EDDP_LOCAL_DDB_PTR_TYPE             pDDB,
    EDDP_PRM_TRANSFER_END_CASE_TYPE     TransferEndCase)
{
    LSA_UINT32          SendClockNs;
    LSA_UINT32          TransferEndCorrectionValue;
    LSA_BOOL            isTransferEndCorrectionPos;

    pDDB->PRM.TransferEndNs     = 0;
    SendClockNs                 = (pDDB->SWI.CycleInfo.CycleBaseFactor * EDDP_CRT_CYCLE_LENGTH_BASE_NS);
    TransferEndCorrectionValue  = EDDP_CRTGetTransferEndCorrectionValue (pDDB);
    isTransferEndCorrectionPos  = EDDP_CRTIsTransferEndCorrectionPos (pDDB);

    EDDP_ASSERT_FALSE( !(EDDP_CRT_IS_TRANSFEREND_CORRECTION_VALUE_IN_VALID_RANGE(TransferEndCorrectionValue, isTransferEndCorrectionPos)) );

    /* ---------------------------------------- */
    /* Calculate "TransferEndNs"                */
    /* ---------------------------------------- */
    switch (TransferEndCase)
    {
        /* -------------------------------------------------------------------------------- */
        /* TransferEnd trigger is OFF														*/
        /* -------------------------------------------------------------------------------- */
        case EDDP_PRM_TRANSFER_END_CASE_OFF:
        {
            pDDB->PRM.TransferEndNs = EDDP_PRM_TRANSFER_END_TRIGGER_OFF_VALUE;
        }
        break;

        /* -------------------------------------------------------------------------------- */
        /* PDIRData with red phase                                                          */
        /* -------------------------------------------------------------------------------- */
        case EDDP_PRM_TRANSFER_END_CASE_PDIRDATA_RED:
        {
            // call from PrmCommit, so PDIRData_A must be present
            if ( !(EDDP_PRMPDIRDataIsPDIRDataPresent(pDDB, LSA_TRUE)) )
            {
                EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMSetupTransferEnd(TransferEndCase=0x%X): FATAL -> The record PDIRData(A) is not present!", 
                    TransferEndCase);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }

            pDDB->PRM.TransferEndNs = EDDP_PRMPDIRDataGetLocalTransferEndNs (pDDB, EDDP_PRM_LOCAL_TRANSFER_START_END_MODE_RX_END, LSA_TRUE);
        }
        break;

        /* -------------------------------------------------------------------------------- */
        /* no PDIRData and no PDSyncData (Default)                                          */
        /* -------------------------------------------------------------------------------- */
        case EDDP_PRM_TRANSFER_END_CASE_NO_PARAMETRIZATION:
        {
            pDDB->PRM.TransferEndNs = 0;
        }
        break;

        /* -------------------------------------------------------------------------------- */
        /* else                                                                             */
        /* -------------------------------------------------------------------------------- */
        default:
        {
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMSetupTransferEnd(TransferEndCase=0x%X): FATAL -> TransferEndCase is unknown!", 
                TransferEndCase);
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }

    /* ---------------------------------------- */
    /* TransferEndNs correction                 */
    /* only if TransferEnd trigger is not OFF   */
    /* ---------------------------------------- */
    if (pDDB->PRM.TransferEndNs != EDDP_PRM_TRANSFER_END_TRIGGER_OFF_VALUE)
    {
        if (isTransferEndCorrectionPos)
        {
            // TransferEndCorrectionValue is positive
            pDDB->PRM.TransferEndNs += TransferEndCorrectionValue;
            // patch TransferEndNs to maximum if it is greater than maximum
            if (pDDB->PRM.TransferEndNs > SendClockNs)
                pDDB->PRM.TransferEndNs = SendClockNs;
        }
        else
        {
            // TransferEndCorrectionValue is negative
            if (pDDB->PRM.TransferEndNs > TransferEndCorrectionValue)
                pDDB->PRM.TransferEndNs -= TransferEndCorrectionValue;
            else
                pDDB->PRM.TransferEndNs = 0;    // patch TransferEndNs to minimum because it is smaller than minimum
        }
    }

    EDDP_PRM_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMSetupTransferEnd(TransferEndCase=0x%X): TransferEndNs=%d, TransferEndCorrectionValue=%d, isTransferEndCorrectionPos=%d, SendClockNs=%d", 
        TransferEndCase, pDDB->PRM.TransferEndNs, TransferEndCorrectionValue, isTransferEndCorrectionPos, SendClockNs);

    /* ---------------------------------------- */
    /* Write "TransferEndNs" to register        */
    /* ---------------------------------------- */
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_APPLCOMPARE_2_1, pDDB->PRM.TransferEndNs);
}

/*****************************************************************************/
/*  end of file eddp_prm.c                                                   */
/*****************************************************************************/
