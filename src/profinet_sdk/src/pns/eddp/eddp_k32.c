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
/*  F i l e               &F: eddp_k32.c                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP KRISC32 support functions                   */
/*                          (system dependend)                               */
/*                                                                           */
/*****************************************************************************/
#ifdef EDDP_MESSAGE
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
/*  2008-12-22  lrg   initial version.                                       */
/*  2009-01-08  lrg   EDDP_K32PutRQB() liefert EDD_STS_ERR_NOT_IMPL statt OK */
/*  2009-02-16  lrg   EDDP_K32RemotePortState(), EDDP_K32DelaySet()          */
/*  2009-03-12  lrg   KRISK-IF-Setup und START in EDDP_K32DeviceSetup()      */
/*  2009-04-27  lrg   Umstellung von EDD_SRV_KRISC32_PUT auf EDD_SRV_PTCP... */
/*  2009-05-26 am2219 EDDP_K32_RRQB_FREE_ALL() for K32-RRQBs cleanup of pDDB */
/*  2009-07-10  lrg   1007lrg001: AP00848470 NULL ptr after channel close    */
/*  2009-08-17  lrg   1708lrg001: K32_RAM32 at Opcode K32_OPC_GSY_DIAG       */
/*  2009-10-09  lrg   091009lrg001: pHDB aus Interface statt pRQB weil der   */
/*                    nach CloseChannel evtl. schon freigegeben ist.         */
/*                    ->  Zugriffsfehler nach pcIOX CloseChannel             */
/*  2009-11-03  lrg   Flusskontrolle fuer EDD_SRV_PTCP_SYNC/DELAY_CONTROL    */
/*                    DelayReqQueue, SyncReqQueue                            */
/*  2009-12-11  111209lrg001: KRISC32-Trace                                  */
/*  2009-12-21  lrg   K32_LOG_TO_TRACE                                       */
/*  2010-01-19  lrg   EDDP_CFG_DEBUG_KRISC32_TRACE_COMPILE_LEVEL             */
/*  2010-03-17  lrg   EDDP_FATAL() if invalid opcode in RRQB                 */
/*  2010-03-26  ma    renamed EDDP_CFG_DEBUG_KRISC32_TRACE_COMPILE_LEVEL     */
/*                    to EDDP_CFG_DEBUG_KRISC32_TRACE_LEVEL.                 */
/*  2010-06-22  lrg   AP00983699: cancel pending DelayRequests and           */
/*                                SyncRequests at close channel              */
/*  2010-10-11  111010lrg001: EDDP_CFG_LOCAL_MEM_STATIC, Idx -> Handle       */
/*  2010-11-24  JS    changes to prevent compiler warnings                   */
/*  2010-12-16  JS    added TRACE_SUBSYS_EDDP_K32_FW for KRISC FW traces     */
/*  2011-06-07  070611lrg: Takeover, Timeout in Trace                        */
/*  2011-07-25  lrg   AP01215492 K32_RSP_OK_ACTIVE, K32_OPC_GSY_DELAY_CNF    */
#endif
/*****************************************************************************/
#define K32_LOG_TO_TRACE
#undef EDDP_K32_LOOPBACK_TEST   // used in EDDP_K32PutRQB()

/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/
#define LTRC_ACT_MODUL_ID  121
#define EDDP_MODULE_ID     LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_K32 */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"               /* eddp headerfiles */
#include "eddp_int.h"               /* internal header  */
#include "eddp_k32_fw.h"

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)

/*===========================================================================*/
/*                                 static functions                          */
/*===========================================================================*/

static  EDD_RSP EDDP_LOCAL_FCT_ATTR  EDDP_CopyTimeStamp(
        volatile EDDP_TIMESTAMP_INFO_TYPE * const pDest, 
        const    EDDP_TIMESTAMP_INFO_TYPE * const pSrc);

static  EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CList_WriteTimeSyncInd (
        EDDP_LOCAL_DDB_PTR_TYPE             const pDDB,
        const    EDDP_TIMESTAMP_INFO_TYPE * const pOldTimestamp,
        const    EDDP_TIMESTAMP_INFO_TYPE * const pNewTimestamp);

static  EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_UpdateTimeDomainIdent (
        EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
        LSA_UINT8                 * pTimeDomainIdent,
        LSA_UINT8             const LengthTimeDomainIdent);


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_K32_RRQB_FREE_ALL                      +*/
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
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This static function frees all K32-RRQBs, owned by pDDB   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32_RRQB_FREE_ALL(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB)
{
    LSA_UINT32  Idx;
    EDD_RSP     Status = EDD_STS_OK;

    for (Idx = 0; Idx < EDDP_MAX_PORT_CNT; Idx++)
    {
        /* Free again allocated K32RQBs
        */
        if (!LSA_HOST_PTR_ARE_EQUAL(pDDB->K32.pK32PortRQB[Idx], LSA_NULL))
        {
            Status = EDDP_K32RQBFree(pDDB->K32.pK32PortRQB[Idx]);
            pDDB->K32.pK32PortRQB[Idx] = LSA_NULL;
        }
        if (!LSA_HOST_PTR_ARE_EQUAL(pDDB->K32.pK32DelayRQB[Idx], LSA_NULL))
        {
            Status = EDDP_K32RQBFree(pDDB->K32.pK32DelayRQB[Idx]);
            pDDB->K32.pK32DelayRQB[Idx] = LSA_NULL;
        }
    }
    if (!LSA_HOST_PTR_ARE_EQUAL(pDDB->K32.pK32CtrlRQB, LSA_NULL))
    {
        Status = EDDP_K32RQBFree(pDDB->K32.pK32CtrlRQB);
        pDDB->K32.pK32CtrlRQB = LSA_NULL;
    }
    if (!LSA_HOST_PTR_ARE_EQUAL(pDDB->K32.pK32ClockRQB, LSA_NULL))
    {
        Status = EDDP_K32RQBFree(pDDB->K32.pK32ClockRQB);
        pDDB->K32.pK32ClockRQB = LSA_NULL;
    }
    if (!LSA_HOST_PTR_ARE_EQUAL(pDDB->K32.pK32TimeRQB, LSA_NULL))
    {
        Status = EDDP_K32RQBFree(pDDB->K32.pK32TimeRQB);
        pDDB->K32.pK32TimeRQB = LSA_NULL;
    }
    return Status;
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_K32InterfaceDeInit                     +*/
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
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from EDDP_K32DeviceClose()        +*/
/*+               Undo Init Interface for K32                               +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32InterfaceDeInit(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB)
{
    EDD_RSP     Status = EDD_STS_OK;

    EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN :EDDP_K32InterfaceDeInit(pDDB=0x%X)",
        pDDB);

    Status = EDDP_K32_RRQB_FREE_ALL(pDDB);

    EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT:EDDP_K32InterfaceDeInit() Status=0x%X", Status);

    return Status;
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_K32InterfaceInit                       +*/
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
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from EDDP_K32DeviceOpen()         +*/
/*+               Init Interface for K32                                    +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32InterfaceInit(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB)
{
    LSA_UINT32  Idx;
    EDD_RSP     Status = EDD_STS_OK;

    EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN :EDDP_K32InterfaceInit(pDDB=0x%X)",
        pDDB);

    for (Idx = 0; Idx < EDDP_MAX_PORT_CNT; Idx++)
    {
        pDDB->K32.pK32PortRQB[Idx] = LSA_NULL;
        pDDB->K32.pK32DelayRQB[Idx] = LSA_NULL;
        pDDB->K32.TopoStateOk[Idx] = K32_INITIAL_TOPO_STATE;    // initial TopoOk setting
    }
    pDDB->K32.pK32ClockRQB = LSA_NULL;
    pDDB->K32.pK32TimeRQB = LSA_NULL;

    /* Allocate K32RQBs for Setup, Start, Stop, DelayControl, SycncControl and TopoOk setting
    */
    pDDB->K32.pK32CtrlRQB = EDDP_K32RQBAlloc();
    if (LSA_HOST_PTR_ARE_EQUAL(pDDB->K32.pK32CtrlRQB, LSA_NULL))
    {
        Status = EDD_STS_ERR_RESOURCE;
    }
    else
    {
        pDDB->K32.pK32ClockRQB = EDDP_K32RQBAlloc();
        if (LSA_HOST_PTR_ARE_EQUAL(pDDB->K32.pK32ClockRQB, LSA_NULL))
        {
            Status = EDD_STS_ERR_RESOURCE;
        }
        else
        {
            pDDB->K32.pK32ClockRQB->Hdr.UserId.void_ptr = LSA_NULL;
            pDDB->K32.pK32TimeRQB = EDDP_K32RQBAlloc();
            if (LSA_HOST_PTR_ARE_EQUAL(pDDB->K32.pK32TimeRQB, LSA_NULL))
            {
                Status = EDD_STS_ERR_RESOURCE;
            }
            else
            {
                pDDB->K32.pK32TimeRQB->Hdr.UserId.void_ptr = LSA_NULL;
                for (Idx = 0; Idx < pDDB->SWI.PortCnt; Idx++)
                {
                    pDDB->K32.pK32PortRQB[Idx] = EDDP_K32RQBAlloc();
                    if (LSA_HOST_PTR_ARE_EQUAL(pDDB->K32.pK32PortRQB[Idx], LSA_NULL))
                    {
                        Status = EDD_STS_ERR_RESOURCE;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Status);
                        break;
                    }
                    pDDB->K32.pK32DelayRQB[Idx] = EDDP_K32RQBAlloc();
                    if (LSA_HOST_PTR_ARE_EQUAL(pDDB->K32.pK32DelayRQB[Idx], LSA_NULL))
                    {
                        Status = EDD_STS_ERR_RESOURCE;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Status);
                        break;
                    }
                    else
                    {
                        pDDB->K32.pK32DelayRQB[Idx]->Hdr.UserId.void_ptr = LSA_NULL;
                    }
                }
            }
        }
        if (Status != EDD_STS_OK)
        {
            EDDP_K32_RRQB_FREE_ALL(pDDB);
            Status = EDD_STS_ERR_RESOURCE;
        }
    }

    EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT:EDDP_K32InterfaceInit() Status=0x%X", Status);

    return Status;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_K32DeviceOpen                          +*/
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
/*+               Initializes all K32 management structures within DDB      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32DeviceOpen(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    EDDP_UPPER_DPB_PTR_TYPE            pDPB)
{
    EDD_RSP      Status = EDD_STS_OK;

    EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN :EDDP_K32DeviceOpen(pDDB=0x%X pDPB=0x%X)",
        pDDB, pDPB);

    EDDP_ASSERT_NO_NULL_PTR(pDDB);
    EDDP_ASSERT_NO_NULL_PTR(pDPB);

    /*@note removed EDDP_CFG_DEBUG_KRISC32_TRACE_LEVEL */
    pDDB->K32.RRQBTraceLevel = pDPB->K32TraceLevel;

    // FT1204141: Initialize non zero members of timeStamp indication
    pDDB->K32.TimestampInfo.TimesStamp.Status = EDDP_PNIO_STATUS_NOT_SYNC;
    pDDB->K32.TimestampInfo.SyncState = EDDP_PTCP_SYNC_STATE_NO_MESSAGE_RECEIVED;

    pDDB->K32.pK32StartRQB = EDDP_K32RQBAlloc();
    if (LSA_HOST_PTR_ARE_EQUAL(pDDB->K32.pK32StartRQB, LSA_NULL))
    {
        Status = EDD_STS_ERR_RESOURCE;
    }
    else
    {
        // ***** do basic initialization
        Status = EDDP_K32InterfaceInit(pDDB);
        if (Status != EDD_STS_OK)
        {
            EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,
                "[H:--] *** EDDP_K32DeviceOpen() Error=0x%X from EDDP_K32InterfaceInit()",
                Status);
        }
    }
    EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,
        "[H:--] OUT:EDDP_K32DeviceOpen() Status=0x%X", Status);

    return Status;
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_K32DeviceSetup                         +*/
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
/*+               Setup Device for K32                                      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32DeviceSetup(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    EDDP_UPPER_DSB_PTR_TYPE            pDSB)
{
    LSA_UINT32  Addr=0; //, IfId;
    LSA_UINT16  Idx;
    EDD_RSP     Status = EDD_STS_OK;

    EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN :EDDP_K32DeviceSetup(pDDB=0x%X pDSB=0x%X)",
        pDDB, pDSB);

    if(0 != Addr)
    {
        EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] *** EDDP_K32DeviceSetup(): ERROR -> in KRISC32 firmware download at address=0x%X",
            Addr);
        Status = EDD_STS_ERR_HW;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Status);
    }
    else
    {
        EDDP_K32_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] --- EDDP_K32DeviceSetup(): KRISC32 download success :-)");

        if(EDD_STS_OK == Status)
        {
            EDDP_K32_FW_SetConfigData(pDDB,pDSB->physicalK32PNIPBase,pDSB->physicalPNIPK32DTCMBase);
            /* init of KRISC32 data queue ram
            */
            LSA_UINT32 partSize = 0;

            K32_QUEUE_PARTITION_IN_DATA_QUEUE_RAM* partPtr =
                (K32_QUEUE_PARTITION_IN_DATA_QUEUE_RAM*)
                EDDP_HALGetRAMSubAddress(pDDB,EDDP_HAL_RAMTYPE_CMD,EDDP_HAL_RAMSUB_CMD_KRISC,&partSize);

            /* initialize opcodes in CMDRAM (eddp/krisc32 rrqb queues) */
            /* note: this code should only be compiled if K32_OPC_NO_OPERATION is not equal zero! */
#if 0 != K32_OPC_NO_OPERATION
//#error This does not work with K32_OPC_LOG at the moment! Please set K32_OPC_NO_OPERATION to zero!
            {
                int index;
                volatile RRQB_QUEUE* queues[2];

                queues[0] = &partPtr->eddp;
                queues[1] = &partPtr->krisc32;

                for(index=0;index<(int)(NUMBER_OF_ELEMENTS(queues));++index)
                {
                    int slotIdx;
                    EDDP_HAL_MEM32_WRITE(pDDB,&(queues[index]->iBegin),0);
                    EDDP_HAL_MEM32_WRITE(pDDB,&(queues[index]->iNextAck), 0);
                    EDDP_HAL_MEM32_WRITE(pDDB,&(queues[index]->iFree), 0);

                    for(slotIdx=0;slotIdx<(int)(NUMBER_OF_ELEMENTS(queues[index]->slot));++slotIdx)
                    {
                        EDDP_HAL_MEM8_WRITE(pDDB,&(queues[index]->slot[slotIdx].Hdr.Opcode), K32_OPC_NO_OPERATION);
                    }
                }
            }
