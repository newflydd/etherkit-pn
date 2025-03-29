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
/*  F i l e               &F: eddp_hal_ram.c                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP Debugging support functions                 */
/*                          (system dependend)                               */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  20.10.08    SF    initial version.                                       */
/*  24.11.10    JS    changes to prevent compiler warnings                   */
#endif
/*****************************************************************************/



/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/

#define LTRC_ACT_MODUL_ID  41
#define EDDP_MODULE_ID     LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_HAL_RAM */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"               /* eddp headerfiles */
#include "eddp_int.h"               /* internal header  */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)

/*===========================================================================*/
/*                                global Variables                           */
/*===========================================================================*/


/*===========================================================================*/
/*                                  Types                                    */
/*===========================================================================*/


// local argument structure for function: _EDDP_HALInitSubComponents()
typedef struct _EDDP_HAL_SUB_COMP_ARG_TYPE  EDDP_LOCAL_MEM_ATTR *   EDDP_HAL_LOCAL_SUB_COMP_ARG_PTR_TYPE;
typedef struct _EDDP_HAL_SUB_COMP_ARG_TYPE
{
    EDD_COMMON_MEM_PTR_TYPE                 pCompRamBase;   // Base Address for RamComponent (Host view)
    LSA_UINT32                              CompRamSize;    // full length of RamComponent
    LSA_UINT32                              CompRamFree;    // for free calculation
    EDDP_HAL_LOCAL_RAM_SUB_COMP_PTR_TYPE    pSubArray;      // points to SubComponents entries
    LSA_UINT32                              Count;          // number of SubComponents
} EDDP_HAL_SUB_COMP_ARG_TYPE;



/*===========================================================================*/
/*                                Constants                                  */
/*===========================================================================*/



/*===========================================================================*/
/*                                 Macros                                    */
/*===========================================================================*/





/*===========================================================================*/
/*                        private functions                                  */
/*===========================================================================*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_HALCheckResetExecution                +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB          +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                (in)  +*/
/*+  RAMType       : selects specific memory range                    (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This utility function checks a signature in APICtrlRAM.   +*/
/*+               After reset there is no signature. If this function finds +*/
/*+               a valid signature, then reset was not run after DevClose()+*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_HALCheckResetExecution(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_COMMON_MEM_U32_PTR_TYPE     pResetChk;

    EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] _EDDP_HALCheckResetExecution()");
    
    switch(EDDP_HALGetPnipRevision(pDDB))
    {
#ifdef EDDP_CFG_HW_ERTEC200P_SUPPORT
        case EDDP_HAL_HW_PNIP_REV1:
        case EDDP_HAL_HW_PNIP_REV2:
            pResetChk = (EDD_COMMON_MEM_U32_PTR_TYPE) EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_API_CTRL, EDDP_HAL_RAMSUB_APICTRL_RESET_CHECK, LSA_NULL);
            break;
#endif
        default:
            EDDP_CORE_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] _EDDP_HALCheckResetExecution(): FATAL -> HW not supported !!!");
            EDDP_FATAL(EDDP_FATAL_ERR_DEV_SETUP);
            return (EDD_STS_ERR_PARAM);
    }

    

    if  (LSA_HOST_PTR_ARE_EQUAL(pResetChk, LSA_NULL))
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] _EDDP_HALCheckResetExecution(): ERROR -> pResetChk is NULL");
        return (EDD_STS_ERR_PARAM);
    }

    if (EDDP_HAL_IS_RESET_CHK_SET(pResetChk))
    {
        // ***** FATAL_ERROR: expect cleared RAM and we found asignature --> reset was not executed
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] _EDDP_HALCheckResetExecution(): FATAL -> no HW-Reset executed !!!");
        EDDP_FATAL(EDDP_FATAL_ERR_DEV_SETUP);
        return (EDD_STS_ERR_PARAM);
    }

    return (EDD_STS_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_HALGetCommonAccessPtr                 +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB          +*/
/*+                             LSA_UINT32                    RAMType       +*/
/*+                                                                         +*/
/*+  Result                :    EDDP_HAL_LOCAL_RAM_COMP_COMMON_PTR_TYPE     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                (in)  +*/
/*+  RAMType       : selects specific memory range                    (in)  +*/
/*+                                                                         +*/
/*+  Results       : EDDP_HAL_LOCAL_RAM_COMP_COMMON_PTR_TYPE                +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This utility function returns the selected RAMType as a   +*/
/*+               common structure pointer or LSA_NULL on error.            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDDP_HAL_LOCAL_RAM_COMP_COMMON_PTR_TYPE _EDDP_HALGetCommonAccessPtr(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  RAMType)
{
    EDDP_HAL_RAMLAYOUT_PTR_TYPE	                pHal;
    EDDP_HAL_LOCAL_RAM_COMP_COMMON_PTR_TYPE     pCommonPtr;

    pHal = &pDDB->Core.Hal.HalRamLayout;

    if ( (RAMType < EDDP_HAL_RAMTYPE_COMP_COUNT) && (pHal->pPnipRamBase) ) 
    {
        pCommonPtr = pHal->RamCommonArrayPtr[RAMType];
    }
    else
    {
        pCommonPtr = LSA_NULL;
    }

    return (pCommonPtr);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_HALGetRamBase                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB          +*/
/*+                             LSA_UINT32                    RAMType       +*/
/*+                             EDD_COMMON_MEM_U32_PTR_TYPE   pCompCount    +*/
/*+                                                                         +*/
/*+  Result                :    EDDP_HAL_LOCAL_RAM_SUB_COMP_PTR_TYPE        +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                (in)  +*/
/*+  RAMType       : selects specific memory range                    (in)  +*/
/*+  pCompCount    : returns number of subcomponents                  (i/o) +*/
/*+                                                                         +*/
/*+  Results       : EDDP_HAL_LOCAL_RAM_SUB_COMP_PTR_TYPE                   +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This utility function returns the RAM base address        +*/
/*+               selected by RAMType and the size in pRamSize (optional).  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_COMMON_MEM_PTR_TYPE _EDDP_HALGetRamBase(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_UINT32                      RAMType,
    EDD_COMMON_MEM_U32_PTR_TYPE     pRamSize)
{
    LSA_UINT32                      RamSize;
    EDDP_HAL_RAMLAYOUT_PTR_TYPE     pHal;
    EDD_COMMON_MEM_PTR_TYPE         pRamBase;

    pHal = &pDDB->Core.Hal.HalRamLayout;

    // only HERA uses EDDP_HAL_RAMTYPE_API_NR
    if ( (RAMType < EDDP_HAL_RAMTYPE_COMP_COUNT) && (pHal->pPnipRamBase) )
    {
        pRamBase = pHal->RamCommonArrayPtr[RAMType]->pComponentRamBase;
        RamSize  = pHal->RamCommonArrayPtr[RAMType]->ComponentRamSize;

    }
    else
    {
        pRamBase = LSA_NULL;
        RamSize  = 0;
    }

    if (pRamSize != LSA_NULL)
    {
        *pRamSize = RamSize;
    }

    return (pRamBase);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_HALGetCompArray                       +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB          +*/
/*+                             LSA_UINT32                    RAMType       +*/
/*+                             EDD_COMMON_MEM_U32_PTR_TYPE   pCompCount    +*/
/*+                                                                         +*/
/*+  Result                :    EDDP_HAL_LOCAL_RAM_SUB_COMP_PTR_TYPE        +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                (in)  +*/
/*+  RAMType       : selects specific memory range                    (in)  +*/
/*+  pCompCount    : returns number of subcomponents                  (i/o) +*/
/*+                                                                         +*/
/*+  Results       : EDDP_HAL_LOCAL_RAM_SUB_COMP_PTR_TYPE                   +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This utility function returns a pointer to the component  +*/
/*+               array selected by RAMType. On error LSA_NULL will be      +*/
/*+               returned. The count is returned in pCompCount (optional). +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDDP_HAL_LOCAL_RAM_SUB_COMP_PTR_TYPE _EDDP_HALGetCompArray(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_UINT32                      RAMType,
    EDD_COMMON_MEM_U32_PTR_TYPE     pCompCount)
{
    LSA_UINT32                              CompCount;
    EDDP_HAL_RAMLAYOUT_PTR_TYPE             pHal;
    EDDP_HAL_LOCAL_RAM_SUB_COMP_PTR_TYPE    pCompArray;

    pHal = &pDDB->Core.Hal.HalRamLayout;

    if ( (RAMType < EDDP_HAL_RAMTYPE_COMP_COUNT) && (pHal->pPnipRamBase) )
    {
        pCompArray = &pHal->RamCommonArrayPtr[RAMType]->ComponentRamArray[0];
        CompCount  = pHal->RamCommonArrayPtr[RAMType]->ComponentRamCount;
    }
    else
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] _EDDP_HALGetCompArray(): ERROR -> RAMType(%d) unknown", RAMType);
        pCompArray  = LSA_NULL;
        CompCount   = 0;
    }

    if (pCompCount != LSA_NULL)
    {
        *pCompCount = CompCount;
    }

    return (pCompArray);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_HALTraceCompLayout                    +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB          +*/
/*+                EDDP_HAL_LOCAL_RAM_COMP_COMMON_PTR_TYPE    pCompPtr      +*/
/*+                LSA_UINT32                                 RAMType       +*/
/*+                LSA_BOOL                                   PrintWrCheck  +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                (in)  +*/
/*+  pCompArray    : Pointer to component                             (in)  +*/
/*+  RAMType       : Type of pCompArray                               (in)  +*/
/*+  PrintWrCheck  : false:ignore, true:print wrcheck-value           (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This utility function is used to trace information for    +*/
/*+               selected component RAM (given by pCompArray).             +*/
/*+               On error LSA_NULL will be returned.                       +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_HALTraceCompLayout(
    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB,
    EDDP_HAL_LOCAL_RAM_COMP_COMMON_PTR_TYPE     pCompPtr,
    LSA_UINT32                                  RAMType,
    LSA_BOOL                                    PrintWrCheck)
{
    LSA_UINT32                              Idx, Len, HostAddr, PnipAddr, CompCount, UsedSize;
    EDDP_HAL_LOCAL_RAM_SUB_COMP_PTR_TYPE    pCompArray;

    EDDP_ASSERT_NO_NULL_PTR(pDDB);
    EDDP_ASSERT_NO_NULL_PTR(pCompPtr);

    pCompArray  = &pCompPtr->ComponentRamArray[0];
    CompCount   = pCompPtr->ComponentRamCount;
    UsedSize    = 0;
    PnipAddr    = EDDP_HAL_H2P_ADDR(pDDB, (LSA_UINT32)pCompArray->pSubComponentRamBase);

    // ***** NOTE: format specifier %s is not allowed with EDDP_XXX_TRACE_0?()
    switch (RAMType)
    {
        case EDDP_HAL_RAMTYPE_STATISTIC:
            EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] RamComponentStatistic, Count=%d, PNIP=0x%x, Size=%d", CompCount, PnipAddr, pCompPtr->ComponentRamSize);
            break;
        case EDDP_HAL_RAMTYPE_SYNC:
            EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] RamComponentSync, Count=%d, PNIP=0x%x, Size=%d", CompCount, PnipAddr, pCompPtr->ComponentRamSize);
            break;
        case EDDP_HAL_RAMTYPE_PACK_DATA:
            EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] RamComponentPackData, Count=%d, PNIP=0x%x, Size=%d", CompCount, PnipAddr, pCompPtr->ComponentRamSize);
            break;
        case EDDP_HAL_RAMTYPE_CMD:
            EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] RamComponentCmd, Count=%d, PNIP=0x%x, Size=%d", CompCount, PnipAddr, pCompPtr->ComponentRamSize);
            break;
        case EDDP_HAL_RAMTYPE_API_CTRL:
            EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] RamComponentApiCtrl, Count=%d, PNIP=0x%x, Size=%d", CompCount, PnipAddr, pCompPtr->ComponentRamSize);
            break;
        case EDDP_HAL_RAMTYPE_API_NRT_CTRL:
            EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] RamComponentApiNRTCtrl, Count=%d, PNIP=0x%x, Size=%d", CompCount, PnipAddr, pCompPtr->ComponentRamSize);
            break;
        case EDDP_HAL_RAMTYPE_FDB_TABLE:
            EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] RamComponentFdbTable, Count=%d, PNIP=0x%x, Size=%d", CompCount, PnipAddr, pCompPtr->ComponentRamSize);
            break;
        case EDDP_HAL_RAMTYPE_VLAN_TABLE:
            EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] RamComponentVlanTable, Count=%d, PNIP=0x%x, Size=%d", CompCount, PnipAddr, pCompPtr->ComponentRamSize);
            break;
        case EDDP_HAL_RAMTYPE_QUEUE_CTRL:
            EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] RamComponentQueueCtrl, Count=%d, PNIP=0x%x, Size=%d", CompCount, PnipAddr, pCompPtr->ComponentRamSize);
            break;
        case EDDP_HAL_RAMTYPE_QUEUE_DATA:
            EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] RamComponentQueueData, Count=%d, PNIP=0x%x, Size=%d", CompCount, PnipAddr, pCompPtr->ComponentRamSize);
            break;
        case EDDP_HAL_RAMTYPE_API_PST:
            EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] RamComponentApiPst, Count=%d, PNIP=0x%x, Size=%d", CompCount, PnipAddr, pCompPtr->ComponentRamSize);
            break;
        default:
            break;
    }

    // step through all subcomponents and print basic information (Host-Addr, PNIP-Addr, Length)
    for ( Idx=0; Idx < CompCount; Idx++ )
    {
        HostAddr    = (LSA_UINT32) pCompArray[Idx].pSubComponentRamBase;
        PnipAddr    = EDDP_HAL_H2P_ADDR(pDDB, HostAddr);
        Len         = pCompArray[Idx].SubComponentRamSize;
        UsedSize   += pCompArray[Idx].SubComponentUsedSize;

        if  (LSA_HOST_PTR_ARE_EQUAL(pCompArray[Idx].pSubComponentRamBase, LSA_NULL))
        {
            EDDP_CORE_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] Sub[%.2d]:  Host=NULL, PNIP=???, Len=%d, BorderChk=%d, BitAlignmt=%d", 
                Idx, Len, pCompArray[Idx].RamSubComponentUseBorderCheck, (32<<pCompArray[Idx].RamSubComponentBitAlignmt));
        }
        else
        {
            EDDP_CORE_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] Sub[%.2d]:  Host=0x%X, PNIP=0x%X, Len=%d, BitAlignmt=%d", 
                Idx, HostAddr, PnipAddr, Len, (32<<pCompArray[Idx].RamSubComponentBitAlignmt));

            if (pCompArray[Idx].RamSubComponentUseBorderCheck)
            {
                HostAddr = (LSA_UINT32) pCompArray[Idx].pSubComponentBorderCheck;
                PnipAddr = EDDP_HAL_H2P_ADDR(pDDB, HostAddr);

                if (PrintWrCheck)
                {
                    Len = EDDP_HAL_MEM32_READ(pDDB, pCompArray[Idx].pSubComponentBorderCheck);
                    EDDP_CORE_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] Sub[%.2d]:  HChk=0x%X, PChk=0x%X, Val=0x%X", 
                        Idx, HostAddr, PnipAddr, Len);
                }
                else
                {
                    EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] Sub[%.2d]:  HChk=0x%X, PChk=0x%X", 
                        Idx, HostAddr, PnipAddr);
                }
            }
        }
    }

    // ***** NOTE: format specifier %s is not allowed with EDDP_XXX_TRACE_0?()
    switch (RAMType)
    {
        case EDDP_HAL_RAMTYPE_STATISTIC:
            EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] RamComponentStatistic, used=%d, free=%d", UsedSize , pCompPtr->ComponentRamFreeSpace);
            break;
        case EDDP_HAL_RAMTYPE_SYNC:
            EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] RamComponentSync, used=%d, free=%d", UsedSize , pCompPtr->ComponentRamFreeSpace);
            break;
        case EDDP_HAL_RAMTYPE_PACK_DATA:
            EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] RamComponentPackData, used=%d, free=%d", UsedSize , pCompPtr->ComponentRamFreeSpace);
            break;
        case EDDP_HAL_RAMTYPE_CMD:
            EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] RamComponentCmd, used=%d, free=%d", UsedSize , pCompPtr->ComponentRamFreeSpace);
            break;
        case EDDP_HAL_RAMTYPE_API_CTRL:
            EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] RamComponentApiCtrl, used=%d, free=%d", UsedSize , pCompPtr->ComponentRamFreeSpace);
            break;
        case EDDP_HAL_RAMTYPE_API_NRT_CTRL:
            EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] RamComponentApiNRTCtrl, used=%d, free=%d", UsedSize , pCompPtr->ComponentRamFreeSpace);
            break;
        case EDDP_HAL_RAMTYPE_FDB_TABLE:
            EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] RamComponentFdbTable, used=%d, free=%d", UsedSize , pCompPtr->ComponentRamFreeSpace);
            break;
        case EDDP_HAL_RAMTYPE_VLAN_TABLE:
            EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] RamComponentVlanTable, used=%d, free=%d", UsedSize , pCompPtr->ComponentRamFreeSpace);
            break;
        case EDDP_HAL_RAMTYPE_QUEUE_CTRL:
            EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] RamComponentQueueCtrl, used=%d, free=%d", UsedSize , pCompPtr->ComponentRamFreeSpace);
            break;
        case EDDP_HAL_RAMTYPE_QUEUE_DATA:
            EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] RamComponentQueueData, used=%d, free=%d", UsedSize , pCompPtr->ComponentRamFreeSpace);
            break;
        case EDDP_HAL_RAMTYPE_API_PST:
            EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] RamComponentApi, used=%d, free=%d", UsedSize , pCompPtr->ComponentRamFreeSpace);
            break;
        default:
            break;
    }

    return (EDD_STS_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_HALInitSubComponents                  +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB          +*/
