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
/*  C o m p o n e n t     &C: PnIODDevkits                              :C&  */
/*                                                                           */
/*  F i l e               &F: pnpb_peri.c                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*                                                                           */
/*  Date        Version        What                                          */
/*  2015-05-05  patch01.2      if ownership is rejected, set flag to PNDV    */
/*  2015-05-12  patch01.5      lock disconnect against io data access        */
/*  2015-05-19  patch01.6      change tracelog-level for diagnosis entry, if */
/*                             no AR is established                          */
/*---------------------------------------------------------------------------*/

#include "compiler.h"
#include "pniousrd.h"
#include "trc_if.h"
#include "pndv_inc.h"

#include "pnpb_cfg.h"
#include "pnpb_com.h"
#include "pnpb_sub_exp.h"
#include "pnpb_sub_real.h"
#include "pnpb_trc.h"
#include "pnpb_peri.h"
#include "pnpb.h"
#include "pnpb_api.h"
#include "pnpb_rec.h"

#include "os_taskprio.h"
#include "iodapi_event.h"

// *-----------------------------------------------------------------
// * 2do
// *-----------------------------------------------------------------
#define SIMULATE_PCMD_RESPONSE      0    // no response of pCmd from PNDV --> simulate it

#define LTRC_ACT_MODUL_ID   208
#define PNPB_MODULE_ID      208


// *-----------------------------------------------------------------
// * external variables
// *-----------------------------------------------------------------
extern PNDV_IFACE_STRUCT_PTR    pPnpbIf;
extern PNPB_INSTANCE            Pnpb;
extern PNIO_UINT32 volatile     ProcessAlarmProcessing;
extern PNIO_UINT32 volatile     DiagAlarmProcessing;
extern PNIO_UINT32 volatile     RosAlarmProcessing;
extern PNIO_UINT32 volatile     UplRetrAlarmProcessing;
extern PNIO_UINT32 volatile     StatusAlarmProcessing;

// *-----------------------------------------------------------------
// * static variables
// *-----------------------------------------------------------------
static PNIO_UINT32  PerifTag[PNIO_ALM_NUMOF_ENTRIES][PERIF_HND_NUMOF_ELEMS_PER_ENTRY];
static PNIO_UINT32  TskId_PnpbPost = 0;


typedef struct
{
    PNIO_UINT32             NumOfEntries;
    PNDV_IFACE_CMD_ENTRY_T  Entry[PNDV_IFACE_CMD_CNT];
    PNIO_EXP_SUB*           pSubExp[PNDV_IFACE_CMD_CNT];
} WR_EVENT_FIFO;

static WR_EVENT_FIFO  WrEventFifo [IOD_CFG_NUMOF_AR];

#if SIMULATE_PCMD_RESPONSE
    static struct
    {
        void*       pCmdProp;
        PNIO_UINT32 SemId;
        PNIO_UINT8  ExpectedCmd;
    }  PnpbGlobReadEvent;

#endif

// *-----------------------------------------------------------------
// * external functions
// *-----------------------------------------------------------------

// *=================================================================
// * public functions
// *=================================================================
void pnpb_peri_init(void)
{
    int i;
    OsMemSet (&WrEventFifo[0], 0, sizeof (WrEventFifo));
    PnpbRealCfgInit();

    for (i = 0; i < SEM_PNPB_MAXNUM; i++)
    {
        OsAllocSemB (&Pnpb.SemId[i]);
    }

    PullPending.WritePending = PNIO_FALSE;
    PullPending.ReadPending = PNIO_FALSE;
}

PNIO_UINT32 ReadPerifTag (PNIO_ALARM_TYPE PerifCmd, PNIO_UINT32 ValIndex)
{
    if ((PerifCmd >= PNIO_ALM_NUMOF_ENTRIES) || (ValIndex >= PERIF_HND_NUMOF_ELEMS_PER_ENTRY))
    {
        PNPB_SYS_TRACE_02(LSA_TRACE_LEVEL_FATAL,  "ReadPerifTag (Cmd%d ValInd%d%d)\n",  PerifCmd, ValIndex);
        return(0);
    }
    else
    {
    	return (PerifTag [PerifCmd][ValIndex]);
    }
}


PNIO_VOID StorePerifTag (PNIO_ALARM_TYPE PerifCmd, PNIO_UINT32 ValIndex, PNIO_UINT32 Val)
{
    if ((PerifCmd >= PNIO_ALM_NUMOF_ENTRIES) ||(ValIndex >= PERIF_HND_NUMOF_ELEMS_PER_ENTRY))
    {
        PNPB_SYS_TRACE_02(LSA_TRACE_LEVEL_FATAL,  "StorePerifTag (Cmd%d ValInd%d%d)\n",  PerifCmd, ValIndex);
    }
    else
    {
    	PerifTag [PerifCmd][ValIndex] = Val;
    }
}


PNDV_IFACE_CMD_ENTRY_T  pnpb_read_event_from_pndv   (PNIO_UINT32* PerifCmdInd)
{
    PNDV_IFACE_CMD_ENTRY_T ret_event;

    if ( pPnpbIf->cmd.read_ptr_pndv_to_peri != pPnpbIf->cmd.write_ptr_pndv_to_peri )
    {
        OsEnterX (OS_MUTEX_PNPB_RD_PERI);
        /* ring buffer has one ore more new entries */
        *PerifCmdInd = pPnpbIf->cmd.read_ptr_pndv_to_peri;
        ret_event = pPnpbIf->cmd.pndv_to_peri[pPnpbIf->cmd.read_ptr_pndv_to_peri];

        if(++pPnpbIf->cmd.read_ptr_pndv_to_peri == PNDV_IFACE_CMD_CNT)
        {
            pPnpbIf->cmd.read_ptr_pndv_to_peri = 0;
        }
        OsExitX (OS_MUTEX_PNPB_RD_PERI);
	    PNPB_SYS_TRACE_04(LSA_TRACE_LEVEL_NOTE,  "peri cmd=%d par1=0x%x par2=0x%x par3=0x%x",
                          PNPB_TRACE_READ_EVENT, ret_event.cmd, ret_event.add_1, ret_event.add_2);
    }
    else
    {
        ret_event.cmd = PNDV_EV_NO;
    }

    return(ret_event);
}


/*****************************************************************************/
PNPB_CODE_FAST PNIO_VOID  pnpb_write_event_to_pndv
                        (PNIO_UINT8  cmd,
                         PNIO_UINT8  add_1,
                         PNIO_UINT16 add_2,
                         PNIO_VOID* pCmdProp)
{

    PNIO_UINT32 CmdIndex;
    PNDV_IFACE_STRUCT_PTR  pndv_iface_ptr = pPnpbIf;

    LSA_UNUSED_ARG (pCmdProp);

    if ( pndv_iface_ptr->cmd.read_ptr_peri_to_pndv == pndv_iface_ptr->cmd.write_ptr_peri_to_pndv + 1 )
    {
        /* ueberlauf -> fatal */
        PNPB_PERIF_TRACE_00(LSA_TRACE_LEVEL_FATAL,  "peri overflow\n");
    }

    OsEnterX (OS_MUTEX_PNPB_WR_PERI);
    CmdIndex = pndv_iface_ptr->cmd.write_ptr_peri_to_pndv;

    pndv_iface_ptr->cmd.peri_to_pndv [CmdIndex].cmd   = cmd;
    pndv_iface_ptr->cmd.peri_to_pndv [CmdIndex].add_1 = add_1;
    pndv_iface_ptr->cmd.peri_to_pndv [CmdIndex].add_2 = add_2;
    //pndv_iface_ptr->cmd.peri_to_pndv [CmdIndex].pCmdProp = pCmdProp;

    if(++pndv_iface_ptr->cmd.write_ptr_peri_to_pndv == PNDV_IFACE_CMD_CNT)
    {
        pndv_iface_ptr->cmd.write_ptr_peri_to_pndv = 0;
    }
    PNPB_PERIF_TRACE_04(LSA_TRACE_LEVEL_NOTE, "peri cmd=%d par1=0x%x par2=0x%x par3=0x%x", PNPB_TRACE_WRITE_EVENT, cmd, add_1, add_2);
    OsExitX (OS_MUTEX_PNPB_WR_PERI);
}



