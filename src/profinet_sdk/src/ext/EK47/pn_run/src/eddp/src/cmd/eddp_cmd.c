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
/*  F i l e               &F: eddp_cmd.c                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP Command Control modul functions             */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  29.01.09    AB    initial version                                        */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID       101
#define EDDP_MODULE_ID          LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_CMD */

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

/*===========================================================================*/
/*                                Constants                                  */
/*===========================================================================*/

/*===========================================================================*/
/*                                 functions                                 */
/*===========================================================================*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CMDDeviceOpen                          +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             EDDP_UPPER_DPB_PTR_TYPE        pDPB         +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
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
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDD_SRV_DEV_OPEN                                          +*/
/*+               Initializes all CMD management structures within DDB.     +*/
/*+                                                                         +*/
/*+       Note:   EDD_STS_OK_PENDING is not allowed as response. This call  +*/
/*+               must be synchronous!                                      +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CMDDeviceOpen(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DPB_PTR_TYPE     pDPB)
{
    EDD_RSP     Response = EDD_STS_OK;
    EDDP_LOCAL_DDB_CMD_PTR_TYPE pCMD = &pDDB->CMD;

    LSA_UNUSED_ARG(pDPB);

    EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] EDDP_CMDDeviceOpen()");
    
    /* command specific parameter, hw dependent */
    if ( (pDDB->Hardware == EDDP_HW_ERTEC200P) || (pDDB->Hardware == EDDP_HW_ERTEC200P_FPGA) )
    {        
        pCMD->MaxAcwCountPerCmd = EDDP_CMD_MAX_ACW_CNT_PER_CMD_REV12;       /* maximum acw count per command */
        pCMD->CrtCmdDepth       = EDDP_CMD_CPM_PPM_COMMAND_DEPTH_REV12;     /* maximum amount of parallel PPM/CPM commands */
        pCMD->CrtCmdDepthAlloc  = EDDP_CMD_CPM_PPM_COMMAND_DEPTH_REV12 + 1; /* maximum abount of internal PPM/CPM parameter elements */
        /* NOTE: +1 --> 1 reserved entry */
    }
    else if( (pDDB->Hardware == EDDP_HW_HERA)  || (pDDB->Hardware == EDDP_HW_HERA_FPGA) )
    {
        pCMD->MaxAcwCountPerCmd = EDDP_CMD_MAX_ACW_CNT_PER_CMD_REV3;       /* maximum acw count per command */
        pCMD->CrtCmdDepth       = EDDP_CMD_CPM_PPM_COMMAND_DEPTH_REV3;     /* maximum amount of parallel PPM/CPM commands */
        pCMD->CrtCmdDepthAlloc  = EDDP_CMD_CPM_PPM_COMMAND_DEPTH_REV3 + 2; /* maximum abount of internal PPM/CPM parameter elements */
        /* NOTE: +2 --> 1 reserved entry + 1 delay command */
    }        
    else
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CMDDeviceOpen(): ERROR -> Cannot map Hardware!");
    }
    
    /* NOTE: hw resources for CRT commands are allocated within HAL-Open, so check size against size from HAL */
    if(pCMD->CrtCmdDepthAlloc != pDDB->Core.Hal.AllocHwResourceCrtCmd)
    {
        EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_CMDDeviceOpen(): FATAL -> allocation size for CRT command resources inconsistent!");
        EDDP_FATAL(EDDP_FATAL_ERR_DEV_OPEN);
    }
    
    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CMDDeviceSetup                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             EDDP_UPPER_DSB_PTR_TYPE        pDSB         +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
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
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDDP_SRV_DEV_SETUP                                        +*/
/*+               Setup Device for CMD                                      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CMDDeviceSetup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DSB_PTR_TYPE     pDSB)
{
    LSA_UNUSED_ARG(pDSB);
   
    EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] EDDP_CMDDeviceSetup()");
    
    _EDDP_CMD_Setup_QueueMgmtTable (pDDB);
    
    return (EDD_STS_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CMDDeviceShutdown                      +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE           pDDB      +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_OK_PENDING                                        +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDDP_SRV_DEV_SHUTDOWN or EDDP_SRV_DEV_SETUP (only if      +*/
/*+               previous setup shall be aborted)                          +*/
/*+               Shutdown Interface of CMD                                 +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CMDDeviceShutdown(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] EDDP_CMDDeviceShutdown()");

    _EDDP_CMD_Delete_QueueMgmtTable (pDDB);

    return (EDD_STS_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CMDDeviceClose                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE           pDDB      +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDDP_SRV_DEV_CLOSE or EDDP_SRV_DEV_OPEN (only if          +*/
/*+               previous OPEN shall be aborted)                           +*/
/*+               Close device for CMD                                      +*/
/*+                                                                         +*/
/*+       Note:   EDD_STS_OK_PENDING not allowed as response. This call     +*/
/*+               must be synchronous!                                      +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CMDDeviceClose(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] EDDP_CMDDeviceClose()");
    LSA_UNUSED_ARG(pDDB);
    return (EDD_STS_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :  _EDDP_CMD_CheckpPermissionToRunCommandOnQueue +*/
/*+  Input/Output          :  EDDP_LOCAL_DDB_PTR_TYPE        pDDB           +*/
/*+                           LSA_UINT32                     QueueIndex     +*/
/*+                           LSA_UINT32                     CommandNumber  +*/
/*+                                                                         +*/
/*+  Result                :  LSA_BOOL                                      +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB                   : Pointer to DeviceDescriptionBlock       (in)  +*/
/*+  QueueIndex             : index of queue                          (in)  +*/
/*+  CommandNumber          : number of command                       (in)  +*/
/*+                                                                         +*/
/*+  Results                : LSA_TRUE  : command can be run                +*/
/*+                           LSA_FALSE : command can not be run            +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function checks the permission to run given command  +*/
/*+               on given queue.                                           +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static LSA_BOOL _EDDP_CMD_CheckpPermissionToRunCommandOnQueue(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  QueueIndex,
    LSA_UINT32                  CommandNumber)
{
    LSA_BOOL permission = LSA_TRUE;
    EDDP_CMD_QUEUE_INFO_PTR_TYPE pCurrent_Queue_Info;
    
    pCurrent_Queue_Info = &(pDDB->CMD.Queue[QueueIndex]);
    
    /* non-CRT queue */
    if(EDDP_CMD_INDEX_HOST_ASYNC_CRT != QueueIndex)
    {
        if(pCurrent_Queue_Info->RunningCommand & ((LSA_UINT32)0x1 << CommandNumber))
        {
            permission = LSA_FALSE;
        }
    }
    /* CRT queue */
    else if (pCurrent_Queue_Info->CmdRunningCount >= pDDB->CMD.CrtCmdDepthAlloc)
    {
        /* shall not happen because a maximum of running commands is checked */
        permission = LSA_FALSE;
    }
    return permission;
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :  _EDDP_CMD_CheckpPermissionToRunCommandOnQueue +*/
/*+  Input/Output          :  EDDP_LOCAL_DDB_PTR_TYPE        pDDB           +*/
/*+                           LSA_UINT32                     QueueIndex     +*/
/*+                           LSA_UINT32                     CommandNumber  +*/
/*+                                                                         +*/
/*+  Result                :  LSA_VOID                                      +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB                   : Pointer to DeviceDescriptionBlock       (in)  +*/
/*+  QueueIndex             : index of queue                          (in)  +*/
/*+  CommandNumber          : number of command                       (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function sets the given commmand to running.         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static LSA_VOID _EDDP_CMD_SetCmdRunningFlag(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  QueueIndex,
    LSA_UINT32                  CommandNumber)
{
    /* NOTE: CRT queue can process several equal commands in parallel */
    
    /* non-CRT queue */
    if(EDDP_CMD_INDEX_HOST_ASYNC_CRT != QueueIndex)
    {
        EDDP_CMD_QUEUE_INFO_PTR_TYPE pCurrent_Queue_Info = &(pDDB->CMD.Queue[QueueIndex]);
        
        pCurrent_Queue_Info->RunningCommand = pCurrent_Queue_Info->RunningCommand | ((LSA_UINT32)0x1 << CommandNumber);
    }
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :  _EDDP_CMD_CheckpPermissionToRunCommandOnQueue +*/
/*+  Input/Output          :  EDDP_LOCAL_DDB_PTR_TYPE        pDDB           +*/
/*+                           LSA_UINT32                     QueueIndex     +*/
/*+                           LSA_UINT32                     CommandNumber  +*/
/*+                                                                         +*/
/*+  Result                :  LSA_VOID                                      +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB                   : Pointer to DeviceDescriptionBlock       (in)  +*/
/*+  QueueIndex             : index of queue                          (in)  +*/
/*+  CommandNumber          : number of command                       (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function sets the given commmand to not running.     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static LSA_VOID _EDDP_CMD_ResetCmdRunningFlag(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  QueueIndex,
    LSA_UINT32                  CommandNumber)
{
    /* NOTE: CRT queue can process several equal commands in parallel */
    
    /* non-CRT queue */
    if(EDDP_CMD_INDEX_HOST_ASYNC_CRT != QueueIndex)
    {
        EDDP_CMD_QUEUE_INFO_PTR_TYPE pCurrent_Queue_Info = &(pDDB->CMD.Queue[QueueIndex]);
        
        pCurrent_Queue_Info->RunningCommand = (pCurrent_Queue_Info->RunningCommand & ~((LSA_UINT32)0x1 << CommandNumber));
    }
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :  _EDDP_CMD_TriggerQueue                        +*/
/*+  Input/Output          :  EDDP_LOCAL_DDB_PTR_TYPE        pDDB           +*/
/*+                           LSA_UINT32                     QueueIndex     +*/
/*+                                                                         +*/
/*+  Result                :  LSA_VOID                                      +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB                   : Pointer to DeviceDescriptionBlock       (in)  +*/
/*+  QueueIndex             : index of queue                          (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function triggers the command queue with given       +*/
/*+               queue index. CRT queue is only triggered for a command    +*/
/*+               sequence and not for every command that is enqueued.      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static LSA_VOID _EDDP_CMD_TriggerQueue(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  QueueIndex)
{    
    if( EDDP_CMD_INDEX_HOST_ASYNC_CRT != QueueIndex )
    { 
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_TRIGGERREQQUEUE, ((LSA_UINT32)0x1 << QueueIndex));
    }
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_CMD_FillCommandBlockAndParam          +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB        +*/
/*+                             EDDP_CMD_PARAM_PTR_TYPE         pParam      +*/
/*+                                                                         +*/
/*+  Result                :    EDDP_CMD_RSP                                +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB                   : Pointer to DeviceDescriptionBlock       (in)  +*/
/*+  pParam                 : Pointer to command parameter structure  (in)  +*/
/*+                                                                         +*/
/*+  Results                :   EDD_STS_OK            (synchronous)         +*/
/*+                             EDD_STS_OK_PENDING    (asynchronous)        +*/
/*+                             EDD_STS_OK_CANCEL     (shutdown)            +*/
/*+                             EDD_STS_ERR_PARAM     (wrong CmdOpcode)     +*/
/*+                             EDD_STS_ERR_SEQUENCE  (cmd already running) +*/
/*+                             EDD_STS_ERR_RESOURCE  (error not expected!) +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function invokes a command at the desired command IF +*/
/*+               using Command Control. If a callback fct is specfied in   +*/
/*+               pParam the command will be processed asynchronously. If   +*/
/*+               cbf is NULL the command will be processed synchronously.  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CmdRequest(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_CMD_PARAM_PTR_TYPE     pParam)
{
    EDD_RSP                         Response;
    LSA_UINT32                      cbl_address, cbl_offset;
    LSA_UINT32                      RequestEntry;
    LSA_UINT32                      QueueIndex;   
    LSA_UINT32                      CommandNumber;
    LSA_UINT32                      TimeoutTicks;
    LSA_UINT32                      SnapCount;
    EDD_COMMON_MEM_PTR_TYPE         RequestEntryHostAddr;
    EDDP_CMD_QUEUE_INFO_PTR_TYPE    pCurrent_Queue_Info;

    EDDP_ASSERT_NO_NULL_PTR(pParam);

    EDDP_CMD_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW,"[H:--] EDDP_CmdRequest(CmdOpcode=0x%X, Cbf=0x%X, InUse=0x%X)", 
        pParam->CmdOpcode, pParam->Cbf, pParam->InUse);

    // ***** check if we got a valid CmdOpcode
    Response        = _EDDP_CMD_CheckCmdOpcodeParams (pDDB, pParam->CmdOpcode);
    RequestEntry    = 0;

    if (Response != EDD_STS_OK)
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_CMD_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CmdRequest(): ERROR -> invalid CmdOpcode(0x%X) or invalid Params", pParam->CmdOpcode);
        return (Response);
    }

#if 1
    Response = EDDP_LL_CmdRequest(pDDB, pParam);
#if 0
    if (pParam->Cbf == 0)
    {
		// fill pParam with return value if there is one
		Response = _EDDP_CMD_ReadCommandReturnValue (pDDB, pParam);

		EDDP_CMD_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] EDDP_CmdRequest(CmdOpcode=0x%X, Response=0x%X): synchronous command has finished after %d ns",
			pParam->CmdOpcode, Response, SnapCount);

		/* free CRT command parameter element after synchronous command call */
		if( EDDP_CMD_CPM_NOP == (EDDP_CMD_GROUP_MASK & pParam->CmdOpcode) )
		{
			EDDP_CRTCmdReleaseInternalCmdParamElement(pDDB);
		}
    }
    else
    {

        /* -------------------------------------------- */
        /* asynchronous command processing              */
        /* -------------------------------------------- */
        QueueIndex          = ((pParam->CmdOpcode & EDDP_CMD_GROUP_MASK) >> 8);
        CommandNumber       = (pParam->CmdOpcode & EDDP_CMD_COMMAND_MASK);
        pCurrent_Queue_Info = &(pDDB->CMD.Queue[QueueIndex]);

        // remember calling interface for returning it later with CBF
        pParam->pDDB = pDDB;

        /* checks whether this command can be executed */
        if (!_EDDP_CMD_CheckpPermissionToRunCommandOnQueue(pDDB, QueueIndex, CommandNumber))
        {
            // command is already running
            EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CmdRequest(): ERROR -> SequenceError: command can not be executed!");
            Response = EDD_STS_ERR_SEQUENCE;
        }
        else
        {
            // Parameter structure is now used by command control and must not be modified by user until the CBF returns it
            pParam->InUse = LSA_TRUE;
            // queue pParam into DDB management structure
            EDDP_GlbQueueAddToEnd (&(pCurrent_Queue_Info->Queue_pParam), &(pParam->Queue));
            pCurrent_Queue_Info->CmdRunningCount++;

			// check if NewReqPtr is at the end of the queue
			if ( pCurrent_Queue_Info->pNewReqPtr == (pCurrent_Queue_Info->pQueueStartAddress + (pCurrent_Queue_Info->QueueLength - 1 )) )
			{
				// set NewReqPtr to next free element
				pCurrent_Queue_Info->pNewReqPtr = pCurrent_Queue_Info->pQueueStartAddress;
			}
			else
			{
				// set NewReqPtr to next free element
				pCurrent_Queue_Info->pNewReqPtr++;
			}

			// set running bit
			_EDDP_CMD_SetCmdRunningFlag(pDDB, QueueIndex, CommandNumber);

			Response = EDD_STS_OK_PENDING;

        }
    }
#endif

