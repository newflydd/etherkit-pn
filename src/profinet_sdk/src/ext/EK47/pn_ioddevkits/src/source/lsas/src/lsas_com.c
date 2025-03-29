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
/*  F i l e               &F: lsas_com.c                                :F&  */
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
 * @file    lsas_com.c
 * @brief   brief description
 * @author  cn3dit09
 * @version
 * @date    24.02.2012
 *
 * long description
 *
 */

/*****************************************************************************/

#define LSAS_MODULE_ID  1

#include "lsas_inc.h"

#define LSAS_COM_EXTERN_ATTR

#include "lsas_int.h"
#include "lsas_dat.h"

#define LSAS_PATH_EXT_ATTR

#include "lsas_path.h"

#ifdef LSAS_CFG_USE_FS

LSA_BOOL lsas_com_is_fs_device_open()
{
    return lsas_data.open.fs_device_opened;
}

void    lsas_com_fs_open(void)
{
    LSA_UINT16  tmp_u16;

    tmp_u16 = fs_open_device( &lsas_path_detail_struct.fs_ldb_flash_0);
    if ( FS_OK != tmp_u16 )
    {
        lsas_fatal_error( LSAS_MODULE_ID , __LINE__, tmp_u16);
    }
    lsas_data.open.fs_device_opened = LSA_TRUE;
}
#endif

void lsas_com_init_pnio_stack(void)
{
    lsas_com_init_lsa_ldbs();
    /*APMA_OPC_POF_INIT  */
#ifdef LSAS_CFG_USE_POF
    pof_init();
#endif

    /* APMA_OPC_TCPIP_INIT */
#ifdef LSAS_CFG_USE_STCP
    stcp_init(APMA_PDB_STCP_MAX_CHANNELS, APMA_PDB_S7DT_MAX_CONNECTIONS);
#endif /* LSAS_CFG_USE_STCP */


#ifndef LSAS_CFG_USE_EXTERNAL_TCPIP_STACK
    tcip_init();
#endif

    sock_init();

    /* APMA_OPC_PNIO_INIT */

     oha_init();
     #ifdef LSAS_CFG_REPORT_NEW_IP_DATA
     lsas_data.oha.oha_ip_rqb_in_use = FALSE;
     #endif

     dcp_init();

     nare_init();

     /* acp init */
     {
         ACP_INIT_TYPE init_data;

         init_data.rta_time_base   = LSA_TIME_BASE_10MS;
         init_data.rta_time_factor = 1;

         acp_init(&init_data);
     }

     /* clrpc init */
     {
         CLRPC_INIT_TYPE clrpc_init_block;

         clrpc_init_block.oneshot_lsa_time_base   = LSA_TIME_BASE_10MS;
         clrpc_init_block.oneshot_lsa_time_factor = 1;

         clrpc_init(&clrpc_init_block);
     }

     /* cm init */
     {
         CM_INIT_TYPE cm_init_block;

         cm_init_block.oneshot_lsa_time_base   = LSA_TIME_BASE_10MS;
         cm_init_block.oneshot_lsa_time_factor = 1;

         cm_init(&cm_init_block);
     }

#ifdef LSAS_CFG_USE_EDDI
     /* APMA_OPC_EDD_REQ_INIT */
    if(eddi_init() != EDD_STS_OK)
    {
        LSAS_FATAL(LSAS_FATAL_ERR_EDD);
    }
#endif
#ifdef LSAS_CFG_USE_EDDP

    LSAS_CFG_SYSTEM_RAM_INIT();

    if(eddp_init() != EDD_STS_OK)
    {
        LSAS_FATAL(LSAS_FATAL_ERR_EDD);
    }
#endif
#ifdef LSAS_CFG_USE_EDDS
    if(edds_init() != EDD_STS_OK)
    {
        LSAS_FATAL(LSAS_FATAL_ERR_EDD);
    }
#endif

    /* APMA_OPC_ORG_INIT */

    gsy_init();

    lldp_init();

    mrp_init();
    PNDV_MEMSET((void*)&glob_coupling_interface.pndv, 0, sizeof(glob_coupling_interface.pndv));
    pndv_init((PNDV_IFACE_STRUCT_PTR)&glob_coupling_interface.pndv);
}

void lsas_com_setup_pnio_stack(LSAS_RQB_PTR_TYPE rqb_ptr)
{
    lsas_edd_req_open();
}


//*************************************************************************************************
void lsas_com_open_pnio_stack_sm (void)
{
        typedef union
        {
            ACP_ARGS_TYPE               acp;
            CLRPC_OPEN_CHANNEL_TYPE     clrpc;
            CM_OPEN_CHANNEL_TYPE        cm;
            EDD_RQB_OPEN_CHANNEL_TYPE   edd;
        } rqb_params_type;

        typedef union
        {
            ACP_RQB_TYPE    acp;
            CLRPC_RQB_TYPE  clrpc;
            OHA_RQB_TYPE    oha;
            GSY_RQB_TYPE    gsy;
            MRP_RQB_TYPE    mrp;
            CM_RQB_TYPE     cm;
            SOCK_RQB_TYPE   sock;
            EDD_RQB_TYPE   edd;
#ifdef LSAS_CFG_USE_STCP
            L4_RQB_TYPE     stcp;
            L4_RQB_TYPE     s7pn;
#endif /* LSAS_CFG_USE_STCP */
        } rqb_type;
/* moved to lsas_data
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
            NARE_UPPER_RQB_PTR_TYPE  nare;
            void *                   _void;
        } rqb_ptr;
*/

    switch(lsas_data.open.sm)
    {
        /*********************************************************
        * allocate memory                                        *
        **********************************************************/
        case LSAS_OPEN_SM_ALLOC_MEM:
        {
            LSAS_ALLOC_LOCAL_MEM( &lsas_data.open.rqb_ptr._void, sizeof(rqb_type));
            if( lsas_data.open.rqb_ptr._void == LSA_NULL )
            {
            LSAS_FATAL(LSAS_FATAL_ERR_NULL_PTR);
            }

            LSAS_ALLOC_LOCAL_MEM( &lsas_data.open.rqb_params_ptr._void, sizeof(rqb_params_type));
            if( lsas_data.open.rqb_params_ptr._void == LSA_NULL )
            {
            LSAS_FATAL(LSAS_FATAL_ERR_NULL_PTR);
            }
            lsas_data.open.sm = LSAS_OPEN_SM_EDD_SYS;
            // no break
        }
        //lint -fallthrough
        /*********************************************************
        * open the system channels of system integration itself  *
        **********************************************************/
        case LSAS_OPEN_SM_EDD_SYS:
        {
            lsas_data.open.sm = LSAS_OPEN_SM_EDD_ACP;
            LSA_RQB_SET_OPCODE(lsas_data.open.rqb_ptr.edd, EDD_OPC_OPEN_CHANNEL);
            EDD_RQB_SET_PPARAM (lsas_data.open.rqb_ptr.edd, lsas_data.open.rqb_params_ptr.edd);
            lsas_data.open.rqb_params_ptr.edd->Cbf         = lsas_cbf_sys_edd_request_lower_done; // edd
            lsas_data.open.rqb_params_ptr.edd->SysPath     = LSAS_PATH_EDD_SYS;
            lsas_data.open.rqb_params_ptr.edd->HandleUpper = LSAS_PATH_EDD_SYS;
#ifdef LSAS_CFG_USE_EDDI
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.edd, eddi_open_channel);
#endif
#ifdef LSAS_CFG_USE_EDDP
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.edd, eddp_open_channel);
#endif
    #ifdef LSAS_CFG_USE_EDDS
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.edd, edds_open_channel);
    #endif
            LSAS_EDD_REQUEST_OPEN_CHANNEL(lsas_data.open.rqb_ptr._void);
            break;
        }
        /**************************************************************************
         * acp_open_channel                                                       *
         **************************************************************************/
        case LSAS_OPEN_SM_EDD_ACP:
        {
#ifndef LSAS_CFG_USE_EXTERNAL_TCPIP_STACK
            lsas_data.open.sm = LSAS_OPEN_SM_EDD_TCIP_ARP;
#else
            lsas_data.open.sm = LSAS_OPEN_SM_SOCK_SYS;
#endif
            //ACP_SET_HANDLE (lsas_data.open.rqb_ptr, user_handle);
            LSA_RQB_SET_OPCODE(lsas_data.open.rqb_ptr.acp, ACP_OPC_OPEN_CHANNEL);
            lsas_data.open.rqb_ptr.acp->args                           = (ACP_UPPER_ARGS_PTR_TYPE)lsas_data.open.rqb_params_ptr.acp;
            lsas_data.open.rqb_ptr.acp->args->channel.open.sys_path    = (LSA_SYS_PATH_TYPE)LSAS_PATH_EDD_ACP;
            //lsas_data.open.rqb_ptr->args->channel.open.handle_upper  = user_handle;
            lsas_data.open.rqb_ptr.acp->args->channel.open.acp_request_upper_done_ptr = (ACP_UPPER_CALLBACK_FCT_PTR_TYPE)lsas_cbf_sys_acp_request_lower_done;
            lsas_data.open.rqb_ptr.acp->args->channel.open.handle      = (LSA_HANDLE_TYPE)ACP_INVALID_HANDLE;
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.acp, acp_open_channel);
            LSAS_ACP_REQUEST_OPEN_CHANNEL(lsas_data.open.rqb_ptr._void);
            break;
        }
        /*****************************************************
        * open the system channels of the tcip component ... *
        ******************************************************/
#ifndef LSAS_CFG_USE_EXTERNAL_TCPIP_STACK
        case LSAS_OPEN_SM_EDD_TCIP_ARP:
        {
            lsas_data.open.sm = LSAS_OPEN_SM_EDD_TCIP_ICMP;
            LSA_RQB_SET_OPCODE(lsas_data.open.rqb_ptr.tcip, TCIP_OPC_OPEN_CHANNEL);
            // cnf only lsas_data.open.rqb_ptr.tcip->args.open.handle;
            lsas_data.open.rqb_ptr.tcip->args.open.handle_upper                = LSA_NULL;
            lsas_data.open.rqb_ptr.tcip->args.open.sys_path                    = LSAS_PATH_EDD_TCIP_ARP;
            lsas_data.open.rqb_ptr.tcip->args.open.tcip_request_upper_done_ptr = (void (*) (TCIP_UPPER_RQB_PTR_TYPE)) lsas_cbf_sys_tcip_request_lower_done;
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.tcip, tcip_open_channel);
            LSAS_TCIP_REQUEST_OPEN_CHANNEL(lsas_data.open.rqb_ptr._void);
            break;
        }
        case LSAS_OPEN_SM_EDD_TCIP_ICMP:
        {
            lsas_data.open.sm = LSAS_OPEN_SM_EDD_TCIP_UDP;
            LSA_RQB_SET_OPCODE(lsas_data.open.rqb_ptr.tcip, TCIP_OPC_OPEN_CHANNEL);
            // cnf only lsas_data.open.rqb_ptr.tcip->args.open.handle;
            lsas_data.open.rqb_ptr.tcip->args.open.handle_upper                = LSA_NULL;
            lsas_data.open.rqb_ptr.tcip->args.open.sys_path                    = LSAS_PATH_EDD_TCIP_ICMP;
            lsas_data.open.rqb_ptr.tcip->args.open.tcip_request_upper_done_ptr = (void (*) (TCIP_UPPER_RQB_PTR_TYPE)) lsas_cbf_sys_tcip_request_lower_done;
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.tcip, tcip_open_channel);
            LSAS_TCIP_REQUEST_OPEN_CHANNEL(lsas_data.open.rqb_ptr._void);
            break;
        }
        case LSAS_OPEN_SM_EDD_TCIP_UDP:
        {
            lsas_data.open.sm = LSAS_OPEN_SM_EDD_TCIP_TCP;
            LSA_RQB_SET_OPCODE(lsas_data.open.rqb_ptr.tcip, TCIP_OPC_OPEN_CHANNEL);
            // cnf only lsas_data.open.rqb_ptr.tcip->args.open.handle;
            lsas_data.open.rqb_ptr.tcip->args.open.handle_upper                = LSA_NULL;
            lsas_data.open.rqb_ptr.tcip->args.open.sys_path                    = LSAS_PATH_EDD_TCIP_UDP;
            lsas_data.open.rqb_ptr.tcip->args.open.tcip_request_upper_done_ptr = (void (*) (TCIP_UPPER_RQB_PTR_TYPE)) lsas_cbf_sys_tcip_request_lower_done;
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.tcip, tcip_open_channel);
            LSAS_TCIP_REQUEST_OPEN_CHANNEL(lsas_data.open.rqb_ptr._void);
            break;
        }
        case LSAS_OPEN_SM_EDD_TCIP_TCP:
        {
            lsas_data.open.sm = LSAS_OPEN_SM_SOCK_SYS;
            LSA_RQB_SET_OPCODE(lsas_data.open.rqb_ptr.tcip, TCIP_OPC_OPEN_CHANNEL);
            // cnf only lsas_data.open.rqb_ptr.tcip->args.open.handle;
            lsas_data.open.rqb_ptr.tcip->args.open.handle_upper                = LSA_NULL;
            lsas_data.open.rqb_ptr.tcip->args.open.sys_path                    = LSAS_PATH_EDD_TCIP_TCP;
            lsas_data.open.rqb_ptr.tcip->args.open.tcip_request_upper_done_ptr = (void (*) (TCIP_UPPER_RQB_PTR_TYPE)) lsas_cbf_sys_tcip_request_lower_done;
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.tcip, tcip_open_channel);
            LSAS_TCIP_REQUEST_OPEN_CHANNEL(lsas_data.open.rqb_ptr._void);
            break;
        }