#endif  /* 0 == K32_OPC_NO_OPERATION */

            if (!LSA_HOST_PTR_ARE_EQUAL(pDDB->K32.pK32CtrlRQB, LSA_NULL))
            {
                /* Send interface port mapping to KRISC32 operating system
                 * Put DDB pointer into UserId to be returned in indication RQBs
                */
                pDDB->K32.pK32CtrlRQB->Hdr.Opcode = K32_OPC_SETUP;
                pDDB->K32.pK32CtrlRQB->Hdr.UserId.void_ptr = pDDB;
                pDDB->K32.pK32CtrlRQB->Params.Setup.PP0_PortID = 0;
                pDDB->K32.pK32CtrlRQB->Params.Setup.PP1_PortID = 0;
                pDDB->K32.pK32CtrlRQB->Params.Setup.PP2_PortID = 0;
                pDDB->K32.pK32CtrlRQB->Params.Setup.PP3_PortID = 0;
                pDDB->K32.pK32CtrlRQB->Params.Setup.TraceIdx = pDDB->TraceIdx;

                for (Idx = 1; Idx <= pDDB->SWI.PortCnt; Idx++)
                {
                    LSA_UINT32  HwPort = EDDP_SWI_get_HwPortID (pDDB, Idx);

                    switch (HwPort)
                    {
                    case 1:
                        pDDB->K32.pK32CtrlRQB->Params.Setup.PP0_PortID = Idx;
                        break;

                    case 2:
                        pDDB->K32.pK32CtrlRQB->Params.Setup.PP1_PortID = Idx;
                        break;

                    case 3:
                        pDDB->K32.pK32CtrlRQB->Params.Setup.PP2_PortID = Idx;
                        break;

                    case 4:
                        pDDB->K32.pK32CtrlRQB->Params.Setup.PP3_PortID = Idx;
                        break;

                    default:
                        EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] *** EDDP_K32DeviceSetup() Invalid HwPort=%u",
                            HwPort);
                        EDDP_FATAL(0);
                        break;
                    }
                }
                EDDP_K32_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] --- EDDP_K32DeviceSetup() sending SETUP: PortCnt=%u PP0=%u PP1=%u PP2=%u PP3=%u TraceIdx=%u",
                    pDDB->SWI.PortCnt, pDDB->K32.pK32CtrlRQB->Params.Setup.PP0_PortID,
                    pDDB->K32.pK32CtrlRQB->Params.Setup.PP1_PortID, pDDB->K32.pK32CtrlRQB->Params.Setup.PP2_PortID,
                    pDDB->K32.pK32CtrlRQB->Params.Setup.PP3_PortID, pDDB->K32.pK32CtrlRQB->Params.Setup.TraceIdx);

                Status = EDDP_K32PutRQB(pDDB->K32.pK32CtrlRQB, pDDB);
                if (EDD_STS_OK != Status)
                {
                    EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] *** EDDP_K32DeviceSetup(): Status=0x%X from EDDP_K32PutRQB()",
                            Status);
                }
            }
            else
            {
                EDDP_K32_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] *** EDDP_K32DeviceSetup() no K32CtrlRQB");
                Status = EDD_STS_ERR_RESOURCE;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Status);
            }
        }
        if (EDD_STS_OK == Status)
        {
            if (!LSA_HOST_PTR_ARE_EQUAL(pDDB->K32.pK32StartRQB, LSA_NULL))
            {
                /* Send START Command to KRISC32 operating system
                */
                EDDP_K32_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] --- EDDP_K32DeviceSetup(): sending START");
                pDDB->K32.pK32StartRQB->Hdr.Opcode = K32_OPC_START;
                pDDB->K32.pK32StartRQB->Hdr.UserId.void_ptr = pDDB;
                Status = EDDP_K32PutRQB(pDDB->K32.pK32StartRQB, pDDB);
                if (EDD_STS_OK != Status)
                {
                    EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] *** EDDP_K32DeviceSetup(): Status=0x%X from EDDP_K32PutRQB()",
                        Status);
                }
                else
                {
                    /* Initialize parameters for user cmd TimeStampGet in PNIP CMD RAM
                    */
                    LSA_UINT32 TimeGetStatusSyncState;
                    EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE UserCmdInfoBlockAddr;

                    TimeGetStatusSyncState = EDDP_PNIO_STATUS_NOT_SYNC;
                    TimeGetStatusSyncState <<= 16;
                    TimeGetStatusSyncState |= EDDP_PTCP_SYNC_STATE_NO_MESSAGE_RECEIVED;

                    // get address of UserCmd-Info-Block in host format
                    UserCmdInfoBlockAddr = (EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE) EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_USER_CMD_INFO_BLOCK, 0);

                    EDDP_HAL_MEM32_WRITE(pDDB, &UserCmdInfoBlockAddr->TimeGetSequenceID,      1);   // As long as NOT_SYNC, the SeqID must be manipulated due to add a temporary Sync Indication.
                    EDDP_HAL_MEM32_WRITE(pDDB, &UserCmdInfoBlockAddr->TimeGetSequenceIDCopy,  1);
                    EDDP_HAL_MEM32_WRITE(pDDB, &UserCmdInfoBlockAddr->TimeGetStatusSyncState, TimeGetStatusSyncState);
                    EDDP_HAL_MEM32_WRITE(pDDB, &UserCmdInfoBlockAddr->TimeGetEpochUTCOffset,  0);
                    EDDP_HAL_MEM32_WRITE(pDDB, &UserCmdInfoBlockAddr->TimeGetMACAddressLo,    0);
                    EDDP_HAL_MEM32_WRITE(pDDB, &UserCmdInfoBlockAddr->TimeGetMACAddressHi,    0);

                    /* Finish DeviceSetup on return of the KRISC32 START RQB in EDDP_K32GetRQB()
                    */
                    Status = EDD_STS_OK_PENDING;
                }
            }
            else
            {
                EDDP_K32_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] *** EDDP_K32DeviceSetup(): No K32StartRQB");
                Status = EDD_STS_ERR_RESOURCE;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Status);
            }
        }
    }

    EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT:EDDP_K32DeviceSetup() Status=0x%X",
        Status);

    LSA_UNUSED_ARG(pDDB); // handle unused parameter

    return Status;
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_K32DeviceIFSetup                       +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE           pDDB      +*/
/*+                             EDDP_UPPER_DSB_PTR_TYPE           pDSB      +*/
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
/*+               EDDP_SRV_DEV_IF_SETUP                                     +*/
/*+               Setup Interface or K32                                    +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32DeviceIFSetup(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    EDDP_UPPER_DSB_PTR_TYPE            pDSB)
{
    EDD_RSP     Status = EDD_STS_OK;

    LSA_UNUSED_ARG(pDDB); // handle unused parameter
    LSA_UNUSED_ARG(pDSB); // handle unused parameter

    EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN :EDDP_K32DeviceIFSetup(pDDB=0x%X pDSB=0x%X)",
        pDDB, pDSB);

    EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT:EDDP_K32DeviceIFSetup() Status=0x%X",
        Status);

    return Status;
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_K32DeviceIFShutdown                    +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE           pDDB      +*/
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
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDDP_SRV_DEV_IF_SHUTDOWN or EDDP_SRV_DEV_IF_SETUP (only if+*/
/*+               previous setup shall be aborted)                          +*/
/*+               Shutdown Interface of K32                                 +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32DeviceIFShutdown(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB)
{
    EDD_RSP     Status = EDD_STS_OK;

    EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN :EDDP_K32DeviceIFShutdown(pDDB=0x%X)",
        pDDB);

    EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT:EDDP_K32DeviceIFShutdown() Status=0x%X",
        Status);

    LSA_UNUSED_ARG(pDDB); // handle unused parameter
    return Status;
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_K32DeviceShutdown                      +*/
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
/*+               Shutdown Interface of K32                                 +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32DeviceShutdown(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB)
{
    EDD_RSP Status = EDD_STS_OK;

    EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN :EDDP_K32DeviceShutdown(pDDB=0x%X)",pDDB);


    /* Send STOP Command to KRISC32 operating system
     */
    if (!LSA_HOST_PTR_ARE_EQUAL(pDDB->K32.pK32StartRQB, LSA_NULL))
    {
        EDDP_K32_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH,
                "[H:--] --- EDDP_K32DeviceShutdown() sending STOP");
        pDDB->K32.pK32StartRQB->Hdr.Opcode = K32_OPC_STOP;
        pDDB->K32.pK32StartRQB->Hdr.UserId.void_ptr = pDDB;
        Status = EDDP_K32PutRQB(pDDB->K32.pK32StartRQB, pDDB);
        if (EDD_STS_OK != Status)
        {
            EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,
                    "[H:--] *** EDDP_K32DeviceShutdown() Status=0x%X from EDDP_K32PutRQB()",
                    Status);
        }
    }

    EDDP_K32_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,
            "[H:--] OUT:EDDP_K32DeviceShutdown()");

    return EDD_STS_OK_PENDING ;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_K32DeviceClose                         +*/
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
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDDP_SRV_DEV_CLOSE or EDDP_SRV_DEV_OPEN (only if          +*/
/*+               previous OPEN shall be aborted)                           +*/
/*+               Close device for K32                                      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32DeviceClose(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB)
{
    EDD_RSP Status = EDD_STS_OK;

    EDDP_ASSERT_NO_NULL_PTR(pDDB);

    EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,
            "[H:--] IN :EDDP_K32DeviceClose(pDDB=0x%X)", pDDB);

    // ***** do de-initialization
    Status = EDDP_K32InterfaceDeInit(pDDB);
    if (Status != EDD_STS_OK)
    {
        EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,
                "[H:--] *** EDDP_K32DeviceClose(): Error=0x%X from EDDP_K32DeInitInterface()",
                Status);
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Status);
    }

    if (!LSA_HOST_PTR_ARE_EQUAL(pDDB->K32.pK32StartRQB, LSA_NULL))
    {
        /* Free allocated K32StartRQB
         */
        Status = EDDP_K32RQBFree(pDDB->K32.pK32StartRQB);
        pDDB->K32.pK32StartRQB = LSA_NULL;
    }

    EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,
            "[H:--] OUT:EDDP_K32DeviceClose() Status=0x%X", Status);

    if ((EDDP_HW_ERTEC200P == pDDB->Hardware) || (EDDP_HW_ERTEC200P_FPGA == pDDB->Hardware))
    {
        EDDP_K32_RESET(pDDB->hSysDev, LSA_TRUE);
    }

    return Status;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_K32OpenChannel                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB           +*/
/*+                        :    EDDP_LOCAL_HDB_PTR_TYPE      pHDB           +*/
/*+                             LSA_HANDLE_TYPE              Handle         +*/
/*+                             EDDP_SYSTEM_CDB_PTR_TYPE     pCDB           +*/
/*+  Result                :    LSA_RESULT                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                    (in) +*/
/*+  Handle     : Handle for this Channel (<  EDD_CFG_MAX_CHANNELS)    (in) +*/
/*+  pCDB       : Pointer to ChannelDescriptionBlock                   (in) +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_CHANNEL_USE                                   +*/
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Opens Channel for K32. The Handle will specify the Channel+*/
/*+               and pDDB the Device. pCDB has all parameters              +*/
/*+               for this channel                                          +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32OpenChannel(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE            pHDB,
    LSA_HANDLE_TYPE                    Handle,
    EDDP_SYSTEM_CDB_PTR_TYPE           pCDB)
{
    LSA_UINT16  Idx;
    EDD_RSP     Status = EDD_STS_OK;
    EDDP_LOCAL_DDB_K32_PTR_TYPE  pK32Iface;
    EDDP_LOCAL_HDB_K32_PTR_TYPE  pHDB_K32_PoolItem;

    EDDP_K32_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN :EDDP_K32OpenChannel(pDDB=0x%X, pHDB=0x%X, Handle:%2X, pCDB=0x%X)",
        pHDB->Handle, pDDB, pHDB, Handle, pCDB);


    pK32Iface = &pDDB->K32;
    pHDB_K32_PoolItem = LSA_NULL;

    /* ----------------------------------------------------------------------*/
    /* If HandleCnt too high return with error                               */
    /* ----------------------------------------------------------------------*/
    if ( Handle >= EDDP_CFG_MAX_CHANNELS )
    {
        EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] *** EDDP_K32OpenChannel() pDDB->K32.HandleCnt is Max",
            pHDB->Handle);
        EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
        Status = EDD_STS_ERR_PARAM;
    }
    else
    {
        /***** alloc handle management-memory
        */
        #ifdef EDDP_CFG_LOCAL_MEM_STATIC
        pHDB_K32_PoolItem = &g_EDDP_HDB_K32[Handle];    //111010lrg001: Idx -> Handle
        #else
        pHDB_K32_PoolItem = EDDP_GlbAllocLocalMem(sizeof(EDDP_HDB_K32_TYPE));
        if (LSA_HOST_PTR_ARE_EQUAL(pHDB_K32_PoolItem, LSA_NULL))
        {
            EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] *** EDDP_K32OpenChannel() Cannot allocate pHDB_K32_PoolItem",
                pHDB->Handle);
            EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_RESOURCE);
            Status = EDD_STS_ERR_RESOURCE;
        }
        #endif
        pHDB->pK32 = pHDB_K32_PoolItem;                                 // assign K32 managment for this channel
    }

    if( Status == EDD_STS_OK)
    {
        /***** init handle management for this channel
        */
        EDDP_GlbRQBQueueInit(&pHDB_K32_PoolItem->CmdReqQueue);          // no RQBs for this channel in queues
        EDDP_GlbRQBQueueInit(&pHDB_K32_PoolItem->DiagReqQueue);
        EDDP_GlbRQBQueueInit(&pHDB_K32_PoolItem->SyncReqQueue[0]);
        EDDP_GlbRQBQueueInit(&pHDB_K32_PoolItem->SyncReqQueue[1]);
        pHDB_K32_PoolItem->pK32DDB = pK32Iface;                         // reference to global K32 mgm
        pHDB_K32_PoolItem->EDD_Handle = pHDB;                           // handle open channel
        pK32Iface->HandleCnt++;                                         // cnt of currently open handles
        pK32Iface->pHDB = pHDB;                                         // set GSY sync channel in IF data
    }

    if( Status == EDD_STS_OK)
    {
        for (Idx = 0; Idx < EDDP_MAX_PORT_CNT; Idx++)
        {
            EDDP_GlbRQBQueueInit(&pHDB_K32_PoolItem->DelayReqQueue[Idx]);
            if (K32_INITIAL_TOPO_STATE != pK32Iface->TopoStateOk[Idx])
            {
                /* Send last TopoState for this port to KRISC32
                */
                EDD_UPPER_KRISC32_RQB_PTR_TYPE pK32RQB = pK32Iface->pK32PortRQB[Idx];

                if (!LSA_HOST_PTR_ARE_EQUAL(pK32RQB, LSA_NULL))
                {
                    /* Send RQB to KRISC32
                    */
                    EDDP_K32_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:%2X] --- EDDP_K32OpenChannel() sending TopoOk=%u for port=%u",
                        pHDB->Handle, pK32Iface->TopoStateOk[Idx], Idx+1);
                    pK32RQB->Hdr.Opcode = K32_OPC_GSY_TOPO_STATE;
                    pK32RQB->Hdr.UserId.void_ptr = pDDB;
                    pK32RQB->Params.TopoState.PortID = Idx+1;
                    pK32RQB->Params.TopoState.TopoStateOk = (pK32Iface->TopoStateOk[Idx] != LSA_FALSE ? K32_TRUE : K32_FALSE);
                    Status = EDDP_K32PutRQB(pK32RQB, pDDB);
                }
                else
                {
                    EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "[H:%2X] *** EDDP_K32OpenChannel() No K32 RQB for port=%u",
                        pHDB->Handle, Idx+1);
                }
            }
        }
    }

    // ***** release memory on error
    if( Status != EDD_STS_OK)
    {
        #ifndef EDDP_CFG_LOCAL_MEM_STATIC
        if (!LSA_HOST_PTR_ARE_EQUAL(pHDB_K32_PoolItem, LSA_NULL))
        {
            EDDP_GlbFreeLocalMem(pHDB_K32_PoolItem);
        }
        #endif
        pHDB->pK32 = LSA_NULL;
    }

    EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT:EDDP_K32OpenChannel() Status=0x%x", Status);

    LSA_UNUSED_ARG(pCDB);       // handle unused parameter
    return Status;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_K32CloseChannel                        +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB           +*/