#else



    if (pParam->Cbf == 0)
    {
        /* -------------------------------------------- */
        /* synchronous command processing               */
        /* -------------------------------------------- */
        // init local addresses
        RequestEntryHostAddr    = EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_REQUEST_HOST_SYNC, 0);
        cbl_address             = (LSA_UINT32)_EDDP_CMD_FillCommandBlockAndParam (pDDB, pParam);
        cbl_address             = EDDP_HAL_H2P_ADDR(pDDB, cbl_address);
        // subtract base address of CMD-Ram to get offset within CMD-Ram address space
        /* PNIP_RAM_CMDCTRL_RAM_START was replaced by PNIP_R2P2_RAM_CMDCTRL_RAM_START */
        /* all macros of group PNIP_xxxx_RAM_CMDCTRL_RAM_START are checked in eddp_hal_pnip_val.h to have the same value*/
        EDDP_ASSERT_FALSE(cbl_address < PNIP_R2P2_RAM_CMDCTRL_RAM_START);
        cbl_offset              = cbl_address - PNIP_R2P2_RAM_CMDCTRL_RAM_START;

        // write RequestEntry into CMD-Ram
        // RequestEntry = RequestStatus(Waiting) | Interface(A) | Interrupt-Maske(0) | Request-ID(0) | RequestPtr(64Bit aligned address of CBL)
        RequestEntry = ((cbl_offset >> 3) | EDDP_CMD_REQUEST_WAITING);
        EDDP_HAL_MEM32_WRITE(pDDB, RequestEntryHostAddr, RequestEntry);

        // trigger queue processing
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_TRIGGERREQQUEUE, (0x1 << EDDP_CMD_INDEX_HOST_SYNC));

        // Timeout check of this synchronous command
        TimeoutTicks    = 10000;    // 10000 x 1�s = 10ms
        SnapCount       = EDDP_HALGetNsCounter (pDDB);

        do
        {
            // wait until command is done or timeout
            RequestEntry = EDDP_HAL_MEM32_READ(pDDB, RequestEntryHostAddr);
            if ((RequestEntry & EDDP_CMD_REQUEST_STATUS_MASK) == EDDP_CMD_REQUEST_CONFIRMED)
            {
                // command is finished
                break;
            }
            EDDP_HALWaitNsTicks (pDDB, EDDP_HAL_1US_TO);
            TimeoutTicks--;
        } while (TimeoutTicks > 0);

        // duration of synchronous command
        SnapCount = EDDP_HALDiffNsCounter (pDDB, SnapCount);

        // has Timeout reached  0 ?
        if (TimeoutTicks == 0)
        {
            // fire a FATAL_ERROR and stop here !!!
            Response = EDD_STS_ERR_TIMEOUT;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_CMD_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_CmdRequest(): FATAL -> Timeout after %d ns: RequestEntry=0x%X, RegQueueErrStatus=0x%X, RegQueueErrEvent=0x%X", 
                SnapCount, RequestEntry, EDDP_HAL_REG32_READ(pDDB, PNIP_REG_QUEUEERRSTATUS), EDDP_HAL_REG32_READ(pDDB, PNIP_REG_QUEUEERREVENT));
            EDDP_FATAL(EDDP_FATAL_ERR_CMD_TIMEOUT);
        }
        else
        {
            // fill pParam with return value if there is one
            Response = _EDDP_CMD_ReadCommandReturnValue (pDDB, pParam);
            EDDP_CMD_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] EDDP_CmdRequest(CmdOpcode=0x%X, Response=0x%X): synchronous command has finished after %d ns", 
                pParam->CmdOpcode, Response, SnapCount);
            
            /* free CRT command parameter element after synchronous command call */
            if( EDDP_CMD_CPM_NOP == (EDDP_CMD_GROUP_MASK & pParam->CmdOpcode) )
            {
                EDDP_CRTCmdReleaseInternalCmdParamElement(pDDB);
            }
        }
    }
    else
    {
        /* -------------------------------------------- */
        /* asynchronous command processing              */
        /* -------------------------------------------- */
        QueueIndex          = ((pParam->CmdOpcode & EDDP_CMD_GROUP_MASK) >> 8);
        CommandNumber       = (pParam->CmdOpcode & EDDP_CMD_COMMAND_MASK);
        pCurrent_Queue_Info = &(pDDB->CMD.Queue[QueueIndex]);

        // remember calling interface for returning it later with CBF
        pParam->pDDB = pDDB;
        
        /* checks whether this command can be executed */
        if (!_EDDP_CMD_CheckpPermissionToRunCommandOnQueue(pDDB, QueueIndex, CommandNumber))
        {
            // command is already running
            EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CmdRequest(): ERROR -> SequenceError: command can not be executed!");
            Response = EDD_STS_ERR_SEQUENCE;
        }
        else
        {   
            // Parameter structure is now used by command control and must not be modified by user until the CBF returns it
            pParam->InUse = LSA_TRUE;
            // queue pParam into DDB management structure
            EDDP_GlbQueueAddToEnd (&(pCurrent_Queue_Info->Queue_pParam), &(pParam->Queue));
            pCurrent_Queue_Info->CmdRunningCount++;
            
            if ( (EDDP_HAL_MEM32_READ(pDDB, pCurrent_Queue_Info->pNewReqPtr) & EDDP_CMD_REQUEST_STATUS_MASK) != EDDP_CMD_REQUEST_FREE )
            {
                // Request queue is full.
                // There are always enough resources available. Only a software/hardware bug can cause this error.
                Response = EDD_STS_ERR_RESOURCE;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CmdRequest(): ERROR -> Request queue is full! Unexpected Error! ");
            }
            else
            {
                // init local addresses
                cbl_address = (LSA_UINT32)_EDDP_CMD_FillCommandBlockAndParam (pDDB, pParam);
                cbl_address = EDDP_HAL_H2P_ADDR(pDDB, cbl_address);
                // subtract base address of CMD-Ram to get offset within CMD-Ram address space        
                /* PNIP_RAM_CMDCTRL_RAM_START was replaced by PNIP_R2P2_RAM_CMDCTRL_RAM_START */
                /* all macros of group PNIP_xxxx_RAM_CMDCTRL_RAM_START are checked in eddp_hal_pnip_val.h to have the same value*/
                EDDP_ASSERT_FALSE(cbl_address < PNIP_R2P2_RAM_CMDCTRL_RAM_START);
                cbl_offset  = cbl_address - PNIP_R2P2_RAM_CMDCTRL_RAM_START;

                // insert RequestEntry into corresponding CMD-Ram request queue
                // RequestEntry = RequestStatus(Waiting) | Interface(A) | Interrupt-Maske(IC2 or IC3) | Request-ID(0) | RequestPtr(64Bit aligned address of CBL)
                RequestEntry = ( (cbl_offset >> 3) | EDDP_CMD_REQUEST_WAITING | ((pDDB->EDDPLocation == EDDP_LOCATION_EXT_HOST) ? (0x1 << 28) : (0x1 << 27)) );
                EDDP_HAL_MEM32_WRITE(pDDB, pCurrent_Queue_Info->pNewReqPtr, RequestEntry);

                /* trigger command queue with queue index */
                _EDDP_CMD_TriggerQueue(pDDB, QueueIndex);

                // check if NewReqPtr is at the end of the queue
                if ( pCurrent_Queue_Info->pNewReqPtr == (pCurrent_Queue_Info->pQueueStartAddress + (pCurrent_Queue_Info->QueueLength - 1 )) )
                {
                    // set NewReqPtr to next free element
                    pCurrent_Queue_Info->pNewReqPtr = pCurrent_Queue_Info->pQueueStartAddress;
                }
                else
                {
                    // set NewReqPtr to next free element
                    pCurrent_Queue_Info->pNewReqPtr++;
                }    

                // set running bit
                _EDDP_CMD_SetCmdRunningFlag(pDDB, QueueIndex, CommandNumber); 

                Response = EDD_STS_OK_PENDING;
            }
        }
    }
#endif

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_CMD_CheckCmdOpcodeParams              +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32					   CmdOpCode    +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB                   : Pointer to DeviceDescriptionBlock       (in)  +*/
/*+  CmdOpCode              : Command OpCode                          (in)  +*/
/*+                                                                         +*/
/*+  Results                : EDD_STS_OK                                    +*/
/*+                           EDD_STS_ERR_PARAM                             +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function checks the given CmdOpcode/Params.          +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP _EDDP_CMD_CheckCmdOpcodeParams(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  CmdOpcode)
{
    EDD_MAC_ADR_TYPE    *pMac;
    EDD_RSP             Response = _EDDP_CMD_IsCmdOpcodeValid(CmdOpcode);

    if (Response == EDD_STS_OK)
    {
        if (CmdOpcode == EDDP_CMD_FDB_INSERT_UC_ENTRY)
        {   // ***** special test here for a valid UC address
            pMac = &pDDB->SWI.CommandParams.InsertUC_Entry.param_union.Fdb.InsertUCEntry.UC_MAC_Address;

            if (pMac->MacAdr[0] & 1)
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_SWI_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_CheckCmdOpcodeParams(): ERROR -> invalid UC address(%02X:%02X:%02X:%02X:%02X:%02X)", 
                    pMac->MacAdr[0], pMac->MacAdr[1], pMac->MacAdr[2], pMac->MacAdr[3], pMac->MacAdr[4], pMac->MacAdr[5]);
            }
        }
    }

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_CMD_IsCmdOpcodeValid                  +*/
/*+  Input/Output          :    LSA_UINT32					   CmdOpCode    +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  CmdOpCode  : Command OpCode                                      (in)  +*/
/*+                                                                         +*/
/*+  Results                : EDD_STS_OK                                    +*/
/*+                           EDD_STS_ERR_PARAM                             +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function checks if the given CmdOpcode is valid.     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP _EDDP_CMD_IsCmdOpcodeValid(
    LSA_UINT32      CmdOpcode)
{
    EDD_RSP     Response;

    switch (CmdOpcode)
    {
        // ApiFilter
        case EDDP_CMD_FILTER_NOP            :
        case EDDP_CMD_FILTER_STOP           :
        case EDDP_CMD_FILTER_CHANGE_BASE    :
        case EDDP_CMD_FILTER_ENABLE_STRING  :
        case EDDP_CMD_FILTER_CHANGE_STRING  :

        // RcvDMACW 
        case EDDP_CMD_ARCV_NOP              :
        case EDDP_CMD_ARCV_DISABLE          :
        case EDDP_CMD_ARCV_DISABLE_DSCR     :
        case EDDP_CMD_ARCV_COPY_DSCR        :	

        // SndDMACW 
        case EDDP_CMD_ASND_NOP              :
        case EDDP_CMD_ASND_DISABLE          :
        case EDDP_CMD_ASND_DISABLE_DSCR     :	

        // CPM / AcwConsumer
        case EDDP_CMD_CPM_NOP               :
        case EDDP_CMD_CPM_ON_OFF            :
        case EDDP_CMD_CPM_ACW_ON_OFF        :
        case EDDP_CMD_CPM_ACW_IN_OUT        :
        case EDDP_CMD_CPM_ACW_SET_UNKNOWN   :
        case EDDP_CMD_CPM_ACW_XCHANGE_BUFF  :

        // PPM / AcwProvider
        case EDDP_CMD_PPM_NOP               :
        case EDDP_CMD_PPM_ON_OFF            :
        case EDDP_CMD_PPM_ACW_ON_OFF        :
        case EDDP_CMD_PPM_ACW_IN_OUT        :
        case EDDP_CMD_PPM_ACW_MOVE          :
        case EDDP_CMD_PPM_ACW_XCHANGE_BUFF  :
        case EDDP_CMD_PPM_ACW_DELAY         :

        // Pack
        case EDDP_CMD_PACK_NOP              :
        case EDDP_CMD_PACK_DISABLE_GRP      :
        case EDDP_CMD_PACK_ENABLE_GRP       :
        case EDDP_CMD_PACK_DIS_ENA_GRP      :	

        // FDB
        case EDDP_CMD_FDB_NOP               :
        case EDDP_CMD_FDB_CLEAR_UC_TABLE    :
        case EDDP_CMD_FDB_CLEAR_UC_ENTRY    :
        case EDDP_CMD_FDB_INSERT_UC_ENTRY   :
        case EDDP_CMD_FDB_SEARCH_MAC        :
        case EDDP_CMD_FDB_CHANGE_MC_ENTRY   :
        case EDDP_CMD_FDB_CLEAR_UC_PORTS    :
        case EDDP_CMD_FDB_PTCP_CHANGE       :

        // VLAN
        case EDDP_CMD_VLAN_NOP              :
        case EDDP_CMD_VLAN_CLEAR            :
        case EDDP_CMD_VLAN_INSERT           :
        case EDDP_CMD_VLAN_SEARCH           :	

        // SYNC
        case EDDP_CMD_GSYNC_NOP             :
        case EDDP_CMD_GSYNC_UPDATE_SYNC_A   :
        case EDDP_CMD_GSYNC_UPDATE_SYNC_B   :
        case EDDP_CMD_GSYNC_UPDATE_TIME     :
        case EDDP_CMD_GSYNC_UPDATE_APPCYCL  :
        case EDDP_CMD_GSYNC_UPDATE_COMCYCL  :
        case EDDP_CMD_GSYNC_UPDATE_BASECYCL :
        case EDDP_CMD_GSYNC_UPDATE_SYNCCYCL :
        case EDDP_CMD_GSYNC_UPDATE_SYNCTIME :
        case EDDP_CMD_GSYNC_STAMP           :
        case EDDP_CMD_GSYNC_CONNECT_FREE    :
        case EDDP_CMD_GSYNC_CONNECT_SYNC_A  :
        case EDDP_CMD_GSYNC_CONNECT_SYNC_B  :
            // valid CmdOpcode
            Response = EDD_STS_OK;
        break;

        default:
            // no valid CmdOpcode found
            Response = EDD_STS_ERR_PARAM;
        break;
    }

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_CMD_Setup_QueueMgmtTable              +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE           pDDB      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function writes all queue management entries for both+*/
/*+               asynchronous and synchronous queues which belong to       +*/
/*+               HOST and USER.                                            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID _EDDP_CMD_Setup_QueueMgmtTable(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT32          queue_start_address;

    // --------  HOST ASYNC ------------- 
    // get start address of reserved RAM for the queues
    queue_start_address = (LSA_UINT32) EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_REQUEST_HOST_ASYNC, 0);
    queue_start_address = EDDP_HAL_H2P_ADDR(pDDB, queue_start_address);

    // set up host FILTER queue
    _EDDP_CMD_Write_QueueMgmt_Entry (pDDB, EDDP_CMD_INDEX_HOST_ASYNC_FILTER, queue_start_address, EDDP_CMD_FILTER_COUNT);

    // set up host ARCV queue
    queue_start_address = queue_start_address + (EDDP_CMD_FILTER_COUNT * 4);
    _EDDP_CMD_Write_QueueMgmt_Entry (pDDB, EDDP_CMD_INDEX_HOST_ASYNC_ARCV, queue_start_address, EDDP_CMD_ARCV_COUNT);

    // set up host ASND queue
    queue_start_address = queue_start_address + (EDDP_CMD_ARCV_COUNT * 4);
    _EDDP_CMD_Write_QueueMgmt_Entry (pDDB, EDDP_CMD_INDEX_HOST_ASYNC_ASND, queue_start_address, EDDP_CMD_ASND_COUNT);

    // set up host CRT queue
    queue_start_address = queue_start_address + (EDDP_CMD_ASND_COUNT * 4);
    _EDDP_CMD_Write_QueueMgmt_Entry (pDDB, EDDP_CMD_INDEX_HOST_ASYNC_CRT, queue_start_address, pDDB->CMD.CrtCmdDepthAlloc);

    // set up host PACK queue
    queue_start_address = queue_start_address + (pDDB->CMD.CrtCmdDepthAlloc * 4);
    _EDDP_CMD_Write_QueueMgmt_Entry (pDDB, EDDP_CMD_INDEX_HOST_ASYNC_PACK, queue_start_address, EDDP_CMD_PACK_COUNT);

    // set up host FDB queue
    queue_start_address = queue_start_address + (EDDP_CMD_PACK_COUNT * 4);
    _EDDP_CMD_Write_QueueMgmt_Entry (pDDB, EDDP_CMD_INDEX_HOST_ASYNC_FDB, queue_start_address, EDDP_CMD_FDB_COUNT);

    // set up host VLAN queue
    queue_start_address = queue_start_address + (EDDP_CMD_FDB_COUNT * 4);
    _EDDP_CMD_Write_QueueMgmt_Entry (pDDB, EDDP_CMD_INDEX_HOST_ASYNC_VLAN, queue_start_address, EDDP_CMD_VLAN_COUNT);

    // set up host GSYNC queue
    queue_start_address = queue_start_address + (EDDP_CMD_VLAN_COUNT * 4);
    _EDDP_CMD_Write_QueueMgmt_Entry (pDDB, EDDP_CMD_INDEX_HOST_ASYNC_GSYNC, queue_start_address, EDDP_CMD_GSYNC_COUNT);

    // --------  HOST SYNC ------------- 
    // get start address of reserved RAM for the queues
    queue_start_address = (LSA_UINT32)EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_REQUEST_HOST_SYNC, 0);
    queue_start_address = EDDP_HAL_H2P_ADDR(pDDB, queue_start_address);

    // set up host SYNC queue
    _EDDP_CMD_Write_QueueMgmt_Entry (pDDB, EDDP_CMD_INDEX_HOST_SYNC, queue_start_address, sizeof(EDDP_CMD_RAM_REQUEST_HOST_SYNC_TYPE)/4 /* = 1 */);

    // --------  USER ASYNC ------------- 
    // get start address of reserved RAM for the queues
    queue_start_address = (LSA_UINT32)EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_REQUEST_USER_ASYNC, 0);
    queue_start_address = EDDP_HAL_H2P_ADDR(pDDB, queue_start_address);

    // set up user FDB queue
    _EDDP_CMD_Write_QueueMgmt_Entry (pDDB, EDDP_CMD_INDEX_USER_ASYNC_FDB, queue_start_address, EDDP_CMD_FDB_COUNT);

    // set up user GSYNC queue
    queue_start_address = queue_start_address + (EDDP_CMD_FDB_COUNT * 4);
    _EDDP_CMD_Write_QueueMgmt_Entry (pDDB, EDDP_CMD_INDEX_USER_ASYNC_GSYNC, queue_start_address, EDDP_CMD_GSYNC_COUNT);

    // --------  USER SYNC ------------- 
    // get start address of reserved RAM for the queues
    queue_start_address = (LSA_UINT32)EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_REQUEST_USER_SYNC, 0);
    queue_start_address = EDDP_HAL_H2P_ADDR(pDDB, queue_start_address);

    // set up user SYNC queue
    _EDDP_CMD_Write_QueueMgmt_Entry (pDDB, EDDP_CMD_INDEX_USER_SYNC, queue_start_address, sizeof(EDDP_CMD_RAM_REQUEST_USER_SYNC_TYPE)/4 /* = 1 */);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_CMD_Write_QueueMgmt_Entry             +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB        +*/
