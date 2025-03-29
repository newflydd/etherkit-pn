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
/*  F i l e               &F: eddp_swi.c                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP SWITCH modul functions                      */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  11.11.08    AB    initial version                                        */
/*  29.06.09    JS    changed EDD_PHY_STATUS_UNKNOWN to                      */
/*                    EDD_PHY_STATUS_ENABLED because PhyStatus will currently*/
/*                    not be changed/handled by EDDP later on.               */
/*  13.08.09    JS    Added PNIP_REG_API_FORWARDINGMODE_P1..4 init           */
/*  13.12.10    JS    Added UC Aging setup                                   */
/*  25.09.12    MH	  AP01380513 - Hardware-Configcheck in DeviceSetup		 */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID   81                 
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_SWI */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/


#include "eddp_inc.h"            /* eddp headerfiles */
#include "eddp_int.h"            /* internal header  */


EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)

#ifdef EDDP_CFG_LOCAL_MEM_STATIC
    EDDP_LOCAL_MEM_ATTR     EDDP_HDB_SWI_TYPE       g_EDDP_HDB_SWI[EDDP_CFG_MAX_CHANNELS];
#endif


/*===========================================================================*/
/*                                global Variables                           */
/*===========================================================================*/

/*===========================================================================*/
/*                                  Types                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                Constants                                  */
/*===========================================================================*/

/*===========================================================================*/
/*                                 functions                                 */
/*===========================================================================*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWIDeviceOpen                          +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             EDDP_UPPER_DPB_PTR_TYPE        pDPB         +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  pDPB       : Pointer to DeviceParameterBlock                     (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDD_SRV_DEV_OPEN                                          +*/
/*+               Initializes all SWI management structures within DDB.     +*/
/*+                                                                         +*/
/*+       Note:   EDD_STS_OK_PENDING is not allowed as response. This call  +*/
/*+               must be synchronous!                                      +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWIDeviceOpen(
    EDDP_LOCAL_DDB_PTR_TYPE	    pDDB,
    EDDP_UPPER_DPB_PTR_TYPE     pDPB)
{
    EDD_RSP		    Response;
    LSA_UINT32      k, DomainBoundaryNum;

    EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_SWIDeviceOpen(pDDB=0x%X, pDPB=0x%X)", pDDB, pDPB);

    Response = EDDP_LL_SWIDeviceOpen(pDDB, pDPB);

    if( (EDD_STS_OK == Response) && (LSA_NULL != pDDB) && (LSA_NULL != pDPB) &&
	    (EDDP_HW_CFG_IRQX_CONTROL_IRQ_LENGTH_MAX >= pDPB->HWCfg.IRQx_Control.IRQ2_Length) && // check and store parameters for "IRQx_Control" register
		(EDDP_HW_CFG_IRQX_CONTROL_IRQ_LENGTH_MAX >= pDPB->HWCfg.IRQx_Control.IRQ3_Length) )
	{
        /* -------------------------------------------------------------------------------- */
        /* device specific data                                                             */
        /* -------------------------------------------------------------------------------- */
        /* copy flag for Rapid Spanning Tree Protocol forwarding */
        pDDB->SWI.Multicast_Bridge_IEEE802RSTP_Forward = (EDD_FEATURE_ENABLE == pDPB->SWI.Multicast_Bridge_IEEE802RSTP_Forward)?LSA_TRUE:LSA_FALSE;

        // set this flag to LSA_FALSE to start with MC table A when calling _EDDP_SWI_switch_MC_table the first time
        pDDB->SWI.States.MC_Table_A_Active = LSA_FALSE;

		// copy IRQx_Control parametrs from DPB
		pDDB->HWCfg.IRQx_Control.IRQ2_Level     = pDPB->HWCfg.IRQx_Control.IRQ2_Level;
		pDDB->HWCfg.IRQx_Control.IRQ2_Length    = pDPB->HWCfg.IRQx_Control.IRQ2_Length;
		pDDB->HWCfg.IRQx_Control.IRQ3_Level     = pDPB->HWCfg.IRQx_Control.IRQ3_Level;
		pDDB->HWCfg.IRQx_Control.IRQ3_Length    = pDPB->HWCfg.IRQx_Control.IRQ3_Length;

		pDDB->SWI.PHYAddressOffset = pDPB->SWI.PhyAddressOffset;

		/* -------------------------------------------------------------------------------- */
		/* interface specific data                                                          */
		/* -------------------------------------------------------------------------------- */

        /* ---------------------------------------- */
        /* Init struct "SWI.LinkStatus[k]"          */
        /* write only values != 0                   */
        /* ---------------------------------------- */
        // k = 0,1,2
        for (k = 0; k <= pDDB->SWI.PortCnt; k++)
        {
            // initialize all link status parameters with default values
            pDDB->SWI.LinkStatus[k].LinkStatus = EDD_LINK_DOWN;
            pDDB->SWI.LinkStatus[k].LinkSpeed = EDD_LINK_UNKNOWN;
            pDDB->SWI.LinkStatus[k].LinkDuplexity = EDD_LINK_UNKNOWN;
            pDDB->SWI.LinkStatus[k].Autoneg = EDD_AUTONEG_UNKNOWN;
            pDDB->SWI.LinkStatus[k].PortStatus = EDD_PORT_PRESENT;
            pDDB->SWI.LinkStatus[k].PhyStatus = EDD_PHY_STATUS_ENABLED;
            pDDB->SWI.LinkStatus[k].IRTPortStatus = EDD_IRT_NOT_SUPPORTED;
            pDDB->SWI.LinkStatus[k].RTClass2_PortStatus = EDD_RTCLASS2_NOT_SUPPORTED;
            pDDB->SWI.LinkStatus[k].MediaType = EDD_MEDIATYPE_UNKNOWN;
            pDDB->SWI.LinkStatus[k].OpticalTransType = EDD_PORT_OPTICALTYPE_ISOTHER;
            pDDB->SWI.LinkStatus[k].Neighbour_TopoState = EDD_SET_REMOTE_PORT_STATE_IGNORE;
            pDDB->SWI.LinkStatus[k].ShortPreamble = EDD_PORT_LONG_PREAMBLE;            // starting with long preamble after reset
            pDDB->SWI.LinkStatus[k].TxFragmentation = EDD_PORT_NO_TX_FRAGMENTATION;
            pDDB->SWI.LinkStatus[k].AddCheckEnabled = EDD_LINK_ADD_CHECK_DISABLED;
            pDDB->SWI.LinkStatus[k].ChangeReason = EDD_LINK_CHANGE_REASON_NONE;
            pDDB->SWI.LinkStatus[k].MAUType = EDD_MAUTYPE_UNKNOWN;
            pDDB->SWI.LinkStatus[k].PortState = EDD_PORT_STATE_DISABLE;
            pDDB->SWI.LinkStatus[k].SyncId0_TopoOk = EDD_PORT_SYNCID0_TOPO_NOT_OK;
            pDDB->SWI.LinkStatus[k].LinkSpeedModeConfigured = EDD_LINK_AUTONEG;

            pDDB->SWI.PrmPreabmleCfg[k].PreambleLength_IsPresent = LSA_FALSE;

            // initialize all RTC3_PortState parameters with default values
            pDDB->SWI.RTC3_PortState[k].Preamble = EDDP_SWI_PREAMBLE_OK; // and this is valid for RTC3PSM
            pDDB->SWI.RTC3_PortState[k].PDEVTopoState = EDD_SET_REMOTE_PORT_STATE_TOPO_MISMATCH;
            pDDB->SWI.RTC3_PortState[k].RemoteState = EDD_SET_REMOTE_PORT_STATE_RT_CLASS3_OFF;
            pDDB->SWI.RTC3_PortState[k].TopoState = EDD_SET_REMOTE_PORT_STATE_TOPO_MISMATCH;

            for (DomainBoundaryNum = 0; DomainBoundaryNum < EDDP_PRM_DOMAIN_BOUNDARY_COUNT; DomainBoundaryNum++)
            {
                // No bounary set for announce frames at port 
                pDDB->SWI.AnnounceBoundary[k][DomainBoundaryNum] = LSA_FALSE;
            }
        }

        pDDB->SWI.SyncOk = LSA_FALSE;

        // initialize cycle base factor with init value (PNIP starts with this value after reset)
        pDDB->SWI.CycleInfo.CycleBaseFactor = EDDP_CRT_CYCLE_BASE_FACTOR_32;

        // init state machine
        pDDB->SWI.SendClockChangeSm.State = EDDP_SWI_SEND_CLOCK_CHANGE_STATE_IDLE;
        pDDB->SWI.SendClockChangeSm.pRQB = LSA_NULL;

        /* ---------------------------------------- */
        /* Alloc memory                             */
        /* ---------------------------------------- */

		// Alloc memory for shadow MC Table and link pointer into DDB
		pDDB->SWI.pMC_Table_Shadow = (EDDP_SWI_MULTICAST_ENTRY_PTR_TYPE)EDDP_GlbAllocLocalMem(sizeof(EDDP_SWI_MULTICAST_ENTRY_TYPE) * (EDDP_SWI_FDB_SIZE_OF_MC_TABLE_32BIT / 4));

		if (LSA_HOST_PTR_ARE_EQUAL(pDDB->SWI.pMC_Table_Shadow, LSA_NULL))
		{
			Response = EDD_STS_ERR_RESOURCE;
			EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
			EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWIDeviceOpen(): ERROR -> alloc memory for pMC_Table_Shadow!");
		}
		else
		{
			pDDB->SWI.MC_Table_Max_Entries = (EDDP_SWI_FDB_SIZE_OF_MC_TABLE_32BIT / 4);
		}

        /* ---------------------------------------- */
        /* Init RxTxDelay                           */
        /* ---------------------------------------- */
		pDDB->SWI.PTCP_RxDelay = EDDP_SWI_PTCP_RX_DELAY_ERTEC200P;
		pDDB->SWI.PTCP_TxDelay = EDDP_SWI_PTCP_TX_DELAY_ERTEC200P;

        /* ---------------------------------------- */
        /* IRT flags                                */
        /* ---------------------------------------- */
        pDDB->SWI.IRTtopFwMode = EDDP_SWI_IRTTOP_FW_MODE_INVALID;
        pDDB->SWI.IRTState = 0;

        /* ---------------------------------------- */
        /* copy MRP settings from DPB               */
        /* ---------------------------------------- */
        pDDB->SWI.MaxMRP_Instances          = pDPB->SWI.MaxMRP_Instances;
        pDDB->SWI.MRPDefaultRoleInstance0   = pDPB->SWI.MRPDefaultRoleInstance0;
        pDDB->SWI.MRPSupportedRole          = pDPB->SWI.MRPSupportedRole;
        pDDB->SWI.MRPSupportedMultipleRole  = pDPB->SWI.MRPSupportedMultipleRole;
        pDDB->SWI.MRAEnableLegacyMode       = pDPB->SWI.MRAEnableLegacyMode;
        pDDB->SWI.MaxMRPInterconn_Instances = pDPB->SWI.MaxMRPInterconn_Instances;
        pDDB->SWI.SupportedMRPInterconnRole = pDPB->SWI.SupportedMRPInterconnRole;

        for (k = 1; k <= pDDB->SWI.PortCnt; k++)
        {
            pDDB->SWI.LinkStatus[k].MRPRingPort = pDPB->SWI.MRPRingPort[k - 1];
            // count default ring ports
            if (pDDB->SWI.LinkStatus[k].MRPRingPort == EDD_MRP_RING_PORT_DEFAULT)
            {
                pDDB->SWI.DefaultRPortCount++;
            }

            if (pDDB->SWI.LinkStatus[k].MRPRingPort == EDD_MRP_NO_RING_PORT)
            {
                pDDB->SWI.NoneRPortCount++;
            }

            if (EDD_SUPPORTS_MRP_INTERCONN_PORT_CONFIG_NO == pDPB->SWI.SupportsMRPInterconnPortConfig[k - 1])
            {
                pDDB->SWI.LinkStatus[k].SupportsMRPInterconnPortConfig = pDPB->SWI.SupportsMRPInterconnPortConfig[k - 1];
            }
            else
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWIDeviceOpen(): ERROR -> SupportsMRPInterconnPortConfig[UserPort %d]=0x%X is not EDD_SUPPORTS_MRP_INTERCONN_PORT_CONFIG_NO!",
                    k, pDPB->SWI.SupportsMRPInterconnPortConfig[k - 1]);
                break;
            }
        }

        if (EDD_STS_OK == Response)
        {
            // check MRP settings
            if (pDDB->SWI.MaxMRP_Instances == 0)
            {
                // no MRP (MRP is not supported): "MRPDefaultRoleInstance0" and "MRPSupportedRole" must be no MRP too. All ports have to been set EDD_MRP_NO_RING_PORT.
                if ((pDDB->SWI.MRPDefaultRoleInstance0 != EDD_MRP_ROLE_NONE)
                    || (pDDB->SWI.MRPSupportedRole != 0)
                    || (pDDB->SWI.NoneRPortCount != pDDB->SWI.PortCnt)
                    )
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_SWI_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWIDeviceOpen(): ERROR -> MaxMRP_Instances(no MRP) but MRPDefaultRoleInstance0(0x%X) or MRPSupportedRole(0x%X) or NoneRPortCount(%u) != PortCnt(%u)!",
                        pDDB->SWI.MRPDefaultRoleInstance0, pDDB->SWI.MRPSupportedRole, pDDB->SWI.NoneRPortCount, pDDB->SWI.PortCnt);
                }
            }
            else
            {
                // MRP is supported

                // "MRPDefaultRoleInstance0" and "MRPSupportedRole" must be Client or Manager
                if (pDDB->SWI.MRPSupportedRole == 0)
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWIDeviceOpen(): ERROR -> MaxMRP_Instances(0x%X) but MRPDefaultRoleInstance0(0x%X) or MRPSupportedRole(0x%X)!",
                        pDDB->SWI.MaxMRP_Instances, pDDB->SWI.MRPDefaultRoleInstance0, pDDB->SWI.MRPSupportedRole);
                }
                else
                {
                    // DefaultRPortCount must be even 
                    if (0 != (pDDB->SWI.DefaultRPortCount & 1))
                    {
                        Response = EDD_STS_ERR_PARAM;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWIDeviceOpen(): ERROR -> MaxMRP_Instances(0x%X) but DefaultRPortCount(0x%X) is not 2!",
                            pDDB->SWI.MaxMRP_Instances, pDDB->SWI.DefaultRPortCount);
                    }

                    //  DefaultRPortCount must not be more than twice of MaxInstances
                    if ((pDDB->SWI.DefaultRPortCount / 2) > pDDB->SWI.MaxMRP_Instances)
                    {
                        Response = EDD_STS_ERR_PARAM;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWIDeviceOpen(): ERROR -> MaxMRP_Instances(0x%X) but DefaultRPortCount(0x%X) is not 2!",
                            pDDB->SWI.MaxMRP_Instances, pDDB->SWI.DefaultRPortCount);
                    }

                    // currently every EDDP hw can only be used as forwarder (or without hsync)
                    if ((pDDB->FeatureSupport.ApplicationExist))
                    {
                        Response = EDD_STS_ERR_PARAM;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWIDeviceOpen(): ERROR -> EDDP does currently not support HSYNC APPLICATION SUPPORT!");
                    }
                }
            }
        }
    }

    /* MRP interconnect settings */
    if (EDD_STS_OK == Response)
    {
        if (pDDB->SWI.MaxMRPInterconn_Instances > pDDB->SWI.MaxMRP_Instances)
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWIDeviceOpen(): ERROR -> MaxMRPInterconn_Instances(0x%X) > MaxMRP_Instances(0x%X)",
                pDDB->SWI.MaxMRPInterconn_Instances, pDDB->SWI.MaxMRP_Instances);
        }

        if (pDDB->SWI.SupportedMRPInterconnRole != EDD_SUPPORTED_MRP_INTERCONN_ROLE_CAP_NONE)
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWIDeviceOpen(): ERROR -> SupportedMRPInterconnRole(0x%X) != NONE",
                pDDB->SWI.SupportedMRPInterconnRole);
        }
    }

    if (EDD_STS_OK == Response)
    {
        // "MRAEnableLegacyMode" must be ENABLED or DISABLED
        if (   (EDD_MRA_ENABLE_LEGACY_MODE  != pDDB->SWI.MRAEnableLegacyMode)
            && (EDD_MRA_DISABLE_LEGACY_MODE != pDDB->SWI.MRAEnableLegacyMode))
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWIDeviceOpen(): ERROR -> MRAEnableLegacyMode(0x%X) illegal value!",
                pDDB->SWI.MRAEnableLegacyMode);
        }
    }

    /* -------------------------------------------------------------------------------- */
    /* Check Response                                                                   */
    /* -------------------------------------------------------------------------------- */
    if (Response != EDD_STS_OK)
    {
        // free memory for shadow MC table
        if (LSA_HOST_PTR_ARE_EQUAL(pDDB->SWI.pMC_Table_Shadow, LSA_NULL))
        {
            // the pointer is already deleted !
        }
        else
        {
            EDDP_GlbFreeLocalMem (pDDB->SWI.pMC_Table_Shadow);
            pDDB->SWI.pMC_Table_Shadow = LSA_NULL;
        }
    }

    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_SWIDeviceOpen(): Response=0x%X", Response);

    return (Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_SWISetPortBarrier                     +*/
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
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This util-function is called from EDDP_SWIDeviceSetup2()  +*/
/*+               Setup Port barriers                                       +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_SWISetPortBarrier(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DSB_PTR_TYPE     pDSB)
{
    LSA_UINT32      Value0to7, Value8to15, PortID, HwPortID;
    LSA_UINT32      RegQcwMapPx0to7, RegQcwMapPx8to11;
    LSA_UINT32      RegCountPx[EDDP_SWI_BARRIER_IDX_MGM+1];
    EDD_RSP         Response = EDD_STS_OK;

    LSA_UNUSED_ARG(pDSB);

#ifdef EDDP_SWI_USE_PORT_BARRIER
    // ***** mapping for EDDP_SWI_BARRIER_IDX_REST: 0, 1, 2, 3, 5, 9
    // ***** mapping for EDDP_SWI_BARRIER_IDX_RTA:  4, 6
    // ***** mapping for EDDP_SWI_BARRIER_IDX_MGM:  10, 11
    Value8to15 =   (((LSA_UINT32)EDDP_SWI_BARRIER_IDX_MGM <<PNIP_REG_QCW_MAPPING_P1_8TO11_SHFT_QUEUE11_MAP)     |
                    ((LSA_UINT32)EDDP_SWI_BARRIER_IDX_MGM <<PNIP_REG_QCW_MAPPING_P1_8TO11_SHFT_QUEUE10_MAP)     |
                    ((LSA_UINT32)EDDP_SWI_BARRIER_IDX_REST<<PNIP_REG_QCW_MAPPING_P1_8TO11_SHFT_QUEUE9_MAP)      |
                    ((LSA_UINT32)EDDP_SWI_BARRIER_IDX_OFF <<PNIP_REG_QCW_MAPPING_P1_8TO11_SHFT_QUEUE8_MAP) );

    Value0to7  =   (((LSA_UINT32)EDDP_SWI_BARRIER_IDX_OFF <<PNIP_REG_QCW_MAPPING_P1_0TO7_SHFT_QUEUE7_MAP)   |
                    ((LSA_UINT32)EDDP_SWI_BARRIER_IDX_RTA <<PNIP_REG_QCW_MAPPING_P1_0TO7_SHFT_QUEUE6_MAP)   |
                    ((LSA_UINT32)EDDP_SWI_BARRIER_IDX_REST<<PNIP_REG_QCW_MAPPING_P1_0TO7_SHFT_QUEUE5_MAP)   |
                    ((LSA_UINT32)EDDP_SWI_BARRIER_IDX_RTA <<PNIP_REG_QCW_MAPPING_P1_0TO7_SHFT_QUEUE4_MAP)   |
                    ((LSA_UINT32)EDDP_SWI_BARRIER_IDX_REST<<PNIP_REG_QCW_MAPPING_P1_0TO7_SHFT_QUEUE3_MAP)   |
                    ((LSA_UINT32)EDDP_SWI_BARRIER_IDX_REST<<PNIP_REG_QCW_MAPPING_P1_0TO7_SHFT_QUEUE2_MAP)   |
                    ((LSA_UINT32)EDDP_SWI_BARRIER_IDX_REST<<PNIP_REG_QCW_MAPPING_P1_0TO7_SHFT_QUEUE1_MAP)   |
                    ((LSA_UINT32)EDDP_SWI_BARRIER_IDX_REST<<PNIP_REG_QCW_MAPPING_P1_0TO7_SHFT_QUEUE0_MAP) );
#else
    Value0to7  = 0xFFFFFFFF;
    Value8to15 = 0x0000FFFF;
#endif

    for ( PortID = 1; PortID <= pDDB->SWI.PortCnt; PortID++ )
    {
        HwPortID = EDDP_SWI_get_HwPortID (pDDB, PortID);

        switch (HwPortID)
        {
            case 1:
            {
                RegQcwMapPx0to7                         = PNIP_REG_QCW_MAPPING_P1_0TO7;
                RegQcwMapPx8to11                        = PNIP_REG_QCW_MAPPING_P1_8TO11;
                RegCountPx[EDDP_SWI_BARRIER_IDX_REST]   = PNIP_REG_QCW_COUNT_P1_0;  // REST
                RegCountPx[EDDP_SWI_BARRIER_IDX_RTA]    = PNIP_REG_QCW_COUNT_P1_1;  // RTA
                RegCountPx[EDDP_SWI_BARRIER_IDX_MGM]    = PNIP_REG_QCW_COUNT_P1_2;  // MGM
            }
            break;

            case 2:
            {
                RegQcwMapPx0to7                         = PNIP_REG_QCW_MAPPING_P2_0TO7;    
                RegQcwMapPx8to11                        = PNIP_REG_QCW_MAPPING_P2_8TO11;  
                RegCountPx[EDDP_SWI_BARRIER_IDX_REST]   = PNIP_REG_QCW_COUNT_P2_0;  // REST
                RegCountPx[EDDP_SWI_BARRIER_IDX_RTA]    = PNIP_REG_QCW_COUNT_P2_1;  // RTA
                RegCountPx[EDDP_SWI_BARRIER_IDX_MGM]    = PNIP_REG_QCW_COUNT_P2_2;  // MGM
            }
            break;           

            default:
            {
                RegQcwMapPx0to7     = 0;    // to avoid compiler warning C4701
                RegQcwMapPx8to11    = 0;    // to avoid compiler warning C4701
                // use the values to 0 any initialization at all
                RegCountPx[EDDP_SWI_BARRIER_IDX_REST] = 0;  // REST
                RegCountPx[EDDP_SWI_BARRIER_IDX_RTA] = 0;   // RTA
                RegCountPx[EDDP_SWI_BARRIER_IDX_MGM] = 0;   // MGM
                Response            = EDD_STS_ERR_PARAM;
                EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] _EDDP_SWISetPortBarrier(): ERROR -> invalid HwPortID(%d)! UsrPortID(%d)", 
                    HwPortID, PortID);
            }
            break;
        }

        if (Response == EDD_STS_OK)
        {
            EDDP_HAL_REG32_WRITE(pDDB, RegQcwMapPx0to7,  Value0to7);
            EDDP_HAL_REG32_WRITE(pDDB, RegQcwMapPx8to11, Value8to15);
            EDDP_HAL_REG32_WRITE(pDDB, RegCountPx[EDDP_SWI_BARRIER_IDX_REST], ((LSA_UINT32)1<<PNIP_REG_QCW_COUNT_P1_0_SHFT_ENABLECOUNT));
            EDDP_HAL_REG32_WRITE(pDDB, RegCountPx[EDDP_SWI_BARRIER_IDX_RTA],  ((LSA_UINT32)1<<PNIP_REG_QCW_COUNT_P1_0_SHFT_ENABLECOUNT));
            EDDP_HAL_REG32_WRITE(pDDB, RegCountPx[EDDP_SWI_BARRIER_IDX_MGM],  ((LSA_UINT32)1<<PNIP_REG_QCW_COUNT_P1_0_SHFT_ENABLECOUNT));
        }
        else
        {
            // exit FOR loop
            break;
        }
    }

    if (Response == EDD_STS_OK)
    {
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_QCW_TRIGGER_PA,   EDDP_SWI_BARRIER_TIME_1MS);
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_QCW_BARRIER_PA_0, EDDP_SWI_BARRIER_QCW_REST);
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_QCW_BARRIER_PA_1, EDDP_SWI_BARRIER_QCW_RTA);
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_QCW_BARRIER_PA_2, EDDP_SWI_BARRIER_QCW_MGM);
    }
    
    return (Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWIDeviceSetup1                        +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             EDDP_UPPER_DSB_PTR_TYPE        pDSB         +*/
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
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDDP_SRV_DEV_SETUP                                        +*/
/*+               Part 1 of Setup Device for SWI.                           +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWIDeviceSetup1(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DSB_PTR_TYPE     pDSB)
{
    EDD_RSP             Response = EDD_STS_OK;
    LSA_UINT32          HwPortID, PortID;
   // LSA_UINT32          PhyAddress, OUI1RegAddress, OUI2RegAddress, OUIUnequalValue;
    __attribute__((unused))LSA_UINT32          reg_val, reg_address;

    EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_SWIDeviceSetup1(pDDB=0x%X, pDSB=0x%X)", pDDB, pDSB);

    if( (LSA_NULL != pDDB) && (LSA_NULL != pDSB) )
	{
		// Initialize MAC/PORT handling
		for ( PortID=1; PortID <= pDDB->SWI.PortCnt; PortID++ )
		{
			HwPortID = EDDP_SWI_get_HwPortID (pDDB, PortID);

			// Port_MAC_Address shall not be MC Mac!
			if(EDDP_IS_MC_MAC(pDSB->PortParams[PortID-1].Port_MAC_Address))
			{
				EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWIDeviceSetup1(): FATAL -> Port_MAC_Address[%u] is MC Mac!", HwPortID);
				EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
			}

			/* ----------------------------------------------------------------------*/
			/* Set up port MAC addresses                                             */
			/* ----------------------------------------------------------------------*/
			// copy port MAC address from DSB to DDB
			pDDB->SWI.PortParams[HwPortID].Port_MAC_Address = pDSB->PortParams[PortID-1].Port_MAC_Address;
			if (pDDB->SWI.PortParams[HwPortID].Port_MAC_Address.MacAdr[0] == 0x00 &&
				pDDB->SWI.PortParams[HwPortID].Port_MAC_Address.MacAdr[1] == 0x00 &&
				pDDB->SWI.PortParams[HwPortID].Port_MAC_Address.MacAdr[2] == 0x00 &&
				pDDB->SWI.PortParams[HwPortID].Port_MAC_Address.MacAdr[3] == 0x00 &&
				pDDB->SWI.PortParams[HwPortID].Port_MAC_Address.MacAdr[4] == 0x00 &&
				pDDB->SWI.PortParams[HwPortID].Port_MAC_Address.MacAdr[5] == 0x00 )
			{
				Response = EDD_STS_ERR_PARAM;
				EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
				EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWIDeviceSetup1: ERROR -> No Port-MAC-Address specified for port %d!",
					HwPortID);
			}
			else if (   (   (EDD_PORT_IS_NOT_WIRELESS != pDDB->SWI.PortParams[HwPortID].IsWireless)
						 && (EDD_PORT_IS_WIRELESS != pDDB->SWI.PortParams[HwPortID].IsWireless) )
					 || (   (EDD_PORT_MDIX_DISABLED != pDDB->SWI.PortParams[HwPortID].IsMDIX)
						 && (EDD_PORT_MDIX_ENABLED != pDDB->SWI.PortParams[HwPortID].IsMDIX) ) )
			{
				Response = EDD_STS_ERR_PARAM;
				EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
				EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWIDeviceSetup1: ERROR -> port %d: Illegal value(s) for IsWireless(%d)/IsMDIX(%d)!",
					HwPortID, pDDB->SWI.PortParams[HwPortID].IsWireless, pDDB->SWI.PortParams[HwPortID].IsMDIX);
			}

