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
/*  F i l e               &F: eddp_usr_cmd_intern.c                     :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP user command intern                         */
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

#define LTRC_ACT_MODUL_ID   141
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_USR_CMD_INTERN */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"               /* eddp headerfiles */
#include "eddp_int.h"               /* internal header  */
#include "eddp_usr_cmd_intern.h"    /* internal header for user command interface */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)


#ifdef EDDP_CFG_USR_CMD_API_INCLUDE

// global pointer to main handle management of EDDP user command intern
EDDP_USER_CMD_INT_MNGM_PTR_TYPE     pEDDPUserCmdIntMngm;
// type definition for handle management
EDDP_LOCAL_MEM_ATTR     EDDP_USER_CMD_INT_MNGM_TYPE     g_EDDP_USER_CMD_INT_HANDLE_MNGM;





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_UserCmdInternInitHandleMngm            +*/
/*+                                                                         +*/
/*+  Input/Output          :    LSA_UINT16          EDDPLocation            +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  EDDPLocation   : Location of EDDP                                 (in) +*/
/*+                     - EDDP_LOCATION_LOCAL                               +*/
/*+                     - EDDP_LOCATION_EXT_HOST                            +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function inits the handle management                 +*/
/*+               for user command interface.                               +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_UserCmdInternInitHandleMngm(
    LSA_UINT16              EDDPLocation, 
    EDD_COMMON_MEM_PTR_TYPE pSharedMem)
{
    EDD_RSP         Response = EDD_STS_OK;
    LSA_UINT32      iHandle;

    EDDP_USER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_UserCmdInternInitHandleMngm(EDDPLocation=0x%X, pSharedMem=0x%08X)", EDDPLocation, pSharedMem);

    if ( !(LSA_HOST_PTR_ARE_EQUAL(pEDDPUserCmdIntMngm, LSA_NULL)) )
    {
        Response = EDD_STS_ERR_SEQUENCE;
        EDDP_USER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_UserCmdInternInitHandleMngm(): ERROR -> the pEDDPUserCmdIntMngm(0x%X) is already initialized! Response=0x%X", 
            pEDDPUserCmdIntMngm, Response);
    }
    else
    {
        // init global pointer of main handle management for EDDP user command intern
        pEDDPUserCmdIntMngm = &g_EDDP_USER_CMD_INT_HANDLE_MNGM;

        if ((EDDPLocation != EDDP_LOCATION_EXT_HOST) && (EDDPLocation != EDDP_LOCATION_LOCAL))
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_USER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_UserCmdInternInitHandleMngm(): ERROR -> given EDDPLocation(0x%X) is invalid! Response=0x%X", 
                EDDPLocation, Response);
        }
        else
        {
            // check "pEDDPUserCmdIntMngm"
            if (LSA_HOST_PTR_ARE_EQUAL(pEDDPUserCmdIntMngm, LSA_NULL))
            {
  	            Response = EDD_STS_ERR_RESOURCE;
                EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_UserCmdInternInitHandleMngm(): ERROR -> pEDDPUserCmdIntMngm=NULL! Response=0x%X", 
                    Response);
            }
            else
            {
                // init main handle management for EDDP user command intern
                EDDP_MEMSET_COMMON((EDD_COMMON_MEM_PTR_TYPE)pEDDPUserCmdIntMngm, 0, sizeof(EDDP_USER_CMD_INT_MNGM_TYPE));

                pEDDPUserCmdIntMngm->EDDPLocation = EDDPLocation;
                pEDDPUserCmdIntMngm->pSharedMem   = (EDDP_USERCMDSHAREDMEM_TYPE*)pSharedMem;

                for ( iHandle=0; iHandle < EDDP_CFG_MAX_USER_CMD_HANDLES; iHandle++ )
                {
                    pEDDPUserCmdIntMngm->HandleMngm[iHandle].InUseHandle    = LSA_FALSE;
                    pEDDPUserCmdIntMngm->HandleMngm[iHandle].HandleId       = (EDDP_USER_CMD_HANDLE_TYPE) iHandle;
	            }
            }
        }
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_UserCmdInternGetNewHandle              +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDD_COMMON_MEM_PTR_TYPE         pPnipBase   +*/
/*+                             EDDP_USER_CMD_HANDLE_TYPE      *pHandle     +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  pPnipBase  : pointer to internal memory of the PN-IP from HOST view    +*/
/*+  pHandle    : pointer to returned handle                                +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function gets new handle index.                      +*/
/*+                                                                         +*/
/*+               Return:                                                   +*/
/*+                 - valid Handle   : if one new handle is present         +*/
/*+                 - invalid Handle : if error or HandleMax is reached     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_UserCmdInternGetNewHandle(
    EDD_COMMON_MEM_PTR_TYPE         pPnipBase,
    EDDP_USER_CMD_HANDLE_TYPE      *pHandle)
{
    EDD_RSP                         Response;
    LSA_UINT32                      iHandle;
    LSA_UINT32                      CheckBytes;
    EDDP_USER_CMD_HANDLE_TYPE       Handle;

    Response    = EDD_STS_OK;
    iHandle     = 0;
    Handle      = EDDP_USER_CMD_HANDLE_INVALID;

    // check "pEDDPUserCmdIntMngm"
    if (LSA_HOST_PTR_ARE_EQUAL(pEDDPUserCmdIntMngm, LSA_NULL))
    {
  	    Response = EDD_STS_ERR_SEQUENCE;
        EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_UserCmdInternGetNewHandle(): ERROR -> pEDDPUserCmdIntMngm=NULL! Response=0x%X", 
            Response);
    }
    else
    {
        if (pEDDPUserCmdIntMngm->HandleUsedCnt >= EDDP_CFG_MAX_USER_CMD_HANDLES)
        {
  	        Response = EDD_STS_ERR_RESOURCE;
            EDDP_USER_TRACE_03(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_UserCmdInternGetNewHandle(): ERROR -> No more free Handles! HandleUsedCnt=%d, HandleUsedCntMax=%d, Response=0x%X", 
                pEDDPUserCmdIntMngm->HandleUsedCnt, EDDP_CFG_MAX_USER_CMD_HANDLES, Response);
        }
        else
        {
            // search handle that is not yet used
            while ((iHandle < EDDP_CFG_MAX_USER_CMD_HANDLES) && (pEDDPUserCmdIntMngm->HandleMngm[iHandle].InUseHandle))
            {
                iHandle++;
            }

            if (iHandle >= EDDP_CFG_MAX_USER_CMD_HANDLES)
	        {
  	            Response = EDD_STS_ERR_RESOURCE;
                EDDP_USER_TRACE_03(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_UserCmdInternGetNewHandle(): ERROR -> No more free Handles! HandleUsedCnt=%d, HandleUsedCntMax=%d, Response=0x%X", 
                    pEDDPUserCmdIntMngm->HandleUsedCnt, EDDP_CFG_MAX_USER_CMD_HANDLES, Response);
	        }
            else
            {
                // init pointer to UserCmd-Info-Block in CMD-Ram
                pEDDPUserCmdIntMngm->HandleMngm[iHandle].pInfoBlockHostAddr = (EDD_COMMON_MEM_PTR_TYPE) ((LSA_UINT32)pPnipBase + EDDP_USER_CMD_RAM_CMD_INFO_BLOCK_PNIP_ADDR);

                // check it is within EDDP-DeviceSetup...DeviceShutdown
                // read CheckBytes from UserCmd-Info-Block in CMD-Ram
                CheckBytes = EDDP_USR_CMD_MEM32_READ(&((EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE)pEDDPUserCmdIntMngm->HandleMngm[iHandle].pInfoBlockHostAddr)->CheckBytes);

                if (CheckBytes != EDDP_CMD_USER_CMD_INFO_BLOCK_CHECK_BYTES)
                {
  	                Response = EDD_STS_ERR_SEQUENCE;
                    EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_UserCmdInternGetNewHandle(): ERROR -> only allowed after EDDP-DeviceSetup and before DeviceShutdown! Response=0x%X", 
                        Response);
                }

                // if no error this handle is used
                if (Response == EDD_STS_OK)
                {
                    pEDDPUserCmdIntMngm->HandleUsedCnt++;

                    pEDDPUserCmdIntMngm->HandleMngm[iHandle].InUseHandle                = LSA_TRUE;
                    pEDDPUserCmdIntMngm->HandleMngm[iHandle].CurrentCmdParam.CmdOpcode  = EDDP_USER_CMD_CURRENT_CMD_OPCODE_INVALID;
                    pEDDPUserCmdIntMngm->HandleMngm[iHandle].pPnipBase                  = pPnipBase;

                    // return new handle
                    Handle = pEDDPUserCmdIntMngm->HandleMngm[iHandle].HandleId;
                }
                else
                {
                    pEDDPUserCmdIntMngm->HandleMngm[iHandle].pInfoBlockHostAddr = LSA_NULL;
                }
            }
        }
    }

    *pHandle = Handle;

    EDDP_USER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_UserCmdInternGetNewHandle(pPnipBase=0x%X): new Handle=0x%X", 
        pPnipBase, Handle);

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_UserCmdInternRequest                   +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_USER_CMD_HANDLE_TYPE       Handle      +*/
/*+                             EDDP_USER_CMD_PARAM_PTR_TYPE    pParam      +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Handle     : Handle from eddp_UserCmdInitHandle()                      +*/
/*+  pParam     : pointer to command parameter block                        +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function invokes a user command for PN-IP.           +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_UserCmdInternRequest(
	EDDP_USER_CMD_HANDLE_TYPE       Handle,
 	EDDP_USER_CMD_PARAM_PTR_TYPE    pParam)
{
    EDD_RSP                                     Response;
    LSA_UINT8                                   ApplTimerMaskParam, ApplTimerMaskCmd;
    LSA_UINT32                                  ApplLengthNs;
    LSA_UINT32                                  TimeoutTicks, RctDelayStart, RctDelayNew;
    LSA_UINT32                                  RequestEntry;
    LSA_UINT32                                  RegApplStatus, RegQueueErrStatus, RegQueueErrEvent;
    EDD_COMMON_MEM_PTR_TYPE                     RequestEntryHostAddr;
    EDDP_USER_CMD_HANDLE_MNGM_SINGLE_PTR_TYPE   pHandleMngm;

    EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_UserCmdInternRequest()", Handle);

    EDDP_USR_CMD_ASSERT_FALSE(Handle >= EDDP_CFG_MAX_USER_CMD_HANDLES);
    EDDP_USR_CMD_ASSERT_FALSE(LSA_HOST_PTR_ARE_EQUAL(pParam, LSA_NULL));

    Response            = EDD_STS_OK;
    ApplTimerMaskParam  = 0;
    ApplTimerMaskCmd    = 0;
    ApplLengthNs        = 0;
    RegApplStatus       = 0;

    /* -------------------------------------------- */
    /* checks                                       */
    /* -------------------------------------------- */
    // check "pEDDPUserCmdIntMngm"
    if (LSA_HOST_PTR_ARE_EQUAL(pEDDPUserCmdIntMngm, LSA_NULL))
    {
  	    Response = EDD_STS_ERR_SEQUENCE;
        EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_UserCmdInternRequest(): ERROR -> pEDDPUserCmdIntMngm=NULL! Response=0x%X", 
            Response);
    }
    else
    {
        // check the given handle is used
        if (!(pEDDPUserCmdIntMngm->HandleMngm[Handle].InUseHandle))
        {
  	        Response = EDD_STS_ERR_SEQUENCE;
            EDDP_USER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_UserCmdInternRequest(): ERROR -> given Handle(0x%X) is not used! Response=0x%X", 
                Handle, Response);
        }

        // check it is within EDDP-DeviceSetup...DeviceShutdown
        if ( (Response == EDD_STS_OK) && (!(EDDP_UserCmdInternAreCheckBytesPresent(Handle))) )
        {
  	        Response = EDD_STS_ERR_SEQUENCE;
            EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_UserCmdInternRequest(): ERROR -> only allowed after EDDP-DeviceSetup and before DeviceShutdown! Response=0x%X", 
                Response);
        }

        // check no user command is running currently (not at TimeStampGet / TimeSyncIndGet)
        if ((Response == EDD_STS_OK) 
		&&  (pParam->CmdOpcode != EDDP_USER_CMD_GSYNC_TIME_STAMP_GET)
		&&  (pParam->CmdOpcode != EDDP_USER_CMD_GSYNC_TIME_SYNC_IND_GET)
		&&  (pEDDPUserCmdIntMngm->HandleMngm[Handle].CurrentCmdParam.CmdOpcode != EDDP_USER_CMD_CURRENT_CMD_OPCODE_INVALID))
        {
  	        Response = EDD_STS_ERR_SEQUENCE;
            EDDP_USER_TRACE_03(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_UserCmdInternRequest(): ERROR -> one command is running currently! CmdOpcode=0x%X, Response=0x%X", 
                Handle, pEDDPUserCmdIntMngm->HandleMngm[Handle].CurrentCmdParam.CmdOpcode, Response);
        }

        if ((Response == EDD_STS_OK) 
            &&  (EDDP_USER_CMD_ASYNC_PROCESSING != pParam->Async)
            &&  (EDDP_USER_CMD_SYNC_PROCESSING != pParam->Async) )
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_USER_TRACE_03(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_UserCmdInternRequest(): ERROR -> illegal value for Async(%d)!  (CmdOpcode=0x%X)", 
                Handle, pParam->Async, pEDDPUserCmdIntMngm->HandleMngm[Handle].CurrentCmdParam.CmdOpcode);
        }

        if (Response == EDD_STS_OK)
        {
            // read register ApplStatus
            RegApplStatus = EDDP_USR_REG32_READ(pEDDPUserCmdIntMngm->HandleMngm[Handle].pPnipBase, PNIP_REG_APPLSTATUS);

            // check CmdOpcode and init ApplTimerMask
            switch (pParam->CmdOpcode)
            {
                /* ---------------------------- */
                /* Update_APPLCYCLE             */
                /* ---------------------------- */
                case EDDP_USER_CMD_GSYNC_UPDATE_APPCYCL:
                {
                    // init ApplTimerMaskParam(1/2/3)
                    if (pParam->Param.UpdateAppCycl.ApplicationTimer1)
                        ApplTimerMaskParam |= PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_SELECT_APPL_TIMER_1;
                    if (pParam->Param.UpdateAppCycl.ApplicationTimer2)
                        ApplTimerMaskParam |= PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_SELECT_APPL_TIMER_2;
                    if (pParam->Param.UpdateAppCycl.ApplicationTimer3)
                        ApplTimerMaskParam |= PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_SELECT_APPL_TIMER_3;

                    // init "ApplTimerMaskCmd" and "ApplLengthNs"
                    ApplTimerMaskCmd    = ApplTimerMaskParam;
                    ApplLengthNs        = pParam->Param.UpdateAppCycl.NewApplicationLength;
                }
                break;

                /* ---------------------------- */
                /* Connect_FREE                 */
                /* ---------------------------- */
                case EDDP_USER_CMD_GSYNC_CONNECT_FREE:
                {
                    // init ApplTimerMaskParam(1) and ApplTimerMaskCmd(1)
                    if (pParam->Param.ConnectFree.ApplicationTimer1)
                    {
                        ApplTimerMaskParam |= PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_SELECT_APPL_TIMER_1;

                        if ( ((RegApplStatus & PNIP_REG_APPLSTATUS__MSK_APPLICATIONSTATUS_1) >> PNIP_REG_APPLSTATUS_SHFT_APPLICATIONSTATUS_1) != EDDP_USER_CMD_REG_APPL_STATUS_VALUE_FREE )
                        {
                            ApplTimerMaskCmd |= PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_SELECT_APPL_TIMER_1;
                        }
                    }

                    // init ApplTimerMaskParam(2) and ApplTimerMaskCmd(2)
                    if (pParam->Param.ConnectFree.ApplicationTimer2)
                    {
                        ApplTimerMaskParam |= PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_SELECT_APPL_TIMER_2;

                        if ( ((RegApplStatus & PNIP_REG_APPLSTATUS__MSK_APPLICATIONSTATUS_2) >> PNIP_REG_APPLSTATUS_SHFT_APPLICATIONSTATUS_2) != EDDP_USER_CMD_REG_APPL_STATUS_VALUE_FREE )
                        {
                            ApplTimerMaskCmd |= PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_SELECT_APPL_TIMER_2;
                        }
                    }

                    // init ApplTimerMaskParam(3) and ApplTimerMaskCmd(3)
                    if (pParam->Param.ConnectFree.ApplicationTimer3)
                    {
                        ApplTimerMaskParam |= PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_SELECT_APPL_TIMER_3;

                        if ( ((RegApplStatus & PNIP_REG_APPLSTATUS__MSK_APPLICATIONSTATUS_3) >> PNIP_REG_APPLSTATUS_SHFT_APPLICATIONSTATUS_3) != EDDP_USER_CMD_REG_APPL_STATUS_VALUE_FREE )
                        {
                            ApplTimerMaskCmd |= PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_SELECT_APPL_TIMER_3;
                        }
                    }
                }
                break;

                /* ---------------------------- */
                /* Connect_SYNC_A               */
                /* ---------------------------- */
                case EDDP_USER_CMD_GSYNC_CONNECT_SYNC_A:
                {
                    // init ApplTimerMaskParam(1) and ApplTimerMaskCmd(1)
                    if (pParam->Param.ConnectSyncA.ApplicationTimer1)
                    {
                        ApplTimerMaskParam |= PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_SELECT_APPL_TIMER_1;

                        if ( ((RegApplStatus & PNIP_REG_APPLSTATUS__MSK_APPLICATIONSTATUS_1) >> PNIP_REG_APPLSTATUS_SHFT_APPLICATIONSTATUS_1) != EDDP_USER_CMD_REG_APPL_STATUS_VALUE_PTCP_CLOCK_IFA )
                        {
                            ApplTimerMaskCmd |= PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_SELECT_APPL_TIMER_1;
                        }
                    }

                    // init ApplTimerMaskParam(2) and ApplTimerMaskCmd(2)
                    if (pParam->Param.ConnectSyncA.ApplicationTimer2)
                    {
                        ApplTimerMaskParam |= PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_SELECT_APPL_TIMER_2;

                        if ( ((RegApplStatus & PNIP_REG_APPLSTATUS__MSK_APPLICATIONSTATUS_2) >> PNIP_REG_APPLSTATUS_SHFT_APPLICATIONSTATUS_2) != EDDP_USER_CMD_REG_APPL_STATUS_VALUE_PTCP_CLOCK_IFA )
                        {
                            ApplTimerMaskCmd |= PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_SELECT_APPL_TIMER_2;
                        }
                    }

                    // init ApplTimerMaskParam(3) and ApplTimerMaskCmd(3)
                    if (pParam->Param.ConnectSyncA.ApplicationTimer3)
                    {
                        ApplTimerMaskParam |= PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_SELECT_APPL_TIMER_3;

                        if ( ((RegApplStatus & PNIP_REG_APPLSTATUS__MSK_APPLICATIONSTATUS_3) >> PNIP_REG_APPLSTATUS_SHFT_APPLICATIONSTATUS_3) != EDDP_USER_CMD_REG_APPL_STATUS_VALUE_PTCP_CLOCK_IFA )
                        {
                            ApplTimerMaskCmd |= PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_SELECT_APPL_TIMER_3;
                        }
                    }
                }
                break;

                /* ---------------------------- */
                /* TimeStampGet                 */
                /* ---------------------------- */
                case EDDP_USER_CMD_GSYNC_TIME_STAMP_GET:
                {
					if (EDDP_USER_CMD_ASYNC_PROCESSING == pParam->Async)
					{
						/* ------------------------------------ */
						/* no asynchronous command processing   */
						/* ------------------------------------ */
						EDDP_USER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_UserCmdInternRequest(): ERROR -> CmdOpcode(0x%X) is Async! Response=0x%X", 
							pParam->CmdOpcode, Response);
						Response = EDD_STS_ERR_PARAM;
					}
					else
					{
						/* ------------------------------------ */
						/* read time stamp and states           */
						/* ------------------------------------ */
						Response = EDDP_UserCmdInternTimeStampGet(Handle, pParam);
					}
                }
                break;

                /* ---------------------------- */
                /* TimeSyncIndGet               */
                /* ---------------------------- */
                case EDDP_USER_CMD_GSYNC_TIME_SYNC_IND_GET:
                {
					if (EDDP_USER_CMD_ASYNC_PROCESSING == pParam->Async)
					{
						/* ------------------------------------ */
						/* no asynchronous command processing   */
						/* ------------------------------------ */
						EDDP_USER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_UserCmdInternRequest(): ERROR -> CmdOpcode(0x%X) is Async! Response=0x%X", 
							pParam->CmdOpcode, Response);
						Response = EDD_STS_ERR_PARAM;
					}
					else
					{
						/* ------------------------------------ */
						/* read Time sync indication            */
						/* ------------------------------------ */
						Response = EDDP_UserCmdInternTimeSyncIndGet(Handle, pParam);
					}
                }
                break;

				/* ---------------------------- */
                /* unknown CmdOpcode            */
                /* ---------------------------- */
                default:
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_USER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_UserCmdInternRequest(): ERROR -> CmdOpcode(0x%X) is unknown! Response=0x%X", 
                        pParam->CmdOpcode, Response);
                }
                break;
            }
        }
    }

    /* -------------------------------------------- */
    /* invoke a user command to PN-IP               */
    /* -------------------------------------------- */
    if (Response == EDD_STS_OK)
    {
        pHandleMngm = &pEDDPUserCmdIntMngm->HandleMngm[Handle];

        EDDP_USER_TRACE_07(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:%2X] EDDP_UserCmdInternRequest(CmdOpcode=0x%X, Async=0x%X, ApplTimerMaskParam=0x%X, ApplLengthNs=%d): ApplTimerMaskCmd=0x%X, RegApplStatus=0x%X", 
            Handle, pParam->CmdOpcode, pParam->Async, ApplTimerMaskParam, ApplLengthNs, ApplTimerMaskCmd, RegApplStatus);

        if (ApplTimerMaskCmd != 0)
        {
            pHandleMngm->CurrentCmdParam.CmdOpcode      = pParam->CmdOpcode;
            pHandleMngm->CurrentCmdParam.ApplTimerMask  = ApplTimerMaskCmd;

            // fill command block and parameters in PN-IP
            EDDP_UserCmdInternFillCblAndParam (Handle, pParam);
            // and trigger queue processing
            EDDP_USR_REG32_WRITE(pHandleMngm->pPnipBase, PNIP_REG_TRIGGERREQQUEUE, (0x1 << EDDP_CMD_INDEX_USER_SYNC));

            if (EDDP_USER_CMD_ASYNC_PROCESSING == pParam->Async)
            {
                /* ------------------------------------ */
                /* asynchronous command processing      */
                /* ------------------------------------ */
                // set Response and wait for calling of eddp_UserCmdReadyTest()
                Response = EDD_STS_OK_PENDING;
            }
            else
            {
                /* ------------------------------------ */
                /* synchronous command processing       */
                /* ------------------------------------ */
                // check the RequestEntry is confirmed by HW
                EDDP_USR_CMD_ASSERT_FALSE(LSA_HOST_PTR_ARE_EQUAL(pHandleMngm->pInfoBlockHostAddr, LSA_NULL));
                RequestEntryHostAddr    = (EDD_COMMON_MEM_PTR_TYPE) (((EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE)pHandleMngm->pInfoBlockHostAddr)->RequestEntryCmdRamAddr);
                RequestEntryHostAddr    = (EDD_COMMON_MEM_PTR_TYPE) EDDP_USR_CMD_P2H_ADDR(RequestEntryHostAddr, pHandleMngm->pPnipBase);
                TimeoutTicks            = EDDP_CFG_USR_CMD_TIMEOUT_TICKS_SYNCHRONOUS_1US;
                RctDelayStart           = EDDP_USR_REG32_READ(pHandleMngm->pPnipBase, PNIP_REG_RCT_DELAY);

                // Timeout check of this synchronous command
                do
                {
                    // wait until command is done or timeout
                    RequestEntry = EDDP_USR_CMD_MEM32_READ(RequestEntryHostAddr);
                    if ((RequestEntry & EDDP_CMD_REQUEST_STATUS_MASK) == EDDP_CMD_REQUEST_CONFIRMED)
                    {
                        // command is finished
                        break;
                    }
                    EDDP_UserCmdInternWaitNsTicks (Handle, EDDP_HAL_1US_TO);
                    TimeoutTicks--;
                } while (TimeoutTicks > 0);

                RctDelayNew = EDDP_USR_REG32_READ(pHandleMngm->pPnipBase, PNIP_REG_RCT_DELAY);

                // has Timeout reached 0 ?
                if (TimeoutTicks == 0)
                {
                    Response            = EDD_STS_ERR_TIMEOUT;
                    RegQueueErrStatus   = EDDP_USR_REG32_READ(pHandleMngm->pPnipBase, PNIP_REG_QUEUEERRSTATUS);
                    RegQueueErrEvent    = EDDP_USR_REG32_READ(pHandleMngm->pPnipBase, PNIP_REG_QUEUEERREVENT);

                    EDDP_USER_TRACE_06(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_UserCmdInternRequest(): ERROR -> Timeout after %d ns: RequestEntry=0x%X, RegQueueErrStatus=0x%X, RegQueueErrEvent=0x%X, Response=0x%X", 
                        Handle, (RctDelayNew - RctDelayStart), RequestEntry, RegQueueErrStatus, RegQueueErrEvent, Response);

                    // delete flags
                    pHandleMngm->CurrentCmdParam.CmdOpcode      = EDDP_USER_CMD_CURRENT_CMD_OPCODE_INVALID;
                    pHandleMngm->CurrentCmdParam.ApplTimerMask  = 0;
                }
                else
                {
                    EDDP_USER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:%2X] EDDP_UserCmdInternRequest(): synchronous command has finished after %d ns", 
                        Handle, (RctDelayNew - RctDelayStart));

                    // finish this command
                    Response = EDDP_UserCmdInternReadyTest (Handle);
                    EDDP_USR_CMD_ASSERT_FALSE((Response == EDD_STS_OK_PENDING) || (Response == EDD_STS_ERR_SEQUENCE) || (Response == EDD_STS_OK_NO_DATA));
                }
            }
        }
    }

    EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_UserCmdInternRequest(): Response=0x%X", Response);

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_UserCmdInternReadyTest                 +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_USER_CMD_HANDLE_TYPE       Handle      +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Handle     : Handle from eddp_UserCmdInitHandle()                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: With this function the command processing checks          +*/
/*+               whether a user command is finished.                       +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_UserCmdInternReadyTest(
	EDDP_USER_CMD_HANDLE_TYPE       Handle)
{
    EDD_RSP                                     Response;
    LSA_UINT32                                  RequestEntry;
    LSA_UINT32                                  RegConfirmingQueueAll, RegConfirmingQueueUser;
    EDD_COMMON_MEM_PTR_TYPE                     CurrentConfPtrHostAddr;
    EDDP_USER_CMD_HANDLE_MNGM_SINGLE_PTR_TYPE   pHandleMngm;

    EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_UserCmdInternReadyTest()", Handle);

    EDDP_USR_CMD_ASSERT_FALSE(Handle >= EDDP_CFG_MAX_USER_CMD_HANDLES);

    Response = EDD_STS_OK;

    /* -------------------------------------------- */
    /* checks                                       */
    /* -------------------------------------------- */
    // check "pEDDPUserCmdIntMngm"
    if (LSA_HOST_PTR_ARE_EQUAL(pEDDPUserCmdIntMngm, LSA_NULL))
    {
  	    Response = EDD_STS_ERR_SEQUENCE;
        EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_UserCmdInternReadyTest(): ERROR -> pEDDPUserCmdIntMngm=NULL! Response=0x%X", 
            Response);
    }
    else
    {
        // check the given handle is used
        if (!(pEDDPUserCmdIntMngm->HandleMngm[Handle].InUseHandle))
        {
  	        Response = EDD_STS_ERR_SEQUENCE;
            EDDP_USER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_UserCmdInternReadyTest(): ERROR -> given Handle(0x%X) is not used! Response=0x%X", 
                Handle, Response);
        }
        else
        {
            // check it is within EDDP-DeviceSetup...DeviceShutdown
            if (!(EDDP_UserCmdInternAreCheckBytesPresent(Handle)))
            {
  	            Response = EDD_STS_ERR_SEQUENCE;
                EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_UserCmdInternReadyTest(): ERROR -> only allowed after EDDP-DeviceSetup and before DeviceShutdown! Response=0x%X", 
                    Response);
            }

            // check one user command is running currently (check it always as last because it is no error)
            if ((Response == EDD_STS_OK) && (pEDDPUserCmdIntMngm->HandleMngm[Handle].CurrentCmdParam.CmdOpcode == EDDP_USER_CMD_CURRENT_CMD_OPCODE_INVALID))
            {
                // this is no error
  	            Response = EDD_STS_OK_NO_DATA;
                EDDP_USER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_UserCmdInternReadyTest(): no asynchronous command is running currently! Response=0x%X", 
                    Handle, Response);
            }
        }
    }

    /* -------------------------------------------- */
    /* check the user command is finished           */
    /* -------------------------------------------- */
    if (Response == EDD_STS_OK)
    {
        pHandleMngm = &pEDDPUserCmdIntMngm->HandleMngm[Handle];

        EDDP_USR_CMD_ASSERT_FALSE(LSA_HOST_PTR_ARE_EQUAL(pHandleMngm->pInfoBlockHostAddr, LSA_NULL));

        // read the register ConfirmingQueue (all queues)
        RegConfirmingQueueAll   = EDDP_USR_REG32_READ(pHandleMngm->pPnipBase, PNIP_REG_CONFIRMINGQUEUE);
        // read only bits from ConfirmingQueue which belong to USER queue
        RegConfirmingQueueUser  = (EDDP_USR_CMD_CONFIRMINGQUEUE_USER_SYNC_MASK & RegConfirmingQueueAll);

        // read value from confirmation pointer (it is RequestEntry)
        CurrentConfPtrHostAddr  = (EDD_COMMON_MEM_PTR_TYPE) (((EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE)pHandleMngm->pInfoBlockHostAddr)->CurrentConfPtrCmdRamAddr);
        CurrentConfPtrHostAddr  = (EDD_COMMON_MEM_PTR_TYPE) EDDP_USR_CMD_P2H_ADDR(CurrentConfPtrHostAddr, pHandleMngm->pPnipBase);
        RequestEntry            = EDDP_USR_CMD_MEM32_READ(CurrentConfPtrHostAddr);

        EDDP_USER_TRACE_05(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:%2X] EDDP_UserCmdInternReadyTest(): CmdOpcode=0x%X, RegConfirmingQueueAll=0x%X, RegConfirmingQueueUser=0x%X, RequestEntry=0x%X", 
            Handle, pHandleMngm->CurrentCmdParam.CmdOpcode, RegConfirmingQueueAll, RegConfirmingQueueUser, RequestEntry);

        // check the RequestEntry is already confirmed by HW
        if (   (RegConfirmingQueueUser != 0)
            && ((RequestEntry & EDDP_CMD_REQUEST_STATUS_MASK) == EDDP_CMD_REQUEST_CONFIRMED)
           )
        {
            /* -------------------------------------------- */
            /* the command is already confirmed by HW       */
            /* -------------------------------------------- */
            // reset bits in ConfirmingQueue register
            EDDP_USR_REG32_WRITE(pHandleMngm->pPnipBase, PNIP_REG_CONFIRMINGQUEUE, RegConfirmingQueueUser);
            // read return value
            Response = EDDP_UserCmdInternReadCommandReturnValue (Handle);

            // set this RequestEntry to state "free" (delete it)
            EDDP_USR_CMD_MEM32_WRITE(CurrentConfPtrHostAddr, EDDP_CMD_REQUEST_FREE);

            // delete flags (this handle is free for new command from now)
            pHandleMngm->CurrentCmdParam.CmdOpcode      = EDDP_USER_CMD_CURRENT_CMD_OPCODE_INVALID;
            pHandleMngm->CurrentCmdParam.ApplTimerMask  = 0;
        }
        else
        {
            /* -------------------------------------------- */
            /* the command is still in processing           */
            /* -------------------------------------------- */
            Response = EDD_STS_OK_PENDING;
        }
    }

    EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_UserCmdInternReadyTest(): Response=0x%X", Response);

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_UserCmdInternFillCblAndParam           +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_USER_CMD_HANDLE_TYPE       Handle      +*/
/*+                             EDDP_USER_CMD_PARAM_PTR_TYPE    pParam      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Handle         : Handle from eddp_UserCmdInitHandle()                  +*/
/*+  pParam         : pointer to command parameter block                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function fills the command block and                 +*/
/*+               corresponding parameter for the current command.          +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_UserCmdInternFillCblAndParam(
	EDDP_USER_CMD_HANDLE_TYPE       Handle,
 	EDDP_USER_CMD_PARAM_PTR_TYPE    pParam)
{
    LSA_UINT32                                  ValueTemp32, UserId, RegIpVersion;
    LSA_UINT8                                   ApplTimerMask;
    LSA_UINT32                                  RequestEntry;
    LSA_UINT32                                  ParamStructCmdRamAddr, ParamTargetSyncRamAddr, CblOffsetCmdRam;
    LSA_UINT8                                   InteruptEvent;
    EDD_COMMON_MEM_PTR_TYPE                     RequestEntryHostAddr;
    EDD_COMMON_MEM_PTR_TYPE                     CblHostAddr;
    EDD_COMMON_MEM_PTR_TYPE                     ParamHostAddr;
    EDDP_USER_CMD_HANDLE_MNGM_SINGLE_PTR_TYPE   pHandleMngm;

    EDDP_USR_CMD_ASSERT_FALSE(Handle >= EDDP_CFG_MAX_USER_CMD_HANDLES);
    EDDP_USR_CMD_ASSERT_FALSE(LSA_HOST_PTR_ARE_EQUAL(pParam, LSA_NULL));
    EDDP_USR_CMD_ASSERT_FALSE(LSA_HOST_PTR_ARE_EQUAL(pEDDPUserCmdIntMngm, LSA_NULL));

    ValueTemp32     = 0;
    UserId          = EDDP_CMD_USER_ID_USER_SYNC;   // for user commands always this UserId (sync and async)
    RequestEntry    = 0;
    pHandleMngm     = &pEDDPUserCmdIntMngm->HandleMngm[Handle];
    ApplTimerMask   = pHandleMngm->CurrentCmdParam.ApplTimerMask;
    InteruptEvent   = 0;

    EDDP_USER_TRACE_03(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:%2X] EDDP_UserCmdInternFillCblAndParam(): ApplTimerMask=0x%X, UserId=%d", 
        Handle, ApplTimerMask, UserId);

    EDDP_USR_CMD_ASSERT_FALSE(LSA_HOST_PTR_ARE_EQUAL(pHandleMngm->pInfoBlockHostAddr, LSA_NULL));
    EDDP_USR_CMD_ASSERT_FALSE(ApplTimerMask == 0);

    // init local addresses
    RequestEntryHostAddr    = (EDD_COMMON_MEM_PTR_TYPE) (((EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE)pHandleMngm->pInfoBlockHostAddr)->RequestEntryCmdRamAddr);
    RequestEntryHostAddr    = (EDD_COMMON_MEM_PTR_TYPE) EDDP_USR_CMD_P2H_ADDR(RequestEntryHostAddr, pHandleMngm->pPnipBase);

    CblHostAddr             = (EDD_COMMON_MEM_PTR_TYPE) (((EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE)pHandleMngm->pInfoBlockHostAddr)->CblCmdRamAddr);
    CblHostAddr             = (EDD_COMMON_MEM_PTR_TYPE) EDDP_USR_CMD_P2H_ADDR(CblHostAddr, pHandleMngm->pPnipBase);
    
    /* PNIP_RAM_CMDCTRL_RAM_START was replaced by PNIP_R2P2_RAM_CMDCTRL_RAM_START, hardware dependency*/
    CblOffsetCmdRam         = ((LSA_UINT32)(((EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE)pHandleMngm->pInfoBlockHostAddr)->CblCmdRamAddr) - PNIP_R2P2_RAM_CMDCTRL_RAM_START);

    ParamHostAddr           = (EDD_COMMON_MEM_PTR_TYPE) (((EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE)pHandleMngm->pInfoBlockHostAddr)->ParamCmdRamAddr);
    ParamHostAddr           = (EDD_COMMON_MEM_PTR_TYPE) EDDP_USR_CMD_P2H_ADDR(ParamHostAddr, pHandleMngm->pPnipBase);

    /* -------------------------------------------------------------------------------- */
    /* parameters dependent of CmdOpcode                                                */
    /* -------------------------------------------------------------------------------- */
    switch (pParam->CmdOpcode)
    {
        case EDDP_USER_CMD_GSYNC_UPDATE_APPCYCL:
        {
            // write new ApplicationLength into CMD-Ram
            ValueTemp32 = pParam->Param.UpdateAppCycl.NewApplicationLength;
            if (ValueTemp32 < 8)
                ValueTemp32 = 0;    // because of 8ns granularity in register "ApplTimer_x"
            EDDP_USR_CMD_MEM32_WRITE(&(((EDDP_CMD_PARAM_GENERIC_PTR_TYPE)ParamHostAddr)->Generic[0].Low_32Bit), ValueTemp32);

            // configure command for copying of ParamStruct from CMD-Ram into Sync-Ram
            // ParamStruct in Sync-Ram has fix address for UserId   -> see "Param_User_ID_3" in PNIP_SWI_RAMSUB_SYNC_PARAM_STRUCT_TYPE
            // ParamStruct = SYNCParamStruct_Base + 8 * ParamStruct_offset(UserId)
            // base of Sync-Ram
            
            /* PNIP_RAM_SYNC_RAM_START was replaced by PNIP_R2P2_RAM_SYNC_RAM_START */
            /* all macros of group PNIP_xxxx_RAM_SYNC_RAM_START are checked in eddp_hal_pnip_val.h to have the same value*/
            ParamTargetSyncRamAddr = PNIP_R2P2_RAM_SYNC_RAM_START;
            ParamTargetSyncRamAddr += (8 * UserId);

            // address of ParamStruct in CMD-Ram
            EDDP_USR_CMD_ASSERT_FALSE(LSA_HOST_PTR_ARE_EQUAL(pHandleMngm->pInfoBlockHostAddr, LSA_NULL));
            /* PNIP_RAM_CMDCTRL_RAM_START was replaced by PNIP_R2P2_RAM_CMDCTRL_RAM_START */
            /* all macros of group PNIP_xxxx_RAM_CMDCTRL_RAM_START are checked in eddp_hal_pnip_val.h to have the same value*/
            ParamStructCmdRamAddr = ( (((EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE)pHandleMngm->pInfoBlockHostAddr)->ParamCmdRamAddr) - PNIP_R2P2_RAM_CMDCTRL_RAM_START );
            // Block-Transfer Commands (Low)
            EDDP_SET_CBL_LOW_32_BWRI(ValueTemp32, ParamStructCmdRamAddr, EDDP_CMD_SYNC_UPDATE_APPLCYCLE_PARAM_COUNT_32BIT);
            EDDP_USR_CMD_MEM32_WRITE(&(((EDDP_CMD_COMMAND_BLOCK_GENERIC_PTR_TYPE)CblHostAddr)->Generic[0].Low_32Bit), ValueTemp32);
            // Block-Transfer Commands (High)
            EDDP_SET_CBL_HIGH_32_BWRI(ValueTemp32, ParamTargetSyncRamAddr);
            EDDP_USR_CMD_MEM32_WRITE(&(((EDDP_CMD_COMMAND_BLOCK_GENERIC_PTR_TYPE)CblHostAddr)->Generic[0].High_32Bit), ValueTemp32);

            // configure "Update_APPLCYCLE" command for given "ApplicationTimer_x" in register "SYNCCommand_IF_Control"
            /* There is a macro that checks PNIP_R2_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE and PNIP_R3_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE has the same value. */
            /* Checked in eddp_hal_pnip_val.h. We use the R2 version. */
            ValueTemp32 = ((UserId << PNIP_REG_SYNCCOMMAND_IF_CONTROL_SHFT_USER_ID)                                                         |
                          (PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_CONFREQUEST_ENABLE << PNIP_REG_SYNCCOMMAND_IF_CONTROL_SHFT_CONFREQUEST)     |
                          ((ApplTimerMask) << PNIP_R2_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE)                                            |
                          (PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_F_CODE_UPDATE_APPLCYCLE << PNIP_REG_SYNCCOMMAND_IF_CONTROL_SHFT_F_CODE));
            EDDP_USR_CMD_MEM32_WRITE(&(((EDDP_CMD_COMMAND_BLOCK_GENERIC_PTR_TYPE)CblHostAddr)->Generic[1].Low_32Bit), ValueTemp32);

            // Single-Data Commands (High)
            EDDP_SET_CBL_HIGH_32_WORD_WCRC(ValueTemp32, PNIP_REG_SYNCCOMMAND_IF_CONTROL, LSA_TRUE);
            EDDP_USR_CMD_MEM32_WRITE(&(((EDDP_CMD_COMMAND_BLOCK_GENERIC_PTR_TYPE)CblHostAddr)->Generic[1].High_32Bit), ValueTemp32);
        }
        break;

        case EDDP_USER_CMD_GSYNC_CONNECT_FREE:
        {
            // configure "Connect_FREE" command for given "ApplicationTimer_x" in register "SYNCCommand_IF_Control"
            /* There is a macro that checks PNIP_R2_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE and PNIP_R3_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE has the same value. */
            /* Checked in eddp_hal_pnip_val.h. We use the R2 version. */
            ValueTemp32 = ((UserId << PNIP_REG_SYNCCOMMAND_IF_CONTROL_SHFT_USER_ID)                                                         |
                          (PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_CONFREQUEST_ENABLE << PNIP_REG_SYNCCOMMAND_IF_CONTROL_SHFT_CONFREQUEST)     |
                          ((ApplTimerMask) << PNIP_R2_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE)                                            |
                          (PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_F_CODE_CONNECT_FREE << PNIP_REG_SYNCCOMMAND_IF_CONTROL_SHFT_F_CODE));
            EDDP_USR_CMD_MEM32_WRITE(&(((EDDP_CMD_COMMAND_BLOCK_GENERIC_PTR_TYPE)CblHostAddr)->Generic[1].Low_32Bit), ValueTemp32);

            // Single-Data Commands (High)
            EDDP_SET_CBL_HIGH_32_WORD_WCRC(ValueTemp32, PNIP_REG_SYNCCOMMAND_IF_CONTROL, LSA_TRUE);
            EDDP_USR_CMD_MEM32_WRITE(&(((EDDP_CMD_COMMAND_BLOCK_GENERIC_PTR_TYPE)CblHostAddr)->Generic[1].High_32Bit), ValueTemp32);
        }
        break;

        case EDDP_USER_CMD_GSYNC_CONNECT_SYNC_A:
        {
            // configure "Connect_SYNC_A" command for given "ApplicationTimer_x" in register "SYNCCommand_IF_Control"
            /* There is a macro that checks PNIP_R2_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE and PNIP_R3_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE has the same value. */
            /* Checked in eddp_hal_pnip_val.h. We use the R2 version. */
            ValueTemp32 = ((UserId << PNIP_REG_SYNCCOMMAND_IF_CONTROL_SHFT_USER_ID)                                                         |
                          (PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_CONFREQUEST_ENABLE << PNIP_REG_SYNCCOMMAND_IF_CONTROL_SHFT_CONFREQUEST)     |
                          ((ApplTimerMask) << PNIP_R2_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE)                                            |
                          (PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_F_CODE_CONNECT_SYNC_A << PNIP_REG_SYNCCOMMAND_IF_CONTROL_SHFT_F_CODE));
            EDDP_USR_CMD_MEM32_WRITE(&(((EDDP_CMD_COMMAND_BLOCK_GENERIC_PTR_TYPE)CblHostAddr)->Generic[1].Low_32Bit), ValueTemp32);

            // Single-Data Commands (High)
            EDDP_SET_CBL_HIGH_32_WORD_WCRC(ValueTemp32, PNIP_REG_SYNCCOMMAND_IF_CONTROL, LSA_TRUE);
            EDDP_USR_CMD_MEM32_WRITE(&(((EDDP_CMD_COMMAND_BLOCK_GENERIC_PTR_TYPE)CblHostAddr)->Generic[1].High_32Bit), ValueTemp32);
        }
        break;

        default:
        {
            EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_UserCmdInternFillCblAndParam(): FATAL -> CmdOpcode(0x%X) is unknown!", 
                pParam->CmdOpcode);
            EDDP_USR_CMD_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }

    /* -------------------------------------------------------------------------------- */
    /* init the InteruptEvent bit (depend on PNIP revision)                             */
    /* -------------------------------------------------------------------------------- */
    RegIpVersion = EDDP_USR_REG32_READ(pHandleMngm->pPnipBase, PNIP_REG_IP_VERSION);
    // get version of PNIP (it is revision)
    /* There is a macro PNIP_R2_REG_IP_VERSION__MSK_VERSION and PNIP_R2_REG_IP_VERSION_SHFT_VERSION too which has the same value. */
    /* Here only _R1_ is used. */
	ValueTemp32 = EDDP_GET_BIT_VALUE(RegIpVersion, PNIP_R1_REG_IP_VERSION__MSK_VERSION, PNIP_R1_REG_IP_VERSION_SHFT_VERSION);

    switch (ValueTemp32)
    {
        case EDDP_USER_CMD_REG_IP_VERSION_VERSION_REV1:
            InteruptEvent = 0;  // for INT_ReqDone_IFA (PNIP revision 1 does not support the IFB)
        break;

        case EDDP_USER_CMD_REG_IP_VERSION_VERSION_REV2:         // INT_ReqDone_IFB is no longer in context with IFB
            InteruptEvent = 1;  // for INT_ReqDone_IFB          // since Rev2 INT_ReqDone_IFB is used for User Comands
        break;
        
        case EDDP_USER_CMD_REG_IP_VERSION_VERSION_REV3:         
            InteruptEvent = 1;  // for INT_ReqDone_IFB
        break;

        default:
        {
            EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_UserCmdInternFillCblAndParam(): FATAL -> Register IP_Version.Version(0x%X) is unknown!!!", 
                ValueTemp32);
            EDDP_USR_CMD_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }

    /* -------------------------------------------------------------------------------- */
    /* fill RequestEntry dependent of asynchronous or synchronous command processing    */
    /* -------------------------------------------------------------------------------- */
    if (EDDP_USER_CMD_ASYNC_PROCESSING == pParam->Async)
    {
        // asynchronous
        RequestEntry = (  (CblOffsetCmdRam >> 3)                                                                        /* Request-Ptr(64Bit aligned address of CBL) */
                        | ((pEDDPUserCmdIntMngm->EDDPLocation == EDDP_LOCATION_EXT_HOST) ? (0x1 << 28) : (0x1 << 27))   /* Interrupt-Maske(IC2 or IC3) */
                        | (InteruptEvent << 29)                                                                         /* Interupt-Event */
                        | EDDP_CMD_REQUEST_WAITING                                                                      /* Request-Status(Waiting) */
                       );
    }
    else
    {
        // synchronous
        RequestEntry = (  (CblOffsetCmdRam >> 3)    /* Request-Ptr(64Bit aligned address of CBL) */
                        | (InteruptEvent << 29)     /* Interupt-Event */
                        | EDDP_CMD_REQUEST_WAITING  /* Request-Status(Waiting) */
                       );
    }

    // write RequestEntry into CMD-Ram
    EDDP_USR_CMD_MEM32_WRITE(RequestEntryHostAddr, RequestEntry);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_UserCmdInternReadCommandReturnValue    +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_USER_CMD_HANDLE_TYPE       Handle      +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Handle     : Handle from eddp_UserCmdInitHandle()                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function reads the command result for the selected   +*/
