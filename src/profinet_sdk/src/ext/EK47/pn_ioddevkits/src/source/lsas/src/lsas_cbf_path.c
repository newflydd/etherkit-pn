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
/*  F i l e               &F: lsas_cbf_path.c                           :F&  */
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
 * @file    lsas_cbf_path.c
 * @brief   brief description
 * @author  cn3dit09
 * @version
 * @date    22.03.2012
 *
 * This file contains redirected callback functions between lsa components
 * e.g. lower requests. The callback of this requests are redirected to be
 * able to send them to an different task and prevent lock-up situations.
 *
 */


#define LSAS_MODULE_ID  5

#include "lsas_inc.h"

#include "lsas_int.h"
#include "lsas_dat.h"

#include "lsas_path.h"

/*****************************************************************************/
//    nare
/*****************************************************************************/

void lsas_dcp_to_nare_cbf( DCP_UPPER_RQB_PTR_TYPE rqb_ptr )
{
    LSAS_RQB_NARE_REQUEST(rqb_ptr, nare_dcp_request_lower_done);

}

/*****************************************************************************/
//    oha
/*****************************************************************************/

void lsas_dcp_to_oha_cbf( DCP_UPPER_RQB_PTR_TYPE rqb_ptr )
{

    LSAS_RQB_OHA_REQUEST(rqb_ptr, oha_dcp_request_lower_done);
}

/*****************************************************************************/

void lsas_lldp_to_oha_cbf( LLDP_UPPER_RQB_PTR_TYPE rqb_ptr )
{

    LSAS_RQB_OHA_REQUEST(rqb_ptr, oha_lldp_request_lower_done);
}

/*****************************************************************************/

void lsas_nare_to_oha_cbf( NARE_UPPER_RQB_PTR_TYPE rqb_ptr)
{

    LSAS_RQB_OHA_REQUEST(rqb_ptr, oha_nare_request_lower_done);
}

/*****************************************************************************/
#ifndef LSAS_CFG_USE_EXTERNAL_TCPIP_STACK
void    lsas_edd_to_tcip_cbf(EDD_UPPER_RQB_PTR_TYPE rqb_ptr)
{

    LSAS_RQB_TCPIP_REQUEST(rqb_ptr, tcip_edd_request_lower_done);
}
#endif
/*****************************************************************************/


void    lsas_sock_to_clrpc_cbf    (CLRPC_LOWER_RQB_PTR_TYPE rqb_ptr)
{

    LSAS_RQB_CLRPC_REQUEST(rqb_ptr, clrpc_request_lower_done);
}

/*****************************************************************************/

void    lsas_sock_to_oha_cbf   (SOCK_UPPER_RQB_PTR_TYPE rqb_ptr)
{

    LSAS_RQB_OHA_REQUEST(rqb_ptr, oha_sock_request_lower_done);
}

/*****************************************************************************/

#ifdef LSAS_CFG_USE_STCP
void    lsas_sock_to_stcp_cbf   (SOCK_UPPER_RQB_PTR_TYPE rqb_ptr)
{

    LSAS_RQB_STCP_REQUEST(rqb_ptr, stcp_request_lower_done);
}


void    lsas_stcp_to_s7pn_cbf   (L4_UPPER_RQB_PTR_TYPE rqb_ptr)
{

    LSAS_RQB_S7PN_REQUEST(rqb_ptr, s7pn_rqb_lower_callback);
}
#endif /* LSAS_CFG_USE_STCP */

/*****************************************************************************/

#ifdef LSAS_CFG_USE_POF
void lsas_edd_to_pof_cbf  (POF_EDD_LOWER_RQB_PTR_TYPE rqb_ptr)
{

    LSAS_RQB_POF_REQUEST(rqb_ptr, pof_edd_request_lower_done);
}

/*****************************************************************************/

void lsas_pof_to_cm_cbf(CM_POF_LOWER_RQB_PTR_TYPE rqb_ptr)
{

    LSAS_RQB_CM_REQUEST(rqb_ptr, cm_pof_request_lower_done);
}
#endif

/*****************************************************************************/