#if 0
			/* ----------------------------------------------------------------------*/
			/* Setup PNIP_REG_API_FORWARDINGMODE_Px                                  */
			/* Queue 15 will be set to CutThrough.All other Queues to Store&Forward. */
			/* ----------------------------------------------------------------------*/
			reg_val = ((0xFFF << PNIP_REG_API_FORWARDINGMODE_P1_SHFT_API_FWMODE) |
						PNIP_REG_API_FORWARDINGMODE_P1__MSK_API_FWMODE_14);

			reg_address = (HwPortID==1 ? PNIP_REG_API_FORWARDINGMODE_P1 :
						  (HwPortID==2 ? PNIP_REG_API_FORWARDINGMODE_P2 :
						  (HwPortID==3 ? PNIP_REG_API_FORWARDINGMODE_P3 :
						  (HwPortID==4 ? PNIP_REG_API_FORWARDINGMODE_P4 : 0))));
			EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_val);

			/* ---------------------------- */
			/* set PortControl_Px           */
			/* ---------------------------- */
			reg_address = (HwPortID==1 ? PNIP_REG_PORTCONTROL_P1 :
						  (HwPortID==2 ? PNIP_REG_PORTCONTROL_P2 :
						  (HwPortID==3 ? PNIP_REG_PORTCONTROL_P3 :
						  (HwPortID==4 ? PNIP_REG_PORTCONTROL_P4 : 0))));

			reg_val = EDDP_HAL_REG32_READ(pDDB, reg_address);

			EDDP_SET_BIT_VALUE32(reg_val, 0x00000001, PNIP_REG_PORTCONTROL_P1__MSK_PTCP_FWD_DELAY, PNIP_REG_PORTCONTROL_P1_SHFT_PTCP_FWD_DELAY);
			EDDP_SET_BIT_VALUE32(reg_val, 0x00000001, PNIP_REG_PORTCONTROL_P1__MSK_DISABLE_MACTXERR, PNIP_REG_PORTCONTROL_P1_SHFT_DISABLE_MACTXERR);
			// for PNIP-Revision(2 and 3) the "Tx_RedGuard_DP2" is activated
			EDDP_SET_BIT_VALUE32(reg_val, 0x00000001, PNIP_REG_PORTCONTROL_P1__MSK_TX_REDGUARD_DP2, PNIP_REG_PORTCONTROL_P1_SHFT_TX_REDGUARD_DP2);

			EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_val);

			/* ---------------------------- */
			/* set FlowControl_Px           */
			/* ---------------------------- */
			reg_address = (HwPortID==1 ? PNIP_REG_FLOWCONTROL_P1 :
						  (HwPortID==2 ? PNIP_REG_FLOWCONTROL_P2 :
						  (HwPortID==3 ? PNIP_REG_FLOWCONTROL_P3 :
						  (HwPortID==4 ? PNIP_REG_FLOWCONTROL_P4 : 0))));

			reg_val = EDDP_HAL_REG32_READ(pDDB, reg_address);

			EDDP_SET_BIT_VALUE32(reg_val, 0x00000001, PNIP_REG_FLOWCONTROL_P1__MSK_RXBOUNDARY_DP2, PNIP_REG_FLOWCONTROL_P1_SHFT_RXBOUNDARY_DP2);
			EDDP_SET_BIT_VALUE32(reg_val, 0x00000001, PNIP_REG_FLOWCONTROL_P1__MSK_TXBOUNDARY_DP2, PNIP_REG_FLOWCONTROL_P1_SHFT_TXBOUNDARY_DP2);
			EDDP_SET_BIT_VALUE32(reg_val, 0x00000001, PNIP_REG_FLOWCONTROL_P1__MSK_TXBOUNDARY_RED2GREEN_DP2, PNIP_REG_FLOWCONTROL_P1_SHFT_TXBOUNDARY_RED2GREEN_DP2);
			EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_val);

			/* ---------------------------- */
			/* set Port_Grouping            */
			/* ---------------------------- */
			reg_val = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_PORT_GROUPING);
			reg_val &= ~(0x00000001 << (HwPortID - 1));
			EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_PORT_GROUPING, reg_val);

			/* ---------------------------- */
			/* set FrameAging_Time_Px       */
			/* ---------------------------- */
			reg_address = (HwPortID==1 ? PNIP_REG_FRAMEAGING_TIME_P1 :
						  (HwPortID==2 ? PNIP_REG_FRAMEAGING_TIME_P2 :
						  (HwPortID==3 ? PNIP_REG_FRAMEAGING_TIME_P3 :
						  (HwPortID==4 ? PNIP_REG_FRAMEAGING_TIME_P4 : 0))));
			reg_val = EDDP_HAL_REG32_READ(pDDB, reg_address);
			EDDP_SET_BIT_VALUE32(reg_val, EDDP_SWI_REG_FRAMEAGING_TIME_INIT, PNIP_REG_FRAMEAGING_TIME_P1__MSK_AGINGTIME, PNIP_REG_FRAMEAGING_TIME_P1_SHFT_AGINGTIME);
			EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_val);
#endif
		} //end for


		/* ---------------------------- */
		/* set IRT_DelaySafety          */
		/* ---------------------------- */
		reg_val = 0;
#if 0
		EDDP_SET_BIT_VALUE32(reg_val, EDDP_SWI_REG_IRT_DELAYSAFETY_100M_INIT_PNIP_REV12, PNIP_REG_IRT_DELAYSAFETY__MSK_SAFETY_TIME_100M, PNIP_REG_IRT_DELAYSAFETY_SHFT_SAFETY_TIME_100M);

		EDDP_SET_BIT_VALUE32(reg_val, EDDP_SWI_REG_IRT_DELAYSAFETY_1G_INIT, PNIP_REG_IRT_DELAYSAFETY__MSK_SAFETY_TIME_1G, PNIP_REG_IRT_DELAYSAFETY_SHFT_SAFETY_TIME_1G);
		EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_IRT_DELAYSAFETY, reg_val);

		/* ---------------------------- */
		/* set SafetyMargin             */
		/* ---------------------------- */
		reg_val = 0;

		// switch to current PNIP revision

		EDDP_SET_BIT_VALUE32(reg_val, EDDP_SWI_REG_SAFETYMARGIN_100M_INIT_PNIP_REV2, PNIP_REG_SAFETYMARGIN__MSK_SAFETYMARGIN_100M, PNIP_REG_SAFETYMARGIN_SHFT_SAFETYMARGIN_100M);
		EDDP_SET_BIT_VALUE32(reg_val, EDDP_SWI_REG_SAFETYMARGIN_1G_INIT, PNIP_REG_SAFETYMARGIN__MSK_SAFETYMARGIN_1G, PNIP_REG_SAFETYMARGIN_SHFT_SAFETYMARGIN_1G);
		EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_SAFETYMARGIN, reg_val);

		/* ---------------------------- */
		/* set IRQx_Control             */
		/* ---------------------------- */
		// IRQ2_Control
		reg_val = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_IRQ2_CONTROL);
		EDDP_SET_BIT_VALUE32(reg_val, pDDB->HWCfg.IRQx_Control.IRQ2_Level, PNIP_REG_IRQ2_CONTROL__MSK_IRQ_LEVEL, PNIP_REG_IRQ2_CONTROL_SHFT_IRQ_LEVEL);
		/* There is a macro that checks                                                                 */
		/* PNIP_R2A0_REG_IRQ2_CONTROL__MSK_IRQ_LENGTH  and PNIP_R2F0_REG_IRQ2_CONTROL__MSK_IRQ_LENGTH,  */
		/* PNIP_R2A0_REG_IRQ2_CONTROL__MSK_IRQ_LENGTH  and PNIP_R3_REG_IRQ2_CONTROL__MSK_IRQ_LENGTH,    */
		/* PNIP_R2A0_REG_IRQ2_CONTROL_SHFT_IRQ_LENGTH and PNIP_R2F0_REG_IRQ2_CONTROL_SHFT_IRQ_LENGTH,   */
		/* PNIP_R2A0_REG_IRQ2_CONTROL_SHFT_IRQ_LENGTH and PNIP_R3_REG_IRQ2_CONTROL_SHFT_IRQ_LENGTH      */
		/* has the same value.                                                                          */
		/* Checked in eddp_hal_pnip_val.h. We use the R2 version.                                       */
		EDDP_SET_BIT_VALUE32(reg_val, pDDB->HWCfg.IRQx_Control.IRQ2_Length, PNIP_R2F0_REG_IRQ2_CONTROL__MSK_IRQ_LENGTH, PNIP_R2F0_REG_IRQ2_CONTROL_SHFT_IRQ_LENGTH);
		EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_IRQ2_CONTROL, reg_val);
		// IRQ3_Control
		reg_val = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_IRQ3_CONTROL);
		EDDP_SET_BIT_VALUE32(reg_val, pDDB->HWCfg.IRQx_Control.IRQ3_Level, PNIP_REG_IRQ3_CONTROL__MSK_IRQ_LEVEL, PNIP_REG_IRQ3_CONTROL_SHFT_IRQ_LEVEL);
		/* PNIP_REG_IRQ3_CONTROL_SHFT_IRQ_LENGTH was replaced by PNIP_R2_REG_IRQ3_CONTROL_SHFT_IRQ_LENGTH, hardware dependency*/
		/* PNIP_REG_IRQ3_CONTROL__MSK_IRQ_LENGTH was replaced by PNIP_R2_REG_IRQ3_CONTROL__MSK_IRQ_LENGTH, hardware dependency*/
		/* There is a macro that checks                                                                                                             */
		/* PNIP_R2A0_REG_IRQ3_CONTROL__MSK_IRQ_LENGTH, PNIP_R2F0_REG_IRQ3_CONTROL__MSK_IRQ_LENGTH and PNIP_R3_REG_IRQ3_CONTROL__MSK_IRQ_LENGTH,     */
		/* PNIP_R2A0_REG_IRQ3_CONTROL_SHFT_IRQ_LENGTH, PNIP_R2F0_REG_IRQ3_CONTROL_SHFT_IRQ_LENGTH and PNIP_R3_REG_IRQ3_CONTROL_SHFT_IRQ_LENGTH      */
		/* has the same value.                                                                                                                      */
		/* Checked in eddp_hal_pnip_val.h. We use the R2 version.                                                                                   */
		EDDP_SET_BIT_VALUE32(reg_val, pDDB->HWCfg.IRQx_Control.IRQ3_Length, PNIP_R2F0_REG_IRQ3_CONTROL__MSK_IRQ_LENGTH, PNIP_R2F0_REG_IRQ3_CONTROL_SHFT_IRQ_LENGTH);
		EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_IRQ3_CONTROL, reg_val);
#endif
		// set up FDB Control
		EDDP_SWI_fdb_setup (pDDB);


		for ( PortID=1; PortID <= pDDB->SWI.PortCnt; PortID++ )
		{
			HwPortID = EDDP_SWI_get_HwPortID (pDDB, PortID);

			// remember CFG parameter from DSB
			// SWI.PortParams[HwPortID]                   DSB.PortParams[UserPortID-1]: because this array is [0..3]
			pDDB->SWI.PortParams[HwPortID].IsWireless   = pDSB->PortParams[PortID-1].IsWireless;
			pDDB->SWI.PortParams[HwPortID].IsMDIX       = pDSB->PortParams[PortID-1].IsMDIX;

			/* ----------------------------------------------------------------------*/
			/* Set initial IsPulled                                                  */
			/* ----------------------------------------------------------------------*/
			pDDB->SWI.LinkStatus[HwPortID].IsPulled = EDD_PORTMODULE_IS_PLUGGED;
		}

		/* initialize CHANGE_PORT state machine and PHYSTS state machine*/
		{
			LSA_UINT16 PortIterator;

			for(PortIterator = 0 ; PortIterator < pDDB->SWI.PortCnt ; PortIterator++)
			{
				pDDB->PhyStatusSm[PortIterator].State = EDDP_PHYSTS_SM_STATE_INIT_WF_PWRUP;
				pDDB->PortModuleSm[PortIterator].State = EDDP_PORT_MODULE_SM_STATE_ISPLUGGED;
			}

		}


#if 0
		/* Enable all PHYs (release reset) and set MDIO_HOLD */

		/* setup default value */
		reg_val = (1 << PNIP_R2_REG_PHY_CONTROL_SHFT_PHY_ACTIVATE);
		EDDP_SET_BIT_VALUE32(reg_val, PNIP_REG_PHY_CONTROL_RSTV_TRANSFERRATE_DIV, PNIP_REG_PHY_CONTROL__MSK_TRANSFERRATE_DIV, PNIP_REG_PHY_CONTROL_SHFT_TRANSFERRATE_DIV);

		/* enter MDIO HOLD value */
		EDDP_SET_BIT_VALUE32(reg_val, PNIP_REG_PHY_CONTROL_MDIO_HOLD_VAL_DEFAULT, PNIP_R2_REG_PHY_CONTROL__MSK_MDIO_HOLD, PNIP_R2_REG_PHY_CONTROL_SHFT_MDIO_HOLD);

		/* write register */
		EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_PHY_CONTROL, reg_val);

#endif
		// Initialize transceiver handling
		// Transceiver initialization can be done only now.
		// This ensures no entrys are generated in MAC Table till now.
		for ( PortID=1; PortID <= pDDB->SWI.PortCnt; PortID++ )
		{
			HwPortID = EDDP_SWI_get_HwPortID (pDDB, PortID);

			// remember CFG parameter from DSB
			// SWI.PortParams[HwPortID]                   DSB.PortParams[UserPortID-1]: because this array is [0..3]
			pDDB->SWI.PortParams[HwPortID].IsWireless   = pDSB->PortParams[PortID-1].IsWireless;
			pDDB->SWI.PortParams[HwPortID].IsMDIX       = pDSB->PortParams[PortID-1].IsMDIX;
			pDDB->SWI.PortParams[HwPortID].GBitSupport  = pDSB->PortParams[PortID-1].GBitSupport;
#if 0
			// get PHY address, vendor register addresses and "unequal compare value" for PHY start-up
			Response = EDDP_PHY_GetPhyAddress (pDDB->hDDB, pDDB->hSysDev, HwPortID, pDDB->SWI.PHYAddressOffset, &PhyAddress, &OUI1RegAddress, &OUI2RegAddress, &OUIUnequalValue);

			if (Response != EDD_STS_OK)
			{
				Response = EDD_STS_ERR_PARAM;
				EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
				EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWIDeviceSetup1(): ERROR -> Call of EDDP_PHY_GetPhyAddress() is not Ok!");
			}

			// remember PHY address in DDB
			pDDB->SWI.PortParams[HwPortID].SMI_PHY_Address = PhyAddress;

			// set PHY_OUI1_Select_PX
			reg_val = (OUI1RegAddress | 0x00000020/*Polling enable*/ | 0x00000040/*Unequal*/ | ((OUIUnequalValue & 0x0000FFFF) << 16));
			// select the right register address for current port number
			reg_address = (HwPortID==1 ? PNIP_REG_PHYOUI1_SELECT_P1 :
						  (HwPortID==2 ? PNIP_REG_PHYOUI1_SELECT_P2 :
						  (HwPortID==3 ? PNIP_REG_PHYOUI1_SELECT_P3 :
						  (HwPortID==4 ? PNIP_REG_PHYOUI1_SELECT_P4 : 0))));
			EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_val);

			// set PHY_OUI2_Select_PX
			reg_val = (OUI2RegAddress | 0x00000020/*Polling enable*/ | 0x00000040/*Unequal*/| (OUIUnequalValue & 0xFFFF0000));
			// select the right register address for current port number
			reg_address = (HwPortID==1 ? PNIP_REG_PHYOUI2_SELECT_P1 :
						  (HwPortID==2 ? PNIP_REG_PHYOUI2_SELECT_P2 :
						  (HwPortID==3 ? PNIP_REG_PHYOUI2_SELECT_P3 :
						  (HwPortID==4 ? PNIP_REG_PHYOUI2_SELECT_P4 : 0))));
			EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_val);

			// set PHYCommand_PX | OUI1/2 polling is now active; when PHY is ready an interrupt is fired and EDDP_SWI_FinishPHYSetup() is called
			// select the right register address for current port number
			reg_address = (HwPortID==1 ? PNIP_REG_PHYCOMMAND_P1 :
						  (HwPortID==2 ? PNIP_REG_PHYCOMMAND_P2 :
						  (HwPortID==3 ? PNIP_REG_PHYCOMMAND_P3 :
						  (HwPortID==4 ? PNIP_REG_PHYCOMMAND_P4 : 0))));
			reg_val = EDDP_HAL_REG32_READ(pDDB, reg_address);
			// ***** clear field PHY_ADDRESS
			reg_val &= ~(PNIP_REG_PHYCOMMAND_P1__MSK_PHY_ADDRESS<<PNIP_REG_PHYCOMMAND_P1_SHFT_PHY_ADDRESS);
			EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_val | PhyAddress | 0x00000020/*Polling enable*/);
