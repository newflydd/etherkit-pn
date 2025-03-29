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
/*  F i l e               &F: evma_cfg.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  This file contains the configuration of the EVMA module                  */
/*                                                                           */
/*****************************************************************************/

#ifndef EVMA_CFG_H_
#define EVMA_CFG_H_

/**                            common
 * choose the Hardware by defining  EVMA_USE_PNIP, EVMA_USE_IRTE or none of them.
 *
 * By defining EVMA_USE_PNIP, an Ertec200p device is expected.
 * By defining EVMA_USE_IRTE, an Ertec200  device is expected.
 * By defining EVMA_USE_RZT2, an RZ/T2M device is expected.
 */
#define IM_HW_RZT2
#ifdef IM_HW_ERTEC200P
    #define EVMA_USE_PNIP
#elif defined(IM_HW_ERTEC200)
    #define EVMA_USE_IRTE
#elif defined(IM_HW_RZT2)
    #define EVMA_USE_RZT2
#endif

/**                            common
 * The interface functions of the EVMA module are not reentrant -> if several
 * tasks / interruptible software layers might access EVMA's functions
 * simultaneously, this has to be prevented by an interrupt lock or semaphore.
 */
#define EVMA_ENTER()
#define EVMA_EXIT()

/*
 * TCM usage macros
 */

    /**
     * use the EVMA_USE_TCM define to locate ISR code and data that is used inside
     * the ISR into DTCM + ITCM. Give the respective memory sections of the DTCM
     * and ITCM as well as the default memory section (SDRAM) by setting the following defines ...
     */
#define EVMA_USE_TCM

#ifdef EVMA_USE_TCM
    #define EVMA_DTCM_SECTION_NAME OS_DATA_DTCM_SECTION_NAME               /**< section name of the DTCM */
    #define EVMA_ITCM_SECTION_NAME OS_ITCM_SECTION_NAME                    /**< section name of the ITCM */

    #define EVMA_DEFAULT_BSS_SECTION_NAME ".bss_sys_evma"       /**< section name of the default data section for EVMA */
    #define EVMA_DEFAULT_TEXT_SECTION_NAME ".text_sys_evma"     /**< section name of the default code section for EVMA */
#endif

/**
 * EVMA_MAX_NUM_INSTANCES_PER_COMPARATOR defines the maximum number of instances,
 * that can be allocated for one comparator. Multiple instances referring to the same
 * comparator are only possible, if all the instances use the same comparator threshold value.
 * The instances might use different outputs (GPIOs, interrupts, GDMA targets)
 */
#define EVMA_MAX_NUM_INSTANCES_PER_COMPARATOR 	10

/**                            common
 * EVMA_SET_INT_HANDLER registers an interrupt callback function for a specific interrupt.
 * ERTEC200P : the IRQ index is ASIC specific and is known by EVMA
 * ERTEC200  : specify the IRQ index to be used by defining EVMA_IRTE_IRQ_INDEX [12/13]. Use the interrupt that is
 *             not used already by the EDD
 * If cbf is NULL, the interrupt has to be disabled; if cbf isn't equal to NULL, the interrupt
 * has to be enabled and the cbf must be registered.
 * @param   irq_index   index of the IRQ to be handled
 * @param   cbf         function pointer to the IRQ callback [void (func*)(void)] or NULL to disable IRQ
 */
#ifdef IM_HW_ERTEC200P
    #define EVMA_SET_INT_HANDLER(irq_index, cbf)  Bsp_EVMA_register_ISR(irq_index, cbf)
#elif defined(IM_HW_ERTEC200)
    #define EVMA_SET_INT_HANDLER(irq_index, cbf)  hama_icu_set_int_handler(irq_index, cbf)
#endif

#ifdef EVMA_USE_IRTE
    /**                    IRTE specific
     * EVMA_IRTE_IRQ_INDEX  specifies which group interrupt should be used by EVMA on the Ertec200. The number is
     * passed to EVMA_SET_INT_HANDLER(irq_index, cbf). Make sure that there are no collisions with the EDDI!
     * --> make sure that the EDDI-IRQ doesn't get disabled by EVMA or anything like that.
     *
     * CHECK WHICH GROUP INTERRUPT THE EDDI USES BY LOOKING FOR YOUR SETTING OF dpb_ptr->SII_IrqNumber @ EDDI_SRV_DEV_OPEN
     * AND USE THE OTHER GROUP INTERRUPT FOR EVMA.
     */
    #define EVMA_IRTE_IRQ_INDEX     HAMA_ICU_INT_IRTE0
#endif


/*                        hardware specifc
 * register access macros
 *
 * Normally no adjustments should be necessary here ...
 */
