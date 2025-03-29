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
/*  F i l e               &F: eddp_hal_isr.c                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP HAL (Hardware abstraction Layer) for ISR    */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  06.11.08    SJ    initial version.                                       */
/*  13.01.11    AM    added EDDP_HALIsrGetGroupMsk(), EDDP_HALGetClockA()    */
#endif
/*****************************************************************************/



/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/

#define LTRC_ACT_MODUL_ID  42
#define EDDP_MODULE_ID     LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_HAL_ISR */

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

typedef struct _EDDP_HAL_ISR_GRP_REGISTER_MAPPING_TYPE
{
    LSA_UINT32                      IRQEventLow;
	LSA_UINT32                      IRQEventMid;
	LSA_UINT32                      IRQEventHigh;
	LSA_UINT32                      IRQMaskLow;
	LSA_UINT32                      IRQMaskMid;
	LSA_UINT32                      IRQMaskHigh;
	LSA_UINT32                      IRQLow;
	LSA_UINT32                      IRQMid;
	LSA_UINT32                      IRQHigh;
	LSA_UINT32                      IRQAckLow;
	LSA_UINT32                      IRQAckMid;
	LSA_UINT32                      IRQAckHigh;
	LSA_UINT32                      IRQEOI;
} EDDP_HAL_ISR_GRP_REGISTER_MAPPING_TYPE;


/*===========================================================================*/
/*                                Constants                                  */
/*===========================================================================*/

/* -------------------------------------------------------------------------*/
/* Subsystemdependend Register mapping of Group-Interrupthandling           */
/* -------------------------------------------------------------------------*/

EDDP_HAL_ISR_GRP_REGISTER_MAPPING_TYPE EDDP_HalIsrGrpRegSubSys[EDDP_HAL_ISR_SUBSYS_CNT] = 
{
	{
		PNIP_REG_IRQ1EVENT_LOW_0,
		PNIP_REG_IRQ1EVENT_MID_0,
		PNIP_REG_IRQ1EVENT_HIGH_0,
		PNIP_REG_IRQ1MASK_LOW_0,
		PNIP_REG_IRQ1MASK_MID_0,
		PNIP_REG_IRQ1MASK_HIGH_0,
		PNIP_REG_IRQ1_LOW_0,
		PNIP_REG_IRQ1_MID_0,
		PNIP_REG_IRQ1_HIGH_0,
		PNIP_REG_IRQ1ACK_LOW_0,
		PNIP_REG_IRQ1ACK_MID_0,
		PNIP_REG_IRQ1ACK_HIGH_0,
		PNIP_REG_IRQ1_EOI_0,
	},
	{
		PNIP_REG_IRQ2EVENT_LOW_0,
		PNIP_REG_IRQ2EVENT_MID_0,
		PNIP_REG_IRQ2EVENT_HIGH_0,
		PNIP_REG_IRQ2MASK_LOW_0,
		PNIP_REG_IRQ2MASK_MID_0,
		PNIP_REG_IRQ2MASK_HIGH_0,
		PNIP_REG_IRQ2_LOW_0,
		PNIP_REG_IRQ2_MID_0,
		PNIP_REG_IRQ2_HIGH_0,
		PNIP_REG_IRQ2ACK_LOW_0,
		PNIP_REG_IRQ2ACK_MID_0,
		PNIP_REG_IRQ2ACK_HIGH_0,
		PNIP_REG_IRQ2_EOI_0,
	},
	{
		PNIP_REG_IRQ3EVENT_LOW_0,
		PNIP_REG_IRQ3EVENT_MID_0,
		PNIP_REG_IRQ3EVENT_HIGH_0,
		PNIP_REG_IRQ3MASK_LOW_0,
		PNIP_REG_IRQ3MASK_MID_0,
		PNIP_REG_IRQ3MASK_HIGH_0,
		PNIP_REG_IRQ3_LOW_0,
		PNIP_REG_IRQ3_MID_0,
		PNIP_REG_IRQ3_HIGH_0,
		PNIP_REG_IRQ3ACK_LOW_0,
		PNIP_REG_IRQ3ACK_MID_0,
		PNIP_REG_IRQ3ACK_HIGH_0,
		PNIP_REG_IRQ3_EOI_0,
	}
};

