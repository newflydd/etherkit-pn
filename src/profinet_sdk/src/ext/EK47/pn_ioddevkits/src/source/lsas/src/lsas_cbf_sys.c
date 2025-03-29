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
/*  F i l e               &F: lsas_cbf_sys.c                            :F&  */
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
 * @file    lsas_cbf.c
 * @brief   brief description
 * @author  cn3dit09
 * @version
 * @date    21.03.2012
 *
 * This file contains callback functions for system channels.
 * These do ussaly nothing but initiate a path chain.
 *
 *
 */

/*****************************************************************************/

#define LSAS_MODULE_ID  4

#include "lsas_inc.h"

#include "lsas_int.h"
#include "lsas_dat.h"


/*****************************************************************************/

void lsas_cbf_sys_edd_request_lower_done(EDD_UPPER_RQB_PTR_TYPE pRQB)
{
    if( pRQB )
    {
        if( EDD_RQB_GET_OPCODE(pRQB) == EDD_OPC_OPEN_CHANNEL )
        {
            if( pRQB->response != EDD_STS_OK )
            {
                LSAS_FATAL(pRQB->response);
            }
            LSAS_RQB_LSAS_REQUEST(pRQB, lsas_com_open_pnio_stack_sm);
        }
        else if(EDD_RQB_GET_OPCODE(pRQB) == EDD_OPC_SYSTEM)
        {
            LSAS_RQB_EDD_REQUEST(pRQB, lsas_edd_req_open);
            lsas_data.edd.request_processed = 1;
        }
    }
    else
    {
        LSAS_FATAL(LSAS_FATAL_ERR_LOWER_DONE);
    }
}

/*****************************************************************************/

void   lsas_cbf_sys_acp_request_lower_done   (ACP_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    if ( rqb_ptr->response != ACP_OK )
    {
        LSAS_FATAL(rqb_ptr->response);
    }
    LSAS_RQB_LSAS_REQUEST(rqb_ptr, lsas_com_open_pnio_stack_sm);
}

/*****************************************************************************/

void    lsas_cbf_sys_tcip_request_lower_done(TCIP_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    if ( rqb_ptr->response != TCIP_OK )
    {
        LSAS_FATAL(rqb_ptr->response);
    }
    LSAS_RQB_LSAS_REQUEST(rqb_ptr, lsas_com_open_pnio_stack_sm);
}

/*****************************************************************************/

void   lsas_cbf_sys_mrp_request_lower_done   (MRP_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    if ( rqb_ptr->response != MRP_RSP_OK )
    {
        LSAS_FATAL(rqb_ptr->response);
    }
    LSAS_RQB_LSAS_REQUEST(rqb_ptr, lsas_com_open_pnio_stack_sm);
}


/*****************************************************************************/

void   lsas_cbf_sys_clrpc_request_lower_done   (CLRPC_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    if ( rqb_ptr->response != CLRPC_OK )
    {
        LSAS_FATAL(rqb_ptr->response);
    }
    LSAS_RQB_LSAS_REQUEST(rqb_ptr, lsas_com_open_pnio_stack_sm);
}

#ifdef LSAS_CFG_USE_STCP
/*****************************************************************************/
void   lsas_cbf_sys_s7pn_request_done   (L4_UPPER_RQB_PTR_TYPE rqb_ptr)
{
   if( rqb_ptr )
   {
    	if( LSA_RQB_GET_OPCODE(rqb_ptr) == L4_OPC_OPEN_CHANNEL )
        {
    LSAS_RQB_LSAS_REQUEST(rqb_ptr, lsas_com_open_pnio_stack_sm);
}
    }
    else
    {
        LSAS_FATAL(LSAS_FATAL_ERR_LOWER_DONE);
    }
}
#endif /* LSAS_CFG_USE_STCP */

/*****************************************************************************/

void   lsas_cbf_sys_oha_request_lower_done   (OHA_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    if ( rqb_ptr->response != OHA_OK )
    {
        LSAS_FATAL(rqb_ptr->response);
    }
    LSAS_RQB_LSAS_REQUEST(rqb_ptr, lsas_com_open_pnio_stack_sm);
}

/*****************************************************************************/

void   lsas_cbf_sys_nare_request_lower_done   (NARE_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    if ( rqb_ptr->response != NARE_RSP_OK )
    {
        LSAS_FATAL(rqb_ptr->response);
    }
    LSAS_RQB_LSAS_REQUEST(rqb_ptr, lsas_com_open_pnio_stack_sm);
}