#endif
        /*****************************************************
        * sys to sock *
        ******************************************************/
        case LSAS_OPEN_SM_SOCK_SYS:
        {
            lsas_data.open.sm = LSAS_OPEN_SM_EDD_MRP;
            LSA_RQB_SET_OPCODE(lsas_data.open.rqb_ptr.sock, SOCK_OPC_OPEN_CHANNEL);
            lsas_data.open.rqb_ptr.sock->err.lsa_component_id = LSA_COMP_ID_UNUSED;
            lsas_data.open.rqb_ptr.sock->args.channel.handle_upper  = LSA_NULL;
            lsas_data.open.rqb_ptr.sock->args.channel.sys_path      = LSAS_PATH_SOCK_SYS;
            lsas_data.open.rqb_ptr.sock->args.channel.sock_request_upper_done_ptr = (void (*) (SOCK_UPPER_RQB_PTR_TYPE)) lsas_cbf_sys_tcip_request_lower_done;
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.sock, sock_open_channel);
            LSAS_SOCK_REQUEST_OPEN_CHANNEL(lsas_data.open.rqb_ptr._void);
            break;
        }
        /**********************************************************
        * open the system channel of the mrp to edd component ... *
        ***********************************************************/
        case LSAS_OPEN_SM_EDD_MRP:
        {
            lsas_data.open.sm = LSAS_OPEN_SM_SOCK_OHA;
            LSA_RQB_SET_OPCODE(lsas_data.open.rqb_ptr.mrp, MRP_OPC_OPEN_CHANNEL);
            lsas_data.open.rqb_ptr.mrp->args.channel.open.handle   = (LSA_HANDLE_TYPE)-1;
            lsas_data.open.rqb_ptr.mrp->args.channel.open.sys_path = LSAS_PATH_EDD_MRP;
            lsas_data.open.rqb_ptr.mrp->args.channel.open.pCbf     = lsas_cbf_sys_mrp_request_lower_done;
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.mrp, mrp_open_channel);
            LSAS_MRP_REQUEST_OPEN_CHANNEL(lsas_data.open.rqb_ptr._void);
            break;
        }
        /****************************************************
        * open the system channels of the oha component ... *
        *****************************************************/
        case LSAS_OPEN_SM_SOCK_OHA:
        {
            lsas_data.open.sm = LSAS_OPEN_SM_EDD_OHA;
            LSA_RQB_SET_OPCODE(lsas_data.open.rqb_ptr.oha, OHA_OPC_OPEN_CHANNEL);
            lsas_data.open.rqb_ptr.oha->args.channel.sys_path  = LSAS_PATH_SOCK_OHA;
            lsas_data.open.rqb_ptr.oha->args.channel.oha_request_upper_done_ptr = lsas_cbf_sys_oha_request_lower_done;
            lsas_data.open.rqb_ptr.oha->args.channel.handle        = OHA_INVALID_HANDLE;
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.oha, oha_open_channel);
            LSAS_OHA_REQUEST_OPEN_CHANNEL(lsas_data.open.rqb_ptr._void);
            break;
        }
        case LSAS_OPEN_SM_EDD_OHA:
        {
            lsas_data.open.sm = LSAS_OPEN_SM_LLDP_OHA;
            LSA_RQB_SET_OPCODE(lsas_data.open.rqb_ptr.oha, OHA_OPC_OPEN_CHANNEL);
            lsas_data.open.rqb_ptr.oha->args.channel.sys_path  = LSAS_PATH_EDD_OHA;
            lsas_data.open.rqb_ptr.oha->args.channel.oha_request_upper_done_ptr = lsas_cbf_sys_oha_request_lower_done;
            lsas_data.open.rqb_ptr.oha->args.channel.handle        = OHA_INVALID_HANDLE;
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.oha, oha_open_channel);
            LSAS_OHA_REQUEST_OPEN_CHANNEL(lsas_data.open.rqb_ptr._void);
            break;
        }
        case LSAS_OPEN_SM_LLDP_OHA:
        {
#if (LSAS_CFG_USE_MRP == 1)
            lsas_data.open.sm = LSAS_OPEN_SM_MRP_OHA;
#else
            lsas_data.open.sm = LSAS_OPEN_SM_SOCK_CLRPC;
#endif
            LSA_RQB_SET_OPCODE(lsas_data.open.rqb_ptr.oha, OHA_OPC_OPEN_CHANNEL);
            lsas_data.open.rqb_ptr.oha->args.channel.sys_path  = LSAS_PATH_EDD_LLDP_OHA;
            lsas_data.open.rqb_ptr.oha->args.channel.oha_request_upper_done_ptr = lsas_cbf_sys_oha_request_lower_done;
            lsas_data.open.rqb_ptr.oha->args.channel.handle        = OHA_INVALID_HANDLE;
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.oha, oha_open_channel);
            LSAS_OHA_REQUEST_OPEN_CHANNEL(lsas_data.open.rqb_ptr._void);
            break;
        }
        case LSAS_OPEN_SM_MRP_OHA:
        {
            lsas_data.open.sm = LSAS_OPEN_SM_SOCK_CLRPC;
            LSA_RQB_SET_OPCODE(lsas_data.open.rqb_ptr.oha, OHA_OPC_OPEN_CHANNEL);
            lsas_data.open.rqb_ptr.oha->args.channel.sys_path                      = LSAS_PATH_MRP_OHA;
            lsas_data.open.rqb_ptr.oha->args.channel.oha_request_upper_done_ptr    = lsas_cbf_sys_oha_request_lower_done;
            lsas_data.open.rqb_ptr.oha->args.channel.handle                        = OHA_INVALID_HANDLE;
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.oha, oha_open_channel);
            LSAS_OHA_REQUEST_OPEN_CHANNEL(lsas_data.open.rqb_ptr._void);
            break;
        }
        /**************************************************************************
         * clrpc_open_channel                                                     *
         **************************************************************************/
        case LSAS_OPEN_SM_SOCK_CLRPC:
        {
            lsas_data.open.sm = LSAS_OPEN_SM_EDD_DCP_OHA;
            LSA_RQB_SET_OPCODE( lsas_data.open.rqb_ptr.clrpc, CLRPC_OPC_OPEN_CHANNEL );
            lsas_data.open.rqb_ptr.clrpc->args.channel.open               = lsas_data.open.rqb_params_ptr.clrpc;
            lsas_data.open.rqb_ptr.clrpc->args.channel.open->handle       = CLRPC_LL_INVALID_HANDLE;
            /*clrpc_lsas_data.open.rqb_ptr->args.channel.open->handle_upper = my_handle; */
            lsas_data.open.rqb_ptr.clrpc->args.channel.open->sys_path     = LSAS_PATH_SOCK_CLRPC;
            lsas_data.open.rqb_ptr.clrpc->args.channel.open->clrpc_request_upper_done_ptr
                                                       = lsas_cbf_sys_clrpc_request_lower_done;
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.clrpc, clrpc_open_channel);
            LSAS_CLRPC_REQUEST_OPEN_CHANNEL(lsas_data.open.rqb_ptr._void);
            break;
        }
        /**************************************************************************
        * oha to dcp *
         **************************************************************************/
        case LSAS_OPEN_SM_EDD_DCP_OHA:
        {
            lsas_data.open.sm = LSAS_OPEN_SM_DCP_NARE;
            LSA_RQB_SET_OPCODE(lsas_data.open.rqb_ptr.oha, OHA_OPC_OPEN_CHANNEL);
            lsas_data.open.rqb_ptr.oha->args.channel.sys_path  = LSAS_PATH_EDD_DCP_OHA;
            lsas_data.open.rqb_ptr.oha->args.channel.oha_request_upper_done_ptr = lsas_cbf_sys_oha_request_lower_done;
            lsas_data.open.rqb_ptr.oha->args.channel.handle        = OHA_INVALID_HANDLE;
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.oha, oha_open_channel);
            LSAS_OHA_REQUEST_OPEN_CHANNEL(lsas_data.open.rqb_ptr._void);
            break;
        }
        /**************************************************************************
        * open the system channels of the nare component ...*
         **************************************************************************/
        case LSAS_OPEN_SM_DCP_NARE:
        {
            lsas_data.open.sm = LSAS_OPEN_SM_EDD_NARE;
            LSA_RQB_SET_OPCODE(lsas_data.open.rqb_ptr.nare, NARE_OPC_OPEN_CHANNEL);
            lsas_data.open.rqb_ptr.nare->args.channel.handle                       = 0; /* output */
            lsas_data.open.rqb_ptr.nare->args.channel.handle_upper                 = 0;
            lsas_data.open.rqb_ptr.nare->args.channel.sys_path                     = LSAS_PATH_DCP_NARE;
            lsas_data.open.rqb_ptr.nare->args.channel.nare_request_upper_done_ptr  = lsas_cbf_sys_nare_request_lower_done;
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.nare, nare_open_channel);
            LSAS_NARE_REQUEST_OPEN_CHANNEL(lsas_data.open.rqb_ptr._void);
            break;
        }
        case LSAS_OPEN_SM_EDD_NARE:
        {
            lsas_data.open.sm = LSAS_OPEN_SM_NARE_OHA;
            LSA_RQB_SET_OPCODE(lsas_data.open.rqb_ptr.nare, NARE_OPC_OPEN_CHANNEL);
            lsas_data.open.rqb_ptr.nare->args.channel.handle                       = 0; /* output */
            lsas_data.open.rqb_ptr.nare->args.channel.handle_upper                 = 0;
            lsas_data.open.rqb_ptr.nare->args.channel.sys_path                     = LSAS_PATH_EDD_NARE;
            lsas_data.open.rqb_ptr.nare->args.channel.nare_request_upper_done_ptr  = lsas_cbf_sys_nare_request_lower_done;
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.nare, nare_open_channel);
            LSAS_NARE_REQUEST_OPEN_CHANNEL(lsas_data.open.rqb_ptr._void);
            break;
        }
        /****************************************************
        * open user channel of the nare component ...       *
        *****************************************************/
        case LSAS_OPEN_SM_NARE_OHA:
        {
            lsas_data.open.sm = LSAS_OPEN_SM_EDD_GSY_SYS;
            LSA_RQB_SET_OPCODE(lsas_data.open.rqb_ptr.oha, OHA_OPC_OPEN_CHANNEL);
            lsas_data.open.rqb_ptr.oha->args.channel.sys_path  = LSAS_PATH_NARE_OHA;
            lsas_data.open.rqb_ptr.oha->args.channel.oha_request_upper_done_ptr = lsas_cbf_sys_oha_request_lower_done;
            lsas_data.open.rqb_ptr.oha->args.channel.handle        = OHA_INVALID_HANDLE;
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.oha, oha_open_channel);
            LSAS_OHA_REQUEST_OPEN_CHANNEL(lsas_data.open.rqb_ptr._void);
            break;
        }
        /***************************************************
        * open the system channel 0 of the gsy component ... *
        ****************************************************/
        case LSAS_OPEN_SM_EDD_GSY_SYS:
        {
            lsas_data.open.sm = LSAS_OPEN_SM_EDD_GSY_ANNO;
            LSA_RQB_SET_OPCODE(lsas_data.open.rqb_ptr.gsy, GSY_OPC_OPEN_CHANNEL);
            lsas_data.open.rqb_ptr.gsy->args.channel.sys_path  = LSAS_PATH_EDD_GSY_SYS;
            lsas_data.open.rqb_ptr.gsy->args.channel.gsy_request_upper_done_ptr = lsas_cbf_sys_gsy_request_lower_done;
            lsas_data.open.rqb_ptr.gsy->args.channel.handle        = (LSA_HANDLE_TYPE)-1;
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.gsy, gsy_open_channel);
            LSAS_GSY_REQUEST_OPEN_CHANNEL(lsas_data.open.rqb_ptr._void);
            break;
        }
        /***************************************************
        * open the system channel 1 of the gsy component ... *
        ****************************************************/
        case LSAS_OPEN_SM_EDD_GSY_ANNO:
        {
            lsas_data.open.sm = LSAS_OPEN_SM_OHA_SYS;
            LSA_RQB_SET_OPCODE(lsas_data.open.rqb_ptr.gsy, GSY_OPC_OPEN_CHANNEL);
            lsas_data.open.rqb_ptr.gsy->args.channel.sys_path  = LSAS_PATH_EDD_GSY_ANNO;
            lsas_data.open.rqb_ptr.gsy->args.channel.gsy_request_upper_done_ptr = lsas_cbf_sys_gsy_request_lower_done;
            lsas_data.open.rqb_ptr.gsy->args.channel.handle        = (LSA_HANDLE_TYPE)-1;
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.gsy, gsy_open_channel);
            LSAS_GSY_REQUEST_OPEN_CHANNEL(lsas_data.open.rqb_ptr._void);
            break;
        }
        /*************************************************************
        * open the system channel of the system to oha component ... *
        **************************************************************/
        case LSAS_OPEN_SM_OHA_SYS:
        {
            lsas_data.open.sm = LSAS_OPEN_SM_EDD_CM;
            LSA_RQB_SET_OPCODE(lsas_data.open.rqb_ptr.oha, OHA_OPC_OPEN_CHANNEL);
            lsas_data.open.rqb_ptr.oha->args.channel.sys_path  = LSAS_PATH_OHA_SYS;
            lsas_data.open.rqb_ptr.oha->args.channel.oha_request_upper_done_ptr = lsas_cbf_sys_oha_sys_request_lower_done;
            lsas_data.open.rqb_ptr.oha->args.channel.handle        = OHA_INVALID_HANDLE;
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.oha, oha_open_channel);
            LSAS_OHA_REQUEST_OPEN_CHANNEL(lsas_data.open.rqb_ptr._void);
            break;
        }
        /***************************************************
        * open the system channels of the cm component ... *
        ****************************************************/
        case LSAS_OPEN_SM_EDD_CM:
        {
            lsas_data.open.sm = LSAS_OPEN_SM_GSY_CM;
            LSA_RQB_SET_OPCODE(lsas_data.open.rqb_ptr.cm, CM_OPC_OPEN_CHANNEL);
            lsas_data.open.rqb_ptr.cm->args.channel.open                            = lsas_data.open.rqb_params_ptr.cm;
            lsas_data.open.rqb_ptr.cm->args.channel.open->handle                    = CM_INVALID_HANDLE;
            lsas_data.open.rqb_ptr.cm->args.channel.open->handle_upper              = CM_INVALID_HANDLE;
            lsas_data.open.rqb_ptr.cm->args.channel.open->sys_path                  = LSAS_PATH_EDD_CM;
            lsas_data.open.rqb_ptr.cm->args.channel.open->cm_request_upper_done_ptr = lsas_cbf_sys_cm_request_lower_done;
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.cm, cm_open_channel);
            LSAS_CM_REQUEST_OPEN_CHANNEL(lsas_data.open.rqb_ptr._void);
            break;
        }
        case LSAS_OPEN_SM_GSY_CM:
        {
            lsas_data.open.sm = LSAS_OPEN_SM_NARE_CM;
            LSA_RQB_SET_OPCODE(lsas_data.open.rqb_ptr.cm, CM_OPC_OPEN_CHANNEL);
            lsas_data.open.rqb_ptr.cm->args.channel.open                            = lsas_data.open.rqb_params_ptr.cm;
            lsas_data.open.rqb_ptr.cm->args.channel.open->handle                    = CM_INVALID_HANDLE;
            lsas_data.open.rqb_ptr.cm->args.channel.open->handle_upper              = CM_INVALID_HANDLE;
            lsas_data.open.rqb_ptr.cm->args.channel.open->sys_path                  = LSAS_PATH_GSY_CM;
            lsas_data.open.rqb_ptr.cm->args.channel.open->cm_request_upper_done_ptr = lsas_cbf_sys_cm_request_lower_done;
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.cm, cm_open_channel);
            LSAS_CM_REQUEST_OPEN_CHANNEL(lsas_data.open.rqb_ptr._void);
            break;
        }
        case LSAS_OPEN_SM_NARE_CM:
        {
            lsas_data.open.sm = LSAS_OPEN_SM_OHASV_CM;
            LSA_RQB_SET_OPCODE(lsas_data.open.rqb_ptr.cm, CM_OPC_OPEN_CHANNEL);
            lsas_data.open.rqb_ptr.cm->args.channel.open                            = lsas_data.open.rqb_params_ptr.cm;
            lsas_data.open.rqb_ptr.cm->args.channel.open->handle                    = CM_INVALID_HANDLE;
            lsas_data.open.rqb_ptr.cm->args.channel.open->handle_upper              = CM_INVALID_HANDLE;
            lsas_data.open.rqb_ptr.cm->args.channel.open->sys_path                  = LSAS_PATH_NARE_CM;
            lsas_data.open.rqb_ptr.cm->args.channel.open->cm_request_upper_done_ptr = lsas_cbf_sys_cm_request_lower_done;
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.cm, cm_open_channel);
            LSAS_CM_REQUEST_OPEN_CHANNEL(lsas_data.open.rqb_ptr._void);
            break;
        }
        case LSAS_OPEN_SM_OHASV_CM:
        {
            lsas_data.open.sm = LSAS_OPEN_SM_MRP_CM;
            LSA_RQB_SET_OPCODE(lsas_data.open.rqb_ptr.cm, CM_OPC_OPEN_CHANNEL);
            lsas_data.open.rqb_ptr.cm->args.channel.open                            = lsas_data.open.rqb_params_ptr.cm;
            lsas_data.open.rqb_ptr.cm->args.channel.open->handle                    = CM_INVALID_HANDLE;
            lsas_data.open.rqb_ptr.cm->args.channel.open->handle_upper              = CM_INVALID_HANDLE;
            lsas_data.open.rqb_ptr.cm->args.channel.open->sys_path                  = LSAS_PATH_OHASV_CM;
            lsas_data.open.rqb_ptr.cm->args.channel.open->cm_request_upper_done_ptr = lsas_cbf_sys_cm_request_lower_done;
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.cm, cm_open_channel);
            LSAS_CM_REQUEST_OPEN_CHANNEL(lsas_data.open.rqb_ptr._void);
            break;
        }
        case LSAS_OPEN_SM_MRP_CM:
        {
            lsas_data.open.sm = LSAS_OPEN_SM_EDD_POF_CM;
            #if CM_CFG_USE_MRP
            LSA_RQB_SET_OPCODE(lsas_data.open.rqb_ptr.cm, CM_OPC_OPEN_CHANNEL);
            lsas_data.open.rqb_ptr.cm->args.channel.open                            = lsas_data.open.rqb_params_ptr.cm;
            lsas_data.open.rqb_ptr.cm->args.channel.open->handle                    = CM_INVALID_HANDLE;
            lsas_data.open.rqb_ptr.cm->args.channel.open->handle_upper              = CM_INVALID_HANDLE;
            lsas_data.open.rqb_ptr.cm->args.channel.open->sys_path                  = LSAS_PATH_MRP_CM;
            lsas_data.open.rqb_ptr.cm->args.channel.open->cm_request_upper_done_ptr = lsas_cbf_sys_cm_request_lower_done;
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.cm, cm_open_channel);
            LSAS_CM_REQUEST_OPEN_CHANNEL(lsas_data.open.rqb_ptr._void);
            break;
            #endif
        }
        case LSAS_OPEN_SM_EDD_POF_CM:
        {
           lsas_data.open.sm = LSAS_OPEN_SM_SOCK_STCP_S7PN;
#ifdef LSAS_CFG_USE_POF
            LSA_RQB_SET_OPCODE(lsas_data.open.rqb_ptr.cm, CM_OPC_OPEN_CHANNEL);
            lsas_data.open.rqb_ptr.cm->args.channel.open                            = lsas_data.open.rqb_params_ptr.cm;
            lsas_data.open.rqb_ptr.cm->args.channel.open->handle                    = CM_INVALID_HANDLE;
            lsas_data.open.rqb_ptr.cm->args.channel.open->handle_upper              = CM_INVALID_HANDLE;
            lsas_data.open.rqb_ptr.cm->args.channel.open->sys_path                  = LSAS_PATH_EDD_POF_CM;
            lsas_data.open.rqb_ptr.cm->args.channel.open->cm_request_upper_done_ptr = lsas_cbf_sys_cm_request_lower_done;
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.cm, cm_open_channel);
            LSAS_CM_REQUEST_OPEN_CHANNEL(lsas_data.open.rqb_ptr._void);
            break;
        }
#else
        }
        //no break
#endif


       /**************************************************************************
         * s7pn, stcp  to sock                                                      *
         **************************************************************************/
        case LSAS_OPEN_SM_SOCK_STCP_S7PN:
        {
            lsas_data.open.sm = LSAS_OPEN_SM_FREE_MEM;
#ifdef LSAS_CFG_USE_STCP
            LSA_RQB_SET_OPCODE( lsas_data.open.rqb_ptr.s7pn, L4_OPC_OPEN_CHANNEL );

            lsas_data.open.rqb_ptr.s7pn->args.channel.handle       = ((LSA_HANDLE_TYPE)-1);
            lsas_data.open.rqb_ptr.s7pn->args.channel.handle_upper = S7PN_RQB_OWN_HANDLE;
            lsas_data.open.rqb_ptr.s7pn->args.channel.sys_path = LSAS_PATH_SOCK_STCP_S7PN;
            lsas_data.open.rqb_ptr.s7pn->args.channel.l4_request_upper_done_ptr
                                                                     = lsas_cbf_sys_s7pn_request_done;
            LSA_RQB_SET_METHOD_PTR(lsas_data.open.rqb_ptr.s7pn, s7pn_rqb_open_channel);
            LSAS_S7PN_REQUEST_OPEN_CHANNEL(lsas_data.open.rqb_ptr._void);

            break;
        }
#else
        }
        //no break
        //lint -fallthrough
#endif


        case LSAS_OPEN_SM_FREE_MEM:
        {
            LSA_UINT32 ret_val;
            lsas_data.open.sm = LSAS_OPEN_SM_DONE;
            LSAS_FREE_LOCAL_MEM((LSA_UINT16 *)&ret_val, lsas_data.open.rqb_params_ptr._void);
            LSAS_FREE_LOCAL_MEM((LSA_UINT16 *)&ret_val, lsas_data.open.rqb_ptr._void);
            LSA_UNUSED_ARG(ret_val);
            LSAS_CFG_PNIO_OPEN_DONE();
            break;
        }

        case LSAS_OPEN_SM_CLOSED:
        case LSAS_OPEN_SM_DONE:
        default:
        {
            LSAS_FATAL(lsas_data.open.sm);
            break;
        }
    }
}


//*************************************************************************************************
void lsas_com_open_pnio_stack (void)
{
    lsas_data.open.sm = LSAS_OPEN_SM_ALLOC_MEM;
    lsas_com_open_pnio_stack_sm();
}

/*************************************************************************************************/

void lsas_com_shutdown_network(void)
{
#ifdef LSAS_CFG_USE_EDDI
    LSAS_EDD_HANDLE     hDDB;

    /* Reset IRTStarttime */
    if (lsas_data.edd_hxdb_set)
    {
        hDDB = lsas_com_get_hddb();
        eddi_ResetIRTStarttime( hDDB );

        /* power down the edd and network transfers */
        eddi_SetExternalException( hDDB );
    }
#elif defined(LSAS_CFG_USE_EDDP)
    /* reset network */
    LSAS_CFG_SYSTEM_RAM_INIT();
#endif

}


