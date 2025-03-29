/***********************************************************************************************************************
 * Copyright [2020] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics America Inc. and may only be used with products
 * of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.  Renesas products are
 * sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for the selection and use
 * of Renesas products and Renesas assumes no liability.  No license, express or implied, to any intellectual property
 * right is granted by Renesas. This software is protected under all applicable laws, including copyright laws. Renesas
 * reserves the right to change or discontinue this software and/or this documentation. THE SOFTWARE AND DOCUMENTATION
 * IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND TO THE FULLEST EXTENT
 * PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY, INCLUDING WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE SOFTWARE OR
 * DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.  TO THE MAXIMUM
 * EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR DOCUMENTATION
 * (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER, INCLUDING,
 * WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY LOST PROFITS,
 * OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE POSSIBILITY
 * OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup ETHER_SWITCH
 * @{
 **********************************************************************************************************************/

#ifndef R_ETHER_SWITCH_H
#define R_ETHER_SWITCH_H

#ifndef BSP_PLAT_RZN1SDB /* RZ/T2 */
#include "bsp_api.h"
#endif /* !BSP_PLAT_RZN1SDB */

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_ether_switch_cfg.h"
#include "r_ether_switch_api_rzt2.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define ETHER_SWITCH_CODE_VERSION_MAJOR         (0U)
#define ETHER_SWITCH_CODE_VERSION_MINOR         (1U)

#define ETHER_SWITCH_PORT_OFFSET                (0x400)
#define ETHER_SWITCH_PORT_OFFSET_4BYTES         (0x04)
#define ETHER_SWITCH_PORT_OFFSET_8BYTES         (0x08)
#define ETHER_SWITCH_PORT_OFFSET_16BYTES        (0x10)
#define ETHER_SWITCH_PORT_OFFSET_32BYTES        (0x20)

#define ETHER_SWITCH_PORT_TX_MSK                (1)      ///< COMMAND_CONFIG_P[n] - mask for tx_ena
#define ETHER_SWITCH_PORT_RX_MSK                (2)      ///< COMMAND_CONFIG_P[n] - mask for rx_ena
#define ETHER_SWITCH_PORT_RXTX                  (ETHER_SWITCH_PORT_TX_MSK | ETHER_SWITCH_PORT_RX_MSK)

#ifdef BSP_PLAT_RZN1SDB /* RZ/N1 */
    #define ETHER_SWITCH_VLANID_MASK            (0x1FFE0)
    #define ETHER_SWITCH_VLANID_OFFSET          (5)
    #define ETHER_SWITCH_VLAN_PORT_MASK         (0x1F)
    #define ETHER_SWITCH_VLAN_INVALID_ENTRY     (0x1FFFF)
#else /* !BSP_PLAT_RZN1SDB */ /* RZ/T2 */
    #define ETHER_SWITCH_VLANID_MASK            (0xFFF0)
    #define ETHER_SWITCH_VLANID_OFFSET          (4)
    #define ETHER_SWITCH_VLAN_PORT_MASK         (0xF)
    #define ETHER_SWITCH_VLAN_INVALID_ENTRY     (0x0FFFF)
#endif /* !BSP_PLAT_RZN1SDB *//* RZ/T2 */
#define ETHER_SWITCH_VLAN_INOUT_MODE_MASK       (0x3)
#define ETHER_SWITCH_VLAN_DEFID_MASK            (0xFFFF)
#define ETHER_SWITCH_VLAN_MODE_BITS             (0x2)

