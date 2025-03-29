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
/*  F i l e               &F: pof_sys.c                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  POF-system functions                             */
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
#define LTRC_ACT_MODUL_ID  2
#define POF_MODULE_ID      LTRC_ACT_MODUL_ID

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/
#include "pof_inc.h"            /* POF headerfiles */
#include "pof_dmi_type.h"
#include "pof_prm_type.h"
#include "pof_int.h"            /* internal header */
#include "pof_prm_req.h"
#include "pof_timer.h"
#include "pof_glb.h"

POF_FILE_SYSTEM_EXTENSION(POF_MODULE_ID)

/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/

/*===========================================================================*/
/*                                global Variables                           */
/*===========================================================================*/
POF_LOCAL_MEM_ATTR  POF_MGM_PTR_TYPE  g_pPOFInfo = LSA_NULL;

/*===========================================================================*/
/*                            local function declaration                     */
/*===========================================================================*/

/*===========================================================================*/
/*                                 Macros                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                 functions                                 */
/*===========================================================================*/

/*===========================================================================*/
/*                             external functions                            */
/*===========================================================================*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_FreeMgm()                               +*/
/*+  Input/Output          :    ---                                         +*/
/*+  Result                :    POF_RSP_OK                                  +*/
/*+                             POF_RSP_ERR_SEQUENCE                        +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Frees POF Managmenent if allocated.                       +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static  POF_RSP  POF_LOCAL_FCT_ATTR  POF_FreeMgm( LSA_VOID )
{
  POF_RSP     Response;
  LSA_UINT16  FreeStat;

  Response = POF_RSP_OK;

  if (!LSA_HOST_PTR_ARE_EQUAL(g_pPOFInfo, LSA_NULL))
  {
    /* ----------------------------------------------------------------------*/
    /* free Timer if present                                                 */
    /* ----------------------------------------------------------------------*/
    if (g_pPOFInfo->Glob.CyclicTimerIndex != POF_TIMER_ID_INVALID)
    {
      Response = POF_TimerFree(g_pPOFInfo->Glob.CyclicTimerIndex);

      /* Note: We don't expect the Timer RQB to be underway! */
      if (Response != POF_RSP_OK)
      {
        POF_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_FATAL, "pof_FreeMgm, POF_TimerFree, Response:0x%X", Response);
        POF_FATAL("POF_FreeMgm", POF_FATAL_ERR_INCONSISTENZ, 0, 0);
        //lint -unreachable
        return LSA_RET_ERR_PARAM;
      }
       
      g_pPOFInfo->Glob.CyclicTimerIndex = POF_TIMER_ID_INVALID;
    }

    /* ----------------------------------------------------------------------*/
    /* free global management if not static                                  */
    /* ----------------------------------------------------------------------*/
    POF_FREE_LOCAL_MEM(&FreeStat, g_pPOFInfo);

    POF_MEM_FREE_FAIL_CHECK(FreeStat);
    
    g_pPOFInfo = LSA_NULL;
  }

  return Response;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    pof_init()                                  +*/
