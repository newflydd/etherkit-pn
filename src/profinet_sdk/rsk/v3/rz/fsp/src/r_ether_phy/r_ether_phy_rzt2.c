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

#ifdef BSP_PLAT_RZN1SDB /* RZ/N1S */
    #include "platform.h"
    #include "GMAC_SFR.h"
    #include "ETHSW_SFR.h"

    #include "fsp_common_api.h"
    #include "r_ether_phy_rzt2.h"

    #undef  R_GMAC_BASE
    #define R_GMAC_BASE     RZN1_GMAC1_BASE

    #undef  R_ETHSW_BASE
    #define R_ETHSW_BASE    RZN1_SWITCH_BASE

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
//    #define ETHSW_MDIO_CONTROOL

#else  /* !BSP_PLAT_RZN1SDB (RZ/T2) */
    #include "bsp_api.h"
    #include "r_ether_phy_rzt2.h"
	#include "r_ioport.h"

    #define BSP_FEATURE_ETHER_PHY_MAX_CHANNELS  (3U)

  #ifndef RZT2_RSK
    #define ETHSW_MDIO_CONTROOL
  #endif
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */


/***********************************************************************************************************************
 Macro definitions
 ***********************************************************************************************************************/
#if 0
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
#endif

extern void R_BSP_PinAccessEnable (void);
extern void R_BSP_PinAccessDisable(void);
extern fsp_err_t R_IOPORT_PinWrite (ioport_ctrl_t * const p_ctrl, bsp_io_port_pin_t pin, bsp_io_level_t level);

/***********************************************************************************************************************
 Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global variables (to be accessed by other files)
 ***********************************************************************************************************************/
extern ioport_instance_ctrl_t g_ioport_ctrl;

/***********************************************************************************************************************
 Exported global function
 ***********************************************************************************************************************/
uint32_t ether_phy_read (ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_addr);
uint32_t ether_phy_read_gmac (ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_addr);
void ether_phy_write (ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_addr, uint32_t data);
void ether_phy_write_gmac (ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_addr, uint32_t data);

#ifdef  ETHSW_MDIO_CONTROOL
uint32_t ether_phy_read_ethsw (ether_phy_instance_ctrl_t * p_instance_ctrl, 
                               uint32_t                    reg_addr,
                               volatile uint32_t         * p_Reg_Command,
                               volatile uint32_t         * p_Reg_Data,
                               volatile uint32_t         * p_Reg_Status );
void ether_phy_write_ethsw (ether_phy_instance_ctrl_t * p_instance_ctrl, 
                            uint32_t                    reg_addr, 
                            uint32_t                    data,
                            volatile uint32_t         * p_Reg_Command,
                            volatile uint32_t         * p_Reg_Data,
                            volatile uint32_t         * p_Reg_Status );
#endif  /* ETHSW_MDIO_CONTROOL */
#ifndef RZT2_RSK_PNS
void ether_phy_targets_initialize (ether_phy_instance_ctrl_t * p_instance_ctrl) __attribute__((weak));
#endif

// Use Switch access to link function as it is way quicker that through the MII
fsp_err_t R_ETHER_SWITCH_LinkStatusGet(ether_phy_ctrl_t * const p_ctrl);

/** ETHER_PHY HAL API mapping for Ethernet PHY Controller interface */
/*LDRA_INSPECTED 27 D This structure must be accessible in user code. It cannot be static. */
const ether_phy_api_t g_ether_phy_on_ether_phy =
{
    .open                      = R_ETHER_PHY_Open,
    .close                     = R_ETHER_PHY_Close,
    .startAutoNegotiate        = R_ETHER_PHY_StartAutoNegotiate,
    .linkPartnerAbilityGet     = R_ETHER_PHY_LinkPartnerAbilityGet,
    .linkStatusGet             = R_ETHER_SWITCH_LinkStatusGet,               //R_ETHER_PHY_LinkStatusGet,
};

