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
#ifdef BSP_PLAT_RZN1SDB /* RZ/N1 */
    #include "platform.h"
    #include "iodefine.h"
    #include "fsp_common_api.h"
    #include "r_ether_api_rzt2.h"
#else /* !BSP_PLAT_RZN1SDB (RZ/T2) */
    #include "bsp_api.h"
    #include "r_ether_api_rzt2.h"
#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/*CONVCTRL[m] register defines*/
#define CONVCTR_FULLD                   (1 << 8)

#define CONVCTR_CONV_MODE_MASK          (0x1f)  /* Mask of Converter operation mode */
#define CONVCTR_CONV_MODE_MII_MODE      (0x00)  /* MII mode (Through mode) */
#define CONVCTR_CONV_MODE_RMII_10M_IN   (0x04)  /* RMII mode 10Mbps REF_CLK input */
#define CONVCTR_CONV_MODE_RMII_100M_IN  (0x05)  /* RMII mode 100Mbps REF_CLK input */
#define CONVCTR_CONV_MODE_RMII_10M_OUT  (0x14)  /* RMII mode 10Mbps REF_CLK output */
#define CONVCTR_CONV_MODE_RMII_100M_OUT (0x15)  /* RMII mode 100Mbps REF_CLK output */
#define CONVCTR_CONV_MODE_RGMII_1G      (0x0a)  /* RGMII mode 1000Mbps */
#define CONVCTR_CONV_MODE_RGMII_100M    (0x09)  /* RGMII mode 100Mbps */
#define CONVCTR_CONV_MODE_RGMII_10M     (0x08)  /* RGMII mode 10Mbps */


/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * @brief Louck/Unlock the protection of ethernet access register
 *
 * @param[in] p_reg_base           Base addres for ethernet access register
 * @param[in] protect              The operational mode is specified.
 *                                      R_ETHER_ACCES_PROTECT_LOCK      - Lock the protection of ethernet access register
 *                                      R_ETHER_ACCES_PROTECT_UNLOCK    - Unlock the protection of ethernet access register
 * @retval  non
 *
 ***********************************************************************************************************************/
#ifdef BSP_PLAT_RZN1SDB /* RZ/N1 */

void R_ETHER_ACCESS_Protection (void * const p_reg_base, ether_access_protect_t const protect)
{
    volatile ETHACS_Type *p_reg_access = (volatile ETHACS_Type *)p_reg_base;

    if(R_ETHER_ACCES_PROTECT_UNLOCK == protect)
    {
        /* Unlock the protection of ethernet access register */
        p_reg_access->PRCMD.LONG = 0x00A5;
        p_reg_access->PRCMD.LONG = 1;
        p_reg_access->PRCMD.LONG = 0xFFFE;
        p_reg_access->PRCMD.LONG = 1;
    }
    else
    {
        /* Lock the protection of ethernet access register */
        p_reg_access->PRCMD.LONG = 0;
    }
}

#else /* !BSP_PLAT_RZN1SDB (RZ/T2) */

void R_ETHER_ACCESS_Protection (void * const p_reg_base, ether_access_protect_t const protect)
{
    R_ETHSS_Type *p_reg_access = (R_ETHSS_Type *)p_reg_base;
    
    if(R_ETHER_ACCES_PROTECT_UNLOCK == protect)
    {
        /* Unlock the protection of ethernet access register */
        while(1)
        {
          p_reg_access->PRCMD = 0x00A5;
          p_reg_access->PRCMD = 1;
          p_reg_access->PRCMD = 0xFFFE;
          p_reg_access->PRCMD = 1;
          __asm("dmb");
          __asm("dsb");
          /* Write protection release check */
          if(p_reg_access->PRCMD == 1)
          {
            break;
          }
        }
    }
    else
    {
        /* Lock the protection of ethernet access register */
        while(1)
        {
          p_reg_access->PRCMD = 0;
          __asm("dmb");
          __asm("dsb");
          /* Write protection check */
          if(p_reg_access->PRCMD == 0)
          {
            break;
          }
        }
    }
}