/*+  Input/Output          :    ---                                         +*/
/*+  Result                :    POF_RESP                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Result:      POF_RSP_OK                                                +*/
/*+               POF_RSP_ERR_RESOURCE                                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Initializes the POF.                                      +*/
/*+                                                                         +*/
/*+               Note: Must only be called if not already initialized, not +*/
/*+                     checked!                                            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
POF_RSP  POF_SYSTEM_IN_FCT_ATTR  pof_init( LSA_VOID )
{
  LSA_UINT32  Ctr;
  LSA_UINT32  POFInfoAllocSize;
  POF_RSP     Status;

  POF_ENTER();

  POF_FUNCTION_TRACE_00(0, LSA_TRACE_LEVEL_CHAT, "IN: pof_init");

  g_pPOFInfo = LSA_NULL; //There are some compilers, which doesn't init global variables!

  #if defined (POF_UNUSED)
  /* pof_init already called, but without pof_undo_init */
  if (!LSA_HOST_PTR_ARE_EQUAL(g_pPOFInfo, LSA_NULL))
  {
    POF_PROGRAM_TRACE_00(0, LSA_TRACE_LEVEL_NOTE_HIGH, "pof_init, g_pPOFInfo != LSA_NULL");
    POF_EXIT();
    return POF_RSP_ERR_SEQUENCE;
  }
  #endif

  /* --------------------------------------------------------------------------*/
  /* check sizeof struct                                                       */
  /* --------------------------------------------------------------------------*/
  Ctr = I2C_DMI_MAP_A2_SIZE_8;
  if (sizeof(POF_DMI_MAP_A2_TYPE) != Ctr)                       //lint !e774 Boolean within 'if' always evaluates to False, define-based behaviour, HM 14.01.2016
  {
    POF_PROGRAM_TRACE_00(0, LSA_TRACE_LEVEL_NOTE_HIGH, "pof_init, invalid sizeof(POF_DMI_MAP_A2_TYPE)");
    POF_EXIT();
    return POF_RSP_ERR_PARAM;
  }

  Ctr = I2C_DMI_MAP_A2_SIZE_8;
  if (sizeof(I2C_A2_MIRROR_TYPE) != Ctr)                        //lint !e774 Boolean within 'if' always evaluates to False, define-based behaviour, HM 14.01.2016
  {
    POF_PROGRAM_TRACE_00(0, LSA_TRACE_LEVEL_NOTE_HIGH, "pof_init, invalid sizeof(I2C_A2_MIRROR_TYPE)");
    POF_EXIT();
    return POF_RSP_ERR_PARAM;
  }

  Ctr = 8;
  if (sizeof(POF_PRM_RECORD_HEADER_TYPE) != Ctr)                //lint !e774 Boolean within 'if' always evaluates to False, define-based behaviour, HM 14.01.2016
  {
    POF_PROGRAM_TRACE_00(0, LSA_TRACE_LEVEL_NOTE_HIGH, "pof_init, invalid sizeof(POF_PRM_RECORD_HEADER_TYPE)");
    POF_EXIT();
    return POF_RSP_ERR_PARAM;
  }

  Ctr = POF_PRM_FO_DATA_REAL_LENGTH;
  if (sizeof(POF_PRM_RECORD_FO_DATA_REAL_TYPE) != Ctr)          //lint !e774 Boolean within 'if' always evaluates to False, define-based behaviour, HM 14.01.2016
  {
    POF_PROGRAM_TRACE_00(0, LSA_TRACE_LEVEL_NOTE_HIGH, "pof_init, invalid sizeof(POF_PRM_RECORD_FO_DATA_REAL_TYPE)");
    POF_EXIT();
    return POF_RSP_ERR_PARAM;
  }

  Ctr = POF_PRM_FO_DATA_CHECK_LENGTH;
  if (sizeof(POF_PRM_RECORD_FO_DATA_CHECK_TYPE) != Ctr)         //lint !e774 Boolean within 'if' always evaluates to False, define-based behaviour, HM 14.01.2016
  {
    POF_PROGRAM_TRACE_00(0, LSA_TRACE_LEVEL_NOTE_HIGH, "pof_init, invalid sizeof(POF_PRM_RECORD_FO_DATA_CHECK_TYPE)");
    POF_EXIT();
    return POF_RSP_ERR_PARAM;
  }

  Ctr = POF_PRM_FO_DATA_ADJUST_LENGTH;
  if (sizeof(POF_PRM_RECORD_FO_DATA_ADJUST_TYPE) != Ctr)        //lint !e774 Boolean within 'if' always evaluates to False, define-based behaviour, HM 14.01.2016
  {
    POF_PROGRAM_TRACE_00(0, LSA_TRACE_LEVEL_NOTE_HIGH, "pof_init, invalid sizeof(POF_PRM_RECORD_FO_DATA_ADJUST_TYPE)");
    POF_EXIT();
    return POF_RSP_ERR_PARAM;
  }

  Ctr = POF_PRM_FO_DATA_TRA_IND_TEST_LENGTH;
  if (sizeof(POF_PRM_RECORD_TRANSCEIVER_INDENT_TYPE) != Ctr)    //lint !e774 Boolean within 'if' always evaluates to False, define-based behaviour, HM 14.01.2016
  {
    POF_PROGRAM_TRACE_00(0, LSA_TRACE_LEVEL_NOTE_HIGH, "pof_init, invalid sizeof(POF_PRM_RECORD_TRANSCEIVER_INDENT_TYPE)");
    POF_EXIT();
    return POF_RSP_ERR_PARAM;
  }

  /* --------------------------------------------------------------------------*/
  /* we allocate memory                                                        */
  /* --------------------------------------------------------------------------*/
  POFInfoAllocSize = (LSA_UINT32)sizeof(POF_MGM_TYPE);
  //round up size for a performant MEMSET
  POFInfoAllocSize = (POFInfoAllocSize + 7UL) & 0xFFFFFFF8UL;
  POF_ALLOC_LOCAL_MEM((POF_LOCAL_MEM_PTR_TYPE POF_LOCAL_MEM_ATTR *)&g_pPOFInfo, POFInfoAllocSize);

  if (LSA_HOST_PTR_ARE_EQUAL(g_pPOFInfo, LSA_NULL))
  {
    POF_PROGRAM_TRACE_00(0, LSA_TRACE_LEVEL_NOTE_HIGH, "pof_init, Allocating memory failed");
    POF_EXIT();
    return POF_RSP_ERR_RESOURCE;
  }

  POF_MEMSET_LOCAL(g_pPOFInfo, (LSA_UINT8)0, POFInfoAllocSize);

  /* --------------------------------------------------------------------------*/
  /* Setup management                                                          */
  /* --------------------------------------------------------------------------*/
  g_pPOFInfo->HUser.MaxHandleCnt = (LSA_UINT16)POF_CFG_MAX_CHANNELS;
  //POFInfo already nulled! g_pPOFInfo->HUser.UsedHandleCnt = 0;

  for (Ctr = 0; Ctr < POF_CFG_MAX_CHANNELS; Ctr++)
  {
    g_pPOFInfo->HUser.HDB[Ctr].State      = POF_HANDLE_STATE_FREE;
    g_pPOFInfo->HUser.HDB[Ctr].ThisHandle = (LSA_HANDLE_TYPE)Ctr;
  }

  /* ----------------------------------------------------------------------*/
  /* Setup Timer-Management                                                */
  /* ----------------------------------------------------------------------*/
  POF_TimerMgmSetup();

  /* ----------------------------------------------------------------------*/
  /* Allocate Send/TTL-Cyclic Timer                                        */
  /* ----------------------------------------------------------------------*/
  g_pPOFInfo->Glob.CyclicTimerIndex = POF_TIMER_ID_INVALID;
  //POFInfo already nulled! g_pPOFInfo->Glob.CyclicTimerUserCnt = 0;

  Status = POF_TimerAlloc((LSA_UINT16)LSA_TIMER_TYPE_CYCLIC,
                          (LSA_UINT16)LSA_TIME_BASE_1S,
                          POF_TIMER_USER_ID_CYLIC,
                          POF_CylicTimerCbf,
                          &g_pPOFInfo->Glob.CyclicTimerIndex);

  if (Status != POF_RSP_OK)
  {
    (LSA_VOID)POF_FreeMgm();
    POF_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_NOTE_HIGH, "pof_init, POF_TimerAlloc, Status:0x%X", Status);
    POF_EXIT();
    return POF_RSP_ERR_RESOURCE;
  }

  g_pPOFInfo->Glob.Excp.Error = POF_FATAL_NO_ERROR;

  POF_FUNCTION_TRACE_01(0, LSA_TRACE_LEVEL_CHAT, "OUT: pof_init, Status:0x%X", Status);

  POF_EXIT();

  return POF_RSP_OK;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    pof_undo_init()                             +*/
