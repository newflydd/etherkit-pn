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
/*  F i l e               &F: lsas_path_com.h                           :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  lsa parameters, path/layer matrix coordinates                            */
/*                                                                           */
/*****************************************************************************/

#ifndef LSAS_PATH_COM_H_
#define LSAS_PATH_COM_H_

/*****************************************************************************/
/* lsa parameters - path/layer matrix coordinates */

/************************************************************************
 * 4 (system)             PATH-LAYER Matrix (System)                         *
 *     0    1    2                                               (dummy)*
 * 3 | |  | |  | |  |    |    |    |    |    |    |    |    |    |    | *
 *   +----+----+----+----+----+----+----+----+----+----+----+----+----+ *
 * 2 | |  | |  | |  |    |    |    |    |    |    |    |    |    |DFC | *
 *   +----+----+----+----+----+----+----+----+----+----+----+----+----+ *
 * 1 | |  |FWUP|IUM |    |    |    |    |    |    |    |    |    |PPCO| *
 *   +----+----+----+----+----+----+----+----+----+----+----+----+----+ *
 * 0 |FS  |FS  |FS  |    |    |    |    |    |    |    |    |    |      *
 *   +----+----+----+----+----+----+----+----+----+----+----+----+----+      *
 *                                                                           *
 *****************************************************************************/

/************************************************************************************************************************************************
 *  6 (system)                PATH-LAYER Matrix (PNIO-Stack)                                                                                         *
 *      3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29    *
 *          cm                                                          sys  nrt  nrt  nrt  nrt                                                      *
 *  5 |cm  |pdsv|    |cm  |    |    |    |    |    |cm  |    |    |cm  |    |icmp|udp |arp |tcp |cm  |cm  |    |    |    |    |cm  |    |cm  |  *
 *    +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+  *
 *  4 | |  | |  |    | |  |    |    |    |    |    | |  |mrp |    | |  |    |    |    |    |    | |  | |  |mrp |    |    |    |mrp |    | |  |  *
 *    +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+  *
 *  3 |acp | |  |    |rpc |oha |acp |oha |oha |    | |  | |  |    | |  |    |    |    |    |    | |  |oha |oha |rpc |oha |    |    |oha | |  |  *
 *    +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+    +----+----+  *
 *  2      | |  |nare|    | |  | |  | |  | |  |    | |  | |  |nare| |  |    |    |    |    |    | |  |         | |  | |  |    |    |nare| |  |  *
 *         +----+----+    +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+         +----+----+----+    +----+----+  *
 *  1      | |  |dcp |    |dcp | |  | |  |lldp|gsy | |  | |  | |  |pof |    |tcip|tcip|tcip|tcip|gsy |         |sock|sock|sock|         |rema|  *
 *         +----+----+    +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+         +----+----+----+         +----+  *
 *  0      |edd |         |edd |edd |edd |edd |edd |edd |edd |edd |edd |edd |edd |edd |edd |edd |                                               *
 *         +----+         +----+----+----+----+----+----+----+----+----+----+----+----+----+----+                                               *
 *                                                                                                                                                   *
 ************************************************************************************************************************************************/

/*
    paths:
*/

#ifdef LSAS_CFG_USE_FS
/* fs paths */
#define LSAS_PATH_FS_APMA               ((LSAS_PATH_TYPE)0x0000)/* mac and ip address       : apma -> fs */
#define LSAS_PATH_FS_FWUP               ((LSAS_PATH_TYPE)0x0001)
#define LSAS_PATH_FS_IUM                ((LSAS_PATH_TYPE)0x0002)
#endif

#define LSAS_PATH_FS_NUM                                 0x0003

/* acp paths */
#define LSAS_PATH_ACP_CM                ((LSAS_PATH_TYPE)0x0003)
#define LSAS_PATH_ACP_NUM                                0x0001

/* cm paths */
#define LSAS_PATH_CMPDSV                ((LSAS_PATH_TYPE)0x0004)
#define LSAS_PATH_CM_NUM                                 0x0001

/* dcp pathes */
#define LSAS_PATH_DCP_NARE              ((LSAS_PATH_TYPE)0x0005)
#define LSAS_PATH_DCP_NUM                                0x0001

/* edd paths */
#define LSAS_PATH_EDD_CLRPC_CM          ((LSAS_PATH_TYPE)0x0006)
#define LSAS_PATH_EDD_DCP_OHA           ((LSAS_PATH_TYPE)0x0007)
#define LSAS_PATH_EDD_ACP               ((LSAS_PATH_TYPE)0x0008)
#define LSAS_PATH_EDD_OHA               ((LSAS_PATH_TYPE)0x0009)
#define LSAS_PATH_EDD_LLDP_OHA          ((LSAS_PATH_TYPE)0x000A)
#define LSAS_PATH_EDD_GSY_SYS           ((LSAS_PATH_TYPE)0x000B)
#define LSAS_PATH_EDD_GSY_ANNO          ((LSAS_PATH_TYPE)0x000C)
#define LSAS_PATH_EDD_CM                ((LSAS_PATH_TYPE)0x000D)
#define LSAS_PATH_EDD_MRP               ((LSAS_PATH_TYPE)0x000E)
#define LSAS_PATH_EDD_NARE              ((LSAS_PATH_TYPE)0x000F)
#define LSAS_PATH_EDD_POF_CM            ((LSAS_PATH_TYPE)0x0010)
#define LSAS_PATH_EDD_SYS               ((LSAS_PATH_TYPE)0x0011)
#define LSAS_PATH_EDD_TCIP_ICMP         ((LSAS_PATH_TYPE)0x0012)
#define LSAS_PATH_EDD_TCIP_UDP          ((LSAS_PATH_TYPE)0x0013)
#define LSAS_PATH_EDD_TCIP_ARP          ((LSAS_PATH_TYPE)0x0014)
#define LSAS_PATH_EDD_TCIP_TCP          ((LSAS_PATH_TYPE)0x0015)
#define LSAS_PATH_EDD_NUM                                0x0010