/*****************************************************************************/

void    lsas_cbf_sys_cm_request_lower_done     (CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    if ( rqb_ptr->response != CM_OK )
    {
        LSAS_FATAL(rqb_ptr->response);
    }
    LSAS_RQB_LSAS_REQUEST(rqb_ptr, lsas_com_open_pnio_stack_sm);
}

/*****************************************************************************/

void   lsas_cbf_sys_gsy_request_lower_done   (GSY_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    if ( rqb_ptr->response != GSY_RSP_OK )
    {
        LSAS_FATAL(rqb_ptr->response);
    }
    LSAS_RQB_LSAS_REQUEST(rqb_ptr, lsas_com_open_pnio_stack_sm);
}

/*****************************************************************************/

void    lsas_cbf_sys_oha_sys_request_lower_done(OHA_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    if( rqb_ptr )
    {
        if ( rqb_ptr->response != OHA_OK )
        {
            LSAS_FATAL(rqb_ptr->response);
        }

    	switch( OHA_RQB_GET_OPCODE(rqb_ptr) )
        {
    	    case OHA_OPC_OPEN_CHANNEL:
    	        lsas_data.oha.handle = rqb_ptr->args.channel.handle;
                LSAS_HANDLE_OHA_REMA_REQUESTS(rqb_ptr);
                LSAS_RQB_LSAS_REQUEST(rqb_ptr, lsas_com_open_pnio_stack_sm);
                break;
    	    case OHA_OPC_REMA_INDICATION:
    	        LSAS_HANDLE_OHA_REMA_REQUESTS(rqb_ptr);
    	        break;
            #ifdef LSAS_CFG_REPORT_NEW_IP_DATA
    	    case OHA_OPC_PRM_READ:
    	    {
    	        /*there is a new IP (was indicated by OHA_REPORT_DCP_SET_RES -> report to user */
                    /*invert byte order*/
    	        LSA_UINT32  ip_address[3];
    	        LSAS_ASSERT(rqb_ptr->args.PrmData.record_index == OHA_RECORD_INDEX_IP_SUITE);

    	        ip_address[0] = OHA_HTONL(*((LSA_UINT32*)(&rqb_ptr->args.PrmData.record_data[OHA_TOPO_BLOCK_HEADER_PDU_SIZE + 2])));
    	        ip_address[1] = OHA_HTONL(*((LSA_UINT32*)(&rqb_ptr->args.PrmData.record_data[OHA_TOPO_BLOCK_HEADER_PDU_SIZE + 2 + 4])));
    	        ip_address[2] = OHA_HTONL(*((LSA_UINT32*)(&rqb_ptr->args.PrmData.record_data[OHA_TOPO_BLOCK_HEADER_PDU_SIZE + 2 + 8])));

                LSAS_CFG_REPORT_NEW_IP_DATA(ip_address);

                lsas_data.oha.oha_ip_rqb_in_use = FALSE;
    	        break;
    	    }
            #endif
            case OHA_OPC_SET_ADDRESS:
			{
                LSA_UINT32 Response;
                LSA_UINT32 ip_address[3];
                LSA_UINT8  *b;

                b = (LSA_UINT8  *) &rqb_ptr->args.Address.pStationData->LocalIpAddr[0];
                ip_address[0] = (b[0] << 24) | (b[1] << 16) | (b[2] << 8) | (b[3]);
                b = (LSA_UINT8  *) &rqb_ptr->args.Address.pStationData->LocalSubnetMask[0];
                ip_address[1] = (b[0] << 24) | (b[1] << 16) | (b[2] << 8) | (b[3]);
                b = (LSA_UINT8  *) &rqb_ptr->args.Address.pStationData->DefaultRouterAddr[0];
                ip_address[2] = (b[0] << 24) | (b[1] << 16) | (b[2] << 8) | (b[3]);

                LSAS_CFG_REPORT_NEW_IP_DATA(ip_address);

                LSAS_FREE_COMP_MEM( &Response, rqb_ptr, LSA_COMP_ID_LSAS );
                if( LSA_RET_OK != Response )
                {
                    LSAS_FATAL( 0);
                }

                break;
            }
    	    default:
    	        LSAS_FATAL(OHA_RQB_GET_OPCODE(rqb_ptr));
    	        break;
        }
    }
    else
    {
        LSAS_FATAL(LSAS_FATAL_ERR_LOWER_DONE);
    }
}

/*** end of file *************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
