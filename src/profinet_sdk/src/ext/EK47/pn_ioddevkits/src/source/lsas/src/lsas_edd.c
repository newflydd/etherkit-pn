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
/*  C o m p o n e n t     &C: PnIODDevkits                              :C&  */
/*                                                                           */
/*  F i l e               &F: lsas_edd.c                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/


/**
 * @file    lsas_edd.c
 * @brief   brief description
 * @author  cn3dit09
 * @version
 * @date    27.02.2012
 *
 * long description
 *
 */

/*****************************************************************************/

#define LSAS_MODULE_ID  2

#include "lsas_inc.h"

#include "lsas_int.h"
#include "lsas_dat.h"



//=================================================================================================
LSA_VOID lsas_cfg_edd_system_req_check(LSA_VOID)
{
    // if this funktion is called it starts a task loop over the mailbox that breaks when
    // the request_processed flag is set

    LSA_RESULT ret_val;

    if (lsas_data.edd.request_processed == 0)
    {
        if (lsas_data.edd.loop_rqb_ptr == 0)
        {
            //first call, alloc rqb mem
            LSAS_ALLOC_LOCAL_MEM((LSAS_LOCAL_MEM_PTR_TYPE)&lsas_data.edd.loop_rqb_ptr, sizeof(LSAS_RQB_TYPE) );
        }

        OsWait_ms(1);

        LSAS_RQB_EDD_REQUEST(lsas_data.edd.loop_rqb_ptr, lsas_cfg_edd_system_req_check); // call this funktion again at next mailcheck
    }
    else
    {
        if (lsas_data.edd.loop_rqb_ptr != 0)
        {
            LSAS_FREE_LOCAL_MEM(&ret_val, lsas_data.edd.loop_rqb_ptr);
            if( ret_val != LSA_RET_OK )
            {
                LSAS_FATAL(LSAS_FATAL_ERR_MEM_FREE);
            }
            lsas_data.edd.loop_rqb_ptr = 0;
        }
    }
}



#ifdef LSAS_CFG_USE_EDDI

