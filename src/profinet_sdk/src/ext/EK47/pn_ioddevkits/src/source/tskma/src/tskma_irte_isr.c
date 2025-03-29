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
/*  F i l e               &F: tskma_irte_isr.c                          :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*   TSKMA interrupt service routines for IRTE/PERI							 */
/*                                                                           */
/*****************************************************************************/

#include "tskma_inc.h"
#include "tskma_int.h"
#include "tskma_dat.h"

#define TSKMA_MODULE_ID 	TSKMA_MODULE_ID_TSKMA_IRTE_ISR


//*============================================================================
//*=====     IRTE     =========================================================
//*============================================================================
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200_400) /* IRTE */

//*----------------------------------------------------------------------------
//*	tskma_isr_00()
//*
//* @brief IRTE interrupt service routine 0
//*----------------------------------------------------------------------------
void tskma_isr_00(LSA_UINT32 int_source)
{

}


//*----------------------------------------------------------------------------
//*	tskma_isr_01()
//*
//* @brief IRTE interrupt service routine 1
//*----------------------------------------------------------------------------
void tskma_isr_01(LSA_UINT32 int_source)
{
    LSA_UINT32 isre_irt, isre_nrt;
    IRTE_SW_MAKRO_REV6_T *irte = (IRTE_SW_MAKRO_REV6_T *)TSKMA_IRTE_BASE;
    LSA_UINT32 nc_event=0;

    /*****************************************************************/
    /* 1. collect IRT events - separate them into high and low list  */
    /*****************************************************************/

    eddi_interrupt_get_user_int(lsas_com_get_hddb(), &isre_irt, &isre_nrt);

    if (isre_irt || isre_nrt)
    {
        LSA_UINT32 isre_irt_quit, isre_nrt_quit;
        isre_irt_quit =
        isre_nrt_quit = 0;

        if(isre_irt & IRQ_START_OPERATE)       /* 1-Bit, start operation */
        {
            irte->s13000_IRT_control.x13400_IRT_syncMode = 3;
            isre_irt_quit |= IRQ_START_OPERATE;
        }

        if(isre_irt & IRQ_IRT_TRANS_END)       /* 1-Bit, start operation */
        {
			#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200_400)
        	pnpb_cbf_trigger_io_exchange();
			#endif

        	if( tskma_data.ts_state == TSKMA_TS_SCHEDULING_STATE_TS_TASK_FINISHED )
            {
                nc_event |= 0x1;
                tskma_data.ts_state = TSKMA_TS_SCHEDULING_STATE_APPLICATION_TASK_RUNNING;

                TSKMA_UPDATE_CONSUMER_DATA();
                TSKMA_UPDATE_PROVIDER_DATA();

                /*TSKMA_RQB_SET_OPCODE(&tmp_tskma_rqb, TSKMA_OPC_APP_CYCLIC_EA); */ /* as this rqb is static it should already contain the correct opcode */
                TSKMA_OS_SEND_MAIL(TSKMA_TASK_NUM_APP, &tskma_data.rqb.cyclic_ea_rqb);
                TSKMA_OS_TASK_WAKE_UP(TSKMA_TASK_NUM_APP);
            }
            isre_irt_quit |= IRQ_IRT_TRANS_END;
        }

        eddi_interrupt_ack_user_int(lsas_com_get_hddb(), isre_irt_quit, isre_nrt_quit);

    }

    eddi_interrupt(lsas_com_get_hddb());

    eddi_interrupt_set_eoi(lsas_com_get_hddb(), 0xf);
}

#endif // (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200_400)


//*============================================================================
//*=====     PERI     =========================================================
//*============================================================================
#if (PNIOD_PLATFORM & (PNIOD_PLATFORM_EB200P | PNIOD_PLATFORM_RZT2_EDDP)) /* PNIP */

//*----------------------------------------------------------------------------
//*	tskma_isr_00()
//*
//* @brief PERI acyclic receive ISR
//*----------------------------------------------------------------------------
OS_CODE_FAST void tskma_isr_00(LSA_UINT32 int_source)
{
    /*
     * to achieve interrupt load limiting, the register IRQxControl_MUXn
     * has to be set to its maximum value before eddp_interrupt is called
     * (see chapter "Limiting Interrupt Load for Receiving Acyclic Frames", eddp_sys.doc)
     */
    eddp_SetIRQ_WaitTime_to_MAX(lsas_com_get_hddb(), 2);

    eddp_interrupt(lsas_com_get_hddb(), 2);
}


//*----------------------------------------------------------------------------
//*	tskma_isr_01()
//*
//* @brief PERI group irq ISR
//*----------------------------------------------------------------------------
OS_CODE_FAST void tskma_isr_01(LSA_UINT32 int_source)
{
    eddp_interrupt(lsas_com_get_hddb(), 0);
}


//*----------------------------------------------------------------------------
//*	tskma_parity_error_isr()
//*
//* @brief PERI parity error ISR
//*----------------------------------------------------------------------------
void tskma_parity_error_isr(LSA_UINT32 int_source)
{
	tskma_parity_error_count++;
	tskma_parity_error_source |= int_source;
	//tskma_fatal (TSKMA_MODULE_ID, __LINE__, int_source);
}


//*----------------------------------------------------------------------------
//*	tskma_access_error_isr()
//*
//* @brief PERI data access error ISR
//*----------------------------------------------------------------------------
void tskma_access_error_isr(LSA_UINT32 int_source)
{
	tskma_access_error_count++;
	tskma_access_error_source |= int_source;
	//tskma_fatal (TSKMA_MODULE_ID, __LINE__, int_source);
}

#endif // (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P)

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
