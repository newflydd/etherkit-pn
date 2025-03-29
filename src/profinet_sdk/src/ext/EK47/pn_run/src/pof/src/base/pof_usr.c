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
/*  C o m p o n e n t     &C: POF (Polymer Optical Fiber)               :C&  */
/*                                                                           */
/*  F i l e               &F: pof_usr.c                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  POF user-interface functions with RQBs           */
/*                                                                           */
/*                          - pof_open_channel()                             */
/*                          - pof_request()                                  */
/*                          - pof_close_channel()                            */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
/*****************************************************************************/

/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/
#define LTRC_ACT_MODUL_ID  1
#define POF_MODULE_ID      LTRC_ACT_MODUL_ID

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/
#include "pof_inc.h"            /* POF headerfiles */
#include "pof_dmi_type.h"
#include "pof_prm_type.h"
#include "pof_int.h"            /* internal header */
#include "pof_prm_req.h"
#include "pof_dmi.h"
#include "pof_prm_state_diag.h"
#include "pof_timer.h"
#include "pof_glb.h"
#include "pof_edd.h"
#include "pof_prm_state.h"

POF_FILE_SYSTEM_EXTENSION(POF_MODULE_ID)

/*===========================================================================*/
/*                                define                                     */
/*===========================================================================*/

/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/

/*===========================================================================*/
/*                                global Variables                           */
/*===========================================================================*/

/*===========================================================================*/
/*                            local function declaration                     */
/*===========================================================================*/
static POF_RSP POF_LOCAL_FCT_ATTR   POF_RequestTestHardware( POF_UPPER_RQB_PTR_TYPE const pRQB,
                                                             POF_HDB_PTR_TYPE       const pHDB );

static POF_RSP POF_LOCAL_FCT_ATTR   POF_RequestSetRawHardware( POF_UPPER_RQB_PTR_TYPE const pRQB,
                                                               POF_HDB_PTR_TYPE       const pHDB );

static POF_RSP POF_LOCAL_FCT_ATTR   POF_RequestGetRawHardware( POF_UPPER_RQB_PTR_TYPE const pRQB,
                                                               POF_HDB_PTR_TYPE       const pHDB );

static POF_RSP POF_LOCAL_FCT_ATTR   POF_RequestSetDiagnostic( POF_UPPER_RQB_PTR_TYPE const pRQB,
                                                              POF_HDB_PTR_TYPE       const pHDB );

static POF_RSP POF_LOCAL_FCT_ATTR   POF_RequestGetParams( POF_UPPER_RQB_PTR_TYPE const pRQB,
                                                          POF_HDB_PTR_TYPE       const pHDB );

static POF_RSP POF_LOCAL_FCT_ATTR   POF_UserGetFreeHDB( POF_HDB_PTR_TYPE POF_LOCAL_MEM_ATTR * const ppHDB );

static LSA_VOID POF_LOCAL_FCT_ATTR  POF_UserReleaseHDB( POF_HDB_PTR_TYPE const pHDB );

static LSA_VOID POF_LOCAL_FCT_ATTR  POF_State_Open_Channel_Edd_Running( POF_COMMON_MEM_BOOL_PTR_TYPE const pbCallCbf,
                                                                        POF_COMMON_MEM_BOOL_PTR_TYPE const pbReleasePath,
                                                                        POF_UPPER_RQB_PTR_TYPE       const pRQB,
                                                                        POF_HDB_PTR_TYPE             const pHDB, 
                                                                        POF_LOCAL_RSP_PTR_TYPE       const pResponse );

static LSA_VOID POF_LOCAL_FCT_ATTR  POF_State_Get_Params_Edd_Running( POF_UPPER_RQB_PTR_TYPE const pRQB,
                                                                      POF_HDB_PTR_TYPE       const pHDB, 
                                                                      POF_LOCAL_RSP_PTR_TYPE const pResponse );

static LSA_VOID POF_LOCAL_FCT_ATTR  POF_State_Get_PortParams_Edd_Running( POF_COMMON_MEM_BOOL_PTR_TYPE const pbCallCbf, 
                                                                          POF_UPPER_RQB_PTR_TYPE       const pRQB,
                                                                          POF_HDB_PTR_TYPE             const pHDB, 
                                                                          POF_LOCAL_RSP_PTR_TYPE       const pResponse );

static LSA_VOID POF_LOCAL_FCT_ATTR  POF_State_Close_Error_Edd_Running( POF_COMMON_MEM_BOOL_PTR_TYPE const pbCallCbf, 
                                                                       POF_COMMON_MEM_BOOL_PTR_TYPE const pbReleasePath,
                                                                       POF_HDB_PTR_TYPE             const pHDB, 
                                                                       POF_LOCAL_RSP_PTR_TYPE       const pResponse );

static LSA_VOID POF_LOCAL_FCT_ATTR  POF_State_Close_Wait_Timer( POF_COMMON_MEM_BOOL_PTR_TYPE const pbCallCbf, 
                                                                POF_HDB_PTR_TYPE             const pHDB, 
                                                                POF_LOCAL_RSP_PTR_TYPE       const pResponse );

static LSA_VOID POF_LOCAL_FCT_ATTR  POF_State_Close_Channel_Edd_Running( POF_COMMON_MEM_BOOL_PTR_TYPE const pbCallCbf, 
                                                                         POF_COMMON_MEM_BOOL_PTR_TYPE const pbReleasePath,
                                                                         POF_UPPER_RQB_PTR_TYPE       const pRQB,
                                                                         POF_HDB_PTR_TYPE             const pHDB, 
                                                                   const POF_LOCAL_RSP_PTR_TYPE       const pResponse );

static POF_RSP POF_LOCAL_FCT_ATTR   pof_open_ch_intern( POF_COMMON_MEM_BOOL_PTR_TYPE   const pbCallCbf,
                                                        POF_COMMON_MEM_BOOL_PTR_TYPE   const pbReleasePath,
                                                        POF_UPPER_RQB_PTR_TYPE         const pRQB,
                                                        LSA_SYS_PTR_TYPE             * const ppSys,
                                                        POF_DETAIL_PTR_TYPE          * const ppDetail );

static LSA_VOID POF_LOCAL_FCT_ATTR  POF_UserCloseChannel( POF_HDB_PTR_TYPE const pHDB );

static LSA_VOID POF_LOCAL_FCT_ATTR  POF_State_Error_CloseEDDChannel( POF_HDB_PTR_TYPE       const pHDB,
                                                               const POF_LOCAL_RSP_PTR_TYPE const pResponse );

static LSA_VOID POF_LOCAL_FCT_ATTR  POF_RequestTimer( POF_UPPER_RQB_PTR_TYPE const pRQB );

/*===========================================================================*/
/*                                 Macros                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                 functions                                 */
/*===========================================================================*/

/*===========================================================================*/
/*                                 Macros                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                 functions                                 */
/*===========================================================================*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_UserGetFreeHDB()                        +*/
/*+  Input/Output          :    POF_HDB_PTR_TYPE POF_LOCAL..pHDBUser        +*/
/*+  Result                :    POF_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  Result     : POF_RSP_OK                                                +*/
/*+               POF_RSP_ERR_RESOURCE                                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Gets Pointer to a free User-HDB                           +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static  POF_RSP  POF_LOCAL_FCT_ATTR  POF_UserGetFreeHDB( POF_HDB_PTR_TYPE  POF_LOCAL_MEM_ATTR  *  const  ppHDB )
{
  LSA_UINT32        Ctr;
  POF_RSP           Status = POF_RSP_OK;
  LSA_BOOL          bFound = LSA_FALSE;
  POF_HDB_PTR_TYPE  pHDBTemp;

  POF_FUNCTION_TRACE_00(0, LSA_TRACE_LEVEL_CHAT, "IN: POF_UserGetFreeHDB");

  /* --------------------------------------------------------------------------*/
  /* Search for a free handle-management                                       */
  /* --------------------------------------------------------------------------*/
  for (Ctr = (LSA_UINT32)POF_CFG_MAX_CHANNELS, pHDBTemp = POF_MGM_GET_HUSER_PTR()->HDB;
       Ctr;
       Ctr--, pHDBTemp++)
  {
    if (pHDBTemp->State == POF_HANDLE_STATE_FREE)
    {
      bFound = LSA_TRUE;
      break; //leave loop
    }
  }

  /* --------------------------------------------------------------------------*/
  /* If a free handle management was found, we initialize it.                  */
  /* --------------------------------------------------------------------------*/
  if (bFound)
  {
    /* save thishandle before clearing memory */
    LSA_HANDLE_TYPE  const  Help = pHDBTemp->ThisHandle;

    /* initialize HDB-memory with 0 */
    POF_MEMSET_LOCAL(pHDBTemp, (LSA_UINT8)0, (LSA_UINT32)sizeof(POF_HDB_TYPE));

    /* restore/set some values */
    pHDBTemp->ThisHandle = Help;

    POF_MGM_GET_HUSER_PTR()->UsedHandleCnt++;

    pHDBTemp->State = POF_HANDLE_STATE_CLOSED;
    *ppHDB = pHDBTemp;
  }
  else
  {
    Status = POF_RSP_ERR_RESOURCE;
    POF_PROGRAM_TRACE_00(0, LSA_TRACE_LEVEL_NOTE_HIGH, "POF_UserGetFreeHDB, no more free handle (HDB)");
  }
  
  POF_FUNCTION_TRACE_01(0, LSA_TRACE_LEVEL_CHAT, "OUT: POF_UserGetFreeHDB, Status:0x%X", Status);
  
  return Status;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_UserReleaseHDB()                        +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pHDB       : Pointer to User Handle to release                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Release User-HDB (no dealloc)                             +*/
