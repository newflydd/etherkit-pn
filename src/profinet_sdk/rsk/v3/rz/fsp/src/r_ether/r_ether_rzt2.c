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
    #include "GMAC_SFR.h"

    #include "fsp_common_api.h"
    #include "r_ether_rzt2.h"

    #undef  R_GMAC_BASE
    #define R_GMAC_BASE    RZN1_GMAC1_BASE
    #define R_ETH_REG      RZN1_SWITCH_BASE

    #define R_BSP_MODULE_START(a, b)
    #define R_BSP_MODULE_STOP(a, b)

    static void R_BSP_IrqCfgEnable (IRQn_Type const irq, uint32_t priority, void * p_context);
    static void * R_FSP_IsrContextGet (IRQn_Type const irq);
    static int RegisterInterrupt (IRQn_Type irq, uint32_t priority, void *isr);

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
    #include "bsp_api.h"
    #include "r_ether_rzt2.h"
#ifndef RZT2_RSK_PNS
#include "FreeRTOS.h"
#include "task.h"
#endif
    #define Delay_usec(usecs_delay) R_BSP_SoftwareDelay(usecs_delay, BSP_DELAY_UNITS_MICROSECONDS)
//    #define RZT2_FPGA

#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */
#if 0
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/
#ifndef ETHER_ERROR_RETURN

/*LDRA_INSPECTED 77 S This macro does not work when surrounded by parentheses. */
 #define ETHER_ERROR_RETURN(a, err)    FSP_ERROR_RETURN((a), (err))
#endif

#define ETHER_ETHERC_REG_SIZE                           (0x400UL)
#define ETHER_ETHERC_EDMAC_REG_SIZE                     (0x100UL)

#define ETHER_ETHERC_INITIALIZATION_WAIT_CYCLE          (0x64UL)

/** "ETHE" in ASCII.  Used to determine if the control block is open. */
#define ETHER_OPEN                                      (0x45544845U)

/* Definition of the maximum / minimum number of data that can be sent at one time in the Ethernet */
#define ETHER_MAXIMUM_FRAME_SIZE                        (1514U) /* Maximum number of transmitted data */
#define ETHER_MINIMUM_FRAME_SIZE                        (60U)   /* Minimum number of transmitted data */

/* Bit definitions of control/status member of Transmit Descriptor-0 */
#define ETHER_TDES0_OWN                                 (1UL << 31)         /* Own Bit */
#define ETHER_TDES0_IC                                  (1UL << 30)         /* Interrupt on Completion */
#define ETHER_TDES0_LS                                  (1UL << 29)         /* Last Segment */
#define ETHER_TDES0_FS                                  (1UL << 28)         /* First Segment */
#define ETHER_TDES0_DC                                  (1UL << 27)         /* Disable CRC */
#define ETHER_TDES0_DP                                  (1UL << 26)         /* Disable Pad */
#define ETHER_TDES0_TTSE                                (1UL << 25)         /* Transmit Timestamp Enable */
#define ETHER_TDES0_CIC_MASK                            (3UL << 22)         /* Checksum Insertion Control */
#define ETHER_TDES0_TER                                 (1UL << 21)         /* Transmit End of Ring */
#define ETHER_TDES0_TCH                                 (1UL << 20)         /* Second Address Chained */
#define ETHER_TDES0_TTSS                                (1UL << 17)         /* Transmit Timestamp Status */
#define ETHER_TDES0_IHE                                 (1UL << 16)         /* IP Header Error */
#define ETHER_TDES0_ES                                  (1UL << 15)         /* Error Summary */
#define ETHER_TDES0_JT                                  (1UL << 14)         /* Jabber Timeout */
#define ETHER_TDES0_FF                                  (1UL << 13)         /* Frame Flushed */
#define ETHER_TDES0_IPE                                 (1UL << 12)         /* IP Payload Error */
#define ETHER_TDES0_LOC                                 (1UL << 11)         /* Loss of Carrier */
#define ETHER_TDES0_NC                                  (1UL << 10)         /* No Carrier */
#define ETHER_TDES0_LC                                  (1UL << 9)          /* Late Collision */
#define ETHER_TDES0_EC                                  (1UL << 8)          /* Excessive Collision */
#define ETHER_TDES0_VF                                  (1UL << 7)          /* VLAN Frame */
#define ETHER_TDES0_CC_MASK                             (0xfUL << 3)        /* Collision Count (Status field) */
#define ETHER_TDES0_ED                                  (1UL << 2)          /* Excessive Deferral */
#define ETHER_TDES0_UF                                  (1UL << 1)          /* Underflow Error */
#define ETHER_TDES0_DB                                  (1UL << 0)          /* Deferred Bit */

/* Bit definitions of control/status member of Transmit Descriptor-1 */
#define ETHER_RDES1_TBS2_MASK                           (0x1fffUL << 16)    /* Receive Buffer 2 Size - Mask */
#define ETHER_RDES1_TBS2_SHFIT                          16                  /* Receive Buffer 2 Size - Shift */
#define ETHER_RDES1_TBS1_MASK                           (0x1fffUL << 0)     /* Receive Buffer 1 Size -  */
#define ETHER_RDES1_TBS1_SHFIT                          0                   /* Receive Buffer 1 Size */

/* Bit definitions of control/status member of Receive Descriptor-0 */
#define ETHER_RDES0_OWN                                 (1UL << 31)         /* Own Bit */
#define ETHER_RDES0_AFM                                 (1UL << 30)         /* Destination Address Filter Fail */
#define ETHER_RDES0_FL_MASK                             (0x3fffUL << 16)    /* Frame Length - Mask */
#define ETHER_RDES0_FL_SHIFT                            16                  /* Frame Length - Shift */
#define ETHER_RDES0_ES                                  (1UL << 15)         /* Error Summary */
#define ETHER_RDES0_DE                                  (1UL << 14)         /* Descriptor Error */
#define ETHER_RDES0_SAF                                 (1UL << 13)         /* Source Address Filter Fail */
#define ETHER_RDES0_LE                                  (1UL << 12)         /* Length Error */
#define ETHER_RDES0_VLAN                                (1UL << 10)         /* VLAN Tag */
#define ETHER_RDES0_FS                                  (1UL << 9)          /* First Descriptor */
#define ETHER_RDES0_LS                                  (1UL << 8)          /* Last Descriptor */
#define ETHER_RDES0_TAGF                                (1UL << 7)          /* Timestamp Available or Giant Frame */
#define ETHER_RDES0_LC                                  (1UL << 6)          /* Late Collision */
#define ETHER_RDES0_FT                                  (1UL << 5)          /* Frame Type */
#define ETHER_RDES0_RWT                                 (1UL << 4)          /* Receive Watchdog Timeout */
#define ETHER_RDES0_RE                                  (1UL << 3)          /* Receive Error */
#define ETHER_RDES0_DBE                                 (1UL << 2)          /* Dribble Bit Error */
#define ETHER_RDES0_CE                                  (1UL << 1)          /* CRC Error */
#define ETHER_RDES0_ESA                                 (1UL << 0)          /* Extended Status Available */

/* Bit definitions of control/status member of Receive Descriptor-1 */
#define ETHER_RDES1_DIC                                 (1UL << 31)         /* Disable Interrupt on Completion */
#define ETHER_RDES1_RBS2_MASK                           (0x1fffUL << 16)    /* Receive Buffer 2 Size - Mask */
#define ETHER_RDES1_RBS2_SHFIT                          16                  /* Receive Buffer 2 Size - Shift */
#define ETHER_RDES1_RER                                 (1UL << 15)         /* Receive End of Ring */
#define ETHER_RDES1_RCH                                 (1UL << 14)         /* Second Address Chained */
#define ETHER_RDES1_RBS1_MASK                           (0x1fffUL << 0)     /* Receive Buffer 1 Size -  */
#define ETHER_RDES1_RBS1_SHFIT                          0                   /* Receive Buffer 1 Size */

/* Bit definition of Interrupt Enable Register */
#define ETHER_INTERRUPT_ENABLE_NIE          (1UL << 16)     /* Normal Interrupt Summary Enable */
#define ETHER_INTERRUPT_ENABLE_AIE          (1UL << 15)     /* Abnormal Interrupt Summary Enable */
#define ETHER_INTERRUPT_ENABLE_ERE          (1UL << 14)     /* Early Receive Interrupt Enable */
#define ETHER_INTERRUPT_ENABLE_FBE          (1UL << 13)     /* Fatal Bus Error Enable */
#define ETHER_INTERRUPT_ENABLE_ETE          (1UL << 10)     /* Early Transmit Interrupt Enable */
#define ETHER_INTERRUPT_ENABLE_RWE          (1UL << 9)      /* Receive Watchdog Timeout Enable */
#define ETHER_INTERRUPT_ENABLE_RSE          (1UL << 8)      /* Receive Stopped Enable */
#define ETHER_INTERRUPT_ENABLE_RUE          (1UL << 7)      /* Receive Buffer Unavailable Enable */
#define ETHER_INTERRUPT_ENABLE_RIE          (1UL << 6)      /* Receive Interrupt Enable */
#define ETHER_INTERRUPT_ENABLE_UNE          (1UL << 5)      /* Underflow Interrupt Enable */
#define ETHER_INTERRUPT_ENABLE_OVE          (1UL << 4)      /* Overflow Interrupt Enable */
#define ETHER_INTERRUPT_ENABLE_TJE          (1UL << 3)      /* Transmit Jabber Timeout Enable */
#define ETHER_INTERRUPT_ENABLE_TUE          (1UL << 2)      /* Transmit Buffer Unavailable Enable */
#define ETHER_INTERRUPT_ENABLE_TSE          (1UL << 1)      /* Transmit Stopped Enable */
#define ETHER_INTERRUPT_ENABLE_TIE          (1UL << 0)      /* Transmit Interrupt Enable */

/* Bit definition of MAC Frame Filter Register */
#define ETHER_MAC_FRAME_FILTER_RA           (1UL << 31)     /* Receive All */
#define ETHER_MAC_FRAME_FILTER_VTFE         (1UL << 16)     /* VLAN Tag Filter Enable */
#define ETHER_MAC_FRAME_FILTER_HPF          (1UL << 10)     /* Hash or Perfect Filter */
#define ETHER_MAC_FRAME_FILTER_SAF          (1UL << 9)      /* Source Address Filter Enable */
#define ETHER_MAC_FRAME_FILTER_SAIF         (1UL << 8)      /* SA Inverse Filtering */
#define ETHER_MAC_FRAME_FILTER_PCF_1        (1UL << 7)      /* Pass Control Frames 1 */
#define ETHER_MAC_FRAME_FILTER_PCF_0        (1UL << 6)      /* Pass Control Frames 0 */
#define ETHER_MAC_FRAME_FILTER_DBF          (1UL << 5)      /* Disable Broadcast Frames */
#define ETHER_MAC_FRAME_FILTER_PM           (1UL << 4)      /* Pass All Multicast */
#define ETHER_MAC_FRAME_FILTER_DAIF         (1UL << 3)      /* DA Inverse Filtering */
#define ETHER_MAC_FRAME_FILTER_HMC          (1UL << 2)      /* Hash Multicast */
#define ETHER_MAC_FRAME_FILTER_HUC          (1UL << 1)      /* Hash Unicast */
#define ETHER_MAC_FRAME_FILTER_PR           (1UL << 0)      /* Promiscuous Mode */

/* Bit definition of Flow Control Register */
#define ETHER_FLOW_CONTROL_PT_VALUE         (0x0000FFFFUL)
#define ETHER_FLOW_CONTROL_PLT_VALUE        (0x00000002UL)

#define ETHER_FLOW_CONTROL_PT               (ETHER_FLOW_CONTROL_PT_VALUE << 16) /* Pause Time (b31-b16) */
#define ETHER_FLOW_CONTROL_DZPQ             (1UL << 7)                          /* Disable Zero-Quanta Pause */
#define ETHER_FLOW_CONTROL_PLT              (ETHER_FLOW_CONTROL_PLT_VALUE << 4) /* Pause Low Threshold */
#define ETHER_FLOW_CONTROL_UP               (1UL << 3)                          /* Unicast Pause Frame Detect */
#define ETHER_FLOW_CONTROL_RFE              (1UL << 2)      /* Receive Flow Control Enable */
#define ETHER_FLOW_CONTROL_TFE              (1UL << 1)      /* Transmit Flow Control Enable */
#define ETHER_FLOW_CONTROL_FCA_BPA          (1UL << 0)      /* Flow Control Busy or Backpressure Activate */

/* Bit definition of Flow Control Register */
#define ETHER_PMT_CONTROL_STATUS_RWKFILTRST (1UL << 31)     /* Wake-Up Frame Filter Register Pointer Reset */
#define ETHER_PMT_CONTROL_STATUS_GLBLUCAST  (1UL << 9)      /* Global Unicas */
#define ETHER_PMT_CONTROL_STATUS_RWKPRCVD   (1UL << 6)      /* Wake-Up Frame Received */
#define ETHER_PMT_CONTROL_STATUS_MGKPRCVD   (1UL << 5)      /* Magic Packet Received */
#define ETHER_PMT_CONTROL_STATUS_RWKPKTEN   (1UL << 2)      /* Wake-Up Frame Enable */
#define ETHER_PMT_CONTROL_STATUS_MGKPKTEN   (1UL << 1)      /* Magic Packet Enable */
#define ETHER_PMT_CONTROL_STATUS_PWRDWN     (1UL << 0)      /* Power Down */


/* Bit definition of MMC_Receive_Interrupt_Mask/MMC_Transmit_Interrupt_Mask Register */
#define ETHER_MMC_RECEIVE_INTERRUPT_MASK_ALL_BIT    0x03FFFFFF
#define ETHER_MMC_TRANSMIT_INTERRUPT_MASK_ALL_BIT   0x03FFFFFF


/* Number of entries in PAUSE resolution table */
#define ETHER_PAUSE_TABLE_ENTRIES                       (8)

