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
/*  F i l e               &F: trace_con.c                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Logging Output function into a circular buffer                           */
/*                                                                           */
/*  THIS MODULE HAS TO BE MODIFIED BY THE PNIO USER                          */
/*                                                                           */
/*****************************************************************************/

#include "compiler.h"
#include "os.h"
#include "pniousrd.h"
#include "pnio_trace.h"
#include "lsa_cfg.h"

#if (PNIO_TRACE == PNIO_TRACE_DK_CONSOLE)

    #define LTRC_ACT_MODUL_ID   140
    #define IOD_MODULE_ID       140



    // *----------------------------------------------------------------*
    // *   TrcConInit (BufSize)   
    // *----------------------------------------------------------------*
    // *   Inititalizes the Trace Module 
    // *    
    // *  input :  PNIO_UINT32  Bufsize   (Buffer-size in Bytes, 
    // *                                  value must be aligned 4 byte.   
    // *  out   :  PNIO_OK, PNIO_NOT_OK   
    // *----------------------------------------------------------------*
    PNIO_UINT32     TrcInit	  (PNIO_UINT32 BufSize)
    {
        LSA_UNUSED_ARG (BufSize);
        return (PNIO_OK);
    }



    void TrcArray (void* ptr, PNIO_UINT32 len)
    {
        PNIO_UINT32 i;
        for (i = 0; i < len; i++)
            PNIO_printf ("0x02x    ", *((PNIO_UINT8*)ptr + i));
    }

    // *----------------------------------------------------------------*
    // *   TrcStoreBuf    (void) 
    // *----------------------------------------------------------------*
    // *   stores the buffer into a persistent memory
    // *    
    // *  input :  -- 
    // *  out   :  PNIO_OK, PNIO_NOT_OK         
    // *----------------------------------------------------------------*
    PNIO_UINT32 TrcStoreBuf    (void)
    {
        PNIO_printf ("TrcStoreBuf is not supported\n");
        return (PNIO_NOT_OK);
    }
    
    void SaveTraceBuffer (void)
	{
        PNIO_printf ("SaveTraceBuffer is not supported\n");

	}

	void RestoreTraceBuffer (void)
	{
        PNIO_printf ("RestoreTraceBuffer is not supported\n");
    }

    PNIO_UINT32 TcpTraceUpload  (void)
    {
        PNIO_printf ("TcpTraceUpload is not supported\n");
        return (PNIO_NOT_OK);
    }


#endif


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