/*+                   EDDP_HAL_LOCAL_SUB_COMP_ARG_PTR_TYPE    pSubCompArgs  +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                (in)  +*/
/*+  pSubCompArgs  : Pointer to input parameter                       (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This utility function is used to assign subcomponent into +*/
/*+               address space of the RAM component.                       +*/
/*+  Note: Used size of subcomponent must be set before call this function! +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_HALInitSubComponents(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_HAL_LOCAL_SUB_COMP_ARG_PTR_TYPE    pSubCompArgs)
{
    LSA_UINT32                              HostAddr, PnipAddr;
    LSA_UINT32                              RamLen, PosDiff, Idx;
    EDDP_HAL_LOCAL_RAM_SUB_COMP_PTR_TYPE    pArrayItem;

    EDDP_ASSERT_NO_NULL_PTR(pDDB);
    EDDP_ASSERT_NO_NULL_PTR(pSubCompArgs);

    pSubCompArgs->CompRamFree   = pSubCompArgs->CompRamSize;
    HostAddr                    = (LSA_UINT32)pSubCompArgs->pCompRamBase;

    // step through all parts and try to insert these into the component RAM
    for ( Idx=0; Idx<pSubCompArgs->Count; Idx++ )
    {
        pArrayItem  = &pSubCompArgs->pSubArray[Idx];    // pointer to definition of component part
        RamLen      = pArrayItem->SubComponentRamSize;  // and its size

        pArrayItem->SubComponentUsedSize        = 0;    // reset used RAM space
        pArrayItem->pSubComponentRamBase        = LSA_NULL;
        pArrayItem->pSubComponentBorderCheck    = LSA_NULL;

        if (RamLen)
        {
            // check alignment on PNIP view and take over the offset into Host view
            PnipAddr = EDDP_HAL_H2P_ADDR(pDDB, HostAddr);

            switch (pArrayItem->RamSubComponentBitAlignmt) // do right alignment
            {
                case EDDP_ALIGMNT_32BIT:
                    PosDiff = (EDDP_HALTrimAlignment(PnipAddr, EDDP_HAL_32BIT_ALIGNMENT) - PnipAddr);
                    break;
                case EDDP_ALIGMNT_64BIT:
                    PosDiff = (EDDP_HALTrimAlignment(PnipAddr, EDDP_HAL_64BIT_ALIGNMENT) - PnipAddr);
                    break;
                case EDDP_ALIGMNT_128BIT:
                    PosDiff = (EDDP_HALTrimAlignment(PnipAddr, EDDP_HAL_128BIT_ALIGNMENT) - PnipAddr);
                    break;
                case EDDP_ALIGMNT_256BIT:
                    PosDiff = (EDDP_HALTrimAlignment(PnipAddr, EDDP_HAL_256BIT_ALIGNMENT) - PnipAddr);
                    break;
                case EDDP_ALIGMNT_512BIT:
                    PosDiff = (EDDP_HALTrimAlignment(PnipAddr, EDDP_HAL_512BIT_ALIGNMENT) - PnipAddr);
                    break;
                case EDDP_ALIGMNT_1024BIT:
                    PosDiff = (EDDP_HALTrimAlignment(PnipAddr, EDDP_HAL_1024BIT_ALIGNMENT) - PnipAddr);
                    break;
                default:
                {
                    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] _EDDP_HALInitSubComponents(): ERROR ->  unknown enum alignment RamSubComponentBitAlignmt(%d)", 
                        pArrayItem->RamSubComponentBitAlignmt);
                    return (EDD_STS_ERR_RESOURCE);  // overflow in component RAM
                }
                break;
            }

            HostAddr += PosDiff;                            // set right alignment
            PosDiff  += pArrayItem->SubComponentRamSize;    // add size for this component part

            if (pSubCompArgs->CompRamFree >= PosDiff)       // test if space left
            {
                pArrayItem->pSubComponentRamBase = (EDD_COMMON_MEM_PTR_TYPE)HostAddr;   // ok
            }
            else
            {
                EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] _EDDP_HALInitSubComponents(): ERROR -> RAM overflow HostAddr(0x%X)",
                    HostAddr);
                return (EDD_STS_ERR_RESOURCE);                                          // overflow in component RAM
            }

            HostAddr                            += pArrayItem->SubComponentRamSize;     // add size for this component part
            pSubCompArgs->CompRamFree           -= PosDiff;                             // decrement space needed by this component part
            pArrayItem->SubComponentUsedSize    += PosDiff;                             // count used RAM space (incl. BorderCheck, Alignemt padding)

            // append 'border check cell' if enabled
            if (pArrayItem->RamSubComponentUseBorderCheck)
            {
                PnipAddr                                = EDDP_HAL_H2P_ADDR(pDDB, HostAddr);
                PosDiff                                 = (EDDP_HALTrimAlignment(PnipAddr, EDDP_HAL_32BIT_ALIGNMENT) - PnipAddr);
                pArrayItem->pSubComponentBorderCheck    = (EDD_COMMON_MEM_U32_PTR_TYPE)(HostAddr+PosDiff); // store aligned address

                PosDiff += sizeof(EDD_COMMON_MEM_U32_PTR_TYPE); // add size for 'border check cell'

                if (pSubCompArgs->CompRamFree < PosDiff)    // test if space left
                {
                    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] _EDDP_HALInitSubComponents(): ERROR -> RAM overflow HostAddr(0x%X)",
                        HostAddr);
                    return (EDD_STS_ERR_RESOURCE);  // overflow in component RAM
                }

                //*pArrayItem->pSubComponentBorderCheck = EDDP_HAL_BORDER_TOKEN;        // set special test pattern later in Setup()
                HostAddr += PosDiff;                                                    // add used space for 'border check cell' (+ alignment space)
                pSubCompArgs->CompRamFree -= PosDiff;                                   // decrement space needed by 'border check cell'
                pArrayItem->SubComponentUsedSize += PosDiff;                            // count used RAM space
            }
        }
        else
        {
            if (pArrayItem->RamSubComponentUseBorderCheck)
            {
                EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] _EDDP_HALInitSubComponents(): ERROR -> Parameter missmatch: HostAddr(0x%X), RamSubComponentUseBorderCheck(0x%X)", 
                    HostAddr, pArrayItem->RamSubComponentUseBorderCheck);
                return (EDD_STS_ERR_PARAM); // parameter missmatch (RAMsize=0 with BoarderCheck=on!!!)
            }
        }
    }

    return (EDD_STS_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_HALSetupWriteBorders                  +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB          +*/
/*+                EDDP_HAL_LOCAL_RAM_SUB_COMP_PTR_TYPE       pCompArray    +*/
/*+                LSA_UINT32                                 CompCount     +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                (in)  +*/
/*+  pCompArray    : Pointer to subcomponents                         (in)  +*/
/*+  CompCount     : Number of subcomponents in pCompArray            (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This utility function is used to setup special values in  +*/
/*+               given RAM component for writecheck.                       +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_HALSetupWriteBorders(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_HAL_LOCAL_RAM_SUB_COMP_PTR_TYPE    pCompArray,
    LSA_UINT32                              CompCount
    )
{
    LSA_UINT32      Idx;

#if !defined EDDP_CFG_DEBUG_ASSERT
    LSA_UNUSED_ARG(pDDB);
#endif

    EDDP_ASSERT_NO_NULL_PTR(pDDB);
    EDDP_ASSERT_NO_NULL_PTR(pCompArray);

    // ***** step through all subcomponents and write bordercheck value if enabled
    for ( Idx=0; Idx < CompCount; Idx++ )
    {
        if  (LSA_HOST_PTR_ARE_EQUAL(pCompArray[Idx].pSubComponentRamBase, LSA_NULL))
        {
            // ***** RAM not reserved or RAM layout error --> unreserved RAM has zero length
            if (pCompArray[Idx].SubComponentRamSize)
            {
                // ***** error because no zero length !!!
                EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] _EDDP_HALSetupWriteBorders(): ERROR -> pCompArray[%d].pSubComponentRamBase is NULL",
                    Idx);
                return (EDD_STS_ERR_PARAM);
            }
        }

        if (pCompArray[Idx].RamSubComponentUseBorderCheck)
        {
            EDDP_HAL_MEM32_WRITE(pDDB, pCompArray[Idx].pSubComponentBorderCheck, EDDP_HAL_BORDER_TOKEN);    // set special test pattern (border)
        }
    }

    return (EDD_STS_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_HALSetupRamBorders                    +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB          +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This utility function is used to setup all writeborder    +*/
/*+               checkers in all RAM components.                           +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_HALSetupRamBorders(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDDP_HAL_RAMLAYOUT_PTR_TYPE     pHal;
    LSA_UINT32                      Idx;

    EDDP_ASSERT_NO_NULL_PTR(pDDB);

    EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: _EDDP_HALSetupRamBorders()");

    pHal = &pDDB->Core.Hal.HalRamLayout;

    if (LSA_HOST_PTR_ARE_EQUAL(pHal->pPnipRamBase, LSA_NULL))
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] _EDDP_HALSetupRamBorders(): ERROR -> pPnipRamBase is NULL");
        return (EDD_STS_ERR_PARAM);
    }

    for ( Idx=0; Idx<EDDP_HAL_RAMTYPE_COMP_COUNT; Idx++ )
    {
        // only HERA uses EDDP_HAL_RAMTYPE_API_NRT_CTRL
        if(  !( (EDDP_HAL_HW_PNIP_REV3 != EDDP_HALGetPnipRevision(pDDB)) && (EDDP_HAL_RAMTYPE_API_NRT_CTRL == Idx) ) 
          && !( (EDDP_HAL_HW_PNIP_REV3 != EDDP_HALGetPnipRevision(pDDB)) && (EDDP_HAL_RAMTYPE_API_PST == Idx) )
          )
        {
            _EDDP_HALSetupWriteBorders (pDDB, pHal->RamCommonArrayPtr[Idx]->ComponentRamArray, pHal->RamCommonArrayPtr[Idx]->ComponentRamCount);
        }
    }

    pHal->SetupDone = LSA_TRUE;

    EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: _EDDP_HALSetupRamBorders()");

    return (EDD_STS_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_HALSetSubCompArgs                     +*/
/*+  Input/Output          :    EDDP_HAL_LOCAL_RAM_SUB_COMP_PTR_TYPE pComp  +*/
/*+                             LSA_BOOL                    Use64BitAligned +*/
/*+                             LSA_UINT32                  SubRamLength    +*/
/*+                             LSA_BOOL                    UseBorderCheck  +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pComp         : Pointer to ComponentRamArray[?] (argument block) (in)  +*/
/*+  Use64BitAligned: false: 32bit, true: 64bit aligned               (in)  +*/
/*+  SubRamLength  : Lenght for this SunComponent RAM                 (in)  +*/
/*+  UseBorderCheck: false: disabled, true: enabled border check      (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This utility function is used to setup the arguments for  +*/
/*+               given subcomponent. If interface does not exist the RAM   +*/
/*+               usage and border check will be set to zero/disabled.      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR _EDDP_HALSetSubCompArgs(
    EDDP_HAL_LOCAL_RAM_SUB_COMP_PTR_TYPE    pComp,
    EDDP_HAL_ARG_ALIGMNT_TYPE               BitAlignmt,
    LSA_UINT32                              SubRamLength,
    LSA_BOOL                                UseBorderCheck)
{
    pComp->RamSubComponentBitAlignmt        = BitAlignmt;
    pComp->SubComponentRamSize              = SubRamLength;
    pComp->RamSubComponentUseBorderCheck    = UseBorderCheck;

    if (!SubRamLength)
    {   // if interface does not exist set RAM usage to zero!!!
        pComp->SubComponentRamSize = 0;
        pComp->RamSubComponentUseBorderCheck = LSA_FALSE;
    }
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_HALInitQueueDataRam                   +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB          +*/
/*+                             EDDP_UPPER_DPB_PTR_TYPE       pDPB          +*/
/*+                             EDD_RSP                       RetCode       +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                (in)  +*/
/*+  pDPB       : Pointer to DeviceParameterBlock                     (in)  +*/
/*+  RetCode       : Status of previos initialization                 (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This utility function is used to do intitialization of    +*/
/*+               its subcomponent if input EDD_STS_OK.                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_HALInitQueueDataRam(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DPB_PTR_TYPE     pDPB,
    EDD_RSP                     RetCode)
{
    // default initialization with full RAM reservation
    // --> overwrite this default implementation to support different subcomponents within the component ram
    EDDP_HAL_RAMLAYOUT_PTR_TYPE	            pHal;
    EDDP_HAL_LOCAL_RAM_SUB_COMP_PTR_TYPE    pComp;
    EDDP_HAL_SUB_COMP_ARG_TYPE              SubCompArgs;

    if (RetCode == EDD_STS_OK)
    {
        pHal    = &pDDB->Core.Hal.HalRamLayout;
        pComp   = &pHal->RamComponentQueueData.ComponentRamArray[0];
        //                      pComp , BitAlignmt,       SubRamLength,                       , UseBorderCheck
        _EDDP_HALSetSubCompArgs(pComp, EDDP_ALIGMNT_32BIT, pHal->RamComponentQueueData.ComponentRamSize, LSA_FALSE);

        // prepare arguments to call commmen utility function for subcomponent initialization
        SubCompArgs.pCompRamBase    = pHal->RamComponentQueueData.pComponentRamBase;
        SubCompArgs.CompRamSize     = pHal->RamComponentQueueData.ComponentRamSize;
        SubCompArgs.CompRamFree     = 0;
        SubCompArgs.pSubArray       = pComp;
        SubCompArgs.Count           = 1;
        RetCode                     = _EDDP_HALInitSubComponents(pDDB, &SubCompArgs);
        pHal->RamComponentQueueData.ComponentRamFreeSpace = SubCompArgs.CompRamFree;
    }

    LSA_UNUSED_ARG(pDPB);    // handle unused parameter

    return (RetCode);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_HALInitQueueCtrlRam                   +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB          +*/
/*+                             EDDP_UPPER_DPB_PTR_TYPE       pDPB          +*/
/*+                             EDD_RSP                       RetCode       +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  pDPB       : Pointer to DeviceParameterBlock                     (in)  +*/
/*+  RetCode    : Status of previos initialization                    (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This utility function is used to do intitialization of    +*/
/*+               its subcomponent if input EDD_STS_OK.                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_HALInitQueueCtrlRam(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DPB_PTR_TYPE     pDPB,
    EDD_RSP                     RetCode)
{
    // default initialization with full RAM reservation
    // --> overwrite this default implementation to support different subcomponents within the component ram
    EDDP_HAL_RAMLAYOUT_PTR_TYPE	pHal;
    EDDP_HAL_LOCAL_RAM_SUB_COMP_PTR_TYPE pComp;
    EDDP_HAL_SUB_COMP_ARG_TYPE SubCompArgs;

    if (RetCode == EDD_STS_OK)
    {
        pHal    = &pDDB->Core.Hal.HalRamLayout;
        pComp   = &pHal->RamComponentQueueCtrl.ComponentRamArray[0];
        //                      pComp , BitAlignmt,       SubRamLength,                       , UseBorderCheck
        _EDDP_HALSetSubCompArgs(pComp, EDDP_ALIGMNT_32BIT, pHal->RamComponentQueueCtrl.ComponentRamSize, LSA_FALSE);

        // ***** prepare arguments to call commmen utility function for subcomponent initialization
        SubCompArgs.pCompRamBase    = pHal->RamComponentQueueCtrl.pComponentRamBase;
        SubCompArgs.CompRamSize     = pHal->RamComponentQueueCtrl.ComponentRamSize;
        SubCompArgs.CompRamFree     = 0;
        SubCompArgs.pSubArray       = pComp;
        SubCompArgs.Count           = 1;
        RetCode                     = _EDDP_HALInitSubComponents(pDDB, &SubCompArgs);
        pHal->RamComponentQueueCtrl.ComponentRamFreeSpace = SubCompArgs.CompRamFree;
    }

    LSA_UNUSED_ARG(pDPB);    // handle unused parameter

    return (RetCode);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_HALInitVlanTableRam                   +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB          +*/
/*+                             EDDP_UPPER_DPB_PTR_TYPE       pDPB          +*/
/*+                             EDD_RSP                       RetCode       +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  pDPB       : Pointer to DeviceParameterBlock                     (in)  +*/
/*+  RetCode    : Status of previos initialization                    (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This utility function is used to do intitialization of    +*/
/*+               its subcomponent if input EDD_STS_OK.                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_HALInitVlanTableRam(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DPB_PTR_TYPE     pDPB,
    EDD_RSP                     RetCode)
{
    // ***** default initialization with full RAM reservation
    //       --> overwrite this default implementation to support different subcomponents within the component ram
    EDDP_HAL_RAMLAYOUT_PTR_TYPE	pHal;
    EDDP_HAL_LOCAL_RAM_SUB_COMP_PTR_TYPE pComp;
    EDDP_HAL_SUB_COMP_ARG_TYPE SubCompArgs;

    if (RetCode == EDD_STS_OK)
    {
        pHal    = &pDDB->Core.Hal.HalRamLayout;
        pComp   = &pHal->RamComponentVlanTable.ComponentRamArray[0];
        //                      pComp , BitAlignmt,       SubRamLength,                       , UseBorderCheck
        _EDDP_HALSetSubCompArgs(pComp, EDDP_ALIGMNT_32BIT, pHal->RamComponentVlanTable.ComponentRamSize, LSA_FALSE);

        // ***** prepare arguments to call commmen utility function for subcomponent initialization
        SubCompArgs.pCompRamBase    = pHal->RamComponentVlanTable.pComponentRamBase;
        SubCompArgs.CompRamSize     = pHal->RamComponentVlanTable.ComponentRamSize;
        SubCompArgs.CompRamFree     = 0;
        SubCompArgs.pSubArray       = pComp;
        SubCompArgs.Count           = 1;
        RetCode                     = _EDDP_HALInitSubComponents(pDDB, &SubCompArgs);
        pHal->RamComponentVlanTable.ComponentRamFreeSpace = SubCompArgs.CompRamFree;
    }

    LSA_UNUSED_ARG(pDPB);    // handle unused parameter

    return (RetCode);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_HALInitFdbTableRam                    +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB          +*/
