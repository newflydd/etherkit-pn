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

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/

#include <string.h>

#ifdef BSP_PLAT_RZN1SDB /* RZ/N1S */

    #include "platform.h"
    #include "ETHSW_SFR.h"

    #include "fsp_common_api.h"
    #include "r_ether_switch_rzt2.h"
#ifdef ETHER_SWITCH_CONFIG_PRIORITY4
    #include <math.h>
#endif /* ETHER_SWITCH_CONFIG_PRIORITY4 */

    #undef  R_ETHSW
    #define R_ETHSW   RZN1_SWITCH_BASE

    #define R_BSP_MODULE_START(a, b)
    #define R_BSP_MODULE_STOP(a, b)

    /** All FSP error codes are returned using this macro. Calls ::FSP_ERROR_LOG function if condition "a" is false. Used
     * to identify runtime errors in FSP functions. */

    #define FSP_ERROR_RETURN(a, err)                        \
        {                                                   \
            if ((a))                                        \
            {                                               \
                (void) 0;                  /* Do nothing */ \
            }                                               \
            else                                            \
            {                                               \
                return err;                                 \
            }                                               \
        }

    #define FSP_ASSERT(a)    FSP_ERROR_RETURN((a), FSP_ERR_ASSERTION)

#else  /* !BSP_PLAT_RZN1SDB (RZ/T2) */
//#include "common_data.h"
#include "ether_config.h"
#if 0
    #include "bsp_api.h"
    #include "r_ether_rzt2.h"
    #include "r_ether_switch_rzt2.h"
#endif
  #ifdef ETHER_SWITCH_CONFIG_PRIORITY4
    #include <math.h>
  #endif /* ETHER_SWITCH_CONFIG_PRIORITY4 */
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */


#include "r_ether_switch_rzt2_ext.h"
extern  ether_switch_portentry_t    switch_config[];

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/
#ifndef ETHER_SWITCH_ERROR_RETURN

/*LDRA_INSPECTED 77 S This macro does not work when surrounded by parentheses. */
 #define ETHER_SWITCH_ERROR_RETURN(a, err)      FSP_ERROR_RETURN((a), (err))
#endif

/* Maximum Frame Length */
#define FRAME_SIZE (0x2408)

/* MMCTL_QTRIG Register Bit Definitions */
#define MMCTL_QTRIG_QUEUE_TRIG_SHIFT            16
#define MMCTL_QTRIG_QUEUE_TRIG_MASK             (0xffff << MMCTL_QTRIG_QUEUE_TRIG_SHIFT)
#define MMCTL_QTRIG_PORT_MASK_SHIFT             0
#define MMCTL_QTRIG_PORT_MASK_MASK              (0xf <<MMCTL_QTRIG_PORT_MASK_SHIFT)

#ifdef WS2 /* FPGA Ver2 */
    /* TCV_SEQ_CTRL Register Bit Definitions */
    #define TDMA_TCV_SEQ_START                  (1UL  << 0)
    #define TDMA_TCV_SEQ_INT                    (1UL  << 1)
    #define TDMA_TCV_SEQ_D_IDX_SHIFT            2
    #define TDMA_TCV_SEQ_D_IDX_MASK             (0x1ff << TDMA_TCV_SEQ_D_IDX_SHIFT)
    #define TDMA_TCV_SEQ_GPIO_SHIFT             11
    #define TDMA_TCV_SEQ_GPIO_MASK              (0xf << TDMA_TCV_SEQ_GPIO_SHIFT)
    #define TDMA_TCV_SEQ_READ_MODE              (1UL  << 31)

    /* TCV_SEQ_ADDR Register Bit Definitions */
    #define TDMA_TCV_SEQ_S_ADDR_SHIFT           0
    #define TDMA_TCV_SEQ_S_ADDR_MASK            (0xfff << TDMA_TCV_SEQ_S_ADDR_SHIFT)
#else /* !WS2 */ /* FPGA Ver1 */
    /* TCV_SEQ Register Bit Definitions */
    #define TDMA_TCV_SEQ_START                  (1UL  << 0)
    #define TDMA_TCV_SEQ_INT                    (1UL  << 1)
    #define TDMA_TCV_SEQ_D_IDX_SHIFT            2
    #define TDMA_TCV_SEQ_D_IDX_MASK             (0x1ff << TDMA_TCV_SEQ_D_IDX_SHIFT)
    #define TDMA_TCV_SEQ_GPIO_SHIFT             11
    #define TDMA_TCV_SEQ_GPIO_MASK              (0xf << TDMA_TCV_SEQ_GPIO_SHIFT)
    #define TDMA_TCV_SEQ_S_ADDR_SHIFT           16
    #define TDMA_TCV_SEQ_S_ADDR_MASK            (0xfff << TDMA_TCV_SEQ_S_ADDR_SHIFT)
    #define TDMA_TCV_SEQ_READ_MODE              (1UL  << 31)
#endif /* !WS2 */ /* FPGA Ver1 */

/* TDMA_CONFIG Register Bit Definitions */
#define TDMA_CONFIG_TIMER_SEL_MASK              0x1

/* TDMA_ENA_CTRL Register Bit Definitions */
#define TDMA_ENA_CTRL_PORT_ENA_MASK             0x7

/* TDMA_IRQ_CONTRO and TDMA_IRQ_STAT_ACK Register Bit Definitions */
#define TDMA_IRQ_CONTROL_TCV_INT_EN             (1UL << 0)
#define TDMA_IRQ_CONTROL_TCV_IDX_INT_ENA_SHIFT  1
#define TDMA_IRQ_CONTROL_TCV_IDX_INT_ENA_MASK   (0xfff << TDMA_IRQ_CONTROL_TCV_IDX_INT_ENA_SHIFT)
#define TDMA_IRQ_CONTROL_CTR1_INT_EN            (1UL << 13)

/* CONFIG Register Bit Definitions */
#define CONFIG_IRQ_TX_EN_SHIFT                  16
#define CONFIG_IRQ_TX_EN_MASK                   (7UL << CONFIG_IRQ_TX_EN_SHIFT)
#define CONFIG_IRQ_TSFIFO_OVR                   (1UL << 2)
#define CONFIG_IRQ_EN                           (1UL << 0)

/* ATIME_CTRL Register Bit Definitions */
#define ATIME_CTRL_ENABLE                       (1UL << 0)
#define ATIME_CTRL_CAPTURE                      (1UL << 11)

/* PTP_DOMAIN Register Bit Definitions */
#define PTP_DOMAIN_SHIFT                        8
#define PTP_DOMAIN_MASK                         0xFF

/* COMMAND_CONFIG Register Bit Definitions */
#define COMMAND_CONFIG_TIMER_SEL                (1UL << 30)
#define COMMAND_CONFIG_ENA_10                   (1UL << 25)
#define COMMAND_CONFIG_NO_LGTH_CHECK            (1UL << 24)
#define COMMAND_CONFIG_CNTL_FRM_ENA             (1UL << 23)
#define COMMAND_CONFIG_LOOP_ENA                 (1UL << 15)
#define COMMAND_CONFIG_SW_RESET                 (1UL << 13)
#define COMMAND_CONFIG_TX_CRC_APPEND            (1UL << 11)
#define COMMAND_CONFIG_HD_ENA                   (1UL << 10)
#define COMMAND_CONFIG_TX_ADDR_INS              (1UL << 9)
#define COMMAND_CONFIG_PAUSE_IGNORE             (1UL << 8)
#define COMMAND_CONFIG_PAUSE_FWD                (1UL << 7)
#define COMMAND_CONFIG_CRC_FWD                  (1UL << 6)
#define COMMAND_CONFIG_PAD_EN                   (1UL << 5)
#define COMMAND_CONFIG_PROMIS_EN                (1UL << 4)
#define COMMAND_CONFIG_ETH_SPEED                (1UL << 3)
#define COMMAND_CONFIG_RX_ENA                   (1UL << 1)
#define COMMAND_CONFIG_TX_ENA                   (1UL << 0)

/* FIFO_READ_CTRL Register Bit Definitions */
#define FIFO_READ_CTRL_TS_VALID                 (1UL << 4)
#define FIFO_READ_CTRL_TS_SEL_SHIFT             6
#define FIFO_READ_CTRL_TS_SEL_MASK              (0x3 << FIFO_READ_CTRL_TS_SEL_SHIFT)
#define FIFO_READ_CTRL_TS_ID_SHIFT              8
#define FIFO_READ_CTRL_TS_ID_MASK               (0xf << FIFO_READ_CTRL_TS_ID_SHIFT)

/* GPARSER Register Bit Definitions */
#define ETHER_SWITCH_GPARSER_PARSER_MAX         (7)
#define ETHER_SWITCH_GPARSER_ARITH_MAX          (7)
#define ETHER_SWITCH_GPARSER_SNOOP_STAGE1       (3)
#define ETHER_SWITCH_GPARSER_SNOOP_STAGE2       (7)
#define ETHER_SWITCH_GPARSER_PARSER_VALID       (1<<24)
#define ETHER_SWITCH_GPARSER_CMPMASK_OFFSET     (30)
#define ETHER_SWITCH_GPARSER_CMP_OFFSET         (8)
#define ETHER_SWITCH_GPARSER_MODE_OFFSET_DA     (1<<23)
#define ETHER_SWITCH_GPARSER_MODE_SKIPVLAN      (1<<25)
#define ETHER_SWITCH_GPARSER_MODE_IPDATA        (1<<26)
#define ETHER_SWITCH_GPARSER_MODE_IPPROTO       (1<<27)
#define ETHER_SWITCH_GPARSER_MODE_COMPARE16     (1<<28)
#define ETHER_SWITCH_GPARSER_MODE_OFFSETPLUS2   (1<<29)
#define ETHER_SWITCH_GPARSER_MODE_CMP_OR        (1<<30)

#define ETHER_SWITCH_GPARSER_MODE_NONIPPROT \
            (ETHER_SWITCH_GPARSER_MODE_SKIPVLAN | ETHER_SWITCH_GPARSER_MODE_CMP_OR | \
              ETHER_SWITCH_GPARSER_MODE_COMPARE16 | ETHER_SWITCH_GPARSER_MODE_OFFSETPLUS2 | \
                ETHER_SWITCH_GPARSER_MODE_IPDATA)

#define ETHER_SWITCH_GPARSER_MODE_VALID \
              (ETHER_SWITCH_GPARSER_MODE_NONIPPROT | ETHER_SWITCH_GPARSER_MODE_IPPROTO)

#define ETHER_SWITCH_GPARSER_MODE_INVALID \
              ~((uint32_t) ETHER_SWITCH_GPARSER_MODE_VALID)

#define ETHER_SWITCH_GPARSER_OFFSET_MASK        (0x3F)
#define ETHER_SWITCH_GPARSER_OFFSET_OFFSET      (16)
#define ETHER_SWITCH_GPARSER_ARITH_BLOCK1       (3)
#define ETHER_SWITCH_GPARSER_ARITH_BLOCK2       (7)
#define ETHER_SWITCH_GPARSER_GPARSER_BLOCK1     (3)
#define ETHER_SWITCH_GPARSER_GPARSER_BLOCK2     (7)
#define ETHER_SWITCH_GPARSER_INVERTMAP          (0xF)
#define ETHER_SWITCH_GPARSER_SELMATCH           (0xF)
#define ETHER_SWITCH_GPARSER_SELMATCH_OFFSET    (8)
#define ETHER_SWITCH_GPARSER_OPERATION_OFFSET   (16)
#define ETHER_SWITCH_GPARSER_RESINVERT_OFFSET   (17)
#define ETHER_SWITCH_GPARSER_SNOOPMODE_OFFSET   (20)
#define ETHER_SWITCH_GPARSER_SNOOPMODE_OFF      (0)
#define ETHER_SWITCH_GPARSER_SNOOPMODE_FWD_MGT  (1)
#define ETHER_SWITCH_GPARSER_SNOOPMODE_FWD_CPY  (2)
#define ETHER_SWITCH_GPARSER_SNOOPMODE_DISCARD  (3)
#define ETHER_SWITCH_GPARSER_ARITHSEL_OFFSET    (12)
#define ETHER_SWITCH_GPARSER_RESINVERT          (1<<17)
#define ETHER_SWITCH_GPARSER_BLOCK_CNT          (2)
#define ETHER_SWITCH_GPARSER_PARSER_PER_BLOCK   (4)
#define ETHER_SWITCH_GPARSER_PARSER_BLOCK_ID1   (1)
#define ETHER_SWITCH_GPARSER_PARSER_BLOCK_ID2   (2)
#define ETHER_SWITCH_GPARSER_PARSER_OPER_OR     (1<<16)
#define ETHER_SWITCH_GPARSER_ARTIH_ALL          (0x7)

/* EEE_CTL_STAT_P[n] Register Bit Definitions */
#define ETHER_SWITCH_EEE_AUTO                   (1)

/* AUTH_PORT[n] Register Bit Definitions */
#define ETHWE_SWITCH_PORT_AUTHORIZED            (1 << 0)
#define ETHWE_SWITCH_PORT_CONTROLLED_BOTH       (1 << 1)
#define ETHWE_SWITCH_PORT_EAPOL_ENABLE          (1 << 2)

/* MGMT_CONFIG Register Bit Definitions */
#define ETHER_SWITCH_MGMT_BPDU_FWD_ENABLE       (1 << 6)
#define ETHER_SWITCH_MGMT_BPDU_DISCARD          (1 << 7)

/* DLR_IRQ_CONTROL Register Bit Definitions */
#define ETHER_SWITCH_TGT_DLR_INT_NEWSTATUS      (1<<0)  ///< new ring status
#define ETHER_SWITCH_TGT_DLR_INT_BEACONTMO_0    (1<<4)  ///< beacon timeout port 0
#define ETHER_SWITCH_TGT_DLR_INT_BEACONTMO_1    (1<<5)  ///< beacon timeout port 1
#define ETHER_SWITCH_TGT_DLR_INT_SUVICHNG       (1<<6)  ///< Supervisor changed
#define ETHER_SWITCH_TGT_DLR_INT_LNKCHNG_0      (1<<7)  ///< link status changed port 0
#define ETHER_SWITCH_TGT_DLR_INT_LNKCHNG_1      (1<<8)  ///< link status changed port 1
#define ETHER_SWITCH_TGT_DLR_INT_BECONRCV_0     (1<<12) ///< beacon frame received port 0
#define ETHER_SWITCH_TGT_DLR_INT_BECONRCV_1     (1<<13) ///< beacon frame received port 1
#define ETHER_SWITCH_TGT_DLR_INT_LOCALLOOP_0    (1<<14) ///< loop detected port 0
#define ETHER_SWITCH_TGT_DLR_INT_LOCALLOOP_1    (1<<15) ///< loop detected port 1

/* FRM_LENGTH_P[n] Register Bit Definitions */
#define ETHER_SWITCH_FRAMESIZE_MASK             0x3FFF  ///< maximum frame size mask

/* ENABLE PORT Bit Definitions(PORT_ENA,UCAST_DEFAULT_MASK,BCAST_DEFAULT_MASK,MCAST_DEFAULT_MASK Register) */
#define ENABLE_PORT0                            (1UL << 0)
#define ENABLE_PORT1                            (1UL << 1)
#define ENABLE_PORT2                            (1UL << 2)
#define ENABLE_PORT3                            (1UL << 3)
#define ENABLE_PORT4                            (1UL << 4)

/* MGMT_TAG_CONFIG Register Tagfield set value */
#define TAGFIELD_DEFAULT                        0xE001

/* INT_CONFIG Register Bit Number Definitions */
#define INT_CONFIG_TSM_INT                      (1UL << 29)
#define INT_CONFIG_TDMA_INT                     (1UL << 30)
#define INT_CONFIG_PATTERN_INT                  (1UL << 31)

#ifdef BSP_PLAT_RZN1SDB /* RZ/N1 */
    #define INT_CONFIG_IRQ_LNK_SHIFT            8
    #define INT_CONFIG_IRQ_LNK_MASK             (0xF << INT_CONFIG_IRQ_LNK_SHIFT)
#else /* !BSP_PLAT_RZN1SDB */ /* RZ/T2 */
    #define INT_CONFIG_IRQ_LNK_SHIFT            8
    #define INT_CONFIG_IRQ_LNK_MASK             (0x7 << INT_CONFIG_IRQ_LNK_SHIFT)
#endif /* !BSP_PLAT_RZN1SDB */ /* RZ/T2 */
#define INT_CONFIG_DLR_INT                      (1UL <<  5)
#define INT_CONFIG_IRQ_EN                       (1UL <<  0)


/* STATUS_P[n] Register PHYSPEED value Definitions */
#define PHYSPEED_10MBPS                         0
#define PHYSPEED_100MBPS                        1
#define PHYSPEED_1GBPS                          2

/** "SWIT" in ASCII.  Used to determine if the control block is open. */
#define ETHER_SWITCH_OPEN                       (0x53574954U)

/* */


/* PAUSE link mask and shift values */

/* driver local define */
#define ETHER_SWITCH_PORT_COUNT         (3U)    ///< Number of port
#define ETHER_SWITCH_TIME_COUNT         (2U)    ///< Number of timer

#define SW_RN_ADVSW_TIMEOUT_MS          10      ///< 10 ms timeout
#define SW_RN_ADVSW_VLAN_TABLE_ENTRIES  32      ///< Number of VLAN table entries

#define SW_RN_ADVSW_DLR_ETHTYPE         0x80E1  ///< DLR EtherType for DLR module
#define SW_RN_ADVSW_DLR_TICKS           200     ///< ticks for 1 us


#define RES_OK(x) (x == FSP_SUCCESS)      ///< positive result verification
#define RES_ERR(x) (x != FSP_SUCCESS)     ///< negative result verification

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
/** MAC table entry(for register access) */
typedef __packed struct st_ether_switch_mactab_entry_reg
{
    ether_switch_mac_addr_t     mac;            ///< MAC address
    uint16_t                    mask_and_flags; ///< Port bit mask and flags
} ether_switch_mactab_entry_reg_t;

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void ether_switch_enable_icu (ether_switch_instance_ctrl_t * const p_instance_ctrl);
static void ether_switch_disable_icu (ether_switch_instance_ctrl_t * const p_instance_ctrl);

#ifdef BSP_PLAT_RZN1SDB /* RZ/N1S */
static void R_BSP_IrqCfgEnable (IRQn_Type const irq, uint32_t priority, void * p_context);
static void * R_FSP_IsrContextGet (IRQn_Type const irq);
static int RegisterInterrupt (IRQn_Type irq, uint32_t priority);
#endif /* BSP_PLAT_RZN1SDB */

static void ether_switch_memcpy_vol(volatile void *p_dst, volatile const void *p_src, size_t cnt);
static void ether_switch_memset_vol(volatile void *p_data, uint8_t chr, size_t cnt);