/*+				  command from command block. It is identical to register   +*/
/*+               SYNCCommand_IF_Status.ConfError.                          +*/
/*+                                                                         +*/
/*+               The result is also present in RequestEntry.ERR.           +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_UserCmdInternReadCommandReturnValue(
	EDDP_USER_CMD_HANDLE_TYPE       Handle)
{
    EDD_RSP                                     Response;
    LSA_UINT32                                  ValueTemp32;
    LSA_UINT32                                  RegQueueErrStatus, RegQueueErrEvent;
    EDD_COMMON_MEM_PTR_TYPE                     CblHostAddr;
    EDDP_USER_CMD_HANDLE_MNGM_SINGLE_PTR_TYPE   pHandleMngm;

    EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:%2X] EDDP_UserCmdInternReadCommandReturnValue()", Handle);

    EDDP_USR_CMD_ASSERT_FALSE(Handle >= EDDP_CFG_MAX_USER_CMD_HANDLES);
    EDDP_USR_CMD_ASSERT_FALSE(LSA_HOST_PTR_ARE_EQUAL(pEDDPUserCmdIntMngm, LSA_NULL));

    Response        = EDD_STS_OK;
    pHandleMngm     = &pEDDPUserCmdIntMngm->HandleMngm[Handle];
    EDDP_USR_CMD_ASSERT_FALSE(LSA_HOST_PTR_ARE_EQUAL(pHandleMngm->pInfoBlockHostAddr, LSA_NULL));
    CblHostAddr     = (EDD_COMMON_MEM_PTR_TYPE) (((EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE)pHandleMngm->pInfoBlockHostAddr)->CblCmdRamAddr);
    CblHostAddr     = (EDD_COMMON_MEM_PTR_TYPE) EDDP_USR_CMD_P2H_ADDR(CblHostAddr, pHandleMngm->pPnipBase);
    ValueTemp32     = EDDP_USR_CMD_MEM32_READ(&(((EDDP_CMD_COMMAND_BLOCK_GENERIC_PTR_TYPE)CblHostAddr)->Generic[1].Low_32Bit)); // identical to register "SYNCCommand_IF_Status"

    // is error present in command result ?
    if ( ((ValueTemp32 & PNIP_REG_SYNCCOMMAND_IF_STATUS__MSK_CONFERROR) >> PNIP_REG_SYNCCOMMAND_IF_STATUS_SHFT_CONFERROR) == PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_CONFERROR_ERROR )
    {
        Response            = EDD_STS_ERR_HW;
        RegQueueErrStatus   = EDDP_USR_REG32_READ(pHandleMngm->pPnipBase, PNIP_REG_QUEUEERRSTATUS);
        RegQueueErrEvent    = EDDP_USR_REG32_READ(pHandleMngm->pPnipBase, PNIP_REG_QUEUEERREVENT);

        // which command was executed
        switch (pHandleMngm->CurrentCmdParam.CmdOpcode)
        {
            case EDDP_USER_CMD_GSYNC_UPDATE_APPCYCL:
            {
                EDDP_USER_TRACE_04(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_UserCmdInternReadCommandReturnValue(): ERROR -> Update_APPLCYCLE, RegQueueErrStatus=0x%X, RegQueueErrEvent=0x%X, Response=0x%X", 
                    Handle, RegQueueErrStatus, RegQueueErrEvent, Response);
            }
            break;

            case EDDP_USER_CMD_GSYNC_CONNECT_FREE:
            {
                EDDP_USER_TRACE_04(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_UserCmdInternReadCommandReturnValue(): ERROR -> Connect_FREE, RegQueueErrStatus=0x%X, RegQueueErrEvent=0x%X, Response=0x%X", 
                    Handle, RegQueueErrStatus, RegQueueErrEvent, Response);
            }
            break;

            case EDDP_USER_CMD_GSYNC_CONNECT_SYNC_A:
            {
                EDDP_USER_TRACE_04(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_UserCmdInternReadCommandReturnValue(): ERROR -> Connect_SYNC_A, RegQueueErrStatus=0x%X, RegQueueErrEvent=0x%X, Response=0x%X", 
                    Handle, RegQueueErrStatus, RegQueueErrEvent, Response);
            }
            break;

            default:
            {
                EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_UserCmdInternReadCommandReturnValue(): FATAL -> CurrentCmdOpcode(0x%X) is unknown!", 
                    pHandleMngm->CurrentCmdParam.CmdOpcode);
                EDDP_USR_CMD_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_UserCmdInternAreCheckBytesPresent      +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_USER_CMD_HANDLE_TYPE       Handle      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_BOOL                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Handle     : Handle from eddp_UserCmdInitHandle()                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function checks whether the CheckBytes are present   +*/
