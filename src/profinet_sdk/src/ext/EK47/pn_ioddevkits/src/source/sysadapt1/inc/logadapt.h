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
/*  F i l e               &F: logadapt.h                                :F&  */
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
#ifndef _LOGADAPT_H
#define _LOGADAPT_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif
/*---------------------------------------------------------------------------*/
/*  logadapt.h         PNIO console on ethernet                              */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*                                                                           */
/*  Date        Version        Who  What                                     */
/*  19.01.05    V0.1                                                         */
/*                                                                           */
/*---------------------------------------------------------------------------*/



    #define PNLOG_UINT32    unsigned int
    #define PNLOG_UINT16    unsigned short
    #define PNLOG_UINT8     unsigned char
    #define PNLOG_INT32     int
    #define PNLOG_INT16     short
    #define PNLOG_INT8      char

    enum  LOGADAPT_COMMANDS_IN
    {
        LOGADAPT_CMD_GETCHAR      =  1,
        LOGADAPT_CMD_SET_LOGLVL   =  2,
        LOGADAPT_IN_NUMOF_ENTRIES
    } ;

    enum  LOGADAPT_COMMANDS_OUT
    {
        LOGADAPT_CMD_STRING      =  1,
        LOGADAPT_CMD_TRC2_LOG    =  2,
        LOGADAPT_OUT_NUMOF_ENTRIES
    } ;

    #define LOGADAPT_CONS_RD        1
    #define LOGADAPT_CONS_WR	    2
    #define LOGADAPT_TRC_WR	        3

    #if (INCLUDE_LOGADAPT == 1)
        void LogAdaptInit (void);

        void LogAdaptConsPrint (PNLOG_INT8* fmt, 
				                PNIO_va_list argptr);
        
        void LogAdaptTrcPrint  (PNLOG_INT8* fmt, 
				                PNIO_va_list argptr);
        
        
        PNLOG_UINT8  LogAdaptGetChar (void);
        PNIO_UINT32  LogAdapGetStrInp(PNIO_UINT8* pInpBuf, PNIO_UINT32 InputBase, PNIO_UINT32 InpBufLen);

    #endif

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
