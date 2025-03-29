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
/*  F i l e               &F: eddp_phy_nsc_out.c                        :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:													 */
/*                                                                           */
/*  This file contains sample implementations for PHY-specific functions	 */
/*  that have to be provided by system adaption.							 */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  28.04.11    SF    initial version                                        */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID   114
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_PHY_NSC_OUT */

#include "eddp_inc.h"
#include "eddp_int.h"
#include "eddp_phy.h"
#include "eddp_phy_nsc.h"

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)

#ifdef EDDP_CFG_PHY_NSC_SUPPORT





#ifdef EDDP_CFG_PHY_NSC_MEDIA_TYPE_INTERNAL
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_PHY_NSC_GET_MEDIA_TYPE                 +*/
/*+  Input/Output          :    EDDP_HANDLE         hDDB                    +*/
/*+                        :    EDDP_SYS_HANDLE     hSysDev                 +*/
/*+                        :    LSA_UINT32          HwPortID                +*/
/*+                        :    LSA_UINT8           *pMediaType             +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+   hDDB       : EDDP Handle                                         (in) +*/
/*+   hSysDev    : Handle of system adaption                           (in) +*/
/*+   HwPortID   : Hardware port ID of the related PHY (1..x)          (in) +*/
/*+   pMediaType : Pointer to address for returned MediaType of port.  (out)+*/
/*+                EDD_MEDIATYPE_COPPER_CABLE                               +*/
/*+                EDD_MEDIATYPE_RADIO_COMMUNICATION                        +*/
/*+                EDD_MEDIATYPE_FIBER_OPTIC_CABLE                          +*/
/*+                                                                         +*/
/*+  Results     : LSA_VOID or EDDP_FATAL()                                 +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Get MediaType of a port for internal NSC PHY.             +*/
/*+                                                                         +*/
/*+       Note:   EDDP expects this function to execute with success.       +*/
/*+               Otherwise a fatal error must be generated.                +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#ifndef EDDP_PHY_NSC_GET_MEDIA_TYPE
LSA_VOID EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NSC_GET_MEDIA_TYPE(
    EDDP_HANDLE         hDDB,
    EDDP_SYS_HANDLE     hSysDev,
    LSA_UINT32          HwPortID,
    LSA_UINT8           *pMediaType,
    LSA_UINT8           *pOpticalTransType,
    LSA_UINT8           *pFxTransceiverType)
{
    LSA_UNUSED_ARG(hDDB);
    LSA_UNUSED_ARG(hSysDev);
    LSA_UNUSED_ARG(HwPortID);

    *pMediaType         = EDD_MEDIATYPE_COPPER_CABLE;
    *pOpticalTransType  = EDD_PORT_OPTICALTYPE_ISOTHER;
    *pFxTransceiverType = EDD_FX_TRANSCEIVER_UNKNOWN;

    return;
}
#endif  // EDDP_PHY_NSC_GET_MEDIA_TYPE
#endif  // EDDP_CFG_PHY_NSC_MEDIA_TYPE_INTERNAL

#ifdef EDDP_CFG_PHY_NSC_MAU_TYPE_INTERNAL
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_PHY_NSC_GET_MAU_TYPE                   +*/
/*+  Input/Output          :    EDDP_HANDLE         hDDB                    +*/
/*+                        :    EDDP_SYS_HANDLE     hSysDev                 +*/
/*+                        :    LSA_UINT32          HwPortID                +*/
/*+                        :    LSA_UINT32          Speed                   +*/
/*+                        :    LSA_UINT32          Duplexity               +*/
/*+                        :    LSA_UINT16          *pMauType               +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+   hDDB       : EDDP Handle                                         (in) +*/
/*+   hSysDev    : Handle of system adaption                           (in) +*/
/*+   HwPortID   : Hardware port ID of the related PHY (1..x)          (in) +*/
/*+   Speed      : Actual Speed of the Link                            (in) +*/
/*+                EDD_LINK_SPEED_10                                        +*/
/*+                EDD_LINK_SPEED_100                                       +*/
/*+   Duplexity  : Actual duplexity of the Link                        (in) +*/
/*+                EDD_LINK_MODE_HALF                                       +*/
/*+                EDD_LINK_MODE_FULL                                       +*/
/*+   pMauType   : Pointer to address for returned MAUType of port.    (out)+*/
/*+                for COPPER_CABLE & RADIO_COMMUNICATION:                  +*/
/*+                10BASETXHD                                               +*/
/*+                10BASETXFD                                               +*/
/*+                100BASETXHD                                              +*/
/*+                100BASETXFD                                              +*/
/*+                for FIBER_OPTIC_CABLE:                                   +*/
/*+                100BASEFXHD                                              +*/
/*+                100BASEPXFD                                              +*/
/*+                100BASEFXFD                                              +*/
/*+                                                                         +*/
/*+  Results     : LSA_VOID or EDDP_FATAL()                                 +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Get MAUType of a port for internal NSC PHY                +*/
/*+                                                                         +*/
/*+       Note:   EDDP expects this function to execute with success.       +*/
/*+               Otherwise a fatal error must be generated.                +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#ifndef EDDP_PHY_NSC_GET_MAU_TYPE
LSA_VOID EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NSC_GET_MAU_TYPE(
    EDDP_HANDLE         hDDB,
    EDDP_SYS_HANDLE     hSysDev,
    LSA_UINT32          HwPortID,
    LSA_UINT32          Speed,
    LSA_UINT32          Duplexity,
    LSA_UINT16          *pMauType)
{
	LSA_UINT32      DevId;

	/* because we cant use hSysDev we use EDDP internal variables */
	DevId = ((EDDP_LOCAL_DDB_PTR_TYPE)hDDB)->DevId;
	EDDP_ASSERT_FALSE(DevId >= EDDP_CFG_MAX_DEVICES);

	LSA_UNUSED_ARG(hSysDev);

    if (g_EDDP_PHY_MEDIATYPE[DevId][HwPortID-1] == EDD_MEDIATYPE_FIBER_OPTIC_CABLE)
    {
        /* ---------------------------------------- */
        /* FIBER_OPTIC_CABLE                        */
        /* ---------------------------------------- */
        if (Speed == EDD_LINK_SPEED_100)
        {
            if (Duplexity == EDD_LINK_MODE_HALF)
            {
                *pMauType = EDDP_PHY_MAUTYPE_100BASEFXHD;
            }
            else if (g_EDDP_PHY_OPTICALTRANSTYPE[DevId][HwPortID-1])
            {
                *pMauType = EDDP_PHY_MAUTYPE_100BASEPXFD;
            }
            else
            {
                *pMauType = EDDP_PHY_MAUTYPE_100BASEFXFD;
            }
            return;
        }
    }
    else
    {
        /* ---------------------------------------- */
        /* COPPER_CABLE & RADIO_COMMUNICATION       */
        /* ---------------------------------------- */
        if (Speed == EDD_LINK_SPEED_10)
        {
            if (Duplexity == EDD_LINK_MODE_HALF)
            {
                *pMauType = EDDP_PHY_MAUTYPE_10BASETXHD;
            }
            else
            {
                *pMauType = EDDP_PHY_MAUTYPE_10BASETXFD;
            }
            return;
        }

        if (Speed == EDD_LINK_SPEED_100 /* && ! FIBER_OPTIC_CABLE */)
        {
            if (Duplexity == EDD_LINK_MODE_HALF)
            {
                *pMauType = EDDP_PHY_MAUTYPE_100BASETXHD;
            }
            else
            {
                *pMauType = EDDP_PHY_MAUTYPE_100BASETXFD;
            }
            return;
        }
    }

    EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
}
#endif  // EDDP_PHY_NSC_GET_MAU_TYPE

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_PHY_NSC_CHECK_MAU_TYPE                 +*/
/*+  Input/Output          :    EDDP_HANDLE         hDDB                    +*/
/*+                        :    EDDP_SYS_HANDLE     hSysDev                 +*/
/*+                        :    LSA_UINT32          HwPortID                +*/
/*+                        :    LSA_UINT16          MAUType                 +*/
/*+                        :    LSA_UINT32          *pSpeed                 +*/
/*+                        :    LSA_UINT32          *pDuplexity             +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+   hDDB       : EDDP Handle                                         (in) +*/
/*+   hSysDev    : Handle of system adaption                           (in) +*/
/*+   HwPortID   : Hardware port ID of the related PHY (1..x)          (in) +*/
/*+   MAUType    : MAU Type to be checked                              (in) +*/
/*+                for COPPER_CABLE & RADIO_COMMUNICATION:                  +*/
/*+                10BASETXHD                                               +*/
/*+                10BASETXFD                                               +*/
/*+                100BASETXHD                                              +*/
/*+                100BASETXFD                                              +*/
/*+                for FIBER_OPTIC_CABLE:                                   +*/
/*+                100BASEFXHD                                              +*/
/*+                100BASEPXFD                                              +*/
/*+                100BASEFXFD                                              +*/
/*+   pSpeed     : Speed for given MAU Type                            (out)+*/
/*+                EDD_LINK_SPEED_10                                        +*/
/*+                EDD_LINK_SPEED_100                                       +*/
/*+   pDuplexity : Mode for given MAU Type                             (out)+*/
/*+                EDD_LINK_MODE_HALF                                       +*/
/*+                EDD_LINK_MODE_FULL                                       +*/
/*+                                                                         +*/
/*+  Results     : EDD_STS_OK        (MAUType is supported by hardware)     +*/
/*+              : EDD_STS_ERR_PARAM (MAUType is NOT supported by hardware) +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function checks if the given MAUType is supported by +*/
/*+               the hardware and returns the related Speed and Duplexity. +*/
/*+                                                                         +*/
/*+       Note:   EDDP expects this function to execute with success.       +*/
/*+               Otherwise a fatal error must be generated.                +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NSC_CHECK_MAU_TYPE(
    EDDP_HANDLE         hDDB,
    EDDP_SYS_HANDLE     hSysDev,
    LSA_UINT32          HwPortID,
    LSA_UINT16          MAUType,
    LSA_UINT32          *pSpeed,
    LSA_UINT32          *pDuplexity)
{
	LSA_UINT32      DevId;
    LSA_UINT8       MediaTypeNeeded = EDD_MEDIATYPE_UNKNOWN;

    *pSpeed     = EDD_LINK_UNKNOWN;
    *pDuplexity = EDD_LINK_UNKNOWN;

	/* because we cant use hSysDev we use EDDP internal variables */
	DevId = ((EDDP_LOCAL_DDB_PTR_TYPE)hDDB)->DevId;
	EDDP_ASSERT_FALSE(DevId >= EDDP_CFG_MAX_DEVICES);

    LSA_UNUSED_ARG(hSysDev);

    switch (MAUType)
    {
        /* ---------------------------------------- */
        /* FIBER_OPTIC_CABLE                        */
        /* ---------------------------------------- */
        case EDDP_PHY_MAUTYPE_100BASEFXHD:
        {
            *pSpeed     = EDD_LINK_SPEED_100;
            *pDuplexity = EDD_LINK_MODE_HALF;
            MediaTypeNeeded = EDD_MEDIATYPE_FIBER_OPTIC_CABLE;
            break;
        }
        case EDDP_PHY_MAUTYPE_100BASEPXFD:
        {
            *pSpeed     = EDD_LINK_SPEED_100;
            *pDuplexity = EDD_LINK_MODE_FULL;
            MediaTypeNeeded = EDD_MEDIATYPE_FIBER_OPTIC_CABLE;
            break;
        }
        case EDDP_PHY_MAUTYPE_100BASEFXFD:
        {
            *pSpeed     = EDD_LINK_SPEED_100;
            *pDuplexity = EDD_LINK_MODE_FULL;
            MediaTypeNeeded = EDD_MEDIATYPE_FIBER_OPTIC_CABLE;
            break;
        }

        /* ---------------------------------------- */
        /* COPPER_CABLE & RADIO_COMMUNICATION       */
        /* ---------------------------------------- */
        case EDDP_PHY_MAUTYPE_10BASETXHD:
        {
            *pSpeed     = EDD_LINK_SPEED_10;
            *pDuplexity = EDD_LINK_MODE_HALF;
            MediaTypeNeeded = EDD_MEDIATYPE_COPPER_CABLE;
            break;
        }
        case EDDP_PHY_MAUTYPE_10BASETXFD:
        {
            *pSpeed     = EDD_LINK_SPEED_10;
            *pDuplexity = EDD_LINK_MODE_FULL;
            MediaTypeNeeded = EDD_MEDIATYPE_COPPER_CABLE;
            break;
        }
        case EDDP_PHY_MAUTYPE_100BASETXHD:
        {
            *pSpeed     = EDD_LINK_SPEED_100;
            *pDuplexity = EDD_LINK_MODE_HALF;
            MediaTypeNeeded = EDD_MEDIATYPE_COPPER_CABLE;
            break;
        }
        case EDDP_PHY_MAUTYPE_100BASETXFD:
        {
            *pSpeed     = EDD_LINK_SPEED_100;
            *pDuplexity = EDD_LINK_MODE_FULL;
            MediaTypeNeeded = EDD_MEDIATYPE_COPPER_CABLE;
            break;
        }
        default:
        {
            /* nothing to do here, will be handled with next if  */
        }
    }

    if (g_EDDP_PHY_MEDIATYPE[DevId][HwPortID-1] != MediaTypeNeeded)
    {
        return (EDD_STS_ERR_PARAM);
    }
    else
    {
        return (EDD_STS_OK);
    }
}
#endif  // EDDP_CFG_PHY_NSC_MAU_TYPE_INTERNAL

