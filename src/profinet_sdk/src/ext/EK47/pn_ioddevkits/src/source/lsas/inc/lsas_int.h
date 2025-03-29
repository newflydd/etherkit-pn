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
/*  F i l e               &F: lsas_int.h                                :F&  */
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
 * @file    lsas_int.h
 * @brief   brief description
 * @author  cn3dit09
 * @version
 * @date    24.02.2012
 *
 * long description
 *
 */

/*****************************************************************************/

#ifndef LSAS_INT_H_
#define LSAS_INT_H_

typedef LSAS_ACP_RQB_TYPE   * LSAS_ACP_RQB_PTR_TYPE;
typedef LSAS_CLRPC_RQB_TYPE * LSAS_CLRPC_RQB_PTR_TYPE;
typedef LSAS_CM_RQB_TYPE    * LSAS_CM_RQB_PTR_TYPE;
typedef LSAS_DCP_RQB_TYPE   * LSAS_DCP_RQB_PTR_TYPE;
typedef LSAS_EDD_RQB_TYPE   * LSAS_EDD_RQB_PTR_TYPE;
typedef LSAS_GSY_RQB_TYPE   * LSAS_GSY_RQB_PTR_TYPE;
typedef LSAS_LLDP_RQB_TYPE  * LSAS_LLDP_RQB_PTR_TYPE;
typedef LSAS_MRP_RQB_TYPE   * LSAS_MRP_RQB_PTR_TYPE;
typedef LSAS_NARE_RQB_TYPE  * LSAS_NARE_RQB_PTR_TYPE;
typedef LSAS_OHA_RQB_TYPE   * LSAS_OHA_RQB_PTR_TYPE;
#ifdef LSAS_CFG_USE_POF
typedef LSAS_POF_RQB_TYPE   * LSAS_POF_RQB_PTR_TYPE;
#endif
typedef LSAS_SOCK_RQB_TYPE  * LSAS_SOCK_RQB_PTR_TYPE;
typedef LSAS_TCIP_RQB_TYPE  * LSAS_TCIP_RQB_PTR_TYPE;
#ifdef LSAS_CFG_USE_STCP
typedef LSAS_STCP_RQB_TYPE  * LSAS_STCP_RQB_PTR_TYPE;
typedef LSAS_S7PN_RQB_TYPE  * LSAS_S7PN_RQB_PTR_TYPE;
#endif /* LSAS_CFG_USE_STCP */



/**********************************************************************************/
/* defines for the edd nrt receive load limitation                                */

/**********************************************************************************
 * how many dmacw ressources are provided for disposing nrt frames into the sdram *
 **********************************************************************************/
#define LSAS_EDD_NRT_RCV_NUMBER_OF_DMACW_BUFFERS 90 /* ANHE_SII ->90*/

/********************************************************************************
 * how many received frames shall be evaluated before the load limitation fires *
 ********************************************************************************/
#define LSAS_EDD_NRT_RCV_MAX_FRAME_TO_LOOK_UP    90 /* ANHE_SII ->90*/

/*********************************************************************
 * how many frames of the LSAS_EDD_NRT_RCV_MAX_FRAME_TO_LOOK_UP ones *
 * shall be sent to the application, e.g. TCP-Stack, DCP, ...        *
 * before the load limitation fires                                  *
 *********************************************************************/
#define LSAS_EDD_NRT_RCV_MAX_FRAME_SEND_TO_USER  90 /* ANHE_SII ->90*/


#define LSAS_EDD_NRT_SND_MAX_FRAME_SEND_TO_USER  60 /* ANHE_SII ->40 oder 60*/
#define LSAS_EDD_NRT_SND_MAX_FRAME_TO_LOOK_UP    60 /* ANHE_SII ->60*/
#define LSAS_EDD_NRT_SND_NUMBER_OF_DMACW_BUFFERS 60 /* ANHE_SII ->60*/
#include "sys_pck1.h"