void lsas_edd_system_setup_gis(EDDI_UPPER_GLOBAL_INI_PTR_TYPE pGIS, LSA_UINT32 ProductID)
{
    LSA_UINT32  SubIndex = 0;
    LSA_UINT32  ErtecTyp = 0;

    /*
    Array 1: ERTEC Typ
    Array 2: ProductTyp
    Array 3: Values
    */

    static LSA_UINT32 const ProductGISValues[4][6][40] =
    {
        {/*Unknown*/
            /*Default*/                    {                         0/*HWTypeUsed*/, EDDI_HW_SUBTYPE_USED_NONE/*HWSubTypeUsed*/, 4/*NrOfPorts*/, EDD_FEATURE_ENABLE/*bIRTSupported*/,   0/*NrOfIRTForwarders*/, EDD_MRP_ROLE_CAP_MANAGER/*MRPDefaultRoleInstance0*/, 1/*MRPDefaultRingPort1*/, 2/*MRPDefaultRingPort2*/,      EDDI_HSYNC_ROLE_NONE/*HSYNCRole*/, EDDI_GIS_USECASE_DEFAULT/*BufferCapacityUseCase*/,        EDDI_I2C_TYPE_NONE/*I2C_Type*/, 256 /*NrOfRTDevices*/, 64/*NrOfIRTDevices*/, 64/*NrOfRedIRTDevices*/, 1/*NrOfQVProviders*/, 16/*NrOfQVConsumers*/, 16/*NrOfRedQVConsumers*/, 8192 /*LogicalAddressSpace*/,  8192/*NrOfSubmod*/,    0/*MaxIOCRDataSize*/, 256/*MaxMCRDataSize*/, 1/*NrOfInstances*/, 2/*NrOfARs*/, 720/*MaxOCRDataSize*/, 720/*MaxICRDataSize*/, 256/*MaxMCRDataSize*/, EDD_FEATURE_DISABLE/*AlternativeCalc*/, 0/*NrOfSubmodules*/, 0/*OutputBytes*/, 0/*InputBytes*/, EDD_FEATURE_DISABLE/*SharedARSupported*/, EDD_FEATURE_DISABLE/*bMIIUsed*/, 0/*InterfaceID*/, 0 /*InitializeTheRest*/}
        },
        {/*E400*/
            /*Default*/                    {EDD_HW_TYPE_USED_ERTEC_400/*HWTypeUsed*/, EDDI_HW_SUBTYPE_USED_NONE/*HWSubTypeUsed*/, 4/*NrOfPorts*/, EDD_FEATURE_ENABLE/*bIRTSupported*/,  64/*NrOfIRTForwarders*/, EDD_MRP_ROLE_CAP_MANAGER/*MRPDefaultRoleInstance0*/, 1/*MRPDefaultRingPort1*/, 2/*MRPDefaultRingPort2*/,      EDDI_HSYNC_ROLE_NONE/*HSYNCRole*/, EDDI_GIS_USECASE_DEFAULT/*BufferCapacityUseCase*/,        EDDI_I2C_TYPE_GPIO/*I2C_Type*/,  96 /*NrOfRTDevices*/, 64/*NrOfIRTDevices*/, 64/*NrOfRedIRTDevices*/, 1/*NrOfQVProviders*/, 16/*NrOfQVConsumers*/, 16/*NrOfRedQVConsumers*/, 8192 /*LogicalAddressSpace*/,  4096/*NrOfSubmod*/,  720/*MaxIOCRDataSize*/, 256/*MaxMCRDataSize*/, 1/*NrOfInstances*/, 2/*NrOfARs*/, 720/*MaxOCRDataSize*/, 720/*MaxICRDataSize*/, 256/*MaxMCRDataSize*/, EDD_FEATURE_DISABLE/*AlternativeCalc*/, 0/*NrOfSubmodules*/, 0/*OutputBytes*/, 0/*InputBytes*/, EDD_FEATURE_DISABLE/*SharedARSupported*/, EDD_FEATURE_DISABLE/*bMIIUsed*/, 0/*InterfaceID*/, 0 /*InitializeTheRest*/},
            /*CPU 317-2PN with ERTEC400*/  {EDD_HW_TYPE_USED_ERTEC_400/*HWTypeUsed*/, EDDI_HW_SUBTYPE_USED_NONE/*HWSubTypeUsed*/, 2/*NrOfPorts*/, EDD_FEATURE_ENABLE/*bIRTSupported*/, 128/*NrOfIRTForwarders*/, EDD_MRP_ROLE_CAP_MANAGER/*MRPDefaultRoleInstance0*/, 1/*MRPDefaultRingPort1*/, 2/*MRPDefaultRingPort2*/,      EDDI_HSYNC_ROLE_NONE/*HSYNCRole*/, EDDI_GIS_USECASE_DEFAULT/*BufferCapacityUseCase*/,        EDDI_I2C_TYPE_GPIO/*I2C_Type*/, 128 /*NrOfRTDevices*/, 64/*NrOfIRTDevices*/, 64/*NrOfRedIRTDevices*/, 1/*NrOfQVProviders*/, 16/*NrOfQVConsumers*/, 16/*NrOfRedQVConsumers*/, 8192 /*LogicalAddressSpace*/,  8192/*NrOfSubmod*/, 1440/*MaxIOCRDataSize*/, 256/*MaxMCRDataSize*/, 1/*NrOfInstances*/, 2/*NrOfARs*/, 720/*MaxOCRDataSize*/, 720/*MaxICRDataSize*/, 256/*MaxMCRDataSize*/, EDD_FEATURE_DISABLE/*AlternativeCalc*/, 0/*NrOfSubmodules*/, 0/*OutputBytes*/, 0/*InputBytes*/, EDD_FEATURE_DISABLE/*SharedARSupported*/, EDD_FEATURE_DISABLE/*bMIIUsed*/, 0/*InterfaceID*/, 0 /*InitializeTheRest*/},
            /*CPU 319-2PN with ERTEC400*/  {EDD_HW_TYPE_USED_ERTEC_400/*HWTypeUsed*/, EDDI_HW_SUBTYPE_USED_NONE/*HWSubTypeUsed*/, 2/*NrOfPorts*/, EDD_FEATURE_ENABLE/*bIRTSupported*/, 128/*NrOfIRTForwarders*/, EDD_MRP_ROLE_CAP_MANAGER/*MRPDefaultRoleInstance0*/, 1/*MRPDefaultRingPort1*/, 2/*MRPDefaultRingPort2*/,      EDDI_HSYNC_ROLE_NONE/*HSYNCRole*/, EDDI_GIS_USECASE_DEFAULT/*BufferCapacityUseCase*/,        EDDI_I2C_TYPE_GPIO/*I2C_Type*/, 128 /*NrOfRTDevices*/, 64/*NrOfIRTDevices*/, 64/*NrOfRedIRTDevices*/, 1/*NrOfQVProviders*/, 16/*NrOfQVConsumers*/, 16/*NrOfRedQVConsumers*/, 8192 /*LogicalAddressSpace*/,  8192/*NrOfSubmod*/, 1440/*MaxIOCRDataSize*/, 256/*MaxMCRDataSize*/, 1/*NrOfInstances*/, 2/*NrOfARs*/, 720/*MaxOCRDataSize*/, 720/*MaxICRDataSize*/, 256/*MaxMCRDataSize*/, EDD_FEATURE_DISABLE/*AlternativeCalc*/, 0/*NrOfSubmodules*/, 0/*OutputBytes*/, 0/*InputBytes*/, EDD_FEATURE_DISABLE/*SharedARSupported*/, EDD_FEATURE_DISABLE/*bMIIUsed*/, 0/*InterfaceID*/, 0 /*InitializeTheRest*/}
        },
        {/*E200*/
            /*Default*/                    {EDD_HW_TYPE_USED_ERTEC_200/*HWTypeUsed*/, EDDI_HW_SUBTYPE_USED_NONE/*HWSubTypeUsed*/, 2/*NrOfPorts*/, EDD_FEATURE_ENABLE/*bIRTSupported*/,  64/*NrOfIRTForwarders*/, EDD_MRP_ROLE_CAP_CLIENT /*MRPDefaultRoleInstance0*/, 1/*MRPDefaultRingPort1*/, 2/*MRPDefaultRingPort2*/,      EDDI_HSYNC_ROLE_NONE/*HSYNCRole*/, EDDI_GIS_USECASE_DEFAULT/*BufferCapacityUseCase*/,        EDDI_I2C_TYPE_NONE/*I2C_Type*/, 0   /*NrOfRTDevices*/, 0 /*NrOfIRTDevices*/, 0 /*NrOfRedIRTDevices*/, 0/*NrOfQVProviders*/,  0/*NrOfQVConsumers*/,  0/*NrOfRedQVConsumers*/,    0 /*LogicalAddressSpace*/,     0/*NrOfSubmod*/,    0/*MaxIOCRDataSize*/,   0/*MaxMCRDataSize*/, 1/*NrOfInstances*/, 2/*NrOfARs*/, 248/*MaxOCRDataSize*/, 248/*MaxICRDataSize*/,  64/*MaxMCRDataSize*/, EDD_FEATURE_DISABLE/*AlternativeCalc*/, 0/*NrOfSubmodules*/, 0/*OutputBytes*/, 0/*InputBytes*/, EDD_FEATURE_DISABLE/*SharedARSupported*/,  EDD_FEATURE_ENABLE/*bMIIUsed*/, 0/*InterfaceID*/, 0 /*InitializeTheRest*/},
            /*ET 200M IM153-4*/            {EDD_HW_TYPE_USED_ERTEC_200/*HWTypeUsed*/, EDDI_HW_SUBTYPE_USED_NONE/*HWSubTypeUsed*/, 2/*NrOfPorts*/, EDD_FEATURE_ENABLE/*bIRTSupported*/, 128/*NrOfIRTForwarders*/, EDD_MRP_ROLE_CAP_CLIENT /*MRPDefaultRoleInstance0*/, 1/*MRPDefaultRingPort1*/, 2/*MRPDefaultRingPort2*/,      EDDI_HSYNC_ROLE_NONE/*HSYNCRole*/, EDDI_GIS_USECASE_DEFAULT/*BufferCapacityUseCase*/,        EDDI_I2C_TYPE_NONE/*I2C_Type*/, 0   /*NrOfRTDevices*/, 0 /*NrOfIRTDevices*/, 0 /*NrOfRedIRTDevices*/, 0/*NrOfQVProviders*/,  0/*NrOfQVConsumers*/,  0/*NrOfRedQVConsumers*/,    0 /*LogicalAddressSpace*/,     0/*NrOfSubmod*/,    0/*MaxIOCRDataSize*/,   0/*MaxMCRDataSize*/, 1/*NrOfInstances*/, 2/*NrOfARs*/, 220/*MaxOCRDataSize*/, 220/*MaxICRDataSize*/,  64/*MaxMCRDataSize*/, EDD_FEATURE_DISABLE/*AlternativeCalc*/, 0/*NrOfSubmodules*/, 0/*OutputBytes*/, 0/*InputBytes*/, EDD_FEATURE_DISABLE/*SharedARSupported*/,  EDD_FEATURE_ENABLE/*bMIIUsed*/, 0/*InterfaceID*/, 0 /*InitializeTheRest*/},
            /*PN/PN Coupler 158-3AD00*/    {EDD_HW_TYPE_USED_ERTEC_200/*HWTypeUsed*/, EDDI_HW_SUBTYPE_USED_NONE/*HWSubTypeUsed*/, 2/*NrOfPorts*/, EDD_FEATURE_ENABLE/*bIRTSupported*/, 128/*NrOfIRTForwarders*/, EDD_MRP_ROLE_CAP_CLIENT /*MRPDefaultRoleInstance0*/, 1/*MRPDefaultRingPort1*/, 2/*MRPDefaultRingPort2*/,      EDDI_HSYNC_ROLE_NONE/*HSYNCRole*/, EDDI_GIS_USECASE_DEFAULT/*BufferCapacityUseCase*/,        EDDI_I2C_TYPE_NONE/*I2C_Type*/, 0   /*NrOfRTDevices*/, 0 /*NrOfIRTDevices*/, 0 /*NrOfRedIRTDevices*/, 0/*NrOfQVProviders*/,  0/*NrOfQVConsumers*/,  0/*NrOfRedQVConsumers*/,    0 /*LogicalAddressSpace*/,     0/*NrOfSubmod*/,    0/*MaxIOCRDataSize*/,   0/*MaxMCRDataSize*/, 1/*NrOfInstances*/, 2/*NrOfARs*/, 436/*MaxOCRDataSize*/, 436/*MaxICRDataSize*/,  64/*MaxMCRDataSize*/, EDD_FEATURE_DISABLE/*AlternativeCalc*/, 0/*NrOfSubmodules*/, 0/*OutputBytes*/, 0/*InputBytes*/, EDD_FEATURE_DISABLE/*SharedARSupported*/,  EDD_FEATURE_ENABLE/*bMIIUsed*/, 0/*InterfaceID*/, 0 /*InitializeTheRest*/}
        },
        {/*SOC1*/
            /*Default*/                    {      EDD_HW_TYPE_USED_SOC/*HWTypeUsed*/, EDDI_HW_SUBTYPE_USED_SOC1/*HWSubTypeUsed*/, 3/*NrOfPorts*/, EDD_FEATURE_ENABLE/*bIRTSupported*/, 128/*NrOfIRTForwarders*/, EDD_MRP_ROLE_CAP_MANAGER/*MRPDefaultRoleInstance0*/, 1/*MRPDefaultRingPort1*/, 2/*MRPDefaultRingPort2*/, EDDI_HSYNC_ROLE_FORWARDER/*HSYNCRole*/, EDDI_GIS_USECASE_IOC_SOC1_3P/*BufferCapacityUseCase*/, EDDI_I2C_TYPE_SOC1_HW/*I2C_Type*/, 256 /*NrOfRTDevices*/, 64/*NrOfIRTDevices*/, 64/*NrOfRedIRTDevices*/, 1/*NrOfQVProviders*/, 16/*NrOfQVConsumers*/, 16/*NrOfRedQVConsumers*/, 8192 /*LogicalAddressSpace*/,  8192/*NrOfSubmod*/, 1440/*MaxIOCRDataSize*/, 256/*MaxMCRDataSize*/, 1/*NrOfInstances*/, 2/*NrOfARs*/, 720/*MaxOCRDataSize*/, 720/*MaxICRDataSize*/, 256/*MaxMCRDataSize*/, EDD_FEATURE_DISABLE/*AlternativeCalc*/, 0/*NrOfSubmodules*/, 0/*OutputBytes*/, 0/*InputBytes*/, EDD_FEATURE_DISABLE/*SharedARSupported*/,  EDD_FEATURE_ENABLE/*bMIIUsed*/, 0/*InterfaceID*/, 0 /*InitializeTheRest*/},
            /*CPU 317-2PN with SOC1*/      {      EDD_HW_TYPE_USED_SOC/*HWTypeUsed*/, EDDI_HW_SUBTYPE_USED_SOC1/*HWSubTypeUsed*/, 2/*NrOfPorts*/, EDD_FEATURE_ENABLE/*bIRTSupported*/, 128/*NrOfIRTForwarders*/, EDD_MRP_ROLE_CAP_MANAGER/*MRPDefaultRoleInstance0*/, 1/*MRPDefaultRingPort1*/, 2/*MRPDefaultRingPort2*/, EDDI_HSYNC_ROLE_FORWARDER/*HSYNCRole*/, EDDI_GIS_USECASE_IOC_SOC1_2P/*BufferCapacityUseCase*/, EDDI_I2C_TYPE_SOC1_HW/*I2C_Type*/, 128 /*NrOfRTDevices*/, 64/*NrOfIRTDevices*/, 64/*NrOfRedIRTDevices*/, 1/*NrOfQVProviders*/, 16/*NrOfQVConsumers*/, 16/*NrOfRedQVConsumers*/, 8192 /*LogicalAddressSpace*/,  8192/*NrOfSubmod*/, 1440/*MaxIOCRDataSize*/, 256/*MaxMCRDataSize*/, 1/*NrOfInstances*/, 4/*NrOfARs*/, 720/*MaxOCRDataSize*/, 720/*MaxICRDataSize*/, 256/*MaxMCRDataSize*/, EDD_FEATURE_DISABLE/*AlternativeCalc*/, 0/*NrOfSubmodules*/, 0/*OutputBytes*/, 0/*InputBytes*/, EDD_FEATURE_DISABLE/*SharedARSupported*/,  EDD_FEATURE_ENABLE/*bMIIUsed*/, 0/*InterfaceID*/, 0 /*InitializeTheRest*/},
            /*CPU 319-2PN with SOC1*/      {      EDD_HW_TYPE_USED_SOC/*HWTypeUsed*/, EDDI_HW_SUBTYPE_USED_SOC1/*HWSubTypeUsed*/, 2/*NrOfPorts*/, EDD_FEATURE_ENABLE/*bIRTSupported*/, 128/*NrOfIRTForwarders*/, EDD_MRP_ROLE_CAP_MANAGER/*MRPDefaultRoleInstance0*/, 1/*MRPDefaultRingPort1*/, 2/*MRPDefaultRingPort2*/, EDDI_HSYNC_ROLE_FORWARDER/*HSYNCRole*/, EDDI_GIS_USECASE_IOC_SOC1_2P/*BufferCapacityUseCase*/, EDDI_I2C_TYPE_SOC1_HW/*I2C_Type*/, 256 /*NrOfRTDevices*/, 64/*NrOfIRTDevices*/, 64/*NrOfRedIRTDevices*/, 1/*NrOfQVProviders*/, 16/*NrOfQVConsumers*/, 16/*NrOfRedQVConsumers*/, 8192 /*LogicalAddressSpace*/,  8192/*NrOfSubmod*/, 1440/*MaxIOCRDataSize*/, 256/*MaxMCRDataSize*/, 1/*NrOfInstances*/, 4/*NrOfARs*/, 720/*MaxOCRDataSize*/, 720/*MaxICRDataSize*/, 256/*MaxMCRDataSize*/, EDD_FEATURE_DISABLE/*AlternativeCalc*/, 0/*NrOfSubmodules*/, 0/*OutputBytes*/, 0/*InputBytes*/, EDD_FEATURE_DISABLE/*SharedARSupported*/,  EDD_FEATURE_ENABLE/*bMIIUsed*/, 0/*InterfaceID*/, 0 /*InitializeTheRest*/},
            /*CPU 515 / 516*/              {      EDD_HW_TYPE_USED_SOC/*HWTypeUsed*/, EDDI_HW_SUBTYPE_USED_SOC1/*HWSubTypeUsed*/, 3/*NrOfPorts*/, EDD_FEATURE_ENABLE/*bIRTSupported*/, 128/*NrOfIRTForwarders*/, EDD_MRP_ROLE_CAP_MANAGER/*MRPDefaultRoleInstance0*/, 1/*MRPDefaultRingPort1*/, 2/*MRPDefaultRingPort2*/, EDDI_HSYNC_ROLE_FORWARDER/*HSYNCRole*/, EDDI_GIS_USECASE_IOC_SOC1_3P/*BufferCapacityUseCase*/, EDDI_I2C_TYPE_SOC1_HW/*I2C_Type*/, 2048/*NrOfRTDevices*/, 64/*NrOfIRTDevices*/, 64/*NrOfRedIRTDevices*/, 1/*NrOfQVProviders*/, 16/*NrOfQVConsumers*/, 16/*NrOfRedQVConsumers*/, 32768/*LogicalAddressSpace*/, 16384/*NrOfSubmod*/, 1440/*MaxIOCRDataSize*/, 256/*MaxMCRDataSize*/, 1/*NrOfInstances*/, 4/*NrOfARs*/, 720/*MaxOCRDataSize*/, 720/*MaxICRDataSize*/, 256/*MaxMCRDataSize*/, EDD_FEATURE_DISABLE/*AlternativeCalc*/, 0/*NrOfSubmodules*/, 0/*OutputBytes*/, 0/*InputBytes*/, EDD_FEATURE_DISABLE/*SharedARSupported*/,  EDD_FEATURE_ENABLE/*bMIIUsed*/, 0/*InterfaceID*/, 0 /*InitializeTheRest*/},
            /*SOC1 PCIe card*/             {      EDD_HW_TYPE_USED_SOC/*HWTypeUsed*/, EDDI_HW_SUBTYPE_USED_SOC1/*HWSubTypeUsed*/, 3/*NrOfPorts*/, EDD_FEATURE_ENABLE/*bIRTSupported*/, 128/*NrOfIRTForwarders*/, EDD_MRP_ROLE_CAP_MANAGER/*MRPDefaultRoleInstance0*/, 1/*MRPDefaultRingPort1*/, 2/*MRPDefaultRingPort2*/, EDDI_HSYNC_ROLE_FORWARDER/*HSYNCRole*/, EDDI_GIS_USECASE_IOC_SOC1_3P/*BufferCapacityUseCase*/, EDDI_I2C_TYPE_SOC1_HW/*I2C_Type*/, 2048/*NrOfRTDevices*/, 64/*NrOfIRTDevices*/, 64/*NrOfRedIRTDevices*/, 1/*NrOfQVProviders*/, 16/*NrOfQVConsumers*/, 16/*NrOfRedQVConsumers*/, 32768/*LogicalAddressSpace*/, 16384/*NrOfSubmod*/, 1440/*MaxIOCRDataSize*/, 256/*MaxMCRDataSize*/, 1/*NrOfInstances*/, 4/*NrOfARs*/, 720/*MaxOCRDataSize*/, 720/*MaxICRDataSize*/, 256/*MaxMCRDataSize*/, EDD_FEATURE_DISABLE/*AlternativeCalc*/, 0/*NrOfSubmodules*/, 0/*OutputBytes*/, 0/*InputBytes*/, EDD_FEATURE_DISABLE/*SharedARSupported*/,  EDD_FEATURE_ENABLE/*bMIIUsed*/, 0/*InterfaceID*/, 0 /*InitializeTheRest*/},
            /*ET 200SP IM155-6PN/3 HF*/    {      EDD_HW_TYPE_USED_SOC/*HWTypeUsed*/, EDDI_HW_SUBTYPE_USED_SOC1/*HWSubTypeUsed*/, 3/*NrOfPorts*/, EDD_FEATURE_ENABLE/*bIRTSupported*/, 128/*NrOfIRTForwarders*/, EDD_MRP_ROLE_CAP_CLIENT /*MRPDefaultRoleInstance0*/, 1/*MRPDefaultRingPort1*/, 2/*MRPDefaultRingPort2*/, EDDI_HSYNC_ROLE_FORWARDER/*HSYNCRole*/, EDDI_GIS_USECASE_IOC_SOC1_3P/*BufferCapacityUseCase*/, EDDI_I2C_TYPE_SOC1_HW/*I2C_Type*/, 0   /*NrOfRTDevices*/, 0 /*NrOfIRTDevices*/, 0 /*NrOfRedIRTDevices*/, 0/*NrOfQVProviders*/, 0 /*NrOfQVConsumers*/, 0 /*NrOfRedQVConsumers*/,    0 /*LogicalAddressSpace*/,     0/*NrOfSubmod*/,    0/*MaxIOCRDataSize*/,   0/*MaxMCRDataSize*/, 1/*NrOfInstances*/, 4/*NrOfARs*/, 500/*MaxOCRDataSize*/, 500/*MaxICRDataSize*/, 256/*MaxMCRDataSize*/, EDD_FEATURE_DISABLE/*AlternativeCalc*/, 0/*NrOfSubmodules*/, 0/*OutputBytes*/, 0/*InputBytes*/, EDD_FEATURE_DISABLE/*SharedARSupported*/,  EDD_FEATURE_ENABLE/*bMIIUsed*/, 0/*InterfaceID*/, 0 /*InitializeTheRest*/}
        }
    };

    #if defined (EDDI_CFG_ERTEC_400)
    ErtecTyp = EDD_HW_TYPE_USED_ERTEC_400;
    #elif defined (EDDI_CFG_ERTEC_200)
    ErtecTyp = EDD_HW_TYPE_USED_ERTEC_200;
    #elif defined (EDDI_CFG_SOC)
    ErtecTyp = EDD_HW_TYPE_USED_SOC;
    #endif

    pGIS->GenIni.HWTypeUsed              =             ProductGISValues[ErtecTyp][ProductID][SubIndex++];
    pGIS->GenIni.HWSubTypeUsed           = (LSA_UINT8) ProductGISValues[ErtecTyp][ProductID][SubIndex++];

    pGIS->GenIni.NrOfPorts               = (LSA_UINT8) ProductGISValues[ErtecTyp][ProductID][SubIndex++];
    pGIS->GenIni.bIRTSupported           = (LSA_UINT8) ProductGISValues[ErtecTyp][ProductID][SubIndex++];
    pGIS->GenIni.NrOfIRTForwarders       = (LSA_UINT16)ProductGISValues[ErtecTyp][ProductID][SubIndex++];

    pGIS->GenIni.MRPDefaultRoleInstance0 = (LSA_UINT16)ProductGISValues[ErtecTyp][ProductID][SubIndex++];
    pGIS->GenIni.MRPDefaultRingPort1     = (LSA_UINT8) ProductGISValues[ErtecTyp][ProductID][SubIndex++];
    pGIS->GenIni.MRPDefaultRingPort2     = (LSA_UINT8) ProductGISValues[ErtecTyp][ProductID][SubIndex++];
    pGIS->GenIni.HSYNCRole               = (LSA_UINT8) ProductGISValues[ErtecTyp][ProductID][SubIndex++];
    pGIS->GenIni.BufferCapacityUseCase   = (LSA_UINT16)ProductGISValues[ErtecTyp][ProductID][SubIndex++];
    pGIS->GenIni.I2C_Type                = (LSA_UINT32)ProductGISValues[ErtecTyp][ProductID][SubIndex++];

    pGIS->IOCIni.NrOfRTDevices           = (LSA_UINT16)ProductGISValues[ErtecTyp][ProductID][SubIndex++];
    pGIS->IOCIni.NrOfIRTDevices          = (LSA_UINT16)ProductGISValues[ErtecTyp][ProductID][SubIndex++];
    pGIS->IOCIni.NrOfRedIRTDevices       = (LSA_UINT16)ProductGISValues[ErtecTyp][ProductID][SubIndex++];
    pGIS->IOCIni.NrOfQVProviders         = (LSA_UINT16)ProductGISValues[ErtecTyp][ProductID][SubIndex++];
    pGIS->IOCIni.NrOfQVConsumers         = (LSA_UINT16)ProductGISValues[ErtecTyp][ProductID][SubIndex++];
    pGIS->IOCIni.NrOfRedQVConsumers      = (LSA_UINT16)ProductGISValues[ErtecTyp][ProductID][SubIndex++];
    pGIS->IOCIni.LogicalAddressSpace     = ProductGISValues[ErtecTyp][ProductID][SubIndex++];
    pGIS->IOCIni.NrOfSubmod              = ProductGISValues[ErtecTyp][ProductID][SubIndex++];
    pGIS->IOCIni.MaxIOCRDataSize         = (LSA_UINT16)ProductGISValues[ErtecTyp][ProductID][SubIndex++];
    pGIS->IOCIni.MaxMCRDataSize          = (LSA_UINT16)ProductGISValues[ErtecTyp][ProductID][SubIndex++];

    pGIS->IODIni.NrOfInstances           = (LSA_UINT16)ProductGISValues[ErtecTyp][ProductID][SubIndex++];
    pGIS->IODIni.NrOfARs                 = (LSA_UINT16)ProductGISValues[ErtecTyp][ProductID][SubIndex++];
    pGIS->IODIni.MaxOCRDataSize          = (LSA_UINT16)ProductGISValues[ErtecTyp][ProductID][SubIndex++];
    pGIS->IODIni.MaxICRDataSize          = (LSA_UINT16)ProductGISValues[ErtecTyp][ProductID][SubIndex++];
    pGIS->IODIni.MaxMCRDataSize          = (LSA_UINT16)ProductGISValues[ErtecTyp][ProductID][SubIndex++];

    pGIS->IODIni.AlternativeCalc         = (LSA_UINT8) ProductGISValues[ErtecTyp][ProductID][SubIndex++];
    pGIS->IODIni.NrOfSubmodules          = (LSA_UINT16)ProductGISValues[ErtecTyp][ProductID][SubIndex++];
    pGIS->IODIni.OutputBytes             = (LSA_UINT16)ProductGISValues[ErtecTyp][ProductID][SubIndex++];
    pGIS->IODIni.InputBytes              = (LSA_UINT16)ProductGISValues[ErtecTyp][ProductID][SubIndex++];
    pGIS->IODIni.SharedARSupported       = (LSA_UINT16)ProductGISValues[ErtecTyp][ProductID][SubIndex++];

    pGIS->NRTIni.bMIIUsed                = (LSA_UINT8) ProductGISValues[ErtecTyp][ProductID][SubIndex++];

    pGIS->InterfaceID                    = ProductGISValues[ErtecTyp][ProductID][SubIndex++];
}