#endif
		} // end for

		// set up base address for SYNC ParamStruct
		reg_val = (LSA_UINT32)EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_SYNC, EDDP_HAL_RAMSUB_SYNC_CMD_PARAM, 0);
		reg_val = EDDP_HALGetPnipRAMOffsetToRAM(pDDB, EDDP_HAL_RAMTYPE_SYNC, (EDD_COMMON_MEM_PTR_TYPE)reg_val);
		EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_SYNCPARAMSTRUCT_BASE, reg_val);

		// set up base address for Time-List (PNIP_REG_PHASEBASE_TIME)
		reg_val = (LSA_UINT32)EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_SYNC, EDDP_HAL_RAMSUB_SYNC_TIME_LIST, 0);
		reg_val = EDDP_HALGetPnipRAMOffsetToRAM(pDDB, EDDP_HAL_RAMTYPE_SYNC, (EDD_COMMON_MEM_PTR_TYPE)reg_val);
		EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_PHASEBASE_TIME, reg_val);

		// set up frameclasses with default values
		//EDDP-TODO:
		EDDP_SWI_write_frameclass (pDDB, (EDDP_SWI_FRAMECLASS_PTR_TYPE)EDDP_SWI_frameclass_array_ERTEC200P);

		// set Traffic_Mode register to topology based frame handling
		//EDDP-TODO:
		EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_TRAFFIC_MODE, EDDP_SWI_REG_TRAFFIC_MODE_INIT);

		// set up "UC_LearningMode"
#if 0
		if(pDDB->SWI.PortCnt > 2)
		{
			reg_val = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_UC_LEARNINGMODE);
			EDDP_SET_BIT_VALUE32 ( reg_val,
									0x0,
									PNIP_REG_UC_LEARNINGMODE__MSK_LEARNINGMODE_A,
									PNIP_REG_UC_LEARNINGMODE_SHFT_LEARNINGMODE_A)
			EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_UC_LEARNINGMODE, reg_val);
		}
		else
		{
			reg_val = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_UC_LEARNINGMODE);
			EDDP_SET_BIT_VALUE32 ( reg_val,
									0x1,
									PNIP_REG_UC_LEARNINGMODE__MSK_LEARNINGMODE_A,
									PNIP_REG_UC_LEARNINGMODE_SHFT_LEARNINGMODE_A)
			EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_UC_LEARNINGMODE, reg_val);
		}
#endif
	}

    if (Response == EDD_STS_OK)
    {
        // set all HOL_Barrier registers
    	//EDDP-TODO:  Response = EDDP_SWI_Set_HOL_Barriers(pDDB);

        if (Response == EDD_STS_OK)
        {
#if 0
            // set prio7 for Queue14 (RTC1, RTC2 and RToverUDP) 
            reg_val = ( PNIP_REG_QUEUE14_MAPPING__MSK_MAPPING_ENABLE | (7 << PNIP_REG_QUEUE14_MAPPING_SHFT_MAPPING_PRIO) );
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_QUEUE14_MAPPING, reg_val);
#endif
            // this flag is evaluated in function _EDDP_SWI_FinishPHYSetup() to see if DeviceSetup is in pending state or not
            pDDB->SWI.States.PendingDeviceSetup2 = LSA_FALSE;
        }
    }
    
    if(EDD_STS_OK == Response)
    {
        if( (EDDP_HAL_HW_HERA == EDDP_HALGetChipVersion(pDDB)) && (EDDP_HAL_HW_IF_A == EDDP_HALGetChipInterface(pDDB)) )
        {
            if ((EDD_FEATURE_ENABLE == pDSB->PortParams[0].GBitSupport))
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_SWI_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWIDeviceSetup1(): ERROR -> GBit not supported for IFA [0].GBitSupport=%d or [1].GBitSupport=%d or [2].GBitSupport=%d or [3].GBitSupport=%d",
                                  pDSB->PortParams[0].GBitSupport,
                                  pDSB->PortParams[1].GBitSupport, 
                                  pDSB->PortParams[2].GBitSupport,
                                  pDSB->PortParams[3].GBitSupport);
            }
        }
        
        if( (EDDP_HAL_HW_HERA == EDDP_HALGetChipVersion(pDDB)) && (EDDP_HAL_HW_IF_B == EDDP_HALGetChipInterface(pDDB)) )
        {
            
            if( ( (EDD_FEATURE_ENABLE != pDSB->PortParams[0].GBitSupport) && (EDD_FEATURE_DISABLE != pDSB->PortParams[0].GBitSupport) ) )
    
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_SWI_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWIDeviceSetup1(): ERROR -> Not initialized [0].GBitSupport=%d or [1].GBitSupport=%d or [2].GBitSupport=%d or [3].GBitSupport=%d",
                                  pDSB->PortParams[0].GBitSupport,
                                  pDSB->PortParams[1].GBitSupport, 
                                  pDSB->PortParams[2].GBitSupport,
                                  pDSB->PortParams[3].GBitSupport);
            }
            
            // only HWPort1 can be enabled for GBitSupport
            // HERADevMark RZ hier die richtige Verwendung der PortID pr�fen
            if( (EDD_FEATURE_ENABLE == pDSB->PortParams[1].GBitSupport) ||
                (EDD_FEATURE_ENABLE == pDSB->PortParams[2].GBitSupport) ||  
                (EDD_FEATURE_ENABLE == pDSB->PortParams[3].GBitSupport)     )   
            {
                // error
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWIDeviceSetup1(): ERROR -> EDD_FEATURE_ENABLE is set for [1].GBitSupport=%d or [2].GBitSupport=%d or [3].GBitSupport=%d",
                                  pDSB->PortParams[1].GBitSupport, 
                                  pDSB->PortParams[2].GBitSupport,
                                  pDSB->PortParams[3].GBitSupport);
            }
            
            if(EDD_FEATURE_ENABLE == pDSB->PortParams[0].GBitSupport)
            {
                if(EDDP_PHY_PORT_IS_GBIT_FALSE == pDDB->SWI.PortParams[0].IsGBitPhy)
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWIDeviceSetup1(): ERROR -> GBit enabled with non EDDP_PHY_TYPE_BCM5461S PhyType=%X",
                                      pDDB->SWI.PortParams[0].PhyType);
                }
                
                if(  pDDB->SWI.PortCnt != 1)
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWIDeviceSetup1(): ERROR -> GBit can only been enabled with one port");
                }
            }
        }
	}

    if(EDD_STS_OK == Response)
    {
    	Response = EDDP_LL_SWIDeviceSetup1(pDDB, pDSB);
    }

    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_SWIDeviceSetup1(): Response=0x%X", Response);

    return (Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_Init_Portmapping_HERA              +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
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
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDDP_CoreDevOpenRequest and initializes Portmapping.      +*/
/*+               This must been done to initialize I2C.                    +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWI_Init_Portmapping_HERA(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
	return EDD_STS_ERR_RESOURCE;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWIDeviceSetup2                        +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             EDDP_UPPER_DSB_PTR_TYPE        pDSB         +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  pDSB       : Pointer to DeviceSetupBlock                         (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK_PENDING                                        +*/
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDDP_SRV_DEV_SETUP                                        +*/
/*+               Part 2 of Setup Device for SWI.                           +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWIDeviceSetup2(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DSB_PTR_TYPE     pDSB)
{
    EDD_RSP     Response = EDD_STS_OK;

    LSA_UNUSED_ARG(pDSB);

    EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] IN: EDDP_SWIDeviceSetup2(pDDB=0x%X, pDSB=0x%X)", pDDB, pDSB);

    // Check if all PHYs are already ready after reset (continue with device setup)
    if (pDDB->SWI.States.ReadyPHYsCnt == pDDB->SWI.PortCnt)
    {
        // Setup of PHYs is finished - nothing to do
    }
    else
    {
        // wait for call of callback function in EDDP_SWI_FinishPHYSetup()
        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H:--] EDDP_SWIDeviceSetup2(): ReadyPHYsCnt(%d) - Setup of PHYs is not yet ready by now. Suspend DeviceSetup until PHY interrupt occurs", 
            pDDB->SWI.States.ReadyPHYsCnt);
        EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H:--] EDDP_SWIDeviceSetup2(): Hint: If setup does not continue you might have set up wrong PHY addresses!");

        pDDB->SWI.States.PendingDeviceSetup2    = LSA_TRUE;
        Response                                = EDD_STS_OK_PENDING;
    }

    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_SWIDeviceSetup2(): Response=0x%X", Response);

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWIDeviceIFSetup                       +*/
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
/*+               EDDP_SRV_DEV_IF_SETUP                                     +*/
/*+               Setup Interface of SWI                                    +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWIDeviceIFSetup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DSB_PTR_TYPE	    pDSB)
{
    EDD_RSP         Response = EDD_STS_OK;


    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_SWIDeviceIFSetup(pDSB=0x%X)", pDSB);

    // IF_MAC_Address shall not be MC Mac!
    if (EDDP_IS_MC_MAC(pDSB->IF_MAC_Address))
    {
        EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWIDeviceIFSetup(): FATAL -> IF_MAC_Address is MC Mac!");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    
    // copy IF MAC address from DSB to DDB
    pDDB->SWI.IF_MAC_Address = pDSB->IF_MAC_Address;

    // set up parameters for EDDP_CMD_FDB_INSERT_UC_ENTRY command
    pDDB->SWI.CommandParams.InsertUC_Entry.Cbf                                                    = LSA_NULL;
    pDDB->SWI.CommandParams.InsertUC_Entry.CmdOpcode                                              = EDDP_CMD_FDB_INSERT_UC_ENTRY;
    pDDB->SWI.CommandParams.InsertUC_Entry.param_union.Fdb.InsertUCEntry.Aging                    = LSA_FALSE;
    pDDB->SWI.CommandParams.InsertUC_Entry.param_union.Fdb.InsertUCEntry.Learning_Disable         = LSA_FALSE;
    pDDB->SWI.CommandParams.InsertUC_Entry.param_union.Fdb.InsertUCEntry.Mgmt_Enable              = LSA_FALSE;
    pDDB->SWI.CommandParams.InsertUC_Entry.param_union.Fdb.InsertUCEntry.Mgmt_Prio                = LSA_FALSE;
    pDDB->SWI.CommandParams.InsertUC_Entry.param_union.Fdb.InsertUCEntry.RxPort                   = 0x00;
    pDDB->SWI.CommandParams.InsertUC_Entry.param_union.Fdb.InsertUCEntry.TxPort                   = 0x01;
    pDDB->SWI.CommandParams.InsertUC_Entry.param_union.Fdb.InsertUCEntry.Static                   = LSA_TRUE;
    pDDB->SWI.CommandParams.InsertUC_Entry.param_union.Fdb.InsertUCEntry.UC_MAC_Address.MacAdr[0] = pDDB->SWI.IF_MAC_Address.MacAdr[0];
    pDDB->SWI.CommandParams.InsertUC_Entry.param_union.Fdb.InsertUCEntry.UC_MAC_Address.MacAdr[1] = pDDB->SWI.IF_MAC_Address.MacAdr[1];
    pDDB->SWI.CommandParams.InsertUC_Entry.param_union.Fdb.InsertUCEntry.UC_MAC_Address.MacAdr[2] = pDDB->SWI.IF_MAC_Address.MacAdr[2];
    pDDB->SWI.CommandParams.InsertUC_Entry.param_union.Fdb.InsertUCEntry.UC_MAC_Address.MacAdr[3] = pDDB->SWI.IF_MAC_Address.MacAdr[3];
    pDDB->SWI.CommandParams.InsertUC_Entry.param_union.Fdb.InsertUCEntry.UC_MAC_Address.MacAdr[4] = pDDB->SWI.IF_MAC_Address.MacAdr[4];
    pDDB->SWI.CommandParams.InsertUC_Entry.param_union.Fdb.InsertUCEntry.UC_MAC_Address.MacAdr[5] = pDDB->SWI.IF_MAC_Address.MacAdr[5];

    /* ---------------------------------------------------------------------------- */
    /* Software workaround for PNIP Learning bug    -> START                        */
    /* synchron command "INSERT_UC_ENTRY"                                           */
    /* ---------------------------------------------------------------------------- */
    /*  1) store current "Learning" settings                                        */
    /*  2) disable Learning in registers UC_Control, MC_Control and IRT_Control     */
    /*  3) wait 125�s                                                               */
    /*  4) FDB command "INSERT_UC_ENTRY" to PN-IP                                   */
    /*  5) restore "Learning" settings after command                                */
    /* ---------------------------------------------------------------------------- */ 

    // set up own IF MAC address in UC Table
    Response = EDDP_CmdRequest (pDDB,&(pDDB->SWI.CommandParams.InsertUC_Entry));
    EDDP_ASSERT_FALSE(Response != EDD_STS_OK);

    pDDB->SWI.CommandParams.ClearUC_Table.param_union.Fdb.ClearUCTable.isCmdRunning = LSA_FALSE;
#if 0
    // set up MAC address registers for RxPort filtering
    // Setup UC Aging
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MAC_SA_LOW_IFA, (LSA_UINT32)(pDDB->SWI.IF_MAC_Address.MacAdr[5])       |
                                                        (LSA_UINT32)(pDDB->SWI.IF_MAC_Address.MacAdr[4]) << 8  |
                                                        (LSA_UINT32)(pDDB->SWI.IF_MAC_Address.MacAdr[3]) << 16 |
                                                        (LSA_UINT32)(pDDB->SWI.IF_MAC_Address.MacAdr[2]) << 24   );

    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MAC_SA_HIGH_IFA, (LSA_UINT32)(pDDB->SWI.IF_MAC_Address.MacAdr[1])       |
                                                                (LSA_UINT32)(pDDB->SWI.IF_MAC_Address.MacAdr[0]) << 8   );


    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_UC_AGING_A, EDDP_SWI_REG_UC_AGING_DEFAULT_INIT);
    
    // set up all phase list register (PNIP_REG_PHASEBASE_RXD_PX and PNIP_REG_PHASEBASE_TXD_PX)
    EDDP_SWI_Setup_Phase_List_Base_Addr(pDDB);  

    if (Response == EDD_STS_OK)
    {
       //EDDP-TODO:
    	Response = _EDDP_SWISetPortBarrier (pDDB, pDSB);
    }
#endif
    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_SWIDeviceIFSetup(): Response=0x%X", Response);

    return (EDD_STS_OK);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWIDeviceIFShutdown                    +*/
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
/*+               EDDP_SRV_DEV_IF_SHUTDOWN or EDDP_SRV_DEV_IF_SETUP (only if+*/
/*+               previous setup shall be aborted)                          +*/
/*+               Shutdown Interface of SWI                                 +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWIDeviceIFShutdown(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP         Response;
    LSA_UINT32      HwPortID;
    LSA_UINT32      i;

    EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_SWIDeviceIFShutdown()");

    Response = EDD_STS_OK;

    // set all Ports to EDD_PORT_STATE_DISABLE
    for ( i = 1; i <= pDDB->SWI.PortCnt; i++ )
    {
        HwPortID = EDDP_SWI_get_HwPortID (pDDB, i);
        EDDP_SWI_Set_Port_State_HW (pDDB, EDD_PORT_STATE_DISABLE, HwPortID);
        pDDB->SWI.LinkStatus[HwPortID].PortState = EDD_PORT_STATE_DISABLE;
        // disable PHY
        Response = pDDB->SWI.PHY_FunctionPointers[HwPortID].EDDP_PHY_SetPowerDown(pDDB->hDDB, pDDB->hSysDev, HwPortID, LSA_TRUE);
        EDDP_ASSERT_FALSE(Response != EDD_STS_OK);
    }

    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_SWIDeviceIFShutdown(): Response=0x%X", Response);

    return (Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWIDeviceShutdown                      +*/
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
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDDP_SRV_DEV_SHUTDOWN or EDDP_SRV_DEV_SETUP (only if      +*/
/*+               previous setup shall be aborted)                          +*/
/*+               Shutdown Interface of SWI                                 +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWIDeviceShutdown(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP             Response;
    LSA_UINT32          PortID, HwPortID;
    LSA_UINT32          reg_val, reg_address;

    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_SWIDeviceShutdown(pDDB=0x%X)", pDDB);

    Response = EDD_STS_OK;

    /* ---------------------------------------- */
    /* Disable PHY_Control (pull reset)         */
    /* ---------------------------------------- */
	reg_val = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_PHY_CONTROL);
	EDDP_SET_BIT_VALUE32(reg_val, 0x00000000, PNIP_R2_REG_PHY_CONTROL__MSK_PHY_ACTIVATE, PNIP_R2_REG_PHY_CONTROL_SHFT_PHY_ACTIVATE);
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_PHY_CONTROL, reg_val);

    /* ---------------------------------------- */
    /* Disable MACControl_Px.Transmit/Receive   */
    /* ---------------------------------------- */
    {
        for ( PortID=1; PortID <= pDDB->SWI.PortCnt; PortID++ )
        { 
            HwPortID = EDDP_SWI_get_HwPortID (pDDB, PortID);

            reg_address = (HwPortID==1 ? PNIP_REG_MACCONTROL_P1 : 
                          (HwPortID==2 ? PNIP_REG_MACCONTROL_P2 : 
                          (HwPortID==3 ? PNIP_REG_MACCONTROL_P3 : 
                          (HwPortID==4 ? PNIP_REG_MACCONTROL_P4 : 0))));
            reg_val = EDDP_HAL_REG32_READ(pDDB, reg_address);
            EDDP_SET_BIT_VALUE32(reg_val, 0x00000000, PNIP_REG_MACCONTROL_P1__MSK_TRANSMIT_ENABLE, PNIP_REG_MACCONTROL_P1_SHFT_TRANSMIT_ENABLE);
            EDDP_SET_BIT_VALUE32(reg_val, 0x00000000, PNIP_REG_MACCONTROL_P1__MSK_RECEIVE_ENABLE, PNIP_REG_MACCONTROL_P1_SHFT_RECEIVE_ENABLE);
            EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_val);
        }
    }

    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_SWIDeviceShutdown(): Response=0x%X", Response);

    return (Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWIDeviceClose                         +*/
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
/*+               Close device for SWI                                      +*/
/*+                                                                         +*/
/*+       Note:   EDD_STS_OK_PENDING not allowed as response. This call     +*/
/*+               must be synchronous!                                      +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWIDeviceClose(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP     Response = EDD_STS_OK;

    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_SWIDeviceClose(pDDB=0x%X)", pDDB);

    // free memory for shadow MC table
    if (LSA_HOST_PTR_ARE_EQUAL(pDDB->SWI.pMC_Table_Shadow, LSA_NULL))
    {
        // the pointer is already deleted !
    }
    else
    {
        EDDP_GlbFreeLocalMem (pDDB->SWI.pMC_Table_Shadow);
        pDDB->SWI.pMC_Table_Shadow = LSA_NULL;
    }

    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_SWIDeviceClose(): Response=0x%X", Response);

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_SWI_PHY_WaitFree                      +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB			+*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock               (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:  utility function for EDDP_SWI_PHY_Reg_Read/Write()       +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define EDDP_SWI_PHY_IO_TIMEOUT 1000
#define EDDP_SWI_PHY_IO_RETRY   5
EDD_RSP  EDDP_LOCAL_FCT_ATTR  _EDDP_SWI_PHY_WaitFree(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT32 nTimeOut, nRetry = EDDP_SWI_PHY_IO_RETRY;

    // ***** Wait until Busy flag is reset or detect a timeout
    while(nRetry)
    {
        nTimeOut = EDDP_SWI_PHY_IO_TIMEOUT;
        while (nTimeOut)
        {
            if ( EDDP_HAL_REG32_READ(pDDB, PNIP_REG_MD_CA) & 0x0800 )
            {   // ***** busy
                nTimeOut--;
            }
            else
            {   // ***** free/idle
                return EDD_STS_OK;
            }
        }
        EDDP_HALWaitNsTicks(pDDB, EDDP_HAL_10US_TO);
        nRetry--;
    }
    // ***** Error handling out of this function or run here into a FATAL  !!!
    EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] _EDDP_SWI_PHY_WaitFree(): FATAL -> critical timeout for EDDP_SWI_PHY_Reg_Read/Write()!");
    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    return EDD_STS_ERR_TIMEOUT;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_PHY_Reg_Write                      +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB			+*/
/*+						   :	LSA_UINT32					HwPortID		+*/
/*+						   :	LSA_UINT32					RegisterNumber	+*/
/*+						   :	LSA_UINT32					Value			+*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock               (in)  +*/
/*+  HwPortID       : Hardware port ID                                (in)  +*/
/*+  RegisterNumber : PHY register number (dezimal)                   (in)  +*/
/*+  Value          : Write register value                            (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:  Writes 'Value' to PHY register 'RegisterNumber' on Port	+*/
/*+				  'HwPortID'.												+*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP  EDDP_LOCAL_FCT_ATTR  EDDP_SWI_PHY_Reg_Write(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32			 	    HwPortID,
    LSA_UINT32				    RegisterNumber,
    LSA_UINT32				    Value)
{
    EDD_RSP Status;
    // ***** Wait for Busy flag is reset
    Status = _EDDP_SWI_PHY_WaitFree(pDDB);

    if ( Status == EDD_STS_OK )
    {
        // ***** Write data into MD_Data register
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MD_DATA, Value);

        // ***** Write MD_CA to invoke SMI write command
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MD_CA, (RegisterNumber | (pDDB->SWI.PortParams[HwPortID].SMI_PHY_Address << 5) | 0x0C00/*Write + Busy*/ | (0x0020 << 12)/*WaitTime_Preamble = standard*/));
    }
    return Status;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_PHY_Reg_Read                       +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB			+*/