static PNIO_VOID  pnpb_write_event_to_fifo
                        (PNIO_UINT32     ar_idx,
                         PNIO_UINT8      cmd,
                         PNIO_UINT8      add_1,
                         PNIO_UINT16     add_2,
                         PNIO_EXP_SUB*   pSubExp )
{
    if (WrEventFifo[ar_idx].NumOfEntries >= PNDV_IFACE_CMD_CNT)
    {
        PNPB_PERIF_TRACE_01(LSA_TRACE_LEVEL_FATAL, "Error pnpb_write_event_to_fifo(E=%d)\n", WrEventFifo[ar_idx].NumOfEntries);
    }

    WrEventFifo[ar_idx].Entry[WrEventFifo[ar_idx].NumOfEntries].cmd        = cmd;
    WrEventFifo[ar_idx].Entry[WrEventFifo[ar_idx].NumOfEntries].add_1      = add_1;
    WrEventFifo[ar_idx].Entry[WrEventFifo[ar_idx].NumOfEntries].add_2      = add_2;
    WrEventFifo[ar_idx].pSubExp[WrEventFifo[ar_idx].NumOfEntries]          = pSubExp;

    WrEventFifo[ar_idx].NumOfEntries++;
}


PNIO_VOID PnpbReqSync
                (PNIO_UINT8  cmd,
                 PNIO_UINT8  add_1,
                 PNIO_UINT16 add_2,
                 PNIO_UINT32 SemId,
                 PNIO_CBF_CMD_CONF  pCbfUsr)    // not used at the moment (PNDV can't handle yet)
{
    LSA_UNUSED_ARG (pCbfUsr);

    // *---------------------------------------------
    // *  write command to perif and trigger PNDV
    // *---------------------------------------------
    pnpb_write_event_to_pndv (cmd, add_1, add_2, NULL);
    PNPB_TRIGGER_PNDV ();


    // *---------------------------------------------
    // *  wait on semaphore and free afterwards..
    // *---------------------------------------------

    OsTakeSemB (SemId);
}

static void pnpb_perform_disconnect_ind (PNDV_IFACE_CMD_ENTRY_T  *pReadEvent)
{
	OsEnterX (OS_MUTEX_PNPB_IO);
#ifdef PNPB_SYSTEM_REDUNDANCY
	/*detect if S1 or S2*/
	if( PNIO_AR_TYPE_SINGLE_SYSRED == pnpb_ArType[ pReadEvent->add_1 ] )
	{
	/*if S2, call new func*/
		if(1 == pnpb_io_s2_ar_release(pReadEvent->add_1))
		{
			/*this if means that there are no more connections (both ARs exited)*/
			/*no special action needed here*/
		}
	}

#endif /*PNPB_SYSTEM_REDUNDANCY*/

    PnpbExp[pReadEvent->add_1].ArValid            = PNIO_FALSE;
    PnpbExp[pReadEvent->add_1].Rdy4InpUpdateValid = PNIO_FALSE;
    PnpbExp[pReadEvent->add_1].FirstParamEndValid = PNIO_FALSE;

    // **** set AR state in PNPB state machine ****
    Pnpb.ArState[pReadEvent->add_1] = PNPB_AR_OFFLINE;

    if (pReadEvent->add_2 < PNIO_AR_REASON_MAX)
    {
        PNIO_AR_REASON DisconnectReason = (PNIO_AR_REASON)pReadEvent->add_2;
        PNIO_cbf_ar_disconn_ind (PNIO_SINGLE_DEVICE_HNDL,                        // device handle
                                 pReadEvent->add_1 + 1,                          // Ar-Handle
                                 pPnpbIf->ar[pReadEvent->add_1].ar_session_key,  // session key
                                 DisconnectReason);                              // reason code
    }
    else
    {
        PNPB_SYS_TRACE_01(LSA_TRACE_LEVEL_FATAL, "Invalid disconnect reason (0x%0X)", pReadEvent->add_2);
    }

    OsExitX  (OS_MUTEX_PNPB_IO);

    pnpb_write_event_to_pndv(PNDV_EV_TO_PNDV_AR_DISCONNECT_IND_DONE,
                             pReadEvent->add_1,
                             pReadEvent->add_2,
                             NULL);
    PNPB_TRIGGER_PNDV ();


}

static void pnpb_perform_ownership_ind (PNDV_IFACE_CMD_ENTRY_T  *pReadEvent)
{  // *** first confirm all queued element to PNDV ***
    PNIO_UINT32             i;
    PNIO_EXP_SUB*           pSubExp = 0;
    PNDV_SET_CFG_ELEMENT_T* pElem;
    PNIO_UINT32             ar_idx = pReadEvent->add_1;

    // **** access current ownership element in PERIF interface ***
    pElem = &(pPnpbIf->set_cfg[pReadEvent->add_1][pReadEvent->add_2].elem);


    // ***** add current element to expected config. in PNPB  *****
    #if 1  // 2015-05-05 patch01.2
        {
            PNIO_UINT32             Flags;
            Flags = pPnpbIf->set_cfg[ar_idx][pReadEvent->add_2].flags;
            pnpb_sub_exp_add (ar_idx,               // AR index
                              pReadEvent->add_2,    // entity_idx in set_cfg
                              pElem,                // ownership data
                              Flags,                // flags, including ownership-flag
                              &pSubExp,             // [out]
                              PNIO_FALSE);          // more follows = FALSE
        }
    #else
        pnpb_sub_exp_add (ar_idx,               // AR index
                          pReadEvent->add_2,    // entity_idx in set_cfg
                          pElem,                // ownership data
                          &pSubExp,             // [out]
                          PNIO_FALSE);          // more follows = FALSE
    #endif

    // ***** store confirmation to PNDV temporarily in fifo *****
    pnpb_write_event_to_fifo   (ar_idx,
                                PNDV_EV_TO_PNDV_OWN_IND_DONE,
                                (PNIO_UINT8) ar_idx,
                                pReadEvent->add_2,
                                pSubExp);      // save ptr to exp-config-element



    // **** access current ownership element in PERIF interface ***
    pElem = &(pPnpbIf->set_cfg[pReadEvent->add_1][pReadEvent->add_2].elem);



    for (i = 0; i < WrEventFifo[ar_idx].NumOfEntries; i++)
    {

        // **** access appropriate ownership elements in PERIF and exp. config. ***
        pSubExp = WrEventFifo[ar_idx].pSubExp[i];
        pElem = &(pPnpbIf->set_cfg[WrEventFifo[ar_idx].Entry[i].add_1][WrEventFifo[ar_idx].Entry[i].add_2].elem);

        // *** first adjust owner and plug state **
        if (pSubExp->IsWrongSubmod)
        { // *** submodule has error ***
            pElem->cmp_result = PNDV_CMP_RES_ERROR;
        }
        else
        { // *** submodule OK ***
            pElem->cmp_result = PNDV_CMP_RES_OK;
        }

        #if 1   // 2015-05-05  patch01.2
            if (pSubExp->OwnSessionKey == 0)
            {  // *** AR is not owner of the submodule ***
                pPnpbIf->set_cfg[ar_idx][pSubExp->EntityIndSetCfg].flags &= ~PNDV_SET_CFG_FLAGS_OWN; // clear bit 0 in flags  (is not owner...)
            }
        #else
            if (pSubExp->OwnSessionKey == 0)
            {  // *** AR is not owner of the submodule ***
                pElem->own_info.owner_session_key = pSubExp->OwnSessionKey;
            }
            else
            {  // *** AR is owner of the submodule ***
                pPnpbIf->set_cfg[ar_idx][pReadEvent->add_2].flags |= PNDV_SET_CFG_FLAGS_OWN; // set bit 0 in flags  (is not owner...)
            }
        #endif

        // ** send confirmation to PNDV **
        pnpb_write_event_to_pndv
                 (WrEventFifo[ar_idx].Entry[i].cmd,
                  WrEventFifo[ar_idx].Entry[i].add_1,
                  WrEventFifo[ar_idx].Entry[i].add_2,
                  0);
    }

    // *** clear fifo  (is empty now..)  ***
    WrEventFifo[ar_idx].NumOfEntries = 0;   // queue is empty now

	/* Problem for CiR here: This function is called also for plug and such a thing iwll disturb ArState */
	/* Of current AR. Thus we have to distinguish between New AR establishment and Plug event */
	if ((PNPB_AR_OWNERSHIP_FINISHED < Pnpb.ArState[ar_idx]) || (Pnpb.ArPending == ar_idx))
	{
		Pnpb.ArPending = 0xff;	/* NO AR */
		// **** set AR state in PNPB state machine ****
		Pnpb.ArState[ar_idx] = PNPB_AR_OWNERSHIP_FINISHED;
	}	/* Nothing to do in else */

    PNPB_TRIGGER_PNDV ();
}