/*****************************************************************************/

void    lsas_edd_system_dev_open    (EDD_RQB_TYPE * rqb_ptr, EDDI_DPB_TYPE * dpb_ptr)
{
    Unsigned16              i;
    EDDI_RQB_DDB_INI_TYPE  *ini;

    /* fill out device parameter block */
    dpb_ptr->InterfaceID                  = LSAS_CFG_INTERFACE_ID;

    dpb_ptr->IRTE_SWI_BaseAdr             = LSAS_CFG_IRTE_START;
    #if defined (EDDI_CFG_SOC)
    dpb_ptr->NRTMEM_LowerLimit            = 0x40E00000UL;                   /* Lower Limit of NRT-Memory (virt.) */
    dpb_ptr->NRTMEM_UpperLimit            = 0x40E00000UL + 0x00500000UL;    /* Upper Limit of NRT-Memory (virt.) */
    #else
    dpb_ptr->NRTMEM_LowerLimit            = 0;             /* Lower Limit of NRT-Memory (virt.) */
    dpb_ptr->NRTMEM_UpperLimit            = 0xFFFFFFFFUL;  /* Upper Limit of NRT-Memory (virt.) */
    #endif

    dpb_ptr->IRTE_SWI_BaseAdr_LBU_16Bit   = 0x0;
    dpb_ptr->BOARD_SDRAM_BaseAddr         = LSAS_CFG_SDRAM_BASE_ADDR;

    //allocate shared memory for 2proc mem
    #if !defined (EDDI_CFG_3BIF_2PROC)
    dpb_ptr->GSharedRAM_BaseAdr           = (LSA_UINT32) &lsas_data.eddi.gshared_mem;
    #endif

    #if defined (EDDI_CFG_SOC)
    dpb_ptr->ERTECx00_SCRB_BaseAddr       = 0 /*not used for SOC*/;
    #else
    dpb_ptr->ERTECx00_SCRB_BaseAddr       = 0x40002600;
    #endif

    dpb_ptr->EnableReset                  = EDD_FEATURE_ENABLE;
     /* for products that have already been delivered with old/too small port delay values in the engineering system,
      * the port delay check should be disabled in order to be backward compatible. */
    dpb_ptr->bDisableMaxPortDelayCheck    = LSAS_IS_MAX_PORT_DELAY_CHECK_DISABLED;
     /*!< Base address of the 144 kByte PAEA-RAM (SOC1/2).
       Setting an address enables IO-Control and moves the process image to the PAEA-RAM.
       In the case of using IOC, the variable edd_dpb.KRam.offset_ProcessImageEnd has to be NULL. */
    #if defined (EDDI_CFG_SOC)
    dpb_ptr->PAEA_BaseAdr = U_IOCC_PA_EA_DIRECT_start;
    #else
    dpb_ptr->PAEA_BaseAdr = 0x0;          /* only for SOC Devices*/
    #endif


    //SII configuration parameters
    dpb_ptr->SII_IrqSelector       = EDDI_SII_IRQ_SP;          //EDDI_SII_IRQ_SP or EDDI_SII_IRQ_HP
    dpb_ptr->SII_IrqNumber         = EDDI_SII_IRQ_1;           //EDDI_SII_IRQ_0  or EDDI_SII_IRQ_1
    #if defined (EDDI_CFG_SII_EXTTIMER_MODE_ON) || defined (EDDI_CFG_SII_EXTTIMER_MODE_FLEX)
    dpb_ptr->SII_ExtTimerInterval  = 500UL;    //in us         //0, 250, 500, 1000 in us
    #endif
    #if defined (EDDI_CFG_SII_FLEX_MODE)
    dpb_ptr->SII_Mode              = EDDI_SII_STANDARD_MODE;   //EDDI_SII_STANDARD_MODE or EDDI_SII_POLLING_MODE
    #endif

    dpb_ptr->I2C_Type              = LSAS_EDDI_I2C_TYPE;

    dpb_ptr->KRam.offset_ProcessImageEnd  = LSAS_PROCESS_IMAGE_SIZE;

#if defined EDDI_CFG_SOC
    dpb_ptr->KRam.size_reserved_for_eddi  = EDDI_KRAM_SIZE_SOC - LSAS_PROCESS_IMAGE_SIZE;
#elif defined EDDI_CFG_ERTEC_200
    dpb_ptr->KRam.size_reserved_for_eddi  = EDDI_KRAM_SIZE_ERTEC200 - LSAS_PROCESS_IMAGE_SIZE;
#else //E400
    dpb_ptr->KRam.size_reserved_for_eddi  = EDDI_KRAM_SIZE_ERTEC400 - LSAS_PROCESS_IMAGE_SIZE;
#endif

    // Deactivate the ERTEC 400 boot loader for the following use case:
    // -> no firmware (EDD on host processor), module is used as PCI slave
    dpb_ptr->Bootloader.Activate_for_use_as_pci_slave  = EDD_FEATURE_ENABLE;

    /* settings for SNMP data base (device specific, projectable features)*/
    #ifdef LSAS_PU_FEATURE_MRPD_SUPPORT
    dpb_ptr->FeatureSupport.bMRPDSupported             = TRUE;          /* only possible for SOC/Ertec400*/
    #endif
    /* FeatureSupport.FragmentationtypeSupported: dynamic frame packet not possible for IRTE, only SOC would be able to support STATIC fragmentation */
    #ifdef LSAS_PU_FEATURE_FRAGTYPE_STATIC_SUPPORTED
    dpb_ptr->FeatureSupport.FragmentationtypeSupported |= EDD_DPB_FEATURE_FRAGTYPE_SUPPORTED_STATIC;    /* only possible for SOC*/
    #endif
    #ifdef LSAS_PU_FEATURE_ABSOLUTE_FWD_SUPPORTED
    dpb_ptr->FeatureSupport.IRTForwardingModeSupported |= EDD_DPB_FEATURE_IRTFWDMODE_SUPPORTED_ABSOLUTE;
    #endif
    /* FeatureSupport.IRTForwardingModeSupported: only absolute forwarding allowed for IRTE, no relative */
    #ifdef LSAS_PU_FEATURE_DFP_SUPPORTED
    dpb_ptr->FeatureSupport.MaxDFPFrames               = 8;             /* only possible on SOC */
    #endif

    //dpb_ptr->FeatureSupport.AdditionalForwardingRulesSupported  // E200 / E400 don't support HSYNC at all (not even as a forwarder),
    //dpb_ptr->FeatureSupport.ApplicationExist                    // see structure ProductGISValues. SOC1 does. Structure fields are preset by EDDI accordingly
    //dpb_ptr->FeatureSupport.ClusterIPSupport                    // IPClustering is the ability to have 2 IP-addresses, only usable in BSD stack. Preset by EDDI
    //  MRP-interconnection support (additional feature/protocol of PA-CI (scalance))
    //dpb_ptr->FeatureSupport.MRPInterconnFwdRulesSupported   = EDD_FEATURE_DISABLE;     // DISABLE: support for forwarding of MRP interconnection frames disabled (=forwarding of MRP interconnection dest. addr. on ring ports)
    //dpb_ptr->FeatureSupport.MRPInterconnOriginatorSupported = EDD_FEATURE_DISABLE;     // currently only disable allowed (7.1 i10)

    /* Port Mapping*/

    dpb_ptr->PortMap.PortCnt                        = LSAS_CFG_PORT_CNT;

    for (i=0; i < EDD_CFG_MAX_PORT_CNT; i++)
    {
        if (i < LSAS_CFG_PORT_CNT)
        {
            dpb_ptr->PortMap.UsrPortID_x_to_HWPort_y[i] = LSAS_CFG_GET_HW_PORT_IDX(i)+1/*idx -> number */;
        }
        else
        {
            dpb_ptr->PortMap.UsrPortID_x_to_HWPort_y[i] = EDDI_PORT_NOT_CONNECTED;
        }
    }

    #if defined (EDDI_SII_ENABLED)
    #if defined (EDDI_CFG_SII_FLEX_MODE)
    dpb_ptr->sii_mode = EDDI_SII_STANDARD_MODE; //EDDI_SII_STANDARD_MODE or EDDI_SII_POLLING_MODE
    #endif
    #endif


    /* fill out sys_rqb for service open_device */

    LSA_RQB_SET_OPCODE(rqb_ptr, EDD_OPC_SYSTEM);

    ini = (EDDI_RQB_DDB_INI_TYPE *)rqb_ptr->pParam;

    ini->Cbf    = 0; // synchron
    ini->pDPB   = dpb_ptr;

    rqb_ptr->Service = EDDI_SRV_DEV_OPEN;

    /* open device */
    eddi_system(rqb_ptr);

    if( EDD_RQB_GET_RESPONSE(rqb_ptr) != EDD_STS_OK )
    {
        LSAS_FATAL(LSAS_FATAL_ERR_EDD);
    }
    else
    {
        /* keep edd_handle for future use */
        lsas_com_set_hxdb(ini->hDDB);
    }

}

/*****************************************************************************/

void    lsas_edd_system_basic_ini   (EDD_RQB_TYPE  * rqb_ptr, EDDI_UPPER_CMP_INI_PTR_TYPE comp_ptr)
{

    /* install interrupt handler here */
    comp_ptr->hDDB = lsas_com_get_hddb();
    comp_ptr->Cbf  = 0; /* do not use call back service */

  //basic_init->NRT.u.IF.A__0.MaxRcvFrame_ToLookUp   = 0; // no receive load limitation
    comp_ptr->NRT.u.IF.A__0.MaxRcvFrame_ToLookUp   = LSAS_EDD_NRT_RCV_MAX_FRAME_TO_LOOK_UP;
    comp_ptr->NRT.u.IF.A__0.MaxRcvFrame_SendToUser = LSAS_EDD_NRT_RCV_MAX_FRAME_SEND_TO_USER;
    comp_ptr->NRT.u.IF.A__0.MaxRcvFragments_ToCopy = LSAS_EDD_NRT_RCV_MAX_FRAME_TO_LOOK_UP;

#ifndef LSAS_CFG_EDD_DISABLE_UNICAST_WHITELIST
    comp_ptr->NRT.u.IF.A__0.RxFilterUDP_Unicast    = LSA_TRUE;
#endif
#ifndef LSAS_CFG_EDD_DISABLE_BROADCAST_WHITELIST
    comp_ptr->NRT.u.IF.A__0.RxFilterUDP_Broadcast  = LSA_TRUE;
#endif
    comp_ptr->NRT.u.IF.A__0.RxFilterIFMACAddr      = LSA_TRUE;

  // in special cases it could be useful to adjust the memory mode per channel
    comp_ptr->NRT.u.IF.A__0.MemModeBuffer  = MEMORY_SDRAM_ERTEC;
    comp_ptr->NRT.u.IF.A__0.MemModeDMACW   = MEMORY_SDRAM_ERTEC;

    comp_ptr->NRT.u.IF.A__0.RxCntDscr      = LSAS_EDD_NRT_RCV_NUMBER_OF_DMACW_BUFFERS;

    comp_ptr->NRT.u.IF.A__0.TxCntDscr      = LSAS_EDD_NRT_SND_NUMBER_OF_DMACW_BUFFERS;

  //basic_init->NRT.u.IF.B__0.MaxRcvFrame_ToLookUp                = 0; // no receive load limitation
    comp_ptr->NRT.u.IF.B__0.MaxRcvFrame_ToLookUp                  = LSAS_EDD_NRT_RCV_MAX_FRAME_TO_LOOK_UP_B0;
    comp_ptr->NRT.u.IF.B__0.MaxRcvFrame_SendToUser                = LSAS_EDD_NRT_RCV_MAX_FRAME_SEND_TO_USER_B0;
    comp_ptr->NRT.u.IF.B__0.MaxRcvFragments_ToCopy                = LSAS_EDD_NRT_RCV_MAX_FRAME_TO_LOOK_UP_B0;

#ifndef LSAS_CFG_EDD_DISABLE_UNICAST_WHITELIST
    comp_ptr->NRT.u.IF.B__0.RxFilterUDP_Unicast    = LSA_TRUE;
#endif
#ifndef LSAS_CFG_EDD_DISABLE_BROADCAST_WHITELIST
    comp_ptr->NRT.u.IF.B__0.RxFilterUDP_Broadcast  = LSA_TRUE;
#endif

  // in special cases it could be useful to adjust the memory mode per channel
    comp_ptr->NRT.u.IF.B__0.MemModeBuffer  = MEMORY_SDRAM_ERTEC;
    comp_ptr->NRT.u.IF.B__0.MemModeDMACW   = MEMORY_SDRAM_ERTEC;

    //ERTEC 200 workaround for the internal PHY: PHY does not detect a cable break if only one line is interrupted
    comp_ptr->PhyBugfix.MysticCom.bActivate               = LSA_TRUE;   // general activation
    comp_ptr->PhyBugfix.MysticCom.bReplaceDefaultSettings = LSA_FALSE;  // internal default settings
    //  basic_init->phy_bugfix.MysticCom.Tp_RcvFrame_InMs      = 30;
    //  basic_init->phy_bugfix.MysticCom.Diff_ESD              = 15;
    //  basic_init->phy_bugfix.MysticCom.T_LinkDown_NoRcv_InMs = 0; // switched off
    //  basic_init->phy_bugfix.MysticCom.T_LinkDown_NoRcv_InMs = 1000 * 60 * 3; // 3 Minutes

    // ini CYCLE-Parameter
    // example: adjust cycle (i.e. without reduction)
    // always apply that ClkCy_Period * AplClk_Divider = cycle length in 10ns ticks

    comp_ptr->CycleComp.ClkCy_Period         = 3125;
    comp_ptr->CycleComp.AplClk_Divider       = 32;
    // !! not release ClkCy_Period Interrupt -> arises with the thousandfold frequency of new cycle
    comp_ptr->CycleComp.ClkCy_Int_Divider    = comp_ptr->CycleComp.AplClk_Divider;

    // no Delay at ClkCy Interrupt
    comp_ptr->CycleComp.ClkCy_Delay          = 0x00000000;
    // no Delay at ClkCy_Int Interrupt
    comp_ptr->CycleComp.ClkCy_Int_Delay      = 0x00000000;

    comp_ptr->CycleComp.CompVal1Int          = LSA_FALSE;
    comp_ptr->CycleComp.CompVal2Int          = LSA_FALSE;
    comp_ptr->CycleComp.CompVal3Int          = LSA_FALSE;
    comp_ptr->CycleComp.CompVal4Int          = LSA_FALSE;

    // release CompVal5Int
    comp_ptr->CycleComp.CompVal5Int          = LSA_FALSE; /* for testing release with LSA_TRUE --> no function at the moment in edd */

    comp_ptr->CycleComp.CompVal1             = 0x00000000;
    comp_ptr->CycleComp.CompVal2             = 0x00000000;
    comp_ptr->CycleComp.CompVal3             = 0x00000000;
    comp_ptr->CycleComp.CompVal4             = 0x00000000;

    // CompVal5Int to the middle of the cycle
    // comp_ptr->CycleComp.CompVal5          = (CycleTimeIn_us * 100) / 2; // For testing only --> reserved for further use in EDD
    comp_ptr->CycleComp.CompVal5             = 0; // Deactivated

    /* For controlling the signals XPLL_Ext_Out and XPLL_EXT_IN to synchronize, following steps are necessary                     */
    /* EDDI_CYCLE_INI_PLL_MODE_OFF or EDDI_CYCLE_INI_PLL_MODE_XPLL_EXT_OUT must be selected                                       */

    /* During the ERTEC is running the register AplClk_Divider will adjust to the CycleLength, the register ClkCy_Period is fix   */
    /* The phase of the synchronize signal XPLL_Ext_Out -> T(XPLL_Ext_Out) = T(Register[ClkCy_Period]) * Register[AplClk_Divider] */
    /* The register ClkCy_Period will be set on 3125 and register AplClk_Divider on CycleBaseFactor                               */
    comp_ptr->CycleComp.PDControlPllMode       = EDDI_CYCLE_INI_PLL_MODE_XPLL_EXT_OUT;

    /* fill out sys_rqb for service nrt init */
    LSA_RQB_SET_OPCODE(rqb_ptr, EDD_OPC_SYSTEM);
    rqb_ptr->Service = EDDI_SRV_DEV_COMP_INI;
    rqb_ptr->pParam  = comp_ptr;


    /* init basic part */
    eddi_system(rqb_ptr);

    if( EDD_RQB_GET_RESPONSE(rqb_ptr) != EDD_STS_OK )
    {
        LSAS_FATAL(LSAS_FATAL_ERR_EDD);
    }

    LSAS_CFG_OUT_CHANGE_PLL_SETTINGS();
}

