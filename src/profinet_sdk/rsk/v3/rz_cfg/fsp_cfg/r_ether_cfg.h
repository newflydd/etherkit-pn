/* generated configuration header file - do not edit */
#ifndef R_ETHER_CFG_H_
#define R_ETHER_CFG_H_

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

#define ETHER_CFG_PARAM_CHECKING_ENABLE (BSP_CFG_PARAM_CHECKING_ENABLE)

#define ETHER_CFG_LINK_PRESENT0 (1)
#define ETHER_CFG_LINK_PRESENT1 (1)
#define ETHER_CFG_LINK_PRESENT2 (1)

#define ETHER_CFG_USE_LINKSTA   (0)     /* default:0 */

#define ETHER_CFG_MAX_PORTS     (3)     /* default:3 */

#define BSP_FEATURE_ETHER_MAX_CHANNELS  (1U)

#define NUM_RX_DESCRIPTOR       (10)     /* default:8 */
#define NUM_TX_DESCRIPTOR       (16)     /* default:8 */

#define ETHER_BUFFER_SIZE       (1536)  /* default:2048 */

#define DEFAULT_INT_PRIORITY    (7u)

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif /* R_ETHER_CFG_H_ */