typedef struct
{
#ifdef LSAS_CFG_USE_EDDI
    /* device open */
    EDDI_RQB_DDB_INI_TYPE       dpb_pparam;
    EDDI_DPB_TYPE               dpb;

    /* basic ini */
    EDDI_RQB_COMP_INI_TYPE      comp;

    /* device setup */
    EDDI_RQB_DDB_SETUP_TYPE     dsb_pparam;
    EDDI_DSB_TYPE               dsb;

    EDDI_GLOBAL_INI_TYPE        gis;
#endif /* LSAS_CFG_USE_EDDI */

#ifdef LSAS_CFG_USE_EDDP
    /* device open */
    EDDP_RQB_DEV_OPEN_TYPE      dpb_pparam;               /* RQB-param of EDDP device open - contains a pointer to dpb */
    EDDP_DPB_TYPE               dpb;                    /* contains pointer to ipb */


    /* device setup */
    EDDP_RQB_DEV_SETUP_TYPE     dsb_pparam;              /* RQB-param of EDDP device setup - contains a pointer to dsb */
    EDDP_DSB_TYPE               dsb;                    /* is referenced by dev_setup */
	EDD_SWICHAR_TYPE			iniSWICHAR;				// Switching Characteristics

#endif
    /* device open */

#ifdef LSAS_CFG_USE_EDDS
    /* device open */
    EDDS_RQB_DDB_INI_TYPE       open_param;      // Device Description Block
    EDDS_DPB_TYPE               open_dpb;        // Device Parameter Block
    #ifdef LSAS_EDDS_ASIC_TI
    TI_LL_PARAMETER_TYPE        tiLLParameter;
    #endif

    EDDS_RQB_DDB_SETUP_TYPE     setup_param;     // Device Setup Param Block
    EDDS_DSB_TYPE               setup_dsb;       // Device Setup DSB Block
#endif

} LSAS_EDD_OPEN_PARA;
#include "sys_unpck.h"
LSA_VOID LSAS_LOCAL_FCT_ATTR lsas_fatal_error(
    LSA_UINT16             ModuleID,
    LSA_UINT16             Line,
    LSAS_FATAL_ERROR_TYPE  Error);


#define LSAS_FATAL(Error)    \
{                            \
lsas_fatal_error(                            \
    LSAS_MODULE_ID, (LSA_UINT16) __LINE__,    \
    (LSAS_FATAL_ERROR_TYPE) Error);            \
}

void                lsas_com_init_path_layer_matrix(void);

#ifndef LSAS_ENTER
LSA_VOID  LSAS_SYSTEM_OUT_FCT_ATTR  LSAS_ENTER(LSA_VOID);
#endif

#ifndef LSAS_EXIT
LSA_VOID  LSAS_SYSTEM_OUT_FCT_ATTR  LSAS_EXIT(LSA_VOID);
#endif

#ifndef LSAS_WAIT_10NS
LSA_VOID LSAS_WAIT_10NS(unsigned int ticks_10ns);
#endif

typedef enum LSAS_OPEN_SM_STATE_E
{
    LSAS_OPEN_SM_CLOSED            ,
    LSAS_OPEN_SM_ALLOC_MEM         ,
    LSAS_OPEN_SM_EDD_SYS           ,
    LSAS_OPEN_SM_EDD_ACP           ,
    LSAS_OPEN_SM_EDD_TCIP_ARP      ,
    LSAS_OPEN_SM_EDD_TCIP_ICMP     ,
    LSAS_OPEN_SM_EDD_TCIP_UDP      ,
    LSAS_OPEN_SM_EDD_TCIP_TCP      ,
    LSAS_OPEN_SM_SOCK_SYS          ,
    LSAS_OPEN_SM_EDD_MRP           ,
    LSAS_OPEN_SM_SOCK_OHA          ,
    LSAS_OPEN_SM_EDD_OHA           ,
    LSAS_OPEN_SM_LLDP_OHA          ,
    LSAS_OPEN_SM_MRP_OHA           ,
    LSAS_OPEN_SM_SOCK_CLRPC        ,
    LSAS_OPEN_SM_EDD_DCP_OHA       ,
    LSAS_OPEN_SM_DCP_NARE          ,
    LSAS_OPEN_SM_EDD_NARE          ,
    LSAS_OPEN_SM_NARE_OHA          ,
    LSAS_OPEN_SM_EDD_GSY_SYS       ,
    LSAS_OPEN_SM_EDD_GSY_ANNO      ,
    LSAS_OPEN_SM_OHA_SYS           ,
    LSAS_OPEN_SM_EDD_CM            ,
    LSAS_OPEN_SM_GSY_CM            ,
    LSAS_OPEN_SM_NARE_CM           ,
    LSAS_OPEN_SM_OHASV_CM          ,
    LSAS_OPEN_SM_MRP_CM            ,
    LSAS_OPEN_SM_EDD_POF_CM        ,
    LSAS_OPEN_SM_SOCK_STCP_S7PN    ,
    LSAS_OPEN_SM_FREE_MEM          ,
    LSAS_OPEN_SM_DONE              ,

} LSAS_OPEN_SM_STATE_E_T;