/*+						   :	LSA_UINT32					HwPortID		+*/
/*+						   :	LSA_UINT32					RegisterNumber	+*/
/*+						   :	LSA_UINT32				   *pValue			+*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock               (in)  +*/
/*+  HwPortID       : Hardware port ID                                (in)  +*/
/*+  RegisterNumber : PHY register number (dezimal)                   (in)  +*/
/*+  pValue         : Read register value                             (out  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:   Reads PHY register 'RegisterNumber' on Port 'HwPortID'	+*/
/*+ 				into *pValue.											+*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP  EDDP_LOCAL_FCT_ATTR  EDDP_SWI_PHY_Reg_Read(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32				    HwPortID,
    LSA_UINT32				    RegisterNumber,
    LSA_UINT32*                 pValue)
{
    EDD_RSP Status;

    // ***** Wait for Busy flag is reset
    Status = _EDDP_SWI_PHY_WaitFree(pDDB);

    if ( Status == EDD_STS_OK )
    {
        // ***** Write MD_CA to invoke SMI write command
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MD_CA, (RegisterNumber | (pDDB->SWI.PortParams[HwPortID].SMI_PHY_Address << 5) | 0x0800/*Read + Busy*/ | (0x0020 << 12)/*WaitTime_Preamble = standard*/));

        // ***** Wait for Busy flag is reset
        Status = _EDDP_SWI_PHY_WaitFree(pDDB);

        // ***** read data
        *pValue = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_MD_DATA);
    }

    return Status;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWIRequest                             +*/
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
/*+     LSA_RESULT              Status:     Return status                   +*/
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:     Depends on service              +*/
/*+                                                                         +*/
/*+  RQB-return values via callback-function depend on service              +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function handles the following services:				+*/
/*+                                                                         +*/
/*+               EDD_SRV_SWITCH_GET_PARAMS                                 +*/
/*+               EDD_SRV_SWITCH_SET_PORT_STATE                             +*/
/*+               EDD_SRV_SWITCH_FLUSH_FILTERING_DB							+*/
/*+																			+*/
/*+               If there's a parameter error we will set EDD_STS_ERR_PARAM+*/
/*+               and call the handle-callbackfunction and abort the request+*/
/*+                                                                         +*/
/*+               EDD_STS_ERR_SERVICE is set if we get an invalid service.  +*/
/*+               EDD_STS_ERR_CHANNEL_USE is set if the service is invalid  +*/
/*+               for this channel (i.e. not supported by this channel)     +*/
/*+                                                                         +*/
/*+               If the request is valid, it is queued or executed.        +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWIRequest(
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

    EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] >>> EDDP_SWIRequest(pRQB=0x%X)", pHDB->Handle, pRQB);

    switch (Service)
    {
        //-------------------------------------------------------------
        case EDD_SRV_SWITCH_SET_PORT_STATE:
        //-------------------------------------------------------------
        {  
            EDDP_UPPER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_SWITCH_SET_PORT_STATE", pHDB->Handle);
            Response = EDDP_SWI_Request_Set_Port_State (pRQB, pDDB, pHDB);
        }
        break;

        //-------------------------------------------------------------
        case EDD_SRV_SWITCH_FLUSH_FILTERING_DB:
        //-------------------------------------------------------------
        {   
            LSA_UINT16                                  UserPortID  = 0;
            LSA_UINT32                                  iPort;
            LSA_BOOL                                    bBreak      = LSA_FALSE;
            EDD_UPPER_SWI_FLUSH_FILTERING_DB_PTR_TYPE   pRqbParam   = (EDD_UPPER_SWI_FLUSH_FILTERING_DB_PTR_TYPE) pRQB->pParam;

            EDDP_UPPER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_SWITCH_FLUSH_FILTERING_DB - ClearUCTable", pHDB->Handle);

            /* -------------------------------- */
            /* check RQB parameters             */
            /* -------------------------------- */
            if (LSA_HOST_PTR_ARE_EQUAL(pRqbParam, LSA_NULL))
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_SWIRequest(EDD_SRV_SWITCH_FLUSH_FILTERING_DB): ERROR -> RQB->pParam=NULL, Response=0x%X", 
                    pHDB->Handle, Response);
                break;  // exit case FLUSH_FILTERING_DB
            }

            if ((pRqbParam->PortCnt < 1) || (pRqbParam->PortCnt > pDDB->SWI.PortCnt))
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_SWIRequest(EDD_SRV_SWITCH_FLUSH_FILTERING_DB): ERROR -> RQB.PortCnt(%d) is invalid", 
                    pHDB->Handle, pRqbParam->PortCnt);
                break;  // exit case FLUSH_FILTERING_DB
            }

            for ( iPort=0; iPort < pRqbParam->PortCnt; iPort++ )
            {
                UserPortID = pRqbParam->PortID[iPort];

                if ((UserPortID < 1) || (UserPortID > pDDB->SWI.PortCnt))
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_SWIRequest(EDD_SRV_SWITCH_FLUSH_FILTERING_DB): ERROR -> RQB.PortID(%d) is invalid", 
                        pHDB->Handle, UserPortID);
                    bBreak = LSA_TRUE;
                    break; // exit FOR loop
                }

                if ( (pDDB->SWI.LinkStatus[UserPortID].PortState != EDD_PORT_STATE_BLOCKING) && (pDDB->SWI.LinkStatus[UserPortID].PortState != EDD_PORT_STATE_DISABLE) )
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_SWIRequest(EDD_SRV_SWITCH_FLUSH_FILTERING_DB): ERROR -> current PortState(0x%X) is not BLOCKING or DISABLE", 
                        pHDB->Handle, pDDB->SWI.LinkStatus[UserPortID].PortState);
                    bBreak = LSA_TRUE;
                    break; // exit FOR loop
                }
            }

            if (bBreak)
            {
                break;  // exit case FLUSH_FILTERING_DB
            }

            /* -------------------------------- */
            /* execute the service              */
            /* -------------------------------- */
            isRequestFinish = LSA_FALSE;

            // if this command is already running we queue the RQB and try it again after the current command has finished.
            if (pDDB->SWI.CommandParams.ClearUC_Table.InUse)
            {    
                EDDP_GlbRQBQueueAddToEnd (&(pDDB->SWI.WaitingRQBs_FDB), pRQB);
                pHDB->pSWI->RunningRQBCount++;
            }
            else
            {
                // queue this RQB in DDB to remember it while the command is running on the PNIP hardware
                EDDP_GlbRQBQueueAddToEnd (&(pDDB->SWI.PendingRQBs_FDB), pRQB);
                pHDB->pSWI->RunningRQBCount++;

                // fill parameter structure
                pDDB->SWI.CommandParams.ClearUC_Table.param_union.Fdb.ClearUCTable.Delete_Only_Dynamic_Entries        = LSA_TRUE;
                pDDB->SWI.CommandParams.ClearUC_Table.param_union.Fdb.ClearUCTable.Delete_Only_Dynamic_Entries_Port1  = LSA_FALSE;  // LSA_TRUE:  delete only port1; LSA_FALSE: delete all ports or port1 not delete 
                pDDB->SWI.CommandParams.ClearUC_Table.param_union.Fdb.ClearUCTable.Delete_Only_Dynamic_Entries_Port2  = LSA_FALSE;  // LSA_TRUE:  delete only port2; LSA_FALSE: delete all ports or port2 not delete 
                pDDB->SWI.CommandParams.ClearUC_Table.param_union.Fdb.ClearUCTable.Delete_Only_Dynamic_Entries_Port3  = LSA_FALSE;  // LSA_TRUE:  delete only port3; LSA_FALSE: delete all ports or port3 not delete 
                pDDB->SWI.CommandParams.ClearUC_Table.param_union.Fdb.ClearUCTable.Delete_Only_Dynamic_Entries_Port4  = LSA_FALSE;  // LSA_TRUE:  delete only port4; LSA_FALSE: delete all ports or port4 not delete 
                pDDB->SWI.CommandParams.ClearUC_Table.Cbf         = EDDP_SWI_CommandDoneCbf_FDB;
                pDDB->SWI.CommandParams.ClearUC_Table.CmdOpcode   = EDDP_CMD_FDB_CLEAR_UC_TABLE;


                /* ---------------------------------------------------------------------------- */
                /* Software workaround for PNIP Learning bug    -> START                        */
                /* asynchron command "CLEAR_UC_TABLE"                                           */
                /* ---------------------------------------------------------------------------- */
                /*  1) store current "Learning" settings                                        */
                /*  2) disable Learning in registers UC_Control, MC_Control and IRT_Control     */
                /*  3) lock accesses to these registers                                         */
                /*  4) FDB command "INSERT_UC_ENTRY" to PN-IP                                   */
                /*  5) restore "Learning" settings after command                                */
                /*  6) unlock accesses to these registers                                       */
                /* ---------------------------------------------------------------------------- */

                // trigger command (runs asynchronously)
                Response = EDDP_CmdRequest (pDDB, &(pDDB->SWI.CommandParams.ClearUC_Table));
                EDDP_ASSERT_FALSE(Response != EDD_STS_OK_PENDING);
            }
        }
        break;

        //-------------------------------------------------------------
        default:
        //-------------------------------------------------------------
        {
            Response = EDD_STS_ERR_SERVICE;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_SWIRequest(): ERROR -> Unknown/Unsupported Service(0x%X)", 
                pHDB->Handle, Service);
        }
        break;
    }

    if (isRequestFinish)
    {
        EDDP_GlbRequestFinish (pHDB, pRQB, Response);
    }

    EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] <<< EDDP_SWIRequest(): Response=0x%X", pHDB->Handle, Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_CommandDoneCbf_FDB                 +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB			+*/
/*+						   :	EDDP_CMD_PARAM_PTR_TYPE       pParam        +*/
/*+						   :	EDD_RSP                       Response	    +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  pParam     : Pointer to parameter structure                      (in)  +*/
/*+  Response   : Response code for the confirmed command             (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:  This function is called when a FDB command has finished  +*/
/*+                which was invoked by SWI. The corrensponding RQB is sent +*/
/*+                back to the user and if there is a waiting (queued) RQB  +*/
/*+ 			   it is executed.											+*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_CommandDoneCbf_FDB(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_CMD_PARAM_PTR_TYPE     pParam,
    EDD_RSP                     ResponseCmd)
{
    EDD_UPPER_RQB_PTR_TYPE      pRQB;
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB;

    EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_SWI_CommandDoneCbf_FDB(pParam=0x%X, ResponseCmd=0x%X): CmdOpcode=0x%X", 
        pParam, ResponseCmd, pParam->CmdOpcode);

    if (ResponseCmd != EDD_STS_OK)
    {
        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_CommandDoneCbf_FDB(): FATAL -> ResponseCmd(0x%X) not Ok!", ResponseCmd);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        /* ------------------------ */
        /* get pending RQB          */
        /* ------------------------ */
        pRQB = EDDP_GlbRQBQueueRemoveFromBegin (&(pDDB->SWI.PendingRQBs_FDB));

        if (LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL))
        {
            EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_CommandDoneCbf_FDB(): FATAL -> There is no pending RQBs!");
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        else
        {
            /* ---------------------------------------------------------------------------- */
            /* Software workaround for PNIP Learning bug    -> Continuation                 */
            /* asynchron command "CLEAR_UC_TABLE"                                           */
            /*  5) restore "Learning" settings after command                                */
            /*  6) unlock accesses to these registers                                       */
            /* ---------------------------------------------------------------------------- */

            /* ---------------------------------------------------------------------------- */
            /* Software workaround for PNIP Learning bug    -> END                          */
            /* ---------------------------------------------------------------------------- */

            // get handle of this RQB
            // NOTE: no need to check the result of EDDP_GlbHandleGetHDB, because this RQB 
            //       and its HDB was previously checked before the RQB was queued --> ASSERT is OK!
            EDDP_GlbHandleGetHDB (EDD_RQB_GET_HANDLE_LOWER(pRQB), &pHDB);
            EDDP_ASSERT_NO_NULL_PTR(pHDB);

            EDDP_ASSERT_FALSE((pHDB->pSWI->RunningRQBCount) == 0);
            // RQB is no more in queue of this handle
            pHDB->pSWI->RunningRQBCount--;

            // send RQB back to user
            EDDP_GlbRequestFinish(pHDB, pRQB, ResponseCmd);
        }

        /* ------------------------ */
        /* get next waiting RQB     */
        /* ------------------------ */
        pRQB = EDDP_GlbRQBQueueRemoveFromBegin (&(pDDB->SWI.WaitingRQBs_FDB));

        if (LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL))
        {
            // there is no waiting RQB
        }
        else
        {
            // get handle of this RQB
            // NOTE: no need to check the result of EDDP_GlbHandleGetHDB, because this RQB 
            //       and its HDB was previously checked before the RQB was queued --> ASSERT is OK!
            EDDP_GlbHandleGetHDB (EDD_RQB_GET_HANDLE_LOWER(pRQB), &pHDB);
            EDDP_ASSERT_NO_NULL_PTR(pHDB);

            EDDP_ASSERT_FALSE((pHDB->pSWI->RunningRQBCount) == 0);
            // RQB is no more in queue of this handle
            pHDB->pSWI->RunningRQBCount--;

            // execute next waiting RQB. Important: Use pHDB->pDDB which was extracted from the waiting RQB!
            EDDP_SWIRequest (pRQB, pHDB->pDDB, pHDB);
        }
    }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_CommandDoneCbf_GSYNC               +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB			+*/
/*+						   :	EDDP_CMD_PARAM_PTR_TYPE       pParam        +*/
/*+						   :	EDD_RSP                       Response	    +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  pParam     : Pointer to parameter structure                      (in)  +*/
/*+  Response   : Response code for the confirmed command             (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:  This function is called when a GSYNC command has finished+*/
/*+                which was invoked by SWI. The corrensponding RQB is sent +*/
/*+                back to the user and if there is a waiting (queued) RQB  +*/
/*+ 			   it is executed.											+*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_CommandDoneCbf_GSYNC(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_CMD_PARAM_PTR_TYPE     pParam,
    EDD_RSP                     ResponseCmd)
{
    EDD_SERVICE                         Service;
    __attribute__((unused))LSA_UINT32                          RegCycleLength;

    EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_SWI_CommandDoneCbf_GSYNC(pParam=0x%X, ResponseCmd=0x%X): CmdOpcode=0x%X", 
        pParam, ResponseCmd, pParam->CmdOpcode);

    if (ResponseCmd != EDD_STS_OK)
    {
        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_CommandDoneCbf_GSYNC(): FATAL -> ResponseCmd(0x%X) not Ok!", 
            ResponseCmd);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {        
        EDDP_SWI_SEND_CLOCK_CHANGE_SM_PTR_TYPE pStateMachine;
        EDD_UPPER_RQB_PTR_TYPE                 pRQB;
        
        /* get RQB from send clock change state machine */
        pStateMachine   = &pDDB->SWI.SendClockChangeSm;
        pRQB            = pStateMachine->pRQB;
        
        /* RQB shall not be empty! */
        if (LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL))
        {
            EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_CommandDoneCbf_GSYNC(): FATAL -> No RQB in work!");
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        else
        {                        
            Service = EDD_RQB_GET_SERVICE(pRQB);

            switch (Service)
            {
                /* ------------------------------------------------ */
                /* change the COM-Cycle (send clock)                */
                /* ------------------------------------------------ */
                case EDD_SRV_SENDCLOCK_CHANGE:
                {
                    if (pParam->CmdOpcode == EDDP_CMD_GSYNC_UPDATE_COMCYCL)
                    {
                        
                        // Update_COMCYCLE was successful -> store new parameter in DDB (used by CRT)
                        pDDB->SWI.CycleInfo.CycleBaseFactor = ((EDD_UPPER_SENDCLOCK_CHANGE_PTR_TYPE)pRQB->pParam)->CycleBaseFactor;
                        RegCycleLength                      = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_CYCLELENGTH_A);

                        EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_SWI_CommandDoneCbf_GSYNC(Service=0x%X): new CycleBaseFactor=%d, RegCycleLength-IFA=%dns", 
                            Service, pDDB->SWI.CycleInfo.CycleBaseFactor, RegCycleLength);

                        // set new value of MRT_RR_ReloadValue
                        // set FrameAgingCycle, only Rev3 (only for RTC1)

						{
							LSA_UINT32      MRT_RR_ReloadValue = EDDP_CRTGetConsMrtRrReloadValue (pDDB);
							/* PNIP_R2_REG_MRT_RR_RELOADVALUE_IFA was replaced by PNIP_R3_REG_MRT_RR_RELOADVALUE_IFA, there is no R3 type of this define*/
							EDDP_HAL_REG32_WRITE(pDDB, PNIP_R3_REG_MRT_RR_RELOADVALUE_IFA, MRT_RR_ReloadValue);
						}


                        // go back to state machine
                        EDDP_SWISendClockChangeStateMachineCbf (pDDB, EDD_STS_OK);
                    }
                    else
                    {
                        EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_CommandDoneCbf_GSYNC(Service=0x%X): FATAL -> unknown CmdOpcode(0x%X) for this service!", 
                            Service, pParam->CmdOpcode);
                        EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                    }
                }
                break;

                /* ------------------------------------------------ */
                /* Unsupported Request in this case of Cbf          */
                /* ------------------------------------------------ */
                default:
                {
                    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_CommandDoneCbf_GSYNC(): FATAL -> Unsupported Service(0x%X) in this case of Cbf!", 
                        Service);
                    EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                }
                break;
            }
        }
    }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_FinishPHYSetup                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB			+*/
/*+						   :	LSA_UINT32					HwPortID		+*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  HwPortID   : Hardware port ID                                    (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:  When a PHY becomes operational after reset was released 	+*/
/*+ 			   this function is called due to an INT_StatusChange 		+*/
/*+                interrupt. We request PHY function pointers from system  +*/
/*+                adaption and set up the polling mechanism for link, speed+*/
/*+                and duplexity changes.                                   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_FinishPHYSetup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32				    HwPortID,
    LSA_BOOL                    bKeepPhyStatus)
{
    EDD_RSP                 Status = EDD_STS_OK;
    LSA_UINT32              i;
    LSA_UINT32              reg_val;
    __attribute__((unused))LSA_UINT32              reg_address;
    LSA_UINT32              IFId, PortID;
    EDDP_PHY_INIT_TYPE  	InitData;
    EDDP_PHY_FCT_PTR_TYPE	pFunctionPointers;
    EDDP_PHY_TYPE           PHYType;

    EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_SWI_FinishPHYSetup(HwPortID=%d): PendingDeviceSetup2=0x%X", 
        HwPortID, pDDB->SWI.States.PendingDeviceSetup2);

    i = HwPortID; // just to shorten variable name..
    // get PortID that is mapped to HwPortID (hint: all arrays in DDB use PortID as index and never HwPortID)
    PortID = EDDP_SWI_get_PortID_and_IF(pDDB, HwPortID, &IFId);
#if 0
    // select the right register address for current HwPortID "i"
    reg_address = (i==1 ? PNIP_REG_PHYOUI12_P1 : 
                  (i==2 ? PNIP_REG_PHYOUI12_P2 : 
                  (i==3 ? PNIP_REG_PHYOUI12_P3 : 
                  (i==4 ? PNIP_REG_PHYOUI12_P4 : 0))));
    // read PHYOUI12_PX which contains the vendor ID of this PHY
    reg_val = EDDP_HAL_REG32_READ(pDDB, reg_address);
#endif
    // get PHY function pointers for this PHY from system adaption
		reg_val = 0; // variable is not used inside EDDP_PHY_GetFunctionPointers.
    EDDP_PHY_GetFunctionPointers(pDDB->hDDB, pDDB->hSysDev, HwPortID, reg_val, &(pDDB->SWI.PHY_FunctionPointers[HwPortID]), &PHYType);    
    pFunctionPointers = &pDDB->SWI.PHY_FunctionPointers[HwPortID];

    // check if no function pointer is null
    if ((0 ==  pFunctionPointers->EDDP_PHY_Init)                      ||
        (0 ==  pFunctionPointers->EDDP_PHY_LoadDelayValues)           ||
        (0 ==  pFunctionPointers->EDDP_PHY_GetLinkStatus)             ||
        (0 ==  pFunctionPointers->EDDP_PHY_LED_BlinkBegin)            ||
        (0 ==  pFunctionPointers->EDDP_PHY_LED_BlinkEnd)              ||
        (0 ==  pFunctionPointers->EDDP_PHY_LED_BlinkSetMode)          ||
        (0 ==  pFunctionPointers->EDDP_PHY_SetSpeedDuplexityOrAutoneg)||
        (0 ==  pFunctionPointers->EDDP_PHY_CheckMAUType)              ||
        (0 ==  pFunctionPointers->EDDP_PHY_SetPowerDown)              ||
        (0 ==  pFunctionPointers->EDDP_PHY_CheckPowerDown))
    {
        EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_FinishPHYSetup(): FATAL -> One or more PHY function pointers are NULL!");
        EDDP_DBG_SET_DETAIL_ERR(pDDB, EDDP_FATAL_ERR_NULL_PTR);        
        EDDP_FATAL(EDDP_FATAL_ERR_NULL_PTR);
        return; //won't come here after fatal error!
    }
    // get PHY init parameters from systemadaption and set some PHY cfg parameter
    Status = pFunctionPointers->EDDP_PHY_Init(pDDB->hDDB, pDDB->hSysDev, pDDB->Hardware, i, pDDB->SWI.PortParams[i].IsWireless, pDDB->SWI.PortParams[i].IsMDIX, &InitData);
    EDDP_ASSERT_FALSE(Status != EDD_STS_OK);
#if 1
    // disable PHY (actually only necessary on PHYs that do not power up in isolate mode)
    Status = pDDB->SWI.PHY_FunctionPointers[HwPortID].EDDP_PHY_SetPowerDown(pDDB->hDDB, pDDB->hSysDev, HwPortID, LSA_TRUE);
    EDDP_ASSERT_FALSE(Status != EDD_STS_OK);
#endif
    if(!bKeepPhyStatus)
    {
        pDDB->SWI.LinkStatus[PortID].PhyStatus = EDD_PHY_STATUS_DISABLED;
    }

    pDDB->SWI.PortParams[HwPortID].PhyType = PHYType;

    // whenever a PHY gets ready after release of PHY reset we increment a counter
    pDDB->SWI.States.ReadyPHYsCnt++;

    // When all PHYs are ready we signal this to the CORE component by calling a callback function.
    // This is only done if the function EDDP_SWIDeviceSetup2 returns with EDD_STS_OK_PENDING and therefore is waiting for us.
    if ((pDDB->SWI.States.ReadyPHYsCnt == pDDB->SWI.PortCnt) && (pDDB->SWI.States.PendingDeviceSetup2))
    {    
        // trigger Core device setup state machine
        EDDP_CoreCBFDeviceSetup (pDDB, Status);
    }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_Request_Set_Port_State             +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                        :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                        :    EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters          (in) +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function sets the requested port states. 			+*/
