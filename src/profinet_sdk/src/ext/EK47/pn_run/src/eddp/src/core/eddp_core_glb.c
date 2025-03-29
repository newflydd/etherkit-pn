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
/*  F i l e               &F: eddp_core_glb.c                           :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP general global core functions               */
/*                                                                           */
/*                          e.g. fatal error handling                        */
/*                               queuing of RQBs                             */
/*                               request finishing                           */
/*                               internal RQBs alloc/free/setup              */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  15.08.08    js    initial version.                                       */
/*  20.07.10    JS    Use EDDP_REQUEST_UPPER_DONE() for finishing requests   */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID  3                 
#define EDDP_MODULE_ID     LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_CORE_GLB */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"            /* edd headerfiles */
#include "eddp_int.h"            /* internal header */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)

#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*/
/*                                global Variables                           */
/*===========================================================================*/

/*===========================================================================*/
/*                                external functions                         */
/*===========================================================================*/

/*===========================================================================*/
/*                                 functions                                 */
/*===========================================================================*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbFatalError                          +*/
/*+  Input/Output               EDDP_FATAL_ERROR_TYPE   Error               +*/
/*+  Input/Output          :    LSA_UINT16              ModuleID            +*/
/*+                             LSA_UINT16              Line                +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  Error                : EDD-fatal-errorcode                             +*/
/*+  ModuleID             : module - id of error                            +*/
/*+  Line                 : line of code (optional)                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Signals Fatal-Error via EDD_FATAL_ERROR macro.            +*/
/*+               This function does not return!                            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GlbFatalError(
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

    EDDP_CORE_TRACE_03(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_GlbFatalError(ModuleID=%d, Line=%d, Error=0x%X)",
        ModuleID, Line, Error);

    EDDP_FATAL_ERROR(sizeof(LSA_FATAL_ERROR_TYPE), &LSAError);

    /* should not return! */
    while (1) ;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbCallCbfLow                          +*/
/*+  Input/Output               LSA_VOID LSA_FCT_PTR..       Cbf            +*/
/*+                             EDD_UPPER_RQB_PTR_TYPE       pRQB           +*/
/*+                             LSA_UINT16                   ModuleId       +*/
/*+                             LSA_UINT16                   LineNr         +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  Cbf                  : Pointer to Callbackfunction                     +*/
/*+  pRQB                 : Pointer to RQB                                  +*/
/*+  ModuleId             : ModuleId used with err       0 if none          +*/
/*+  LineNr               : LineNr   used with err       0 if none          +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Calls Cbf with RQB.                                       +*/
/*+               if Cbf is LSA_NULL Cbf is not called.                     +*/
/*+               On an RQB-Error we fill up the "err" structure with       +*/
/*+               aditional information like the ModuleID and LineNr.       +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GlbCallCbfLow(
    LSA_VOID LSA_FCT_PTR(EDD_UPPER_OUT_FCT_ATTR, Cbf)(EDD_UPPER_RQB_PTR_TYPE pRQB),
    EDD_UPPER_RQB_PTR_TYPE       pRQB,
    LSA_UINT32                   ModuleId,
    LSA_UINT32                   LineNr)
{
    LSA_UNUSED_ARG(LineNr);
    LSA_UNUSED_ARG(ModuleId);

    //CBF for system services. CBF is not mandatory here.
    if ( ! (0 == Cbf) )
    {
        Cbf(pRQB);
    }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbCallCbf                             +*/
/*+  Input/Output               LSA_VOID LSA_FCT_PTR..      Cbf             +*/
/*+                             EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  Cbf                  : Pointer to Callbackfunction                     +*/
/*+  pRQB                 : Pointer to RQB                                  +*/
/*+  pDDB                 : Optional pointer to DDB holding additional err  +*/
/*+                         information (LSA_NULL if not present)           +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Calls Cbf with RQB.                                       +*/
/*+               if Cbf is LSA_NULL Cbf is not called.                     +*/
/*+               On an RQB-Error we fill up the "DebugInfo" structure with +*/
/*+               aditional information like the ModuleID and LineNr.       +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GlbCallCbf(
    LSA_VOID LSA_FCT_PTR(EDD_UPPER_OUT_FCT_ATTR, Cbf)(EDD_UPPER_RQB_PTR_TYPE pRQB),
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT16      ModuleId = 0;
    LSA_UINT16      LineNr   = 0;

    /* ------------------------------------------------------------------------ */
    /* If a DDB is present we take the info for additional "err" info.          */
    /* if this RQB has no error this info will be discarded.                    */
    /* ------------------------------------------------------------------------ */
    #ifdef EDDP_CFG_DETAIL_ERROR_INFO
    if (!LSA_HOST_PTR_ARE_EQUAL(pDDB, LSA_NULL))
    {
        if (pDDB->ErrDetail.bSet)
        {
            ModuleId                = (LSA_UINT16) pDDB->ErrDetail.ModuleId;
            LineNr                  = (LSA_UINT16) pDDB->ErrDetail.LineNr;
            pDDB->ErrDetail.bSet    = LSA_FALSE;
        }
    }
    #else
    LSA_UNUSED_ARG(pDDB);
    #endif

    EDDP_GlbCallCbfLow (Cbf, pRQB, ModuleId, LineNr);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbCallChannelCbfLow                   +*/
/*+  Input/Output               LSA_VOID LSA_FCT_PTR..       Cbf            +*/
/*+                             EDD_UPPER_RQB_PTR_TYPE       pRQB           +*/
/*+                             LSA_SYS_PTR_TYPE             pSys           +*/
/*+                             LSA_UINT16                   ModuleId       +*/
/*+                             LSA_UINT16                   LineNr         +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  Cbf                  : Pointer to Callbackfunction                     +*/
/*+  pRQB                 : Pointer to RQB                                  +*/
/*+  pSys                   Systempointer                                   +*/
/*+  ModuleId             : ModuleId used with err       0 if none          +*/
/*+  LineNr               : LineNr   used with err       0 if none          +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Calls EDDP_REQUEST_UPPER_DONE with RQB.                   +*/
/*+               On an RQB-Error we fill up the "DebugInfo" structure with +*/
/*+               aditional information like the ModuleID and LineNr.       +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GlbCallChannelCbfLow(
    LSA_VOID LSA_FCT_PTR(EDD_UPPER_OUT_FCT_ATTR, Cbf)(EDD_UPPER_RQB_PTR_TYPE pRQB),
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    LSA_SYS_PTR_TYPE            pSys,
    LSA_UINT32                  ModuleId,
    LSA_UINT32                  LineNr)
{
    /* ------------------------------------------------------------------------ */
    /* fill the error DebugInfo with response only if error is in pRQB          */
    /* ------------------------------------------------------------------------ */
    #ifdef EDDP_CFG_DETAIL_ERROR_INFO
    if (   (EDD_RQB_GET_RESPONSE(pRQB) != EDD_STS_OK)
        && (EDD_RQB_GET_RESPONSE(pRQB) != EDD_STS_OK_CANCEL)
       )
    {
        EDDP_CRTFillDebugInfoError (pRQB, ModuleId, LineNr);
    }
    #else
    LSA_UNUSED_ARG(LineNr);
    LSA_UNUSED_ARG(ModuleId);
    #endif

    EDDP_REQUEST_UPPER_DONE(Cbf,pRQB,pSys);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbCallChannelCbf                      +*/
/*+  Input/Output               LSA_VOID LSA_FCT_PTR..      Cbf             +*/
/*+                             EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_SYS_PTR_TYPE            pSys            +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  Cbf                  : Pointer to Callbackfunction                     +*/
/*+  pRQB                 : Pointer to RQB                                  +*/
/*+  pDDB                 : Optional pointer to DDB holding additional err  +*/
/*+                         information (LSA_NULL if not present)           +*/
/*+  pSys                   Systempointer                                   +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Calls Cbf with RQB for a User channel request.            +*/
/*+               if Cbf is LSA_NULL Cbf is not called.                     +*/
/*+               On an RQB-Error we fill up the "err" structure with       +*/
/*+               aditional information like the ModuleID and LineNr.       +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GlbCallChannelCbf(
    LSA_VOID LSA_FCT_PTR(EDD_UPPER_OUT_FCT_ATTR, Cbf)(EDD_UPPER_RQB_PTR_TYPE pRQB),
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_SYS_PTR_TYPE            pSys)
{
    LSA_UINT16      ModuleId = 0;
    LSA_UINT16      LineNr   = 0;

    /* ------------------------------------------------------------------------ */
    /* If a DDB is present we take the info for additional "err" info.          */
    /* if this RQB has no error this info will be discarded.                    */
    /* ------------------------------------------------------------------------ */
    #ifdef EDDP_CFG_DETAIL_ERROR_INFO
    if ( !(LSA_HOST_PTR_ARE_EQUAL(pDDB, LSA_NULL)) )
    {
        if (pDDB->ErrDetail.bSet)
        {
            ModuleId                = (LSA_UINT16) pDDB->ErrDetail.ModuleId;
            LineNr                  = (LSA_UINT16) pDDB->ErrDetail.LineNr;
            pDDB->ErrDetail.bSet    = LSA_FALSE;
        }
    }
    #else
    LSA_UNUSED_ARG(pDDB);
    #endif

    EDDP_GlbCallChannelCbfLow (Cbf, pRQB, pSys, ModuleId, LineNr);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbRequestFinish                       +*/