/*+                        :    EDDP_LOCAL_HDB_PTR_TYPE      pHDB           +*/
/*+                             LSA_HANDLE_TYPE              Handle         +*/
/*+  Result                :    LSA_RESULT                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                    (in) +*/
/*+  Handle     : Handle for this Channel (<  EDD_CFG_MAX_CHANNELS)    (in) +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_OK_PENDING                                        +*/
/*+               EDD_STS_ERR_xxx                                           +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Close Channel for K32.                                    +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32CloseChannel(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE            pHDB,
    LSA_HANDLE_TYPE                    Handle)
{
    LSA_UINT16  Idx;
    EDD_RSP     Status;
    EDDP_LOCAL_DDB_K32_PTR_TYPE pK32Iface;
    EDD_UPPER_RQB_PTR_TYPE      pRQB;

    EDDP_K32_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN :EDDP_K32CloseChannel(pDDB=0x%X, pHDB=0x%X, Handle=%2X)",
        pHDB->Handle, pDDB, pHDB, Handle);


    /* ----------------------------------------------------------------------*/
    /* If handle dont uses K32 return ok.                                    */
    /* ----------------------------------------------------------------------*/

    if (LSA_HOST_PTR_ARE_EQUAL(pHDB->pK32, LSA_NULL))
    {
        return(EDD_STS_OK);
    }

    pK32Iface = &pDDB->K32;

    /* ----------------------------------------------------------------------*/
    /* Next we check if any request is present                               */
    /* ----------------------------------------------------------------------*/

    EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:%2X] --- EDDP_K32CloseChannel() CmdReqQueue.Count=%u",
        pHDB->Handle, pHDB->pK32->CmdReqQueue.Count);

    if (pHDB->pK32->CmdReqQueue.Count != 0)
    {
        /* Cancel pending CmdRequests
        */
        do
        {
            pRQB = EDDP_GlbRQBQueueRemoveFromBegin(&pHDB->pK32->CmdReqQueue);
            if (!LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL))
            {
                EDDP_GlbRequestFinish(pHDB, pRQB, EDD_STS_OK_CANCEL);
            }
        } while (!LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL));
    }
    if (pHDB->pK32->DiagReqQueue.Count != 0)
    {
        /* Cancel pending DiagRequests
        */
        do
        {
            pRQB = EDDP_GlbRQBQueueRemoveFromBegin(&pHDB->pK32->DiagReqQueue);
            if (!LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL))
            {
                EDDP_GlbRequestFinish(pHDB, pRQB, EDD_STS_OK_CANCEL);
            }
        } while (!LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL));
    }

    /* AP00983699: cancel pending DelayRequests and SyncRequests
    */
    for (Idx = 0; Idx < EDDP_MAX_PORT_CNT; Idx++)
    {
        if (pHDB->pK32->DelayReqQueue[Idx].Count != 0)
        {
            /* Cancel pending DelayRequests
            */
            do
            {
                pRQB = EDDP_GlbRQBQueueRemoveFromBegin(&pHDB->pK32->DelayReqQueue[Idx]);
                if (!LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL))
                {
                    EDDP_GlbRequestFinish(pHDB, pRQB, EDD_STS_OK_CANCEL);
                }
            } while (!LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL));
        }
    }
    for (Idx = 0; Idx < 2; Idx++)
    {
        if (pHDB->pK32->SyncReqQueue[Idx].Count != 0)
        {
            /* Cancel pending SyncRequests
            */
            do
            {
                pRQB = EDDP_GlbRQBQueueRemoveFromBegin(&pHDB->pK32->SyncReqQueue[Idx]);
                if (!LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL))
                {
                    EDDP_GlbRequestFinish(pHDB, pRQB, EDD_STS_OK_CANCEL);
                }
            } while (!LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL));
        }
    }

    Status = EDD_STS_OK;

#ifndef EDDP_CFG_LOCAL_MEM_STATIC
    // *****  Release all memory used by the handle.
    if ( EDDP_GlbFreeLocalMem(pHDB->pK32) != LSA_RET_OK )
    {
        EDDP_K32_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] *** EDDP_K32CloseChannel() Status=0x%X from EDDP_GlbFreeLocalMem(0x%X)",
            pHDB->Handle, Status, pHDB->pK32);
        Status = EDD_STS_ERR_RESOURCE;
    }
    pHDB->pK32 = LSA_NULL;          // We always release the handle
#endif

    EDDP_ASSERT(pK32Iface->HandleCnt);
    pK32Iface->HandleCnt--;         // Count of currently open handles
    pK32Iface->pHDB = LSA_NULL;

    EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT:EDDP_K32CloseChannel() Status=%x", Status);

    LSA_UNUSED_ARG(Handle);     // handle unused parameter
    return Status;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_K32RequestDelay                        +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE          pRQB        +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE         pDDB        +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE         pHDB        +*/
/*+                             EDD_UPPER_KRISC32_RQB_PTR_TYPE  pK32UpperRQB+*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function handles the following services for K32:     +*/
/*+                                                                         +*/
/*+               EDD_SRV_PTCP_DELAY_CONTROL                                +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32RequestDelay(
    EDD_UPPER_RQB_PTR_TYPE             pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE            pHDB,
    EDD_UPPER_KRISC32_RQB_PTR_TYPE     pK32UpperRQB)
{
    EDD_UPPER_PTCP_DELAY_CONTROL_PTR_TYPE pParam = (EDD_UPPER_PTCP_DELAY_CONTROL_PTR_TYPE)pRQB->pParam;
    EDD_RSP Status = EDD_STS_OK;

    EDDP_K32_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN :EDDP_K32RequestDelay(pRQB=0x%X, pK32RQB=0x%X, pDDB=0x%X, pHDB=0x%X)",
        pHDB->Handle, pRQB, pK32UpperRQB, pDDB, pHDB);

    /* Set K32 RQB values from pParam and store RQB ptr in the UserId (lower RQB in use)
    */
    pK32UpperRQB->Hdr.Opcode = K32_OPC_GSY_DELAY_REQ;
    pK32UpperRQB->Hdr.UserId.void_ptr = pRQB;
    pK32UpperRQB->Params.DelayReq.PortID = pParam->PortID;
    {
        LSA_UINT32  iMacAddr;
        for ( iMacAddr=0; iMacAddr < EDD_MAC_ADDR_SIZE; iMacAddr++ )
        {
            EDDP_HAL_MEM8_WRITE(pDDB,&(pK32UpperRQB->Params.DelayReq.PortMacAddr[iMacAddr]), pParam->PortMacAddr.MacAdr[iMacAddr]);
        }
    }
    pK32UpperRQB->Params.DelayReq.ReqActive = pParam->ReqActive;
    pK32UpperRQB->Params.DelayReq.RspActive = pParam->RspActive;
    pK32UpperRQB->Params.DelayReq.RxPortDelay = pParam->RxPortDelay;
    pK32UpperRQB->Params.DelayReq.TxPortDelay = pParam->TxPortDelay;

    /* Queue upper RQB into CmdReqQueue and send K32RQB to KRISC32
    */
    EDDP_GlbRQBQueueAddToEnd(&pHDB->pK32->CmdReqQueue, pRQB);
    Status = EDDP_K32PutRQB(pK32UpperRQB, pDDB);
    if (Status == EDD_STS_OK)
    {
        EDDP_K32_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:%2X] --- EDDP_K32RequestDelay(pRQB=0x%X) EDD_SRV_PTCP_DELAY_CONTROL Port=%u Active=%u: queued in CmdReqQueue: Count=%u",
            pHDB->Handle, pRQB, pParam->PortID, pParam->ReqActive, pHDB->pK32->CmdReqQueue.Count);
    }
    else
    {
        /* Remove upper RQB from Queue and unuse K32 RQB
        */
        EDDP_GlbRQBQueueRemoveFromQueue(&pHDB->pK32->CmdReqQueue, pRQB);
        pK32UpperRQB->Hdr.UserId.void_ptr = LSA_NULL;
        EDDP_K32_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] *** EDDP_K32RequestDelay(pRQB=0x%X): EDD_SRV_PTCP_DELAY_CONTROL: Status(0x%X) from EDDP_K32PutRQB()",
            pHDB->Handle, pRQB, Status);
    }

    EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT:EDDP_K32RequestDelay() Status=0x%X", Status);

    return(Status);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_K32RequestSync                         +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE          pRQB        +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE         pDDB        +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE         pHDB        +*/
/*+                             EDD_UPPER_KRISC32_RQB_PTR_TYPE  pK32UpperRQB+*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function handles the following services for K32:     +*/
/*+                                                                         +*/
/*+               EDD_SRV_PTCP_SYNC_CONTROL                                 +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32RequestSync(
    EDD_UPPER_RQB_PTR_TYPE             pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE            pHDB,
    EDD_UPPER_KRISC32_RQB_PTR_TYPE     pK32UpperRQB)
{
    EDD_RSP Status = EDD_STS_OK;
    EDD_UPPER_PTCP_SYNC_CONTROL_PTR_TYPE pParam = (EDD_UPPER_PTCP_SYNC_CONTROL_PTR_TYPE)pRQB->pParam;

    EDDP_K32_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN :EDDP_K32RequestSync(pRQB=0x%X, pK32RQB=0x%X, pDDB=0x%X, pHDB=0x%X)",
        pHDB->Handle, pRQB, pK32UpperRQB, pDDB, pHDB);

    // Update Time Domain Ident, ignore response because the feature is optional
    EDDP_UpdateTimeDomainIdent(pHDB->pDDB, LSA_NULL, 0);

    /* Set K32 RQB values from pParam and store RQB ptr in the UserId (lower RQB in use)
    */
    pK32UpperRQB->Hdr.Opcode = K32_OPC_GSY_SYNC;
    pK32UpperRQB->Hdr.UserId.void_ptr = pRQB;
    pK32UpperRQB->Params.SyncReq.SyncId = pParam->SyncId;
    pK32UpperRQB->Params.SyncReq.SlaveState = pParam->SlaveMode;
    pK32UpperRQB->Params.SyncReq.MasterState = pParam->MasterMode;
    pK32UpperRQB->Params.SyncReq.LengthTLV = pParam->LengthTLV;
    pK32UpperRQB->Params.SyncReq.PLLWindow = pParam->PLLWindow;
    pK32UpperRQB->Params.SyncReq.SyncSendFactor = pParam->SyncSendFactor;
    pK32UpperRQB->Params.SyncReq.TakeoverFactor = pParam->TakeoverFactor;
    pK32UpperRQB->Params.SyncReq.TimeoutFactor = pParam->TimeoutFactor;
    EDDP_MEMCOPY_COMMON(pK32UpperRQB->Params.SyncReq.TLV,pParam->TLV, pParam->LengthTLV);

    /* Queue upper RQB into CmdReqQueue and send K32RQB to KRISC32
    */
    EDDP_GlbRQBQueueAddToEnd(&pHDB->pK32->CmdReqQueue, pRQB);
    Status = EDDP_K32PutRQB(pK32UpperRQB, pDDB);
    if (Status == EDD_STS_OK)
    {
        EDDP_K32_TRACE_08(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:%8X] --- EDDP_K32RequestSync(pRQB=0x%X) EDD_SRV_PTCP_SYNC_CONTROL SyncId=%u MasterMode=%u SlaveMode=%u Takeover=%u Timeout=%u: queued in CmdReqQueue: Count=%u",
             pHDB->pK32->EDD_Handle, pRQB, pParam->SyncId, pParam->MasterMode, pParam->SlaveMode,
             pK32UpperRQB->Params.SyncReq.TakeoverFactor, pK32UpperRQB->Params.SyncReq.TimeoutFactor,
             pHDB->pK32->CmdReqQueue.Count);
    }
    else
    {
        /* Remove upper RQB from Queue and unuse K32 RQB
        */
        EDDP_GlbRQBQueueRemoveFromQueue(&pHDB->pK32->CmdReqQueue, pRQB);
        pK32UpperRQB->Hdr.UserId.void_ptr = LSA_NULL;
        EDDP_K32_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%8X] *** EDDP_K32RequestSync(pRQB=0x%X) EDD_SRV_PTCP_SYNC_CONTROL: Status(0x%X) from EDDP_K32PutRQB()",
            pHDB->pK32->EDD_Handle, pRQB, Status);
    }

    EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT:EDDP_K32RequestSync() Status=0x%X", Status);

    return(Status);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_K32Request                             +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+  Result                :    LSA_RESULT                                  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters          (in) +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         Opcode:     EDD_OPC_REQUEST                 +*/
/*+     LSA_HANDLE_TYPE         Handle:                                     +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user (not used)           +*/
/*+     EDD_SERVICE             Service:    EDD_SRV_xxx                     +*/
/*+                                         EDD_SRV_xxx                     +*/
/*+                                         EDD_SRV_xxx                     +*/
/*+     LSA_RESULT              Status:     Return status                   +*/
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:     Depend on service               +*/
/*+                                                                         +*/
/*+  RQB-return values via callback-function depends on service             +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function handles the following services for K32:     +*/
/*+                                                                         +*/
/*+               EDD_SRV_xxx_yyy                                           +*/
/*+                                                                         +*/
/*+               If we have a parameter error we will set EDD_STS_ERR_PARAM+*/
/*+               and call the handle-callbackfunction and abort the request+*/
/*+                                                                         +*/
/*+               EDD_STS_ERR_SERVICE is set if we get an invalid Service.  +*/
/*+               EDD_STS_ERR_CHANNEL_USE is set if the service is invalid  +*/
/*+               for this channel (i.e. not used by this channel)          +*/
/*+                                                                         +*/
/*+               If the request is valid, it is queued or executed.        +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_K32Request(
    EDD_UPPER_RQB_PTR_TYPE             pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE            pHDB)
{
    EDD_RSP   Status;
    EDD_UPPER_KRISC32_RQB_PTR_TYPE pK32UpperRQB = LSA_NULL;

    EDDP_K32_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN :EDDP_K32Request(pRQB=0x%X, pDDB=0x%X, pHDB=0x%X)",
        pHDB->Handle, pRQB, pDDB, pHDB);
    Status = EDD_STS_OK;

    //---------------------------------------------------------------------------
    // Check if management is initialized, --> test private K32 pointer
    //---------------------------------------------------------------------------
    if (LSA_HOST_PTR_ARE_EQUAL(pHDB->pK32, LSA_NULL))
    {
        EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "[H:%2X] *** EDDP_K32Request: K32 not initialized yet",
            pHDB->Handle);
        EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_CHANNEL_USE);
        Status = EDD_STS_ERR_CHANNEL_USE;
    }

    //---------------------------------------------------------------------------
    // every request needs a param-ptr, so check it.
    //---------------------------------------------------------------------------
    if (Status == EDD_STS_OK)
    {
        if  (LSA_HOST_PTR_ARE_EQUAL(pRQB->pParam, LSA_NULL))
        {
            EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] *** EDDP_K32Request: Service=0x%X, RQB->pParam is NULL",
                pHDB->Handle, EDD_RQB_GET_SERVICE(pRQB));
            Status = EDD_STS_ERR_PARAM;
        }
    }

    if (Status == EDD_STS_OK)
    {
        Status = EDD_STS_ERR_NOT_SUPPORTED;  // default error
        switch(EDD_RQB_GET_SERVICE(pRQB))
        {
            //-------------------------------------------------------------
            case EDD_SRV_PTCP_DIAG_IND_PROVIDE:
            //-------------------------------------------------------------
            {   // Diagnosis indication request. Queue the Request within Queue used by handle.

                EDDP_UPPER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_PTCP_DIAG_IND_PROVIDE",
                    pHDB->Handle);

                /* Put Diag RQBs into DiagReqQueue
                */
                EDDP_GlbRQBQueueAddToEnd(&pHDB->pK32->DiagReqQueue, pRQB);
                Status = EDD_STS_OK;
                EDDP_K32_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%8X] --- EDDP_K32Request(pRQB=0x%X) is queued in DiagReqQueue: Count=%u",
                    pHDB->pK32->EDD_Handle, pRQB,pHDB->pK32->DiagReqQueue.Count);
                break;
            }

            //-------------------------------------------------------------
            case EDD_SRV_PTCP_DELAY_CONTROL:
            //-------------------------------------------------------------
            {
                /* Upper delay control service
                */
                EDD_UPPER_PTCP_DELAY_CONTROL_PTR_TYPE pParam = (EDD_UPPER_PTCP_DELAY_CONTROL_PTR_TYPE)pRQB->pParam;

                if ((pParam->PortID > 0) && (pParam->PortID <= pDDB->SWI.PortCnt))
                {
                    pK32UpperRQB = pDDB->K32.pK32DelayRQB[pParam->PortID-1];

                    EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_PTCP_DELAY_CONTROL PortId=%u",
                        pHDB->Handle, pParam->PortID);

                    if (!LSA_HOST_PTR_ARE_EQUAL(pK32UpperRQB, LSA_NULL))
                    {
                        if (LSA_HOST_PTR_ARE_EQUAL(pK32UpperRQB->Hdr.UserId.void_ptr, LSA_NULL))
                        {
                            Status = EDDP_K32RequestDelay(pRQB, pDDB, pHDB, pK32UpperRQB);
                        }
                        else
                        {
                            Status = EDD_STS_OK;
                            EDDP_GlbRQBQueueAddToEnd(&pHDB->pK32->DelayReqQueue[pParam->PortID-1], pRQB);
                            EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%8X] --- EDDP_K32Request(pRQB=0x%X) EDD_SRV_PTCP_DELAY_CONTROL: Request pending",
                                pHDB->pK32->EDD_Handle, pRQB);
                        }
                    }
                    else
                    {
                        Status = EDD_STS_ERR_RESOURCE;
                        EDDP_K32_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%8X] *** EDDP_K32Request(pRQB=0x%X) EDD_SRV_PTCP_DELAY_CONTROL: No free K32 delay control RQB=%X",
                            pHDB->pK32->EDD_Handle, pRQB, pK32UpperRQB);
                    }
                }
                else
                {
                    Status = EDD_STS_ERR_PARAM;
                    EDDP_K32_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%8X] *** EDDP_K32Request(pRQB=0x%X) EDD_SRV_PTCP_DELAY_CONTROL: Invalid PortId=%u",
                        pHDB->pK32->EDD_Handle, pRQB, pParam->PortID);
                }
                break;
            }

            //-------------------------------------------------------------
            case EDD_SRV_PTCP_SYNC_CONTROL:
            //-------------------------------------------------------------
            {
                /* Upper sync control service
                */
                EDD_UPPER_PTCP_SYNC_CONTROL_PTR_TYPE pParam = (EDD_UPPER_PTCP_SYNC_CONTROL_PTR_TYPE)pRQB->pParam;

                EDDP_UPPER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] >>> Request: EDD_SRV_PTCP_SYNC_CONTROL SyncId=%u",
                    pHDB->Handle, pParam->SyncId);

                if (EDD_PTCP_SYNC_ID_CLOCK == pParam->SyncId)
                {
                    pK32UpperRQB = pDDB->K32.pK32ClockRQB;
                }
                else if (EDD_PTCP_SYNC_ID_TIME == pParam->SyncId)
                {
                    pK32UpperRQB = pDDB->K32.pK32TimeRQB;
                }

                if (!LSA_HOST_PTR_ARE_EQUAL(pK32UpperRQB, LSA_NULL))
                {
                    if (LSA_HOST_PTR_ARE_EQUAL(pK32UpperRQB->Hdr.UserId.void_ptr, LSA_NULL))
                    {
                        Status = EDDP_K32RequestSync(pRQB, pDDB, pHDB, pK32UpperRQB);
                    }
                    else
                    {
                        Status = EDD_STS_OK;
                        EDDP_GlbRQBQueueAddToEnd(&pHDB->pK32->SyncReqQueue[pParam->SyncId], pRQB);
                        EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] --- EDDP_K32Request(pRQB=0x%X) EDD_SRV_PTCP_SYNC_CONTROL: Request pending",
                            pHDB->Handle, pRQB);
                    }
                }
                else
                {
                    Status = EDD_STS_ERR_PARAM;
                    EDDP_K32_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] *** EDDP_K32Request(pRQB=0x%X) EDD_SRV_PTCP_SYNC_CONTROL: Invalid SyncId=%u",
                        pHDB->Handle, pRQB, pParam->SyncId);
                }
                break;
            }

            //-------------------------------------------------------------
            default:
            //-------------------------------------------------------------
            {
                EDDP_K32_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] *** EDDP_K32Request(pRQB=0x%X) Unknown KRISC-Service=0x%X",
                    pHDB->Handle, pRQB,EDD_RQB_GET_SERVICE(pRQB));
                Status = EDD_STS_ERR_SERVICE;
                break;
            }
        }
    }

    if (Status != EDD_STS_OK)
    {
        EDDP_GlbRequestFinish(pHDB, pRQB, Status);
    }

    EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT:EDDP_K32Request() Status=0x%X", Status);

    //LSA_UNUSED_ARG(xxx); // handle unused parameter
    return;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_K32RemotePortState                     +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+  Result                :    LSA_RESULT                                  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters          (in) +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         Opcode:     EDD_OPC_REQUEST                 +*/