void lsas_com_init_lsa_ldbs(void)
{
/******************************************************************************
 *                                                                            *
 *                                    LDB's                                   *
 *                                                                            *
 ******************************************************************************/

    LSAS_MEMSET( &lsas_path_detail_struct, 0, sizeof(lsas_path_detail_struct) );

    /* acp ldb's ============================================================*/

    /* LSAS_PATH_ACP_CM */

    lsas_path_detail_struct.acp_ldb_cm_acp.nic_id       = EDD_INTERFACE_ID_MIN;                   /* unknown *//* reference to the network device which must be used */
    lsas_path_detail_struct.acp_ldb_cm_acp.channel_type = ACP_CHANNEL_TYPE_USER;

            /* alarm */
    lsas_path_detail_struct.acp_ldb_cm_acp.type.user.alarm_ref_base  = 0;                         /* first available alarm-reference number */
    lsas_path_detail_struct.acp_ldb_cm_acp.type.user.alarm_nr_of_ref = LSAS_MAX_ARS_RTC1_RTC2;    /* number of available reference numbers */

    /* LSAS_PATH_EDD_ACP */

    lsas_path_detail_struct.edd_ldb_acp_acp.nic_id       = EDD_INTERFACE_ID_MIN;                  /* unknown *//* reference to the network device which must be used */
    lsas_path_detail_struct.edd_ldb_acp_acp.channel_type = ACP_CHANNEL_TYPE_SYSTEM;

    lsas_path_detail_struct.edd_ldb_acp_acp.type.system.max_edd_nrt_send_res = (LSAS_MAX_ARS_RTC1_RTC2+1)*2;     /* count of send resouces, used by edd.nrt.send */
    lsas_path_detail_struct.edd_ldb_acp_acp.type.system.max_edd_nrt_recv_res = (LSAS_MAX_ARS_RTC1_RTC2+1)*2;     /* count of recv resouces, used by edd.nrt.recv */

    /* clrpc ldb's ==========================================================*/

    /* LSAS_PATH_EDD_CLRPC_CM */

    /* io controller */
    lsas_path_detail_struct.edd_clrpc_ldb_cm_clrpc.path_type                        = CLRPC_PATH_TYPE_USER;


    /* client */
    lsas_path_detail_struct.edd_clrpc_ldb_cm_clrpc.u.cl.nothing_so_far                = 0;

    /* LSAS_PATH_SOCK_CLRPC */

    /* io controller */
    lsas_path_detail_struct.clrpc_ldb_clrpc.path_type                        = CLRPC_PATH_TYPE_SYSTEM;

    /* EPM server */

    lsas_path_detail_struct.clrpc_ldb_clrpc.u.epm.nothing_so_far               = 0;

    /* cm ldb's =============================================================*/

    /* LSAS_PATH_EDD_CLRPC_CM */

    /* io controler */
    lsas_path_detail_struct.edd_clrpc_ldb_cm_cm.path_type                                 = CM_PATH_TYPE_RPC;
    lsas_path_detail_struct.edd_clrpc_ldb_cm_cm.nic_id                                    = EDD_INTERFACE_ID_MIN;

    lsas_path_detail_struct.edd_clrpc_ldb_cm_cm.u.rpc.recv_resource_count                 = 20;

    /* LSAS_PATH_EDD_CM */

    lsas_path_detail_struct.edd_ldb_cm_cm.path_type                                       = CM_PATH_TYPE_EDD;
    lsas_path_detail_struct.edd_ldb_cm_cm.nic_id                                          = EDD_INTERFACE_ID_MIN;

    /* LSAS_PATH_GSY_CM */

    lsas_path_detail_struct.gsy_ldb_cm_cm.path_type                                       = CM_PATH_TYPE_GSY;
    lsas_path_detail_struct.gsy_ldb_cm_cm.nic_id                                          = EDD_INTERFACE_ID_MIN;

    lsas_path_detail_struct.gsy_ldb_cm_cm.u.gsy.nothing_so_far                            = 0;

    /* LSAS_PATH_NARE_CM */

    lsas_path_detail_struct.nare_ldb_cm_cm.path_type                                      = CM_PATH_TYPE_NARE;
    lsas_path_detail_struct.nare_ldb_cm_cm.nic_id                                         = EDD_INTERFACE_ID_MIN;
    lsas_path_detail_struct.nare_ldb_cm_cm.u.nare.resolve_notify_resource_count           = 0;


    /* LSAS_PATH_MRP_CM */
    #if CM_CFG_USE_MRP
    lsas_path_detail_struct.mrp_ldb_cm_cm.path_type                                       = CM_PATH_TYPE_MRP;
    lsas_path_detail_struct.mrp_ldb_cm_cm.nic_id                                          = EDD_INTERFACE_ID_MIN;
    lsas_path_detail_struct.mrp_ldb_cm_cm.u.mrp.nothing_so_far                            = 0;
    #endif

    /* LSAS_PATH_OHASV_CM */

    lsas_path_detail_struct.oha_ldb_cm_cm.path_type                                       = CM_PATH_TYPE_OHA;
    lsas_path_detail_struct.oha_ldb_cm_cm.nic_id                                          = EDD_INTERFACE_ID_MIN;

    lsas_path_detail_struct.oha_ldb_cm_cm.u.oha.hello_resource_count                      = 0;    /* device: value := 0 */

    /* LSAS_PATH_CMPDSV */

    lsas_path_detail_struct.cmpdsv_ldb_cm_cm.path_type                                    = CM_PATH_TYPE_PD;
    lsas_path_detail_struct.cmpdsv_ldb_cm_cm.nic_id                                       = EDD_INTERFACE_ID_MIN;

    lsas_path_detail_struct.cmpdsv_ldb_cm_cm.u.pd.epm.recv_resource_count                    = 20;
    lsas_path_detail_struct.cmpdsv_ldb_cm_cm.u.pd.epm.send_resource_count                    = 20;

    /* LSAS_PATH_ACP_CM */

    /* io controler */
    lsas_path_detail_struct.acp_ldb_cm_cm.path_type                                = CM_PATH_TYPE_SERVER;
    lsas_path_detail_struct.acp_ldb_cm_cm.nic_id                                   = EDD_INTERFACE_ID_MIN;

    /* for clrpc */
    lsas_path_detail_struct.acp_ldb_cm_cm.u.sv.rpc.alloc_len                          = LSAS_PDB_MAX_RPC_DATA_LEN + CM_RECORD_OFFSET;/* frame size ; lt. LSA_CM_Speicherbedarf.xls 4652 Byte for cennect.req*/
    lsas_path_detail_struct.acp_ldb_cm_cm.u.sv.rpc.call_resource_count                = CM_CFG_MAX_SV_ARS + CLRPC_CFG_MAX_READ_IMPLICIT_PER_OBJ_UUID; /* summe = 13 */
    lsas_path_detail_struct.acp_ldb_cm_cm.u.sv.rpc.recv_resource_count                = 3;
    lsas_path_detail_struct.acp_ldb_cm_cm.u.sv.rpc.send_resource_count                = 10;

    /* for acp */
    lsas_path_detail_struct.acp_ldb_cm_cm.u.sv.acp.alarm_high_prio_ind_resource_count = 30;
    lsas_path_detail_struct.acp_ldb_cm_cm.u.sv.acp.alarm_low_prio_ind_resource_count  = 30;
    lsas_path_detail_struct.acp_ldb_cm_cm.u.sv.acp.max_alarm_data_length              = CM_ALARM_DATA_LENGTH_MIN;

    /* dcp ldb's ============================================================*/

   /* LSAS_PATH_EDD_DCP_OHA */

    lsas_path_detail_struct.edd_dcp_ldb_oha_dcp.LowerId                 = DCP_DEV_ETH;/* Lower Channel Identification                     */
    lsas_path_detail_struct.edd_dcp_ldb_oha_dcp.DevId                   = DCP_DEV_ETH;/* Channel device: Ethernet or Profibus             */
                                                                                      /* (currently only Eth. supported)                  */
    lsas_path_detail_struct.edd_dcp_ldb_oha_dcp.ClientCh                = FALSE;      /* FALSE: This is a server channel                  */
    lsas_path_detail_struct.edd_dcp_ldb_oha_dcp.ServerHelloResources    = 0;          /* FSU channel: Number of receive RQBs in lower      */
                                                                                      /* layer for incoming HELLO requests                */
                                                                                      /* ------------------------------------------------ */

    lsas_path_detail_struct.edd_dcp_ldb_oha_dcp.ClientResources         = 0;          /* Client channel: Number of RQBs and timers for */
                                                                                      /* outgoing client requests                         */

    /* client mode only */
    lsas_path_detail_struct.edd_dcp_ldb_oha_dcp.AllocOnRequest          = LSA_FALSE; /* FALSE: Allocate all lower RBs at open_channel    */
    lsas_path_detail_struct.edd_dcp_ldb_oha_dcp.HelloEnable             = LSA_FALSE; /* Client channel: Use 10ms HELLO timer               */


    /* LSAS_PATH_DCP_NARE */

    lsas_path_detail_struct.dcp_ldb_nare_dcp.LowerId                    = DCP_DEV_ETH;  /* Lower Channel Identification                     */
    lsas_path_detail_struct.dcp_ldb_nare_dcp.DevId                      = DCP_DEV_ETH;  /* Channel device: Ethernet or Profibus             */
                                                                                        /* (currently only Eth. supported)                  */
    lsas_path_detail_struct.dcp_ldb_nare_dcp.ClientCh                   = LSA_TRUE;     /* FALSE: This is a server channel */
    lsas_path_detail_struct.dcp_ldb_nare_dcp.ServerHelloResources       = 0;            /* FSU channel: Number of receive RQBs in lower      */
                                                                                        /* layer for incoming HELLO requests                */
                                                                                        /* ------------------------------------------------ */
    lsas_path_detail_struct.dcp_ldb_nare_dcp.ClientResources            = 1;            /* Client channel: Number of RQBs and timers for */
                                                                                        /* outgoing client requests                         */

                                                                                        /* client mode only */

    lsas_path_detail_struct.dcp_ldb_nare_dcp.AllocOnRequest             = LSA_FALSE;    /* FALSE: Allocate all lower RBs at open_channel    */
    lsas_path_detail_struct.dcp_ldb_nare_dcp.HelloEnable                = LSA_TRUE;     /* Client channel: Use 10ms HELLO timer               */


    /* edd ldb's ============================================================*/

    /* LSAS_PATH_EDD_DCP_OHA */
#ifdef LSAS_CFG_USE_EDDI
    lsas_path_detail_struct.edd_dcp_ldb_oha_edd.UsedComp.UseNRT                       = LSA_TRUE;
    lsas_path_detail_struct.edd_dcp_ldb_oha_edd.UsedComp.UseXRT                       = LSA_FALSE;
    lsas_path_detail_struct.edd_dcp_ldb_oha_edd.UsedComp.UseSWITCH                    = LSA_FALSE;
    lsas_path_detail_struct.edd_dcp_ldb_oha_edd.UsedComp.UseSYNC                      = LSA_FALSE;
    lsas_path_detail_struct.edd_dcp_ldb_oha_edd.UsedComp.UsePRM                       = LSA_FALSE;
  /*lsas_path_detail_struct.edd_dcp_ldb_oha_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_dcp_ldb_oha_edd.InsertSrcMAC                          = LSA_TRUE;
    lsas_path_detail_struct.edd_dcp_ldb_oha_edd.pNRT                                  = &lsas_path_detail_struct.edd_dcp_nrt_oha;
    lsas_path_detail_struct.edd_dcp_ldb_oha_edd.Prm_PortDataAdjustLesserCheckQuality  = LSA_FALSE;

    lsas_path_detail_struct.edd_dcp_nrt_oha.FrameFilter               = EDD_NRT_FRAME_DCP;/* enable types with length field and DCP SNAP */
    lsas_path_detail_struct.edd_dcp_nrt_oha.Channel                   = EDDI_NRT_CHANEL_A_IF_0;
    lsas_path_detail_struct.edd_dcp_nrt_oha.UseFrameTriggerUnit       = LSA_FALSE;
#endif
#ifdef LSAS_CFG_USE_EDDP
    lsas_path_detail_struct.edd_dcp_ldb_oha_edd.UsedComp.UseNRT                       = LSA_TRUE;
    lsas_path_detail_struct.edd_dcp_ldb_oha_edd.UsedComp.UseCRT                       = LSA_FALSE;
    lsas_path_detail_struct.edd_dcp_ldb_oha_edd.UsedComp.UseK32                       = LSA_FALSE;
    lsas_path_detail_struct.edd_dcp_ldb_oha_edd.UsedComp.UsePRM                       = LSA_FALSE;
  /*lsas_path_detail_struct.edd_dcp_ldb_oha_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_dcp_ldb_oha_edd.pNRT                                  = &lsas_path_detail_struct.edd_dcp_nrt_oha;
    lsas_path_detail_struct.edd_dcp_ldb_oha_edd.pCRT                                  = LSA_NULL;
    lsas_path_detail_struct.edd_dcp_ldb_oha_edd.pPRM                                  = LSA_NULL;

    lsas_path_detail_struct.edd_dcp_nrt_oha.FrameFilter               = EDD_NRT_FRAME_DCP;/* enable types with length field and DCP SNAP */

#endif
#ifdef LSAS_CFG_USE_EDDS
    lsas_path_detail_struct.edd_dcp_ldb_oha_edd.UsedComp.UseNRT                          = LSA_TRUE;
    lsas_path_detail_struct.edd_dcp_ldb_oha_edd.UsedComp.UseCSRT                      = LSA_FALSE;
    lsas_path_detail_struct.edd_dcp_ldb_oha_edd.UsedComp.UsePRM                          = LSA_FALSE;
    /* lsas_path_detail_struct.edd_dcp_ldb_oha_edd.hDDB                               = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_dcp_ldb_oha_edd.InsertSrcMAC                          = LSA_FALSE;
    lsas_path_detail_struct.edd_dcp_ldb_oha_edd.pNRT                                  = &lsas_path_detail_struct.edd_dcp_nrt_oha;
    lsas_path_detail_struct.edd_dcp_ldb_oha_edd.pPRM                                  = LSA_NULL;

    lsas_path_detail_struct.edd_dcp_nrt_oha.FrameFilter                                  = EDD_NRT_FRAME_DCP;/* enable types with length field and DCP SNAP */
#endif
    /* LSAS_PATH_EDD_OHA */
#ifdef LSAS_CFG_USE_EDDI
    lsas_path_detail_struct.edd_ldb_oha_edd.UsedComp.UseNRT                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_oha_edd.UsedComp.UseXRT                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_oha_edd.UsedComp.UseSWITCH                    = LSA_FALSE;  /* this path is only for information exchange */
    lsas_path_detail_struct.edd_ldb_oha_edd.UsedComp.UseSYNC                      = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_oha_edd.UsedComp.UsePRM                       = LSA_FALSE;
  /*lsas_path_detail_struct.edd_ldb_oha_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_oha_edd.InsertSrcMAC                          = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_oha_edd.pNRT                                  = NIL;
    lsas_path_detail_struct.edd_ldb_oha_edd.Prm_PortDataAdjustLesserCheckQuality  = LSA_FALSE;
#endif
#ifdef LSAS_CFG_USE_EDDP
    lsas_path_detail_struct.edd_ldb_oha_edd.UsedComp.UseNRT                       = LSA_TRUE;     /* see PCIOX: on channel open, oha tries to set an ARP filter */
    lsas_path_detail_struct.edd_ldb_oha_edd.UsedComp.UseCRT                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_oha_edd.UsedComp.UseK32                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_oha_edd.UsedComp.UsePRM                       = LSA_FALSE;
  /*lsas_path_detail_struct.edd_ldb_oha_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_oha_edd.pCRT                                  = LSA_NULL;
    lsas_path_detail_struct.edd_ldb_oha_edd.pNRT                                  = &lsas_path_detail_struct.edd_nrt_oha_edd;
    lsas_path_detail_struct.edd_ldb_oha_edd.pPRM                                  = LSA_NULL;

    lsas_path_detail_struct.edd_nrt_oha_edd.FrameFilter                           = 0;            /* see PCIOX: no frames to be received */
#endif
#ifdef LSAS_CFG_USE_EDDS
    lsas_path_detail_struct.edd_ldb_oha_edd.UsedComp.UseNRT                          = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_oha_edd.UsedComp.UseCSRT                      = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_oha_edd.UsedComp.UsePRM                          = LSA_FALSE;
    /* lsas_path_detail_struct.edd_ldb_oha_edd.hDDB                               = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_oha_edd.InsertSrcMAC                          = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_oha_edd.pNRT                                  = &lsas_path_detail_struct.edd_nrt_oha_edd;
    lsas_path_detail_struct.edd_ldb_oha_edd.pPRM                                  = LSA_NULL;

    lsas_path_detail_struct.edd_nrt_oha_edd.FrameFilter                              = 0;            /* see PCIOX: no frames to be received */
#endif
    /* LSAS_PATH_EDD_ACP */
#ifdef LSAS_CFG_USE_EDDI
    lsas_path_detail_struct.edd_ldb_acp_edd.UsedComp.UseNRT                       = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_acp_edd.UsedComp.UseXRT                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_acp_edd.UsedComp.UseSWITCH                    = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_acp_edd.UsedComp.UseSYNC                      = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_acp_edd.UsedComp.UsePRM                       = LSA_FALSE;
  /*lsas_path_detail_structedd_ldb_acp_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_acp_edd.InsertSrcMAC                          = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_acp_edd.pNRT                                  = &lsas_path_detail_struct.edd_nrt_acp;
    lsas_path_detail_struct.edd_ldb_acp_edd.Prm_PortDataAdjustLesserCheckQuality  = LSA_FALSE;

    lsas_path_detail_struct.edd_nrt_acp.FrameFilter          = EDD_NRT_FRAME_ASRT;          /* all ASRT frames */
    lsas_path_detail_struct.edd_nrt_acp.Channel              = EDDI_NRT_CHANEL_A_IF_0;
    lsas_path_detail_struct.edd_nrt_acp.UseFrameTriggerUnit  = LSA_FALSE;
#endif
#ifdef LSAS_CFG_USE_EDDP
    lsas_path_detail_struct.edd_ldb_acp_edd.UsedComp.UseNRT                       = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_acp_edd.UsedComp.UseCRT                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_acp_edd.UsedComp.UseK32                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_acp_edd.UsedComp.UsePRM                       = LSA_FALSE;
  /*lsas_path_detail_struct.edd_ldb_acp_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_acp_edd.pNRT                                  = &lsas_path_detail_struct.edd_nrt_acp_edd;
    lsas_path_detail_struct.edd_ldb_acp_edd.pCRT                                  = NIL;
    lsas_path_detail_struct.edd_ldb_acp_edd.pPRM                                  = LSA_NULL;

    lsas_path_detail_struct.edd_nrt_acp_edd.FrameFilter          = EDD_NRT_FRAME_ASRT;          /* all ASRT frames */
#endif
#ifdef LSAS_CFG_USE_EDDS
    lsas_path_detail_struct.edd_ldb_acp_edd.UsedComp.UseNRT                          = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_acp_edd.UsedComp.UseCSRT                      = LSA_FALSE;  // FALSE according to PSI. Old value: LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_acp_edd.UsedComp.UsePRM                          = LSA_FALSE;
 /* lsas_path_detail_struct.edd_ldb_acp_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_acp_edd.InsertSrcMAC                          = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_acp_edd.pNRT                                  = &lsas_path_detail_struct.edd_nrt_acp_edd;
    lsas_path_detail_struct.edd_ldb_acp_edd.pPRM                                  = LSA_NULL;

    lsas_path_detail_struct.edd_nrt_acp_edd.FrameFilter                              = EDD_NRT_FRAME_ASRT;          /* all ASRT frames */
#endif
    /* LSAS_PATH_EDD_LLDP_OHA */
#ifdef LSAS_CFG_USE_EDDI
    lsas_path_detail_struct.edd_lldp_ldb_oha_edd.UsedComp.UseNRT                       = LSA_TRUE;
    lsas_path_detail_struct.edd_lldp_ldb_oha_edd.UsedComp.UseXRT                       = LSA_FALSE;
    lsas_path_detail_struct.edd_lldp_ldb_oha_edd.UsedComp.UseSWITCH                    = LSA_FALSE;
    lsas_path_detail_struct.edd_lldp_ldb_oha_edd.UsedComp.UseSYNC                      = LSA_FALSE;
    lsas_path_detail_struct.edd_lldp_ldb_oha_edd.UsedComp.UsePRM                       = LSA_FALSE;
  /*lsas_path_detail_struct.edd_lldp_ldb_oha_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_lldp_ldb_oha_edd.InsertSrcMAC                          = LSA_FALSE;
    lsas_path_detail_struct.edd_lldp_ldb_oha_edd.pNRT                                  = &lsas_path_detail_struct.edd_lldp_nrt_oha;
    lsas_path_detail_struct.edd_lldp_ldb_oha_edd.Prm_PortDataAdjustLesserCheckQuality  = LSA_FALSE;

    lsas_path_detail_struct.edd_lldp_nrt_oha.FrameFilter          = EDD_NRT_FRAME_LLDP;          /* all LLDP frames */
    lsas_path_detail_struct.edd_lldp_nrt_oha.Channel              = EDDI_NRT_CHANEL_B_IF_0;
    lsas_path_detail_struct.edd_lldp_nrt_oha.UseFrameTriggerUnit  = LSA_FALSE;
#endif
#ifdef LSAS_CFG_USE_EDDP
    lsas_path_detail_struct.edd_lldp_ldb_oha_edd.UsedComp.UseNRT                       = LSA_TRUE;
    lsas_path_detail_struct.edd_lldp_ldb_oha_edd.UsedComp.UseCRT                       = LSA_FALSE;
    lsas_path_detail_struct.edd_lldp_ldb_oha_edd.UsedComp.UseK32                       = LSA_FALSE;
    lsas_path_detail_struct.edd_lldp_ldb_oha_edd.UsedComp.UsePRM                       = LSA_FALSE;
  /*lsas_path_detail_struct.edd_lldp_ldb_oha_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_lldp_ldb_oha_edd.pNRT                                  = &lsas_path_detail_struct.edd_lldp_nrt_oha;
    lsas_path_detail_struct.edd_lldp_ldb_oha_edd.pCRT                                  = LSA_NULL;
    lsas_path_detail_struct.edd_lldp_ldb_oha_edd.pPRM                                  = LSA_NULL;

    lsas_path_detail_struct.edd_lldp_nrt_oha.FrameFilter          = EDD_NRT_FRAME_LLDP;
#endif
#ifdef LSAS_CFG_USE_EDDS
    lsas_path_detail_struct.edd_lldp_ldb_oha_edd.UsedComp.UseNRT                      = LSA_TRUE;
    lsas_path_detail_struct.edd_lldp_ldb_oha_edd.UsedComp.UseCSRT                      = LSA_FALSE;
    lsas_path_detail_struct.edd_lldp_ldb_oha_edd.UsedComp.UsePRM                      = LSA_FALSE;
 /* lsas_path_detail_struct.edd_lldp_ldb_oha_edd.hDDB                                 = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_lldp_ldb_oha_edd.InsertSrcMAC                          = LSA_FALSE;
    lsas_path_detail_struct.edd_lldp_ldb_oha_edd.pNRT                                  = &lsas_path_detail_struct.edd_lldp_nrt_oha;
    lsas_path_detail_struct.edd_lldp_ldb_oha_edd.pPRM                                  = LSA_NULL;

    lsas_path_detail_struct.edd_lldp_nrt_oha.FrameFilter                              = EDD_NRT_FRAME_LLDP;          /* all ASRT frames */
#endif
    /* LSAS_PATH_EDD_GSY_SYS */
#ifdef LSAS_CFG_USE_EDDI
    lsas_path_detail_struct.edd_ldb_gsy_edd_sys.UsedComp.UseNRT                       = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_gsy_edd_sys.UsedComp.UseXRT                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_gsy_edd_sys.UsedComp.UseSWITCH                    = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_gsy_edd_sys.UsedComp.UseSYNC                      = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_gsy_edd_sys.UsedComp.UsePRM                       = LSA_FALSE;
  /*lsas_path_detail_struct.edd_ldb_gsy_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_gsy_edd_sys.InsertSrcMAC                          = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_gsy_edd_sys.pNRT                                  = &lsas_path_detail_struct.edd_nrt_gsy_sys;
    lsas_path_detail_struct.edd_ldb_gsy_edd_sys.Prm_PortDataAdjustLesserCheckQuality  = LSA_FALSE;

    lsas_path_detail_struct.edd_nrt_gsy_sys.FrameFilter          = EDD_NRT_FRAME_PTCP_SYNC|EDD_NRT_FRAME_PTCP_DELAY;
    lsas_path_detail_struct.edd_nrt_gsy_sys.Channel              = EDDI_NRT_CHANEL_B_IF_0;
    lsas_path_detail_struct.edd_nrt_gsy_sys.UseFrameTriggerUnit  = LSA_FALSE;
#endif
#ifdef LSAS_CFG_USE_EDDP
    lsas_path_detail_struct.edd_ldb_gsy_edd_sys.UsedComp.UseNRT                       = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_gsy_edd_sys.UsedComp.UseCRT                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_gsy_edd_sys.UsedComp.UseK32                       = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_gsy_edd_sys.UsedComp.UsePRM                       = LSA_FALSE;
  /*lsas_path_detail_struct.edd_ldb_gsy_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_gsy_edd_sys.pNRT                                  = &lsas_path_detail_struct.edd_nrt_gsy_sys;
    lsas_path_detail_struct.edd_ldb_gsy_edd_sys.pCRT                                  = LSA_NULL;
    lsas_path_detail_struct.edd_ldb_gsy_edd_sys.pPRM                                  = LSA_NULL;

    lsas_path_detail_struct.edd_nrt_gsy_sys.FrameFilter          = EDD_NRT_FRAME_PTCP_SYNC|EDD_NRT_FRAME_PTCP_DELAY;
#endif
#ifdef LSAS_CFG_USE_EDDS
    lsas_path_detail_struct.edd_ldb_gsy_edd_sys.UsedComp.UseNRT                            = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_gsy_edd_sys.UsedComp.UseCSRT                      = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_gsy_edd_sys.UsedComp.UsePRM                          = LSA_FALSE;
 /* lsas_path_detail_struct.edd_ldb_gsy_edd_sys.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_gsy_edd_sys.InsertSrcMAC                          = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_gsy_edd_sys.pNRT                                  = &lsas_path_detail_struct.edd_nrt_gsy_sys;
    lsas_path_detail_struct.edd_ldb_gsy_edd_sys.pPRM                                  = LSA_NULL;

    lsas_path_detail_struct.edd_nrt_gsy_sys.FrameFilter                              = EDD_NRT_FRAME_PTCP_SYNC|EDD_NRT_FRAME_PTCP_DELAY;
#endif
    /* LSAS_PATH_EDD_GSY_ANNO */
#ifdef LSAS_CFG_USE_EDDI
    lsas_path_detail_struct.edd_ldb_gsy_edd_anno.UsedComp.UseNRT = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_gsy_edd_anno.UsedComp.UseXRT = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_gsy_edd_anno.UsedComp.UseSWITCH = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_gsy_edd_anno.UsedComp.UseSYNC = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_gsy_edd_anno.UsedComp.UsePRM = LSA_FALSE;
    /*lsas_path_detail_struct.edd_ldb_gsy_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_gsy_edd_anno.InsertSrcMAC = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_gsy_edd_anno.pNRT = &lsas_path_detail_struct.edd_nrt_gsy_anno;
    lsas_path_detail_struct.edd_ldb_gsy_edd_anno.Prm_PortDataAdjustLesserCheckQuality = LSA_FALSE;

    lsas_path_detail_struct.edd_nrt_gsy_anno.FrameFilter = EDD_NRT_FRAME_PTCP_ANNO;
    lsas_path_detail_struct.edd_nrt_gsy_anno.Channel = EDDI_NRT_CHANEL_A_IF_0;
    lsas_path_detail_struct.edd_nrt_gsy_anno.UseFrameTriggerUnit = LSA_FALSE;
#endif
#ifdef LSAS_CFG_USE_EDDP
    lsas_path_detail_struct.edd_ldb_gsy_edd_anno.UsedComp.UseNRT = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_gsy_edd_anno.UsedComp.UseCRT = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_gsy_edd_anno.UsedComp.UseK32 = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_gsy_edd_anno.UsedComp.UsePRM = LSA_FALSE;
  /*lsas_path_detail_struct.edd_ldb_gsy_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_gsy_edd_anno.pNRT = &lsas_path_detail_struct.edd_nrt_gsy_anno;
    lsas_path_detail_struct.edd_ldb_gsy_edd_anno.pCRT = LSA_NULL;
    lsas_path_detail_struct.edd_ldb_gsy_edd_anno.pPRM = LSA_NULL;

    lsas_path_detail_struct.edd_nrt_gsy_anno.FrameFilter = EDD_NRT_FRAME_PTCP_ANNO;
#endif
#ifdef LSAS_CFG_USE_EDDS
    lsas_path_detail_struct.edd_ldb_gsy_edd_anno.UsedComp.UseNRT                      = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_gsy_edd_anno.UsedComp.UseCSRT                      = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_gsy_edd_anno.UsedComp.UsePRM                      = LSA_FALSE;
 /* lsas_path_detail_struct.edd_ldb_gsy_edd_anno.hDDB                                 = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_gsy_edd_anno.InsertSrcMAC                          = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_gsy_edd_anno.pNRT                                  = &lsas_path_detail_struct.edd_nrt_gsy_anno;
    lsas_path_detail_struct.edd_ldb_gsy_edd_anno.pPRM                                  = LSA_NULL;

    lsas_path_detail_struct.edd_nrt_gsy_anno.FrameFilter                              = EDD_NRT_FRAME_PTCP_ANNO;
#endif
    /* LSAS_PATH_EDD_MRP */
#ifdef LSAS_CFG_USE_EDDI
    lsas_path_detail_struct.edd_ldb_mrp_edd.UsedComp.UseNRT                       = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_mrp_edd.UsedComp.UseXRT                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_mrp_edd.UsedComp.UseSWITCH                    = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_mrp_edd.UsedComp.UseSYNC                      = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_mrp_edd.UsedComp.UsePRM                       = LSA_FALSE;
  /*lsas_path_detail_struct.edd_ldb_mrp_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_mrp_edd.InsertSrcMAC                          = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_mrp_edd.pNRT                                  = &lsas_path_detail_struct.edd_nrt_mrp;
    lsas_path_detail_struct.edd_ldb_mrp_edd.Prm_PortDataAdjustLesserCheckQuality  = LSA_FALSE;

    lsas_path_detail_struct.edd_nrt_mrp.FrameFilter          = EDD_NRT_FRAME_MRP;     /*  MRP Frame mit TLV=0x883E */
    lsas_path_detail_struct.edd_nrt_mrp.Channel              = EDDI_NRT_CHANEL_B_IF_0;
    lsas_path_detail_struct.edd_nrt_mrp.UseFrameTriggerUnit  = LSA_FALSE;
#endif
#ifdef LSAS_CFG_USE_EDDP
    lsas_path_detail_struct.edd_ldb_mrp_edd.UsedComp.UseNRT                       = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_mrp_edd.UsedComp.UseCRT                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_mrp_edd.UsedComp.UseK32                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_mrp_edd.UsedComp.UsePRM                       = LSA_FALSE;
  /*lsas_path_detail_struct.edd_ldb_mrp_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_mrp_edd.pNRT                                  = &lsas_path_detail_struct.edd_nrt_mrp;
    lsas_path_detail_struct.edd_ldb_mrp_edd.pCRT                                  = LSA_NULL;
    lsas_path_detail_struct.edd_ldb_mrp_edd.pPRM                                  = LSA_NULL;

    lsas_path_detail_struct.edd_nrt_mrp.FrameFilter          = EDD_NRT_FRAME_MRP;     /*  MRP Frame mit TLV=0x883E */
#endif
#ifdef LSAS_CFG_USE_EDDS
    lsas_path_detail_struct.edd_ldb_mrp_edd.UsedComp.UseNRT                            = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_mrp_edd.UsedComp.UseCSRT                      = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_mrp_edd.UsedComp.UsePRM                            = LSA_FALSE;
 /* lsas_path_detail_struct.edd_ldb_mrp_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_mrp_edd.InsertSrcMAC                          = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_mrp_edd.pNRT                                  = &lsas_path_detail_struct.edd_nrt_mrp;
    lsas_path_detail_struct.edd_ldb_mrp_edd.pPRM                                  = LSA_NULL;

    lsas_path_detail_struct.edd_nrt_mrp.FrameFilter                              = EDD_NRT_FRAME_MRP; /*  MRP Frame mit TLV=0x883E */
#endif
    /* LSAS_PATH_EDD_NARE */
#ifdef LSAS_CFG_USE_EDDI
    lsas_path_detail_struct.edd_ldb_nare_edd.UsedComp.UseNRT                       = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_nare_edd.UsedComp.UseXRT                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_nare_edd.UsedComp.UseSWITCH                    = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_nare_edd.UsedComp.UseSYNC                      = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_nare_edd.UsedComp.UsePRM                       = LSA_FALSE;
  /*lsas_path_detail_struct.edd_ldb_nare_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_nare_edd.InsertSrcMAC                          = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_nare_edd.pNRT                                  = &lsas_path_detail_struct.edd_nrt_nare;
    lsas_path_detail_struct.edd_ldb_nare_edd.Prm_PortDataAdjustLesserCheckQuality  = LSA_FALSE;

    lsas_path_detail_struct.edd_nrt_nare.FrameFilter               = 0x00000000; /* nothing to receive */
    lsas_path_detail_struct.edd_nrt_nare.Channel                   = EDDI_NRT_CHANEL_A_IF_0;
    lsas_path_detail_struct.edd_nrt_nare.UseFrameTriggerUnit       = LSA_FALSE;
#endif
#ifdef LSAS_CFG_USE_EDDP
    lsas_path_detail_struct.edd_ldb_nare_edd.UsedComp.UseNRT                       = LSA_TRUE;    /* NRT for sending ARPs, see PCIOX */
    lsas_path_detail_struct.edd_ldb_nare_edd.UsedComp.UseCRT                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_nare_edd.UsedComp.UseK32                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_nare_edd.UsedComp.UsePRM                       = LSA_FALSE;
  /*lsas_path_detail_struct.edd_ldb_nare_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_nare_edd.pCRT                                  = LSA_NULL;
    lsas_path_detail_struct.edd_ldb_nare_edd.pNRT                                  = &lsas_path_detail_struct.edd_nrt_nare;
    lsas_path_detail_struct.edd_ldb_nare_edd.pPRM                                  = LSA_NULL;

    lsas_path_detail_struct.edd_nrt_nare.FrameFilter = 0; /* nothing to receive */
#endif
#ifdef LSAS_CFG_USE_EDDS
    lsas_path_detail_struct.edd_ldb_nare_edd.UsedComp.UseNRT                      = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_nare_edd.UsedComp.UseCSRT                      = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_nare_edd.UsedComp.UsePRM                      = LSA_FALSE;
 /* lsas_path_detail_struct.edd_ldb_nare_edd.hDDB                                 = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_nare_edd.InsertSrcMAC                          = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_nare_edd.pNRT                                  = &lsas_path_detail_struct.edd_nrt_nare;
    lsas_path_detail_struct.edd_ldb_nare_edd.pPRM                                  = LSA_NULL;

    lsas_path_detail_struct.edd_nrt_nare.FrameFilter                              = 0;
#endif

    /* LSAS_PATH_EDD_POF_CM */
#ifdef LSAS_CFG_USE_POF
#ifdef LSAS_CFG_USE_EDDI
    lsas_path_detail_struct.edd_ldb_pof_edd.UsedComp.UseNRT                       = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_pof_edd.UsedComp.UseXRT                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_pof_edd.UsedComp.UseSWITCH                    = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_pof_edd.UsedComp.UseSYNC                      = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_pof_edd.UsedComp.UsePRM                       = LSA_FALSE;
  /*lsas_path_detail_struct.edd_ldb_pof_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_pof_edd.InsertSrcMAC                          = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_pof_edd.pNRT                                  = &lsas_path_detail_struct.edd_nrt_pof;
    lsas_path_detail_struct.edd_ldb_pof_edd.Prm_PortDataAdjustLesserCheckQuality  = LSA_FALSE;

    lsas_path_detail_struct.edd_nrt_pof.FrameFilter              = EDD_NRT_FRAME_OTHER;   /*  Type-Frames not IP/DCP/LLDP/xRT */
    lsas_path_detail_struct.edd_nrt_pof.Channel                  = EDDI_NRT_CHANEL_A_IF_0;
    lsas_path_detail_struct.edd_nrt_pof.UseFrameTriggerUnit      = LSA_FALSE;
#endif
#ifdef LSAS_CFG_USE_EDDP
    lsas_path_detail_struct.edd_ldb_pof_edd.UsedComp.UseNRT                       = LSA_FALSE;    /* according to PCIOX */
    lsas_path_detail_struct.edd_ldb_pof_edd.UsedComp.UseCRT                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_pof_edd.UsedComp.UseK32                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_pof_edd.UsedComp.UsePRM                       = LSA_FALSE;
  /*lsas_path_detail_struct.edd_ldb_pof_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_pof_edd.pCRT                                  = LSA_NULL;
    lsas_path_detail_struct.edd_ldb_pof_edd.pNRT                                  = LSA_NULL;
    lsas_path_detail_struct.edd_ldb_pof_edd.pPRM                                  = LSA_NULL;
#endif
#ifdef LSAS_CFG_USE_EDDS
    /* not possible with edds */
#endif
    lsas_path_detail_struct.pof_ldb_cm_cm.path_type              = CM_PATH_TYPE_POF;
    lsas_path_detail_struct.pof_ldb_cm_cm.nic_id                 = EDD_INTERFACE_ID_MIN;
    lsas_path_detail_struct.pof_ldb_cm_cm.u.pof.nothing_so_far   = 0;

    {
        /* portID is the logical portID of the bus adaptor; it's not the hardware portID! */
        LSA_UINT32 portID;

        for(portID = 0; portID < EDD_CFG_MAX_PORT_CNT; portID++)
        {
            if(portID < LSAS_CFG_PORT_CNT)
            {
                LSA_UINT32 hwPortID;           /* according to Profinet stack numbering scheme: 1-based index */
                hwPortID = LSAS_CFG_GET_HW_PORT_IDX(portID)+1/*idx -> number*/;

                if (LSAS_PORT_IS_POF(hwPortID))
                {
                    lsas_path_detail_struct.pof_ldb_cm_pof.I2CAvailable[portID]    = POF_I2C_AVAILABLE;
                    #ifdef LSAS_CFG_USE_EDDI
                       /* external I2C-multiplexer is used, set I2CMuxSelect to 1..2 according to portID */
                       lsas_path_detail_struct.pof_ldb_cm_pof.I2CMuxSelect[portID] = portID + 1;
                    #else
                    /* if no external I2C-multiplexer is used, set I2CMuxSelect to 0 */
                    lsas_path_detail_struct.pof_ldb_cm_pof.I2CMuxSelect[portID]   = 0;
                    #endif

                }
                else
                {
                    lsas_path_detail_struct.pof_ldb_cm_pof.I2CAvailable[portID]       = POF_I2C_NOT_AVAILABLE;
                }
            }
            else
            {
                lsas_path_detail_struct.pof_ldb_cm_pof.I2CAvailable[portID]       = POF_I2C_NOT_AVAILABLE;
            }
        }
    }

#endif
    /* LSAS_PATH_EDD_CM */
#ifdef LSAS_CFG_USE_EDDI
    lsas_path_detail_struct.edd_ldb_cm_edd.UsedComp.UseNRT                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_cm_edd.UsedComp.UseXRT                       = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_cm_edd.UsedComp.UseSWITCH                    = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_cm_edd.UsedComp.UseSYNC                      = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_cm_edd.UsedComp.UsePRM                       = LSA_TRUE;
  /*lsas_path_detail_struct.edd_ldb_cm_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_cm_edd.InsertSrcMAC                          = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_cm_edd.pNRT                                  = NIL;
    lsas_path_detail_struct.edd_ldb_cm_edd.Prm_PortDataAdjustLesserCheckQuality  = LSA_FALSE;
#endif
#ifdef LSAS_CFG_USE_EDDP
    lsas_path_detail_struct.edd_ldb_cm_edd.UsedComp.UseNRT                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_cm_edd.UsedComp.UseCRT                       = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_cm_edd.UsedComp.UseK32                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_cm_edd.UsedComp.UsePRM                       = LSA_TRUE;
  /*lsas_path_detail_struct.edd_ldb_cm_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_cm_edd.pNRT                                  = NIL;
    lsas_path_detail_struct.edd_ldb_cm_edd.pCRT                                  = &lsas_path_detail_struct.edd_crt_cm_edd;
    lsas_path_detail_struct.edd_ldb_cm_edd.pPRM                                  = &lsas_path_detail_struct.edd_prm_cm_edd;

    lsas_path_detail_struct.edd_prm_cm_edd.PortDataAdjustLesserCheckQuality      = LSA_FALSE;
    lsas_path_detail_struct.edd_crt_cm_edd.Dummy            = 0;
#endif
#ifdef LSAS_CFG_USE_EDDS
    lsas_path_detail_struct.edd_ldb_cm_edd.UsedComp.UseNRT                          = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_cm_edd.UsedComp.UseCSRT                         = LSA_TRUE; // TRUE according to PSI. Old value: LSA_FALSE
    lsas_path_detail_struct.edd_ldb_cm_edd.UsedComp.UsePRM                          = LSA_TRUE;
 /* lsas_path_detail_struct.edd_ldb_cm_edd.hDDB                                 = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_cm_edd.InsertSrcMAC                              = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_cm_edd.pNRT                                      = LSA_NULL;
    lsas_path_detail_struct.edd_ldb_cm_edd.pPRM                                      = &lsas_path_detail_struct.edd_prm_cm_edd;

    lsas_path_detail_struct.edd_prm_cm_edd.PortDataAdjustLesserCheckQuality                              = LSA_FALSE;
#endif
    /* LSAS_PATH_EDD_TCIP_ICMP */
#ifdef LSAS_CFG_USE_EDDI
    lsas_path_detail_struct.edd_ldb_tcip_icmp_edd.UsedComp.UseNRT                       = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_tcip_icmp_edd.UsedComp.UseXRT                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_icmp_edd.UsedComp.UseSWITCH                    = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_icmp_edd.UsedComp.UseSYNC                      = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_icmp_edd.UsedComp.UsePRM                       = LSA_FALSE;
  /*lsas_path_detail_struct.edd_ldb_tcip_icmp_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_tcip_icmp_edd.InsertSrcMAC                          = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_tcip_icmp_edd.pNRT                                  = &lsas_path_detail_struct.edd_nrt_icmp;
    lsas_path_detail_struct.edd_ldb_tcip_icmp_edd.Prm_PortDataAdjustLesserCheckQuality  = LSA_FALSE;

    lsas_path_detail_struct.edd_nrt_icmp.FrameFilter           = EDD_NRT_FRAME_IP_ICMP;
    lsas_path_detail_struct.edd_nrt_icmp.Channel               = EDDI_NRT_CHANEL_A_IF_0;
    lsas_path_detail_struct.edd_nrt_icmp.UseFrameTriggerUnit   = LSA_FALSE;
#endif
#ifdef LSAS_CFG_USE_EDDP
    lsas_path_detail_struct.edd_ldb_tcip_icmp_edd.UsedComp.UseNRT                       = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_tcip_icmp_edd.UsedComp.UseCRT                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_icmp_edd.UsedComp.UsePRM                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_icmp_edd.UsedComp.UseK32                       = LSA_FALSE;
  /*lsas_path_detail_struct.edd_ldb_tcip_icmp_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_tcip_icmp_edd.pNRT                                  = &lsas_path_detail_struct.edd_nrt_icmp;
    lsas_path_detail_struct.edd_ldb_tcip_icmp_edd.pCRT                                  = 0;
    lsas_path_detail_struct.edd_ldb_tcip_icmp_edd.pPRM                                  = 0;

    lsas_path_detail_struct.edd_nrt_icmp.FrameFilter           = EDD_NRT_FRAME_IP_ICMP;
#endif
#ifdef LSAS_CFG_USE_EDDS
    lsas_path_detail_struct.edd_ldb_tcip_icmp_edd.UsedComp.UseNRT                          = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_tcip_icmp_edd.UsedComp.UseCSRT                          = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_icmp_edd.UsedComp.UsePRM                          = LSA_FALSE;
 /* lsas_path_detail_struct.edd_ldb_tcip_icmp_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_tcip_icmp_edd.InsertSrcMAC                              = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_icmp_edd.pNRT                                      = &lsas_path_detail_struct.edd_nrt_icmp;
    lsas_path_detail_struct.edd_ldb_tcip_icmp_edd.pPRM                                      = LSA_NULL;

    lsas_path_detail_struct.edd_nrt_icmp.FrameFilter                              = EDD_NRT_FRAME_IP_ICMP;
#endif
    /* LSAS_PATH_EDD_TCIP_UDP */
#ifdef LSAS_CFG_USE_EDDI
    lsas_path_detail_struct.edd_ldb_tcip_udp_edd.UsedComp.UseNRT                       = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_tcip_udp_edd.UsedComp.UseXRT                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_udp_edd.UsedComp.UseSWITCH                    = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_udp_edd.UsedComp.UseSYNC                      = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_udp_edd.UsedComp.UsePRM                       = LSA_FALSE;
  /*lsas_path_detail_struct.edd_ldb_tcip_udp_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_tcip_udp_edd.InsertSrcMAC                          = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_tcip_udp_edd.pNRT                                  = &lsas_path_detail_struct.edd_nrt_udp;
    lsas_path_detail_struct.edd_ldb_tcip_udp_edd.Prm_PortDataAdjustLesserCheckQuality  = LSA_FALSE;

    lsas_path_detail_struct.edd_nrt_udp.FrameFilter           = EDD_NRT_FRAME_IP_UDP;
    lsas_path_detail_struct.edd_nrt_udp.Channel               = EDDI_NRT_CHANEL_A_IF_0;
    lsas_path_detail_struct.edd_nrt_udp.UseFrameTriggerUnit   = LSA_FALSE;
#endif
#ifdef LSAS_CFG_USE_EDDP
    lsas_path_detail_struct.edd_ldb_tcip_udp_edd.UsedComp.UseNRT                       = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_tcip_udp_edd.UsedComp.UseCRT                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_udp_edd.UsedComp.UsePRM                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_udp_edd.UsedComp.UseK32                       = LSA_FALSE;
  /*lsas_path_detail_struct.edd_ldb_tcip_udp_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_tcip_udp_edd.pNRT                                  = &lsas_path_detail_struct.edd_nrt_udp;
    lsas_path_detail_struct.edd_ldb_tcip_udp_edd.pCRT                                  = 0;
    lsas_path_detail_struct.edd_ldb_tcip_udp_edd.pPRM                                  = 0;

    lsas_path_detail_struct.edd_nrt_udp.FrameFilter                                    = EDD_NRT_FRAME_IP_UDP;
#endif
#ifdef LSAS_CFG_USE_EDDS
    lsas_path_detail_struct.edd_ldb_tcip_udp_edd.UsedComp.UseNRT                          = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_tcip_udp_edd.UsedComp.UseCSRT                          = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_udp_edd.UsedComp.UsePRM                          = LSA_FALSE;
 /* lsas_path_detail_struct.edd_ldb_tcip_udp_edd.hDDB                                   = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_tcip_udp_edd.InsertSrcMAC                              = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_udp_edd.pNRT                                      = &lsas_path_detail_struct.edd_nrt_udp;
    lsas_path_detail_struct.edd_ldb_tcip_udp_edd.pPRM                                      = LSA_NULL;

    lsas_path_detail_struct.edd_nrt_udp.FrameFilter                              = EDD_NRT_FRAME_IP_UDP;
#endif
    /* LSAS_PATH_EDD_TCIP_ARP */
#ifdef LSAS_CFG_USE_EDDI
    lsas_path_detail_struct.edd_ldb_tcip_arp_edd.UsedComp.UseNRT                       = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_tcip_arp_edd.UsedComp.UseXRT                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_arp_edd.UsedComp.UseSWITCH                    = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_arp_edd.UsedComp.UseSYNC                      = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_arp_edd.UsedComp.UsePRM                       = LSA_FALSE;
  /*lsas_path_detail_struct.edd_ldb_tcip_arp_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_tcip_arp_edd.InsertSrcMAC                          = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_tcip_arp_edd.pNRT                                  = &lsas_path_detail_struct.edd_nrt_arp;
    lsas_path_detail_struct.edd_ldb_tcip_arp_edd.Prm_PortDataAdjustLesserCheckQuality  = LSA_FALSE;

    lsas_path_detail_struct.edd_nrt_arp.FrameFilter           = EDD_NRT_FRAME_ARP;
    lsas_path_detail_struct.edd_nrt_arp.Channel               = EDDI_NRT_CHANEL_A_IF_0;
    lsas_path_detail_struct.edd_nrt_arp.UseFrameTriggerUnit   = LSA_FALSE;
#endif
#ifdef LSAS_CFG_USE_EDDP
    lsas_path_detail_struct.edd_ldb_tcip_arp_edd.UsedComp.UseNRT                       = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_tcip_arp_edd.UsedComp.UseCRT                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_arp_edd.UsedComp.UsePRM                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_arp_edd.UsedComp.UseK32                       = LSA_FALSE;
  /*lsas_path_detail_struct.edd_ldb_tcip_arp_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_tcip_arp_edd.pNRT                                  = &lsas_path_detail_struct.edd_nrt_arp;
    lsas_path_detail_struct.edd_ldb_tcip_arp_edd.pCRT                                  = 0;
    lsas_path_detail_struct.edd_ldb_tcip_arp_edd.pPRM                                  = 0;

    lsas_path_detail_struct.edd_nrt_arp.FrameFilter           = EDD_NRT_FRAME_ARP;
#endif
#ifdef LSAS_CFG_USE_EDDS
    lsas_path_detail_struct.edd_ldb_tcip_arp_edd.UsedComp.UseNRT                          = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_tcip_arp_edd.UsedComp.UseCSRT                          = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_arp_edd.UsedComp.UsePRM                          = LSA_FALSE;
 /* lsas_path_detail_struct.edd_ldb_tcip_arp_edd.hDDB                                   = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_tcip_arp_edd.InsertSrcMAC                              = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_arp_edd.pNRT                                      = &lsas_path_detail_struct.edd_nrt_arp;
    lsas_path_detail_struct.edd_ldb_tcip_arp_edd.pPRM                                      = LSA_NULL;

    lsas_path_detail_struct.edd_nrt_arp.FrameFilter                              = EDD_NRT_FRAME_ARP;
#endif
    /* LSAS_PATH_EDD_TCIP_TCP */
#ifdef LSAS_CFG_USE_EDDI
    lsas_path_detail_struct.edd_ldb_tcip_tcp_edd.UsedComp.UseNRT                       = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_tcip_tcp_edd.UsedComp.UseXRT                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_tcp_edd.UsedComp.UseSWITCH                    = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_tcp_edd.UsedComp.UseSYNC                      = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_tcp_edd.UsedComp.UsePRM                       = LSA_FALSE;
  /*lsas_path_detail_struct.edd_ldb_tcip_tcp_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_tcip_tcp_edd.InsertSrcMAC                          = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_tcip_tcp_edd.pNRT                                  = &lsas_path_detail_struct.edd_nrt_tcp;
    lsas_path_detail_struct.edd_ldb_tcip_tcp_edd.Prm_PortDataAdjustLesserCheckQuality  = LSA_FALSE;

    lsas_path_detail_struct.edd_nrt_tcp.FrameFilter           = EDD_NRT_FRAME_IP_TCP;
    lsas_path_detail_struct.edd_nrt_tcp.Channel               = EDDI_NRT_CHANEL_A_IF_0;
    lsas_path_detail_struct.edd_nrt_tcp.UseFrameTriggerUnit   = LSA_FALSE;
#endif
#ifdef LSAS_CFG_USE_EDDP
    lsas_path_detail_struct.edd_ldb_tcip_tcp_edd.UsedComp.UseNRT                       = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_tcip_tcp_edd.UsedComp.UseCRT                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_tcp_edd.UsedComp.UsePRM                       = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_tcp_edd.UsedComp.UseK32                       = LSA_FALSE;
    /*lsas_path_detail_struct.edd_ldb_tcip_tcp_edd.hDDB                                  = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_tcip_tcp_edd.pNRT                                  = &lsas_path_detail_struct.edd_nrt_tcp;
    lsas_path_detail_struct.edd_ldb_tcip_tcp_edd.pCRT                                  = 0;
    lsas_path_detail_struct.edd_ldb_tcip_tcp_edd.pPRM                                  = 0;

    lsas_path_detail_struct.edd_nrt_tcp.FrameFilter           = EDD_NRT_FRAME_IP_TCP;
#endif
#ifdef LSAS_CFG_USE_EDDS
    lsas_path_detail_struct.edd_ldb_tcip_tcp_edd.UsedComp.UseNRT                          = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_tcip_tcp_edd.UsedComp.UseCSRT                          = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_tcp_edd.UsedComp.UsePRM                          = LSA_FALSE;
 /* lsas_path_detail_struct.edd_ldb_tcip_tcp_edd.hDDB                                   = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_tcip_tcp_edd.InsertSrcMAC                              = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_tcip_tcp_edd.pNRT                                      = &lsas_path_detail_struct.edd_nrt_tcp;
    lsas_path_detail_struct.edd_ldb_tcip_tcp_edd.pPRM                                      = LSA_NULL;

    lsas_path_detail_struct.edd_nrt_tcp.FrameFilter                              = EDD_NRT_FRAME_IP_TCP;
#endif
    /* sys to edd ldb ============================================================*/

    /* LSAS_PATH_EDD_SYS */
#ifdef LSAS_CFG_USE_EDDI
    lsas_path_detail_struct.edd_ldb_sys_edd.hDDB               = /*lsas_path_detail_struct.edd_ldb_sys_edd.hDDB*/ 0;
    lsas_path_detail_struct.edd_ldb_sys_edd.InsertSrcMAC       = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_sys_edd.pNRT               = 0;
    lsas_path_detail_struct.edd_ldb_sys_edd.UsedComp.UseNRT    = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_sys_edd.UsedComp.UsePRM    = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_sys_edd.UsedComp.UseSWITCH = LSA_TRUE;
    lsas_path_detail_struct.edd_ldb_sys_edd.UsedComp.UseSYNC   = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_sys_edd.UsedComp.UseXRT    = LSA_FALSE;
#endif
#ifdef LSAS_CFG_USE_EDDP
    /*lsas_path_detail_struct.edd_ldb_sys_edd.hDDB               = lsas_path_detail_struct.edd_ldb_sys_edd.hDDB 0;*/
    lsas_path_detail_struct.edd_ldb_sys_edd.pNRT               = 0;
    lsas_path_detail_struct.edd_ldb_sys_edd.pCRT               = 0;
    lsas_path_detail_struct.edd_ldb_sys_edd.pPRM               = 0;
    lsas_path_detail_struct.edd_ldb_sys_edd.UsedComp.UseNRT    = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_sys_edd.UsedComp.UsePRM    = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_sys_edd.UsedComp.UseCRT    = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_sys_edd.UsedComp.UseK32    = LSA_FALSE;

#endif
#ifdef LSAS_CFG_USE_EDDS
    lsas_path_detail_struct.edd_ldb_sys_edd.UsedComp.UseNRT                              = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_sys_edd.UsedComp.UseCSRT                          = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_sys_edd.UsedComp.UsePRM                              = LSA_FALSE;
 /* lsas_path_detail_struct.edd_ldb_sys_edd.hDDB                                    = 0; inserted later after opening device */
    lsas_path_detail_struct.edd_ldb_sys_edd.InsertSrcMAC                              = LSA_FALSE;
    lsas_path_detail_struct.edd_ldb_sys_edd.pNRT                                      = LSA_NULL;
    lsas_path_detail_struct.edd_ldb_sys_edd.pPRM                                      = LSA_NULL;
#endif
    /* fs ldb's =============================================================*/

    /* fs_open_device( detail_ptr) */

    /* LSAS_PATH_FS_APMA */
    /* LSAS_PATH_FS_FWUP */

    /* use fix data, look not in boot_static_data */
#ifdef LSAS_CFG_USE_FS
    lsas_path_detail_struct.fs_ldb_flash_0.rem_start_addr = (unsigned char *)IM_FLASH_BASE_ADDRESS/*apma_data.boot_static_data_ptr->fs_flash_device_ptr !!! ???*/;

    lsas_path_detail_struct.fs_ldb_flash_0.header_version = 0/*apma_data.boot_static_data_ptr->fs_firmware_block_header_version !!! ???*/;

    lsas_path_detail_struct.fs_ldb_flash_0.partition[ LSAS_FS_PARTITION_LOC_DATA].active_block  = (FS_LOWER_UINT8_PTR) LSAS_FS_LOC_DATA_ACTIVE_BLOCK();
    lsas_path_detail_struct.fs_ldb_flash_0.partition[ LSAS_FS_PARTITION_LOC_DATA].passive_block = (FS_LOWER_UINT8_PTR) LSAS_FS_LOC_DATA_PASSIVE_BLOCK();
    lsas_path_detail_struct.fs_ldb_flash_0.partition[ LSAS_FS_PARTITION_LOC_DATA].length        = LSAS_FS_LOC_DATA_LENGTH();
    lsas_path_detail_struct.fs_ldb_flash_0.partition[ LSAS_FS_PARTITION_LOC_DATA].type          = FS_PART_USR;

    lsas_path_detail_struct.fs_ldb_flash_0.partition[ LSAS_FS_PARTITION_FWUP].active_block      = (FS_LOWER_UINT8_PTR) LSAS_FS_FWUP_ACTIVE_BLOCK();
    lsas_path_detail_struct.fs_ldb_flash_0.partition[ LSAS_FS_PARTITION_FWUP].passive_block     = (FS_LOWER_UINT8_PTR) LSAS_FS_FWUP_PASSIVE_BLOCK();
    lsas_path_detail_struct.fs_ldb_flash_0.partition[ LSAS_FS_PARTITION_FWUP].length            = LSAS_FS_FWUP_LENGTH();
    lsas_path_detail_struct.fs_ldb_flash_0.partition[ LSAS_FS_PARTITION_FWUP].type              = FS_PART_RAW;

    #ifdef LSAS_FS_PARTITION_LOC_DATA2
    lsas_path_detail_struct.fs_ldb_flash_0.partition[ LSAS_FS_PARTITION_LOC_DATA2].active_block  = (FS_LOWER_UINT8_PTR) LSAS_FS_LOC_DATA2_ACTIVE_BLOCK();
    lsas_path_detail_struct.fs_ldb_flash_0.partition[ LSAS_FS_PARTITION_LOC_DATA2].passive_block = (FS_LOWER_UINT8_PTR) LSAS_FS_LOC_DATA2_PASSIVE_BLOCK();
    lsas_path_detail_struct.fs_ldb_flash_0.partition[ LSAS_FS_PARTITION_LOC_DATA2].length        = LSAS_FS_LOC_DATA2_LENGTH();
    lsas_path_detail_struct.fs_ldb_flash_0.partition[ LSAS_FS_PARTITION_LOC_DATA2].type          = FS_PART_USR;
    #endif

#endif

    /* gsy ldb's =============================================================*/

    /* AMPA_PDB_LSA_PATH_EDD_GSY_SYS */

    lsas_path_detail_struct.edd_ldb_gsy_gsy_sys.PathType                                  = GSY_PATH_TYPE_SYNC;
    lsas_path_detail_struct.edd_ldb_gsy_gsy_sys.NicId                                     = EDD_INTERFACE_ID_MIN;

    /* AMPA_PDB_LSA_PATH_EDD_GSY_ANNO */

    lsas_path_detail_struct.edd_ldb_gsy_gsy_anno.PathType                                  = GSY_PATH_TYPE_ANNO;
    lsas_path_detail_struct.edd_ldb_gsy_gsy_anno.NicId                                     = EDD_INTERFACE_ID_MIN;



    /* LSAS_PATH_GSY_CM */

    lsas_path_detail_struct.gsy_ldb_cm_gsy.PathType                                   = GSY_PATH_TYPE_USR;
    lsas_path_detail_struct.gsy_ldb_cm_gsy.NicId                                      = EDD_INTERFACE_ID_MIN;


    /* mrp ldb's =============================================================*/

    /* AMPA_PDB_LSA_PATH_EDD_MRP */

    lsas_path_detail_struct.edd_ldb_mrp_mrp.PathType                                  = MRP_PATH_TYPE_EDD;
    lsas_path_detail_struct.edd_ldb_mrp_mrp.NicId                                     = EDD_INTERFACE_ID_MIN;

    /* AMPA_PDB_LSA_PATH_MRP_OHA */

    lsas_path_detail_struct.mrp_ldb_oha_mrp.PathType                                  = MRP_PATH_TYPE_USER;
    lsas_path_detail_struct.mrp_ldb_oha_mrp.NicId                                     = EDD_INTERFACE_ID_MIN;
    lsas_path_detail_struct.mrp_ldb_oha_mrp.type.user.isPrmInterface                  = LSA_FALSE;
    lsas_path_detail_struct.mrp_ldb_oha_mrp.type.user.PrioDefault                     = LSA_NULL;


    /* LSAS_PATH_MRP_CM */

    lsas_path_detail_struct.mrp_ldb_cm_mrp.PathType                                   = MRP_PATH_TYPE_USER;
    lsas_path_detail_struct.mrp_ldb_cm_mrp.NicId                                      = EDD_INTERFACE_ID_MIN;
    lsas_path_detail_struct.mrp_ldb_cm_mrp.type.user.isPrmInterface                   = LSA_TRUE;
    lsas_path_detail_struct.mrp_ldb_cm_mrp.type.user.PrioDefault                      = 0xA000;

    /* nare ldb's ==========================================================*/

    /* PMA_PDB_LSA_PATH_DCP_NARE */

    lsas_path_detail_struct.dcp_ldb_nare_nare.PathType                        = NARE_PATH_TYPE_DCP;

    lsas_path_detail_struct.dcp_ldb_nare_nare.Params.Dcp.DCPIdentifyRetryCnt  = 0;
    lsas_path_detail_struct.dcp_ldb_nare_nare.Params.Dcp.DCPSetRetryCnt       = 0;
    lsas_path_detail_struct.dcp_ldb_nare_nare.Params.Dcp.MaxRDCP              = 1;
    lsas_path_detail_struct.dcp_ldb_nare_nare.Params.Dcp.NICId                = EDD_INTERFACE_ID_MIN;

    /* LSAS_PATH_EDD_NARE */

    lsas_path_detail_struct.edd_ldb_nare_nare.PathType                        = NARE_PATH_TYPE_EDD;

    lsas_path_detail_struct.edd_ldb_nare_nare.Params.Edd.ARPSniffTime         = 1000;
    lsas_path_detail_struct.edd_ldb_nare_nare.Params.Edd.ARPRetryCnt          = 2;            /* spezifies the additional ARPs that */
                                                                                     /* will be send if no response recv.  */
                                                                                     /* in "NARE_OPC_TEST_IP" or           */
                                                                                     /* in "NARE_OPC_RESOLVE_IP".          */
                                                                                     /* 1..x.                              */
                                                                                     /* delays are spezified in multiple of 100ms       */
    lsas_path_detail_struct.edd_ldb_nare_nare.Params.Edd.ARPSniffTimeout1     = 2;            /* delay until the next ARP is send (odd retries)  */
    lsas_path_detail_struct.edd_ldb_nare_nare.Params.Edd.ARPSniffTimeout2     = 1;            /* delay until the next ARP is send (even retries) */
    lsas_path_detail_struct.edd_ldb_nare_nare.Params.Edd.MaxREDD              = 1;
    lsas_path_detail_struct.edd_ldb_nare_nare.Params.Edd.NICId                = EDD_INTERFACE_ID_MIN;

    /* LSAS_PATH_NARE_OHA*/

    lsas_path_detail_struct.nare_ldb_oha_nare.PathType                        = NARE_PATH_TYPE_USER;

    lsas_path_detail_struct.nare_ldb_oha_nare.Params.User.MaxRUser            = 1;
    lsas_path_detail_struct.nare_ldb_oha_nare.Params.User.NICId               = EDD_INTERFACE_ID_MIN;

    /* LSAS_PATH_NARE_CM*/

    lsas_path_detail_struct.nare_ldb_cm_nare.PathType                         = NARE_PATH_TYPE_USER;

    lsas_path_detail_struct.nare_ldb_cm_nare.Params.User.MaxRUser             = 1;
    lsas_path_detail_struct.nare_ldb_cm_nare.Params.User.NICId                = EDD_INTERFACE_ID_MIN;

    /* lldp ldb's ==========================================================*/

    /* LSAS_PATH_EDD_LLDP_OHA */

    //lsas_path_detail_struct.edd_lldp_ldb_oha_lldp.PORTMaxSender                       = 8; /* how many MAC's must be managed */
    lsas_path_detail_struct.edd_lldp_ldb_oha_lldp.dummy                       = 0;

    /* oha ldb's ===========================================================*/

    /* LSAS_PATH_EDD_OHA */

    lsas_path_detail_struct.edd_ldb_oha_oha.PathType                           = OHA_PATH_TYPE_EDD;
    lsas_path_detail_struct.edd_ldb_oha_oha.Params.Edd.NICId                   = EDD_INTERFACE_ID_MIN;
    lsas_path_detail_struct.edd_ldb_oha_oha.Params.Edd.NoS_allow_upper_cases   = LSA_TRUE;

    /* LSAS_PATH_SOCK_OHA */

    lsas_path_detail_struct.sock_ldb_oha_oha.PathType                          = OHA_PATH_TYPE_SOCK;
    lsas_path_detail_struct.sock_ldb_oha_oha.Params.Sock.Reserved              = 0;

    /* LSAS_PATH_EDD_DCP_OHA */

    lsas_path_detail_struct.edd_dcp_ldb_oha_oha.PathType                           = OHA_PATH_TYPE_DCP;
    lsas_path_detail_struct.edd_dcp_ldb_oha_oha.Params.Dcp.NICId                   = EDD_INTERFACE_ID_MIN;
    lsas_path_detail_struct.edd_dcp_ldb_oha_oha.Params.Dcp.NumOfRecv           = DCP_NUM_SERVER_IDENTIFY_RESOURCES + DCP_NUM_SERVER_SET_RESOURCES + DCP_NUM_SERVER_GET_RESOURCES; /*5*/

    /* LSAS_PATH_EDD_LLDP_OHA */

    lsas_path_detail_struct.edd_lldp_ldb_oha_oha.PathType                      = OHA_PATH_TYPE_LLDP;
    lsas_path_detail_struct.edd_lldp_ldb_oha_oha.Params.Lldp.NICId             = EDD_INTERFACE_ID_MIN;
    lsas_path_detail_struct.edd_lldp_ldb_oha_oha.Params.Lldp.NumOfRecv         = 2;

    /* LSAS_PATH_MRP_OHA */

    lsas_path_detail_struct.mrp_ldb_oha_oha.PathType                           = OHA_PATH_TYPE_MRP;
    lsas_path_detail_struct.mrp_ldb_oha_oha.Params.Mrp.NICId                   = EDD_INTERFACE_ID_MIN;


    /* LSAS_PATH_NARE_OHA */

    lsas_path_detail_struct.nare_ldb_oha_oha.PathType                          = OHA_PATH_TYPE_NARE;
    lsas_path_detail_struct.nare_ldb_oha_oha.Params.Nare.CheckIpEnabled        = LSA_FALSE;
    lsas_path_detail_struct.nare_ldb_oha_oha.Params.Nare.NICId                 = EDD_INTERFACE_ID_MIN;

    /* LSAS_PATH_OHASV_CM */

     lsas_path_detail_struct.oha_ldb_cm_oha.PathType                            = OHA_PATH_TYPE_USER;
     lsas_path_detail_struct.oha_ldb_cm_oha.Params.User.DcpIndicationFilter     = OHA_CTRL_RESET_TO_FACTORY;
         /* according to Profinet V2.3, we have to support DCP suboption "SuboptionResetToFactory" that
            allows to reset communication parameters. */
     lsas_path_detail_struct.oha_ldb_cm_oha.Params.User.DcpIndicationFilter    |= OHA_CTRL_RESET_COMMUNICATION ;
     lsas_path_detail_struct.oha_ldb_cm_oha.Params.User.RemaStationFormat       = OHA_REMA_STATION_FORMAT_ADVANCED;
     lsas_path_detail_struct.oha_ldb_cm_oha.Params.User.NICId                   = EDD_INTERFACE_ID_MIN;
     lsas_path_detail_struct.oha_ldb_cm_oha.Params.User.allow_non_pnio_mautypes = LSA_FALSE; /* must be FALSE in PN Devices */
     lsas_path_detail_struct.oha_ldb_cm_oha.Params.User.CheckTopology           = LSA_TRUE;
     lsas_path_detail_struct.oha_ldb_cm_oha.Params.User.AllowOverlappingSubnet  = LSA_FALSE;

    /* LSAS_PATH_OHA_SYS */
     lsas_path_detail_struct.oha_ldb_sys_oha.PathType                            = OHA_PATH_TYPE_USER;
     lsas_path_detail_struct.oha_ldb_sys_oha.Params.User.DcpIndicationFilter     = 0;
     lsas_path_detail_struct.oha_ldb_sys_oha.Params.User.RemaStationFormat       = OHA_REMA_STATION_FORMAT_ADVANCED;
     lsas_path_detail_struct.oha_ldb_sys_oha.Params.User.NICId                   = EDD_INTERFACE_ID_MIN;
     lsas_path_detail_struct.oha_ldb_sys_oha.Params.User.allow_non_pnio_mautypes = 0; /* must be FALSE in PN Devices */
     lsas_path_detail_struct.oha_ldb_sys_oha.Params.User.AllowOverlappingSubnet  = LSA_FALSE;

    /* sock ldb's ===========================================================*/

    /* LSAS_PATH_EDD_CLRPC_CM */

    lsas_path_detail_struct.edd_clrpc_ldb_cm_sock.socket_option.SO_REUSEADDR_       = LSA_TRUE; /* allow local address reuse */
    lsas_path_detail_struct.edd_clrpc_ldb_cm_sock.socket_option.SO_DONTROUTE_       = LSA_FALSE;/* just use interface addresses */
    lsas_path_detail_struct.edd_clrpc_ldb_cm_sock.socket_option.SO_SNDBUF_          = LSA_FALSE;/* send buffer size */
    lsas_path_detail_struct.edd_clrpc_ldb_cm_sock.socket_option.SO_RCVBUF_          = LSA_FALSE;/* receive buffer size */
    lsas_path_detail_struct.edd_clrpc_ldb_cm_sock.socket_option.SO_LINGER_          = LSA_FALSE;/* socket lingers on close */
    lsas_path_detail_struct.edd_clrpc_ldb_cm_sock.socket_option.SO_TCPNODELAY_      = LSA_FALSE;/* delay sending data (Nagle algorithm) */
    lsas_path_detail_struct.edd_clrpc_ldb_cm_sock.socket_option.SO_TCPNOACKDELAY_   = LSA_FALSE;/* delay sending ACKs */
    lsas_path_detail_struct.edd_clrpc_ldb_cm_sock.socket_option.SO_BROADCAST_       = LSA_FALSE;/* broadcast allowed */

    lsas_path_detail_struct.edd_clrpc_ldb_cm_sock.send_buffer_max_len         =
    lsas_path_detail_struct.edd_clrpc_ldb_cm_sock.rec_buffer_max_len          =
    lsas_path_detail_struct.edd_clrpc_ldb_cm_sock.sock_close_at_once          =
    lsas_path_detail_struct.edd_clrpc_ldb_cm_sock.linger_time                 = LSA_NULL;

    /* LSAS_PATH_SOCK_CLRPC */

#ifdef LSAS_CLRPC_QUEUE_LIMIT
    lsas_path_detail_struct.sock_ldb_clrpc.socket_option.SO_SNDBUF_         = LSA_TRUE; /* send buffer size */
    lsas_path_detail_struct.sock_ldb_clrpc.socket_option.SO_RCVBUF_         = LSA_TRUE; /* receive buffer size */
    lsas_path_detail_struct.sock_ldb_clrpc.send_buffer_max_len              = LSAS_CLRPC_QUEUE_LIMIT;
    lsas_path_detail_struct.sock_ldb_clrpc.rec_buffer_max_len               = LSAS_CLRPC_QUEUE_LIMIT;
#else
    lsas_path_detail_struct.sock_ldb_clrpc.socket_option.SO_SNDBUF_         = LSA_FALSE; /* send buffer size */
    lsas_path_detail_struct.sock_ldb_clrpc.socket_option.SO_RCVBUF_         = LSA_FALSE; /* receive buffer size */
    lsas_path_detail_struct.sock_ldb_clrpc.send_buffer_max_len              = LSA_NULL;  /* use default 4kB */
    lsas_path_detail_struct.sock_ldb_clrpc.rec_buffer_max_len               = LSA_NULL;  /* use default 4kB */
#endif
    lsas_path_detail_struct.sock_ldb_clrpc.socket_option.SO_REUSEADDR_      = LSA_TRUE; /* allow local address reuse */
    lsas_path_detail_struct.sock_ldb_clrpc.socket_option.SO_DONTROUTE_      = LSA_FALSE;/* just use interface addresses */
    lsas_path_detail_struct.sock_ldb_clrpc.socket_option.SO_SNDBUF_         = LSA_FALSE;/* send buffer size */
    lsas_path_detail_struct.sock_ldb_clrpc.socket_option.SO_RCVBUF_         = LSA_FALSE;/* receive buffer size */
    lsas_path_detail_struct.sock_ldb_clrpc.socket_option.SO_LINGER_         = LSA_FALSE;/* socket lingers on close */
    lsas_path_detail_struct.sock_ldb_clrpc.socket_option.SO_TCPNODELAY_     = LSA_FALSE;/* delay sending data (Nagle algorithm) */
    lsas_path_detail_struct.sock_ldb_clrpc.socket_option.SO_TCPNOACKDELAY_  = LSA_FALSE;/* delay sending ACKs */
    lsas_path_detail_struct.sock_ldb_clrpc.socket_option.SO_BROADCAST_      = LSA_FALSE;/* broadcast allowed */

    lsas_path_detail_struct.sock_ldb_clrpc.send_buffer_max_len         =
    lsas_path_detail_struct.sock_ldb_clrpc.rec_buffer_max_len          =
    lsas_path_detail_struct.sock_ldb_clrpc.sock_close_at_once          =
    lsas_path_detail_struct.sock_ldb_clrpc.linger_time                 = LSA_NULL;


    /* LSAS_PATH_SOCK_OHA */

    lsas_path_detail_struct.sock_ldb_oha_sock.socket_option.SO_REUSEADDR_     = LSA_TRUE; /* allow local address reuse */
    lsas_path_detail_struct.sock_ldb_oha_sock.socket_option.SO_DONTROUTE_     = LSA_FALSE;/* just use interface addresses */
    lsas_path_detail_struct.sock_ldb_oha_sock.socket_option.SO_SNDBUF_        = LSA_FALSE;/* send buffer size */
    lsas_path_detail_struct.sock_ldb_oha_sock.socket_option.SO_RCVBUF_        = LSA_FALSE;/* receive buffer size */
    lsas_path_detail_struct.sock_ldb_oha_sock.socket_option.SO_LINGER_        = LSA_FALSE;/* socket lingers on close */
    lsas_path_detail_struct.sock_ldb_oha_sock.socket_option.SO_TCPNODELAY_    = LSA_TRUE;/* delayed ACK */
    lsas_path_detail_struct.sock_ldb_oha_sock.socket_option.SO_TCPNOACKDELAY_ = LSA_TRUE;/* delay sending ACKs */
    lsas_path_detail_struct.sock_ldb_oha_sock.socket_option.SO_BROADCAST_     = LSA_FALSE;/* broadcast allowed */

    lsas_path_detail_struct.sock_ldb_oha_sock.send_buffer_max_len         =
    lsas_path_detail_struct.sock_ldb_oha_sock.rec_buffer_max_len          =
    lsas_path_detail_struct.sock_ldb_oha_sock.sock_close_at_once          =
    lsas_path_detail_struct.sock_ldb_oha_sock.linger_time                 = LSA_NULL;


    /* LSAS_PATH_SOCK_SYS */

    lsas_path_detail_struct.sock_ldb_sys_sock.socket_option.SO_REUSEADDR_       = LSA_TRUE; /* allow local address reuse */
    lsas_path_detail_struct.sock_ldb_sys_sock.socket_option.SO_DONTROUTE_       = LSA_FALSE;/* just use interface addresses */
    lsas_path_detail_struct.sock_ldb_sys_sock.socket_option.SO_SNDBUF_          = LSA_FALSE;/* send buffer size */
    lsas_path_detail_struct.sock_ldb_sys_sock.socket_option.SO_RCVBUF_          = LSA_FALSE;/* receive buffer size */
    lsas_path_detail_struct.sock_ldb_sys_sock.socket_option.SO_LINGER_          = LSA_FALSE;/* socket lingers on close */
    lsas_path_detail_struct.sock_ldb_sys_sock.socket_option.SO_TCPNODELAY_      = LSA_TRUE;/* delayed ACK */
    lsas_path_detail_struct.sock_ldb_sys_sock.socket_option.SO_TCPNOACKDELAY_   = LSA_TRUE;/* delay sending ACKs */
    lsas_path_detail_struct.sock_ldb_sys_sock.socket_option.SO_BROADCAST_       = LSA_FALSE;/* broadcast allowed */

    lsas_path_detail_struct.sock_ldb_sys_sock.send_buffer_max_len         =
    lsas_path_detail_struct.sock_ldb_sys_sock.rec_buffer_max_len          =
    lsas_path_detail_struct.sock_ldb_sys_sock.sock_close_at_once          =
    lsas_path_detail_struct.sock_ldb_sys_sock.linger_time                 = LSA_NULL;


#ifdef LSAS_CFG_USE_STCP
    /* LSAS_PATH_SOCK_STCP */

    lsas_path_detail_struct.sock_ldb_s7pn_sock.socket_option.SO_REUSEADDR_      = LSA_FALSE; /* allow local address reuse */
    lsas_path_detail_struct.sock_ldb_s7pn_sock.socket_option.SO_DONTROUTE_      = LSA_FALSE;/* just use interface addresses */
    lsas_path_detail_struct.sock_ldb_s7pn_sock.socket_option.SO_SNDBUF_         = LSA_TRUE;/* send buffer size */
    lsas_path_detail_struct.sock_ldb_s7pn_sock.socket_option.SO_RCVBUF_         = LSA_TRUE;/* receive buffer size */
    lsas_path_detail_struct.sock_ldb_s7pn_sock.socket_option.SO_LINGER_         = LSA_TRUE;/* socket lingers on close */
    lsas_path_detail_struct.sock_ldb_s7pn_sock.socket_option.SO_TCPNODELAY_     = LSA_TRUE;/* delayed ACK */
    lsas_path_detail_struct.sock_ldb_s7pn_sock.socket_option.SO_TCPNOACKDELAY_  = LSA_FALSE;/* delay sending ACKs */
    lsas_path_detail_struct.sock_ldb_s7pn_sock.socket_option.SO_BROADCAST_      = LSA_FALSE;/* broadcast allowed */

    lsas_path_detail_struct.sock_ldb_s7pn_sock.send_buffer_max_len         = 4096;
    lsas_path_detail_struct.sock_ldb_s7pn_sock.rec_buffer_max_len          = 4096;
    lsas_path_detail_struct.sock_ldb_s7pn_sock.sock_close_at_once          = LSA_TRUE;
    lsas_path_detail_struct.sock_ldb_s7pn_sock.linger_time                 = 0;
#endif /* LSAS_CFG_USE_STCP */



#ifdef LSAS_CFG_USE_STCP
    /* stcp ldb's ===========================================================*/

    /* APMA_PDB_LSA_PATH_SOCK_STCP review  - werte */

    lsas_path_detail_struct.sock_ldb_s7pn_stcp.default_timeout                = 40; /* Default value for connection setup timer, specific values  given with CONNECT */
    lsas_path_detail_struct.sock_ldb_s7pn_stcp.receive_buffer_size            = 0; /* Length for an UPPER-MEM buffer for receive data,  for RCV_MODE_REQ_WITHOUT_BUFFER and */
                                                                             /* RCV_MODE_NO_REQ it is allocated with this buffer length */
//  lsas_path_detail_struct.sock_ldb_stcp_stcp.max_low_user_data_segment_size = APMA_PDB_S7DT_MAX_TPDU_SIZE;    /* Maximum size of the net dat (i. e. without header, */
                                                                             /* which is added by the layer 4 component ), which can be */
                                                                             /* passed in a job to the lower-layer components */
    lsas_path_detail_struct.sock_ldb_s7pn_stcp.cr_delay                       = 1; /* runtime for the timer with which a new active */
                                                                             /* connection setup is delayed */
                                                                             /* STPC_NO_TIMEOUT can also be specified, then the */
                                                                             /* connection is recreated immediately */
    lsas_path_detail_struct.sock_ldb_s7pn_stcp.listen_port                    = 102; /* Port number of the listen port for the lower-layer interface */
                                                                             /* is also used for the active connection setup */
//  lsas_path_detail_struct.sock_ldb_stcp_stcp.device_id                      = 0; /* Device_id for this channel */
    lsas_path_detail_struct.sock_ldb_s7pn_stcp.receive_mode                   = STCP_RCV_MODE_REQ_WITH_BUFFER /* 1 */ ; /* for the constants to be used: see above */
    lsas_path_detail_struct.sock_ldb_s7pn_stcp.rfc_tpdu_size                  = STCP_TPDU_SIZE_1024_OCTETS ; /* for the constants to be used: see above */
                                                                            /* proposed maximum TPDU size (in octets including header) */
                                                                            /* to be used over the transport connections */
                                                                            /* Only the constants defined above are allowed! */
                                                    /* max_low_user_data_size     value from common L4 interface, is not used */
                                                    /*                            this value is redundant to the value rfc_tpdu_size */
    lsas_path_detail_struct.sock_ldb_s7pn_stcp.listen_channel                 = LSA_TRUE;  /* LSA_TRUE ==> Identification  for the channel on which a listen */
                                                                             /*              is to be passed down */
                                                                             /*              then the parameters backlog and accept_number have to be filled */
                                                                             /* LSA_FALSE ==> Channel is passed down but no LISTEN is sent */
                                                                             /*              then the parameters backlog and accept_number are  */
                                                                             /*               irrelevant and have to be set to 0 */
    lsas_path_detail_struct.sock_ldb_s7pn_stcp.backlog                        = 1; /* Parameter for LISTEN job: how many connection setup requests */
                                                                             /* are accepted simultaneously by the lower layer */
    lsas_path_detail_struct.sock_ldb_s7pn_stcp.accept_number                  = 1; /* Number of the ACCEPT-NOTIFY jobs that are to stand below at the same time */



#endif /* LSAS_CFG_USE_STCP */



#ifdef LSAS_CFG_USE_STCP
        /* s7pn ldb's ===========================================================*/
   lsas_path_detail_struct.sock_ldb_s7pn_s7pn.dummy_number                  = 1; /* for future */
#endif /* LSAS_CFG_USE_STCP */

    /* tcip ldb's ===========================================================*/

    /* LSAS_PATH_EDD_TCIP_ICMP */

    lsas_path_detail_struct.edd_ldb_tcip_icmp_tcip.path_type                   = TCIP_PATH_TYPE_ICMP;
    lsas_path_detail_struct.edd_ldb_tcip_icmp_tcip.group_id                    = EDD_INTERFACE_ID_MIN;
    lsas_path_detail_struct.edd_ldb_tcip_icmp_tcip.number_of_recv_resources    = 10;
    lsas_path_detail_struct.edd_ldb_tcip_icmp_tcip.number_of_send_resources    = 0;



    /* LSAS_PATH_EDD_TCIP_UDP */

    lsas_path_detail_struct.edd_ldb_tcip_udp_tcip.path_type                   = TCIP_PATH_TYPE_UDP;
    lsas_path_detail_struct.edd_ldb_tcip_udp_tcip.group_id                    = EDD_INTERFACE_ID_MIN;
    lsas_path_detail_struct.edd_ldb_tcip_udp_tcip.number_of_recv_resources    = 98;
    lsas_path_detail_struct.edd_ldb_tcip_udp_tcip.number_of_send_resources    = 0;


    /* LSAS_PATH_EDD_TCIP_ARP */

    lsas_path_detail_struct.edd_ldb_tcip_arp_tcip.path_type                   = TCIP_PATH_TYPE_ARP;
    lsas_path_detail_struct.edd_ldb_tcip_arp_tcip.group_id                    = EDD_INTERFACE_ID_MIN;
    lsas_path_detail_struct.edd_ldb_tcip_arp_tcip.number_of_recv_resources    = 20;
    lsas_path_detail_struct.edd_ldb_tcip_arp_tcip.number_of_send_resources    = 40; /* AP01265427 */
                                                                                    /* edd_ldb_tcip_icmp_tcip.number_of_recv_resources +
                                                                                     * edd_ldb_tcip_arp_tcip.number_of_recv_resources + 10 for udp responses */



    /* LSAS_PATH_EDD_TCIP_TCP */

    lsas_path_detail_struct.edd_ldb_tcip_tcp_tcip.path_type                   = TCIP_PATH_TYPE_TCP;
    lsas_path_detail_struct.edd_ldb_tcip_tcp_tcip.group_id                    = EDD_INTERFACE_ID_MIN;
    lsas_path_detail_struct.edd_ldb_tcip_tcp_tcip.number_of_recv_resources    = LSAS_CFG_TCIP_PATH_TYPE_TCP_RECV_RES_COUNT;
    lsas_path_detail_struct.edd_ldb_tcip_tcp_tcip.number_of_send_resources    = 0;


    /*=======================================================================*/


    /* init detail-info-used-signs of path-layer-matrix */
    {
        LSA_UINT32 tmp_count_0,tmp_count_1;
        for ( tmp_count_0 = 0; LSAS_PATH_NUM > tmp_count_0; tmp_count_0++ )
        {
            for ( tmp_count_1 = 0; LSAS_PATH_LAYER_NUM > tmp_count_1; tmp_count_1++ )
            {
                lsas_path_layer_matrix[ tmp_count_0][ tmp_count_1].detail_ptr       = NIL;
                lsas_path_layer_matrix[ tmp_count_0][ tmp_count_1].system_ptr.path  = LSAS_PATH_NUM;
                lsas_path_layer_matrix[ tmp_count_0][ tmp_count_1].system_ptr.layer = LSAS_PATH_LAYER_NUM;
                lsas_path_layer_matrix[ tmp_count_0][ tmp_count_1].used             = TRUE;
            }
        }
    }

    lsas_com_init_path_layer_matrix();

}

