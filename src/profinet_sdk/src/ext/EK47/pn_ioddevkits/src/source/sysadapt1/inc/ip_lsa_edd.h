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
/*  F i l e               &F: ip_lsa_edd.h                              :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  lsa interface to edd, used for ip-stack                                  */
/*                                                                           */
/*****************************************************************************/
#ifndef IP_LSA_EDD_H                   /* ----- reinclude-protection ----- */
#define IP_LSA_EDD_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif


void IpLsaStart(void);
void IpLsaStop(void);
LSA_VOID IpLsaCallbackEDD_arp       ( EDD_RQB_TYPE  *pRQB);
LSA_VOID IpLsaCallbackEDD_ip_udp    ( EDD_RQB_TYPE  *pRQB);
LSA_VOID IpLsaCallbackEDD_ip_tcp    ( EDD_RQB_TYPE  *pRQB);
LSA_VOID IpLsaCallbackEDD_icmp      ( EDD_RQB_TYPE  *pRQB);

EDD_UPPER_RQB_PTR_TYPE IpLsaGetRQB( LSA_UINT16 sizeParam, 
                                      int sizeBuffer, 
                                      EDD_LOWER_MEM_PTR_TYPE* pFrame  );
LSA_VOID IpLsaFreeRQB( EDD_RQB_TYPE *pRQB, LSA_UINT32 service );
LSA_VOID IpLsaNrtReceiveEDD(LSA_HANDLE_TYPE Handle, void* pCallback);
LSA_VOID IpLsaNrtSendEDD( char* pSendBuf, LSA_UINT32 sendLen);
LSA_VOID EddLsaNrtCancel(PNIO_UINT32 ChannelHandle, void* pCallback);

void     set_ipstack_initialized(PNIO_BOOL value);
void     set_ipstack_running    (PNIO_BOOL value);
LSA_BOOL is_ipstack_initialized(void);
LSA_BOOL is_ipstack_running    (void);

/*---------------------------------------------------------------------------*/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif
#endif


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