#ifdef EVMA_USE_PNIP
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200P)
        /**
         *  EVMA_WRITE_PNIP_PLL_OUT_CONTROL is used to set the Multiplexer of the PNIP PNPLL module.
         *  @param x        defines the multiplexer output index (0-20)
         *  @param value    defines the 32 bit register value to be written
         */
    #define EVMA_WRITE_PNIP_PLL_OUT_CONTROL(x, value)       EDDP_USR_REG32_WRITE(U_PNIP__IP_VERSION, U_PNIP__PLL_OUT_CONTROL_0-U_PNIP__IP_VERSION+4*x, value)

        /**
         *  EVMA_WRITE_PNIP_APPLTIMER2_COMP_TIME is used to set a comparator value of application timer 2
         *  @param x        defines the comparator index (0(1) - 6)
         *  @param value    defines the 32 bit register value to be written
         */
    #define EVMA_WRITE_PNIP_APPLTIMER2_COMP_TIME(x, value)  EDDP_USR_REG32_WRITE(U_PNIP__IP_VERSION, U_PNIP__APPLCOMPARE_2_1-U_PNIP__IP_VERSION+x*4, value)
        /**
         * EVMA_READ_PNIP_APPLTIMER2_TIME reads the current timer value of application timer 2
         */
    #define EVMA_READ_PNIP_APPLTIMER2_TIME()                EDDP_USR_REG32_READ(U_PNIP__IP_VERSION, U_PNIP__APPLTIMER_2-U_PNIP__IP_VERSION)
        /**
         * EVMA_READ_PNIP_APPLTIMER2_LENGTH reads the cycle length of application timer 2
         */
    #define EVMA_READ_PNIP_APPLTIMER2_LENGTH()              EDDP_USR_REG32_READ(U_PNIP__IP_VERSION, U_PNIP__APPLLENGTH_2-U_PNIP__IP_VERSION)
        /**
         * EVMA_READ_PNIP_APPLTIMER2_COMP_TIME reads a comparator value of application timer 2. 0(1) <= x <= 6 (= comparator index)
         */
    #define EVMA_READ_PNIP_APPLTIMER2_COMP_TIME(x)          EDDP_USR_REG32_READ(U_PNIP__IP_VERSION, U_PNIP__APPLCOMPARE_2_1-U_PNIP__IP_VERSION+4*x)
#else
 /**
         *  EVMA_WRITE_PNIP_PLL_OUT_CONTROL is used to set the Multiplexer of the PNIP PNPLL module.
         *  @param x        defines the multiplexer output index (0-20)
         *  @param value    defines the 32 bit register value to be written
         */
    LSA_VOID EVMA_WRITE_PNIP_PLL_OUT_CONTROL(LSA_UINT32 index, LSA_UINT32 value);

        /**
         *  EVMA_WRITE_PNIP_APPLTIMER2_COMP_TIME is used to set a comparator value of application timer 2
         *  @param x        defines the comparator index (0(1) - 6)
         *  @param value    defines the 32 bit register value to be written
         */
    LSA_VOID EVMA_WRITE_PNIP_APPLTIMER2_COMP_TIME(LSA_UINT32 index, LSA_UINT32 value);
        /**
         * EVMA_READ_PNIP_APPLTIMER2_TIME reads the current timer value of application timer 2
         */
    LSA_UINT32 EVMA_READ_PNIP_APPLTIMER2_TIME(LSA_VOID);
        /**
         * EVMA_READ_PNIP_APPLTIMER2_LENGTH reads the cycle length of application timer 2
         */
    LSA_UINT32 EVMA_READ_PNIP_APPLTIMER2_LENGTH(LSA_VOID);
        /**
         * EVMA_READ_PNIP_APPLTIMER2_COMP_TIME reads a comparator value of application timer 2. 0(1) <= x <= 6 (= comparator index)
         */
    LSA_UINT32 EVMA_READ_PNIP_APPLTIMER2_COMP_TIME(LSA_UINT32 comparator);