/* Local device and link partner PAUSE settings */
#define ETHER_PAUSE_XMIT_OFF                            (0)      /* The pause frame transmission is prohibited. */
#define ETHER_PAUSE_RECV_OFF                            (0)      /* The pause frame reception is prohibited.    */
#define ETHER_PAUSE_XMIT_ON                             (1)      /* The pause frame transmission is permitted.  */
#define ETHER_PAUSE_RECV_ON                             (1)      /* The pause frame reception is permitted.     */

/* Macro definition for buffer alignment adjustment */
#define ETHER_BUFFER_4BYTE_ALIGNMENT_MASK              (0x3UL) /* Mask for checking whether or not 4-byte alignment. */

/* PAUSE link mask and shift values */

/*
 * The mask value and shift value which are for that shift the bits form a line and
 * for comparing the bit information of PAUSE function which support the local device and
 * Link partner with the assorted table(pause_resolution) which enable or disable the PAUSE frame.
 */
#define ETHER_LINK_RESOLUTION_ABILITY_MASK              (3)
#define ETHER_LINK_RESOLUTION_LOCAL_ABILITY_BITSHIFT    (2)

/* Etherc mode */
#define ETHER_NO_USE_MAGIC_PACKET_DETECT                (0)
#define ETHER_USE_MAGIC_PACKET_DETECT                   (1)

/* ETHER_NO_DATA is the return value that indicates that no received data. */
#define ETHER_NO_DATA                                   (0)


/* Define DATA1 of Management-Tag */
#define ETHER_MANTAG_CONTROL_DATA1_PORT_MASK    0x0F
#define ETHER_MANTAG_CONTROL_DATA1_PORT_SHIFT   0
#define ETHER_MANTAG_CONTROL_DATA1_TIME_MASK    0x30
#define ETHER_MANTAG_CONTROL_DATA1_TIME_SHIFT   4
#define ETHER_MANTAG_CONTROL_DATA1_RED          (1 << 6)

/* PAUSE link mask and shift values */
#endif


/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
#if (ETHER_CFG_PARAM_CHECKING_ENABLE)
static fsp_err_t ether_open_param_check(ether_instance_ctrl_t const * const p_instance_ctrl,
                                        ether_cfg_t const * const           p_cfg);

#endif

RZT2_FUNC_ATTR static void      ether_enable_icu(ether_instance_ctrl_t * const p_instance_ctrl);
RZT2_FUNC_ATTR static void      ether_disable_icu(ether_instance_ctrl_t * const p_instance_ctrl);
RZT2_FUNC_ATTR void      ether_reset_mac(volatile R_GMAC_Type * const p_reg);
RZT2_FUNC_ATTR void      ether_init_descriptors(ether_instance_ctrl_t * const p_instance_ctrl);
RZT2_FUNC_ATTR fsp_err_t ether_buffer_get(ether_instance_ctrl_t * const p_instance_ctrl,
                                  void ** const                 p_buffer,
                                  uint32_t                    * p_buffer_size);
RZT2_FUNC_ATTR void      ether_config_ethernet(ether_instance_ctrl_t const * const p_instance_ctrl, const uint8_t mode);
RZT2_FUNC_ATTR void      ether_pause_resolution(uint32_t const local_ability,
                                   uint32_t const partner_ability,
                                   uint32_t     * ptx_pause,
                                   uint32_t     * prx_pause);
RZT2_FUNC_ATTR void      ether_configure_mac(ether_instance_ctrl_t * const p_instance_ctrl,
                                const uint8_t                 mac_addr[],
                                const uint8_t                 mode);
RZT2_FUNC_ATTR void      ether_disable (ether_instance_ctrl_t * const p_instance_ctrl);
RZT2_FUNC_ATTR fsp_err_t ether_do_link (ether_instance_ctrl_t * const p_instance_ctrl, const uint32_t port);
RZT2_FUNC_ATTR void      ether_magic_packet_detection (ether_instance_ctrl_t * const p_instance_ctrl);

RZT2_FUNC_ATTR fsp_err_t ether_link_status_check(ether_instance_ctrl_t const * const p_instance_ctrl, uint32_t port);
uint8_t   ether_check_magic_packet_detection_bit(ether_instance_ctrl_t const * const p_instance_ctrl);

void ether_callback_switch (ether_switch_callback_args_t * const p_arg);

RZT2_FUNC_ATTR uint32_t  ether_convert_port_bit_to_num( uint32_t in_bits );

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/** Name of module used by error logger macro */
#if BSP_CFG_ERROR_LOG != 0
static const char g_module_name[] = "ether";
#endif

#if defined(__GNUC__)

/* This structure is affected by warnings from a GCC compiler bug. This pragma suppresses the warnings in this
 * structure only.*/

/*LDRA_INSPECTED 69 S */
 #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

/** ETHER HAL API mapping for Ethernet Controller interface */
/*LDRA_INSPECTED 27 D This structure must be accessible in user code. It cannot be static. */
const ether_api_t g_ether_on_ether =
{
    .open            = R_ETHER_Open,
    .close           = R_ETHER_Close,
    .read            = R_ETHER_Read,
    .bufferRelease   = R_ETHER_BufferRelease,
    .write           = R_ETHER_Write,
    .linkProcess     = R_ETHER_LinkProcess,
    .wakeOnLANEnable = R_ETHER_WakeOnLANEnable,
};

/*
 * PAUSE Resolution as documented in IEEE 802.3-2008_section2 Annex
 * 28B, Table 28B-3. The following table codify logic that
 * determines how the PAUSE is configured for local transmitter
 * and receiver and partner transmitter and receiver.
 */
static const ether_pause_resolution_t pause_resolution[ETHER_PAUSE_TABLE_ENTRIES] =
{
    {ETHER_PAUSE_MASKC, ETHER_PAUSE_VAL0, ETHER_PAUSE_XMIT_OFF, ETHER_PAUSE_RECV_OFF  },
    {ETHER_PAUSE_MASKE, ETHER_PAUSE_VAL4, ETHER_PAUSE_XMIT_OFF, ETHER_PAUSE_RECV_OFF  },
    {ETHER_PAUSE_MASKF, ETHER_PAUSE_VAL6, ETHER_PAUSE_XMIT_OFF, ETHER_PAUSE_RECV_OFF  },
    {ETHER_PAUSE_MASKF, ETHER_PAUSE_VAL7, ETHER_PAUSE_XMIT_ON,  ETHER_PAUSE_RECV_OFF  },
    {ETHER_PAUSE_MASKE, ETHER_PAUSE_VAL8, ETHER_PAUSE_XMIT_OFF, ETHER_PAUSE_RECV_OFF  },
    {ETHER_PAUSE_MASKA, ETHER_PAUSE_VALA, ETHER_PAUSE_XMIT_ON,  ETHER_PAUSE_RECV_ON   },
    {ETHER_PAUSE_MASKF, ETHER_PAUSE_VALC, ETHER_PAUSE_XMIT_OFF, ETHER_PAUSE_RECV_OFF  },
    {ETHER_PAUSE_MASKF, ETHER_PAUSE_VALD, ETHER_PAUSE_XMIT_OFF, ETHER_PAUSE_RECV_ON   }
};


/*******************************************************************************************************************//**
 * @addtogroup ETHER
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/********************************************************************************************************************//**
 * @brief After ETHERC, EDMAC and PHY-LSI are reset in software, an auto negotiation of PHY-LSI is begun.
 * Afterwards, the link signal change interrupt is permitted. Implements @ref ether_api_t::open.
 *
 * @retval  FSP_SUCCESS                             Channel opened successfully.
 * @retval  FSP_ERR_ASSERTION                       Pointer to ETHER control block or configuration structure is NULL.
 * @retval  FSP_ERR_ALREADY_OPEN                    Control block has already been opened or channel is being used by another
 *                                                  instance. Call close() then open() to reconfigure.
 * @retval  FSP_ERR_ETHER_ERROR_PHY_COMMUNICATION   Initialization of PHY-LSI failed.
 * @retval  FSP_ERR_INVALID_CHANNEL                 Invalid channel number is given.
 * @retval  FSP_ERR_INVALID_POINTER                 Pointer to MAC address is NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT                Interrupt is not enabled.
 * @retval  FSP_ERR_ETHER_PHY_ERROR_LINK            Initialization of PHY-LSI failed.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_Open (ether_ctrl_t * const p_ctrl, ether_cfg_t const * const p_cfg)
{
    fsp_err_t               err             = FSP_SUCCESS;
    ether_instance_ctrl_t * p_instance_ctrl = (ether_instance_ctrl_t *) p_ctrl;
    volatile R_GMAC_Type *  p_reg_etherc    = (R_GMAC_Type *) R_GMAC_BASE;
    int                     i;
    __attribute__((unused))uint8_t                 open_phy[ETHER_NUM_PORT] = {0};
    uint8_t                 open_switch              = 0;

#if (ETHER_CFG_PARAM_CHECKING_ENABLE)

    /** Check parameters. */
    err = ether_open_param_check(p_instance_ctrl, p_cfg); /** check arguments */
    ETHER_ERROR_RETURN((FSP_SUCCESS == err), err);
#endif

    /** Make sure this channel exists. */
    p_instance_ctrl->p_reg_etherc = (void *)p_reg_etherc;
    p_instance_ctrl->p_reg_access = (void *)R_ETHSS;

    /* Initialize configuration of Ethernet module. */
    p_instance_ctrl->p_ether_cfg = p_cfg;

    /* Initialize the flags */
    p_instance_ctrl->magic_packet          = ETHER_MAGIC_PACKET_NOT_DETECTED;

    for (i=0; ETHER_NUM_PORT>i; i++)
    {
        p_instance_ctrl->link_establish_status[i] = ETHER_LINK_ESTABLISH_STATUS_DOWN;
        p_instance_ctrl->link_change[i]           = ETHER_LINK_CHANGE_NO_CHANGE;
        p_instance_ctrl->previous_link_status[i]  = ETHER_PREVIOUS_LINK_STATUS_DOWN;
    }

    p_instance_ctrl->status_link = 0;

    /* Initialize the transmit and receive descriptor */
    memset(p_instance_ctrl->p_ether_cfg->p_rx_descriptors,
           0x00,
           sizeof(ether_instance_descriptor_t) *
           p_instance_ctrl->p_ether_cfg->num_rx_descriptors);
    memset(p_instance_ctrl->p_ether_cfg->p_tx_descriptors,
           0x00,
           sizeof(ether_instance_descriptor_t) *
           p_instance_ctrl->p_ether_cfg->num_tx_descriptors);


    /* Initialize the Ether buffer */
    for (i = 0;
         i < (p_instance_ctrl->p_ether_cfg->num_tx_descriptors + p_instance_ctrl->p_ether_cfg->num_rx_descriptors);
         i++)
    {
        memset(p_instance_ctrl->p_ether_cfg->pp_ether_buffers[i],
               0x00,
               p_instance_ctrl->p_ether_cfg->ether_buffer_size);
    }

#ifdef RZT2_RSK
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_RESET);
    R_BSP_MODULE_START(FSP_IP_ETHSS, p_cfg->channel);
    R_BSP_MODULE_START(FSP_IP_GMAC, p_cfg->channel);
    R_BSP_MODULE_START(FSP_IP_ETHSW, p_cfg->channel);
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_RESET);

    R_RWP_NS->PRCRN = 0xA502;
    R_SYSC_NS->MRCTLE_b.MRCTLE00 = 0;
    R_SYSC_NS->MRCTLE_b.MRCTLE01 = 0;
    R_SYSC_NS->MRCTLE_b.MRCTLE02 = 0;
    R_SYSC_NS->MRCTLE_b.MRCTLE05 = 0;
    R_SYSC_NS->MRCTLE_b.MRCTLE06 = 0;
    R_RWP_NS->PRCRN = 0xA500;
#else /* !RZT2_RSK */
    R_BSP_MODULE_START(FSP_IP_ETHER, p_instance_ctrl->p_ether_cfg->channel);
#endif


    /* Initilize Ethernet Access register */
    R_ETHER_ACCESS_Initilize (p_instance_ctrl->p_reg_access,
            (ETHER_CFG_LINK_PRESENT0 << 0)|(ETHER_CFG_LINK_PRESENT1 << 1)|(ETHER_CFG_LINK_PRESENT2 << 2));

    /* Software reset */
    ether_reset_mac(p_instance_ctrl->p_reg_etherc);

    /* Open Ether-Switch Driver */
    {
        ether_switch_ctrl_t * p_switch_ctl;
        ether_switch_cfg_t  * p_switch_cfg;
        fsp_err_t               switch_ret;

        p_switch_ctl = p_instance_ctrl->p_ether_cfg->p_ether_switch_instance->p_ctrl;
        p_switch_cfg = (ether_switch_cfg_t *)p_instance_ctrl->p_ether_cfg->p_ether_switch_instance->p_cfg;
        p_switch_cfg->p_context  = p_ctrl;

#if (ETHER_CFG_USE_LINKSTA == 1)
        p_switch_cfg->p_callback = ether_callback_switch;
#elif (ETHER_CFG_USE_LINKSTA == 0)
  // :      p_switch_cfg->p_callback = 0;
#endif

        switch_ret = p_instance_ctrl->p_ether_cfg->p_ether_switch_instance->p_api->open(
                            p_switch_ctl, p_switch_cfg);

        if (FSP_SUCCESS == switch_ret)
        {
            err = FSP_SUCCESS;
            open_switch = 1;
        }
        else
        {
            err = switch_ret;
        }
    }
#ifndef RZT2_RSK_PNS
    /* Open Ether-Phy Driver and Start Auto-Negotiate */
    if (FSP_SUCCESS == err)
    {
        fsp_err_t               phy_ret;

        /* Software reset the PHY */
        for (i=0; ETHER_NUM_PORT>i; i++)
        {
            phy_ret = p_instance_ctrl->p_ether_cfg->p_ether_phy_instance->p_api->open(
                (p_instance_ctrl->p_ether_cfg->p_ether_phy_instance + i)->p_ctrl,
                (p_instance_ctrl->p_ether_cfg->p_ether_phy_instance + i)->p_cfg);

            if (FSP_SUCCESS == phy_ret)
            {
                open_phy[i] = 1;
                p_instance_ctrl->p_ether_cfg->p_ether_phy_instance->p_api->startAutoNegotiate(
                    (p_instance_ctrl->p_ether_cfg->p_ether_phy_instance +i)->p_ctrl);
            }
            else
            {
                break;
            }
        }

        if (FSP_ERR_ETHER_PHY_ERROR_LINK == phy_ret)
        {
                err = FSP_ERR_ETHER_ERROR_PHY_COMMUNICATION;
        }
        else
        {
                err = phy_ret;
        }
    }