/*+               in UserCmd-Info-Block in CMD-Ram.                         +*/
/*+                                                                         +*/
/*+               It is information whether the UserCmd-Info-Block is       +*/
/*+               valid/invalid.                                            +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_UserCmdInternAreCheckBytesPresent(
    EDDP_USER_CMD_HANDLE_TYPE       Handle)
{
    LSA_BOOL        State = LSA_FALSE;
    LSA_UINT32      CheckBytes;

    EDDP_USR_CMD_ASSERT(Handle < EDDP_CFG_MAX_USER_CMD_HANDLES);

    // read CheckBytes from UserCmd-Info-Block in CMD-Ram
    EDDP_USR_CMD_ASSERT_FALSE(LSA_HOST_PTR_ARE_EQUAL(pEDDPUserCmdIntMngm->HandleMngm[Handle].pInfoBlockHostAddr, LSA_NULL));
    CheckBytes = EDDP_USR_CMD_MEM32_READ(&((EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE)pEDDPUserCmdIntMngm->HandleMngm[Handle].pInfoBlockHostAddr)->CheckBytes);

    if (CheckBytes == EDDP_CMD_USER_CMD_INFO_BLOCK_CHECK_BYTES)
    {
        State = LSA_TRUE;
    }

    return (State);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_UserCmdInternWaitNsTicks               +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_USER_CMD_HANDLE_TYPE       Handle      +*/