/*****************************************************************************/

void    lsas_edd_system_device_setup(EDD_RQB_TYPE * rqb_ptr, EDDI_DSB_TYPE * dsb_ptr)
{
    EDDI_RQB_DDB_SETUP_TYPE *param_ptr;
    #define NO_FDB_ENTRY    1

    int    i;

    /*********************************************************************************/
    /* 1. Glob Para                                                                  */
    /*********************************************************************************/

    //Local MAC address for Channel A and the source MAC address for xRT telegrams
    LSAS_CFG_GET_MAC_ADDR(& dsb_ptr->GlobPara.xRT.MACAddressSrc);

    for (i=0; i < LSAS_CFG_PORT_CNT; i++)
    {
        // Each PHY is assigned a PhyAdr
        // this is to be careful when the port mapping is switched on,
        // that the assignment is made from a user perspective and not from a hardware perspective !
        #ifdef IM_SUPPORT_DUAL_PORT_PHY
        dsb_ptr->GlobPara.PortParams[i].PhyAdr = lsas_eddi_get_phy_adr_from_hw_port(LSAS_CFG_GET_HW_PORT_IDX(i));
        #else
        dsb_ptr->GlobPara.PortParams[i].PhyAdr = LSAS_CFG_GET_HW_PORT_IDX(i);
        #endif

        /* port-specific sourceMAC-address */
        LSAS_CFG_GET_PORT_MAC_ADDR(i,(LSA_UINT8 *) & dsb_ptr->GlobPara.PortParams[i].MACAddress.MacAdr);

        if (i <= 1)
        {
            if (LSAS_CFG_PORT_CNT == 1)
            {
                dsb_ptr->GlobPara.PortParams[i].MRPRingPort    = EDD_MRP_NO_RING_PORT;
            }
            else
            {
                dsb_ptr->GlobPara.PortParams[i].MRPRingPort    = EDD_MRP_RING_PORT_DEFAULT;
            }
        }
        else
        {
            /* Port 1 + 2 are ringports */
            dsb_ptr->GlobPara.PortParams[i].MRPRingPort    = EDD_MRP_NO_RING_PORT;
        }

        dsb_ptr->GlobPara.PortParams[i].IsWireless     = LSA_FALSE; /* No Wireless Port */

        #if (LSAS_CFG_PORT_CNT == 3)
        /* no cross cable ist necessary in case of Auto-Negotiation is off */
        /* mandatory wiring between port2 device1 <--> port1 device2 */
        /* port1 -> Endgeraeteport; port2 -> Switchport; port3 -> Switchport */
        if ( i == 0 )
        {
            dsb_ptr->GlobPara.PortParams[i].IsMDIX = LSA_TRUE;  /* If Auto-Neg is off; Rx and Tx lines will be crossed */
        }
        else
        {
            dsb_ptr->GlobPara.PortParams[i].IsMDIX = LSA_FALSE; /* When Auto-Negotiation is off, MDIX is disabled */
        }
        #else
        /* no cross cable ist necessary in case of Auto-Negotiation is off */
        /* mandatory wiring between port2 device1 <--> port1 device2 */
        /* Freitagsrundenprotokoll "Festeinstellung ohne Crosskabel" 080516 */
        /* port1 -> Endgeraeteport; port2 -> Switchport */
        if(i & 1)
        {
            dsb_ptr->GlobPara.PortParams[i].IsMDIX     = LSAS_CFG_EDD_EVN_PORT_MDIX; /* When Auto-Negotiation is off, MDIX is disabled */
        }
        else
        {
            dsb_ptr->GlobPara.PortParams[i].IsMDIX     = LSAS_CFG_EDD_ODD_PORT_MDIX;  /* LSA_TRUE: If Auto-Neg is off; Rx and Tx lines will be crossed */
        }
        #endif

        //dsb_ptr->GlobPara.PortParams[i].bBC5221_MCModeSet // parameter for special Broadcom PHY Transceiver BC5221, not used
        dsb_ptr->GlobPara.PortParams[i].SupportsMRPInterconnPortConfig = EDD_SUPPORTS_MRP_INTERCONN_PORT_CONFIG_NO;   /* EDDI does not support MRP interconnection roles "client/manager" */
    }

    //only enable MRP if more then 1 port is used
    if (LSAS_CFG_PORT_CNT == 1)
    {
        dsb_ptr->GlobPara.MRPDefaultRoleInstance0 = EDD_MRP_ROLE_NONE; /* EDD_MRP_ROLE_MANAGER or EDD_MRP_ROLE_CLIENT */
        dsb_ptr->GlobPara.MRPSupportedRole = EDD_MRP_ROLE_NONE;
        dsb_ptr->GlobPara.MaxMRP_Instances  = 0;

        dsb_ptr->GlobPara.MRPSupportedMultipleRole    = 0;
        dsb_ptr->GlobPara.MRAEnableLegacyMode         = EDD_MRA_DISABLE_LEGACY_MODE;  /* default, device supports MRA mode 2.0. It's stored only */
        dsb_ptr->GlobPara.MaxMRPInterconn_Instances   = 0;
        //dsb_ptr->GlobPara.SupportedMRPInterconnRole = EDD_SUPPORTED_MRP_INTERCONN_ROLE_CAP_NONE;    // EDDI doesn't support anything else
    }
    else
    {
        dsb_ptr->GlobPara.MRPDefaultRoleInstance0 = EDD_MRP_ROLE_CLIENT; /* EDD_MRP_ROLE_MANAGER or EDD_MRP_ROLE_CLIENT */
        dsb_ptr->GlobPara.MRPSupportedRole = EDD_MRP_ROLE_CAP_CLIENT;
        dsb_ptr->GlobPara.MaxMRP_Instances  = 1;

        dsb_ptr->GlobPara.MRPSupportedMultipleRole    = 0;
        dsb_ptr->GlobPara.MRAEnableLegacyMode         = EDD_MRA_DISABLE_LEGACY_MODE;      /* default, device supports MRA mode 2.0. It's stored only */
        dsb_ptr->GlobPara.MaxMRPInterconn_Instances   = 0;
        //dsb_ptr->GlobPara.SupportedMRPInterconnRole = EDD_SUPPORTED_MRP_INTERCONN_ROLE_CAP_NONE;    // EDDI doesn't support anything else
    }

    #ifdef EDDI_CFG_ERTEC_400
        dsb_ptr->GlobPara.MaxBridgeDelay           = 1920;
    #else
        dsb_ptr->GlobPara.MaxBridgeDelay           = 2920;
    #endif

    /*********************************************************************************/
    /* 2. Swi Para                                                                   */
    /*********************************************************************************/



    /*********************************************************************************/
    /* 3. NRT Para                                                                   */
    /*********************************************************************************/


    /* fill out sys_rqb for service device setup */
    LSA_RQB_SET_OPCODE(rqb_ptr, EDD_OPC_SYSTEM);
    rqb_ptr->Service = EDDI_SRV_DEV_SETUP;
    param_ptr = rqb_ptr->pParam;

    param_ptr->hDDB  = lsas_com_get_hddb();
    param_ptr->Cbf   = 0; /* do not use call back service */
    param_ptr->pDSB  = dsb_ptr;

    /* setup and start device */
    eddi_system(rqb_ptr);

    if( EDD_RQB_GET_RESPONSE(rqb_ptr) != EDD_STS_OK )
    {
        LSAS_FATAL(LSAS_FATAL_ERR_EDD);
    }

}



void lsas_eddi_req_open(void)
{
    LSAS_EDD_OPEN_PARA    * edd_param_ptr;
    EDD_RQB_TYPE          * edd_sys_rqb_ptr;
    LSA_RESULT              ret_val;
    IRTE_SW_MAKRO_T       * _irte = (IRTE_SW_MAKRO_T *)LSAS_CFG_IRTE_START;

    ret_val = EDD_STS_OK;

    LSAS_ALLOC_LOCAL_MEM( (LSAS_LOCAL_MEM_PTR_TYPE)& edd_param_ptr, sizeof(LSAS_EDD_OPEN_PARA));
    if (edd_param_ptr == 0)
    {
        LSAS_FATAL(LSAS_FATAL_ERR_NULL_PTR);
    }
    else
    {
        /* memset needed for clean gis values */
        LSA_MEMSET(edd_param_ptr, 0, sizeof(LSAS_EDD_OPEN_PARA));
    }
    LSAS_ALLOC_LOCAL_MEM( (LSAS_LOCAL_MEM_PTR_TYPE)& edd_sys_rqb_ptr, sizeof(EDD_RQB_TYPE));
    if (edd_sys_rqb_ptr == 0)
    {
        LSAS_FATAL(LSAS_FATAL_ERR_NULL_PTR);
    }
    /* setup global init struct *************************************/
    /****************************************************************/

    edd_param_ptr->gis.pInternalUse = LSA_NULL;

    /* ERTEC200: 0 Default
     *           1 ET 200M IM153-4
     *           2 PN/PN Coupler 158-3AD00
     * SOC1:     0 Default
     *           1 CPU 317-2PN
     *           2 CPU 319-2PN
     *           3 CPU 515 / 516
     *           4 SOC1 PCIe card
     *           5 ET 200SP IM155-6PN/3 HF
     *
     * ErtecType will be decide in lsas_edd_system_setup_gis */

#if defined (EDDI_CFG_ERTEC_200)
    lsas_edd_system_setup_gis(&edd_param_ptr->gis, 1);
#elif defined (EDDI_CFG_SOC)
    /* SOC1 */
    lsas_edd_system_setup_gis(&edd_param_ptr->gis, 5);
#else
    /* ERTEC400, not used */
    lsas_edd_system_setup_gis(&edd_param_ptr->gis, 1);
#endif

    /* call edd convenience functions *******************************/
    /****************************************************************/

    ret_val = eddi_InitDPBWithDefaults(&edd_param_ptr->dpb, &edd_param_ptr->gis);
    if (EDD_STS_OK != ret_val)
    {
        LSAS_FATAL(LSAS_FATAL_ERR_EDD);
    }

    ret_val = eddi_InitCOMPWithDefaults(&edd_param_ptr->comp, &edd_param_ptr->gis);
    if (EDD_STS_OK != ret_val)
    {
        LSAS_FATAL(LSAS_FATAL_ERR_EDD);
    }

    ret_val = eddi_InitDSBWithDefaults(&edd_param_ptr->dsb, &edd_param_ptr->gis);
    if (EDD_STS_OK != ret_val)
    {
        LSAS_FATAL(LSAS_FATAL_ERR_EDD);
    }

    if (edd_param_ptr->gis.KRAMFree <= 0)
    {
        LSAS_FATAL(LSAS_FATAL_ERR_EDD);
    }

    /* edd device setup ==========================================*/

    /*************************************************************
     * setup and open device                                     *
     *************************************************************/
    /* fill out parameters */

    edd_sys_rqb_ptr->pParam = &edd_param_ptr->dpb_pparam;
    lsas_edd_system_dev_open(edd_sys_rqb_ptr, &edd_param_ptr->dpb);


    /* finished setup open device ********************************/
    #if defined (EDDI_CFG_SOC)
    LSAS_SET_PHY_PWRDOWN_OFF();     /* necessary for SOC-based interface modules (external PHY, set GPIO-based PHY power switch) */
    #endif

    /*************************************************************
     * setup and init basic part                                 *
     *************************************************************/

    /* fill out paramters */

    edd_sys_rqb_ptr->pParam = &edd_param_ptr->comp;
    lsas_edd_system_basic_ini(edd_sys_rqb_ptr,  &edd_param_ptr->comp );


    /* finished setup and open nrt part **************************/

    /*************************************************************
     * setup and start device                                    *
     *************************************************************/

#ifndef LSAS_EDDI_CUSTOM_INTERRUPT_MASK
#if defined (EDDI_SII_ENABLED)
    eddi_interrupt_unmask_user_int(lsas_com_get_hddb(), IRQ_IRT_TRANS_END|IRQ_START_OPERATE, 0);
    EDDI_CLEAR_SINGLE_BIT(_irte->s17014_switch_prozessor.x17014_SW_Interrupt_Mode_Register, (IRTE_UINT32) EDDI_PASTE_32(00,00,00,02));
#endif
#endif

#if defined (LSAS_EDDI_CUSTOM_INTERRUPT_MASK)
    LSAS_EDDI_CUSTOM_INTERRUPT_MASK();
#endif

    LSAS_INIT_ISR();


    /* fill out parameters */
    edd_sys_rqb_ptr->pParam = &edd_param_ptr->dsb_pparam;
    lsas_edd_system_device_setup(edd_sys_rqb_ptr,  &edd_param_ptr->dsb);

    LSAS_CFG_INIT_COM_RAM();

    /* finished setup and start device ***************************/

    LSAS_FREE_LOCAL_MEM(& ret_val, edd_param_ptr);
    if( ret_val != LSA_RET_OK )
    {
        LSAS_FATAL(LSAS_FATAL_ERR_MEM_FREE);
    }

    LSAS_FREE_LOCAL_MEM(& ret_val, edd_sys_rqb_ptr);
    if( ret_val != LSA_RET_OK )
    {
        LSAS_FATAL(LSAS_FATAL_ERR_MEM_FREE);
    }

    LSAS_CFG_EDD_OPEN_DONE();

}