/*+     LSA_HANDLE_TYPE         Handle:                                     +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user (not used)           +*/
/*+     EDD_SERVICE             Service:    EDD_SRV_SET_REMOTE_PORT_STATE   +*/
/*+     LSA_RESULT              Status:     Return status                   +*/
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:     Depend on service               +*/
/*+                                                                         +*/
/*+  RQB-return values via callback-function depends on service             +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function handles the following service for K32:      +*/
/*+                                                                         +*/
/*+               EDD_SRV_SET_REMOTE_PORT_STATE                             +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32RemotePortState(
    EDD_UPPER_RQB_PTR_TYPE             pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE            pHDB)
{
    EDD_RSP Status = EDD_STS_OK;

    EDDP_K32_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN :EDDP_K32RemotePortState(pRQB=0x%X, pDDB=0x%X, pHDB=0x%X)",
        pHDB->Handle, pRQB, pDDB, pHDB);

    //---------------------------------------------------------------------------
    // every request needs a param-ptr, so check it.
    //---------------------------------------------------------------------------
    if  (LSA_HOST_PTR_ARE_EQUAL(pRQB->pParam, LSA_NULL))
    {
        EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] *** EDDP_K32RemotePortState() RQB=0x%X: pParam is NULL",
            pHDB->Handle, pRQB);
        Status = EDD_STS_ERR_PARAM;
    }

    if (Status == EDD_STS_OK)
    {
        EDD_UPPER_SET_REMOTE_PORT_STATE_PTR_TYPE pParam = (EDD_UPPER_SET_REMOTE_PORT_STATE_PTR_TYPE)pRQB->pParam;

        if ((0 == pParam->PortID) || (pParam->PortID > pDDB->SWI.PortCnt))
        {
            EDDP_K32_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] *** EDDP_K32RemotePortState() RQB=0x%X: invalid port=%u",
                pHDB->Handle, pRQB, pParam->PortID);
            Status = EDD_STS_ERR_PARAM;
        }
        else if (EDD_SET_REMOTE_PORT_STATE_IGNORE != pParam->SyncId0_TopoState)
        {
            /* Send PortID and SyncId0_TopoState to KRISC-FW
            */
            EDDP_K32TopoStateSet(pParam->PortID, pParam->SyncId0_TopoState, pDDB);
        }
    }

    EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT:EDDP_K32RemotePortState() Status=0x%X", Status);

    LSA_UNUSED_ARG(pHDB);  // handle unused parameter
    return EDD_STS_OK;     // Always return success
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_K32TopoStateSet                        +*/
/*+  Input/Output          :    see below                                   +*/
/*+  Result                :    see below                                   +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  PortId     : PortID from EDD_SRV_SET_REMOTE_PORT_STATE            (in) +*/
/*+  TopoState  : SyncId0_TopoState from EDD_SRV_SET_REMOTE_PORT_STATE (in) +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+  Results    : none                                                      +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function transfers a K32-RQB with opcode             +*/
/*+  K32_OPC_GSY_TOPO_STATE to FW on the KRISC32                            +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_K32TopoStateSet (
LSA_UINT16              PortId,
LSA_UINT32              TopoState,
EDDP_LOCAL_DDB_PTR_TYPE pDDB)
{
    LSA_BOOL    TopoStateOk = LSA_FALSE;
    EDD_RSP     Status = EDD_STS_OK;

    EDDP_K32_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN :EDDP_K32TopoStateSet(PortID=%u, TopoState=%u, pDDB=0x%X)",
        PortId, TopoState, pDDB);

    if (EDD_SET_REMOTE_PORT_STATE_TOPO_MISMATCH == TopoState)
    {
        TopoStateOk = LSA_FALSE;
    }
    else
    {
        if (EDD_SET_REMOTE_PORT_STATE_TOPO_OK == TopoState)
        {
            TopoStateOk = LSA_TRUE;
        }
        else
        {
            EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "[H:--] *** EDDP_K32TopoStateSet() Unknown SyncId0_TopoState=%u",
                TopoState);
            Status = EDD_STS_ERR_PARAM;
        }
    }

    if ((Status == EDD_STS_OK) && (TopoStateOk != pDDB->K32.TopoStateOk[PortId-1]))
    {
        /* Save topo state
        */
        pDDB->K32.TopoStateOk[PortId-1] = TopoStateOk;

        if (!LSA_HOST_PTR_ARE_EQUAL(pDDB->K32.pHDB, LSA_NULL))
        {
            /* K32 channel is open
            */
            EDD_UPPER_KRISC32_RQB_PTR_TYPE pK32RQB = pDDB->K32.pK32PortRQB[PortId-1];

            if (!LSA_HOST_PTR_ARE_EQUAL(pK32RQB, LSA_NULL))
            {
                /* Send RQB to KRISC32
                */
                EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] --- EDDP_K32TopoStateSet() sending TopoOK=%u for port=%u",
                    TopoStateOk, PortId);
                pK32RQB->Hdr.Opcode = K32_OPC_GSY_TOPO_STATE;
                pK32RQB->Hdr.UserId.void_ptr = pDDB;
                pK32RQB->Params.TopoState.PortID = PortId;
                pK32RQB->Params.TopoState.TopoStateOk = (TopoStateOk != LSA_FALSE ? K32_TRUE : K32_FALSE);
                Status = EDDP_K32PutRQB(pK32RQB, pDDB);
            }
            else
            {
                EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] *** EDDP_K32TopoStateSet() No K32 RQB for port=%u",
                    PortId);
            }
        }
        else
        {
            EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] --- EDDP_K32TopoStateSet() K32 channel not open: saved TopoOK=%u for port=%u",
                pDDB->K32.TopoStateOk[PortId-1], PortId);
        }
    }

    EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT:EDDP_K32TopoStateSet() Status=0x%X", Status);
    return;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_K32DelaySet                            +*/