void lsas_com_init_path_layer_matrix(void)
{
    /*=======================================================================*/

    /* fill path-layer-matrix */

    /* fs paths =============================================================*/
#ifdef LSAS_CFG_USE_FS
    /* LSAS_PATH_FS_APMA */

    lsas_path_layer_matrix[ LSAS_PATH_FS_APMA][ LSAS_PATH_LAYER_FS].detail_ptr          = &lsas_path_detail_struct.fs_ldb_flash_0;
    lsas_path_layer_matrix[ LSAS_PATH_FS_APMA][ LSAS_PATH_LAYER_FS].system_ptr.path     = LSAS_PATH_FS_APMA;
    lsas_path_layer_matrix[ LSAS_PATH_FS_APMA][ LSAS_PATH_LAYER_FS].system_ptr.layer    = LSAS_PATH_LAYER_FS;
    lsas_path_layer_matrix[ LSAS_PATH_FS_APMA][ LSAS_PATH_LAYER_FS].used                = FALSE;

    /* LSAS_PATH_FS_FWUM */

    lsas_path_layer_matrix[ LSAS_PATH_FS_FWUP][ LSAS_PATH_LAYER_FS].detail_ptr          = &lsas_path_detail_struct.fs_ldb_flash_0;
    lsas_path_layer_matrix[ LSAS_PATH_FS_FWUP][ LSAS_PATH_LAYER_FS].system_ptr.path     = LSAS_PATH_FS_FWUP;
    lsas_path_layer_matrix[ LSAS_PATH_FS_FWUP][ LSAS_PATH_LAYER_FS].system_ptr.layer    = LSAS_PATH_LAYER_FS;
    lsas_path_layer_matrix[ LSAS_PATH_FS_FWUP][ LSAS_PATH_LAYER_FS].used                = FALSE;

    /* LSAS_PATH_FS_IUA */

    lsas_path_layer_matrix[ LSAS_PATH_FS_IUM][ LSAS_PATH_LAYER_FS].detail_ptr          = &lsas_path_detail_struct.fs_ldb_flash_0;
    lsas_path_layer_matrix[ LSAS_PATH_FS_IUM][ LSAS_PATH_LAYER_FS].system_ptr.path     = LSAS_PATH_FS_IUM;
    lsas_path_layer_matrix[ LSAS_PATH_FS_IUM][ LSAS_PATH_LAYER_FS].system_ptr.layer    = LSAS_PATH_LAYER_FS;
    lsas_path_layer_matrix[ LSAS_PATH_FS_IUM][ LSAS_PATH_LAYER_FS].used                = FALSE;
#endif
    /* acp paths ============================================================*/

    /* LSAS_PATH_ACP_CM */

    lsas_path_layer_matrix[ LSAS_PATH_ACP_CM][ LSAS_PATH_LAYER_ACP].detail_ptr         = &lsas_path_detail_struct.acp_ldb_cm_acp;
    lsas_path_layer_matrix[ LSAS_PATH_ACP_CM][ LSAS_PATH_LAYER_ACP].system_ptr.path    = LSAS_PATH_ACP_CM;
    lsas_path_layer_matrix[ LSAS_PATH_ACP_CM][ LSAS_PATH_LAYER_ACP].system_ptr.layer   = LSAS_PATH_LAYER_ACP;
    lsas_path_layer_matrix[ LSAS_PATH_ACP_CM][ LSAS_PATH_LAYER_ACP].used               = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_ACP_CM][ LSAS_PATH_LAYER_CM].detail_ptr          = &lsas_path_detail_struct.acp_ldb_cm_cm;
    lsas_path_layer_matrix[ LSAS_PATH_ACP_CM][ LSAS_PATH_LAYER_CM].system_ptr.path     = LSAS_PATH_ACP_CM;
    lsas_path_layer_matrix[ LSAS_PATH_ACP_CM][ LSAS_PATH_LAYER_CM].system_ptr.layer    = LSAS_PATH_LAYER_CM;
    lsas_path_layer_matrix[ LSAS_PATH_ACP_CM][ LSAS_PATH_LAYER_CM].used                = FALSE;

    /* cm paths =============================================================*/

    lsas_path_layer_matrix[ LSAS_PATH_CMPDSV][ LSAS_PATH_LAYER_CM].detail_ptr          = &lsas_path_detail_struct.cmpdsv_ldb_cm_cm;
    lsas_path_layer_matrix[ LSAS_PATH_CMPDSV][ LSAS_PATH_LAYER_CM].system_ptr.path     = LSAS_PATH_CMPDSV;
    lsas_path_layer_matrix[ LSAS_PATH_CMPDSV][ LSAS_PATH_LAYER_CM].system_ptr.layer    = LSAS_PATH_LAYER_CM;
    lsas_path_layer_matrix[ LSAS_PATH_CMPDSV][ LSAS_PATH_LAYER_CM].used                = FALSE;

    /* dcp paths ============================================================*/

    lsas_path_layer_matrix[ LSAS_PATH_DCP_NARE][ LSAS_PATH_LAYER_DCP].detail_ptr          = &lsas_path_detail_struct.dcp_ldb_nare_dcp;
    lsas_path_layer_matrix[ LSAS_PATH_DCP_NARE][ LSAS_PATH_LAYER_DCP].system_ptr.path     = LSAS_PATH_DCP_NARE;
    lsas_path_layer_matrix[ LSAS_PATH_DCP_NARE][ LSAS_PATH_LAYER_DCP].system_ptr.layer    = LSAS_PATH_LAYER_DCP;
    lsas_path_layer_matrix[ LSAS_PATH_DCP_NARE][ LSAS_PATH_LAYER_DCP].used                = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_DCP_NARE][ LSAS_PATH_LAYER_NARE].detail_ptr          = &lsas_path_detail_struct.dcp_ldb_nare_nare;
    lsas_path_layer_matrix[ LSAS_PATH_DCP_NARE][ LSAS_PATH_LAYER_NARE].system_ptr.path     = LSAS_PATH_DCP_NARE;
    lsas_path_layer_matrix[ LSAS_PATH_DCP_NARE][ LSAS_PATH_LAYER_NARE].system_ptr.layer    = LSAS_PATH_LAYER_NARE;
    lsas_path_layer_matrix[ LSAS_PATH_DCP_NARE][ LSAS_PATH_LAYER_NARE].used                = FALSE;

    /* edd paths ============================================================*/

    /* LSAS_PATH_EDD_SYS */

    lsas_path_layer_matrix[ LSAS_PATH_EDD_SYS ][LSAS_PATH_LAYER_EDD].detail_ptr         = &lsas_path_detail_struct.edd_ldb_sys_edd;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_SYS][ LSAS_PATH_LAYER_EDD].system_ptr.path    = LSAS_PATH_EDD_SYS;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_SYS][ LSAS_PATH_LAYER_EDD].system_ptr.layer   = LSAS_PATH_LAYER_EDD;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_SYS][ LSAS_PATH_LAYER_EDD].used               = FALSE;

    /* LSAS_PATH_EDD_ACP */

    lsas_path_layer_matrix[ LSAS_PATH_EDD_ACP][ LSAS_PATH_LAYER_EDD].detail_ptr         = &lsas_path_detail_struct.edd_ldb_acp_edd;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_ACP][ LSAS_PATH_LAYER_EDD].system_ptr.path    = LSAS_PATH_EDD_ACP;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_ACP][ LSAS_PATH_LAYER_EDD].system_ptr.layer   = LSAS_PATH_LAYER_EDD;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_ACP][ LSAS_PATH_LAYER_EDD].used               = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_EDD_ACP][ LSAS_PATH_LAYER_ACP].detail_ptr         = &lsas_path_detail_struct.edd_ldb_acp_acp;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_ACP][ LSAS_PATH_LAYER_ACP].system_ptr.path    = LSAS_PATH_EDD_ACP;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_ACP][ LSAS_PATH_LAYER_ACP].system_ptr.layer   = LSAS_PATH_LAYER_ACP;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_ACP][ LSAS_PATH_LAYER_ACP].used               = FALSE;

    /* LSAS_PATH_EDD_CM */

    lsas_path_layer_matrix[ LSAS_PATH_EDD_CM][ LSAS_PATH_LAYER_EDD].detail_ptr         = &lsas_path_detail_struct.edd_ldb_cm_edd;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_CM][ LSAS_PATH_LAYER_EDD].system_ptr.path    = LSAS_PATH_EDD_CM;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_CM][ LSAS_PATH_LAYER_EDD].system_ptr.layer   = LSAS_PATH_LAYER_EDD;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_CM][ LSAS_PATH_LAYER_EDD].used               = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_EDD_CM][ LSAS_PATH_LAYER_CM].detail_ptr         = &lsas_path_detail_struct.edd_ldb_cm_cm;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_CM][ LSAS_PATH_LAYER_CM].system_ptr.path    = LSAS_PATH_EDD_CM;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_CM][ LSAS_PATH_LAYER_CM].system_ptr.layer   = LSAS_PATH_LAYER_CM;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_CM][ LSAS_PATH_LAYER_CM].used               = FALSE;

    /* LSAS_PATH_EDD_DCP_OHA */

    lsas_path_layer_matrix[ LSAS_PATH_EDD_DCP_OHA][ LSAS_PATH_LAYER_EDD].detail_ptr          = &lsas_path_detail_struct.edd_dcp_ldb_oha_edd;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_DCP_OHA][ LSAS_PATH_LAYER_EDD].system_ptr.path     = LSAS_PATH_EDD_DCP_OHA;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_DCP_OHA][ LSAS_PATH_LAYER_EDD].system_ptr.layer    = LSAS_PATH_LAYER_EDD;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_DCP_OHA][ LSAS_PATH_LAYER_EDD].used                = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_EDD_DCP_OHA][ LSAS_PATH_LAYER_DCP].detail_ptr          = &lsas_path_detail_struct.edd_dcp_ldb_oha_dcp;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_DCP_OHA][ LSAS_PATH_LAYER_DCP].system_ptr.path     = LSAS_PATH_EDD_DCP_OHA;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_DCP_OHA][ LSAS_PATH_LAYER_DCP].system_ptr.layer    = LSAS_PATH_LAYER_DCP;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_DCP_OHA][ LSAS_PATH_LAYER_DCP].used                = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_EDD_DCP_OHA][ LSAS_PATH_LAYER_OHA].detail_ptr          = &lsas_path_detail_struct.edd_dcp_ldb_oha_oha;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_DCP_OHA][ LSAS_PATH_LAYER_OHA].system_ptr.path     = LSAS_PATH_EDD_DCP_OHA;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_DCP_OHA][ LSAS_PATH_LAYER_OHA].system_ptr.layer    = LSAS_PATH_LAYER_OHA;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_DCP_OHA][ LSAS_PATH_LAYER_OHA].used                = FALSE;

    /* LSAS_PATH_EDD_GSY_SYS */

    lsas_path_layer_matrix[ LSAS_PATH_EDD_GSY_SYS][ LSAS_PATH_LAYER_EDD].detail_ptr         = &lsas_path_detail_struct.edd_ldb_gsy_edd_sys;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_GSY_SYS][ LSAS_PATH_LAYER_EDD].system_ptr.path    = LSAS_PATH_EDD_GSY_SYS;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_GSY_SYS][ LSAS_PATH_LAYER_EDD].system_ptr.layer   = LSAS_PATH_LAYER_EDD;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_GSY_SYS][ LSAS_PATH_LAYER_EDD].used               = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_EDD_GSY_SYS][ LSAS_PATH_LAYER_GSY].detail_ptr         = &lsas_path_detail_struct.edd_ldb_gsy_gsy_sys;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_GSY_SYS][ LSAS_PATH_LAYER_GSY].system_ptr.path    = LSAS_PATH_EDD_GSY_SYS;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_GSY_SYS][ LSAS_PATH_LAYER_GSY].system_ptr.layer   = LSAS_PATH_LAYER_GSY;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_GSY_SYS][ LSAS_PATH_LAYER_GSY].used               = FALSE;

    /* LSAS_PATH_EDD_GSY_ANNO */

    lsas_path_layer_matrix[ LSAS_PATH_EDD_GSY_ANNO][ LSAS_PATH_LAYER_EDD].detail_ptr         = &lsas_path_detail_struct.edd_ldb_gsy_edd_anno;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_GSY_ANNO][ LSAS_PATH_LAYER_EDD].system_ptr.path    = LSAS_PATH_EDD_GSY_ANNO;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_GSY_ANNO][ LSAS_PATH_LAYER_EDD].system_ptr.layer   = LSAS_PATH_LAYER_EDD;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_GSY_ANNO][ LSAS_PATH_LAYER_EDD].used               = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_EDD_GSY_ANNO][ LSAS_PATH_LAYER_GSY].detail_ptr         = &lsas_path_detail_struct.edd_ldb_gsy_gsy_anno;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_GSY_ANNO][ LSAS_PATH_LAYER_GSY].system_ptr.path    = LSAS_PATH_EDD_GSY_ANNO;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_GSY_ANNO][ LSAS_PATH_LAYER_GSY].system_ptr.layer   = LSAS_PATH_LAYER_GSY;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_GSY_ANNO][ LSAS_PATH_LAYER_GSY].used               = FALSE;


    /* LSAS_PATH_EDD_MRP */

    lsas_path_layer_matrix[ LSAS_PATH_EDD_MRP][ LSAS_PATH_LAYER_EDD].detail_ptr         = &lsas_path_detail_struct.edd_ldb_mrp_edd;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_MRP][ LSAS_PATH_LAYER_EDD].system_ptr.path    = LSAS_PATH_EDD_MRP;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_MRP][ LSAS_PATH_LAYER_EDD].system_ptr.layer   = LSAS_PATH_LAYER_EDD;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_MRP][ LSAS_PATH_LAYER_EDD].used               = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_EDD_MRP][ LSAS_PATH_LAYER_MRP].detail_ptr         = &lsas_path_detail_struct.edd_ldb_mrp_mrp;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_MRP][ LSAS_PATH_LAYER_MRP].system_ptr.path    = LSAS_PATH_EDD_MRP;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_MRP][ LSAS_PATH_LAYER_MRP].system_ptr.layer   = LSAS_PATH_LAYER_MRP;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_MRP][ LSAS_PATH_LAYER_MRP].used               = FALSE;

    /* LSAS_PATH_EDD_NARE */

    lsas_path_layer_matrix[ LSAS_PATH_EDD_NARE][ LSAS_PATH_LAYER_EDD].detail_ptr         = &lsas_path_detail_struct.edd_ldb_nare_edd;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_NARE][ LSAS_PATH_LAYER_EDD].system_ptr.path    = LSAS_PATH_EDD_NARE;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_NARE][ LSAS_PATH_LAYER_EDD].system_ptr.layer   = LSAS_PATH_LAYER_EDD;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_NARE][ LSAS_PATH_LAYER_EDD].used               = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_EDD_NARE][ LSAS_PATH_LAYER_NARE].detail_ptr         = &lsas_path_detail_struct.edd_ldb_nare_nare;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_NARE][ LSAS_PATH_LAYER_NARE].system_ptr.path    = LSAS_PATH_EDD_NARE;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_NARE][ LSAS_PATH_LAYER_NARE].system_ptr.layer   = LSAS_PATH_LAYER_NARE;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_NARE][ LSAS_PATH_LAYER_NARE].used               = FALSE;

    /* LSAS_PATH_EDD_LLDP_OHA */

    lsas_path_layer_matrix[ LSAS_PATH_EDD_LLDP_OHA][ LSAS_PATH_LAYER_EDD].detail_ptr         = &lsas_path_detail_struct.edd_lldp_ldb_oha_edd;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_LLDP_OHA][ LSAS_PATH_LAYER_EDD].system_ptr.path    = LSAS_PATH_EDD_LLDP_OHA;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_LLDP_OHA][ LSAS_PATH_LAYER_EDD].system_ptr.layer   = LSAS_PATH_LAYER_EDD;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_LLDP_OHA][ LSAS_PATH_LAYER_EDD].used               = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_EDD_LLDP_OHA][ LSAS_PATH_LAYER_LLDP].detail_ptr         = &lsas_path_detail_struct.edd_lldp_ldb_oha_lldp;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_LLDP_OHA][ LSAS_PATH_LAYER_LLDP].system_ptr.path    = LSAS_PATH_EDD_LLDP_OHA;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_LLDP_OHA][ LSAS_PATH_LAYER_LLDP].system_ptr.layer   = LSAS_PATH_LAYER_LLDP;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_LLDP_OHA][ LSAS_PATH_LAYER_LLDP].used               = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_EDD_LLDP_OHA][ LSAS_PATH_LAYER_OHA].detail_ptr         = &lsas_path_detail_struct.edd_lldp_ldb_oha_oha;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_LLDP_OHA][ LSAS_PATH_LAYER_OHA].system_ptr.path    = LSAS_PATH_EDD_LLDP_OHA;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_LLDP_OHA][ LSAS_PATH_LAYER_OHA].system_ptr.layer   = LSAS_PATH_LAYER_OHA;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_LLDP_OHA][ LSAS_PATH_LAYER_OHA].used               = FALSE;


    /* LSAS_PATH_EDD_OHA */

    lsas_path_layer_matrix[ LSAS_PATH_EDD_OHA][ LSAS_PATH_LAYER_EDD].detail_ptr         = &lsas_path_detail_struct.edd_ldb_oha_edd;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_OHA][ LSAS_PATH_LAYER_EDD].system_ptr.path    = LSAS_PATH_EDD_OHA;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_OHA][ LSAS_PATH_LAYER_EDD].system_ptr.layer   = LSAS_PATH_LAYER_EDD;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_OHA][ LSAS_PATH_LAYER_EDD].used               = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_EDD_OHA][ LSAS_PATH_LAYER_OHA].detail_ptr         = &lsas_path_detail_struct.edd_ldb_oha_oha;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_OHA][ LSAS_PATH_LAYER_OHA].system_ptr.path    = LSAS_PATH_EDD_OHA;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_OHA][ LSAS_PATH_LAYER_OHA].system_ptr.layer   = LSAS_PATH_LAYER_OHA;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_OHA][ LSAS_PATH_LAYER_OHA].used               = FALSE;

    /* LSAS_PATH_EDD_TCIP_ICMP */

    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_ICMP][ LSAS_PATH_LAYER_EDD].detail_ptr         = &lsas_path_detail_struct.edd_ldb_tcip_icmp_edd;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_ICMP][ LSAS_PATH_LAYER_EDD].system_ptr.path    = LSAS_PATH_EDD_TCIP_ICMP;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_ICMP][ LSAS_PATH_LAYER_EDD].system_ptr.layer   = LSAS_PATH_LAYER_EDD;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_ICMP][ LSAS_PATH_LAYER_EDD].used               = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_ICMP][ LSAS_PATH_LAYER_TCIP].detail_ptr         = &lsas_path_detail_struct.edd_ldb_tcip_icmp_tcip;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_ICMP][ LSAS_PATH_LAYER_TCIP].system_ptr.path    = LSAS_PATH_EDD_TCIP_ICMP;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_ICMP][ LSAS_PATH_LAYER_TCIP].system_ptr.layer   = LSAS_PATH_LAYER_TCIP;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_ICMP][ LSAS_PATH_LAYER_TCIP].used               = FALSE;

    /* LSAS_PATH_EDD_TCIP_UDP */

    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_UDP][ LSAS_PATH_LAYER_EDD].detail_ptr         = &lsas_path_detail_struct.edd_ldb_tcip_udp_edd;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_UDP][ LSAS_PATH_LAYER_EDD].system_ptr.path    = LSAS_PATH_EDD_TCIP_UDP;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_UDP][ LSAS_PATH_LAYER_EDD].system_ptr.layer   = LSAS_PATH_LAYER_EDD;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_UDP][ LSAS_PATH_LAYER_EDD].used               = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_UDP][ LSAS_PATH_LAYER_TCIP].detail_ptr         = &lsas_path_detail_struct.edd_ldb_tcip_udp_tcip;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_UDP][ LSAS_PATH_LAYER_TCIP].system_ptr.path    = LSAS_PATH_EDD_TCIP_UDP;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_UDP][ LSAS_PATH_LAYER_TCIP].system_ptr.layer   = LSAS_PATH_LAYER_TCIP;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_UDP][ LSAS_PATH_LAYER_TCIP].used               = FALSE;

    /* LSAS_PATH_EDD_TCIP_ARP */

    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_ARP][ LSAS_PATH_LAYER_EDD].detail_ptr         = &lsas_path_detail_struct.edd_ldb_tcip_arp_edd;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_ARP][ LSAS_PATH_LAYER_EDD].system_ptr.path    = LSAS_PATH_EDD_TCIP_ARP;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_ARP][ LSAS_PATH_LAYER_EDD].system_ptr.layer   = LSAS_PATH_LAYER_EDD;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_ARP][ LSAS_PATH_LAYER_EDD].used               = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_ARP][ LSAS_PATH_LAYER_TCIP].detail_ptr         = &lsas_path_detail_struct.edd_ldb_tcip_arp_tcip;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_ARP][ LSAS_PATH_LAYER_TCIP].system_ptr.path    = LSAS_PATH_EDD_TCIP_ARP;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_ARP][ LSAS_PATH_LAYER_TCIP].system_ptr.layer   = LSAS_PATH_LAYER_TCIP;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_ARP][ LSAS_PATH_LAYER_TCIP].used               = FALSE;

    /* LSAS_PATH_EDD_TCIP_TCP */

    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_TCP][ LSAS_PATH_LAYER_EDD].detail_ptr         = &lsas_path_detail_struct.edd_ldb_tcip_tcp_edd;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_TCP][ LSAS_PATH_LAYER_EDD].system_ptr.path    = LSAS_PATH_EDD_TCIP_TCP;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_TCP][ LSAS_PATH_LAYER_EDD].system_ptr.layer   = LSAS_PATH_LAYER_EDD;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_TCP][ LSAS_PATH_LAYER_EDD].used               = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_TCP][ LSAS_PATH_LAYER_TCIP].detail_ptr         = &lsas_path_detail_struct.edd_ldb_tcip_tcp_tcip;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_TCP][ LSAS_PATH_LAYER_TCIP].system_ptr.path    = LSAS_PATH_EDD_TCIP_TCP;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_TCP][ LSAS_PATH_LAYER_TCIP].system_ptr.layer   = LSAS_PATH_LAYER_TCIP;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_TCIP_TCP][ LSAS_PATH_LAYER_TCIP].used               = FALSE;

    /* LSAS_PATH_EDD_CLRPC_CM */
    /* attention: this path is broken by design between sock and edd, therefore tcpip continues the path to edd */

    lsas_path_layer_matrix[ LSAS_PATH_EDD_CLRPC_CM][ LSAS_PATH_LAYER_SOCK].detail_ptr        = &lsas_path_detail_struct.edd_clrpc_ldb_cm_sock;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_CLRPC_CM][ LSAS_PATH_LAYER_SOCK].system_ptr.path   = LSAS_PATH_EDD_CLRPC_CM;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_CLRPC_CM][ LSAS_PATH_LAYER_SOCK].system_ptr.layer  = LSAS_PATH_LAYER_SOCK;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_CLRPC_CM][ LSAS_PATH_LAYER_SOCK].used              = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_EDD_CLRPC_CM][ LSAS_PATH_LAYER_CLRPC].detail_ptr       = &lsas_path_detail_struct.edd_clrpc_ldb_cm_clrpc;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_CLRPC_CM][ LSAS_PATH_LAYER_CLRPC].system_ptr.path  = LSAS_PATH_EDD_CLRPC_CM;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_CLRPC_CM][ LSAS_PATH_LAYER_CLRPC].system_ptr.layer = LSAS_PATH_LAYER_CLRPC;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_CLRPC_CM][ LSAS_PATH_LAYER_CLRPC].used             = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_EDD_CLRPC_CM][ LSAS_PATH_LAYER_CM].detail_ptr          = &lsas_path_detail_struct.edd_clrpc_ldb_cm_cm;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_CLRPC_CM][ LSAS_PATH_LAYER_CM].system_ptr.path     = LSAS_PATH_EDD_CLRPC_CM;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_CLRPC_CM][ LSAS_PATH_LAYER_CM].system_ptr.layer    = LSAS_PATH_LAYER_CM;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_CLRPC_CM][ LSAS_PATH_LAYER_CM].used                = FALSE;

    /* gsy paths ============================================================*/

    /* LSAS_PATH_GSY_CM */

    lsas_path_layer_matrix[ LSAS_PATH_GSY_CM][ LSAS_PATH_LAYER_GSY].detail_ptr          = &lsas_path_detail_struct.gsy_ldb_cm_gsy;
    lsas_path_layer_matrix[ LSAS_PATH_GSY_CM][ LSAS_PATH_LAYER_GSY].system_ptr.path     = LSAS_PATH_GSY_CM;
    lsas_path_layer_matrix[ LSAS_PATH_GSY_CM][ LSAS_PATH_LAYER_GSY].system_ptr.layer    = LSAS_PATH_LAYER_GSY;
    lsas_path_layer_matrix[ LSAS_PATH_GSY_CM][ LSAS_PATH_LAYER_GSY].used                = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_GSY_CM][ LSAS_PATH_LAYER_CM].detail_ptr           = &lsas_path_detail_struct.gsy_ldb_cm_cm;
    lsas_path_layer_matrix[ LSAS_PATH_GSY_CM][ LSAS_PATH_LAYER_CM].system_ptr.path      = LSAS_PATH_GSY_CM;
    lsas_path_layer_matrix[ LSAS_PATH_GSY_CM][ LSAS_PATH_LAYER_CM].system_ptr.layer     = LSAS_PATH_LAYER_CM;
    lsas_path_layer_matrix[ LSAS_PATH_GSY_CM][ LSAS_PATH_LAYER_CM].used                 = FALSE;

    /* mrp paths ============================================================*/

    /* LSAS_PATH_MRP_CM */

    lsas_path_layer_matrix[ LSAS_PATH_MRP_CM][ LSAS_PATH_LAYER_MRP].detail_ptr          = &lsas_path_detail_struct.mrp_ldb_cm_mrp;
    lsas_path_layer_matrix[ LSAS_PATH_MRP_CM][ LSAS_PATH_LAYER_MRP].system_ptr.path     = LSAS_PATH_MRP_CM;
    lsas_path_layer_matrix[ LSAS_PATH_MRP_CM][ LSAS_PATH_LAYER_MRP].system_ptr.layer    = LSAS_PATH_LAYER_MRP;
    lsas_path_layer_matrix[ LSAS_PATH_MRP_CM][ LSAS_PATH_LAYER_MRP].used                = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_MRP_CM][ LSAS_PATH_LAYER_CM].detail_ptr           = &lsas_path_detail_struct.mrp_ldb_cm_cm;
    lsas_path_layer_matrix[ LSAS_PATH_MRP_CM][ LSAS_PATH_LAYER_CM].system_ptr.path      = LSAS_PATH_MRP_CM;
    lsas_path_layer_matrix[ LSAS_PATH_MRP_CM][ LSAS_PATH_LAYER_CM].system_ptr.layer     = LSAS_PATH_LAYER_CM;
    lsas_path_layer_matrix[ LSAS_PATH_MRP_CM][ LSAS_PATH_LAYER_CM].used                 = FALSE;

    /* LSAS_PATH_MRP_OHA */

    lsas_path_layer_matrix[ LSAS_PATH_MRP_OHA][ LSAS_PATH_LAYER_MRP].detail_ptr         = &lsas_path_detail_struct.mrp_ldb_oha_mrp;
    lsas_path_layer_matrix[ LSAS_PATH_MRP_OHA][ LSAS_PATH_LAYER_MRP].system_ptr.path    = LSAS_PATH_MRP_OHA;
    lsas_path_layer_matrix[ LSAS_PATH_MRP_OHA][ LSAS_PATH_LAYER_MRP].system_ptr.layer   = LSAS_PATH_LAYER_MRP;
    lsas_path_layer_matrix[ LSAS_PATH_MRP_OHA][ LSAS_PATH_LAYER_MRP].used               = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_MRP_OHA][ LSAS_PATH_LAYER_OHA].system_ptr.path    = LSAS_PATH_MRP_OHA;
    lsas_path_layer_matrix[ LSAS_PATH_MRP_OHA][ LSAS_PATH_LAYER_OHA].detail_ptr         = &lsas_path_detail_struct.mrp_ldb_oha_oha;
    lsas_path_layer_matrix[ LSAS_PATH_MRP_OHA][ LSAS_PATH_LAYER_OHA].system_ptr.layer   = LSAS_PATH_LAYER_OHA;
    lsas_path_layer_matrix[ LSAS_PATH_MRP_OHA][ LSAS_PATH_LAYER_OHA].used               = FALSE;

    /* nare paths ===========================================================*/

    /* LSAS_PATH_NARE_OHA */

    lsas_path_layer_matrix[ LSAS_PATH_NARE_OHA][ LSAS_PATH_LAYER_NARE].detail_ptr          = &lsas_path_detail_struct.nare_ldb_oha_nare;
    lsas_path_layer_matrix[ LSAS_PATH_NARE_OHA][ LSAS_PATH_LAYER_NARE].system_ptr.path     = LSAS_PATH_NARE_OHA;
    lsas_path_layer_matrix[ LSAS_PATH_NARE_OHA][ LSAS_PATH_LAYER_NARE].system_ptr.layer    = LSAS_PATH_LAYER_NARE;
    lsas_path_layer_matrix[ LSAS_PATH_NARE_OHA][ LSAS_PATH_LAYER_NARE].used                = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_NARE_OHA][ LSAS_PATH_LAYER_OHA].detail_ptr           = &lsas_path_detail_struct.nare_ldb_oha_oha;
    lsas_path_layer_matrix[ LSAS_PATH_NARE_OHA][ LSAS_PATH_LAYER_OHA].system_ptr.path      = LSAS_PATH_NARE_OHA;
    lsas_path_layer_matrix[ LSAS_PATH_NARE_OHA][ LSAS_PATH_LAYER_OHA].system_ptr.layer     = LSAS_PATH_LAYER_OHA;
    lsas_path_layer_matrix[ LSAS_PATH_NARE_OHA][ LSAS_PATH_LAYER_OHA].used                 = FALSE;

    /* LSAS_PATH_NARE_CM */

    lsas_path_layer_matrix[ LSAS_PATH_NARE_CM][ LSAS_PATH_LAYER_NARE].detail_ptr          = &lsas_path_detail_struct.nare_ldb_cm_nare;
    lsas_path_layer_matrix[ LSAS_PATH_NARE_CM][ LSAS_PATH_LAYER_NARE].system_ptr.path     = LSAS_PATH_NARE_CM;
    lsas_path_layer_matrix[ LSAS_PATH_NARE_CM][ LSAS_PATH_LAYER_NARE].system_ptr.layer    = LSAS_PATH_LAYER_NARE;
    lsas_path_layer_matrix[ LSAS_PATH_NARE_CM][ LSAS_PATH_LAYER_NARE].used                = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_NARE_CM][ LSAS_PATH_LAYER_CM].detail_ptr            = &lsas_path_detail_struct.nare_ldb_cm_cm;
    lsas_path_layer_matrix[ LSAS_PATH_NARE_CM][ LSAS_PATH_LAYER_CM].system_ptr.path       = LSAS_PATH_NARE_CM;
    lsas_path_layer_matrix[ LSAS_PATH_NARE_CM][ LSAS_PATH_LAYER_CM].system_ptr.layer      = LSAS_PATH_LAYER_CM;
    lsas_path_layer_matrix[ LSAS_PATH_NARE_CM][ LSAS_PATH_LAYER_CM].used                  = FALSE;

    /* rema paths ===========================================================*/

    /* pof paths ============================================================*/

    /* LSAS_PATH_EDD_POF_CM */
