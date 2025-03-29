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
/*  F i l e               &F: eddp_crt.c                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  CRT (Cycle Real Time) for EDDP  -> user interface                        */
/*      - start up services                                                  */
/*      - shut down services                                                 */
/*      - services of RQBHandler                                             */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who         What                                             */
#ifdef EDDP_MESSAGE
/*  22.10.08    Sergeev     Erstellung                                       */
#endif
/*****************************************************************************/


/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/

#define LTRC_ACT_MODUL_ID   21
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_CRT */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"           /* eddp headerfiles */
#include "eddp_int.h"           /* internal header  */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTRequest                             +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters          (in) +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         Opcode:     EDD_OPC_REQUEST                 +*/
/*+     LSA_HANDLE_TYPE         Handle:                                     +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user (not used)           +*/
/*+     EDD_SERVICE             Service:    EDD_SRV_xxx                     +*/
/*+                                         EDD_SRV_xxx                     +*/
/*+                                         EDD_SRV_xxx                     +*/
/*+     EDD_RSP                 Response:   Return status                   +*/
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:     Depend on service               +*/
/*+                                                                         +*/
/*+  RQB-return values via callback-function:                               +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         opcode:     EDD_OPC_REQUEST                 +*/
/*+     LSA_HANDLE_TYPE         Handle:     upper-handle from open_channel  +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user                      +*/
/*+     LSA_COMP_ID_TYPE        CompId:     not changed                     +*/
/*+     EDD_SERVICE             Service:    not changed                     +*/
/*+     EDD_RSP                 Response:   EDD_STS_OK                      +*/
/*+                                         EDD_STS_ERR_OPCODE              +*/
/*+                                         EDD_STS_ERR_SERVICE             +*/
/*+                                         EDD_STS_ERR_PARAM               +*/
/*+                                         EDD_STS_ERR_CHANNEL_USE         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function handles all CRT-Requests for EDDP           +*/
/*+               which are called from outside (EDDP-CORE).                +*/
/*+                                                                         +*/
/*+               This errors are used:                                     +*/
/*+                 - EDD_STS_ERR_PARAM, if we have a parameter error       +*/
/*+                 - EDD_STS_ERR_SERVICE, if we get an invalid Service     +*/
/*+               If error, call the Handle-CallBackFunction                +*/
/*+               and abort the request.                                    +*/
/*+                                                                         +*/
/*+               Note: Opcode not checked!                                 +*/
/*+                     CRT-Component already initialized (not checked)     +*/
/*+                     Channel uses this component (not checked)           +*/
/*+                                                                         +*/
/*+               If the request is valid, it is queued or executed.        +*/
/*+                                                                         +*/
/*+               Request is synchron:                                      +*/
/*+                 - execute this RQB                                      +*/
/*+                 - request is finished here                              +*/
/*+                                                                         +*/
/*+               Request is asynchron:                                     +*/
/*+                 - put this RQB to Queue of CRT                          +*/
/*+                 - call the RQBHandler for this queue and execute        +*/
/*+                   this RQB by RQBHandler                                +*/
/*+                 - if no error in RQBHandler, request will be finished   +*/
/*+                   after command execution in EDDP_CRTCmdDoneCbfxxx()    +*/
/*+                 - if error in RQBHandler, request will be finished by   +*/
/*+                   RQBHandler                                            +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTRequest(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP         Response;
    EDD_SERVICE     Service;
    LSA_BOOL        isRequestFinish;

    Response            = EDD_STS_OK;
    Service             = EDD_RQB_GET_SERVICE(pRQB);
    isRequestFinish     = LSA_TRUE;

    EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] >>> EDDP_CRTRequest(pRQB=0x%X)", pHDB->Handle, pRQB);

    if (LSA_HOST_PTR_ARE_EQUAL(pRQB->pParam, LSA_NULL))
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_CRTRequest(): ERROR -> RQB->pParam=NULL, Response=0x%X", 
            pHDB->Handle, Response);
    }
    else
    {
        switch (Service)
	    {
            /* ---------------------------------------------------------------- */
		    /* provider add                                                     */
		    /* provider control                                                 */
		    /* provider remove                                                  */
		    /* provider change phase                                            */
            /* ---------------------------------------------------------------- */
		    case EDD_SRV_SRT_PROVIDER_ADD:
		    case EDD_SRV_SRT_PROVIDER_CONTROL:
		    case EDD_SRV_SRT_PROVIDER_REMOVE:
            case EDD_SRV_SRT_PROVIDER_CHANGE_PHASE:
            /* ---------------------------------------------------------------- */
		    /* consumer add                                                     */
		    /* consumer control                                                 */
		    /* consumer remove                                                  */
		    /* ---------------------------------------------------------------- */
		    case EDD_SRV_SRT_CONSUMER_ADD:
		    case EDD_SRV_SRT_CONSUMER_CONTROL:
		    case EDD_SRV_SRT_CONSUMER_REMOVE:
            {
                // put this RQB to Queue
                EDDP_GlbRQBQueueAddToEnd (&(pDDB->GlbCrtRQBHandler.RqbQueue), pRQB);
                pHDB->pCRT->RunningRQBCount++;
                // and call RQB-Handler
                EDDP_GlbCrtRQBHandler (pDDB);
                // request is queued
                isRequestFinish = LSA_FALSE;
            }
			break;

            /* ---------------------------------------------------------------- */
		    /* provide indication resources                                     */
		    /* ---------------------------------------------------------------- */
		    case EDD_SRV_SRT_IND_PROVIDE:
            {
                EDDP_UPPER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_SRT_IND_PROVIDE", pHDB->Handle);
                Response = EDDP_CRTIndicationProvide (pRQB, pDDB, pHDB);
                // request is queued
                isRequestFinish = LSA_FALSE;
            }
			break;

            /* ---------------------------------------------------------------- */
		    /* provider set state                                               */
		    /* ---------------------------------------------------------------- */
		    case EDD_SRV_SRT_PROVIDER_SET_STATE:
            {
                EDDP_UPPER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_SRT_PROVIDER_SET_STATE", pHDB->Handle);
			    Response = EDDP_CRTProviderSetState (pRQB, pDDB, pHDB);
            }
			break;

            /* ---------------------------------------------------------------- */
		    /* get apdu-status                                                  */
		    /* ---------------------------------------------------------------- */
		    case EDD_SRV_SRT_GET_APDU_STATUS:
            {
                EDDP_UPPER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_SRT_GET_APDU_STATUS", pHDB->Handle);
		        Response = EDDP_CRTGetAPDUStatus (pRQB, pDDB, pHDB);
            }
			break;

            /* ---------------------------------------------------------------- */
		    /* Unknown/Unsupported Request                                      */
		    /* ---------------------------------------------------------------- */
		    default:
            {
                Response = EDD_STS_ERR_SERVICE;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_CRTRequest(): ERROR -> Unknown/Unsupported Service(0x%X)", 
                    pHDB->Handle, Service);
            }
			break;
        }
    }

    if (isRequestFinish)
    {
        // request is finished here
        EDDP_GlbRequestFinish (pHDB, pRQB, Response);
    }

    EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] <<< EDDP_CRTRequest(): Response=0x%X", pHDB->Handle, Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTOpenChannel                         +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB           +*/