/*+  Input/Output          :    see below                                   +*/
/*+  Result                :    see below                                   +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  PortId     : Port ID from KRISK32                                 (in) +*/
/*+  LineDelay  : Line delay measured at this port in nanoseconds      (in) +*/
/*+  CableDelay : Cable delay measured at this port in nanoseconds     (in) +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+  Results    : none                                                      +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function transfers line delay and cable delay to     +*/
/*+  port data for EDD_SRV_GET_PORT_PARAMS                                  +*/
/*+  and EDD_SRV_LINK_STATUS_IND_PROVIDE_EXT                                +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_K32DelaySet (
LSA_UINT16              PortId,
LSA_UINT32              LineDelay,
LSA_UINT32              CableDelay,
LSA_UINT16              Response,
EDDP_LOCAL_HDB_PTR_TYPE pHDB)
{
    LSA_UINT8           SyncId;
    LSA_UINT32          Index;
    LSA_BOOL            Forward = LSA_TRUE;
    EDD_MAC_ADR_TYPE    Announce_MAC_Address = {{0x01, 0x0E, 0xCF, 0x00, 0x04, 0x00}};

    EDDP_K32_TRACE_05(pHDB->pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN :EDDP_K32DelaySet(PortID=%u, LineDelay=%u, CableDelay=%u, pHDB=0x%X)",
        pHDB->Handle, PortId, LineDelay, CableDelay, pHDB);

    if (Response != K32_RSP_OK)
    {
        EDDP_K32_TRACE_03(pHDB->pDDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "[H:%2X] *** EDDP_K32DelaySet(PortId=%u) Response=0x%X",
            pHDB->Handle, PortId, Response);
    }

    if ((LineDelay != pHDB->pDDB->SWI.LinkStatus[PortId].LineDelayInTicksMeasured)
    ||  (CableDelay != pHDB->pDDB->SWI.LinkStatus[PortId].CableDelayInNsMeasured))
    {
        if (((0 == LineDelay) && (0 != pHDB->pDDB->SWI.LinkStatus[PortId].LineDelayInTicksMeasured))
        ||  ((0 != LineDelay) && (0 == pHDB->pDDB->SWI.LinkStatus[PortId].LineDelayInTicksMeasured)))
        {
            /* Enable/disable hardware forwarding of announce frames
            */
            if (0 == LineDelay)
            {
                Forward = LSA_FALSE;
            }
            for (SyncId = 0; SyncId < 2; SyncId++)
            {
                /* Enable only if no boundary is set for announce frames
                */
                if (!Forward || (SyncId >= EDDP_PRM_DOMAIN_BOUNDARY_COUNT) || !pHDB->pDDB->SWI.AnnounceBoundary[PortId][SyncId])
                {
                    EDDP_K32_TRACE_04(pHDB->pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] --- EDDP_K32DelaySet(PortId=%u) Announce Forward=%u SyncId=%u",
                        pHDB->Handle, PortId, Forward, SyncId);
                    Announce_MAC_Address.MacAdr[5] |= SyncId;
                    Index = EDDP_SWI_MC_Entry_Get_Index(pHDB->pDDB, Announce_MAC_Address);
                    EDDP_SWI_ChangeAnnounceFwd(pHDB->pDDB, Index, (LSA_UINT32)PortId, Forward, SyncId);
                }
            }
        }
        /* Set LinkStatus delay values for port and initiate ExtLinkIndication
        */
        EDDP_K32_TRACE_04(pHDB->pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] --- EDDP_K32DelaySet(PortId=%u) LineDelay=%u CableDelay=%u",
            pHDB->Handle, PortId, LineDelay, CableDelay);

        pHDB->pDDB->SWI.LinkStatus[PortId].LineDelayInTicksMeasured = LineDelay;
        pHDB->pDDB->SWI.LinkStatus[PortId].CableDelayInNsMeasured = CableDelay;

        EDDP_GENIndicateLinkChange_Ext(pHDB->pDDB, (LSA_UINT32)PortId);

        // trigger update of IRT_DelayTime_Px register
        EDDP_SWI_Set_IRT_DelayTime(pHDB->pDDB, (LSA_UINT32)PortId);
    }

    EDDP_K32_TRACE_01(pHDB->pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT:EDDP_K32DelaySet() Response=0x%X", Response);
    return;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_K32SyncSet                             +*/
/*+  Input/Output          :    see below                                   +*/
/*+  Result                :    see below                                   +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                    (in) +*/
/*+  SyncId     : Sync ID from KRISK32 (0 = clock, 1 = time)           (in) +*/
/*+  SyncOk     : Synchronisation is reached if LSA_TRUE else lost     (in) +*/
/*+  SyncState  : Slave sync state for timeStamp                       (in) +*/
/*+  SecondsHigh: PTCP epoch for timeStamp                             (in) +*/
/*+  UTCOffset  : current UTCOffset for timeStamp                      (in) +*/
/*+                                                                         +*/
/*+  Results    : none                                                      +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function transfers KRISC32 synchronisation state to  +*/
/*+  RTClass3 state machine on clock sync indication.                       +*/
/*+  On time sync indication the other params are stored for TimeStampGet   +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_K32SyncSet (
EDDP_LOCAL_HDB_PTR_TYPE pHDB,
LSA_UINT8               SyncId,
LSA_BOOL                SyncOk,
K32_RQB_PTR_TYPE        pK32RQB)
{
    LSA_UINT32  PortID;
    EDDP_K32_TRACE_04(pHDB->pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN :EDDP_K32SyncSet(SyncId:%u, SyncOk=%u, pHDB=0x%X)",
        pHDB->Handle, SyncId, SyncOk, pHDB);

    EDDP_K32_TRACE_03(pHDB->pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:%2X] !!! EDDP_K32SyncSet(%02u) sync state change ->%d",
        pHDB->Handle, SyncId, SyncOk);

    if (0 == SyncId)
    {
        /* Clock sync indication: store current sync state and triggger RTC3 port state machine
        */
        pHDB->pDDB->SWI.SyncOk = SyncOk;
        for ( PortID=1; PortID <= pHDB->pDDB->SWI.PortCnt; PortID++ )
        {
            EDDP_SWI_Trigger_RTC3_PortState(pHDB->pDDB, PortID);
        }
    }
    else
    {
        /* Time sync indication:
        */
        EDD_RSP                     Response;
        LSA_UINT64                  nanoseconds;
        EDDP_TIMESTAMP_INFO_TYPE    TimestampNew;

        // old time (timestamp of SyncState Change in the old time context)
        // Store time into 64bit nanoseconds
        nanoseconds   = EDDP_HAL_MEM32_READ(pHDB->pDDB,&(pK32RQB->Params.SyncInd.OctHighTimeOld));
        nanoseconds <<= 32;
        nanoseconds  |= EDDP_HAL_MEM32_READ(pHDB->pDDB,&(pK32RQB->Params.SyncInd.OctLowTimeOld));

        // Update time values in the old Timestamp
        pHDB->pDDB->K32.TimestampInfo.TimesStamp.Nanoseconds = (LSA_UINT32)(nanoseconds % 1000000000);
        pHDB->pDDB->K32.TimestampInfo.TimesStamp.SecondsLow  = (LSA_UINT32)(nanoseconds / 1000000000);

        // New time (timestamp of SyncState Change in the new time context)
        // Store time into 64bit nanoseconds
        nanoseconds   = EDDP_HAL_MEM32_READ(pHDB->pDDB,&(pK32RQB->Params.SyncInd.OctHighTimeNew));
        nanoseconds <<= 32;
        nanoseconds  |= EDDP_HAL_MEM32_READ(pHDB->pDDB,&(pK32RQB->Params.SyncInd.OctLowTimeNew));

        // Get the current time with the current SyncState
        Response = eddp_GetSyncTime(pHDB->pDDB->hDDB, &TimestampNew);
        if (Response != EDD_STS_OK)
        {
            EDDP_K32_TRACE_02(pHDB->pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] *** EDDP_K32SyncSet(): eddp_GetSyncTime() failed, Response=0x%X", 
                              pHDB->Handle, Response);
        }
        else if (  (EDDP_PNIO_STATUS_NOT_SYNC != TimestampNew.TimesStamp.Status)    // As long as NOT_SYNC, the SeqID is manipulated.
                && (TimestampNew.SequenceID != EDDP_HAL_MEM32_READ(pHDB->pDDB,&(pK32RQB->Params.SyncInd.SequenceID))))
        {
            EDDP_K32_TRACE_03(pHDB->pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] *** EDDP_K32SyncSet(): Current SequenceID(%d) does not match the SyncInd SequenceID(%d)", 
                              pHDB->Handle, TimestampNew.SequenceID, EDDP_HAL_MEM32_READ(pHDB->pDDB,&(pK32RQB->Params.SyncInd.SequenceID)));
        }
        else
        {
            // Update time values in the new Timestamp
            TimestampNew.TimesStamp.Nanoseconds = (LSA_UINT32)(nanoseconds % 1000000000);
            TimestampNew.TimesStamp.SecondsLow  = (LSA_UINT32)(nanoseconds / 1000000000);

            // In case of being never in Sync until now, add a temporary Sync Indication to meet the state machine requirements 
            if ( (EDDP_PNIO_STATUS_NOT_SYNC   == pHDB->pDDB->K32.TimestampInfo.TimesStamp.Status) &&
                 (EDDP_PNIO_STATUS_LOCAL_SYNC == TimestampNew.TimesStamp.Status) )
            {
                TimestampNew.TimesStamp.Status = EDDP_PNIO_STATUS_MASTER_SYNC;
                TimestampNew.SequenceID        = pHDB->pDDB->K32.TimestampInfo.SequenceID + 1;

                // Write Sync Indication to the shared memory
                Response = EDDP_CList_WriteTimeSyncInd(pHDB->pDDB, &pHDB->pDDB->K32.TimestampInfo, &TimestampNew);
                switch (Response)
                {
                    case EDD_STS_OK:
                        // Output macro signals the change of the Time Sync State to the application
                        EDDP_SIGNAL_TIME_SYNC_STATE_CHANGE();
                        break;
                    case EDD_STS_ERR_LIMIT_REACHED:
                        EDDP_K32_TRACE_02(pHDB->pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,  "[H:%2X] *** EDDP_K32SyncSet(): (A->C) EDDP_CList_WriteTimeSyncInd() failed, list is full, Response=0x%X", pHDB->Handle, Response);
                        break;
                    case EDD_STS_ERR_NOT_SUPPORTED:
                        EDDP_K32_TRACE_02(pHDB->pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN,  "[H:%2X] *** EDDP_K32SyncSet(): (A->C) EDDP_CList_WriteTimeSyncInd() No Shared Memory, Response=0x%X", pHDB->Handle, Response);
                        break;
                    default:
                        EDDP_K32_TRACE_02(pHDB->pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] *** EDDP_K32SyncSet(): (A->C) EDDP_CList_WriteTimeSyncInd() failed, Response=0x%X", pHDB->Handle, Response);
                        break;
                }

                EDDP_K32_TRACE_06(pHDB->pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:%2X] (A->C) OLD EDDP_K32SyncSet(%02u) Status=%u SyncState=%u SecondsHigh=%u UTCOffset=%u",
                    pHDB->Handle, SyncId, 
                    pHDB->pDDB->K32.TimestampInfo.TimesStamp.Status, 
                    pHDB->pDDB->K32.TimestampInfo.SyncState, 
                    pHDB->pDDB->K32.TimestampInfo.TimesStamp.SecondsHigh, 
                    pHDB->pDDB->K32.TimestampInfo.UTCOffset);

                // Copy new timestamp to the DDB
                EDDP_MEMCOPY_COMMON(&pHDB->pDDB->K32.TimestampInfo, &TimestampNew, sizeof(EDDP_TIMESTAMP_INFO_TYPE));

                EDDP_K32_TRACE_06(pHDB->pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:%2X] (A->C) NEW EDDP_K32SyncSet(%02u) Status=%u SyncState=%u SecondsHigh=%u UTCOffset=%u",
                    pHDB->Handle, SyncId, 
                    pHDB->pDDB->K32.TimestampInfo.TimesStamp.Status, 
                    pHDB->pDDB->K32.TimestampInfo.SyncState, 
                    pHDB->pDDB->K32.TimestampInfo.TimesStamp.SecondsHigh, 
                    pHDB->pDDB->K32.TimestampInfo.UTCOffset);

                // Prepare New TimeStampInfo for the next regular Sync Indication
                TimestampNew.TimesStamp.Status = EDDP_PNIO_STATUS_LOCAL_SYNC;
                TimestampNew.SequenceID        = pHDB->pDDB->K32.TimestampInfo.SequenceID + 1;
                EDDP_MEMSET_COMMON(TimestampNew.MasterIdent, 0, sizeof(TimestampNew.MasterIdent));
                EDDP_MEMSET_COMMON(TimestampNew.DomainIdent, 0, sizeof(TimestampNew.DomainIdent));
            }

            // Write Sync Indication to the shared memory
            Response = EDDP_CList_WriteTimeSyncInd(pHDB->pDDB, &pHDB->pDDB->K32.TimestampInfo, &TimestampNew);
            switch (Response)
            {
                case EDD_STS_OK:
                    // Output macro signals the change of the Time Sync State to the application
                    EDDP_SIGNAL_TIME_SYNC_STATE_CHANGE();
                    break;
                case EDD_STS_ERR_LIMIT_REACHED:
                    EDDP_K32_TRACE_02(pHDB->pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,  "[H:%2X] *** EDDP_K32SyncSet(): EDDP_CList_WriteTimeSyncInd() failed, list is full, Response=0x%X", pHDB->Handle, Response);
                    break;
                case EDD_STS_ERR_NOT_SUPPORTED:
                    EDDP_K32_TRACE_02(pHDB->pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN,  "[H:%2X] *** EDDP_K32SyncSet(): EDDP_CList_WriteTimeSyncInd() No Shared Memory, Response=0x%X", pHDB->Handle, Response);
                    break;
                default:
                    EDDP_K32_TRACE_02(pHDB->pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] *** EDDP_K32SyncSet(): EDDP_CList_WriteTimeSyncInd() failed, Response=0x%X", pHDB->Handle, Response);
                    break;
            }

            EDDP_K32_TRACE_06(pHDB->pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:%2X] OLD EDDP_K32SyncSet(%02u) Status=%u SyncState=%u SecondsHigh=%u UTCOffset=%u",
                pHDB->Handle, SyncId, 
                pHDB->pDDB->K32.TimestampInfo.TimesStamp.Status, 
                pHDB->pDDB->K32.TimestampInfo.SyncState, 
                pHDB->pDDB->K32.TimestampInfo.TimesStamp.SecondsHigh, 
                pHDB->pDDB->K32.TimestampInfo.UTCOffset);

            // Copy new timestamp to the DDB
            EDDP_MEMCOPY_COMMON(&pHDB->pDDB->K32.TimestampInfo, &TimestampNew, sizeof(EDDP_TIMESTAMP_INFO_TYPE));

            EDDP_K32_TRACE_06(pHDB->pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:%2X] NEW EDDP_K32SyncSet(%02u) Status=%u SyncState=%u SecondsHigh=%u UTCOffset=%u",
                pHDB->Handle, SyncId, 
                pHDB->pDDB->K32.TimestampInfo.TimesStamp.Status, 
                pHDB->pDDB->K32.TimestampInfo.SyncState, 
                pHDB->pDDB->K32.TimestampInfo.TimesStamp.SecondsHigh, 
                pHDB->pDDB->K32.TimestampInfo.UTCOffset);

            // Write a backup of the TimeStampInfo in the Shared Memory
            if (   (!LSA_HOST_PTR_ARE_EQUAL(pHDB->pDDB->pUserCmdSharedMem, LSA_NULL))
                && (EDD_STS_OK != EDDP_CopyTimeStamp(&pHDB->pDDB->pUserCmdSharedMem->TimestampInfoBackup, &TimestampNew))  )
            {
                EDDP_K32_TRACE_02(pHDB->pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] *** EDDP_K32SyncSet(): Backup of the TimeStampInfo failed! Response=0x%X", pHDB->Handle, Response);
            }
        }
    }

    EDDP_K32_TRACE_00(pHDB->pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT:EDDP_K32SyncSet()");
    return;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_K32CopyUpperRQB                        +*/
/*+  Input/Output          :    see below                                   +*/
/*+  Result                :    none                                        +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pK32RBu    : Pointer to KRISC32 request block from user           (in) +*/
/*+  pK32RBl    : Pointer to KRISC32 request block to FW               (in) +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+  Results    : none                                                      +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function transfers a upper K32-RQB to the lower one  +*/
/*+               converting the endianess.                                 +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_K32CopyUpperRQB(
    EDDP_LOCAL_DDB_PTR_TYPE pDDB,
    EDD_UPPER_KRISC32_RQB_PTR_TYPE  pK32RBu,
    volatile K32_RQB_TYPE*      pK32RBl)
{
    LSA_UINT32  Idx;

    EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN :EDDP_K32CopyUpperRQB(pK32RBu=0x%X pK32RBl=0x%X)",
        pK32RBu, pK32RBl);

    /* Transfer header and set response to OK
    */
    EDDP_HAL_MEM8_WRITE(pDDB,&(pK32RBl->Hdr.InterfaceID),pK32RBu->Hdr.InterfaceID);
    EDDP_HAL_MEM8_WRITE(pDDB,&(pK32RBl->Hdr.Opcode),pK32RBu->Hdr.Opcode);
    EDDP_HAL_MEM32_WRITE(pDDB,&(pK32RBl->Hdr.UserId.uvar32),pK32RBu->Hdr.UserId.uvar32);
    EDDP_HAL_MEM16_WRITE(pDDB,&(pK32RBl->Hdr.Response),K32_RSP_OK);

    /* Transfer parameter union depending on opcode
    */
    switch (EDDP_HAL_MEM8_READ(pDDB,&(pK32RBl->Hdr.Opcode)))
    {
    case K32_OPC_START:
    case K32_OPC_STOP:
        /* No parameter
        */
        break;

    case K32_OPC_SETUP:
        EDDP_HAL_MEM16_WRITE(pDDB,&(pK32RBl->Params.Setup.PP0_PortID),pK32RBu->Params.Setup.PP0_PortID);
        EDDP_HAL_MEM16_WRITE(pDDB,&(pK32RBl->Params.Setup.PP1_PortID),pK32RBu->Params.Setup.PP1_PortID);
        EDDP_HAL_MEM16_WRITE(pDDB,&(pK32RBl->Params.Setup.PP2_PortID),pK32RBu->Params.Setup.PP2_PortID);
        EDDP_HAL_MEM16_WRITE(pDDB,&(pK32RBl->Params.Setup.PP3_PortID),pK32RBu->Params.Setup.PP3_PortID);
        EDDP_HAL_MEM32_WRITE(pDDB,&(pK32RBl->Params.Setup.TraceIdx),pK32RBu->Params.Setup.TraceIdx);
        break;

    case K32_OPC_GSY_TOPO_STATE:
        EDDP_HAL_MEM16_WRITE(pDDB,&(pK32RBl->Params.TopoState.PortID),pK32RBu->Params.TopoState.PortID);
        EDDP_HAL_MEM8_WRITE(pDDB,&(pK32RBl->Params.TopoState.TopoStateOk),pK32RBu->Params.TopoState.TopoStateOk);
        break;

    case K32_OPC_GSY_SYNC:
        EDDP_HAL_MEM8_WRITE(pDDB,&(pK32RBl->Params.SyncReq.SyncId), pK32RBu->Params.SyncReq.SyncId);
        EDDP_HAL_MEM8_WRITE(pDDB,&(pK32RBl->Params.SyncReq.SlaveState),pK32RBu->Params.SyncReq.SlaveState);
        EDDP_HAL_MEM8_WRITE(pDDB,&(pK32RBl->Params.SyncReq.MasterState),pK32RBu->Params.SyncReq.MasterState);
        EDDP_HAL_MEM8_WRITE(pDDB,&(pK32RBl->Params.SyncReq.LengthTLV),pK32RBu->Params.SyncReq.LengthTLV);
        EDDP_HAL_MEM32_WRITE(pDDB,&(pK32RBl->Params.SyncReq.SyncSendFactor),pK32RBu->Params.SyncReq.SyncSendFactor);
        EDDP_HAL_MEM16_WRITE(pDDB,&(pK32RBl->Params.SyncReq.TimeoutFactor),pK32RBu->Params.SyncReq.TimeoutFactor);
        EDDP_HAL_MEM16_WRITE(pDDB,&(pK32RBl->Params.SyncReq.TakeoverFactor),pK32RBu->Params.SyncReq.TakeoverFactor);
        EDDP_HAL_MEM32_WRITE(pDDB,&(pK32RBl->Params.SyncReq.PLLWindow),pK32RBu->Params.SyncReq.PLLWindow);
        for (Idx = 0; Idx < pK32RBu->Params.SyncReq.LengthTLV; Idx++)
        {
            EDDP_HAL_MEM8_WRITE(pDDB,&(pK32RBl->Params.SyncReq.TLV[Idx]),pK32RBu->Params.SyncReq.TLV[Idx]);
        }
        break;

    case K32_OPC_GSY_DELAY_REQ:
        EDDP_HAL_MEM8_WRITE(pDDB,&(pK32RBl->Params.DelayReq.ReqActive),pK32RBu->Params.DelayReq.ReqActive);
        EDDP_HAL_MEM8_WRITE(pDDB,&(pK32RBl->Params.DelayReq.RspActive),pK32RBu->Params.DelayReq.RspActive);
        {
            LSA_UINT32  iMacAddr;
            for ( iMacAddr=0; iMacAddr < EDD_MAC_ADDR_SIZE; iMacAddr++ )
            {
                EDDP_HAL_MEM8_WRITE(pDDB,&(pK32RBl->Params.DelayReq.PortMacAddr[iMacAddr]), pK32RBu->Params.DelayReq.PortMacAddr[iMacAddr]);
            }
        }
        EDDP_HAL_MEM16_WRITE(pDDB,&(pK32RBl->Params.DelayReq.PortID),pK32RBu->Params.DelayReq.PortID);
        EDDP_HAL_MEM32_WRITE(pDDB,&(pK32RBl->Params.DelayReq.RxPortDelay),pK32RBu->Params.DelayReq.RxPortDelay);
        EDDP_HAL_MEM32_WRITE(pDDB,&(pK32RBl->Params.DelayReq.TxPortDelay),pK32RBu->Params.DelayReq.TxPortDelay);
        break;

    default:
        EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] *** EDDP_K32CopyUpperRQB() Invalid opcode=%u at addr=0x%X",
            EDDP_HAL_MEM8_READ(pDDB,&(pK32RBl->Hdr.Opcode)), &pK32RBl->Hdr.Opcode);
        EDDP_FATAL(0);
        break;
    }

    LSA_UNUSED_ARG(pDDB); // handle unused parameter

    EDDP_K32_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT:EDDP_K32CopyUpperRQB()");
    return;
}