/*+                             EDDP_UPPER_DPB_PTR_TYPE       pDPB          +*/
/*+                             EDD_RSP                       RetCode       +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                (in)  +*/
/*+  pDPB          : Pointer to DeviceParameterBlock                  (in)  +*/
/*+  RetCode       : Status of previos initialization                 (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This utility function is used to do intitialization of    +*/
/*+               its subcomponent if input EDD_STS_OK.                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_HALInitFdbTableRam(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DPB_PTR_TYPE     pDPB,
    EDD_RSP                     RetCode)
{
    EDDP_HAL_RAMLAYOUT_PTR_TYPE	pHal;
    EDDP_HAL_LOCAL_RAM_SUB_COMP_PTR_TYPE pComp;
    EDDP_HAL_SUB_COMP_ARG_TYPE SubCompArgs;

    if (RetCode == EDD_STS_OK)
    {
        pHal    = &pDDB->Core.Hal.HalRamLayout;
        pComp   = &pHal->RamComponentFdbTable.ComponentRamArray[0];

        //                      pComp                                         , BitAlignmt          , SubRamLength                                      , UseBorderCheck
        // MC_TABLE_A is the master table; MC_TABLE_B is the shadow table. Their meanings are swapped when the MC_Tables getting changed during runtime.
        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_FDB_TABLE_MC_TABLE_A]   , EDDP_ALIGMNT_128BIT , sizeof(EDDP_HAL_RAMSUB_FDB_TABLE_MC_TABLE_A_TYPE)  , LSA_TRUE);
        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_FDB_TABLE_MC_TABLE_B]   , EDDP_ALIGMNT_128BIT , sizeof(EDDP_HAL_RAMSUB_FDB_TABLE_MC_TABLE_B_TYPE)  , LSA_TRUE);
        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_FDB_TABLE_IRT_TABLE_A]  , EDDP_ALIGMNT_32BIT  , sizeof(EDDP_HAL_RAMSUB_FDB_TABLE_IRT_TABLE_A_TYPE) , LSA_TRUE);


        switch(EDDP_HALGetPnipRevision(pDDB))
        {
            #if( (defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT)) || (defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)) )
            case EDDP_HAL_HW_PNIP_REV1:
            case EDDP_HAL_HW_PNIP_REV2:
            {
                //                      pComp                                         , BitAlignmt          , SubRamLength                                      , UseBorderCheck
                _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_FDB_TABLE_PARAM_STRUCT] , EDDP_ALIGMNT_64BIT  , sizeof(EDDP_HAL_RAMSUB_FDB_TABLE_PARAM_STRUCT_ERTEC200P_TYPE), LSA_TRUE);
                _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_FDB_TABLE_UC_TABLE_A]   , EDDP_ALIGMNT_1024BIT, sizeof(EDDP_HAL_RAMSUB_FDB_TABLE_UC_TABLE_A_ERTEC200P_TYPE)  , LSA_TRUE);
                break;
            } // case EDDP_HAL_HW_PNIP_REV1/_REV2
            #endif // #if( (defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT)) || (defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)) )

            default:
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] _EDDP_HALInitFdbTableRam(): FATAL -> Revision(%d) of PNIP is unknown", 
                                    EDDP_HALGetPnipRevision(pDDB));
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                break;
            }
        } // switch(EDDP_HALGetPnipRevision(pDDB))

        // ***** prepare arguments to call commmen utility function for subcomponent initialization
        SubCompArgs.pCompRamBase    = pHal->RamComponentFdbTable.pComponentRamBase;
        SubCompArgs.CompRamSize     = pHal->RamComponentFdbTable.ComponentRamSize;
        SubCompArgs.CompRamFree     = 0;
        SubCompArgs.pSubArray       = pComp;
        SubCompArgs.Count           = EDDP_HAL_RAMSUB_FDB_TABLE_COUNT;
        RetCode                     = _EDDP_HALInitSubComponents(pDDB, &SubCompArgs);
        pHal->RamComponentFdbTable.ComponentRamFreeSpace = SubCompArgs.CompRamFree;
    }

    LSA_UNUSED_ARG(pDPB);    // handle unused parameter

    return (RetCode);
}


#ifdef EDDP_CFG_HW_ERTEC200P_SUPPORT
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_HALInitApiCtrlRam                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB          +*/
/*+                             EDDP_UPPER_DPB_PTR_TYPE       pDPB          +*/
/*+                             EDD_RSP                       RetCode       +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                (in)  +*/
/*+  pDPB          : Pointer to DeviceParameterBlock                  (in)  +*/
/*+  RetCode       : Status of previos initialization                 (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This utility function is used to do intitialization of    +*/
/*+               its subcomponent if input EDD_STS_OK.                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_HALInitApiCtrlRam(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DPB_PTR_TYPE     pDPB,
    EDD_RSP                     RetCode)
{
    LSA_UINT32                                  SubCnt = 29;     // do not use define "EDDP_HAL_RAMSUB_API_CTRL_COUNT" to see if define itself is incompatible
    LSA_UINT32                                  AllocSizeDcp, AllocSizeDcpHello;
    EDDP_HAL_SUB_COMP_ARG_TYPE                  SubCompArgs;
    EDDP_HAL_RAMLAYOUT_PTR_TYPE	                pHal;
    EDDP_HAL_LOCAL_RAM_SUB_COMP_PTR_TYPE        pComp;

    LSA_UNUSED_ARG(pDPB);   // handle unused parameter

    if (RetCode == EDD_STS_OK)
    {
        pHal = &pDDB->Core.Hal.HalRamLayout;

        if (pHal->RamComponentApiCtrl.ComponentRamCount != SubCnt)    // --> EDDP_HAL_RAMSUB_API_CTRL_COUNT
        {
            EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] _EDDP_HALInitApiCtrlRam(): ERROR -> ComponentRamCount(%d) != SubCnt(%d)", 
                pHal->RamComponentApiCtrl.ComponentRamCount, SubCnt);
            RetCode = EDD_STS_ERR_PARAM;
        }
        else
        {
            //EDDP_MEMSET_COMMON(&pHal->RamComponentApiCtrl.ComponentRamArray[0], 0, sizeof(EDDP_HAL_RAM_SUB_COMP_TYPE)*EDDP_HAL_RAMSUB_API_CTRL_COUNT);
            pComp = &pHal->RamComponentApiCtrl.ComponentRamArray[0];

            _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_APICTRL_RESET_CHECK],           EDDP_ALIGMNT_32BIT, EDDP_HAL_RESET_CHK_SIZE, LSA_TRUE);

            // ***** set defaults to invalid
            pDDB->CRT.CfgHwAllocParamApi.MaxHWPpmAcw            = 0;
            pDDB->CRT.CfgHwAllocParamApi.MaxHWPpmAcwDG          = 0;
            pDDB->CRT.CfgHwAllocParamApi.MaxHWPpmTrog           = 0;
            pDDB->CRT.CfgHwAllocParamApi.MaxHWPpmDataStatus     = 0;
            pDDB->CRT.CfgHwAllocParamApi.HWPpmAcwSize           = 0;
            pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmAcw            = 0;
            pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmAcwContainer   = 0;
            pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmAcwDG          = 0;
            pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmRBaseEntries   = 0;
            pDDB->CRT.CfgHwAllocParamApi.HWCpmAcwSize           = 0;
            pDDB->CRT.CfgHwAllocParamApi.HWCpmAcwDgSize         = 0;

            switch (pDDB->Hardware)
            {
                case EDDP_HW_ERTEC200P:
                case EDDP_HW_ERTEC200P_FPGA:   
                {
                    /* -------------------------------------------------------------------- */
                    /* ***** NRT: set specific initialization values --> 1..7               */
                    /* -------------------------------------------------------------------- */
                    //                      pComp ,                                                    BitAlignmt,         SubRamLength,                             UseBorderCheck
                    _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_APICTRL_NRT_COMMON_CODE],           EDDP_ALIGMNT_64BIT, sizeof(EDDP_HAL_APICTRL_NRT_COMMON_TYPE), LSA_TRUE);
                    _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_APICTRL_NRT_IFA_MGM]    ,           EDDP_ALIGMNT_64BIT, sizeof(EDDP_HAL_APICTRL_NRT_IF_MGM_TYPE), LSA_TRUE);
                    AllocSizeDcp = 1 + pDDB->SWI.PortCnt;                           // IF + ? ports
                    AllocSizeDcp *= sizeof(EDDP_HAL_APICTRL_NRT_IF_DCP_FILTER_TYPE);
                    _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_APICTRL_NRT_IFA_DCPPORT_FILTER],    EDDP_ALIGMNT_64BIT, AllocSizeDcp,                             LSA_TRUE);
                    AllocSizeDcpHello = 3 * sizeof(EDDP_HAL_APICTRL_NRT_IF_DCPHELLO_FILTER_TYPE);   // only for test
                    _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_APICTRL_NRT_IFA_DCPHELLO_FILTER],   EDDP_ALIGMNT_64BIT, AllocSizeDcpHello,                        LSA_TRUE);

                    /* -------------------------------------------------------------------- */
                    /* ***** CRT: set specific initialization values --> 8..25              */
                    /* -------------------------------------------------------------------- */
                    pDDB->CRT.CfgHwAllocParamApi.MaxHWPpmAcw            = EDDP_CRT_MAX_PROVIDER_ACW_ERTEC200P;
                    pDDB->CRT.CfgHwAllocParamApi.MaxHWPpmAcwDG          = EDDP_CRT_MAX_PROVIDER_ACW_DG_ERTEC200P;
                    pDDB->CRT.CfgHwAllocParamApi.MaxHWPpmTrog           = EDDP_CRT_MAX_PROVIDER_TROG_ERTEC200P;
                    pDDB->CRT.CfgHwAllocParamApi.MaxHWPpmDataStatus     = EDDP_CRT_MAX_PROVIDER_DATASTATUS_ERTEC200P;
                    pDDB->CRT.CfgHwAllocParamApi.HWPpmAcwSize           = sizeof(EDDP_HAL_APICTRL_CRT_PROV_ACW);
                    pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmAcw            = EDDP_CRT_MAX_CONSUMER_ACW_ERTEC200P;
                    pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmAcwContainer   = EDDP_CRT_MAX_CONSUMER_ACW_CONTAINER_ERTEC200P;
                    pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmAllAcw         = pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmAcw + pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmAcwContainer;
                    pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmAcwDG          = EDDP_CRT_MAX_CONSUMER_ACW_DG_ERTEC200P;
                    pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmRBaseEntries   = EDDP_CRT_MAX_COUNT_RBASE;
                    pDDB->CRT.CfgHwAllocParamApi.HWCpmAcwSize           = sizeof(EDDP_HAL_APICTRL_CRT_CONS_ACW);
                    pDDB->CRT.CfgHwAllocParamApi.HWCpmAcwDgSize         = sizeof(EDDP_HAL_APICTRL_CRT_CONS_ACW_DG);
                    // DDB
                    //                      pComp ,                                                BitAlignmt,         SubRamLength,                                                                               UseBorderCheck
                    _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_PROV_ACW]   ,   EDDP_ALIGMNT_64BIT, (pDDB->CRT.CfgHwAllocParamApi.MaxHWPpmAcw * sizeof(EDDP_HAL_APICTRL_CRT_PROV_ACW))          ,   LSA_TRUE);
                    _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_PROV_ACW_DG],   EDDP_ALIGMNT_64BIT, (pDDB->CRT.CfgHwAllocParamApi.MaxHWPpmAcwDG * sizeof(EDDP_HAL_APICTRL_CRT_PROV_ACW_DG))     ,   LSA_TRUE);
                    _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_CONS_ACW]   ,   EDDP_ALIGMNT_64BIT, ((pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmAllAcw) * sizeof(EDDP_HAL_APICTRL_CRT_CONS_ACW))     ,   LSA_TRUE);
                    _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_CONS_APDU]  ,   EDDP_ALIGMNT_32BIT, ((pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmAllAcw) * sizeof(EDDP_HAL_APICTRL_CRT_CONS_APDU))    ,   LSA_TRUE);
                    _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_CONS_ACW_DG],   EDDP_ALIGMNT_64BIT, (pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmAcwDG * sizeof(EDDP_HAL_APICTRL_CRT_CONS_ACW_DG))     ,   LSA_TRUE);
                    _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_CONS_DG_STATUS],EDDP_ALIGMNT_32BIT, (pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmAcwDG * sizeof(EDDP_HAL_APICTRL_CRT_CONS_DG_STATUS))  ,   LSA_TRUE);
                    // IFA
                    _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_TBASE_LIST] ,   EDDP_ALIGMNT_64BIT, sizeof(EDDP_HAL_APICTRL_CRT_TBASE_ARRAY)                                                    ,   LSA_TRUE);
                    _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_TROG_LIST]  ,   EDDP_ALIGMNT_64BIT, (pDDB->CRT.CfgHwAllocParamApi.MaxHWPpmTrog * sizeof(EDDP_HAL_APICTRL_CRT_TROG_ARRAY))       ,   LSA_TRUE);

                    // switch to current PNIP revision
                    switch (EDDP_HALGetPnipRevision(pDDB))
                    {
                        #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
                        case EDDP_HAL_HW_PNIP_REV1:
                        {
                            _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_DS_LIST]    ,   EDDP_ALIGMNT_64BIT, (pDDB->CRT.CfgHwAllocParamApi.MaxHWPpmDataStatus * sizeof(EDDP_HAL_PNIP_R1_APICTRL_CRT_DS_ARRAY))   ,   LSA_TRUE);
                        }
                        break;
                        #endif

                        #ifdef EDDP_CFG_HW_PNIP_REV2_SUPPORT
                        case EDDP_HAL_HW_PNIP_REV2:
                        {
                            _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_DS_LIST]    ,   EDDP_ALIGMNT_64BIT, (pDDB->CRT.CfgHwAllocParamApi.MaxHWPpmDataStatus * sizeof(EDDP_HAL_PNIP_R2_APICTRL_CRT_DS_ARRAY))   ,   LSA_TRUE);
                        }
                        break;
                        #endif
                        
                        default:
                        {
                            EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] _EDDP_HALInitApiCtrlRam(): FATAL -> Version(%d) of PNIP is unknown", 
                                EDDP_HALGetPnipRevision(pDDB));
                            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                        }
                        break;
                    }

                    _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_RBASE_LIST] ,   EDDP_ALIGMNT_64BIT, sizeof(EDDP_HAL_APICTRL_CRT_RBASE_ARRAY)                                                    ,   LSA_TRUE);
                    _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_CONS_TSB]   ,   EDDP_ALIGMNT_64BIT, (pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmAcw * sizeof(EDDP_HAL_APICTRL_CRT_CONS_TSB))          ,   LSA_TRUE);
                    _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_CONS_CSB0]  ,   EDDP_ALIGMNT_64BIT, (pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmAcw * sizeof(EDDP_HAL_APICTRL_CRT_CONS_CSB))          ,   LSA_TRUE);
                    _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_CONS_CSB1]  ,   EDDP_ALIGMNT_64BIT, (pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmAcw * sizeof(EDDP_HAL_APICTRL_CRT_CONS_CSB))          ,   LSA_TRUE);

                    /* -------------------------------------------------------------------- */
                    /* ***** CMD: parameter block for command interface                     */
                    /* -------------------------------------------------------------------- */
                    _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_APICTRL_COMMON_CMD_ARGS]    ,   EDDP_ALIGMNT_64BIT, sizeof(EDDP_HAL_API_RT_CTRL_COMMON_CMD_ARG_TYPE) * pDDB->Core.Hal.AllocHwResourceCrtCmd, LSA_TRUE);
                }
                break;

                default:
                {
                    EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
                    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] _EDDP_HALInitApiCtrlRam(): ERROR -> unknown Hardware(0x%X)", 
                        pDDB->Hardware);
                    return (EDD_STS_ERR_PARAM);
                }
                break;
            }

            // ***** prepare arguments to call commmen utility function for subcomponent initialization
            SubCompArgs.pCompRamBase    = pHal->RamComponentApiCtrl.pComponentRamBase;
            SubCompArgs.CompRamSize     = pHal->RamComponentApiCtrl.ComponentRamSize;
            SubCompArgs.CompRamFree     = 0;
            SubCompArgs.pSubArray       = pComp;
            SubCompArgs.Count           = SubCnt;
            RetCode                     = _EDDP_HALInitSubComponents(pDDB, &SubCompArgs);
            pHal->RamComponentApiCtrl.ComponentRamFreeSpace = SubCompArgs.CompRamFree;
        }
    }

    return (RetCode);
}
#endif

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_HALInitCmdRam                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB          +*/
/*+                             EDDP_UPPER_DPB_PTR_TYPE       pDPB          +*/
/*+                             EDD_RSP                       RetCode       +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                (in)  +*/
/*+  pDPB          : Pointer to DeviceParameterBlock                  (in)  +*/
/*+  RetCode       : Status of previos initialization                 (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This utility function is used to do intitialization of    +*/
/*+               its subcomponent if input EDD_STS_OK.                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_HALInitCmdRam(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DPB_PTR_TYPE     pDPB,
    EDD_RSP                     RetCode)
{
    EDDP_HAL_RAMLAYOUT_PTR_TYPE	            pHal;
    EDDP_HAL_LOCAL_RAM_SUB_COMP_PTR_TYPE    pComp;
    EDDP_HAL_SUB_COMP_ARG_TYPE              SubCompArgs;

    if (RetCode == EDD_STS_OK)
    {
        pHal    = &pDDB->Core.Hal.HalRamLayout;
        pComp   = &pHal->RamComponentCmd.ComponentRamArray[0];
        //                       pComp                                         , BitAlignmt          , SubRamLength                                        , UseBorderCheck
        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_CMD_QUEUE_MGMT]         , EDDP_ALIGMNT_64BIT  , sizeof(EDDP_CMD_RAM_QUEUE_MGMT_TYPE)                , LSA_FALSE);

        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_CMD_KRISC]              , EDDP_ALIGMNT_64BIT  , sizeof(EDDP_CMD_RAM_KRISC_TYPE)                     , LSA_TRUE);

        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_CMD_USER_CMD_INFO_BLOCK], EDDP_ALIGMNT_32BIT  , sizeof(EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_TYPE)       , LSA_TRUE);

        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_CMD_REQUEST_HOST_ASYNC] , EDDP_ALIGMNT_32BIT  , sizeof(LSA_UINT32) * (EDDP_CMD_TOTAL_COUNT_OF_NON_CRT_COMMANDS + pDDB->Core.Hal.AllocHwResourceCrtCmd) , LSA_TRUE);
        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_CMD_COMMAND_HOST_ASYNC] , EDDP_ALIGMNT_64BIT  , sizeof(EDDP_CMD_RAM_COMMAND_BLOCKS_HOST_ASYNC_TYPE) , LSA_TRUE);
        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_CMD_CRT_COMMAND_HOST_ASYNC] , EDDP_ALIGMNT_64BIT  , sizeof(EDDP_CMD_COMMAND_BLOCKS_CRT_HOST_ASYNC_TYPE) * pDDB->Core.Hal.AllocHwResourceCrtCmd , LSA_TRUE);
        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_CMD_PARAM_HOST_ASYNC]   , EDDP_ALIGMNT_64BIT  , sizeof(EDDP_CMD_RAM_PARAM_HOST_ASYNC_TYPE)          , LSA_TRUE);

        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_CMD_REQUEST_HOST_SYNC]  , EDDP_ALIGMNT_32BIT  , sizeof(EDDP_CMD_RAM_REQUEST_HOST_SYNC_TYPE)         , LSA_TRUE);
        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_CMD_COMMAND_HOST_SYNC]  , EDDP_ALIGMNT_64BIT  , sizeof(EDDP_CMD_RAM_COMMAND_BLOCK_HOST_SYNC_TYPE)   , LSA_TRUE);
        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_CMD_PARAM_HOST_SYNC]    , EDDP_ALIGMNT_64BIT  , sizeof(EDDP_CMD_RAM_PARAM_HOST_SYNC_TYPE)           , LSA_TRUE);

        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_CMD_REQUEST_USER_ASYNC] , EDDP_ALIGMNT_32BIT  , sizeof(EDDP_CMD_RAM_REQUEST_USER_ASYNC_TYPE)        , LSA_TRUE);
        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_CMD_COMMAND_USER_ASYNC] , EDDP_ALIGMNT_64BIT  , sizeof(EDDP_CMD_RAM_COMMAND_BLOCKS_USER_ASYNC_TYPE) , LSA_TRUE);
        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_CMD_PARAM_USER_ASYNC]   , EDDP_ALIGMNT_64BIT  , sizeof(EDDP_CMD_RAM_PARAM_USER_ASYNC_TYPE)          , LSA_TRUE);

        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_CMD_REQUEST_USER_SYNC]  , EDDP_ALIGMNT_32BIT  , sizeof(EDDP_CMD_RAM_REQUEST_USER_SYNC_TYPE)         , LSA_TRUE);
        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_CMD_COMMAND_USER_SYNC]  , EDDP_ALIGMNT_64BIT  , sizeof(EDDP_CMD_RAM_COMMAND_BLOCK_USER_SYNC_TYPE)   , LSA_TRUE);
        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_CMD_PARAM_USER_SYNC]    , EDDP_ALIGMNT_64BIT  , sizeof(EDDP_CMD_RAM_PARAM_USER_SYNC_TYPE)           , LSA_TRUE);

        // ***** prepare arguments to call commmen utility function for subcomponent initialization
        SubCompArgs.pCompRamBase    = pHal->RamComponentCmd.pComponentRamBase;
        SubCompArgs.CompRamSize     = pHal->RamComponentCmd.ComponentRamSize;
        SubCompArgs.CompRamFree     = 0;
        SubCompArgs.pSubArray       = pComp;
        SubCompArgs.Count           = EDDP_HAL_RAMSUB_CMD_COUNT;
        RetCode                     = _EDDP_HALInitSubComponents(pDDB, &SubCompArgs);
        pHal->RamComponentCmd.ComponentRamFreeSpace = SubCompArgs.CompRamFree;
    }

    LSA_UNUSED_ARG(pDPB);    // handle unused parameter

    return (RetCode);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_HALInitPackDataRam                    +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB          +*/
