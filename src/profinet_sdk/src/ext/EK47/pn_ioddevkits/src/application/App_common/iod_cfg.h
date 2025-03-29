/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
/*  This program is protected by German copyright law and international      */
/*  treaties. The use of this software including but not limited to its      */
/*  Source Code is subject to restrictions as agreed in the license          */
/*  agreement between you and Siemens.                                       */
/*  Copying or distribution is not allowed unless expressly permitted        */
/*  according to your license agreement with Siemens.                        */
/*****************************************************************************/
/*                                                                           */
/*  P r o j e c t         &P: PROFINET IO Runtime Software              :P&  */
/*                                                                           */
/*  P a c k a g e         &W: PROFINET IO Runtime Software              :W&  */
/*                                                                           */
/*  C o m p o n e n t     &C: PnIODDevkits                              :C&  */
/*                                                                           */
/*  F i l e               &F: iod_cfg.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  configuration file for the PNIO stack                                    */
/*  has to be modified by the user.                                          */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*                                                                           */
/*  Date        Version        Who  What                                     */
/*                                                                           */
/*****************************************************************************/

/* The file for all the settings of Devkit */

#ifndef APPLICATION_APP_COMMON_PNIO_CONF_H_
#define APPLICATION_APP_COMMON_PNIO_CONF_H_

/* If C++ - compiler: Use C linkage */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief macro switch for asset management record
 *
 * 0: no AMR functionality
 * 1: with AMR functionality
 */
#define IOD_INCLUDE_AMR                 1

/**
 * @brief marco switch for S2 redundancy
 *
 * 0: no S2 functionality
 * 1: S2 functionality enabled
 */
#ifndef IOD_INCLUDE_S2_REDUNDANCY
#define IOD_INCLUDE_S2_REDUNDANCY       1
#endif
#if defined INCLUDE_S2_REDUNDANCY
#error "INCLUDE_S2_REDUNDANCY macro is depreciated and replaced by IOD_INCLUDE_S2_REDUNDANCY"
#endif

/**
 *  @brief macro switch for XHIF connection
 *         ERTEC will behave as slave only for PNIO stack with prepared memory interface
 *
 * 0: no XHIF memory interface
 * 1: XHIF memory interface is functional
 * */
#ifndef IOD_USED_WITH_XHIF_HOST
#define IOD_USED_WITH_XHIF_HOST         0
#endif
/**
 *  @brief macro switch for dynamic reconfiguration (CiR)
 *      Needs S2 redundancy activated to operate
 *
 *  0: support for DR is not enabled
 *  1: support for DR is enabled
 */
#ifndef IOD_INCLUDE_DR
#define IOD_INCLUDE_DR                  1
#endif
#if ((0 == IOD_INCLUDE_S2_REDUNDANCY) && (1 == IOD_INCLUDE_DR))
#error "Dynamic reconfiguration requires S2 redundancy activated to operate"
#endif

/**
 *  @brief macro switch for I&M data handling
 *
 * 1: PNIO stack handles IM0..4
 * 0: application handles all IM
 */
#define IOD_INCLUDE_IM0_4               1
#if defined INCLUDE_IM0_4_HANDLING
#error "INCLUDE_IM0_4_HANDLING macro is depreciated and replaced by IOD_INCLUDE_IM0_4"
#endif

/**
 * @brief marco switch for handing with 8028/8029 record
 *
 * 0: application handles rec 8028/8029
 * 1: record read index 8028/8029 handling inside stack;
 */
#define IOD_INCLUDE_REC8028_8029        1
#if defined INCLUDE_REC8028_8029_HANDLING
#error "INCLUDE_REC8028_8029_HANDLING macro is depreciated and replaced by IOD_INCLUDE_REC8028_8029"
#endif

/**
 * @brief macro switch for MRP functionality
 *
 * 0: no MRP functionality
 * 1: MRP functionality enabled
 */
#ifndef IOD_INCLUDE_MRP
#define IOD_INCLUDE_MRP                 1
#endif
#if defined INCLUDE_MRP_CAPABILITY
#error "INCLUDE_MRP_CAPABILITY macro is depreciated and replaced by IOD_INCLUDE_MRP"
#endif

/**
 * @brief macro switch for MRPD functionality
 *
 * 0: no MRPD functionality
 * 1: MRPD functionality enabled
 */