#ifndef K32_LOG_TO_TRACE
#include <stdio.h>
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_K32Log                                 +*/
/*+  Input/Output          :    K32_RQB_PTR_TYPE         pK32RQB            +*/
/*+  Result                :    none                                        +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pLogBuffer : Pointer to KRISC32 trace buffer data                 (in) +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+  Results    : none                                                      +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function transfers a KRISC32 trace buffer as hex     +*/
/*+               dump into the trace files.                                +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_K32Log(
    K32_TRACE_BUFFER    *pLogBuffer)
{
    static FILE *k32_LogHandle = NULL;
    static char         *k32_LogFile = "krisc32.bin";
    static char         *k32_LogNumStr = "%03u.bin";
    static char         k32_LogNumFile[16] = {"krisc32.000.bin"};
    static unsigned int k32_LogNumMax = 999;
    static unsigned int k32_LogNum = 0;
    static unsigned int k32_LogBytes = 0;
    static unsigned int k32_LogBytesMax = 100000000;    // max. 100MB

    if (NULL == k32_LogHandle)
    {
        k32_LogHandle = fopen( k32_LogFile, "wb");
        if (k32_LogHandle == NULL)
        {
            EDDP_K32_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR,"[H:--] *** EDDP_K32Log() cannot open file: krisc32.bin");
        }
    }

    if ((NULL != k32_LogHandle) && (0 != pLogBuffer->elements))
    {
        fwrite(&pLogBuffer->sequenceID, sizeof(pLogBuffer->sequenceID), 1, k32_LogHandle);
        fwrite(&pLogBuffer->elements, sizeof(pLogBuffer->elements), 1, k32_LogHandle);
        fwrite(&pLogBuffer->data, sizeof(pLogBuffer->data[0]), (size_t)pLogBuffer->elements, k32_LogHandle);
        k32_LogBytes += sizeof(LSA_UINT32) + sizeof(pLogBuffer->elements) + EDDP_K32_L2H32(pLogBuffer->elements) * sizeof(pLogBuffer->data[0]);
        fflush(k32_LogHandle);
        if (k32_LogBytes >= k32_LogBytesMax)
        {
            if (k32_LogNum > k32_LogNumMax)
                k32_LogNum = 0;
            sprintf(k32_LogNumFile + 8, k32_LogNumStr, k32_LogNum++);
            fclose(k32_LogHandle);
            remove(k32_LogNumFile);
            rename(k32_LogFile, k32_LogNumFile);
            k32_LogHandle = NULL;
            k32_LogBytes = 0;
        }
    }
}
#endif  //not K32_LOG_TO_TRACE
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_K32GetRQB                              +*/
/*+  Input/Output          :    K32_RQB_PTR_TYPE         pK32RQB            +*/
/*+  Result                :    none                                        +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pK32RQB    : Pointer to KRISC32 request block from FW             (in) +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+  Results    : LSA_TRUE if more RRQBs could be processed,                +*/
/*+               LSA_FALSE if a shoutdown is pending:                      +*/
/*+               RRQB queues shouldn't be accessed any more.               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function transfers a K32-RQB to the user.            +*/
/*+               The K32-RRQB is send from KRISC32. It will be             +*/
/*+               acknowleged on end of function.                           +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_K32GetRQB(
    EDDP_LOCAL_DDB_PTR_TYPE  pDDB,
    K32_RQB_PTR_TYPE         pK32RQB)
{
    //K32_RQB_TYPE          K32RQB; // in K32GetRQB we needn't to copy RRQB, since we acknowlege it on end of function

    EDDP_LOCAL_HDB_PTR_TYPE pHDB = LSA_NULL;
    EDD_UPPER_RQB_PTR_TYPE  pRQB = LSA_NULL;
    EDD_UPPER_RQB_PTR_TYPE  pRQBNext = LSA_NULL;
    EDD_UPPER_KRISC32_RQB_PTR_TYPE  pK32UpperRQB = LSA_NULL;
    EDD_RSP     Status = EDD_STS_OK;
    LSA_BOOL    retval = LSA_TRUE;
    LSA_UINT16  Response = K32_RSP_OK;
    LSA_UINT8   Opcode; // = EDDP_HAL_MEM8_READ(pDDB, &(pK32RQB->Hdr.Opcode));
    LSA_USER_ID_TYPE    UserId;
    LSA_UINT16          PortIdx;

    EDDP_ASSERT_NO_NULL_PTR(pDDB);
    EDDP_ASSERT_NO_NULL_PTR(pK32RQB);

    Opcode = EDDP_HAL_MEM8_READ(pDDB, &(pK32RQB->Hdr.Opcode));

    UserId.uvar32 = EDDP_HAL_MEM32_READ(pDDB,&(pK32RQB->Hdr.UserId.uvar32));

    EDDP_K32_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN :EDDP_K32GetRQB(IF[%d], pK32RQB=0x%X Opcode=%u UserId=0x%X)",
        InterfaceID, pK32RQB, Opcode, UserId.uvar32);

    if ((K32_OPC_GSY_DIAG == Opcode)
    ||  (K32_OPC_GSY_DELAY_IND == Opcode)
    ||  (K32_OPC_GSY_SYNC_IND == Opcode))
    {
        /* Pointer to interface data is in UserId: get pHDB from K32 interface data
        */
        EDDP_ASSERT(pDDB == (EDDP_LOCAL_DDB_PTR_TYPE)UserId.void_ptr);

        if (!LSA_HOST_PTR_ARE_EQUAL(pDDB, LSA_NULL))
        {
            pHDB = pDDB->K32.pHDB;
            if (!LSA_HOST_PTR_ARE_EQUAL(pHDB, LSA_NULL))
            {
                if (K32_OPC_GSY_DIAG == Opcode)
                {
                    /* Get upper RQB from Queue
                    */
                    pRQB = EDDP_GlbRQBQueueRemoveFromBegin(&pHDB->pK32->DiagReqQueue);
                    if (!LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL))
                    {
                        EDD_UPPER_PTCP_DIAG_IND_PROVIDE_PTR_TYPE pParam = (EDD_UPPER_PTCP_DIAG_IND_PROVIDE_PTR_TYPE)pRQB->pParam;
                        pParam->SyncId = EDDP_HAL_MEM8_READ(pDDB,&(pK32RQB->Params.DiagInd.SyncId));
                        pParam->DiagSrc = EDDP_HAL_MEM8_READ(pDDB,&(pK32RQB->Params.DiagInd.DiagSrc));
                        pParam->SlaveState = EDDP_HAL_MEM8_READ(pDDB,&(pK32RQB->Params.DiagInd.SlaveState));
                        pParam->RcvSyncPrio = EDDP_HAL_MEM8_READ(pDDB,&(pK32RQB->Params.DiagInd.RcvSyncPrio));
                        {
                            LSA_UINT32  iMacAddr;
                            for ( iMacAddr=0; iMacAddr < EDD_MAC_ADDR_SIZE; iMacAddr++ )
                            {
                                pParam->MasterMacAddr.MacAdr[iMacAddr] = EDDP_HAL_MEM8_READ(pDDB,&(pK32RQB->Params.DiagInd.MasterMacAddr[iMacAddr]));
                            }
                        }
                        pParam->SequenceId = EDDP_HAL_MEM16_READ(pDDB,&(pK32RQB->Params.DiagInd.SequenceId));
                        pParam->RateInterval = EDDP_HAL_MEM32_READ(pDDB,&(pK32RQB->Params.DiagInd.RateInterval));
                        Status = EDD_STS_OK;
                    }
                    else
                    {
                        EDDP_K32_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] *** EDDP_K32GetRQB() No upper RQB in DiagReqQueue");
                    }
                }
                else
                {
                    /* Copy K32-RQB into lokal K32-RQB
                    */
                    //K32RQB = *pK32RQB;
                }
            }
            else
            {
                Status = EDD_STS_ERR_CHANNEL_USE;
                EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] *** EDDP_K32GetRQB() pDDB=0x%X Opcode=%u: K32 channel not open",
                    pDDB, Opcode);
            }
        }
        else
        {
            Status = EDD_STS_ERR_PARAM;
            EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] *** EDDP_K32GetRQB() Opcode=%u: pDDB=LSA_NULL",
                Opcode);
        }
    }
    else if (K32_OPC_GSY_DELAY_CNF == Opcode)
    {
        /* Pointer to upper RQB is in UserId: get pHDB from handle
        */
        pRQB = (EDD_UPPER_RQB_PTR_TYPE)UserId.void_ptr;
        if (!LSA_HOST_PTR_ARE_EQUAL( pRQB, LSA_NULL))
        {
            //pDDB = pDDB->pIF[InterfaceID];                            //250711lrg
            //if (!LSA_HOST_PTR_ARE_EQUAL(pDDB, LSA_NULL))          //250711lrg
            {
                pHDB = pDDB->K32.pHDB;
                if (!LSA_HOST_PTR_ARE_EQUAL(pHDB, LSA_NULL))
                {
                    /* Remove LSA-RQB from queue get K32 response for Confirmation
                    */
                    if (EDDP_GlbRQBQueueRemoveFromQueue(&pHDB->pK32->CmdReqQueue, pRQB))
                    {
                        Response = EDDP_HAL_MEM16_READ(pDDB,&(pK32RQB->Hdr.Response));
                        if (K32_RSP_OK == Response)
                        {
                            Status = EDD_STS_OK;
                        }
                        else
                        {
                            EDDP_K32_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] *** EDDP_K32GetRQB() pK32RQB=0x%X Opcode=%u Response=0x%x",
                                pK32RQB, Opcode, Response);
                            Status = EDD_STS_ERR_HW;
                        }
                        /* Look for pending upper request block
                        */
                        PortIdx = EDDP_HAL_MEM16_READ(pDDB,&(pK32RQB->Params.DelayReq.PortID)) - 1;
                        pK32UpperRQB = pDDB->K32.pK32DelayRQB[PortIdx];
                        pRQBNext = EDDP_GlbRQBQueueRemoveFromBegin(&pHDB->pK32->DelayReqQueue[PortIdx]);

                        /* Mark upper K32 RQB not longer in use
                        */
                        pK32UpperRQB->Hdr.UserId.void_ptr = LSA_NULL;
                    }
                    else
                    {
                        EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] *** EDDP_K32GetRQB() pRQB=0x%X not in CmdReqQueue",
                            pRQB);
                        pRQB = LSA_NULL;
                    }
                }
                else
                {
                    EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] *** EDDP_K32GetRQB() pRQB=0x%X Opcode=%u: K32 channel not open",
                        pRQB, Opcode);
                    pRQB = LSA_NULL;
                }
            }
            //else
            //{
            //  Status = EDD_STS_ERR_PARAM;
            //  EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] *** EDDP_K32GetRQB() Opcode=%u: pDDB=LSA_NULL",
            //      Opcode);
            //}
        }
        else
        {
            EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] *** EDDP_K32GetRQB() Opcode=%u: Upper RQB=LSA_NULL",
                Opcode);
        }
    }

    switch (Opcode)
    {
    case K32_OPC_GSY_DIAG:
    case K32_OPC_GSY_DELAY_CNF:
        if (!LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL)
        &&  !LSA_HOST_PTR_ARE_EQUAL(pHDB, LSA_NULL))
        {
            /* Send back upper RQB to GSY
            */
            EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] --- EDDP_K32GetRQB() Sending pRQB=0x%X to GSY Opcode=%u",
                pRQB, Opcode);
            EDDP_GlbRequestFinish(pHDB, pRQB, Status);
            EDDP_HAL_MEM16_WRITE(pDDB,&(pK32RQB->Hdr.Response),K32_RSP_OK);
            if (!LSA_HOST_PTR_ARE_EQUAL(pRQBNext, LSA_NULL))
            {
                /* Process next upper command RQB
                */
                EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] --- EDDP_K32GetRQB() Sending pending pRQB=0x%X to KRISC32 Opcode=%u",
                    pRQBNext, Opcode);
                Status = EDDP_K32RequestDelay(pRQBNext, pDDB, pHDB, pK32UpperRQB);
                if (Status != EDD_STS_OK)
                {
                    /* On error: send back next upper RQB to GSY
                    */
                    EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] *** EDDP_K32GetRQB() Status=0x%X sending pRQB=0x%X to KRISC32",
                        Status, pRQB);
                    EDDP_GlbRequestFinish(pHDB, pRQBNext, Status);
                }
            }
        }
        break;

    case K32_OPC_GSY_DELAY_IND:
        if (!LSA_HOST_PTR_ARE_EQUAL(pHDB, LSA_NULL))
        {
            /* Indicate new line and cable delay
            */
            EDDP_K32DelaySet(EDDP_HAL_MEM16_READ(pDDB,&(pK32RQB->Params.DelayInd.PortID)),
                             EDDP_HAL_MEM32_READ(pDDB,&(pK32RQB->Params.DelayInd.LineDelay)),
                             EDDP_HAL_MEM32_READ(pDDB,&(pK32RQB->Params.DelayInd.CableDelay)),
                             EDDP_HAL_MEM16_READ(pDDB,&(pK32RQB->Hdr.Response)), pHDB);
            EDDP_HAL_MEM16_WRITE(pDDB,&(pK32RQB->Hdr.Response),K32_RSP_OK);
        }
        break;

    case K32_OPC_GSY_SYNC_IND:
        if (!LSA_HOST_PTR_ARE_EQUAL(pHDB, LSA_NULL))
        {
            /* Indicate synchronisation state
            */
            EDDP_K32SyncSet(pHDB, EDDP_HAL_MEM8_READ(pDDB,&(pK32RQB->Params.SyncInd.SyncId)),
                            (EDDP_HAL_MEM8_READ(pDDB,&(pK32RQB->Params.SyncInd.SyncOk)) == K32_TRUE ? LSA_TRUE : LSA_FALSE),
                            pK32RQB);
            EDDP_HAL_MEM16_WRITE(pDDB,&(pK32RQB->Hdr.Response),K32_RSP_OK);
        }
        break;
    case K32_OPC_LOG:
        {
#if 0 //def K32_LOG_TO_TRACE
            {
                LSA_UINT32  elements = EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->elements));
                #if (EDDP_CFG_TRACE_MODE != 0)
                LSA_UINT32  sequenceID = EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->sequenceID));
                #endif
                EDDP_K32_FW_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "<K32LOG_BEGIN SID='%X' ELM='%d' />",
                    sequenceID, elements);

                for(i = 0; i < elements; /* i is updated in for-block */ )
                {
                    switch( (elements - i) > 15 ? 15 : (elements - i))  /* only 15 elements maximum. */
                    {
                    case 1: /* SID + 1 element */
                        EDDP_K32_FW_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "<K32LOG SID='%X'>%X</K32LOG>",
                            sequenceID,
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+0])));
                        i+=1;
                        break;
                    case 2: /* 2 elements */
                        EDDP_K32_FW_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "<K32LOG SID='%X'>%X %X</K32LOG>",
                            sequenceID,
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+0])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+1])));
                        i+=2;
                        break;
                    case 3: /* 3 elements */
                        EDDP_K32_FW_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "<K32LOG SID='%X'>%X %X %X</K32LOG>",
                            sequenceID,
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+0])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+1])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+2])));
                        i+=3;
                        break;
                    case 4: /* 4 elements */
                        EDDP_K32_FW_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "<K32LOG SID='%X'>%X %X %X %X</K32LOG>",
                            sequenceID,
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+0])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+1])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+2])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+3])));
                        i+=4;
                        break;
                    case 5: /* 5 elements */
                        EDDP_K32_FW_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "<K32LOG SID='%X'>%X %X %X %X %X</K32LOG>",
                            sequenceID,
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+0])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+1])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+2])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+3])),
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+4])));
                        i+=5;
                        break;
                    case 6: /* 6 elements */
                        EDDP_K32_FW_TRACE_07(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "<K32LOG SID='%X'>%X %X %X %X %X %X</K32LOG>",
                            sequenceID,
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+0])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+1])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+2])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+3])),
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+4])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+5])));
                        i+=6;
                        break;
                    case 7: /* 7 elements */
                        EDDP_K32_FW_TRACE_08(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "<K32LOG SID='%X'>%X %X %X %X %X %X %X</K32LOG>",
                            sequenceID,
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+0])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+1])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+2])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+3])),
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+4])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+5])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+6])));
                        i+=7;
                        break;
                    case 8: /* 8 elements */
                        EDDP_K32_FW_TRACE_09(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "<K32LOG SID='%X'>%X %X %X %X %X %X %X %X</K32LOG>",
                            sequenceID,
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+0])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+1])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+2])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+3])),
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+4])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+5])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+6])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+7])));
                        i+=8;
                        break;
                    case 9: /* 9 elements */
                        EDDP_K32_FW_TRACE_10(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "<K32LOG SID='%X'>%X %X %X %X %X %X %X %X %X</K32LOG>",
                            sequenceID,
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+0])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+1])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+2])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+3])),
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+4])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+5])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+6])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+7])),
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+8])));
                        i+=9;
                        break;
                    case 10:/* 10 elements */
                        EDDP_K32_FW_TRACE_11(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "<K32LOG SID='%X'>%X %X %X %X %X %X %X %X %X %X</K32LOG>",
                            sequenceID,
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+0])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+1])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+2])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+3])),
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+4])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+5])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+6])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+7])),
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+8])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+9])));
                        i+=10;
                        break;
                    case 11:/* 11 elements */
                        EDDP_K32_FW_TRACE_12(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "<K32LOG SID='%X'>%X %X %X %X %X %X %X %X %X %X %X</K32LOG>",
                            sequenceID,
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+0])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+1])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+2])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+3])),
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+4])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+5])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+6])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+7])),
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+8])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+9])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+10])));
                        i+=11;
                        break;
                    case 12:/* 12 elements */
                        EDDP_K32_FW_TRACE_13(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "<K32LOG SID='%X'>%X %X %X %X %X %X %X %X %X %X %X %X</K32LOG>",
                            sequenceID,
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+0])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+1])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+2])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+3])),
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+4])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+5])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+6])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+7])),
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+8])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+9])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+10])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+11])));
                        i+=12;
                        break;
                    case 13:/* 13 elements */
                        EDDP_K32_FW_TRACE_14(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "<K32LOG SID='%X'>%X %X %X %X %X %X %X %X %X %X %X %X %X</K32LOG>",
                            sequenceID,
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+0])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+1])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+2])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+3])),
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+4])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+5])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+6])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+7])),
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+8])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+9])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+10])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+11])),
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+12])));
                        i+=13;
                        break;
                    case 14:/* 14 elements */
                        EDDP_K32_FW_TRACE_15(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "<K32LOG SID='%X'>%X %X %X %X %X %X %X %X %X %X %X %X %X %X</K32LOG>",
                            sequenceID,
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+0])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+1])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+2])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+3])),
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+4])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+5])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+6])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+7])),
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+8])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+9])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+10])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+11])),
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+12])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+13])));
                        i+=14;
                        break;
                    case 15:    /* SID + 15 elements */
                        EDDP_K32_FW_TRACE_16(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "<K32LOG SID='%X'>%X %X %X %X %X %X %X %X %X %X %X %X %X %X %X</K32LOG>",
                            sequenceID,
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+0])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+1])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+2])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+3])),
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+4])), EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+5])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+6])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+7])),
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+8])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+9])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+10])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+11])),
                            EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+12])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+13])),EDDP_HAL_MEM32_READ(pDDB,&(logBuffer->data[i+14])));
                        i+=15;
                        break;
                    default:
                        EDDP_K32_FW_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "THIS CODE SHOULD NEVER BEEN REACHED");
                        i = elements;   /* break for-loop */
                        break;
                    }
                }

                EDDP_K32_FW_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "<K32LOG_END SID='%X' ELM='%d' />",
                    sequenceID, elements);
            }
