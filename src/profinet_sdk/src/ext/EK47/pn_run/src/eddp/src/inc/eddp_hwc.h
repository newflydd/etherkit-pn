
#ifndef eddp_hwc_H                       /* ----- reinclude-protection ----- */
#define eddp_hwc_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

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
/*  C o m p o n e n t     &C: EDDP (EthernetDeviceDriver for PN-IP)     :C&  */
/*                                                                           */
/*  F i l e               &F: eddp_hwc.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  xx.xx.xx    xx    initial version.                                       */
#endif
/*****************************************************************************/
         
                                   
/*===========================================================================*/
/*                              constants                                    */
/*===========================================================================*/

#define EDDP_PNIP_NULL_PTR         0xFFFFFFFF

/*===========================================================================*/
/*                                 defines                                   */
/*===========================================================================*/


//#define EDDP_HWCMkSdtEntryValue(en, len, ptr)    (((en & 0x03)<<30) | ((len & 0x1FF)<<16) | ((ptr & 0xFFFF)<<0)) // En[2], Free[5], Len[9], Ptr[16]


/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                  data                                     */
/*===========================================================================*/

/*===========================================================================*/
/*                                  macros                                   */
/*===========================================================================*/

/*===========================================================================*/
/*                                prototyping                                */
/*===========================================================================*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// LowLevel access: write data to APICtrlRAM
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HWCWriteDataToRAM(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    //LSA_UINT32                  nIfaceIdx,
    LSA_UINT32                  nApiCtrlRamHostAddr,
    LSA_UINT8 *                 pBuffer,
    LSA_UINT16                  nWrLen);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// LowLevel access: read data from APICtrlRAM
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HWCReadDataFromRAM(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    //LSA_UINT32                  nIfaceIdx,
    LSA_UINT32                  nApiCtrlRamHostAddr,
    LSA_UINT8 *                 pBuffer,
    LSA_UINT16                  nRdLen);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// LowLevel specific access: write SDT entry
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HWCWriteSdtEntry(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  nApiCtrlSdtBaseHostAddr,
    LSA_UINT8                   nSdtIdx,
    LSA_UINT32                  nSdtValue);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// LowLevel specific access: read SDT entry
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HWCReadSdtEntry(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  nApiCtrlSdtBaseHostAddr,
    LSA_UINT8                   nSdtIdx,
    LSA_UINT32 *                pnSdtValue);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// load filtercode --> eddp_hwfiltercode[] into API-CtrlRAM and init FilterFragmentRegister
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HWCInitFilterCode(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT32                  nApiCtrlCodeHostAddr);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// reset statistic/error registers
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HWCSetRegisterToDefaults(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// init SDT with disabled entries
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HWCInitDisabledSdtEntries(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT32                  nApiCtrlSdtBaseHostAddr);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// init StoreEntries with Pattern
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HWC_InitStoreEntries(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  nApiCtrlStoreBaseHostAddr);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// load string and update Entry in SDT
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HWCUpdateSdtString(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  nApiCtrlSdtBaseHostAddr,
    LSA_UINT8                   nSdtIdx,
    LSA_UINT32                  nSdtValue,
    LSA_UINT32                  nApiCtrlSdtStringHostAddr,
    LSA_UINT8 *                 pString,
    LSA_UINT16                  nStrLen);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// read version of hwfiltercode
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HWCGetHWFilterVersion(
	LSA_UINT8 *     pDestBuffer,
	LSA_UINT8       nBufCnt);


/*****************************************************************************/
/*  end of file eddp_hwc.h                                                   */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of eddp_hwc_H */