/*+                                                                         +*/
/*+               Possible states are:                                      +*/
/*+               - EDD_PORT_STATE_DISABLE                                  +*/
/*+               - EDD_PORT_STATE_BLOCKING                                 +*/
/*+               - EDD_PORT_STATE_LISTENING                                +*/
/*+				  - EDD_PORT_STATE_LEARNING                                 +*/
/*+               - EDD_PORT_STATE_FORWARDING                               +*/
/*+               - EDD_PORT_STATE_UNCHANGED                                +*/
/*+               - EDD_PORT_STATE_FORWARDING_WITHOUT_LEARNING              +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWI_Request_Set_Port_State(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP                                 Response;
    LSA_UINT32                              i, HwPortID;
    EDD_UPPER_SWI_SET_PORT_STATE_PTR_TYPE   pParam;

    EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_SWI_Request_Set_Port_State(pRQB=0x%X)", 
        pHDB->Handle, pRQB);

    Response    = EDD_STS_OK;
    pParam      = (EDD_UPPER_SWI_SET_PORT_STATE_PTR_TYPE) pRQB->pParam;

    for ( i=1; i <= pDDB->SWI.PortCnt; i++ )
    {
        HwPortID = EDDP_SWI_get_HwPortID (pDDB, i);

        EDDP_SWI_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_SWI_Request_Set_Port_State(UsrPortID=%d, HwPortID=%d): PortState(current=0x%X, new=0x%X), LinkStatus=0x%X", 
            pHDB->Handle, i, HwPortID, pDDB->SWI.LinkStatus[i].PortState, pParam->PortIDState[i-1], pDDB->SWI.LinkStatus[i].LinkStatus);

        if (pParam->PortIDState[i-1] == pDDB->SWI.LinkStatus[i].PortState)
        {
            // if current port state is equal to requested port state we do nothing
            // go to next port in the FOR loop
            continue;
        }

        switch (pParam->PortIDState[i-1])
        {
            case EDD_PORT_STATE_DISABLE:
            case EDD_PORT_STATE_BLOCKING:
            case EDD_PORT_STATE_LISTENING:
            case EDD_PORT_STATE_LEARNING:
            case EDD_PORT_STATE_FORWARDING:
            case EDD_PORT_STATE_FORWARDING_WITHOUT_LEARNING:
            {
                // update LinkStatus only if link is up
                if (   (pDDB->SWI.LinkStatus[i].LinkStatus == EDD_LINK_UP)
                    || (pDDB->SWI.LinkStatus[i].LinkStatus == EDD_LINK_UP_CLOSED)
                    || (pDDB->SWI.LinkStatus[i].LinkStatus == EDD_LINK_UP_DISABLED)
                   )
                {
                    EDDP_SWI_Set_Port_State_HW (pDDB, pParam->PortIDState[i-1], HwPortID);

                    if (   (pParam->PortIDState[i-1] == EDD_PORT_STATE_BLOCKING)
                        || (pParam->PortIDState[i-1] == EDD_PORT_STATE_LISTENING)
                        || (pParam->PortIDState[i-1] == EDD_PORT_STATE_LEARNING)
                       )
                    {
                        // link is up and new port state is set to "Blocking"
                        pDDB->SWI.LinkStatus[i].LinkStatus = EDD_LINK_UP_CLOSED;
                    }
                    else if (pParam->PortIDState[i-1] == EDD_PORT_STATE_DISABLE)
                    {
                        // link is up and new port state is set to "Disable"
                        pDDB->SWI.LinkStatus[i].LinkStatus = EDD_LINK_UP_DISABLED;
                    }
                    else
                    {
                        // link is up and new port state is other
                        pDDB->SWI.LinkStatus[i].LinkStatus = EDD_LINK_UP;
                    }

                    EDDP_SWI_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_SWI_Request_Set_Port_State(UsrPortID=%d, HwPortID=%d): new LinkStatus=0x%X", 
                        pHDB->Handle, i, HwPortID, pDDB->SWI.LinkStatus[i].LinkStatus);
                }

                // store new port state in DDB
                pDDB->SWI.LinkStatus[i].PortState = pParam->PortIDState[i-1];
                // fire link change indication
                EDDP_GENIndicateLinkChange_Ext (pDDB, i);
                // check if "fastest link" status has changed
                if (EDDP_GENFindFastestPort(pDDB))
                {
                    EDDP_GENIndicateLinkChange (pDDB, EDD_PORT_ID_AUTO);
                }
            }
            break;

            case EDD_PORT_STATE_UNCHANGED:
                // nothing to do
            break;

            default:
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR(pHDB->pDDB, Response);
                EDDP_SWI_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_SWI_Request_Set_Port_State(UsrPortID=%d, HwPortID=%d): ERROR -> PortIDState(0x%X) is unknown! Response=0x%X", 
                    pHDB->Handle, i, HwPortID, pParam->PortIDState[i-1], Response);
                i = pDDB->SWI.PortCnt;  // exit FOR loop
            }
            break;
        }
    }

    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_SWI_Request_Set_Port_State(): Response=0x%X", 
        Response);

    return (Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_Set_Port_State_HW                  +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                        :    LSA_UINT16                  PortState       +*/
/*+                        :    LSA_UINT32                  HwPortID        +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  PortID     : User port ID (1..4)                                  (in) +*/
/*+  PortState  : Requested port state change                          (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function sets PNIP registers according to the        +*/
/*+               requested port state for the selected HwPortID.           +*/
/*+               Possible states are:                                      +*/
/*+                                                                         +*/
/*+               EDD_PORT_STATE_DISABLE                                    +*/
/*+               EDD_PORT_STATE_BLOCKING                                   +*/
/*+               EDD_PORT_STATE_LISTENING                                  +*/
/*+				  EDD_PORT_STATE_LEARNING                                   +*/
/*+               EDD_PORT_STATE_FORWARDING                                 +*/
/*+               EDD_PORT_STATE_FORWARDING_WITHOUT_LEARNING                +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Set_Port_State_HW(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PortState,
    LSA_UINT32                  HwPortID)
{
    LSA_UINT32      reg_val, reg_address;

    /* ------------------------------------------------ */
    /* Forwarding/Blocking                              */
    /* ------------------------------------------------ */
    if ((PortState == EDD_PORT_STATE_FORWARDING) || (PortState == EDD_PORT_STATE_FORWARDING_WITHOUT_LEARNING))
    {
        /* ------------------------------------------------ */
        /* Forwarding                                       */
        /* set REG_BLOCKED_PRIOMASK to 'Forwarding'         */
        /* ------------------------------------------------ */
        reg_address = (HwPortID==1 ? PNIP_REG_BLOCKED_PRIOMASK_DP1_P1 : 
                      (HwPortID==2 ? PNIP_REG_BLOCKED_PRIOMASK_DP1_P2 : 
                      (HwPortID==3 ? PNIP_REG_BLOCKED_PRIOMASK_DP1_P3 : 
                      (HwPortID==4 ? PNIP_REG_BLOCKED_PRIOMASK_DP1_P4 : 0))));
        EDDP_HAL_REG32_WRITE(pDDB, reg_address, EDDP_SWI_BLOCKED_PRIOMASK_FORWARDING);

        reg_address = (HwPortID==1 ? PNIP_REG_BLOCKED_PRIOMASK_DP1S_P1 : 
                      (HwPortID==2 ? PNIP_REG_BLOCKED_PRIOMASK_DP1S_P2 : 
                      (HwPortID==3 ? PNIP_REG_BLOCKED_PRIOMASK_DP1S_P3 : 
                      (HwPortID==4 ? PNIP_REG_BLOCKED_PRIOMASK_DP1S_P4 : 0))));
        EDDP_HAL_REG32_WRITE(pDDB, reg_address, EDDP_SWI_BLOCKED_PRIOMASK_FORWARDING);

        reg_address = (HwPortID==1 ? PNIP_REG_BLOCKED_PRIOMASK_DP1R_P1 : 
                      (HwPortID==2 ? PNIP_REG_BLOCKED_PRIOMASK_DP1R_P2 : 
                      (HwPortID==3 ? PNIP_REG_BLOCKED_PRIOMASK_DP1R_P3 : 
                      (HwPortID==4 ? PNIP_REG_BLOCKED_PRIOMASK_DP1R_P4 : 0))));
        EDDP_HAL_REG32_WRITE(pDDB, reg_address, EDDP_SWI_BLOCKED_PRIOMASK_FORWARDING);

        reg_address = (HwPortID==1 ? PNIP_REG_BLOCKED_PRIOMASK_DP2_P1 : 
                      (HwPortID==2 ? PNIP_REG_BLOCKED_PRIOMASK_DP2_P2 : 
                      (HwPortID==3 ? PNIP_REG_BLOCKED_PRIOMASK_DP2_P3 : 
                      (HwPortID==4 ? PNIP_REG_BLOCKED_PRIOMASK_DP2_P4 : 0))));
        EDDP_HAL_REG32_WRITE(pDDB, reg_address, EDDP_SWI_BLOCKED_PRIOMASK_FORWARDING);

        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Set_Port_State_HW(HwPortID=%d): set REG_BLOCKED_PRIOMASK to 'Forwarding'!", 
            HwPortID);
    }
    else 
    {
        /* ------------------------------------------------ */
        /* Blocking                                         */
        /* set REG_BLOCKED_PRIOMASK to 'Blocking'           */
        /* ------------------------------------------------ */
        reg_address = (HwPortID==1 ? PNIP_REG_BLOCKED_PRIOMASK_DP1_P1 : 
                      (HwPortID==2 ? PNIP_REG_BLOCKED_PRIOMASK_DP1_P2 : 
                      (HwPortID==3 ? PNIP_REG_BLOCKED_PRIOMASK_DP1_P3 : 
                      (HwPortID==4 ? PNIP_REG_BLOCKED_PRIOMASK_DP1_P4 : 0))));
        EDDP_HAL_REG32_WRITE(pDDB, reg_address, EDDP_SWI_BLOCKED_PRIOMASK_BLOCKING);

        reg_address = (HwPortID==1 ? PNIP_REG_BLOCKED_PRIOMASK_DP1S_P1 : 
                      (HwPortID==2 ? PNIP_REG_BLOCKED_PRIOMASK_DP1S_P2 : 
                      (HwPortID==3 ? PNIP_REG_BLOCKED_PRIOMASK_DP1S_P3 : 
                      (HwPortID==4 ? PNIP_REG_BLOCKED_PRIOMASK_DP1S_P4 : 0))));
        EDDP_HAL_REG32_WRITE(pDDB, reg_address, EDDP_SWI_BLOCKED_PRIOMASK_BLOCKING);

        reg_address = (HwPortID==1 ? PNIP_REG_BLOCKED_PRIOMASK_DP1R_P1 : 
                      (HwPortID==2 ? PNIP_REG_BLOCKED_PRIOMASK_DP1R_P2 : 
                      (HwPortID==3 ? PNIP_REG_BLOCKED_PRIOMASK_DP1R_P3 : 
                      (HwPortID==4 ? PNIP_REG_BLOCKED_PRIOMASK_DP1R_P4 : 0))));
        EDDP_HAL_REG32_WRITE(pDDB,reg_address, EDDP_SWI_BLOCKED_PRIOMASK_BLOCKING);   

        // for PNIP_REG_BLOCKED_PRIOMASK_DP2_Px nothing to do!
        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Set_Port_State_HW(HwPortID=%d): set REG_BLOCKED_PRIOMASK to 'Blocking'!", 
            HwPortID);
    }        

    /* ------------------------------------------------ */
    /* Enable/Disable Port                              */
    /* set REG_FLOWCONTROL to 'Enabled'/'Disabled'      */
    /* ------------------------------------------------ */
    reg_address = (HwPortID==1 ? PNIP_REG_FLOWCONTROL_P1 : 
                  (HwPortID==2 ? PNIP_REG_FLOWCONTROL_P2 : 
                  (HwPortID==3 ? PNIP_REG_FLOWCONTROL_P3 : 
                  (HwPortID==4 ? PNIP_REG_FLOWCONTROL_P4 : 0))));
    reg_val = EDDP_HAL_REG32_READ(pDDB, reg_address);    

    if (PortState == EDD_PORT_STATE_DISABLE)
    {
        // disable port
        EDDP_SET_BIT_VALUE32(reg_val, 0x00000001, PNIP_REG_FLOWCONTROL_P1__MSK_DISABLEPORT, PNIP_REG_FLOWCONTROL_P1_SHFT_DISABLEPORT);
        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Set_Port_State_HW(HwPortID=%d): disable port in REG_FLOWCONTROL!", 
            HwPortID);

    }
    else
    {
        // enable port
        EDDP_CLR_BIT_VALUE32(reg_val, PNIP_REG_FLOWCONTROL_P1__MSK_DISABLEPORT);
        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Set_Port_State_HW(HwPortID=%d): enable port in REG_FLOWCONTROL!", 
            HwPortID);
    }
    EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_val);

    /* ------------------------------------------------ */
    /* Learning in register                             */
    /*  - UC_Control.UC_Learning_Enable                 */
    /*  - MC_Control.MC_Learning_Enable                 */
    /* ------------------------------------------------ */
    if ((PortState == EDD_PORT_STATE_LEARNING) || (PortState == EDD_PORT_STATE_FORWARDING))
    {
        /* ---------------------------- */
        /* Enable Learning              */
        /* ---------------------------- */
        if (EDDP_SWI_IsFdbCmdRunning(pDDB))
        {
            // one FDB command is running - store new state in SW, it will be set in HW after command is finished
            EDDP_SET_BIT_VALUE32(pDDB->SWI.CommandParams.ClearUC_Table.param_union.Fdb.ClearUCTable.UC_Learning_Enable, 1, (1 << HwPortID), HwPortID);
            EDDP_SET_BIT_VALUE32(pDDB->SWI.CommandParams.ClearUC_Table.param_union.Fdb.ClearUCTable.MC_Learning_Enable, 1, (1 << HwPortID), HwPortID);

            EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Set_Port_State_HW(HwPortID=%d): enable 'Learning' - store new state in SW, it will be set in HW after FDB command is finished!", 
                HwPortID);
        }
        else
        {
            // no FDB command is running - set new state in HW
            // enable learning in UC_Control for DP1, DP1S
            reg_val = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_UC_CONTROL);
            EDDP_SET_BIT_VALUE32(reg_val, 1, (1 << HwPortID), HwPortID);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_UC_CONTROL, reg_val);
            // enable learning in MC_Control for DP1, DP1S
            reg_val = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_MC_CONTROL);
            EDDP_SET_BIT_VALUE32(reg_val, 1, (1 << HwPortID), HwPortID);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MC_CONTROL, reg_val);

            EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Set_Port_State_HW(HwPortID=%d): enable 'Learning' in REG_UC_CONTROL and REG_MC_CONTROL!", 
                HwPortID);
        }
    }
    else
    {
        /* ---------------------------- */
        /* Disable Learning             */
        /* ---------------------------- */
        if (EDDP_SWI_IsFdbCmdRunning(pDDB))
        {
            // one FDB command is running - store new state in SW, it will be set in HW after command is finished
            EDDP_SET_BIT_VALUE32(pDDB->SWI.CommandParams.ClearUC_Table.param_union.Fdb.ClearUCTable.UC_Learning_Enable, 0, (1 << HwPortID), HwPortID);
            EDDP_SET_BIT_VALUE32(pDDB->SWI.CommandParams.ClearUC_Table.param_union.Fdb.ClearUCTable.MC_Learning_Enable, 0, (1 << HwPortID), HwPortID);

            EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Set_Port_State_HW(HwPortID=%d): disable 'Learning' - store new state in SW, it will be set in HW after FDB command is finished!", 
                HwPortID);
        }
        else
        {
            // no FDB command is running - set new state in HW
            // disable learning in UC_Control for DP1, DP1S
            reg_val = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_UC_CONTROL);
            EDDP_SET_BIT_VALUE32(reg_val, 0, (1 << HwPortID), HwPortID);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_UC_CONTROL, reg_val);
            // disable learning in MC_Control for DP1, DP1S
            reg_val = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_MC_CONTROL);
            EDDP_SET_BIT_VALUE32(reg_val, 0, (1 << HwPortID), HwPortID);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MC_CONTROL, reg_val);

            EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Set_Port_State_HW(HwPortID=%d): disable 'Learning' in REG_UC_CONTROL and REG_MC_CONTROL!", 
                HwPortID);
        }
    }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_IsFdbCmdRunning                    +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                                                                         +*/
/*+  Result                :    LSA_BOOL                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function checks whether one FDB command is running   +*/
/*+               currently.                                                +*/
/*+                                                                         +*/
/*+               Return LSA_TRUE  : one FDB command is running             +*/
/*+                      LSA_FALSE : no FDB command is running              +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_SWI_IsFdbCmdRunning(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_BOOL        State = LSA_FALSE;

    if (pDDB->SWI.CommandParams.ClearUC_Table.param_union.Fdb.ClearUCTable.isCmdRunning)
    {
        State = LSA_TRUE;
    }

    return (State);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWIOpenChannel                         +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB           +*/
/*+                        :    EDDP_LOCAL_HDB_PTR_TYPE      pHDB           +*/
/*+                             LSA_HANDLE_TYPE              Handle         +*/
/*+                             EDDP_SYSTEM_CDB_PTR_TYPE     pCDB           +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
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
/*+                                                                         +*/
/*+  Description: Opens Channel for SWI. The Handle will specify the Channel+*/
/*+               and pDDB the Device. pCDB has all parameters              +*/
/*+               for this channel.                                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWIOpenChannel(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB,
    LSA_HANDLE_TYPE             Handle,
    EDDP_SYSTEM_CDB_PTR_TYPE    pCDB)
{
    EDD_RSP                         Response;
    EDDP_LOCAL_HDB_SWI_PTR_TYPE     pHDB_SWI_PoolItem;

    LSA_UNUSED_ARG(pDDB);
    LSA_UNUSED_ARG(Handle);
#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pCDB);
#endif

    EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_SWIOpenChannel(pCDB=0x%X)", pHDB->Handle, pCDB);

    Response = EDD_STS_OK;

#ifdef EDDP_CFG_LOCAL_MEM_STATIC
    pHDB_SWI_PoolItem = &g_EDDP_HDB_SWI[pHDB->Handle];                          // use static RAM
#else
    pHDB_SWI_PoolItem = EDDP_GlbAllocLocalMem (sizeof(EDDP_HDB_SWI_TYPE));      // alloc Mgmt-data in Heap

    // check Alloc successful ?
    if (LSA_HOST_PTR_ARE_EQUAL(pHDB_SWI_PoolItem, LSA_NULL))
    {
        Response = EDD_STS_ERR_RESOURCE;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_SWIOpenChannel(): ERROR -> AllocLocalMem of pHDB->pSWI(0x%X) is not Ok! Response=0x%X", 
            pHDB->Handle, pHDB_SWI_PoolItem, Response);
    }
#endif
    pHDB->pSWI = pHDB_SWI_PoolItem;

    if (Response == EDD_STS_OK)
    {
        // init RunningRQBCount of this new channel: it will be checked at SWICloseChannel
        pHDB->pSWI->RunningRQBCount = 0;
    }

    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_SWIOpenChannel(): Response=0x%X", Response);

    return (Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWICloseChannel                        +*/
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
/*+             : EDD_STS_ERR_SEQUENCE                                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Close Channel for SWI.                                    +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWICloseChannel(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB,
    LSA_HANDLE_TYPE             Handle)
{
    EDD_RSP         Response;

    LSA_UNUSED_ARG(pDDB);
    LSA_UNUSED_ARG(Handle);

    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_SWICloseChannel()", pHDB->Handle);

    Response = EDD_STS_OK;

    /* ------------------------------------------------------------ */
    /* check if there are some RQBs in processing of this channel   */
    /* ------------------------------------------------------------ */
    if (pHDB->pSWI->RunningRQBCount > 0)
    {
        Response = EDD_STS_ERR_SEQUENCE;
        EDDP_DBG_SET_DETAIL_ERR(pHDB->pDDB, Response);
        EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_SWICloseChannel(): ERROR -> There are still some RQBs in processing of this channel! SWI-RunningRQBCount=%d, Response=0x%X", 
            pHDB->Handle, pHDB->pSWI->RunningRQBCount, Response);
    }

    /* ------------------------------------------------------------ */
    /* now free local memory of SWI management of this channel      */
    /* ------------------------------------------------------------ */
    if (Response == EDD_STS_OK)
    {
#ifndef EDDP_CFG_LOCAL_MEM_STATIC
        if (LSA_HOST_PTR_ARE_EQUAL(pHDB->pSWI, LSA_NULL))
        {
            // the pointer is already deleted !
        }
        else
        {
            EDDP_GlbFreeLocalMem (pHDB->pSWI);
        }
#endif
        pHDB->pSWI = LSA_NULL;
    }

    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_SWICloseChannel(): Response=0x%X", Response);

    return (Response);
}

