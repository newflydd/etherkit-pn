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
/*  F i l e               &F: pof_glb.c                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  POF-global functions                             */
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
#define LTRC_ACT_MODUL_ID  3
#define POF_MODULE_ID      LTRC_ACT_MODUL_ID

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/
#include "pof_inc.h"            /* POF headerfiles */
#include "pof_dmi_type.h"
#include "pof_prm_type.h"
#include "pof_int.h"            /* internal header */
#include "pof_glb.h"

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

/*===========================================================================*/
/*                                 Macros                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                 functions                                 */
/*===========================================================================*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_FatalError()                            +*/
/*+  Input/Output               POF_FATAL_ERROR_TYPE Error                  +*/
/*+  Input/Output          :    LSA_UINT16           ModuleID               +*/
/*+                             LSA_UINT16           Line                   +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  Error                : POF-fatal-errorcode                             +*/
/*+  ModuleID             : module - id of error                            +*/
/*+  Line                 : line of code (optional)                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Signals Fatal-error via POF_FATAL_ERROR macro.            +*/
/*+               This function does not return!                            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID  POF_LOCAL_FCT_ATTR  POF_FatalError( LSA_UINT32     const  Line,
                                              LSA_UINT8   *  const  sFile,
                                              LSA_UINT32     const  ModuleID,
                                              LSA_UINT8   *  const  sErr,
                                              LSA_UINT32     const  Error,
                                              LSA_UINT32     const  DW_0,
                                              LSA_UINT32     const  DW_1 )
{
  LSA_UINT8             *  p;
  LSA_UINT32               i, start;
  LSA_FATAL_ERROR_TYPE     LSAError;

  if (POF_MGM_GET_GLOB_PTR()->Excp.Error != POF_FATAL_NO_ERROR)
  {
    return;
  }

  LSAError.lsa_component_id  = LSA_COMP_ID_POF;
  LSAError.module_id         = (LSA_UINT16)ModuleID;
  LSAError.line              = (LSA_UINT16)Line;
  LSAError.error_code[0]     = Error;
  LSAError.error_code[1]     = POF_MODULE_ID;
  LSAError.error_code[2]     = DW_0;
  LSAError.error_code[3]     = DW_1;
  LSAError.error_data_length = POF_MAX_EXCP_INFO;

  POF_MGM_GET_GLOB_PTR()->Excp.Error    = Error;
  POF_MGM_GET_GLOB_PTR()->Excp.Line     = Line;
  POF_MGM_GET_GLOB_PTR()->Excp.ModuleID = ModuleID;

  POF_MEMSET_LOCAL(POF_MGM_GET_GLOB_PTR()->Excp.sInfo, (LSA_UINT8)0, (LSA_UINT32)POF_MAX_EXCP_INFO);

  start = 0;

  if (0 != sFile)
  {
    p = sFile;

    for (i = 0; i < 256; i++)
    {
      if (*p == '\\' )
      {
       start = 0;
       p++;
       continue;
      }
      if ((*p == 0) || (start == (POF_MAX_EXCP_INFO - 5)))
      {
        POF_MGM_GET_GLOB_PTR()->Excp.sInfo[start] = ' ';
        start++;
        POF_MGM_GET_GLOB_PTR()->Excp.sInfo[start] = '=';
        start++;
        POF_MGM_GET_GLOB_PTR()->Excp.sInfo[start] = '>';
        start++;
        POF_MGM_GET_GLOB_PTR()->Excp.sInfo[start] = ' ';
        start++;
        break;
      }

      POF_MGM_GET_GLOB_PTR()->Excp.sInfo[start] = *p;
      p++;
      start++;
    }

    if (sErr)
    {
      p = sErr;
    }
    else
    {
      p = (LSA_UINT8 *)(void *)"No Message specified!";
    }

    for (i = start; i < (POF_MAX_EXCP_INFO - 1); i++)
    {
      if (*p == 0)
      {
        break;
      }

      POF_MGM_GET_GLOB_PTR()->Excp.sInfo[i] = *p;
      p++;
    }
  }
  else /* POF_CFG_NO_FATAL_FILE_INFO */
  {
    POF_MGM_GET_GLOB_PTR()->Excp.sInfo[start] = 'N';
    start++;
    POF_MGM_GET_GLOB_PTR()->Excp.sInfo[start] = 'o';
    start++;
    POF_MGM_GET_GLOB_PTR()->Excp.sInfo[start] = ' ';
    start++;
    POF_MGM_GET_GLOB_PTR()->Excp.sInfo[start] = 'F';
    start++;
    POF_MGM_GET_GLOB_PTR()->Excp.sInfo[start] = 'i';
    start++;
    POF_MGM_GET_GLOB_PTR()->Excp.sInfo[start] = 'l';
    start++;
    POF_MGM_GET_GLOB_PTR()->Excp.sInfo[start] = 'e';
    start++;
    POF_MGM_GET_GLOB_PTR()->Excp.sInfo[start] = ' ';
    start++;
    POF_MGM_GET_GLOB_PTR()->Excp.sInfo[start] = 'I';
    start++;
    POF_MGM_GET_GLOB_PTR()->Excp.sInfo[start] = 'n';
    start++;
    POF_MGM_GET_GLOB_PTR()->Excp.sInfo[start] = 'f';
    start++;
    POF_MGM_GET_GLOB_PTR()->Excp.sInfo[start] = 'o';
    start++;

    POF_MGM_GET_GLOB_PTR()->Excp.sInfo[start] = ' ';
    start++;
    POF_MGM_GET_GLOB_PTR()->Excp.sInfo[start] = '=';
    start++;
    POF_MGM_GET_GLOB_PTR()->Excp.sInfo[start] = '>';
    start++;
    POF_MGM_GET_GLOB_PTR()->Excp.sInfo[start] = ' ';
  }

  LSAError.error_data_ptr = POF_MGM_GET_GLOB_PTR()->Excp.sInfo; 

  POF_FATAL_ERROR((LSA_UINT16)sizeof(LSA_FATAL_ERROR_TYPE), &LSAError);
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_CallCbf()                               +*/
/*+  Input/Output               POF_UPPER_CALLBACK_FCT_PTR_TYPE const Cbf   +*/
/*+                             POF_UPPER_RQB_PTR_TYPE          const pRQB  +*/
/*+                             LSA_SYS_PTR_TYPE                const pSys  +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Calls Cbf with RQB.                                       +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID  POF_LOCAL_FCT_ATTR  POF_CallCbf( LSA_UINT32                       const  TraceIdx,
                                           POF_UPPER_CALLBACK_FCT_PTR_TYPE  const  pCbf,
                                           POF_UPPER_RQB_PTR_TYPE           const  pRQB,
                                           LSA_SYS_PTR_TYPE                 const  pSys )
{
    POF_FUNCTION_TRACE_03(TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_CallCbf, Cbf:0x%X pRQB:0x%X Response:0x%X",
                          pCbf, pRQB, (POF_RQB_GET_RESPONSE(pRQB)));

    #if defined (POF_CFG_TRACE_RQB_ERRORS)
    switch (POF_RQB_GET_RESPONSE(pRQB)) 
    {
        case POF_RSP_OK:
        case POF_RSP_OK_CANCEL:
            break;
        default:
            POF_UPPER_TRACE_03(TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "RQB-Response error, Opcode:0x%X Handle:0x%X Response:0x%X",
                               POF_RQB_GET_OPCODE(pRQB), POF_RQB_GET_HANDLE(pRQB), POF_RQB_GET_RESPONSE(pRQB));
            break;
    }
    #endif

    POF_REQUEST_UPPER_DONE(pCbf, pRQB, pSys);
    LSA_UNUSED_ARG(TraceIdx); //satisfy lint!

    POF_FUNCTION_TRACE_00(TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_CallCbf");
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_UserRequestFinish()                     +*/
/*+  Input/Output               POF_HDB_PTR_TYPE           pHDB             +*/
/*+                             POF_UPPER_RQB_PTR_TYPE     pRQB             +*/
/*+                             POF_RSP                    Response         +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pHDB                 : Pointer to HDB                                  +*/
/*+  pRQB                 : Pointer to RQB                                  +*/
/*+  Response             : Response to set in RQB                          +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Finishs Request by calling call-back-function located in  +*/
/*+               HDB. if Cbf is LSA_NULL noting is done.                   +*/
/*+                                                                         +*/
/*+               Fill the RQB-local-Err-Structure (err) with local POF     +*/
/*+               error if lsa_component_id is LSA_COMP_ID_UNUSED.          +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID  POF_LOCAL_FCT_ATTR  POF_UserRequestFinish( POF_HDB_PTR_TYPE        const  pHDB,
                                                     POF_UPPER_RQB_PTR_TYPE  const  pRQB,
                                                     POF_RSP                 const  Response )
{
    POF_FUNCTION_TRACE_03(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_UserRequestFinish, pHDB:0x%X pRQB:0x%X Response:0x%X",
                          pHDB, pRQB, Response);

    POF_RQB_SET_RESPONSE(pRQB, Response);
    POF_RQB_SET_HANDLE(pRQB, pHDB->Params.HandleUpper);

    POF_UPPER_TRACE_04(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "<<<: Request finished, Opcode:0x%X Handle:0x%Xh UpperHandle:0x%X Response:0x%X", 
                       POF_RQB_GET_OPCODE(pRQB), pHDB->ThisHandle, POF_RQB_GET_HANDLE(pRQB), Response);

    POF_CallCbf(pHDB->TraceIdx, pHDB->Params.Cbf, pRQB, pHDB->Params.pSys);

    POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_UserRequestFinish");
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_GetUserHDBFromHandle()                  +*/
/*+  Input/Output          :    LSA_HANDLE_TYPE     Handle                  +*/
/*+  Result                :    POF_HDB_PTR_TYPE                            +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  Handle     : POF user -channel handle to get pointer to HDB for.       +*/
/*+                                                                         +*/
/*+  Result     : Pointer to HDB or LSA_NULL if Handle invalid              +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Gets Pointer to valid User-HDB for handle.                +*/
/*+               Handle has to be a User-Channel-Handle!!                  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
POF_HDB_PTR_TYPE  POF_LOCAL_FCT_ATTR  POF_GetUserHDBFromHandle( LSA_HANDLE_TYPE  const  Handle )
{
    LSA_UINT32  const  Idx = (LSA_UINT32)Handle; //Get User-Idx

    if (Idx < (LSA_UINT32)POF_CFG_MAX_CHANNELS)
    {
        POF_HDB_PTR_TYPE  const  pHDB = &(POF_MGM_GET_HUSER_PTR()->HDB[Idx]);

        //Handle must be in use (<> free)
        if (pHDB->State == POF_HANDLE_STATE_FREE)
        {
            return LSA_NULL;
        }
        else
        {
            return pHDB;
        }
    }
    else 
    {
        POF_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_UNEXP, "POF_GetUserHDBFromHandle, Invalid user handle:0x%X out of range", Handle);
        return LSA_NULL;
    }
}
/*---------------------- end [subroutine] ---------------------------------*/