/*+               Release memory for port management                        +*/
/*+               Sets Handle-state to POF_HANDLE_STATE_FREE                +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static  LSA_VOID  POF_LOCAL_FCT_ATTR  POF_UserReleaseHDB( POF_HDB_PTR_TYPE  const  pHDB )
{
  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_UserReleaseHDB, pHDB:0x%X", pHDB);

  POF_CHECK_NULL_PTR(pHDB);

  POF_EDDFreePortMgm(pHDB);

  pHDB->State = POF_HANDLE_STATE_FREE;

  POF_MGM_GET_HUSER_PTR()->UsedHandleCnt--;
  
  POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_UserReleaseHDB");
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_CylicTimerCbf()                         +*/
/*+  Input                      ...                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pIntRQB    : Pointer to internal RQB-mgm.                              +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Handles Timerevent for Cylcic Timer.                      +*/
/*+               Called by POF-Timerhandler                                +*/
/*+                                                                         +*/
/*+               It can be configured if we should call the output macro   +*/
/*+               POF_DO_TIMER_REQUEST for doing a context change or if     +*/
/*+               we should do the handling inside the function.            +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID  POF_LOCAL_FCT_ATTR  POF_CylicTimerCbf( POF_INTERNAL_REQUEST_PTR_TYPE  pIntRQB )
{
    if (!pIntRQB->bInUse)
    {
        /* Note that the RQB is already filled in startup! */
        pIntRQB->bInUse = LSA_TRUE;

        POF_DO_TIMER_REQUEST(pIntRQB->pRQB);
    }
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_State_Error_CloseEDDChannel()           +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pHDB       : Pointer to HDB to handle Channel request for              +*/
/*+  Response   : Responsecode of actual action taken (depends on state)    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static  LSA_VOID  POF_LOCAL_FCT_ATTR  POF_State_Error_CloseEDDChannel( POF_HDB_PTR_TYPE        const  pHDB,
                                                                 const POF_LOCAL_RSP_PTR_TYPE  const  pResponse )
{
  pHDB->OpenResponse = *pResponse; //save response for later

  POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "POF_State_Error_CloseEDDChannel, failed");

  pHDB->State = POF_HANDLE_STATE_CLOSE_ERROR_EDD_RUNNING;

  POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "EDD-Closing Channel because of error");

  if (POF_EDDCloseChannel(pHDB) != POF_RSP_OK)
  {
    POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "Cannot Close EDD Channel, Handle:0x%X", pHDB->ThisHandle);
    POF_FATAL("POF_State_Error_CloseEDDChannel", POF_FATAL_ERR_EDD, 0, 0);
    //lint -unreachable
    return;
  }
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_State_Open_Channel_Edd_Running()        +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pHDB       : Pointer to HDB to handle Channel request for              +*/
/*+  Response   : Responsecode of actual action taken (depends on state)    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  D e s c r i p t i o n:     *pbCallCbf and *pbReleasePath is presetted  +*/
/*+                             with LSA_FALSE outside                      +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static  LSA_VOID  POF_LOCAL_FCT_ATTR  POF_State_Open_Channel_Edd_Running( POF_COMMON_MEM_BOOL_PTR_TYPE  const  pbCallCbf,
                                                                          POF_COMMON_MEM_BOOL_PTR_TYPE  const  pbReleasePath,
                                                                          POF_UPPER_RQB_PTR_TYPE        const  pRQB,
                                                                          POF_HDB_PTR_TYPE              const  pHDB, 
                                                                          POF_LOCAL_RSP_PTR_TYPE        const  pResponse )
{
  #if (POF_CFG_TRACE_MODE == 0)
  LSA_UNUSED_ARG(pRQB);
  #endif

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_State_Open_Channel_Edd_Running, pRQB:0x%X", pRQB);

  if (*pResponse != POF_RSP_OK)
  {
    POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "POF_State_Open_Channel_Edd_Running, failed, Response:0x%X", *pResponse);
  
    POF_SYSTEM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_State_Open_Channel_Edd_Running, starting releasing path");
    *pbReleasePath = LSA_TRUE;
    *pbCallCbf     = LSA_TRUE;
    return;
  }

  /* Get MAC-Address and PortCnt from EDD */
  pHDB->State = POF_HANDLE_STATE_GET_PARAMS_EDD_RUNNING;

  *pResponse = POF_EDDGetParams(pHDB);

  if (*pResponse != POF_RSP_OK)
  {
    POF_State_Error_CloseEDDChannel(pHDB, pResponse);
  }

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_State_Open_Channel_Edd_Running, pRQB:0x%X", pRQB);
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_State_Get_Params_Edd_Running()          +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pHDB       : Pointer to HDB to handle Channel request for              +*/
/*+  Response   : Responsecode of actual action taken (depends on state)    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static  LSA_VOID  POF_LOCAL_FCT_ATTR  POF_State_Get_Params_Edd_Running( POF_UPPER_RQB_PTR_TYPE  const  pRQB,
                                                                        POF_HDB_PTR_TYPE        const  pHDB, 
                                                                        POF_LOCAL_RSP_PTR_TYPE  const  pResponse )
{
  #if (POF_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pRQB);
  #endif
  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_State_Get_Params_Edd_Running, pRQB:0x%X", pRQB);

  if (*pResponse != POF_RSP_OK)
  {
    POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "POF_State_Get_Params_Edd_Running, failed");
  
    POF_State_Error_CloseEDDChannel(pHDB, pResponse);
    return;
  }

  if ((pHDB->PortCnt == 0) || (pHDB->PortCnt > POF_PORT_CNT_MAX))
  {
    *pResponse = POF_RSP_ERR_LL;           

    POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "POF_State_Get_Params_Edd_Running, PortCnt not supported (PortCnt:%d)", pHDB->PortCnt);

    POF_State_Error_CloseEDDChannel(pHDB, pResponse);
    return;
  }

  *pResponse = POF_EDDInitPortMgm(pHDB, pHDB->PortCnt);
  if (*pResponse != POF_RSP_OK)
  {
    POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "POF_State_Get_Params_Edd_Running, Cannot init POF_EDDInitPortMgm");

    POF_State_Error_CloseEDDChannel(pHDB, pResponse);
    return;
  }
  
  /* service to get PhyTpye per port */
  *pResponse = POF_EDDGetPortParams(pHDB);  
  if (*pResponse != POF_RSP_OK)
  {
    POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "POF_State_Get_Params_Edd_Running, Cannot init POF_EDDGetPortParams");

    POF_State_Error_CloseEDDChannel(pHDB, pResponse);
    return;
  }

  pHDB->State = POF_HANDLE_STATE_GET_PORT_PARAMS_EDD_RUNNING;

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_State_Get_Params_Edd_Running, pRQB:0x%X", pRQB);
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_State_Get_PortParams_Edd_Running()      +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pHDB       : Pointer to HDB to handle Channel request for              +*/
/*+  Response   : Responsecode of actual action taken (depends on state)    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  D e s c r i p t i o n:  *pbCallCbf is presetted with LSA_FALSE outside +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static  LSA_VOID  POF_LOCAL_FCT_ATTR  POF_State_Get_PortParams_Edd_Running( POF_COMMON_MEM_BOOL_PTR_TYPE  const  pbCallCbf, 
                                                                            POF_UPPER_RQB_PTR_TYPE        const  pRQB,
                                                                            POF_HDB_PTR_TYPE              const  pHDB, 
                                                                            POF_LOCAL_RSP_PTR_TYPE        const  pResponse )
{
  #if (POF_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pRQB);
  #endif

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_State_Get_PortParams_Edd_Running, pRQB:0x%X", pRQB);

  if (*pResponse != POF_RSP_OK)
  {
    POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "POF_State_Get_PortParams_Edd_Running, failed");
  
    POF_State_Error_CloseEDDChannel(pHDB, pResponse);
    return;
  }

  //wait until the response for all ports are back
  if (0 != pHDB->LinkIndPendingCnt)
  {
    return;
  }

  //provide Link-Indication Requests
  *pResponse = POF_EDDLinkIndProvide(pHDB, pHDB->PortCnt);  
  if (*pResponse != POF_RSP_OK)
  {
    POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "Cannot init POF_EDDLinkIndProvide");

    POF_State_Error_CloseEDDChannel(pHDB, pResponse);
    return;
  }

  //Start the cyclic Timer if not already started
  if (!POF_MGM_GET_GLOB_PTR()->CyclicTimerUserCnt)
  {
    //Start the 1s timer
    POF_TimerStart(POF_MGM_GET_GLOB_PTR()->CyclicTimerIndex, (LSA_UINT16)1);
  }

  POF_MGM_GET_GLOB_PTR()->CyclicTimerUserCnt++;

  //no more actions needed. Channel is open now.
  pHDB->State = POF_HANDLE_STATE_OPEN;

  *pbCallCbf = LSA_TRUE;

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_State_Get_PortParams_Edd_Running, pRQB:0x%X", pRQB);
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_State_Close_Error_Edd_Running()         +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pHDB       : Pointer to HDB to handle Channel request for              +*/
/*+  Response   : Responsecode of actual action taken (depends on state)    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  D e s c r i p t i o n:     *pbCallCbf and *pbReleasePath is presetted  +*/
/*+                             with LSA_FALSE outside                      +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static  LSA_VOID  POF_LOCAL_FCT_ATTR  POF_State_Close_Error_Edd_Running( POF_COMMON_MEM_BOOL_PTR_TYPE  const  pbCallCbf,
                                                                         POF_COMMON_MEM_BOOL_PTR_TYPE  const  pbReleasePath,
                                                                         POF_HDB_PTR_TYPE              const  pHDB,
                                                                         POF_LOCAL_RSP_PTR_TYPE        const  pResponse )
{
  LSA_UINT16  PortIndex;

    POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_State_Close_Error_Edd_Running");

    //change state of all pof-transceivers for all ports
    for (PortIndex = 0; PortIndex < pHDB->PortCnt; PortIndex++)
    {
        POF_TransceiverSMTransition(PortIndex, POF_TRANSCEIVER_TRIGGER_STOP, pHDB);
    }

    if (*pResponse != POF_RSP_OK)
    {
        POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_State_Close_Error_Edd_Running, failed");
        POF_FATAL("POF_State_Close_Error_Edd_Running", POF_FATAL_ERR_EDD, 0, 0);
        //lint -unreachable
        return;
    }

    /* check if all pending requests canceled (should be!) */
    if (pHDB->LinkIndPendingCnt)
    {
        POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_State_Close_Error_Edd_Running, EDD Link-Indications not canceled!");
        POF_FATAL("POF_State_Close_Error_Edd_Running", POF_FATAL_ERR_EDD, 0, 0);
        //lint -unreachable
        return;
    }

    *pResponse = pHDB->OpenResponse; //get Error-Response

    POF_SYSTEM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_State_Close_Error_Edd_Running, starting releasing path");
    *pbReleasePath = LSA_TRUE;
    *pbCallCbf     = LSA_TRUE;

    POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_State_Close_Error_Edd_Running");
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_State_Close_Wait_Timer()                +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pHDB       : Pointer to HDB to handle Channel request for              +*/
/*+  Response   : Responsecode of actual action taken (depends on state)    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  D e s c r i p t i o n:  *pbCallCbf is presetted with LSA_FALSE outside +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static  LSA_VOID  POF_LOCAL_FCT_ATTR  POF_State_Close_Wait_Timer( POF_COMMON_MEM_BOOL_PTR_TYPE  const  pbCallCbf,
                                                                  POF_HDB_PTR_TYPE              const  pHDB,
                                                                  POF_LOCAL_RSP_PTR_TYPE        const  pResponse )
{
  LSA_UINT16  PortIndex;

  POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_State_Close_Wait_Timer");

  //change state of all pof-transceivers for all ports
  for (PortIndex = 0; PortIndex < pHDB->PortCnt; PortIndex++)
  {
      POF_TransceiverSMTransition(PortIndex, POF_TRANSCEIVER_TRIGGER_STOP, pHDB);
  }

  if (*pResponse != POF_RSP_OK)
  {
    POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_State_Close_Wait_Timer, Unexpected error");
    POF_FATAL("POF_State_Close_Wait_Timer, Response:", POF_FATAL_ERR_EDD, *pResponse, 0);
    //lint -unreachable
    return;
  }
  
  POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "POF_State_Close_Wait_Timer, EDD-Closing Channel");

  pHDB->State = POF_HANDLE_STATE_CLOSE_CHANNEL_EDD_RUNNING;

  *pResponse = POF_EDDCloseChannel(pHDB);

  if (*pResponse != POF_RSP_OK)
  {
    POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_State_Close_Wait_Timer, Cannot Close EDD Channel, Handle:0x%X", pHDB->ThisHandle);
    *pbCallCbf = LSA_TRUE;
    return;
  }

  /* Request is underway now. */
  pHDB->State = POF_HANDLE_STATE_CLOSE_CHANNEL_EDD_RUNNING;

  POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_State_Close_Wait_Timer");
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_State_Close_Channel_Edd_Running()       +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pHDB       : Pointer to HDB to handle Channel request for              +*/
/*+  Response   : Responsecode of actual action taken (depends on state)    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  D e s c r i p t i o n:     *pbCallCbf and *pbReleasePath is presetted  +*/
/*+                             with LSA_FALSE outside                      +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static  LSA_VOID  POF_LOCAL_FCT_ATTR  POF_State_Close_Channel_Edd_Running( POF_COMMON_MEM_BOOL_PTR_TYPE  const  pbCallCbf, 
                                                                           POF_COMMON_MEM_BOOL_PTR_TYPE  const  pbReleasePath,
                                                                           POF_UPPER_RQB_PTR_TYPE        const  pRQB,
                                                                           POF_HDB_PTR_TYPE              const  pHDB, 
                                                                     const POF_LOCAL_RSP_PTR_TYPE        const  pResponse )
{
    #if (POF_CFG_TRACE_MODE == 0)
      LSA_UNUSED_ARG(pRQB);
    #endif
  
    POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_State_Close_Channel_Edd_Running, pRQB:0x%X", pRQB);

    *pbCallCbf = LSA_TRUE;

    if (*pResponse != POF_RSP_OK)
    {
        POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "POF_State_Close_Channel_Edd_Running, failed");
        return;
    }

    /* check if all pending requests cancled (should be!!) */
    if (pHDB->LinkIndPendingCnt)
    {
        POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "EDD Link-Indications not canceled!");
        POF_FATAL("POF_State_Close_Channel_Edd_Running", POF_FATAL_ERR_EDD, 0, 0);
        //lint -unreachable
        return;
    }

    POF_SYSTEM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_State_Close_Channel_Edd_Running, starting releasing path");
    *pbReleasePath = LSA_TRUE;

    POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_State_Close_Channel_Edd_Running, pRQB:0x%X", pRQB);
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_UserChannelStateMachine()               +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pHDB       : Pointer to HDB to handle Channel request for              +*/
/*+  Response   : Responsecode of actual action taken (depends on state)    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Statemachine for open/close channel.                      +*/
/*+                                                                         +*/
/*+               Actions taken to fulfill the task to open or close        +*/
/*+               the channel. The action depends on the current "State"    +*/
/*+               of the channel.                                           +*/
/*+                                                                         +*/
/*+  States:                                                                +*/
/*+                                                                         +*/
/*+  Open without error:  POF_HANDLE_STATE_OPEN_CHANNEL_EDD_RUNNING         +*/
/*+                       POF_HANDLE_STATE_GET_PARAMS_EDD_RUNNING           +*/
/*+                       POF_HANDLE_STATE_GET_PORT_PARAMS_EDD_RUNNING      +*/
/*+                       POF_HANDLE_STATE_OPEN                             +*/
/*+                                                                         +*/
/*+  Close without error: POF_HANDLE_STATE_CLOSE_WAIT_TIMER                 +*/
/*+                       POF_HANDLE_STATE_CLOSE_CHANNEL_EDD_RUNNING        +*/
/*+                       POF_HANDLE_STATE_FREE                             +*/
/*+                                                                         +*/
/*+  On error while opening, we have to undo several things using the states+*/
/*+                                                                         +*/
/*+                       POF_HANDLE_STATE_CLOSE_ERROR_EDD_RUNNING          +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID  POF_LOCAL_FCT_ATTR  POF_UserChannelStateMachine( POF_HDB_PTR_TYPE  const  pHDB,
                                                           POF_RSP                  Response )
{
    LSA_BOOL                                bCallCbf     = LSA_FALSE; //don't call Cbf by default!
    LSA_BOOL                                bReleasePath = LSA_FALSE; //don't release path by default!
    POF_UPPER_RQB_PTR_TYPE                  pRQB;
    LSA_SYS_PTR_TYPE                        pSys;
    POF_DETAIL_PTR_TYPE                     pDetail;
    POF_UPPER_CALLBACK_FCT_PTR_TYPE         Cbf;
    LSA_UINT32                       const  TraceIdx = pHDB->TraceIdx;

    POF_FUNCTION_TRACE_02(TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_UserChannelStateMachine, pHDB:0x%X Response:0x%X", pHDB, Response);

    POF_CHECK_NULL_PTR(pHDB);

    //save parameters from HDB because handle (=HDB) maybe freed!
    pRQB    = pHDB->pOpenOrCLoseRQB;
    Cbf     = pHDB->Params.Cbf;
    pSys    = pHDB->Params.pSys;
    pDetail = pHDB->Params.pDetail;

    POF_PROGRAM_TRACE_01(TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_UserChannelStateMachine, pHDB->State:0x%X", pHDB->State);

    switch (pHDB->State) 
    {
        /*-------------------------------------------------------------------*/
        /* Just finished an EDD-Open Channel request.                        */
        /* Next Get PortCnt from EDD                                         */
        /*-------------------------------------------------------------------*/
        case POF_HANDLE_STATE_OPEN_CHANNEL_EDD_RUNNING:     
            POF_State_Open_Channel_Edd_Running(&bCallCbf, &bReleasePath, pRQB, pHDB, &Response);
            break;

        /*-------------------------------------------------------------------*/
        /* Just finished a GetParams EDD-Request while opening channel       */
        /* Provide Recv and Linkindication requests. Start cyclic Timer.     */
        /*-------------------------------------------------------------------*/
        case POF_HANDLE_STATE_GET_PARAMS_EDD_RUNNING:  
            POF_State_Get_Params_Edd_Running(pRQB, pHDB, &Response);
            break;

        /*-------------------------------------------------------------------*/
        /* Just finished a GetParams EDD-Request while opening channel       */
        /* Provide Recv and Linkindication requests. Start cyclic Timer.     */
        /*-------------------------------------------------------------------*/
        case POF_HANDLE_STATE_GET_PORT_PARAMS_EDD_RUNNING:  
            POF_State_Get_PortParams_Edd_Running(&bCallCbf, pRQB, pHDB, &Response);
            break;

        /*-------------------------------------------------------------------*/
        /* Just finished an EDD-Close-Channel because of error within Open   */
        /*-------------------------------------------------------------------*/
        case POF_HANDLE_STATE_CLOSE_ERROR_EDD_RUNNING:  
            POF_State_Close_Error_Edd_Running(&bCallCbf, &bReleasePath, pHDB, &Response);
            break;
    
        /*-------------------------------------------------------------------*/
        /* Just finished waiting for timer to get free while closing channel */
        /* Note: If further handling failed we do not restart the timer or   */
        /*       reprovide rx-resource. The handle will not be useful anymore*/
        /*-------------------------------------------------------------------*/
        case POF_HANDLE_STATE_CLOSE_WAIT_TIMER:          
            POF_State_Close_Wait_Timer(&bCallCbf, pHDB, &Response);
            break;
    
        /*-------------------------------------------------------------------*/
        /* Just finished an EDD-Close-Channel request                        */
        /* Note: If handling failed we do not restart the timer or           */
        /*       reprovide rx-resource. The handle will not be useful anymore*/
        /*-------------------------------------------------------------------*/
        case POF_HANDLE_STATE_CLOSE_CHANNEL_EDD_RUNNING:
            POF_State_Close_Channel_Edd_Running(&bCallCbf, &bReleasePath, pRQB, pHDB, &Response);
            break;

        /*-------------------------------------------------------------------*/
        /* Other states not expected within this function.                   */
        /*-------------------------------------------------------------------*/
        default:
        {
            POF_PROGRAM_TRACE_00(TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_UserChannelStateMachine");
            POF_FATAL("POF_UserChannelStateMachine", POF_FATAL_ERR_INCONSISTENZ, 0, 0);
            //lint -unreachable
            return;
        }
    } /* switch */      

    if //release path requested?
       (bReleasePath)
    {
        //free handle and HDB (POF_HANDLE_STATE_FREE) (no HDB-dealloc)
        POF_UserReleaseHDB(pHDB);
    }

    if //call Cbf requested? (= finish service)
       (bCallCbf)
    {
        pHDB->pOpenOrCLoseRQB = LSA_NULL;  /* to signal no more request pending */

        POF_RQB_SET_RESPONSE(pRQB, Response);
        /* upper-handle already set */
        POF_UPPER_TRACE_03(TraceIdx, LSA_TRACE_LEVEL_NOTE, "<<<: Open/Close-Request finished, Opcode:0x%X UpperHandle:0x%X Response:0x%X",
                           POF_RQB_GET_OPCODE(pRQB), POF_RQB_GET_HANDLE(pRQB), Response);
        POF_CallCbf(TraceIdx, Cbf, pRQB, pSys);
    }

    if //release path requested?
       (bReleasePath)
    {
        LSA_UINT16  PathStatus;

        POF_SYSTEM_TRACE_00(TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_UserChannelStateMachine, release path requested, calling POF_RELEASE_PATH_INFO()");

        POF_RELEASE_PATH_INFO(&PathStatus, pSys, pDetail);

        if (PathStatus != LSA_RET_OK)
        {
            POF_PROGRAM_TRACE_01(TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_UserChannelStateMachine, POF_RELEASE_PATH_INFO() failed, RetVal:0x%X", PathStatus);
            POF_FATAL("POF_UserChannelStateMachine, POF_RELEASE_PATH_INFO() failed, RetVal:", POF_FATAL_ERR_RELEASE_PATH_INFO, PathStatus, 0);
            //lint -unreachable
            return;
        }
    }

    POF_FUNCTION_TRACE_00(TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_UserChannelStateMachine");
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_UserCloseChannel()                      +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pHDB                : User HDB                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Closes User-Handle.                                       +*/
/*+               Stops Cyclic Timer. If Timer RQB is underway we have to   +*/
/*+               wait for the RQB arrives so we go into Wait-for-timer     +*/
/*+               State.                                                    +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static  LSA_VOID  POF_LOCAL_FCT_ATTR  POF_UserCloseChannel( POF_HDB_PTR_TYPE  const  pHDB )
{
    LSA_BOOL  bWaitTimer = LSA_FALSE;

    POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_UserCloseChannel, pHDB:0x%X", pHDB);

    /* if we are not in sate OPEN, a previous attempt to close */
    /* this handle failed and we must not stop the timer again */
    if (pHDB->State == POF_HANDLE_STATE_OPEN)
    {
        /* if this is the last user, we stop the timer */
        if (POF_MGM_GET_GLOB_PTR()->CyclicTimerUserCnt == 1)
        {
            POF_TimerStop(POF_MGM_GET_GLOB_PTR()->CyclicTimerIndex);
            bWaitTimer = LSA_TRUE;
        }
        POF_MGM_GET_GLOB_PTR()->CyclicTimerUserCnt--;
    }

    /* check if timer RQB is free. if not we have to wait if we are     */
    /* the last one using the timer                                     */
    /* (the timer RQB maybe pending because a timer event occurred just */
    /* before we stopped the timer)                                     */
    pHDB->State = POF_HANDLE_STATE_CLOSE_WAIT_TIMER;

    if (   (bWaitTimer)
        || (!g_pPOFInfo->Glob.Timer[POF_MGM_GET_GLOB_PTR()->CyclicTimerIndex].IntRQB.bInUse))
    {
        /* don't need to wait for timer. continue... */
        POF_UserChannelStateMachine(pHDB, POF_RSP_OK);
    }
    else
    {
        POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "Have to wait for Timer before closing channel");

        /* Request is underway now. check in timer-hanling again and finish the request. */
    }

    POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_UserCloseChannel");
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_RequestTimer()                          +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE        opcode:     POF_OPC_TIMER                    +*/
/*+     LSA_HANDLE_TYPE        handle:                                      +*/
/*+     LSA_USER_ID_TYPE       user_id:                                     +*/
/*+     POF_RSP                Response    Response                         +*/
/*+     POF_RQB_ARGS_TYPE      args.Timer: POF_RQB_TIMER_TYPE               +*/
/*+                                                                         +*/
/*+     args.Timer:                                                         +*/
/*+                                                                         +*/
/*+     TimerID  : POF-Timer ID                                             +*/
/*+     UserID   : not used yet                                             +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Request initiated from POF within pof_timeout().          +*/
/*+               Handles Cyclic Timer event.                               +*/
/*+                                                                         +*/
/*+               Send data and checks for TTL.                             +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static  LSA_VOID  POF_LOCAL_FCT_ATTR  POF_RequestTimer( POF_UPPER_RQB_PTR_TYPE  const  pRQB )
{
  POF_HDB_PTR_TYPE  pHDB;
  LSA_UINT32        i;
  
  POF_FUNCTION_TRACE_01(0, LSA_TRACE_LEVEL_CHAT, "IN: POF_RequestTimer, pRQB:0x%X", pRQB);
  
  if (pRQB->args.Timer.UserID != POF_TIMER_USER_ID_CYLIC)
  {
    POF_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_FATAL, "pof_RequestTimer. Invalid Timer-UserID (0x%X)",pRQB->args.Timer.UserID);
    POF_FATAL("POF_RequestTimer", POF_FATAL_ERR_INCONSISTENZ, 0, 0);
    //lint -unreachable
    return;
  }
  
  POF_TimerRQBFree(pRQB->args.Timer.POFTimerIndex);
  
  pHDB = POF_MGM_GET_HUSER_PTR()->HDB;
  
  ++POF_MGM_GET_GLOB_PTR()->CyclicTimerCnt;
  
  /* For all Channels we do the timer handling */
  for (i = 0; i < POF_CFG_MAX_CHANNELS; i++)
  {
    /* check if we are in waiting for Timer to close the channel. If so we */
    /* finish the close-handling                                           */
    /* Note: Currently we only have one User-Handle!!                      */
    switch (pHDB->State)
    {
      case POF_HANDLE_STATE_CLOSE_WAIT_TIMER:
      {
        POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "Timer event while closing, continue");
      
        if (!LSA_HOST_PTR_ARE_EQUAL(pHDB->pOpenOrCLoseRQB, LSA_NULL))
        {
          //timer RQB is free now
          POF_UserChannelStateMachine(pHDB, POF_RSP_OK);
        }
        else
        {
          POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "pOpenOrCLoseRQB == LSA_NULL, Cannot Close channel");
          POF_FATAL("POF_RequestTimer", POF_FATAL_ERR_INCONSISTENZ, 0, 0);
          //lint -unreachable
          return;
        }
      } break;

      case POF_HANDLE_STATE_OPEN:
      {
        #if !defined (POF_CFG_SIMULATE_DIAGNOSTIC)
        POF_DMI_ReadHW_TimeOut(pHDB); //read diagnostic from hardware/transceiver
        #endif
        POF_PRM_StateDiag_Validate_TimeOut(pHDB, LSA_FALSE); //validate diagnostic
      } break;

      default:
        //ignore timer
        break;
    }

    pHDB++;
  }

  POF_FUNCTION_TRACE_00(0, LSA_TRACE_LEVEL_CHAT, "OUT: POF_RequestTimer");
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_RequestTestHardware()                   +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    POF_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pHDB       : User HDB                                                  +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE        opcode:      POF_OPC_GET_CONFIG              +*/
/*+     LSA_HANDLE_TYPE        handle:                                      +*/
/*+     LSA_USER_ID_TYPE       user_id:                                     +*/
/*+     POF_RSP                Response     Response                        +*/
/*+     POF_RQB_ARGS_TYPE      args.TestHardware                            +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Request to test POF-Hardware                              +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static  POF_RSP  POF_LOCAL_FCT_ATTR  POF_RequestTestHardware( POF_UPPER_RQB_PTR_TYPE  const  pRQB,
                                                              POF_HDB_PTR_TYPE        const  pHDB )
{
  POF_RSP     Status;
  LSA_UINT16  PortIndex;
  EDD_RSP     EDDRetVal;

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_RequestTestHardware, pRQB:0x%X", pRQB);

  //service not allowed in Simulate-Diagnostic-Mode

  #if defined (POF_CFG_SIMULATE_DIAGNOSTIC)
  return POF_RSP_ERR_NOT_ALLOWED;
  #endif

  Status = POF_Get_Port_Index(pRQB->args.TestHardware.edd_port_id, &PortIndex, LSA_FALSE, pHDB);
  if (Status != POF_RSP_OK)
  {
    return Status;
  }  

  if (!POF_IS_POF_PORT(PortIndex, pHDB))
  {
    POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestTestHardware, no POF_IS_POF_PORT, PortIndex:0x%X", PortIndex);
    return POF_RSP_ERR_PARAM;
  }

  //This should never happen!!
  if (pHDB->I2C[PortIndex].I2CAvailable != POF_I2C_AVAILABLE)
  {
    POF_FATAL("POF_I2C_READ_OFFSET without I2C_AVAILABLE!", POF_FATAL_ERR_EDD, 0, 0);
  }

  switch (pRQB->args.TestHardware.Memory)
  {
    case POF_GET_RAW_A0:
    {
      if (POF_ACTION_READ == pRQB->args.TestHardware.Action)
      {
        EDDRetVal = EDD_STS_ERR_EXCP;
        // I2C availability is checked above!
        POF_I2C_READ_OFFSET((LSA_UINT16 POF_LOCAL_MEM_ATTR *)&EDDRetVal, //ret_val_ptr
                            pHDB->Params.pSys,                           //sys_ptr
                            PortIndex + 1UL, //PortId=PortIndex+1        //PortId
                            pHDB->I2C[PortIndex].I2CMuxSelect,           //I2CMuxSelect
                            POF_ADR_PAGE_A0,                             //I2CDevAddr
                            POF_I2C_STD_OFFSET_CNT,                      //I2COffsetCnt
                            pRQB->args.TestHardware.Address,             //I2COffset1
                            0,                                           //I2COffset2
                            (LSA_UINT32)pRQB->args.TestHardware.Length,  //Size
                            pRQB->args.TestHardware.pData);              //pBuf
        if (EDDRetVal != EDD_STS_OK)
        {
          POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_RequestTestHardware, POF_I2C_READ_OFFSET with invalid ret_val:0x%X", EDDRetVal);
          POF_FATAL("POF_RequestTestHardware, POF_I2C_READ_OFFSET with invalid ret_val:", POF_FATAL_ERR_EDD, EDDRetVal, 0);
          //lint -unreachable
          return POF_RSP_ERR_EXCP;
        }
      }
      else
      {
        EDDRetVal = EDD_STS_ERR_EXCP;
        // I2C availability is checked above!
        POF_I2C_WRITE_OFFSET((LSA_UINT16 POF_LOCAL_MEM_ATTR *)&EDDRetVal, //ret_val_ptr
                             pHDB->Params.pSys,                           //sys_ptr
                             PortIndex + 1UL, //PortId=PortIndex+1        //PortId
                             pHDB->I2C[PortIndex].I2CMuxSelect,           //I2CMuxSelect
                             POF_ADR_PAGE_A0,                             //I2CDevAddr
                             POF_I2C_STD_OFFSET_CNT,                      //I2COffsetCnt
                             pRQB->args.TestHardware.Address,             //I2COffset1
                             0,                                           //I2COffset2
                             (LSA_UINT32)pRQB->args.TestHardware.Length,  //Size
                             pRQB->args.TestHardware.pData);              //pBuf
        if (EDDRetVal != EDD_STS_OK)
        {
          POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_RequestTestHardware, POF_I2C_WRITE_OFFSET with invalid ret_val:0x%X", EDDRetVal);
          POF_FATAL("POF_RequestTestHardware, POF_I2C_WRITE_OFFSET with invalid ret_val:", POF_FATAL_ERR_EDD, EDDRetVal, 0);
          //lint -unreachable
          return POF_RSP_ERR_EXCP;
        }
      }
    } break;

    case POF_GET_RAW_A2:
    {
      if (POF_ACTION_READ == pRQB->args.TestHardware.Action)
      {
        EDDRetVal = EDD_STS_ERR_EXCP;
        // I2C availability is checked above!
        POF_I2C_READ_OFFSET((LSA_UINT16 POF_LOCAL_MEM_ATTR *)&EDDRetVal, //ret_val_ptr
                            pHDB->Params.pSys,                           //sys_ptr
                            PortIndex + 1UL, //PortId=PortIndex+1        //PortId
                            pHDB->I2C[PortIndex].I2CMuxSelect,           //I2CMuxSelect
                            POF_ADR_PAGE_A2,                             //I2CDevAddr
                            POF_I2C_STD_OFFSET_CNT,                      //I2COffsetCnt
                            pRQB->args.TestHardware.Address,             //I2COffset1
                            0,                                           //I2COffset2
                            (LSA_UINT32)pRQB->args.TestHardware.Length,  //Size
                            pRQB->args.TestHardware.pData);              //pBuf
        if (EDDRetVal != EDD_STS_OK)
        {
          POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_RequestTestHardware, POF_I2C_READ_OFFSET with invalid ret_val:0x%X", EDDRetVal);
          POF_FATAL("POF_RequestTestHardware, POF_I2C_READ_OFFSET with invalid ret_val:", POF_FATAL_ERR_EDD, EDDRetVal, 0);
          //lint -unreachable
          return POF_RSP_ERR_EXCP;
        }
      }
      else
      {
        EDDRetVal = EDD_STS_ERR_EXCP;
        // I2C availability is checked above!
        POF_I2C_WRITE_OFFSET((LSA_UINT16 POF_LOCAL_MEM_ATTR *)&EDDRetVal, //ret_val_ptr
                             pHDB->Params.pSys,                           //sys_ptr
                             PortIndex + 1UL, //PortId=PortIndex+1        //PortId
                             pHDB->I2C[PortIndex].I2CMuxSelect,           //I2CMuxSelect
                             POF_ADR_PAGE_A2,                             //I2CDevAddr
                             POF_I2C_STD_OFFSET_CNT,                      //I2COffsetCnt
                             pRQB->args.TestHardware.Address,             //I2COffset1
                             0,                                           //I2COffset2
                             (LSA_UINT32)pRQB->args.TestHardware.Length,  //Size
                             pRQB->args.TestHardware.pData);              //pBuf
        if (EDDRetVal != EDD_STS_OK)
        {
          POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_RequestTestHardware, POF_I2C_WRITE_OFFSET with invalid ret_val:0x%X", EDDRetVal);
          POF_FATAL("POF_RequestTestHardware, POF_I2C_WRITE_OFFSET with invalid ret_val:", POF_FATAL_ERR_EDD, EDDRetVal, 0);
          //lint -unreachable
          return POF_RSP_ERR_EXCP;
        }
      }
    } break;
  }
  
  POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_RequestTestHardware");

  return POF_RSP_OK;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_RequestGetRawHardware()                 +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    POF_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pHDB       : User HDB                                                  +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE        opcode:      POF_OPC_GET_CONFIG              +*/
