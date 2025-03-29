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
/*  F i l e               &F: eddp_swi_frameclass.c                     :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP SWITCH frameclass configuration functions   */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  05.03.09    AB    initial version                                        */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID   84                 
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_SWI_FRAMECLASS */

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


/* ------------------------------------------------------------------------ */
/*  FrameClass-Table                                                        */
/* ------------------------------------------------------------------------ */
/*  ET          : EtherType                                                 */
/*  CT          : CompType ( 0: = | 1: < )                                  */
/*  V           : Valid                                                     */
/*  FIDlo       : FrameID_low                                               */
/*  FIDhi       : FrameID_high                                              */
/*  FH(S)       : FrameHandling (_Select)                                   */
/*  UT(S)       : UseType (_Select)                                         */
/*  DTC/DTS     : DefaultTraffic_Class / DefaultTraffic_Select              */
/*  FTS         : FrameTraffic_Select                                       */
/*  R           : Redundancy                                                */
/*  F           : Fragmentation                                             */
/*  BD          : BridgeDelay_Enable (only used in PN-IP Rev1)              */
/*  LD          : Learning_Disable                                          */
/*  BS          : BitType_Select                                            */
/*  APIT(S)     : APIType (_Select)                                         */
/*  RT(S)       : ResourceType (_Select)                                    */
/*  PM(S)       : PriorityMapping (_Select)                                 */
/* ------------------------------------------------------------------------ */
/*  last update : 2014-03-12                                                */
/* ------------------------------------------------------------------------ */
#if ( (defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT)) || (defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)) )
const   EDDP_SWI_FRAMECLASS_TYPE    EDDP_SWI_frameclass_array_ERTEC200P [EDDP_SWI_FRAMECLASS_SIZE] = 
{
//     ET,     CT, V, FIDlo,  FIDhi,  (FH),                             FHS, (UT),                              UTS, DTC, DTS, FTS, R, F, BD, LD, BS,  (APIT),                        APITS,   (RT),                            /*RTV,*/ RTS, PM, PMS             //ResourceType_VLANSupport
/*1*/ {0x8892, 0,  1, 0x0100, 0x06FF, (EDDP_MCFCT_FRAME_HANDLING_RT_H), 1,   (EDDP_MCFCT_USE_TYPE_STANDARD),    0,   0,   0,   0,   0, 0, 0,  0,  0x0, (EDDP_MCFCT_API_TYPE_RT_CT),   0,       (EDDP_MCFCT_RESOURCE_TYPE_RT_H), /*0,*/   0,   0,  0     },
/*2*/ {0x8892, 0,  1, 0x0700, 0x0FFF, (EDDP_MCFCT_FRAME_HANDLING_RT_H), 1,   (EDDP_MCFCT_USE_TYPE_STANDARD),    0,   0,   0,   0,   1, 0, 0,  1,  0x0, (EDDP_MCFCT_API_TYPE_RT_CT),   0,       (EDDP_MCFCT_RESOURCE_TYPE_RT_H), /*0,*/   0,   0,  0     },
/*3*/ {0x8892, 0,  1, 0x8000, 0xBFFF, (EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,   (EDDP_MCFCT_USE_TYPE_STANDARD),    0,   0,   0,   0,   0, 0, 0,  0,  0x0, (EDDP_MCFCT_API_TYPE_RT_SF),   0,       (EDDP_MCFCT_RESOURCE_TYPE_RT_L), /*0,*/   0,   0,  0     },
/*4*/ {0x8892, 0,  1, 0xC000, 0xFBFF, (EDDP_MCFCT_FRAME_HANDLING_NRT),  0,   (EDDP_MCFCT_USE_TYPE_STANDARD),    0,   0,   0,   0,   0, 0, 0,  0,  0x0, (EDDP_MCFCT_API_TYPE_RT_SF),   0,       (EDDP_MCFCT_RESOURCE_TYPE_RT_L), /*0,*/   0,   0,  0     },
/*5*/ {0x8892, 0,  1, 0xFC01, 0xFC01, (EDDP_MCFCT_FRAME_HANDLING_RTA),  0,   (EDDP_MCFCT_USE_TYPE_STANDARD),    0,   0,   0,   0,   0, 1, 0,  0,  0x0, (EDDP_MCFCT_API_TYPE_ALARM),   0,       (EDDP_MCFCT_RESOURCE_TYPE_RTA),  /*0,*/   0,   0,  0     },
/*6*/ {0x8892, 0,  1, 0xFE01, 0xFE01, (EDDP_MCFCT_FRAME_HANDLING_RTA),  0,   (EDDP_MCFCT_USE_TYPE_STANDARD),    0,   0,   0,   0,   0, 1, 0,  0,  0x0, (EDDP_MCFCT_API_TYPE_ALARM),   0,       (EDDP_MCFCT_RESOURCE_TYPE_RTA),  /*0,*/   0,   0,  0     },
/*7*/ {0x8892, 0,  1, 0xFEFC, 0xFEFF, (EDDP_MCFCT_FRAME_HANDLING_NRT),  0,   (EDDP_MCFCT_USE_TYPE_STANDARD),    0,   0,   0,   0,   0, 1, 0,  0,  0x0, (EDDP_MCFCT_API_TYPE_DCP),     0,       (EDDP_MCFCT_RESOURCE_TYPE_OTHER),/*0,*/   0,   0,  0     },
/*8*/ {0x8892, 0,  1, 0xFF40, 0xFF43, (EDDP_MCFCT_FRAME_HANDLING_NRT),  0,   (EDDP_MCFCT_USE_TYPE_PTCP_DELAY),  0,   0,   0,   0,   0, 0, 0,  1,  0x0, (EDDP_MCFCT_API_TYPE_PTCP),    0,       (EDDP_MCFCT_RESOURCE_TYPE_PTCP), /*0,*/   0,   0,  0     },
/*9*/ {0x8892, 0,  1, 0xFF42, 0xFF42, (EDDP_MCFCT_FRAME_HANDLING_NRT),  0,   (EDDP_MCFCT_USE_TYPE_STANDARD),    0,   0,   0,   0,   0, 0, 0,  1,  0x0, (EDDP_MCFCT_API_TYPE_PTCP),    0,       (EDDP_MCFCT_RESOURCE_TYPE_PTCP), /*0,*/   0,   0,  0     },
/*10*/{0x0806, 0,  1, 0x0000, 0xFFFF, (EDDP_MCFCT_FRAME_HANDLING_NRT),  0,   (EDDP_MCFCT_USE_TYPE_STANDARD),    0,   0,   0,   0,   0, 1, 0,  0,  0x0, (EDDP_MCFCT_API_TYPE_ARP),     0,       (EDDP_MCFCT_RESOURCE_TYPE_OTHER),/*0,*/   0,   0,  0     },
/*11*/{0x88CC, 0,  1, 0x0000, 0xFFFF, (EDDP_MCFCT_FRAME_HANDLING_NRT),  0,   (EDDP_MCFCT_USE_TYPE_STANDARD),    0,   0,   0,   0,   0, 1, 0,  1,  0x0, (EDDP_MCFCT_API_TYPE_LLDP),    0,       (EDDP_MCFCT_RESOURCE_TYPE_LLDP), /*0,*/   0,   0,  0     },
/*12*/{0x0800, 0,  1, 0x0000, 0xFFFF, (EDDP_MCFCT_FRAME_HANDLING_NRT),  0,   (EDDP_MCFCT_USE_TYPE_STANDARD),    0,   0,   0,   0,   0, 1, 0,  0,  0x0, (EDDP_MCFCT_API_TYPE_IP),      0,       (EDDP_MCFCT_RESOURCE_TYPE_OTHER),/*0,*/   0,   0,  0     },
/*13*/{0x88E3, 0,  1, 0x0000, 0xFFFF, (EDDP_MCFCT_FRAME_HANDLING_NRT),  0,   (EDDP_MCFCT_USE_TYPE_STANDARD),    0,   0,   0,   0,   0, 0, 0,  1,  0x0, (EDDP_MCFCT_API_TYPE_MRP),     0,       (EDDP_MCFCT_RESOURCE_TYPE_MRP),  /*0,*/   0,   0,  0     },
/*14*/{0x0600, 1,  1, 0x0000, 0xFFFF, (EDDP_MCFCT_FRAME_HANDLING_NRT),  0,   (EDDP_MCFCT_USE_TYPE_STANDARD),    0,   0,   0,   0,   0, 1, 0,  0,  0x0, (EDDP_MCFCT_API_TYPE_LEN),     0,       (EDDP_MCFCT_RESOURCE_TYPE_OTHER),/*0,*/   0,   0,  0     },
/*15*/{0,      0,  0,       0,     0,                               0,  0,                                0,    0,   0,   0,   0,   0, 0, 0,  1,    0,                         0,     0,       0,                               /*0,*/   0,   0,  0     },
/*16*/{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*0,*/0,0,0},
/*17*/{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*0,*/0,0,0},
/*18*/{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*0,*/0,0,0},
/*19*/{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*0,*/0,0,0},
/*20*/{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*0,*/0,0,0}
};
#endif

const   EDDP_SWI_FRAMECLASS_TYPE    EDDP_SWI_frameclass_default = 
{
//   ET,     CT, V, FIDlo,  FIDhi,  (FH),                            FHS, (UT),                           UTS, DTC, DTS, FTS, R, F, BDE, LD, BTS, (APIT),                       APITS, RT, /*RTV,*/ RTS, PM, PMS
     0x0000, 0,  0, 0,      0,      (EDDP_MCFCT_FRAME_HANDLING_NRT), 1,   (EDDP_MCFCT_USE_TYPE_STANDARD), 1,   0,   1,   0,   0, 1, 0,   0,  0x3, (EDDP_MCFCT_API_TYPE_OTHER),  1,     0,  /*0,*/   1,   0,  1
};

const  LSA_UINT32  EDDP_SWI_frameclass_0_register_mapping [EDDP_SWI_FRAMECLASS_SIZE] =
{
    PNIP_REG_FRAMECLASS_0_1,
    PNIP_REG_FRAMECLASS_0_2,
    PNIP_REG_FRAMECLASS_0_3,
    PNIP_REG_FRAMECLASS_0_4,
    PNIP_REG_FRAMECLASS_0_5,
    PNIP_REG_FRAMECLASS_0_6,
    PNIP_REG_FRAMECLASS_0_7,
    PNIP_REG_FRAMECLASS_0_8,
    PNIP_REG_FRAMECLASS_0_9,
    PNIP_REG_FRAMECLASS_0_10,
    PNIP_REG_FRAMECLASS_0_11,
    PNIP_REG_FRAMECLASS_0_12,
    PNIP_REG_FRAMECLASS_0_13,
    PNIP_REG_FRAMECLASS_0_14,
    PNIP_REG_FRAMECLASS_0_15,
    PNIP_REG_FRAMECLASS_0_16,
    PNIP_REG_FRAMECLASS_0_17,
    PNIP_REG_FRAMECLASS_0_18,
    PNIP_REG_FRAMECLASS_0_19,
    PNIP_REG_FRAMECLASS_0_20
};

const  LSA_UINT32  EDDP_SWI_frameclass_1_register_mapping [EDDP_SWI_FRAMECLASS_SIZE] =
{
    PNIP_REG_FRAMECLASS_1_1,
    PNIP_REG_FRAMECLASS_1_2,
    PNIP_REG_FRAMECLASS_1_3,
    PNIP_REG_FRAMECLASS_1_4,
    PNIP_REG_FRAMECLASS_1_5,
    PNIP_REG_FRAMECLASS_1_6,
    PNIP_REG_FRAMECLASS_1_7,
    PNIP_REG_FRAMECLASS_1_8,
    PNIP_REG_FRAMECLASS_1_9,
    PNIP_REG_FRAMECLASS_1_10,
    PNIP_REG_FRAMECLASS_1_11,
    PNIP_REG_FRAMECLASS_1_12,
    PNIP_REG_FRAMECLASS_1_13,
    PNIP_REG_FRAMECLASS_1_14,
    PNIP_REG_FRAMECLASS_1_15,
    PNIP_REG_FRAMECLASS_1_16,
    PNIP_REG_FRAMECLASS_1_17,
    PNIP_REG_FRAMECLASS_1_18,
    PNIP_REG_FRAMECLASS_1_19,
    PNIP_REG_FRAMECLASS_1_20
};

const  LSA_UINT32  EDDP_SWI_frameclass_2_register_mapping [EDDP_SWI_FRAMECLASS_SIZE] =
{
    PNIP_REG_FRAMECLASS_2_1,
    PNIP_REG_FRAMECLASS_2_2,
    PNIP_REG_FRAMECLASS_2_3,
    PNIP_REG_FRAMECLASS_2_4,
    PNIP_REG_FRAMECLASS_2_5,
    PNIP_REG_FRAMECLASS_2_6,
    PNIP_REG_FRAMECLASS_2_7,
    PNIP_REG_FRAMECLASS_2_8,
    PNIP_REG_FRAMECLASS_2_9,
    PNIP_REG_FRAMECLASS_2_10,
    PNIP_REG_FRAMECLASS_2_11,
    PNIP_REG_FRAMECLASS_2_12,
    PNIP_REG_FRAMECLASS_2_13,
    PNIP_REG_FRAMECLASS_2_14,
    PNIP_REG_FRAMECLASS_2_15,
    PNIP_REG_FRAMECLASS_2_16,
    PNIP_REG_FRAMECLASS_2_17,
    PNIP_REG_FRAMECLASS_2_18,
    PNIP_REG_FRAMECLASS_2_19,
    PNIP_REG_FRAMECLASS_2_20
};
/*===========================================================================*/
/*                                 functions                                 */
/*===========================================================================*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname         : EDDP_SWI_write_frameclass                       +*/
/*+  Input/Output         : EDDP_LOCAL_DDB_PTR_TYPE       pDDB              +*/
/*+						  :	EDDP_SWI_FRAMECLASS_PTR_TYPE  pFrameclass_Array +*/
/*+                                                                         +*/
/*+  Result               :    LSA_VOID                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock           (in)  +*/
/*+  pFrameclass_Array  : Pointer to array of frameclass information  (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:  This function writes the static frameclass information   +*/
/*+ 			   from an array (on Host/ARM) to the corresponding         +*/
/*+                PNIP registers.                                          +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID  EDDP_LOCAL_FCT_ATTR EDDP_SWI_write_frameclass (
	EDDP_LOCAL_DDB_PTR_TYPE       pDDB,
    EDDP_SWI_FRAMECLASS_PTR_TYPE  pFrameclass_Array )
{
    LSA_UINT32      i;
    LSA_UINT32      reg_val;

    for ( i=0; i < EDDP_SWI_FRAMECLASS_SIZE; i++ )
    {
        //clear all frameclass entries
        EDDP_HAL_REG32_WRITE(pDDB, EDDP_SWI_frameclass_0_register_mapping[i], 0);
        EDDP_HAL_REG32_WRITE(pDDB, EDDP_SWI_frameclass_1_register_mapping[i], 0);
        EDDP_HAL_REG32_WRITE(pDDB, EDDP_SWI_frameclass_2_register_mapping[i], 0);
    }//for

    for ( i=0; i < EDDP_SWI_FRAMECLASS_SIZE; i++ )
    {
        //special handling for DLR/EIP
        LSA_UINT16 EtherType = pFrameclass_Array[i].EtherType;
        LSA_UINT8  Fragmentation = pFrameclass_Array[i].Fragmentation;

        if ((EDD_FEATURE_ENABLE == pDDB->FeatureSupport.DLRExtensions)
            && (EDDP_MCFCT_API_TYPE_MRP == pFrameclass_Array[i].APIType))
        {
            //remap Ethertype 
            EtherType     = EDDP_SWI_ETHERTYPE_DLR;
            Fragmentation = 1;

            //APIType/ResourceType stay "MRP"!
        }
            
        // merge all values for PNIP_REG_FRAMECLASS_2_X register
        reg_val = ((((LSA_UINT32)pFrameclass_Array[i].PriorityMapping_Select) << PNIP_REG_FRAMECLASS_2_1_SHFT_PM_SELECT)              |
                   (((LSA_UINT32)pFrameclass_Array[i].PriorityMapping)        << PNIP_REG_FRAMECLASS_2_1_SHFT_PM)                     |
                   (((LSA_UINT32)pFrameclass_Array[i].ResourceType_Select)    << PNIP_REG_FRAMECLASS_2_1_SHFT_RESOURCETYPE_SELECT)    |
//                 (((LSA_UINT32)pFrameclass_Array[i].ResourceType_VLAN)      << PNIP_R2_REG_FRAMECLASS_2_1_SHFT_RESOURCETYPE_VLAN)   |     // see def of EDDP_SWI_FRAMECLASS_TYPE  attention: only for revision 2 //ResourceType_VLANSupport
                   (((LSA_UINT32)pFrameclass_Array[i].ResourceType)           << PNIP_REG_FRAMECLASS_2_1_SHFT_RESOURCETYPE)           |
                   (((LSA_UINT32)pFrameclass_Array[i].APIType_Select)         << PNIP_REG_FRAMECLASS_2_1_SHFT_APITYPE_SELECT)         |
                   (((LSA_UINT32)pFrameclass_Array[i].APIType)                << PNIP_REG_FRAMECLASS_2_1_SHFT_APITYPE)                |
                   (((LSA_UINT32)pFrameclass_Array[i].BitType_Select)         << PNIP_REG_FRAMECLASS_2_1_SHFT_BITTYPE_SELECT)         |
                   (((LSA_UINT32)pFrameclass_Array[i].Learning_Disable)       << PNIP_REG_FRAMECLASS_2_1_SHFT_LEARNING_DISABLE)       |
                   (((LSA_UINT32)Fragmentation)                               << PNIP_REG_FRAMECLASS_2_1_SHFT_FRAGMENTATION)          |
                   (((LSA_UINT32)pFrameclass_Array[i].Redundancy)             << PNIP_REG_FRAMECLASS_2_1_SHFT_REDUNDANCY)             |
                   (((LSA_UINT32)pFrameclass_Array[i].FrameTraffic_Select)    << PNIP_REG_FRAMECLASS_2_1_SHFT_FRAMETRAFFIC_SELECT)    |
                   (((LSA_UINT32)pFrameclass_Array[i].DefaultTraffic_Select)  << PNIP_REG_FRAMECLASS_2_1_SHFT_DEFAULTTRAFFIC_SELECT)  |
                   (((LSA_UINT32)pFrameclass_Array[i].DefaultTraffic_Class)   << PNIP_REG_FRAMECLASS_2_1_SHFT_DEFAULTTRAFFIC_CLASS)   |
                   (((LSA_UINT32)pFrameclass_Array[i].UseType_Select)         << PNIP_REG_FRAMECLASS_2_1_SHFT_USETYPE_SELECT)         |
                   (((LSA_UINT32)pFrameclass_Array[i].UseType)                << PNIP_REG_FRAMECLASS_2_1_SHFT_USETYPE)                |
                   (((LSA_UINT32)pFrameclass_Array[i].FrameHandling_Select)   << PNIP_REG_FRAMECLASS_2_1_SHFT_FRAMEHANDLING_SELECT)   |
                   (((LSA_UINT32)pFrameclass_Array[i].FrameHandling)          << PNIP_REG_FRAMECLASS_2_1_SHFT_FRAMEHANDLING)              );
        // write PNIP_REG_FRAMECLASS_2_X register
        EDDP_HAL_REG32_WRITE(pDDB, EDDP_SWI_frameclass_2_register_mapping[i], reg_val); 
        
        // merge all values for PNIP_REG_FRAMECLASS_1_X register
        reg_val = ((((LSA_UINT32)pFrameclass_Array[i].FrameID_high) << PNIP_REG_FRAMECLASS_1_2_SHFT_FRAMEID_HIGH)  |
                   (((LSA_UINT32)pFrameclass_Array[i].FrameID_low)  << PNIP_REG_FRAMECLASS_1_2_SHFT_FRAMEID_LOW)     );
        // write PNIP_REG_FRAMECLASS_1_X
        EDDP_HAL_REG32_WRITE(pDDB, EDDP_SWI_frameclass_1_register_mapping[i], reg_val);    
        
        // merge all values for PNIP_REG_FRAMECLASS_0_X register
        reg_val = ((((LSA_UINT32)pFrameclass_Array[i].Valid)     << PNIP_REG_FRAMECLASS_0_1_SHFT_VALID)     |
                   (((LSA_UINT32)pFrameclass_Array[i].CompType)  << PNIP_REG_FRAMECLASS_0_1_SHFT_COMPTYPE)  |
                   (((LSA_UINT32)EtherType)                      << PNIP_REG_FRAMECLASS_0_1_SHFT_ETHERTYPE)   );
        // write PNIP_REG_FRAMECLASS_0_X register
        EDDP_HAL_REG32_WRITE(pDDB, EDDP_SWI_frameclass_0_register_mapping[i], reg_val);
    }//for

    // merge all values for PNIP_REG_FRAMECLASS_DEFAULT register
    reg_val = ((((LSA_UINT32)EDDP_SWI_frameclass_default.PriorityMapping_Select) << PNIP_REG_FRAMECLASS_2_1_SHFT_PM_SELECT)              |
               (((LSA_UINT32)EDDP_SWI_frameclass_default.PriorityMapping)        << PNIP_REG_FRAMECLASS_2_1_SHFT_PM)                     |
               (((LSA_UINT32)EDDP_SWI_frameclass_default.ResourceType_Select)    << PNIP_REG_FRAMECLASS_2_1_SHFT_RESOURCETYPE_SELECT)    |
//             (((LSA_UINT32)pFrameclass_Array[i].ResourceType_VLAN)             << PNIP_R2_REG_FRAMECLASS_2_1_SHFT_RESOURCETYPE_VLAN)   |     // see def of EDDP_SWI_FRAMECLASS_TYPE  attention: only for revision 2 //ResourceType_VLANSupport
               (((LSA_UINT32)EDDP_SWI_frameclass_default.ResourceType)           << PNIP_REG_FRAMECLASS_2_1_SHFT_RESOURCETYPE)           |
               (((LSA_UINT32)EDDP_SWI_frameclass_default.APIType_Select)         << PNIP_REG_FRAMECLASS_2_1_SHFT_APITYPE_SELECT)         |
               (((LSA_UINT32)EDDP_SWI_frameclass_default.APIType)                << PNIP_REG_FRAMECLASS_2_1_SHFT_APITYPE)                |
               (((LSA_UINT32)EDDP_SWI_frameclass_default.BitType_Select)         << PNIP_REG_FRAMECLASS_2_1_SHFT_BITTYPE_SELECT)         |
               (((LSA_UINT32)EDDP_SWI_frameclass_default.Learning_Disable)       << PNIP_REG_FRAMECLASS_2_1_SHFT_LEARNING_DISABLE)       |
               (((LSA_UINT32)EDDP_SWI_frameclass_default.Fragmentation)          << PNIP_REG_FRAMECLASS_2_1_SHFT_FRAGMENTATION)          |
               (((LSA_UINT32)EDDP_SWI_frameclass_default.Redundancy)             << PNIP_REG_FRAMECLASS_2_1_SHFT_REDUNDANCY)             |
               (((LSA_UINT32)EDDP_SWI_frameclass_default.FrameTraffic_Select)    << PNIP_REG_FRAMECLASS_2_1_SHFT_FRAMETRAFFIC_SELECT)    |
               (((LSA_UINT32)EDDP_SWI_frameclass_default.DefaultTraffic_Select)  << PNIP_REG_FRAMECLASS_2_1_SHFT_DEFAULTTRAFFIC_SELECT)  |
               (((LSA_UINT32)EDDP_SWI_frameclass_default.DefaultTraffic_Class)   << PNIP_REG_FRAMECLASS_2_1_SHFT_DEFAULTTRAFFIC_CLASS)   |
               (((LSA_UINT32)EDDP_SWI_frameclass_default.UseType_Select)         << PNIP_REG_FRAMECLASS_2_1_SHFT_USETYPE_SELECT)         |
               (((LSA_UINT32)EDDP_SWI_frameclass_default.UseType)                << PNIP_REG_FRAMECLASS_2_1_SHFT_USETYPE)                |
               (((LSA_UINT32)EDDP_SWI_frameclass_default.FrameHandling_Select)   << PNIP_REG_FRAMECLASS_2_1_SHFT_FRAMEHANDLING_SELECT)   |
               (((LSA_UINT32)EDDP_SWI_frameclass_default.FrameHandling)          << PNIP_REG_FRAMECLASS_2_1_SHFT_FRAMEHANDLING)              );
    //set FrameClass_Default register
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_FRAMECLASS_DEFAULT, reg_val);
}

/*****************************************************************************/
/*  end of file eddp_swi_frameclass.c                                        */
/*****************************************************************************/
