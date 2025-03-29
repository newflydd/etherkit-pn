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
 * @defgroup ETHER_API Ethernet Interface
 * @brief Interface for Ethernet functions.
 *
 * @section ETHER_API_Summary Summary
 * The Ethernet interface provides Ethernet functionality.
 * The Ethernet interface supports the following features:
 * - Transmit/receive processing (Blocking and Non-Blocking)
 * - Callback function with returned event code
 * - Magic packet detection mode support
 * - Auto negotiation support
 * - Flow control support
 * - Multicast filtering support
 *
 * Implemented by:
 * - @ref ETHER
 *
 * @{
 **********************************************************************************************************************/

#ifndef R_ETHER_API_H
#define R_ETHER_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

/* Register definitions, common services and error codes. */
#ifndef BSP_PLAT_RZN1SDB /* RZ/T2 */
#include "bsp_api.h"
#endif /* !BSP_PLAT_RZN1SDB */

#include "r_ether_phy_api_rzt2.h"
#include "r_ether_switch_api_rzt2.h"
#include "r_ether_access_api_rzt2.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define ETHER_API_VERSION_MAJOR    (0U)
#define ETHER_API_VERSION_MINOR    (1U)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Wake on LAN */
typedef enum e_ether_wake_on_lan
{
    ETHER_WAKE_ON_LAN_DISABLE = 0,     ///< Disable Wake on LAN
    ETHER_WAKE_ON_LAN_ENABLE  = 1,     ///< Enable Wake on LAN
} ether_wake_on_lan_t;

/** Flow control functionality  */
typedef enum e_ether_flow_control
{
    ETHER_FLOW_CONTROL_DISABLE = 0,    ///< Disable flow control functionality
    ETHER_FLOW_CONTROL_ENABLE  = 1,    ///< Enable flow control functionality with pause frames
} ether_flow_control_t;

/** Multicast Filter */
typedef enum e_ether_multicast
{
    ETHER_MULTICAST_DISABLE = 0,       ///< Disable reception of multicast frames
    ETHER_MULTICAST_ENABLE  = 1,       ///< Enable reception of multicast frames
} ether_multicast_t;

/** Promiscuous Mode */
typedef enum e_ether_promiscuous
{
    ETHER_PROMISCUOUS_DISABLE = 0,     ///< Only receive packets with current MAC address, multicast, and broadcast
    ETHER_PROMISCUOUS_ENABLE  = 1,     ///< Receive all packets
} ether_promiscuous_t;

/** Zero copy */
typedef enum e_ether_zerocopy
{
    ETHER_ZEROCOPY_DISABLE = 0,        ///< Disable zero copy in Read/Write function
    ETHER_ZEROCOPY_ENABLE  = 1,        ///< Enable zero copy in Read/Write function
} ether_zerocopy_t;
#pragma pack(1)
/** EDMAC descriptor as defined in the hardware manual.
 * Structure must be packed at 1 byte.
 */
typedef struct st_ether_instance_descriptor
{
    volatile uint32_t des0;            ///< TDES0/RDES0
    volatile uint32_t des1;            ///< TDES0/RDES0
    volatile uint32_t des2;            ///< TDES0/RDES0
    volatile uint32_t des3;            ///< TDES0/RDES0
} ether_instance_descriptor_t;


typedef struct st_ether_instance_tagged_header
{
    volatile uint8_t  des_addr[6];     ///< destination address
    volatile uint8_t  src_addr[6];     ///< source address
    volatile uint16_t controlTag;      ///< default: E001h
    volatile uint16_t controlData;     ///< controlData
    volatile uint32_t controlData2;    ///< controlData2
} ether_instance_tagged_header_t;
#pragma pack()


/** Control block.  Allocate an instance specific control block to pass into the API calls.
 * @par Implemented as
 * - ether_instance_ctrl_t
 */
typedef void ether_ctrl_t;

/** Configuration parameters. */
typedef struct st_ether_cfg
{
    uint8_t              channel;                        ///< Channel
    ether_zerocopy_t     zerocopy;                       ///< Zero copy enable or disable in Read/Write function
    ether_multicast_t    multicast;                      ///< Multicast enable or disable
    ether_promiscuous_t  promiscuous;                    ///< Promiscuous mode enable or disable
    ether_flow_control_t flow_control;                   ///< Flow control functionally enable or disable
    uint32_t             broadcast_filter;               ///< Limit of the number of broadcast frames received continuously
    uint8_t            * p_mac_address;                  ///< Pointer of MAC address

    ether_instance_descriptor_t * p_rx_descriptors;      ///< Receive descriptor buffer pool
    ether_instance_descriptor_t * p_tx_descriptors;      ///< Transmit descriptor buffer pool

    uint8_t num_tx_descriptors;                          ///< Number of transmission descriptor
    uint8_t num_rx_descriptors;                          ///< Number of receive descriptor

    //ether_instance_descriptor_t * p_tx_descriptor_top;  // Profinet: Pointer to the top of TX descriptor list


    uint8_t ** pp_ether_buffers;                         ///< Transmit and receive buffer

    uint32_t ether_buffer_size;                          ///< Size of transmit and receive buffer

    IRQn_Type sbd_irq;                                   ///< SBD interrupt number
    uint32_t  sbd_interrupt_priority;                    ///< SBD interrupt priority
    
    IRQn_Type pmt_irq;                                   ///< PMT_interrupt number
    uint32_t  pmt_interrupt_priority;                    ///< PMT_interrupt priority

    void (* p_callback)(ether_callback_args_t * p_args); ///< Callback provided when an ISR occurs.

    ether_phy_instance_t const * p_ether_phy_instance;  ///< Pointer to ETHER_PHY instance
    ether_switch_instance_t const * p_ether_switch_instance;    ///< Pointer to ETHER_SWITCH instance

   // void * p_profinet_red_frames;                 // Pointer to a pointer to the first of max. 5 profinet red frames

    /** Placeholder for user data.  Passed to the user callback in ether_callback_args_t. */
    void const * p_context;                              ///< Placeholder for user data.
    void const * p_extend;                               ///< Placeholder for user extension.
} ether_cfg_t;

/** Functions implemented at the HAL layer will follow this API. */
typedef struct st_ether_api
{
    /** Open driver.
     * @par Implemented as
     * - @ref R_ETHER_Open()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  p_cfg        Pointer to pin configuration structure.
     */
    fsp_err_t (* open)(ether_ctrl_t * const p_api_ctrl, ether_cfg_t const * const p_cfg);

    /** Close driver.
     * @par Implemented as
     * - @ref R_ETHER_Close()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     */
    fsp_err_t (* close)(ether_ctrl_t * const p_api_ctrl);

    /** Read packet if data is available.
     * @par Implemented as
     * - @ref R_ETHER_Read()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  p_buffer     Pointer to where to store read data.
     * @param[in]  length_bytes Number of bytes in buffer
     */
    fsp_err_t (* read)(ether_ctrl_t * const p_api_ctrl, void * const p_buffer, uint32_t * const length_bytes);

    /** Release rx buffer from buffer pool process in zero-copy read operation.
     * @par Implemented as
     * - @ref R_ETHER_BufferRelease()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     */
    fsp_err_t (* bufferRelease)(ether_ctrl_t * const p_api_ctrl);

    /** Write packet.
     * @par Implemented as
     * - @ref R_ETHER_Write()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  p_buffer     Pointer to data to write.
     * @param[in]  frame_length Send ethernet frame size (without 4 bytes of CRC data size).
     */
    fsp_err_t (* write)(ether_ctrl_t * const p_api_ctrl, void * const p_buffer, uint32_t const frame_length);

    /** Process link.
     * @par Implemented as
     * - @ref R_ETHER_LinkProcess()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     */
    fsp_err_t (* linkProcess)(ether_ctrl_t * const p_api_ctrl);

    /** Enable magic packet detection.
     * @par Implemented as
     * - @ref R_ETHER_WakeOnLANEnable()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     */
    fsp_err_t (* wakeOnLANEnable)(ether_ctrl_t * const p_api_ctrl);
} ether_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_ether_instance
{
    ether_ctrl_t      * p_ctrl;        ///< Pointer to the control structure for this instance
    ether_cfg_t const * p_cfg;         ///< Pointer to the configuration structure for this instance
    ether_api_t const * p_api;         ///< Pointer to the API structure for this instance
} ether_instance_t;

/*******************************************************************************************************************//**
 * @} (end addtogroup ETHER_API)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /* R_ETHERNET_API_H */