/*+                             LSA_UINT32                      QueueIndex  +*/
/*+                             LSA_UINT32              queue_start_address +*/
/*+                             LSA_UINT32                     queue_length +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB                   : Pointer to DeviceDescriptionBlock       (in)  +*/
/*+  QueueIndex             : Queue index 0..n                        (in)  +*/
/*+  queue_start_address    : PNIP start address of this queue        (in)  +*/
/*+  queue_length           : Queue length (count of request entries) (in)  +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function writes one queue management entry           +*/
/*+               using the given parameters. It also stores queue          +*/
/*+               information in the DDB structure.                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID _EDDP_CMD_Write_QueueMgmt_Entry(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  QueueIndex,
    LSA_UINT32                  queue_start_address,
    LSA_UINT32                  queue_length)
{
    LSA_UINT32                  queue_start_offset;
    EDD_COMMON_MEM_PTR_TYPE     queue_mgmt_address;

    EDDP_ASSERT(QueueIndex < EDDP_CMD_QUEUE_MGMT_SIZE_8BYTE);

    // get base address of QueueMgmtTable
    queue_mgmt_address = EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_QUEUE_MGMT, 0);

    // subtract base address of CMD-Ram to get offset within CMD-Ram address space
    /* PNIP_RAM_CMDCTRL_RAM_START was replaced by PNIP_R2P2_RAM_CMDCTRL_RAM_START */
    /* all macros of group PNIP_xxxx_RAM_CMDCTRL_RAM_START are checked in eddp_hal_pnip_val.h to have the same value*/
    queue_start_offset = queue_start_address - PNIP_R2P2_RAM_CMDCTRL_RAM_START;

    // write queue management entry to CMD-Ram
    EDDP_HAL_MEM64_WRITE(pDDB, &((EDDP_CMD_RAM_QUEUE_MGMT_PTR_TYPE)queue_mgmt_address)->QueueMGMT[QueueIndex], (LSA_UINT64) 
                                                                                           ( ((LSA_UINT64)queue_start_offset  >> 2  ) | 
                                                                                             ((LSA_UINT64)queue_length        << 16 ) | 
                                                                                             ((LSA_UINT64)queue_start_offset  << 30 )   ));
    // set up DDB management structure
    pDDB->CMD.Queue[QueueIndex].pCurrentConfPtr     = (LSA_UINT32*)EDDP_HAL_P2H_ADDR(pDDB, queue_start_address);
    pDDB->CMD.Queue[QueueIndex].pNewReqPtr          = (LSA_UINT32*)EDDP_HAL_P2H_ADDR(pDDB, queue_start_address);
    pDDB->CMD.Queue[QueueIndex].pQueueStartAddress  = (LSA_UINT32*)EDDP_HAL_P2H_ADDR(pDDB, queue_start_address);
    pDDB->CMD.Queue[QueueIndex].QueueLength         = queue_length;
    pDDB->CMD.Queue[QueueIndex].CmdRunningCount     = 0;
    pDDB->CMD.Queue[QueueIndex].CmdRunSeqCount      = 0;
    pDDB->CMD.Queue[QueueIndex].CmdReadyToBeAcknCount = 0;

    #ifdef EDDP_CFG_USR_CMD_API_SUPPORT
    // only for UserCmd-Info-Block of CmdQueue[USER_SYNC]
    if (QueueIndex == EDDP_CMD_INDEX_USER_SYNC)
    {
        LSA_UINT32      UserCmdInfoBlockAddr, RequestEntryAddr, CblAddr, ParamAddr;

        // check size of UserCmd-Info-Block
        EDDP_ASSERT_FALSE((EDDP_CMD_USER_CMD_INFO_BLOCK_SIZE != sizeof(EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_TYPE)));

        // get address of UserCmd-Info-Block in host format
        UserCmdInfoBlockAddr = (LSA_UINT32) EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_USER_CMD_INFO_BLOCK, 0);

        // RequestEntryAddr: offset within CMD-Ram
        RequestEntryAddr    = (LSA_UINT32) EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_REQUEST_USER_SYNC, 0);
        RequestEntryAddr    = (EDDP_HAL_H2P_ADDR(pDDB, RequestEntryAddr));
        // CblAddr: offset within CMD-Ram
        CblAddr             = (LSA_UINT32) EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_COMMAND_USER_SYNC, 0);
        CblAddr             = (EDDP_HAL_H2P_ADDR(pDDB, CblAddr));
        // ParamAddr: offset within CMD-Ram
        ParamAddr           = (LSA_UINT32) EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_PARAM_USER_SYNC, 0);
        ParamAddr           = (EDDP_HAL_H2P_ADDR(pDDB, ParamAddr));

        // write data to UserCmd-Info-Block into CMD-Ram
        EDDP_HAL_MEM32_WRITE(pDDB, &((EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE)UserCmdInfoBlockAddr)->CheckBytes, EDDP_CMD_USER_CMD_INFO_BLOCK_CHECK_BYTES);
        EDDP_HAL_MEM32_WRITE(pDDB, &((EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE)UserCmdInfoBlockAddr)->CurrentConfPtrCmdRamAddr, queue_start_address);
        EDDP_HAL_MEM32_WRITE(pDDB, &((EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE)UserCmdInfoBlockAddr)->NewReqPtrCmdRamAddr, queue_start_address);
        EDDP_HAL_MEM32_WRITE(pDDB, &((EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE)UserCmdInfoBlockAddr)->QueueStartCmdRamAddr, queue_start_address);
        EDDP_HAL_MEM32_WRITE(pDDB, &((EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE)UserCmdInfoBlockAddr)->RequestEntryCmdRamAddr, RequestEntryAddr);
        EDDP_HAL_MEM32_WRITE(pDDB, &((EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE)UserCmdInfoBlockAddr)->CblCmdRamAddr, CblAddr);
        EDDP_HAL_MEM32_WRITE(pDDB, &((EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE)UserCmdInfoBlockAddr)->ParamCmdRamAddr, ParamAddr);

        // trace addresses
        EDDP_CMD_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] _EDDP_CMD_Write_QueueMgmt_Entry(QueueIndex=%d, QueueStartAddr=0x%X, QueueLen=%d): RequestEntryAddr=0x%X, CblAddr=0x%X, ParamAddr=0x%X", 
            QueueIndex, queue_start_address, queue_length, RequestEntryAddr, CblAddr, ParamAddr);
    }
    #endif
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_CMD_Delete_QueueMgmt_Entry            +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_UINT32                  QueueIndex      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock             (in)    +*/
/*+  QueueIndex     : Queue index 0..n                              (in)    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function deletes one queue management entry          +*/
/*+               using the given parameters.                               +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID _EDDP_CMD_Delete_QueueMgmt_Entry(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  QueueIndex)
{
    EDDP_ASSERT(QueueIndex < EDDP_CMD_QUEUE_MGMT_SIZE_8BYTE);

    // delete DDB management structure
    pDDB->CMD.Queue[QueueIndex].pCurrentConfPtr     = 0;
    pDDB->CMD.Queue[QueueIndex].pNewReqPtr          = 0;
    pDDB->CMD.Queue[QueueIndex].pQueueStartAddress  = 0;
    pDDB->CMD.Queue[QueueIndex].QueueLength         = 0;
    pDDB->CMD.Queue[QueueIndex].CmdRunningCount     = 0;
    pDDB->CMD.Queue[QueueIndex].CmdRunSeqCount      = 0;
    pDDB->CMD.Queue[QueueIndex].CmdReadyToBeAcknCount = 0;

    #ifdef EDDP_CFG_USR_CMD_API_SUPPORT
    // only for UserCmd-Info-Block of CmdQueue[USER_SYNC]
    if (QueueIndex == EDDP_CMD_INDEX_USER_SYNC)
    {
        LSA_UINT32      UserCmdInfoBlockAddr;

        // get address of UserCmd-Info-Block in host format
        UserCmdInfoBlockAddr = (LSA_UINT32) EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_USER_CMD_INFO_BLOCK, 0);

        // delete data in UserCmd-Info-Block in CMD-Ram
        EDDP_HAL_MEM32_WRITE(pDDB, &((EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE)UserCmdInfoBlockAddr)->CheckBytes, 0);
        EDDP_HAL_MEM32_WRITE(pDDB, &((EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE)UserCmdInfoBlockAddr)->CurrentConfPtrCmdRamAddr, 0);
        EDDP_HAL_MEM32_WRITE(pDDB, &((EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE)UserCmdInfoBlockAddr)->NewReqPtrCmdRamAddr, 0);
        EDDP_HAL_MEM32_WRITE(pDDB, &((EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE)UserCmdInfoBlockAddr)->QueueStartCmdRamAddr, 0);
        EDDP_HAL_MEM32_WRITE(pDDB, &((EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE)UserCmdInfoBlockAddr)->RequestEntryCmdRamAddr, 0);
        EDDP_HAL_MEM32_WRITE(pDDB, &((EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE)UserCmdInfoBlockAddr)->CblCmdRamAddr, 0);
        EDDP_HAL_MEM32_WRITE(pDDB, &((EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE)UserCmdInfoBlockAddr)->ParamCmdRamAddr, 0);
    }
    #endif
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_CMD_Delete_QueueMgmtTable             +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE           pDDB      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function deletes the queue management entries for    +*/
/*+               asynchronous and synchronous queues which belong to       +*/
/*+               HOST and USER.                                            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID _EDDP_CMD_Delete_QueueMgmtTable(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    // --------  USER SYNC ------------- 
    // delete user SYNC queue
    _EDDP_CMD_Delete_QueueMgmt_Entry (pDDB, EDDP_CMD_INDEX_USER_SYNC);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_CMD_FillCommandBlockAndParam          +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB        +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE         pDDB        +*/
