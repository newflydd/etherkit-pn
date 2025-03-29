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
/*  C o m p o n e n t     &C: EDDP (EthernetDeviceDriver for PNIP)      :C&  */
/*                                                                           */
/*  F i l e               &F: eddp_usr_sysred_api.c                     :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/* H i s t o r y :                                                           */
/* ________________________________________________________________________  */
/*                                                                           */
/* Date             Who            What                                      */
#ifdef EDDP_MESSAGE
/* 2013-JAN-14      z00301au       Initial Version                           */
#endif
/*                                                                           */
/*****************************************************************************/

#include "eddp_inc.h"           /* eddp header files */
#include "eddp_int.h"
#include "pnip_xrsl.h"          /* PNIP register defines */
#include "eddp_trc.h"           /* EDDP Trace */
#include "eddp_usr_sysred_intern.h" /* internal structures for management    */
#ifdef EDDP_CFG_SYSRED_API_SUPPORT
/*===========================================================================*/
/*                              Vars                                         */
/*===========================================================================*/

EDDP_IO_CB_TYPE     eddp_IOCB[EDDP_CFG_MAX_DEVICES];
EDDP_IO_CB_TYPE *   eddp_IOInstanceList[EDD_INTERFACE_ID_MAX+1];

#define LTRC_ACT_MODUL_ID   151
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_USR_SYSRED_API */

/*===========================================================================*/
/*                              Local Macros                                 */
/*===========================================================================*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Macroname             :    EDDP_IO_GET_IOCB                            +*/
/*+  Input/Output          :    EDDP_IO_CB_TYPE         pIOCB_              +*/
/*+                             LSA_UINT32              InstanceHandle_     +*/
/*+                                                                         +*/
/*+  Result                :    none                                        +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to IO CB Block                              (in)  +*/
/*+  pDSB       : Number of the instance                              (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Initialisation of the system for controling the provider  +*/
/*+                  DataStatus                                             +*/
/*+               This macro definition allows to use the return ( ... );   +*/
/*+               statement in error situations.                            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define EDDP_IO_GET_IOCB(pIOCB_, InstanceHandle_)  {                                                                                    \
        if ((InstanceHandle_) > EDD_INTERFACE_ID_MAX)                                                                                   \
        {                                                                                                                               \
            EDDP_USER_TRACE_03(0, LSA_TRACE_LEVEL_ERROR, "[H:%2X] eddp_SysRed_xxx Line:%d ERROR ->illegal InstanceHandle 0x%X",  InstanceHandle_,__LINE__, InstanceHandle_); \
            return (EDD_STS_ERR_PARAM);                                                                                                 \
        }                                                                                                                               \
                                                                                                                                        \
        (pIOCB_) = eddp_IOInstanceList[InstanceHandle_];                                                                                \
        if((LSA_NULL == pIOCB_) || (EDD_DATAOFFSET_INVALID == (LSA_UINT32) pIOCB_ ))                                                    \
        {                                                                                                                               \
            EDDP_USER_TRACE_03(0, LSA_TRACE_LEVEL_ERROR, "[H:%2X] eddp_SysRed_xxx Line:%d ERROR ->InstanceList not initialized! InstanceHandle 0x%X", InstanceHandle_,__LINE__, InstanceHandle_);\
            return(EDD_STS_ERR_EXCP);                                                                                               \
        }                                                                                                                               \
        if ((pIOCB_)->InstanceHandle != InstanceHandle_)                                                                                \
        {                                                                                                                               \
            EDDP_USER_TRACE_03(0, LSA_TRACE_LEVEL_ERROR, "[H:%2X] eddp_SysRed_xxx Line:%d ERROR ->illegal InstanceHandle 0x%X",  InstanceHandle_, __LINE__, InstanceHandle_);\
            return (EDD_STS_ERR_PARAM);                                                                                                 \
        }                                                                                                                               \
    }\
     
/*===========================================================================*/
/*                              Local functions                              */
/*===========================================================================*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _eddp_usr_sysred_checkAlignment             +*/
/*+  Input/Output          :    LSA_UINT32 Value                            +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  Value      : Value to check for alignment                        (in)  +*/
/*+                                                                         +*/
/*+  Results    : LSA_TRUE                                                  +*/
/*+               LSA_FALSE                                                 +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:   This function checks the alignment of a given memory    +*/
/*+                 pointer, e.g. the alignment of the DataStatusOffset     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static EDD_RSP eddp_usr_sysred_check2ByteAlignment(LSA_UINT32 Value)
{
    if ( (EDDP_GET_BIT_VALUE( Value,
                              EDDP_GLB_ALIGNMENT_2_MASK,
                              EDDP_GLB_ALIGNMENT_SHIFT))
       )
    {
        return LSA_FALSE;  // not aligned!
    }
    return LSA_TRUE;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_UserSysRedInternFatalError             +*/
