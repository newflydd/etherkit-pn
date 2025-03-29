#ifndef EDDP_PHY_H                       /* ----- reinclude-protection ----- */
#define EDDP_PHY_H

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
/*  F i l e               &F: eddp_phy.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  28.04.11    SF    initial version.                                       */
#endif
/*****************************************************************************/

/*=============================== Defines ===================================*/	

// Config which port operates in MDI or MDI-X mode when doing NO auto-neg.
extern LSA_UINT8    g_EDDP_PHY_MDI_X_MAPPING             [EDDP_CFG_MAX_DEVICES][EDDP_MAX_PORT_CNT];
extern LSA_UINT8    g_EDDP_PHY_ISWIRELESS                [EDDP_CFG_MAX_DEVICES][EDDP_MAX_PORT_CNT];
//extern LSA_UINT16   g_EDDP_PHY_MAUTYPE                 [EDDP_CFG_MAX_DEVICES][EDDP_MAX_PORT_CNT];
extern LSA_UINT8    g_EDDP_PHY_MEDIATYPE                 [EDDP_CFG_MAX_DEVICES][EDDP_MAX_PORT_CNT];
extern LSA_UINT8    g_EDDP_PHY_OPTICALTRANSTYPE          [EDDP_CFG_MAX_DEVICES][EDDP_MAX_PORT_CNT];
extern LSA_UINT8    g_EDDP_PHY_FXTRANSCEIVERTYPE         [EDDP_CFG_MAX_DEVICES][EDDP_MAX_PORT_CNT];
extern LSA_UINT8    g_EDDP_PHY_LINK_SPEED_MODE_CONFIGURED[EDDP_CFG_MAX_DEVICES][EDDP_MAX_PORT_CNT];

/*---------------------------------------------------------------------------*/
/* Supported MAUTypes														 */
/*---------------------------------------------------------------------------*/
// ***** COPPER, RADIO 
#define EDDP_PHY_MAUTYPE_10BASETXHD			    0x000A
#define EDDP_PHY_MAUTYPE_10BASETXFD			    0x000B
#define EDDP_PHY_MAUTYPE_100BASETXHD		    0x000F 
#define EDDP_PHY_MAUTYPE_100BASETXFD		    0x0010
// ***** FIBER
#define EDDP_PHY_MAUTYPE_100BASEFXHD		    0x0011
#define EDDP_PHY_MAUTYPE_100BASEFXFD            0x0012
#define EDDP_PHY_MAUTYPE_100BASEPXFD            0x0036

EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_MapAutonegSpeedDuplexityToLinkSpeedModeConfigured(
    const LSA_UINT8                 Autoneg,
    const LSA_UINT32                Speed,
    const LSA_UINT32                Duplexity,
    LSA_UINT8                       *pLinkSpeedModeConfigured);


/*****************************************************************************/
/*  end of file EDDP_PHY.H                                                   */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_PHY_H */
