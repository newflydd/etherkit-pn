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
/*  F i l e               &F: stcp_pck.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Internal header file                                                     */
/*  Defines internal types and data (and constants) for stcp,                */
/*  which must be used with packing alignment.                               */
/*                                                                           */
/*****************************************************************************/

#ifndef STCP_PCK_H                     /* ----- reinclude-protection ----- */
#define STCP_PCK_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif


/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Version        Who  What                                     */
/*  2002-02-19                 st   new file for the PDUs, which must not contain any */
/*                                  filler bytes                                      */
/*                                  Compiler error fixed                              */
/* 2002-05-23                  st   Global LSA header: apply current changes          */
/*
 * 2004-03-30                  st   Access error with tasking compiler V2.0
 *                                  for structure STCP_ROUT_TSAP ==> structure
 *                                  define in stcp_pck.txt instead of stcp_int.h
 *                                  to ensure that the compiler does not insert any filler bytes
 * 2004-04-15                  st   Sub structures that can be used for the error structure
 *                                  for the individual error cases
 * 2004-05-27                  st   DEV_2 is not needed ==> removed
 * 2004-07-12                  st   Adaptation to LSA_GLOB_P2.1.0.0_0.4.1.1
 *                                  C++ parentheses and copyright
 * 2004-10-04                  st   STCP_DEV_2_LOCAL_ERROR_TYPE also has to be defined here
 *                                  it is already included in stcp_int
 * 2005-08-19                   st  Special characters	file changes for it to be also readable in Notepad
 *                                  Replaced special characters
 * 2006-10-19                   st  Translation into english
 * 2007-07-24                   st  new structure element for CHA_2_LOCAL_ERROR: rfc_tpdu_size 
 * 2007-08-21                   st  in den Koepfen das Datum und die Version auf XXXXX setzen
 * 2012-02-23                   st  Fileheader	darauf achten , das es ueberall 
 *                                  "P a c k a g e     Stcp" heisst, nicht manchmal stcp oder STCP

 *                                                                           */
/*****************************************************************************/

// zzxch stcp : >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include "pnio_pck1_on.h"
// zzxch stcp : <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/



/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/
/* ********************************* *************************************** */
/* these structures/PDUs have to lie packed in the storage,                  */
/* there may not be any filler bytes!                                        */
/* ********************************************************** ************** */

typedef struct stcp_crcc_tpdu_tag {
    LSA_UINT8 tpkt_version;   /* 0x03: RFC1006_VERS */
    LSA_UINT8 tpkt_reserved;  /* 0x00 */
    LSA_UINT16 tpkt_length;   /* Length of the complete packet */
    LSA_UINT8 tpkt_rfc_li;    /* Length of the header from code to parameter */
    LSA_UINT8 tpkt_opcode;    /* 0xE0 or 0xD0: Opcode for CR or CC */
    LSA_UINT16 tpkt_dst;      /* CN_ID of the recipient of the PDU (CR:0x00) */
    LSA_UINT16 tpkt_src;      /* CN_ID of the sender of the PDU */
    LSA_UINT8 tpkt_class;     /* 0x00 for TP0: Transport class 0 */
    LSA_UINT8 tpkt_para[1];   /* parameter buffer */
} STCP_CRCC_PDU;


typedef struct stcp_data_tpdu_tag {
    LSA_UINT8 tpkt_version;   /* 0x03 */
    LSA_UINT8 tpkt_reserved;  /* 0x00 */
    LSA_UINT16 tpkt_length;   /* Length of the complete packet */
    LSA_UINT8 tpkt_rfc_li;    /* 0x02:  Length of the header from code to  EOT */
    LSA_UINT8 tpkt_opcode;    /* 0xF0: Opcode for DT */
    LSA_UINT8 tpkt_seqnr_eot; /* EOT selection; sequence no is always  0. */
    LSA_UINT8 tpkt_data[1];   /* Data buffer */
} STCP_DATA_PDU;


typedef struct stcp_rout_tsap_tag {
    LSA_UINT8 tsap_type;      /* structure handle for TSAPs with length >16 */
    LSA_UINT8 subnet_id_len;  /* Filler length for subnet_id[] */
    LSA_UINT8 st_addr_len;    /* Filler length for st_addr[] */
    LSA_UINT8 subtsap_id_len; /* variable length for subtsap_id[] */
    LSA_UINT8 subnet_id[STCP_ROUT_TSAP_FIX_SUBNET_ID_LEN];
    LSA_UINT8 st_addr[STCP_ROUT_TSAP_FIX_ST_ADDR_LEN];
    LSA_UINT8 subtsap_id[1];
} STCP_ROUT_TSAP;