/*+  Input/Output               EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+                             EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDD_RSP                     Response        +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pHDB                 : Pointer to HDB                                  +*/
/*+  pRQB                 : Pointer to RQB                                  +*/
/*+  Status               : Status to set in RQB                            +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Finishs Request by calling call-back-function located in  +*/
/*+               HDB. if Cbf is LSA_NULL noting is done.                   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GlbRequestFinish(
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB,
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDD_RSP                     Response)
{
    EDDP_ASSERT_NO_NULL_PTR(pHDB);

    EDDP_CORE_TRACE_03(pHDB->pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_GlbRequestFinish(pRQB=0x%X, Response=0x%X)",
        pHDB->Handle, pRQB, Response);

    EDD_RQB_SET_RESPONSE(pRQB, Response);
    EDD_RQB_SET_HANDLE(pRQB, pHDB->UpperHandle );

    EDDP_UPPER_TRACE_05(pHDB->pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] <<< EDDP_GlbRequestFinish(): pRQB=0x%X, Opcode=0x%X, Service=0x%X, Response=0x%X",
                       pHDB->Handle,
                       pRQB,
                       EDD_RQB_GET_OPCODE(pRQB),
                       EDD_RQB_GET_SERVICE(pRQB),
                       Response);

    EDDP_GlbCallChannelCbf (pHDB->Cbf, pRQB, pHDB->pDDB, pHDB->pSys);

    EDDP_CORE_TRACE_00(pHDB->pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_GlbRequestFinish()");
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbHandleGetHDB                        +*/
/*+  Input/Output          :    LSA_HANDLE_TYPE                    Handle   +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE            * ppHDB  +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  Handle       Channel Handle                                            +*/
/*+  ppHDB        Pointer to address for handle-management which will be    +*/
/*+               returned.                                                 +*/
/*+                                                                         +*/
/*+  Result:      EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Gets pointer to handle-managment entry (HDB) for handle.  +*/
/*+               The "Handle" has to be "InUse" !                          +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GlbHandleGetHDB( 
    EDD_HANDLE_LOWER_TYPE                            HandleLower,
    EDDP_LOCAL_HDB_PTR_TYPE EDDP_LOCAL_MEM_ATTR     *ppHDB)
{    
    EDDP_LOCAL_HDB_PTR_TYPE const pHDB = (EDDP_LOCAL_HDB_PTR_TYPE)HandleLower;
    EDD_RSP Response = EDD_STS_ERR_PARAM; 
    
    /* NOTE: return NULL when HandleLower is invalid */
    *ppHDB = LSA_NULL;
    
    if ( pHDB && (pHDB->InUse) )
    {
        if( EDDP_CFG_MAX_CHANNELS > pHDB->Handle )
        {
            if ( LSA_HOST_PTR_ARE_EQUAL(pHDB, &pEDDPInfo->HDB[pHDB->Handle]) )
            {
                *ppHDB = pHDB; 
                Response = EDD_STS_OK;
            }
        }
    }
        
    return Response;
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbAllocLocalMem                       +*/
/*+  Input/Output               LSA_UINT32  length                          +*/
/*+  Result                :    EDDP_LOCAL_MEM_PTR_TYPE                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  length        : Length in bytes of memory to be allocated              +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Allocates local Memory with EDDP_ALLOC_LOCAL_MEM          +*/
/*+               Returns LSA_NULL if alloc fails                           +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDDP_LOCAL_MEM_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_GlbAllocLocalMem(
    LSA_UINT32      Length)
{
    EDDP_LOCAL_MEM_PTR_TYPE     pMem;

    EDDP_SYSTEM_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] --> EDDP_ALLOC_LOCAL_MEM(&pMem=0x%X, Length=%d)",
        &pMem, Length);
    EDDP_ALLOC_LOCAL_MEM(&pMem,Length);
    EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] <-- EDDP_ALLOC_LOCAL_MEM(): pMem=0x%X", pMem);

    return (pMem);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbFreeLocalMem                        +*/
/*+  Input/Output               EDDP_LOCAL_MEM_PTR_TYPE  pMem                +*/
/*+  Result                :    LSA_UINT16                                  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pMem       : Pointer to Local memory to be freed                       +*/
/*+                                                                         +*/
/*+  Result     : LSA_RET_OK                                                +*/
/*+               LSA_RET_ERR_PARAM                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Frees local Memory allocated with EDDP_AllocLocalMem      +*/
/*+               Returns LSA_RET_OK or LSA_RET_ERR_PARAM                   +*/
/*+               If EDDP_CFG_FREE_ERROR_IS_FATAL is defined the function   +*/
/*+               will not return on error.                                 +*/
/*+               If not defined only an error-trace will be done on error  +*/
/*+               Typically the caller will ignore the error and continue   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_GlbFreeLocalMem(
    EDDP_LOCAL_MEM_PTR_TYPE     pMem)
{
    LSA_UINT16      FreeStat;

    EDDP_SYSTEM_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] --> EDDP_FREE_LOCAL_MEM(&FreeStat=0x%X, pMem=0x%X)",
        &FreeStat, pMem);
    EDDP_FREE_LOCAL_MEM(&FreeStat, pMem);
    if (FreeStat != LSA_RET_OK)                                                                                                                             
    {                            
        EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_FREE_LOCAL_MEM(RetVal=0x%X): Freeing memory failed!", FreeStat);  
        EDDP_FREE_CHECK(FreeStat);
    }
    return (FreeStat);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbAllocUpperMemLocal                  +*/
/*+  Input/Output               LSA_UINT16  Length                          +*/
/*+  Result                :    EDD_UPPER_MEM_PTR_TYPE                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  length        : Length in bytes of memory to be allocated              +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Allocates Upper Memory with EDDP_ALLOC_UPPER_MEM_LOCAL    +*/
/*+               Returns LSA_NULL if alloc fails                           +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_UPPER_MEM_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_GlbAllocUpperMemLocal(
    LSA_UINT16      Length)
{
    EDD_UPPER_MEM_PTR_TYPE      pMem;

    EDDP_SYSTEM_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] --> EDDP_ALLOC_UPPER_MEM_LOCAL(&pMem=0x%X, Length=%d)",
        &pMem, Length);
    EDDP_ALLOC_UPPER_MEM_LOCAL(&pMem,Length);
    EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] <-- EDDP_ALLOC_UPPER_MEM_LOCAL(): pMem=0x%X", pMem);

    return (pMem);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbFreeUpperMemLocal                   +*/
/*+  Input/Output               EDD_UPPER_MEM_PTR_TYPE  pMem                +*/
/*+  Result                :    LSA_UINT16                                  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pMem       : Pointer to upper memory to be freed                       +*/
/*+                                                                         +*/
/*+  Result     : LSA_RET_OK                                                +*/
/*+               LSA_RET_ERR_PARAM                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Frees local Memory allocated with EDDP_AllocUpperMemLocal +*/
/*+               Returns LSA_RET_OK or LSA_RET_ERR_PARAM                   +*/
/*+               If EDDP_CFG_FREE_ERROR_IS_FATAL is defined the function   +*/
/*+               will not return on error.                                 +*/
/*+               If not defined only an error-trace will be done on error  +*/
/*+               Typically the caller will ignore the error and continue   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_GlbFreeUpperMemLocal(
    EDD_UPPER_MEM_PTR_TYPE      pMem)
{
    LSA_UINT16      FreeStat;

    EDDP_SYSTEM_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] --> EDDP_FREE_UPPER_MEM_LOCAL(&FreeStat=0x%X, pMem=0x%X)",
        &FreeStat, pMem);
    EDDP_FREE_UPPER_MEM_LOCAL(&FreeStat, pMem);
    if(FreeStat != LSA_RET_OK)
    {
        EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] <-- EDDP_FREE_UPPER_MEM_LOCAL() failed: FreeStat=0x%X", FreeStat);
        EDDP_FREE_CHECK(FreeStat);
    }

    return (FreeStat);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbAllocUpperRQBLocal                  +*/
/*+  Input/Output               LSA_UINT16  Length                          +*/
/*+  Result                :    EDD_UPPER_MEM_PTR_TYPE                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  length        : Length in bytes of memory to be allocated              +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Allocates Upper RQB Memory with EDDP_ALLOC_UPPER_RQB_LOCAL+*/
/*+               Returns LSA_NULL if alloc fails                           +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_UPPER_MEM_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_GlbAllocUpperRQBLocal(
    LSA_UINT16 Length)
{
    EDD_UPPER_RQB_PTR_TYPE      pMem;

    EDDP_SYSTEM_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] --> EDDP_ALLOC_UPPER_RQB_LOCAL(&pMem=0x%X, Length=%d)",
        &pMem, Length);
    EDDP_ALLOC_UPPER_RQB_LOCAL(&pMem,Length);
    /*lint -save -e62 */
    EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] <-- EDDP_ALLOC_UPPER_RQB_LOCAL(): pMem=0x%X", pMem);
    /*lint -restore */

    return (pMem);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbFreeUpperRQBLocal                   +*/
/*+  Input/Output               EDD_UPPER_MEM_PTR_TYPE  pMem                +*/
/*+  Result                :    LSA_UINT16                                  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pMem       : Pointer to upper RQB memory to be freed                   +*/
/*+                                                                         +*/
/*+  Result     : LSA_RET_OK                                                +*/
/*+               LSA_RET_ERR_PARAM                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Frees local Memory allocated with EDDP_AllocUpperRQBLocal +*/
/*+               Returns LSA_RET_OK or LSA_RET_ERR_PARAM                   +*/
/*+               If EDDP_CFG_FREE_ERROR_IS_FATAL is defined the function   +*/
/*+               will not return on error.                                 +*/
/*+               If not defined only an error-trace will be done on error  +*/
/*+               Typically the caller will ignore the error and continue   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_GlbFreeUpperRQBLocal(
    EDD_UPPER_RQB_PTR_TYPE      pMem)
{
    LSA_UINT16      FreeStat;

    EDDP_SYSTEM_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] --> EDDP_FREE_UPPER_RQB_LOCAL(&FreeStat=0x%X, pMem=0x%X)",
        &FreeStat, pMem);
    EDDP_FREE_UPPER_RQB_LOCAL(&FreeStat, pMem);
    if(FreeStat != LSA_RET_OK)
    {
        EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] <-- EDDP_FREE_UPPER_RQB_LOCAL() failed: FreeStat=0x%X", FreeStat);
        EDDP_FREE_CHECK(FreeStat);
    }

    return (FreeStat);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbAllocInternalRQB                    +*/
/*+  Input/Output               ----                                        +*/
/*+  Result                :    EDD_UPPER_RQB_PTR_TYPE                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Allocates an upper RQB for internal use (with parameter)  +*/
/*+               pParam gets pointer to EDDP_RQB_DEV_INTERNAL_TYPE         +*/
/*+               Returns LSA_NULL if alloc fails                           +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_UPPER_RQB_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_GlbAllocInternalRQB(LSA_VOID)
{
    EDD_UPPER_RQB_PTR_TYPE      pRQB;

    EDDP_CORE_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_GlbAllocInternalRQB()");

    pRQB = (EDD_UPPER_RQB_PTR_TYPE)EDDP_GlbAllocUpperRQBLocal(sizeof(EDD_RQB_TYPE));

    if (! LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL))
    {
        pRQB->pParam = EDDP_GlbAllocUpperMemLocal(sizeof(EDDP_RQB_DEV_INTERNAL_TYPE));

        if (LSA_HOST_PTR_ARE_EQUAL(pRQB->pParam, LSA_NULL))
        {
            EDDP_GlbFreeUpperRQBLocal(pRQB);
        } 
    }

    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_GlbAllocInternalRQB(pRQB=0x%X)", pRQB);

    return (pRQB);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbFreeInternalRQB                     +*/
/*+  Input/Output               EDD_UPPER_RQB_PTR_TYPE  pRQB                +*/
/*+  Result                :    EDD_UPPER_RQB_PTR_TYPE                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Frees an upper RQB allocated with EDDAllocInternalRQB     +*/
/*+               Only frees if pointers <> LSA_NULL.                       +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GlbFreeInternalRQB(
    EDD_UPPER_RQB_PTR_TYPE pRQB)
{
    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_GlbFreeInternalRQB(pRQB=0x%X)", pRQB);

    if ( ! LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL) )
    {
        if ( ! LSA_HOST_PTR_ARE_EQUAL(pRQB->pParam, LSA_NULL)) 
        {
            EDDP_GlbFreeUpperMemLocal(pRQB->pParam);
        }
    
        EDDP_GlbFreeUpperRQBLocal(pRQB);
    }   

    EDDP_CORE_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_GlbFreeInternalRQB()");
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbSetupInternalRQB                    +*/
/*+  Input/Output          :    see below                                   +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Initializes internal RQB-Structure                        +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID  EDDP_LOCAL_FCT_ATTR EDDP_GlbSetupInternalRQB(
    EDD_UPPER_RQB_PTR_TYPE          pRQB,
    EDD_SERVICE                     Service,
    LSA_HANDLE_TYPE                 Handle,
    EDDP_HANDLE                     hDDB,
    LSA_UINT32                      ID,
    LSA_UINT32                      Param) 
{
    EDD_RQB_SET_OPCODE(pRQB,EDD_OPC_SYSTEM);
    EDD_RQB_SET_SERVICE(pRQB,Service);
    EDD_RQB_SET_HANDLE(pRQB,Handle);

    ((EDDP_UPPER_DEV_INTERNAL_PTR_TYPE) pRQB->pParam)->hDDB     = (EDDP_HANDLE) hDDB;
    ((EDDP_UPPER_DEV_INTERNAL_PTR_TYPE) pRQB->pParam)->UserID   = ID;
    ((EDDP_UPPER_DEV_INTERNAL_PTR_TYPE) pRQB->pParam)->Param    = Param;
}




/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbAllocUpperRxMem                     +*/
/*+  Input/Output               EDDP_LOCAL_DDB_PTR_TYPE   pDDB              +*/
/*+                             LSA_UINT32                Length            +*/
/*+  Result                :    EDD_UPPER_MEM_PTR_TYPE                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DDB                                         +*/
/*+  Length        : Length in bytes of memory to be allocated              +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Allocates upper rx memory with EDDP_ALLOC_UPPER_RX_MEM    +*/
/*+               Returns LSA_NULL if alloc fails                           +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_UPPER_MEM_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_GlbAllocUpperRxMem(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  Length)
{
    EDD_UPPER_MEM_PTR_TYPE      pMem;

    EDDP_SYSTEM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] --> EDDP_ALLOC_UPPER_RX_MEM(&pMem=0x%X, Length=%d)", &pMem, Length);
    EDDP_ALLOC_UPPER_RX_MEM(pDDB->hSysDev,&pMem,Length);
    EDDP_SYSTEM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] <-- EDDP_ALLOC_UPPER_RX_MEM(): pMem=0x%X", pMem);

    return (pMem);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbFreeUpperRxMem                      +*/
/*+  Input/Output               EDDP_LOCAL_DDB_PTR_TYPE   pDDB              +*/
/*+                             EDD_UPPER_MEM_PTR_TYPE    pMem              +*/
/*+  Result                :    LSA_UINT16                                  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DDB                                            +*/
/*+  pMem       : Pointer to upper memory to be freed                       +*/
/*+                                                                         +*/
/*+  Result     : LSA_RET_OK                                                +*/
/*+               LSA_RET_ERR_PARAM                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Frees upper rx memory                                     +*/
/*+               Returns LSA_RET_OK or LSA_RET_ERR_PARAM                   +*/
/*+               If EDDP_CFG_FREE_ERROR_IS_FATAL is defined the function   +*/
/*+               will not return on error.                                 +*/
/*+               If not defined only an error-trace will be done on error  +*/
/*+               Typically the caller will ignore the error and continue   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_GlbFreeUpperRxMem(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_UPPER_MEM_PTR_TYPE      pMem)
{
    LSA_UINT16      FreeStat;

    EDDP_SYSTEM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] --> EDDP_FREE_UPPER_RX_MEM(&FreeStat=0x%X, pMem=0x%X)", &FreeStat,pMem);
    EDDP_FREE_UPPER_RX_MEM(pDDB->hSysDev,&FreeStat, pMem);
    if(FreeStat != LSA_RET_OK)
    {
        EDDP_SYSTEM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] <-- EDDP_FREE_UPPER_RX_MEM() failed: FreeStat=0x%X", FreeStat);
        EDDP_FREE_CHECK(FreeStat);
    }

    return (FreeStat);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbAllocUpperTxMem                     +*/
/*+  Input/Output               EDDP_LOCAL_DDB_PTR_TYPE   pDDB              +*/
/*+                             LSA_UINT32                Length            +*/
/*+  Result                :    EDD_UPPER_MEM_PTR_TYPE                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DDB                                         +*/
/*+  Length        : Length in bytes of memory to be allocated              +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Allocates upper rx memory with EDDP_ALLOC_UPPER_TX_MEM    +*/
/*+               Returns LSA_NULL if alloc fails                           +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_UPPER_MEM_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_GlbAllocUpperTxMem(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  Length)
{
    EDD_UPPER_MEM_PTR_TYPE      pMem;

    EDDP_SYSTEM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] --> EDDP_ALLOC_UPPER_TX_MEM(&pMem=0x%X, Length=%d)", &pMem, Length);
    EDDP_ALLOC_UPPER_TX_MEM(pDDB->hSysDev,&pMem,Length);
    EDDP_SYSTEM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] <-- EDDP_ALLOC_UPPER_TX_MEM(): pMem=0x%X", pMem);

    return (pMem);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbFreeUpperTxMem                      +*/