/*+  Input/Output          :    EDDP_FATAL_ERROR_TYPE       Error           +*/
/*+                             LSA_UINT16                  ModuleID        +*/
/*+                             LSA_UINT32                  Line            +*/
/*+                                                                         +*/
/*+  Result                :    none                                        +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  Error      : The error code displayed in the fatal msg           (in)  +*/
/*+  ModuleID   : module ID of the component causing the error        (in)  +*/
/*+  Line       : line of code                                        (in)  +*/
/*+                                                                         +*/
/*+  Results    : LSA_VOID                                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:   This function traces a fatal error that may occur       +*/
/*+                 during the operation of the SRD API. Note that this     +*/
/*+                 internal function is (re)implemented here to provide a  +*/
/*+                 FATAL Error implementation within the SRD API since     +*/
/*+                 the SRD API code does not use _any_ internal eddp       +*/
/*+                 source code.                                            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_UserSysRedInternFatalError(
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

    EDDP_USER_TRACE_03(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_UserSysRedInternFatalError(ModuleID=%d, Line=%d, Error=0x%X)",
                       ModuleID, Line, Error);

    EDDP_FATAL_ERROR(sizeof(LSA_FATAL_ERROR_TYPE), &LSAError);

    /* should not return! */
    while (1) ;

}