/*+                        :    EDDP_LOCAL_HDB_PTR_TYPE      pHDB           +*/
/*+                             LSA_HANDLE_TYPE              Handle         +*/
/*+                             EDDP_SYSTEM_CDB_PTR_TYPE     pCDB           +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                    (in) +*/
/*+  Handle     : Handle for this Channel (<  EDD_CFG_MAX_CHANNELS)    (in) +*/
/*+  pCDB       : Pointer to ChannelDescriptionBlock                   (in) +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_CHANNEL_USE                                   +*/
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: Opens Channel for CRT. The Handle will specify the Channel+*/
/*+               and pDDB the Device. pCDB has all parameters              +*/
/*+               for this channel.                                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTOpenChannel(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB,
    LSA_HANDLE_TYPE             Handle,
    EDDP_SYSTEM_CDB_PTR_TYPE    pCDB)
{
    EDD_RSP         Response;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(Handle);
#endif

    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_CRTOpenChannel(Handle=0x%X, pCDB=0x%X)", 
        pHDB->Handle, Handle, pCDB);

    Response = EDD_STS_OK;

    if (Response == EDD_STS_OK)
    {
        if ( (EDDP_GET_BIT_VALUE( (pDDB->CRT.BitFlag), 
                                  EDDP_CRT_BITFLAG_OPEN_CHANNEL_MASK, 
                                  EDDP_CRT_BITFLAG_OPEN_CHANNEL_SHIFT))
           )
        {
            // OpenChannel for CRT already done - no more OpenChannel!
            Response = EDD_STS_ERR_CHANNEL_USE;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_CRTOpenChannel(): ERROR -> OpenChannel for CRT is already done - CRT use only one Channel!", 
                pHDB->Handle);
        }
        else
        {
            // alloc handle management-memory
            Response = EDDP_CRTAllocLocalMemHDB (pDDB, pHDB);

            if (Response == EDD_STS_OK)
            {
                pHDB->pCRT->UseCRT          = pCDB->UsedComp.UseCRT;
                pHDB->pCRT->RunningRQBCount = 0;
            }
        }
    }

    if (Response == EDD_STS_OK)
    {
        // set OpenChannel-Bit
        EDDP_SET_BIT_VALUE32 ( (pDDB->CRT.BitFlag), 
                               1, 
                               EDDP_CRT_BITFLAG_OPEN_CHANNEL_MASK, 
                               EDDP_CRT_BITFLAG_OPEN_CHANNEL_SHIFT)
    }
    else
    {
        // do FreeLocalMem of CRT management in HDB
        EDDP_CRTFreeLocalMemHDB (pDDB, pHDB);
    }

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTOpenChannel(): Response=0x%X", Response);

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTCloseChannel                        +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB           +*/
/*+                        :    EDDP_LOCAL_HDB_PTR_TYPE      pHDB           +*/
/*+                             LSA_HANDLE_TYPE              Handle         +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                    (in) +*/
/*+  Handle     : Handle for this Channel (< EDD_CFG_MAX_CHANNELS)     (in) +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_SEQUENCE                                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: Close Channel for CRT.                                    +*/
/*+                                                                         +*/
/*+               Check all provider/consumer are removed by user.          +*/
/*+               If some provider or consumer are available in CRT         +*/
/*+               management the CloseChannel will be aborted with          +*/
/*                EDD_STS_ERR_SEQUENCE.                                     +*/
/*+                                                                         +*/
/*+               Finish all indication RQBs.                               +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCloseChannel(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB,
    LSA_HANDLE_TYPE             Handle)
{
    EDD_RSP         Response;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(Handle);
#endif

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_CRTCloseChannel(Handle=0x%X)", pHDB->Handle, Handle);

    Response = EDD_STS_OK;

    Response = EDDP_CRTCheckCloseChannel (pDDB, pHDB);

    if (Response == EDD_STS_OK)
    {
        // finish all indication RQBs
        EDDP_CRTIndProvideFinishAll (pDDB, EDD_STS_OK_CANCEL);

        // do FreeLocalMem of CRT management in HDB
        EDDP_CRTFreeLocalMemHDB (pDDB, pHDB);

        // delete OpenChannel-Bit
        EDDP_SET_BIT_VALUE32 ( (pDDB->CRT.BitFlag), 
                               0, 
                               EDDP_CRT_BITFLAG_OPEN_CHANNEL_MASK, 
                               EDDP_CRT_BITFLAG_OPEN_CHANNEL_SHIFT)
    }

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTCloseChannel(): Response=0x%X", Response);

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTDeviceOpen                          +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             EDDP_UPPER_DPB_PTR_TYPE        pDPB         +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  pDPB       : Pointer to DeviceParameterBlock                     (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDD_SRV_DEV_OPEN.                                         +*/
/*+                                                                         +*/
/*+               - Check the configuration parameter of system adaption    +*/
/*+               - Alloc HOST memory                                       +*/
/*+               - Init all CRT management structures within DDB           +*/
/*+               - Alloc timer                                             +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTDeviceOpen(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DPB_PTR_TYPE     pDPB)
{
    EDD_RSP             Response;

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CRTDeviceOpen(pDDB=0x%X, pDPB=0x%X)", 
        pDDB, pDPB);

    Response = EDD_STS_OK;

    // ***** check and init all configuration parameter of CRT management
    Response = EDDP_CRTInitCfgParam (pDDB, pDPB);

    // ***** init CRT management
    if (Response == EDD_STS_OK)
    {
        // ***** alloc HOST memory
        Response = EDDP_CRTAllocLocalMemDDB (pDDB);

        if (Response == EDD_STS_OK)
        {   // ***** init common parts of CRT
            Response = EDDP_CRTInitDevOpen(pDDB, pDPB);
                    
            if (Response == EDD_STS_OK)
            {   // ***** init provider
                Response = EDDP_CRTInitProviderDevOpen (pDDB, pDPB);
    
                if (Response == EDD_STS_OK)
                {   // ***** init consumer
                    Response = EDDP_CRTInitConsumerDevOpen (pDDB, pDPB);
    
                    if (Response == EDD_STS_OK)
                    {   // ***** init inbound (SW mgmt only)
                        Response = EDDP_CRTInitInboundDevOpen (pDDB, pDPB);
    
                        if (Response == EDD_STS_OK)
                        {   // ***** alloc timer
                            Response = EDDP_CRTAllocTimer (pDDB);
    
                            if (Response == EDD_STS_OK)
                            {   // ***** init state machines
                                Response = EDDP_CRTInitStateMachine (pDDB);
                            }
                        }
                    }
                }
            }
        }
    }

    if (Response == EDD_STS_OK)
    {
            // set DEV_OPEN-Bit
        EDDP_SET_BIT_VALUE32 ( (pDDB->CRT.BitFlag), 
                               1, 
                               EDDP_CRT_BITFLAG_DEV_OPEN_MASK, 
                               EDDP_CRT_BITFLAG_DEV_OPEN_SHIFT)
    }
    else
    {
        // do FreeLocalMem of CRT management
        EDDP_CRTFreeLocalMemDDB (pDDB);
    }

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTDeviceOpen(): Response=0x%X", Response);

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname          :    EDDP_CRTDeviceClose                             +*/
/*+                                                                             +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE           pDDB          +*/
/*+                                                                             +*/
/*+  Result                :    EDD_RSP                                         +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)      +*/
/*+                                                                             +*/
/*+  Results    : EDD_STS_OK                                                    +*/
/*+               EDD_STS_OK_PENDING                                            +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function is called from CORE within service              +*/
/*+               - EDDP_SRV_DEV_CLOSE or                                       +*/
/*+               - EDDP_SRV_DEV_OPEN (only if previous OPEN shall be aborted)  +*/
/*+                                                                             +*/
/*+               Close device for CRT and give memory and timer free again     +*/
/*+               that was allocated in DEV_OPEN.                               +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTDeviceClose(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP             Response = EDD_STS_OK;

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CRTDeviceClose(pDDB=0x%X)", pDDB);

    // do FreeLocalMem of CRT management
    Response = EDDP_CRTFreeLocalMemDDB (pDDB);

    if (Response == EDD_STS_OK)
    {
        // do FreeTimer of CRT
        Response = EDDP_CRTFreeTimer (pDDB);
    }

    if (Response == EDD_STS_OK)
    {
        // delete DEV_OPEN-Bit
        EDDP_SET_BIT_VALUE32 ( (pDDB->CRT.BitFlag), 
                                0, 
                                EDDP_CRT_BITFLAG_DEV_OPEN_MASK, 
                                EDDP_CRT_BITFLAG_DEV_OPEN_SHIFT)
    }

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTDeviceClose(): Response=0x%X", Response);

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTDeviceSetup                         +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             EDDP_UPPER_DSB_PTR_TYPE        pDSB         +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  pDSB       : Pointer to DeviceSetupBlock                         (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_OK_PENDING                                        +*/
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDDP_SRV_DEV_SETUP                                        +*/
/*+               Setup Device for CRT                                      +*/
/*+                                                                         +*/
/*+               Init the PNIP-Register for CRT.                           +*/
/*+               Activate PPM, CPM.                                        +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTDeviceSetup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DSB_PTR_TYPE     pDSB)
{
    EDD_RSP         Response;

    EDDP_CRT_TRACE_02(pDDB->TraceIdx,  LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CRTDeviceSetup(pDDB=0x%X, pDSB=0x%X)", pDDB, pDSB);

    Response = EDD_STS_OK;

    // ***** init HW RAM for provider
    Response = EDDP_CRTInitProviderDevSetup (pDDB, pDSB);

    if (Response == EDD_STS_OK)
    {   // ***** init HW RAM for consumer
        Response = EDDP_CRTInitConsumerDevSetup (pDDB, pDSB);

        if (Response == EDD_STS_OK)
        {   // ***** init HW RAM for inbound
            Response = EDDP_CRTInitInboundDevSetup(pDDB, pDSB);

            if (Response == EDD_STS_OK)
            {   // ***** init PNIP-Register
                Response = EDDP_CRTInitPnipRegDevSetup (pDDB, pDSB);
            }
        }
    }

    if (Response == EDD_STS_OK)
    {
        // ***** set DEV_SETUP-Bit
        EDDP_SET_BIT_VALUE32 ( (pDDB->CRT.BitFlag), 
                                1, 
                                EDDP_CRT_BITFLAG_DEV_SETUP_MASK, 
                                EDDP_CRT_BITFLAG_DEV_SETUP_SHIFT)
    }

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTDeviceSetup(): Response=0x%X", Response);

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                 +*/
/*+  Functionname          :    EDDP_CRTDeviceShutdown                              +*/
/*+                                                                                 +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE           pDDB              +*/
/*+                                                                                 +*/
/*+  Result                :    EDD_RSP                                             +*/
/*+                                                                                 +*/
/*+---------------------------------------------------------------------------------+*/
/*+                                                                                 +*/
/*+  Input/Output:                                                                  +*/
/*+                                                                                 +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)          +*/
/*+                                                                                 +*/
/*+  Results    : EDD_STS_OK                                                        +*/
/*+               EDD_STS_OK_PENDING                                                +*/
/*+               EDD_STS_ERR_PARAM                                                 +*/
/*+                                                                                 +*/
/*+---------------------------------------------------------------------------------+*/
/*+                                                                                 +*/
/*+  Description: This function is called from CORE within service                  +*/
/*+               - EDDP_SRV_DEV_SHUTDOWN or                                        +*/
/*+               - EDDP_SRV_DEV_SETUP (only if previous setup shall be aborted)    +*/
/*+               Shutdown Interface of CRT                                         +*/
/*+                                                                                 +*/
/*+               Delete CRT container in APICtrl-RAM ot this device                +*/
/*+                                                                                 +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTDeviceShutdown(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP             Response;

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CRTDeviceShutdown(): isLateErrLSATimerRunning=0x%X", 
        pDDB->CRT.Inbound.isLateErrLSATimerRunning);

    Response = EDD_STS_OK;

    // delete CRT container in APICtrl-RAM ot this device
    EDDP_CRTDeleteHwApiDDB (pDDB);

    // stop the Inbound-LateErrLSATimer if it is running
    if (pDDB->CRT.Inbound.isLateErrLSATimerRunning)
    {
        EDDP_CRTLateErrorStopLSATimer (pDDB);
    }

    // delete DEV_SETUP-Bit
    EDDP_SET_BIT_VALUE32 ( (pDDB->CRT.BitFlag), 
                            0, 
                            EDDP_CRT_BITFLAG_DEV_SETUP_MASK, 
                            EDDP_CRT_BITFLAG_DEV_SETUP_SHIFT);

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTDeviceShutdown(): Response=0x%X", Response);

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTDeviceIFSetup                       +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE           pDDB      +*/
/*+                             EDDP_UPPER_DSB_PTR_TYPE           pDSB      +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  pDSB       : Pointer to DeviceSetupBlock                         (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_OK_PENDING                                        +*/
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDDP_SRV_DEV_IF_SETUP                                     +*/
/*+               Setup Interface or CRT                                    +*/
/*+                                                                         +*/
/*+               Activate PPM, CPM                                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTDeviceIFSetup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DSB_PTR_TYPE     pDSB)
{
    EDD_RSP         Response, Response2;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pDSB);
#endif

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CRTDeviceIFSetup(pDSB=0x%X)", pDSB);

    Response = EDD_STS_OK;

    /* Init management variables */
    pDDB->CRT.Cons.SBEventPending               = LSA_FALSE;
    pDDB->CRT.IndProvide.ConsIndicationPending  = LSA_FALSE;

    /* -------------------------------------------------------- */
    /* PPM_Starttime and PPM activate                           */
    /*  - only if providers are used                            */
    /* -------------------------------------------------------- */
    if (pDDB->CRT.CfgParam.ProviderMaxNumber > 0)
    {
        switch (EDDP_HALGetPnipRevision(pDDB))
        {
            #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
            case EDDP_HAL_HW_PNIP_REV1:
            {
                /* NOTE: this command is processed on SYNC queue on NOT on CRT queue!!! */
                EDDP_CMD_PARAM_PTR_TYPE pCmdParam = &pDDB->CRT.CmdParam.Ppm_Starttime;
                Response = EDDP_CRTCmdRequestPpm (pDDB, EDDP_CRT_CMD_PPM_STARTTIME, EDDP_CRT_CMD_SYNCHRON, pCmdParam);
            }
            break;
            #endif

            #if defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
            case EDDP_HAL_HW_PNIP_REV2:
                // no PPM_Starttime command (in PN-IP Rev2 it is already set by HW init)
            break;
            #endif
            
            default:
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTDeviceIFSetup(): FATAL -> Version(%d) of PNIP is unknown", 
                    EDDP_HALGetPnipRevision(pDDB));
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            }
            break;
        }

        if (Response == EDD_STS_OK)
        {
            EDDP_CMD_PARAM_PTR_TYPE pCmdParam = EDDP_CRTCmdGetInternalCmdParamElement(pDDB);
            Response = EDDP_CRTCmdRequestPpm (pDDB, EDDP_CRT_CMD_PPM_ON, EDDP_CRT_CMD_SYNCHRON, pCmdParam);
        }
    }

    /* -------------------------------------------------------- */
    /* CPM activate                                             */
    /*  - only if consumers are used                            */
    /* -------------------------------------------------------- */
    if (pDDB->CRT.CfgParam.ConsumerMaxNumber > 0)
    {   
        Response2 = EDDP_CRTCmdRequestCpm (pDDB, EDDP_CRT_CMD_CPM_ON, EDDP_CRT_CHAIN_IDX_END, LSA_NULL);

        if (Response == EDD_STS_OK)
        {
            Response = Response2;
        }
    }

    /* -------------------------------------------------------- */
    /* ApplTimerIfSetup state machine only for TransferEnd mode */
    /* -------------------------------------------------------- */
    if ( (Response == EDD_STS_OK) && (EDDP_APPL_TIMER_CFG_MODE_TRANSFER_END == EDDP_CRTGetApplTimerMode(pDDB)) )
    {
        // start the ApplTimerIfSetup state machine
        EDDP_ASSERT_FALSE(pDDB->CRT.ApplTimerIfSetupSm.State != EDDP_CRT_APPL_TIMER_STATE_IDLE);
        pDDB->CRT.ApplTimerIfSetupSm.State  = EDDP_CRT_APPL_TIMER_STATE_START;
        pDDB->CRT.ApplTimerIfSetupSm.Cbf    = EDDP_CoreCBFDeviceSetup;
        EDDP_CRTApplTimerIfSetupStateMachine (pDDB, EDD_STS_OK);
        // set Response for caller
        Response = EDD_STS_OK_PENDING;
    }
    else
    {
        if (Response == EDD_STS_OK)
        {
            // signal start/finish of SendClockChange to application (without actually changing the SendClock on startup)
            EDDP_SIGNAL_SENDCLOCK_CHANGE (pDDB->hSysDev, pDDB->SWI.CycleInfo.CycleBaseFactor, EDDP_SENDCLOCK_CHANGE_MODE_STARTING);
            EDDP_SIGNAL_SENDCLOCK_CHANGE (pDDB->hSysDev, pDDB->SWI.CycleInfo.CycleBaseFactor, EDDP_SENDCLOCK_CHANGE_MODE_FINISHED);
        }
    }

    if ((Response == EDD_STS_OK) || (Response == EDD_STS_OK_PENDING))
    {
        // set DEV_IF_SETUP-Bit
        EDDP_SET_BIT_VALUE32 ( (pDDB->CRT.BitFlag), 
                               1, 
                               EDDP_CRT_BITFLAG_DEV_IF_SETUP_MASK, 
                               EDDP_CRT_BITFLAG_DEV_IF_SETUP_SHIFT)
    }

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTDeviceIFSetup(): Response=0x%X", Response);

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTDeviceIFShutdown                                +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE           pDDB                  +*/
/*+                                                                                     +*/
/*+  Result                :    EDD_RSP                                                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)              +*/
/*+  pDSB       : Pointer to DeviceSetupBlock                         (in)              +*/
/*+                                                                                     +*/
/*+  Results    : EDD_STS_OK                                                            +*/
/*+               EDD_STS_OK_PENDING                                                    +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function is called from CORE within service                      +*/
/*+               - EDDP_SRV_DEV_IF_SHUTDOWN or                                         +*/
/*+               - EDDP_SRV_DEV_IF_SETUP (only if previous setup shall be aborted)     +*/
/*+               Shutdown Interface of CRT                                             +*/
/*+                                                                                     +*/
/*+               Check provider or consumer are available in CRT management            +*/
/*+               If available, FATAL                                                   +*/
/*+                                                                                     +*/
/*+               Stop timer of CRT                                                     +*/
/*+                                                                                     +*/
/*+               Deactivate PPM, CPM                                                   +*/
/*+                                                                                     +*/
/*+               Delete CRT container in APICtrl-RAM of this interface                 +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTDeviceIFShutdown(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP         Response, Response2;

    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CRTDeviceIFShutdown()");

    Response = EDD_STS_OK;

    // are there some providers or consumers in this interface ?
    if (EDDP_CRTCheckIsProviderConsumerPresent(pDDB))
    {
        // user has to remove all provider and consumer before CloseChannel
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTDeviceIFShutdown(): FATAL -> Provider or Consumer available in this interface");
        EDDP_FATAL(EDDP_FATAL_ERR_DEV_IF_SHUTDOWN);
    }

    // we expect the (ACW_Treductionselect = 0) here
    if (pDDB->CRT.Prov.ACW_Treductionselect != 0)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTDeviceIFShutdown(): FATAL -> Prov.ACW_Treductionselect(0x%X) != 0", 
            pDDB->CRT.Prov.ACW_Treductionselect);
        EDDP_FATAL(EDDP_FATAL_ERR_DEV_IF_SHUTDOWN);
    }

    /* -------------------------------------------- */
    /* stop Timer of CRT                            */
    /* -------------------------------------------- */
    EDDP_CoreTimerStop (pDDB->CRT.Prov.PpmHolNotEmptyTimer);
    EDDP_CoreTimerStop (pDDB->CRT.Prov.PpmNotFinishedTimer);

    #ifdef EDDP_MESSAGE
    #ifdef EDDP_PRAGMA_MESSAGE
    #pragma EDDP_PRAGMA_MESSAGE("Workaround CRT.WrkarPackGrpDisable for PNIP-Revision(1) - AP01372025")
    #endif /* EDDP_PRAGMA_MESSAGE */
    #endif /* EDDP_MESSAGE */
    #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
    if (EDDP_HAL_HW_PNIP_REV1 == EDDP_HALGetPnipRevision(pDDB))
    {
        EDDP_CRTWrkarPackGrpDisableStopLSATimer (pDDB);
    }
    #endif

    /* -------------------------------------------- */
    /* PPM deactivate                               */
    /* -------------------------------------------- */
    {
        EDDP_CMD_PARAM_PTR_TYPE pCmdParam = EDDP_CRTCmdGetInternalCmdParamElement(pDDB);
        Response = EDDP_CRTCmdRequestPpm (pDDB, EDDP_CRT_CMD_PPM_OFF, EDDP_CRT_CMD_SYNCHRON, pCmdParam);
    }

    /* -------------------------------------------- */
    /* CPM deactivate                               */
    /* -------------------------------------------- */
    // ***** call SYNCHRON
    Response2 = EDDP_CRTCmdRequestCpm(pDDB, EDDP_CRT_CMD_CPM_OFF, EDDP_CRT_CHAIN_IDX_END, LSA_NULL);

    if (Response == EDD_STS_OK)
    {
        Response = Response2;
    }

    if (Response == EDD_STS_OK)
    {
        /* -------------------------------------------- */
        /* delete CRT container in APICtrl-RAM          */
        /* -------------------------------------------- */
        EDDP_CRTDeleteHwApiDDB (pDDB);

        // delete DEV_IF_SETUP-Bit
        EDDP_SET_BIT_VALUE32 ( (pDDB->CRT.BitFlag), 
                                0, 
                                EDDP_CRT_BITFLAG_DEV_IF_SETUP_MASK, 
                                EDDP_CRT_BITFLAG_DEV_IF_SETUP_SHIFT)
    }

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTDeviceIFShutdown(): Response=0x%X", Response);

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTIndicationProvide                   +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                         +*/
/*+                                                                         +*/
/*+  EDD_RQB_CSRT_IND_PROVIDE_TYPE                                          +*/
/*+                                                                         +*/
/*+  Result     :    EDD_STS_OK                                             +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function is called by service EDD_SRV_SRT_IND_PROVIDE+*/
/*+               and saves the Indication-RQB in the Indication-Queue      +*/
/*+               of the CRT.                                               +*/
/*+                                                                         +*/
/*+               Check if indications of provider/consumer are pending.    +*/
/*+               If yes indicate them.                                     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTIndicationProvide(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP                                 Response;
    EDDP_DDB_CRT_PTR_TYPE                   pCRTMngm;
    EDD_UPPER_CSRT_IND_PROVIDE_PTR_TYPE     pIndParam;

    Response        = EDD_STS_OK;
    pCRTMngm    = &pDDB->CRT;
    pIndParam       = (EDD_UPPER_CSRT_IND_PROVIDE_PTR_TYPE) pRQB->pParam;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pHDB);
#endif

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_CRTIndicationProvide(pRQB=0x%X)", pHDB->Handle, pRQB);

    // clear parameter block of new indication RQB
    EDDP_MEMSET_COMMON(pIndParam, 0, (sizeof(EDD_RQB_CSRT_IND_PROVIDE_TYPE)));

    // put this RQB to Indication Queue
    EDDP_GlbRQBQueueAddToEnd (&(pCRTMngm->IndProvide.RqbQueue), pRQB);

    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_CRTIndicationProvide(): ConsIndicationPending=0x%X, new RqbQueue.Count=%d", 
        pHDB->Handle, pCRTMngm->IndProvide.ConsIndicationPending, pCRTMngm->IndProvide.RqbQueue.Count);

    /* -------------------------------------------------------------------------------- */
    /* Look if any consumer indications are pending. EDDP_CRTConsumerCheckIndication()  */
    /* checks internally that an indication RQB is existing, because the call above of  */
    /* EDDP_CRTProviderIndDropEvent() can the added RQB to use up beforehand.           */
    /* -------------------------------------------------------------------------------- */
    if (pCRTMngm->IndProvide.ConsIndicationPending)
    {
        EDDP_CRTConsumerCheckIndication(pDDB);
    }

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTIndicationProvide(): Response=0x%X", Response);

    return (Response);
}