#ifdef EDDP_CFG_PHY_NSC_LED_BLINK_INTERNAL
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_PHY_NSC_LED_BlinkBegin                 +*/
/*+  Input/Output          :    EDDP_HANDLE         hDDB                    +*/
/*+                        :    EDDP_SYS_HANDLE     hSysDev                 +*/
/*+                        :    LSA_UINT32          HwPortID                +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+   hDDB       : EDDP Handle                                         (in) +*/
/*+   hSysDev    : Handle of system adaption                           (in) +*/
/*+   HwPortID   : Hardware port ID of the related PHY                 (in) +*/
/*+                                                                         +*/
/*+  Results     : EDD_STS_OK                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: EDDP calls this function within the service               +*/
/*+               EDD_SRV_LED_BLINK() before the LED(s) start to blink. Can +*/
/*+               be used e.g. to diable the link/activity LED function of  +*/
/*+               a PHY if necessary.                                       +*/
/*+                                                                         +*/
/*+       Note:   EDDP expects this function to return with success. Any    +*/
/*+               other return value than "EDD_STS_OK" will be interpreted  +*/
/*+               as a fatal error.                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NSC_LED_BlinkBegin(
    EDDP_HANDLE         hDDB,
	EDDP_SYS_HANDLE     hSysDev,
	LSA_UINT32          HwPortID)
{
    LSA_UINT32		RegVal;

    LSA_UNUSED_ARG(hSysDev);

    EDDP_SWI_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT," +++ EDDP_PHY_LED_BlinkBegin +++ ");

    eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_PHY_NSC_REG_LED_DIRECT_CONTROL, &RegVal);

    eddp_ll_PhyRegisterWrite (hDDB,HwPortID, EDDP_PHY_NSC_REG_LED_DIRECT_CONTROL, (RegVal | 0x38));

    return (EDD_STS_OK);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_PHY_NSC_LED_BlinkSetMode               +*/