/*===========================================================================*/
/*                              global functions                             */
/*===========================================================================*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_SysRed_init                            +*/
/*+  Input/Output          :    none                                        +*/
/*+                                                                         +*/
/*+  Result                :    none                                        +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output   : LSA_VOID                                              +*/
/*+                                                                         +*/
/*+  Results        : LSA_VOID                                              +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:   Initialisation of the system for controling the provider+*/
/*+                 DataStatus                                              +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID eddp_SysRed_Init( LSA_VOID)
{
    LSA_UINT8  i;
    EDDP_USER_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] --> eddp_SysRed_Init()");

    for (i=0; i<=EDD_INTERFACE_ID_MAX; i++)
    {
        eddp_IOInstanceList[i] = (EDDP_IO_CB_TYPE *)EDD_DATAOFFSET_INVALID;
    }

    for (i=0; i<EDDP_CFG_MAX_DEVICES; i++)
    {
        eddp_IOCB[i].InstanceHandle = EDDP_IO_UNDEFINED_INSTANCE_HANDLE;
    }

    EDDP_USER_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] <-- eddp_SysRed_Init()");
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                              +*/
/*+  Functionname          :    eddp_SysRed_Deinit                               +*/
/*+  Input/Output          :    none                                             +*/
/*+                                                                              +*/
/*+  Result                :    none                                             +*/
/*+------------------------------------------------------------------------------+*/
/*+                                                                              +*/
/*+  Input/Output   : LSA_VOID                                                   +*/
/*+                                                                              +*/
/*+  Results        : LSA_VOID                                                   +*/
/*+------------------------------------------------------------------------------+*/
/*+  Description:   Deinitialisation of the management structures for all        +*/
/*+                 Instances                                                    +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID eddp_SysRed_Deinit( LSA_VOID)
{
    LSA_UINT8  i;
    EDDP_USER_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] --> eddp_SysRed_Deinit()");
    
    for (i=0; i<=EDD_INTERFACE_ID_MAX; i++)
    {
        eddp_IOInstanceList[i] = (EDDP_IO_CB_TYPE *)EDD_DATAOFFSET_INVALID;
    }
    
    for (i=0; i<EDDP_CFG_MAX_DEVICES; i++)
    {
        eddp_IOCB[i].InstanceHandle = EDDP_IO_UNDEFINED_INSTANCE_HANDLE;
    }
    
    EDDP_USER_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] <-- eddp_SysRed_Deinit()");

}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_SysRed_Setup                           +*/
/*+  Input/Output          :    LSA_UINT32 const    InstanceHandle          +*/
/*+                             LSA_UINT32 const    TraceIdx                +*/
/*+                             LSA_UINT32 const    InstanceHandle          +*/
/*+                             LSA_VOID_PTR_TYPE   pPnipBase               +*/
/*+                             LSA_UINT32 const    TraceIdx                +*/
/*+                             LSA_VOID_PTR_TYPE   pGSharedRAM             +*/
/*+                             LSA_VOID_PTR_TYPE   pPnipBase               +*/
/*+                             LSA_UINT16          Hardware                +*/
/*+                                                                         +*/ 
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  InstanceHandle :   An instance handle unambiguously identifying   (in) +*/
/*+                     the used device/interface throughout the whole      +*/
/*+                     system. It is advised to use the InterfaceID.       +*/
/*+  TraceIdx       :   Trace index                                    (in) +*/
/*+  pPnipBase      :   Pointer to the start of KRAM. In PNIP,         (in) +*/
/*+                     this is the pointer to the start of the             +*/
/*+                     API Ctrl RAM. Since EDDI and EDDP share the same    +*/
/*+                     API naming convention, it's also called pKRAM here. +*/
/*+                     This is a virtual address (as the user sees the     +*/
/*+                     Memory).                                            +*/
/*+  pGSharedRAM    :   Pointer to the start of the shared memory     (in)  +*/
/*+                     block given by the system adaption. It's a memory   +*/
/*+                     block in the host  system to the same memory area   +*/
/*+                     as given in the Device Parameter Block (see         +*/
/*+                     EDD_SRV_DEV_OPEN).                                  +*/
/*+                     This is a virtual address (as the user sees the     +*/
/*+                     Memory).                                            +*/
/*+  Hardware       :   Hardware on which the EDDP runs (same paramter as   +*/
/*+                     in DPB )                                            +*/
/*+                                                                         +*/
/*+  Results        :   EDD_STS_OK                                          +*/
/*+                     EDD_STS_ERR_PARAM                                   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP eddp_SysRed_Setup (
    LSA_UINT32          const   InstanceHandle, 
    LSA_UINT32          const   TraceIdx, 
    LSA_VOID_PTR_TYPE           pPnipBase, 
    LSA_VOID_PTR_TYPE           pGSharedRAM,
    LSA_UINT16          const   Hardware)

{
    LSA_UINT16 id;
    EDDP_IO_CB_TYPE * pIOCB = LSA_NULL;

    EDDP_USER_TRACE_02(TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] --> eddp_SysRed_Setup(): pPnipBase:0x%X",
                       InstanceHandle, (LSA_UINT32)pPnipBase);


    /* Check pointers to PNIP API Ctrl RAM and GSharedRAM */
    if(InstanceHandle < 1 && InstanceHandle > EDD_INTERFACE_ID_MAX)
    {
        EDDP_USER_TRACE_01(TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] eddp_SysRed_Setup(): ERROR -> InstanceHandle is ouut of bound",
                           InstanceHandle);
        return EDD_STS_ERR_PARAM;      
    }
    if(LSA_HOST_PTR_ARE_EQUAL(pPnipBase, LSA_NULL))
    {
        EDDP_USER_TRACE_01(TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] eddp_SysRed_Setup(): ERROR -> pPnipBase Pointer is NULL",
                           InstanceHandle);
        return EDD_STS_ERR_PARAM;
    }
    if(LSA_HOST_PTR_ARE_EQUAL(pGSharedRAM, LSA_NULL))
    {
        EDDP_USER_TRACE_01(TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] eddp_SysRed_Setup(): ERROR -> GSharedRAM Pointer is NULL",
                           InstanceHandle);
        return EDD_STS_ERR_PARAM;
    }
    if( Hardware != EDDP_HW_ERTEC200P      &&
        Hardware != EDDP_HW_ERTEC200P_FPGA &&
        Hardware != EDDP_HW_HERA           &&
        Hardware != EDDP_HW_HERA_FPGA
      )
    {
        EDDP_USER_TRACE_01(TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] eddp_SysRed_Setup(): ERROR -> Hardware is not supported",
                           InstanceHandle);
        return EDD_STS_ERR_PARAM;
    }

    /* find next free instance */
    for (id=0; id<EDDP_CFG_MAX_DEVICES; id++)
    {
        if (InstanceHandle == eddp_IOCB[id].InstanceHandle)
        {
            EDDP_USER_TRACE_02(TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] eddp_SysRed_Setup(): ERROR -> InstanceHandle 0x%X exists",
                               InstanceHandle, InstanceHandle);
            return EDD_STS_ERR_PARAM;
        }
        else if (EDDP_IO_UNDEFINED_INSTANCE_HANDLE == eddp_IOCB[id].InstanceHandle)
        {
            eddp_IOInstanceList[InstanceHandle] = &eddp_IOCB[id];  
            eddp_IOCB[id].InstanceHandle = InstanceHandle;
            break;
        }
    }

    if (id>=EDDP_CFG_MAX_DEVICES)
    {
        EDDP_USER_TRACE_01(TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] eddp_SysRed_Setup(): ERROR -> no more instances",
                           InstanceHandle);
        return EDD_STS_ERR_SEQUENCE;
    }

    /* Initialize the Management Structures */
    EDDP_IO_GET_IOCB(pIOCB, InstanceHandle);  

    pIOCB->pGSharedRAM  = (EDDP_GSHAREDMEM_TYPE *)pGSharedRAM;
    pIOCB->pPnipBase    = pPnipBase; // pPnipBase (API Ctrl RAM), API name shall be the same as EDDI SysRed API
    pIOCB->TraceIdx     = TraceIdx;
    pIOCB->Hardware     = Hardware;

    for (id=0; id<EDDP_MAX_NR_APPL_PROVIDERS ; id++)
    {
        pIOCB->provider_list[id].provider_id       = EDDP_IO_UNKNOWN_PROV_ID;
    }

    for (id=0; id < EDD_CFG_MAX_SRSHM_SIZE; id++)
    {
        pIOCB->pGSharedRAM->SRSHM[id] = 0;
    }

    EDDP_USER_TRACE_01(TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] <-- eddp_SysRed_Setup()", InstanceHandle);
    return EDD_STS_OK;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_SysRed_UndoSetup                       +*/