/*******************************************************************************************************************//**
 * @addtogroup ETHER_PHY
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

uint8_t phy_first_access = 0; 

/********************************************************************************************************************//**
 * @brief Resets Ethernet PHY device. Implements @ref ether_phy_api_t::open.
 * 
 * @retval  FSP_SUCCESS                             Channel opened successfully.
 * @retval  FSP_ERR_ASSERTION                       Pointer to ETHER_PHY control block or configuration structure is NULL.
 * @retval  FSP_ERR_ALREADY_OPEN                    Control block has already been opened or channel is being used by another
 *                                                  instance. Call close() then open() to reconfigure.
 * @retval  FSP_ERR_INVALID_CHANNEL                 Invalid channel number is given.
 * @retval  FSP_ERR_INVALID_POINTER                 Pointer to p_cfg is NULL.
 * @retval  FSP_ERR_TIMEOUT                         PHY-LSI Reset wait timeout.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_Open (ether_phy_ctrl_t * p_ctrl, ether_phy_cfg_t * p_cfg)
{
    fsp_err_t               err    = FSP_SUCCESS;
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl;
    uint32_t                    reg;
    __attribute__((unused))uint32_t                    count = 0;

#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_PHY_ERROR_RETURN(NULL != p_cfg, FSP_ERR_INVALID_POINTER);
    ETHER_PHY_ERROR_RETURN((ETHER_PHY_OPEN != p_instance_ctrl->open), FSP_ERR_ALREADY_OPEN);
    ETHER_PHY_ERROR_RETURN((BSP_FEATURE_ETHER_PHY_MAX_CHANNELS > p_cfg->channel), FSP_ERR_INVALID_CHANNEL);
#endif

    /* Initialize configuration of ethernet phy module. */
    p_instance_ctrl->p_ether_phy_cfg = p_cfg;

#ifdef ETHSW_MDIO_CONTROOL
    /** Make sure this channel exists. */
    switch (p_instance_ctrl->p_ether_phy_cfg->channel)
    {
        case 0:
            p_instance_ctrl->p_reg_etherc   = (uint32_t *)R_GMAC_BASE;
            break;
        case 1:
            p_instance_ctrl->p_reg_etherc   = (uint32_t *)R_ETHSW_BASE;
            break;
        case 2:
            p_instance_ctrl->p_reg_etherc   = (uint32_t *)R_ETHSW_BASE;
            break;
        default:
            break;
    }
#else /* !ETHSW_MDIO_CONTROOL */
    p_instance_ctrl->p_reg_etherc   = (uint32_t *)R_GMAC_BASE;
#endif /* !ETHSW_MDIO_CONTROOL */

    p_instance_ctrl->local_advertise = 0;

#ifdef RZT2_RSK
//    if (0 == p_instance_ctrl->p_ether_phy_cfg->channel)
    if (0 == phy_first_access)
    {
        /* This code uses BSP IO functions to show how it is used.*/
        R_BSP_PinAccessEnable();

        /* Write Low-output to ESC_RESETOUT# as PHY reset */
#ifdef _RENESAS_RZT_
        R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) BSP_IO_PORT_20_PIN_7, (bsp_io_level_t)0);
#endif
#ifdef _RENESAS_RZN_
        R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) BSP_IO_PORT_13_PIN_4, (bsp_io_level_t)0);
#endif

        /* Delay 2us */
#ifdef UPD60620_PHY
        R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MICROSECONDS);
#else
        R_BSP_SoftwareDelay(2, BSP_DELAY_UNITS_MICROSECONDS);
#endif 
        /* Write High-output to ESC_RESETOUT# as reseass */
#ifdef _RENESAS_RZT_
        R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) BSP_IO_PORT_20_PIN_7, (bsp_io_level_t)1);
#endif
#ifdef _RENESAS_RZN_
        R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) BSP_IO_PORT_13_PIN_4, (bsp_io_level_t)1);