/*+     LSA_HANDLE_TYPE        handle:                                      +*/
/*+     LSA_USER_ID_TYPE       user_id:                                     +*/
/*+     POF_RSP                Response     Response                        +*/
/*+     POF_RQB_ARGS_TYPE      args.GetRawHardware                          +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Request to get SW-diagnostic-buffer (1 byte)              +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static  POF_RSP  POF_LOCAL_FCT_ATTR  POF_RequestGetRawHardware( POF_UPPER_RQB_PTR_TYPE  const  pRQB,
                                                                POF_HDB_PTR_TYPE        const  pHDB )
{
  POF_RSP     Status;
  LSA_UINT16  PortIndex;

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_RequestGetRawHardware, pRQB:0x%X", pRQB);

  Status = POF_Get_Port_Index(pRQB->args.GetRawHardware.edd_port_id, &PortIndex, LSA_FALSE, pHDB);
  if (Status != POF_RSP_OK) 
  {
    return Status;
  }  

  switch (pRQB->args.GetRawHardware.Memory)
  {
    case POF_GET_RAW_A0:
    {
      POF_DMI_ReadOffset_A0(PortIndex, 
                            pRQB->args.GetRawHardware.Address, 
                            1UL, //1 byte
                            &pRQB->args.GetRawHardware.Value, 
                            pHDB);
    } break;

    case POF_GET_RAW_A2:
    {
      POF_DMI_ReadOffset_A2(PortIndex, 
                            pRQB->args.GetRawHardware.Address, 
                            1UL, //1 byte
                            &pRQB->args.GetRawHardware.Value, 
                            pHDB);
    } break;
  }
  
  POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_RequestGetRawHardware");

  return POF_RSP_OK;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_RequestSetRawHardware()                 +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    POF_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pHDB       : User HDB                                                  +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE        opcode:      POF_OPC_GET_CONFIG              +*/
