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
/*  F i l e               &F: pof_tim.c                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  POF-Timer functions                              */
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
#define LTRC_ACT_MODUL_ID  4
#define POF_MODULE_ID      LTRC_ACT_MODUL_ID

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/
#include "pof_inc.h"            /* POF headerfiles */
#include "pof_dmi_type.h"
#include "pof_prm_type.h"
#include "pof_int.h"            /* internal header */
#include "pof_glb.h"
#include "pof_timer.h"

POF_FILE_SYSTEM_EXTENSION(POF_MODULE_ID)

/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/

/*===========================================================================*/
/*                                global Variables                           */
/*===========================================================================*/

/*===========================================================================*/
/*                            local function declaration                     */
/*===========================================================================*/
static LSA_VOID POF_LOCAL_FCT_ATTR POF_TimerSet( 
       LSA_TIMER_ID_TYPE                      const TimerID,
       POF_UPPER_RQB_PTR_TYPE                 const pRQB,
       LSA_VOID                                     LSA_FCT_PTR(POF_LOCAL_FCT_ATTR, const Cbf) 
                                                               (POF_INTERNAL_REQUEST_PTR_TYPE pIntRQB),
       LSA_TIMER_ID_TYPE POF_LOCAL_MEM_ATTR * const pPOFTimerIndex);