#endif

        /* Delay 2us */
        R_BSP_SoftwareDelay(2, BSP_DELAY_UNITS_MICROSECONDS);

        /* Protect PFS registers */
        R_BSP_PinAccessDisable();
        phy_first_access = 1; 
    }
    
    ether_phy_targets_initialize (p_instance_ctrl);

        // Set Phy inuitial setting acccording to desired link speed or autoneg
    ether_phy_link_speed_t link_speed = p_instance_ctrl->p_ether_phy_cfg->link_speed; 
    //R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MICROSECONDS);
    uint32_t reg_60620_0 = 0;
  reg_60620_0  = ether_phy_read(p_instance_ctrl, 0);
  
  //R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MICROSECONDS);
  switch (link_speed)
  {
    case ETHER_PHY_LINK_SPEED_NO_LINK:
      // do nothing  (Leave Strap options)
      reg_60620_0 = 0x0800;    // Set Power Down
      break; 
    
    case ETHER_PHY_AUTONEG: 
      reg_60620_0 = 0x01000;    // Enable Autoneg
      
      break; 
      
    case ETHER_PHY_LINK_SPEED_100F:
      reg_60620_0 &= ~(0x01800);    // Disable Autoneg and Powerdown
      reg_60620_0 |= 0x02100;       // Set Speed to 100 MBit FD
    break; 
    
    default: 
      break; 
  }
  ether_phy_write(p_instance_ctrl, 0, reg_60620_0 & (~0x04040));  // Disable Loopback, GBit for GBit Phys
    
    p_instance_ctrl->open = ETHER_PHY_OPEN;
#else
    /* Reset PHY */
    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_CONTROL, ETHER_PHY_CONTROL_RESET);

    /* Reset completion waiting */
    do
    {
        reg = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_CONTROL);
        count++;
    } while ((reg & ETHER_PHY_CONTROL_RESET) && (count < p_cfg->phy_reset_wait_time));

    if (count < p_cfg->phy_reset_wait_time)
    {
        ether_phy_targets_initialize (p_instance_ctrl);

        p_instance_ctrl->open = ETHER_PHY_OPEN;

        err = FSP_SUCCESS;
    }
    else
    {
        err = FSP_ERR_TIMEOUT;
    }
#endif

#ifdef PHY_FIXED_100M
    ether_phy_write(p_instance_ctrl, 
                    ETHER_PHY_REG_CONTROL,
                    (ETHER_PHY_CONTROL_100_MBPS|ETHER_PHY_CONTROL_FULL_DUPLEX));
#endif /* PHY_FIXED_100M */

    return err;
} /* End of function R_ETHER_PHY_Open() */

/********************************************************************************************************************//**
 * @brief Close Ethernet PHY device. Implements @ref ether_phy_api_t::close.
 *
 * @retval  FSP_SUCCESS                                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER_PHY control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened
 *
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_Close (ether_phy_ctrl_t * const p_ctrl)
{
    fsp_err_t               err    = FSP_SUCCESS;
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl;

#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif
    uint32_t regval = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_CONTROL);
    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_CONTROL, regval | 0x0800);    // Set powerdown bit

    /** Clear configure block parameters. */
    p_instance_ctrl->p_ether_phy_cfg = NULL;
    p_instance_ctrl->local_advertise = 0;
    p_instance_ctrl->p_reg_etherc    = 0;

    p_instance_ctrl->open = 0;

    return err;
} /* End of function R_ETHER_PHY_Close() */

/********************************************************************************************************************//**
 * @brief Starts auto-negotiate. Implements @ref ether_phy_api_t::startAutoNegotiate.
 *
 * @retval  FSP_SUCCESS                                 ETHER_PHY successfully starts auto-negotiate.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER_PHY control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened
 *
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_StartAutoNegotiate(ether_phy_ctrl_t * const p_ctrl)
{
#ifndef PHY_FIXED_100M
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl;

#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Set local ability */
    /* When pause frame is not used */
    if (ETHER_PHY_FLOW_CONTROL_DISABLE == p_instance_ctrl->p_ether_phy_cfg->flow_control)
    {
        p_instance_ctrl->local_advertise = ((((ETHER_PHY_AN_ADVERTISEMENT_100F |
        ETHER_PHY_AN_ADVERTISEMENT_100H) |
        ETHER_PHY_AN_ADVERTISEMENT_10F) |
        ETHER_PHY_AN_ADVERTISEMENT_10H) |
        ETHER_PHY_AN_ADVERTISEMENT_SELECTOR);

    }

    /* When pause frame is used */
    else
    {
        p_instance_ctrl->local_advertise = ((((((ETHER_PHY_AN_ADVERTISEMENT_ASM_DIR |
        ETHER_PHY_AN_ADVERTISEMENT_PAUSE) |
        ETHER_PHY_AN_ADVERTISEMENT_100F) |
        ETHER_PHY_AN_ADVERTISEMENT_100H) |
        ETHER_PHY_AN_ADVERTISEMENT_10F) |
        ETHER_PHY_AN_ADVERTISEMENT_10H) |
        ETHER_PHY_AN_ADVERTISEMENT_SELECTOR);
    }

    #ifdef PHY_100MBPS_LIMIT       /* RZ/T2 FPGA does not support 1000BT ! */
    {
        ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_1000BT_CONTROL, 0x0000);
    }
    #endif /* PHY_100MBPS_LIMIT */

    /* Configure what the PHY and the Ethernet controller on this board supports */
    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_AN_ADVERTISEMENT, p_instance_ctrl->local_advertise);
    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_CONTROL, (ETHER_PHY_CONTROL_AN_ENABLE |
            ETHER_PHY_CONTROL_AN_RESTART));

    ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_AN_ADVERTISEMENT);