/*+                             EDDP_CMD_PARAM_PTR_TYPE         pParam      +*/
/*+                                                                         +*/
/*+  Result                :    EDD_COMMON_MEM_PTR_TYPE                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB                   : Pointer to DeviceDescriptionBlock       (in)  +*/
/*+  pParam                 : Pointer to command parameter structure  (in)  +*/
/*+                                                                         +*/
/*+  Results                : Pointer to current command block (host view) +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function fills the commmand block and corresponding  +*/
/*+               parameter for the current command. The pointer to the     +*/
/*+               command block is returned (host view address).            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_COMMON_MEM_PTR_TYPE _EDDP_CMD_FillCommandBlockAndParam(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_CMD_PARAM_PTR_TYPE     pParam)
{
    LSA_UINT32                  param_address_pnip;
    LSA_UINT32                  target_param_address;
    LSA_UINT32                  target_param_address_offset;
    LSA_UINT32                  register_value;
    LSA_UINT32                  user_id;
    LSA_UINT32                  i;
    LSA_BOOL                    isAsync;
    EDD_COMMON_MEM_PTR_TYPE     cbl_address;
    EDD_COMMON_MEM_PTR_TYPE     param_address;
    EDDP_CMD_COMMAND_TYPE*      pCommandBlock;
    EDDP_CMD_CBL_PARAM_TYPE*    pCommandParamBlock;

    register_value = 0;
    isAsync = LSA_TRUE;
    pCommandBlock = LSA_NULL;
    pCommandParamBlock = LSA_NULL;
    cbl_address = LSA_NULL;
    param_address = LSA_NULL;

    if (pParam->Cbf == 0) // get addresses for synchronous command processing
    {   
        cbl_address        = EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_COMMAND_HOST_SYNC, 0);
        pCommandBlock      = (EDDP_CMD_COMMAND_TYPE*)&(((EDDP_CMD_RAM_COMMAND_BLOCK_HOST_SYNC_TYPE*)cbl_address)->Generic.Generic[0]);
        param_address      = EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_PARAM_HOST_SYNC, 0);
        pCommandParamBlock = (EDDP_CMD_CBL_PARAM_TYPE*)&(((EDDP_CMD_RAM_PARAM_HOST_SYNC_TYPE*)param_address)->Param[0]);
        user_id            = EDDP_CMD_USER_ID_HOST_SYNC;     // indicates synchronous command processing on Host/ARM
        
        isAsync = LSA_FALSE;
    }
    else // asynchronous command processing
    {
        user_id = EDDP_CMD_USER_ID_HOST_ASYNC;  // indicates asynchronous command processing on Host/ARM
    }
    
    // fill command block (CBL) and parameters for the selected command
    switch (pParam->CmdOpcode)
    {
        /* ------------------------------------------------ */
        /* FILTER                                           */
        /* ------------------------------------------------ */
        case EDDP_CMD_FILTER_STOP:
        {
            if(isAsync)
            {
                //get offset address of CBLs belonging to this queue
                cbl_address = EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_COMMAND_HOST_ASYNC, 0);
                //get offset address of parameters belonging to this queue
                param_address = EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_PARAM_HOST_ASYNC, 0);
                pCommandBlock = &(((EDDP_CMD_RAM_COMMAND_BLOCKS_HOST_ASYNC_PTR_TYPE)cbl_address)->Filter_Stop);
                cbl_address   = (EDD_COMMON_MEM_PTR_TYPE)pCommandBlock;
            }
            
            register_value = (((LSA_UINT32)pParam->param_union.Filter.StopFilter.StopFilterMask) << EDDP_CMD_API_SHIFT_STOPFILTERMASK  | /* command parameter */
                              (user_id << PNIP_REG_APIFILTERCOMMAND_IF_CONTROL_SHFT_USER_ID) | /* select User_ID */
                              (PNIP_REG_APIFILTERCOMMAND_IF_CONTROL__VAL_CONFREQUEST_ENABLE << PNIP_REG_APIFILTERCOMMAND_IF_CONTROL_SHFT_CONFREQUEST) | /* Enable ConfRequest */
                              (PNIP_REG_APIFILTERCOMMAND_IF_CONTROL__VAL_IF_SELECT_IFA) << PNIP_REG_APIFILTERCOMMAND_IF_CONTROL_SHFT_IF_SELECT |     /*  IFA  */
                              (PNIP_REG_APIFILTERCOMMAND_IF_CONTROL__VAL_F_CODE_STOP_FILTER << PNIP_REG_APIFILTERCOMMAND_IF_CONTROL_SHFT_F_CODE));   /* F_Code: Stop_Filer */
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].Low_32Bit), register_value);

            EDDP_SET_CBL_HIGH_32_WORD_WCRC(register_value, PNIP_REG_APIFILTERCOMMAND_IF_CONTROL, LSA_TRUE);
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].High_32Bit), register_value);
        }
        break;

        /* ------------------------------------------------ */
        /* RcvDMACW                                         */
        /* ------------------------------------------------ */

        /* ------------------------------------------------ */
        /* SndDMACW                                         */
        /* ------------------------------------------------ */

        /* ------------------------------------------------ */
        /* CPM                                              */
        /* ------------------------------------------------ */
        case EDDP_CMD_CPM_NOP:
        {
            if(isAsync)
            {                
                pCommandBlock = pParam->param_union.Crt.pCBL;
                cbl_address   = (EDD_COMMON_MEM_PTR_TYPE)pCommandBlock;
            }
            
            register_value = 0x40000000; /* NOP */
            EDDP_HAL_MEM32_WRITE(pDDB,  &(pCommandBlock[0].High_32Bit), register_value);
        }
        break;

        case EDDP_CMD_CPM_ON_OFF:
        {
            // ***** ON:  set CPM_Enable in PNIP_REG_CPM_CONTROL_IF?
            // ***** OFF: clr CPM_Enable in PNIP_REG_CPM_CONTROL_IF?
            if(isAsync)
            {
                pCommandBlock = pParam->param_union.Crt.pCBL;
                cbl_address   = (EDD_COMMON_MEM_PTR_TYPE)pCommandBlock;
            }
            
            register_value = ((pParam->param_union.Crt.Cpm.OnOff.CpmOn ? EDDP_CMD_CPM_CPM_ACTIVATE : EDDP_CMD_CPM_CPM_DEACTIVATE) << PNIP_REG_CPMCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE |
                              (user_id << PNIP_REG_CPMCOMMAND_IF_CONTROL_SHFT_USER_ID) |
                              (PNIP_REG_CPMCOMMAND_IF_CONTROL__VAL_CONFREQUEST_ENABLE << PNIP_REG_CPMCOMMAND_IF_CONTROL_SHFT_CONFREQUEST) |
                              (PNIP_REG_CPMCOMMAND_IF_CONTROL__VAL_IF_SELECT_IFA) << PNIP_REG_CPMCOMMAND_IF_CONTROL_SHFT_IF_SELECT |
                              (PNIP_REG_CPMCOMMAND_IF_CONTROL__VAL_F_CODE_CPM_ON_OFF << PNIP_REG_CPMCOMMAND_IF_CONTROL_SHFT_F_CODE));
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].Low_32Bit), register_value);

            EDDP_SET_CBL_HIGH_32_WORD_WCRC(register_value, PNIP_REG_CPMCOMMAND_IF_CONTROL, LSA_TRUE);
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].High_32Bit), register_value);            
        }
        break;

        case EDDP_CMD_CPM_ACW_ON_OFF:
        {
            // ***** ON:  set ActiveBit, OneShotBit in ACW, set Valid, WDT, MRT, DHT, Again, Frame_received in TSB
            // ***** OFF: clr ActiveBit in ACW, clr DHT, Valid in TSB
            if(isAsync)
            {
                pCommandBlock = pParam->param_union.Crt.pCBL;
                cbl_address   = (EDD_COMMON_MEM_PTR_TYPE)pCommandBlock;
            }
            
            register_value = ((pParam->param_union.Crt.Cpm.ACW_OnOff.Acw_On ? EDDP_CMD_CPM_ACW_ACTIVATE : EDDP_CMD_CPM_ACW_DEACTIVATE) << PNIP_REG_CPMCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE |
                              (pParam->param_union.Crt.Cpm.ACW_OnOff.Acw_DG ? EDDP_CMD_CPM_IS_ACW_DG : EDDP_CMD_CPM_IS_ACW) << PNIP_REG_CPMCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE |
                              (pParam->param_union.Crt.Cpm.ACW_OnOff.ApiAdr_Acw << EDDP_CMD_CPM_SHIFT_ACW_ADDRESS) << PNIP_REG_CPMCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE |
                              (user_id << PNIP_REG_CPMCOMMAND_IF_CONTROL_SHFT_USER_ID) |
                              (PNIP_REG_CPMCOMMAND_IF_CONTROL__VAL_CONFREQUEST_ENABLE << PNIP_REG_CPMCOMMAND_IF_CONTROL_SHFT_CONFREQUEST) |
                              (PNIP_REG_CPMCOMMAND_IF_CONTROL__VAL_F_CODE_ACW_ON_OFF << PNIP_REG_CPMCOMMAND_IF_CONTROL_SHFT_F_CODE));
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].Low_32Bit), register_value);

            EDDP_SET_CBL_HIGH_32_WORD_WCRC(register_value, PNIP_REG_CPMCOMMAND_IF_CONTROL, LSA_TRUE);
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].High_32Bit), register_value);               
        }
        break;

        case EDDP_CMD_CPM_ACW_IN_OUT:
        {
            // ***** IN:  set link in prev. ACW (update ACW_netxoffset to new inserted ACW)
            // ***** OUT: update link in prev. ACW to position of ACW after the ACW that will be cut out
            if(isAsync)
            {
                pCommandBlock = pParam->param_union.Crt.pCBL;
                cbl_address   = (EDD_COMMON_MEM_PTR_TYPE)pCommandBlock;
            }
            
            // configure command for APICtrl-Ram
            target_param_address        = (LSA_UINT32)pParam->param_union.Crt.pExternalCmdParam_ApiCtrlRam;
            target_param_address_offset = EDDP_HALGetPnipRAMOffsetToRAM (pDDB, EDDP_HAL_RAMTYPE_API_CTRL, (EDD_COMMON_MEM_PTR_TYPE)target_param_address);
            
            // write first 32bit 'Parameterblock' APICtrl-Ram
            register_value = ((pParam->param_union.Crt.Cpm.ACW_InOut.Word0.Acw_In ? EDDP_CMD_CPM_PARAM_ACW_IN : EDDP_CMD_CPM_PARAM_ACW_OUT) |
                              (pParam->param_union.Crt.Cpm.ACW_InOut.Word0.Acw_DG ? EDDP_CMD_CPM_PARAM_IS_ACW_DG : EDDP_CMD_CPM_PARAM_IS_ACW) |
                              (pParam->param_union.Crt.Cpm.ACW_InOut.Word0.ApiAdr_NextPointer_Prev << EDDP_CMD_CPM_SHIFT_PARAM_NEXT_POINTER));
            EDDP_HAL_MEM32_WRITE(pDDB, (LSA_UINT32*)target_param_address, register_value);
            // write second 32bit 'Parameterblock' APICtrl-Ram
            register_value = pParam->param_union.Crt.Cpm.ACW_InOut.Word1.ApiAdr_Acw_New;
            EDDP_HAL_MEM32_WRITE(pDDB, (LSA_UINT32*)target_param_address + 1, register_value);

            // configure "ACW_in_out" command
            // write first 32bit of WCRC command
            register_value = ((target_param_address_offset << EDDP_CMD_CPM_SHIFT_PARAMETERBLOCK_ADDR) |
                              (user_id << PNIP_REG_CPMCOMMAND_IF_CONTROL_SHFT_USER_ID) |
                              (PNIP_REG_CPMCOMMAND_IF_CONTROL__VAL_CONFREQUEST_ENABLE << PNIP_REG_CPMCOMMAND_IF_CONTROL_SHFT_CONFREQUEST) | 
                              (PNIP_REG_CPMCOMMAND_IF_CONTROL__VAL_F_CODE_ACW_IN_OUT << PNIP_REG_CPMCOMMAND_IF_CONTROL_SHFT_F_CODE));                                
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].Low_32Bit), register_value);
            // write second 32bit of WCRC command
            EDDP_SET_CBL_HIGH_32_WORD_WCRC(register_value, PNIP_REG_CPMCOMMAND_IF_CONTROL, LSA_TRUE);
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].High_32Bit), register_value);
        }
        break;

        case EDDP_CMD_CPM_ACW_SET_UNKNOWN:
        {
            // ***** WDT-, MRT- und DHT-Reload, clr AGAIN + Frame_received
            if(isAsync)
            {
                pCommandBlock = pParam->param_union.Crt.pCBL;
                cbl_address   = (EDD_COMMON_MEM_PTR_TYPE)pCommandBlock;
            }
            
            register_value = (((pParam->param_union.Crt.Cpm.SetUnknown.Acw_DG ? EDDP_CMD_CPM_IS_ACW_DG : EDDP_CMD_CPM_IS_ACW) << PNIP_REG_CPMCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE) |
                              ((pParam->param_union.Crt.Cpm.SetUnknown.ApiAdr_Acw << EDDP_CMD_CPM_SHIFT_ACW_ADDRESS) << PNIP_REG_CPMCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE) |
                              (user_id << PNIP_REG_CPMCOMMAND_IF_CONTROL_SHFT_USER_ID) |
                              (PNIP_REG_CPMCOMMAND_IF_CONTROL__VAL_CONFREQUEST_ENABLE << PNIP_REG_CPMCOMMAND_IF_CONTROL_SHFT_CONFREQUEST) |
                              (PNIP_REG_CPMCOMMAND_IF_CONTROL__VAL_F_CODE_ACW_SETTOUNKNOWN << PNIP_REG_CPMCOMMAND_IF_CONTROL_SHFT_F_CODE));
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].Low_32Bit), register_value);

            EDDP_SET_CBL_HIGH_32_WORD_WCRC(register_value, PNIP_REG_CPMCOMMAND_IF_CONTROL, LSA_TRUE);
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].High_32Bit), register_value);   
        }
        break;

        case EDDP_CMD_CPM_ACW_XCHANGE_BUFF:
        {
            if(isAsync)
            {
                pCommandBlock = pParam->param_union.Crt.pCBL;
                cbl_address   = (EDD_COMMON_MEM_PTR_TYPE)pCommandBlock;
            }
            
            // ***** exchange buffers and check that old buffer is free
            register_value = 0x40000000; /* NOP */
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].High_32Bit), register_value);
        }
        break;

        /* ------------------------------------------------ */
        /* PPM                                              */
        /* ------------------------------------------------ */
        case EDDP_CMD_PPM_NOP:
        {
            if(isAsync)
            {
                pCommandBlock = pParam->param_union.Crt.pCBL;
                cbl_address   = (EDD_COMMON_MEM_PTR_TYPE)pCommandBlock;
            }
            
            register_value = 0x40000000; /* NOP */
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].High_32Bit), register_value);
        }
        break;

        case EDDP_CMD_PPM_ON_OFF:
        {
            if(isAsync)
            {
                pCommandBlock = pParam->param_union.Crt.pCBL;
                cbl_address   = (EDD_COMMON_MEM_PTR_TYPE)pCommandBlock;
            }
            
            /* There is a macro that checks 
            PNIP_R2_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE and PNIP_R3_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE
            PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_USER_ID and PNIP_R3_REG_PPMCOMMAND_IF_CONTROL_SHFT_USER_ID
            PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_CONFREQUEST and PNIP_R3_REG_PPMCOMMAND_IF_CONTROL_SHFT_CONFREQUEST
            PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_IF_SELECT and PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_IF_SELECT
            PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_F_CODE and PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_F_CODE
            has the same value. */
            /* Checked in eddp_hal_pnip_val.h. We use the R2 version. */
            register_value = ((pParam->param_union.Crt.Ppm.OnOff.PpmOn ? EDDP_CMD_PPM_PPM_ACTIVATE : EDDP_CMD_PPM_PPM_DEACTIVATE) << PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE |
                              (user_id << PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_USER_ID) |
                              (PNIP_REG_PPMCOMMAND_IF_CONTROL__VAL_CONFREQUEST_ENABLE << PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_CONFREQUEST) |
                              (PNIP_REG_PPMCOMMAND_IF_CONTROL__VAL_IF_SELECT_IFA) << PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_IF_SELECT |
                              (PNIP_REG_PPMCOMMAND_IF_CONTROL__VAL_F_CODE_PPM_ON_OFF << PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_F_CODE));
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].Low_32Bit), register_value);

            /* There is a macro that checks PNIP_R2_REG_PPMCOMMAND_IF_CONTROL and PNIP_R3_REG_PPMCOMMAND_IF_CONTROL has the same value. */
            /* Checked in eddp_hal_pnip_val.h. We use the R2 version. */
            EDDP_SET_CBL_HIGH_32_WORD_WCRC(register_value, PNIP_R2_REG_PPMCOMMAND_IF_CONTROL, LSA_TRUE);
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].High_32Bit), register_value);             
        }
        break;

        case EDDP_CMD_PPM_ACW_ON_OFF:
        {
            switch(EDDP_HALGetPnipRevision(pDDB))
            {
                #if  ( defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT) || defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT))
                case EDDP_HAL_HW_PNIP_REV1:
                case EDDP_HAL_HW_PNIP_REV2:
                {
                    if(isAsync)
                    {
                        pCommandBlock = pParam->param_union.Crt.pCBL;
                        cbl_address   = (EDD_COMMON_MEM_PTR_TYPE)pCommandBlock;
                    }
            
                    // ***** Note: it seems that all arrays can be used with Acw_Count for range: 0..3
                    if (pParam->param_union.Crt.Ppm.ACW_OnOff.Acw_Count > pDDB->CMD.MaxAcwCountPerCmd)
                    {
                        EDDP_CMD_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] _EDDP_CMD_FillCommandBlockAndParam(): FATAL -> Ppm.ACW_OnOff.Acw_Count(%d) > AcwMaxNr(%d) per command!", 
                                          pParam->param_union.Crt.Ppm.ACW_OnOff.Acw_Count, pDDB->CMD.MaxAcwCountPerCmd);
                        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                    }

                    for ( i=0; i < pParam->param_union.Crt.Ppm.ACW_OnOff.Acw_Count; i++ )
                    {
                        /* There is a macro that checks                                                                                      */
                        /* PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_USER_ID and PNIP_R3_REG_PPMCOMMAND_IF_CONTROL_SHFT_USER_ID                 */
                        /* PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE and PNIP_R3_REG_PPMCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE       */
                        /* PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_CONFREQUEST and PNIP_R3_REG_PPMCOMMAND_IF_CONTROL_SHFT_CONFREQUEST         */
                        /* PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_F_CODE and PNIP_R3_REG_PPMCOMMAND_IF_CONTROL_SHFT_F_CODE                   */
                        /* has the same value.                                                                                               */
                        /* Checked in eddp_hal_pnip_val.h. We use the R2 version.                                                            */
                        register_value = ((pParam->param_union.Crt.Ppm.ACW_OnOff.Acw_On ? EDDP_CMD_PPM_ACW_ACTIVATE : EDDP_CMD_PPM_ACW_DEACTIVATE) << PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE |
                                          (pParam->param_union.Crt.Ppm.ACW_OnOff.Acw_DG ? EDDP_CMD_PPM_IS_ACW_DG : EDDP_CMD_PPM_IS_ACW) << PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE |
                                          (pParam->param_union.Crt.Ppm.ACW_OnOff.FirstSend ? EDDP_CMD_PPM_FIRST_SEND_ACTIVATE : EDDP_CMD_PPM_FIRST_SEND_DEACTIVATE) << PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE |
                                          (pParam->param_union.Crt.Ppm.ACW_OnOff.ApiAdr_Acw[i] << EDDP_CMD_PPM_SHIFT_ACW_ADDRESS) << PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE |
                                          (user_id << PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_USER_ID) |
                                          (PNIP_REG_PPMCOMMAND_IF_CONTROL__VAL_CONFREQUEST_ENABLE << PNIP_R3_REG_PPMCOMMAND_IF_CONTROL_SHFT_CONFREQUEST) |
                                          (PNIP_REG_PPMCOMMAND_IF_CONTROL__VAL_F_CODE_ACW_ON_OFF << PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_F_CODE));
                        EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[i].Low_32Bit), register_value);

                        /* There is a macro that checks PNIP_R2_REG_PPMCOMMAND_IF_CONTROL and PNIP_R2_REG_PPMCOMMAND_IF_CONTROL has the same value. */
                        /* Checked in eddp_hal_pnip_val.h. We use the R2 version. */
                        
                        EDDP_SET_CBL_HIGH_32_WORD_WCRC(register_value, PNIP_R2_REG_PPMCOMMAND_IF_CONTROL, ((i+1 == pParam->param_union.Crt.Ppm.ACW_OnOff.Acw_Count) ? LSA_TRUE : LSA_FALSE));
                        EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[i].High_32Bit), register_value);
                    }
                }
                break;
                #endif

                default:
                {
                    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] _EDDP_CMD_FillCommandBlockAndParam(): FATAL -> PnipRevision(%d) is unknown", 
                                       EDDP_HALGetPnipRevision(pDDB));
                    EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                }
                break;
            }


        }
        break;

        case EDDP_CMD_PPM_ACW_IN_OUT:
        {
            if(isAsync)
            {
                pCommandBlock = pParam->param_union.Crt.pCBL;
                cbl_address   = (EDD_COMMON_MEM_PTR_TYPE)pCommandBlock;
            }
    
            // ***** Note: it seems that all arrays can be used with Acw_Count for range: 0..EDDP_CMD_MAX_ACW_CNT_PER_CMD
            if (pParam->param_union.Crt.Ppm.ACW_InOut.Acw_Count > pDDB->CMD.MaxAcwCountPerCmd)
            {
                EDDP_CMD_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] _EDDP_CMD_FillCommandBlockAndParam(): FATAL -> Ppm.ACW_InOut.Acw_Count(%d) > AcwMaxNr(%d) per command!", 
                    pParam->param_union.Crt.Ppm.ACW_OnOff.Acw_Count, pDDB->CMD.MaxAcwCountPerCmd);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }

            for ( i=0; i < pParam->param_union.Crt.Ppm.ACW_InOut.Acw_Count; i++ )
            {
                target_param_address = (LSA_UINT32)(&pParam->param_union.Crt.pExternalCmdParam_ApiCtrlRam[i*2]);
        
                // write first 32bit 'Parameterblock' into API-Ctrl-Ram
                register_value = ((pParam->param_union.Crt.Ppm.ACW_InOut.Acw_In ? EDDP_CMD_PPM_PARAM_ACW_IN : EDDP_CMD_PPM_PARAM_ACW_OUT) |
                                  (pParam->param_union.Crt.Ppm.ACW_InOut.ApiAdr_NextPointer_Prev[i] << EDDP_CMD_PPM_SHIFT_PARAM_NEXT_POINTER));
                
                EDDP_HAL_MEM32_WRITE(pDDB, (LSA_UINT32*)target_param_address, register_value);
                // write second 32bit 'Parameterblock' into API-Ctrl-Ram
                register_value = pParam->param_union.Crt.Ppm.ACW_InOut.ApiAdr_Acw_New[i] | 
                                 (pParam->param_union.Crt.Ppm.ACW_InOut.ApiAdr_Acw_DG_Next[i] << EDDP_CMD_PPM_SHIFT_PARAM_NEXT_ACW_DG);
                EDDP_HAL_MEM32_WRITE(pDDB, ((LSA_UINT32*)target_param_address) + 1, register_value);

                // configure "ACW_in_out" command
                target_param_address_offset = EDDP_HALGetPnipRAMOffsetToRAM (pDDB, EDDP_HAL_RAMTYPE_API_CTRL, (EDD_COMMON_MEM_PTR_TYPE)target_param_address);
                // write first 32bit of WCRC command
                /* There is a macro that checks                                                                                    */
                /* PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_USER_ID       and PNIP_R3_REG_PPMCOMMAND_IF_CONTROL_SHFT_USER_ID         */
                /* PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_CONFREQUEST   and PNIP_R3_REG_PPMCOMMAND_IF_CONTROL_SHFT_CONFREQUEST     */
                /* PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_F_CODE        and PNIP_R3_REG_PPMCOMMAND_IF_CONTROL_SHFT_F_CODE          */
                /* has the same value.                                                                                             */
                /* Checked in eddp_hal_pnip_val.h. We use the R2 version. */
                register_value = ((target_param_address_offset << EDDP_CMD_PPM_SHIFT_PARAMETERBLOCK_ADDR) |
                                  (user_id << PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_USER_ID) |
                                  (PNIP_REG_PPMCOMMAND_IF_CONTROL__VAL_CONFREQUEST_ENABLE << PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_CONFREQUEST) | 
                                  (PNIP_REG_PPMCOMMAND_IF_CONTROL__VAL_F_CODE_ACW_IN_OUT << PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_F_CODE));  
                
                
                
                EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[i].Low_32Bit), register_value);
                // write second 32bit of WCRC command
                /* There is a macro that checks PNIP_R2_REG_PPMCOMMAND_IF_CONTROL and PNIP_R3_REG_PPMCOMMAND_IF_CONTROL has the same value. */
                /* Checked in eddp_hal_pnip_val.h. We use the R2 version. */
                EDDP_SET_CBL_HIGH_32_WORD_WCRC(register_value, PNIP_R2_REG_PPMCOMMAND_IF_CONTROL, ((i+1 == pParam->param_union.Crt.Ppm.ACW_InOut.Acw_Count) ? LSA_TRUE : LSA_FALSE));
                EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[i].High_32Bit), register_value);
            }
        }
        break;

        case EDDP_CMD_PPM_ACW_MOVE:               
        {
            if(isAsync)
            {
                pCommandBlock = pParam->param_union.Crt.pCBL;
                cbl_address   = (EDD_COMMON_MEM_PTR_TYPE)pCommandBlock;
            }

            // configure command for APICtrl-Ram
            target_param_address        = (LSA_UINT32)pParam->param_union.Crt.pExternalCmdParam_ApiCtrlRam;
            target_param_address_offset = EDDP_HALGetPnipRAMOffsetToRAM (pDDB, EDDP_HAL_RAMTYPE_API_CTRL, (EDD_COMMON_MEM_PTR_TYPE)target_param_address);
            
            // write first 32bit 'Parameterblock' into APICtrl-Ram
            register_value = (((pParam->param_union.Crt.Ppm.ACW_Move.ApiAdr_NextPointer_New_Prev & EDDP_CMD_PPM_NEW_NEXT_PART_1_BITMASK) << EDDP_CMD_PPM_NEW_NEXT_PART_1_SHIFT) | 
                              ((pParam->param_union.Crt.Ppm.ACW_Move.ApiAdr_NextPointer_Old_Prev) << EDDP_CMD_PPM_MOVE_NEXT_OLD_SHIFT));
           
            EDDP_HAL_MEM32_WRITE(pDDB, (LSA_UINT32*)target_param_address, register_value);
            
            // write second 32bit 'Parameterblock' into APICtrl-Ram
            register_value = ((pParam->param_union.Crt.Ppm.ACW_Move.ApiAdr_Acw) | 
                              ((pParam->param_union.Crt.Ppm.ACW_Move.ApiAdr_NextPointer_New_Prev & ~EDDP_CMD_PPM_NEW_NEXT_PART_1_BITMASK) << EDDP_CMD_PPM_NEW_NEXT_PART_2_SHIFT));
            // write second 32bit 'Parameterblock' APICtrl-Ram
            EDDP_HAL_MEM32_WRITE(pDDB, (LSA_UINT32*)target_param_address + 1, register_value);

            // configure "ACW_move" command
            // write first 32bit of WCRC command
            /* There is a macro that checks                                                                                    */
            /* PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_USER_ID       and PNIP_R3_REG_PPMCOMMAND_IF_CONTROL_SHFT_USER_ID         */
            /* PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_CONFREQUEST   and PNIP_R3_REG_PPMCOMMAND_IF_CONTROL_SHFT_CONFREQUEST     */
            /* PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_F_CODE        and PNIP_R3_REG_PPMCOMMAND_IF_CONTROL_SHFT_F_CODE          */
            /* has the same value.                                                                                             */
            /* Checked in eddp_hal_pnip_val.h. We use the R2 version. */
            register_value = ((target_param_address_offset << EDDP_CMD_PPM_SHIFT_PARAMETERBLOCK_ADDR) |
                              (user_id << PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_USER_ID) |
                              (PNIP_REG_PPMCOMMAND_IF_CONTROL__VAL_CONFREQUEST_ENABLE << PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_CONFREQUEST) | 
                              (PNIP_REG_PPMCOMMAND_IF_CONTROL__VAL_F_CODE_ACW_MOVE << PNIP_R2_REG_PPMCOMMAND_IF_CONTROL_SHFT_F_CODE)); 
            
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].Low_32Bit), register_value);
            // write second 32bit of WCRC command
            /* There is a macro that checks PNIP_R2_REG_PPMCOMMAND_IF_CONTROL and PNIP_R3_REG_PPMCOMMAND_IF_CONTROL has the same value. */
            /* Checked in eddp_hal_pnip_val.h. We use the R2 version. */
            EDDP_SET_CBL_HIGH_32_WORD_WCRC(register_value, PNIP_R2_REG_PPMCOMMAND_IF_CONTROL, LSA_TRUE);
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].High_32Bit), register_value);
        }
        break;

        case EDDP_CMD_PPM_ACW_XCHANGE_BUFF:
        {
            #ifdef EDDP_MESSAGE
            #ifdef EDDP_PRAGMA_MESSAGE
            #pragma EDDP_PRAGMA_MESSAGE("WARNING: EDDP_CMD_PPM_ACW_XCHANGE_BUFF not implemented yet!")
            #endif /* EDDP_PRAGMA_MESSAGE */
            #endif /* EDDP_MESSAGE */
        }
        break;

        /* ------------------------------------------------ */
        /* Pack                                             */
        /* ------------------------------------------------ */
        case EDDP_CMD_PACK_NOP:
        {
            if(isAsync)
            {
                //get offset address of CBLs belonging to this queue
                cbl_address = EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_COMMAND_HOST_ASYNC, 0);
                //get offset address of parameters belonging to this queue
                param_address = EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_PARAM_HOST_ASYNC, 0);
                pCommandBlock = &(((EDDP_CMD_RAM_COMMAND_BLOCKS_HOST_ASYNC_PTR_TYPE)cbl_address)->Pack_Nop);
                cbl_address   = (EDD_COMMON_MEM_PTR_TYPE)pCommandBlock;
            }
            
            register_value = 0x40000000; // NOP
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].High_32Bit), register_value);
        }
        break;

        case EDDP_CMD_PACK_DISABLE_GRP:
        {
            if(isAsync)
            {
                //get offset address of CBLs belonging to this queue
                cbl_address = EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_COMMAND_HOST_ASYNC, 0);
                //get offset address of parameters belonging to this queue
                param_address = EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_PARAM_HOST_ASYNC, 0);
                pCommandBlock = &(((EDDP_CMD_RAM_COMMAND_BLOCKS_HOST_ASYNC_PTR_TYPE)cbl_address)->Pack_DisableGrp);
                cbl_address   = (EDD_COMMON_MEM_PTR_TYPE)pCommandBlock;
            }
            
            // ***** PackGrp 0..7 and bit for In=1/Out=0
            register_value = (((pParam->param_union.Pack.DisableGrp.IsInGrp ? PNIP_REG_PACKCOMMAND_IF_CONTROL__VAL_PACKGRP_IN : PNIP_REG_PACKCOMMAND_IF_CONTROL__VAL_PACKGRP_OUT)       << PNIP_REG_PACKCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE) |
                              (((pParam->param_union.Pack.DisableGrp.DisPackGrp & (LSA_UINT32)7) << 1 ) << PNIP_REG_PACKCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE) |
                              (user_id << PNIP_REG_PACKCOMMAND_IF_CONTROL_SHFT_USER_ID) |
                              (PNIP_REG_PACKCOMMAND_IF_CONTROL__VAL_CONFREQUEST_ENABLE << PNIP_REG_PACKCOMMAND_IF_CONTROL_SHFT_CONFREQUEST) |
                              (PNIP_REG_PACKCOMMAND_IF_CONTROL__VAL_F_CODE_DISABLE_GRP << PNIP_REG_PACKCOMMAND_IF_CONTROL_SHFT_F_CODE_2_0));
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].Low_32Bit), register_value);

            EDDP_SET_CBL_HIGH_32_WORD_WCRC(register_value, PNIP_REG_PACKCOMMAND_IF_CONTROL, LSA_TRUE);
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].High_32Bit), register_value);
        }
        break;

        case EDDP_CMD_PACK_ENABLE_GRP:
        {
            if(isAsync)
            {
                //get offset address of CBLs belonging to this queue
                cbl_address = EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_COMMAND_HOST_ASYNC, 0);
                //get offset address of parameters belonging to this queue
                param_address = EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_PARAM_HOST_ASYNC, 0);
                pCommandBlock = &(((EDDP_CMD_RAM_COMMAND_BLOCKS_HOST_ASYNC_PTR_TYPE)cbl_address)->Pack_EnableGrp);
                cbl_address   = (EDD_COMMON_MEM_PTR_TYPE)pCommandBlock;
            }
            
            // ***** PackGrp 0..7 and bit for In=1/Out=0
            register_value = (((pParam->param_union.Pack.EnableGrp.IsInGrp ? PNIP_REG_PACKCOMMAND_IF_CONTROL__VAL_PACKGRP_IN : PNIP_REG_PACKCOMMAND_IF_CONTROL__VAL_PACKGRP_OUT)       << PNIP_REG_PACKCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE) |
                              (((pParam->param_union.Pack.EnableGrp.EnPackGrp & (LSA_UINT32)7) << 1 ) << PNIP_REG_PACKCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE) |
                              (user_id << PNIP_REG_PACKCOMMAND_IF_CONTROL_SHFT_USER_ID) |
                              (PNIP_REG_PACKCOMMAND_IF_CONTROL__VAL_CONFREQUEST_ENABLE << PNIP_REG_PACKCOMMAND_IF_CONTROL_SHFT_CONFREQUEST) |
                              (PNIP_REG_PACKCOMMAND_IF_CONTROL__VAL_F_CODE_ENABLE_GRP << PNIP_REG_PACKCOMMAND_IF_CONTROL_SHFT_F_CODE_2_0));
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].Low_32Bit), register_value);

            EDDP_SET_CBL_HIGH_32_WORD_WCRC(register_value, PNIP_REG_PACKCOMMAND_IF_CONTROL, LSA_TRUE);
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].High_32Bit), register_value);
        }
        break;

        case EDDP_CMD_PACK_DIS_ENA_GRP:
        {
            if(isAsync)
            {
                //get offset address of CBLs belonging to this queue
                cbl_address = EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_COMMAND_HOST_ASYNC, 0);
                //get offset address of parameters belonging to this queue
                param_address = EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_PARAM_HOST_ASYNC, 0);
                pCommandBlock = &(((EDDP_CMD_RAM_COMMAND_BLOCKS_HOST_ASYNC_PTR_TYPE)cbl_address)->Pack_DisableEnableGrp);
                cbl_address   = (EDD_COMMON_MEM_PTR_TYPE)pCommandBlock;
            }
            
            // ***** EnPackGrp 0..7, DisPackGrp 0..7 and bit for In=1/Out=0
            register_value = (((pParam->param_union.Pack.DisEnaGrp.IsInGrp ? PNIP_REG_PACKCOMMAND_IF_CONTROL__VAL_PACKGRP_IN : PNIP_REG_PACKCOMMAND_IF_CONTROL__VAL_PACKGRP_OUT)       << PNIP_REG_PACKCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE) |
                              (((pParam->param_union.Pack.DisEnaGrp.DisPackGrp & (LSA_UINT32)7) << 1 ) << PNIP_REG_PACKCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE) |
                              (((pParam->param_union.Pack.DisEnaGrp.EnPackGrp & (LSA_UINT32)7) << (1+3) ) << PNIP_REG_PACKCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE) |
                              (user_id << PNIP_REG_PACKCOMMAND_IF_CONTROL_SHFT_USER_ID) |
                              (PNIP_REG_PACKCOMMAND_IF_CONTROL__VAL_CONFREQUEST_ENABLE << PNIP_REG_PACKCOMMAND_IF_CONTROL_SHFT_CONFREQUEST) |
                              (PNIP_REG_PACKCOMMAND_IF_CONTROL__VAL_F_CODE_DIS_ENA_GRP << PNIP_REG_PACKCOMMAND_IF_CONTROL_SHFT_F_CODE_2_0));
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].Low_32Bit), register_value);

            EDDP_SET_CBL_HIGH_32_WORD_WCRC(register_value, PNIP_REG_PACKCOMMAND_IF_CONTROL, LSA_TRUE);
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].High_32Bit), register_value);
        }
        break;
        /*
        --> verify command with InBoundPack_x.Inbound_Valid
        --> verify command with OutBoundPack_x.Outbound_Valid
        */

        /* ------------------------------------------------ */
        /* FDB                                              */
        /* ------------------------------------------------ */
        case EDDP_CMD_FDB_CLEAR_UC_TABLE:
        {
            if(isAsync)
            {
                //get offset address of CBLs belonging to this queue
                cbl_address = EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_COMMAND_HOST_ASYNC, 0);
                //get offset address of parameters belonging to this queue
                param_address = EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_PARAM_HOST_ASYNC, 0);
                pCommandBlock = &(((EDDP_CMD_RAM_COMMAND_BLOCKS_HOST_ASYNC_PTR_TYPE)cbl_address)->Fdb_ClearUcTable);
                cbl_address   = (EDD_COMMON_MEM_PTR_TYPE)pCommandBlock;
            }
            
            register_value = (((pParam->param_union.Fdb.ClearUCTable.Delete_Only_Dynamic_Entries ? EDDP_CMD_FDB_DELETE_ONLY_DYNAMIC_ENTRIES : EDDP_CMD_FDB_DELETE_ALL_ENTRIES) << PNIP_REG_FDBCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE) |
                              ((pParam->param_union.Fdb.ClearUCTable.Delete_Only_Dynamic_Entries_Port1 ? EDDP_CMD_FDB_DELETE_ONLY_DYNAMIC_ENTRIES_PORTX : EDDP_CMD_FDB_DELETE_ONLY_DYNAMIC_ENTRIES_ALL_PORT) << (PNIP_REG_FDBCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE + 1)) |
                              ((pParam->param_union.Fdb.ClearUCTable.Delete_Only_Dynamic_Entries_Port2 ? EDDP_CMD_FDB_DELETE_ONLY_DYNAMIC_ENTRIES_PORTX : EDDP_CMD_FDB_DELETE_ONLY_DYNAMIC_ENTRIES_ALL_PORT) << (PNIP_REG_FDBCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE + 2)) |
                              ((pParam->param_union.Fdb.ClearUCTable.Delete_Only_Dynamic_Entries_Port3 ? EDDP_CMD_FDB_DELETE_ONLY_DYNAMIC_ENTRIES_PORTX : EDDP_CMD_FDB_DELETE_ONLY_DYNAMIC_ENTRIES_ALL_PORT) << (PNIP_REG_FDBCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE + 3)) |
                              ((pParam->param_union.Fdb.ClearUCTable.Delete_Only_Dynamic_Entries_Port4 ? EDDP_CMD_FDB_DELETE_ONLY_DYNAMIC_ENTRIES_PORTX : EDDP_CMD_FDB_DELETE_ONLY_DYNAMIC_ENTRIES_ALL_PORT) << (PNIP_REG_FDBCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE + 4)) |
                              (user_id << PNIP_REG_FDBCOMMAND_IF_CONTROL_SHFT_USER_ID) |
                              (PNIP_REG_FDBCOMMAND_IF_CONTROL__VAL_CONFREQUEST_ENABLE << PNIP_REG_FDBCOMMAND_IF_CONTROL_SHFT_CONFREQUEST) |
                              (PNIP_REG_FDBCOMMAND_IF_CONTROL__VAL_IF_SELECT_IFA) << PNIP_REG_FDBCOMMAND_IF_CONTROL_SHFT_IF_SELECT |
                              (PNIP_REG_FDBCOMMAND_IF_CONTROL__VAL_F_CODE_CLEAR_UC_TABLE << PNIP_REG_FDBCOMMAND_IF_CONTROL_SHFT_F_CODE));
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].Low_32Bit), register_value);

            EDDP_SET_CBL_HIGH_32_WORD_WCRC(register_value, PNIP_REG_FDBCOMMAND_IF_CONTROL, LSA_TRUE);
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].High_32Bit), register_value);
        }
        break;

        case EDDP_CMD_FDB_INSERT_UC_ENTRY:
        {
            if(isAsync)
            {
                //get offset address of CBLs belonging to this queue
                cbl_address = EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_COMMAND_HOST_ASYNC, 0);
                //get offset address of parameters belonging to this queue
                param_address = EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_PARAM_HOST_ASYNC, 0);
                pCommandParamBlock = &(((EDDP_CMD_RAM_PARAM_HOST_ASYNC_PTR_TYPE)param_address)->Fdb_InsertUcEntry);
                param_address      = (EDD_COMMON_MEM_PTR_TYPE)pCommandParamBlock;
                pCommandBlock      = &(((EDDP_CMD_RAM_COMMAND_BLOCKS_HOST_ASYNC_PTR_TYPE)cbl_address)->Fdb_InsertUcEntry[0]);
                cbl_address        = (EDD_COMMON_MEM_PTR_TYPE)pCommandBlock;
            }
            
            // write first 32bit of UC-Entry into CMD-Ram
            register_value = ((LSA_UINT32)(pParam->param_union.Fdb.InsertUCEntry.TxPort)                                                                                                           |
                              (LSA_UINT32)(pParam->param_union.Fdb.InsertUCEntry.Static           ? EDDP_CMD_FDB_PARAM_STATIC_ACTIVATE : EDDP_CMD_FDB_PARAM_STATIC_DEACTIVATE)                     |
                              (LSA_UINT32)(pParam->param_union.Fdb.InsertUCEntry.Aging            ? EDDP_CMD_FDB_PARAM_AGING_ACTIVATE : EDDP_CMD_FDB_PARAM_AGING_DEACTIVATE)                       |
                              (LSA_UINT32)(pParam->param_union.Fdb.InsertUCEntry.RxPort << EDDP_CMD_FDB_PARAM_SHIFT_RXPORT)                                                                        |
                              (LSA_UINT32)(pParam->param_union.Fdb.InsertUCEntry.Learning_Disable ? EDDP_CMD_FDB_PARAM_LEARNING_DISABLE_ACTIVATE : EDDP_CMD_FDB_PARAM_LEARNING_DISABLE_DEACTIVATE) |
                              (LSA_UINT32)(pParam->param_union.Fdb.InsertUCEntry.Mgmt_Enable      ? EDDP_CMD_FDB_PARAM_MGMT_ENABLE_ACTIVATE : EDDP_CMD_FDB_PARAM_MGMT_ENABLE_DEACTIVATE)           |
                              (LSA_UINT32)(pParam->param_union.Fdb.InsertUCEntry.Mgmt_Prio        ? EDDP_CMD_FDB_PARAM_MGMT_PRIO_ACTIVATE : EDDP_CMD_FDB_PARAM_MGMT_PRIO_DEACTIVATE)               |
                              (LSA_UINT32)(pParam->param_union.Fdb.InsertUCEntry.UC_MAC_Address.MacAdr[5] << EDDP_CMD_FDB_PARAM_SHIFT_MAC_ADDR_5)                                                  |
                              (LSA_UINT32)(pParam->param_union.Fdb.InsertUCEntry.UC_MAC_Address.MacAdr[4] << EDDP_CMD_FDB_PARAM_SHIFT_MAC_ADDR_4));
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandParamBlock[0].Low_32Bit), register_value);
            // write second 32bit of UC entry into CMD-Ram
            register_value = (LSA_UINT32)(pParam->param_union.Fdb.InsertUCEntry.UC_MAC_Address.MacAdr[3])                                         |
                             (LSA_UINT32)(pParam->param_union.Fdb.InsertUCEntry.UC_MAC_Address.MacAdr[2] <<  EDDP_CMD_FDB_PARAM_SHIFT_MAC_ADDR_2) |
                             (LSA_UINT32)(pParam->param_union.Fdb.InsertUCEntry.UC_MAC_Address.MacAdr[1] << EDDP_CMD_FDB_PARAM_SHIFT_MAC_ADDR_1)  |
                             (LSA_UINT32)(pParam->param_union.Fdb.InsertUCEntry.UC_MAC_Address.MacAdr[0] << EDDP_CMD_FDB_PARAM_SHIFT_MAC_ADDR_0);
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandParamBlock[0].High_32Bit), register_value);

            // configure command for copying param_struct from CMD-Ram into FDB-Ram
            target_param_address = (LSA_UINT32)EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_FDB_TABLE, EDDP_HAL_RAMSUB_FDB_TABLE_PARAM_STRUCT, 0);
            target_param_address = EDDP_HAL_H2P_ADDR(pDDB, target_param_address);    

            // is synchronous command processing ?
            if (user_id == EDDP_CMD_USER_ID_HOST_SYNC)
            {
                // for synchonous command processing we use User_ID '1' 
                target_param_address += 16;
            }

            param_address_pnip = EDDP_HAL_H2P_ADDR(pDDB, param_address);
            /* PNIP_RAM_CMDCTRL_RAM_START was replaced by PNIP_R2P2_RAM_CMDCTRL_RAM_START */
            /* all macros of group PNIP_xxxx_RAM_CMDCTRL_RAM_START are checked in eddp_hal_pnip_val.h to have the same value*/
            param_address_pnip = param_address_pnip - PNIP_R2P2_RAM_CMDCTRL_RAM_START;
            // write first 32bit of first command
            EDDP_SET_CBL_LOW_32_BWRI(register_value, param_address_pnip, EDDP_CMD_FDB_UC_INSERT_PARAMBLOCK_COUNT_32BIT);
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].Low_32Bit), register_value);
            // write second 32bit of first command
            EDDP_SET_CBL_HIGH_32_BWRI(register_value, target_param_address);
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].High_32Bit), register_value);

            // configure "Insert UC-Entry" command
            // write first 32bit of WCRC command
            register_value = ((user_id << PNIP_REG_FDBCOMMAND_IF_CONTROL_SHFT_USER_ID)                                                                                                                             |
                              (PNIP_REG_FDBCOMMAND_IF_CONTROL__VAL_CONFREQUEST_ENABLE << PNIP_REG_FDBCOMMAND_IF_CONTROL_SHFT_CONFREQUEST)                                                                          |
                              (PNIP_REG_FDBCOMMAND_IF_CONTROL__VAL_IF_SELECT_IFA) << PNIP_REG_FDBCOMMAND_IF_CONTROL_SHFT_IF_SELECT |
                              (PNIP_REG_FDBCOMMAND_IF_CONTROL__VAL_F_CODE_INSERT_UC_ENTRY << PNIP_REG_FDBCOMMAND_IF_CONTROL_SHFT_F_CODE));
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[1].Low_32Bit), register_value);
            // write second 32bit of WCRC command
            EDDP_SET_CBL_HIGH_32_WORD_WCRC(register_value, PNIP_REG_FDBCOMMAND_IF_CONTROL, LSA_TRUE);
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[1].High_32Bit), register_value);
        }
        break;

        /* ------------------------------------------------ */
        /* VLAN                                             */
        /* ------------------------------------------------ */

        /* ------------------------------------------------ */
        /* SYNC                                             */
        /* ------------------------------------------------ */
        case EDDP_CMD_GSYNC_UPDATE_COMCYCL:
        {
            if(isAsync)
            {
                //get offset address of CBLs belonging to this queue
                cbl_address = EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_COMMAND_HOST_ASYNC, 0);
                //get offset address of parameters belonging to this queue
                param_address = EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_PARAM_HOST_ASYNC, 0);
                pCommandParamBlock = &(((EDDP_CMD_RAM_PARAM_HOST_ASYNC_PTR_TYPE)param_address)->Gsync_UpdateComCycl);
                param_address      = (EDD_COMMON_MEM_PTR_TYPE)pCommandParamBlock;
                pCommandBlock      = &(((EDDP_CMD_RAM_COMMAND_BLOCKS_HOST_ASYNC_PTR_TYPE)cbl_address)->Gsync_UpdateComCycl[0]);
                cbl_address        = (EDD_COMMON_MEM_PTR_TYPE)pCommandBlock;
            }
            
            // write new CommunicationLength into CMD-Ram
            register_value = pParam->param_union.Gsync.UpdateComCycl.CommunicationLength_1ns;
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandParamBlock[0].Low_32Bit), register_value);

            // configure command for copying param_struct from CMD-Ram into Sync-Ram
            target_param_address = (LSA_UINT32)EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_SYNC, EDDP_HAL_RAMSUB_SYNC_CMD_PARAM, 0);
            target_param_address = EDDP_HAL_H2P_ADDR(pDDB, target_param_address);    

            // is synchronous command processing ?
            if (user_id == EDDP_CMD_USER_ID_HOST_SYNC)
            {
                // for synchonous command processing we use User_ID '1' 
                target_param_address += 8;
            }

            param_address_pnip = EDDP_HAL_H2P_ADDR(pDDB, param_address);
            /* PNIP_RAM_CMDCTRL_RAM_START was replaced by PNIP_R2P2_RAM_CMDCTRL_RAM_START */
            /* all macros of group PNIP_xxxx_RAM_CMDCTRL_RAM_START are checked in eddp_hal_pnip_val.h to have the same value*/
            param_address_pnip = param_address_pnip - PNIP_R2P2_RAM_CMDCTRL_RAM_START;
            // write first 32bit of first command
            EDDP_SET_CBL_LOW_32_BWRI(register_value, param_address_pnip, EDDP_CMD_SYNC_UPDATE_COMCYCLE_PARAM_COUNT_32BIT);
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].Low_32Bit), register_value);
            // write second 32bit of first command
            EDDP_SET_CBL_HIGH_32_BWRI(register_value, target_param_address);
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].High_32Bit), register_value);            

            // configure "Update_COMCYCLE" command
            // write first 32bit of WCRC command
            /* There is a macro that checks PNIP_R2_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE and PNIP_R3_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE has the same value. */
            /* Checked in eddp_hal_pnip_val.h. We use the R2 version. */
            register_value = ((user_id << PNIP_REG_SYNCCOMMAND_IF_CONTROL_SHFT_USER_ID)                                                                                                                                  |
                              (PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_CONFREQUEST_ENABLE << PNIP_REG_SYNCCOMMAND_IF_CONTROL_SHFT_CONFREQUEST)                                                                              |
                              (PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_IF_SELECT_IFA) << PNIP_R2_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE |
                              (PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_F_CODE_UPDATE_COMCYCLE << PNIP_REG_SYNCCOMMAND_IF_CONTROL_SHFT_F_CODE));
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[1].Low_32Bit), register_value);
            // write second 32bit of WCRC command
            EDDP_SET_CBL_HIGH_32_WORD_WCRC(register_value, PNIP_REG_SYNCCOMMAND_IF_CONTROL, LSA_TRUE);
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[1].High_32Bit), register_value);
        }
        break;

        case EDDP_CMD_GSYNC_CONNECT_SYNC_A:
        {
            if(isAsync)
            {
                //get offset address of CBLs belonging to this queue
                cbl_address = EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_COMMAND_HOST_ASYNC, 0);
                //get offset address of parameters belonging to this queue
                param_address = EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_PARAM_HOST_ASYNC, 0);
                pCommandBlock = &(((EDDP_CMD_RAM_COMMAND_BLOCKS_HOST_ASYNC_PTR_TYPE)cbl_address)->Gsync_ConnectSyncA[0]);
                cbl_address   = (EDD_COMMON_MEM_PTR_TYPE)pCommandBlock;
            }
            
            // configure "Connect_SYNC_A" command for given "ApplicationTimer_x"
            /* There is a macro that checks PNIP_R2_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE and PNIP_R3_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE has the same value. */
            /* Checked in eddp_hal_pnip_val.h. We use the R2 version. */
            register_value = ((user_id << PNIP_REG_SYNCCOMMAND_IF_CONTROL_SHFT_USER_ID)                                                                                                                                  |
                              (PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_CONFREQUEST_ENABLE << PNIP_REG_SYNCCOMMAND_IF_CONTROL_SHFT_CONFREQUEST)                                                                              |
                              (pParam->param_union.Gsync.ConnectSyncA.ApplTimerNr) << PNIP_R2_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE |
                              (PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_F_CODE_CONNECT_SYNC_A << PNIP_REG_SYNCCOMMAND_IF_CONTROL_SHFT_F_CODE));
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[1].Low_32Bit), register_value);
            // write second 32bit of WCRC command
            EDDP_SET_CBL_HIGH_32_WORD_WCRC(register_value, PNIP_REG_SYNCCOMMAND_IF_CONTROL, LSA_TRUE);
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[1].High_32Bit), register_value);
        }
        break;

        case EDDP_CMD_GSYNC_UPDATE_APPCYCL:
        {
            if(isAsync)
            {
                //get offset address of CBLs belonging to this queue
                cbl_address = EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_COMMAND_HOST_ASYNC, 0);
                //get offset address of parameters belonging to this queue
                param_address = EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_PARAM_HOST_ASYNC, 0);
                pCommandParamBlock = &(((EDDP_CMD_RAM_PARAM_HOST_ASYNC_PTR_TYPE)param_address)->Gsync_UpdateAppCycl);
                param_address      = (EDD_COMMON_MEM_PTR_TYPE)pCommandParamBlock;
                pCommandBlock      = &(((EDDP_CMD_RAM_COMMAND_BLOCKS_HOST_ASYNC_PTR_TYPE)cbl_address)->Gsync_UpdateAppCycl[0]);
                cbl_address        = (EDD_COMMON_MEM_PTR_TYPE)pCommandBlock;
            }
            
            // write new ApplicationLength into CMD-Ram
            register_value = pParam->param_union.Gsync.UpdateAppCycl.ApplicationLengthNs;
            if (register_value < 8)
                register_value = 0; // because of 8ns granularity in register "ApplTimer_x"
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandParamBlock[0].Low_32Bit), register_value);

            // configure command for copying param_struct from CMD-Ram into Sync-Ram
            target_param_address = (LSA_UINT32)EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_SYNC, EDDP_HAL_RAMSUB_SYNC_CMD_PARAM, 0);
            target_param_address = EDDP_HAL_H2P_ADDR(pDDB, target_param_address);    

            // is synchronous command processing ?
            if (user_id == EDDP_CMD_USER_ID_HOST_SYNC)
            {
                // for synchonous command processing we use User_ID '1' 
                target_param_address += 8;
            }

            param_address_pnip = EDDP_HAL_H2P_ADDR(pDDB, param_address);
            /* PNIP_RAM_CMDCTRL_RAM_START was replaced by PNIP_R2P2_RAM_CMDCTRL_RAM_START */
            /* all macros of group PNIP_xxxx_RAM_CMDCTRL_RAM_START are checked in eddp_hal_pnip_val.h to have the same value*/
            param_address_pnip = param_address_pnip - PNIP_R2P2_RAM_CMDCTRL_RAM_START;
            // write first 32bit of first command
            EDDP_SET_CBL_LOW_32_BWRI(register_value, param_address_pnip, EDDP_CMD_SYNC_UPDATE_APPLCYCLE_PARAM_COUNT_32BIT);
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].Low_32Bit), register_value);
            // write second 32bit of first command
            EDDP_SET_CBL_HIGH_32_BWRI(register_value, target_param_address);
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].High_32Bit), register_value);            

            // configure "Update_APPLCYCLE" command for given "ApplicationTimer_x"
            /* There is a macro that checks PNIP_R2_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE and PNIP_R3_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE has the same value. */
            /* Checked in eddp_hal_pnip_val.h. We use the R2 version. */
            register_value = ((user_id << PNIP_REG_SYNCCOMMAND_IF_CONTROL_SHFT_USER_ID)                                                                                                                                  |
                              (PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_CONFREQUEST_ENABLE << PNIP_REG_SYNCCOMMAND_IF_CONTROL_SHFT_CONFREQUEST)                                                                              |
                              (pParam->param_union.Gsync.UpdateAppCycl.ApplTimerNr) << PNIP_R2_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE |
                              (PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_F_CODE_UPDATE_APPLCYCLE << PNIP_REG_SYNCCOMMAND_IF_CONTROL_SHFT_F_CODE));
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[1].Low_32Bit), register_value);
            // write second 32bit of WCRC command
            EDDP_SET_CBL_HIGH_32_WORD_WCRC(register_value, PNIP_REG_SYNCCOMMAND_IF_CONTROL, LSA_TRUE);
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[1].High_32Bit), register_value);
        }
        break;

        case EDDP_CMD_GSYNC_UPDATE_BASECYCL:
        {
            if(isAsync)
            {
                //get offset address of CBLs belonging to this queue
                cbl_address = EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_COMMAND_HOST_ASYNC, 0);
                //get offset address of parameters belonging to this queue
                param_address = EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_PARAM_HOST_ASYNC, 0);
                pCommandParamBlock = &(((EDDP_CMD_RAM_PARAM_HOST_ASYNC_PTR_TYPE)param_address)->Gsync_UpdateBaseCycl);
                param_address      = (EDD_COMMON_MEM_PTR_TYPE)pCommandParamBlock;
                pCommandBlock = &(((EDDP_CMD_RAM_COMMAND_BLOCKS_HOST_ASYNC_PTR_TYPE)cbl_address)->Gsync_UpdateBaseCycl[0]);
                cbl_address   = (EDD_COMMON_MEM_PTR_TYPE)pCommandBlock;
            }
            
            // write new PPM_Starttime into CMD-Ram
            register_value = pParam->param_union.Gsync.UpdateBaseCycl.PPM_Starttime_Ns;
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandParamBlock[0].Low_32Bit), register_value);

            // configure command for copying param_struct from CMD-Ram into Sync-Ram
            target_param_address = (LSA_UINT32)EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_SYNC, EDDP_HAL_RAMSUB_SYNC_CMD_PARAM, 0);
            target_param_address = EDDP_HAL_H2P_ADDR(pDDB, target_param_address);    

            // is synchronous command processing ?
            if (user_id == EDDP_CMD_USER_ID_HOST_SYNC)
            {
                // for synchonous command processing we use User_ID '1' 
                target_param_address += 8;
            }

            param_address_pnip = EDDP_HAL_H2P_ADDR(pDDB, param_address);
            /* PNIP_RAM_CMDCTRL_RAM_START was replaced by PNIP_R2P2_RAM_CMDCTRL_RAM_START, hardware dependency*/
            param_address_pnip = param_address_pnip - PNIP_R2P2_RAM_CMDCTRL_RAM_START;
            // write first 32bit of first command
            EDDP_SET_CBL_LOW_32_BWRI(register_value, param_address_pnip, EDDP_CMD_SYNC_UPDATE_BASECYCL_PARAM_COUNT_32BIT);
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].Low_32Bit), register_value);
            // write second 32bit of first command
            EDDP_SET_CBL_HIGH_32_BWRI(register_value, target_param_address);
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[0].High_32Bit), register_value);            

            // configure "Update_BASECYCLE" command
            // write first 32bit of WCRC command
            /* There is a macro that checks PNIP_R2_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE and PNIP_R3_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE has the same value. */
            /* Checked in eddp_hal_pnip_val.h. We use the R2 version. */
            register_value = ((user_id << PNIP_REG_SYNCCOMMAND_IF_CONTROL_SHFT_USER_ID)                                                                                                                                  |
                              (PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_CONFREQUEST_ENABLE << PNIP_REG_SYNCCOMMAND_IF_CONTROL_SHFT_CONFREQUEST)                                                                              |
                              (PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_IF_SELECT_IFA) << PNIP_R2_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE |
                              (PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_F_CODE_UPDATE_BASECYCLE << PNIP_REG_SYNCCOMMAND_IF_CONTROL_SHFT_F_CODE));
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[1].Low_32Bit), register_value);
            // write second 32bit of WCRC command
            EDDP_SET_CBL_HIGH_32_WORD_WCRC(register_value, PNIP_REG_SYNCCOMMAND_IF_CONTROL, LSA_TRUE);
            EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandBlock[1].High_32Bit), register_value);
        }
        break;

        /* ------------------------------------------------ */
        /* else                                             */
        /* ------------------------------------------------ */
        default:
        {
            EDDP_CMD_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL,"[H:--] _EDDP_CMD_FillCommandBlockAndParam(): FATAL -> Command Opcode not supported! CmdOpcode=0x%x", pParam->CmdOpcode);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }

    EDDP_CMD_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW,"[H:--] _EDDP_CMD_FillCommandBlockAndParam(): CmdOpcode=0x%X, register_value=0x%X, param_address=0x%X, user_id=%d", pParam->CmdOpcode, register_value, param_address, user_id);

    return (cbl_address);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CMD_ISR_ReqDone_IFA                    +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32					   EventNr      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  EventNr    : Event Number (Interrupt Number)                     (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE after INT_ReqDone_IFA   +*/
/*+				  interrupt occurred. It is executed within	LOW context		+*/
/*+               and does all the finishing after a request was confirmed  +*/
/*+               by Command Control.                                       +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CMD_ISR_ReqDone_IFA(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr)
{
#if 1

	EDDP_CMD_PARAM_PTR_TYPE pParam = EDDP_LL_CmdRequestPrm(pDDB);
	LSA_UINT32      QueueIndex;
    LSA_UNUSED_ARG(EventNr);       // handle unused parameter

    while(LSA_NULL != pParam)
    {
    	QueueIndex          = ((pParam->CmdOpcode & EDDP_CMD_GROUP_MASK) >> 8);
    	//CommandNumber       = (pParam->CmdOpcode & EDDP_CMD_COMMAND_MASK);


    	// Parameter structure is no longer used by command control
    	pParam->InUse = LSA_FALSE;

    	/* call callback function to notify user that the command is done */
    	if(LSA_NULL != pParam->Cbf)
    	{
    		pParam->Cbf(pParam->pDDB, pParam, pParam->Response);
    	}

		QueueIndex          = ((pParam->CmdOpcode & EDDP_CMD_GROUP_MASK) >> 8);
		//CommandNumber       = (pParam->CmdOpcode & EDDP_CMD_COMMAND_MASK);
		if(EDDP_CMD_INDEX_HOST_ASYNC_CRT == QueueIndex)
		{
			/* free cmd param element */
			EDDP_CRTCmdReleaseInternalCmdParamElement(pDDB);
		}

    	pParam->InUse = LSA_FALSE;

    	pParam = EDDP_LL_CmdRequestPrm(pDDB);
    }

    EDDP_GlbCrtRQBHandler(pDDB);

