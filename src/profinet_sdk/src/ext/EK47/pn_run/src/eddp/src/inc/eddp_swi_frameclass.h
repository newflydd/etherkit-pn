#ifndef EDDP_SWI_FRAMECLASS_H            /* ----- reinclude-protection ----- */
#define EDDP_SWI_FRAMECLASS_H

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
/*  F i l e               &F: eddp_swi_frameclass.h                     :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Headerfile for SWITCH frameclass configuration                           */
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

/*===========================================================================*/
/*                                 defines                                   */
/*===========================================================================*/
//Andi: PNIP supports a minimum of 20 frameclasses. It is possible that this 
//value will change during ASIC development.
#define  EDDP_SWI_FRAMECLASS_SIZE   20


/*===========================================================================*/
/*                                 macros                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/
typedef struct _EDDP_SWI_FRAMECLASS_TYPE    EDDP_LOCAL_MEM_ATTR *   EDDP_SWI_FRAMECLASS_PTR_TYPE;
typedef struct _EDDP_SWI_FRAMECLASS_TYPE
{
    LSA_UINT16      EtherType;
    LSA_UINT8       CompType;
    LSA_UINT8       Valid;
    LSA_UINT16      FrameID_low;
    LSA_UINT16      FrameID_high;
    LSA_UINT8       FrameHandling;
    LSA_UINT8       FrameHandling_Select;
    LSA_UINT8       UseType;
    LSA_UINT8       UseType_Select;
    LSA_UINT8       DefaultTraffic_Class;
    LSA_UINT8       DefaultTraffic_Select;
    LSA_UINT8       FrameTraffic_Select;
    LSA_UINT8       Redundancy;
    LSA_UINT8       Fragmentation;
    LSA_UINT8       BridgeDelay_Enable; /* only used in PN-IP Rev1 */
    LSA_UINT8       Learning_Disable;
    LSA_UINT8       BitType_Select;
    LSA_UINT8       APIType;
    LSA_UINT8       APIType_Select;
    LSA_UINT8       ResourceType;
//  LSA_UINT8       ResourceType_VLAN;  // new feature in PNIO-Spec, which is not supported by EDDP anyway up to now //ResourceType_VLANSupport
    LSA_UINT8       ResourceType_Select;
    LSA_UINT8       PriorityMapping;
    LSA_UINT8       PriorityMapping_Select;
} EDDP_SWI_FRAMECLASS_TYPE;


/*===========================================================================*/
/*                                  data                                     */
/*===========================================================================*/

#if ( (defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT)) || (defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)) )
LSA_EXTERN const  EDDP_SWI_FRAMECLASS_TYPE  EDDP_SWI_frameclass_array_ERTEC200P [EDDP_SWI_FRAMECLASS_SIZE];
#endif

LSA_EXTERN const  LSA_UINT32  EDDP_SWI_frameclass_0_register_mapping [EDDP_SWI_FRAMECLASS_SIZE];

LSA_EXTERN const  LSA_UINT32  EDDP_SWI_frameclass_1_register_mapping [EDDP_SWI_FRAMECLASS_SIZE];

LSA_EXTERN const  LSA_UINT32  EDDP_SWI_frameclass_2_register_mapping [EDDP_SWI_FRAMECLASS_SIZE];

/*===========================================================================*/
/*                                prototyping                                */
/*===========================================================================*/
LSA_VOID  EDDP_LOCAL_FCT_ATTR EDDP_SWI_write_frameclass (
	EDDP_LOCAL_DDB_PTR_TYPE       pDDB,
    EDDP_SWI_FRAMECLASS_PTR_TYPE  pFrameclass_Array );

/*****************************************************************************/
/*  end of file EDDP_SWI_FRAMECLASS.H                                        */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_SWI_FRAMECLASS_H */