#endif /* !PHY_FIXED_100M */

    return FSP_SUCCESS;
} /* End of function R_ETHER_PHY_StartAutoNegotiate() */

/********************************************************************************************************************//**
 * @brief Reports the other side's physical capability. Implements @ref ether_phy_api_t::linkPartnerAbilityGet.
 *
 * @retval  FSP_SUCCESS                                 ETHER_PHY successfully get link partner ability.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER_PHY control block is NULL.
 * @retval  FSP_ERR_INVALID_POINTER                     Pointer to arguments are NULL.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened
 * @retval  FSP_ERR_ETHER_PHY_ERROR_LINK                PHY-LSI is not link up.
 * @retval  FSP_ERR_ETHER_PHY_NOT_READY                 The auto-negotiation isn't completed
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_LinkPartnerAbilityGet(ether_phy_ctrl_t * const p_ctrl, uint32_t * const p_line_speed_duplex,
            uint32_t * const p_local_pause, uint32_t * const p_partner_pause)
{
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl;
    uint32_t reg;
    uint32_t reg_status;

#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    ETHER_PHY_ERROR_RETURN(NULL != p_line_speed_duplex, FSP_ERR_INVALID_POINTER);
    ETHER_PHY_ERROR_RETURN(NULL != p_local_pause, FSP_ERR_INVALID_POINTER);
    ETHER_PHY_ERROR_RETURN(NULL != p_partner_pause, FSP_ERR_INVALID_POINTER);
#endif

    *p_local_pause   = 0;
    *p_partner_pause = 0;

#ifdef PHY_FIXED_100M
    (*p_line_speed_duplex) = ETHER_PHY_LINK_SPEED_100F;
#else /* !PHY_FIXED_100M */
    /* Because reading the first time shows the previous state, the Link status bit is read twice. */
    ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_STATUS);
    reg_status = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_STATUS);

    /* When the link isn't up, return error */
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_STATUS_LINK_UP == (reg_status & ETHER_PHY_STATUS_LINK_UP), FSP_ERR_ETHER_PHY_ERROR_LINK);

    /* Establish local pause capability */
    if (ETHER_PHY_AN_ADVERTISEMENT_PAUSE == (p_instance_ctrl->local_advertise & ETHER_PHY_AN_ADVERTISEMENT_PAUSE))
    {
        (*p_local_pause) |= (1 << 1);
    }

    if (ETHER_PHY_AN_ADVERTISEMENT_ASM_DIR == (p_instance_ctrl->local_advertise & ETHER_PHY_AN_ADVERTISEMENT_ASM_DIR))
    {
        (*p_local_pause) |= 1;
    }

    /* When the auto-negotiation isn't completed, return error */
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_STATUS_AN_COMPLETE == (reg_status & ETHER_PHY_STATUS_AN_COMPLETE), FSP_ERR_ETHER_PHY_NOT_READY);

    /* Get the link partner response */
    reg = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_AN_LINK_PARTNER);

    /* Establish partner pause capability */
    if (ETHER_PHY_AN_LINK_PARTNER_PAUSE == (reg & ETHER_PHY_AN_LINK_PARTNER_PAUSE))
    {
        (*p_partner_pause) |= (1 << 1);
    }

    if (ETHER_PHY_AN_LINK_PARTNER_ASM_DIR == (reg & ETHER_PHY_AN_LINK_PARTNER_ASM_DIR))
    {
        (*p_partner_pause) |= 1;
    }

    /* Establish the line speed and the duplex */
    if (ETHER_PHY_AN_LINK_PARTNER_10H == (reg & ETHER_PHY_AN_LINK_PARTNER_10H))
    {
        (*p_line_speed_duplex) = ETHER_PHY_LINK_SPEED_10H;
    }

    if (ETHER_PHY_AN_LINK_PARTNER_10F == (reg & ETHER_PHY_AN_LINK_PARTNER_10F))
    {
        (*p_line_speed_duplex) = ETHER_PHY_LINK_SPEED_10F;
    }

    if (ETHER_PHY_AN_LINK_PARTNER_100H == (reg & ETHER_PHY_AN_LINK_PARTNER_100H))
    {
        (*p_line_speed_duplex) = ETHER_PHY_LINK_SPEED_100H;
    }

    if (ETHER_PHY_AN_LINK_PARTNER_100F == (reg & ETHER_PHY_AN_LINK_PARTNER_100F))
    {
        (*p_line_speed_duplex) = ETHER_PHY_LINK_SPEED_100F;
    }

    #ifndef PHY_100MBPS_LIMIT   /* RZ/T2 FPGA does not support 1000BT ! */
    if (reg_status & ETHER_PHY_STATUS_EX_STATUS)
    {
        /* Get 1000BASE-T Status */
        reg = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_1000BT_STATUS);

        if (ETHER_PHY_1000BT_STATUS_PARTNER_1000H == (reg & ETHER_PHY_1000BT_STATUS_PARTNER_1000H))
        {
            (*p_line_speed_duplex) = ETHER_PHY_LINK_SPEED_1000H;
        }

        if (ETHER_PHY_1000BT_STATUS_PARTNER_1000F == (reg & ETHER_PHY_1000BT_STATUS_PARTNER_1000F))
        {
            (*p_line_speed_duplex) = ETHER_PHY_LINK_SPEED_1000F;
        }
    }
    #endif /* !PHY_100MBPS_LIMIT */
