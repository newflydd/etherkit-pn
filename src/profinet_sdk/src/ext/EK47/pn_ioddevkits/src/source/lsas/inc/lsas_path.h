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
/*  F i l e               &F: lsas_path.h                               :F&  */
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
 * @file    lsas_path.h
 * @brief   brief description
 * @author  cn3dit09
 * @version
 * @date    01.03.2012
 *
 * long description
 *
 */

/*****************************************************************************/

#ifndef LSAS_PATH_H_
#define LSAS_PATH_H_

#ifndef LSAS_PATH_EXT_ATTR
    #define LSAS_PATH_EXT_ATTR extern
#else
    #define LSAS_PATH_EXT_ATTR_DECLARE
#endif

/*****************************************************************************/
/* lsa parameters (detail and system information) */

#include "lsas_path_com.h"

/* detail_ptr for all paths -------------------------------------------------*/
LSAS_PATH_EXT_ATTR struct
{
    /* APMA_PDB_LSA_PATH_FS_APMA */
    /* APMA_PDB_LSA_PATH_FS_FWUM */
    /* APMA_PDB_LSA_PATH_FS_IUA */
#ifdef LSAS_CFG_USE_FS
    FS_DETAIL_INFO_TYPE     fs_ldb_flash_0;
#endif
    /* APMA_PDB_LSA_PATH_EDD_CLRPC_CM */

    SOCK_DETAILS_TYPE       edd_clrpc_ldb_cm_sock;
    CLRPC_DETAIL_TYPE       edd_clrpc_ldb_cm_clrpc;
    CM_DETAIL_TYPE          edd_clrpc_ldb_cm_cm;

    /* APMA_PDB_LSA_PATH_MRP_CM */

    MRP_DETAIL_TYPE         mrp_ldb_cm_mrp;
    CM_DETAIL_TYPE          mrp_ldb_cm_cm;

    /* APMA_PDB_LSA_PATH_MRP_OHA */

    OHA_CDB_TYPE            mrp_ldb_oha_oha;
    MRP_DETAIL_TYPE         mrp_ldb_oha_mrp;

    /* APMA_PDB_LSA_PATH_NARE_OHA */

    OHA_CDB_TYPE            nare_ldb_oha_oha;
    NARE_CDB_TYPE           nare_ldb_oha_nare;

    /* LSAS_PATH_NARE_CM */

    CM_DETAIL_TYPE          nare_ldb_cm_cm;
    NARE_CDB_TYPE           nare_ldb_cm_nare;

    /* APMA_PDB_LSA_PATH_ACP_CM */

    ACP_DETAIL_TYPE         acp_ldb_cm_acp;
    CM_DETAIL_TYPE          acp_ldb_cm_cm;

    /* APMA_PDB_LSA_PATH_EDD_ACP */
#ifdef LSAS_CFG_USE_EDDI
    EDDI_CDB_NRT_TYPE       edd_nrt_acp;   /* nrt (alarms) in acp */

    EDDI_CDB_TYPE           edd_ldb_acp_edd;
#endif

#ifdef LSAS_CFG_USE_EDDP
    EDDP_CDB_CRT_TYPE       edd_crt_cm_edd;
    EDDP_CDB_NRT_TYPE       edd_nrt_acp_edd;   /* nrt (alarms) in acp */

    EDDP_CDB_TYPE           edd_ldb_acp_edd;
#endif

#ifdef LSAS_CFG_USE_EDDS
    EDDS_CDB_NRT_TYPE		edd_nrt_acp_edd;

    EDDS_CDB_TYPE 			edd_ldb_acp_edd;
#endif
    ACP_DETAIL_TYPE         edd_ldb_acp_acp;

    /* APMA_PDB_LSA_PATH_DCP_NARE */

    DCP_DETAIL_TYPE         dcp_ldb_nare_dcp;
    NARE_CDB_TYPE           dcp_ldb_nare_nare;

    /* APMA_PDB_LSA_PATH_SOCK_CLRPC */

    SOCK_DETAILS_TYPE       sock_ldb_clrpc;
    CLRPC_DETAIL_TYPE       clrpc_ldb_clrpc;

    /* APMA_PDB_LSA_PATH_SOCK_OHA */

    SOCK_DETAILS_TYPE       sock_ldb_oha_sock;
    OHA_CDB_TYPE            sock_ldb_oha_oha;

    /* APMA_PDB_LSA_PATH_SOCK_SYS */

    SOCK_DETAILS_TYPE       sock_ldb_sys_sock;

#ifdef LSAS_CFG_USE_STCP
    /* APMA_PDB_LSA_PATH_SOCK_STCP  */

    SOCK_DETAILS_TYPE       sock_ldb_s7pn_sock;
    STCP_DETAILS_TYPE       sock_ldb_s7pn_stcp;
    S7PN_DETAILS_TYPE       sock_ldb_s7pn_s7pn;
#endif /* LSAS_CFG_USE_STCP */

    /* APMA_PDB_LSA_PATH_EDD_OHA */
#ifdef LSAS_CFG_USE_EDDI
    EDDI_CDB_TYPE           edd_ldb_oha_edd;
#endif
#ifdef LSAS_CFG_USE_EDDP
    EDDP_CDB_TYPE           edd_ldb_oha_edd;

    EDDP_CDB_NRT_TYPE       edd_nrt_oha_edd;
#endif
#ifdef LSAS_CFG_USE_EDDS
    EDDS_CDB_NRT_TYPE		edd_nrt_oha_edd;

    EDDS_CDB_TYPE 			edd_ldb_oha_edd;
#endif
    OHA_CDB_TYPE            edd_ldb_oha_oha;

    /* APMA_PDB_LSA_PATH_EDD_DCP_OHA */
#ifdef LSAS_CFG_USE_EDDI
    EDDI_CDB_NRT_TYPE       edd_dcp_nrt_oha;    /* nrt in dcp */

    EDDI_CDB_TYPE           edd_dcp_ldb_oha_edd;
#endif
#ifdef LSAS_CFG_USE_EDDP
    EDDP_CDB_NRT_TYPE       edd_dcp_nrt_oha;    /* nrt in dcp */

    EDDP_CDB_TYPE           edd_dcp_ldb_oha_edd;
#endif
#ifdef LSAS_CFG_USE_EDDS
    EDDS_CDB_NRT_TYPE		edd_dcp_nrt_oha;

    EDDS_CDB_TYPE 			edd_dcp_ldb_oha_edd;
#endif
    DCP_DETAIL_TYPE         edd_dcp_ldb_oha_dcp;
    OHA_CDB_TYPE            edd_dcp_ldb_oha_oha;

    /* APMA_PDB_LSA_PATH_EDD_NARE */
#ifdef LSAS_CFG_USE_EDDI
    EDDI_CDB_TYPE           edd_ldb_nare_edd;
    EDDI_CDB_NRT_TYPE       edd_nrt_nare;       /* nrt in nare */
#endif
#ifdef LSAS_CFG_USE_EDDP
    EDDP_CDB_TYPE           edd_ldb_nare_edd;

    EDDP_CDB_NRT_TYPE       edd_nrt_nare;
#endif
#ifdef LSAS_CFG_USE_EDDS
    EDDS_CDB_NRT_TYPE		edd_nrt_nare;

    EDDS_CDB_TYPE 			edd_ldb_nare_edd;
#endif
    NARE_CDB_TYPE           edd_ldb_nare_nare;

    /* APMA_PDB_LSA_PATH_CMPDSV */

    CM_DETAIL_TYPE          cmpdsv_ldb_cm_cm;

    /* APMA_PDB_LSA_PATH_OHASV_CM */

    OHA_CDB_TYPE            oha_ldb_cm_oha;
    CM_DETAIL_TYPE          oha_ldb_cm_cm;

    /* APMA_PDB_LSA_PATH_GSY_CM */

    GSY_DETAIL_TYPE         gsy_ldb_cm_gsy;
    CM_DETAIL_TYPE          gsy_ldb_cm_cm;

    /* APMA_PDB_LSA_PATH_EDD_CM */
#ifdef LSAS_CFG_USE_EDDI
    EDDI_CDB_TYPE           edd_ldb_cm_edd;
#endif
#ifdef LSAS_CFG_USE_EDDP
    EDDP_CDB_TYPE           edd_ldb_cm_edd;
    EDDP_CDB_PRM_TYPE       edd_prm_cm_edd;
#endif
#ifdef LSAS_CFG_USE_EDDS
    EDDS_CDB_PRM_TYPE		edd_prm_cm_edd;

    EDDS_CDB_TYPE 			edd_ldb_cm_edd;
#endif
    CM_DETAIL_TYPE          edd_ldb_cm_cm;

    /* APMA_PDB_LSA_PATH_EDD_GSY_SYS */
#ifdef LSAS_CFG_USE_EDDI
    EDDI_CDB_NRT_TYPE       edd_nrt_gsy_sys;
    EDDI_CDB_TYPE           edd_ldb_gsy_edd_sys;
#endif
#ifdef LSAS_CFG_USE_EDDP
    EDDP_CDB_NRT_TYPE       edd_nrt_gsy_sys;
    EDDP_CDB_TYPE           edd_ldb_gsy_edd_sys;
#endif
#ifdef LSAS_CFG_USE_EDDS
    EDDS_CDB_NRT_TYPE		edd_nrt_gsy_sys;

    EDDS_CDB_TYPE 			edd_ldb_gsy_edd_sys;
#endif
    GSY_DETAIL_TYPE         edd_ldb_gsy_gsy_sys;

    /* APMA_PDB_LSA_PATH_EDD_GSY_ANNO */
#ifdef LSAS_CFG_USE_EDDI
    EDDI_CDB_NRT_TYPE       edd_nrt_gsy_anno;
    EDDI_CDB_TYPE           edd_ldb_gsy_edd_anno;
#endif
#ifdef LSAS_CFG_USE_EDDP
    EDDP_CDB_NRT_TYPE       edd_nrt_gsy_anno;
    EDDP_CDB_TYPE           edd_ldb_gsy_edd_anno;
#endif
#ifdef LSAS_CFG_USE_EDDS
    EDDS_CDB_NRT_TYPE		edd_nrt_gsy_anno;

    EDDS_CDB_TYPE 			edd_ldb_gsy_edd_anno;
#endif
    GSY_DETAIL_TYPE         edd_ldb_gsy_gsy_anno;

    /* APMA_PDB_LSA_PATH_EDD_MRP */
#ifdef LSAS_CFG_USE_EDDI
    EDDI_CDB_NRT_TYPE       edd_nrt_mrp;
    EDDI_CDB_TYPE           edd_ldb_mrp_edd;
#endif
#ifdef LSAS_CFG_USE_EDDP
    EDDP_CDB_NRT_TYPE       edd_nrt_mrp;
    EDDP_CDB_TYPE           edd_ldb_mrp_edd;
#endif
#ifdef LSAS_CFG_USE_EDDS
    EDDS_CDB_NRT_TYPE		edd_nrt_mrp;

    EDDS_CDB_TYPE 			edd_ldb_mrp_edd;
#endif
    MRP_DETAIL_TYPE         edd_ldb_mrp_mrp;

    /* APMA_PDB_LSA_PATH_OHA_SYS */
    OHA_CDB_TYPE            oha_ldb_sys_oha;

#ifdef LSAS_CFG_USE_POF
    /* APMA_PDB_LSA_PATH_EDD_POF_CM */
#ifdef LSAS_CFG_USE_EDDI
    EDDI_CDB_NRT_TYPE       edd_nrt_pof;
    EDDI_CDB_TYPE           edd_ldb_pof_edd;
#endif
#ifdef LSAS_CFG_USE_EDDP
    EDDP_CDB_TYPE           edd_ldb_pof_edd;
#endif
#ifdef LSAS_CFG_USE_EDDS
    EDDS_CDB_NRT_TYPE       edd_nrt_pof;
    EDDS_CDB_TYPE           edd_ldb_pof_edd;
#endif
    POF_CDB_TYPE            pof_ldb_cm_pof;
    CM_DETAIL_TYPE          pof_ldb_cm_cm;
#endif

    /* APMA_PDB_LSA_PATH_EDD_LLDP_OHA */
#ifdef LSAS_CFG_USE_EDDI
    EDDI_CDB_NRT_TYPE       edd_lldp_nrt_oha;
    EDDI_CDB_TYPE           edd_lldp_ldb_oha_edd;
#endif
#ifdef LSAS_CFG_USE_EDDP
    EDDP_CDB_NRT_TYPE       edd_lldp_nrt_oha;
    EDDP_CDB_TYPE           edd_lldp_ldb_oha_edd;
#endif
#ifdef LSAS_CFG_USE_EDDS
    EDDS_CDB_NRT_TYPE		edd_lldp_nrt_oha;

    EDDS_CDB_TYPE 			edd_lldp_ldb_oha_edd;
#endif
    LLDP_CDB_TYPE           edd_lldp_ldb_oha_lldp;
    OHA_CDB_TYPE            edd_lldp_ldb_oha_oha;

    /* APMA_PDB_LSA_PATH_EDD_SYS */

#ifdef LSAS_CFG_USE_EDDI
    EDDI_CDB_TYPE           edd_ldb_sys_edd;
#endif
#ifdef LSAS_CFG_USE_EDDP
    EDDP_CDB_TYPE           edd_ldb_sys_edd;
#endif
#ifdef LSAS_CFG_USE_EDDS
    EDDS_CDB_TYPE 			edd_ldb_sys_edd;
#endif

    /* APMA_PDB_LSA_PATH_EDD_TCIP_ICMP */
    TCIP_DETAIL_TYPE        edd_ldb_tcip_icmp_tcip;

#ifdef LSAS_CFG_USE_EDDI
    EDDI_CDB_TYPE           edd_ldb_tcip_icmp_edd;
    EDDI_CDB_NRT_TYPE       edd_nrt_icmp;
#endif
#ifdef LSAS_CFG_USE_EDDP
    EDDP_CDB_TYPE           edd_ldb_tcip_icmp_edd;
    EDDP_CDB_NRT_TYPE       edd_nrt_icmp;
#endif
#ifdef LSAS_CFG_USE_EDDS
    EDDS_CDB_NRT_TYPE		edd_nrt_icmp;

    EDDS_CDB_TYPE 			edd_ldb_tcip_icmp_edd;
#endif
    /* APMA_PDB_LSA_PATH_EDD_TCIP_UDP */
    TCIP_DETAIL_TYPE        edd_ldb_tcip_udp_tcip;

#ifdef LSAS_CFG_USE_EDDI
    EDDI_CDB_TYPE           edd_ldb_tcip_udp_edd;
    EDDI_CDB_NRT_TYPE       edd_nrt_udp;
#endif
#ifdef LSAS_CFG_USE_EDDP
    EDDP_CDB_TYPE           edd_ldb_tcip_udp_edd;
    EDDP_CDB_NRT_TYPE       edd_nrt_udp;
#endif
#ifdef LSAS_CFG_USE_EDDS
    EDDS_CDB_NRT_TYPE		edd_nrt_udp;

    EDDS_CDB_TYPE 			edd_ldb_tcip_udp_edd;
#endif
    /* APMA_PDB_LSA_PATH_EDD_TCIP_ARP */
    TCIP_DETAIL_TYPE        edd_ldb_tcip_arp_tcip;

#ifdef LSAS_CFG_USE_EDDI
    EDDI_CDB_TYPE           edd_ldb_tcip_arp_edd;
    EDDI_CDB_NRT_TYPE       edd_nrt_arp;
#endif
#ifdef LSAS_CFG_USE_EDDP
    EDDP_CDB_TYPE           edd_ldb_tcip_arp_edd;
    EDDP_CDB_NRT_TYPE       edd_nrt_arp;
#endif
#ifdef LSAS_CFG_USE_EDDS
    EDDS_CDB_NRT_TYPE		edd_nrt_arp;

    EDDS_CDB_TYPE 			edd_ldb_tcip_arp_edd;
#endif
    /* APMA_PDB_LSA_PATH_EDD_TCIP_TCP */
    TCIP_DETAIL_TYPE        edd_ldb_tcip_tcp_tcip;
#ifdef LSAS_CFG_USE_EDDI
    EDDI_CDB_TYPE           edd_ldb_tcip_tcp_edd;
    EDDI_CDB_NRT_TYPE       edd_nrt_tcp;
#endif
#ifdef LSAS_CFG_USE_EDDP
    EDDP_CDB_TYPE           edd_ldb_tcip_tcp_edd;
    EDDP_CDB_NRT_TYPE       edd_nrt_tcp;
#endif
#ifdef LSAS_CFG_USE_EDDS
    EDDS_CDB_NRT_TYPE		edd_nrt_tcp;

    EDDS_CDB_TYPE 			edd_ldb_tcip_tcp_edd;
#endif

}   lsas_path_detail_struct;

/* lsa detail_ptr and system_ptr matrix for all components and paths --------*/

typedef struct
{
    LSAS_SYSTEM_PTR system_ptr;
    LSAS_DETAIL_PTR detail_ptr;

    LSA_BOOL         used;

}   LSAS_PATH_INFO_T;

typedef LSAS_PATH_INFO_T * LSAS_PATH_INFO_T_PTR;

LSAS_PATH_EXT_ATTR LSAS_PATH_INFO_T lsas_path_layer_matrix[ LSAS_PATH_NUM][ LSAS_PATH_LAYER_NUM];


#endif /* LSAS_PATH_H_ */

/*** end of file *************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
