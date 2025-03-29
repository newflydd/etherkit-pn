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
/*  F i l e               &F: eddp_crt_isr_ppm.c                        :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  CRT (Cycle Real Time) for EDDP  -> Interrupts PPM                        */
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

#define LTRC_ACT_MODUL_ID   31
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_CRT_PPM_ISR */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"           /* eddp headerfiles */
#include "eddp_int.h"           /* internal header  */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRT_ISR_Ppm_Err                        +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32                     EventNr      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  EventNr    : Event Number (Interrupt Number)                     (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function is called by CORE from the                  +*/
/*+               PPM error interrupt "INT_PPM_ERR".                        +*/
/*+               It is executed within LOW context.                        +*/
/*+                                                                         +*/
/*+               Register "PNIP_REG_IRQxEVENT_LOW_0"                       +*/
/*+               Interrupt event "EDDP_INT_PPM_ERR"                        +*/
/*+                 -> Bit(16): 0x00010000                                  +*/
/*+                                                                         +*/
/*+               Get info of register                                      +*/
/*+                 - PNIP_REG_PPMERRSTATUS                                 +*/
/*+                 - PNIP_REG_PPMERRADR                                    +*/
/*+               and generate FATAL-ERROR!                                 +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRT_ISR_Ppm_Err(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr)
{
    LSA_UINT32          RegPpmErrStatus;
    LSA_UINT32          RegPpmErrAdr;
    LSA_UINT32          RegPpmErrAdr2;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(EventNr);
#endif

    switch(EDDP_HALGetPnipRevision(pDDB))
    {
        case EDDP_HAL_HW_PNIP_REV1:
        case EDDP_HAL_HW_PNIP_REV2:
        {
            RegPpmErrStatus     = EDDP_HAL_REG32_READ(pDDB, PNIP_R2_REG_PPMERRSTATUS);
            RegPpmErrAdr        = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_PPMERRADR);

            EDDP_CRT_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRT_ISR_Ppm_Err(EventNr=0x%X): FATAL -> RegPpmErrStatus=0x%X, RegPpmErrAdr=0x%X, AcwUsedCnt=%d, AcwActCnt-IFA=%d", 
                              EventNr, RegPpmErrStatus, RegPpmErrAdr, pDDB->CRT.Prov.AcwUsedCnt, pDDB->CRT.Prov.AcwActCnt);
        }
        break;

        case EDDP_HAL_HW_PNIP_REV3:
        {    
            RegPpmErrStatus     = EDDP_HAL_REG32_READ(pDDB, PNIP_R3_REG_PPMERRSTATUS);
            RegPpmErrAdr        = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_PPMERRADR);
            RegPpmErrAdr2       = EDDP_HAL_REG32_READ(pDDB, PNIP_R3_REG_PPMERRADR2);

            EDDP_CRT_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRT_ISR_Ppm_Err(EventNr=0x%X): FATAL -> RegPpmErrStatus=0x%X, RegPpmErrAdr=0x%X, RegPpmErrAdr2=0x%X, AcwUsedCnt=%d, AcwActCnt-IFA=%d", 
                EventNr, RegPpmErrStatus, RegPpmErrAdr, RegPpmErrAdr2, pDDB->CRT.Prov.AcwUsedCnt, pDDB->CRT.Prov.AcwActCnt);
        }
        break;

        default:
        {
            EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRT_ISR_Ppm_Err(): FATAL -> PnipRevision(%d) is unknown", 
            EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }



    EDDP_FATAL(EDDP_FATAL_ERR_UNEXP_INTERRUPT);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRT_ISR_Ppm_not_finished_IFA           +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32                     EventNr      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  EventNr    : Event Number (Interrupt Number)                     (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function is called by CORE from the                  +*/
