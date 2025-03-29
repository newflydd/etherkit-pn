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
/*  F i l e               &F: trace_mem_tcp.c                           :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Usr functions, called by pnio stack in case of events and alarms         */
/*                                                                           */
/*  THIS MODULE HAS TO BE MODIFIED BY THE PNIO USER                          */
/*                                                                           */
/*****************************************************************************/

    #include "compiler.h"
    #include "usriod_cfg.h"
#ifndef LSAS_CFG_USE_EXTERNAL_TCPIP_STACK
#if (PNIO_TRACE == PNIO_TRACE_DK_MEM)
    #include "pniousrd.h"
    #include "os.h"
    #include "nv_data.h"
    #include "bspadapt.h"
    #include "usriod_utils.h"
    #include "tcp_if.h"
    #include "trace_mem.h"

    #define FLASH_TRCBUF_START_ADR     0x30d00000     /* Flash start address in virtual memory */
    #define FLASH_TRCBUF_SIZE          0x00020000     /* 128 MB excluding the last 64k sector */

    #define TCP_TRC_UPLOAD_PORTNUMBER    998

    #define MAX_BLOCK_SIZE 0x1000

    typedef struct
    {
        int StartTag;
        int Command;
    }   TCP_COMMAND_HEADER;


    static int LocSockId, RemSockId;


    PNIO_UINT32 TcpTraceUpload  (void)
    {
	    // *-------------------------------------------
	    // *   receive flash image by tcp
	    // *-------------------------------------------
        TCP_COMMAND_HEADER Hdr;
        PNIO_UINT8* pBuf = TrcGetBufPtr();
        PNIO_UINT32 NumOfBytes;

	    PNIO_printf ("TCP interface initialized\n");

        // ***** init tcp interface ******
        LocSockId = tcp_if_inits();
        if (LocSockId == TCP_SOCK_ERROR)
        {
            PNIO_printf ("create socket error\n");
            return (PNIO_NOT_OK);
        }
	    PNIO_printf ("TCP interface wait on connection...\n");

        // ***** connect to client ******
        RemSockId = tcp_if_connectS (LocSockId, 0, TCP_TRC_UPLOAD_PORTNUMBER);
        if (RemSockId == TCP_SOCK_ERROR)
        {
            PNIO_printf ("tcp connect error\n");
            return (PNIO_NOT_OK);
        }
	    PNIO_printf ("...ok, established\n");
	    PNIO_printf ("receive Trace Upload Request...\n");

        // ***** read data header  ******
        NumOfBytes = tcp_if_receive (RemSockId,
                                     (PNIO_UINT8*) &Hdr,
                                     sizeof (Hdr));
        if (NumOfBytes != sizeof (Hdr))
        {
            PNIO_printf ("Error trace upload header header size = %d\n", NumOfBytes);
            tcp_if_disconnect (RemSockId);
            tcp_if_close (LocSockId);
            return (PNIO_NOT_OK);
        }



        {
            // **** send image file to tcp server *****
            int RestBytes       = (int) TrcGetFillSize();
            int RestBytes_htonl = OsHtonl (RestBytes);
            int BufOffset       = 0;
            int BytesToSend     = 0;
            int BytesSent       = 0;
#define HYSO_TEMP_FIX_FOR_TRACES_DOWNLOAD
#ifdef HYSO_TEMP_FIX_FOR_TRACES_DOWNLOAD
            int BuffersToSend = 1;
            int BuffersToSend_htonl = OsHtonl ( BuffersToSend );


	        BytesSent = tcp_if_send (RemSockId,
                                     (unsigned char*) &BuffersToSend_htonl,
                                     4);
#endif /*#ifdef HYSO_TEMP_FIX_FOR_TRACES_DOWNLOAD*/
            // **** send image file to tcp server *****
	        BytesSent = tcp_if_send (RemSockId,
                                     (unsigned char*) &RestBytes_htonl,
                                     4);
	        PNIO_printf("send %d data to server   \n", RestBytes);
            while (RestBytes > 0)
            {
            	BytesToSend = (RestBytes > MAX_BLOCK_SIZE) ? MAX_BLOCK_SIZE : RestBytes;;

            	BytesSent = tcp_if_send (RemSockId, pBuf + BufOffset, BytesToSend);

                if (BytesSent != BytesToSend)
                {
                    PNIO_printf ("error: sent bytes = %d of %d\n", BytesSent, BytesToSend);
                }
                RestBytes -= BytesToSend;
                BufOffset += BytesToSend;
	            PNIO_printf("sent %d\n", BytesSent);
            }
            PNIO_printf ("done\n");
        }



        // ***** disconnect and free memory ****
        OsWait_ms (100);
        tcp_if_disconnect (RemSockId);
        tcp_if_close (LocSockId);
        OsFree (pBuf);
        return (PNIO_OK);
    }
#endif
#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