#endif /* LSAS_CFG_USE_EDDI */

#ifdef LSAS_CFG_USE_EDDP

LSA_VOID lsas_eddp_system_ram_init()
{
    LSA_UINT32 i;

    // reset StatisticRam
    for (i = 0; i < PNIP_RAM__SIZE_STATISTIC; i+=4)
    {
        WRITE_ULONG(   (U_PNIP__BASE + PNIP_RAM__OFFSET_STATISTIC + i), 0x00000000);
    }

    // reset SyncRam
    for (i = 0; i < PNIP_RAM__SIZE_SYNC; i+=4)
    {
        WRITE_ULONG(   (U_PNIP__BASE + PNIP_RAM__OFFSET_SYNC + i), 0x00000000);
    }
    /*  reset is done by hardware
    // reset PackCtrlRam
    for (i = 0; i < PNIP_RAM__SIZE_PACK_CTRL; i+=4)
    {
        WRITE_ULONG(   (U_PNIP__BASE + PNIP_RAM__OFFSET_PACK_CTRL + i), 0x00000000);
    }

    // reset PackDataRam
    for (i = 0; i < PNIP_RAM__SIZE_PACK_DATA; i+=4)
    {
        WRITE_ULONG(   (U_PNIP__BASE + PNIP_RAM__OFFSET_PACK_DATA + i), 0x00000000);
    }
    */
    // reset CmdRam
    for (i = 0; i < PNIP_RAM__SIZE_CMD; i+=4)
    {
        WRITE_ULONG(   (U_PNIP__BASE + PNIP_RAM__OFFSET_CMD + i), 0x00000000);
    }

    // reset ApiCtrlRam
    for (i = 0; i < PNIP_RAM__SIZE_API_CTRL; i+=4)
    {
        WRITE_ULONG(   (U_PNIP__BASE + PNIP_RAM__OFFSET_API_CTRL + i), 0x00000000);
    }
}

LSA_VOID lsas_eddp_system_dev_open(EDD_RQB_TYPE * rqb_ptr, LSAS_EDD_OPEN_PARA * eddp_param_ptr)
{
    LSA_UINT8                k;
    EDD_RSP                  response;

    LSAS_MEMSET(rqb_ptr,0,sizeof(EDD_RQB_TYPE));
    LSAS_MEMSET(&eddp_param_ptr->dpb_pparam,0,sizeof(EDDP_RQB_DEV_OPEN_TYPE));
    LSAS_MEMSET(&eddp_param_ptr->dpb,0,sizeof(eddp_param_ptr->dpb));

    //edd_param_ptr->dpb =
    rqb_ptr->pParam = &eddp_param_ptr->dpb_pparam;
    eddp_param_ptr->dpb_pparam.pDPB = &eddp_param_ptr->dpb;   /* setup dependencies of nested dev-open-structures */

    /* fill the device descriptor with default settings */
    response = eddp_InitDPBWithDefaults(eddp_param_ptr->dpb_pparam.pDPB, EDDP_HW_ERTEC200P, EDDP_LOCATION_LOCAL, EDDP_HW_IF_A, &(eddp_param_ptr->iniSWICHAR));
    if (response != EDD_STS_OK )
    {
        LSAS_FATAL(0);
    }

    eddp_param_ptr->dpb_pparam.pDPB->hSysDev      = 0xDE;      /* identifier of the device. Specified by system adaption and used for device specific output macros etc. */
    eddp_param_ptr->dpb_pparam.pDPB->pPnipBase    = (EDD_COMMON_MEM_PTR_TYPE) U_PNIP__BASE;       /* address base for PNIP from EDDPs point of view*/
    eddp_param_ptr->dpb_pparam.pDPB->pK32TCMBase  = (EDD_COMMON_MEM_PTR_TYPE) KRISC_DTCM_BASE;
#ifdef EDDP_CFG_SYSRED_API_SUPPORT
    eddp_param_ptr->dpb_pparam.pDPB->GSharedRAM_BaseAddr  = (EDD_COMMON_MEM_PTR_TYPE) &lsas_data.eddp.gshared_mem;
#endif
    eddp_param_ptr->dpb_pparam.pDPB->UserCmdSharedMem_BaseAddr  = (EDD_COMMON_MEM_PTR_TYPE)&lsas_data.eddp.usrcmdshared_mem;
    eddp_param_ptr->dpb_pparam.pDPB->InterfaceID  = LSAS_CFG_INTERFACE_ID;

    //eddp_param_ptr->dpb_pparam.pDPB->K32TraceLevel = LSA_TRACE_LEVEL_CHAT;

    // ***** enable receiption of dropped frames
    eddp_param_ptr->dpb_pparam.pDPB->NRT.RxDropLogging = LSA_TRUE;
    // ***** overload TCP --> default is 0 (disabled)
    eddp_param_ptr->dpb_pparam.pDPB->NRT.TxItemsInTcpRing = 32;
    eddp_param_ptr->dpb_pparam.pDPB->NRT.RxItemsInTcpRing = 32;
#ifndef LSAS_CFG_EDD_DISABLE_UNICAST_WHITELIST
    eddp_param_ptr->dpb_pparam.pDPB->NRT.RxFilterUDP_Unicast    = LSA_TRUE;
#endif
#ifndef LSAS_CFG_EDD_DISABLE_BROADCAST_WHITELIST
    eddp_param_ptr->dpb_pparam.pDPB->NRT.RxFilterUDP_Broadcast  = LSA_TRUE;
#endif
    eddp_param_ptr->dpb_pparam.pDPB->SWI.PhyAddressOffset       = 0;


    /* Map PortID to Hardware Port for each IF. Not used PortIDs are set to EDDP_PORT_NOT_CONNECTED. */
    for (k=0; k < EDD_CFG_MAX_PORT_CNT; k++)
    {
        if (k < LSAS_CFG_PORT_CNT)
        {
            eddp_param_ptr->dpb_pparam.pDPB->SWI.PortMap.PortID_x_to_HardwarePort_y[k] = LSAS_CFG_GET_HW_PORT_IDX(k) + 1/*idx -> number*/;
            // structure elements PortID_x_to_ASICPort_y[] and PortID_x_to_SMIPort_y[] are for HERA, don't care for Ertec200+
        }
        else
        {
            eddp_param_ptr->dpb_pparam.pDPB->SWI.PortMap.PortID_x_to_HardwarePort_y[k] = EDDP_PORT_NOT_CONNECTED;
        }
    }

    /* MRP configuration: see eddp_usr.doc, chapter MRP (media redundancy protocol).
     * "If no R-Ports are configured MRP is not supported and MRP PRM-Records will cause a PRM-Error (Consistency) on PRM-END!"
     */
#if ( LSAS_CFG_USE_MRP == 0 )
        eddp_param_ptr->dpb_pparam.pDPB->SWI.MaxMRP_Instances           = 0;
        eddp_param_ptr->dpb_pparam.pDPB->SWI.MRPDefaultRoleInstance0    = EDD_MRP_ROLE_NONE;
        eddp_param_ptr->dpb_pparam.pDPB->SWI.MRPSupportedRole           = EDD_MRP_ROLE_NONE;
#else
        eddp_param_ptr->dpb_pparam.pDPB->SWI.MaxMRP_Instances           = 1;
        eddp_param_ptr->dpb_pparam.pDPB->SWI.MRPDefaultRoleInstance0    = EDD_MRP_ROLE_CLIENT;
        eddp_param_ptr->dpb_pparam.pDPB->SWI.MRPSupportedRole           = EDD_MRP_ROLE_CAP_CLIENT;

        for ( k=0; k < LSAS_CFG_PORT_CNT; k++ )
        {
            eddp_param_ptr->dpb_pparam.pDPB->SWI.MRPRingPort[k] = EDD_MRP_RING_PORT_DEFAULT;
            //eddp_param_ptr->dpb_pparam.pDPB->SWI.SupportsMRPInterconnPortConfig[k] = EDD_SUPPORTS_MRP_INTERCONN_PORT_CONFIG_NO;           /* EDDP does not support MRP interconnection (Client/Manager) */
        }
#endif

    eddp_param_ptr->dpb_pparam.pDPB->SWI.MRAEnableLegacyMode = EDD_MRA_DISABLE_LEGACY_MODE;      /* default, device supports MRA mode 2.0. It's stored only */

    eddp_param_ptr->dpb_pparam.pDPB->SWI.MaxMRPInterconn_Instances   = 0;     /* MRP interconnection: needs to be set to >0 if MRPInterconnFwdRulesSupported  is set to true. */
    //eddp_param_ptr->dpb_pparam.pDPB->SWI.SupportedMRPInterconnRole = 0;     /* EDDP does not support MRP interconnection (Client/Manager) */

    /* settings for SNMP data base (device specific, projectable features)*/
    #ifdef LSAS_PU_FEATURE_SHORTPREAMBLE_SUPPORT
    eddp_param_ptr->dpb_pparam.pDPB->FeatureSupport.bShortPreambleSupported    = TRUE;
    #endif
    #ifdef LSAS_PU_FEATURE_MRPD_SUPPORT
    eddp_param_ptr->dpb_pparam.pDPB->FeatureSupport.bMRPDSupported             = TRUE;
    #endif
    #ifdef LSAS_PU_FEATURE_FRAGTYPE_DYNAMIC_SUPPORTED
    eddp_param_ptr->dpb_pparam.pDPB->FeatureSupport.FragmentationtypeSupported = EDD_DPB_FEATURE_FRAGTYPE_SUPPORTED_DYNAMIC;
    #endif
    #ifdef LSAS_PU_FEATURE_ABSOLUTE_FWD_SUPPORTED
    eddp_param_ptr->dpb_pparam.pDPB->FeatureSupport.IRTForwardingModeSupported |= EDD_DPB_FEATURE_IRTFWDMODE_SUPPORTED_ABSOLUTE;
    #endif
    #ifdef LSAS_PU_FEATURE_RELATIVE_FWD_SUPPORTED
    eddp_param_ptr->dpb_pparam.pDPB->FeatureSupport.IRTForwardingModeSupported |= EDD_DPB_FEATURE_IRTFWDMODE_SUPPORTED_RELATIVE;
    #endif
    #ifdef LSAS_PU_FEATURE_DFP_SUPPORTED
    eddp_param_ptr->dpb_pparam.pDPB->FeatureSupport.MaxDFPFrames               = 8;
    #endif
#if ( LSAS_CFG_USE_MRP == 0 )
    eddp_param_ptr->dpb_pparam.pDPB->FeatureSupport.AdditionalForwardingRulesSupported  = EDD_FEATURE_DISABLE;   /* device allows forwarding of MRP HSYNC frames */
#else
    eddp_param_ptr->dpb_pparam.pDPB->FeatureSupport.AdditionalForwardingRulesSupported  = EDD_FEATURE_ENABLE;   /* device allows forwarding of MRP HSYNC frames */
#endif
    eddp_param_ptr->dpb_pparam.pDPB->FeatureSupport.ApplicationExist                    = EDD_FEATURE_DISABLE;  /* device is just a HSYNC forwarder */
    //eddp_param_ptr->dpb_pparam.pDBP->FeatureSupport.ClusterIPSupport                    = EDD_FEATURE_DISABLE;  /* currently not used in EDDP. ClusterIPSupport is the ability
    //                                                                                                               to have multiple IP addresses. Only realized for BSD stack */
    //pDPB->FeatureSupport.MRPInterconnFwdRulesSupported   = EDD_FEATURE_DISABLE;      // disable = no special support for forwarding of MRP interconnection frames (=new protocol in 7.1 i10)
    //pDPB->FeatureSupport.MRPInterconnOriginatorSupported = EDD_FEATURE_DISABLE;      // only disable is supported

    /* base address of IO buffer in PERIF. Is used by EDDP if PNIP-data is put into PERIF memory */
    eddp_param_ptr->dpb_pparam.pDPB->CRT.pIOBaseAdrPerif   = (EDD_UPPER_MEM_U8_PTR_TYPE) U_PERIF_AHB__BASE;
    /* is used if provider/consumer doesn't use the PERIF */
    eddp_param_ptr->dpb_pparam.pDPB->CRT.pIOBaseAdrExtRam  = (EDD_UPPER_MEM_U8_PTR_TYPE) 0;

    /* EDDP 5.2 features : let EDDP manage application timer 2 and its first comparator (transfer end) */
    eddp_param_ptr->dpb_pparam.pDPB->CRT.ApplTimerMode = EDDP_APPL_TIMER_CFG_MODE_TRANSFER_END;
        /* reduction usage in application timer's cycle length (1 -> no reduction, 2 -> application timer cycle length doubled) */
    eddp_param_ptr->dpb_pparam.pDPB->CRT.ApplTimerReductionRatio = 1;
        /* comparator adujustment in nanoseconds*/
    eddp_param_ptr->dpb_pparam.pDPB->CRT.TransferEndCorrectionValue = 0;
        /* sign of parameter TransferEndCorrectionValue*/
    eddp_param_ptr->dpb_pparam.pDPB->CRT.isTransferEndCorrectionPos = LSA_TRUE;

    eddp_param_ptr->dpb_pparam.pDPB->HWCfg.IRQAcycRcv  = 2;  /* IRQ-number to use with INT_Acyc_Rcv_IF interrupts */
    //eddp_param_ptr->dpb_pparam.pDPB->HWCfg.TxLocalApplicationFeedExtNs / RxLocalApplicationDelayExtNs  : HERA only, preset by EDDP

    /* set callback function for asynchronous notification (processing of request finished) */
    eddp_param_ptr->dpb_pparam.Cbf          = lsas_cbf_sys_edd_request_lower_done;

    /* fill the RQB */
    LSAS_RQB_SET_OPCODE(rqb_ptr, EDD_OPC_SYSTEM);
    //LSAS_RQB_SET_HANDLE(rqb_ptr, 0);
    //LSAS_RQB_SET_USERID_UVAR32(rqb_ptr, 0);
    rqb_ptr->Service = EDDP_SRV_DEV_OPEN;
    rqb_ptr->comp_id = LSA_COMP_ID_EDDP; /* so this RQB can be differed */

    eddp_system((EDD_UPPER_RQB_PTR_TYPE)rqb_ptr);

}