/*+               "PPM_not_finished_IFA" Interrupt                          +*/
/*+                 -> not all frames were sent in a cycle.                 +*/
/*+               It is executed within LOW context.                        +*/
/*+                                                                         +*/
/*+               Register "PNIP_REG_IRQxEVENT_MID_0"                       +*/
/*+               Interrupt event "EDDP_INT_PPM_NOT_FINISHED_IFA"           +*/
/*+                 -> Bit(16): 0x00010000                                  +*/
/*+                                                                         +*/
/*+               Indicate the provider DROP-Event                          +*/
/*+               -> EDD_CSRT_PROV_EVENT_DROP                               +*/
/*+                                                                         +*/
/*+               To do:                                                    +*/
/*+                 - Deactivate this Interrupt                             +*/
/*+                 - Indicate the provider DROP-Event                      +*/
/*+                   EDD_CSRT_PROV_EVENT_DROP                              +*/
/*+                 - start the PpmNotFinishedTimer                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRT_ISR_Ppm_not_finished_IFA(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr)
{
#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(EventNr);
#endif

    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H:--] EDDP_CRT_ISR_Ppm_not_finished_IFA(EventNr=0x%X): AcwUsedCnt=%d, AcwActCnt-IFA=%d", 
        EventNr, pDDB->CRT.Prov.AcwUsedCnt, pDDB->CRT.Prov.AcwActCnt);
    // do not delete the empty spaces in this trace text
    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--]    Deactivate the ISR_Ppm_not_finished and start the PpmNotFinishedTimer(%ds)", 
        EDDP_CRT_PPM_NOT_FINISHED_TIME);

    // deactivate this interrupt
    EDDP_CoreISRControlInterrupt (pDDB, LSA_FALSE, EventNr);


    // start the timer
    EDDP_CoreTimerStart (pDDB->CRT.Prov.PpmNotFinishedTimer, EDDP_CRT_PPM_NOT_FINISHED_TIME);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRT_ISR_Ppm_HOL_not_empty_IFA          +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32                     EventNr      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  EventNr    : Event Number (Interrupt Number)                     (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function is called by CORE from the                  +*/