#endif /* !PHY_FIXED_100M */

    return FSP_SUCCESS;

} /* End of function R_ETHER_PHY_LinkPartnerAbilityGet() */

/********************************************************************************************************************//**
 * @brief Returns the status of the physical link. Implements @ref ether_phy_api_t::linkStatusGet.
 *
 * @retval  FSP_SUCCESS                                 ETHER_PHY successfully get link partner ability.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER_PHY control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened
 * @retval  FSP_ERR_ETHER_PHY_ERROR_LINK                PHY-LSI is not link up.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_LinkStatusGet(ether_phy_ctrl_t * const p_ctrl)
{
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl;
    uint32_t reg;
    fsp_err_t err    = FSP_SUCCESS;

#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Because reading the first time shows the previous state, the Link status bit is read twice. */
    ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_STATUS);
    reg = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_STATUS);

    
    
    uint32_t reg_0 = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_CONTROL);
#ifdef PrintfDebug
		{
			char text_string[20];
			snprintf(&text_string[0], 20,  "Phy %d %x %x",p_instance_ctrl->p_ether_phy_cfg->channel,  reg_0, reg);
			printeth(text_string);
		}
#endif

    /* When the link isn't up, return error */
    if (ETHER_PHY_STATUS_LINK_UP != (reg & ETHER_PHY_STATUS_LINK_UP))
    {
        /* Link is down */
        err = FSP_ERR_ETHER_PHY_ERROR_LINK;
    }
    else
    {
        /* Link is up */
        err = FSP_SUCCESS;
    }

    return err;

} /* End of function R_ETHER_PHY_LinkStatusGet() */

/*******************************************************************************************************************//**
 * @} (end addtogroup ETHER_PHY)
 **********************************************************************************************************************/

/**
 * Private functions
 */
/***********************************************************************************************************************
 * Function Name: ether_phy_read
 * Description  : Reads a PHY register
 * Arguments    : ether_channel -
 *                    Ethernet channel number
 *                reg_addr -
 *                    address of the PHY register
 * Return Value : read value
 ***********************************************************************************************************************/