/*+  Input/Output          :    LSA_UINT32              InstanceHandle      +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  InstanceHandle         : Handle to instance                      (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Free instance resources                                   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP eddp_SysRed_UndoSetup (
    LSA_UINT32    const   InstanceHandle
)
{
    EDDP_IO_CB_TYPE  *  pIOCB = LSA_NULL;
    EDDP_IO_GET_IOCB(pIOCB, InstanceHandle);

    (pIOCB)->InstanceHandle = EDDP_IO_UNDEFINED_INSTANCE_HANDLE;

    return EDD_STS_OK;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_SysRed_Provider_Add                    +*/
/*+  Input/Output          :    LSA_UINT32         InstanceHandle           +*/
/*+                             LSA_UINT16    *    pApplProviderID          +*/
/*+                             LSA_UINT32         DataStatusOffset         +*/
/*+                             LSA_UINT8          ProvType                 +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  InstanceHandle   : Handle to instance                            (in)  +*/
/*+  pApplProviderID  : Ptr to ApplProviderID                         (in)  +*/
/*+  DataStatusOffset : Offset of the DataStatus from the beginning   (in)  +*/
/*+                     of KRAM (== API CTRL RAM in PNIP)                   +*/
/*+  ProvType         : provider type, possible values:                     +*/
/*+                        EDD_PROVADD_TYPE_DEFAULT                         +*/
/*+                        EDD_PROVADD_TYPE_SYSRED                          +*/
/*+                                                                         +*/
/*+  Results          : EDD_STS_OK                                          +*/
/*+                     EDD_STS_ERR_PARAM                                   +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Add a provider, only on ERTEC200P otherwise an error      +*/
/*+               EDD_STS_ERR_NOT_ALLOWED is returned                       +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP eddp_SysRed_Provider_Add (
    LSA_UINT32      const   InstanceHandle,
    LSA_UINT16    * const   pApplProviderID,
    LSA_UINT32      const   DataStatusOffset,
    LSA_UINT8       const   ProvType)
{
    EDDP_IO_PROVIDER_LIST_ENTRY_TYPE  * pProvider   = LSA_NULL;
    LSA_UINT16                          ProviderID  = 0x0;
    EDDP_IO_CB_TYPE                   * pIOCB       = LSA_NULL;
    LSA_UINT32                          regVal      = 0x0;


    //this macros guarantees to returns a valid pIOCB otherwise the execution of the current function
    // is abored with an error code
    EDDP_IO_GET_IOCB(pIOCB, InstanceHandle);

    EDDP_USER_TRACE_02(pIOCB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] --> eddp_SysRed_Provider_Add(): DataStatusOffset:0x%X",
                       InstanceHandle,
                       (LSA_UINT32)DataStatusOffset
                      );
    if(pIOCB->Hardware != EDDP_HW_ERTEC200P &&
       pIOCB->Hardware != EDDP_HW_ERTEC200P_FPGA
      )
    {
        EDDP_USER_TRACE_02(pIOCB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] eddp_SysRed_Provider_Add(): ERROR -> Hardware [H:%2X] not allowed only allowed for EDDP_HW_ERTEC200P and EDDP_HW_ERTEC200P_FPGA!", InstanceHandle, pIOCB->Hardware);
        return (EDD_STS_ERR_NOT_ALLOWED);
    }
    /***************************/
    /* check validity          */
    /***************************/
    // Check the Provider Type. Note that, unlike EDDi, EDD_PROVADD_TYPE_DEFAULT is not allowed within EDDP
    if (EDD_PROVADD_TYPE_SYSRED  != ProvType)
    {
        if(EDD_PROVADD_TYPE_DEFAULT == ProvType)
        {
            EDDP_USER_TRACE_02(pIOCB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] eddp_SysRed_Provider_Add(): ERROR -> illegal ProvType:0x%X. EDD_PROVADD_TYPE_DEFAULT is not allowed within EDDP!", InstanceHandle, ProvType);
            return (EDD_STS_ERR_PARAM);
        }
        else
        {
            EDDP_USER_TRACE_02(pIOCB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] eddp_SysRed_Provider_Add(): ERROR -> illegal ProvType:0x%X", InstanceHandle, ProvType);
            return (EDD_STS_ERR_PARAM);
        }
    }

    // Check the alignment of the DataStatusOffset. It has to be 2Byte aligned.
    if(! eddp_usr_sysred_check2ByteAlignment(DataStatusOffset))
    {
        EDDP_USER_TRACE_02(pIOCB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] eddp_SysRed_Provider_Add(): ERROR -> DataStatusOffset:0x%X is not 2 Byte aligned!", InstanceHandle, DataStatusOffset);
        return (EDD_STS_ERR_PARAM);
    }

    // Check the Offset of the DataStatusOffset. It has to be within the API Ctrl RAM.

    /* PNIP_RAM_APICTRL_RAM_END was replaced by PNIP_R2P2_RAM_APICTRL_RAM_END, hardware dependency*/
    /* PNIP_RAM_APICTRL_RAM_START was replaced by PNIP_R2P2_RAM_APICTRL_RAM_START, hardware dependency*/
    if((DataStatusOffset > PNIP_R2P2_RAM_APICTRL_RAM_END) || (DataStatusOffset < PNIP_R2P2_RAM_APICTRL_RAM_START))
    {
        /* PNIP_RAM_APICTRL_RAM_END was replaced by PNIP_R2P2_RAM_APICTRL_RAM_END, hardware dependency*/
        /* PNIP_RAM_APICTRL_RAM_START was replaced by PNIP_R2P2_RAM_APICTRL_RAM_START, hardware dependency*/
        EDDP_USER_TRACE_04(pIOCB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] eddp_SysRed_Provider_Add(): ERROR -> DataStatusOffset:0x%X is out of boundaries (0x%X...0x%X) of the APICtrl-RAM!",
                           InstanceHandle, DataStatusOffset, PNIP_R2P2_RAM_APICTRL_RAM_START, PNIP_R2P2_RAM_APICTRL_RAM_END);
        return (EDD_STS_ERR_PARAM);
    }

    /***************************/
    /* Find free provider      */
    /***************************/
    *pApplProviderID = EDDP_IO_UNKNOWN_PROV_ID;
    pProvider = &pIOCB->provider_list[0];

    for (ProviderID=0; ProviderID<EDDP_MAX_NR_APPL_PROVIDERS; ProviderID++)
    {
        if (EDDP_IO_UNKNOWN_PROV_ID == pProvider->provider_id)
        {
            //found!
            *pApplProviderID         = ProviderID;
            pProvider->provider_id   = ProviderID;

            break;
        }
        pProvider++;
    }

    if (EDDP_IO_UNKNOWN_PROV_ID == *pApplProviderID)
    {
        EDDP_USER_TRACE_02(pIOCB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] eddp_SysRed_Provider_Add(): ERROR -> No more providers in Instance 0x%X", InstanceHandle, InstanceHandle);
        return EDD_STS_ERR_PARAM;
    }

    /***************************/
    /* Setup provider          */
    /***************************/

    if (EDD_DATAOFFSET_INVALID == DataStatusOffset)
    {
        EDDP_USER_TRACE_03(pIOCB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] eddp_SysRed_Provider_Add(): ERROR -> Invalid DataStatusOffset (0x%X) in Instance 0x%X ", InstanceHandle, DataStatusOffset, InstanceHandle);
        return EDD_STS_ERR_PARAM;
    }
    else
    {
        pProvider->pDataStatus        = (LSA_UINT8 * )((LSA_UINT32)pIOCB->pPnipBase + DataStatusOffset); // BasePtr to PNIP-RAM (host format) + DataStatusOffset from this BaseAdr
    }



    /***************************/
    /* Init provider           */
    /***************************/
    if (EDD_PROVADD_TYPE_SYSRED == ProvType)
    {
        EDDP_ENTER_IO();
        /* read the current value */
        regVal = EDDP_USR_SYSRED_MEM8_READ((EDD_COMMON_MEM_PTR_TYPE) pProvider->pDataStatus );

        /* clear the state bit, don't touch the other bits */
        regVal &= ~EDD_CSRT_DSTAT_BIT_STATE; // set State := BACKUP

        /* write to the hw */
        EDDP_USR_SYSRED_MEM8_WRITE((EDD_COMMON_MEM_PTR_TYPE) pProvider->pDataStatus, regVal);

        EDDP_EXIT_IO();
    }

    EDDP_USER_TRACE_02(pIOCB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] <-- eddp_SysRed_Provider_Add(): ApplProviderID:0x%X",
                       InstanceHandle, (LSA_UINT32)*pApplProviderID);

    return EDD_STS_OK;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CMDDeviceIFSetup                       +*/