/*=============================================================================
 * function name:  POF_QueueAddToEnd()
 *
 * function:       Adds an Element to the end of the Queue
 *                 
 *===========================================================================*/
LSA_VOID  POF_LOCAL_FCT_ATTR  POF_QueueAddToEnd( POF_QUEUE_PTR_TYPE          const  pQueue,
                                                 POF_QUEUE_ELEMENT_PTR_TYPE  const  pNewElement )
{
    if (pQueue->Count == 0)  
    {
        //First Element in List --> init pBegin
        pQueue->pFirst        = pNewElement;
        pNewElement->prev_ptr = LSA_NULL; 
    }
    else
    {
        pQueue->pLast->next_ptr = pNewElement;
        pNewElement->prev_ptr   = pQueue->pLast;
    }

    pNewElement->next_ptr = LSA_NULL;
    //Update pEnd and Counter
    pQueue->pLast = pNewElement;
    pQueue->Count ++;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*=============================================================================
 * function name: POF_QueueRemoveFromBegin()
 *
 * function:      Removes an Element from the begin of the Queue
 *                 
 *===========================================================================*/
POF_QUEUE_ELEMENT_PTR_TYPE  POF_LOCAL_FCT_ATTR  POF_QueueRemoveFromBegin( POF_QUEUE_PTR_TYPE  const  pQueue )
{
    POF_QUEUE_ELEMENT_PTR_TYPE  pRemElement;

    if (pQueue->Count == 0)  
    {
        return LSA_NULL; 
    }

    pRemElement = pQueue->pFirst;
  
    if (pQueue->Count == 1)
    {
        //Last Element in List --> also update pBegin
        pQueue->pFirst = LSA_NULL;
        pQueue->pLast  = LSA_NULL;
    }
    else
    {
        //Update pBegin
        pQueue->pFirst           = pQueue->pFirst->next_ptr;
        pQueue->pFirst->prev_ptr = LSA_NULL;
    }

    pQueue->Count --;

    pRemElement->next_ptr = LSA_NULL;
    pRemElement->prev_ptr = LSA_NULL;

    return pRemElement;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*****************************************************************************/
/*  end of file pof_glb.c                                                    */
/*****************************************************************************/