uint32_t ether_phy_read (ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_addr)
{
#ifdef ETHSW_MDIO_CONTROOL
    switch (p_instance_ctrl->p_ether_phy_cfg->channel)
    {
        case 0:
            return (ether_phy_read_gmac (p_instance_ctrl, reg_addr));
        case 1:
            {
                R_ETHSW_Type      * p_Reg_Ethsw = (R_ETHSW_Type *)p_instance_ctrl->p_reg_etherc;

                return (ether_phy_read_ethsw (p_instance_ctrl, reg_addr,
                            &p_Reg_Ethsw->MDIO_COMMAND,
                            &p_Reg_Ethsw->MDIO_DATA,
                            &p_Reg_Ethsw->MDIO_CFG_STATUS));
            }
        case 2:
            {
                R_ETHSW_Type      * p_Reg_Ethsw = (R_ETHSW_Type *)p_instance_ctrl->p_reg_etherc;

                return (ether_phy_read_ethsw (p_instance_ctrl, reg_addr,
                            &p_Reg_Ethsw->MDIO_COMMAND2,
                            &p_Reg_Ethsw->MDIO_DATA2,
                            &p_Reg_Ethsw->MDIO_CFG_STATUS2));
            }
        default:
            return (uint32_t)-1;
    }
#else /* !ETHSW_MDIO_CONTROOL */
    return (ether_phy_read_gmac (p_instance_ctrl, reg_addr));
#endif /* !ETHSW_MDIO_CONTROOL */
}

/***********************************************************************************************************************
 * Function Name: ether_phy_read (GMAC)
 * Description  : Reads a PHY register
 * Arguments    : ether_channel -
 *                    Ethernet channel number
 *                reg_addr -
 *                    address of the PHY register
 * Return Value : read value
 ***********************************************************************************************************************/
uint32_t ether_phy_read_gmac (ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_addr)
{
    volatile R_GMAC_Type *  p_reg_etherc = (R_GMAC_Type *)p_instance_ctrl->p_reg_etherc;

    uint32_t val_tmp;

    val_tmp = (p_instance_ctrl->p_ether_phy_cfg->phy_lsi_address << 11) | // Phy address
              ((reg_addr & 0x1f) << 6) |    // Phy reg address
              (0x4 << 2) |                  // Speed
              (0 << 1) |                    // Read
              (1 << 0);                     // Busy
    while (p_reg_etherc->GMII_Address & 0x01) {}  // Wait for busy bit to be cleared
    
    p_reg_etherc->GMII_Address = val_tmp;   // Write phy register

    while (1)
    { // Wait read complete
        if (!(p_reg_etherc->GMII_Address_b.GB))
        {
            return p_reg_etherc->GMII_Data & 0xffff;   // Return phy value
        }
    }
} /* End of function ether_phy_read_gmac() */

/***********************************************************************************************************************
 * Function Name: ether_phy_read (ETHSW)
 * Description  : Reads a PHY register
 * Arguments    : ether_channel -
 *                    Ethernet channel number
 *                reg_addr -
 *                    address of the PHY register
 * Return Value : read value
 ***********************************************************************************************************************/
#ifdef  ETHSW_MDIO_CONTROOL
uint32_t ether_phy_read_ethsw (ether_phy_instance_ctrl_t * p_instance_ctrl, 
                               uint32_t                    reg_addr,
                               volatile uint32_t         * p_Reg_Command,
                               volatile uint32_t         * p_Reg_Data,
                               volatile uint32_t         * p_Reg_Status )
{
    /* initialize and execute read command */
    *p_Reg_Command = (reg_addr & ETHSW_MDIO_COMMAND_REGADDR_MASK) |
                     ((p_instance_ctrl->p_ether_phy_cfg->phy_lsi_address << ETHSW_MDIO_COMMAND_PHYADDR_SHIFT) & ETHSW_MDIO_COMMAND_PHYADDR_MASK);
    *p_Reg_Command |= ETHSW_MDIO_COMMAND_TRANINIT_READ;

    /* wait until MDIO is ready */
    while(1)
    {
        if (ETHSW_MDIO_CFG_STATUS_BUSY != (*p_Reg_Status & ETHSW_MDIO_CFG_STATUS_BUSY))
        {
            if(ETHSW_MDIO_CFG_STATUS_READERR != (*p_Reg_Status & ETHSW_MDIO_CFG_STATUS_READERR))
            {
                return (*p_Reg_Data & 0xffff);   // Return phy value
            }
            break;
        }
    }
    return (uint32_t)-1;
} /* End of function ether_phy_read_ethsw() */
#endif /* ETHSW_MDIO_CONTROOL */

