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
/*  F i l e               &F: lsas_dat.h                                :F&  */
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
 * @file    lsas_dat.h
 * @brief   brief description
 * @author  cn3dit09
 * @version
 * @date    24.02.2012
 *
 * long description
 *
 */

/*****************************************************************************/

#ifndef LSAS_DAT_H_
#define LSAS_DAT_H_

#ifndef LSAS_DAT_EXT_ATTR
    #define LSAS_DAT_EXT_ATTR extern
#else
    #define LSAS_DAT_EXT_ATTR_DECLARE
#endif

#ifdef LSAS_CFG_USE_EDDI
  #ifdef LSAS_CFG_EDDI_ERTEC400
		// when using IOM_KRAM_PROP_USE_CONS_EX_BUFFERS the unkram_buffer has to be doubled (next and user buffers)
        LSAS_DAT_EXT_ATTR Unsigned32    lsas_process_shadow_buffer[2*((LSAS_PROCESS_OUTPUT_SIZE + 3) / 4)];
  #elif defined LSAS_CFG_EDDI_ERTEC200
        LSAS_DAT_EXT_ATTR Unsigned32    lsas_process_shadow_buffer[1*((LSAS_PROCESS_OUTPUT_SIZE + 3) / 4)];
  #elif defined (IM_HW_SOC)
        LSAS_DAT_EXT_ATTR Unsigned32    lsas_cons_buffer[((IM_MAX_ARS_RT*1536) / 4)+1]; /* every consumer cr has its buffer additionally 1*uint32 as mailbox element */
  #endif
#endif

#include "sys_pck1.h"
typedef struct LSAS_DATA_S
{
    LSAS_EDD_HANDLE             edd_hddb;
    LSA_UINT32                  edd_hxdb_set;

    LSAS_RQB_TYPE       		tskma_rqb;

#ifdef LSAS_CFG_USE_EDDI
    struct
    {
        EDDI_GSHAREDMEM_TYPE    gshared_mem;
        #if defined (EDDI_CFG_REV7)
        Boolean                 soc_i2c_init_done;
        #endif
    } eddi;
#endif

#ifdef LSAS_CFG_USE_EDDP
    EDDP_USER_CMD_HANDLE_TYPE       eddp_cmd_iface_handle;
    struct
    {
        LSAS_EDDP_SM_STATE_E_T      sm;
        LSAS_EDD_OPEN_PARA         *eddp_param_ptr;
        EDD_RQB_TYPE               *eddp_sys_rqb_ptr;
        EDDP_GSHAREDMEM_TYPE        gshared_mem;
        EDDP_USERCMDSHAREDMEM_TYPE  usrcmdshared_mem;
    } eddp;
#endif

#ifdef LSAS_CFG_USE_EDDS
    struct
    {
        LSAS_EDDS_SM_STATE_E_T      sm;
        LSAS_EDD_OPEN_PARA         *param_ptr;
        EDD_RQB_TYPE               *sys_rqb_ptr;
        #ifdef LSAS_EDDS_ASIC_TI
        TI_LL_HANDLE_TYPE          hLL;
		#else
        LSA_VOID_PTR_TYPE          hLL;
        #endif
        EDDS_LL_TABLE_TYPE   	   ll_func_table; // Packet32LLFuncTable
        EDDS_IOBUFFER_HANDLE_TYPE  h_io_buffer_management;
        void* 					   interrupt_tx_cbf;
        LSA_UINT16                 timer_id;
        LSAS_EDD_RQB_TYPE          scheduler_trigger_rqb;
        LSA_BOOL                   scheduler_rqb_in_use;
        LSA_UINT32 tskid_eddScheduler;
        LSAS_EDD_RQB_TYPE          scheduler_notify_rqb;
    } edds;
#endif

    struct
    {
        LSAS_RQB_TYPE              *loop_rqb_ptr;
    	LSA_UINT32                  request_processed;
    } edd;

    struct
    {
        struct
        {
            LSA_HANDLE_TYPE     handle;
            LSA_SYS_PATH_TYPE   path;
            LSA_BOOL            used;
            LSA_VOID            (*method_ptr) (void *);
        }handledb[GSY_MAX_SYS_CHANNELS*2]; //Sys + Anno
    }gsy;

    struct
    {
        LSA_HANDLE_TYPE         handle;
        #ifdef LSAS_CFG_REPORT_NEW_IP_DATA
            /* variables for IP request */
        LSA_BOOL                oha_ip_rqb_in_use;
        OHA_RQB_TYPE            oha_ip_rqb;
        LSA_UINT8               ip_data_buffer[OHA_TOPO_BLOCK_HEADER_PDU_SIZE + 2/*Reserved*/ + OHA_IP_SUITE_SIZE];
        #endif
    }oha;

    struct
    {
    	LSAS_OPEN_SM_STATE_E_T 		sm;

    	union
		{
			ACP_ARGS_TYPE  *            acp;
			CLRPC_OPEN_CHANNEL_TYPE *   clrpc;
			CM_OPEN_CHANNEL_TYPE *      cm;
			EDD_RQB_OPEN_CHANNEL_TYPE * edd;
			void *                      _void;
		} rqb_params_ptr;

		union
		{
			ACP_UPPER_RQB_PTR_TYPE   acp;
			CLRPC_UPPER_RQB_PTR_TYPE clrpc;
			OHA_UPPER_RQB_PTR_TYPE   oha;
			GSY_UPPER_RQB_PTR_TYPE   gsy;
			MRP_UPPER_RQB_PTR_TYPE   mrp;
			CM_UPPER_RQB_PTR_TYPE    cm;
			SOCK_UPPER_RQB_PTR_TYPE  sock;
			TCIP_UPPER_RQB_PTR_TYPE  tcip;
			EDD_UPPER_RQB_PTR_TYPE   edd;
#ifdef LSAS_CFG_USE_STCP
            L4_UPPER_RQB_PTR_TYPE    stcp;
           L4_UPPER_RQB_PTR_TYPE     s7pn;
#endif /* LSAS_CFG_USE_STCP */
			NARE_UPPER_RQB_PTR_TYPE  nare;
			void *                   _void;
		} rqb_ptr;

		LSA_BOOL fs_device_opened;

    } open;

}LSAS_DATA_S_T;
#include "sys_unpck.h"
LSAS_DAT_EXT_ATTR LSAS_DATA_S_T lsas_data;

#endif /* LSAS_DAT_H_ */

/*** end of file *************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