/* gsy paths */
#define LSAS_PATH_GSY_CM                ((LSAS_PATH_TYPE)0x0016)
#define LSAS_PATH_GSY_NUM                                0x0001

/* oha paths */
#define LSAS_PATH_OHASV_CM              ((LSAS_PATH_TYPE)0x0017)
#define LSAS_PATH_OHA_SYS               ((LSAS_PATH_TYPE)0x0018)
#define LSAS_PATH_OHA_NUM                                0x0002

/* sock paths */
#define LSAS_PATH_SOCK_CLRPC            ((LSAS_PATH_TYPE)0x0019)
#define LSAS_PATH_SOCK_OHA              ((LSAS_PATH_TYPE)0x001A)
#define LSAS_PATH_SOCK_SYS              ((LSAS_PATH_TYPE)0x001B)
#define LSAS_PATH_SOCK_STCP_S7PN        ((LSAS_PATH_TYPE)0x001C)
#define LSAS_PATH_SOCK_NUM                               0x0004

/* mrp paths */
#define LSAS_PATH_MRP_CM                ((LSAS_PATH_TYPE)0x001D)
#define LSAS_PATH_MRP_OHA               ((LSAS_PATH_TYPE)0x001E)
#define LSAS_PATH_MRP_NUM                                0x0002

/* nare paths*/

#define LSAS_PATH_NARE_OHA              ((LSAS_PATH_TYPE)0x001F)
#define LSAS_PATH_NARE_CM               ((LSAS_PATH_TYPE)0x0020)
#define LSAS_PATH_NARE_NUM                               0x0002

#define LSAS_PATH_NUM                 ( LSAS_PATH_FS_NUM   +     \
                                        LSAS_PATH_ACP_NUM  +     \
                                        LSAS_PATH_CM_NUM   +     \
                                        LSAS_PATH_DCP_NUM  +     \
                                        LSAS_PATH_EDD_NUM  +     \
                                        LSAS_PATH_GSY_NUM  +     \
                                        LSAS_PATH_OHA_NUM  +     \
                                        LSAS_PATH_SOCK_NUM +     \
                                        LSAS_PATH_MRP_NUM  +     \
                                        LSAS_PATH_NARE_NUM +     \
                                        1 )

/*
    layers:
*/


#define LSAS_PATH_LAYER_FS                   ((LSAS_LAYER_TYPE)0x0000)
#define LSAS_PATH_LAYER_PPCO                 ((LSAS_LAYER_TYPE)0x0001)  /* not to be implemented, dummy only */
#define LSAS_PATH_LAYER_IUA                  ((LSAS_LAYER_TYPE)0x0001)
#define LSAS_PATH_LAYER_DFC                  ((LSAS_LAYER_TYPE)0x0002)


#define LSAS_PATH_LAYER_EDD                  ((LSAS_LAYER_TYPE)0x0000)
#define LSAS_PATH_LAYER_DCP                  ((LSAS_LAYER_TYPE)0x0001)
#define LSAS_PATH_LAYER_LLDP                 ((LSAS_LAYER_TYPE)0x0001)
#define LSAS_PATH_LAYER_GSY                  ((LSAS_LAYER_TYPE)0x0001)
#define LSAS_PATH_LAYER_SOCK                 ((LSAS_LAYER_TYPE)0x0001)
#define LSAS_PATH_LAYER_TCIP                 ((LSAS_LAYER_TYPE)0x0001)
#define LSAS_PATH_LAYER_REMA                 ((LSAS_LAYER_TYPE)0x0001)
#define LSAS_PATH_LAYER_NARE                 ((LSAS_LAYER_TYPE)0x0002)
#define LSAS_PATH_LAYER_POF                  ((LSAS_LAYER_TYPE)0x0001)
#define LSAS_PATH_LAYER_CLRPC                ((LSAS_LAYER_TYPE)0x0003)
#define LSAS_PATH_LAYER_OHA                  ((LSAS_LAYER_TYPE)0x0003)
#define LSAS_PATH_LAYER_ACP                  ((LSAS_LAYER_TYPE)0x0003)
#define LSAS_PATH_LAYER_MRP                  ((LSAS_LAYER_TYPE)0x0004)
#define LSAS_PATH_LAYER_CM                   ((LSAS_LAYER_TYPE)0x0005)
#define LSAS_PATH_LAYER_STCP                 ((LSAS_LAYER_TYPE)0x0004)
#define LSAS_PATH_LAYER_S7PN                 ((LSAS_LAYER_TYPE)0x0005)

#define LSAS_PATH_LAYER_NUM                                    0x0006/* longest path: cm -> clrpc -> sock -> edd */


#endif /* LSAS_PATH_COM_H_ */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