#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */



uint32_t conv_reg; 
/***********************************************************************************************************************
 * @brief Initilize ethernet access register
 *
 * @param[in] p_reg_base           Base addres for ethernet access register
 * @param[in] link_control         Link Status(bit0:Port0, bit1:Port1, ..)
 *                                    0:Active High, 1:Active Low
 * @retval  non
 *
 ***********************************************************************************************************************/
#ifdef BSP_PLAT_RZN1SDB /* RZ/N1 */

#define PHYLNK_SWLNK_MASK   0x0F

#define SWLNK_P0            0x1
#define SWLNK_P1            0x2
#define SWLNK_P2            0x4

#define LINK_CONTROL_P0     0x1
#define LINK_CONTROL_P1     0x2
#define LINK_CONTROL_P2     0x4

void R_ETHER_ACCESS_Initilize (void * const p_reg_base, uint32_t const link_control)
{
    volatile ETHACS_Type * p_reg_access = (volatile ETHACS_Type *)p_reg_base;
    uint32_t               swlnk;

    R_ETHER_ACCESS_Protection (p_reg_base, R_ETHER_ACCES_PROTECT_UNLOCK);

    swlnk = 0;
    if (0 == (link_control & LINK_CONTROL_P0))
    {
        swlnk |= SWLNK_P0;
    }
    if (0 == (link_control & LINK_CONTROL_P1))
    {
        swlnk |= SWLNK_P1;
    }
    if (0 == (link_control & LINK_CONTROL_P2))
    {
        swlnk |= SWLNK_P2;
    }
    p_reg_access->PHYLNK.BIT.SWLNK = swlnk;

    p_reg_access->SWCTRL.LONG = 0;              /* SwitchCore Control Register */
    p_reg_access->SWDUPC.LONG = 0;              /* SwitchCore Duplex Mode Register */

    R_ETHER_ACCESS_Protection (p_reg_base, R_ETHER_ACCES_PROTECT_LOCK);
}

#else /* !BSP_PLAT_RZN1SDB (RZ/T2) */

#define PHYLNK_SWLNK_MASK  0x07

void R_ETHER_ACCESS_Initilize (void * const p_reg_base, uint32_t const link_control)
{
    R_ETHSS_Type * p_reg_access = (R_ETHSS_Type *)p_reg_base;

    R_ETHER_ACCESS_Protection (p_reg_base, R_ETHER_ACCES_PROTECT_UNLOCK);


    p_reg_access->MODCTRL_b.SW_MODE = 0;        /* switch_mux_mode */

                                                /* MTIP SwitchCore Link signal control */
//    p_reg_access->PHYLNK_b.SWLNK = ~(link_control & PHYLNK_SWLNK_MASK);
    p_reg_access->PHYLNK_b.SWLINK = (link_control & PHYLNK_SWLNK_MASK);
    
    p_reg_access->SWCTRL = 0;                   /* SwitchCore Control Register */
    p_reg_access->SWDUPC = 0;                   /* SwitchCore Duplex Mode Register */
    {
        uint32_t    convctrl;

        convctrl = p_reg_access->CONVCTRL[0];
        convctrl &= ~(CONVCTR_FULLD|CONVCTR_CONV_MODE_MASK);
#ifdef UPD60620_PHY_MII
        convctrl |= CONVCTR_CONV_MODE_MII_MODE|CONVCTR_FULLD;
#else
        convctrl |= CONVCTR_CONV_MODE_RGMII_100M|CONVCTR_FULLD;
#endif
        //        convctrl |= CONVCTR_CONV_MODE_RGMII_1G|CONVCTR_FULLD;
        p_reg_access->CONVCTRL[0] = convctrl;

        convctrl = p_reg_access->CONVCTRL[1];
        convctrl &= ~(CONVCTR_FULLD|CONVCTR_CONV_MODE_MASK);
#ifdef UPD60620_PHY_MII
        convctrl |= CONVCTR_CONV_MODE_MII_MODE|CONVCTR_FULLD;
#else
        convctrl |= CONVCTR_CONV_MODE_RGMII_100M|CONVCTR_FULLD;
#endif
//        convctrl |= CONVCTR_CONV_MODE_RGMII_1G|CONVCTR_FULLD;
        p_reg_access->CONVCTRL[1] = convctrl;

        convctrl = p_reg_access->CONVCTRL[2];
        convctrl &= ~(CONVCTR_FULLD|CONVCTR_CONV_MODE_MASK);
#ifdef UPD60620_PHY_MII
        convctrl |= CONVCTR_CONV_MODE_MII_MODE|CONVCTR_FULLD;
#else
        convctrl |= CONVCTR_CONV_MODE_RGMII_100M|CONVCTR_FULLD;
#endif
//        convctrl |= CONVCTR_CONV_MODE_RGMII_1G|CONVCTR_FULLD;
        p_reg_access->CONVCTRL[2] = convctrl;
    }

    p_reg_access->CONVRST_b.PHYIR = 0x0;   /* Assert RESET */
    R_BSP_SoftwareDelay (200, BSP_DELAY_UNITS_MICROSECONDS);
    p_reg_access->CONVRST_b.PHYIR = 0x7;   /* Release RESET */
conv_reg = p_reg_access->CONVCTRL[0]; 

    R_ETHER_ACCESS_Protection (p_reg_base, R_ETHER_ACCES_PROTECT_LOCK);
}