#ifdef LSAS_CFG_USE_POF
    lsas_path_layer_matrix[ LSAS_PATH_EDD_POF_CM][ LSAS_PATH_LAYER_EDD].detail_ptr          = &lsas_path_detail_struct.edd_ldb_pof_edd;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_POF_CM][ LSAS_PATH_LAYER_EDD].system_ptr.path     = LSAS_PATH_EDD_POF_CM;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_POF_CM][ LSAS_PATH_LAYER_EDD].system_ptr.layer    = LSAS_PATH_LAYER_EDD;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_POF_CM][ LSAS_PATH_LAYER_EDD].used                = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_EDD_POF_CM][ LSAS_PATH_LAYER_POF].detail_ptr          = &lsas_path_detail_struct.pof_ldb_cm_pof;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_POF_CM][ LSAS_PATH_LAYER_POF].system_ptr.path     = LSAS_PATH_EDD_POF_CM;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_POF_CM][ LSAS_PATH_LAYER_POF].system_ptr.layer    = LSAS_PATH_LAYER_POF;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_POF_CM][ LSAS_PATH_LAYER_POF].used                = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_EDD_POF_CM][ LSAS_PATH_LAYER_CM].detail_ptr           = &lsas_path_detail_struct.pof_ldb_cm_cm;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_POF_CM][ LSAS_PATH_LAYER_CM].system_ptr.path      = LSAS_PATH_EDD_POF_CM;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_POF_CM][ LSAS_PATH_LAYER_CM].system_ptr.layer     = LSAS_PATH_LAYER_CM;
    lsas_path_layer_matrix[ LSAS_PATH_EDD_POF_CM][ LSAS_PATH_LAYER_CM].used                 = FALSE;
