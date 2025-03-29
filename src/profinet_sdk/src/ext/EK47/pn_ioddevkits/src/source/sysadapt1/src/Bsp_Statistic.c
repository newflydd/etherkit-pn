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
/*  F i l e               &F: Bsp_Statistic.c                           :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Ethernet statistic module                                                */
/*  performs a software statistic counter                                    */
/*                                                                           */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*                                                                           */
/*  Date        Version        Who  What                                     */
/*  17.02.11    V1.0           store startup ethernet parameter in dcp       */
/*---------------------------------------------------------------------------*/

#define LTRC_ACT_MODUL_ID   125
#define	IOD_MODULE_ID       125

#include "compiler.h"
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_DKSW)
    #include "glob_sys.h"
    #include "trc_if.h"
    #include "os.h"
//    #include "enic.h"
    #include "bspadapt.h"
    #include "pniousrd.h"
    #include "Bsp_Statistic.h"



    // *----------------------------------*
    // *  defines
    // *----------------------------------*
    #define BSP_ADAPT_ASSERT(cond) { \
	    if (!(cond)) { LSA_TRACE_00  (TRACE_SUBSYS_IOD_MEMORY, LSA_TRACE_LEVEL_FATAL, "IOX_ASSERT ERROR\n" );} \
	    }





    // *----------------------------------*
    // *  local data
    // *----------------------------------*
    static BSP_STATISTIC_DATA  Stat[IOD_CFG_PDEV_NUMOF_PORTS];

    // *-----------------------------------------------------------*
    // *  Bsp_StatisticRecFrame ()
    // *  Refreshes the statistic counter for a received frame
    // *
    // *  input:   PortID       Port id, 1...IOD_PDEV_CFG_NUM_OF_PORTS
    // *           FrameLen     Byte length of the received frame
    // *  output   return		Status  (LSA_OK)
    // *
    // *-----------------------------------------------------------*
    PNIO_VOID Bsp_StatisticRecFrame (PNIO_UINT32 PortID, PNIO_UINT32 DatLen, PNIO_VOID* pBuf)
    {
        // *** Increment statistic counter ***
        if ( *((PNIO_UINT8*)pBuf) & 0x01)   // destination is multicast or broadcast mac address?
            Stat[PortID-1].RxFramesNUcast++;
        else
            Stat[PortID-1].RxFramesUcast++;

        Stat[PortID-1].RxBytesTotal+= DatLen + 4;   // including 4 byte frame checksum !!
    }


    // *-----------------------------------------------------------*
    // *  Bsp_StatisticSendFrame ()
    // *  Refreshes the statistic counter for a sent frame
    // *
    // *  input:   PortID       Port id, 1...IOD_PDEV_CFG_NUM_OF_PORTS
    // *           FrameLen     Byte length of the sent frame
    // *  output   return		Status  (LSA_OK)
    // *
    // *-----------------------------------------------------------*
    PNIO_VOID Bsp_StatisticSendFrame (PNIO_UINT32 PortID, PNIO_UINT32 DatLen, PNIO_VOID* pBuf)
    {
        // *** Increment statistic counter ***
        if ( *((PNIO_UINT8*)pBuf) & 0x01)   // destination is multicast or broadcast mac address?
            Stat[PortID-1].TxFramesNUcast++;
        else
            Stat[PortID-1].TxFramesUcast++;
        Stat[PortID-1].TxBytesTotal+= DatLen + 4;   // including 4 byte frame checksum !!
    }



    // *-----------------------------------------------------------*
    // *  Bsp_ResetStatisticData ()
    // *  Resets the statistic counter for a sent frame
    // *
    // *  input:   PortID       Port id, 1...IOD_PDEV_CFG_NUM_OF_PORTS
    // *                                 0:  reset for all ports
    // *  output   return		Status  (LSA_OK)
    // *
    // *-----------------------------------------------------------*
    PNIO_UINT32 Bsp_ResetStatisticData (PNIO_UINT32 PortID)
    {
        OsMemSet (&Stat[PortID-1], 0, sizeof (Stat[0]));
        return (PNIO_OK);
    }

    PNIO_UINT32 Bsp_GetStatisticData (PNIO_UINT32 PortID, BSP_STATISTIC_DATA* pStat)
    {
        BSP_ADAPT_ASSERT (PortID <= IOD_CFG_PDEV_NUMOF_PORTS);

        if (PortID)
        {   // take port specific statistic data
            OsMemCpy (pStat, &Stat[PortID-1], sizeof (BSP_STATISTIC_DATA));
        }
        else
        {   // interface: add statistic data from all ports together
            int i;
            OsMemSet(pStat, 0, sizeof (BSP_STATISTIC_DATA));    // what is that ?
            for (i = 0; i < IOD_CFG_PDEV_NUMOF_PORTS; i++)
            {
                pStat->RxBytesTotal   += Stat[i].RxBytesTotal;
                pStat->TxBytesTotal   += Stat[i].TxBytesTotal;
                pStat->RxFramesUcast  += Stat[i].RxFramesUcast;
                pStat->TxFramesUcast  += Stat[i].TxFramesUcast;
                pStat->RxFramesNUcast += Stat[i].RxFramesNUcast;
                pStat->TxFramesNUcast += Stat[i].TxFramesNUcast;
            }
        }

        return (PNIO_OK);
    }
#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