#endif /* #ifndef RZT2_RSK_PNS */
    if (FSP_SUCCESS == err)
    {
        ether_enable_icu(p_instance_ctrl);
        p_instance_ctrl->open = ETHER_OPEN;

        #ifdef BSP_PLAT_RZN1SDB /* RZ/N1S */
        {
            err  = RegisterInterrupt (p_instance_ctrl->p_ether_cfg->sbd_irq,
                                  p_instance_ctrl->p_ether_cfg->sbd_interrupt_priority,
                                  (void *)&ether_isr_sbd);

            err |= RegisterInterrupt (p_instance_ctrl->p_ether_cfg->pmt_irq,
                                  p_instance_ctrl->p_ether_cfg->pmt_interrupt_priority,
                                  (void *)&ether_isr_pmt);
        }
        #endif /* !BSP_PLAT_RZN1SDB */  /* RZ/T2 */
    }
    else
    {
#ifndef RZT2_RSK_PNS
        /* Close operation */
        for (i=0; ETHER_NUM_PORT>i; i++)
        {
            if ( (0 != open_switch) && (0 != open_phy) )
            {
                p_instance_ctrl->p_ether_cfg->p_ether_phy_instance->p_api->close(
                    (p_instance_ctrl->p_ether_cfg->p_ether_phy_instance + i)->p_ctrl);
            }
        }
#endif /* #ifndef RZT2_RSK_PNS */
        if (0 != open_switch)
        {
                p_instance_ctrl->p_ether_cfg->p_ether_switch_instance->p_api->close(
                    p_instance_ctrl->p_ether_cfg->p_ether_switch_instance->p_ctrl);
        }
    }
    return err;
}                                      /* End of function R_ETHER_Open() */

/********************************************************************************************************************//**
 * @brief Disables interrupts. Removes power and releases hardware lock. Implements @ref ether_api_t::close.
 *
 * @retval  FSP_SUCCESS                                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened
 *
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_Close (ether_ctrl_t * const p_ctrl)
{
    fsp_err_t               err             = FSP_SUCCESS;
    int                     i;

    ether_instance_ctrl_t * p_instance_ctrl = (ether_instance_ctrl_t *) p_ctrl;
    volatile R_GMAC_Type *   p_reg_etherc;

#if (ETHER_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_ERROR_RETURN(ETHER_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    p_reg_etherc = (R_GMAC_Type *) p_instance_ctrl->p_reg_etherc;

    /* Disable Ethernet interrupt. */
    ether_disable_icu(p_instance_ctrl);

    for (i=0; ETHER_NUM_PORT>i; i++)
    {
        p_instance_ctrl->p_ether_cfg->p_ether_phy_instance->p_api->close(
            (p_instance_ctrl->p_ether_cfg->p_ether_phy_instance + i)->p_ctrl);
    }

    p_instance_ctrl->p_ether_cfg->p_ether_switch_instance->p_api->close(
            p_instance_ctrl->p_ether_cfg->p_ether_switch_instance->p_ctrl);

    p_reg_etherc->Interrupt_Enable = 0;

    /* Disable TE and RE  */
    p_reg_etherc->MAC_Configuration_b.RE = 0;
    p_reg_etherc->MAC_Configuration_b.TE = 0;

    /* Initialize the flags */
    p_instance_ctrl->magic_packet          = ETHER_MAGIC_PACKET_NOT_DETECTED;

    for (i=0; ETHER_NUM_PORT>i; i++)
    {
        p_instance_ctrl->link_establish_status[i] = ETHER_LINK_ESTABLISH_STATUS_DOWN;
        p_instance_ctrl->link_change[i]           = ETHER_LINK_CHANGE_NO_CHANGE;
        p_instance_ctrl->previous_link_status[i]  = ETHER_PREVIOUS_LINK_STATUS_DOWN;
    }

    p_instance_ctrl->status_link = 0;

    /** Remove power to the channel. */
#ifdef RZT2_RSK
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_RESET);
    R_BSP_MODULE_STOP(FSP_IP_ETHSS, p_cfg->channel);
    R_BSP_MODULE_STOP(FSP_IP_GMAC, p_cfg->channel);
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_RESET);



#else /* !RZT2_RSK */
    R_BSP_MODULE_STOP(FSP_IP_ETHER, p_instance_ctrl->p_ether_cfg->channel);
#endif



    /** Clear configure block parameters. */
    p_instance_ctrl->p_ether_cfg = NULL;

    /** Mark the channel not open so other APIs cannot use it. */
    p_instance_ctrl->open = 0U;

    return err;
}                                      /* End of function R_ETHER_Close() */

/********************************************************************************************************************//**
 * @brief Move to the next buffer in the circular receive buffer list. Implements @ref ether_api_t::bufferRelease.
 *
 * @retval  FSP_SUCCESS                             Processing completed successfully.
 * @retval  FSP_ERR_ASSERTION                       Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN                        The control block has not been opened
 * @retval  FSP_ERR_ETHER_ERROR_MAGIC_PACKET_MODE   As a Magic Packet is being detected, transmission and reception is
 *                                                  not enabled.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_BufferRelease (ether_ctrl_t * const p_ctrl)
{
    fsp_err_t                       err             = FSP_SUCCESS;
    ether_instance_ctrl_t *         p_instance_ctrl = (ether_instance_ctrl_t *) p_ctrl;
    volatile R_GMAC_Type *          p_reg_etherc;
    ether_link_establish_status_t   link_establish_status = ETHER_LINK_ESTABLISH_STATUS_DOWN;
    uint32_t                        i;
 //   taskENTER_CRITICAL();

    /* Check argument */
#if (ETHER_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_ERROR_RETURN(ETHER_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* When the Link up processing is not completed, return error */
    for (i=0; i<ETHER_NUM_PORT; i++)
    {
        link_establish_status |= p_instance_ctrl->previous_link_status[i];
    }

    ETHER_ERROR_RETURN(ETHER_LINK_ESTABLISH_STATUS_UP == link_establish_status,
                       FSP_ERR_ETHER_ERROR_LINK);

    /* In case of detection mode of magic packet, return error. */
    ETHER_ERROR_RETURN(0 == ether_check_magic_packet_detection_bit(p_instance_ctrl),
                       FSP_ERR_ETHER_ERROR_MAGIC_PACKET_MODE);

    /* When receive data exists */
    if (ETHER_RDES0_OWN != (p_instance_ctrl->p_rx_descriptor->des0 & ETHER_RDES0_OWN))
    {

        /* Enable current descriptor */
        p_instance_ctrl->p_rx_descriptor->des0 |= ETHER_RDES0_OWN;

        /* Update pointer to the currently referenced receive descriptor  */
        if (ETHER_RDES1_RER == (p_instance_ctrl->p_rx_descriptor->des1 &  ETHER_RDES1_RER))
        {
            p_instance_ctrl->p_rx_descriptor = p_instance_ctrl->p_ether_cfg->p_rx_descriptors;
        }
        else
        {
            p_instance_ctrl->p_rx_descriptor++;
        }
    }

    p_reg_etherc = (R_GMAC_Type *) p_instance_ctrl->p_reg_etherc;
    p_reg_etherc->Receive_Poll_Demand_b.RPD = 0;

    err = FSP_SUCCESS;
  //  taskEXIT_CRITICAL();
    return err;
}                                      /* End of function R_ETHER_BufferRelease() */

/********************************************************************************************************************//**
 * @brief The Link up processing, the Link down processing, and the magic packet detection processing are executed.
 *  Implements @ref ether_api_t::linkProcess.
 *
 * @retval  FSP_SUCCESS                                 Link is up.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened.
 * @retval  FSP_ERR_ETHER_ERROR_LINK                    Link is down.
 * @retval  FSP_ERR_ETHER_ERROR_PHY_COMMUNICATION       When reopening the PHY interface initialization of the PHY-LSI failed.
 * @retval  FSP_ERR_ALREADY_OPEN                        When reopening the PHY interface it was already opened.
 * @retval  FSP_ERR_INVALID_CHANNEL                     When reopening the PHY interface an invalid channel was passed.
 * @retval  FSP_ERR_INVALID_POINTER                     When reopening the PHY interface the MAC address pointer was NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT                    When reopening the PHY interface the interrupt was not enabled.
 * @retval  FSP_ERR_ETHER_PHY_ERROR_LINK                Initialization of the PHY-LSI failed.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_LinkProcess (ether_ctrl_t * const p_ctrl)
{
    fsp_err_t               err             = FSP_SUCCESS;
    ether_instance_ctrl_t * p_instance_ctrl = (ether_instance_ctrl_t *) p_ctrl;

    ether_callback_args_t                   callback_arg;
    ether_cfg_t const *                     p_ether_cfg;
    volatile ether_previous_link_status_t   previous_link_status[ETHER_NUM_PORT];
    ether_previous_link_status_t            all_previous_link_status;

    uint32_t port;
    uint32_t i;

#if (ETHER_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_ERROR_RETURN(ETHER_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    port = ether_convert_port_bit_to_num(p_instance_ctrl->link_proc_port);

#if (ETHER_CFG_PARAM_CHECKING_ENABLE)
    ETHER_ERROR_RETURN(ETHER_NUM_PORT > port, FSP_ERR_INVALID_ARGUMENT);
#endif

    p_ether_cfg = p_instance_ctrl->p_ether_cfg;


    /* When the magic packet is detected. */
    if (ETHER_MAGIC_PACKET_DETECTED == p_instance_ctrl->magic_packet)
    {
        p_instance_ctrl->magic_packet = ETHER_MAGIC_PACKET_NOT_DETECTED;

        if (NULL != p_ether_cfg->p_callback)
        {
            callback_arg.channel      = p_ether_cfg->channel;
            callback_arg.event        = ETHER_EVENT_WAKEON_LAN;
            callback_arg.status_ether = 0;
            callback_arg.status_link  = 0;
            (*p_ether_cfg->p_callback)((void *) &callback_arg);
        }

        /*
         * After the close function is called, the open function is called
         * to have to set ETHERC to a usual operational mode
         * to usually communicate after magic packet is detected.
         *//* back up previous_link_status */
        for (i=0; i<ETHER_NUM_PORT; i++)
        {
            previous_link_status[i] = p_instance_ctrl->previous_link_status[i];
        }


        err = R_ETHER_Close((ether_ctrl_t *) p_instance_ctrl);
        ETHER_ERROR_RETURN(FSP_SUCCESS == err, err);

        err = R_ETHER_Open((ether_ctrl_t *) p_instance_ctrl, (ether_cfg_t *) p_ether_cfg);
        ETHER_ERROR_RETURN(FSP_SUCCESS == err, err);

        /* restore previous_link_status */
        for (i=0; i<ETHER_NUM_PORT; i++)
        {
            p_instance_ctrl->previous_link_status[i] = previous_link_status[i];
        }
    }

    /* Previous link status of all ports except specified port */
    all_previous_link_status = ETHER_PREVIOUS_LINK_STATUS_DOWN;
    for (i=0; i<ETHER_NUM_PORT; i++)
    {
        if (port != i)
        {
            all_previous_link_status |= p_instance_ctrl->previous_link_status[i];
        }
    }

#if (ETHER_CFG_USE_LINKSTA == 1)
    if( (p_instance_ctrl->status_link & (1UL << port)) != 0)
    {
        /* When becoming Link up */
        if (ETHER_PREVIOUS_LINK_STATUS_DOWN == p_instance_ctrl->previous_link_status[port])
        {
            p_instance_ctrl->link_change[port] = ETHER_LINK_CHANGE_LINK_UP;
            p_instance_ctrl->previous_link_status[port] = ETHER_PREVIOUS_LINK_STATUS_UP;
        }
        err = FSP_SUCCESS;
    }
    else
    {
        /* When becoming Link down */
        if (ETHER_PREVIOUS_LINK_STATUS_UP == p_instance_ctrl->previous_link_status[port])
        {
            p_instance_ctrl->link_change[port] = ETHER_LINK_CHANGE_LINK_DOWN;
            p_instance_ctrl->previous_link_status[port] = ETHER_PREVIOUS_LINK_STATUS_DOWN;
        }
        err = FSP_ERR_ETHER_ERROR_LINK;
    }
#elif (ETHER_CFG_USE_LINKSTA == 0)
    err = ether_link_status_check(p_instance_ctrl, port);

    /* The state of the link status in PHY-LSI is confirmed and Link Up/Down is judged. */
    if (FSP_SUCCESS == err)
    {
        /* When becoming Link up */
        if (ETHER_PREVIOUS_LINK_STATUS_DOWN == p_instance_ctrl->previous_link_status[port])
        {
            p_instance_ctrl->link_change[port] = ETHER_LINK_CHANGE_LINK_UP;

            /* Update Link status */
            p_instance_ctrl->previous_link_status[port] = ETHER_PREVIOUS_LINK_STATUS_UP;
        }
    }
    else
    {
        /* When becoming Link down */
        if (ETHER_PREVIOUS_LINK_STATUS_UP == p_instance_ctrl->previous_link_status[port])
        {
            p_instance_ctrl->link_change[port] = ETHER_LINK_CHANGE_LINK_DOWN;

            /* Update Link status */
            p_instance_ctrl->previous_link_status[port] = ETHER_PREVIOUS_LINK_STATUS_DOWN;
        }
    }
