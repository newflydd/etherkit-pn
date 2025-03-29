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
/*  F i l e               &F: lsas.c                                    :F&  */
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
 * @file    lsas.c
 * @brief   brief description
 * @author  cn3dit09
 * @version
 * @date    27.02.2012
 *
 * long description
 *
 */

/*****************************************************************************/

#define LSAS_MODULE_ID  3

#include "lsas_inc.h"

#define LSAS_DAT_EXT_ATTR

#include "lsas_int.h"
#include "lsas_dat.h"


void LSAS_SYSTEM_OUT_FCT_ATTR lsas_init()
{
    LSAS_MEMSET(&lsas_data, 0, sizeof(lsas_data));

    lsas_data.open.sm = LSAS_OPEN_SM_CLOSED;
    #ifdef LSAS_CFG_USE_EDDP
    lsas_data.eddp.sm = LSAS_EDDP_SM_CLOSED;
    #endif
    #ifdef LSAS_CFG_USE_EDDS
    lsas_data.edds.sm = LSAS_EDDS_SM_CLOSED;
    #endif
}

void LSAS_SYSTEM_OUT_FCT_ATTR lsas_request(LSAS_RQB_PTR_TYPE rqb_ptr)
{
    LSAS_ENTER();


    if (LSAS_RQB_GET_OPCODE(rqb_ptr) == LSAS_OPC_INIT_PNIO_STACK)
    {
        lsas_com_init_pnio_stack();

        LSAS_CFG_INIT_DONE_IND(rqb_ptr);

    }
    else if (LSAS_RQB_GET_OPCODE(rqb_ptr) == LSAS_OPC_SETUP_PNIO_STACK)
    {
        lsas_com_setup_pnio_stack(rqb_ptr);
    }
    else if (LSAS_RQB_GET_OPCODE(rqb_ptr) == LSAS_OPC_OPEN_PNIO_STACK)
    {
        lsas_com_open_pnio_stack();
    }
    else if (LSAS_RQB_GET_OPCODE(rqb_ptr) == LSAS_OPC_INIT_OHA_DB)
    {
        lsas_oha_db_init(rqb_ptr);
    }
    else if (LSAS_RQB_GET_OPCODE(rqb_ptr) == LSAS_OPC_FS_OPEN)
    {
#ifdef LSAS_CFG_USE_FS
        lsas_com_fs_open();
#else
        LSAS_RQB_SET_RESPONSE(rqb_ptr, LSA_RSP_ERR_PARAM);
#endif
        LSAS_CFG_FS_OPEN_DONE();
    }
    else
    {
        /* unknown opcode */
    }

#ifdef LSAS_FREE_REQUEST_RQB

#endif

    LSAS_EXIT();

}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    lsas_fatal_error                            +*/
/*+  Input/Output               LSAS_FATAL_ERROR_TYPE Error                 +*/
/*+  Input/Output          :    LSA_UINT16            ModuleID              +*/
/*+                             LSA_UINT16            Line                  +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  Error                : LSAS-fatal-errorcode                            +*/
/*+  ModuleID             : module - id of error                            +*/
/*+  Line                 : line of code (optional)                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Signals Fatal-error via LSAS_FATAL_ERROR macro.           +*/
/*+               This function does not return!                            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID LSAS_LOCAL_FCT_ATTR lsas_fatal_error(
    LSA_UINT16             ModuleID,
    LSA_UINT16             Line,
    LSAS_FATAL_ERROR_TYPE  Error)

{
    LSA_FATAL_ERROR_TYPE    LSAError;

    LSAError.lsa_component_id  = LSA_COMP_ID_LSAS;
    LSAError.module_id         = ModuleID;
    LSAError.line              = Line;
    LSAError.error_code[0]     = Error;
    LSAError.error_code[1]     = 0;
    LSAError.error_code[2]     = 0;
    LSAError.error_code[3]     = 0;
    LSAError.error_data_length = 0;
    LSAError.error_data_ptr    = LSA_NULL; /* currently no message */

//    LSAS_PROGRAM_TRACE_03(0,LSA_TRACE_LEVEL_FATAL,"Fatal Error. ModID: %d, Line: %d, Error: %Xh",ModuleID,Line,Error);
    LSAS_FATAL_ERROR( &LSAError);
    /* should not return! */
#ifdef _DEBUG
    _asm int 3;
#endif
    while (1) ;

}

/*** end of file *************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