/*+  Input/Output          :    ---                                         +*/
/*+  Result                :    POF_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Result:      POF_RSP_OK                                                +*/
/*+               POF_RSP_ERR_SEQUENCE                                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Uninizializes POF.                                        +*/
/*+               This is only possible if no device is open                +*/
/*+               POF_RSP_ERR_SEQUENCE if device is open.                   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
POF_RSP  POF_SYSTEM_IN_FCT_ATTR  pof_undo_init( LSA_VOID )
{
  POF_RSP  Status;

  POF_ENTER();

  POF_FUNCTION_TRACE_00(0, LSA_TRACE_LEVEL_CHAT, "IN: pof_undo_init");

  if (LSA_HOST_PTR_ARE_EQUAL(g_pPOFInfo, LSA_NULL))
  {
    POF_PROGRAM_TRACE_00(0, LSA_TRACE_LEVEL_NOTE_HIGH, "pof_undo_init, g_pPOFInfo == LSA_NULL");
    POF_EXIT();
    return POF_RSP_ERR_SEQUENCE;    
  }

  if (POF_MGM_GET_HUSER_PTR()->UsedHandleCnt)
  {
    POF_PROGRAM_TRACE_00(0, LSA_TRACE_LEVEL_NOTE_HIGH, "pof_undo_init, Not all channels closed yet");
    POF_EXIT();
    return POF_RSP_ERR_SEQUENCE;
  }
  
  Status = POF_FreeMgm();
  if (POF_RSP_OK != Status)
  {
    POF_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_NOTE_HIGH, "pof_undo_init, POF_FreeMgm, invalid Status:0x%X", Status);
    POF_EXIT();
    return POF_RSP_ERR_RESOURCE;
  }

  POF_FUNCTION_TRACE_00(0, LSA_TRACE_LEVEL_CHAT, "OUT: pof_undo_init");

  POF_EXIT();

  return POF_RSP_OK;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*****************************************************************************/
/*  end of file pof_sys.c                                                    */
/*****************************************************************************/