#if 0
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_Set_HOL_Barriers                   +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+             : EDD_STS_ERR_PARAM                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This functions sets all HOL barriers according to the     +*/
/*+               number of interfaces and used hardware.                   +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWI_Set_HOL_Barriers(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP             Status;
    LSA_UINT32          PortCntDevice, PortCntIF;
    LSA_UINT32          HOL_Barriers_Count, HOL_Barriers_Count_Max;

    Status                  = EDD_STS_OK;
    HOL_Barriers_Count      = 0;
    HOL_Barriers_Count_Max  = 0;
    PortCntDevice           = 0;

    /* ------------------------------------------------------------ */
    /* first init all HOL-Barrier/Control registers                 */
    /* ------------------------------------------------------------ */
    switch (EDDP_HALGetPnipRevision_Ext(pDDB))
    {
        /* -------------------------------- */
        /* ERTEC200P (Rev1)                 */
        /* -------------------------------- */
        #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
        case EDDP_HAL_HW_PNIP_R1A1:
        {
            HOL_Barriers_Count_Max  = EDDP_SW_QDB_MAX_ERTEC200P_REV1;
            PortCntIF               = pDDB->SWI.PortCnt;

            if (PortCntIF > 1)
            {
                PortCntDevice             = 2;
            }
            else
            {
                PortCntDevice             = 1;
            }

            // set up HOL_Control registers
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_CONTROL_PA,  EDDP_SWI_REG_HOL_CONTROL_PA_INIT);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_CONTROL_IFA, EDDP_SWI_REG_HOL_CONTROL_IFA_INIT);

            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_0, EDDP_SWI_QDB_BARRIER_P_RES_0_ERTEC200P_REV1 / PortCntDevice * PortCntIF);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_1, EDDP_SWI_QDB_BARRIER_P_RES_1_ERTEC200P_REV1 / PortCntDevice);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_2, EDDP_SWI_QDB_BARRIER_P_RES_2_ERTEC200P_REV1 / PortCntDevice);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_3, EDDP_SWI_QDB_BARRIER_P_RES_3_ERTEC200P_REV1 / PortCntDevice);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_4, EDDP_SWI_QDB_BARRIER_P_RES_4_ERTEC200P_REV1 / PortCntDevice);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_5, EDDP_SWI_QDB_BARRIER_P_RES_5_ERTEC200P_REV1 / PortCntDevice);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_6, EDDP_SWI_QDB_BARRIER_P_RES_6_ERTEC200P_REV1 / PortCntDevice);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_7,     0x00000000);  // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_8,  0x00000000);  // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_9,  0x00000000);  // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_10, 0x00000000);  // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_11, 0x00000000);  // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_12, 0x00000000);  // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_13, 0x00000000);  // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_14, 0x00000000);  // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_15, 0x00000000);  // this resource type is not used

            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_0, EDDP_SWI_QDB_BARRIER_IF_RES_0_ERTEC200P_REV1);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_1, EDDP_SWI_QDB_BARRIER_IF_RES_1_ERTEC200P_REV1);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_2, EDDP_SWI_QDB_BARRIER_IF_RES_2_ERTEC200P_REV1);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_3, EDDP_SWI_QDB_BARRIER_IF_RES_3_ERTEC200P_REV1);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_4, EDDP_SWI_QDB_BARRIER_IF_RES_4_ERTEC200P_REV1);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_5, EDDP_SWI_QDB_BARRIER_IF_RES_5_ERTEC200P_REV1);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_6, EDDP_SWI_QDB_BARRIER_IF_RES_6_ERTEC200P_REV1);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_7, EDDP_SWI_QDB_BARRIER_IF_RES_7_ERTEC200P_REV1);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_8,  0x00000000); // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_9,  0x00000000); // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_10, 0x00000000); // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_11, 0x00000000); // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_12, 0x00000000); // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_13, 0x00000000); // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_14, 0x00000000); // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_15, 0x00000000); // this resource type is not used
        }
        break;
        #endif

        /* -------------------------------- */
        /* ERTEC200P-FPGA (Rev1)            */
        /* -------------------------------- */
        #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
        case EDDP_HAL_HW_PNIP_R1F1:
        {
            HOL_Barriers_Count_Max  = EDDP_SW_QDB_MAX_ERTEC200P_REV1;
            PortCntIF               = pDDB->SWI.PortCnt;

            if (PortCntIF > 1)
            {
                PortCntDevice             = 2;
            }
            else
            {
                PortCntDevice             = 1;
            }

            // set up HOL_Control registers (define which ResourceType is allowed to take resources from the global resource pool)
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_CONTROL_PA,  EDDP_SWI_REG_HOL_CONTROL_PA_INIT);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_CONTROL_IFA, EDDP_SWI_REG_HOL_CONTROL_IFA_INIT);

            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_0, EDDP_SWI_QDB_BARRIER_P_RES_0_ERTEC200P_REV1 / PortCntDevice * PortCntIF);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_1, EDDP_SWI_QDB_BARRIER_P_RES_1_ERTEC200P_REV1 / PortCntDevice);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_2, EDDP_SWI_QDB_BARRIER_P_RES_2_ERTEC200P_REV1 / PortCntDevice);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_3, EDDP_SWI_QDB_BARRIER_P_RES_3_ERTEC200P_REV1 / PortCntDevice);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_4, EDDP_SWI_QDB_BARRIER_P_RES_4_ERTEC200P_REV1 / PortCntDevice);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_5, EDDP_SWI_QDB_BARRIER_P_RES_5_ERTEC200P_REV1 / PortCntDevice);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_6, EDDP_SWI_QDB_BARRIER_P_RES_6_ERTEC200P_REV1 / PortCntDevice);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_7, 0x00000000);  // this resource type is not used

            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_0, EDDP_SWI_QDB_BARRIER_IF_RES_0_ERTEC200P_REV1);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_1, EDDP_SWI_QDB_BARRIER_IF_RES_1_ERTEC200P_REV1);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_2, EDDP_SWI_QDB_BARRIER_IF_RES_2_ERTEC200P_REV1);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_3, EDDP_SWI_QDB_BARRIER_IF_RES_3_ERTEC200P_REV1);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_4, EDDP_SWI_QDB_BARRIER_IF_RES_4_ERTEC200P_REV1);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_5, EDDP_SWI_QDB_BARRIER_IF_RES_5_ERTEC200P_REV1);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_6, EDDP_SWI_QDB_BARRIER_IF_RES_6_ERTEC200P_REV1);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_7, EDDP_SWI_QDB_BARRIER_IF_RES_7_ERTEC200P_REV1);
        }
        break;
        #endif

        /* -------------------------------- */
        /* ERTEC200P (Rev2)                 */
        /* -------------------------------- */
        #ifdef EDDP_CFG_HW_PNIP_REV2_SUPPORT
        case EDDP_HAL_HW_PNIP_R2A1:
        {
            HOL_Barriers_Count_Max  = EDDP_SW_QDB_MAX_ERTEC200P_REV2;
            PortCntIF               = pDDB->SWI.PortCnt;

            if (PortCntIF > 1)
            {
                PortCntDevice             = 2;
            }
            else
            {
                PortCntDevice             = 1;
            }

            // set up HOL_Control registers
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_CONTROL_PA,  EDDP_SWI_REG_HOL_CONTROL_PA_INIT);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_CONTROL_IFA, EDDP_SWI_REG_HOL_CONTROL_IFA_INIT);

            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_0, EDDP_SWI_QDB_BARRIER_P_RES_0_ERTEC200P_REV2 / PortCntDevice * PortCntIF);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_1, EDDP_SWI_QDB_BARRIER_P_RES_1_ERTEC200P_REV2 / PortCntDevice);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_2, EDDP_SWI_QDB_BARRIER_P_RES_2_ERTEC200P_REV2 / PortCntDevice);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_3, EDDP_SWI_QDB_BARRIER_P_RES_3_ERTEC200P_REV2 / PortCntDevice);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_4, EDDP_SWI_QDB_BARRIER_P_RES_4_ERTEC200P_REV2 / PortCntDevice);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_5, EDDP_SWI_QDB_BARRIER_P_RES_5_ERTEC200P_REV2 / PortCntDevice);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_6, EDDP_SWI_QDB_BARRIER_P_RES_6_ERTEC200P_REV2 / PortCntDevice);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_7,     0x00000000);  // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_8,  0x00000000);  // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_9,  0x00000000);  // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_10, 0x00000000);  // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_11, 0x00000000);  // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_12, 0x00000000);  // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_13, 0x00000000);  // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_14, 0x00000000);  // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_15, 0x00000000);  // this resource type is not used

            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_0, EDDP_SWI_QDB_BARRIER_IF_RES_0_ERTEC200P_REV2);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_1, EDDP_SWI_QDB_BARRIER_IF_RES_1_ERTEC200P_REV2);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_2, EDDP_SWI_QDB_BARRIER_IF_RES_2_ERTEC200P_REV2);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_3, EDDP_SWI_QDB_BARRIER_IF_RES_3_ERTEC200P_REV2);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_4, EDDP_SWI_QDB_BARRIER_IF_RES_4_ERTEC200P_REV2);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_5, EDDP_SWI_QDB_BARRIER_IF_RES_5_ERTEC200P_REV2);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_6, EDDP_SWI_QDB_BARRIER_IF_RES_6_ERTEC200P_REV2);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_7, EDDP_SWI_QDB_BARRIER_IF_RES_7_ERTEC200P_REV2);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_8,  0x00000000); // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_9,  0x00000000); // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_10, 0x00000000); // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_11, 0x00000000); // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_12, 0x00000000); // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_13, 0x00000000); // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_14, 0x00000000); // this resource type is not used
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_15, 0x00000000); // this resource type is not used
        }
        break;
        #endif

        /* -------------------------------- */
        /* ERTEC200P-FPGA (Rev2)            */
        /* -------------------------------- */
        #ifdef EDDP_CFG_HW_PNIP_REV2_SUPPORT
        case EDDP_HAL_HW_PNIP_R2F1:
        {
            HOL_Barriers_Count_Max  = EDDP_SW_QDB_MAX_ERTEC200P_REV2;
            PortCntIF               = pDDB->SWI.PortCnt;

            if (PortCntIF > 1)
            {
                PortCntDevice             = 2;
            }
            else
            {
                PortCntDevice             = 1;
            }

            // set up HOL_Control registers (define which ResourceType is allowed to take resources from the global resource pool)
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_CONTROL_PA,  EDDP_SWI_REG_HOL_CONTROL_PA_INIT);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_CONTROL_IFA, EDDP_SWI_REG_HOL_CONTROL_IFA_INIT);

            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_0, EDDP_SWI_QDB_BARRIER_P_RES_0_ERTEC200P_REV2 / PortCntDevice * PortCntIF);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_1, EDDP_SWI_QDB_BARRIER_P_RES_1_ERTEC200P_REV2 / PortCntDevice);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_2, EDDP_SWI_QDB_BARRIER_P_RES_2_ERTEC200P_REV2 / PortCntDevice);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_3, EDDP_SWI_QDB_BARRIER_P_RES_3_ERTEC200P_REV2 / PortCntDevice);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_4, EDDP_SWI_QDB_BARRIER_P_RES_4_ERTEC200P_REV2 / PortCntDevice);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_5, EDDP_SWI_QDB_BARRIER_P_RES_5_ERTEC200P_REV2 / PortCntDevice);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_6, EDDP_SWI_QDB_BARRIER_P_RES_6_ERTEC200P_REV2 / PortCntDevice);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_PA_7, 0x00000000);  // this resource type is not used

            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_0, EDDP_SWI_QDB_BARRIER_IF_RES_0_ERTEC200P_REV2);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_1, EDDP_SWI_QDB_BARRIER_IF_RES_1_ERTEC200P_REV2);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_2, EDDP_SWI_QDB_BARRIER_IF_RES_2_ERTEC200P_REV2);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_3, EDDP_SWI_QDB_BARRIER_IF_RES_3_ERTEC200P_REV2);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_4, EDDP_SWI_QDB_BARRIER_IF_RES_4_ERTEC200P_REV2);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_5, EDDP_SWI_QDB_BARRIER_IF_RES_5_ERTEC200P_REV2);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_6, EDDP_SWI_QDB_BARRIER_IF_RES_6_ERTEC200P_REV2);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_HOL_BARRIER_IFA_7, EDDP_SWI_QDB_BARRIER_IF_RES_7_ERTEC200P_REV2);
        }
        break;
        #endif
        
       
        default:
        {
            EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_Set_HOL_Barriers(): FATAL -> PnipRevision(%d) is unknown", 
                EDDP_HALGetPnipRevision_Ext(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }

    /* ------------------------------------------------------------ */
    /* check whether the sum of all HOL barriers is not greater     */
    /* than the total number of available QCW resources             */
    /* ------------------------------------------------------------ */

    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Set_HOL_Barriers(): PortCntDevice(%d)", 
                      PortCntDevice);      

    if( (EDDP_HAL_HW_PNIP_REV3 == EDDP_HALGetPnipRevision(pDDB)) && (EDDP_HAL_HW_IF_B == EDDP_HALGetChipInterface(pDDB)) )
    {
        //HERA with interface B
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_0);
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_1) * PortCntDevice;
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_2) * PortCntDevice;
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_3) * PortCntDevice;
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_4) * PortCntDevice;
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_5) * PortCntDevice;
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_6) * PortCntDevice;
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_7) * PortCntDevice;
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_8) * PortCntDevice;
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_9) * PortCntDevice;
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_10)* PortCntDevice;
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_11)* PortCntDevice;
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_12)* PortCntDevice;
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_13)* PortCntDevice;
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_14)* PortCntDevice;
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_15)* PortCntDevice;

        EDDP_SWI_TRACE_08(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_SWI_Set_HOL_Barriers(): Values PNIP_REG_HOL_BARRIER PA0(%d), PA1(%d), PA2(%d), PA3(%d), PA4(%d), PA5(%d), PA6(%d), PA7(%d), ", 
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_0),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_1),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_2),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_3),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_4),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_5),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_6),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_7));

        EDDP_SWI_TRACE_08(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_SWI_Set_HOL_Barriers(): Values PNIP_A0_REG_HOL_BARRIER PA8(%d), PA9(%d), PA10(%d), PA11(%d), PA12(%d), PA13(%d), PA14(%d), PA15(%d), ", 
                            EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_8),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_9),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_10),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_11),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_12),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_13),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_14),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_15));

        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_0);
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_1);
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_2);
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_3);
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_4);
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_5);
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_6);
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_7);
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_8);
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_9);
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_10);
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_11);
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_12);
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_13);
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_14);
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_15);

        EDDP_SWI_TRACE_08(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_SWI_Set_HOL_Barriers(): Values PNIP_REG_HOL_BARRIER IFA0(%d), IFA1(%d), IFA2(%d), IFA3(%d), IFA4(%d), IFA5(%d), IFA6(%d), IFA7(%d), ", 
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_0),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_1),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_2),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_3),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_4),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_5),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_6),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_7));

        EDDP_SWI_TRACE_08(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_SWI_Set_HOL_Barriers(): Values PNIP_A0_REG_HOL_BARRIER IF8(%d), IFA9(%d), IFA10(%d), IFA11(%d), IFA12(%d), IFA13(%d), IFA14(%d), IFA15(%d), ", 
                            EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_8),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_9),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_10),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_11),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_12),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_13),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_14),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_15));
        
    }
    else
    {
        // Interface A
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_0);
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_1) * PortCntDevice;
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_2) * PortCntDevice;
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_3) * PortCntDevice;
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_4) * PortCntDevice;
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_5) * PortCntDevice;
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_6) * PortCntDevice;
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_7) * PortCntDevice;

        EDDP_SWI_TRACE_08(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_SWI_Set_HOL_Barriers(): Values PNIP_REG_HOL_BARRIER PA0(%d), PA1(%d), PA2(%d), PA3(%d), PA4(%d), PA5(%d), PA6(%d), PA7(%d), ", 
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_0),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_1),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_2),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_3),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_4),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_5),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_6),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_PA_7));
        
        if (EDDP_HAL_HW_TYPE_ASIC == EDDP_HALGetChipType(pDDB) || EDDP_HAL_HW_HERA == EDDP_HALGetChipVersion(pDDB))
        {
            // these registers are existing with ASIC only for ERTEC200P
            HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_8) * PortCntDevice;
            HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_9) * PortCntDevice;
            HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_10)* PortCntDevice;
            HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_11)* PortCntDevice;
            HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_12)* PortCntDevice;
            HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_13)* PortCntDevice;
            HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_14)* PortCntDevice;
            HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_15)* PortCntDevice;

            EDDP_SWI_TRACE_08(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_SWI_Set_HOL_Barriers(): Values PNIP_A0_REG_HOL_BARRIER PA8(%d), PA9(%d), PA10(%d), PA11(%d), PA12(%d), PA13(%d), PA14(%d), PA15(%d), ", 
                                EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_8),
                                EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_9),
                                EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_10),
                                EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_11),
                                EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_12),
                                EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_13),
                                EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_14),
                                EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_PA_15));
        }

        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_0);
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_1);
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_2);
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_3);
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_4);
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_5);
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_6);
        HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_7);

        EDDP_SWI_TRACE_08(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_SWI_Set_HOL_Barriers(): Values PNIP_REG_HOL_BARRIER IFA0(%d), IFA1(%d), IFA2(%d), IFA3(%d), IFA4(%d), IFA5(%d), IFA6(%d), IFA7(%d), ", 
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_0),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_1),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_2),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_3),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_4),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_5),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_6),
                            EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_BARRIER_IFA_7));

        if (EDDP_HAL_HW_TYPE_ASIC == EDDP_HALGetChipType(pDDB) || EDDP_HAL_HW_HERA == EDDP_HALGetChipVersion(pDDB))
        {
            // these registers are existing with ASIC only for ERTEC200P
            HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_8);
            HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_9);
            HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_10);
            HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_11);
            HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_12);
            HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_13);
            HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_14);
            HOL_Barriers_Count += EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_15);

            EDDP_SWI_TRACE_08(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_SWI_Set_HOL_Barriers(): Values PNIP_A0_REG_HOL_BARRIER IFA8(%d), IFA9(%d), IFA10(%d), IFA11(%d), IFA12(%d), IFA13(%d), IFA14(%d), IFA15(%d), ", 
                                EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_8),
                                EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_9),
                                EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_10),
                                EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_11),
                                EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_12),
                                EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_13),
                                EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_14),
                                EDDP_HAL_REG32_READ(pDDB, PNIP_A0_REG_HOL_BARRIER_IFA_15));
        }
    }

    if (HOL_Barriers_Count > HOL_Barriers_Count_Max)
    {
        EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_Set_HOL_Barriers(): FATAL -> HOL_Barriers_Count(%d) > HOL_Barriers_Count_Max(%d)! Wrong implementation!", 
            HOL_Barriers_Count, HOL_Barriers_Count_Max);
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }

    EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_SWI_Set_HOL_Barriers(): HOL_Barriers_Count(%d), HOL_Barriers_Count_Max(%d)", 
        HOL_Barriers_Count, HOL_Barriers_Count_Max);

    return (Status);
}
#endif
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_Change_ComCycle                    +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters          (in) +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+             : EDD_STS_ERR_SEQUENCE                                      +*/
/*+             : EDD_STS_ERR_PARAM                                         +*/
/*+             : EDD_STS_OK_PENDING                                        +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function changes the COM-Cycle (send clock).         +*/
/*+                                                                         +*/
/*+               Call the SendClockChange state machine.                   +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWI_Change_ComCycle(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP                                 Response;
    EDD_UPPER_SENDCLOCK_CHANGE_PTR_TYPE     pRqbParam;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pHDB);
#endif

    Response    = EDD_STS_OK;
    pRqbParam   = (EDD_UPPER_SENDCLOCK_CHANGE_PTR_TYPE) pRQB->pParam;

    EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_SWI_Change_ComCycle(pRQB=0x%X)", 
        pHDB->Handle, pRQB);

    EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_SWI_Change_ComCycle(): current SWI.CycleBaseFactor=%d, new RQB.CycleBaseFactor=%d", 
        pHDB->Handle, pDDB->SWI.CycleInfo.CycleBaseFactor, pRqbParam->CycleBaseFactor);

    /* send clock change service can be run only at once */
    if(!LSA_HOST_PTR_ARE_EQUAL(pDDB->SWI.SendClockChangeSm.pRQB, LSA_NULL))
    {
        EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_Change_ComCycle(): FATAL -> changing SCF already in progress!");
        EDDP_FATAL(EDDP_FATAL_ERR_SERVICE);
    }
    
    /* ------------------------------------ */
    /* check parameter                      */
    /* ------------------------------------ */
    // check whether new parameter "CycleBaseFactor" has changed
    if (pRqbParam->CycleBaseFactor == pDDB->SWI.CycleInfo.CycleBaseFactor)
    {
        EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] EDDP_SWI_Change_ComCycle(): not executing the command - CycleBaseFactor has not changed");
    }
    else
    {
        // check if there are still ACTIVE providers or consumers in CRT
        if ( EDDP_CRTCheckIsActiveProviderConsumerPresent(pDDB) )
        {
            Response = EDD_STS_ERR_SEQUENCE;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_Change_ComCycle(): ERROR -> There are still some ACTIVE Providers or Consumers in CRT!");
        }
        else // no ACTIVE providers or consumers in CRT
        {
            // check valid range of CycleBaseFactor
            if ((pRqbParam->CycleBaseFactor > EDDP_CRT_CYCLE_BASE_FACTOR_MAX)
#if defined(EDDP_APPLY_IRTPU_RESTRICTIONS)
                || ((pRqbParam->CycleBaseFactor < EDDP_CRT_CYCLE_BASE_FACTOR_8)
                    && (EDDP_CRT_CYCLE_BASE_FACTOR_4 != pRqbParam->CycleBaseFactor)
                    && (EDDP_CRT_CYCLE_BASE_FACTOR_6 != pRqbParam->CycleBaseFactor) 
                    && (0 != pRqbParam->CycleBaseFactor)
                   )
#endif 
               )
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
#if defined(EDDP_APPLY_IRTPU_RESTRICTIONS)
                EDDP_SWI_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_Change_ComCycle(): ERROR -> CycleBaseFactor(%d) is out of range (%d;%d;%d..%d)!", 
                    pRqbParam->CycleBaseFactor ,EDDP_CRT_CYCLE_BASE_FACTOR_4, EDDP_CRT_CYCLE_BASE_FACTOR_6, EDDP_CRT_CYCLE_BASE_FACTOR_8, EDDP_CRT_CYCLE_BASE_FACTOR_MAX);
#else
                EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_Change_ComCycle(): ERROR -> CycleBaseFactor(%d) is out of range (0..%d)!", 
                    pRqbParam->CycleBaseFactor, EDDP_CRT_CYCLE_BASE_FACTOR_MAX);