LSA_VOID lsas_eddp_system_device_setup(EDD_RQB_TYPE * rqb_ptr, LSAS_EDD_OPEN_PARA * eddp_param_ptr)
{
    EDD_RSP                  response;
    LSA_UINT32               port_cnt;

    rqb_ptr->pParam = &eddp_param_ptr->dsb_pparam;

    LSAS_MEMSET(rqb_ptr,0,sizeof(EDD_RQB_TYPE));
    LSAS_MEMSET(&eddp_param_ptr->dsb_pparam,0,sizeof(EDDP_RQB_DEV_SETUP_TYPE));
    LSAS_MEMSET(&eddp_param_ptr->dsb,0,sizeof(EDDP_DSB_TYPE));

    eddp_param_ptr->dsb_pparam.hDDB         = lsas_com_get_hddb();
    eddp_param_ptr->dsb_pparam.pDSB         = &eddp_param_ptr->dsb;
    eddp_param_ptr->dsb_pparam.Cbf          = lsas_cbf_sys_edd_request_lower_done;

    /* use settings of device open request to setup the device setup block (DSB) with default values*/
    response= eddp_InitDSBWithDefaults(&eddp_param_ptr->dpb, eddp_param_ptr->dsb_pparam.pDSB);
    if (response != EDD_STS_OK )
    {
        return;
    }

    // set interface MAC address
    LSAS_CFG_GET_MAC_ADDR(& eddp_param_ptr->dsb_pparam.pDSB->IF_MAC_Address.MacAdr[0]);

    for(port_cnt = 0; port_cnt < LSAS_CFG_PORT_CNT; port_cnt++)
    {
        /* port-specific sourceMAC-address */
        LSAS_CFG_GET_PORT_MAC_ADDR(port_cnt,(LSA_UINT8 *) & eddp_param_ptr->dsb_pparam.pDSB->PortParams[port_cnt].Port_MAC_Address.MacAdr[0]);

        eddp_param_ptr->dsb_pparam.pDSB->PortParams[port_cnt].IsWireless = LSA_FALSE;
        eddp_param_ptr->dsb_pparam.pDSB->PortParams[port_cnt].IsPulled   = LSA_FALSE;
        if (port_cnt & 1)
        {   // ***** Port2 + 4: LSA_TRUE
            eddp_param_ptr->dsb_pparam.pDSB->PortParams[port_cnt].IsMDIX = LSAS_CFG_EDD_EVN_PORT_MDIX;
        }
        else
        {   // ***** Port1 + 3: LSA_FALSE
            eddp_param_ptr->dsb_pparam.pDSB->PortParams[port_cnt].IsMDIX = LSAS_CFG_EDD_ODD_PORT_MDIX;
        }
        eddp_param_ptr->dsb_pparam.pDSB->PortParams[port_cnt].GBitSupport = EDD_FEATURE_DISABLE;            // feature for HERA
    }

    /*-----------------------------------------------------------------------*/
    /* Setup default wait times for group interrupts                         */
    /*-----------------------------------------------------------------------*/
    /* EOI-WaitTime              */
    /* in 1024ns ticks           */
    /* 977 = 1ms                 */
    /* 488 = 500us               */
    /* 244 = 250us               */
    /* 122 = 125us               */
    /* 61  = 62.5us              */
    /* 30  = 31.25us             */
    /* Bei Werten kleiner als 2 gibt es offenbar Probleme, dass der Puls fuer die Quittung zu kurz werden kann */
    eddp_param_ptr->dsb_pparam.pDSB->InterruptHandling.EOI_WaitTime         = 244;
    eddp_param_ptr->dsb_pparam.pDSB->InterruptHandling.IRQ_WaitTime_AcycRcv = 31;   // 1 ms throttle rate for acyclic receive IRQ (according to PSI).
                                                                                    // With the default value (4 ms), we don't make the load test

    /* fill the RQB */
    LSAS_RQB_SET_OPCODE(rqb_ptr, EDD_OPC_SYSTEM);
    //EDD_RQB_SET_HANDLE(rqb_ptr, 0);
    //EDD_RQB_SET_USERID_UVAR32(rqb_ptr, 0);
    rqb_ptr->Service = EDDP_SRV_DEV_SETUP;
    rqb_ptr->pParam = &eddp_param_ptr->dsb_pparam;

    rqb_ptr->comp_id = LSA_COMP_ID_EDDP; /* so this RQB can be differed */

    eddp_system((EDD_UPPER_RQB_PTR_TYPE)rqb_ptr);
}


LSA_VOID lsas_eddp_system_init_cmd_iface()
{
    EDD_COMMON_MEM_PTR_TYPE      local_pnip_base = (EDD_COMMON_MEM_PTR_TYPE)U_PNIP__BASE;
    EDDP_USER_CMD_HANDLE_TYPE    local_handle;
    EDD_RSP                      ret_val;

    ret_val = eddp_UserCmdInit(EDDP_LOCATION_LOCAL, (EDD_COMMON_MEM_PTR_TYPE)&lsas_data.eddp.usrcmdshared_mem); // *NEW* Shared Memory for SyncIndication (optional)
    if (ret_val != EDD_STS_OK)
    {
        LSAS_FATAL(ret_val);
    }

    ret_val = eddp_UserCmdInitHandle(local_pnip_base, &local_handle);
    if (ret_val != EDD_STS_OK)
    {
        LSAS_FATAL(ret_val);
    }

    lsas_data.eddp_cmd_iface_handle = local_handle;

    return;
}


LSA_VOID lsas_eddp_req_open()
{
    LSA_RESULT              ret_val;

    switch(lsas_data.eddp.sm)
    {
        case LSAS_EDDP_SM_CLOSED:
        {
            /* first call to this function at startup */
            /* prepare rqb and safe pointer */
            LSAS_EDD_OPEN_PARA    * edd_param_ptr;
            EDD_RQB_TYPE          * edd_sys_rqb_ptr;


            //ret_val = EDD_STS_OK;

            LSAS_ALLOC_LOCAL_MEM( (LSAS_LOCAL_MEM_PTR_TYPE)& edd_param_ptr, sizeof(LSAS_EDD_OPEN_PARA));
            if (edd_param_ptr == 0)
            {
                LSAS_FATAL(LSAS_FATAL_ERR_NULL_PTR);
            }
            lsas_data.eddp.eddp_param_ptr = edd_param_ptr;
            LSAS_ALLOC_LOCAL_MEM( (LSAS_LOCAL_MEM_PTR_TYPE)& edd_sys_rqb_ptr, sizeof(EDD_RQB_TYPE));
            if (edd_sys_rqb_ptr == 0)
            {
                LSAS_FATAL(LSAS_FATAL_ERR_NULL_PTR);
            }
            lsas_data.eddp.eddp_sys_rqb_ptr = edd_sys_rqb_ptr;

            /*************************************************************
             * setup and open device                                     *
             *************************************************************/
            lsas_eddp_system_dev_open(lsas_data.eddp.eddp_sys_rqb_ptr, edd_param_ptr);

            lsas_data.eddp.sm = LSAS_EDDP_SM_WAIT_OPEN_DONE;

            break;
        }
        case LSAS_EDDP_SM_WAIT_OPEN_DONE:
        {
            /* finished setup open device ********************************/

            if( LSAS_RQB_GET_RESPONSE(lsas_data.eddp.eddp_sys_rqb_ptr) != EDD_STS_OK )
            {
                LSAS_FATAL(0);
            }
            else
            {
                lsas_com_set_hxdb(lsas_data.eddp.eddp_param_ptr->dpb_pparam.hDDB);

                LSAS_INIT_ISR(0);
            }

            /*************************************************************
             * setup and start device                                    *
             *************************************************************/

            /*
             * The interrupt number is defined in the
             * interface parameter block (HWCfg-field, -> EDDP_SRV_DEV_OPEN call).
             */

            /* device setup */
            /* setup port MAC addresses and interrupt wait time */
            lsas_data.edd.request_processed = 0;

            lsas_eddp_system_device_setup(lsas_data.eddp.eddp_sys_rqb_ptr,  lsas_data.eddp.eddp_param_ptr);

            lsas_cfg_edd_system_req_check();

            lsas_data.eddp.sm = LSAS_EDDP_SM_WAIT_SETUP_DONE;

            break;
        }
        case LSAS_EDDP_SM_WAIT_SETUP_DONE:
        {
            LSAS_FREE_LOCAL_MEM(&ret_val, lsas_data.eddp.eddp_param_ptr);
            if( ret_val != LSA_RET_OK )
            {
                LSAS_FATAL(LSAS_FATAL_ERR_MEM_FREE);
            }
            LSAS_FREE_LOCAL_MEM(&ret_val, lsas_data.eddp.eddp_sys_rqb_ptr);
            if( ret_val != LSA_RET_OK )
            {
                LSAS_FATAL(LSAS_FATAL_ERR_MEM_FREE);
            }

            LSAS_CFG_INIT_COM_RAM();

            lsas_eddp_system_init_cmd_iface();

            lsas_data.eddp.sm = LSAS_EDDP_SM_DONE;

            LSAS_CFG_EDD_OPEN_DONE();

            break;
        }
        case LSAS_EDDP_SM_NOT_USED:
        default:
        {
            LSAS_FATAL(lsas_data.eddp.sm);
        }
    }
}

#endif


//=================================================================================================
//=================================================================================================
//=================================================================================================
#ifdef LSAS_CFG_USE_EDDS

//=================================================================================================
// timer callback for edds_scheduler()-call, runs in EDDS task so that it can't be interrupted
// by edds_request() calls.
//=================================================================================================
LSA_VOID lsas_edds_scheduler_timer_edd_task(LSAS_EDD_RQB_TYPE *rqb)
{
    lsas_data.edds.scheduler_rqb_in_use = FALSE;

    edds_scheduler(lsas_com_get_hddb(), FALSE);
}

LSA_VOID lsas_edds_scheduler_edd_task(LSAS_EDD_RQB_TYPE *rqb)
{
    edds_scheduler(lsas_com_get_hddb(), 1);
}

//=================================================================================================
// timer callback for edds_scheduler()-call, runs in some arbitrary task.
// It triggers the EDDS-task which, in the end, calls edds_scheduler()
//=================================================================================================
extern PNIO_UINT32 g_EddTimerTaskLock;
LSA_VOID lsas_edds_scheduler_timer_arb_task(LSA_UINT16 timer_id, LSA_USER_ID_TYPE  user_id)
{
    LSA_BOOL req = FALSE;
#if 0
    LSAS_EDDS_ENTER_CRITICAL();
    if( !lsas_data.edds.scheduler_rqb_in_use )
    {
        req = TRUE;
        lsas_data.edds.scheduler_rqb_in_use = TRUE;
    }
    LSAS_EDDS_EXIT_CRITICAL();
    if(req)
	{
		LSAS_RQB_EDD_REQUEST(&lsas_data.edds.scheduler_trigger_rqb, lsas_edds_scheduler_timer_edd_task);
	}


#else
    // OsSendMessage(TSKMA_TASK_ID_EDD, (void *) 1, OS_MBX_PRIO_NORM);
    // OsSendMessageToFront(TSKMA_TASK_ID_EDD, (void *) 0, OS_MBX_PRIO_NORM);
    if(0U < g_EddTimerTaskLock)
    {
    	OsGiveSemB(g_EddTimerTaskLock);
    }
    
#endif

}