/*-------------------------------------------------------------------------------------------*/
/* Structures for the error entry in the request block, in the LSA_RQB_LOCAL_ERROR structure */
/* the error type is entered in the error_code_id                                            */
/* The length of the individual structures must not exceed 12 bytes!                         */
/*-------------------------------------------------------------------------------------------*/

/* Start of every error entry with module and line number */
#define STCP_LOCAL_ERROR_HEADER                                    \
    LSA_UINT16    module;            /* Module ID */            \
    LSA_UINT16    line;              /* line number in the module */

/* Error type STCP_CHA_1_LOCAL_ERROR */
typedef struct stcp_cha_1_local_error_tag
{
    STCP_LOCAL_ERROR_HEADER          /* Start of every error entry with module and line */
    LSA_UINT16    number_channels;    /* Number of max. possible channels */
} STCP_CHA_1_LOCAL_ERROR_TYPE;

/* Error type STCP_CHA_2_LOCAL_ERROR */
typedef struct stcp_cha_2_local_error_tag
{
    STCP_LOCAL_ERROR_HEADER          /* Start of every error entry with module and line */
    LSA_UINT16    cha_handle;        /* Handle of the channel */
    LSA_UINT8    cha_state;          /* State of the channel */
    LSA_BOOL    listen_channel;      /* Flag indicating whether a LISTEN is to be used on this channel or not */
    LSA_UINT16    accept_number;     /* only valid for channel with LISTEN: */
                                     /* Number of the ACCEPT jobs which are to be passed down */
    LSA_UINT8    rcv_mode;           /* set receive mode */
    LSA_UINT8   rfc_tpdu_size;       /* rfc_tpdu_size from detail_ptr */
} STCP_CHA_2_LOCAL_ERROR_TYPE;

/* Error type  STCP_CHA_3_LOCAL_ERROR */
typedef struct stcp_cha_3_local_error_tag
{
    STCP_LOCAL_ERROR_HEADER          /* Start of every error entry with module and line */
    LSA_UINT16    cha_handle;        /* Handle of the channel */
    LSA_UINT16            ret_val;    /* Return message of the call GET_PATH_INFO */
    LSA_SYS_PATH_TYPE    sys_path;    /* Path specification */
} STCP_CHA_3_LOCAL_ERROR_TYPE;

/* Error type  STCP_CN_1_LOCAL_ERROR */
typedef struct stcp_cn_1_local_error_tag
{
    STCP_LOCAL_ERROR_HEADER              /* Start of every error entry with module and line */
    LSA_UINT16    cn_id;             /* Connection ID (may never be greater than UINT16 because of the protocol) */
    LSA_UINT8    cn_state;           /* Main state of this connection */
    LSA_UINT8    cntl_state;         /* Sub state of this connection */
    LSA_OPCODE_TYPE    opcode;       /* Opcode in the cntl-Rqb in the connection */
} STCP_CN_1_LOCAL_ERROR_TYPE;

/* Error type  STCP_CN_2_LOCAL_ERROR */
typedef struct stcp_cn_2_local_error_tag
{
    STCP_LOCAL_ERROR_HEADER          /* Start of every error entry with module and line */
    LSA_UINT16    number_connections;    /* Number of max. possible connections */
} STCP_CN_2_LOCAL_ERROR_TYPE;

/* Error type  STCP_CN_3_LOCAL_ERROR */
typedef struct stcp_cn_3_local_error_tag
{
    STCP_LOCAL_ERROR_HEADER         /* Start of every error entry with module and line */
    LSA_UINT16    cn_id;            /* Connection ID (may never be greater than UINT16 because of the protocol) */
    LSA_UINT8    cn_state;          /* main state of this connection */
    LSA_UINT8    cntl_state;        /* sub state of this connection */
    LSA_UINT8    snd_state;         /* send state */
    LSA_UINT8    rcv_state;         /* receive state */
    LSA_UINT8    rcv_mode;          /* set receive mode */
} STCP_CN_3_LOCAL_ERROR_TYPE;

/* Error type  STCP_CN_4_LOCAL_ERROR */
typedef struct stcp_cn_4_local_error_tag
{
    STCP_LOCAL_ERROR_HEADER         /* Start of every error entry with module and line */
    LSA_UINT16    cn_id;            /* Connection ID (may never be greater than UINT16 because of the protocol) */
    LSA_UINT8    cn_state;          /* main state of this connection */
    LSA_UINT8    cntl_state;        /* sub state of this connection */
    LSA_UINT16    number_devices;   /* Number of the max. possible lower devices / sockets */
} STCP_CN_4_LOCAL_ERROR_TYPE;

