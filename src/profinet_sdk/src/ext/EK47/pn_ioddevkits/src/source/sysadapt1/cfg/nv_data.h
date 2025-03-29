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
/*  F i l e               &F: nv_data.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/* socket layer abstraction interface, implementation for Windows            */
/*                                                                           */
/*****************************************************************************/
#ifndef _NV_DATA_H
#define _NV_DATA_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif


    #define NV_DATA_VERSION     0x0411                   // version 4.1.0 of NV-Data


    // NV data configuration
    #define NV_USERDATA_TOT_LEN  0x8000      //* Size of the NVRAM sector for non-volatile data
    #define NV_USERDATA_SEC_LEN  0x4000      //* Size of the NVRAM single data sector
    #define NV_USERDATA_SEC1     0           //* First NVRAM sector number
    #define NV_USERDATA_SEC2     1           //* Second NVRAM sector number
    // Important - in case of NAND Flash nv data are stored in sectors of size 0x4000 therefore
    // second section offset has exactly that size

	#define ENTER_NV_DATA	OsEnterX(OS_MUTEX_NV_DATA);
	#define EXIT_NV_DATA	OsExitX(OS_MUTEX_NV_DATA);

    #define MAC_ADDR_LEN              6
    #define REMA_DATA_MAXLEN       1024

    #define SNMP_SYSNAME_MAXLEN     255     // according to RFC1213, do not change
    #define SNMP_SYSLOC_MAXLEN      255     // according to RFC1213, do not change
    #define SNMP_SYSCONT_MAXLEN     255     // according to RFC1213, do not change
    #define SNMP_SYSDESCR_MAXLEN    255     // according to RFC1213, do not change
    #define DEVICE_NAME_MAXLEN      256
    #define PRIV_DATA_MAXLEN        128
	#define IP_SUITE_LEN			12
	#define ARFSU_LEN				20		//4B Mode + 16B UUID





    typedef enum
    {
        PNIO_NVDATA_MAC_ADDR ,                          // type = device mac address
        PNIO_NVDATA_DEVICENAME,                         // type = device name
        PNIO_NVDATA_IPSUITE,                            // type = IP suite:  IP address, Subnet Mask, Default Router, format: network (big endian)
        PNIO_NVDATA_PDEV_RECORD,                        // type = physical device records
        PNIO_NVDATA_IM1,                                // type = I&M1 data
        PNIO_NVDATA_IM2,                                // type = I&M1 data
        PNIO_NVDATA_IM3,                                // type = I&M1 data
        PNIO_NVDATA_IM4,                                // type = I&M1 data
        PNIO_NVDATA_SNMP_SYSNAME,                       // SNMP MIB2 object "system name"
        PNIO_NVDATA_SNMP_SYSCONT,                       // SNMP MIB2 object "system contact"
        PNIO_NVDATA_SNMP_SYSLOC,                        // SNMP MIB2 object "system location"
        PNIO_NVDATA_PRIVATE_DATA,						// Own remanent DATA (e.g. MAC address)
		PNIO_NVDATA_ARFSU,								// UUID for AR-FSU functionality,
		PNIO_NVDATA_SUBCFG,								// Submodule Configurations,
		PNIO_NVDATA_VENDOR_PRM,							// Vendor Parameter,
    } PNIO_NVDATA_TYPE;

    typedef struct
    {
        PNIO_UINT32 IpAddr;                             // ip address
        PNIO_UINT32 SubnetMask;                         // network mask
        PNIO_UINT32 DefRouter;                          // default router
    } NV_IP_SUITE;  // format: network (big endian)

    typedef struct
    {
        PNIO_INT8   sysName     [SNMP_SYSNAME_MAXLEN];     // MIB2 object sysName
        PNIO_INT8   sysContact  [SNMP_SYSCONT_MAXLEN];     // MIB2 object sysContact
        PNIO_INT8   sysLocation [SNMP_SYSLOC_MAXLEN];      // MIB2 object sysLocation
    } NV_SNMP;

    typedef PNIO_VOID (*nv_function_handler)(PNIO_VOID*, PNIO_UINT32);

    typedef struct
    {
        PNIO_BOOL   trigger_pndv;
        PNIO_BOOL   perform_host_callback;
        PNIO_BOOL   im_callback;
        PNIO_UINT32 nv_data_type;
        PNIO_BOOL   restore_first_section;
    } nv_function_param;


    // ******* structure with callback and argument passed to queue ********
	typedef struct
	{
		nv_function_handler callback;
		void* arg;
	} NVM_CBF_STR;

	// *********** data structures **********
#define PNS_VENDOR_NAME_SIZE	20U
#define PNS_PRODUCT_NAME_SIZE	20U
#define PNS_PRODUCT_FAMILY_SIZE	22U
#define PNS_MAX_MAC_ADDR		(3U+1U)

#pragma pack(1)
	typedef struct _PNS_VENDOR_PRM_TYPE
	{
		PNIO_UINT16			VendorID;
		PNIO_UINT16			DeviceID;
		PNIO_UINT32         ProductCode;
		PNIO_UINT32         ArticleNr;
		PNIO_UINT8          VendorName[PNS_VENDOR_NAME_SIZE];
		PNIO_UINT8          ProductName[PNS_PRODUCT_NAME_SIZE];
		PNIO_UINT8          ProductFamily[PNS_PRODUCT_FAMILY_SIZE];
		PNIO_UINT8          OrderID[IM0_ORDER_ID_LEN];
		PNIO_UINT8          SerialNr[IM0_SER_NUM_LEN];
		PNIO_UINT8          MacAddr[PNS_MAX_MAC_ADDR][MAC_ADDR_LEN];              // mac address
	} PNS_VENDOR_PRM;
