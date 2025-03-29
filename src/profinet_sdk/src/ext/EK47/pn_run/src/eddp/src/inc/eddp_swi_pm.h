#ifndef EDDP_SWI_PM_H                    /* ----- reinclude-protection ----- */
#define EDDP_SWI_PM_H

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
/*  F i l e               &F: eddp_swi_pm.h                             :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Headerfile for SWITCH port map functions                                 */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  11.11.08    AB    initial version                                        */
#endif
/*****************************************************************************/

/*===========================================================================*/
/*                                  data                                     */
/*===========================================================================*/

/*===========================================================================*/
/*                                 macros                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/


/*===========================================================================*/
/*                                prototyping                                */
/*===========================================================================*/


EDD_RSP  EDDP_LOCAL_FCT_ATTR EDDP_SWI_portmap_check ( 
	EDDP_LOCAL_DDB_PTR_TYPE  pDDB,
	EDDP_UPPER_DPB_PTR_TYPE  pDPB );

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_SWI_get_HwPortID ( 
	EDDP_LOCAL_DDB_PTR_TYPE  pDDB,
	LSA_UINT32               PortID );

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_SWI_get_PortID_and_IF ( 
	EDDP_LOCAL_DDB_PTR_TYPE  pDDB,
	LSA_UINT32               HwPortID,
	LSA_UINT32		        *pIFNumber );	

/*****************************************************************************/
/*  end of file EDDP_SWI_PM.H                                                */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_SWI_PM_H */