/***********************************************************************************************************************
 * Function Name: ether_phy_write
 * Description  : Writes to a PHY register
 * Arguments    : ether_channel -
 *                    Ethernet channel number
 *                reg_addr -
 *                    address of the PHY register
 *                data -
 *                    value
 * Return Value : none
 ***********************************************************************************************************************/
void ether_phy_write (ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_addr, uint32_t data)
{
#ifdef ETHSW_MDIO_CONTROOL
    switch (p_instance_ctrl->p_ether_phy_cfg->channel)
    {
        case 0:
            ether_phy_write_gmac (p_instance_ctrl, reg_addr, data);
            break;
        case 1:
            {
                R_ETHSW_Type      * p_Reg_Ethsw = (R_ETHSW_Type *)p_instance_ctrl->p_reg_etherc;

                ether_phy_write_ethsw (p_instance_ctrl, reg_addr, data,
                            &p_Reg_Ethsw->MDIO_COMMAND,
                            &p_Reg_Ethsw->MDIO_DATA,
                            &p_Reg_Ethsw->MDIO_CFG_STATUS);
                break;
            }
        case 2:
            {
                R_ETHSW_Type      * p_Reg_Ethsw = (R_ETHSW_Type *)p_instance_ctrl->p_reg_etherc;

                ether_phy_write_ethsw (p_instance_ctrl, reg_addr, data,
                            &p_Reg_Ethsw->MDIO_COMMAND2,
                            &p_Reg_Ethsw->MDIO_DATA2,
                            &p_Reg_Ethsw->MDIO_CFG_STATUS2);
                break;
            }
        default:
            break;
    }
#else /* !ETHSW_MDIO_CONTROOL */
    ether_phy_write_gmac (p_instance_ctrl, reg_addr, data);
#endif /* !ETHSW_MDIO_CONTROOL */
}

/***********************************************************************************************************************
 * Function Name: ether_phy_write (GMAC)
 * Description  : Writes to a PHY register
 * Arguments    : ether_channel -
 *                    Ethernet channel number
 *                reg_addr -
 *                    address of the PHY register
 *                data -
 *                    value
 * Return Value : none
 ***********************************************************************************************************************/
void ether_phy_write_gmac (ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_addr, uint32_t data)
{
    volatile R_GMAC_Type *  p_reg_etherc = (R_GMAC_Type *)p_instance_ctrl->p_reg_etherc;

    uint32_t val_tmp;

    while (1)
    { // wait to become ready
        if (!(p_reg_etherc->GMII_Address_b.GB))
        {
            break;
        }
    }

    
    val_tmp = (p_instance_ctrl->p_ether_phy_cfg->phy_lsi_address << 11) | // Phy address
              ((reg_addr & 0x1f) << 6) |    // Phy reg address
              (0x4 << 2) |                  // Speed
              (1 << 1) |                    // Read
              (1 << 0);                     // Busy

    p_reg_etherc->GMII_Data    = data;      // Set write value
    p_reg_etherc->GMII_Address = val_tmp;   // Write phy register

    while (1)
    { // wait write complete
        if (!(p_reg_etherc->GMII_Address_b.GB))
        {
            break;
        }
    }
} /* End of function ether_phy_write_gmac() */

/***********************************************************************************************************************
 * Function Name: ether_phy_write (ETHSW)
 * Description  : Writes to a PHY register
 * Arguments    : ether_channel -
 *                    Ethernet channel number
 *                reg_addr -
 *                    address of the PHY register
 *                data -
 *                    value
 * Return Value : none
 ***********************************************************************************************************************/