#else

    LSA_UINT32      RegConfirmingQueueAll, RegConfirmingQueueHost;
    LSA_UINT32      QueueIndex;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(EventNr);
#endif

    // read the register ConfirmingQueue (all queues)
    RegConfirmingQueueAll = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_CONFIRMINGQUEUE);
    // read only bits from ConfirmingQueue which belong to HOST queues
    RegConfirmingQueueHost = (EDDP_CMD_CONFIRMINGQUEUE_HOST_MASK & RegConfirmingQueueAll);
    // reset bits in ConfirmingQueue register for HOST queues
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_CONFIRMINGQUEUE, RegConfirmingQueueHost);

    EDDP_CMD_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] IN: EDDP_CMD_ISR_ReqDone_IFA(EventNr=0x%X): RegConfirmingQueueAll=0x%X, RegConfirmingQueueHost=0x%X", 
        EventNr, RegConfirmingQueueAll, RegConfirmingQueueHost);

    // check if there are queues with confirmed requests
    if (RegConfirmingQueueHost != 0)
    {
        QueueIndex = 0;

        while (RegConfirmingQueueHost != 0)
        {
            if (RegConfirmingQueueHost & 0x1)
            {
                if(EDDP_CMD_INDEX_HOST_ASYNC_CRT == QueueIndex)
                    _EDDP_CMD_FinishCrtCommandQueue (pDDB, QueueIndex);
                else
                    _EDDP_CMD_FinishCommandQueue (pDDB, QueueIndex);
            }
            QueueIndex++;
            EDDP_SHIFT_VALUE_RIGHT(RegConfirmingQueueHost, 1);
        }
    }
    
    /* trigger CRT command queue */
    EDDP_CRTCmdTriggerCrtQueue(pDDB);
    
    /* call RQB handler for pending RQBs */
    EDDP_GlbCrtRQBHandler(pDDB);
    