#define ETHER_SWITCH_LK_MASK                    (0x9E)
#define ETHER_SWITCH_LK_PERVLAN                 (7)
#define ETHER_SWITCH_LK_DISCUNKN                (4)
#define ETHER_SWITCH_LK_MIGRAT                  (3)
#define ETHER_SWITCH_LK_AGING                   (2)
#define ETHER_SWITCH_LK_LEARN                   (1)
#define ETHER_SWITCH_LK_ADDR_CLEAR              (1<<29)
#define ETHER_SWITCH_LK_ADDR_LOOKUP             (1<<28)
#define ETHER_SWITCH_LK_ADDR_VALID              (1)
#define ETHER_SWITCH_LK_ADDR_WRITE              (1<<25)
#define ETHER_SWITCH_LK_ADDR_REG_VALID          (1<<16)
#define ETHER_SWITCH_LK_ADDR_REG_TYPE           (1<<17)
#define ETHER_SWITCH_LK_ADDR_PORTMASK           (0xF << 21)
#define ETHER_SWITCH_LK_ADDR_PRIOMASK           (0x7 << 18)
#define ETHER_SWITCH_LK_ADDR_PORT_OFFSET        (5)
#define ETHER_SWITCH_LK_ADDR_PORT_REG_OFFSET    (21)
#define ETHER_SWITCH_LK_ADDR_PRIO_REG_OFFSET    (18)
#define ETHER_SWITCH_LK_ADDR_MASKANDFLAGS_MASK  (0xFFFFu << 16)

#define ETHER_SWITCH_COUNTER_OFFSET             (0x400)

#define ETHER_SWITCH_PHY_LINK                   (4)

#ifdef ETHER_SWITCH_CONFIG_PRIORITY3_OTHER
#define ETHER_SWITCH_QOS_VLANPRIO_MASK          (0xFFFFFF)
#ifdef BSP_PLAT_RZN1SDB /* RZ/N1 */
    #define ETHER_SWITCH_QOS_OUTQUEUES          (4)
#else /* !BSP_PLAT_RZN1SDB (RZ/T2) */
    #define ETHER_SWITCH_QOS_OUTQUEUES          (8)
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */

#ifdef BSP_PLAT_RZN1SDB /* RZ/N1 */
    #define ETHER_SWITCH_TARGET_ETH_PORT_COUNT  4       ///< external port count
#else /* !BSP_PLAT_RZN1SDB (RZ/T2) */
    #define ETHER_SWITCH_TARGET_ETH_PORT_COUNT  2       ///< external port count
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */

#define ETHER_SWITCH_TARGET_ETH_PORT_ALL        (ETHER_SWITCH_TARGET_ETH_PORT_COUNT + 1)    ///< ext and int (host) port count
#define ETHER_SWITCH_BOARD_MAP_PORT_TO_TGT(res, port, portTgt) \
    res = ether_switch_map_to_tgt(portmap_switch, port, &portTgt)

#define ETHER_SWITCH_QOS_MODE_MASK              (0xF)
#define ETHER_SWITCH_QOS_DEFPRIO_OFFSET         (4)
#ifdef BSP_PLAT_RZN1SDB /* RZ/N1 */
    #define ETHER_SWITCH_QOS_DEFPRIO_MASK       (3)
#else /* !BSP_PLAT_RZN1SDB (RZ/T2) */
    #define ETHER_SWITCH_QOS_DEFPRIO_MASK       (7)
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */
#define ETHER_SWITCH_QOS_IPPRIO_READ            (1u<<31)
#define ETHER_SWITCH_QOS_IPPRIO_WRITE           (0u<<31)
#define ETHER_SWITCH_QOS_IPPRIO_OFFSET          (9)
#define ETHER_SWITCH_QOS_IPPRIO_DSMASK          (0xFF)
#ifdef BSP_PLAT_RZN1SDB /* RZ/N1 */
    #define ETHER_SWITCH_QOS_IPPRIO_MASK       (3)
#else /* !BSP_PLAT_RZN1SDB (RZ/T2) */
    #define ETHER_SWITCH_QOS_IPPRIO_MASK       (7)
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */
#endif /* ETHER_SWITCH_CONFIG_PRIORITY3_OTHER */

#ifdef ETHER_SWITCH_CONFIG_PRIORITY4
#define ETHER_SWITCH_HZ                         (200000000)
#ifdef BSP_PLAT_RZN1SDB /* RZ/N1 */
    #define ETHER_SWITCH_DOS_TIMEFACTOR         (65536) ///< The time factor is calculated in 65536 switch system clock cycles