#endif

    /* When the link is up */
    if (ETHER_LINK_CHANGE_LINK_UP == p_instance_ctrl->link_change[port])
    {
        /*
         * The status of the LINK signal became "link-up" even if PHY-LSI did not detect "link-up"
         * after a reset. To avoid this wrong detection, processing in R_ETHER_LinkProcess has been modified to
         * clear the flag after link-up is confirmed in R_ETHER_CheckLink_ZC.
         */
        p_instance_ctrl->link_change[port] = ETHER_LINK_CHANGE_NO_CHANGE;

        if (ETHER_PREVIOUS_LINK_STATUS_DOWN == all_previous_link_status)
        {
            /* Previous link status of all ports are down */
            /* Initialize the transmit and receive descriptor */
            memset(p_ether_cfg->p_rx_descriptors,
                   0x00,
                   sizeof(ether_instance_descriptor_t) * p_ether_cfg->num_rx_descriptors);
            memset(p_ether_cfg->p_tx_descriptors,
                   0x00,
                   sizeof(ether_instance_descriptor_t) * p_ether_cfg->num_tx_descriptors);

            /* Initialize the Ether buffer */
            for (i = 0;
                 i < (p_ether_cfg->num_tx_descriptors + p_ether_cfg->num_rx_descriptors);
                 i++)
            {
                memset(p_ether_cfg->pp_ether_buffers[i],
                       0x00,
                       p_ether_cfg->ether_buffer_size);
            }

            /*
             * ETHERC and EDMAC are set after ETHERC and EDMAC are reset in software
             * and sending and receiving is permitted.
             */
            ether_configure_mac(p_instance_ctrl,
                                p_ether_cfg->p_mac_address,
                                ETHER_NO_USE_MAGIC_PACKET_DETECT);
        }

        err = ether_do_link(p_instance_ctrl, port);
        if (FSP_SUCCESS == err)
        {
            p_instance_ctrl->link_establish_status[port] = ETHER_LINK_ESTABLISH_STATUS_UP;

            if (NULL != p_ether_cfg->p_callback)
            {
                callback_arg.channel      = p_ether_cfg->channel;
                callback_arg.event        = ETHER_EVENT_LINK_ON;
                callback_arg.status_ether = 0;
                callback_arg.status_link  = p_instance_ctrl->link_proc_port;
                (*p_ether_cfg->p_callback)((void *) &callback_arg);
            }
        }
        else
        {
            /* When PHY auto-negotiation is not completed */
            p_instance_ctrl->link_establish_status[port] = ETHER_LINK_ESTABLISH_STATUS_DOWN;
            p_instance_ctrl->link_change[port]           = ETHER_LINK_CHANGE_LINK_UP;
        }
    }
    /* When the link is down */
    else if (ETHER_LINK_CHANGE_LINK_DOWN == p_instance_ctrl->link_change[port])
    {
        p_instance_ctrl->link_change[port] = ETHER_LINK_CHANGE_NO_CHANGE;

        if (ETHER_PREVIOUS_LINK_STATUS_DOWN == all_previous_link_status)
        {
            /* Disable receive and transmit. */
            ether_disable (p_instance_ctrl);
        }

        p_instance_ctrl->link_establish_status[port] = ETHER_LINK_ESTABLISH_STATUS_DOWN;

        if (NULL != p_ether_cfg->p_callback)
        {
            callback_arg.channel      = p_ether_cfg->channel;
            callback_arg.event        = ETHER_EVENT_LINK_OFF;
            callback_arg.status_ether = 0;
            callback_arg.status_link  = p_instance_ctrl->link_proc_port;
            (*p_ether_cfg->p_callback)((void *) &callback_arg);
        }

        err = FSP_ERR_ETHER_ERROR_LINK;
    }
    else
    {
        if ((ETHER_PREVIOUS_LINK_STATUS_UP == p_instance_ctrl->previous_link_status[port]) &&
            (ETHER_LINK_ESTABLISH_STATUS_UP != p_instance_ctrl->link_establish_status[port]))
        {
            err = ether_do_link(p_instance_ctrl, port);
            if (FSP_SUCCESS == err)
            {
                p_instance_ctrl->link_establish_status[port] = ETHER_LINK_ESTABLISH_STATUS_UP;

                if (NULL != p_ether_cfg->p_callback)
                {
                    callback_arg.channel      = p_ether_cfg->channel;
                    callback_arg.event        = ETHER_EVENT_LINK_ON;
                    callback_arg.status_ether = 0;
                    callback_arg.status_link  = p_instance_ctrl->link_proc_port;
                    (*p_ether_cfg->p_callback)((void *) &callback_arg);
                }
            }
        }
        else
        {
            /* no operation */
        }
    }

    return err;
} /* End of function R_ETHER_LinkProcess() */

/********************************************************************************************************************//**
 * @brief The setting of ETHERC is changed from normal sending and receiving mode to magic packet detection mode.
 * Implements @ref ether_api_t::wakeOnLANEnable.
 *
 * @retval  FSP_SUCCESS                                 Processing completed successfully.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened.
 * @retval  FSP_ERR_ETHER_PHY_ERROR_LINK                Initialization of PHY-LSI failed.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_WakeOnLANEnable (ether_ctrl_t * const p_ctrl)
{
    fsp_err_t   err     = FSP_SUCCESS;

    ether_instance_ctrl_t * p_instance_ctrl = (ether_instance_ctrl_t *) p_ctrl;

    /* Check argument */
#if (ETHER_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_ERROR_RETURN(ETHER_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* When the Link up processing is completed */
    /* Change to the magic packet detection mode.  */
    ether_configure_mac(p_instance_ctrl, p_instance_ctrl->p_ether_cfg->p_mac_address, ETHER_USE_MAGIC_PACKET_DETECT);
    ether_magic_packet_detection (p_instance_ctrl);

    return err;
}                                      /* End of function R_ETHER_WakeOnLANEnable() */

/********************************************************************************************************************//**
 * @brief Receive Ethernet frame. Receives data to the location specified by the pointer to the receive buffer.
 * In zero copy mode, the address of the receive buffer is returned.
 * In non zero copy mode, the received data in the internal buffer is copied to the pointer passed by the argument.
 * Implements @ref ether_api_t::read.
 *
 * @retval  FSP_SUCCESS                                 Processing completed successfully.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened.
 * @retval  FSP_ERR_ETHER_ERROR_NO_DATA                 There is no data in receive buffer.
 * @retval  FSP_ERR_ETHER_ERROR_MAGIC_PACKET_MODE       As a Magic Packet is being detected, transmission and reception
 *                                                      is not enabled.
 * @retval  FSP_ERR_ETHER_ERROR_FILTERING               Multicast Frame filter is enable, and Multicast Address Frame is
 *                                                      received.
 * @retval  FSP_ERR_INVALID_POINTER                     Value of the pointer is NULL.
 *
 ***********************************************************************************************************************/

#ifdef  ETHER_CONFIG_PTP
fsp_err_t get_current_timestamp (ether_instance_ctrl_t * p_instance_ctrl, uint8_t tim_num, uint32_t * tim_sec, uint32_t* tim_nsec)
{
    ether_switch_cmd_t          switch_cmd;
    ether_switch_timestamp_t    switch_tmv;
    fsp_err_t                   err;

    switch_tmv.tim_num = tim_num;

    switch_cmd.cmd     = ETHER_SWITCH_CMD_TIMER_VALUE;
    switch_cmd.flg_set = ETHER_SWITCH_FALSE;
    switch_cmd.port    = 0;
    switch_cmd.data.p_timestamp = &switch_tmv;

    err = p_instance_ctrl->p_ether_cfg->p_ether_switch_instance->p_api->control(
                p_instance_ctrl->p_ether_cfg->p_ether_switch_instance->p_ctrl, &switch_cmd);

    if (FSP_SUCCESS == err)
    {
        *tim_sec  = switch_tmv.tim_sec;
        *tim_nsec = switch_tmv.tim_nsec;
    }
    else
    {
        *tim_sec  = 0;
        *tim_nsec = 0;
    }

    return  err;
}
#endif /* ETHER_CONFIG_PTP */

fsp_err_t R_ETHER_Read (ether_ctrl_t * const p_ctrl, void * const p_buffer, uint32_t * const length_bytes)
{
    fsp_err_t                       err             = FSP_SUCCESS;
    ether_instance_ctrl_t         * p_instance_ctrl = (ether_instance_ctrl_t *) p_ctrl;
    uint32_t                        received_size   = ETHER_NO_DATA;
    uint8_t                      ** pp_read_buffer  = (uint8_t **) p_buffer;
    ether_frame_user_t            * p_ether_frame_user;
    ether_frame_mgtag_t           * p_ether_frame_mgtag;
    ether_link_establish_status_t   link_establish_status = ETHER_LINK_ESTABLISH_STATUS_DOWN;
    uint32_t                        i;

    /* Check argument */
#if (ETHER_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_ERROR_RETURN(ETHER_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    ETHER_ERROR_RETURN(NULL != p_buffer, FSP_ERR_INVALID_POINTER);
    ETHER_ERROR_RETURN(NULL != length_bytes, FSP_ERR_INVALID_POINTER);
#endif

    /* Retrieve the receive buffer location controlled by the  descriptor. */
    /* When the Link up processing is not completed, return error */
    for (i=0; i<ETHER_NUM_PORT; i++)
    {
        link_establish_status |= p_instance_ctrl->link_establish_status[i];
    }

    ETHER_ERROR_RETURN(ETHER_LINK_ESTABLISH_STATUS_UP == link_establish_status,
                       FSP_ERR_ETHER_ERROR_LINK);

    /* In case of detection mode of magic packet, return error. */
    ETHER_ERROR_RETURN(0 == ether_check_magic_packet_detection_bit(p_instance_ctrl),
                       FSP_ERR_ETHER_ERROR_MAGIC_PACKET_MODE);

    while (FSP_SUCCESS == err)
    {
        /* When receive data exists. */

        if (ETHER_RDES0_OWN != (p_instance_ctrl->p_rx_descriptor->des0 & ETHER_RDES0_OWN))
        {

            if ((ETHER_RDES0_ES == (p_instance_ctrl->p_rx_descriptor->des0 & ETHER_RDES0_ES)) ||
                ((ETHER_RDES0_FS|ETHER_RDES0_LS) != (p_instance_ctrl->p_rx_descriptor->des0 & (ETHER_RDES0_FS|ETHER_RDES0_LS))))
            {
                /* The buffer is released when it is at the error or when single-frame is not  in single descriptor. */
                err = R_ETHER_BufferRelease((ether_ctrl_t *) p_instance_ctrl);
                continue;
            }

            p_ether_frame_mgtag = (ether_frame_mgtag_t *)p_instance_ctrl->p_rx_descriptor->des2;
            received_size = (p_instance_ctrl->p_rx_descriptor->des0 & ETHER_RDES0_FL_MASK) >> ETHER_RDES0_FL_SHIFT;

            if ((sizeof(p_ether_frame_mgtag->dst_addr)+
                 sizeof(p_ether_frame_mgtag->src_addr)+
                 sizeof(p_ether_frame_mgtag->mgtag)) > received_size)
            {
                /* bytes received is short */
                err = R_ETHER_BufferRelease((ether_ctrl_t *) p_instance_ctrl);
                continue;
            }

            /* Check multicast is detected when multicast frame filter is enabled */
            if (ETHER_MULTICAST_DISABLE == p_instance_ctrl->p_ether_cfg->multicast)
            {
                if ((0x01 == p_ether_frame_mgtag->dst_addr[0]) &&
                    (0x00 == p_ether_frame_mgtag->dst_addr[1]) &&
                    (0x5E == p_ether_frame_mgtag->dst_addr[2]) &&
                    (0 == (0x80 & p_ether_frame_mgtag->dst_addr[3])))
                    /* The buffer is released at the multicast frame detect.  */
                {
                    err = R_ETHER_BufferRelease((ether_ctrl_t *) p_instance_ctrl);
                    if (FSP_SUCCESS == err)
                    {
                        err = FSP_ERR_ETHER_ERROR_FILTERING;
                    }
                    break;
                }
            }
        }
        else
        {
            err = FSP_ERR_ETHER_ERROR_NO_DATA;
        }
        break;
    }

    if (FSP_SUCCESS == err)
    {
        /* store receive port information */
        p_instance_ctrl->read_port =
            1 << ((p_ether_frame_mgtag->mgtag.control_data[1] & ETHER_MANTAG_CONTROL_DATA1_PORT_MASK) >> ETHER_MANTAG_CONTROL_DATA1_PORT_SHIFT);

        #ifdef  ETHER_CONFIG_PTP
        {
            uint32_t    current_sec;
            uint32_t    current_nsec;

            /* Timer used for timestamping the frame */
            p_instance_ctrl->rx_manage.tim_num  = 0;
                //-OS-((p_ether_frame_mgtag->mgtag.control_data[1] & ETHER_MANTAG_CONTROL_DATA1_TIME_MASK) >> ETHER_MANTAG_CONTROL_DATA1_TIME_SHIFT);
            /* RED period indication. */
            p_instance_ctrl->rx_manage.red      =
                (p_ether_frame_mgtag->mgtag.control_data[1] & ETHER_MANTAG_CONTROL_DATA1_RED) ? ETHER_TRUE: ETHER_FALSE;
            /* Received timestamp of the frame (seconds). */
            p_instance_ctrl->rx_manage.tim_nsec = ((uint32_t)p_ether_frame_mgtag->mgtag.control_data2[0] << 24)
                                                | ((uint32_t)p_ether_frame_mgtag->mgtag.control_data2[1] << 16)
                                                | ((uint32_t)p_ether_frame_mgtag->mgtag.control_data2[2] << 8)
                                                | (uint32_t)p_ether_frame_mgtag->mgtag.control_data2[3];
            /* Received timestamp of the frame (nanoseconds). */
            get_current_timestamp (p_instance_ctrl, p_instance_ctrl->rx_manage.tim_num, &current_sec, &current_nsec);
            if (current_nsec < p_instance_ctrl->rx_manage.tim_nsec)
            {
                current_sec--;
            }
            p_instance_ctrl->rx_manage.tim_sec = current_sec;
        }
        #endif /* ETHER_CONFIG_PTP */

        if (ETHER_ZEROCOPY_DISABLE == p_instance_ctrl->p_ether_cfg->zerocopy)
        {
            p_ether_frame_user  = (ether_frame_user_t *)p_buffer;

            /* copy destination addres from descripter's buffer to user's buffer */
            memcpy(p_ether_frame_user->dst_addr,
                   p_ether_frame_mgtag->dst_addr,
                   sizeof(p_ether_frame_mgtag->dst_addr));

            /* copy source addres from descripter's buffer to user's buffer */
            memcpy(p_ether_frame_user->src_addr,
                   p_ether_frame_mgtag->src_addr,
                   sizeof(p_ether_frame_mgtag->src_addr));

            /* copy type-field and datagram from descripter's buffer to user's buffer */
            memcpy(p_ether_frame_user->type,
                   p_ether_frame_mgtag->type,
                   (received_size - sizeof(p_ether_frame_mgtag->dst_addr)
                                  - sizeof(p_ether_frame_mgtag->src_addr)
                                  - sizeof(p_ether_frame_mgtag->mgtag)));

            err = R_ETHER_BufferRelease((ether_ctrl_t *) p_instance_ctrl);
        }
        else
        {
            *pp_read_buffer = (uint8_t *)p_ether_frame_mgtag + sizeof(p_ether_frame_mgtag->mgtag);
            p_ether_frame_user  = (ether_frame_user_t  *)*pp_read_buffer;

            /* move destination addres to the appropriate location */
            memcpy(p_ether_frame_user->src_addr,
                   p_ether_frame_mgtag->src_addr,
                   sizeof(p_ether_frame_mgtag->src_addr));

            /* move source addres to the appropriate location */
            memcpy(p_ether_frame_user->dst_addr,
                   p_ether_frame_mgtag->dst_addr,
                   sizeof(p_ether_frame_mgtag->dst_addr));
        }

        *length_bytes = received_size - sizeof(p_ether_frame_mgtag->mgtag) - ETHER_FCS_SIZE;
    }

    return err;
}                                      /* End of function R_ETHER_Read() */