/************************************************************/
LSA_VOID  LSAS_SYSTEM_OUT_FCT_ATTR  LSAS_ENTER(    LSA_VOID);
LSA_VOID  LSAS_SYSTEM_OUT_FCT_ATTR  LSAS_EXIT(    LSA_VOID);
LSA_VOID  LSAS_SYSTEM_OUT_FCT_ATTR  LSAS_ALLOC_LOCAL_MEM(    LSAS_LOCAL_MEM_PTR_TYPE  LSAS_LOCAL_MEM_ATTR *  local_mem_ptr_ptr,    LSA_UINT32                                      length);
LSA_VOID  LSAS_SYSTEM_OUT_FCT_ATTR  LSAS_FREE_LOCAL_MEM(    LSA_UINT16                 LSAS_LOCAL_MEM_ATTR *  ret_val_ptr,    LSAS_LOCAL_MEM_PTR_TYPE                           local_mem_ptr);
LSA_VOID  LSAS_EDD_LOWER_OUT_FCT_ATTR LSAS_EDD_REQUEST_OPEN_CHANNEL (LSAS_EDD_RQB_PTR_TYPE  lower_rqb_ptr);
LSA_VOID  LSAS_ACP_LOWER_OUT_FCT_ATTR LSAS_ACP_REQUEST_OPEN_CHANNEL (LSAS_ACP_RQB_PTR_TYPE  lower_rqb_ptr);
LSA_VOID  LSAS_TCIP_LOWER_OUT_FCT_ATTR LSAS_TCIP_REQUEST_OPEN_CHANNEL (LSAS_TCIP_RQB_PTR_TYPE  lower_rqb_ptr);
#ifdef LSAS_CFG_USE_STCP
LSA_VOID  LSAS_STCP_LOWER_OUT_FCT_ATTR LSAS_STCP_REQUEST_OPEN_CHANNEL (LSAS_STCP_RQB_PTR_TYPE  lower_rqb_ptr);
LSA_VOID  LSAS_S7PN_LOWER_OUT_FCT_ATTR LSAS_S7PN_REQUEST_OPEN_CHANNEL (LSAS_S7PN_RQB_PTR_TYPE  lower_rqb_ptr);
#endif /* LSAS_CFG_USE_STCP */
LSA_VOID  LSAS_SOCK_LOWER_OUT_FCT_ATTR LSAS_SOCK_REQUEST_OPEN_CHANNEL (LSAS_SOCK_RQB_PTR_TYPE  lower_rqb_ptr);
LSA_VOID  LSAS_MRP_LOWER_OUT_FCT_ATTR LSAS_MRP_REQUEST_OPEN_CHANNEL (LSAS_MRP_RQB_PTR_TYPE  lower_rqb_ptr);
LSA_VOID  LSAS_OHA_LOWER_OUT_FCT_ATTR LSAS_OHA_REQUEST_OPEN_CHANNEL (LSAS_OHA_RQB_PTR_TYPE  lower_rqb_ptr);
LSA_VOID  LSAS_CLRPC_LOWER_OUT_FCT_ATTR LSAS_CLRPC_REQUEST_OPEN_CHANNEL (LSAS_CLRPC_RQB_PTR_TYPE  lower_rqb_ptr);
LSA_VOID  LSAS_NARE_LOWER_OUT_FCT_ATTR LSAS_NARE_REQUEST_OPEN_CHANNEL (LSAS_NARE_RQB_PTR_TYPE  lower_rqb_ptr);
LSA_VOID  LSAS_GSY_LOWER_OUT_FCT_ATTR LSAS_GSY_REQUEST_OPEN_CHANNEL (LSAS_GSY_RQB_PTR_TYPE  lower_rqb_ptr);
LSA_VOID  LSAS_CM_LOWER_OUT_FCT_ATTR LSAS_CM_REQUEST_OPEN_CHANNEL (LSAS_CM_RQB_PTR_TYPE  lower_rqb_ptr);
LSA_VOID  LSAS_DCP_LOWER_OUT_FCT_ATTR LSAS_DCP_REQUEST_OPEN_CHANNEL (LSAS_DCP_RQB_PTR_TYPE  lower_rqb_ptr);
LSA_VOID LSAS_ACP_REQUEST(LSAS_ACP_RQB_PTR_TYPE rqb_ptr);
LSA_VOID LSAS_CLRPC_REQUEST(LSAS_CLRPC_RQB_PTR_TYPE rqb_ptr);
LSA_VOID LSAS_CM_REQUEST(LSAS_CM_RQB_PTR_TYPE rqb_ptr);
LSA_VOID LSAS_DCP_REQUEST(LSAS_DCP_RQB_PTR_TYPE rqb_ptr);
LSA_VOID LSAS_EDD_REQUEST(LSAS_EDD_RQB_PTR_TYPE rqb_ptr);
LSA_VOID LSAS_GSY_REQUEST(LSAS_GSY_RQB_PTR_TYPE rqb_ptr);
LSA_VOID LSAS_LLDP_REQUEST(LSAS_LLDP_RQB_PTR_TYPE rqb_ptr);
LSA_VOID LSAS_MRP_REQUEST(LSAS_MRP_RQB_PTR_TYPE rqb_ptr);
LSA_VOID LSAS_NARE_REQUEST(LSAS_NARE_RQB_PTR_TYPE rqb_ptr);
LSA_VOID LSAS_OHA_REQUEST(LSAS_OHA_RQB_PTR_TYPE rqb_ptr);
#ifdef LSAS_CFG_USE_POF
LSA_VOID LSAS_POF_REQUEST(LSAS_POF_RQB_PTR_TYPE rqb_ptr);
#endif

