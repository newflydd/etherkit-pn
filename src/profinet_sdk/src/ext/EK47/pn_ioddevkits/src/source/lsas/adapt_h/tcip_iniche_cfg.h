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
/*  F i l e               &F: tcip_iniche_cfg.h                         :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Configuration module                                                     */
/*                                                                           */
/*****************************************************************************/
#ifndef TCIP_INICHE_CFG_H                       /* ----- reinclude-protection ----- */
#define TCIP_INICHE_CFG_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif


/*===========================================================================*/
/*=== pcIOX configuration (do not copy/paste it) ============================*/
/*===========================================================================*/

#define TCIP_CFG_MAXARPS  256 /* maximum mumber of ARP table entries */
#define TCIP_CFG_SYS_SERVICES  0x4C /* 0x4C == 0b01001100 == Layer 7+4+3, see RFC3418, sysServices */
#define TCIP_CFG_SOMAXCONN  5 /* maximum length of the TCP backlog (1-127, 0 defaults to 5) */
#define TCIP_CFG_COMPRESS_RCV_MBUFS  0 /* off, see sockbuf define SB_MBCOMP */
#define TCIP_CFG_NPDEBUG  0 /* off */

#define TCIP_CFG_NUMBIGBUFS  120 /* number of packets of size BIGBUFSIZE */
#define TCIP_CFG_NUMLILBUFS   30 /* number of packets of size LILBUFSIZE */

#define TCIP_MAX_IF_COUNT 1

#ifndef TCIP_MODULE_ID /* hopefully this does not disturb the non-InterNiche files */
#define tcip_log_fct "iniche"
#endif

/*===========================================================================*/
/*===========================================================================*/

#if TCIP_CFG_TRACE_MODE != 0 /* LTRC-IF */
extern int tcip_log_level;  /* see LTRC_LEVEL_TYPE */
#endif /* LTRC-IF */

void  tcp_sleep(void * timeout);
void  tcp_wakeup(void * wake);

void LOCK_NET_RESOURCE(int res);
void UNLOCK_NET_RESOURCE(int res);

#ifndef TCIP_CFG_COPY_ON_SEND
#define TCIP_CFG_COPY_ON_SEND  0  
#endif

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif  /* of TCIP_CFG_H (reinclude protection) */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