void    pnpb_perform_services  (void)
{
     if ( pPnpbIf->cmd.read_ptr_pndv_to_peri != pPnpbIf->cmd.write_ptr_pndv_to_peri )
     {
        if (Pnpb.SemEventAvailable == 0)
        {
            Pnpb.SemEventAvailable++;
            OsGiveSemB (Pnpb.SemId[SEM_PNPB_PERIF]);
        }
     }
}


// *--------------------------------------------------------
// *  Task_PnpbPost()
// *
// *  callback handler for post task, to handle synchronous
// *  event endications from pndv to pnpb
// *--------------------------------------------------------
PNPB_CODE_FAST static PNIO_INT32  Task_PnpbPost (void)
{
  PNIO_VOID* pData = LSA_NULL;            // pointer to Requestblock
  PNIO_UINT32 taskID;

  // Wait until own TaskID has been saved by the father process
  OsWaitOnEnable(); // must be first call in every task
  taskID = OsGetThreadId();


  // *-----------------------------------------------------------
  // * loop forever
  // *-----------------------------------------------------------
  while (1)
  {
    OsReadMessageBlocked ((void**)&pData, taskID);
    if (pData == 0)
    {
        PNIO_TrcPrintf ("Error in Task_Pnpb_Post()\n");
    }
    else
    {
        PNDV_IFACE_CMD_ENTRY_T* pReadEvent = (PNDV_IFACE_CMD_ENTRY_T*)pData;
        switch (pReadEvent->cmd)
        {
            case PNDV_EV_TO_PERI_OWN_IND:
                 pnpb_perform_ownership_ind (pReadEvent);
                 break;
            case PNDV_EV_TO_PERI_AR_DISCONNECT_IND:
                 pnpb_perform_disconnect_ind (pReadEvent);
                 break;
            default:
                 PNPB_SYS_TRACE_01(LSA_TRACE_LEVEL_FATAL,  "wrong CMD (%d)\n", pReadEvent->cmd);
                 break;
        }
        OsFree (pData);
    }
  }
  return (0);   /*lint !e527 unreachable code */
}


void pnbp_StartTaskPost(void)
{
    PNIO_UINT32 Status;

    // *-----------------------------------------------------------
    // * create and start task PNPB-ownership handler
    // *-----------------------------------------------------------
    // *** create and start Task for EDD high priority context, add message queue to task ***
    Status = OsCreateThread ((void(*)(void))Task_PnpbPost, (PNIO_UINT8*)"Pnio_PnpbPost",  TASK_PRIO_PNPB_OWN_IND, &TskId_PnpbPost);
    PNIO_APP_ASSERT(Status == PNIO_OK, "ERROR: OsCreateThread\n");
    Status = OsCreateMsgQueue (TskId_PnpbPost);  // install the task message queue
    PNIO_APP_ASSERT(Status == PNIO_OK, "ERROR: OsCreateMsgQueue\n");
    Status = OsStartThread (TskId_PnpbPost);
    PNIO_APP_ASSERT(Status == PNIO_OK, "ERROR: OsStartThread\n");
}

static void pnpb_async_req_done (PNIO_UINT16 Entity, PNIO_ALARM_TYPE AlarmType)
{
    PNIO_DEV_ADDR Addr;     // location (module/submodule)
    PNIO_UINT32   Api;
    PNIO_UINT32   Status;
    PNIO_EXP_SUB* pSubExp;
    PNIO_UINT32   ar_idx;
    PNIO_UINT32  User_tag;

    if (AlarmType == PNIO_ALM_GEN_DIAG)
    { // generic diag alarm: Entity points to generic diag data in PERIF
        // **** find api/slot/subslot in generic diag data of PERIF
        Api              = pPnpbIf->generic_diag_data[Entity].api;
        Addr.Geo.Slot    = pPnpbIf->generic_diag_data[Entity].slot;
        Addr.Geo.Subslot = pPnpbIf->generic_diag_data[Entity].subslot;
        User_tag		 = pPnpbIf->generic_diag_data[Entity].diag_tag;
        Status           = PNIO_OK;
    }
    else
    { // all other alarms: Entity points to real config in PERIF
        // **** find api/slot/subslot in real configuration
        Status = PnpbRealCfgGetApiSlotSubslot  (Entity, &Api, &Addr.Geo.Slot, &Addr.Geo.Subslot);

        if ( PNIO_ALM_CHAN_DIAG == AlarmType )
        {
        	User_tag		 = pPnpbIf->real_cfg[Entity].dial.chn_diag[0].diag_tag;
        }
        else if ( PNIO_ALM_EXT_CHAN_DIAG == AlarmType )
        {
        	User_tag		 = pPnpbIf->real_cfg[Entity].xdial.ext_diag[0].diag_tag;
        }
        else if ( PNIO_ALM_PROC == AlarmType )
        {
        	User_tag		 = pPnpbIf->real_cfg[Entity].pral.diag_tag;
        }
        else
        {
            User_tag         = 0;
        }
    }

    // *** read AR number from expected configuration ****
    pSubExp = pnpb_sub_exp_allAr_getp (&ar_idx, Api, Addr.Geo.Slot, Addr.Geo.Subslot);
    if (pSubExp == NULL)
    {
        #if 1  // 2015-05-19  patch01.6
            PNPB_SYS_TRACE_02(LSA_TRACE_LEVEL_NOTE, "no AR for sl=%d ss=%d\n",
                              Addr.Geo.Slot, Addr.Geo.Subslot);
        #else
            PNPB_SYS_TRACE_02(LSA_TRACE_LEVEL_ERROR,  "no AR for sl=%d ss=%d\n",
                              Addr.Geo.Slot, Addr.Geo.Subslot);
        #endif
        return;
    }
    PNIO_cbf_async_req_done (PNIO_SINGLE_DEVICE_HNDL, ar_idx + 1, AlarmType, Api, &Addr, Status, User_tag);
}