/********************************************************************************************************************//**
 * @brief Transmit Ethernet frame. Transmits data from the location specified by the pointer to the transmit
 *  buffer, with the data size equal to the specified frame length.
 *  In the non zero copy mode, transmits data after being copied to the internal buffer.
 *  Implements @ref ether_api_t::write.
 *
 * @retval  FSP_SUCCESS                                 Processing completed successfully.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened.
 * @retval  FSP_ERR_ETHER_ERROR_MAGIC_PACKET_MODE       As a Magic Packet is being detected, transmission and reception
 *                                                      is not enabled.
 * @retval  FSP_ERR_ETHER_ERROR_TRANSMIT_BUFFER_FULL    Transmit buffer is not empty.
 * @retval  FSP_ERR_INVALID_POINTER                     Value of the pointer is NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT                    Value of the send frame size is out of range.
 *
 ***********************************************************************************************************************/
#if 0
/* Define management tag for transmit */
#define TX_MGTAG_DATA_FORWARD_FORCE     (1UL << 0)
#define TX_MGTAG_DATA_TIMESTAMP         (1UL << 3)
#define TX_MGTAG_DATA_ONE_STEP          (1UL << 4)
#define TX_MGTAG_DATA_PRP_SUPPRESS      (1UL << 5)
#define TX_MGTAG_DATA_PRP_FORCE         (1UL << 6)
#define TX_MGTAG_DATA_PRP_SEQUENCE      (1UL << 7)
#define TX_MGTAG_DATA_QUE_NUM           (1UL << 9)
#define TX_MGTAG_DATA_TIM_NUM           (1UL << 13)

#define TX_MGTAG_DATA_QUE_NUM_SHIFT     10
#define TX_MGTAG_DATA_QUE_NUM_MASK      (0x7 << TX_MGTAG_DATA_QUE_NUM_SHIFT)
#define TX_MGTAG_DATA_TIM_NUM_SHIFT     14
#define TX_MGTAG_DATA_TIM_NUM_MASK      (0x1 << TX_MGTAG_DATA_TIM_NUM_SHIFT)

#define TX_MGTAG_DATA2_PORT_SHIFT       0
#define TX_MGTAG_DATA2_PORT_MASK        (0x7 << TX_MGTAG_DATA2_PORT_SHIFT)

#ifdef WS2 /* FPGA Ver2 */
    #define TX_MGTAG_DATA2_TSID_SHIFT       9
    #define TX_MGTAG_DATA2_TSID_MASK        (0x7F << TX_MGTAG_DATA2_TSID_SHIFT)
#else /* !WS2 */ /* FPGA Ver1 */
    #define TX_MGTAG_DATA2_TSID_SHIFT       12
    #define TX_MGTAG_DATA2_TSID_MASK        (0xF << TX_MGTAG_DATA2_TSID_SHIFT)
#endif /* !WS2 */ /* FPGA Ver1 */
#endif
/* #define ISSUE_NO33 */
#ifdef  ISSUE_NO33  /* �s��?����� */
#define     GMAC_LOG_MAX    512

#define     GMAC_LOG_UPDATE     { \
                                    gmac_log_index++; \
                                    if (GMAC_LOG_MAX <= gmac_log_index) \
                                    { \
                                        gmac_log_index = 0; \
                                    } \
                                }

uint32_t    gmac_log[GMAC_LOG_MAX];
uint32_t    gmac_log_index = 0;
#endif /* ISSUE_NO33 */

fsp_err_t R_ETHER_Write (ether_ctrl_t * const p_ctrl, void * const p_buffer, uint32_t const frame_length)
{
    fsp_err_t               err             = FSP_SUCCESS;
    ether_instance_ctrl_t * p_instance_ctrl = (ether_instance_ctrl_t *) p_ctrl;
    __attribute__((unused))volatile  R_GMAC_Type *  p_reg_etherc;
    uint8_t *               p_write_buffer;
    uint32_t                write_buffer_size;
    int32_t                 i;
    uint8_t                 link_status = 0;
    ether_frame_user_t  *   p_ether_frame_user;
    ether_frame_mgtag_t *   p_ether_frame_mgtag;
    uint8_t                 write_port;
    uint16_t                mgtag_data = 0;
    uint32_t                mgtag_data2 = 0;
#ifdef RZT2_RSK_PNS
    __attribute__((unused))ether_instance_descriptor_t * p_tx_descriptor_last;
#endif
    /* Check argument */
#if (ETHER_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_ERROR_RETURN(ETHER_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    ETHER_ERROR_RETURN(NULL != p_buffer, FSP_ERR_INVALID_POINTER);
    ETHER_ERROR_RETURN((ETHER_MINIMUM_FRAME_SIZE <= frame_length) && (ETHER_MAXIMUM_FRAME_SIZE >= frame_length),
                       FSP_ERR_INVALID_ARGUMENT);
    if (ETHER_ZEROCOPY_ENABLE == p_instance_ctrl->p_ether_cfg->zerocopy)
    {
        ETHER_ERROR_RETURN(0 == ((uint32_t) p_buffer & (uint32_t) ETHER_BUFFER_4BYTE_ALIGNMENT_MASK),
                           FSP_ERR_INVALID_POINTER);
    }
#endif

    /* Get an established port by bits */
    for (i=ETHER_NUM_PORT-1; i>=0; i--)
    {
        link_status = link_status << 1;
        if (ETHER_LINK_ESTABLISH_STATUS_UP == p_instance_ctrl->link_establish_status[i])
        {
            link_status |= 1;
        }
    }

    if (0 != p_instance_ctrl->write_port)
    {
        /* If a port is specified, it is error if all of the specified ports are not established. */
        write_port = p_instance_ctrl->write_port;
        ETHER_ERROR_RETURN(write_port == (write_port & link_status), FSP_ERR_INVALID_ARGUMENT);
    }
    else
    {
        /* If no port is specified, it is error if one or more ports are not established. */
        ETHER_ERROR_RETURN(0 != link_status, FSP_ERR_INVALID_ARGUMENT);
    }

    /* In case of detection mode of magic packet, return error. */
    ETHER_ERROR_RETURN(0 == ether_check_magic_packet_detection_bit(p_instance_ctrl),
                       FSP_ERR_ETHER_ERROR_MAGIC_PACKET_MODE);

    /* Retrieve the transmit buffer location controlled by the  descriptor. */
    err = ether_buffer_get(p_instance_ctrl, (void **) &p_write_buffer, &write_buffer_size);
    ETHER_ERROR_RETURN(FSP_SUCCESS == err, FSP_ERR_ETHER_ERROR_TRANSMIT_BUFFER_FULL);

    if (FSP_SUCCESS == err)
    {
        /* Copy address from user buffer to transmit buffer */
        p_ether_frame_user  = (ether_frame_user_t  *)p_buffer;
        p_ether_frame_mgtag = (ether_frame_mgtag_t *)p_write_buffer;

        memcpy(p_ether_frame_mgtag->dst_addr,
                p_ether_frame_user->dst_addr,
                sizeof(p_ether_frame_mgtag->dst_addr));
        memcpy(p_ether_frame_mgtag->src_addr,
                p_ether_frame_user->src_addr,
                sizeof(p_ether_frame_mgtag->src_addr));

        if (0 != p_instance_ctrl->write_port)
        {
            /* Forced Forwarding */
            mgtag_data  |= TX_MGTAG_DATA_FORWARD_FORCE;
            mgtag_data2 |= ((p_instance_ctrl->write_port << TX_MGTAG_DATA2_PORT_SHIFT) & TX_MGTAG_DATA2_PORT_MASK);
        }

        #if ETHER_SWITCH_CONFIG_PTP
        {
            if (ETHER_SWITCH_TX_FUNC_TIMESTAMP == (p_instance_ctrl->tx_manage.flg_func & ETHER_SWITCH_TX_FUNC_TIMESTAMP))
            {
               mgtag_data  |= TX_MGTAG_DATA_TIMESTAMP;
               mgtag_data2 |= ((p_instance_ctrl->tx_manage.timestamp_id << TX_MGTAG_DATA2_TSID_SHIFT) & TX_MGTAG_DATA2_TSID_MASK);
            }
#if 0
            if (ETHER_SWITCH_TX_FUNC_ONESTEP == (p_instance_ctrl->tx_manage.flg_func &  ETHER_SWITCH_TX_FUNC_ONESTEP))
            {
                mgtag_data  |= TX_MGTAG_DATA_ONE_STEP;
            }

            if (ETHER_SWITCH_TX_FUNC_QUE_NUM == (p_instance_ctrl->tx_manage.flg_func &  ETHER_SWITCH_TX_FUNC_QUE_NUM))
            {
                mgtag_data  |= TX_MGTAG_DATA_QUE_NUM;
                mgtag_data  |= ((p_instance_ctrl->tx_manage.que_num << TX_MGTAG_DATA_QUE_NUM_SHIFT) & TX_MGTAG_DATA_QUE_NUM_MASK);
            }
#endif
            if (ETHER_SWITCH_TX_FUNC_TIM_NUM == (p_instance_ctrl->tx_manage.flg_func &  ETHER_SWITCH_TX_FUNC_TIM_NUM))
            {
                mgtag_data  |= TX_MGTAG_DATA_TIM_NUM;    
                mgtag_data  |= ((p_instance_ctrl->tx_manage.tim_num << TX_MGTAG_DATA_TIM_NUM_SHIFT) & TX_MGTAG_DATA_TIM_NUM_MASK);
            }
        }
        #endif /* ETHER_SWITCH_CONFIG_PTP */

        /* Set management-tag to transmit buffer */
        p_ether_frame_mgtag->mgtag.control_tag[0]   = 0xE0;
        p_ether_frame_mgtag->mgtag.control_tag[1]   = 0x01;
        p_ether_frame_mgtag->mgtag.control_data[0]  = (uint8_t)((mgtag_data  >>  8) & 0xff);
        p_ether_frame_mgtag->mgtag.control_data[1]  = (uint8_t)((mgtag_data  >>  0) & 0xff);
        p_ether_frame_mgtag->mgtag.control_data2[0] = (uint8_t)((mgtag_data2 >> 24) & 0xff);
        p_ether_frame_mgtag->mgtag.control_data2[1] = (uint8_t)((mgtag_data2 >> 16) & 0xff);
        p_ether_frame_mgtag->mgtag.control_data2[2] = (uint8_t)((mgtag_data2 >>  8) & 0xff);
        p_ether_frame_mgtag->mgtag.control_data2[3] = (uint8_t)((mgtag_data2 >>  0) & 0xff);

        if (ETHER_ZEROCOPY_DISABLE == p_instance_ctrl->p_ether_cfg->zerocopy)
        {
            if (write_buffer_size < (frame_length + sizeof(p_ether_frame_mgtag->mgtag)))
            {
                err = FSP_ERR_ETHER_ERROR_TRANSMIT_BUFFER_FULL; /* Transmit buffer overflow */
            }
            else
            {
                memcpy(p_ether_frame_mgtag->type,
                       p_ether_frame_user->type,
                       (frame_length-sizeof(p_ether_frame_mgtag->dst_addr)-sizeof(p_ether_frame_mgtag->src_addr)));

                p_instance_ctrl->p_tx_descriptor->des1 = frame_length + sizeof(p_ether_frame_mgtag->mgtag);    // write into next open descriptor
            }
        }
        else   // Zerocopy
        {
            p_instance_ctrl->p_tx_descriptor->des3 = (uint32_t)&p_ether_frame_user->type;
            p_instance_ctrl->p_tx_descriptor->des1 =
                        ((sizeof(p_ether_frame_mgtag->dst_addr)
                        + sizeof(p_ether_frame_mgtag->src_addr)
                        + sizeof(p_ether_frame_mgtag->mgtag)) << ETHER_RDES1_TBS1_SHFIT) & ETHER_RDES1_TBS1_MASK;
            p_instance_ctrl->p_tx_descriptor->des1 |=
                        ((frame_length
                        - sizeof(p_ether_frame_mgtag->dst_addr)
                        - sizeof(p_ether_frame_mgtag->src_addr)) << ETHER_RDES1_TBS2_SHFIT) & ETHER_RDES1_TBS2_MASK;
        }
    }

    if (FSP_SUCCESS == err)
    {
#ifdef  ISSUE_NO33  /* �s��?����� */
        ether_instance_descriptor_t   * p_tx_descriptor = p_instance_ctrl->p_tx_descriptor;
#endif /* ISSUE_NO33 */

        /* Update pointer to the currently referenced transmit descriptor  */
        //if (ETHER_TDES0_TER == (p_instance_ctrl->p_tx_descriptor->des0 & ETHER_TDES0_TER))   // end of descriptor chain reached, turn around
        if (p_instance_ctrl->p_tx_descriptor == (p_instance_ctrl->p_ether_cfg->p_tx_descriptors + (p_instance_ctrl->p_ether_cfg->num_tx_descriptors) - 1))
        {
            p_instance_ctrl->p_tx_descriptor->des0 = (ETHER_TDES0_OWN|ETHER_TDES0_LS|ETHER_TDES0_FS);//|ETHER_TDES0_ICETHER_TDES0_TER);//| ); //ETHER_TDES0_TCH
            p_instance_ctrl->p_tx_descriptor = p_instance_ctrl->p_ether_cfg->p_tx_descriptors;  // back to first in ring
        }
        else
        {
            p_instance_ctrl->p_tx_descriptor->des0 = (ETHER_TDES0_OWN|ETHER_TDES0_LS|ETHER_TDES0_FS);//); // |ETHER_TDES0_IC
            p_instance_ctrl->p_tx_descriptor++;      // Next descriptor
        }

        /* Restart transmit */
        p_reg_etherc = (R_GMAC_Type *) p_instance_ctrl->p_reg_etherc;     

#ifdef  ISSUE_NO33  /* �s��?����� */
        {
            __disable_irq();

            gmac_log[gmac_log_index] = 0xAAAA0001;
            GMAC_LOG_UPDATE

            gmac_log[gmac_log_index] = (uint32_t)p_tx_descriptor;
            GMAC_LOG_UPDATE

            gmac_log[gmac_log_index] = p_reg_etherc->Status;
            GMAC_LOG_UPDATE

            gmac_log[gmac_log_index]  = *((uint16_t*)p_ether_frame_mgtag->type) & 0xffff;
            gmac_log[gmac_log_index] |= *((uint16_t*)&p_ether_frame_mgtag->data[26]) << 16;
            GMAC_LOG_UPDATE

            gmac_log[gmac_log_index] = 0xAAAAAAAA;

            __enable_irq();
        }
#endif /* ISSUE_NO33 */

#ifndef RZT2_RSK_PNS
   //         __asm("dmb");
   //Axel     p_reg_etherc->Transmit_Poll_Demand = 0;    /* */
#endif

#ifdef  ISSUE_NO33  /* �s��?����� */
        {
            uint32_t    des0;

            Delay_usec(2*1000);

            __disable_irq();

            gmac_log[gmac_log_index] = 0xAAAA0002;
            GMAC_LOG_UPDATE

            gmac_log[gmac_log_index] = (uint32_t)p_reg_etherc->Current_Host_Transmit_Descriptor;
            GMAC_LOG_UPDATE

            gmac_log[gmac_log_index] = p_reg_etherc->Status;
            GMAC_LOG_UPDATE

            des0 = p_tx_descriptor->des0;
            gmac_log[gmac_log_index] = des0;
            GMAC_LOG_UPDATE

            gmac_log[gmac_log_index] = 0xAAAAAAAA;
            if (ETHER_TDES0_OWN == (p_tx_descriptor->des0 & ETHER_TDES0_OWN))
            {
                gmac_log[gmac_log_index] = 0xBBBBBBBB;
            }

            __enable_irq();
        }
#endif /* ISSUE_NO33 */

    }
    return err;
}                                      /* End of function R_ETHER_Write() */