/*+  Input/Output               EDDP_LOCAL_DDB_PTR_TYPE   pDDB              +*/
/*+                             EDD_UPPER_MEM_PTR_TYPE    pMem              +*/
/*+  Result                :    LSA_UINT16                                  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DDB                                            +*/
/*+  pMem       : Pointer to upper memory to be freed                       +*/
/*+                                                                         +*/
/*+  Result     : LSA_RET_OK                                                +*/
/*+               LSA_RET_ERR_PARAM                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Frees upper tx memory                                     +*/
/*+               Returns LSA_RET_OK or LSA_RET_ERR_PARAM                   +*/
/*+               If EDDP_CFG_FREE_ERROR_IS_FATAL is defined the function   +*/
/*+               will not return on error.                                 +*/
/*+               If not defined only an error-trace will be done on error  +*/
/*+               Typically the caller will ignore the error and continue   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_GlbFreeUpperTxMem(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_UPPER_MEM_PTR_TYPE      pMem)
{
    LSA_UINT16      FreeStat;

    EDDP_SYSTEM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] --> EDDP_FREE_UPPER_TX_MEM(&FreeStat=0x%X, pMem=0x%X)", &FreeStat, pMem);
    EDDP_FREE_UPPER_TX_MEM(pDDB->hSysDev,&FreeStat, pMem);
    if(FreeStat != LSA_RET_OK)
    {
        EDDP_SYSTEM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] <-- EDDP_FREE_UPPER_TX_MEM() failed: FreeStat=0x%X", FreeStat);
        EDDP_FREE_CHECK(FreeStat);
    }

    return (FreeStat);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbAllocUpperDevMem                    +*/
/*+  Input/Output               EDDP_LOCAL_DDB_PTR_TYPE   pDDB              +*/
/*+                             LSA_UINT32                Length            +*/
/*+  Result                :    EDD_UPPER_MEM_PTR_TYPE                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DDB                                         +*/
/*+  Length        : Length in bytes of memory to be allocated              +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Allocates upper device mem with EDDP_ALLOC_UPPER_DEV_MEM  +*/
/*+               Returns LSA_NULL if alloc fails                           +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_UPPER_MEM_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_GlbAllocUpperDevMem(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  Length)
{
    EDD_UPPER_MEM_PTR_TYPE      pMem;

    EDDP_SYSTEM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] --> EDDP_ALLOC_UPPER_DEV_MEM(&pMem=0x%X, Length=%d)", &pMem, Length);
    EDDP_ALLOC_UPPER_DEV_MEM(pDDB->hSysDev, &pMem, Length);
    EDDP_SYSTEM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] <-- EDDP_ALLOC_UPPER_DEV_MEM(): pMem=0x%X", pMem);

    return (pMem);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbFreeUpperDevMem                     +*/
/*+  Input/Output               EDDP_LOCAL_DDB_PTR_TYPE   pDDB              +*/
/*+                             EDD_UPPER_MEM_PTR_TYPE    pMem              +*/
/*+  Result                :    LSA_UINT16                                  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DDB                                            +*/
/*+  pMem       : Pointer to upper memory to be freed                       +*/
/*+                                                                         +*/
/*+  Result     : LSA_RET_OK                                                +*/
/*+               LSA_RET_ERR_PARAM                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Frees upper device memory                                 +*/
/*+               Returns LSA_RET_OK or LSA_RET_ERR_PARAM                   +*/
/*+               If EDDP_CFG_FREE_ERROR_IS_FATAL is defined the function   +*/
/*+               will not return on error.                                 +*/
/*+               If not defined only an error-trace will be done on error  +*/
/*+               Typically the caller will ignore the error and continue   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR  EDDP_GlbFreeUpperDevMem(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_UPPER_MEM_PTR_TYPE      pMem)
{
    LSA_UINT16      FreeStat;

    EDDP_SYSTEM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] --> EDDP_FREE_UPPER_DEV_MEM(&FreeStat=0x%X, pMem=0x%X)", &FreeStat, pMem);
    EDDP_FREE_UPPER_DEV_MEM(pDDB->hSysDev,&FreeStat, pMem);
    if(FreeStat != LSA_RET_OK)
    {
        EDDP_SYSTEM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] <-- EDDP_FREE_UPPER_DEV_MEM() failed: FreeStat=0x%X", FreeStat);
        EDDP_FREE_CHECK(FreeStat);
    }

    return (FreeStat);
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbQueueInit                           +*/
/*+  Input                 :    EDD_QUEUE_PTR_TYPE            pQueue        +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pQueue        : Pointer to internal queue structure                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:  Inits internal queue structure.                          +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID  EDDP_LOCAL_FCT_ATTR EDDP_GlbQueueInit( 
    EDDP_QUEUE_PTR_TYPE     pQueue)
{
    pQueue->Count  = 0;
    pQueue->pFirst = LSA_NULL;
    pQueue->pLast  = LSA_NULL;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbQueueAddToEnd                       +*/
/*+  Input                 :    EDDP_QUEUE_PTR_TYPE            pQueue       +*/
/*+                             EDDP_QUEUE_ELEMENT_PTR_TYPE    pNewElement  +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pQueue        : Pointer to queue                                       +*/
/*+  pNewElement   : Pointer to queueelement to add                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:  Adds an Element to the end of the Queue                  +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GlbQueueAddToEnd(
    EDDP_QUEUE_PTR_TYPE             pQueue,
    EDDP_QUEUE_ELEMENT_PTR_TYPE     pNewElement)
{
    if (pQueue->Count == 0)  
    {
        // First Element in List --> init pBegin
        pQueue->pFirst          = pNewElement;
        pNewElement->prev_ptr   = LSA_NULL; 
    }
    else
    {
        pQueue->pLast->next_ptr = pNewElement;
        pNewElement->prev_ptr   = pQueue->pLast;
    }
    
    pNewElement->next_ptr   = LSA_NULL;
    // Update pEnd and Counter
    pQueue->pLast           = pNewElement;
    pQueue->Count ++;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbQueueRemoveFromBegin                +*/
/*+  Input                 :    EDDP_QUEUE_PTR_TYPE            pQueue       +*/
/*+  Result                :    EDDP_QUEUE_ELEMENT_PTR_TYPE                 +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pQueue        : Pointer to queue                                       +*/
/*+  Result        : Pointer to queueelement removed from queue             +*/
/*+                  LSA_NULL if queue was empty                            +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:  Removes an Element from the Begin of the Queue           +*/
/*+                Note: removed element linking will not be set to 0!      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDDP_QUEUE_ELEMENT_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_GlbQueueRemoveFromBegin(
    EDDP_QUEUE_PTR_TYPE     pQueue)
{
    EDDP_QUEUE_ELEMENT_PTR_TYPE     pRemElement;

    if (pQueue->Count == 0)  
    {
        pRemElement =  LSA_NULL; 
    }
    else
    {
        pRemElement = pQueue->pFirst;
  
        if (pQueue->Count == 1)  
        {
            // Last Element in List --> also update pLast
            pQueue->pFirst = LSA_NULL;
            pQueue->pLast  = LSA_NULL;
        }
        else
        {
            // Update pBegin
            pQueue->pFirst           = pQueue->pFirst->next_ptr;
            pQueue->pFirst->prev_ptr = LSA_NULL;
        }

        pQueue->Count --;
  
        pRemElement->next_ptr = LSA_NULL;
        pRemElement->prev_ptr = LSA_NULL;
    }       

    return (pRemElement);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbRQBQueueRemoveFromQueue             +*/
/*+  Input                 :    EDDP_RQB_QUEUE_PTR_TYPE            pQueue   +*/
/*+                             EDD_UPPER_RQB_PTR_TYPE        pExtractRQB   +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pQueue        : Pointer to RQB-queue                                   +*/
/*+  pNewElement   : Pointer to queueelement to extract (RQB)               +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:  Removes an Element (RQB) from the RQB Queue              +*/
/*+                Note: removed element linking will not be set to 0       +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_GlbRQBQueueRemoveFromQueue(
    EDDP_RQB_QUEUE_PTR_TYPE     pQueue,
    EDD_UPPER_RQB_PTR_TYPE      pExtractRQB)
{
    EDD_UPPER_RQB_PTR_TYPE      pTmpElement;

    if (pQueue->Count)  
    {
        if (pExtractRQB == pQueue->pFirst)
        {   // First Element in List --> also update pFirst
            pQueue->pFirst = EDD_RQB_GET_NEXT_RQB_PTR(pExtractRQB);

            if (pQueue->pFirst != LSA_NULL)
            {
                EDD_RQB_SET_PREV_RQB_PTR(pQueue->pFirst, LSA_NULL);
            }
            else
            {
                pQueue->pLast = LSA_NULL;
            }
        }
        else
        {
            if (pExtractRQB == pQueue->pLast)
            {   // Last Element in List --> also update pLast
                pTmpElement = EDD_RQB_GET_PREV_RQB_PTR(pExtractRQB);

                if (pTmpElement != LSA_NULL)
                {
                    EDD_RQB_SET_NEXT_RQB_PTR(pTmpElement, LSA_NULL);
                }
                pQueue->pLast = pTmpElement;
            }
            else
            {   // Somewhere in middle of List
                pTmpElement = EDD_RQB_GET_NEXT_RQB_PTR(pExtractRQB);
                if ( pTmpElement == LSA_NULL )
                {   // expect at minimum 3 items in list
                    return LSA_FALSE;
                }
                EDD_RQB_SET_PREV_RQB_PTR(pTmpElement, EDD_RQB_GET_PREV_RQB_PTR(pExtractRQB));
                EDD_RQB_SET_NEXT_RQB_PTR(EDD_RQB_GET_PREV_RQB_PTR(pExtractRQB), pTmpElement);
            }
        }

        pQueue->Count--;
        EDD_RQB_SET_PREV_RQB_PTR(pExtractRQB, LSA_NULL);
        EDD_RQB_SET_NEXT_RQB_PTR(pExtractRQB, LSA_NULL);
        return (LSA_TRUE);
    }

    return (LSA_FALSE);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbRQBQueueInit                        +*/