/*+                             LSA_UINT32                      nTicks      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Handle     : Handle from eddp_UserCmdInitHandle()                      +*/
/*+  nTicks     : wait time in ns (granularity is 4ns !!!)                  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function can be called to wait for a defined time.   +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_UserCmdInternWaitNsTicks(
    EDDP_USER_CMD_HANDLE_TYPE       Handle,
    LSA_UINT32                      nTicks)
{
    LSA_UINT32                                  RctDelayDiff, RctDelayStart, RctDelayNew;
    EDDP_USER_CMD_HANDLE_MNGM_SINGLE_PTR_TYPE   pHandleMngm;

    EDDP_USR_CMD_ASSERT_FALSE(Handle >= EDDP_CFG_MAX_USER_CMD_HANDLES);
    EDDP_USR_CMD_ASSERT_FALSE(LSA_HOST_PTR_ARE_EQUAL(pEDDPUserCmdIntMngm, LSA_NULL));

    pHandleMngm     = &pEDDPUserCmdIntMngm->HandleMngm[Handle];
    RctDelayStart   = EDDP_USR_REG32_READ(pHandleMngm->pPnipBase, PNIP_REG_RCT_DELAY);

    do
    {
        // two lower bits cut off by HW
        RctDelayNew     = EDDP_USR_REG32_READ(pHandleMngm->pPnipBase, PNIP_REG_RCT_DELAY);
        RctDelayDiff    = RctDelayNew - RctDelayStart;
    } while (RctDelayDiff < nTicks);    // Note: 0..3 --> waits 4ns
}



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_UserCmdInternGetTimeFromPNIPRegister   +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDD_COMMON_MEM_PTR_TYPE    pPnipBase        +*/
/*+                             LSA_UINT64               * pNanoSeconds     +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Handle         : pointer to PnipBase address                           +*/
/*+  pParam         : pointer to to a 64bit nanoseconds value               +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: Read time from PNIP register OCT_Time in nanoseconds      +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_UserCmdInternGetTimeFromPNIPRegister (
    EDD_COMMON_MEM_PTR_TYPE     pPnipBase,
    LSA_UINT64                * pNanoSeconds)
{
    LSA_UINT32 highTime;
    LSA_UINT32 lowTime;
    LSA_UINT32 firstTime;
    
    // First read high 32bit part of time... 
    firstTime = EDDP_USR_REG32_READ(pPnipBase, PNIP_REG_OCTHIGH_TIME);
    // ...then read low 32bit part of time... 
    lowTime = EDDP_USR_REG32_READ(pPnipBase, PNIP_REG_OCTLOW_TIME);
    // ...and read again high 32bit part of time...
    highTime = EDDP_USR_REG32_READ(pPnipBase, PNIP_REG_OCTHIGH_TIME);
    // ...to detect a possible wrap around
    if(highTime != firstTime)
    {
        // If wrap around occurred, read low 32bit part of time again
        lowTime = EDDP_USR_REG32_READ(pPnipBase, PNIP_REG_OCTLOW_TIME);
    }

    // Now store time into 64bit nanoseconds
    *pNanoSeconds   = highTime;
    *pNanoSeconds <<= 32;
    *pNanoSeconds  |= lowTime;
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_UserCmdInternTimeStampGet              +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_USER_CMD_HANDLE_TYPE       Handle      +*/
/*+                             EDDP_USER_CMD_PARAM_PTR_TYPE    pParam      +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Handle         : Handle from eddp_UserCmdInitHandle()                  +*/
/*+  pParam         : pointer to command parameter block                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: TimeStampGet implementation                               +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_UserCmdInternTimeStampGet(
	EDDP_USER_CMD_HANDLE_TYPE       Handle,
 	EDDP_USER_CMD_PARAM_PTR_TYPE    pParam)
{
    EDD_RSP                                   Response = EDD_STS_OK;
    EDDP_USER_CMD_HANDLE_MNGM_SINGLE_PTR_TYPE pHandleMngm = &pEDDPUserCmdIntMngm->HandleMngm[Handle];
    EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE pInfoBlockHostAddr = (EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE)(pHandleMngm->pInfoBlockHostAddr);
    EDDP_TIMESTAMP_INFO_PTR_TYPE              pTimestampInfo = pParam->Param.TimeStampGet.pTimestamp;

    EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_UserCmdInternTimeStampGet()", Handle);

    // address of ParamStruct in CMD-Ram
    EDDP_USR_CMD_ASSERT_FALSE(LSA_HOST_PTR_ARE_EQUAL(pInfoBlockHostAddr, LSA_NULL));

    if (LSA_HOST_PTR_ARE_EQUAL(pTimestampInfo, LSA_NULL))
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_UserCmdInternTimeStampGet(): ERROR -> pTimestampInfo=NULL!", Handle); 
    }
    else
    {
        // Read time from PNIP register OCT_Time
        LSA_UINT64 nanoseconds;

        LSA_UINT32 TimeGetSequenceID, TimeGetSequenceIDCopy;
        LSA_UINT32 TimeGetStatusSyncState, TimeGetEpochUTCOffset;
        LSA_UINT32 TimeGetMACAddressLo, TimeGetMACAddressHi;

        do {
            Response = EDD_STS_OK_PENDING;

            // at first, get SequenceID from UserCmd-Info-Block
            TimeGetSequenceID = EDDP_USR_CMD_MEM32_READ(&pInfoBlockHostAddr->TimeGetSequenceID);

            // get current SOE Status and SyncState from UserCmd-Info-Block
            TimeGetStatusSyncState = EDDP_USR_CMD_MEM32_READ(&pInfoBlockHostAddr->TimeGetStatusSyncState);

            // get current UTC offset and PTCP epoch from UserCmd-Info-Block
            TimeGetEpochUTCOffset = EDDP_USR_CMD_MEM32_READ(&pInfoBlockHostAddr->TimeGetEpochUTCOffset);

            // get MAC Address from UserCmd-Info-Block
            TimeGetMACAddressLo = EDDP_USR_CMD_MEM32_READ(&pInfoBlockHostAddr->TimeGetMACAddressLo);
            TimeGetMACAddressHi = EDDP_USR_CMD_MEM32_READ(&pInfoBlockHostAddr->TimeGetMACAddressHi);

            // Read time from PNIP register OCT_Time
            EDDP_UserCmdInternGetTimeFromPNIPRegister(pHandleMngm->pPnipBase, &nanoseconds);

            // last action, get the copy of SequenceID from UserCmd-Info-Block
            TimeGetSequenceIDCopy = EDDP_USR_CMD_MEM32_READ(&pInfoBlockHostAddr->TimeGetSequenceIDCopy);
            if (TimeGetSequenceID == TimeGetSequenceIDCopy)
            {
                // and read the SequenceID again
                TimeGetSequenceID = EDDP_USR_CMD_MEM32_READ(&pInfoBlockHostAddr->TimeGetSequenceID);
                if (TimeGetSequenceID == TimeGetSequenceIDCopy)
                {
                    Response = EDD_STS_OK;
                }
            }

            // during a time set operation, SequenceID and it's copy are different
            if (EDD_STS_OK != Response)
            {
                if (LSA_HOST_PTR_ARE_EQUAL(pEDDPUserCmdIntMngm->pSharedMem, LSA_NULL))
                {
                    Response = EDD_STS_OK;      // without a Shared Memory it is not possible to provide consistent timestamps, but an error is not allowed at this point
                    EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_WARN, "[H:%2X] EDDP_UserCmdInternTimeStampGet(): pSharedMem==NULL!", 
                                       Handle); 
                }
                else
                {
                    //LSA_UINT32 diff;

                    // get old Timestamp data from Shared Memory
                    EDDP_TIMESTAMP_INFO_TYPE volatile * pTimestampInfoBackup = &pEDDPUserCmdIntMngm->pSharedMem->TimestampInfoBackup;
                    TimeGetSequenceID        = pTimestampInfoBackup->SequenceID;
                    TimeGetStatusSyncState   = pTimestampInfoBackup->TimesStamp.Status;
                    TimeGetStatusSyncState <<= 16;
                    TimeGetStatusSyncState  |= pTimestampInfoBackup->SyncState;
                    TimeGetEpochUTCOffset    = pTimestampInfoBackup->TimesStamp.SecondsHigh;
                    TimeGetEpochUTCOffset  <<= 16;
                    TimeGetEpochUTCOffset   |= pTimestampInfoBackup->UTCOffset;
                    EDDP_MAC_ADDR_EUI64_TO_HI_LO(pTimestampInfoBackup->MasterIdent, &TimeGetMACAddressHi, &TimeGetMACAddressLo);

                    // Time before the invalidation of the InfoBlock by incrementing the SequenceID
                    nanoseconds   = EDDP_USR_CMD_MEM32_READ(&pInfoBlockHostAddr->TimeGetOctHighTimeOld);
                    nanoseconds <<= 32;
                    nanoseconds  |= EDDP_USR_CMD_MEM32_READ(&pInfoBlockHostAddr->TimeGetOctLowTimeOld);

                    // Time difference since the invalidation of the InfoBlock
                    //diff  = EDDP_USR_REG32_READ(pHandleMngm->pPnipBase, PNIP_REG_RCT_DELAY);
                    //diff -= EDDP_USR_CMD_MEM32_READ(&pInfoBlockHostAddr->TimeGetRctDelayOld);
                    // Calculate the time 
                    //nanoseconds += diff;

                    // The SequenceID has to be inconsistent at the end of this sequence, otherwise a simple re-read of the timestamp is sufficient
                    TimeGetSequenceIDCopy = EDDP_USR_CMD_MEM32_READ(&pInfoBlockHostAddr->TimeGetSequenceIDCopy);
                    if (TimeGetSequenceID != TimeGetSequenceIDCopy)
                    {
                        Response = EDD_STS_OK;
                    }
                }
            }
        } while (EDD_STS_OK != Response);

        // fill out the timestamp struct
        pTimestampInfo->SyncState = (LSA_UINT16)TimeGetStatusSyncState;
        pTimestampInfo->UTCOffset = (LSA_UINT16)TimeGetEpochUTCOffset;
        pTimestampInfo->SequenceID = TimeGetSequenceID;
        pTimestampInfo->TimesStamp.Status = (LSA_UINT16)(TimeGetStatusSyncState >> 16);
        pTimestampInfo->TimesStamp.SecondsHigh = (LSA_UINT16)(TimeGetEpochUTCOffset >> 16);
        pTimestampInfo->TimesStamp.Nanoseconds = (LSA_UINT32)(nanoseconds % 1000000000);
        pTimestampInfo->TimesStamp.SecondsLow = (LSA_UINT32)(nanoseconds / 1000000000);

        if (EDDP_PNIO_STATUS_NOT_SYNC == pTimestampInfo->TimesStamp.Status)
        {
            // As long as NOT_SYNC, the SeqID must be manipulated.
            if (pTimestampInfo->SequenceID > 0)
            {
                pTimestampInfo->SequenceID--;
            }
        }

        if (EDDP_PNIO_STATUS_MASTER_SYNC == pTimestampInfo->TimesStamp.Status)
        {
            EDDP_MAC_ADDR_HI_LO_TO_EUI64(pTimestampInfo->MasterIdent, TimeGetMACAddressHi, TimeGetMACAddressLo);
            if (LSA_HOST_PTR_ARE_EQUAL(pEDDPUserCmdIntMngm->pSharedMem, LSA_NULL))
            {
                // without a Shared Memory it is not possible to provide a valid DomainIdent, but an error is not allowed at this point
                EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_WARN, "[H:%2X] EDDP_UserCmdInternTimeStampGet(): pSharedMem==NULL!", Handle);
                EDDP_MEMSET_COMMON(pTimestampInfo->DomainIdent, 0, sizeof(pTimestampInfo->DomainIdent));
            }
            else
            {
                EDDP_MEMCOPY_COMMON(pTimestampInfo->DomainIdent, pEDDPUserCmdIntMngm->pSharedMem->TimestampInfoBackup.DomainIdent, sizeof(pTimestampInfo->DomainIdent));
            }
        }
        else
        {
            EDDP_MEMSET_COMMON(pTimestampInfo->MasterIdent, 0, sizeof(pTimestampInfo->MasterIdent));
            EDDP_MEMSET_COMMON(pTimestampInfo->DomainIdent, 0, sizeof(pTimestampInfo->DomainIdent));
        }

        EDDP_USER_TRACE_06(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_UserCmdInternTimeStampGet(): Status=%u Epoch=%u Seconds=%u.%09u UTCOffset=%u SyncState=0x%04x", 
            pTimestampInfo->TimesStamp.Status,pTimestampInfo->TimesStamp.SecondsHigh,pTimestampInfo->TimesStamp.SecondsLow, pTimestampInfo->TimesStamp.Nanoseconds,
            pTimestampInfo->UTCOffset,pTimestampInfo->SyncState);
        EDDP_USER_TRACE_09(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_UserCmdInternTimeStampGet(): SequenceID=%d TimeMasterIdent=%02x-%02x-%02x-%02x %02x-%02x-%02x-%02x", 
            pTimestampInfo->SequenceID, 
            pTimestampInfo->MasterIdent[0], pTimestampInfo->MasterIdent[1], pTimestampInfo->MasterIdent[2], pTimestampInfo->MasterIdent[3], 
            pTimestampInfo->MasterIdent[4], pTimestampInfo->MasterIdent[5], pTimestampInfo->MasterIdent[6], pTimestampInfo->MasterIdent[7]);
    }

    EDDP_USER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:%2X] OUT: EDDP_UserCmdInternTimeStampGet() Response=0x%x", Handle, Response);

    return(Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_UserCmdInternTimeSyncIndGet            +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_USER_CMD_HANDLE_TYPE       Handle      +*/