/*******************************************************************************************************************//**
 * @} (end addtogroup ETHER)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

#if (ETHER_CFG_PARAM_CHECKING_ENABLE)

/*******************************************************************************************************************//**
 * @brief Parameter error check function for open.
 *
 * @param[in] p_instance_ctrl   Pointer to the control block for the channel
 * @param[in] p_cfg             Pointer to the configuration structure specific to UART mode
 *
 * @retval  FSP_SUCCESS                  No parameter error found
 * @retval  FSP_ERR_ASSERTION            Pointer to ETHER control block or configuration structure is NULL
 * @retval  FSP_ERR_ALREADY_OPEN         Control block has already been opened
 * @retval  FSP_ERR_INVALID_CHANNEL      Invalid channel number is given.
 * @retval  FSP_ERR_INVALID_POINTER      Pointer to MAC address is NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT      Irq number lower then 0.
 **********************************************************************************************************************/
static fsp_err_t ether_open_param_check (ether_instance_ctrl_t const * const p_instance_ctrl,
                                         ether_cfg_t const * const           p_cfg)
{
    FSP_ASSERT(p_instance_ctrl);
    ETHER_ERROR_RETURN((NULL != p_cfg), FSP_ERR_INVALID_POINTER);
    ETHER_ERROR_RETURN((NULL != p_cfg->p_mac_address), FSP_ERR_INVALID_POINTER);
    ETHER_ERROR_RETURN((BSP_FEATURE_ETHER_MAX_CHANNELS > p_cfg->channel), FSP_ERR_INVALID_CHANNEL);

#ifndef BSP_PLAT_RZN1SDB /* RZ/T2 */
    ETHER_ERROR_RETURN((0 <= p_cfg->sbd_irq), FSP_ERR_INVALID_ARGUMENT);
    ETHER_ERROR_RETURN((0 <= p_cfg->pmt_irq), FSP_ERR_INVALID_ARGUMENT);
#endif

    ETHER_ERROR_RETURN((NULL != p_cfg->p_rx_descriptors), FSP_ERR_INVALID_POINTER);
    ETHER_ERROR_RETURN((NULL != p_cfg->p_tx_descriptors), FSP_ERR_INVALID_POINTER);
    ETHER_ERROR_RETURN((NULL != p_cfg->pp_ether_buffers), FSP_ERR_INVALID_POINTER);

    ETHER_ERROR_RETURN((ETHER_OPEN != p_instance_ctrl->open), FSP_ERR_ALREADY_OPEN);

    return FSP_SUCCESS;
}

#endif

/***********************************************************************************************************************
 * Function Name: ether_reset_mac
 * Description  : The EDMAC and EtherC are reset through the software reset.
 * Arguments    : channel -
 *                    ETHERC channel number
 * Return Value : none
 ***********************************************************************************************************************/
RZT2_FUNC_ATTR void ether_reset_mac(volatile R_GMAC_Type * const p_reg)
{
    p_reg->Bus_Mode_b.SWR = 1;

    /* Wait for reset to complete */
    while( p_reg->Bus_Mode_b.SWR==1);

#if 0 //EL: this is the logoc for RA �m�F�v
    /*
     * Waiting time until the initialization of ETHERC and EDMAC is completed is 64 cycles
     * in the clock conversion of an internal bus of EDMAC.
     */
    R_BSP_SoftwareDelay(ETHER_ETHERC_INITIALIZATION_WAIT_CYCLE * BSP_DELAY_UNITS_SECONDS / SystemCoreClock + 1,
                        BSP_DELAY_UNITS_MICROSECONDS);
#endif //EL: this is the logoc for RA

}                                      /* End of function ether_reset_mac() */

/***********************************************************************************************************************
 * Function Name: ether_init_descriptors
 * Description  : The EDMAC descriptors and the driver buffers are initialized.
 * Arguments    : channel -
 *                    ETHERC channel number
 * Return Value : none
 ***********************************************************************************************************************/
RZT2_FUNC_ATTR void ether_init_descriptors (ether_instance_ctrl_t * const p_instance_ctrl)
{
    ether_instance_descriptor_t * p_descriptor = NULL;
    uint32_t i;

    /* Initialize the receive descriptors */
    for (i = 0; i < p_instance_ctrl->p_ether_cfg->num_rx_descriptors; i++)
    {
        p_descriptor       = &p_instance_ctrl->p_ether_cfg->p_rx_descriptors[i];

        p_descriptor->des0 = ETHER_RDES0_OWN;           /* receive status */
        p_descriptor->des1 =                            /* Buffer 1 Byte Counte */
            p_instance_ctrl->p_ether_cfg->ether_buffer_size;
        p_descriptor->des2 =                            /* Buffer 1 Address */
            (uint32_t)p_instance_ctrl->p_ether_cfg->pp_ether_buffers[i];
        p_descriptor->des3 = 0;                         /* Buffer 2 Address */
    }

    if (NULL != p_descriptor)
    {
        /* The last descriptor points back to the start */
        p_descriptor->des1 |= ETHER_RDES1_RER;           /* Receive End of Ring */

        /* Initialize application receive descriptor pointer */
        p_instance_ctrl->p_rx_descriptor = &p_instance_ctrl->p_ether_cfg->p_rx_descriptors[0];
    }

    /* Initialize the transmit descriptors */
    for (i = 0; i < p_instance_ctrl->p_ether_cfg->num_tx_descriptors; i++)
    {
        p_descriptor       = &p_instance_ctrl->p_ether_cfg->p_tx_descriptors[i];

        p_descriptor->des0 = 0;                         /* transmit control/status */
        p_descriptor->des1 = 0;                         /* Buffer 1 Byte Counte */
        p_descriptor->des2 =                            /* Buffer 1 Address */
            (uint32_t)p_instance_ctrl->p_ether_cfg->pp_ether_buffers[p_instance_ctrl->p_ether_cfg->num_rx_descriptors + i];
        p_descriptor->des3 = 0;                         /* Buffer 2 Address */
    }

    if (NULL != p_descriptor)
    {
        /* The last descriptor points back to the start */
        p_descriptor->des0 |= ETHER_TDES0_TER;           /* Transmit End of Ring */

        /* Initialize application transmit descriptor pointer */
        p_instance_ctrl->p_tx_descriptor = &p_instance_ctrl->p_ether_cfg->p_tx_descriptors[0];
    }
}                                      /* End of function ether_init_descriptors() */

/********************************************************************************************************************//**
 * @brief Get Points to the buffer pointer used by the stack.
 * @param[in]  p_instance_ctrl                              Ethernet driver control block.
 * @param[out] p_buffer                                     Pointer to location to store the buffer pointer
 * @param[out] p_buffer_size                                Pointer to location to store the buffer size
 * @retval     FSP_SUCCESS                                  Processing completed successfully.
 * @retval     FSP_ERR_ETHER_ERROR_TRANSMIT_BUFFER_FULL     Transmit buffer is not empty.
 *
 ***********************************************************************************************************************/
RZT2_FUNC_ATTR fsp_err_t ether_buffer_get (ether_instance_ctrl_t * const p_instance_ctrl,
                                   void ** const                 p_buffer,
                                   uint32_t                    * p_buffer_size)
{
    fsp_err_t err = FSP_SUCCESS;

    /* When the Link up processing is completed */
    /* All transmit buffers are full */
    if (ETHER_TDES0_OWN == (p_instance_ctrl->p_tx_descriptor->des0 & ETHER_TDES0_OWN))
    {
        err = FSP_ERR_ETHER_ERROR_TRANSMIT_BUFFER_FULL;
    }
    else
    {
        /* Give application another buffer to work with */
        (*p_buffer)      = (void *)p_instance_ctrl->p_tx_descriptor->des2;
        (*p_buffer_size) = p_instance_ctrl->p_ether_cfg->ether_buffer_size;
        err              = FSP_SUCCESS;
    }

    return err;
}                                      /* End of function ether_buffer_get() */

/***********************************************************************************************************************
 * Function Name: ether_config_ethernet
 * Description  : Configure the Ethernet Controller (EtherC) and the Ethernet
 *                Direct Memory Access controller (EDMAC).
 * Arguments    : channel -
 *                    ETHERC channel number
 *                mode -
 *                   The operational mode is specified.
 *                   NO_USE_MAGIC_PACKET_DETECT (0) - Communicate mode usually
 *                   USE_MAGIC_PACKET_DETECT    (1) - Magic packet detection mode
 * Return Value : none
 ***********************************************************************************************************************/