/*+  Input                 :    EDDP_RQB_QUEUE_PTR_TYPE        pQueue       +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pQueue        : Pointer to RQB queue structure                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:  Inits RQB-Queue structure                                +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GlbRQBQueueInit( 
    EDDP_RQB_QUEUE_PTR_TYPE     pQueue)
{
    pQueue->Count  = 0;
    pQueue->pFirst = LSA_NULL;
    pQueue->pLast  = LSA_NULL;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbRQBQueueAddToEnd                    +*/
/*+  Input                 :    EDDP_RQB_QUEUE_PTR_TYPE        pQueue       +*/
/*+                             EDD_UPPER_RQB_PTR_TYPE        pNewElement   +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pQueue        : Pointer to RQB-queue                                   +*/
/*+  pNewElement   : Pointer to queueelement to add (RQB)                   +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:  Adds an Element (RQB) to end of the RQB Queue            +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GlbRQBQueueAddToEnd( 
    EDDP_RQB_QUEUE_PTR_TYPE     pQueue,
    EDD_UPPER_RQB_PTR_TYPE      pNewElement )
{
    if (pQueue->Count == 0)  
    {
        // First Element in List --> init pBegin
        pQueue->pFirst = pNewElement;
        EDD_RQB_SET_PREV_RQB_PTR(pNewElement,LSA_NULL);
    }
    else
    {
        EDD_RQB_SET_NEXT_RQB_PTR(pQueue->pLast,pNewElement);
        EDD_RQB_SET_PREV_RQB_PTR(pNewElement,pQueue->pLast);
    }

    EDD_RQB_SET_NEXT_RQB_PTR(pNewElement,LSA_NULL);
    // Update pEnd and Counter
    pQueue->pLast = pNewElement;
    pQueue->Count ++;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbRQBQueueRemoveFromBegin             +*/
/*+  Input                 :    EDDP_RQB_QUEUE_PTR_TYPE            pQueue   +*/
/*+  Result                :    EDD_UPPER_RQB_PTR_TYPE                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pQueue        : Pointer to RQB-queue                                   +*/
/*+  Result        : Pointer to queueelement (RQB) removed from queue       +*/
/*+                  LSA_NULL if queue was empty                            +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:  Removes an Element (RQB) from the Begin of the RQB Queue +*/
/*+                Note: removed element linking will not be set to 0!      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_UPPER_RQB_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_GlbRQBQueueRemoveFromBegin(
    EDDP_RQB_QUEUE_PTR_TYPE     pQueue)
{
    EDD_UPPER_RQB_PTR_TYPE      pRemElement;

    if (pQueue->Count == 0)  
    {
        pRemElement = LSA_NULL; 
    }
    else
    {
        pRemElement = pQueue->pFirst;
  
        if (pQueue->Count == 1)  
        {
            // Last Element in List --> also update pLast
            pQueue->pFirst = LSA_NULL;
            pQueue->pLast  = LSA_NULL;
        }
        else
        {
            // Update pBegin
            pQueue->pFirst = EDD_RQB_GET_NEXT_RQB_PTR(pQueue->pFirst);
            EDD_RQB_SET_PREV_RQB_PTR(pQueue->pFirst,LSA_NULL);
        }

        pQueue->Count --;
    }       

    return (pRemElement);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbRQBQueueRemoveFromEnd               +*/
/*+  Input                 :    EDDP_RQB_QUEUE_PTR_TYPE            pQueue   +*/
/*+  Result                :    EDD_UPPER_RQB_PTR_TYPE                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pQueue        : Pointer to RQB-queue                                   +*/
/*+  Result        : Pointer to queueelement (RQB) removed from queue       +*/
/*+                  LSA_NULL if queue was empty                            +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:  Removes an Element (RQB) from the End of the RQB Queue   +*/
/*+                Note: removed element linking will not be set to 0!      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_UPPER_RQB_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_GlbRQBQueueRemoveFromEnd(
    EDDP_RQB_QUEUE_PTR_TYPE     pQueue)
{
    EDD_UPPER_RQB_PTR_TYPE      pRemElement;

    if (pQueue->Count == 0)  
    {
        pRemElement = LSA_NULL; 
    }
    else
    {
        pRemElement = pQueue->pLast;
  
        if (pQueue->Count == 1)  
        {
            // Last Element in List --> also update pLast
            pQueue->pFirst = LSA_NULL;
            pQueue->pLast  = LSA_NULL;
        }
        else
        {
            // Update pEnd
            pQueue->pLast = EDD_RQB_GET_PREV_RQB_PTR(pQueue->pLast);
            EDD_RQB_SET_NEXT_RQB_PTR(pQueue->pLast,LSA_NULL);
        }

        pQueue->Count --;
    }       

    return (pRemElement);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbRQBQueueIsEmpty                     +*/
/*+  Input                 :    EDDP_RQB_QUEUE_PTR_TYPE            pQueue   +*/
/*+  Result                :    LSA_BOOL                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pQueue        : Pointer to RQB-queue                                   +*/
/*+  Result        : LSA_TRUE  : Queue contains requests.                   +*/
/*+                  LSA_FALSE : Queue is empty.                            +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:  Checks whether queue contains in progress request.       +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_GlbRQBQueueIsEmpty(
        EDDP_RQB_QUEUE_PTR_TYPE pQueue)
{
    if(0 != pQueue->Count)
    {
        return LSA_FALSE;
    }
    else
    {
        return LSA_TRUE;
    }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbRQBPeekFirstElement                 +*/
/*+  Input                 :    EDDP_RQB_QUEUE_PTR_TYPE            pQueue   +*/
/*+  Result                :    EDD_UPPER_RQB_PTR_TYPE                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pQueue        : Pointer to RQB-queue                                   +*/
/*+  Result        : Pointer to first element.                              +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:  Get first element without removing it.                   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_UPPER_RQB_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_GlbRQBPeekFirstElement(
        EDDP_RQB_QUEUE_PTR_TYPE pQueue)
{
    EDD_UPPER_RQB_PTR_TYPE pElement = LSA_NULL;
    
    if(0 != pQueue->Count)
    {
        pElement = pQueue->pFirst;
    }
    return pElement;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_GlbIsAligned                                       +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                        +*/
/*+                             LSA_UINT32                  Value                       +*/
/*+                             LSA_UINT16                  Alignment                   +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_BOOL                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                                     +*/
/*+  Value      : Value                                                                 +*/
/*+  Alignment  : which alignment must be checked ?                                     +*/
/*+                 - EDDP_GLB_ALIGNMENT_2_BYTE : for 2 byte alignment                  +*/
/*+                 - EDDP_GLB_ALIGNMENT_4_BYTE : for 4 byte alignment                  +*/
/*+                 - EDDP_GLB_ALIGNMENT_8_BYTE : for 8 byte alignment                  +*/
/*+                 - EDDP_GLB_ALIGNMENT_64_BYTE: for 64 byte alignment                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks the alignment for given Value.                   +*/
/*+                                                                                     +*/
/*+               Return LSA_TRUE  : Value is aligned                                   +*/
/*+                      LSA_FALSE : Value is not aligned                               +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_GlbIsAligned(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  Value,
    LSA_UINT16                  Alignment)
{
    LSA_BOOL        State = LSA_TRUE;

    LSA_UNUSED_ARG(pDDB);

    switch (Alignment)
    {
        /* ---------------------------- */
        /* 2 byte alignment             +*/
        /* ---------------------------- */
        case EDDP_GLB_ALIGNMENT_2_BYTE:
        {
            if ( (EDDP_GET_BIT_VALUE( Value, 
                                      EDDP_GLB_ALIGNMENT_2_MASK, 
                                      EDDP_GLB_ALIGNMENT_SHIFT))
               )
            {
                State = LSA_FALSE;  // not aligned!
            }
        }
        break;

        /* ---------------------------- */
        /* 4 byte alignment             */
        /* ---------------------------- */
        case EDDP_GLB_ALIGNMENT_4_BYTE:
        {
            if ( (EDDP_GET_BIT_VALUE( Value, 
                                      EDDP_GLB_ALIGNMENT_4_MASK, 
                                      EDDP_GLB_ALIGNMENT_SHIFT))
               )
            {
                State = LSA_FALSE;  // not aligned!
            }
        }
        break;

        /* ---------------------------- */
        /* 8 byte alignment             */
        /* ---------------------------- */
        case EDDP_GLB_ALIGNMENT_8_BYTE:
        {
            if ( (EDDP_GET_BIT_VALUE( Value, 
                                      EDDP_GLB_ALIGNMENT_8_MASK, 
                                      EDDP_GLB_ALIGNMENT_SHIFT))
               )
            {
                State = LSA_FALSE;  // not aligned!
            }
        }
        break;

        /* ---------------------------- */
        /* 16 byte alignment            */
        /* ---------------------------- */
        case EDDP_GLB_ALIGNMENT_16_BYTE:
        {
            if ( (EDDP_GET_BIT_VALUE( Value, 
                                      EDDP_GLB_ALIGNMENT_16_MASK, 
                                      EDDP_GLB_ALIGNMENT_SHIFT))
               )
            {
                State = LSA_FALSE;  // not aligned!
            }
        }
        break;        
        
        /* ---------------------------- */
        /* 64 byte alignment            */
        /* ---------------------------- */
        case EDDP_GLB_ALIGNMENT_64_BYTE:
        {
            if ( (EDDP_GET_BIT_VALUE( Value, 
                                      EDDP_GLB_ALIGNMENT_64_MASK, 
                                      EDDP_GLB_ALIGNMENT_SHIFT))
               )
            {
                State = LSA_FALSE;  // not aligned!
            }
        }
        break;        

        default:
        {
            EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_GlbIsAligned(): FATAL -> given Alignment(0x%X) is unknown!", 
                Alignment);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }

    return (State);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_GlbCrtRQBHandler                                   +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                   (in)      +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function is a handler between EDDP-Requests and RQB-Queue.       +*/