/* Error type STCP_CN_5_LOCAL_ERROR */
typedef struct stcp_cn_5_local_error_tag
{
    STCP_LOCAL_ERROR_HEADER         /* Start of every error entry with module and line */
    LSA_UINT16    cn_id;            /* Connection ID (may never be greater than UINT16 because of the protocol) */
    LSA_UINT8    cn_state;          /* main state of this connection */
    LSA_UINT8    cntl_state;        /* sub state of this connection */
    LSA_UINT16    dev_handle;       /* Handle for the lower connection/device/socket -> Device */
    LSA_UINT8    dev_state;         /* State of the device */
} STCP_CN_5_LOCAL_ERROR_TYPE;

/* Error type STCP_CN_6_LOCAL_ERROR */
typedef struct stcp_cn_6_local_error_tag
{
    STCP_LOCAL_ERROR_HEADER         /* Start of every error entry with module and line */
    LSA_UINT16    cn_id;            /* Connection ID (may never be greater than UINT16 because of the protocol) */
    LSA_UINT8    cn_state;          /* main state of this connection */
    LSA_UINT8    cntl_state;        /* sub state of this connection */
    LSA_UINT8    rcv_state;         /* State when receiving */
    LSA_UINT8    tpkt_version;      /* Version handle from the received PDU */
} STCP_CN_6_LOCAL_ERROR_TYPE;

/* Error type STCP_CN_7_LOCAL_ERROR */
typedef struct stcp_cn_7_local_error_tag
{
    STCP_LOCAL_ERROR_HEADER         /* Start of every error entry with module and line */
    LSA_UINT16    cn_id;            /* Connection ID (may never be greater than UINT16 because of the protocol) */
    LSA_UINT8    cn_state;          /* main state of this connection */
    LSA_UINT8    cntl_state;        /* sub state of this connection */
    LSA_UINT8    tpkt_opcode;       /* Opcode from the PDU */
    LSA_UINT8    tpkt_rfc_li;       /* Length of the header (see above) from the PDU */
} STCP_CN_7_LOCAL_ERROR_TYPE;

/* Error type STCP_DEV_1_LOCAL_ERROR */
typedef struct stcp_dev_1_local_error_tag
{
    STCP_LOCAL_ERROR_HEADER         /* Start of every error entry with module and line */
    LSA_UINT16    dev_handle;       /* Handle of the device */
    LSA_UINT8    dev_state;         /* State of the device */
    LSA_UINT8    tpkt_version;      /* Version handle from the received PDU */
} STCP_DEV_1_LOCAL_ERROR_TYPE;

/* Error type  STCP_DEV_2_LOCAL_ERROR */
typedef struct stcp_dev_2_local_error_tag
{
    STCP_LOCAL_ERROR_HEADER         /* Start of every error entry with module and line */
    LSA_UINT16    dev_handle ;      /* Handle of the device */
    LSA_UINT8    dev_state;         /* State of the device */
} STCP_DEV_2_LOCAL_ERROR_TYPE;

/* Error type  STCP_RQB_1_LOCAL_ERROR */
typedef struct stcp_rqb_1_local_error_tag
{
    STCP_LOCAL_ERROR_HEADER        /* Start of every error entry with module and line */
    LSA_OPCODE_TYPE    opcode;     /* Opcode like STCP has read it in the RQB */
} STCP_RQB_1_LOCAL_ERROR_TYPE;

/* Error type  STCP_RQB_2_LOCAL_ERROR */
typedef struct stcp_rqb_2_local_error_tag
{
    STCP_LOCAL_ERROR_HEADER              /* Start of every error entry with module and line */
    L4_CN_ID_TYPE    cn_id;        /* cn_id like STCP has read it in the RQB  */
} STCP_RQB_2_LOCAL_ERROR_TYPE;

/* Error type  STCP_USER_ID_LOCAL_ERROR */
typedef struct stcp_user_id_local_error_tag
{
    STCP_LOCAL_ERROR_HEADER              /* Start of every error entry with module and line */
    LSA_UINT32    uvar32;          /* uvar32 part of the user_id */
} STCP_USER_ID_LOCAL_ERROR_TYPE;

/*===========================================================================*/
/*                                  data                                     */
/*===========================================================================*/

#if (STCP_MODULE_ID == 1)
#endif

// zzxch stcp : >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include "pnio_pck_off.h"
// zzxch stcp : <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


/*****************************************************************************/
/*  end of file STCP_PCK.H                                                   */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of STCP_PCK_H */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
