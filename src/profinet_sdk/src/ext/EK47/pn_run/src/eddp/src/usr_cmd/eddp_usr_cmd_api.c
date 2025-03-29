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
/*  F i l e               &F: eddp_usr_cmd_api.c                        :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP user command API                            */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  27.04.12    VS    initial version                                        */
#endif
/*****************************************************************************/


/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/

#define LTRC_ACT_MODUL_ID   140
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_USR_CMD_API */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"           /* eddp header files */
#include "pnip_xrsl.h"          /* PNIP register defines */
#include "eddp_trc.h"           /* EDDP Trace */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)


#ifdef EDDP_CFG_USR_CMD_API_INCLUDE





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_UserCmdInit                            +*/
/*+                                                                         +*/
/*+  Input/Output          :    LSA_UINT16          EDDPLocation            +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  EDDPLocation   : Location of EDDP                                 (in) +*/
/*+                     - EDDP_LOCATION_LOCAL                               +*/
/*+                     - EDDP_LOCATION_EXT_HOST                            +*/
/*+  pSharedMem     : Shared Memory for SyncIndication (optional)      (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function inits the user command interface            +*/
/*+               on start up.                                              +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDD_UPPER_IN_FCT_ATTR eddp_UserCmdInit(
	LSA_UINT16              EDDPLocation,
    EDD_COMMON_MEM_PTR_TYPE pSharedMem)
{
    EDD_RSP             Response;

    EDDP_USER_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] --> eddp_UserCmdInit()");

    EDDP_USER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] eddp_UserCmdInit(EDDPLocation=0x%X, pSharedMem=0x%08X)", EDDPLocation, pSharedMem);

    Response = EDDP_UserCmdInternInitHandleMngm (EDDPLocation, pSharedMem);

    EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] <-- eddp_UserCmdInit(): Response=0x%X", Response);

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_UserCmdInitHandle                      +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDD_COMMON_MEM_PTR_TYPE         pPnipBase   +*/
/*+                             EDDP_USER_CMD_HANDLE_TYPE      *pHandle     +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pPnipBase  : pointer to internal memory of the PN-IP              (in) +*/
/*+               from HOST view                                            +*/
/*+  pHandle    : pointer to returned handle                       (in/out) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function inits the user command interface for one    +*/
/*+               EDDP-PNIP-device and returns the new handle to user.      +*/
/*+                                                                         +*/
/*+               Important:                                                +*/
/*+               This function only may be called after initialization of  +*/
/*+               PN-IP! So only after start up of EDDP (after DeviceSetup)!+*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDD_UPPER_IN_FCT_ATTR eddp_UserCmdInitHandle(
	EDD_COMMON_MEM_PTR_TYPE         pPnipBase,
	EDDP_USER_CMD_HANDLE_TYPE      *pHandle)
{
    EDD_RSP         Response = EDD_STS_OK;

    EDDP_USER_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] --> eddp_UserCmdInitHandle()");

    EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] eddp_UserCmdInitHandle(pPnipBase=0x%X)", pPnipBase);

    /* -------------------------------- */
    /* checks                           */
    /* -------------------------------- */
    if (LSA_HOST_PTR_ARE_EQUAL(pPnipBase, LSA_NULL))
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] eddp_UserCmdInitHandle(): ERROR -> pPnipBase=NULL! Response=0x%X", 
            Response);
    }
    else
    {
        if (LSA_HOST_PTR_ARE_EQUAL(pHandle, LSA_NULL))
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] eddp_UserCmdInitHandle(): ERROR -> pHandle=NULL! Response=0x%X", 
                Response);
        }
    }

    /* -------------------------------- */
    /* execute the CmdInitHandle        */
    /* -------------------------------- */
    if (Response == EDD_STS_OK)
    {
        Response = EDDP_UserCmdInternGetNewHandle (pPnipBase, pHandle);
    }

    EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] <-- eddp_UserCmdInitHandle(): Response=0x%X", Response);

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_UserCmdRequest                         +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_USER_CMD_HANDLE_TYPE       Handle      +*/
/*+                             EDDP_USER_CMD_PARAM_PTR_TYPE    pParam      +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  Handle     : Handle from eddp_UserCmdInitHandle()                 (in) +*/
/*+  pParam     : pointer to command parameter block                   (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function invokes a user command for PN-IP.           +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDD_UPPER_IN_FCT_ATTR eddp_UserCmdRequest(
	EDDP_USER_CMD_HANDLE_TYPE       Handle,
 	EDDP_USER_CMD_PARAM_PTR_TYPE    pParam)
{
    EDD_RSP         Response = EDD_STS_OK;

    EDDP_USER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:%2X] --> eddp_UserCmdRequest(pParam=0x%X)", Handle, pParam);

    EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:%2X] eddp_UserCmdRequest()", Handle);

    /* -------------------------------- */
    /* checks                           */
    /* -------------------------------- */
    if (LSA_HOST_PTR_ARE_EQUAL(pParam, LSA_NULL))
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] eddp_UserCmdRequest(): ERROR -> pParam=NULL! Response=0x%X", 
            Response);
    }
    else
    {
        if (Handle >= EDDP_CFG_MAX_USER_CMD_HANDLES)
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_USER_TRACE_03(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] eddp_UserCmdRequest(): ERROR -> given Handle(0x%X) > HandleMax(0x%X)! Response=0x%X", 
                Handle, (EDDP_CFG_MAX_USER_CMD_HANDLES-1), Response);
        }
    }

    /* -------------------------------- */
    /* execute the CmdRequest           */
    /* -------------------------------- */
    if (Response == EDD_STS_OK)
    {
        Response = EDDP_UserCmdInternRequest (Handle, pParam);
    }

    EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] <-- eddp_UserCmdRequest(): Response=0x%X", Response);

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_UserCmdReadyTest                       +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_USER_CMD_HANDLE_TYPE       Handle      +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  Handle     : Handle from eddp_UserCmdInitHandle()                 (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: With this function the command processing checks          +*/
/*+               whether a user command is finished.                       +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDD_UPPER_IN_FCT_ATTR eddp_UserCmdReadyTest(
	EDDP_USER_CMD_HANDLE_TYPE       Handle)
{
    EDD_RSP         Response = EDD_STS_OK;

    EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:%2X] --> eddp_UserCmdReadyTest()", Handle);

    EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:%2X] eddp_UserCmdReadyTest()", Handle);

    /* -------------------------------- */
    /* checks                           */
    /* -------------------------------- */
    if (Handle >= EDDP_CFG_MAX_USER_CMD_HANDLES)
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_USER_TRACE_03(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] eddp_UserCmdReadyTest(): ERROR -> given Handle(0x%X) > HandleMax(0x%X)! Response=0x%X", 
            Handle, (EDDP_CFG_MAX_USER_CMD_HANDLES-1), Response);
    }

    /* -------------------------------- */
    /* execute the CmdReadyTest         */
    /* -------------------------------- */
    if (Response == EDD_STS_OK)
    {
        Response = EDDP_UserCmdInternReadyTest (Handle);
    }

    EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] <-- eddp_UserCmdReadyTest(): Response=0x%X", Response);

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_UserCmdFreeHandle                      +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_USER_CMD_HANDLE_TYPE       Handle      +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  Handle     : Handle from eddp_UserCmdInitHandle()                 (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function deinits the user command interface for      +*/
/*+               given hanlde.                                             +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDD_UPPER_IN_FCT_ATTR eddp_UserCmdFreeHandle(
	EDDP_USER_CMD_HANDLE_TYPE       Handle)
{
    EDD_RSP         Response = EDD_STS_OK;

    EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:%2X] --> eddp_UserCmdFreeHandle()", Handle);

    EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:%2X] eddp_UserCmdFreeHandle()", Handle);

    /* -------------------------------- */
    /* checks                           */
    /* -------------------------------- */
    if (Handle >= EDDP_CFG_MAX_USER_CMD_HANDLES)
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_USER_TRACE_03(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] eddp_UserCmdFreeHandle(): ERROR -> given Handle(0x%X) > HandleMax(0x%X)! Response=0x%X", 
            Handle, (EDDP_CFG_MAX_USER_CMD_HANDLES-1), Response);
    }

    /* -------------------------------- */
    /* execute the CmdFreeHandle        */
    /* -------------------------------- */
    if (Response == EDD_STS_OK)
    {
        Response = EDDP_UserCmdInternFreeHandle (Handle);
    }

    EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] <-- eddp_UserCmdFreeHandle(): Response=0x%X", Response);

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_UserCmdDeinit                          +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function deinits the user command interface          +*/
/*+               on shut down.                                             +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDD_UPPER_IN_FCT_ATTR eddp_UserCmdDeinit(LSA_VOID)
{
    EDD_RSP         Response = EDD_STS_OK;

    EDDP_USER_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] --> eddp_UserCmdDeinit()");

    EDDP_USER_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] eddp_UserCmdDeinit()");

    /* -------------------------------- */
    /* execute the CmdDeinit            */
    /* -------------------------------- */
    Response = EDDP_UserCmdInternDeinitHandleMngm();

    EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] <-- eddp_UserCmdDeinit(): Response=0x%X", Response);

    return (Response);
}

#endif  // EDDP_CFG_USR_CMD_API_INCLUDE

/*****************************************************************************/
/*  end of file eddp_usr_cmd_api.c                                           */
/*****************************************************************************/
