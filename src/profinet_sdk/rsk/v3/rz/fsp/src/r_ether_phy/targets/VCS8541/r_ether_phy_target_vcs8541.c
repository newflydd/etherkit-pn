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

/* Access to peripherals and board defines. */
#include "bsp_api.h"
//#include "r_ether_phy.h"
#include "r_ether_phy_rzt2.h"

#if ( (ETHER_PHY_CFG_USE_PHY == ETHER_PHY_CFG_USE_PHY_AUTO) || (ETHER_PHY_CFG_USE_PHY == ETHER_PHY_CFG_USE_PHY_VCS8541) )
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

/* Vendor Specific PHY Registers */
#define ETHER_PHY_REG_LED_MODE_SELECT                 (0x1D)
#define ETHER_PHY_REG_LED_BEHAVIOR                    (0x1E)
#define ETHER_PHY_REG_EXTEND_GPIO_PAGE                (0x1F)

#define ETHER_PHY_REG_LED0_MODE_SELECT_OFFSET         (0x0)
#define ETHER_PHY_REG_LED1_MODE_SELECT_OFFSET         (0x4)
#define ETHER_PHY_REG_LED0_FEATURE_DISABLE_OFFSET     (0x0)
#define ETHER_PHY_REG_LED1_FEATURE_DISABLE_OFFSET     (0x1)

#define ETHER_PHY_LEDMODE_ANYSPEED_LINK_ACTIVITY      (0x0)
#define ETHER_PHY_LEDMODE_1000M_LINK_ACTIVITY         (0x1)
#define ETHER_PHY_LEDMODE_100M_LINK_ACTIVITY          (0x2)
#define ETHER_PHY_LEDMODE_10M_LINK_ACTIVITY           (0x3)
#define ETHER_PHY_LEDMODE_100M_1000M_LINK_ACTIVITY    (0x4)
#define ETHER_PHY_LEDMODE_10M_100M_LINK_ACTIVITY      (0x5)
#define ETHER_PHY_LEDMODE_ACTIVITY      (10)
#define ETHER_PHY_LEDMODE_BIT_MASK                    (0xF)

/* Vendor Specific PHY Registers: Extended Page 2 */
#define ETHER_PHY_REG_EXPAGE2_RGMII_CTRL              (0x14)

#define ETHER_PHY_TXCLKDLY_OFFSET                     (0)
#define ETHER_PHY_RXCLKDLY_OFFSET                     (4)
#define ETHER_PHY_TXCLKDLY_MASK                       (0x7)
#define ETHER_PHY_RXCLKDLY_MASK                       (0x7)

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global function
 ***********************************************************************************************************************/
//void            ether_phy_targets_initialize(ether_phy_instance_ctrl_t * p_instance_ctrl);
extern uint32_t ether_phy_read(ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_addr);
extern void     ether_phy_write(ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_addr, uint32_t data);

/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: ether_phy_targets_initialize
 * Description  : PHY-LSI specific initialization processing
 * Arguments    : p_api_ctrl -
 *                    Ethernet channel number
 * Return Value : none
 ***********************************************************************************************************************/
