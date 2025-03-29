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
/*  F i l e               &F: OsSocket_INICH.c                          :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  socket layer abstraction interface, implementation for Windows           */
/*                                                                           */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*                                                                           */
/*  Date        Version        Who  What                                     */
/*  11.05.04    V0.2                                                         */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#include "compiler.h"
#define SOCKET      PNIO_UINT32
#include "os_taskprio.h"

#if (INCLUDE_LOGADAPT == 1)

#undef  INVALID_SOCKET
#undef  SOCKET_ERROR
#undef  SOL_SOCKET

#include "tcpapp.h"
#include "bsdsock.h"
#include "tcp.h"
#include "tcp_if.h"
#include "logadapt.h"

#define LTRC_ACT_MODUL_ID 105 
#define SOCK_MODULE_ID    105

#define UDP_CONSOLE_RECV_PAUSE_TIME_MS     50



    static const  char _ThisFileName_[] = __FILE__;



    #define MAXNUM_SOCKETS		20		    // maximum 5 sockets open
    #define FREE				0
    #define USED				1
    #define OSSOCK_INITIALIZED  0x72391854  // a value that is a impropable fluke in a uninitialized memory
    #define NO_THREAD_ID		0

    #define MAXLEN_IPSTACK      1460

    typedef struct
    {
	    LSA_UINT32		SockId;			// Socket ID
	    char*			pBuf;
	    int				len;
	    int				flags;
	    OS_SOCKADDR_IN*	pSockAdr;
	    int				SockAdrLen;
	    void*			UsrContext;		// User context for Callback function
        void*			pCbf;			// callback pointer
    }RCV_RQU;		//receive request parameter


    typedef struct
    {
	    LSA_UINT32  used;			// occupation tag: socket is used/free
	    LSA_UINT32  SockHndl;		// Socket Handle, returned by the operating system
	    LSA_UINT32  ThreadId;		// ID of the socket - service thread
        LSA_UINT32  SockType;       // specifies, if receiving UDP can be asynchronous or synchron only

        // *---- rest for debugging only 
	    LSA_UINT32	BindLocPort;	// port number
        LSA_UINT32  BindAddr;
        LSA_UINT16  BindFamily;
	    LSA_UINT32	BindYes;		// bind call has been made
        LSA_UINT32  NumOfPendingAsyncRecRq;   // number of asynchrounos pending receive requests
        LSA_UINT32  TotalRecvCnt;   // total number of ReceiveFrom Requests
        LSA_UINT32  TotalSendCnt;   // total number of SendTo Requests

        RCV_RQU     LastRcvRq;
    } SOCK_PROP;

    typedef struct 
    {
       SOCK_PROP	Sock[MAXNUM_SOCKETS];
       SOCK_PROP	SockSav[MAXNUM_SOCKETS];
       LSA_UINT32   initialized;
    }  SOCK_MANAGER;


    static SOCK_MANAGER	Pool;

    static LSA_INT32 SockTask (void);
    static LSA_INT32 OsSockRebind (LSA_INT32 SockId);




    // *---------------------------------------------------------------
    // *  OsSockInit (void)
    // *  initializes the local socket data. Because the tcp/ip stack
    // *  has been already started by the threadX system, here is 
    // *  nothing else to do.
    // * 
    // *  Input :  ---
    // *  Output:  ---
    // * 
    // *---------------------------------------------------------------
    LSA_INT32 OsSockInit (void)
    {
	    OsMemSet (&Pool, 0, sizeof (Pool));
        Pool.initialized = OSSOCK_INITIALIZED;

        return (PNIO_OK);
    }


    // *---------------------------------------------------------------
    // *  OsSockSaveOpenSockets (void)
    // *  save list of open sockets
    // * 
    // *---------------------------------------------------------------
    PNIO_INT32 OsSockSaveOpenSockets (void)
    {
        OsMemCpy (&Pool.SockSav, &Pool.Sock, sizeof (Pool.Sock));
	    return (PNIO_OK);
    }





    // *---------------------------------------------------------------
    // *  OsSockUdpOpen (LSA_UINT32*  SockId, LSA_UINT32 SockType)
    // *  Opens a datagram socket. For every socket a new service task is 
    // *  started. This service task handles a recv_from() socket call,
    // *  which is normally a blocking call. 
    // *  If a socket is closed, the service task will not be deleted and
    // *  can be used at the next OsSockUdpOpen() call. 
    // * 
    // *  Input :  *SockId    pointer to memory for socket Id
    // *           SockType   OSSOCK_TYPE_ASYNCHRON (Default), OSSOCK_TYPE_SYNCRHON
    // *  Output:  *SockId    contains the socket ID
    // * 		  return     PNIO_OK   
    // *                      PNIO_NOT_OK
    // *---------------------------------------------------------------
    LSA_INT32 OsSockUdpOpen (LSA_UINT32*  SockId, PNIO_UINT32 SockType)
    {
       SOCKET	SockRd;
       LSA_INT32		i;
       LSA_INT32		Optval = 1;		//Set an boolean socket option
       LSA_INT32		Error;

       if (Pool.initialized != OSSOCK_INITIALIZED)
       {// OsSockInit() has not been called...
           LSA_TRACE_00  (TRACE_SUBSYS_APPL_PLATFORM, LSA_TRACE_LEVEL_FATAL, "OsSockInit() must be called first\n" );
	       return (PNIO_NOT_OK);
       }

       // find free place in socket pool
       for (i = 0; i < MAXNUM_SOCKETS; i++)
       {
		    if (Pool.Sock[i].used == FREE)
		    {
			    break;
		    }
       }

       if (MAXNUM_SOCKETS == i)
       {// no free socket available
	       return (PNIO_NOT_OK);
       }

       OsMemSet (&Pool.Sock[i], 0, sizeof (SOCK_PROP));

       // ******** BEGIN of platform specific code ********
       SockRd = t_socket (AF_INET, SOCK_DGRAM, 0);

       Error = t_setsockopt (   SockRd, 
                                SOL_SOCKET, 
                                //SO_REUSEADDR, 
                                SO_NONBLOCK, 
                                (char*) &Optval,
                                 sizeof(LSA_INT32)); 
 
       if ( (Error!= 0) || (SockRd == INVALID_SOCKET )) 
       {
		    // easy...
            LSA_TRACE_00  (TRACE_SUBSYS_APPL_PLATFORM, LSA_TRACE_LEVEL_FATAL, "Error OsSockUdpOpen()" );
		    return (PNIO_NOT_OK);       // wird nicht erreicht
       }

       // ******** END of platform specific code ********

       // *** start task for receive ***
       Pool.Sock[i].SockHndl   = SockRd;
       Pool.Sock[i].used	    = USED;
       Pool.Sock[i].SockType	= SockType;

       *SockId =  i+1;		// socket id = Field index + 1
       return (PNIO_OK);
    }

    // *---------------------------------------------------------------
    // * 
    // *  OsSockUdpClose	 (LSA_UINT32 SockId)
    // *  closes the socket.
    // * 
    // * 
    // *  Input :  SockId     socket to close
    // * 		  return     PNIO_OK   
    // *                      PNIO_NOT_OK
    // * 
    // *---------------------------------------------------------------
    LSA_INT32 OsSockUdpClose (LSA_UINT32 SockId)
    {
        LSA_INT32 Status;

	    // *** get socket handle from socket id (id = list index+1)
        SOCKET	SockHndl;

        // *** check for valid socket ID
	    if ((SockId == 0) || (SockId > MAXNUM_SOCKETS))
        {
            LSA_TRACE_01  (TRACE_SUBSYS_APPL_PLATFORM, LSA_TRACE_LEVEL_FATAL, "Error SockId = 0x%x\n", SockId );
		    return (PNIO_NOT_OK);       // wird nicht erreicht
        }
	    

	    // *-----------------------------------------
	    // * Close Read Socket
	    // *-----------------------------------------
	    SockHndl = Pool.Sock[SockId-1].SockHndl;	// get windows socket ID 


	    // ******** BEGIN of platform specific code ********
	    Status = t_socketclose (SockHndl);
	    if( Status)
		    {
			    // easy...
                LSA_TRACE_00  (TRACE_SUBSYS_APPL_PLATFORM, LSA_TRACE_LEVEL_FATAL, "sock_request: cannot close socket\n");
			    return (PNIO_NOT_OK);
		    }

	    // ******** END of platform specific code ********


        // ** the thread is alive and will not be deleted, because its faster to use him
        // ** again at the next OsSockUdpOpen(). Only the used-tag is set to state FREE
	    Pool.Sock[SockId-1].used			    = FREE;

	    return (PNIO_OK);
    }


    // *---------------------------------------------------------------
    // * 
    // *  OsSockBind (LSA_INT32 SockId, void *pSockAdr, LSA_INT32 pSockAdrLen)
    // *  binds a socket to a port
    // * 
    // *  Input :  SockId        socket to close
    // * 		  *pSockAdr     socket data (port, IP address)
    // * 		  pSockAdrLen   length of *pSockAdr
    // * 
    // * 		  return     PNIO_OK   
    // *                      PNIO_NOT_OK
    // * 
    // * 
    // *---------------------------------------------------------------
    LSA_INT32 OsSockBind (LSA_INT32 SockId, OS_SOCKADDR_IN *pSockAdr, LSA_INT32 SockAdrLen)
    {

	    // *** get socket handle from socket id (id = list index+1)
	    struct sockaddr_in*  pSock = (struct  sockaddr_in *) pSockAdr;
        SOCKET	SockHndl;
        LSA_UNUSED_ARG (SockAdrLen);


        if (SockId == 0)
		    return (PNIO_NOT_OK);
	    SockHndl = Pool.Sock[SockId-1].SockHndl;	// get windows socket ID 

        #if 1   // debug only
            switch (SockId)
            {
                case 1:
                        SockId = SockId;        // dummy
                        break;
                case 2:
                        SockId = SockId;        // dummy
                        break;
                case 3:
                        SockId = SockId;        // dummy
                        break;
                default:
                        SockId = SockId;        // dummy
                        break;
            }
        #endif

       // ******** BEGIN of platform specific code ********
	    {
		    LSA_INT32 Status;
		    struct sockaddr_in SockAdr;

		    // **** convert sock-address from PNIO format into platform format ***
		    SockAdr.sin_family				= pSockAdr->sin_family;
		    SockAdr.sin_port				= pSockAdr->sin_port;
		    SockAdr.sin_addr.s_addr     	= pSockAdr->sin_addr.S_un.S_addr;

		    // **** bind to socket  ***
		    Status = t_bind(SockHndl, (struct sockaddr *) &SockAdr, sizeof (SockAdr));
		    if( Status)
		    {
			    // easy...
                LSA_TRACE_00  (TRACE_SUBSYS_APPL_PLATFORM, LSA_TRACE_LEVEL_FATAL, "sock_request: cannot bind");
			    return (PNIO_NOT_OK);
		    }
	    }
       // ******** END of platform specific code ********
       Pool.Sock[SockId-1].BindLocPort  = pSockAdr->sin_port;
       Pool.Sock[SockId-1].BindFamily   = pSockAdr->sin_family;
       Pool.Sock[SockId-1].BindAddr     = pSockAdr->sin_addr.S_un.S_addr;
       Pool.Sock[SockId-1].BindYes++;


     return (PNIO_OK);
    }

    static LSA_INT32 OsSockRebind (LSA_INT32 SockId)
    {
        SOCKET	SockHndl;



        if (SockId == 0)
		    return (PNIO_NOT_OK);

        if (Pool.Sock[SockId-1].BindYes == 0)
            return (PNIO_OK);


        SockHndl = Pool.Sock[SockId-1].SockHndl;	// get windows socket ID 

       // ******** BEGIN of platform specific code ********
	    {
		    LSA_INT32 Status;
		    struct sockaddr_in SockAdr;

		    // **** convert sock-address from PNIO format into platform format ***
		    SockAdr.sin_family				= Pool.Sock[SockId-1].BindFamily;
		    SockAdr.sin_port				= Pool.Sock[SockId-1].BindLocPort;
		    SockAdr.sin_addr.s_addr     	= Pool.Sock[SockId-1].BindAddr;

		    // **** bind to socket  ***
		    Status = t_bind(SockHndl, (struct sockaddr *) &SockAdr, sizeof (SockAdr));
		    if( Status)
		    {
			    // easy...
                LSA_TRACE_00  (TRACE_SUBSYS_APPL_PLATFORM, LSA_TRACE_LEVEL_FATAL, "sock_request: cannot bind");
			    return (PNIO_NOT_OK);
		    }
	    }


        return (PNIO_OK);
    }


    // *---------------------------------------------------------------
    // *  OsGetSockName 	(LSA_INT32 SockId, void *pSockAdr, LSA_INT32* pSockAdrLen)
    // *
    // * 
    // * 
    // * 
    // *---------------------------------------------------------------
    LSA_INT32 OsGetSockName (LSA_INT32 SockId, OS_SOCKADDR_IN* pSockAdr, LSA_INT32* pSockAdrLen)
    {
	    LSA_INT32 Status;

	    // *** get socket handle from socket id (id = list index+1)
        SOCKET	SockHndlRd;
        if (SockId == 0)
		    return (PNIO_NOT_OK);
	    SockHndlRd = Pool.Sock[SockId-1].SockHndl;	// get windows socket ID 


       // ******** BEGIN of platform specific code ********
	    {
		    struct sockaddr_in SockAdr;
        
		    // **** convert sock-address from PNIO format into platform format ***
		    LSA_INT SockAdrLen			= sizeof (SockAdr);
		    SockAdr.sin_family			= pSockAdr->sin_family;
		    SockAdr.sin_port			= pSockAdr->sin_port;
		    SockAdr.sin_addr.s_addr		= pSockAdr->sin_addr.S_un.S_addr;

		    // **** call platform specific send function  ***
		    Status = t_getsockname (SockHndlRd, (struct sockaddr *) &SockAdr, &SockAdrLen);

		    // **** convert sock-address from platform format into PNIO format ***
		    pSockAdr->sin_family			= SockAdr.sin_family;
		    pSockAdr->sin_port				= SockAdr.sin_port;
		    pSockAdr->sin_addr.S_un.S_addr	= SockAdr.sin_addr.s_addr;
		    *pSockAdrLen					= SockAdrLen;

		    // **** encode  platform specific error response  ***
		    if (Status)
		    {
			    // easy...
                LSA_TRACE_00  (TRACE_SUBSYS_APPL_PLATFORM, LSA_TRACE_LEVEL_FATAL, "sock_request: cannot getsockname");
			    return (PNIO_NOT_OK);
		    }
	    }
	    // ******** END of platform specific code ********
	    
        return (PNIO_OK);
    }



    // *********************************************************************************
    // **************** U D P  -  S E N D  **** ****************************************
    // *********************************************************************************

    // *-------------------------------------------------
    // * OsSockSendTo   (...)
    // * sends a UDP Telegram. The function is synchronous
    // *
    // *
    // *-------------------------------------------------
    LSA_INT32 OsSockSendTo (LSA_INT32		SockId,
						    char*			pBuf,
						    LSA_INT32		len,
						    LSA_INT32		flags,
						    OS_SOCKADDR_IN* pSockAdr,
						    LSA_INT32		SockAdrLen)
    {
	    // *** get socket handle from socket id (id = list index+1)
        SOCKET	SockHndlWr;

	    LSA_UNUSED_ARG (SockAdrLen);
	    LSA_UNUSED_ARG (flags);

        if (SockId == 0)
		    return (PNIO_NOT_OK);
	    SockHndlWr = Pool.Sock[SockId-1].SockHndl;	// get windows socket ID 

	    // ******** BEGIN of platform specific code ********
	    {
		    LSA_UINT32			Status;
		    struct sockaddr_in	SockAdr;

		    // **** convert sock-address from PNIO format into platform format ***
		    SockAdr.sin_family		= pSockAdr->sin_family;
		    SockAdr.sin_port		= pSockAdr->sin_port;
		    SockAdr.sin_addr.s_addr	= pSockAdr->sin_addr.S_un.S_addr;

		    if (len > MAXLEN_IPSTACK)
                len = MAXLEN_IPSTACK;

		    // **** call platform specific send function  ***
		    Status = t_sendto(SockHndlWr, //SOCKET s,
						    pBuf, //LPWSABUF lpBuffers,
						    len, //LPDWORD lpNumberOfBytesSent,
						    0, //DWORD dwFlags,
						    (struct sockaddr *)&SockAdr,
                            sizeof (SockAdr));			//const struct sockaddr FAR *lpTo,


		    if (Status == SOCKET_ERROR)
		    {
                LSA_TRACE_01  (TRACE_SUBSYS_APPL_PLATFORM, LSA_TRACE_LEVEL_FATAL, "sock_do_send err(0x%x) cannot WSASendTo\n", Status);
		    }
	    }
	    return (PNIO_OK);
    }



    // *-------------------------------------------------
    // * OsSockRecvFromSync   ()
    // * receives a UDP Telegram. The function is synchronous.
    // *
    // *-------------------------------------------------

    LSA_INT32 OsSockRecvFromSync 
                       (LSA_INT32       SockId,				// Socket Handle
					    PNIO_UINT8     *pBuf,				// receive buffer ptr
					    LSA_INT32      *pLen,				// receive buffer length
					    LSA_INT32       flags,				// flags
					    OS_SOCKADDR_IN *pSockAdr,	    	// remote address pointer
					    LSA_INT32       SockAdrLen			// remote address length
                        )				
    {
	    struct sockaddr_in 	SockAdr;
        LSA_UINT32          DataLen;
        LSA_UINT32          Status;
        SOCKET			    SockHndlRd;		// socket handle from TCPIP Stack (OS

	    if (*pLen > MAXLEN_IPSTACK)
            *pLen = MAXLEN_IPSTACK;

        // **** convert sock-address from PNIO format into platform format ***

        SockAdr.sin_family			 = pSockAdr->sin_family;
	    SockAdr.sin_port			 = pSockAdr->sin_port;
	    SockAdr.sin_addr.s_addr      = pSockAdr->sin_addr.S_un.S_addr;
	    SockAdrLen					 = sizeof (SockAdr);

	    // *** get socket handle from socket id (id = list index+1)
	    SockHndlRd = Pool.Sock[SockId - 1].SockHndl;	// get operating specific socket ID 

	    // **** call platform specific receive function  ***
	    Pool.Sock[SockId - 1].NumOfPendingAsyncRecRq++;

	    DataLen = t_recvfrom (SockHndlRd,			// socket handle
						    pBuf,		    	        // pointer to receive buffer
						    *pLen,			            // length of receive buffer
						    flags,		                // flags, here  0000
						    (struct sockaddr *)&SockAdr,// remote address (UDP sender)
                            &SockAdrLen);               // length of struct. SockAdr

        while (DataLen == SOCKET_ERROR)
        { // asynchronous socket -->  wait and try again until success
	        DataLen = t_recvfrom (SockHndlRd,			// socket handle
						        pBuf,		    	        // pointer to receive buffer
						        *pLen,			            // length of receive buffer
						        flags,		                // flags, here  0000
						        (struct sockaddr *)&SockAdr,// remote address (UDP sender)
                                &SockAdrLen);               // length of struct. SockAdr
            OsWait_ms (UDP_CONSOLE_RECV_PAUSE_TIME_MS);
        }

	    Pool.Sock[SockId - 1].NumOfPendingAsyncRecRq--;

	    // **** convert sock-address from platform format into PNIO format ***
	    pSockAdr->sin_family			= SockAdr.sin_family;
	    pSockAdr->sin_port				= SockAdr.sin_port;
	    pSockAdr->sin_addr.S_un.S_addr	= SockAdr.sin_addr.s_addr;


	    // **** encode  platform specific error response  ***
	    if (DataLen == SOCKET_ERROR)
	    {// *** a socket error occured ***
            LSA_TRACE_00  (TRACE_SUBSYS_APPL_PLATFORM, LSA_TRACE_LEVEL_FATAL, "Error OsSockTask() after receiveFrom()" );
	        Status = PNIO_NOT_OK;
	    }
	    else
	    {// **** ok, data have been received ***
		    Status = PNIO_OK;
	    }

        *pLen = DataLen;

        return (Status);
    }



#endif // Compiler != MSVC6


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