/* -------------------------------------------------------------------------*/
/* Subsystemdependend Register mapping of Single-Interrupthandling  (MUX)   */
/* -------------------------------------------------------------------------*/

const EDDP_HAL_ISR_MUX_REGISTER_MAPPING_TYPE EDDP_HalIsrMUXRegSubSys[EDDP_HAL_ISR_SUBSYS_CNT] = 
{
	{
		PNIP_REG_IRQ1_ACTIVATE,
		{	PNIP_REG_IRQ1CONTROL_MUX1,
			PNIP_REG_IRQ1CONTROL_MUX2,
			0, /* not present !*/
			0, /* not present !*/
			0, /* not present !*/
			0, /* not present !*/
			0, /* not present !*/
			0, /* not present !*/
			0, /* not present !*/
			0, /* not present !*/
			0, /* not present !*/
			0, /* not present !*/
			0, /* not present !*/
			0, /* not present !*/
			0, /* not present !*/
		}
	},
	{
		PNIP_REG_IRQ2_ACTIVATE,
		{	PNIP_REG_IRQ2CONTROL_MUX2,
			PNIP_REG_IRQ2CONTROL_MUX3,
			PNIP_REG_IRQ2CONTROL_MUX4,
			PNIP_REG_IRQ2CONTROL_MUX5,
			PNIP_REG_IRQ2CONTROL_MUX6,
			PNIP_REG_IRQ2CONTROL_MUX7,
			PNIP_REG_IRQ2CONTROL_MUX8,
			PNIP_REG_IRQ2CONTROL_MUX9,
			PNIP_REG_IRQ2CONTROL_MUX10,
			PNIP_REG_IRQ2CONTROL_MUX11,
			PNIP_REG_IRQ2CONTROL_MUX12,
			PNIP_REG_IRQ2CONTROL_MUX13,
			PNIP_REG_IRQ2CONTROL_MUX14,
			PNIP_REG_IRQ2CONTROL_MUX15,
		}
	},
	{
		PNIP_REG_IRQ3_ACTIVATE,
		{	PNIP_REG_IRQ3CONTROL_MUX2,
			PNIP_REG_IRQ3CONTROL_MUX3,
			PNIP_REG_IRQ3CONTROL_MUX4,
			PNIP_REG_IRQ3CONTROL_MUX5,
			PNIP_REG_IRQ3CONTROL_MUX6,
			PNIP_REG_IRQ3CONTROL_MUX7,
			PNIP_REG_IRQ3CONTROL_MUX8,
			PNIP_REG_IRQ3CONTROL_MUX9,
			PNIP_REG_IRQ3CONTROL_MUX10,
			PNIP_REG_IRQ3CONTROL_MUX11,
			PNIP_REG_IRQ3CONTROL_MUX12,
			PNIP_REG_IRQ3CONTROL_MUX13,
			PNIP_REG_IRQ3CONTROL_MUX14,
			PNIP_REG_IRQ3CONTROL_MUX15,
		}
	}
};


