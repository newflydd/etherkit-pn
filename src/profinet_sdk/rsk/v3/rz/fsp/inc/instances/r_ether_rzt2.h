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
 * @addtogroup ETHER
 * @{
 **********************************************************************************************************************/

#ifndef R_ETHER_H
#define R_ETHER_H

#ifndef BSP_PLAT_RZN1SDB /* RZ/T2 */
#include "bsp_api.h"
   
//#define  ETHER_CONFIG_PTP
#endif /* !BSP_PLAT_RZN1SDB */

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_ether_cfg.h"
#include "eddp_cfg.h"
#include "r_ether_api_rzt2.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define ETHER_CODE_VERSION_MAJOR    (0U)
#define ETHER_CODE_VERSION_MINOR    (1U)

/* the number of port */
#define ETHER_NUM_PORT              (3U)
#ifndef RZT2_RSK_PNS
#define RZT2_FUNC_ATTR	static
#else
#define RZT2_FUNC_ATTR
#endif
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
#define ETHER_MANTAG_CONTROL_DATA1_TIME_MASK    0x10
#define ETHER_MANTAG_CONTROL_DATA1_TIME_SHIFT   4
#define ETHER_MANTAG_CONTROL_DATA1_RED          (1 << 6)

/* PAUSE link mask and shift values */

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

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
//#pragma pack(1)
typedef enum e_ether_previous_link_status
{
    ETHER_PREVIOUS_LINK_STATUS_DOWN = 0, ///< Previous link status is down
    ETHER_PREVIOUS_LINK_STATUS_UP   = 1, ///< Previous link status is up
} ether_previous_link_status_t;

typedef enum e_ether_link_change
{
    ETHER_LINK_CHANGE_NO_CHANGE = 0,   ///< Link status is no change
    ETHER_LINK_CHANGE_LINK_DOWN = 1,   ///< Link status changes to down
    ETHER_LINK_CHANGE_LINK_UP   = 2,   ///< Link status changes to up
} ether_link_change_t;

typedef enum e_ether_magic_packet
{
    ETHER_MAGIC_PACKET_NOT_DETECTED = 0, ///< Magic packet is not detected
    ETHER_MAGIC_PACKET_DETECTED     = 1, ///< Magic packet is detected
} ether_magic_packet_t;

typedef enum e_ether_link_establish_status
{
    ETHER_LINK_ESTABLISH_STATUS_DOWN = 0, ///< Link establish status is down
    ETHER_LINK_ESTABLISH_STATUS_UP   = 1, ///< Link establish status is up
} ether_link_establish_status_t;

typedef enum e_ether_link_status
{
    ETHER_LINK_STATUS_DOWN = 0, ///< Link establish status is down
    ETHER_LINK_STATUS_UP   = 1, ///< Link establish status is up
} ether_link_status_t;

typedef enum e_ether_bool
{
    ETHER_FALSE = 0,
    ETHER_TRUE  = 1,
} ether_bool_t;

//#ifdef  ETHER_CONFIG_PTP
typedef struct st_ether_rx_manage
{
    uint8_t         tim_num;            /// Timer used for timestamping the frame.
    ether_bool_t    red;                /// RED period indication.
    uint32_t        tim_sec;            /// Received timestamp of the frame (nanoseconds).
    uint32_t        tim_nsec;           /// Received timestamp of the frame (seconds).
} ether_rx_manage_t; 

typedef struct st_ether_tx_manage
{
    uint8_t         flg_func;           /// Function flag.
    uint8_t         que_num;            /// Destination queue.
    uint8_t         tim_num;            /// Timer selection.
    uint8_t         timestamp_id;       /// Transmit timestamp ID.
    uint16_t        prp_seq;            /// Sequence number for PRP
} ether_tx_manage_t;

/* Function flag */
#define ETHER_SWITCH_TX_FUNC_NON            0
#define ETHER_SWITCH_TX_FUNC_TIMESTAMP      (1 << 0)
#define ETHER_SWITCH_TX_FUNC_ONESTEP        (1 << 1)
#define ETHER_SWITCH_TX_FUNC_PRP_SUPPRESS   (1 << 2)
#define ETHER_SWITCH_TX_FUNC_PRP_FORCE      (1 << 3)
#define ETHER_SWITCH_TX_FUNC_PRP_SEQUENCE   (1 << 4)
#define ETHER_SWITCH_TX_FUNC_QUE_NUM        (1 << 5)
#define ETHER_SWITCH_TX_FUNC_TIM_NUM        (1 << 6)
//#endif /* ETHER_CONFIG_PTP */

#define ipARP_FRAME_TYPE	( 0x0608U )
#define ipIPv4_FRAME_TYPE	( 0x0008U )


/** ETHER control block. DO NOT INITIALIZE.  Initialization occurs when @ref ether_api_t::open is called. */
typedef struct st_ether_instance_ctrl
{
    uint8_t  read_port;                                  ///< port for R_ETHER_Read
    uint8_t  write_port;                                 ///< port for R_ETHER_Write
    uint8_t  link_proc_port;                             ///< port for R_ETHER_LinkProcess

//#ifdef  ETHER_CONFIG_PTP
    ether_rx_manage_t   rx_manage;                       /// Informaton of management tag when receive
    ether_tx_manage_t   tx_manage;                       /// Informaton of management tag when transmit
//#endif /* ETHER_CONFIG_PTP */

    uint32_t open;                                       ///< Used to determine if the channel is configured

    /* Configuration of ethernet module. */
    ether_cfg_t const * p_ether_cfg;                     ///< Pointer to initial configurations.

    /* Buffer of ethernet module. */
    uint8_t dummy;        // Just used to align the tx_descriptor_red to a word boundary
    ether_instance_descriptor_t * p_rx_descriptor;       ///< Pointer to the currently referenced receive  descriptor(first empty)
    ether_instance_descriptor_t * p_tx_descriptor;       ///< Pointer to the currently referenced transmit descriptor

#ifdef RZT2_RSK_PNS
    ether_instance_descriptor_t * p_tx_descriptor_top;  ///< Profinet: Pointer to the base TX descriptor
    ether_instance_descriptor_t * p_tx_descriptor_base;  ///< Profinet: Pointer to the base TX descriptor
    ether_instance_descriptor_t * p_tx_descriptor_rtc3;
    ether_instance_descriptor_t * p_tx_descriptor_rtcx;
    uint32_t Prev_Host_Transmit_Descriptor;
#endif
    //ether_instance_descriptor_t * p_tx_descriptor_base;  ///< Profinet: Pointer to the base TX descriptor
    //ether_instance_descriptor_t * p_tx_descriptor_red; //[EDDP_CFG_MAX_DEVICES];   ///< Profinet: Pointer to the TX descriptors for the red frames

    /* Interface for PHY-LSI chip. */
    void * p_reg_etherc;                                 ///< Base register of ethernet controller for this channel
    void * p_reg_access;                                 ///< Base register of ethernet access register

    uint32_t status_link;                                ///< link status by callback from switch

    /* Status of ethernet driver. */
    ether_previous_link_status_t  previous_link_status[ETHER_NUM_PORT];  ///< Previous link status
    ether_link_change_t           link_change[ETHER_NUM_PORT];           ///< status of link change
    ether_magic_packet_t          magic_packet;          ///< status of magic packet detection

    ether_link_establish_status_t link_establish_status[ETHER_NUM_PORT]; ///< Current Link status

} ether_instance_ctrl_t;

/*
 * PauseMaskE, PauseValE and pause_resolutionS are use to create
 * PAUSE resolution Table 28B-3 in IEEE 802.3-2008 standard.
 */
typedef enum e_ether_pause_mask
{
    ETHER_PAUSE_MASK0,
    ETHER_PAUSE_MASK1,
    ETHER_PAUSE_MASK2,
    ETHER_PAUSE_MASK3,
    ETHER_PAUSE_MASK4,
    ETHER_PAUSE_MASK5,
    ETHER_PAUSE_MASK6,
    ETHER_PAUSE_MASK7,
    ETHER_PAUSE_MASK8,
    ETHER_PAUSE_MASK9,
    ETHER_PAUSE_MASKA,
    ETHER_PAUSE_MASKB,
    ETHER_PAUSE_MASKC,
    ETHER_PAUSE_MASKD,
    ETHER_PAUSE_MASKE,
    ETHER_PAUSE_MASKF
} ether_pause_mask_t;

typedef enum e_ether_pause_val
{
    ETHER_PAUSE_VAL0,
    ETHER_PAUSE_VAL1,
    ETHER_PAUSE_VAL2,
    ETHER_PAUSE_VAL3,
    ETHER_PAUSE_VAL4,
    ETHER_PAUSE_VAL5,
    ETHER_PAUSE_VAL6,
    ETHER_PAUSE_VAL7,
    ETHER_PAUSE_VAL8,
    ETHER_PAUSE_VAL9,
    ETHER_PAUSE_VALA,
    ETHER_PAUSE_VALB,
    ETHER_PAUSE_VALC,
    ETHER_PAUSE_VALD,
    ETHER_PAUSE_VALE,
    ETHER_PAUSE_VALF
} ether_pause_val_t;

#pragma pack(1)
typedef struct st_ether_pause_resolution
{
    ether_pause_mask_t mask;
    ether_pause_val_t  value;
    uint8_t            transmit;
    uint8_t            receive;
} ether_pause_resolution_t;

/*
 * transmit/receive buffer from/to application
 */

typedef struct st_ether_frame_user
{
    uint8_t     dst_addr[6];        /* destination addres */
    uint8_t     src_addr[6];        /* source address */
    uint8_t     type[2];            /* type */
    uint8_t     data[2];            /* data */
} ether_frame_user_t;

/*
 * transmit/receive buffer from/to switch
 */
typedef struct st_ether_mgtag
{
    uint8_t     control_tag[2];     /* control_tag of management-tag  */
    uint8_t     control_data[2];    /* control_data of management-tag  */
    uint8_t     control_data2[4];   /* control_data2 of management-tag  */
}ether_mgtag_t;  

typedef struct st_ether_frame_mgtag
{
    uint8_t         dst_addr[6];    /* destination addres */
    uint8_t         src_addr[6];    /* source address */
    ether_mgtag_t   mgtag;          /* management tag */
    uint8_t         type[2];        /* type */
    uint8_t         data[2];        /* data */
} ether_frame_mgtag_t;
#pragma pack()

#define ETHER_FCS_SIZE  (4U)

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const ether_api_t g_ether_on_ether;

/** @endcond */

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 ***********************************************************************************************************************/
#ifdef BSP_PLAT_RZN1SDB /* RZ/N1S */
    static void      ether_isr_sbd (IRQn_Type *irq_num_ptr);
    static void      ether_isr_pmt (IRQn_Type *irq_num_ptr);
#else /* !BSP_PLAT_RZN1SDB (RZ/T2) */
  #ifndef RZT2_RSK
    void ether_isr_pmt (uint32_t int_sense);
    void ether_isr_sbd (uint32_t int_sense);
  #endif /* !RZT2_RSK */
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */

/**********************************************************************************************************************
 * Public Function Prototypes
 **********************************************************************************************************************/
fsp_err_t R_ETHER_Open(ether_ctrl_t * const p_ctrl, ether_cfg_t const * const p_cfg);

fsp_err_t R_ETHER_Close(ether_ctrl_t * const p_ctrl);

fsp_err_t R_ETHER_Read(ether_ctrl_t * const p_ctrl, void * const p_buffer, uint32_t * const length_bytes);

fsp_err_t R_ETHER_BufferRelease(ether_ctrl_t * const p_ctrl);

fsp_err_t R_ETHER_Write(ether_ctrl_t * const p_ctrl, void * const p_buffer, uint32_t const frame_length);

fsp_err_t R_ETHER_LinkProcess(ether_ctrl_t * const p_ctrl);

fsp_err_t R_ETHER_WakeOnLANEnable(ether_ctrl_t * const p_ctrl);

/*******************************************************************************************************************//**
 * @} (end addtogroup ETHER)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 // R_ETHER_H