/*+                             EDDP_UPPER_DPB_PTR_TYPE       pDPB          +*/
/*+                             EDD_RSP                       RetCode       +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                (in)  +*/
/*+  pDPB          : Pointer to DeviceParameterBlock                  (in)  +*/
/*+  RetCode       : Status of previos initialization                 (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This utility function is used to do intitialization of    +*/
/*+               its subcomponent if input EDD_STS_OK.                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_HALInitPackDataRam(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DPB_PTR_TYPE     pDPB,
    EDD_RSP                     RetCode)
{
    // ***** default initialization with full RAM reservation
    //       --> overwrite this default implementation to support different subcomponents within the component ram
    EDDP_HAL_RAMLAYOUT_PTR_TYPE	pHal;
    EDDP_HAL_LOCAL_RAM_SUB_COMP_PTR_TYPE pComp;
    EDDP_HAL_SUB_COMP_ARG_TYPE SubCompArgs;

    if (RetCode == EDD_STS_OK)
    {
        pHal    = &pDDB->Core.Hal.HalRamLayout;
        pComp   = &pHal->RamComponentPackData.ComponentRamArray[0];
        //                      pComp , BitAlignmt,       SubRamLength,                       , UseBorderCheck
        _EDDP_HALSetSubCompArgs(pComp, EDDP_ALIGMNT_32BIT, pHal->RamComponentPackData.ComponentRamSize, LSA_FALSE);

        // ***** prepare arguments to call commmen utility function for subcomponent initialization
        SubCompArgs.pCompRamBase    = pHal->RamComponentPackData.pComponentRamBase;
        SubCompArgs.CompRamSize     = pHal->RamComponentPackData.ComponentRamSize;
        SubCompArgs.CompRamFree     = 0;
        SubCompArgs.pSubArray       = pComp;
        SubCompArgs.Count           = 1;
        RetCode                     = _EDDP_HALInitSubComponents(pDDB, &SubCompArgs);
        pHal->RamComponentPackData.ComponentRamFreeSpace = SubCompArgs.CompRamFree;
    }

    LSA_UNUSED_ARG(pDPB);    // handle unused parameter

    return (RetCode);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_HALInitSyncRam                        +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB          +*/
/*+                             EDDP_UPPER_DPB_PTR_TYPE       pDPB          +*/
/*+                             EDD_RSP                       RetCode       +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                (in)  +*/
/*+  pDPB          : Pointer to DeviceParameterBlock                  (in)  +*/
/*+  RetCode       : Status of previos initialization                 (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This utility function is used to do intitialization of    +*/
/*+               its subcomponent if input EDD_STS_OK.                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_HALInitSyncRam(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DPB_PTR_TYPE     pDPB,
    EDD_RSP                     RetCode)
{
    EDDP_HAL_RAMLAYOUT_PTR_TYPE	pHal;
    EDDP_HAL_LOCAL_RAM_SUB_COMP_PTR_TYPE pComp;
    EDDP_HAL_SUB_COMP_ARG_TYPE SubCompArgs;

    if (RetCode == EDD_STS_OK)
    {
        pHal    = &pDDB->Core.Hal.HalRamLayout;
        pComp   = &pHal->RamComponentSync.ComponentRamArray[0];
        //                       pComp                                        ,BitAlignmt           , SubRamLength                                                                , UseBorderCheck
        // ParamStruct for SYNC control commands
        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_SYNC_CMD_PARAM]        , EDDP_ALIGMNT_64BIT  , sizeof(PNIP_SWI_RAMSUB_SYNC_PARAM_STRUCT_TYPE)                              , LSA_FALSE);
        // Phase list
        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P1] , EDDP_ALIGMNT_32BIT  , (pDDB->SWI.PortCnt >= 1) ? sizeof(PNIP_SWI_RAMSUB_SYNC_PHASE_LIST_TYPE) : 0 , LSA_TRUE);
        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P2] , EDDP_ALIGMNT_32BIT  , (pDDB->SWI.PortCnt >= 2) ? sizeof(PNIP_SWI_RAMSUB_SYNC_PHASE_LIST_TYPE) : 0 , LSA_TRUE);
        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P3] , EDDP_ALIGMNT_32BIT  , (pDDB->SWI.PortCnt >= 3) ? sizeof(PNIP_SWI_RAMSUB_SYNC_PHASE_LIST_TYPE) : 0 , LSA_TRUE);
        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P4] , EDDP_ALIGMNT_32BIT  , (pDDB->SWI.PortCnt == 4) ? sizeof(PNIP_SWI_RAMSUB_SYNC_PHASE_LIST_TYPE) : 0 , LSA_TRUE);

        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P1] , EDDP_ALIGMNT_32BIT  , (pDDB->SWI.PortCnt >= 1) ? sizeof(PNIP_SWI_RAMSUB_SYNC_PHASE_LIST_TYPE) : 0 , LSA_TRUE);
        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P2] , EDDP_ALIGMNT_32BIT  , (pDDB->SWI.PortCnt >= 2) ? sizeof(PNIP_SWI_RAMSUB_SYNC_PHASE_LIST_TYPE) : 0 , LSA_TRUE);
        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P3] , EDDP_ALIGMNT_32BIT  , (pDDB->SWI.PortCnt >= 3) ? sizeof(PNIP_SWI_RAMSUB_SYNC_PHASE_LIST_TYPE) : 0 , LSA_TRUE);
        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P4] , EDDP_ALIGMNT_32BIT  , (pDDB->SWI.PortCnt == 4) ? sizeof(PNIP_SWI_RAMSUB_SYNC_PHASE_LIST_TYPE) : 0 , LSA_TRUE);
        // Time list
        _EDDP_HALSetSubCompArgs(&pComp[EDDP_HAL_RAMSUB_SYNC_TIME_LIST]        , EDDP_ALIGMNT_32BIT  , (pDDB->SWI.PortCnt * (EDDP_SWI_SYNC_MAX_TIME_LIST_ENTRIES_PER_PORT + 6) * sizeof(PNIP_SWI_RAMSUB_SYNC_TIME_LIST_ENTRY_TYPE)), LSA_TRUE);  

        // ***** prepare arguments to call commmen utility function for subcomponent initialization
        SubCompArgs.pCompRamBase    = pHal->RamComponentSync.pComponentRamBase;
        SubCompArgs.CompRamSize     = pHal->RamComponentSync.ComponentRamSize;
        SubCompArgs.CompRamFree     = 0;
        SubCompArgs.pSubArray       = pComp;
        SubCompArgs.Count           = EDDP_HAL_RAMSUB_SYNC_COUNT;
        RetCode                     = _EDDP_HALInitSubComponents(pDDB, &SubCompArgs);
        pHal->RamComponentSync.ComponentRamFreeSpace = SubCompArgs.CompRamFree;
    }

    LSA_UNUSED_ARG(pDPB);    // handle unused parameter

    return (RetCode);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_HALInitStatisticRam                   +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB          +*/