/*===========================================================================*/
/*                                 Macros                                    */
/*===========================================================================*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_HALIsrInit                             +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB          +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Inits Interrupthandling within HAL. Called on DeviceSetup +*/
/*+                                                                         +*/
/*+               The CPU Subsystem used depends on EDDPLocation from DPB   +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_HALIsrInit(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDDP_HAL_ISR_GRP_REGS_TYPE      Events;

    /* ----------------------------------------------------------------------*/
    /* Setup the CPUSubsystem to use with Interrupthandling                  */
    /* ----------------------------------------------------------------------*/
	switch (pDDB->EDDPLocation)
    {
		case EDDP_LOCATION_EXT_HOST:
			pDDB->Core.Hal.Isr.CPUSubys = EDDP_HAL_ISR_SUBSYS_EXT_HOST;
		break;

        case EDDP_LOCATION_LOCAL:
			pDDB->Core.Hal.Isr.CPUSubys = EDDP_HAL_ISR_SUBSYS_LOCAL;
		break;

        default:
        {
       	   	EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALIsrInit(): FATAL -> Invalid EDDPLocation(%d)!",
                pDDB->EDDPLocation);
           	EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
        }
		break;
	}

    /* ----------------------------------------------------------------------*/
    /* ACK all pending Events (maybe set from previous Setup?)               */
    /* ----------------------------------------------------------------------*/
	Events.Low  = 0xFFFFFFFFl;
	Events.Mid  = 0xFFFFFFFFl;
	Events.High = 0xFFFFFFFFl;

	EDDP_HALIsrAckGroupIRQ (pDDB, &Events);

	/* TBD:							*/
	/* setup IRQ-Level !? Registers */
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_HALIsrGetGroupMsk                      +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+                             EDDP_HAL_ISR_GRP_REGS_PTR_TYPE   pEvents    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Get Mask for GroupInterrupt (low/mid/high)                +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_HALIsrGetGroupMsk(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_HAL_ISR_GRP_REGS_PTR_TYPE          pEvents)
{
    EDDP_LOWER_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] IN: EDDP_HALIsrGetGroupMsk()");

    pEvents->Low = EDDP_HAL_REG32_READ(pDDB,
		                 EDDP_HalIsrGrpRegSubSys[pDDB->Core.Hal.Isr.CPUSubys].IRQMaskLow);

	pEvents->Mid = EDDP_HAL_REG32_READ(pDDB,
		                 EDDP_HalIsrGrpRegSubSys[pDDB->Core.Hal.Isr.CPUSubys].IRQMaskMid);

	pEvents->High = EDDP_HAL_REG32_READ(pDDB,
		                 EDDP_HalIsrGrpRegSubSys[pDDB->Core.Hal.Isr.CPUSubys].IRQMaskHigh);

    EDDP_LOWER_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] OUT: EDDP_HALIsrGetGroupMsk(L=0x%X, M=0x%X, H=0x%X)",
		pEvents->Low, pEvents->Mid, pEvents->High);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_HALIsrSetGroupMsk                      +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+                             EDDP_HAL_ISR_GRP_REGS_PTR_TYPE   pEvents    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Set Mask for GroupInterrupt (low/mid/high)                +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_HALIsrSetGroupMsk(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_HAL_ISR_GRP_REGS_PTR_TYPE          pEvents)
{
    EDDP_LOWER_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] IN: EDDP_HALIsrSetGroupMsk (L=0x%X, M=0x%X, H=0x%X)",
		pEvents->Low, pEvents->Mid, pEvents->High);

	EDDP_HAL_REG32_WRITE(pDDB,
		                 EDDP_HalIsrGrpRegSubSys[pDDB->Core.Hal.Isr.CPUSubys].IRQMaskLow,
						 pEvents->Low);

	EDDP_HAL_REG32_WRITE(pDDB,
		                 EDDP_HalIsrGrpRegSubSys[pDDB->Core.Hal.Isr.CPUSubys].IRQMaskMid,
						 pEvents->Mid);

	EDDP_HAL_REG32_WRITE(pDDB,
		                 EDDP_HalIsrGrpRegSubSys[pDDB->Core.Hal.Isr.CPUSubys].IRQMaskHigh,
						 pEvents->High);

    EDDP_LOWER_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] OUT: EDDP_HALIsrSetGroupMsk()");
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_HALIsrAckGroupIRQ                      +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+                             EDDP_HAL_ISR_GRP_REGS_PTR_TYPE   pEvents    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Acknoledge group interrupts                               +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_HALIsrAckGroupIRQ(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_HAL_ISR_GRP_REGS_PTR_TYPE          pEvents)
{
    EDDP_LOWER_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] IN: EDDP_HALIsrAckGroupIRQ(L=0x%X, M=0x%X, H=0x%X)",
		pEvents->Low, pEvents->Mid, pEvents->High);

	EDDP_HAL_REG32_WRITE(pDDB,
		                EDDP_HalIsrGrpRegSubSys[pDDB->Core.Hal.Isr.CPUSubys].IRQAckLow,
						pEvents->Low);

	EDDP_HAL_REG32_WRITE(pDDB,
		                EDDP_HalIsrGrpRegSubSys[pDDB->Core.Hal.Isr.CPUSubys].IRQAckMid,
						pEvents->Mid);

	EDDP_HAL_REG32_WRITE(pDDB,
		                EDDP_HalIsrGrpRegSubSys[pDDB->Core.Hal.Isr.CPUSubys].IRQAckHigh,
						pEvents->High);

    EDDP_LOWER_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] OUT: EDDP_HALIsrAckGroupIRQ()");
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_HALIsrGetGroupIRQ                      +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+                             EDDP_HAL_ISR_GRP_REGS_PTR_TYPE   pEvents    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Gets pending group IRQs                                   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_HALIsrGetGroupIRQ(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_HAL_ISR_GRP_REGS_PTR_TYPE          pEvents)
{
    EDDP_LOWER_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] IN: EDDP_HALIsrGetGroupIRQ()");

	pEvents->Low  = EDDP_HAL_REG32_READ(pDDB,
		                 EDDP_HalIsrGrpRegSubSys[pDDB->Core.Hal.Isr.CPUSubys].IRQLow);

	pEvents->Mid  = EDDP_HAL_REG32_READ(pDDB,
		                 EDDP_HalIsrGrpRegSubSys[pDDB->Core.Hal.Isr.CPUSubys].IRQMid);

	pEvents->High = EDDP_HAL_REG32_READ(pDDB,
		                 EDDP_HalIsrGrpRegSubSys[pDDB->Core.Hal.Isr.CPUSubys].IRQHigh);

    EDDP_LOWER_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] OUT: EDDP_HALIsrGetGroupIRQ(L=0x%X, M=0x%X, H=0x%X)",
		pEvents->Low, pEvents->Mid, pEvents->High);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_HALIsrGetEvents                        +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+                             EDDP_HAL_ISR_GRP_REGS_PTR_TYPE   pEvents    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Gets pending Interrupt Events from Event-Registers        +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_HALIsrGetEvents(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_HAL_ISR_GRP_REGS_PTR_TYPE          pEvents)
{
    EDDP_LOWER_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] IN: EDDP_HALIsrGetEvents()");

	pEvents->Low  = EDDP_HAL_REG32_READ(pDDB,
		                 EDDP_HalIsrGrpRegSubSys[pDDB->Core.Hal.Isr.CPUSubys].IRQEventLow);

	pEvents->Mid  = EDDP_HAL_REG32_READ(pDDB,
		                 EDDP_HalIsrGrpRegSubSys[pDDB->Core.Hal.Isr.CPUSubys].IRQEventMid);

	pEvents->High = EDDP_HAL_REG32_READ(pDDB,
		                 EDDP_HalIsrGrpRegSubSys[pDDB->Core.Hal.Isr.CPUSubys].IRQEventHigh);


	EDDP_HAL_REG32_WRITE(pDDB, EDDP_HalIsrGrpRegSubSys[pDDB->Core.Hal.Isr.CPUSubys].IRQEventLow, 0U);

	EDDP_HAL_REG32_WRITE(pDDB, EDDP_HalIsrGrpRegSubSys[pDDB->Core.Hal.Isr.CPUSubys].IRQEventMid, 0U);

	EDDP_HAL_REG32_WRITE(pDDB, EDDP_HalIsrGrpRegSubSys[pDDB->Core.Hal.Isr.CPUSubys].IRQEventHigh, 0U);

    EDDP_LOWER_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] OUT: EDDP_HALIsrGetEvents (L=0x%X, M=0x%X, H=0x%X)",
		pEvents->Low, pEvents->Mid, pEvents->High);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_HALIsrGetSingleOffsets                 +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+                             ...                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Gets register offsets for fast checking and acknowleding  +*/
