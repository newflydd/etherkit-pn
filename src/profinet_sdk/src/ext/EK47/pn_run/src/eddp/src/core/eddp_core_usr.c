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
/*  C o m p o n e n t     &C: EDDP (EthernetDeviceDriver for PN-IP)     :C&  */
/*                                                                           */
/*  F i l e               &F: eddp_core_usr.c                           :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDD user-interface functions with RQBs           */
/*                                                                           */
/*                          - edd_request()                                  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  20.08.08    JS    initial version.                                       */
/*  22.12.08    LRG   #ifdef EDDP_USE_K32                                    */
/*  02.11.09    sf    renamed from eddp_usr.c --> eddp_core_usr.c but the    */ 
/*                    h-file eddp_usr.h was not renamed (common API) !!!     */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID  4                 
#define EDDP_MODULE_ID     LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_USR */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"            /* edd headerfiles */
#include "eddp_int.h"            /* internal header */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID) 

/*===========================================================================*/
/*                                global Variables                           */
/*===========================================================================*/


/*===========================================================================*/
/*                                external functions                         */
/*===========================================================================*/

/*===========================================================================*/
/*                                 Macros                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                 functions                                 */
/*===========================================================================*/

/*===========================================================================*/
/*                            main-functions                                 */
/*===========================================================================*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreUserRequest                        +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE     pRQB             +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         Opcode:     EDD_OPC_REQUEST                 +*/
/*+     LSA_HANDLE_TYPE         Handle:     valid channel-handle            +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user (not used)           +*/
/*+     LSA_COMP_ID_TYPE        CompId:     Cmponent-ID                     +*/
/*+     EDD_SERVICE             Service:    not used                        +*/
/*+     LSA_RESPONSE_TYPE       Status:     Return status                   +*/
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:     Depend on kind of request.      +*/
/*+                                                                         +*/
/*+  RQB-return values via callback-function:                               +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         opcode:     EDD_OPC_REQUEST                 +*/
/*+     LSA_HANDLE_TYPE         Handle:     upper-handle from open_channel  +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user                      +*/
/*+     EDD_RSP                 Service:    not changed                     +*/
/*+     LSA_RESPONSE_TYPE       Status:     EDD_STS_OK                      +*/
/*+                                         EDD_STS_ERR_OPCODE              +*/
/*+                                         EDD_STS_ERR_SERVICE             +*/
/*+                                         EDD_STS_ERR_PARAM               +*/
/*+                                         EDD_STS_ERR_CHANNEL_USE         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Handles EDD user Requests (EDD_OPC_REQUEST)               +*/
/*+                                                                         +*/
/*+               For this requests a valid channel handle is needed. The   +*/
/*+               handle will be given back on open_channel-request.        +*/
/*+                                                                         +*/
/*+  Note on error-handling:                                                +*/
/*+                                                                         +*/
/*+               In some cases it is not possible to call the call-back-   +*/
/*+               function of the requestor to confirm the request. in this +*/
/*+               case we call the output-macro EDDP_RQB_ERROR with the RQB +*/
/*+               to notify this error. This erros are most likely caused   +*/
/*+               by a implementation error and are of the type             +*/
/*+                                                                         +*/
/*+               EDD_STS_ERR_SERVICE                                       +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreUserRequest(
    EDD_UPPER_RQB_PTR_TYPE      pRQB)
{
    EDD_RSP                     Response;
    EDD_SERVICE                 Service;
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB;

    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CoreUserRequest(pRQB=0x%X)", pRQB);

    /*---------------------------------------------------------------------------*/
    /* Check for valid parameters. Here we cant use the call-back-function       */
    /* on error.                                                                 */
    /*---------------------------------------------------------------------------*/
    Response = EDDP_GlbHandleGetHDB (EDD_RQB_GET_HANDLE_LOWER(pRQB), &pHDB); /* get handle */

    if (Response != EDD_STS_OK)
    {
        // trace first because "EDDP_RQB_ERROR" could be a fatal
        EDDP_UPPER_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "EDDP_CoreUserRequest(): ERROR -> Invalid RQB handle lower in RQB!");
        EDD_RQB_SET_RESPONSE(pRQB, Response);
        EDDP_RQB_ERROR(pRQB);
    }
    else
    {
        Service     = EDD_RQB_GET_SERVICE(pRQB);
        Response    = EDD_STS_OK;

        switch (Service & EDD_SRV_TYPE_MASK)
        {
            /*------------------------------------------------------------*/
            /* NRT-Request                                                */
            /*------------------------------------------------------------*/
            case EDD_SRV_NRT_TYPE:
            case EDD_SRV_NRT_FILTER_TYPE:
            {
                /* check if channel configured for this services */
                if (pHDB->UsedComp & EDDP_COMP_NRT)
                {
                    EDDP_NRTRequest (pRQB, pHDB->pDDB, pHDB);
                }                
                else
                {
                    Response = EDD_STS_ERR_CHANNEL_USE;
                    EDD_RQB_SET_RESPONSE(pRQB, Response);
                    EDDP_UPPER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "EDDP_CoreUserRequest(): ERROR -> NRT not supported by this channel! (Service: 0x%X, pHDB: 0x%X)",
                        Service, pHDB);
                }
            }
            break;

            /*------------------------------------------------------------*/
            /* SRT-Request                                                */
            /*------------------------------------------------------------*/
            case EDD_SRV_SRT_TYPE:
            {
                /* check if channel configured for this services */
                if (pHDB->UsedComp & EDDP_COMP_CRT)
                {
                    EDDP_CRTRequest (pRQB, pHDB->pDDB, pHDB);
                }                
                else
                {
                    Response = EDD_STS_ERR_CHANNEL_USE;
                    EDD_RQB_SET_RESPONSE(pRQB, Response);
                    EDDP_UPPER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "EDDP_CoreUserRequest(): ERROR -> CRT not supported by this channel! (Service: 0x%X, pHDB: 0x%X)",
                            Service, pHDB);
                }
            }
            break;

            /*------------------------------------------------------------*/
            /* SWI-Request                                                */
            /*------------------------------------------------------------*/
            case EDD_SRV_SWI_TYPE:
            {
                /* check if channel configured for this services */
                if (pHDB->UsedComp & EDDP_COMP_SWI)
                {
                    EDDP_SWIRequest (pRQB, pHDB->pDDB, pHDB);
                }                
                else
                {
                    Response = EDD_STS_ERR_CHANNEL_USE;
                    EDD_RQB_SET_RESPONSE(pRQB, Response);
                    EDDP_UPPER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "EDDP_CoreUserRequest(): ERROR -> SWI not supported by this channel! (Service: 0x%X, pHDB: 0x%X)",
                            Service, pHDB);
                }
            }
            break;

            /*------------------------------------------------------------*/
            /* General requests                                           */
            /*------------------------------------------------------------*/
            case EDD_SRV_GENERAL_TYPE:
            {
                EDDP_GENRequest (pRQB, pHDB->pDDB, pHDB);
            }
            break;

            /*------------------------------------------------------------*/
            /* PRM-Request                                                */
            /*------------------------------------------------------------*/
            case EDD_SRV_PRM_TYPE:
            {
                /* check if channel configured for this services */
                if (pHDB->UsedComp & EDDP_COMP_PRM)
                {
                    EDDP_PRMRequest (pRQB, pHDB->pDDB, pHDB);
                }                
                else
                {
                    Response = EDD_STS_ERR_CHANNEL_USE;
                    EDD_RQB_SET_RESPONSE(pRQB, Response);
                    EDDP_UPPER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "EDDP_CoreUserRequest(): ERROR -> PRM not supported by this channel! (Service: 0x%X, pHDB: 0x%X)",
                        Service, pHDB);
                }
            }
            break;

            /*------------------------------------------------------------*/
            /* PTCP-Request                                               */
            /*------------------------------------------------------------*/
            case EDD_SRV_PTCP_TYPE:
            {
                /* check if channel configured for this services */
                if (pHDB->UsedComp & EDDP_COMP_K32)
                {
                    EDDP_K32Request (pRQB, pHDB->pDDB, pHDB);
                }                
                else
                {
                    Response = EDD_STS_ERR_CHANNEL_USE;
                    EDD_RQB_SET_RESPONSE(pRQB, Response);
                    EDDP_UPPER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "EDDP_CoreUserRequest(): ERROR -> K32 not supported by this channel! (Service: 0x%X, pHDB: 0x%X)",
                        Service, pHDB);
                }
            }
            break;

            /*------------------------------------------------------------*/
            /* Unknown Service                                            */
            /*------------------------------------------------------------*/
            default:
            {
            	EDDP_LL_CoreRequest(pRQB, pHDB);
#if 0
                Response = EDD_STS_ERR_SERVICE;
                EDD_RQB_SET_RESPONSE(pRQB, Response);
                EDDP_UPPER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "EDDP_CoreUserRequest(): ERROR -> Unknown/Unsupported Service! (Service: 0x%X, pHDB: 0x%X)",
                    Service, pHDB);