/*===========================================================================*/
/*                                 Macros                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                 functions                                 */
/*===========================================================================*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_TimerMgmSetup()                         +*/
/*+  Input/Output          :    ---                                         +*/
/*+  Result                :    LSA_RESULT                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  Result:      POF_STS_OK                                                +*/
/*+               POF_STS_ERR_RESOURCE                                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Initializes the POF global Timer-Management               +*/
/*+               Note that the global management hast to be allocated.     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID  POF_LOCAL_FCT_ATTR  POF_TimerMgmSetup( LSA_VOID )
{
  LSA_UINT32  i;

  for (i= 0; i< POF_CFG_MAX_TIMER; i++)
  {
    POF_MGM_GET_GLOB_PTR()->Timer[i].bInUse = LSA_FALSE;
  }
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :   POF_TimerSet()                               +*/
/*+  Input/Output          :   LSA_UINT16                      TimerID      +*/
/*+                            POF_UPPER_RQB_PTR_TYPE          pRQB         +*/
/*+                            LSA_FCT_PTR(POF_LOCAL_FCT_ATTR, Cbf) (..)    +*/
/*+                            POF_LOCAL_MEM_U16_PTR_TYPE      pPOFTimerIndex+*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  TimerID      LSA-Timer ID put into timer-management                    +*/
/*+  pRQB         Pointer to intenal RQB put into timer-management          +*/
/*+  Cbf          Optional Callbackfunction called when timer elapses.      +*/
/*+               Gets pointer to IntRQB as parameter                       +*/
/*+  pPOFTimerIndex POF internal Timer ID (= index ) returned               +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Puts new Timer into Timer-management                      +*/
/*+               if a Cbf is present, it is called if the timer elapses.   +*/
/*+               if no Cbf is present, when the timer elapses the output   +*/
/*+               Macro POF_DO_TIMER_REQUEST is called with the rqb         +*/
/*+               specified (only if rqb is not underway).                  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static  LSA_VOID  POF_LOCAL_FCT_ATTR  POF_TimerSet( LSA_TIMER_ID_TYPE                         const  TimerID,
                                                    POF_UPPER_RQB_PTR_TYPE                    const  pRQB,
                                                    LSA_VOID                                         LSA_FCT_PTR(POF_LOCAL_FCT_ATTR, const Cbf) 
                                                                                                                (POF_INTERNAL_REQUEST_PTR_TYPE  pIntRQB),
                                                    LSA_TIMER_ID_TYPE  POF_LOCAL_MEM_ATTR  *  const  pPOFTimerIndex )
{
  LSA_UINT32               Ctr;
  POF_TIMER_INFO_PTR_TYPE  pTimer = POF_MGM_GET_GLOB_PTR()->Timer;

  for (Ctr = 0; Ctr < POF_CFG_MAX_TIMER; pTimer++, Ctr++)
  {
    if (!pTimer->bInUse)
    {
      pTimer->TimerID       = TimerID;
      pTimer->IntRQB.pRQB   = pRQB;
      pTimer->IntRQB.bInUse = LSA_FALSE;
      pTimer->Cbf           = Cbf;

      /* setup RQB */
      POF_RQB_SET_OPCODE(pRQB, POF_OPC_TIMER);

      /* POF-Timer ID */
      pRQB->args.Timer.POFTimerIndex = (LSA_TIMER_ID_TYPE)Ctr;

      /* set bInUse at last to be sure other entries are valid */
      pTimer->bInUse = LSA_TRUE;

      *pPOFTimerIndex = (LSA_TIMER_ID_TYPE)Ctr;

      return;
    }
  }

  POF_PROGRAM_TRACE_00(0, LSA_TRACE_LEVEL_FATAL, "POF_TimerSet, no more free Timer-Resources, POF_CFG_MAX_TIMER wrong?");
  POF_FATAL("POF_TimerSet", POF_FATAL_ERR_TIMER_RESOURCE, 0, 0);
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :   POF_TimerAlloc()                             +*/
/*+  Input/Output              LSA_UINT16                      TimerType    +*/
/*+                            LSA_UINT16                      TimerBase    +*/
/*+                            LSA_UINT32                      UserID       +*/
/*+                            LSA_FCT_PTR(POF_LOCAL_FCT_ATTR, Cbf) (..)    +*/
/*+                            POF_LOCAL_MEM_U16_PTR_TYPE      pPOFTimerIndex+*/
/*+                                                                         +*/
/*+  Result                :   POF_RSP_OK                                   +*/
/*+                            POF_RSP_ERR_RESOURCE                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  TimerType    LSA-TimerType                                             +*/
/*+               LSA_TIMER_TYPE_ONE_SHOT                                   +*/
/*+               LSA_TIMER_TYPE_CYCLIC                                     +*/
/*+  TimerBase    LSA-TimerBase                                             +*/
/*+               LSA_TIME_BASE_1MS etc                                     +*/
/*+  UserID       UserID put into RQB send on timer-event.                  +*/
/*+  Cbf          Optional Callbackfunction called when timer elapses.      +*/
/*+               Gets pointer to IntRQB as parameter                       +*/
/*+               LSA_NULL for standardhandling                             +*/
/*+  pPOFTimerIndex POF internal Timer ID (= index ) returned               +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Allocates LSA-Timer and setup POF-Timer management        +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
POF_RSP  POF_LOCAL_FCT_ATTR  POF_TimerAlloc( LSA_UINT16                         const  TimerType,
                                             LSA_UINT16                         const  TimerBase,
                                             LSA_UINT32                         const  UserID,
                                             LSA_VOID                                  LSA_FCT_PTR(POF_LOCAL_FCT_ATTR, const  Cbf) 
                                                                                       (POF_INTERNAL_REQUEST_PTR_TYPE  pIntRQB),
                                             LSA_TIMER_ID_TYPE  POF_LOCAL_MEM_ATTR  *  pPOFTimerIndex )
{
  LSA_UINT16              RetVal;
  LSA_TIMER_ID_TYPE       TimerID;
  POF_UPPER_RQB_PTR_TYPE  pRQB;

  POF_FUNCTION_TRACE_03(0, LSA_TRACE_LEVEL_CHAT, "IN: POF_TimerAlloc, Type:0x%X Base:0x%X UserID:0x%X", TimerType, TimerBase, UserID);

  POF_ALLOC_UPPER_RQB_LOCAL(&pRQB, (LSA_UINT16)sizeof(POF_RQB_TYPE));
  if (LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL))
  {
    POF_PROGRAM_TRACE_00(0, LSA_TRACE_LEVEL_ERROR, "pof_TimerAlloc, Allocation RQB memory failed");
    return POF_RSP_ERR_RESOURCE;
  }

  POF_SYSTEM_TRACE_02(0, LSA_TRACE_LEVEL_NOTE, "pof_TimerAlloc, Calling POF_ALLOC_TIMER, TimerType:0x%X TimerBase:0x%X", TimerType, TimerBase);

  POF_ALLOC_TIMER(&RetVal, &TimerID, TimerType, TimerBase);

  if (RetVal != LSA_RET_OK)
  {
    POF_SYSTEM_TRACE_01(0, LSA_TRACE_LEVEL_ERROR, "pof_TimerAlloc, Calling POF_ALLOC_TIMER failed, Response:0x%X", RetVal);
    POF_FREE_UPPER_RQB_LOCAL(&RetVal,pRQB);
    POF_MEM_FREE_FAIL_CHECK(RetVal);
    return POF_RSP_ERR_RESOURCE;
  }

  POF_SYSTEM_TRACE_01(0, LSA_TRACE_LEVEL_NOTE, "<--: Calling POF_ALLOC_TIMER success, TimerID:%d", (LSA_UINT32)TimerID);

  /* setup RQB-user-ID */
  pRQB->args.Timer.UserID = UserID;

  POF_TimerSet(TimerID, pRQB, Cbf, pPOFTimerIndex);

  POF_FUNCTION_TRACE_01(0, LSA_TRACE_LEVEL_CHAT, "OUT: POF_TimerAlloc, POFTimerIndex:0x%X", *pPOFTimerIndex);

  return POF_RSP_OK;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_TimerFree()                             +*/