/*+  Input/Output          :    LSA_UINT32               InstanceHandle     +*/
/*+                             LSA_UINT16               ApplProviderID     +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  InstanceHandle :   The instance handle.                           (in) +*/
/*+  ApplProviderID :   ProviderID from eddp_SysRed_Provider_Add (in) +*/
/*+                                                                         +*/
/*+  Results        :   EDD_STS_OK                                          +*/
/*+                     EDD_STS_ERR_PARAM                                   +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Remove a provider from the management only on ERTEC200P   +*/
/*+                otherwise an error EDD_STS_ERR_NOT_ALLOWED is returned   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP eddp_SysRed_Provider_Remove(
    LSA_UINT32    const   InstanceHandle,
    LSA_UINT16    const   ApplProviderID
)
{
    EDDP_IO_PROVIDER_LIST_ENTRY_TYPE  *  pProvider;
    EDDP_IO_CB_TYPE                   *  pIOCB = LSA_NULL;

    EDDP_IO_GET_IOCB(pIOCB, InstanceHandle);

    EDDP_USER_TRACE_02(pIOCB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] --> eddp_SysRed_Provider_Remove() -> ProvID:0x%X", InstanceHandle, (LSA_UINT32)ApplProviderID);

    
    if(pIOCB->Hardware != EDDP_HW_ERTEC200P &&
       pIOCB->Hardware != EDDP_HW_ERTEC200P_FPGA
      )
    {
        EDDP_USER_TRACE_02(pIOCB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] eddp_SysRed_Provider_Remove(): ERROR -> Hardware [H:%2X] not allowed only allowed for EDDP_HW_ERTEC200P and EDDP_HW_ERTEC200P_FPGA!", InstanceHandle, pIOCB->Hardware);
        return (EDD_STS_ERR_NOT_ALLOWED);
    }
    /***************************/
    /* check validity          */
    /***************************/
    if (ApplProviderID >= EDDP_MAX_NR_APPL_PROVIDERS)
    {
        EDDP_USER_TRACE_03(pIOCB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] eddp_SysRed_Provider_Remove(): ERROR -> ApplProvID:0x%X bigger then EDDP_MAX_NR_APPL_PROVIDERS:0x%X",
                           InstanceHandle, ApplProviderID, EDDP_MAX_NR_APPL_PROVIDERS);
        return EDD_STS_ERR_PARAM;
    }

    pProvider = &pIOCB->provider_list[ApplProviderID];

    if (pProvider->provider_id == EDDP_IO_UNKNOWN_PROV_ID)
    {
        EDDP_USER_TRACE_02(pIOCB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] eddp_SysRed_Provider_Remove(): ERROR -> Unknown ProvID:0x%X", InstanceHandle, pProvider->provider_id);
        return EDD_STS_ERR_PARAM;
    }

    /***************************/
    /* Init provider           */
    /***************************/
    pProvider->provider_id   = EDDP_IO_UNKNOWN_PROV_ID;
    pProvider->pARSet        = (LSA_UINT16 *)EDD_DATAOFFSET_UNDEFINED;

    EDDP_USER_TRACE_01(pIOCB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] <-- eddp_SysRed_Provider_Remove()", InstanceHandle);
    return EDD_STS_OK;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_ProviderSysRed_SetDS            +*/
