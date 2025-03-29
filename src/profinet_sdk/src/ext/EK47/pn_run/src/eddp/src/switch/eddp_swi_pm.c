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
/*  F i l e               &F: eddp_swi_pm.c                             :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP SWITCH port map functions                   */
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

#define LTRC_ACT_MODUL_ID   82                 
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_SWI_PM */

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

/*===========================================================================*/
/*                                 functions                                 */
/*===========================================================================*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname:             EDDP_SWI_portmap_check                       +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB         Pointer to Device Description Block                  (in) +*/
/*+  pDPB         Pointer to Device Parameter Block                    (in) +*/
/*+																			+*/
/*+  Result:      EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Checks if PortID to HwPortID mapping is plausible and     +*/
/*+               copies parameters from DPB to DDB.                        +*/
/*+               It is checked whether:                                    +*/
/*+                 - HwPortID is between 1 and MaxPortNumberDDB            +*/
/*+                 - each HwPortID is only used once                       +*/
/*+                 - there are no gaps between two PortIDs                 +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWI_portmap_check( 
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_UPPER_DPB_PTR_TYPE     pDPB)
{
	LSA_BOOL        end_table, found_hw_id[EDDP_MAX_PORT_CNT];
	LSA_UINT32      i, k;
    LSA_UINT32      PortCntTotal;       // port count total of all interfaces (or of one DDB)
    LSA_UINT16      MaxPortNumberDDB;   // max port number of one DDB

    PortCntTotal        = 0;
    MaxPortNumberDDB    = 0;

    // initialize array with LSA_FALSE
    for ( i=0; i < EDDP_MAX_PORT_CNT; i++ )
	{
		found_hw_id[i] = LSA_FALSE;
	}

    // which Hardware is used ?
    switch (pDPB->Hardware)
    {
        case EDDP_HW_ERTEC200P:
        case EDDP_HW_ERTEC200P_FPGA:    
        {
            MaxPortNumberDDB = EDDP_SWI_MAX_PORT_CNT_ERTEC200P;
        }
        break;

        case EDDP_HW_HERA:
        case EDDP_HW_HERA_FPGA:
        {
            if(EDDP_HW_IF_A == pDPB->HardwareIF)
            {
                MaxPortNumberDDB = EDDP_MAX_PORT_CNT_HERA_IF_A;
            }
            else
            {
                MaxPortNumberDDB = EDDP_MAX_PORT_CNT_HERA_IF_B;
            }
        }
        break;

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_portmap_check(): FATAL -> Hardware(0x%X) is not supported!", 
                pDPB->Hardware);
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }

	end_table = LSA_FALSE;

    // loop through all ports of this interface
	for ( k=0; k < MaxPortNumberDDB; k++ )
	{
		if (pDPB->SWI.PortMap.PortID_x_to_HardwarePort_y[k] == EDDP_PORT_NOT_CONNECTED)
		{
            end_table = LSA_TRUE;
		}
		else if ((pDPB->SWI.PortMap.PortID_x_to_HardwarePort_y[k] > 0) && (pDPB->SWI.PortMap.PortID_x_to_HardwarePort_y[k] <= MaxPortNumberDDB))
		{
			if (end_table)
			{
    			EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
                EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_portmap_check(IF[%d]): ERROR -> There is a gap between two PortIDs!", 
                    pDPB->HardwareIF);
				return (EDD_STS_ERR_PARAM);
			}
			else
			{	
				if (found_hw_id[pDPB->SWI.PortMap.PortID_x_to_HardwarePort_y[k]-1])
				{
    				EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
                    EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_portmap_check(IF[%d]): ERROR -> Hardware PortID(%d) is used twice!", 
                        pDPB->HardwareIF, pDPB->SWI.PortMap.PortID_x_to_HardwarePort_y[k]);
					return (EDD_STS_ERR_PARAM);
				}
				else
				{
                    // check is ok so far
					pDDB->SWI.PortCnt++;
                    PortCntTotal++;

					found_hw_id[pDPB->SWI.PortMap.PortID_x_to_HardwarePort_y[k]-1] = LSA_TRUE;

                    // check PortCnt of current interface
                    if ((pDDB->SWI.PortCnt > MaxPortNumberDDB) || (pDDB->SWI.PortCnt > EDD_CFG_MAX_PORT_CNT))
                    {
		                EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
                        EDDP_SWI_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_portmap_check(IF[%d]): ERROR -> SWI.PortCnt(%d) does not match to MaxPortNumberDDB(%d) or EDD_CFG_MAX_PORT_CNT(%d)", 
                            pDPB->HardwareIF, pDDB->SWI.PortCnt, MaxPortNumberDDB, EDD_CFG_MAX_PORT_CNT);
		                return (EDD_STS_ERR_PARAM);
                    }
                }
            }
		}
		else
		{
    		EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
            EDDP_SWI_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_portmap_check(IF[%d]): ERROR -> HwPortID(%d) is not 1..%d! PortID=%d", 
                pDPB->HardwareIF, pDPB->SWI.PortMap.PortID_x_to_HardwarePort_y[k], MaxPortNumberDDB, k+1);
			return (EDD_STS_ERR_PARAM);
		}
	}

    if(EDDP_HW_IF_A == pDPB->HardwareIF)
    {
        if(pDDB->SWI.PortCnt > EDDP_MAX_PORT_CNT_HERA_IF_A)
        {
            EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
            EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_portmap_check(IF[%d]): ERROR -> pDPB->SWI.PortCnt(%d) > EDDP_MAX_PORT_CNT_HERA_IF_A(%d)", 
                              pDPB->HardwareIF, pDDB->SWI.PortCnt, EDDP_MAX_PORT_CNT_HERA_IF_A);
            return (EDD_STS_ERR_PARAM);
        }
    }
    else
    {
        if(pDDB->SWI.PortCnt > EDDP_MAX_PORT_CNT_HERA_IF_B)
        {
            EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
            EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_portmap_check(IF[%d]): ERROR -> pDPB->SWI.PortCnt(%d) > EDDP_MAX_PORT_CNT_HERA_IF_B(%d)", 
                              pDPB->HardwareIF, pDDB->SWI.PortCnt, EDDP_MAX_PORT_CNT_HERA_IF_B);
            return (EDD_STS_ERR_PARAM);
        }
    }

    if( (EDDP_HW_HERA == pDPB->Hardware) || (EDDP_HW_HERA_FPGA == pDPB->Hardware) )
    {
        LSA_UINT8 iter_i;
        LSA_UINT8 iter_j;
        LSA_UINT32 minAsicPort;
        minAsicPort= 0xFFFFFFFF;

        for(iter_i = 0 ; iter_i < PortCntTotal/* EDDP_MAX_PORT_CNT_HERA */; iter_i++)
        {
            // check for multiusage of ASCI ports
            for(iter_j = iter_i+1 ; iter_j < PortCntTotal ; iter_j++)
            {
                if( (EDDP_PORT_NOT_CONNECTED == pDPB->SWI.PortMap.PortID_x_to_HardwarePort_y[iter_i]) ||
                    (EDDP_PORT_NOT_CONNECTED == pDPB->SWI.PortMap.PortID_x_to_HardwarePort_y[iter_i])   )
                {
                    // don't care
                }
                else
                {
                    // check multple usage
                    if(pDPB->SWI.PortMap.PortID_x_to_ASICPort_y[iter_i] == pDPB->SWI.PortMap.PortID_x_to_ASICPort_y[iter_j])
                    {
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
                        EDDP_SWI_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_portmap_check(IF[%d]): ERROR -> PortID_x_to_ASICPort_y[%d]=%d equals PortID_x_to_ASICPort_y[%d]=%d", 
                                          pDPB->HardwareIF, iter_i, pDPB->SWI.PortMap.PortID_x_to_ASICPort_y[iter_i], iter_j, pDPB->SWI.PortMap.PortID_x_to_ASICPort_y[iter_j]);
                        return (EDD_STS_ERR_PARAM);
                    }
                }
            }
            if(EDDP_PORT_NOT_CONNECTED == pDPB->SWI.PortMap.PortID_x_to_HardwarePort_y[iter_i])
            {
                // don't care
            }
            else
            {
                // check
                if(0 == pDPB->SWI.PortMap.PortID_x_to_ASICPort_y[iter_i])
                {
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
                    EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_portmap_check(IF[%d]): ERROR -> PortID_x_to_ASICPort_y[%d]=%d is null", 
                                      pDPB->HardwareIF, iter_i, pDPB->SWI.PortMap.PortID_x_to_ASICPort_y[iter_i]);
                    return (EDD_STS_ERR_PARAM);
                }
                if(pDPB->SWI.PortMap.PortID_x_to_ASICPort_y[iter_i] > EDDP_MAX_PORT_CNT_HERA)
                {
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
                    EDDP_SWI_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_portmap_check(IF[%d]): ERROR -> PortID_x_to_ASICPort_y[%d]=%d is higher or equal EDDP_MAX_PORT_CNT_HERA=%d",
                                      pDPB->HardwareIF, iter_i, pDPB->SWI.PortMap.PortID_x_to_ASICPort_y[iter_i], EDDP_MAX_PORT_CNT_HERA);
                    return (EDD_STS_ERR_PARAM);
                }

                if(0 == pDPB->SWI.PortMap.PortID_x_to_SMIPort_y[iter_i])
                {
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
                    EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_portmap_check(IF[%d]): ERROR -> PortID_x_to_SMIPort_y[%d]=%d is null",
                                      pDPB->HardwareIF, iter_i, pDPB->SWI.PortMap.PortID_x_to_SMIPort_y[iter_i]);
                    return (EDD_STS_ERR_PARAM);
                }
                if(pDPB->SWI.PortMap.PortID_x_to_SMIPort_y[iter_i] > EDDP_MAX_PORT_CNT_HERA)
                {
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
                    EDDP_SWI_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_portmap_check(IF[%d]): ERROR -> PortID_x_to_SMIPort_y[%d]=%d is higher or equal EDDP_MAX_PORT_CNT_HERA=%d",
                                      pDPB->HardwareIF, iter_i, pDPB->SWI.PortMap.PortID_x_to_SMIPort_y[iter_i], EDDP_MAX_PORT_CNT_HERA);
                    return (EDD_STS_ERR_PARAM);
                }
            }

            if(pDPB->SWI.PortMap.PortID_x_to_ASICPort_y[iter_i] < minAsicPort)
            {
                minAsicPort = pDPB->SWI.PortMap.PortID_x_to_ASICPort_y[iter_i];
            }
            
        }
    }
    
    // copy port mapping from DPB to DDB
	{
		LSA_UINT8 port_iter;
		for (port_iter = 0; port_iter < EDDP_MAX_PORT_CNT; port_iter++)
		{	    
			pDDB->SWI.PortMap.PortID_x_to_HardwarePort_y[port_iter] = pDPB->SWI.PortMap.PortID_x_to_HardwarePort_y[port_iter];
			pDDB->SWI.PortMap.PortID_x_to_ASICPort_y[port_iter] = pDPB->SWI.PortMap.PortID_x_to_ASICPort_y[port_iter];
			pDDB->SWI.PortMap.PortID_x_to_SMIPort_y[port_iter] = pDPB->SWI.PortMap.PortID_x_to_SMIPort_y[port_iter];
			pDDB->SWI.PortMap.SMI_PHY_Address_y[port_iter] = 0;
		}
	}

    // check PortCntTotal of all interfaces
    if (PortCntTotal > MaxPortNumberDDB)
    {
	    EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
        EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_portmap_check(): ERROR -> PortCntTotal(%d) > MaxPortNumberDDB(%d)", 
            PortCntTotal, MaxPortNumberDDB);
	    return (EDD_STS_ERR_PARAM);
    }

    // store total port count to DDB
	pDDB->SWI.PortCnt = PortCntTotal;

	return (EDD_STS_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname:             EDDP_SWI_get_HwPortID                        +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB         Pointer to Interface Description Block               (in) +*/
/*+  PortID       Port ID                                              (in) +*/
/*+                                                                         +*/
/*+  return       HwPortID (Hardware port ID)                         (out) +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Returns the HwPortID for the specified PortID.            +*/
/*+               If the PortID is not valid the function returns 0.        +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_SWI_get_HwPortID(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT32                  PortID)
{
    LSA_UINT32      HwPortID;

    if ((PortID < 1) || (PortID > pDDB->SWI.PortCnt))
	{
        EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_get_HwPortID(): FATAL -> given PortID(%d) is not between 1..%d!",
            PortID, pDDB->SWI.PortCnt);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        return (0);     // 0 is not a valid hardware port ID; won't come here anyway..
	}

    HwPortID = pDDB->SWI.PortMap.PortID_x_to_HardwarePort_y[PortID-1];
    EDDP_ASSERT_FALSE((HwPortID < 1) || (HwPortID > EDDP_MAX_PORT_CNT));

    return (HwPortID);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname:             EDDP_SWI_get_PortID_and_IF                   +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB         Pointer to Device Description Block                  (in) +*/
/*+  HwPortID     Hardware Port ID                                     (in) +*/
/*+  pIFNumber	  Address of Interface Number						  (out) +*/
/*+                                                                         +*/
/*+  return       Port ID												    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Returns the PortID and IF number for the specified        +*/
/*+               HwPort ID. In case the IF number is not relevant for the  +*/
/*+               user; set this pointer to LSA_NULL.                       +*/
/*+               If the HwPortID is not valid the function returns 0.      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_SWI_get_PortID_and_IF(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT32                  HwPortID,
	LSA_UINT32*                 pIFNumber)
{	
	LSA_UINT32		k;

	for ( k=0; k < pDDB->SWI.PortCnt; k++ )
	{
		if (pDDB->SWI.PortMap.PortID_x_to_HardwarePort_y[k] == HwPortID)
		{		
			if ( !(LSA_HOST_PTR_ARE_EQUAL(pIFNumber, LSA_NULL)) )
            {
                *pIFNumber = 0;
            }

            return (k+1);
		}
	}

    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_get_PortID_and_IF(): FATAL -> given HwPortID(%d) is not mapped!", 
        HwPortID);
    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    return (0);     // 0 is not a valid Port ID; won#t come here anyway..
}


/*****************************************************************************/
/*  end of file eddp_swi_pm.c                                                */
/*****************************************************************************/