fsp_err_t ether_switch_mac_tab_set(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_mac_tab_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_mac_tab_conf_set(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_mac_tab_conf_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_mac_tab_clear(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_link_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_admin_state_set(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_admin_state_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);

static fsp_err_t ether_switch_vlan_def_set(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_vlan_def_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_vlan_port_add(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_vlan_port_rem(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_vlan_in_mode_set(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_vlan_in_mode_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_vlan_out_mode_set(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_vlan_out_mode_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_vlan_table_cnt(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_vlan_table_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_vlan_verify_set(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_vlan_verify_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_vlan_discunk_set(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_vlan_discunk_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_learning(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_port_fwd(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);

#ifdef ETHER_SWITCH_CONFIG_PTP
static fsp_err_t ether_switch_rx_port_timer(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
    static fsp_err_t ether_switch_ptp_init(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
    static fsp_err_t ether_switch_latency_egress_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
    static fsp_err_t ether_switch_latency_ingress_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
    static fsp_err_t ether_switch_timer_active(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
    fsp_err_t ether_switch_timer_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
    static fsp_err_t ether_switch_ptp_domain(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
    static fsp_err_t ether_switch_ptp_peerdelay(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
    fsp_err_t ether_switch_ptp_callback(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
    static void ether_switch_isr_tsm(
                        ether_switch_instance_ctrl_t * p_instance_ctrl);
#endif /* ETHER_SWITCH_CONFIG_PTP */

#ifdef ETHER_SWITCH_CONFIG_TDMA
    static fsp_err_t ether_switch_tdma_enable_set(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
    static fsp_err_t ether_switch_tdma_enable_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
    static fsp_err_t ether_switch_tdma_tcv_seq_entry_set(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
    static fsp_err_t ether_switch_tdma_tcv_seq_entry_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
    static fsp_err_t ether_switch_tdma_tcv_dat_entry_set(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
    static fsp_err_t ether_switch_tdma_tcv_dat_entry_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
    static fsp_err_t ether_switch_tdma_tcv_seq_range_set(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
    static fsp_err_t ether_switch_tdma_tcv_seq_range_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
    static fsp_err_t ether_switch_mmctl_qgate_set(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
    static void ether_switch_isr_tdma(
                        ether_switch_instance_ctrl_t * p_instance_ctrl);
#endif /* ETHER_SWITCH_CONFIG_TDMA */

#ifdef ETHER_SWITCH_CONFIG_PRIORITY3_OTHER
static fsp_err_t ether_switch_mirror_set(R_ETHSW_Type volatile *p_switch_reg,
                                         ether_switch_cmd_t *   p_cmd);
static fsp_err_t ether_switch_mirror_get(R_ETHSW_Type volatile *p_switch_reg,
                                         ether_switch_cmd_t *   p_cmd);
fsp_err_t ether_switch_qos_mode_set(R_ETHSW_Type volatile *  p_switch_reg,
                                           ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_qos_mode_get(R_ETHSW_Type volatile *  p_switch_reg,
                                           ether_switch_cmd_t *     p_cmd);
fsp_err_t ether_switch_qos_prio_vlan_set(R_ETHSW_Type volatile * p_switch_reg,
                                                ether_switch_cmd_t *    p_cmd);
static fsp_err_t ether_switch_qos_prio_vlan_get(R_ETHSW_Type volatile * p_switch_reg,
                                                ether_switch_cmd_t *    p_cmd);
static fsp_err_t ether_switch_qos_prio_ip_set(R_ETHSW_Type volatile *   p_switch_reg,
                                              ether_switch_cmd_t *      p_cmd);
static fsp_err_t ether_switch_qos_prio_ip_get(R_ETHSW_Type volatile *   p_switch_reg,
                                              ether_switch_cmd_t *      p_cmd);
static fsp_err_t ether_switch_qos_prio_type_set(R_ETHSW_Type volatile * p_switch_reg,
                                                ether_switch_cmd_t *    p_cmd);
static fsp_err_t ether_switch_qos_prio_type_get(R_ETHSW_Type volatile * p_switch_reg,
                                                ether_switch_cmd_t *    p_cmd);
static fsp_err_t ether_switch_out_queues_get(R_ETHSW_Type volatile *p_switch_reg,
                                             ether_switch_cmd_t *   p_cmd);
static fsp_err_t ether_switch_disc_unk_set(R_ETHSW_Type volatile *  p_switch_reg,
                                           ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_disc_unk_get(R_ETHSW_Type volatile *  p_switch_reg,
                                           ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_flood_unk_dest_set(R_ETHSW_Type volatile *p_switch_reg,
                                                 ether_switch_cmd_t *   p_cmd);
static fsp_err_t ether_switch_flood_unk_dest_get(R_ETHSW_Type volatile *p_switch_reg,
                                                 ether_switch_cmd_t *   p_cmd);
#endif /* ETHER_SWITCH_CONFIG_PRIORITY3_OTHER */

#ifdef ETHER_SWITCH_CONFIG_PRIORITY4
static fsp_err_t ether_switch_storm_time_set(R_ETHSW_Type volatile * p_switch_reg,
                                             ether_switch_cmd_t * p_cmd);
static fsp_err_t ether_switch_storm_time_get(R_ETHSW_Type volatile * p_switch_reg,
                                             ether_switch_cmd_t * p_cmd);
static fsp_err_t ether_switch_bcast_limit_set(R_ETHSW_Type volatile * p_switch_reg,
                                              ether_switch_cmd_t * p_cmd);
static fsp_err_t ether_switch_bcast_limit_get(R_ETHSW_Type volatile * p_switch_reg,
                                              ether_switch_cmd_t * p_cmd);
static fsp_err_t ether_switch_mcast_limit_set(R_ETHSW_Type volatile * p_switch_reg,
                                              ether_switch_cmd_t * p_cmd);
static fsp_err_t ether_switch_mcast_limit_get(R_ETHSW_Type volatile * p_switch_reg,
                                              ether_switch_cmd_t * p_cmd);
static fsp_err_t ether_switch_tx_rate_set(R_ETHSW_Type volatile * p_switch_reg,
                                          ether_switch_cmd_t * p_cmd);
static fsp_err_t ether_switch_tx_rate_get(R_ETHSW_Type volatile * p_switch_reg,
                                          ether_switch_cmd_t * p_cmd);
#endif  /* ETHER_SWITCH_CONFIG_PRIORITY4 */

#ifdef ETHER_SWITCH_CONFIG_PRIORITY5
static fsp_err_t ether_switch_eee_set(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_eee_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);

#ifdef ETHER_SWITCH_CONFIG_SNOOP
static fsp_err_t ether_switch_snoop_add(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_snoop_rem(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_snoop_free_gparser_get(
                        R_ETHSW_Type volatile * p_switch_reg,
                        uint32_t                block,
                        uint32_t *              pId );
static fsp_err_t ether_switch_gparser_set(
                        R_ETHSW_Type volatile * p_switch_reg,
                        uint8_t                 parserid,
                        ether_switch_bool_t     enable,
                        uint32_t                mode,
                        uint16_t                compval,
                        uint8_t                 mask,
                        uint16_t                offset);
fsp_err_t ether_switch_snoop_init(
                        R_ETHSW_Type volatile * p_switch_reg,
                        ether_switch_snoop_action_t mode1,
                        ether_switch_snoop_action_t mode2);
#endif /* ETHER_SWITCH_CONFIG_SNOOP */

#ifdef ETHER_SWITCH_CONFIG_BRIDGE
static fsp_err_t ether_switch_auth(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_controlled_direction(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_eapol_reception(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_bpdu_forwarding(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
#endif /* ETHER_SWITCH_CONFIG_BRIDGE */

#ifdef ETHER_SWITCH_CONFIG_DLR
static fsp_err_t ether_switch_dlr_init(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_dlr_tgl(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_dlr_status(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_dlr_supervisor_mac(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_dlr_forward_enable(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
void ether_switch_isr_dlr (ether_switch_instance_ctrl_t * p_instance_ctrl);
#endif /* ETHER_SWITCH_CONFIG_DLR */

static fsp_err_t ether_switch_frame_size_set(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);
static fsp_err_t ether_switch_frame_size_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd);

#endif  /* ETHER_SWITCH_CONFIG_PRIORITY5 */



/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

#if defined(__GNUC__)

/* This structure is affected by warnings from a GCC compiler bug. This pragma suppresses the warnings in this
 * structure only.*/

/*LDRA_INSPECTED 69 S */
 #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

/** ETHER SWITCH HAL API mapping for Ethernet Controller interface */
/*LDRA_INSPECTED 27 D This structure must be accessible in user code. It cannot be static. */
const ether_switch_api_t g_ether_switch_on_ether =
{
    .open               = R_ETHER_SWITCH_Open,
    .close              = R_ETHER_SWITCH_Close,
    .confSpeed          = R_ETHER_SWITCH_ConfSpeed,
    .control            = R_ETHER_SWITCH_Control
};

#ifdef BSP_PLAT_RZN1SDB /* RZ/N1S */
    static uint32_t portHost    = 4;                ///< host port
    static uint16_t switchPorts = 5;                ///< switch port count
#else /* !BSP_PLAT_RZN1SDB (RZ/T2) */
    static uint32_t portHost    = 3;                ///< host port
    static uint16_t switchPorts = 4;                ///< switch port count
#endif /* BSP_PLAT_RZN1SDB */

#ifdef ETHER_SWITCH_CONFIG_PTP
    uint8_t time_stamp_current_idx=0;
	  uint32_t *  ptp_time_stamp_nanos[2*NUM_TX_DESCRIPTOR] = {0};
    void (* p_ptp_callback)(                        ///< pointer of callback function called when an PTP Interupt occurs
                    ether_switch_ptp_event_t   event,
                    uint8_t                    port,
                    uint32_t                   nanos,
                    ether_switch_timestamp_t * p_timestamp);
#endif /* ETHER_SWITCH_CONFIG_PTP */

#ifdef ETHER_SWITCH_CONFIG_TDMA
    void (* p_tdma_callback)(                       ///< pointer of callback function called when an TDMA Interupt occurs
                    ether_switch_tdma_event_t           event,
                    ether_switch_tdma_callback_data_t * p_tdma_data);
#endif /* ETHER_SWITCH_CONFIG_TDMA */

#ifdef ETHER_SWITCH_CONFIG_PRIORITY5
  #ifdef ETHER_SWITCH_CONFIG_DLR
    void (* p_dlr_callback)(                        ///< pointer of callback function called when an DLR Interupt occurs
                    ether_switch_dlr_event_t    event,
                    uint32_t                    port);
  #endif /* ETHER_SWITCH_CONFIG_DLR */

  #ifdef ETHER_SWITCH_CONFIG_SNOOP
    static ether_switch_snoop_action_t block1Mode;  ///< Snoop mode of parser block 1
    static ether_switch_snoop_action_t block2Mode;  ///< Snoop mode of parser block 2
  #endif /* ETHER_SWITCH_CONFIG_SNOOP */
#endif /* ETHER_SWITCH_CONFIG_PRIORITY5 */

/*******************************************************************************************************************//**
 * @addtogroup ETHER_SWITCH
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

    
void R_ETHER_SWITCH_Enable_Link_Interrupt (uint32_t portmap)
{
    fsp_err_t               err             = FSP_SUCCESS;
    R_ETHSW_Type          * p_reg_switch;
    p_reg_switch = (void *)R_ETHSW;
    p_reg_switch->INT_CONFIG_b.IRQ_LINK = portmap;
           
}
        
/********************************************************************************************************************//**
 * @brief Open the switch ports and gets it ready to pass through the frames.
 *
 * @retval  FSP_SUCCESS                             Channel opened successfully.
 * @retval  FSP_ERR_ASSERTION                       Pointer to ETHER SWITCH control block or configuration structure is NULL.
 * @retval  FSP_ERR_ALREADY_OPEN                    Control block has already been opened or channel is being used by another
 *                                                  instance. Call close() then open() to reconfigure.
 * @retval  FSP_ERR_INVALID_CHANNEL                 Invalid channel number is given.
 * @retval  FSP_ERR_INVALID_ARGUMENT                Interrupt is not enabled.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_SWITCH_Open (ether_switch_ctrl_t * const p_ctrl, ether_switch_cfg_t const * const p_cfg)
{
    fsp_err_t               err             = FSP_SUCCESS;
    ether_switch_instance_ctrl_t * p_instance_ctrl
                                            = (ether_switch_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type          * p_reg_switch;

#if (ETHER_SWITCH_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_SWITCH_ERROR_RETURN((NULL != p_cfg), FSP_ERR_INVALID_POINTER);
    ETHER_SWITCH_ERROR_RETURN((BSP_FEATURE_ETHER_SWITCH_MAX_CHANNELS > p_cfg->channel), FSP_ERR_INVALID_CHANNEL);
    ETHER_SWITCH_ERROR_RETURN((0 <= p_cfg->irq), FSP_ERR_INVALID_ARGUMENT);

    ETHER_SWITCH_ERROR_RETURN((ETHER_SWITCH_OPEN != p_instance_ctrl->open), FSP_ERR_ALREADY_OPEN);
#endif

    p_instance_ctrl->p_reg_switch = (void *)R_ETHSW;

    p_reg_switch = (R_ETHSW_Type *)p_instance_ctrl->p_reg_switch;

    /* Initialize configuration of Ethernet switch module. */
    p_instance_ctrl->p_switch_cfg = p_cfg;

#ifdef RZT2_RSK
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_RESET);
    R_BSP_MODULE_START(FSP_IP_ETHSW, p_cfg->channel);
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_RESET);    
#else /* !RZT2_RSK */
    R_BSP_MODULE_START(FSP_IP_ETHER�QSWITCH, p_instance_ctrl->p_switch_cfg->channel);
#endif

    /* set Maximum Frame Length size */
    p_reg_switch->FRM_LENGTH_P0_b.FRM_LENGTH = FRAME_SIZE;
    p_reg_switch->FRM_LENGTH_P1_b.FRM_LENGTH = FRAME_SIZE;
    p_reg_switch->FRM_LENGTH_P2_b.FRM_LENGTH = FRAME_SIZE;
    p_reg_switch->FRM_LENGTH_P3_b.FRM_LENGTH = FRAME_SIZE;

#ifdef BSP_PLAT_RZN1SDB /* RZ/N1S */
    p_reg_switch->FRM_LENGTH_P4_b.FRM_LENGTH = FRAME_SIZE;
#endif /* BSP_PLAT_RZN1SDB */

#ifdef PHY_100MBPS_LIMIT
    /* Initialize COMMAND_CONFIG_p[0] Reg */
    p_reg_switch->COMMAND_CONFIG_P0 = (COMMAND_CONFIG_NO_LGTH_CHECK | COMMAND_CONFIG_TX_CRC_APPEND | 
                                                                  COMMAND_CONFIG_RX_ENA | COMMAND_CONFIG_TX_ENA);

	p_reg_switch->COMMAND_CONFIG_P0_b.TIMER_SEL = 0; //TODO: -OS- remove
    /* Initialize COMMAND_CONFIG_p[1] Reg */
    p_reg_switch->COMMAND_CONFIG_P1 = (COMMAND_CONFIG_NO_LGTH_CHECK | COMMAND_CONFIG_TX_CRC_APPEND | 
                                                                  COMMAND_CONFIG_RX_ENA | COMMAND_CONFIG_TX_ENA);

    /* Initialize COMMAND_CONFIG_p[2] Reg */
    p_reg_switch->COMMAND_CONFIG_P2 = (COMMAND_CONFIG_NO_LGTH_CHECK | COMMAND_CONFIG_TX_CRC_APPEND | 
                                                                  COMMAND_CONFIG_RX_ENA | COMMAND_CONFIG_TX_ENA);

    /* Initialize COMMAND_CONFIG_p[3] Reg */
    p_reg_switch->COMMAND_CONFIG_P3 = (COMMAND_CONFIG_NO_LGTH_CHECK | COMMAND_CONFIG_TX_CRC_APPEND | 
                                                                  COMMAND_CONFIG_RX_ENA | COMMAND_CONFIG_TX_ENA);
#else /* !PHY_100MBPS_LIMIT */
	p_reg_switch->COMMAND_CONFIG_P0_b.TIMER_SEL = 0; //TODO: -OS- remove
    /* Initialize COMMAND_CONFIG_p[0] Reg */
    p_reg_switch->COMMAND_CONFIG_P0 = (COMMAND_CONFIG_NO_LGTH_CHECK | COMMAND_CONFIG_TX_CRC_APPEND | 
                                       COMMAND_CONFIG_ETH_SPEED | COMMAND_CONFIG_RX_ENA | COMMAND_CONFIG_TX_ENA);

    /* Initialize COMMAND_CONFIG_p[1] Reg */
    p_reg_switch->COMMAND_CONFIG_P1 = (COMMAND_CONFIG_NO_LGTH_CHECK | COMMAND_CONFIG_TX_CRC_APPEND | 
                                       COMMAND_CONFIG_ETH_SPEED | COMMAND_CONFIG_RX_ENA | COMMAND_CONFIG_TX_ENA);

    /* Initialize COMMAND_CONFIG_p[2] Reg */
    p_reg_switch->COMMAND_CONFIG_P2 = (COMMAND_CONFIG_NO_LGTH_CHECK | COMMAND_CONFIG_TX_CRC_APPEND | 
                                       COMMAND_CONFIG_ETH_SPEED | COMMAND_CONFIG_RX_ENA | COMMAND_CONFIG_TX_ENA);

    /* Initialize COMMAND_CONFIG_p[3] Reg */
    p_reg_switch->COMMAND_CONFIG_P3 = (COMMAND_CONFIG_NO_LGTH_CHECK | COMMAND_CONFIG_TX_CRC_APPEND | 
                                       COMMAND_CONFIG_ETH_SPEED | COMMAND_CONFIG_RX_ENA | COMMAND_CONFIG_TX_ENA);
#endif /* !PHY_100MBPS_LIMIT */

#ifdef BSP_PLAT_RZN1SDB /* RZ/N1S */
    /* Initialize COMMAND_CONFIG_p[4] Reg */
    p_reg_switch->COMMAND_CONFIG_P4 = (COMMAND_CONFIG_NO_LGTH_CHECK | COMMAND_CONFIG_TX_CRC_APPEND | 
                                       COMMAND_CONFIG_ETH_SPEED | COMMAND_CONFIG_RX_ENA | COMMAND_CONFIG_TX_ENA);
#endif /* BSP_PLAT_RZN1SDB */

    /* Initialize PORT ENABLE Reg */
#ifdef BSP_PLAT_RZN1SDB /* RZ/N1S */
    p_reg_switch->PORT_ENA_b.RXENA = (ENABLE_PORT0 | ENABLE_PORT1 | ENABLE_PORT2 | ENABLE_PORT3 | ENABLE_PORT4);
    p_reg_switch->PORT_ENA_b.TXENA = (ENABLE_PORT0 | ENABLE_PORT1 | ENABLE_PORT2 | ENABLE_PORT3 | ENABLE_PORT4);
#else /* !BSP_PLAT_RZN1SDB (RZ/T2) */
    p_reg_switch->PORT_ENA_b.RXENA = (ENABLE_PORT0 | ENABLE_PORT1 | ENABLE_PORT2 | ENABLE_PORT3);
    p_reg_switch->PORT_ENA_b.TXENA = (ENABLE_PORT0 | ENABLE_PORT1 | ENABLE_PORT2 | ENABLE_PORT3);
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */

    /* Initialize Learning/Lookup function global configuration Reg */
    p_reg_switch->LK_CTRL                   = 0;
    p_reg_switch->LK_CTRL_b.ALW_MGRT = 1;
    p_reg_switch->LK_CTRL_b.AGING_EN    = 1;
    p_reg_switch->LK_CTRL_b.LEARN_EN = 1;
    p_reg_switch->LK_CTRL_b.LKUP_EN   = 1;

#ifdef BSP_PLAT_RZN1SDB /* RZ/N1S */
    /* enable Default unicast resolution */
    p_reg_switch->UCAST_DEFAULT_MASK_b.UCASTDM = (ENABLE_PORT0 | ENABLE_PORT1 | ENABLE_PORT2 | ENABLE_PORT3 | ENABLE_PORT4);

    /* enable Default broadcast resolution */
    p_reg_switch->BCAST_DEFAULT_MASK_b.BCASTDM = (ENABLE_PORT0 | ENABLE_PORT1 | ENABLE_PORT2 | ENABLE_PORT3 | ENABLE_PORT4);

    /* enable Default multicast resolution */
    p_reg_switch->MCAST_DEFAULT_MASK_b.MCASTDM = (ENABLE_PORT0 | ENABLE_PORT1 | ENABLE_PORT2 | ENABLE_PORT3 | ENABLE_PORT4);
#else /* !BSP_PLAT_RZN1SDB (RZ/T2) */
    /* enable Default unicast resolution */
    p_reg_switch->UCAST_DEFAULT_MASK0_b.UCASTDM = (ENABLE_PORT0 | ENABLE_PORT1 | ENABLE_PORT2 | ENABLE_PORT3);

    /* enable Default broadcast resolution */
    p_reg_switch->BCAST_DEFAULT_MASK0_b.BCASTDM = (ENABLE_PORT0 | ENABLE_PORT1 | ENABLE_PORT2 | ENABLE_PORT3);

    /* enable Default multicast resolution */
    p_reg_switch->MCAST_DEFAULT_MASK0_b.MCASTDM = (ENABLE_PORT0 | ENABLE_PORT1 | ENABLE_PORT2 | ENABLE_PORT3);
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */

    /* set port in authorized state(Port 0 authentication control and configuration Reg) */
    p_reg_switch->AUTH_PORT_b[0].AUTH = 1;

    /* set port in authorized state(Port 1 authentication control and configuration Reg) */
    p_reg_switch->AUTH_PORT_b[1].AUTH = 1;

    /* set port in authorized state(Port 2 authentication control and configuration Reg) */
    p_reg_switch->AUTH_PORT_b[2].AUTH = 1;

    /* set port in authorized state(Port 3 authentication control and configuration Reg) */
    p_reg_switch->AUTH_PORT_b[3].AUTH = 1;

#ifdef BSP_PLAT_RZN1SDB /* RZ/N1S */
    /* set port in authorized state(Port 4 authentication control and configuration Reg) */
    p_reg_switch->AUTH_PORT4_b.authorized = 1;
#endif /* BSP_PLAT_RZN1SDB */

    /* Initialize MGMT_TAG_CONFIG Reg */
    p_reg_switch->MGMT_TAG_CONFIG_b.TAGFIELD     = TAGFIELD_DEFAULT;
    p_reg_switch->MGMT_TAG_CONFIG_b.TYPE2_EN = 0;
    p_reg_switch->MGMT_TAG_CONFIG_b.TYPE1_EN = 0;
    p_reg_switch->MGMT_TAG_CONFIG_b.AL_FRAMES   = 1;
    p_reg_switch->MGMT_TAG_CONFIG_b.ENABLE       = 1;


#ifdef ETHER_SWITCH_CONFIG_PRIORITY5
  #ifdef ETHER_SWITCH_CONFIG_SNOOP
    err = ether_switch_snoop_init(p_reg_switch, ETHER_SWITCH_SNOOP_ACTION_CAPTURE, ETHER_SWITCH_SNOOP_ACTION_CPY_FW);
    if(err != FSP_SUCCESS)
    {
        return err;
    }
  #endif /* ETHER_SWITCH_CONFIG_SNOOP */
#endif /* ETHER_SWITCH_CONFIG_PRIORITY5 */

    /* Set Ethernet Switch interrupt level and enable */
    ether_switch_enable_icu(p_instance_ctrl);

#ifdef BSP_PLAT_RZN1SDB /* RZ/N1S */
    err  = RegisterInterrupt (p_instance_ctrl->p_switch_cfg->irq, 
                                      p_instance_ctrl->p_switch_cfg->interrupt_priority);
#endif /* BSP_PLAT_RZN1SDB */

    if (NULL != p_instance_ctrl->p_switch_cfg->p_callback)
    {
        /* Set line port phy link change interrupt enable Bit (PORT0�`2) */
        p_reg_switch->INT_CONFIG_b.IRQ_LINK = (ENABLE_PORT0 | ENABLE_PORT1 | ENABLE_PORT2);

        /* Set Interrupt global enable */
        p_reg_switch->INT_CONFIG_b.IRQ_EN   = 1;
    }

    /* Set up for Profinet usage. This should probably be put to the EDD ater, but for testing leave it here */
//    start_profinet();
// if start_profinet is not used need to start timer manually
/* SG: replaced by reset function call
	  p_reg_switch->ATIME_SEC0 = 0x0;      // Reset timer     
	  p_reg_switch->ATIME0 = 0x0;          // Restet timer
	  p_reg_switch->ATIME_INC0 = 0x08;     // Enable timer,    
	  p_reg_switch->ATIME_CTRL0 = 0x01;    // Enable timer,  	
      
#if 0      
          p_reg_switch->ATIME_SEC1 = 0x0;      // Reset timer     
	  p_reg_switch->ATIME1 = 0x0;          // Restet timer
	  p_reg_switch->ATIME_INC1 = 0x08;     // Enable timer,    
	  p_reg_switch->ATIME_CTRL1 = 0x01;    // Enable timer,  		  
#endif
 */   
    ETHSW_ResetTimer(ETHSW_TIMER0);      
    ETHSW_ResetTimer(ETHSW_TIMER1);           
    
    p_instance_ctrl->open = ETHER_SWITCH_OPEN;

    return err;
}                                      /* End of function R_ETHER_SWITCH_Open() */

/********************************************************************************************************************//**
 * @brief Close the switch ports
 *
 * @retval  FSP_SUCCESS                                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened
 *
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_SWITCH_Close (ether_switch_ctrl_t * const p_ctrl)
{
    fsp_err_t               err = FSP_SUCCESS;
    ether_switch_instance_ctrl_t * p_instance_ctrl = (ether_switch_instance_ctrl_t *) p_ctrl;

    R_ETHSW_Type          * p_reg_switch;

    p_reg_switch = (R_ETHSW_Type *)p_instance_ctrl->p_reg_switch;

#if (ETHER_SWITCH_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_SWITCH_ERROR_RETURN(ETHER_SWITCH_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Set line port phy link change interrupt disable Bit (PORT0�`2) */
    p_reg_switch->INT_CONFIG_b.IRQ_LINK   = 0;

    /* Set Interrupt global disable */
    p_reg_switch->INT_CONFIG_b.IRQ_EN     = 0;

    /* set port in unauthorized state(Port 0 authentication control and configuration Reg) */
    p_reg_switch->AUTH_PORT_b[0].AUTH = 0;

    /* set port in unauthorized state(Port 1 authentication control and configuration Reg) */
    p_reg_switch->AUTH_PORT_b[1].AUTH = 0;

    /* set port in unauthorized state(Port 2 authentication control and configuration Reg) */
    p_reg_switch->AUTH_PORT_b[2].AUTH = 0;

    /* set port in unauthorized state(Port 3 authentication control and configuration Reg) */
    p_reg_switch->AUTH_PORT_b[3].AUTH = 0;

    /* clear Maximum Frame Length Value */
    p_reg_switch->FRM_LENGTH_P0_b.FRM_LENGTH = FRAME_SIZE;
    p_reg_switch->FRM_LENGTH_P1_b.FRM_LENGTH = FRAME_SIZE;
    p_reg_switch->FRM_LENGTH_P2_b.FRM_LENGTH = FRAME_SIZE;
    p_reg_switch->FRM_LENGTH_P3_b.FRM_LENGTH = FRAME_SIZE;

    /* Disable PORT Rx Tx */
    p_reg_switch->PORT_ENA_b.RXENA = 0U;
    p_reg_switch->PORT_ENA_b.TXENA = 0U;
    
    /* Disable the MAC reception function(COMMAND_CONFIG_p[1] Reg) */
    p_reg_switch->COMMAND_CONFIG_P0 = (COMMAND_CONFIG_NO_LGTH_CHECK | COMMAND_CONFIG_TX_CRC_APPEND);

    /* Disable the MAC reception function(COMMAND_CONFIG_p[2] Reg) */
    p_reg_switch->COMMAND_CONFIG_P1 = (COMMAND_CONFIG_NO_LGTH_CHECK | COMMAND_CONFIG_TX_CRC_APPEND);

    /* Disable the MAC reception function(COMMAND_CONFIG_p[2] Reg) */
    p_reg_switch->COMMAND_CONFIG_P2 = (COMMAND_CONFIG_NO_LGTH_CHECK | COMMAND_CONFIG_TX_CRC_APPEND);

    /* Disable the MAC reception function(COMMAND_CONFIG_p[3] Reg) */
    p_reg_switch->COMMAND_CONFIG_P3 = (COMMAND_CONFIG_NO_LGTH_CHECK | COMMAND_CONFIG_TX_CRC_APPEND);
   
    /* Disable Ethernet interrupt. */
    ether_switch_disable_icu(p_instance_ctrl);

#ifdef RZT2_RSK
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_RESET);
    R_BSP_MODULE_STOP(FSP_IP_ETHSW, p_cfg->channel);
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_RESET);
#else /* !RZT2_RSK */
    R_BSP_MODULE_STOP(FSP_IP_ETHER�QSWITCH, p_instance_ctrl->p_switch_cfg->channel);
#endif

    /** Clear configure block parameters. */
    p_instance_ctrl->p_switch_cfg = NULL;

    /** Mark the channel not open so other APIs cannot use it. */
    p_instance_ctrl->open = 0U;

    return err;
}                                      /* End of function R_ETHER_Close() */

/********************************************************************************************************************//**
 * @brief Configs speed and duplex of the port
 *
 * @retval  FSP_SUCCESS                                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_ARGUMENT                    Invalid input parameter.
 *
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_SWITCH_ConfSpeed (ether_switch_ctrl_t *     const p_ctrl, 
                                    ether_switch_port_t       const port,
                                    ether_switch_link_speed_t const speed)
{
    ether_switch_instance_ctrl_t * p_instance_ctrl = (ether_switch_instance_ctrl_t *) p_ctrl;
    fsp_err_t                      err             = FSP_SUCCESS;
    R_ETHSW_Type volatile        * p_reg_switch;
    uint32_t volatile            * p_reg_command_config;
    uint32_t                       command_config;

#if (ETHER_SWITCH_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_SWITCH_ERROR_RETURN(ETHER_SWITCH_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    p_reg_switch = (R_ETHSW_Type *)p_instance_ctrl->p_reg_switch;

    switch (port)
    {
        case ETHER_SWITCH_PORT_0:
            p_reg_command_config = (uint32_t volatile *)&p_reg_switch->COMMAND_CONFIG_P0;
            break;

        case ETHER_SWITCH_PORT_1:
            p_reg_command_config = (uint32_t volatile *)&p_reg_switch->COMMAND_CONFIG_P1;
            break;

        case ETHER_SWITCH_PORT_2:
            p_reg_command_config = (uint32_t volatile *)&p_reg_switch->COMMAND_CONFIG_P2;
            break;

        default:
            err = FSP_ERR_INVALID_ARGUMENT;
            break;
    }

    if (err == FSP_SUCCESS)
    {
        command_config = *p_reg_command_config;

        command_config &= ~(COMMAND_CONFIG_ENA_10
                           |COMMAND_CONFIG_HD_ENA
                           |COMMAND_CONFIG_ETH_SPEED);
        switch (speed)
        {
            case ETHER_SWITCH_LINK_SPEED_10H:
                command_config |= (COMMAND_CONFIG_ENA_10
                                  |COMMAND_CONFIG_HD_ENA);
                break;

            case ETHER_SWITCH_LINK_SPEED_10F:
                command_config |= (COMMAND_CONFIG_ENA_10);
                break;

            case ETHER_SWITCH_LINK_SPEED_100H:
                command_config |= (COMMAND_CONFIG_HD_ENA);
                break;

            case ETHER_SWITCH_LINK_SPEED_100F:
                break;

            case ETHER_SWITCH_LINK_SPEED_1000H:
                command_config |= (COMMAND_CONFIG_HD_ENA
                                  |COMMAND_CONFIG_ETH_SPEED);
                break;

            case ETHER_SWITCH_LINK_SPEED_1000F:
                command_config |= (COMMAND_CONFIG_ETH_SPEED);
                break;

            default:
                err = FSP_ERR_INVALID_ARGUMENT;
                break;
        }
        *p_reg_command_config = command_config;
    }
    return err;
}

/********************************************************************************************************************//**
 * @brief Control switch parameter
 *
 * @retval  FSP_SUCCESS                                 Command successfully.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_ARGUMENT                    Invalid input parameter.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_SWITCH_Control (ether_switch_ctrl_t *     const p_ctrl, 
                                  ether_switch_cmd_t *      p_cmd)
{
    ether_switch_instance_ctrl_t * p_instance_ctrl = (ether_switch_instance_ctrl_t *) p_ctrl;
    fsp_err_t                      err             = FSP_SUCCESS;
    R_ETHSW_Type volatile        * p_reg_switch;

#if (ETHER_SWITCH_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_SWITCH_ERROR_RETURN((NULL != p_cmd), FSP_ERR_INVALID_ARGUMENT);

    ETHER_SWITCH_ERROR_RETURN(ETHER_SWITCH_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    p_reg_switch = (R_ETHSW_Type *)p_instance_ctrl->p_reg_switch;

    switch (p_cmd->cmd)
    {
        case ETHER_SWITCH_CMD_PORT_COUNT :
            *p_cmd->data.p_cnt_ports = ETHER_SWITCH_PORT_COUNT;
            break;

        case ETHER_SWITCH_CMD_MACTAB_GET :
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = FSP_ERR_INVALID_ARGUMENT;
            }
            else
            {
                err = ether_switch_mac_tab_get(p_reg_switch, p_cmd);
            }
            break;

        case ETHER_SWITCH_CMD_MACTAB_SET :
            err = ether_switch_mac_tab_set(p_reg_switch, p_cmd);
            break;

        case ETHER_SWITCH_CMD_MACTAB_CONF :
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = ether_switch_mac_tab_conf_set(p_reg_switch, p_cmd);
            }
            else
            {
                err = ether_switch_mac_tab_conf_get(p_reg_switch, p_cmd);
            }
            break;

        case ETHER_SWITCH_CMD_MACTAB_CLR :
        case ETHER_SWITCH_CMD_MAC_UNI_FLUSH :
        case ETHER_SWITCH_CMD_MAC_MULTI_FLUSH :
            err = ether_switch_mac_tab_clear(p_reg_switch, p_cmd);
            break;

        case ETHER_SWITCH_CMD_LINK_STATE :
            err = ether_switch_link_get(p_reg_switch, p_cmd);
            break;

        case ETHER_SWITCH_CMD_PORT_ADMIN_STATE :
            if (p_cmd->flg_set == ETHER_SWITCH_TRUE)
            {
                err = ether_switch_admin_state_set(p_reg_switch, p_cmd);
            }
            else
            {
                err = ether_switch_admin_state_get(p_reg_switch, p_cmd);
            }
            break;

        case ETHER_SWITCH_CMD_VLAN_DEF :
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = ether_switch_vlan_def_set(p_reg_switch, p_cmd);
            }
            else
            {
                err = ether_switch_vlan_def_get(p_reg_switch, p_cmd);
            }
            break;

        case ETHER_SWITCH_CMD_VLAN_PORT_ADD :
            err = ether_switch_vlan_port_add(p_reg_switch, p_cmd);
            break;

        case ETHER_SWITCH_CMD_VLAN_PORT_REM :
            err = ether_switch_vlan_port_rem(p_reg_switch, p_cmd);
            break;

        case ETHER_SWITCH_CMD_VLAN_MODE_IN :
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = ether_switch_vlan_in_mode_set(p_reg_switch, p_cmd);
            }
            else
            {
                err = ether_switch_vlan_in_mode_get(p_reg_switch, p_cmd);
            }
            break;

        case ETHER_SWITCH_CMD_VLAN_MODE_OUT :
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = ether_switch_vlan_out_mode_set(p_reg_switch, p_cmd);
            }
            else
            {
                err = ether_switch_vlan_out_mode_get(p_reg_switch, p_cmd);
            }
            break;

        case ETHER_SWITCH_CMD_VLAN_TABLE_CNT :
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = FSP_ERR_UNSUPPORTED;
            }
            else
            {
                err = ether_switch_vlan_table_cnt(p_reg_switch, p_cmd);
            }
            break;

        case ETHER_SWITCH_CMD_VLAN_TABLE_GET :
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = FSP_ERR_UNSUPPORTED;
            }
            else
            {
                err = ether_switch_vlan_table_get(p_reg_switch, p_cmd);
            }
            break;

        case ETHER_SWITCH_CMD_VLAN_VERIFY :
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = ether_switch_vlan_verify_set(p_reg_switch, p_cmd);
            }
            else
            {
                err = ether_switch_vlan_verify_get(p_reg_switch, p_cmd);
            }
            break;

        case ETHER_SWITCH_CMD_VLAN_DISCUNK :
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = ether_switch_vlan_discunk_set(p_reg_switch, p_cmd);
            }
            else
            {
                err = ether_switch_vlan_discunk_get(p_reg_switch, p_cmd);
            }
            break;

        case ETHER_SWITCH_CMD_LEARNING :
            err = ether_switch_learning(p_reg_switch, p_cmd);
            break;

        case ETHER_SWITCH_CMD_PORT_FWD_ADD :
        case ETHER_SWITCH_CMD_PORT_FWD_DEL :
            err = ether_switch_port_fwd(p_reg_switch, p_cmd);
            break;

#ifdef ETHER_SWITCH_CONFIG_PTP
        case ETHER_SWITCH_CMD_TIMESTAMP :               ///< Enables / disables the send time stamp of the external port.
            err = ether_switch_ptp_init(p_reg_switch, p_cmd);
            break;

       case ETHER_SWITCH_CMD_TIMESTAMP_RX :               ///< Axel select timer for RX timestamping for each port
            err = ether_switch_rx_port_timer(p_reg_switch, p_cmd);
            break;

        case ETHER_SWITCH_CMD_LATENCY_EGRESS :            ///< Gets the latency from timestamping the transmission frame to transmission by the PHY.
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = FSP_ERR_INVALID_ARGUMENT;
            }
            else
            {
                err = ether_switch_latency_egress_get(p_reg_switch, p_cmd);
            }
            break;

        case ETHER_SWITCH_CMD_LATENCY_INGRESS :           ///< Gets the latency from when the PHY receives the received frame until it is time stamped.
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = FSP_ERR_INVALID_ARGUMENT;
            }
            else
            {
                err = ether_switch_latency_ingress_get(p_reg_switch, p_cmd);
            }
            break;

        case ETHER_SWITCH_CMD_TIMER_ACTIVE :              ///< Enables / disables the timer with the specified timer number.
            err = ether_switch_timer_active(p_reg_switch, p_cmd);
            break;

        case ETHER_SWITCH_CMD_TIMER_VALUE :               ///< Gets current timer value.
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = FSP_ERR_INVALID_ARGUMENT;
            }
            else
            {
                err = ether_switch_timer_get(p_reg_switch, p_cmd);
            }
            break;

        case ETHER_SWITCH_CMD_PTP_DOMAIN :                ///< Sets / gets the domain number in the specified timer
            err = ether_switch_ptp_domain(p_reg_switch, p_cmd);
            break;

        case ETHER_SWITCH_CMD_PTP_PEERDELAY :             ///< Sets / gets the peer delay value for the specified timer number of the specified port number.
            err = ether_switch_ptp_peerdelay(p_reg_switch, p_cmd);
            break;

        case ETHER_SWITCH_CMD_PTP_CALLBACK :              ///< Register a callback function that receives a send timestamp event.
            err = ether_switch_ptp_callback(p_reg_switch, p_cmd);
            break;
#endif /* ETHER_SWITCH_CONFIG_PTP */

#ifdef ETHER_SWITCH_CONFIG_TDMA
        case ETHER_SWITCH_CMD_TDMA_ENABLE :               ///< Sets /gets the state of the TDMA scheduler.
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = ether_switch_tdma_enable_set(p_reg_switch, p_cmd);
            }
            else
            {
                err = ether_switch_tdma_enable_get(p_reg_switch, p_cmd);
            }
            break;
        case ETHER_SWITCH_CMD_TDMA_TCV_SEQ_ENTRY :        ///< Sets /gets TCV sequence entry.
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = ether_switch_tdma_tcv_seq_entry_set(p_reg_switch, p_cmd);
            }
            else
            {
                err = ether_switch_tdma_tcv_seq_entry_get(p_reg_switch, p_cmd);
            }
            break;
        case ETHER_SWITCH_CMD_TDMA_TCV_DAT_ENTRY :        ///< Sets /gets TCV data entry.
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = ether_switch_tdma_tcv_dat_entry_set(p_reg_switch, p_cmd);
            }
            else
            {
                err = ether_switch_tdma_tcv_dat_entry_get(p_reg_switch, p_cmd);
            }
            break;
        case ETHER_SWITCH_CMD_TDMA_TCV_SEQ_RANGE :        ///< Sets /gets start and last of TCV sequence entry.
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = ether_switch_tdma_tcv_seq_range_set(p_reg_switch, p_cmd);
            }
            else
            {
                err = ether_switch_tdma_tcv_seq_range_get(p_reg_switch, p_cmd);
            }
            break;
        case ETHER_SWITCH_CMD_MMCLT_QGATE :               ///< Sets queue gate.
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = ether_switch_mmctl_qgate_set(p_reg_switch, p_cmd);
            }
            else
            {
                err = FSP_ERR_INVALID_ARGUMENT;
            }
            break;
#endif /* ETHER_SWITCH_CONFIG_TDMA */

#ifdef ETHER_SWITCH_CONFIG_PRIORITY3_OTHER
        case ETHER_SWITCH_CMD_PORT_MIRROR :
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = ether_switch_mirror_set(p_reg_switch, p_cmd);
            }
            else
            {
                /* TODO: port isn't returned to caller as it is a stack-local variable -- also make sure it is converted in the board driver after returning it */
                err = ether_switch_mirror_get(p_reg_switch, p_cmd);
            }
            break;

        case ETHER_SWITCH_CMD_QOS_MODE :
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = ether_switch_qos_mode_set(p_reg_switch, p_cmd);
            }
            else
            {
                err = ether_switch_qos_mode_get(p_reg_switch, p_cmd);
            }
            break;

        case ETHER_SWITCH_CMD_QOS_PRIO_VLAN :
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = ether_switch_qos_prio_vlan_set(p_reg_switch, p_cmd);
            }
            else
            {
                err = ether_switch_qos_prio_vlan_get(p_reg_switch, p_cmd);
            }
            break;

        case ETHER_SWITCH_CMD_QOS_PRIO_IP :
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = ether_switch_qos_prio_ip_set(p_reg_switch, p_cmd);
            }
            else
            {
                err = ether_switch_qos_prio_ip_get(p_reg_switch, p_cmd);
            }
            break;

        case ETHER_SWITCH_CMD_QOS_PRIO_TYPE :
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = ether_switch_qos_prio_type_set(p_reg_switch, p_cmd);
            }
            else
            {
                err = ether_switch_qos_prio_type_get(p_reg_switch, p_cmd);
            }
            break;

        case ETHER_SWITCH_CMD_QUEUES_PER_PORT:
            err = ether_switch_out_queues_get(p_reg_switch, p_cmd);
            break;

        case ETHER_SWITCH_CMD_DISC_UNK :
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = ether_switch_disc_unk_set(p_reg_switch, p_cmd);
            }
            else
            {
                err = ether_switch_disc_unk_get(p_reg_switch, p_cmd);
            }
            break;

        case ETHER_SWITCH_CMD_FLOOD_UNK :
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = ether_switch_flood_unk_dest_set(p_reg_switch, p_cmd);
            }
            else
            {
                err = ether_switch_flood_unk_dest_get(p_reg_switch, p_cmd);
            }
            break;
#endif /* ETHER_SWITCH_CONFIG_PRIORITY3_OTHER */

#ifdef ETHER_SWITCH_CONFIG_PRIORITY4
        case ETHER_SWITCH_CMD_STORMTIME :
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = ether_switch_storm_time_set(p_reg_switch, p_cmd);
            }
            else
            {
                err = ether_switch_storm_time_get(p_reg_switch, p_cmd);
            }
            break;

        case ETHER_SWITCH_CMD_BCAST_LIMIT :
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = ether_switch_bcast_limit_set(p_reg_switch, p_cmd);
            }
            else
            {
                err = ether_switch_bcast_limit_get(p_reg_switch, p_cmd);
            }
            break;

        case ETHER_SWITCH_CMD_MCAST_LIMIT :
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = ether_switch_mcast_limit_set(p_reg_switch, p_cmd);
            }
            else
            {
                err = ether_switch_mcast_limit_get(p_reg_switch, p_cmd);
            }
            break;

        case ETHER_SWITCH_CMD_TXRATE :
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set)
            {
                err = ether_switch_tx_rate_set(p_reg_switch, p_cmd);
            }
            else
            {
                err = ether_switch_tx_rate_get(p_reg_switch, p_cmd);
            }
            break;
#endif  /* ETHER_SWITCH_CONFIG_PRIORITY4 */

#ifdef ETHER_SWITCH_CONFIG_PRIORITY5
        /* EEE */
        case ETHER_SWITCH_CMD_EEE:
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set) {
                err = ether_switch_eee_set(p_reg_switch, p_cmd);
            } else {
                err = ether_switch_eee_get(p_reg_switch, p_cmd);
            }
            break;

  #ifdef ETHER_SWITCH_CONFIG_SNOOP
        /* Add snooping config */
        case ETHER_SWITCH_CMD_SNOOP_ADD:
            err = ether_switch_snoop_add(p_reg_switch, p_cmd);
            break;

        /* Remove snooping config */
        case ETHER_SWITCH_CMD_SNOOP_REM:
            err = ether_switch_snoop_rem(p_reg_switch, p_cmd);
            break;
  #endif /* ETHER_SWITCH_CONFIG_SNOOP */

  #ifdef ETHER_SWITCH_CONFIG_BRIDGE
        /* Get/set port authorization */
        case ETHER_SWITCH_CMD_PORT_AUTH:
            err = ether_switch_auth(p_reg_switch, p_cmd);
            break;

        /* Get/set port controlled direction */
        case ETHER_SWITCH_CMD_PORT_CTRL_DIR:
            err = ether_switch_controlled_direction(p_reg_switch, p_cmd);
            break;

        /* Get/set EAPOL reception mode */
        case ETHER_SWITCH_CMD_PORT_EAPOL_ENABLE:
            err = ether_switch_eapol_reception(p_reg_switch, p_cmd);
            break;

        /* Get/set BPDU control */
        case ETHER_SWITCH_CMD_BPDU:
            err = ether_switch_bpdu_forwarding(p_reg_switch, p_cmd);
            break;
  #endif /* ETHER_SWITCH_CONFIG_BRIDGE */

  #ifdef ETHER_SWITCH_CONFIG_DLR
        /* initialize DLR module */
        case ETHER_SWITCH_CMD_DLR_INIT:
            err = ether_switch_dlr_init(p_reg_switch, p_cmd);
            break;

        /* control DLR module */
        case ETHER_SWITCH_CMD_DLR_ENABLE:
        case ETHER_SWITCH_CMD_DLR_SHUTDOWN:
            err = ether_switch_dlr_tgl(p_reg_switch, p_cmd);
            break;

        /* get DLR status values */
        case ETHER_SWITCH_CMD_DLR_LAST_BEACON:
        case ETHER_SWITCH_CMD_DLR_NODE_STATE:
        case ETHER_SWITCH_CMD_DLR_RING_STATE:
        case ETHER_SWITCH_CMD_DLR_SV_IP:
        case ETHER_SWITCH_CMD_DLR_SV_PREC:
        case ETHER_SWITCH_CMD_DLR_VLAN:
            err = ether_switch_dlr_status(p_reg_switch, p_cmd);
            break;

        /*  get DLR Super Visor MAc address */
        case ETHER_SWITCH_CMD_DLR_SV_MAC:
            err = ether_switch_dlr_supervisor_mac(p_reg_switch, p_cmd);
            break;

        /* enable forwarding DLR frames */
        case ETHER_SWITCH_CMD_DLR_FORWARD:
            err = ether_switch_dlr_forward_enable(p_reg_switch, p_cmd);
            break;
  #endif /* ETHER_SWITCH_CONFIG_DLR */

        case ETHER_SWITCH_CMD_FRAMESIZE_MAX:
            if (ETHER_SWITCH_TRUE == p_cmd->flg_set) {
                err = ether_switch_frame_size_set(p_reg_switch, p_cmd);
            } else {
                err = ether_switch_frame_size_get(p_reg_switch, p_cmd);
            }
            break;
#endif  /* ETHER_SWITCH_CONFIG_PRIORITY5 */

        default:
            err = FSP_ERR_UNSUPPORTED;
            break;
    }

    return err;
}                                      /* End of function R_ETHER_SWITCH_Control() */

/*******************************************************************************************************************//**
 * @} (end addtogroup ETHER)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: ether_eint_isr
 * Description  : Interrupt handler for Ethernet receive and transmit interrupts.
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
#ifdef BSP_PLAT_RZN1SDB /* RZ/N1S */
    static void ether_switch_isr_intr (IRQn_Type *irq_num_ptr)
#else /* !BSP_PLAT_RZN1SDB (RZ/T2) */
  #ifdef RZT2_RSK
    void ether_switch_isr_intr (void)
  #else /* !RZT2_RSK */
    void ether_switch_isr_intr (uint32_t int_sense)
  #endif
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */

{
    ether_switch_callback_args_t callback_arg;
    uint32_t                     ulInt_Stat_Ack_Val;
    R_ETHSW_Type               * p_reg_switch;
    volatile uint32_t            dammy_read;

#ifdef BSP_PLAT_RZN1SDB /* RZ/N1S */
    ether_switch_instance_ctrl_t * p_instance_ctrl 
                                            = (ether_switch_instance_ctrl_t *) R_FSP_IsrContextGet(*irq_num_ptr);
#else /* !BSP_PLAT_RZN1SDB (RZ/T2) */
    IRQn_Type               irq             = R_FSP_CurrentIrqGet();
    ether_switch_instance_ctrl_t * p_instance_ctrl
                                            = (ether_switch_instance_ctrl_t *) R_FSP_IsrContextGet(irq);
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */


    p_reg_switch = (R_ETHSW_Type *)p_instance_ctrl->p_reg_switch;

    ulInt_Stat_Ack_Val = p_reg_switch->INT_STAT_ACK;

    /* Clear IRQ_LINK Interrupt */
    p_reg_switch->INT_STAT_ACK = ulInt_Stat_Ack_Val;

    /* Dammy read for Countermeasure when the CPU clock is 400 or 800MHz */
    dammy_read = p_reg_switch->INT_STAT_ACK;
    dammy_read = dammy_read;

    /* Callback : Interrupt handler */
    if ((0L != (ulInt_Stat_Ack_Val & INT_CONFIG_IRQ_LNK_MASK)) &&
        (NULL != p_instance_ctrl->p_switch_cfg->p_callback))
    {
        callback_arg.channel     = p_instance_ctrl->p_switch_cfg->channel;

        callback_arg.status_link  = p_reg_switch->STATUS_P0_b.PHYLINK;
        callback_arg.status_link |= p_reg_switch->STATUS_P1_b.PHYLINK << 1;
        callback_arg.status_link |= p_reg_switch->STATUS_P2_b.PHYLINK << 2;

        callback_arg.event       = ETHER_SWITCH_EVENT_LINK_CHANGE;
        callback_arg.p_context   = p_instance_ctrl->p_switch_cfg->p_context;

        (*p_instance_ctrl->p_switch_cfg->p_callback)((void *) &callback_arg);
    }

#ifdef ETHER_SWITCH_CONFIG_PTP
    /* PTP Interrupt */
    if(INT_CONFIG_TSM_INT == (ulInt_Stat_Ack_Val & INT_CONFIG_TSM_INT))
    {
        ether_switch_isr_tsm (p_instance_ctrl);
    }
#endif /* ETHER_SWITCH_CONFIG_PTP */


#ifdef ETHER_SWITCH_CONFIG_TDMA
    /* PTP Interrupt */
    if(INT_CONFIG_TDMA_INT == (ulInt_Stat_Ack_Val & INT_CONFIG_TDMA_INT))
    {
        ether_switch_isr_tdma (p_instance_ctrl);
    }
#endif /* ETHER_SWITCH_CONFIG_TDMA */

    /* Pattern Matcher Interrupt */
    if(INT_CONFIG_PATTERN_INT == (ulInt_Stat_Ack_Val & INT_CONFIG_PATTERN_INT))
    {
        ether_switch_isr_pattern (p_instance_ctrl);
    }

#ifdef ETHER_SWITCH_CONFIG_PRIORITY5
  #ifdef ETHER_SWITCH_CONFIG_DLR
    /* DLR Interrupt */
    if(INT_CONFIG_DLR_INT == (ulInt_Stat_Ack_Val & INT_CONFIG_DLR_INT))
    {
        ether_switch_isr_dlr (p_instance_ctrl);
    }
  #endif /* ETHER_SWITCH_CONFIG_DLR */
#endif /* ETHER_SWITCH_CONFIG_PRIORITY5 */

#ifndef BSP_PLAT_RZN1SDB /* RZ/T2 */
    /* Clear pending interrupt flag to make sure it doesn't fire again
     * after exiting. */
  #ifndef RZT2_RSK
    R_BSP_IrqStatusClear(R_FSP_CurrentIrqGet());
  #endif /* !RZT2_RSK */
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */

  }/* End of function ether_switch_eint_isr() */

/***********************************************************************************************************************
 * Function Name: ether_suitch_enable_icu
 * Description  :
 * Arguments    : p_instance_ctrl -
 *                    pointer to control structure
 * Return Value : none
 ***********************************************************************************************************************/
static void ether_switch_enable_icu (ether_switch_instance_ctrl_t * const p_instance_ctrl)
{
    /** Configure the Ethernet Switch interrupt. */
    R_BSP_IrqCfgEnable(p_instance_ctrl->p_switch_cfg->irq,
                       p_instance_ctrl->p_switch_cfg->interrupt_priority,
                       p_instance_ctrl);
}                                      /* End of function ether_switch_enable_icu() */

/***********************************************************************************************************************
 * Function Name: ether_switch_disable_icu
 * Description  :
 * Arguments    : p_instance_ctrl -
 *                    pointer to control structure
 *                -
 * Return Value : none
 ***********************************************************************************************************************/
static void ether_switch_disable_icu (ether_switch_instance_ctrl_t * const p_instance_ctrl)
{
#ifndef BSP_PLAT_RZN1SDB /* RZ/T2 */
    /* Get IRQ number for A5PSW_Int interrupt. */
    R_BSP_IrqDisable (p_instance_ctrl->p_switch_cfg->irq);
    R_FSP_IsrContextSet(p_instance_ctrl->p_switch_cfg->irq, NULL);
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */
}                                      /* End of function ether_disable_icu() */


#ifdef BSP_PLAT_RZN1SDB /* RZ/N1S */
/*
 * RZ/N1�p�_�~�[��?�
 */
#define MAX_IRQ     256
static void * gp_context[MAX_IRQ] = {0};

static void R_BSP_IrqCfgEnable (IRQn_Type const irq, uint32_t priority, void * p_context)
{
    if (irq < MAX_IRQ)
    {
        gp_context[irq] = p_context;
    }
}

static void * R_FSP_IsrContextGet (IRQn_Type const irq)
{
    if (irq < MAX_IRQ)
    {
        return  gp_context[irq];
    }
    else
    {
        return  0;
    }
}

static int RegisterInterrupt (IRQn_Type irq, uint32_t priority)
{
    uint32_t int_priority = priority;

    /* Register interrupts */
    if (R_BSP_InterruptRegisterCallback(irq, (bsp_int_cb_t) &ether_switch_isr_intr)
                == MCU_INT_SUCCESS)
    {
        if (R_BSP_InterruptControl(irq, MCU_INT_CMD_SET_INTERRUPT_PRIORITY, &int_priority)
                    == MCU_INT_SUCCESS)
        {
            if ( R_BSP_InterruptControl(irq, MCU_INT_CMD_INTERRUPT_ENABLE, FIT_NO_PTR)
                        == MCU_INT_SUCCESS)
            {
                return 0;
            }
        }
    }
    return -1;
}
#endif /* BSP_PLAT_RZN1SDB */

/***********************************************************************************************************************
 * Function Name: ether_switch_memset_vol
 * Description  : Memset with volatile support
 * Parameters   : pData
 *              :       - dest address
 *              : chr
 *              :       - set character
 *              : cnt
 *              :       - set size
 *              : - 
 * Return Value : none
 **********************************************************************************************************************/
static void ether_switch_memset_vol(volatile void * p_data, uint8_t chr, size_t cnt)
{
    void * p_dst_mem = (void *) ((uint32_t) p_data);    ///< de-volatiled pointer

    ETHER_SWITCH_MEMSET_SAVE(p_dst_mem, chr, cnt);
}


/***********************************************************************************************************************
 * Function Name: ether_switch_memcpy_vol
 * Description  : Memcpy with volatile support
 * Parameters   : p_dst
 *              :       - dest address
 *              : p_src
 *              :       - source address
 *              : cnt
 *              :       - copy size
 *              : - 
 * Return Value : None
 **********************************************************************************************************************/
static void ether_switch_memcpy_vol(volatile void * p_dst, volatile const void * p_src, size_t cnt)
{
    void * p_dst_mem = (void *) ((uint32_t) p_dst); ///< de-volatiled dst pointer
    void * p_src_mem = (void *) ((uint32_t) p_src); ///< de-volatiled src pointer

    /* copy to target register */
    ETHER_SWITCH_MEMCPY_SAVE(p_dst_mem, p_src_mem, cnt);
}

/***********************************************************************************************************************
 * Function Name: ether_switch_mac_tab_set
 * Description  : This function sets the static MAC address entry for the given MAC address.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 *              : FSP_ERR_TIMEOUT
 *              :       - Timeout error
**************************************************************************************************/
fsp_err_t ether_switch_mac_tab_set(R_ETHSW_Type volatile *   p_switch_reg,
                                          ether_switch_cmd_t *      p_cmd)
{
    volatile ether_switch_mactab_entry_reg_t  * p_entry;    /* MAC table entry(for register access) */
#ifndef BSP_PLAT_RZN1SDB /* RZ/T2 */
    uint64_t                                    timeout;    /* timeout */
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */
    ether_switch_port_mask_t                    port_mask;  /* converted portmask */
    uint32_t                                    priority;   /* switching priority for entry */
    ether_switch_mac_addr_t                     addr;       /* mac address buffer */

    /* this function only works with a portmask */
    if (!(p_cmd->data.p_mac_tab->portmask & ETHER_SWITCH_PORT_MASK))
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

    /* remove portmask bit and translate host port if set */
    port_mask = p_cmd->data.p_mac_tab->portmask & ETHER_SWITCH_PORT_BITS;
    if (p_cmd->data.p_mac_tab->portmask & ETHER_SWITCH_PORT_HOST)
    {
        port_mask |= (1 << portHost);
    }

    /* set Entry-pointer to "Lower 32-Bit Data of Lookup Memory Entry" register */
    p_entry = (volatile ether_switch_mactab_entry_reg_t *) &(p_switch_reg->LK_DATA_LO);

    /* copy mac address to get valid alignment */
    memcpy(addr, * p_cmd->data.p_mac_tab->addr, ETHER_SWITCH_MAC_ADDR_LEN);

    /* copy aligned mac address into register */
    ether_switch_memset_vol(p_entry, 0, sizeof(ether_switch_mactab_entry_reg_t));
    ether_switch_memcpy_vol(p_entry->mac, addr, ETHER_SWITCH_MAC_ADDR_LEN);

    /* Delete entry? */
    if (0 == port_mask)
    {
        ETHER_SWITCH_REG_SET(p_switch_reg->LK_ADDR_CTRL, p_switch_reg->LK_ADDR_CTRL | ETHER_SWITCH_LK_ADDR_CLEAR | ETHER_SWITCH_LK_ADDR_LOOKUP);
    }
    else
    {
#ifndef BSP_PLAT_RZN1SDB /* RZ/T2 */
        /* calculate timeout based on current timestamp */
        timeout = Get_Time_Stamp() + SW_RN_ADVSW_TIMEOUT_MS;
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */

        /* Perform lookup to get address entry */
        ETHER_SWITCH_REG_SET(p_switch_reg->LK_ADDR_CTRL, p_switch_reg->LK_ADDR_CTRL | ETHER_SWITCH_LK_ADDR_LOOKUP);
        while (ETHER_SWITCH_TRUE == p_switch_reg->LK_ADDR_CTRL_b.BUSY)
        {
#ifndef BSP_PLAT_RZN1SDB /* RZ/T2 */
            if (Get_Time_Stamp() >= timeout)
            {
                return FSP_ERR_TIMEOUT;
            }
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */
        }

        /* extract requested priority if given by command data */
        priority = p_cmd->data.p_mac_tab->priority;

        /* Address is already there. Just update the port mask and priority */
        if (p_entry->mask_and_flags & ETHER_SWITCH_LK_ADDR_VALID)
        {

            /* update port bit mask  */
            ETHER_SWITCH_REG_SET(p_switch_reg->LK_DATA_HI, p_switch_reg->LK_DATA_HI & ~((uint32_t) ETHER_SWITCH_LK_ADDR_PORTMASK));
            ETHER_SWITCH_REG_SET(p_switch_reg->LK_DATA_HI, p_switch_reg->LK_DATA_HI | (port_mask << ETHER_SWITCH_LK_ADDR_PORT_REG_OFFSET));

            /* updated priority */
            ETHER_SWITCH_REG_SET(p_switch_reg->LK_DATA_HI, p_switch_reg->LK_DATA_HI & ~((uint32_t) ETHER_SWITCH_LK_ADDR_PRIOMASK));
            ETHER_SWITCH_REG_SET(p_switch_reg->LK_DATA_HI, p_switch_reg->LK_DATA_HI | ((priority << ETHER_SWITCH_LK_ADDR_PRIO_REG_OFFSET) & ETHER_SWITCH_LK_ADDR_PRIOMASK));

        }
        else
        {

            /* New static entry to be created */
            ETHER_SWITCH_REG_SET(p_switch_reg->LK_DATA_HI, p_switch_reg->LK_DATA_HI & ~(ETHER_SWITCH_LK_ADDR_MASKANDFLAGS_MASK));
            ETHER_SWITCH_REG_SET(p_switch_reg->LK_DATA_HI, p_switch_reg->LK_DATA_HI | ETHER_SWITCH_LK_ADDR_REG_VALID);
            ETHER_SWITCH_REG_SET(p_switch_reg->LK_DATA_HI, p_switch_reg->LK_DATA_HI | ETHER_SWITCH_LK_ADDR_REG_TYPE);
            ETHER_SWITCH_REG_SET(p_switch_reg->LK_DATA_HI, p_switch_reg->LK_DATA_HI | (port_mask << ETHER_SWITCH_LK_ADDR_PORT_REG_OFFSET));
            ETHER_SWITCH_REG_SET(p_switch_reg->LK_DATA_HI, p_switch_reg->LK_DATA_HI | ((priority << ETHER_SWITCH_LK_ADDR_PRIO_REG_OFFSET) & ETHER_SWITCH_LK_ADDR_PRIOMASK));
        }

        /* trigger MAC table write */
        ETHER_SWITCH_REG_SET(p_switch_reg->LK_ADDR_CTRL, p_switch_reg->LK_ADDR_CTRL | ETHER_SWITCH_LK_ADDR_WRITE);
    }

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_mac_tab_get
 * Description  : This function retrieves the port mask for the given MAC address from the static MAC table.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 *              : FSP_ERR_TIMEOUT
 *              :       - Timeout error
 **********************************************************************************************************************/
static fsp_err_t ether_switch_mac_tab_get(R_ETHSW_Type volatile *   p_switch_reg,
                                          ether_switch_cmd_t *      p_cmd)
{
#ifndef BSP_PLAT_RZN1SDB /* RZ/T2 */
    uint64_t                                    timeout;    /* timeout */
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */
    volatile ether_switch_mactab_entry_reg_t  * p_entry;    /* MAC table entry(for register access) */
    ether_switch_mac_addr_t                     addr;       /* mac address buffer */

    p_entry = (volatile ether_switch_mactab_entry_reg_t *) &(p_switch_reg->LK_DATA_LO);

    /* copy mac address to get valid alignment */
    memcpy(addr, * p_cmd->data.p_mac_tab->addr, ETHER_SWITCH_MAC_ADDR_LEN);

    /* copy aligned mac address into register */
    ether_switch_memset_vol(p_entry, 0, sizeof(ether_switch_mactab_entry_reg_t));
    ether_switch_memcpy_vol(p_entry->mac, addr, ETHER_SWITCH_MAC_ADDR_LEN);

#ifndef BSP_PLAT_RZN1SDB /* RZ/T2 */
    /* calculate timeout based on current timestamp */
    timeout = Get_Time_Stamp() + SW_RN_ADVSW_TIMEOUT_MS;
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */

    /* Perform lookup to get address entry */
    ETHER_SWITCH_REG_SET(p_switch_reg->LK_ADDR_CTRL, p_switch_reg->LK_ADDR_CTRL | ETHER_SWITCH_LK_ADDR_LOOKUP);
    while (ETHER_SWITCH_TRUE == p_switch_reg->LK_ADDR_CTRL_b.BUSY)
    {
#ifndef BSP_PLAT_RZN1SDB /* RZ/T2 */
        if (Get_Time_Stamp() >= timeout)
        {
            return FSP_ERR_TIMEOUT;
        }
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */
    }

    /* Entry found? */
    if (p_entry->mask_and_flags & ETHER_SWITCH_LK_ADDR_VALID)
    {
        p_cmd->data.p_mac_tab->portmask = p_entry->mask_and_flags >> ETHER_SWITCH_LK_ADDR_PORT_OFFSET;

        /* if host port is set convert to host port */
        if (p_cmd->data.p_mac_tab->portmask & (1 << portHost))
        {
            p_cmd->data.p_mac_tab->portmask = (p_cmd->data.p_mac_tab->portmask & ~(1 << portHost)) | ETHER_SWITCH_PORT_HOST;
        }

        /* set port mask flag */
        p_cmd->data.p_mac_tab->portmask |= ETHER_SWITCH_PORT_MASK;
    }
    else
    {
        p_cmd->data.p_mac_tab->portmask = 0;
        return FSP_ERR_INVALID_ARGUMENT;
    }

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_mac_tab_conf_set
 * Description  : This function configures the MAC learning and aging parameters of MAC table.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_mac_tab_conf_set(R_ETHSW_Type volatile *  p_switch_reg,
                                               ether_switch_cmd_t *     p_cmd)
{
    uint32_t reg_val;                            /* Register value */

    reg_val = p_switch_reg->LK_CTRL & ~((uint32_t) ETHER_SWITCH_LK_MASK);
    reg_val |= ((uint32_t) p_cmd->data.p_mac_tab_cnf->learn << ETHER_SWITCH_LK_LEARN)
             | ((uint32_t) p_cmd->data.p_mac_tab_cnf->aging << ETHER_SWITCH_LK_AGING)
             | ((uint32_t) p_cmd->data.p_mac_tab_cnf->migration << ETHER_SWITCH_LK_MIGRAT)
             | ((uint32_t) p_cmd->data.p_mac_tab_cnf->discunknown << ETHER_SWITCH_LK_DISCUNKN)
             | ((uint32_t) p_cmd->data.p_mac_tab_cnf->pervlan << ETHER_SWITCH_LK_PERVLAN);
    ETHER_SWITCH_REG_SET(p_switch_reg->LK_CTRL, reg_val);

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_mac_tab_conf_get
 * Description  : This function configures the MAC learning and aging parameters of MAC table.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_mac_tab_conf_get(R_ETHSW_Type volatile *  p_switch_reg,
                                               ether_switch_cmd_t *     p_cmd)
{
    p_cmd->data.p_mac_tab_cnf->learn        = p_switch_reg->LK_CTRL & (1 << ETHER_SWITCH_LK_LEARN)      ? ETHER_SWITCH_TRUE : ETHER_SWITCH_FALSE;
    p_cmd->data.p_mac_tab_cnf->aging        = p_switch_reg->LK_CTRL & (1 << ETHER_SWITCH_LK_AGING)      ? ETHER_SWITCH_TRUE : ETHER_SWITCH_FALSE;
    p_cmd->data.p_mac_tab_cnf->migration    = p_switch_reg->LK_CTRL & (1 << ETHER_SWITCH_LK_MIGRAT)     ? ETHER_SWITCH_TRUE : ETHER_SWITCH_FALSE;
    p_cmd->data.p_mac_tab_cnf->discunknown  = p_switch_reg->LK_CTRL & (1 << ETHER_SWITCH_LK_DISCUNKN)   ? ETHER_SWITCH_TRUE : ETHER_SWITCH_FALSE;
    p_cmd->data.p_mac_tab_cnf->pervlan      = p_switch_reg->LK_CTRL & (1 << ETHER_SWITCH_LK_PERVLAN)    ? ETHER_SWITCH_TRUE : ETHER_SWITCH_FALSE;

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_mac_tab_clear
 * Description  : This function clears specific types of entries from the MAC table or clears the whole table.
 * Parameters   : p_switch_reg
 *              :       - switch register address
 *              : p_cmd
 *              :       - command pointer
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 *              : FSP_ERR_TIMEOUT
 *              :       - Timeout error
 **********************************************************************************************************************/
static fsp_err_t ether_switch_mac_tab_clear(R_ETHSW_Type volatile * p_switch_reg,
                                            ether_switch_cmd_t *    p_cmd)
{
    fsp_err_t                       res = FSP_ERR_TIMEOUT;  /* result */
#ifndef BSP_PLAT_RZN1SDB /* RZ/T2 */
    uint64_t                        timeout;                /* timeout */
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */
    ether_switch_mactab_clr_modes_t mode;                   /* clear mode */

    /* select clear mode by command or use application settings */
    if ((ETHER_SWITCH_CMD_MAC_UNI_FLUSH == p_cmd->cmd) ||
        (ETHER_SWITCH_CMD_MAC_MULTI_FLUSH == p_cmd->cmd))
    {
        mode = ETHER_SWITCH_MACTAB_CLR_DYNAMIC;
    }
    else
    {
        mode = * p_cmd->data.p_mac_tab_clr;
    }

#ifndef BSP_PLAT_RZN1SDB /* RZ/T2 */
    /* calculate timeout based on current timestamp */
    timeout = Get_Time_Stamp() + SW_RN_ADVSW_TIMEOUT_MS;
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */

    switch (mode)
    {
        case ETHER_SWITCH_MACTAB_CLR_ALL:
            p_switch_reg->LK_CTRL_b.CLRTBL = ETHER_SWITCH_TRUE;

            while (ETHER_SWITCH_TRUE == p_switch_reg->LK_CTRL_b.CLRTBL)
            {
#ifndef BSP_PLAT_RZN1SDB /* RZ/N1S */
                if (Get_Time_Stamp() >= timeout)
                {
                    return FSP_ERR_TIMEOUT;
                }
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */
            }

            res = FSP_SUCCESS;
            break;

        case ETHER_SWITCH_MACTAB_CLR_DYNAMIC:
            p_switch_reg->LK_ADDR_CTRL_b.CLR_DYNAMIC = ETHER_SWITCH_TRUE;

            while (ETHER_SWITCH_TRUE == p_switch_reg->LK_ADDR_CTRL_b.CLR_DYNAMIC)
            {
#ifndef BSP_PLAT_RZN1SDB /* RZ/T2 */
                if (Get_Time_Stamp() >= timeout)
                {
                    return FSP_ERR_TIMEOUT;
                }
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */
            }

            res = FSP_SUCCESS;
            break;

        case ETHER_SWITCH_MACTAB_CLR_STATIC:
            p_switch_reg->LK_ADDR_CTRL_b.CLR_STATIC = ETHER_SWITCH_TRUE;

            while (ETHER_SWITCH_TRUE == p_switch_reg->LK_ADDR_CTRL_b.CLR_STATIC)
            {
#ifndef BSP_PLAT_RZN1SDB /* RZ/T2 */
                if (Get_Time_Stamp() >= timeout)
                {
                    return FSP_ERR_TIMEOUT;
                }
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */
            }

            res = FSP_SUCCESS;
            break;

        default:
            res = FSP_ERR_INVALID_ARGUMENT;
            break;
    }

    return res;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_link_get
 * Description  : Return link state for given port
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_link_get(R_ETHSW_Type volatile *  p_switch_reg,
                                       ether_switch_cmd_t *     p_cmd)
{
    const volatile uint32_t * p_reg;            /* Register */

    p_reg = (const volatile uint32_t *) ((uint8_t *) &p_switch_reg->STATUS_P0 + ETHER_SWITCH_COUNTER_OFFSET * p_cmd->port);
    * p_cmd->data.p_state_link = (* p_reg & ETHER_SWITCH_PHY_LINK) ? ETHER_SWITCH_STATE_UP : ETHER_SWITCH_STATE_DOWN;

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_admin_state_set
 * Description  : Set admin state for the given port
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_admin_state_set(R_ETHSW_Type volatile *   p_switch_reg,
                                              ether_switch_cmd_t *      p_cmd)
{
    volatile uint32_t * p_reg;                  /* Register */

    p_reg = (volatile uint32_t *) ((uint8_t *) &(p_switch_reg->COMMAND_CONFIG_P0) + ETHER_SWITCH_PORT_OFFSET * p_cmd->port);

    if (* p_cmd->data.p_state_port)
    {
        * p_reg |= ETHER_SWITCH_PORT_RXTX;
    }
    else
    {
        * p_reg &= ~((uint32_t) ETHER_SWITCH_PORT_RXTX);
    }

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_admin_state_get
 * Description  : Get admin state for the given port
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_admin_state_get(R_ETHSW_Type volatile *   p_switch_reg,
                                              ether_switch_cmd_t *      p_cmd)
{
    volatile uint32_t * p_reg;                  /* Register */

    p_reg = (volatile uint32_t *) ((uint8_t *) &(p_switch_reg->COMMAND_CONFIG_P0) + ETHER_SWITCH_PORT_OFFSET * p_cmd->port);
    * p_cmd->data.p_state_port = ((* p_reg & ETHER_SWITCH_PORT_RXTX) == ETHER_SWITCH_PORT_RXTX) ? ETHER_SWITCH_TRUE : ETHER_SWITCH_FALSE;

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_vlan_def_set
 * Description  : Sets the default VLAN for input/output processing for the specified port.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_vlan_def_set(R_ETHSW_Type volatile *  p_switch_reg,
                                           ether_switch_cmd_t *     p_cmd)
{
    volatile uint32_t * p_def_tag;              /* default tag */

    p_def_tag = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->SYSTEM_TAGINFO[0] + ETHER_SWITCH_PORT_OFFSET_4BYTES * p_cmd->port);
    ETHER_SWITCH_REG_SET(* p_def_tag, * p_cmd->data.p_id_vlan & ETHER_SWITCH_VLAN_DEFID_MASK);

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_vlan_def_get
 * Description  : Retrieves the default VLAN for the given port.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_vlan_def_get(R_ETHSW_Type volatile *  p_switch_reg,
                                           ether_switch_cmd_t *     p_cmd)
{
    volatile uint32_t * p_def_tag;                /* default tag */

    p_def_tag = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->SYSTEM_TAGINFO[0] + ETHER_SWITCH_PORT_OFFSET_4BYTES * p_cmd->port);
    * p_cmd->data.p_id_vlan = * p_def_tag & ETHER_SWITCH_VLAN_DEFID_MASK;

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_vlan_port_add
 * Description  : This function adds a VLAN for a given port to the VLAN domain table.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_vlan_port_add(R_ETHSW_Type volatile * p_switch_reg,
                                            ether_switch_cmd_t *    p_cmd)
{
    volatile uint32_t * p_vlan_entry;           /* Pointer VLAN table entry */

    p_vlan_entry = (volatile uint32_t *) &(p_switch_reg->VLAN_RES_TABLE[0]);

    /* First run: check whether the vlan id entry is already in the table */
    while (p_vlan_entry <= (volatile uint32_t *) &p_switch_reg->VLAN_RES_TABLE[31])
    {
        if ((uint32_t) (* p_vlan_entry & ETHER_SWITCH_VLANID_MASK) == (uint32_t) (* p_cmd->data.p_id_vlan << ETHER_SWITCH_VLANID_OFFSET))
        {
            /* Update port info and return */
            ETHER_SWITCH_REG_SET(* p_vlan_entry, * p_vlan_entry | (1 << p_cmd->port));
            return FSP_SUCCESS;
        }
        p_vlan_entry = (volatile uint32_t *) ((uint8_t *) p_vlan_entry + ETHER_SWITCH_PORT_OFFSET_4BYTES);
    }

    /* Second run: find free entry and add VLAN id and port */
    p_vlan_entry = (volatile uint32_t *) &(p_switch_reg->VLAN_RES_TABLE[0]);
    while (p_vlan_entry <= (volatile uint32_t *) &p_switch_reg->VLAN_RES_TABLE[31])
    {
        /* Default VLANid is all bit set */
        if (ETHER_SWITCH_VLANID_MASK == (* p_vlan_entry & ETHER_SWITCH_VLANID_MASK))
        {
            ETHER_SWITCH_REG_SET(* p_vlan_entry, (1u << p_cmd->port) | (((uint32_t) * p_cmd->data.p_id_vlan) << ETHER_SWITCH_VLANID_OFFSET));
            return FSP_SUCCESS;
        }
        p_vlan_entry = (volatile uint32_t *) ((uint8_t *) p_vlan_entry + ETHER_SWITCH_PORT_OFFSET_4BYTES);
    }

    /* VLAN not in table and no free entries left */
    return FSP_ERR_INVALID_ARGUMENT;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_vlan_port_rem
 * Description  : This function removes a VLAN entry from the VLAN domain table for the given port.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_vlan_port_rem(R_ETHSW_Type volatile *   p_switch_reg,
                                            ether_switch_cmd_t *      p_cmd)
{
    volatile uint32_t *p_vlan_entry;            /* Pointer VLAN table entry */

    p_vlan_entry = (volatile uint32_t *) &(p_switch_reg->VLAN_RES_TABLE[0]);

    /* First run: check whether the vlan id entry is already in the table */
    while (p_vlan_entry <= (volatile uint32_t *) &p_switch_reg->VLAN_RES_TABLE[31])
    {
        if ((uint32_t) (*p_vlan_entry & ETHER_SWITCH_VLANID_MASK) == (uint32_t) (*p_cmd->data.p_id_vlan << ETHER_SWITCH_VLANID_OFFSET))
        {
            /* Remove port from port mask */
            * p_vlan_entry &= ~(1 << p_cmd->port);

            /* Reset to default if no more ports are mapped
             * so the entry is free again for new VLAN ids
             */
            if (0 == ((* p_vlan_entry) & ETHER_SWITCH_VLAN_PORT_MASK))
            {
                ETHER_SWITCH_REG_SET(* p_vlan_entry, ETHER_SWITCH_VLAN_INVALID_ENTRY);
            }
            return FSP_SUCCESS;
        }
        p_vlan_entry = (volatile uint32_t *) ((uint8_t *) p_vlan_entry + ETHER_SWITCH_PORT_OFFSET_4BYTES);
    }

    /* The VLAN id was not in the table so nothing to remove */
    return FSP_ERR_INVALID_ARGUMENT;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_vlan_in_mode_set
 * Description  : Sets the mode of VLAN input operations for the given port.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_vlan_in_mode_set(R_ETHSW_Type volatile *  p_switch_reg,
                                               ether_switch_cmd_t *     p_cmd)
{
    uint32_t reg_val;                           /* switch reg value */

    /* Disable VLAN input processing ? */
    if (ETHER_SWITCH_VLANIN_DISABLE == * p_cmd->data.p_mode_vlan_in)
    {
        /* Disable input processing and reset mode */
        ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, VLAN_IN_MODE_ENA, VLAN_IN_MODE_ENA_b, VLANINMDEN, p_switch_reg->VLAN_IN_MODE_ENA_b.VLANINMDEN & ~(1 << p_cmd->port), ETHER_SWITCH_TYPE_ETHSW);
        ETHER_SWITCH_REG_SET(p_switch_reg->VLAN_IN_MODE, p_switch_reg->VLAN_IN_MODE & ~((uint32_t) ETHER_SWITCH_VLAN_INOUT_MODE_MASK << (p_cmd->port * ETHER_SWITCH_VLAN_MODE_BITS)));
    }
    else
    {
        reg_val = p_switch_reg->VLAN_IN_MODE;
        /* Set new mode */
        reg_val &= ~(((uint32_t) ETHER_SWITCH_VLAN_INOUT_MODE_MASK) << (p_cmd->port * ETHER_SWITCH_VLAN_MODE_BITS));
        reg_val |= ((uint32_t) * p_cmd->data.p_mode_vlan_in << (p_cmd->port * 2));
        ETHER_SWITCH_REG_SET(p_switch_reg->VLAN_IN_MODE, reg_val);

        /* Enable input processing */
        ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, VLAN_IN_MODE_ENA, VLAN_IN_MODE_ENA_b, VLANINMDEN, p_switch_reg->VLAN_IN_MODE_ENA_b.VLANINMDEN | (1 << p_cmd->port), ETHER_SWITCH_TYPE_ETHSW);
    }

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_vlan_in_mode_get
 * Description  : Reads the current VLAN input mode settings for the given port.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_vlan_in_mode_get(R_ETHSW_Type volatile *   p_switch_reg,
                                               ether_switch_cmd_t *      p_cmd)
{
    /* VLAN input processing enabled? */
    if (0 == (p_switch_reg->VLAN_IN_MODE_ENA & (1 << p_cmd->port)))
    {
        * p_cmd->data.p_mode_vlan_in = ETHER_SWITCH_VLANIN_DISABLE;
    }
    else
    {
        * p_cmd->data.p_mode_vlan_in = (ether_switch_vlan_in_mode_t) ((p_switch_reg->VLAN_IN_MODE >> (p_cmd->port * ETHER_SWITCH_VLAN_MODE_BITS)) & ETHER_SWITCH_VLAN_INOUT_MODE_MASK);
    }

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_vlan_out_mode_set
 * Description  : Sets the mode of VLAN output operations for the given port.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_vlan_out_mode_set(R_ETHSW_Type volatile * p_switch_reg,
                                                ether_switch_cmd_t *    p_cmd)
{
    uint32_t reg_val;                           /* register value */

    reg_val = p_switch_reg->VLAN_OUT_MODE;

    /* Set new mode */
    reg_val &= ~((uint32_t) ETHER_SWITCH_VLAN_INOUT_MODE_MASK << (p_cmd->port * ETHER_SWITCH_VLAN_MODE_BITS));
    reg_val |= ((uint32_t) * p_cmd->data.p_mode_vlan_out << (p_cmd->port * ETHER_SWITCH_VLAN_MODE_BITS));
    ETHER_SWITCH_REG_SET(p_switch_reg->VLAN_OUT_MODE, reg_val);

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_vlan_out_mode_get
 * Description  : Reads the current VLAN output mode settings for the given port.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_vlan_out_mode_get(R_ETHSW_Type volatile * p_switch_reg,
                                                ether_switch_cmd_t *    p_cmd)
{
    * p_cmd->data.p_mode_vlan_out = (ether_switch_vlan_out_mode_t) ((p_switch_reg->VLAN_OUT_MODE >> (p_cmd->port * ETHER_SWITCH_VLAN_MODE_BITS)) & ETHER_SWITCH_VLAN_INOUT_MODE_MASK);

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_vlan_table_cnt
 * Description  : Gets count of VLAN table entries.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_vlan_table_cnt(R_ETHSW_Type volatile *    p_switch_reg,
                                             ether_switch_cmd_t *       p_cmd)
{
    * p_cmd->data.p_cnt_tab_entries = SW_RN_ADVSW_VLAN_TABLE_ENTRIES;

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_vlan_table_get
 * Description  : Gets information of VLAN table entries.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_vlan_table_get(R_ETHSW_Type volatile *    p_switch_reg,
                                             ether_switch_cmd_t *       p_cmd)
{
    volatile uint32_t * p_vlan_entry;           /* VLAN entry */

    if (NULL == p_cmd->data.p_vlan || SW_RN_ADVSW_VLAN_TABLE_ENTRIES <= p_cmd->data.p_vlan->index)
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

    p_vlan_entry = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->VLAN_RES_TABLE[0] + ETHER_SWITCH_PORT_OFFSET_4BYTES * p_cmd->data.p_vlan->index);   /* Pointer VLAN table entry */

    /* Entry valid ? */
    if (ETHER_SWITCH_VLAN_INVALID_ENTRY == (* p_vlan_entry & ETHER_SWITCH_VLAN_INVALID_ENTRY))
    {
        /* Invalid */
        p_cmd->data.p_vlan->valid = ETHER_SWITCH_FALSE;
        p_cmd->data.p_vlan->vlan = 0;
        p_cmd->data.p_vlan->portmask = 0;
    }
    else
    {
        /* Entry is valid */
        p_cmd->data.p_vlan->valid = ETHER_SWITCH_TRUE;
        p_cmd->data.p_vlan->vlan = (* p_vlan_entry  & ETHER_SWITCH_VLANID_MASK) >> ETHER_SWITCH_VLANID_OFFSET;
        p_cmd->data.p_vlan->portmask = (* p_vlan_entry) & ETHER_SWITCH_VLAN_PORT_MASK;
    }

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_vlan_verify_set
 * Description  : Set VLAN verification for the given port.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_vlan_verify_set(R_ETHSW_Type volatile *   p_switch_reg,
                                              ether_switch_cmd_t *      p_cmd)
{
    if (* p_cmd->data.p_flag_enable)
    {
        ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, VLAN_VERIFY, VLAN_VERIFY_b, VLANVERI, p_switch_reg->VLAN_VERIFY_b.VLANVERI | (1 << p_cmd->port), ETHER_SWITCH_TYPE_ETHSW);
    }
    else
    {
        ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, VLAN_VERIFY, VLAN_VERIFY_b, VLANVERI, p_switch_reg->VLAN_VERIFY_b.VLANVERI & ~(1 << p_cmd->port), ETHER_SWITCH_TYPE_ETHSW);
    }

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_vlan_verify_get
 * Description  : Get VLAN verification for the given port.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_vlan_verify_get(R_ETHSW_Type volatile *   p_switch_reg,
                                              ether_switch_cmd_t *      p_cmd)
{
    if (p_switch_reg->VLAN_VERIFY_b.VLANVERI & (1 << p_cmd->port))
    {
        * p_cmd->data.p_flag_enable = ETHER_SWITCH_TRUE;
    }
    else
    {
        * p_cmd->data.p_flag_enable = ETHER_SWITCH_FALSE;
    }

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_vlan_discunk_set
 * Description  : Set VLAN discartion of unknown frames for the given port.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_vlan_discunk_set(R_ETHSW_Type volatile *  p_switch_reg,
                                               ether_switch_cmd_t *     p_cmd)
{
    if (* p_cmd->data.p_flag_enable)
    {
        ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, VLAN_VERIFY, VLAN_VERIFY_b, VLANDISC, p_switch_reg->VLAN_VERIFY_b.VLANDISC | (1 << p_cmd->port), ETHER_SWITCH_TYPE_ETHSW);
    }
    else
    {
        ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, VLAN_VERIFY, VLAN_VERIFY_b, VLANDISC, p_switch_reg->VLAN_VERIFY_b.VLANDISC & ~(1 << p_cmd->port), ETHER_SWITCH_TYPE_ETHSW);
    }

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_vlan_discunk_get
 * Description  : Get VLAN discartion of unknown frames for the given port.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_vlan_discunk_get(R_ETHSW_Type volatile *  p_switch_reg,
                                               ether_switch_cmd_t *     p_cmd)
{
    if (p_switch_reg->VLAN_VERIFY_b.VLANDISC & (1 << p_cmd->port))
    {
        * p_cmd->data.p_flag_enable = ETHER_SWITCH_TRUE;
    }
    else
    {
        * p_cmd->data.p_flag_enable = ETHER_SWITCH_FALSE;
    }

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_learning
 * Description  : Sets/gets learning state for the given port
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_UNSUPPORTED
 *              :       - Selected value not supported by this API
 **********************************************************************************************************************/
static fsp_err_t ether_switch_learning(R_ETHSW_Type volatile *  p_switch_reg,
                                       ether_switch_cmd_t *     p_cmd)
{
    /* reject host port */
    if (ETHER_SWITCH_PORT_HOST == p_cmd->port)
    {
        return FSP_ERR_UNSUPPORTED;
    }

    /* set learning state */
    if (p_cmd->flg_set)
    {
        if (* p_cmd->data.p_flag_dlr_learn)
        {
            ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, INPUT_LEARN_BLOCK, INPUT_LEARN_BLOCK_b, LEARNDIS, p_switch_reg->INPUT_LEARN_BLOCK_b.LEARNDIS & ~(1 << p_cmd->port), ETHER_SWITCH_TYPE_ETHSW);
        }
        else
        {
            ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, INPUT_LEARN_BLOCK, INPUT_LEARN_BLOCK_b, LEARNDIS,  p_switch_reg->INPUT_LEARN_BLOCK_b.LEARNDIS | (1 << p_cmd->port), ETHER_SWITCH_TYPE_ETHSW);
        }
    }
    else
    {
        * p_cmd->data.p_flag_dlr_learn = (ether_switch_bool_t) !(p_switch_reg->INPUT_LEARN_BLOCK_b.LEARNDIS & (1 << p_cmd->port));
    }

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_port_fwd
 * Description  : Enables/disables the forwarding of incoming frames on a port for RSTP
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_port_fwd(R_ETHSW_Type volatile *  p_switch_reg,
                                       ether_switch_cmd_t *     p_cmd)
{
    uint32_t block;                             /* register value */
    uint32_t tx;                                /* register value */

    block = p_switch_reg->INPUT_LEARN_BLOCK_b.BLOCKEN;
    tx = p_switch_reg->PORT_ENA_b.TXENA;
    if (ETHER_SWITCH_CMD_PORT_FWD_ADD == p_cmd->cmd)
    {
        block &= ~(1 << p_cmd->port);
        tx |= (1 << p_cmd->port);
    }
    else
    {
        block |= (1 << p_cmd->port);
        tx &= ~(1 << p_cmd->port);
    }

    ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, INPUT_LEARN_BLOCK, INPUT_LEARN_BLOCK_b, BLOCKEN, block, ETHER_SWITCH_TYPE_ETHSW);
    ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, PORT_ENA, PORT_ENA_b, TXENA, tx, ETHER_SWITCH_TYPE_ETHSW);

    return FSP_SUCCESS;
}

#ifdef ETHER_SWITCH_CONFIG_PTP
/***********************************************************************************************************************
 * Function Name: ether_switch_ptp_init
 * Description  : Enable Tx timestamping for an external port
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_ptp_init(R_ETHSW_Type volatile *  p_switch_reg,
                                       ether_switch_cmd_t *     p_cmd)
{
    uint32_t    reg_config;         /* CONFIG */

    uint32_t    reg_int_config;     /* INT_CONFIG */

    if ( ETHER_SWITCH_PORT_COUNT <= p_cmd->port)
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

    reg_config     = p_switch_reg->TSM_CONFIG;      /* CONFIG register */
    reg_int_config = p_switch_reg->INT_CONFIG;  /* INT_CONFIG register */

    if (ETHER_SWITCH_FALSE != *p_cmd->data.p_flag_enable)
    {
        /* enable options */
        reg_config |= (CONFIG_IRQ_EN | CONFIG_IRQ_TSFIFO_OVR | (1 << (p_cmd->port + CONFIG_IRQ_TX_EN_SHIFT)));
        ETHER_SWITCH_REG_SET(p_switch_reg->TSM_CONFIG, reg_config);

        reg_int_config |= (INT_CONFIG_IRQ_EN | INT_CONFIG_TSM_INT);
        ETHER_SWITCH_REG_SET(p_switch_reg->INT_CONFIG, reg_int_config);
    }
    else
    {
        /* disable options */
        reg_config &= (~(1 << (p_cmd->port + CONFIG_IRQ_TX_EN_SHIFT)));

        if(0 == (reg_config & CONFIG_IRQ_TX_EN_MASK))
        {
            reg_config &= (~CONFIG_IRQ_TSFIFO_OVR);

            reg_int_config &= (~INT_CONFIG_TSM_INT);
            ETHER_SWITCH_REG_SET(p_switch_reg->INT_CONFIG, reg_int_config);
        }
        ETHER_SWITCH_REG_SET(p_switch_reg->TSM_CONFIG, reg_config);
    }

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_latency_egress_get
 * Description  : Get the latancy between leaving PHY and timestamping of outgoing frames
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_latency_egress_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    uint32_t port;      /* port */

    /* this function only works without a portmask */
    if (p_cmd->port & ETHER_SWITCH_PORT_MASK)
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }
    else
    {
        port = (p_cmd->port & ETHER_SWITCH_PORT_HOST) ? portHost : p_cmd->port;
    }

    /* check for valid port */
    if ((switchPorts <= port) || (portHost == port))
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

    /* store egress latency */
    *p_cmd->data.p_latency_egress = switch_config[port].latency_egress;

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_latency_ingress_get
 * Description  : Get the latency between receiving in PHY and timestamping of incoming frames
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_latency_ingress_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    uint32_t port;      /* port */

    /* this function only works without a portmask */
    if (p_cmd->port & ETHER_SWITCH_PORT_MASK)
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }
    else
    {
        port = (p_cmd->port & ETHER_SWITCH_PORT_HOST) ? portHost : p_cmd->port;
    }

    /* check for valid port */
    if ((switchPorts <= port) || (portHost == port))
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

    /* store ingress latency */
    *p_cmd->data.p_latency_ingress = switch_config[port].latency_ingress;

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_timer_active
 * Description  : Enables / disables the timer with the specified timer number.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 **********************************************************************************************************************/
#define ATIME_INC_CLOCK_PERIOD_SHIFT    0
#define ATIME_INC_CLOCK_PERIOD_MASK     (0x7F << ATIME_INC_CLOCK_PERIOD_SHIFT)

static fsp_err_t ether_switch_timer_active(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    volatile uint32_t * p_atime_ctrl;   /* time control for time */
    volatile uint32_t * p_atime_inc;    /* Clock period of the timestamping clock */

    if (ETHER_SWITCH_TIME_COUNT <= p_cmd->data.p_time_enable->tim_num)
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

    p_atime_ctrl = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->ATIME_CTRL0 +      /* Pointer ATIME_CTRL[n] register */
                        ETHER_SWITCH_PORT_OFFSET_32BYTES * p_cmd->data.p_time_enable->tim_num);
    p_atime_inc  = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->ATIME_INC0 +       /* Pointer ATIME_INC[n] register */
                        ETHER_SWITCH_PORT_OFFSET_32BYTES * p_cmd->data.p_time_enable->tim_num);

    if (ETHER_SWITCH_TRUE == p_cmd->data.p_time_enable->flag_enable)
    {
        /* enable operation */
        *p_atime_ctrl |= ATIME_CTRL_ENABLE;

        *p_atime_inc  &= (~ATIME_INC_CLOCK_PERIOD_MASK);
        *p_atime_inc  |= ((p_cmd->data.p_time_enable->clock_period << ATIME_INC_CLOCK_PERIOD_SHIFT) & ATIME_INC_CLOCK_PERIOD_MASK);
    }
    else
    {
        /* disable operation */
        *p_atime_ctrl &= (~ATIME_CTRL_ENABLE);
    }

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_rx_port_timer  called by ETHER_SWITCH_CMD_TIMESTAMP_RX
 * Description  : Selects the timer for the respective input port timestamping 
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd->port
 *              :       - Port number to select timer for
 *              : p_cmd->data.p_timer
 *              :       - pointer to uint8_t with timer number to use for timestamping rx-frames on this port
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 *              : FSP_ERR_TIMEOUT
 *              :       - Timeout error
 **********************************************************************************************************************/
static fsp_err_t ether_switch_rx_port_timer(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    __attribute__((unused))R_ETHSW_Type volatile        * p_reg_switch;

    switch (p_cmd->port)
    {
        case ETHER_SWITCH_PORT_0:
            p_switch_reg->COMMAND_CONFIG_P0  |= *p_cmd->data.p_timer << 30;
            break;

        case ETHER_SWITCH_PORT_1:
            p_switch_reg->COMMAND_CONFIG_P1  |= *p_cmd->data.p_timer << 30;
            break;

        case ETHER_SWITCH_PORT_2:
            p_switch_reg->COMMAND_CONFIG_P2  |= *p_cmd->data.p_timer << 30;
            break;

        default:
            return FSP_ERR_INVALID_ARGUMENT;
            break;
    }

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_timer_get
 * Description  : Gets current timer
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 *              : FSP_ERR_TIMEOUT
 *              :       - Timeout error
 **********************************************************************************************************************/
fsp_err_t ether_switch_timer_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    volatile uint32_t * p_atime_ctrl;   /* time control for time */
    volatile uint32_t * p_atime;        /* nanoseconds time value */
    volatile uint32_t * p_atime_sec;    /* seconds time value */
    uint64_t            timeout;    /* timeout */

    if (ETHER_SWITCH_TIME_COUNT <= p_cmd->data.p_timestamp->tim_num)
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

    p_atime_ctrl = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->ATIME_CTRL0 +      /* Pointer ATIME_CTRL[n] register */
                        ETHER_SWITCH_PORT_OFFSET_32BYTES * p_cmd->data.p_timestamp->tim_num);
    p_atime      = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->ATIME0 +           /* Pointer ATIME[n] register */
                        ETHER_SWITCH_PORT_OFFSET_32BYTES * p_cmd->data.p_timestamp->tim_num);
    p_atime_sec  = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->ATIME_SEC0 +       /* Pointer ATIME_SEC[n] register */
                        ETHER_SWITCH_PORT_OFFSET_32BYTES * p_cmd->data.p_timestamp->tim_num);

    *p_atime_ctrl |= ATIME_CTRL_CAPTURE;

    timeout = Get_Time_Stamp() + SW_RN_ADVSW_TIMEOUT_MS;
    while (ATIME_CTRL_CAPTURE == (*p_atime_ctrl & ATIME_CTRL_CAPTURE))
    {
        if (Get_Time_Stamp() >= timeout)
        {
            p_cmd->data.p_timestamp->tim_sec  = 0;
            p_cmd->data.p_timestamp->tim_nsec = 0;
            p_cmd->data.p_timestamp->timestamp_id = 0;

            return FSP_ERR_TIMEOUT;
        }
    }

    p_cmd->data.p_timestamp->tim_sec  = *p_atime_sec;
    p_cmd->data.p_timestamp->tim_nsec = *p_atime;
    p_cmd->data.p_timestamp->timestamp_id = 0;

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_ptp_domain
 * Description  : Sets / gets the domain number in the specified timer
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_ptp_domain(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    uint32_t    domain_mask;
    uint32_t    domain;

    if (ETHER_SWITCH_TIME_COUNT <= p_cmd->data.p_domain->tim_num)
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

    domain_mask = PTP_DOMAIN_MASK << (p_cmd->data.p_domain->tim_num * PTP_DOMAIN_SHIFT);

    domain  = p_switch_reg->PTP_DOMAIN;

    if (p_cmd->flg_set) 
    {
        domain &= (~domain_mask);
        domain |= (p_cmd->data.p_domain->domain_num << (p_cmd->data.p_domain->tim_num * PTP_DOMAIN_SHIFT));
        p_switch_reg->PTP_DOMAIN = domain;
    }
    else
    {
        domain &= domain_mask;
        p_cmd->data.p_domain->domain_num = (uint8_t)(domain >> (p_cmd->data.p_domain->tim_num * PTP_DOMAIN_SHIFT));
    }
    
    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_ptp_peerdelay
 * Description  : Sets / gets the peer delay value for the specified timer number of the specified port number.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_ptp_peerdelay(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    volatile uint32_t * p_peerdelay;
    uint32_t            port;

    /* this function only works without a portmask */
    if (p_cmd->port & ETHER_SWITCH_PORT_MASK)
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }
    else
    {
        port = (p_cmd->port & ETHER_SWITCH_PORT_HOST) ? portHost : p_cmd->port;
    }

    /* check for valid port */
    if ((switchPorts <= port) || (portHost == port))
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

    if (ETHER_SWITCH_TIME_COUNT <= p_cmd->data.p_peerdelay->tim_num)
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

    p_peerdelay = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->PEERDELAY_P0_T0 +
                        (ETHER_SWITCH_PORT_OFFSET_16BYTES * p_cmd->port) +
                        (ETHER_SWITCH_PORT_OFFSET_4BYTES  * p_cmd->data.p_peerdelay->tim_num));

    if (p_cmd->flg_set) 
    {
        *p_peerdelay = p_cmd->data.p_peerdelay->peerdelay;
    }
    else
    {
        p_cmd->data.p_peerdelay->peerdelay = *p_peerdelay;
    }
    
    return FSP_SUCCESS;
}



/***********************************************************************************************************************
 * Function Name: ether_switch_ptp_callback
 * Description  : Register a callback function that receives a send timestamp event.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 ***********************************************************************************************************************/
fsp_err_t ether_switch_ptp_callback(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    p_ptp_callback = p_cmd->data.p_ptp_callback;
    
    return FSP_SUCCESS;
}

#define SWITCH_TX_TS_DELAY  (1*8 + 8 + 0)    // Use upper valueof range, so that cable delay cannot become negative,      
/***********************************************************************************************************************
 * Function Name: ether_switch_isr_ptp[n]
 * Description  : Interrupt handler for PTP
 *                This function is called by ether_switch_isr_intr
 * Arguments    : p_instance_ctrl -
 *                    pointer to control structure
 *                -
 * Return Value : none
 ***********************************************************************************************************************/
/* S_FIFO_READ_CTRL Register Bit Definitions */
#define TS_FIFO_READ_CTRL_PORT_NUM_SHIFT    0
#define TS_FIFO_READ_CTRL_PORT_NUM_MASK     (0xf << TS_FIFO_READ_CTRL_PORT_NUM_SHIFT)
#define TS_FIFO_READ_CTRL_TS_VALID          1 << 4
#define TS_FIFO_READ_CTRL_TS_SEL_SHIFT      6
#define TS_FIFO_READ_CTRL_TS_SEL_MASK       (0x3 << TS_FIFO_READ_CTRL_TS_SEL_SHIFT)
#define TS_FIFO_READ_CTRL_TS_ID_SHIFT       8
#define TS_FIFO_READ_CTRL_TS_ID_MASK        (0x7f << TS_FIFO_READ_CTRL_TS_ID_SHIFT)
#define CAPTURE (1 << 11)

static void ether_switch_isr_tsm (ether_switch_instance_ctrl_t * p_instance_ctrl)
{							  
	uint32_t                 irq_stat_ack;
	R_ETHSW_Type *           p_switch_reg;
	ether_switch_timestamp_t timestamp;
	
	p_switch_reg = (R_ETHSW_Type *)p_instance_ctrl->p_reg_switch;
	irq_stat_ack = p_switch_reg->TSM_IRQ_STAT_ACK;
	
	uint8_t portnumber = 0;
	
	// Find port on which interrupt occured. If multiple TX timest. triggered only the lowest is handled, new interupt for next
	if ((irq_stat_ack & (0x01 << (16 + portnumber))) == 0) // TxTimestamp Interrupt on this port
  {
		++portnumber;
		
		if (portnumber > 2) 
    {           
			return;
		}
	}
    
      int64_t  TSnano =  0;
      uint32_t tmp    =  0;

  	  p_switch_reg->TSM_IRQ_STAT_ACK = 0x01 << (16+portnumber);   // Clear this interrupt
      p_switch_reg->TS_FIFO_READ_CTRL = portnumber;
      tmp = p_switch_reg->TS_FIFO_READ_CTRL;
      
      while (tmp & 0x010) 
      {
        TSnano = p_switch_reg->TS_FIFO_READ_TIMESTAMP;
        timestamp.timestamp_id = (tmp & 0x7f00) >> 8; // Read Tag.

    #if 1
        if (TSnano > SWITCH_TX_TS_DELAY)
        {
          TSnano = TSnano - SWITCH_TX_TS_DELAY;   // Adjust for Timestamp measurement delay        
        }
        else
        {
          TSnano = TSnano + 1000000000 - SWITCH_TX_TS_DELAY;
        }
    #endif
        timestamp.tim_nsec = TSnano;
        
        if (p_ptp_callback)
        {
          if(ptp_time_stamp_nanos[timestamp.timestamp_id] != 0)
          {
            (* p_ptp_callback)(ETHER_SWITCH_PTP_CALLBACK_TS_VALID,portnumber, TSnano, (ether_switch_timestamp_t*)ptp_time_stamp_nanos[timestamp.timestamp_id]);
            ptp_time_stamp_nanos[timestamp.timestamp_id]  = 0; // Handshake
          }
        }
        
        //p_switch_reg->TS_FIFO_READ_CTRL = portnumber;
        tmp = p_switch_reg->TS_FIFO_READ_CTRL;
      } // end while        
	
} /* End of function ether_switch_isr_tsm() */

#endif /* ETHER_SWITCH_CONFIG_PTP */


#ifdef ETHER_SWITCH_CONFIG_TDMA
/***********************************************************************************************************************
 * Function Name: ether_switch_tdma_enable_set
 * Description  : Sets the state of the TDMA scheduler.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 ***********************************************************************************************************************/
static fsp_err_t ether_switch_tdma_enable_set(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    ether_switch_tdma_enable_t * p_tdma_enable = p_cmd->data.p_tdma_enable;
    uint32_t                     portmask;

	//TODO: -OS- enable, off for sync debugging 
    //-OS- if(ETHER_SWITCH_TRUE == p_tdma_enable->flg_enable)
	if(0) //-OS-
    {
        /* enable operation */
        if ((!(p_cmd->data.p_tdma_enable->portmask & ETHER_SWITCH_PORT_MASK)) ||
            (p_cmd->data.p_tdma_enable->portmask & ETHER_SWITCH_PORT_HOST))
        {
            return FSP_ERR_INVALID_ARGUMENT;
        }

        portmask = p_tdma_enable->portmask & (~ETHER_SWITCH_PORT_MASK);

        if ((TDMA_ENA_CTRL_PORT_ENA_MASK < portmask) ||
            (TDMA_CONFIG_TIMER_SEL_MASK < p_tdma_enable->tim_num))
        {
            return FSP_ERR_INVALID_ARGUMENT;
        }

        p_tdma_callback = p_tdma_enable->p_tdma_callback;

        p_switch_reg->TDMA_START  = p_tdma_enable->tdma_start;
        p_switch_reg->TDMA_MODULO = p_tdma_enable->tdma_modulo;
        p_switch_reg->TDMA_CYCLE  = p_tdma_enable->tdma_cycle;
        p_switch_reg->TDMA_ENA_CTRL_b.PORT_ENA = p_tdma_enable->portmask & TDMA_ENA_CTRL_PORT_ENA_MASK;
        p_switch_reg->TDMA_CONFIG_b.TIMER_SEL  = p_tdma_enable->tim_num;

        p_switch_reg->TDMA_IRQ_CONTROL_b.TCV_INT_EN = 1;
        p_switch_reg->INT_CONFIG_b.TDMA_INT         = 1;
        p_switch_reg->INT_CONFIG_b.IRQ_EN           = 1;
        p_switch_reg->TDMA_CONFIG_b.TDMA_ENA        = 1;
    }
    else
    {
        /* disable operation */
        p_switch_reg->TDMA_CONFIG_b.TDMA_ENA        = 0;
        p_switch_reg->TDMA_IRQ_CONTROL_b.TCV_INT_EN = 0;
        p_switch_reg->INT_CONFIG_b.TDMA_INT         = 0;
    }

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_tdma_enable_get
 * Description  : Gets the state of the TDMA scheduler.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 ***********************************************************************************************************************/
static fsp_err_t ether_switch_tdma_enable_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    ether_switch_tdma_enable_t * p_tdma_enable = p_cmd->data.p_tdma_enable;

    if(p_switch_reg->TDMA_CONFIG_b.TDMA_ENA == 1)
    {
        /* enable operation */
        p_tdma_enable->tdma_start  = p_switch_reg->TDMA_START;
        p_tdma_enable->tdma_modulo = p_switch_reg->TDMA_MODULO;
        p_tdma_enable->tdma_cycle  = p_switch_reg->TDMA_CYCLE;
        p_tdma_enable->portmask    = p_switch_reg->TDMA_ENA_CTRL_b.PORT_ENA|ETHER_SWITCH_PORT_MASK;
        p_tdma_enable->tim_num     = p_switch_reg->TDMA_CONFIG_b.TIMER_SEL_ACTIVE;
        p_tdma_enable->flg_enable  = ETHER_SWITCH_TRUE;
    }
    else
    {
        /* disable operation */
        p_tdma_enable->tdma_start  = 0;
        p_tdma_enable->tdma_modulo = 0;
        p_tdma_enable->tdma_cycle  = 0;
        p_tdma_enable->portmask    = ETHER_SWITCH_PORT_MASK;
        p_tdma_enable->tim_num     = 0;
        p_tdma_enable->flg_enable  = ETHER_SWITCH_FALSE;
    }
    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_tdma_tcv_seq_entry_set
 * Description  : Sets TCV sequence entry.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 ***********************************************************************************************************************/
static fsp_err_t ether_switch_tdma_tcv_seq_entry_set(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    ether_switch_tdma_seq_entry_t * p_tdma_seq_entry = p_cmd->data.p_tdma_seq_entry;
    uint32_t                        tcv_seq = 0;

    if (((TDMA_TCV_SEQ_D_IDX_MASK >> TDMA_TCV_SEQ_D_IDX_SHIFT) < p_tdma_seq_entry->tcv_d_index) ||
        ((TDMA_TCV_SEQ_S_ADDR_MASK >> TDMA_TCV_SEQ_S_ADDR_SHIFT) < p_tdma_seq_entry->tcv_s_addr))
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

    if (ETHER_SWITCH_TDMA_TCV_START == (p_tdma_seq_entry->tcv_func & ETHER_SWITCH_TDMA_TCV_START))
    {
        tcv_seq |= TDMA_TCV_SEQ_START;
    }

    if (ETHER_SWITCH_TDMA_TCV_INT == (p_tdma_seq_entry->tcv_func & ETHER_SWITCH_TDMA_TCV_INT))
    {
        tcv_seq |= TDMA_TCV_SEQ_INT;
    }

    tcv_seq |= (p_tdma_seq_entry->tcv_d_index << TDMA_TCV_SEQ_D_IDX_SHIFT) & TDMA_TCV_SEQ_D_IDX_MASK;
#ifdef WS2 /* FPGA Ver2 */
    p_switch_reg->TCV_SEQ_ADDR = (p_tdma_seq_entry->tcv_s_addr  << TDMA_TCV_SEQ_S_ADDR_SHIFT) & TDMA_TCV_SEQ_S_ADDR_MASK;
    p_switch_reg->TCV_SEQ_CTRL = tcv_seq;
#else /* !WS2 */ /* FPGA Ver1 */
    tcv_seq |= (p_tdma_seq_entry->tcv_s_addr  << TDMA_TCV_SEQ_S_ADDR_SHIFT) & TDMA_TCV_SEQ_S_ADDR_MASK;
    p_switch_reg->TCV_SEQ = tcv_seq;
#endif /* !WS2 */ /* FPGA Ver1 */

    return FSP_SUCCESS;
}
/***********************************************************************************************************************
 * Function Name: ether_switch_tdma_tcv_seq_entry_get
 * Description  : Gets TCV sequence entry.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 ***********************************************************************************************************************/
static fsp_err_t ether_switch_tdma_tcv_seq_entry_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    ether_switch_tdma_seq_entry_t * p_tdma_seq_entry = p_cmd->data.p_tdma_seq_entry;
    uint32_t                        tcv_seq;

    if ((TDMA_TCV_SEQ_S_ADDR_MASK >> TDMA_TCV_SEQ_S_ADDR_SHIFT) < p_tdma_seq_entry->tcv_s_addr)
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

#ifdef WS2 /* FPGA Ver2 */
    p_switch_reg->TCV_SEQ_ADDR = (p_tdma_seq_entry->tcv_s_addr  << TDMA_TCV_SEQ_S_ADDR_SHIFT) & TDMA_TCV_SEQ_S_ADDR_MASK;
    p_switch_reg->TCV_SEQ_CTRL = TDMA_TCV_SEQ_READ_MODE;

    tcv_seq = p_switch_reg->TCV_SEQ_CTRL;
#else /* !WS2 */ /* FPGA Ver1 */
    tcv_seq = (p_tdma_seq_entry->tcv_s_addr << TDMA_TCV_SEQ_S_ADDR_SHIFT) & TDMA_TCV_SEQ_S_ADDR_MASK;
    tcv_seq |= TDMA_TCV_SEQ_READ_MODE;

    p_switch_reg->TCV_SEQ = tcv_seq;

    tcv_seq = p_switch_reg->TCV_SEQ;
#endif /* !WS2 */ /* FPGA Ver1 */
    p_tdma_seq_entry->tcv_func = ETHER_SWITCH_TDMA_TCV_NON;

    if (tcv_seq & TDMA_TCV_SEQ_START)
    {
        p_tdma_seq_entry->tcv_func |= ETHER_SWITCH_TDMA_TCV_START;
    }

    if (tcv_seq & TDMA_TCV_SEQ_INT)
    {
        p_tdma_seq_entry->tcv_func |= ETHER_SWITCH_TDMA_TCV_INT;
    }

    p_tdma_seq_entry->tcv_d_index = (tcv_seq & TDMA_TCV_SEQ_D_IDX_MASK) >> TDMA_TCV_SEQ_D_IDX_SHIFT;

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_tdma_tcv_dat_entry_set
 * Description  : Sets TCV data entry.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 ***********************************************************************************************************************/
#define TCV_DATA_ADDR_MASK  0x1FF
#define TCV_DATA_QGATE_MASK 0xFF
#define TCV_DATA_PMASK_MASK 0x7

static fsp_err_t ether_switch_tdma_tcv_dat_entry_set(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    uint32_t    tcv_d_pmask;

    ether_switch_tdma_dat_entry_t * p_tdma_dat_entry = p_cmd->data.p_tdma_dat_entry;

    if ((!(p_tdma_dat_entry->tcv_d_pmask & ETHER_SWITCH_PORT_MASK)) ||
          (p_tdma_dat_entry->tcv_d_pmask & ETHER_SWITCH_PORT_HOST))
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

    tcv_d_pmask = p_tdma_dat_entry->tcv_d_pmask & (~ETHER_SWITCH_PORT_MASK);

    if ((TCV_DATA_ADDR_MASK < p_tdma_dat_entry->tcv_d_addr)   ||
        (TCV_DATA_QGATE_MASK < p_tdma_dat_entry->tcv_d_qgate) ||
        (TCV_DATA_PMASK_MASK < tcv_d_pmask))
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

    p_switch_reg->TCV_D_ADDR_b.ADDR  = (p_tdma_dat_entry->tcv_d_addr   & TCV_DATA_ADDR_MASK);
    p_switch_reg->TCV_D_OFFSET       =  p_tdma_dat_entry->tcv_d_offset;
    p_switch_reg->TCV_D_CTRL_b.QGATE = (p_tdma_dat_entry->tcv_d_qgate  & TCV_DATA_QGATE_MASK);
    p_switch_reg->TCV_D_CTRL_b.PMASK = (p_tdma_dat_entry->tcv_d_pmask  & TCV_DATA_PMASK_MASK);
#ifdef WS2 /* FPGA Ver2 */
    p_switch_reg->TCV_D_CTRL_b.GATE_MODE = 1;
#endif /* WS2 */ /* FPGA Ver2 */

    return FSP_SUCCESS;
}
/***********************************************************************************************************************
 * Function Name: ether_switch_tdma_tcv_dat_entry_get
 * Description  : Gets TCV data entry.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 ***********************************************************************************************************************/
static fsp_err_t ether_switch_tdma_tcv_dat_entry_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    ether_switch_tdma_dat_entry_t * p_tdma_dat_entry = p_cmd->data.p_tdma_dat_entry;

    if (TCV_DATA_ADDR_MASK < p_tdma_dat_entry->tcv_d_addr)
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

    p_switch_reg->TCV_D_ADDR_b.ADDR = p_tdma_dat_entry->tcv_d_addr;

    p_tdma_dat_entry->tcv_d_offset = p_switch_reg->TCV_D_OFFSET;
    p_tdma_dat_entry->tcv_d_qgate  = p_switch_reg->TCV_D_CTRL_b.QGATE;
    p_tdma_dat_entry->tcv_d_pmask  = p_switch_reg->TCV_D_CTRL_b.PMASK | ETHER_SWITCH_PORT_MASK;

    return FSP_SUCCESS;
}
/***********************************************************************************************************************
 * Function Name: ether_switch_tdma_tcv_seq_range_set
 * Description  : Sets start and last of TCV sequence entry.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 ***********************************************************************************************************************/
#define TCV_S_ADDR_MASK 0xfff

static fsp_err_t ether_switch_tdma_tcv_seq_range_set(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    ether_switch_tdma_seq_range_t * p_tdma_seq_range = p_cmd->data.p_tdma_seq_range;

    if ((TCV_S_ADDR_MASK < p_tdma_seq_range->tcv_start) ||
        (TCV_S_ADDR_MASK < p_tdma_seq_range->tcv_last))
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

    p_switch_reg->TDMA_TCV_START      = p_tdma_seq_range->tcv_start;
    p_switch_reg->TCV_SEQ_LAST_b.LAST = p_tdma_seq_range->tcv_last;

    return FSP_SUCCESS;
}
/***********************************************************************************************************************
 * Function Name: ether_switch_tdma_tcv_seq_range_set
 * Description  : Gets start and last of TCV sequence entry.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 ***********************************************************************************************************************/
static fsp_err_t ether_switch_tdma_tcv_seq_range_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    ether_switch_tdma_seq_range_t * p_tdma_seq_range = p_cmd->data.p_tdma_seq_range;

    p_tdma_seq_range->tcv_start = p_switch_reg->TDMA_TCV_START;
    p_tdma_seq_range->tcv_last  = p_switch_reg->TCV_SEQ_LAST_b.LAST;

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_mmctl_qgate_set
 * Description  : Sets queue gate
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 ***********************************************************************************************************************/
static fsp_err_t ether_switch_mmctl_qgate_set(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    ether_switch_mmclt_qgate_t *    p_mmctl_qgate = p_cmd->data.p_mmctl_qgate;
    uint32_t                        mmctl_qgate;

    if ((!(p_cmd->data.p_mmctl_qgate->portmask & ETHER_SWITCH_PORT_MASK)) ||
          (p_cmd->data.p_mmctl_qgate->portmask & ETHER_SWITCH_PORT_HOST))
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

    mmctl_qgate  = ((p_mmctl_qgate->qgate << MMCTL_QTRIG_QUEUE_TRIG_SHIFT) & MMCTL_QTRIG_QUEUE_TRIG_MASK);
    mmctl_qgate |= ((p_mmctl_qgate->portmask << MMCTL_QTRIG_PORT_MASK_SHIFT) & MMCTL_QTRIG_PORT_MASK_MASK);
    p_switch_reg->MMCTL_QGATE = mmctl_qgate;

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_isr_tdma
 * Description  : Interrupt handler for TDMA
 *                This function is called by ether_switch_isr_intr
 * Arguments    : p_instance_ctrl -
 *                    pointer to control structure
 *                -
 * Return Value : none
 ***********************************************************************************************************************/
static void ether_switch_isr_tdma (ether_switch_instance_ctrl_t * p_instance_ctrl)
{
    ether_switch_tdma_callback_data_t   callback_data;
    R_ETHSW_Type *                      p_switch_reg;
    uint32_t                            tdma_irq_stat_ack;

    p_switch_reg = (R_ETHSW_Type *)p_instance_ctrl->p_reg_switch;
    tdma_irq_stat_ack = p_switch_reg->TDMA_IRQ_STAT_ACK;
    p_switch_reg->TDMA_IRQ_STAT_ACK = tdma_irq_stat_ack;

    if (TDMA_IRQ_CONTROL_TCV_INT_EN == (tdma_irq_stat_ack &TDMA_IRQ_CONTROL_TCV_INT_EN))
    {
        if( 0 != p_tdma_callback )
        {
            callback_data.tcv_s_idx = (tdma_irq_stat_ack & TDMA_IRQ_CONTROL_TCV_IDX_INT_ENA_MASK) >> TDMA_IRQ_CONTROL_TCV_IDX_INT_ENA_SHIFT;
            (* p_tdma_callback)(ETHER_SWITCH_TDMA_CALLBACK_TCV_INT, &callback_data);
        }
    }
}

#endif /* ETHER_SWITCH_CONFIG_TDMA */

#ifdef ETHER_SWITCH_CONFIG_PRIORITY3_OTHER
/***********************************************************************************************************************
 * Function Name: ether_switch_mirror_set
 * Description  : Sets the dedicated port that will received all frames that are mirrored as
 *                well as the type of mirrored frames.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_mirror_set(R_ETHSW_Type volatile *p_switch_reg,
                                         ether_switch_cmd_t *   p_cmd)
{
    volatile char * p_reg;                      /* Pointer to reg */
    ether_switch_mac_addr_t addr;               /* mac address buffer */

    /* Disable port mirroring for reconfig */
    ETHER_SWITCH_REG_SET(p_switch_reg->MIRROR_CONTROL, 0);

    /* Configure specific mode */
    switch (p_cmd->data.p_cnf_mirror->mirrortype)
    {

        /* Match egress dest. address */
        case ETHER_SWITCH_MIRR_MODE_EGRESS_DA:
            ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, MIRROR_CONTROL, MIRROR_CONTROL_b, EG_DA_MATCH, ETHER_SWITCH_TRUE, ETHER_SWITCH_TYPE_ETHSW);
            p_reg = (volatile char *) &(p_switch_reg->MIRROR_EDST_0);

            /* copy mac address to get valid alignment */
            memcpy(addr, *p_cmd->data.p_cnf_mirror->macaddr, ETHER_SWITCH_MAC_ADDR_LEN);

            /* copy aligned mac address into register */
            ether_switch_memcpy_vol(p_reg, addr, ETHER_SWITCH_MAC_ADDR_LEN);
            break;

        /* Match egress src. address */
        case ETHER_SWITCH_MIRR_MODE_EGRESS_SA:
            ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, MIRROR_CONTROL, MIRROR_CONTROL_b, EG_SA_MATCH, ETHER_SWITCH_TRUE, ETHER_SWITCH_TYPE_ETHSW);
            p_reg = (volatile char *) &(p_switch_reg->MIRROR_ESRC_0);

            /* copy mac address to get valid alignment */
            memcpy(addr, *p_cmd->data.p_cnf_mirror->macaddr, ETHER_SWITCH_MAC_ADDR_LEN);

            /* copy aligned mac address into register */
            ether_switch_memcpy_vol(p_reg, addr, ETHER_SWITCH_MAC_ADDR_LEN);
            break;

        /* Match ingress dest. address */
        case ETHER_SWITCH_MIRR_MODE_INGRESS_DA:
            ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, MIRROR_CONTROL, MIRROR_CONTROL_b, ING_DA_MATCH, ETHER_SWITCH_TRUE, ETHER_SWITCH_TYPE_ETHSW);
            p_reg = (volatile char *) &(p_switch_reg->MIRROR_IDST_0);

            /* copy mac address to get valid alignment */
            memcpy(addr, *p_cmd->data.p_cnf_mirror->macaddr, ETHER_SWITCH_MAC_ADDR_LEN);

            /* copy aligned mac address into register */
            ether_switch_memcpy_vol(p_reg, addr, ETHER_SWITCH_MAC_ADDR_LEN);
            break;

        /* Match ingress src. address */
        case ETHER_SWITCH_MIRR_MODE_INGRESS_SA:
            ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, MIRROR_CONTROL, MIRROR_CONTROL_b, ING_SA_MATCH, ETHER_SWITCH_TRUE, ETHER_SWITCH_TYPE_ETHSW);
            p_reg = (volatile char *) &(p_switch_reg->MIRROR_ISRC_0);

            /* copy mac address to get valid alignment */
            memcpy(addr, *p_cmd->data.p_cnf_mirror->macaddr, ETHER_SWITCH_MAC_ADDR_LEN);

            /* copy aligned mac address into register */
            ether_switch_memcpy_vol(p_reg, addr, ETHER_SWITCH_MAC_ADDR_LEN);
            break;

        /* Match egress port */
        case ETHER_SWITCH_MIRR_MODE_EGRESS_PORT:
            ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, MIRROR_CONTROL, MIRROR_CONTROL_b, EG_MAP_EN, ETHER_SWITCH_TRUE, ETHER_SWITCH_TYPE_ETHSW);
            ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, MIRROR_EG_MAP, MIRROR_EG_MAP_b, EMAP, (uint8_t) (p_cmd->data.p_cnf_mirror->portmap & (~ETHER_SWITCH_PORT_MASK)), ETHER_SWITCH_TYPE_ETHSW);
            break;

        /* Match ingress port */
        case ETHER_SWITCH_MIRR_MODE_INGRESS_PORT:
            ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, MIRROR_CONTROL, MIRROR_CONTROL_b, ING_MAP_EN, ETHER_SWITCH_TRUE, ETHER_SWITCH_TYPE_ETHSW);
            ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, MIRROR_ING_MAP, MIRROR_ING_MAP_b, IMAP, (uint8_t) (p_cmd->data.p_cnf_mirror->portmap & (~ETHER_SWITCH_PORT_MASK)), ETHER_SWITCH_TYPE_ETHSW);
            break;

        default:
            break;
    }

    /* Set mirror port and enable mirroring */
    ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, MIRROR_CONTROL, MIRROR_CONTROL_b, PORT, p_cmd->port, ETHER_SWITCH_TYPE_ETHSW);
    ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, MIRROR_CONTROL, MIRROR_CONTROL_b, MIRROR_EN, ETHER_SWITCH_TRUE, ETHER_SWITCH_TYPE_ETHSW);

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_mirror_get
 * Description  : Gets the current port mirroring configuration
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_mirror_get(R_ETHSW_Type volatile *p_switch_reg,
                                         ether_switch_cmd_t *   p_cmd)
{
    p_cmd->port = p_switch_reg->MIRROR_CONTROL_b.PORT;

    /* Mode dependent settings */

    /* Match egress dest. address */
    if (p_switch_reg->MIRROR_CONTROL_b.EG_DA_MATCH)
    {
        p_cmd->data.p_cnf_mirror->mirrortype = ETHER_SWITCH_MIRR_MODE_EGRESS_DA;
        ether_switch_memcpy_vol(*p_cmd->data.p_cnf_mirror->macaddr, &p_switch_reg->MIRROR_EDST_0, ETHER_SWITCH_MAC_ADDR_LEN);

        /* Match egress src. address */
    }
    if (p_switch_reg->MIRROR_CONTROL_b.EG_SA_MATCH)
    {
        p_cmd->data.p_cnf_mirror->mirrortype = ETHER_SWITCH_MIRR_MODE_EGRESS_SA;
        ether_switch_memcpy_vol(*p_cmd->data.p_cnf_mirror->macaddr, &p_switch_reg->MIRROR_ESRC_0, ETHER_SWITCH_MAC_ADDR_LEN);

        /* Match ingress dest. address */
    }
    else if (p_switch_reg->MIRROR_CONTROL_b.ING_DA_MATCH)
    {
        p_cmd->data.p_cnf_mirror->mirrortype = ETHER_SWITCH_MIRR_MODE_INGRESS_DA;
        ether_switch_memcpy_vol(*p_cmd->data.p_cnf_mirror->macaddr, &p_switch_reg->MIRROR_IDST_0, ETHER_SWITCH_MAC_ADDR_LEN);

        /* Match ingress src. address */
    }
    else if (p_switch_reg->MIRROR_CONTROL_b.ING_SA_MATCH)
    {
        p_cmd->data.p_cnf_mirror->mirrortype = ETHER_SWITCH_MIRR_MODE_INGRESS_SA;
        ether_switch_memcpy_vol(*p_cmd->data.p_cnf_mirror->macaddr, &p_switch_reg->MIRROR_ISRC_0, ETHER_SWITCH_MAC_ADDR_LEN);

        /* Match egress port */
    }
    else if (p_switch_reg->MIRROR_CONTROL_b.EG_MAP_EN)
    {
        p_cmd->data.p_cnf_mirror->mirrortype = ETHER_SWITCH_MIRR_MODE_EGRESS_PORT;
        p_cmd->data.p_cnf_mirror->portmap = (uint8_t) (p_switch_reg->MIRROR_EG_MAP_b.EMAP & (~ETHER_SWITCH_PORT_MASK));

        /* Match ingress port */
    }
    else if (p_switch_reg->MIRROR_CONTROL_b.ING_MAP_EN)
    {
        p_cmd->data.p_cnf_mirror->mirrortype = ETHER_SWITCH_MIRR_MODE_INGRESS_PORT;
        p_cmd->data.p_cnf_mirror->portmap = (uint8_t) (p_switch_reg->MIRROR_ING_MAP_b.IMAP & (~ETHER_SWITCH_PORT_MASK));
    }

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_qos_mode_set
 * Description  : Sets the priority resolution mode to use for a specific port.
 *                It also defines the default priority if no priority resolution produced a result.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
fsp_err_t ether_switch_qos_mode_set(R_ETHSW_Type volatile *  p_switch_reg,
                                           ether_switch_cmd_t *     p_cmd)
{
    volatile uint32_t *p_reg;                   /* register value */
    uint32_t port;                              /* port */

    port = (p_cmd->port & ETHER_SWITCH_PORT_HOST) ? portHost : p_cmd->port;
    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->PRIORITY_CFG[0] + ETHER_SWITCH_PORT_OFFSET_4BYTES * port);

    ETHER_SWITCH_REG_SET(* p_reg, (((uint32_t) p_cmd->data.p_mode_qos->defprio & ETHER_SWITCH_QOS_DEFPRIO_MASK) << ETHER_SWITCH_QOS_DEFPRIO_OFFSET)
                                | ((uint32_t) p_cmd->data.p_mode_qos->mode & ETHER_SWITCH_QOS_MODE_MASK));

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_qos_mode_get
 * Description  : Returns the priority resolution mode to use for a specific port.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_qos_mode_get(R_ETHSW_Type volatile *  p_switch_reg,
                                           ether_switch_cmd_t *     p_cmd)
{
    volatile uint32_t *p_reg;                   /* register value */
    uint32_t port;                              /* port */

    port = (p_cmd->port & ETHER_SWITCH_PORT_HOST) ? portHost : p_cmd->port;
    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->PRIORITY_CFG[0] + ETHER_SWITCH_PORT_OFFSET_4BYTES * port);

    /* Get mode and mask */
    p_cmd->data.p_mode_qos->mode = * p_reg & ETHER_SWITCH_QOS_MODE_MASK;
    p_cmd->data.p_mode_qos->defprio = (* p_reg >> ETHER_SWITCH_QOS_DEFPRIO_OFFSET) & ETHER_SWITCH_QOS_DEFPRIO_MASK;

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_qos_prio_vlan_set
 * Description  : Sets the priority mapping between VLAN tag priorities and the switch core priorities.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
fsp_err_t ether_switch_qos_prio_vlan_set(R_ETHSW_Type volatile * p_switch_reg,
                                                ether_switch_cmd_t *    p_cmd)
{
    volatile uint32_t * p_reg;              /* register value */
    uint32_t            port;               /* port */

    port = (p_cmd->port & ETHER_SWITCH_PORT_HOST) ? portHost : p_cmd->port;
    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->VLAN_PRIORITY[0] + ETHER_SWITCH_PORT_OFFSET_4BYTES * port);
    ETHER_SWITCH_REG_SET(*p_reg, *p_cmd->data.p_bm_prio_vlan & ETHER_SWITCH_QOS_VLANPRIO_MASK);

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_qos_prio_vlan_get
 * Description  : Gets the priority mapping between VLAN tag priorities and the switch core priorities.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_qos_prio_vlan_get(R_ETHSW_Type volatile * p_switch_reg,
                                                ether_switch_cmd_t *    p_cmd)
{
    volatile uint32_t * p_reg;              /* register value */
    uint32_t            port;               /* port */

    port = (p_cmd->port & ETHER_SWITCH_PORT_HOST) ? portHost : p_cmd->port;
    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->VLAN_PRIORITY[0] + ETHER_SWITCH_PORT_OFFSET_4BYTES * port);
    *p_cmd->data.p_bm_prio_vlan = *p_reg & ETHER_SWITCH_QOS_VLANPRIO_MASK;

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_qos_prio_ip_set
 * Description  : Sets the priority for the given DiffServ field of an IP packet for the given port.
 *                Up to 64 different DiffServ entries may be defined per port.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_qos_prio_ip_set(R_ETHSW_Type volatile *   p_switch_reg,
                                              ether_switch_cmd_t *      p_cmd)
{
    volatile uint32_t * p_reg;                  /* register value */
    uint32_t            port;                   /* port */

    port = (p_cmd->port & ETHER_SWITCH_PORT_HOST) ? portHost : p_cmd->port;
    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->IP_PRIORITY[0] + ETHER_SWITCH_PORT_OFFSET_4BYTES * port);
    ETHER_SWITCH_REG_SET(*p_reg, ETHER_SWITCH_QOS_IPPRIO_WRITE | ((uint32_t) p_cmd->data.p_prio_qos_ip->priority << ETHER_SWITCH_QOS_IPPRIO_OFFSET)
                                | ((uint32_t) p_cmd->data.p_prio_qos_ip->diffserv & ETHER_SWITCH_QOS_IPPRIO_DSMASK));

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_qos_prio_ip_get
 * Description  : Gets the priority for the given DiffServ field of an IP packet for the given port.
 *                Up to 64 different DiffServ entries may be defined per port.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_qos_prio_ip_get(R_ETHSW_Type volatile *   p_switch_reg,
                                              ether_switch_cmd_t *      p_cmd)
{
    volatile uint32_t * p_reg;                  /* register value */
    uint32_t            port;                   /* port */

    port = (p_cmd->port & ETHER_SWITCH_PORT_HOST) ? portHost : p_cmd->port;
    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->IP_PRIORITY[0] + ETHER_SWITCH_PORT_OFFSET_4BYTES * port);
    ETHER_SWITCH_REG_SET(*p_reg, ETHER_SWITCH_QOS_IPPRIO_READ | (p_cmd->data.p_prio_qos_ip->diffserv & ETHER_SWITCH_QOS_IPPRIO_DSMASK));

    p_cmd->data.p_prio_qos_ip->priority = (*p_reg >> ETHER_SWITCH_QOS_IPPRIO_OFFSET) & ETHER_SWITCH_QOS_IPPRIO_MASK;

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_qos_prio_type_set
 * Description  : Sets the priority for the given Ethertypes to the given values.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_qos_prio_type_set(R_ETHSW_Type volatile * p_switch_reg,
                                                ether_switch_cmd_t *    p_cmd)
{
    if (!p_cmd->data.p_prio_qos_ethtype->type1)
    {
        ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, PRIORITY_TYPE1, PRIORITY_TYPE1_b, VALID, ETHER_SWITCH_FALSE, ETHER_SWITCH_TYPE_ETHSW);
        ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, PRIORITY_TYPE1, PRIORITY_TYPE1_b, PRIORITY, 0, ETHER_SWITCH_TYPE_ETHSW);
        ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, PRIORITY_TYPE1, PRIORITY_TYPE1_b, TYPEVAL, 0, ETHER_SWITCH_TYPE_ETHSW);
    }
    else
    {
        ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, PRIORITY_TYPE1, PRIORITY_TYPE1_b, PRIORITY, p_cmd->data.p_prio_qos_ethtype->prio1, ETHER_SWITCH_TYPE_ETHSW);
        ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, PRIORITY_TYPE1, PRIORITY_TYPE1_b, TYPEVAL, p_cmd->data.p_prio_qos_ethtype->type1, ETHER_SWITCH_TYPE_ETHSW);
        ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, PRIORITY_TYPE1, PRIORITY_TYPE1_b, VALID, ETHER_SWITCH_TRUE, ETHER_SWITCH_TYPE_ETHSW);
    }

    if (!p_cmd->data.p_prio_qos_ethtype->type2)
    {
        ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, PRIORITY_TYPE2, PRIORITY_TYPE2_b, VALID, 0, ETHER_SWITCH_TYPE_ETHSW);
        ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, PRIORITY_TYPE2, PRIORITY_TYPE2_b, PRIORITY, 0, ETHER_SWITCH_TYPE_ETHSW);
        ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, PRIORITY_TYPE2, PRIORITY_TYPE2_b, TYPEVAL, 0, ETHER_SWITCH_TYPE_ETHSW);
    }
    else
    {
        ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, PRIORITY_TYPE2, PRIORITY_TYPE2_b, PRIORITY, p_cmd->data.p_prio_qos_ethtype->prio2, ETHER_SWITCH_TYPE_ETHSW);
        ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, PRIORITY_TYPE2, PRIORITY_TYPE2_b, TYPEVAL, p_cmd->data.p_prio_qos_ethtype->type2, ETHER_SWITCH_TYPE_ETHSW);
        ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, PRIORITY_TYPE2, PRIORITY_TYPE2_b, VALID, ETHER_SWITCH_TRUE, ETHER_SWITCH_TYPE_ETHSW);
    }

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_qos_prio_type_get
 * Description  : Gets the priority settings for type based priorities.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_qos_prio_type_get(R_ETHSW_Type volatile * p_switch_reg,
                                                ether_switch_cmd_t *    p_cmd)
{
    p_cmd->data.p_prio_qos_ethtype->type1 = (p_switch_reg->PRIORITY_TYPE1_b.VALID) ? p_switch_reg->PRIORITY_TYPE1_b.TYPEVAL : 0;
    p_cmd->data.p_prio_qos_ethtype->prio1 = p_switch_reg->PRIORITY_TYPE1_b.PRIORITY;
    p_cmd->data.p_prio_qos_ethtype->type2 = (p_switch_reg->PRIORITY_TYPE2_b.VALID) ? p_switch_reg->PRIORITY_TYPE2_b.TYPEVAL : 0;
    p_cmd->data.p_prio_qos_ethtype->prio2 = p_switch_reg->PRIORITY_TYPE2_b.PRIORITY;

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_out_queues_get
 * Description  : Returns the number of output queues per port.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_out_queues_get(R_ETHSW_Type volatile *p_switch_reg,
                                             ether_switch_cmd_t *   p_cmd)
{
    *(p_cmd->data.p_out_queues) = ETHER_SWITCH_QOS_OUTQUEUES;

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_disc_unk_set
 * Description  : Set the Discard Unknown Destination Bit in Control register
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_disc_unk_set(R_ETHSW_Type volatile *  p_switch_reg,
                                           ether_switch_cmd_t *     p_cmd)
{
    if (ETHER_SWITCH_TRUE == * p_cmd->data.p_flag_enable)
    {
        ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, LK_CTRL, LK_CTRL_b, DISC_UNK_DEST, ETHER_SWITCH_TRUE, ETHER_SWITCH_TYPE_ETHSW);
    }
    else
    {
        ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, LK_CTRL, LK_CTRL_b, DISC_UNK_DEST, ETHER_SWITCH_FALSE, ETHER_SWITCH_TYPE_ETHSW);
    }

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_disc_unk_get
 * Description  : Get the Discard Unknown Destination Bit in Control register
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_disc_unk_get(R_ETHSW_Type volatile *  p_switch_reg,
                                           ether_switch_cmd_t *     p_cmd)
{
    if (p_switch_reg->LK_CTRL_b.DISC_UNK_DEST)
    {
        * p_cmd->data.p_flag_enable = ETHER_SWITCH_TRUE;
    }
    else
    {
        * p_cmd->data.p_flag_enable = ETHER_SWITCH_FALSE;
    }

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_flood_unk_dest_set
 * Description  : Set the flood domain port masks for frames with unknown destinations
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_flood_unk_dest_set(R_ETHSW_Type volatile *p_switch_reg,
                                                 ether_switch_cmd_t *   p_cmd)
{
    ether_switch_flood_unk_conf_t * p_config;   /* config data */
    uint32_t reg;                               /* value */

    p_config = p_cmd->data.p_flood_unk_conf;

    /* this function only works with a portmask */
    if (!(p_config->port_mask_bcast & ETHER_SWITCH_PORT_MASK) ||
        !(p_config->port_mask_mcast & ETHER_SWITCH_PORT_MASK) ||
        !(p_config->port_mask_ucast & ETHER_SWITCH_PORT_MASK))
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

    reg = p_config->port_mask_bcast;
    if (reg & ETHER_SWITCH_PORT_HOST)
    {
        reg &= ~ETHER_SWITCH_PORT_HOST;
        reg |= ETHER_SWITCH_PORT_BIT(portHost);
    }
    reg &= ~ETHER_SWITCH_PORT_MASK;
    ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, BCAST_DEFAULT_MASK0, BCAST_DEFAULT_MASK0_b, BCASTDM, reg, ETHER_SWITCH_TYPE_ETHSW);

    reg = p_config->port_mask_mcast;
    if (reg & ETHER_SWITCH_PORT_HOST)
    {
        reg &= ~ETHER_SWITCH_PORT_HOST;
        reg |= ETHER_SWITCH_PORT_BIT(portHost);
    }
    reg &= ~ETHER_SWITCH_PORT_MASK;
    ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, MCAST_DEFAULT_MASK0, MCAST_DEFAULT_MASK0_b, MCASTDM, reg, ETHER_SWITCH_TYPE_ETHSW);

    reg = p_config->port_mask_ucast;
    if (reg & ETHER_SWITCH_PORT_HOST)
    {
        reg &= ~ETHER_SWITCH_PORT_HOST;
        reg |= ETHER_SWITCH_PORT_BIT(portHost);
    }
    reg &= ~ETHER_SWITCH_PORT_MASK;
    ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, UCAST_DEFAULT_MASK0, UCAST_DEFAULT_MASK0_b, UCASTDM, reg, ETHER_SWITCH_TYPE_ETHSW);

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_flood_unk_dest_get
 * Description  : Get the flood domain port masks for frames with unknown destinations
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_flood_unk_dest_get(R_ETHSW_Type volatile *p_switch_reg,
                                                 ether_switch_cmd_t *   p_cmd)
{
    ether_switch_flood_unk_conf_t * p_config;   /* config data */

    p_config = p_cmd->data.p_flood_unk_conf;

    p_config->port_mask_bcast = p_switch_reg->BCAST_DEFAULT_MASK0_b.BCASTDM;
    p_config->port_mask_mcast = p_switch_reg->MCAST_DEFAULT_MASK0_b.MCASTDM;
    p_config->port_mask_ucast = p_switch_reg->UCAST_DEFAULT_MASK0_b.UCASTDM;

    if (p_config->port_mask_bcast & (~ETHER_SWITCH_PORT_MASK & ETHER_SWITCH_PORT_BIT(portHost)))
    {
        p_config->port_mask_bcast &= ~ETHER_SWITCH_PORT_BIT(portHost);
        p_config->port_mask_bcast |= ETHER_SWITCH_PORT_HOST;
    }
    p_config->port_mask_bcast |= ETHER_SWITCH_PORT_MASK;

    if (p_config->port_mask_mcast & (~ETHER_SWITCH_PORT_MASK & ETHER_SWITCH_PORT_BIT(portHost)))
    {
        p_config->port_mask_mcast &= ~ETHER_SWITCH_PORT_BIT(portHost);
        p_config->port_mask_mcast |= ETHER_SWITCH_PORT_HOST;
    }
    p_config->port_mask_mcast |= ETHER_SWITCH_PORT_MASK;

    if (p_config->port_mask_ucast & (~ETHER_SWITCH_PORT_MASK & ETHER_SWITCH_PORT_BIT(portHost)))
    {
        p_config->port_mask_ucast &= ~ETHER_SWITCH_PORT_BIT(portHost);
        p_config->port_mask_ucast |= ETHER_SWITCH_PORT_HOST;
    }
    p_config->port_mask_ucast |= ETHER_SWITCH_PORT_MASK;

    return FSP_SUCCESS;
}
#endif /* ETHER_SWITCH_CONFIG_PRIORITY3_OTHER */