/*+  Input/Output          :    LSA_UINT32            InstanceHandle        +*/
/*+                             LSA_UINT16            ApplProviderID        +*/
/*+                             LSA_UINT8             Status                +*/
/*+                             LSA_UINT8             Mask                  +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  InstanceHandle :   The instance handle.                          (in)  +*/
/*+  ApplProviderID :   The provider-ID                               (in)  +*/
/*+  Status         :   Data status value                             (in)  +*/
/*+  Mask           :   Mask for status bits that must be changed     (in)  +*/
/*+                                                                         +*/
/*+  Results        :   EDD_STS_OK                                          +*/
/*+                     EDD_STS_ERR_PARAM                                   +*/
/*+                     EDD_STS_xxx                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: These bits in "DataStatus" can be changed:                +*/
/*+                                                                         +*/
/*+                     Bit(1)  : State        (0 == Backup / 1 == Primary) +*/
/*+                     Bit(2)  : Redundancy   (0 == non Redundant /        +*/
/*+                                                    1 == redundant)      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP eddp_SysRed_ProviderDataStatus_Set (
    LSA_UINT32    const   InstanceHandle,
    LSA_UINT16    const   ApplProviderID,
    LSA_UINT8     const   Status,
    LSA_UINT8     const   Mask
)
{
    LSA_UINT8 regVal;

    EDDP_IO_PROVIDER_LIST_ENTRY_TYPE  *  pProvider;
    EDDP_IO_CB_TYPE                   *  pIOCB = LSA_NULL;

    EDDP_IO_GET_IOCB(pIOCB, InstanceHandle);

    EDDP_USER_TRACE_04(pIOCB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] --> eddp_SysRed_ProviderDataStatus_Set() -> ProvID:0x%X, Status = 0x%X, Mask = 0x%X, ",
                        InstanceHandle,
                       (LSA_UINT32)ApplProviderID,
                       Status,
                       Mask);
    
    if(pIOCB->Hardware != EDDP_HW_ERTEC200P &&
       pIOCB->Hardware != EDDP_HW_ERTEC200P_FPGA
      )
    {
        EDDP_USER_TRACE_02(pIOCB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] eddp_SysRed_ProviderDataStatus_Set(): ERROR -> Hardware [H:%2X] not allowed only allowed for EDDP_HW_ERTEC200P and EDDP_HW_ERTEC200P_FPGA!", InstanceHandle, pIOCB->Hardware);
        return (EDD_STS_ERR_NOT_ALLOWED);
    }
    /****************************************/
    /*      Check parameters                */
    /****************************************/
    // Only the bits EDD_CSRT_DSTAT_BIT_STATE and/or EDD_CSRT_DSTAT_BIT_REDUNDANCY may be set
    if(Status & ~(EDD_CSRT_DSTAT_BIT_STATE | EDD_CSRT_DSTAT_BIT_REDUNDANCY))
    {
        EDDP_USER_TRACE_02(pIOCB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] eddp_SysRed_ProviderDataStatus_Set(): ERROR -> Status = 0x%X is invalid", InstanceHandle, Status);
        return EDD_STS_ERR_PARAM;
    }
    /* check the mask. This function may only set the State or Redundancy flag. */
    if( (Mask & ~(EDD_CSRT_DSTAT_BIT_STATE | EDD_CSRT_DSTAT_BIT_REDUNDANCY))  // mask the two bits, check if other bits are set
      )
    {
        EDDP_USER_TRACE_02(pIOCB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] eddp_SysRed_ProviderDataStatus_Set(): ERROR -> Mask = 0x%X is invalid", InstanceHandle, Mask);
        return EDD_STS_ERR_PARAM;
    }

    if (ApplProviderID >= EDDP_MAX_NR_APPL_PROVIDERS)
    {
        EDDP_USER_TRACE_03(pIOCB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] eddp_SysRed_ProviderDataStatus_Set(): ERROR -> ApplProvID:0x%X bigger then EDDP_MAX_NR_APPL_PROVIDERS:0x%X",
                           InstanceHandle, ApplProviderID, EDDP_MAX_NR_APPL_PROVIDERS);
        return EDD_STS_ERR_PARAM;
    }

    pProvider = &pIOCB->provider_list[ApplProviderID];

    if (pProvider->provider_id == EDDP_IO_UNKNOWN_PROV_ID)
    {
        EDDP_USER_TRACE_02(pIOCB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] eddp_SysRed_ProviderDataStatus_Set(): ERROR -> Unknown ProvID:0x%X", InstanceHandle, pProvider->provider_id);
        return EDD_STS_ERR_PARAM;
    }

    /****************************/
    /* Set DS                   */
    /* Read-Modify-Write access */
    /****************************/
    if(Mask) // only Read-Modify-Write to the RAM if the value has to be written
    {
        /****************************************/
        /*      Begin critcal section           */
        /****************************************/
        EDDP_ENTER_IO();  // lock memory

        // read the current value
        regVal = EDDP_USR_SYSRED_MEM8_READ((EDD_COMMON_MEM_PTR_TYPE)pProvider->pDataStatus);

        // clear the two bits
        regVal &= ~(Mask);
        // set the bits that need to be set
        regVal |=  (Status & Mask);

        // write to the hw
        EDDP_USR_SYSRED_MEM8_WRITE((EDD_COMMON_MEM_PTR_TYPE) pProvider->pDataStatus, regVal);

        EDDP_EXIT_IO();   // release memory lock
        /****************************************/
        /*      End critcal section             */
        /****************************************/
    }
    EDDP_USER_TRACE_01(pIOCB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] <-- eddp_SysRed_ProviderDataStatus_Set()", InstanceHandle);

    return EDD_STS_OK;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_SysRed_ShM_Array_Set                   +*/