/*+                                                                                     +*/
/*+               If error, request will be finished here by RQBHandler.                +*/
/*+                                                                                     +*/
/*+               If no error, request will be finished here if synchron or             +*/
/*+               after command execution in a CallBackFunction if asynchron.           +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GlbCrtRQBHandler(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB)
{
    EDD_RSP                             Response;
    LSA_BOOL                            Ready;
    LSA_UINT8                           isRTC3PSMRunningAsynMask;
    EDD_SERVICE                         Service;
    EDD_UPPER_RQB_PTR_TYPE              pRQB;
    EDDP_LOCAL_HDB_PTR_TYPE             pHDB;
    EDDP_GLB_CRT_RQB_HANDLER_PTR_TYPE   pRQBHandler;

    Response                    = EDD_STS_OK;
    Ready                       = LSA_FALSE;
    isRTC3PSMRunningAsynMask    = 0;
    pRQB                        = LSA_NULL;
    pHDB                        = LSA_NULL;
    pRQBHandler                 = &pDDB->GlbCrtRQBHandler;

    #ifdef EDDP_MESSAGE
    #ifdef EDDP_PRAGMA_MESSAGE
    #pragma EDDP_PRAGMA_MESSAGE("Workaround CRT.WrkarPackGrpDisable for PNIP-Revision(1) - AP01372025")
    #endif /* EDDP_PRAGMA_MESSAGE */
    #endif /* EDDP_MESSAGE */
    #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
    if ( EDDP_HAL_HW_PNIP_REV1 == EDDP_HALGetPnipRevision(pDDB) )
    {
        isRTC3PSMRunningAsynMask = pDDB->SWI.isRTC3PSMRunningAsynMask;
    }
    #endif

    EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_GlbCrtRQBHandler(): RqbQueue.Count=%d, RqbInProgressQueue.Count=%d, isRTC3PSMRunningAsynMask=0x%X", 
        pRQBHandler->RqbQueue.Count, 
        pRQBHandler->RqbInProgressQueue.Count, 
        isRTC3PSMRunningAsynMask);

    /* return if ...                               */
    /* ... a RQB sequence is already running OR    */
    /* ... a CRT command sequence is still in work */
    if( !EDDP_GlbRQBQueueIsEmpty(&pRQBHandler->RqbInProgressQueue) || EDDP_CRTCmdIsSequenceInWork(pDDB) )
    {
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_GlbCrtRQBHandler(): Either RQB sequence is currently being processed or a CRT command sequence is not yet finished!");
    }
    else
    {    
        /* -------------------------------------------------------- */
        /* check if a Consumer Scoreboard event is pending.         */
        /* If so call the Scoreboard handler.                       */
        /* -------------------------------------------------------- */
        if ( pDDB->CRT.Cons.SBEventPending &&
             (isRTC3PSMRunningAsynMask == 0) )
        {
            EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_GlbCrtRQBHandler(): Consumer Scoreboard event is pending - call the ScoreboardHandler");
            EDDP_CRTScoreboardHandler (pDDB);
        }
        
        /* ------------------------------------------------------------ */
        /* process RQB's till ...                                       */
        /*  ... abort condition is not achieved AND                     */
        /*  ... no RTC3PSM(Port1/2) is running asynchron                */
        /* ------------------------------------------------------------ */
        while ( (!Ready) && (isRTC3PSMRunningAsynMask == 0) )
        {
            Ready = EDDP_GlbCrtRQBHandlerCheckAbortConditions(pDDB, pRQBHandler);
            
            if( !Ready )
            {        
                /* get first RQB from queue */
                pRQB = EDDP_GlbRQBQueueRemoveFromBegin (&(pRQBHandler->RqbQueue));
    
                /* get Handle for this RQB */
                Response = EDDP_GlbHandleGetHDB (EDD_RQB_GET_HANDLE_LOWER(pRQB), &pHDB);
    
                if (Response != EDD_STS_OK)
                {
                    EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "EDDP_GlbCrtRQBHandler(): Invalid handle lower in RQB!");
                    EDD_RQB_SET_RESPONSE(pRQB, EDD_STS_ERR_PARAM);
                    EDDP_RQB_ERROR(pRQB);
                }
                else
                {   
                    /* queue RQB to "in progress" queue for later usage */
                    EDDP_GlbRQBQueueAddToEnd(&pRQBHandler->RqbInProgressQueue, pRQB);
                    
                    /* execute this RQB now */
                    /* Important: Use pHDB->pDDB which was extracted from the RQB! */
                    Response = EDDP_GlbRequestFromGlbCrtRQBHandler (pRQB, pHDB->pDDB, pHDB);
    
                    if (Response == EDD_STS_OK_PENDING)
                    {
                        /* do nothing ... request is pending ... */
                    }
                    else
                    {
                        /* synchronous request or request failed */
                        
                        /* remove RQB from end of "in progress" queue */
                        pRQB = EDDP_GlbRQBQueueRemoveFromEnd(&pRQBHandler->RqbInProgressQueue);
                        
                        /* store Service from RQB before RequestFinish */
                        Service = EDD_RQB_GET_SERVICE(pRQB);
    
                        /* confirm request */
                        EDDP_GlbRequestFinish (pHDB, pRQB, Response);
    
                        /* trigger the finish events of this Service */
                        EDDP_GlbTriggerFinishEventAfterRQBConfirmation (pDDB, pHDB, Service);
                    }
                }
            }
        }
        
        /* trigger CRT command queue */
        EDDP_CRTCmdTriggerCrtQueue(pDDB);
    }

    EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_GlbCrtRQBHandler(): RqbQueue.Count=%d, RqbInProgressQueue.Count=%d", 
        pRQBHandler->RqbQueue.Count, pRQBHandler->RqbInProgressQueue.Count);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbRequestFromGlbCrtRQBHandler         +*/
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
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters          (in) +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function handles all Requests for EDDP               +*/
/*+               which are called from GlbCrtRQBHandler.                   +*/
/*+                                                                         +*/
/*+               If the request is unknown -> FATAL!                       +*/
/*+               This should have been stated by caller of GlbCrtRQBHandler+*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GlbRequestFromGlbCrtRQBHandler(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP         Response    = EDD_STS_OK;
    EDD_SERVICE     Service     = EDD_RQB_GET_SERVICE(pRQB);

    switch (Service)
    {
        /* ---------------------------------------------------------------- */
        /* provider add                                                     */
        /* ---------------------------------------------------------------- */
        case EDD_SRV_SRT_PROVIDER_ADD:
        {
            #if (EDDP_CFG_TRACE_MODE != 0)
            EDD_UPPER_CSRT_PROVIDER_ADD_PTR_TYPE    pRqbParam = (EDD_UPPER_CSRT_PROVIDER_ADD_PTR_TYPE) pRQB->pParam;
            #endif  
            EDDP_UPPER_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_SRT_PROVIDER_ADD (ProvProperties=0x%X, FrameID=0x%X, DataLen=%d): pCRT->RunningRQBCount=%d", 
                pHDB->Handle, pRqbParam->Properties, pRqbParam->FrameID, pRqbParam->DataLen, pHDB->pCRT->RunningRQBCount);

            Response = EDDP_CRTProviderAdd (pRQB, pDDB, pHDB);
        }
        break;

        /* ---------------------------------------------------------------- */
        /* provider control                                                 */
        /* ---------------------------------------------------------------- */
        case EDD_SRV_SRT_PROVIDER_CONTROL:
        {
            #if (EDDP_CFG_TRACE_MODE != 0)
            EDD_UPPER_CSRT_PROVIDER_CONTROL_PTR_TYPE    pRqbParam = (EDD_UPPER_CSRT_PROVIDER_CONTROL_PTR_TYPE) pRQB->pParam;
            #endif  
            EDDP_UPPER_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_SRT_PROVIDER_CONTROL (ProviderID=%d, Mode=0x%X, FrameID=0x%X): pCRT->RunningRQBCount=%d", 
                pHDB->Handle, pRqbParam->ProviderID, pRqbParam->Mode, pRqbParam->FrameID, pHDB->pCRT->RunningRQBCount);

            Response = EDDP_CRTProviderControl (pRQB, pDDB, pHDB);
        }
        break;

        /* ---------------------------------------------------------------- */
        /* provider remove                                                  */
        /* ---------------------------------------------------------------- */
        case EDD_SRV_SRT_PROVIDER_REMOVE:
        {
            #if (EDDP_CFG_TRACE_MODE != 0)
            EDD_UPPER_CSRT_PROVIDER_REMOVE_PTR_TYPE     pRqbParam = (EDD_UPPER_CSRT_PROVIDER_REMOVE_PTR_TYPE) pRQB->pParam;
            #endif
            EDDP_UPPER_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_SRT_PROVIDER_REMOVE (ProviderID=%d): pCRT->RunningRQBCount=%d", 
                pHDB->Handle, pRqbParam->ProviderID, pHDB->pCRT->RunningRQBCount);

            Response = EDDP_CRTProviderRemove (pRQB, pDDB, pHDB);
        }
        break;

        /* ---------------------------------------------------------------- */
        /* provider change phase                                            */
        /* ---------------------------------------------------------------- */
        case EDD_SRV_SRT_PROVIDER_CHANGE_PHASE:
        {
            #if (EDDP_CFG_TRACE_MODE != 0)
            EDD_UPPER_CSRT_PROVIDER_CHANGE_PHASE_PTR_TYPE   pRqbParam = (EDD_UPPER_CSRT_PROVIDER_CHANGE_PHASE_PTR_TYPE) pRQB->pParam;
            #endif
            EDDP_UPPER_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_SRT_PROVIDER_CHANGE_PHASE (ProviderID=%d, CyclePhase=%d, CyclePhaseSequence=0x%X): pCRT->RunningRQBCount=%d", 
                pHDB->Handle, pRqbParam->ProviderID, pRqbParam->CyclePhase, pRqbParam->CyclePhaseSequence, pHDB->pCRT->RunningRQBCount);

            Response = EDDP_CRTProviderChangePhase (pRQB, pDDB, pHDB);
        }
        break;

        /* ---------------------------------------------------------------- */
        /* consumer add                                                     */
        /* ---------------------------------------------------------------- */
        case EDD_SRV_SRT_CONSUMER_ADD:
        {
            #if (EDDP_CFG_TRACE_MODE != 0)
            EDD_UPPER_CSRT_CONSUMER_ADD_PTR_TYPE    pRqbParam = (EDD_UPPER_CSRT_CONSUMER_ADD_PTR_TYPE) pRQB->pParam;
            #endif
            EDDP_UPPER_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_SRT_CONSUMER_ADD (ConsProperties=0x%X, FrameID=0x%X, DataLen=%d): pCRT->RunningRQBCount=%d", 
                pHDB->Handle, pRqbParam->Properties, pRqbParam->FrameID, pRqbParam->DataLen, pHDB->pCRT->RunningRQBCount);

            Response = EDDP_CRTConsumerAdd (pRQB, pDDB, pHDB);
        }
        break;

        /* ---------------------------------------------------------------- */
        /* consumer control                                                 */
        /* ---------------------------------------------------------------- */
        case EDD_SRV_SRT_CONSUMER_CONTROL:
        {
            #if (EDDP_CFG_TRACE_MODE != 0)
            EDD_UPPER_CSRT_CONSUMER_CONTROL_PTR_TYPE    pRqbParam = (EDD_UPPER_CSRT_CONSUMER_CONTROL_PTR_TYPE) pRQB->pParam;
            #endif
            EDDP_UPPER_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_SRT_CONSUMER_CONTROL (ConsumerID=%d, Mode=0x%X): pCRT->RunningRQBCount=%d", 
                pHDB->Handle, pRqbParam->ConsumerID, pRqbParam->Mode, pHDB->pCRT->RunningRQBCount);

            Response = EDDP_CRTConsumerControl (pRQB, pDDB, pHDB);
        }
        break;

        /* ---------------------------------------------------------------- */
        /* consumer remove                                                  */
        /* ---------------------------------------------------------------- */
        case EDD_SRV_SRT_CONSUMER_REMOVE:
        {
            #if (EDDP_CFG_TRACE_MODE != 0)
            EDD_UPPER_CSRT_CONSUMER_REMOVE_PTR_TYPE     pRqbParam = (EDD_UPPER_CSRT_CONSUMER_REMOVE_PTR_TYPE) pRQB->pParam;
            #endif
            EDDP_UPPER_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_SRT_CONSUMER_REMOVE (ConsumerID=%d): pCRT->RunningRQBCount=%d", 
                pHDB->Handle, pRqbParam->ConsumerID, pHDB->pCRT->RunningRQBCount);

            Response = EDDP_CRTConsumerRemove (pRQB, pDDB, pHDB);
        }
        break;

        /* ---------------------------------------------------------------- */
        /* change the COM-Cycle (send clock)                                */
        /* ---------------------------------------------------------------- */
        case EDD_SRV_SENDCLOCK_CHANGE:
        {
            #if (EDDP_CFG_TRACE_MODE != 0)
            EDD_UPPER_SENDCLOCK_CHANGE_PTR_TYPE     pRqbParam = (EDD_UPPER_SENDCLOCK_CHANGE_PTR_TYPE) pRQB->pParam;
            #endif
            EDDP_UPPER_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"[H:%2X] >>> Request: EDD_SRV_SENDCLOCK_CHANGE (CycleBaseFactor=%d): pSWI->RunningRQBCount=%d", 
                pHDB->Handle, pRqbParam->CycleBaseFactor, pHDB->pSWI->RunningRQBCount);

            Response = EDDP_SWI_Change_ComCycle (pRQB, pDDB, pHDB);
        }
        break;

        /* ---------------------------------------------------------------- */
        /* PrmRead                                                          */
        /* ---------------------------------------------------------------- */
        case EDD_SRV_PRM_READ:
        {
            #if (EDDP_CFG_TRACE_MODE != 0)
            EDD_UPPER_PRM_READ_PTR_TYPE     pRqbParam = (EDD_UPPER_PRM_READ_PTR_TYPE) pRQB->pParam;
            #endif
            EDDP_UPPER_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_PRM_READ (record_index=0x%X, edd_port_id=%d): pPRM->RunningRQBCount=%d", 
                pHDB->Handle, pRqbParam->record_index, pRqbParam->edd_port_id, pHDB->pPRM->RunningRQBCount);

            Response = EDDP_PRMRequestPrmRead (pRQB, pDDB, pHDB);
        }
        break;

        /* ---------------------------------------------------------------- */
        /* PrmPrepare                                                       */
        /* ---------------------------------------------------------------- */
        case EDD_SRV_PRM_PREPARE:
        {
            EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_PRM_PREPARE: pPRM->RunningRQBCount=%d", 
                pHDB->Handle, pHDB->pPRM->RunningRQBCount);

            Response = EDDP_PRMRequestPrmPrepare (pRQB, pDDB, pHDB);
        }
        break;

        /* ---------------------------------------------------------------- */
        /* PrmWrite                                                         */
        /* ---------------------------------------------------------------- */
        case EDD_SRV_PRM_WRITE:
        {
            #if (EDDP_CFG_TRACE_MODE != 0)
            EDD_UPPER_PRM_WRITE_PTR_TYPE    pRqbParam = (EDD_UPPER_PRM_WRITE_PTR_TYPE) pRQB->pParam;
            #endif
            EDDP_UPPER_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_PRM_WRITE (record_index=0x%X, edd_port_id=%d): pPRM->RunningRQBCount=%d", 
                pHDB->Handle, pRqbParam->record_index, pRqbParam->edd_port_id, pHDB->pPRM->RunningRQBCount);

            Response = EDDP_PRMRequestPrmWrite (pRQB, pDDB, pHDB);
        }
        break;

        /* ---------------------------------------------------------------- */
        /* PrmEnd                                                           */
        /* ---------------------------------------------------------------- */
        case EDD_SRV_PRM_END:
        {
            #if (EDDP_CFG_TRACE_MODE != 0)
            EDD_UPPER_PRM_END_PTR_TYPE      pRqbParam = (EDD_UPPER_PRM_END_PTR_TYPE) pRQB->pParam;
            #endif
            EDDP_UPPER_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_PRM_END (Local=0x%X): pPRM->RunningRQBCount=%d", 
                pHDB->Handle, pRqbParam->Local, pHDB->pPRM->RunningRQBCount);

            Response = EDDP_PRMRequestPrmEnd (pRQB, pDDB, pHDB);
        }
        break;

        /* ---------------------------------------------------------------- */
        /* PrmCommit                                                        */
        /* ---------------------------------------------------------------- */
        case EDD_SRV_PRM_COMMIT:
        {
            EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_PRM_COMMIT: pPRM->RunningRQBCount=%d", 
                pHDB->Handle, pHDB->pPRM->RunningRQBCount);

            Response = EDDP_PRMRequestPrmCommit (pRQB, pDDB, pHDB);
        }
        break;

        /* ---------------------------------------------------------------- */
        /* PRM_CHANGE_PORT                                                  */
        /* ---------------------------------------------------------------- */
        case EDD_SRV_PRM_CHANGE_PORT:
        {
            EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_PRM_CHANGE_PORT: pPRM->RunningRQBCount=%d", 
                                pHDB->Handle, 
                                pHDB->pPRM->RunningRQBCount);
            
            /* PULL and PLUG only allowed at these states */
            if ((pDDB->PRM.PrmState == EDDP_PRM_STATE_PREPARED) || (pDDB->PRM.PrmState == EDDP_PRM_STATE_WRITE))
            {
                Response = EDDP_PRMRequestPrmChangePort (pRQB, pDDB, pHDB);
            }
            else
            {
                EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_PRM_CHANGE_PORT: pPRM->RunningRQBCount=%d", 
                                    pHDB->Handle, 
                                    pHDB->pPRM->RunningRQBCount);
                Response = EDD_STS_ERR_SEQUENCE;
            }

        }
        break;

        /* ---------------------------------------------------------------- */
        /* Unknown/Unsupported Request                                      */
        /* this should have been stated by caller of GlbCrtRQBHandler       */
        /* ---------------------------------------------------------------- */
        default:
        {
            EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:%2X] EDDP_GlbRequestFromGlbCrtRQBHandler(): FATAL -> Unknown/Unsupported Service(0x%X)", 
                pHDB->Handle, Service);
            EDDP_FATAL(EDDP_FATAL_ERR_SERVICE);
        }
        break;
    }

    return (Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbTriggerFinishEventAfterRQBConfirmation+*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+                             EDD_SERVICE                 Service         +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                    (in) +*/
/*+  Service    : EDDP-Service                                         (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function is called by EDDP_GlbCrtRQBHandler() or     +*/
/*+               by service always after reguest confirmation!             +*/
/*+                 -> after EDDP_GlbRequestFinish() of RQB !!!             +*/
/*+                                                                         +*/
/*+               This function contains some activities for finish of      +*/
/*+               given service.                                            +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GlbTriggerFinishEventAfterRQBConfirmation(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB,
    EDD_SERVICE                 Service)
{
    EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:%2X] EDDP_GlbTriggerFinishEventAfterRQBConfirmation(Service=0x%X)", 
        pHDB->Handle, Service);

    switch (Service)
    {
        case EDD_SRV_SRT_PROVIDER_ADD:
        case EDD_SRV_SRT_PROVIDER_CONTROL:
        case EDD_SRV_SRT_PROVIDER_REMOVE:
        case EDD_SRV_SRT_PROVIDER_CHANGE_PHASE:
        case EDD_SRV_SRT_CONSUMER_ADD:
        case EDD_SRV_SRT_CONSUMER_REMOVE:
        {
            // decrement the counter of running RQBs
            EDDP_ASSERT_FALSE((pHDB->pCRT->RunningRQBCount) == 0);
            pHDB->pCRT->RunningRQBCount--;
        }
        break;

        case EDD_SRV_SRT_CONSUMER_CONTROL:
        {
            // decrement the counter of running RQBs
            EDDP_ASSERT_FALSE((pHDB->pCRT->RunningRQBCount) == 0);
            pHDB->pCRT->RunningRQBCount--;
            // trigger the ConsumerControl finish
            EDDP_CRTTriggerConsumerControlFinish (pDDB);
        }
        break;

        case EDD_SRV_SENDCLOCK_CHANGE:
        {
            // decrement the counter of running RQBs
            EDDP_ASSERT_FALSE((pHDB->pSWI->RunningRQBCount) == 0);
            pHDB->pSWI->RunningRQBCount--;
        }
        break;

        case EDD_SRV_PRM_READ:
        case EDD_SRV_PRM_PREPARE:
        case EDD_SRV_PRM_WRITE:
        case EDD_SRV_PRM_END:
        {
            // decrement the counter of running RQBs
            EDDP_ASSERT_FALSE((pHDB->pPRM->RunningRQBCount) == 0);
            pHDB->pPRM->RunningRQBCount--;
        }
        break;

        case EDD_SRV_PRM_COMMIT:
        {
            // decrement the counter of running RQBs
            EDDP_ASSERT_FALSE((pHDB->pPRM->RunningRQBCount) == 0);
            pHDB->pPRM->RunningRQBCount--;
            // PrmCommit is no more running
            pDDB->PRM.isPrmCommitRunning = LSA_FALSE;
            // trigger the PrmCommit finish
            EDDP_PRMTriggerPrmCommitFinish (pDDB);
        }
        break;

        case EDD_SRV_PRM_CHANGE_PORT:
        {
            // decrement the counter of running RQBs
            EDDP_ASSERT_FALSE((pHDB->pPRM->RunningRQBCount) == 0);
            pHDB->pPRM->RunningRQBCount--;
        }
        break;

        default:
        {
            EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:%2X] EDDP_GlbTriggerFinishEventAfterRQBConfirmation(Service=0x%X): FATAL -> given Service is unknown!", 
                pHDB->Handle, Service);
            EDDP_FATAL(EDDP_FATAL_ERR_SERVICE);
        }
        break;
    }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_GlbTraceByteArray                      +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDD_UPPER_MEM_U8_PTR_TYPE   pBuffer         +*/
/*+                             LSA_UINT16                  Length          +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pBuffer    : address of byte array                                     +*/
/*+  Length     : length of byte array                                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: Trace the given byte array.                               +*/
/*+                                                                         +*/
/*+               The call of this function must be traced like in this     +*/
/*+               example:                                                  +*/
/*+                                                                         +*/
/*+     #ifdef EDDP_PRM_TRACE_BYTE_ARRAY_ON                                                                                                 +*/
/*+     EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] EDDP_PRMRequestPrmWrite(): >>> TraceByteArray PDIRSubframeData");    +*/
/*+     EDDP_GlbTraceByteArray (pDDB, pPrmWrite->record_data, 100);                                                                         +*/
/*+     EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] EDDP_PRMRequestPrmWrite(): <<< TraceByteArray PDIRSubframeData");    +*/
/*+     #endif                                                                                                                              +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GlbTraceByteArray(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_UPPER_MEM_U8_PTR_TYPE   pBuffer,
    LSA_UINT16                  Length)
{
#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pBuffer);
    LSA_UNUSED_ARG(Length);
#endif

    LSA_UNUSED_ARG(pDDB);

    EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: EDDP_GlbTraceByteArray(pBuffer=0x%X, Length=%d)", pBuffer, Length);
    EDDP_CORE_TRACE_BYTE_ARRAY(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "   Data:", pBuffer, Length);
    EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: EDDP_GlbTraceByteArray()");
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_GlbCrtRQBHandlerCheckAbortConditions               +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_BOOL                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB                              : Pointer to DeviceDescriptionBlock    (in)      +*/
/*+  EDDP_GLB_CRT_RQB_HANDLER_PTR_TYPE : Pointer to global CRT RQB handler    (in)      +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks the abort conditions for a work sequence.        +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_GlbCrtRQBHandlerCheckAbortConditions(
    EDDP_LOCAL_DDB_PTR_TYPE           pDDB,
    EDDP_GLB_CRT_RQB_HANDLER_PTR_TYPE pRQBHandler)
{
    EDD_UPPER_RQB_PTR_TYPE pRQB;
    EDD_SERVICE                     Service;
    EDDP_CMD_QUEUE_INFO_PTR_TYPE    pCrtQueue;
    
    pCrtQueue = &pDDB->CMD.Queue[EDDP_CMD_INDEX_HOST_ASYNC_CRT];
    pRQB      = EDDP_GlbRQBPeekFirstElement(&pRQBHandler->RqbQueue);
    
    /* no more RQB in queue */
    if( LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL) )
    {
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_GlbCrtRQBHandlerCheckAbortConditions(): no more RQB in queue!");
        
        return LSA_TRUE;
    }
    
    /* maximum CRT commands reached */
    if( pCrtQueue->CmdRunningCount >= pDDB->CMD.CrtCmdDepth )
    {
        EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_GlbCrtRQBHandlerCheckAbortConditions(): maximum PPM/CPM command depth reached (RunningCount=%d)!", 
            pCrtQueue->CmdRunningCount);
        
        return LSA_TRUE;
    }
    
    /* get service from RQB */
    Service = EDD_RQB_GET_SERVICE(pRQB);
    
    /* non-CRT request */
    if( EDD_SRV_SRT_TYPE != (EDD_SRV_TYPE_MASK & Service) )
    {
        /* any CRT commands currently running? */
        if( 0 != pCrtQueue->CmdRunningCount )
        {
            EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_GlbCrtRQBHandlerCheckAbortConditions(): non-CRT service (0x%X) will be blocked until previous CRT-services will return!", Service);
            return LSA_TRUE;
        }
        
        /* any other non-CRT request in work? */
        if( 0 != pRQBHandler->RqbInProgressQueue.Count )
        {
            EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_GlbCrtRQBHandlerCheckAbortConditions(): non-CRT service (0x%X) will be blocked until previous non-CRT service will return!", Service);
            return LSA_TRUE;
        }
    }
    
    return LSA_FALSE;
}

#ifdef __cplusplus
}
#endif
/*****************************************************************************/
/*  end of file eddp_core_glb.c                                              */
/*****************************************************************************/