/*+               of single interrupts                                      +*/
/*+  Function can be called from DEV_OPEN !                                 +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#if defined  (EDDP_CFG_ISR_POLLING_MODE)    
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_HALIsrGetSingleOffsets(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    LSA_UINT32                              EventNr,
    LSA_UINT32                            * pAckRegOffset,
    LSA_UINT32                            * pIsrRegOffset,
    LSA_UINT32                            * pMsk)
{
    EDDP_LOWER_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] IN: EDDP_HALIsrGetSingleOffsets()");
    LSA_UINT32  CPUSubys = 0;

    if (EDDP_LOCATION_EXT_HOST == pDDB->EDDPLocation)
    {
        CPUSubys = EDDP_HAL_ISR_SUBSYS_EXT_HOST;
    }
    else if (EDDP_LOCATION_LOCAL == pDDB->EDDPLocation)
    {
        CPUSubys = EDDP_HAL_ISR_SUBSYS_LOCAL;
    }
    else
    {
        EDDP_LOWER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HALIsrGetSingleOffsets(): FATAL -> Invalid EDDPLocation(%d)!",
            pDDB->EDDPLocation);
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
        return; 
    }

    if (EventNr >= EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS) /* High */
    {
        *pMsk = (LSA_UINT32)1 << (EventNr - (EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS));
        *pAckRegOffset = EDDP_HalIsrGrpRegSubSys[CPUSubys].IRQAckHigh;
        *pIsrRegOffset = EDDP_HalIsrGrpRegSubSys[CPUSubys].IRQEventHigh;
    }
    else if (EventNr >= EDDP_ISR_LOW_MAX_EVENTS) /* Mid */
    {
        *pMsk  = (LSA_UINT32)1 << (EventNr - (EDDP_ISR_LOW_MAX_EVENTS));
        *pAckRegOffset = EDDP_HalIsrGrpRegSubSys[CPUSubys].IRQAckMid;
        *pIsrRegOffset = EDDP_HalIsrGrpRegSubSys[CPUSubys].IRQEventMid;
    }
    else /* Low */
    {
        *pMsk  = (LSA_UINT32)1 << EventNr;
        *pAckRegOffset = EDDP_HalIsrGrpRegSubSys[CPUSubys].IRQAckLow;
        *pIsrRegOffset = EDDP_HalIsrGrpRegSubSys[CPUSubys].IRQEventLow;
    }

    EDDP_LOWER_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] OUT: EDDP_HALIsrGetSingleOffsets (Msk=0x%X, AckRegOffset=0x%X, IsrRegOffset=0x%X)",
        *pMsk, *pAckRegOffset, *pIsrRegOffset);
}
#endif //(EDDP_CFG_ISR_POLLING_MODE)    


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_HALGetClockA                           +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                (in)  +*/
/*+                                                                         +*/
/*+  returns 64 bit Clock A timestamp                                       +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Return Clock A timestamp                                  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_UINT64 EDDP_LOCAL_FCT_ATTR EDDP_HALGetClockA(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB)
{
	LSA_UINT32 highTime;
	LSA_UINT32 lowTime;
	LSA_UINT64 Time;

	/* first read high 32bit part of clock... */
	highTime = EDDP_HAL_REG32_READ(pDDB,PNIP_REG_OCTHIGH_CLOCK_A);
	/* ...then read low 32bit part of clock... */
	lowTime = EDDP_HAL_REG32_READ(pDDB,PNIP_REG_OCTLOW_CLOCK_A);
	/* ...and read again high 32bit part of clock... */
	Time = EDDP_HAL_REG32_READ(pDDB,PNIP_REG_OCTHIGH_CLOCK_A);
	/* ...to detect a possible wrap around! */
	if((LSA_UINT32)Time != highTime)
	{
		/* if wrap around occurred, read low 32bit part of clock again! */
		lowTime = EDDP_HAL_REG32_READ(pDDB,PNIP_REG_OCTLOW_CLOCK_A);
	}

	/* now store clock into 64bit value */
	Time <<=32;
	Time |= lowTime;

	return Time;
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_HALIsrGroupEOI                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Issue EOI for group interrupts                            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_HALIsrGroupEOI(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB)
{

	EDDP_HAL_REG32_WRITE(pDDB,
		                EDDP_HalIsrGrpRegSubSys[pDDB->Core.Hal.Isr.CPUSubys].IRQEOI,
                        pDDB->Core.Hal.Isr.EOI_WaitTime);

#if defined(EDDP_CFG_USE_ISR_EXTRA_TRACE)
    EDDP_LOWER_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] EDDP_HALIsrGroupEOI (pDDB: 0x%X, EOI written: %X, read again: %X)",
		pDDB, pDDB->Core.Hal.Isr.EOI_WaitTime, 	EDDP_HAL_REG32_READ(pDDB, EDDP_HalIsrGrpRegSubSys[pDDB->Core.Hal.Isr.CPUSubys].IRQEOI));