/*+               "PPM_HOL_not_empty_IFA" Interrupt                         +*/
/*+               It is executed within LOW context.                        +*/
/*+                                                                         +*/
/*+               Register "PNIP_REG_IRQxEVENT_MID_0"                       +*/
/*+               Interrupt event "EDDP_INT_PPM_HOL_NOT_EMPTY_IFA"          +*/
/*+                 -> Bit(18): 0x00040000                                  +*/
/*+                                                                         +*/
/*+               To do:                                                    +*/
/*+                 - trace REG_PPM_HOL_NOT_EMPTY -> ResourceType-Nr.       +*/
/*+                 - Deactivate this Interrupt                             +*/
/*+                 - Indicate the provider DROP-Event                      +*/
/*+                   EDD_CSRT_PROV_EVENT_DROP                              +*/
/*+                 - start the PpmHolNotEmptyTimer                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRT_ISR_Ppm_HOL_not_empty_IFA(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr)
{
    LSA_UINT32      RegPpmHolNotEmpty, RegHolStatus, RegResLackCounter;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(EventNr);
#endif

    RegPpmHolNotEmpty   = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_PPM_HOL_NOT_EMPTY_IFA);
    RegHolStatus        = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_HOL_STATUS_IFA);
    RegResLackCounter   = 0;

    // RegResLackCounter dependent of PNIP revision
    switch (EDDP_HALGetPnipRevision(pDDB))
    {
        #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
        case EDDP_HAL_HW_PNIP_REV1:
        {
            // there is no PNIP_R2_REG_PPM_RESSOURCE_LACK_COUNTER_IFA in revision 1
        }
        break;
        #endif

        #if defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
        case EDDP_HAL_HW_PNIP_REV2:
        case EDDP_HAL_HW_PNIP_REV3:
        {
            /* PNIP_R2_REG_PPM_RESSOURCE_LACK_COUNTER_IFA was updated by PNIP_R3_REG_PPM_RESSOURCE_LACK_COUNTER_IFA*/
            RegResLackCounter = EDDP_HAL_REG32_READ(pDDB, PNIP_R3_REG_PPM_RESSOURCE_LACK_COUNTER_IFA);
        }
        break;
        #endif

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRT_ISR_Ppm_HOL_not_empty_IFA(): FATAL -> Version(%d) of PNIP is unknown", 
                EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }

    EDDP_CRT_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H:--] EDDP_CRT_ISR_Ppm_HOL_not_empty_IFA(EventNr=0x%X): RegPpmHolNotEmpty(ResourceType)=0x%X, RegHolStatus=0x%X, RegResLackCounter=0x%X, AcwUsedCnt=%d, AcwActCnt-IFA=%d", 
        EventNr, RegPpmHolNotEmpty, RegHolStatus, RegResLackCounter, pDDB->CRT.Prov.AcwUsedCnt, pDDB->CRT.Prov.AcwActCnt);

    #ifdef EDDP_CRT_ISR_PPM_HOL_NOT_EMPTY_RESOURCETYPE6_RTC3_FATAL_ON
    if (RegPpmHolNotEmpty & PNIP_REG_PPM_HOL_NOT_EMPTY_IFA__MSK_RESSOURCETYPE_6)
    {
        // this error is fatal: ResourceType6-RTC3 (Port1)
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRT_ISR_Ppm_HOL_not_empty_IFA(): FATAL -> RegPpmHolNotEmpty(ResourceType6-RTC3)");
        EDDP_FATAL(EDDP_FATAL_ERR_UNEXP_INTERRUPT);
    }
    #endif

    #ifdef EDDP_CRT_ISR_PPM_HOL_NOT_EMPTY_RESOURCETYPE7_RTC3_FATAL_ON
    if (RegPpmHolNotEmpty & PNIP_REG_PPM_HOL_NOT_EMPTY_IFA__MSK_RESSOURCETYPE_7)
    {
        // this error is fatal: ResourceType7-RTC3 (Port2)
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRT_ISR_Ppm_HOL_not_empty_IFA(): FATAL -> RegPpmHolNotEmpty(ResourceType7-RTC3)");
        EDDP_FATAL(EDDP_FATAL_ERR_UNEXP_INTERRUPT);
    }
    #endif

    // do not delete the empty spaces in this trace text
    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--]    Deactivate the ISR_Ppm_HOL_not_empty and start the PpmHolNotEmptyTimer(%ds)", 
        EDDP_CRT_PPM_HOL_NOT_EMPTY_TIME);

    // deactivate this interrupt
    EDDP_CoreISRControlInterrupt (pDDB, LSA_FALSE, EventNr);

    // start the timer
    EDDP_CoreTimerStart (pDDB->CRT.Prov.PpmHolNotEmptyTimer, EDDP_CRT_PPM_HOL_NOT_EMPTY_TIME);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRT_ISR_PhaseTransferError             +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32                     EventNr      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  EventNr    : Event Number (Interrupt Number)                     (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function is called by CORE from the                  +*/
/*+               "INT_PhaseTransferError" interrupt                        +*/
/*+                 -> faulty project planning of the sending phases.       +*/
/*+               It is executed within LOW context.                        +*/
/*+                                                                         +*/
/*+               Register "PNIP_REG_IRQxEVENT_LOW_0"                       +*/
/*+               Interrupt event "EDDP_INT_PHASETRANSFERERROR"             +*/
/*+                 -> Bit(20): 0x00100000                                  +*/
/*+                                                                         +*/
/*+               Trace only this interrupt!                                +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRT_ISR_PhaseTransferError(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr)
{
    LSA_UINT32          RegPhaseTransferStatus;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(EventNr);
#endif

    RegPhaseTransferStatus = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_PHASETRANSFER_STATUS);

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRT_ISR_PhaseTransferError(EventNr=0x%X): ERROR -> RegPhaseTransferStatus(TxPort)=0x%X", 
        EventNr, RegPhaseTransferStatus);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTPpmHolNotEmptyTimerPassed                           +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: This function is called by a TimerRequest if the "PpmHolNotEmptyTimer"    +*/
/*+               has passed.                                                               +*/
/*+                                                                                         +*/
/*+               Activate the PPM_HOL_not_empty interrupt.                                 +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTPpmHolNotEmptyTimerPassed(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTPpmHolNotEmptyTimerPassed(): Activate the ISR_Ppm_HOL_not_empty");

    // activate this interrupt
    EDDP_CoreISRControlInterrupt (pDDB, LSA_TRUE, EDDP_INT_PPM_HOL_NOT_EMPTY_IFA);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTPpmNotFinishedTimerPassed                           +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: This function is called by a TimerRequest if the "PpmNotFinishedTimer"    +*/
/*+               has passed.                                                               +*/
/*+                                                                                         +*/
/*+               Activate the PPM_not_finished interrupt.                                  +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTPpmNotFinishedTimerPassed(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTPpmNotFinishedTimerPassed(): Activate the ISR_Ppm_not_finished");

    // activate this interrupt
    EDDP_CoreISRControlInterrupt (pDDB, LSA_TRUE, EDDP_INT_PPM_NOT_FINISHED_IFA);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRT_ISR_DelayStatus                    +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32                     EventNr      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  EventNr    : Event Number (Interrupt Number)                     (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function is called by CORE from the                  +*/
/*+               interrupt "INT_IRT_DelayStatus".                          +*/
/*+               It is executed within LOW context.                        +*/
/*+                                                                         +*/
/*+               Register "PNIP_REG_IRQxEVENT_LOW_0"                       +*/
/*+               Interrupt event "INT_IRT_DelayStatus"                     +*/
/*+                 -> Bit(18): 0x00040000                                  +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRT_ISR_DelayStatus(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr)
{
    LSA_UINT32          HwPortID;
    LSA_UINT32          reg_address_0;
    LSA_UINT32          reg_address_1;
    LSA_UINT32          RegDelayStatus_0;
    LSA_UINT32          RegDelayStatus_1;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(EventNr);
#endif

    for ( HwPortID=1; HwPortID <= pDDB->SWI.PortCnt; HwPortID++ )
    { 
        reg_address_0 = (HwPortID==1 ? PNIP_REG_IRT_DELAYSTATUS_0_P1 : 
                        (HwPortID==2 ? PNIP_REG_IRT_DELAYSTATUS_0_P2 : 
                        (HwPortID==3 ? PNIP_REG_IRT_DELAYSTATUS_0_P3 : 
                        (HwPortID==4 ? PNIP_REG_IRT_DELAYSTATUS_0_P4 : 0))));
        
        reg_address_1 = (HwPortID==1 ? PNIP_REG_IRT_DELAYSTATUS_1_P1 : 
                        (HwPortID==2 ? PNIP_REG_IRT_DELAYSTATUS_1_P2 : 
                        (HwPortID==3 ? PNIP_REG_IRT_DELAYSTATUS_1_P3 : 
                        (HwPortID==4 ? PNIP_REG_IRT_DELAYSTATUS_1_P4 : 0))));
        
        RegDelayStatus_0 = EDDP_HAL_REG32_READ(pDDB, reg_address_0);
        RegDelayStatus_1 = EDDP_HAL_REG32_READ(pDDB, reg_address_1);
        
        EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H:--] EDDP_CRT_ISR_DelayStatus(EventNr=0x%X): HwPortID=0x%X, RegDelayStatus_0=0x%X, RegDelayStatus_1=0x%X", 
           EventNr, HwPortID,
           RegDelayStatus_0, RegDelayStatus_1);
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRT_ISR_PhaseCrossed                   +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32                     EventNr      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  EventNr    : Event Number (Interrupt Number)                     (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function is called by CORE from the                  +*/
/*+               interrupt "INT_PhaseCrossed".                             +*/
/*+               It is executed within LOW context.                        +*/
/*+                                                                         +*/
/*+               Register "PNIP_REG_IRQxEVENT_LOW_0"                       +*/
/*+               Interrupt event "INT_PhaseCrossed"                        +*/
/*+                 -> Bit(21): 0x00200000                                  +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRT_ISR_PhaseCrossed(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr)
{
    LSA_UINT32          RegPhaseCrossed;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(EventNr);
#endif

    RegPhaseCrossed = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_PHASECROSSED);

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRT_ISR_PhaseCrossed(EventNr=0x%X): ERROR -> RegPhaseCrossed=0x%X", 
        EventNr, RegPhaseCrossed);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRT_ISR_InBound_Late_ERR               +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32                     EventNr      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  EventNr    : Event Number (Interrupt Number)                     (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function is called by CORE from the                  +*/
/*+               interrupt "INT_InBound_Late_ERR".                         +*/
/*+               It is executed within LOW context.                        +*/
/*+                                                                         +*/
/*+               Register "PNIP_REG_IRQxEVENT_LOW_0"                       +*/
/*+               Interrupt event "INT_InBound_Late_ERR"                    +*/
/*+                 -> Bit(31): 0x80000000                                  +*/
/*+                                                                         +*/
/*+               Register "InBound_Late_Err_Status" for PackGrp(x):        +*/
/*+                 0: no error                                             +*/
/*+                 1: InBound-Late-Error detected by HW                    +*/
/*+                                                                         +*/
/*+               Register "InBound_Late_Err_Mask" for PackGrp(x):          +*/
/*+                 0: INT_InBound_Late_Err enable                          +*/
/*+                 1: INT_InBound_Late_Err disable                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRT_ISR_InBound_Late_ERR(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr)
{
    LSA_UINT32                              i;
    LSA_UINT8                               MaxPackGrp;
    LSA_UINT32                              RegInBoundLateErrMask, RegInBoundLateErrStatus;
    LSA_UINT8                               InboundActiveMask;
    LSA_UINT32                              PackGrpLoopMask;
    EDDP_CRT_INBOUND_MNGM_PTR_TYPE      pInboundMgmt;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(EventNr);
#endif

    pInboundMgmt            = &pDDB->CRT.Inbound;
    RegInBoundLateErrMask   = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_INBOUND_LATE_ERR_MASK);
    RegInBoundLateErrStatus = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_INBOUND_LATE_ERR_STATUS);
    InboundActiveMask       = pInboundMgmt->InboundActiveMask;
    PackGrpLoopMask         = 1;
    MaxPackGrp              = 0;

    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CRT_ISR_InBound_Late_ERR()");

    EDDP_CRT_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRT_ISR_InBound_Late_ERR(EventNr=0x%X): ERROR -> RegLateErrMask=0x%X, RegLateErrStatus=0x%X, InboundUsedMask=0x%X, InboundActiveMask=0x%X", 
        EventNr, RegInBoundLateErrMask, RegInBoundLateErrStatus, pInboundMgmt->InboundUsedMask[0], InboundActiveMask);

    /* ------------------------------------------------------------------------------------------------ */
    /* Do something only if at least:                                                                   */
    /*  - one active PackGrp is present                                                                 */
    /*  - one LateError in register "Late_Err_Status" is present                                        */
    /* ------------------------------------------------------------------------------------------------ */
    if ((InboundActiveMask != 0) && (RegInBoundLateErrStatus != 0))
    {
        /* ------------------------------------------------ */
        /* Init MaxPackGrp                                  */
        /* ------------------------------------------------ */
        switch (pDDB->Hardware)
        {
            case EDDP_HW_ERTEC200P:
            case EDDP_HW_ERTEC200P_FPGA:
            case EDDP_HW_HERA_FPGA:    
            {
                MaxPackGrp = EDDP_CRT_MAX_PACK_FRAMES_ERTEC200P;
            }
            break;

            default:
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRT_ISR_InBound_Late_ERR(): FATAL -> Hardware(0x%X) is not supported!", 
                    pDDB->Hardware);
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            }
            break;
        }

        /* ------------------------------------------------ */
        /* Look for PackGrp that:                           */
        /*  - is active                                     */
        /*  - has InBound-LateError                         */
        /*  - LateError interrupt is enable                 */
        /* ------------------------------------------------ */
        for ( i=0; i < MaxPackGrp; i++ )
        {
            if (   (InboundActiveMask & PackGrpLoopMask)            /* PackGrp is active */
                && (RegInBoundLateErrStatus & PackGrpLoopMask)      /* Late_Err_Status is set */
                && (!(RegInBoundLateErrMask & PackGrpLoopMask))     /* Late_Err_Mask is not set */
               )
            {
                // this PackGrp must be connected to PRM -> PDIRData.PDIRFrameData[x]
                EDDP_ASSERT_FALSE(pInboundMgmt->InboundGrps[i].LateErrDiag.PrmPDIRFrameDataIdx == EDDP_CRT_CHAIN_IDX_END);

                // disable interrupt "ISR_InBound_Late_ERR" of this PackGrp
                EDDP_CRTLateErrorSetupLateErrMask (pDDB, (LSA_UINT8)i, LSA_TRUE);
                // delete this PackGrp in register "Late_Err_Status"
                EDDP_CRTLateErrorDeleteLateErrStatus (pDDB, (LSA_UINT8)i);
                // trigger LateError diagnosis (Appear)
                EDDP_PRMDiagTriggerLateErrorDiagSingle (pDDB, pInboundMgmt->InboundGrps[i].LateErrDiag.PrmPDIRFrameDataIdx, LSA_TRUE, LSA_FALSE, LSA_FALSE);
                // start timer of this PackGrp
                EDDP_CRTLateErrorTimerStartPackGrp (pDDB, (LSA_UINT8)i, EDDP_CRT_INBOUND_LATE_ERR_TIMER_TYPE_PACKGRP_LATE_ERROR);
            }

            // next PackGrp in loop
            EDDP_SHIFT_VALUE_LEFT(PackGrpLoopMask, 1);
        }
    }

    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRT_ISR_InBound_Late_ERR()");
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRT_ISR_Pack_ERR                       +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32                     EventNr      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  EventNr    : Event Number (Interrupt Number)                     (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function is called by CORE from the                  +*/
/*+               PPM error interrupt "INT_Pack_ERR".                       +*/
/*+               It is executed within LOW context.                        +*/
/*+                                                                         +*/
/*+               Register "PNIP_REG_IRQxEVENT_LOW_0"                       +*/
/*+               Interrupt event "EDDP_INT_PACK_ERR"                       +*/
/*+                 -> Bit(17): 0x00020000                                  +*/
/*+                                                                         +*/
/*+               Get info of register                                      +*/
/*+                 - PNIP_REG_PACK_ERR_STATUS                              +*/
/*+                                                                         +*/
/*+               Error handling:                                           +*/
/*+               OutBound_Packgroup_Err    INT_Pack_Param_ERR      Error   +*/
/*+               InBound_Packgroup_Err     INT_Pack_Param_ERR      Error   +*/
/*+               InBound_Receive_Err       INT_Pack_Receive_ERR	Error   +*/
/*+               InBound_Send_Err          INT_Pack_Send_ERR       Error   +*/
/*+               else                      INT_Pack_APIwrite_ERR   Error   +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRT_ISR_Pack_ERR(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr)
{
    LSA_UINT32          RegPackErrStatus;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(EventNr);
#endif

    RegPackErrStatus = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_PACK_ERR_STATUS);

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRT_ISR_Pack_ERR(EventNr=0x%X): ERROR -> RegPackErrStatus=0x%X", 
        EventNr, RegPackErrStatus);

    /* ---------------------------------------------------------------- */
    /* INT_Pack_Param_ERR                                               */
    /* OutBound_Packgroup_Err   : RegPackErrStatus-Bit(3)=1 is error    */
    /* InBound_Packgroup_Err    : RegPackErrStatus-Bit(7)=1 is error    */
    /* ---------------------------------------------------------------- */
    if ((RegPackErrStatus & 0x8) || (RegPackErrStatus & 0x80))
    {
        // do not delete the empty spaces in this trace text
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--]    INT_Pack_Param_ERR(): ERROR");
    }

    /* ---------------------------------------------------------------- */
    /* INT_Pack_Receive_ERR                                             */
    /* InBound_Receive_Err      : RegPackErrStatus-Bit(15)=1 is error   */
    /* ---------------------------------------------------------------- */
    if (RegPackErrStatus & 0x8000)
    {
        // do not delete the empty spaces in this trace text
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--]    INT_Pack_Receive_ERR(): ERROR");
    }

    /* ---------------------------------------------------------------- */
    /* INT_Pack_Send_ERR                                                */
    /* InBound_Send_Err         : RegPackErrStatus-Bit(23)=1 is error   */
    /* ---------------------------------------------------------------- */
    if (RegPackErrStatus & 0x800000)
    {
        // do not delete the empty spaces in this trace text
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--]    INT_Pack_Send_ERR(): ERROR");
    }
}
