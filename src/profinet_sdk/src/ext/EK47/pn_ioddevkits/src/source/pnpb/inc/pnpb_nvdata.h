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
/*  F i l e               &F: pnpb_nvdata.h                             :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
#ifndef PNPB_NVDATA_H
#define PNPB_NVDATA_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

	#define PNPB_CTRL_IP_SUITE 		( PNIO_UINT32 )0x00000002
	#define PNPB_CTRL_STATION_NAME 	( PNIO_UINT32 )0x00000008

	#define PNPB_SYSNAME_MAXLEN     255     // according to RFC1213, do not change
    #define PNPB_SYSLOC_MAXLEN      255     // according to RFC1213, do not change
    #define PNPB_SYSCONT_MAXLEN     255     // according to RFC1213, do not change
    #define PNPB_SYSDESCR_MAXLEN    255     // according to RFC1213, do not change

    #define PNPB_SWITCH_TO_BIG_END(x) 	(((x) >> 24 ) | (((x) << 8) & 0x00ff0000) | (((x) >> 8) & 0x0000ff00) | ((x) << 24))

    typedef struct
    {
        PNIO_UINT16         SysNameLen;                     // 0xffff: invalid, else length value
        PNIO_UINT8          SysName[PNPB_SYSNAME_MAXLEN];
        PNIO_UINT16         SysContLen;                     // 0xffff: invalid, else length value
        PNIO_UINT8          SysCont[PNPB_SYSCONT_MAXLEN];
        PNIO_UINT16         SysLocLen;                      // 0xffff: invalid, else length value
        PNIO_UINT8          SysLoc[PNPB_SYSLOC_MAXLEN];
        PNIO_UINT16         SysDescrLen;                    // 0xffff: invalid, else length value
        PNIO_UINT8          SysDescr[PNPB_SYSDESCR_MAXLEN];
    }  PNPB_SNMP_DATA;

    typedef struct pnpb_station_tag  * PNPB_UPPER_STATION_PTR_TYPE;

    typedef struct pnpb_station_tag
    {
        PNIO_UINT8              LocalMacAddr[6];
        PNIO_UINT8              LocalIpAddr[4];
        PNIO_UINT8              LocalSubnetMask[4];
        PNIO_UINT8              DefaultRouterAddr[4];
        PNIO_VOID *  			pStationType;
        PNIO_UINT16             StationTypeLen;             /* OHA_MAX_STATION_TYPE_LEN */
        PNIO_VOID *  			pStationName;               /* OHA_MAX_STATION_NAME_LEN */
        PNIO_UINT16             StationNameLen;             /* OHA_MAX_STATION_NAME_LEN */
        PNIO_UINT16             VendorId;                   /* Vendor-ID                */
        PNIO_UINT16             DeviceId;                   /* Device-ID                */
        PNIO_UINT16             TcpIpStatus;        /* status-word for TCP/IP parameter */
        PNIO_UINT8              ProfinetRole;               /* 0=no role, 1=Profinet IO,*/
        PNIO_UINT16             DeviceInstance;             /* DeviceInstanceValue      */
    } PNPB_STATION_TYPE;


    PNIO_VOID PnpbNvInit (void);

    PNIO_VOID PnpbNvSaveStationName (PNIO_INT8   *pStationName,     // unique station name
                                     PNIO_UINT32 StationNameLen,
                                     PNIO_BOOL   Remanent);

    PNIO_VOID PnpbNvSetStationName  (PNIO_INT8   *pStationName,     // unique station name
                                     PNIO_UINT32 StationNameLen);

    PNIO_VOID PnpbNvSetStationType (PNIO_INT8   *pStationType,     // station type
                                    PNIO_UINT32 StationTypeLen);
    PNIO_VOID PnpbNvSetIpSuite  (PNIO_UINT32 IpAddr,            // IP Suite: IP Addr
                                 PNIO_UINT32 SubnetMask,        // IP Suite: Subnet Mask
                                 PNIO_UINT32 DefRouterAddr);    // IP Suite: default router address

    PNIO_VOID Pnpb_set_new_address( PNIO_UINT32 param_to_change );
    PNIO_VOID Pnpb_finish_set_new_address( PNPB_UPPER_STATION_PTR_TYPE pStationData );

    PNIO_VOID PnpbNvSaveIpSuite (PNIO_UINT32 IpAddr,            // IP Suite: IP Addr
                                 PNIO_UINT32 SubnetMask,        // IP Suite: Subnet Mask
                                 PNIO_UINT32 DefRouterAddr,     // IP Suite: default router address
                                 PNIO_BOOL   Remanent);          // LSA_TRUE: save remanent

    PNIO_UINT32 Pnpb_report_new_ip_data(	PNIO_UINT32 NewIpAddr,
       								    	PNIO_UINT32 SubnetMask,
   											PNIO_UINT32 DefRouterAddr);

    PNIO_VOID PnpbNvSetVendorAndDeviceId (PNIO_UINT16 VendorId, PNIO_UINT16 DeviceId);

    PNIO_VOID PnpbNvSetAnnotation (PNIO_ANNOTATION *pDevAnnotation);

    PNIO_VOID PnpbNvSetSubmodId(PNIO_SUB_LIST_ENTRY   *pPDEVSubList,
                                PNIO_UINT32           pPDEVSubListLen);

    PNIO_VOID PnpbNvSetSnmpData (PNIO_SNMP_LLDP *pSnmpData);

    PNIO_VOID* PnpbNvGetAnnotation (void);

    PNPB_SNMP_DATA* PnpbNvGetpSnmpData (void);

    PNIO_VOID   PnpbNvpSnmpDatas ( PNIO_UINT8** ppName, PNIO_UINT32* pNameLen, PNIO_UINT8** ppContact, PNIO_UINT32* pContactLen, PNIO_UINT8** ppLocation, PNIO_UINT32* pLocationLen);

    PNIO_UINT32 PnpbNvGetStationName ( PNIO_UINT8* pStationName, PNIO_UINT32 BufSize);

    PNIO_VOID   PnpbNvGetpStationName ( PNIO_UINT8** ppStationName, PNIO_UINT32* pStationNameLen);

    PNIO_UINT32 PnpbNvGetStationType  (PNIO_UINT8* pStationType, PNIO_UINT32 BufSize);

    PNIO_UINT32 PnpbNvGetOrderId 	  ( PNIO_UINT8* pOrderId,    PNIO_UINT32 BufSize);

    PNIO_UINT32 PnpbNvGetSerialNumber ( PNIO_UINT8* pSerialNumber, PNIO_UINT32 BufSize);

    PNIO_UINT32 PnpbNvGetHwRevision   ( void );

    PNIO_UINT32 PnpbNvGetSwRevision   ( PNIO_UINT8* pSwRevision, PNIO_UINT32 BufSize);

    PNIO_UINT16 PnpbGetVendorId (void);

    PNIO_UINT16 PnpbGetDeviceId (void);

    PNIO_UINT32 PnpbGetModId    (PNIO_UINT32 SubmodIdx);

    PNIO_UINT32 PnpbGetSubmodId ( PNIO_UINT32 SubmodIdx );

    PNIO_UINT32 PnpbGetSubmodCnt (void);

    PNIO_VOID   PnpbNvGetIpSuite (PNIO_UINT32* pIpAddr,             // IP Suite: IP Addr
                                  PNIO_UINT32* pSubnetMask,         // IP Suite: Subnet Mask
                                  PNIO_UINT32* pDefRouterAddr);     // IP Suite: default router address

    PNIO_VOID   PnpbNvGetpIpSuit (PNIO_UINT8** ppIP, PNIO_UINT32* pIPLen);

    PNIO_VOID   PnpbNvResetToFactorySettings (PNIO_RTF_OPTION RtfOption);

    PNIO_UINT32 PnpbNvStoreRemaMem ( PNIO_UINT32         MemSize,   // REMA data size
                                     PNIO_UINT8*         pMem);     // pointer to pointer to rema data


    PNIO_UINT32  PnpbNvRestoreRemaMem (PNIO_UINT32*        pMemSize, // REMA data size
                                       PNIO_UINT8**        ppMem);   // pointer to pointer to rema data

    PNIO_UINT32  PnpbNvStoreSnmpData
        (
            PNIO_UINT32         SysNameLen,
            PNIO_UINT8*         pSysName,
            PNIO_UINT32         SysContLen,
            PNIO_UINT8*         pSysCont,
            PNIO_UINT32         SysLocLen,
            PNIO_UINT8*         pSysLoc
        );


#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif



#endif


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