/*+     LSA_HANDLE_TYPE        handle:                                      +*/
/*+     LSA_USER_ID_TYPE       user_id:                                     +*/
/*+     POF_RSP                Response     Response                        +*/
/*+     POF_RQB_ARGS_TYPE      args.SetRawHardware                          +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Request to set SW-A2-diagnostic-buffer (1 byte)           +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static  POF_RSP  POF_LOCAL_FCT_ATTR  POF_RequestSetRawHardware( POF_UPPER_RQB_PTR_TYPE  const  pRQB,
                                                                POF_HDB_PTR_TYPE        const  pHDB )
{
  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_RequestSetRawHardware, pRQB:0x%X", pRQB);

  //service only allowed in Simulate-Diagnostic-Mode!

  #if defined (POF_CFG_SIMULATE_DIAGNOSTIC)
  {
    POF_RSP     Status;
    LSA_UINT16  PortIndex;

    Status = POF_Get_Port_Index(pRQB->args.SetRawHardware.edd_port_id, &PortIndex, LSA_FALSE, pHDB);
    if (Status != POF_RSP_OK)
    {
      return Status;
    }

    pHDB->pPCtrl[PortIndex].DMIMap.A2_Mirror.Array_8[pRQB->args.SetRawHardware.Address] = pRQB->args.SetRawHardware.Value;

    POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_RequestSetRawHardware");

    return POF_RSP_OK;
  }
  #else //!POF_CFG_SIMULATE_DIAGNOSTIC
  LSA_UNUSED_ARG(pRQB); //satisfy lint!
  LSA_UNUSED_ARG(pHDB); //satisfy lint!
  return POF_RSP_ERR_NOT_ALLOWED;
  #endif //POF_CFG_SIMULATE_DIAGNOSTIC
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_RequestSetDiagnostic()                  +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    POF_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pHDB       : User HDB                                                  +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE        opcode:      POF_OPC_GET_CONFIG              +*/
/*+     LSA_HANDLE_TYPE        handle:                                      +*/
/*+     LSA_USER_ID_TYPE       user_id:                                     +*/
/*+     POF_RSP                Response     Response                        +*/
/*+     POF_RQB_ARGS_TYPE      args.SetDiagnostic                           +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Request to set POF-diagnostic (good, required, demanded)  +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static  POF_RSP  POF_LOCAL_FCT_ATTR  POF_RequestSetDiagnostic( POF_UPPER_RQB_PTR_TYPE  const  pRQB,
                                                               POF_HDB_PTR_TYPE        const  pHDB )
{
  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_RequestSetDiagnostic, pRQB:0x%X", pRQB);

  //service only allowed in Simulate-Diagnostic-Mode!

  //To simulate DiagnosisObject -> POF_OPC_PRM_INDICATION

  #if defined (POF_CFG_SIMULATE_DIAGNOSTIC)
  {
    POF_RSP                 Status;
    LSA_UINT16              PortIndex;
    POF_EDD_PCTRL_PTR_TYPE  pPCtrl;

    Status = POF_Get_Port_Index(pRQB->args.SetDiagnostic.edd_port_id, &PortIndex, LSA_FALSE, pHDB);
    if (Status != POF_RSP_OK)
    {
      return Status;
    }

    pPCtrl = &pHDB->pPCtrl[PortIndex];

    switch (pRQB->args.SetDiagnostic.DiagnosticType)
    {
      case POF_DIAGNOSTIC_RX_PWR_GOOD: //=Good
      {
        pPCtrl->DMIMap.A2_Mirror.u.ElementPOF.Flagbit_113 &= ~POF_FLAG_RX_OMA_MARGIN_ALARM;
        pPCtrl->DMIMap.A2_Mirror.u.ElementPOF.Flagbit_117 &= ~POF_FLAG_RX_OMA_MARGIN_WARN;
      } break;

      case POF_DIAGNOSTIC_RX_PWR_REQUIRED: //=Warning
      {
        pPCtrl->DMIMap.A2_Mirror.u.ElementPOF.Flagbit_113 &= ~POF_FLAG_RX_OMA_MARGIN_ALARM;
        pPCtrl->DMIMap.A2_Mirror.u.ElementPOF.Flagbit_117 |= POF_FLAG_RX_OMA_MARGIN_WARN;
      } break;

      case POF_DIAGNOSTIC_RX_PWR_DEMANDED: //=Alarm
      {
        pPCtrl->DMIMap.A2_Mirror.u.ElementPOF.Flagbit_113 |= POF_FLAG_RX_OMA_MARGIN_ALARM;
        pPCtrl->DMIMap.A2_Mirror.u.ElementPOF.Flagbit_117 &= ~POF_FLAG_RX_OMA_MARGIN_WARN;
      } break;

      default:
      {
        POF_SET_DETAIL_ERR(POF_ERR_DIAGNOSTIC_TYPE);
        return POF_RSP_ERR_PARAM;
      }
    }

    POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_RequestSetDiagnostic");

    return POF_RSP_OK;
  }
  #else //!POF_CFG_SIMULATE_DIAGNOSTIC
  LSA_UNUSED_ARG(pRQB); //satisfy lint!
  LSA_UNUSED_ARG(pHDB); //satisfy lint!
  return POF_RSP_ERR_NOT_ALLOWED;
  #endif //POF_CFG_SIMULATE_DIAGNOSTIC
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_RequestGetParams()                      +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    POF_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pHDB       : User HDB                                                  +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE        opcode:      POF_OPC_GET_CONFIG              +*/
/*+     LSA_HANDLE_TYPE        handle:                                      +*/
/*+     LSA_USER_ID_TYPE       user_id:                                     +*/
/*+     POF_RSP                Response     Response                        +*/
/*+     POF_RQB_ARGS_TYPE      args.GetParams                               +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Request to get config parameters from POF                 +*/
/*+                                                                         +*/
/*+               Fills RQB with config data and confirms request.          +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static  POF_RSP  POF_LOCAL_FCT_ATTR  POF_RequestGetParams( POF_UPPER_RQB_PTR_TYPE  const  pRQB,
                                                           POF_HDB_PTR_TYPE        const  pHDB )
{
  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_RequestGetParams, pRQB:0x%X", pRQB);
 
  pRQB->args.GetParams.MaxPortCnt = (LSA_UINT16)pHDB->PortCnt;
  
  POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_RequestGetParams");

  return POF_RSP_OK;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*===========================================================================*/