#endif
        }
        break;

    default:
        EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] *** EDDP_K32GetRQB(IF[%d]) Invalid opcode=%u",
            InterfaceID, EDDP_HAL_MEM8_READ(pDDB,&(pK32RQB->Hdr.Opcode)));
        break;
    }

    /* acknowledge K32-RQB
    */
    if(!EDDP_K32_RRQB_ACK(pDDB, pK32RQB))
    {
        EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_K32GetRQB(pK32RQB=0x%X pDDB=0x%X): FAILED TO ACK",
        pK32RQB, pDDB);
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }

    EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT:EDDP_K32GetRQB() Status=0x%X",
        Status);

    return retval;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_K32AckRQB                              +*/
/*+  Input/Output          :    K32_RQB_PTR_TYPE         pK32RQB            +*/
/*+  Result                :    none                                        +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pK32RQB    : Pointer to KRISC32 ack request block from FW         (in) +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+  Results    : LSA_TRUE if more RRQBs could be processed,                +*/
/*+               LSA_FALSE if a shoutdown is pending:                      +*/
/*+               RRQB queues shouldn't be accessed any more.               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function transfers a K32-ACKRQB to the user:         +*/
/*+               The K32-ACKRRQB is a K32-RQB send by EDDP and now         +*/
/*+               acknowledged by KRISC32.                                  +*/
/*+               As soon as possible the K32-ACKRQB is finalized,          +*/
/*+               so it can become again a send RRQB.                       +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_K32AckRQB(
    EDDP_LOCAL_DDB_PTR_TYPE  pDDB,
    K32_RQB_PTR_TYPE         pK32RQB)
{
    EDDP_LOCAL_HDB_PTR_TYPE pHDB = LSA_NULL;
    EDD_UPPER_RQB_PTR_TYPE  pRQB = LSA_NULL;
    EDD_UPPER_RQB_PTR_TYPE  pRQBNext = LSA_NULL;
    EDD_UPPER_KRISC32_RQB_PTR_TYPE  pK32UpperRQB = LSA_NULL;
    LSA_UINT16  Response = K32_RSP_OK;
    EDD_RSP     Status = EDD_STS_OK;
    LSA_BOOL    retval = LSA_TRUE;
    LSA_UINT8   Opcode = pK32RQB->Hdr.Opcode; //EDDP_HAL_MEM8_READ(pDDB,&(pK32RQB->Hdr.Opcode));

    LSA_USER_ID_TYPE UserId;

    /* Get Response and UserId from header
    */
    Response = pK32RQB->Hdr.Response; // EDDP_HAL_MEM16_READ(pDDB,&(pK32RQB->Hdr.Response));
    UserId.uvar32 = EDDP_HAL_MEM32_READ(pDDB,&(pK32RQB->Hdr.UserId.uvar32));

    if ((K32_OPC_GSY_SYNC == Opcode)
    ||  ((K32_OPC_GSY_DELAY_REQ == Opcode) && (K32_RSP_OK_ACTIVE != Response)))
    {
        /* Pointer to upper RQB is in UserId: get pHDB from handle
        */
        pRQB =  (EDD_UPPER_RQB_PTR_TYPE)UserId.void_ptr;
        if (!LSA_HOST_PTR_ARE_EQUAL( pRQB, LSA_NULL))
        {
            pHDB = pDDB->K32.pHDB;              //091009lrg001
//          Status = EDDP_GlbHandleGetHDB(EDD_RQB_GET_HANDLE(pRQB), &pHDB);
/*          if (EDD_STS_OK != Status)
            {
                EDDP_K32_TRACE_03(LSA_TRACE_LEVEL_ERROR,"*** EDDP_K32AckRQB() Error=0x%X getting pHDB from pRQB=0x%X Handle=%u",
                    Status, pRQB, EDD_RQB_GET_HANDLE(pRQB));
                pRQB = LSA_NULL;
            }
            else if (LSA_HOST_PTR_ARE_EQUAL(pHDB->pK32, LSA_NULL))  //1007lrg001
*/          if (LSA_HOST_PTR_ARE_EQUAL(pHDB, LSA_NULL))             //091009lrg001
            {
                EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] *** EDDP_K32AckRQB() pRQB=0x%X opcode=%u: K32 channel not open",
                    pRQB, Opcode);
                pRQB = LSA_NULL;
            }
            else
            {
                /* Remove LSA-RQB from queue get K32 response for Confirmation
                */
                if (EDDP_GlbRQBQueueRemoveFromQueue(&pHDB->pK32->CmdReqQueue, pRQB))
                {
//                  Response = EDDP_K32_L2H16(pK32RQB->Hdr.Response);
                    if (K32_RSP_OK == Response)
                    {
                        Status = EDD_STS_OK;
                    }
                    else
                    {
                        EDDP_K32_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] *** EDDP_K32AckRQB() pK32RQB=0x%X Opcode=%u Response=0x%x",
                            pK32RQB, Opcode, Response);
                        Status = EDD_STS_ERR_HW;
                    }
                }
                else
                {
                    EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] *** EDDP_K32AckRQB() pRQB=0x%X not in CmdReqQueue",
                        pRQB);
                    pRQB = LSA_NULL;
                }
            }
        }
        else
        {
            EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] *** EDDP_K32AckRQB() Opcode=%u: Upper RQB=LSA_NULL",
                Opcode);
        }
    }

    /* release K32-RQB
    */
    if(!EDDP_K32_RRQB_FINALIZE(pDDB, pK32RQB))
    {
        EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_K32GetRQB(pK32RQB=0x%X pDDB=0x%X): FAILED TO FINALIZE",
        pK32RQB, pDDB);
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }

    switch (Opcode)
    {
    case K32_OPC_START:

        /* Finish DeviceSetup depending on KRISK32-OS response
        */
        ///pDDB = (EDDP_LOCAL_DDB_PTR_TYPE)K32RQB.Hdr.UserId.void_ptr;
        EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] --- EDDP_K32AckRQB() Opcode=START: Response=0x%X",
            Response);
        if (K32_RSP_OK == Response)
        {
            Status = EDD_STS_OK;
        }
        else
        {
            Status = EDD_STS_ERR_HW;
        }

        EDDP_CoreCBFDeviceSetup(pDDB, Status);
        break;
    case K32_OPC_STOP:
        {
            LSA_UINT32          ramsize = 0;
            LSA_VOID_PTR_TYPE   mem = 0;
            retval = LSA_FALSE; /* shutdown is now pending; RRQB queues should not be accessed any more. */
            mem = EDDP_HALGetRAMSubAddress(pDDB,EDDP_HAL_RAMTYPE_CMD,EDDP_HAL_RAMSUB_CMD_KRISC,&ramsize);
            if(mem)
            {
                EDDP_HALMemset(pDDB,EDDP_HAL_RAMTYPE_CMD,EDDP_HAL_RAMSUB_CMD_KRISC,0,ramsize,0);
            }
            Status = EDD_STS_OK;
            EDDP_CoreCBFDeviceShutdown(pDDB, Status);
        }
        break;
    case K32_OPC_SETUP:
    case K32_OPC_GSY_TOPO_STATE:
        /* Nothing to do
        */
        Status = EDD_STS_OK;
        break;
    case K32_OPC_GSY_SYNC:
    case K32_OPC_GSY_DELAY_REQ:

        /* Look for pending upper request block
        */
        if (K32_OPC_GSY_SYNC == Opcode)
        {
            if (EDD_PTCP_SYNC_ID_CLOCK == EDDP_HAL_MEM8_READ(pDDB,&(pK32RQB->Params.SyncReq.SyncId)))
            {
                pK32UpperRQB = pDDB->K32.pK32ClockRQB;
                if (!LSA_HOST_PTR_ARE_EQUAL(pHDB, LSA_NULL))
                    pRQBNext = EDDP_GlbRQBQueueRemoveFromBegin(&pHDB->pK32->SyncReqQueue[0]);
            }
            else
            {
                pK32UpperRQB = pDDB->K32.pK32TimeRQB;
                if (!LSA_HOST_PTR_ARE_EQUAL(pHDB, LSA_NULL))
                    pRQBNext = EDDP_GlbRQBQueueRemoveFromBegin(&pHDB->pK32->SyncReqQueue[1]);
            }
            /* Mark upper K32 RQB not longer in use
            */
            pK32UpperRQB->Hdr.UserId.void_ptr = LSA_NULL;
        }
        else if (K32_RSP_OK_ACTIVE != Response)
        {
            LSA_UINT16 PortIdx = EDDP_HAL_MEM16_READ(pDDB,&(pK32RQB->Params.DelayReq.PortID)) - 1;
            pK32UpperRQB = pDDB->K32.pK32DelayRQB[PortIdx];
            if (!LSA_HOST_PTR_ARE_EQUAL(pHDB, LSA_NULL))
                pRQBNext = EDDP_GlbRQBQueueRemoveFromBegin(&pHDB->pK32->DelayReqQueue[PortIdx]);

            /* Mark upper K32 RQB not longer in use
            */
            pK32UpperRQB->Hdr.UserId.void_ptr = LSA_NULL;
        }

        if (!LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL) && !LSA_HOST_PTR_ARE_EQUAL(pHDB, LSA_NULL))
        {
            /* Send back upper RQB to GSY
            */
            EDDP_GlbRequestFinish(pHDB, pRQB, Status);
            if (!LSA_HOST_PTR_ARE_EQUAL(pRQBNext, LSA_NULL))
            {
                /* Process next upper command RQB
                */
                EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] --- EDDP_K32AckRQB() Sending pending pRQB=0x%X to KRISC32 Opcode=%u",
                    pRQBNext, Opcode);
                if (K32_OPC_GSY_SYNC == Opcode)
                    Status = EDDP_K32RequestSync(pRQBNext, pDDB, pHDB, pK32UpperRQB);
                else
                    Status = EDDP_K32RequestDelay(pRQBNext, pDDB, pHDB, pK32UpperRQB);
                if (Status != EDD_STS_OK)
                {
                    /* On error: send back next upper RQB to GSY
                    */
                    EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] *** EDDP_K32AckRQB() Status=0x%X sending pRQB=0x%X to KRISC32",
                        Status, pRQB);
                    EDDP_GlbRequestFinish(pHDB, pRQBNext, Status);
                }
            }
        }
        break;

    /* these opcodes should never appear in a k32-ackrrqb
    */
    case K32_OPC_GSY_DIAG:
    case K32_OPC_GSY_DELAY_IND:

    default:
        EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] *** EDDP_K32AckRQB() Invalid opcode=%u",
            Opcode);
        break;
    }

    EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT:EDDP_K32AckRQB() Status=0x%X", Status);
    return retval;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_K32PutRQB                              +*/
/*+  Input/Output          :    see below                                   +*/
/*+  Result                :    LSA_RESULT                                  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pK32RQB    : Pointer to KRISC32 request block from user           (in) +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function transfers a K32-RQB to FW on the KRISC32    +*/
/*+               The K32-RQB is send from EDDP to KRISC32.                 +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
extern EDD_RSP EDDP_LL_K32PutRQB(
	    EDD_UPPER_KRISC32_RQB_PTR_TYPE      pK32RQB,
	    EDDP_LOCAL_DDB_PTR_TYPE             pDDB
	    );

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32PutRQB(
    EDD_UPPER_KRISC32_RQB_PTR_TYPE      pK32RQB,
//  EDDP_LOCAL_DDB_PTR_TYPE             pDDB,
    EDDP_LOCAL_DDB_PTR_TYPE             pDDB
    )
{

    EDDP_K32_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] IN :EDDP_K32PutRQB(pK32RQB=0x%X pDDB=0x%X)",
        pK32RQB, pDDB);

    if (!LSA_HOST_PTR_ARE_EQUAL(pDDB, LSA_NULL))
    {
        pK32RQB->Hdr.InterfaceID = (LSA_UINT8) EDDP_IF_A;  // only EDDP_IF_A is supported anymore
    }
    pK32RQB->Hdr.Response = K32_RSP_OK;

#if 1
	return EDDP_LL_K32PutRQB(pK32RQB, pDDB);
#else

    EDD_RSP Status = EDD_STS_ERR_NOT_IMPL;



#ifndef EDDP_K32_LOOPBACK_TEST

    if(!EDDP_K32_RRQB_SEND(pDDB, pK32RQB))
    {
        EDDP_K32_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_K32PutRQB(pK32RQB=0x%X pDDB=0x%X): FAILED TO SEND",
        pK32RQB, pDDB);
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }
    Status = EDD_STS_OK;
#else
    {   // only for EDDP_K32_LOOPBACK_TEST
        K32_RQB_TYPE K32RBl = {0};

        if ((K32_OPC_GSY_SYNC == pK32RQB->Hdr.Opcode)
        &&  (/*GSY_SYNC_START!!!!*/ K32_OPC_GSY_SYNC == pK32RQB->Params.SyncReq.SlaveState))
        {
            /* Simulate first Diagnosis
            */
            K32RBl.Hdr.Opcode = K32_OPC_GSY_DIAG;
            K32RBl.Hdr.Response = K32_RSP_OK;
            K32RBl.Hdr.UserId.void_ptr = pDDB;
            K32RBl.Hdr.InterfaceID = pK32RQB->Hdr.InterfaceID;
            K32RBl.Params.DiagInd.DiagSrc = 7;
            K32RBl.Params.DiagInd.SlaveState = 1;
            K32RBl.Params.DiagInd.SyncId = pK32RQB->Params.SyncReq.SyncId;
            EDDP_K32GetRQB(pDDB, &K32RBl);
        }
        EDDP_K32CopyUpperRQB(pDDB, pK32RQB, &K32RBl);
        EDDP_K32GetRQB(pDDB, &K32RBl);
        if (K32_OPC_GSY_DELAY_REQ == pK32RQB->Hdr.Opcode)
        {
            /* Simulate delay indication
            */
            K32RBl.Hdr.Opcode = K32_OPC_GSY_DELAY_IND;
            K32RBl.Hdr.Response = K32_RSP_OK;
            K32RBl.Hdr.UserId.void_ptr = pDDB;
            K32RBl.Hdr.InterfaceID = pK32RQB->Hdr.InterfaceID;
            K32RBl.Params.DelayInd.PortID = pK32RQB->Params.DelayReq.PortID;
            if (/*GSY_SYNC_START!!!!!*/ K32_OPC_GSY_SYNC == pK32RQB->Params.DelayReq.ReqActive)
            {
                K32RBl.Params.DelayInd.LineDelay = pK32RQB->Params.DelayReq.RxPortDelay
                                                 + pK32RQB->Params.DelayReq.TxPortDelay;
                K32RBl.Params.DelayInd.CableDelay = 10;
            }
            else
            {
                K32RBl.Params.DelayInd.LineDelay = 0;
                K32RBl.Params.DelayInd.CableDelay = 0;
            }
            EDDP_K32GetRQB(pDDB, &K32RBl);
        }
        Status = EDD_STS_OK;
    }