#ifndef IOD_INCLUDE_MRPD
#define IOD_INCLUDE_MRPD                0
#endif
#if defined INCLUDE_MRPD_CAPABILITY
#error "INCLUDE_MRPD_CAPABILITY macro is depreciated and replaced by IOD_INCLUDE_MRPD"
#endif

/**
 * @brief macro switch for IM5 functionality
 *
 * 0: no IM5 functionality
 * 1: IM5 functionality
 */
//#define IOD_INCLUDE_IM5                 1
#define IOD_INCLUDE_IM5                 0
#if defined INCLUDE_IM5_CAPABILITY
#error "INCLUDE_REMA macro is depreciated and replaced by IOD_INCLUDE_IM5"
#endif

/**
 * @brief macro switch for POF functionality
 *
 * 0: no POF board
 * 1: POF board
 *
 * */
#ifndef IOD_INCLUDE_POF
#define IOD_INCLUDE_POF                 0
#endif
#if defined IOD_ENABLE_POF
#error "IOD_ENABLE_POF macro is depreciated and replaced by IOD_INCLUDE_POF"
#endif

/**
 * @brief macro switch for FSU functionality
 *
 * 0: deactivate FSU capability
 * 1: activate FSU capability
 *
 */
#define IOD_FSU_SUPPORTED               1
#if defined IOD_INCLUDE_FSU
#error "IOD_INCLUDE_FSU macro is depreciated and replaced by IOD_FSU_SUPPORTED"
#endif

/**
 * @brief Number of PDEV ports
 *
 * 1        : for PNIOD_PLATFORM_DKSW
 * 1 - 2    : for PNIOD_PLATFORM_EB200
 * 1 - 2    : for PNIOD_PLATFORM_EB200P
 * 1 - 4    : for PNIOD_PLATFORM_EB400
 */
#ifndef IOD_CFG_PDEV_NUMOF_PORTS
#define IOD_CFG_PDEV_NUMOF_PORTS        2
#endif
#if defined IOD_PDEV_NUMOF_PORTS
#error "IOD_PDEV_NUMOF_PORTS macro is depreciated and replaced by IOD_CFG_PDEV_NUMOF_PORTS"
#endif

 /**
  * @brief Enable performance upgrade features (such as short preamble, DFP, 
  *        dynamic fragmentation, absolute/relative forwarding)
  *
  * 1        : enable (possible only with ERTEC200P-2)
  * 0        : disable
  */
#define IOD_CFG_PU_FEATURES_SUPPORT     0

/**
 * @brief Device Tpe in SNMP
 *
 *
 */
#ifdef _RENESAS_RZT_
#define IOD_CFG_DEVICE_TYPE                     "RSK-RZT2"
#endif
#ifdef _RENESAS_RZN_
#define IOD_CFG_DEVICE_TYPE                     "DEVKIT"
#endif

/**
 * @brief Device Vendor in SNMP
 *
 *
 */
#define IOD_CFG_DEVKIT_VENDOR                   "Siemens AG"

/**
 * @brief Device Product Family in SNMP
 *
 *
 */
#define IOD_CFG_DEVKIT_PRODUCTFAMILY            "SINUMERIK"

/**
 * @brief Device Product Name
 *
 *
 */
#ifdef _RENESAS_RZT_
#define IOD_CFG_DEVKIT_PRODUCTNAME              "RSK RZ/T2 PN-IO"
#endif
#ifdef _RENESAS_RZN_
#define IOD_CFG_DEVKIT_PRODUCTNAME              "RSK RZ/N2 PN-IO"
#endif

/**
 * @brief Device Order ID
 *
 *
 */
#ifdef _RENESAS_RZT_
    
#ifdef UPD60620_PHY
#define IOD_CFG_DEV_ANNOTATION_ORDER_ID         "RSK-RZT2-PNIO-REPHY"
  #else  
#define IOD_CFG_DEV_ANNOTATION_ORDER_ID         "RSK-RZT2-PNIO-XXXX"
#endif

#endif
#ifdef _RENESAS_RZN_
#define IOD_CFG_DEV_ANNOTATION_ORDER_ID         "RSK-RZN2-PNIO-XXXX"
#endif
#ifdef DEV_ANNOTATION_ORDER_ID
#error "DEV_ANNOTATION_ORDER_ID macro is depreciated and replaced by IOD_CFG_DEV_ANNOTATION_ORDER_ID"
#endif