//=================================================================================================
// fill lower level function table
//=================================================================================================
LSA_VOID lsas_edds_fill_func_table(void)
{
    LSAS_MEMSET(&lsas_data.edds.ll_func_table, 0, sizeof(lsas_data.edds.ll_func_table));

    // --------- fill lower level function table ---------
#if 1
    lsas_data.edds.ll_func_table.open                       = (EDDS_LL_OPEN_FUNC_PTR)               EDDS_LLIF_OPEN;
    lsas_data.edds.ll_func_table.setup                      = (EDDS_LL_SETUP_FUNC_PTR)              EDDS_LLIF_SETUP;
    lsas_data.edds.ll_func_table.shutdown                   = (EDDS_LL_SHUTDOWN_FUNC_PTR)           EDDS_LLIF_SHUTDOWN;
    lsas_data.edds.ll_func_table.close                      = (EDDS_LL_CLOSE_FUNC_PTR)              &EDDS_LLIF_CLOSE;
    lsas_data.edds.ll_func_table.enqueueSendBuffer          = (EDDS_LL_SEND_FUNC_PTR)               &EDDS_LLIF_SEND;
    lsas_data.edds.ll_func_table.getNextFinishedSendBuffer  = (EDDS_LL_SEND_STS_FUNC_PTR)           &EDDS_LLIF_SEND_STS;

    lsas_data.edds.ll_func_table.triggerSend                = (EDDS_LL_SEND_TRIGGER_FUNC_PTR)       &EDDS_LLIF_SEND_TRIGGER;

    lsas_data.edds.ll_func_table.getNextReceivedBuffer      = (EDDS_LL_RECV_FUNC_PTR)               &EDDS_LLIF_RECV;
    lsas_data.edds.ll_func_table.provideReceiveBuffer       = (EDDS_LL_RECV_PROVIDE_FUNC_PTR)       &EDDS_LLIF_RECV_PROVIDE;
    lsas_data.edds.ll_func_table.triggerReceive             = (EDDS_LL_RECV_TRIGGER_FUNC_PTR)       &EDDS_LLIF_RECV_TRIGGER;
    lsas_data.edds.ll_func_table.getLinkState               = (EDDS_LL_GET_LINK_STATE_FUNC_PTR)     &EDDS_LLIF_GET_LINK_STATE;
    lsas_data.edds.ll_func_table.getStatistics              = (EDDS_LL_GET_STATS_FUNC_PTR)          &EDDS_LLIF_GET_STATS;
    lsas_data.edds.ll_func_table.enableMC                   = (EDDS_LL_MC_ENABLE_FUNC_PTR)          &EDDS_LLIF_MC_ENABLE;
    lsas_data.edds.ll_func_table.disableMC                  = (EDDS_LL_MC_DISABLE_FUNC_PTR)         &EDDS_LLIF_MC_DISABLE;
    lsas_data.edds.ll_func_table.recurringTask				= (EDDS_LL_RECURRING_TASK_PTR)          &EDDS_LLIF_RECURRING_TASK;

    lsas_data.edds.ll_func_table.setLinkState               = (EDDS_LL_SET_LINK_STATE_FUNC_PTR)     &EDDS_LLIF_SET_LINK_STATE;
    lsas_data.edds.ll_func_table.backupLocationLEDs         = (EDDS_LL_LED_BACKUP_MODE_FUNC_PTR)    &EDDS_LLIF_LED_BACKUP_MODE;
    lsas_data.edds.ll_func_table.restoreLocationLEDs        = (EDDS_LL_LED_RESTORE_MODE_FUNC_PTR)   &EDDS_LLIF_LED_RESTORE_MODE;
    lsas_data.edds.ll_func_table.setLocationLEDs            = (EDDS_LL_LED_SET_FUNC_PTR)            &EDDS_LLIF_LED_SET_MODE;

    lsas_data.edds.ll_func_table.changePort                 = (EDDS_LL_SWITCH_CHANGE_PORT_FUNC_PTR) &EDDS_LLIF_CHANGE_PORT;
    #ifdef LLIF_CFG_SWITCH_SUPPORT
    lsas_data.edds.ll_func_table.setSwitchPortState         = (EDDS_LL_SWITCH_SET_PORT_STATE_FUNC_PTR)      &EDDS_LLIF_SWITCH_SET_PORT_STATE;
    lsas_data.edds.ll_func_table.controlSwitchMulticastFwd  = (EDDS_LL_SWITCH_MULTICAST_FWD_CTRL_FUNC_PTR)  &EDDS_LLIF_SWITCH_MULTICAST_FWD_CTRL;
    lsas_data.edds.ll_func_table.flushSwitchFilteringDB     = (EDDS_LL_SWITCH_FLUSH_FILTERING_DB_FUNC_PTR)  &EDDS_LLIF_SWITCH_FLUSH_FILTERING_DB;
    lsas_data.edds.ll_func_table.setSwitchPortRedundant     = (EDDS_LL_SWITCH_SET_PORT_REDUNDANT_FUNC_PTR)  &EDDS_LLIF_SWITCH_SET_PORT_REDUNDANT;
    lsas_data.edds.ll_func_table.setSwitchBridgeMode        = (EDDS_LL_SWITCH_SET_BRIDGE_MODE_FUNC_PTR)     &EDDS_LLIF_SWITCH_SET_BRIDGE_MODE;
    #ifdef LLIF_CFG_SWITCH_DROP_FRAME_SUPPORT
    lsas_data.edds.ll_func_table.getSwitchDroppedFrames     = (EDDS_LL_SWITCH_DROP_CNT_FUNC_PTR) &EDDS_LLIF_SWITCH_DROP_CNT;
    #endif /* LLIF_CFG_SWITCH_DROP_FRAME_SUPPORT */
    #endif /* LLIF_CFG_SWITCH_SUPPORT */

    #ifdef LLIF_CFG_USE_LL_ARP_FILTER
    lsas_data.edds.ll_func_table.setArpFilter               = (EDDS_LL_SET_ARP_FILTER_FUNC_PTR) &EDDS_LLIF_SET_ARP_FILTER;
    #endif

    lsas_data.edds.ll_func_table.txBuffAvailable			= (EDDS_LL_TX_BUFF_AVAILABLE_PTR)EDDS_LLIF_TX_BUFF_AVAILABLE;

#else

    lsas_data.edds.ll_func_table.open                       = (EDDS_LL_OPEN_FUNC_PTR)               &lsas_edds_ll_open;
    lsas_data.edds.ll_func_table.setup                      = (EDDS_LL_SETUP_FUNC_PTR)              &lsas_edds_ll_setup;
    lsas_data.edds.ll_func_table.shutdown                   = (EDDS_LL_SHUTDOWN_FUNC_PTR)           &lsas_edds_ll_shutdown;
    lsas_data.edds.ll_func_table.close                      = (EDDS_LL_CLOSE_FUNC_PTR)              &lsas_edds_ll_close;
    lsas_data.edds.ll_func_table.enqueueSendBuffer          = (EDDS_LL_SEND_FUNC_PTR)               &lsas_edds_ll_send;
    lsas_data.edds.ll_func_table.getNextFinishedSendBuffer  = (EDDS_LL_SEND_STS_FUNC_PTR)           &lsas_edds_ll_send_sts;

    lsas_data.edds.ll_func_table.triggerSend                = (EDDS_LL_SEND_TRIGGER_FUNC_PTR)       &lsas_edds_ll_send_trigger;

    lsas_data.edds.ll_func_table.getNextReceivedBuffer      = (EDDS_LL_RECV_FUNC_PTR)               &lsas_edds_ll_recv;
    lsas_data.edds.ll_func_table.provideReceiveBuffer       = (EDDS_LL_RECV_PROVIDE_FUNC_PTR)       &lsas_edds_ll_recv_provide;
    lsas_data.edds.ll_func_table.triggerReceive             = (EDDS_LL_RECV_TRIGGER_FUNC_PTR)       &lsas_edds_ll_recv_trigger;
    lsas_data.edds.ll_func_table.getLinkState               = (EDDS_LL_GET_LINK_STATE_FUNC_PTR)     &lsas_edds_ll_get_link_state;
    lsas_data.edds.ll_func_table.getStatistics              = (EDDS_LL_GET_STATS_FUNC_PTR)          &lsas_edds_ll_get_stats;
    lsas_data.edds.ll_func_table.enableMC                   = (EDDS_LL_MC_ENABLE_FUNC_PTR)          &lsas_edds_ll_mc_enable;
    lsas_data.edds.ll_func_table.disableMC                  = (EDDS_LL_MC_DISABLE_FUNC_PTR)         &lsas_edds_ll_mc_disable;
    lsas_data.edds.ll_func_table.recurringTask				= (EDDS_LL_RECURRING_TASK_PTR)          &lsas_edds_ll_recurringTask;

    lsas_data.edds.ll_func_table.setLinkState               = (EDDS_LL_SET_LINK_STATE_FUNC_PTR)     &lsas_edds_ll_set_link_state;
    lsas_data.edds.ll_func_table.backupLocationLEDs         = (EDDS_LL_LED_BACKUP_MODE_FUNC_PTR)    &lsas_edds_ll_led_backup_mode;
    lsas_data.edds.ll_func_table.restoreLocationLEDs        = (EDDS_LL_LED_RESTORE_MODE_FUNC_PTR)   &lsas_edds_ll_led_restore_mode;
    lsas_data.edds.ll_func_table.setLocationLEDs            = (EDDS_LL_LED_SET_FUNC_PTR)            &lsas_edds_ll_led_set_mode;

    lsas_data.edds.ll_func_table.changePort                 = (EDDS_LL_SWITCH_CHANGE_PORT_FUNC_PTR) &lsas_edds_ll_change_port;
    #ifdef LLIF_CFG_SWITCH_SUPPORT
    lsas_data.edds.ll_func_table.setSwitchPortState         = (EDDS_LL_SWITCH_SET_PORT_STATE_FUNC_PTR)      &lsas_edds_ll_switch_set_port_state;
    lsas_data.edds.ll_func_table.controlSwitchMulticastFwd  = (EDDS_LL_SWITCH_MULTICAST_FWD_CTRL_FUNC_PTR)  &lsas_edds_ll_switch_multicast_fwd_ctrl;
    lsas_data.edds.ll_func_table.flushSwitchFilteringDB     = (EDDS_LL_SWITCH_FLUSH_FILTERING_DB_FUNC_PTR)  &lsas_edds_ll_switch_flush_filtering_db;
    lsas_data.edds.ll_func_table.setSwitchPortRedundant     = (EDDS_LL_SWITCH_SET_PORT_REDUNDANT_FUNC_PTR)  &lsas_edds_ll_switch_set_port_redundant;
    lsas_data.edds.ll_func_table.setSwitchBridgeMode        = (EDDS_LL_SWITCH_SET_BRIDGE_MODE_FUNC_PTR)     &lsas_edds_ll_switch_set_bridge_mode;
    #ifdef LLIF_CFG_SWITCH_DROP_FRAME_SUPPORT
    lsas_data.edds.ll_func_table.getSwitchDroppedFrames     = (EDDS_LL_SWITCH_DROP_CNT_FUNC_PTR) &lsas_edds_ll_switch_drop_cnt;
    #endif /* LLIF_CFG_SWITCH_DROP_FRAME_SUPPORT */
    #endif /* LLIF_CFG_SWITCH_SUPPORT */

    #ifdef LLIF_CFG_USE_LL_ARP_FILTER
    lsas_data.edds.ll_func_table.setArpFilter               = (EDDS_LL_SET_ARP_FILTER_FUNC_PTR) &lsas_edds_ll_set_arp_filter;
    #endif
#endif
}

LSA_VOID* lsas_edds_system_device_open__init_llparam(LSAS_EDD_OPEN_PARA * edds_param_ptr)
{
#ifdef LSAS_EDDS_ASIC_TI
    LSA_UINT8 portIdx;

    edds_param_ptr->tiLLParameter.pRegBaseAddrGMAC                        = 0x48484000;         /* Register Base Address on TI LL (phys. base address of GMAC-SW register memory) */
    edds_param_ptr->tiLLParameter.DeviceID                                = (LSA_UINT16)0x8888; /* according to EPS, dummy value?! */
    // port mapping
    edds_param_ptr->tiLLParameter.PortMap.PortID_x_to_HardwarePort_y[0]   = 2;
    edds_param_ptr->tiLLParameter.PortMap.PortID_x_to_MDIO_USERACCESS[0]  = 0x1080;             /* according to ti_reg.h: TI_MDIO_USERACCESS0 */
    edds_param_ptr->tiLLParameter.PortMap.PortID_x_to_HardwarePort_y[1]   = 3;
    edds_param_ptr->tiLLParameter.PortMap.PortID_x_to_MDIO_USERACCESS[1]  = 0x1088;             /* according to ti_reg.h: TI_MDIO_USERACCESS1 */
    // preset rest of port mapping
    for (portIdx = 2; portIdx < EDDS_MAX_PORT_CNT; portIdx++)
    {
        edds_param_ptr->tiLLParameter.PortMap.PortID_x_to_HardwarePort_y[portIdx]  = TI_GMAC_PORT_NOT_CONNECTED;
        edds_param_ptr->tiLLParameter.PortMap.PortID_x_to_MDIO_USERACCESS[portIdx] = 0;
    }

    return ((LSA_VOID*) &edds_param_ptr->tiLLParameter);
#else
    return LSA_NULL;
#endif
}

