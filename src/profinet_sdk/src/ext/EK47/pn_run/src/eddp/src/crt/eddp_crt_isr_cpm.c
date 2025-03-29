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
/*  F i l e               &F: eddp_crt_isr_cpm.c                        :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  CRT (Cycle Real Time) for EDDP  -> Interrupts CPM                        */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who         What                                             */
#ifdef EDDP_MESSAGE
/*  09.07.09    Freitag     Erstellung                                       */
#endif
/*****************************************************************************/


/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/

#define LTRC_ACT_MODUL_ID   30
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_CRT_CPM_ISR */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"           /* eddp headerfiles */
#include "eddp_int.h"           /* internal header  */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)


// ??: INT_CPM_DMA_Err          --> indication of TransferEnd after selected reduction
// ??: INT_CPM_OutDataStart     --> if selected nr of data bytes (parameter) are tranfered to RWB
// ??: INT_CPM_OutDataValid     --> good received frame and data transfered to RWB
// ??: INT_CPM_OutDataErr       --> received frame with errors (eg: APDU, old/new detection, ...)
// ??: INT_CSB_changed_IFA      --> end of scoreboard handling within a cycle and changes detected
// ??: INT_CSB_changed_IFB
// --: INT_CPMCmd_Done          --> not used because all commands execute over common CmdInterface
// ok: INT_CPM_ERR              --> parameter error              





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRT_ISR_CSB_changed_IFA                +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32                     EventNr      +*/
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
/*+  Description: This function is called from CORE after the scoreboard    +*/
/*+               interrupt occurred. It is executed within LOW context.    +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRT_ISR_CSB_changed_IFA(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT32                  EventNr)
{

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(EventNr);
#endif


    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRT_ISR_CSB_changed_IFA(EventNr=0x%X)", EventNr);

    /* Check if a CRT-Service is running. If so only set a flag that a      */
    /* ConsumerScoreboard Event was present an call the handler later       */
    /* This is to prevent situations like:                                  */
    /* Consumer activate.                                                   */
    /* AGAIN-Event comes in before Actiavte was signaled to user.           */
    /* We muss first confirmate the activate! So we delay the handler       */

    
    if (EDDP_GlbRQBQueueIsEmpty(&pDDB->GlbCrtRQBHandler.RqbInProgressQueue))
    {
        // no RQB is running currently
        EDDP_CRTScoreboardHandler (pDDB);
    }
    else
    {
        // at least one RQB is running currently
        // call check later after finish of current request
        pDDB->CRT.Cons.SBEventPending = LSA_TRUE;

        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRT_ISR_CSB_changed_IFA(): at least one RQB is running currently");
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRT_ISR_Cpm_Err                        +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32                     EventNr      +*/
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
/*+  Description: This function is called from CORE after a CPM error       +*/
/*+               interrupt "INT_CPM_ERR" occurred.                         +*/
/*+               It is executed within LOW context.                        +*/
/*+                                                                         +*/
/*+               Register "PNIP_REG_IRQ3EVENT_LOW_0"                       +*/
/*+               Interrupt event "EDDP_INT_CPM_ERR"                        +*/
/*+                 -> Bit(15): 0x00008000                                  +*/
/*+                                                                         +*/
/*+               Get info of register                                      +*/
/*+                 - PNIP_REG_CPMERRSTATUS                                 +*/
/*+                 - PNIP_REG_CPMERRADR                                    +*/
/*+               and generate FATAL-ERROR!                                 +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRT_ISR_Cpm_Err( 
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT32                  EventNr)
{
    LSA_UINT32          RegCpmErrStatus;
    LSA_UINT32          RegCpmErrAdr;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(EventNr);
#endif

    RegCpmErrStatus     = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_CPMERRSTATUS);
    RegCpmErrAdr        = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_CPMERRADR);

    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRT_ISR_Cpm_Err(EventNr=0x%X): FATAL -> RegCpmErrStatus=0x%X, RegCpmErrAdr=0x%X", 
        EventNr, RegCpmErrStatus, RegCpmErrAdr);
    EDDP_FATAL(EDDP_FATAL_ERR_UNEXP_INTERRUPT);
}