#endif
    EDDP_CMD_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] OUT: EDDP_CMD_ISR_ReqDone_IFA(EventNr=0x%X)", EventNr);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_CMD_FinishCommandQueue                +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32					   QueueIndex   +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB        : Pointer to DeviceDescriptionBlock                  (in)  +*/
/*+  QueueIndex  : Queue index                                        (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function loops thrue the selected queue and does the +*/
/*+				  finishing for all confirmed requests. This includes 		+*/
/*+               writing the command result to the pParam struct and       +*/
/*+               calling the corresponding callback function.              +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID _EDDP_CMD_FinishCommandQueue(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  QueueIndex)
{
    EDDP_CMD_QUEUE_INFO_PTR_TYPE    pCurrent_Queue_Info;
    EDDP_CMD_PARAM_PTR_TYPE         pParam;

    pCurrent_Queue_Info = &(pDDB->CMD.Queue[QueueIndex]);

    EDDP_CMD_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] _EDDP_CMD_FinishCommandQueue(QueueIndex=%d)", QueueIndex);

    while ( (EDDP_HAL_MEM32_READ(pDDB, pCurrent_Queue_Info->pCurrentConfPtr) & EDDP_CMD_REQUEST_STATUS_MASK) == EDDP_CMD_REQUEST_CONFIRMED )
    {
        // get stored pParam structure from DDB
        pParam = (EDDP_CMD_PARAM_PTR_TYPE)EDDP_GlbQueueRemoveFromBegin(&(pCurrent_Queue_Info->Queue_pParam));
        // fill pParam with return value if there is one
        pParam->Response = _EDDP_CMD_ReadCommandReturnValue(pDDB, pParam);
        // reset running bit for this command (command is now eligible to be used again)
        _EDDP_CMD_ResetCmdRunningFlag(pDDB, QueueIndex, (pParam->CmdOpcode & EDDP_CMD_COMMAND_MASK));
        // set current RequestEntry to state "free"
        EDDP_HAL_MEM32_WRITE(pDDB, pCurrent_Queue_Info->pCurrentConfPtr, EDDP_CMD_REQUEST_FREE);

        /* decrement running command count */
        pCurrent_Queue_Info->CmdRunningCount--;
        
        // check if pCurrentConfPtr is at the end of the queue
        if ( pCurrent_Queue_Info->pCurrentConfPtr == (pCurrent_Queue_Info->pQueueStartAddress + (pCurrent_Queue_Info->QueueLength - 1 )) )
        {   // set pCurrentConfPtr to next confirmed element
            pCurrent_Queue_Info->pCurrentConfPtr = pCurrent_Queue_Info->pQueueStartAddress;
        }
        else
        {   // set pCurrentConfPtr to next confirmed element
            pCurrent_Queue_Info->pCurrentConfPtr++;
        }
        
        // Parameter structure is no longer used by command control 
        pParam->InUse = LSA_FALSE;
        // call callback function to notify user that the command is done
        pParam->Cbf(pParam->pDDB, pParam, pParam->Response);
    }
}



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_CMD_FinishCrtCommandQueue             +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32                     QueueIndex   +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB        : Pointer to DeviceDescriptionBlock                  (in)  +*/
/*+  QueueIndex  : Queue index                                        (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function loops thrue the selected queue and does the +*/
/*+               finishing for all confirmed requests. This includes       +*/
/*+               writing the command result to the pParam struct and       +*/
/*+               calling the corresponding callback function.              +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID _EDDP_CMD_FinishCrtCommandQueue(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  QueueIndex)
{
    EDDP_CMD_QUEUE_INFO_PTR_TYPE    pCurrent_Queue_Info;
    EDDP_CMD_PARAM_PTR_TYPE         pParam;

    pCurrent_Queue_Info = &(pDDB->CMD.Queue[QueueIndex]);
    
    while ( (EDDP_HAL_MEM32_READ(pDDB, pCurrent_Queue_Info->pCurrentConfPtr) & EDDP_CMD_REQUEST_STATUS_MASK) == EDDP_CMD_REQUEST_CONFIRMED )
    {
        /* shall not happen */
        EDDP_ASSERT(0 < pCurrent_Queue_Info->CmdRunningCount);
        
        /* get stored pParam structure from "in work" queue */
        pParam = (EDDP_CMD_PARAM_PTR_TYPE)EDDP_GlbQueueRemoveFromBegin(&(pCurrent_Queue_Info->Queue_pParam));
        
        /* store pParam for later usage within "confirmation" queue */
        EDDP_GlbQueueAddToEnd (&(pCurrent_Queue_Info->QueueConf_pParam), &(pParam->Queue));
        
        /* check confirmed command for errors */
        pParam->Response = _EDDP_CMD_ReadCommandReturnValue(pDDB, pParam);
        
        /* reset running bit for this command */
        _EDDP_CMD_ResetCmdRunningFlag(pDDB, QueueIndex, (pParam->CmdOpcode & EDDP_CMD_COMMAND_MASK));
        
        /* decrement running command count and increment command count that is to be acknowledged in step 2 */
        pCurrent_Queue_Info->CmdRunningCount--;
        pCurrent_Queue_Info->CmdReadyToBeAcknCount++;
        
        /* set current RequestEntry to state "free" */
        EDDP_HAL_MEM32_WRITE(pDDB, pCurrent_Queue_Info->pCurrentConfPtr, EDDP_CMD_REQUEST_FREE);

        /* check if pCurrentConfPtr is at the end of the queue */
        if ( pCurrent_Queue_Info->pCurrentConfPtr == (pCurrent_Queue_Info->pQueueStartAddress + (pCurrent_Queue_Info->QueueLength - 1 )) )
        {   /* set pCurrentConfPtr to next confirmed element */
            pCurrent_Queue_Info->pCurrentConfPtr = pCurrent_Queue_Info->pQueueStartAddress;
        }
        else
        {   /* set pCurrentConfPtr to next confirmed element */
            pCurrent_Queue_Info->pCurrentConfPtr++;
        }
    }
    
    EDDP_CMD_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] _EDDP_CMD_FinishCrtCommandQueue(QueueIndex=%d): running=%d, ready=%d, sequence-count=%d", 
        QueueIndex, pCurrent_Queue_Info->CmdRunningCount, pCurrent_Queue_Info->CmdReadyToBeAcknCount, pCurrent_Queue_Info->CmdRunSeqCount);
    
    /* nothing to confirm, quit for now */
    if( 0 == pCurrent_Queue_Info->CmdReadyToBeAcknCount )
    {
        EDDP_CMD_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] _EDDP_CMD_FinishCrtCommandQueue(QueueIndex=%d): Nothing to confirm!", QueueIndex);
    }
    else
    {
        /* sequence is complete, confirm commands */
        if( 0 == pCurrent_Queue_Info->CmdRunningCount )
        {            
            EDDP_ASSERT(pCurrent_Queue_Info->CmdRunSeqCount == pCurrent_Queue_Info->CmdReadyToBeAcknCount);
            
            while(pCurrent_Queue_Info->CmdReadyToBeAcknCount)
            {                
                pCurrent_Queue_Info->CmdReadyToBeAcknCount--;
                
                /* get stored pParam structure from "confirmation" queue */
                pParam = (EDDP_CMD_PARAM_PTR_TYPE)EDDP_GlbQueueRemoveFromBegin(&(pCurrent_Queue_Info->QueueConf_pParam));
                pParam->InUse = LSA_FALSE;
                
                /* sanity check, command must be a CRT command */
                EDDP_ASSERT(EDDP_CMD_CPM_NOP == (EDDP_CMD_GROUP_MASK & pParam->CmdOpcode));
                
                EDDP_CMD_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] _EDDP_CMD_FinishCrtCommandQueue(QueueIndex=%d): Callback for command (CmdOpcode=0x%X, pParam=0x%X) called. Remaining command confirmation count=%d", 
                    QueueIndex, pParam->CmdOpcode, pParam, pCurrent_Queue_Info->CmdReadyToBeAcknCount);
                
                /* call callback function to notify user that the command is done */
                pParam->Cbf(pParam->pDDB, pParam, pParam->Response);
                                
                /* free cmd param element */
                EDDP_CRTCmdReleaseInternalCmdParamElement(pDDB);
            }
                        
            /* sequence is done */
            pCurrent_Queue_Info->CmdRunSeqCount        = 0;
            pCurrent_Queue_Info->CmdReadyToBeAcknCount = 0;
        }
        /* sequence not complete */
        else
        {
            /* NOTE: dont confirm any RQB of an incomplete RQB/command sequence */
            EDDP_CMD_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] _EDDP_CMD_FinishCrtCommandQueue(QueueIndex=%d): Command sequence not yet finished! Wait for remaining commands. Quit for now.", QueueIndex);
        }
    }
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_CMD_ReadCommandReturnValue            +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             EDDP_CMD_PARAM_PTR_TYPE        pParam       +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB        : Pointer to DeviceDescriptionBlock                  (in)  +*/
/*+  pParam      : Pointer to command parameter structure             (in)  +*/
/*+                                                                         +*/
/*+  Results     :   EDD_STS_OK                                             +*/
/*+              :   EDD_STS_ERR_RESOURCE                                   +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function reads the command result for the selected   +*/
/*+				  command from the corresponding CBL (WCRC command) and     +*/
/*+               writes it to the pParam struct. If the command does not   +*/
/*+               have any result nothing is done.                          +*/
/*+               The function returns EDD_STS_ERR_RESOURCE if something    +*/
/*+               went wrong when executing the command.                    +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP _EDDP_CMD_ReadCommandReturnValue(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_CMD_PARAM_PTR_TYPE     pParam)
{
    EDD_RSP                     Response;
    LSA_UINT32                  reg_val;
    LSA_BOOL                    isAsync;
    EDD_COMMON_MEM_PTR_TYPE     cbl_address;
    EDDP_CMD_COMMAND_TYPE*      pCommandBlock;

    Response = EDD_STS_OK;
    isAsync = LSA_TRUE;
    pCommandBlock = LSA_NULL;

    if (pParam->Cbf == 0)
    {
        // get addresses for synchronous command processing
        cbl_address   = EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_COMMAND_HOST_SYNC, 0);
        pCommandBlock = (EDDP_CMD_COMMAND_TYPE*)&(((EDDP_CMD_RAM_COMMAND_BLOCK_HOST_SYNC_TYPE*)cbl_address)->Generic.Generic[0]);
        
        isAsync = LSA_FALSE;
    }

    switch (pParam->CmdOpcode)
    {
        /* ------------------------------------------------ */
        /* Filter                                           */
        /* ------------------------------------------------ */
        case EDDP_CMD_FILTER_STOP:
        {
            if(isAsync)
            {
                // get offset address of CBLs belonging to this queue
                cbl_address = EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_COMMAND_HOST_ASYNC, 0);
                pCommandBlock = &(((EDDP_CMD_RAM_COMMAND_BLOCKS_HOST_ASYNC_PTR_TYPE)cbl_address)->Filter_Stop);
            }
            
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[0].Low_32Bit));
            if ( (reg_val & PNIP_REG_APIFILTERCOMMAND_IF_STATUS__MSK_CONFERROR) == (0x00000001/*Error*/ << PNIP_REG_APIFILTERCOMMAND_IF_STATUS_SHFT_CONFERROR) )
            {   
                EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_FILTER_STOP: Error in return code !!! ");
                Response = EDD_STS_ERR_PARAM;
            }
        }
        break;

        /* ------------------------------------------------ */
        /* RcvDMACW                                         */
        /* ------------------------------------------------ */

        /* ------------------------------------------------ */
        /* SndDMACW                                         */
        /* ------------------------------------------------ */

        /* ------------------------------------------------ */
        /* CPM                                              */
        /* ------------------------------------------------ */
        case EDDP_CMD_CPM_NOP:
            /* TBD */
        break;

        case EDDP_CMD_CPM_ON_OFF:
        {
            if(isAsync)
            {
                // get offset address of CBLs belonging to this queue
                pCommandBlock = pParam->param_union.Crt.pCBL;
            }
            
            // evaluate error bit of CBL
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[0].High_32Bit));
            if (reg_val & EDDP_CMD_CBL_ERROR_MASK)
            {
                EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_CPM_ON_OFF: Error at invoking command! ");
                Response = EDD_STS_ERR_PARAM;    
            }

            // evaluate return value of WCRC command
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[0].Low_32Bit));
            if (reg_val & EDDP_CMD_IF_STATUS_CONFERROR_MASK)
            {
                reg_val = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_CPMERRSTATUS);
                EDDP_CMD_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_CPM_ON_OFF: Error at running command! CPMErrStatus: 0x%x", reg_val);
                Response = EDD_STS_ERR_PARAM;
            }
        }
        break;

        case EDDP_CMD_CPM_ACW_ON_OFF:
        {
            if(isAsync)
            {
                // get offset address of CBLs belonging to this queue
                pCommandBlock = pParam->param_union.Crt.pCBL;
            }
            
            // evaluate error bit of CBL
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[0].High_32Bit));
            if (reg_val & EDDP_CMD_CBL_ERROR_MASK)
            {
                EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_CPM_ACW_ON_OFF: Error at invoking command! ");
                Response = EDD_STS_ERR_PARAM;    
            }

            // evaluate return value of WCRC command
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[0].Low_32Bit));
            if (reg_val & EDDP_CMD_IF_STATUS_CONFERROR_MASK)
            {
                reg_val = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_CPMERRSTATUS);
                EDDP_CMD_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_CPM_ACW_ON_OFF: Error at running command! CPMErrStatus: 0x%x", reg_val);
                Response = EDD_STS_ERR_PARAM;
            }
        }
        break;

        case EDDP_CMD_CPM_ACW_IN_OUT:
        {
            if(isAsync)
            {
                // get offset address of CBLs belonging to this queue
                pCommandBlock = pParam->param_union.Crt.pCBL;
            }
            
            // evaluate error bit of first CBL
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[0].High_32Bit));
            if (reg_val & EDDP_CMD_CBL_ERROR_MASK)
            {
                EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_CPM_ACW_IN_OUT: Error at copying paramstruct! ");
                Response = EDD_STS_ERR_PARAM;    
            }

            // evaluate error bit of second CBL
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[0].High_32Bit));
            if (reg_val & EDDP_CMD_CBL_ERROR_MASK)
            {
                EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_CPM_ACW_IN_OUT: Error at invoking command!");
                Response = EDD_STS_ERR_PARAM;    
            }

            // evaluate return value of WCRC command
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[1].Low_32Bit));
            if (reg_val & EDDP_CMD_IF_STATUS_CONFERROR_MASK)
            {
                reg_val = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_CPMERRSTATUS);
                EDDP_CMD_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_CPM_ACW_IN_OUT: Error at running command! CPMErrStatus: 0x%x", reg_val);
                Response = EDD_STS_ERR_PARAM;
            }
        }
        break;

        case EDDP_CMD_CPM_ACW_SET_UNKNOWN:
        {
            if(isAsync)
            {
                // get offset address of CBLs belonging to this queue
                pCommandBlock = pParam->param_union.Crt.pCBL;
            }
            
            // evaluate error bit of CBL
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[0].High_32Bit));
            if (reg_val & EDDP_CMD_CBL_ERROR_MASK)
            {
                EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_CPM_ACW_SET_UNKNOWN: Error at invoking command! ");
                Response = EDD_STS_ERR_PARAM;    
            } 

            // evaluate return value of WCRC command
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[0].Low_32Bit));
            if (reg_val & EDDP_CMD_IF_STATUS_CONFERROR_MASK)
            {
                reg_val = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_CPMERRSTATUS);
                EDDP_CMD_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_CPM_ACW_SETUNKNOWN: Error at running command! CPMErrStatus: 0x%x", reg_val);
                Response = EDD_STS_ERR_PARAM;
            }
        }
        break;

        case EDDP_CMD_CPM_ACW_XCHANGE_BUFF:
            /* TBD */
        break;

        /* ------------------------------------------------ */
        /* PPM                                              */
        /* ------------------------------------------------ */
        case EDDP_CMD_PPM_NOP:
            /* TBD */
        break;

        case EDDP_CMD_PPM_ON_OFF:
        {
            if(isAsync)
            {
                // get offset address of CBLs belonging to this queue
                pCommandBlock = pParam->param_union.Crt.pCBL;
            }
            
            // evaluate error bit of CBL
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[0].High_32Bit));
            if (reg_val & EDDP_CMD_CBL_ERROR_MASK)
            {
                EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_PPM_ON_OFF: Error at invoking command! ");
                Response = EDD_STS_ERR_PARAM;    
            } 

            // evaluate return value of WCRC command
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[0].Low_32Bit));
            if (reg_val & EDDP_CMD_IF_STATUS_CONFERROR_MASK)
            {
                /* There is a macro that checks PNIP_R2_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE and PNIP_R3_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE has the same value. */
                /* Checked in eddp_hal_pnip_val.h. We use the R2 version. */
                reg_val = EDDP_HAL_REG32_READ(pDDB, PNIP_R2_REG_PPMERRSTATUS);
                EDDP_CMD_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_PPM_ON_OFF: Error at running command! PPMErrStatus: 0x%x, EDDP_HALGetPnipRevision(pDDB): 0x%x", 
                    reg_val, EDDP_HALGetPnipRevision(pDDB));
                Response = EDD_STS_ERR_PARAM;
            }
        }
        break;

        case EDDP_CMD_PPM_ACW_ON_OFF:
        {
            if(isAsync)
            {
                // get offset address of CBLs belonging to this queue
                pCommandBlock = pParam->param_union.Crt.pCBL;
            }
            
            // evaluate error bit of CBL
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[0].High_32Bit));
            if (reg_val & EDDP_CMD_CBL_ERROR_MASK)
            {
                EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_PPM_ACW_ON_OFF: Error at invoking command! ");
                Response = EDD_STS_ERR_PARAM;    
            } 

            // evaluate return value of WCRC command
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[0].Low_32Bit));
            if (reg_val & EDDP_CMD_IF_STATUS_CONFERROR_MASK)
            {
                /* There is a macro that checks PNIP_R2_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE and PNIP_R3_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE has the same value. */
                /* Checked in eddp_hal_pnip_val.h. We use the R2 version. */
                reg_val = EDDP_HAL_REG32_READ(pDDB, PNIP_R2_REG_PPMERRSTATUS);
                EDDP_CMD_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_PPM_ACW_ON_OFF: Error at running command! PPMErrStatus: 0x%x, EDDP_HALGetPnipRevision(pDDB): 0x%x", 
                    reg_val, EDDP_HALGetPnipRevision(pDDB));
                Response = EDD_STS_ERR_PARAM;
            }
        }
        break;

        case EDDP_CMD_PPM_ACW_IN_OUT:
        {
            if(isAsync)
            {
                // get offset address of CBLs belonging to this queue
                pCommandBlock = pParam->param_union.Crt.pCBL;
            }
            
            // evaluate error bit of first CBL
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[0].Low_32Bit));
            if (reg_val & EDDP_CMD_CBL_ERROR_MASK)
            {
                EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_PPM_ACW_IN_OUT: Error at copying paramstruct! ");
                Response = EDD_STS_ERR_PARAM;    
            }

            // evaluate error bit of second CBL
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[0].High_32Bit));
            if (reg_val & EDDP_CMD_CBL_ERROR_MASK)
            {
                EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_PPM_ACW_IN_OUT: Error at invoking command!");
                Response = EDD_STS_ERR_PARAM;    
            }

            // evaluate return value of WCRC command
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[1].Low_32Bit));
            if (reg_val & EDDP_CMD_IF_STATUS_CONFERROR_MASK)
            {
                /* There is a macro that checks PNIP_R2_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE and PNIP_R3_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE has the same value. */
                /* Checked in eddp_hal_pnip_val.h. We use the R2 version. */
                reg_val = EDDP_HAL_REG32_READ(pDDB, PNIP_R2_REG_PPMERRSTATUS);
                EDDP_CMD_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_PPM_ACW_IN_OUT: Error at running command! PPMErrStatus: 0x%x, EDDP_HALGetPnipRevision(pDDB): 0x%x",
                    reg_val, EDDP_HALGetPnipRevision(pDDB));
                Response = EDD_STS_ERR_PARAM;
            }
        }
        break;
        
        case EDDP_CMD_PPM_ACW_DELAY:
        {
            if(isAsync)
            {
                // get offset address of CBLs belonging to this queue
                pCommandBlock = pParam->param_union.Crt.pCBL;
            }
            
            // evaluate error bit of first CBL
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[0].Low_32Bit));
            if (reg_val & EDDP_CMD_CBL_ERROR_MASK)
            {
                EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_PPM_ACW_IN_OUT_DELAY: Error at copying paramstruct! ");
                Response = EDD_STS_ERR_PARAM;    
            }

            // evaluate error bit of second CBL
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[0].High_32Bit));
            if (reg_val & EDDP_CMD_CBL_ERROR_MASK)
            {
                EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_PPM_ACW_IN_OUT_DELAY: Error at invoking command!");
                Response = EDD_STS_ERR_PARAM;    
            }

            // evaluate return value of WCRC command
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[1].Low_32Bit));
            if (reg_val & EDDP_CMD_IF_STATUS_CONFERROR_MASK)
            {
                /* There is a macro that checks PNIP_R2_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE and PNIP_R3_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE has the same value. */
                /* Checked in eddp_hal_pnip_val.h. We use the R2 version. */
                reg_val = EDDP_HAL_REG32_READ(pDDB, PNIP_R2_REG_PPMERRSTATUS);
                EDDP_CMD_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_PPM_ACW_IN_OUT_DELAY: Error at running command! PPMErrStatus: 0x%x, EDDP_HALGetPnipRevision(pDDB): 0x%x",
                    reg_val, EDDP_HALGetPnipRevision(pDDB));
                Response = EDD_STS_ERR_PARAM;
            }
        }
        break;        

        case EDDP_CMD_PPM_ACW_MOVE:
        {
            if(isAsync)
            {
                // get offset address of CBLs belonging to this queue
                pCommandBlock = pParam->param_union.Crt.pCBL;
            }
            
            // evaluate error bit of first CBL
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[0].Low_32Bit));
            if (reg_val & EDDP_CMD_CBL_ERROR_MASK)
            {
                EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_PPM_ACW_MOVE: Error at copying paramstruct! ");
                Response = EDD_STS_ERR_PARAM;    
            }

            // evaluate error bit of second CBL
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[0].High_32Bit));
            if (reg_val & EDDP_CMD_CBL_ERROR_MASK)
            {
                EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_PPM_ACW_MOVE: Error at invoking command!");
                Response = EDD_STS_ERR_PARAM;    
            }

            // evaluate return value of WCRC command
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[1].Low_32Bit));
            if (reg_val & EDDP_CMD_IF_STATUS_CONFERROR_MASK)
            {
                /* There is a macro that checks PNIP_R2_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE and PNIP_R3_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE has the same value. */
                /* Checked in eddp_hal_pnip_val.h. We use the R2 version. */
                reg_val = EDDP_HAL_REG32_READ(pDDB, PNIP_R2_REG_PPMERRSTATUS);
                EDDP_CMD_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_PPM_ACW_MOVE: Error at running command! PPMErrStatus: 0x%x, EDDP_HALGetPnipRevision(pDDB): 0x%x",
                    reg_val, EDDP_HALGetPnipRevision(pDDB));
                Response = EDD_STS_ERR_PARAM;
            }
        }
        break;

        case EDDP_CMD_PPM_ACW_XCHANGE_BUFF:
            /* TBD */
        break;

        /* ------------------------------------------------ */
        /* Pack                                             */
        /* ------------------------------------------------ */
        case EDDP_CMD_PACK_NOP:
            /* TBD */
        break;

        case EDDP_CMD_PACK_DISABLE_GRP:
        {
            if(isAsync)
            {
                // get offset address of CBLs belonging to this queue
                cbl_address = EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_COMMAND_HOST_ASYNC, 0);
                pCommandBlock = &(((EDDP_CMD_RAM_COMMAND_BLOCKS_HOST_ASYNC_PTR_TYPE)cbl_address)->Pack_DisableGrp);
            }
            
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[0].Low_32Bit));
            if ( (reg_val & PNIP_REG_PACKCOMMAND_IF_STATUS__MSK_CONFERROR) == (0x00000001/*Error*/ << PNIP_REG_PACKCOMMAND_IF_STATUS_SHFT_CONFERROR) )
            {   
                EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_PACK_xxx_GRP: Error in return code !!! ");
                Response = EDD_STS_ERR_PARAM;
            }
        }
        break;
        
        case EDDP_CMD_PACK_ENABLE_GRP:
        {
            if(isAsync)
            {
                // get offset address of CBLs belonging to this queue
                cbl_address = EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_COMMAND_HOST_ASYNC, 0);
                pCommandBlock = &(((EDDP_CMD_RAM_COMMAND_BLOCKS_HOST_ASYNC_PTR_TYPE)cbl_address)->Pack_EnableGrp);
            }
            
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[0].Low_32Bit));
            if ( (reg_val & PNIP_REG_PACKCOMMAND_IF_STATUS__MSK_CONFERROR) == (0x00000001/*Error*/ << PNIP_REG_PACKCOMMAND_IF_STATUS_SHFT_CONFERROR) )
            {   
                EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_PACK_xxx_GRP: Error in return code !!! ");
                Response = EDD_STS_ERR_PARAM;
            }
        }
        break;
        
        case EDDP_CMD_PACK_DIS_ENA_GRP:
        {
            if(isAsync)
            {
                // get offset address of CBLs belonging to this queue
                cbl_address = EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_COMMAND_HOST_ASYNC, 0);
                pCommandBlock = &(((EDDP_CMD_RAM_COMMAND_BLOCKS_HOST_ASYNC_PTR_TYPE)cbl_address)->Pack_DisableEnableGrp);
            }
            
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[0].Low_32Bit));
            if ( (reg_val & PNIP_REG_PACKCOMMAND_IF_STATUS__MSK_CONFERROR) == (0x00000001/*Error*/ << PNIP_REG_PACKCOMMAND_IF_STATUS_SHFT_CONFERROR) )
            {   
                EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_PACK_xxx_GRP: Error in return code !!! ");
                Response = EDD_STS_ERR_PARAM;
            }
        }
        break;

        /* ------------------------------------------------ */
        /* FDB                                              */
        /* ------------------------------------------------ */
        case EDDP_CMD_FDB_CLEAR_UC_TABLE:
            // there is no return value for this command -> do nothing
        break;

        case EDDP_CMD_FDB_INSERT_UC_ENTRY:
        {
            if(isAsync)
            {
                // get offset address of CBLs belonging to this queue
                cbl_address = EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_COMMAND_HOST_ASYNC, 0);
                pCommandBlock = &(((EDDP_CMD_RAM_COMMAND_BLOCKS_HOST_ASYNC_PTR_TYPE)cbl_address)->Fdb_InsertUcEntry[0]);
            }
            
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[1].Low_32Bit));
            pParam->param_union.Fdb.InsertUCEntry.Result = (reg_val & PNIP_REG_FDBCOMMAND_IF_STATUS__MSK_RETURNVALUE) >> PNIP_REG_FDBCOMMAND_IF_STATUS_SHFT_RETURNVALUE;
            if (pParam->param_union.Fdb.InsertUCEntry.Result == EDDP_CMD_SYNC_RETURNVALUE_INSERT_UC_ENTRY_ERROR)
            {
                EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_FDB_INSERT_UC_ENTRY: Error detected in result code! ");
                Response = EDD_STS_ERR_PARAM;
            }
        }
        break;

        /* ------------------------------------------------ */
        /* VLAN                                             */
        /* ------------------------------------------------ */

        /* ------------------------------------------------ */
        /* SYNC                                             */
        /* ------------------------------------------------ */
        case EDDP_CMD_GSYNC_UPDATE_COMCYCL:
        {
            if(isAsync)
            {
                // get offset address of CBLs belonging to this queue
                cbl_address = EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_COMMAND_HOST_ASYNC, 0);
                pCommandBlock = &(((EDDP_CMD_RAM_COMMAND_BLOCKS_HOST_ASYNC_PTR_TYPE)cbl_address)->Gsync_UpdateComCycl[0]);
            }
            
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[1].Low_32Bit));
            if ( ((reg_val & PNIP_REG_SYNCCOMMAND_IF_STATUS__MSK_CONFERROR) >> PNIP_REG_SYNCCOMMAND_IF_STATUS_SHFT_CONFERROR) == PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_CONFERROR_ERROR )
            {
                EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_GSYNC_UPDATE_COMCYCL: Error in return code !!!");
                Response = EDD_STS_ERR_PARAM;
            }
        }
        break;

        case EDDP_CMD_GSYNC_CONNECT_SYNC_A:
        {
            if(isAsync)
            {
                // get offset address of CBLs belonging to this queue
                cbl_address = EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_COMMAND_HOST_ASYNC, 0);
                pCommandBlock = &(((EDDP_CMD_RAM_COMMAND_BLOCKS_HOST_ASYNC_PTR_TYPE)cbl_address)->Gsync_ConnectSyncA[0]);
            }
            
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[1].Low_32Bit));
            if ( ((reg_val & PNIP_REG_SYNCCOMMAND_IF_STATUS__MSK_CONFERROR) >> PNIP_REG_SYNCCOMMAND_IF_STATUS_SHFT_CONFERROR) == PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_CONFERROR_ERROR )
            {
                EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_GSYNC_CONNECT_SYNC_A: Error in return code !!!");
                Response = EDD_STS_ERR_PARAM;
            }
        }
        break;

        case EDDP_CMD_GSYNC_UPDATE_APPCYCL:
        {
            if(isAsync)
            {
                // get offset address of CBLs belonging to this queue
                cbl_address = EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_COMMAND_HOST_ASYNC, 0);
                pCommandBlock = &(((EDDP_CMD_RAM_COMMAND_BLOCKS_HOST_ASYNC_PTR_TYPE)cbl_address)->Gsync_UpdateAppCycl[0]);
            }
            
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[1].Low_32Bit));
            if ( ((reg_val & PNIP_REG_SYNCCOMMAND_IF_STATUS__MSK_CONFERROR) >> PNIP_REG_SYNCCOMMAND_IF_STATUS_SHFT_CONFERROR) == PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_CONFERROR_ERROR )
            {
                EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_GSYNC_UPDATE_APPCYCL: Error in return code !!!");
                Response = EDD_STS_ERR_PARAM;
            }
        }
        break;

        case EDDP_CMD_GSYNC_UPDATE_BASECYCL:
        {
            if(isAsync)
            {
                // get offset address of CBLs belonging to this queue
                cbl_address = EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_COMMAND_HOST_ASYNC, 0);
                pCommandBlock = &(((EDDP_CMD_RAM_COMMAND_BLOCKS_HOST_ASYNC_PTR_TYPE)cbl_address)->Gsync_UpdateBaseCycl[0]);
            }
            
            reg_val = EDDP_HAL_MEM32_READ(pDDB, &(pCommandBlock[1].Low_32Bit));
            if ( ((reg_val & PNIP_REG_SYNCCOMMAND_IF_STATUS__MSK_CONFERROR) >> PNIP_REG_SYNCCOMMAND_IF_STATUS_SHFT_CONFERROR) == PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_CONFERROR_ERROR )
            {
                EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> EDDP_CMD_GSYNC_UPDATE_BASECYCL: Error in return code !!!");
                Response = EDD_STS_ERR_PARAM;
            }
        }
        break;

        /* ------------------------------------------------ */
        /* else                                             */
        /* ------------------------------------------------ */
        default:
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_CMD_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_CMD_ReadCommandReturnValue(): ERROR -> Command Opcode not supported! CmdOpcode: 0x%x", 
                pParam->CmdOpcode);
        }
        break;
    }

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CMD_Is_Command_Running                 +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32					   CmdOpCode    +*/
/*+                                                                         +*/
/*+  Result                :    LSA_BOOL                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  CmdOpCode  : Command OpCode                                      (in)  +*/
/*+                                                                         +*/
/*+  Results     :   LSA_TRUE                                               +*/
/*+              :   LSA_FALSE                                              +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function checks if the specified command is currently+*/
/*+				  running on the hardware.                                  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CMD_Is_Command_Running(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  CmdOpCode)
{
    LSA_UINT32                      QueueIndex; 
    LSA_UINT32                      CommandNumber;
    LSA_BOOL                        bRunning;
    EDDP_CMD_QUEUE_INFO_PTR_TYPE    pCurrent_Queue_Info;

    if ( _EDDP_CMD_IsCmdOpcodeValid(CmdOpCode)!= EDD_STS_OK )
    {   // ***** CmdOpCode is no valid command -> we return 'not found'
        bRunning = LSA_FALSE;
    }
    else
    {
        QueueIndex          = ((CmdOpCode & EDDP_CMD_GROUP_MASK) >> 8);
        CommandNumber       = (CmdOpCode & EDDP_CMD_COMMAND_MASK);
        pCurrent_Queue_Info = &(pDDB->CMD.Queue[QueueIndex]);

        if ( pCurrent_Queue_Info->RunningCommand & (0x1 << CommandNumber) )
        {
            bRunning = LSA_TRUE;
        }
        else
        {
            bRunning = LSA_FALSE;
        }
    }
    
    return (bRunning);
}    


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CMD_ISR_ReqQueue_ERR                   +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32                     EventNr      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  EventNr    : Event Number (Interrupt Number)                     (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function is called by CORE from the                  +*/
/*+               interrupt "INT_ReqQueue_ERR".                             +*/
/*+               It is executed within LOW context.                        +*/
/*+                                                                         +*/
/*+               Register "PNIP_REG_IRQxEVENT_LOW_0"                       +*/
/*+               Interrupt event "INT_ReqQueue_ERR"                        +*/
/*+                 -> Bit(14): 0x00004000                                  +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CMD_ISR_ReqQueue_ERR(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr)
{
    LSA_UINT32      RegCurrentCmd;
    LSA_UINT32      RegQueueErrStatus;
    LSA_UINT32      RegQueueErrEvent;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(EventNr);
#endif

    RegCurrentCmd       = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_CURRENTCMD);
    RegQueueErrStatus   = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_QUEUEERRSTATUS);
    RegQueueErrEvent    = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_QUEUEERREVENT);

    EDDP_CMD_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CMD_ISR_ReqQueue_ERR(EventNr=0x%X): FATAL -> RegCurrentCmd=0x%X, RegQueueErrStatus=0x%X, RegQueueErrEvent=0x%X", 
        EventNr, RegCurrentCmd, RegQueueErrStatus, RegQueueErrEvent);
    EDDP_FATAL(EDDP_FATAL_ERR_UNEXP_INTERRUPT);
}

/*****************************************************************************/
/*  end of file eddp_cmd.c                                                   */
/*****************************************************************************/
