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
 * @addtogroup ETHER_PHY
 * @{
 **********************************************************************************************************************/


#ifndef R_ETHER_PHY_H
#define R_ETHER_PHY_H

#ifndef BSP_PLAT_RZN1SDB /* RZ/T2 */
#include "bsp_api.h"
#endif /* !BSP_PLAT_RZN1SDB */

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_ether_phy_cfg.h"
#include "r_ether_phy_api_rzt2.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define ETHER_PHY_CODE_VERSION_MAJOR        (0U)
#define ETHER_PHY_CODE_VERSION_MINOR        (1U)

//#define ETHER_PHY_CFG_USE_PHY_KSZ8091RNB    (1)
//#define ETHER_PHY_CFG_USE_PHY_KSZ8041       (2)
//#define ETHER_PHY_CFG_USE_PHY_DP83620       (3)

#ifndef ETHER_PHY_ERROR_RETURN

/*LDRA_INSPECTED 77 S This macro does not work when surrounded by parentheses. */
 #define ETHER_PHY_ERROR_RETURN(a, err)    FSP_ERROR_RETURN((a), (err))
#endif

#define ETHERC_REG_SIZE                            (0x400UL)

/** "RPHY" in ASCII.  Used to determine if the control block is open. */
#define ETHER_PHY_OPEN    (0x52504859U)

/* Media Independent Interface */
#define ETHER_PHY_MII_ST                      (1)
#define ETHER_PHY_MII_READ                    (2)
#define ETHER_PHY_MII_WRITE                   (1)

/* Standard PHY Registers */
#define ETHER_PHY_REG_CONTROL                 (0)
#define ETHER_PHY_REG_STATUS                  (1)
#define ETHER_PHY_REG_IDENTIFIER1             (2)
#define ETHER_PHY_REG_IDENTIFIER2             (3)
#define ETHER_PHY_REG_AN_ADVERTISEMENT        (4)
#define ETHER_PHY_REG_AN_LINK_PARTNER         (5)
#define ETHER_PHY_REG_AN_EXPANSION            (6)
#define ETHER_PHY_REG_1000BT_CONTROL          (9)
#define ETHER_PHY_REG_1000BT_STATUS           (10)

/* Basic Mode Control Register Bit Definitions */
#define ETHER_PHY_CONTROL_RESET               (1 << 15)
#define ETHER_PHY_CONTROL_LOOPBACK            (1 << 14)
#define ETHER_PHY_CONTROL_100_MBPS            (1 << 13)
#define ETHER_PHY_CONTROL_AN_ENABLE           (1 << 12)
#define ETHER_PHY_CONTROL_POWER_DOWN          (1 << 11)
#define ETHER_PHY_CONTROL_ISOLATE             (1 << 10)
#define ETHER_PHY_CONTROL_AN_RESTART          (1 << 9)
#define ETHER_PHY_CONTROL_FULL_DUPLEX         (1 << 8)
#define ETHER_PHY_CONTROL_COLLISION           (1 << 7)

/* Basic Mode Status Register Bit Definitions */
#define ETHER_PHY_STATUS_100_T4               (1 << 15)
#define ETHER_PHY_STATUS_100F                 (1 << 14)
#define ETHER_PHY_STATUS_100H                 (1 << 13)
#define ETHER_PHY_STATUS_10F                  (1 << 12)
#define ETHER_PHY_STATUS_10H                  (1 << 11)
#define ETHER_PHY_STATUS_EX_STATUS            (1 << 8)
#define ETHER_PHY_STATUS_AN_COMPLETE          (1 << 5)
#define ETHER_PHY_STATUS_RM_FAULT             (1 << 4)
#define ETHER_PHY_STATUS_AN_ABILITY           (1 << 3)
#define ETHER_PHY_STATUS_LINK_UP              (1 << 2)
#define ETHER_PHY_STATUS_JABBER               (1 << 1)
#define ETHER_PHY_STATUS_EX_CAPABILITY        (1 << 0)

/* Auto Negotiation Advertisement Bit Definitions */
#define ETHER_PHY_AN_ADVERTISEMENT_NEXT_PAGE  (1 << 15)
#define ETHER_PHY_AN_ADVERTISEMENT_RM_FAULT   (1 << 13)
#define ETHER_PHY_AN_ADVERTISEMENT_ASM_DIR    (1 << 11)
#define ETHER_PHY_AN_ADVERTISEMENT_PAUSE      (1 << 10)
#define ETHER_PHY_AN_ADVERTISEMENT_100_T4     (1 << 9)
#define ETHER_PHY_AN_ADVERTISEMENT_100F       (1 << 8)
#define ETHER_PHY_AN_ADVERTISEMENT_100H       (1 << 7)
#define ETHER_PHY_AN_ADVERTISEMENT_10F        (1 << 6)
#define ETHER_PHY_AN_ADVERTISEMENT_10H        (1 << 5)
#define ETHER_PHY_AN_ADVERTISEMENT_SELECTOR   (1 << 0)

/* Auto Negotiate Link Partner Ability Bit Definitions */
#define ETHER_PHY_AN_LINK_PARTNER_NEXT_PAGE   (1 << 15)
#define ETHER_PHY_AN_LINK_PARTNER_ACK         (1 << 14)
#define ETHER_PHY_AN_LINK_PARTNER_RM_FAULT    (1 << 13)
#define ETHER_PHY_AN_LINK_PARTNER_ASM_DIR     (1 << 11)
#define ETHER_PHY_AN_LINK_PARTNER_PAUSE       (1 << 10)
#define ETHER_PHY_AN_LINK_PARTNER_100_T4      (1 << 9)
#define ETHER_PHY_AN_LINK_PARTNER_100F        (1 << 8)
#define ETHER_PHY_AN_LINK_PARTNER_100H        (1 << 7)
#define ETHER_PHY_AN_LINK_PARTNER_10F         (1 << 6)
#define ETHER_PHY_AN_LINK_PARTNER_10H         (1 << 5)
#define ETHER_PHY_AN_LINK_PARTNER_SELECTOR    (1 << 0)

/* 1000BASE-T Status */
#define ETHER_PHY_1000BT_STATUS_PARTNER_1000F   (1 << 11)
#define ETHER_PHY_1000BT_STATUS_PARTNER_1000H   (1 << 10)


#define ETHER_PHY_PIR_MDI_MASK                (1 << 3)
#define ETHER_PHY_PIR_MDO_HIGH                (0x04)
#define ETHER_PHY_PIR_MDO_LOW                 (0x00)
#define ETHER_PHY_PIR_MMD_WRITE               (0x02)
#define ETHER_PHY_PIR_MMD_READ                (0x00)
#define ETHER_PHY_PIR_MDC_HIGH                (0x01)
#define ETHER_PHY_PIR_MDC_LOW                 (0x00)

#define ETHER_PHY_PREAMBLE_LENGTH             (32U)
#define ETHER_PHY_WRITE_DATA_BIT_MASK         (0x8000)

#ifdef  ETHSW_MDIO_CONTROOL
    /* Define for MDIO_CFG_STATUS of MDIO_ETHSW */
    #define ETHSW_MDIO_CFG_STATUS_BUSY          (1 << 0)    /**< busy bit mask */
    #define ETHSW_MDIO_CFG_STATUS_READERR       (1 << 1)    /**< read error mask */

    /* Define for MDIO_COMMAND of MDIO_ETHSW */
    #define ETHSW_MDIO_COMMAND_REGADDR_MASK     0x1f        /**< REG address mask */
    #define ETHSW_MDIO_COMMAND_PHYADDR_SHIFT    5           /**< PHY address shift */
                                                        /**< PHY address mask */
    #define ETHSW_MDIO_COMMAND_PHYADDR_MASK     (0x1f << ETHSW_MDIO_COMMAND_PHYADDR_SHIFT)
    #define ETHSW_MDIO_COMMAND_TRANINIT_READ    (1 << 15)   /**< transaction init read */
#endif  /* ETHSW_MDIO_CONTROOL */


/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** ETHER PHY control block. DO NOT INITIALIZE.  Initialization occurs when @ref ether_phy_api_t::open is called. */
typedef struct st_ether_phy_instance_ctrl
{
    uint32_t open;                           ///< Used to determine if the channel is configured

    /* Configuration of Ethernet PHY-LSI module. */
    ether_phy_cfg_t *p_ether_phy_cfg; ///< Pointer to initial configurations.

    /* Interface for PHY-LSI chip. */
//EL    volatile uint32_t * p_reg_pir;           ///< Pointer to ETHERC peripheral registers.
    volatile uint32_t * p_reg_etherc;         ///< Pointer to ETHERC peripheral registers.

    /* The capabilities of the local link as PHY data */
    uint32_t local_advertise;                ///< Capabilities bitmap for local advertising.
} ether_phy_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const ether_phy_api_t g_ether_phy_on_ether_phy;

/** @endcond */

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 ***********************************************************************************************************************/

/**********************************************************************************************************************
 * Public Function Prototypes
 **********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_Open(ether_phy_ctrl_t * p_ctrl, ether_phy_cfg_t *p_cfg);

fsp_err_t R_ETHER_PHY_Close(ether_phy_ctrl_t * const p_ctrl);

fsp_err_t R_ETHER_PHY_StartAutoNegotiate(ether_phy_ctrl_t * const p_ctrl);

fsp_err_t R_ETHER_PHY_LinkPartnerAbilityGet(ether_phy_ctrl_t * const p_ctrl,
                                            uint32_t * const         p_line_speed_duplex,
                                            uint32_t * const         p_local_pause,
                                            uint32_t * const         p_partner_pause);

fsp_err_t R_ETHER_PHY_LinkStatusGet(ether_phy_ctrl_t * const p_ctrl);

void            ether_phy_targets_initialize(ether_phy_instance_ctrl_t * p_instance_ctrl);

#if ETHER_PHY_CFG_USE_PHY == ETHER_PHY_CFG_USE_PHY_AUTO
void            ether_phy_targets_initialize_vsc8541(ether_phy_instance_ctrl_t * p_instance_ctrl);
void            ether_phy_targets_initialize_ksz9131mx(ether_phy_instance_ctrl_t * p_instance_ctrl);
void            ether_phy_targets_initialize_ksz9031(ether_phy_instance_ctrl_t * p_instance_ctrl);
void            ether_phy_targets_initialize_ksz8041(ether_phy_instance_ctrl_t * p_instance_ctrl);
void            ether_phy_targets_initialize_rtl8211_rgmii(ether_phy_instance_ctrl_t *p_instance_ctrl);



#endif



/*******************************************************************************************************************//**
 * @} (end addtogroup ETHER_PHY)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 // R_ETHER_PHY_H