LSA_VOID LSAS_SOCK_REQUEST(LSAS_SOCK_RQB_PTR_TYPE rqb_ptr);

LSA_VOID  LSAS_SYSTEM_OUT_FCT_ATTR  LSAS_ALLOC_LOCAL_MEM(
    LSAS_LOCAL_MEM_PTR_TYPE  LSAS_LOCAL_MEM_ATTR *  local_mem_ptr_ptr,
    LSA_UINT32                                      length
);

/************************************************************/

void    lsas_edd_req_open(void);
#ifdef LSAS_CFG_USE_EDDI
void    lsas_edd_system_setup_gis(EDDI_UPPER_GLOBAL_INI_PTR_TYPE pGIS, LSA_UINT32 ProductID);
void    lsas_edd_system_dev_open    (EDD_RQB_TYPE * rqb_ptr, EDDI_DPB_TYPE * dpb_ptr);
void    lsas_edd_system_basic_ini   (EDD_RQB_TYPE  * rqb_ptr, EDDI_UPPER_CMP_INI_PTR_TYPE comp_ptr);
void    lsas_edd_system_device_setup(EDD_RQB_TYPE * rqb_ptr, EDDI_DSB_TYPE * dsb_ptr);

#if defined (EDDI_CFG_PHY_VARIABLE_PORTSWAPPING_NSC)
LSA_BOOL   lsas_eddi_get_fiber_optic_support( void );
LSA_UINT16 lsas_eddi_get_phy_adr_from_hw_port( LSA_UINT32 hw_port );
#endif
#endif

LSA_VOID lsas_eddi_phy_led_ctrl_normal( LSA_UINT32 HwPortIndex );
LSA_VOID lsas_eddi_phy_led_ctrl_force( LSA_UINT32 HwPortIndex );
LSA_VOID lsas_eddi_phy_set_link_led_value( LSA_UINT32 HwPortIndex, LSA_UINT32 value );

#ifdef LSAS_CFG_USE_EDDP
LSA_VOID lsas_eddp_system_dev_open(EDD_RQB_TYPE * rqb_ptr, LSAS_EDD_OPEN_PARA * eddp_param_ptr);
LSA_VOID lsas_eddp_system_device_setup(EDD_RQB_TYPE * rqb_ptr, LSAS_EDD_OPEN_PARA * eddp_param_ptr);
LSA_VOID lsas_cfg_eddp_system_req_check(LSA_VOID);
#endif
void lsas_oha_db_init(LSAS_RQB_PTR_TYPE rqb_ptr);

/**************** cbf **********************/