void lsas_cm_to_appl_cbf (CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{

    LSAS_RQB_APPL_REQUEST(rqb_ptr, pndv_cm_callback);
}

/*****************************************************************************/

void lsas_acp_to_cm_cbf  (CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{

    LSAS_RQB_CM_REQUEST(rqb_ptr, cm_acp_request_lower_done);
}

/*****************************************************************************/

void lsas_clrpc_to_cm_cbf    (CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{

    LSAS_RQB_CM_REQUEST(rqb_ptr, cm_rpc_request_lower_done);
}

/*****************************************************************************/

void lsas_edd_to_acp_cbf (ACP_LOWER_RQB_PTR_TYPE rqb_ptr)
{

    LSAS_RQB_ACP_REQUEST(rqb_ptr, acp_request_lower_done);
}

/*****************************************************************************/

void lsas_edd_to_cm_cbf  (CM_EDD_LOWER_RQB_PTR_TYPE rqb_ptr)
{

    LSAS_RQB_CM_REQUEST(rqb_ptr, cm_edd_request_lower_done);
}

/*****************************************************************************/

void lsas_edd_to_dcp_cbf    (DCP_ETH_RQB_PTR_TYPE rqb_ptr)
{

    LSAS_RQB_DCP_REQUEST( rqb_ptr, dcp_request_lower_eth_done);
}

/*****************************************************************************/

void lsas_edd_to_nare_cbf    (EDD_UPPER_RQB_PTR_TYPE rqb_ptr )
{

    LSAS_RQB_NARE_REQUEST(rqb_ptr, nare_edd_request_lower_done);
}

/*****************************************************************************/

void lsas_edd_to_oha_cbf    (EDD_UPPER_RQB_PTR_TYPE rqb_ptr )
{

    LSAS_RQB_OHA_REQUEST( rqb_ptr, oha_edd_request_lower_done);
}

/*****************************************************************************/

void lsas_oha_to_cm_cbf  (CM_UPPER_RQB_PTR_TYPE void_ptr)
{

   LSAS_RQB_CM_REQUEST(void_ptr, cm_oha_request_lower_done);
}

/*****************************************************************************/

void lsas_nare_to_cm_cbf(NARE_UPPER_RQB_PTR_TYPE void_ptr)
{
    LSAS_RQB_CM_REQUEST(void_ptr, cm_nare_request_lower_done);
}

/*****************************************************************************/

#if CM_CFG_USE_MRP
void lsas_mrp_to_cm_cbf(CM_MRP_LOWER_RQB_PTR_TYPE rqb_ptr)
{

    LSAS_RQB_CM_REQUEST(rqb_ptr, cm_mrp_request_lower_done);
}
#endif


/*****************************************************************************/
#ifndef OHA_CFG_NO_MRP
void lsas_mrp_to_oha_cbf  (OHA_MRP_LOWER_RQB_PTR_TYPE rqb_ptr)
{

    LSAS_RQB_OHA_REQUEST(rqb_ptr, oha_mrp_request_lower_done);
}
#endif

/*****************************************************************************/

void lsas_edd_to_lldp_cbf (LLDP_EDD_LOWER_RQB_PTR_TYPE rqb_ptr)
{

    LSAS_RQB_LLDP_REQUEST(rqb_ptr, lldp_edd_request_lower_done);
}

/*****************************************************************************/

extern void mrp_edd_request_lower_done  (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB);

void lsas_edd_to_mrp_cbf  (EDD_UPPER_RQB_PTR_TYPE rqb_ptr )
{

    LSAS_RQB_MRP_REQUEST(rqb_ptr, mrp_edd_request_lower_done);
}

/*****************************************************************************/

void lsas_edd_to_gsy_cbf  (GSY_LOWER_RQB_PTR_TYPE rqb_ptr)
{
    EDD_RQB_OPEN_CHANNEL_TYPE   * local_pParam_ptr;

    if (LSA_RQB_GET_OPCODE(rqb_ptr)==EDD_OPC_OPEN_CHANNEL)
    {
        /* save handle after open_channel */
        local_pParam_ptr = (EDD_RQB_OPEN_CHANNEL_TYPE *)(rqb_ptr->pParam);

        switch (local_pParam_ptr->SysPath)
        {
            case LSAS_PATH_EDD_GSY_SYS:
            {
                if (lsas_data.gsy.handledb[0].used == LSA_FALSE)
                   {
                       lsas_data.gsy.handledb[0].handle       =   LSA_RQB_GET_HANDLE(rqb_ptr);
                       lsas_data.gsy.handledb[0].used         =   LSA_TRUE;
                       lsas_data.gsy.handledb[0].path         =   LSAS_PATH_EDD_GSY_SYS;
                       lsas_data.gsy.handledb[0].method_ptr   =   (void (*) (void *)) (gsy_request_lower_done);
                   }
                   else
                   {
                       /* already used */
                       lsas_fatal_error( LSAS_MODULE_ID, (LSA_UINT16) __LINE__, LSA_RQB_GET_HANDLE(rqb_ptr));
                   }
                break;
            }
            case LSAS_PATH_EDD_GSY_ANNO:
            {
                if (lsas_data.gsy.handledb[1].used == LSA_FALSE)
                   {
                       lsas_data.gsy.handledb[1].handle       =   LSA_RQB_GET_HANDLE(rqb_ptr);
                       lsas_data.gsy.handledb[1].used         =   LSA_TRUE;
                       lsas_data.gsy.handledb[1].path         =   LSAS_PATH_EDD_GSY_ANNO;
                       lsas_data.gsy.handledb[1].method_ptr   =   (void (*) (void *)) (gsy_request_lower_anno_done);
                   }
                   else
                   {
                       /* already used */
                       lsas_fatal_error( LSAS_MODULE_ID, (LSA_UINT16) __LINE__, LSA_RQB_GET_HANDLE(rqb_ptr));
                   }
                break;
            }
            default:
            {
                /* unknown channel, check implementation */
                lsas_fatal_error( LSAS_MODULE_ID, (LSA_UINT16) __LINE__, local_pParam_ptr->SysPath);
                break;
            }
        }

    }

    {
        /* search handle in db */
        LSA_UINT32      i;

        for (i=0; i < (GSY_MAX_SYS_CHANNELS*2) ;i++ )
        {
            if ((lsas_data.gsy.handledb[i].used == LSA_TRUE)&&(lsas_data.gsy.handledb[i].handle == LSA_RQB_GET_HANDLE(rqb_ptr)))
            {
            	/* handle found */
            	break;
            }
        }
        if (i < (GSY_MAX_SYS_CHANNELS*2))
        {
            LSAS_RQB_GSY_REQUEST(rqb_ptr, lsas_data.gsy.handledb[i].method_ptr);
        }
        else
        {
            /* handle not found */
            lsas_fatal_error( LSAS_MODULE_ID, (LSA_UINT16) __LINE__, 0);
        }
    }



}

/*****************************************************************************/

void lsas_gsy_to_cm_cbf(CM_GSY_LOWER_RQB_PTR_TYPE rqb_ptr)
{

    LSAS_RQB_CM_REQUEST(rqb_ptr, cm_gsy_request_lower_done);
}



/*** end of file *************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