#else /* !BSP_PLAT_RZN1SDB (RZ/T2) */
    #define ETHER_SWITCH_DOS_TIMEFACTOR         (16385) ///< The time factor is calculated in 16385 switch system clock cycles
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */

#define ETHER_SWITCH_RATE_IS_MASK               (0x7FF)
#define ETHER_SWITCH_RATE_IS_MAX                (80)
#endif /* ETHER_SWITCH_CONFIG_PRIORITY4 */

      
      
      
/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** ETHER PHY control block. DO NOT INITIALIZE.  Initialization occurs when @ref ether_phy_api_t::open is called. */
typedef struct st_ether_switch_instance_ctrl
{
    uint32_t open;                          ///< Used to determine if the channel is configured

    /* Configuration of Ethernet SWITCH-LSI module. */
    ether_switch_cfg_t const * p_switch_cfg;   ///< Pointer to initial configurations.

    /* Base register of Ethernet SWITCH-LSI chip. */
    volatile uint32_t * p_reg_switch;       ///< Pointer to ETHERC peripheral registers.

} ether_switch_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const ether_switch_api_t g_ether_switch_on_ether;

/** @endcond */


/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 ***********************************************************************************************************************/
#ifdef BSP_PLAT_RZN1SDB /* RZ/N1S */
    static void ether_switch_isr_intr (IRQn_Type *irq_num_ptr);
#else /* !BSP_PLAT_RZN1SDB (RZ/T2) */
  #ifndef RZT2_RSK
    void ether_switch_isr_intr (uint32_t int_sense);
  #endif /* !RZT2_RSK */
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */

    
/**********************************************************************************************************************
 * Public Function Prototypes
 **********************************************************************************************************************/
fsp_err_t R_ETHER_SWITCH_Open(ether_switch_ctrl_t * const p_ctrl, ether_switch_cfg_t const * const p_cfg);

fsp_err_t R_ETHER_SWITCH_Close(ether_switch_ctrl_t * const p_ctrl);

fsp_err_t R_ETHER_SWITCH_ConfSpeed(ether_switch_ctrl_t *     const p_ctrl, 
                                   ether_switch_port_t       const port,
                                   ether_switch_link_speed_t const speed);


fsp_err_t R_ETHER_SWITCH_Control(ether_switch_ctrl_t * const p_ctrl, ether_switch_cmd_t * p_cmd);
void profinet_filter_conf(void);
/****************************************************************************/
/* Register write access workaround */
/****************************************************************************/
#ifndef A5PSW_WORKAROUND
#  define A5PSW_WORKAROUND 0
#else
#  ifndef BSP_PLAT_RZN1SDB /* !BSP_PLAT_RZN1SDB (RZ/T2) */
#    error "Define A5PSW_WORKAROUND is only allowed on RZ/N family CPUs"
#  endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */

#  error "A5PSW_WORKAROUND is not supported"
#endif

#if A5PSW_WORKAROUND == 0

#   define ETHER_SWITCH_SW_SEMA_BUS_GET(timeout)    ///< empty macro for compliance
#   define ETHER_SWITCH_SW_SEMA_BUS_RELEASE         ///< empty macro for compliance
#   define ETHER_SWITCH_CPU_LOCK                    ///< empty macro for compliance
#   define ETHER_SWITCH_CPU_UNLOCK                  ///< empty macro for compliance

/* memcpy default mapping macro for compliance */
#   define ETHER_SWITCH_MEMCPY_SAVE(pDst, pSrc, size) \
        memcpy(pDst, pSrc, size);

/* memset default mapping macro for compliance */
#   define ETHER_SWITCH_MEMSET_SAVE(pDst, value, size) \
        memset(pDst, value, size);

    /* default register writing for via union access */
#   define ETHER_SWITCH_REG_SET_FIELD(set_base, set_reg, set_reg_bit, set_field, set_value, type) \
        set_base->set_reg_bit.set_field = set_value;

    /* default register writing for via union access */
#   define ETHER_SWITCH_REG_SET(set_expression, val)  \
        set_expression = val;

#endif

/*******************************************************************************************************************//**
 * @} (end addtogroup ETHER_SWITCH)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 // R_ETHER_SWITCH_H