/*+  Input/Output          :    EDDP_HANDLE         hDDB                    +*/
/*+                        :    EDDP_SYS_HANDLE     hSysDev                 +*/
/*+                        :    LSA_UINT32          HwPortID                +*/
/*+                        :    LSA_UINT32		    LEDMode                 +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+   hDDB       : EDDP Handle                                         (in) +*/
/*+   hSysDev    : Handle of system adaption                           (in) +*/
/*+   HwPortID   : Hardware port ID of the related PHY                 (in) +*/
/*+   LEDMode    : Turns the LED(s) ON or OFF                          (in) +*/
/*+                                                                         +*/
/*+  Results     : EDD_STS_OK                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: EDDP calls this function repeatedly within the service    +*/
/*+               EDD_SRV_LED_BLINK() in order to turn on and turn off the  +*/
/*+               LED(s) alternately.                                       +*/
/*+                                                                         +*/
/*+       Note:   EDDP expects this function to return with success. Any    +*/
/*+               other return value than "EDD_STS_OK" will be interpreted  +*/
/*+               as a fatal error.                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NSC_LED_BlinkSetMode(
	EDDP_HANDLE 	    hDDB,
	EDDP_SYS_HANDLE     hSysDev,	
	LSA_UINT32          HwPortID,
	LSA_UINT32		    LEDMode)
{
    LSA_UINT32     RegVal;

    LSA_UNUSED_ARG(hSysDev);

    if(LEDMode == EDDP_LED_MODE_ON)
    {
        EDDP_SWI_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT," +++ EDDP_PHY_LED_BlinkSetMode | LED ON  | +++ ");

        eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_PHY_NSC_REG_LED_DIRECT_CONTROL, &RegVal);

        eddp_ll_PhyRegisterWrite (hDDB,HwPortID, EDDP_PHY_NSC_REG_LED_DIRECT_CONTROL, (RegVal | 0x7));
    }
    else if (LEDMode == EDDP_LED_MODE_OFF)
    {
        EDDP_SWI_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT," +++ EDDP_PHY_LED_BlinkSetMode | LED OFF | +++ ");

        eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_PHY_NSC_REG_LED_DIRECT_CONTROL, &RegVal);

        eddp_ll_PhyRegisterWrite (hDDB,HwPortID, EDDP_PHY_NSC_REG_LED_DIRECT_CONTROL, (RegVal & ~0x7));
    }
    else
    {
        EDDP_SWI_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR," +++ Error: Wrong LED Mode! +++ ");
    }

    return (EDD_STS_OK);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_PHY_NSC_LED_BlinkEnd                   +*/