#endif
            }
            break;
        }

        /* ------------------------------------------------------------------------*/
        /* On error we finish the request. If no error the request will be handled */
        /* by the service handler.                                                 */
        /* ------------------------------------------------------------------------*/
        if (Response != EDD_STS_OK)
        {
            EDDP_GlbRequestFinish (pHDB, pRQB, Response);
        }
    }

    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CoreUserRequest(RQB-Response=0x%X)", Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_request                                +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         Opcode:     EDD_OPC_REQUEST                 +*/
/*+                                         EDD_OPC_REQUEST_SRT_BUFFER      +*/
/*+     LSA_HANDLE_TYPE         Handle:     valid channel-handle            +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user (not used)           +*/
/*+     EDD_SERVICE             Service:    not used                        +*/
/*+     LSA_RESPONSE_TYPE       Status:     Return status                   +*/
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:     Depend on kind of request.      +*/
/*+                                                                         +*/
/*+  RQB-return values via callback-function:                               +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         opcode:     EDD_OPC_REQUEST                 +*/
/*+                                         EDD_OPC_REQUEST_SRT_BUFFER      +*/
/*+     LSA_HANDLE_TYPE         Handle:     upper-handle from open_channel  +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user                      +*/
/*+     EDD_SERVICE             Service:    not changed                     +*/
/*+     LSA_RESPONSE_TYPE       Status:     EDD_STS_OK                      +*/
/*+                                         EDD_STS_ERR_OPCODE              +*/
/*+                                         EDD_STS_ERR_SERVICE             +*/
/*+                                         EDD_STS_ERR_PARAM               +*/
/*+                                         EDD_STS_ERR_CHANNEL_USE         +*/
/*+                                         :                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Do a user request asigned to a channel                    +*/
/*+                                                                         +*/
/*+               For this requests a valid channel handle is needed. The   +*/
/*+               handle will be given back on open_channel-request.        +*/
/*+                                                                         +*/
/*+  Note on error-handling:                                                +*/
/*+                                                                         +*/
/*+               In some cases it is not possible to call the call-back-   +*/
/*+               function of the requestor to confirm the request. in this +*/
/*+               case we call the output-macro EDDP_RQB_ERROR with the RQB +*/
/*+               to notify this error. This erros are most likely caused   +*/
/*+               by a implementation error and are of the type             +*/
/*+                                                                         +*/
/*+               EDD_STS_ERR_OPCODE                                        +*/
/*+               EDD_STS_ERR_SERVICE                                       +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDD_UPPER_IN_FCT_ATTR eddp_request(
    EDD_UPPER_RQB_PTR_TYPE      pRQB)
{
    EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] --> eddp_request(pRQB=0x%X)", pRQB);

    switch (EDD_RQB_GET_OPCODE(pRQB))
    {
        case EDD_OPC_REQUEST_SRT_BUFFER:
        {
            /* tbd */
            EDDP_UPPER_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_UNEXP, "[H:--] ==> eddp_request(): EDD_OPC_REQUEST_SRT_BUFFER not supported yet!");
            EDD_RQB_SET_RESPONSE(pRQB, EDD_STS_ERR_OPCODE);
            EDDP_RQB_ERROR(pRQB);
        }
        break;

        case EDD_OPC_REQUEST:
        {
            EDDP_ENTER_LOW_S();
        
            EDDP_CoreUserRequest (pRQB);

            EDDP_EXIT_LOW_S();
        }
        break;

        default:
        {
            EDDP_UPPER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_UNEXP, "[H:--] ==> eddp_request(): Invalid opcode (0x%X)",
                EDD_RQB_GET_OPCODE(pRQB));
            EDD_RQB_SET_RESPONSE(pRQB, EDD_STS_ERR_OPCODE);
            EDDP_RQB_ERROR(pRQB);
        }
        break;
    }

    EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] <-- eddp_request(pRQB=0x%X)", pRQB);
}

/*****************************************************************************/
/*  end of file EDDP_CORE_USR.C                                              */
/*****************************************************************************/