#ifdef ETHER_SWITCH_CONFIG_PRIORITY4
/***********************************************************************************************************************
 * Function Name: ether_switch_storm_time_set
 * Description  : Sets the timeframe for the broadcast/multicast rate limiting in ms.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_storm_time_set(R_ETHSW_Type volatile *p_switch_reg,
                                             ether_switch_cmd_t *   p_cmd)
{
    double val;                                 /* value */

    val = round((((double) * p_cmd->data.p_time_storm) * (ETHER_SWITCH_HZ / 1000)) / ETHER_SWITCH_DOS_TIMEFACTOR);
    ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, BCAST_STORM_LIMIT, BCAST_STORM_LIMIT_b, TMOUT, (uint16_t) val, ETHER_SWITCH_TYPE_ETHSW);

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_storm_time_get
 * Description  : Returns the timeframe for the broadcast/multicast rate limiting in ms.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_storm_time_get(R_ETHSW_Type volatile *p_switch_reg,
                                             ether_switch_cmd_t *   p_cmd)
{
    double val;                                 /* value */

    val = round((((double) p_switch_reg->BCAST_STORM_LIMIT_b.TMOUT) * ETHER_SWITCH_DOS_TIMEFACTOR) / (ETHER_SWITCH_HZ / 1000));
    *p_cmd->data.p_time_storm = (uint16_t) val;

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_storm_bcast_limit_set
 * Description  : Sets the max. allowed broadcast frames that can be received in a given time.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_bcast_limit_set(R_ETHSW_Type volatile *   p_switch_reg,
                                              ether_switch_cmd_t *      p_cmd)
{
    ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, BCAST_STORM_LIMIT, BCAST_STORM_LIMIT_b, BCASTLIMIT, *p_cmd->data.p_num_storm_frames, ETHER_SWITCH_TYPE_ETHSW);

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_storm_bcast_limit_get
 * Description  : Returns the max. allowed broadcast frames that can be received in a given time.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_bcast_limit_get(R_ETHSW_Type volatile *   p_switch_reg,
                                              ether_switch_cmd_t *      p_cmd)
{
    *p_cmd->data.p_num_storm_frames = p_switch_reg->BCAST_STORM_LIMIT_b.BCASTLIMIT;

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_mcast_limit_set
 * Description  : Sets the max. allowed multicast frames that can be received in a given time.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_mcast_limit_set(R_ETHSW_Type volatile *   p_switch_reg,
                                              ether_switch_cmd_t *      p_cmd)
{
    ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, MCAST_STORM_LIMIT, MCAST_STORM_LIMIT_b, MCASTLIMIT, *p_cmd->data.p_num_storm_frames, ETHER_SWITCH_TYPE_ETHSW);

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_mcast_limit_get
 * Description  : Returns the max. allowed multicast frames that can be received in a given time.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_mcast_limit_get(R_ETHSW_Type volatile *   p_switch_reg,
                                              ether_switch_cmd_t *      p_cmd)
{
    *p_cmd->data.p_num_storm_frames = p_switch_reg->MCAST_STORM_LIMIT_b.MCASTLIMIT;

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_tx_rate_set
 * Description  : Specifies the max. allowed bandwidth for the given port in percent.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_tx_rate_set(R_ETHSW_Type volatile *   p_switch_reg,
                                          ether_switch_cmd_t *      p_cmd)
{
    float idleslope;                            /* Idle Slope value */
    volatile uint32_t *p_reg;                    /* register */

    /* calculate Traffic Shaper bandwidth control register addresse */
    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->IDLE_SLOPE_P0 + ETHER_SWITCH_PORT_OFFSET * p_cmd->port);

    /* Valid value ? */
    if (ETHER_SWITCH_RATE_IS_MAX < *p_cmd->data.p_rate)
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

    /* Calc idle slope and set it */
    idleslope = (512 * (*p_cmd->data.p_rate / 100)) / (1 - (*p_cmd->data.p_rate / 100));

    /* check if calculated slope rate exceeds maximum */
    if (ETHER_SWITCH_RATE_IS_MASK < idleslope)
    {
        /* limit slope rate to maximum value */
        idleslope = ETHER_SWITCH_RATE_IS_MASK;
    }

    /* write slope rate to port specific register (IDLE_SLOPE_P[n]) */
    *p_reg = (uint16_t) idleslope;

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_tx_rate_get
 * Description  : Returns the max. allowed bandwidth for the given port in percent.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_tx_rate_get(R_ETHSW_Type volatile *   p_switch_reg,
                                          ether_switch_cmd_t *      p_cmd)
{
    volatile uint32_t *p_reg;                   /* register */

    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->IDLE_SLOPE_P0 + ETHER_SWITCH_PORT_OFFSET * p_cmd->port);
    *p_cmd->data.p_rate = (1 / (1 + (512 / (float) (*p_reg & ETHER_SWITCH_RATE_IS_MASK)))) * 100;

    return FSP_SUCCESS;
}
#endif  /* ETHER_SWITCH_CONFIG_PRIORITY4 */

#ifdef ETHER_SWITCH_CONFIG_PRIORITY5
/***********************************************************************************************************************
 * Function Name: ether_switch_eee_set
 * Description  : Enables/disables and configure the Energy Efficient Ethernet Mode of the switch.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_eee_set(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    volatile uint32_t *p_reg;                   /* Register */
    volatile uint32_t *pIdlereg;                /* Idle Register */

    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->EEE_CTL_STAT_P0 + ETHER_SWITCH_COUNTER_OFFSET * p_cmd->port);

    if (ETHER_SWITCH_TRUE == p_cmd->data.p_cnf_eee->state)
    {
        /* Disable EEE, set idle time and enable again */
        ETHER_SWITCH_REG_SET(*p_reg, *p_reg & ~((uint32_t) ETHER_SWITCH_EEE_AUTO));
        pIdlereg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->EEE_IDLE_TIME_P0 + ETHER_SWITCH_COUNTER_OFFSET * p_cmd->port);
        ETHER_SWITCH_REG_SET(*pIdlereg, p_cmd->data.p_cnf_eee->idletime);
        ETHER_SWITCH_REG_SET(*p_reg, *p_reg | ETHER_SWITCH_EEE_AUTO);
    }
    else
    {
        /* Disable EEE */
        *p_reg &= ~((uint32_t) ETHER_SWITCH_EEE_AUTO);
    }

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_eee_get
 * Description  : Returns the Energy Efficient Ethernet configuration of the switch.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_eee_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    volatile uint32_t *p_reg;                   /* Register */
    volatile uint32_t *pIdlereg;                /* Idle Register */

    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->EEE_CTL_STAT_P0 + ETHER_SWITCH_COUNTER_OFFSET * p_cmd->port);
    pIdlereg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->EEE_IDLE_TIME_P0 + ETHER_SWITCH_COUNTER_OFFSET * p_cmd->port);
    p_cmd->data.p_cnf_eee->state = (*p_reg & ETHER_SWITCH_EEE_AUTO) ? ETHER_SWITCH_TRUE : ETHER_SWITCH_FALSE;
    p_cmd->data.p_cnf_eee->idletime = *pIdlereg;

    return FSP_SUCCESS;
}

