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
/*  F i l e               &F: os_socket.h                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  socket layer abstraction interface definition                            */
/*                                                                           */
/*****************************************************************************/
#ifndef OSSOCKET_H                         /* ----- reinclude-protection ----- */
#define OSSOCKET_H       

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*                                                                           */
/*  Date        Version        Who  What                                     */
/*  11.05.04    V0.2                                                         */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#define PNIO_SNMP_PORT      161


#ifdef INADDR_ANY
    #define SOCK_INADDR_ANY    INADDR_ANY   // same like INADDR_ANY
#else
    #define SOCK_INADDR_ANY    0            // same like INADDR_ANY
#endif



#define OSSOCK_TYPE_ASYNCHRON		1   // OsSockRecvFromAsync and OsSockRecvSync can be used
#define OSSOCK_TYPE_SYNCHRON        2   // Only OsSockRecvSync can be used for receiv. UDP
                                  
PNIO_INT32 OsSockInit	(void);
                   
PNIO_INT32 OsSockCleanup (void);

PNIO_INT32 OsSockUdpOpen	 (PNIO_UINT32*  SockId, PNIO_UINT32 SockType);
PNIO_INT32 OsSockUdpClose (PNIO_UINT32 SockId);
PNIO_INT32 OsSockUdpCloseAll (void);
PNIO_INT32 OsSockSaveOpenSockets (void);
PNIO_INT32 OsSockRestoreOpenSockets (void);

PNIO_INT32 OsSockBind	(PNIO_INT32 SockId, OS_SOCKADDR_IN  *pSockAdr, PNIO_INT32 pSockAdrLen);
PNIO_INT32 OsGetSockName (PNIO_INT32 SockId, OS_SOCKADDR_IN  *pSockAdr, PNIO_INT32* pSockAdrLen);


PNIO_INT32 OsSockSendTo (PNIO_INT32 Sock,
						PNIO_INT8* pBuf,
						PNIO_INT32 len,
						PNIO_INT32 flags,
						OS_SOCKADDR_IN * pSockAdr,
						PNIO_INT32 SockAdrLen);

PNIO_INT32 OsSockRecvFrom (PNIO_INT32 Sock,					// Socket Handle
						  PNIO_INT8* pBuf,					// receive buffer ptr
						  PNIO_INT32 len,					// receive buffer length
						  PNIO_INT32 flags,					// flags
						  OS_SOCKADDR_IN * pSockAdr,		// remote address pointer
						  PNIO_INT32 SockAdrLen,				// remote address length
						  void pCbf (void* UsrContext,
									 PNIO_UINT32 DatLen,
									 PNIO_UINT32 status),	// callback pointer
						  void* UsrContext);				// User context for Callback function


// *--------------------------------------------------------------------------------------------------
// *      OPTIONAL FUNCTIONS 
// *         OsSockRecvFromSync     (used for snmp mib2 support in the NET+OS example)
// *--------------------------------------------------------------------------------------------------
PNIO_INT32 OsSockRecvFromSync ( PNIO_INT32      SockId,				// Socket Handle
					            PNIO_UINT8      *pBuf,				// receive buffer ptr
					            PNIO_INT32      *pLen,				// receive buffer length
					            PNIO_INT32      flags,				// flags
					            OS_SOCKADDR_IN  *pSockAdr,	    	// remote address pointer
					            PNIO_INT32       SockAdrLen	    	// remote address length
                              );	



#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