/*+  Input/Output          :    EDDP_HANDLE         hDDB                    +*/
/*+                        :    EDDP_SYS_HANDLE     hSysDev                 +*/
/*+                        :    LSA_UINT32          HwPortID                +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+   hDDB       : EDDP Handle                                         (in) +*/
/*+   hSysDev    : Handle of system adaption                           (in) +*/
/*+   HwPortID   : Hardware port ID of the related PHY                 (in) +*/
/*+                                                                         +*/
/*+  Results     : EDD_STS_OK                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: EDDP calls this function within the service               +*/
/*+               EDD_SRV_LED_BLINK() after blinking of LED(s) has finished.+*/
/*+               Can be used e.g. to re-enable the link/activity LED       +*/
/*+               function of a PHY if necessary.                           +*/
/*+                                                                         +*/
/*+       Note:   EDDP expects this function to return with success. Any    +*/
/*+               other return value than "EDD_STS_OK" will be interpreted  +*/
/*+               as a fatal error.                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NSC_LED_BlinkEnd(
	EDDP_HANDLE 	    hDDB,
	EDDP_SYS_HANDLE     hSysDev,
	LSA_UINT32          HwPortID)
{
    LSA_UINT32     RegVal;

    LSA_UNUSED_ARG(hSysDev);

    EDDP_SWI_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT," +++ EDDP_PHY_LED_BlinkEnd +++ ");

    eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_PHY_NSC_REG_LED_DIRECT_CONTROL, &RegVal);

    eddp_ll_PhyRegisterWrite (hDDB,HwPortID, EDDP_PHY_NSC_REG_LED_DIRECT_CONTROL, (RegVal & ~0x3F));

    return (EDD_STS_OK);
}
#endif  // EDDP_CFG_PHY_NSC_NO_LED_BLINK

#endif  // EDDP_CFG_PHY_NSC_SUPPORT

/*****************************************************************************/
/*  end of file eddp_phy_nsc_out.c                                           */
/*****************************************************************************/
