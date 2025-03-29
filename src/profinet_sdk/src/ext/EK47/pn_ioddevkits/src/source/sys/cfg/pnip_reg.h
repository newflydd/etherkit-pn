//////////////////////////////////////////////////////////////////////
////  Register Include File
//// Version:           <revision>
//// Version RSL-Tools: 0.7.0
//// Erstellungsdatum:  17.03.2011
////
//////////////////////////////////////////////////////////////////////
#ifndef __PNIP_REG_H__
#define __PNIP_REG_H__

// ------------------------------------------
// Makros for access to peripheral registers
// ------------------------------------------
#define REG8(x)  (*(unsigned char  volatile *)(x))
#define REG16(x) (*(unsigned short volatile *)(x))
#define REG32(x) (*(unsigned long  volatile *)(x))

//////////////////////////////////////////////////////////////////////
//Registers for Module ERTEC200P_TOP
//////////////////////////////////////////////////////////////////////
///
#define PNIP_DEV_TOP_BASE 0x00000000

#define U_PNIP__IP_VERSION                                                        (0x10600000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IP_VERSION__CONFIGURATION                                         (0xFFFF0000)
#define U_PNIP__IP_VERSION__VERSION                                               (0x0000FF00)
#define U_PNIP__IP_VERSION__DEBUG_VERSION                                         (0x000000FF)
#define U_PNIP__IP_VERSION_RESET__VALUE                          0x10100
#define U_PNIP__IP_DEVELOPMENT                                                    (0x10600004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IP_DEVELOPMENT__IDENTIFICATION                                    (0xFFE00000)
#define U_PNIP__IP_DEVELOPMENT__TYPE                                              (0x00180000)
#define U_PNIP__IP_DEVELOPMENT__LABEL_STRUCTURE                                   (0x00070000)
#define U_PNIP__IP_DEVELOPMENT__INKR                                              (0x0000F800)
#define U_PNIP__IP_DEVELOPMENT__R_LABEL                                           (0x000007FF)
#define U_PNIP__IP_DEVELOPMENT_RESET__VALUE                      0x4A82088
#define U_PNIP__INITDONE                                                          (0x10600100 + PNIP_DEV_TOP_BASE)
#define U_PNIP__INITDONE__API_DONE                                                (0x00000080)
#define U_PNIP__INITDONE__SYNC_DONE                                               (0x00000040)
#define U_PNIP__INITDONE__CMD_DONE                                                (0x00000020)
#define U_PNIP__INITDONE__PACK_DONE                                               (0x00000010)
#define U_PNIP__INITDONE__STATISTIC_DONE                                          (0x00000008)
#define U_PNIP__INITDONE__QUEUE_DONE                                              (0x00000004)
#define U_PNIP__INITDONE__VLAN_DONE                                               (0x00000002)
#define U_PNIP__INITDONE__FDB_DONE                                                (0x00000001)
#define U_PNIP__INITDONE_RESET__VALUE                            0x0
#define U_PNIP__EDC_ERROR                                                         (0x10600104 + PNIP_DEV_TOP_BASE)
#define U_PNIP__EDC_ERROR__SYNC_2B                                                (0x02000000)
#define U_PNIP__EDC_ERROR__CMD_2B                                                 (0x01000000)
#define U_PNIP__EDC_ERROR__APICTRL_2B                                             (0x00800000)
#define U_PNIP__EDC_ERROR__STATISTIC_2B                                           (0x00400000)
#define U_PNIP__EDC_ERROR__PACKDATA_2B                                            (0x00100000)
#define U_PNIP__EDC_ERROR__QUEUECTRL_2B                                           (0x00080000)
#define U_PNIP__EDC_ERROR__QUEUEDATA_2B                                           (0x00040000)
#define U_PNIP__EDC_ERROR__VLAN_2B                                                (0x00020000)
#define U_PNIP__EDC_ERROR__FDB_2B                                                 (0x00010000)
#define U_PNIP__EDC_ERROR__SYNC_1B                                                (0x00000200)
#define U_PNIP__EDC_ERROR__CMD_1B                                                 (0x00000100)
#define U_PNIP__EDC_ERROR__APICTRL_1B                                             (0x00000080)
#define U_PNIP__EDC_ERROR__STATISTIC_1B                                           (0x00000040)
#define U_PNIP__EDC_ERROR__PACKDATA_1B                                            (0x00000010)
#define U_PNIP__EDC_ERROR__QUEUECTRL_1B                                           (0x00000008)
#define U_PNIP__EDC_ERROR__QUEUEDATA_1B                                           (0x00000004)
#define U_PNIP__EDC_ERROR__VLAN_1B                                                (0x00000002)
#define U_PNIP__EDC_ERROR__FDB_1B                                                 (0x00000001)
#define U_PNIP__EDC_ERROR_RESET__VALUE                           0x0
#define U_PNIP__BRIDGEDELAY                                                       (0x10600200 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BRIDGEDELAY__BRIDGEDELAY_1G                                       (0xFFFF0000)
#define U_PNIP__BRIDGEDELAY__BRIDGEDELAY_100M                                     (0x0000FFFF)
#define U_PNIP__BRIDGEDELAY_RESET__VALUE                         0x2600798
#define U_PNIP__SAFETYMARGIN                                                      (0x10600204 + PNIP_DEV_TOP_BASE)
#define U_PNIP__SAFETYMARGIN__SAFETYMARGIN_1G                                     (0x01FF0000)
#define U_PNIP__SAFETYMARGIN__SAFETYMARGIN_100M                                   (0x00000FFF)
#define U_PNIP__SAFETYMARGIN_RESET__VALUE                        0x0
#define U_PNIP__IRT_DELAYSAFETY                                                   (0x10600208 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRT_DELAYSAFETY__SAFETY_TIME_1G                                   (0x03FF0000)
#define U_PNIP__IRT_DELAYSAFETY__SAFETY_TIME_100M                                 (0x00001FFF)
#define U_PNIP__IRT_DELAYSAFETY_RESET__VALUE                     0x2A01A40
#define U_PNIP__FRAGMENTATION_CONTROL                                             (0x10600300 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAGMENTATION_CONTROL__DEBUG                                      (0x0F000000)
#define U_PNIP__FRAGMENTATION_CONTROL__MIN_FRAMELENGTH                            (0x000007FF)
#define U_PNIP__FRAGMENTATION_CONTROL_RESET__VALUE               0x66
#define U_PNIP__FRAGMENTATION_HEADER                                              (0x10600304 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAGMENTATION_HEADER__FRAGID                                      (0xFFFF0000)
#define U_PNIP__FRAGMENTATION_HEADER__FRAGTYPE                                    (0x0000FFFF)
#define U_PNIP__FRAGMENTATION_HEADER_RESET__VALUE                0xFF808892
#define U_PNIP__FRAGMENTATION_STATUS                                              (0x10600308 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAGMENTATION_STATUS__FRAGSTATUS_P4                               (0x0F000000)
#define U_PNIP__FRAGMENTATION_STATUS__FRAGSTATUS_P3                               (0x000F0000)
#define U_PNIP__FRAGMENTATION_STATUS__FRAGSTATUS_P2                               (0x00000F00)
#define U_PNIP__FRAGMENTATION_STATUS__FRAGSTATUS_P1                               (0x0000000F)
#define U_PNIP__FRAGMENTATION_STATUS_RESET__VALUE                0x0
#define U_PNIP__FRAGMENTATION_DEBUG                                               (0x1060030C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAGMENTATION_DEBUG__FRAGLENGTH                                   (0xFF000000)
#define U_PNIP__FRAGMENTATION_DEBUG__FRAGNO                                       (0x00FF0000)
#define U_PNIP__FRAGMENTATION_DEBUG__FRAMENO                                      (0x00000F00)
#define U_PNIP__FRAGMENTATION_DEBUG__FRAGDEFRAG_ACTIV                             (0x00000001)
#define U_PNIP__FRAGMENTATION_DEBUG_RESET__VALUE                 0x0
#define U_PNIP__PHASETRANSFER_STATUS                                              (0x10600400 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHASETRANSFER_STATUS__TXPORT                                      (0x0000000F)
#define U_PNIP__PHASETRANSFER_STATUS_RESET__VALUE                0x0
#define U_PNIP__PHASECROSSED                                                      (0x10600404 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHASECROSSED__PHASECROSSED_P4                                     (0x00000008)
#define U_PNIP__PHASECROSSED__PHASECROSSED_P3                                     (0x00000004)
#define U_PNIP__PHASECROSSED__PHASECROSSED_P2                                     (0x00000002)
#define U_PNIP__PHASECROSSED__PHASECROSSED_P1                                     (0x00000001)
#define U_PNIP__PHASECROSSED_RESET__VALUE                        0x0
#define U_PNIP__PORT_GROUPING                                                     (0x10600500 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PORT_GROUPING__GROUPING_P4                                        (0x00000008)
#define U_PNIP__PORT_GROUPING__GROUPING_P3                                        (0x00000004)
#define U_PNIP__PORT_GROUPING__GROUPING_P2                                        (0x00000002)
#define U_PNIP__PORT_GROUPING__GROUPING_P1                                        (0x00000001)
#define U_PNIP__PORT_GROUPING_RESET__VALUE                       0x0
#define U_PNIP__QUEUETYPE_SELECT                                                  (0x10600600 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUETYPE_SELECT__APITYPE_VALID15                                 (0x00100000)
#define U_PNIP__QUEUETYPE_SELECT__APITYPE_QUEUE15                                 (0x000F0000)
#define U_PNIP__QUEUETYPE_SELECT__APITYPE_VALID14                                 (0x00000010)
#define U_PNIP__QUEUETYPE_SELECT__APITYPE_QUEUE14                                 (0x0000000F)
#define U_PNIP__QUEUETYPE_SELECT_RESET__VALUE                    0x0
#define U_PNIP__QUEUE14_MAPPING                                                   (0x10600604 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUE14_MAPPING__MAPPING_ENABLE                                   (0x00000010)
#define U_PNIP__QUEUE14_MAPPING__MAPPING_PRIO                                     (0x0000000F)
#define U_PNIP__QUEUE14_MAPPING_RESET__VALUE                     0x0
#define U_PNIP__DEBUG_CONTROL                                                     (0x10600F00 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DEBUG_CONTROL__DEBUGSELECT                                        (0x0000003F)
#define U_PNIP__DEBUG_CONTROL_RESET__VALUE                       0x0
#define U_PNIP__DEBUG_STATUS                                                      (0x10600F04 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DEBUG_STATUS__DEBUGSTATE                                          (0x7FFFFFFF)
#define U_PNIP__DEBUG_STATUS_RESET__VALUE                        0x0
#define U_PNIP__PORTCONTROL_P1                                                    (0x10601000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PORTCONTROL_P1__DISABLE_MACTXERR                                  (0x80000000)
#define U_PNIP__PORTCONTROL_P1__SUPERFASTFORWARDING                               (0x00000100)
#define U_PNIP__PORTCONTROL_P1__TX_REDGUARD_DP2                                   (0x00000080)
#define U_PNIP__PORTCONTROL_P1__PTCP_FWD_ENABLE                                   (0x00000040)
#define U_PNIP__PORTCONTROL_P1__PTCP_FWD_DELAY                                    (0x00000020)
#define U_PNIP__PORTCONTROL_P1__FASTFORWARDING                                    (0x00000010)
#define U_PNIP__PORTCONTROL_P1__TXD_FRAGMENTATION                                 (0x00000008)
#define U_PNIP__PORTCONTROL_P1__RXD_FRAGMENTATION                                 (0x00000004)
#define U_PNIP__PORTCONTROL_P1__FORWARDING_MODE                                   (0x00000002)
#define U_PNIP__PORTCONTROL_P1__TRANSFERRATE                                      (0x00000001)
#define U_PNIP__PORTCONTROL_P1_RESET__VALUE                      0x44
#define U_PNIP__FLOWCONTROL_P1                                                    (0x10601004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FLOWCONTROL_P1__TXBOUNDARY_RED2GREEN_DP2                          (0x04000000)
#define U_PNIP__FLOWCONTROL_P1__TXBOUNDARY_DP2                                    (0x02000000)
#define U_PNIP__FLOWCONTROL_P1__RXBOUNDARY_DP2                                    (0x01000000)
#define U_PNIP__FLOWCONTROL_P1__RTLOWBOUNDARY_DP1S                                (0x00080000)
#define U_PNIP__FLOWCONTROL_P1__RTHIGHBOUNDARY_DP1S                               (0x00040000)
#define U_PNIP__FLOWCONTROL_P1__TXBOUNDARY_DP1S                                   (0x00020000)
#define U_PNIP__FLOWCONTROL_P1__RXBOUNDARY_DP1S                                   (0x00010000)
#define U_PNIP__FLOWCONTROL_P1__RTLOWBOUNDARY_DP1_R                               (0x00000800)
#define U_PNIP__FLOWCONTROL_P1__RTHIGHBOUNDARY_DP1_R                              (0x00000400)
#define U_PNIP__FLOWCONTROL_P1__TXBOUNDARY_DP1_R                                  (0x00000200)
#define U_PNIP__FLOWCONTROL_P1__RXBOUNDARY_DP1_R                                  (0x00000100)
#define U_PNIP__FLOWCONTROL_P1__DISABLEPORT                                       (0x00000001)
#define U_PNIP__FLOWCONTROL_P1_RESET__VALUE                      0x1
#define U_PNIP__BLOCKED_PRIOMASK_DP1_P1                                           (0x10601008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BLOCKED_PRIOMASK_DP1_P1__BLOCKED_PRIORITY_DP1_15_14               (0x0000C000)
#define U_PNIP__BLOCKED_PRIOMASK_DP1_P1__BLOCKED_PRIORITY_DP1_11_0                (0x00000FFF)
#define U_PNIP__BLOCKED_PRIOMASK_DP1_P1_RESET__VALUE             0x0
#define U_PNIP__BLOCKED_PRIOMASK_DP1R_P1                                          (0x1060100C + PNIP_DEV_TOP_BASE)
#define U_PNIP__BLOCKED_PRIOMASK_DP1R_P1__BLOCKED_PRIORITY_DP1R_15_14             (0x0000C000)
#define U_PNIP__BLOCKED_PRIOMASK_DP1R_P1__BLOCKED_PRIORITY_DP1R_11_0              (0x00000FFF)
#define U_PNIP__BLOCKED_PRIOMASK_DP1R_P1_RESET__VALUE            0x0
#define U_PNIP__BLOCKED_PRIOMASK_DP1S_P1                                          (0x10601010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BLOCKED_PRIOMASK_DP1S_P1__BLOCKED_PRIORITY_DP1S_15_14             (0x0000C000)
#define U_PNIP__BLOCKED_PRIOMASK_DP1S_P1__BLOCKED_PRIORITY_DP1S_11_0              (0x00000FFF)
#define U_PNIP__BLOCKED_PRIOMASK_DP1S_P1_RESET__VALUE            0x0
#define U_PNIP__BLOCKED_PRIOMASK_DP2_P1                                           (0x10601014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BLOCKED_PRIOMASK_DP2_P1__BLOCKED_PRIORITY_DP2_15_14               (0x0000C000)
#define U_PNIP__BLOCKED_PRIOMASK_DP2_P1__BLOCKED_PRIORITY_DP2_11_0                (0x00000FFF)
#define U_PNIP__BLOCKED_PRIOMASK_DP2_P1_RESET__VALUE             0x0
#define U_PNIP__MACCONTROL_P1                                                     (0x10601018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MACCONTROL_P1__BYPASS_MII_SYNC                                    (0x00010000)
#define U_PNIP__MACCONTROL_P1__RECEIVE_HALTREQUEST                                (0x00000200)
#define U_PNIP__MACCONTROL_P1__RECEIVE_ENABLE                                     (0x00000100)
#define U_PNIP__MACCONTROL_P1__TRANSMIT_HALTREQUEST                               (0x00000002)
#define U_PNIP__MACCONTROL_P1__TRANSMIT_ENABLE                                    (0x00000001)
#define U_PNIP__MACCONTROL_P1_RESET__VALUE                       0x0
#define U_PNIP__MACSTATUS_P1                                                      (0x1060101C + PNIP_DEV_TOP_BASE)
#define U_PNIP__MACSTATUS_P1__VLAN_TAGGED_RECEIVE                                 (0x00008000)
#define U_PNIP__MACSTATUS_P1__BROADCAST_RECEIVE                                   (0x00004000)
#define U_PNIP__MACSTATUS_P1__MULTICAST_RECEIVE                                   (0x00002000)
#define U_PNIP__MACSTATUS_P1__UNICAST_RECEIVE                                     (0x00001000)
#define U_PNIP__MACSTATUS_P1__LONG_ERROR                                          (0x00000800)
#define U_PNIP__MACSTATUS_P1__SHORT_ERROR                                         (0x00000400)
#define U_PNIP__MACSTATUS_P1__ALIGNMENT_ERROR                                     (0x00000200)
#define U_PNIP__MACSTATUS_P1__CRC_ERROR                                           (0x00000100)
#define U_PNIP__MACSTATUS_P1__GOOD_RECEIVE                                        (0x00000080)
#define U_PNIP__MACSTATUS_P1__RECEIVE_COMPLETION                                  (0x00000040)
#define U_PNIP__MACSTATUS_P1__VLAN_TAGGED_TRANSMIT                                (0x00000020)
#define U_PNIP__MACSTATUS_P1__BROADCAST_TRANSMIT                                  (0x00000010)
#define U_PNIP__MACSTATUS_P1__MULTICAST_TRANSMIT                                  (0x00000008)
#define U_PNIP__MACSTATUS_P1__UNICAST_TRANSMIT                                    (0x00000004)
#define U_PNIP__MACSTATUS_P1__GOOD_TRANSMIT                                       (0x00000002)
#define U_PNIP__MACSTATUS_P1__TRANSMIT_COMPLETION                                 (0x00000001)
#define U_PNIP__MACSTATUS_P1_RESET__VALUE                        0x0
#define U_PNIP__MACERRORSTATUS_P1                                                 (0x10601020 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MACERRORSTATUS_P1__MACTCLOCKDRIFTERR                              (0x00001000)
#define U_PNIP__MACERRORSTATUS_P1__MACTCLOCKERR                                   (0x00000800)
#define U_PNIP__MACERRORSTATUS_P1__MACTCRCDOUBLECHECKERR                          (0x00000400)
#define U_PNIP__MACERRORSTATUS_P1__MACTBUFFERUNDERFLOWERR                         (0x00000200)
#define U_PNIP__MACERRORSTATUS_P1__MACTBUFFEROVERFLOWERR                          (0x00000100)
#define U_PNIP__MACERRORSTATUS_P1__MACRFRAMEOVERFLOWERR                           (0x00000020)
#define U_PNIP__MACERRORSTATUS_P1__MACRBUFFERDISCARD                              (0x00000010)
#define U_PNIP__MACERRORSTATUS_P1__MACRFRAMEGAPERR                                (0x00000008)
#define U_PNIP__MACERRORSTATUS_P1__MACRCRCDOUBLECHECKERR                          (0x00000004)
#define U_PNIP__MACERRORSTATUS_P1__MACRBUFFERUNDERFLOWERR                         (0x00000002)
#define U_PNIP__MACERRORSTATUS_P1__MACRBUFFEROVERFLOWERR                          (0x00000001)
#define U_PNIP__MACERRORSTATUS_P1_RESET__VALUE                   0x0
#define U_PNIP__LINEDELAY_P1                                                      (0x10601024 + PNIP_DEV_TOP_BASE)
#define U_PNIP__LINEDELAY_P1__LINEDELAY                                           (0xFFFFFFFF)
#define U_PNIP__LINEDELAY_P1_RESET__VALUE                        0x0
#define U_PNIP__MINIMUMIFG_P1                                                     (0x10601028 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MINIMUMIFG_P1__MINIFG_1G                                          (0x00000700)
#define U_PNIP__MINIMUMIFG_P1__MINIFG_100M                                        (0x00000007)
#define U_PNIP__MINIMUMIFG_P1_RESET__VALUE                       0x300
#define U_PNIP__PREAMBLELENGTH_P1                                                 (0x1060102C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PREAMBLELENGTH_P1__RXD_MINLENGTH_1G                               (0x07000000)
#define U_PNIP__PREAMBLELENGTH_P1__TXD_LENGTH_1G                                  (0x00070000)
#define U_PNIP__PREAMBLELENGTH_P1__RXD_MINLENGTH_100M                             (0x00000700)
#define U_PNIP__PREAMBLELENGTH_P1__TXD_LENGTH_100M                                (0x00000007)
#define U_PNIP__PREAMBLELENGTH_P1_RESET__VALUE                   0x70007
#define U_PNIP__RED_PRIOMASK_P1                                                   (0x10601030 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RED_PRIOMASK_P1__QUEUE_PRIORITY_X                                 (0x00000FFF)
#define U_PNIP__RED_PRIOMASK_P1_RESET__VALUE                     0x100
#define U_PNIP__ORANGE_PRIOMASK_P1                                                (0x10601034 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ORANGE_PRIOMASK_P1__QUEUE_PRIORITY_X                              (0x00000FFF)
#define U_PNIP__ORANGE_PRIOMASK_P1_RESET__VALUE                  0xD80
#define U_PNIP__GREEN_PRIOMASK_P1                                                 (0x10601038 + PNIP_DEV_TOP_BASE)
#define U_PNIP__GREEN_PRIOMASK_P1__QUEUE_PRIORITY_X                               (0x00000FFF)
#define U_PNIP__GREEN_PRIOMASK_P1_RESET__VALUE                   0xFFF
#define U_PNIP__IRT_DELAYTIME_P1                                                  (0x1060103C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRT_DELAYTIME_P1__DELAY_MODE                                      (0x80000000)
#define U_PNIP__IRT_DELAYTIME_P1__DELAY_TIME                                      (0x0003FFFF)
#define U_PNIP__IRT_DELAYTIME_P1_RESET__VALUE                    0x0
#define U_PNIP__IRT_DELAYSTATUS_0_P1                                              (0x10601040 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRT_DELAYSTATUS_0_P1__ETHERTYPE                                   (0xFFFF0000)
#define U_PNIP__IRT_DELAYSTATUS_0_P1__FRAMEID                                     (0x0000FFFF)
#define U_PNIP__IRT_DELAYSTATUS_0_P1_RESET__VALUE                0x0
#define U_PNIP__IRT_DELAYSTATUS_1_P1                                              (0x10601044 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRT_DELAYSTATUS_1_P1__BUSY                                        (0x80000000)
#define U_PNIP__IRT_DELAYSTATUS_1_P1__TIMEBASE                                    (0x40000000)
#define U_PNIP__IRT_DELAYSTATUS_1_P1__SOURCE                                      (0x20000000)
#define U_PNIP__IRT_DELAYSTATUS_1_P1__TXTIME                                      (0x1FFFFFFF)
#define U_PNIP__IRT_DELAYSTATUS_1_P1_RESET__VALUE                0x0
#define U_PNIP__API_FORWARDINGMODE_P1                                             (0x10601048 + PNIP_DEV_TOP_BASE)
#define U_PNIP__API_FORWARDINGMODE_P1__API_FWMODE_15                              (0x00008000)
#define U_PNIP__API_FORWARDINGMODE_P1__API_FWMODE_14                              (0x00004000)
#define U_PNIP__API_FORWARDINGMODE_P1__API_FWMODE                                 (0x00000FFF)
#define U_PNIP__API_FORWARDINGMODE_P1_RESET__VALUE               0xCFFF
#define U_PNIP__FFWDELAY_CONTROL_P1                                               (0x1060104C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FFWDELAY_CONTROL_P1__EN_FFWDELAY_DP2                              (0x00000008)
#define U_PNIP__FFWDELAY_CONTROL_P1__EN_FFWDELAY_DP1S                             (0x00000004)
#define U_PNIP__FFWDELAY_CONTROL_P1__EN_FFWDELAY_DP1R                             (0x00000002)
#define U_PNIP__FFWDELAY_CONTROL_P1__EN_FFWDELAY_DP1                              (0x00000001)
#define U_PNIP__FFWDELAY_CONTROL_P1_RESET__VALUE                 0x7
#define U_PNIP__RXFRAMELENGTH_P1                                                  (0x10601050 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RXFRAMELENGTH_P1__RXFRAMELENGTH                                   (0x000007FF)
#define U_PNIP__RXFRAMELENGTH_P1_RESET__VALUE                    0x600
#define U_PNIP__TXFRAMELENGTH_P1                                                  (0x10601054 + PNIP_DEV_TOP_BASE)
#define U_PNIP__TXFRAMELENGTH_P1__TXFRAMELENGTH                                   (0x000007FF)
#define U_PNIP__TXFRAMELENGTH_P1_RESET__VALUE                    0x5FC
#define U_PNIP__PORTCONTROL_P2                                                    (0x10602000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PORTCONTROL_P2__DISABLE_MACTXERR                                  (0x80000000)
#define U_PNIP__PORTCONTROL_P2__SUPERFASTFORWARDING                               (0x00000100)
#define U_PNIP__PORTCONTROL_P2__TX_REDGUARD_DP2                                   (0x00000080)
#define U_PNIP__PORTCONTROL_P2__PTCP_FWD_ENABLE                                   (0x00000040)
#define U_PNIP__PORTCONTROL_P2__PTCP_FWD_DELAY                                    (0x00000020)
#define U_PNIP__PORTCONTROL_P2__FASTFORWARDING                                    (0x00000010)
#define U_PNIP__PORTCONTROL_P2__TXD_FRAGMENTATION                                 (0x00000008)
#define U_PNIP__PORTCONTROL_P2__RXD_FRAGMENTATION                                 (0x00000004)
#define U_PNIP__PORTCONTROL_P2__FORWARDING_MODE                                   (0x00000002)
#define U_PNIP__PORTCONTROL_P2__TRANSFERRATE                                      (0x00000001)
#define U_PNIP__PORTCONTROL_P2_RESET__VALUE                      0x44
#define U_PNIP__FLOWCONTROL_P2                                                    (0x10602004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FLOWCONTROL_P2__TXBOUNDARY_RED2GREEN_DP2                          (0x04000000)
#define U_PNIP__FLOWCONTROL_P2__TXBOUNDARY_DP2                                    (0x02000000)
#define U_PNIP__FLOWCONTROL_P2__RXBOUNDARY_DP2                                    (0x01000000)
#define U_PNIP__FLOWCONTROL_P2__RTLOWBOUNDARY_DP1S                                (0x00080000)
#define U_PNIP__FLOWCONTROL_P2__RTHIGHBOUNDARY_DP1S                               (0x00040000)
#define U_PNIP__FLOWCONTROL_P2__TXBOUNDARY_DP1S                                   (0x00020000)
#define U_PNIP__FLOWCONTROL_P2__RXBOUNDARY_DP1S                                   (0x00010000)
#define U_PNIP__FLOWCONTROL_P2__RTLOWBOUNDARY_DP1_R                               (0x00000800)
#define U_PNIP__FLOWCONTROL_P2__RTHIGHBOUNDARY_DP1_R                              (0x00000400)
#define U_PNIP__FLOWCONTROL_P2__TXBOUNDARY_DP1_R                                  (0x00000200)
#define U_PNIP__FLOWCONTROL_P2__RXBOUNDARY_DP1_R                                  (0x00000100)
#define U_PNIP__FLOWCONTROL_P2__DISABLEPORT                                       (0x00000001)
#define U_PNIP__FLOWCONTROL_P2_RESET__VALUE                      0x1
#define U_PNIP__BLOCKED_PRIOMASK_DP1_P2                                           (0x10602008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BLOCKED_PRIOMASK_DP1_P2__BLOCKED_PRIORITY_DP1_15_14               (0x0000C000)
#define U_PNIP__BLOCKED_PRIOMASK_DP1_P2__BLOCKED_PRIORITY_DP1_11_0                (0x00000FFF)
#define U_PNIP__BLOCKED_PRIOMASK_DP1_P2_RESET__VALUE             0x0
#define U_PNIP__BLOCKED_PRIOMASK_DP1R_P2                                          (0x1060200C + PNIP_DEV_TOP_BASE)
#define U_PNIP__BLOCKED_PRIOMASK_DP1R_P2__BLOCKED_PRIORITY_DP1R_15_14             (0x0000C000)
#define U_PNIP__BLOCKED_PRIOMASK_DP1R_P2__BLOCKED_PRIORITY_DP1R_11_0              (0x00000FFF)
#define U_PNIP__BLOCKED_PRIOMASK_DP1R_P2_RESET__VALUE            0x0
#define U_PNIP__BLOCKED_PRIOMASK_DP1S_P2                                          (0x10602010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BLOCKED_PRIOMASK_DP1S_P2__BLOCKED_PRIORITY_DP1S_15_14             (0x0000C000)
#define U_PNIP__BLOCKED_PRIOMASK_DP1S_P2__BLOCKED_PRIORITY_DP1S_11_0              (0x00000FFF)
#define U_PNIP__BLOCKED_PRIOMASK_DP1S_P2_RESET__VALUE            0x0
#define U_PNIP__BLOCKED_PRIOMASK_DP2_P2                                           (0x10602014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BLOCKED_PRIOMASK_DP2_P2__BLOCKED_PRIORITY_DP2_15_14               (0x0000C000)
#define U_PNIP__BLOCKED_PRIOMASK_DP2_P2__BLOCKED_PRIORITY_DP2_11_0                (0x00000FFF)
#define U_PNIP__BLOCKED_PRIOMASK_DP2_P2_RESET__VALUE             0x0
#define U_PNIP__MACCONTROL_P2                                                     (0x10602018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MACCONTROL_P2__BYPASS_MII_SYNC                                    (0x00010000)
#define U_PNIP__MACCONTROL_P2__RECEIVE_HALTREQUEST                                (0x00000200)
#define U_PNIP__MACCONTROL_P2__RECEIVE_ENABLE                                     (0x00000100)
#define U_PNIP__MACCONTROL_P2__TRANSMIT_HALTREQUEST                               (0x00000002)
#define U_PNIP__MACCONTROL_P2__TRANSMIT_ENABLE                                    (0x00000001)
#define U_PNIP__MACCONTROL_P2_RESET__VALUE                       0x0
#define U_PNIP__MACSTATUS_P2                                                      (0x1060201C + PNIP_DEV_TOP_BASE)
#define U_PNIP__MACSTATUS_P2__VLAN_TAGGED_RECEIVE                                 (0x00008000)
#define U_PNIP__MACSTATUS_P2__BROADCAST_RECEIVE                                   (0x00004000)
#define U_PNIP__MACSTATUS_P2__MULTICAST_RECEIVE                                   (0x00002000)
#define U_PNIP__MACSTATUS_P2__UNICAST_RECEIVE                                     (0x00001000)
#define U_PNIP__MACSTATUS_P2__LONG_ERROR                                          (0x00000800)
#define U_PNIP__MACSTATUS_P2__SHORT_ERROR                                         (0x00000400)
#define U_PNIP__MACSTATUS_P2__ALIGNMENT_ERROR                                     (0x00000200)
#define U_PNIP__MACSTATUS_P2__CRC_ERROR                                           (0x00000100)
#define U_PNIP__MACSTATUS_P2__GOOD_RECEIVE                                        (0x00000080)
#define U_PNIP__MACSTATUS_P2__RECEIVE_COMPLETION                                  (0x00000040)
#define U_PNIP__MACSTATUS_P2__VLAN_TAGGED_TRANSMIT                                (0x00000020)
#define U_PNIP__MACSTATUS_P2__BROADCAST_TRANSMIT                                  (0x00000010)
#define U_PNIP__MACSTATUS_P2__MULTICAST_TRANSMIT                                  (0x00000008)
#define U_PNIP__MACSTATUS_P2__UNICAST_TRANSMIT                                    (0x00000004)
#define U_PNIP__MACSTATUS_P2__GOOD_TRANSMIT                                       (0x00000002)
#define U_PNIP__MACSTATUS_P2__TRANSMIT_COMPLETION                                 (0x00000001)
#define U_PNIP__MACSTATUS_P2_RESET__VALUE                        0x0
#define U_PNIP__MACERRORSTATUS_P2                                                 (0x10602020 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MACERRORSTATUS_P2__MACTCLOCKDRIFTERR                              (0x00001000)
#define U_PNIP__MACERRORSTATUS_P2__MACTCLOCKERR                                   (0x00000800)
#define U_PNIP__MACERRORSTATUS_P2__MACTCRCDOUBLECHECKERR                          (0x00000400)
#define U_PNIP__MACERRORSTATUS_P2__MACTBUFFERUNDERFLOWERR                         (0x00000200)
#define U_PNIP__MACERRORSTATUS_P2__MACTBUFFEROVERFLOWERR                          (0x00000100)
#define U_PNIP__MACERRORSTATUS_P2__MACRFRAMEOVERFLOWERR                           (0x00000020)
#define U_PNIP__MACERRORSTATUS_P2__MACRBUFFERDISCARD                              (0x00000010)
#define U_PNIP__MACERRORSTATUS_P2__MACRFRAMEGAPERR                                (0x00000008)
#define U_PNIP__MACERRORSTATUS_P2__MACRCRCDOUBLECHECKERR                          (0x00000004)
#define U_PNIP__MACERRORSTATUS_P2__MACRBUFFERUNDERFLOWERR                         (0x00000002)
#define U_PNIP__MACERRORSTATUS_P2__MACRBUFFEROVERFLOWERR                          (0x00000001)
#define U_PNIP__MACERRORSTATUS_P2_RESET__VALUE                   0x0
#define U_PNIP__LINEDELAY_P2                                                      (0x10602024 + PNIP_DEV_TOP_BASE)
#define U_PNIP__LINEDELAY_P2__LINEDELAY                                           (0xFFFFFFFF)
#define U_PNIP__LINEDELAY_P2_RESET__VALUE                        0x0
#define U_PNIP__MINIMUMIFG_P2                                                     (0x10602028 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MINIMUMIFG_P2__MINIFG_1G                                          (0x00000700)
#define U_PNIP__MINIMUMIFG_P2__MINIFG_100M                                        (0x00000007)
#define U_PNIP__MINIMUMIFG_P2_RESET__VALUE                       0x300
#define U_PNIP__PREAMBLELENGTH_P2                                                 (0x1060202C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PREAMBLELENGTH_P2__RXD_MINLENGTH_1G                               (0x07000000)
#define U_PNIP__PREAMBLELENGTH_P2__TXD_LENGTH_1G                                  (0x00070000)
#define U_PNIP__PREAMBLELENGTH_P2__RXD_MINLENGTH_100M                             (0x00000700)
#define U_PNIP__PREAMBLELENGTH_P2__TXD_LENGTH_100M                                (0x00000007)
#define U_PNIP__PREAMBLELENGTH_P2_RESET__VALUE                   0x70007
#define U_PNIP__RED_PRIOMASK_P2                                                   (0x10602030 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RED_PRIOMASK_P2__QUEUE_PRIORITY_X                                 (0x00000FFF)
#define U_PNIP__RED_PRIOMASK_P2_RESET__VALUE                     0x100
#define U_PNIP__ORANGE_PRIOMASK_P2                                                (0x10602034 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ORANGE_PRIOMASK_P2__QUEUE_PRIORITY_X                              (0x00000FFF)
#define U_PNIP__ORANGE_PRIOMASK_P2_RESET__VALUE                  0xD80
#define U_PNIP__GREEN_PRIOMASK_P2                                                 (0x10602038 + PNIP_DEV_TOP_BASE)
#define U_PNIP__GREEN_PRIOMASK_P2__QUEUE_PRIORITY_X                               (0x00000FFF)
#define U_PNIP__GREEN_PRIOMASK_P2_RESET__VALUE                   0xFFF
#define U_PNIP__IRT_DELAYTIME_P2                                                  (0x1060203C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRT_DELAYTIME_P2__DELAY_MODE                                      (0x80000000)
#define U_PNIP__IRT_DELAYTIME_P2__DELAY_TIME                                      (0x0003FFFF)
#define U_PNIP__IRT_DELAYTIME_P2_RESET__VALUE                    0x0
#define U_PNIP__IRT_DELAYSTATUS_0_P2                                              (0x10602040 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRT_DELAYSTATUS_0_P2__ETHERTYPE                                   (0xFFFF0000)
#define U_PNIP__IRT_DELAYSTATUS_0_P2__FRAMEID                                     (0x0000FFFF)
#define U_PNIP__IRT_DELAYSTATUS_0_P2_RESET__VALUE                0x0
#define U_PNIP__IRT_DELAYSTATUS_1_P2                                              (0x10602044 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRT_DELAYSTATUS_1_P2__BUSY                                        (0x80000000)
#define U_PNIP__IRT_DELAYSTATUS_1_P2__TIMEBASE                                    (0x40000000)
#define U_PNIP__IRT_DELAYSTATUS_1_P2__SOURCE                                      (0x20000000)
#define U_PNIP__IRT_DELAYSTATUS_1_P2__TXTIME                                      (0x1FFFFFFF)
#define U_PNIP__IRT_DELAYSTATUS_1_P2_RESET__VALUE                0x0
#define U_PNIP__API_FORWARDINGMODE_P2                                             (0x10602048 + PNIP_DEV_TOP_BASE)
#define U_PNIP__API_FORWARDINGMODE_P2__API_FWMODE_15                              (0x00008000)
#define U_PNIP__API_FORWARDINGMODE_P2__API_FWMODE_14                              (0x00004000)
#define U_PNIP__API_FORWARDINGMODE_P2__API_FWMODE                                 (0x00000FFF)
#define U_PNIP__API_FORWARDINGMODE_P2_RESET__VALUE               0xCFFF
#define U_PNIP__FFWDELAY_CONTROL_P2                                               (0x1060204C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FFWDELAY_CONTROL_P2__EN_FFWDELAY_DP2                              (0x00000008)
#define U_PNIP__FFWDELAY_CONTROL_P2__EN_FFWDELAY_DP1S                             (0x00000004)
#define U_PNIP__FFWDELAY_CONTROL_P2__EN_FFWDELAY_DP1R                             (0x00000002)
#define U_PNIP__FFWDELAY_CONTROL_P2__EN_FFWDELAY_DP1                              (0x00000001)
#define U_PNIP__FFWDELAY_CONTROL_P2_RESET__VALUE                 0x7
#define U_PNIP__RXFRAMELENGTH_P2                                                  (0x10602050 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RXFRAMELENGTH_P2__RXFRAMELENGTH                                   (0x000007FF)
#define U_PNIP__RXFRAMELENGTH_P2_RESET__VALUE                    0x600
#define U_PNIP__TXFRAMELENGTH_P2                                                  (0x10602054 + PNIP_DEV_TOP_BASE)
#define U_PNIP__TXFRAMELENGTH_P2__TXFRAMELENGTH                                   (0x000007FF)
#define U_PNIP__TXFRAMELENGTH_P2_RESET__VALUE                    0x5FC
#define U_PNIP__PORTCONTROL_P3                                                    (0x10603000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PORTCONTROL_P3__DISABLE_MACTXERR                                  (0x80000000)
#define U_PNIP__PORTCONTROL_P3__SUPERFASTFORWARDING                               (0x00000100)
#define U_PNIP__PORTCONTROL_P3__TX_REDGUARD_DP2                                   (0x00000080)
#define U_PNIP__PORTCONTROL_P3__PTCP_FWD_ENABLE                                   (0x00000040)
#define U_PNIP__PORTCONTROL_P3__PTCP_FWD_DELAY                                    (0x00000020)
#define U_PNIP__PORTCONTROL_P3__FASTFORWARDING                                    (0x00000010)
#define U_PNIP__PORTCONTROL_P3__TXD_FRAGMENTATION                                 (0x00000008)
#define U_PNIP__PORTCONTROL_P3__RXD_FRAGMENTATION                                 (0x00000004)
#define U_PNIP__PORTCONTROL_P3__FORWARDING_MODE                                   (0x00000002)
#define U_PNIP__PORTCONTROL_P3__TRANSFERRATE                                      (0x00000001)
#define U_PNIP__PORTCONTROL_P3_RESET__VALUE                      0x44
#define U_PNIP__FLOWCONTROL_P3                                                    (0x10603004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FLOWCONTROL_P3__TXBOUNDARY_RED2GREEN_DP2                          (0x04000000)
#define U_PNIP__FLOWCONTROL_P3__TXBOUNDARY_DP2                                    (0x02000000)
#define U_PNIP__FLOWCONTROL_P3__RXBOUNDARY_DP2                                    (0x01000000)
#define U_PNIP__FLOWCONTROL_P3__RTLOWBOUNDARY_DP1S                                (0x00080000)
#define U_PNIP__FLOWCONTROL_P3__RTHIGHBOUNDARY_DP1S                               (0x00040000)
#define U_PNIP__FLOWCONTROL_P3__TXBOUNDARY_DP1S                                   (0x00020000)
#define U_PNIP__FLOWCONTROL_P3__RXBOUNDARY_DP1S                                   (0x00010000)
#define U_PNIP__FLOWCONTROL_P3__RTLOWBOUNDARY_DP1_R                               (0x00000800)
#define U_PNIP__FLOWCONTROL_P3__RTHIGHBOUNDARY_DP1_R                              (0x00000400)
#define U_PNIP__FLOWCONTROL_P3__TXBOUNDARY_DP1_R                                  (0x00000200)
#define U_PNIP__FLOWCONTROL_P3__RXBOUNDARY_DP1_R                                  (0x00000100)
#define U_PNIP__FLOWCONTROL_P3__DISABLEPORT                                       (0x00000001)
#define U_PNIP__FLOWCONTROL_P3_RESET__VALUE                      0x1
#define U_PNIP__BLOCKED_PRIOMASK_DP1_P3                                           (0x10603008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BLOCKED_PRIOMASK_DP1_P3__BLOCKED_PRIORITY_DP1_15_14               (0x0000C000)
#define U_PNIP__BLOCKED_PRIOMASK_DP1_P3__BLOCKED_PRIORITY_DP1_11_0                (0x00000FFF)
#define U_PNIP__BLOCKED_PRIOMASK_DP1_P3_RESET__VALUE             0x0
#define U_PNIP__BLOCKED_PRIOMASK_DP1R_P3                                          (0x1060300C + PNIP_DEV_TOP_BASE)
#define U_PNIP__BLOCKED_PRIOMASK_DP1R_P3__BLOCKED_PRIORITY_DP1R_15_14             (0x0000C000)
#define U_PNIP__BLOCKED_PRIOMASK_DP1R_P3__BLOCKED_PRIORITY_DP1R_11_0              (0x00000FFF)
#define U_PNIP__BLOCKED_PRIOMASK_DP1R_P3_RESET__VALUE            0x0
#define U_PNIP__BLOCKED_PRIOMASK_DP1S_P3                                          (0x10603010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BLOCKED_PRIOMASK_DP1S_P3__BLOCKED_PRIORITY_DP1S_15_14             (0x0000C000)
#define U_PNIP__BLOCKED_PRIOMASK_DP1S_P3__BLOCKED_PRIORITY_DP1S_11_0              (0x00000FFF)
#define U_PNIP__BLOCKED_PRIOMASK_DP1S_P3_RESET__VALUE            0x0
#define U_PNIP__BLOCKED_PRIOMASK_DP2_P3                                           (0x10603014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BLOCKED_PRIOMASK_DP2_P3__BLOCKED_PRIORITY_DP2_15_14               (0x0000C000)
#define U_PNIP__BLOCKED_PRIOMASK_DP2_P3__BLOCKED_PRIORITY_DP2_11_0                (0x00000FFF)
#define U_PNIP__BLOCKED_PRIOMASK_DP2_P3_RESET__VALUE             0x0
#define U_PNIP__MACCONTROL_P3                                                     (0x10603018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MACCONTROL_P3__BYPASS_MII_SYNC                                    (0x00010000)
#define U_PNIP__MACCONTROL_P3__RECEIVE_HALTREQUEST                                (0x00000200)
#define U_PNIP__MACCONTROL_P3__RECEIVE_ENABLE                                     (0x00000100)
#define U_PNIP__MACCONTROL_P3__TRANSMIT_HALTREQUEST                               (0x00000002)
#define U_PNIP__MACCONTROL_P3__TRANSMIT_ENABLE                                    (0x00000001)
#define U_PNIP__MACCONTROL_P3_RESET__VALUE                       0x0
#define U_PNIP__MACSTATUS_P3                                                      (0x1060301C + PNIP_DEV_TOP_BASE)
#define U_PNIP__MACSTATUS_P3__VLAN_TAGGED_RECEIVE                                 (0x00008000)
#define U_PNIP__MACSTATUS_P3__BROADCAST_RECEIVE                                   (0x00004000)
#define U_PNIP__MACSTATUS_P3__MULTICAST_RECEIVE                                   (0x00002000)
#define U_PNIP__MACSTATUS_P3__UNICAST_RECEIVE                                     (0x00001000)
#define U_PNIP__MACSTATUS_P3__LONG_ERROR                                          (0x00000800)
#define U_PNIP__MACSTATUS_P3__SHORT_ERROR                                         (0x00000400)
#define U_PNIP__MACSTATUS_P3__ALIGNMENT_ERROR                                     (0x00000200)
#define U_PNIP__MACSTATUS_P3__CRC_ERROR                                           (0x00000100)
#define U_PNIP__MACSTATUS_P3__GOOD_RECEIVE                                        (0x00000080)
#define U_PNIP__MACSTATUS_P3__RECEIVE_COMPLETION                                  (0x00000040)
#define U_PNIP__MACSTATUS_P3__VLAN_TAGGED_TRANSMIT                                (0x00000020)
#define U_PNIP__MACSTATUS_P3__BROADCAST_TRANSMIT                                  (0x00000010)
#define U_PNIP__MACSTATUS_P3__MULTICAST_TRANSMIT                                  (0x00000008)
#define U_PNIP__MACSTATUS_P3__UNICAST_TRANSMIT                                    (0x00000004)
#define U_PNIP__MACSTATUS_P3__GOOD_TRANSMIT                                       (0x00000002)
#define U_PNIP__MACSTATUS_P3__TRANSMIT_COMPLETION                                 (0x00000001)
#define U_PNIP__MACSTATUS_P3_RESET__VALUE                        0x0
#define U_PNIP__MACERRORSTATUS_P3                                                 (0x10603020 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MACERRORSTATUS_P3__MACTCLOCKDRIFTERR                              (0x00001000)
#define U_PNIP__MACERRORSTATUS_P3__MACTCLOCKERR                                   (0x00000800)
#define U_PNIP__MACERRORSTATUS_P3__MACTCRCDOUBLECHECKERR                          (0x00000400)
#define U_PNIP__MACERRORSTATUS_P3__MACTBUFFERUNDERFLOWERR                         (0x00000200)
#define U_PNIP__MACERRORSTATUS_P3__MACTBUFFEROVERFLOWERR                          (0x00000100)
#define U_PNIP__MACERRORSTATUS_P3__MACRFRAMEOVERFLOWERR                           (0x00000020)
#define U_PNIP__MACERRORSTATUS_P3__MACRBUFFERDISCARD                              (0x00000010)
#define U_PNIP__MACERRORSTATUS_P3__MACRFRAMEGAPERR                                (0x00000008)
#define U_PNIP__MACERRORSTATUS_P3__MACRCRCDOUBLECHECKERR                          (0x00000004)
#define U_PNIP__MACERRORSTATUS_P3__MACRBUFFERUNDERFLOWERR                         (0x00000002)
#define U_PNIP__MACERRORSTATUS_P3__MACRBUFFEROVERFLOWERR                          (0x00000001)
#define U_PNIP__MACERRORSTATUS_P3_RESET__VALUE                   0x0
#define U_PNIP__LINEDELAY_P3                                                      (0x10603024 + PNIP_DEV_TOP_BASE)
#define U_PNIP__LINEDELAY_P3__LINEDELAY                                           (0xFFFFFFFF)
#define U_PNIP__LINEDELAY_P3_RESET__VALUE                        0x0
#define U_PNIP__MINIMUMIFG_P3                                                     (0x10603028 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MINIMUMIFG_P3__MINIFG_1G                                          (0x00000700)
#define U_PNIP__MINIMUMIFG_P3__MINIFG_100M                                        (0x00000007)
#define U_PNIP__MINIMUMIFG_P3_RESET__VALUE                       0x300
#define U_PNIP__PREAMBLELENGTH_P3                                                 (0x1060302C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PREAMBLELENGTH_P3__RXD_MINLENGTH_1G                               (0x07000000)
#define U_PNIP__PREAMBLELENGTH_P3__TXD_LENGTH_1G                                  (0x00070000)
#define U_PNIP__PREAMBLELENGTH_P3__RXD_MINLENGTH_100M                             (0x00000700)
#define U_PNIP__PREAMBLELENGTH_P3__TXD_LENGTH_100M                                (0x00000007)
#define U_PNIP__PREAMBLELENGTH_P3_RESET__VALUE                   0x70007
#define U_PNIP__RED_PRIOMASK_P3                                                   (0x10603030 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RED_PRIOMASK_P3__QUEUE_PRIORITY_X                                 (0x00000FFF)
#define U_PNIP__RED_PRIOMASK_P3_RESET__VALUE                     0x100
#define U_PNIP__ORANGE_PRIOMASK_P3                                                (0x10603034 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ORANGE_PRIOMASK_P3__QUEUE_PRIORITY_X                              (0x00000FFF)
#define U_PNIP__ORANGE_PRIOMASK_P3_RESET__VALUE                  0xD80
#define U_PNIP__GREEN_PRIOMASK_P3                                                 (0x10603038 + PNIP_DEV_TOP_BASE)
#define U_PNIP__GREEN_PRIOMASK_P3__QUEUE_PRIORITY_X                               (0x00000FFF)
#define U_PNIP__GREEN_PRIOMASK_P3_RESET__VALUE                   0xFFF
#define U_PNIP__IRT_DELAYTIME_P3                                                  (0x1060303C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRT_DELAYTIME_P3__DELAY_MODE                                      (0x80000000)
#define U_PNIP__IRT_DELAYTIME_P3__DELAY_TIME                                      (0x0003FFFF)
#define U_PNIP__IRT_DELAYTIME_P3_RESET__VALUE                    0x0
#define U_PNIP__IRT_DELAYSTATUS_0_P3                                              (0x10603040 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRT_DELAYSTATUS_0_P3__ETHERTYPE                                   (0xFFFF0000)
#define U_PNIP__IRT_DELAYSTATUS_0_P3__FRAMEID                                     (0x0000FFFF)
#define U_PNIP__IRT_DELAYSTATUS_0_P3_RESET__VALUE                0x0
#define U_PNIP__IRT_DELAYSTATUS_1_P3                                              (0x10603044 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRT_DELAYSTATUS_1_P3__BUSY                                        (0x80000000)
#define U_PNIP__IRT_DELAYSTATUS_1_P3__TIMEBASE                                    (0x40000000)
#define U_PNIP__IRT_DELAYSTATUS_1_P3__SOURCE                                      (0x20000000)
#define U_PNIP__IRT_DELAYSTATUS_1_P3__TXTIME                                      (0x1FFFFFFF)
#define U_PNIP__IRT_DELAYSTATUS_1_P3_RESET__VALUE                0x0
#define U_PNIP__API_FORWARDINGMODE_P3                                             (0x10603048 + PNIP_DEV_TOP_BASE)
#define U_PNIP__API_FORWARDINGMODE_P3__API_FWMODE_15                              (0x00008000)
#define U_PNIP__API_FORWARDINGMODE_P3__API_FWMODE_14                              (0x00004000)
#define U_PNIP__API_FORWARDINGMODE_P3__API_FWMODE                                 (0x00000FFF)
#define U_PNIP__API_FORWARDINGMODE_P3_RESET__VALUE               0xCFFF
#define U_PNIP__FFWDELAY_CONTROL_P3                                               (0x1060304C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FFWDELAY_CONTROL_P3__EN_FFWDELAY_DP2                              (0x00000008)
#define U_PNIP__FFWDELAY_CONTROL_P3__EN_FFWDELAY_DP1S                             (0x00000004)
#define U_PNIP__FFWDELAY_CONTROL_P3__EN_FFWDELAY_DP1R                             (0x00000002)
#define U_PNIP__FFWDELAY_CONTROL_P3__EN_FFWDELAY_DP1                              (0x00000001)
#define U_PNIP__FFWDELAY_CONTROL_P3_RESET__VALUE                 0x7
#define U_PNIP__RXFRAMELENGTH_P3                                                  (0x10603050 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RXFRAMELENGTH_P3__RXFRAMELENGTH                                   (0x000007FF)
#define U_PNIP__RXFRAMELENGTH_P3_RESET__VALUE                    0x600
#define U_PNIP__TXFRAMELENGTH_P3                                                  (0x10603054 + PNIP_DEV_TOP_BASE)
#define U_PNIP__TXFRAMELENGTH_P3__TXFRAMELENGTH                                   (0x000007FF)
#define U_PNIP__TXFRAMELENGTH_P3_RESET__VALUE                    0x5FC
#define U_PNIP__PORTCONTROL_P4                                                    (0x10604000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PORTCONTROL_P4__DISABLE_MACTXERR                                  (0x80000000)
#define U_PNIP__PORTCONTROL_P4__SUPERFASTFORWARDING                               (0x00000100)
#define U_PNIP__PORTCONTROL_P4__TX_REDGUARD_DP2                                   (0x00000080)
#define U_PNIP__PORTCONTROL_P4__PTCP_FWD_ENABLE                                   (0x00000040)
#define U_PNIP__PORTCONTROL_P4__PTCP_FWD_DELAY                                    (0x00000020)
#define U_PNIP__PORTCONTROL_P4__FASTFORWARDING                                    (0x00000010)
#define U_PNIP__PORTCONTROL_P4__TXD_FRAGMENTATION                                 (0x00000008)
#define U_PNIP__PORTCONTROL_P4__RXD_FRAGMENTATION                                 (0x00000004)
#define U_PNIP__PORTCONTROL_P4__FORWARDING_MODE                                   (0x00000002)
#define U_PNIP__PORTCONTROL_P4__TRANSFERRATE                                      (0x00000001)
#define U_PNIP__PORTCONTROL_P4_RESET__VALUE                      0x44
#define U_PNIP__FLOWCONTROL_P4                                                    (0x10604004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FLOWCONTROL_P4__TXBOUNDARY_RED2GREEN_DP2                          (0x04000000)
#define U_PNIP__FLOWCONTROL_P4__TXBOUNDARY_DP2                                    (0x02000000)
#define U_PNIP__FLOWCONTROL_P4__RXBOUNDARY_DP2                                    (0x01000000)
#define U_PNIP__FLOWCONTROL_P4__RTLOWBOUNDARY_DP1S                                (0x00080000)
#define U_PNIP__FLOWCONTROL_P4__RTHIGHBOUNDARY_DP1S                               (0x00040000)
#define U_PNIP__FLOWCONTROL_P4__TXBOUNDARY_DP1S                                   (0x00020000)
#define U_PNIP__FLOWCONTROL_P4__RXBOUNDARY_DP1S                                   (0x00010000)
#define U_PNIP__FLOWCONTROL_P4__RTLOWBOUNDARY_DP1_R                               (0x00000800)
#define U_PNIP__FLOWCONTROL_P4__RTHIGHBOUNDARY_DP1_R                              (0x00000400)
#define U_PNIP__FLOWCONTROL_P4__TXBOUNDARY_DP1_R                                  (0x00000200)
#define U_PNIP__FLOWCONTROL_P4__RXBOUNDARY_DP1_R                                  (0x00000100)
#define U_PNIP__FLOWCONTROL_P4__DISABLEPORT                                       (0x00000001)
#define U_PNIP__FLOWCONTROL_P4_RESET__VALUE                      0x1
#define U_PNIP__BLOCKED_PRIOMASK_DP1_P4                                           (0x10604008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BLOCKED_PRIOMASK_DP1_P4__BLOCKED_PRIORITY_DP1_15_14               (0x0000C000)
#define U_PNIP__BLOCKED_PRIOMASK_DP1_P4__BLOCKED_PRIORITY_DP1_11_0                (0x00000FFF)
#define U_PNIP__BLOCKED_PRIOMASK_DP1_P4_RESET__VALUE             0x0
#define U_PNIP__BLOCKED_PRIOMASK_DP1R_P4                                          (0x1060400C + PNIP_DEV_TOP_BASE)
#define U_PNIP__BLOCKED_PRIOMASK_DP1R_P4__BLOCKED_PRIORITY_DP1R_15_14             (0x0000C000)
#define U_PNIP__BLOCKED_PRIOMASK_DP1R_P4__BLOCKED_PRIORITY_DP1R_11_0              (0x00000FFF)
#define U_PNIP__BLOCKED_PRIOMASK_DP1R_P4_RESET__VALUE            0x0
#define U_PNIP__BLOCKED_PRIOMASK_DP1S_P4                                          (0x10604010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BLOCKED_PRIOMASK_DP1S_P4__BLOCKED_PRIORITY_DP1S_15_14             (0x0000C000)
#define U_PNIP__BLOCKED_PRIOMASK_DP1S_P4__BLOCKED_PRIORITY_DP1S_11_0              (0x00000FFF)
#define U_PNIP__BLOCKED_PRIOMASK_DP1S_P4_RESET__VALUE            0x0
#define U_PNIP__BLOCKED_PRIOMASK_DP2_P4                                           (0x10604014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BLOCKED_PRIOMASK_DP2_P4__BLOCKED_PRIORITY_DP2_15_14               (0x0000C000)
#define U_PNIP__BLOCKED_PRIOMASK_DP2_P4__BLOCKED_PRIORITY_DP2_11_0                (0x00000FFF)
#define U_PNIP__BLOCKED_PRIOMASK_DP2_P4_RESET__VALUE             0x0
#define U_PNIP__MACCONTROL_P4                                                     (0x10604018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MACCONTROL_P4__BYPASS_MII_SYNC                                    (0x00010000)
#define U_PNIP__MACCONTROL_P4__RECEIVE_HALTREQUEST                                (0x00000200)
#define U_PNIP__MACCONTROL_P4__RECEIVE_ENABLE                                     (0x00000100)
#define U_PNIP__MACCONTROL_P4__TRANSMIT_HALTREQUEST                               (0x00000002)
#define U_PNIP__MACCONTROL_P4__TRANSMIT_ENABLE                                    (0x00000001)
#define U_PNIP__MACCONTROL_P4_RESET__VALUE                       0x0
#define U_PNIP__MACSTATUS_P4                                                      (0x1060401C + PNIP_DEV_TOP_BASE)
#define U_PNIP__MACSTATUS_P4__VLAN_TAGGED_RECEIVE                                 (0x00008000)
#define U_PNIP__MACSTATUS_P4__BROADCAST_RECEIVE                                   (0x00004000)
#define U_PNIP__MACSTATUS_P4__MULTICAST_RECEIVE                                   (0x00002000)
#define U_PNIP__MACSTATUS_P4__UNICAST_RECEIVE                                     (0x00001000)
#define U_PNIP__MACSTATUS_P4__LONG_ERROR                                          (0x00000800)
#define U_PNIP__MACSTATUS_P4__SHORT_ERROR                                         (0x00000400)
#define U_PNIP__MACSTATUS_P4__ALIGNMENT_ERROR                                     (0x00000200)
#define U_PNIP__MACSTATUS_P4__CRC_ERROR                                           (0x00000100)
#define U_PNIP__MACSTATUS_P4__GOOD_RECEIVE                                        (0x00000080)
#define U_PNIP__MACSTATUS_P4__RECEIVE_COMPLETION                                  (0x00000040)
#define U_PNIP__MACSTATUS_P4__VLAN_TAGGED_TRANSMIT                                (0x00000020)
#define U_PNIP__MACSTATUS_P4__BROADCAST_TRANSMIT                                  (0x00000010)
#define U_PNIP__MACSTATUS_P4__MULTICAST_TRANSMIT                                  (0x00000008)
#define U_PNIP__MACSTATUS_P4__UNICAST_TRANSMIT                                    (0x00000004)
#define U_PNIP__MACSTATUS_P4__GOOD_TRANSMIT                                       (0x00000002)
#define U_PNIP__MACSTATUS_P4__TRANSMIT_COMPLETION                                 (0x00000001)
#define U_PNIP__MACSTATUS_P4_RESET__VALUE                        0x0
#define U_PNIP__MACERRORSTATUS_P4                                                 (0x10604020 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MACERRORSTATUS_P4__MACTCLOCKDRIFTERR                              (0x00001000)
#define U_PNIP__MACERRORSTATUS_P4__MACTCLOCKERR                                   (0x00000800)
#define U_PNIP__MACERRORSTATUS_P4__MACTCRCDOUBLECHECKERR                          (0x00000400)
#define U_PNIP__MACERRORSTATUS_P4__MACTBUFFERUNDERFLOWERR                         (0x00000200)
#define U_PNIP__MACERRORSTATUS_P4__MACTBUFFEROVERFLOWERR                          (0x00000100)
#define U_PNIP__MACERRORSTATUS_P4__MACRFRAMEOVERFLOWERR                           (0x00000020)
#define U_PNIP__MACERRORSTATUS_P4__MACRBUFFERDISCARD                              (0x00000010)
#define U_PNIP__MACERRORSTATUS_P4__MACRFRAMEGAPERR                                (0x00000008)
#define U_PNIP__MACERRORSTATUS_P4__MACRCRCDOUBLECHECKERR                          (0x00000004)
#define U_PNIP__MACERRORSTATUS_P4__MACRBUFFERUNDERFLOWERR                         (0x00000002)
#define U_PNIP__MACERRORSTATUS_P4__MACRBUFFEROVERFLOWERR                          (0x00000001)
#define U_PNIP__MACERRORSTATUS_P4_RESET__VALUE                   0x0
#define U_PNIP__LINEDELAY_P4                                                      (0x10604024 + PNIP_DEV_TOP_BASE)
#define U_PNIP__LINEDELAY_P4__LINEDELAY                                           (0xFFFFFFFF)
#define U_PNIP__LINEDELAY_P4_RESET__VALUE                        0x0
#define U_PNIP__MINIMUMIFG_P4                                                     (0x10604028 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MINIMUMIFG_P4__MINIFG_1G                                          (0x00000700)
#define U_PNIP__MINIMUMIFG_P4__MINIFG_100M                                        (0x00000007)
#define U_PNIP__MINIMUMIFG_P4_RESET__VALUE                       0x300
#define U_PNIP__PREAMBLELENGTH_P4                                                 (0x1060402C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PREAMBLELENGTH_P4__RXD_MINLENGTH_1G                               (0x07000000)
#define U_PNIP__PREAMBLELENGTH_P4__TXD_LENGTH_1G                                  (0x00070000)
#define U_PNIP__PREAMBLELENGTH_P4__RXD_MINLENGTH_100M                             (0x00000700)
#define U_PNIP__PREAMBLELENGTH_P4__TXD_LENGTH_100M                                (0x00000007)
#define U_PNIP__PREAMBLELENGTH_P4_RESET__VALUE                   0x70007
#define U_PNIP__RED_PRIOMASK_P4                                                   (0x10604030 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RED_PRIOMASK_P4__QUEUE_PRIORITY_X                                 (0x00000FFF)
#define U_PNIP__RED_PRIOMASK_P4_RESET__VALUE                     0x100
#define U_PNIP__ORANGE_PRIOMASK_P4                                                (0x10604034 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ORANGE_PRIOMASK_P4__QUEUE_PRIORITY_X                              (0x00000FFF)
#define U_PNIP__ORANGE_PRIOMASK_P4_RESET__VALUE                  0xD80
#define U_PNIP__GREEN_PRIOMASK_P4                                                 (0x10604038 + PNIP_DEV_TOP_BASE)
#define U_PNIP__GREEN_PRIOMASK_P4__QUEUE_PRIORITY_X                               (0x00000FFF)
#define U_PNIP__GREEN_PRIOMASK_P4_RESET__VALUE                   0xFFF
#define U_PNIP__IRT_DELAYTIME_P4                                                  (0x1060403C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRT_DELAYTIME_P4__DELAY_MODE                                      (0x80000000)
#define U_PNIP__IRT_DELAYTIME_P4__DELAY_TIME                                      (0x0003FFFF)
#define U_PNIP__IRT_DELAYTIME_P4_RESET__VALUE                    0x0
#define U_PNIP__IRT_DELAYSTATUS_0_P4                                              (0x10604040 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRT_DELAYSTATUS_0_P4__ETHERTYPE                                   (0xFFFF0000)
#define U_PNIP__IRT_DELAYSTATUS_0_P4__FRAMEID                                     (0x0000FFFF)
#define U_PNIP__IRT_DELAYSTATUS_0_P4_RESET__VALUE                0x0
#define U_PNIP__IRT_DELAYSTATUS_1_P4                                              (0x10604044 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRT_DELAYSTATUS_1_P4__BUSY                                        (0x80000000)
#define U_PNIP__IRT_DELAYSTATUS_1_P4__TIMEBASE                                    (0x40000000)
#define U_PNIP__IRT_DELAYSTATUS_1_P4__SOURCE                                      (0x20000000)
#define U_PNIP__IRT_DELAYSTATUS_1_P4__TXTIME                                      (0x1FFFFFFF)
#define U_PNIP__IRT_DELAYSTATUS_1_P4_RESET__VALUE                0x0
#define U_PNIP__API_FORWARDINGMODE_P4                                             (0x10604048 + PNIP_DEV_TOP_BASE)
#define U_PNIP__API_FORWARDINGMODE_P4__API_FWMODE_15                              (0x00008000)
#define U_PNIP__API_FORWARDINGMODE_P4__API_FWMODE_14                              (0x00004000)
#define U_PNIP__API_FORWARDINGMODE_P4__API_FWMODE                                 (0x00000FFF)
#define U_PNIP__API_FORWARDINGMODE_P4_RESET__VALUE               0xCFFF
#define U_PNIP__FFWDELAY_CONTROL_P4                                               (0x1060404C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FFWDELAY_CONTROL_P4__EN_FFWDELAY_DP2                              (0x00000008)
#define U_PNIP__FFWDELAY_CONTROL_P4__EN_FFWDELAY_DP1S                             (0x00000004)
#define U_PNIP__FFWDELAY_CONTROL_P4__EN_FFWDELAY_DP1R                             (0x00000002)
#define U_PNIP__FFWDELAY_CONTROL_P4__EN_FFWDELAY_DP1                              (0x00000001)
#define U_PNIP__FFWDELAY_CONTROL_P4_RESET__VALUE                 0x7
#define U_PNIP__RXFRAMELENGTH_P4                                                  (0x10604050 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RXFRAMELENGTH_P4__RXFRAMELENGTH                                   (0x000007FF)
#define U_PNIP__RXFRAMELENGTH_P4_RESET__VALUE                    0x600
#define U_PNIP__TXFRAMELENGTH_P4                                                  (0x10604054 + PNIP_DEV_TOP_BASE)
#define U_PNIP__TXFRAMELENGTH_P4__TXFRAMELENGTH                                   (0x000007FF)
#define U_PNIP__TXFRAMELENGTH_P4_RESET__VALUE                    0x5FC
#define U_PNIP__PHY_CONTROL                                                       (0x10608000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHY_CONTROL__MDIO_HOLD                                            (0x00030000)
#define U_PNIP__PHY_CONTROL__TRANSFERRATE_DIV                                     (0x0000FF00)
#define U_PNIP__PHY_CONTROL__PHY_ACTIVATE                                         (0x00000001)
#define U_PNIP__PHY_CONTROL_RESET__VALUE                         0x13200
#define U_PNIP__MD_CA                                                             (0x10608004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MD_CA__WAITTIME_PREAMBLE                                          (0x0003F000)
#define U_PNIP__MD_CA__BUSY                                                       (0x00000800)
#define U_PNIP__MD_CA__WRITE                                                      (0x00000400)
#define U_PNIP__MD_CA__PHY_ADDRESS                                                (0x000003E0)
#define U_PNIP__MD_CA__ADDRESS                                                    (0x0000001F)
#define U_PNIP__MD_CA_RESET__VALUE                               0x20000
#define U_PNIP__MD_DATA                                                           (0x10608008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MD_DATA__MD_DATA                                                  (0x0000FFFF)
#define U_PNIP__MD_DATA_RESET__VALUE                             0x0
#define U_PNIP__STATUSCHANGE                                                      (0x1060800C + PNIP_DEV_TOP_BASE)
#define U_PNIP__STATUSCHANGE__PHY_PWRUP_P4                                        (0x08000000)
#define U_PNIP__STATUSCHANGE__STATUSCHANGE3_P4                                    (0x04000000)
#define U_PNIP__STATUSCHANGE__STATUSCHANGE2_P4                                    (0x02000000)
#define U_PNIP__STATUSCHANGE__STATUSCHANGE1_P4                                    (0x01000000)
#define U_PNIP__STATUSCHANGE__PHY_PWRUP_P3                                        (0x00080000)
#define U_PNIP__STATUSCHANGE__STATUSCHANGE3_P3                                    (0x00040000)
#define U_PNIP__STATUSCHANGE__STATUSCHANGE2_P3                                    (0x00020000)
#define U_PNIP__STATUSCHANGE__STATUSCHANGE1_P3                                    (0x00010000)
#define U_PNIP__STATUSCHANGE__PHY_PWRUP_P2                                        (0x00000800)
#define U_PNIP__STATUSCHANGE__STATUSCHANGE3_P2                                    (0x00000400)
#define U_PNIP__STATUSCHANGE__STATUSCHANGE2_P2                                    (0x00000200)
#define U_PNIP__STATUSCHANGE__STATUSCHANGE1_P2                                    (0x00000100)
#define U_PNIP__STATUSCHANGE__PHY_PWRUP_P1                                        (0x00000008)
#define U_PNIP__STATUSCHANGE__STATUSCHANGE3_P1                                    (0x00000004)
#define U_PNIP__STATUSCHANGE__STATUSCHANGE2_P1                                    (0x00000002)
#define U_PNIP__STATUSCHANGE__STATUSCHANGE1_P1                                    (0x00000001)
#define U_PNIP__STATUSCHANGE_RESET__VALUE                        0x0
#define U_PNIP__PHYCOMMAND_P1                                                     (0x10608010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYCOMMAND_P1__EN_PWRUPRST                                        (0x00001000)
#define U_PNIP__PHYCOMMAND_P1__WAITTIME_PREAMBLE                                  (0x00000FC0)
#define U_PNIP__PHYCOMMAND_P1__ENABLE                                             (0x00000020)
#define U_PNIP__PHYCOMMAND_P1__PHY_ADDRESS                                        (0x0000001F)
#define U_PNIP__PHYCOMMAND_P1_RESET__VALUE                       0x800
#define U_PNIP__PHYOUI1_SELECT_P1                                                 (0x10608014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYOUI1_SELECT_P1__OUI1                                           (0xFFFF0000)
#define U_PNIP__PHYOUI1_SELECT_P1__COMPARE_MODE                                   (0x00000040)
#define U_PNIP__PHYOUI1_SELECT_P1__ENABLE                                         (0x00000020)
#define U_PNIP__PHYOUI1_SELECT_P1__REGISTER_ADDRESS                               (0x0000001F)
#define U_PNIP__PHYOUI1_SELECT_P1_RESET__VALUE                   0x2
#define U_PNIP__PHYOUI2_SELECT_P1                                                 (0x10608018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYOUI2_SELECT_P1__OUI2                                           (0xFFFF0000)
#define U_PNIP__PHYOUI2_SELECT_P1__COMPARE_MODE                                   (0x00000040)
#define U_PNIP__PHYOUI2_SELECT_P1__ENABLE                                         (0x00000020)
#define U_PNIP__PHYOUI2_SELECT_P1__REGISTER_ADDRESS                               (0x0000001F)
#define U_PNIP__PHYOUI2_SELECT_P1_RESET__VALUE                   0x3
#define U_PNIP__PHYSTATUS1_SELECT_P1                                              (0x1060801C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYSTATUS1_SELECT_P1__BIT_MASK                                    (0xFFFF0000)
#define U_PNIP__PHYSTATUS1_SELECT_P1__LINKCHECK_INDEX                             (0x0000F000)
#define U_PNIP__PHYSTATUS1_SELECT_P1__LINKCHECK_LEVEL                             (0x00000200)
#define U_PNIP__PHYSTATUS1_SELECT_P1__LINKCHECK_ENABLE                            (0x00000100)
#define U_PNIP__PHYSTATUS1_SELECT_P1__ENABLE                                      (0x00000020)
#define U_PNIP__PHYSTATUS1_SELECT_P1__REGISTER_ADDRESS                            (0x0000001F)
#define U_PNIP__PHYSTATUS1_SELECT_P1_RESET__VALUE                0x42001
#define U_PNIP__PHYSTATUS2_SELECT_P1                                              (0x10608020 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYSTATUS2_SELECT_P1__BIT_MASK                                    (0xFFFF0000)
#define U_PNIP__PHYSTATUS2_SELECT_P1__ENABLE                                      (0x00000020)
#define U_PNIP__PHYSTATUS2_SELECT_P1__REGISTER_ADDRESS                            (0x0000001F)
#define U_PNIP__PHYSTATUS2_SELECT_P1_RESET__VALUE                0x4001F
#define U_PNIP__PHYSTATUS3_SELECT_P1                                              (0x10608024 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYSTATUS3_SELECT_P1__BIT_MASK                                    (0xFFFF0000)
#define U_PNIP__PHYSTATUS3_SELECT_P1__ENABLE                                      (0x00000020)
#define U_PNIP__PHYSTATUS3_SELECT_P1__REGISTER_ADDRESS                            (0x0000001F)
#define U_PNIP__PHYSTATUS3_SELECT_P1_RESET__VALUE                0x10001F
#define U_PNIP__PHYOUI12_P1                                                       (0x10608028 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYOUI12_P1__PHY_OUI2                                             (0xFFFF0000)
#define U_PNIP__PHYOUI12_P1__PHY_OUI1                                             (0x0000FFFF)
#define U_PNIP__PHYOUI12_P1_RESET__VALUE                         0x0
#define U_PNIP__PHYSTATUS1_P1                                                     (0x1060802C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYSTATUS1_P1__POLLSTATE                                          (0x00010000)
#define U_PNIP__PHYSTATUS1_P1__PHY_STATUS1                                        (0x0000FFFF)
#define U_PNIP__PHYSTATUS1_P1_RESET__VALUE                       0x0
#define U_PNIP__PHYSTATUS2_P1                                                     (0x10608030 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYSTATUS2_P1__POLLSTATE                                          (0x00010000)
#define U_PNIP__PHYSTATUS2_P1__PHY_STATUS2                                        (0x0000FFFF)
#define U_PNIP__PHYSTATUS2_P1_RESET__VALUE                       0x0
#define U_PNIP__PHYSTATUS3_P1                                                     (0x10608034 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYSTATUS3_P1__POLLSTATE                                          (0x00010000)
#define U_PNIP__PHYSTATUS3_P1__PHY_STATUS3                                        (0x0000FFFF)
#define U_PNIP__PHYSTATUS3_P1_RESET__VALUE                       0x0
#define U_PNIP__PHYCOMMAND_P2                                                     (0x10608038 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYCOMMAND_P2__EN_PWRUPRST                                        (0x00001000)
#define U_PNIP__PHYCOMMAND_P2__WAITTIME_PREAMBLE                                  (0x00000FC0)
#define U_PNIP__PHYCOMMAND_P2__ENABLE                                             (0x00000020)
#define U_PNIP__PHYCOMMAND_P2__PHY_ADDRESS                                        (0x0000001F)
#define U_PNIP__PHYCOMMAND_P2_RESET__VALUE                       0x800
#define U_PNIP__PHYOUI1_SELECT_P2                                                 (0x1060803C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYOUI1_SELECT_P2__OUI1                                           (0xFFFF0000)
#define U_PNIP__PHYOUI1_SELECT_P2__COMPARE_MODE                                   (0x00000040)
#define U_PNIP__PHYOUI1_SELECT_P2__ENABLE                                         (0x00000020)
#define U_PNIP__PHYOUI1_SELECT_P2__REGISTER_ADDRESS                               (0x0000001F)
#define U_PNIP__PHYOUI1_SELECT_P2_RESET__VALUE                   0x2
#define U_PNIP__PHYOUI2_SELECT_P2                                                 (0x10608040 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYOUI2_SELECT_P2__OUI2                                           (0xFFFF0000)
#define U_PNIP__PHYOUI2_SELECT_P2__COMPARE_MODE                                   (0x00000040)
#define U_PNIP__PHYOUI2_SELECT_P2__ENABLE                                         (0x00000020)
#define U_PNIP__PHYOUI2_SELECT_P2__REGISTER_ADDRESS                               (0x0000001F)
#define U_PNIP__PHYOUI2_SELECT_P2_RESET__VALUE                   0x3
#define U_PNIP__PHYSTATUS1_SELECT_P2                                              (0x10608044 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYSTATUS1_SELECT_P2__BIT_MASK                                    (0xFFFF0000)
#define U_PNIP__PHYSTATUS1_SELECT_P2__LINKCHECK_INDEX                             (0x0000F000)
#define U_PNIP__PHYSTATUS1_SELECT_P2__LINKCHECK_LEVEL                             (0x00000200)
#define U_PNIP__PHYSTATUS1_SELECT_P2__LINKCHECK_ENABLE                            (0x00000100)
#define U_PNIP__PHYSTATUS1_SELECT_P2__ENABLE                                      (0x00000020)
#define U_PNIP__PHYSTATUS1_SELECT_P2__REGISTER_ADDRESS                            (0x0000001F)
#define U_PNIP__PHYSTATUS1_SELECT_P2_RESET__VALUE                0x42001
#define U_PNIP__PHYSTATUS2_SELECT_P2                                              (0x10608048 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYSTATUS2_SELECT_P2__BIT_MASK                                    (0xFFFF0000)
#define U_PNIP__PHYSTATUS2_SELECT_P2__ENABLE                                      (0x00000020)
#define U_PNIP__PHYSTATUS2_SELECT_P2__REGISTER_ADDRESS                            (0x0000001F)
#define U_PNIP__PHYSTATUS2_SELECT_P2_RESET__VALUE                0x4001F
#define U_PNIP__PHYSTATUS3_SELECT_P2                                              (0x1060804C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYSTATUS3_SELECT_P2__BIT_MASK                                    (0xFFFF0000)
#define U_PNIP__PHYSTATUS3_SELECT_P2__ENABLE                                      (0x00000020)
#define U_PNIP__PHYSTATUS3_SELECT_P2__REGISTER_ADDRESS                            (0x0000001F)
#define U_PNIP__PHYSTATUS3_SELECT_P2_RESET__VALUE                0x10001F
#define U_PNIP__PHYOUI12_P2                                                       (0x10608050 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYOUI12_P2__PHY_OUI2                                             (0xFFFF0000)
#define U_PNIP__PHYOUI12_P2__PHY_OUI1                                             (0x0000FFFF)
#define U_PNIP__PHYOUI12_P2_RESET__VALUE                         0x0
#define U_PNIP__PHYSTATUS1_P2                                                     (0x10608054 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYSTATUS1_P2__POLLSTATE                                          (0x00010000)
#define U_PNIP__PHYSTATUS1_P2__PHY_STATUS1                                        (0x0000FFFF)
#define U_PNIP__PHYSTATUS1_P2_RESET__VALUE                       0x0
#define U_PNIP__PHYSTATUS2_P2                                                     (0x10608058 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYSTATUS2_P2__POLLSTATE                                          (0x00010000)
#define U_PNIP__PHYSTATUS2_P2__PHY_STATUS2                                        (0x0000FFFF)
#define U_PNIP__PHYSTATUS2_P2_RESET__VALUE                       0x0
#define U_PNIP__PHYSTATUS3_P2                                                     (0x1060805C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYSTATUS3_P2__POLLSTATE                                          (0x00010000)
#define U_PNIP__PHYSTATUS3_P2__PHY_STATUS3                                        (0x0000FFFF)
#define U_PNIP__PHYSTATUS3_P2_RESET__VALUE                       0x0
#define U_PNIP__PHYCOMMAND_P3                                                     (0x10608060 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYCOMMAND_P3__EN_PWRUPRST                                        (0x00001000)
#define U_PNIP__PHYCOMMAND_P3__WAITTIME_PREAMBLE                                  (0x00000FC0)
#define U_PNIP__PHYCOMMAND_P3__ENABLE                                             (0x00000020)
#define U_PNIP__PHYCOMMAND_P3__PHY_ADDRESS                                        (0x0000001F)
#define U_PNIP__PHYCOMMAND_P3_RESET__VALUE                       0x800
#define U_PNIP__PHYOUI1_SELECT_P3                                                 (0x10608064 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYOUI1_SELECT_P3__OUI1                                           (0xFFFF0000)
#define U_PNIP__PHYOUI1_SELECT_P3__COMPARE_MODE                                   (0x00000040)
#define U_PNIP__PHYOUI1_SELECT_P3__ENABLE                                         (0x00000020)
#define U_PNIP__PHYOUI1_SELECT_P3__REGISTER_ADDRESS                               (0x0000001F)
#define U_PNIP__PHYOUI1_SELECT_P3_RESET__VALUE                   0x2
#define U_PNIP__PHYOUI2_SELECT_P3                                                 (0x10608068 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYOUI2_SELECT_P3__OUI2                                           (0xFFFF0000)
#define U_PNIP__PHYOUI2_SELECT_P3__COMPARE_MODE                                   (0x00000040)
#define U_PNIP__PHYOUI2_SELECT_P3__ENABLE                                         (0x00000020)
#define U_PNIP__PHYOUI2_SELECT_P3__REGISTER_ADDRESS                               (0x0000001F)
#define U_PNIP__PHYOUI2_SELECT_P3_RESET__VALUE                   0x3
#define U_PNIP__PHYSTATUS1_SELECT_P3                                              (0x1060806C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYSTATUS1_SELECT_P3__BIT_MASK                                    (0xFFFF0000)
#define U_PNIP__PHYSTATUS1_SELECT_P3__LINKCHECK_INDEX                             (0x0000F000)
#define U_PNIP__PHYSTATUS1_SELECT_P3__LINKCHECK_LEVEL                             (0x00000200)
#define U_PNIP__PHYSTATUS1_SELECT_P3__LINKCHECK_ENABLE                            (0x00000100)
#define U_PNIP__PHYSTATUS1_SELECT_P3__ENABLE                                      (0x00000020)
#define U_PNIP__PHYSTATUS1_SELECT_P3__REGISTER_ADDRESS                            (0x0000001F)
#define U_PNIP__PHYSTATUS1_SELECT_P3_RESET__VALUE                0x42001
#define U_PNIP__PHYSTATUS2_SELECT_P3                                              (0x10608070 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYSTATUS2_SELECT_P3__BIT_MASK                                    (0xFFFF0000)
#define U_PNIP__PHYSTATUS2_SELECT_P3__ENABLE                                      (0x00000020)
#define U_PNIP__PHYSTATUS2_SELECT_P3__REGISTER_ADDRESS                            (0x0000001F)
#define U_PNIP__PHYSTATUS2_SELECT_P3_RESET__VALUE                0x4001F
#define U_PNIP__PHYSTATUS3_SELECT_P3                                              (0x10608074 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYSTATUS3_SELECT_P3__BIT_MASK                                    (0xFFFF0000)
#define U_PNIP__PHYSTATUS3_SELECT_P3__ENABLE                                      (0x00000020)
#define U_PNIP__PHYSTATUS3_SELECT_P3__REGISTER_ADDRESS                            (0x0000001F)
#define U_PNIP__PHYSTATUS3_SELECT_P3_RESET__VALUE                0x10001F
#define U_PNIP__PHYOUI12_P3                                                       (0x10608078 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYOUI12_P3__PHY_OUI2                                             (0xFFFF0000)
#define U_PNIP__PHYOUI12_P3__PHY_OUI1                                             (0x0000FFFF)
#define U_PNIP__PHYOUI12_P3_RESET__VALUE                         0x0
#define U_PNIP__PHYSTATUS1_P3                                                     (0x1060807C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYSTATUS1_P3__POLLSTATE                                          (0x00010000)
#define U_PNIP__PHYSTATUS1_P3__PHY_STATUS1                                        (0x0000FFFF)
#define U_PNIP__PHYSTATUS1_P3_RESET__VALUE                       0x0
#define U_PNIP__PHYSTATUS2_P3                                                     (0x10608080 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYSTATUS2_P3__POLLSTATE                                          (0x00010000)
#define U_PNIP__PHYSTATUS2_P3__PHY_STATUS2                                        (0x0000FFFF)
#define U_PNIP__PHYSTATUS2_P3_RESET__VALUE                       0x0
#define U_PNIP__PHYSTATUS3_P3                                                     (0x10608084 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYSTATUS3_P3__POLLSTATE                                          (0x00010000)
#define U_PNIP__PHYSTATUS3_P3__PHY_STATUS3                                        (0x0000FFFF)
#define U_PNIP__PHYSTATUS3_P3_RESET__VALUE                       0x0
#define U_PNIP__PHYCOMMAND_P4                                                     (0x10608088 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYCOMMAND_P4__EN_PWRUPRST                                        (0x00001000)
#define U_PNIP__PHYCOMMAND_P4__WAITTIME_PREAMBLE                                  (0x00000FC0)
#define U_PNIP__PHYCOMMAND_P4__ENABLE                                             (0x00000020)
#define U_PNIP__PHYCOMMAND_P4__PHY_ADDRESS                                        (0x0000001F)
#define U_PNIP__PHYCOMMAND_P4_RESET__VALUE                       0x800
#define U_PNIP__PHYOUI1_SELECT_P4                                                 (0x1060808C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYOUI1_SELECT_P4__OUI1                                           (0xFFFF0000)
#define U_PNIP__PHYOUI1_SELECT_P4__COMPARE_MODE                                   (0x00000040)
#define U_PNIP__PHYOUI1_SELECT_P4__ENABLE                                         (0x00000020)
#define U_PNIP__PHYOUI1_SELECT_P4__REGISTER_ADDRESS                               (0x0000001F)
#define U_PNIP__PHYOUI1_SELECT_P4_RESET__VALUE                   0x2
#define U_PNIP__PHYOUI2_SELECT_P4                                                 (0x10608090 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYOUI2_SELECT_P4__OUI2                                           (0xFFFF0000)
#define U_PNIP__PHYOUI2_SELECT_P4__COMPARE_MODE                                   (0x00000040)
#define U_PNIP__PHYOUI2_SELECT_P4__ENABLE                                         (0x00000020)
#define U_PNIP__PHYOUI2_SELECT_P4__REGISTER_ADDRESS                               (0x0000001F)
#define U_PNIP__PHYOUI2_SELECT_P4_RESET__VALUE                   0x3
#define U_PNIP__PHYSTATUS1_SELECT_P4                                              (0x10608094 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYSTATUS1_SELECT_P4__BIT_MASK                                    (0xFFFF0000)
#define U_PNIP__PHYSTATUS1_SELECT_P4__LINKCHECK_INDEX                             (0x0000F000)
#define U_PNIP__PHYSTATUS1_SELECT_P4__LINKCHECK_LEVEL                             (0x00000200)
#define U_PNIP__PHYSTATUS1_SELECT_P4__LINKCHECK_ENABLE                            (0x00000100)
#define U_PNIP__PHYSTATUS1_SELECT_P4__ENABLE                                      (0x00000020)
#define U_PNIP__PHYSTATUS1_SELECT_P4__REGISTER_ADDRESS                            (0x0000001F)
#define U_PNIP__PHYSTATUS1_SELECT_P4_RESET__VALUE                0x42001
#define U_PNIP__PHYSTATUS2_SELECT_P4                                              (0x10608098 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYSTATUS2_SELECT_P4__BIT_MASK                                    (0xFFFF0000)
#define U_PNIP__PHYSTATUS2_SELECT_P4__ENABLE                                      (0x00000020)
#define U_PNIP__PHYSTATUS2_SELECT_P4__REGISTER_ADDRESS                            (0x0000001F)
#define U_PNIP__PHYSTATUS2_SELECT_P4_RESET__VALUE                0x4001F
#define U_PNIP__PHYSTATUS3_SELECT_P4                                              (0x1060809C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYSTATUS3_SELECT_P4__BIT_MASK                                    (0xFFFF0000)
#define U_PNIP__PHYSTATUS3_SELECT_P4__ENABLE                                      (0x00000020)
#define U_PNIP__PHYSTATUS3_SELECT_P4__REGISTER_ADDRESS                            (0x0000001F)
#define U_PNIP__PHYSTATUS3_SELECT_P4_RESET__VALUE                0x10001F
#define U_PNIP__PHYOUI12_P4                                                       (0x106080A0 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYOUI12_P4__PHY_OUI2                                             (0xFFFF0000)
#define U_PNIP__PHYOUI12_P4__PHY_OUI1                                             (0x0000FFFF)
#define U_PNIP__PHYOUI12_P4_RESET__VALUE                         0x0
#define U_PNIP__PHYSTATUS1_P4                                                     (0x106080A4 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYSTATUS1_P4__POLLSTATE                                          (0x00010000)
#define U_PNIP__PHYSTATUS1_P4__PHY_STATUS1                                        (0x0000FFFF)
#define U_PNIP__PHYSTATUS1_P4_RESET__VALUE                       0x0
#define U_PNIP__PHYSTATUS2_P4                                                     (0x106080A8 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYSTATUS2_P4__POLLSTATE                                          (0x00010000)
#define U_PNIP__PHYSTATUS2_P4__PHY_STATUS2                                        (0x0000FFFF)
#define U_PNIP__PHYSTATUS2_P4_RESET__VALUE                       0x0
#define U_PNIP__PHYSTATUS3_P4                                                     (0x106080AC + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHYSTATUS3_P4__POLLSTATE                                          (0x00010000)
#define U_PNIP__PHYSTATUS3_P4__PHY_STATUS3                                        (0x0000FFFF)
#define U_PNIP__PHYSTATUS3_P4_RESET__VALUE                       0x0
#define U_PNIP__GOOD_BYTES_RECEIVED_P1                                            (0x10609000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__GOOD_BYTES_RECEIVED_P1__GOOD_BYTES_RECEIVED                       (0xFFFFFFFF)
#define U_PNIP__GOOD_BYTES_RECEIVED_P1_RESET__VALUE              0x0
#define U_PNIP__BAD_BYTES_RECEIVED_P1                                             (0x10609004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BAD_BYTES_RECEIVED_P1__BAD_BYTES_RECEIVED                         (0xFFFFFFFF)
#define U_PNIP__BAD_BYTES_RECEIVED_P1_RESET__VALUE               0x0
#define U_PNIP__TOTAL_BYTES_RECEIVED_P1                                           (0x10609008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__TOTAL_BYTES_RECEIVED_P1__TOTAL_BYTES_RECEIVED                     (0xFFFFFFFF)
#define U_PNIP__TOTAL_BYTES_RECEIVED_P1_RESET__VALUE             0x0
#define U_PNIP__GOOD_FRAMES_RECEIVED_P1                                           (0x1060900C + PNIP_DEV_TOP_BASE)
#define U_PNIP__GOOD_FRAMES_RECEIVED_P1__GOOD_FRAMES_RECEIVED                     (0xFFFFFFFF)
#define U_PNIP__GOOD_FRAMES_RECEIVED_P1_RESET__VALUE             0x0
#define U_PNIP__BAD_FRAMES_RECEIVED_P1                                            (0x10609010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BAD_FRAMES_RECEIVED_P1__BAD_FRAMES_RECEIVED                       (0xFFFFFFFF)
#define U_PNIP__BAD_FRAMES_RECEIVED_P1_RESET__VALUE              0x0
#define U_PNIP__TOTAL_FRAMES_RECEIVED_P1                                          (0x10609014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__TOTAL_FRAMES_RECEIVED_P1__TOTAL_FRAMES_RECEIVED                   (0xFFFFFFFF)
#define U_PNIP__TOTAL_FRAMES_RECEIVED_P1_RESET__VALUE            0x0
#define U_PNIP__UC_FRAMES_RECEIVED_P1                                             (0x10609018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__UC_FRAMES_RECEIVED_P1__UC_FRAMES_RECEIVED                         (0xFFFFFFFF)
#define U_PNIP__UC_FRAMES_RECEIVED_P1_RESET__VALUE               0x0
#define U_PNIP__UC_FRAMES_RECEIVED_UNKNOWN_P1                                     (0x1060901C + PNIP_DEV_TOP_BASE)
#define U_PNIP__UC_FRAMES_RECEIVED_UNKNOWN_P1__UC_FRAMES_RECEIVED_UNKNOWN         (0xFFFFFFFF)
#define U_PNIP__UC_FRAMES_RECEIVED_UNKNOWN_P1_RESET__VALUE       0x0
#define U_PNIP__MC_FRAMES_RECEIVED_P1                                             (0x10609020 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MC_FRAMES_RECEIVED_P1__MC_FRAMES_RECEIVED                         (0xFFFFFFFF)
#define U_PNIP__MC_FRAMES_RECEIVED_P1_RESET__VALUE               0x0
#define U_PNIP__MC_FRAMES_RECEIVED_UNKNOWN_P1                                     (0x10609024 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MC_FRAMES_RECEIVED_UNKNOWN_P1__MC_FRAMES_RECEIVED_UNKNOWN         (0xFFFFFFFF)
#define U_PNIP__MC_FRAMES_RECEIVED_UNKNOWN_P1_RESET__VALUE       0x0
#define U_PNIP__BROADCAST_FRAMES_RECEIVED_P1                                      (0x10609028 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BROADCAST_FRAMES_RECEIVED_P1__BROADCAST_FRAMES_RECEIVED           (0xFFFFFFFF)
#define U_PNIP__BROADCAST_FRAMES_RECEIVED_P1_RESET__VALUE        0x0
#define U_PNIP__CRC_ALIGNMENT_ERR_P1                                              (0x1060902C + PNIP_DEV_TOP_BASE)
#define U_PNIP__CRC_ALIGNMENT_ERR_P1__CRC_ALIGNMENT_ERR                           (0xFFFFFFFF)
#define U_PNIP__CRC_ALIGNMENT_ERR_P1_RESET__VALUE                0x0
#define U_PNIP__FRAMES_DROPPED_L1_P1                                              (0x10609030 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_DROPPED_L1_P1__FRAMES_DROPPED_L1                           (0xFFFFFFFF)
#define U_PNIP__FRAMES_DROPPED_L1_P1_RESET__VALUE                0x0
#define U_PNIP__FRAMES_DROPPED_L2_P1                                              (0x10609034 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_DROPPED_L2_P1__FRAMES_DROPPED_L2                           (0xFFFFFFFF)
#define U_PNIP__FRAMES_DROPPED_L2_P1_RESET__VALUE                0x0
#define U_PNIP__FRAMES_DROPPED_L3_P1                                              (0x10609038 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_DROPPED_L3_P1__FRAMES_DROPPED_L3                           (0xFFFFFFFF)
#define U_PNIP__FRAMES_DROPPED_L3_P1_RESET__VALUE                0x0
#define U_PNIP__HOL_CROSSED_P1                                                    (0x1060903C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_CROSSED_P1__HOL_CROSSED                                       (0xFFFFFFFF)
#define U_PNIP__HOL_CROSSED_P1_RESET__VALUE                      0x0
#define U_PNIP__QCW_CROSSED_P1                                                    (0x10609040 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_CROSSED_P1__QCW_CROSSED                                       (0xFFFFFFFF)
#define U_PNIP__QCW_CROSSED_P1_RESET__VALUE                      0x0
#define U_PNIP__UC_DEFAULT_FORWARDING_P1                                          (0x10609044 + PNIP_DEV_TOP_BASE)
#define U_PNIP__UC_DEFAULT_FORWARDING_P1__UC_DEFAULT_FW                           (0xFFFFFFFF)
#define U_PNIP__UC_DEFAULT_FORWARDING_P1_RESET__VALUE            0x0
#define U_PNIP__MC_DEFAULT_FORWARDING_P1                                          (0x10609048 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MC_DEFAULT_FORWARDING_P1__MC_DEFAULT_FW                           (0xFFFFFFFF)
#define U_PNIP__MC_DEFAULT_FORWARDING_P1_RESET__VALUE            0x0
#define U_PNIP__FRAMEAGING_CROSSED_P1                                             (0x1060904C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMEAGING_CROSSED_P1__FRAMEAGING_PX                              (0xFFFFFFFF)
#define U_PNIP__FRAMEAGING_CROSSED_P1_RESET__VALUE               0x0
#define U_PNIP__TOTAL_BYTES_SENT_P1                                               (0x10609050 + PNIP_DEV_TOP_BASE)
#define U_PNIP__TOTAL_BYTES_SENT_P1__TOTAL_BYTES_SENT                             (0xFFFFFFFF)
#define U_PNIP__TOTAL_BYTES_SENT_P1_RESET__VALUE                 0x0
#define U_PNIP__TOTAL_FRAMES_SENT_P1                                              (0x10609054 + PNIP_DEV_TOP_BASE)
#define U_PNIP__TOTAL_FRAMES_SENT_P1__TOTAL_FRAMES_SENT                           (0xFFFFFFFF)
#define U_PNIP__TOTAL_FRAMES_SENT_P1_RESET__VALUE                0x0
#define U_PNIP__UNICAST_FRAMES_SENT_P1                                            (0x10609058 + PNIP_DEV_TOP_BASE)
#define U_PNIP__UNICAST_FRAMES_SENT_P1__UNICAST_FRAMES_SENT                       (0xFFFFFFFF)
#define U_PNIP__UNICAST_FRAMES_SENT_P1_RESET__VALUE              0x0
#define U_PNIP__MULTICAST_FRAMES_SENT_P1                                          (0x1060905C + PNIP_DEV_TOP_BASE)
#define U_PNIP__MULTICAST_FRAMES_SENT_P1__MULTICAST_FRAMES_SENT                   (0xFFFFFFFF)
#define U_PNIP__MULTICAST_FRAMES_SENT_P1_RESET__VALUE            0x0
#define U_PNIP__BROADCAST_FRAMES_SENT_P1                                          (0x10609060 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BROADCAST_FRAMES_SENT_P1__BROADCAST_FRAMES_SENT                   (0xFFFFFFFF)
#define U_PNIP__BROADCAST_FRAMES_SENT_P1_RESET__VALUE            0x0
#define U_PNIP__IRT_DELAYTIME_CROSSED_P1                                          (0x10609064 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRT_DELAYTIME_CROSSED_P1__IRT_DELAYTIME_CROSSED                   (0xFFFFFFFF)
#define U_PNIP__IRT_DELAYTIME_CROSSED_P1_RESET__VALUE            0x0
#define U_PNIP__FRAMES_LESS_64BYTES_P1                                            (0x10609068 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_LESS_64BYTES_P1__FRAMES_LESS_64BYTES                       (0xFFFFFFFF)
#define U_PNIP__FRAMES_LESS_64BYTES_P1_RESET__VALUE              0x0
#define U_PNIP__FRAMES_WITH_64BYTES_P1                                            (0x1060906C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_WITH_64BYTES_P1__FRAMES_WITH_64BYTES                       (0xFFFFFFFF)
#define U_PNIP__FRAMES_WITH_64BYTES_P1_RESET__VALUE              0x0
#define U_PNIP__FRAMES_BETWEEN_65TO127BYTES_P1                                    (0x10609070 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_BETWEEN_65TO127BYTES_P1__FRAMES_BETWEEN_65TO127BYTES       (0xFFFFFFFF)
#define U_PNIP__FRAMES_BETWEEN_65TO127BYTES_P1_RESET__VALUE      0x0
#define U_PNIP__FRAMES_BETWEEN_128TO255BYTES_P1                                   (0x10609074 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_BETWEEN_128TO255BYTES_P1__FRAMES_BETWEEN_128TO255BYTES     (0xFFFFFFFF)
#define U_PNIP__FRAMES_BETWEEN_128TO255BYTES_P1_RESET__VALUE     0x0
#define U_PNIP__FRAMES_BETWEEN_256TO511BYTES_P1                                   (0x10609078 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_BETWEEN_256TO511BYTES_P1__FRAMES_BETWEEN_256TO511BYTES     (0xFFFFFFFF)
#define U_PNIP__FRAMES_BETWEEN_256TO511BYTES_P1_RESET__VALUE     0x0
#define U_PNIP__FRAMES_BETWEEN_512TO1023BYTES_P1                                  (0x1060907C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_BETWEEN_512TO1023BYTES_P1__FRAMES_BETWEEN_512TO1023BYTES   (0xFFFFFFFF)
#define U_PNIP__FRAMES_BETWEEN_512TO1023BYTES_P1_RESET__VALUE    0x0
#define U_PNIP__FRAMES_BETWEEN_1024TO1536BYTES_P1                                 (0x10609080 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_BETWEEN_1024TO1536BYTES_P1__FRAMES_BETWEEN_1024TO1536BYTES (0xFFFFFFFF)
#define U_PNIP__FRAMES_BETWEEN_1024TO1536BYTES_P1_RESET__VALUE   0x0
#define U_PNIP__OVERSIZE_FRAMES_P1                                                (0x10609084 + PNIP_DEV_TOP_BASE)
#define U_PNIP__OVERSIZE_FRAMES_P1__OVERSIZE_FRAMES                               (0xFFFFFFFF)
#define U_PNIP__OVERSIZE_FRAMES_P1_RESET__VALUE                  0x0
#define U_PNIP__ERRCODE_RXERR_P1                                                  (0x10609088 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_RXERR_P1__ERRCODE_RXERR_P1                                (0xFFFFFFFF)
#define U_PNIP__ERRCODE_RXERR_P1_RESET__VALUE                    0x0
#define U_PNIP__ERRCODE_FCSERR_P1                                                 (0x1060908C + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_FCSERR_P1__ERRCODE_FCSERR                                 (0xFFFFFFFF)
#define U_PNIP__ERRCODE_FCSERR_P1_RESET__VALUE                   0x0
#define U_PNIP__ERRCODE_LONGERR_P1                                                (0x10609090 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_LONGERR_P1__ERRCODE_LONGERR                               (0xFFFFFFFF)
#define U_PNIP__ERRCODE_LONGERR_P1_RESET__VALUE                  0x0
#define U_PNIP__ERRCODE_SHORTERR_P1                                               (0x10609094 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_SHORTERR_P1__ERRCODE_SHORTERR                             (0xFFFFFFFF)
#define U_PNIP__ERRCODE_SHORTERR_P1_RESET__VALUE                 0x0
#define U_PNIP__ERRCODE_DGFCSERR_P1                                               (0x10609098 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_DGFCSERR_P1__ERRCODE_DGFCSERR                             (0xFFFFFFFF)
#define U_PNIP__ERRCODE_DGFCSERR_P1_RESET__VALUE                 0x0
#define U_PNIP__ERRCODE_PACKSHORTERR_P1                                           (0x1060909C + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_PACKSHORTERR_P1__ERRCODE_PACKSHORTERR                     (0xFFFFFFFF)
#define U_PNIP__ERRCODE_PACKSHORTERR_P1_RESET__VALUE             0x0
#define U_PNIP__ERRCODE_PPMERR_P1                                                 (0x106090A0 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_PPMERR_P1__ERRCODE_PPMERR                                 (0xFFFFFFFF)
#define U_PNIP__ERRCODE_PPMERR_P1_RESET__VALUE                   0x0
#define U_PNIP__ERRCODE_NORESOURCEERR_P1                                          (0x106090A4 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_NORESOURCEERR_P1__ERRCODE_NORESOURCEERR                   (0xFFFFFFFF)
#define U_PNIP__ERRCODE_NORESOURCEERR_P1_RESET__VALUE            0x0
#define U_PNIP__ERRCODE_MACBUFFERERR_P1                                           (0x106090A8 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_MACBUFFERERR_P1__ERRCODE_MACBUFFERERR                     (0xFFFFFFFF)
#define U_PNIP__ERRCODE_MACBUFFERERR_P1_RESET__VALUE             0x0
#define U_PNIP__ERRCODE_MACSAERR_P1                                               (0x106090AC + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_MACSAERR_P1__ERRCODE_MACSAERR                             (0xFFFFFFFF)
#define U_PNIP__ERRCODE_MACSAERR_P1_RESET__VALUE                 0x0
#define U_PNIP__ERRCODE_DISABLEERR_P1                                             (0x106090B0 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_DISABLEERR_P1__ERRCODE_DISABLEERR                         (0xFFFFFFFF)
#define U_PNIP__ERRCODE_DISABLEERR_P1_RESET__VALUE               0x0
#define U_PNIP__ERRCODE_PACKPARERR_P1                                             (0x106090B4 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_PACKPARERR_P1__ERRCODE_PACKPARERR                         (0xFFFFFFFF)
#define U_PNIP__ERRCODE_PACKPARERR_P1_RESET__VALUE               0x0
#define U_PNIP__ERRCODE_PTCPERR_P1                                                (0x106090B8 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_PTCPERR_P1__ERRCODE_PTCPERR                               (0xFFFFFFFF)
#define U_PNIP__ERRCODE_PTCPERR_P1_RESET__VALUE                  0x0
#define U_PNIP__GOOD_BYTES_RECEIVED_P2                                            (0x1060A000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__GOOD_BYTES_RECEIVED_P2__GOOD_BYTES_RECEIVED                       (0xFFFFFFFF)
#define U_PNIP__GOOD_BYTES_RECEIVED_P2_RESET__VALUE              0x0
#define U_PNIP__BAD_BYTES_RECEIVED_P2                                             (0x1060A004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BAD_BYTES_RECEIVED_P2__BAD_BYTES_RECEIVED                         (0xFFFFFFFF)
#define U_PNIP__BAD_BYTES_RECEIVED_P2_RESET__VALUE               0x0
#define U_PNIP__TOTAL_BYTES_RECEIVED_P2                                           (0x1060A008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__TOTAL_BYTES_RECEIVED_P2__TOTAL_BYTES_RECEIVED                     (0xFFFFFFFF)
#define U_PNIP__TOTAL_BYTES_RECEIVED_P2_RESET__VALUE             0x0
#define U_PNIP__GOOD_FRAMES_RECEIVED_P2                                           (0x1060A00C + PNIP_DEV_TOP_BASE)
#define U_PNIP__GOOD_FRAMES_RECEIVED_P2__GOOD_FRAMES_RECEIVED                     (0xFFFFFFFF)
#define U_PNIP__GOOD_FRAMES_RECEIVED_P2_RESET__VALUE             0x0
#define U_PNIP__BAD_FRAMES_RECEIVED_P2                                            (0x1060A010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BAD_FRAMES_RECEIVED_P2__BAD_FRAMES_RECEIVED                       (0xFFFFFFFF)
#define U_PNIP__BAD_FRAMES_RECEIVED_P2_RESET__VALUE              0x0
#define U_PNIP__TOTAL_FRAMES_RECEIVED_P2                                          (0x1060A014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__TOTAL_FRAMES_RECEIVED_P2__TOTAL_FRAMES_RECEIVED                   (0xFFFFFFFF)
#define U_PNIP__TOTAL_FRAMES_RECEIVED_P2_RESET__VALUE            0x0
#define U_PNIP__UC_FRAMES_RECEIVED_P2                                             (0x1060A018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__UC_FRAMES_RECEIVED_P2__UC_FRAMES_RECEIVED                         (0xFFFFFFFF)
#define U_PNIP__UC_FRAMES_RECEIVED_P2_RESET__VALUE               0x0
#define U_PNIP__UC_FRAMES_RECEIVED_UNKNOWN_P2                                     (0x1060A01C + PNIP_DEV_TOP_BASE)
#define U_PNIP__UC_FRAMES_RECEIVED_UNKNOWN_P2__UC_FRAMES_RECEIVED_UNKNOWN         (0xFFFFFFFF)
#define U_PNIP__UC_FRAMES_RECEIVED_UNKNOWN_P2_RESET__VALUE       0x0
#define U_PNIP__MC_FRAMES_RECEIVED_P2                                             (0x1060A020 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MC_FRAMES_RECEIVED_P2__MC_FRAMES_RECEIVED                         (0xFFFFFFFF)
#define U_PNIP__MC_FRAMES_RECEIVED_P2_RESET__VALUE               0x0
#define U_PNIP__MC_FRAMES_RECEIVED_UNKNOWN_P2                                     (0x1060A024 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MC_FRAMES_RECEIVED_UNKNOWN_P2__MC_FRAMES_RECEIVED_UNKNOWN         (0xFFFFFFFF)
#define U_PNIP__MC_FRAMES_RECEIVED_UNKNOWN_P2_RESET__VALUE       0x0
#define U_PNIP__BROADCAST_FRAMES_RECEIVED_P2                                      (0x1060A028 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BROADCAST_FRAMES_RECEIVED_P2__BROADCAST_FRAMES_RECEIVED           (0xFFFFFFFF)
#define U_PNIP__BROADCAST_FRAMES_RECEIVED_P2_RESET__VALUE        0x0
#define U_PNIP__CRC_ALIGNMENT_ERR_P2                                              (0x1060A02C + PNIP_DEV_TOP_BASE)
#define U_PNIP__CRC_ALIGNMENT_ERR_P2__CRC_ALIGNMENT_ERR                           (0xFFFFFFFF)
#define U_PNIP__CRC_ALIGNMENT_ERR_P2_RESET__VALUE                0x0
#define U_PNIP__FRAMES_DROPPED_L1_P2                                              (0x1060A030 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_DROPPED_L1_P2__FRAMES_DROPPED_L1                           (0xFFFFFFFF)
#define U_PNIP__FRAMES_DROPPED_L1_P2_RESET__VALUE                0x0
#define U_PNIP__FRAMES_DROPPED_L2_P2                                              (0x1060A034 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_DROPPED_L2_P2__FRAMES_DROPPED_L2                           (0xFFFFFFFF)
#define U_PNIP__FRAMES_DROPPED_L2_P2_RESET__VALUE                0x0
#define U_PNIP__FRAMES_DROPPED_L3_P2                                              (0x1060A038 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_DROPPED_L3_P2__FRAMES_DROPPED_L3                           (0xFFFFFFFF)
#define U_PNIP__FRAMES_DROPPED_L3_P2_RESET__VALUE                0x0
#define U_PNIP__HOL_CROSSED_P2                                                    (0x1060A03C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_CROSSED_P2__HOL_CROSSED                                       (0xFFFFFFFF)
#define U_PNIP__HOL_CROSSED_P2_RESET__VALUE                      0x0
#define U_PNIP__QCW_CROSSED_P2                                                    (0x1060A040 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_CROSSED_P2__QCW_CROSSED                                       (0xFFFFFFFF)
#define U_PNIP__QCW_CROSSED_P2_RESET__VALUE                      0x0
#define U_PNIP__UC_DEFAULT_FORWARDING_P2                                          (0x1060A044 + PNIP_DEV_TOP_BASE)
#define U_PNIP__UC_DEFAULT_FORWARDING_P2__UC_DEFAULT_FW                           (0xFFFFFFFF)
#define U_PNIP__UC_DEFAULT_FORWARDING_P2_RESET__VALUE            0x0
#define U_PNIP__MC_DEFAULT_FORWARDING_P2                                          (0x1060A048 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MC_DEFAULT_FORWARDING_P2__MC_DEFAULT_FW                           (0xFFFFFFFF)
#define U_PNIP__MC_DEFAULT_FORWARDING_P2_RESET__VALUE            0x0
#define U_PNIP__FRAMEAGING_CROSSED_P2                                             (0x1060A04C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMEAGING_CROSSED_P2__FRAMEAGING_PX                              (0xFFFFFFFF)
#define U_PNIP__FRAMEAGING_CROSSED_P2_RESET__VALUE               0x0
#define U_PNIP__TOTAL_BYTES_SENT_P2                                               (0x1060A050 + PNIP_DEV_TOP_BASE)
#define U_PNIP__TOTAL_BYTES_SENT_P2__TOTAL_BYTES_SENT                             (0xFFFFFFFF)
#define U_PNIP__TOTAL_BYTES_SENT_P2_RESET__VALUE                 0x0
#define U_PNIP__TOTAL_FRAMES_SENT_P2                                              (0x1060A054 + PNIP_DEV_TOP_BASE)
#define U_PNIP__TOTAL_FRAMES_SENT_P2__TOTAL_FRAMES_SENT                           (0xFFFFFFFF)
#define U_PNIP__TOTAL_FRAMES_SENT_P2_RESET__VALUE                0x0
#define U_PNIP__UNICAST_FRAMES_SENT_P2                                            (0x1060A058 + PNIP_DEV_TOP_BASE)
#define U_PNIP__UNICAST_FRAMES_SENT_P2__UNICAST_FRAMES_SENT                       (0xFFFFFFFF)
#define U_PNIP__UNICAST_FRAMES_SENT_P2_RESET__VALUE              0x0
#define U_PNIP__MULTICAST_FRAMES_SENT_P2                                          (0x1060A05C + PNIP_DEV_TOP_BASE)
#define U_PNIP__MULTICAST_FRAMES_SENT_P2__MULTICAST_FRAMES_SENT                   (0xFFFFFFFF)
#define U_PNIP__MULTICAST_FRAMES_SENT_P2_RESET__VALUE            0x0
#define U_PNIP__BROADCAST_FRAMES_SENT_P2                                          (0x1060A060 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BROADCAST_FRAMES_SENT_P2__BROADCAST_FRAMES_SENT                   (0xFFFFFFFF)
#define U_PNIP__BROADCAST_FRAMES_SENT_P2_RESET__VALUE            0x0
#define U_PNIP__IRT_DELAYTIME_CROSSED_P2                                          (0x1060A064 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRT_DELAYTIME_CROSSED_P2__IRT_DELAYTIME_CROSSED                   (0xFFFFFFFF)
#define U_PNIP__IRT_DELAYTIME_CROSSED_P2_RESET__VALUE            0x0
#define U_PNIP__FRAMES_LESS_64BYTES_P2                                            (0x1060A068 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_LESS_64BYTES_P2__FRAMES_LESS_64BYTES                       (0xFFFFFFFF)
#define U_PNIP__FRAMES_LESS_64BYTES_P2_RESET__VALUE              0x0
#define U_PNIP__FRAMES_WITH_64BYTES_P2                                            (0x1060A06C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_WITH_64BYTES_P2__FRAMES_WITH_64BYTES                       (0xFFFFFFFF)
#define U_PNIP__FRAMES_WITH_64BYTES_P2_RESET__VALUE              0x0
#define U_PNIP__FRAMES_BETWEEN_65TO127BYTES_P2                                    (0x1060A070 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_BETWEEN_65TO127BYTES_P2__FRAMES_BETWEEN_65TO127BYTES       (0xFFFFFFFF)
#define U_PNIP__FRAMES_BETWEEN_65TO127BYTES_P2_RESET__VALUE      0x0
#define U_PNIP__FRAMES_BETWEEN_128TO255BYTES_P2                                   (0x1060A074 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_BETWEEN_128TO255BYTES_P2__FRAMES_BETWEEN_128TO255BYTES     (0xFFFFFFFF)
#define U_PNIP__FRAMES_BETWEEN_128TO255BYTES_P2_RESET__VALUE     0x0
#define U_PNIP__FRAMES_BETWEEN_256TO511BYTES_P2                                   (0x1060A078 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_BETWEEN_256TO511BYTES_P2__FRAMES_BETWEEN_256TO511BYTES     (0xFFFFFFFF)
#define U_PNIP__FRAMES_BETWEEN_256TO511BYTES_P2_RESET__VALUE     0x0
#define U_PNIP__FRAMES_BETWEEN_512TO1023BYTES_P2                                  (0x1060A07C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_BETWEEN_512TO1023BYTES_P2__FRAMES_BETWEEN_512TO1023BYTES   (0xFFFFFFFF)
#define U_PNIP__FRAMES_BETWEEN_512TO1023BYTES_P2_RESET__VALUE    0x0
#define U_PNIP__FRAMES_BETWEEN_1024TO1536BYTES_P2                                 (0x1060A080 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_BETWEEN_1024TO1536BYTES_P2__FRAMES_BETWEEN_1024TO1536BYTES (0xFFFFFFFF)
#define U_PNIP__FRAMES_BETWEEN_1024TO1536BYTES_P2_RESET__VALUE   0x0
#define U_PNIP__OVERSIZE_FRAMES_P2                                                (0x1060A084 + PNIP_DEV_TOP_BASE)
#define U_PNIP__OVERSIZE_FRAMES_P2__OVERSIZE_FRAMES                               (0xFFFFFFFF)
#define U_PNIP__OVERSIZE_FRAMES_P2_RESET__VALUE                  0x0
#define U_PNIP__ERRCODE_RXERR_P2                                                  (0x1060A088 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_RXERR_P2__ERRCODE_RXERR                                   (0xFFFFFFFF)
#define U_PNIP__ERRCODE_RXERR_P2_RESET__VALUE                    0x0
#define U_PNIP__ERRCODE_FCSERR_P2                                                 (0x1060A08C + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_FCSERR_P2__ERRCODE_FCSERR                                 (0xFFFFFFFF)
#define U_PNIP__ERRCODE_FCSERR_P2_RESET__VALUE                   0x0
#define U_PNIP__ERRCODE_LONGERR_P2                                                (0x1060A090 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_LONGERR_P2__ERRCODE_LONGERR                               (0xFFFFFFFF)
#define U_PNIP__ERRCODE_LONGERR_P2_RESET__VALUE                  0x0
#define U_PNIP__ERRCODE_SHORTERR_P2                                               (0x1060A094 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_SHORTERR_P2__ERRCODE_SHORTERR                             (0xFFFFFFFF)
#define U_PNIP__ERRCODE_SHORTERR_P2_RESET__VALUE                 0x0
#define U_PNIP__ERRCODE_DGFCSERR_P2                                               (0x1060A098 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_DGFCSERR_P2__ERRCODE_DGFCSERR                             (0xFFFFFFFF)
#define U_PNIP__ERRCODE_DGFCSERR_P2_RESET__VALUE                 0x0
#define U_PNIP__ERRCODE_PACKSHORTERR_P2                                           (0x1060A09C + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_PACKSHORTERR_P2__ERRCODE_PACKSHORTERR                     (0xFFFFFFFF)
#define U_PNIP__ERRCODE_PACKSHORTERR_P2_RESET__VALUE             0x0
#define U_PNIP__ERRCODE_PPMERR_P2                                                 (0x1060A0A0 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_PPMERR_P2__ERRCODE_PPMERR                                 (0xFFFFFFFF)
#define U_PNIP__ERRCODE_PPMERR_P2_RESET__VALUE                   0x0
#define U_PNIP__ERRCODE_NORESOURCEERR_P2                                          (0x1060A0A4 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_NORESOURCEERR_P2__ERRCODE_NORESOURCEERR                   (0xFFFFFFFF)
#define U_PNIP__ERRCODE_NORESOURCEERR_P2_RESET__VALUE            0x0
#define U_PNIP__ERRCODE_MACBUFFERERR_P2                                           (0x1060A0A8 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_MACBUFFERERR_P2__ERRCODE_MACBUFFERERR                     (0xFFFFFFFF)
#define U_PNIP__ERRCODE_MACBUFFERERR_P2_RESET__VALUE             0x0
#define U_PNIP__ERRCODE_MACSAERR_P2                                               (0x1060A0AC + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_MACSAERR_P2__ERRCODE_MACSAERR                             (0xFFFFFFFF)
#define U_PNIP__ERRCODE_MACSAERR_P2_RESET__VALUE                 0x0
#define U_PNIP__ERRCODE_DISABLEERR_P2                                             (0x1060A0B0 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_DISABLEERR_P2__ERRCODE_DISABLEERR                         (0xFFFFFFFF)
#define U_PNIP__ERRCODE_DISABLEERR_P2_RESET__VALUE               0x0
#define U_PNIP__ERRCODE_PACKPARERR_P2                                             (0x1060A0B4 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_PACKPARERR_P2__ERRCODE_PACKPARERR                         (0xFFFFFFFF)
#define U_PNIP__ERRCODE_PACKPARERR_P2_RESET__VALUE               0x0
#define U_PNIP__ERRCODE_PTCPERR_P2                                                (0x1060A0B8 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_PTCPERR_P2__ERRCODE_PTCPERR                               (0xFFFFFFFF)
#define U_PNIP__ERRCODE_PTCPERR_P2_RESET__VALUE                  0x0
#define U_PNIP__GOOD_BYTES_RECEIVED_P3                                            (0x1060B000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__GOOD_BYTES_RECEIVED_P3__GOOD_BYTES_RECEIVED                       (0xFFFFFFFF)
#define U_PNIP__GOOD_BYTES_RECEIVED_P3_RESET__VALUE              0x0
#define U_PNIP__BAD_BYTES_RECEIVED_P3                                             (0x1060B004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BAD_BYTES_RECEIVED_P3__BAD_BYTES_RECEIVED                         (0xFFFFFFFF)
#define U_PNIP__BAD_BYTES_RECEIVED_P3_RESET__VALUE               0x0
#define U_PNIP__TOTAL_BYTES_RECEIVED_P3                                           (0x1060B008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__TOTAL_BYTES_RECEIVED_P3__TOTAL_BYTES_RECEIVED                     (0xFFFFFFFF)
#define U_PNIP__TOTAL_BYTES_RECEIVED_P3_RESET__VALUE             0x0
#define U_PNIP__GOOD_FRAMES_RECEIVED_P3                                           (0x1060B00C + PNIP_DEV_TOP_BASE)
#define U_PNIP__GOOD_FRAMES_RECEIVED_P3__GOOD_FRAMES_RECEIVED                     (0xFFFFFFFF)
#define U_PNIP__GOOD_FRAMES_RECEIVED_P3_RESET__VALUE             0x0
#define U_PNIP__BAD_FRAMES_RECEIVED_P3                                            (0x1060B010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BAD_FRAMES_RECEIVED_P3__BAD_FRAMES_RECEIVED                       (0xFFFFFFFF)
#define U_PNIP__BAD_FRAMES_RECEIVED_P3_RESET__VALUE              0x0
#define U_PNIP__TOTAL_FRAMES_RECEIVED_P3                                          (0x1060B014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__TOTAL_FRAMES_RECEIVED_P3__TOTAL_FRAMES_RECEIVED                   (0xFFFFFFFF)
#define U_PNIP__TOTAL_FRAMES_RECEIVED_P3_RESET__VALUE            0x0
#define U_PNIP__UC_FRAMES_RECEIVED_P3                                             (0x1060B018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__UC_FRAMES_RECEIVED_P3__UC_FRAMES_RECEIVED                         (0xFFFFFFFF)
#define U_PNIP__UC_FRAMES_RECEIVED_P3_RESET__VALUE               0x0
#define U_PNIP__UC_FRAMES_RECEIVED_UNKNOWN_P3                                     (0x1060B01C + PNIP_DEV_TOP_BASE)
#define U_PNIP__UC_FRAMES_RECEIVED_UNKNOWN_P3__UC_FRAMES_RECEIVED_UNKNOWN         (0xFFFFFFFF)
#define U_PNIP__UC_FRAMES_RECEIVED_UNKNOWN_P3_RESET__VALUE       0x0
#define U_PNIP__MC_FRAMES_RECEIVED_P3                                             (0x1060B020 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MC_FRAMES_RECEIVED_P3__MC_FRAMES_RECEIVED                         (0xFFFFFFFF)
#define U_PNIP__MC_FRAMES_RECEIVED_P3_RESET__VALUE               0x0
#define U_PNIP__MC_FRAMES_RECEIVED_UNKNOWN_P3                                     (0x1060B024 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MC_FRAMES_RECEIVED_UNKNOWN_P3__MC_FRAMES_RECEIVED_UNKNOWN         (0xFFFFFFFF)
#define U_PNIP__MC_FRAMES_RECEIVED_UNKNOWN_P3_RESET__VALUE       0x0
#define U_PNIP__BROADCAST_FRAMES_RECEIVED_P3                                      (0x1060B028 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BROADCAST_FRAMES_RECEIVED_P3__BROADCAST_FRAMES_RECEIVED           (0xFFFFFFFF)
#define U_PNIP__BROADCAST_FRAMES_RECEIVED_P3_RESET__VALUE        0x0
#define U_PNIP__CRC_ALIGNMENT_ERR_P3                                              (0x1060B02C + PNIP_DEV_TOP_BASE)
#define U_PNIP__CRC_ALIGNMENT_ERR_P3__CRC_ALIGNMENT_ERR                           (0xFFFFFFFF)
#define U_PNIP__CRC_ALIGNMENT_ERR_P3_RESET__VALUE                0x0
#define U_PNIP__FRAMES_DROPPED_L1_P3                                              (0x1060B030 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_DROPPED_L1_P3__FRAMES_DROPPED_L1                           (0xFFFFFFFF)
#define U_PNIP__FRAMES_DROPPED_L1_P3_RESET__VALUE                0x0
#define U_PNIP__FRAMES_DROPPED_L2_P3                                              (0x1060B034 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_DROPPED_L2_P3__FRAMES_DROPPED_L2                           (0xFFFFFFFF)
#define U_PNIP__FRAMES_DROPPED_L2_P3_RESET__VALUE                0x0
#define U_PNIP__FRAMES_DROPPED_L3_P3                                              (0x1060B038 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_DROPPED_L3_P3__FRAMES_DROPPED_L3                           (0xFFFFFFFF)
#define U_PNIP__FRAMES_DROPPED_L3_P3_RESET__VALUE                0x0
#define U_PNIP__HOL_CROSSED_P3                                                    (0x1060B03C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_CROSSED_P3__HOL_CROSSED                                       (0xFFFFFFFF)
#define U_PNIP__HOL_CROSSED_P3_RESET__VALUE                      0x0
#define U_PNIP__QCW_CROSSED_P3                                                    (0x1060B040 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_CROSSED_P3__QCW_CROSSED                                       (0xFFFFFFFF)
#define U_PNIP__QCW_CROSSED_P3_RESET__VALUE                      0x0
#define U_PNIP__UC_DEFAULT_FORWARDING_P3                                          (0x1060B044 + PNIP_DEV_TOP_BASE)
#define U_PNIP__UC_DEFAULT_FORWARDING_P3__UC_DEFAULT_FW                           (0xFFFFFFFF)
#define U_PNIP__UC_DEFAULT_FORWARDING_P3_RESET__VALUE            0x0
#define U_PNIP__MC_DEFAULT_FORWARDING_P3                                          (0x1060B048 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MC_DEFAULT_FORWARDING_P3__MC_DEFAULT_FW                           (0xFFFFFFFF)
#define U_PNIP__MC_DEFAULT_FORWARDING_P3_RESET__VALUE            0x0
#define U_PNIP__FRAMEAGING_CROSSED_P3                                             (0x1060B04C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMEAGING_CROSSED_P3__FRAMEAGING_PX                              (0xFFFFFFFF)
#define U_PNIP__FRAMEAGING_CROSSED_P3_RESET__VALUE               0x0
#define U_PNIP__TOTAL_BYTES_SENT_P3                                               (0x1060B050 + PNIP_DEV_TOP_BASE)
#define U_PNIP__TOTAL_BYTES_SENT_P3__TOTAL_BYTES_SENT                             (0xFFFFFFFF)
#define U_PNIP__TOTAL_BYTES_SENT_P3_RESET__VALUE                 0x0
#define U_PNIP__TOTAL_FRAMES_SENT_P3                                              (0x1060B054 + PNIP_DEV_TOP_BASE)
#define U_PNIP__TOTAL_FRAMES_SENT_P3__TOTAL_FRAMES_SENT                           (0xFFFFFFFF)
#define U_PNIP__TOTAL_FRAMES_SENT_P3_RESET__VALUE                0x0
#define U_PNIP__UNICAST_FRAMES_SENT_P3                                            (0x1060B058 + PNIP_DEV_TOP_BASE)
#define U_PNIP__UNICAST_FRAMES_SENT_P3__UNICAST_FRAMES_SENT                       (0xFFFFFFFF)
#define U_PNIP__UNICAST_FRAMES_SENT_P3_RESET__VALUE              0x0
#define U_PNIP__MULTICAST_FRAMES_SENT_P3                                          (0x1060B05C + PNIP_DEV_TOP_BASE)
#define U_PNIP__MULTICAST_FRAMES_SENT_P3__MULTICAST_FRAMES_SENT                   (0xFFFFFFFF)
#define U_PNIP__MULTICAST_FRAMES_SENT_P3_RESET__VALUE            0x0
#define U_PNIP__BROADCAST_FRAMES_SENT_P3                                          (0x1060B060 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BROADCAST_FRAMES_SENT_P3__BROADCAST_FRAMES_SENT                   (0xFFFFFFFF)
#define U_PNIP__BROADCAST_FRAMES_SENT_P3_RESET__VALUE            0x0
#define U_PNIP__IRT_DELAYTIME_CROSSED_P3                                          (0x1060B064 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRT_DELAYTIME_CROSSED_P3__IRT_DELAYTIME_CROSSED                   (0xFFFFFFFF)
#define U_PNIP__IRT_DELAYTIME_CROSSED_P3_RESET__VALUE            0x0
#define U_PNIP__FRAMES_LESS_64BYTES_P3                                            (0x1060B068 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_LESS_64BYTES_P3__FRAMES_LESS_64BYTES                       (0xFFFFFFFF)
#define U_PNIP__FRAMES_LESS_64BYTES_P3_RESET__VALUE              0x0
#define U_PNIP__FRAMES_WITH_64BYTES_P3                                            (0x1060B06C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_WITH_64BYTES_P3__FRAMES_WITH_64BYTES                       (0xFFFFFFFF)
#define U_PNIP__FRAMES_WITH_64BYTES_P3_RESET__VALUE              0x0
#define U_PNIP__FRAMES_BETWEEN_65TO127BYTES_P3                                    (0x1060B070 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_BETWEEN_65TO127BYTES_P3__FRAMES_BETWEEN_65TO127BYTES       (0xFFFFFFFF)
#define U_PNIP__FRAMES_BETWEEN_65TO127BYTES_P3_RESET__VALUE      0x0
#define U_PNIP__FRAMES_BETWEEN_128TO255BYTES_P3                                   (0x1060B074 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_BETWEEN_128TO255BYTES_P3__FRAMES_BETWEEN_128TO255BYTES     (0xFFFFFFFF)
#define U_PNIP__FRAMES_BETWEEN_128TO255BYTES_P3_RESET__VALUE     0x0
#define U_PNIP__FRAMES_BETWEEN_256TO511BYTES_P3                                   (0x1060B078 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_BETWEEN_256TO511BYTES_P3__FRAMES_BETWEEN_256TO511BYTES     (0xFFFFFFFF)
#define U_PNIP__FRAMES_BETWEEN_256TO511BYTES_P3_RESET__VALUE     0x0
#define U_PNIP__FRAMES_BETWEEN_512TO1023BYTES_P3                                  (0x1060B07C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_BETWEEN_512TO1023BYTES_P3__FRAMES_BETWEEN_512TO1023BYTES   (0xFFFFFFFF)
#define U_PNIP__FRAMES_BETWEEN_512TO1023BYTES_P3_RESET__VALUE    0x0
#define U_PNIP__FRAMES_BETWEEN_1024TO1536BYTES_P3                                 (0x1060B080 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_BETWEEN_1024TO1536BYTES_P3__FRAMES_BETWEEN_1024TO1536BYTES (0xFFFFFFFF)
#define U_PNIP__FRAMES_BETWEEN_1024TO1536BYTES_P3_RESET__VALUE   0x0
#define U_PNIP__OVERSIZE_FRAMES_P3                                                (0x1060B084 + PNIP_DEV_TOP_BASE)
#define U_PNIP__OVERSIZE_FRAMES_P3__OVERSIZE_FRAMES                               (0xFFFFFFFF)
#define U_PNIP__OVERSIZE_FRAMES_P3_RESET__VALUE                  0x0
#define U_PNIP__ERRCODE_RXERR_P3                                                  (0x1060B088 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_RXERR_P3__ERRCODE_RXERR                                   (0xFFFFFFFF)
#define U_PNIP__ERRCODE_RXERR_P3_RESET__VALUE                    0x0
#define U_PNIP__ERRCODE_FCSERR_P3                                                 (0x1060B08C + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_FCSERR_P3__ERRCODE_FCSERR                                 (0xFFFFFFFF)
#define U_PNIP__ERRCODE_FCSERR_P3_RESET__VALUE                   0x0
#define U_PNIP__ERRCODE_LONGERR_P3                                                (0x1060B090 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_LONGERR_P3__ERRCODE_LONGERR                               (0xFFFFFFFF)
#define U_PNIP__ERRCODE_LONGERR_P3_RESET__VALUE                  0x0
#define U_PNIP__ERRCODE_SHORTERR_P3                                               (0x1060B094 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_SHORTERR_P3__ERRCODE_SHORTERR                             (0xFFFFFFFF)
#define U_PNIP__ERRCODE_SHORTERR_P3_RESET__VALUE                 0x0
#define U_PNIP__ERRCODE_DGFCSERR_P3                                               (0x1060B098 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_DGFCSERR_P3__ERRCODE_DGFCSERR                             (0xFFFFFFFF)
#define U_PNIP__ERRCODE_DGFCSERR_P3_RESET__VALUE                 0x0
#define U_PNIP__ERRCODE_PACKSHORTERR_P3                                           (0x1060B09C + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_PACKSHORTERR_P3__ERRCODE_PACKSHORTERR                     (0xFFFFFFFF)
#define U_PNIP__ERRCODE_PACKSHORTERR_P3_RESET__VALUE             0x0
#define U_PNIP__ERRCODE_PPMERR_P3                                                 (0x1060B0A0 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_PPMERR_P3__ERRCODE_PPMERR                                 (0xFFFFFFFF)
#define U_PNIP__ERRCODE_PPMERR_P3_RESET__VALUE                   0x0
#define U_PNIP__ERRCODE_NORESOURCEERR_P3                                          (0x1060B0A4 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_NORESOURCEERR_P3__ERRCODE_NORESOURCEERR                   (0xFFFFFFFF)
#define U_PNIP__ERRCODE_NORESOURCEERR_P3_RESET__VALUE            0x0
#define U_PNIP__ERRCODE_MACBUFFERERR_P3                                           (0x1060B0A8 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_MACBUFFERERR_P3__ERRCODE_MACBUFFERERR                     (0xFFFFFFFF)
#define U_PNIP__ERRCODE_MACBUFFERERR_P3_RESET__VALUE             0x0
#define U_PNIP__ERRCODE_MACSAERR_P3                                               (0x1060B0AC + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_MACSAERR_P3__ERRCODE_MACSAERR                             (0xFFFFFFFF)
#define U_PNIP__ERRCODE_MACSAERR_P3_RESET__VALUE                 0x0
#define U_PNIP__ERRCODE_DISABLEERR_P3                                             (0x1060B0B0 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_DISABLEERR_P3__ERRCODE_DISABLEERR                         (0xFFFFFFFF)
#define U_PNIP__ERRCODE_DISABLEERR_P3_RESET__VALUE               0x0
#define U_PNIP__ERRCODE_PACKPARERR_P3                                             (0x1060B0B4 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_PACKPARERR_P3__ERRCODE_PACKPARERR                         (0xFFFFFFFF)
#define U_PNIP__ERRCODE_PACKPARERR_P3_RESET__VALUE               0x0
#define U_PNIP__ERRCODE_PTCPERR_P3                                                (0x1060B0B8 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_PTCPERR_P3__ERRCODE_PTCPERR                               (0xFFFFFFFF)
#define U_PNIP__ERRCODE_PTCPERR_P3_RESET__VALUE                  0x0
#define U_PNIP__GOOD_BYTES_RECEIVED_P4                                            (0x1060C000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__GOOD_BYTES_RECEIVED_P4__GOOD_BYTES_RECEIVED                       (0xFFFFFFFF)
#define U_PNIP__GOOD_BYTES_RECEIVED_P4_RESET__VALUE              0x0
#define U_PNIP__BAD_BYTES_RECEIVED_P4                                             (0x1060C004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BAD_BYTES_RECEIVED_P4__BAD_BYTES_RECEIVED                         (0xFFFFFFFF)
#define U_PNIP__BAD_BYTES_RECEIVED_P4_RESET__VALUE               0x0
#define U_PNIP__TOTAL_BYTES_RECEIVED_P4                                           (0x1060C008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__TOTAL_BYTES_RECEIVED_P4__TOTAL_BYTES_RECEIVED                     (0xFFFFFFFF)
#define U_PNIP__TOTAL_BYTES_RECEIVED_P4_RESET__VALUE             0x0
#define U_PNIP__GOOD_FRAMES_RECEIVED_P4                                           (0x1060C00C + PNIP_DEV_TOP_BASE)
#define U_PNIP__GOOD_FRAMES_RECEIVED_P4__GOOD_FRAMES_RECEIVED                     (0xFFFFFFFF)
#define U_PNIP__GOOD_FRAMES_RECEIVED_P4_RESET__VALUE             0x0
#define U_PNIP__BAD_FRAMES_RECEIVED_P4                                            (0x1060C010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BAD_FRAMES_RECEIVED_P4__BAD_FRAMES_RECEIVED                       (0xFFFFFFFF)
#define U_PNIP__BAD_FRAMES_RECEIVED_P4_RESET__VALUE              0x0
#define U_PNIP__TOTAL_FRAMES_RECEIVED_P4                                          (0x1060C014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__TOTAL_FRAMES_RECEIVED_P4__TOTAL_FRAMES_RECEIVED                   (0xFFFFFFFF)
#define U_PNIP__TOTAL_FRAMES_RECEIVED_P4_RESET__VALUE            0x0
#define U_PNIP__UC_FRAMES_RECEIVED_P4                                             (0x1060C018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__UC_FRAMES_RECEIVED_P4__UC_FRAMES_RECEIVED                         (0xFFFFFFFF)
#define U_PNIP__UC_FRAMES_RECEIVED_P4_RESET__VALUE               0x0
#define U_PNIP__UC_FRAMES_RECEIVED_UNKNOWN_P4                                     (0x1060C01C + PNIP_DEV_TOP_BASE)
#define U_PNIP__UC_FRAMES_RECEIVED_UNKNOWN_P4__UC_FRAMES_RECEIVED_UNKNOWN         (0xFFFFFFFF)
#define U_PNIP__UC_FRAMES_RECEIVED_UNKNOWN_P4_RESET__VALUE       0x0
#define U_PNIP__MC_FRAMES_RECEIVED_P4                                             (0x1060C020 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MC_FRAMES_RECEIVED_P4__MC_FRAMES_RECEIVED                         (0xFFFFFFFF)
#define U_PNIP__MC_FRAMES_RECEIVED_P4_RESET__VALUE               0x0
#define U_PNIP__MC_FRAMES_RECEIVED_UNKNOWN_P4                                     (0x1060C024 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MC_FRAMES_RECEIVED_UNKNOWN_P4__MC_FRAMES_RECEIVED_UNKNOWN         (0xFFFFFFFF)
#define U_PNIP__MC_FRAMES_RECEIVED_UNKNOWN_P4_RESET__VALUE       0x0
#define U_PNIP__BROADCAST_FRAMES_RECEIVED_P4                                      (0x1060C028 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BROADCAST_FRAMES_RECEIVED_P4__BROADCAST_FRAMES_RECEIVED           (0xFFFFFFFF)
#define U_PNIP__BROADCAST_FRAMES_RECEIVED_P4_RESET__VALUE        0x0
#define U_PNIP__CRC_ALIGNMENT_ERR_P4                                              (0x1060C02C + PNIP_DEV_TOP_BASE)
#define U_PNIP__CRC_ALIGNMENT_ERR_P4__CRC_ALIGNMENT_ERR                           (0xFFFFFFFF)
#define U_PNIP__CRC_ALIGNMENT_ERR_P4_RESET__VALUE                0x0
#define U_PNIP__FRAMES_DROPPED_L1_P4                                              (0x1060C030 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_DROPPED_L1_P4__FRAMES_DROPPED_L1                           (0xFFFFFFFF)
#define U_PNIP__FRAMES_DROPPED_L1_P4_RESET__VALUE                0x0
#define U_PNIP__FRAMES_DROPPED_L2_P4                                              (0x1060C034 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_DROPPED_L2_P4__FRAMES_DROPPED_L2                           (0xFFFFFFFF)
#define U_PNIP__FRAMES_DROPPED_L2_P4_RESET__VALUE                0x0
#define U_PNIP__FRAMES_DROPPED_L3_P4                                              (0x1060C038 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_DROPPED_L3_P4__FRAMES_DROPPED_L3                           (0xFFFFFFFF)
#define U_PNIP__FRAMES_DROPPED_L3_P4_RESET__VALUE                0x0
#define U_PNIP__HOL_CROSSED_P4                                                    (0x1060C03C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_CROSSED_P4__HOL_CROSSED                                       (0xFFFFFFFF)
#define U_PNIP__HOL_CROSSED_P4_RESET__VALUE                      0x0
#define U_PNIP__QCW_CROSSED_P4                                                    (0x1060C040 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_CROSSED_P4__QCW_CROSSED                                       (0xFFFFFFFF)
#define U_PNIP__QCW_CROSSED_P4_RESET__VALUE                      0x0
#define U_PNIP__UC_DEFAULT_FORWARDING_P4                                          (0x1060C044 + PNIP_DEV_TOP_BASE)
#define U_PNIP__UC_DEFAULT_FORWARDING_P4__UC_DEFAULT_FW                           (0xFFFFFFFF)
#define U_PNIP__UC_DEFAULT_FORWARDING_P4_RESET__VALUE            0x0
#define U_PNIP__MC_DEFAULT_FORWARDING_P4                                          (0x1060C048 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MC_DEFAULT_FORWARDING_P4__MC_DEFAULT_FW                           (0xFFFFFFFF)
#define U_PNIP__MC_DEFAULT_FORWARDING_P4_RESET__VALUE            0x0
#define U_PNIP__FRAMEAGING_CROSSED_P4                                             (0x1060C04C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMEAGING_CROSSED_P4__FRAMEAGING_PX                              (0xFFFFFFFF)
#define U_PNIP__FRAMEAGING_CROSSED_P4_RESET__VALUE               0x0
#define U_PNIP__TOTAL_BYTES_SENT_P4                                               (0x1060C050 + PNIP_DEV_TOP_BASE)
#define U_PNIP__TOTAL_BYTES_SENT_P4__TOTAL_BYTES_SENT                             (0xFFFFFFFF)
#define U_PNIP__TOTAL_BYTES_SENT_P4_RESET__VALUE                 0x0
#define U_PNIP__TOTAL_FRAMES_SENT_P4                                              (0x1060C054 + PNIP_DEV_TOP_BASE)
#define U_PNIP__TOTAL_FRAMES_SENT_P4__TOTAL_FRAMES_SENT                           (0xFFFFFFFF)
#define U_PNIP__TOTAL_FRAMES_SENT_P4_RESET__VALUE                0x0
#define U_PNIP__UNICAST_FRAMES_SENT_P4                                            (0x1060C058 + PNIP_DEV_TOP_BASE)
#define U_PNIP__UNICAST_FRAMES_SENT_P4__UNICAST_FRAMES_SENT                       (0xFFFFFFFF)
#define U_PNIP__UNICAST_FRAMES_SENT_P4_RESET__VALUE              0x0
#define U_PNIP__MULTICAST_FRAMES_SENT_P4                                          (0x1060C05C + PNIP_DEV_TOP_BASE)
#define U_PNIP__MULTICAST_FRAMES_SENT_P4__MULTICAST_FRAMES_SENT                   (0xFFFFFFFF)
#define U_PNIP__MULTICAST_FRAMES_SENT_P4_RESET__VALUE            0x0
#define U_PNIP__BROADCAST_FRAMES_SENT_P4                                          (0x1060C060 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BROADCAST_FRAMES_SENT_P4__BROADCAST_FRAMES_SENT                   (0xFFFFFFFF)
#define U_PNIP__BROADCAST_FRAMES_SENT_P4_RESET__VALUE            0x0
#define U_PNIP__IRT_DELAYTIME_CROSSED_P4                                          (0x1060C064 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRT_DELAYTIME_CROSSED_P4__IRT_DELAYTIME_CROSSED                   (0xFFFFFFFF)
#define U_PNIP__IRT_DELAYTIME_CROSSED_P4_RESET__VALUE            0x0
#define U_PNIP__FRAMES_LESS_64BYTES_P4                                            (0x1060C068 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_LESS_64BYTES_P4__FRAMES_LESS_64BYTES                       (0xFFFFFFFF)
#define U_PNIP__FRAMES_LESS_64BYTES_P4_RESET__VALUE              0x0
#define U_PNIP__FRAMES_WITH_64BYTES_P4                                            (0x1060C06C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_WITH_64BYTES_P4__FRAMES_WITH_64BYTES                       (0xFFFFFFFF)
#define U_PNIP__FRAMES_WITH_64BYTES_P4_RESET__VALUE              0x0
#define U_PNIP__FRAMES_BETWEEN_65TO127BYTES_P4                                    (0x1060C070 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_BETWEEN_65TO127BYTES_P4__FRAMES_BETWEEN_65TO127BYTES       (0xFFFFFFFF)
#define U_PNIP__FRAMES_BETWEEN_65TO127BYTES_P4_RESET__VALUE      0x0
#define U_PNIP__FRAMES_BETWEEN_128TO255BYTES_P4                                   (0x1060C074 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_BETWEEN_128TO255BYTES_P4__FRAMES_BETWEEN_128TO255BYTES     (0xFFFFFFFF)
#define U_PNIP__FRAMES_BETWEEN_128TO255BYTES_P4_RESET__VALUE     0x0
#define U_PNIP__FRAMES_BETWEEN_256TO511BYTES_P4                                   (0x1060C078 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_BETWEEN_256TO511BYTES_P4__FRAMES_BETWEEN_256TO511BYTES     (0xFFFFFFFF)
#define U_PNIP__FRAMES_BETWEEN_256TO511BYTES_P4_RESET__VALUE     0x0
#define U_PNIP__FRAMES_BETWEEN_512TO1023BYTES_P4                                  (0x1060C07C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_BETWEEN_512TO1023BYTES_P4__FRAMES_BETWEEN_512TO1023BYTES   (0xFFFFFFFF)
#define U_PNIP__FRAMES_BETWEEN_512TO1023BYTES_P4_RESET__VALUE    0x0
#define U_PNIP__FRAMES_BETWEEN_1024TO1536BYTES_P4                                 (0x1060C080 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMES_BETWEEN_1024TO1536BYTES_P4__FRAMES_BETWEEN_1024TO1536BYTES (0xFFFFFFFF)
#define U_PNIP__FRAMES_BETWEEN_1024TO1536BYTES_P4_RESET__VALUE   0x0
#define U_PNIP__OVERSIZE_FRAMES_P4                                                (0x1060C084 + PNIP_DEV_TOP_BASE)
#define U_PNIP__OVERSIZE_FRAMES_P4__OVERSIZE_FRAMES                               (0xFFFFFFFF)
#define U_PNIP__OVERSIZE_FRAMES_P4_RESET__VALUE                  0x0
#define U_PNIP__ERRCODE_RXERR_P4                                                  (0x1060C088 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_RXERR_P4__ERRCODE_RXERR                                   (0xFFFFFFFF)
#define U_PNIP__ERRCODE_RXERR_P4_RESET__VALUE                    0x0
#define U_PNIP__ERRCODE_FCSERR_P4                                                 (0x1060C08C + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_FCSERR_P4__ERRCODE_FCSERR                                 (0xFFFFFFFF)
#define U_PNIP__ERRCODE_FCSERR_P4_RESET__VALUE                   0x0
#define U_PNIP__ERRCODE_LONGERR_P4                                                (0x1060C090 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_LONGERR_P4__ERRCODE_LONGERR                               (0xFFFFFFFF)
#define U_PNIP__ERRCODE_LONGERR_P4_RESET__VALUE                  0x0
#define U_PNIP__ERRCODE_SHORTERR_P4                                               (0x1060C094 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_SHORTERR_P4__ERRCODE_SHORTERR                             (0xFFFFFFFF)
#define U_PNIP__ERRCODE_SHORTERR_P4_RESET__VALUE                 0x0
#define U_PNIP__ERRCODE_DGFCSERR_P4                                               (0x1060C098 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_DGFCSERR_P4__ERRCODE_DGFCSERR                             (0xFFFFFFFF)
#define U_PNIP__ERRCODE_DGFCSERR_P4_RESET__VALUE                 0x0
#define U_PNIP__ERRCODE_PACKSHORTERR_P4                                           (0x1060C09C + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_PACKSHORTERR_P4__ERRCODE_PACKSHORTERR                     (0xFFFFFFFF)
#define U_PNIP__ERRCODE_PACKSHORTERR_P4_RESET__VALUE             0x0
#define U_PNIP__ERRCODE_PPMERR_P4                                                 (0x1060C0A0 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_PPMERR_P4__ERRCODE_PPMERR                                 (0xFFFFFFFF)
#define U_PNIP__ERRCODE_PPMERR_P4_RESET__VALUE                   0x0
#define U_PNIP__ERRCODE_NORESOURCEERR_P4                                          (0x1060C0A4 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_NORESOURCEERR_P4__ERRCODE_NORESOURCEERR                   (0xFFFFFFFF)
#define U_PNIP__ERRCODE_NORESOURCEERR_P4_RESET__VALUE            0x0
#define U_PNIP__ERRCODE_MACBUFFERERR_P4                                           (0x1060C0A8 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_MACBUFFERERR_P4__ERRCODE_MACBUFFERERR                     (0xFFFFFFFF)
#define U_PNIP__ERRCODE_MACBUFFERERR_P4_RESET__VALUE             0x0
#define U_PNIP__ERRCODE_MACSAERR_P4                                               (0x1060C0AC + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_MACSAERR_P4__ERRCODE_MACSAERR                             (0xFFFFFFFF)
#define U_PNIP__ERRCODE_MACSAERR_P4_RESET__VALUE                 0x0
#define U_PNIP__ERRCODE_DISABLEERR_P4                                             (0x1060C0B0 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_DISABLEERR_P4__ERRCODE_DISABLEERR                         (0xFFFFFFFF)
#define U_PNIP__ERRCODE_DISABLEERR_P4_RESET__VALUE               0x0
#define U_PNIP__ERRCODE_PACKPARERR_P4                                             (0x1060C0B4 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_PACKPARERR_P4__ERRCODE_PACKPARERR                         (0xFFFFFFFF)
#define U_PNIP__ERRCODE_PACKPARERR_P4_RESET__VALUE               0x0
#define U_PNIP__ERRCODE_PTCPERR_P4                                                (0x1060C0B8 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERRCODE_PTCPERR_P4__ERRCODE_PTCPERR                               (0xFFFFFFFF)
#define U_PNIP__ERRCODE_PTCPERR_P4_RESET__VALUE                  0x0
#define U_PNIP__STATISTICCONTROL                                                  (0x1060D000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__STATISTICCONTROL__RESET_P4                                        (0x00000008)
#define U_PNIP__STATISTICCONTROL__RESET_P3                                        (0x00000004)
#define U_PNIP__STATISTICCONTROL__RESET_P2                                        (0x00000002)
#define U_PNIP__STATISTICCONTROL__RESET_P1                                        (0x00000001)
#define U_PNIP__STATISTICCONTROL_RESET__VALUE                    0x0
#define U_PNIP__FRAMECLASS_0_1                                                    (0x10610000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_0_1__VALID                                             (0x80000000)
#define U_PNIP__FRAMECLASS_0_1__COMPTYPE                                          (0x40000000)
#define U_PNIP__FRAMECLASS_0_1__ETHERTYPE                                         (0x0000FFFF)
#define U_PNIP__FRAMECLASS_0_1_RESET__VALUE                      0x80008892
#define U_PNIP__FRAMECLASS_0_2                                                    (0x10610004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_0_2__VALID                                             (0x80000000)
#define U_PNIP__FRAMECLASS_0_2__COMPTYPE                                          (0x40000000)
#define U_PNIP__FRAMECLASS_0_2__ETHERTYPE                                         (0x0000FFFF)
#define U_PNIP__FRAMECLASS_0_2_RESET__VALUE                      0x80008892
#define U_PNIP__FRAMECLASS_0_3                                                    (0x10610008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_0_3__VALID                                             (0x80000000)
#define U_PNIP__FRAMECLASS_0_3__COMPTYPE                                          (0x40000000)
#define U_PNIP__FRAMECLASS_0_3__ETHERTYPE                                         (0x0000FFFF)
#define U_PNIP__FRAMECLASS_0_3_RESET__VALUE                      0x80008892
#define U_PNIP__FRAMECLASS_0_4                                                    (0x1061000C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_0_4__VALID                                             (0x80000000)
#define U_PNIP__FRAMECLASS_0_4__COMPTYPE                                          (0x40000000)
#define U_PNIP__FRAMECLASS_0_4__ETHERTYPE                                         (0x0000FFFF)
#define U_PNIP__FRAMECLASS_0_4_RESET__VALUE                      0x80008892
#define U_PNIP__FRAMECLASS_0_5                                                    (0x10610010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_0_5__VALID                                             (0x80000000)
#define U_PNIP__FRAMECLASS_0_5__COMPTYPE                                          (0x40000000)
#define U_PNIP__FRAMECLASS_0_5__ETHERTYPE                                         (0x0000FFFF)
#define U_PNIP__FRAMECLASS_0_5_RESET__VALUE                      0x80008892
#define U_PNIP__FRAMECLASS_0_6                                                    (0x10610014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_0_6__VALID                                             (0x80000000)
#define U_PNIP__FRAMECLASS_0_6__COMPTYPE                                          (0x40000000)
#define U_PNIP__FRAMECLASS_0_6__ETHERTYPE                                         (0x0000FFFF)
#define U_PNIP__FRAMECLASS_0_6_RESET__VALUE                      0x80008892
#define U_PNIP__FRAMECLASS_0_7                                                    (0x10610018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_0_7__VALID                                             (0x80000000)
#define U_PNIP__FRAMECLASS_0_7__COMPTYPE                                          (0x40000000)
#define U_PNIP__FRAMECLASS_0_7__ETHERTYPE                                         (0x0000FFFF)
#define U_PNIP__FRAMECLASS_0_7_RESET__VALUE                      0x80008892
#define U_PNIP__FRAMECLASS_0_8                                                    (0x1061001C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_0_8__VALID                                             (0x80000000)
#define U_PNIP__FRAMECLASS_0_8__COMPTYPE                                          (0x40000000)
#define U_PNIP__FRAMECLASS_0_8__ETHERTYPE                                         (0x0000FFFF)
#define U_PNIP__FRAMECLASS_0_8_RESET__VALUE                      0x80008892
#define U_PNIP__FRAMECLASS_0_9                                                    (0x10610020 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_0_9__VALID                                             (0x80000000)
#define U_PNIP__FRAMECLASS_0_9__COMPTYPE                                          (0x40000000)
#define U_PNIP__FRAMECLASS_0_9__ETHERTYPE                                         (0x0000FFFF)
#define U_PNIP__FRAMECLASS_0_9_RESET__VALUE                      0x80008892
#define U_PNIP__FRAMECLASS_0_10                                                   (0x10610024 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_0_10__VALID                                            (0x80000000)
#define U_PNIP__FRAMECLASS_0_10__COMPTYPE                                         (0x40000000)
#define U_PNIP__FRAMECLASS_0_10__ETHERTYPE                                        (0x0000FFFF)
#define U_PNIP__FRAMECLASS_0_10_RESET__VALUE                     0x80008892
#define U_PNIP__FRAMECLASS_0_11                                                   (0x10610028 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_0_11__VALID                                            (0x80000000)
#define U_PNIP__FRAMECLASS_0_11__COMPTYPE                                         (0x40000000)
#define U_PNIP__FRAMECLASS_0_11__ETHERTYPE                                        (0x0000FFFF)
#define U_PNIP__FRAMECLASS_0_11_RESET__VALUE                     0x800088F7
#define U_PNIP__FRAMECLASS_0_12                                                   (0x1061002C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_0_12__VALID                                            (0x80000000)
#define U_PNIP__FRAMECLASS_0_12__COMPTYPE                                         (0x40000000)
#define U_PNIP__FRAMECLASS_0_12__ETHERTYPE                                        (0x0000FFFF)
#define U_PNIP__FRAMECLASS_0_12_RESET__VALUE                     0x80000806
#define U_PNIP__FRAMECLASS_0_13                                                   (0x10610030 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_0_13__VALID                                            (0x80000000)
#define U_PNIP__FRAMECLASS_0_13__COMPTYPE                                         (0x40000000)
#define U_PNIP__FRAMECLASS_0_13__ETHERTYPE                                        (0x0000FFFF)
#define U_PNIP__FRAMECLASS_0_13_RESET__VALUE                     0x800088CC
#define U_PNIP__FRAMECLASS_0_14                                                   (0x10610034 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_0_14__VALID                                            (0x80000000)
#define U_PNIP__FRAMECLASS_0_14__COMPTYPE                                         (0x40000000)
#define U_PNIP__FRAMECLASS_0_14__ETHERTYPE                                        (0x0000FFFF)
#define U_PNIP__FRAMECLASS_0_14_RESET__VALUE                     0x80000800
#define U_PNIP__FRAMECLASS_0_15                                                   (0x10610038 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_0_15__VALID                                            (0x80000000)
#define U_PNIP__FRAMECLASS_0_15__COMPTYPE                                         (0x40000000)
#define U_PNIP__FRAMECLASS_0_15__ETHERTYPE                                        (0x0000FFFF)
#define U_PNIP__FRAMECLASS_0_15_RESET__VALUE                     0x800088E3
#define U_PNIP__FRAMECLASS_0_16                                                   (0x1061003C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_0_16__VALID                                            (0x80000000)
#define U_PNIP__FRAMECLASS_0_16__COMPTYPE                                         (0x40000000)
#define U_PNIP__FRAMECLASS_0_16__ETHERTYPE                                        (0x0000FFFF)
#define U_PNIP__FRAMECLASS_0_16_RESET__VALUE                     0xC0000600
#define U_PNIP__FRAMECLASS_0_17                                                   (0x10610040 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_0_17__VALID                                            (0x80000000)
#define U_PNIP__FRAMECLASS_0_17__COMPTYPE                                         (0x40000000)
#define U_PNIP__FRAMECLASS_0_17__ETHERTYPE                                        (0x0000FFFF)
#define U_PNIP__FRAMECLASS_0_17_RESET__VALUE                     0x0
#define U_PNIP__FRAMECLASS_0_18                                                   (0x10610044 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_0_18__VALID                                            (0x80000000)
#define U_PNIP__FRAMECLASS_0_18__COMPTYPE                                         (0x40000000)
#define U_PNIP__FRAMECLASS_0_18__ETHERTYPE                                        (0x0000FFFF)
#define U_PNIP__FRAMECLASS_0_18_RESET__VALUE                     0x0
#define U_PNIP__FRAMECLASS_0_19                                                   (0x10610048 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_0_19__VALID                                            (0x80000000)
#define U_PNIP__FRAMECLASS_0_19__COMPTYPE                                         (0x40000000)
#define U_PNIP__FRAMECLASS_0_19__ETHERTYPE                                        (0x0000FFFF)
#define U_PNIP__FRAMECLASS_0_19_RESET__VALUE                     0x0
#define U_PNIP__FRAMECLASS_0_20                                                   (0x1061004C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_0_20__VALID                                            (0x80000000)
#define U_PNIP__FRAMECLASS_0_20__COMPTYPE                                         (0x40000000)
#define U_PNIP__FRAMECLASS_0_20__ETHERTYPE                                        (0x0000FFFF)
#define U_PNIP__FRAMECLASS_0_20_RESET__VALUE                     0x0
#define U_PNIP__FRAMECLASS_0_21                                                   (0x10610050 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_0_21__VALID                                            (0x80000000)
#define U_PNIP__FRAMECLASS_0_21__COMPTYPE                                         (0x40000000)
#define U_PNIP__FRAMECLASS_0_21__ETHERTYPE                                        (0x0000FFFF)
#define U_PNIP__FRAMECLASS_0_21_RESET__VALUE                     0x0
#define U_PNIP__FRAMECLASS_0_22                                                   (0x10610054 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_0_22__VALID                                            (0x80000000)
#define U_PNIP__FRAMECLASS_0_22__COMPTYPE                                         (0x40000000)
#define U_PNIP__FRAMECLASS_0_22__ETHERTYPE                                        (0x0000FFFF)
#define U_PNIP__FRAMECLASS_0_22_RESET__VALUE                     0x0
#define U_PNIP__FRAMECLASS_0_23                                                   (0x10610058 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_0_23__VALID                                            (0x80000000)
#define U_PNIP__FRAMECLASS_0_23__COMPTYPE                                         (0x40000000)
#define U_PNIP__FRAMECLASS_0_23__ETHERTYPE                                        (0x0000FFFF)
#define U_PNIP__FRAMECLASS_0_23_RESET__VALUE                     0x0
#define U_PNIP__FRAMECLASS_0_24                                                   (0x1061005C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_0_24__VALID                                            (0x80000000)
#define U_PNIP__FRAMECLASS_0_24__COMPTYPE                                         (0x40000000)
#define U_PNIP__FRAMECLASS_0_24__ETHERTYPE                                        (0x0000FFFF)
#define U_PNIP__FRAMECLASS_0_24_RESET__VALUE                     0x0
#define U_PNIP__FRAMECLASS_1_1                                                    (0x10610100 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_1_1__FRAMEID_HIGH                                      (0xFFFF0000)
#define U_PNIP__FRAMECLASS_1_1__FRAMEID_LOW                                       (0x0000FFFF)
#define U_PNIP__FRAMECLASS_1_1_RESET__VALUE                      0xFFF0100
#define U_PNIP__FRAMECLASS_1_2                                                    (0x10610104 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_1_2__FRAMEID_HIGH                                      (0xFFFF0000)
#define U_PNIP__FRAMECLASS_1_2__FRAMEID_LOW                                       (0x0000FFFF)
#define U_PNIP__FRAMECLASS_1_2_RESET__VALUE                      0x57FF4800
#define U_PNIP__FRAMECLASS_1_3                                                    (0x10610108 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_1_3__FRAMEID_HIGH                                      (0xFFFF0000)
#define U_PNIP__FRAMECLASS_1_3__FRAMEID_LOW                                       (0x0000FFFF)
#define U_PNIP__FRAMECLASS_1_3_RESET__VALUE                      0x67FF5800
#define U_PNIP__FRAMECLASS_1_4                                                    (0x1061010C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_1_4__FRAMEID_HIGH                                      (0xFFFF0000)
#define U_PNIP__FRAMECLASS_1_4__FRAMEID_LOW                                       (0x0000FFFF)
#define U_PNIP__FRAMECLASS_1_4_RESET__VALUE                      0x77FF6800
#define U_PNIP__FRAMECLASS_1_5                                                    (0x10610110 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_1_5__FRAMEID_HIGH                                      (0xFFFF0000)
#define U_PNIP__FRAMECLASS_1_5__FRAMEID_LOW                                       (0x0000FFFF)
#define U_PNIP__FRAMECLASS_1_5_RESET__VALUE                      0xBFFF7800
#define U_PNIP__FRAMECLASS_1_6                                                    (0x10610114 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_1_6__FRAMEID_HIGH                                      (0xFFFF0000)
#define U_PNIP__FRAMECLASS_1_6__FRAMEID_LOW                                       (0x0000FFFF)
#define U_PNIP__FRAMECLASS_1_6_RESET__VALUE                      0xFBFFC000
#define U_PNIP__FRAMECLASS_1_7                                                    (0x10610118 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_1_7__FRAMEID_HIGH                                      (0xFFFF0000)
#define U_PNIP__FRAMECLASS_1_7__FRAMEID_LOW                                       (0x0000FFFF)
#define U_PNIP__FRAMECLASS_1_7_RESET__VALUE                      0xFE01FC01
#define U_PNIP__FRAMECLASS_1_8                                                    (0x1061011C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_1_8__FRAMEID_HIGH                                      (0xFFFF0000)
#define U_PNIP__FRAMECLASS_1_8__FRAMEID_LOW                                       (0x0000FFFF)
#define U_PNIP__FRAMECLASS_1_8_RESET__VALUE                      0xFEFFFEFC
#define U_PNIP__FRAMECLASS_1_9                                                    (0x10610120 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_1_9__FRAMEID_HIGH                                      (0xFFFF0000)
#define U_PNIP__FRAMECLASS_1_9__FRAMEID_LOW                                       (0x0000FFFF)
#define U_PNIP__FRAMECLASS_1_9_RESET__VALUE                      0xFF43FF40
#define U_PNIP__FRAMECLASS_1_10                                                   (0x10610124 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_1_10__FRAMEID_HIGH                                     (0xFFFF0000)
#define U_PNIP__FRAMECLASS_1_10__FRAMEID_LOW                                      (0x0000FFFF)
#define U_PNIP__FRAMECLASS_1_10_RESET__VALUE                     0xFF42FF41
#define U_PNIP__FRAMECLASS_1_11                                                   (0x10610128 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_1_11__FRAMEID_HIGH                                     (0xFFFF0000)
#define U_PNIP__FRAMECLASS_1_11__FRAMEID_LOW                                      (0x0000FFFF)
#define U_PNIP__FRAMECLASS_1_11_RESET__VALUE                     0xFFFF0000
#define U_PNIP__FRAMECLASS_1_12                                                   (0x1061012C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_1_12__FRAMEID_HIGH                                     (0xFFFF0000)
#define U_PNIP__FRAMECLASS_1_12__FRAMEID_LOW                                      (0x0000FFFF)
#define U_PNIP__FRAMECLASS_1_12_RESET__VALUE                     0xFFFF0000
#define U_PNIP__FRAMECLASS_1_13                                                   (0x10610130 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_1_13__FRAMEID_HIGH                                     (0xFFFF0000)
#define U_PNIP__FRAMECLASS_1_13__FRAMEID_LOW                                      (0x0000FFFF)
#define U_PNIP__FRAMECLASS_1_13_RESET__VALUE                     0xFFFF0000
#define U_PNIP__FRAMECLASS_1_14                                                   (0x10610134 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_1_14__FRAMEID_HIGH                                     (0xFFFF0000)
#define U_PNIP__FRAMECLASS_1_14__FRAMEID_LOW                                      (0x0000FFFF)
#define U_PNIP__FRAMECLASS_1_14_RESET__VALUE                     0xFFFF0000
#define U_PNIP__FRAMECLASS_1_15                                                   (0x10610138 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_1_15__FRAMEID_HIGH                                     (0xFFFF0000)
#define U_PNIP__FRAMECLASS_1_15__FRAMEID_LOW                                      (0x0000FFFF)
#define U_PNIP__FRAMECLASS_1_15_RESET__VALUE                     0xFFFF0000
#define U_PNIP__FRAMECLASS_1_16                                                   (0x1061013C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_1_16__FRAMEID_HIGH                                     (0xFFFF0000)
#define U_PNIP__FRAMECLASS_1_16__FRAMEID_LOW                                      (0x0000FFFF)
#define U_PNIP__FRAMECLASS_1_16_RESET__VALUE                     0xFFFF0000
#define U_PNIP__FRAMECLASS_1_17                                                   (0x10610140 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_1_17__FRAMEID_HIGH                                     (0xFFFF0000)
#define U_PNIP__FRAMECLASS_1_17__FRAMEID_LOW                                      (0x0000FFFF)
#define U_PNIP__FRAMECLASS_1_17_RESET__VALUE                     0x0
#define U_PNIP__FRAMECLASS_1_18                                                   (0x10610144 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_1_18__FRAMEID_HIGH                                     (0xFFFF0000)
#define U_PNIP__FRAMECLASS_1_18__FRAMEID_LOW                                      (0x0000FFFF)
#define U_PNIP__FRAMECLASS_1_18_RESET__VALUE                     0x0
#define U_PNIP__FRAMECLASS_1_19                                                   (0x10610148 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_1_19__FRAMEID_HIGH                                     (0xFFFF0000)
#define U_PNIP__FRAMECLASS_1_19__FRAMEID_LOW                                      (0x0000FFFF)
#define U_PNIP__FRAMECLASS_1_19_RESET__VALUE                     0x0
#define U_PNIP__FRAMECLASS_1_20                                                   (0x1061014C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_1_20__FRAMEID_HIGH                                     (0xFFFF0000)
#define U_PNIP__FRAMECLASS_1_20__FRAMEID_LOW                                      (0x0000FFFF)
#define U_PNIP__FRAMECLASS_1_20_RESET__VALUE                     0x0
#define U_PNIP__FRAMECLASS_1_21                                                   (0x10610150 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_1_21__FRAMEID_HIGH                                     (0xFFFF0000)
#define U_PNIP__FRAMECLASS_1_21__FRAMEID_LOW                                      (0x0000FFFF)
#define U_PNIP__FRAMECLASS_1_21_RESET__VALUE                     0x0
#define U_PNIP__FRAMECLASS_1_22                                                   (0x10610154 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_1_22__FRAMEID_HIGH                                     (0xFFFF0000)
#define U_PNIP__FRAMECLASS_1_22__FRAMEID_LOW                                      (0x0000FFFF)
#define U_PNIP__FRAMECLASS_1_22_RESET__VALUE                     0x0
#define U_PNIP__FRAMECLASS_1_23                                                   (0x10610158 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_1_23__FRAMEID_HIGH                                     (0xFFFF0000)
#define U_PNIP__FRAMECLASS_1_23__FRAMEID_LOW                                      (0x0000FFFF)
#define U_PNIP__FRAMECLASS_1_23_RESET__VALUE                     0x0
#define U_PNIP__FRAMECLASS_1_24                                                   (0x1061015C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_1_24__FRAMEID_HIGH                                     (0xFFFF0000)
#define U_PNIP__FRAMECLASS_1_24__FRAMEID_LOW                                      (0x0000FFFF)
#define U_PNIP__FRAMECLASS_1_24_RESET__VALUE                     0x0
#define U_PNIP__FRAMECLASS_2_1                                                    (0x10610200 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_2_1__PM_SELECT                                         (0x80000000)
#define U_PNIP__FRAMECLASS_2_1__PM                                                (0x40000000)
#define U_PNIP__FRAMECLASS_2_1__RESOURCETYPE_SELECT                               (0x20000000)
#define U_PNIP__FRAMECLASS_2_1__RESOURCETYPE                                      (0x0F000000)
#define U_PNIP__FRAMECLASS_2_1__APITYPE_SELECT                                    (0x00800000)
#define U_PNIP__FRAMECLASS_2_1__APITYPE                                           (0x003C0000)
#define U_PNIP__FRAMECLASS_2_1__BITTYPE_SELECT                                    (0x00030000)
#define U_PNIP__FRAMECLASS_2_1__LEARNING_DISABLE                                  (0x00008000)
#define U_PNIP__FRAMECLASS_2_1__BRIDGEDELAY_ENABLE                                (0x00004000)
#define U_PNIP__FRAMECLASS_2_1__FRAGMENTATION                                     (0x00002000)
#define U_PNIP__FRAMECLASS_2_1__REDUNDANCY                                        (0x00001000)
#define U_PNIP__FRAMECLASS_2_1__FRAMETRAFFIC_SELECT                               (0x00000800)
#define U_PNIP__FRAMECLASS_2_1__DEFAULTTRAFFIC_SELECT                             (0x00000400)
#define U_PNIP__FRAMECLASS_2_1__DEFAULTTRAFFIC_CLASS                              (0x00000380)
#define U_PNIP__FRAMECLASS_2_1__USETYPE_SELECT                                    (0x00000040)
#define U_PNIP__FRAMECLASS_2_1__USETYPE                                           (0x00000038)
#define U_PNIP__FRAMECLASS_2_1__FRAMEHANDLING_SELECT                              (0x00000004)
#define U_PNIP__FRAMECLASS_2_1__FRAMEHANDLING                                     (0x00000003)
#define U_PNIP__FRAMECLASS_2_1_RESET__VALUE                      0x4158003
#define U_PNIP__FRAMECLASS_2_2                                                    (0x10610204 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_2_2__PM_SELECT                                         (0x80000000)
#define U_PNIP__FRAMECLASS_2_2__PM                                                (0x40000000)
#define U_PNIP__FRAMECLASS_2_2__RESOURCETYPE_SELECT                               (0x20000000)
#define U_PNIP__FRAMECLASS_2_2__RESOURCETYPE                                      (0x0F000000)
#define U_PNIP__FRAMECLASS_2_2__APITYPE_SELECT                                    (0x00800000)
#define U_PNIP__FRAMECLASS_2_2__APITYPE                                           (0x003C0000)
#define U_PNIP__FRAMECLASS_2_2__BITTYPE_SELECT                                    (0x00030000)
#define U_PNIP__FRAMECLASS_2_2__LEARNING_DISABLE                                  (0x00008000)
#define U_PNIP__FRAMECLASS_2_2__BRIDGEDELAY_ENABLE                                (0x00004000)
#define U_PNIP__FRAMECLASS_2_2__FRAGMENTATION                                     (0x00002000)
#define U_PNIP__FRAMECLASS_2_2__REDUNDANCY                                        (0x00001000)
#define U_PNIP__FRAMECLASS_2_2__FRAMETRAFFIC_SELECT                               (0x00000800)
#define U_PNIP__FRAMECLASS_2_2__DEFAULTTRAFFIC_SELECT                             (0x00000400)
#define U_PNIP__FRAMECLASS_2_2__DEFAULTTRAFFIC_CLASS                              (0x00000380)
#define U_PNIP__FRAMECLASS_2_2__USETYPE_SELECT                                    (0x00000040)
#define U_PNIP__FRAMECLASS_2_2__USETYPE                                           (0x00000038)
#define U_PNIP__FRAMECLASS_2_2__FRAMEHANDLING_SELECT                              (0x00000004)
#define U_PNIP__FRAMECLASS_2_2__FRAMEHANDLING                                     (0x00000003)
#define U_PNIP__FRAMECLASS_2_2_RESET__VALUE                      0x5151003
#define U_PNIP__FRAMECLASS_2_3                                                    (0x10610208 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_2_3__PM_SELECT                                         (0x80000000)
#define U_PNIP__FRAMECLASS_2_3__PM                                                (0x40000000)
#define U_PNIP__FRAMECLASS_2_3__RESOURCETYPE_SELECT                               (0x20000000)
#define U_PNIP__FRAMECLASS_2_3__RESOURCETYPE                                      (0x0F000000)
#define U_PNIP__FRAMECLASS_2_3__APITYPE_SELECT                                    (0x00800000)
#define U_PNIP__FRAMECLASS_2_3__APITYPE                                           (0x003C0000)
#define U_PNIP__FRAMECLASS_2_3__BITTYPE_SELECT                                    (0x00030000)
#define U_PNIP__FRAMECLASS_2_3__LEARNING_DISABLE                                  (0x00008000)
#define U_PNIP__FRAMECLASS_2_3__BRIDGEDELAY_ENABLE                                (0x00004000)
#define U_PNIP__FRAMECLASS_2_3__FRAGMENTATION                                     (0x00002000)
#define U_PNIP__FRAMECLASS_2_3__REDUNDANCY                                        (0x00001000)
#define U_PNIP__FRAMECLASS_2_3__FRAMETRAFFIC_SELECT                               (0x00000800)
#define U_PNIP__FRAMECLASS_2_3__DEFAULTTRAFFIC_SELECT                             (0x00000400)
#define U_PNIP__FRAMECLASS_2_3__DEFAULTTRAFFIC_CLASS                              (0x00000380)
#define U_PNIP__FRAMECLASS_2_3__USETYPE_SELECT                                    (0x00000040)
#define U_PNIP__FRAMECLASS_2_3__USETYPE                                           (0x00000038)
#define U_PNIP__FRAMECLASS_2_3__FRAMEHANDLING_SELECT                              (0x00000004)
#define U_PNIP__FRAMECLASS_2_3__FRAMEHANDLING                                     (0x00000003)
#define U_PNIP__FRAMECLASS_2_3_RESET__VALUE                      0x5150003
#define U_PNIP__FRAMECLASS_2_4                                                    (0x1061020C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_2_4__PM_SELECT                                         (0x80000000)
#define U_PNIP__FRAMECLASS_2_4__PM                                                (0x40000000)
#define U_PNIP__FRAMECLASS_2_4__RESOURCETYPE_SELECT                               (0x20000000)
#define U_PNIP__FRAMECLASS_2_4__RESOURCETYPE                                      (0x0F000000)
#define U_PNIP__FRAMECLASS_2_4__APITYPE_SELECT                                    (0x00800000)
#define U_PNIP__FRAMECLASS_2_4__APITYPE                                           (0x003C0000)
#define U_PNIP__FRAMECLASS_2_4__BITTYPE_SELECT                                    (0x00030000)
#define U_PNIP__FRAMECLASS_2_4__LEARNING_DISABLE                                  (0x00008000)
#define U_PNIP__FRAMECLASS_2_4__BRIDGEDELAY_ENABLE                                (0x00004000)
#define U_PNIP__FRAMECLASS_2_4__FRAGMENTATION                                     (0x00002000)
#define U_PNIP__FRAMECLASS_2_4__REDUNDANCY                                        (0x00001000)
#define U_PNIP__FRAMECLASS_2_4__FRAMETRAFFIC_SELECT                               (0x00000800)
#define U_PNIP__FRAMECLASS_2_4__DEFAULTTRAFFIC_SELECT                             (0x00000400)
#define U_PNIP__FRAMECLASS_2_4__DEFAULTTRAFFIC_CLASS                              (0x00000380)
#define U_PNIP__FRAMECLASS_2_4__USETYPE_SELECT                                    (0x00000040)
#define U_PNIP__FRAMECLASS_2_4__USETYPE                                           (0x00000038)
#define U_PNIP__FRAMECLASS_2_4__FRAMEHANDLING_SELECT                              (0x00000004)
#define U_PNIP__FRAMECLASS_2_4__FRAMEHANDLING                                     (0x00000003)
#define U_PNIP__FRAMECLASS_2_4_RESET__VALUE                      0x5151002
#define U_PNIP__FRAMECLASS_2_5                                                    (0x10610210 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_2_5__PM_SELECT                                         (0x80000000)
#define U_PNIP__FRAMECLASS_2_5__PM                                                (0x40000000)
#define U_PNIP__FRAMECLASS_2_5__RESOURCETYPE_SELECT                               (0x20000000)
#define U_PNIP__FRAMECLASS_2_5__RESOURCETYPE                                      (0x0F000000)
#define U_PNIP__FRAMECLASS_2_5__APITYPE_SELECT                                    (0x00800000)
#define U_PNIP__FRAMECLASS_2_5__APITYPE                                           (0x003C0000)
#define U_PNIP__FRAMECLASS_2_5__BITTYPE_SELECT                                    (0x00030000)
#define U_PNIP__FRAMECLASS_2_5__LEARNING_DISABLE                                  (0x00008000)
#define U_PNIP__FRAMECLASS_2_5__BRIDGEDELAY_ENABLE                                (0x00004000)
#define U_PNIP__FRAMECLASS_2_5__FRAGMENTATION                                     (0x00002000)
#define U_PNIP__FRAMECLASS_2_5__REDUNDANCY                                        (0x00001000)
#define U_PNIP__FRAMECLASS_2_5__FRAMETRAFFIC_SELECT                               (0x00000800)
#define U_PNIP__FRAMECLASS_2_5__DEFAULTTRAFFIC_SELECT                             (0x00000400)
#define U_PNIP__FRAMECLASS_2_5__DEFAULTTRAFFIC_CLASS                              (0x00000380)
#define U_PNIP__FRAMECLASS_2_5__USETYPE_SELECT                                    (0x00000040)
#define U_PNIP__FRAMECLASS_2_5__USETYPE                                           (0x00000038)
#define U_PNIP__FRAMECLASS_2_5__FRAMEHANDLING_SELECT                              (0x00000004)
#define U_PNIP__FRAMECLASS_2_5__FRAMEHANDLING                                     (0x00000003)
#define U_PNIP__FRAMECLASS_2_5_RESET__VALUE                      0x5150002
#define U_PNIP__FRAMECLASS_2_6                                                    (0x10610214 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_2_6__PM_SELECT                                         (0x80000000)
#define U_PNIP__FRAMECLASS_2_6__PM                                                (0x40000000)
#define U_PNIP__FRAMECLASS_2_6__RESOURCETYPE_SELECT                               (0x20000000)
#define U_PNIP__FRAMECLASS_2_6__RESOURCETYPE                                      (0x0F000000)
#define U_PNIP__FRAMECLASS_2_6__APITYPE_SELECT                                    (0x00800000)
#define U_PNIP__FRAMECLASS_2_6__APITYPE                                           (0x003C0000)
#define U_PNIP__FRAMECLASS_2_6__BITTYPE_SELECT                                    (0x00030000)
#define U_PNIP__FRAMECLASS_2_6__LEARNING_DISABLE                                  (0x00008000)
#define U_PNIP__FRAMECLASS_2_6__BRIDGEDELAY_ENABLE                                (0x00004000)
#define U_PNIP__FRAMECLASS_2_6__FRAGMENTATION                                     (0x00002000)
#define U_PNIP__FRAMECLASS_2_6__REDUNDANCY                                        (0x00001000)
#define U_PNIP__FRAMECLASS_2_6__FRAMETRAFFIC_SELECT                               (0x00000800)
#define U_PNIP__FRAMECLASS_2_6__DEFAULTTRAFFIC_SELECT                             (0x00000400)
#define U_PNIP__FRAMECLASS_2_6__DEFAULTTRAFFIC_CLASS                              (0x00000380)
#define U_PNIP__FRAMECLASS_2_6__USETYPE_SELECT                                    (0x00000040)
#define U_PNIP__FRAMECLASS_2_6__USETYPE                                           (0x00000038)
#define U_PNIP__FRAMECLASS_2_6__FRAMEHANDLING_SELECT                              (0x00000004)
#define U_PNIP__FRAMECLASS_2_6__FRAMEHANDLING                                     (0x00000003)
#define U_PNIP__FRAMECLASS_2_6_RESET__VALUE                      0x6090000
#define U_PNIP__FRAMECLASS_2_7                                                    (0x10610218 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_2_7__PM_SELECT                                         (0x80000000)
#define U_PNIP__FRAMECLASS_2_7__PM                                                (0x40000000)
#define U_PNIP__FRAMECLASS_2_7__RESOURCETYPE_SELECT                               (0x20000000)
#define U_PNIP__FRAMECLASS_2_7__RESOURCETYPE                                      (0x0F000000)
#define U_PNIP__FRAMECLASS_2_7__APITYPE_SELECT                                    (0x00800000)
#define U_PNIP__FRAMECLASS_2_7__APITYPE                                           (0x003C0000)
#define U_PNIP__FRAMECLASS_2_7__BITTYPE_SELECT                                    (0x00030000)
#define U_PNIP__FRAMECLASS_2_7__LEARNING_DISABLE                                  (0x00008000)
#define U_PNIP__FRAMECLASS_2_7__BRIDGEDELAY_ENABLE                                (0x00004000)
#define U_PNIP__FRAMECLASS_2_7__FRAGMENTATION                                     (0x00002000)
#define U_PNIP__FRAMECLASS_2_7__REDUNDANCY                                        (0x00001000)
#define U_PNIP__FRAMECLASS_2_7__FRAMETRAFFIC_SELECT                               (0x00000800)
#define U_PNIP__FRAMECLASS_2_7__DEFAULTTRAFFIC_SELECT                             (0x00000400)
#define U_PNIP__FRAMECLASS_2_7__DEFAULTTRAFFIC_CLASS                              (0x00000380)
#define U_PNIP__FRAMECLASS_2_7__USETYPE_SELECT                                    (0x00000040)
#define U_PNIP__FRAMECLASS_2_7__USETYPE                                           (0x00000038)
#define U_PNIP__FRAMECLASS_2_7__FRAMEHANDLING_SELECT                              (0x00000004)
#define U_PNIP__FRAMECLASS_2_7__FRAMEHANDLING                                     (0x00000003)
#define U_PNIP__FRAMECLASS_2_7_RESET__VALUE                      0x71D2001
#define U_PNIP__FRAMECLASS_2_8                                                    (0x1061021C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_2_8__PM_SELECT                                         (0x80000000)
#define U_PNIP__FRAMECLASS_2_8__PM                                                (0x40000000)
#define U_PNIP__FRAMECLASS_2_8__RESOURCETYPE_SELECT                               (0x20000000)
#define U_PNIP__FRAMECLASS_2_8__RESOURCETYPE                                      (0x0F000000)
#define U_PNIP__FRAMECLASS_2_8__APITYPE_SELECT                                    (0x00800000)
#define U_PNIP__FRAMECLASS_2_8__APITYPE                                           (0x003C0000)
#define U_PNIP__FRAMECLASS_2_8__BITTYPE_SELECT                                    (0x00030000)
#define U_PNIP__FRAMECLASS_2_8__LEARNING_DISABLE                                  (0x00008000)
#define U_PNIP__FRAMECLASS_2_8__BRIDGEDELAY_ENABLE                                (0x00004000)
#define U_PNIP__FRAMECLASS_2_8__FRAGMENTATION                                     (0x00002000)
#define U_PNIP__FRAMECLASS_2_8__REDUNDANCY                                        (0x00001000)
#define U_PNIP__FRAMECLASS_2_8__FRAMETRAFFIC_SELECT                               (0x00000800)
#define U_PNIP__FRAMECLASS_2_8__DEFAULTTRAFFIC_SELECT                             (0x00000400)
#define U_PNIP__FRAMECLASS_2_8__DEFAULTTRAFFIC_CLASS                              (0x00000380)
#define U_PNIP__FRAMECLASS_2_8__USETYPE_SELECT                                    (0x00000040)
#define U_PNIP__FRAMECLASS_2_8__USETYPE                                           (0x00000038)
#define U_PNIP__FRAMECLASS_2_8__FRAMEHANDLING_SELECT                              (0x00000004)
#define U_PNIP__FRAMECLASS_2_8__FRAMEHANDLING                                     (0x00000003)
#define U_PNIP__FRAMECLASS_2_8_RESET__VALUE                      0xA0A32444
#define U_PNIP__FRAMECLASS_2_9                                                    (0x10610220 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_2_9__PM_SELECT                                         (0x80000000)
#define U_PNIP__FRAMECLASS_2_9__PM                                                (0x40000000)
#define U_PNIP__FRAMECLASS_2_9__RESOURCETYPE_SELECT                               (0x20000000)
#define U_PNIP__FRAMECLASS_2_9__RESOURCETYPE                                      (0x0F000000)
#define U_PNIP__FRAMECLASS_2_9__APITYPE_SELECT                                    (0x00800000)
#define U_PNIP__FRAMECLASS_2_9__APITYPE                                           (0x003C0000)
#define U_PNIP__FRAMECLASS_2_9__BITTYPE_SELECT                                    (0x00030000)
#define U_PNIP__FRAMECLASS_2_9__LEARNING_DISABLE                                  (0x00008000)
#define U_PNIP__FRAMECLASS_2_9__BRIDGEDELAY_ENABLE                                (0x00004000)
#define U_PNIP__FRAMECLASS_2_9__FRAGMENTATION                                     (0x00002000)
#define U_PNIP__FRAMECLASS_2_9__REDUNDANCY                                        (0x00001000)
#define U_PNIP__FRAMECLASS_2_9__FRAMETRAFFIC_SELECT                               (0x00000800)
#define U_PNIP__FRAMECLASS_2_9__DEFAULTTRAFFIC_SELECT                             (0x00000400)
#define U_PNIP__FRAMECLASS_2_9__DEFAULTTRAFFIC_CLASS                              (0x00000380)
#define U_PNIP__FRAMECLASS_2_9__USETYPE_SELECT                                    (0x00000040)
#define U_PNIP__FRAMECLASS_2_9__USETYPE                                           (0x00000038)
#define U_PNIP__FRAMECLASS_2_9__FRAMEHANDLING_SELECT                              (0x00000004)
#define U_PNIP__FRAMECLASS_2_9__FRAMEHANDLING                                     (0x00000003)
#define U_PNIP__FRAMECLASS_2_9_RESET__VALUE                      0x30D0018
#define U_PNIP__FRAMECLASS_2_10                                                   (0x10610224 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_2_10__PM_SELECT                                        (0x80000000)
#define U_PNIP__FRAMECLASS_2_10__PM                                               (0x40000000)
#define U_PNIP__FRAMECLASS_2_10__RESOURCETYPE_SELECT                              (0x20000000)
#define U_PNIP__FRAMECLASS_2_10__RESOURCETYPE                                     (0x0F000000)
#define U_PNIP__FRAMECLASS_2_10__APITYPE_SELECT                                   (0x00800000)
#define U_PNIP__FRAMECLASS_2_10__APITYPE                                          (0x003C0000)
#define U_PNIP__FRAMECLASS_2_10__BITTYPE_SELECT                                   (0x00030000)
#define U_PNIP__FRAMECLASS_2_10__LEARNING_DISABLE                                 (0x00008000)
#define U_PNIP__FRAMECLASS_2_10__BRIDGEDELAY_ENABLE                               (0x00004000)
#define U_PNIP__FRAMECLASS_2_10__FRAGMENTATION                                    (0x00002000)
#define U_PNIP__FRAMECLASS_2_10__REDUNDANCY                                       (0x00001000)
#define U_PNIP__FRAMECLASS_2_10__FRAMETRAFFIC_SELECT                              (0x00000800)
#define U_PNIP__FRAMECLASS_2_10__DEFAULTTRAFFIC_SELECT                            (0x00000400)
#define U_PNIP__FRAMECLASS_2_10__DEFAULTTRAFFIC_CLASS                             (0x00000380)
#define U_PNIP__FRAMECLASS_2_10__USETYPE_SELECT                                   (0x00000040)
#define U_PNIP__FRAMECLASS_2_10__USETYPE                                          (0x00000038)
#define U_PNIP__FRAMECLASS_2_10__FRAMEHANDLING_SELECT                             (0x00000004)
#define U_PNIP__FRAMECLASS_2_10__FRAMEHANDLING                                    (0x00000003)
#define U_PNIP__FRAMECLASS_2_10_RESET__VALUE                     0xA0810400
#define U_PNIP__FRAMECLASS_2_11                                                   (0x10610228 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_2_11__PM_SELECT                                        (0x80000000)
#define U_PNIP__FRAMECLASS_2_11__PM                                               (0x40000000)
#define U_PNIP__FRAMECLASS_2_11__RESOURCETYPE_SELECT                              (0x20000000)
#define U_PNIP__FRAMECLASS_2_11__RESOURCETYPE                                     (0x0F000000)
#define U_PNIP__FRAMECLASS_2_11__APITYPE_SELECT                                   (0x00800000)
#define U_PNIP__FRAMECLASS_2_11__APITYPE                                          (0x003C0000)
#define U_PNIP__FRAMECLASS_2_11__BITTYPE_SELECT                                   (0x00030000)
#define U_PNIP__FRAMECLASS_2_11__LEARNING_DISABLE                                 (0x00008000)
#define U_PNIP__FRAMECLASS_2_11__BRIDGEDELAY_ENABLE                               (0x00004000)
#define U_PNIP__FRAMECLASS_2_11__FRAGMENTATION                                    (0x00002000)
#define U_PNIP__FRAMECLASS_2_11__REDUNDANCY                                       (0x00001000)
#define U_PNIP__FRAMECLASS_2_11__FRAMETRAFFIC_SELECT                              (0x00000800)
#define U_PNIP__FRAMECLASS_2_11__DEFAULTTRAFFIC_SELECT                            (0x00000400)
#define U_PNIP__FRAMECLASS_2_11__DEFAULTTRAFFIC_CLASS                             (0x00000380)
#define U_PNIP__FRAMECLASS_2_11__USETYPE_SELECT                                   (0x00000040)
#define U_PNIP__FRAMECLASS_2_11__USETYPE                                          (0x00000038)
#define U_PNIP__FRAMECLASS_2_11__FRAMEHANDLING_SELECT                             (0x00000004)
#define U_PNIP__FRAMECLASS_2_11__FRAMEHANDLING                                    (0x00000003)
#define U_PNIP__FRAMECLASS_2_11_RESET__VALUE                     0x2290030
#define U_PNIP__FRAMECLASS_2_12                                                   (0x1061022C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_2_12__PM_SELECT                                        (0x80000000)
#define U_PNIP__FRAMECLASS_2_12__PM                                               (0x40000000)
#define U_PNIP__FRAMECLASS_2_12__RESOURCETYPE_SELECT                              (0x20000000)
#define U_PNIP__FRAMECLASS_2_12__RESOURCETYPE                                     (0x0F000000)
#define U_PNIP__FRAMECLASS_2_12__APITYPE_SELECT                                   (0x00800000)
#define U_PNIP__FRAMECLASS_2_12__APITYPE                                          (0x003C0000)
#define U_PNIP__FRAMECLASS_2_12__BITTYPE_SELECT                                   (0x00030000)
#define U_PNIP__FRAMECLASS_2_12__LEARNING_DISABLE                                 (0x00008000)
#define U_PNIP__FRAMECLASS_2_12__BRIDGEDELAY_ENABLE                               (0x00004000)
#define U_PNIP__FRAMECLASS_2_12__FRAGMENTATION                                    (0x00002000)
#define U_PNIP__FRAMECLASS_2_12__REDUNDANCY                                       (0x00001000)
#define U_PNIP__FRAMECLASS_2_12__FRAMETRAFFIC_SELECT                              (0x00000800)
#define U_PNIP__FRAMECLASS_2_12__DEFAULTTRAFFIC_SELECT                            (0x00000400)
#define U_PNIP__FRAMECLASS_2_12__DEFAULTTRAFFIC_CLASS                             (0x00000380)
#define U_PNIP__FRAMECLASS_2_12__USETYPE_SELECT                                   (0x00000040)
#define U_PNIP__FRAMECLASS_2_12__USETYPE                                          (0x00000038)
#define U_PNIP__FRAMECLASS_2_12__FRAMEHANDLING_SELECT                             (0x00000004)
#define U_PNIP__FRAMECLASS_2_12__FRAMEHANDLING                                    (0x00000003)
#define U_PNIP__FRAMECLASS_2_12_RESET__VALUE                     0x252000
#define U_PNIP__FRAMECLASS_2_13                                                   (0x10610230 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_2_13__PM_SELECT                                        (0x80000000)
#define U_PNIP__FRAMECLASS_2_13__PM                                               (0x40000000)
#define U_PNIP__FRAMECLASS_2_13__RESOURCETYPE_SELECT                              (0x20000000)
#define U_PNIP__FRAMECLASS_2_13__RESOURCETYPE                                     (0x0F000000)
#define U_PNIP__FRAMECLASS_2_13__APITYPE_SELECT                                   (0x00800000)
#define U_PNIP__FRAMECLASS_2_13__APITYPE                                          (0x003C0000)
#define U_PNIP__FRAMECLASS_2_13__BITTYPE_SELECT                                   (0x00030000)
#define U_PNIP__FRAMECLASS_2_13__LEARNING_DISABLE                                 (0x00008000)
#define U_PNIP__FRAMECLASS_2_13__BRIDGEDELAY_ENABLE                               (0x00004000)
#define U_PNIP__FRAMECLASS_2_13__FRAGMENTATION                                    (0x00002000)
#define U_PNIP__FRAMECLASS_2_13__REDUNDANCY                                       (0x00001000)
#define U_PNIP__FRAMECLASS_2_13__FRAMETRAFFIC_SELECT                              (0x00000800)
#define U_PNIP__FRAMECLASS_2_13__DEFAULTTRAFFIC_SELECT                            (0x00000400)
#define U_PNIP__FRAMECLASS_2_13__DEFAULTTRAFFIC_CLASS                             (0x00000380)
#define U_PNIP__FRAMECLASS_2_13__USETYPE_SELECT                                   (0x00000040)
#define U_PNIP__FRAMECLASS_2_13__USETYPE                                          (0x00000038)
#define U_PNIP__FRAMECLASS_2_13__FRAMEHANDLING_SELECT                             (0x00000004)
#define U_PNIP__FRAMECLASS_2_13__FRAMEHANDLING                                    (0x00000003)
#define U_PNIP__FRAMECLASS_2_13_RESET__VALUE                     0x9352000
#define U_PNIP__FRAMECLASS_2_14                                                   (0x10610234 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_2_14__PM_SELECT                                        (0x80000000)
#define U_PNIP__FRAMECLASS_2_14__PM                                               (0x40000000)
#define U_PNIP__FRAMECLASS_2_14__RESOURCETYPE_SELECT                              (0x20000000)
#define U_PNIP__FRAMECLASS_2_14__RESOURCETYPE                                     (0x0F000000)
#define U_PNIP__FRAMECLASS_2_14__APITYPE_SELECT                                   (0x00800000)
#define U_PNIP__FRAMECLASS_2_14__APITYPE                                          (0x003C0000)
#define U_PNIP__FRAMECLASS_2_14__BITTYPE_SELECT                                   (0x00030000)
#define U_PNIP__FRAMECLASS_2_14__LEARNING_DISABLE                                 (0x00008000)
#define U_PNIP__FRAMECLASS_2_14__BRIDGEDELAY_ENABLE                               (0x00004000)
#define U_PNIP__FRAMECLASS_2_14__FRAGMENTATION                                    (0x00002000)
#define U_PNIP__FRAMECLASS_2_14__REDUNDANCY                                       (0x00001000)
#define U_PNIP__FRAMECLASS_2_14__FRAMETRAFFIC_SELECT                              (0x00000800)
#define U_PNIP__FRAMECLASS_2_14__DEFAULTTRAFFIC_SELECT                            (0x00000400)
#define U_PNIP__FRAMECLASS_2_14__DEFAULTTRAFFIC_CLASS                             (0x00000380)
#define U_PNIP__FRAMECLASS_2_14__USETYPE_SELECT                                   (0x00000040)
#define U_PNIP__FRAMECLASS_2_14__USETYPE                                          (0x00000038)
#define U_PNIP__FRAMECLASS_2_14__FRAMEHANDLING_SELECT                             (0x00000004)
#define U_PNIP__FRAMECLASS_2_14__FRAMEHANDLING                                    (0x00000003)
#define U_PNIP__FRAMECLASS_2_14_RESET__VALUE                     0x392000
#define U_PNIP__FRAMECLASS_2_15                                                   (0x10610238 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_2_15__PM_SELECT                                        (0x80000000)
#define U_PNIP__FRAMECLASS_2_15__PM                                               (0x40000000)
#define U_PNIP__FRAMECLASS_2_15__RESOURCETYPE_SELECT                              (0x20000000)
#define U_PNIP__FRAMECLASS_2_15__RESOURCETYPE                                     (0x0F000000)
#define U_PNIP__FRAMECLASS_2_15__APITYPE_SELECT                                   (0x00800000)
#define U_PNIP__FRAMECLASS_2_15__APITYPE                                          (0x003C0000)
#define U_PNIP__FRAMECLASS_2_15__BITTYPE_SELECT                                   (0x00030000)
#define U_PNIP__FRAMECLASS_2_15__LEARNING_DISABLE                                 (0x00008000)
#define U_PNIP__FRAMECLASS_2_15__BRIDGEDELAY_ENABLE                               (0x00004000)
#define U_PNIP__FRAMECLASS_2_15__FRAGMENTATION                                    (0x00002000)
#define U_PNIP__FRAMECLASS_2_15__REDUNDANCY                                       (0x00001000)
#define U_PNIP__FRAMECLASS_2_15__FRAMETRAFFIC_SELECT                              (0x00000800)
#define U_PNIP__FRAMECLASS_2_15__DEFAULTTRAFFIC_SELECT                            (0x00000400)
#define U_PNIP__FRAMECLASS_2_15__DEFAULTTRAFFIC_CLASS                             (0x00000380)
#define U_PNIP__FRAMECLASS_2_15__USETYPE_SELECT                                   (0x00000040)
#define U_PNIP__FRAMECLASS_2_15__USETYPE                                          (0x00000038)
#define U_PNIP__FRAMECLASS_2_15__FRAMEHANDLING_SELECT                             (0x00000004)
#define U_PNIP__FRAMECLASS_2_15__FRAMEHANDLING                                    (0x00000003)
#define U_PNIP__FRAMECLASS_2_15_RESET__VALUE                     0xA8AF2444
#define U_PNIP__FRAMECLASS_2_16                                                   (0x1061023C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_2_16__PM_SELECT                                        (0x80000000)
#define U_PNIP__FRAMECLASS_2_16__PM                                               (0x40000000)
#define U_PNIP__FRAMECLASS_2_16__RESOURCETYPE_SELECT                              (0x20000000)
#define U_PNIP__FRAMECLASS_2_16__RESOURCETYPE                                     (0x0F000000)
#define U_PNIP__FRAMECLASS_2_16__APITYPE_SELECT                                   (0x00800000)
#define U_PNIP__FRAMECLASS_2_16__APITYPE                                          (0x003C0000)
#define U_PNIP__FRAMECLASS_2_16__BITTYPE_SELECT                                   (0x00030000)
#define U_PNIP__FRAMECLASS_2_16__LEARNING_DISABLE                                 (0x00008000)
#define U_PNIP__FRAMECLASS_2_16__BRIDGEDELAY_ENABLE                               (0x00004000)
#define U_PNIP__FRAMECLASS_2_16__FRAGMENTATION                                    (0x00002000)
#define U_PNIP__FRAMECLASS_2_16__REDUNDANCY                                       (0x00001000)
#define U_PNIP__FRAMECLASS_2_16__FRAMETRAFFIC_SELECT                              (0x00000800)
#define U_PNIP__FRAMECLASS_2_16__DEFAULTTRAFFIC_SELECT                            (0x00000400)
#define U_PNIP__FRAMECLASS_2_16__DEFAULTTRAFFIC_CLASS                             (0x00000380)
#define U_PNIP__FRAMECLASS_2_16__USETYPE_SELECT                                   (0x00000040)
#define U_PNIP__FRAMECLASS_2_16__USETYPE                                          (0x00000038)
#define U_PNIP__FRAMECLASS_2_16__FRAMEHANDLING_SELECT                             (0x00000004)
#define U_PNIP__FRAMECLASS_2_16__FRAMEHANDLING                                    (0x00000003)
#define U_PNIP__FRAMECLASS_2_16_RESET__VALUE                     0x3D2000
#define U_PNIP__FRAMECLASS_2_17                                                   (0x10610240 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_2_17__PM_SELECT                                        (0x80000000)
#define U_PNIP__FRAMECLASS_2_17__PM                                               (0x40000000)
#define U_PNIP__FRAMECLASS_2_17__RESOURCETYPE_SELECT                              (0x20000000)
#define U_PNIP__FRAMECLASS_2_17__RESOURCETYPE                                     (0x0F000000)
#define U_PNIP__FRAMECLASS_2_17__APITYPE_SELECT                                   (0x00800000)
#define U_PNIP__FRAMECLASS_2_17__APITYPE                                          (0x003C0000)
#define U_PNIP__FRAMECLASS_2_17__BITTYPE_SELECT                                   (0x00030000)
#define U_PNIP__FRAMECLASS_2_17__LEARNING_DISABLE                                 (0x00008000)
#define U_PNIP__FRAMECLASS_2_17__BRIDGEDELAY_ENABLE                               (0x00004000)
#define U_PNIP__FRAMECLASS_2_17__FRAGMENTATION                                    (0x00002000)
#define U_PNIP__FRAMECLASS_2_17__REDUNDANCY                                       (0x00001000)
#define U_PNIP__FRAMECLASS_2_17__FRAMETRAFFIC_SELECT                              (0x00000800)
#define U_PNIP__FRAMECLASS_2_17__DEFAULTTRAFFIC_SELECT                            (0x00000400)
#define U_PNIP__FRAMECLASS_2_17__DEFAULTTRAFFIC_CLASS                             (0x00000380)
#define U_PNIP__FRAMECLASS_2_17__USETYPE_SELECT                                   (0x00000040)
#define U_PNIP__FRAMECLASS_2_17__USETYPE                                          (0x00000038)
#define U_PNIP__FRAMECLASS_2_17__FRAMEHANDLING_SELECT                             (0x00000004)
#define U_PNIP__FRAMECLASS_2_17__FRAMEHANDLING                                    (0x00000003)
#define U_PNIP__FRAMECLASS_2_17_RESET__VALUE                     0x10000
#define U_PNIP__FRAMECLASS_2_18                                                   (0x10610244 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_2_18__PM_SELECT                                        (0x80000000)
#define U_PNIP__FRAMECLASS_2_18__PM                                               (0x40000000)
#define U_PNIP__FRAMECLASS_2_18__RESOURCETYPE_SELECT                              (0x20000000)
#define U_PNIP__FRAMECLASS_2_18__RESOURCETYPE                                     (0x0F000000)
#define U_PNIP__FRAMECLASS_2_18__APITYPE_SELECT                                   (0x00800000)
#define U_PNIP__FRAMECLASS_2_18__APITYPE                                          (0x003C0000)
#define U_PNIP__FRAMECLASS_2_18__BITTYPE_SELECT                                   (0x00030000)
#define U_PNIP__FRAMECLASS_2_18__LEARNING_DISABLE                                 (0x00008000)
#define U_PNIP__FRAMECLASS_2_18__BRIDGEDELAY_ENABLE                               (0x00004000)
#define U_PNIP__FRAMECLASS_2_18__FRAGMENTATION                                    (0x00002000)
#define U_PNIP__FRAMECLASS_2_18__REDUNDANCY                                       (0x00001000)
#define U_PNIP__FRAMECLASS_2_18__FRAMETRAFFIC_SELECT                              (0x00000800)
#define U_PNIP__FRAMECLASS_2_18__DEFAULTTRAFFIC_SELECT                            (0x00000400)
#define U_PNIP__FRAMECLASS_2_18__DEFAULTTRAFFIC_CLASS                             (0x00000380)
#define U_PNIP__FRAMECLASS_2_18__USETYPE_SELECT                                   (0x00000040)
#define U_PNIP__FRAMECLASS_2_18__USETYPE                                          (0x00000038)
#define U_PNIP__FRAMECLASS_2_18__FRAMEHANDLING_SELECT                             (0x00000004)
#define U_PNIP__FRAMECLASS_2_18__FRAMEHANDLING                                    (0x00000003)
#define U_PNIP__FRAMECLASS_2_18_RESET__VALUE                     0x10000
#define U_PNIP__FRAMECLASS_2_19                                                   (0x10610248 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_2_19__PM_SELECT                                        (0x80000000)
#define U_PNIP__FRAMECLASS_2_19__PM                                               (0x40000000)
#define U_PNIP__FRAMECLASS_2_19__RESOURCETYPE_SELECT                              (0x20000000)
#define U_PNIP__FRAMECLASS_2_19__RESOURCETYPE                                     (0x0F000000)
#define U_PNIP__FRAMECLASS_2_19__APITYPE_SELECT                                   (0x00800000)
#define U_PNIP__FRAMECLASS_2_19__APITYPE                                          (0x003C0000)
#define U_PNIP__FRAMECLASS_2_19__BITTYPE_SELECT                                   (0x00030000)
#define U_PNIP__FRAMECLASS_2_19__LEARNING_DISABLE                                 (0x00008000)
#define U_PNIP__FRAMECLASS_2_19__BRIDGEDELAY_ENABLE                               (0x00004000)
#define U_PNIP__FRAMECLASS_2_19__FRAGMENTATION                                    (0x00002000)
#define U_PNIP__FRAMECLASS_2_19__REDUNDANCY                                       (0x00001000)
#define U_PNIP__FRAMECLASS_2_19__FRAMETRAFFIC_SELECT                              (0x00000800)
#define U_PNIP__FRAMECLASS_2_19__DEFAULTTRAFFIC_SELECT                            (0x00000400)
#define U_PNIP__FRAMECLASS_2_19__DEFAULTTRAFFIC_CLASS                             (0x00000380)
#define U_PNIP__FRAMECLASS_2_19__USETYPE_SELECT                                   (0x00000040)
#define U_PNIP__FRAMECLASS_2_19__USETYPE                                          (0x00000038)
#define U_PNIP__FRAMECLASS_2_19__FRAMEHANDLING_SELECT                             (0x00000004)
#define U_PNIP__FRAMECLASS_2_19__FRAMEHANDLING                                    (0x00000003)
#define U_PNIP__FRAMECLASS_2_19_RESET__VALUE                     0x10000
#define U_PNIP__FRAMECLASS_2_20                                                   (0x1061024C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_2_20__PM_SELECT                                        (0x80000000)
#define U_PNIP__FRAMECLASS_2_20__PM                                               (0x40000000)
#define U_PNIP__FRAMECLASS_2_20__RESOURCETYPE_SELECT                              (0x20000000)
#define U_PNIP__FRAMECLASS_2_20__RESOURCETYPE                                     (0x0F000000)
#define U_PNIP__FRAMECLASS_2_20__APITYPE_SELECT                                   (0x00800000)
#define U_PNIP__FRAMECLASS_2_20__APITYPE                                          (0x003C0000)
#define U_PNIP__FRAMECLASS_2_20__BITTYPE_SELECT                                   (0x00030000)
#define U_PNIP__FRAMECLASS_2_20__LEARNING_DISABLE                                 (0x00008000)
#define U_PNIP__FRAMECLASS_2_20__BRIDGEDELAY_ENABLE                               (0x00004000)
#define U_PNIP__FRAMECLASS_2_20__FRAGMENTATION                                    (0x00002000)
#define U_PNIP__FRAMECLASS_2_20__REDUNDANCY                                       (0x00001000)
#define U_PNIP__FRAMECLASS_2_20__FRAMETRAFFIC_SELECT                              (0x00000800)
#define U_PNIP__FRAMECLASS_2_20__DEFAULTTRAFFIC_SELECT                            (0x00000400)
#define U_PNIP__FRAMECLASS_2_20__DEFAULTTRAFFIC_CLASS                             (0x00000380)
#define U_PNIP__FRAMECLASS_2_20__USETYPE_SELECT                                   (0x00000040)
#define U_PNIP__FRAMECLASS_2_20__USETYPE                                          (0x00000038)
#define U_PNIP__FRAMECLASS_2_20__FRAMEHANDLING_SELECT                             (0x00000004)
#define U_PNIP__FRAMECLASS_2_20__FRAMEHANDLING                                    (0x00000003)
#define U_PNIP__FRAMECLASS_2_20_RESET__VALUE                     0x10000
#define U_PNIP__FRAMECLASS_2_21                                                   (0x10610250 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_2_21__PM_SELECT                                        (0x80000000)
#define U_PNIP__FRAMECLASS_2_21__PM                                               (0x40000000)
#define U_PNIP__FRAMECLASS_2_21__RESOURCETYPE_SELECT                              (0x20000000)
#define U_PNIP__FRAMECLASS_2_21__RESOURCETYPE                                     (0x0F000000)
#define U_PNIP__FRAMECLASS_2_21__APITYPE_SELECT                                   (0x00800000)
#define U_PNIP__FRAMECLASS_2_21__APITYPE                                          (0x003C0000)
#define U_PNIP__FRAMECLASS_2_21__BITTYPE_SELECT                                   (0x00030000)
#define U_PNIP__FRAMECLASS_2_21__LEARNING_DISABLE                                 (0x00008000)
#define U_PNIP__FRAMECLASS_2_21__BRIDGEDELAY_ENABLE                               (0x00004000)
#define U_PNIP__FRAMECLASS_2_21__FRAGMENTATION                                    (0x00002000)
#define U_PNIP__FRAMECLASS_2_21__REDUNDANCY                                       (0x00001000)
#define U_PNIP__FRAMECLASS_2_21__FRAMETRAFFIC_SELECT                              (0x00000800)
#define U_PNIP__FRAMECLASS_2_21__DEFAULTTRAFFIC_SELECT                            (0x00000400)
#define U_PNIP__FRAMECLASS_2_21__DEFAULTTRAFFIC_CLASS                             (0x00000380)
#define U_PNIP__FRAMECLASS_2_21__USETYPE_SELECT                                   (0x00000040)
#define U_PNIP__FRAMECLASS_2_21__USETYPE                                          (0x00000038)
#define U_PNIP__FRAMECLASS_2_21__FRAMEHANDLING_SELECT                             (0x00000004)
#define U_PNIP__FRAMECLASS_2_21__FRAMEHANDLING                                    (0x00000003)
#define U_PNIP__FRAMECLASS_2_21_RESET__VALUE                     0x10000
#define U_PNIP__FRAMECLASS_2_22                                                   (0x10610254 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_2_22__PM_SELECT                                        (0x80000000)
#define U_PNIP__FRAMECLASS_2_22__PM                                               (0x40000000)
#define U_PNIP__FRAMECLASS_2_22__RESOURCETYPE_SELECT                              (0x20000000)
#define U_PNIP__FRAMECLASS_2_22__RESOURCETYPE                                     (0x0F000000)
#define U_PNIP__FRAMECLASS_2_22__APITYPE_SELECT                                   (0x00800000)
#define U_PNIP__FRAMECLASS_2_22__APITYPE                                          (0x003C0000)
#define U_PNIP__FRAMECLASS_2_22__BITTYPE_SELECT                                   (0x00030000)
#define U_PNIP__FRAMECLASS_2_22__LEARNING_DISABLE                                 (0x00008000)
#define U_PNIP__FRAMECLASS_2_22__BRIDGEDELAY_ENABLE                               (0x00004000)
#define U_PNIP__FRAMECLASS_2_22__FRAGMENTATION                                    (0x00002000)
#define U_PNIP__FRAMECLASS_2_22__REDUNDANCY                                       (0x00001000)
#define U_PNIP__FRAMECLASS_2_22__FRAMETRAFFIC_SELECT                              (0x00000800)
#define U_PNIP__FRAMECLASS_2_22__DEFAULTTRAFFIC_SELECT                            (0x00000400)
#define U_PNIP__FRAMECLASS_2_22__DEFAULTTRAFFIC_CLASS                             (0x00000380)
#define U_PNIP__FRAMECLASS_2_22__USETYPE_SELECT                                   (0x00000040)
#define U_PNIP__FRAMECLASS_2_22__USETYPE                                          (0x00000038)
#define U_PNIP__FRAMECLASS_2_22__FRAMEHANDLING_SELECT                             (0x00000004)
#define U_PNIP__FRAMECLASS_2_22__FRAMEHANDLING                                    (0x00000003)
#define U_PNIP__FRAMECLASS_2_22_RESET__VALUE                     0x10000
#define U_PNIP__FRAMECLASS_2_23                                                   (0x10610258 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_2_23__PM_SELECT                                        (0x80000000)
#define U_PNIP__FRAMECLASS_2_23__PM                                               (0x40000000)
#define U_PNIP__FRAMECLASS_2_23__RESOURCETYPE_SELECT                              (0x20000000)
#define U_PNIP__FRAMECLASS_2_23__RESOURCETYPE                                     (0x0F000000)
#define U_PNIP__FRAMECLASS_2_23__APITYPE_SELECT                                   (0x00800000)
#define U_PNIP__FRAMECLASS_2_23__APITYPE                                          (0x003C0000)
#define U_PNIP__FRAMECLASS_2_23__BITTYPE_SELECT                                   (0x00030000)
#define U_PNIP__FRAMECLASS_2_23__LEARNING_DISABLE                                 (0x00008000)
#define U_PNIP__FRAMECLASS_2_23__BRIDGEDELAY_ENABLE                               (0x00004000)
#define U_PNIP__FRAMECLASS_2_23__FRAGMENTATION                                    (0x00002000)
#define U_PNIP__FRAMECLASS_2_23__REDUNDANCY                                       (0x00001000)
#define U_PNIP__FRAMECLASS_2_23__FRAMETRAFFIC_SELECT                              (0x00000800)
#define U_PNIP__FRAMECLASS_2_23__DEFAULTTRAFFIC_SELECT                            (0x00000400)
#define U_PNIP__FRAMECLASS_2_23__DEFAULTTRAFFIC_CLASS                             (0x00000380)
#define U_PNIP__FRAMECLASS_2_23__USETYPE_SELECT                                   (0x00000040)
#define U_PNIP__FRAMECLASS_2_23__USETYPE                                          (0x00000038)
#define U_PNIP__FRAMECLASS_2_23__FRAMEHANDLING_SELECT                             (0x00000004)
#define U_PNIP__FRAMECLASS_2_23__FRAMEHANDLING                                    (0x00000003)
#define U_PNIP__FRAMECLASS_2_23_RESET__VALUE                     0x10000
#define U_PNIP__FRAMECLASS_2_24                                                   (0x1061025C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_2_24__PM_SELECT                                        (0x80000000)
#define U_PNIP__FRAMECLASS_2_24__PM                                               (0x40000000)
#define U_PNIP__FRAMECLASS_2_24__RESOURCETYPE_SELECT                              (0x20000000)
#define U_PNIP__FRAMECLASS_2_24__RESOURCETYPE                                     (0x0F000000)
#define U_PNIP__FRAMECLASS_2_24__APITYPE_SELECT                                   (0x00800000)
#define U_PNIP__FRAMECLASS_2_24__APITYPE                                          (0x003C0000)
#define U_PNIP__FRAMECLASS_2_24__BITTYPE_SELECT                                   (0x00030000)
#define U_PNIP__FRAMECLASS_2_24__LEARNING_DISABLE                                 (0x00008000)
#define U_PNIP__FRAMECLASS_2_24__BRIDGEDELAY_ENABLE                               (0x00004000)
#define U_PNIP__FRAMECLASS_2_24__FRAGMENTATION                                    (0x00002000)
#define U_PNIP__FRAMECLASS_2_24__REDUNDANCY                                       (0x00001000)
#define U_PNIP__FRAMECLASS_2_24__FRAMETRAFFIC_SELECT                              (0x00000800)
#define U_PNIP__FRAMECLASS_2_24__DEFAULTTRAFFIC_SELECT                            (0x00000400)
#define U_PNIP__FRAMECLASS_2_24__DEFAULTTRAFFIC_CLASS                             (0x00000380)
#define U_PNIP__FRAMECLASS_2_24__USETYPE_SELECT                                   (0x00000040)
#define U_PNIP__FRAMECLASS_2_24__USETYPE                                          (0x00000038)
#define U_PNIP__FRAMECLASS_2_24__FRAMEHANDLING_SELECT                             (0x00000004)
#define U_PNIP__FRAMECLASS_2_24__FRAMEHANDLING                                    (0x00000003)
#define U_PNIP__FRAMECLASS_2_24_RESET__VALUE                     0x10000
#define U_PNIP__FRAMECLASS_DEFAULT                                                (0x10610300 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASS_DEFAULT__PM_SELECT                                     (0x80000000)
#define U_PNIP__FRAMECLASS_DEFAULT__PM                                            (0x40000000)
#define U_PNIP__FRAMECLASS_DEFAULT__RESOURCETYPE_SELECT                           (0x20000000)
#define U_PNIP__FRAMECLASS_DEFAULT__RESOURCETYPE                                  (0x0F000000)
#define U_PNIP__FRAMECLASS_DEFAULT__APITYPE_SELECT                                (0x00800000)
#define U_PNIP__FRAMECLASS_DEFAULT__APITYPE                                       (0x003C0000)
#define U_PNIP__FRAMECLASS_DEFAULT__BITTYPE_SELECT                                (0x00030000)
#define U_PNIP__FRAMECLASS_DEFAULT__LEARNING_DISABLE                              (0x00008000)
#define U_PNIP__FRAMECLASS_DEFAULT__BRIDGEDELAY_ENABLE                            (0x00004000)
#define U_PNIP__FRAMECLASS_DEFAULT__FRAGMENTATION                                 (0x00002000)
#define U_PNIP__FRAMECLASS_DEFAULT__REDUNDANCY                                    (0x00001000)
#define U_PNIP__FRAMECLASS_DEFAULT__FRAMETRAFFIC_SELECT                           (0x00000800)
#define U_PNIP__FRAMECLASS_DEFAULT__DEFAULTTRAFFIC_SELECT                         (0x00000400)
#define U_PNIP__FRAMECLASS_DEFAULT__DEFAULTTRAFFIC_CLASS                          (0x00000380)
#define U_PNIP__FRAMECLASS_DEFAULT__USETYPE_SELECT                                (0x00000040)
#define U_PNIP__FRAMECLASS_DEFAULT__USETYPE                                       (0x00000038)
#define U_PNIP__FRAMECLASS_DEFAULT__FRAMEHANDLING_SELECT                          (0x00000004)
#define U_PNIP__FRAMECLASS_DEFAULT__FRAMEHANDLING                                 (0x00000003)
#define U_PNIP__FRAMECLASS_DEFAULT_RESET__VALUE                  0xA0832444
#define U_PNIP__FRAMECLASSOVERLOAD_1                                              (0x10610380 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASSOVERLOAD_1__FRAMEID_HIGH                                (0xFFFF0000)
#define U_PNIP__FRAMECLASSOVERLOAD_1__FRAMEID_LOW                                 (0x0000FFFF)
#define U_PNIP__FRAMECLASSOVERLOAD_1_RESET__VALUE                0x0
#define U_PNIP__FRAMECLASSOVERLOAD_2                                              (0x10610384 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECLASSOVERLOAD_2__RESOURCETYPE_OVERLOAD                       (0x00002000)
#define U_PNIP__FRAMECLASSOVERLOAD_2__RESOURCETYPE                                (0x00000F00)
#define U_PNIP__FRAMECLASSOVERLOAD_2__APITYPE_OVERLOAD                            (0x00000020)
#define U_PNIP__FRAMECLASSOVERLOAD_2__APITYPE                                     (0x0000000F)
#define U_PNIP__FRAMECLASSOVERLOAD_2_RESET__VALUE                0x0
#define U_PNIP__IEEE_PRIORITYMAPPING_A0                                           (0x10610400 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IEEE_PRIORITYMAPPING_A0__IEEE_TRAFFIC_CLASS_7_PRIORITY            (0xF0000000)
#define U_PNIP__IEEE_PRIORITYMAPPING_A0__IEEE_TRAFFIC_CLASS_6_PRIORITY            (0x0F000000)
#define U_PNIP__IEEE_PRIORITYMAPPING_A0__IEEE_TRAFFIC_CLASS_5_PRIORITY            (0x00F00000)
#define U_PNIP__IEEE_PRIORITYMAPPING_A0__IEEE_TRAFFIC_CLASS_4_PRIORITY            (0x000F0000)
#define U_PNIP__IEEE_PRIORITYMAPPING_A0__IEEE_TRAFFIC_CLASS_3_PRIORITY            (0x0000F000)
#define U_PNIP__IEEE_PRIORITYMAPPING_A0__IEEE_TRAFFIC_CLASS_2_PRIORITY            (0x00000F00)
#define U_PNIP__IEEE_PRIORITYMAPPING_A0__IEEE_TRAFFIC_CLASS_1_PRIORITY            (0x000000F0)
#define U_PNIP__IEEE_PRIORITYMAPPING_A0__IEEE_TRAFFIC_CLASS_0_PRIORITY            (0x0000000F)
#define U_PNIP__IEEE_PRIORITYMAPPING_A0_RESET__VALUE             0x95321001
#define U_PNIP__IEEE_PRIORITYMAPPING_A1                                           (0x10610404 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IEEE_PRIORITYMAPPING_A1__IEEE_TRAFFIC_CLASS_UNTAGGED_PRIORITY     (0x0000000F)
#define U_PNIP__IEEE_PRIORITYMAPPING_A1_RESET__VALUE             0x1
#define U_PNIP__IEEE_PRIORITYMAPPING_B0                                           (0x10610408 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IEEE_PRIORITYMAPPING_B0__IEEE_TRAFFIC_CLASS_7_PRIORITY            (0xF0000000)
#define U_PNIP__IEEE_PRIORITYMAPPING_B0__IEEE_TRAFFIC_CLASS_6_PRIORITY            (0x0F000000)
#define U_PNIP__IEEE_PRIORITYMAPPING_B0__IEEE_TRAFFIC_CLASS_5_PRIORITY            (0x00F00000)
#define U_PNIP__IEEE_PRIORITYMAPPING_B0__IEEE_TRAFFIC_CLASS_4_PRIORITY            (0x000F0000)
#define U_PNIP__IEEE_PRIORITYMAPPING_B0__IEEE_TRAFFIC_CLASS_3_PRIORITY            (0x0000F000)
#define U_PNIP__IEEE_PRIORITYMAPPING_B0__IEEE_TRAFFIC_CLASS_2_PRIORITY            (0x00000F00)
#define U_PNIP__IEEE_PRIORITYMAPPING_B0__IEEE_TRAFFIC_CLASS_1_PRIORITY            (0x000000F0)
#define U_PNIP__IEEE_PRIORITYMAPPING_B0__IEEE_TRAFFIC_CLASS_0_PRIORITY            (0x0000000F)
#define U_PNIP__IEEE_PRIORITYMAPPING_B0_RESET__VALUE             0x0
#define U_PNIP__IEEE_PRIORITYMAPPING_B1                                           (0x1061040C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IEEE_PRIORITYMAPPING_B1__IEEE_TRAFFIC_CLASS_UNTAGGED_PRIORITY     (0x0000000F)
#define U_PNIP__IEEE_PRIORITYMAPPING_B1_RESET__VALUE             0x0
#define U_PNIP__RT_A_PRIORITYMAPPING                                              (0x10610410 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RT_A_PRIORITYMAPPING__RT_TRAFFIC_CLASS_6_3                        (0x0000F000)
#define U_PNIP__RT_A_PRIORITYMAPPING__RT_TRAFFIC_CLASS_6_2                        (0x00000F00)
#define U_PNIP__RT_A_PRIORITYMAPPING__RTA_TRAFFIC_CLASS_6_1                       (0x000000F0)
#define U_PNIP__RT_A_PRIORITYMAPPING__RTA_TRAFFIC_CLASS_5_1                       (0x0000000F)
#define U_PNIP__RT_A_PRIORITYMAPPING_RESET__VALUE                0x8764
#define U_PNIP__MNGM_PRIORITYMAPPING                                              (0x10610414 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MNGM_PRIORITYMAPPING__TRAFFIC_CLASS_MNGM_HIGH                     (0x000000F0)
#define U_PNIP__MNGM_PRIORITYMAPPING__TRAFFIC_CLASS_MNGM_LOW                      (0x0000000F)
#define U_PNIP__MNGM_PRIORITYMAPPING_RESET__VALUE                0xBA
#define U_PNIP__TRAFFIC_MODE                                                      (0x10610418 + PNIP_DEV_TOP_BASE)
#define U_PNIP__TRAFFIC_MODE__TRAFFICMODE_P4                                      (0x00000C00)
#define U_PNIP__TRAFFIC_MODE__TRAFFICMODE_P3                                      (0x00000300)
#define U_PNIP__TRAFFIC_MODE__TRAFFICMODE_P2                                      (0x000000C0)
#define U_PNIP__TRAFFIC_MODE__TRAFFICMODE_P1                                      (0x00000030)
#define U_PNIP__TRAFFIC_MODE__TRAFFICMODE_IFB                                     (0x0000000C)
#define U_PNIP__TRAFFIC_MODE__TRAFFICMODE_IFA                                     (0x00000003)
#define U_PNIP__TRAFFIC_MODE_RESET__VALUE                        0x0
#define U_PNIP__UC_CONTROL                                                        (0x10611000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__UC_CONTROL__UC_RT_A_PRIORITY_ENABLE                               (0x1F000000)
#define U_PNIP__UC_CONTROL__UC_ENABLE_SA_INGRESS                                  (0x001E0000)
#define U_PNIP__UC_CONTROL__UC_LEARNING_ENABLE_R                                  (0x0000FF00)
#define U_PNIP__UC_CONTROL__UC_LEARNING_ENABLE                                    (0x0000001F)
#define U_PNIP__UC_CONTROL_RESET__VALUE                          0x1F00551E
#define U_PNIP__UC_DEFAULT_A                                                      (0x10611004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__UC_DEFAULT_A__TXPORTS                                             (0x0000001F)
#define U_PNIP__UC_DEFAULT_A_RESET__VALUE                        0x1E
#define U_PNIP__UC_DEFAULT_B                                                      (0x10611008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__UC_DEFAULT_B__TXPORTS                                             (0x0000001F)
#define U_PNIP__UC_DEFAULT_B_RESET__VALUE                        0x1E
#define U_PNIP__UC_LFSR_MASK                                                      (0x1061100C + PNIP_DEV_TOP_BASE)
#define U_PNIP__UC_LFSR_MASK__LFSR_MASK                                           (0x000001FF)
#define U_PNIP__UC_LFSR_MASK_RESET__VALUE                        0xAA
#define U_PNIP__UC_TABLE_BASE_A                                                   (0x10611010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__UC_TABLE_BASE_A__BASE_UC_TABLE                                    (0x001FFFFF)
#define U_PNIP__UC_TABLE_BASE_A_RESET__VALUE                     0x100000
#define U_PNIP__UC_LFSR_RANGE_A                                                   (0x10611014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__UC_LFSR_RANGE_A__LFSRRANGE_A                                      (0x000001FF)
#define U_PNIP__UC_LFSR_RANGE_A_RESET__VALUE                     0x3F
#define U_PNIP__UC_LEARNINGFAULT_A                                                (0x10611018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__UC_LEARNINGFAULT_A__LEARNINGFAULT_A                               (0xFFFFFFFF)
#define U_PNIP__UC_LEARNINGFAULT_A_RESET__VALUE                  0x0
#define U_PNIP__UC_TABLE_BASE_B                                                   (0x1061101C + PNIP_DEV_TOP_BASE)
#define U_PNIP__UC_TABLE_BASE_B__BASE_UC_TABLE_B                                  (0x001FFFFF)
#define U_PNIP__UC_TABLE_BASE_B_RESET__VALUE                     0x11FF80
#define U_PNIP__UC_LFSR_RANGE_B                                                   (0x10611020 + PNIP_DEV_TOP_BASE)
#define U_PNIP__UC_LFSR_RANGE_B__LFSRRANGE_B                                      (0x000001FF)
#define U_PNIP__UC_LFSR_RANGE_B_RESET__VALUE                     0x0
#define U_PNIP__UC_LEARNINGFAULT_B                                                (0x10611024 + PNIP_DEV_TOP_BASE)
#define U_PNIP__UC_LEARNINGFAULT_B__LEARNINGFAULT_B                               (0xFFFFFFFF)
#define U_PNIP__UC_LEARNINGFAULT_B_RESET__VALUE                  0x0
#define U_PNIP__UC_LEARNINGMODE                                                   (0x10611028 + PNIP_DEV_TOP_BASE)
#define U_PNIP__UC_LEARNINGMODE__LEARNINGMODE_B                                   (0x00000002)
#define U_PNIP__UC_LEARNINGMODE__LEARNINGMODE_A                                   (0x00000001)
#define U_PNIP__UC_LEARNINGMODE_RESET__VALUE                     0x0
#define U_PNIP__MC_CONTROL                                                        (0x10612000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MC_CONTROL__MC_RT_A_PRIORITY_ENABLE                               (0x1F000000)
#define U_PNIP__MC_CONTROL__MC_ENABLE_SA_INGRESS                                  (0x001E0000)
#define U_PNIP__MC_CONTROL__MC_LEARNING_ENABLE_R                                  (0x0000FF00)
#define U_PNIP__MC_CONTROL__MC_LEARNING_ENABLE                                    (0x0000001F)
#define U_PNIP__MC_CONTROL_RESET__VALUE                          0x1F00551E
#define U_PNIP__MC_TABLE_BASE                                                     (0x10612004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MC_TABLE_BASE__BASE_MC_TABLE                                      (0x001FFFFF)
#define U_PNIP__MC_TABLE_BASE_RESET__VALUE                       0x11FFF0
#define U_PNIP__MC_TABLE_RANGE                                                    (0x10612008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MC_TABLE_RANGE__MC_TABLE_RANGE                                    (0x000003FF)
#define U_PNIP__MC_TABLE_RANGE_RESET__VALUE                      0x0
#define U_PNIP__MC_DEFAULT_API                                                    (0x1061200C + PNIP_DEV_TOP_BASE)
#define U_PNIP__MC_DEFAULT_API__TXPORTS                                           (0x0000001F)
#define U_PNIP__MC_DEFAULT_API_RESET__VALUE                      0x1E
#define U_PNIP__MC_DEFAULT_P1                                                     (0x10612010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MC_DEFAULT_P1__TXPORTS                                            (0x0000001F)
#define U_PNIP__MC_DEFAULT_P1_RESET__VALUE                       0x1E
#define U_PNIP__MC_DEFAULT_P2                                                     (0x10612014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MC_DEFAULT_P2__TXPORTS                                            (0x0000001F)
#define U_PNIP__MC_DEFAULT_P2_RESET__VALUE                       0x1E
#define U_PNIP__MC_DEFAULT_P3                                                     (0x10612018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MC_DEFAULT_P3__TXPORTS                                            (0x0000001F)
#define U_PNIP__MC_DEFAULT_P3_RESET__VALUE                       0x1E
#define U_PNIP__MC_DEFAULT_P4                                                     (0x1061201C + PNIP_DEV_TOP_BASE)
#define U_PNIP__MC_DEFAULT_P4__TXPORTS                                            (0x0000001F)
#define U_PNIP__MC_DEFAULT_P4_RESET__VALUE                       0x1E
#define U_PNIP__IRT_TABLE_BASE1_A                                                 (0x10613000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRT_TABLE_BASE1_A__BASE_IRT_TABLE                                 (0x001FFFFF)
#define U_PNIP__IRT_TABLE_BASE1_A_RESET__VALUE                   0x11FFFF
#define U_PNIP__IRT_TABLE_BASE2_A                                                 (0x10613004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRT_TABLE_BASE2_A__BASE_IRT_TABLE                                 (0x001FFFFF)
#define U_PNIP__IRT_TABLE_BASE2_A_RESET__VALUE                   0x11FFFF
#define U_PNIP__IRT_FRAMEID_RANGE1_A                                              (0x10613008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRT_FRAMEID_RANGE1_A__FRAMEID_HIGH                                (0xFFFF0000)
#define U_PNIP__IRT_FRAMEID_RANGE1_A__FRAMEID_LOW                                 (0x0000FFFF)
#define U_PNIP__IRT_FRAMEID_RANGE1_A_RESET__VALUE                0x810080
#define U_PNIP__IRT_FRAMEID_RANGE2_A                                              (0x1061300C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRT_FRAMEID_RANGE2_A__FRAMEID_HIGH                                (0xFFFF0000)
#define U_PNIP__IRT_FRAMEID_RANGE2_A__FRAMEID_LOW                                 (0x0000FFFF)
#define U_PNIP__IRT_FRAMEID_RANGE2_A_RESET__VALUE                0xFFF0100
#define U_PNIP__IRT_TABLE_BASE1_B                                                 (0x10613010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRT_TABLE_BASE1_B__BASE_IRT_TABLE                                 (0x001FFFFF)
#define U_PNIP__IRT_TABLE_BASE1_B_RESET__VALUE                   0x11FFFF
#define U_PNIP__IRT_TABLE_BASE2_B                                                 (0x10613014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRT_TABLE_BASE2_B__BASE_IRT_TABLE                                 (0x001FFFFF)
#define U_PNIP__IRT_TABLE_BASE2_B_RESET__VALUE                   0x11FFFF
#define U_PNIP__IRT_FRAMEID_RANGE1_B                                              (0x10613018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRT_FRAMEID_RANGE1_B__FRAMEID_HIGH                                (0xFFFF0000)
#define U_PNIP__IRT_FRAMEID_RANGE1_B__FRAMEID_LOW                                 (0x0000FFFF)
#define U_PNIP__IRT_FRAMEID_RANGE1_B_RESET__VALUE                0x810080
#define U_PNIP__IRT_FRAMEID_RANGE2_B                                              (0x1061301C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRT_FRAMEID_RANGE2_B__FRAMEID_HIGH                                (0xFFFF0000)
#define U_PNIP__IRT_FRAMEID_RANGE2_B__FRAMEID_LOW                                 (0x0000FFFF)
#define U_PNIP__IRT_FRAMEID_RANGE2_B_RESET__VALUE                0xFFF0100
#define U_PNIP__IRT_CONTROL                                                       (0x10613020 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRT_CONTROL__IRT_LEARNING_ENABLE                                  (0x0000FFC0)
#define U_PNIP__IRT_CONTROL__TOPFW_MODE_B                                         (0x00000002)
#define U_PNIP__IRT_CONTROL__TOPFW_MODE_A                                         (0x00000001)
#define U_PNIP__IRT_CONTROL_RESET__VALUE                         0x5500
#define U_PNIP__FDBCOMMAND_IF_CONTROL                                             (0x10614000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FDBCOMMAND_IF_CONTROL__COMMANDVALUE                               (0x00001F00)
#define U_PNIP__FDBCOMMAND_IF_CONTROL__USER_ID                                    (0x000000E0)
#define U_PNIP__FDBCOMMAND_IF_CONTROL__CONFREQUEST                                (0x00000010)
#define U_PNIP__FDBCOMMAND_IF_CONTROL__IF_SELECT                                  (0x00000008)
#define U_PNIP__FDBCOMMAND_IF_CONTROL__F_CODE                                     (0x00000007)
#define U_PNIP__FDBCOMMAND_IF_CONTROL_RESET__VALUE               0x0
#define U_PNIP__FDBCOMMAND_IF_STATUS                                              (0x10614004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FDBCOMMAND_IF_STATUS__RETURNVALUE                                 (0x1FFFFF00)
#define U_PNIP__FDBCOMMAND_IF_STATUS__USER_ID                                     (0x000000E0)
#define U_PNIP__FDBCOMMAND_IF_STATUS__CONFRESPONSE                                (0x00000010)
#define U_PNIP__FDBCOMMAND_IF_STATUS__CONFERROR                                   (0x00000008)
#define U_PNIP__FDBCOMMAND_IF_STATUS__F_CODE                                      (0x00000007)
#define U_PNIP__FDBCOMMAND_IF_STATUS_RESET__VALUE                0x0
#define U_PNIP__FDBPARAMSTRUCT_BASE                                               (0x10614008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FDBPARAMSTRUCT_BASE__BASE_PARAMSTRUCT                             (0x001FFFFF)
#define U_PNIP__FDBPARAMSTRUCT_BASE_RESET__VALUE                 0x11FFF0
#define U_PNIP__UC_AGING_A                                                        (0x1061400C + PNIP_DEV_TOP_BASE)
#define U_PNIP__UC_AGING_A__OVERLOAD_AGING                                        (0x80000000)
#define U_PNIP__UC_AGING_A__AGING_DELAY                                           (0x0000FFFF)
#define U_PNIP__UC_AGING_A_RESET__VALUE                          0x0
#define U_PNIP__UC_AGING_B                                                        (0x10614010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__UC_AGING_B__OVERLOAD_AGING                                        (0x80000000)
#define U_PNIP__UC_AGING_B__AGING_DELAY                                           (0x0000FFFF)
#define U_PNIP__UC_AGING_B_RESET__VALUE                          0x0
#define U_PNIP__REDUNDANCE_CONTROL                                                (0x10614800 + PNIP_DEV_TOP_BASE)
#define U_PNIP__REDUNDANCE_CONTROL__REDUNDANCEPORT_R2                             (0x00000F00)
#define U_PNIP__REDUNDANCE_CONTROL__REDUNDANCEPORT_R1                             (0x0000000F)
#define U_PNIP__REDUNDANCE_CONTROL_RESET__VALUE                  0x0
#define U_PNIP__OUTBOUND_APDU                                                     (0x10615000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__OUTBOUND_APDU__TRANSFERSTATUS                                     (0x0000FF00)
#define U_PNIP__OUTBOUND_APDU__DATASTATUS                                         (0x000000FF)
#define U_PNIP__OUTBOUND_APDU_RESET__VALUE                       0xB5
#define U_PNIP__OUTBOUNDPACK_0                                                    (0x10615100 + PNIP_DEV_TOP_BASE)
#define U_PNIP__OUTBOUNDPACK_0__OUTBOUND_VALID                                    (0x80000000)
#define U_PNIP__OUTBOUNDPACK_0__OUTBOUND_INTERFACE                                (0x40000000)
#define U_PNIP__OUTBOUNDPACK_0__FID_REDUNDANT                                     (0x04000000)
#define U_PNIP__OUTBOUNDPACK_0__OUTBOUND_TXPORT                                   (0x03000000)
#define U_PNIP__OUTBOUNDPACK_0__OUTBOUND_WO_DCS                                   (0x00800000)
#define U_PNIP__OUTBOUNDPACK_0__OUTBOUND_POSNR                                    (0x007F0000)
#define U_PNIP__OUTBOUNDPACK_0__OUTBOUND_FRAMEID                                  (0x0000FFFF)
#define U_PNIP__OUTBOUNDPACK_0_RESET__VALUE                      0x0
#define U_PNIP__OUTBOUNDPACK_1                                                    (0x10615104 + PNIP_DEV_TOP_BASE)
#define U_PNIP__OUTBOUNDPACK_1__OUTBOUND_VALID                                    (0x80000000)
#define U_PNIP__OUTBOUNDPACK_1__OUTBOUND_INTERFACE                                (0x40000000)
#define U_PNIP__OUTBOUNDPACK_1__FID_REDUNDANT                                     (0x04000000)
#define U_PNIP__OUTBOUNDPACK_1__OUTBOUND_TXPORT                                   (0x03000000)
#define U_PNIP__OUTBOUNDPACK_1__OUTBOUND_WO_DCS                                   (0x00800000)
#define U_PNIP__OUTBOUNDPACK_1__OUTBOUND_POSNR                                    (0x007F0000)
#define U_PNIP__OUTBOUNDPACK_1__OUTBOUND_FRAMEID                                  (0x0000FFFF)
#define U_PNIP__OUTBOUNDPACK_1_RESET__VALUE                      0x0
#define U_PNIP__OUTBOUNDPACK_2                                                    (0x10615108 + PNIP_DEV_TOP_BASE)
#define U_PNIP__OUTBOUNDPACK_2__OUTBOUND_VALID                                    (0x80000000)
#define U_PNIP__OUTBOUNDPACK_2__OUTBOUND_INTERFACE                                (0x40000000)
#define U_PNIP__OUTBOUNDPACK_2__FID_REDUNDANT                                     (0x04000000)
#define U_PNIP__OUTBOUNDPACK_2__OUTBOUND_TXPORT                                   (0x03000000)
#define U_PNIP__OUTBOUNDPACK_2__OUTBOUND_WO_DCS                                   (0x00800000)
#define U_PNIP__OUTBOUNDPACK_2__OUTBOUND_POSNR                                    (0x007F0000)
#define U_PNIP__OUTBOUNDPACK_2__OUTBOUND_FRAMEID                                  (0x0000FFFF)
#define U_PNIP__OUTBOUNDPACK_2_RESET__VALUE                      0x0
#define U_PNIP__OUTBOUNDPACK_3                                                    (0x1061510C + PNIP_DEV_TOP_BASE)
#define U_PNIP__OUTBOUNDPACK_3__OUTBOUND_VALID                                    (0x80000000)
#define U_PNIP__OUTBOUNDPACK_3__OUTBOUND_INTERFACE                                (0x40000000)
#define U_PNIP__OUTBOUNDPACK_3__FID_REDUNDANT                                     (0x04000000)
#define U_PNIP__OUTBOUNDPACK_3__OUTBOUND_TXPORT                                   (0x03000000)
#define U_PNIP__OUTBOUNDPACK_3__OUTBOUND_WO_DCS                                   (0x00800000)
#define U_PNIP__OUTBOUNDPACK_3__OUTBOUND_POSNR                                    (0x007F0000)
#define U_PNIP__OUTBOUNDPACK_3__OUTBOUND_FRAMEID                                  (0x0000FFFF)
#define U_PNIP__OUTBOUNDPACK_3_RESET__VALUE                      0x0
#define U_PNIP__OUTBOUNDPACK_4                                                    (0x10615110 + PNIP_DEV_TOP_BASE)
#define U_PNIP__OUTBOUNDPACK_4__OUTBOUND_VALID                                    (0x80000000)
#define U_PNIP__OUTBOUNDPACK_4__OUTBOUND_INTERFACE                                (0x40000000)
#define U_PNIP__OUTBOUNDPACK_4__FID_REDUNDANT                                     (0x04000000)
#define U_PNIP__OUTBOUNDPACK_4__OUTBOUND_TXPORT                                   (0x03000000)
#define U_PNIP__OUTBOUNDPACK_4__OUTBOUND_WO_DCS                                   (0x00800000)
#define U_PNIP__OUTBOUNDPACK_4__OUTBOUND_POSNR                                    (0x007F0000)
#define U_PNIP__OUTBOUNDPACK_4__OUTBOUND_FRAMEID                                  (0x0000FFFF)
#define U_PNIP__OUTBOUNDPACK_4_RESET__VALUE                      0x0
#define U_PNIP__OUTBOUNDPACK_5                                                    (0x10615114 + PNIP_DEV_TOP_BASE)
#define U_PNIP__OUTBOUNDPACK_5__OUTBOUND_VALID                                    (0x80000000)
#define U_PNIP__OUTBOUNDPACK_5__OUTBOUND_INTERFACE                                (0x40000000)
#define U_PNIP__OUTBOUNDPACK_5__FID_REDUNDANT                                     (0x04000000)
#define U_PNIP__OUTBOUNDPACK_5__OUTBOUND_TXPORT                                   (0x03000000)
#define U_PNIP__OUTBOUNDPACK_5__OUTBOUND_WO_DCS                                   (0x00800000)
#define U_PNIP__OUTBOUNDPACK_5__OUTBOUND_POSNR                                    (0x007F0000)
#define U_PNIP__OUTBOUNDPACK_5__OUTBOUND_FRAMEID                                  (0x0000FFFF)
#define U_PNIP__OUTBOUNDPACK_5_RESET__VALUE                      0x0
#define U_PNIP__OUTBOUNDPACK_6                                                    (0x10615118 + PNIP_DEV_TOP_BASE)
#define U_PNIP__OUTBOUNDPACK_6__OUTBOUND_VALID                                    (0x80000000)
#define U_PNIP__OUTBOUNDPACK_6__OUTBOUND_INTERFACE                                (0x40000000)
#define U_PNIP__OUTBOUNDPACK_6__FID_REDUNDANT                                     (0x04000000)
#define U_PNIP__OUTBOUNDPACK_6__OUTBOUND_TXPORT                                   (0x03000000)
#define U_PNIP__OUTBOUNDPACK_6__OUTBOUND_WO_DCS                                   (0x00800000)
#define U_PNIP__OUTBOUNDPACK_6__OUTBOUND_POSNR                                    (0x007F0000)
#define U_PNIP__OUTBOUNDPACK_6__OUTBOUND_FRAMEID                                  (0x0000FFFF)
#define U_PNIP__OUTBOUNDPACK_6_RESET__VALUE                      0x0
#define U_PNIP__OUTBOUNDPACK_7                                                    (0x1061511C + PNIP_DEV_TOP_BASE)
#define U_PNIP__OUTBOUNDPACK_7__OUTBOUND_VALID                                    (0x80000000)
#define U_PNIP__OUTBOUNDPACK_7__OUTBOUND_INTERFACE                                (0x40000000)
#define U_PNIP__OUTBOUNDPACK_7__FID_REDUNDANT                                     (0x04000000)
#define U_PNIP__OUTBOUNDPACK_7__OUTBOUND_TXPORT                                   (0x03000000)
#define U_PNIP__OUTBOUNDPACK_7__OUTBOUND_WO_DCS                                   (0x00800000)
#define U_PNIP__OUTBOUNDPACK_7__OUTBOUND_POSNR                                    (0x007F0000)
#define U_PNIP__OUTBOUNDPACK_7__OUTBOUND_FRAMEID                                  (0x0000FFFF)
#define U_PNIP__OUTBOUNDPACK_7_RESET__VALUE                      0x0
#define U_PNIP__INBOUNDPACK_0                                                     (0x10615200 + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUNDPACK_0__INBOUND_VALID                                      (0x80000000)
#define U_PNIP__INBOUNDPACK_0__INBOUND_INTERFACE                                  (0x40000000)
#define U_PNIP__INBOUNDPACK_0__INBOUND_INUSE                                      (0x20000000)
#define U_PNIP__INBOUNDPACK_0__FID_REDUNDANT                                      (0x04000000)
#define U_PNIP__INBOUNDPACK_0__INBOUND_TXPORT                                     (0x03000000)
#define U_PNIP__INBOUNDPACK_0__INBOUND_FRAMEID                                    (0x0000FFFF)
#define U_PNIP__INBOUNDPACK_0_RESET__VALUE                       0x0
#define U_PNIP__INBOUNDPACK_1                                                     (0x10615204 + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUNDPACK_1__INBOUND_VALID                                      (0x80000000)
#define U_PNIP__INBOUNDPACK_1__INBOUND_INTERFACE                                  (0x40000000)
#define U_PNIP__INBOUNDPACK_1__INBOUND_INUSE                                      (0x20000000)
#define U_PNIP__INBOUNDPACK_1__FID_REDUNDANT                                      (0x04000000)
#define U_PNIP__INBOUNDPACK_1__INBOUND_TXPORT                                     (0x03000000)
#define U_PNIP__INBOUNDPACK_1__INBOUND_FRAMEID                                    (0x0000FFFF)
#define U_PNIP__INBOUNDPACK_1_RESET__VALUE                       0x0
#define U_PNIP__INBOUNDPACK_2                                                     (0x10615208 + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUNDPACK_2__INBOUND_VALID                                      (0x80000000)
#define U_PNIP__INBOUNDPACK_2__INBOUND_INTERFACE                                  (0x40000000)
#define U_PNIP__INBOUNDPACK_2__INBOUND_INUSE                                      (0x20000000)
#define U_PNIP__INBOUNDPACK_2__FID_REDUNDANT                                      (0x04000000)
#define U_PNIP__INBOUNDPACK_2__INBOUND_TXPORT                                     (0x03000000)
#define U_PNIP__INBOUNDPACK_2__INBOUND_FRAMEID                                    (0x0000FFFF)
#define U_PNIP__INBOUNDPACK_2_RESET__VALUE                       0x0
#define U_PNIP__INBOUNDPACK_3                                                     (0x1061520C + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUNDPACK_3__INBOUND_VALID                                      (0x80000000)
#define U_PNIP__INBOUNDPACK_3__INBOUND_INTERFACE                                  (0x40000000)
#define U_PNIP__INBOUNDPACK_3__INBOUND_INUSE                                      (0x20000000)
#define U_PNIP__INBOUNDPACK_3__FID_REDUNDANT                                      (0x04000000)
#define U_PNIP__INBOUNDPACK_3__INBOUND_TXPORT                                     (0x03000000)
#define U_PNIP__INBOUNDPACK_3__INBOUND_FRAMEID                                    (0x0000FFFF)
#define U_PNIP__INBOUNDPACK_3_RESET__VALUE                       0x0
#define U_PNIP__INBOUNDPACK_4                                                     (0x10615210 + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUNDPACK_4__INBOUND_VALID                                      (0x80000000)
#define U_PNIP__INBOUNDPACK_4__INBOUND_INTERFACE                                  (0x40000000)
#define U_PNIP__INBOUNDPACK_4__INBOUND_INUSE                                      (0x20000000)
#define U_PNIP__INBOUNDPACK_4__FID_REDUNDANT                                      (0x04000000)
#define U_PNIP__INBOUNDPACK_4__INBOUND_TXPORT                                     (0x03000000)
#define U_PNIP__INBOUNDPACK_4__INBOUND_FRAMEID                                    (0x0000FFFF)
#define U_PNIP__INBOUNDPACK_4_RESET__VALUE                       0x0
#define U_PNIP__INBOUNDPACK_5                                                     (0x10615214 + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUNDPACK_5__INBOUND_VALID                                      (0x80000000)
#define U_PNIP__INBOUNDPACK_5__INBOUND_INTERFACE                                  (0x40000000)
#define U_PNIP__INBOUNDPACK_5__INBOUND_INUSE                                      (0x20000000)
#define U_PNIP__INBOUNDPACK_5__FID_REDUNDANT                                      (0x04000000)
#define U_PNIP__INBOUNDPACK_5__INBOUND_TXPORT                                     (0x03000000)
#define U_PNIP__INBOUNDPACK_5__INBOUND_FRAMEID                                    (0x0000FFFF)
#define U_PNIP__INBOUNDPACK_5_RESET__VALUE                       0x0
#define U_PNIP__INBOUNDPACK_6                                                     (0x10615218 + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUNDPACK_6__INBOUND_VALID                                      (0x80000000)
#define U_PNIP__INBOUNDPACK_6__INBOUND_INTERFACE                                  (0x40000000)
#define U_PNIP__INBOUNDPACK_6__INBOUND_INUSE                                      (0x20000000)
#define U_PNIP__INBOUNDPACK_6__FID_REDUNDANT                                      (0x04000000)
#define U_PNIP__INBOUNDPACK_6__INBOUND_TXPORT                                     (0x03000000)
#define U_PNIP__INBOUNDPACK_6__INBOUND_FRAMEID                                    (0x0000FFFF)
#define U_PNIP__INBOUNDPACK_6_RESET__VALUE                       0x0
#define U_PNIP__INBOUNDPACK_7                                                     (0x1061521C + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUNDPACK_7__INBOUND_VALID                                      (0x80000000)
#define U_PNIP__INBOUNDPACK_7__INBOUND_INTERFACE                                  (0x40000000)
#define U_PNIP__INBOUNDPACK_7__INBOUND_INUSE                                      (0x20000000)
#define U_PNIP__INBOUNDPACK_7__FID_REDUNDANT                                      (0x04000000)
#define U_PNIP__INBOUNDPACK_7__INBOUND_TXPORT                                     (0x03000000)
#define U_PNIP__INBOUNDPACK_7__INBOUND_FRAMEID                                    (0x0000FFFF)
#define U_PNIP__INBOUNDPACK_7_RESET__VALUE                       0x0
#define U_PNIP__MAC_SA_LOW_IFA                                                    (0x10616000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MAC_SA_LOW_IFA__MAC_SA_31_0                                       (0xFFFFFFFF)
#define U_PNIP__MAC_SA_LOW_IFA_RESET__VALUE                      0x0
#define U_PNIP__MAC_SA_HIGH_IFA                                                   (0x10616004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MAC_SA_HIGH_IFA__DISABLE_APIFILTER                                (0x80000000)
#define U_PNIP__MAC_SA_HIGH_IFA__MAC_SA_47_32                                     (0x0000FFFF)
#define U_PNIP__MAC_SA_HIGH_IFA_RESET__VALUE                     0x0
#define U_PNIP__MAC_SA_LOW_IFB                                                    (0x10616008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MAC_SA_LOW_IFB__MAC_SA_31_0                                       (0xFFFFFFFF)
#define U_PNIP__MAC_SA_LOW_IFB_RESET__VALUE                      0x0
#define U_PNIP__MAC_SA_HIGH_IFB                                                   (0x1061600C + PNIP_DEV_TOP_BASE)
#define U_PNIP__MAC_SA_HIGH_IFB__DISABLE_APIFILTER                                (0x80000000)
#define U_PNIP__MAC_SA_HIGH_IFB__MAC_SA_47_32                                     (0x0000FFFF)
#define U_PNIP__MAC_SA_HIGH_IFB_RESET__VALUE                     0x0
#define U_PNIP__MAC_SA_LOW_P1                                                     (0x10616010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MAC_SA_LOW_P1__MAC_SA_31_0                                        (0xFFFFFFFF)
#define U_PNIP__MAC_SA_LOW_P1_RESET__VALUE                       0x0
#define U_PNIP__MAC_SA_HIGH_P1                                                    (0x10616014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MAC_SA_HIGH_P1__DISABLE_APIFILTER                                 (0x80000000)
#define U_PNIP__MAC_SA_HIGH_P1__MAC_SA_47_32                                      (0x0000FFFF)
#define U_PNIP__MAC_SA_HIGH_P1_RESET__VALUE                      0x0
#define U_PNIP__MAC_SA_LOW_P2                                                     (0x10616018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MAC_SA_LOW_P2__MAC_SA_31_0                                        (0xFFFFFFFF)
#define U_PNIP__MAC_SA_LOW_P2_RESET__VALUE                       0x0
#define U_PNIP__MAC_SA_HIGH_P2                                                    (0x1061601C + PNIP_DEV_TOP_BASE)
#define U_PNIP__MAC_SA_HIGH_P2__DISABLE_APIFILTER                                 (0x80000000)
#define U_PNIP__MAC_SA_HIGH_P2__MAC_SA_47_32                                      (0x0000FFFF)
#define U_PNIP__MAC_SA_HIGH_P2_RESET__VALUE                      0x0
#define U_PNIP__MAC_SA_LOW_P3                                                     (0x10616020 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MAC_SA_LOW_P3__MAC_SA_31_0                                        (0xFFFFFFFF)
#define U_PNIP__MAC_SA_LOW_P3_RESET__VALUE                       0x0
#define U_PNIP__MAC_SA_HIGH_P3                                                    (0x10616024 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MAC_SA_HIGH_P3__DISABLE_APIFILTER                                 (0x80000000)
#define U_PNIP__MAC_SA_HIGH_P3__MAC_SA_47_32                                      (0x0000FFFF)
#define U_PNIP__MAC_SA_HIGH_P3_RESET__VALUE                      0x0
#define U_PNIP__MAC_SA_LOW_P4                                                     (0x10616028 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MAC_SA_LOW_P4__MAC_SA_31_0                                        (0xFFFFFFFF)
#define U_PNIP__MAC_SA_LOW_P4_RESET__VALUE                       0x0
#define U_PNIP__MAC_SA_HIGH_P4                                                    (0x1061602C + PNIP_DEV_TOP_BASE)
#define U_PNIP__MAC_SA_HIGH_P4__DISABLE_APIFILTER                                 (0x80000000)
#define U_PNIP__MAC_SA_HIGH_P4__MAC_SA_47_32                                      (0x0000FFFF)
#define U_PNIP__MAC_SA_HIGH_P4_RESET__VALUE                      0x0
#define U_PNIP__PTCP_SUBDOMAINUUID_CLOCK_A_0                                      (0x10617000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_SUBDOMAINUUID_CLOCK_A_0__PTCP_SUBDOMAINUUID_31_0             (0xFFFFFFFF)
#define U_PNIP__PTCP_SUBDOMAINUUID_CLOCK_A_0_RESET__VALUE        0x0
#define U_PNIP__PTCP_SUBDOMAINUUID_CLOCK_A_1                                      (0x10617004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_SUBDOMAINUUID_CLOCK_A_1__PTCP_SUBDOMAINUUID_63_32            (0xFFFFFFFF)
#define U_PNIP__PTCP_SUBDOMAINUUID_CLOCK_A_1_RESET__VALUE        0x0
#define U_PNIP__PTCP_SUBDOMAINUUID_CLOCK_A_2                                      (0x10617008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_SUBDOMAINUUID_CLOCK_A_2__PTCP_SUBDOMAINUUID_95_64            (0xFFFFFFFF)
#define U_PNIP__PTCP_SUBDOMAINUUID_CLOCK_A_2_RESET__VALUE        0x0
#define U_PNIP__PTCP_SUBDOMAINUUID_CLOCK_A_3                                      (0x1061700C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_SUBDOMAINUUID_CLOCK_A_3__PTCP_SUBDOMAINUUID_127_96           (0xFFFFFFFF)
#define U_PNIP__PTCP_SUBDOMAINUUID_CLOCK_A_3_RESET__VALUE        0x0
#define U_PNIP__PTCP_MASTERSA_LOW_CLOCK_A                                         (0x10617010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_MASTERSA_LOW_CLOCK_A__PTCP_MASTERSOURCEADDRESS_31_0          (0xFFFFFFFF)
#define U_PNIP__PTCP_MASTERSA_LOW_CLOCK_A_RESET__VALUE           0x0
#define U_PNIP__PTCP_MASTERSA_HIGH_CLOCK_A                                        (0x10617014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_MASTERSA_HIGH_CLOCK_A__PTCP_MASTERSOURCEADDRESS_47_32        (0x0000FFFF)
#define U_PNIP__PTCP_MASTERSA_HIGH_CLOCK_A_RESET__VALUE          0x0
#define U_PNIP__PTCP_SEQUENCEID_CLOCK_A                                           (0x10617018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_SEQUENCEID_CLOCK_A__PTCP_PORT                                (0x70000000)
#define U_PNIP__PTCP_SEQUENCEID_CLOCK_A__PTCP_SEQUENCEID                          (0x0000FFFF)
#define U_PNIP__PTCP_SEQUENCEID_CLOCK_A_RESET__VALUE             0x0
#define U_PNIP__PTCP_WD_CLOCK_A                                                   (0x1061701C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_WD_CLOCK_A__PTCP_WD_ENABLE                                   (0x80000000)
#define U_PNIP__PTCP_WD_CLOCK_A__PTCP_WD_VALUE                                    (0x0007FFFF)
#define U_PNIP__PTCP_WD_CLOCK_A_RESET__VALUE                     0x0
#define U_PNIP__PTCP_WDRELOAD_CLOCK_A                                             (0x10617020 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_WDRELOAD_CLOCK_A__PTCP_WD_RELOAD                             (0x0007FFFF)
#define U_PNIP__PTCP_WDRELOAD_CLOCK_A_RESET__VALUE               0x0
#define U_PNIP__PTCP_SUBDOMAINUUID_CLOCK_B_0                                      (0x10617100 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_SUBDOMAINUUID_CLOCK_B_0__PTCP_SUBDOMAINUUID_31_0             (0xFFFFFFFF)
#define U_PNIP__PTCP_SUBDOMAINUUID_CLOCK_B_0_RESET__VALUE        0x0
#define U_PNIP__PTCP_SUBDOMAINUUID_CLOCK_B_1                                      (0x10617104 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_SUBDOMAINUUID_CLOCK_B_1__PTCP_SUBDOMAINUUID_63_32            (0xFFFFFFFF)
#define U_PNIP__PTCP_SUBDOMAINUUID_CLOCK_B_1_RESET__VALUE        0x0
#define U_PNIP__PTCP_SUBDOMAINUUID_CLOCK_B_2                                      (0x10617108 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_SUBDOMAINUUID_CLOCK_B_2__PTCP_SUBDOMAINUUID95_64             (0xFFFFFFFF)
#define U_PNIP__PTCP_SUBDOMAINUUID_CLOCK_B_2_RESET__VALUE        0x0
#define U_PNIP__PTCP_SUBDOMAINUUID_CLOCK_B_3                                      (0x1061710C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_SUBDOMAINUUID_CLOCK_B_3__PTCP_SUBDOMAINUUID_127_96           (0xFFFFFFFF)
#define U_PNIP__PTCP_SUBDOMAINUUID_CLOCK_B_3_RESET__VALUE        0x0
#define U_PNIP__PTCP_MASTERSA_LOW_CLOCK_B                                         (0x10617110 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_MASTERSA_LOW_CLOCK_B__PTCP_MASTERSOURCEADDRESS_31_0          (0xFFFFFFFF)
#define U_PNIP__PTCP_MASTERSA_LOW_CLOCK_B_RESET__VALUE           0x0
#define U_PNIP__PTCP_MASTERSA_HIGH_CLOCK_B                                        (0x10617114 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_MASTERSA_HIGH_CLOCK_B__PTCP_MASTERSOURCEADDRESS_47_32        (0x0000FFFF)
#define U_PNIP__PTCP_MASTERSA_HIGH_CLOCK_B_RESET__VALUE          0x0
#define U_PNIP__PTCP_SEQUENCEID_CLOCK_B                                           (0x10617118 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_SEQUENCEID_CLOCK_B__PTCP_PORT                                (0x70000000)
#define U_PNIP__PTCP_SEQUENCEID_CLOCK_B__PTCP_SEQUENCEID                          (0x0000FFFF)
#define U_PNIP__PTCP_SEQUENCEID_CLOCK_B_RESET__VALUE             0x0
#define U_PNIP__PTCP_WD_CLOCK_B                                                   (0x1061711C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_WD_CLOCK_B__PTCP_WD_ENABLE                                   (0x80000000)
#define U_PNIP__PTCP_WD_CLOCK_B__PTCP_WD_VALUE                                    (0x0007FFFF)
#define U_PNIP__PTCP_WD_CLOCK_B_RESET__VALUE                     0x0
#define U_PNIP__PTCP_WDRELOAD_CLOCK_B                                             (0x10617120 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_WDRELOAD_CLOCK_B__PTCP_WD_RELOAD                             (0x0007FFFF)
#define U_PNIP__PTCP_WDRELOAD_CLOCK_B_RESET__VALUE               0x0
#define U_PNIP__PTCP_SUBDOMAINUUID_TIME_0                                         (0x10617200 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_SUBDOMAINUUID_TIME_0__PTCP_SUBDOMAINUUID_31_0                (0xFFFFFFFF)
#define U_PNIP__PTCP_SUBDOMAINUUID_TIME_0_RESET__VALUE           0x0
#define U_PNIP__PTCP_SUBDOMAINUUID_TIME_1                                         (0x10617204 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_SUBDOMAINUUID_TIME_1__PTCP_SUBDOMAINUUID_63_32               (0xFFFFFFFF)
#define U_PNIP__PTCP_SUBDOMAINUUID_TIME_1_RESET__VALUE           0x0
#define U_PNIP__PTCP_SUBDOMAINUUID_TIME_2                                         (0x10617208 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_SUBDOMAINUUID_TIME_2__PTCP_SUBDOMAINUUID_95_64               (0xFFFFFFFF)
#define U_PNIP__PTCP_SUBDOMAINUUID_TIME_2_RESET__VALUE           0x0
#define U_PNIP__PTCP_SUBDOMAINUUID_TIME_3                                         (0x1061720C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_SUBDOMAINUUID_TIME_3__PTCP_SUBDOMAINUUID_127_96              (0xFFFFFFFF)
#define U_PNIP__PTCP_SUBDOMAINUUID_TIME_3_RESET__VALUE           0x0
#define U_PNIP__PTCP_MASTERSA_LOW_TIME                                            (0x10617210 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_MASTERSA_LOW_TIME__PTCP_MASTERSOURCEADDRESS_31_0             (0xFFFFFFFF)
#define U_PNIP__PTCP_MASTERSA_LOW_TIME_RESET__VALUE              0x0
#define U_PNIP__PTCP_MASTERSA_HIGH_TIME                                           (0x10617214 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_MASTERSA_HIGH_TIME__PTCP_MASTERSOURCEADDRESS_47_32           (0x0000FFFF)
#define U_PNIP__PTCP_MASTERSA_HIGH_TIME_RESET__VALUE             0x0
#define U_PNIP__PTCP_SEQUENCEID_TIME                                              (0x10617218 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_SEQUENCEID_TIME__PTCP_PORT                                   (0x70000000)
#define U_PNIP__PTCP_SEQUENCEID_TIME__PTCP_SEQUENCEID                             (0x0000FFFF)
#define U_PNIP__PTCP_SEQUENCEID_TIME_RESET__VALUE                0x0
#define U_PNIP__PTCP_WD_TIME                                                      (0x1061721C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_WD_TIME__PTCP_WD_ENABLE                                      (0x80000000)
#define U_PNIP__PTCP_WD_TIME__PTCP_WD_VALUE                                       (0x0007FFFF)
#define U_PNIP__PTCP_WD_TIME_RESET__VALUE                        0x0
#define U_PNIP__PTCP_WDRELOAD_TIME                                                (0x10617220 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_WDRELOAD_TIME__PTCP_WD_RELOAD                                (0x0007FFFF)
#define U_PNIP__PTCP_WDRELOAD_TIME_RESET__VALUE                  0x0
#define U_PNIP__PTCP_SUBDOMAINUUID_CMD_0                                          (0x10617300 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_SUBDOMAINUUID_CMD_0__PTCP_SUBDOMAINUUID_31_0                 (0xFFFFFFFF)
#define U_PNIP__PTCP_SUBDOMAINUUID_CMD_0_RESET__VALUE            0x0
#define U_PNIP__PTCP_SUBDOMAINUUID_CMD_1                                          (0x10617304 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_SUBDOMAINUUID_CMD_1__PTCP_SUBDOMAINUUID_63_32                (0xFFFFFFFF)
#define U_PNIP__PTCP_SUBDOMAINUUID_CMD_1_RESET__VALUE            0x0
#define U_PNIP__PTCP_SUBDOMAINUUID_CMD_2                                          (0x10617308 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_SUBDOMAINUUID_CMD_2__PTCP_SUBDOMAINUUID95_64                 (0xFFFFFFFF)
#define U_PNIP__PTCP_SUBDOMAINUUID_CMD_2_RESET__VALUE            0x0
#define U_PNIP__PTCP_SUBDOMAINUUID_CMD_3                                          (0x1061730C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_SUBDOMAINUUID_CMD_3__PTCP_SUBDOMAINUUID_127_96               (0xFFFFFFFF)
#define U_PNIP__PTCP_SUBDOMAINUUID_CMD_3_RESET__VALUE            0x0
#define U_PNIP__PTCP_MASTERSA_LOW_CMD                                             (0x10617310 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_MASTERSA_LOW_CMD__PTCP_MASTERSOURCEADDRESS_31_0              (0xFFFFFFFF)
#define U_PNIP__PTCP_MASTERSA_LOW_CMD_RESET__VALUE               0x0
#define U_PNIP__PTCP_MASTERSA_HIGH_CMD                                            (0x10617314 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_MASTERSA_HIGH_CMD__PTCP_MASTERSOURCEADDRESS_47_32            (0x0000FFFF)
#define U_PNIP__PTCP_MASTERSA_HIGH_CMD_RESET__VALUE              0x0
#define U_PNIP__PTCP_WD_EXPIRED                                                   (0x10617400 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_WD_EXPIRED__PTCP_WD_TIME                                     (0x00000004)
#define U_PNIP__PTCP_WD_EXPIRED__PTCP_WD_CLOCK_B                                  (0x00000002)
#define U_PNIP__PTCP_WD_EXPIRED__PTCP_WD_CLOCK_A                                  (0x00000001)
#define U_PNIP__PTCP_WD_EXPIRED_RESET__VALUE                     0x0
#define U_PNIP__PTCP_FORWARDING                                                   (0x10617404 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCP_FORWARDING__PTCP_TXBOUNDARY_P4                               (0x30000000)
#define U_PNIP__PTCP_FORWARDING__PTCP_RXBOUNDARY_P4                               (0x03000000)
#define U_PNIP__PTCP_FORWARDING__PTCP_TXBOUNDARY_P3                               (0x00300000)
#define U_PNIP__PTCP_FORWARDING__PTCP_RXBOUNDARY_P3                               (0x00030000)
#define U_PNIP__PTCP_FORWARDING__PTCP_TXBOUNDARY_P2                               (0x00003000)
#define U_PNIP__PTCP_FORWARDING__PTCP_RXBOUNDARY_P2                               (0x00000300)
#define U_PNIP__PTCP_FORWARDING__PTCP_TXBOUNDARY_P1                               (0x00000030)
#define U_PNIP__PTCP_FORWARDING__PTCP_RXBOUNDARY_P1                               (0x00000003)
#define U_PNIP__PTCP_FORWARDING_RESET__VALUE                     0x33333333
#define U_PNIP__VLAN_CONTROL                                                      (0x1061C000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__VLAN_CONTROL__BASICMODE_TAG_IFB                                   (0x07000000)
#define U_PNIP__VLAN_CONTROL__BASICMODE_TAG_IFA                                   (0x00070000)
#define U_PNIP__VLAN_CONTROL__VLAN_INGRESS                                        (0x00003F00)
#define U_PNIP__VLAN_CONTROL__TAG_MODE                                            (0x0000003F)
#define U_PNIP__VLAN_CONTROL_RESET__VALUE                        0x3030000
#define U_PNIP__VLAN_DEFAULT_IFA                                                  (0x1061C004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__VLAN_DEFAULT_IFA__DEFAULT_VLAN_PRIORITY                           (0xE0000000)
#define U_PNIP__VLAN_DEFAULT_IFA__DEFAULT_CFI                                     (0x10000000)
#define U_PNIP__VLAN_DEFAULT_IFA__DEFAULT_VLAN_ID                                 (0x0FFF0000)
#define U_PNIP__VLAN_DEFAULT_IFA__DEFAULT_STRIPPORT                               (0x00001E00)
#define U_PNIP__VLAN_DEFAULT_IFA__DEFAULT_VLANPORT                                (0x0000001F)
#define U_PNIP__VLAN_DEFAULT_IFA_RESET__VALUE                    0x0
#define U_PNIP__VLAN_DEFAULTPRIO_IFA                                              (0x1061C008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__VLAN_DEFAULTPRIO_IFA__VLANPRIO_NRT                                (0x00070000)
#define U_PNIP__VLAN_DEFAULTPRIO_IFA__VLANPRIO_RTA                                (0x00007000)
#define U_PNIP__VLAN_DEFAULTPRIO_IFA__VLANPRIO_RTLOW                              (0x00000700)
#define U_PNIP__VLAN_DEFAULTPRIO_IFA__VLANPRIO_RTHIGH                             (0x00000070)
#define U_PNIP__VLAN_DEFAULTPRIO_IFA__VLANPRIO_RT                                 (0x00000007)
#define U_PNIP__VLAN_DEFAULTPRIO_IFA_RESET__VALUE                0x5566
#define U_PNIP__VLAN_DEFAULT_IFB                                                  (0x1061C00C + PNIP_DEV_TOP_BASE)
#define U_PNIP__VLAN_DEFAULT_IFB__DEFAULT_VLAN_PRIORITY                           (0xE0000000)
#define U_PNIP__VLAN_DEFAULT_IFB__DEFAULT_CFI                                     (0x10000000)
#define U_PNIP__VLAN_DEFAULT_IFB__DEFAULT_VLAN_ID                                 (0x0FFF0000)
#define U_PNIP__VLAN_DEFAULT_IFB__DEFAULT_STRIPPORT                               (0x00001E00)
#define U_PNIP__VLAN_DEFAULT_IFB__DEFAULT_VLANPORT                                (0x0000001F)
#define U_PNIP__VLAN_DEFAULT_IFB_RESET__VALUE                    0x0
#define U_PNIP__VLAN_DEFAULTPRIO_IFB                                              (0x1061C010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__VLAN_DEFAULTPRIO_IFB__VLANPRIO_NRT                                (0x00070000)
#define U_PNIP__VLAN_DEFAULTPRIO_IFB__VLANPRIO_RTA                                (0x00007000)
#define U_PNIP__VLAN_DEFAULTPRIO_IFB__VLANPRIO_RTLOW                              (0x00000700)
#define U_PNIP__VLAN_DEFAULTPRIO_IFB__VLANPRIO_RTHIGH                             (0x00000070)
#define U_PNIP__VLAN_DEFAULTPRIO_IFB__VLANPRIO_RT                                 (0x00000007)
#define U_PNIP__VLAN_DEFAULTPRIO_IFB_RESET__VALUE                0x5566
#define U_PNIP__VLAN_DEFAULT_P1                                                   (0x1061C014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__VLAN_DEFAULT_P1__DEFAULT_VLAN_PRIORITY                            (0xE0000000)
#define U_PNIP__VLAN_DEFAULT_P1__DEFAULT_CFI                                      (0x10000000)
#define U_PNIP__VLAN_DEFAULT_P1__DEFAULT_VLAN_ID                                  (0x0FFF0000)
#define U_PNIP__VLAN_DEFAULT_P1__DEFAULT_STRIPPORT                                (0x00001E00)
#define U_PNIP__VLAN_DEFAULT_P1__DEFAULT_VLANPORT                                 (0x0000001F)
#define U_PNIP__VLAN_DEFAULT_P1_RESET__VALUE                     0x0
#define U_PNIP__VLAN_DEFAULTPRIO_P1                                               (0x1061C018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__VLAN_DEFAULTPRIO_P1__VLANPRIO_NRT                                 (0x00070000)
#define U_PNIP__VLAN_DEFAULTPRIO_P1__VLANPRIO_RTA                                 (0x00007000)
#define U_PNIP__VLAN_DEFAULTPRIO_P1__VLANPRIO_RTLOW                               (0x00000700)
#define U_PNIP__VLAN_DEFAULTPRIO_P1__VLANPRIO_RTHIGH                              (0x00000070)
#define U_PNIP__VLAN_DEFAULTPRIO_P1__VLANPRIO_RT                                  (0x00000007)
#define U_PNIP__VLAN_DEFAULTPRIO_P1_RESET__VALUE                 0x5566
#define U_PNIP__VLAN_DEFAULT_P2                                                   (0x1061C01C + PNIP_DEV_TOP_BASE)
#define U_PNIP__VLAN_DEFAULT_P2__DEFAULT_VLAN_PRIORITY                            (0xE0000000)
#define U_PNIP__VLAN_DEFAULT_P2__DEFAULT_CFI                                      (0x10000000)
#define U_PNIP__VLAN_DEFAULT_P2__DEFAULT_VLAN_ID                                  (0x0FFF0000)
#define U_PNIP__VLAN_DEFAULT_P2__DEFAULT_STRIPPORT                                (0x00001E00)
#define U_PNIP__VLAN_DEFAULT_P2__DEFAULT_VLANPORT                                 (0x0000001F)
#define U_PNIP__VLAN_DEFAULT_P2_RESET__VALUE                     0x0
#define U_PNIP__VLAN_DEFAULTPRIO_P2                                               (0x1061C020 + PNIP_DEV_TOP_BASE)
#define U_PNIP__VLAN_DEFAULTPRIO_P2__VLANPRIO_NRT                                 (0x00070000)
#define U_PNIP__VLAN_DEFAULTPRIO_P2__VLANPRIO_RTA                                 (0x00007000)
#define U_PNIP__VLAN_DEFAULTPRIO_P2__VLANPRIO_RTLOW                               (0x00000700)
#define U_PNIP__VLAN_DEFAULTPRIO_P2__VLANPRIO_RTHIGH                              (0x00000070)
#define U_PNIP__VLAN_DEFAULTPRIO_P2__VLANPRIO_RT                                  (0x00000007)
#define U_PNIP__VLAN_DEFAULTPRIO_P2_RESET__VALUE                 0x5566
#define U_PNIP__VLAN_DEFAULT_P3                                                   (0x1061C024 + PNIP_DEV_TOP_BASE)
#define U_PNIP__VLAN_DEFAULT_P3__DEFAULT_VLAN_PRIORITY                            (0xE0000000)
#define U_PNIP__VLAN_DEFAULT_P3__DEFAULT_CFI                                      (0x10000000)
#define U_PNIP__VLAN_DEFAULT_P3__DEFAULT_VLAN_ID                                  (0x0FFF0000)
#define U_PNIP__VLAN_DEFAULT_P3__DEFAULT_STRIPPORT                                (0x00001E00)
#define U_PNIP__VLAN_DEFAULT_P3__DEFAULT_VLANPORT                                 (0x0000001F)
#define U_PNIP__VLAN_DEFAULT_P3_RESET__VALUE                     0x0
#define U_PNIP__VLAN_DEFAULTPRIO_P3                                               (0x1061C028 + PNIP_DEV_TOP_BASE)
#define U_PNIP__VLAN_DEFAULTPRIO_P3__VLANPRIO_NRT                                 (0x00070000)
#define U_PNIP__VLAN_DEFAULTPRIO_P3__VLANPRIO_RTA                                 (0x00007000)
#define U_PNIP__VLAN_DEFAULTPRIO_P3__VLANPRIO_RTLOW                               (0x00000700)
#define U_PNIP__VLAN_DEFAULTPRIO_P3__VLANPRIO_RTHIGH                              (0x00000070)
#define U_PNIP__VLAN_DEFAULTPRIO_P3__VLANPRIO_RT                                  (0x00000007)
#define U_PNIP__VLAN_DEFAULTPRIO_P3_RESET__VALUE                 0x5566
#define U_PNIP__VLAN_DEFAULT_P4                                                   (0x1061C02C + PNIP_DEV_TOP_BASE)
#define U_PNIP__VLAN_DEFAULT_P4__DEFAULT_VLAN_PRIORITY                            (0xE0000000)
#define U_PNIP__VLAN_DEFAULT_P4__DEFAULT_CFI                                      (0x10000000)
#define U_PNIP__VLAN_DEFAULT_P4__DEFAULT_VLAN_ID                                  (0x0FFF0000)
#define U_PNIP__VLAN_DEFAULT_P4__DEFAULT_STRIPPORT                                (0x00001E00)
#define U_PNIP__VLAN_DEFAULT_P4__DEFAULT_VLANPORT                                 (0x0000001F)
#define U_PNIP__VLAN_DEFAULT_P4_RESET__VALUE                     0x0
#define U_PNIP__VLAN_DEFAULTPRIO_P4                                               (0x1061C030 + PNIP_DEV_TOP_BASE)
#define U_PNIP__VLAN_DEFAULTPRIO_P4__VLANPRIO_NRT                                 (0x00070000)
#define U_PNIP__VLAN_DEFAULTPRIO_P4__VLANPRIO_RTA                                 (0x00007000)
#define U_PNIP__VLAN_DEFAULTPRIO_P4__VLANPRIO_RTLOW                               (0x00000700)
#define U_PNIP__VLAN_DEFAULTPRIO_P4__VLANPRIO_RTHIGH                              (0x00000070)
#define U_PNIP__VLAN_DEFAULTPRIO_P4__VLANPRIO_RT                                  (0x00000007)
#define U_PNIP__VLAN_DEFAULTPRIO_P4_RESET__VALUE                 0x5566
#define U_PNIP__VLAN_LFSR_MASK                                                    (0x1061C034 + PNIP_DEV_TOP_BASE)
#define U_PNIP__VLAN_LFSR_MASK__LFSRMASK                                          (0x000000FF)
#define U_PNIP__VLAN_LFSR_MASK_RESET__VALUE                      0xAA
#define U_PNIP__VLAN_LFSR_RANGE_A                                                 (0x1061C038 + PNIP_DEV_TOP_BASE)
#define U_PNIP__VLAN_LFSR_RANGE_A__LFSRRANGE_A                                    (0x000000FF)
#define U_PNIP__VLAN_LFSR_RANGE_A_RESET__VALUE                   0xFF
#define U_PNIP__VLAN_TABLE_BASE_B                                                 (0x1061C03C + PNIP_DEV_TOP_BASE)
#define U_PNIP__VLAN_TABLE_BASE_B__VLAN_TABLE_BASE_B                              (0x001FFFFF)
#define U_PNIP__VLAN_TABLE_BASE_B_RESET__VALUE                   0x11FFE0
#define U_PNIP__VLAN_LFSR_RANGE_B                                                 (0x1061C040 + PNIP_DEV_TOP_BASE)
#define U_PNIP__VLAN_LFSR_RANGE_B__LFSRRANGE_B                                    (0x000000FF)
#define U_PNIP__VLAN_LFSR_RANGE_B_RESET__VALUE                   0x0
#define U_PNIP__VLANCOMMAND_IF_CONTROL                                            (0x1061C100 + PNIP_DEV_TOP_BASE)
#define U_PNIP__VLANCOMMAND_IF_CONTROL__COMMANDVALUE                              (0xFFFFFF00)
#define U_PNIP__VLANCOMMAND_IF_CONTROL__USER_ID                                   (0x000000E0)
#define U_PNIP__VLANCOMMAND_IF_CONTROL__CONFREQUEST                               (0x00000010)
#define U_PNIP__VLANCOMMAND_IF_CONTROL__IF_SELECT                                 (0x00000008)
#define U_PNIP__VLANCOMMAND_IF_CONTROL__F_CODE                                    (0x00000007)
#define U_PNIP__VLANCOMMAND_IF_CONTROL_RESET__VALUE              0x0
#define U_PNIP__VLANCOMMAND_IF_STATUS                                             (0x1061C104 + PNIP_DEV_TOP_BASE)
#define U_PNIP__VLANCOMMAND_IF_STATUS__RETURNVALUE                                (0x1FFFFF00)
#define U_PNIP__VLANCOMMAND_IF_STATUS__USER_ID                                    (0x000000E0)
#define U_PNIP__VLANCOMMAND_IF_STATUS__CONFRESPONSE                               (0x00000010)
#define U_PNIP__VLANCOMMAND_IF_STATUS__CONFERROR                                  (0x00000008)
#define U_PNIP__VLANCOMMAND_IF_STATUS__F_CODE                                     (0x00000007)
#define U_PNIP__VLANCOMMAND_IF_STATUS_RESET__VALUE               0x0
#define U_PNIP__QUEUESTATUS_API                                                   (0x10620000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUESTATUS_API__QUEUESTATUS_TIME                                 (0x00008000)
#define U_PNIP__QUEUESTATUS_API__QUEUESTATUS_PHASE                                (0x00004000)
#define U_PNIP__QUEUESTATUS_API__QUEUESTATUS_PRIO                                 (0x00000FFF)
#define U_PNIP__QUEUESTATUS_API_RESET__VALUE                     0x0
#define U_PNIP__QUEUELIST_API_0                                                   (0x10620004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_API_0__LAST_QCW                                         (0x0FFF0000)
#define U_PNIP__QUEUELIST_API_0__FIRST_QCW                                        (0x00000FFF)
#define U_PNIP__QUEUELIST_API_0_RESET__VALUE                     0x0
#define U_PNIP__QUEUELIST_API_1                                                   (0x10620008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_API_1__LAST_QCW                                         (0x0FFF0000)
#define U_PNIP__QUEUELIST_API_1__FIRST_QCW                                        (0x00000FFF)
#define U_PNIP__QUEUELIST_API_1_RESET__VALUE                     0x0
#define U_PNIP__QUEUELIST_API_2                                                   (0x1062000C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_API_2__LAST_QCW                                         (0x0FFF0000)
#define U_PNIP__QUEUELIST_API_2__FIRST_QCW                                        (0x00000FFF)
#define U_PNIP__QUEUELIST_API_2_RESET__VALUE                     0x0
#define U_PNIP__QUEUELIST_API_3                                                   (0x10620010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_API_3__LAST_QCW                                         (0x0FFF0000)
#define U_PNIP__QUEUELIST_API_3__FIRST_QCW                                        (0x00000FFF)
#define U_PNIP__QUEUELIST_API_3_RESET__VALUE                     0x0
#define U_PNIP__QUEUELIST_API_4                                                   (0x10620014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_API_4__LAST_QCW                                         (0x0FFF0000)
#define U_PNIP__QUEUELIST_API_4__FIRST_QCW                                        (0x00000FFF)
#define U_PNIP__QUEUELIST_API_4_RESET__VALUE                     0x0
#define U_PNIP__QUEUELIST_API_5                                                   (0x10620018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_API_5__LAST_QCW                                         (0x0FFF0000)
#define U_PNIP__QUEUELIST_API_5__FIRST_QCW                                        (0x00000FFF)
#define U_PNIP__QUEUELIST_API_5_RESET__VALUE                     0x0
#define U_PNIP__QUEUELIST_API_6                                                   (0x1062001C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_API_6__LAST_QCW                                         (0x0FFF0000)
#define U_PNIP__QUEUELIST_API_6__FIRST_QCW                                        (0x00000FFF)
#define U_PNIP__QUEUELIST_API_6_RESET__VALUE                     0x0
#define U_PNIP__QUEUELIST_API_7                                                   (0x10620020 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_API_7__LAST_QCW                                         (0x0FFF0000)
#define U_PNIP__QUEUELIST_API_7__FIRST_QCW                                        (0x00000FFF)
#define U_PNIP__QUEUELIST_API_7_RESET__VALUE                     0x0
#define U_PNIP__QUEUELIST_API_8                                                   (0x10620024 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_API_8__LAST_QCW                                         (0x0FFF0000)
#define U_PNIP__QUEUELIST_API_8__FIRST_QCW                                        (0x00000FFF)
#define U_PNIP__QUEUELIST_API_8_RESET__VALUE                     0x0
#define U_PNIP__QUEUELIST_API_9                                                   (0x10620028 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_API_9__LAST_QCW                                         (0x0FFF0000)
#define U_PNIP__QUEUELIST_API_9__FIRST_QCW                                        (0x00000FFF)
#define U_PNIP__QUEUELIST_API_9_RESET__VALUE                     0x0
#define U_PNIP__QUEUELIST_API_10                                                  (0x1062002C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_API_10__LAST_QCW                                        (0x0FFF0000)
#define U_PNIP__QUEUELIST_API_10__FIRST_QCW                                       (0x00000FFF)
#define U_PNIP__QUEUELIST_API_10_RESET__VALUE                    0x0
#define U_PNIP__QUEUELIST_API_11                                                  (0x10620030 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_API_11__LAST_QCW                                        (0x0FFF0000)
#define U_PNIP__QUEUELIST_API_11__FIRST_QCW                                       (0x00000FFF)
#define U_PNIP__QUEUELIST_API_11_RESET__VALUE                    0x0
#define U_PNIP__QUEUELIST_API_14                                                  (0x1062003C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_API_14__LAST_QCW                                        (0x0FFF0000)
#define U_PNIP__QUEUELIST_API_14__FIRST_QCW                                       (0x00000FFF)
#define U_PNIP__QUEUELIST_API_14_RESET__VALUE                    0x0
#define U_PNIP__QUEUELIST_API_15                                                  (0x10620040 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_API_15__LAST_QCW                                        (0x0FFF0000)
#define U_PNIP__QUEUELIST_API_15__FIRST_QCW                                       (0x00000FFF)
#define U_PNIP__QUEUELIST_API_15_RESET__VALUE                    0x0
#define U_PNIP__HOL_STATUS_IFA                                                    (0x10620100 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_STATUS_IFA__HOL_MASK                                          (0xFFFF0000)
#define U_PNIP__HOL_STATUS_IFA__HOL_EXPIRED                                       (0x0000FFFF)
#define U_PNIP__HOL_STATUS_IFA_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_IFA_0                                                   (0x10620104 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFA_0__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_IFA_0__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFA_0_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_IFA_1                                                   (0x10620108 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFA_1__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_IFA_1__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFA_1_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_IFA_2                                                   (0x1062010C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFA_2__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_IFA_2__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFA_2_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_IFA_3                                                   (0x10620110 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFA_3__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_IFA_3__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFA_3_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_IFA_4                                                   (0x10620114 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFA_4__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_IFA_4__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFA_4_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_IFA_5                                                   (0x10620118 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFA_5__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_IFA_5__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFA_5_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_IFA_6                                                   (0x1062011C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFA_6__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_IFA_6__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFA_6_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_IFA_7                                                   (0x10620120 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFA_7__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_IFA_7__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFA_7_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_IFA_8                                                   (0x10620124 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFA_8__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_IFA_8__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFA_8_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_IFA_9                                                   (0x10620128 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFA_9__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_IFA_9__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFA_9_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_IFA_10                                                  (0x1062012C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFA_10__RESOURCEOVERRUN                                 (0x80000000)
#define U_PNIP__HOL_COUNT_IFA_10__RESOURCECOUNT                                   (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFA_10_RESET__VALUE                    0x0
#define U_PNIP__HOL_COUNT_IFA_11                                                  (0x10620130 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFA_11__RESOURCEOVERRUN                                 (0x80000000)
#define U_PNIP__HOL_COUNT_IFA_11__RESOURCECOUNT                                   (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFA_11_RESET__VALUE                    0x0
#define U_PNIP__HOL_COUNT_IFA_12                                                  (0x10620134 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFA_12__RESOURCEOVERRUN                                 (0x80000000)
#define U_PNIP__HOL_COUNT_IFA_12__RESOURCECOUNT                                   (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFA_12_RESET__VALUE                    0x0
#define U_PNIP__HOL_COUNT_IFA_13                                                  (0x10620138 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFA_13__RESOURCEOVERRUN                                 (0x80000000)
#define U_PNIP__HOL_COUNT_IFA_13__RESOURCECOUNT                                   (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFA_13_RESET__VALUE                    0x0
#define U_PNIP__HOL_COUNT_IFA_14                                                  (0x1062013C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFA_14__RESOURCEOVERRUN                                 (0x80000000)
#define U_PNIP__HOL_COUNT_IFA_14__RESOURCECOUNT                                   (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFA_14_RESET__VALUE                    0x0
#define U_PNIP__HOL_COUNT_IFA_15                                                  (0x10620140 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFA_15__RESOURCEOVERRUN                                 (0x80000000)
#define U_PNIP__HOL_COUNT_IFA_15__RESOURCECOUNT                                   (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFA_15_RESET__VALUE                    0x0
#define U_PNIP__HOL_CONTROL_IFA                                                   (0x10620144 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_CONTROL_IFA__HOL_MODE                                         (0x0000FFFE)
#define U_PNIP__HOL_CONTROL_IFA_RESET__VALUE                     0x0
#define U_PNIP__HOL_BARRIER_IFA_0                                                 (0x10620148 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFA_0__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFA_0_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_IFA_1                                                 (0x1062014C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFA_1__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFA_1_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_IFA_2                                                 (0x10620150 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFA_2__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFA_2_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_IFA_3                                                 (0x10620154 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFA_3__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFA_3_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_IFA_4                                                 (0x10620158 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFA_4__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFA_4_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_IFA_5                                                 (0x1062015C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFA_5__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFA_5_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_IFA_6                                                 (0x10620160 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFA_6__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFA_6_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_IFA_7                                                 (0x10620164 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFA_7__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFA_7_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_IFA_8                                                 (0x10620168 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFA_8__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFA_8_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_IFA_9                                                 (0x1062016C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFA_9__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFA_9_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_IFA_10                                                (0x10620170 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFA_10__RESOURCETYPE_X_BARRIER                        (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFA_10_RESET__VALUE                  0x230
#define U_PNIP__HOL_BARRIER_IFA_11                                                (0x10620174 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFA_11__RESOURCETYPE_X_BARRIER                        (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFA_11_RESET__VALUE                  0x230
#define U_PNIP__HOL_BARRIER_IFA_12                                                (0x10620178 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFA_12__RESOURCETYPE_X_BARRIER                        (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFA_12_RESET__VALUE                  0x230
#define U_PNIP__HOL_BARRIER_IFA_13                                                (0x1062017C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFA_13__RESOURCETYPE_X_BARRIER                        (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFA_13_RESET__VALUE                  0x230
#define U_PNIP__HOL_BARRIER_IFA_14                                                (0x10620180 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFA_14__RESOURCETYPE_X_BARRIER                        (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFA_14_RESET__VALUE                  0x230
#define U_PNIP__HOL_BARRIER_IFA_15                                                (0x10620184 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFA_15__RESOURCETYPE_X_BARRIER                        (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFA_15_RESET__VALUE                  0x230
#define U_PNIP__HOL_STATUS_IFB                                                    (0x10620200 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_STATUS_IFB__HOL_MASK                                          (0xFFFF0000)
#define U_PNIP__HOL_STATUS_IFB__HOL_EXPIRED                                       (0x0000FFFF)
#define U_PNIP__HOL_STATUS_IFB_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_IFB_0                                                   (0x10620204 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFB_0__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_IFB_0__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFB_0_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_IFB_1                                                   (0x10620208 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFB_1__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_IFB_1__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFB_1_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_IFB_2                                                   (0x1062020C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFB_2__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_IFB_2__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFB_2_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_IFB_3                                                   (0x10620210 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFB_3__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_IFB_3__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFB_3_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_IFB_4                                                   (0x10620214 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFB_4__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_IFB_4__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFB_4_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_IFB_5                                                   (0x10620218 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFB_5__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_IFB_5__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFB_5_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_IFB_6                                                   (0x1062021C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFB_6__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_IFB_6__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFB_6_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_IFB_7                                                   (0x10620220 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFB_7__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_IFB_7__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFB_7_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_IFB_8                                                   (0x10620224 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFB_8__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_IFB_8__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFB_8_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_IFB_9                                                   (0x10620228 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFB_9__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_IFB_9__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFB_9_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_IFB_10                                                  (0x1062022C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFB_10__RESOURCEOVERRUN                                 (0x80000000)
#define U_PNIP__HOL_COUNT_IFB_10__RESOURCECOUNT                                   (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFB_10_RESET__VALUE                    0x0
#define U_PNIP__HOL_COUNT_IFB_11                                                  (0x10620230 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFB_11__RESOURCEOVERRUN                                 (0x80000000)
#define U_PNIP__HOL_COUNT_IFB_11__RESOURCECOUNT                                   (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFB_11_RESET__VALUE                    0x0
#define U_PNIP__HOL_COUNT_IFB_12                                                  (0x10620234 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFB_12__RESOURCEOVERRUN                                 (0x80000000)
#define U_PNIP__HOL_COUNT_IFB_12__RESOURCECOUNT                                   (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFB_12_RESET__VALUE                    0x0
#define U_PNIP__HOL_COUNT_IFB_13                                                  (0x10620238 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFB_13__RESOURCEOVERRUN                                 (0x80000000)
#define U_PNIP__HOL_COUNT_IFB_13__RESOURCECOUNT                                   (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFB_13_RESET__VALUE                    0x0
#define U_PNIP__HOL_COUNT_IFB_14                                                  (0x1062023C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFB_14__RESOURCEOVERRUN                                 (0x80000000)
#define U_PNIP__HOL_COUNT_IFB_14__RESOURCECOUNT                                   (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFB_14_RESET__VALUE                    0x0
#define U_PNIP__HOL_COUNT_IFB_15                                                  (0x10620240 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_IFB_15__RESOURCEOVERRUN                                 (0x80000000)
#define U_PNIP__HOL_COUNT_IFB_15__RESOURCECOUNT                                   (0x00001FFF)
#define U_PNIP__HOL_COUNT_IFB_15_RESET__VALUE                    0x0
#define U_PNIP__HOL_CONTROL_IFB                                                   (0x10620244 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_CONTROL_IFB__HOL_MODE                                         (0x0000FFFE)
#define U_PNIP__HOL_CONTROL_IFB_RESET__VALUE                     0x0
#define U_PNIP__HOL_BARRIER_IFB_0                                                 (0x10620248 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFB_0__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFB_0_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_IFB_1                                                 (0x1062024C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFB_1__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFB_1_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_IFB_2                                                 (0x10620250 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFB_2__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFB_2_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_IFB_3                                                 (0x10620254 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFB_3__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFB_3_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_IFB_4                                                 (0x10620258 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFB_4__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFB_4_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_IFB_5                                                 (0x1062025C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFB_5__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFB_5_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_IFB_6                                                 (0x10620260 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFB_6__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFB_6_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_IFB_7                                                 (0x10620264 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFB_7__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFB_7_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_IFB_8                                                 (0x10620268 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFB_8__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFB_8_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_IFB_9                                                 (0x1062026C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFB_9__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFB_9_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_IFB_10                                                (0x10620270 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFB_10__RESOURCETYPE_X_BARRIER                        (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFB_10_RESET__VALUE                  0x230
#define U_PNIP__HOL_BARRIER_IFB_11                                                (0x10620274 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFB_11__RESOURCETYPE_X_BARRIER                        (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFB_11_RESET__VALUE                  0x230
#define U_PNIP__HOL_BARRIER_IFB_12                                                (0x10620278 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFB_12__RESOURCETYPE_X_BARRIER                        (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFB_12_RESET__VALUE                  0x230
#define U_PNIP__HOL_BARRIER_IFB_13                                                (0x1062027C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFB_13__RESOURCETYPE_X_BARRIER                        (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFB_13_RESET__VALUE                  0x230
#define U_PNIP__HOL_BARRIER_IFB_14                                                (0x10620280 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFB_14__RESOURCETYPE_X_BARRIER                        (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFB_14_RESET__VALUE                  0x230
#define U_PNIP__HOL_BARRIER_IFB_15                                                (0x10620284 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_IFB_15__RESOURCETYPE_X_BARRIER                        (0x00001FFF)
#define U_PNIP__HOL_BARRIER_IFB_15_RESET__VALUE                  0x230
#define U_PNIP__QUEUESTATUS_P1                                                    (0x10621000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUESTATUS_P1__QUEUESTATUS_TIME                                  (0x00008000)
#define U_PNIP__QUEUESTATUS_P1__QUEUESTATUS_PHASE                                 (0x00004000)
#define U_PNIP__QUEUESTATUS_P1__QUEUESTATUS_PRIO                                  (0x00000FFF)
#define U_PNIP__QUEUESTATUS_P1_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P1_0                                                    (0x10621004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P1_0__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P1_0__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P1_0_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P1_1                                                    (0x10621008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P1_1__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P1_1__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P1_1_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P1_2                                                    (0x1062100C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P1_2__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P1_2__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P1_2_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P1_3                                                    (0x10621010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P1_3__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P1_3__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P1_3_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P1_4                                                    (0x10621014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P1_4__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P1_4__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P1_4_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P1_5                                                    (0x10621018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P1_5__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P1_5__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P1_5_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P1_6                                                    (0x1062101C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P1_6__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P1_6__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P1_6_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P1_7                                                    (0x10621020 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P1_7__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P1_7__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P1_7_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P1_8                                                    (0x10621024 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P1_8__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P1_8__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P1_8_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P1_9                                                    (0x10621028 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P1_9__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P1_9__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P1_9_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P1_10                                                   (0x1062102C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P1_10__LAST_QCW                                         (0x0FFF0000)
#define U_PNIP__QUEUELIST_P1_10__FIRST_QCW                                        (0x00000FFF)
#define U_PNIP__QUEUELIST_P1_10_RESET__VALUE                     0x0
#define U_PNIP__QUEUELIST_P1_11                                                   (0x10621030 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P1_11__LAST_QCW                                         (0x0FFF0000)
#define U_PNIP__QUEUELIST_P1_11__FIRST_QCW                                        (0x00000FFF)
#define U_PNIP__QUEUELIST_P1_11_RESET__VALUE                     0x0
#define U_PNIP__QUEUELIST_P1_14                                                   (0x1062103C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P1_14__LAST_QCW                                         (0x0FFF0000)
#define U_PNIP__QUEUELIST_P1_14__FIRST_QCW                                        (0x00000FFF)
#define U_PNIP__QUEUELIST_P1_14_RESET__VALUE                     0x0
#define U_PNIP__QUEUELIST_P1_15                                                   (0x10621040 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P1_15__LAST_QCW                                         (0x0FFF0000)
#define U_PNIP__QUEUELIST_P1_15__FIRST_QCW                                        (0x00000FFF)
#define U_PNIP__QUEUELIST_P1_15_RESET__VALUE                     0x0
#define U_PNIP__DELAYLIST_P1                                                      (0x10621044 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DELAYLIST_P1__DELAYSTATUS                                         (0x80000000)
#define U_PNIP__DELAYLIST_P1__LAST_QCW                                            (0x0FFF0000)
#define U_PNIP__DELAYLIST_P1__FIRST_QCW                                           (0x00000FFF)
#define U_PNIP__DELAYLIST_P1_RESET__VALUE                        0x0
#define U_PNIP__HOL_STATUS_P1                                                     (0x10621100 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_STATUS_P1__HOL_MASK                                           (0xFFFF0000)
#define U_PNIP__HOL_STATUS_P1__HOL_EXPIRED                                        (0x0000FFFF)
#define U_PNIP__HOL_STATUS_P1_RESET__VALUE                       0x0
#define U_PNIP__HOL_COUNT_P1_1                                                    (0x10621104 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P1_1__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P1_1__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P1_1_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P1_2                                                    (0x10621108 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P1_2__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P1_2__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P1_2_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P1_3                                                    (0x1062110C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P1_3__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P1_3__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P1_3_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P1_4                                                    (0x10621110 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P1_4__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P1_4__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P1_4_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P1_5                                                    (0x10621114 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P1_5__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P1_5__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P1_5_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P1_6                                                    (0x10621118 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P1_6__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P1_6__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P1_6_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P1_7                                                    (0x1062111C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P1_7__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P1_7__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P1_7_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P1_8                                                    (0x10621120 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P1_8__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P1_8__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P1_8_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P1_9                                                    (0x10621124 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P1_9__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P1_9__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P1_9_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P1_10                                                   (0x10621128 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P1_10__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_P1_10__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_P1_10_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_P1_11                                                   (0x1062112C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P1_11__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_P1_11__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_P1_11_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_P1_12                                                   (0x10621130 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P1_12__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_P1_12__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_P1_12_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_P1_13                                                   (0x10621134 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P1_13__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_P1_13__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_P1_13_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_P1_14                                                   (0x10621138 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P1_14__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_P1_14__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_P1_14_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_P1_15                                                   (0x1062113C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P1_15__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_P1_15__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_P1_15_RESET__VALUE                     0x0
#define U_PNIP__QCW_STATUS_P1                                                     (0x10621200 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_STATUS_P1__QCW_EXPIRED                                        (0x00000FFF)
#define U_PNIP__QCW_STATUS_P1_RESET__VALUE                       0x0
#define U_PNIP__QCW_COUNT_P1_0                                                    (0x10621204 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P1_0__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P1_0__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P1_0__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P1_0_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P1_1                                                    (0x10621208 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P1_1__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P1_1__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P1_1__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P1_1_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P1_2                                                    (0x1062120C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P1_2__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P1_2__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P1_2__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P1_2_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P1_3                                                    (0x10621210 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P1_3__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P1_3__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P1_3__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P1_3_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P1_4                                                    (0x10621214 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P1_4__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P1_4__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P1_4__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P1_4_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P1_5                                                    (0x10621218 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P1_5__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P1_5__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P1_5__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P1_5_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P1_6                                                    (0x1062121C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P1_6__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P1_6__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P1_6__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P1_6_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P1_7                                                    (0x10621220 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P1_7__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P1_7__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P1_7__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P1_7_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P1_8                                                    (0x10621224 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P1_8__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P1_8__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P1_8__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P1_8_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P1_9                                                    (0x10621228 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P1_9__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P1_9__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P1_9__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P1_9_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P1_10                                                   (0x1062122C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P1_10__ENABLECOUNT                                      (0x80000000)
#define U_PNIP__QCW_COUNT_P1_10__QCWMODE                                          (0x40000000)
#define U_PNIP__QCW_COUNT_P1_10__QCWCOUNT                                         (0x00001FFF)
#define U_PNIP__QCW_COUNT_P1_10_RESET__VALUE                     0x0
#define U_PNIP__QCW_COUNT_P1_11                                                   (0x10621230 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P1_11__ENABLECOUNT                                      (0x80000000)
#define U_PNIP__QCW_COUNT_P1_11__QCWMODE                                          (0x40000000)
#define U_PNIP__QCW_COUNT_P1_11__QCWCOUNT                                         (0x00001FFF)
#define U_PNIP__QCW_COUNT_P1_11_RESET__VALUE                     0x0
#define U_PNIP__QCW_MAPPING_P1_0TO7                                               (0x10621234 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_MAPPING_P1_0TO7__QUEUE7_MAP                                   (0xF0000000)
#define U_PNIP__QCW_MAPPING_P1_0TO7__QUEUE6_MAP                                   (0x0F000000)
#define U_PNIP__QCW_MAPPING_P1_0TO7__QUEUE5_MAP                                   (0x00F00000)
#define U_PNIP__QCW_MAPPING_P1_0TO7__QUEUE4_MAP                                   (0x000F0000)
#define U_PNIP__QCW_MAPPING_P1_0TO7__QUEUE3_MAP                                   (0x0000F000)
#define U_PNIP__QCW_MAPPING_P1_0TO7__QUEUE2_MAP                                   (0x00000F00)
#define U_PNIP__QCW_MAPPING_P1_0TO7__QUEUE1_MAP                                   (0x000000F0)
#define U_PNIP__QCW_MAPPING_P1_0TO7__QUEUE0_MAP                                   (0x0000000F)
#define U_PNIP__QCW_MAPPING_P1_0TO7_RESET__VALUE                 0xFFFFFFFF
#define U_PNIP__QCW_MAPPING_P1_8TO11                                              (0x10621238 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_MAPPING_P1_8TO11__QUEUE11_MAP                                 (0x0000F000)
#define U_PNIP__QCW_MAPPING_P1_8TO11__QUEUE10_MAP                                 (0x00000F00)
#define U_PNIP__QCW_MAPPING_P1_8TO11__QUEUE9_MAP                                  (0x000000F0)
#define U_PNIP__QCW_MAPPING_P1_8TO11__QUEUE8_MAP                                  (0x0000000F)
#define U_PNIP__QCW_MAPPING_P1_8TO11_RESET__VALUE                0xFFFF
#define U_PNIP__FRAMEAGING_STATUS_P1                                              (0x10621300 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMEAGING_STATUS_P1__FRAMEAGING_EXPIRED                          (0x0000FFFF)
#define U_PNIP__FRAMEAGING_STATUS_P1_RESET__VALUE                0x0
#define U_PNIP__FRAMEAGING_TIME_P1                                                (0x10621304 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMEAGING_TIME_P1__AGINGTIME                                     (0x7FFFFFFF)
#define U_PNIP__FRAMEAGING_TIME_P1_RESET__VALUE                  0x0
#define U_PNIP__FRAMEAGING_CYCLE_P1                                               (0x10621308 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMEAGING_CYCLE_P1__AGINGCYCLE                                   (0x0000007F)
#define U_PNIP__FRAMEAGING_CYCLE_P1_RESET__VALUE                 0x0
#define U_PNIP__QUEUESTATUS_P2                                                    (0x10622000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUESTATUS_P2__QUEUESTATUS_TIME                                  (0x00008000)
#define U_PNIP__QUEUESTATUS_P2__QUEUESTATUS_PHASE                                 (0x00004000)
#define U_PNIP__QUEUESTATUS_P2__QUEUESTATUS_PRIO                                  (0x00000FFF)
#define U_PNIP__QUEUESTATUS_P2_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P2_0                                                    (0x10622004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P2_0__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P2_0__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P2_0_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P2_1                                                    (0x10622008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P2_1__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P2_1__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P2_1_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P2_2                                                    (0x1062200C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P2_2__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P2_2__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P2_2_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P2_3                                                    (0x10622010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P2_3__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P2_3__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P2_3_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P2_4                                                    (0x10622014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P2_4__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P2_4__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P2_4_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P2_5                                                    (0x10622018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P2_5__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P2_5__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P2_5_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P2_6                                                    (0x1062201C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P2_6__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P2_6__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P2_6_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P2_7                                                    (0x10622020 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P2_7__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P2_7__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P2_7_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P2_8                                                    (0x10622024 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P2_8__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P2_8__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P2_8_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P2_9                                                    (0x10622028 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P2_9__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P2_9__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P2_9_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P2_10                                                   (0x1062202C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P2_10__LAST_QCW                                         (0x0FFF0000)
#define U_PNIP__QUEUELIST_P2_10__FIRST_QCW                                        (0x00000FFF)
#define U_PNIP__QUEUELIST_P2_10_RESET__VALUE                     0x0
#define U_PNIP__QUEUELIST_P2_11                                                   (0x10622030 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P2_11__LAST_QCW                                         (0x0FFF0000)
#define U_PNIP__QUEUELIST_P2_11__FIRST_QCW                                        (0x00000FFF)
#define U_PNIP__QUEUELIST_P2_11_RESET__VALUE                     0x0
#define U_PNIP__QUEUELIST_P2_14                                                   (0x1062203C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P2_14__LAST_QCW                                         (0x0FFF0000)
#define U_PNIP__QUEUELIST_P2_14__FIRST_QCW                                        (0x00000FFF)
#define U_PNIP__QUEUELIST_P2_14_RESET__VALUE                     0x0
#define U_PNIP__QUEUELIST_P2_15                                                   (0x10622040 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P2_15__LAST_QCW                                         (0x0FFF0000)
#define U_PNIP__QUEUELIST_P2_15__FIRST_QCW                                        (0x00000FFF)
#define U_PNIP__QUEUELIST_P2_15_RESET__VALUE                     0x0
#define U_PNIP__DELAYLIST_P2                                                      (0x10622044 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DELAYLIST_P2__DELAYSTATUS                                         (0x80000000)
#define U_PNIP__DELAYLIST_P2__LAST_QCW                                            (0x0FFF0000)
#define U_PNIP__DELAYLIST_P2__FIRST_QCW                                           (0x00000FFF)
#define U_PNIP__DELAYLIST_P2_RESET__VALUE                        0x0
#define U_PNIP__HOL_STATUS_P2                                                     (0x10622100 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_STATUS_P2__HOL_MASK                                           (0xFFFF0000)
#define U_PNIP__HOL_STATUS_P2__HOL_EXPIRED                                        (0x0000FFFF)
#define U_PNIP__HOL_STATUS_P2_RESET__VALUE                       0x0
#define U_PNIP__HOL_COUNT_P2_1                                                    (0x10622104 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P2_1__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P2_1__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P2_1_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P2_2                                                    (0x10622108 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P2_2__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P2_2__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P2_2_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P2_3                                                    (0x1062210C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P2_3__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P2_3__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P2_3_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P2_4                                                    (0x10622110 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P2_4__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P2_4__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P2_4_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P2_5                                                    (0x10622114 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P2_5__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P2_5__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P2_5_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P2_6                                                    (0x10622118 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P2_6__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P2_6__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P2_6_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P2_7                                                    (0x1062211C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P2_7__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P2_7__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P2_7_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P2_8                                                    (0x10622120 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P2_8__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P2_8__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P2_8_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P2_9                                                    (0x10622124 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P2_9__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P2_9__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P2_9_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P2_10                                                   (0x10622128 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P2_10__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_P2_10__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_P2_10_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_P2_11                                                   (0x1062212C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P2_11__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_P2_11__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_P2_11_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_P2_12                                                   (0x10622130 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P2_12__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_P2_12__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_P2_12_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_P2_13                                                   (0x10622134 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P2_13__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_P2_13__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_P2_13_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_P2_14                                                   (0x10622138 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P2_14__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_P2_14__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_P2_14_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_P2_15                                                   (0x1062213C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P2_15__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_P2_15__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_P2_15_RESET__VALUE                     0x0
#define U_PNIP__QCW_STATUS_P2                                                     (0x10622200 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_STATUS_P2__QCW_EXPIRED                                        (0x00000FFF)
#define U_PNIP__QCW_STATUS_P2_RESET__VALUE                       0x0
#define U_PNIP__QCW_COUNT_P2_0                                                    (0x10622204 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P2_0__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P2_0__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P2_0__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P2_0_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P2_1                                                    (0x10622208 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P2_1__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P2_1__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P2_1__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P2_1_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P2_2                                                    (0x1062220C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P2_2__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P2_2__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P2_2__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P2_2_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P2_3                                                    (0x10622210 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P2_3__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P2_3__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P2_3__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P2_3_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P2_4                                                    (0x10622214 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P2_4__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P2_4__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P2_4__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P2_4_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P2_5                                                    (0x10622218 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P2_5__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P2_5__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P2_5__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P2_5_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P2_6                                                    (0x1062221C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P2_6__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P2_6__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P2_6__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P2_6_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P2_7                                                    (0x10622220 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P2_7__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P2_7__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P2_7__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P2_7_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P2_8                                                    (0x10622224 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P2_8__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P2_8__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P2_8__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P2_8_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P2_9                                                    (0x10622228 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P2_9__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P2_9__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P2_9__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P2_9_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P2_10                                                   (0x1062222C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P2_10__ENABLECOUNT                                      (0x80000000)
#define U_PNIP__QCW_COUNT_P2_10__QCWMODE                                          (0x40000000)
#define U_PNIP__QCW_COUNT_P2_10__QCWCOUNT                                         (0x00001FFF)
#define U_PNIP__QCW_COUNT_P2_10_RESET__VALUE                     0x0
#define U_PNIP__QCW_COUNT_P2_11                                                   (0x10622230 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P2_11__ENABLECOUNT                                      (0x80000000)
#define U_PNIP__QCW_COUNT_P2_11__QCWMODE                                          (0x40000000)
#define U_PNIP__QCW_COUNT_P2_11__QCWCOUNT                                         (0x00001FFF)
#define U_PNIP__QCW_COUNT_P2_11_RESET__VALUE                     0x0
#define U_PNIP__QCW_MAPPING_P2_0TO7                                               (0x10622234 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_MAPPING_P2_0TO7__QUEUE7_MAP                                   (0xF0000000)
#define U_PNIP__QCW_MAPPING_P2_0TO7__QUEUE6_MAP                                   (0x0F000000)
#define U_PNIP__QCW_MAPPING_P2_0TO7__QUEUE5_MAP                                   (0x00F00000)
#define U_PNIP__QCW_MAPPING_P2_0TO7__QUEUE4_MAP                                   (0x000F0000)
#define U_PNIP__QCW_MAPPING_P2_0TO7__QUEUE3_MAP                                   (0x0000F000)
#define U_PNIP__QCW_MAPPING_P2_0TO7__QUEUE2_MAP                                   (0x00000F00)
#define U_PNIP__QCW_MAPPING_P2_0TO7__QUEUE1_MAP                                   (0x000000F0)
#define U_PNIP__QCW_MAPPING_P2_0TO7__QUEUE0_MAP                                   (0x0000000F)
#define U_PNIP__QCW_MAPPING_P2_0TO7_RESET__VALUE                 0xFFFFFFFF
#define U_PNIP__QCW_MAPPING_P2_8TO11                                              (0x10622238 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_MAPPING_P2_8TO11__QUEUE11_MAP                                 (0x0000F000)
#define U_PNIP__QCW_MAPPING_P2_8TO11__QUEUE10_MAP                                 (0x00000F00)
#define U_PNIP__QCW_MAPPING_P2_8TO11__QUEUE9_MAP                                  (0x000000F0)
#define U_PNIP__QCW_MAPPING_P2_8TO11__QUEUE8_MAP                                  (0x0000000F)
#define U_PNIP__QCW_MAPPING_P2_8TO11_RESET__VALUE                0xFFFF
#define U_PNIP__FRAMEAGING_STATUS_P2                                              (0x10622300 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMEAGING_STATUS_P2__FRAMEAGING_EXPIRED                          (0x0000FFFF)
#define U_PNIP__FRAMEAGING_STATUS_P2_RESET__VALUE                0x0
#define U_PNIP__FRAMEAGING_TIME_P2                                                (0x10622304 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMEAGING_TIME_P2__AGINGTIME                                     (0x7FFFFFFF)
#define U_PNIP__FRAMEAGING_TIME_P2_RESET__VALUE                  0x0
#define U_PNIP__FRAMEAGING_CYCLE_P2                                               (0x10622308 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMEAGING_CYCLE_P2__AGINGCYCLE                                   (0x0000007F)
#define U_PNIP__FRAMEAGING_CYCLE_P2_RESET__VALUE                 0x0
#define U_PNIP__QUEUESTATUS_P3                                                    (0x10623000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUESTATUS_P3__QUEUESTATUS_TIME                                  (0x00008000)
#define U_PNIP__QUEUESTATUS_P3__QUEUESTATUS_PHASE                                 (0x00004000)
#define U_PNIP__QUEUESTATUS_P3__QUEUESTATUS_PRIO                                  (0x00000FFF)
#define U_PNIP__QUEUESTATUS_P3_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P3_0                                                    (0x10623004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P3_0__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P3_0__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P3_0_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P3_1                                                    (0x10623008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P3_1__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P3_1__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P3_1_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P3_2                                                    (0x1062300C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P3_2__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P3_2__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P3_2_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P3_3                                                    (0x10623010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P3_3__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P3_3__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P3_3_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P3_4                                                    (0x10623014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P3_4__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P3_4__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P3_4_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P3_5                                                    (0x10623018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P3_5__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P3_5__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P3_5_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P3_6                                                    (0x1062301C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P3_6__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P3_6__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P3_6_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P3_7                                                    (0x10623020 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P3_7__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P3_7__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P3_7_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P3_8                                                    (0x10623024 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P3_8__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P3_8__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P3_8_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P3_9                                                    (0x10623028 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P3_9__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P3_9__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P3_9_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P3_10                                                   (0x1062302C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P3_10__LAST_QCW                                         (0x0FFF0000)
#define U_PNIP__QUEUELIST_P3_10__FIRST_QCW                                        (0x00000FFF)
#define U_PNIP__QUEUELIST_P3_10_RESET__VALUE                     0x0
#define U_PNIP__QUEUELIST_P3_11                                                   (0x10623030 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P3_11__LAST_QCW                                         (0x0FFF0000)
#define U_PNIP__QUEUELIST_P3_11__FIRST_QCW                                        (0x00000FFF)
#define U_PNIP__QUEUELIST_P3_11_RESET__VALUE                     0x0
#define U_PNIP__QUEUELIST_P3_14                                                   (0x1062303C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P3_14__LAST_QCW                                         (0x0FFF0000)
#define U_PNIP__QUEUELIST_P3_14__FIRST_QCW                                        (0x00000FFF)
#define U_PNIP__QUEUELIST_P3_14_RESET__VALUE                     0x0
#define U_PNIP__QUEUELIST_P3_15                                                   (0x10623040 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P3_15__LAST_QCW                                         (0x0FFF0000)
#define U_PNIP__QUEUELIST_P3_15__FIRST_QCW                                        (0x00000FFF)
#define U_PNIP__QUEUELIST_P3_15_RESET__VALUE                     0x0
#define U_PNIP__DELAYLIST_P3                                                      (0x10623044 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DELAYLIST_P3__DELAYSTATUS                                         (0x80000000)
#define U_PNIP__DELAYLIST_P3__LAST_QCW                                            (0x0FFF0000)
#define U_PNIP__DELAYLIST_P3__FIRST_QCW                                           (0x00000FFF)
#define U_PNIP__DELAYLIST_P3_RESET__VALUE                        0x0
#define U_PNIP__HOL_STATUS_P3                                                     (0x10623100 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_STATUS_P3__HOL_MASK                                           (0xFFFF0000)
#define U_PNIP__HOL_STATUS_P3__HOL_EXPIRED                                        (0x0000FFFF)
#define U_PNIP__HOL_STATUS_P3_RESET__VALUE                       0x0
#define U_PNIP__HOL_COUNT_P3_1                                                    (0x10623104 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P3_1__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P3_1__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P3_1_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P3_2                                                    (0x10623108 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P3_2__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P3_2__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P3_2_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P3_3                                                    (0x1062310C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P3_3__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P3_3__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P3_3_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P3_4                                                    (0x10623110 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P3_4__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P3_4__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P3_4_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P3_5                                                    (0x10623114 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P3_5__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P3_5__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P3_5_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P3_6                                                    (0x10623118 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P3_6__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P3_6__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P3_6_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P3_7                                                    (0x1062311C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P3_7__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P3_7__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P3_7_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P3_8                                                    (0x10623120 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P3_8__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P3_8__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P3_8_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P3_9                                                    (0x10623124 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P3_9__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P3_9__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P3_9_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P3_10                                                   (0x10623128 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P3_10__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_P3_10__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_P3_10_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_P3_11                                                   (0x1062312C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P3_11__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_P3_11__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_P3_11_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_P3_12                                                   (0x10623130 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P3_12__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_P3_12__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_P3_12_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_P3_13                                                   (0x10623134 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P3_13__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_P3_13__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_P3_13_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_P3_14                                                   (0x10623138 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P3_14__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_P3_14__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_P3_14_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_P3_15                                                   (0x1062313C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P3_15__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_P3_15__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_P3_15_RESET__VALUE                     0x0
#define U_PNIP__QCW_STATUS_P3                                                     (0x10623200 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_STATUS_P3__QCW_EXPIRED                                        (0x00000FFF)
#define U_PNIP__QCW_STATUS_P3_RESET__VALUE                       0x0
#define U_PNIP__QCW_COUNT_P3_0                                                    (0x10623204 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P3_0__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P3_0__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P3_0__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P3_0_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P3_1                                                    (0x10623208 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P3_1__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P3_1__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P3_1__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P3_1_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P3_2                                                    (0x1062320C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P3_2__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P3_2__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P3_2__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P3_2_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P3_3                                                    (0x10623210 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P3_3__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P3_3__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P3_3__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P3_3_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P3_4                                                    (0x10623214 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P3_4__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P3_4__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P3_4__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P3_4_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P3_5                                                    (0x10623218 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P3_5__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P3_5__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P3_5__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P3_5_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P3_6                                                    (0x1062321C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P3_6__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P3_6__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P3_6__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P3_6_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P3_7                                                    (0x10623220 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P3_7__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P3_7__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P3_7__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P3_7_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P3_8                                                    (0x10623224 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P3_8__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P3_8__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P3_8__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P3_8_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P3_9                                                    (0x10623228 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P3_9__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P3_9__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P3_9__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P3_9_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P3_10                                                   (0x1062322C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P3_10__ENABLECOUNT                                      (0x80000000)
#define U_PNIP__QCW_COUNT_P3_10__QCWMODE                                          (0x40000000)
#define U_PNIP__QCW_COUNT_P3_10__QCWCOUNT                                         (0x00001FFF)
#define U_PNIP__QCW_COUNT_P3_10_RESET__VALUE                     0x0
#define U_PNIP__QCW_COUNT_P3_11                                                   (0x10623230 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P3_11__ENABLECOUNT                                      (0x80000000)
#define U_PNIP__QCW_COUNT_P3_11__QCWMODE                                          (0x40000000)
#define U_PNIP__QCW_COUNT_P3_11__QCWCOUNT                                         (0x00001FFF)
#define U_PNIP__QCW_COUNT_P3_11_RESET__VALUE                     0x0
#define U_PNIP__QCW_MAPPING_P3_0TO7                                               (0x10623234 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_MAPPING_P3_0TO7__QUEUE7_MAP                                   (0xF0000000)
#define U_PNIP__QCW_MAPPING_P3_0TO7__QUEUE6_MAP                                   (0x0F000000)
#define U_PNIP__QCW_MAPPING_P3_0TO7__QUEUE5_MAP                                   (0x00F00000)
#define U_PNIP__QCW_MAPPING_P3_0TO7__QUEUE4_MAP                                   (0x000F0000)
#define U_PNIP__QCW_MAPPING_P3_0TO7__QUEUE3_MAP                                   (0x0000F000)
#define U_PNIP__QCW_MAPPING_P3_0TO7__QUEUE2_MAP                                   (0x00000F00)
#define U_PNIP__QCW_MAPPING_P3_0TO7__QUEUE1_MAP                                   (0x000000F0)
#define U_PNIP__QCW_MAPPING_P3_0TO7__QUEUE0_MAP                                   (0x0000000F)
#define U_PNIP__QCW_MAPPING_P3_0TO7_RESET__VALUE                 0xFFFFFFFF
#define U_PNIP__QCW_MAPPING_P3_8TO11                                              (0x10623238 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_MAPPING_P3_8TO11__QUEUE11_MAP                                 (0x0000F000)
#define U_PNIP__QCW_MAPPING_P3_8TO11__QUEUE10_MAP                                 (0x00000F00)
#define U_PNIP__QCW_MAPPING_P3_8TO11__QUEUE9_MAP                                  (0x000000F0)
#define U_PNIP__QCW_MAPPING_P3_8TO11__QUEUE8_MAP                                  (0x0000000F)
#define U_PNIP__QCW_MAPPING_P3_8TO11_RESET__VALUE                0xFFFF
#define U_PNIP__FRAMEAGING_STATUS_P3                                              (0x10623300 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMEAGING_STATUS_P3__FRAMEAGING_EXPIRED                          (0x0000FFFF)
#define U_PNIP__FRAMEAGING_STATUS_P3_RESET__VALUE                0x0
#define U_PNIP__FRAMEAGING_TIME_P3                                                (0x10623304 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMEAGING_TIME_P3__AGINGTIME                                     (0x7FFFFFFF)
#define U_PNIP__FRAMEAGING_TIME_P3_RESET__VALUE                  0x0
#define U_PNIP__FRAMEAGING_CYCLE_P3                                               (0x10623308 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMEAGING_CYCLE_P3__AGINGCYCLE                                   (0x0000007F)
#define U_PNIP__FRAMEAGING_CYCLE_P3_RESET__VALUE                 0x0
#define U_PNIP__QUEUESTATUS_P4                                                    (0x10624000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUESTATUS_P4__QUEUESTATUS_TIME                                  (0x00008000)
#define U_PNIP__QUEUESTATUS_P4__QUEUESTATUS_PHASE                                 (0x00004000)
#define U_PNIP__QUEUESTATUS_P4__QUEUESTATUS_PRIO                                  (0x00000FFF)
#define U_PNIP__QUEUESTATUS_P4_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P4_0                                                    (0x10624004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P4_0__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P4_0__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P4_0_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P4_1                                                    (0x10624008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P4_1__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P4_1__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P4_1_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P4_2                                                    (0x1062400C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P4_2__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P4_2__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P4_2_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P4_3                                                    (0x10624010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P4_3__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P4_3__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P4_3_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P4_4                                                    (0x10624014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P4_4__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P4_4__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P4_4_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P4_5                                                    (0x10624018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P4_5__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P4_5__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P4_5_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P4_6                                                    (0x1062401C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P4_6__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P4_6__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P4_6_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P4_7                                                    (0x10624020 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P4_7__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P4_7__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P4_7_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P4_8                                                    (0x10624024 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P4_8__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P4_8__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P4_8_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P4_9                                                    (0x10624028 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P4_9__LAST_QCW                                          (0x0FFF0000)
#define U_PNIP__QUEUELIST_P4_9__FIRST_QCW                                         (0x00000FFF)
#define U_PNIP__QUEUELIST_P4_9_RESET__VALUE                      0x0
#define U_PNIP__QUEUELIST_P4_10                                                   (0x1062402C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P4_10__LAST_QCW                                         (0x0FFF0000)
#define U_PNIP__QUEUELIST_P4_10__FIRST_QCW                                        (0x00000FFF)
#define U_PNIP__QUEUELIST_P4_10_RESET__VALUE                     0x0
#define U_PNIP__QUEUELIST_P4_11                                                   (0x10624030 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P4_11__LAST_QCW                                         (0x0FFF0000)
#define U_PNIP__QUEUELIST_P4_11__FIRST_QCW                                        (0x00000FFF)
#define U_PNIP__QUEUELIST_P4_11_RESET__VALUE                     0x0
#define U_PNIP__QUEUELIST_P4_14                                                   (0x1062403C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P4_14__LAST_QCW                                         (0x0FFF0000)
#define U_PNIP__QUEUELIST_P4_14__FIRST_QCW                                        (0x00000FFF)
#define U_PNIP__QUEUELIST_P4_14_RESET__VALUE                     0x0
#define U_PNIP__QUEUELIST_P4_15                                                   (0x10624040 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUELIST_P4_15__LAST_QCW                                         (0x0FFF0000)
#define U_PNIP__QUEUELIST_P4_15__FIRST_QCW                                        (0x00000FFF)
#define U_PNIP__QUEUELIST_P4_15_RESET__VALUE                     0x0
#define U_PNIP__DELAYLIST_P4                                                      (0x10624044 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DELAYLIST_P4__DELAYSTATUS                                         (0x80000000)
#define U_PNIP__DELAYLIST_P4__LAST_QCW                                            (0x0FFF0000)
#define U_PNIP__DELAYLIST_P4__FIRST_QCW                                           (0x00000FFF)
#define U_PNIP__DELAYLIST_P4_RESET__VALUE                        0x0
#define U_PNIP__HOL_STATUS_P4                                                     (0x10624100 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_STATUS_P4__HOL_MASK                                           (0xFFFF0000)
#define U_PNIP__HOL_STATUS_P4__HOL_EXPIRED                                        (0x0000FFFF)
#define U_PNIP__HOL_STATUS_P4_RESET__VALUE                       0x0
#define U_PNIP__HOL_COUNT_P4_1                                                    (0x10624104 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P4_1__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P4_1__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P4_1_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P4_2                                                    (0x10624108 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P4_2__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P4_2__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P4_2_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P4_3                                                    (0x1062410C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P4_3__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P4_3__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P4_3_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P4_4                                                    (0x10624110 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P4_4__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P4_4__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P4_4_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P4_5                                                    (0x10624114 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P4_5__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P4_5__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P4_5_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P4_6                                                    (0x10624118 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P4_6__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P4_6__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P4_6_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P4_7                                                    (0x1062411C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P4_7__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P4_7__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P4_7_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P4_8                                                    (0x10624120 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P4_8__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P4_8__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P4_8_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P4_9                                                    (0x10624124 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P4_9__RESOURCEOVERRUN                                   (0x80000000)
#define U_PNIP__HOL_COUNT_P4_9__RESOURCECOUNT                                     (0x00001FFF)
#define U_PNIP__HOL_COUNT_P4_9_RESET__VALUE                      0x0
#define U_PNIP__HOL_COUNT_P4_10                                                   (0x10624128 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P4_10__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_P4_10__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_P4_10_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_P4_11                                                   (0x1062412C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P4_11__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_P4_11__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_P4_11_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_P4_12                                                   (0x10624130 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P4_12__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_P4_12__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_P4_12_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_P4_13                                                   (0x10624134 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P4_13__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_P4_13__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_P4_13_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_P4_14                                                   (0x10624138 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P4_14__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_P4_14__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_P4_14_RESET__VALUE                     0x0
#define U_PNIP__HOL_COUNT_P4_15                                                   (0x1062413C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_P4_15__RESOURCEOVERRUN                                  (0x80000000)
#define U_PNIP__HOL_COUNT_P4_15__RESOURCECOUNT                                    (0x00001FFF)
#define U_PNIP__HOL_COUNT_P4_15_RESET__VALUE                     0x0
#define U_PNIP__QCW_STATUS_P4                                                     (0x10624200 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_STATUS_P4__QCW_EXPIRED                                        (0x00000FFF)
#define U_PNIP__QCW_STATUS_P4_RESET__VALUE                       0x0
#define U_PNIP__QCW_COUNT_P4_0                                                    (0x10624204 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P4_0__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P4_0__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P4_0__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P4_0_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P4_1                                                    (0x10624208 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P4_1__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P4_1__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P4_1__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P4_1_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P4_2                                                    (0x1062420C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P4_2__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P4_2__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P4_2__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P4_2_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P4_3                                                    (0x10624210 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P4_3__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P4_3__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P4_3__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P4_3_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P4_4                                                    (0x10624214 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P4_4__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P4_4__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P4_4__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P4_4_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P4_5                                                    (0x10624218 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P4_5__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P4_5__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P4_5__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P4_5_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P4_6                                                    (0x1062421C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P4_6__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P4_6__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P4_6__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P4_6_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P4_7                                                    (0x10624220 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P4_7__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P4_7__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P4_7__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P4_7_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P4_8                                                    (0x10624224 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P4_8__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P4_8__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P4_8__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P4_8_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P4_9                                                    (0x10624228 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P4_9__ENABLECOUNT                                       (0x80000000)
#define U_PNIP__QCW_COUNT_P4_9__QCWMODE                                           (0x40000000)
#define U_PNIP__QCW_COUNT_P4_9__QCWCOUNT                                          (0x00001FFF)
#define U_PNIP__QCW_COUNT_P4_9_RESET__VALUE                      0x0
#define U_PNIP__QCW_COUNT_P4_10                                                   (0x1062422C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P4_10__ENABLECOUNT                                      (0x80000000)
#define U_PNIP__QCW_COUNT_P4_10__QCWMODE                                          (0x40000000)
#define U_PNIP__QCW_COUNT_P4_10__QCWCOUNT                                         (0x00001FFF)
#define U_PNIP__QCW_COUNT_P4_10_RESET__VALUE                     0x0
#define U_PNIP__QCW_COUNT_P4_11                                                   (0x10624230 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_COUNT_P4_11__ENABLECOUNT                                      (0x80000000)
#define U_PNIP__QCW_COUNT_P4_11__QCWMODE                                          (0x40000000)
#define U_PNIP__QCW_COUNT_P4_11__QCWCOUNT                                         (0x00001FFF)
#define U_PNIP__QCW_COUNT_P4_11_RESET__VALUE                     0x0
#define U_PNIP__QCW_MAPPING_P4_0TO7                                               (0x10624234 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_MAPPING_P4_0TO7__QUEUE7_MAP                                   (0xF0000000)
#define U_PNIP__QCW_MAPPING_P4_0TO7__QUEUE6_MAP                                   (0x0F000000)
#define U_PNIP__QCW_MAPPING_P4_0TO7__QUEUE5_MAP                                   (0x00F00000)
#define U_PNIP__QCW_MAPPING_P4_0TO7__QUEUE4_MAP                                   (0x000F0000)
#define U_PNIP__QCW_MAPPING_P4_0TO7__QUEUE3_MAP                                   (0x0000F000)
#define U_PNIP__QCW_MAPPING_P4_0TO7__QUEUE2_MAP                                   (0x00000F00)
#define U_PNIP__QCW_MAPPING_P4_0TO7__QUEUE1_MAP                                   (0x000000F0)
#define U_PNIP__QCW_MAPPING_P4_0TO7__QUEUE0_MAP                                   (0x0000000F)
#define U_PNIP__QCW_MAPPING_P4_0TO7_RESET__VALUE                 0xFFFFFFFF
#define U_PNIP__QCW_MAPPING_P4_8TO11                                              (0x10624238 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_MAPPING_P4_8TO11__QUEUE11_MAP                                 (0x0000F000)
#define U_PNIP__QCW_MAPPING_P4_8TO11__QUEUE10_MAP                                 (0x00000F00)
#define U_PNIP__QCW_MAPPING_P4_8TO11__QUEUE9_MAP                                  (0x000000F0)
#define U_PNIP__QCW_MAPPING_P4_8TO11__QUEUE8_MAP                                  (0x0000000F)
#define U_PNIP__QCW_MAPPING_P4_8TO11_RESET__VALUE                0xFFFF
#define U_PNIP__FRAMEAGING_STATUS_P4                                              (0x10624300 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMEAGING_STATUS_P4__FRAMEAGING_EXPIRED                          (0x0000FFFF)
#define U_PNIP__FRAMEAGING_STATUS_P4_RESET__VALUE                0x0
#define U_PNIP__FRAMEAGING_TIME_P4                                                (0x10624304 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMEAGING_TIME_P4__AGINGTIME                                     (0x7FFFFFFF)
#define U_PNIP__FRAMEAGING_TIME_P4_RESET__VALUE                  0x0
#define U_PNIP__FRAMEAGING_CYCLE_P4                                               (0x10624308 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMEAGING_CYCLE_P4__AGINGCYCLE                                   (0x0000007F)
#define U_PNIP__FRAMEAGING_CYCLE_P4_RESET__VALUE                 0x0
#define U_PNIP__FREELIST                                                          (0x10626000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FREELIST__FREESTATUS                                              (0x80000000)
#define U_PNIP__FREELIST__LAST_QCW                                                (0x0FFF0000)
#define U_PNIP__FREELIST__FIRST_QCW                                               (0x00000FFF)
#define U_PNIP__FREELIST_RESET__VALUE                            0x822F0000
#define U_PNIP__HOL_COUNT_PA                                                      (0x10627000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_PA__RESOURCEOVERRUN                                     (0x80000000)
#define U_PNIP__HOL_COUNT_PA__RESOURCECOUNT                                       (0x00001FFF)
#define U_PNIP__HOL_COUNT_PA_RESET__VALUE                        0x0
#define U_PNIP__HOL_CONTROL_PA                                                    (0x10627004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_CONTROL_PA__HOL_MODE                                          (0x0000FFFE)
#define U_PNIP__HOL_CONTROL_PA_RESET__VALUE                      0x0
#define U_PNIP__HOL_BARRIER_PA_0                                                  (0x10627008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PA_0__RESOURCETYPE_X_BARRIER                          (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PA_0_RESET__VALUE                    0x230
#define U_PNIP__HOL_BARRIER_PA_1                                                  (0x1062700C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PA_1__RESOURCETYPE_X_BARRIER                          (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PA_1_RESET__VALUE                    0x230
#define U_PNIP__HOL_BARRIER_PA_2                                                  (0x10627010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PA_2__RESOURCETYPE_X_BARRIER                          (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PA_2_RESET__VALUE                    0x230
#define U_PNIP__HOL_BARRIER_PA_3                                                  (0x10627014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PA_3__RESOURCETYPE_X_BARRIER                          (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PA_3_RESET__VALUE                    0x230
#define U_PNIP__HOL_BARRIER_PA_4                                                  (0x10627018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PA_4__RESOURCETYPE_X_BARRIER                          (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PA_4_RESET__VALUE                    0x230
#define U_PNIP__HOL_BARRIER_PA_5                                                  (0x1062701C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PA_5__RESOURCETYPE_X_BARRIER                          (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PA_5_RESET__VALUE                    0x230
#define U_PNIP__HOL_BARRIER_PA_6                                                  (0x10627020 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PA_6__RESOURCETYPE_X_BARRIER                          (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PA_6_RESET__VALUE                    0x230
#define U_PNIP__HOL_BARRIER_PA_7                                                  (0x10627024 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PA_7__RESOURCETYPE_X_BARRIER                          (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PA_7_RESET__VALUE                    0x230
#define U_PNIP__HOL_BARRIER_PA_8                                                  (0x10627028 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PA_8__RESOURCETYPE_X_BARRIER                          (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PA_8_RESET__VALUE                    0x230
#define U_PNIP__HOL_BARRIER_PA_9                                                  (0x1062702C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PA_9__RESOURCETYPE_X_BARRIER                          (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PA_9_RESET__VALUE                    0x230
#define U_PNIP__HOL_BARRIER_PA_10                                                 (0x10627030 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PA_10__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PA_10_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_PA_11                                                 (0x10627034 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PA_11__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PA_11_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_PA_12                                                 (0x10627038 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PA_12__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PA_12_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_PA_13                                                 (0x1062703C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PA_13__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PA_13_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_PA_14                                                 (0x10627040 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PA_14__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PA_14_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_PA_15                                                 (0x10627044 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PA_15__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PA_15_RESET__VALUE                   0x230
#define U_PNIP__HOL_COUNT_PB                                                      (0x10628000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_COUNT_PB__RESOURCEOVERRUN                                     (0x80000000)
#define U_PNIP__HOL_COUNT_PB__RESOURCECOUNT                                       (0x00001FFF)
#define U_PNIP__HOL_COUNT_PB_RESET__VALUE                        0x0
#define U_PNIP__HOL_CONTROL_PB                                                    (0x10628004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_CONTROL_PB__HOL_MODE                                          (0x0000FFFE)
#define U_PNIP__HOL_CONTROL_PB_RESET__VALUE                      0x0
#define U_PNIP__HOL_BARRIER_PB_0                                                  (0x10628008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PB_0__RESOURCETYPE_X_BARRIER                          (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PB_0_RESET__VALUE                    0x230
#define U_PNIP__HOL_BARRIER_PB_1                                                  (0x1062800C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PB_1__RESOURCETYPE_X_BARRIER                          (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PB_1_RESET__VALUE                    0x230
#define U_PNIP__HOL_BARRIER_PB_2                                                  (0x10628010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PB_2__RESOURCETYPE_X_BARRIER                          (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PB_2_RESET__VALUE                    0x230
#define U_PNIP__HOL_BARRIER_PB_3                                                  (0x10628014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PB_3__RESOURCETYPE_X_BARRIER                          (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PB_3_RESET__VALUE                    0x230
#define U_PNIP__HOL_BARRIER_PB_4                                                  (0x10628018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PB_4__RESOURCETYPE_X_BARRIER                          (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PB_4_RESET__VALUE                    0x230
#define U_PNIP__HOL_BARRIER_PB_5                                                  (0x1062801C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PB_5__RESOURCETYPE_X_BARRIER                          (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PB_5_RESET__VALUE                    0x230
#define U_PNIP__HOL_BARRIER_PB_6                                                  (0x10628020 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PB_6__RESOURCETYPE_X_BARRIER                          (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PB_6_RESET__VALUE                    0x230
#define U_PNIP__HOL_BARRIER_PB_7                                                  (0x10628024 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PB_7__RESOURCETYPE_X_BARRIER                          (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PB_7_RESET__VALUE                    0x230
#define U_PNIP__HOL_BARRIER_PB_8                                                  (0x10628028 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PB_8__RESOURCETYPE_X_BARRIER                          (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PB_8_RESET__VALUE                    0x230
#define U_PNIP__HOL_BARRIER_PB_9                                                  (0x1062802C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PB_9__RESOURCETYPE_X_BARRIER                          (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PB_9_RESET__VALUE                    0x230
#define U_PNIP__HOL_BARRIER_PB_10                                                 (0x10628030 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PB_10__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PB_10_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_PB_11                                                 (0x10628034 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PB_11__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PB_11_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_PB_12                                                 (0x10628038 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PB_12__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PB_12_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_PB_13                                                 (0x1062803C + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PB_13__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PB_13_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_PB_14                                                 (0x10628040 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PB_14__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PB_14_RESET__VALUE                   0x230
#define U_PNIP__HOL_BARRIER_PB_15                                                 (0x10628044 + PNIP_DEV_TOP_BASE)
#define U_PNIP__HOL_BARRIER_PB_15__RESOURCETYPE_X_BARRIER                         (0x00001FFF)
#define U_PNIP__HOL_BARRIER_PB_15_RESET__VALUE                   0x230
#define U_PNIP__QCW_TRIGGER_PA                                                    (0x10629000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_TRIGGER_PA__QCWTRIGGER                                        (0x0FFFFFF8)
#define U_PNIP__QCW_TRIGGER_PA_RESET__VALUE                      0x0
#define U_PNIP__QCW_BARRIER_PA_0                                                  (0x10629004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_BARRIER_PA_0__QCWBARRIER                                      (0x00001FFF)
#define U_PNIP__QCW_BARRIER_PA_0_RESET__VALUE                    0x230
#define U_PNIP__QCW_BARRIER_PA_1                                                  (0x10629008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_BARRIER_PA_1__QCWBARRIER                                      (0x00001FFF)
#define U_PNIP__QCW_BARRIER_PA_1_RESET__VALUE                    0x230
#define U_PNIP__QCW_BARRIER_PA_2                                                  (0x1062900C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_BARRIER_PA_2__QCWBARRIER                                      (0x00001FFF)
#define U_PNIP__QCW_BARRIER_PA_2_RESET__VALUE                    0x230
#define U_PNIP__QCW_BARRIER_PA_3                                                  (0x10629010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_BARRIER_PA_3__QCWBARRIER                                      (0x00001FFF)
#define U_PNIP__QCW_BARRIER_PA_3_RESET__VALUE                    0x230
#define U_PNIP__QCW_BARRIER_PA_4                                                  (0x10629014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_BARRIER_PA_4__QCWBARRIER                                      (0x00001FFF)
#define U_PNIP__QCW_BARRIER_PA_4_RESET__VALUE                    0x230
#define U_PNIP__QCW_BARRIER_PA_5                                                  (0x10629018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_BARRIER_PA_5__QCWBARRIER                                      (0x00001FFF)
#define U_PNIP__QCW_BARRIER_PA_5_RESET__VALUE                    0x230
#define U_PNIP__QCW_BARRIER_PA_6                                                  (0x1062901C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_BARRIER_PA_6__QCWBARRIER                                      (0x00001FFF)
#define U_PNIP__QCW_BARRIER_PA_6_RESET__VALUE                    0x230
#define U_PNIP__QCW_BARRIER_PA_7                                                  (0x10629020 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_BARRIER_PA_7__QCWBARRIER                                      (0x00001FFF)
#define U_PNIP__QCW_BARRIER_PA_7_RESET__VALUE                    0x230
#define U_PNIP__QCW_BARRIER_PA_8                                                  (0x10629024 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_BARRIER_PA_8__QCWBARRIER                                      (0x00001FFF)
#define U_PNIP__QCW_BARRIER_PA_8_RESET__VALUE                    0x230
#define U_PNIP__QCW_BARRIER_PA_9                                                  (0x10629028 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_BARRIER_PA_9__QCWBARRIER                                      (0x00001FFF)
#define U_PNIP__QCW_BARRIER_PA_9_RESET__VALUE                    0x230
#define U_PNIP__QCW_BARRIER_PA_10                                                 (0x1062902C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_BARRIER_PA_10__QCWBARRIER                                     (0x00001FFF)
#define U_PNIP__QCW_BARRIER_PA_10_RESET__VALUE                   0x230
#define U_PNIP__QCW_BARRIER_PA_11                                                 (0x10629030 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_BARRIER_PA_11__QCWBARRIER                                     (0x00001FFF)
#define U_PNIP__QCW_BARRIER_PA_11_RESET__VALUE                   0x230
#define U_PNIP__QCW_TRIGGER_PB                                                    (0x1062A000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_TRIGGER_PB__QCWTRIGGER                                        (0x0FFFFFF8)
#define U_PNIP__QCW_TRIGGER_PB_RESET__VALUE                      0x0
#define U_PNIP__QCW_BARRIER_PB_0                                                  (0x1062A004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_BARRIER_PB_0__QCWBARRIER                                      (0x00001FFF)
#define U_PNIP__QCW_BARRIER_PB_0_RESET__VALUE                    0x230
#define U_PNIP__QCW_BARRIER_PB_1                                                  (0x1062A008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_BARRIER_PB_1__QCWBARRIER                                      (0x00001FFF)
#define U_PNIP__QCW_BARRIER_PB_1_RESET__VALUE                    0x230
#define U_PNIP__QCW_BARRIER_PB_2                                                  (0x1062A00C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_BARRIER_PB_2__QCWBARRIER                                      (0x00001FFF)
#define U_PNIP__QCW_BARRIER_PB_2_RESET__VALUE                    0x230
#define U_PNIP__QCW_BARRIER_PB_3                                                  (0x1062A010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_BARRIER_PB_3__QCWBARRIER                                      (0x00001FFF)
#define U_PNIP__QCW_BARRIER_PB_3_RESET__VALUE                    0x230
#define U_PNIP__QCW_BARRIER_PB_4                                                  (0x1062A014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_BARRIER_PB_4__QCWBARRIER                                      (0x00001FFF)
#define U_PNIP__QCW_BARRIER_PB_4_RESET__VALUE                    0x230
#define U_PNIP__QCW_BARRIER_PB_5                                                  (0x1062A018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_BARRIER_PB_5__QCWBARRIER                                      (0x00001FFF)
#define U_PNIP__QCW_BARRIER_PB_5_RESET__VALUE                    0x230
#define U_PNIP__QCW_BARRIER_PB_6                                                  (0x1062A01C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_BARRIER_PB_6__QCWBARRIER                                      (0x00001FFF)
#define U_PNIP__QCW_BARRIER_PB_6_RESET__VALUE                    0x230
#define U_PNIP__QCW_BARRIER_PB_7                                                  (0x1062A020 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_BARRIER_PB_7__QCWBARRIER                                      (0x00001FFF)
#define U_PNIP__QCW_BARRIER_PB_7_RESET__VALUE                    0x230
#define U_PNIP__QCW_BARRIER_PB_8                                                  (0x1062A024 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_BARRIER_PB_8__QCWBARRIER                                      (0x00001FFF)
#define U_PNIP__QCW_BARRIER_PB_8_RESET__VALUE                    0x230
#define U_PNIP__QCW_BARRIER_PB_9                                                  (0x1062A028 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_BARRIER_PB_9__QCWBARRIER                                      (0x00001FFF)
#define U_PNIP__QCW_BARRIER_PB_9_RESET__VALUE                    0x230
#define U_PNIP__QCW_BARRIER_PB_10                                                 (0x1062A02C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_BARRIER_PB_10__QCWBARRIER                                     (0x00001FFF)
#define U_PNIP__QCW_BARRIER_PB_10_RESET__VALUE                   0x230
#define U_PNIP__QCW_BARRIER_PB_11                                                 (0x1062A030 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QCW_BARRIER_PB_11__QCWBARRIER                                     (0x00001FFF)
#define U_PNIP__QCW_BARRIER_PB_11_RESET__VALUE                   0x230
#define U_PNIP__RCT_DELAY                                                         (0x10630000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RCT_DELAY__RATECOMPENSATIONTIMER                                  (0xFFFFFFFF)
#define U_PNIP__RCT_DELAY_RESET__VALUE                           0x0
#define U_PNIP__RCF_DELAY                                                         (0x10630004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RCF_DELAY__RCF_VALID                                              (0x80000000)
#define U_PNIP__RCF_DELAY__RATECOMPENSATIONINCREMENT                              (0x40000000)
#define U_PNIP__RCF_DELAY__RATECOMPENSATIONFACTOR                                 (0x3FFFFFFF)
#define U_PNIP__RCF_DELAY_RESET__VALUE                           0x0
#define U_PNIP__OCTLOW_TIME                                                       (0x10630100 + PNIP_DEV_TOP_BASE)
#define U_PNIP__OCTLOW_TIME__OFFSETCOMPENSATIONTIMER_LOW                          (0xFFFFFFFF)
#define U_PNIP__OCTLOW_TIME_RESET__VALUE                         0x0
#define U_PNIP__OCTHIGH_TIME                                                      (0x10630104 + PNIP_DEV_TOP_BASE)
#define U_PNIP__OCTHIGH_TIME__OFFSETCOMPENSATIONTIMER_HIGH                        (0xFFFFFFFF)
#define U_PNIP__OCTHIGH_TIME_RESET__VALUE                        0x0
#define U_PNIP__OCF_TIME                                                          (0x10630108 + PNIP_DEV_TOP_BASE)
#define U_PNIP__OCF_TIME__OCF_VALID                                               (0x80000000)
#define U_PNIP__OCF_TIME__OFFSETCOMPENSATIONINCREMENT                             (0x40000000)
#define U_PNIP__OCF_TIME__OFFSETCOMPENSATIONFACTOR                                (0x3FFFFFFF)
#define U_PNIP__OCF_TIME_RESET__VALUE                            0x0
#define U_PNIP__RCT_TIME                                                          (0x1063010C + PNIP_DEV_TOP_BASE)
#define U_PNIP__RCT_TIME__RATECOMPENSATIONTIMER                                   (0xFFFFFFFF)
#define U_PNIP__RCT_TIME_RESET__VALUE                            0x0
#define U_PNIP__RCF_TIME                                                          (0x10630110 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RCF_TIME__RCF_VALID                                               (0x80000000)
#define U_PNIP__RCF_TIME__RATECOMPENSATIONINCREMENT                               (0x40000000)
#define U_PNIP__RCF_TIME__RATECOMPENSATIONFACTOR                                  (0x3FFFFFFF)
#define U_PNIP__RCF_TIME_RESET__VALUE                            0x0
#define U_PNIP__SYNCTIMER                                                         (0x10630114 + PNIP_DEV_TOP_BASE)
#define U_PNIP__SYNCTIMER__SYNCHRONISATIONTIMER                                   (0xFFFFFFFF)
#define U_PNIP__SYNCTIMER_RESET__VALUE                           0x0
#define U_PNIP__SYNCTIMERLENGTH                                                   (0x10630118 + PNIP_DEV_TOP_BASE)
#define U_PNIP__SYNCTIMERLENGTH__SYNCHRONISATIONTIMERLENGTH                       (0xFFFFFFFF)
#define U_PNIP__SYNCTIMERLENGTH_RESET__VALUE                     0x0
#define U_PNIP__SYNCTIMERCOMPARE                                                  (0x1063011C + PNIP_DEV_TOP_BASE)
#define U_PNIP__SYNCTIMERCOMPARE__SYNCHRONISATIONCOMPARATOR                       (0xFFFFFFFF)
#define U_PNIP__SYNCTIMERCOMPARE_RESET__VALUE                    0x0
#define U_PNIP__PLL_OUT_CONTROL_TIME                                              (0x10630120 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PLL_OUT_CONTROL_TIME__PLL_LENGTH                                  (0x000000FE)
#define U_PNIP__PLL_OUT_CONTROL_TIME__PLL_LEVEL                                   (0x00000001)
#define U_PNIP__PLL_OUT_CONTROL_TIME_RESET__VALUE                0x1
#define U_PNIP__PLL_IN_CONTROL_TIME                                               (0x10630124 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PLL_IN_CONTROL_TIME__PLL_EDGE                                     (0x00000100)
#define U_PNIP__PLL_IN_CONTROL_TIME__PLL_SELECT                                   (0x00000003)
#define U_PNIP__PLL_IN_CONTROL_TIME_RESET__VALUE                 0x100
#define U_PNIP__OCTLOW_STAMP_TIME                                                 (0x10630128 + PNIP_DEV_TOP_BASE)
#define U_PNIP__OCTLOW_STAMP_TIME__OFFSETCOMPENSATIONSTAMP                        (0xFFFFFFFF)
#define U_PNIP__OCTLOW_STAMP_TIME_RESET__VALUE                   0x0
#define U_PNIP__OCTHIGH_STAMP_TIME                                                (0x1063012C + PNIP_DEV_TOP_BASE)
#define U_PNIP__OCTHIGH_STAMP_TIME__OFFSETCOMPENSATIONSTAMP                       (0xFFFFFFFF)
#define U_PNIP__OCTHIGH_STAMP_TIME_RESET__VALUE                  0x0
#define U_PNIP__PTCPNANOSEC_TIME                                                  (0x10630130 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCPNANOSEC_TIME__PTCPNANOSECONDS                                 (0xFFFFFFFF)
#define U_PNIP__PTCPNANOSEC_TIME_RESET__VALUE                    0x0
#define U_PNIP__PTCPSEC_TIME                                                      (0x10630134 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCPSEC_TIME__PTCPSECONDS                                         (0xFFFFFFFF)
#define U_PNIP__PTCPSEC_TIME_RESET__VALUE                        0x0
#define U_PNIP__OCTLOW_CLOCK_A                                                    (0x10630200 + PNIP_DEV_TOP_BASE)
#define U_PNIP__OCTLOW_CLOCK_A__OFFSETCOMPENSATIONTIMER                           (0xFFFFFFFF)
#define U_PNIP__OCTLOW_CLOCK_A_RESET__VALUE                      0x0
#define U_PNIP__OCTHIGH_CLOCK_A                                                   (0x10630204 + PNIP_DEV_TOP_BASE)
#define U_PNIP__OCTHIGH_CLOCK_A__OFFSETCOMPENSATIONTIMER                          (0xFFFFFFFF)
#define U_PNIP__OCTHIGH_CLOCK_A_RESET__VALUE                     0x0
#define U_PNIP__OCF_CLOCK_A                                                       (0x10630208 + PNIP_DEV_TOP_BASE)
#define U_PNIP__OCF_CLOCK_A__OCF_VALID                                            (0x80000000)
#define U_PNIP__OCF_CLOCK_A__OFFSETCOMPENSATIONINCREMENT                          (0x40000000)
#define U_PNIP__OCF_CLOCK_A__OFFSETCOMPENSATIONFACTOR                             (0x3FFFFFFF)
#define U_PNIP__OCF_CLOCK_A_RESET__VALUE                         0x0
#define U_PNIP__RCT_CLOCK_A                                                       (0x1063020C + PNIP_DEV_TOP_BASE)
#define U_PNIP__RCT_CLOCK_A__RATECOMPENSATIONTIMER                                (0xFFFFFFFF)
#define U_PNIP__RCT_CLOCK_A_RESET__VALUE                         0x0
#define U_PNIP__RCF_CLOCK_A                                                       (0x10630210 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RCF_CLOCK_A__RCF_VALID                                            (0x80000000)
#define U_PNIP__RCF_CLOCK_A__RATECOMPENSATIONINCREMENT                            (0x40000000)
#define U_PNIP__RCF_CLOCK_A__RATECOMPENSATIONFACTOR                               (0x3FFFFFFF)
#define U_PNIP__RCF_CLOCK_A_RESET__VALUE                         0x0
#define U_PNIP__CYCLE_A                                                           (0x10630214 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CYCLE_A__COMMUNICATIONTIMER                                       (0xFFFFFFFF)
#define U_PNIP__CYCLE_A_RESET__VALUE                             0x0
#define U_PNIP__CYCLENUMBER_A                                                     (0x10630218 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CYCLENUMBER_A__COMMUNICATIONCYCLE                                 (0xFFFFFFFF)
#define U_PNIP__CYCLENUMBER_A_RESET__VALUE                       0x0
#define U_PNIP__CYCLELENGTH_A                                                     (0x1063021C + PNIP_DEV_TOP_BASE)
#define U_PNIP__CYCLELENGTH_A__COMMUNICATIONLENGTH                                (0xFFFFFFFF)
#define U_PNIP__CYCLELENGTH_A_RESET__VALUE                       0xF4240
#define U_PNIP__SENDCYCLE_A                                                       (0x10630220 + PNIP_DEV_TOP_BASE)
#define U_PNIP__SENDCYCLE_A__SENDCYCLE                                            (0x000000FF)
#define U_PNIP__SENDCYCLE_A_RESET__VALUE                         0x0
#define U_PNIP__BASECYCLE_A                                                       (0x10630224 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BASECYCLE_A__BASETIMER                                            (0x00007FFF)
#define U_PNIP__BASECYCLE_A_RESET__VALUE                         0x1F40
#define U_PNIP__CYCLECOUNTER_A                                                    (0x10630228 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CYCLECOUNTER_A__CYCLECOUNTER                                      (0xFFFFFFFF)
#define U_PNIP__CYCLECOUNTER_A_RESET__VALUE                      0x0
#define U_PNIP__CCOUNTER_VALIDAREA_A                                              (0x1063022C + PNIP_DEV_TOP_BASE)
#define U_PNIP__CCOUNTER_VALIDAREA_A__CC_VALID_AREA                               (0x0000FFFF)
#define U_PNIP__CCOUNTER_VALIDAREA_A_RESET__VALUE                0x8000
#define U_PNIP__RT_CCOUNTER_A                                                     (0x10630230 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RT_CCOUNTER_A__RT_CYCLENO                                         (0x0000FFFF)
#define U_PNIP__RT_CCOUNTER_A_RESET__VALUE                       0x0
#define U_PNIP__RTOVERUDP_CCOUNTER_A                                              (0x10630234 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RTOVERUDP_CCOUNTER_A__RTOVERUDP_CYCLENO                           (0x0000FFFF)
#define U_PNIP__RTOVERUDP_CCOUNTER_A_RESET__VALUE                0x0
#define U_PNIP__SYNCCYCLE_A                                                       (0x10630238 + PNIP_DEV_TOP_BASE)
#define U_PNIP__SYNCCYCLE_A__SYNCTIMER                                            (0xFFFFFFFF)
#define U_PNIP__SYNCCYCLE_A_RESET__VALUE                         0x0
#define U_PNIP__SYNCLENGTH_A                                                      (0x1063023C + PNIP_DEV_TOP_BASE)
#define U_PNIP__SYNCLENGTH_A__SYNCHRONISATIONLENGTH                               (0xFFFFFFFF)
#define U_PNIP__SYNCLENGTH_A_RESET__VALUE                        0x0
#define U_PNIP__SYNCCOMPARE_A                                                     (0x10630240 + PNIP_DEV_TOP_BASE)
#define U_PNIP__SYNCCOMPARE_A__SYNCHRONISATIONCOMPARATOR                          (0xFFFFFFFF)
#define U_PNIP__SYNCCOMPARE_A_RESET__VALUE                       0x0
#define U_PNIP__PPM_STARTTIME_A                                                   (0x10630244 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PPM_STARTTIME_A__PPM_STARTTIME                                    (0x00007FFF)
#define U_PNIP__PPM_STARTTIME_A_RESET__VALUE                     0x1F40
#define U_PNIP__PLL_IN_CONTROL_A                                                  (0x10630280 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PLL_IN_CONTROL_A__PLL_EDGE                                        (0x00000100)
#define U_PNIP__PLL_IN_CONTROL_A__PLL_SELECT                                      (0x00000003)
#define U_PNIP__PLL_IN_CONTROL_A_RESET__VALUE                    0x100
#define U_PNIP__OCTLOW_STAMP_A                                                    (0x10630284 + PNIP_DEV_TOP_BASE)
#define U_PNIP__OCTLOW_STAMP_A__OFFSETCOMPENSATIONSTAMP                           (0xFFFFFFFF)
#define U_PNIP__OCTLOW_STAMP_A_RESET__VALUE                      0x0
#define U_PNIP__OCTHIGH_STAMP_A                                                   (0x10630288 + PNIP_DEV_TOP_BASE)
#define U_PNIP__OCTHIGH_STAMP_A__OFFSETCOMPENSATIONSTAMP                          (0xFFFFFFFF)
#define U_PNIP__OCTHIGH_STAMP_A_RESET__VALUE                     0x0
#define U_PNIP__PTCPNANOSEC_A                                                     (0x1063028C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCPNANOSEC_A__PTCPNANOSECONDS                                    (0xFFFFFFFF)
#define U_PNIP__PTCPNANOSEC_A_RESET__VALUE                       0x0
#define U_PNIP__PTCPSEC_A                                                         (0x10630290 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCPSEC_A__PTCPSECONDS                                            (0xFFFFFFFF)
#define U_PNIP__PTCPSEC_A_RESET__VALUE                           0x0
#define U_PNIP__CYCLECOMPARE_A_1                                                  (0x106302C0 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CYCLECOMPARE_A_1__COMMUNICATIONCOMPARATOR                         (0xFFFFFFFF)
#define U_PNIP__CYCLECOMPARE_A_1_RESET__VALUE                    0x0
#define U_PNIP__CYCLECOMPARE_A_2                                                  (0x106302C4 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CYCLECOMPARE_A_2__COMMUNICATIONCOMPARATOR                         (0xFFFFFFFF)
#define U_PNIP__CYCLECOMPARE_A_2_RESET__VALUE                    0x0
#define U_PNIP__CYCLECOMPARE_A_3                                                  (0x106302C8 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CYCLECOMPARE_A_3__COMMUNICATIONCOMPARATOR                         (0xFFFFFFFF)
#define U_PNIP__CYCLECOMPARE_A_3_RESET__VALUE                    0x0
#define U_PNIP__CYCLECOMPARE_A_4                                                  (0x106302CC + PNIP_DEV_TOP_BASE)
#define U_PNIP__CYCLECOMPARE_A_4__COMMUNICATIONCOMPARATOR                         (0xFFFFFFFF)
#define U_PNIP__CYCLECOMPARE_A_4_RESET__VALUE                    0x0
#define U_PNIP__CYCLECOMPARE_A_5                                                  (0x106302D0 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CYCLECOMPARE_A_5__COMMUNICATIONCOMPARATOR                         (0xFFFFFFFF)
#define U_PNIP__CYCLECOMPARE_A_5_RESET__VALUE                    0x0
#define U_PNIP__OCTLOW_CLOCK_B                                                    (0x10630300 + PNIP_DEV_TOP_BASE)
#define U_PNIP__OCTLOW_CLOCK_B__OFFSETCOMPENSATIONTIMER                           (0xFFFFFFFF)
#define U_PNIP__OCTLOW_CLOCK_B_RESET__VALUE                      0x0
#define U_PNIP__OCTHIGH_CLOCK_B                                                   (0x10630304 + PNIP_DEV_TOP_BASE)
#define U_PNIP__OCTHIGH_CLOCK_B__OFFSETCOMPENSATIONTIMER                          (0xFFFFFFFF)
#define U_PNIP__OCTHIGH_CLOCK_B_RESET__VALUE                     0x0
#define U_PNIP__OCF_CLOCK_B                                                       (0x10630308 + PNIP_DEV_TOP_BASE)
#define U_PNIP__OCF_CLOCK_B__OCF_VALID                                            (0x80000000)
#define U_PNIP__OCF_CLOCK_B__OFFSETCOMPENSATIONINCREMENT                          (0x40000000)
#define U_PNIP__OCF_CLOCK_B__OFFSETCOMPENSATIONFACTOR                             (0x3FFFFFFF)
#define U_PNIP__OCF_CLOCK_B_RESET__VALUE                         0x0
#define U_PNIP__RCT_CLOCK_B                                                       (0x1063030C + PNIP_DEV_TOP_BASE)
#define U_PNIP__RCT_CLOCK_B__RATECOMPENSATIONTIMER                                (0xFFFFFFFF)
#define U_PNIP__RCT_CLOCK_B_RESET__VALUE                         0x0
#define U_PNIP__RCF_CLOCK_B                                                       (0x10630310 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RCF_CLOCK_B__RCF_VALID                                            (0x80000000)
#define U_PNIP__RCF_CLOCK_B__RATECOMPENSATIONINCREMENT                            (0x40000000)
#define U_PNIP__RCF_CLOCK_B__RATECOMPENSATIONFACTOR                               (0x3FFFFFFF)
#define U_PNIP__RCF_CLOCK_B_RESET__VALUE                         0x0
#define U_PNIP__CYCLE_B                                                           (0x10630314 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CYCLE_B__COMMUNICATIONTIMER                                       (0xFFFFFFFF)
#define U_PNIP__CYCLE_B_RESET__VALUE                             0x0
#define U_PNIP__CYCLENUMBER_B                                                     (0x10630318 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CYCLENUMBER_B__COMMUNICATIONCYCLE                                 (0xFFFFFFFF)
#define U_PNIP__CYCLENUMBER_B_RESET__VALUE                       0x0
#define U_PNIP__CYCLELENGTH_B                                                     (0x1063031C + PNIP_DEV_TOP_BASE)
#define U_PNIP__CYCLELENGTH_B__COMMUNICATIONLENGTH                                (0xFFFFFFFF)
#define U_PNIP__CYCLELENGTH_B_RESET__VALUE                       0xF4240
#define U_PNIP__SENDCYCLE_B                                                       (0x10630320 + PNIP_DEV_TOP_BASE)
#define U_PNIP__SENDCYCLE_B__SENDCYCLE                                            (0x000000FF)
#define U_PNIP__SENDCYCLE_B_RESET__VALUE                         0x0
#define U_PNIP__BASECYCLE_B                                                       (0x10630324 + PNIP_DEV_TOP_BASE)
#define U_PNIP__BASECYCLE_B__BASETIMER                                            (0x00007FFF)
#define U_PNIP__BASECYCLE_B_RESET__VALUE                         0x1F40
#define U_PNIP__CYCLECOUNTER_B                                                    (0x10630328 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CYCLECOUNTER_B__CYCLECOUNTER                                      (0xFFFFFFFF)
#define U_PNIP__CYCLECOUNTER_B_RESET__VALUE                      0x0
#define U_PNIP__CCOUNTER_VALIDAREA_B                                              (0x1063032C + PNIP_DEV_TOP_BASE)
#define U_PNIP__CCOUNTER_VALIDAREA_B__CC_VALID_AREA                               (0x0000FFFF)
#define U_PNIP__CCOUNTER_VALIDAREA_B_RESET__VALUE                0x8000
#define U_PNIP__RT_CCOUNTER_B                                                     (0x10630330 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RT_CCOUNTER_B__RT_CYCLENO                                         (0x0000FFFF)
#define U_PNIP__RT_CCOUNTER_B_RESET__VALUE                       0x0
#define U_PNIP__RTOVERUDP_CCOUNTER_B                                              (0x10630334 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RTOVERUDP_CCOUNTER_B__RTOVERUDP_CYCLENO                           (0x0000FFFF)
#define U_PNIP__RTOVERUDP_CCOUNTER_B_RESET__VALUE                0x0
#define U_PNIP__SYNCCYCLE_B                                                       (0x10630338 + PNIP_DEV_TOP_BASE)
#define U_PNIP__SYNCCYCLE_B__SYNCTIMER                                            (0xFFFFFFFF)
#define U_PNIP__SYNCCYCLE_B_RESET__VALUE                         0x0
#define U_PNIP__SYNCLENGTH_B                                                      (0x1063033C + PNIP_DEV_TOP_BASE)
#define U_PNIP__SYNCLENGTH_B__SYNCHRONISATIONLENGTH                               (0xFFFFFFFF)
#define U_PNIP__SYNCLENGTH_B_RESET__VALUE                        0x0
#define U_PNIP__SYNCCOMPARE_B                                                     (0x10630340 + PNIP_DEV_TOP_BASE)
#define U_PNIP__SYNCCOMPARE_B__SYNCHRONISATIONCOMPARATOR                          (0xFFFFFFFF)
#define U_PNIP__SYNCCOMPARE_B_RESET__VALUE                       0x0
#define U_PNIP__PPM_STARTTIME_B                                                   (0x10630344 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PPM_STARTTIME_B__PPM_STARTTIME                                    (0x00007FFF)
#define U_PNIP__PPM_STARTTIME_B_RESET__VALUE                     0x1F40
#define U_PNIP__PLL_IN_CONTROL_B                                                  (0x10630380 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PLL_IN_CONTROL_B__PLL_EDGE                                        (0x00000100)
#define U_PNIP__PLL_IN_CONTROL_B__PLL_SELECT                                      (0x00000003)
#define U_PNIP__PLL_IN_CONTROL_B_RESET__VALUE                    0x100
#define U_PNIP__OCTLOW_STAMP_B                                                    (0x10630384 + PNIP_DEV_TOP_BASE)
#define U_PNIP__OCTLOW_STAMP_B__OFFSETCOMPENSATIONSTAMP                           (0xFFFFFFFF)
#define U_PNIP__OCTLOW_STAMP_B_RESET__VALUE                      0x0
#define U_PNIP__OCTHIGH_STAMP_B                                                   (0x10630388 + PNIP_DEV_TOP_BASE)
#define U_PNIP__OCTHIGH_STAMP_B__OFFSETCOMPENSATIONSTAMP                          (0xFFFFFFFF)
#define U_PNIP__OCTHIGH_STAMP_B_RESET__VALUE                     0x0
#define U_PNIP__PTCPNANOSEC_B                                                     (0x1063038C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCPNANOSEC_B__PTCPNANOSECONDS                                    (0xFFFFFFFF)
#define U_PNIP__PTCPNANOSEC_B_RESET__VALUE                       0x0
#define U_PNIP__PTCPSEC_B                                                         (0x10630390 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PTCPSEC_B__PTCPSECONDS                                            (0xFFFFFFFF)
#define U_PNIP__PTCPSEC_B_RESET__VALUE                           0x0
#define U_PNIP__CYCLECOMPARE_B_1                                                  (0x106303C0 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CYCLECOMPARE_B_1__COMMUNICATIONCOMPARATOR                         (0xFFFFFFFF)
#define U_PNIP__CYCLECOMPARE_B_1_RESET__VALUE                    0x0
#define U_PNIP__CYCLECOMPARE_B_2                                                  (0x106303C4 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CYCLECOMPARE_B_2__COMMUNICATIONCOMPARATOR                         (0xFFFFFFFF)
#define U_PNIP__CYCLECOMPARE_B_2_RESET__VALUE                    0x0
#define U_PNIP__CYCLECOMPARE_B_3                                                  (0x106303C8 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CYCLECOMPARE_B_3__COMMUNICATIONCOMPARATOR                         (0xFFFFFFFF)
#define U_PNIP__CYCLECOMPARE_B_3_RESET__VALUE                    0x0
#define U_PNIP__CYCLECOMPARE_B_4                                                  (0x106303CC + PNIP_DEV_TOP_BASE)
#define U_PNIP__CYCLECOMPARE_B_4__COMMUNICATIONCOMPARATOR                         (0xFFFFFFFF)
#define U_PNIP__CYCLECOMPARE_B_4_RESET__VALUE                    0x0
#define U_PNIP__CYCLECOMPARE_B_5                                                  (0x106303D0 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CYCLECOMPARE_B_5__COMMUNICATIONCOMPARATOR                         (0xFFFFFFFF)
#define U_PNIP__CYCLECOMPARE_B_5_RESET__VALUE                    0x0
#define U_PNIP__APPLSTATUS                                                        (0x10631000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLSTATUS__APPLICATIONSTATUS_3                                   (0x00030000)
#define U_PNIP__APPLSTATUS__APPLICATIONSTATUS_2                                   (0x00000300)
#define U_PNIP__APPLSTATUS__APPLICATIONSTATUS_1                                   (0x00000003)
#define U_PNIP__APPLSTATUS_RESET__VALUE                          0x0
#define U_PNIP__APPLTIMER_1                                                       (0x10631100 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLTIMER_1__APPLICATIONTIMER                                     (0xFFFFFFFF)
#define U_PNIP__APPLTIMER_1_RESET__VALUE                         0x0
#define U_PNIP__APPLLENGTH_1                                                      (0x10631104 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLLENGTH_1__APPLICATIONLENGTH                                   (0xFFFFFFFF)
#define U_PNIP__APPLLENGTH_1_RESET__VALUE                        0x0
#define U_PNIP__APPLRCF_1                                                         (0x10631108 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLRCF_1__RCF_VALID                                              (0x80000000)
#define U_PNIP__APPLRCF_1__RATECOMPENSATIONINCREMENT                              (0x40000000)
#define U_PNIP__APPLRCF_1__RATECOMPENSATIONFACTOR                                 (0x3FFFFFFF)
#define U_PNIP__APPLRCF_1_RESET__VALUE                           0x0
#define U_PNIP__APPLCOMPARE_1_1                                                   (0x1063110C + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLCOMPARE_1_1__APPLICATIONCOMPARATOR                            (0xFFFFFFFF)
#define U_PNIP__APPLCOMPARE_1_1_RESET__VALUE                     0x0
#define U_PNIP__APPLCOMPARE_1_2                                                   (0x10631110 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLCOMPARE_1_2__APPLICATIONCOMPARATOR                            (0xFFFFFFFF)
#define U_PNIP__APPLCOMPARE_1_2_RESET__VALUE                     0x0
#define U_PNIP__APPLCOMPARE_1_3                                                   (0x10631114 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLCOMPARE_1_3__APPLICATIONCOMPARATOR                            (0xFFFFFFFF)
#define U_PNIP__APPLCOMPARE_1_3_RESET__VALUE                     0x0
#define U_PNIP__APPLCOMPARE_1_4                                                   (0x10631118 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLCOMPARE_1_4__APPLICATIONCOMPARATOR                            (0xFFFFFFFF)
#define U_PNIP__APPLCOMPARE_1_4_RESET__VALUE                     0x0
#define U_PNIP__APPLCOMPARE_1_5                                                   (0x1063111C + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLCOMPARE_1_5__APPLICATIONCOMPARATOR                            (0xFFFFFFFF)
#define U_PNIP__APPLCOMPARE_1_5_RESET__VALUE                     0x0
#define U_PNIP__APPLCOMPARE_1_6                                                   (0x10631120 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLCOMPARE_1_6__APPLICATIONCOMPARATOR                            (0xFFFFFFFF)
#define U_PNIP__APPLCOMPARE_1_6_RESET__VALUE                     0x0
#define U_PNIP__APPLCOMPARE_1_7                                                   (0x10631124 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLCOMPARE_1_7__APPLICATIONCOMPARATOR                            (0xFFFFFFFF)
#define U_PNIP__APPLCOMPARE_1_7_RESET__VALUE                     0x0
#define U_PNIP__APPLTIMER_2                                                       (0x10631200 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLTIMER_2__APPLICATIONTIMER                                     (0xFFFFFFFF)
#define U_PNIP__APPLTIMER_2_RESET__VALUE                         0x0
#define U_PNIP__APPLLENGTH_2                                                      (0x10631204 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLLENGTH_2__APPLICATIONLENGTH                                   (0xFFFFFFFF)
#define U_PNIP__APPLLENGTH_2_RESET__VALUE                        0x0
#define U_PNIP__APPLRCF_2                                                         (0x10631208 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLRCF_2__RCF_VALID                                              (0x80000000)
#define U_PNIP__APPLRCF_2__RATECOMPENSATIONINCREMENT                              (0x40000000)
#define U_PNIP__APPLRCF_2__RATECOMPENSATIONFACTOR                                 (0x3FFFFFFF)
#define U_PNIP__APPLRCF_2_RESET__VALUE                           0x0
#define U_PNIP__APPLCOMPARE_2_1                                                   (0x1063120C + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLCOMPARE_2_1__APPLICATIONCOMPARATOR                            (0xFFFFFFFF)
#define U_PNIP__APPLCOMPARE_2_1_RESET__VALUE                     0x0
#define U_PNIP__APPLCOMPARE_2_2                                                   (0x10631210 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLCOMPARE_2_2__APPLICATIONCOMPARATOR                            (0xFFFFFFFF)
#define U_PNIP__APPLCOMPARE_2_2_RESET__VALUE                     0x0
#define U_PNIP__APPLCOMPARE_2_3                                                   (0x10631214 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLCOMPARE_2_3__APPLICATIONCOMPARATOR                            (0xFFFFFFFF)
#define U_PNIP__APPLCOMPARE_2_3_RESET__VALUE                     0x0
#define U_PNIP__APPLCOMPARE_2_4                                                   (0x10631218 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLCOMPARE_2_4__APPLICATIONCOMPARATOR                            (0xFFFFFFFF)
#define U_PNIP__APPLCOMPARE_2_4_RESET__VALUE                     0x0
#define U_PNIP__APPLCOMPARE_2_5                                                   (0x1063121C + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLCOMPARE_2_5__APPLICATIONCOMPARATOR                            (0xFFFFFFFF)
#define U_PNIP__APPLCOMPARE_2_5_RESET__VALUE                     0x0
#define U_PNIP__APPLCOMPARE_2_6                                                   (0x10631220 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLCOMPARE_2_6__APPLICATIONCOMPARATOR                            (0xFFFFFFFF)
#define U_PNIP__APPLCOMPARE_2_6_RESET__VALUE                     0x0
#define U_PNIP__APPLCOMPARE_2_7                                                   (0x10631224 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLCOMPARE_2_7__APPLICATIONCOMPARATOR                            (0xFFFFFFFF)
#define U_PNIP__APPLCOMPARE_2_7_RESET__VALUE                     0x0
#define U_PNIP__APPLTIMER_3                                                       (0x10631300 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLTIMER_3__APPLICATIONTIMER                                     (0xFFFFFFFF)
#define U_PNIP__APPLTIMER_3_RESET__VALUE                         0x0
#define U_PNIP__APPLLENGTH_3                                                      (0x10631304 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLLENGTH_3__APPLICATIONLENGTH                                   (0xFFFFFFFF)
#define U_PNIP__APPLLENGTH_3_RESET__VALUE                        0x0
#define U_PNIP__APPLRCF_3                                                         (0x10631308 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLRCF_3__RCF_VALID                                              (0x80000000)
#define U_PNIP__APPLRCF_3__RATECOMPENSATIONINCREMENT                              (0x40000000)
#define U_PNIP__APPLRCF_3__RATECOMPENSATIONFACTOR                                 (0x3FFFFFFF)
#define U_PNIP__APPLRCF_3_RESET__VALUE                           0x0
#define U_PNIP__APPLCOMPARE_3_1                                                   (0x1063130C + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLCOMPARE_3_1__APPLICATIONCOMPARATOR                            (0xFFFFFFF8)
#define U_PNIP__APPLCOMPARE_3_1_RESET__VALUE                     0x0
#define U_PNIP__APPLCOMPARE_3_2                                                   (0x10631310 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLCOMPARE_3_2__APPLICATIONCOMPARATOR                            (0xFFFFFFFF)
#define U_PNIP__APPLCOMPARE_3_2_RESET__VALUE                     0x0
#define U_PNIP__APPLCOMPARE_3_3                                                   (0x10631314 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLCOMPARE_3_3__APPLICATIONCOMPARATOR                            (0xFFFFFFFF)
#define U_PNIP__APPLCOMPARE_3_3_RESET__VALUE                     0x0
#define U_PNIP__APPLCOMPARE_3_4                                                   (0x10631318 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLCOMPARE_3_4__APPLICATIONCOMPARATOR                            (0xFFFFFFFF)
#define U_PNIP__APPLCOMPARE_3_4_RESET__VALUE                     0x0
#define U_PNIP__APPLCOMPARE_3_5                                                   (0x1063131C + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLCOMPARE_3_5__APPLICATIONCOMPARATOR                            (0xFFFFFFFF)
#define U_PNIP__APPLCOMPARE_3_5_RESET__VALUE                     0x0
#define U_PNIP__APPLCOMPARE_3_6                                                   (0x10631320 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLCOMPARE_3_6__APPLICATIONCOMPARATOR                            (0xFFFFFFFF)
#define U_PNIP__APPLCOMPARE_3_6_RESET__VALUE                     0x0
#define U_PNIP__APPLCOMPARE_3_7                                                   (0x10631324 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLCOMPARE_3_7__APPLICATIONCOMPARATOR                            (0xFFFFFFFF)
#define U_PNIP__APPLCOMPARE_3_7_RESET__VALUE                     0x0
#define U_PNIP__PLL_OUT_CONTROL_0                                                 (0x10632000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PLL_OUT_CONTROL_0__PLL_LENGTH                                     (0x00FE0000)
#define U_PNIP__PLL_OUT_CONTROL_0__PLL_LEVEL                                      (0x00010000)
#define U_PNIP__PLL_OUT_CONTROL_0__PLL_APISELECT                                  (0x00000F00)
#define U_PNIP__PLL_OUT_CONTROL_0__PLL_CMPSELECT                                  (0x0000001F)
#define U_PNIP__PLL_OUT_CONTROL_0_RESET__VALUE                   0x10000
#define U_PNIP__PLL_OUT_CONTROL_1                                                 (0x10632004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PLL_OUT_CONTROL_1__PLL_LENGTH                                     (0x00FE0000)
#define U_PNIP__PLL_OUT_CONTROL_1__PLL_LEVEL                                      (0x00010000)
#define U_PNIP__PLL_OUT_CONTROL_1__PLL_APISELECT                                  (0x00000F00)
#define U_PNIP__PLL_OUT_CONTROL_1__PLL_CMPSELECT                                  (0x0000001F)
#define U_PNIP__PLL_OUT_CONTROL_1_RESET__VALUE                   0x10000
#define U_PNIP__PLL_OUT_CONTROL_2                                                 (0x10632008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PLL_OUT_CONTROL_2__PLL_LENGTH                                     (0x00FE0000)
#define U_PNIP__PLL_OUT_CONTROL_2__PLL_LEVEL                                      (0x00010000)
#define U_PNIP__PLL_OUT_CONTROL_2__PLL_APISELECT                                  (0x00000F00)
#define U_PNIP__PLL_OUT_CONTROL_2__PLL_CMPSELECT                                  (0x0000001F)
#define U_PNIP__PLL_OUT_CONTROL_2_RESET__VALUE                   0x10000
#define U_PNIP__PLL_OUT_CONTROL_3                                                 (0x1063200C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PLL_OUT_CONTROL_3__PLL_LENGTH                                     (0x00FE0000)
#define U_PNIP__PLL_OUT_CONTROL_3__PLL_LEVEL                                      (0x00010000)
#define U_PNIP__PLL_OUT_CONTROL_3__PLL_APISELECT                                  (0x00000F00)
#define U_PNIP__PLL_OUT_CONTROL_3__PLL_CMPSELECT                                  (0x0000001F)
#define U_PNIP__PLL_OUT_CONTROL_3_RESET__VALUE                   0x10000
#define U_PNIP__PLL_OUT_CONTROL_4                                                 (0x10632010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PLL_OUT_CONTROL_4__PLL_LENGTH                                     (0x00FE0000)
#define U_PNIP__PLL_OUT_CONTROL_4__PLL_LEVEL                                      (0x00010000)
#define U_PNIP__PLL_OUT_CONTROL_4__PLL_APISELECT                                  (0x00000F00)
#define U_PNIP__PLL_OUT_CONTROL_4__PLL_CMPSELECT                                  (0x0000001F)
#define U_PNIP__PLL_OUT_CONTROL_4_RESET__VALUE                   0x10000
#define U_PNIP__PLL_OUT_CONTROL_5                                                 (0x10632014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PLL_OUT_CONTROL_5__PLL_LENGTH                                     (0x00FE0000)
#define U_PNIP__PLL_OUT_CONTROL_5__PLL_LEVEL                                      (0x00010000)
#define U_PNIP__PLL_OUT_CONTROL_5__PLL_APISELECT                                  (0x00000F00)
#define U_PNIP__PLL_OUT_CONTROL_5__PLL_CMPSELECT                                  (0x0000001F)
#define U_PNIP__PLL_OUT_CONTROL_5_RESET__VALUE                   0x10000
#define U_PNIP__PLL_OUT_CONTROL_6                                                 (0x10632018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PLL_OUT_CONTROL_6__PLL_LENGTH                                     (0x00FE0000)
#define U_PNIP__PLL_OUT_CONTROL_6__PLL_LEVEL                                      (0x00010000)
#define U_PNIP__PLL_OUT_CONTROL_6__PLL_APISELECT                                  (0x00000F00)
#define U_PNIP__PLL_OUT_CONTROL_6__PLL_CMPSELECT                                  (0x0000001F)
#define U_PNIP__PLL_OUT_CONTROL_6_RESET__VALUE                   0x10000
#define U_PNIP__PLL_OUT_CONTROL_7                                                 (0x1063201C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PLL_OUT_CONTROL_7__PLL_LENGTH                                     (0x00FE0000)
#define U_PNIP__PLL_OUT_CONTROL_7__PLL_LEVEL                                      (0x00010000)
#define U_PNIP__PLL_OUT_CONTROL_7__PLL_APISELECT                                  (0x00000F00)
#define U_PNIP__PLL_OUT_CONTROL_7__PLL_CMPSELECT                                  (0x0000001F)
#define U_PNIP__PLL_OUT_CONTROL_7_RESET__VALUE                   0x10000
#define U_PNIP__PLL_OUT_CONTROL_8                                                 (0x10632020 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PLL_OUT_CONTROL_8__PLL_LENGTH                                     (0x00FE0000)
#define U_PNIP__PLL_OUT_CONTROL_8__PLL_LEVEL                                      (0x00010000)
#define U_PNIP__PLL_OUT_CONTROL_8__PLL_APISELECT                                  (0x00000F00)
#define U_PNIP__PLL_OUT_CONTROL_8__PLL_CMPSELECT                                  (0x0000001F)
#define U_PNIP__PLL_OUT_CONTROL_8_RESET__VALUE                   0x10000
#define U_PNIP__PLL_OUT_CONTROL_9                                                 (0x10632024 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PLL_OUT_CONTROL_9__PLL_LENGTH                                     (0x00FE0000)
#define U_PNIP__PLL_OUT_CONTROL_9__PLL_LEVEL                                      (0x00010000)
#define U_PNIP__PLL_OUT_CONTROL_9__PLL_APISELECT                                  (0x00000F00)
#define U_PNIP__PLL_OUT_CONTROL_9__PLL_CMPSELECT                                  (0x0000001F)
#define U_PNIP__PLL_OUT_CONTROL_9_RESET__VALUE                   0x10000
#define U_PNIP__PLL_OUT_CONTROL_10                                                (0x10632028 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PLL_OUT_CONTROL_10__PLL_LENGTH                                    (0x00FE0000)
#define U_PNIP__PLL_OUT_CONTROL_10__PLL_LEVEL                                     (0x00010000)
#define U_PNIP__PLL_OUT_CONTROL_10__PLL_APISELECT                                 (0x00000F00)
#define U_PNIP__PLL_OUT_CONTROL_10__PLL_CMPSELECT                                 (0x0000001F)
#define U_PNIP__PLL_OUT_CONTROL_10_RESET__VALUE                  0x10000
#define U_PNIP__PLL_OUT_CONTROL_11                                                (0x1063202C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PLL_OUT_CONTROL_11__PLL_LENGTH                                    (0x00FE0000)
#define U_PNIP__PLL_OUT_CONTROL_11__PLL_LEVEL                                     (0x00010000)
#define U_PNIP__PLL_OUT_CONTROL_11__PLL_APISELECT                                 (0x00000F00)
#define U_PNIP__PLL_OUT_CONTROL_11__PLL_CMPSELECT                                 (0x0000001F)
#define U_PNIP__PLL_OUT_CONTROL_11_RESET__VALUE                  0x10000
#define U_PNIP__PLL_OUT_CONTROL_12                                                (0x10632030 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PLL_OUT_CONTROL_12__PLL_LENGTH                                    (0x00FE0000)
#define U_PNIP__PLL_OUT_CONTROL_12__PLL_LEVEL                                     (0x00010000)
#define U_PNIP__PLL_OUT_CONTROL_12__PLL_APISELECT                                 (0x00000F00)
#define U_PNIP__PLL_OUT_CONTROL_12__PLL_CMPSELECT                                 (0x0000001F)
#define U_PNIP__PLL_OUT_CONTROL_12_RESET__VALUE                  0x10000
#define U_PNIP__PLL_OUT_CONTROL_13                                                (0x10632034 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PLL_OUT_CONTROL_13__PLL_LENGTH                                    (0x00FE0000)
#define U_PNIP__PLL_OUT_CONTROL_13__PLL_LEVEL                                     (0x00010000)
#define U_PNIP__PLL_OUT_CONTROL_13__PLL_APISELECT                                 (0x00000F00)
#define U_PNIP__PLL_OUT_CONTROL_13__PLL_CMPSELECT                                 (0x0000001F)
#define U_PNIP__PLL_OUT_CONTROL_13_RESET__VALUE                  0x10000
#define U_PNIP__PLL_OUT_CONTROL_14                                                (0x10632038 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PLL_OUT_CONTROL_14__PLL_LENGTH                                    (0x00FE0000)
#define U_PNIP__PLL_OUT_CONTROL_14__PLL_LEVEL                                     (0x00010000)
#define U_PNIP__PLL_OUT_CONTROL_14__PLL_APISELECT                                 (0x00000F00)
#define U_PNIP__PLL_OUT_CONTROL_14__PLL_CMPSELECT                                 (0x0000001F)
#define U_PNIP__PLL_OUT_CONTROL_14_RESET__VALUE                  0x10000
#define U_PNIP__PLL_OUT_CONTROL_15                                                (0x1063203C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PLL_OUT_CONTROL_15__PLL_LENGTH                                    (0x00FE0000)
#define U_PNIP__PLL_OUT_CONTROL_15__PLL_LEVEL                                     (0x00010000)
#define U_PNIP__PLL_OUT_CONTROL_15__PLL_APISELECT                                 (0x00000F00)
#define U_PNIP__PLL_OUT_CONTROL_15__PLL_CMPSELECT                                 (0x0000001F)
#define U_PNIP__PLL_OUT_CONTROL_15_RESET__VALUE                  0x10000
#define U_PNIP__PLL_OUT_CONTROL_16                                                (0x10632040 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PLL_OUT_CONTROL_16__PLL_LENGTH                                    (0x00FE0000)
#define U_PNIP__PLL_OUT_CONTROL_16__PLL_LEVEL                                     (0x00010000)
#define U_PNIP__PLL_OUT_CONTROL_16__PLL_APISELECT                                 (0x00000F00)
#define U_PNIP__PLL_OUT_CONTROL_16__PLL_CMPSELECT                                 (0x0000001F)
#define U_PNIP__PLL_OUT_CONTROL_16_RESET__VALUE                  0x10000
#define U_PNIP__PLL_OUT_CONTROL_17                                                (0x10632044 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PLL_OUT_CONTROL_17__PLL_LENGTH                                    (0x00FE0000)
#define U_PNIP__PLL_OUT_CONTROL_17__PLL_LEVEL                                     (0x00010000)
#define U_PNIP__PLL_OUT_CONTROL_17__PLL_APISELECT                                 (0x00000F00)
#define U_PNIP__PLL_OUT_CONTROL_17__PLL_CMPSELECT                                 (0x0000001F)
#define U_PNIP__PLL_OUT_CONTROL_17_RESET__VALUE                  0x10000
#define U_PNIP__PLL_OUT_CONTROL_18                                                (0x10632048 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PLL_OUT_CONTROL_18__PLL_LENGTH                                    (0x00FE0000)
#define U_PNIP__PLL_OUT_CONTROL_18__PLL_LEVEL                                     (0x00010000)
#define U_PNIP__PLL_OUT_CONTROL_18__PLL_APISELECT                                 (0x00000F00)
#define U_PNIP__PLL_OUT_CONTROL_18__PLL_CMPSELECT                                 (0x0000001F)
#define U_PNIP__PLL_OUT_CONTROL_18_RESET__VALUE                  0x10000
#define U_PNIP__PLL_OUT_CONTROL_19                                                (0x1063204C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PLL_OUT_CONTROL_19__PLL_LENGTH                                    (0x00FE0000)
#define U_PNIP__PLL_OUT_CONTROL_19__PLL_LEVEL                                     (0x00010000)
#define U_PNIP__PLL_OUT_CONTROL_19__PLL_APISELECT                                 (0x00000F00)
#define U_PNIP__PLL_OUT_CONTROL_19__PLL_CMPSELECT                                 (0x0000001F)
#define U_PNIP__PLL_OUT_CONTROL_19_RESET__VALUE                  0x10000
#define U_PNIP__PLL_OUT_CONTROL_20                                                (0x10632050 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PLL_OUT_CONTROL_20__PLL_LENGTH                                    (0x00FE0000)
#define U_PNIP__PLL_OUT_CONTROL_20__PLL_LEVEL                                     (0x00010000)
#define U_PNIP__PLL_OUT_CONTROL_20__PLL_APISELECT                                 (0x00000F00)
#define U_PNIP__PLL_OUT_CONTROL_20__PLL_CMPSELECT                                 (0x0000001F)
#define U_PNIP__PLL_OUT_CONTROL_20_RESET__VALUE                  0x10000
#define U_PNIP__IRQ_CONTROL_0                                                     (0x10632100 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ_CONTROL_0__IRQ_CMPSELECT                                      (0x0000001F)
#define U_PNIP__IRQ_CONTROL_0_RESET__VALUE                       0x0
#define U_PNIP__IRQ_CONTROL_1                                                     (0x10632104 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ_CONTROL_1__IRQ_CMPSELECT                                      (0x0000001F)
#define U_PNIP__IRQ_CONTROL_1_RESET__VALUE                       0x0
#define U_PNIP__IRQ_CONTROL_2                                                     (0x10632108 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ_CONTROL_2__IRQ_CMPSELECT                                      (0x0000001F)
#define U_PNIP__IRQ_CONTROL_2_RESET__VALUE                       0x0
#define U_PNIP__IRQ_CONTROL_3                                                     (0x1063210C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ_CONTROL_3__IRQ_CMPSELECT                                      (0x0000001F)
#define U_PNIP__IRQ_CONTROL_3_RESET__VALUE                       0x0
#define U_PNIP__IRQ_CONTROL_4                                                     (0x10632110 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ_CONTROL_4__IRQ_CMPSELECT                                      (0x0000001F)
#define U_PNIP__IRQ_CONTROL_4_RESET__VALUE                       0x0
#define U_PNIP__IRQ_CONTROL_5                                                     (0x10632114 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ_CONTROL_5__IRQ_CMPSELECT                                      (0x0000001F)
#define U_PNIP__IRQ_CONTROL_5_RESET__VALUE                       0x0
#define U_PNIP__IRQ_CONTROL_6                                                     (0x10632118 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ_CONTROL_6__IRQ_CMPSELECT                                      (0x0000001F)
#define U_PNIP__IRQ_CONTROL_6_RESET__VALUE                       0x0
#define U_PNIP__IRQ_CONTROL_7                                                     (0x1063211C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ_CONTROL_7__IRQ_CMPSELECT                                      (0x0000001F)
#define U_PNIP__IRQ_CONTROL_7_RESET__VALUE                       0x0
#define U_PNIP__IRQ_CONTROL_8                                                     (0x10632120 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ_CONTROL_8__IRQ_CMPSELECT                                      (0x0000001F)
#define U_PNIP__IRQ_CONTROL_8_RESET__VALUE                       0x0
#define U_PNIP__IRQ_CONTROL_9                                                     (0x10632124 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ_CONTROL_9__IRQ_CMPSELECT                                      (0x0000001F)
#define U_PNIP__IRQ_CONTROL_9_RESET__VALUE                       0x0
#define U_PNIP__SYNCCOMMAND_IF_CONTROL                                            (0x10633000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__SYNCCOMMAND_IF_CONTROL__COMMANDVALUE                              (0x0FFFFE00)
#define U_PNIP__SYNCCOMMAND_IF_CONTROL__USER_ID                                   (0x000001C0)
#define U_PNIP__SYNCCOMMAND_IF_CONTROL__CONFREQUEST                               (0x00000020)
#define U_PNIP__SYNCCOMMAND_IF_CONTROL__F_CODE                                    (0x0000000F)
#define U_PNIP__SYNCCOMMAND_IF_CONTROL_RESET__VALUE              0x0
#define U_PNIP__SYNCCOMMAND_IF_STATUS                                             (0x10633004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__SYNCCOMMAND_IF_STATUS__USER_ID                                    (0x000001C0)
#define U_PNIP__SYNCCOMMAND_IF_STATUS__CONFRESPONSE                               (0x00000020)
#define U_PNIP__SYNCCOMMAND_IF_STATUS__CONFERROR                                  (0x00000010)
#define U_PNIP__SYNCCOMMAND_IF_STATUS__F_CODE                                     (0x0000000F)
#define U_PNIP__SYNCCOMMAND_IF_STATUS_RESET__VALUE               0x0
#define U_PNIP__SYNCPARAMSTRUCT_BASE                                              (0x10633008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__SYNCPARAMSTRUCT_BASE__BASE_PARAMSTRUCT                            (0x001FFFFF)
#define U_PNIP__SYNCPARAMSTRUCT_BASE_RESET__VALUE                0x3FFF8
#define U_PNIP__PHASERANGE_A                                                      (0x10634000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHASERANGE_A__PHASERANGE                                          (0x0000003F)
#define U_PNIP__PHASERANGE_A_RESET__VALUE                        0x0
#define U_PNIP__PHASERANGE_B                                                      (0x10634004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHASERANGE_B__PHASERANGE                                          (0x0000003F)
#define U_PNIP__PHASERANGE_B_RESET__VALUE                        0x0
#define U_PNIP__PHASEENABLE_P1                                                    (0x10634100 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHASEENABLE_P1__START_TXRED                                       (0x00001000)
#define U_PNIP__PHASEENABLE_P1__PHASELOCK_TXMODE                                  (0x00000C00)
#define U_PNIP__PHASEENABLE_P1__ENABLE_TXORANGE                                   (0x00000200)
#define U_PNIP__PHASEENABLE_P1__ENABLE_TXRED                                      (0x00000100)
#define U_PNIP__PHASEENABLE_P1__PHASELOCK_RXMODE                                  (0x00000004)
#define U_PNIP__PHASEENABLE_P1__ENABLE_RXORANGE                                   (0x00000002)
#define U_PNIP__PHASEENABLE_P1__ENABLE_RXRED                                      (0x00000001)
#define U_PNIP__PHASEENABLE_P1_RESET__VALUE                      0x0
#define U_PNIP__PHASEBASE_RXD_P1                                                  (0x10634104 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHASEBASE_RXD_P1__BASEADR_PHASELIST                               (0x001FFFFF)
#define U_PNIP__PHASEBASE_RXD_P1_RESET__VALUE                    0x3FFFC
#define U_PNIP__PHASESTATUS_RXD_P1                                                (0x10634108 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHASESTATUS_RXD_P1__PHASETYPE_T_3                                 (0xFC000000)
#define U_PNIP__PHASESTATUS_RXD_P1__PHASETYPE_T_2                                 (0x03F00000)
#define U_PNIP__PHASESTATUS_RXD_P1__PHASETYPE_T_1                                 (0x000FC000)
#define U_PNIP__PHASESTATUS_RXD_P1__PHASETYPE_T                                   (0x00003F00)
#define U_PNIP__PHASESTATUS_RXD_P1__PHASECHECK                                    (0x00000080)
#define U_PNIP__PHASESTATUS_RXD_P1__PHASELOCK                                     (0x00000040)
#define U_PNIP__PHASESTATUS_RXD_P1__PHASENUMBER                                   (0x0000003F)
#define U_PNIP__PHASESTATUS_RXD_P1_RESET__VALUE                  0x0
#define U_PNIP__PHASEBASE_TXD_P1                                                  (0x1063410C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHASEBASE_TXD_P1__BASEADR_PHASELIST                               (0x001FFFFF)
#define U_PNIP__PHASEBASE_TXD_P1_RESET__VALUE                    0x3FFFC
#define U_PNIP__PHASESTATUS_TXD_P1                                                (0x10634110 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHASESTATUS_TXD_P1__PHASETYPE_T_3                                 (0xFC000000)
#define U_PNIP__PHASESTATUS_TXD_P1__PHASETYPE_T_2                                 (0x03F00000)
#define U_PNIP__PHASESTATUS_TXD_P1__PHASETYPE_T_1                                 (0x000FC000)
#define U_PNIP__PHASESTATUS_TXD_P1__PHASETYPE_T                                   (0x00003F00)
#define U_PNIP__PHASESTATUS_TXD_P1__PHASECHECK                                    (0x00000080)
#define U_PNIP__PHASESTATUS_TXD_P1__PHASELOCK                                     (0x00000040)
#define U_PNIP__PHASESTATUS_TXD_P1__PHASENUMBER                                   (0x0000003F)
#define U_PNIP__PHASESTATUS_TXD_P1_RESET__VALUE                  0x0
#define U_PNIP__PHASEENABLE_P2                                                    (0x10634200 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHASEENABLE_P2__START_TXRED                                       (0x00001000)
#define U_PNIP__PHASEENABLE_P2__PHASELOCK_TXMODE                                  (0x00000C00)
#define U_PNIP__PHASEENABLE_P2__ENABLE_TXORANGE                                   (0x00000200)
#define U_PNIP__PHASEENABLE_P2__ENABLE_TXRED                                      (0x00000100)
#define U_PNIP__PHASEENABLE_P2__PHASELOCK_RXMODE                                  (0x00000004)
#define U_PNIP__PHASEENABLE_P2__ENABLE_RXORANGE                                   (0x00000002)
#define U_PNIP__PHASEENABLE_P2__ENABLE_RXRED                                      (0x00000001)
#define U_PNIP__PHASEENABLE_P2_RESET__VALUE                      0x0
#define U_PNIP__PHASEBASE_RXD_P2                                                  (0x10634204 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHASEBASE_RXD_P2__BASEADR_PHASELIST                               (0x001FFFFF)
#define U_PNIP__PHASEBASE_RXD_P2_RESET__VALUE                    0x3FFFC
#define U_PNIP__PHASESTATUS_RXD_P2                                                (0x10634208 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHASESTATUS_RXD_P2__PHASETYPE_T_3                                 (0xFC000000)
#define U_PNIP__PHASESTATUS_RXD_P2__PHASETYPE_T_2                                 (0x03F00000)
#define U_PNIP__PHASESTATUS_RXD_P2__PHASETYPE_T_1                                 (0x000FC000)
#define U_PNIP__PHASESTATUS_RXD_P2__PHASETYPE_T                                   (0x00003F00)
#define U_PNIP__PHASESTATUS_RXD_P2__PHASECHECK                                    (0x00000080)
#define U_PNIP__PHASESTATUS_RXD_P2__PHASELOCK                                     (0x00000040)
#define U_PNIP__PHASESTATUS_RXD_P2__PHASENUMBER                                   (0x0000003F)
#define U_PNIP__PHASESTATUS_RXD_P2_RESET__VALUE                  0x0
#define U_PNIP__PHASEBASE_TXD_P2                                                  (0x1063420C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHASEBASE_TXD_P2__BASEADR_PHASELIST                               (0x001FFFFF)
#define U_PNIP__PHASEBASE_TXD_P2_RESET__VALUE                    0x3FFFC
#define U_PNIP__PHASESTATUS_TXD_P2                                                (0x10634210 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHASESTATUS_TXD_P2__PHASETYPE_T_3                                 (0xFC000000)
#define U_PNIP__PHASESTATUS_TXD_P2__PHASETYPE_T_2                                 (0x03F00000)
#define U_PNIP__PHASESTATUS_TXD_P2__PHASETYPE_T_1                                 (0x000FC000)
#define U_PNIP__PHASESTATUS_TXD_P2__PHASETYPE_T                                   (0x00003F00)
#define U_PNIP__PHASESTATUS_TXD_P2__PHASECHECK                                    (0x00000080)
#define U_PNIP__PHASESTATUS_TXD_P2__PHASELOCK                                     (0x00000040)
#define U_PNIP__PHASESTATUS_TXD_P2__PHASENUMBER                                   (0x0000003F)
#define U_PNIP__PHASESTATUS_TXD_P2_RESET__VALUE                  0x0
#define U_PNIP__PHASEENABLE_P3                                                    (0x10634300 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHASEENABLE_P3__START_TXRED                                       (0x00001000)
#define U_PNIP__PHASEENABLE_P3__PHASELOCK_TXMODE                                  (0x00000C00)
#define U_PNIP__PHASEENABLE_P3__ENABLE_TXORANGE                                   (0x00000200)
#define U_PNIP__PHASEENABLE_P3__ENABLE_TXRED                                      (0x00000100)
#define U_PNIP__PHASEENABLE_P3__PHASELOCK_RXMODE                                  (0x00000004)
#define U_PNIP__PHASEENABLE_P3__ENABLE_RXORANGE                                   (0x00000002)
#define U_PNIP__PHASEENABLE_P3__ENABLE_RXRED                                      (0x00000001)
#define U_PNIP__PHASEENABLE_P3_RESET__VALUE                      0x0
#define U_PNIP__PHASEBASE_RXD_P3                                                  (0x10634304 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHASEBASE_RXD_P3__BASEADR_PHASELIST                               (0x001FFFFF)
#define U_PNIP__PHASEBASE_RXD_P3_RESET__VALUE                    0x3FFFC
#define U_PNIP__PHASESTATUS_RXD_P3                                                (0x10634308 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHASESTATUS_RXD_P3__PHASETYPE_T_3                                 (0xFC000000)
#define U_PNIP__PHASESTATUS_RXD_P3__PHASETYPE_T_2                                 (0x03F00000)
#define U_PNIP__PHASESTATUS_RXD_P3__PHASETYPE_T_1                                 (0x000FC000)
#define U_PNIP__PHASESTATUS_RXD_P3__PHASETYPE_T                                   (0x00003F00)
#define U_PNIP__PHASESTATUS_RXD_P3__PHASECHECK                                    (0x00000080)
#define U_PNIP__PHASESTATUS_RXD_P3__PHASELOCK                                     (0x00000040)
#define U_PNIP__PHASESTATUS_RXD_P3__PHASENUMBER                                   (0x0000003F)
#define U_PNIP__PHASESTATUS_RXD_P3_RESET__VALUE                  0x0
#define U_PNIP__PHASEBASE_TXD_P3                                                  (0x1063430C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHASEBASE_TXD_P3__BASEADR_PHASELIST                               (0x001FFFFF)
#define U_PNIP__PHASEBASE_TXD_P3_RESET__VALUE                    0x3FFFC
#define U_PNIP__PHASESTATUS_TXD_P3                                                (0x10634310 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHASESTATUS_TXD_P3__PHASETYPE_T_3                                 (0xFC000000)
#define U_PNIP__PHASESTATUS_TXD_P3__PHASETYPE_T_2                                 (0x03F00000)
#define U_PNIP__PHASESTATUS_TXD_P3__PHASETYPE_T_1                                 (0x000FC000)
#define U_PNIP__PHASESTATUS_TXD_P3__PHASETYPE_T                                   (0x00003F00)
#define U_PNIP__PHASESTATUS_TXD_P3__PHASECHECK                                    (0x00000080)
#define U_PNIP__PHASESTATUS_TXD_P3__PHASELOCK                                     (0x00000040)
#define U_PNIP__PHASESTATUS_TXD_P3__PHASENUMBER                                   (0x0000003F)
#define U_PNIP__PHASESTATUS_TXD_P3_RESET__VALUE                  0x0
#define U_PNIP__PHASEENABLE_P4                                                    (0x10634400 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHASEENABLE_P4__START_TXRED                                       (0x00001000)
#define U_PNIP__PHASEENABLE_P4__PHASELOCK_TXMODE                                  (0x00000C00)
#define U_PNIP__PHASEENABLE_P4__ENABLE_TXORANGE                                   (0x00000200)
#define U_PNIP__PHASEENABLE_P4__ENABLE_TXRED                                      (0x00000100)
#define U_PNIP__PHASEENABLE_P4__PHASELOCK_RXMODE                                  (0x00000004)
#define U_PNIP__PHASEENABLE_P4__ENABLE_RXORANGE                                   (0x00000002)
#define U_PNIP__PHASEENABLE_P4__ENABLE_RXRED                                      (0x00000001)
#define U_PNIP__PHASEENABLE_P4_RESET__VALUE                      0x0
#define U_PNIP__PHASEBASE_RXD_P4                                                  (0x10634404 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHASEBASE_RXD_P4__BASEADR_PHASELIST                               (0x001FFFFF)
#define U_PNIP__PHASEBASE_RXD_P4_RESET__VALUE                    0x3FFFC
#define U_PNIP__PHASESTATUS_RXD_P4                                                (0x10634408 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHASESTATUS_RXD_P4__PHASETYPE_T_3                                 (0xFC000000)
#define U_PNIP__PHASESTATUS_RXD_P4__PHASETYPE_T_2                                 (0x03F00000)
#define U_PNIP__PHASESTATUS_RXD_P4__PHASETYPE_T_1                                 (0x000FC000)
#define U_PNIP__PHASESTATUS_RXD_P4__PHASETYPE_T                                   (0x00003F00)
#define U_PNIP__PHASESTATUS_RXD_P4__PHASECHECK                                    (0x00000080)
#define U_PNIP__PHASESTATUS_RXD_P4__PHASELOCK                                     (0x00000040)
#define U_PNIP__PHASESTATUS_RXD_P4__PHASENUMBER                                   (0x0000003F)
#define U_PNIP__PHASESTATUS_RXD_P4_RESET__VALUE                  0x0
#define U_PNIP__PHASEBASE_TXD_P4                                                  (0x1063440C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHASEBASE_TXD_P4__BASEADR_PHASELIST                               (0x001FFFFF)
#define U_PNIP__PHASEBASE_TXD_P4_RESET__VALUE                    0x3FFFC
#define U_PNIP__PHASESTATUS_TXD_P4                                                (0x10634410 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHASESTATUS_TXD_P4__PHASETYPE_T_3                                 (0xFC000000)
#define U_PNIP__PHASESTATUS_TXD_P4__PHASETYPE_T_2                                 (0x03F00000)
#define U_PNIP__PHASESTATUS_TXD_P4__PHASETYPE_T_1                                 (0x000FC000)
#define U_PNIP__PHASESTATUS_TXD_P4__PHASETYPE_T                                   (0x00003F00)
#define U_PNIP__PHASESTATUS_TXD_P4__PHASECHECK                                    (0x00000080)
#define U_PNIP__PHASESTATUS_TXD_P4__PHASELOCK                                     (0x00000040)
#define U_PNIP__PHASESTATUS_TXD_P4__PHASENUMBER                                   (0x0000003F)
#define U_PNIP__PHASESTATUS_TXD_P4_RESET__VALUE                  0x0
#define U_PNIP__PHASEBASE_TIME                                                    (0x10634500 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PHASEBASE_TIME__BASEADR_TIMELIST                                  (0x001FFFFF)
#define U_PNIP__PHASEBASE_TIME_RESET__VALUE                      0x3FFFC
#define U_PNIP__IRQEXT_EVENT                                                      (0x10640000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQEXT_EVENT__EXT_IRQ3_EDGE                                       (0x02000000)
#define U_PNIP__IRQEXT_EVENT__EXT_IRQ3_LEVEL                                      (0x01000000)
#define U_PNIP__IRQEXT_EVENT__EXT_IRQ2_EDGE                                       (0x00020000)
#define U_PNIP__IRQEXT_EVENT__EXT_IRQ2_LEVEL                                      (0x00010000)
#define U_PNIP__IRQEXT_EVENT__EXT_IRQ1_EDGE                                       (0x00000200)
#define U_PNIP__IRQEXT_EVENT__EXT_IRQ1_LEVEL                                      (0x00000100)
#define U_PNIP__IRQEXT_EVENT__EXT_IRQ0_EDGE                                       (0x00000002)
#define U_PNIP__IRQEXT_EVENT__EXT_IRQ0_LEVEL                                      (0x00000001)
#define U_PNIP__IRQEXT_EVENT_RESET__VALUE                        0x1010101
#define U_PNIP__IRQ_TIMEVALUE                                                     (0x10640004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ_TIMEVALUE__TIMER_VALUE                                        (0xFFFFFFFF)
#define U_PNIP__IRQ_TIMEVALUE_RESET__VALUE                       0x0
#define U_PNIP__IRQ1_CONTROL                                                      (0x10641000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ1_CONTROL__IRQDEBUG_ENABLE                                     (0x00100000)
#define U_PNIP__IRQ1_CONTROL__IRQDEBUG_SELECT                                     (0x000F0000)
#define U_PNIP__IRQ1_CONTROL__IRQ_LENGTH                                          (0x0000000E)
#define U_PNIP__IRQ1_CONTROL__IRQ_LEVEL                                           (0x00000001)
#define U_PNIP__IRQ1_CONTROL_RESET__VALUE                        0x0
#define U_PNIP__IRQ1_ACTIVATE                                                     (0x10641004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ1_ACTIVATE__ACTIVATE                                           (0x0000000F)
#define U_PNIP__IRQ1_ACTIVATE_RESET__VALUE                       0x0
#define U_PNIP__IRQ1_TIMESTAMP                                                    (0x10641008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ1_TIMESTAMP__TIMER_STAMP_31_3                                  (0xFFFFFFF8)
#define U_PNIP__IRQ1_TIMESTAMP__TIMER_STAMP_2_0                                   (0x00000007)
#define U_PNIP__IRQ1_TIMESTAMP_RESET__VALUE                      0x0
#define U_PNIP__IRQ1EVENT_LOW_0                                                   (0x10641100 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_INBOUND_LATE_ERR                             (0x80000000)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_FRAMEAGING_EXPIRED                           (0x20000000)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_HOL_EXPIRED_PB                               (0x10000000)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_HOL_EXPIRED_PA                               (0x08000000)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_HOL_EXPIRED_IFB                              (0x04000000)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_HOL_EXPIRED_IFA                              (0x02000000)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_QCW_EMPTY                                    (0x01000000)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_PHASECROSSED                                 (0x00200000)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_PHASETRANSFERERROR                           (0x00100000)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_FRAGFAULT                                    (0x00080000)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_IRT_DELAYSTATUS                              (0x00040000)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_PACK_ERR                                     (0x00020000)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_PPM_ERR                                      (0x00010000)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_CPM_ERR                                      (0x00008000)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_REQQUEUE_ERR                                 (0x00004000)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_MAC_ERR                                      (0x00002000)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_FILTERCMD_DONE                               (0x00000800)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_VLANCMD_DONE                                 (0x00000400)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_FDBCMD_DONE                                  (0x00000200)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_SYNCCMD_DONE                                 (0x00000100)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_PACKCMD_DONE                                 (0x00000080)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_PPMCMD_DONE                                  (0x00000040)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_CPMCMD_DONE                                  (0x00000020)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_ASNDCMD_DONE                                 (0x00000010)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_ARCVCMD_DONE                                 (0x00000008)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_REQDONE_IFB_1                                (0x00000004)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_REQDONE_IFA_1                                (0x00000002)
#define U_PNIP__IRQ1EVENT_LOW_0__INT_STATUSCHANGE                                 (0x00000001)
#define U_PNIP__IRQ1EVENT_LOW_0_RESET__VALUE                     0x0
#define U_PNIP__IRQ1EVENT_MID_0                                                   (0x10641104 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ1EVENT_MID_0__EVENT_BITS_63_60                                 (0xF0000000)
#define U_PNIP__IRQ1EVENT_MID_0__PN_EXT_IRQ_3_0                                   (0x0F000000)
#define U_PNIP__IRQ1EVENT_MID_0__INT_PPM_DMA_ERR                                  (0x00200000)
#define U_PNIP__IRQ1EVENT_MID_0__INT_PPM_HOL_NOT_EMPTY_IFB                        (0x00080000)
#define U_PNIP__IRQ1EVENT_MID_0__INT_PPM_HOL_NOT_EMPTY_IFA                        (0x00040000)
#define U_PNIP__IRQ1EVENT_MID_0__INT_PPM_NOT_FINISHED_IFB                         (0x00020000)
#define U_PNIP__IRQ1EVENT_MID_0__INT_PPM_NOT_FINISHED_IFA                         (0x00010000)
#define U_PNIP__IRQ1EVENT_MID_0__INT_CSCOREBOARD_CHANGED_IFB                      (0x00008000)
#define U_PNIP__IRQ1EVENT_MID_0__INT_CSCOREBOARD_CHANGED_IFA                      (0x00004000)
#define U_PNIP__IRQ1EVENT_MID_0__INT_CPM_DMA_ERR                                  (0x00002000)
#define U_PNIP__IRQ1EVENT_MID_0__INT_CPM_OUTDATAERR                               (0x00000800)
#define U_PNIP__IRQ1EVENT_MID_0__INT_CPM_OUTDATAVALID                             (0x00000400)
#define U_PNIP__IRQ1EVENT_MID_0__INT_CPM_OUTDATASTART                             (0x00000200)
#define U_PNIP__IRQ1EVENT_MID_0__INT_ACYC_SND_ERR                                 (0x00000080)
#define U_PNIP__IRQ1EVENT_MID_0__INT_ACYC_RCV_ERR                                 (0x00000040)
#define U_PNIP__IRQ1EVENT_MID_0__INT_ACYC_SND_IFB_1                               (0x00000020)
#define U_PNIP__IRQ1EVENT_MID_0__INT_ACYC_RCV_IFB_1                               (0x00000010)
#define U_PNIP__IRQ1EVENT_MID_0__INT_ACYC_SND_IFA_1                               (0x00000008)
#define U_PNIP__IRQ1EVENT_MID_0__INT_ACYC_RCV_IFA_1                               (0x00000004)
#define U_PNIP__IRQ1EVENT_MID_0__INT_APIFILTER_ERR                                (0x00000001)
#define U_PNIP__IRQ1EVENT_MID_0_RESET__VALUE                     0x0
#define U_PNIP__IRQ1EVENT_HIGH_0                                                  (0x10641108 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ1EVENT_HIGH_0__INT_I2C                                         (0x00100000)
#define U_PNIP__IRQ1EVENT_HIGH_0__INT_TIMER_2                                     (0x00080000)
#define U_PNIP__IRQ1EVENT_HIGH_0__INT_TIMER_1                                     (0x00040000)
#define U_PNIP__IRQ1EVENT_HIGH_0__INT_TIMER_0                                     (0x00020000)
#define U_PNIP__IRQ1EVENT_HIGH_0__INT_WD                                          (0x00010000)
#define U_PNIP__IRQ1EVENT_HIGH_0__INT_PTCP_WD                                     (0x00008000)
#define U_PNIP__IRQ1EVENT_HIGH_0__INT_PLL9                                        (0x00001000)
#define U_PNIP__IRQ1EVENT_HIGH_0__INT_PLL8                                        (0x00000800)
#define U_PNIP__IRQ1EVENT_HIGH_0__INT_PLL7                                        (0x00000400)
#define U_PNIP__IRQ1EVENT_HIGH_0__INT_PLL6                                        (0x00000200)
#define U_PNIP__IRQ1EVENT_HIGH_0__INT_PLL5                                        (0x00000100)
#define U_PNIP__IRQ1EVENT_HIGH_0__INT_PLL4                                        (0x00000080)
#define U_PNIP__IRQ1EVENT_HIGH_0__INT_PLL3                                        (0x00000040)
#define U_PNIP__IRQ1EVENT_HIGH_0__INT_PLL2                                        (0x00000020)
#define U_PNIP__IRQ1EVENT_HIGH_0__INT_PLL1                                        (0x00000010)
#define U_PNIP__IRQ1EVENT_HIGH_0__INT_PLL0                                        (0x00000008)
#define U_PNIP__IRQ1EVENT_HIGH_0__INT_PLL_IN_TIME                                 (0x00000004)
#define U_PNIP__IRQ1EVENT_HIGH_0__INT_PLL_IN_B                                    (0x00000002)
#define U_PNIP__IRQ1EVENT_HIGH_0__INT_PLL_IN_A                                    (0x00000001)
#define U_PNIP__IRQ1EVENT_HIGH_0_RESET__VALUE                    0x0
#define U_PNIP__IRQ1MASK_LOW_0                                                    (0x1064110C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ1MASK_LOW_0__MASK_BITS_31_0                                    (0xFFFFFFFF)
#define U_PNIP__IRQ1MASK_LOW_0_RESET__VALUE                      0xFFFFFFFF
#define U_PNIP__IRQ1MASK_MID_0                                                    (0x10641110 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ1MASK_MID_0__MASK_BITS_63_32                                   (0xFFFFFFFF)
#define U_PNIP__IRQ1MASK_MID_0_RESET__VALUE                      0xFFFFFFFF
#define U_PNIP__IRQ1MASK_HIGH_0                                                   (0x10641114 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ1MASK_HIGH_0__MASK_BITS_85_64                                  (0x003FFFFF)
#define U_PNIP__IRQ1MASK_HIGH_0_RESET__VALUE                     0x3FFFFF
#define U_PNIP__IRQ1_LOW_0                                                        (0x10641118 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ1_LOW_0__IRQ_BITS_31_0                                         (0xFFFFFFFF)
#define U_PNIP__IRQ1_LOW_0_RESET__VALUE                          0x0
#define U_PNIP__IRQ1_MID_0                                                        (0x1064111C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ1_MID_0__IRQ_BITS_63_32                                        (0xFFFFFFFF)
#define U_PNIP__IRQ1_MID_0_RESET__VALUE                          0x0
#define U_PNIP__IRQ1_HIGH_0                                                       (0x10641120 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ1_HIGH_0__IRQ_BITS_85_64                                       (0x003FFFFF)
#define U_PNIP__IRQ1_HIGH_0_RESET__VALUE                         0x0
#define U_PNIP__IRQ1ACK_LOW_0                                                     (0x10641124 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ1ACK_LOW_0__ACK_BITS_31_0                                      (0xFFFFFFFF)
#define U_PNIP__IRQ1ACK_LOW_0_RESET__VALUE                       0x0
#define U_PNIP__IRQ1ACK_MID_0                                                     (0x10641128 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ1ACK_MID_0__ACK_BITS_63_32                                     (0xFFFFFFFF)
#define U_PNIP__IRQ1ACK_MID_0_RESET__VALUE                       0x0
#define U_PNIP__IRQ1ACK_HIGH_0                                                    (0x1064112C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ1ACK_HIGH_0__ACK_BITS_85_64                                    (0x003FFFFF)
#define U_PNIP__IRQ1ACK_HIGH_0_RESET__VALUE                      0x0
#define U_PNIP__IRQ1_EOI_0                                                        (0x10641130 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ1_EOI_0__WAIT_TIME                                             (0x00003FFF)
#define U_PNIP__IRQ1_EOI_0_RESET__VALUE                          0x0
#define U_PNIP__IRQ1CONTROL_MUX1                                                  (0x10641200 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ1CONTROL_MUX1__IRQ_WAITTIME                                    (0x01FF0000)
#define U_PNIP__IRQ1CONTROL_MUX1__IRQ_EVENT                                       (0x00000100)
#define U_PNIP__IRQ1CONTROL_MUX1__IRQ_SELECTMUX                                   (0x0000007F)
#define U_PNIP__IRQ1CONTROL_MUX1_RESET__VALUE                    0x7F
#define U_PNIP__IRQ1CONTROL_MUX2                                                  (0x10641204 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ1CONTROL_MUX2__IRQ_WAITTIME                                    (0x01FF0000)
#define U_PNIP__IRQ1CONTROL_MUX2__IRQ_EVENT                                       (0x00000100)
#define U_PNIP__IRQ1CONTROL_MUX2__IRQ_SELECTMUX                                   (0x0000007F)
#define U_PNIP__IRQ1CONTROL_MUX2_RESET__VALUE                    0x7F
#define U_PNIP__IRQ2_CONTROL                                                      (0x10642000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2_CONTROL__IRQDEBUG_ENABLE                                     (0x00100000)
#define U_PNIP__IRQ2_CONTROL__IRQDEBUG_SELECT                                     (0x000F0000)
#define U_PNIP__IRQ2_CONTROL__IRQ_LENGTH                                          (0x0000000E)
#define U_PNIP__IRQ2_CONTROL__IRQ_LEVEL                                           (0x00000001)
#define U_PNIP__IRQ2_CONTROL_RESET__VALUE                        0x1
#define U_PNIP__IRQ2_ACTIVATE                                                     (0x10642004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2_ACTIVATE__ACTIVATE                                           (0x0000000F)
#define U_PNIP__IRQ2_ACTIVATE_RESET__VALUE                       0x0
#define U_PNIP__IRQ2_TIMESTAMP                                                    (0x10642008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2_TIMESTAMP__TIMER_STAMP_31_3                                  (0xFFFFFFF8)
#define U_PNIP__IRQ2_TIMESTAMP__TIMER_STAMP_2_0                                   (0x00000007)
#define U_PNIP__IRQ2_TIMESTAMP_RESET__VALUE                      0x0
#define U_PNIP__IRQ2EVENT_LOW_0                                                   (0x10642100 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_INBOUND_LATE_ERR                             (0x80000000)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_FRAMEAGING_EXPIRED                           (0x20000000)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_HOL_EXPIRED_PB                               (0x10000000)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_HOL_EXPIRED_PA                               (0x08000000)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_HOL_EXPIRED_IFB                              (0x04000000)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_HOL_EXPIRED_IFA                              (0x02000000)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_QCW_EMPTY                                    (0x01000000)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_PHASECROSSED                                 (0x00200000)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_PHASETRANSFERERROR                           (0x00100000)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_FRAGFAULT                                    (0x00080000)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_IRT_DELAYSTATUS                              (0x00040000)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_PACK_ERR                                     (0x00020000)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_PPM_ERR                                      (0x00010000)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_CPM_ERR                                      (0x00008000)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_REQQUEUE_ERR                                 (0x00004000)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_MAC_ERR                                      (0x00002000)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_FILTERCMD_DONE                               (0x00000800)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_VLANCMD_DONE                                 (0x00000400)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_FDBCMD_DONE                                  (0x00000200)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_SYNCCMD_DONE                                 (0x00000100)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_PACKCMD_DONE                                 (0x00000080)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_PPMCMD_DONE                                  (0x00000040)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_CPMCMD_DONE                                  (0x00000020)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_ASNDCMD_DONE                                 (0x00000010)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_ARCVCMD_DONE                                 (0x00000008)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_REQDONE_IFB_2                                (0x00000004)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_REQDONE_IFA_2                                (0x00000002)
#define U_PNIP__IRQ2EVENT_LOW_0__INT_STATUSCHANGE                                 (0x00000001)
#define U_PNIP__IRQ2EVENT_LOW_0_RESET__VALUE                     0x0
#define U_PNIP__IRQ2EVENT_MID_0                                                   (0x10642104 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2EVENT_MID_0__EVENT_BITS_63_60                                 (0xF0000000)
#define U_PNIP__IRQ2EVENT_MID_0__PN_EXT_IRQ_3_0                                   (0x0F000000)
#define U_PNIP__IRQ2EVENT_MID_0__INT_PPM_DMA_ERR                                  (0x00200000)
#define U_PNIP__IRQ2EVENT_MID_0__INT_PPM_HOL_NOT_EMPTY_IFB                        (0x00080000)
#define U_PNIP__IRQ2EVENT_MID_0__INT_PPM_HOL_NOT_EMPTY_IFA                        (0x00040000)
#define U_PNIP__IRQ2EVENT_MID_0__INT_PPM_NOT_FINISHED_IFB                         (0x00020000)
#define U_PNIP__IRQ2EVENT_MID_0__INT_PPM_NOT_FINISHED_IFA                         (0x00010000)
#define U_PNIP__IRQ2EVENT_MID_0__INT_CSCOREBOARD_CHANGED_IFB                      (0x00008000)
#define U_PNIP__IRQ2EVENT_MID_0__INT_CSCOREBOARD_CHANGED_IFA                      (0x00004000)
#define U_PNIP__IRQ2EVENT_MID_0__INT_CPM_DMA_ERR                                  (0x00002000)
#define U_PNIP__IRQ2EVENT_MID_0__INT_CPM_OUTDATAERR                               (0x00000800)
#define U_PNIP__IRQ2EVENT_MID_0__INT_CPM_OUTDATAVALID                             (0x00000400)
#define U_PNIP__IRQ2EVENT_MID_0__INT_CPM_OUTDATASTART                             (0x00000200)
#define U_PNIP__IRQ2EVENT_MID_0__INT_ACYC_SND_ERR                                 (0x00000080)
#define U_PNIP__IRQ2EVENT_MID_0__INT_ACYC_RCV_ERR                                 (0x00000040)
#define U_PNIP__IRQ2EVENT_MID_0__INT_ACYC_SND_IFB_2                               (0x00000020)
#define U_PNIP__IRQ2EVENT_MID_0__INT_ACYC_RCV_IFB_2                               (0x00000010)
#define U_PNIP__IRQ2EVENT_MID_0__INT_ACYC_SND_IFA_2                               (0x00000008)
#define U_PNIP__IRQ2EVENT_MID_0__INT_ACYC_RCV_IFA_2                               (0x00000004)
#define U_PNIP__IRQ2EVENT_MID_0__INT_APIFILTER_ERR                                (0x00000001)
#define U_PNIP__IRQ2EVENT_MID_0_RESET__VALUE                     0x0
#define U_PNIP__IRQ2EVENT_HIGH_0                                                  (0x10642108 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2EVENT_HIGH_0__INT_I2C                                         (0x00100000)
#define U_PNIP__IRQ2EVENT_HIGH_0__INT_TIMER_2                                     (0x00080000)
#define U_PNIP__IRQ2EVENT_HIGH_0__INT_TIMER_1                                     (0x00040000)
#define U_PNIP__IRQ2EVENT_HIGH_0__INT_TIMER_0                                     (0x00020000)
#define U_PNIP__IRQ2EVENT_HIGH_0__INT_WD                                          (0x00010000)
#define U_PNIP__IRQ2EVENT_HIGH_0__INT_PTCP_WD                                     (0x00008000)
#define U_PNIP__IRQ2EVENT_HIGH_0__INT_PLL9                                        (0x00001000)
#define U_PNIP__IRQ2EVENT_HIGH_0__INT_PLL8                                        (0x00000800)
#define U_PNIP__IRQ2EVENT_HIGH_0__INT_PLL7                                        (0x00000400)
#define U_PNIP__IRQ2EVENT_HIGH_0__INT_PLL6                                        (0x00000200)
#define U_PNIP__IRQ2EVENT_HIGH_0__INT_PLL5                                        (0x00000100)
#define U_PNIP__IRQ2EVENT_HIGH_0__INT_PLL4                                        (0x00000080)
#define U_PNIP__IRQ2EVENT_HIGH_0__INT_PLL3                                        (0x00000040)
#define U_PNIP__IRQ2EVENT_HIGH_0__INT_PLL2                                        (0x00000020)
#define U_PNIP__IRQ2EVENT_HIGH_0__INT_PLL1                                        (0x00000010)
#define U_PNIP__IRQ2EVENT_HIGH_0__INT_PLL0                                        (0x00000008)
#define U_PNIP__IRQ2EVENT_HIGH_0__INT_PLL_IN_TIME                                 (0x00000004)
#define U_PNIP__IRQ2EVENT_HIGH_0__INT_PLL_IN_B                                    (0x00000002)
#define U_PNIP__IRQ2EVENT_HIGH_0__INT_PLL_IN_A                                    (0x00000001)
#define U_PNIP__IRQ2EVENT_HIGH_0_RESET__VALUE                    0x0
#define U_PNIP__IRQ2MASK_LOW_0                                                    (0x1064210C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2MASK_LOW_0__MASK_BITS_31_0                                    (0xFFFFFFFF)
#define U_PNIP__IRQ2MASK_LOW_0_RESET__VALUE                      0xFFFFFFFF
#define U_PNIP__IRQ2MASK_MID_0                                                    (0x10642110 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2MASK_MID_0__MASK_BITS_63_32                                   (0xFFFFFFFF)
#define U_PNIP__IRQ2MASK_MID_0_RESET__VALUE                      0xFFFFFFFF
#define U_PNIP__IRQ2MASK_HIGH_0                                                   (0x10642114 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2MASK_HIGH_0__MASK_BITS_85_64                                  (0x003FFFFF)
#define U_PNIP__IRQ2MASK_HIGH_0_RESET__VALUE                     0x3FFFFF
#define U_PNIP__IRQ2_LOW_0                                                        (0x10642118 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2_LOW_0__IRQ_BITS_31_0                                         (0xFFFFFFFF)
#define U_PNIP__IRQ2_LOW_0_RESET__VALUE                          0x0
#define U_PNIP__IRQ2_MID_0                                                        (0x1064211C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2_MID_0__IRQ_BITS_63_32                                        (0xFFFFFFFF)
#define U_PNIP__IRQ2_MID_0_RESET__VALUE                          0x0
#define U_PNIP__IRQ2_HIGH_0                                                       (0x10642120 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2_HIGH_0__IRQ_BITS_85_64                                       (0x003FFFFF)
#define U_PNIP__IRQ2_HIGH_0_RESET__VALUE                         0x0
#define U_PNIP__IRQ2ACK_LOW_0                                                     (0x10642124 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2ACK_LOW_0__ACK_BITS_31_0                                      (0xFFFFFFFF)
#define U_PNIP__IRQ2ACK_LOW_0_RESET__VALUE                       0x0
#define U_PNIP__IRQ2ACK_MID_0                                                     (0x10642128 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2ACK_MID_0__ACK_BITS_63_32                                     (0xFFFFFFFF)
#define U_PNIP__IRQ2ACK_MID_0_RESET__VALUE                       0x0
#define U_PNIP__IRQ2ACK_HIGH_0                                                    (0x1064212C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2ACK_HIGH_0__ACK_BITS_85_64                                    (0x003FFFFF)
#define U_PNIP__IRQ2ACK_HIGH_0_RESET__VALUE                      0x0
#define U_PNIP__IRQ2_EOI_0                                                        (0x10642130 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2_EOI_0__WAIT_TIME                                             (0x00003FFF)
#define U_PNIP__IRQ2_EOI_0_RESET__VALUE                          0x0
#define U_PNIP__IRQ2EVENT_LOW_1                                                   (0x10642200 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_INBOUND_LATE_ERR                             (0x80000000)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_FRAMEAGING_EXPIRED                           (0x20000000)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_HOL_EXPIRED_PB                               (0x10000000)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_HOL_EXPIRED_PA                               (0x08000000)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_HOL_EXPIRED_IFB                              (0x04000000)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_HOL_EXPIRED_IFA                              (0x02000000)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_QCW_EMPTY                                    (0x01000000)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_PHASECROSSED                                 (0x00200000)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_PHASETRANSFERERROR                           (0x00100000)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_FRAGFAULT                                    (0x00080000)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_IRT_DELAYSTATUS                              (0x00040000)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_PACK_ERR                                     (0x00020000)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_PPM_ERR                                      (0x00010000)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_CPM_ERR                                      (0x00008000)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_REQQUEUE_ERR                                 (0x00004000)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_MAC_ERR                                      (0x00002000)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_FILTERCMD_DONE                               (0x00000800)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_VLANCMD_DONE                                 (0x00000400)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_FDBCMD_DONE                                  (0x00000200)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_SYNCCMD_DONE                                 (0x00000100)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_PACKCMD_DONE                                 (0x00000080)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_PPMCMD_DONE                                  (0x00000040)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_CPMCMD_DONE                                  (0x00000020)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_ASNDCMD_DONE                                 (0x00000010)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_ARCVCMD_DONE                                 (0x00000008)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_REQDONE_IFB_2                                (0x00000004)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_REQDONE_IFA_2                                (0x00000002)
#define U_PNIP__IRQ2EVENT_LOW_1__INT_STATUSCHANGE                                 (0x00000001)
#define U_PNIP__IRQ2EVENT_LOW_1_RESET__VALUE                     0x0
#define U_PNIP__IRQ2EVENT_MID_1                                                   (0x10642204 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2EVENT_MID_1__EVENT_BITS_63_60                                 (0xF0000000)
#define U_PNIP__IRQ2EVENT_MID_1__PN_EXT_IRQ_3_0                                   (0x0F000000)
#define U_PNIP__IRQ2EVENT_MID_1__INT_PPM_DMA_ERR                                  (0x00200000)
#define U_PNIP__IRQ2EVENT_MID_1__INT_PPM_HOL_NOT_EMPTY_IFB                        (0x00080000)
#define U_PNIP__IRQ2EVENT_MID_1__INT_PPM_HOL_NOT_EMPTY_IFA                        (0x00040000)
#define U_PNIP__IRQ2EVENT_MID_1__INT_PPM_NOT_FINISHED_IFB                         (0x00020000)
#define U_PNIP__IRQ2EVENT_MID_1__INT_PPM_NOT_FINISHED_IFA                         (0x00010000)
#define U_PNIP__IRQ2EVENT_MID_1__INT_CSCOREBOARD_CHANGED_IFB                      (0x00008000)
#define U_PNIP__IRQ2EVENT_MID_1__INT_CSCOREBOARD_CHANGED_IFA                      (0x00004000)
#define U_PNIP__IRQ2EVENT_MID_1__INT_CPM_DMA_ERR                                  (0x00002000)
#define U_PNIP__IRQ2EVENT_MID_1__INT_CPM_OUTDATAERR                               (0x00000800)
#define U_PNIP__IRQ2EVENT_MID_1__INT_CPM_OUTDATAVALID                             (0x00000400)
#define U_PNIP__IRQ2EVENT_MID_1__INT_CPM_OUTDATASTART                             (0x00000200)
#define U_PNIP__IRQ2EVENT_MID_1__INT_ACYC_SND_ERR                                 (0x00000080)
#define U_PNIP__IRQ2EVENT_MID_1__INT_ACYC_RCV_ERR                                 (0x00000040)
#define U_PNIP__IRQ2EVENT_MID_1__INT_ACYC_SND_IFB_2                               (0x00000020)
#define U_PNIP__IRQ2EVENT_MID_1__INT_ACYC_RCV_IFB_2                               (0x00000010)
#define U_PNIP__IRQ2EVENT_MID_1__INT_ACYC_SND_IFA_2                               (0x00000008)
#define U_PNIP__IRQ2EVENT_MID_1__INT_ACYC_RCV_IFA_2                               (0x00000004)
#define U_PNIP__IRQ2EVENT_MID_1__INT_APIFILTER_ERR                                (0x00000001)
#define U_PNIP__IRQ2EVENT_MID_1_RESET__VALUE                     0x0
#define U_PNIP__IRQ2EVENT_HIGH_1                                                  (0x10642208 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2EVENT_HIGH_1__INT_I2C                                         (0x00100000)
#define U_PNIP__IRQ2EVENT_HIGH_1__INT_TIMER_2                                     (0x00080000)
#define U_PNIP__IRQ2EVENT_HIGH_1__INT_TIMER_1                                     (0x00040000)
#define U_PNIP__IRQ2EVENT_HIGH_1__INT_TIMER_0                                     (0x00020000)
#define U_PNIP__IRQ2EVENT_HIGH_1__INT_WD                                          (0x00010000)
#define U_PNIP__IRQ2EVENT_HIGH_1__INT_PTCP_WD                                     (0x00008000)
#define U_PNIP__IRQ2EVENT_HIGH_1__INT_PLL9                                        (0x00001000)
#define U_PNIP__IRQ2EVENT_HIGH_1__INT_PLL8                                        (0x00000800)
#define U_PNIP__IRQ2EVENT_HIGH_1__INT_PLL7                                        (0x00000400)
#define U_PNIP__IRQ2EVENT_HIGH_1__INT_PLL6                                        (0x00000200)
#define U_PNIP__IRQ2EVENT_HIGH_1__INT_PLL5                                        (0x00000100)
#define U_PNIP__IRQ2EVENT_HIGH_1__INT_PLL4                                        (0x00000080)
#define U_PNIP__IRQ2EVENT_HIGH_1__INT_PLL3                                        (0x00000040)
#define U_PNIP__IRQ2EVENT_HIGH_1__INT_PLL2                                        (0x00000020)
#define U_PNIP__IRQ2EVENT_HIGH_1__INT_PLL1                                        (0x00000010)
#define U_PNIP__IRQ2EVENT_HIGH_1__INT_PLL0                                        (0x00000008)
#define U_PNIP__IRQ2EVENT_HIGH_1__INT_PLL_IN_TIME                                 (0x00000004)
#define U_PNIP__IRQ2EVENT_HIGH_1__INT_PLL_IN_B                                    (0x00000002)
#define U_PNIP__IRQ2EVENT_HIGH_1__INT_PLL_IN_A                                    (0x00000001)
#define U_PNIP__IRQ2EVENT_HIGH_1_RESET__VALUE                    0x0
#define U_PNIP__IRQ2MASK_LOW_1                                                    (0x1064220C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2MASK_LOW_1__MASK_BITS_31_0                                    (0xFFFFFFFF)
#define U_PNIP__IRQ2MASK_LOW_1_RESET__VALUE                      0xFFFFFFFF
#define U_PNIP__IRQ2MASK_MID_1                                                    (0x10642210 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2MASK_MID_1__MASK_BITS_63_32                                   (0xFFFFFFFF)
#define U_PNIP__IRQ2MASK_MID_1_RESET__VALUE                      0xFFFFFFFF
#define U_PNIP__IRQ2MASK_HIGH_1                                                   (0x10642214 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2MASK_HIGH_1__MASK_BITS_85_64                                  (0x003FFFFF)
#define U_PNIP__IRQ2MASK_HIGH_1_RESET__VALUE                     0x3FFFFF
#define U_PNIP__IRQ2_LOW_1                                                        (0x10642218 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2_LOW_1__IRQ_BITS_31_0                                         (0xFFFFFFFF)
#define U_PNIP__IRQ2_LOW_1_RESET__VALUE                          0x0
#define U_PNIP__IRQ2_MID_1                                                        (0x1064221C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2_MID_1__IRQ_BITS_63_32                                        (0xFFFFFFFF)
#define U_PNIP__IRQ2_MID_1_RESET__VALUE                          0x0
#define U_PNIP__IRQ2_HIGH_1                                                       (0x10642220 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2_HIGH_1__IRQ_BITS_85_64                                       (0x003FFFFF)
#define U_PNIP__IRQ2_HIGH_1_RESET__VALUE                         0x0
#define U_PNIP__IRQ2ACK_LOW_1                                                     (0x10642224 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2ACK_LOW_1__ACK_BITS_31_0                                      (0xFFFFFFFF)
#define U_PNIP__IRQ2ACK_LOW_1_RESET__VALUE                       0x0
#define U_PNIP__IRQ2ACK_MID_1                                                     (0x10642228 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2ACK_MID_1__ACK_BITS_63_32                                     (0xFFFFFFFF)
#define U_PNIP__IRQ2ACK_MID_1_RESET__VALUE                       0x0
#define U_PNIP__IRQ2ACK_HIGH_1                                                    (0x1064222C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2ACK_HIGH_1__ACK_BITS_85_64                                    (0x003FFFFF)
#define U_PNIP__IRQ2ACK_HIGH_1_RESET__VALUE                      0x0
#define U_PNIP__IRQ2_EOI_1                                                        (0x10642230 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2_EOI_1__WAIT_TIME                                             (0x00003FFF)
#define U_PNIP__IRQ2_EOI_1_RESET__VALUE                          0x0
#define U_PNIP__IRQ2CONTROL_MUX2                                                  (0x10642300 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2CONTROL_MUX2__IRQ_WAITTIME                                    (0x01FF0000)
#define U_PNIP__IRQ2CONTROL_MUX2__IRQ_EVENT                                       (0x00000100)
#define U_PNIP__IRQ2CONTROL_MUX2__IRQ_SELECTMUX                                   (0x0000007F)
#define U_PNIP__IRQ2CONTROL_MUX2_RESET__VALUE                    0x7F
#define U_PNIP__IRQ2CONTROL_MUX3                                                  (0x10642304 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2CONTROL_MUX3__IRQ_WAITTIME                                    (0x01FF0000)
#define U_PNIP__IRQ2CONTROL_MUX3__IRQ_EVENT                                       (0x00000100)
#define U_PNIP__IRQ2CONTROL_MUX3__IRQ_SELECTMUX                                   (0x0000007F)
#define U_PNIP__IRQ2CONTROL_MUX3_RESET__VALUE                    0x7F
#define U_PNIP__IRQ2CONTROL_MUX4                                                  (0x10642308 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2CONTROL_MUX4__IRQ_WAITTIME                                    (0x01FF0000)
#define U_PNIP__IRQ2CONTROL_MUX4__IRQ_EVENT                                       (0x00000100)
#define U_PNIP__IRQ2CONTROL_MUX4__IRQ_SELECTMUX                                   (0x0000007F)
#define U_PNIP__IRQ2CONTROL_MUX4_RESET__VALUE                    0x7F
#define U_PNIP__IRQ2CONTROL_MUX5                                                  (0x1064230C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2CONTROL_MUX5__IRQ_WAITTIME                                    (0x01FF0000)
#define U_PNIP__IRQ2CONTROL_MUX5__IRQ_EVENT                                       (0x00000100)
#define U_PNIP__IRQ2CONTROL_MUX5__IRQ_SELECTMUX                                   (0x0000007F)
#define U_PNIP__IRQ2CONTROL_MUX5_RESET__VALUE                    0x7F
#define U_PNIP__IRQ2CONTROL_MUX6                                                  (0x10642310 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2CONTROL_MUX6__IRQ_WAITTIME                                    (0x01FF0000)
#define U_PNIP__IRQ2CONTROL_MUX6__IRQ_EVENT                                       (0x00000100)
#define U_PNIP__IRQ2CONTROL_MUX6__IRQ_SELECTMUX                                   (0x0000007F)
#define U_PNIP__IRQ2CONTROL_MUX6_RESET__VALUE                    0x7F
#define U_PNIP__IRQ2CONTROL_MUX7                                                  (0x10642314 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2CONTROL_MUX7__IRQ_WAITTIME                                    (0x01FF0000)
#define U_PNIP__IRQ2CONTROL_MUX7__IRQ_EVENT                                       (0x00000100)
#define U_PNIP__IRQ2CONTROL_MUX7__IRQ_SELECTMUX                                   (0x0000007F)
#define U_PNIP__IRQ2CONTROL_MUX7_RESET__VALUE                    0x7F
#define U_PNIP__IRQ2CONTROL_MUX8                                                  (0x10642318 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2CONTROL_MUX8__IRQ_WAITTIME                                    (0x01FF0000)
#define U_PNIP__IRQ2CONTROL_MUX8__IRQ_EVENT                                       (0x00000100)
#define U_PNIP__IRQ2CONTROL_MUX8__IRQ_SELECTMUX                                   (0x0000007F)
#define U_PNIP__IRQ2CONTROL_MUX8_RESET__VALUE                    0x7F
#define U_PNIP__IRQ2CONTROL_MUX9                                                  (0x1064231C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2CONTROL_MUX9__IRQ_WAITTIME                                    (0x01FF0000)
#define U_PNIP__IRQ2CONTROL_MUX9__IRQ_EVENT                                       (0x00000100)
#define U_PNIP__IRQ2CONTROL_MUX9__IRQ_SELECTMUX                                   (0x0000007F)
#define U_PNIP__IRQ2CONTROL_MUX9_RESET__VALUE                    0x7F
#define U_PNIP__IRQ2CONTROL_MUX10                                                 (0x10642320 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2CONTROL_MUX10__IRQ_WAITTIME                                   (0x01FF0000)
#define U_PNIP__IRQ2CONTROL_MUX10__IRQ_EVENT                                      (0x00000100)
#define U_PNIP__IRQ2CONTROL_MUX10__IRQ_SELECTMUX                                  (0x0000007F)
#define U_PNIP__IRQ2CONTROL_MUX10_RESET__VALUE                   0x7F
#define U_PNIP__IRQ2CONTROL_MUX11                                                 (0x10642324 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2CONTROL_MUX11__IRQ_WAITTIME                                   (0x01FF0000)
#define U_PNIP__IRQ2CONTROL_MUX11__IRQ_EVENT                                      (0x00000100)
#define U_PNIP__IRQ2CONTROL_MUX11__IRQ_SELECTMUX                                  (0x0000007F)
#define U_PNIP__IRQ2CONTROL_MUX11_RESET__VALUE                   0x7F
#define U_PNIP__IRQ2CONTROL_MUX12                                                 (0x10642328 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2CONTROL_MUX12__IRQ_WAITTIME                                   (0x01FF0000)
#define U_PNIP__IRQ2CONTROL_MUX12__IRQ_EVENT                                      (0x00000100)
#define U_PNIP__IRQ2CONTROL_MUX12__IRQ_SELECTMUX                                  (0x0000007F)
#define U_PNIP__IRQ2CONTROL_MUX12_RESET__VALUE                   0x7F
#define U_PNIP__IRQ2CONTROL_MUX13                                                 (0x1064232C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2CONTROL_MUX13__IRQ_WAITTIME                                   (0x01FF0000)
#define U_PNIP__IRQ2CONTROL_MUX13__IRQ_EVENT                                      (0x00000100)
#define U_PNIP__IRQ2CONTROL_MUX13__IRQ_SELECTMUX                                  (0x0000007F)
#define U_PNIP__IRQ2CONTROL_MUX13_RESET__VALUE                   0x7F
#define U_PNIP__IRQ2CONTROL_MUX14                                                 (0x10642330 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2CONTROL_MUX14__IRQ_WAITTIME                                   (0x01FF0000)
#define U_PNIP__IRQ2CONTROL_MUX14__IRQ_EVENT                                      (0x00000100)
#define U_PNIP__IRQ2CONTROL_MUX14__IRQ_SELECTMUX                                  (0x0000007F)
#define U_PNIP__IRQ2CONTROL_MUX14_RESET__VALUE                   0x7F
#define U_PNIP__IRQ2CONTROL_MUX15                                                 (0x10642334 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ2CONTROL_MUX15__IRQ_WAITTIME                                   (0x01FF0000)
#define U_PNIP__IRQ2CONTROL_MUX15__IRQ_EVENT                                      (0x00000100)
#define U_PNIP__IRQ2CONTROL_MUX15__IRQ_SELECTMUX                                  (0x0000007F)
#define U_PNIP__IRQ2CONTROL_MUX15_RESET__VALUE                   0x7F
#define U_PNIP__IRQ3_CONTROL                                                      (0x10643000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3_CONTROL__IRQDEBUG_ENABLE                                     (0x00100000)
#define U_PNIP__IRQ3_CONTROL__IRQDEBUG_SELECT                                     (0x000F0000)
#define U_PNIP__IRQ3_CONTROL__IRQ_LENGTH                                          (0x0000000E)
#define U_PNIP__IRQ3_CONTROL__IRQ_LEVEL                                           (0x00000001)
#define U_PNIP__IRQ3_CONTROL_RESET__VALUE                        0x1
#define U_PNIP__IRQ3_ACTIVATE                                                     (0x10643004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3_ACTIVATE__ACTIVATE                                           (0xFFFFFFFF)
#define U_PNIP__IRQ3_ACTIVATE_RESET__VALUE                       0x0
#define U_PNIP__IRQ3_TIMESTAMP                                                    (0x10643008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3_TIMESTAMP__TIMER_STAMP_31_3                                  (0xFFFFFFF8)
#define U_PNIP__IRQ3_TIMESTAMP__TIMER_STAMP_2_0                                   (0x00000007)
#define U_PNIP__IRQ3_TIMESTAMP_RESET__VALUE                      0x0
#define U_PNIP__IRQ3EVENT_LOW_0                                                   (0x10643100 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_INBOUND_LATE_ERR                             (0x80000000)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_FRAMEAGING_EXPIRED                           (0x20000000)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_HOL_EXPIRED_PB                               (0x10000000)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_HOL_EXPIRED_PA                               (0x08000000)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_HOL_EXPIRED_IFB                              (0x04000000)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_HOL_EXPIRED_IFA                              (0x02000000)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_QCW_EMPTY                                    (0x01000000)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_PHASECROSSED                                 (0x00200000)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_PHASETRANSFERERROR                           (0x00100000)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_FRAGFAULT                                    (0x00080000)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_IRT_DELAYSTATUS                              (0x00040000)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_PACK_ERR                                     (0x00020000)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_PPM_ERR                                      (0x00010000)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_CPM_ERR                                      (0x00008000)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_REQQUEUE_ERR                                 (0x00004000)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_MAC_ERR                                      (0x00002000)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_FILTERCMD_DONE                               (0x00000800)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_VLANCMD_DONE                                 (0x00000400)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_FDBCMD_DONE                                  (0x00000200)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_SYNCCMD_DONE                                 (0x00000100)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_PACKCMD_DONE                                 (0x00000080)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_PPMCMD_DONE                                  (0x00000040)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_CPMCMD_DONE                                  (0x00000020)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_ASNDCMD_DONE                                 (0x00000010)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_ARCVCMD_DONE                                 (0x00000008)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_REQDONE_IFB_3                                (0x00000004)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_REQDONE_IFA_3                                (0x00000002)
#define U_PNIP__IRQ3EVENT_LOW_0__INT_STATUSCHANGE                                 (0x00000001)
#define U_PNIP__IRQ3EVENT_LOW_0_RESET__VALUE                     0x0
#define U_PNIP__IRQ3EVENT_MID_0                                                   (0x10643104 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3EVENT_MID_0__EVENT_BITS_63_60                                 (0xF0000000)
#define U_PNIP__IRQ3EVENT_MID_0__PN_EXT_IRQ_3_0                                   (0x0F000000)
#define U_PNIP__IRQ3EVENT_MID_0__INT_PPM_DMA_ERR                                  (0x00200000)
#define U_PNIP__IRQ3EVENT_MID_0__INT_PPM_HOL_NOT_EMPTY_IFB                        (0x00080000)
#define U_PNIP__IRQ3EVENT_MID_0__INT_PPM_HOL_NOT_EMPTY_IFA                        (0x00040000)
#define U_PNIP__IRQ3EVENT_MID_0__INT_PPM_NOT_FINISHED_IFB                         (0x00020000)
#define U_PNIP__IRQ3EVENT_MID_0__INT_PPM_NOT_FINISHED_IFA                         (0x00010000)
#define U_PNIP__IRQ3EVENT_MID_0__INT_CSCOREBOARD_CHANGED_IFB                      (0x00008000)
#define U_PNIP__IRQ3EVENT_MID_0__INT_CSCOREBOARD_CHANGED_IFA                      (0x00004000)
#define U_PNIP__IRQ3EVENT_MID_0__INT_CPM_DMA_ERR                                  (0x00002000)
#define U_PNIP__IRQ3EVENT_MID_0__INT_CPM_OUTDATAERR                               (0x00000800)
#define U_PNIP__IRQ3EVENT_MID_0__INT_CPM_OUTDATAVALID                             (0x00000400)
#define U_PNIP__IRQ3EVENT_MID_0__INT_CPM_OUTDATASTART                             (0x00000200)
#define U_PNIP__IRQ3EVENT_MID_0__INT_ACYC_SND_ERR                                 (0x00000080)
#define U_PNIP__IRQ3EVENT_MID_0__INT_ACYC_RCV_ERR                                 (0x00000040)
#define U_PNIP__IRQ3EVENT_MID_0__INT_ACYC_SND_IFB_3                               (0x00000020)
#define U_PNIP__IRQ3EVENT_MID_0__INT_ACYC_RCV_IFB_3                               (0x00000010)
#define U_PNIP__IRQ3EVENT_MID_0__INT_ACYC_SND_IFA_3                               (0x00000008)
#define U_PNIP__IRQ3EVENT_MID_0__INT_ACYC_RCV_IFA_3                               (0x00000004)
#define U_PNIP__IRQ3EVENT_MID_0__INT_APIFILTER_ERR                                (0x00000001)
#define U_PNIP__IRQ3EVENT_MID_0_RESET__VALUE                     0x0
#define U_PNIP__IRQ3EVENT_HIGH_0                                                  (0x10643108 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3EVENT_HIGH_0__INT_I2C                                         (0x00100000)
#define U_PNIP__IRQ3EVENT_HIGH_0__INT_TIMER_2                                     (0x00080000)
#define U_PNIP__IRQ3EVENT_HIGH_0__INT_TIMER_1                                     (0x00040000)
#define U_PNIP__IRQ3EVENT_HIGH_0__INT_TIMER_0                                     (0x00020000)
#define U_PNIP__IRQ3EVENT_HIGH_0__INT_WD                                          (0x00010000)
#define U_PNIP__IRQ3EVENT_HIGH_0__INT_PLL9                                        (0x00001000)
#define U_PNIP__IRQ3EVENT_HIGH_0__INT_PLL8                                        (0x00000800)
#define U_PNIP__IRQ3EVENT_HIGH_0__INT_PLL7                                        (0x00000400)
#define U_PNIP__IRQ3EVENT_HIGH_0__INT_PLL6                                        (0x00000200)
#define U_PNIP__IRQ3EVENT_HIGH_0__INT_PLL5                                        (0x00000100)
#define U_PNIP__IRQ3EVENT_HIGH_0__INT_PLL4                                        (0x00000080)
#define U_PNIP__IRQ3EVENT_HIGH_0__INT_PLL3                                        (0x00000040)
#define U_PNIP__IRQ3EVENT_HIGH_0__INT_PLL2                                        (0x00000020)
#define U_PNIP__IRQ3EVENT_HIGH_0__INT_PLL1                                        (0x00000010)
#define U_PNIP__IRQ3EVENT_HIGH_0__INT_PLL0                                        (0x00000008)
#define U_PNIP__IRQ3EVENT_HIGH_0__INT_PLL_IN_TIME                                 (0x00000004)
#define U_PNIP__IRQ3EVENT_HIGH_0__INT_PLL_IN_B                                    (0x00000002)
#define U_PNIP__IRQ3EVENT_HIGH_0__INT_PLL_IN_A                                    (0x00000001)
#define U_PNIP__IRQ3EVENT_HIGH_0_RESET__VALUE                    0x0
#define U_PNIP__IRQ3MASK_LOW_0                                                    (0x1064310C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3MASK_LOW_0__MASK_BITS_31_0                                    (0xFFFFFFFF)
#define U_PNIP__IRQ3MASK_LOW_0_RESET__VALUE                      0xFFFFFFFF
#define U_PNIP__IRQ3MASK_MID_0                                                    (0x10643110 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3MASK_MID_0__MASK_BITS_63_32                                   (0xFFFFFFFF)
#define U_PNIP__IRQ3MASK_MID_0_RESET__VALUE                      0xFFFFFFFF
#define U_PNIP__IRQ3MASK_HIGH_0                                                   (0x10643114 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3MASK_HIGH_0__MASK_BITS_85_64                                  (0x003FFFFF)
#define U_PNIP__IRQ3MASK_HIGH_0_RESET__VALUE                     0x3FFFFF
#define U_PNIP__IRQ3_LOW_0                                                        (0x10643118 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3_LOW_0__IRQ_BITS_31_0                                         (0xFFFFFFFF)
#define U_PNIP__IRQ3_LOW_0_RESET__VALUE                          0x0
#define U_PNIP__IRQ3_MID_0                                                        (0x1064311C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3_MID_0__IRQ_BITS_63_32                                        (0xFFFFFFFF)
#define U_PNIP__IRQ3_MID_0_RESET__VALUE                          0x0
#define U_PNIP__IRQ3_HIGH_0                                                       (0x10643120 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3_HIGH_0__IRQ_BITS_85_64                                       (0x003FFFFF)
#define U_PNIP__IRQ3_HIGH_0_RESET__VALUE                         0x0
#define U_PNIP__IRQ3ACK_LOW_0                                                     (0x10643124 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3ACK_LOW_0__ACK_BITS_31_0                                      (0xFFFFFFFF)
#define U_PNIP__IRQ3ACK_LOW_0_RESET__VALUE                       0x0
#define U_PNIP__IRQ3ACK_MID_0                                                     (0x10643128 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3ACK_MID_0__ACK_BITS_63_32                                     (0xFFFFFFFF)
#define U_PNIP__IRQ3ACK_MID_0_RESET__VALUE                       0x0
#define U_PNIP__IRQ3ACK_HIGH_0                                                    (0x1064312C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3ACK_HIGH_0__ACK_BITS_85_64                                    (0x003FFFFF)
#define U_PNIP__IRQ3ACK_HIGH_0_RESET__VALUE                      0x0
#define U_PNIP__IRQ3_EOI_0                                                        (0x10643130 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3_EOI_0__WAIT_TIME                                             (0x00003FFF)
#define U_PNIP__IRQ3_EOI_0_RESET__VALUE                          0x0
#define U_PNIP__IRQ3EVENT_LOW_1                                                   (0x10643200 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_INBOUND_LATE_ERR                             (0x80000000)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_FRAMEAGING_EXPIRED                           (0x20000000)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_HOL_EXPIRED_PB                               (0x10000000)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_HOL_EXPIRED_PA                               (0x08000000)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_HOL_EXPIRED_IFB                              (0x04000000)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_HOL_EXPIRED_IFA                              (0x02000000)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_QCW_EMPTY                                    (0x01000000)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_PHASECROSSED                                 (0x00200000)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_PHASETRANSFERERROR                           (0x00100000)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_FRAGFAULT                                    (0x00080000)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_IRT_DELAYSTATUS                              (0x00040000)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_PACK_ERR                                     (0x00020000)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_PPM_ERR                                      (0x00010000)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_CPM_ERR                                      (0x00008000)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_REQQUEUE_ERR                                 (0x00004000)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_MAC_ERR                                      (0x00002000)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_FILTERCMD_DONE                               (0x00000800)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_VLANCMD_DONE                                 (0x00000400)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_FDBCMD_DONE                                  (0x00000200)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_SYNCCMD_DONE                                 (0x00000100)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_PACKCMD_DONE                                 (0x00000080)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_PPMCMD_DONE                                  (0x00000040)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_CPMCMD_DONE                                  (0x00000020)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_ASNDCMD_DONE                                 (0x00000010)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_ARCVCMD_DONE                                 (0x00000008)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_REQDONE_IFB_3                                (0x00000004)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_REQDONE_IFA_3                                (0x00000002)
#define U_PNIP__IRQ3EVENT_LOW_1__INT_STATUSCHANGE                                 (0x00000001)
#define U_PNIP__IRQ3EVENT_LOW_1_RESET__VALUE                     0x0
#define U_PNIP__IRQ3EVENT_MID_1                                                   (0x10643204 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3EVENT_MID_1__EVENT_BITS_63_60                                 (0xF0000000)
#define U_PNIP__IRQ3EVENT_MID_1__PN_EXT_IRQ_3_0                                   (0x0F000000)
#define U_PNIP__IRQ3EVENT_MID_1__INT_PPM_DMA_ERR                                  (0x00200000)
#define U_PNIP__IRQ3EVENT_MID_1__INT_PPM_HOL_NOT_EMPTY_IFB                        (0x00080000)
#define U_PNIP__IRQ3EVENT_MID_1__INT_PPM_HOL_NOT_EMPTY_IFA                        (0x00040000)
#define U_PNIP__IRQ3EVENT_MID_1__INT_PPM_NOT_FINISHED_IFB                         (0x00020000)
#define U_PNIP__IRQ3EVENT_MID_1__INT_PPM_NOT_FINISHED_IFA                         (0x00010000)
#define U_PNIP__IRQ3EVENT_MID_1__INT_CSCOREBOARD_CHANGED_IFB                      (0x00008000)
#define U_PNIP__IRQ3EVENT_MID_1__INT_CSCOREBOARD_CHANGED_IFA                      (0x00004000)
#define U_PNIP__IRQ3EVENT_MID_1__INT_CPM_DMA_ERR                                  (0x00002000)
#define U_PNIP__IRQ3EVENT_MID_1__INT_CPM_OUTDATAERR                               (0x00000800)
#define U_PNIP__IRQ3EVENT_MID_1__INT_CPM_OUTDATAVALID                             (0x00000400)
#define U_PNIP__IRQ3EVENT_MID_1__INT_CPM_OUTDATASTART                             (0x00000200)
#define U_PNIP__IRQ3EVENT_MID_1__INT_ACYC_SND_ERR                                 (0x00000080)
#define U_PNIP__IRQ3EVENT_MID_1__INT_ACYC_RCV_ERR                                 (0x00000040)
#define U_PNIP__IRQ3EVENT_MID_1__INT_ACYC_SND_IFB_3                               (0x00000020)
#define U_PNIP__IRQ3EVENT_MID_1__INT_ACYC_RCV_IFB_3                               (0x00000010)
#define U_PNIP__IRQ3EVENT_MID_1__INT_ACYC_SND_IFA_3                               (0x00000008)
#define U_PNIP__IRQ3EVENT_MID_1__INT_ACYC_RCV_IFA_3                               (0x00000004)
#define U_PNIP__IRQ3EVENT_MID_1__INT_APIFILTER_ERR                                (0x00000001)
#define U_PNIP__IRQ3EVENT_MID_1_RESET__VALUE                     0x0
#define U_PNIP__IRQ3EVENT_HIGH_1                                                  (0x10643208 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3EVENT_HIGH_1__INT_TIMER_2                                     (0x00080000)
#define U_PNIP__IRQ3EVENT_HIGH_1__INT_TIMER_1                                     (0x00040000)
#define U_PNIP__IRQ3EVENT_HIGH_1__INT_TIMER_0                                     (0x00020000)
#define U_PNIP__IRQ3EVENT_HIGH_1__INT_WD                                          (0x00010000)
#define U_PNIP__IRQ3EVENT_HIGH_1__INT_PLL9                                        (0x00001000)
#define U_PNIP__IRQ3EVENT_HIGH_1__INT_PLL8                                        (0x00000800)
#define U_PNIP__IRQ3EVENT_HIGH_1__INT_PLL7                                        (0x00000400)
#define U_PNIP__IRQ3EVENT_HIGH_1__INT_PLL6                                        (0x00000200)
#define U_PNIP__IRQ3EVENT_HIGH_1__INT_PLL5                                        (0x00000100)
#define U_PNIP__IRQ3EVENT_HIGH_1__INT_PLL4                                        (0x00000080)
#define U_PNIP__IRQ3EVENT_HIGH_1__INT_PLL3                                        (0x00000040)
#define U_PNIP__IRQ3EVENT_HIGH_1__INT_PLL2                                        (0x00000020)
#define U_PNIP__IRQ3EVENT_HIGH_1__INT_PLL1                                        (0x00000010)
#define U_PNIP__IRQ3EVENT_HIGH_1__INT_PLL0                                        (0x00000008)
#define U_PNIP__IRQ3EVENT_HIGH_1__INT_PLL_IN_TIME                                 (0x00000004)
#define U_PNIP__IRQ3EVENT_HIGH_1__INT_PLL_IN_B                                    (0x00000002)
#define U_PNIP__IRQ3EVENT_HIGH_1__INT_PLL_IN_A                                    (0x00000001)
#define U_PNIP__IRQ3EVENT_HIGH_1_RESET__VALUE                    0x0
#define U_PNIP__IRQ3MASK_LOW_1                                                    (0x1064320C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3MASK_LOW_1__MASK_BITS_31_0                                    (0xFFFFFFFF)
#define U_PNIP__IRQ3MASK_LOW_1_RESET__VALUE                      0xFFFFFFFF
#define U_PNIP__IRQ3MASK_MID_1                                                    (0x10643210 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3MASK_MID_1__MASK_BITS_63_32                                   (0xFFFFFFFF)
#define U_PNIP__IRQ3MASK_MID_1_RESET__VALUE                      0xFFFFFFFF
#define U_PNIP__IRQ3MASK_HIGH_1                                                   (0x10643214 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3MASK_HIGH_1__MASK_BITS_85_64                                  (0x003FFFFF)
#define U_PNIP__IRQ3MASK_HIGH_1_RESET__VALUE                     0x3FFFFF
#define U_PNIP__IRQ3_LOW_1                                                        (0x10643218 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3_LOW_1__IRQ_BITS_31_0                                         (0xFFFFFFFF)
#define U_PNIP__IRQ3_LOW_1_RESET__VALUE                          0x0
#define U_PNIP__IRQ3_MID_1                                                        (0x1064321C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3_MID_1__IRQ_BITS_63_32                                        (0xFFFFFFFF)
#define U_PNIP__IRQ3_MID_1_RESET__VALUE                          0x0
#define U_PNIP__IRQ3_HIGH_1                                                       (0x10643220 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3_HIGH_1__IRQ_BITS_85_64                                       (0x003FFFFF)
#define U_PNIP__IRQ3_HIGH_1_RESET__VALUE                         0x0
#define U_PNIP__IRQ3ACK_LOW_1                                                     (0x10643224 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3ACK_LOW_1__ACK_BITS_31_0                                      (0xFFFFFFFF)
#define U_PNIP__IRQ3ACK_LOW_1_RESET__VALUE                       0x0
#define U_PNIP__IRQ3ACK_MID_1                                                     (0x10643228 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3ACK_MID_1__ACK_BITS_63_32                                     (0xFFFFFFFF)
#define U_PNIP__IRQ3ACK_MID_1_RESET__VALUE                       0x0
#define U_PNIP__IRQ3ACK_HIGH_1                                                    (0x1064322C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3ACK_HIGH_1__ACK_BITS_85_64                                    (0x003FFFFF)
#define U_PNIP__IRQ3ACK_HIGH_1_RESET__VALUE                      0x0
#define U_PNIP__IRQ3_EOI_1                                                        (0x10643230 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3_EOI_1__WAIT_TIME                                             (0x00003FFF)
#define U_PNIP__IRQ3_EOI_1_RESET__VALUE                          0x0
#define U_PNIP__IRQ3CONTROL_MUX2                                                  (0x10643300 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3CONTROL_MUX2__IRQ_WAITTIME                                    (0x01FF0000)
#define U_PNIP__IRQ3CONTROL_MUX2__IRQ_EVENT                                       (0x00000100)
#define U_PNIP__IRQ3CONTROL_MUX2__IRQ_SELECTMUX                                   (0x0000007F)
#define U_PNIP__IRQ3CONTROL_MUX2_RESET__VALUE                    0x7F
#define U_PNIP__IRQ3CONTROL_MUX3                                                  (0x10643304 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3CONTROL_MUX3__IRQ_WAITTIME                                    (0x01FF0000)
#define U_PNIP__IRQ3CONTROL_MUX3__IRQ_EVENT                                       (0x00000100)
#define U_PNIP__IRQ3CONTROL_MUX3__IRQ_SELECTMUX                                   (0x0000007F)
#define U_PNIP__IRQ3CONTROL_MUX3_RESET__VALUE                    0x7F
#define U_PNIP__IRQ3CONTROL_MUX4                                                  (0x10643308 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3CONTROL_MUX4__IRQ_WAITTIME                                    (0x01FF0000)
#define U_PNIP__IRQ3CONTROL_MUX4__IRQ_EVENT                                       (0x00000100)
#define U_PNIP__IRQ3CONTROL_MUX4__IRQ_SELECTMUX                                   (0x0000007F)
#define U_PNIP__IRQ3CONTROL_MUX4_RESET__VALUE                    0x7F
#define U_PNIP__IRQ3CONTROL_MUX5                                                  (0x1064330C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3CONTROL_MUX5__IRQ_WAITTIME                                    (0x01FF0000)
#define U_PNIP__IRQ3CONTROL_MUX5__IRQ_EVENT                                       (0x00000100)
#define U_PNIP__IRQ3CONTROL_MUX5__IRQ_SELECTMUX                                   (0x0000007F)
#define U_PNIP__IRQ3CONTROL_MUX5_RESET__VALUE                    0x7F
#define U_PNIP__IRQ3CONTROL_MUX6                                                  (0x10643310 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3CONTROL_MUX6__IRQ_WAITTIME                                    (0x01FF0000)
#define U_PNIP__IRQ3CONTROL_MUX6__IRQ_EVENT                                       (0x00000100)
#define U_PNIP__IRQ3CONTROL_MUX6__IRQ_SELECTMUX                                   (0x0000007F)
#define U_PNIP__IRQ3CONTROL_MUX6_RESET__VALUE                    0x7F
#define U_PNIP__IRQ3CONTROL_MUX7                                                  (0x10643314 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3CONTROL_MUX7__IRQ_WAITTIME                                    (0x01FF0000)
#define U_PNIP__IRQ3CONTROL_MUX7__IRQ_EVENT                                       (0x00000100)
#define U_PNIP__IRQ3CONTROL_MUX7__IRQ_SELECTMUX                                   (0x0000007F)
#define U_PNIP__IRQ3CONTROL_MUX7_RESET__VALUE                    0x7F
#define U_PNIP__IRQ3CONTROL_MUX8                                                  (0x10643318 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3CONTROL_MUX8__IRQ_WAITTIME                                    (0x01FF0000)
#define U_PNIP__IRQ3CONTROL_MUX8__IRQ_EVENT                                       (0x00000100)
#define U_PNIP__IRQ3CONTROL_MUX8__IRQ_SELECTMUX                                   (0x0000007F)
#define U_PNIP__IRQ3CONTROL_MUX8_RESET__VALUE                    0x7F
#define U_PNIP__IRQ3CONTROL_MUX9                                                  (0x1064331C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3CONTROL_MUX9__IRQ_WAITTIME                                    (0x01FF0000)
#define U_PNIP__IRQ3CONTROL_MUX9__IRQ_EVENT                                       (0x00000100)
#define U_PNIP__IRQ3CONTROL_MUX9__IRQ_SELECTMUX                                   (0x0000007F)
#define U_PNIP__IRQ3CONTROL_MUX9_RESET__VALUE                    0x7F
#define U_PNIP__IRQ3CONTROL_MUX10                                                 (0x10643320 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3CONTROL_MUX10__IRQ_WAITTIME                                   (0x01FF0000)
#define U_PNIP__IRQ3CONTROL_MUX10__IRQ_EVENT                                      (0x00000100)
#define U_PNIP__IRQ3CONTROL_MUX10__IRQ_SELECTMUX                                  (0x0000007F)
#define U_PNIP__IRQ3CONTROL_MUX10_RESET__VALUE                   0x7F
#define U_PNIP__IRQ3CONTROL_MUX11                                                 (0x10643324 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3CONTROL_MUX11__IRQ_WAITTIME                                   (0x01FF0000)
#define U_PNIP__IRQ3CONTROL_MUX11__IRQ_EVENT                                      (0x00000100)
#define U_PNIP__IRQ3CONTROL_MUX11__IRQ_SELECTMUX                                  (0x0000007F)
#define U_PNIP__IRQ3CONTROL_MUX11_RESET__VALUE                   0x7F
#define U_PNIP__IRQ3CONTROL_MUX12                                                 (0x10643328 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3CONTROL_MUX12__IRQ_WAITTIME                                   (0x01FF0000)
#define U_PNIP__IRQ3CONTROL_MUX12__IRQ_EVENT                                      (0x00000100)
#define U_PNIP__IRQ3CONTROL_MUX12__IRQ_SELECTMUX                                  (0x0000007F)
#define U_PNIP__IRQ3CONTROL_MUX12_RESET__VALUE                   0x7F
#define U_PNIP__IRQ3CONTROL_MUX13                                                 (0x1064332C + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3CONTROL_MUX13__IRQ_WAITTIME                                   (0x01FF0000)
#define U_PNIP__IRQ3CONTROL_MUX13__IRQ_EVENT                                      (0x00000100)
#define U_PNIP__IRQ3CONTROL_MUX13__IRQ_SELECTMUX                                  (0x0000007F)
#define U_PNIP__IRQ3CONTROL_MUX13_RESET__VALUE                   0x7F
#define U_PNIP__IRQ3CONTROL_MUX14                                                 (0x10643330 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3CONTROL_MUX14__IRQ_WAITTIME                                   (0x01FF0000)
#define U_PNIP__IRQ3CONTROL_MUX14__IRQ_EVENT                                      (0x00000100)
#define U_PNIP__IRQ3CONTROL_MUX14__IRQ_SELECTMUX                                  (0x0000007F)
#define U_PNIP__IRQ3CONTROL_MUX14_RESET__VALUE                   0x7F
#define U_PNIP__IRQ3CONTROL_MUX15                                                 (0x10643334 + PNIP_DEV_TOP_BASE)
#define U_PNIP__IRQ3CONTROL_MUX15__IRQ_WAITTIME                                   (0x01FF0000)
#define U_PNIP__IRQ3CONTROL_MUX15__IRQ_EVENT                                      (0x00000100)
#define U_PNIP__IRQ3CONTROL_MUX15__IRQ_SELECTMUX                                  (0x0000007F)
#define U_PNIP__IRQ3CONTROL_MUX15_RESET__VALUE                   0x7F
#define U_PNIP__INBOUNDPACK_CONTROL_0_0                                           (0x10650000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUNDPACK_CONTROL_0_0__INBOUND_RXPORT_DIS                       (0x000F0000)
#define U_PNIP__INBOUNDPACK_CONTROL_0_0__AUTOSTOP_MAX                             (0x00007F00)
#define U_PNIP__INBOUNDPACK_CONTROL_0_0__LBUF_WDOG_MAX                            (0x0000007F)
#define U_PNIP__INBOUNDPACK_CONTROL_0_0_RESET__VALUE             0x0
#define U_PNIP__INBOUNDPACK_CONTROL_0_1                                           (0x10650004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUNDPACK_CONTROL_0_1__INBOUND_RXPORT_DIS                       (0x000F0000)
#define U_PNIP__INBOUNDPACK_CONTROL_0_1__AUTOSTOP_MAX                             (0x00007F00)
#define U_PNIP__INBOUNDPACK_CONTROL_0_1__LBUF_WDOG_MAX                            (0x0000007F)
#define U_PNIP__INBOUNDPACK_CONTROL_0_1_RESET__VALUE             0x0
#define U_PNIP__INBOUNDPACK_CONTROL_0_2                                           (0x10650008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUNDPACK_CONTROL_0_2__INBOUND_RXPORT_DIS                       (0x000F0000)
#define U_PNIP__INBOUNDPACK_CONTROL_0_2__AUTOSTOP_MAX                             (0x00007F00)
#define U_PNIP__INBOUNDPACK_CONTROL_0_2__LBUF_WDOG_MAX                            (0x0000007F)
#define U_PNIP__INBOUNDPACK_CONTROL_0_2_RESET__VALUE             0x0
#define U_PNIP__INBOUNDPACK_CONTROL_0_3                                           (0x1065000C + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUNDPACK_CONTROL_0_3__INBOUND_RXPORT_DIS                       (0x000F0000)
#define U_PNIP__INBOUNDPACK_CONTROL_0_3__AUTOSTOP_MAX                             (0x00007F00)
#define U_PNIP__INBOUNDPACK_CONTROL_0_3__LBUF_WDOG_MAX                            (0x0000007F)
#define U_PNIP__INBOUNDPACK_CONTROL_0_3_RESET__VALUE             0x0
#define U_PNIP__INBOUNDPACK_CONTROL_0_4                                           (0x10650010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUNDPACK_CONTROL_0_4__INBOUND_RXPORT_DIS                       (0x000F0000)
#define U_PNIP__INBOUNDPACK_CONTROL_0_4__AUTOSTOP_MAX                             (0x00007F00)
#define U_PNIP__INBOUNDPACK_CONTROL_0_4__LBUF_WDOG_MAX                            (0x0000007F)
#define U_PNIP__INBOUNDPACK_CONTROL_0_4_RESET__VALUE             0x0
#define U_PNIP__INBOUNDPACK_CONTROL_0_5                                           (0x10650014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUNDPACK_CONTROL_0_5__INBOUND_RXPORT_DIS                       (0x000F0000)
#define U_PNIP__INBOUNDPACK_CONTROL_0_5__AUTOSTOP_MAX                             (0x00007F00)
#define U_PNIP__INBOUNDPACK_CONTROL_0_5__LBUF_WDOG_MAX                            (0x0000007F)
#define U_PNIP__INBOUNDPACK_CONTROL_0_5_RESET__VALUE             0x0
#define U_PNIP__INBOUNDPACK_CONTROL_0_6                                           (0x10650018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUNDPACK_CONTROL_0_6__INBOUND_RXPORT_DIS                       (0x000F0000)
#define U_PNIP__INBOUNDPACK_CONTROL_0_6__AUTOSTOP_MAX                             (0x00007F00)
#define U_PNIP__INBOUNDPACK_CONTROL_0_6__LBUF_WDOG_MAX                            (0x0000007F)
#define U_PNIP__INBOUNDPACK_CONTROL_0_6_RESET__VALUE             0x0
#define U_PNIP__INBOUNDPACK_CONTROL_0_7                                           (0x1065001C + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUNDPACK_CONTROL_0_7__INBOUND_RXPORT_DIS                       (0x000F0000)
#define U_PNIP__INBOUNDPACK_CONTROL_0_7__AUTOSTOP_MAX                             (0x00007F00)
#define U_PNIP__INBOUNDPACK_CONTROL_0_7__LBUF_WDOG_MAX                            (0x0000007F)
#define U_PNIP__INBOUNDPACK_CONTROL_0_7_RESET__VALUE             0x0
#define U_PNIP__INBOUNDPACK_CONTROL_1_0                                           (0x10650100 + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUNDPACK_CONTROL_1_0__FRAMEDATALENGTH                          (0x07FF0000)
#define U_PNIP__INBOUNDPACK_CONTROL_1_0__PACK_KIND                                (0x00000100)
#define U_PNIP__INBOUNDPACK_CONTROL_1_0__INBOUND_POS_NR                           (0x0000003F)
#define U_PNIP__INBOUNDPACK_CONTROL_1_0_RESET__VALUE             0x0
#define U_PNIP__INBOUNDPACK_CONTROL_1_1                                           (0x10650104 + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUNDPACK_CONTROL_1_1__FRAMEDATALENGTH                          (0x07FF0000)
#define U_PNIP__INBOUNDPACK_CONTROL_1_1__PACK_KIND                                (0x00000100)
#define U_PNIP__INBOUNDPACK_CONTROL_1_1__INBOUND_POS_NR                           (0x0000003F)
#define U_PNIP__INBOUNDPACK_CONTROL_1_1_RESET__VALUE             0x0
#define U_PNIP__INBOUNDPACK_CONTROL_1_2                                           (0x10650108 + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUNDPACK_CONTROL_1_2__FRAMEDATALENGTH                          (0x07FF0000)
#define U_PNIP__INBOUNDPACK_CONTROL_1_2__PACK_KIND                                (0x00000100)
#define U_PNIP__INBOUNDPACK_CONTROL_1_2__INBOUND_POS_NR                           (0x0000003F)
#define U_PNIP__INBOUNDPACK_CONTROL_1_2_RESET__VALUE             0x0
#define U_PNIP__INBOUNDPACK_CONTROL_1_3                                           (0x1065010C + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUNDPACK_CONTROL_1_3__FRAMEDATALENGTH                          (0x07FF0000)
#define U_PNIP__INBOUNDPACK_CONTROL_1_3__PACK_KIND                                (0x00000100)
#define U_PNIP__INBOUNDPACK_CONTROL_1_3__INBOUND_POS_NR                           (0x0000003F)
#define U_PNIP__INBOUNDPACK_CONTROL_1_3_RESET__VALUE             0x0
#define U_PNIP__INBOUNDPACK_CONTROL_1_4                                           (0x10650110 + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUNDPACK_CONTROL_1_4__FRAMEDATALENGTH                          (0x07FF0000)
#define U_PNIP__INBOUNDPACK_CONTROL_1_4__PACK_KIND                                (0x00000100)
#define U_PNIP__INBOUNDPACK_CONTROL_1_4__INBOUND_POS_NR                           (0x0000003F)
#define U_PNIP__INBOUNDPACK_CONTROL_1_4_RESET__VALUE             0x0
#define U_PNIP__INBOUNDPACK_CONTROL_1_5                                           (0x10650114 + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUNDPACK_CONTROL_1_5__FRAMEDATALENGTH                          (0x07FF0000)
#define U_PNIP__INBOUNDPACK_CONTROL_1_5__PACK_KIND                                (0x00000100)
#define U_PNIP__INBOUNDPACK_CONTROL_1_5__INBOUND_POS_NR                           (0x0000003F)
#define U_PNIP__INBOUNDPACK_CONTROL_1_5_RESET__VALUE             0x0
#define U_PNIP__INBOUNDPACK_CONTROL_1_6                                           (0x10650118 + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUNDPACK_CONTROL_1_6__FRAMEDATALENGTH                          (0x07FF0000)
#define U_PNIP__INBOUNDPACK_CONTROL_1_6__PACK_KIND                                (0x00000100)
#define U_PNIP__INBOUNDPACK_CONTROL_1_6__INBOUND_POS_NR                           (0x0000003F)
#define U_PNIP__INBOUNDPACK_CONTROL_1_6_RESET__VALUE             0x0
#define U_PNIP__INBOUNDPACK_CONTROL_1_7                                           (0x1065011C + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUNDPACK_CONTROL_1_7__FRAMEDATALENGTH                          (0x07FF0000)
#define U_PNIP__INBOUNDPACK_CONTROL_1_7__PACK_KIND                                (0x00000100)
#define U_PNIP__INBOUNDPACK_CONTROL_1_7__INBOUND_POS_NR                           (0x0000003F)
#define U_PNIP__INBOUNDPACK_CONTROL_1_7_RESET__VALUE             0x0
#define U_PNIP__AUTOSTOP_BASETIME                                                 (0x10650200 + PNIP_DEV_TOP_BASE)
#define U_PNIP__AUTOSTOP_BASETIME__AUTOSTOP_BASETIME_22DT11                       (0x007FF800)
#define U_PNIP__AUTOSTOP_BASETIME__AUTOSTOP_BASETIME_10DT0                        (0x000007FF)
#define U_PNIP__AUTOSTOP_BASETIME_RESET__VALUE                   0x773594
#define U_PNIP__INBOUND_LATE_ERR_STATUS                                           (0x10650204 + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUND_LATE_ERR_STATUS__INBOUND_LATE_ERR                         (0x000000FF)
#define U_PNIP__INBOUND_LATE_ERR_STATUS_RESET__VALUE             0x0
#define U_PNIP__INBOUND_LATE_ERR_MASK                                             (0x10650208 + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUND_LATE_ERR_MASK__INBOUND_LATE_ERR_MASK                      (0x000000FF)
#define U_PNIP__INBOUND_LATE_ERR_MASK_RESET__VALUE               0x0
#define U_PNIP__PACK_ERR_STATUS                                                   (0x10650280 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PACK_ERR_STATUS__INBOUND_SEND_ERR                                 (0x00FF0000)
#define U_PNIP__PACK_ERR_STATUS__INBOUND_RECEIVE_ERR                              (0x0000FF00)
#define U_PNIP__PACK_ERR_STATUS__INBOUND_PACKGROUP_ERR                            (0x000000F0)
#define U_PNIP__PACK_ERR_STATUS__OUTBOUND_PACKGROUP_ERR                           (0x0000000F)
#define U_PNIP__PACK_ERR_STATUS_RESET__VALUE                     0x0
#define U_PNIP__PACKCOMMAND_IF_CONTROL                                            (0x10650300 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PACKCOMMAND_IF_CONTROL__COMMANDVALUE                              (0x00007F00)
#define U_PNIP__PACKCOMMAND_IF_CONTROL__USER_ID                                   (0x000000E0)
#define U_PNIP__PACKCOMMAND_IF_CONTROL__CONFREQUEST                               (0x00000010)
#define U_PNIP__PACKCOMMAND_IF_CONTROL__F_CODE_2_0                                (0x00000007)
#define U_PNIP__PACKCOMMAND_IF_CONTROL_RESET__VALUE              0x0
#define U_PNIP__PACKCOMMAND_IF_STATUS                                             (0x10650304 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PACKCOMMAND_IF_STATUS__RETURNVALUE                                (0xFFFFFF00)
#define U_PNIP__PACKCOMMAND_IF_STATUS__USER_ID_2_0                                (0x000000E0)
#define U_PNIP__PACKCOMMAND_IF_STATUS__CONFRESPONSE                               (0x00000010)
#define U_PNIP__PACKCOMMAND_IF_STATUS__CONFERROR                                  (0x00000008)
#define U_PNIP__PACKCOMMAND_IF_STATUS__F_CODE_2_0                                 (0x00000007)
#define U_PNIP__PACKCOMMAND_IF_STATUS_RESET__VALUE               0x0
#define U_PNIP__INBOUND_DELAY_MINMAX                                              (0x10650400 + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUND_DELAY_MINMAX__PACK_DEBUG_COUNTER_MAX                      (0xFFFF0000)
#define U_PNIP__INBOUND_DELAY_MINMAX__PACK_DEBUG_COUNTER_MIN                      (0x0000FFFF)
#define U_PNIP__INBOUND_DELAY_MINMAX_RESET__VALUE                0xFFFF
#define U_PNIP__INBOUND_DELAY_CONTROL                                             (0x10650404 + PNIP_DEV_TOP_BASE)
#define U_PNIP__INBOUND_DELAY_CONTROL__PACK_DEBUG_PACKGROUP                       (0x00070000)
#define U_PNIP__INBOUND_DELAY_CONTROL__PACK_DEBUG_COUNTER                         (0x0000FFFF)
#define U_PNIP__INBOUND_DELAY_CONTROL_RESET__VALUE               0x0
#define U_PNIP__TRIGGERREQQUEUE                                                   (0x10660000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_31                                  (0x80000000)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_30                                  (0x40000000)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_29                                  (0x20000000)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_28                                  (0x10000000)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_27                                  (0x08000000)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_26                                  (0x04000000)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_25                                  (0x02000000)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_24                                  (0x01000000)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_23                                  (0x00800000)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_22                                  (0x00400000)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_21                                  (0x00200000)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_20                                  (0x00100000)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_19                                  (0x00080000)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_18                                  (0x00040000)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_17                                  (0x00020000)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_16                                  (0x00010000)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_15                                  (0x00008000)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_14                                  (0x00004000)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_13                                  (0x00002000)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_12                                  (0x00001000)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_11                                  (0x00000800)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_10                                  (0x00000400)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_9                                   (0x00000200)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_8                                   (0x00000100)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_7                                   (0x00000080)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_6                                   (0x00000040)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_5                                   (0x00000020)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_4                                   (0x00000010)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_3                                   (0x00000008)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_2                                   (0x00000004)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_1                                   (0x00000002)
#define U_PNIP__TRIGGERREQQUEUE__TRIGGERQUEUE_0                                   (0x00000001)
#define U_PNIP__TRIGGERREQQUEUE_RESET__VALUE                     0x0
#define U_PNIP__CONFIRMINGQUEUE                                                   (0x10660004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_31                               (0x80000000)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_30                               (0x40000000)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_29                               (0x20000000)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_28                               (0x10000000)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_27                               (0x08000000)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_26                               (0x04000000)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_25                               (0x02000000)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_24                               (0x01000000)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_23                               (0x00800000)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_22                               (0x00400000)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_21                               (0x00200000)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_20                               (0x00100000)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_19                               (0x00080000)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_18                               (0x00040000)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_17                               (0x00020000)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_16                               (0x00010000)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_15                               (0x00008000)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_14                               (0x00004000)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_13                               (0x00002000)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_12                               (0x00001000)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_11                               (0x00000800)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_10                               (0x00000400)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_9                                (0x00000200)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_8                                (0x00000100)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_7                                (0x00000080)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_6                                (0x00000040)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_5                                (0x00000020)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_4                                (0x00000010)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_3                                (0x00000008)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_2                                (0x00000004)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_1                                (0x00000002)
#define U_PNIP__CONFIRMINGQUEUE__CONFIRMINGQUEUE_0                                (0x00000001)
#define U_PNIP__CONFIRMINGQUEUE_RESET__VALUE                     0x0
#define U_PNIP__QUEUEERRSTATUS                                                    (0x10660008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_31                                 (0x80000000)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_30                                 (0x40000000)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_29                                 (0x20000000)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_28                                 (0x10000000)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_27                                 (0x08000000)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_26                                 (0x04000000)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_25                                 (0x02000000)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_24                                 (0x01000000)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_23                                 (0x00800000)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_22                                 (0x00400000)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_21                                 (0x00200000)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_20                                 (0x00100000)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_19                                 (0x00080000)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_18                                 (0x00040000)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_17                                 (0x00020000)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_16                                 (0x00010000)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_15                                 (0x00008000)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_14                                 (0x00004000)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_13                                 (0x00002000)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_12                                 (0x00001000)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_11                                 (0x00000800)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_10                                 (0x00000400)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_9                                  (0x00000200)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_8                                  (0x00000100)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_7                                  (0x00000080)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_6                                  (0x00000040)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_5                                  (0x00000020)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_4                                  (0x00000010)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_3                                  (0x00000008)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_2                                  (0x00000004)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_1                                  (0x00000002)
#define U_PNIP__QUEUEERRSTATUS__ERRSTATUSQUEUE_0                                  (0x00000001)
#define U_PNIP__QUEUEERRSTATUS_RESET__VALUE                      0x0
#define U_PNIP__QUEUEERREVENT                                                     (0x1066000C + PNIP_DEV_TOP_BASE)
#define U_PNIP__QUEUEERREVENT__ERROREVENT                                         (0x000001FF)
#define U_PNIP__QUEUEERREVENT_RESET__VALUE                       0x0
#define U_PNIP__CURRENTQUEUE                                                      (0x10660010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CURRENTQUEUE__CURRENTQUEUE                                        (0x0000003F)
#define U_PNIP__CURRENTQUEUE_RESET__VALUE                        0x3F
#define U_PNIP__CURRENTCMD                                                        (0x10660014 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CURRENTCMD__CURRENTCMD                                            (0x0000FFFF)
#define U_PNIP__CURRENTCMD_RESET__VALUE                          0x0
#define U_PNIP__CURRENTDATA                                                       (0x10660018 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CURRENTDATA__CURRENTDATA                                          (0x0000FFFF)
#define U_PNIP__CURRENTDATA_RESET__VALUE                         0x0
#define U_PNIP__CANCELREQQUEUE                                                    (0x1066001C + PNIP_DEV_TOP_BASE)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_31                                    (0x80000000)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_30                                    (0x40000000)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_29                                    (0x20000000)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_28                                    (0x10000000)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_27                                    (0x08000000)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_26                                    (0x04000000)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_25                                    (0x02000000)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_24                                    (0x01000000)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_23                                    (0x00800000)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_22                                    (0x00400000)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_21                                    (0x00200000)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_20                                    (0x00100000)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_19                                    (0x00080000)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_18                                    (0x00040000)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_17                                    (0x00020000)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_16                                    (0x00010000)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_15                                    (0x00008000)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_14                                    (0x00004000)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_13                                    (0x00002000)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_12                                    (0x00001000)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_11                                    (0x00000800)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_10                                    (0x00000400)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_9                                     (0x00000200)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_8                                     (0x00000100)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_7                                     (0x00000080)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_6                                     (0x00000040)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_5                                     (0x00000020)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_4                                     (0x00000010)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_3                                     (0x00000008)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_2                                     (0x00000004)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_1                                     (0x00000002)
#define U_PNIP__CANCELREQQUEUE__CANCELQUEUE_0                                     (0x00000001)
#define U_PNIP__CANCELREQQUEUE_RESET__VALUE                      0x0
#define U_PNIP__KRISC_TCM_MAP                                                     (0x10670000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__KRISC_TCM_MAP__TCM_PARTITION                                      (0x0000000F)
#define U_PNIP__KRISC_TCM_MAP_RESET__VALUE                       0x0
#define U_PNIP__CONTROLSTATUS_0                                                   (0x10670100 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CONTROLSTATUS_0__STATUS                                           (0x00000020)
#define U_PNIP__CONTROLSTATUS_0__RELOAD_MODE                                      (0x00000004)
#define U_PNIP__CONTROLSTATUS_0__LOAD                                             (0x00000002)
#define U_PNIP__CONTROLSTATUS_0__RUN_XSTOP                                        (0x00000001)
#define U_PNIP__CONTROLSTATUS_0_RESET__VALUE                     0x0
#define U_PNIP__CONTROLSTATUS_1                                                   (0x10670104 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CONTROLSTATUS_1__KASKADIERUNG                                     (0x00000040)
#define U_PNIP__CONTROLSTATUS_1__STATUS                                           (0x00000020)
#define U_PNIP__CONTROLSTATUS_1__RELOAD_MODE                                      (0x00000004)
#define U_PNIP__CONTROLSTATUS_1__LOAD                                             (0x00000002)
#define U_PNIP__CONTROLSTATUS_1__RUN_XSTOP                                        (0x00000001)
#define U_PNIP__CONTROLSTATUS_1_RESET__VALUE                     0x0
#define U_PNIP__RELOAD_0                                                          (0x10670108 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RELOAD_0__RELOAD_VALUE                                            (0xFFFFFFFF)
#define U_PNIP__RELOAD_0_RESET__VALUE                            0x0
#define U_PNIP__RELOAD_1                                                          (0x1067010C + PNIP_DEV_TOP_BASE)
#define U_PNIP__RELOAD_1__RELOAD_VALUE                                            (0xFFFFFFFF)
#define U_PNIP__RELOAD_1_RESET__VALUE                            0x0
#define U_PNIP__CONTROL_PREDIV                                                    (0x10670110 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CONTROL_PREDIV__LOAD_V1                                           (0x00000008)
#define U_PNIP__CONTROL_PREDIV__RUN_XSTOP_V1                                      (0x00000004)
#define U_PNIP__CONTROL_PREDIV__LOAD_V0                                           (0x00000002)
#define U_PNIP__CONTROL_PREDIV__RUN_XSTOP_V0                                      (0x00000001)
#define U_PNIP__CONTROL_PREDIV_RESET__VALUE                      0x0
#define U_PNIP__RELOAD_PREDIV                                                     (0x10670114 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RELOAD_PREDIV__PREDIV1_VALUE                                      (0x0000FF00)
#define U_PNIP__RELOAD_PREDIV__PREDIV0_VALUE                                      (0x000000FF)
#define U_PNIP__RELOAD_PREDIV_RESET__VALUE                       0x0
#define U_PNIP__TIMER_0                                                           (0x10670118 + PNIP_DEV_TOP_BASE)
#define U_PNIP__TIMER_0__TIMER_VALUE                                              (0xFFFFFFFF)
#define U_PNIP__TIMER_0_RESET__VALUE                             0x0
#define U_PNIP__TIMER_1                                                           (0x1067011C + PNIP_DEV_TOP_BASE)
#define U_PNIP__TIMER_1__TIMER_VALUE                                              (0xFFFFFFFF)
#define U_PNIP__TIMER_1_RESET__VALUE                             0x0
#define U_PNIP__CONTROLSTATUS_2                                                   (0x10670120 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CONTROLSTATUS_2__TIMER_MODE                                       (0x00040000)
#define U_PNIP__CONTROLSTATUS_2__ONESHOT_MODE                                     (0x00020000)
#define U_PNIP__CONTROLSTATUS_2__RUN_XSTOP                                        (0x00010000)
#define U_PNIP__CONTROLSTATUS_2__RELOAD_VALUE                                     (0x0000FFFF)
#define U_PNIP__CONTROLSTATUS_2_RESET__VALUE                     0x0
#define U_PNIP__TIMER_2                                                           (0x10670124 + PNIP_DEV_TOP_BASE)
#define U_PNIP__TIMER_2__TIMER_VALUE                                              (0x0000FFFF)
#define U_PNIP__TIMER_2_RESET__VALUE                             0x0
#define U_PNIP__CONTROLSTATUS_WD                                                  (0x10670200 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CONTROLSTATUS_WD__KEYVALUE                                        (0xFFFF0000)
#define U_PNIP__CONTROLSTATUS_WD__STATUS_WD1                                      (0x00000010)
#define U_PNIP__CONTROLSTATUS_WD__STATUS_WD0                                      (0x00000008)
#define U_PNIP__CONTROLSTATUS_WD__LOAD_TRIGGER                                    (0x00000004)
#define U_PNIP__CONTROLSTATUS_WD__RUN_XSTOP_WD1                                   (0x00000002)
#define U_PNIP__CONTROLSTATUS_WD__RUN_XSTOP_WD0                                   (0x00000001)
#define U_PNIP__CONTROLSTATUS_WD_RESET__VALUE                    0x0
#define U_PNIP__RELOAD_WD0_LOW                                                    (0x10670204 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RELOAD_WD0_LOW__KEYVALUE                                          (0xFFFF0000)
#define U_PNIP__RELOAD_WD0_LOW__RELOADVALUE_WD0_LOW                               (0x0000FFFF)
#define U_PNIP__RELOAD_WD0_LOW_RESET__VALUE                      0xFFFF
#define U_PNIP__RELOAD_WD0_HIGH                                                   (0x10670208 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RELOAD_WD0_HIGH__KEYVALUE                                         (0xFFFF0000)
#define U_PNIP__RELOAD_WD0_HIGH__RELOADVALUE_WD0_HIGH                             (0x0000FFFF)
#define U_PNIP__RELOAD_WD0_HIGH_RESET__VALUE                     0xFFFF
#define U_PNIP__RELOAD_WD1_LOW                                                    (0x1067020C + PNIP_DEV_TOP_BASE)
#define U_PNIP__RELOAD_WD1_LOW__KEYVALUE                                          (0xFFFF0000)
#define U_PNIP__RELOAD_WD1_LOW__RELOADVALUE_WD1_LOW                               (0x0000FFFF)
#define U_PNIP__RELOAD_WD1_LOW_RESET__VALUE                      0xFFFF
#define U_PNIP__RELOAD_WD1_HIGH                                                   (0x10670210 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RELOAD_WD1_HIGH__KEYVALUE                                         (0xFFFF0000)
#define U_PNIP__RELOAD_WD1_HIGH__RELOADVALUE_WD1_HIGH                             (0x0000FFFF)
#define U_PNIP__RELOAD_WD1_HIGH_RESET__VALUE                     0xFFFF
#define U_PNIP__WD_0                                                              (0x10670214 + PNIP_DEV_TOP_BASE)
#define U_PNIP__WD_0__TIMER_WD0_VALUE                                             (0xFFFFFFFF)
#define U_PNIP__WD_0_RESET__VALUE                                0xFFFFFFFF
#define U_PNIP__WD_1                                                              (0x10670218 + PNIP_DEV_TOP_BASE)
#define U_PNIP__WD_1__TIMER_WD1_VALUE                                             (0xFFFFFFFF)
#define U_PNIP__WD_1_RESET__VALUE                                0xFFFFFFFF
#define U_PNIP__CONTROLSTATUS_RESET                                               (0x1067021C + PNIP_DEV_TOP_BASE)
#define U_PNIP__CONTROLSTATUS_RESET__RESET_CONTROL                                (0x00010000)
#define U_PNIP__CONTROLSTATUS_RESET__RESET_LENGTH                                 (0x00000E00)
#define U_PNIP__CONTROLSTATUS_RESET__RESET_LEVEL                                  (0x00000100)
#define U_PNIP__CONTROLSTATUS_RESET__RESET_STATUS                                 (0x00000003)
#define U_PNIP__CONTROLSTATUS_RESET_RESET__VALUE                 0x0
#define U_PNIP__MI2C_ADDR                                                         (0x10670300 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MI2C_ADDR__SLA6                                                   (0x00000080)
#define U_PNIP__MI2C_ADDR__SLA5                                                   (0x00000040)
#define U_PNIP__MI2C_ADDR__SLA4                                                   (0x00000020)
#define U_PNIP__MI2C_ADDR__SLA3                                                   (0x00000010)
#define U_PNIP__MI2C_ADDR__SLA2                                                   (0x00000008)
#define U_PNIP__MI2C_ADDR__SLA1                                                   (0x00000004)
#define U_PNIP__MI2C_ADDR__SLA0                                                   (0x00000002)
#define U_PNIP__MI2C_ADDR__GCE                                                    (0x00000001)
#define U_PNIP__MI2C_ADDR_RESET__VALUE                           0x0
#define U_PNIP__MI2C_DATA                                                         (0x10670304 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MI2C_DATA__DATA                                                   (0x000000FF)
#define U_PNIP__MI2C_DATA_RESET__VALUE                           0x0
#define U_PNIP__MI2C_CNTR                                                         (0x10670308 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MI2C_CNTR__IEN                                                    (0x00000080)
#define U_PNIP__MI2C_CNTR__ENAB                                                   (0x00000040)
#define U_PNIP__MI2C_CNTR__STA                                                    (0x00000020)
#define U_PNIP__MI2C_CNTR__STP                                                    (0x00000010)
#define U_PNIP__MI2C_CNTR__IFLG                                                   (0x00000008)
#define U_PNIP__MI2C_CNTR__AAK                                                    (0x00000004)
#define U_PNIP__MI2C_CNTR_RESET__VALUE                           0x0
#define U_PNIP__MI2C_STAT                                                         (0x1067030C + PNIP_DEV_TOP_BASE)
#define U_PNIP__MI2C_STAT__STATUS                                                 (0x000000FF)
#define U_PNIP__MI2C_STAT_RESET__VALUE                           0xF8
#define U_PNIP__MI2C_CCR                                                          (0x1067030C + PNIP_DEV_TOP_BASE)
#define U_PNIP__MI2C_CCR__M3_0                                                    (0x00000078)
#define U_PNIP__MI2C_CCR__N2_0                                                    (0x00000007)
#define U_PNIP__MI2C_CCR_RESET__VALUE                            0x0
#define U_PNIP__MI2C_XADDR                                                        (0x10670310 + PNIP_DEV_TOP_BASE)
#define U_PNIP__MI2C_XADDR__SLAX7                                                 (0x00000080)
#define U_PNIP__MI2C_XADDR__SLAX6                                                 (0x00000040)
#define U_PNIP__MI2C_XADDR__SLAX5                                                 (0x00000020)
#define U_PNIP__MI2C_XADDR__SLAX4                                                 (0x00000010)
#define U_PNIP__MI2C_XADDR__SLAX3                                                 (0x00000008)
#define U_PNIP__MI2C_XADDR__SLAX2                                                 (0x00000004)
#define U_PNIP__MI2C_XADDR__SLAX1                                                 (0x00000002)
#define U_PNIP__MI2C_XADDR__SLAX0                                                 (0x00000001)
#define U_PNIP__MI2C_XADDR_RESET__VALUE                          0x0
#define U_PNIP__MI2C_SOFTWARE_RESET                                               (0x1067031C + PNIP_DEV_TOP_BASE)
#define U_PNIP__MI2C_SOFTWARE_RESET__SOFTRESET                                    (0x000000FF)
#define U_PNIP__MI2C_SOFTWARE_RESET_RESET__VALUE                 0x0
#define U_PNIP__EX_CONTROL_1                                                      (0x10670320 + PNIP_DEV_TOP_BASE)
#define U_PNIP__EX_CONTROL_1__SCL_TOGGLE                                          (0x00000080)
#define U_PNIP__EX_CONTROL_1__ERROR                                               (0x00000010)
#define U_PNIP__EX_CONTROL_1__BUSY                                                (0x00000008)
#define U_PNIP__EX_CONTROL_1__IN                                                  (0x00000004)
#define U_PNIP__EX_CONTROL_1__OUT                                                 (0x00000002)
#define U_PNIP__EX_CONTROL_1__MODE                                                (0x00000001)
#define U_PNIP__EX_CONTROL_1_RESET__VALUE                        0x0
#define U_PNIP__EX_ADDRESS_1                                                      (0x10670324 + PNIP_DEV_TOP_BASE)
#define U_PNIP__EX_ADDRESS_1__ADDRESS                                             (0x000000FF)
#define U_PNIP__EX_ADDRESS_1_RESET__VALUE                        0x0
#define U_PNIP__EX_DATA_OUT_1                                                     (0x10670328 + PNIP_DEV_TOP_BASE)
#define U_PNIP__EX_DATA_OUT_1__OUT_DATA                                           (0x000000FF)
#define U_PNIP__EX_DATA_OUT_1_RESET__VALUE                       0x0
#define U_PNIP__EX_DATA_IN_1                                                      (0x1067032C + PNIP_DEV_TOP_BASE)
#define U_PNIP__EX_DATA_IN_1__IN_DATA                                             (0x000000FF)
#define U_PNIP__EX_DATA_IN_1_RESET__VALUE                        0x0
#define U_PNIP__EX_CONTROL_2                                                      (0x10670330 + PNIP_DEV_TOP_BASE)
#define U_PNIP__EX_CONTROL_2__ERROR                                               (0x00000010)
#define U_PNIP__EX_CONTROL_2__BUSY                                                (0x00000008)
#define U_PNIP__EX_CONTROL_2__IN                                                  (0x00000004)
#define U_PNIP__EX_CONTROL_2__OUT                                                 (0x00000002)
#define U_PNIP__EX_CONTROL_2__MODE                                                (0x00000001)
#define U_PNIP__EX_CONTROL_2_RESET__VALUE                        0x0
#define U_PNIP__EX_ADDRESS_2                                                      (0x10670334 + PNIP_DEV_TOP_BASE)
#define U_PNIP__EX_ADDRESS_2__ADDRESS                                             (0x000000FF)
#define U_PNIP__EX_ADDRESS_2_RESET__VALUE                        0x0
#define U_PNIP__EX_DATA_OUT_2                                                     (0x10670338 + PNIP_DEV_TOP_BASE)
#define U_PNIP__EX_DATA_OUT_2__OUT_DATA                                           (0x000000FF)
#define U_PNIP__EX_DATA_OUT_2_RESET__VALUE                       0x0
#define U_PNIP__EX_DATA_IN_2                                                      (0x1067033C + PNIP_DEV_TOP_BASE)
#define U_PNIP__EX_DATA_IN_2__IN_DATA                                             (0x000000FF)
#define U_PNIP__EX_DATA_IN_2_RESET__VALUE                        0x0
#define U_PNIP__EX_CONTROL_3                                                      (0x10670340 + PNIP_DEV_TOP_BASE)
#define U_PNIP__EX_CONTROL_3__ERROR                                               (0x00000010)
#define U_PNIP__EX_CONTROL_3__BUSY                                                (0x00000008)
#define U_PNIP__EX_CONTROL_3__IN                                                  (0x00000004)
#define U_PNIP__EX_CONTROL_3__OUT                                                 (0x00000002)
#define U_PNIP__EX_CONTROL_3__MODE                                                (0x00000001)
#define U_PNIP__EX_CONTROL_3_RESET__VALUE                        0x0
#define U_PNIP__EX_ADDRESS_3                                                      (0x10670344 + PNIP_DEV_TOP_BASE)
#define U_PNIP__EX_ADDRESS_3__ADDRESS                                             (0x000000FF)
#define U_PNIP__EX_ADDRESS_3_RESET__VALUE                        0x0
#define U_PNIP__EX_DATA_OUT_3                                                     (0x10670348 + PNIP_DEV_TOP_BASE)
#define U_PNIP__EX_DATA_OUT_3__OUT_DATA                                           (0x000000FF)
#define U_PNIP__EX_DATA_OUT_3_RESET__VALUE                       0x0
#define U_PNIP__EX_DATA_IN_3                                                      (0x1067034C + PNIP_DEV_TOP_BASE)
#define U_PNIP__EX_DATA_IN_3__IN_DATA                                             (0x000000FF)
#define U_PNIP__EX_DATA_IN_3_RESET__VALUE                        0x0
#define U_PNIP__EX_CONTROL_4                                                      (0x10670350 + PNIP_DEV_TOP_BASE)
#define U_PNIP__EX_CONTROL_4__ERROR                                               (0x00000010)
#define U_PNIP__EX_CONTROL_4__BUSY                                                (0x00000008)
#define U_PNIP__EX_CONTROL_4__IN                                                  (0x00000004)
#define U_PNIP__EX_CONTROL_4__OUT                                                 (0x00000002)
#define U_PNIP__EX_CONTROL_4__MODE                                                (0x00000001)
#define U_PNIP__EX_CONTROL_4_RESET__VALUE                        0x0
#define U_PNIP__EX_ADDRESS_4                                                      (0x10670354 + PNIP_DEV_TOP_BASE)
#define U_PNIP__EX_ADDRESS_4__ADDRESS                                             (0x000000FF)
#define U_PNIP__EX_ADDRESS_4_RESET__VALUE                        0x0
#define U_PNIP__EX_DATA_OUT_4                                                     (0x10670358 + PNIP_DEV_TOP_BASE)
#define U_PNIP__EX_DATA_OUT_4__OUT_DATA                                           (0x000000FF)
#define U_PNIP__EX_DATA_OUT_4_RESET__VALUE                       0x0
#define U_PNIP__EX_DATA_IN_4                                                      (0x1067035C + PNIP_DEV_TOP_BASE)
#define U_PNIP__EX_DATA_IN_4__IN_DATA                                             (0x000000FF)
#define U_PNIP__EX_DATA_IN_4_RESET__VALUE                        0x0
#define U_PNIP__ERROR_SLAVE_ADDRESS                                               (0x10670360 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ERROR_SLAVE_ADDRESS__ERROR_SLAVE_ADDRESS                          (0x000000FE)
#define U_PNIP__ERROR_SLAVE_ADDRESS__ERROR_ENABLE                                 (0x00000001)
#define U_PNIP__ERROR_SLAVE_ADDRESS_RESET__VALUE                 0xFA
#define U_PNIP__I2C_ENABLE                                                        (0x10670364 + PNIP_DEV_TOP_BASE)
#define U_PNIP__I2C_ENABLE__I2C_PARAMENABLE                                       (0x00000001)
#define U_PNIP__I2C_ENABLE_RESET__VALUE                          0x0
#define U_PNIP__I2C_CONTROL                                                       (0x10670368 + PNIP_DEV_TOP_BASE)
#define U_PNIP__I2C_CONTROL__SDA_I                                                (0x00000008)
#define U_PNIP__I2C_CONTROL__SCL_I                                                (0x00000004)
#define U_PNIP__I2C_CONTROL__SW_SDA_O                                             (0x00000002)
#define U_PNIP__I2C_CONTROL__SW_SCL_O                                             (0x00000001)
#define U_PNIP__I2C_CONTROL_RESET__VALUE                         0xF
#define U_PNIP__I2C_SELECT                                                        (0x1067036C + PNIP_DEV_TOP_BASE)
#define U_PNIP__I2C_SELECT__I2C_SELECTOR                                          (0x00000003)
#define U_PNIP__I2C_SELECT_RESET__VALUE                          0x0
#define U_PNIP__I2C_CLOCKCONTROL                                                  (0x10670370 + PNIP_DEV_TOP_BASE)
#define U_PNIP__I2C_CLOCKCONTROL__CLOCKDIVIDER                                    (0x000000FF)
#define U_PNIP__I2C_CLOCKCONTROL_RESET__VALUE                    0x7C
#define U_PNIP__ACCESS_ERROR                                                      (0x10674000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ACCESS_ERROR__ERROR_LOCK                                          (0x80000000)
#define U_PNIP__ACCESS_ERROR__RD_WR                                               (0x40000000)
#define U_PNIP__ACCESS_ERROR__AHB_SIZE                                            (0x30000000)
#define U_PNIP__ACCESS_ERROR__AHB_CMD_CONTROL_SELECT                              (0x08000000)
#define U_PNIP__ACCESS_ERROR__ADDRESS                                             (0x001FFFFF)
#define U_PNIP__ACCESS_ERROR_RESET__VALUE                        0x0
#define U_PNIP__APIFILTERCOMMAND_IF_CONTROL                                       (0x10679000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APIFILTERCOMMAND_IF_CONTROL__PARAMETER                            (0xFFFFFF00)
#define U_PNIP__APIFILTERCOMMAND_IF_CONTROL__USER_ID                              (0x000000E0)
#define U_PNIP__APIFILTERCOMMAND_IF_CONTROL__CONFREQUEST                          (0x00000010)
#define U_PNIP__APIFILTERCOMMAND_IF_CONTROL__IF_SELECT                            (0x00000008)
#define U_PNIP__APIFILTERCOMMAND_IF_CONTROL__F_CODE                               (0x00000007)
#define U_PNIP__APIFILTERCOMMAND_IF_CONTROL_RESET__VALUE         0x0
#define U_PNIP__APIFILTERCOMMAND_IF_STATUS                                        (0x10679004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APIFILTERCOMMAND_IF_STATUS__RETURNVALUE                           (0xFFFFFF00)
#define U_PNIP__APIFILTERCOMMAND_IF_STATUS__USER_ID                               (0x000000E0)
#define U_PNIP__APIFILTERCOMMAND_IF_STATUS__CONFRESPONSE                          (0x00000010)
#define U_PNIP__APIFILTERCOMMAND_IF_STATUS__CONFERROR                             (0x00000008)
#define U_PNIP__APIFILTERCOMMAND_IF_STATUS__F_CODE                                (0x00000007)
#define U_PNIP__APIFILTERCOMMAND_IF_STATUS_RESET__VALUE          0x0
#define U_PNIP__FILTERCODEBASE_IFA_00                                             (0x10679400 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFA_00__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFA_00_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFA_01                                             (0x10679404 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFA_01__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFA_01_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFA_02                                             (0x10679408 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFA_02__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFA_02_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFA_03                                             (0x1067940C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFA_03__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFA_03_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFA_04                                             (0x10679410 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFA_04__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFA_04_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFA_05                                             (0x10679414 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFA_05__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFA_05_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFA_06                                             (0x10679418 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFA_06__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFA_06_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFA_07                                             (0x1067941C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFA_07__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFA_07_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFA_08                                             (0x10679420 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFA_08__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFA_08_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFA_09                                             (0x10679424 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFA_09__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFA_09_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFA_10                                             (0x10679428 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFA_10__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFA_10_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFA_11                                             (0x1067942C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFA_11__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFA_11_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFA_12                                             (0x10679430 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFA_12__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFA_12_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFA_13                                             (0x10679434 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFA_13__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFA_13_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFA_14                                             (0x10679438 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFA_14__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFA_14_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFA_15                                             (0x1067943C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFA_15__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFA_15_RESET__VALUE               0x80000
#define U_PNIP__DROPCOUNT_IFA_00                                                  (0x10679440 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFA_00__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFA_00_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFA_01                                                  (0x10679444 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFA_01__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFA_01_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFA_02                                                  (0x10679448 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFA_02__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFA_02_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFA_03                                                  (0x1067944C + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFA_03__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFA_03_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFA_04                                                  (0x10679450 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFA_04__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFA_04_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFA_05                                                  (0x10679454 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFA_05__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFA_05_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFA_06                                                  (0x10679458 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFA_06__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFA_06_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFA_07                                                  (0x1067945C + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFA_07__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFA_07_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFA_08                                                  (0x10679460 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFA_08__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFA_08_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFA_09                                                  (0x10679464 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFA_09__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFA_09_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFA_10                                                  (0x10679468 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFA_10__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFA_10_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFA_11                                                  (0x1067946C + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFA_11__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFA_11_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFA_12                                                  (0x10679470 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFA_12__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFA_12_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFA_13                                                  (0x10679474 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFA_13__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFA_13_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFA_14                                                  (0x10679478 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFA_14__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFA_14_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFA_15                                                  (0x1067947C + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFA_15__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFA_15_RESET__VALUE                    0x0
#define U_PNIP__FILTERENABLED_IFA                                                 (0x10679480 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERENABLED_IFA__FILTERSTATUS_15                                (0x00008000)
#define U_PNIP__FILTERENABLED_IFA__FILTERSTATUS_14                                (0x00004000)
#define U_PNIP__FILTERENABLED_IFA__FILTERSTATUS_13                                (0x00002000)
#define U_PNIP__FILTERENABLED_IFA__FILTERSTATUS_12                                (0x00001000)
#define U_PNIP__FILTERENABLED_IFA__FILTERSTATUS_11                                (0x00000800)
#define U_PNIP__FILTERENABLED_IFA__FILTERSTATUS_10                                (0x00000400)
#define U_PNIP__FILTERENABLED_IFA__FILTERSTATUS_9                                 (0x00000200)
#define U_PNIP__FILTERENABLED_IFA__FILTERSTATUS_8                                 (0x00000100)
#define U_PNIP__FILTERENABLED_IFA__FILTERSTATUS_7                                 (0x00000080)
#define U_PNIP__FILTERENABLED_IFA__FILTERSTATUS_6                                 (0x00000040)
#define U_PNIP__FILTERENABLED_IFA__FILTERSTATUS_5                                 (0x00000020)
#define U_PNIP__FILTERENABLED_IFA__FILTERSTATUS_4                                 (0x00000010)
#define U_PNIP__FILTERENABLED_IFA__FILTERSTATUS_3                                 (0x00000008)
#define U_PNIP__FILTERENABLED_IFA__FILTERSTATUS_2                                 (0x00000004)
#define U_PNIP__FILTERENABLED_IFA__FILTERSTATUS_1                                 (0x00000002)
#define U_PNIP__FILTERENABLED_IFA__FILTERSTATUS_0                                 (0x00000001)
#define U_PNIP__FILTERENABLED_IFA_RESET__VALUE                   0x0
#define U_PNIP__FILTERERRSTATUS_IFA                                               (0x10679484 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERERRSTATUS_IFA__FILTERERRSTATUS_15                           (0x00008000)
#define U_PNIP__FILTERERRSTATUS_IFA__FILTERERRSTATUS_14                           (0x00004000)
#define U_PNIP__FILTERERRSTATUS_IFA__FILTERERRSTATUS_13                           (0x00002000)
#define U_PNIP__FILTERERRSTATUS_IFA__FILTERERRSTATUS_12                           (0x00001000)
#define U_PNIP__FILTERERRSTATUS_IFA__FILTERERRSTATUS_11                           (0x00000800)
#define U_PNIP__FILTERERRSTATUS_IFA__FILTERERRSTATUS_10                           (0x00000400)
#define U_PNIP__FILTERERRSTATUS_IFA__FILTERERRSTATUS_9                            (0x00000200)
#define U_PNIP__FILTERERRSTATUS_IFA__FILTERERRSTATUS_8                            (0x00000100)
#define U_PNIP__FILTERERRSTATUS_IFA__FILTERERRSTATUS_7                            (0x00000080)
#define U_PNIP__FILTERERRSTATUS_IFA__FILTERERRSTATUS_6                            (0x00000040)
#define U_PNIP__FILTERERRSTATUS_IFA__FILTERERRSTATUS_5                            (0x00000020)
#define U_PNIP__FILTERERRSTATUS_IFA__FILTERERRSTATUS_4                            (0x00000010)
#define U_PNIP__FILTERERRSTATUS_IFA__FILTERERRSTATUS_3                            (0x00000008)
#define U_PNIP__FILTERERRSTATUS_IFA__FILTERERRSTATUS_2                            (0x00000004)
#define U_PNIP__FILTERERRSTATUS_IFA__FILTERERRSTATUS_1                            (0x00000002)
#define U_PNIP__FILTERERRSTATUS_IFA__FILTERERRSTATUS_0                            (0x00000001)
#define U_PNIP__FILTERERRSTATUS_IFA_RESET__VALUE                 0x0
#define U_PNIP__FILTERERREVENT_IFA                                                (0x10679488 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERERREVENT_IFA__ERREVENT                                      (0x00000007)
#define U_PNIP__FILTERERREVENT_IFA_RESET__VALUE                  0x0
#define U_PNIP__LOGCONTROL_IFA                                                    (0x1067948C + PNIP_DEV_TOP_BASE)
#define U_PNIP__LOGCONTROL_IFA__LOGCONTROL_15                                     (0x00008000)
#define U_PNIP__LOGCONTROL_IFA__LOGCONTROL_14                                     (0x00004000)
#define U_PNIP__LOGCONTROL_IFA__LOGCONTROL_13                                     (0x00002000)
#define U_PNIP__LOGCONTROL_IFA__LOGCONTROL_12                                     (0x00001000)
#define U_PNIP__LOGCONTROL_IFA__LOGCONTROL_11                                     (0x00000800)
#define U_PNIP__LOGCONTROL_IFA__LOGCONTROL_10                                     (0x00000400)
#define U_PNIP__LOGCONTROL_IFA__LOGCONTROL_9                                      (0x00000200)
#define U_PNIP__LOGCONTROL_IFA__LOGCONTROL_8                                      (0x00000100)
#define U_PNIP__LOGCONTROL_IFA__LOGCONTROL_7                                      (0x00000080)
#define U_PNIP__LOGCONTROL_IFA__LOGCONTROL_6                                      (0x00000040)
#define U_PNIP__LOGCONTROL_IFA__LOGCONTROL_5                                      (0x00000020)
#define U_PNIP__LOGCONTROL_IFA__LOGCONTROL_4                                      (0x00000010)
#define U_PNIP__LOGCONTROL_IFA__LOGCONTROL_3                                      (0x00000008)
#define U_PNIP__LOGCONTROL_IFA__LOGCONTROL_2                                      (0x00000004)
#define U_PNIP__LOGCONTROL_IFA__LOGCONTROL_1                                      (0x00000002)
#define U_PNIP__LOGCONTROL_IFA__LOGCONTROL_0                                      (0x00000001)
#define U_PNIP__LOGCONTROL_IFA_RESET__VALUE                      0x0
#define U_PNIP__SDTABLEBASE_IFA                                                   (0x10679490 + PNIP_DEV_TOP_BASE)
#define U_PNIP__SDTABLEBASE_IFA__SDTABLEBASE                                      (0x001FFFFF)
#define U_PNIP__SDTABLEBASE_IFA_RESET__VALUE                     0x80000
#define U_PNIP__STORETABLEBASE_IFA                                                (0x10679494 + PNIP_DEV_TOP_BASE)
#define U_PNIP__STORETABLEBASE_IFA__STORETABLEBASE                                (0x001FFFFF)
#define U_PNIP__STORETABLEBASE_IFA_RESET__VALUE                  0x80000
#define U_PNIP__FILTERCODEBASE_IFB_00                                             (0x10679800 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFB_00__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFB_00_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFB_01                                             (0x10679804 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFB_01__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFB_01_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFB_02                                             (0x10679808 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFB_02__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFB_02_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFB_03                                             (0x1067980C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFB_03__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFB_03_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFB_04                                             (0x10679810 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFB_04__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFB_04_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFB_05                                             (0x10679814 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFB_05__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFB_05_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFB_06                                             (0x10679818 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFB_06__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFB_06_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFB_07                                             (0x1067981C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFB_07__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFB_07_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFB_08                                             (0x10679820 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFB_08__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFB_08_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFB_09                                             (0x10679824 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFB_09__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFB_09_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFB_10                                             (0x10679828 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFB_10__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFB_10_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFB_11                                             (0x1067982C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFB_11__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFB_11_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFB_12                                             (0x10679830 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFB_12__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFB_12_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFB_13                                             (0x10679834 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFB_13__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFB_13_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFB_14                                             (0x10679838 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFB_14__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFB_14_RESET__VALUE               0x80000
#define U_PNIP__FILTERCODEBASE_IFB_15                                             (0x1067983C + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERCODEBASE_IFB_15__FILTERCODEBASE                             (0x001FFFFF)
#define U_PNIP__FILTERCODEBASE_IFB_15_RESET__VALUE               0x80000
#define U_PNIP__DROPCOUNT_IFB_00                                                  (0x10679840 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFB_00__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFB_00_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFB_01                                                  (0x10679844 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFB_01__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFB_01_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFB_02                                                  (0x10679848 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFB_02__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFB_02_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFB_03                                                  (0x1067984C + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFB_03__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFB_03_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFB_04                                                  (0x10679850 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFB_04__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFB_04_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFB_05                                                  (0x10679854 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFB_05__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFB_05_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFB_06                                                  (0x10679858 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFB_06__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFB_06_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFB_07                                                  (0x1067985C + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFB_07__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFB_07_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFB_08                                                  (0x10679860 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFB_08__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFB_08_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFB_09                                                  (0x10679864 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFB_09__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFB_09_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFB_10                                                  (0x10679868 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFB_10__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFB_10_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFB_11                                                  (0x1067986C + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFB_11__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFB_11_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFB_12                                                  (0x10679870 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFB_12__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFB_12_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFB_13                                                  (0x10679874 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFB_13__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFB_13_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFB_14                                                  (0x10679878 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFB_14__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFB_14_RESET__VALUE                    0x0
#define U_PNIP__DROPCOUNT_IFB_15                                                  (0x1067987C + PNIP_DEV_TOP_BASE)
#define U_PNIP__DROPCOUNT_IFB_15__DROPCOUNT                                       (0x0000FFFF)
#define U_PNIP__DROPCOUNT_IFB_15_RESET__VALUE                    0x0
#define U_PNIP__FILTERENABLED_IFB                                                 (0x10679880 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERENABLED_IFB__FILTERSTATUS_15                                (0x00008000)
#define U_PNIP__FILTERENABLED_IFB__FILTERSTATUS_14                                (0x00004000)
#define U_PNIP__FILTERENABLED_IFB__FILTERSTATUS_13                                (0x00002000)
#define U_PNIP__FILTERENABLED_IFB__FILTERSTATUS_12                                (0x00001000)
#define U_PNIP__FILTERENABLED_IFB__FILTERSTATUS_11                                (0x00000800)
#define U_PNIP__FILTERENABLED_IFB__FILTERSTATUS_10                                (0x00000400)
#define U_PNIP__FILTERENABLED_IFB__FILTERSTATUS_9                                 (0x00000200)
#define U_PNIP__FILTERENABLED_IFB__FILTERSTATUS_8                                 (0x00000100)
#define U_PNIP__FILTERENABLED_IFB__FILTERSTATUS_7                                 (0x00000080)
#define U_PNIP__FILTERENABLED_IFB__FILTERSTATUS_6                                 (0x00000040)
#define U_PNIP__FILTERENABLED_IFB__FILTERSTATUS_5                                 (0x00000020)
#define U_PNIP__FILTERENABLED_IFB__FILTERSTATUS_4                                 (0x00000010)
#define U_PNIP__FILTERENABLED_IFB__FILTERSTATUS_3                                 (0x00000008)
#define U_PNIP__FILTERENABLED_IFB__FILTERSTATUS_2                                 (0x00000004)
#define U_PNIP__FILTERENABLED_IFB__FILTERSTATUS_1                                 (0x00000002)
#define U_PNIP__FILTERENABLED_IFB__FILTERSTATUS_0                                 (0x00000001)
#define U_PNIP__FILTERENABLED_IFB_RESET__VALUE                   0x0
#define U_PNIP__FILTERERRSTATUS_IFB                                               (0x10679884 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERERRSTATUS_IFB__FILTERERRSTATUS_15                           (0x00008000)
#define U_PNIP__FILTERERRSTATUS_IFB__FILTERERRSTATUS_14                           (0x00004000)
#define U_PNIP__FILTERERRSTATUS_IFB__FILTERERRSTATUS_13                           (0x00002000)
#define U_PNIP__FILTERERRSTATUS_IFB__FILTERERRSTATUS_12                           (0x00001000)
#define U_PNIP__FILTERERRSTATUS_IFB__FILTERERRSTATUS_11                           (0x00000800)
#define U_PNIP__FILTERERRSTATUS_IFB__FILTERERRSTATUS_10                           (0x00000400)
#define U_PNIP__FILTERERRSTATUS_IFB__FILTERERRSTATUS_9                            (0x00000200)
#define U_PNIP__FILTERERRSTATUS_IFB__FILTERERRSTATUS_8                            (0x00000100)
#define U_PNIP__FILTERERRSTATUS_IFB__FILTERERRSTATUS_7                            (0x00000080)
#define U_PNIP__FILTERERRSTATUS_IFB__FILTERERRSTATUS_6                            (0x00000040)
#define U_PNIP__FILTERERRSTATUS_IFB__FILTERERRSTATUS_5                            (0x00000020)
#define U_PNIP__FILTERERRSTATUS_IFB__FILTERERRSTATUS_4                            (0x00000010)
#define U_PNIP__FILTERERRSTATUS_IFB__FILTERERRSTATUS_3                            (0x00000008)
#define U_PNIP__FILTERERRSTATUS_IFB__FILTERERRSTATUS_2                            (0x00000004)
#define U_PNIP__FILTERERRSTATUS_IFB__FILTERERRSTATUS_1                            (0x00000002)
#define U_PNIP__FILTERERRSTATUS_IFB__FILTERERRSTATUS_0                            (0x00000001)
#define U_PNIP__FILTERERRSTATUS_IFB_RESET__VALUE                 0x0
#define U_PNIP__FILTERERREVENT_IFB                                                (0x10679888 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FILTERERREVENT_IFB__ERREVENT                                      (0x00000007)
#define U_PNIP__FILTERERREVENT_IFB_RESET__VALUE                  0x0
#define U_PNIP__LOGCONTROL_IFB                                                    (0x1067988C + PNIP_DEV_TOP_BASE)
#define U_PNIP__LOGCONTROL_IFB__LOGCONTROL_15                                     (0x00008000)
#define U_PNIP__LOGCONTROL_IFB__LOGCONTROL_14                                     (0x00004000)
#define U_PNIP__LOGCONTROL_IFB__LOGCONTROL_13                                     (0x00002000)
#define U_PNIP__LOGCONTROL_IFB__LOGCONTROL_12                                     (0x00001000)
#define U_PNIP__LOGCONTROL_IFB__LOGCONTROL_11                                     (0x00000800)
#define U_PNIP__LOGCONTROL_IFB__LOGCONTROL_10                                     (0x00000400)
#define U_PNIP__LOGCONTROL_IFB__LOGCONTROL_9                                      (0x00000200)
#define U_PNIP__LOGCONTROL_IFB__LOGCONTROL_8                                      (0x00000100)
#define U_PNIP__LOGCONTROL_IFB__LOGCONTROL_7                                      (0x00000080)
#define U_PNIP__LOGCONTROL_IFB__LOGCONTROL_6                                      (0x00000040)
#define U_PNIP__LOGCONTROL_IFB__LOGCONTROL_5                                      (0x00000020)
#define U_PNIP__LOGCONTROL_IFB__LOGCONTROL_4                                      (0x00000010)
#define U_PNIP__LOGCONTROL_IFB__LOGCONTROL_3                                      (0x00000008)
#define U_PNIP__LOGCONTROL_IFB__LOGCONTROL_2                                      (0x00000004)
#define U_PNIP__LOGCONTROL_IFB__LOGCONTROL_1                                      (0x00000002)
#define U_PNIP__LOGCONTROL_IFB__LOGCONTROL_0                                      (0x00000001)
#define U_PNIP__LOGCONTROL_IFB_RESET__VALUE                      0x0
#define U_PNIP__SDTABLEBASE_IFB                                                   (0x10679890 + PNIP_DEV_TOP_BASE)
#define U_PNIP__SDTABLEBASE_IFB__SDTABLEBASE                                      (0x001FFFFF)
#define U_PNIP__SDTABLEBASE_IFB_RESET__VALUE                     0x80000
#define U_PNIP__STORETABLEBASE_IFB                                                (0x10679894 + PNIP_DEV_TOP_BASE)
#define U_PNIP__STORETABLEBASE_IFB__STORETABLEBASE                                (0x001FFFFF)
#define U_PNIP__STORETABLEBASE_IFB_RESET__VALUE                  0x80000
#define U_PNIP__ASNDCOMMAND_IF_CONTROL                                            (0x1067A000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ASNDCOMMAND_IF_CONTROL__PARAMETER                                 (0x00000F00)
#define U_PNIP__ASNDCOMMAND_IF_CONTROL__USER_ID                                   (0x000000E0)
#define U_PNIP__ASNDCOMMAND_IF_CONTROL__CONFREQUEST                               (0x00000010)
#define U_PNIP__ASNDCOMMAND_IF_CONTROL__IF_SELECT                                 (0x00000008)
#define U_PNIP__ASNDCOMMAND_IF_CONTROL__F_CODE                                    (0x00000007)
#define U_PNIP__ASNDCOMMAND_IF_CONTROL_RESET__VALUE              0x0
#define U_PNIP__ASNDCOMMAND_IF_STATUS                                             (0x1067A004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ASNDCOMMAND_IF_STATUS__RETURNVALUE                                (0xFFFFFF00)
#define U_PNIP__ASNDCOMMAND_IF_STATUS__USER_ID                                    (0x000000E0)
#define U_PNIP__ASNDCOMMAND_IF_STATUS__CONFRESPONSE                               (0x00000010)
#define U_PNIP__ASNDCOMMAND_IF_STATUS__CONFERROR                                  (0x00000008)
#define U_PNIP__ASNDCOMMAND_IF_STATUS__F_CODE                                     (0x00000007)
#define U_PNIP__ASNDCOMMAND_IF_STATUS_RESET__VALUE               0x0
#define U_PNIP__SNDTABLEBASE_IFA                                                  (0x1067A400 + PNIP_DEV_TOP_BASE)
#define U_PNIP__SNDTABLEBASE_IFA__SNDTABLEBASE                                    (0x001FFFFF)
#define U_PNIP__SNDTABLEBASE_IFA_RESET__VALUE                    0xFFFF8
#define U_PNIP__SNDTRIGGER_IFA                                                    (0x1067A404 + PNIP_DEV_TOP_BASE)
#define U_PNIP__SNDTRIGGER_IFA__SNDTRIGER_SD_15                                   (0x00008000)
#define U_PNIP__SNDTRIGGER_IFA__SNDTRIGER_SD_14                                   (0x00004000)
#define U_PNIP__SNDTRIGGER_IFA__SNDTRIGER_SD_13                                   (0x00002000)
#define U_PNIP__SNDTRIGGER_IFA__SNDTRIGER_SD_12                                   (0x00001000)
#define U_PNIP__SNDTRIGGER_IFA__SNDTRIGER_SD_11                                   (0x00000800)
#define U_PNIP__SNDTRIGGER_IFA__SNDTRIGER_SD_10                                   (0x00000400)
#define U_PNIP__SNDTRIGGER_IFA__SNDTRIGER_SD_9                                    (0x00000200)
#define U_PNIP__SNDTRIGGER_IFA__SNDTRIGER_SD_8                                    (0x00000100)
#define U_PNIP__SNDTRIGGER_IFA__SNDTRIGER_SD_7                                    (0x00000080)
#define U_PNIP__SNDTRIGGER_IFA__SNDTRIGER_SD_6                                    (0x00000040)
#define U_PNIP__SNDTRIGGER_IFA__SNDTRIGER_SD_5                                    (0x00000020)
#define U_PNIP__SNDTRIGGER_IFA__SNDTRIGER_SD_4                                    (0x00000010)
#define U_PNIP__SNDTRIGGER_IFA__SNDTRIGER_SD_3                                    (0x00000008)
#define U_PNIP__SNDTRIGGER_IFA__SNDTRIGER_SD_2                                    (0x00000004)
#define U_PNIP__SNDTRIGGER_IFA__SNDTRIGER_SD_1                                    (0x00000002)
#define U_PNIP__SNDTRIGGER_IFA__SNDTRIGER_SD_0                                    (0x00000001)
#define U_PNIP__SNDTRIGGER_IFA_RESET__VALUE                      0x0
#define U_PNIP__SNDWATCHDOGLIMIT_IFA                                              (0x1067A408 + PNIP_DEV_TOP_BASE)
#define U_PNIP__SNDWATCHDOGLIMIT_IFA__SDWATCHDOGLIMIT                             (0x0000FFFF)
#define U_PNIP__SNDWATCHDOGLIMIT_IFA_RESET__VALUE                0x0
#define U_PNIP__SNDEVENT_IFA                                                      (0x1067A40C + PNIP_DEV_TOP_BASE)
#define U_PNIP__SNDEVENT_IFA__SNDEVENT                                            (0x000003FF)
#define U_PNIP__SNDEVENT_IFA_RESET__VALUE                        0x0
#define U_PNIP__SNDSTATUS_IFA                                                     (0x1067A410 + PNIP_DEV_TOP_BASE)
#define U_PNIP__SNDSTATUS_IFA__SNDSTATUS_SD_15                                    (0x00008000)
#define U_PNIP__SNDSTATUS_IFA__SNDSTATUS_SD_14                                    (0x00004000)
#define U_PNIP__SNDSTATUS_IFA__SNDSTATUS_SD_13                                    (0x00002000)
#define U_PNIP__SNDSTATUS_IFA__SNDSTATUS_SD_12                                    (0x00001000)
#define U_PNIP__SNDSTATUS_IFA__SNDSTATUS_SD_11                                    (0x00000800)
#define U_PNIP__SNDSTATUS_IFA__SNDSTATUS_SD_10                                    (0x00000400)
#define U_PNIP__SNDSTATUS_IFA__SNDSTATUS_SD_9                                     (0x00000200)
#define U_PNIP__SNDSTATUS_IFA__SNDSTATUS_SD_8                                     (0x00000100)
#define U_PNIP__SNDSTATUS_IFA__SNDSTATUS_SD_7                                     (0x00000080)
#define U_PNIP__SNDSTATUS_IFA__SNDSTATUS_SD_6                                     (0x00000040)
#define U_PNIP__SNDSTATUS_IFA__SNDSTATUS_SD_5                                     (0x00000020)
#define U_PNIP__SNDSTATUS_IFA__SNDSTATUS_SD_4                                     (0x00000010)
#define U_PNIP__SNDSTATUS_IFA__SNDSTATUS_SD_3                                     (0x00000008)
#define U_PNIP__SNDSTATUS_IFA__SNDSTATUS_SD_2                                     (0x00000004)
#define U_PNIP__SNDSTATUS_IFA__SNDSTATUS_SD_1                                     (0x00000002)
#define U_PNIP__SNDSTATUS_IFA__SNDSTATUS_SD_0                                     (0x00000001)
#define U_PNIP__SNDSTATUS_IFA_RESET__VALUE                       0x0
#define U_PNIP__SNDERREVENT_IFA                                                   (0x1067A414 + PNIP_DEV_TOP_BASE)
#define U_PNIP__SNDERREVENT_IFA__ERROREVENT                                       (0x0000000F)
#define U_PNIP__SNDERREVENT_IFA_RESET__VALUE                     0x0
#define U_PNIP__SNDERRSTATUS_IFA                                                  (0x1067A418 + PNIP_DEV_TOP_BASE)
#define U_PNIP__SNDERRSTATUS_IFA__SNDERRSTATUS_SD_15                              (0x00008000)
#define U_PNIP__SNDERRSTATUS_IFA__SNDERRSTATUS_SD_14                              (0x00004000)
#define U_PNIP__SNDERRSTATUS_IFA__SNDERRSTATUS_SD_13                              (0x00002000)
#define U_PNIP__SNDERRSTATUS_IFA__SNDERRSTATUS_SD_12                              (0x00001000)
#define U_PNIP__SNDERRSTATUS_IFA__SNDERRSTATUS_SD_11                              (0x00000800)
#define U_PNIP__SNDERRSTATUS_IFA__SNDERRSTATUS_SD_10                              (0x00000400)
#define U_PNIP__SNDERRSTATUS_IFA__SNDERRSTATUS_SD_9                               (0x00000200)
#define U_PNIP__SNDERRSTATUS_IFA__SNDERRSTATUS_SD_8                               (0x00000100)
#define U_PNIP__SNDERRSTATUS_IFA__SNDERRSTATUS_SD_7                               (0x00000080)
#define U_PNIP__SNDERRSTATUS_IFA__SNDERRSTATUS_SD_6                               (0x00000040)
#define U_PNIP__SNDERRSTATUS_IFA__SNDERRSTATUS_SD_5                               (0x00000020)
#define U_PNIP__SNDERRSTATUS_IFA__SNDERRSTATUS_SD_4                               (0x00000010)
#define U_PNIP__SNDERRSTATUS_IFA__SNDERRSTATUS_SD_3                               (0x00000008)
#define U_PNIP__SNDERRSTATUS_IFA__SNDERRSTATUS_SD_2                               (0x00000004)
#define U_PNIP__SNDERRSTATUS_IFA__SNDERRSTATUS_SD_1                               (0x00000002)
#define U_PNIP__SNDERRSTATUS_IFA__SNDERRSTATUS_SD_0                               (0x00000001)
#define U_PNIP__SNDERRSTATUS_IFA_RESET__VALUE                    0x0
#define U_PNIP__SNDREADBACK_IFA                                                   (0x1067A41C + PNIP_DEV_TOP_BASE)
#define U_PNIP__SNDREADBACK_IFA__RCVREADBACK_15                                   (0x00008000)
#define U_PNIP__SNDREADBACK_IFA__RCVREADBACK_14                                   (0x00004000)
#define U_PNIP__SNDREADBACK_IFA__RCVREADBACK_13                                   (0x00002000)
#define U_PNIP__SNDREADBACK_IFA__RCVREADBACK_12                                   (0x00001000)
#define U_PNIP__SNDREADBACK_IFA__RCVREADBACK_11                                   (0x00000800)
#define U_PNIP__SNDREADBACK_IFA__RCVREADBACK_10                                   (0x00000400)
#define U_PNIP__SNDREADBACK_IFA__RCVREADBACK_9                                    (0x00000200)
#define U_PNIP__SNDREADBACK_IFA__RCVREADBACK_8                                    (0x00000100)
#define U_PNIP__SNDREADBACK_IFA__RCVREADBACK_7                                    (0x00000080)
#define U_PNIP__SNDREADBACK_IFA__RCVREADBACK_6                                    (0x00000040)
#define U_PNIP__SNDREADBACK_IFA__RCVREADBACK_5                                    (0x00000020)
#define U_PNIP__SNDREADBACK_IFA__RCVREADBACK_4                                    (0x00000010)
#define U_PNIP__SNDREADBACK_IFA__RCVREADBACK_3                                    (0x00000008)
#define U_PNIP__SNDREADBACK_IFA__RCVREADBACK_2                                    (0x00000004)
#define U_PNIP__SNDREADBACK_IFA__RCVREADBACK_1                                    (0x00000002)
#define U_PNIP__SNDREADBACK_IFA__RCVREADBACK_0                                    (0x00000001)
#define U_PNIP__SNDREADBACK_IFA_RESET__VALUE                     0x0
#define U_PNIP__DMACW_COUNT_0_IFA                                                 (0x1067A500 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_COUNT_0_IFA__ENABLECOUNT                                    (0x80000000)
#define U_PNIP__DMACW_COUNT_0_IFA__ENABLEPRIORITY                                 (0x40000000)
#define U_PNIP__DMACW_COUNT_0_IFA__DMACW_COUNT                                    (0x0000FFFF)
#define U_PNIP__DMACW_COUNT_0_IFA_RESET__VALUE                   0x0
#define U_PNIP__DMACW_COUNT_1_IFA                                                 (0x1067A504 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_COUNT_1_IFA__ENABLECOUNT                                    (0x80000000)
#define U_PNIP__DMACW_COUNT_1_IFA__ENABLEPRIORITY                                 (0x40000000)
#define U_PNIP__DMACW_COUNT_1_IFA__DMACW_COUNT                                    (0x0000FFFF)
#define U_PNIP__DMACW_COUNT_1_IFA_RESET__VALUE                   0x0
#define U_PNIP__DMACW_COUNT_2_IFA                                                 (0x1067A508 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_COUNT_2_IFA__ENABLECOUNT                                    (0x80000000)
#define U_PNIP__DMACW_COUNT_2_IFA__ENABLEPRIORITY                                 (0x40000000)
#define U_PNIP__DMACW_COUNT_2_IFA__DMACW_COUNT                                    (0x0000FFFF)
#define U_PNIP__DMACW_COUNT_2_IFA_RESET__VALUE                   0x0
#define U_PNIP__DMACW_COUNT_3_IFA                                                 (0x1067A50C + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_COUNT_3_IFA__ENABLECOUNT                                    (0x80000000)
#define U_PNIP__DMACW_COUNT_3_IFA__ENABLEPRIORITY                                 (0x40000000)
#define U_PNIP__DMACW_COUNT_3_IFA__DMACW_COUNT                                    (0x0000FFFF)
#define U_PNIP__DMACW_COUNT_3_IFA_RESET__VALUE                   0x0
#define U_PNIP__DMACW_COUNT_4_IFA                                                 (0x1067A510 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_COUNT_4_IFA__ENABLECOUNT                                    (0x80000000)
#define U_PNIP__DMACW_COUNT_4_IFA__ENABLEPRIORITY                                 (0x40000000)
#define U_PNIP__DMACW_COUNT_4_IFA__DMACW_COUNT                                    (0x0000FFFF)
#define U_PNIP__DMACW_COUNT_4_IFA_RESET__VALUE                   0x0
#define U_PNIP__DMACW_COUNT_5_IFA                                                 (0x1067A514 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_COUNT_5_IFA__ENABLECOUNT                                    (0x80000000)
#define U_PNIP__DMACW_COUNT_5_IFA__ENABLEPRIORITY                                 (0x40000000)
#define U_PNIP__DMACW_COUNT_5_IFA__DMACW_COUNT                                    (0x0000FFFF)
#define U_PNIP__DMACW_COUNT_5_IFA_RESET__VALUE                   0x0
#define U_PNIP__DMACW_COUNT_6_IFA                                                 (0x1067A518 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_COUNT_6_IFA__ENABLECOUNT                                    (0x80000000)
#define U_PNIP__DMACW_COUNT_6_IFA__ENABLEPRIORITY                                 (0x40000000)
#define U_PNIP__DMACW_COUNT_6_IFA__DMACW_COUNT                                    (0x0000FFFF)
#define U_PNIP__DMACW_COUNT_6_IFA_RESET__VALUE                   0x0
#define U_PNIP__DMACW_COUNT_7_IFA                                                 (0x1067A51C + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_COUNT_7_IFA__ENABLECOUNT                                    (0x80000000)
#define U_PNIP__DMACW_COUNT_7_IFA__ENABLEPRIORITY                                 (0x40000000)
#define U_PNIP__DMACW_COUNT_7_IFA__DMACW_COUNT                                    (0x0000FFFF)
#define U_PNIP__DMACW_COUNT_7_IFA_RESET__VALUE                   0x0
#define U_PNIP__DMACW_BARRIER_0_IFA                                               (0x1067A540 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_BARRIER_0_IFA__DMACW_BARRIER                                (0x0000FFFF)
#define U_PNIP__DMACW_BARRIER_0_IFA_RESET__VALUE                 0xFFFF
#define U_PNIP__DMACW_BARRIER_1_IFA                                               (0x1067A544 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_BARRIER_1_IFA__DMACW_BARRIER                                (0x0000FFFF)
#define U_PNIP__DMACW_BARRIER_1_IFA_RESET__VALUE                 0xFFFF
#define U_PNIP__DMACW_BARRIER_2_IFA                                               (0x1067A548 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_BARRIER_2_IFA__DMACW_BARRIER                                (0x0000FFFF)
#define U_PNIP__DMACW_BARRIER_2_IFA_RESET__VALUE                 0xFFFF
#define U_PNIP__DMACW_BARRIER_3_IFA                                               (0x1067A54C + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_BARRIER_3_IFA__DMACW_BARRIER                                (0x0000FFFF)
#define U_PNIP__DMACW_BARRIER_3_IFA_RESET__VALUE                 0xFFFF
#define U_PNIP__DMACW_BARRIER_4_IFA                                               (0x1067A550 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_BARRIER_4_IFA__DMACW_BARRIER                                (0x0000FFFF)
#define U_PNIP__DMACW_BARRIER_4_IFA_RESET__VALUE                 0xFFFF
#define U_PNIP__DMACW_BARRIER_5_IFA                                               (0x1067A554 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_BARRIER_5_IFA__DMACW_BARRIER                                (0x0000FFFF)
#define U_PNIP__DMACW_BARRIER_5_IFA_RESET__VALUE                 0xFFFF
#define U_PNIP__DMACW_BARRIER_6_IFA                                               (0x1067A558 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_BARRIER_6_IFA__DMACW_BARRIER                                (0x0000FFFF)
#define U_PNIP__DMACW_BARRIER_6_IFA_RESET__VALUE                 0xFFFF
#define U_PNIP__DMACW_BARRIER_7_IFA                                               (0x1067A55C + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_BARRIER_7_IFA__DMACW_BARRIER                                (0x0000FFFF)
#define U_PNIP__DMACW_BARRIER_7_IFA_RESET__VALUE                 0xFFFF
#define U_PNIP__DMACW_MAPPING_0TO7_IFA                                            (0x1067A580 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_MAPPING_0TO7_IFA__DMACW7_MAP                                (0xF0000000)
#define U_PNIP__DMACW_MAPPING_0TO7_IFA__DMACW6_MAP                                (0x0F000000)
#define U_PNIP__DMACW_MAPPING_0TO7_IFA__DMACW5_MAP                                (0x00F00000)
#define U_PNIP__DMACW_MAPPING_0TO7_IFA__DMACW4_MAP                                (0x000F0000)
#define U_PNIP__DMACW_MAPPING_0TO7_IFA__DMACW3_MAP                                (0x0000F000)
#define U_PNIP__DMACW_MAPPING_0TO7_IFA__DMACW2_MAP                                (0x00000F00)
#define U_PNIP__DMACW_MAPPING_0TO7_IFA__DMACW1_MAP                                (0x000000F0)
#define U_PNIP__DMACW_MAPPING_0TO7_IFA__DMACW0_MAP                                (0x0000000F)
#define U_PNIP__DMACW_MAPPING_0TO7_IFA_RESET__VALUE              0x0
#define U_PNIP__DMACW_MAPPING_8TO15_IFA                                           (0x1067A584 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_MAPPING_8TO15_IFA__DMACW15_MAP                              (0xF0000000)
#define U_PNIP__DMACW_MAPPING_8TO15_IFA__DMACW14_MAP                              (0x0F000000)
#define U_PNIP__DMACW_MAPPING_8TO15_IFA__DMACW13_MAP                              (0x00F00000)
#define U_PNIP__DMACW_MAPPING_8TO15_IFA__DMACW12_MAP                              (0x000F0000)
#define U_PNIP__DMACW_MAPPING_8TO15_IFA__DMACW11_MAP                              (0x0000F000)
#define U_PNIP__DMACW_MAPPING_8TO15_IFA__DMACW10_MAP                              (0x00000F00)
#define U_PNIP__DMACW_MAPPING_8TO15_IFA__DMACW9_MAP                               (0x000000F0)
#define U_PNIP__DMACW_MAPPING_8TO15_IFA__DMACW8_MAP                               (0x0000000F)
#define U_PNIP__DMACW_MAPPING_8TO15_IFA_RESET__VALUE             0x0
#define U_PNIP__DMACW_TRIGGER_IFA                                                 (0x1067A5C0 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_TRIGGER_IFA__DMACW_TRIGGER                                  (0x0FFFFFFF)
#define U_PNIP__DMACW_TRIGGER_IFA_RESET__VALUE                   0x0
#define U_PNIP__DMACW_STATUS_IFA                                                  (0x1067A5C4 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_STATUS_IFA__DMACW_EXPIRED                                   (0x0000FFFF)
#define U_PNIP__DMACW_STATUS_IFA_RESET__VALUE                    0x0
#define U_PNIP__SNDTABLEBASE_IFB                                                  (0x1067A800 + PNIP_DEV_TOP_BASE)
#define U_PNIP__SNDTABLEBASE_IFB__SNDTABLEBASE                                    (0x001FFFFF)
#define U_PNIP__SNDTABLEBASE_IFB_RESET__VALUE                    0xFFFF8
#define U_PNIP__SNDTRIGGER_IFB                                                    (0x1067A804 + PNIP_DEV_TOP_BASE)
#define U_PNIP__SNDTRIGGER_IFB__SNDTRIGER_SD_15                                   (0x00008000)
#define U_PNIP__SNDTRIGGER_IFB__SNDTRIGER_SD_14                                   (0x00004000)
#define U_PNIP__SNDTRIGGER_IFB__SNDTRIGER_SD_13                                   (0x00002000)
#define U_PNIP__SNDTRIGGER_IFB__SNDTRIGER_SD_12                                   (0x00001000)
#define U_PNIP__SNDTRIGGER_IFB__SNDTRIGER_SD_11                                   (0x00000800)
#define U_PNIP__SNDTRIGGER_IFB__SNDTRIGER_SD_10                                   (0x00000400)
#define U_PNIP__SNDTRIGGER_IFB__SNDTRIGER_SD_9                                    (0x00000200)
#define U_PNIP__SNDTRIGGER_IFB__SNDTRIGER_SD_8                                    (0x00000100)
#define U_PNIP__SNDTRIGGER_IFB__SNDTRIGER_SD_7                                    (0x00000080)
#define U_PNIP__SNDTRIGGER_IFB__SNDTRIGER_SD_6                                    (0x00000040)
#define U_PNIP__SNDTRIGGER_IFB__SNDTRIGER_SD_5                                    (0x00000020)
#define U_PNIP__SNDTRIGGER_IFB__SNDTRIGER_SD_4                                    (0x00000010)
#define U_PNIP__SNDTRIGGER_IFB__SNDTRIGER_SD_3                                    (0x00000008)
#define U_PNIP__SNDTRIGGER_IFB__SNDTRIGER_SD_2                                    (0x00000004)
#define U_PNIP__SNDTRIGGER_IFB__SNDTRIGER_SD_1                                    (0x00000002)
#define U_PNIP__SNDTRIGGER_IFB__SNDTRIGER_SD_0                                    (0x00000001)
#define U_PNIP__SNDTRIGGER_IFB_RESET__VALUE                      0x0
#define U_PNIP__SNDWATCHDOGLIMIT_IFB                                              (0x1067A808 + PNIP_DEV_TOP_BASE)
#define U_PNIP__SNDWATCHDOGLIMIT_IFB__SDWATCHDOGLIMIT                             (0x0000FFFF)
#define U_PNIP__SNDWATCHDOGLIMIT_IFB_RESET__VALUE                0x0
#define U_PNIP__SNDEVENT_IFB                                                      (0x1067A80C + PNIP_DEV_TOP_BASE)
#define U_PNIP__SNDEVENT_IFB__SNDEVENT                                            (0x000003FF)
#define U_PNIP__SNDEVENT_IFB_RESET__VALUE                        0x0
#define U_PNIP__SNDSTATUS_IFB                                                     (0x1067A810 + PNIP_DEV_TOP_BASE)
#define U_PNIP__SNDSTATUS_IFB__SNDSTATUS_SD_15                                    (0x00008000)
#define U_PNIP__SNDSTATUS_IFB__SNDSTATUS_SD_14                                    (0x00004000)
#define U_PNIP__SNDSTATUS_IFB__SNDSTATUS_SD_13                                    (0x00002000)
#define U_PNIP__SNDSTATUS_IFB__SNDSTATUS_SD_12                                    (0x00001000)
#define U_PNIP__SNDSTATUS_IFB__SNDSTATUS_SD_11                                    (0x00000800)
#define U_PNIP__SNDSTATUS_IFB__SNDSTATUS_SD_10                                    (0x00000400)
#define U_PNIP__SNDSTATUS_IFB__SNDSTATUS_SD_9                                     (0x00000200)
#define U_PNIP__SNDSTATUS_IFB__SNDSTATUS_SD_8                                     (0x00000100)
#define U_PNIP__SNDSTATUS_IFB__SNDSTATUS_SD_7                                     (0x00000080)
#define U_PNIP__SNDSTATUS_IFB__SNDSTATUS_SD_6                                     (0x00000040)
#define U_PNIP__SNDSTATUS_IFB__SNDSTATUS_SD_5                                     (0x00000020)
#define U_PNIP__SNDSTATUS_IFB__SNDSTATUS_SD_4                                     (0x00000010)
#define U_PNIP__SNDSTATUS_IFB__SNDSTATUS_SD_3                                     (0x00000008)
#define U_PNIP__SNDSTATUS_IFB__SNDSTATUS_SD_2                                     (0x00000004)
#define U_PNIP__SNDSTATUS_IFB__SNDSTATUS_SD_1                                     (0x00000002)
#define U_PNIP__SNDSTATUS_IFB__SNDSTATUS_SD_0                                     (0x00000001)
#define U_PNIP__SNDSTATUS_IFB_RESET__VALUE                       0x0
#define U_PNIP__SNDERREVENT_IFB                                                   (0x1067A814 + PNIP_DEV_TOP_BASE)
#define U_PNIP__SNDERREVENT_IFB__ERROREVENT                                       (0x0000000F)
#define U_PNIP__SNDERREVENT_IFB_RESET__VALUE                     0x0
#define U_PNIP__SNDERRSTATUS_IFB                                                  (0x1067A818 + PNIP_DEV_TOP_BASE)
#define U_PNIP__SNDERRSTATUS_IFB__SNDERRSTATUS_SD_15                              (0x00008000)
#define U_PNIP__SNDERRSTATUS_IFB__SNDERRSTATUS_SD_14                              (0x00004000)
#define U_PNIP__SNDERRSTATUS_IFB__SNDERRSTATUS_SD_13                              (0x00002000)
#define U_PNIP__SNDERRSTATUS_IFB__SNDERRSTATUS_SD_12                              (0x00001000)
#define U_PNIP__SNDERRSTATUS_IFB__SNDERRSTATUS_SD_11                              (0x00000800)
#define U_PNIP__SNDERRSTATUS_IFB__SNDERRSTATUS_SD_10                              (0x00000400)
#define U_PNIP__SNDERRSTATUS_IFB__SNDERRSTATUS_SD_9                               (0x00000200)
#define U_PNIP__SNDERRSTATUS_IFB__SNDERRSTATUS_SD_8                               (0x00000100)
#define U_PNIP__SNDERRSTATUS_IFB__SNDERRSTATUS_SD_7                               (0x00000080)
#define U_PNIP__SNDERRSTATUS_IFB__SNDERRSTATUS_SD_6                               (0x00000040)
#define U_PNIP__SNDERRSTATUS_IFB__SNDERRSTATUS_SD_5                               (0x00000020)
#define U_PNIP__SNDERRSTATUS_IFB__SNDERRSTATUS_SD_4                               (0x00000010)
#define U_PNIP__SNDERRSTATUS_IFB__SNDERRSTATUS_SD_3                               (0x00000008)
#define U_PNIP__SNDERRSTATUS_IFB__SNDERRSTATUS_SD_2                               (0x00000004)
#define U_PNIP__SNDERRSTATUS_IFB__SNDERRSTATUS_SD_1                               (0x00000002)
#define U_PNIP__SNDERRSTATUS_IFB__SNDERRSTATUS_SD_0                               (0x00000001)
#define U_PNIP__SNDERRSTATUS_IFB_RESET__VALUE                    0x0
#define U_PNIP__SNDREADBACK_IFB                                                   (0x1067A81C + PNIP_DEV_TOP_BASE)
#define U_PNIP__SNDREADBACK_IFB__RCVREADBACK_15                                   (0x00008000)
#define U_PNIP__SNDREADBACK_IFB__RCVREADBACK_14                                   (0x00004000)
#define U_PNIP__SNDREADBACK_IFB__RCVREADBACK_13                                   (0x00002000)
#define U_PNIP__SNDREADBACK_IFB__RCVREADBACK_12                                   (0x00001000)
#define U_PNIP__SNDREADBACK_IFB__RCVREADBACK_11                                   (0x00000800)
#define U_PNIP__SNDREADBACK_IFB__RCVREADBACK_10                                   (0x00000400)
#define U_PNIP__SNDREADBACK_IFB__RCVREADBACK_9                                    (0x00000200)
#define U_PNIP__SNDREADBACK_IFB__RCVREADBACK_8                                    (0x00000100)
#define U_PNIP__SNDREADBACK_IFB__RCVREADBACK_7                                    (0x00000080)
#define U_PNIP__SNDREADBACK_IFB__RCVREADBACK_6                                    (0x00000040)
#define U_PNIP__SNDREADBACK_IFB__RCVREADBACK_5                                    (0x00000020)
#define U_PNIP__SNDREADBACK_IFB__RCVREADBACK_4                                    (0x00000010)
#define U_PNIP__SNDREADBACK_IFB__RCVREADBACK_3                                    (0x00000008)
#define U_PNIP__SNDREADBACK_IFB__RCVREADBACK_2                                    (0x00000004)
#define U_PNIP__SNDREADBACK_IFB__RCVREADBACK_1                                    (0x00000002)
#define U_PNIP__SNDREADBACK_IFB__RCVREADBACK_0                                    (0x00000001)
#define U_PNIP__SNDREADBACK_IFB_RESET__VALUE                     0x0
#define U_PNIP__DMACW_COUNT_0_IFB                                                 (0x1067A900 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_COUNT_0_IFB__ENABLECOUNT                                    (0x80000000)
#define U_PNIP__DMACW_COUNT_0_IFB__ENABLEPRIORITY                                 (0x40000000)
#define U_PNIP__DMACW_COUNT_0_IFB__DMACW_COUNT                                    (0x0000FFFF)
#define U_PNIP__DMACW_COUNT_0_IFB_RESET__VALUE                   0x0
#define U_PNIP__DMACW_COUNT_1_IFB                                                 (0x1067A904 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_COUNT_1_IFB__ENABLECOUNT                                    (0x80000000)
#define U_PNIP__DMACW_COUNT_1_IFB__ENABLEPRIORITY                                 (0x40000000)
#define U_PNIP__DMACW_COUNT_1_IFB__DMACW_COUNT                                    (0x0000FFFF)
#define U_PNIP__DMACW_COUNT_1_IFB_RESET__VALUE                   0x0
#define U_PNIP__DMACW_COUNT_2_IFB                                                 (0x1067A908 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_COUNT_2_IFB__ENABLECOUNT                                    (0x80000000)
#define U_PNIP__DMACW_COUNT_2_IFB__ENABLEPRIORITY                                 (0x40000000)
#define U_PNIP__DMACW_COUNT_2_IFB__DMACW_COUNT                                    (0x0000FFFF)
#define U_PNIP__DMACW_COUNT_2_IFB_RESET__VALUE                   0x0
#define U_PNIP__DMACW_COUNT_3_IFB                                                 (0x1067A90C + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_COUNT_3_IFB__ENABLECOUNT                                    (0x80000000)
#define U_PNIP__DMACW_COUNT_3_IFB__ENABLEPRIORITY                                 (0x40000000)
#define U_PNIP__DMACW_COUNT_3_IFB__DMACW_COUNT                                    (0x0000FFFF)
#define U_PNIP__DMACW_COUNT_3_IFB_RESET__VALUE                   0x0
#define U_PNIP__DMACW_COUNT_4_IFB                                                 (0x1067A910 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_COUNT_4_IFB__ENABLECOUNT                                    (0x80000000)
#define U_PNIP__DMACW_COUNT_4_IFB__ENABLEPRIORITY                                 (0x40000000)
#define U_PNIP__DMACW_COUNT_4_IFB__DMACW_COUNT                                    (0x0000FFFF)
#define U_PNIP__DMACW_COUNT_4_IFB_RESET__VALUE                   0x0
#define U_PNIP__DMACW_COUNT_5_IFB                                                 (0x1067A914 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_COUNT_5_IFB__ENABLECOUNT                                    (0x80000000)
#define U_PNIP__DMACW_COUNT_5_IFB__ENABLEPRIORITY                                 (0x40000000)
#define U_PNIP__DMACW_COUNT_5_IFB__DMACW_COUNT                                    (0x0000FFFF)
#define U_PNIP__DMACW_COUNT_5_IFB_RESET__VALUE                   0x0
#define U_PNIP__DMACW_COUNT_6_IFB                                                 (0x1067A918 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_COUNT_6_IFB__ENABLECOUNT                                    (0x80000000)
#define U_PNIP__DMACW_COUNT_6_IFB__ENABLEPRIORITY                                 (0x40000000)
#define U_PNIP__DMACW_COUNT_6_IFB__DMACW_COUNT                                    (0x0000FFFF)
#define U_PNIP__DMACW_COUNT_6_IFB_RESET__VALUE                   0x0
#define U_PNIP__DMACW_COUNT_7_IFB                                                 (0x1067A91C + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_COUNT_7_IFB__ENABLECOUNT                                    (0x80000000)
#define U_PNIP__DMACW_COUNT_7_IFB__ENABLEPRIORITY                                 (0x40000000)
#define U_PNIP__DMACW_COUNT_7_IFB__DMACW_COUNT                                    (0x0000FFFF)
#define U_PNIP__DMACW_COUNT_7_IFB_RESET__VALUE                   0x0
#define U_PNIP__DMACW_BARRIER_0_IFB                                               (0x1067A940 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_BARRIER_0_IFB__DMACW_BARRIER                                (0x0000FFFF)
#define U_PNIP__DMACW_BARRIER_0_IFB_RESET__VALUE                 0xFFFF
#define U_PNIP__DMACW_BARRIER_1_IFB                                               (0x1067A944 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_BARRIER_1_IFB__DMACW_BARRIER                                (0x0000FFFF)
#define U_PNIP__DMACW_BARRIER_1_IFB_RESET__VALUE                 0xFFFF
#define U_PNIP__DMACW_BARRIER_2_IFB                                               (0x1067A948 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_BARRIER_2_IFB__DMACW_BARRIER                                (0x0000FFFF)
#define U_PNIP__DMACW_BARRIER_2_IFB_RESET__VALUE                 0xFFFF
#define U_PNIP__DMACW_BARRIER_3_IFB                                               (0x1067A94C + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_BARRIER_3_IFB__DMACW_BARRIER                                (0x0000FFFF)
#define U_PNIP__DMACW_BARRIER_3_IFB_RESET__VALUE                 0xFFFF
#define U_PNIP__DMACW_BARRIER_4_IFB                                               (0x1067A950 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_BARRIER_4_IFB__DMACW_BARRIER                                (0x0000FFFF)
#define U_PNIP__DMACW_BARRIER_4_IFB_RESET__VALUE                 0xFFFF
#define U_PNIP__DMACW_BARRIER_5_IFB                                               (0x1067A954 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_BARRIER_5_IFB__DMACW_BARRIER                                (0x0000FFFF)
#define U_PNIP__DMACW_BARRIER_5_IFB_RESET__VALUE                 0xFFFF
#define U_PNIP__DMACW_BARRIER_6_IFB                                               (0x1067A958 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_BARRIER_6_IFB__DMACW_BARRIER                                (0x0000FFFF)
#define U_PNIP__DMACW_BARRIER_6_IFB_RESET__VALUE                 0xFFFF
#define U_PNIP__DMACW_BARRIER_7_IFB                                               (0x1067A95C + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_BARRIER_7_IFB__DMACW_BARRIER                                (0x0000FFFF)
#define U_PNIP__DMACW_BARRIER_7_IFB_RESET__VALUE                 0xFFFF
#define U_PNIP__DMACW_MAPPING_0TO7_IFB                                            (0x1067A980 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_MAPPING_0TO7_IFB__DMACW7_MAP                                (0xF0000000)
#define U_PNIP__DMACW_MAPPING_0TO7_IFB__DMACW6_MAP                                (0x0F000000)
#define U_PNIP__DMACW_MAPPING_0TO7_IFB__DMACW5_MAP                                (0x00F00000)
#define U_PNIP__DMACW_MAPPING_0TO7_IFB__DMACW4_MAP                                (0x000F0000)
#define U_PNIP__DMACW_MAPPING_0TO7_IFB__DMACW3_MAP                                (0x0000F000)
#define U_PNIP__DMACW_MAPPING_0TO7_IFB__DMACW2_MAP                                (0x00000F00)
#define U_PNIP__DMACW_MAPPING_0TO7_IFB__DMACW1_MAP                                (0x000000F0)
#define U_PNIP__DMACW_MAPPING_0TO7_IFB__DMACW0_MAP                                (0x0000000F)
#define U_PNIP__DMACW_MAPPING_0TO7_IFB_RESET__VALUE              0x0
#define U_PNIP__DMACW_MAPPING_8TO15_IFB                                           (0x1067A984 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_MAPPING_8TO15_IFB__DMACW15_MAP                              (0xF0000000)
#define U_PNIP__DMACW_MAPPING_8TO15_IFB__DMACW14_MAP                              (0x0F000000)
#define U_PNIP__DMACW_MAPPING_8TO15_IFB__DMACW13_MAP                              (0x00F00000)
#define U_PNIP__DMACW_MAPPING_8TO15_IFB__DMACW12_MAP                              (0x000F0000)
#define U_PNIP__DMACW_MAPPING_8TO15_IFB__DMACW11_MAP                              (0x0000F000)
#define U_PNIP__DMACW_MAPPING_8TO15_IFB__DMACW10_MAP                              (0x00000F00)
#define U_PNIP__DMACW_MAPPING_8TO15_IFB__DMACW9_MAP                               (0x000000F0)
#define U_PNIP__DMACW_MAPPING_8TO15_IFB__DMACW8_MAP                               (0x0000000F)
#define U_PNIP__DMACW_MAPPING_8TO15_IFB_RESET__VALUE             0x0
#define U_PNIP__DMACW_TRIGGER_IFB                                                 (0x1067A9C0 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_TRIGGER_IFB__DMACW_TRIGGER                                  (0x0FFFFFFF)
#define U_PNIP__DMACW_TRIGGER_IFB_RESET__VALUE                   0x0
#define U_PNIP__DMACW_STATUS_IFB                                                  (0x1067A9C4 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DMACW_STATUS_IFB__DMACW_EXPIRED                                   (0x0000FFFF)
#define U_PNIP__DMACW_STATUS_IFB_RESET__VALUE                    0x0
#define U_PNIP__ARCVCOMMAND_IF_CONTROL                                            (0x1067B000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ARCVCOMMAND_IF_CONTROL__PARAMETER                                 (0xFFFFFF00)
#define U_PNIP__ARCVCOMMAND_IF_CONTROL__USER_ID                                   (0x000000E0)
#define U_PNIP__ARCVCOMMAND_IF_CONTROL__CONFREQUEST                               (0x00000010)
#define U_PNIP__ARCVCOMMAND_IF_CONTROL__IF_SELECT                                 (0x00000008)
#define U_PNIP__ARCVCOMMAND_IF_CONTROL__F_CODE                                    (0x00000007)
#define U_PNIP__ARCVCOMMAND_IF_CONTROL_RESET__VALUE              0x0
#define U_PNIP__ARCVCOMMAND_IF_STATUS                                             (0x1067B004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ARCVCOMMAND_IF_STATUS__RETURNVALUE                                (0xFFFFFF00)
#define U_PNIP__ARCVCOMMAND_IF_STATUS__USER_ID                                    (0x000000E0)
#define U_PNIP__ARCVCOMMAND_IF_STATUS__CONFRESPONSE                               (0x00000010)
#define U_PNIP__ARCVCOMMAND_IF_STATUS__CONFERROR                                  (0x00000008)
#define U_PNIP__ARCVCOMMAND_IF_STATUS__F_CODE                                     (0x00000007)
#define U_PNIP__ARCVCOMMAND_IF_STATUS_RESET__VALUE               0x0
#define U_PNIP__RCVTABLEBASE_IFA                                                  (0x1067B400 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RCVTABLEBASE_IFA__RCVTABLEBASE                                    (0x001FFFFF)
#define U_PNIP__RCVTABLEBASE_IFA_RESET__VALUE                    0xFFFF8
#define U_PNIP__RCVENABLED_IFA                                                    (0x1067B404 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_31                                     (0x80000000)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_30                                     (0x40000000)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_29                                     (0x20000000)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_28                                     (0x10000000)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_27                                     (0x08000000)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_26                                     (0x04000000)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_25                                     (0x02000000)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_24                                     (0x01000000)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_23                                     (0x00800000)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_22                                     (0x00400000)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_21                                     (0x00200000)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_20                                     (0x00100000)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_19                                     (0x00080000)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_18                                     (0x00040000)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_17                                     (0x00020000)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_16                                     (0x00010000)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_15                                     (0x00008000)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_14                                     (0x00004000)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_13                                     (0x00002000)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_12                                     (0x00001000)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_11                                     (0x00000800)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_10                                     (0x00000400)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_9                                      (0x00000200)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_8                                      (0x00000100)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_7                                      (0x00000080)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_6                                      (0x00000040)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_5                                      (0x00000020)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_4                                      (0x00000010)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_3                                      (0x00000008)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_2                                      (0x00000004)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_1                                      (0x00000002)
#define U_PNIP__RCVENABLED_IFA__RCVENABLED_0                                      (0x00000001)
#define U_PNIP__RCVENABLED_IFA_RESET__VALUE                      0x0
#define U_PNIP__RCVEVENT_IFA                                                      (0x1067B408 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RCVEVENT_IFA__RCVEVENT                                            (0x0000000F)
#define U_PNIP__RCVEVENT_IFA_RESET__VALUE                        0x0
#define U_PNIP__RCVSTATUS_IFA                                                     (0x1067B40C + PNIP_DEV_TOP_BASE)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_31                                   (0x80000000)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_30                                   (0x40000000)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_29                                   (0x20000000)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_28                                   (0x10000000)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_27                                   (0x08000000)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_26                                   (0x04000000)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_25                                   (0x02000000)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_24                                   (0x01000000)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_23                                   (0x00800000)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_22                                   (0x00400000)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_21                                   (0x00200000)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_20                                   (0x00100000)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_19                                   (0x00080000)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_18                                   (0x00040000)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_17                                   (0x00020000)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_16                                   (0x00010000)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_15                                   (0x00008000)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_14                                   (0x00004000)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_13                                   (0x00002000)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_12                                   (0x00001000)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_11                                   (0x00000800)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_10                                   (0x00000400)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_9                                    (0x00000200)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_8                                    (0x00000100)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_7                                    (0x00000080)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_6                                    (0x00000040)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_5                                    (0x00000020)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_4                                    (0x00000010)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_3                                    (0x00000008)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_2                                    (0x00000004)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_1                                    (0x00000002)
#define U_PNIP__RCVSTATUS_IFA__RECEIVESTATUS_0                                    (0x00000001)
#define U_PNIP__RCVSTATUS_IFA_RESET__VALUE                       0x0
#define U_PNIP__RCVERREVENT_IFA                                                   (0x1067B410 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RCVERREVENT_IFA__ERROREVENT                                       (0x0000000F)
#define U_PNIP__RCVERREVENT_IFA_RESET__VALUE                     0x0
#define U_PNIP__RCVERRSTATUS_IFA                                                  (0x1067B414 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_31                                 (0x80000000)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_30                                 (0x40000000)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_29                                 (0x20000000)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_28                                 (0x10000000)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_27                                 (0x08000000)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_26                                 (0x04000000)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_25                                 (0x02000000)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_24                                 (0x01000000)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_23                                 (0x00800000)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_22                                 (0x00400000)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_21                                 (0x00200000)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_20                                 (0x00100000)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_19                                 (0x00080000)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_18                                 (0x00040000)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_17                                 (0x00020000)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_16                                 (0x00010000)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_15                                 (0x00008000)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_14                                 (0x00004000)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_13                                 (0x00002000)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_12                                 (0x00001000)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_11                                 (0x00000800)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_10                                 (0x00000400)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_9                                  (0x00000200)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_8                                  (0x00000100)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_7                                  (0x00000080)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_6                                  (0x00000040)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_5                                  (0x00000020)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_4                                  (0x00000010)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_3                                  (0x00000008)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_2                                  (0x00000004)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_1                                  (0x00000002)
#define U_PNIP__RCVERRSTATUS_IFA__RCVERRSTATUS_0                                  (0x00000001)
#define U_PNIP__RCVERRSTATUS_IFA_RESET__VALUE                    0x0
#define U_PNIP__RCVREADBACK_IFA                                                   (0x1067B418 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_31                                   (0x80000000)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_30                                   (0x40000000)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_29                                   (0x20000000)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_28                                   (0x10000000)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_27                                   (0x08000000)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_26                                   (0x04000000)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_25                                   (0x02000000)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_24                                   (0x01000000)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_23                                   (0x00800000)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_22                                   (0x00400000)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_21                                   (0x00200000)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_20                                   (0x00100000)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_19                                   (0x00080000)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_18                                   (0x00040000)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_17                                   (0x00020000)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_16                                   (0x00010000)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_15                                   (0x00008000)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_14                                   (0x00004000)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_13                                   (0x00002000)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_12                                   (0x00001000)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_11                                   (0x00000800)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_10                                   (0x00000400)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_9                                    (0x00000200)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_8                                    (0x00000100)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_7                                    (0x00000080)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_6                                    (0x00000040)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_5                                    (0x00000020)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_4                                    (0x00000010)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_3                                    (0x00000008)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_2                                    (0x00000004)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_1                                    (0x00000002)
#define U_PNIP__RCVREADBACK_IFA__RCVREADBACK_0                                    (0x00000001)
#define U_PNIP__RCVREADBACK_IFA_RESET__VALUE                     0x0
#define U_PNIP__RCVTABLEBASE_IFB                                                  (0x1067B800 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RCVTABLEBASE_IFB__RCVTABLEBASE                                    (0x001FFFFF)
#define U_PNIP__RCVTABLEBASE_IFB_RESET__VALUE                    0xFFFF8
#define U_PNIP__RCVENABLED_IFB                                                    (0x1067B804 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_31                                     (0x80000000)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_30                                     (0x40000000)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_29                                     (0x20000000)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_28                                     (0x10000000)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_27                                     (0x08000000)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_26                                     (0x04000000)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_25                                     (0x02000000)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_24                                     (0x01000000)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_23                                     (0x00800000)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_22                                     (0x00400000)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_21                                     (0x00200000)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_20                                     (0x00100000)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_19                                     (0x00080000)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_18                                     (0x00040000)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_17                                     (0x00020000)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_16                                     (0x00010000)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_15                                     (0x00008000)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_14                                     (0x00004000)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_13                                     (0x00002000)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_12                                     (0x00001000)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_11                                     (0x00000800)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_10                                     (0x00000400)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_9                                      (0x00000200)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_8                                      (0x00000100)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_7                                      (0x00000080)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_6                                      (0x00000040)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_5                                      (0x00000020)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_4                                      (0x00000010)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_3                                      (0x00000008)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_2                                      (0x00000004)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_1                                      (0x00000002)
#define U_PNIP__RCVENABLED_IFB__RCVENABLED_0                                      (0x00000001)
#define U_PNIP__RCVENABLED_IFB_RESET__VALUE                      0x0
#define U_PNIP__RCVEVENT_IFB                                                      (0x1067B808 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RCVEVENT_IFB__RCVEVENT                                            (0x00000003)
#define U_PNIP__RCVEVENT_IFB_RESET__VALUE                        0x0
#define U_PNIP__RCVSTATUS_IFB                                                     (0x1067B80C + PNIP_DEV_TOP_BASE)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_31                                   (0x80000000)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_30                                   (0x40000000)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_29                                   (0x20000000)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_28                                   (0x10000000)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_27                                   (0x08000000)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_26                                   (0x04000000)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_25                                   (0x02000000)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_24                                   (0x01000000)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_23                                   (0x00800000)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_22                                   (0x00400000)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_21                                   (0x00200000)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_20                                   (0x00100000)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_19                                   (0x00080000)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_18                                   (0x00040000)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_17                                   (0x00020000)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_16                                   (0x00010000)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_15                                   (0x00008000)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_14                                   (0x00004000)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_13                                   (0x00002000)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_12                                   (0x00001000)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_11                                   (0x00000800)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_10                                   (0x00000400)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_9                                    (0x00000200)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_8                                    (0x00000100)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_7                                    (0x00000080)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_6                                    (0x00000040)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_5                                    (0x00000020)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_4                                    (0x00000010)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_3                                    (0x00000008)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_2                                    (0x00000004)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_1                                    (0x00000002)
#define U_PNIP__RCVSTATUS_IFB__RECEIVESTATUS_0                                    (0x00000001)
#define U_PNIP__RCVSTATUS_IFB_RESET__VALUE                       0x0
#define U_PNIP__RCVERREVENT_IFB                                                   (0x1067B810 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RCVERREVENT_IFB__ERROREVENT                                       (0x0000000F)
#define U_PNIP__RCVERREVENT_IFB_RESET__VALUE                     0x0
#define U_PNIP__RCVERRSTATUS_IFB                                                  (0x1067B814 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_31                                 (0x80000000)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_30                                 (0x40000000)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_29                                 (0x20000000)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_28                                 (0x10000000)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_27                                 (0x08000000)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_26                                 (0x04000000)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_25                                 (0x02000000)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_24                                 (0x01000000)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_23                                 (0x00800000)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_22                                 (0x00400000)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_21                                 (0x00200000)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_20                                 (0x00100000)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_19                                 (0x00080000)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_18                                 (0x00040000)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_17                                 (0x00020000)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_16                                 (0x00010000)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_15                                 (0x00008000)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_14                                 (0x00004000)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_13                                 (0x00002000)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_12                                 (0x00001000)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_11                                 (0x00000800)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_10                                 (0x00000400)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_9                                  (0x00000200)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_8                                  (0x00000100)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_7                                  (0x00000080)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_6                                  (0x00000040)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_5                                  (0x00000020)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_4                                  (0x00000010)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_3                                  (0x00000008)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_2                                  (0x00000004)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_1                                  (0x00000002)
#define U_PNIP__RCVERRSTATUS_IFB__RCVERRSTATUS_0                                  (0x00000001)
#define U_PNIP__RCVERRSTATUS_IFB_RESET__VALUE                    0x0
#define U_PNIP__RCVREADBACK_IFB                                                   (0x1067B818 + PNIP_DEV_TOP_BASE)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_31                                   (0x80000000)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_30                                   (0x40000000)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_29                                   (0x20000000)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_28                                   (0x10000000)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_27                                   (0x08000000)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_26                                   (0x04000000)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_25                                   (0x02000000)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_24                                   (0x01000000)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_23                                   (0x00800000)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_22                                   (0x00400000)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_21                                   (0x00200000)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_20                                   (0x00100000)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_19                                   (0x00080000)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_18                                   (0x00040000)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_17                                   (0x00020000)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_16                                   (0x00010000)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_15                                   (0x00008000)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_14                                   (0x00004000)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_13                                   (0x00002000)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_12                                   (0x00001000)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_11                                   (0x00000800)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_10                                   (0x00000400)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_9                                    (0x00000200)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_8                                    (0x00000100)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_7                                    (0x00000080)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_6                                    (0x00000040)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_5                                    (0x00000020)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_4                                    (0x00000010)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_3                                    (0x00000008)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_2                                    (0x00000004)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_1                                    (0x00000002)
#define U_PNIP__RCVREADBACK_IFB__RCVREADBACK_0                                    (0x00000001)
#define U_PNIP__RCVREADBACK_IFB_RESET__VALUE                     0x0
#define U_PNIP__PPMCOMMAND_IF_CONTROL                                             (0x1067C000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PPMCOMMAND_IF_CONTROL__COMMANDVALUE                               (0xFFFFFF00)
#define U_PNIP__PPMCOMMAND_IF_CONTROL__USER_ID                                    (0x000000E0)
#define U_PNIP__PPMCOMMAND_IF_CONTROL__CONFREQUEST                                (0x00000010)
#define U_PNIP__PPMCOMMAND_IF_CONTROL__IF_SELECT                                  (0x00000008)
#define U_PNIP__PPMCOMMAND_IF_CONTROL__F_CODE                                     (0x00000007)
#define U_PNIP__PPMCOMMAND_IF_CONTROL_RESET__VALUE               0x0
#define U_PNIP__PPMCOMMAND_IF_STATUS                                              (0x1067C004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PPMCOMMAND_IF_STATUS__RETURNVALUE                                 (0xFFFFFF00)
#define U_PNIP__PPMCOMMAND_IF_STATUS__USER_ID                                     (0x000000E0)
#define U_PNIP__PPMCOMMAND_IF_STATUS__CONFRESPONSE                                (0x00000010)
#define U_PNIP__PPMCOMMAND_IF_STATUS__CONFERROR                                   (0x00000008)
#define U_PNIP__PPMCOMMAND_IF_STATUS__F_CODE                                      (0x00000007)
#define U_PNIP__PPMCOMMAND_IF_STATUS_RESET__VALUE                0x0
#define U_PNIP__PPMERRSTATUS                                                      (0x1067C008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PPMERRSTATUS__PPM_ERR_INTERFACE                                   (0x00100000)
#define U_PNIP__PPMERRSTATUS__INT_PPM_ERR                                         (0x000F0000)
#define U_PNIP__PPMERRSTATUS__INT_PPM_DMA_ERR_G2                                  (0x00000002)
#define U_PNIP__PPMERRSTATUS__INT_PPM_DMA_ERR_G1                                  (0x00000001)
#define U_PNIP__PPMERRSTATUS_RESET__VALUE                        0x0
#define U_PNIP__PPMERRADR                                                         (0x1067C00C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PPMERRADR__PPMERRADR                                              (0xFFFFFFFF)
#define U_PNIP__PPMERRADR_RESET__VALUE                           0x0
#define U_PNIP__PPM_FSO_LEADTIME                                                  (0x1067C010 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PPM_FSO_LEADTIME__PPM_FSO_LEADTIME                                (0x00003FF8)
#define U_PNIP__PPM_FSO_LEADTIME_RESET__VALUE                    0x0
#define U_PNIP__LOCAL_IP_HEADER                                                   (0x1067C400 + PNIP_DEV_TOP_BASE)
#define U_PNIP__LOCAL_IP_HEADER__IP_DIFFERENCIAL_SERVICES                         (0x0000FF00)
#define U_PNIP__LOCAL_IP_HEADER__IP_TTL                                           (0x000000FF)
#define U_PNIP__LOCAL_IP_HEADER_RESET__VALUE                     0x0
#define U_PNIP__LOCAL_IP_SA_0                                                     (0x1067C404 + PNIP_DEV_TOP_BASE)
#define U_PNIP__LOCAL_IP_SA_0__LOCAL_IP_SA                                        (0xFFFFFFFF)
#define U_PNIP__LOCAL_IP_SA_0_RESET__VALUE                       0x0
#define U_PNIP__LOCAL_IP_SA_1                                                     (0x1067C408 + PNIP_DEV_TOP_BASE)
#define U_PNIP__LOCAL_IP_SA_1__LOCAL_IP_SA                                        (0xFFFFFFFF)
#define U_PNIP__LOCAL_IP_SA_1_RESET__VALUE                       0x0
#define U_PNIP__LOCAL_IP_SA_2                                                     (0x1067C40C + PNIP_DEV_TOP_BASE)
#define U_PNIP__LOCAL_IP_SA_2__LOCAL_IP_SA                                        (0xFFFFFFFF)
#define U_PNIP__LOCAL_IP_SA_2_RESET__VALUE                       0x0
#define U_PNIP__LOCAL_IP_SA_3                                                     (0x1067C410 + PNIP_DEV_TOP_BASE)
#define U_PNIP__LOCAL_IP_SA_3__LOCAL_IP_SA                                        (0xFFFFFFFF)
#define U_PNIP__LOCAL_IP_SA_3_RESET__VALUE                       0x0
#define U_PNIP__LOCAL_SPORT_ETYPE_0                                               (0x1067C414 + PNIP_DEV_TOP_BASE)
#define U_PNIP__LOCAL_SPORT_ETYPE_0__LOCAL_SPORT_ETYPE                            (0x0000FFFF)
#define U_PNIP__LOCAL_SPORT_ETYPE_0_RESET__VALUE                 0x0
#define U_PNIP__LOCAL_SPORT_ETYPE_1                                               (0x1067C418 + PNIP_DEV_TOP_BASE)
#define U_PNIP__LOCAL_SPORT_ETYPE_1__LOCAL_SPORT_ETYPE                            (0x0000FFFF)
#define U_PNIP__LOCAL_SPORT_ETYPE_1_RESET__VALUE                 0x0
#define U_PNIP__LOCAL_SPORT_ETYPE_2                                               (0x1067C41C + PNIP_DEV_TOP_BASE)
#define U_PNIP__LOCAL_SPORT_ETYPE_2__LOCAL_SPORT_ETYPE                            (0x0000FFFF)
#define U_PNIP__LOCAL_SPORT_ETYPE_2_RESET__VALUE                 0x0
#define U_PNIP__LOCAL_SPORT_ETYPE_3                                               (0x1067C420 + PNIP_DEV_TOP_BASE)
#define U_PNIP__LOCAL_SPORT_ETYPE_3__LOCAL_SPORT_ETYPE                            (0x0000FFFF)
#define U_PNIP__LOCAL_SPORT_ETYPE_3_RESET__VALUE                 0x0
#define U_PNIP__PPMGROUPSTART_G1                                                  (0x1067C464 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PPMGROUPSTART_G1__GROUPSTART_CR                                   (0x0FFFFFFE)
#define U_PNIP__PPMGROUPSTART_G1_RESET__VALUE                    0x0
#define U_PNIP__PPMGROUPSTART_G2                                                  (0x1067C468 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PPMGROUPSTART_G2__GROUPSTART_CR                                   (0x0FFFFFFE)
#define U_PNIP__PPMGROUPSTART_G2_RESET__VALUE                    0x0
#define U_PNIP__PPMTRANSFEREND_G1                                                 (0x1067C46C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PPMTRANSFEREND_G1__DMAEND_CR                                      (0x0FFFFFFE)
#define U_PNIP__PPMTRANSFEREND_G1_RESET__VALUE                   0x0
#define U_PNIP__PPMTRANSFEREND_G2                                                 (0x1067C470 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PPMTRANSFEREND_G2__DMAEND_CR                                      (0x0FFFFFFE)
#define U_PNIP__PPMTRANSFEREND_G2_RESET__VALUE                   0x0
#define U_PNIP__PPM_CONTROL_IFA                                                   (0x1067C800 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PPM_CONTROL_IFA__PPM_ENABLE                                       (0x80000000)
#define U_PNIP__PPM_CONTROL_IFA_RESET__VALUE                     0x0
#define U_PNIP__ACW_TBASEPTR_IFA                                                  (0x1067C804 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ACW_TBASEPTR_IFA__ACW_TBASEPTR                                    (0x001FFFFF)
#define U_PNIP__ACW_TBASEPTR_IFA_RESET__VALUE                    0x80000
#define U_PNIP__ACW_TREDUCTIONSELECT_IFA                                          (0x1067C808 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ACW_TREDUCTIONSELECT_IFA__ACW_TREDUCTIONSELECT                    (0x00007FFF)
#define U_PNIP__ACW_TREDUCTIONSELECT_IFA_RESET__VALUE            0x0
#define U_PNIP__ACW_T_PO1K_IFA                                                    (0x1067C80C + PNIP_DEV_TOP_BASE)
#define U_PNIP__ACW_T_PO1K_IFA__PHASE_OFFSET                                      (0x0000000F)
#define U_PNIP__ACW_T_PO1K_IFA_RESET__VALUE                      0x0
#define U_PNIP__ACW_T_PO2K_IFA                                                    (0x1067C810 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ACW_T_PO2K_IFA__PHASE_OFFSET                                      (0x0000001F)
#define U_PNIP__ACW_T_PO2K_IFA_RESET__VALUE                      0x0
#define U_PNIP__ACW_T_PO4K_IFA                                                    (0x1067C814 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ACW_T_PO4K_IFA__PHASE_OFFSET                                      (0x0000003F)
#define U_PNIP__ACW_T_PO4K_IFA_RESET__VALUE                      0x0
#define U_PNIP__ACW_T_PO8K_IFA                                                    (0x1067C818 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ACW_T_PO8K_IFA__PHASE_OFFSET                                      (0x0000007F)
#define U_PNIP__ACW_T_PO8K_IFA_RESET__VALUE                      0x0
#define U_PNIP__ACW_T_PO16K_IFA                                                   (0x1067C81C + PNIP_DEV_TOP_BASE)
#define U_PNIP__ACW_T_PO16K_IFA__PHASE_OFFSET                                     (0x000000FF)
#define U_PNIP__ACW_T_PO16K_IFA_RESET__VALUE                     0x0
#define U_PNIP__ACW_TROGPTR_IFA                                                   (0x1067C820 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ACW_TROGPTR_IFA__ACW_TROGPTR                                      (0x001FFFFF)
#define U_PNIP__ACW_TROGPTR_IFA_RESET__VALUE                     0x80000
#define U_PNIP__ROUTER_MAC_DA_LOW_IFA                                             (0x1067C824 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ROUTER_MAC_DA_LOW_IFA__ROUTER_MAC_DA_3_0                          (0xFFFFFFFF)
#define U_PNIP__ROUTER_MAC_DA_LOW_IFA_RESET__VALUE               0x0
#define U_PNIP__ROUTER_MAC_DA_HIGH_IFA                                            (0x1067C828 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ROUTER_MAC_DA_HIGH_IFA__ROUTER_MAC_DA_5_4                         (0x0000FFFF)
#define U_PNIP__ROUTER_MAC_DA_HIGH_IFA_RESET__VALUE              0x0
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA                                             (0x1067C82C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__MASK_RESSOURCETYPE_15                      (0x80000000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__MASK_RESSOURCETYPE_14                      (0x40000000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__MASK_RESSOURCETYPE_13                      (0x20000000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__MASK_RESSOURCETYPE_12                      (0x10000000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__MASK_RESSOURCETYPE_11                      (0x08000000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__MASK_RESSOURCETYPE_10                      (0x04000000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__MASK_RESSOURCETYPE_9                       (0x02000000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__MASK_RESSOURCETYPE_8                       (0x01000000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__MASK_RESSOURCETYPE_7                       (0x00800000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__MASK_RESSOURCETYPE_6                       (0x00400000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__MASK_RESSOURCETYPE_5                       (0x00200000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__MASK_RESSOURCETYPE_4                       (0x00100000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__MASK_RESSOURCETYPE_3                       (0x00080000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__MASK_RESSOURCETYPE_2                       (0x00040000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__MASK_RESSOURCETYPE_1                       (0x00020000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__MASK_RESSOURCETYPE_0                       (0x00010000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__RESSOURCETYPE_15                           (0x00008000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__RESSOURCETYPE_14                           (0x00004000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__RESSOURCETYPE_13                           (0x00002000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__RESSOURCETYPE_12                           (0x00001000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__RESSOURCETYPE_11                           (0x00000800)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__RESSOURCETYPE_10                           (0x00000400)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__RESSOURCETYPE_9                            (0x00000200)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__RESSOURCETYPE_8                            (0x00000100)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__RESSOURCETYPE_7                            (0x00000080)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__RESSOURCETYPE_6                            (0x00000040)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__RESSOURCETYPE_5                            (0x00000020)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__RESSOURCETYPE_4                            (0x00000010)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__RESSOURCETYPE_3                            (0x00000008)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__RESSOURCETYPE_2                            (0x00000004)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__RESSOURCETYPE_1                            (0x00000002)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA__RESSOURCETYPE_0                            (0x00000001)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFA_RESET__VALUE               0x0
#define U_PNIP__APPLDATASTATUS_IFA_0                                              (0x1067C830 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLDATASTATUS_IFA_0__APPLDATASTATUS                              (0x000000FF)
#define U_PNIP__APPLDATASTATUS_IFA_0_RESET__VALUE                0x0
#define U_PNIP__APPLDATASTATUS_IFA_1                                              (0x1067C834 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLDATASTATUS_IFA_1__APPLDATASTATUS                              (0x000000FF)
#define U_PNIP__APPLDATASTATUS_IFA_1_RESET__VALUE                0x0
#define U_PNIP__APPLDATASTATUS_IFA_2                                              (0x1067C838 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLDATASTATUS_IFA_2__APPLDATASTATUS                              (0x000000FF)
#define U_PNIP__APPLDATASTATUS_IFA_2_RESET__VALUE                0x0
#define U_PNIP__APPLDATASTATUS_IFA_3                                              (0x1067C83C + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLDATASTATUS_IFA_3__APPLDATASTATUS                              (0x000000FF)
#define U_PNIP__APPLDATASTATUS_IFA_3_RESET__VALUE                0x0
#define U_PNIP__APPLDATASTATUS_IFA_4                                              (0x1067C840 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLDATASTATUS_IFA_4__APPLDATASTATUS                              (0x000000FF)
#define U_PNIP__APPLDATASTATUS_IFA_4_RESET__VALUE                0x0
#define U_PNIP__APPLDATASTATUS_IFA_5                                              (0x1067C844 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLDATASTATUS_IFA_5__APPLDATASTATUS                              (0x000000FF)
#define U_PNIP__APPLDATASTATUS_IFA_5_RESET__VALUE                0x0
#define U_PNIP__APPLDATASTATUS_IFA_6                                              (0x1067C848 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLDATASTATUS_IFA_6__APPLDATASTATUS                              (0x000000FF)
#define U_PNIP__APPLDATASTATUS_IFA_6_RESET__VALUE                0x0
#define U_PNIP__APPLDATASTATUS_IFA_7                                              (0x1067C84C + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLDATASTATUS_IFA_7__APPLDATASTATUS                              (0x000000FF)
#define U_PNIP__APPLDATASTATUS_IFA_7_RESET__VALUE                0x0
#define U_PNIP__DATASTATUS_MASK_IFA                                               (0x1067C850 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DATASTATUS_MASK_IFA__DATASTATUS_MASK                              (0x000000FF)
#define U_PNIP__DATASTATUS_MASK_IFA_RESET__VALUE                 0x0
#define U_PNIP__DG_DATASTATUS_IFA_0                                               (0x1067C854 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DG_DATASTATUS_IFA_0__DG_DATASTATUS                                (0x000000FF)
#define U_PNIP__DG_DATASTATUS_IFA_0_RESET__VALUE                 0x0
#define U_PNIP__DG_DATASTATUS_IFA_1                                               (0x1067C858 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DG_DATASTATUS_IFA_1__DG_DATASTATUS                                (0x000000FF)
#define U_PNIP__DG_DATASTATUS_IFA_1_RESET__VALUE                 0x0
#define U_PNIP__DG_DATASTATUS_IFA_2                                               (0x1067C85C + PNIP_DEV_TOP_BASE)
#define U_PNIP__DG_DATASTATUS_IFA_2__DG_DATASTATUS                                (0x000000FF)
#define U_PNIP__DG_DATASTATUS_IFA_2_RESET__VALUE                 0x0
#define U_PNIP__DG_DATASTATUS_IFA_3                                               (0x1067C860 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DG_DATASTATUS_IFA_3__DG_DATASTATUS                                (0x000000FF)
#define U_PNIP__DG_DATASTATUS_IFA_3_RESET__VALUE                 0x0
#define U_PNIP__DG_DATASTATUS_IFA_4                                               (0x1067C864 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DG_DATASTATUS_IFA_4__DG_DATASTATUS                                (0x000000FF)
#define U_PNIP__DG_DATASTATUS_IFA_4_RESET__VALUE                 0x0
#define U_PNIP__DG_DATASTATUS_IFA_5                                               (0x1067C868 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DG_DATASTATUS_IFA_5__DG_DATASTATUS                                (0x000000FF)
#define U_PNIP__DG_DATASTATUS_IFA_5_RESET__VALUE                 0x0
#define U_PNIP__DG_DATASTATUS_IFA_6                                               (0x1067C86C + PNIP_DEV_TOP_BASE)
#define U_PNIP__DG_DATASTATUS_IFA_6__DG_DATASTATUS                                (0x000000FF)
#define U_PNIP__DG_DATASTATUS_IFA_6_RESET__VALUE                 0x0
#define U_PNIP__DG_DATASTATUS_IFA_7                                               (0x1067C870 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DG_DATASTATUS_IFA_7__DG_DATASTATUS                                (0x000000FF)
#define U_PNIP__DG_DATASTATUS_IFA_7_RESET__VALUE                 0x0
#define U_PNIP__DG_DATASTATUS_MASK_IFA                                            (0x1067C874 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DG_DATASTATUS_MASK_IFA__DG_DATASTATUS_MASK                        (0x000000FF)
#define U_PNIP__DG_DATASTATUS_MASK_IFA_RESET__VALUE              0x0
#define U_PNIP__DATASTATUS_BASE_IFA                                               (0x1067C878 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DATASTATUS_BASE_IFA__DS_BASEPTR                                   (0x001FFFFF)
#define U_PNIP__DATASTATUS_BASE_IFA_RESET__VALUE                 0x80000
#define U_PNIP__PPM_CONTROL_IFB                                                   (0x1067CC00 + PNIP_DEV_TOP_BASE)
#define U_PNIP__PPM_CONTROL_IFB__PPM_ENABLE                                       (0x80000000)
#define U_PNIP__PPM_CONTROL_IFB_RESET__VALUE                     0x0
#define U_PNIP__ACW_TBASEPTR_IFB                                                  (0x1067CC04 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ACW_TBASEPTR_IFB__ACW_TBASEPTR                                    (0x001FFFFF)
#define U_PNIP__ACW_TBASEPTR_IFB_RESET__VALUE                    0x80000
#define U_PNIP__ACW_TREDUCTIONSELECT_IFB                                          (0x1067CC08 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ACW_TREDUCTIONSELECT_IFB__ACW_TREDUCTIONSELECT                    (0x00007FFF)
#define U_PNIP__ACW_TREDUCTIONSELECT_IFB_RESET__VALUE            0x0
#define U_PNIP__ACW_T_PO1K_IFB                                                    (0x1067CC0C + PNIP_DEV_TOP_BASE)
#define U_PNIP__ACW_T_PO1K_IFB__PHASE_OFFSET                                      (0x0000000F)
#define U_PNIP__ACW_T_PO1K_IFB_RESET__VALUE                      0x0
#define U_PNIP__ACW_T_PO2K_IFB                                                    (0x1067CC10 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ACW_T_PO2K_IFB__PHASE_OFFSET                                      (0x0000001F)
#define U_PNIP__ACW_T_PO2K_IFB_RESET__VALUE                      0x0
#define U_PNIP__ACW_T_PO4K_IFB                                                    (0x1067CC14 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ACW_T_PO4K_IFB__PHASE_OFFSET                                      (0x0000003F)
#define U_PNIP__ACW_T_PO4K_IFB_RESET__VALUE                      0x0
#define U_PNIP__ACW_T_PO8K_IFB                                                    (0x1067CC18 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ACW_T_PO8K_IFB__PHASE_OFFSET                                      (0x0000007F)
#define U_PNIP__ACW_T_PO8K_IFB_RESET__VALUE                      0x0
#define U_PNIP__ACW_T_PO16K_IFB                                                   (0x1067CC1C + PNIP_DEV_TOP_BASE)
#define U_PNIP__ACW_T_PO16K_IFB__PHASE_OFFSET                                     (0x000000FF)
#define U_PNIP__ACW_T_PO16K_IFB_RESET__VALUE                     0x0
#define U_PNIP__ACW_TROGPTR_IFB                                                   (0x1067CC20 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ACW_TROGPTR_IFB__ACW_TROGPTR                                      (0x001FFFFF)
#define U_PNIP__ACW_TROGPTR_IFB_RESET__VALUE                     0x80000
#define U_PNIP__ROUTER_MAC_DA_LOW_IFB                                             (0x1067CC24 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ROUTER_MAC_DA_LOW_IFB__ROUTER_MAC_DA_3_0                          (0xFFFFFFFF)
#define U_PNIP__ROUTER_MAC_DA_LOW_IFB_RESET__VALUE               0x0
#define U_PNIP__ROUTER_MAC_DA_HIGH_IFB                                            (0x1067CC28 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ROUTER_MAC_DA_HIGH_IFB__ROUTER_MAC_DA_5_4                         (0x0000FFFF)
#define U_PNIP__ROUTER_MAC_DA_HIGH_IFB_RESET__VALUE              0x0
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB                                             (0x1067CC2C + PNIP_DEV_TOP_BASE)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__MASK_RESSOURCETYPE_15                      (0x80000000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__MASK_RESSOURCETYPE_14                      (0x40000000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__MASK_RESSOURCETYPE_13                      (0x20000000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__MASK_RESSOURCETYPE_12                      (0x10000000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__MASK_RESSOURCETYPE_11                      (0x08000000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__MASK_RESSOURCETYPE_10                      (0x04000000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__MASK_RESSOURCETYPE_9                       (0x02000000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__MASK_RESSOURCETYPE_8                       (0x01000000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__MASK_RESSOURCETYPE_7                       (0x00800000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__MASK_RESSOURCETYPE_6                       (0x00400000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__MASK_RESSOURCETYPE_5                       (0x00200000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__MASK_RESSOURCETYPE_4                       (0x00100000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__MASK_RESSOURCETYPE_3                       (0x00080000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__MASK_RESSOURCETYPE_2                       (0x00040000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__MASK_RESSOURCETYPE_1                       (0x00020000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__MASK_RESSOURCETYPE_0                       (0x00010000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__RESSOURCETYPE_15                           (0x00008000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__RESSOURCETYPE_14                           (0x00004000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__RESSOURCETYPE_13                           (0x00002000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__RESSOURCETYPE_12                           (0x00001000)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__RESSOURCETYPE_11                           (0x00000800)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__RESSOURCETYPE_10                           (0x00000400)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__RESSOURCETYPE_9                            (0x00000200)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__RESSOURCETYPE_8                            (0x00000100)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__RESSOURCETYPE_7                            (0x00000080)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__RESSOURCETYPE_6                            (0x00000040)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__RESSOURCETYPE_5                            (0x00000020)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__RESSOURCETYPE_4                            (0x00000010)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__RESSOURCETYPE_3                            (0x00000008)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__RESSOURCETYPE_2                            (0x00000004)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__RESSOURCETYPE_1                            (0x00000002)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB__RESSOURCETYPE_0                            (0x00000001)
#define U_PNIP__PPM_HOL_NOT_EMPTY_IFB_RESET__VALUE               0x0
#define U_PNIP__APPLDATASTATUS_IFB_0                                              (0x1067CC30 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLDATASTATUS_IFB_0__APPLDATASTATUS                              (0x000000FF)
#define U_PNIP__APPLDATASTATUS_IFB_0_RESET__VALUE                0x0
#define U_PNIP__APPLDATASTATUS_IFB_1                                              (0x1067CC34 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLDATASTATUS_IFB_1__APPLDATASTATUS                              (0x000000FF)
#define U_PNIP__APPLDATASTATUS_IFB_1_RESET__VALUE                0x0
#define U_PNIP__APPLDATASTATUS_IFB_2                                              (0x1067CC38 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLDATASTATUS_IFB_2__APPLDATASTATUS                              (0x000000FF)
#define U_PNIP__APPLDATASTATUS_IFB_2_RESET__VALUE                0x0
#define U_PNIP__APPLDATASTATUS_IFB_3                                              (0x1067CC3C + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLDATASTATUS_IFB_3__APPLDATASTATUS                              (0x000000FF)
#define U_PNIP__APPLDATASTATUS_IFB_3_RESET__VALUE                0x0
#define U_PNIP__APPLDATASTATUS_IFB_4                                              (0x1067CC40 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLDATASTATUS_IFB_4__APPLDATASTATUS                              (0x000000FF)
#define U_PNIP__APPLDATASTATUS_IFB_4_RESET__VALUE                0x0
#define U_PNIP__APPLDATASTATUS_IFB_5                                              (0x1067CC44 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLDATASTATUS_IFB_5__APPLDATASTATUS                              (0x000000FF)
#define U_PNIP__APPLDATASTATUS_IFB_5_RESET__VALUE                0x0
#define U_PNIP__APPLDATASTATUS_IFB_6                                              (0x1067CC48 + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLDATASTATUS_IFB_6__APPLDATASTATUS                              (0x000000FF)
#define U_PNIP__APPLDATASTATUS_IFB_6_RESET__VALUE                0x0
#define U_PNIP__APPLDATASTATUS_IFB_7                                              (0x1067CC4C + PNIP_DEV_TOP_BASE)
#define U_PNIP__APPLDATASTATUS_IFB_7__APPLDATASTATUS                              (0x000000FF)
#define U_PNIP__APPLDATASTATUS_IFB_7_RESET__VALUE                0x0
#define U_PNIP__DATASTATUS_MASK_IFB                                               (0x1067CC50 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DATASTATUS_MASK_IFB__DATASTATUS_MASK                              (0x000000FF)
#define U_PNIP__DATASTATUS_MASK_IFB_RESET__VALUE                 0x0
#define U_PNIP__DG_DATASTATUS_IFB_0                                               (0x1067CC54 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DG_DATASTATUS_IFB_0__DG_DATASTATUS                                (0x000000FF)
#define U_PNIP__DG_DATASTATUS_IFB_0_RESET__VALUE                 0x0
#define U_PNIP__DG_DATASTATUS_IFB_1                                               (0x1067CC58 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DG_DATASTATUS_IFB_1__DG_DATASTATUS                                (0x000000FF)
#define U_PNIP__DG_DATASTATUS_IFB_1_RESET__VALUE                 0x0
#define U_PNIP__DG_DATASTATUS_IFB_2                                               (0x1067CC5C + PNIP_DEV_TOP_BASE)
#define U_PNIP__DG_DATASTATUS_IFB_2__DG_DATASTATUS                                (0x000000FF)
#define U_PNIP__DG_DATASTATUS_IFB_2_RESET__VALUE                 0x0
#define U_PNIP__DG_DATASTATUS_IFB_3                                               (0x1067CC60 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DG_DATASTATUS_IFB_3__DG_DATASTATUS                                (0x000000FF)
#define U_PNIP__DG_DATASTATUS_IFB_3_RESET__VALUE                 0x0
#define U_PNIP__DG_DATASTATUS_IFB_4                                               (0x1067CC64 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DG_DATASTATUS_IFB_4__DG_DATASTATUS                                (0x000000FF)
#define U_PNIP__DG_DATASTATUS_IFB_4_RESET__VALUE                 0x0
#define U_PNIP__DG_DATASTATUS_IFB_5                                               (0x1067CC68 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DG_DATASTATUS_IFB_5__DG_DATASTATUS                                (0x000000FF)
#define U_PNIP__DG_DATASTATUS_IFB_5_RESET__VALUE                 0x0
#define U_PNIP__DG_DATASTATUS_IFB_6                                               (0x1067CC6C + PNIP_DEV_TOP_BASE)
#define U_PNIP__DG_DATASTATUS_IFB_6__DG_DATASTATUS                                (0x000000FF)
#define U_PNIP__DG_DATASTATUS_IFB_6_RESET__VALUE                 0x0
#define U_PNIP__DG_DATASTATUS_IFB_7                                               (0x1067CC70 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DG_DATASTATUS_IFB_7__DG_DATASTATUS                                (0x000000FF)
#define U_PNIP__DG_DATASTATUS_IFB_7_RESET__VALUE                 0x0
#define U_PNIP__DG_DATASTATUS_MASK_IFB                                            (0x1067CC74 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DG_DATASTATUS_MASK_IFB__DG_DATASTATUS_MASK                        (0x000000FF)
#define U_PNIP__DG_DATASTATUS_MASK_IFB_RESET__VALUE              0x0
#define U_PNIP__DATASTATUS_BASE_IFB                                               (0x1067CC78 + PNIP_DEV_TOP_BASE)
#define U_PNIP__DATASTATUS_BASE_IFB__DS_BASEPTR                                   (0x001FFFFF)
#define U_PNIP__DATASTATUS_BASE_IFB_RESET__VALUE                 0x80000
#define U_PNIP__CPMCOMMAND_IF_CONTROL                                             (0x1067D000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CPMCOMMAND_IF_CONTROL__COMMANDVALUE                               (0xFFFFFF00)
#define U_PNIP__CPMCOMMAND_IF_CONTROL__USER_ID                                    (0x000000E0)
#define U_PNIP__CPMCOMMAND_IF_CONTROL__CONFREQUEST                                (0x00000010)
#define U_PNIP__CPMCOMMAND_IF_CONTROL__IF_SELECT                                  (0x00000008)
#define U_PNIP__CPMCOMMAND_IF_CONTROL__F_CODE                                     (0x00000007)
#define U_PNIP__CPMCOMMAND_IF_CONTROL_RESET__VALUE               0x0
#define U_PNIP__CPMCOMMAND_IF_STATUS                                              (0x1067D004 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CPMCOMMAND_IF_STATUS__RETURNVALUE                                 (0xFFFFFF00)
#define U_PNIP__CPMCOMMAND_IF_STATUS__USER_ID                                     (0x000000E0)
#define U_PNIP__CPMCOMMAND_IF_STATUS__CONFRESPONSE                                (0x00000010)
#define U_PNIP__CPMCOMMAND_IF_STATUS__CONFERROR                                   (0x00000008)
#define U_PNIP__CPMCOMMAND_IF_STATUS__F_CODE                                      (0x00000007)
#define U_PNIP__CPMCOMMAND_IF_STATUS_RESET__VALUE                0x0
#define U_PNIP__CPMERRSTATUS                                                      (0x1067D008 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CPMERRSTATUS__CPM_ERR_INTERFACE                                   (0x00080000)
#define U_PNIP__CPMERRSTATUS__INT_CPM_ERR                                         (0x00070000)
#define U_PNIP__CPMERRSTATUS__INT_CPM_DMA_ERR_G3                                  (0x00000400)
#define U_PNIP__CPMERRSTATUS__INT_CPM_DMA_ERR_G2                                  (0x00000200)
#define U_PNIP__CPMERRSTATUS__INT_CPM_DMA_ERR_G1                                  (0x00000100)
#define U_PNIP__CPMERRSTATUS_RESET__VALUE                        0x0
#define U_PNIP__CPMERRADR                                                         (0x1067D00C + PNIP_DEV_TOP_BASE)
#define U_PNIP__CPMERRADR__CPMERRADR                                              (0xFFFFFFFF)
#define U_PNIP__CPMERRADR_RESET__VALUE                           0x0
#define U_PNIP__FRAMECOUNT_G1                                                     (0x1067D400 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECOUNT_G1__FRAMECOUNTER_CTRL                                  (0x01000000)
#define U_PNIP__FRAMECOUNT_G1__FRAMECOUNTER                                       (0x000FFC00)
#define U_PNIP__FRAMECOUNT_G1__FRAMES_RECEIVED                                    (0x000003FF)
#define U_PNIP__FRAMECOUNT_G1_RESET__VALUE                       0x0
#define U_PNIP__FRAMECOUNT_G2                                                     (0x1067D404 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECOUNT_G2__FRAMECOUNTER_CTRL                                  (0x03000000)
#define U_PNIP__FRAMECOUNT_G2__FRAMECOUNTER                                       (0x000FFC00)
#define U_PNIP__FRAMECOUNT_G2__FRAMES_RECEIVED                                    (0x000003FF)
#define U_PNIP__FRAMECOUNT_G2_RESET__VALUE                       0x0
#define U_PNIP__FRAMECOUNT_G3                                                     (0x1067D408 + PNIP_DEV_TOP_BASE)
#define U_PNIP__FRAMECOUNT_G3__FRAMECOUNTER_CTRL                                  (0x03000000)
#define U_PNIP__FRAMECOUNT_G3__FRAMECOUNTER                                       (0x000FFC00)
#define U_PNIP__FRAMECOUNT_G3__FRAMES_RECEIVED                                    (0x000003FF)
#define U_PNIP__FRAMECOUNT_G3_RESET__VALUE                       0x0
#define U_PNIP__CPMGROUPCYCLE_G1                                                  (0x1067D40C + PNIP_DEV_TOP_BASE)
#define U_PNIP__CPMGROUPCYCLE_G1__REDUCTION                                       (0xF0000000)
#define U_PNIP__CPMGROUPCYCLE_G1__INTERFACE                                       (0x08000000)
#define U_PNIP__CPMGROUPCYCLE_G1__GROUP_TIMER                                     (0x07FFFFF8)
#define U_PNIP__CPMGROUPCYCLE_G1_RESET__VALUE                    0x0
#define U_PNIP__CPMGROUPCYCLE_G2                                                  (0x1067D410 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CPMGROUPCYCLE_G2__REDUCTION                                       (0xF0000000)
#define U_PNIP__CPMGROUPCYCLE_G2__INTERFACE                                       (0x08000000)
#define U_PNIP__CPMGROUPCYCLE_G2__GROUP_TIMER                                     (0x07FFFFF8)
#define U_PNIP__CPMGROUPCYCLE_G2_RESET__VALUE                    0x0
#define U_PNIP__CPMGROUPCYCLE_G3                                                  (0x1067D414 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CPMGROUPCYCLE_G3__REDUCTION                                       (0xF0000000)
#define U_PNIP__CPMGROUPCYCLE_G3__INTERFACE                                       (0x08000000)
#define U_PNIP__CPMGROUPCYCLE_G3__GROUP_TIMER                                     (0x07FFFFF8)
#define U_PNIP__CPMGROUPCYCLE_G3_RESET__VALUE                    0x0
#define U_PNIP__CPMTRANSFEREND_G1                                                 (0x1067D418 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CPMTRANSFEREND_G1__TRANSFEREND                                    (0x00000001)
#define U_PNIP__CPMTRANSFEREND_G1_RESET__VALUE                   0x0
#define U_PNIP__CPMTRANSFEREND_G2                                                 (0x1067D41C + PNIP_DEV_TOP_BASE)
#define U_PNIP__CPMTRANSFEREND_G2__TRANSFEREND                                    (0x00000001)
#define U_PNIP__CPMTRANSFEREND_G2_RESET__VALUE                   0x0
#define U_PNIP__CPMTRANSFEREND_G3                                                 (0x1067D420 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CPMTRANSFEREND_G3__TRANSFEREND                                    (0x00000001)
#define U_PNIP__CPMTRANSFEREND_G3_RESET__VALUE                   0x0
#define U_PNIP__CPM_OUTDATASTART                                                  (0x1067D424 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CPM_OUTDATASTART__OUTDATASTART                                    (0x000007FF)
#define U_PNIP__CPM_OUTDATASTART_RESET__VALUE                    0x0
#define U_PNIP__CPM_OUTDATACOUNT                                                  (0x1067D428 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CPM_OUTDATACOUNT__OUTDATACOUNT                                    (0x000007FF)
#define U_PNIP__CPM_OUTDATACOUNT_RESET__VALUE                    0x0
#define U_PNIP__CPM_AHB_LOCKOUT_STARTTIME                                         (0x1067D42C + PNIP_DEV_TOP_BASE)
#define U_PNIP__CPM_AHB_LOCKOUT_STARTTIME__CPM_AHB_LOCKOUT_STARTED                (0xC0000000)
#define U_PNIP__CPM_AHB_LOCKOUT_STARTTIME__CPM_AHB_LOCKOUT_STARTTIME              (0x07FFFFF8)
#define U_PNIP__CPM_AHB_LOCKOUT_STARTTIME_RESET__VALUE           0x0
#define U_PNIP__CPM_AHB_LOCKOUT_STOPTIME                                          (0x1067D430 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CPM_AHB_LOCKOUT_STOPTIME__CPM_AHB_LOCKOUT_STOPED                  (0xC0000000)
#define U_PNIP__CPM_AHB_LOCKOUT_STOPTIME__CPM_AHB_LOCKOUT_STOPTIME                (0x07FFFFF8)
#define U_PNIP__CPM_AHB_LOCKOUT_STOPTIME_RESET__VALUE            0x0
#define U_PNIP__CPM_AHB_LOCKOUT_CONTROL                                           (0x1067D434 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CPM_AHB_LOCKOUT_CONTROL__CPM_AHB_LOCKOUT_INTERFACE                (0x80000000)
#define U_PNIP__CPM_AHB_LOCKOUT_CONTROL__CPM_AHB_LOCKOUT_PHASE                    (0x3FFF0000)
#define U_PNIP__CPM_AHB_LOCKOUT_CONTROL__CPM_AHB_LOCKOUT_WITHACW                  (0x00000200)
#define U_PNIP__CPM_AHB_LOCKOUT_CONTROL__CPM_AHB_LOCKOUT_WITHTIME                 (0x00000100)
#define U_PNIP__CPM_AHB_LOCKOUT_CONTROL__CPM_AHB_LOCKOUT_REDUCTION                (0x0000000F)
#define U_PNIP__CPM_AHB_LOCKOUT_CONTROL_RESET__VALUE             0x0
#define U_PNIP__CPM_CONTROL_IFA                                                   (0x1067D800 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CPM_CONTROL_IFA__CPM_ENABLE                                       (0x80000000)
#define U_PNIP__CPM_CONTROL_IFA_RESET__VALUE                     0x0
#define U_PNIP__ACW_RBASEPTR_IFA                                                  (0x1067D804 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ACW_RBASEPTR_IFA__ACW_RBASEPTR                                    (0x001FFFFF)
#define U_PNIP__ACW_RBASEPTR_IFA_RESET__VALUE                    0x80000
#define U_PNIP__ACW_RINDEXSIZE_IFA                                                (0x1067D808 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ACW_RINDEXSIZE_IFA__ACW_RINDEXSIZE                                (0x0000000F)
#define U_PNIP__ACW_RINDEXSIZE_IFA_RESET__VALUE                  0x0
#define U_PNIP__TSB_IFA                                                           (0x1067D80C + PNIP_DEV_TOP_BASE)
#define U_PNIP__TSB_IFA__TIMER_SCOREBOARD_ENTRIES                                 (0xFFE00000)
#define U_PNIP__TSB_IFA__TIMER_SCOREBOARD_BASEPTR                                 (0x001FFFFF)
#define U_PNIP__TSB_IFA_RESET__VALUE                             0x80000
#define U_PNIP__CSB_0_IFA                                                         (0x1067D810 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CSB_0_IFA__CONSUMER_SCOREBOARD_BASEPTR                            (0x001FFFFF)
#define U_PNIP__CSB_0_IFA_RESET__VALUE                           0x80000
#define U_PNIP__CSB_1_IFA                                                         (0x1067D814 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CSB_1_IFA__CONSUMER_SCOREBOARD_BASEPTR                            (0x001FFFFF)
#define U_PNIP__CSB_1_IFA_RESET__VALUE                           0x80000
#define U_PNIP__CSB_CONTROL_IFA                                                   (0x1067D818 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CSB_CONTROL_IFA__CSB_NEW_ENTRY                                    (0x80000000)
#define U_PNIP__CSB_CONTROL_IFA__CSB_SELECT                                       (0x00000002)
#define U_PNIP__CSB_CONTROL_IFA__CSB_CHANGE_ENABLE                                (0x00000001)
#define U_PNIP__CSB_CONTROL_IFA_RESET__VALUE                     0x0
#define U_PNIP__CPM_CONTROL_IFB                                                   (0x1067DC00 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CPM_CONTROL_IFB__CPM_ENABLE                                       (0x80000000)
#define U_PNIP__CPM_CONTROL_IFB_RESET__VALUE                     0x0
#define U_PNIP__ACW_RBASEPTR_IFB                                                  (0x1067DC04 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ACW_RBASEPTR_IFB__ACW_RBASEPTR                                    (0x001FFFFF)
#define U_PNIP__ACW_RBASEPTR_IFB_RESET__VALUE                    0x80000
#define U_PNIP__ACW_RINDEXSIZE_IFB                                                (0x1067DC08 + PNIP_DEV_TOP_BASE)
#define U_PNIP__ACW_RINDEXSIZE_IFB__ACW_RINDEXSIZE                                (0x0000000F)
#define U_PNIP__ACW_RINDEXSIZE_IFB_RESET__VALUE                  0x0
#define U_PNIP__TSB_IFB                                                           (0x1067DC0C + PNIP_DEV_TOP_BASE)
#define U_PNIP__TSB_IFB__TIMER_SCOREBOARD_ENTRIES                                 (0xFFE00000)
#define U_PNIP__TSB_IFB__TIMER_SCOREBOARD_BASEPTR                                 (0x001FFFFF)
#define U_PNIP__TSB_IFB_RESET__VALUE                             0x80000
#define U_PNIP__CSB_0_IFB                                                         (0x1067DC10 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CSB_0_IFB__CONSUMER_SCOREBOARD_BASEPTR                            (0x001FFFFF)
#define U_PNIP__CSB_0_IFB_RESET__VALUE                           0x80000
#define U_PNIP__CSB_1_IFB                                                         (0x1067DC14 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CSB_1_IFB__CONSUMER_SCOREBOARD_BASEPTR                            (0x001FFFFF)
#define U_PNIP__CSB_1_IFB_RESET__VALUE                           0x80000
#define U_PNIP__CSB_CONTROL_IFB                                                   (0x1067DC18 + PNIP_DEV_TOP_BASE)
#define U_PNIP__CSB_CONTROL_IFB__CSB_NEW_ENTRY                                    (0x80000000)
#define U_PNIP__CSB_CONTROL_IFB__CSB_SELECT                                       (0x00000002)
#define U_PNIP__CSB_CONTROL_IFB__CSB_CHANGE_ENABLE                                (0x00000001)
#define U_PNIP__CSB_CONTROL_IFB_RESET__VALUE                     0x0
#define U_PERIF_AHB__COMMAND_IF_CONTROL_APP                                       (0x10800000 + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__COMMAND_IF_CONTROL_APP__CR_NUMBER                            (0x0000001F)
#define U_PERIF_AHB__COMMAND_IF_CONTROL_APP__CONFREQUEST                          (0x00000020)
#define U_PERIF_AHB__COMMAND_IF_CONTROL_APP__F_CODE                               (0x000000C0)
#define U_PERIF_AHB__COMMAND_IF_CONTROL_APP__USER_ID                              (0x0000E000)
#define U_PERIF_AHB__COMMAND_IF_CONTROL_APP_RESET__VALUE         0x0000
#define U_PERIF_AHB__COMMAND_IF_STATUS_APP                                        (0x10800004 + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__COMMAND_IF_STATUS_APP__CR_NUMBER                             (0x0000001F)
#define U_PERIF_AHB__COMMAND_IF_STATUS_APP__CONFRESPONSE                          (0x00000020)
#define U_PERIF_AHB__COMMAND_IF_STATUS_APP__F_CODE                                (0x000000C0)
#define U_PERIF_AHB__COMMAND_IF_STATUS_APP__CONFVALUE                             (0x00000400)
#define U_PERIF_AHB__COMMAND_IF_STATUS_APP__USER_ID                               (0x0000E000)
#define U_PERIF_AHB__COMMAND_IF_STATUS_APP_RESET__VALUE          0x0000
#define U_PERIF_AHB__HOST_IRQ_LOW                                                 (0x10800008 + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__HOST_IRQ_LOW__IRQ_BITS                                       (0xFFFFFFFF)
#define U_PERIF_AHB__HOST_IRQ_LOW_RESET__VALUE                   0x00000000
#define U_PERIF_AHB__HOST_IRQ_HIGH                                                (0x1080000C + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__HOST_IRQ_HIGH__IRQ_BITS                                      (0xFFFFFFFF)
#define U_PERIF_AHB__HOST_IRQ_HIGH_RESET__VALUE                  0x00000000
#define U_PERIF_AHB__HOST_IRQMASK_LOW                                             (0x10800010 + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__HOST_IRQMASK_LOW__MASK_BITS                                  (0xFFFFFFFF)
#define U_PERIF_AHB__HOST_IRQMASK_LOW_RESET__VALUE               0xFFFFFFFF
#define U_PERIF_AHB__HOST_IRQMASK_HIGH                                            (0x10800014 + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__HOST_IRQMASK_HIGH__MASK_BITS                                 (0xFFFFFFFF)
#define U_PERIF_AHB__HOST_IRQMASK_HIGH_RESET__VALUE              0xFFFFFFFF
#define U_PERIF_AHB__HOST_EVENT_LOW                                               (0x10800018 + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__HOST_EVENT_LOW__EVENT_BITS                                   (0xFFFFFFFF)
#define U_PERIF_AHB__HOST_EVENT_LOW_RESET__VALUE                 0x00000000
#define U_PERIF_AHB__HOST_EVENT_HIGH                                              (0x1080001C + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__HOST_EVENT_HIGH__EVENT_BITS                                  (0xFFFFFFFF)
#define U_PERIF_AHB__HOST_EVENT_HIGH_RESET__VALUE                0x00000000
#define U_PERIF_AHB__HOST_IRQACK_LOW                                              (0x10800020 + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__HOST_IRQACK_LOW__ACK_BITS                                    (0xFFFFFFFF)
#define U_PERIF_AHB__HOST_IRQACK_LOW_RESET__VALUE                0x00000000
#define U_PERIF_AHB__HOST_IRQACK_HIGH                                             (0x10800024 + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__HOST_IRQACK_HIGH__ACK_BITS                                   (0xFFFFFFFF)
#define U_PERIF_AHB__HOST_IRQACK_HIGH_RESET__VALUE               0x00000000
#define U_PERIF_AHB__HOST_EOI                                                     (0x10800028 + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__HOST_EOI__WAIT_TIME                                          (0x0003FFFF)
#define U_PERIF_AHB__HOST_EOI_RESET__VALUE                       0x00000
#define U_PERIF_AHB__PN_IRQ_LOW                                                   (0x1080002C + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__PN_IRQ_LOW__IRQ_BITS                                         (0xFFFFFFFF)
#define U_PERIF_AHB__PN_IRQ_LOW_RESET__VALUE                     0x00000000
#define U_PERIF_AHB__PN_IRQ_HIGH                                                  (0x10800030 + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__PN_IRQ_HIGH__IRQ_BITS                                        (0xFFFFFFFF)
#define U_PERIF_AHB__PN_IRQ_HIGH_RESET__VALUE                    0x00000000
#define U_PERIF_AHB__PN_IRQMASK_LOW                                               (0x10800034 + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__PN_IRQMASK_LOW__MASK_BITS                                    (0xFFFFFFFF)
#define U_PERIF_AHB__PN_IRQMASK_LOW_RESET__VALUE                 0xFFFFFFFF
#define U_PERIF_AHB__PN_IRQMASK_HIGH                                              (0x10800038 + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__PN_IRQMASK_HIGH__MASK_BITS                                   (0xFFFFFFFF)
#define U_PERIF_AHB__PN_IRQMASK_HIGH_RESET__VALUE                0xFFFFFFFF
#define U_PERIF_AHB__PN_EVENT_LOW                                                 (0x1080003C + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__PN_EVENT_LOW__EVENT_BITS                                     (0xFFFFFFFF)
#define U_PERIF_AHB__PN_EVENT_LOW_RESET__VALUE                   0x00000000
#define U_PERIF_AHB__PN_EVENT_HIGH                                                (0x10800040 + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__PN_EVENT_HIGH__EVENT_BITS                                    (0xFFFFFFFF)
#define U_PERIF_AHB__PN_EVENT_HIGH_RESET__VALUE                  0x00000000
#define U_PERIF_AHB__PN_IRQACK_LOW                                                (0x10800044 + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__PN_IRQACK_LOW__ACK_BITS                                      (0xFFFFFFFF)
#define U_PERIF_AHB__PN_IRQACK_LOW_RESET__VALUE                  0x00000000
#define U_PERIF_AHB__PN_IRQACK_HIGH                                               (0x10800048 + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__PN_IRQACK_HIGH__ACK_BITS                                     (0xFFFFFFFF)
#define U_PERIF_AHB__PN_IRQACK_HIGH_RESET__VALUE                 0x00000000
#define U_PERIF_AHB__PN_EOI                                                       (0x1080004C + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__PN_EOI__WAIT_TIME                                            (0x0003FFFF)
#define U_PERIF_AHB__PN_EOI_RESET__VALUE                         0x00000

#define U_GDMA__GDMA_REG_ADDR                                                     (0x10A00000 + PNIP_DEV_TOP_BASE)
#define U_GDMA__GDMA_REG_ADDR__REG_ADDR                                           (0xFFFFFFF0)
#define U_GDMA__GDMA_REG_ADDR_RESET__VALUE                       0x00000000
#define U_GDMA__GDMA_LIST_ADDR                                                    (0x10A00004 + PNIP_DEV_TOP_BASE)
#define U_GDMA__GDMA_LIST_ADDR__LIST_ADDR                                         (0xFFFFFFF0)
#define U_GDMA__GDMA_LIST_ADDR_RESET__VALUE                      0x00000000
#define U_GDMA__GDMA_MAIN_CTRL                                                    (0x10A00008 + PNIP_DEV_TOP_BASE)
#define U_GDMA__GDMA_MAIN_CTRL__DMA_EN                                            (0x00000001)
#define U_GDMA__GDMA_MAIN_CTRL__SW_RESET                                          (0x00000002)
#define U_GDMA__GDMA_MAIN_CTRL__JC_RESET                                          (0x00008000)
#define U_GDMA__GDMA_MAIN_CTRL__ERR_INT_EN                                        (0x00010000)
#define U_GDMA__GDMA_MAIN_CTRL__LIST_SIZE                                         (0xFF000000)
#define U_GDMA__GDMA_MAIN_CTRL_RESET__VALUE                      0x00000000
#define U_GDMA__GDMA_JC_EN                                                        (0x10A0000C + PNIP_DEV_TOP_BASE)
#define U_GDMA__GDMA_JC_EN_RESET__VALUE                          0x00000000
#define U_GDMA__GDMA_JOB0_CTRL                                                    (0x10A00010 + PNIP_DEV_TOP_BASE)
#define U_GDMA__GDMA_JOB0_CTRL__SW_JOB_START                                      (0x00000001)
#define U_GDMA__GDMA_JOB0_CTRL__JOB_EN                                            (0x00000002)
#define U_GDMA__GDMA_JOB0_CTRL__HW_JOB_START_EN                                   (0x00000004)
#define U_GDMA__GDMA_JOB0_CTRL__HW_FLOW_EN                                        (0x00000008)
#define U_GDMA__GDMA_JOB0_CTRL__INTR_EN                                           (0x00000010)
#define U_GDMA__GDMA_JOB0_CTRL__JOB_RESET                                         (0x00000020)
#define U_GDMA__GDMA_JOB0_CTRL__HW_SELECT                                         (0x00003F00)
#define U_GDMA__GDMA_JOB0_CTRL__JOB_PRIO                                          (0x001F0000)
#define U_GDMA__GDMA_JOB0_CTRL__TRANSFER_PTR                                      (0xFF000000)
#define U_GDMA__GDMA_JOB0_CTRL_RESET__VALUE                      0x00000000
#define U_GDMA__GDMA_JOB31_CTRL                                                   (0x10A0008C + PNIP_DEV_TOP_BASE)
#define U_GDMA__GDMA_JOB31_CTRL__SW_JOB_START                                     (0x00000001)


#define U_GDMA__GDMA_JOB_STATUS                                                   (0x10A00090 + PNIP_DEV_TOP_BASE)
#define U_GDMA__GDMA_JOB_STATUS_RESET__VALUE                     0x00000000
#define U_GDMA__GDMA_FINISHED_JOBS                                                (0x10A00094 + PNIP_DEV_TOP_BASE)
#define U_GDMA__GDMA_FINISHED_JOBS_RESET__VALUE                  0x00000000
#define U_GDMA__GDMA_ACTUAL_STATUS                                                (0x10A00098 + PNIP_DEV_TOP_BASE)
#define U_GDMA__GDMA_ACTUAL_STATUS__ACT_JOB                                       (0x0000001F)
#define U_GDMA__GDMA_ACTUAL_STATUS__ACT_JOB_VAL                                   (0x00000020)
#define U_GDMA__GDMA_ACTUAL_STATUS_RESET__VALUE                  0x0000001F
#define U_GDMA__GDMA_IRQ_STATUS                                                   (0x10A0009C + PNIP_DEV_TOP_BASE)
#define U_GDMA__GDMA_IRQ_STATUS_RESET__VALUE                     0x00000000
#define U_GDMA__GDMA_ERR_IRQ_STATUS                                               (0x10A000A0 + PNIP_DEV_TOP_BASE)
#define U_GDMA__GDMA_ERR_IRQ_STATUS__ERR_DST_ADDR                                 (0x00000001)
#define U_GDMA__GDMA_ERR_IRQ_STATUS__ERR_DST_ADDR_JOB_NR                          (0x0000003E)
#define U_GDMA__GDMA_ERR_IRQ_STATUS__ERR_AHB                                      (0x00000040)
#define U_GDMA__GDMA_ERR_IRQ_STATUS__ERR_AHB_JOB_NR                               (0x00000F80)
#define U_GDMA__GDMA_ERR_IRQ_STATUS__ERR_JOB_START                                (0x00001000)
#define U_GDMA__GDMA_ERR_IRQ_STATUS__ERR_JOB_START_JOB_NR                         (0x0003E000)
#define U_GDMA__GDMA_ERR_IRQ_STATUS__ERR_AHB_SLV_WRITE                            (0x00040000)
#define U_GDMA__GDMA_ERR_IRQ_STATUS__ERR_AM_HOLD                                  (0x00080000)
#define U_GDMA__GDMA_ERR_IRQ_STATUS__ERR_AM_HOLD_JOB_NR                           (0x01F00000)
#define U_GDMA__GDMA_ERR_IRQ_STATUS_RESET__VALUE                 0x00000000
#define U_GDMA__GDMA_JOB_COUNT                                                    (0x10A000A4 + PNIP_DEV_TOP_BASE)
#define U_GDMA__GDMA_JOB_COUNT_RESET__VALUE                      0x00000000
#define U_GDMA__REVISION_CODE                                                     (0x10A000A8 + PNIP_DEV_TOP_BASE)
#define U_GDMA__REVISION_CODE__MINOR_REVISION                                     (0x000000FF)
#define U_GDMA__REVISION_CODE__MAJOR_REVISION                                     (0x0000FF00)
#define U_GDMA__REVISION_CODE_RESET__VALUE                       0x0101


#define U_PERIF_APB__CR_ADDRESS_1                                                 (0x40000000 + PNIP_DEV_TOP_BASE)
#define U_PERIF_APB__CR_ADDRESS_1__CR_STARTADDRESS                                (0x00000FFC)
#define U_PERIF_APB__CR_ADDRESS_1__CR_ENDADDRESS                                  (0x03FF0000)
#define U_PERIF_APB__CR_ADDRESS_1__ZERO_DATA                                      (0x08000000)
#define U_PERIF_APB__CR_ADDRESS_1__NEW_DATA_INT                                   (0xF0000000)
#define U_PERIF_APB__CR_ADDRESS_1_RESET__VALUE                   0x00000FFC
// ...
#define U_PERIF_APB__CR_ADDRESS_27                                                (0x40000068 + PNIP_DEV_TOP_BASE)

#define U_PERIF_APB__CR_STATE_1                                                   (0x40000100 + PNIP_DEV_TOP_BASE)
#define U_PERIF_APB__CR_STATE_1__BUFFER_NUMBER_DATA_BUFFER                        (0x00000003)
#define U_PERIF_APB__CR_STATE_1__BUFFER_NUMBER_FREE_BUFFER                        (0x00000030)
#define U_PERIF_APB__CR_STATE_1__BUFFER_NUMBER_F2_BUFFER                          (0x000000C0)
#define U_PERIF_APB__CR_STATE_1__BUFFER_NUMBER_NEXT_BUFFER                        (0x00000300)
#define U_PERIF_APB__CR_STATE_1__BUFFER_NUMBER_N2_BUFFER                          (0x00000C00)
#define U_PERIF_APB__CR_STATE_1__BUFFER_NUMBER_USER_BUFFER                        (0x00003000)
#define U_PERIF_APB__CR_STATE_1__EXT_BUF_MODE                                     (0x00008000)
#define U_PERIF_APB__CR_STATE_1__MAPPING                                          (0xFFFF0000)
#define U_PERIF_APB__CR_STATE_1_RESET__VALUE                     0x00000FD2
// ...
#define U_PERIF_APB__CR_STATE_27                                                  (0x40000168 + PNIP_DEV_TOP_BASE)


#define U_PERIF_APB__GUARD_CONTROL_1                                              (0x40000200 + PNIP_DEV_TOP_BASE)
#define U_PERIF_APB__GUARD_CONTROL_1__GUARD_ADDRESS                               (0x00000FFF)
#define U_PERIF_APB__GUARD_CONTROL_1__GUARD_TYPE                                  (0x40000000)
#define U_PERIF_APB__GUARD_CONTROL_1__GUARD_VALID                                 (0x80000000)
#define U_PERIF_APB__GUARD_CONTROL_1_RESET__VALUE                0x00000000
// ...
#define U_PERIF_APB__GUARD_CONTROL_27                                             (0x40000268 + PNIP_DEV_TOP_BASE)


////////////////////////////////////////////////////////////////////////////////////
// Submodules for Module ERTEC200P_TOP
////////////////////////////////////////////////////////////////////////////////////
#ifdef _RENESAS_RZT_
#ifdef USE_HRAM
#define U_PNIP__BASE       (0x68000000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__END        (0x6817FFFF + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__BASE  (0x68180000 + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__END   (0x681FFFFF + PNIP_DEV_TOP_BASE)
#else
#define U_PNIP__BASE       (0x78000000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__END        (0x7817FFFF + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__BASE  (0x78180000 + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__END   (0x781FFFFF + PNIP_DEV_TOP_BASE)
#endif
#endif
#ifdef _RENESAS_RZN_
#ifdef USE_HRAM
#define U_PNIP__BASE       (0x64000000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__END        (0x6417FFFF + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__BASE  (0x64180000 + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__END   (0x641FFFFF + PNIP_DEV_TOP_BASE)
#else
#define U_PNIP__BASE       (0x74000000 + PNIP_DEV_TOP_BASE)
#define U_PNIP__END        (0x7417FFFF + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__BASE  (0x74180000 + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB__END   (0x741FFFFF + PNIP_DEV_TOP_BASE)
#endif
#endif

//#define U_GDMA__BASE       (0x10A00000 + PNIP_DEV_TOP_BASE)
//#define U_GDMA__END        (0x10AFFFFF + PNIP_DEV_TOP_BASE)

//#define U_PERIF_APB__BASE  (0x40000000 + PNIP_DEV_TOP_BASE)
//#define U_PERIF_APB__END   (0x40007FFF + PNIP_DEV_TOP_BASE)

////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////
// Memories for Module ERTEC200P_TOP
////////////////////////////////////////////////////////////////////////////////////
#if 0
#define U_PNIP_STATISTIC_RAM__START         (0x1060E000 + PNIP_DEV_TOP_BASE)
#define U_PNIP_STATISTIC_RAM__END           (0x1060E3FF + PNIP_DEV_TOP_BASE)

#define U_PNIP_FDB_RAM__START               (0x10700000 + PNIP_DEV_TOP_BASE)
#define U_PNIP_FDB_RAM__END                 (0x1071BFFF + PNIP_DEV_TOP_BASE)

#define U_PNIP_VLAN_RAM__START              (0x1071C000 + PNIP_DEV_TOP_BASE)
#define U_PNIP_VLAN_RAM__END                (0x1071FFFF + PNIP_DEV_TOP_BASE)
#define U_PNIP_QUEUECTRL_RAM__START         (0x10720000 + PNIP_DEV_TOP_BASE)
#define U_PNIP_QUEUECTRL_RAM__END           (0x1073FFFF + PNIP_DEV_TOP_BASE)
#define U_PNIP_QUEUEDATA_RAM__START         (0x10740000 + PNIP_DEV_TOP_BASE)
#define U_PNIP_QUEUEDATA_RAM__END           (0x107FFFFF + PNIP_DEV_TOP_BASE)
#define U_PNIP_SYNC_RAM__START              (0x1063C000 + PNIP_DEV_TOP_BASE)
#define U_PNIP_SYNC_RAM__END                (0x1063FFFF + PNIP_DEV_TOP_BASE)
#define U_PNIP_PACKDATA_RAM__START          (0x10658000 + PNIP_DEV_TOP_BASE)
#define U_PNIP_PACKDATA_RAM__END            (0x1065FFFF + PNIP_DEV_TOP_BASE)
#define U_PNIP_CMDCTRL_RAM__START           (0x10668000 + PNIP_DEV_TOP_BASE)
#define U_PNIP_CMDCTRL_RAM__END             (0x1066FFFF + PNIP_DEV_TOP_BASE)
#define U_PNIP_APICTRL_RAM__START           (0x10680000 + PNIP_DEV_TOP_BASE)
#define U_PNIP_APICTRL_RAM__END             (0x106FFFFF + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB_PERIF_IO_RAM__START     (0x10800800 + PNIP_DEV_TOP_BASE)
#define U_PERIF_AHB_PERIF_IO_RAM__END       (0x1080DFFF + PNIP_DEV_TOP_BASE)

#define U_PERIF_APB_PERIF_IO_RAM__START     (0x40002000 + PNIP_DEV_TOP_BASE)
#define U_PERIF_APB_PERIF_IO_RAM__END       (0x40004FFF + PNIP_DEV_TOP_BASE)
#endif
////////////////////////////////////////////////////////////////////////////////////

#define SY_INT_PER_IF_ARM_IRQ  	1
#define SY_INT_GDMA_IRQ			2

#endif