/*+  Input/Output          :    LSA_UINT16                   POFTimerIndex  +*/
/*+  Result                :    POF_RSP_OK                                  +*/
/*+                             POF_RPS_ERR_SEQUENCE                        +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  POFTimerIndex   POF-Timer ID to free in management                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Frees timer in timermanagement and releases LSA-Timer     +*/
/*+               Fees internal RQB. If RQB still in use we abort with error+*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
POF_RSP  POF_LOCAL_FCT_ATTR  POF_TimerFree( LSA_TIMER_ID_TYPE  const  POFTimerIndex )

{
  POF_TIMER_INFO_PTR_TYPE  pTimer;
  LSA_UINT16               RetVal;
  LSA_TIMER_ID_TYPE        TimerID;

  POF_FUNCTION_TRACE_01(0, LSA_TRACE_LEVEL_CHAT, "IN: POF_TimerFree, POFTimerIndex:0x%X", POFTimerIndex);

  if (POFTimerIndex >= POF_CFG_MAX_TIMER)
  {
    POF_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_FATAL, "POF_TimerFree, Wrong Timer ID, POFTimerIndex:0x%X", POFTimerIndex);
    POF_FATAL("POF_TimerFree", POF_FATAL_ERR_INCONSISTENZ, 0, 0);
    //lint -unreachable
    return LSA_RET_ERR_PARAM;
  }

  pTimer = &POF_MGM_GET_GLOB_PTR()->Timer[POFTimerIndex];

  if (!pTimer->bInUse)
  {
    POF_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_NOTE_HIGH, "pof_TimerFree, timer not in use, POFTimerIndex:0x%X", POFTimerIndex);
    return POF_RSP_OK;
  }

  if (pTimer->IntRQB.bInUse)
  {
    POF_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_NOTE_HIGH, "pof_TimerFree, RQB still in use, POFTimerIndex:0x%X", POFTimerIndex);
    return POF_RSP_ERR_SEQUENCE;
  }

  TimerID = pTimer->TimerID;

  POF_STOP_TIMER(&RetVal, TimerID); //just for sure. don't matter if already stopped

  switch (RetVal)
  {
    case POF_RSP_OK_TIMER_NOT_RUNNING:
    case POF_RSP_OK:                     
      break;
  
    default: 
    {
      POF_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_FATAL, "POF_TimerFree, POF_STOP_TIMER Wrong Timer ID, POFTimerIndex:0x%X", POFTimerIndex);
      POF_FATAL("POF_TimerFree, POF_STOP_TIMER", POF_FATAL_ERR_INCONSISTENZ, 0, 0);
      //lint -unreachable
      return LSA_RET_ERR_PARAM;
    }
  }  

  POF_FREE_TIMER(&RetVal,TimerID);
  POF_MEM_FREE_FAIL_CHECK(RetVal);

  pTimer->bInUse = LSA_FALSE;

  /* free internal RQB for Timer */
  POF_FREE_UPPER_RQB_LOCAL(&RetVal, pTimer->IntRQB.pRQB);
  POF_MEM_FREE_FAIL_CHECK(RetVal);

  pTimer->IntRQB.pRQB   = LSA_NULL;
  pTimer->IntRQB.bInUse = LSA_FALSE;

  POF_FUNCTION_TRACE_00(0, LSA_TRACE_LEVEL_CHAT, "OUT: POF_TimerFree");

  return POF_RSP_OK;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_TimerStart()                            +*/