#if(ETHER_PHY_CFG_USE_PHY == ETHER_PHY_CFG_USE_PHY_AUTO)
void ether_phy_targets_initialize_vsc8541 (ether_phy_instance_ctrl_t * p_instance_ctrl)
#else
void ether_phy_targets_initialize (ether_phy_instance_ctrl_t * p_instance_ctrl)
#endif
{
    uint32_t reg;
    __attribute__((unused))int read_err = 0;
#if 0
    for (int phyaddr = 0; phyaddr < 32; phyaddr++)
    {
        // reg = ether_phy_read(p_instance_ctrl, 3);
        R_ETHSW->MDIO_CFG_STATUS_b.HOLD = 7;

        R_ETHSW->MDIO_COMMAND_b.PHYADDR = phyaddr;

        /* Main registers */
        R_ETHSW->MDIO_COMMAND_b.REGADDR = 31;
        R_ETHSW->MDIO_DATA              = 0x0000;
        FSP_HARDWARE_REGISTER_WAIT(R_ETHSW->MDIO_CFG_STATUS_b.BUSY, 0);

        /* Read Status */
        R_ETHSW->MDIO_COMMAND_b.REGADDR  = 1;
        R_ETHSW->MDIO_COMMAND_b.TRANINIT = 1;
        FSP_HARDWARE_REGISTER_WAIT(R_ETHSW->MDIO_CFG_STATUS_b.BUSY, 0);

        if (0 != R_ETHSW->MDIO_CFG_STATUS_b.READERR)
        {
            read_err++;
            continue;
        }
        else
        {
            reg = R_ETHSW->MDIO_DATA;
        }

        /* model number */

        // reg = (reg >> 4) & 0x3f;
        if ((reg != 0) && (reg != 0xffff))
        {
            // break;
        }
    }
#endif
    
    /*
     * When VSC8541-02 of the Microsemi Corporation is used,
     * the pin that outputs the state of LINK is used combinedly with LINK/ACTIVITY in default.
     * The setting of the pin is changed so that only the state of LINK is output.
     * LED Mode Select (0x1D)
     * b7:4 = 0000b : LED1 mode any speed LINK/ACTIVITY
     * b3:0 = 0000b : LED0 mode any speed LINK/ACTIVITY
     * LED Behavior (0x1E)
     * b1 = 1 : LED1 combination feature disable (link only, duplex only)
     * b0 = 0 : LED0 combination feature enable  (link/activity)
     *//* LED Mode Select */
    reg  = ether_phy_read(p_instance_ctrl, 0x03);
    while (reg != 0x0772) {
      reg  = ether_phy_read(p_instance_ctrl, 0x03);
    }
    
    // LED 1 is used as link signal for Switch
    ether_phy_write(p_instance_ctrl,
                    ETHER_PHY_REG_LED_MODE_SELECT,
                    (ETHER_PHY_LEDMODE_ANYSPEED_LINK_ACTIVITY << ETHER_PHY_REG_LED1_MODE_SELECT_OFFSET) |
                    (ETHER_PHY_LEDMODE_ACTIVITY << ETHER_PHY_REG_LED0_MODE_SELECT_OFFSET));

    /* LED Bahavior */
    reg  = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_LED_BEHAVIOR);
    reg &= 0xFFFC;
    reg |= 1 << ETHER_PHY_REG_LED1_FEATURE_DISABLE_OFFSET | 1 << 14;  // Actrivity only for TX
    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_LED_BEHAVIOR, reg);

    reg  = ether_phy_read(p_instance_ctrl, 18);
    ether_phy_write(p_instance_ctrl, 18, reg & ~0x080);   // Clear MDI-X control bit to enable MDI-X Control
    ether_phy_write(p_instance_ctrl, 28, 0);   // // Disable ActiPhy 
    reg  = ether_phy_read(p_instance_ctrl, 28);    

#if 0
    if (ETHER_PHY_MII_TYPE_MII == p_instance_ctrl->p_ether_phy_cfg->mii_type)
    {
        /*
               This PHY uses a MII on the RSK board. 
         */
        //ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_PAGE_SELECT, 0x0006);
        //reg = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_14H);
        //if (0x800A == reg)
        //{
        //}
            ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_00H, 0x1000);    // Configure Ato negotiation
            ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_04H, 0x01e1);    // Configure Auto negotiation for 100 Base FD            
            ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_29H, 0x000A);    // Configure LED0 for Activity Configure LED1 for LINK/Activity   
            ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_30H, 0x0002);    // Configure LED1 for LINK only
            
//            ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_PAGE_SELECT, 0x0000);
    }
#endif
}                                      /* End of function ether_phy_targets_initialize() */

#endif /* ETHER_PHY_CFG_USE_PHY == ETHER_PHY_CFG_USE_PHY_VCS8541 */