#endif

    EDDP_K32_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT:EDDP_K32PutRQB() Status=0x%X", Status);

    return Status;
#endif
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_K32RQBAlloc                            +*/
/*+  Input/Output          :    EDD_UPPER_KRISC32_RQB_PTR_TYPE pK32RQB      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pK32RQB    : Pointer to KRISC32 request block                    (out) +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+  Results    : no RQB available if LSA_NULL                              +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function allocates a K32-RQB                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_UPPER_KRISC32_RQB_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_K32RQBAlloc (
    LSA_VOID)
{
    EDD_UPPER_KRISC32_RQB_PTR_TYPE pK32RQB;

    EDDP_K32_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] IN :EDDP_K32RQBAlloc()");

    pK32RQB = (EDD_UPPER_KRISC32_RQB_PTR_TYPE)EDDP_GlbAllocUpperMemLocal(sizeof(K32_RQB_TYPE));

    EDDP_K32_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT:EDDP_K32RQBAlloc() pK32RQB=0x%X", pK32RQB);

    return pK32RQB;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_K32RQBFree                             +*/
/*+  Input/Output          :    EDD_UPPER_KRISC32_RQB_PTR_TYPE pK32RQB      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pK32RQB    : Pointer to KRISC32 request block                     (in) +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_ERR_PARAM if not allocated                        +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function frees an allocated K32-RQB                  +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32RQBFree (
    EDD_UPPER_KRISC32_RQB_PTR_TYPE           pK32RQB)
{
    EDD_RSP Status;

    EDDP_K32_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] IN :EDDP_K32RQBFree(pK32RQB=0x%X)",
        pK32RQB);

    Status = EDDP_GlbFreeUpperMemLocal((EDD_UPPER_MEM_PTR_TYPE)pK32RQB);

    EDDP_K32_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT:EDDP_K32RQBFree() Status=0x%X",
        Status);

    return Status;
}

LSA_BOOL EDDP_K32_RRQB_SEND(EDDP_LOCAL_DDB_PTR_TYPE pDDB, K32_RQB_PTR_TYPE pRRQB)
{
    LSA_VOID_PTR_TYPE pKRISCReceiveQueue = (void*)&(((K32_QUEUE_PARTITION_IN_DATA_QUEUE_RAM*)
            EDDP_HALGetRAMSubAddress(pDDB,EDDP_HAL_RAMTYPE_CMD,EDDP_HAL_RAMSUB_CMD_KRISC,0))->krisc32);
    /*LSA_VOID_PTR_TYPE pKRISCReceiveQueue = (void*)&(
            ((K32_QUEUE_PARTITION_IN_DATA_QUEUE_RAM*)
                pDDB->Core.Hal.HalRamLayout.RamComponentCmd.ComponentRamArray[EDDP_HAL_RAMSUB_CMD_KRISC].pSubComponentRamBase)->krisc32);
    */
    return K32_HAL_RRQB_SEND(pDDB,pRRQB,(LSA_VOID_PTR_TYPE)pKRISCReceiveQueue);
}

LSA_BOOL EDDP_K32_RRQB_GET(EDDP_LOCAL_DDB_PTR_TYPE pDDB, K32_RQB_PTR_TYPE* ppRRQB)
{
    LSA_VOID_PTR_TYPE pEDDPReceiveQueue = (void*)&(((K32_QUEUE_PARTITION_IN_DATA_QUEUE_RAM*)
        EDDP_HALGetRAMSubAddress(pDDB,EDDP_HAL_RAMTYPE_CMD,EDDP_HAL_RAMSUB_CMD_KRISC,0))->eddp);
    /*LSA_VOID_PTR_TYPE pEDDPReceiveQueue = (void*)&(
            ((K32_QUEUE_PARTITION_IN_DATA_QUEUE_RAM*)
                pDDB->Core.Hal.HalRamLayout.RamComponentCmd.ComponentRamArray[EDDP_HAL_RAMSUB_CMD_KRISC].pSubComponentRamBase)->eddp);
    */
    return K32_HAL_RRQB_GET(pDDB,ppRRQB,(LSA_VOID_PTR_TYPE)pEDDPReceiveQueue);
}

LSA_BOOL EDDP_K32_RRQB_ACK(EDDP_LOCAL_DDB_PTR_TYPE pDDB, K32_RQB_PTR_TYPE pRRQB)
{
    LSA_VOID_PTR_TYPE pEDDPReceiveQueue = (void*)&(((K32_QUEUE_PARTITION_IN_DATA_QUEUE_RAM*)
        EDDP_HALGetRAMSubAddress(pDDB,EDDP_HAL_RAMTYPE_CMD,EDDP_HAL_RAMSUB_CMD_KRISC,0))->eddp);
    /*LSA_VOID_PTR_TYPE pEDDPReceiveQueue = (void*)&(
            ((K32_QUEUE_PARTITION_IN_DATA_QUEUE_RAM*)
                pDDB->Core.Hal.HalRamLayout.RamComponentCmd.ComponentRamArray[EDDP_HAL_RAMSUB_CMD_KRISC].pSubComponentRamBase)->eddp);
    */
    return (K32_HAL_RRQB_ACK(pDDB,pRRQB,(LSA_VOID_PTR_TYPE)pEDDPReceiveQueue) >= 0);
}

LSA_BOOL EDDP_K32_RRQB_GET_ACK(EDDP_LOCAL_DDB_PTR_TYPE pDDB, K32_RQB_PTR_TYPE* ppRRQB)
{
    LSA_VOID_PTR_TYPE pKRISCReceiveQueue = (void*)&(((K32_QUEUE_PARTITION_IN_DATA_QUEUE_RAM*)
            EDDP_HALGetRAMSubAddress(pDDB,EDDP_HAL_RAMTYPE_CMD,EDDP_HAL_RAMSUB_CMD_KRISC,0))->krisc32);
    /*LSA_VOID_PTR_TYPE pKRISCReceiveQueue = (void*)&(
            ((K32_QUEUE_PARTITION_IN_DATA_QUEUE_RAM*)
                pDDB->Core.Hal.HalRamLayout.RamComponentCmd.ComponentRamArray[EDDP_HAL_RAMSUB_CMD_KRISC].pSubComponentRamBase)->krisc32);
    */
    return K32_HAL_RRQB_GET_ACK(pDDB,ppRRQB,(LSA_VOID_PTR_TYPE)pKRISCReceiveQueue);
}

LSA_BOOL EDDP_K32_RRQB_FINALIZE(EDDP_LOCAL_DDB_PTR_TYPE pDDB, K32_RQB_PTR_TYPE pRRQB)
{
    LSA_VOID_PTR_TYPE pKRISCReceiveQueue = (void*)&(((K32_QUEUE_PARTITION_IN_DATA_QUEUE_RAM*)
        EDDP_HALGetRAMSubAddress(pDDB,EDDP_HAL_RAMTYPE_CMD,EDDP_HAL_RAMSUB_CMD_KRISC,0))->krisc32);
    /*LSA_VOID_PTR_TYPE pKRISCReceiveQueue = (void*)&(
            ((K32_QUEUE_PARTITION_IN_DATA_QUEUE_RAM*)
                pDDB->Core.Hal.HalRamLayout.RamComponentCmd.ComponentRamArray[EDDP_HAL_RAMSUB_CMD_KRISC].pSubComponentRamBase)->krisc32);
    */
    return (K32_HAL_RRQB_FINALIZE(pDDB,pRRQB,(LSA_VOID_PTR_TYPE)pKRISCReceiveQueue) >= 0);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CopyTimeStamp                          +*/
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

static EDD_RSP EDDP_LOCAL_FCT_ATTR  EDDP_CopyTimeStamp(volatile EDDP_TIMESTAMP_INFO_TYPE * const pDest, const EDDP_TIMESTAMP_INFO_TYPE * const pSrc)
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
/*+  Functionname          :    EDDP_CList_WriteTimeSyncInd                 +*/
/*+  Input/Output          :    see below                                   +*/
/*+  Result                :    see below                                   +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB            : Pointer to DDB                                  (in) +*/
/*+  pOldTimestamp   : Pointer to the old timestap                     (in) +*/
/*+  pNewTimestamp   : Pointer to the new timestap                     (in) +*/
/*+                                                                         +*/
/*+  Results         : EDD_RSP                                              +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This helper function writes two timestaps to the          +*/
/*+               Shared Memory in the Cyclic List                          +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CList_WriteTimeSyncInd (
    EDDP_LOCAL_DDB_PTR_TYPE           const pDDB,
    const EDDP_TIMESTAMP_INFO_TYPE  * const pOldTimestamp,
    const EDDP_TIMESTAMP_INFO_TYPE  * const pNewTimestamp)
{
    EDD_RSP Response = EDD_STS_OK;

    if (LSA_HOST_PTR_ARE_EQUAL(pDDB->pUserCmdSharedMem, LSA_NULL))
    {
        Response = EDD_STS_ERR_NOT_SUPPORTED;
        EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,  "[H:--] *** EDDP_CList_WriteTimeSyncInd(): pUserCmdSharedMem==NULL! pDDB=0x%X Response=0x%X", pDDB, Response);
    }
    else if (LSA_HOST_PTR_ARE_EQUAL(pOldTimestamp, LSA_NULL))
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] *** EDDP_CList_WriteTimeSyncInd(): pOldTimestamp==NULL! pDDB=0x%X Response=0x%X", pDDB, Response);
    }
    else if (LSA_HOST_PTR_ARE_EQUAL(pNewTimestamp, LSA_NULL))
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] *** EDDP_CList_WriteTimeSyncInd(): pNewTimestamp==NULL! pDDB=0x%X Response=0x%X", pDDB, Response);
    }
    else
    {
        LSA_UINT32 NewWriteIndex = pDDB->pUserCmdSharedMem->CircListDescr.SrcWriteIndex + 1;
        if (NewWriteIndex >= pDDB->pUserCmdSharedMem->CircListDescr.ListSize)
        {
            NewWriteIndex = 0;
        }

        if (NewWriteIndex == pDDB->pUserCmdSharedMem->CircListDescr.DstReadIndex)
        {
            Response = EDD_STS_ERR_LIMIT_REACHED;
            EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] *** EDDP_CList_WriteTimeSyncInd(): List on Shared Memory is full! ReadIndex=%d ListSize=%d", 
                pDDB->pUserCmdSharedMem->CircListDescr.DstReadIndex, 
                pDDB->pUserCmdSharedMem->CircListDescr.ListSize);
        }
        else
        {
            volatile EDDP_TIMESTAMP_INFO_PTR_TYPE pSMOldTimestamp = &pDDB->pUserCmdSharedMem->TimestampList[pDDB->pUserCmdSharedMem->CircListDescr.SrcWriteIndex].OldTimestamp;
            volatile EDDP_TIMESTAMP_INFO_PTR_TYPE pSMNewTimestamp = &pDDB->pUserCmdSharedMem->TimestampList[pDDB->pUserCmdSharedMem->CircListDescr.SrcWriteIndex].NewTimestamp;

            Response = EDDP_CopyTimeStamp(pSMOldTimestamp, pOldTimestamp);
            if (EDD_STS_OK != Response)
            {
                EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] *** EDDP_CList_WriteTimeSyncInd(): Copy Old TimeStamp failed! pDDB=0x%X Response=0x%X", pDDB, Response);
            }
            else
            {
                Response = EDDP_CopyTimeStamp(pSMNewTimestamp, pNewTimestamp);
                if (EDD_STS_OK != Response)
                {
                    EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] *** EDDP_CList_WriteTimeSyncInd(): Copy New TimeStamp failed! pDDB=0x%X Response=0x%X", pDDB, Response);
                }
                else
                {
                    pDDB->pUserCmdSharedMem->CircListDescr.SrcWriteIndex = NewWriteIndex;
                }
            }
        }
    }

    return(Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_UpdateTimeDomainIdent                  +*/
/*+  Input/Output          :    see below                                   +*/
/*+  Result                :    see below                                   +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB            : Pointer to DDB                                  (in) +*/
/*+  pTimeDomainIdent: Pointer to the TimeDomainIdent                  (in) +*/
/*+                                                                         +*/
/*+  Results         : EDD_RSP                                              +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This helper function updates the time domain ident        +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_UpdateTimeDomainIdent (
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT8                 * pTimeDomainIdent,
    LSA_UINT8             const LengthTimeDomainIdent)
{
    EDD_RSP Response = EDD_STS_OK;

    // Clear old DomainIdent completely, bytes behind LengthTimeDomainIdent will be zero
    EDDP_MEMSET_COMMON(pDDB->K32.TimestampInfo.DomainIdent, 0, EDDP_TIME_DOMAIN_IDENT_SIZE);

    if (LSA_HOST_PTR_ARE_EQUAL(pTimeDomainIdent, LSA_NULL))
    {
        Response = EDD_STS_OK_NO_DATA;
        EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--]     EDDP_UpdateTimeDomainIdent(): pTimeDomainIdent==NULL! pDDB=0x%X Response=0x%X", pDDB, Response);
    }
    else if (!LengthTimeDomainIdent)
    {
        Response = EDD_STS_OK_NO_DATA;
        EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--]     EDDP_UpdateTimeDomainIdent(): LengthTimeDomainIdent==0! pDDB=0x%X Response=0x%X", pDDB, Response);
    }
    else
    {
        EDDP_MEMCOPY_COMMON( pDDB->K32.TimestampInfo.DomainIdent, pTimeDomainIdent, LSA_MIN(LengthTimeDomainIdent, EDDP_TIME_DOMAIN_IDENT_SIZE));

        if (LSA_HOST_PTR_ARE_EQUAL(pDDB->pUserCmdSharedMem, LSA_NULL))
        {
            Response = EDD_STS_ERR_NOT_SUPPORTED;
            EDDP_K32_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,  "[H:--] *** EDDP_UpdateTimeDomainIdent(): pUserCmdSharedMem==NULL! pDDB=0x%X Response=0x%X", pDDB, Response);
        }
        else
        {
            EDDP_MEMCOPY_COMMON(pDDB->pUserCmdSharedMem->TimestampInfoBackup.DomainIdent, pDDB->K32.TimestampInfo.DomainIdent, EDDP_TIME_DOMAIN_IDENT_SIZE);
        }
    }

    return(Response);
}