void    lsas_cbf_sys_edd_request_lower_done                             (EDD_UPPER_RQB_PTR_TYPE pRQB);
void    lsas_cbf_sys_acp_request_lower_done              (ACP_UPPER_RQB_PTR_TYPE rqb_ptr);
void    lsas_cbf_sys_tcip_request_lower_done                (TCIP_UPPER_RQB_PTR_TYPE rqb_ptr);
void    lsas_cbf_sys_clrpc_request_lower_done            (CLRPC_UPPER_RQB_PTR_TYPE rqb_ptr);

#ifdef LSAS_CFG_USE_STCP
void   lsas_cbf_sys_s7pn_request_done   (L4_UPPER_RQB_PTR_TYPE rqb_ptr);
void   lsas_stcp_timeout_wrap           (LSA_UINT16 timer_id, LSA_USER_ID_TYPE user_id);
#endif /* LSAS_CFG_USE_STCP */

void    lsas_cbf_sys_oha_request_lower_done              (OHA_UPPER_RQB_PTR_TYPE rqb_ptr);
void    lsas_cbf_sys_mrp_request_lower_done              (MRP_UPPER_RQB_PTR_TYPE rqb_ptr);
void    lsas_cbf_sys_nare_request_lower_done             (NARE_UPPER_RQB_PTR_TYPE rqb_ptr);
void    lsas_cbf_sys_cm_request_lower_done               (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void    lsas_cbf_sys_oha_sys_request_lower_done          (OHA_UPPER_RQB_PTR_TYPE rqb_ptr);
void    lsas_cbf_sys_gsy_request_lower_done                  (GSY_UPPER_RQB_PTR_TYPE rqb_ptr);

void    lsas_com_open_pnio_stack_sm (void);


#ifndef LSAS_HANDLE_OHA_REMA_REQUESTS
LSA_VOID    LSAS_HANDLE_OHA_REMA_REQUESTS   (OHA_UPPER_RQB_PTR_TYPE rqb_ptr);
#endif

#ifndef LSAS_CFG_SYSTEM_RAM_INIT
LSA_VOID    LSAS_CFG_SYSTEM_RAM_INIT(LSA_VOID);
#endif

extern  void    lsas_acp_to_cm_cbf   (CM_UPPER_RQB_PTR_TYPE void_ptr);
extern  void    lsas_clrpc_to_cm_cbf (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
extern  void    lsas_cm_to_appl_cbf  (CM_UPPER_RQB_PTR_TYPE rqb_ptr);
extern  void    lsas_dcp_to_nare_cbf                            (DCP_UPPER_RQB_PTR_TYPE rqb_ptr);
extern  void    lsas_dcp_to_oha_cbf                             (DCP_UPPER_RQB_PTR_TYPE rqb_ptr);
extern  void    lsas_edd_to_acp_cbf      (ACP_LOWER_RQB_PTR_TYPE rqb_ptr);
extern  void    lsas_edd_to_cm_cbf   (CM_EDD_LOWER_RQB_PTR_TYPE rqb_ptr);
extern  void    lsas_edd_to_dcp_cbf       (DCP_ETH_RQB_PTR_TYPE rqb_ptr);
extern  void    lsas_edd_to_gsy_cbf                           (GSY_LOWER_RQB_PTR_TYPE rqb_ptr);
extern  void    lsas_edd_to_lldp_cbf                          (LLDP_EDD_LOWER_RQB_PTR_TYPE rqb_ptr);
extern  void    lsas_edd_to_mrp_cbf                             (EDD_UPPER_RQB_PTR_TYPE rqb_ptr);
extern  void    lsas_edd_to_nare_cbf                            (EDD_UPPER_RQB_PTR_TYPE rqb_ptr);
extern  void    lsas_edd_to_oha_cbf                             (EDD_UPPER_RQB_PTR_TYPE rqb_ptr);
#ifdef LSAS_CFG_USE_POF
extern  void    lsas_edd_to_pof_cbf                           (POF_EDD_LOWER_RQB_PTR_TYPE rqb_ptr);
#endif
#ifndef LSAS_CFG_USE_EXTERNAL_TCPIP_STACK
extern  void    lsas_edd_to_tcip_cbf                            (EDD_UPPER_RQB_PTR_TYPE rqb_ptr);
#endif
extern  void    lsas_gsy_to_cm_cbf   (CM_GSY_LOWER_RQB_PTR_TYPE rqb_ptr);
extern  void    lsas_lldp_to_oha_cbf                            (LLDP_UPPER_RQB_PTR_TYPE rqb_ptr);
#if CM_CFG_USE_MRP
extern  void    lsas_mrp_to_cm_cbf   (CM_MRP_LOWER_RQB_PTR_TYPE rqb_ptr);
#endif
#ifndef OHA_CFG_NO_MRP
extern  void    lsas_mrp_to_oha_cbf                             (OHA_MRP_LOWER_RQB_PTR_TYPE rqb_ptr);
#endif
extern  void    lsas_nare_to_oha_cbf                            (NARE_UPPER_RQB_PTR_TYPE rqb_ptr);
extern  void    lsas_oha_to_cm_cbf   (CM_UPPER_RQB_PTR_TYPE void_ptr);
extern  void    lsas_nare_to_cm_cbf  (NARE_UPPER_RQB_PTR_TYPE void_ptr);
#ifdef LSAS_CFG_USE_POF
extern  void    lsas_pof_to_cm_cbf   (CM_POF_LOWER_RQB_PTR_TYPE rqb_ptr);
#endif
extern  void    lsas_sock_to_clrpc_cbf    (CLRPC_LOWER_RQB_PTR_TYPE rqb_ptr);
extern  void    lsas_sock_to_oha_cbf      (SOCK_UPPER_RQB_PTR_TYPE rqb_ptr);

#ifdef LSAS_CFG_USE_STCP
extern  void    lsas_sock_to_stcp_cbf                           (SOCK_UPPER_RQB_PTR_TYPE rqb_ptr);
/* moved to com */ // extern  void    lsas_stcp_to_s7pn_cbf                           (L4_UPPER_RQB_PTR_TYPE rqb_ptr);
#endif /* LSAS_CFG_USE_STCP */

//=================================================================================================
// EDDS lower level interface
//=================================================================================================

#ifdef LSAS_CFG_USE_EDDS
LSA_RESULT lsas_edds_ll_open (LSA_VOID_PTR_TYPE pLLManagement,
                              EDDS_HANDLE                      hDDB,
                              EDDS_UPPER_DPB_PTR_TYPE          pDPB,
							  LSA_UINT32				       TraceIdx,
							  EDDS_LOCAL_EDDS_LL_CAPS_PTR_TYPE pCaps);

LSA_RESULT lsas_edds_ll_setup (LSA_VOID_PTR_TYPE pLLManagement,
                               EDDS_UPPER_DSB_PTR_TYPE         pDSB,
                               EDDS_LOCAL_LL_HW_PARAM_PTR_TYPE pStartupLinkParam );

LSA_RESULT lsas_edds_ll_shutdown (LSA_VOID_PTR_TYPE pLLManagement);

LSA_RESULT lsas_edds_ll_close (LSA_VOID_PTR_TYPE pLLManagement);

LSA_RESULT lsas_edds_ll_send (LSA_VOID_PTR_TYPE pLLManagement,
                              EDD_UPPER_MEM_PTR_TYPE           pTxBuf,
                              LSA_UINT32                       Datlen,
                              LSA_UINT32                       PortID);

LSA_RESULT lsas_edds_ll_send_sts (LSA_VOID_PTR_TYPE pLLManagement);

LSA_VOID lsas_edds_ll_send_trigger (LSA_VOID_PTR_TYPE pLLManagement);

LSA_VOID   lsas_edds_ll_recurringTask (LSA_VOID_PTR_TYPE pLLManagement,
                                        LSA_BOOL                               hwTimeSlicing);

LSA_RESULT lsas_edds_ll_recv (LSA_VOID_PTR_TYPE pLLManagement,
                              EDD_UPPER_MEM_PTR_TYPE  EDDS_LOCAL_MEM_ATTR     *pBufferAddr,
                              EDDS_LOCAL_MEM_U32_PTR_TYPE                      pLength,
                              EDDS_LOCAL_MEM_U32_PTR_TYPE                      pPortID);

LSA_RESULT lsas_edds_ll_recv_provide (LSA_VOID_PTR_TYPE pLLManagement,
                                      EDD_UPPER_MEM_PTR_TYPE                   pBufferAddr);

LSA_VOID   lsas_edds_ll_recv_trigger (LSA_VOID_PTR_TYPE pLLManagement);

LSA_RESULT lsas_edds_ll_get_link_state (LSA_VOID_PTR_TYPE pLLManagement,
                                        LSA_UINT32                             PortID,
                                        EDD_UPPER_GET_LINK_STATUS_PTR_TYPE     pLinkStat,
                                        EDDS_LOCAL_MEM_U16_PTR_TYPE            pMAUType,
                                        EDDS_LOCAL_MEM_U8_PTR_TYPE             pMediaType,
										EDDS_LOCAL_MEM_U8_PTR_TYPE             pOpticalTransType,
                                        EDDS_LOCAL_MEM_U32_PTR_TYPE            pPortStatus,
                                        EDDS_LOCAL_MEM_U32_PTR_TYPE            pAutonegCapAdvertised,
                                        EDDS_LOCAL_MEM_U8_PTR_TYPE             pLinkSpeedModeConfigured);

LSA_RESULT lsas_edds_ll_get_stats (LSA_VOID_PTR_TYPE pLLManagement,
                                    LSA_UINT32                                 PortID,
                                    EDDS_LOCAL_STATISTICS_PTR_TYPE             pStats);

LSA_RESULT lsas_edds_ll_mc_enable (LSA_VOID_PTR_TYPE pLLManagement,
                                    EDD_MAC_ADR_TYPE EDD_UPPER_MEM_ATTR       *pMCAddr);

LSA_RESULT lsas_edds_ll_mc_disable (LSA_VOID_PTR_TYPE pLLManagement,
                                    LSA_BOOL                                   DisableAll,
                                    EDD_MAC_ADR_TYPE EDD_UPPER_MEM_ATTR       *pMCAddr);

LSA_RESULT lsas_edds_ll_set_link_state (LSA_VOID_PTR_TYPE pLLManagement,
                                        LSA_UINT32                             PortID,
                                        LSA_UINT8    * const                   pLinkStat,
                                        LSA_UINT8    * const                   pEddPhyPower);

LSA_VOID   lsas_edds_ll_led_backup_mode (LSA_VOID_PTR_TYPE pLLManagement);

LSA_VOID   lsas_edds_ll_led_restore_mode (LSA_VOID_PTR_TYPE pLLManagement);

LSA_VOID   lsas_edds_ll_led_set_mode (LSA_VOID_PTR_TYPE pLLManagement,
                                      LSA_BOOL              On);

LSA_RESULT lsas_edds_ll_change_port( LSA_VOID_PTR_TYPE pLLManagement,
                                      LSA_UINT16            PortID,
                                      LSA_UINT8             IsPulled);

#ifdef LLIF_CFG_SWITCH_SUPPORT
LSA_RESULT lsas_edds_ll_switch_set_port_state (LSA_VOID_PTR_TYPE pLLManagement,
                                               EDD_UPPER_SWI_SET_PORT_STATE_PTR_TYPE   pPortStates);

LSA_RESULT lsas_edds_ll_switch_multicast_fwd_ctrl (LSA_VOID_PTR_TYPE pLLManagement,
                                                    EDD_UPPER_PORTID_MODE_PTR_TYPE pPortIDModeArray,
                                                    LSA_UINT16                     PortIDModeCnt,
                                                    LSA_UINT16                     MACAddrPrio,
                                                    LSA_UINT16                     MACAddrGroup,
                                                    LSA_UINT32                     MACAddrLow);

LSA_RESULT lsas_edds_ll_switch_flush_filtering_db (LSA_VOID_PTR_TYPE pLLManagement);

LSA_RESULT lsas_edds_ll_switch_set_port_redundant (LSA_VOID_PTR_TYPE pLLManagement,
                                                   EDDS_UPPER_SWI_SET_PORT_REDUNDANT_PTR_TYPE  pPortRed);

LSA_RESULT lsas_edds_ll_switch_set_bridge_mode (LSA_VOID_PTR_TYPE pLLManagement,
                                                LSA_UINT32                          Mode);

#ifdef LLIF_CFG_SWITCH_DROP_FRAME_SUPPORT
LSA_RESULT  lsas_edds_ll_switch_drop_cnt (LSA_VOID_PTR_TYPE          pDDB,
                                          EDD_UPPER_MEM_U32_PTR_TYPE pDropCnt);
#endif /* LLIF_CFG_SWITCH_DROP_FRAME_SUPPORT */

#endif /* LLIF_CFG_SWITCH_SUPPORT */

#ifdef LLIF_CFG_USE_LL_ARP_FILTER
LSA_RESULT lsas_edds_ll_set_arp_filter (LSA_VOID_PTR_TYPE pLLManagement,
		LSA_UINT8                           Mode,
		    LSA_UINT32                          UserIndex,
		    EDD_IP_ADR_TYPE                     IPAddr);
                                       // EDDS_ARP_FRAME_FILTER_PTR pArpFilter);
#endif /* LLIF_CFG_USE_LL_ARP_FILTER */


#define EDDS_LLIF_OPEN				lsas_edds_ll_open
#define EDDS_LLIF_SETUP				lsas_edds_ll_setup
#define EDDS_LLIF_SHUTDOWN 			lsas_edds_ll_shutdown
#define EDDS_LLIF_CLOSE				lsas_edds_ll_close
#define EDDS_LLIF_SEND				lsas_edds_ll_send
#define EDDS_LLIF_SEND_STS			lsas_edds_ll_send_sts
#define EDDS_LLIF_SEND_TRIGGER		lsas_edds_ll_send_trigger
#define EDDS_LLIF_RECURRING_TASK	lsas_edds_ll_recurringTask
#define EDDS_LLIF_RECV				lsas_edds_ll_recv
#define EDDS_LLIF_RECV_PROVIDE		lsas_edds_ll_recv_provide
#define EDDS_LLIF_RECV_TRIGGER 		lsas_edds_ll_recv_trigger
#define EDDS_LLIF_GET_LINK_STATE	lsas_edds_ll_get_link_state
#define EDDS_LLIF_GET_STATS			lsas_edds_ll_get_stats
#define EDDS_LLIF_MC_ENABLE			lsas_edds_ll_mc_enable
#define EDDS_LLIF_MC_DISABLE		lsas_edds_ll_mc_disable
#define EDDS_LLIF_SET_LINK_STATE	lsas_edds_ll_set_link_state
#define EDDS_LLIF_LED_BACKUP_MODE	lsas_edds_ll_led_backup_mode
#define EDDS_LLIF_LED_RESTORE_MODE	lsas_edds_ll_led_restore_mode
#define EDDS_LLIF_LED_SET_MODE		lsas_edds_ll_led_set_mode
#define EDDS_LLIF_CHANGE_PORT		lsas_edds_ll_change_port

#ifdef LLIF_CFG_SWITCH_SUPPORT
#define EDDS_LLIF_SWITCH_SET_PORT_STATE		lsas_edds_ll_switch_set_port_state
#define EDDS_LLIF_SWITCH_MULTICAST_FWD_CTRL	lsas_edds_ll_switch_multicast_fwd_ctrl
#define EDDS_LLIF_SWITCH_FLUSH_FILTERING_DB	lsas_edds_ll_switch_flush_filtering_db
#define EDDS_LLIF_SWITCH_SET_PORT_REDUNDANT	lsas_edds_ll_switch_set_port_redundant
#define EDDS_LLIF_SWITCH_SET_BRIDGE_MODE	lsas_edds_ll_switch_set_bridge_mode

#ifdef LLIF_CFG_SWITCH_DROP_FRAME_SUPPORT
#define  EDDS_LLIF_SWITCH_DROP_CNT			lsas_edds_ll_switch_drop_cnt
#endif /* LLIF_CFG_SWITCH_DROP_FRAME_SUPPORT */
#endif /* LLIF_CFG_SWITCH_SUPPORT */

#ifdef LLIF_CFG_USE_LL_ARP_FILTER
#define EDDS_LLIF_SET_ARP_FILTER			lsas_edds_ll_set_arp_filter
#endif /* LLIF_CFG_USE_LL_ARP_FILTER */

#define	EDDS_LLIF_REINIT_TX					lsas_edds_ll_reinit_tx
#define EDDS_LLIF_GET_LINK_STATE_SETUP		lsas_edds_ll_get_link_state_setup
#define EDDS_LLIF_CHECK_MAUTYPE				lsas_edds_ll_check_mautype
#define EDDS_LLIF_GET_CAPS					lsas_edds_ll_get_caps
#define EDDS_LLIF_GET_MAC_ADDR				lsas_edds_ll_get_mac_addr
#define EDDS_LLIF_CHECK_LINK_STATE			lsas_edds_ll_check_link_state
#ifdef LLIF_CFG_PORT_CNT_DYNAMIC
#define EDDS_LLIF_GET_PORT_CNT				lsas_edds_ll_get_port_cnt
#endif

#endif //LSAS_CFG_USE_EDDS
//=================================================================================================



#endif /* LSAS_INT_H_ */

/*** end of file *************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
