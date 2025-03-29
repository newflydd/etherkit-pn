/* generated configuration header file - do not edit */
#ifndef R_ETHER_PHY_CFG_H_
#define R_ETHER_PHY_CFG_H_

#define ETHER_PHY_CFG_USE_PHY_DP83620           1
#define ETHER_PHY_CFG_USE_PHY_KSZ8041           2
#define ETHER_PHY_CFG_USE_PHY_KSZ8091RNB        3
#define ETHER_PHY_CFG_USE_PHY_KSZ9031           4
#define ETHER_PHY_CFG_USE_PHY_KSZ9131RNX        5
#define ETHER_PHY_CFG_USE_PHY_VCS8541        6
#define ETHER_PHY_CFG_USE_PHY_AUTO        7   // Use this to automatically detect The PHY. Does not work on FPGA boards 


#define ETHER_PHY_CFG_PARAM_CHECKING_ENABLE (BSP_CFG_PARAM_CHECKING_ENABLE)
//#define ETHER_PHY_CFG_USE_PHY (BOARD_PHY_TYPE)
#define ETHER_PHY_CFG_USE_PHY (ETHER_PHY_CFG_USE_PHY_AUTO)
#define ETHER_PHY_CFG_USE_REF_CLK (1)

#define BSP_FEATURE_ETHER_PHY_MAX_CHANNELS  (3U)
#endif /* R_ETHER_PHY_CFG_H_ */
