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
/*  F i l e               &F: lsas_usr.h                                :F&  */
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


/**
 * @file    lsas_usr.h
 * @brief   brief description
 * @author  cn3dit09
 * @version
 * @date    01.03.2012
 *
 * long description
 *
 */

/*****************************************************************************/

#ifndef LSAS_USR_H_
#define LSAS_USR_H_

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

/*---------------------------------------------------------------------------*/
/*  Response within RQB (Response)                                           */
/*---------------------------------------------------------------------------*/

#define LSAS_RSP  LSA_RESPONSE_TYPE  /* dont change */

/* LSA OK-Codes */
#define LSAS_RSP_OK                          (LSAS_RSP)    LSA_RSP_OK
/* LSA LSAS-Codes */
#define LSAS_RSP_ERR_RESOURCE                (LSAS_RSP)    LSA_RSP_ERR_RESOURCE
#define LSAS_RSP_ERR_PARAM                   (LSAS_RSP)    LSA_RSP_ERR_PARAM
#define LSAS_RSP_ERR_SEQUENCE                (LSAS_RSP)    LSA_RSP_ERR_SEQUENCE
#define LSAS_RSP_ERR_SYS_PATH                (LSAS_RSP)    LSA_RSP_ERR_SYS_PATH

/* NARE OK-Code (MaxOffset = 0x3F) */
/* LSA_RSP_OK_OFFSET = 0x40       */

#define LSAS_RSP_OK_CANCEL                   (LSAS_RSP)    (LSA_RSP_OK_OFFSET + 0x01)

/* NARE ERR-Codes (MaxOffset = 0x3E)*/
/* LSA_RSP_ERR_OFFSET = 0xC0       */

#define LSAS_RSP_ERR_OPCODE                  (LSAS_RSP)    (LSA_RSP_ERR_OFFSET + 0x01)
#define LSAS_RSP_ERR_CHANNEL_USE             (LSAS_RSP)    (LSA_RSP_ERR_OFFSET + 0x02)
#define LSAS_RSP_ERR_LL                      (LSAS_RSP)    (LSA_RSP_ERR_OFFSET + 0x03)
#define LSAS_RSP_ERR_TLV_MISMATCH            (LSAS_RSP)    (LSA_RSP_ERR_OFFSET + 0x04)
#define LSAS_RSP_ERR_TLV_UNKNOWN             (LSAS_RSP)    (LSA_RSP_ERR_OFFSET + 0x05)
#define LSAS_RSP_ERR_TLV_PARAM_ERROR         (LSAS_RSP)    (LSA_RSP_ERR_OFFSET + 0x06)
#define LSAS_RSP_ERR_NO_TIMER                (LSAS_RSP)    (LSA_RSP_ERR_OFFSET + 0x07)

LSA_VOID            lsas_com_set_hxdb(LSAS_EDD_HANDLE hddb);
LSAS_EDD_HANDLE     lsas_com_get_hddb(void);
#ifdef LSAS_CFG_USE_EDDP
LSA_VOID            lsas_com_get_sync_time(LSAS_PTCP_TIMESTAMP_TYPE *p_ptcp_time_stamp);
LSA_UINT16          lsas_com_get_sync_change_state(LSAS_PTCP_TIMESTAMP_TYPE *p_ptcp_old_time_stamp,
                                                   LSAS_PTCP_TIMESTAMP_TYPE *p_ptcp_new_time_stamp
                                                   );
#endif

LSA_UINT16 acp_version      (LSA_UINT16 length, LSA_VERSION_TYPE *version);
LSA_UINT16 clrpc_version    (LSA_UINT16 length, LSA_VERSION_TYPE *version);
LSA_UINT16 cm_version       (LSA_UINT16 length, LSA_VERSION_TYPE *version);
LSA_UINT16 dcp_version      (LSA_UINT16 length, LSA_VERSION_TYPE *version);
LSA_UINT16 eddi_version     (LSA_UINT16 length, LSA_VERSION_TYPE *version);
LSA_UINT16 eddp_version     (LSA_UINT16 length, LSA_VERSION_TYPE *version);
LSA_UINT16 edds_version     (LSA_UINT16 length, LSA_VERSION_TYPE *version);
LSA_UINT16 gsy_version      (LSA_UINT16 length, LSA_VERSION_TYPE *version);
LSA_UINT16 lldp_version     (LSA_UINT16 length, LSA_VERSION_TYPE *version);
LSA_UINT16 mrp_version      (LSA_UINT16 length, LSA_VERSION_TYPE *version);
LSA_UINT16 nare_version     (LSA_UINT16 length, LSA_VERSION_TYPE *version);
LSA_UINT16 oha_version      (LSA_UINT16 length, LSA_VERSION_TYPE *version);
LSA_UINT16 pof_version      (LSA_UINT16 length, LSA_VERSION_TYPE *version);
LSA_UINT16 sock_version     (LSA_UINT16 length, LSA_VERSION_TYPE *version);
LSA_UINT16 tcpip_version    (LSA_UINT16 length, LSA_VERSION_TYPE *version);

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif /* LSAS_USR_H_ */

/*** end of file *************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