static void pnpb_device_stop_done ( void )
{
    PNIO_printf ("device is stopped...\n");

    PnpbRealCfgInit();

    return;
}

void    pnpb_process_service_requests  (void)
{
    PNDV_IFACE_STRUCT_PTR  pndv_iface_ptr = pPnpbIf;

    PNDV_IFACE_CMD_ENTRY_T read_event;
    PNIO_UINT32            PeriCmdIndex;

    for( read_event = pnpb_read_event_from_pndv(&PeriCmdIndex); read_event.cmd != PNDV_EV_NO; read_event = pnpb_read_event_from_pndv(&PeriCmdIndex) ) /*lint !e441*/
    {
       PNPB_API_TRACE_04(LSA_TRACE_LEVEL_NOTE_LOW,
                          "API: subPlugRspMore(cmd%d Aa%d Ab%d tag0x%x\n",
                          read_event.cmd,
                          read_event.add_1,
                          read_event.add_2,
                          0);

        switch(read_event.cmd)
        {
            /* ----------------------------------------------------- */
            case PNDV_EV_NO:
            {
                break;
            }

            /* ----------------------------------------------------- */
            case PNDV_EV_TO_PERI_PNDV_START_DONE:
            {
                // **** call user function *****
                Pnpb.State = PNPB_PNDV_STARTED;
#ifdef PNPB_SYSTEM_REDUNDANCY
                Unsigned8   ret_val = LSA_RET_OK;
                int i;

                for(i = 0; i < PNPB_MAX_S2_AR_SETS; i++)
                {
                	LSAS_ALLOC_TIMER( &ret_val, &pnpb_data.timer[i].ident, LSA_TIMER_TYPE_ONE_SHOT,
                                     LSA_TIME_BASE_1MS, pnpb_trigger_rdht_timeout, pnpb_data.init.component_id );

                    if( LSA_RET_OK != ret_val)
                    {
                    	PnpbSetLastError (PNPB_ERR_TIMER_BAD_RETURN_VAL);
                    	PNPB_SYS_TRACE_01(LSA_TRACE_LEVEL_ERROR, "bad return value from LSAS_STOP_TIMER", ret_val);
                    }
                }
#endif
                (*Pnpb.OpenDoneCbf)();
                break;
            }
            case PNDV_EV_TO_PERI_PNDV_STOP_DONE:
            {
                Pnpb.State = PNPB_STOP;

                pnpb_device_stop_done();
                break;
            }
            /* ----------------------------------------------------- */
            case PNDV_EV_TO_PERI_CM_DV_AKT_IND:
            {
                // ***** send confirmation to PNPB *****
                Pnpb.State = PNPB_RDY_FOR_PLUG;
                pnpb_write_event_to_pndv(PNDV_EV_TO_PNDV_CM_DV_AKT_IND_DONE, 0, 0, 0);

                // **** call user function *****

                break;
            }
            case PNDV_EV_TO_PERI_CM_DV_DEAKT_IND:
            {
                PNIO_printf ("PN device deactivated\n");
                pnpb_write_event_to_pndv(PNDV_EV_TO_PNDV_CM_DV_DEAKT_IND_DONE, 0, 0, 0);
                break;
            }
            case PNDV_EV_TO_PERI_PLUG_AL_QUIT:
            {
                Pnpb.State = PNPB_PDEV_STARTED;
                if (pPnpbIf->real_cfg[read_event.add_2].elem.state == PNDV_IFACE_SERVICE_PROCCESSING)
                {
                    pPnpbIf->real_cfg[read_event.add_2].elem.state = PNDV_IFACE_SERVICE_IDLE;

                    // **** call user function *****
                    OsGiveSemB(Pnpb.SemId[SEM_PNPB_PLUG]);
                }
                else
                {
                    PNPB_SYS_TRACE_01(LSA_TRACE_LEVEL_ERROR,
                                      "error state (%d) PULL_AL) ",
                                      pPnpbIf->real_cfg[read_event.add_2].elem.state);
                }
                break;
            }
            case PNDV_EV_TO_PERI_PLUG_AL_MORE_FOLLOWS_QUIT:
            {
                // **** call user function *****
                if (pPnpbIf->real_cfg[read_event.add_2].elem.state == PNDV_IFACE_SERVICE_PROCCESSING)
                {
                    pPnpbIf->real_cfg[read_event.add_2].elem.state = PNDV_IFACE_SERVICE_IDLE;
                }
                else
                {
                    PNPB_SYS_TRACE_01(LSA_TRACE_LEVEL_ERROR,
                                      "error state (%d) PULL_AL_MORE_FOLLOWS) ",
                                      pPnpbIf->real_cfg[read_event.add_2].elem.state);
                }
                break;
            }

            case PNDV_EV_TO_PERI_PULL_AL_QUIT:
                if (pPnpbIf->real_cfg[read_event.add_2].elem.state == PNDV_IFACE_SERVICE_PROCCESSING)
                    pPnpbIf->real_cfg[read_event.add_2].elem.state = PNDV_IFACE_SERVICE_IDLE;
                else
                    PNPB_SYS_TRACE_01(LSA_TRACE_LEVEL_ERROR,
                                      "error state (%d) PULL_AL_QUIT) ",
                                      pPnpbIf->real_cfg[read_event.add_2].elem.state);

                PnpbRealCfgFree  (read_event.add_2);
                OsGiveSemB(Pnpb.SemId[SEM_PNPB_PLUG]);
                break;

            case PNDV_EV_TO_PERI_PULL_AL_MORE_FOLLOWS_QUIT:
            {
                if (pPnpbIf->real_cfg[read_event.add_2].elem.state == PNDV_IFACE_SERVICE_PROCCESSING)
                    pPnpbIf->real_cfg[read_event.add_2].elem.state = PNDV_IFACE_SERVICE_IDLE;
                else
                    PNPB_SYS_TRACE_01(LSA_TRACE_LEVEL_ERROR,
                                      "error state (%d) PULL_AL_QUIT) ",
                                      pPnpbIf->real_cfg[read_event.add_2].elem.state);
                PnpbRealCfgFree  (read_event.add_2);
                break;
            }

            case PNDV_EV_TO_PERI_PERI_STATE_IND_DONE:
            {

                OsGiveSemB(Pnpb.SemId[SEM_PNPB_START]);
                Pnpb.State = PNPB_OPERATING;
                break;
            }

/*special events valid for device with system redundancy*/
#ifdef PNPB_SYSTEM_REDUNDANCY
            case PNDV_EV_TO_PERI_SR_EDGE_IND_DONE:
            {

            	/*parameters: ar_id, edge*/
                OsEnterX (OS_MUTEX_PNPB_IO);
            	pnpb_io_s2_ar_set_trigger_cnf(read_event.add_1, read_event.add_2);
            	OsExitX (OS_MUTEX_PNPB_IO);
            	break;
            }

#endif

            case PNDV_EV_TO_PERI_CONNECT_IND:
            { // add_1: ar_idx,  add_2: ar_type

#ifdef PNPB_SYSTEM_REDUNDANCY
            	PNIO_UINT16 ar_valid = PNIO_AR_TYPE_SINGLE;
            	switch (read_event.add_2)
            	{
            		case PNIO_AR_TYPE_SINGLE:
					{
						pnpb_ArType[ read_event.add_1 ] = PNIO_AR_TYPE_SINGLE;
						break;
					}
            		case PNIO_AR_TYPE_SUPERVISOR:
					{
						pnpb_ArType[ read_event.add_1 ] = PNIO_AR_TYPE_SUPERVISOR;
						break;
					}
            		case PNIO_AR_TYPE_SINGLE_RTC3:
					{
						pnpb_ArType[ read_event.add_1 ] = PNIO_AR_TYPE_SINGLE_RTC3;
						break;
					}
            		case PNIO_AR_TYPE_SINGLE_SYSRED:
            		{
            		    pnpb_ArType[ read_event.add_1 ] = PNIO_AR_TYPE_SINGLE_SYSRED;

            		    OsEnterX (OS_MUTEX_PNPB_IO);
            		    Pnpb_ar_sr_set[ read_event.add_1 ] = pPnpbIf->ar[ read_event.add_1 ].sr_ar_set_nr;
            	        /*load time for redundancy switchover data hold*/
            	        pnpb_data.io.s2.rdht[Pnpb_ar_sr_set[ read_event.add_1 ]] = pnpb_max__(pnpb_data.io.s2.rdht[Pnpb_ar_sr_set[ read_event.add_1 ]], pPnpbIf->ar[read_event.add_1].sr_rdht_msec);
                        OsExitX (OS_MUTEX_PNPB_IO);
            			break;
            		}

            		default: ar_valid = PNIO_AR_TYPE_RESERVED;
            	}
            	if (PNIO_AR_TYPE_RESERVED == ar_valid)
            	{
            		PNPB_SYS_TRACE_01(LSA_TRACE_LEVEL_ERROR, "CONNECT_IND: Invalid AR type (0x%0X)", read_event.add_2);
            		pnpb_write_event_to_pndv(PNDV_EV_TO_PNDV_CONNECT_IND_DONE, read_event.add_1, PNIO_NOT_OK, NULL);
            	}
            	else
            	{
					PNIO_cbf_ar_connect_ind (0,                                                 // DevHndl,
											pnpb_ArType[ read_event.add_1 ],                    // DevHndl, AR type
                                             read_event.add_1 + 1,                              // DevHndl, AR type, AR number
                                             pPnpbIf->ar[read_event.add_1].ar_session_key,
                                             pPnpbIf->ar[read_event.add_1].send_clock,          // sendclock
                                             pPnpbIf->ar[read_event.add_1].reduction_ratio[0],  // reduction ratio input iocr
                                             pPnpbIf->ar[read_event.add_1].reduction_ratio[1],
                                             pPnpbIf->ar[read_event.add_1].host_ip);            // Use network byte orded since it is big endian!

                pnpb_remove_all_sub (read_event.add_1);  // init memory for all exp. submodules of this AR
#else
            	PNIO_AR_TYPE ArType;
            	switch (read_event.add_2)
            	{
            		case PNIO_AR_TYPE_SINGLE: 			ArType = PNIO_AR_TYPE_SINGLE; 		break;
            		case PNIO_AR_TYPE_SUPERVISOR: 		ArType = PNIO_AR_TYPE_SUPERVISOR; 	break;
            		case PNIO_AR_TYPE_SINGLE_RTC3: 		ArType = PNIO_AR_TYPE_SINGLE_RTC3; 	break;
            		case PNIO_AR_TYPE_SINGLE_SYSRED: 	ArType = PNIO_AR_TYPE_RESERVED; 	break; // not supported yet
            		default: ArType = PNIO_AR_TYPE_RESERVED;
            	}
            	if (PNIO_AR_TYPE_RESERVED == ArType)
            	{
            		PNPB_SYS_TRACE_01(LSA_TRACE_LEVEL_ERROR, "CONNECT_IND: Invalid AR type (0x%0X)", read_event.add_2);
            		pnpb_write_event_to_pndv(PNDV_EV_TO_PNDV_CONNECT_IND_DONE, read_event.add_1, PNIO_NOT_OK, NULL);
            	}
            	else
            	{
					PNIO_cbf_ar_connect_ind (0,                                                // DevHndl,
                                             ArType,                                           // DevHndl, AR type
                                             read_event.add_1 + 1,                             // DevHndl, AR type, AR number
                                             pPnpbIf->ar[read_event.add_1].ar_session_key,
                                             pPnpbIf->ar[read_event.add_1].send_clock,         // sendclock
                                             pPnpbIf->ar[read_event.add_1].reduction_ratio[0], // reduction ratio input iocr
                                             pPnpbIf->ar[read_event.add_1].reduction_ratio[1],
                                             pPnpbIf->ar[read_event.add_1].host_ip);           // Use network byte orded since it is big endian!

                    pnpb_remove_all_sub (read_event.add_1);  // init memory for all exp. submodules of this AR

                    PnpbExp[read_event.add_1].ArType = read_event.add_2;
#endif
                    // **** set AR state in PNPB state machine ****
                    Pnpb.ArState[read_event.add_1] = PNPB_AR_CONNECT_IND;

					/* Remember pending AR */
					Pnpb.ArPending = read_event.add_1;

					// ***** send confirmation to PNDV *****
					pnpb_write_event_to_pndv(PNDV_EV_TO_PNDV_CONNECT_IND_DONE, read_event.add_1, PNIO_OK, NULL);
            	}
            	PNPB_TRIGGER_PNDV();
                break;
            }

            /* ----------------------------------------------------- */
            case PNDV_EV_TO_PERI_OWN_IND_MORE_FOLLOWS:
            { // add_1: ar_idx,  add_2: entity_idx in set_cfg
                PNDV_SET_CFG_ELEMENT_T*          pElem;
                PNIO_EXP_SUB*   pSubExp = 0;

                if (Pnpb.ArState[read_event.add_1] == PNPB_AR_CONNECT_IND) // first call after connect indication ?
                {
                    PNIO_UINT32 IocrInDatLen = 0;
                    PNIO_UINT32 IocrOutDatLen = 0;
                    iom_get_iocr_buffer_sizes (read_event.add_1,
                                               &IocrInDatLen,
                                               &IocrOutDatLen);

                    if ((IocrInDatLen == 0) && (IocrOutDatLen == 0))
                    {
                        pnpb_device_ar_abort (read_event.add_1+1);
                    }
                }

                pElem = &(pPnpbIf->set_cfg[read_event.add_1][read_event.add_2].elem);
                #if 1   // 2015-05-05  patch01.2
                    {
                        PNIO_UINT32  Flags = pPnpbIf->set_cfg[read_event.add_1][read_event.add_2].flags;
                        pnpb_sub_exp_add (read_event.add_1,     // AR number
                                          read_event.add_2,     // entity_idx in set_cfg
                                          pElem,                // [in]  ownership data
                                          Flags,                // flags, including ownership flag
                                          &pSubExp,             // [out]
                                          PNIO_TRUE);           // more follows = TRUE
                    }
                #else
                    pnpb_sub_exp_add (read_event.add_1,     // AR number
                                      read_event.add_2,     // entity_idx in set_cfg
                                      pElem,                // [in]  ownership data
                                      &pSubExp,             // [out]
                                      PNIO_TRUE);           // more follows = TRUE
                #endif
                // **** set AR state in PNPB state machine ****
                Pnpb.ArState[read_event.add_1] = PNPB_AR_OWNERSHIP_STARTED;

                // ***** store confirmation to PNDV temporarily in fifo *****
                pnpb_write_event_to_fifo   (read_event.add_1,
                                            PNDV_EV_TO_PNDV_OWN_IND_DONE_MORE_FOLLOWS,
                                            read_event.add_1,
                                            read_event.add_2,
                                            pSubExp);      // save ptr to exp-config-element

                PNPB_TRIGGER_PNDV ();
                break;
            }

            /* ----------------------------------------------------- */
            case PNDV_EV_TO_PERI_OWN_IND:
            { // add_1: ar_idx,  add_2: entity_idx in set_cfg

                if (Pnpb.ArState[read_event.add_1] == PNPB_AR_CONNECT_IND) // first call after connect indication ?
                {
                    PNIO_UINT32 IocrInDatLen = 0;
                    PNIO_UINT32 IocrOutDatLen = 0;
                    iom_get_iocr_buffer_sizes (read_event.add_1,
				                               &IocrInDatLen,
					                           &IocrOutDatLen);

                    if ((IocrInDatLen == 0) && (IocrOutDatLen == 0))
                    {
                        pnpb_device_ar_abort (read_event.add_1+1);
                    }
                }

                { // **** send event to ownership-indication handler task for further use ****
                    PNDV_IFACE_CMD_ENTRY_T  *pReadEvent;
                    OsAlloc ((void**) &pReadEvent, 0, sizeof (PNDV_IFACE_CMD_ENTRY_T));
                    pReadEvent->cmd   = read_event.cmd;
                    pReadEvent->add_1 = read_event.add_1;
                    pReadEvent->add_2 = read_event.add_2;
                    OsSendMessage (TskId_PnpbPost, pReadEvent, OS_MBX_PRIO_NORM);
                }
                break;
            }

            case PNDV_EV_TO_PERI_PRM_END_IND_MORE_FOLLOWS:
            { // add_1: ar_idx,  add_2: entity_idx in set_cfg

                // **** set AR state in PNPB state machine ****
                Pnpb.ArState[read_event.add_1] = PNPB_AR_PARAM_END_STARTED;

                // ***** send confirmation to PNDV *****
                pnpb_write_event_to_fifo   (read_event.add_1,
                                            PNDV_EV_TO_PNDV_PRM_END_IND_DONE_MORE_FOLLOWS,
                                            read_event.add_1,
                                            read_event.add_2,
                                            NULL);

                break;
            }

            case PNDV_EV_TO_PERI_PRM_END_IND:
            { // add_1: ar_idx,  add_2: entity_idx in set_cfg
                PNIO_UINT8   ar_idx     = read_event.add_1;  // AR index in exp_sub
                PNIO_UINT16  entity_idx = read_event.add_2;  // entity Index;
                PNIO_UINT32  appl_ready;
                PNIO_UINT32  DoAsyncApplReady = PNIO_FALSE;

                pnpb_write_event_to_fifo   (ar_idx,
                                            PNDV_EV_TO_PNDV_PRM_END_IND_DONE,
                                            ar_idx,
                                            read_event.add_2,
                                            NULL);

                PnpbExp[read_event.add_1].FirstParamEndValid = PNIO_TRUE;

                {  // *** first confirm all queued element to PNDV ***
                    PNIO_UINT32 i;
                    for (i = 0; i < WrEventFifo[ar_idx].NumOfEntries; i++)
                    {
                        PNDV_SET_CFG_ELEMENT_T*          pElem;
                        CM_SV_AR_OWNERSHIP_ELEMENT_TYPE* pOwn;
                        PNIO_BOOL                        MoreFollows;

                        entity_idx = WrEventFifo[ar_idx].Entry[i].add_2;  // entity Index;


                        // *** get pointer to ownership element in exp_cfg ***

                        // *** get pointer to ownership element in exp_cfg ***
                        pElem = &(pPnpbIf->set_cfg[ar_idx][entity_idx].elem);
                        pOwn  = &(pElem->own_info);

                        { // find expected submodule and mark ParamEnd as passed.
                            PNIO_EXP_SUB*   pSubExp = pnpb_sub_exp_oneAr_getp (ar_idx,
                                                                               pOwn->api,
                                                                               pOwn->slot_nr,
                                                                               pOwn->subslot_nr);

                            if (pSubExp)
                            {
                                pSubExp->ParamEndValid = PNIO_TRUE;
                            }
                        }


                        if (i == (WrEventFifo[ar_idx].NumOfEntries - 1))
                        {
                            MoreFollows = PNIO_FALSE;
                        }
                        else
                        {
                            MoreFollows = PNIO_TRUE;
                        }


                        appl_ready = PNIO_cbf_param_end_ind (  PNIO_SINGLE_DEVICE_HNDL,
                                                               (PNIO_UINT16) (ar_idx + 1),
                                                               pOwn->owner_session_key,
                                                               pOwn->api,
                                                               pOwn->slot_nr,
                                                               pOwn->subslot_nr,
                                                               MoreFollows
                                                            );

                        if (appl_ready == PNIO_SUBMOD_STATE_STOP)
                        {
                           // pPnpbIf->set_cfg[ar_idx][entity_idx].elem.cmp_result = PNDV_CMP_RES_ERROR;
                            pPnpbIf->set_cfg[ar_idx][entity_idx].flags |= PNDV_SET_CFG_FLAGS_SET_ARP; // set bit 4 (applicatin ready pending...)
                        }
                        else if (appl_ready == PNIO_SUBMOD_STATE_APPL_RDY_FOLLOWS)
                        {
                            DoAsyncApplReady = PNIO_TRUE;
                        }
                        else if (   (MoreFollows == PNIO_FALSE)                     // last param end
                                 && (appl_ready == PNIO_SUBMOD_STATE_RUN)           // subslot ok
                                 && (PnpbExp[ar_idx].Rdy4InpUpdateValid == PNIO_TRUE))  // AR is already in IO data exchange (submodule replugged..)
                        { // we are already in IO data exchange and submodule has been replugged....
                            if (PnpbExp[ar_idx].Rdy4InpUpdateValid == PNIO_TRUE)
                            {
                                PnpbExp[read_event.add_1].IoUpdatePending = PNIO_TRUE;

                                // **** perform IO update here, because readyForInputUpdate event does not come again from PNDV ****
                                PNIO_cbf_ready_for_input_update_ind (PNIO_SINGLE_DEVICE_HNDL,       // device handle
                                                                     ar_idx + 1,                    // AR number = AR index + 1
                                                                     PNIO_AR_INDATA);               // use case replug a submodule

                                while (PnpbExp[read_event.add_1].IoUpdatePending)
                                {
                                    OsWait_ms(IO_UPDATE_POLLING_MS);
                                }
                            }

                        }

                        pElem->prm_end_state = PNDV_IFACE_SERVICE_IDLE;
                     }

                    if (DoAsyncApplReady == PNIO_FALSE)
                    {
                        for (i = 0; i < WrEventFifo[ar_idx].NumOfEntries; i++)
                        {
                            // ** send confirmation to PNDV **
                            pnpb_write_event_to_pndv
                                     (WrEventFifo[ar_idx].Entry[i].cmd,
                                      WrEventFifo[ar_idx].Entry[i].add_1,
                                      WrEventFifo[ar_idx].Entry[i].add_2,
                                      0);
                            if (i == (WrEventFifo[ar_idx].NumOfEntries - 1))
                                Pnpb.ArState[ar_idx] = PNPB_AR_PARAM_END_FINISHED;

                            PNPB_TRIGGER_PNDV ();
                        }


                        // *** clear fifo  (is empty now..)  ***
                        WrEventFifo[ar_idx].NumOfEntries = 0;   // queue is empty now
                    }

                }
                break;
            }

            case PNDV_EV_TO_PERI_SM_IN_DATA_MORE_FOLLOWS:
            { // add_1: ar_idx,  add_2: entity_idx in set_cfg
                PNIO_UINT8   ar_idx     = read_event.add_1;  // AR index in exp_sub
                PNIO_UINT16  entity_idx = read_event.add_2;  // entity Index;

                // *** get pointer to ownership element in exp_cfg ***
                // PNDV_SET_CFG_ELEMENT_T*          pElem;
                // pElem = &(pPnpbIf->set_cfg[ar_idx][entity_idx].elem);

                // *** call INDATA USER function ***
                // PNIO_printf ("SM indata more follows Slot=%d Subslot= %d\n",  // &&&2do
                //             pElem->own_info.slot_nr,
                //             pElem->own_info.subslot_nr);

                // ***** send confirmation to PNPB *****
                pnpb_write_event_to_pndv(PNDV_EV_TO_PNDV_SM_IN_DATA_DONE_MORE_FOLLOWS,
                                         ar_idx,
                                         entity_idx,
                                         NULL);
                PNPB_TRIGGER_PNDV ();
                break;
            }

            case PNDV_EV_TO_PERI_SM_IN_DATA:
            { // add_1: ar_idx,  add_2: entity_idx in set_cfg
                PNIO_UINT8   ar_idx     = read_event.add_1;  // AR index in exp_sub
                PNIO_UINT16  entity_idx = read_event.add_2;  // entity Index;

                // *** get pointer to ownership element in exp_cfg ***
                // PNDV_SET_CFG_ELEMENT_T*          pElem;
                // pElem = &(pPnpbIf->set_cfg[ar_idx][entity_idx].elem);

                // *** call INDATA USER function ***
                // PNIO_printf ("SM indata  Slot=%d Subslot= %d\n",  // &&&2do
                //              pElem->own_info.slot_nr,
                //              pElem->own_info.subslot_nr);

                PNIO_cbf_ar_indata_ind (PNIO_SINGLE_DEVICE_HNDL,                        // device handle
                                        read_event.add_1 + 1,                           // AR number
                                        pPnpbIf->ar[read_event.add_1].ar_session_key);  // session key

                // **** set AR state in PNPB state machine ****
                Pnpb.ArState[read_event.add_1] = PNPB_AR_SM_INDATA_IND;

                // ***** send confirmation to PNPB *****
                pnpb_write_event_to_pndv(PNDV_EV_TO_PNDV_SM_IN_DATA_DONE,
                                         ar_idx,
                                         entity_idx,
                                         NULL);


                PNPB_TRIGGER_PNDV ();
                break;
            }


            case PNDV_EV_TO_PERI_AR_IN_DATA:
            {
                // **** set AR state in PNPB state machine ****
                Pnpb.ArState[read_event.add_1] = PNPB_AR_INDATA_IND;
#ifdef PNPB_SYSTEM_REDUNDANCY
				if( PNIO_AR_TYPE_SINGLE_SYSRED == pnpb_ArType[ read_event.add_1 ] )
				{
					int ar_act = read_event.add_1;
					int ar_other = PNPB_AR_OTHER(read_event.add_1);

					/* Global parameterization already ready from other ar */
					if(PNIO_TRUE == PnpbExp[PNPB_AR_OTHER(read_event.add_1)].FirstParamEndValid)
					{
						PnpbExp[ar_act].FirstParamEndValid = PNIO_TRUE;
					}

					/* If no other AR - no reason why save previous values to actual */
					if(ar_other < IOD_CFG_NUMOF_AR)
					{
						/* Other AR found */

						/* Parameterization of single modules already ready from other ar */
						/* Find and match with previous configuration */
						int j, k;
						PNIO_printf("NumOfPluggedSub: %d, ARother: %d, NumOfPreviouslyPluggedSub: %d\n",
								PnpbExp[ar_act].NumOfPluggedSub, ar_other, PnpbExp[ar_other].NumOfPluggedSub);

						for(j = 0; j < PnpbExp[ar_act].NumOfPluggedSub; j++)
						{
							for(k = 0; k < PnpbExp[ar_other].NumOfPluggedSub; k++)
						{

								/* Bug fix - after removing module in HW configuration and then adding another one in the
								 * same place (both configuration loaded to CPU) there were created a hole which has
								 * to be skipped when old values are restored back! Therefore the check of slot and subslot
								 * number was added. */

								if ((PnpbExp[ar_other].Sub[k].ApiNum == PnpbExp[ar_act].Sub[j].ApiNum)
										&& (PnpbExp[ar_other].Sub[k].SlotNum == PnpbExp[ar_act].Sub[j].SlotNum)
										&& (PnpbExp[ar_other].Sub[k].SubNum == PnpbExp[ar_act].Sub[j].SubNum))
								{
									/* Match! */
									if (PNIO_TRUE == PnpbExp[ar_other].Sub[k].ParamEndValid)
									{
										PnpbExp[ar_act].Sub[j].ParamEndValid = PNIO_TRUE;
                                    }
									if( (0 != PnpbExp[ar_other].Sub[k].OwnSessionKey)
											&& (0 == PnpbExp[ar_act].Sub[j].OwnSessionKey) )
                                    {
										PnpbExp[ar_act].Sub[j].OwnSessionKey = PnpbExp[ar_other].Sub[k].OwnSessionKey;
									}

									break;
						        }
					        }
						}
					}
				}

#endif




                #if 0  // we ignore this request, because no response code has been defined
                    // ***** send confirmation to PNPB *****
                    pnpb_write_event_to_pndv(PNDV_EV_TO_PNDV_SM_IN_DATA_DONE,
                                             ar_idx,
                                             entity_idx,
                                             NULL);
                    PNPB_TRIGGER_PNDV ();
                #endif

                break;
            }

            /* ----------------------------------------------------- */
            case PNDV_EV_TO_PERI_DS_RW:
            { // add_1: ar_idx,  add_2: ---

                pnpb_rec_read_write  (read_event.add_1,
                                      read_event.add_2,
                                      pndv_iface_ptr);



                break;
            }

            case PNDV_EV_TO_PERI_AR_DISCONNECT_IND:
            { // **** send event to ownership-indication handler task for further use ****
                PNDV_IFACE_CMD_ENTRY_T  *pReadEvent;
                OsAlloc ((void**) &pReadEvent, 0, sizeof (PNDV_IFACE_CMD_ENTRY_T));
                pReadEvent->cmd   = read_event.cmd;
                pReadEvent->add_1 = read_event.add_1;
                pReadEvent->add_2 = read_event.add_2;
                OsSendMessage (TskId_PnpbPost, pReadEvent, OS_MBX_PRIO_NORM);
                break;
            }

            case PNDV_EV_TO_PERI_CHANNEL_DIAG_QUIT:
            {
                // **** call user function *****
                DiagAlarmProcessing = PNIO_FALSE;
                pnpb_async_req_done (read_event.add_2, PNIO_ALM_CHAN_DIAG);
                pPnpbIf->real_cfg[read_event.add_2].dial.state = PNDV_IFACE_SERVICE_IDLE;
                break;
            }


            case PNDV_EV_TO_PERI_EXT_CHANNEL_DIAG_QUIT:
            {
                // **** call user function  *****
                DiagAlarmProcessing = PNIO_FALSE;
                pnpb_async_req_done (read_event.add_2, PNIO_ALM_EXT_CHAN_DIAG);
                pPnpbIf->real_cfg[read_event.add_2].xdial.state = PNDV_IFACE_SERVICE_IDLE;
                break;
            }

            case PNDV_EV_TO_PERI_GENERIC_DIAG_QUIT:
            {
                // **** call user function *****
                DiagAlarmProcessing = PNIO_FALSE;
                pnpb_async_req_done (read_event.add_2, PNIO_ALM_GEN_DIAG);
                pPnpbIf->generic_diag_data[read_event.add_2].state = PNDV_IFACE_SERVICE_IDLE;
                break;
            }

            /* ----------------------------------------------------- */
            case PNDV_EV_TO_PERI_PRAL_QUIT:
            {
                // **** call user function *****
                ProcessAlarmProcessing = PNIO_FALSE;
                pnpb_async_req_done (read_event.add_2, PNIO_ALM_PROC);
                pPnpbIf->real_cfg[read_event.add_2].pral.state = PNDV_IFACE_SERVICE_IDLE;
                break;
            }

            /* ----------------------------------------------------- */
            case PNDV_EV_TO_PERI_STAL_QUIT:
            {
                // **** call user function *****
            	StatusAlarmProcessing = PNIO_FALSE;
                pnpb_async_req_done (read_event.add_2, PNIO_ALM_STATUS);
                pPnpbIf->real_cfg[read_event.add_2].stal.state = PNDV_IFACE_SERVICE_IDLE;
                break;
            }

            /* ----------------------------------------------------- */
            case PNDV_EV_TO_PERI_ROS_AL_QUIT:
            {
                // **** call user function &&&2Do for DK4.0.0 *****
                RosAlarmProcessing = PNIO_FALSE;
                pnpb_async_req_done (read_event.add_2, PNIO_ALM_RET_OF_SUB);
                pPnpbIf->real_cfg[read_event.add_2].ros.state = PNDV_IFACE_SERVICE_IDLE;
               break;
            }

            /* ----------------------------------------------------- */
            case PNDV_EV_TO_PERI_URAL_QUIT:
            {
                // **** call user function &&&2Do for DK4.0.0 *****
                UplRetrAlarmProcessing = PNIO_FALSE;
                pnpb_async_req_done (read_event.add_2, PNIO_ALM_UPLOAD_RETRIEVAL);
                pPnpbIf->real_cfg[read_event.add_2].ural.state = PNDV_IFACE_SERVICE_IDLE;
                break;
            }

            case PNDV_EV_TO_PERI_READY_FOR_INPUT_UPDATE:
            {
                // allow single IO update before InData
                //PnpbExp[read_event.add_1].ArForceUpdate = PNIO_TRUE;

                PnpbExp[read_event.add_1].Rdy4InpUpdateValid = PNIO_TRUE;
                PnpbExp[read_event.add_1].IoUpdatePending    = PNIO_TRUE;

                // **** 1st update IO data by user ****
                PNIO_cbf_ready_for_input_update_ind (PNIO_SINGLE_DEVICE_HNDL,       // device handle
                                                     read_event.add_1 + 1,          // AR number = AR index + 1
                                                     PNIO_AR_STARTUP);              // use case first valid input update after AR startup

                while (PnpbExp[read_event.add_1].IoUpdatePending)
                {
                    OsWait_ms(IO_UPDATE_POLLING_MS);
                }

                pnpb_write_event_to_pndv(PNDV_EV_TO_PNDV_READY_FOR_INPUT_UPDATE_DONE,
                                         read_event.add_1,
                                         read_event.add_2,
                                         NULL);
                PNPB_TRIGGER_PNDV ();
               break;
            }

		    case PNDV_EV_TO_PERI_AR_ABORT_REQ_DONE:
			{
				break;
			}

            /* ----------------------------------------------------------------- */
            default:
            {
                PNPB_SYS_TRACE_04(LSA_TRACE_LEVEL_FATAL,  "unknown peri cmd=%d par1=0x%x par2=0x%x par3=0x%x",
                                  PNPB_TRACE_READ_EVENT, read_event.cmd, read_event.add_1, read_event.add_2);
                break;
            }
        }
    }
}

PNIO_UINT32  PNIO_async_appl_rdy
                    (PNIO_UINT32        DevHndl,            // [in] handle for a multidevice
                     PNIO_UINT16        ArNum,              // [in] ar - number (1..N)
                     PNIO_UINT32        Api,                // [in] API (valid only, if SubslotNum <> 0)
                     PNIO_UINT16        SlotNum,            // [in] SlotNum (valid only, if SubslotNum <> 0)
                     PNIO_UINT16        SubNum,             // [in] 0: for all modules, <>0: for this submodule
                     PNIO_SUBMOD_STATE  SubState,           // [in] PNIO_SUBMOD_STATE_RUN, PNIO_SUBMOD_STATE_STOP
                     PNIO_BOOL          MoreFollows)        // [in] PNIO_TRUE: application ready to stack is executed, PNIO_FALSE: application ready to stack is still delayed
{
    PNIO_UINT32 ar_idx = ArNum - 1;

    if (ArNum == 0)
    {
        PNPB_API_TRACE_04(LSA_TRACE_LEVEL_ERROR,  "invalid ArNum (AR%d Api%d Sl%d Sub%d)\n", ArNum, Api, SlotNum, SubNum);
        return (PNIO_NOT_OK);
    }

    if (SubState == PNIO_SUBMOD_STATE_APPL_RDY_FOLLOWS)
    {
        PNPB_API_TRACE_04(LSA_TRACE_LEVEL_ERROR,  "invalid substate ALL_RDY_FOLLOWS (AR%d Api%d Sl%d Sub%d)\n", ArNum, Api, SlotNum, SubNum);
        return (PNIO_NOT_OK);        // not allowed here, SubState must be RUN or STOP
    }


    if (SubState == PNIO_SUBMOD_STATE_STOP)
    {
        PNIO_EXP_SUB* pSubExp;
        pSubExp = pnpb_sub_exp_oneAr_getp (  ar_idx,   //AR index
                                             Api,
                                             SlotNum,
                                             SubNum);

        if (pSubExp)
        {
            //pPnpbIf->set_cfg[ar_idx][pSubExp->EntityIndSetCfg].elem.cmp_result = PNDV_CMP_RES_ERROR;
            pPnpbIf->set_cfg[ar_idx][pSubExp->EntityIndSetCfg].flags |= PNDV_SET_CFG_FLAGS_SET_ARP; // set bit 4 (applicatin ready pending...)
            PNPB_API_TRACE_04(LSA_TRACE_LEVEL_NOTE_HIGH,  "set substate INVALID (AR%d Api%d Sl%d Sub%d)\n", ArNum, Api, SlotNum, SubNum);
        }
        else
        {
            PNPB_API_TRACE_04(LSA_TRACE_LEVEL_ERROR,  "submod not found (AR%d Api%d Sl%d Sub%d)\n", ArNum, Api, SlotNum, SubNum);
            return (PNIO_FALSE);        // error: expected submodule not found
        }
    }
    else
    {
        PNPB_API_TRACE_04(LSA_TRACE_LEVEL_NOTE,  "set substate OK (AR%d Api%d Sl%d Sub%d)\n", ArNum, Api, SlotNum, SubNum);
    }

    if (MoreFollows == PNIO_FALSE)
    {
    	PNIO_UINT32 i;
        for (i = 0; i < WrEventFifo[ar_idx].NumOfEntries; i++)
        {
             // ** send confirmation to PNDV **
             pnpb_write_event_to_pndv
                      (WrEventFifo[ar_idx].Entry[i].cmd,
                       WrEventFifo[ar_idx].Entry[i].add_1,
                       WrEventFifo[ar_idx].Entry[i].add_2,
                       0);

             if (i == (WrEventFifo[ar_idx].NumOfEntries - 1))
                 Pnpb.ArState[WrEventFifo[ar_idx].Entry[i].add_1] = PNPB_AR_PARAM_END_FINISHED;

             PNPB_TRIGGER_PNDV ();
        }

        // *** clear fifo  (is empty now..)  ***
        WrEventFifo[ar_idx].NumOfEntries = 0;   // queue is empty now
    }

    return (PNIO_OK);
}



/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