#endif /* defined(EDDP_CFG_USE_ISR_EXTRA_TRACE) */

}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_HALIsrEnableMUXIRQ                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+                             LSA_UINT32                       IRQNr      +*/
/*+                             LSA_UINT32                       EventNr    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                (in)  +*/
/*+  IRQNr         : Number of the IRQ to use (2..15)                       +*/
/*+  EventNr       : EventNr to set (0..127) 127 = OFF                      +*/
/*+  IRQWaitTime   : WaitTime in 32.768 (0..511)                            +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Enables a single interrupt within IRQxControl_MUXn        +*/
/*+               by setting the EventNr.                                   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_HALIsrEnableMUXIRQ(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
	LSA_UINT32                              IRQNr,
	LSA_UINT32                              EventNr,
    LSA_UINT32                              IRQWaitTime)
{
    LSA_UINT32  MuxReg = 0x0;
    LSA_UNUSED_ARG(IRQWaitTime);

	EDDP_ASSERT_FALSE(IRQNr > EDDP_HAL_ISR_MUX_CNT);

    /* Note: We take the Masks/Shift values from IRQ1 MUX1 because its the */
    /* same for all registers                                              */ 

    EDDP_SET_BIT_VALUE32( MuxReg, 
                          EventNr, 
                          PNIP_REG_IRQ1CONTROL_MUX1__MSK_IRQ_SELECTMUX, 
                          PNIP_REG_IRQ1CONTROL_MUX1_SHFT_IRQ_SELECTMUX);

	EDDP_HAL_REG32_WRITE(pDDB,
		                EDDP_HalIsrMUXRegSubSys[pDDB->Core.Hal.Isr.CPUSubys].IRQControlMux[IRQNr-2],
						MuxReg);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_HALIsrRetriggerMUXIRQ_WaitTime         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+                             LSA_UINT32                       IRQNr      +*/