/**
 * @brief Vendor ID
 *
 * must fit to GSDML file
 */
#define IOD_CFG_VENDOR_ID                       0x002a

/**
 * @brief Device ID
 *
 * must fit to GSDML file
 */
#ifdef UPD60620_PHY
#define IOD_CFG_DEVICE_ID                       0x0004
#else  
#define IOD_CFG_DEVICE_ID                       0x0308
#endif

/**
 * @brief maximum number of controller/supervisor ARs per device
 *
 *
 */
#if EXAMPL_DEV_CONFIG_VERSION == 2
 //Example Application 2 supports one Ar.
 #define IOD_CFG_NUMOF_IO_AR                     1
#else 
 #define IOD_CFG_NUMOF_IO_AR                     2
#endif
/**
 * @brief maximum number of device access ARs per device
 *
 *
 */
#define IOD_CFG_NUMOF_DEV_ACCESS_AR             1

/**
 * @brief maximum number of slots
 *  
 * 
 */
 #define IOD_CFG_MAX_SLOT_NUMBER                 64

/**
 * @brief maximum number of subslots
 * 
 *
 */
 #define IOD_CFG_NUMOF_SUBSLOTS                  32

/**
 * @brief maximum number of subslots per slot
 *
 *
 */
#define IOD_CFG_MAX_NUMOF_SUBSL_PER_SLOT        6

/**
 * @brief Number of bytes per subslot
 *
 *
 */
#define IOD_CFG_NUMOF_BYTES_PER_SUBSLOT         255

/**
 * @brief Device Access Point slot
 *
 * must fit to GSDML file
 */
#define IOD_CFG_DAP_SLOT_NUMBER                 0

/**
 * @brief Device Access Point subslot
 *
 * must fit to GSDML file
 */
#define IOD_CFG_DAP_SUBSLOT_NUMBER              1

/**
 * @brief Maximum data size for record read/wwrite RQs
 *
 *
 */
#define IOD_CFG_MAX_RECORD_LENGTH               8*1024

/**
 * @brief Maximum IO net length per subslot
 *
 *
 */
#define IOD_CFG_MAX_IO_NET_LENGTH_PER_SUBSLOT   256

/**
 * @brief Maximum data size for process alarms
 *
 *
 */
#define IOD_CFG_MAX_PROCESS_ALARM_LEN           64

/**
 * @brief Maximum info data size for upload/retrieval alarms
 *
 *
 */
#define IOD_CFG_MAX_UPLOAED_RETRIEVAL_INFO_LEN  24

/**
 * @brief Maximum data size for generic diagnosis data
 *
 *
 */
#define IOD_CFG_MAX_GEN_DIAG_DATA_LEN           200

/**
 * @brief Serial number of device
 *
 * must be 16 Bytes, fillbyte = <blank>
 */
#define IOD_CFG_IM0_SERIAL_NUMBER               "1234567890     "
#ifdef IM0_SERIAL_NUMBER
#error "IM0_SERIAL_NUMBER macro is depreciated and replaced by IOD_CFG_IM0_SERIAL_NUMBER"
#endif

/**
 * @brief HW revision of Device
 *
 * used in I&M0 and SNMP
 */
#define IOD_CFG_HW_REVISION                     100

/**
 * @brief interface name description in SNMP
 *
 */
#ifdef _RENESAS_RZT_
#define IOD_CFG_SNMP_INTERFACE_NAME             "Renesas, RZT2-DEVKIT, internal, X1"
#endif
#ifdef _RENESAS_RZN_
#define IOD_CFG_SNMP_INTERFACE_NAME             "Renesas, RZN2-DEVKIT, internal, X1"
#endif
#ifdef SNMP_INTERFACE_NAME
#error "SNMP_INTERFACE_NAME macro is depreciated and replaced by IOD_CFG_SNMP_INTERFACE_NAME"
#endif

/**
 * @brief PHY port 1 description in SNMP
 *
 */
#ifdef _RENESAS_RZT_
#define IOD_CFG_SNMP_PORT1_NAME                 "Renesas, RZT2-DEVKIT, Ethernet Port, X1 P1"
#endif
#ifdef _RENESAS_RZN_
#define IOD_CFG_SNMP_PORT1_NAME                 "Renesas, RZN2-DEVKIT, Ethernet Port, X1 P1"
#endif
#ifdef SNMP_PORT1_NAME
#error "SNMP_PORT1_NAME macro is depreciated and replaced by IOD_CFG_SNMP_PORT1_NAME"
#endif