RZT2_FUNC_ATTR void ether_config_ethernet (ether_instance_ctrl_t const * const p_instance_ctrl, const uint8_t mode)
{
    volatile R_GMAC_Type *  p_reg_etherc;

#if (ETHER_CFG_PARAM_CHECKING_ENABLE)
    /* Check argument */
    if ((NULL == p_instance_ctrl) || (ETHER_OPEN != p_instance_ctrl->open))
    {
        return;
    }
#endif

    p_reg_etherc = (R_GMAC_Type *) p_instance_ctrl->p_reg_etherc;

    /* Initialize Receive Descriptor List Address */
    p_reg_etherc->Receive_Descriptor_List_Address_b.RDESLA_32bit =
                (uint32_t) p_instance_ctrl->p_rx_descriptor >> 2;

    /* Initialize Transmit Descriptor List Address */
    p_reg_etherc->Transmit_Descriptor_List_Address_b.TDESLA_32bit =
                (uint32_t) p_instance_ctrl->p_tx_descriptor >> 2;

    /* Initialize MAC Frame Filter */
    p_reg_etherc->MAC_Frame_Filter_b.PR   =       /* promiscuous mode */
                p_instance_ctrl->p_ether_cfg->promiscuous;

    /* Initialize MMC Receive/Transmit Interrupt Mask */
    p_reg_etherc->MMC_Receive_Interrupt_Mask  = ETHER_MMC_RECEIVE_INTERRUPT_MASK_ALL_BIT;
    p_reg_etherc->MMC_Transmit_Interrupt_Mask = ETHER_MMC_TRANSMIT_INTERRUPT_MASK_ALL_BIT;

        /* Initialize MAC Configuration */
#ifdef RZT2_FPGA
    p_reg_etherc->MAC_Configuration_b.PS  = 1;    /* Port Selec: 100/10Mbps */
#else /* !RZT2_FPGA */
    p_reg_etherc->MAC_Configuration_b.PS  = 0;    /* Port Selec: 1Gbps */
#endif /* !RZT2_FPGA */
    p_reg_etherc->MAC_Configuration_b.FES = 1;    /* Speed: 100Mbps */
    p_reg_etherc->MAC_Configuration_b.DM  = 1;    /* Duplex Mode: Full-duple */

    /* Magic packet detection mode */
    if (ETHER_USE_MAGIC_PACKET_DETECT == mode)
    {
        p_reg_etherc->Interrupt_Mask_b.PMTIM      = 0;    /* PMT Interrupt Enable */
    }
    /* Normal mode */
    else
    {
        p_reg_etherc->Interrupt_Mask_b.PMTIM      = 1;    /* PMT Interrupt Disable */

        /* Interrupt Enable Register */
        p_reg_etherc->Interrupt_Enable =
                        ETHER_INTERRUPT_ENABLE_NIE |    /* Normal Interrupt Summary Enable */
                        ETHER_INTERRUPT_ENABLE_AIE |    /* Abnormal Interrupt Summary Enable */
                        ETHER_INTERRUPT_ENABLE_ERE |    /* Early Receive Interrupt Enable */
                        ETHER_INTERRUPT_ENABLE_FBE |    /* Fatal Bus Error Enable */
#ifndef RZT2_FPGA
                        ETHER_INTERRUPT_ENABLE_ETE |    /* Early Transmit Interrupt Enable */
#endif /* RZT2_FPGA */
                        ETHER_INTERRUPT_ENABLE_RWE |    /* Receive Watchdog Timeout Enable */
                        ETHER_INTERRUPT_ENABLE_RUE |    /* Receive Buffer Unavailable Enable */
                        ETHER_INTERRUPT_ENABLE_RIE |    /* Receive Interrupt Enable */
                        ETHER_INTERRUPT_ENABLE_UNE |    /* Underflow Interrupt Enable */
                        ETHER_INTERRUPT_ENABLE_OVE |    /* Overflow Interrupt Enable */
                        ETHER_INTERRUPT_ENABLE_TIE;     /* Transmit Interrupt Enable */
    }
    
            /* Initialize Operation Mode */
        p_reg_etherc->Operation_Mode_b.RSF   = 1;     /* Receive Store and Forward */
        p_reg_etherc->Operation_Mode_b.SR    = 1;     /* Start or Stop Receive */
        p_reg_etherc->Operation_Mode_b.TSF   = 1;     /* TX Frame only when completely arrived in TX Fifo */
        p_reg_etherc->Operation_Mode_b.ST    = 1;
           // p_reg_etherc->Bus_Mode    = 0x80100800;      // Set Bus mode to RX Burst 8, TX Burst 8 
           // p_reg_etherc->Bus_Mode    = 0x8810c800;      // Set Bus mode to TX-Prio, Prio Ratio 4:1, RX Burst 8, TX Burst 8 
           // p_reg_etherc->Bus_Mode    = 0x80100100;      // Set Bus mode to RX Burst 8, TX Burst 1 
           p_reg_etherc->AXI_Bus_Mode_b.BLEN4 = 0; 
           p_reg_etherc->AXI_Bus_Mode_b.BLEN8 = 1; 
            p_reg_etherc->AXI_Bus_Mode_b.BLEN16 = 0; 
          //  p_reg_etherc->AXI_Bus_Mode_b.RD_OSR_LMT = 3; 
          //  p_reg_etherc->AXI_Bus_Mode_b.WR_OSR_LMT = 3; 
            
}                                      /* End of function ether_config_ethernet() */

/***********************************************************************************************************************
 * Function Name: ether_pause_resolution
 * Description  : Determines PAUSE frame generation and handling. Uses
 *                the resolution Table 28B-3 of IEEE 802.3-2008.
 * Arguments    : local_ability -
 *                    local PAUSE capability (2 least significant bits)
 *                partner_ability -
 *                    link partner PAUSE capability (2 least significant bits)
 *                *ptx_pause -
 *                    pointer to location to store the result of the table lookup for transmit
 *                    PAUSE. 1 is enable, 0 is disable.
 *                *prx_pause -
 *                    pointer to location to store the result of the table lookup for receive
 *                    PAUSE. 1 is enable, 0 is disable.
 * Return Value : none
 ***********************************************************************************************************************/
RZT2_FUNC_ATTR void ether_pause_resolution (uint32_t const local_ability,
                                    uint32_t const partner_ability,
                                    uint32_t     * ptx_pause,
                                    uint32_t     * prx_pause)
{
    uint32_t i;
    uint32_t ability_compare;

    /*
     * Arrange the bits so that they correspond to the Table 28B-3
     * of the IEEE 802.3 values.
     */
    ability_compare =
        (uint32_t) (((local_ability & ETHER_LINK_RESOLUTION_ABILITY_MASK) <<
                     ETHER_LINK_RESOLUTION_LOCAL_ABILITY_BITSHIFT) |
                    (partner_ability & ETHER_LINK_RESOLUTION_ABILITY_MASK));

    /* Walk through the look up table */
    for (i = 0; i < ETHER_PAUSE_TABLE_ENTRIES; i++)
    {
        if ((ability_compare & pause_resolution[i].mask) == pause_resolution[i].value)
        {
            (*ptx_pause) = pause_resolution[i].transmit;
            (*prx_pause) = pause_resolution[i].receive;

            return;
        }
    }
}                                      /* End of function ether_pause_resolution() */

/***********************************************************************************************************************
 * Function Name: ether_configure_mac
 * Description  : Software reset is executed, and ETHERC and EDMAC are configured.
 * Arguments    : channel -
 *                    ETHERC channel number
 *                mac_addr -
 *                    The MAC address of ETHERC
 *                mode -
 *                    The operational mode is specified.
 *                    NO_USE_MAGIC_PACKET_DETECT (0) - Communicate mode usually
 *                    USE_MAGIC_PACKET_DETECT    (1) - Magic packet detection mode
 * Return Value : none
 ***********************************************************************************************************************/
RZT2_FUNC_ATTR void ether_configure_mac (ether_instance_ctrl_t * const p_instance_ctrl,
                                 const uint8_t                 mac_addr[],
                                 const uint8_t                 mode)
{
    volatile R_GMAC_Type *  p_reg_etherc;
    uint32_t                mac_h;
    uint32_t                mac_l;

#if (ETHER_CFG_PARAM_CHECKING_ENABLE)

    /* Check argument */
    if ((NULL == p_instance_ctrl) || (ETHER_OPEN != p_instance_ctrl->open))
    {
        return;
    }
#endif

    p_reg_etherc = (R_GMAC_Type *) p_instance_ctrl->p_reg_etherc;

    /* Disable receive and transmit. */
    ether_disable (p_instance_ctrl);

    /* Set MAC address */
    mac_h = ((uint32_t) mac_addr[5] << 8) |
            ((uint32_t) mac_addr[4] << 0);

    mac_l = ((uint32_t) mac_addr[3] << 24) |
            ((uint32_t) mac_addr[2] << 16) |
            ((uint32_t) mac_addr[1] <<  8) |
            ((uint32_t) mac_addr[0] <<  0);

    p_reg_etherc->MAR0_H_b.ADDRHI = mac_h;
    p_reg_etherc->MAR0_L_b.ADDRLO  = mac_l;

    /* Initialize receive and transmit descriptors */
    ether_init_descriptors(p_instance_ctrl);

    /* Perform reset of hardware interface configuration */

    ether_config_ethernet(p_instance_ctrl, mode);
}                                      /* End of function ether_configure_mac() */

/***********************************************************************************************************************
 * @brief   Disable receive and transmit
 * @param[in] p_instance_ctrl           Pointer to the control block for the channel
 * Return Value : none
 ***********************************************************************************************************************/
RZT2_FUNC_ATTR void ether_disable (ether_instance_ctrl_t * const p_instance_ctrl)
{
    volatile R_GMAC_Type *  p_reg_etherc;

    p_reg_etherc = (R_GMAC_Type *) p_instance_ctrl->p_reg_etherc;

    p_reg_etherc->Operation_Mode_b.ST    = 0;
    while (p_reg_etherc->Debug_b.TRCSTS != 0);
    p_reg_etherc->MAC_Configuration_b.TE = 0;
    p_reg_etherc->MAC_Configuration_b.RE = 0;
    while (p_reg_etherc->Debug_b.RRCSTS != 0);
    p_reg_etherc->Operation_Mode_b.SR    = 0;
}

/***********************************************************************************************************************
 * @brief Determines the partner PHY capability through auto-negotiation process. The link abilities
 *        are handled to determine duplex, speed and flow control (PAUSE frames).
 *
 * @param[in] p_instance_ctrl           Pointer to the control block for the channel
 * @param[in] mode                      The operational mode is specified.
 *                                      NO_USE_MAGIC_PACKET_DETECT (0) - Communicate mode usually
 *                                      USE_MAGIC_PACKET_DETECT    (1) - Magic packet detection mode
 * @retval  FSP_SUCCESS                 Processing completed successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block or configuration structure is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_ETHER_ERROR_LINK    Auto-negotiation of PHY-LSI is not completed
 *                                      or result of Auto-negotiation is abnormal.
 *
 ***********************************************************************************************************************/
RZT2_FUNC_ATTR fsp_err_t ether_do_link (ether_instance_ctrl_t * const p_instance_ctrl, const uint32_t port)
{
    fsp_err_t               err = FSP_SUCCESS;
    volatile R_GMAC_Type *  p_reg_etherc;

    uint32_t        local_pause_bits   = 0;
    uint32_t        partner_pause_bits = 0;
    uint32_t        transmit_pause_set = 0;
    uint32_t        receive_pause_set  = 0;
    uint32_t        full_duplex        = 0;
    uint32_t        link_speed_duplex  = 0;
    uint32_t        flow_control;
    uint32_t        i;
    ether_link_establish_status_t   link_establish_status_or;
    ether_switch_link_speed_t       speed;

#if (ETHER_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_ERROR_RETURN(ETHER_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    link_establish_status_or = ETHER_LINK_ESTABLISH_STATUS_DOWN;
    for (i=0; i<ETHER_NUM_PORT; i++)
    {
        if(i != port)
        {
            link_establish_status_or |= p_instance_ctrl->link_establish_status[i];
        }
    }

    p_reg_etherc = (R_GMAC_Type *) p_instance_ctrl->p_reg_etherc;

    /* Get the link status */
    err = p_instance_ctrl->p_ether_cfg->p_ether_phy_instance->p_api->linkPartnerAbilityGet(
                        (p_instance_ctrl->p_ether_cfg->p_ether_phy_instance + port)->p_ctrl,
                        &link_speed_duplex,
                        &local_pause_bits,
                        &partner_pause_bits);

    if (FSP_SUCCESS == err)
    {
        switch (link_speed_duplex)
        {
            /* Half duplex link */
            case ETHER_PHY_LINK_SPEED_1000H:
                speed = ETHER_SWITCH_LINK_SPEED_1000H;
                break;

            case ETHER_PHY_LINK_SPEED_100H:
                speed = ETHER_SWITCH_LINK_SPEED_100H;
                break;

            case ETHER_PHY_LINK_SPEED_10H:
                speed = ETHER_SWITCH_LINK_SPEED_10H;
                break;

            /* Full duplex link */
            case ETHER_PHY_LINK_SPEED_1000F:
                speed = ETHER_SWITCH_LINK_SPEED_1000F;
                break;

            case ETHER_PHY_LINK_SPEED_100F:
                speed = ETHER_SWITCH_LINK_SPEED_100F;
                break;

            case ETHER_PHY_LINK_SPEED_10F:
                speed = ETHER_SWITCH_LINK_SPEED_10F;
                break;

            default:
                err = FSP_ERR_ETHER_ERROR_LINK;
                break;
        }
    }

    if (FSP_SUCCESS == err)
    {
        err = R_ETHER_ACCESS_Set_convctrl (p_instance_ctrl->p_reg_access, port, link_speed_duplex);

    }

    if (FSP_SUCCESS == err)
    {
        err = p_instance_ctrl->p_ether_cfg->p_ether_switch_instance->p_api->confSpeed(
                    p_instance_ctrl->p_ether_cfg->p_ether_switch_instance->p_ctrl,
                    (ether_switch_port_t)port,
                    speed);

        if (FSP_SUCCESS == err)
        {
            /* This port is 1-st establish */
            if(ETHER_LINK_ESTABLISH_STATUS_UP != link_establish_status_or)
            {
                /* When pause frame is used */
                if ((full_duplex) && (ETHER_FLOW_CONTROL_ENABLE == p_instance_ctrl->p_ether_cfg->flow_control))
                {
                    /* pause frame can be used when  when passing through the switch */
                    /* NOTE: not confirmed this operation !!! */

                    flow_control = ETHER_FLOW_CONTROL_PT |      /* Pause Time (b31-b16) */
                                   ETHER_FLOW_CONTROL_PLT|      /* Pause Low Threshold */
                                   ETHER_FLOW_CONTROL_FCA_BPA;  /* Flow Control Busy or Backpressure Activate */
                    /**
                     * Enable PAUSE for full duplex link depending on
                     * the pause resolution results
                     */
                    ether_pause_resolution(local_pause_bits,
                                           partner_pause_bits,
                                           &transmit_pause_set,
                                           &receive_pause_set);

                    if (ETHER_PAUSE_XMIT_ON == transmit_pause_set)
                    {
                        /* Enable automatic PAUSE frame transmission */
                        flow_control |= ETHER_FLOW_CONTROL_TFE; /* Transmit Flow Control Enable */
                    }

                    if (ETHER_PAUSE_RECV_ON == receive_pause_set)
                    {
                        /* Enable reception of PAUSE frames */
                        flow_control |= ETHER_FLOW_CONTROL_RFE; /* Receive Flow Control Enable */
                    }

                    p_reg_etherc->Flow_Control = flow_control;
                }
                /* When pause frame is not used */
                else
                {
                    /* It did the software reset, so no operation */
                }

                /* Initialize Operation Mode */
                p_reg_etherc->Operation_Mode_b.RSF   = 1;     /* Receive Store and Forward */
                p_reg_etherc->Operation_Mode_b.SR    = 1;     /* Start or Stop Receive */
                p_reg_etherc->Operation_Mode_b.TSF   = 1;
                p_reg_etherc->Operation_Mode_b.ST    = 1;

                /* Enable receive and transmit. */
                p_reg_etherc->MAC_Configuration_b.RE = 1;
                p_reg_etherc->MAC_Configuration_b.TE = 1;
            }
        }
    }
    else
    {
        err = FSP_ERR_ETHER_ERROR_LINK;
    }

    return err;
}                                      /* End of function ether_do_link() */