/*+                             EDDP_USER_CMD_PARAM_PTR_TYPE    pParam      +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Handle         : Handle from eddp_UserCmdInitHandle()                  +*/
/*+  pParam         : pointer to command parameter block                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: TimeSyncIndGet implementation                             +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_UserCmdInternTimeSyncIndGet(
	EDDP_USER_CMD_HANDLE_TYPE       Handle,
 	EDDP_USER_CMD_PARAM_PTR_TYPE    pParam)
{
    EDD_RSP                                   Response       = EDD_STS_OK;
    EDDP_TIMESTAMP_INFO_TYPE          * const pOldTimestamp  = pParam->Param.TimeStampGet.pOldTimestamp;
    EDDP_TIMESTAMP_INFO_TYPE          * const pNewTimestamp  = pParam->Param.TimeStampGet.pNewTimestamp;

    #if  (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(Handle);
    #endif

    EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_UserCmdInternTimeSyncIndGet()", Handle);

    if ( (LSA_HOST_PTR_ARE_EQUAL(pOldTimestamp, LSA_NULL)) || (LSA_HOST_PTR_ARE_EQUAL(pNewTimestamp, LSA_NULL)) )
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_USER_TRACE_03(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_UserCmdInternTimeSyncIndGet(): ERROR -> pOldTimestamp=0x%X pNewTimestamp=0x%X", 
                           Handle, pOldTimestamp, pNewTimestamp); 
    }
    else if (LSA_HOST_PTR_ARE_EQUAL(pEDDPUserCmdIntMngm->pSharedMem, LSA_NULL))
    {
        Response = EDD_STS_ERR_NOT_SUPPORTED;
        EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_WARN, "[H:%2X] EDDP_UserCmdInternTimeSyncIndGet(): pSharedMem==NULL!", 
                           Handle); 
    }
    else
    {
        Response = EDDP_UserCmd_CList_ReadTimeSyncInd(pEDDPUserCmdIntMngm->pSharedMem, pOldTimestamp, pNewTimestamp);
        if (EDD_STS_OK != Response)
        {
            EDDP_USER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_UserCmdInternTimeSyncIndGet(): ERROR -> EDDP_UserCmd_CList_ReadTimeSyncInd() failed! Response=0x%x",
                               Handle, Response); 
        }
        else
        {
            EDDP_USER_TRACE_06(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_UserCmdInternTimeSyncIndGet(): Status=%u Epoch=%u Seconds=%u.%09u UTCOffset=%u SyncState=0x%04x", 
                pOldTimestamp->TimesStamp.Status,pOldTimestamp->TimesStamp.SecondsHigh,pOldTimestamp->TimesStamp.SecondsLow, pOldTimestamp->TimesStamp.Nanoseconds,
                pOldTimestamp->UTCOffset,pOldTimestamp->SyncState);
            EDDP_USER_TRACE_09(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_UserCmdInternTimeSyncIndGet(): SequenceID=%d TimeMasterIdent=%02x-%02x-%02x-%02x %02x-%02x-%02x-%02x", 
                pOldTimestamp->SequenceID,
                pOldTimestamp->MasterIdent[0], pOldTimestamp->MasterIdent[1], pOldTimestamp->MasterIdent[2], pOldTimestamp->MasterIdent[3],
                pOldTimestamp->MasterIdent[4], pOldTimestamp->MasterIdent[5], pOldTimestamp->MasterIdent[6], pOldTimestamp->MasterIdent[7]);
            EDDP_USER_TRACE_06(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_UserCmdInternTimeSyncIndGet(): Status=%u Epoch=%u Seconds=%u.%09u UTCOffset=%u SyncState=0x%04x", 
                pNewTimestamp->TimesStamp.Status,pNewTimestamp->TimesStamp.SecondsHigh,pNewTimestamp->TimesStamp.SecondsLow, pNewTimestamp->TimesStamp.Nanoseconds,
                pNewTimestamp->UTCOffset,pNewTimestamp->SyncState);
            EDDP_USER_TRACE_09(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_UserCmdInternTimeSyncIndGet(): SequenceID=%d TimeMasterIdent=%02x-%02x-%02x-%02x %02x-%02x-%02x-%02x", 
                pNewTimestamp->SequenceID,
                pNewTimestamp->MasterIdent[0], pNewTimestamp->MasterIdent[1], pNewTimestamp->MasterIdent[2], pNewTimestamp->MasterIdent[3],
                pNewTimestamp->MasterIdent[4], pNewTimestamp->MasterIdent[5], pNewTimestamp->MasterIdent[6], pNewTimestamp->MasterIdent[7]);
        }
    }

    EDDP_USER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:%2X] OUT: EDDP_UserCmdInternTimeSyncIndGet() Response=0x%x", Handle, Response);

    return(Response);
}



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_UserCmdInternFreeHandle                +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_USER_CMD_HANDLE_TYPE       Handle      +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Handle     : Handle from eddp_UserCmdInitHandle()                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: Deinit the given handle.                                  +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_UserCmdInternFreeHandle(
	EDDP_USER_CMD_HANDLE_TYPE       Handle)
{
    EDD_RSP                                     Response;
    EDDP_USER_CMD_HANDLE_MNGM_SINGLE_PTR_TYPE   pHandleMngm;

    EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:%2X] EDDP_UserCmdInternFreeHandle()", Handle);

    EDDP_USR_CMD_ASSERT_FALSE(Handle >= EDDP_CFG_MAX_USER_CMD_HANDLES);

    Response = EDD_STS_OK;

    /* -------------------------------------------- */
    /* checks                                       */
    /* -------------------------------------------- */
    // check "pEDDPUserCmdIntMngm"
    if (LSA_HOST_PTR_ARE_EQUAL(pEDDPUserCmdIntMngm, LSA_NULL))
    {
  	    Response = EDD_STS_ERR_SEQUENCE;
        EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_UserCmdInternFreeHandle(): ERROR -> pEDDPUserCmdIntMngm=NULL! Response=0x%X", 
            Response);
    }
    else
    {
        // check the given handle is used
        if (!(pEDDPUserCmdIntMngm->HandleMngm[Handle].InUseHandle))
        {
  	        Response = EDD_STS_ERR_SEQUENCE;
            EDDP_USER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_UserCmdInternFreeHandle(): ERROR -> given Handle(0x%X) is not used! Response=0x%X", 
                Handle, Response);
        }
        else
        {
            // check no user command is running currently
            if (pEDDPUserCmdIntMngm->HandleMngm[Handle].CurrentCmdParam.CmdOpcode != EDDP_USER_CMD_CURRENT_CMD_OPCODE_INVALID)
            {
                // check the EDDP-DeviceShutdown is already done
                if (!(EDDP_UserCmdInternAreCheckBytesPresent(Handle)))
                {
                    // this is no error
                    EDDP_USER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:%2X] EDDP_UserCmdInternFreeHandle(): one asynchronous command is still running but EDDP-DeviceShutdown is already done. CmdOpcode=0x%X", 
                        Handle, pEDDPUserCmdIntMngm->HandleMngm[Handle].CurrentCmdParam.CmdOpcode);
                }
                else
                {
                    Response = EDD_STS_ERR_SEQUENCE;
                    EDDP_USER_TRACE_03(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_UserCmdInternFreeHandle(): ERROR -> one asynchronous command is still running! CmdOpcode=0x%X, Response=0x%X", 
                        Handle, pEDDPUserCmdIntMngm->HandleMngm[Handle].CurrentCmdParam.CmdOpcode, Response);
                }
            }
        }
    }

    /* -------------------------------------------- */
    /* deinit this handle                           */
    /* -------------------------------------------- */
    if (Response == EDD_STS_OK)
    {
        pHandleMngm = &pEDDPUserCmdIntMngm->HandleMngm[Handle];

        pHandleMngm->InUseHandle        = LSA_FALSE;
        pHandleMngm->pPnipBase          = LSA_NULL;
        pHandleMngm->pInfoBlockHostAddr = LSA_NULL;

        EDDP_USR_CMD_ASSERT_FALSE(pEDDPUserCmdIntMngm->HandleUsedCnt == 0);
        pEDDPUserCmdIntMngm->HandleUsedCnt--;
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_UserCmdInternDeinitHandleMngm          +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function deinits the handle management               +*/
/*+               for user command interface.                               +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_UserCmdInternDeinitHandleMngm(LSA_VOID)
{
    EDD_RSP         Response = EDD_STS_OK;

    EDDP_USER_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_UserCmdInternDeinitHandleMngm()");

    /* -------------------------------------------- */
    /* checks                                       */
    /* -------------------------------------------- */
    // check "pEDDPUserCmdIntMngm"
    if (LSA_HOST_PTR_ARE_EQUAL(pEDDPUserCmdIntMngm, LSA_NULL))
    {
  	    Response = EDD_STS_ERR_SEQUENCE;
        EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_UserCmdInternDeinitHandleMngm(): ERROR -> pEDDPUserCmdIntMngm=NULL! Response=0x%X", 
            Response);
    }
    else
    {
        // check one handle is still in use
        if (pEDDPUserCmdIntMngm->HandleUsedCnt > 0)
        {
  	        Response = EDD_STS_ERR_SEQUENCE;
            EDDP_USER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_UserCmdInternDeinitHandleMngm(): ERROR -> at least one handle is still in use! HandleUsedCnt=%d, Response=0x%X!", 
                pEDDPUserCmdIntMngm->HandleUsedCnt, Response);
        }
    }

    /* -------------------------------------------- */
    /* deinit the user command interface            */
    /* -------------------------------------------- */
    if (Response == EDD_STS_OK)
    {
        // delete main handle management for EDDP user command intern
        EDDP_MEMSET_COMMON((EDD_COMMON_MEM_PTR_TYPE)pEDDPUserCmdIntMngm, 0, sizeof(EDDP_USER_CMD_INT_MNGM_TYPE));
        pEDDPUserCmdIntMngm = LSA_NULL;
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_UserCmdInternFatalError                +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_FATAL_ERROR_TYPE   Error               +*/
/*+                             LSA_UINT16              ModuleID            +*/
/*+                             LSA_UINT16              Line                +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Error      : EDD-fatal-errorcode                                       +*/
/*+  ModuleID   : module - id of error                                      +*/
/*+  Line       : line of code (optional)                                   +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: Signals Fatal-Error via EDDP_FATAL_ERROR.                 +*/
/*+               This function does not return!                            +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_UserCmdInternFatalError(
	EDDP_FATAL_ERROR_TYPE   Error,
	LSA_UINT16              ModuleID,
	LSA_UINT32              Line)
{
    LSA_FATAL_ERROR_TYPE    LSAError;

    LSAError.lsa_component_id       = LSA_COMP_ID_EDD;
    LSAError.module_id              = ModuleID;
    LSAError.line                   = (LSA_UINT16) Line;
    LSAError.error_code[0]          = (LSA_UINT32) Error;
    LSAError.error_code[1]          = 0;
    LSAError.error_code[2]          = 0;
    LSAError.error_code[3]          = 0;
    LSAError.error_data_length      = 0;
    LSAError.error_data_ptr         = LSA_NULL; /* currently no message */

    EDDP_USER_TRACE_03(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_UserCmdInternFatalError(ModuleID=%d, Line=%d, Error=0x%X)",
        ModuleID, Line, Error);

    EDDP_FATAL_ERROR(sizeof(LSA_FATAL_ERROR_TYPE), &LSAError);

    /* should not return! */
    while (1) ;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_UserCmd_CopyTimeStamp                  +*/
/*+                                                                         +*/
/*+  Output                :    EDDP_TIMESTAMP_INFO_TYPE *pDest             +*/
/*+  Input                 :    EDDP_TIMESTAMP_INFO_TYPE *pSrc              +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: Internal function to copy timestamps                      +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static EDD_RSP EDDP_LOCAL_FCT_ATTR  EDDP_UserCmd_CopyTimeStamp(EDDP_TIMESTAMP_INFO_TYPE * const pDest, volatile const EDDP_TIMESTAMP_INFO_TYPE * const pSrc)
{
    EDD_RSP Response = EDD_STS_OK;
    LSA_INT32 i;

    if ( (LSA_HOST_PTR_ARE_EQUAL(pDest, LSA_NULL)) || (LSA_HOST_PTR_ARE_EQUAL(pSrc, LSA_NULL)) )
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_USER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_UserCmd_CopyTimeStamp(): ERROR -> pDest=0x%X pSrc=0x%X", pDest, pSrc);
    }
    else 
    {
        pDest->TimesStamp.Status        = pSrc->TimesStamp.Status;
        pDest->TimesStamp.SecondsHigh   = pSrc->TimesStamp.SecondsHigh;
        pDest->TimesStamp.SecondsLow    = pSrc->TimesStamp.SecondsLow;
        pDest->TimesStamp.Nanoseconds   = pSrc->TimesStamp.Nanoseconds;

        pDest->UTCOffset                = pSrc->UTCOffset;
        pDest->SyncState                = pSrc->SyncState;
        pDest->SequenceID               = pSrc->SequenceID;

        for (i = 0; i < EDDP_TIME_MASTER_IDENT_SIZE; i++)
        {
            pDest->MasterIdent[i] = pSrc->MasterIdent[i];
        }

        for (i = 0; i < EDDP_TIME_DOMAIN_IDENT_SIZE; i++)
        {
            pDest->DomainIdent[i] = pSrc->DomainIdent[i];
        }
    }

    return (Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_UserCmd_CList_ReadTimeSyncInd          +*/
/*+                                                                         +*/
/*+  Input                 :    EDDP_USERCMDSHAREDMEM_TYPE * pSharedMem     +*/
/*+  Output                :    EDDP_TIMESTAMP_INFO_TYPE   * pOldTimestamp  +*/
/*+  Output                :    EDDP_TIMESTAMP_INFO_TYPE   * pNewTimestamp  +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: The function reads the timestamps from the Shared Memory  +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_UserCmd_CList_ReadTimeSyncInd( EDDP_USERCMDSHAREDMEM_TYPE * const pSharedMem,
                                                                EDDP_TIMESTAMP_INFO_TYPE   * const pOldTimestamp,
                                                                EDDP_TIMESTAMP_INFO_TYPE   * const pNewTimestamp)
{
    EDD_RSP Response = EDD_STS_OK;

    if (LSA_HOST_PTR_ARE_EQUAL(pSharedMem, LSA_NULL))
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_UserCmd_CList_ReadTimeSyncInd(): ERROR -> pSharedMem=NULL! Response=0x%X", Response);
    }
    else if (pSharedMem->CircListDescr.DstReadIndex == pSharedMem->CircListDescr.SrcWriteIndex)
    {
        Response = EDD_STS_OK_NO_DATA;
        EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] EDDP_UserCmd_CList_ReadTimeSyncInd(): List is empty! Response=0x%X", Response);
    }
    else 
    {
        LSA_UINT32 ReadIndex = pSharedMem->CircListDescr.DstReadIndex;
        volatile EDDP_TIMESTAMP_INFO_PTR_TYPE pSMOldTimestamp = &pSharedMem->TimestampList[ReadIndex].OldTimestamp;
        volatile EDDP_TIMESTAMP_INFO_PTR_TYPE pSMNewTimestamp = &pSharedMem->TimestampList[ReadIndex].NewTimestamp;

        // Read Timestamps from List
        Response = EDDP_UserCmd_CopyTimeStamp(pOldTimestamp, pSMOldTimestamp);
        if (EDD_STS_OK != Response)
        {
            EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_UserCmd_CList_ReadTimeSyncInd(): ERROR -> Copy Old TimeStamp failed! Response=0x%X", Response);
        }
        else
        {
            Response = EDDP_UserCmd_CopyTimeStamp(pNewTimestamp, pSMNewTimestamp);
            if (EDD_STS_OK != Response)
            {
                EDDP_USER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_UserCmd_CList_ReadTimeSyncInd(): ERROR -> Copy New TimeStamp failed! Response=0x%X", Response);
            }
        }

        // Update ReadIndex
        ReadIndex++;
        if (ReadIndex >= pSharedMem->CircListDescr.ListSize)
        {
            ReadIndex = 0;
        }
        pSharedMem->CircListDescr.DstReadIndex = ReadIndex;
    }
    return (Response);
}


#endif  // #ifdef EDDP_CFG_USR_CMD_API_INCLUDE

/*****************************************************************************/
/*  end of file eddp_usr_cmd_intern.c                                        */
/*****************************************************************************/