/*+                             EDDP_UPPER_DPB_PTR_TYPE       pDPB          +*/
/*+                             EDD_RSP                       RetCode       +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                (in)  +*/
/*+  pDPB          : Pointer to DeviceParameterBlock                  (in)  +*/
/*+  RetCode       : Status of previos initialization                 (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This utility function is used to do intitialization of    +*/
/*+               its subcomponent if input EDD_STS_OK.                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_HALInitStatisticRam(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DPB_PTR_TYPE     pDPB,
    EDD_RSP                     RetCode)
{
    // ***** default initialization with full RAM reservation
    //       --> overwrite this default implementation to support different subcomponents within the component ram
    EDDP_HAL_RAMLAYOUT_PTR_TYPE             pHal;
    EDDP_HAL_LOCAL_RAM_SUB_COMP_PTR_TYPE    pComp;
    EDDP_HAL_SUB_COMP_ARG_TYPE              SubCompArgs;

    if (RetCode == EDD_STS_OK)
    {
        pHal    = &pDDB->Core.Hal.HalRamLayout;
        pComp   = &pHal->RamComponentStatistic.ComponentRamArray[0];
        //                      pComp , BitAlignmt,       SubRamLength,                       , UseBorderCheck
        _EDDP_HALSetSubCompArgs(pComp, EDDP_ALIGMNT_32BIT, pHal->RamComponentStatistic.ComponentRamSize, LSA_FALSE);

        // prepare arguments to call commmen utility function for subcomponent initialization
        SubCompArgs.pCompRamBase    = pHal->RamComponentStatistic.pComponentRamBase;
        SubCompArgs.CompRamSize     = pHal->RamComponentStatistic.ComponentRamSize;
        SubCompArgs.CompRamFree     = 0;
        SubCompArgs.pSubArray       = pComp;
        SubCompArgs.Count           = 1;
        RetCode                     = _EDDP_HALInitSubComponents(pDDB, &SubCompArgs);
        pHal->RamComponentStatistic.ComponentRamFreeSpace = SubCompArgs.CompRamFree;
    }

    LSA_UNUSED_ARG(pDPB);   // handle unused parameter

    return (RetCode);
}

/*===========================================================================*/
/*                        public  functions                                  */
/*===========================================================================*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_HALTrimAlignment                       +*/
/*+  Input/Output          :    LSA_UINT32              Address             +*/
/*+                             LSA_UINT32              Alignment           +*/
/*+                                                                         +*/
/*+  Result                :    LSA_UINT32                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  Address    : Address with unknown alignment                      (in)  +*/
/*+  Alignment  : Selects the alignment                               (in)  +*/
/*+                                                                         +*/
/*+  Results    : aligned address                                           +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This utility function is used to do selected alignment    +*/
/**               and return the aligned address.                           +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_HALTrimAlignment(
    LSA_UINT32      Address,
    LSA_UINT32      Alignment)
{
    LSA_UINT32      RetAddress = Address;

    while (RetAddress & Alignment)
    {
        RetAddress += 1;
    }

    return (RetAddress);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_HALDeviceOpen                          +*/
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
/*+  Description: Initializes all HAL management structures within DDB.     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HALDeviceOpen(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DPB_PTR_TYPE     pDPB)
{
    EDD_RSP										RetCode = EDD_STS_OK;
    LSA_UINT32									PnipVersion, PnipDevelopment, VersionTemp, ConfigurationTemp, TypeTemp;
    LSA_UINT32									RamLengthInfo[EDDP_HAL_RAMTYPE_COMP_COUNT];
    LSA_UINT32									BaseAddr;
    LSA_UINT32                                  VlanRamStartAdd = 0, VlanRamLen = 0;
    LSA_UINT32                                  QueueCtrlRamStartAdd = 0, QueueCtrlRamLen = 0;
    LSA_UINT32                                  QueueDataRamStartAdd = 0, QueueDataRamLen = 0;
    LSA_UINT32                                  StatisticRamLen;
    LSA_UINT32                                  SyncRamLen;
    LSA_UINT32                                  PackdataRamLen;
    LSA_UINT32                                  CmdctrlRamLen;
    LSA_UINT32                                  ApictrlRamLen;
    LSA_UINT32                                  ApiNRTctrlRamLen;
    LSA_UINT32                                  FdbRamLen;              
    LSA_UINT32                                  ApiPstRamLen;
    EDDP_HAL_RAMLAYOUT_PTR_TYPE					pHal;
    EDDP_HAL_LOCAL_RAM_COMP_COMMON_PTR_TYPE     pCompPtr;
    
    EDDP_CORE_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_HALDeviceOpen(pDDB=0x%X, pDPB=0x%X)",
        pDDB, pDPB);

    EDDP_ASSERT_NO_NULL_PTR(pDDB);
    EDDP_ASSERT_NO_NULL_PTR(pDPB);

    pHal                = &pDDB->Core.Hal.HalRamLayout;
    pHal->pPnipRamBase  = LSA_NULL;
    EDDP_ASSERT_NO_NULL_PTR(pDDB->pPnipBase);
    pHal->pPnipRamBase  = pDDB->pPnipBase;
    pHal->SetupDone     = LSA_FALSE;

    /* -------------------------------------------------------- */
    /* Init "PnipVersion" and "PnipDevelopment"                 */
    /* -------------------------------------------------------- */
    RetCode = EDDP_HALGetVersion (pDDB, &PnipVersion, &PnipDevelopment);
    EDDP_ASSERT_FALSE(RetCode != EDD_STS_OK);

    // get version of PNIP (it is revision)
    /* There is a macro PNIP_R2_REG_IP_VERSION__MSK_VERSION and PNIP_R2_REG_IP_VERSION_SHFT_VERSION too which has the same value. Checked in eddp_hal_pnip_val.h.*/
    /* Here only _R1_ is used. */
    VersionTemp = EDDP_GET_BIT_VALUE(PnipVersion, PNIP_R1_REG_IP_VERSION__MSK_VERSION, PNIP_R1_REG_IP_VERSION_SHFT_VERSION);

    ConfigurationTemp = EDDP_GET_BIT_VALUE(PnipVersion, EDDP_HAL_R3_REG_IP_VERSION__MSK_CONFIGURATION, EDDP_HAL_R3_REG_IP_VERSION_SHFT_CONFIGURATION);

    /* There is a macro PNIP_F0_REG_IP_DEVELOPMENT__MSK_TYPE and PNIP_F0_REG_IP_DEVELOPMENT_SHFT_TYPE too which has the same value. Checked in eddp_hal_pnip_val.h.*/
    TypeTemp = EDDP_GET_BIT_VALUE(PnipDevelopment, PNIP_A0_REG_IP_DEVELOPMENT__MSK_TYPE, PNIP_A0_REG_IP_DEVELOPMENT_SHFT_TYPE);

    StatisticRamLen    = 0;
    SyncRamLen         = 0;
    PackdataRamLen     = 0;
    CmdctrlRamLen      = 0;
    ApictrlRamLen      = 0;
    ApiNRTctrlRamLen   = 0;
    FdbRamLen          = 0;
    ApiPstRamLen       = 0;

    switch (VersionTemp)
    {
        /* -------------------------------------------- */
        /* PNIP-Revision(1)                             */
        /* -------------------------------------------- */
        case EDDP_HAL_REG_IP_VERSION_VERSION_REV1:
        {
            pDDB->Core.Hal.HardwareVersion      = EDDP_HAL_HW_PNIP_REV1;
            pDDB->Core.Hal.HardwareChipVersion  = EDDP_HAL_HW_ERTEC200P;
            pDDB->Core.Hal.HardwareStep         = EDDP_HAL_HW_STEP1;
            pDDB->Core.Hal.HardwarePortCntType  = EDDP_HAL_HW_PNIP_CFG_2PORT;

            switch (pDDB->Hardware)
            {
                case EDDP_HW_ERTEC200P:
                {
                    if(EDDP_HAL_REG_IP_DEVELOPMENT_TYPE_ASIC != TypeTemp)
                    {
                        EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALDeviceOpen(): FATAL -> Hardware(0x%X) has wrong Type (ASIC/FPGA)", 
                            pDDB->Hardware);
                        EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                    }
                    pDDB->Core.Hal.PNIP_Revision       = EDDP_HAL_HW_PNIP_R1A1;
                    pDDB->Core.Hal.HardwareChipType    = EDDP_HAL_HW_TYPE_ASIC;
                    /* PNIP_A0_RAM_VLAN_RAM_START was replaced by PNIP_R2A0P2_RAM_VLAN_RAM_START */
                    /* all macros of group PNIP_xxxx_RAM_VLAN_RAM_START are checked in eddp_hal_pnip_val.h to have the same value*/
                    VlanRamStartAdd                 = PNIP_R2A0_RAM_VLAN_RAM_START;
                    /* PNIP_A0_RAM_VLAN_RAM_LEN was replaced by PNIP_R2A0P2_RAM_VLAN_RAM_LEN, hardware dependency*/
                    VlanRamLen                      = PNIP_R2A0_RAM_VLAN_RAM_LEN;
                }
                break;
                case EDDP_HW_ERTEC200P_FPGA:
                {
                    if(EDDP_HAL_REG_IP_DEVELOPMENT_TYPE_FPGA != TypeTemp)
                    {
                        EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALDeviceOpen(): FATAL -> Hardware(0x%X) has wrong Type (ASIC/FPGA)", 
                            pDDB->Hardware);
                        EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                    }
                    pDDB->Core.Hal.PNIP_Revision    = EDDP_HAL_HW_PNIP_R1F1;
                    pDDB->Core.Hal.HardwareChipType = EDDP_HAL_HW_TYPE_FPGA;
                    /* PNIP_F0_RAM_VLAN_RAM_START was replaced by PNIP_R2F0P2_RAM_VLAN_RAM_START */
                    /* all macros of group PNIP_xxxx_RAM_VLAN_RAM_START are checked in eddp_hal_pnip_val.h to have the same value*/
                    VlanRamStartAdd                 = PNIP_R2F0_RAM_VLAN_RAM_START;
                    /* PNIP_F0_RAM_VLAN_RAM_LEN was replaced by PNIP_R2F0P2_RAM_VLAN_RAM_LEN, hardware dependency*/
                    VlanRamLen                      = PNIP_R2F0_RAM_VLAN_RAM_LEN;
                }
                break;

                default:
                {
                    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALDeviceOpen(): FATAL -> Hardware(0x%X) is not supported!",
                        pDDB->Hardware);
                    EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                }
                break;
            }

            if(EDDP_HAL_HW_IF_A != pDPB->HardwareIF)
            {
                EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALDeviceOpen(): FATAL ->Hardware(0x%X) supports just InterfaceA!", 
                    pDDB->Hardware);
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            }

            /* PNIP_RAM_STATISTIC_RAM_LEN was replaced by PNIP_R2P2_RAM_STATISTIC_RAM_LEN, hardware dependency*/
            StatisticRamLen = PNIP_R2P2_RAM_STATISTIC_RAM_LEN;
            /* PNIP_RAM_SYNC_RAM_LEN was replaced by PNIP_R2P2_RAM_SYNC_RAM_LEN, hardware dependency*/
            SyncRamLen      = PNIP_R2P2_RAM_SYNC_RAM_LEN;
            /* PNIP_RAM_PACKDATA_RAM_LEN was replaced by PNIP_R2P2_RAM_PACKDATA_RAM_LEN, hardware dependency*/
            PackdataRamLen  = PNIP_R2P2_RAM_PACKDATA_RAM_LEN;
            /* PNIP_RAM_CMDCTRL_RAM_LEN was replaced by PNIP_R2P2_RAM_CMDCTRL_RAM_LEN, hardware dependency*/
            CmdctrlRamLen   = PNIP_R2P2_RAM_CMDCTRL_RAM_LEN;
            /* PNIP_RAM_APICTRL_RAM_LEN was replaced by PNIP_R2P2_RAM_APICTRL_RAM_LEN, hardware dependency*/
            ApictrlRamLen   = PNIP_R2P2_RAM_APICTRL_RAM_LEN;
            /* PNIP_RAM_FDB_RAM_LEN was replaced by PNIP_R2P2_RAM_FDB_RAM_LEN, hardware dependency*/
            FdbRamLen       = PNIP_R2P2_RAM_FDB_RAM_LEN;

            /* PNIP_R1_RAM_QUEUECTRL_RAM_START was replaced by PNIP_R1P2_RAM_QUEUECTRL_RAM_START */
            /* all macros of group PNIP_xxxx_RAM_QUEUECTRL_RAM_START are checked in eddp_hal_pnip_val.h to have the same value*/
            QueueCtrlRamStartAdd    = PNIP_R1_RAM_QUEUECTRL_RAM_START;
            /* PNIP_R1_RAM_QUEUECTRL_RAM_LEN was replaced by PNIP_R1P2_RAM_QUEUECTRL_RAM_LEN, hardware dependency*/
            QueueCtrlRamLen         = PNIP_R1_RAM_QUEUECTRL_RAM_LEN;
            /* PNIP_R1_RAM_QUEUEDATA_RAM_START was replaced by PNIP_R1P2_RAM_QUEUEDATA_RAM_START */
            /* all macros of group PNIP_xxxx_RAM_QUEUEDATA_RAM_START are checked in eddp_hal_pnip_val.h to have the same value*/
            QueueDataRamStartAdd    = PNIP_R1_RAM_QUEUEDATA_RAM_START;
            /* PNIP_R1_RAM_QUEUEDATA_RAM_LEN was replaced by PNIP_R1P2_RAM_QUEUEDATA_RAM_LEN, hardware dependency*/
            QueueDataRamLen         = PNIP_R1_RAM_QUEUEDATA_RAM_LEN;
            
            #if (!defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT))
            {
                RetCode = EDD_STS_ERR_NOT_SUPPORTED;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, RetCode);
                EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_HALDeviceOpen(): ERROR -> PNIP-HW is Revision(1) but EDDP_CFG_HW_PNIP_REV1_SUPPORT is not defined");
            }
            #endif
        }
        break;

        /* -------------------------------------------- */
        /* PNIP-Revision(2)                             */
        /* -------------------------------------------- */
        case EDDP_HAL_REG_IP_VERSION_VERSION_REV2:
        {
            pDDB->Core.Hal.HardwareVersion      = EDDP_HAL_HW_PNIP_REV2;
            pDDB->Core.Hal.HardwareChipVersion  = EDDP_HAL_HW_ERTEC200P;
            pDDB->Core.Hal.HardwareStep         = EDDP_HAL_HW_STEP2;
            pDDB->Core.Hal.HardwarePortCntType  = EDDP_HAL_HW_PNIP_CFG_2PORT;

            switch (pDDB->Hardware)
            {
                case EDDP_HW_ERTEC200P:
                {
                    if(EDDP_HAL_REG_IP_DEVELOPMENT_TYPE_ASIC != TypeTemp)
                    {
                        EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALDeviceOpen(): FATAL -> Hardware(0x%X) has wrong Type (ASIC/FPGA)", 
                            pDDB->Hardware);
                        EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                    }
                    pDDB->Core.Hal.PNIP_Revision    = EDDP_HAL_HW_PNIP_R2A1;
                    pDDB->Core.Hal.HardwareChipType = EDDP_HAL_HW_TYPE_ASIC;
                    /* PNIP_A0_RAM_VLAN_RAM_START was replaced by PNIP_R2A0P2_RAM_VLAN_RAM_START */
                    /* all macros of group PNIP_xxxx_RAM_VLAN_RAM_START are checked in eddp_hal_pnip_val.h to have the same value*/
                    VlanRamStartAdd                 = PNIP_R2A0_RAM_VLAN_RAM_START;
                    /* PNIP_A0_RAM_VLAN_RAM_LEN was replaced by PNIP_R2A0P2_RAM_VLAN_RAM_LEN, hardware dependency*/
                    VlanRamLen                      = PNIP_R2A0_RAM_VLAN_RAM_LEN;
                }
                break;

                case EDDP_HW_ERTEC200P_FPGA:
                {
                    if(EDDP_HAL_REG_IP_DEVELOPMENT_TYPE_FPGA != TypeTemp)
                    {
                        EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALDeviceOpen(): FATAL -> Hardware(0x%X) has wrong Type (ASIC/FPGA)", 
                            pDDB->Hardware);
                        EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                    }
                    pDDB->Core.Hal.PNIP_Revision    = EDDP_HAL_HW_PNIP_R2F1;
                    pDDB->Core.Hal.HardwareChipType = EDDP_HAL_HW_TYPE_FPGA;
                    /* PNIP_F0_RAM_VLAN_RAM_START was replaced by PNIP_R2F0P2_RAM_VLAN_RAM_START */
                    /* all macros of group PNIP_xxxxxx_RAM_VLAN_RAM_START are checked in eddp_hal_pnip_val.h to have the same value*/
                    VlanRamStartAdd                 = PNIP_R2F0_RAM_VLAN_RAM_START;
                    /* PNIP_F0_RAM_VLAN_RAM_LEN was replaced by PNIP_R2F0P2_RAM_VLAN_RAM_LEN, hardware dependency*/
                    VlanRamLen                      = PNIP_R2F0_RAM_VLAN_RAM_LEN;
                }
                break;

                default:
                {
                    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALDeviceOpen(): FATAL -> Hardware(0x%X) is not supported!", 
                        pDDB->Hardware);
                    EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                }
                break;
            }


            if(EDDP_HAL_HW_IF_A != pDPB->HardwareIF)
            {
                EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALDeviceOpen(): FATAL ->Hardware(0x%X) supports just InterfaceA!", 
                    pDDB->Hardware);
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            }


            /* PNIP_RAM_STATISTIC_RAM_LEN was replaced by PNIP_R2P2_RAM_STATISTIC_RAM_LEN, hardware dependency*/
            StatisticRamLen     = PNIP_R2P2_RAM_STATISTIC_RAM_LEN;
            /* PNIP_RAM_SYNC_RAM_LEN was replaced by PNIP_R2P2_RAM_SYNC_RAM_LEN, hardware dependency*/
            SyncRamLen          = PNIP_R2P2_RAM_SYNC_RAM_LEN;
            /* PNIP_RAM_PACKDATA_RAM_LEN was replaced by PNIP_R2P2_RAM_PACKDATA_RAM_LEN, hardware dependency*/
            PackdataRamLen      = PNIP_R2P2_RAM_PACKDATA_RAM_LEN;
            /* PNIP_RAM_CMDCTRL_RAM_LEN was replaced by PNIP_R2P2_RAM_CMDCTRL_RAM_LEN, hardware dependency*/
            CmdctrlRamLen       = PNIP_R2P2_RAM_CMDCTRL_RAM_LEN;
            /* PNIP_RAM_APICTRL_RAM_LEN was replaced by PNIP_R2P2_RAM_APICTRL_RAM_LEN, hardware dependency*/
            ApictrlRamLen       = PNIP_R2P2_RAM_APICTRL_RAM_LEN;
            /* PNIP_RAM_FDB_RAM_LEN was replaced by PNIP_R2P2_RAM_FDB_RAM_LEN, hardware dependency*/
            FdbRamLen           = PNIP_R2P2_RAM_FDB_RAM_LEN;
			
            /* PNIP_R2_RAM_QUEUECTRL_RAM_START was replaced by PNIP_R2P2_RAM_QUEUECTRL_RAM_START */
		    /* all macros of group PNIP_xxxx_RAM_QUEUECTRL_RAM_START are checked in eddp_hal_pnip_val.h to have the same value*/
            QueueCtrlRamStartAdd    = PNIP_R2P2_RAM_QUEUECTRL_RAM_START;
            /* PNIP_R2_RAM_QUEUECTRL_RAM_LEN was replaced by PNIP_R2P2_RAM_QUEUECTRL_RAM_LEN, hardware dependency*/
            QueueCtrlRamLen         = PNIP_R2P2_RAM_QUEUECTRL_RAM_LEN;
            /* PNIP_R2_RAM_QUEUEDATA_RAM_START was replaced by PNIP_R2P2_RAM_QUEUEDATA_RAM_START */
            /* all macros of group PNIP_xxxx_RAM_QUEUEDATA_RAM_START are checked in eddp_hal_pnip_val.h to have the same value*/
            QueueDataRamStartAdd    = PNIP_R2P2_RAM_QUEUEDATA_RAM_START;
            /* PNIP_R2_RAM_QUEUEDATA_RAM_LEN was replaced by PNIP_R2P2_RAM_QUEUEDATA_RAM_LEN, hardware dependency*/
            QueueDataRamLen         = PNIP_R2P2_RAM_QUEUEDATA_RAM_LEN;
            
            #if (!defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT))
            {
                RetCode = EDD_STS_ERR_NOT_SUPPORTED;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, RetCode);
                EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_HALDeviceOpen(): ERROR -> PNIP-HW is Revision(2) but EDDP_CFG_HW_PNIP_REV2_SUPPORT is not defined");
            }
            #endif
        }
        break;

        /* -------------------------------------------- */
        /* PNIP-Revision(3)                             */
        /* -------------------------------------------- */
        case EDDP_HAL_REG_IP_VERSION_VERSION_REV3:
        {
            pDDB->Core.Hal.HardwareVersion      = EDDP_HAL_HW_PNIP_REV3;
            pDDB->Core.Hal.HardwareChipVersion  = EDDP_HAL_HW_HERA;
            pDDB->Core.Hal.HardwareStep         = EDDP_HAL_HW_STEP1;

            switch (pDDB->Hardware)
            {
                case EDDP_HW_HERA_FPGA:
                {
                    if(EDDP_HAL_REG_IP_DEVELOPMENT_TYPE_FPGA != TypeTemp)
                    {
                        EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALDeviceOpen(): FATAL -> Hardware(0x%X) has wrong Type (ASIC/FPGA)", 
                            pDDB->Hardware);
                        EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                    }
                    pDDB->Core.Hal.PNIP_Revision    = EDDP_HAL_HW_PNIP_R3F1;
                    pDDB->Core.Hal.HardwareChipType = EDDP_HAL_HW_TYPE_FPGA;
                    /* PNIP_F0_RAM_VLAN_RAM_START was replaced by PNIP_R2F0P2_RAM_VLAN_RAM_START */
                    /* all macros of group PNIP_xxxxxx_RAM_VLAN_RAM_START are checked in eddp_hal_pnip_val.h to have the same value*/
                    VlanRamStartAdd                 = PNIP_R3F0P2_RAM_VLAN_RAM_START;
                    /* PNIP_F0_RAM_VLAN_RAM_LEN was replaced by PNIP_R2F0P2_RAM_VLAN_RAM_LEN, hardware dependency*/
                    /* no hardware dependency on 2 port and 4 port HERA fpgas, which is checked in eddp_hal_pnip_val.h to have the same value*/
                    VlanRamLen                      = PNIP_R3F0P2_RAM_VLAN_RAM_LEN;
                    ApiPstRamLen                    = PNIP_R3_RAM_APIPST_RAM_LEN;
                }
                break;

                default:
                {
                    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALDeviceOpen(): FATAL -> Hardware(0x%X) is not supported!", 
                        pDDB->Hardware);
                    EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                }
                break;
            }

            switch(ConfigurationTemp)
            {
                case EDDP_HAL_REG_IP_VERSION_VALUE_CONFIGURATION_4P_R2F0_1IF:
                case EDDP_HAL_REG_IP_VERSION_VALUE_CONFIGURATION_4P_R3F0_1IFA_INC4:
                case EDDP_HAL_REG_IP_VERSION_VALUE_CONFIGURATION_4P_R3F0_1IFA_INC10:
                case EDDP_HAL_REG_IP_VERSION_VALUE_CONFIGURATION_4P_R3F0_1IFA_INC8:
                case EDDP_HAL_REG_IP_VERSION_VALUE_CONFIGURATION_4P_R3F0_1IFA_INC12:
                case EDDP_HAL_REG_IP_VERSION_VALUE_CONFIGURATION_4P_R3A0_1IFA:
                {
                    pDDB->Core.Hal.HardwarePortCntType  = EDDP_HAL_HW_PNIP_CFG_4PORT;
                    
                    /* PNIP_RAM_STATISTIC_RAM_LEN was replaced by PNIP_R2P2_RAM_STATISTIC_RAM_LEN, hardware dependency*/
                    StatisticRamLen         = PNIP_R3P4_RAM_STATISTIC_RAM_LEN;
                    /* PNIP_RAM_SYNC_RAM_LEN was replaced by PNIP_R2P2_RAM_SYNC_RAM_LEN, hardware dependency*/
                    SyncRamLen              = PNIP_R3P4_RAM_SYNC_RAM_LEN;
                    /* PNIP_RAM_PACKDATA_RAM_LEN was replaced by PNIP_R2P2_RAM_PACKDATA_RAM_LEN, hardware dependency*/
                    PackdataRamLen          = PNIP_R3_RAM_PACKDATA_RAM_LEN;
                    /* PNIP_RAM_CMDCTRL_RAM_LEN was replaced by PNIP_R2P2_RAM_CMDCTRL_RAM_LEN, hardware dependency*/
                    CmdctrlRamLen           = PNIP_R3P4_RAM_CMDCTRL_RAM_LEN;
                    /* PNIP_RAM_APICTRL_RAM_LEN was replaced by PNIP_R2P2_RAM_APICTRL_RAM_LEN, hardware dependency*/
                    ApictrlRamLen           = PNIP_R3P4_RAM_APICTRL_RAM_LEN;
                    ApiNRTctrlRamLen        = PNIP_R3P4_RAM_APINRTCTRL_RAM_LEN;
                    ApiPstRamLen            = PNIP_R3_RAM_APIPST_RAM_LEN;
                    /* PNIP_RAM_FDB_RAM_LEN was replaced by PNIP_R2P2_RAM_FDB_RAM_LEN, hardware dependency*/
                    FdbRamLen               = PNIP_R3P4_RAM_FDB_RAM_LEN;
                    /* PNIP_R2_RAM_QUEUECTRL_RAM_LEN was replaced by PNIP_R2P2_RAM_QUEUECTRL_RAM_LEN, hardware dependency*/
                    QueueCtrlRamLen         = PNIP_R3P4_RAM_QUEUECTRL_RAM_LEN;
                    /* PNIP_R2_RAM_QUEUEDATA_RAM_LEN was replaced by PNIP_R2P2_RAM_QUEUEDATA_RAM_LEN, hardware dependency*/
                    QueueDataRamLen         = PNIP_R3P4_RAM_QUEUEDATA_RAM_LEN;

                    if(EDDP_HAL_HW_IF_A != pDPB->HardwareIF)
                    {
                        EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALDeviceOpen(): FATAL ->Hardware(0x%X) supports just InterfaceA!", 
                            pDDB->Hardware);
                        EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                    }
                }
                break;


                case EDDP_HAL_REG_IP_VERSION_VALUE_CONFIGURATION_2P_R2F0_1IF:
                case EDDP_HAL_REG_IP_VERSION_VALUE_CONFIGURATION_2P_R3F0_1IFB_INC14:
                case EDDP_HAL_REG_IP_VERSION_VALUE_CONFIGURATION_2P_R2_1IF:
                case EDDP_HAL_REG_IP_VERSION_VALUE_CONFIGURATION_2P_R3A0_1IFB:
                {
                    pDDB->Core.Hal.HardwarePortCntType  = EDDP_HAL_HW_PNIP_CFG_2PORT;
                    
                    /* PNIP_RAM_STATISTIC_RAM_LEN was replaced by PNIP_R2P2_RAM_STATISTIC_RAM_LEN, hardware dependency*/
                    StatisticRamLen         = PNIP_R3P2_RAM_STATISTIC_RAM_LEN;
                    /* PNIP_RAM_SYNC_RAM_LEN was replaced by PNIP_R2P2_RAM_SYNC_RAM_LEN, hardware dependency*/
                    SyncRamLen              = PNIP_R3P2_RAM_SYNC_RAM_LEN;
                    /* PNIP_RAM_PACKDATA_RAM_LEN was replaced by PNIP_R2P2_RAM_PACKDATA_RAM_LEN, hardware dependency*/
                    PackdataRamLen          = PNIP_R3_RAM_PACKDATA_RAM_LEN;
                    /* PNIP_RAM_CMDCTRL_RAM_LEN was replaced by PNIP_R2P2_RAM_CMDCTRL_RAM_LEN, hardware dependency*/
                    CmdctrlRamLen           = PNIP_R3P2_RAM_CMDCTRL_RAM_LEN;
                    /* PNIP_RAM_APICTRL_RAM_LEN was replaced by PNIP_R2P2_RAM_APICTRL_RAM_LEN, hardware dependency*/
                    ApictrlRamLen           = PNIP_R3P2_RAM_APICTRL_RAM_LEN;
                    ApiNRTctrlRamLen        = PNIP_R3P2_RAM_APINRTCTRL_RAM_LEN;
                    /* PNIP_RAM_FDB_RAM_LEN was replaced by PNIP_R2P2_RAM_FDB_RAM_LEN, hardware dependency*/
                    FdbRamLen               = PNIP_R3P2_RAM_FDB_RAM_LEN;
                    /* PNIP_R2_RAM_QUEUECTRL_RAM_LEN was replaced by PNIP_R2P2_RAM_QUEUECTRL_RAM_LEN, hardware dependency*/
                    QueueCtrlRamLen         = PNIP_R3P2_RAM_QUEUECTRL_RAM_LEN;  
                    /* PNIP_R2_RAM_QUEUEDATA_RAM_LEN was replaced by PNIP_R2P2_RAM_QUEUEDATA_RAM_LEN, hardware dependency*/
                    QueueDataRamLen         = PNIP_R3P2_RAM_QUEUEDATA_RAM_LEN;
                    
                    if(EDDP_HAL_HW_IF_B != pDPB->HardwareIF)
                    {
                        EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALDeviceOpen(): FATAL ->Hardware(0x%X) supports just InterfaceA!", 
                            pDDB->Hardware);
                        EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                    }
                }
                break;
                
                default:
                {
                    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALDeviceOpen(): FATAL ->Configuration of this Hardware(0x%X) is not supported!", 
                        pDDB->Hardware);
                    EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                }
                break;
            }
            
            /* PNIP_R2_RAM_QUEUECTRL_RAM_START was replaced by PNIP_R2P2_RAM_QUEUECTRL_RAM_START */
            /* all macros of group PNIP_xxxx_RAM_QUEUECTRL_RAM_START are checked in eddp_hal_pnip_val.h to have the same value*/
            QueueCtrlRamStartAdd    = PNIP_R2P2_RAM_QUEUECTRL_RAM_START;
            
            /* PNIP_R2_RAM_QUEUEDATA_RAM_START was replaced by PNIP_R2P2_RAM_QUEUEDATA_RAM_START */
            /* all macros of group PNIP_xxxx_RAM_QUEUEDATA_RAM_START are checked in eddp_hal_pnip_val.h to have the same value*/
            QueueDataRamStartAdd    = PNIP_R2P2_RAM_QUEUEDATA_RAM_START;
            
            {
                RetCode = EDD_STS_ERR_NOT_SUPPORTED;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, RetCode);
                EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_HALDeviceOpen(): ERROR -> PNIP-HW is Revision(3) but EDDP_CFG_HW_PNIP_REV3_SUPPORT is not defined");
            }
        }
        break;

        default:
        {
			EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALDeviceOpen(): FATAL -> Register IP_Development.Inkr(0x%X) is unknown !!!", 
                    VersionTemp);
            EDDP_FATAL(EDDP_FATAL_ERR_DEV_OPEN);
        }
        break;
    }
    
    /* set HW resource size for CRT commands */
    if ( pDDB->Core.Hal.HardwareChipVersion == EDDP_HAL_HW_ERTEC200P )
    {   
        pDDB->Core.Hal.AllocHwResourceCrtCmd = EDDP_CMD_CPM_PPM_COMMAND_DEPTH_REV12 + 1; /* NOTE: +1 --> 1 reserved entry */
    }
    else if( pDDB->Core.Hal.HardwareChipVersion == EDDP_HAL_HW_HERA )
    {
        pDDB->Core.Hal.AllocHwResourceCrtCmd = EDDP_CMD_CPM_PPM_COMMAND_DEPTH_REV3 + 2; /* NOTE: +2 --> 1 reserved entry + 1 delay command */
    }        
    else
    {
        EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALDeviceOpen(): FATAL -> hardware chip version (0x%X) unknown !!!", 
                pDDB->Core.Hal.HardwareChipVersion);
        EDDP_FATAL(EDDP_FATAL_ERR_DEV_OPEN);
    }
    
    if( (0 == StatisticRamLen) ||
        (0 == SyncRamLen     ) ||
        (0 == PackdataRamLen ) ||
        (0 == CmdctrlRamLen  ) ||
        (0 == ApictrlRamLen  ) ||
        (0 == FdbRamLen      )
      )
    {
        	EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALDeviceOpen(): FATAL -> RamLen Unknown !!! Hardware=0x%X, VersionTemp=0x%X,  ConfigurationTemp=0x%X", 
                pDDB->Hardware,
                VersionTemp,
                ConfigurationTemp);
            EDDP_FATAL(EDDP_FATAL_ERR_DEV_OPEN);
    }

    pDDB->Core.Hal.HardwareIF = pDPB->HardwareIF;

    // ***** initialization of ram module
    BaseAddr = (LSA_UINT32)pDDB->pPnipBase;

    /* PNIP_RAM_STATISTIC_RAM_LEN was replaced by PNIP_R2P2_RAM_STATISTIC_RAM_LEN, hardware dependency*/
    RamLengthInfo[EDDP_HAL_RAMTYPE_STATISTIC]   = StatisticRamLen;
    /* PNIP_RAM_SYNC_RAM_LEN was replaced by PNIP_R2P2_RAM_SYNC_RAM_LEN, hardware dependency*/
    RamLengthInfo[EDDP_HAL_RAMTYPE_SYNC]        = SyncRamLen;
    /* PNIP_RAM_PACKDATA_RAM_LEN was replaced by PNIP_R2P2_RAM_PACKDATA_RAM_LEN, hardware dependency*/
    RamLengthInfo[EDDP_HAL_RAMTYPE_PACK_DATA]   = PackdataRamLen;
    /* PNIP_RAM_CMDCTRL_RAM_LEN was replaced by PNIP_R2P2_RAM_CMDCTRL_RAM_LEN, hardware dependency*/
    RamLengthInfo[EDDP_HAL_RAMTYPE_CMD]         = CmdctrlRamLen;
    /* PNIP_RAM_APICTRL_RAM_LEN was replaced by PNIP_R2P2_RAM_APICTRL_RAM_LEN, hardware dependency*/
    RamLengthInfo[EDDP_HAL_RAMTYPE_API_CTRL]    = ApictrlRamLen;
    RamLengthInfo[EDDP_HAL_RAMTYPE_API_NRT_CTRL]    = ApiNRTctrlRamLen;
    /* PNIP_RAM_FDB_RAM_LEN was replaced by PNIP_R2P2_RAM_FDB_RAM_LEN, hardware dependency*/
    RamLengthInfo[EDDP_HAL_RAMTYPE_FDB_TABLE]   = FdbRamLen;
    RamLengthInfo[EDDP_HAL_RAMTYPE_API_PST]     = ApiPstRamLen;

    RamLengthInfo[EDDP_HAL_RAMTYPE_VLAN_TABLE]  = VlanRamLen;
    RamLengthInfo[EDDP_HAL_RAMTYPE_QUEUE_CTRL]  = QueueCtrlRamLen;
    RamLengthInfo[EDDP_HAL_RAMTYPE_QUEUE_DATA]  = QueueDataRamLen;

    //-------------------------------------------------------------
    pHal->RamComponentQueueData.pComponentRamBase           = (EDD_COMMON_MEM_PTR_TYPE)(BaseAddr + QueueDataRamStartAdd);
    pHal->RamComponentQueueData.ComponentRamSize            = RamLengthInfo[EDDP_HAL_RAMTYPE_QUEUE_DATA];
    pHal->RamComponentQueueData.ComponentRamCount           = EDDP_HAL_RAMSUB_QUEUE_DATA_COUNT;
    pHal->RamComponentQueueData.Is8BitAccessAllowed         = LSA_TRUE;
    RetCode = _EDDP_HALInitQueueDataRam (pDDB, pDPB, RetCode);
    pHal->RamCommonArrayPtr[EDDP_HAL_RAMTYPE_QUEUE_DATA]    = (EDDP_HAL_LOCAL_RAM_COMP_COMMON_PTR_TYPE)&pHal->RamComponentQueueData;

    //-------------------------------------------------------------
    pHal->RamComponentQueueCtrl.pComponentRamBase           = (EDD_COMMON_MEM_PTR_TYPE)(BaseAddr + QueueCtrlRamStartAdd);
    pHal->RamComponentQueueCtrl.ComponentRamSize            = RamLengthInfo[EDDP_HAL_RAMTYPE_QUEUE_CTRL];
    pHal->RamComponentQueueCtrl.ComponentRamCount           = EDDP_HAL_RAMSUB_QUEUE_CTRL_COUNT;
    pHal->RamComponentQueueCtrl.Is8BitAccessAllowed         = LSA_TRUE;
    RetCode = _EDDP_HALInitQueueCtrlRam (pDDB, pDPB, RetCode);
    pHal->RamCommonArrayPtr[EDDP_HAL_RAMTYPE_QUEUE_CTRL]    = (EDDP_HAL_LOCAL_RAM_COMP_COMMON_PTR_TYPE)&pHal->RamComponentQueueCtrl;

    //-------------------------------------------------------------
    pHal->RamComponentVlanTable.pComponentRamBase           = (EDD_COMMON_MEM_PTR_TYPE)(BaseAddr + VlanRamStartAdd);
    pHal->RamComponentVlanTable.ComponentRamSize            = RamLengthInfo[EDDP_HAL_RAMTYPE_VLAN_TABLE];
    pHal->RamComponentVlanTable.ComponentRamCount           = EDDP_HAL_RAMSUB_VLAN_TABLE_COUNT;
    pHal->RamComponentVlanTable.Is8BitAccessAllowed         = LSA_TRUE;
    RetCode = _EDDP_HALInitVlanTableRam (pDDB, pDPB, RetCode);
    pHal->RamCommonArrayPtr[EDDP_HAL_RAMTYPE_VLAN_TABLE]    = (EDDP_HAL_LOCAL_RAM_COMP_COMMON_PTR_TYPE)&pHal->RamComponentVlanTable;

    //-------------------------------------------------------------
    /* PNIP_RAM_FDB_RAM_START was replaced by PNIP_R2P2_RAM_FDB_RAM_START */
    /* all macros of group PNIP_xxxx_RAM_FDB_RAM_START are checked in eddp_hal_pnip_val.h to have the same value*/
    pHal->RamComponentFdbTable.pComponentRamBase        = (EDD_COMMON_MEM_PTR_TYPE)(BaseAddr + PNIP_R2P2_RAM_FDB_RAM_START);
    pHal->RamComponentFdbTable.ComponentRamSize         = RamLengthInfo[EDDP_HAL_RAMTYPE_FDB_TABLE];
    pHal->RamComponentFdbTable.ComponentRamCount        = EDDP_HAL_RAMSUB_FDB_TABLE_COUNT;
    pHal->RamComponentFdbTable.Is8BitAccessAllowed      = LSA_TRUE;
    RetCode = _EDDP_HALInitFdbTableRam (pDDB, pDPB, RetCode);
    pHal->RamCommonArrayPtr[EDDP_HAL_RAMTYPE_FDB_TABLE] = (EDDP_HAL_LOCAL_RAM_COMP_COMMON_PTR_TYPE)&pHal->RamComponentFdbTable;

    switch(EDDP_HALGetPnipRevision(pDDB))
    {
#ifdef EDDP_CFG_HW_ERTEC200P_SUPPORT
        case EDDP_HAL_HW_PNIP_REV1:
        case EDDP_HAL_HW_PNIP_REV2:
        {
            //-------------------------------------------------------------
            /* PNIP_RAM_APICTRL_RAM_START was replaced by PNIP_R2P2RAM_APICTRL_RAM_START, hardware dependency*/
            pHal->RamComponentApiCtrl.pComponentRamBase         = (EDD_COMMON_MEM_PTR_TYPE)(BaseAddr + PNIP_R2P2_RAM_APICTRL_RAM_START);
            pHal->RamComponentApiCtrl.ComponentRamSize          = RamLengthInfo[EDDP_HAL_RAMTYPE_API_CTRL];
            pHal->RamComponentApiCtrl.ComponentRamCount         = EDDP_HAL_RAMSUB_API_CTRL_COUNT;
            pHal->RamComponentApiCtrl.Is8BitAccessAllowed       = LSA_TRUE;
            RetCode = _EDDP_HALInitApiCtrlRam (pDDB, pDPB, RetCode);
            pHal->RamCommonArrayPtr[EDDP_HAL_RAMTYPE_API_CTRL]  = (EDDP_HAL_LOCAL_RAM_COMP_COMMON_PTR_TYPE)&pHal->RamComponentApiCtrl;
        }
        break;
#endif
        default:
        {
            EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALDeviceOpen(): FATAL -> Revision(0x%X) is not supported!", 
                EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;

    }
    //-------------------------------------------------------------
    /* PNIP_RAM_CMDCTRL_RAM_START was replaced by PNIP_R2P2RAM_CMDCTRL_RAM_START, hardware dependency*/
    pHal->RamComponentCmd.pComponentRamBase        = (EDD_COMMON_MEM_PTR_TYPE)(BaseAddr + PNIP_R2P2_RAM_CMDCTRL_RAM_START);
    pHal->RamComponentCmd.ComponentRamSize         = RamLengthInfo[EDDP_HAL_RAMTYPE_CMD];
    pHal->RamComponentCmd.ComponentRamCount        = EDDP_HAL_RAMSUB_CMD_COUNT;
    pHal->RamComponentCmd.Is8BitAccessAllowed      = LSA_TRUE;
    RetCode = _EDDP_HALInitCmdRam (pDDB, pDPB, RetCode);
    pHal->RamCommonArrayPtr[EDDP_HAL_RAMTYPE_CMD] = (EDDP_HAL_LOCAL_RAM_COMP_COMMON_PTR_TYPE)&pHal->RamComponentCmd;

    //-------------------------------------------------------------
    /* PNIP_RAM_PACKDATA_RAM_START was replaced by PNIP_R2P2_RAM_PACKDATA_RAM_START */
    /* all macros of group PNIP_xxxx_RAM_PACKDATA_RAM_START are checked in eddp_hal_pnip_val.h to have the same value*/
    pHal->RamComponentPackData.pComponentRamBase   = (EDD_COMMON_MEM_PTR_TYPE)(BaseAddr + PNIP_R2P2_RAM_PACKDATA_RAM_START);
    pHal->RamComponentPackData.ComponentRamSize    = RamLengthInfo[EDDP_HAL_RAMTYPE_PACK_DATA];
    pHal->RamComponentPackData.ComponentRamCount   = EDDP_HAL_RAMSUB_PACK_DATA_COUNT;
    pHal->RamComponentPackData.Is8BitAccessAllowed = LSA_TRUE;
    RetCode = _EDDP_HALInitPackDataRam (pDDB, pDPB, RetCode);
    pHal->RamCommonArrayPtr[EDDP_HAL_RAMTYPE_PACK_DATA] = (EDDP_HAL_LOCAL_RAM_COMP_COMMON_PTR_TYPE)&pHal->RamComponentPackData;

    //-------------------------------------------------------------
    /* PNIP_RAM_SYNC_RAM_START was replaced by PNIP_R2P2_RAM_SYNC_RAM_START */
    /* all macros of group PNIP_xxxx_RAM_SYNC_RAM_START are checked in eddp_hal_pnip_val.h to have the same value*/
    pHal->RamComponentSync.pComponentRamBase        = (EDD_COMMON_MEM_PTR_TYPE)(BaseAddr + PNIP_R2P2_RAM_SYNC_RAM_START);
    pHal->RamComponentSync.ComponentRamSize         = RamLengthInfo[EDDP_HAL_RAMTYPE_SYNC];
    pHal->RamComponentSync.ComponentRamCount        = EDDP_HAL_RAMSUB_SYNC_COUNT;
    pHal->RamComponentSync.Is8BitAccessAllowed      = LSA_TRUE;
    RetCode = _EDDP_HALInitSyncRam (pDDB, pDPB, RetCode);
    pHal->RamCommonArrayPtr[EDDP_HAL_RAMTYPE_SYNC]  = (EDDP_HAL_LOCAL_RAM_COMP_COMMON_PTR_TYPE)&pHal->RamComponentSync;

    //-------------------------------------------------------------
    /* PNIP_RAM_SYNC_RAM_START was replaced by PNIP_R2P2_RAM_SYNC_RAM_START */
    /* all macros of group PNIP_xxxx_RAM_STATISTIC_RAM_START are checked in eddp_hal_pnip_val.h to have the same value*/
    pHal->RamComponentStatistic.pComponentRamBase       = (EDD_COMMON_MEM_PTR_TYPE)(BaseAddr + PNIP_R2P2_RAM_STATISTIC_RAM_START);
    pHal->RamComponentStatistic.ComponentRamSize        = RamLengthInfo[EDDP_HAL_RAMTYPE_STATISTIC];
    pHal->RamComponentStatistic.ComponentRamCount       = EDDP_HAL_RAMSUB_STATISTIC_COUNT;
    pHal->RamComponentStatistic.Is8BitAccessAllowed     = LSA_TRUE;
    RetCode = _EDDP_HALInitStatisticRam (pDDB, pDPB, RetCode);
    pHal->RamCommonArrayPtr[EDDP_HAL_RAMTYPE_STATISTIC] = (EDDP_HAL_LOCAL_RAM_COMP_COMMON_PTR_TYPE)&pHal->RamComponentStatistic;

    if (RetCode != EDD_STS_OK)
    {   // ***** set to uninitialized on error
        pHal->pPnipRamBase = LSA_NULL;
    }
    else
    {
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_HALDeviceOpen() -> Systeminformation");
        // do not delete the empty spaces in this trace text
        EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "          PNIP-Version: PnipVersion(0x%X), PnipDevelopment(0x%X), PnipRevision(%d)", 
            PnipVersion, PnipDevelopment, pDDB->Core.Hal.PNIP_Revision);
		EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "          PNIP internal RAM: Host(0x%X), PNIP(0x%X)", pHal->pPnipRamBase, 0);
        pCompPtr = pHal->RamCommonArrayPtr[EDDP_HAL_RAMTYPE_STATISTIC];
        EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "            - STATISTIC      Offs: 0x%X, Host: 0x%X, Len: %d",  (LSA_UINT32)pCompPtr->pComponentRamBase - (LSA_UINT32)pHal->pPnipRamBase, pCompPtr->pComponentRamBase, pCompPtr->ComponentRamSize);
        pCompPtr = pHal->RamCommonArrayPtr[EDDP_HAL_RAMTYPE_SYNC];
        EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "            - SYNC           Offs: 0x%X, Host: 0x%X, Len: %d",  (LSA_UINT32)pCompPtr->pComponentRamBase - (LSA_UINT32)pHal->pPnipRamBase, pCompPtr->pComponentRamBase, pCompPtr->ComponentRamSize);
        pCompPtr = pHal->RamCommonArrayPtr[EDDP_HAL_RAMTYPE_PACK_DATA];
        EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "            - PACK_DATA      Offs: 0x%X, Host: 0x%X, Len: %d",  (LSA_UINT32)pCompPtr->pComponentRamBase - (LSA_UINT32)pHal->pPnipRamBase, pCompPtr->pComponentRamBase, pCompPtr->ComponentRamSize);
        pCompPtr = pHal->RamCommonArrayPtr[EDDP_HAL_RAMTYPE_CMD];
        EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "            - CMD            Offs: 0x%X, Host: 0x%X, Len: %d",  (LSA_UINT32)pCompPtr->pComponentRamBase - (LSA_UINT32)pHal->pPnipRamBase, pCompPtr->pComponentRamBase, pCompPtr->ComponentRamSize);
        pCompPtr = pHal->RamCommonArrayPtr[EDDP_HAL_RAMTYPE_API_CTRL];
        EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "            - API_CTRL       Offs: 0x%X, Host: 0x%X, Len: %d",  (LSA_UINT32)pCompPtr->pComponentRamBase - (LSA_UINT32)pHal->pPnipRamBase, pCompPtr->pComponentRamBase, pCompPtr->ComponentRamSize);

        // only HERA uses EDDP_HAL_RAMTYPE_API_NRT_CTRL
        if( EDDP_HAL_HW_PNIP_REV3 == EDDP_HALGetPnipRevision(pDDB) )
        {
            pCompPtr = pHal->RamCommonArrayPtr[EDDP_HAL_RAMTYPE_API_NRT_CTRL];
            EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "            - API_NRT_CTRL   Offs: 0x%X, Host: 0x%X, Len: %d",  (LSA_UINT32)pCompPtr->pComponentRamBase - (LSA_UINT32)pHal->pPnipRamBase, pCompPtr->pComponentRamBase, pCompPtr->ComponentRamSize);       
            pCompPtr = pHal->RamCommonArrayPtr[EDDP_HAL_RAMTYPE_API_PST];
            EDDP_CORE_TRACE_03(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "            - API_PST        Offs: 0x%X, Host: 0x%X, Len: %d",  (LSA_UINT32)pCompPtr->pComponentRamBase - (LSA_UINT32)pHal->pPnipRamBase, pCompPtr->pComponentRamBase, pCompPtr->ComponentRamSize);
        }
        pCompPtr = pHal->RamCommonArrayPtr[EDDP_HAL_RAMTYPE_FDB_TABLE];
        EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "            - FDB_TABLE      Offs: 0x%X, Host: 0x%X, Len: %d",  (LSA_UINT32)pCompPtr->pComponentRamBase - (LSA_UINT32)pHal->pPnipRamBase, pCompPtr->pComponentRamBase, pCompPtr->ComponentRamSize);
        pCompPtr = pHal->RamCommonArrayPtr[EDDP_HAL_RAMTYPE_VLAN_TABLE];                                 
        EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "            - VLAN_TABLE     Offs: 0x%X, Host: 0x%X, Len: %d",  (LSA_UINT32)pCompPtr->pComponentRamBase - (LSA_UINT32)pHal->pPnipRamBase, pCompPtr->pComponentRamBase, pCompPtr->ComponentRamSize);
        pCompPtr = pHal->RamCommonArrayPtr[EDDP_HAL_RAMTYPE_QUEUE_CTRL];                                 
        EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "            - QUEUE_CTRL     Offs: 0x%X, Host: 0x%X, Len: %d",  (LSA_UINT32)pCompPtr->pComponentRamBase - (LSA_UINT32)pHal->pPnipRamBase, pCompPtr->pComponentRamBase, pCompPtr->ComponentRamSize);
        pCompPtr = pHal->RamCommonArrayPtr[EDDP_HAL_RAMTYPE_QUEUE_DATA];                                 
        EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "            - QUEUE_DATA     Offs: 0x%X, Host: 0x%X, Len: %d",  (LSA_UINT32)pCompPtr->pComponentRamBase - (LSA_UINT32)pHal->pPnipRamBase, pCompPtr->pComponentRamBase, pCompPtr->ComponentRamSize);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_HALDeviceOpen() <- Systeminformation");

        EDDP_HALTraceLayout(pDDB);
    }

    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_HALDeviceOpen(): RetCode=0x%X", RetCode);
    
    return (RetCode);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_HALWaitPNIPInitDone                    +*/
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
/*+               EDD_STS_ERR_HW                                            +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Waits for INITDone of PNIP for 10ms. On error             +*/
/*+               returns EDD_STS_ERR_HW                                    +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HALWaitPNIPInitDone(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DSB_PTR_TYPE     pDSB)
{
    EDD_RSP         Response;
    LSA_BOOL        InitOK;
    LSA_UINT32      WaitForInit;
    LSA_UINT32      InitDone;

#define EDDP_PNIP_INITDONE_MSK  \
    (PNIP_A0_REG_INITDONE__MSK_API_DONE         | \
     PNIP_A0_REG_INITDONE__MSK_SYNC_DONE        | \
     PNIP_A0_REG_INITDONE__MSK_CMD_DONE         | \
     PNIP_A0_REG_INITDONE__MSK_PACK_DONE        | \
     PNIP_A0_REG_INITDONE__MSK_STATISTIC_DONE   | \
     PNIP_F0_REG_INITDONE__MSK_QUEUE_DONE       | \
     PNIP_F0_REG_INITDONE__MSK_VLAN_DONE        | \
     PNIP_A0_REG_INITDONE__MSK_FDB_DONE)

    // ***** Check/Wait for INITDone of PNIP
    WaitForInit = 1000; // 1000 * 10us = 10ms
    InitOK      = LSA_FALSE;

    do
    {
        InitDone = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_INITDONE);

        if ((InitDone & EDDP_PNIP_INITDONE_MSK) == EDDP_PNIP_INITDONE_MSK)
        {   // ***** successful
            InitOK = LSA_TRUE;
            break;
        }
        else
        {   // ***** failure
            EDDP_HALWaitNsTicks(pDDB, EDDP_HAL_10US_TO); // wait 10us
            WaitForInit--;
        }
    } while (WaitForInit);

    if (!InitOK)
    {
        // ***** failure
        Response = EDD_STS_ERR_HW;
        EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_HALWaitPNIPInitDone(): ERROR -> PNIP-InitDone failed! InitDone=0x%X", 
            InitDone);
    }
    else
    {
        // ***** successful
        Response = EDD_STS_OK;
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_HALWaitPNIPInitDone(): PNIP-InitDone successful");
    }

    LSA_UNUSED_ARG(pDSB);    // handle unused parameter

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_HALDeviceSetup                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             EDDP_UPPER_DSB_PTR_TYPE        pDSB         +*/
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
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_HW                                            +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Setup device for HAL.                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HALDeviceSetup(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_UPPER_DSB_PTR_TYPE     pDSB)
{
    EDD_RSP                         Response;
    EDD_COMMON_MEM_U32_PTR_TYPE     pResetChk;

    EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_HALDeviceSetup(pDDB=0x%X, pDSB=0x%X)", 
        pDDB, pDSB);

    Response = EDDP_HALWaitPNIPInitDone (pDDB, pDSB);
    /*PNTASK review*/

    if (Response == EDD_STS_OK)
    {
        Response = _EDDP_HALCheckResetExecution (pDDB);

        // ***** request RAM-pointers from user and execute RAM-Test
        if (Response != EDD_STS_OK)
        {
            EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_HALDeviceSetup(): expected no RESET signature !!!");
        }
    }

    if (Response == EDD_STS_OK)
    {
        Response = _EDDP_HALSetupRamBorders (pDDB);
    }

    if (Response == EDD_STS_OK )
    {
        pResetChk = (EDD_COMMON_MEM_U32_PTR_TYPE) EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_API_CTRL, EDDP_HAL_RAMSUB_APICTRL_RESET_CHECK, LSA_NULL);

        if (pResetChk != LSA_NULL)
        {
            EDDP_HAL_WR_RESET_CHK(pResetChk);
        }
        //EDDP_HALTraceLayout(pDDB);
    }

    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_HALDeviceSetup(): Response=0x%X", Response);

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_HALDeviceShutdown                      +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Shutdown Device for HAL                                   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HALDeviceShutdown(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_HALDeviceShutdown()");

    LSA_UNUSED_ARG(pDDB);    // handle unused parameter

    EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_HALDeviceShutdown()");

    return (EDD_STS_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_HALMemset                              +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB          +*/
/*+                             LSA_UINT32                    RAMType       +*/
/*+                             LSA_UINT32                    RAMSubType    +*/
/*+                             LSA_UINT32                    IntOffs       +*/
/*+                             LSA_UINT32                    Length        +*/
/*+                             LSA_UINT8                     Pattern       +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  RAMType    : selects specific memory range                       (in)  +*/
/*+  RAMSubType : selects specific memory in sub module               (in)  +*/
/*+  IntOffs    : offset within selected sub ram                      (in)  +*/
/*+  Length     : length from offset that is to be filled             (in)  +*/
/*+  Pattern    : byte for filling selected memory range              (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Fill selected memory space                                +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HALMemset(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  RAMType,
    LSA_UINT32                  RAMSubType,
    LSA_UINT32                  IntOffs,
    LSA_UINT32                  Length,
    LSA_UINT8                   Pattern)
{
    EDD_RSP                                     Response = EDD_STS_OK;
    LSA_UINT32                                  CompCount;
    LSA_UINT32                                  MemSetAddr;
    EDDP_HAL_RAMLAYOUT_PTR_TYPE	                pHal;
    EDDP_HAL_LOCAL_RAM_SUB_COMP_PTR_TYPE        pCompArray;
    EDDP_HAL_LOCAL_RAM_COMP_COMMON_PTR_TYPE     pCommonPtr;

    //EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_HALMemset()");

    EDDP_ASSERT_NO_NULL_PTR(pDDB);

    pHal = &pDDB->Core.Hal.HalRamLayout;

    if  (LSA_HOST_PTR_ARE_EQUAL(pHal->pPnipRamBase, LSA_NULL))
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_HALMemset(): ERROR -> pPnipRamBase is NULL");
        return (EDD_STS_ERR_PARAM);
    }

	if ((EDDP_HAL_RAMTYPE_API_NRT_CTRL == RAMType) &&
		(EDDP_HAL_HW_HERA != EDDP_HALGetChipVersion(pDDB))
		)
	{
		EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
		EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALMemset(): FATAL -> RAMTYPE is EDDP_HAL_RAMTYPE_API_NRT_CTRL and Chip is not HERA");
		EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
	}

    pCompArray = _EDDP_HALGetCompArray(pDDB, RAMType, &CompCount);
    
    if  (LSA_HOST_PTR_ARE_EQUAL(pCompArray, LSA_NULL))
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_HALMemset(): ERROR -> pCompArray is NULL! RAMType=%d", 
            RAMType);
        return (EDD_STS_ERR_PARAM);
    }
	
    if ( (EDDP_HAL_HW_HERA == EDDP_HALGetChipVersion(pDDB)) &&
         ( (EDDP_HAL_RAMSUB_APICTRL_NRT_COMMON_CODE         == RAMSubType) ||
           (EDDP_HAL_RAMSUB_APICTRL_NRT_IFA_MGM             == RAMSubType) ||
           (EDDP_HAL_RAMSUB_APICTRL_NRT_IFA_DCPPORT_FILTER  == RAMSubType) || // HERADevMark TH
           (EDDP_HAL_RAMSUB_APICTRL_NRT_IFA_DCPHELLO_FILTER == RAMSubType) // HERADevMark TH
         )
       )
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALMemset(): FATAL -> RAMTYPE is EDDP_HAL_RAMTYPE_API_CTRL with HERA as chip does not support SubRAMType");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    if ( (EDDP_HAL_HW_HERA                                      == EDDP_HALGetChipVersion(pDDB)) &&
         (EDDP_HAL_RAMTYPE_API_NRT_CTRL                         == RAMType) &&
         (EDDP_HAL_RAMSUB_API_NRT_CTRL_NRT_COMMON_CODE          != RAMSubType) &&
         (EDDP_HAL_RAMSUB_API_NRT_CTRL_NRT_IFA_MGM              != RAMSubType)
       )
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALMemset(): FATAL -> HERA is used with RAMType API_NRT_CTRL with not supported RAMSubType");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    if ( (EDDP_HAL_HW_HERA != EDDP_HALGetChipVersion(pDDB)) &&
         (EDDP_HAL_RAMTYPE_API_NRT_CTRL == RAMType) &&
         ( (EDDP_HAL_RAMSUB_API_NRT_CTRL_NRT_COMMON_CODE         == RAMSubType) ||
           (EDDP_HAL_RAMSUB_API_NRT_CTRL_NRT_IFA_MGM             == RAMSubType)
         )
       )
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALMemset(): FATAL -> Rev1/2 does not support this SubRAMType");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    if (RAMSubType < CompCount)
    {
        if (Length > pCompArray[RAMSubType].SubComponentRamSize)
        {
            Length = pCompArray[RAMSubType].SubComponentRamSize;
        }

        if (Length)
        {
            if ((IntOffs+Length) <= pCompArray[RAMSubType].SubComponentRamSize)
            {
                MemSetAddr  = (LSA_UINT32)pCompArray[RAMSubType].pSubComponentRamBase;
                MemSetAddr += IntOffs;
                pCommonPtr  = _EDDP_HALGetCommonAccessPtr(pDDB, RAMType);

                if (LSA_HOST_PTR_ARE_EQUAL(pCommonPtr, LSA_NULL))
                {
                    EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
                    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_HALMemset(): ERROR -> pCommonPtr is NULL! RAMType=%d", 
                        RAMType);
                    return (EDD_STS_ERR_PARAM);
                }

                if (pCommonPtr->Is8BitAccessAllowed)
                {
                    EDDP_MEMSET_COMMON((EDD_COMMON_MEM_PTR_TYPE)MemSetAddr, Pattern, Length);
                }
                else
                {
                    Length      = (Length+3)/4;
                    CompCount   = ((Pattern<<24)|(Pattern<<16)|(Pattern<<8)|(Pattern<<0));

                    while (Length)
                    {
                        *((LSA_UINT32 EDDP_LOCAL_MEM_ATTR *)MemSetAddr) = CompCount;
                        MemSetAddr += 4;
                        Length --;
                    }
                }
            }
            else
            {
                EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
                EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_HALMemset(): ERROR -> (IntOffs+Length)(%d) > SubComponentRamSize(%d)", 
                    (IntOffs+Length), pCompArray[RAMSubType].SubComponentRamSize);
                return (EDD_STS_ERR_PARAM);
            }
        }
    }
    else
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_HALMemset(): ERROR -> RAMSubType(%d) >= CompCount(%d)! RAMType=%d", 
            RAMSubType, CompCount, RAMType);
        return (EDD_STS_ERR_PARAM);
    }

    //EDDP_CORE_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_HALMemset()");

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_HALTraceLayout()                       +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB          +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                (in)     +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function can be called from CORE to print RAM layout +*/
/*+               settings into the trace file (LSA_TRACE_LEVEL_CHAT)       +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HALTraceLayout(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT32                                  Idx;
    EDDP_HAL_RAMLAYOUT_PTR_TYPE	                pHal;
    EDDP_HAL_LOCAL_RAM_COMP_COMMON_PTR_TYPE     pCompPtr;

    EDDP_ASSERT_NO_NULL_PTR(pDDB);

    EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_HALTraceLayout()");

    pHal = &pDDB->Core.Hal.HalRamLayout;

    if (LSA_HOST_PTR_ARE_EQUAL(pHal->pPnipRamBase, LSA_NULL))
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_HALTraceLayout(): ERROR -> pPnipRamBase is NULL");
        return (EDD_STS_ERR_PARAM);
    }

    // ***** print in reverse order because the external excel sheet read these information from the LOG file to build a diagram graph
    for ( Idx = (EDDP_HAL_RAMTYPE_COMP_COUNT-1); Idx ; Idx-- )
    {
        // for HERA EDDP_HAL_RAMTYPE_API_NRT_CTRL is not used
        if(  !( (EDDP_HAL_HW_PNIP_REV3 != EDDP_HALGetPnipRevision(pDDB)) && (EDDP_HAL_RAMTYPE_API_NRT_CTRL == Idx) ) 
          && !( (EDDP_HAL_HW_PNIP_REV3 != EDDP_HALGetPnipRevision(pDDB)) && (EDDP_HAL_RAMTYPE_API_PST == Idx) ) 
          )
        {
            pCompPtr = pHal->RamCommonArrayPtr[Idx];
            _EDDP_HALTraceCompLayout(pDDB, pCompPtr, Idx, pHal->SetupDone);
        }
    }

    EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_HALTraceLayout()");

    return (EDD_STS_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname       :    EDDP_HALTestRamBorder                          +*/
/*+  Input/Output       :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB             +*/
/*+                          LSA_UINT32                    RAMType          +*/
/*+                          LSA_UINT32                    RAMSubType       +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB             : Pointer to DeviceDescriptionBlock             (in)  +*/
/*+  RAMType          : selects specific memory range                 (in)  +*/
/*+  RAMSubType       : selects specific memory in sub module         (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function can be used to test initialized border      +*/
/*+               value. Function returns EDD_STS_OK if border check is not +*/
/*+               enabled or if found expected value on this RAM cell.      +*/
/*+               EDD_STS_ERR_EXCP will be returned if overwrite found.     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HALTestRamBorder(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  RAMType,
    LSA_UINT32                  RAMSubType)
{
    LSA_UINT32                              CompCount;
    EDDP_HAL_RAMLAYOUT_PTR_TYPE	            pHal;
    EDDP_HAL_LOCAL_RAM_SUB_COMP_PTR_TYPE    pCompArray;

    EDDP_ASSERT_NO_NULL_PTR(pDDB);

    pHal = &pDDB->Core.Hal.HalRamLayout;

    if (LSA_HOST_PTR_ARE_EQUAL(pHal->pPnipRamBase, LSA_NULL))
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_SEQUENCE);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_HALTestRamBorder(): ERROR -> pPnipRamBase is NULL");
        return (EDD_STS_ERR_SEQUENCE);
    }

	if ((EDDP_HAL_RAMTYPE_API_NRT_CTRL == RAMType) &&
		(EDDP_HAL_HW_HERA != EDDP_HALGetChipVersion(pDDB))
		)
	{
		EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
		EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALTestRamBorder(): FATAL -> RAMTYPE is EDDP_HAL_RAMTYPE_API_NRT_CTRL and Chip is not HERA");
		EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
	}

    pCompArray = _EDDP_HALGetCompArray(pDDB, RAMType, &CompCount);

    if  (LSA_HOST_PTR_ARE_EQUAL(pCompArray, LSA_NULL))
    {
        return (EDD_STS_ERR_PARAM);
    }
	
    if ( (EDDP_HAL_HW_HERA == EDDP_HALGetChipVersion(pDDB)) &&
         ( (EDDP_HAL_RAMSUB_APICTRL_NRT_COMMON_CODE         == RAMSubType) ||
           (EDDP_HAL_RAMSUB_APICTRL_NRT_IFA_MGM             == RAMSubType) ||
           (EDDP_HAL_RAMSUB_APICTRL_NRT_IFA_DCPPORT_FILTER  == RAMSubType) || // HERADevMark TH
           (EDDP_HAL_RAMSUB_APICTRL_NRT_IFA_DCPHELLO_FILTER == RAMSubType) // HERADevMark TH
         )
       )
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALTestRamBorder(): FATAL -> RAMTYPE is EDDP_HAL_RAMTYPE_API_CTRL with HERA as chip does not support SubRAMType");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    if ( (EDDP_HAL_HW_HERA                                      == EDDP_HALGetChipVersion(pDDB)) &&
         (EDDP_HAL_RAMTYPE_API_NRT_CTRL                         == RAMType) &&
         (EDDP_HAL_RAMSUB_API_NRT_CTRL_NRT_COMMON_CODE          != RAMSubType) &&
         (EDDP_HAL_RAMSUB_API_NRT_CTRL_NRT_IFA_MGM              != RAMSubType)
       )
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALTestRamBorder(): FATAL -> HERA is used with RAMType API_NRT_CTRL with not supported RAMSubType");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    if ( (EDDP_HAL_HW_HERA != EDDP_HALGetChipVersion(pDDB)) &&
         (EDDP_HAL_RAMTYPE_API_NRT_CTRL == RAMType) &&
         ( (EDDP_HAL_RAMSUB_API_NRT_CTRL_NRT_COMMON_CODE         == RAMSubType) ||
           (EDDP_HAL_RAMSUB_API_NRT_CTRL_NRT_IFA_MGM             == RAMSubType)
         )
       )
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALTestRamBorder(): FATAL -> Rev1/2 does not support this SubRAMType");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    if (RAMSubType < CompCount)
    {
        if (pCompArray->RamSubComponentUseBorderCheck)
        {
            if  (LSA_HOST_PTR_ARE_EQUAL(pCompArray->pSubComponentBorderCheck, LSA_NULL))
            {
                return (EDD_STS_ERR_PARAM);
            }

            CompCount = EDDP_HAL_MEM32_READ(pDDB, pCompArray->pSubComponentBorderCheck);

            if (CompCount != EDDP_HAL_BORDER_TOKEN)
            {
                EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_EXCP);
                EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_HALTestRamBorder(): ERROR -> CompCount(0x%X) != EDDP_HAL_BORDER_TOKEN(0x%X)", 
                    CompCount, EDDP_HAL_BORDER_TOKEN);
                return (EDD_STS_ERR_EXCP);
            }
        }
        return (EDD_STS_OK);
    }

    return (EDD_STS_ERR_PARAM);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname       :    EDDP_HALGetVersion                             +*/
/*+  Input/Output       :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB             +*/
/*+                          EDD_COMMON_MEM_U32_PTR_TYPE   pPnipVersion     +*/
/*+                          EDD_COMMON_MEM_U32_PTR_TYPE   pPnipDevelopment +*/
/*+                                                                         +*/
/*+  Result             :    EDD_RSP                                        +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB             : Pointer to DeviceDescriptionBlock             (in)  +*/
/*+  pPnipVersion     : buffer for IP_Version                         (out) +*/
/*+  pPnipDevelopment : buffer for IP_Development                     (out) +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_SEQUENCE                                      +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function can be called after EDDP_HALInit() reading  +*/
/*+               out ASIC IP- and Development Version                      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HALGetVersion(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDD_COMMON_MEM_U32_PTR_TYPE     pPnipVersion,
    EDD_COMMON_MEM_U32_PTR_TYPE     pPnipDevelopment)
{
    EDDP_ASSERT_NO_NULL_PTR(pDDB);

    if (LSA_HOST_PTR_ARE_EQUAL(pPnipVersion, LSA_NULL))
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_HALGetVersion(): ERROR -> pPnipVersion is NULL");
        return (EDD_STS_ERR_PARAM);
    }

    if (LSA_HOST_PTR_ARE_EQUAL(pPnipDevelopment, LSA_NULL))
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_HALGetVersion(): ERROR -> pPnipDevelopment is NULL");
        return (EDD_STS_ERR_PARAM);
    }

    if (LSA_HOST_PTR_ARE_EQUAL(pDDB->Core.Hal.HalRamLayout.pPnipRamBase, LSA_NULL))
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_SEQUENCE);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_HALGetVersion(): ERROR -> pPnipRamBase is NULL");
        return (EDD_STS_ERR_SEQUENCE);
    }

    *pPnipVersion       = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_IP_VERSION);       // @ 0x0000
    *pPnipDevelopment   = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_IP_DEVELOPMENT);   // @ 0x0004

    return (EDD_STS_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname       :    EDDP_HALGetPnipBaseAddress                     +*/
/*+  Input/Output       :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB             +*/
/*+                                                                         +*/
/*+  Result             :    EDD_COMMON_MEM_PTR_TYPE                        +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB             : Pointer to DeviceDescriptionBlock             (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_COMMON_MEM_PTR_TYPE or LSA_NULL on error              +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function can be called after EDDP_HALInit() reading  +*/
/*+               BaseAddr of PNIP memory                                   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_COMMON_MEM_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_HALGetPnipBaseAddress(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDDP_ASSERT_NO_NULL_PTR(pDDB);
    return (pDDB->Core.Hal.HalRamLayout.pPnipRamBase);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname       :    EDDP_HALGetRAMBaseAddress                      +*/
/*+  Input/Output       :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB             +*/
/*+                          LSA_UINT32                    RAMType          +*/
/*+                          EDD_COMMON_MEM_U32_PTR_TYPE   pRAMSize         +*/
/*+                                                                         +*/
/*+  Result             :    EDD_COMMON_MEM_PTR_TYPE                        +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB             : Pointer to DeviceDescriptionBlock             (in)  +*/
/*+  RAMType          : selects specific memory range                 (in)  +*/
/*+  pRAMSize         : buffer to return size information [optional]  (out) +*/
/*+                                                                         +*/
/*+  Results    : EDD_COMMON_MEM_PTR_TYPE or LSA_NULL on error              +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function can be called after EDDP_HALInit() reading  +*/
/*+               out starting address and size for sel. memory space       +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_COMMON_MEM_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_HALGetRAMBaseAddress(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_UINT32            	        RAMType,
    EDD_COMMON_MEM_U32_PTR_TYPE     pRAMSize)
{
    EDD_COMMON_MEM_PTR_TYPE         pRamBase;
    EDDP_HAL_RAMLAYOUT_PTR_TYPE     pHal;

    EDDP_ASSERT_NO_NULL_PTR(pDDB);

    pHal = &pDDB->Core.Hal.HalRamLayout;

    if (LSA_HOST_PTR_ARE_EQUAL(pHal->pPnipRamBase, LSA_NULL))
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_HALGetRAMBaseAddress(): ERROR -> pPnipRamBase is NULL");
        return (LSA_NULL);
    }

    if ( (EDDP_HAL_RAMTYPE_API_NRT_CTRL == RAMType) &&
         (EDDP_HAL_HW_HERA != EDDP_HALGetChipVersion(pDDB))
       )
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALGetRAMBaseAddress(): FATAL -> RAMTYPE is EDDP_HAL_RAMTYPE_API_NRT_CTRL and Chip is not HERA");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    } 

    pRamBase = _EDDP_HALGetRamBase (pDDB, RAMType, pRAMSize);

    return (pRamBase);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname       :    EDDP_HALGetRAMSubAddress                       +*/
/*+  Input/Output       :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB             +*/
/*+                          LSA_UINT32                    RAMType          +*/
/*+                          LSA_UINT32                    RAMSubType       +*/
/*+                          EDD_COMMON_MEM_U32_PTR_TYPE   pRAMSize         +*/
/*+                                                                         +*/
/*+  Result             :    EDD_COMMON_MEM_PTR_TYPE                        +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB             : Pointer to DeviceDescriptionBlock             (in)  +*/
/*+  RAMType          : selects specific memory range                 (in)  +*/
/*+  RAMSubType       : selects specific memory in sub module         (in)  +*/
/*+  pRAMSize         : buffer to return size information             (out) +*/
/*+                                                                         +*/
/*+  Results    : valid pointer of type EDD_COMMON_MEM_PTR_TYPE or LSA_NULL +*/
/*+               if RAM not used (e.g. RAM-lenght=0) or on error           +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function can be called after EDDP_HALInit() reading  +*/
/*+               out starting address and size for sel. sub memory space   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_COMMON_MEM_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_HALGetRAMSubAddress(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_UINT32                      RAMType,
    LSA_UINT32                      RAMSubType,
    EDD_COMMON_MEM_U32_PTR_TYPE     pRAMSize)
{
    LSA_UINT32                              CompCount = 0;
    EDDP_HAL_RAMLAYOUT_PTR_TYPE	            pHal;
    EDD_COMMON_MEM_PTR_TYPE                 pRamSubBase;
    EDDP_HAL_LOCAL_RAM_SUB_COMP_PTR_TYPE    pCompArray = LSA_NULL;

    EDDP_ASSERT_NO_NULL_PTR(pDDB);

    pHal = &pDDB->Core.Hal.HalRamLayout;

    if (LSA_HOST_PTR_ARE_EQUAL(pHal->pPnipRamBase, LSA_NULL))
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_HALGetRAMSubAddress(): ERROR -> pPnipRamBase is NULL");
        return (LSA_NULL);
    }

    if (pRAMSize != LSA_NULL)
    {   // parameter is optional
        *pRAMSize = 0;
    }

	if ((EDDP_HAL_RAMTYPE_API_NRT_CTRL == RAMType) &&
		(EDDP_HAL_HW_HERA != EDDP_HALGetChipVersion(pDDB))
		)
	{
		EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
		EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALGetRAMSubAddress(): FATAL -> RAMTYPE is EDDP_HAL_RAMTYPE_API_NRT_CTRL and Chip is not HERA");
		EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
	}

    pCompArray = _EDDP_HALGetCompArray(pDDB, RAMType, &CompCount);
    
    if (LSA_HOST_PTR_ARE_EQUAL(pCompArray, LSA_NULL))
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_HALGetRAMSubAddress(): ERROR ->  pCompArray is NULL! RAMType=%d", 
            RAMType);
        return (LSA_NULL);
    }
	
	if ( (EDDP_HAL_HW_HERA == EDDP_HALGetChipVersion(pDDB)) &&
         ( (EDDP_HAL_RAMSUB_APICTRL_NRT_COMMON_CODE         == RAMSubType) ||
           (EDDP_HAL_RAMSUB_APICTRL_NRT_IFA_MGM             == RAMSubType) ||
           (EDDP_HAL_RAMSUB_APICTRL_NRT_IFA_DCPPORT_FILTER  == RAMSubType) ||
           (EDDP_HAL_RAMSUB_APICTRL_NRT_IFA_DCPHELLO_FILTER == RAMSubType)
         )
       )
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALGetRAMSubAddress(): FATAL -> RAMTYPE is EDDP_HAL_RAMTYPE_API_CTRL with HERA as chip does not support SubRAMType");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    if ( ( EDDP_HAL_HW_ERTEC200P == EDDP_HALGetChipVersion(pDDB) ) &&
         ( EDDP_HAL_RAMTYPE_API_NRT_CTRL == RAMType ) 
       )
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALGetRAMSubAddress(): FATAL -> Rev1/2 does not support this SubRAMType");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    if ( (EDDP_HAL_HW_HERA                                      == EDDP_HALGetChipVersion(pDDB)) &&
         (EDDP_HAL_RAMTYPE_API_NRT_CTRL                         == RAMType) &&
         (EDDP_HAL_RAMSUB_API_NRT_CTRL_RESET_CHECK              != RAMSubType) &&
         (EDDP_HAL_RAMSUB_API_NRT_CTRL_NRT_COMMON_CODE          != RAMSubType) &&
         (EDDP_HAL_RAMSUB_API_NRT_CTRL_NRT_IFA_MGM              != RAMSubType)
       )
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALGetRAMSubAddress(): FATAL -> HERA is used with RAMType API_NRT_CTRL with not supported RAMSubType");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    if (RAMSubType < CompCount)
    {
        if (pRAMSize != LSA_NULL)
        {   // parameter is optional
            *pRAMSize = pCompArray[RAMSubType].SubComponentRamSize; // reserved space from SubBasePtr (without alignment padding and boardercheck)
        }
        pRamSubBase = pCompArray[RAMSubType].pSubComponentRamBase;
    }
    else
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_HALGetRAMSubAddress(): ERROR -> RAMSubType(%d) >= CompCount(%d)! RAMType=%d", 
            RAMSubType, CompCount, RAMType);
        pRamSubBase = LSA_NULL;
    }

    return (pRamSubBase);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname       :    EDDP_HALGetPnipOffsetToStart                   +*/
/*+  Input/Output       :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB             +*/
/*+                          EDD_COMMON_MEM_PTR_TYPE       pRAMAddr         +*/
/*+                                                                         +*/
/*+  Result             :    LSA_UINT32                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB             : Pointer to DeviceDescriptionBlock             (in)  +*/
/*+  pRAMAddr         : address position to build offset information  (in)  +*/
/*+                                                                         +*/
/*+  Results    : offset or EDDP_HAL_RAM_BAD_PNIP_ADDR on error             +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function can be called after EDDP_HALInit() reading  +*/
/*+               out offset to BaseAddr of PNIP memory (PNIP view)         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_HALGetPnipOffsetToStart(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_COMMON_MEM_PTR_TYPE     pRAMAddr)
{
    LSA_UINT32                      RetOffs;
    EDDP_HAL_RAMLAYOUT_PTR_TYPE     pHal;

    EDDP_ASSERT_NO_NULL_PTR(pDDB);

    if (LSA_HOST_PTR_ARE_EQUAL(pRAMAddr, LSA_NULL))
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_HALGetPnipOffsetToStart(): ERROR -> pRAMAddr is NULL");
        return (EDDP_HAL_RAM_BAD_PNIP_ADDR);
    }

    pHal = &pDDB->Core.Hal.HalRamLayout;

    if (LSA_HOST_PTR_ARE_EQUAL(pHal->pPnipRamBase, LSA_NULL))
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_HALGetPnipOffsetToStart(): ERROR -> pPnipRamBase is NULL");
        return (EDDP_HAL_RAM_BAD_PNIP_ADDR);
    }

    if ((LSA_UINT32)pRAMAddr >= (LSA_UINT32)pHal->pPnipRamBase)
    {
        RetOffs = ( (LSA_UINT32)pRAMAddr - (LSA_UINT32)pHal->pPnipRamBase );
        // maybe test if offset bigger than PNIP RAM size
    }
    else
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_HALGetPnipOffsetToStart(): ERROR -> pRAMAddr(0x%X) < pPnipRamBase(0x%X)", 
            pRAMAddr, pHal->pPnipRamBase);
        RetOffs = EDDP_HAL_RAM_BAD_PNIP_ADDR;   // default error
    }

    return (RetOffs);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname       :    EDDP_HALGetPnipRAMOffsetToRAM                  +*/
/*+  Input/Output       :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB             +*/
/*+                          LSA_UINT32                    RAMType          +*/
/*+                          EDD_COMMON_MEM_PTR_TYPE       pRAMAddr         +*/
/*+                                                                         +*/
/*+  Result             :    LSA_UINT32                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB             : Pointer to DeviceDescriptionBlock             (in)  +*/
/*+  RAMType          : selects specific memory range                 (in)  +*/
/*+  pRAMAddr         : address position to build offset information  (in)  +*/
/*+                                                                         +*/
/*+  Results    : offset or EDDP_HAL_RAM_BAD_PNIP_ADDR on error             +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function can be called after EDDP_HALInit() reading  +*/
/*+               out offset to sel. ModuleAddr of PNIP memory (PNIP view)  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_HALGetPnipRAMOffsetToRAM(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  RAMType,
    EDD_COMMON_MEM_PTR_TYPE     pRAMAddr)
{
    LSA_UINT32                      RetOffs;
    LSA_UINT32                      RamSize;
    EDD_COMMON_MEM_PTR_TYPE         pRamBase;
    EDDP_HAL_RAMLAYOUT_PTR_TYPE     pHal;

    EDDP_ASSERT_NO_NULL_PTR(pDDB);

    if (LSA_HOST_PTR_ARE_EQUAL(pRAMAddr, LSA_NULL))
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_HALGetPnipRAMOffsetToRAM(): ERROR -> pRAMAddr is NULL");
        return (EDDP_HAL_RAM_BAD_PNIP_ADDR);
    }

    pHal = &pDDB->Core.Hal.HalRamLayout;

    if (LSA_HOST_PTR_ARE_EQUAL(pHal->pPnipRamBase, LSA_NULL))
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_HALGetPnipRAMOffsetToRAM(): ERROR -> pPnipRamBase is NULL");
        return (EDDP_HAL_RAM_BAD_PNIP_ADDR);
    }

    if ( (EDDP_HAL_RAMTYPE_API_NRT_CTRL == RAMType) &&
         (EDDP_HAL_HW_HERA != EDDP_HALGetChipVersion(pDDB))
       )
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALGetPnipRAMOffsetToRAM(): FATAL -> RAMTYPE is EDDP_HAL_RAMTYPE_API_NRT_CTRL and Chip is not HERA");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    pRamBase = _EDDP_HALGetRamBase(pDDB, RAMType, &RamSize);

    if (   (LSA_HOST_PTR_ARE_EQUAL(pRAMAddr, LSA_NULL))
        || (LSA_HOST_PTR_ARE_EQUAL(pRamBase, LSA_NULL))
        || (pRAMAddr < pRamBase)
       )
    {
        EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
        EDDP_CORE_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_HALGetPnipRAMOffsetToRAM(): ERROR -> some parameters are wrong: pRAMAddr(0x%X), pRamBase(0x%X)! RAMType(%d), RamSize(0x%X)", 
            pRAMAddr, pRamBase, RAMType, RamSize);
        RetOffs = EDDP_HAL_RAM_BAD_PNIP_ADDR;   // range error or unknown parameter
    }
    else
    {
        RetOffs = ( ((LSA_UINT32)pRAMAddr - (LSA_UINT32)pRamBase) );

        if (RetOffs >= RamSize)
        {
            EDDP_DBG_SET_DETAIL_ERR(pDDB,EDD_STS_ERR_PARAM);
            EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_HALGetPnipRAMOffsetToRAM(): ERROR -> RetOffs(0x%X) >= RamSize(0x%X)! RAMType(%d)", 
                RetOffs, RamSize, RAMType);
            RetOffs = EDDP_HAL_RAM_BAD_PNIP_ADDR;   // out of sub module range
        }
    }

    return (RetOffs);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname       :    EDDP_HALWaitNsTicks                            +*/
/*+  Input/Output       :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB             +*/
/*+                          LSA_UINT32                    nTicks           +*/
/*+                                                                         +*/
/*+  Result             :    LSA_VOID                                       +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB             : Pointer to DeviceDescriptionBlock             (in)  +*/
/*+  nTicks           : wait time in ns (granularity is 4ns !!!)      (in)  +*/
/*+                                                                         +*/
/*+  NOTE:  avoid values for nTicks higher than UINT32_MAX/2-1              +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function can be called to wait for a defined time.   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_HALWaitNsTicks(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  nTicks)
{
    LSA_UINT32      Diff, Start;

    Start = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_RCT_DELAY);

    do
    {
        Diff = ( EDDP_HAL_REG32_READ(pDDB, PNIP_REG_RCT_DELAY) - Start );   // two lower bits cut off by HW
    } while (Diff < nTicks);    // Note: 0..3 --> waits 4ns

    return;
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname       :    EDDP_HALGetNsCounter                           +*/
/*+  Input/Output       :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB             +*/
/*+                                                                         +*/
/*+  Result             :    LSA_UINT32                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB             : Pointer to DeviceDescriptionBlock             (in)  +*/
/*+                                                                         +*/
/*+  Results    : snapshot of running timer (two lower bits cut off by HW)  +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function can be called to read a snapshot of the     +*/
/*+               running counter for detecting a timeout in combination    +*/
/*+               with EDDP_HALDiffNsCounter()                              +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_HALGetNsCounter(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    // ***** 250MHz free running counter  --> granularity is 4ns, range 0ns .. 4,29s
    return (EDDP_HAL_REG32_READ(pDDB, PNIP_REG_RCT_DELAY));
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname       :    EDDP_HALDiffNsCounter                          +*/
/*+  Input/Output       :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB             +*/
/*+                          LSA_UINT32                    nStart           +*/
/*+                                                                         +*/
/*+  Result             :    LSA_UINT32                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB             : Pointer to DeviceDescriptionBlock             (in)  +*/
/*+  nStart           : snapshot of running timer                     (in)  +*/
/*+                                                                         +*/
/*+  Results    : difference of current timer to nStart, granularity 4ns !  +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function can be called to calculate the timeout      +*/
/*+               You can use EDDP_HAL_XXX_TO                               +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_HALDiffNsCounter(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  nStart)
{
    LSA_UINT32 Curr = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_RCT_DELAY);
    return (Curr - nStart);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname       :    EDDP_HALGetPnipRevision_Ext                    +*/
/*+  Input/Output       :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB             +*/
/*+                                                                         +*/
/*+  Result             :    LSA_UINT16                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock            (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function returns the "PNIP_Revision".                +*/
/*+               Defines of "PNIP_Revision" see <eddp_hal_inc.h>.          +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_HALGetPnipRevision_Ext(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    return (pDDB->Core.Hal.PNIP_Revision);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname       :    EDDP_HALGetPnipRevision                        +*/
/*+  Input/Output       :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB             +*/
/*+                                                                         +*/
/*+  Result             :    LSA_UINT16                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock            (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function returns the Revision of PNIP.               +*/
/*+               Defines of "PNIP_Revision" see <eddp_hal_inc.h>.          +*/
/*+               EDDP_HAL_HW_PNIP_REV1                                     +*/
/*+               EDDP_HAL_HW_PNIP_REV2                                     +*/
/*+               EDDP_HAL_HW_PNIP_REV3                                     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_HALGetPnipRevision(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    return (pDDB->Core.Hal.HardwareVersion);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname       :    EDDP_HALPnipPortCfg                            +*/
/*+  Input/Output       :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB             +*/
/*+                                                                         +*/
/*+  Result             :    LSA_UINT16                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock            (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function returns the configuration of ports.         +*/
/*+  Results:     EDDP_HAL_HW_PNIP_CFG_2PORT                                +*/
/*+               EDDP_HAL_HW_PNIP_CFG_4PORT                                +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_HALPnipPortCfg(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    return (pDDB->Core.Hal.HardwarePortCntType);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname       :    EDDP_HALGetChipType                            +*/
/*+  Input/Output       :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB             +*/
/*+                                                                         +*/
/*+  Result             :    LSA_UINT16                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock            (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function returns the type of the pnip chip.          +*/
/*+  Results:     EDDP_HAL_HW_TYPE_ASIC                                     +*/
/*+               EDDP_HAL_HW_TYPE_FPGA                                     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_HALGetChipType(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    return pDDB->Core.Hal.HardwareChipType;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname       :    EDDP_HALGetChipVersion                         +*/
/*+  Input/Output       :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB             +*/
/*+                                                                         +*/
/*+  Result             :    LSA_UINT16                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock            (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function returns the type of the pnip chip.          +*/
/*+  Results:     EDDP_HAL_HW_ERTEC200P                                     +*/
/*+               EDDP_HAL_HW_HERA                                          +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_HALGetChipVersion(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    return pDDB->Core.Hal.HardwareChipVersion;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname       :    EDDP_HALGetChipStep                            +*/
/*+  Input/Output       :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB             +*/
/*+                                                                         +*/
/*+  Result             :    LSA_UINT16                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock            (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function returns the step of the pnip chip.          +*/
/*+  Results:     EDDP_HAL_HW_STEP1                                         +*/
/*+               EDDP_HAL_HW_STEP2                                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_HALGetChipStep(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    return pDDB->Core.Hal.HardwareStep;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname       :    EDDP_HALGetChipInterface                       +*/
/*+  Input/Output       :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB             +*/
/*+                                                                         +*/
/*+  Result             :    LSA_UINT16                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock            (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function returns the interface type of the pnip chip.+*/
/*+  Results:     EDDP_HAL_HW_IF_A                                         +*/
/*+               EDDP_HAL_HW_IF_B                                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_HALGetChipInterface(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    return pDDB->Core.Hal.HardwareIF;
}


/*****************************************************************************/
/*  end of file eddp_hal_ram.c                                               */
/*****************************************************************************/