//=================================================================================================
// from EddApi_OpenDevice
//=================================================================================================
LSA_VOID lsas_edds_system_device_open(EDD_RQB_TYPE * rqb_ptr, LSAS_EDD_OPEN_PARA * edds_param_ptr)
{
    LSAS_MEMSET(&edds_param_ptr->open_dpb, 0, sizeof(EDDS_DPB_TYPE));

    edds_InitDPBWithDefaults (&edds_param_ptr->open_dpb);

    // --------- fill lower level function table ---------
    lsas_edds_fill_func_table();

    // --------- fill User DPB---------
    edds_param_ptr->open_dpb.InterfaceID             = LSAS_EDDS_SINGLE_DEVICE_HNDL;/* Device Nummer               */
    edds_param_ptr->open_dpb.TxBufferCnt             = EDDS_TX_BUFFER_CNT;          /* number of Tx-Buffers     */
    edds_param_ptr->open_dpb.RxBufferCnt             = EDDS_RX_BUFFER_CNT;          /* number of Rx-Buffers     */
    edds_param_ptr->open_dpb.MaxInterfaceCntOfAllEDD = EDD_CFG_MAX_INTERFACE_CNT;
    edds_param_ptr->open_dpb.MaxPortCntOfAllEDD      = EDD_CFG_MAX_PORT_CNT_ALL_INTERFACE;

    edds_param_ptr->open_dpb.pLLParameter            = lsas_edds_system_device_open__init_llparam(edds_param_ptr);
    edds_param_ptr->open_dpb.schedulerCycleIO 		 = 1;
    edds_param_ptr->open_dpb.schedulerCycleNRT 		 = 1;
    //edds_param_ptr->open_dpb.RxLimitationPackets_IO  = 0;  // unlimited processed packets per millisecond in IO mode (only limited by bandwidth)
    //edds_param_ptr->open_dpb.RxLimitationPackets_NRT = 0; // unlimited processed packets per millisecond in NRT mode (only limited by bandwidth)

    #ifdef LSAS_EDDS_ASIC_TI
    edds_param_ptr->open_dpb.pLLManagementData       = &lsas_data.edds.hLL; // uninitialized data structure has to be passed
    #else
    lsas_data.edds.hLL = EDDS_LLIF_ALLOC_MNG_DATA(LSAS_EDDS_SINGLE_DEVICE_HNDL);
    edds_param_ptr->open_dpb.pLLManagementData       = lsas_data.edds.hLL;
    #endif
    edds_param_ptr->open_dpb.pLLFunctionTable        = (LSA_VOID_PTR_TYPE) &lsas_data.edds.ll_func_table;  // Packet32LLFuncTable

    #ifdef LLIF_CFG_SWITCH_SUPPORT
        #ifndef EDDS_DONT_USE_MRP
            {
                LSA_UINT32 i;
                edds_param_ptr->open_dpb.SWI.MRPDefaultRoleInstance0    = EDD_MRP_ROLE_CLIENT;
                edds_param_ptr->open_dpb.SWI.MRPSupportedRole           = EDD_MRP_ROLE_CLIENT;
                edds_param_ptr->open_dpb.SWI.MRPSupportedMultipleRole   = EDD_MRP_ROLE_NONE;
                edds_param_ptr->open_dpb.SWI.MRAEnableLegacyMode        =  EDD_MRA_DISABLE_LEGACY_MODE;  /* default, device supports MRA mode 2.0. It's stored only */
                for (i = 0; i < EDDS_MAX_PORT_CNT; i++)
                {
                    /* set port 0, 1 to default ring ports, the rest to 0 */
                    if (i < 2)
                    {
                        edds_param_ptr->open_dpb.SWI.MRPRingPort[i] = EDD_MRP_RING_PORT_DEFAULT;
                    }
                    else
                    {
                        edds_param_ptr->open_dpb.SWI.MRPRingPort[i] = EDD_MRP_NO_RING_PORT;
                    }
                }
            }
        #endif
    #endif

    /* NRT */
    edds_param_ptr->open_dpb.NRT.IO_Configured = LSA_TRUE;
    edds_param_ptr->open_dpb.NRT.FeedInLoadLimitationActive = LSA_TRUE;
    edds_param_ptr->open_dpb.NRT.RxFilterUDP_Broadcast = LSA_FALSE;
    edds_param_ptr->open_dpb.NRT.RxFilterUDP_Unicast = LSA_FALSE;

    edds_param_ptr->open_dpb.NRT.parameterSet[EDDS_DPB_PARAMETERSET_NRT_MODE].RxBufferCntARPIP = 156;       // for NRT frames: ICMP, IGMP TCP UDP IP_OTHER ARP RARP
    edds_param_ptr->open_dpb.NRT.parameterSet[EDDS_DPB_PARAMETERSET_NRT_MODE].RxBufferCntOther = 78;        // for NRT frames: DCP, TSYNC, STDBY, HSR, SINEC_FWL, OTHER, LLDP, MRP, OTHER
    edds_param_ptr->open_dpb.NRT.parameterSet[EDDS_DPB_PARAMETERSET_NRT_MODE].RxBufferCntASRTAlarm = 78;    // for NRT frames: ASRT, UDP_ASRT
    edds_param_ptr->open_dpb.NRT.parameterSet[EDDS_DPB_PARAMETERSET_NRT_MODE].RxBufferCntASRTOther = 78;    // for NRT frames: DCP, UDP_DCP, PTCP
    /*Demanded to be non zero */
    edds_param_ptr->open_dpb.NRT.parameterSet[EDDS_DPB_PARAMETERSET_CRT_MODE].RxBufferCntARPIP = 60;        // for NRT frames: ICMP, IGMP TCP UDP IP_OTHER ARP RARP
    edds_param_ptr->open_dpb.NRT.parameterSet[EDDS_DPB_PARAMETERSET_CRT_MODE].RxBufferCntOther = 30;        // for NRT frames: DCP, TSYNC, STDBY, HSR, SINEC_FWL, OTHER, LLDP, MRP, OTHER
    edds_param_ptr->open_dpb.NRT.parameterSet[EDDS_DPB_PARAMETERSET_CRT_MODE].RxBufferCntASRTAlarm = 30;    // for NRT frames: ASRT, UDP_ASRT
    edds_param_ptr->open_dpb.NRT.parameterSet[EDDS_DPB_PARAMETERSET_CRT_MODE].RxBufferCntASRTOther = 30;    // for NRT frames: DCP, UDP_DCP, PTCP

    /* CSRT */
    edds_param_ptr->open_dpb.CSRT.CycleBaseFactor                 = LSAS_EDDS_CFG_CYCLE_BASE_FACTOR; /*32: = 1ms, 64: = 2ms, 128: =4ms */
    edds_param_ptr->open_dpb.CSRT.ConsumerFrameIDBaseClass1       = LSAS_EDDS_CONSUMER_C1_FRAME_ID_BASE;
    edds_param_ptr->open_dpb.CSRT.ConsumerCntClass1               = LSAS_EDDS_CONSUMER_C1_FRAME_ID_COUNT;
    edds_param_ptr->open_dpb.CSRT.ConsumerFrameIDBaseClass2       = LSAS_EDDS_CONSUMER_C2_FRAME_ID_BASE;
    edds_param_ptr->open_dpb.CSRT.ConsumerCntClass2               = LSAS_EDDS_CONSUMER_C2_FRAME_ID_COUNT;
    edds_param_ptr->open_dpb.CSRT.ProviderCnt                     = LSAS_EDDS_CONSUMER_C1_FRAME_ID_COUNT + LSAS_EDDS_CONSUMER_C2_FRAME_ID_COUNT;
    edds_param_ptr->open_dpb.CSRT.ProviderCycleDropCnt            = 10;

    /* FeatureSupport */
#if !defined(EDDS_DONT_USE_MRP) && defined(LLIF_CFG_SWITCH_SUPPORT)
    edds_param_ptr->open_dpb.FeatureSupport.AdditionalForwardingRulesSupported = EDD_FEATURE_DISABLE;// EDD_FEATURE_ENABLE;   /* device allows forwarding of MRP HSYNC frames */
#endif
    edds_param_ptr->open_dpb.FeatureSupport.ApplicationExist                   = EDD_FEATURE_DISABLE;  /* device is just a HSYNC forwarder */
    //edds_param_ptr->open_dpb.FeatureSupport.ClusterIPSupport                 = EDD_FEATURE_ENABLE;   /* IPClustering is the ability to have 2 IP-addresses, only usable in BSD stack */

    // --------- fill DDB ---------
    LSAS_MEMSET(&edds_param_ptr->open_param, 0, sizeof(EDDS_RQB_DDB_INI_TYPE));
    edds_param_ptr->open_param.hDDB = 0;                                     // will be set
    edds_param_ptr->open_param.pDPB = &edds_param_ptr->open_dpb;            // points to User DPB
    edds_param_ptr->open_param.Cbf  = lsas_cbf_sys_edd_request_lower_done;     // callback function

    // --------- fill RQB ---------
    rqb_ptr->pParam = &edds_param_ptr->open_param;
    rqb_ptr->Service = EDDS_SRV_DEV_OPEN;
    rqb_ptr->comp_id = LSA_COMP_ID_EDDS;
    LSAS_RQB_SET_OPCODE(rqb_ptr, EDD_OPC_SYSTEM);
    LSAS_RQB_SET_HANDLE(rqb_ptr, 0);
    //LSAS_RQB_SET_USERID_UVAR32(rqb_ptr, 0x1234);
    edds_system((EDD_UPPER_RQB_PTR_TYPE)rqb_ptr);
}

//=================================================================================================
// from EddApi_SetupDevice
//=================================================================================================
LSA_VOID lsas_edds_system_device_setup(EDD_RQB_TYPE * rqb_ptr, LSAS_EDD_OPEN_PARA * edds_param_ptr)
{
    LSA_UINT32 i;

    // ----- Fill Setup Param -----
    LSAS_MEMSET(&edds_param_ptr->setup_param, 0, sizeof(EDDS_RQB_DDB_SETUP_TYPE));
    edds_param_ptr->setup_param.hDDB = edds_param_ptr->open_param.hDDB;
    edds_param_ptr->setup_param.pDSB = &edds_param_ptr->setup_dsb;
    edds_param_ptr->setup_param.Cbf  = lsas_cbf_sys_edd_request_lower_done;

    // ----- Fill Setup DSB  -----
    edds_InitDSBWithDefaults (&edds_param_ptr->setup_dsb);

    LSAS_CFG_GET_MAC_ADDR(&edds_param_ptr->setup_dsb.MACAddress[0].MacAdr[0]);
    for (i=0; i < LSAS_CFG_PORT_CNT; i++)
    {
        /* port-specific sourceMAC-address */
        LSAS_CFG_GET_PORT_MAC_ADDR(i,(LSA_UINT8 *) &edds_param_ptr->setup_dsb.MACAddress[i+1].MacAdr[0]);
    }

    // --------- fill RQB ---------
    rqb_ptr->pParam = &edds_param_ptr->setup_param;
    rqb_ptr->Service = EDDS_SRV_DEV_SETUP;
    rqb_ptr->comp_id = LSA_COMP_ID_EDDS;
    LSAS_RQB_SET_OPCODE(rqb_ptr, EDD_OPC_SYSTEM);
    LSAS_RQB_SET_HANDLE(rqb_ptr, 0);
    //LSAS_RQB_SET_USERID_UVAR32(rqb_ptr, 0x1234);
    edds_system((EDD_UPPER_RQB_PTR_TYPE)rqb_ptr);
}

//=================================================================================================
LSA_VOID lsas_edds_req_open(LSA_VOID)
{
    LSA_RESULT              ret_val;

    switch(lsas_data.edds.sm)
    {
        case LSAS_EDDS_SM_CLOSED:
        {
            lsas_data.edds.sm = LSAS_EDDS_SM_ALLOC_MEM;
            // no break
        }

        case LSAS_EDDS_SM_ALLOC_MEM:
        {
            /* first call to this function at startup */
            /* prepare rqb and safe pointer */
            LSAS_EDD_OPEN_PARA    * edd_param_ptr;
            EDD_RQB_TYPE          * edd_sys_rqb_ptr;

            LSAS_ALLOC_LOCAL_MEM( (LSAS_LOCAL_MEM_PTR_TYPE)& edd_param_ptr, sizeof(LSAS_EDD_OPEN_PARA));
            if (edd_param_ptr == 0)
            {
                LSAS_FATAL(LSAS_FATAL_ERR_NULL_PTR)
            }
            lsas_data.edds.param_ptr = edd_param_ptr;

            LSAS_ALLOC_LOCAL_MEM( (LSAS_LOCAL_MEM_PTR_TYPE)& edd_sys_rqb_ptr, sizeof(EDD_RQB_TYPE));
            if (edd_sys_rqb_ptr == 0)
            {
                LSAS_FATAL(LSAS_FATAL_ERR_NULL_PTR);
            }
            lsas_data.edds.sys_rqb_ptr = edd_sys_rqb_ptr;
            LSAS_MEMSET(edd_sys_rqb_ptr, 0, sizeof(EDD_RQB_TYPE));

            lsas_data.edds.sm = LSAS_EDDS_SM_DEVICE_OPEN;
            // no break
        }

        case LSAS_EDDS_SM_DEVICE_OPEN:
        {

            lsas_data.edd.request_processed = 0;

            lsas_edds_system_device_open(lsas_data.edds.sys_rqb_ptr, lsas_data.edds.param_ptr);

            lsas_cfg_edd_system_req_check();

            lsas_data.edds.sm = LSAS_EDDS_SM_WAIT_OPEN_DONE;
            break;
        }

        case LSAS_EDDS_SM_WAIT_OPEN_DONE:
        {
            // check device open results
            if( LSAS_RQB_GET_RESPONSE(lsas_data.edds.sys_rqb_ptr) != EDD_STS_OK )
            {
                LSAS_FATAL(0);
            }
            else
            {
                lsas_com_set_hxdb(lsas_data.edds.param_ptr->open_param.hDDB);
            }
            lsas_data.edds.sm = LSAS_EDDS_SM_DEVICE_SETUP;
            // no break
        }

        case LSAS_EDDS_SM_DEVICE_SETUP:
        {
            lsas_data.edds.h_io_buffer_management = lsas_data.edds.param_ptr->open_dpb.CSRT.hIOBufferManagement;
            lsas_data.edd.request_processed = 0;

            lsas_edds_system_device_setup(lsas_data.edds.sys_rqb_ptr, lsas_data.edds.param_ptr);

            lsas_cfg_edd_system_req_check();

            lsas_data.edds.sm = LSAS_EDDS_SM_WAIT_SETUP_DONE;
            break;
        }

        case LSAS_EDDS_SM_WAIT_SETUP_DONE:
        {
            LSA_UINT16        ret_val = 0;
            LSA_USER_ID_TYPE user_id;

            // check setup results
            if( LSAS_RQB_GET_RESPONSE(lsas_data.edds.sys_rqb_ptr) != EDD_STS_OK )
            {
                LSAS_FATAL(0);
            }

            // start EDDS-scheduler. edds_scheduler() shall be executed cyclically. It may interrupt
            // edds_request() calls and may not be interrupted itself by an edds dependent task (including RQB-context,
            // see "EDDS Operational Modes", chapter about cyclic IO mode)
            // -> running edds_scheduler() in same task as edds_request() seems wise
            LSAS_ALLOC_TIMER(&ret_val, &lsas_data.edds.timer_id, LSA_TIMER_TYPE_CYCLIC, LSA_TIME_BASE_1MS, lsas_edds_scheduler_timer_arb_task, LSA_COMP_ID_EDDS);
            if (ret_val == LSA_RET_ERR_PARAM)
            {
                LSAS_FATAL(0);
            }
            user_id.uvar32 = 0;
            LSAS_START_TIMER(&ret_val, lsas_data.edds.timer_id , user_id, 1, LSA_COMP_ID_EDDS);
            if (ret_val == LSA_RET_ERR_PARAM)
            {
                LSAS_FATAL(0);
            }

            lsas_data.edds.sm = LSAS_EDDS_SM_FREE_MEM;
            // no break
        }

        case LSAS_EDDS_SM_FREE_MEM:
        {

            LSAS_FREE_LOCAL_MEM(&ret_val, lsas_data.edds.param_ptr);
            if( ret_val != LSA_RET_OK )
            {
                LSAS_FATAL(LSAS_FATAL_ERR_MEM_FREE);
            }

            LSAS_FREE_LOCAL_MEM(&ret_val, lsas_data.edds.sys_rqb_ptr);
            if( ret_val != LSA_RET_OK )
            {
                LSAS_FATAL(LSAS_FATAL_ERR_MEM_FREE);
            }

            LSAS_CFG_INIT_COM_RAM();

            LSAS_CFG_EDD_OPEN_DONE();

            lsas_data.edds.sm = LSAS_EDDS_SM_DONE;
            break;
        }

        case LSAS_EDDS_SM_NOT_USED:
        default:
        {
            LSAS_FATAL(lsas_data.edds.sm);
        }
    }
}


#endif

LSA_VOID lsas_edd_req_open()
{
#ifdef LSAS_CFG_USE_EDDI
    lsas_eddi_req_open();
#endif
#ifdef LSAS_CFG_USE_EDDP
    lsas_eddp_req_open();
#endif
#ifdef LSAS_CFG_USE_EDDS
    lsas_edds_req_open();
#endif
}


/*** end of file *************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