/*+  Input/Output          :    LSA_UINT16                   POFTimerIndex  +*/
/*+                             LSA_UINT16                   Time           +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  POFTimerIndex      POF-Timer ID to free in management                  +*/
/*+  Time               running time                                        +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Starts Timer.                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID  POF_LOCAL_FCT_ATTR  POF_TimerStart( LSA_TIMER_ID_TYPE  const  POFTimerIndex,
                                              LSA_UINT16         const  Time )
{
  LSA_UINT16               RetVal;
  POF_TIMER_INFO_PTR_TYPE  pTimer;
  LSA_USER_ID_TYPE         Dummy;

  POF_FUNCTION_TRACE_02(0, LSA_TRACE_LEVEL_CHAT, "IN: POF_TimerStart, POFTimerIndex:0x%X Time:0x%X", POFTimerIndex, Time);

  if (POFTimerIndex >= POF_CFG_MAX_TIMER)
  {
    POF_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_FATAL, "POF_TimerStart, Wrong Timer ID, POFTimerIndex:0x%X", POFTimerIndex);
    POF_FATAL("POF_TimerStart", POF_FATAL_ERR_INCONSISTENZ, 0, 0);
    //lint -unreachable
    return;
  }

  pTimer = &POF_MGM_GET_GLOB_PTR()->Timer[POFTimerIndex];

  if (!pTimer->bInUse)
  {
    POF_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_FATAL, "POF_TimerStart, Timer not in use, POFTimerIndex:0x%X", POFTimerIndex);
    POF_FATAL("POF_TimerStart", POF_FATAL_ERR_INCONSISTENZ, 0, 0);
    //lint -unreachable
    return;
  }

  if (0 == Time)
  {
    POF_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_FATAL, "POF_TimerStart, 0 == Time, POFTimerIndex:0x%X", POFTimerIndex);
    POF_FATAL("POF_TimerStart", POF_FATAL_ERR_INCONSISTENZ, 0, 0);
    //lint -unreachable
    return;
  }

  Dummy.uvar32 = POFTimerIndex; /* we use user-id for POF-Timer ID */

  POF_START_TIMER(&RetVal, pTimer->TimerID, Dummy, Time);

  if (RetVal == LSA_RET_ERR_PARAM)
  {
    POF_SYSTEM_TRACE_01(0, LSA_TRACE_LEVEL_FATAL, "<--: Calling POF_START_TIMER failed, RetVal:0x%X", RetVal);
    POF_FATAL("POF_TimerStart", POF_FATAL_ERR_INCONSISTENZ, 0, 0);
    //lint -unreachable
    return;
  }

  POF_FUNCTION_TRACE_00(0, LSA_TRACE_LEVEL_CHAT, "OUT: POF_TimerStart");  
} 
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_TimerStop()                             +*/
/*+  Input/Output          :    LSA_UINT16                   POFTimerIndex  +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  POFTimerIndex   POF-Timer ID to free in management                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Starts Timer.                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID  POF_LOCAL_FCT_ATTR  POF_TimerStop( LSA_TIMER_ID_TYPE  const  POFTimerIndex )
{
  LSA_UINT16               RetVal;
  POF_TIMER_INFO_PTR_TYPE  pTimer;

  POF_FUNCTION_TRACE_01(0, LSA_TRACE_LEVEL_CHAT, "IN: POF_TimerStop, POFTimerIndex:0x%X", (LSA_UINT32)POFTimerIndex);

  if (POFTimerIndex >= POF_CFG_MAX_TIMER)
  {
    POF_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_FATAL, "POF_TimerStop, Wrong Timer ID, POFTimerIndex:0x%X", POFTimerIndex);
    POF_FATAL("POF_TimerStop", POF_FATAL_ERR_INCONSISTENZ, 0, 0);
    //lint -unreachable
    return;
  }

  pTimer = &POF_MGM_GET_GLOB_PTR()->Timer[POFTimerIndex];

  if (!pTimer->bInUse)
  {
    POF_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_FATAL, "POF_TimerStop, Timer not in use, POFTimerIndex:0x%X", POFTimerIndex);
    POF_FATAL("POF_TimerStop", POF_FATAL_ERR_INCONSISTENZ, 0, 0);
    //lint -unreachable
    return;
  }

  POF_STOP_TIMER(&RetVal, pTimer->TimerID);

  POF_FUNCTION_TRACE_00(0, LSA_TRACE_LEVEL_CHAT, "OUT: POF_TimerStop");
} 
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_TimerRQBFree()                          +*/
/*+  Input/Output          :    LSA_UINT16                   POFTimerIndex  +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  POFTimerIndex   POF-Timer ID to free in management                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Called after RQB was received and is free for further use +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID  POF_LOCAL_FCT_ATTR  POF_TimerRQBFree( LSA_TIMER_ID_TYPE  const  POFTimerIndex )
{
  POF_TIMER_INFO_PTR_TYPE  pTimer;

  if (POFTimerIndex >= POF_CFG_MAX_TIMER)
  {
    POF_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_FATAL, "POF_TimerRQBFree, Invalid Timer-ID, POFTimerIndex:0x%X", POFTimerIndex);
    POF_FATAL("POF_TimerRQBFree", POF_FATAL_ERR_INCONSISTENZ, POFTimerIndex, 0);
    //lint -unreachable
    return;
  }

  pTimer = &POF_MGM_GET_GLOB_PTR()->Timer[POFTimerIndex];
      
  if (!pTimer->bInUse)
  {
    POF_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_FATAL, "POF_TimerRQBFree, LSA_FALSE == pTimer->bInUse, POFTimerIndex:0x%X", POFTimerIndex);
    POF_FATAL("POF_TimerRQBFree", POF_FATAL_ERR_INCONSISTENZ, POFTimerIndex, 0);
    //lint -unreachable
    return;
  }

  pTimer->IntRQB.bInUse = LSA_FALSE;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    pof_timeout()                               +*/