#ifdef ETHER_SWITCH_CONFIG_SNOOP
/***********************************************************************************************************************
 * Function Name: ether_switch_snoop_add
 * Description  : Add a snooping configuration
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 *              : FSP_ERR_OUT_OF_MEMORY
 *              :       - Cannot be allocate
 **********************************************************************************************************************/
static fsp_err_t ether_switch_snoop_add(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    fsp_err_t           res = FSP_SUCCESS;          /* Result */
    uint32_t            id = 0;                     /* Parser id */
    ether_switch_bool_t found = ETHER_SWITCH_FALSE; /* found flag */
    uint32_t            mode = 0;                   /* mode */
    uint16_t            offset = 0;                 /* offset */

    if (NULL == p_cmd->data.p_cnf_snoop)
    {
        return FSP_ERR_INVALID_POINTER;
    }

    /* Check whether a block has the requested mode */
    if (block1Mode == p_cmd->data.p_cnf_snoop->action)
    {
        res = ether_switch_snoop_free_gparser_get(p_switch_reg, ETHER_SWITCH_GPARSER_PARSER_BLOCK_ID1, &id);
        if (RES_OK(res))
        {
            found = ETHER_SWITCH_TRUE;
        }
    }

    if ((block2Mode == p_cmd->data.p_cnf_snoop->action) && (ETHER_SWITCH_FALSE == found))
    {
        res = ether_switch_snoop_free_gparser_get(p_switch_reg, ETHER_SWITCH_GPARSER_PARSER_BLOCK_ID2, &id);
    }

    if (RES_OK(res))
    {
        /* Set params based on mode */
        switch (p_cmd->data.p_cnf_snoop->mode)
        {
            /* IP data mode */
            case ETHER_SWITCH_SNOOP_MODE_IPDATA:
                mode |= ETHER_SWITCH_GPARSER_MODE_IPDATA;
                offset = (uint16_t) p_cmd->data.p_cnf_snoop->offset;
                break;

             /* IP proto mode */
            case ETHER_SWITCH_SNOOP_MODE_IPPROT:
                mode |= ETHER_SWITCH_GPARSER_MODE_IPPROTO;
                break;

            /* Ethertype mode */
            case ETHER_SWITCH_SNOOP_MODE_ETYPE:
                /* Ethertype consists of data mode, compare16 and skip vlan */
                mode |= ETHER_SWITCH_GPARSER_MODE_COMPARE16;
                mode |= ETHER_SWITCH_GPARSER_MODE_SKIPVLAN;
                break;

            /* Free data mode */
            case ETHER_SWITCH_SNOOP_MODE_DATA:
                mode |= p_cmd->data.p_cnf_snoop->flags;
                offset = (uint16_t) p_cmd->data.p_cnf_snoop->offset;
                break;
        }

        p_cmd->data.p_cnf_snoop->id = id;
        res = ether_switch_gparser_set(p_switch_reg, (uint8_t) id, ETHER_SWITCH_TRUE, mode, p_cmd->data.p_cnf_snoop->match, p_cmd->data.p_cnf_snoop->mask, offset);
    }
    return res;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_snoop_rem
 * Description  : Removes a snooping config from hardware
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_snoop_rem(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    return ether_switch_gparser_set(p_switch_reg, (uint8_t) *p_cmd->data.p_id_cnf_snoop, ETHER_SWITCH_FALSE, 0, 0, 0, 0);
}

/***********************************************************************************************************************
 * Function Name: ether_switch_snoop_free_gparser_get
 * Description  : Search for free parser in given block
 * Parameters   : p_switch_reg
 *              :       - switch register address
 *              : block
 *              :       - block number
 *              : p_id
 *              :       - ID pointer
 *              : -
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 *              : FSP_ERR_OUT_OF_MEMORY
 *              :       - Not found paeser
 **********************************************************************************************************************/
static fsp_err_t ether_switch_snoop_free_gparser_get(
                        R_ETHSW_Type volatile * p_switch_reg,   
                        uint32_t                block,          /* block number */
                        uint32_t *              p_id)           /* id */
{
    fsp_err_t           res = FSP_SUCCESS;          /* Result */
    ether_switch_bool_t found = ETHER_SWITCH_FALSE; /* True if free parser was found */
    volatile uint32_t * p_reg;                      /* Register */
    uint32_t            idx;                        /* Index */

    /* Check whether block id is valid */
    if (ETHER_SWITCH_GPARSER_BLOCK_CNT < block || 0 == block)
    {
        res = FSP_ERR_INVALID_ARGUMENT;
    }

    if (RES_OK(res))
    {
        /* Select start register depending on block */
        if (ETHER_SWITCH_GPARSER_PARSER_BLOCK_ID1 == block)
        {
            p_reg = (volatile uint32_t *) &(p_switch_reg->GPARSER0);
        }
        else
        {
            p_reg = (volatile uint32_t *) &(p_switch_reg->GPARSER4);
        }

        /* Look for free parser */
        for (idx = 0; idx < ETHER_SWITCH_GPARSER_PARSER_PER_BLOCK && !found; idx++)
        {
            if (!(*(p_reg) & ETHER_SWITCH_GPARSER_PARSER_VALID))
            {
                found = ETHER_SWITCH_TRUE;
                *p_id = idx;
                if (ETHER_SWITCH_GPARSER_PARSER_BLOCK_ID2 == block)
                {
                    *p_id += ETHER_SWITCH_GPARSER_PARSER_PER_BLOCK;
                }
            }
            p_reg = (volatile uint32_t *) ((uint8_t *) p_reg + ETHER_SWITCH_PORT_OFFSET_4BYTES);
        }
    }

    /* Set error if no free parser was found */
    if (ETHER_SWITCH_FALSE == found)
    {
        res = FSP_ERR_OUT_OF_MEMORY;
    }

    return res;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_gparser_set
 * Description  : Configures the generic parser with the given ID.
 * Parameters   : p_switch_reg
 *              :       - switch register address
 *              : parserid
 *              :       - the id of the parser to configure. Valid range is 0-7
 *              : enable
 *              :       - enable/disable the parser
 *              : mode
 *              :       - the mode of comparison
 *              : compval
 *              :       - the value to compare with
 *              : mask
 *              :       - the mask which used to AND with the compare value
 *              : offset
 *              :       - offset in bytes for the field to match
 *              : -
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_gparser_set(
                        R_ETHSW_Type volatile * p_switch_reg,   
                        uint8_t                 parserid,       /* The id of the parser to configure. Valid range is 0-7. */
                        ether_switch_bool_t     enable,         /* Enable/disable the parser */
                        uint32_t                mode,           /* The mode of comparison */
                        uint16_t                compval,        /* The value to compare with. */
                        uint8_t                 mask,           /* The mask which used to AND with the compare value. */
                        uint16_t                offset)         /* Offset in bytes for the field to match. */
{
    fsp_err_t           res = FSP_SUCCESS;      /* Result */
    volatile uint32_t * p_reg;                  /* Register */
    uint32_t            val = 0;                /* Parser config */

    /* Check parser id */
    if (ETHER_SWITCH_GPARSER_PARSER_MAX < parserid)
    {
        res = FSP_ERR_INVALID_ARGUMENT;
    }

    if (RES_OK(res)) 
    {
        /* Calc offset depending on block */
        if (parserid <= ETHER_SWITCH_GPARSER_GPARSER_BLOCK1)
        {
            p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->GPARSER0 + ETHER_SWITCH_PORT_OFFSET_4BYTES * parserid);
        }
        else
        {
            p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->GPARSER4 + ETHER_SWITCH_PORT_OFFSET_4BYTES * (parserid - 4));
        }
        if (enable)
        {
            /* Check whether modes are ok */
            if ((mode & ETHER_SWITCH_GPARSER_MODE_COMPARE16) && (mode & ETHER_SWITCH_GPARSER_MODE_CMP_OR)) 
            {
                res = FSP_ERR_INVALID_ARGUMENT;
            }
            else if ((mode & ETHER_SWITCH_GPARSER_MODE_IPPROTO) && (mode & ETHER_SWITCH_GPARSER_MODE_NONIPPROT))
            {
                res = FSP_ERR_INVALID_ARGUMENT;
            }
            else if ((mode & ETHER_SWITCH_GPARSER_MODE_IPDATA) && (mode & ETHER_SWITCH_GPARSER_MODE_SKIPVLAN))
            {
                res = FSP_ERR_INVALID_ARGUMENT;
            }
            else if (mode & ETHER_SWITCH_GPARSER_MODE_INVALID)
            {
                res = FSP_ERR_INVALID_ARGUMENT;
            }

            if (RES_OK(res)) 
            {
                /* Set compare values */
                if ((mode & ETHER_SWITCH_GPARSER_MODE_COMPARE16) || (mode & ETHER_SWITCH_GPARSER_MODE_OFFSETPLUS2)) 
                {
                    /* In COMPARE16 or OFFSETPLUS2 mode, no mask is given */
                    val |= (compval & 0xFFFF);
                }
                else
                {
                    val |= ((uint32_t) compval & 0xFF) << ETHER_SWITCH_GPARSER_CMP_OFFSET;
                    val |= mask;
                }
                /* Set offset and activate parser */
                val |= ((uint32_t) offset & ETHER_SWITCH_GPARSER_OFFSET_MASK) << ETHER_SWITCH_GPARSER_OFFSET_OFFSET;
                val |= ETHER_SWITCH_GPARSER_PARSER_VALID;
                val |= mode;
                ETHER_SWITCH_REG_SET(*p_reg, val);
            }
        }
        else
        {
            ETHER_SWITCH_REG_SET(*p_reg, *p_reg & ~((uint32_t) ETHER_SWITCH_GPARSER_PARSER_VALID));
        }
    }

    return res;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_snoop_init
 * Description  : Configures the action for matched frames.
 * Parameters   : mode1
 *              :       - Snoop mode for block 1
 *                mode2
 *              :       - Snoop mode for block 2
 *              : -
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
fsp_err_t ether_switch_snoop_init(
                        R_ETHSW_Type volatile * p_switch_reg,
                        ether_switch_snoop_action_t mode1,      /* Snoop mode for block 1 */
                        ether_switch_snoop_action_t mode2)      /* Snoop mode for block 2 */
{
    volatile uint32_t *p_reg;       /* arith reg */

    block1Mode = mode1;
    block2Mode = mode2;

    /* Set operation for all arith to OR */
    p_reg = (volatile uint32_t *) &(p_switch_reg->GARITH0);
    while (p_reg <= (volatile uint32_t *) &p_switch_reg->GARITH3) {
        ETHER_SWITCH_REG_SET(*p_reg, *p_reg | ETHER_SWITCH_GPARSER_PARSER_OPER_OR);
        p_reg = (volatile uint32_t *) ((uint8_t *) p_reg + ETHER_SWITCH_PORT_OFFSET_4BYTES);
    }

    p_reg = (volatile uint32_t *) &(p_switch_reg->GARITH4);
    while (p_reg <= (volatile uint32_t *) &p_switch_reg->GARITH7) {
        ETHER_SWITCH_REG_SET(*p_reg, *p_reg | ETHER_SWITCH_GPARSER_PARSER_OPER_OR);
        p_reg = (volatile uint32_t *) ((uint8_t *) p_reg + ETHER_SWITCH_PORT_OFFSET_4BYTES);
    }

    /* Take all results of the stage into account */
    ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, GARITH3, GARITH3_b, SEL_MATCH, ETHER_SWITCH_GPARSER_ARTIH_ALL, GOAL_SW_TYPE_ETHSW);
    ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, GARITH7, GARITH7_b, SEL_MATCH, ETHER_SWITCH_GPARSER_ARTIH_ALL, GOAL_SW_TYPE_ETHSW);

    /* Set snooping modes for both stages */
    ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, GARITH3, GARITH3_b, SNP_MD, mode1, ETHER_SWITCH_TYPE_ETHSW);
    ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, GARITH7, GARITH7_b, SNP_MD, mode2, ETHER_SWITCH_TYPE_ETHSW);

    return FSP_SUCCESS;
}
#endif /* ETHER_SWITCH_CONFIG_SNOOP */