#endif // EDDP_APPLY_IRTPU_RESTRICTIONS
            }
            else // all is good and CycleBaseFactor will be changed
            {
                /* ------------------------------------ */
                /* execute this service                 */
                /* ------------------------------------ */
                
                /* save SCF change RQB within state machine for later usage */
                pDDB->SWI.SendClockChangeSm.pRQB = pRQB;
                
                // start the SendClockChange state machine
                EDDP_ASSERT_FALSE(pDDB->SWI.SendClockChangeSm.State != EDDP_SWI_SEND_CLOCK_CHANGE_STATE_IDLE);
                pDDB->SWI.SendClockChangeSm.State = EDDP_SWI_SEND_CLOCK_CHANGE_STATE_START;
                EDDP_SWISendClockChangeStateMachine (pDDB, EDD_STS_OK);
                // set Response for RQBHandler
                Response = EDD_STS_OK_PENDING;
            }
        }
    }

    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_SWI_Change_ComCycle(): Response=0x%X", Response);

    return (Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_ISR_MacErr                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32					   EventNr      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  EventNr    : Event Number (Interrupt Number)                     (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called by CORE from the                  +*/
/*+               interrupt "INT_MAC_ERR".                                  +*/
/*+               It is executed within LOW context.                        +*/
/*+                                                                         +*/
/*+               Register "PNIP_REG_IRQxEVENT_LOW_0"                       +*/
/*+               Interrupt event "INT_MAC_ERR"                             +*/
/*+                 -> Bit(13): 0x00002000                                  +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_ISR_MacErr(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr)
{
#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(EventNr);
#endif

    switch (pDDB->Hardware)
    {
#ifdef EDDP_CFG_HW_ERTEC200P_SUPPORT
        case EDDP_HW_ERTEC200P:
        case EDDP_HW_ERTEC200P_FPGA:
        {
            LSA_UINT32          RegMacErrorStatusP1, RegMacErrorStatusP2;
            __attribute__((unused))LSA_UINT32          RegMacStatusP1, RegMacStatusP2;
            
            RegMacErrorStatusP1 = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_MACERRORSTATUS_P1);
            RegMacErrorStatusP2 = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_MACERRORSTATUS_P2);
            RegMacStatusP1      = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_MACSTATUS_P1);
            RegMacStatusP2      = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_MACSTATUS_P2);

            // Note: 0x800 Bit(11) "MACTClockErr" in REG_MACERRORSTATUS_Px is no error (normal situation) if LinkDown
            // so trace it with NOTE_HIGH
            EDDP_SWI_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] EDDP_SWI_ISR_MacErr(EventNr=0x%X): RegMacErrorStatusP1=0x%X, RegMacErrorStatusP2=0x%X, RegMacStatusP1=0x%X, RegMacStatusP2=0x%X", 
                EventNr, RegMacErrorStatusP1, RegMacErrorStatusP2, RegMacStatusP1, RegMacStatusP2);

            #ifdef EDDP_SWI_ISR_MACERR_MACTBUFFEROVERFLOWERR_FATAL_ON
            if (   (RegMacErrorStatusP1 & EDDP_SWI_REG_MACERRORSTATUS_MACTBUFFEROVERFLOWERR_MASK) 
                || (RegMacErrorStatusP2 & EDDP_SWI_REG_MACERRORSTATUS_MACTBUFFEROVERFLOWERR_MASK)
               )
            {
                // this error is fatal: MACTBufferOverflowErr
                EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_ISR_MacErr(): FATAL -> RegMacErrorStatus(MACTBufferOverflowErr)!");
                EDDP_FATAL(EDDP_FATAL_ERR_UNEXP_INTERRUPT);
            }
            #endif

            #ifdef EDDP_SWI_ISR_MACERR_MACTBUFFERUNDERFLOWERR_FATAL_ON
            if (   (RegMacErrorStatusP1 & EDDP_SWI_REG_MACERRORSTATUS_MACTBUFFERUNDERFLOWERR_MASK) 
                || (RegMacErrorStatusP2 & EDDP_SWI_REG_MACERRORSTATUS_MACTBUFFERUNDERFLOWERR_MASK)
               )
            {
                // this error is fatal: MACTBufferUnderflowErr
                EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_ISR_MacErr(): FATAL -> RegMacErrorStatus(MACTBufferUnderflowErr)!");
                EDDP_FATAL(EDDP_FATAL_ERR_UNEXP_INTERRUPT);
            }
            #endif
        }
        break;
#endif
        default:
        {
            EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_ISR_MacErr(): FATAL -> Hardware(0x%X) is not supported!", 
                pDDB->Hardware);
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_ISR_FrameAging_Expired             +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32					   EventNr      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  EventNr    : Event Number (Interrupt Number)                     (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE after a 		        +*/
/*+				  FrameAging_Expired interrupt occurred. It is executed     +*/
/*+               within LOW context and makes a trace entry.               +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_ISR_FrameAging_Expired(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr)
{
#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(EventNr);
#endif

    switch (pDDB->Hardware)
    {
#ifdef EDDP_CFG_HW_ERTEC200P_SUPPORT
        case EDDP_HW_ERTEC200P:
        case EDDP_HW_ERTEC200P_FPGA:
        {
            __attribute__((unused))LSA_UINT32          RegFrameAgingStatusP1;
            __attribute__((unused))LSA_UINT32          RegFrameAgingStatusP2;
            
            RegFrameAgingStatusP1 = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_FRAMEAGING_STATUS_P1);
            RegFrameAgingStatusP2 = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_FRAMEAGING_STATUS_P2);

            EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H:--] EDDP_SWI_ISR_FrameAging_Expired(EventNr=0x%X): RegFrameAgingStatusP1=0x%X, RegFrameAgingStatusP2=0x%X", 
                EventNr, RegFrameAgingStatusP1, RegFrameAgingStatusP2);

            #ifdef EDDP_SWI_ISR_FRAMEAGING_EXPIRED_QUEUEPRIO8_RTC3_FATAL_ON
            if (   (RegFrameAgingStatusP1 & EDDP_SWI_REG_FRAMEAGING_STATUS_QUEUEPRIO8_RTC3_MASK) 
                || (RegFrameAgingStatusP2 & EDDP_SWI_REG_FRAMEAGING_STATUS_QUEUEPRIO8_RTC3_MASK)
               )
            {
                // this error is fatal: QueuePrio8-RTC3
                EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_ISR_FrameAging_Expired(): FATAL -> RegFrameAgingStatus(QueuePrio8-RTC3)!");
                EDDP_FATAL(EDDP_FATAL_ERR_UNEXP_INTERRUPT);
            }
            #endif

            #ifdef EDDP_SWI_ISR_FRAMEAGING_EXPIRED_QUEUEPRIO15_RTC3_FATAL_ON
            if (   (RegFrameAgingStatusP1 & EDDP_SWI_REG_FRAMEAGING_STATUS_QUEUEPRIO15_RTC3_MASK) 
                || (RegFrameAgingStatusP2 & EDDP_SWI_REG_FRAMEAGING_STATUS_QUEUEPRIO15_RTC3_MASK)
               )
            {
                // this error is fatal: QueuePrio15-RTC3
                EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_ISR_FrameAging_Expired(): FATAL -> RegFrameAgingStatus(QueuePrio15-RTC3)!");
                EDDP_FATAL(EDDP_FATAL_ERR_UNEXP_INTERRUPT);
            }
            #endif
        }
        break;
#endif
        default:
        {
            EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_ISR_FrameAging_Expired(): FATAL -> Hardware(0x%X) is not supported!", 
                pDDB->Hardware);
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_ISR_QCW_Empty                      +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32                     EventNr      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  EventNr    : Event Number (Interrupt Number)                     (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function is called by CORE from the                  +*/
/*+               interrupt "INT_QCW_Empty".                                +*/
/*+               It is executed within LOW context.                        +*/
/*+                                                                         +*/
/*+               Register "PNIP_REG_IRQxEVENT_LOW_0"                       +*/
/*+               Interrupt event "INT_QCW_Empty"                           +*/
/*+                 -> Bit(24): 0x01000000                                  +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_ISR_QCW_Empty(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr)
{
    __attribute__((unused))LSA_UINT32      RegHolStatus, RegResLackCounter;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(EventNr);
#endif

    RegHolStatus        = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_STATUS_IFA);
    RegResLackCounter   = 0;

    // RegResLackCounter dependent of PNIP revision
    switch (EDDP_HALGetPnipRevision(pDDB))
    {
        #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
        case EDDP_HAL_HW_PNIP_REV1:
        {
            // there is no PNIP_R2_REG_PPM_RESSOURCE_LACK_COUNTER_IFA in revision 1
        }
        break;
        #endif

        #if defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
        case EDDP_HAL_HW_PNIP_REV2:
        case EDDP_HAL_HW_PNIP_REV3:
        {
            /* PNIP_R2_REG_PPM_RESSOURCE_LACK_COUNTER_IFA was replaced by PNIP_R3_REG_PPM_RESSOURCE_LACK_COUNTER_IFA, there is no R3 type of this define*/
            RegResLackCounter = EDDP_HAL_REG32_READ(pDDB, PNIP_R3_REG_PPM_RESSOURCE_LACK_COUNTER_IFA);
        }
        break;
        #endif
        
        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_ISR_QCW_Empty(): FATAL -> Version(%d) of PNIP is unknown", 
                EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }

    EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H:--] EDDP_SWI_ISR_QCW_Empty(EventNr=0x%X): RegHolStatus=0x%X, RegResLackCounter=0x%X", 
        EventNr, RegHolStatus, RegResLackCounter);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_ISR_HOL_Expired_IFA                +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32                     EventNr      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  EventNr    : Event Number (Interrupt Number)                     (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function is called by CORE from the                  +*/
/*+               interrupt "INT_HOL_Expired_IFA".                          +*/
/*+               It is executed within LOW context.                        +*/
/*+                                                                         +*/
/*+               Register "PNIP_REG_IRQxEVENT_LOW_0"                       +*/
/*+               Interrupt event "INT_HOL_Expired_IFA"                     +*/
/*+                 -> Bit(25): 0x02000000                                  +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_ISR_HOL_Expired_IFA(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr)
{
    __attribute__((unused))LSA_UINT32      RegHolStatus, RegResLackCounter;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(EventNr);
#endif

    RegHolStatus        = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_STATUS_IFA);
    RegResLackCounter   = 0;

    // RegResLackCounter dependent of PNIP revision
    switch (EDDP_HALGetPnipRevision(pDDB))
    {
        #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
        case EDDP_HAL_HW_PNIP_REV1:
        {
            // there is no PNIP_R2_REG_PPM_RESSOURCE_LACK_COUNTER_IFA in revision 1
        }
        break;
        #endif

        #if defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
        case EDDP_HAL_HW_PNIP_REV2:
        case EDDP_HAL_HW_PNIP_REV3:
        {
            /* PNIP_R2_REG_PPM_RESSOURCE_LACK_COUNTER_IFA was replaced by PNIP_R3_REG_PPM_RESSOURCE_LACK_COUNTER_IFA, there is no R3 type of this define*/
            RegResLackCounter = EDDP_HAL_REG32_READ(pDDB, PNIP_R3_REG_PPM_RESSOURCE_LACK_COUNTER_IFA);
        }
        break;
        #endif

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_ISR_HOL_Expired_IFA(): FATAL -> Version(%d) of PNIP is unknown", 
                EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }

    EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] EDDP_SWI_ISR_HOL_Expired_IFA(EventNr=0x%X): RegHolStatus=0x%X, RegResLackCounter=0x%X", 
        EventNr, RegHolStatus, RegResLackCounter);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_ISR_HOL_Expired_PA                 +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32                     EventNr      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  EventNr    : Event Number (Interrupt Number)                     (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function is called by CORE from the                  +*/
/*+               interrupt "INT_HOL_Expired_PA".                           +*/
/*+               It is executed within LOW context.                        +*/
/*+                                                                         +*/
/*+               Register "PNIP_REG_IRQxEVENT_LOW_0"                       +*/
/*+               Interrupt event "INT_HOL_Expired_PA"                      +*/
/*+                 -> Bit(27): 0x08000000                                  +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_ISR_HOL_Expired_PA(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr)
{
    __attribute__((unused))LSA_UINT32      RegHolStatus, RegResLackCounter;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(EventNr);
#endif

    RegHolStatus        = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_STATUS_IFA);
    RegResLackCounter   = 0;

    // RegResLackCounter dependent of PNIP revision
    switch (EDDP_HALGetPnipRevision(pDDB))
    {
        #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
        case EDDP_HAL_HW_PNIP_REV1:
        {
            // there is no PNIP_R2_REG_PPM_RESSOURCE_LACK_COUNTER_IFA in revision 1
        }
        break;
        #endif

        #if defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
        case EDDP_HAL_HW_PNIP_REV2:
        case EDDP_HAL_HW_PNIP_REV3:
        {
            /* PNIP_R2_REG_PPM_RESSOURCE_LACK_COUNTER_IFA was replaced by PNIP_R3_REG_PPM_RESSOURCE_LACK_COUNTER_IFA, there is no R3 type of this define*/
            RegResLackCounter = EDDP_HAL_REG32_READ(pDDB, PNIP_R3_REG_PPM_RESSOURCE_LACK_COUNTER_IFA);
        }
        break;
        #endif
        
        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_ISR_HOL_Expired_PA(): FATAL -> Version(%d) of PNIP is unknown", 
                EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }

    EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] EDDP_SWI_ISR_HOL_Expired_PA(EventNr=0x%X): RegHolStatus=0x%X, RegResLackCounter=0x%X", 
        EventNr, RegHolStatus, RegResLackCounter);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_ISR_FragmentationFault             +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32					   EventNr      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  EventNr    : Event Number (Interrupt Number)                     (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called by CORE from the                  +*/
/*+               interrupt "INT_FragFault".                                +*/
/*+               It is executed within LOW context.                        +*/
/*+                                                                         +*/
/*+               Register "PNIP_REG_IRQxEVENT_LOW_0"                       +*/
/*+               Interrupt event "INT_FragFault"                           +*/
/*+                 -> Bit(19): 0x00080000                                  +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_ISR_FragmentationFault(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr)
{
    __attribute__((unused))LSA_UINT32          RegFragmentationStatus, RegFragmentationDebug, RegCycleLength;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(EventNr);
#endif

    RegFragmentationStatus  = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_FRAGMENTATION_STATUS);
    RegFragmentationDebug   = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_FRAGMENTATION_DEBUG);
    RegCycleLength          = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_CYCLELENGTH_A);

    EDDP_SWI_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H:--] EDDP_SWI_ISR_FragmentationFault(EventNr=0x%X): RegFragmentationStatus=0x%X, RegFragmentationDebug=0x%X, RegCycleLength=%dns", 
        EventNr, RegFragmentationStatus, RegFragmentationDebug, RegCycleLength);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_SWISendClockChangeStateMachine                     +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             EDD_RSP                         ResponseSm              +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ResponseSm     : Response of last action                                           +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: state machine for service SENDCLOCK_CHANGE.                           +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWISendClockChangeStateMachine(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_RSP                     ResponseSm)
{
    EDD_RSP                                     Response;
    EDDP_SWI_SEND_CLOCK_CHANGE_STATE_TYPE       ActState;
    EDDP_SWI_SEND_CLOCK_CHANGE_STATE_TYPE       NextState;
    LSA_BOOL                                    Ready;
    EDDP_SWI_SEND_CLOCK_CHANGE_SM_PTR_TYPE      pStateMachine;
    EDDP_LOCAL_HDB_PTR_TYPE                     pHDB;
    EDDP_GLB_CRT_RQB_HANDLER_PTR_TYPE           pRQBHandler;
    EDD_UPPER_SENDCLOCK_CHANGE_PTR_TYPE         pRqbParam;
    EDD_UPPER_RQB_PTR_TYPE                      pRQB;

    EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_SWISendClockChangeStateMachine(ResponseSm=0x%X): SWI.IRTState=0x%X", 
        ResponseSm, pDDB->SWI.IRTState);

    Response        = EDD_STS_OK;
    pStateMachine   = &pDDB->SWI.SendClockChangeSm;
    ActState        = pStateMachine->State;     // actual state of state machine
    NextState       = ActState;
    Ready           = LSA_FALSE;
    pHDB            = LSA_NULL;
    pRQBHandler     = &pDDB->GlbCrtRQBHandler;
    pRQB            = pStateMachine->pRQB;
    
    if (   (ResponseSm != EDD_STS_OK) 
        || (LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL))
        || (EDD_SRV_SENDCLOCK_CHANGE != EDD_RQB_GET_SERVICE(pRQB))
       )
    {
        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWISendClockChangeStateMachine(): FATAL -> INCONSISTENCE! ResponseSm=0x%X, No RQB in work or wrong service detected!", 
            ResponseSm);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    pRqbParam = (EDD_UPPER_SENDCLOCK_CHANGE_PTR_TYPE) pRQB->pParam;

    /* ------------------------------------------------------------------------ */
    /* finish handling of actual state before go to next state                  */
    /* ------------------------------------------------------------------------ */
    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_SWISendClockChangeStateMachine(): ActState(%d)", ActState);

    switch (ActState)
    {
        /* ------------------------------------------------------------ */
        case EDDP_SWI_SEND_CLOCK_CHANGE_STATE_START:
        /* ------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;

            if (pRqbParam->CycleBaseFactor != EDDP_SWI_CYCLE_BASE_FACTOR_DISABLE_IRT)
            {
                // signal start of SendClockChange to application
                EDDP_SIGNAL_SENDCLOCK_CHANGE (pDDB->hSysDev, pDDB->SWI.CycleInfo.CycleBaseFactor, EDDP_SENDCLOCK_CHANGE_MODE_STARTING);
            }

            NextState = EDDP_SWI_SEND_CLOCK_CHANGE_STATE_PASSIVATE_IRT;
        }
        break;

        /* ------------------------------------------------------------ */
        case EDDP_SWI_SEND_CLOCK_CHANGE_STATE_PASSIVATE_IRT:
        /* ------------------------------------------------------------ */
        {
            // this state is synchron
            EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWISendClockChangeStateMachine(): FATAL -> The State(%d) is synchron!", 
                ActState);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;

        /* ------------------------------------------------------------ */
        case EDDP_SWI_SEND_CLOCK_CHANGE_STATE_PASSIVATE_CRT_PACK:
        /* ------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            NextState               = EDDP_SWI_SEND_CLOCK_CHANGE_STATE_CHANGE_COMCYCLE;
        }
        break;

        /* ------------------------------------------------------------ */
        case EDDP_SWI_SEND_CLOCK_CHANGE_STATE_CHANGE_COMCYCLE:
        /* ------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            NextState               = EDDP_SWI_SEND_CLOCK_CHANGE_STATE_SETUP_TRANSFER_END_OFF;
        }
        break;

        /* ------------------------------------------------------------ */
        case EDDP_SWI_SEND_CLOCK_CHANGE_STATE_SETUP_TRANSFER_END_OFF:
        /* ------------------------------------------------------------ */
        {
            // this state is synchron
            EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWISendClockChangeStateMachine(): FATAL -> The State(%d) is synchron!", 
                ActState);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;

        /* ------------------------------------------------------------ */
        case EDDP_SWI_SEND_CLOCK_CHANGE_STATE_UPDATE_APPL_LENGTH_ON:
        /* ------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            NextState               = EDDP_SWI_SEND_CLOCK_CHANGE_STATE_SETUP_TRANSFER_END_ON;
        }
        break;

        /* ------------------------------------------------------------ */
        case EDDP_SWI_SEND_CLOCK_CHANGE_STATE_SETUP_TRANSFER_END_ON:
        /* ------------------------------------------------------------ */
        {
            // this state is synchron
            EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWISendClockChangeStateMachine(): FATAL -> The State(%d) is synchron!", 
                ActState);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;

        /* ------------------------------------------------------------ */
        case EDDP_SWI_SEND_CLOCK_CHANGE_STATE_FINISH:
        default:
        /* ------------------------------------------------------------ */
        {
            EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWISendClockChangeStateMachine(): FATAL -> Unexpected ActState(%d)", 
                ActState);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }

    /* ------------------------------------------------------------------------ */
    /* forward the state machine to next state                                  */
    /* ------------------------------------------------------------------------ */
    while (!Ready)
    {
        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_SWISendClockChangeStateMachine(): loop -> NextState(%d)", 
            NextState);

        pStateMachine->State = NextState;

        switch (NextState)
        {
            /* ------------------------------------------------------------ */
            case EDDP_SWI_SEND_CLOCK_CHANGE_STATE_PASSIVATE_IRT:
            /* ------------------------------------------------------------ */
            {
                // disable all IRT settings
                EDDP_SWI_PRM_Disable_IRT (pDDB);

                NextState = EDDP_SWI_SEND_CLOCK_CHANGE_STATE_PASSIVATE_CRT_PACK;
            }
            break;

            /* ------------------------------------------------------------ */
            case EDDP_SWI_SEND_CLOCK_CHANGE_STATE_PASSIVATE_CRT_PACK:
            /* ------------------------------------------------------------ */
            {
                // is DFP active in RecordsetA ?
                if ( EDDP_PRMPDIRSubframeDataIsDfpPresent(pDDB, LSA_TRUE) )
                {
                    // start the CRT-Passivate-Pack state machine
                    EDDP_ASSERT_FALSE(pDDB->CRT.PackPassivateSm.State != EDDP_CRT_PACK_PASSIVATE_STATE_IDLE);
                    pDDB->CRT.PackPassivateSm.State = EDDP_CRT_PACK_PASSIVATE_STATE_START;
                    pDDB->CRT.PackPassivateSm.Cbf   = EDDP_SWISendClockChangeStateMachineCbf;
                    EDDP_CRTPassivatePackStateMachine (pDDB, EDD_STS_OK);

                    Ready = LSA_TRUE;
                }
                else
                {
                    NextState = EDDP_SWI_SEND_CLOCK_CHANGE_STATE_CHANGE_COMCYCLE;
                }
            }
            break;

            /* ------------------------------------------------------------ */
            case EDDP_SWI_SEND_CLOCK_CHANGE_STATE_CHANGE_COMCYCLE:
            /* ------------------------------------------------------------ */
            {
                // PDSyncData(A), PDIRData(A) and PDIRSubframeData(A) are no more active
                // Important: these flags may be deleted only after the state "PASSIVATE_CRT_PACK" because they are needed there!
                pDDB->PRM.pRecordsetA->PDSyncData_IsPresent       = LSA_FALSE;
                pDDB->PRM.pRecordsetA->PDIRData_IsPresent         = LSA_FALSE;
                pDDB->PRM.pRecordsetA->PDIRSubframeData_IsPresent = LSA_FALSE;

                if (pRqbParam->CycleBaseFactor == EDDP_SWI_CYCLE_BASE_FACTOR_DISABLE_IRT)
                {
                    // no "Update_COMCYCLE" - go to next state
                    NextState = EDDP_SWI_SEND_CLOCK_CHANGE_STATE_SETUP_TRANSFER_END_OFF;
                }
                else
                {
                    // fill parameter structure for command "Update_COMCYCLE"
                    pDDB->SWI.CommandParams.Update_COMCYCLE.param_union.Gsync.UpdateComCycl.CommunicationLength_1ns = 
                        (pRqbParam->CycleBaseFactor * EDDP_CRT_CYCLE_LENGTH_BASE_NS);
                    pDDB->SWI.CommandParams.Update_COMCYCLE.Cbf           = EDDP_SWI_CommandDoneCbf_GSYNC;
                    pDDB->SWI.CommandParams.Update_COMCYCLE.CmdOpcode     = EDDP_CMD_GSYNC_UPDATE_COMCYCL;
                    // trigger command (runs asynchronously)
                    Response = EDDP_CmdRequest (pDDB, &(pDDB->SWI.CommandParams.Update_COMCYCLE));
                    EDDP_ASSERT_FALSE(Response != EDD_STS_OK_PENDING);

                    Ready = LSA_TRUE;
                }
            }
            break;

            /* ------------------------------------------------------------ */
            case EDDP_SWI_SEND_CLOCK_CHANGE_STATE_SETUP_TRANSFER_END_OFF:
            /* ------------------------------------------------------------ */
            {
                // this state is only for TransferEnd mode
                if (EDDP_APPL_TIMER_CFG_MODE_TRANSFER_END == EDDP_CRTGetApplTimerMode(pDDB))
                {
                    EDDP_PRMSetupTransferEnd (pDDB, EDDP_PRM_TRANSFER_END_CASE_OFF);
                }

                NextState = EDDP_SWI_SEND_CLOCK_CHANGE_STATE_UPDATE_APPL_LENGTH_ON;
            }
            break;

            /* ------------------------------------------------------------ */
            case EDDP_SWI_SEND_CLOCK_CHANGE_STATE_UPDATE_APPL_LENGTH_ON:
            /* ------------------------------------------------------------ */
            {
                // this state is only for TransferEnd mode and only if ChangeComCycle was requested
                if (   (EDDP_APPL_TIMER_CFG_MODE_TRANSFER_END == EDDP_CRTGetApplTimerMode(pDDB)) 
                    && (pRqbParam->CycleBaseFactor != EDDP_SWI_CYCLE_BASE_FACTOR_DISABLE_IRT)
                   )
                {
                    // call CmdRequest for "Update_APPLCYCLE"
                    pDDB->CRT.CmdParam.Update_ApplCycle.param_union.Gsync.UpdateAppCycl.isApplTimerOn   = LSA_TRUE;
                    pDDB->CRT.CmdParam.Update_ApplCycle.param_union.Gsync.UpdateAppCycl.SmCbf           = EDDP_SWISendClockChangeStateMachineCbf;
                    Response = EDDP_CRTCmdRequestApplTimer (pDDB, EDDP_CRT_CMD_APPL_TIMER_UPDATE_APPLCYCLE, EDDP_CRT_CMD_ASYNCHRON);

                    switch (Response)
                    {
                        case EDD_STS_OK_PENDING:
                            Ready = LSA_TRUE;
                        break;

                        case EDD_STS_OK:
                            NextState = EDDP_SWI_SEND_CLOCK_CHANGE_STATE_SETUP_TRANSFER_END_ON;
                        break;

                        default:
                        {
                            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWISendClockChangeStateMachine(): FATAL -> Unexpected Response(0x%X) in state UPDATE_APPL_LENGTH_OFF", 
                                Response);
                            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                        }
                        break;
                    }
                }
                else
                {
                    NextState = EDDP_SWI_SEND_CLOCK_CHANGE_STATE_SETUP_TRANSFER_END_ON;
                }
            }
            break;

            /* ------------------------------------------------------------ */
            case EDDP_SWI_SEND_CLOCK_CHANGE_STATE_SETUP_TRANSFER_END_ON:
            /* ------------------------------------------------------------ */
            {
                // this state is only for TransferEnd mode
                if (EDDP_APPL_TIMER_CFG_MODE_TRANSFER_END == EDDP_CRTGetApplTimerMode(pDDB))
                {
                    EDDP_PRMSetupTransferEnd (pDDB, EDDP_PRM_TRANSFER_END_CASE_NO_PARAMETRIZATION);
                }

                NextState = EDDP_SWI_SEND_CLOCK_CHANGE_STATE_FINISH;
            }
            break;

            /* ------------------------------------------------------------ */
            case EDDP_SWI_SEND_CLOCK_CHANGE_STATE_FINISH:
            /* ------------------------------------------------------------ */
            {
                EDD_UPPER_RQB_PTR_TYPE pRQB_from_queue = EDDP_GlbRQBQueueRemoveFromBegin(&pRQBHandler->RqbInProgressQueue);
                
                if (   (LSA_HOST_PTR_ARE_EQUAL(pRQB_from_queue, LSA_NULL)                         /* valid RQB? */
                    || (!LSA_HOST_PTR_ARE_EQUAL(pRQB_from_queue, pRQB))                           /* sanity check for processed RQB */
                    || (EDD_SRV_SENDCLOCK_CHANGE != EDD_RQB_GET_SERVICE(pRQB_from_queue))         /* correct service? */
                    || (0 != pRQBHandler->RqbInProgressQueue.Count))                              /* exclusive RQB? */
                   )
                {
                    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWISendClockChangeStateMachine(): FATAL -> INCONSISTENCE! RQB wrong or sequence error! ResponseSM=0x%X",
                        ResponseSm);
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                }
                
                if (pRqbParam->CycleBaseFactor != EDDP_SWI_CYCLE_BASE_FACTOR_DISABLE_IRT)
                {
                    // signal finish of SendClockChange to application
                    EDDP_SIGNAL_SENDCLOCK_CHANGE (pDDB->hSysDev, pDDB->SWI.CycleInfo.CycleBaseFactor, EDDP_SENDCLOCK_CHANGE_MODE_FINISHED);
                }
                
                /* ------------------------------------ */
                /* get Handle for this RQB              */
                /* ------------------------------------ */
                Response = EDDP_GlbHandleGetHDB (EDD_RQB_GET_HANDLE_LOWER(pRQB), &pHDB);

                if (Response != EDD_STS_OK)
                {
                    EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "EDDP_SWISendClockChangeStateMachine(): Invalid handle lower in RQB!");
                    EDD_RQB_SET_RESPONSE(pRQB, EDD_STS_ERR_PARAM);
                    EDDP_RQB_ERROR(pRQB);
                }

                /* ------------------------------------ */
                /* finish the SENDCLOCK_CHANGE request  */
                /* ------------------------------------ */
                EDDP_GlbRequestFinish (pHDB, pRQB, EDD_STS_OK);

                // trigger the finish events of this Service
                EDDP_GlbTriggerFinishEventAfterRQBConfirmation (pDDB, pHDB, EDD_SRV_SENDCLOCK_CHANGE);

                /* ------------------------------------ */
                /* finish the state machine             */
                /* ------------------------------------ */
                Ready                   = LSA_TRUE;
                pStateMachine->State    = EDDP_SWI_SEND_CLOCK_CHANGE_STATE_IDLE;
                pStateMachine->pRQB     = LSA_NULL;

                /* NOTE: EDDP_GlbCrtRQBHandler is called within EDDP_CMD_ISR_ReqDone_IFA */
            }
            break;

            /* ------------------------------------------------------------ */
            case EDDP_SWI_SEND_CLOCK_CHANGE_STATE_START:
            default:
            /* ------------------------------------------------------------ */
            {
                EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWISendClockChangeStateMachine(): FATAL -> Unexpected NextState(%d)", 
                    NextState);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }

    EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_SWISendClockChangeStateMachine(): Response=0x%X, SWI.IRTState=0x%X", 
        Response, pDDB->SWI.IRTState);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_SWISendClockChangeStateMachineCbf                  +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             EDD_RSP                         Response                +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  Response       : Response of last action                                           +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Callbackfunction of SendClockChange state machine.                    +*/