#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */

/***********************************************************************************************************************
 * @brief Config the speed and the duplex to CONVCTROL in ethernet access register
 *
 * @param[in] port              Port number
 * @param[in] speed_duplex      Speed and Duplex
 *                                  ETHER_PHY_LINK_SPEED_10H    - 10Mbps, Half-duplex
 *                                  ETHER_PHY_LINK_SPEED_10F    - 10Mbps, Full-duplex
 *                                  ETHER_PHY_LINK_SPEED_100H   - 100Mbps, Half-duplex
 *                                  ETHER_PHY_LINK_SPEED_100F   - 100Mbps, Full-duplex
 *                                  ETHER_PHY_LINK_SPEED_1000H  - 1000Mbps, Half-duplex
 *                                  ETHER_PHY_LINK_SPEED_1000F  - 1000Mbps, Full-duplex
 * @retval  non
 *
 ***********************************************************************************************************************/
#ifdef BSP_PLAT_RZN1SDB /* RZ/N1 */

fsp_err_t R_ETHER_ACCESS_Set_convctrl (void * const p_reg_base, uint32_t const port, uint32_t const speed_duplex)
{
    fsp_err_t              err          = FSP_SUCCESS;
    volatile ETHACS_Type * p_reg_access = (volatile ETHACS_Type *)p_reg_base;
    volatile uint32_t    * p_convctrl;
    uint32_t               value;

#if (ETHER_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_reg_base);
#endif

    switch (port)
    {
        case 0:
            p_convctrl = (volatile uint32_t *)&p_reg_access->CONVCTRL5.LONG;
            break;

        case 1:
            p_convctrl = (volatile uint32_t *)&p_reg_access->CONVCTRL4.LONG;
            break;

        case 2:
            p_convctrl = (volatile uint32_t *)&p_reg_access->CONVCTRL3.LONG;
            break;

        default:
            err = FSP_ERR_INVALID_ARGUMENT;
            break;
    }

    if (err == FSP_SUCCESS)
    {
        if ((port == 0) || (port == 1))
        {
            value = CONVCTR_CONV_MODE_MII_MODE;
        }
        else
        {
            switch (speed_duplex)
            {
                case ETHER_PHY_LINK_SPEED_10H:
                    value = (CONVCTR_CONV_MODE_RGMII_10M);
                    break;

                case ETHER_PHY_LINK_SPEED_10F:
                    value = (CONVCTR_CONV_MODE_RGMII_10M|CONVCTR_FULLD);
                    break;

                case ETHER_PHY_LINK_SPEED_100H:
                    value = (CONVCTR_CONV_MODE_RGMII_100M);
                    break;

                case ETHER_PHY_LINK_SPEED_100F:
                    value = (CONVCTR_CONV_MODE_RGMII_100M|CONVCTR_FULLD);
                    break;

                case ETHER_PHY_LINK_SPEED_1000H:
                    value = (CONVCTR_CONV_MODE_RGMII_1G);
                    break;

                case ETHER_PHY_LINK_SPEED_1000F:
                    value = (CONVCTR_CONV_MODE_RGMII_1G|CONVCTR_FULLD);
                    break;

                default:
                    err = FSP_ERR_INVALID_ARGUMENT;
                    break;
            }
        }
    }

    if (err == FSP_SUCCESS)
    {
        R_ETHER_ACCESS_Protection (p_reg_base, R_ETHER_ACCES_PROTECT_UNLOCK);

        *p_convctrl &= ~(CONVCTR_FULLD|CONVCTR_CONV_MODE_MASK);
        *p_convctrl |= value;

        R_ETHER_ACCESS_Protection (p_reg_base, R_ETHER_ACCES_PROTECT_LOCK);
    }

    return err;
}