#ifdef ETHER_SWITCH_CONFIG_BRIDGE
/***********************************************************************************************************************
 * Function Name: ether_switch_auth
 * Description  : Set or get authorization state for given port
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_auth(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    volatile uint32_t *p_reg;                   /* Register */

    /* reject host port */
    if (ETHER_SWITCH_PORT_HOST == p_cmd->port)
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

    /* get pointer to authentication register */
    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->AUTH_PORT[0] + ETHER_SWITCH_PORT_OFFSET_4BYTES * p_cmd->port);

    /* set authentication */
    if (p_cmd->flg_set)
    {
        if (ETHER_SWITCH_AUTH_AUTHORIZED == *p_cmd->data.p_state_auth)
        {
            ETHER_SWITCH_REG_SET(*p_reg, *p_reg | ETHWE_SWITCH_PORT_AUTHORIZED);
        }
        else if (ETHER_SWITCH_AUTH_UNAUTHORIZED == *p_cmd->data.p_state_auth)
        {
            ETHER_SWITCH_REG_SET(*p_reg, *p_reg & ~((uint32_t) ETHWE_SWITCH_PORT_AUTHORIZED));
        }
        else
        {
            return FSP_ERR_INVALID_ARGUMENT;
        }
    }
    else
    {
        *p_cmd->data.p_state_auth = (*p_reg & ETHWE_SWITCH_PORT_AUTHORIZED) ? ETHER_SWITCH_AUTH_AUTHORIZED : ETHER_SWITCH_AUTH_UNAUTHORIZED;
    }

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_controlled_direction
 * Description  : Set or get controlled direction for given port
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_controlled_direction(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    volatile uint32_t *p_reg;                   /* Register */

    /* reject host port */
    if (ETHER_SWITCH_PORT_HOST == p_cmd->port)
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

    /* get pointer to authentication register */
    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->AUTH_PORT[0] + ETHER_SWITCH_PORT_OFFSET_4BYTES * p_cmd->port);

    /* set controlled direction */
    if (p_cmd->flg_set)
    {
        /* check given direction value */
        if (ETHER_SWITCH_CTRL_DIR_BOTH == *p_cmd->data.p_state_dir_port_ctrl)
        {
            /* set port to controll input and output */
            ETHER_SWITCH_REG_SET(*p_reg, *p_reg | ETHWE_SWITCH_PORT_CONTROLLED_BOTH);

        }
        else if (ETHER_SWITCH_CTRL_DIR_INPUT == *p_cmd->data.p_state_dir_port_ctrl) 
        {
            /* set port to control only input */
            ETHER_SWITCH_REG_SET(*p_reg, *p_reg & ~((uint32_t) ETHWE_SWITCH_PORT_CONTROLLED_BOTH));

        }
        else
        {
            return FSP_ERR_INVALID_ARGUMENT;
        }
    }
    else
    {
        /* get controlled direction from register */
        *p_cmd->data.p_state_dir_port_ctrl = (*p_reg & ETHWE_SWITCH_PORT_CONTROLLED_BOTH) ? ETHER_SWITCH_CTRL_DIR_BOTH : ETHER_SWITCH_CTRL_DIR_INPUT;
    }

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_eapol_reception
 * Description  : Set or get EAPOL reception mode for given port
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_eapol_reception(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    volatile uint32_t *p_reg;                   /* Register */

    /* reject host port */
    if (ETHER_SWITCH_PORT_HOST == p_cmd->port)
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

    /* get pointer to authentication register */
    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->AUTH_PORT[0] + ETHER_SWITCH_PORT_OFFSET_4BYTES * p_cmd->port);

    /* set EAPOL reception mode */
    if (p_cmd->flg_set)
    {
        /* check given reception mode value */
        if (ETHER_SWITCH_EAPOL_ENABLE == *p_cmd->data.p_mode_eapol_recv)
        {
            /* enable reception of EAPOL frames */
            ETHER_SWITCH_REG_SET(*p_reg, *p_reg | ETHWE_SWITCH_PORT_EAPOL_ENABLE);

        }
        else if (ETHER_SWITCH_EAPOL_DISABLE == *p_cmd->data.p_mode_eapol_recv)
        {
            /* disable reception of EAPOL frames */
            ETHER_SWITCH_REG_SET(*p_reg, *p_reg & ~((uint32_t) ETHWE_SWITCH_PORT_EAPOL_ENABLE));

        }
        else
        {
            return FSP_ERR_INVALID_ARGUMENT;
        }
    }
    else
    {
        /* get EAPOL reception mode from register */
        *p_cmd->data.p_mode_eapol_recv = (*p_reg & ETHWE_SWITCH_PORT_EAPOL_ENABLE) ? ETHER_SWITCH_EAPOL_ENABLE : ETHER_SWITCH_EAPOL_DISABLE;
    }
    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_bpdu_forwarding
 * Description  : Set or get Bridge Protocol Frame (BPDU) forwarding mode
 *                Enabling the BPDU forwarding mode this function will also disable the
 *                unchecked dropping of BPDU frames.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_bpdu_forwarding(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    volatile uint32_t *p_reg;                   /* Register */

    /* get pointer to Management configuration register (0x44050020) */
    p_reg = (volatile uint32_t *) &(p_switch_reg->MGMT_CONFIG);

    /* set BPDU forwarding mode */
    if (p_cmd->flg_set)
    {
        /* check given forwarding mode value */
        if (ETHER_SWITCH_BPDU_CTRL_MGMT_FWD == *p_cmd->data.p_state_bpdu_ctrl)
        {

            /* Enable BPDU forwarding */
            ETHER_SWITCH_REG_SET(*p_reg, *p_reg | ETHER_SWITCH_MGMT_BPDU_FWD_ENABLE);

            /* Disable discarding BPDU frames */
            ETHER_SWITCH_REG_SET(*p_reg, *p_reg & ~((uint32_t) ETHER_SWITCH_MGMT_BPDU_DISCARD));

        }
        else if (ETHER_SWITCH_BPDU_CTRL_DISCARD == *p_cmd->data.p_state_bpdu_ctrl)
        {

            /* Disable BPDU forwarding */
            ETHER_SWITCH_REG_SET(*p_reg, *p_reg & ~((uint32_t) ETHER_SWITCH_MGMT_BPDU_FWD_ENABLE));

            /* Enable discarding BPDU frames */
            ETHER_SWITCH_REG_SET(*p_reg, *p_reg | ETHER_SWITCH_MGMT_BPDU_DISCARD);

        }
        else
        {
            return FSP_ERR_INVALID_ARGUMENT;
        }

    }
    else
    {
        /* Check if discarding and forwarding are enabled at the same time */
        if (((*p_reg & ETHER_SWITCH_MGMT_BPDU_FWD_ENABLE) && (*p_reg & ETHER_SWITCH_MGMT_BPDU_DISCARD))
            || (!(*p_reg & ETHER_SWITCH_MGMT_BPDU_FWD_ENABLE) && !(*p_reg & ETHER_SWITCH_MGMT_BPDU_DISCARD)))
        {
            *p_cmd->data.p_state_bpdu_ctrl = ETHER_SWITCH_BPDU_CTRL_UNKNOWN;
        }
        else
        {
            /* get BPDU forwarding mode from register */
            *p_cmd->data.p_state_bpdu_ctrl = (*p_reg & ETHER_SWITCH_MGMT_BPDU_FWD_ENABLE) ? ETHER_SWITCH_BPDU_CTRL_MGMT_FWD : ETHER_SWITCH_BPDU_CTRL_DISCARD;
        }

    }
    return FSP_SUCCESS;
}
#endif /* ETHER_SWITCH_CONFIG_BRIDGE */