#endif
    /* oha paths ============================================================*/

    /* LSAS_PATH_OHASV_CM */

    lsas_path_layer_matrix[ LSAS_PATH_OHASV_CM][ LSAS_PATH_LAYER_OHA].detail_ptr          = &lsas_path_detail_struct.oha_ldb_cm_oha;
    lsas_path_layer_matrix[ LSAS_PATH_OHASV_CM][ LSAS_PATH_LAYER_OHA].system_ptr.path     = LSAS_PATH_OHASV_CM;
    lsas_path_layer_matrix[ LSAS_PATH_OHASV_CM][ LSAS_PATH_LAYER_OHA].system_ptr.layer    = LSAS_PATH_LAYER_OHA;
    lsas_path_layer_matrix[ LSAS_PATH_OHASV_CM][ LSAS_PATH_LAYER_OHA].used                = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_OHASV_CM][ LSAS_PATH_LAYER_CM].detail_ptr           = &lsas_path_detail_struct.oha_ldb_cm_cm;
    lsas_path_layer_matrix[ LSAS_PATH_OHASV_CM][ LSAS_PATH_LAYER_CM].system_ptr.path      = LSAS_PATH_OHASV_CM;
    lsas_path_layer_matrix[ LSAS_PATH_OHASV_CM][ LSAS_PATH_LAYER_CM].system_ptr.layer     = LSAS_PATH_LAYER_CM;
    lsas_path_layer_matrix[ LSAS_PATH_OHASV_CM][ LSAS_PATH_LAYER_CM].used                 = FALSE;

    /* LSAS_PATH_OHA_SYS */

    lsas_path_layer_matrix[ LSAS_PATH_OHA_SYS][ LSAS_PATH_LAYER_OHA].detail_ptr         = &lsas_path_detail_struct.oha_ldb_sys_oha;
    lsas_path_layer_matrix[ LSAS_PATH_OHA_SYS][ LSAS_PATH_LAYER_OHA].system_ptr.path    = LSAS_PATH_OHA_SYS;
    lsas_path_layer_matrix[ LSAS_PATH_OHA_SYS][ LSAS_PATH_LAYER_OHA].system_ptr.layer   = LSAS_PATH_LAYER_OHA;
    lsas_path_layer_matrix[ LSAS_PATH_OHA_SYS][ LSAS_PATH_LAYER_OHA].used               = FALSE;

    /* sock paths ============================================================*/

    /* LSAS_PATH_SOCK_CLRPC */

    lsas_path_layer_matrix[ LSAS_PATH_SOCK_CLRPC][ LSAS_PATH_LAYER_SOCK].detail_ptr          = &lsas_path_detail_struct.sock_ldb_clrpc;
    lsas_path_layer_matrix[ LSAS_PATH_SOCK_CLRPC][ LSAS_PATH_LAYER_SOCK].system_ptr.path     = LSAS_PATH_SOCK_CLRPC;
    lsas_path_layer_matrix[ LSAS_PATH_SOCK_CLRPC][ LSAS_PATH_LAYER_SOCK].system_ptr.layer    = LSAS_PATH_LAYER_SOCK;
    lsas_path_layer_matrix[ LSAS_PATH_SOCK_CLRPC][ LSAS_PATH_LAYER_SOCK].used                = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_SOCK_CLRPC][ LSAS_PATH_LAYER_CLRPC].detail_ptr         = &lsas_path_detail_struct.clrpc_ldb_clrpc;
    lsas_path_layer_matrix[ LSAS_PATH_SOCK_CLRPC][ LSAS_PATH_LAYER_CLRPC].system_ptr.path    = LSAS_PATH_SOCK_CLRPC;
    lsas_path_layer_matrix[ LSAS_PATH_SOCK_CLRPC][ LSAS_PATH_LAYER_CLRPC].system_ptr.layer   = LSAS_PATH_LAYER_CLRPC;
    lsas_path_layer_matrix[ LSAS_PATH_SOCK_CLRPC][ LSAS_PATH_LAYER_CLRPC].used               = FALSE;

    /* LSAS_PATH_SOCK_OHA */

    lsas_path_layer_matrix[ LSAS_PATH_SOCK_OHA][ LSAS_PATH_LAYER_SOCK].detail_ptr          = &lsas_path_detail_struct.sock_ldb_oha_sock;
    lsas_path_layer_matrix[ LSAS_PATH_SOCK_OHA][ LSAS_PATH_LAYER_SOCK].system_ptr.path     = LSAS_PATH_SOCK_OHA;
    lsas_path_layer_matrix[ LSAS_PATH_SOCK_OHA][ LSAS_PATH_LAYER_SOCK].system_ptr.layer    = LSAS_PATH_LAYER_SOCK;
    lsas_path_layer_matrix[ LSAS_PATH_SOCK_OHA][ LSAS_PATH_LAYER_SOCK].used                = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_SOCK_OHA][ LSAS_PATH_LAYER_OHA].detail_ptr           = &lsas_path_detail_struct.sock_ldb_oha_oha;
    lsas_path_layer_matrix[ LSAS_PATH_SOCK_OHA][ LSAS_PATH_LAYER_OHA].system_ptr.path      = LSAS_PATH_SOCK_OHA;
    lsas_path_layer_matrix[ LSAS_PATH_SOCK_OHA][ LSAS_PATH_LAYER_OHA].system_ptr.layer     = LSAS_PATH_LAYER_OHA;
    lsas_path_layer_matrix[ LSAS_PATH_SOCK_OHA][ LSAS_PATH_LAYER_OHA].used                 = FALSE;

    /* LSAS_PATH_SOCK_SYS */

    lsas_path_layer_matrix[ LSAS_PATH_SOCK_SYS][ LSAS_PATH_LAYER_SOCK].detail_ptr          = &lsas_path_detail_struct.sock_ldb_sys_sock;
    lsas_path_layer_matrix[ LSAS_PATH_SOCK_SYS][ LSAS_PATH_LAYER_SOCK].system_ptr.path     = LSAS_PATH_SOCK_SYS;
    lsas_path_layer_matrix[ LSAS_PATH_SOCK_SYS][ LSAS_PATH_LAYER_SOCK].system_ptr.layer    = LSAS_PATH_LAYER_SOCK;
    lsas_path_layer_matrix[ LSAS_PATH_SOCK_SYS][ LSAS_PATH_LAYER_SOCK].used                = FALSE;


    /* LSAS_PATH_SOCK_STCP */
