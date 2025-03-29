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
//#include "r_ether_phy_rzt2.h"
#include "r_ether_phy_rzt2.h"

#if((ETHER_PHY_CFG_USE_PHY == ETHER_PHY_CFG_USE_PHY_AUTO) || (ETHER_PHY_CFG_USE_PHY == ETHER_PHY_CFG_USE_PHY_KSZ9131RNX) )

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

/* Vendor Specific PHY Registers */
#define ETHER_PHY_REG_MMD_ACCESS_CONTROL			(0x0D)
#define ETHER_PHY_REG_MMD_OPERATION_MODE_OFFSET		(0xE)
#define ETHER_PHY_REG_MMD_ACCESS_REGISTER_DATA		(0x0E)
#define ETHER_PHY_REG_KSZ9031_LED_MODE				(0x1A)
#define ETHER_PHY_REG_KSZ9031_LED_MODE_OFFSET		(0xE)
#define ETHER_PHY_MMDREG_COMMON_COTROL				(0x0200)
#define ETHER_PHY_MMDREG_SINGLE_LED_OFFSET			(0x4)
#define ETHER_PHY_MMDREG_MODE_STRAP_OVERRIDE		(0x0202)
#define ETHER_PHY_MMDREG_AN_ALL_NG_H_CAP_OFFSET		(0xE)
#define ETHER_PHY_REG_MMD_CONTROL    13
#define ETHER_PHY_REG_MMD_ADDR_DATA  14

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
void ether_phy_targets_initialize_ksz9131mx (ether_phy_instance_ctrl_t * p_instance_ctrl)
#else
void ether_phy_targets_initialize (ether_phy_instance_ctrl_t * p_instance_ctrl)
#endif
{
    uint32_t reg;
    __attribute__((unused))uint32_t mmd_device_address;
	  __attribute__((unused))uint32_t mmd_reg;

#if 0
     /*
     * When KSZ9131RNX of the Micrel, Inc. is used,
     * RGMMI mode which advertise all capabilities except 1000BASE-T half duplex is to be selected.
	 * MMD Device Addres - 2
	 * MMD Index         - 2
	 * an_all_ng_h_cap 2.2.14
	 * b14 = 1 : RGMII mode
     */
	mmd_device_address = ETHER_PHY_MMDREG_MODE_STRAP_OVERRIDE >> 8;
	mmd_reg = ETHER_PHY_MMDREG_MODE_STRAP_OVERRIDE & 0xff;
    ether_phy_write(p_instance_ctrl,
					ETHER_PHY_REG_MMD_ACCESS_CONTROL,
					(0 << ETHER_PHY_REG_MMD_OPERATION_MODE_OFFSET | mmd_device_address) );
    ether_phy_write(p_instance_ctrl,
					ETHER_PHY_REG_MMD_ACCESS_REGISTER_DATA,
					mmd_reg);
    ether_phy_write(p_instance_ctrl,
					ETHER_PHY_REG_MMD_ACCESS_CONTROL,
					(1 << ETHER_PHY_REG_MMD_OPERATION_MODE_OFFSET | mmd_device_address) );
    reg = 1 << ETHER_PHY_MMDREG_AN_ALL_NG_H_CAP_OFFSET;
    ether_phy_write(p_instance_ctrl,
					ETHER_PHY_REG_MMD_ACCESS_REGISTER_DATA,
					reg);
    reg  = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_MMD_ACCESS_REGISTER_DATA);
    /*
     * When KSZ9131RNX of the Micrel, Inc. is used,
     * the pin that outputs the state of LINK is used combinedly with LINK/ACTIVITY in default.
     * The setting of the LED2 pin is changed so that only the state of LINK is output.
	 * MMD Device Addres - 2
	 * MMD Index         - 0
	 * Single LED 2.0.4
	 * b4 = 1 : Invividual LED mode
	 * b4 = 0 : Tri-color LED mode
     */
	mmd_device_address = ETHER_PHY_MMDREG_COMMON_COTROL >> 8;
	mmd_reg = ETHER_PHY_MMDREG_COMMON_COTROL & 0xff;
    ether_phy_write(p_instance_ctrl,
					ETHER_PHY_REG_MMD_ACCESS_CONTROL,
					(0 << ETHER_PHY_REG_MMD_OPERATION_MODE_OFFSET | mmd_device_address) );
    ether_phy_write(p_instance_ctrl,
					ETHER_PHY_REG_MMD_ACCESS_REGISTER_DATA,
					mmd_reg);
    ether_phy_write(p_instance_ctrl,
					ETHER_PHY_REG_MMD_ACCESS_CONTROL,
					(1 << ETHER_PHY_REG_MMD_OPERATION_MODE_OFFSET | mmd_device_address) );
    reg  = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_MMD_ACCESS_REGISTER_DATA);
    reg |= 1 << ETHER_PHY_MMDREG_SINGLE_LED_OFFSET;
    ether_phy_write(p_instance_ctrl,
					ETHER_PHY_REG_MMD_ACCESS_REGISTER_DATA,
					reg);
    reg  = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_MMD_ACCESS_REGISTER_DATA);
#endif
  /* 2.77 TX DLL */
    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_MMD_CONTROL,   0x0002);  /* Select MMD Device Address */
    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_MMD_ADDR_DATA, 77);  /* MMD Register Address */
    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_MMD_CONTROL,   0x4002);  /* Select register data */

    reg = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_MMD_ADDR_DATA);
    reg &= ~(1 << 12);  /* Enable TX DLL */
    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_MMD_ADDR_DATA, (uint16_t)reg);

    /* Reset TX DLL */
    reg = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_MMD_ADDR_DATA);
    reg |= (1 << 13);   /* Set Reset bit */
    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_MMD_ADDR_DATA, (uint16_t)reg);
    reg = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_MMD_ADDR_DATA);
    reg &= ~(1 << 13);  /* Clear reset bit */
    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_MMD_ADDR_DATA, (uint16_t)reg);

}                                      /* End of function ether_phy_targets_initialize() */

#endif /* ETHER_PHY_CFG_USE_PHY == ETHER_PHY_CFG_USE_PHY_KSZ9131RNX */