/*+               This is called, if asynchron state is finished.                       +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWISendClockChangeStateMachineCbf(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_RSP                     Response)
{
    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_SWISendClockChangeStateMachineCbf(): Response(0x%X)", Response);

    EDDP_SWISendClockChangeStateMachine (pDDB, Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_SWIFillPHYLinkStatus                               +*/
/*+                                                                                     +*/
/*+  Input                 :    pLinkStatus_In     EDDP_CONST_PHY_LINK_STATUS_PTR_TYPE  +*/
/*+  Output                :    pLinkStatus_Out    EDDP_DDB_SWI_LINK_STATUS_PTR_TYPE    +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: fill the pLinkStatus_Out with the pLinkStatus_in values               +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWIFillPHYLinkStatus(
    EDDP_PHY_LINK_STATUS_PTR_CONST_TYPE           pLinkStatus_In,
    EDDP_DDB_SWI_LINK_STATUS_PTR_TYPE             pLinkStatus_Out)
{

    pLinkStatus_Out->LinkSpeed                  = (LSA_UINT8)pLinkStatus_In->Speed;
    pLinkStatus_Out->LinkDuplexity              = (LSA_UINT8)pLinkStatus_In->Duplexity;
    pLinkStatus_Out->MAUType                    = pLinkStatus_In->MAUType;
    pLinkStatus_Out->MediaType                  = pLinkStatus_In->MediaType;
    pLinkStatus_Out->OpticalTransType           = pLinkStatus_In->OpticalTransType;
    pLinkStatus_Out->AutonegCapAdvertised       = pLinkStatus_In->AutonegCapAdvertised;
    pLinkStatus_Out->RealPortTxDelay            = pLinkStatus_In->RealPortTxDelay;
    pLinkStatus_Out->RealPortRxDelay            = pLinkStatus_In->RealPortRxDelay;
    pLinkStatus_Out->MaxPortTxDelay             = pLinkStatus_In->MaxPortTxDelay;
    pLinkStatus_Out->MaxPortRxDelay             = pLinkStatus_In->MaxPortRxDelay;
    pLinkStatus_Out->IsWireless                 = pLinkStatus_In->IsWireless;
    pLinkStatus_Out->IsMDIX                     = pLinkStatus_In->IsMDIX;

}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_SWI_ReadPhyStatusRegs                 +*/
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
/*+  Description: Utility function for EDDP_SWI_PhySTS_StateMachine()       +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#if 0
LSA_VOID EDDP_LOCAL_FCT_ATTR _EDDP_SWI_ReadPhyStatusRegs(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_UINT32			            HWPortID,
    EDDP_PHY_LINK_STATUS_PTR_TYPE   pLinkStatus)
{
    LSA_UINT32  reg_address;
    LSA_UINT32 Link = EDD_LINK_UNKNOWN;
    LSA_UINT32 Speed = EDD_LINK_UNKNOWN;
    LSA_UINT32 Duplexity = EDD_LINK_UNKNOWN;

    EDDP_ASSERT_FALSE((LSA_HOST_PTR_ARE_EQUAL(pLinkStatus, LSA_NULL)));

    EDDP_LL_GET_LINK_STATUS( pDDB->InterfaceID,  HWPortID, &Link, &Speed, &Duplexity);

    pLinkStatus->Link = Link;
    pLinkStatus->Link = Speed;
    pLinkStatus->Link = ;

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
/*+  Functionname          :    EDDP_SWI_GEN_PhySTS_StateMachine_Transition_Start_DEV_SETUP+*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  HwPortIndex: Which port to handle 0...MaxCnt                           +*/
/*+  pResponse:   Returnparameter of this function                          +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Utility function for EDDP_SWI_PhySTS_StateMachine()       +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_GEN_PhySTS_StateMachine_Transition_DoStart_DEV_SETUP(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  HwPortIndex,
    EDD_RSP*                    pResponse)
{
    LSA_UINT32			        HwPortID;
    EDDP_PHY_LINK_STATUS_TYPE   PhyLinkStatus; 

    HwPortID = HwPortIndex+1;
    // finish setup of PHY when it is operational after reset has been released
    // initializes pDDB->SWI.PHY_FunctionPointers[HwPortID]
    EDDP_SWI_FinishPHYSetup (pDDB, HwPortID, LSA_FALSE /*bKeepPhyStatus*/);

  // EDDP_LL:  _EDDP_SWI_ReadPhyStatusRegs (pDDB, HwPortID, &PhyLinkStatus);

    // convert register values into EDD defines (done by system adaption)
    *pResponse = pDDB->SWI.PHY_FunctionPointers[HwPortID].EDDP_PHY_GetLinkStatus(pDDB->hDDB, pDDB->hSysDev, pDDB->Hardware, HwPortID, &PhyLinkStatus);
    EDDP_ASSERT_FALSE(*pResponse != EDD_STS_OK);
    EDDP_ASSERT_FALSE((EDD_PORT_MDIX_DISABLED != PhyLinkStatus.IsMDIX) && (EDD_PORT_MDIX_ENABLED != PhyLinkStatus.IsMDIX));
    EDDP_ASSERT_FALSE((EDD_PORT_IS_NOT_WIRELESS != PhyLinkStatus.IsWireless) && (EDD_PORT_IS_WIRELESS != PhyLinkStatus.IsWireless));
    EDDP_ASSERT_FALSE((EDD_PORT_OPTICALTYPE_ISOTHER != PhyLinkStatus.OpticalTransType) && (EDD_PORT_OPTICALTYPE_ISPOF != PhyLinkStatus.OpticalTransType));

    // actually we are just interested in PortTxDelay and PortRxDelay (we already need those values at the very beginning)

    //LinkSpeedModeConfiguredParam represents a marker for the prm service to indicate the LinkSpeedModeConfiguredParam has been initiliazed.
    pDDB->SWI.LinkStatus[HwPortID].LinkSpeedModeConfiguredParam = EDD_LINK_AUTONEG;

    EDDP_SWIFillPHYLinkStatus(&PhyLinkStatus,&(pDDB->SWI.LinkStatus[HwPortID]));

    EDDP_CORE_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_GEN_ISR_LinkChange(UsrPortID=%d, HwPortID=%d): Finish setup of PHY: PHY-RealPortTxDelay=%d, PHY-RealPortRxDelay=%d, PHY-MaxPortTxDelay=%d, PHY-MaxPortRxDelay=%d",
        HwPortID, HwPortID, PhyLinkStatus.RealPortTxDelay, PhyLinkStatus.RealPortRxDelay, PhyLinkStatus.MaxPortTxDelay, PhyLinkStatus.MaxPortRxDelay);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_GEN_PhySTS_StateMachine_Transition_Start_PLUGGED+*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  HwPortIndex: Which port to handle 0...MaxCnt                           +*/
/*+  pResponse:   Returnparameter of this function                          +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Utility function for EDDP_SWI_PhySTS_StateMachine()       +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_GEN_PhySTS_StateMachine_Transition_DoStart_PLUGGED(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  HwPortIndex,
    EDD_RSP*                    pResponse)
{
    LSA_UINT32			        HwPortID;
    LSA_UINT8                   bAutoneg = EDD_AUTONEG_OFF;

    HwPortID = HwPortIndex+1;
    // finish setup of PHY when it is operational after reset has been released
    // initializes pDDB->SWI.PHY_FunctionPointers[HwPortID]
    EDDP_SWI_FinishPHYSetup (pDDB, HwPortID, LSA_TRUE /*bKeepPhyStatus*/);

    
    if (   (EDD_LINK_UNKNOWN == pDDB->SWI.LinkStatus[HwPortID].LinkSpeed)
        || (EDD_LINK_UNKNOWN == pDDB->SWI.LinkStatus[HwPortID].LinkDuplexity) )
    {
        bAutoneg = EDD_AUTONEG_ON;
    }

    *pResponse = pDDB->SWI.PHY_FunctionPointers[HwPortID].EDDP_PHY_SetSpeedDuplexityOrAutoneg(pDDB->hDDB, 
                                                                                              pDDB->hSysDev, 
                                                                                              HwPortID,
                                                                                              &pDDB->SWI.LinkStatus[HwPortID].LinkSpeedModeConfigured,
                                                                                              pDDB->SWI.LinkStatus[HwPortID].LinkSpeed,                                                                                                
                                                                                              pDDB->SWI.LinkStatus[HwPortID].LinkDuplexity, bAutoneg);

    EDDP_ASSERT_FALSE(*pResponse != EDD_STS_OK);

    if (EDD_PHY_STATUS_DISABLED != pDDB->SWI.LinkStatus[HwPortID].PhyStatus)
    {
        *pResponse = pDDB->SWI.PHY_FunctionPointers[HwPortID].EDDP_PHY_SetPowerDown(pDDB->hDDB, pDDB->hSysDev, HwPortID, LSA_FALSE /*LSA_BOOL Powerdown*/);
        EDDP_ASSERT_FALSE(*pResponse != EDD_STS_OK);
    }
    
    EDDP_CORE_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_GEN_ISR_LinkChange(UsrPortID=%d, HwPortID=%d): Finish setup of PHY: PHY-RealPortTxDelay=%d, PHY-RealPortRxDelay=%d, PHY-MaxPortTxDelay=%d, PHY-MaxPortRxDelay=%d",
        HwPortID, HwPortID, pDDB->SWI.LinkStatus[HwPortID].RealPortTxDelay, pDDB->SWI.LinkStatus[HwPortID].RealPortRxDelay, pDDB->SWI.LinkStatus[HwPortID].MaxPortTxDelay, pDDB->SWI.LinkStatus[HwPortID].MaxPortRxDelay);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWI_GEN_PhySTS_StateMachine_Transition_Stop+*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Utility function for EDDP_SWI_PhySTS_StateMachine()       +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_GEN_PhySTS_StateMachine_Transition_DoStop(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT8                   MediaType,
    LSA_UINT32                  HwPortIndex,
    EDD_RSP*                    pResponse)
{
    __attribute__((unused))LSA_UINT32          HwPortID;
    __attribute__((unused))LSA_UINT32          PhyAddress, OUI1RegAddress, OUI2RegAddress, OUIUnequalValue;
    __attribute__((unused))LSA_UINT32          reg_val, reg_address;

    HwPortID = HwPortIndex + 1;

    EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_SWI_GEN_PhySTS_StateMachine_Transition_Stop()");


	// set PHYCommand_PX | OUI1/2 polling is now active; when PHY is ready an interrupt is fired and EDDP_SWI_FinishPHYSetup() is called
	// select the right register address for current port number



    //// ***** clear field PHY_ADDRESS
    //reg_val &= ~(PNIP_REG_PHYCOMMAND_P1__MSK_PHY_ADDRESS<<PNIP_REG_PHYCOMMAND_P1_SHFT_PHY_ADDRESS);

    /*power up reset enable*/

    /*Polling disable*/

    // reset PHY

    // wait 100us


    // set new mode in SCRB.PHY_CONFIG.Px_FX_MODE
    // Cmd = EDDP_PHY_CONFIG_CMD_SET_FX_MODE/_SET_COPPER_MODE)

    // activate phy polling: PHYCommand_Px.Enable = 1

    // take away PHY-Reset: SCRB.PHY_CONFIG.Px_PHY_ENB = 1

    // a StatusChange-Interrupt will be generated when PHY is started up AND OUI-Registers could been read


    EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_SWI_GEN_PhySTS_StateMachine_Transition_Stop()");
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_SWI_PhySTS_StateMachine                            +*/
/*+                                                                                     +*/
/*+  Input                 :    EDDP_PHYSTS_SM_TRIGGER_TYPE     Trigger                 +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: state machine for Phy startup and restart                             +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define EDDP_STATUSCHANGE_MSK_PWRUP     (PNIP_REG_STATUSCHANGE__MSK_PHY_PWRUP_P1)
#define EDDP_STATUSCHANGE_MSK_STATUS    (PNIP_REG_STATUSCHANGE__MSK_STATUSCHANGE1_P1|PNIP_REG_STATUSCHANGE__MSK_STATUSCHANGE2_P1|PNIP_REG_STATUSCHANGE__MSK_STATUSCHANGE3_P1)
#define EDDP_STATUSCHANGE_MSK_ALL       (EDDP_STATUSCHANGE_MSK_STATUS | EDDP_STATUSCHANGE_MSK_PWRUP)

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_GEN_PhySTS_StateMachine(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_PHYSTS_SM_TRIGGER_TYPE Trigger,
    LSA_UINT32                  HwPortIndex
    )
{
    _EDDP_SWI_PHY_STS_SM_PTR_TYPE pStateMachine;
    EDDP_PHYSTS_SM_STATE_TYPE     State;
    EDD_RSP                       Response;
    LSA_UINT32                    HwPortID;

    Response                    = EDD_STS_OK;
    HwPortID                    = HwPortIndex + 1;
    pStateMachine               = &pDDB->PhyStatusSm[HwPortIndex];
    pStateMachine->Response     = EDD_STS_OK;

    State                       = pStateMachine->State;     // actual state of state machine

    EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H: ] IN: EDDP_SWI_GEN_PhySTS_StateMachine(): State=0x%X, Trigger=0x%X", 
                        State,
                        Trigger);

    switch(State)
    {
        case EDDP_PHYSTS_SM_STATE_INIT_WF_PWRUP:
        {
            if(EDDP_PHYSTS_SM_TRIGGER_INT_STATUS_CHANGE == Trigger)
            {
                pStateMachine->State = EDDP_PHYSTS_SM_STATE_RDY;

                EDDP_SWI_GEN_PhySTS_StateMachine_Transition_DoStart_DEV_SETUP(pDDB, HwPortIndex, &pStateMachine->Response);

                EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H: ] EDDP_SWI_GEN_PhySTS_StateMachine(): NOTE -> Trigger=0x%X Transision from State=0x%X to State=0x%X",
                                  Trigger,              
                                  State,
                                  pStateMachine->State);
            }
            else
            {
                // errorhandling
                Response = EDD_STS_ERR_SEQUENCE;

                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response); 

                EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H: ] EDDP_SWI_GEN_PhySTS_StateMachine(): WARNING -> Wrong Trigger for State! PrmState=0x%X, Trigger=0x%X",
                                    State,
                                    Trigger);
            }
        }
        break;

        case EDDP_PHYSTS_SM_STATE_RDY:
        {
            if(EDDP_PHYSTS_SM_TRIGGER_PLUG_COMMIT == Trigger)
            {
                pStateMachine->State = EDDP_PHYSTS_SM_STATE_RUN_WF_PWRUP;
                
                EDDP_SWI_GEN_PhySTS_StateMachine_Transition_DoStop(pDDB, pDDB->SWI.LinkStatus[HwPortID].MediaType, HwPortIndex, &pStateMachine->Response);

                EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H: ] EDDP_SWI_GEN_PhySTS_StateMachine(): NOTE -> Trigger=0x%X Transision from State=0x%X to State=0x%X",
                                  Trigger,              
                                  State,
                                  pStateMachine->State);
            }
            else if(EDDP_PHYSTS_SM_TRIGGER_INT_STATUS_CHANGE == Trigger)
            {
                //State persists, empty int?
            }
            else
            {
                // errorhandling
                Response = EDD_STS_ERR_SEQUENCE;

                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response); 

                EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H: ] EDDP_SWI_GEN_PhySTS_StateMachine(): WARNING -> Wrong Trigger for State! PrmState=0x%X, Trigger=0x%X",
                                    State,
                                    Trigger);
            }
        }
        break;

        case EDDP_PHYSTS_SM_STATE_RUN_WF_PWRUP:
        {
            if(EDDP_PHYSTS_SM_TRIGGER_INT_STATUS_CHANGE == Trigger)
            {
                pStateMachine->State = EDDP_PHYSTS_SM_STATE_RDY;

                EDDP_SWI_GEN_PhySTS_StateMachine_Transition_DoStart_PLUGGED(pDDB, HwPortIndex, &pStateMachine->Response);

                //call PORT_CHANGE statemachine
                /* 4th parameter (pRQBParam) is 0 because we don't have it in ISR */
                EDDP_PRMChangePortStateMachine(pDDB, EDDP_PRM_PORT_MODULE_SM_TRIGGER_PHY_ISR, HwPortIndex, (EDD_RQB_PRM_CHANGE_PORT_TYPE*)0/*no pRQBParam available*/);

                EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H: ] EDDP_SWI_GEN_PhySTS_StateMachine(): NOTE -> Trigger=0x%X Transision from State=0x%X to State=0x%X",
                                  Trigger,              
                                  State,
                                  pStateMachine->State);
            }
            else
            {
                // errorhandling
                Response = EDD_STS_ERR_SEQUENCE;

                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response); 

                EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H: ] EDDP_SWI_GEN_PhySTS_StateMachine(): WARNING -> Wrong Trigger for State! PrmState=0x%X, Trigger=0x%X",
                                    State,
                                    Trigger);
            }
        }
        break;

        default:
        {
            // errorhandling
            Response = EDD_STS_ERR_PARAM;

            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);  

            EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H: ] EDDP_SWI_GEN_PhySTS_StateMachine(): WARNING -> Unknown State=0x%X",
                                State);
        }
        break;
    }

    EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H: ] OUT: EDDP_SWI_GEN_PhySTS_StateMachine(): State=0x%X, Trigger=0x%X", 
                        pStateMachine->State,
                        Trigger);
    
}

/*****************************************************************************/
/*  end of file eddp_swi.c                                                   */
/*****************************************************************************/
