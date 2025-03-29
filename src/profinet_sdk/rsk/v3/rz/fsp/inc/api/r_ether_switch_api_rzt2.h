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
 * @ingroup RENESAS_INTERFACES
 * @defgroup ETHER_PHY_API Ethernet PHY Interface
 * @brief Interface for Ethernet PHY functions.
 *
 * @section ETHER_SWITCH_API_Summary Summary
 * The Ethernet SWITCH module (r_ether_switch) provides an API for standard Ethernet SWITCH communications applications that use
 * the ETHERC peripheral.
 *
 * The Ethernet SWITCH interface supports the following features:
 * - Link change check support
 *
 * Implemented by:
 * - @ref ETHER_SWITCH
 *
 * @{
 **********************************************************************************************************************/

#ifndef R_ETHER_SWITCH_API_H
#define R_ETHER_SWITCH_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

/* Register definitions, common services and error codes. */
#ifndef BSP_PLAT_RZN1SDB /* RZ/T2 */
#include "bsp_api.h"
#endif /* !BSP_PLAT_RZN1SDB */

//#define ETHER_SWITCH_CONFIG_PTP
#define ETHER_SWITCH_CONFIG_TDMA
#define ETHER_SWITCH_CONFIG_PRIORITY3_OTHER
#define ETHER_SWITCH_CONFIG_PRIORITY4
#define ETHER_SWITCH_CONFIG_PRIORITY5

#ifdef ETHER_SWITCH_CONFIG_PRIORITY5
    #define ETHER_SWITCH_CONFIG_SNOOP
    #define ETHER_SWITCH_CONFIG_BRIDGE
#ifndef RZT2_RSK_PNS
    #define ETHER_SWITCH_CONFIG_DLR
#endif
#endif /* ETHER_SWITCH_CONFIG_PRIORITY5 */

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define ETHER_SWITCH_API_VERSION_MAJOR      (0U)
#define ETHER_SWITCH_API_VERSION_MINOR      (1U)

#define ETHER_SWITCH_MAC_ADDR_LEN           (6U)

#define ETHER_SWITCH_STATE_DEFAULT          0   ///< default state
#define ETHER_SWITCH_STATE_UNKNOWN          1   ///< unknown state
#define ETHER_SWITCH_STATE_DOWN             2   ///< state down
#define ETHER_SWITCH_STATE_UP               3   ///< state up
#define ETHER_SWITCH_CONFIG_PTP				1

#define  ETHSW_LK_CLEAR_TABLE            (1<<6)
#define  ETHSW_LK_CLEAR_DYNAMIC         (1<<22)
#define  ETHSW_LK_CLEAR_STATIC          (1<<23)
#define  ETHSW_LK_GETLASTNEW            (1<<24)
#define  ETHSW_LK_WRITE                 (1<<25)
#define  ETHSW_LK_READ                  (1<<26)
#define  ETHSW_LK_WAIT_COMPLETE         (1<<27)
#define  ETHSW_LK_LOOKUP                (1<<28)
#define  ETHSW_LK_CLEAR                 (1<<29)
#define  ETHSW_LK_DELETE_PORT           (1<<30)
#define  ETHSW_LK_BUSY                  (1<<31)

//#define  ETHSW_LK_DATA_HI               0x410
#define  ETHSW_LK_DH_VALID              (1<<16)
#define  ETHSW_LK_DH_STATIC             (1<<17)
#define  ETHSW_LK_DH_AGE                (1<<18)
#define  ETHSW_LK_DH_PORT_NUM_MASK      0x3e00000
#define  ETHSW_LK_DH_PORT_NUM(x)        (x<<21)

#define  ETHSW_LK_ADDR_MASK             0x1fff

#define ETHSW_MAC_CLEAR_ALL 0
#define ETHSW_MAC_CLEAR_DYNAMIC 1
#define ETHSW_MAC_CLEAR_STATIC 2


/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

typedef uint32_t    ether_switch_port_cast;
typedef uint32_t    ether_switch_port_mask_t;

/** Ethernet port macros */
#define ETHER_SWITCH_PORT_HOST_CNT      1
#define ETHER_SWITCH_PORT_HOST          ((ether_switch_port_cast) (1 << 30))
#define ETHER_SWITCH_PORT_MASK          ((ether_switch_port_cast) ((uint64_t) 1 << 31))
#define ETHER_SWITCH_PORT(x)            ((ether_switch_port_cast) (x) & ~ETHER_SWITCH_PORT_MASK)
#define ETHER_SWITCH_PORT_BIT(x)        ((ether_switch_port_cast) (1 << (x)) | ETHER_SWITCH_PORT_MASK)
#define ETHER_SWITCH_PORT_BITS          (~(ETHER_SWITCH_PORT_HOST | ETHER_SWITCH_PORT_MASK))

typedef uint8_t ether_switch_mac_addr_t[ETHER_SWITCH_MAC_ADDR_LEN];

/**  speed and duplex of the port */
typedef enum e_ether_switch_link_speed
{
    ETHER_SWITCH_LINK_SPEED_NO_LINK = 0,    ///< Link is not established
    ETHER_SWITCH_LINK_SPEED_10H     = 1,    ///< Link status is 10Mbit/s and half duplex
    ETHER_SWITCH_LINK_SPEED_10F     = 2,    ///< Link status is 10Mbit/s and full duplex
    ETHER_SWITCH_LINK_SPEED_100H    = 3,    ///< Link status is 100Mbit/s and half duplex
    ETHER_SWITCH_LINK_SPEED_100F    = 4,    ///< Link status is 100Mbit/s and full duplex
    ETHER_SWITCH_LINK_SPEED_1000H   = 5,    ///< Link status is 1000Mbit/s and half duplex
    ETHER_SWITCH_LINK_SPEED_1000F   = 6     ///< Link status is 1000Mbit/s and full duplex
} ether_switch_link_speed_t;

/** port number */
typedef enum e_ether_switch_port
{
    ETHER_SWITCH_PORT_0 = 0,                ///< port-0
    ETHER_SWITCH_PORT_1 = 1,                ///< port-1
    ETHER_SWITCH_PORT_2 = 2,                 ///< port-2
    ETHER_SWITCH_PORT_3 = 3                 ///< port-3
} ether_switch_port_t;

/** control command */
typedef enum e_ether_switch_command
{
    ETHER_SWITCH_CMD_PORT_COUNT = 0x1000,       ///< get count of available external ethernet ports
    ETHER_SWITCH_CMD_MACTAB_GET,                ///< get the port mask for the given MAC address from the MAC table.
    ETHER_SWITCH_CMD_MACTAB_SET,                ///< set the MAC adrres entry to the static MAC table
    ETHER_SWITCH_CMD_MACTAB_CONF,               ///< set/get config paramter for the MAC table
    ETHER_SWITCH_CMD_MACTAB_CLR,                ///< clear all or parts of entry from the MAC table
    ETHER_SWITCH_CMD_MAC_UNI_FLUSH,             ///< clear dynamic entryentry from the MAC table
    ETHER_SWITCH_CMD_MAC_MULTI_FLUSH,           ///< clear dynamic entryentry from the MAC table
    ETHER_SWITCH_CMD_LINK_STATE,                ///< get link state for given port
    ETHER_SWITCH_CMD_PORT_ADMIN_STATE,          ///< set/get admin state for the given port
    
    ETHER_SWITCH_CMD_VLAN_DEF,                  ///< set/get the default VLAN for the given port.
    ETHER_SWITCH_CMD_VLAN_PORT_ADD,             ///< add a VLAN for a given port to the VLAN domain table.
    ETHER_SWITCH_CMD_VLAN_PORT_REM,             ///< remove a VLAN entry from the VLAN domain table for the given port.
    ETHER_SWITCH_CMD_VLAN_MODE_IN,              ///< set/get the mode of VLAN input operations for the given port
    ETHER_SWITCH_CMD_VLAN_MODE_OUT,             ///< set/get the mode of VLAN output operations for the given port.
    ETHER_SWITCH_CMD_VLAN_TABLE_CNT,            ///< get count of VLAN table entries
    ETHER_SWITCH_CMD_VLAN_TABLE_GET,            ///< get information of VLAN table entry.
    ETHER_SWITCH_CMD_VLAN_VERIFY,               ///< set/get VLAN verification for the given port.
    ETHER_SWITCH_CMD_VLAN_DISCUNK,              ///< set/get VLAN discartion of unknown frames for the given port.
    ETHER_SWITCH_CMD_LEARNING,                  ///< set/get learning state for the given port.
    ETHER_SWITCH_CMD_PORT_FWD_ADD,              ///< enable/disable the forwarding of incoming frames on a port for RSTP
    ETHER_SWITCH_CMD_PORT_FWD_DEL,              ///< enable/disable the forwarding of incoming frames on a port for RSTP

#ifdef ETHER_SWITCH_CONFIG_PTP
    ETHER_SWITCH_CMD_TIMESTAMP,                 ///< enable/disable the send time stamp of the external port.
    ETHER_SWITCH_CMD_TIMESTAMP_RX,              ///< Axel select timer for RX timestamping for each port
    ETHER_SWITCH_CMD_LATENCY_EGRESS,            ///< get the latency from timestamping the transmission frame to transmission by the PHY.
    ETHER_SWITCH_CMD_LATENCY_INGRESS,           ///< get the latency from when the PHY receives the received frame until it is time stamped.
    ETHER_SWITCH_CMD_TIMER_ACTIVE,              ///< enable/disable the timer with the specified timer number.
    ETHER_SWITCH_CMD_TIMER_VALUE,               ///< get current timer value.
    ETHER_SWITCH_CMD_PTP_DOMAIN,                ///< set/get the domain number in the specified timer
    ETHER_SWITCH_CMD_PTP_PEERDELAY,             ///< set/get the peer delay value for the specified timer number of the specified port number.
    ETHER_SWITCH_CMD_PTP_CALLBACK,              ///< register a callback function that receives a send timestamp event.
#endif /* ETHER_SWITCH_CONFIG_PTP */

#ifdef ETHER_SWITCH_CONFIG_TDMA
    ETHER_SWITCH_CMD_TDMA_ENABLE,               ///< set the state of the TDMA scheduler.
    ETHER_SWITCH_CMD_TDMA_TCV_SEQ_ENTRY,        ///< set/get TCV sequence entry.
    ETHER_SWITCH_CMD_TDMA_TCV_DAT_ENTRY,        ///< set/get TCV data entry.
    ETHER_SWITCH_CMD_TDMA_TCV_SEQ_RANGE,        ///< set/get start and last of TCV sequence entry.
    ETHER_SWITCH_CMD_MMCLT_QGATE,               ///< set queue gate.
#endif /* ETHER_SWITCH_CONFIG_TDMA */

#ifdef ETHER_SWITCH_CONFIG_PRIORITY3_OTHER
    ETHER_SWITCH_CMD_PORT_MIRROR,               ///< set/get port mirroring configuration.
    ETHER_SWITCH_CMD_QOS_MODE,                  ///< set/get the priority resolution mode for the given port.
    ETHER_SWITCH_CMD_QOS_PRIO_VLAN,             ///< set/get the priority mapping between VLAN tag priorities and the switch core priorities.
    ETHER_SWITCH_CMD_QOS_PRIO_IP,               ///< set/get the priority for the given DiffServ field of an IP packet for the given port.
    ETHER_SWITCH_CMD_QOS_PRIO_TYPE,             ///< set/get the priority for the given Ethertypes to the given values.
    ETHER_SWITCH_CMD_QUEUES_PER_PORT,           ///< get counter of output queues per port.
    ETHER_SWITCH_CMD_DISC_UNK,                  ///< set/get the Discard Unknown Destination Bit in Control register
    ETHER_SWITCH_CMD_FLOOD_UNK,                 ///< set/get the flood domain port masks for frames with unknown destinations
#endif  /* ETHER_SWITCH_CONFIG_PRIORITY3_OTHER */

#ifdef ETHER_SWITCH_CONFIG_PRIORITY4
    ETHER_SWITCH_CMD_STORMTIME,                 ///< set/get the timeframe for the broadcast/multicast rate limiting in ms.
    ETHER_SWITCH_CMD_BCAST_LIMIT,               ///< set/get the max allowed broadcast frames that can be received in a given time.
    ETHER_SWITCH_CMD_MCAST_LIMIT,               ///< set/get the max allowed multicast frames that can be received in a given time.
    ETHER_SWITCH_CMD_TXRATE,                    ///< set the max allowed bandwidth for the given port in percent.
#endif  /* ETHER_SWITCH_CONFIG_PRIORITY4 */

#ifdef ETHER_SWITCH_CONFIG_PRIORITY5
    ETHER_SWITCH_CMD_EEE,                       ///< set/get the EEE (Energy Efficient Ethernet) automatic operation mode of the switch.

#ifdef ETHER_SWITCH_CONFIG_SNOOP
    ETHER_SWITCH_CMD_SNOOP_ADD,                 ///< add settings for the port snoop function.
    ETHER_SWITCH_CMD_SNOOP_REM,                 ///< delete the port snoop function setting.
#endif  /* ETHER_SWITCH_CONFIG_SNOOP */

#ifdef ETHER_SWITCH_CONFIG_BRIDGE
    ETHER_SWITCH_CMD_PORT_AUTH,                 ///< set/get the authentication status of the specified port.
    ETHER_SWITCH_CMD_PORT_CTRL_DIR,             ///< set/get the direction of the specified port.
    ETHER_SWITCH_CMD_PORT_EAPOL_ENABLE,         ///< set/get the reception permission of the EAPOL frame of the specified port.
    ETHER_SWITCH_CMD_BPDU,                      ///< set/get permission to receive BPDU frames on the specified port.
#endif  /* ETHER_SWITCH_CONFIG_BRIDGE */

#ifdef ETHER_SWITCH_CONFIG_DLR
    ETHER_SWITCH_CMD_DLR_INIT,                  ///< initialize the DLR extension module.
    ETHER_SWITCH_CMD_DLR_ENABLE,                ///< set up the DLR extension module.
    ETHER_SWITCH_CMD_DLR_SHUTDOWN,              ///< stop the DLR expansion module.
    ETHER_SWITCH_CMD_DLR_LAST_BEACON,           ///< get the beacon reception status of the specified port.
    ETHER_SWITCH_CMD_DLR_NODE_STATE,            ///< get the current state of the DLR local node.
    ETHER_SWITCH_CMD_DLR_RING_STATE,            ///< get the current state of the DLR local node.
    ETHER_SWITCH_CMD_DLR_SV_IP,                 ///< get the IP address of the DLR supervisor.
    ETHER_SWITCH_CMD_DLR_SV_PREC,               ///< get DLR Supervisor Priority.
    ETHER_SWITCH_CMD_DLR_VLAN,                  ///< get the VLAN ID of the DLR beacon frame.
    ETHER_SWITCH_CMD_DLR_SV_MAC,                ///< get the MAC address of the DLR supervisor.
    ETHER_SWITCH_CMD_DLR_FORWARD,               ///< initialize the DLR compatibility feature.
#endif  /* ETHER_SWITCH_CONFIG_DLR */

    ETHER_SWITCH_CMD_FRAMESIZE_MAX,             ///< set/get the maximum frame length of the specified port.
#endif  /* ETHER_SWITCH_CONFIG_PRIORITY5 */

} ether_switch_command_t;

/** boolean data type */
typedef enum e_ether_switch_bool
{
    ETHER_SWITCH_FALSE  = 0,                    ///< false
    ETHER_SWITCH_TRUE   = 1                     ///< true
} ether_switch_bool_t;


/** Control block.  Allocate an instance specific control block to pass into the API calls.
 * @par Implemented as
 * - ether_switch_instance_ctrl_t
 */
typedef void ether_switch_ctrl_t;

/** Configuration parameters. */
typedef struct st_ether_switch_cfg
{
    uint8_t         channel;                    ///< Channel

    IRQn_Type       irq;                        ///< NVIC interrupt number
    uint32_t        interrupt_priority;         ///< NVIC interrupt priority

    void (* p_callback)(ether_switch_callback_args_t * p_args);  ///< Callback provided when an ISR occurs.

    /** Placeholder for user data.  Passed to the user callback in ether_switch_callback_args_t. */
    void const *    p_context;
    void const *    p_extend;                   ///< Placeholder for user extension.
} ether_switch_cfg_t;

/** MAC table entry */
typedef struct st_ether_switch_mactab_entry
{
    ether_switch_mac_addr_t *   addr;           ///< MAC address pointer
    uint32_t                    portmask;       ///< port mask
    uint32_t                    priority;       ///< switching priority
} ether_switch_mactab_entry_t;

/** MAC table config */
typedef struct st_ether_switch_mactab_conf
{
    ether_switch_bool_t         learn;          ///< Enable/disable MAC learning
    ether_switch_bool_t         aging;          ///< Enable/disable address aging
    ether_switch_bool_t         migration;      ///< Enable/disable the migration of devices from one port to another
    ether_switch_bool_t         pervlan;        ///< Enable/disable per VLAN MAC learning
    ether_switch_bool_t         discunknown;    ///< Enable/disable discarding of unknown destination frames
} ether_switch_mactab_conf_t;

/** MAC table clear/flush modes */
typedef enum e_ether_switch_mactab_clr_modes
{
    ETHER_SWITCH_MACTAB_CLR_STATIC,             ///< Static MAC table entries
    ETHER_SWITCH_MACTAB_CLR_DYNAMIC,            ///< Learned MAC table entries
    ETHER_SWITCH_MACTAB_CLR_ALL                 ///< Static and learn entries
} ether_switch_mactab_clr_modes_t;

/** VLAN input processing modes */
typedef enum e_ether_switch_vlan_in_mode
{
    ETHER_SWITCH_VLANIN_PASSTHROUGH_OVERRIDE,   ///< Single Tagging with Passthrough/VID Overwrite
    ETHER_SWITCH_VLANIN_REPLACE,                ///< Single Tagging with Replace
    ETHER_SWITCH_VLANIN_TAG_ALWAYS,             ///< Tag always
    ETHER_SWITCH_VLANIN_DISABLE = 0xFF          ///< Disable VLAN
} ether_switch_vlan_in_mode_t;

/** VLAN output processing mode */
typedef enum e_ether_switch_vlan_out_mode
{
    ETHER_SWITCH_VLANOUT_DISABLE,               ///< No manipulation
    ETHER_SWITCH_VLANOUT_STRIP,                 ///< Strip mode
    ETHER_SWITCH_VLANOUT_TAGTHROUGH,            ///< Tag Thru mode
    ETHER_SWITCH_VLANOUT_DOMAINTRANSP           ///< Domain / Transparent mode
} ether_switch_vlan_out_mode_t;

/** VLAN table entry */
typedef struct st_ether_switch_vlan_entry
{
   uint32_t             index;                  ///< Index of entry
   ether_switch_bool_t  valid;                  ///< Entry valid
   uint32_t             vlan;                   ///< VLAN ID
   uint32_t             portmask;               ///< Port mask
} ether_switch_vlan_entry_t;


#ifdef ETHER_SWITCH_CONFIG_PTP
    /** Timer enable/disable control */
    typedef struct st_ether_switch_time_enable
    {
        ether_switch_bool_t flag_enable;        ///< Timer enable flag
        uint8_t             tim_num;            ///< Timer number
        uint8_t             clock_period;       ///< Clock period of the timestamping clock
    } ether_switch_time_enable_t;
    
    /** Timer value */
    typedef struct st_ether_switch_timestamp
    {
        uint8_t             tim_num;            ///< Timer number
        uint32_t            tim_sec;            ///< Second
        uint32_t            tim_nsec;           ///< Nanosecond
        uint32_t            timestamp_id;       ///< Timeatamp ID
    } ether_switch_timestamp_t;
    
    /** Domain & Timer mapping info */
    typedef struct st_ether_switch_ptp_domain
    {
        uint8_t             tim_num;            ///< Timer number
        uint8_t             domain_num;         ///< Domain number
    } ether_switch_ptp_domain_t;
    
    
    /** Peer delay info */
    typedef struct st_ether_switch_ptp_peerdelay
    {
            uint8_t         tim_num;            ///< Timer number
            uint32_t        peerdelay;          ///< Peer delay value
    } ether_switch_ptp_peerdelay_t;
    
    /** PTP callback event */
    typedef enum e_ether_switch_ptp_event
    {
        ETHER_SWITCH_PTP_CALLBACK_TS_VALID = 0x10,
        ETHER_SWITCH_PTP_CALLBACK_TS_OVER,
    } ether_switch_ptp_event_t;
#endif /* ETHER_SWITCH_CONFIG_PTP */

#ifdef ETHER_SWITCH_CONFIG_TDMA
    /** tdma callback event */
    typedef enum e_ether_switch_tdma_event
    {
        ETHER_SWITCH_TDMA_CALLBACK_TCV_INT = 0x100, ///< active event of TCV sequence entry
    } ether_switch_tdma_event_t;

    /** tdma callback data */
    typedef union
    {
        uint16_t            tcv_s_idx;          ///< index of TCV sequence entry (valu of TDMA_IRQ_STAT_ACK_b.TCV_IDX_ACK)
    } ether_switch_tdma_callback_data_t;

    typedef struct st_ether_switch_tdma_sched_entries {
        uint32_t offset;    //Axelinterval;         // offset of schedule entry in ns 
        uint8_t port_mask;        // Port Mask with one-hot encoding 
        uint8_t gate_mask;        // Gate Mask with one-hot encoding 
        uint8_t hold_req;          // One bit enable Hold request for frame preemption , 
        uint8_t gate_mode;          // One bit enable gate mode , only on newer devices, 
        uint8_t trigger_mode;          // One bit enable trigger mode , 
        uint8_t in_ct_enable;          // One bit enable cut through , 
        uint8_t out_ct_enable;          // One bit enable cut through , 
        uint8_t red_phase;          // Indicate Red Phase in IRT applications
        uint8_t inc_1;          // enable increment 1 , 
        uint8_t inc_0;          // enable increment 0 ,
        uint8_t gpio_mask;       // GPIO to trigger when this slot is active
    } ether_switch_tdma_sched_entries_t;


    /** tdma enable info */
    typedef struct st_ether_switch_tdma_enable
    {
        ether_switch_bool_t flg_enable;
        uint8_t             tim_num;            ///< timer number
        uint32_t            portmask;           ///< port mask
        uint32_t            tdma_start;         ///< start time for the first cycle of TDMA
        uint32_t            tdma_modulo;        ///< system timer modulo for 1s
        uint32_t            tdma_cycle;         ///< periodic cycle time for TDMA scheduler
        uint8_t             num_entries;        ///< amount of entries in the scheduler data base 
        uint16_t            gpio_mode;        ///<  mode for each of 8 GPIO, 2 bits for each GPIO  
        ether_switch_tdma_sched_entries_t *  p_tdma_sched_entries;   ///< tdma schedule entry info
        void (* p_tdma_callback)(               ///< pointer of callback function
                    ether_switch_tdma_event_t            event,
                    ether_switch_tdma_callback_data_t * p_tdma_data);

    } ether_switch_tdma_enable_t; 
    
    /** tdma sequence entry info */
    typedef struct st_ether_switch_tdma_seq_entry
    {
        uint32_t            tcv_func;           ///< TCV function
        uint16_t            tcv_d_index;        ///< index in the TCV data table
        uint16_t            tcv_s_addr;         ///< index in the TCV sequence entry
    } ether_switch_tdma_seq_entry_t;
    
    /** define of tcv_func member in ether_switch_tdma_seq_entry_t */
    #define ETHER_SWITCH_TDMA_TCV_NON       0x0 ///< non
    #define ETHER_SWITCH_TDMA_TCV_START     0x1 ///< TCV start vector
    #define ETHER_SWITCH_TDMA_TCV_INT       0x2 ///< TCV generate interrupt
    
    /** tdma data entry info */
    typedef struct st_ether_switch_tdma_dat_entry
    {
        uint32_t            tcv_d_addr;         ///< index in the TCV data table
        uint32_t            tcv_d_offset;       ///< time offset from the TDMA Cycle Start
        uint16_t            tcv_d_qgate;        ///< gate state of each queue
        uint32_t            tcv_d_pmask;        ///< tcv_d_qgate affected ports
    } ether_switch_tdma_dat_entry_t;
    
    /** range(start-last) of tdma sequence entry */
    typedef struct st_ether_switch_tdma_seq_range
    {
        uint32_t            tcv_start;          ///< first index in the TCV sequence entry
        uint32_t            tcv_last;           ///< last index in the TCV sequence entry
    } ether_switch_tdma_seq_range_t; 

    /** queue gate */
    typedef struct st_ether_switch_mmctl_qgate
    {
        uint32_t            portmask;           ///< per-port bit mask
        uint16_t            qgate;              ///< action to be performed on each queue (2-bits per queue)
    } ether_switch_mmclt_qgate_t; 

    /** CQF queue gate */
    typedef struct st_ether_switch_mmctl_cqf
    {
        uint8_t            cqf_sel;     //   Selcts which CQF should be configured
        uint8_t             prio_enable_mask;     //   - pointer to uint8_t with mask for priority enable. Each bit enables a queue
        uint8_t             queue_sel;     //- pointer to uint8_t with number for queue to be used for CQF group 
        uint8_t             gate_sel;     //- pointer to uint8_t with number for gate signal (from TDMA) to be used for CQF group 
        uint8_t             use_sop;     //ppointer to uint8_t with 1 if queue is decided at SOP, otherwise at EOP
        uint8_t             ref_sel;     //  pointer to uint8_t with 1 if gate control signal is used at ingress port, otherise at egress port
    } ether_switch_mmclt_cqf_t; 

        
#endif /* ETHER_SWITCH_CONFIG_TDMA */

#ifdef ETHER_SWITCH_CONFIG_PRIORITY3_OTHER
/** port mirroring modes */
typedef enum e_ether_switch_mirr_mode
{
    ETHER_SWITCH_MIRR_MODE_EGRESS_DA,           ///< Egress DA match
    ETHER_SWITCH_MIRR_MODE_EGRESS_SA,           ///< Egress SA match
    ETHER_SWITCH_MIRR_MODE_INGRESS_DA,          ///< Ingress DA match
    ETHER_SWITCH_MIRR_MODE_INGRESS_SA,          ///< Ingress SA match
    ETHER_SWITCH_MIRR_MODE_INGRESS_PORT,        ///< Ingress port match
    ETHER_SWITCH_MIRR_MODE_EGRESS_PORT          ///< Egress port match
} ether_switch_mirr_mode_t;

/** mirror port config */
typedef struct st_ether_switch_mirror_conf
{
    uint32_t                    port;           ///< The port id that will received all mirrored frames. A value of 0 disables the port mirroring.
    ether_switch_mirr_mode_t    mirrortype;     ///< The type of mirroring to configure.
    uint8_t                     portmap;        ///< The port map to which the mirror configuration will be applied to.
    ether_switch_mac_addr_t *   macaddr;        ///< MAC address pointer
} ether_switch_mirror_conf_t;

/** QoS mode */
typedef struct st_ether_switch_qos_mode
{
    uint16_t                    mode;           ///< A bitfield describing which modes shall be used.
    uint8_t                     defprio;        ///< Default priority
} ether_switch_qos_mode_t;

/** QoS IP priority */
typedef struct st_ether_switch_qos_prio_ip
{
    uint8_t                     diffserv;       ///< The DiffServ field of the IP packet
    uint8_t                     priority;       ///< The priority to assign.
} ether_switch_qos_prio_ip_t;

/** QoS Ethertype priority */
typedef struct st_ether_switch_qos_prio_type
{
    uint16_t                    type1;          ///< The first type to match against. A value of 0 disables that match.
    uint8_t                     prio1;          ///< The priority to be assigned for the first match.
    uint16_t                    type2;          ///< The second type to match against. A value of 0 disables that match.
    uint8_t                     prio2;          ///< The priority to be assigned for the second match.
} ether_switch_qos_prio_type_t;

/** flood domain configuration for unknown frames */
typedef struct st_ether_switch_flood_unk_conf
{
    ether_switch_port_mask_t port_mask_bcast;   ///< flood domain port mask for broadcasts with unkown destination
    ether_switch_port_mask_t port_mask_mcast;   ///< flood domain port mask for multicasts with unkown destination
    ether_switch_port_mask_t port_mask_ucast;   ///< flood domain port mask for unicasts with unkown destination
} ether_switch_flood_unk_conf_t;
#endif  /* ETHER_SWITCH_CONFIG_PRIORITY3_OTHER */

#ifdef ETHER_SWITCH_CONFIG_PRIORITY5
    /** EEE */
    typedef struct st_ether_switch_eee
    {
        ether_switch_bool_t state;                  ///< State of EEE auto mode (enabled/disabled)
        uint32_t            idletime;               ///< Idle time of EEE
    } ether_switch_eee_t;

#ifdef ETHER_SWITCH_CONFIG_SNOOP
    /** snooping modes */
    typedef enum e_ether_switch_snoop_matchmode
    {
        ETHER_SWITCH_SNOOP_MODE_IPDATA,             ///< Match data within IP pkt
        ETHER_SWITCH_SNOOP_MODE_IPPROT,             ///< Match IP protocol
        ETHER_SWITCH_SNOOP_MODE_ETYPE,              ///< Match ether type
        ETHER_SWITCH_SNOOP_MODE_DATA                ///< Match any data
    } ether_switch_snoop_matchmode_t;

    /** snooping actions */
    typedef enum e_ether_switch_snoop_action
    {
        ETHER_SWITCH_SNOOP_ACTION_CAPTURE = 1,      ///< Frames are only forwarded to mgt port
        ETHER_SWITCH_SNOOP_ACTION_CPY_FW,           ///< Frames are forwarded to mgt port and dest port
        ETHER_SWITCH_SNOOP_ACTION_DISCARD           ///< Discard frames
    } ether_switch_snoop_action_t;

    /** Snoop config */
    typedef struct st_ether_switch_snoop_config
    {
        ether_switch_snoop_matchmode_t  mode;       ///< Snooping mode
        ether_switch_snoop_action_t     action;     ///< How to handle matched frames
        uint16_t                        match;      ///< Data to match
        uint8_t                         mask;       ///< Match mask
        uint32_t                        flags;      ///< Flags
        uint32_t                        offset;     ///< Data offset
        uint32_t                        id;         ///< Id of snoop config
    } ether_switch_snoop_config_t;
#endif /* ETHER_SWITCH_CONFIG_SNOOP */

#ifdef ETHER_SWITCH_CONFIG_BRIDGE
    /** Port authorization states */
    #define ETHER_SWITCH_AUTH_DEFAULT           0       ///< default state
    #define ETHER_SWITCH_AUTH_UNKNOWN           1       ///< unknown state
    #define ETHER_SWITCH_AUTH_UNAUTHORIZED      2       ///< port is unauthorized
    #define ETHER_SWITCH_AUTH_AUTHORIZED        3       ///< port is authorized

    /** Port control directions */
    #define ETHER_SWITCH_CTRL_DIR_DEFAULT       0       ///< default EAPOL reception state
    #define ETHER_SWITCH_CTRL_DIR_UNKNOWN       1       ///< unknown EAPOL reception state
    #define ETHER_SWITCH_CTRL_DIR_INPUT         2       ///< EAPOL reception disabled
    #define ETHER_SWITCH_CTRL_DIR_BOTH          3       ///< EAPOL reception enabled

    /** Port EAPOL reception states */
    #define ETHER_SWITCH_EAPOL_DEFAULT          0       ///< default EAPOL reception state
    #define ETHER_SWITCH_EAPOL_UNKNOWN          1       ///< unknown EAPOL reception state
    #define ETHER_SWITCH_EAPOL_DISABLE          2       ///< EAPOL reception disabled
    #define ETHER_SWITCH_EAPOL_ENABLE           3       ///< EAPOL reception enabled

    /** BPDU management control states */
    typedef enum e_ether_switch_bpdu_ctrl
    {
        ETHER_SWITCH_BPDU_CTRL_UNKNOWN,             ///< unknown
        ETHER_SWITCH_BPDU_CTRL_DEFAULT,             ///< default
        ETHER_SWITCH_BPDU_CTRL_DISCARD,             ///< discarding
        ETHER_SWITCH_BPDU_CTRL_MGMT_FWD,            ///< mgmt port forward only
    } ether_switch_bpdu_ctrl_t;
#endif /* ETHER_SWITCH_CONFIG_BRIDGE */

#ifdef ETHER_SWITCH_CONFIG_DLR
    /** DLR events for the DLR State Machine */
    typedef enum e_ether_switch_dlr_event
    {
        DLR_EVENT_LINKLOST,                         ///< Link was lost
        DLR_EVENT_OWNFRAME,                         ///< the device received its own frame
        DLR_EVENT_LINKRESTORED,                     ///< Link is restored
        DLR_EVENT_BEACONFRAME,                      ///< Beacon Frame received
        DLR_EVENT_BEACONTIMEOUT,                    ///< Beacon Timer timed out
        DLR_EVENT_NEWSUPERVISOR,                    ///< a new Ring Supervisor was detected
        DLR_EVENT_LOCATEFAULT,                      ///< locate Fault frame received
        DLR_EVENT_NEIGHBORCHECKREQ,                 ///< Neighbor_Check_Request received
        DLR_EVENT_NEIGHBORCHECKRES,                 ///< Neighbor_Check_Response received
        DLR_EVENT_NEIGHBORTIMEOUT,                  ///< NeighborCheck Timer expired
        DLR_EVENT_FLUSHTABLES,                      ///< Flush_Tables Frame received
        DLR_EVENT_SIGNON,                           ///< Sign_On Frame received
        DLR_EVENT_NEWSTATE                          ///< Beacon Hardware detected new state
    } ether_switch_dlr_event_t;

    /** DLR initilize */
    typedef struct st_ether_switch_dlr_init
    {
        ether_switch_mac_addr_t *   p_host_addr;    ///< host MAC address pointer
        void (* p_dlr_callback)(                    ///< callback function pointer
                    ether_switch_dlr_event_t    event,
                    uint32_t                    port);
    } ether_switch_dlr_init_t;
#endif /* ETHER_SWITCH_CONFIG_DLR */
#endif /* ETHER_SWITCH_CONFIG_PRIORITY5 */

/** Ethernet command data */
typedef struct st_ether_switch_cmd 
{
    ether_switch_command_t  cmd;                ///< command
    ether_switch_bool_t     flg_set;            ///< write flag
    uint32_t                port;               ///< port or port mask

    union {
        uint32_t *                          p_cnt_ports;        ///< port count
        ether_switch_mactab_entry_t *       p_mac_tab;          ///< MAC table entry ptr
        ether_switch_mactab_conf_t *        p_mac_tab_cnf;      ///< MAC table config ptr
        ether_switch_mactab_clr_modes_t *   p_mac_tab_clr;      ///< MAC table clear modes ptr
        uint32_t *                          p_state_link;       ///< link state (up/down)
        uint32_t *                          p_state_port;       ///< port state (on/off)
        uint16_t *                          p_id_vlan;          ///< VLAN id
        ether_switch_vlan_in_mode_t *       p_mode_vlan_in;     ///< VLAN in mode
        ether_switch_vlan_out_mode_t *      p_mode_vlan_out;    ///< VLAN out mode
        uint32_t *                          p_cnt_tab_entries;  ///< table entries
        ether_switch_vlan_entry_t *         p_vlan;             ///< VLAN entry
        ether_switch_bool_t *               p_flag_enable;      ///< enable flag
        ether_switch_bool_t *               p_flag_dlr_learn;   ///< DLR learning flag

#ifdef ETHER_SWITCH_CONFIG_PTP
        uint32_t *                          p_latency_egress;   ///< egress latency
        uint32_t *                          p_latency_ingress;  ///< ingress latency

        ether_switch_time_enable_t *        p_time_enable;      ///< timer enable/disable control ptr
        uint8_t *                           p_timer;            ///< timer for assignment to each port

        ether_switch_timestamp_t *          p_timestamp;        ///< timer value ptr
        ether_switch_ptp_domain_t *         p_domain;           ///< domain & Timer mapping info ptr
        ether_switch_ptp_peerdelay_t *      p_peerdelay;        ///< peer delay info ptr

        void (* p_ptp_callback)(ether_switch_ptp_event_t  event,///< Callback provided when an ISR occurs.
                                uint8_t                   port,
                                uint32_t                  nanos,
                                ether_switch_timestamp_t *p_timestamp);
#endif /* ETHER_SWITCH_CONFIG_PTP */

#ifdef ETHER_SWITCH_CONFIG_TDMA
        ether_switch_tdma_enable_t *        p_tdma_enable;      ///< tdma enable info
        ether_switch_tdma_seq_entry_t *     p_tdma_seq_entry;   ///< tdma sequence entry info
        ether_switch_tdma_dat_entry_t *     p_tdma_dat_entry;   ///< tdma data entry info
        ether_switch_tdma_seq_range_t *     p_tdma_seq_range;   ///< range(start-last) of tdma sequence entry
        ether_switch_mmclt_qgate_t *        p_mmctl_qgate;      ///< queue gate
#endif /* ETHER_SWITCH_CONFIG_TDMA */

#ifdef ETHER_SWITCH_CONFIG_PRIORITY3_OTHER
        ether_switch_mirror_conf_t *        p_cnf_mirror;       ///< mirror port configuration
        ether_switch_qos_mode_t *           p_mode_qos;         ///< QoS mode
        uint32_t *                          p_bm_prio_vlan;     ///< VLAN priority bitmap
        ether_switch_qos_prio_ip_t *        p_prio_qos_ip;      ///< QoS IP priority
        ether_switch_qos_prio_type_t *      p_prio_qos_ethtype; ///< QoS Ethertype priority
        ether_switch_flood_unk_conf_t *     p_flood_unk_conf;   ///< configuration of flood domain for frames with unknown destination
        uint32_t *                          p_out_queues;       ///< Number of output queues
#endif  /* ETHER_SWITCH_CONFIG_PRIORITY3_OTHER */

#ifdef ETHER_SWITCH_CONFIG_PRIORITY4
        uint16_t *                          p_time_storm;       ///< timeframe for storm protection
        uint16_t *                          p_num_storm_frames; ///< number of frames for storm protection
        float *                             p_rate;             ///< rate
#endif  /* ETHER_SWITCH_CONFIG_PRIORITY4 */

#ifdef ETHER_SWITCH_CONFIG_PRIORITY5
        ether_switch_eee_t *                p_cnf_eee;          ///< EEE configuration

#ifdef ETHER_SWITCH_CONFIG_SNOOP
        ether_switch_snoop_config_t *       p_cnf_snoop;        ///< IGMP snooping configuration
        uint32_t *                          p_id_cnf_snoop;     ///< IGMP snooping configuration id
#endif /* ETHER_SWITCH_CONFIG_SNOOP */

#ifdef ETHER_SWITCH_CONFIG_BRIDGE
        uint32_t *                          p_state_auth;           ///< port authorization state
        uint32_t *                          p_state_dir_port_ctrl;  ///< port control direction state
        uint32_t *                          p_mode_eapol_recv;      ///< port EAPOL receive mode
        ether_switch_bpdu_ctrl_t *          p_state_bpdu_ctrl;      ///< BPDU management control state
#endif /* ETHER_SWITCH_CONFIG_BRIDGE */

#ifdef ETHER_SWITCH_CONFIG_DLR
        ether_switch_dlr_init_t *       p_dlr_init;             ///< DLR initilize
        uint32_t *                      p_state_dlr;            ///< DLR status
        ether_switch_mac_addr_t *       pp_addr_mac;            ///< MAC address ptr ref
#endif /* ETHER_SWITCH_CONFIG_DLR */

        uint32_t *                      p_frame_size_max;       ///< maximum frame size
#endif  /* ETHER_SWITCH_CONFIG_PRIORITY5 */
    } data;
} ether_switch_cmd_t;


/** Definition for GOAL interface compatibility */
typedef struct st_ether_switch_portentry
{
    uint32_t            latency_egress;     ///< Egress latency
    uint32_t            latency_ingress;    ///< Ingress latency
} ether_switch_portentry_t;

/** Functions implemented at the HAL layer will follow this API. */
typedef struct st_ether_switch_api
{
    /** Open driver.
     * @par Implemented as
     * - @ref R_ETHER_SWITCH_Open()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  p_cfg            Pointer to pin configuration structure.
     */
    fsp_err_t (* open)(ether_switch_ctrl_t * const p_api_ctrl, 
                       ether_switch_cfg_t const * const p_cfg);

    /** Close driver.
     * @par Implemented as
     * - @ref R_ETHER_SWITCH_Close()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     */
    fsp_err_t (* close)(ether_switch_ctrl_t * const p_api_ctrl);

    /** Configs speed and duplex of the port.
     * @par Implemented as
     * - @ref R_ETHER_SWITCH_ConfSpeed()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  port             Port number.
     * @param[in]  spee             Speed and Duplex,
     */
    fsp_err_t (* confSpeed)(ether_switch_ctrl_t *     const p_api_ctrl,
                            ether_switch_port_t       const port,
                            ether_switch_link_speed_t const speed);

    /** Control switch parameter.
     * @par Implemented as
     * - @ref R_ETHER_SWITCH_Control()
     *
     * @param[in]       p_api_ctrl  Pointer to control structure.
     * @param[in/out]   p_cmd       Pointer to command structure "ether_switch_timestamp_t" format.
     */
    fsp_err_t (* control)(ether_switch_ctrl_t *     const p_api_ctrl,
                          ether_switch_cmd_t *      p_cmd);

} ether_switch_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_ether_switch_instance
{
    ether_switch_ctrl_t      * p_ctrl;    ///< Pointer to the control structure for this instance
    ether_switch_cfg_t const * p_cfg;     ///< Pointer to the configuration structure for this instance
    ether_switch_api_t const * p_api;     ///< Pointer to the API structure for this instance
} ether_switch_instance_t;

/**********************************************************************************************************************
 * Public Function Prototypes
 **********************************************************************************************************************/
uint64_t Get_Time_Stamp(void);

/*******************************************************************************************************************//**
 * @} (end addtogroup ETHER_PHY_API)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /* R_ETHER_SWITCH_API_H */