#ifdef LSAS_CFG_USE_STCP

    lsas_path_layer_matrix[ LSAS_PATH_SOCK_STCP_S7PN][ LSAS_PATH_LAYER_SOCK].detail_ptr          = &lsas_path_detail_struct.sock_ldb_s7pn_sock;
    lsas_path_layer_matrix[ LSAS_PATH_SOCK_STCP_S7PN][ LSAS_PATH_LAYER_SOCK].system_ptr.path     = LSAS_PATH_SOCK_STCP_S7PN;
    lsas_path_layer_matrix[ LSAS_PATH_SOCK_STCP_S7PN][ LSAS_PATH_LAYER_SOCK].system_ptr.layer    = LSAS_PATH_LAYER_SOCK;
    lsas_path_layer_matrix[ LSAS_PATH_SOCK_STCP_S7PN][ LSAS_PATH_LAYER_SOCK].used                = FALSE;

    lsas_path_layer_matrix[ LSAS_PATH_SOCK_STCP_S7PN][ LSAS_PATH_LAYER_STCP].detail_ptr          = &lsas_path_detail_struct.sock_ldb_s7pn_stcp;
    lsas_path_layer_matrix[ LSAS_PATH_SOCK_STCP_S7PN][ LSAS_PATH_LAYER_STCP].system_ptr.path     = LSAS_PATH_SOCK_STCP_S7PN;
    lsas_path_layer_matrix[ LSAS_PATH_SOCK_STCP_S7PN][ LSAS_PATH_LAYER_STCP].system_ptr.layer    = LSAS_PATH_LAYER_STCP;
    lsas_path_layer_matrix[ LSAS_PATH_SOCK_STCP_S7PN][ LSAS_PATH_LAYER_STCP].used                = FALSE;

     lsas_path_layer_matrix[ LSAS_PATH_SOCK_STCP_S7PN][ LSAS_PATH_LAYER_S7PN].detail_ptr          = &lsas_path_detail_struct.sock_ldb_s7pn_s7pn;
     lsas_path_layer_matrix[ LSAS_PATH_SOCK_STCP_S7PN][ LSAS_PATH_LAYER_S7PN].system_ptr.path     = LSAS_PATH_SOCK_STCP_S7PN;
     lsas_path_layer_matrix[ LSAS_PATH_SOCK_STCP_S7PN][ LSAS_PATH_LAYER_S7PN].system_ptr.layer    = LSAS_PATH_LAYER_S7PN;
     lsas_path_layer_matrix[ LSAS_PATH_SOCK_STCP_S7PN][ LSAS_PATH_LAYER_S7PN].used                = FALSE;