#ifdef ETHER_SWITCH_CONFIG_DLR
/***********************************************************************************************************************
 * Function Name: ether_switch_dlr_init
 * Description  : Initialize DLR module
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 *              : FSP_ERR_TIMEOUT
 *              :       - Timeout error
 **********************************************************************************************************************/
static fsp_err_t ether_switch_dlr_init(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    ether_switch_dlr_init_t *   p_dlr_init;
    ether_switch_mactab_entry_t mac_entry;              /* MAC table entry */
    ether_switch_cmd_t          mac_cmd;                /* MAC table set command */
    fsp_err_t   res;                                    /* result */
    uint8_t     cnt;                                    /* loop counter */
    uint8_t     dlr_mac[ETHER_SWITCH_MAC_ADDR_LEN] =    /* Multicast MAC for Beacon Frames */
                            {0x01, 0x21, 0x6c, 0x00, 0x00, 0x01}; 

    /* initialize command data */
    mac_cmd.data.p_mac_tab = &mac_entry;

    /* Forced Forwarding via Static MAC Table entries */
    mac_entry.addr = (ether_switch_mac_addr_t *) &dlr_mac;

    /* --> Beacon Frames (01:21:6c:00:00:01) ports 0, 1 */
    dlr_mac[5] = 0x01;
    mac_entry.portmask = (1 << ETHER_SWITCH_PORT(0)) | (1 << ETHER_SWITCH_PORT(1)) |
                        ETHER_SWITCH_PORT_MASK;
    res = ether_switch_mac_tab_set(p_switch_reg, &mac_cmd);
    if (RES_ERR(res))
    {
        return res;
    }
    /* --> Neighbor_Check/SignOn Frames (01:21:6c:00:00:02) port 3 */
    dlr_mac[5] = 0x02;
    mac_entry.portmask = ETHER_SWITCH_PORT_HOST | ETHER_SWITCH_PORT_MASK;

    res = ether_switch_mac_tab_set(p_switch_reg, &mac_cmd);
    if (RES_ERR(res))
    {
        return res;
    }
    /* --> other Frames (01:21:6c:00:00:{03..05}) ports 0, 1, 3 */
    mac_entry.portmask = (1 << ETHER_SWITCH_PORT(0)) | (1 << ETHER_SWITCH_PORT(1)) |
                        ETHER_SWITCH_PORT_HOST | ETHER_SWITCH_PORT_MASK;
    for (cnt = 3; cnt < 6; cnt++)
    {
        dlr_mac[5] = cnt;
        res = ether_switch_mac_tab_set(p_switch_reg, &mac_cmd);
        if (RES_ERR(res))
        {
            return res;
        }
    }

    /* write local MAC for Loop Filter */
    p_dlr_init = p_cmd->data.p_dlr_init;

    ETHER_SWITCH_REG_SET(p_switch_reg->DLR_LOC_MAClo,
                    (unsigned int) ( (((uint8_t*)p_dlr_init->p_host_addr)[0] <<  0)
                                   | (((uint8_t*)p_dlr_init->p_host_addr)[1] <<  8)
                                   | (((uint8_t*)p_dlr_init->p_host_addr)[2] << 16) 
                                   | (((uint8_t*)p_dlr_init->p_host_addr)[3] << 24)));
    ETHER_SWITCH_REG_SET(p_switch_reg->DLR_LOC_MAChi,
                    (unsigned int) ( (((uint8_t*)p_dlr_init->p_host_addr)[4] << 0)
                                   | (((uint8_t*)p_dlr_init->p_host_addr)[5] << 8)));

    /* callback function pointer  */
    p_dlr_callback = p_dlr_init->p_dlr_callback;

    /* set DLR EtherType */
    ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, DLR_ETH_TYP, DLR_ETH_TYP_b, DLR_ETH_TYP, SW_RN_ADVSW_DLR_ETHTYPE, ETHER_SWITCH_TYPE_ETHSW);

    /* set number of cycles for 1 us */
    /* 200 MHz -> 200 Ticks = 1 us */
    ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, DLR_CONTROL, DLR_CONTROL_b, US_TIME, SW_RN_ADVSW_DLR_TICKS, ETHER_SWITCH_TYPE_ETHSW);

    /* discard Bacon frames with invalid Timeout values  */
    ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, DLR_CONTROL, DLR_CONTROL_b, IGNORE_INVTM, 1, ETHER_SWITCH_TYPE_ETHSW);

    /* interrupts */
    ETHER_SWITCH_REG_SET(p_switch_reg->DLR_IRQ_STAT_ACK, 0x0000FFFF);
    ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, INT_CONFIG, INT_CONFIG_b, IRQ_EN,  1, ETHER_SWITCH_TYPE_ETHSW);
    ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, INT_CONFIG, INT_CONFIG_b, DLR_INT, 1, ETHER_SWITCH_TYPE_ETHSW);
    ETHER_SWITCH_REG_SET(p_switch_reg->DLR_IRQ_CONTROL,
                        ETHER_SWITCH_TGT_DLR_INT_BEACONTMO_0 |
                        ETHER_SWITCH_TGT_DLR_INT_BEACONTMO_1 |
                        ETHER_SWITCH_TGT_DLR_INT_SUVICHNG    |
                        ETHER_SWITCH_TGT_DLR_INT_LNKCHNG_0   |
                        ETHER_SWITCH_TGT_DLR_INT_LNKCHNG_1   |
                        ETHER_SWITCH_TGT_DLR_INT_NEWSTATUS   |
                        ETHER_SWITCH_TGT_DLR_INT_LOCALLOOP_0 |
                        ETHER_SWITCH_TGT_DLR_INT_LOCALLOOP_1);

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_dlr_tgl
 * Description  : Toggle DLR module
 *                This function ether activates or deactivates the DLR module.
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_dlr_tgl(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    if (ETHER_SWITCH_CMD_DLR_ENABLE == p_cmd->cmd)
    {
        ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, DLR_CONTROL, DLR_CONTROL_b, ENABLE, 1, ETHER_SWITCH_TYPE_ETHSW);
    }
    else
    {
        ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, DLR_CONTROL, DLR_CONTROL_b, ENABLE, 0, ETHER_SWITCH_TYPE_ETHSW);
        ETHER_SWITCH_REG_SET_FIELD(p_switch_reg, INT_CONFIG, INT_CONFIG_b, DLR_INT, 0, ETHER_SWITCH_TYPE_ETHSW);
        ETHER_SWITCH_REG_SET(p_switch_reg->DLR_IRQ_CONTROL, 0x00000000);
        ETHER_SWITCH_REG_SET(p_switch_reg->DLR_IRQ_STAT_ACK, 0x0000FFFF);
    }

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_dlr_status
 * Description  : Get DLR Status values
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_UNSUPPORTED
 *              :       - Selected value not supported by this API
 **********************************************************************************************************************/
