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
/*  F i l e               &F: eddp_core_oc.c                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP Device and handle-management functions      */
/*                                                                           */
/*                          - edd_open_channel()                             */
/*                          - edd_close_channel()                            */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  20.08.08    JS    initial version.                                       */
/*  22.12.08    LRG   #ifdef EDDP_USE_K32                                    */
/*  24.02.09    LRG   UsedComp.UseK32                                        */
/*  20.07.10    JS    Use EDDP_REQUEST_UPPER_DONE() for finishing requests   */
/*  04.11.10    JS    In OPEN_CANNEL call RELEASE_PATH_INFO after CBF on err */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID  7                 
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_CORE_OC */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"            /* eddp headerfiles */
#include "eddp_int.h"            /* internal header  */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID) 

/*===========================================================================*/
/*                                global Variables                           */
/*===========================================================================*/

/*===========================================================================*/
/*                                  Types                                    */
/*===========================================================================*/

/* --------------------------------------------------------------------------*/
/* CLOSE_CHANNEL Fct-table                                                   */
/* --------------------------------------------------------------------------*/

typedef struct _EDDP_HDB_CORE_CLOSE_CHANNEL_FCT_TBL_TYPE
{
    EDD_RSP                 LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, CloseChannelFct)
                                       (EDDP_LOCAL_DDB_PTR_TYPE      pDDB,
                                        EDDP_LOCAL_HDB_PTR_TYPE      pHDB,
                                        LSA_HANDLE_TYPE              Handle);

    LSA_UINT32              UsedComp; // e.g.EDDP_COMP_NRT

} EDDP_HDB_CORE_CLOSE_CHANNEL_FCT_TBL_TYPE;

/*===========================================================================*/
/*                                Constants                                  */
/*===========================================================================*/

/* --------------------------------------------------------------------------*/
/* CLOSE_CHANNEL Fct-table                                                   */
/* --------------------------------------------------------------------------*/

static EDDP_HDB_CORE_CLOSE_CHANNEL_FCT_TBL_TYPE  EDDP_CoreCloseChannelFctTable[] =
{
    { EDDP_CRTCloseChannel	, EDDP_COMP_CRT},
    { EDDP_NRTCloseChannel	, EDDP_COMP_NRT},  
    { EDDP_PRMCloseChannel	, EDDP_COMP_PRM},
    { EDDP_SWICloseChannel	, EDDP_COMP_SWI},
    { EDDP_GENCloseChannel	, EDDP_COMP_GEN},
    { EDDP_K32CloseChannel	, EDDP_COMP_K32},
    { LSA_NULL				, 0			   }/* end of list */
};

/*===========================================================================*/
/*                                 functions                                 */
/*===========================================================================*/