/*+                             LSA_UINT32                       EventNr    +*/
/*+                             LSA_UINT32                       IRQWaitTime+*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                (in)  +*/
/*+  IRQNr         : Number of the IRQ to use (2..15)                       +*/
/*+  EventNr       : EventNr to set (0..127) 127 = OFF                      +*/
/*+  IRQWaitTime   : WaitTime in 32.768 (0..511)                            +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function retriggers the IRQ_WaitTime for single      +*/
/*+               interrupts (but only if the EventNr is acitve for this    +*/
/*+               IRQNr.                                                    +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_HALIsrRetriggerMUXIRQ_WaitTime(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
	LSA_UINT32                              IRQNr,
	LSA_UINT32                              EventNr,
	LSA_UINT32                              IRQWaitTime)
{
    LSA_UINT32  MuxReg = 0;
    LSA_UINT32  IRQ_SelectMUX;

    EDDP_LOWER_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] IN: EDDP_HALIsrRetriggerMUXIRQ_WaitTime(IRQNr=0x%X, EventNr=0x%X, IRQWaitTime=%u)",
		IRQNr, EventNr, IRQWaitTime);

    EDDP_ASSERT_FALSE(IRQNr > EDDP_HAL_ISR_MUX_CNT);

    /* Note: We take the Masks/Shift values from IRQ1 MUX1 because its the */
    /* same for all registers                                              */
   
    /* Read actual value */   
    MuxReg = EDDP_HAL_REG32_READ(pDDB, EDDP_HalIsrMUXRegSubSys[pDDB->Core.Hal.Isr.CPUSubys].IRQControlMux[IRQNr-2]);

    /* get actual IRQ_SelectMUX */
    IRQ_SelectMUX = ( MuxReg  & PNIP_REG_IRQ1CONTROL_MUX1__MSK_IRQ_SELECTMUX ) >> PNIP_REG_IRQ1CONTROL_MUX1_SHFT_IRQ_SELECTMUX;

    /* check if single Interrupt is not disabled and the right one */
    if (IRQ_SelectMUX == EventNr)
    {
        MuxReg = (MuxReg & PNIP_REG_IRQ1CONTROL_MUX1__MSK_IRQ_WAITTIME) >> PNIP_REG_IRQ1CONTROL_MUX1_SHFT_IRQ_WAITTIME;

        if (MuxReg == 0)
        {
            EDDP_LOWER_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN,"[H:--] EDDP_HALIsrRetriggerMUXIRQ_WaitTime(): Remaining IRQ_WaitTime before setting new value == 0x0!");
        }

        MuxReg = 0x0;

        EDDP_SET_BIT_VALUE32( MuxReg, 
                              IRQWaitTime, 
                              PNIP_REG_IRQ1CONTROL_MUX1__MSK_IRQ_WAITTIME, 
                              PNIP_REG_IRQ1CONTROL_MUX1_SHFT_IRQ_WAITTIME);

        EDDP_SET_BIT_VALUE32( MuxReg, 
                              EventNr, 
                              PNIP_REG_IRQ1CONTROL_MUX1__MSK_IRQ_SELECTMUX, 
                              PNIP_REG_IRQ1CONTROL_MUX1_SHFT_IRQ_SELECTMUX);

	    EDDP_HAL_REG32_WRITE( pDDB,
		                      EDDP_HalIsrMUXRegSubSys[pDDB->Core.Hal.Isr.CPUSubys].IRQControlMux[IRQNr-2],
			    			  MuxReg);
    }
    else
    {
        EDDP_LOWER_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH,"[H:--] EDDP_HALIsrRetriggerMUXIRQ_WaitTime(): Single Interrupt disabled or wrong. don�t retrigger WaitTime (0x%X,0x%X)!",IRQ_SelectMUX,EventNr);
    }    
  						  
    EDDP_LOWER_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] OUT: EDDP_HALIsrRetriggerMUXIRQ_WaitTime()");
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_HALIsrDisableMUXIRQ                    +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+                             LSA_UINT32                       IRQNr      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                (in)  +*/
/*+  IRQNr         : Number of the IRQ to use (2..15)                       +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Disables a single interrupt within IRQxControl_MUXn       +*/
/*+               by setting the EventNr to 127.                            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_HALIsrDisableMUXIRQ(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
	LSA_UINT32                              IRQNr)
{
	#define  EDDP_HAL_MUX_IRQSelect_Disabled  0x7F

    LSA_UINT32      MuxReg = 0;

    EDDP_LOWER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] IN: EDDP_HALIsrDisableMUXIRQ(IRQNr=0x%X)", IRQNr);

    EDDP_ASSERT_FALSE(IRQNr > EDDP_HAL_ISR_MUX_CNT);

    /* Note: We take the Masks/Shift values from IRQ1 MUX1 because its the */
    /* same for all registers                                              */

    EDDP_SET_BIT_VALUE32( MuxReg, 
                          EDDP_HAL_MUX_IRQSelect_Disabled, 
                          PNIP_REG_IRQ1CONTROL_MUX1__MSK_IRQ_SELECTMUX, 
                          PNIP_REG_IRQ1CONTROL_MUX1_SHFT_IRQ_SELECTMUX);

	EDDP_HAL_REG32_WRITE(pDDB,
		                EDDP_HalIsrMUXRegSubSys[pDDB->Core.Hal.Isr.CPUSubys].IRQControlMux[IRQNr-2],
						MuxReg);

    EDDP_LOWER_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] OUT: EDDP_HALIsrDisableMUXIRQ()");
}

/*****************************************************************************/
/*  end of file EDDP_HAL_isr.c                                               */
/*****************************************************************************/