/*+  Input/Output          :    EDDP_HANDLE             hDDB                +*/
/*+                             LSA_UINT16              ShMIndex            +*/
/*+                             LSA_UINT16*             ShMValue            +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+  hDDB         :   the instance handle.                   (in)           +*/
/*+  ShMIndex     :   shared memory index                    (in)           +*/
/*+                                Range:                                   +*/
/*+                                     0... EDD_CFG_MAX_SRSHM_SIZE-1       +*/
/*+  ShMValue     :   shared memory array element value       (in)          +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_EXCP                                          +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: set the value to ShMValue of the element with the index   +*/
/*                ShMIndex in the sr shared memory array                    +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP   eddp_SysRed_ShM_Array_Set( 
           LSA_UINT32    const   InstanceHandle,
           LSA_UINT16    const   ShMIndex,
           LSA_UINT16    const   ShMValue)
{    
    EDD_RSP        result = EDD_STS_OK;
    EDDP_IO_CB_TYPE  *pIOCB  = LSA_NULL;

    EDDP_IO_GET_IOCB(pIOCB, InstanceHandle);
    
    EDDP_USER_TRACE_03(pIOCB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] --> eddp_SysRed_ShM_Array_Set() -> ShMIndex:0x%X, ShMValue = 0x%X ",
    InstanceHandle,
    ShMIndex,
    ShMValue);
    
    if( ShMIndex >= EDD_CFG_MAX_SRSHM_SIZE )
    {
        result = EDD_STS_ERR_PARAM;
    }
    else if( (LSA_HOST_PTR_ARE_EQUAL(pIOCB,LSA_NULL)) || (LSA_HOST_PTR_ARE_EQUAL(pIOCB->pGSharedRAM,LSA_NULL)) )
    {
         result = EDD_STS_ERR_EXCP;
        
    }
    else
    {
         pIOCB->pGSharedRAM->SRSHM[ShMIndex] = H_TO_LE_S(ShMValue);
    }

    return result;
}

#endif //EDDP_CFG_USR_SYSRED_API_INCLUDE
/*****************************************************************************/
/*  end of file eddp_usr_sysred_api.c                                        */
/*****************************************************************************/