static fsp_err_t ether_switch_dlr_status(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    fsp_err_t res = FSP_SUCCESS;                /* result */

    switch (p_cmd->cmd)
    {
        case ETHER_SWITCH_CMD_DLR_LAST_BEACON:
            if (((ETHER_SWITCH_PORT(0) == p_cmd->port) && (0 != (p_switch_reg->DLR_STATUS_b.LastBcnRcvPort & 0x01))) ||
                ((ETHER_SWITCH_PORT(1) == p_cmd->port) && (0 != (p_switch_reg->DLR_STATUS_b.LastBcnRcvPort & 0x02)))) {
                *p_cmd->data.p_state_dlr = 1;
            }
            else {
                *p_cmd->data.p_state_dlr = 0;
            }
            break;

        case ETHER_SWITCH_CMD_DLR_NODE_STATE:
        case ETHER_SWITCH_CMD_DLR_RING_STATE:
            /* ring state indirectly coupled to node state */
            *p_cmd->data.p_state_dlr = p_switch_reg->DLR_STATUS_b.NODE_STATE;
            break;

        case ETHER_SWITCH_CMD_DLR_SV_IP:
            *p_cmd->data.p_state_dlr = p_switch_reg->DLR_SUPR_IPADR;
            break;

        case ETHER_SWITCH_CMD_DLR_SV_PREC:
            *p_cmd->data.p_state_dlr = p_switch_reg->DLR_SUPR_MAChi_b.PRECE;
            break;

        case ETHER_SWITCH_CMD_DLR_VLAN:
            *p_cmd->data.p_state_dlr = p_switch_reg->DLR_STATE_VLAN_b.VLANTAG;
            break;

        default:
            res = FSP_ERR_UNSUPPORTED;
            break;
    }

    return res;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_dlr_supervisor_mac
 * Description  : Get DLR Supervisor MAC address
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_dlr_supervisor_mac(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    uint32_t regVal;                            /* register value */

    regVal = p_switch_reg->DLR_SUPR_MAClo;
    (*p_cmd->data.pp_addr_mac)[0] = (uint8_t) (regVal & 0x000000ff);
    (*p_cmd->data.pp_addr_mac)[1] = (uint8_t) ((regVal & 0x0000ff00) >> 8);
    (*p_cmd->data.pp_addr_mac)[2] = (uint8_t) ((regVal & 0x00ff0000) >> 16);
    (*p_cmd->data.pp_addr_mac)[3] = (uint8_t) ((regVal & 0xff000000) >> 24);

    regVal = p_switch_reg->DLR_SUPR_MAChi;
    (*p_cmd->data.pp_addr_mac)[4] = (uint8_t) (regVal & 0x000000ff);
    (*p_cmd->data.pp_addr_mac)[5] = (uint8_t) ((regVal & 0x0000ff00) >> 8);

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_dlr_forward_enable
 * Description  : Initialize DLR compatibility functionality
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 *              : FSP_ERR_TIMEOUT
 *              :       - Timeout error
 **********************************************************************************************************************/
static fsp_err_t ether_switch_dlr_forward_enable(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    uint8_t dlr_mac[ETHER_SWITCH_MAC_ADDR_LEN] = {0x01, 0x21, 0x6c, 0x00, 0x00, 0x00}; /* Multicast MAC for DLR */
    ether_switch_mactab_entry_t mac_entry;      /* Mac table entry */
    fsp_err_t res;                              /* result */

    uint32_t cnt;                               /* count */

    /* initialize command data */
    p_cmd->data.p_mac_tab = &mac_entry;

    /* initialize MAC Table entries */
    mac_entry.addr = (ether_switch_mac_addr_t *) &dlr_mac;

    for (cnt = 1; cnt < 6; cnt++)
    {
        /* set DLR Multicast MAC address */
        dlr_mac[5] = cnt;

        /* delete address entry from MAC Table */
        mac_entry.portmask = ETHER_SWITCH_PORT_MASK;
        res = ether_switch_mac_tab_set(p_switch_reg, p_cmd);
        if (RES_ERR(res))
        {
            return res;
        }
    }

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_dlr_callback
 * Description  : Call the callback function registered for the DLR.
 * Parameters   : event
 *              :       - event ID
 *              : port
 *              :       - source port of DLR event
 *              : -
 * Return Value : none
 **********************************************************************************************************************/
void ether_switch_dlr_callback(
    ether_switch_dlr_event_t    event,
    uint32_t                    port)
{
    
    if( 0 != p_dlr_callback )
    {
        (* p_dlr_callback)(event, port);
    }
}

/***********************************************************************************************************************
 * Function Name: ether_switch_isr_dlr
 * Description  : Interrupt handler for DLR
 *                This function is called by ether_switch_isr_intr
 * Arguments    : p_instance_ctrl -
 *                    pointer to control structure
 *                -
 * Return Value : none
 ***********************************************************************************************************************/
void ether_switch_isr_dlr (ether_switch_instance_ctrl_t * p_instance_ctrl)
{
    R_ETHSW_Type *      p_switch_reg;
    ether_switch_cmd_t  cmd;
    uint32_t            link_state = 0;     /* link state */
    uint32_t            int_ID;             /* interrupt Flag */
    uint32_t            reg_val;            /* register content */
    int32_t             cnt;                /* loop counter */

    p_switch_reg = (R_ETHSW_Type *)p_instance_ctrl->p_reg_switch;

    while (p_switch_reg->DLR_IRQ_STAT_ACK)
    {
        reg_val = p_switch_reg->DLR_IRQ_STAT_ACK;
        ETHER_SWITCH_REG_SET(p_switch_reg->DLR_IRQ_STAT_ACK, reg_val);

        /* scan all status bits */
        for (cnt = 0; cnt < 16; cnt++)
        {

            int_ID = reg_val & (1 << cnt);

            if (int_ID == 0)
            {
                /* bit wasn't set, check next one */
                continue;
            }

            switch (int_ID)
            {
                case ETHER_SWITCH_TGT_DLR_INT_NEWSTATUS:
                    ether_switch_dlr_callback(DLR_EVENT_NEWSTATE, ETHER_SWITCH_PORT_HOST);
                    break;

                case ETHER_SWITCH_TGT_DLR_INT_BEACONTMO_0:
                    ether_switch_dlr_callback(DLR_EVENT_BEACONTIMEOUT, ETHER_SWITCH_PORT(0));
                    break;

                case ETHER_SWITCH_TGT_DLR_INT_BEACONTMO_1:
                    ether_switch_dlr_callback(DLR_EVENT_BEACONTIMEOUT, ETHER_SWITCH_PORT(1));
                    break;

                case ETHER_SWITCH_TGT_DLR_INT_SUVICHNG:
                    ether_switch_dlr_callback(DLR_EVENT_NEWSUPERVISOR, ETHER_SWITCH_PORT(0));
                    break;

                case ETHER_SWITCH_TGT_DLR_INT_LNKCHNG_0:
                    cmd.port = ETHER_SWITCH_PORT(0);
                    cmd.data.p_state_link = &link_state;
                    ether_switch_link_get(p_switch_reg, &cmd);

                    if (ETHER_SWITCH_STATE_UP == link_state)
                    {
                        ether_switch_dlr_callback(DLR_EVENT_LINKRESTORED, ETHER_SWITCH_PORT(0));
                    }
                    else
                    {
                        ether_switch_dlr_callback(DLR_EVENT_LINKLOST, ETHER_SWITCH_PORT(0));
                    }
                    break;

                case ETHER_SWITCH_TGT_DLR_INT_LNKCHNG_1:
                    cmd.port = ETHER_SWITCH_PORT(1);
                    cmd.data.p_state_link = &link_state;
                    ether_switch_link_get(p_switch_reg, &cmd);

                    if (ETHER_SWITCH_STATE_UP == link_state)
                    {
                        ether_switch_dlr_callback(DLR_EVENT_LINKRESTORED, ETHER_SWITCH_PORT(1));
                    }
                    else
                    {
                        ether_switch_dlr_callback(DLR_EVENT_LINKLOST, ETHER_SWITCH_PORT(1));
                    }
                    break;

                case ETHER_SWITCH_TGT_DLR_INT_LOCALLOOP_0:
                    ether_switch_dlr_callback(DLR_EVENT_OWNFRAME, ETHER_SWITCH_PORT(0));
                    break;

                case ETHER_SWITCH_TGT_DLR_INT_LOCALLOOP_1:
                    ether_switch_dlr_callback(DLR_EVENT_OWNFRAME, ETHER_SWITCH_PORT(1));
                    break;

                default:
                    break;
            }
        }
    }
}
#endif /* ETHER_SWITCH_CONFIG_DLR */

/***********************************************************************************************************************
 * Function Name: ether_switch_frame_size_set
 * Description  : Set maximum frame size of given port
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_frame_size_set(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    volatile uint32_t *p_reg;                   /* Register */
    uint32_t port;                              /* port */

    /* calculate per port address of the frame size register */
    port = (p_cmd->port & ETHER_SWITCH_PORT_HOST) ? portHost : p_cmd->port;
    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->FRM_LENGTH_P0 + ETHER_SWITCH_PORT_OFFSET * port);

    /* assign masked command data to register */
    ETHER_SWITCH_REG_SET(*p_reg, ((*p_cmd->data.p_frame_size_max) & ETHER_SWITCH_FRAMESIZE_MASK));

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: ether_switch_frame_size_get
 * Description  : Get maximum frame size of given port
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd
 *              :       - command pointer "ether_switch_timestamp_t" format
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ether_switch_frame_size_get(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    volatile uint32_t *p_reg;                   /* Register */
    uint32_t port;                              /* port */

    /* calculate per port address of the frame size register */
    port = (p_cmd->port & ETHER_SWITCH_PORT_HOST) ? portHost : p_cmd->port;
    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->FRM_LENGTH_P0 + ETHER_SWITCH_PORT_OFFSET * port);

    /* assign masked register value to command data */
    *p_cmd->data.p_frame_size_max = ((*p_reg) & ETHER_SWITCH_FRAMESIZE_MASK);

    return FSP_SUCCESS;
}

#endif  /* ETHER_SWITCH_CONFIG_PRIORITY5 */

#ifdef WS2
/***********************************************************************************************************************
 * Function Name: ether_switch_cqf_set  called by ETHER_SWITCH_CQF_SET
 * Description  : Configures Cyclic Queuing  
 * Parameters   : p_switch_reg
 *              :       - base address of ether switch register
 *              : p_cmd->port
 *              :       - Port mask to select ingress ports to join the CQF 
 *              : p_cmd->data.cqf_prio_enable_mask
 *              :       - pointer to uint8_t with mask for priority enable. Each bit enables a queue
 *              : p_cmd->data.cqf_sel
 *              :       - pointer to uint8_t with number for CQF group, 0 and 1 is supported 
 *              : p_cmd->data.cqf_queue_sel
 *              :       - pointer to uint8_t with number for queue to be used for CQF group 
 *              : p_cmd->data.cqf_gate_sel
 *              :       - pointer to uint8_t with number (not mask) for gate signal (from TDMA) to be used for CQF group 
 *              : p_cmd->data.cqf_use_sop
 *              :       - pointer to uint8_t with 1 if queue is decided at SOP, otherwise at EOP
 *              : p_cmd->data.cqf_ref_sel
 *              :       - pointer to uint8_t with 1 if gate control signal is used at ingress port, otherise at egress port
 *              : - 
 * Return Value : FSP_SUCCESS
 *              :       - Command successfully.
 *              : FSP_ERR_INVALID_ARGUMENT
 *              :       - Invalid input parameter.
 *              : FSP_ERR_TIMEOUT
 *              :       - Timeout error
 **********************************************************************************************************************/
#if 0
fsp_err_t ether_switch_cqf_set(
                        R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
    ether_switch_mmclt_cqf_t *    p_mmctl_cqf = p_cmd->data.p_mmctl_cqf;
    uint8_t portnum;
    uint32_t config = p_mmctl_cqf->ref_sel << 15 | p_mmctl_cqf->use_sop << 14 | p_mmctl_cqf->gate_sel << 11 | p_mmctl_cqf->queue_sel << 8 | p_mmctl_cqf->prio_enable_mask;
      if (p_mmctl_cqf->cqf_sel == 1) {
        config = config << 16;
      }

      for (portnum=0; portnum<(ETHER_CFG_MAX_PORTS+1); portnum++) {
       if (p_cmd->port & (0x01<< portnum)) { 
        
        switch (portnum)
        {
          case ETHER_SWITCH_PORT_0:
            if (p_mmctl_cqf->cqf_sel == 0) {
              p_switch_reg->MMCTL_CQF_CTRL_P0 &= 0xffff0000; 
            }
            else {
              p_switch_reg->MMCTL_CQF_CTRL_P0 &= 0x0000ffff; 
            }  
            p_switch_reg->MMCTL_CQF_CTRL_P0 |= config;
            break;

          case ETHER_SWITCH_PORT_1:
            if (p_mmctl_cqf->cqf_sel == 0) {
              p_switch_reg->MMCTL_CQF_CTRL_P1 &= 0xffff0000; 
            }
            else {
              p_switch_reg->MMCTL_CQF_CTRL_P1 &= 0x0000ffff; 
            }  
            p_switch_reg->MMCTL_CQF_CTRL_P1 |= config;
            break;

          case ETHER_SWITCH_PORT_2:
           if (p_mmctl_cqf->cqf_sel == 0) {
              p_switch_reg->MMCTL_CQF_CTRL_P2 &= 0xffff0000; 
            }
            else {
              p_switch_reg->MMCTL_CQF_CTRL_P2 &= 0x0000ffff; 
            }  
            p_switch_reg->MMCTL_CQF_CTRL_P2 |= config;
            break;

           case ETHER_SWITCH_PORT_3:
           if (p_mmctl_cqf->cqf_sel == 0) {
              p_switch_reg->MMCTL_CQF_CTRL_P3 &= 0xffff0000; 
            }
            else {
              p_switch_reg->MMCTL_CQF_CTRL_P3 &= 0x0000ffff; 
            }  
            p_switch_reg->MMCTL_CQF_CTRL_P3 |= config;
            break;

          default:
            return FSP_ERR_INVALID_ARGUMENT;
            break;
       }  // switch
      } // if
    } // for
    return FSP_SUCCESS;
}
#endif


/* This function sets up a basic scheduleing scheme 
port:   Port mask for which the time aware transmission should be done, , only affects buffering within the port, has no impact on gemeral use of TDMA
enable: 1 to enable, 0 to disable (not implemented)
timer_sel: Number of the timer to use
base_time: Start time in ns when scheme should start, 
cycle_time: in ns. 
num_entries: Amount of scheduling entries, maximum 8
*entries: Pointer to struct array of scheduling entries
Nte that the Priority scheme must be enabled first, the default is not sufficient, it ust be explicitely enabled.*/
void ethsw_tdma_qopt_set ( R_ETHSW_Type volatile *  p_switch_reg,
                        ether_switch_cmd_t *     p_cmd)
{
  
        ether_switch_tdma_enable_t *    p_tdma_enable = p_cmd->data.p_tdma_enable;
        uint8_t portmask = p_tdma_enable->portmask;
        __attribute__((unused))uint8_t enable = p_tdma_enable->flg_enable;
        uint8_t timer_sel = p_tdma_enable->tim_num; 
        uint32_t base_time = p_tdma_enable->tdma_start; 
        uint32_t cycle_time = p_tdma_enable->tdma_cycle; 
        uint8_t num_entries = p_tdma_enable->num_entries; 
        ether_switch_tdma_sched_entries_t * entries = (ether_switch_tdma_sched_entries_t *) p_cmd->data.p_tdma_enable->p_tdma_sched_entries;  // Pointer to first of num_entries structs containing timing data
        ether_switch_tdma_sched_entries_t *entry; 
        int i;
        uint32_t start = 1;
        R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)p_switch_reg;

//-OS- keep all timer running, both are used
//        if (timer_sel == 0) {
//          p_reg_ethsw->ATIME_CTRL0 = 0x00;      // Disable timer 0        
//        }
//        else {
//          p_reg_ethsw->ATIME_CTRL1 = 0x00;      // Disable timer  1      
//        }          
        p_reg_ethsw->TDMA_START = base_time;   // Starttime for cycling
        p_reg_ethsw->TDMA_ENA_CTRL = portmask & 0x0f;   // Enable TDMA mode for this portmap, only affects buffering within the port, has no impact on gemeral use of TDMA
        p_reg_ethsw->TDMA_CYCLE = cycle_time;   // 
        p_reg_ethsw->TDMA_MODULO = 1000000000;   //  Wrap around at 1 sec  / 1000000000ns 
        entry = entries;
        for (i=0; i<num_entries; i++) {
#ifdef ETHSW_FPGA1           
           if (i>0) start = 0;
           p_reg_ethsw->TCV_SEQ = (i<<16) | (i<<2) | start ;    // Write address in sequencer, no auto increment, using same address for data and address, no paging
#else
           p_reg_ethsw->TCV_SEQ_ADDR = i;    // Write address in sequencer, no auto increment
           if (i>0) start = 0;
           p_reg_ethsw->TCV_SEQ_CTRL = ((entry->gpio_mask)<<22) | (i<<2) | start;   // TCV_DATA_INDEX | Startbit   
#endif
           p_reg_ethsw->TCV_D_ADDR = i;      // Write address in TCV_Data table,
           p_reg_ethsw->TCV_D_OFFSET = entry->offset; 
           p_reg_ethsw->TCV_D_CTRL = (entry->port_mask << 16) | (entry->gate_mask << 8) | (entry->hold_req << 7) | (entry->gate_mode << 6) | (entry->trigger_mode << 5) | 
                                      (entry->in_ct_enable << 4) | (entry->out_ct_enable << 3) | (entry->red_phase << 2) | (entry->inc_1 << 1) | (entry->inc_0 << 0) ;  // Gate_mode | Enable Gateing for the ports in portmask for the gates in gatemask and enable 
           
           entry = ++entries;
        }
        p_reg_ethsw->TCV_SEQ_LAST = num_entries - 1; 
        p_reg_ethsw->TDMA_TCV_START = 0; 
        p_reg_ethsw->TDMA_CONFIG = ((timer_sel & 0x03)<<2);   // Set the timer
        p_reg_ethsw->TDMA_GPIO = p_tdma_enable->gpio_mode << 16;
        
        if (timer_sel == 0) {
          p_reg_ethsw->ATIME_SEC0 = 0x0;      // Reset timer   (Hardware Rest timer anly affects ns part   
          p_reg_ethsw->ATIME0 = 0x0;      // Restet timer
          p_reg_ethsw->ATIME_CTRL0 = 0x01;      // Enable timer,    
        }
        else if (timer_sel == 1) {
          p_reg_ethsw->ATIME_SEC1 = 0x0;      // Reset timer     
          p_reg_ethsw->ATIME1 = 0x0;      // Restet timer
          p_reg_ethsw->ATIME_CTRL1 = 0x01;      // Enable timer,    
        }
        p_reg_ethsw->TDMA_CONFIG |= 0x01 ;   // Enable and Now start the whole thing
        return;

}

#endif   // WS2
