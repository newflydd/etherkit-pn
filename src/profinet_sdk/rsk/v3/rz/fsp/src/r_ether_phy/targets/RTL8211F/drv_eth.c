//#include "hal_data.h"
//#include "r_ether_phy.h"
#include "r_ether_phy_rzt2.h"

extern uint32_t ether_phy_read(ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_addr);
extern void     ether_phy_write(ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_addr, uint32_t data);
// ID2  0X03  0XC916
static int phy_rtl8211f_led_fixup(ether_phy_instance_ctrl_t *phydev)
{
#define RTL_8211F_PAGE_SELECT 0x1F
#define RTL_8211F_EEELCR_ADDR 0x11
#define RTL_8211F_LED_PAGE 0xD04
#define RTL_8211F_LCR_ADDR 0x10

    uint32_t val1, val2 = 0;

    /* switch to led page */
    ether_phy_write(phydev, RTL_8211F_PAGE_SELECT, RTL_8211F_LED_PAGE);

    /* set led1(green) Link 10/100/1000M, and set led2(yellow) Link 10/100/1000M+Active */
    //ether_phy_read(phydev, RTL_8211F_LCR_ADDR, &val1);
    val1 = ether_phy_read(phydev, RTL_8211F_LCR_ADDR);
    val1 |= (1 << 5);
    val1 |= (1 << 8);
    val1 &= (uint32_t)(~(1 << 9));
    val1 |= (1 << 10);
    val1 |= (1 << 11);
    ether_phy_write(phydev, RTL_8211F_LCR_ADDR, val1);

    /* set led1(green) EEE LED function disabled so it can keep on when linked */
    //ether_phy_read(phydev, RTL_8211F_EEELCR_ADDR, &val2);
    val2 = ether_phy_read(phydev, RTL_8211F_EEELCR_ADDR);
    val2 &= (uint32_t)(~(1 << 2));
    ether_phy_write(phydev, RTL_8211F_EEELCR_ADDR, val2);

    /* switch back to page0 */
    ether_phy_write(phydev, RTL_8211F_PAGE_SELECT, 0xa42);

    return 0;
}

void ether_phy_targets_initialize_rtl8211_rgmii(ether_phy_instance_ctrl_t *p_instance_ctrl)
{
	R_BSP_SoftwareDelay(100, 1000);
    phy_rtl8211f_led_fixup(p_instance_ctrl);
}