/*+  Input/Output          :    LSA_UINT16        timer_id                  +*/
/*+                             LSA_USER_ID_TYPE  user_id                   +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  timer_id     : ID of Timer                                             +*/
/*+  user_id      : Used for POFTimerIndex                                  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Called from systemadaption on timerevent. May be called   +*/
/*+               within ISR-Handler.                                       +*/
/*+                                                                         +*/
/*+               LSA-Systemfunction                                        +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID  POF_SYSTEM_IN_FCT_ATTR  pof_timeout( LSA_TIMER_ID_TYPE  const  timer_id,
                                               LSA_USER_ID_TYPE   const  user_id )
{
  POF_TIMER_INFO_PTR_TYPE  pTimer;
  LSA_TIMER_ID_TYPE        POFTimerIndex;

  POFTimerIndex = (LSA_TIMER_ID_TYPE)user_id.uvar16;
    
  POF_FUNCTION_TRACE_02(0, LSA_TRACE_LEVEL_CHAT, "IN: pof_timeout, timer_id:0x%X user_id:0x%X", timer_id, user_id.uvar32);
  
  if (POFTimerIndex >= POF_CFG_MAX_TIMER)
  {
    POF_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_FATAL, "pof_timeout, Invalid Timer-ID, POFTimerIndex:0x%X", POFTimerIndex);
    POF_FATAL("pof_timeout, POFTimerIndex:", POF_FATAL_ERR_INCONSISTENZ, POFTimerIndex, 0);
    //lint -unreachable
    return;
  }

  pTimer = &POF_MGM_GET_GLOB_PTR()->Timer[POFTimerIndex];

  if (!pTimer->bInUse)
  {
    /* Race Condition */
    POF_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_NOTE_HIGH, "pof_timeout, LSA_FALSE == pTimer->bInUse, POFTimerIndex:0x%X", POFTimerIndex);

    return;
  }

  if (pTimer->TimerID != timer_id)
  {
    /* Race Condition */
    POF_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_NOTE_HIGH, "pof_timeout, pTimer->TimerID != timer_id, POFTimerIndex:0x%X", POFTimerIndex);

    return;
  }

  pTimer->Cbf(&pTimer->IntRQB);

  POF_FUNCTION_TRACE_00(0, LSA_TRACE_LEVEL_CHAT, "OUT: pof_timeout");
}
/*---------------------- end [subroutine] ---------------------------------*/


/*****************************************************************************/
/*  end of file pof_tim.c                                                    */
/*****************************************************************************/