#ifdef  ETHSW_MDIO_CONTROOL
void ether_phy_write_ethsw (ether_phy_instance_ctrl_t * p_instance_ctrl, 
                            uint32_t                    reg_addr, 
                            uint32_t                    data,
                            volatile uint32_t         * p_Reg_Command,
                            volatile uint32_t         * p_Reg_Data,
                            volatile uint32_t         * p_Reg_Status )
{
    /* initialize and execute write command */
    *p_Reg_Command = (reg_addr & ETHSW_MDIO_COMMAND_REGADDR_MASK) |
                     ((p_instance_ctrl->p_ether_phy_cfg->phy_lsi_address << ETHSW_MDIO_COMMAND_PHYADDR_SHIFT) & ETHSW_MDIO_COMMAND_PHYADDR_MASK);

    *p_Reg_Data = data & 0xffff;

    /* wait until MDIO is ready */
    while(1)
    {
        if (ETHSW_MDIO_CFG_STATUS_BUSY != (*p_Reg_Status & ETHSW_MDIO_CFG_STATUS_BUSY))
        {
            break;
        }
    }
} /* End of function ether_phy_write_ethsw() */
#endif /* ETHSW_MDIO_CONTROOL */


#if ETHER_PHY_CFG_USE_PHY == ETHER_PHY_CFG_USE_PHY_AUTO
/***********************************************************************************************************************
 * Function Name: ether_phy_targets_initialize
 * Description  : PHY-LSI specific initialization processing
 * Arguments    : p_ctrl -
 *                    Ethernet channel number
 * Return Value : none
 ***********************************************************************************************************************/
#define RTL8211  0X01
#define RTL8211_PHYID2 0XC916
unsigned char phy_err =0;
void ether_phy_targets_initialize (ether_phy_instance_ctrl_t * pInstanceCtrl)
{
    uint32_t reg;
    uint16_t count = 0;
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t*)pInstanceCtrl;
#if RTL8211
    reg  = ether_phy_read(p_instance_ctrl, 0x03);
    while(reg != 0XC916)
    {
        ether_phy_write(p_instance_ctrl, 0x1F, 0xa42);
        reg  = ether_phy_read(p_instance_ctrl, 0x03);
        if (count > 1000) 
        { 
            count = 0;
            phy_err =1;
            R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_14_PIN_0, 0);
            break;
        }
        count++;
    }
    ether_phy_targets_initialize_rtl8211_rgmii(p_instance_ctrl);
#else
    reg  = ether_phy_read(p_instance_ctrl, 0x03);
    while ((reg != 0x0772) && (0x1640 != (reg & 0x0fff0)) && (reg != 0x2825)) {     // Mask out revision info
      reg  = ether_phy_read(p_instance_ctrl, 0x03);
      if (count > 1000) {             // It seems that is takes some time for the Interface to come up, so we need to repeat 
        // Seems like the wrong Phy Address, check for other; 
        if(p_instance_ctrl->p_ether_phy_cfg->phy_lsi_address < 0x4) {
          p_instance_ctrl->p_ether_phy_cfg->phy_lsi_address += 0x4;  // Older Boards have the PHYs at base address 0x100 
          count = 0; 
        }
        else {
          p_instance_ctrl->p_ether_phy_cfg->phy_lsi_address = 0x0;  // Older Boards have the PHYs at base address 0x100 
          reg = 0x0772;    // No Phy found use default PHY 
          break;   
          //return;   // No Phy found 
        }
      }  
      count++; 
      /* Delay 1ms */
      R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    }

    if (reg == 0x2825) {
#ifdef UPD60620_PHY
      ether_phy_targets_initialize_upd60620 (p_instance_ctrl);
#endif
    }
    else if (reg == 0x0772) {
      ether_phy_targets_initialize_vsc8541 (p_instance_ctrl);
    }
    else if (0x1620 == ( reg & 0x0fff0)) {   // mask out revision number
      ether_phy_targets_initialize_ksz9031 (p_instance_ctrl); // AG untested
    }
    else if (0x1640 == ( reg & 0x0fff0)) {   // mask out revision number
      ether_phy_targets_initialize_ksz9131mx (p_instance_ctrl); 
    }
    else if (0x1510 == ( reg & 0x0fff0)) {   // mask out revision number
      ether_phy_targets_initialize_ksz8041 (p_instance_ctrl);  // AG untested
    }
    else if (1906 == ( reg & 0x0fff0)) {   // mask out revision number
      ether_phy_targets_initialize_ksz8041 (p_instance_ctrl);  // AG untested
    }
    else
    {
    	while(1);
    }
#endif
} /* End of function ether_phy_targets_initialize() */

#endif //ETHER_PHY_CFG_USE_PHY == ETHER_PHY_CFG_USE_PHY_AUTO