#pragma pack()


#pragma pack(1)
typedef struct _s_pns_nv_subcfg
{
	PNIO_UINT32 nApi;
	PNIO_UINT16 nSlot;
	PNIO_UINT16 nSslot;
	PNIO_UINT32 nModId;
	PNIO_UINT32 nSubId;
	PNIO_UINT16 nIn;
	PNIO_UINT16 nOut;
}TS_PNS_NV_SUBCFG, *TS_PNS_NV_SUBCFG_PTR;
#pragma pack()


#pragma pack(1)
typedef struct _NV_DATA_VENDOR_TYPE
{
	PNIO_UINT32         CheckSum;                           // checksum over all, must be first element
	PNIO_UINT32         Version;                            // version of the data
	PNS_VENDOR_PRM      tPrm;
} NV_DATA_VENDOR_PRM;
#pragma pack()



// *********** data structures **********
#pragma pack(1)
typedef struct _NV_DATA_STRUCT_TYPE
{
	PNIO_UINT32         CheckSum;                           // checksum over all, must be first element
	PNIO_UINT32         Version;                            // version of the data
	PNIO_UINT8          MacAddr[MAC_ADDR_LEN];              // mac address
	NV_IP_SUITE         IpSuite;                            // ip suite (ip-addr, subnet mask, default router)
	IM1_DATA            IM1 [IOD_CFG_MAX_NUMOF_SUBSLOTS];    // I&M1  data
	IM2_DATA            IM2 [IOD_CFG_MAX_NUMOF_SUBSLOTS];    // I&M2  data
	IM3_DATA            IM3 [IOD_CFG_MAX_NUMOF_SUBSLOTS];    // I&M3  data
	IM4_DATA            IM4 [IOD_CFG_MAX_NUMOF_SUBSLOTS];    // I&M4  data

	PNIO_UINT16         SnmpSysNameLen;                     // 0xffff: invalid, else length value
	PNIO_UINT8          SnmpSysName[SNMP_SYSNAME_MAXLEN];
	PNIO_UINT16         SnmpSysContLen;                     // 0xffff: invalid, else length value
	PNIO_UINT8          SnmpSysCont[SNMP_SYSCONT_MAXLEN];
	PNIO_UINT16         SnmpSysLocLen;                      // 0xffff: invalid, else length value
	PNIO_UINT8          SnmpSysLoc[SNMP_SYSLOC_MAXLEN];
	PNIO_UINT16         DevNameLen;                         // 0xffff: invalid, else length value
	PNIO_UINT8          DevName[DEVICE_NAME_MAXLEN];
	PNIO_UINT16         RemaDataLen;                        // 0xffff: invalid, else length value
	PNIO_UINT8          RemaData[REMA_DATA_MAXLEN];
	PNIO_UINT16         PrivDataLen;                        // 0xffff: invalid, else length value
	PNIO_UINT8          PrivData[PRIV_DATA_MAXLEN];
	PNIO_UINT8			ARFSU[ARFSU_LEN];			//0x0000: empty
	PNIO_UINT16         SubCfgSize;
	TS_PNS_NV_SUBCFG    tSubCfgList[IOD_CFG_NUMOF_SUBSLOTS];
} NV_DATA_STRUCT;
#pragma pack()


//#define RZT2_PNS_NV_SUBCFG_SIZE  (sizeof(TS_PNS_NV_SUBCFG) * IOD_CFG_NUMOF_SUBSLOTS ) // // (sizeof(g_tPnsNvSubCfg));


    PNIO_UINT32  Bsp_im_data_store
    		(
    			PNIO_NVDATA_TYPE    NvDataType,     // type of data (device name, IP suite, PDEV records,...)
    			void*               pMem,           // pointer to data source
    			PNIO_UINT32         MemSize,         // size of memory to store
    			LSA_UINT32			ModIdent        // entity index in periph interface-real_cfg
    		);

	PNIO_UINT32  Bsp_im_data_store_no_pndv_trigger
		(
			PNIO_NVDATA_TYPE    NvDataType,     // type of data (device name, IP suite, PDEV records,...)
			void*               pMem,           // pointer to data source
			PNIO_UINT32         MemSize,         // size of memory to store
			LSA_UINT32			ModIdent        // entity index in periph interface-real_cfg
		);

    PNIO_UINT32  Bsp_nv_data_store
		    (
                PNIO_NVDATA_TYPE    NvDataType,         // type of data (device name, IP suite, PDEV records,...)
			    void*		        pMem,		        // pointer to data source
			    PNIO_UINT32     	MemSize             // size of memory to store
            );

    PNIO_UINT32  Bsp_im_data_restore
      		(
      			PNIO_NVDATA_TYPE    NvDataType,     // type of data (device name, IP suite, PDEV records,...)
      			void**              ppMem,           // pointer to data source
      			PNIO_UINT32*         pMemSize,         // size of memory to restore
      			LSA_UINT32			ModIdent
      		);


    PNIO_UINT32  Bsp_nv_data_restore
		    (
                PNIO_NVDATA_TYPE    NvDataType,         // type of data (device name, IP suite, PDEV records,...)
			    void** 		        ppMem, 		        // destination pointer
			    PNIO_UINT32*     	pMemSize            // size of memory to restore
            );

   PNIO_UINT32  Bsp_nv_data_init    (PNIO_VOID* nvDataInit, PNIO_UINT32 nvDataLen);
   PNIO_UINT32  Bsp_nv_data_clear   (PNIO_RTF_OPTION RtfOption);// reset to factory settings
   PNIO_UINT32  Bsp_nv_data_memfree (PNIO_VOID* pMem);  // free memory (must be called after



#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