/**
 * @brief PHY port 2 description in SNMP
 *
 */
#ifdef _RENESAS_RZT_
#define IOD_CFG_SNMP_PORT2_NAME                 "Renesas, RZT2-DEVKIT, Ethernet Port, X1 P2"
#endif
#ifdef _RENESAS_RZN_
#define IOD_CFG_SNMP_PORT2_NAME                 "Renesas, RZN2-DEVKIT, Ethernet Port, X1 P2"
#endif
#ifdef SNMP_PORT2_NAME
#error "SNMP_PORT2_NAME macro is depreciated and replaced by IOD_CFG_SNMP_PORT2_NAME"
#endif

/**
 * @brief PHY port 3 description in SNMP
 *
 */
#ifdef _RENESAS_RZT_
#define IOD_CFG_SNMP_PORT3_NAME                 "Renesas, RZT2-DEVKIT, Ethernet Port, X1 P3"
#endif
#ifdef _RENESAS_RZN_
#define IOD_CFG_SNMP_PORT3_NAME                 "Renesas, RZN2-DEVKIT, Ethernet Port, X1 P3"
#endif
#ifdef SNMP_PORT3_NAME
#error "SNMP_PORT3_NAME macro is depreciated and replaced by IOD_CFG_SNMP_PORT3_NAME"
#endif

/**
 * @brief PHY port 4 description in SNMP
 *
 */
#ifdef _RENESAS_RZT_
#define IOD_CFG_SNMP_PORT4_NAME                 "Renesas, RZT2-DEVKIT, Ethernet Port, X1 P4"
#endif
#ifdef _RENESAS_RZN_
#define IOD_CFG_SNMP_PORT4_NAME                 "Renesas, RZN2-DEVKIT, Ethernet Port, X1 P4"
#endif
#ifdef SNMP_PORT4_NAME
#error "SNMP_PORT4_NAME macro is depreciated and replaced by IOD_CFG_SNMP_PORT4_NAME"
#endif

/**
 * @brief interface description used in SNMP
 *
 */
#ifdef _RENESAS_RZT_
#define IOD_CFG_SNMP_INTERFACE_DESCRIPTION      "RSK RZT2 based PNIO device"
#endif
#ifdef _RENESAS_RZN_
#define IOD_CFG_SNMP_INTERFACE_DESCRIPTION      "RSK RZN2 based PNIO device"
#endif
#ifdef SNMP_INTERFACE_DESCRIPTION
#error "SNMP_INTERFACE_DESCRIPTION macro is depreciated and replaced by IOD_CFG_SNMP_INTERFACE_DESCRIPTION"
#endif


/**
 *  @brief macro switch for ARP Frame Filter(Hardware)
 *
 *  0: ARP Filter(Hardware) is not enabled
 *  1: ARP Filter(Hardware) is enabled
 */
#ifndef IOD_INCLUDE_STORM_FILTER_ARP_HW
#define IOD_INCLUDE_STORM_FILTER_ARP_HW                  0
#endif

/**
 *  @brief macro switch for ARP Frame Filter(Software)
 *
 *  0: ARP Filter(Software) is not enabled
 *  1: ARP Filter(Software) is enabled
 */
#ifndef IOD_INCLUDE_STORM_FILTER_ARP_SW
#define IOD_INCLUDE_STORM_FILTER_ARP_SW                  0
#endif

/**
 *  @brief macro switch for DCP Frame Filter(Software)
 *
 *  0: DCP Filter(Software) is not enabled
 *  1: DCP Filter(Software) is enabled
 */
#ifndef IOD_INCLUDE_STORM_FILTER_DCP_SW
#define IOD_INCLUDE_STORM_FILTER_DCP_SW                  0
#endif

/**
 *  @brief macro switch for EtherType Frame Filter(Software)
 *
 *  0: EtherType Filter(Software) is not enabled
 *  1: EtherType Filter(Software) is enabled
 */
#ifndef IOD_INCLUDE_STORM_FILTER_ETHER_SW
#define IOD_INCLUDE_STORM_FILTER_ETHER_SW                0
#endif

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif /* APPLICATION_APP_COMMON_PNIO_CONF_H_ */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