#endif /* LSAS_CFG_USE_STCP */


}


LSA_VOID lsas_com_set_hxdb(LSAS_EDD_HANDLE hddb)
{
    lsas_data.edd_hddb                                      = hddb;
#if (defined LSAS_CFG_USE_EDDI) | (defined LSAS_CFG_USE_EDDS)
    lsas_path_detail_struct.edd_ldb_acp_edd.hDDB            =
    lsas_path_detail_struct.edd_ldb_tcip_icmp_edd.hDDB      =
    lsas_path_detail_struct.edd_ldb_tcip_udp_edd.hDDB       =
    lsas_path_detail_struct.edd_ldb_tcip_arp_edd.hDDB       =
    lsas_path_detail_struct.edd_ldb_tcip_tcp_edd.hDDB       =
    lsas_path_detail_struct.edd_dcp_ldb_oha_edd.hDDB        =
    lsas_path_detail_struct.edd_ldb_oha_edd.hDDB            =
    lsas_path_detail_struct.edd_lldp_ldb_oha_edd.hDDB       =
    lsas_path_detail_struct.edd_ldb_gsy_edd_sys.hDDB        =
    lsas_path_detail_struct.edd_ldb_gsy_edd_anno.hDDB       =
    lsas_path_detail_struct.edd_ldb_mrp_edd.hDDB            =
    lsas_path_detail_struct.edd_ldb_nare_edd.hDDB           =
#if defined LSAS_CFG_USE_POF
    lsas_path_detail_struct.edd_ldb_pof_edd.hDDB            =
#endif
    lsas_path_detail_struct.edd_ldb_cm_edd.hDDB             =
    lsas_path_detail_struct.edd_ldb_sys_edd.hDDB            = hddb;
#endif
#ifdef LSAS_CFG_USE_EDDP
    lsas_path_detail_struct.edd_ldb_acp_edd.hDDB            =
    lsas_path_detail_struct.edd_ldb_tcip_icmp_edd.hDDB      =
    lsas_path_detail_struct.edd_ldb_tcip_udp_edd.hDDB       =
    lsas_path_detail_struct.edd_ldb_tcip_arp_edd.hDDB       =
    lsas_path_detail_struct.edd_ldb_tcip_tcp_edd.hDDB       =
    lsas_path_detail_struct.edd_dcp_ldb_oha_edd.hDDB        =
    lsas_path_detail_struct.edd_ldb_oha_edd.hDDB            =
    lsas_path_detail_struct.edd_lldp_ldb_oha_edd.hDDB       =
    lsas_path_detail_struct.edd_ldb_gsy_edd_sys.hDDB        =
    lsas_path_detail_struct.edd_ldb_gsy_edd_anno.hDDB       =
    lsas_path_detail_struct.edd_ldb_mrp_edd.hDDB            =
    lsas_path_detail_struct.edd_ldb_nare_edd.hDDB           =
#if defined LSAS_CFG_USE_POF
    lsas_path_detail_struct.edd_ldb_pof_edd.hDDB            =
#endif
    lsas_path_detail_struct.edd_ldb_cm_edd.hDDB             =
    lsas_path_detail_struct.edd_ldb_sys_edd.hDDB            = hddb;
#endif

    lsas_data.edd_hxdb_set = LSA_TRUE;
}

LSAS_ATTR_CODE_FAST LSAS_EDD_HANDLE lsas_com_get_hddb(void)
{
    LSAS_EDD_HANDLE ret_val = LSA_NULL;

    if(lsas_data.edd_hxdb_set)
    {
        ret_val = lsas_data.edd_hddb;
    }
    else
    {
        //LSAS_FATAL(LSAS_FATAL_ERR_HANDLE_INVALID);
    }

    return ret_val;
}


LSAS_RSP lsas_com_get_path_info  (LSAS_SYSTEM_PTR_PTR system_ptr_ptr, LSAS_DETAIL_PTR *detail_ptr_ptr, UnsignedOpt path, UnsignedOpt layer)
{

    if (   (LSAS_PATH_NUM       >  path)
        && (LSAS_PATH_LAYER_NUM >  layer)
// do not check because of sock failure        && (FALSE                  == lsas_path_detail_struct_path_info[ path][ layer].used)
       )
    {
        lsas_path_layer_matrix[ path][ layer].used = TRUE;

        *system_ptr_ptr = lsas_path_layer_matrix[ path][ layer].system_ptr;
        *detail_ptr_ptr = lsas_path_layer_matrix[ path][ layer].detail_ptr;
    }
    else
    {
        LSAS_FATAL(LSAS_FATAL_ERR_GET_PATH_INFO);
    }

    return( LSAS_RSP_OK );
}


/*****************************************************************************/


LSAS_RSP lsas_com_release_path_info  (LSAS_SYSTEM_PTR system_ptr, LSAS_DETAIL_PTR detail_ptr)
{

    /* handle path <-> layer matrix */

    if (   (LSAS_PATH_NUM  >  system_ptr.path)
        && (LSAS_PATH_LAYER_NUM >  system_ptr.layer)
// do not check because of sock failure        && (FALSE                  != lsas_path_detail_struct_path_info[ system_ptr.path][ system_ptr.layer].used)
        && (detail_ptr == lsas_path_layer_matrix[ system_ptr.path][ system_ptr.layer].detail_ptr)
       )
    {
        lsas_path_layer_matrix[ system_ptr.path][ system_ptr.layer].used = FALSE;
    }
    else
    {
        LSAS_FATAL(LSAS_FATAL_ERR_GET_PATH_INFO);
    }


    return( LSAS_RSP_OK );
}

LSA_UINT16 lsas_get_path_info  (LSAS_SYSTEM_PTR_PTR system_ptr_ptr, LSAS_DETAIL_PTR *detail_ptr_ptr, LSAS_PATH_TYPE path, LSAS_LAYER_TYPE layer)
{
    if (   (LSAS_PATH_NUM  >  path)
        && (LSAS_PATH_LAYER_NUM >  layer)
// do not check because of sock failure        && (FALSE                  == lsas_path_detail_struct_path_info[ path][ layer].used)
       )
    {
        lsas_path_layer_matrix[ path][ layer].used = TRUE;

        *system_ptr_ptr = lsas_path_layer_matrix[ path][ layer].system_ptr;
        *detail_ptr_ptr = lsas_path_layer_matrix[ path][ layer].detail_ptr;
    }
    else
    {
        lsas_fatal_error( LSAS_MODULE_ID, (LSA_UINT16)__LINE__, 0);
    }

    return( LSAS_RSP_OK);
}

LSA_UINT16 lsas_release_path_info  (LSAS_SYSTEM_PTR system_ptr, LSAS_DETAIL_PTR detail_ptr)
{

    /* handle path <-> layer matrix */

    if (   (LSAS_PATH_NUM  >  system_ptr.path)
        && (LSAS_PATH_LAYER_NUM >  system_ptr.layer)
// do not check because of sock failure        && (FALSE                  != lsas_path_detail_struct_path_info[ system_ptr.path][ system_ptr.layer].used)
//!        && (apma_host_ptr_are_equal__( detail_ptr, lsas_path_layer_matrix[ system_ptr.path][ system_ptr.layer].detail_ptr))
       )
    {
        lsas_path_layer_matrix[ system_ptr.path][ system_ptr.layer].used = FALSE;
    }
    else
    {
        lsas_fatal_error( LSAS_MODULE_ID, (LSA_UINT16)__LINE__, 0);
    }


    return( LSAS_RSP_OK);
}

#ifdef LSAS_CFG_USE_EDDP

#include "lsas_locate_tcm.h"

LSA_UINT16 lsas_com_get_sync_change_state(LSAS_PTCP_TIMESTAMP_TYPE *p_ptcp_old_time_stamp, LSAS_PTCP_TIMESTAMP_TYPE *p_ptcp_new_time_stamp)
{
    EDDP_USER_CMD_PARAM_TYPE    local_cmd_param;
    EDD_RSP                     ret_val = 1;
    if (lsas_data.eddp.sm >= LSAS_EDDP_SM_DONE)
    {
        local_cmd_param.Async = LSA_FALSE;
        local_cmd_param.CmdOpcode = EDDP_USER_CMD_GSYNC_TIME_SYNC_IND_GET;
        local_cmd_param.Param.TimeStampGet.pOldTimestamp = (EDDP_TIMESTAMP_INFO_PTR_TYPE) p_ptcp_old_time_stamp;
        local_cmd_param.Param.TimeStampGet.pNewTimestamp = (EDDP_TIMESTAMP_INFO_PTR_TYPE) p_ptcp_new_time_stamp;
        ret_val = eddp_UserCmdRequest(lsas_data.eddp_cmd_iface_handle, &local_cmd_param);
    }
    else
    {
        LSAS_FATAL(lsas_data.eddp.sm);
    }

    return ret_val;
}

LSA_VOID lsas_com_get_sync_time(LSAS_PTCP_TIMESTAMP_TYPE *p_ptcp_time_stamp)
{
    EDDP_USER_CMD_PARAM_TYPE    local_cmd_param;
    EDD_RSP                     ret_val = 1;
    if (lsas_data.eddp.sm >= LSAS_EDDP_SM_DONE)
    {
        local_cmd_param.Async = LSA_FALSE;
        local_cmd_param.CmdOpcode = EDDP_USER_CMD_GSYNC_TIME_STAMP_GET;
        local_cmd_param.Param.TimeStampGet.pTimestamp = (EDDP_TIMESTAMP_INFO_PTR_TYPE) p_ptcp_time_stamp;
        ret_val = eddp_UserCmdRequest(lsas_data.eddp_cmd_iface_handle, &local_cmd_param);
        if( ret_val != EDD_STS_OK )
        {
            LSAS_FATAL(ret_val);
        }
    }
    else
    {
        LSAS_FATAL(lsas_data.eddp.sm);
    }

    return;
}
#endif  /* LSAS_CFG_USE_EDDP */

#include "lsas_locate_default.h"

/*** end of file *************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