/*                            main-functions                                 */
/*===========================================================================*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    pof_open_channel()                          +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE        opcode:       POF_OPC_OPEN_CHANNEL           +*/
/*+     LSA_HANDLE_TYPE        handle:       -                              +*/
/*+     LSA_USER_ID_TYPE       user_id:      -                              +*/
/*+     POF_RSP                Response      -                              +*/
/*+     POF_RQB_ARGS_TYPE      Args.Channel: POF_RQB_OPEN_CHANNEL_TYPE      +*/
/*+                                                                         +*/
/*+     Args.Channel:                                                       +*/
/*+                                                                         +*/
/*+     LSA_HANDLE_TYPE         handle:       channel-handle of POF (ret)   +*/
/*+     LSA_HANDLE_TYPE         handle_upper  channel-handle of user        +*/
/*+     LSA_SYS_PATH_TYPE       sys_path      system-path of channel        +*/               
/*+     POF_UPPER_CALLBACK_FCT_PTR_TYPE  Cbf  callback-function             +*/
/*+                                                                         +*/
/*+  RQB-return values via callback-function:                               +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_HANDLE_TYPE        handle:      returned channel of user. This  +*/
/*+                                         is HandleUpper from Args.Channel+*/
/*+     POF_RSP                Response     POF_RSP_OK                      +*/
/*+                                         POF_RSP_ERR_RESOURCE            +*/
/*+                                         POF_RSP_ERR_SYS_PATH            +*/
/*+                                         POF_RSP_ERR_PARAM               +*/
/*+                                         ...                             +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Request to open a channel                                 +*/
/*+                                                                         +*/
/*+               Within this request the output - macro POF_GET_PATH_INFO  +*/
/*+               is used to get further channel-descriptons and parameters +*/
/*+                                                                         +*/
/*+               After a successful open a channel-handle is returned      +*/
/*+               in Args.Channel.Handle for use with further requests.     +*/
/*+                                                                         +*/
/*+               HandleUpper:                                              +*/
/*+                                                                         +*/
/*+               This handle will be stored within POF and return as       +*/
/*+               handle on request-confirmation (in RQB-header).           +*/
/*+                                                                         +*/
/*+               cbf:                                                      +*/
/*+                                                                         +*/
/*+               The request is always finished by calling the specified   +*/
/*+               callback-function. This callback-function will also be    +*/
/*+               used for all other other future requests for this channel.+*/
/*+                                                                         +*/
/*+               SysPath:                                                  +*/
/*+                                                                         +*/
/*+               The SysPath variable will not be used inside but given to +*/
/*+               systemadaption via POF_GET_PATH_INFO.                     +*/
/*+                                                                         +*/
/*+               This function is most likely done asynchroneous because   +*/
/*+               of calling lower layer open channel (EDD)                 +*/
/*+               depending on channel type specified in CDP (detail-ptr)   +*/
/*+                                                                         +*/
/*+  Note on error-handling:                                                +*/
/*+                                                                         +*/
/*+               In some cases it is not possible to call the call-back-   +*/
/*+               function of the requestor to confirm the request. in this +*/
/*+               case we call the output-macro POF_RQB_ERROR with the RQB  +*/
/*+               to notify this error. These errors are most likely caused +*/
/*+               by an implementation error and are of the type            +*/
/*+                                                                         +*/
/*+               POF_RSP_ERR_OPCODE                                        +*/
/*+               POF_RSP_ERR_PARAM                                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID  POF_UPPER_IN_FCT_ATTR  pof_open_channel( POF_UPPER_RQB_PTR_TYPE  pRQB )
{
    POF_RSP              Status;
    LSA_BOOL             bCallCbf     = LSA_FALSE; //don't call Cbf by default!
    LSA_BOOL             bReleasePath = LSA_FALSE; //don't release path by default!
    POF_DETAIL_PTR_TYPE  pDetail      = (POF_DETAIL_PTR_TYPE)LSA_NULL;
    LSA_SYS_PTR_TYPE     pSys         = {0}; 

    POF_UPPER_TRACE_00(0, LSA_TRACE_LEVEL_CHAT, "IN: pof_open_channel");

    POF_ENTER();

    Status = pof_open_ch_intern(&bCallCbf, &bReleasePath, pRQB, &pSys, &pDetail);

    if //call Cbf requested?
       (bCallCbf)
    {
        //upper-handle is already set
        POF_RQB_SET_RESPONSE(pRQB, Status);
        POF_UPPER_TRACE_03(0, LSA_TRACE_LEVEL_NOTE, "<<<: Open/Close-Request finished, Opcode:0x%X UpperHandle:0x%X Response:0x%X",
                           POF_RQB_GET_OPCODE(pRQB), POF_RQB_GET_HANDLE(pRQB), Status);
        POF_CallCbf(0, pRQB->args.channel.Cbf, pRQB, pSys);
    }

    if //release path requested?
       (bReleasePath)
    {
        LSA_UINT16  PathStatus;

        POF_SYSTEM_TRACE_00(0, LSA_TRACE_LEVEL_NOTE, "pof_open_channel, release path requested, calling POF_RELEASE_PATH_INFO()");

        POF_RELEASE_PATH_INFO(&PathStatus, pSys, pDetail);

        if (PathStatus != LSA_RET_OK)
        {
            POF_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_FATAL, "pof_open_channel, POF_RELEASE_PATH_INFO() failed, RetVal:0x%X", PathStatus);
            POF_FATAL("pof_open_channel, POF_RELEASE_PATH_INFO() failed, RetVal:", POF_FATAL_ERR_RELEASE_PATH_INFO, PathStatus, 0);
            //lint -unreachable
            POF_EXIT();
            return;
        }
    }

    POF_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_CHAT, "OUT: pof_open_channel, Status:0x%X", Status);

    POF_EXIT();
}
/*---------------------- end [subroutine] ---------------------------------*/