/*===========================================================================*/
/*                                 Macros                                    */
/*===========================================================================*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_open_channel                           +*/
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
/*+     LSA_OPCODE_TYPE         Opcode:     EDD_OPC_OPEN_CHANNEL            +*/
/*+     LSA_HANDLE_TYPE         Handle:                                     +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user (not used)           +*/
/*+     EDD_SERVICE             Service:    not used                        +*/
/*+     LSA_RESPONSE_TYPE       Status:     Return status                   +*/
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:     Depend on kind of request.      +*/
/*+                                                                         +*/
/*+  pParam points to EDD_RQB_OPEN_CHANNEL_TYPE                             +*/
/*+                                                                         +*/
/*+     LSA_HANDLE_TYPE         Handle:         channel-handle of EDD (ret) +*/
/*+     LSA_HANDLE_TYPE         HandleUpper:    channel-handle of user      +*/
/*+     LSA_SYS_PATH_TYPE       SysPath:        system-path of channel      +*/               
/*+     LSA_VOID                LSA_FCT_PTR(EDD_UPPER_OUT_FCT_ATTR, Cbf)    +*/
/*+                                         (EDD_UPPER_RQB_PTR_TYPE pRQB)   +*/
/*+                                             callback-function           +*/
/*+  RQB-return values via callback-function:                               +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         opcode:     EDD_OPC_OPEN_CHANNEL            +*/
/*+     LSA_HANDLE_TYPE         Handle:     returned channel of user        +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user                      +*/
/*+     EDD_SERVICE             Service:    not changed                     +*/
/*+     LSA_RESPONSE_TYPE       Status:     EDD_STS_OK                      +*/
/*+                                         EDD_STS_ERR_RESOURCE            +*/
/*+                                         EDD_STS_ERR_SYS_PATH            +*/
/*+                                         EDD_STS_ERR_PARAM               +*/
/*+                                         EDD_STS_ERR_LOWER_LAYER         +*/
/*+                                         EDD_STS_ERR_OPCODE              +*/
/*+                                         EDD_STS_ERR_CHANNEL_USE         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Request to open a channel of a device.                    +*/
/*+                                                                         +*/
/*+               Within this request the output - macro EDD_GET_PATH_INFO  +*/
/*+               is used to get further channel-descriptons and parameters +*/
/*+               (e.g. the device-handle).                                 +*/
/*+                                                                         +*/
/*+               After a successful open a handle is channel-handle is     +*/
/*+               returned and channel-dependend requests can be used.      +*/
/*+                                                                         +*/
/*+               HandleUpper:                                              +*/
/*+                                                                         +*/
/*+               This handle will be returned in the RQB in all further    +*/
/*+               request-confirmations.                                    +*/
/*+                                                                         +*/
/*+               cbf:                                                      +*/
/*+                                                                         +*/
/*+               The request is always finished by calling the specified   +*/
/*+               callback-function. This callback-funktion will also be    +*/
/*+               used for all other other future requests for this channel.+*/
/*+                                                                         +*/
/*+               SysPath:                                                  +*/
/*+                                                                         +*/
/*+               The SysPath variable will not be used inside but given to +*/
/*+               systemadaption via EDD_GET_PATH_INFO.                     +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+  Note: This functions is synchronously. Each Subcomponent have to be    +*/
/*+        synchronously. a EDD_STS_OK_PENDING is not allowed!              +*/
/*+                                                                         +*/
/*+  Note on error-handling:                                                +*/
/*+                                                                         +*/
/*+               In some cases it is not possible to call the call-back-   +*/
/*+               function of the requestor to confirm the request. in this +*/
/*+               case we call the output-macro EDDP_RQB_ERROR with the RQB +*/
/*+               to notify this error. This erros are most likely caused   +*/
/*+               by a implementation error an are of the type              +*/
/*+                                                                         +*/
/*+               EDD_STS_ERR_OPCODE                                        +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDD_UPPER_IN_FCT_ATTR eddp_open_channel(
    EDD_UPPER_RQB_PTR_TYPE      pRQB)
{
    EDD_RSP                             Response;
    EDD_RSP                             Response2;
    LSA_UINT32                          i;
    LSA_HANDLE_TYPE                     Handle;
    EDDP_LOCAL_HDB_PTR_TYPE             pHDB;
    LSA_SYS_PTR_TYPE                    pSys = {0};
    EDDP_DETAIL_PTR_TYPE                pDetail;
    EDDP_LOCAL_DDB_PTR_TYPE             pDDB;
    LSA_UINT16                          PathStat;
    LSA_BOOL                            HandleInit;
    LSA_BOOL                            SysPathInit;
    LSA_UINT16                          RelStat;
    LSA_BOOL                            ReleasePath;
    EDD_UPPER_OPEN_CHANNEL_PTR_TYPE     pRQBChannel;

    EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] --> eddp_open_channel(pRQB=0x%X)", pRQB);

    EDDP_ENTER_LOW_S();

    HandleInit  = LSA_FALSE;
    SysPathInit = LSA_FALSE;
    ReleasePath = LSA_FALSE;
    Response    = EDD_STS_OK;
    pRQBChannel = LSA_NULL;
    pDetail     = LSA_NULL;  /* access prevents compiler warning */
    pDDB        = LSA_NULL;  /* access prevents compiler warning */

    /*---------------------------------------------------------------------------*/
    /* Check for valid parameters. Here we cant use the call-back-function       */
    /* on error.                                                                 */
    /*---------------------------------------------------------------------------*/
    EDDP_UPPER_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE,"[H:--] >>> eddp_open_channel");

    if (EDD_RQB_GET_OPCODE(pRQB) != EDD_OPC_OPEN_CHANNEL)
    {
        EDDP_UPPER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_UNEXP, "[H:--] <<< eddp_open_channel: Invalid RQB-Opcode(0x%X)",
            EDD_RQB_GET_OPCODE(pRQB));
        EDD_RQB_SET_RESPONSE(pRQB, EDD_STS_ERR_OPCODE);
        Response = EDD_STS_ERR_OPCODE;
        EDDP_RQB_ERROR(pRQB);
    }
    else 
    {
        if (LSA_HOST_PTR_ARE_EQUAL(pRQB->pParam, LSA_NULL))
        {
            EDDP_UPPER_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] <<< eddp_open_channel(): ERROR -> pParam is NULL");
            EDD_RQB_SET_RESPONSE(pRQB, EDD_STS_ERR_PARAM);
            Response = EDD_STS_ERR_PARAM;
            EDDP_RQB_ERROR(pRQB);
        }
        else
        {
            pRQBChannel = (EDD_UPPER_OPEN_CHANNEL_PTR_TYPE) pRQB->pParam;

            if (0 == pRQBChannel->Cbf)
            {
                EDDP_UPPER_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] <<< eddp_open_channel(): ERROR -> Cbf is NULL");
                EDD_RQB_SET_RESPONSE(pRQB, EDD_STS_ERR_PARAM);
                Response = EDD_STS_ERR_PARAM;
                EDDP_RQB_ERROR(pRQB);
            }
        }
    }

    /*---------------------------------------------------------------------------*/
    /* If parameters are valid, we have a call-back-function now.                */
    /*---------------------------------------------------------------------------*/
    if (Response == EDD_STS_OK) 
    {
        /*-----------------------------------------------------------------------*/
        /* Get a Handle. On error abort                                          */
        /*-----------------------------------------------------------------------*/
        Response = EDDP_CoreDevHandleNew (&Handle, &pHDB);

        if (Response == EDD_STS_OK)
        {
            HandleInit          = LSA_TRUE;
            /* ini HDB (Handle-Discription-Block) */
            /* Note: Already init with 0!         */
            pHDB->InUse         = LSA_TRUE;
            pHDB->UsedComp      = 0;
            pHDB->SysPath       = pRQBChannel->SysPath;
            pHDB->UpperHandle   = pRQBChannel->HandleUpper;
            pHDB->Handle        = Handle;
            pHDB->Cbf           = pRQBChannel->Cbf;
            pHDB->pDDB          = LSA_NULL;
        }
        else
        {
            EDDP_UPPER_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] eddp_open_channel(): FATAL -> Cannot get free EDDP handle");
            EDDP_FATAL(EDDP_FATAL_ERR_OPEN_CHANNEL);
        }

        /*-----------------------------------------------------------------------*/
        /* Call LSA Output-macro..                                               */
        /*-----------------------------------------------------------------------*/
        if (Response == EDD_STS_OK)
        {
            EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX,  LSA_TRACE_LEVEL_NOTE, "[H:%2X] --> Calling EDDP_GET_PATH_INFO.",
                pHDB->Handle);
            EDDP_GET_PATH_INFO(&PathStat, &pSys, &pDetail, pRQBChannel->SysPath);
            
            if (LSA_HOST_PTR_ARE_EQUAL(pDetail, LSA_NULL))
            {
                EDDP_UPPER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:%2X] eddp_open_channel(): ERROR -> EDD_GET_PATH_INFO() pDetail is NULL",
                    pHDB->Handle);
                Response = EDD_STS_ERR_PARAM;
            }
            else if (PathStat != LSA_RET_OK)
            {
                EDDP_SYSTEM_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:%2X] <-- EDDP_GET_PATH_INFO failed (Rsp: 0x%X).",
                    pHDB->Handle, PathStat);
                EDDP_UPPER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:%2X] eddp_open_channel(): FATAL -> EDD_GET_PATH_INFO failed",
                    pHDB->Handle);
                EDDP_FATAL(EDDP_FATAL_ERR_OPEN_CHANNEL);
                Response = EDD_STS_ERR_EXCP;
            } 
            else if (   (   (EDD_CDB_CHANNEL_USE_ON != pDetail->UsedComp.UsePRM)
                && (EDD_CDB_CHANNEL_USE_OFF != pDetail->UsedComp.UsePRM))
                || (   (EDD_CDB_CHANNEL_USE_ON != pDetail->UsedComp.UseNRT)
                    && (EDD_CDB_CHANNEL_USE_OFF != pDetail->UsedComp.UseNRT))
                || (   (EDD_CDB_CHANNEL_USE_ON != pDetail->UsedComp.UseCRT)
                    && (EDD_CDB_CHANNEL_USE_OFF != pDetail->UsedComp.UseCRT))
                || (   (EDD_CDB_CHANNEL_USE_ON != pDetail->UsedComp.UseK32)
                    && (EDD_CDB_CHANNEL_USE_OFF != pDetail->UsedComp.UseK32))
                )
            {
                EDDP_SYSTEM_TRACE_05(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:%2X] <-- EDDP_GET_PATH_INFO illegal value for UsePRM (%d)/UseNRT(%d)/UseNRT(%d)/UseK32(%d)",
                    pHDB->Handle, pDetail->UsedComp.UsePRM, pDetail->UsedComp.UseNRT, pDetail->UsedComp.UseCRT, pDetail->UsedComp.UseK32);
                EDDP_UPPER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:%2X] eddp_open_channel(): FATAL -> EDD_GET_PATH_INFO failed",
                    pHDB->Handle);
                EDDP_FATAL(EDDP_FATAL_ERR_OPEN_CHANNEL);
                Response = EDD_STS_ERR_EXCP;
            }
            else if (EDD_CDB_CHANNEL_USE_ON == pDetail->UsedComp.UsePRM)
            {
                if (!pDetail->pPRM)
                {
                    EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:%2X] <-- EDDP_GET_PATH_INFO pDetail->pPRM NULLPtr)",
                        pHDB->Handle);
                    EDDP_UPPER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:%2X] eddp_open_channel(): FATAL -> EDD_GET_PATH_INFO failed",
                        pHDB->Handle);
                    EDDP_FATAL(EDDP_FATAL_ERR_OPEN_CHANNEL);
                    Response = EDD_STS_ERR_EXCP;
                }
                else if ((EDD_SYS_PDPORTDATAADJUST_CHECK_FOR_PNIO_STANDARD != pDetail->pPRM->PortDataAdjustLesserCheckQuality)
                    && (EDD_SYS_PDPORTDATAADJUST_LESSER_CHECK_QUALITY != pDetail->pPRM->PortDataAdjustLesserCheckQuality))
                {
                    EDDP_SYSTEM_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:%2X] <-- EDDP_GET_PATH_INFO PortDataAdjustLesserCheckQuality(%d)",
                        pHDB->Handle, pDetail->pPRM->PortDataAdjustLesserCheckQuality);
                    EDDP_UPPER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:%2X] eddp_open_channel(): FATAL -> EDD_GET_PATH_INFO failed",
                        pHDB->Handle);
                    EDDP_FATAL(EDDP_FATAL_ERR_OPEN_CHANNEL);
                    Response = EDD_STS_ERR_EXCP;
                }
            }
            
            if (EDD_STS_OK == Response)
            {
                EDDP_SYSTEM_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:%2X] <-- EDD_GET_PATH_INFO success (Detail-Ptr: 0x%X).",
                    pHDB->Handle, pDetail);

                SysPathInit     = LSA_TRUE;
                pHDB->pSys      = pSys;
                pHDB->pDetail   = pDetail;
            }
        }

        /*-----------------------------------------------------------------------*/
        /* Get pointer to DDB                                                    */
        /*-----------------------------------------------------------------------*/
        if (Response == EDD_STS_OK)
        {
            Response = EDDP_CoreDevGetDDB (pDetail->hDDB, &pDDB);

            if (Response != EDD_STS_OK)
            {
                EDDP_UPPER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:%2X] eddp_open_channel(): ERROR -> Invalid hDDB(0x%X) within CDB",
                    pHDB->Handle, pDetail->hDDB);
                Response = EDD_STS_ERR_PARAM;
            }
            else
            {
                if (pDDB->DevIsSetup ) /* check if device is setup */
                {
                    /* save references to DDB within HDB */
                    pHDB->pDDB  = pDDB;
                    
                    EDDP_ASSERT_NO_NULL_PTR(pDDB);
                }
                else
                {
                    EDDP_UPPER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:%2X]  eddp_open_channel(): ERROR -> EDDP device not SETUP yet!", 
                        pHDB->Handle);
                    Response = EDD_STS_ERR_SEQUENCE;    
                }
            }
        }

        /*-----------------------------------------------------------------------*/
        /* pDDB now valid. Init components                                       */
        /*-----------------------------------------------------------------------*/
        /*-----------------------------------------------------------------------*/
        /* Init   GEN Component                                                  */
        /*-----------------------------------------------------------------------*/
        if (Response == EDD_STS_OK)
        {
            /* open channel for GEN */
            Response = EDDP_GENOpenChannel (pDDB, pHDB, Handle, pDetail);

            if (Response == EDD_STS_OK)
            {               
                pHDB->UsedComp |= EDDP_COMP_GEN;
            }   
            else
            {
                EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
                EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:%2X] eddp_open_channel(): ERROR -> EDDP_GENOpenChannel failed (Response=0x%X)", 
                    pHDB->Handle, Response);
            }
        }	    

        /*-----------------------------------------------------------------------*/
        /* Init   PRM Component.                                                 */
        /*-----------------------------------------------------------------------*/
        if (Response == EDD_STS_OK)
        {
            if (EDD_CDB_CHANNEL_USE_ON == pDetail->UsedComp.UsePRM)
            {
                /* open channel for PRM */
                Response = EDDP_PRMOpenChannel (pDDB, pHDB, Handle, pDetail);

                if (Response == EDD_STS_OK)
                {               
                    pHDB->UsedComp |= EDDP_COMP_PRM;
                }   
                else
                {
                    EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
                    EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:%2X] eddp_open_channel(): ERROR -> EDDP_PRMOpenChannel failed (Response=0x%X)", 
                        pHDB->Handle, Response);
                }
            }
        }

        /*-----------------------------------------------------------------------*/
        /* Init   NRT Component.                                                 */
        /*-----------------------------------------------------------------------*/
        if (Response == EDD_STS_OK)
        {
            if (EDD_CDB_CHANNEL_USE_ON == pDetail->UsedComp.UseNRT)
            {
                /* open channel for NRT */
                Response = EDDP_NRTOpenChannel (pDDB, pHDB, Handle, pDetail);

                if (Response == EDD_STS_OK)
                {               
                    pHDB->UsedComp |= EDDP_COMP_NRT;
                }   
                else
                {
                    EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
                    EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:%2X] eddp_open_channel(): ERROR -> EDDP_NRTOpenChannel failed (Response=0x%X)", 
                        pHDB->Handle, Response);
                }
            }
        }

        /*-----------------------------------------------------------------------*/
        /* Init  CRT  Component.                                                 */
        /*-----------------------------------------------------------------------*/
        if (Response == EDD_STS_OK)
        {
            if (pDetail->UsedComp.UseCRT)
            {
                /* open channel for CRT */
                Response = EDDP_CRTOpenChannel (pDDB, pHDB, Handle, pDetail);

                if (Response == EDD_STS_OK)
                {               
                    pHDB->UsedComp |= EDDP_COMP_CRT;
                }   
                else
                {
                    EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
                    EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:%2X] eddp_open_channel(): ERROR -> EDDP_CRTOpenChannel failed (Response=0x%X)", 
                        pHDB->Handle, Response);
                }
            }
        }

        /*-----------------------------------------------------------------------*/
        /* Init  SWI  Component.                                                 */
        /*-----------------------------------------------------------------------*/
        if (Response == EDD_STS_OK)
        {
                /* open channel for SWI */
                Response = EDDP_SWIOpenChannel (pDDB, pHDB, Handle, pDetail);

                if (Response == EDD_STS_OK)
                {
                    pHDB->UsedComp |= EDDP_COMP_SWI;
                }   
                else
                {
                    EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
                    EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:%2X] eddp_open_channel(): ERROR -> EDDP_SWIOpenChannel failed (Response=0x%X)", 
                        pHDB->Handle,Response);
                }
        }

        /*-----------------------------------------------------------------------*/
        /* Init  KRISC32 Component.                                              */
        /*-----------------------------------------------------------------------*/
        if (Response == EDD_STS_OK)
        {
            if (pDetail->UsedComp.UseK32)
            {
                /* open channel for KRISC32 */
                Response = EDDP_K32OpenChannel (pDDB, pHDB, Handle, pDetail);

                if (Response == EDD_STS_OK)
                {               
                    pHDB->UsedComp |= EDDP_COMP_K32;
                }   
                else
                {
                    EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
                    EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:%2X] eddp_open_channel(): ERROR -> EDDP_K32OpenChannel failed (Resp: 0x%X)", pHDB->Handle, Response);
                }
            }
        }

        /*-----------------------------------------------------------------------*/
        /* On error we have to undo several things..                             */
        /* (without statuscheck, because we cant do anything .)                  */
        /*-----------------------------------------------------------------------*/
        if (Response != EDD_STS_OK)
        {
            EDDP_UPPER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_UNEXP, "[H:--] eddp_open_channel(): failed (Response=0x%X). Closing components.",
                Response);

            if (HandleInit)
            {
                if (SysPathInit) /* if init */
                {
                    i = 0;

                    /* ---------------------------------------------------------*/
                    /* Close all open Subcomponents from FctTable if Opened     */
                    /* ---------------------------------------------------------*/
                    /* LSA_NULL if table end.. */
                    while (! (0 == EDDP_CoreCloseChannelFctTable[i].CloseChannelFct))
                    {
                        if (pHDB->UsedComp & EDDP_CoreCloseChannelFctTable[i].UsedComp)
                        {
                            EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] eddp_open_channel[I:%d]. Calling DeviceCloseFct",
                                i);

                            /* Call Subcomponente CloseChannelFct*/
                            Response2 = EDDP_CoreCloseChannelFctTable[i].CloseChannelFct(pHDB->pDDB,pHDB,Handle);

                            if (Response2 != EDD_STS_OK)    /* here no error expected! and no PENDING! */
                            {
                                EDDP_UPPER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_UNEXP, "[H:--] eddp_open_channel[I:%d]: Cannot close channel after error (Response=0x%X)!",
                                    i, Response);
                                EDDP_FATAL(EDDP_FATAL_ERR_OPEN_CHANNEL);
                            }
                            else
                            {
                                pHDB->UsedComp &= ~ EDDP_CoreCloseChannelFctTable[i].UsedComp;
                                EDDP_UPPER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] eddp_open_channel[I:%d]. Calling CloseChannelFct successful",
                                    i);
                            }
                        }
                        else
                        {
                            EDDP_UPPER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] eddp_open_channel[I:%d]. Already closed. Skipping.",
                                i);
                        }
                        i++;
                    }
                    /* ---------------------------------------------------------*/
                    ReleasePath = LSA_TRUE; /* Release Path info later ..*/
                }

                EDDP_CoreDevHandleRel (pHDB->Handle); /* free handle */
            }
        }
        else
        {
            pDDB->Core.HandleCnt++; // number of handles used by Interface 
            EDDP_UPPER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] eddp_open_channel(): EDD_OPC_OPEN_CHANNEL successful!", Handle);
        }

        pRQBChannel->HandleLower = pHDB;  /* set EDD Lower Handle */

        EDDP_UPPER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] <<< eddp_open_channel(): Response=0x%X", Response);

        EDD_RQB_SET_HANDLE(         pRQB, pRQBChannel->HandleUpper);
        EDD_RQB_SET_RESPONSE(       pRQB, Response);

        /* Note: pSys must be valid here. All errors will result in FATAL which */
        /*       cause pSys to be invalid                                       */

        EDDP_GlbCallChannelCbf (pRQBChannel->Cbf, pRQB, pDDB, pSys); /* Note: pDDB may be LSA_NULL. Allowed here! */

        /* Call LSA-Release Path Info. This is done only on error   */
        /* must be done after call of CBF!                          */
        if (ReleasePath)
        {
            EDDP_SYSTEM_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] --> Calling EDD_RELEASE_PATH_INFO.");
            EDDP_RELEASE_PATH_INFO(&RelStat,pSys,pDetail);
            EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] <-- EDD_RELEASE_PATH_INFO (Response=0x%X)", RelStat);
        }

    }

    EDDP_EXIT_LOW_S();

    EDDP_SYSTEM_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] <-- eddp_open_channel()");
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_close_channel                          +*/
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
/*+     LSA_OPCODE_TYPE         Opcode:     EDD_OPC_CLOSE_CHANNEL           +*/
/*+     LSA_HANDLE_TYPE         Handle:     valid channel-handle            +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user (not used)           +*/
/*+     EDD_SERVICE             Service:    not used                        +*/
/*+     LSA_RESPONSE_TYPE       Status:     Return status                   +*/
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:     LSA_NULL                        +*/
/*+                                                                         +*/
/*+  RQB-return values via callback-function:                               +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         opcode:     EDD_OPC_CLOSE_CHANNEL           +*/
/*+     LSA_HANDLE_TYPE         Handle:     upper-handle from open_channel  +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user                      +*/
/*+     EDD_SERVICE             Service:    not changed                     +*/
/*+     LSA_RESPONSE_TYPE       Status:     EDD_STS_OK                      +*/
/*+                                         EDD_STS_ERR_PARAM               +*/
/*+                                         EDD_STS_ERR_SEQUENCE            +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Closes an channel.                                        +*/
/*+                                                                         +*/
/*+               Calls every components Close-Channel function. Every      +*/
/*+               component checks if it is used for this handle and if     +*/
/*+               so it trys to close the channel. if it fails the component+*/
/*+               returns an error-status.                                  +*/
/*+                                                                         +*/
/*+  Note on error-handling:                                                +*/
/*+                                                                         +*/
/*+               In some cases it is not possible to call the call-back-   +*/
/*+               function of the requestor to confirm the request. in this +*/
/*+               case we call the output-macro EDDP_RQB_ERROR with the RQB +*/
/*+               to notify this error. This erros are most likely caused   +*/
/*+               by a implementation error an are of the type              +*/
/*+                                                                         +*/
/*+               EDD_STS_ERR_OPCODE                                        +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDD_UPPER_IN_FCT_ATTR eddp_close_channel(
    EDD_UPPER_RQB_PTR_TYPE      pRQB)
{
    EDD_RSP                             Response;
    LSA_UINT16                          Status;
    EDDP_LOCAL_HDB_PTR_TYPE             pHDB;
    EDDP_LOCAL_DDB_PTR_TYPE             pDDB;
    LSA_HANDLE_TYPE                     Handle;
    LSA_SYS_PTR_TYPE                    pSys;
    EDDP_DETAIL_PTR_TYPE                pDetail;
    LSA_HANDLE_TYPE                     UpperHandle;
    LSA_VOID                            LSA_FCT_PTR(EDD_UPPER_OUT_FCT_ATTR, Cbf) (EDD_UPPER_RQB_PTR_TYPE pRQB);
    LSA_UINT32                          i;
    LSA_BOOL                            Ready;

    EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] --> eddp_close_channel(pRQB=0x%X)", pRQB);

    EDDP_ENTER_LOW_S();

    Response    = EDD_STS_OK;
    Handle      = 0;        /* to prevent compiler bug on optimize */
    pHDB        = LSA_NULL; /* access prevents compiler warning */
    pDDB        = LSA_NULL;

    EDDP_UPPER_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] >>> eddp_close_channel()");

    /*---------------------------------------------------------------------------*/
    /* Check for valid parameters. Here we cant use the call-back-function       */
    /* on error.                                                                 */
    /*---------------------------------------------------------------------------*/
    if (EDD_RQB_GET_OPCODE(pRQB) != EDD_OPC_CLOSE_CHANNEL)
    {
        EDDP_UPPER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_UNEXP, "[H:--] <<< eddp_close_channel(): Invalid RQB-Opcode(0x%X)",
            EDD_RQB_GET_OPCODE(pRQB));

        EDD_RQB_SET_RESPONSE(pRQB, EDD_STS_ERR_OPCODE);
        Response = EDD_STS_ERR_OPCODE;
        EDDP_RQB_ERROR(pRQB);
    }
    else 
    {
        Response = EDDP_GlbHandleGetHDB (EDD_RQB_GET_HANDLE_LOWER(pRQB), &pHDB);

        if (Response != EDD_STS_OK)
        {
            EDDP_UPPER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_UNEXP, "[H:--] <<< eddp_close_channel(): Invalid Handle(0x%X)",
                Handle);
            EDD_RQB_SET_RESPONSE(pRQB, EDD_STS_ERR_PARAM);
            Response = EDD_STS_ERR_PARAM;
            EDDP_RQB_ERROR(pRQB);
        }
    }

    if (Response == EDD_STS_OK) 
    {
        UpperHandle = pHDB->UpperHandle;    /* save this because we will free HDB */
        Cbf         = pHDB->Cbf;            /* save this because we will free the HDB later on */
        pDDB        = pHDB->pDDB;
        pSys        = pHDB->pSys;           /* save this because we will free the HDB */
        pDetail     = pHDB->pDetail;        /* save this because we will free the HDB */
        i           = 0;
        Ready       = LSA_FALSE;

        /* ----------------------------------------------------------------------*/
        /* Close all open Subcomponents from FctTable if Opened                  */
        /* ----------------------------------------------------------------------*/
        /* LSA_NULL if table end.. */
        while (!Ready)
        {
            if (! (0 == EDDP_CoreCloseChannelFctTable[i].CloseChannelFct))
            {
                if (pHDB->UsedComp & EDDP_CoreCloseChannelFctTable[i].UsedComp)
                {
                    EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:%2X] eddp_close_channel() [I:%d]. Calling DeviceCloseFct",
                            pHDB->Handle, i);

                    /* Call Subcomponente CloseChannelFct*/
                    Response = EDDP_CoreCloseChannelFctTable[i].CloseChannelFct(pHDB->pDDB,pHDB,Handle);

                    if (Response != EDD_STS_OK) /* here no error expected! and no PENDING! */
                    {
                        EDDP_UPPER_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] eddp_close_channel() [I:%d]: ERROR -> Cannot close channel (Response=0x%X)!",
                                pHDB->Handle, i, Response);
                        Ready = LSA_TRUE;
                    }
                    else
                    {
                        pHDB->UsedComp &= ~ EDDP_CoreCloseChannelFctTable[i].UsedComp;

                        EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:%2X] eddp_close_channel() [I:%d]. Calling CloseChannelFct successful",
                                pHDB->Handle, i);
                    }
                }
                else
                {
                    EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:%2X] eddp_close_channel() [I:%d]. Already closed. Skipping.",
                            pHDB->Handle, i);
                }
            }
            else
            {
                Ready = LSA_TRUE;
            }

            i++;
        }
        /* ----------------------------------------------------------------------*/
    
        /*-----------------------------------------------------------------------*/
        /* If all channels successfully closed, we release the handle and call   */
        /* the LSA release path info-function.                                   */
        /*-----------------------------------------------------------------------*/
        if (Response == EDD_STS_OK)
        {
            Response = EDDP_CoreDevHandleRel (pHDB->Handle); /* free HDB. dont use pHDB anymore!   */

            if (Response == EDD_STS_OK)
            {
                EDDP_ASSERT(pDDB->Core.HandleCnt);
                pDDB->Core.HandleCnt--; // number of handles used by Interface 
            }
        } 
    
        /*-----------------------------------------------------------------------*/
        /* Finish the request. Note: dont use EDDRequestFinish() here, because   */
        /* pHDB may be released and invalid!!                                    */
        /*-----------------------------------------------------------------------*/
        EDDP_UPPER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] <<< eddp_close_channel(): Response=0x%X", Response);

        EDD_RQB_SET_RESPONSE(pRQB, Response);
        EDD_RQB_SET_HANDLE(pRQB, UpperHandle );

        /* Note: pSys must be valid here! */
        EDDP_GlbCallChannelCbf (Cbf, pRQB, pDDB, pSys); /* Note: pDDB may be LSA_NULL. Allowed here! */

        /*-----------------------------------------------------------------------*/
        /* Call Release Pathinfo after finishing the Request because pSys must   */
        /* be valid on finishing the request!                                    */
        /*-----------------------------------------------------------------------*/
        if (Response == EDD_STS_OK)
        {
            /* Call LSA-Release Path Info. This is done only on success */
            /* NOTE: If this fails we have already closed all things and*/
            /*       cant do anything so we ignore this error or fatal? */

            EDDP_SYSTEM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"[H:--] --> Calling EDD_RELEASE_PATH_INFO.");

            EDDP_RELEASE_PATH_INFO(&Status,pSys,pDetail);

            EDDP_SYSTEM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"[H:--] <-- EDD_RELEASE_PATH_INFO (Rsp=0x%X)", Status);

            if (Status != LSA_RET_OK)
            {
                EDDP_FATAL(EDDP_FATAL_ERR_RELEASE_PATH_INFO);
            }
        }
    }

    EDDP_EXIT_LOW_S();

    EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] <-- eddp_close_channel(Response=0x%X)", Response);
}

/*****************************************************************************/
/*  end of file eddp_core_oc.c                                               */
/*****************************************************************************/