#else /* !BSP_PLAT_RZN1SDB (RZ/T2) */

fsp_err_t R_ETHER_ACCESS_Set_convctrl (void * const p_reg_base, uint32_t const port, uint32_t const speed_duplex)
{
    fsp_err_t           err          = FSP_SUCCESS;
    R_ETHSS_Type    * p_reg_access = (R_ETHSS_Type *)p_reg_base;
    volatile uint32_t * p_convctrl;
    uint32_t            value;

#if (ETHER_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_reg_base);
#endif

    switch (port)
    {
        case 0:
            p_convctrl = (volatile uint32_t *)&p_reg_access->CONVCTRL[0];
            break;

        case 1:
            p_convctrl = (volatile uint32_t *)&p_reg_access->CONVCTRL[1];
            break;

        case 2:
            p_convctrl = (volatile uint32_t *)&p_reg_access->CONVCTRL[2];
            break;

        default:
            err = FSP_ERR_INVALID_ARGUMENT;
            break;
    }

    if (err == FSP_SUCCESS)
    {
        switch (speed_duplex)
        {
            case ETHER_PHY_LINK_SPEED_10H:
                value = (CONVCTR_CONV_MODE_RGMII_10M);
                break;

            case ETHER_PHY_LINK_SPEED_10F:
                value = (CONVCTR_CONV_MODE_RGMII_10M|CONVCTR_FULLD);
                break;

            case ETHER_PHY_LINK_SPEED_100H:
              #ifdef UPD60620_PHY_MII
                value = (CONVCTR_CONV_MODE_MII_MODE);
#else
                value = (CONVCTR_CONV_MODE_RGMII_100M);
#endif
                break;

            case ETHER_PHY_LINK_SPEED_100F:
              #ifdef UPD60620_PHY_MII
                value = (CONVCTR_CONV_MODE_MII_MODE|CONVCTR_FULLD);
#else  
                value = (CONVCTR_CONV_MODE_RGMII_100M|CONVCTR_FULLD);
#endif
                break;

            case ETHER_PHY_LINK_SPEED_1000H:
                value = (CONVCTR_CONV_MODE_RGMII_1G);
                break;

            case ETHER_PHY_LINK_SPEED_1000F:
                value = (CONVCTR_CONV_MODE_RGMII_1G|CONVCTR_FULLD);
                break;

            default:
                err = FSP_ERR_INVALID_ARGUMENT;
                break;
        }
    }

    if (err == FSP_SUCCESS)
    {
        R_ETHER_ACCESS_Protection (p_reg_base, R_ETHER_ACCES_PROTECT_UNLOCK);

        *p_convctrl &= ~(CONVCTR_FULLD|CONVCTR_CONV_MODE_MASK);
        *p_convctrl |= value;

        R_ETHER_ACCESS_Protection (p_reg_base, R_ETHER_ACCES_PROTECT_LOCK);
    }

    return err;
}

#endif /* !BSP_PLAT_RZN1SDB (RZ/T2) */