/***************************************************************************/
/* F u n c t i o n:       pof_open_ch_intern()                             */
/*                                                                         */
/* D e s c r i p t i o n: *pbCallCbf and *pbReleasePath is presetted       */
/*                        with LSA_FALSE outside                           */
/*                                                                         */
/* A r g u m e n t s:                                                      */
/*                                                                         */
/* Return Value:          POF_RSP                                          */
/*                                                                         */
/***************************************************************************/
static  POF_RSP  POF_LOCAL_FCT_ATTR  pof_open_ch_intern( POF_COMMON_MEM_BOOL_PTR_TYPE     const  pbCallCbf,
                                                         POF_COMMON_MEM_BOOL_PTR_TYPE     const  pbReleasePath,
                                                         POF_UPPER_RQB_PTR_TYPE           const  pRQB,
                                                         LSA_SYS_PTR_TYPE              *  const  ppSys,
                                                         POF_DETAIL_PTR_TYPE           *  const  ppDetail )
{
    POF_RSP              Status;
    LSA_SYS_PTR_TYPE     pSys;
    POF_DETAIL_PTR_TYPE  pDetail = (POF_DETAIL_PTR_TYPE)LSA_NULL;
    LSA_UINT16           PathStatus;
    POF_HDB_PTR_TYPE     pHDB = (POF_HDB_PTR_TYPE)LSA_NULL;

    POF_FUNCTION_TRACE_01(0, LSA_TRACE_LEVEL_CHAT, "IN: pof_open_ch_intern, pRQB:0x%X", pRQB);

    #if defined (POF_UNUSED)
    {
        LSA_SYS_PTR_TYPE  const  tmp = {0};
        pSys                         = tmp; /* pSys may not be a pointer! */
    }
    #endif

    POF_CHECK_NULL_PTR(pRQB);

    /*----------------------------------------------------------------------------*/
    /* setup some return parameters within RQB.                                   */
    /*----------------------------------------------------------------------------*/
    //set upper-handle
    POF_RQB_SET_HANDLE(pRQB, pRQB->args.channel.handle_upper);

    //check whether pof_init() was already executed
    if (LSA_HOST_PTR_ARE_EQUAL(g_pPOFInfo, LSA_NULL))
    {
        POF_UPPER_TRACE_00(0, LSA_TRACE_LEVEL_FATAL, "pof_open_ch_intern, g_pPOFInfo == LSA_NULL, pof_init() not called yet");
        POF_FATAL("pof_open_ch_intern, g_pPOFInfo == LSA_NULL, pof_init() not called yet", POF_FATAL_ERR_OPEN_CHANNEL, 0, 0);
        //lint -unreachable
        return POF_RSP_ERR_SEQUENCE;
    }

    //Reset DetailError
    POF_MGM_GET_HUSER_PTR()->ErrDetail.bSet = 0;

    /*----------------------------------------------------------------------------*/
    /* Check some parameters                                                      */
    /*----------------------------------------------------------------------------*/
    if (POF_RQB_GET_OPCODE(pRQB) != POF_OPC_OPEN_CHANNEL)
    {
        POF_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_FATAL, "pof_open_ch_intern, invalid Opcode:0x%X", POF_RQB_GET_OPCODE(pRQB));
        POF_RQB_ERROR(pRQB);
        POF_FATAL("pof_open_ch_intern, invalid Opcode:", POF_FATAL_ERR_OPEN_CHANNEL, POF_RQB_GET_OPCODE(pRQB), 0);
        //lint -unreachable
        return POF_RSP_ERR_OPCODE;
    }

    POF_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_NOTE, ">>>: Request: POF_OPC_OPEN_CHANNEL, Handle:0x%X", POF_RQB_GET_HANDLE(pRQB));

    if (0 == pRQB->args.channel.Cbf)
    {
        POF_UPPER_TRACE_00(0, LSA_TRACE_LEVEL_FATAL, "pof_open_ch_intern, Cbf == NULL");
        POF_RQB_ERROR(pRQB);
        POF_FATAL("pof_open_ch_intern, Cbf == NULL", POF_FATAL_ERR_OPEN_CHANNEL, 0, 0);
        //lint -unreachable
        return POF_RSP_ERR_PARAM;
    }
  
    if (POF_MGM_GET_GLOB_PTR()->Excp.Error != POF_FATAL_NO_ERROR)
    {
        POF_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_FATAL, "pof_open_ch_intern, Error:0x%X != POF_FATAL_NO_ERROR", POF_MGM_GET_GLOB_PTR()->Excp.Error);
        POF_RQB_ERROR(pRQB);
        POF_FATAL("pof_open_ch_intern, Error != POF_FATAL_NO_ERROR", POF_FATAL_ERR_OPEN_CHANNEL, POF_MGM_GET_GLOB_PTR()->Excp.Error, 0);
        //lint -unreachable
        return POF_RSP_ERR_PARAM;
    }

    /*------------------------------------------------------------------------*/
    /* Call LSA Output-macro.                                                 */
    /*------------------------------------------------------------------------*/
    POF_SYSTEM_TRACE_00(0, LSA_TRACE_LEVEL_NOTE, "pof_open_ch_intern, calling POF_GET_PATH_INFO()");

    POF_GET_PATH_INFO(&PathStatus, &pSys, &pDetail, pRQB->args.channel.sys_path);

    if (PathStatus != LSA_RET_OK)
    {
        POF_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_FATAL, "pof_open_ch_intern, POF_GET_PATH_INFO() failed, RetVal:0x%X", PathStatus);
        POF_FATAL("pof_open_ch_intern, POF_GET_PATH_INFO() failed, RetVal:", POF_FATAL_ERR_GET_PATH_INFO, PathStatus, 0);
        //lint -unreachable
        return POF_RSP_ERR_SYS_PATH;
    }

    POF_SYSTEM_TRACE_01(0, LSA_TRACE_LEVEL_NOTE, "<--: POF_GET_PATH_INFO() successful, Detail-Ptr:0x%X", pDetail);

    if (LSA_HOST_PTR_ARE_EQUAL(pDetail, LSA_NULL))
    {
        POF_PROGRAM_TRACE_00(0, LSA_TRACE_LEVEL_FATAL, "pof_open_ch_intern, Detail-Ptr == NULL");
        POF_FATAL("pof_open_ch_intern, Detail-Ptr == NULL", POF_FATAL_ERR_GET_PATH_INFO, 0, 0);
        //lint -unreachable
        return POF_RSP_ERR_SYS_PATH;
    }

    //pSys and pDetail are valid here! return them to caller!
    *ppSys    = pSys;
    *ppDetail = pDetail;

    //get a free initialized HDB
    Status = POF_UserGetFreeHDB(&pHDB);

    if //a free HDB is available?
       (Status == POF_RSP_OK) 
    {
        LSA_UINT8   UsrPortIndex;
        POF_CHECK_NULL_PTR(pHDB);

        //Init POF-HDB
        pHDB->Params.pSys        = pSys;
        pHDB->Params.pDetail     = pDetail;
        pHDB->Params.Cbf         = pRQB->args.channel.Cbf;
        pHDB->Params.SysPath     = pRQB->args.channel.sys_path;
        pHDB->Params.HandleUpper = pRQB->args.channel.handle_upper;
        pHDB->PortCnt = 0; //currently none present!

        for (UsrPortIndex = 0; UsrPortIndex < EDD_CFG_MAX_PORT_CNT; UsrPortIndex++)
        {
            //Default state is "Modular port" until changed by PRM_END
            pHDB->IsModularPort[UsrPortIndex] = POF_PRM_PORT_IS_MODULAR;

            // Store the information from POF_GET_PATH_INFO
            pHDB->I2C[UsrPortIndex].I2CMuxSelect = pDetail->I2CMuxSelect[UsrPortIndex];
            pHDB->I2C[UsrPortIndex].I2CAvailable = pDetail->I2CAvailable[UsrPortIndex];
        }

        //set our handle in RQB
        pRQB->args.channel.handle = pHDB->ThisHandle;

        //starts opening an EDD-Channel
        Status = POF_EDDOpenChannel(pHDB);

        if //starting opening an EDD-Channel ok?
           (Status == POF_RSP_OK)
        {
            //EDD-Request is on the way now (asynchronous service), no error till now!
            pHDB->State           = POF_HANDLE_STATE_OPEN_CHANNEL_EDD_RUNNING;
            pHDB->pOpenOrCLoseRQB = pRQB; //save pointer to request-RQB for later use
            return Status;
        }
        else //starting opening an EDD-Channel failed
        {
            POF_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_UNEXP, "pof_open_ch_intern, POF_EDDOpenChannel() failed, RetVal:0x%X", Status);
            POF_UserReleaseHDB(pHDB);
        }
    }
    else //no free HDB available
    {
        POF_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_UNEXP, "pof_open_ch_intern, POF_UserGetFreeHDB() failed, RetVal:0x%X", Status);
    }

    POF_SYSTEM_TRACE_00(0, LSA_TRACE_LEVEL_NOTE, "pof_open_ch_intern, starting releasing path");
    *pbReleasePath = LSA_TRUE;
    *pbCallCbf     = LSA_TRUE;
    return Status;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    pof_close_channel()                         +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         opcode:       POF_OPC_CLOSE_CHANNEL         +*/