#endif
#elif defined(EVMA_USE_IRTE)

        /**
         * definition of the IRTE base address. Ertec200 uses Revision 6 of the IP
         */
    #define EVMA_IRTE_BASE_ADDR                             ((IRTE_SW_MAKRO_REV6_T *) IRTE_BASE)

        /**
         * EVMA_IRTE_REG_ADDR_APPLTIMER_COMP_x(x) returns the address of the comparator register of application timer x
         */
    #define EVMA_IRTE_REG_ADDR_APPLTIMER_COMP_x(x)          ((LSA_UINT32*)&EVMA_IRTE_BASE_ADDR->s11000_Zyklussync.x11018_Comperator_Value_1 + x)

        /**
         * EVMA_IRTE_REG_ADDR_GPIO_MUX returns the address of the MUX selector register that has to be modified to forward comparator
         * events to GPIO pins.
         */
    #define EVMA_IRTE_REG_ADDR_GPIO_MUX                     (&EVMA_IRTE_BASE_ADDR->s11000_Zyklussync.x11030_Mux_Selektor)

        /**
         * EVMA_IRTE_REG_ADDR_IRT_CYCLE_ID returns the address of the IRT_Current_Cycle_ID register inside the IRTE switch controller.
         * It contains mask bits to forward the IRQ impulse that is used in interrupt context AND GPIO output context.
         */
    #define EVMA_IRTE_REG_ADDR_IRT_CYCLE_ID                 (&EVMA_IRTE_BASE_ADDR->s19000_global_para.x19010_IRT_Current_Cycle_ID)

        /**
         * EVMA_IRTE_REG_ADDR_ACK_IRT returns the address of the IRT interrupt acknowledge register inside the IRTE switch controller
         */
    #define EVMA_IRTE_REG_ADDR_ACK_IRT                      (&EVMA_IRTE_BASE_ADDR->s17430_switch_prozessor.x17438_SW_Interrupt_Ackn_Register_IRT)

        /**
         * EVMA_IRTE_REG_ADDR_IRQ_MASK_IRT_0 returns the address of the interrupt mask register for group interrupt 0
         */
    #define EVMA_IRTE_REG_ADDR_IRQ_MASK_IRT_0               (&EVMA_IRTE_BASE_ADDR->s17014_switch_prozessor.x17018_SW_Interrupt_MaskRegister_IRT_IRQ0)

        /**
         * EVMA_IRTE_REG_ADDR_IRQ_MASK_IRT_1 returns the address of the interrupt mask register for group interrupt 1
         */
    #define EVMA_IRTE_REG_ADDR_IRQ_MASK_IRT_1               (&EVMA_IRTE_BASE_ADDR->s17014_switch_prozessor.x17020_SW_Interrupt_MaskRegister_IRT_IRQ1)

        /**
         * EVMA_IRTE_REG_ADDR_IRQ_STATUS_0 returns the address of the interrupt flag register for group interrupt 0
         */
    #define EVMA_IRTE_REG_ADDR_IRQ_STATUS_0                 (&EVMA_IRTE_BASE_ADDR->s17430_switch_prozessor.x17428_SW_Interrupt_Register_IRT_IRQ0)

        /**
         * EVMA_IRTE_REG_ADDR_IRQ_STATUS_1 returns the address of the interrupt flag register for group interrupt 1
         */
    #define EVMA_IRTE_REG_ADDR_IRQ_STATUS_1                 (&EVMA_IRTE_BASE_ADDR->s17430_switch_prozessor.x17430_SW_Interrupt_Register_IRT_IRQ1)

        /**
         * EVMA_IRTE_REG_ADDR_EOI_0 returns the address of the end of interrupt register for group interrupt 0. If the EVMA
         * shall not write to this register, just omit the define.
         */
    #define EVMA_IRTE_REG_ADDR_EOI_0                        (&EVMA_IRTE_BASE_ADDR->s17430_switch_prozessor.x17448_SW_EOI_IRQ0_Register)

        /**
         * EVMA_IRTE_REG_ADDR_EOI_0 returns the address of the end of interrupt register for group interrupt 1. If the EVMA
         * shall not write to this register, just omit the define.
         */
    #define EVMA_IRTE_REG_ADDR_EOI_1                        (&EVMA_IRTE_BASE_ADDR->s17430_switch_prozessor.x1744c_SW_EOI_IRQ1_Register)

        /**
         * EVMA_IRTE_REG_ADDR_CLKCY_PERIOD returns the address of the Clkcy_Period register serving as base clock for the
         * application timer
         */
    #define EVMA_IRTE_REG_ADDR_CLKCY_PERIOD                 (&EVMA_IRTE_BASE_ADDR->s11000_Zyklussync.x11010_Clkcy_Period)

        /**
         * EVMA_IRTE_REG_ADDR_APLCLK_DIVIDER returns the address off the AplClk_Divider register holding the sendclock factor.
         */
    #define EVMA_IRTE_REG_ADDR_APLCLK_DIVIDER               (&EVMA_IRTE_BASE_ADDR->s11000_Zyklussync.x11000_AplClk_Divider)

        /**
         * EVMA_IRTE_REG_ADDR_APLCLK_TIMER returns the address of the timer register (counter)
         */
    #define EVMA_IRTE_REG_ADDR_APLCLK_TIMER                 (&EVMA_IRTE_BASE_ADDR->s11000_Zyklussync.x11410_AplClk_Timer)

#elif defined(EVMA_USE_RZT2)

#endif


/**                 common

 * error handling
 */

    /** if you define EVMA_CFG_FATAL_ERROR_ON_FAILURE, EVMA will go to fatal error if an assert fails */
#define EVMA_CFG_FATAL_ERROR_ON_FAILURE

    /** the lsa_component_id is passed to the fatal-error handler within the error detail structure */
#define EVMA_LSA_COMPONENT_ID                    0x0000
    /** set the fatal error handler here. it gets the error detail structure as a parameter (type of the structure : LSA_FATAL_ERROR_TYPE) */
#define EVMA_FATAL_ERROR(_ERROR_DETAIL_PTR)      LSAS_FATAL_ERROR(_ERROR_DETAIL_PTR)
    /** set the module id of evma_pnpll.c here. It is passed to the fatal error handler through the detail structure
     *  (-> field ERROR_DETAIL_PTR.module_id)*/
#define EVMA_MODULE                              0x01

/* ****************************************************************** */
#include "evma_plau.h"
/* ****************************************************************** */

#endif /* EVMA_CFG_H_ */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
