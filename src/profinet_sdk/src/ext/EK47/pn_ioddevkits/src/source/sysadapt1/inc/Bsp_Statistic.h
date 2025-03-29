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
/*  F i l e               &F: Bsp_Statistic.h                           :F&  */
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
#ifndef _BSP_STATISTIC_H
#define _BSP_STATISTIC_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif
    /*---------------------------------------------------------------------------*/
    /*  Bsp_Statistic.h                                                          */
    /*---------------------------------------------------------------------------*/
    /*                                                                           */
    /*  H i s t o r y :                                                          */
    /*                                                                           */
    /*  Date        Version        Who  What                                     */
    /*                                                                           */
    /*                                                                           */
    /*---------------------------------------------------------------------------*/

    // *----------------------------------------------------------
    // *    defines
    // *----------------------------------------------------------
    typedef struct
    {   
       PNIO_UINT32  TxFramesUcast;     // number of total unicast frames sent
       PNIO_UINT32  TxFramesNUcast;    // number of total non-unicast frames sent
       PNIO_UINT32  TxBytesTotal;     // number of total bytes sent
       PNIO_UINT32  RxFramesUcast;     // number of total unicast frames received
       PNIO_UINT32  RxFramesNUcast;    // number of total non-unicast frames received
       PNIO_UINT32  RxBytesTotal;     // number of total bytes received
    }  BSP_STATISTIC_DATA;


    


    // *-------------------------------------------------
    // *    functions for receiving packets
    // *-------------------------------------------------
    PNIO_UINT32 Bsp_ResetStatisticData (PNIO_UINT32 PortID);                          // resets statistic counter
    PNIO_VOID   Bsp_StatisticSendFrame (PNIO_UINT32 PortID, PNIO_UINT32 FrameLen, PNIO_VOID* pBuf);    // refreshes send counter
    PNIO_VOID   Bsp_StatisticRecFrame  (PNIO_UINT32 PortID, PNIO_UINT32 FrameLen, PNIO_VOID* pBuf);    // refreshes receive counter
    PNIO_UINT32 Bsp_GetStatisticData (PNIO_UINT32 PortID, BSP_STATISTIC_DATA* pStat); // reads current statistic data from stack


    // *----------------------------------------------------------
    // *    BSP macro interface
    // *----------------------------------------------------------
    // ***** macros, called by the PNIO stack ********
    #define BSP_RESET_STATISTIC_DATA(_PortID)           Bsp_ResetStatisticData(_PortID)
    #define BSP_GET_STATISTIC_DATA(PortID,pStat)        Bsp_GetStatisticData(PortID,pStat)

    // ***** macros, called by the user (BSP adaptation interface) ********
    #define BSP_STATISTIC_SEND_FRAME(_PortID,_FrameLen,_pBuf) Bsp_StatisticSendFrame(_PortID,_FrameLen,_pBuf)
    #define BSP_STATISTIC_REC_FRAME(_PortID,_FrameLen,_pBuf)  Bsp_StatisticRecFrame(_PortID,_FrameLen,_pBuf)



#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