/*+     LSA_HANDLE_TYPE         handle:       -                             +*/
/*+     LSA_USER_ID_TYPE        user_id:      -                             +*/
/*+     POF_RSP                 Response      -                             +*/
/*+     POF_RQB_ARGS_TYPE       Args          -                             +*/
/*+                                                                         +*/
/*+  RQB-return values via callback-function:                               +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_HANDLE_TYPE         handle:     returned channel of user.       +*/
/*+     POF_RSP                 Response    POF_RSP_OK                      +*/
/*+                                         POF_RSP_ERR_RESOURCE            +*/
/*+                                         POF_RSP_ERR_PARAM               +*/
/*+                                         ...                             +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Closes a channel.                                         +*/
/*+               Close lower-layer channel if neccessary. (depends on      +*/
/*+               channel type). Releases Path-info. Note that this         +*/
/*+               function is done asynchron because of calling lower layer +*/
/*+               functions with callback. On error we may finish synchron. +*/
/*+                                                                         +*/
/*+  Note on error-handling:                                                +*/
/*+                                                                         +*/
/*+               In some cases it is not possible to call the call-back-   +*/
/*+               function of the requestor to confirm the request. in this +*/
/*+               case we call the output-macro POF_RQB_ERROR with the RQB  +*/
/*+               to notify this error. This erros are most likely caused   +*/
/*+               by a implementation error an are of the type              +*/
/*+                                                                         +*/
/*+               POF_RSP_ERR_OPCODE                                        +*/
/*+               POF_RSP_ERR_PARAM                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID  POF_UPPER_IN_FCT_ATTR  pof_close_channel( POF_UPPER_RQB_PTR_TYPE  pRQB )
{
  LSA_HANDLE_TYPE                  Handle;
  POF_HDB_PTR_TYPE                 pHDB;
  POF_UPPER_CALLBACK_FCT_PTR_TYPE  Cbf;

  POF_ENTER();

  POF_FUNCTION_TRACE_01(0, LSA_TRACE_LEVEL_CHAT, "IN: pof_close_channel, pRQB:0x%X", pRQB);

  POF_CHECK_NULL_PTR(pRQB);

  Handle = POF_RQB_GET_HANDLE(pRQB);

  /*---------------------------------------------------------------------------*/
  /* setup some return parameters within RQB.                                  */
  /*---------------------------------------------------------------------------*/
  POF_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_NOTE, ">>>: Request: POF_OPC_CLOSE_CHANNEL, Handle:0x%X", Handle);

  /* check pof_init already called */
  if (LSA_HOST_PTR_ARE_EQUAL(g_pPOFInfo, LSA_NULL))
  {
    POF_UPPER_TRACE_00(0, LSA_TRACE_LEVEL_ERROR, "pof_close_channel, g_pPOFInfo == LSA_NULL, pof_init not called");
    POF_RQB_SET_RESPONSE(pRQB, POF_RSP_ERR_SEQUENCE);
    POF_RQB_ERROR(pRQB);
    POF_EXIT();
    return;
  }

  pHDB = POF_GetUserHDBFromHandle(Handle);    
  
  if (LSA_HOST_PTR_ARE_EQUAL(pHDB, LSA_NULL))
  {
    POF_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_ERROR, "pof_close_channel, invalid POF-User Handle:0x%X", Handle);
    POF_RQB_SET_RESPONSE(pRQB, POF_RSP_ERR_PARAM);
    POF_RQB_ERROR(pRQB);
    POF_EXIT();
    return;
  }

  Cbf = pHDB->Params.Cbf;

  //if (0 == Cbf)
  //{
  //  POF_FATAL("pof_close_channel, Cbf == 0", POF_FATAL_ERR_NULL_PTR, 0, 0);
  //  //lint -unreachable
  //  POF_EXIT();
  //  return;
  //}

  /* set upper handle in RQB */
  POF_RQB_SET_HANDLE(pRQB, pHDB->Params.HandleUpper);

  if (POF_RQB_GET_OPCODE(pRQB) != POF_OPC_CLOSE_CHANNEL)
  {
    POF_UPPER_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "pof_close_channel, Invalid Opcode:0x%X", POF_RQB_GET_OPCODE(pRQB));
    POF_RQB_SET_RESPONSE(pRQB, POF_RSP_ERR_PARAM);
    POF_CallCbf(pHDB->TraceIdx, Cbf, pRQB, pHDB->Params.pSys);
    POF_EXIT();
    return;
  }

  /* only if no open/close request in progress */
  if (!LSA_HOST_PTR_ARE_EQUAL(pHDB->pOpenOrCLoseRQB, LSA_NULL))
  {
    POF_UPPER_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "pof_close_channel, Close channel failed. Open or Close already running");
    POF_RQB_SET_RESPONSE(pRQB, POF_RSP_ERR_SEQUENCE);
    POF_CallCbf(pHDB->TraceIdx, Cbf, pRQB, pHDB->Params.pSys);
    POF_EXIT();
    return;
  }

  POF_UserCloseChannel(pHDB);

  /*-----------------------------------------------------------------------*/
  /* Don't finish the request immediately!                                 */
  /*-----------------------------------------------------------------------*/
  pHDB->pOpenOrCLoseRQB = pRQB; //save request-RQB for later use

  POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: pof_close_channel");

  POF_EXIT();
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    pof_request()                               +*/
/*+  Input/Output          :                                                +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         opcode:       varies:                       +*/
/*+                                           POF_OPC_xxx                   +*/
/*+                                           POF_OPC_TIMER                 +*/
/*+     LSA_HANDLE_TYPE        handle:       -                              +*/
/*+     LSA_USER_ID_TYPE       user_id:      -                              +*/
/*+     POF_RSP                Response      -                              +*/
/*+     POF_RQB_ARGS_TYPE      Args          varies depending on opcode     +*/
/*+                                                                         +*/
/*+  RQB-return values via callback-function:                               +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_HANDLE_TYPE        handle:     upper-handle from open_channel   +*/
/*+     POF_RSP                Response    POF_RSP_OK                       +*/
/*+                                        POF_RSP_ERR_RESOURCE             +*/
/*+                                        POF_RSP_ERR_PARAM                +*/
/*+                                        ...                              +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Do a channel request.                                     +*/
/*+                                                                         +*/
/*+               For this requests a valid channel handle is needed. The   +*/
/*+               handle will be given on open_channel-request.             +*/
/*+                                                                         +*/
/*+  Note on error-handling:                                                +*/
/*+                                                                         +*/
/*+               In some cases it is not possible to call the call-back-   +*/
/*+               function of the requestor to confirm the request. in this +*/
/*+               case we call the output-macro POF_RQB_ERROR with the RQB  +*/
/*+               to notify this error. This erros are most likely caused   +*/
/*+               by a implementation error and are of the type             +*/
/*+                                                                         +*/
/*+               POF_RSP_ERR_OPCODE                                        +*/
/*+               POF_RSP_ERR_PARAM                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID  POF_UPPER_IN_FCT_ATTR  pof_request( POF_UPPER_RQB_PTR_TYPE  pRQB )
{
  LSA_BOOL          bIndicate;
  POF_RSP           Response;
  LSA_HANDLE_TYPE   Handle;
  POF_HDB_PTR_TYPE  pHDB;
  LSA_OPCODE_TYPE   Opcode;
  
  POF_ENTER();
  
  POF_FUNCTION_TRACE_01(0, LSA_TRACE_LEVEL_CHAT, "IN: pof_request, pRQB:0x%X", pRQB);
  
  POF_CHECK_NULL_PTR(pRQB);
  
  /* check pof_init already called */
  if (LSA_HOST_PTR_ARE_EQUAL(g_pPOFInfo, LSA_NULL))
  {
    POF_RQB_SET_RESPONSE(pRQB, POF_RSP_ERR_SEQUENCE);
    POF_UPPER_TRACE_00(0, LSA_TRACE_LEVEL_UNEXP, ">>>: pof_request, g_pPOFInfo == LSA_NULL, pof_init not called");
    POF_RQB_ERROR(pRQB);
    POF_EXIT();
    return;
  }

  /* Reset DetailError */
  POF_MGM_GET_HUSER_PTR()->ErrDetail.bSet = 0;
  
  Handle = POF_RQB_GET_HANDLE(pRQB);

  POF_PROGRAM_TRACE_02(0, LSA_TRACE_LEVEL_CHAT, ">>>: pof_request, Opcode:0x%X Handle:0x%X", POF_RQB_GET_OPCODE(pRQB), POF_RQB_GET_HANDLE(pRQB));
  
  /* TIMER-Request needs special handling */
  if (POF_RQB_GET_OPCODE(pRQB) == POF_OPC_TIMER)
  {
    POF_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_CHAT, ">>>: pof_request, POF_OPC_TIMER, Handle:0x%X", POF_RQB_GET_HANDLE(pRQB));
    //We should not get here if I2C is NOT available. But if we get the cyclic tigger and I2C is NOT available the SM is in the state POF_TRANSCEIVER_STATE_STOPPED and nothing happens.
    POF_RequestTimer(pRQB);
    POF_FUNCTION_TRACE_00(0, LSA_TRACE_LEVEL_CHAT, "OUT: pof_request 1");
    POF_EXIT();
    return;
  }

  pHDB = POF_GetUserHDBFromHandle(Handle);    
  
  if (LSA_HOST_PTR_ARE_EQUAL(pHDB, LSA_NULL))
  {
    POF_RQB_SET_RESPONSE(pRQB, POF_RSP_ERR_PARAM);
    POF_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_ERROR, ">>>: pof_request, Invalid handle:0x%X", Handle);
    POF_RQB_ERROR(pRQB);
    POF_FUNCTION_TRACE_00(0, LSA_TRACE_LEVEL_CHAT, "OUT: pof_request 2");
    POF_EXIT();
    return;
  }   
  
  /* no open/close runnning ? and channel open ?                    */
  /* Note: If a close-channel failed within some closing state. The */
  /*       channel can not be used anymore for this user - requests */
  /*       The channel is in some inoperable state and it can only  */
  /*       be tried to do another close-request..                   */
    
  if ((pHDB->State != POF_HANDLE_STATE_OPEN) || (!LSA_HOST_PTR_ARE_EQUAL(pHDB->pOpenOrCLoseRQB, LSA_NULL)))
  {
    POF_UPPER_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, ">>>: pof_request, Channel not open or Open/Close running, Opcode:0x%X", POF_RQB_GET_OPCODE(pRQB));
    POF_UserRequestFinish(pHDB, pRQB, POF_RSP_ERR_SEQUENCE);
    POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: pof_request 3");
    POF_EXIT();
    return;
  }

  bIndicate = LSA_TRUE;

  Opcode = POF_RQB_GET_OPCODE(pRQB); 

  switch (Opcode)
  {
    case POF_OPC_SET_RAW_HARDWARE:
    {
      POF_UPPER_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, ">>>: pof_request: POF_OPC_SET_RAW_HARDWARE, Handle:0x%X", POF_RQB_GET_HANDLE(pRQB));
      Response = POF_RequestSetRawHardware(pRQB, pHDB);
    } break;

    case POF_OPC_GET_RAW_HARDWARE:
    {
      POF_UPPER_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, ">>>: pof_request: POF_OPC_GET_RAW_HARDWARE, Handle:0x%X", POF_RQB_GET_HANDLE(pRQB));
      Response = POF_RequestGetRawHardware(pRQB, pHDB);
    } break;

    case POF_OPC_SET_DIAGNOSTIC:
    {
      POF_UPPER_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, ">>>: pof_request: POF_OPC_SET_DIAGNOSTIC, Handle:0x%X", POF_RQB_GET_HANDLE(pRQB));
      Response = POF_RequestSetDiagnostic(pRQB, pHDB);
    } break;

    case POF_OPC_GET_PARAMS:
    {
      POF_UPPER_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, ">>>: pof_request: POF_OPC_GET_PARAMS, Handle:0x%X", POF_RQB_GET_HANDLE(pRQB));
      Response = POF_RequestGetParams(pRQB, pHDB);
    } break;

    case POF_OPC_PRM_READ:
    {
      POF_UPPER_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, ">>>: pof_request: POF_OPC_PRM_READ, Handle:0x%X", POF_RQB_GET_HANDLE(pRQB));
      Response = POF_RequestPrmRead(pRQB, pHDB);
    } break;

    case POF_OPC_PRM_PREPARE:
    {
      POF_UPPER_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, ">>>: pof_request: POF_OPC_PRM_PREPARE, Handle:0x%X", POF_RQB_GET_HANDLE(pRQB));
      Response = POF_PrmState_Request(pRQB, pHDB, Opcode);
    } break;

    case POF_OPC_PRM_WRITE:
    {
      POF_UPPER_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, ">>>: pof_request: POF_OPC_PRM_WRITE, Handle:0x%X", POF_RQB_GET_HANDLE(pRQB));
      Response = POF_PrmState_Request(pRQB, pHDB, Opcode);
    } break;

    case POF_OPC_PRM_END:
    {
      POF_UPPER_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, ">>>: pof_request: POF_OPC_PRM_END, Handle:0x%X", POF_RQB_GET_HANDLE(pRQB));
      Response = POF_PrmState_Request(pRQB, pHDB, Opcode);
    } break;

    case POF_OPC_PRM_CHANGE_PORT:
    {
      POF_UPPER_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, ">>>: pof_request: POF_OPC_PRM_CHANGE_PORT, Handle:0x%X", POF_RQB_GET_HANDLE(pRQB));
      Response = POF_PrmState_Request(pRQB, pHDB, Opcode);
    } break;

    case POF_OPC_PRM_COMMIT:
    {
      POF_UPPER_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, ">>>: pof_request: POF_OPC_PRM_COMMIT, Handle:0x%X", POF_RQB_GET_HANDLE(pRQB));
      Response = POF_PrmState_Request(pRQB, pHDB, Opcode);
    } break;

    case POF_OPC_PRM_INDICATION:
    {
      POF_UPPER_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, ">>>: pof_request: POF_OPC_PRM_INDICATION, Handle:0x%X", POF_RQB_GET_HANDLE(pRQB));
      Response = POF_RequestPrmIndication(&bIndicate, pRQB, pHDB);
    } break;

    case POF_OPC_TEST_HARDWARE: //undocumented test-service
    {
      POF_UPPER_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, ">>>: pof_request: POF_OPC_TEST_HARDWARE, Handle:0x%X", POF_RQB_GET_HANDLE(pRQB));
      Response = POF_RequestTestHardware(pRQB, pHDB);
    } break;

    default:
    {
      POF_UPPER_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, ">>>: pof_request, invalid Opcode:0x%X", Opcode);
      Response = POF_RSP_ERR_OPCODE;
    } break;
  } /* switch */

  if (bIndicate)
  {
    POF_UserRequestFinish(pHDB, pRQB, Response);

    /* prm-indication must be called after commit.cnf */
    if ((POF_OPC_PRM_COMMIT == Opcode) && (Response == POF_RSP_OK))
    {
      /* validate diagnose and send prm_indication */
      POF_PRM_StateDiag_Validate_TimeOut(pHDB, LSA_TRUE); 
    }
  }

  POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: pof_request");
  
  POF_EXIT();
}
/*---------------------- end [subroutine] ---------------------------------*/


/*****************************************************************************/
/*  end of file pof_usr.c                                                    */
/*****************************************************************************/