/***********************************************************************************************************************
 * @brief The magic packet detection is permitted.
 *
 * @param[in] p_instance_ctrl           Pointer to the control block for the channel
 * @retval  non
 *
 ***********************************************************************************************************************/
void ether_magic_packet_detection (ether_instance_ctrl_t * const p_instance_ctrl)
{
    volatile R_GMAC_Type *  p_reg_etherc = (R_GMAC_Type *) p_instance_ctrl->p_reg_etherc;

    /* The magic packet detection is permitted. */
    p_reg_etherc->PMT_Control_Status   =
                ETHER_PMT_CONTROL_STATUS_MGKPKTEN |   /* Magic Packet Enable */
                ETHER_PMT_CONTROL_STATUS_PWRDWN;      /* Power Down */

    /* Because data is not transmitted for the magic packet detection waiting,
    * only the reception is permitted. */
    p_reg_etherc->MAC_Configuration_b.RE = 1;

}

/***********************************************************************************************************************
 * Function Name: ether_check_magic_packet_detection_bit
 * Description  :
 * Arguments    : ether_instance_ctrl_t const * const p_instance_ctrl
 * Return Value : 1: Magic Packet detection is enabled.
 *                0: Magic Packet detection is disabled.
 ***********************************************************************************************************************/
uint8_t ether_check_magic_packet_detection_bit (ether_instance_ctrl_t const * const p_instance_ctrl)
{
    uint8_t          ret          = 0;

    volatile R_GMAC_Type *  p_reg_etherc = (R_GMAC_Type *) p_instance_ctrl->p_reg_etherc;

    if (1 == p_reg_etherc->PMT_Control_Status_b.MGKPKTEN)
    {
        ret = 1;
    }
    else
    {
        ret = 0;
    }

    return ret;
}                                      /* End of function ether_check_magic_packet_detection_bit() */

/*******************************************************************************************************************//**
 * @brief Verifies the Etherent link is up or not.
 *
 * @param[in] p_instance_ctrl   Pointer to the control block for the channel
 *
 * @retval  FSP_SUCCESS:                  Link is up
 * @retval  FSP_ERR_ETHER_ERROR_LINK:     Link is down
 * @retval  FSP_ERR_ETHER_PHY_ERROR_LINK  Initialization of PHY-LSI failed.
 **********************************************************************************************************************/
RZT2_FUNC_ATTR fsp_err_t ether_link_status_check (ether_instance_ctrl_t const * const p_instance_ctrl, uint32_t port)
{
    fsp_err_t err = FSP_SUCCESS;
    fsp_err_t link_status;

    link_status = p_instance_ctrl->p_ether_cfg->p_ether_phy_instance->p_api->linkStatusGet(
        (p_instance_ctrl->p_ether_cfg->p_ether_phy_instance + port)->p_ctrl);

    if (FSP_ERR_ETHER_PHY_ERROR_LINK == link_status)
    {
        /* Link is down */
        err = FSP_ERR_ETHER_ERROR_LINK;
    }
    else
    {
        /* Link is up */
        err = FSP_SUCCESS;
    }

    return err;
}                                      /* End of function ether_link_status_check() */


/*******************************************************************************************************************//**
 * @brief Callback function for link status change from switch
 *
 * @param[in] p_arg     Pointer to the callback parameter
 *
 * @retval  non
 **********************************************************************************************************************/
void ether_callback_switch (ether_switch_callback_args_t * const p_arg)
{
    ether_instance_ctrl_t * p_instance_ctrl = (ether_instance_ctrl_t *) p_arg->p_context;

    p_instance_ctrl->status_link = p_arg->status_link;

#ifdef  ISSUE_NO33  /* �s��?����� */
    {
        gmac_log[gmac_log_index] = 0xAAAA0004;
        GMAC_LOG_UPDATE

        gmac_log[gmac_log_index] = p_arg->status_link;
        GMAC_LOG_UPDATE

        gmac_log[gmac_log_index] = 0xAAAAAAAA;
    }
#endif /* ISSUE_NO33 */

}                                      /* End of function ether_callback_switch() */


/***********************************************************************************************************************
 * Function Name: ether_isr_sbd
 * Description  : Interrupt handler for Ethernet receive and transmit interrupts.
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
/* #define ISSUE_MMC */

#ifdef ISSUE_MMC
uint32_t    gmac_statuc = 0;
uint32_t    gmac_interrupt_status = 0;
uint32_t    gmac_mmc_receive_interrupt = 0;
uint32_t    gmac_mmc_transmit_interrupt = 0;
#endif /* ISSUE_MMC */

#ifdef BSP_PLAT_RZN1SDB /* RZ/N1S */
    static void ether_isr_sbd (IRQn_Type *irq_num_ptr)
#else /* !BSP_PLAT_RZN1SDB (RZ/T2) */
  #ifdef RZT2_RSK
    void ether_isr_sbd (void)
  #else /* !RZT2_RSK */
    void ether_isr_sbd (uint32_t int_sense)
  #endif
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */

{
    ether_callback_args_t   callback_arg;
    volatile R_GMAC_Type *  p_reg_etherc;
    uint32_t                status;
    volatile uint32_t       dammy_read;

#ifdef BSP_PLAT_RZN1SDB /* RZ/N1S */
    ether_instance_ctrl_t * p_instance_ctrl = (ether_instance_ctrl_t *) R_FSP_IsrContextGet(*irq_num_ptr);
#else /* !BSP_PLAT_RZN1SDB (RZ/T2) */
    IRQn_Type               irq             = R_FSP_CurrentIrqGet();
    ether_instance_ctrl_t * p_instance_ctrl = (ether_instance_ctrl_t *) R_FSP_IsrContextGet(irq);
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */


    p_reg_etherc = (R_GMAC_Type *) p_instance_ctrl->p_reg_etherc;

    status = p_reg_etherc->Status;
    p_reg_etherc->Status = status;

    /* Dammy read for Countermeasure when the CPU clock is 400 or 800MHz */
    dammy_read = p_reg_etherc->Status;
    dammy_read = dammy_read;

#ifdef  ISSUE_NO33  /* �s��?����� */
    {
        gmac_log[gmac_log_index] = 0xAAAA0003;
        GMAC_LOG_UPDATE

        gmac_log[gmac_log_index] = (uint32_t)p_reg_etherc->Current_Host_Transmit_Descriptor;
        GMAC_LOG_UPDATE

        gmac_log[gmac_log_index] = status;
        GMAC_LOG_UPDATE

        gmac_log[gmac_log_index] = 0xAAAAAAAA;
    }
#endif /* ISSUE_NO33 */

#ifdef ISSUE_MMC
    {
        if((1 << 27) & status)
        {
            /* MMC Interrupt */
            gmac_statuc                 = status;
            gmac_interrupt_status       = p_reg_etherc->Interrupt_Status;
            gmac_mmc_receive_interrupt  = p_reg_etherc->MMC_Receive_Interrupt;
            gmac_mmc_transmit_interrupt = p_reg_etherc->MMC_Transmit_Interrupt;
        }
    }
#endif /* ISSUE_MMC */

    /* Callback : Interrupt handler */
    if (NULL != p_instance_ctrl->p_ether_cfg->p_callback)
    {
        callback_arg.channel      = p_instance_ctrl->p_ether_cfg->channel;
        callback_arg.event        = ETHER_EVENT_SBD_INTERRUPT;
        callback_arg.status_ether = status;
        callback_arg.status_link  = 0;
        callback_arg.p_context    = p_instance_ctrl->p_ether_cfg->p_context;

        (*p_instance_ctrl->p_ether_cfg->p_callback)((void *) &callback_arg);
    }

#ifndef BSP_PLAT_RZN1SDB /* RZ/T2 */
  #ifndef RZT2_RSK
    R_BSP_IrqStatusClear(R_FSP_CurrentIrqGet());
  #endif /* !RZT2_RSK */
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */
}                                      /* End of function ether_isr_sbd() */

/***********************************************************************************************************************
 * Function Name: ether_isr_pmt
 * Description  : Interrupt handler for Ethernet receive and transmit interrupts.
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
#ifdef BSP_PLAT_RZN1SDB /* RZ/N1S */
    static void ether_isr_pmt (IRQn_Type *irq_num_ptr)
#else /* !BSP_PLAT_RZN1SDB (RZ/T2) */
  #ifdef RZT2_RSK
    void ether_isr_pmt (void)
  #else /* !RZT2_RSK */
    void ether_isr_pmt (uint32_t int_sense)
  #endif
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */

{
    ether_callback_args_t   callback_arg;
    volatile R_GMAC_Type *  p_reg_etherc;
    uint32_t                pmt_control_status;

#ifdef BSP_PLAT_RZN1SDB /* RZ/N1S */
    ether_instance_ctrl_t * p_instance_ctrl = (ether_instance_ctrl_t *) R_FSP_IsrContextGet(*irq_num_ptr);
#else /* !BSP_PLAT_RZN1SDB (RZ/T2) */
    IRQn_Type               irq             = R_FSP_CurrentIrqGet();
    ether_instance_ctrl_t * p_instance_ctrl = (ether_instance_ctrl_t *) R_FSP_IsrContextGet(irq);
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */

    p_reg_etherc = (R_GMAC_Type *) p_instance_ctrl->p_reg_etherc;

    pmt_control_status = p_reg_etherc->PMT_Control_Status;

    /* Callback : Interrupt handler */
    if (NULL != p_instance_ctrl->p_ether_cfg->p_callback)
    {
        callback_arg.channel      = p_instance_ctrl->p_ether_cfg->channel;
        callback_arg.event        = ETHER_EVENT_PMT_INTERRUPT;
        callback_arg.status_ether = pmt_control_status;
        callback_arg.status_link  = 0;
        callback_arg.p_context    = p_instance_ctrl->p_ether_cfg->p_context;

        (*p_instance_ctrl->p_ether_cfg->p_callback)((void *) &callback_arg);
    }

    if( (ETHER_PMT_CONTROL_STATUS_MGKPRCVD == (ETHER_PMT_CONTROL_STATUS_MGKPRCVD & pmt_control_status)) ||
        (ETHER_PMT_CONTROL_STATUS_PWRDWN != (ETHER_PMT_CONTROL_STATUS_PWRDWN & pmt_control_status)) )
    {
        p_instance_ctrl->magic_packet = ETHER_MAGIC_PACKET_DETECTED;
    }
    else
    {
        p_reg_etherc->PMT_Control_Status =
                ETHER_PMT_CONTROL_STATUS_MGKPKTEN |   /* Magic Packet Enable */
                ETHER_PMT_CONTROL_STATUS_PWRDWN;      /* Power Down */
    }
#ifndef BSP_PLAT_RZN1SDB /* RZ/T2 */
  #ifndef RZT2_RSK
    R_BSP_IrqStatusClear(R_FSP_CurrentIrqGet());
  #endif /* !RZT2_RSK */
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */
}                                      /* End of function ether_isr_pmt() */

/***********************************************************************************************************************
 * Function Name: ether_enable_icu
 * Description  :
 * Arguments    : channel -
 *                    Ethernet channel number
 * Return Value : none
 ***********************************************************************************************************************/
static void ether_enable_icu (ether_instance_ctrl_t * const p_instance_ctrl)
{

    /** Configure the Ethernet interrupt. */
    R_BSP_IrqCfgEnable(p_instance_ctrl->p_ether_cfg->sbd_irq,
                       p_instance_ctrl->p_ether_cfg->sbd_interrupt_priority,
                       p_instance_ctrl);

    R_BSP_IrqCfgEnable(p_instance_ctrl->p_ether_cfg->pmt_irq,
                       p_instance_ctrl->p_ether_cfg->pmt_interrupt_priority,
                       p_instance_ctrl);
}                                      /* End of function ether_enable_icu() */

/***********************************************************************************************************************
 * Function Name: ether_disable_icu
 * Description  :
 * Arguments    : channel -
 *                    Ethernet channel number
 * Return Value : none
 ***********************************************************************************************************************/
static void ether_disable_icu (ether_instance_ctrl_t * const p_instance_ctrl)
{
#ifndef BSP_PLAT_RZN1SDB /* RZ/T2 */
    /* Get IRQ number for EDMAC_EINT interrupt. */
    R_BSP_IrqDisable(p_instance_ctrl->p_ether_cfg->sbd_irq);
    R_FSP_IsrContextSet(p_instance_ctrl->p_ether_cfg->sbd_irq, NULL);

    R_BSP_IrqDisable(p_instance_ctrl->p_ether_cfg->pmt_irq);
    R_FSP_IsrContextSet(p_instance_ctrl->p_ether_cfg->pmt_irq, NULL);
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */

}                                      /* End of function ether_disable_icu() */


/***********************************************************************************************************************
 * Function Name: Convert bit number to value
 * Description  :
 * Arguments    : in_bit    Value to convert
 * Return Value : converted value
 ***********************************************************************************************************************/
uint32_t ether_convert_port_bit_to_num( uint32_t in_bits )
{
    uint32_t    port;

    switch (in_bits)
    {
        case 1:
            port=0;
            break;
        case 2:
            port=1;
            break;
        case 4:
            port=2;
            break;
        default:
            port=-1;
            break;
    }
    return  port;
}                                      /* End of function ether_convert_port_bit_to_num() */


#ifdef BSP_PLAT_RZN1SDB /* RZ/N1S */
/*
 * RZ/N1�p�_�~�[��?�
 */
#define MAX_IRQ     256
void * gp_context[MAX_IRQ] = {0};

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

static int RegisterInterrupt (IRQn_Type irq, uint32_t priority, void *isr)
{
    uint32_t int_priority = priority;

    /* Register interrupts */
    if (R_BSP_InterruptRegisterCallback(irq, (bsp_int_cb_t) isr)
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
