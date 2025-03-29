
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
/*  F i l e               &F: eddp_hwc.c                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP xxxx                                        */
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

#define LTRC_ACT_MODUL_ID  62
#define EDDP_MODULE_ID     LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_NRT_HWC */


/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"                               /* edd headerfiles */
#include "eddp_int.h"                               /* internal header */
#include "eddp_hwc.h"                               /* own header */


EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)


/*===========================================================================*/
/*                                local defines                              */
/*===========================================================================*/


#define SDT_ENTRY_SIZE      4                       // 32 bit, tbd:   --> eg global define

// *** simple guarding on compile time
#if (EDDP_SDT_IDX_LAST_USED > EDDP_SDT_IDX_MAX_POS )
    #error "SDT index overflow IN :fix ASM source code"
#endif
#if (EDDP_HWFC_SIZE > EDDP_HW_FILTER_MEM_OFFS )
    #error "missmatch 'EDDP_HWFC_SIZE' and 'EDDP_HW_FILTER_MEM_OFFS' in ASM file"
#endif




/*===========================================================================*/
/*                                global Variables                           */
/*===========================================================================*/


/*===========================================================================*/
/*                                external functions                         */
/*===========================================================================*/

/*===========================================================================*/
/*                                 functions                                 */
/*===========================================================================*/




/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_HWCWriteDataToRAM                      +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE pDDB                +*/
/*+                             LSA_UINT32              nIfaceIdx           +*/
/*+                             LSA_UINT32              nApiCtrlRamHostAddr +*/
/*+                             LSA_UINT8*              pBuffer             +*/
/*+                             LSA_UINT16              nWrLen              +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock           (in)  +*/
/*+  nIfaceIdx          : Interface Index (not used at this time)     (in)  +*/
/*+  nApiCtrlRamHostAddr: Destination addr for writing                (in)  +*/
/*+  pBuffer            : Write buffer                                (in)  +*/
/*+  nWrLen             : Nr of bytes to be written                   (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: LowLevel access: write data to APICtrlRAM                 +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HWCWriteDataToRAM(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    //LSA_UINT32                  nIfaceIdx,
    LSA_UINT32                  nApiCtrlRamHostAddr,
    LSA_UINT8 *                 pBuffer,
    LSA_UINT16                  nWrLen)
{
    //LSA_UINT32  *pDst = (LSA_UINT32*)pBuffer;
    //LSA_UINT16  nPos, nCpyLen = (nWrLen+3)/4;

    EDDP_ASSERT_NO_NULL_PTR(pDDB);

    EDDP_NRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_HWCWriteDataToRAM(pDDB=0x%X, RamAddr=0x%X, Buf=0x%X, Len=%d)",
        pDDB, nApiCtrlRamHostAddr, pBuffer, nWrLen);
    // *** write data byte code
    EDDP_MEMCOPY_COMMON((EDD_COMMON_MEM_PTR_TYPE)nApiCtrlRamHostAddr, pBuffer, nWrLen);
    // memcpy with 8bit access fixed in FPGA!!!
    /*
    for ( nPos=0; nPos < nCpyLen; nPos++ )
    {
        EDDP_HAL_MEM32_WRITE(pDDB, (LSA_UINT32*)nApiCtrlRamHostAddr, *pDst);
        pDst++;
        nApiCtrlRamHostAddr+=4;
    }
    */
    EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_HWCWriteDataToRAM(Len=%d)",
        nWrLen);

    //LSA_UNUSED_ARG(nIfaceIdx);    // handle unused parameter

    return (EDD_STS_OK);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_HWCReadDataFromRAM                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE pDDB                +*/
/*+                             LSA_UINT32              nIfaceIdx           +*/
/*+                             LSA_UINT32              nApiCtrlRamHostAddr +*/
/*+                             LSA_UINT8*              pBuffer             +*/
/*+                             LSA_UINT16              nRdLen              +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock           (in)  +*/
/*+  nIfaceIdx          : Interface Index (not used at this time)     (in)  +*/
/*+  nApiCtrlRamHostAddr: Source addr for reading                     (in)  +*/
/*+  pBuffer            : Read buffer                                 (in)  +*/
/*+  nWrLen             : Nr of bytes to be read                      (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: LowLevel access: read data from APICtrlRAM                +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HWCReadDataFromRAM(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    //LSA_UINT32                  nIfaceIdx,
    LSA_UINT32                  nApiCtrlRamHostAddr,
    LSA_UINT8 *                 pBuffer,
    LSA_UINT16                  nRdLen)
{
#if !defined EDDP_CFG_DEBUG_ASSERT
    LSA_UNUSED_ARG(pDDB);
#endif

    EDDP_ASSERT_NO_NULL_PTR(pDDB);

    //EDDP_NRT_TRACE_04(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT,"[H:--] IN: EDDP_HWCReadDataFromRAM(pDDB=0x%X, RamAddr=0x%X, Buf=0x%X, Len=%d)",
    //    pDDB, nApiCtrlRamAddr, pBuffer, nRdLen);

    // *** read data byte code
    EDDP_MEMCOPY_COMMON(pBuffer, (EDD_COMMON_MEM_PTR_TYPE)nApiCtrlRamHostAddr , nRdLen);

    //EDDP_NRT_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT,"[H:--] OUT: EDDP_HWCReadDataFromRAM(CodeLen=%d)", EDDP_HWFC_SIZE);

    //LSA_UNUSED_ARG(nIfaceIdx);    // handle unused parameter

    return (EDD_STS_OK);
}



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_HWCWriteSdtEntry                       +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE pDDB                +*/
/*+                             LSA_UINT32          nApiCtrlSdtBaseHostAddr +*/
/*+                             LSA_UINT8           nSdtIdx                 +*/
/*+                             LSA_UINT32          nSdtValue               +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB                : Pointer to DeviceDescriptionBlock          (in)  +*/
/*+  nApiCtrlSdtBaseHostAddr: Base addr of SDT                        (in)  +*/
/*+  nSdtIdx             : Selects an entry in SDT                    (in)  +*/
/*+  nSdtValue           : New SDT-Value (En, Len, StringPtr)         (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: LowLevel specific access: write SDT entry                 +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HWCWriteSdtEntry(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  nApiCtrlSdtBaseHostAddr,
    LSA_UINT8                   nSdtIdx,
    LSA_UINT32                  nSdtValue)
{
    EDD_COMMON_MEM_U32_PTR_TYPE     pEndAddr;

#if !defined EDDP_CFG_DEBUG_ASSERT
    LSA_UNUSED_ARG(pDDB);
#endif

    EDDP_ASSERT_NO_NULL_PTR(pDDB);
    pEndAddr = (EDD_COMMON_MEM_U32_PTR_TYPE)(nApiCtrlSdtBaseHostAddr + (nSdtIdx * SDT_ENTRY_SIZE));
    EDDP_HAL_MEM32_WRITE(pDDB, pEndAddr, nSdtValue);

    return (EDD_STS_OK);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_HWCReadSdtEntry                        +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE pDDB                +*/
/*+                             LSA_UINT32          nApiCtrlSdtBaseHostAddr +*/
/*+                             LSA_UINT8           nSdtIdx                 +*/
/*+                             LSA_UINT32*         pnSdtValue              +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB                : Pointer to DeviceDescriptionBlock          (in)  +*/
/*+  nApiCtrlSdtBaseHostAddr : Base addr of SDT                       (in)  +*/
/*+  nSdtIdx             : Selects an entry in SDT                    (in)  +*/
/*+  pnSdtValue          : Buffer for reading                         (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: LowLevel specific access: read SDT entry                  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HWCReadSdtEntry(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  nApiCtrlSdtBaseHostAddr,
    LSA_UINT8                   nSdtIdx,
    LSA_UINT32 *                pnSdtValue)
{
    EDD_COMMON_MEM_U32_PTR_TYPE     pEndAddr;

#if !defined EDDP_CFG_DEBUG_ASSERT
    LSA_UNUSED_ARG(pDDB);
#endif

    EDDP_ASSERT_NO_NULL_PTR(pDDB);
    pEndAddr = (EDD_COMMON_MEM_U32_PTR_TYPE)(nApiCtrlSdtBaseHostAddr + (nSdtIdx * SDT_ENTRY_SIZE));
    *pnSdtValue = EDDP_HAL_MEM32_READ(pDDB, pEndAddr);

    return (EDD_STS_OK);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_HWCInitFilterCode                      +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE pDDB                +*/
/*+                             LSA_UINT32              nApiCtrlCodeHostAddr+*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB                : Pointer to DeviceDescriptionBlock          (in)  +*/
/*+  nApiCtrlCodeHostAddr: Base addr of in ApiCtrlRAM for HWfilters   (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: load filtercode eddp_hwfiltercode[] into API-CtrlRAM      +*/
/*+  and init FilterFragmentRegister                                        +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HWCInitFilterCode(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  nApiCtrlCodeHostAddr)
{
    LSA_UINT32                  nApiCtrlCodeAddr;

    EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_HWCInitFilterCode(pDDB=0x%X, CodeAddr=0x%X)",
        pDDB, nApiCtrlCodeHostAddr);

    nApiCtrlCodeAddr = EDDP_HAL_H2P_ADDR(pDDB, nApiCtrlCodeHostAddr);

    // *** set fragment filter code addresses
    #if (EDDP_HWF_ALL_CODE_MASK != 0xC1FF)
        #error "check location of filter fragments!!!"
    #endif

    //-------------------------------------------------------------
    // EDDP_IF_A:
    //-------------------------------------------------------------
    // ***** copy HW filter code to IFA and set ASIC fragment registers

    /* There is a macro that checks                                                    */
    /* PNIP_R2_REG_FILTERCODEBASE_IFA_00 and PNIP_R3_REG_FILTERCODEBASE_IFA_00         */
    /* PNIP_R2_REG_FILTERCODEBASE_IFA_01 and PNIP_R3_REG_FILTERCODEBASE_IFA_01         */
    /* PNIP_R2_REG_FILTERCODEBASE_IFA_02 and PNIP_R3_REG_FILTERCODEBASE_IFA_02         */
    /* PNIP_R2_REG_FILTERCODEBASE_IFA_03 and PNIP_R3_REG_FILTERCODEBASE_IFA_03         */
    /* PNIP_R2_REG_FILTERCODEBASE_IFA_04 and PNIP_R3_REG_FILTERCODEBASE_IFA_04         */
    /* PNIP_R2_REG_FILTERCODEBASE_IFA_05 and PNIP_R3_REG_FILTERCODEBASE_IFA_05         */
    /* PNIP_R2_REG_FILTERCODEBASE_IFA_06 and PNIP_R3_REG_FILTERCODEBASE_IFA_06         */
    /* PNIP_R2_REG_FILTERCODEBASE_IFA_07 and PNIP_R3_REG_FILTERCODEBASE_IFA_07         */
    /* PNIP_R2_REG_FILTERCODEBASE_IFA_08 and PNIP_R3_REG_FILTERCODEBASE_IFA_08         */
    /* PNIP_R2_REG_FILTERCODEBASE_IFA_09 and PNIP_R3_REG_FILTERCODEBASE_IFA_09         */
    /* PNIP_R2_REG_FILTERCODEBASE_IFA_10 and PNIP_R3_REG_FILTERCODEBASE_IFA_10         */
    /* PNIP_R2_REG_FILTERCODEBASE_IFA_11 and PNIP_R3_REG_FILTERCODEBASE_IFA_11         */
    /* PNIP_R2_REG_FILTERCODEBASE_IFA_12 and PNIP_R3_REG_FILTERCODEBASE_IFA_12         */
    /* PNIP_R2_REG_FILTERCODEBASE_IFA_13 and PNIP_R3_REG_FILTERCODEBASE_IFA_13         */
    /* PNIP_R2_REG_FILTERCODEBASE_IFA_14 and PNIP_R3_REG_FILTERCODEBASE_IFA_14         */
    /* PNIP_R2_REG_FILTERCODEBASE_IFA_15 and PNIP_R3_REG_FILTERCODEBASE_IFA_15         */
    /* has the same value.                                                             */
    /* Checked in eddp_hal_pnip_val.h. We use the R2 version.                          */
    switch (EDDP_HALGetPnipRevision(pDDB))
    {
#if defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT) || defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
        case EDDP_HAL_HW_PNIP_REV1:
        case EDDP_HAL_HW_PNIP_REV2:
        {
            EDDP_HWCWriteDataToRAM(pDDB, nApiCtrlCodeHostAddr, &eddp_hwfiltercode[0], EDDP_HWFC_SIZE);

            EDDP_NRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] EDDP_HWCInitFilterCode(): set FilterFragments IFA");


            EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2_REG_FILTERCODEBASE_IFA_00, nApiCtrlCodeAddr+EDDP_API_TYPE_OTHER);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2_REG_FILTERCODEBASE_IFA_01, nApiCtrlCodeAddr+EDDP_API_TYPE_IP);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2_REG_FILTERCODEBASE_IFA_02, nApiCtrlCodeAddr+EDDP_API_TYPE_ARP);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2_REG_FILTERCODEBASE_IFA_03, nApiCtrlCodeAddr+EDDP_API_TYPE_MRP);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2_REG_FILTERCODEBASE_IFA_04, nApiCtrlCodeAddr+EDDP_API_TYPE_LLDP);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2_REG_FILTERCODEBASE_IFA_05, nApiCtrlCodeAddr+EDDP_API_TYPE_PN_DCP);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2_REG_FILTERCODEBASE_IFA_06, nApiCtrlCodeAddr+EDDP_API_TYPE_ALARM);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2_REG_FILTERCODEBASE_IFA_07, nApiCtrlCodeAddr+EDDP_API_TYPE_LEN);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2_REG_FILTERCODEBASE_IFA_08, nApiCtrlCodeAddr+EDDP_API_TYPE_PTCP);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2_REG_FILTERCODEBASE_IFA_09, EDDP_PNIP_NULL_PTR);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2_REG_FILTERCODEBASE_IFA_10, EDDP_PNIP_NULL_PTR);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2_REG_FILTERCODEBASE_IFA_11, EDDP_PNIP_NULL_PTR);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2_REG_FILTERCODEBASE_IFA_12, EDDP_PNIP_NULL_PTR);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2_REG_FILTERCODEBASE_IFA_13, EDDP_PNIP_NULL_PTR);
            // special filter (for Queue 14/15) to detect wrong switching of RT-frames into NRT instead into CRT
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2_REG_FILTERCODEBASE_IFA_14, nApiCtrlCodeAddr+EDDP_API_TYPE_RT_CT);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2_REG_FILTERCODEBASE_IFA_15, nApiCtrlCodeAddr+EDDP_API_TYPE_RT_SF);

            EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_HWCInitFilterCode(CodeLen=%d)", EDDP_HWFC_SIZE);
        }
        break;
#endif
        default:
        {
            EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HWCInitFilterCode(): FATAL -> Version(%d) of PNIP is unknown",
                EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }

    return (EDD_STS_OK);
}



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_HWCSetRegisterToDefaults               +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE pDDB                +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB             : Pointer to DeviceDescriptionBlock             (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: reset statistic/error registers                           +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HWCSetRegisterToDefaults(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT8   idx;

    EDDP_ASSERT_NO_NULL_PTR(pDDB);

    EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_HWCSetRegisterToDefaults(pDDB=0x%X)", pDDB);

    // *** clr old Errors
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_FILTERERRSTATUS_IFA, PNIP_REG_FILTERERREVENT_IFA__MSK_ERREVENT);
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_FILTERERREVENT_IFA,  PNIP_REG_FILTERERREVENT_IFA__MSK_ERREVENT);
    // *** disable LOG feature
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_LOGCONTROL_IFA, 0);
    // *** clr DROP counter
    for (idx =0; idx < 16*4; idx+=4)
    {
        /* There is a macro that checks PNIP_R2_REG_DROPCOUNT_IFA_00 and PNIP_R3_REG_DROPCOUNT_IFA_00 has the same value. */
        /* Checked in eddp_hal_pnip_val.h. We use the R2 version. */
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2_REG_DROPCOUNT_IFA_00+idx, 0);
    }

    // EDDP_HAL_REG32_WRITE(pDDB, FilterEnabled_IFA, 1);
    EDDP_NRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_HWCSetRegisterToDefaults()");

    return (EDD_STS_OK);
}



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_HWCInitDisabledSdtEntries              +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE pDDB                +*/
/*+                             LSA_UINT32          nApiCtrlSdtHostBaseAddr +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB                    : Pointer to DeviceDescriptionBlock      (in)  +*/
/*+  nApiCtrlSdtHostBaseAddr : Base addr of SDT                       (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: init SDT with disabled entries                            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HWCInitDisabledSdtEntries(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  nApiCtrlSdtBaseHostAddr)
{
    LSA_UINT8   uc_tmp = 0;

    EDDP_ASSERT_NO_NULL_PTR(pDDB);

    EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_HWCInitDisabledSdtEntries(pDDB=0x%X, SdtBaseAddr=0x%X)",
        pDDB, nApiCtrlSdtBaseHostAddr);

    switch (EDDP_HALGetPnipRevision(pDDB))
    {
#if defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT) || defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
        case EDDP_HAL_HW_PNIP_REV1:
        case EDDP_HAL_HW_PNIP_REV2:
        {
            // *** set all SDT entries disabled, but ARP to wildcard match
            for ( uc_tmp = 0; uc_tmp <= EDDP_SDT_IDX_MAX_POS; uc_tmp++ )
            {
                if (uc_tmp == EDDP_SDT_IDX_ARP_IP)
                {
                    EDDP_HWCWriteSdtEntry(pDDB, nApiCtrlSdtBaseHostAddr, uc_tmp, PNIP_FILTER_STRING_DESCR_DEF_ON);  // StringDescriptor set to "Wildcard match"
                }
                else
                {
                    EDDP_HWCWriteSdtEntry(pDDB, nApiCtrlSdtBaseHostAddr, uc_tmp, PNIP_FILTER_STRING_DESCR_DEF_OFF); // StringDescriptor set to "Wildcard missmatch"
                }
            }
        }
        break;
#endif
        default:
        {
            EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HWCInitDisabledSdtEntries(): FATAL -> Version(%d) of PNIP is unknown",
                EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }

    EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_HWCInitDisabledSdtEntries(%d SdtItems disabled)", uc_tmp);

    return (EDD_STS_OK);
}

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HWC_InitStoreEntries(EDDP_LOCAL_DDB_PTR_TYPE pDDB, LSA_UINT32 nApiCtrlStoreBaseHostAddr)
{
    EDD_RSP Response = EDD_STS_OK;
    LSA_UINT32 i;

    EDDP_ASSERT_NO_NULL_PTR(pDDB);

    switch (EDDP_HALGetPnipRevision(pDDB))
    {
#if defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT) || defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
        case EDDP_HAL_HW_PNIP_REV1:
        case EDDP_HAL_HW_PNIP_REV2:
            for(i = 0; i < EDDP_HW_FILTER_MEM_SIZE/2; i++)
            {
                EDD_COMMON_MEM_U16_PTR_TYPE pEndAddr = (EDD_COMMON_MEM_U16_PTR_TYPE)(nApiCtrlStoreBaseHostAddr + (i * sizeof(LSA_UINT16)));
                if(i%2)
                    EDDP_HAL_MEM16_WRITE(pDDB, pEndAddr, 0xEFBE);
                else
                    EDDP_HAL_MEM16_WRITE(pDDB, pEndAddr, 0xADDE);
            }
            break;
#endif
            default:
                EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_HWCInitDisabledSdtEntries(): FATAL -> Version(%d) of PNIP is unknown",
                    EDDP_HALGetPnipRevision(pDDB));
                //EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                Response = EDD_STS_ERR_PARAM;
                break;
    }

    return Response;
}



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :  EDDP_HWCUpdateSdtString                       +*/
/*+  Input/Output          :  EDDP_LOCAL_DDB_PTR_TYPE pDDB                  +*/
/*+                           LSA_UINT32          nApiCtrlSdtHostBaseAddr   +*/
/*+                           LSA_UINT8           nSdtIdx                   +*/
/*+                           LSA_UINT32          nSdtValue                 +*/
/*+                           LSA_UINT32          nApiCtrlSdtHostStringAddr +*/
/*+                           LSA_UINT8*          pString                   +*/
/*+                           LSA_UINT16          nStrLen                   +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB                  : Pointer to DeviceDescriptionBlock        (in)  +*/
/*+  nApiCtrlSdtHostBaseAddr: Base addr of SDT                        (in)  +*/
/*+  nSdtIdx               : Selects an entry in SDT                  (in)  +*/
/*+  nSdtValue             : New SDT-Value (En, Len, StringPtr)       (in)  +*/
/*+  nApiCtrlSdtHostStringAddr: Addr for string in ApiCtrlRAM         (in)  +*/
/*+  pString               : String buffer                            (in)  +*/
/*+  nStrLen               : Nr of bytes to be written                (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: load string and update Entry in SDT (eg. DCP_HELLO, ARP)  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HWCUpdateSdtString(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  nApiCtrlSdtBaseHostAddr,
    LSA_UINT8                   nSdtIdx,
    LSA_UINT32                  nSdtValue,
    LSA_UINT32                  nApiCtrlSdtStringHostAddr,
    LSA_UINT8 *                 pString,
    LSA_UINT16                  nStrLen)
{
    EDDP_ASSERT_NO_NULL_PTR(pDDB);

    EDDP_NRT_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_HWCUpdateSdtString(pDDB=0x%X, SdtBaseAddr=0x%X, Idx=%d, Val=0x%X, StrAddr=0x%X, Len=%d)",
        pDDB, nApiCtrlSdtBaseHostAddr, nSdtIdx, nSdtValue, nApiCtrlSdtStringHostAddr, nStrLen);

    //EDDP_MEMCOPY_COMMON((EDD_COMMON_MEM_PTR_TYPE)nApiCtrlSdtStringHostAddr, pString, nStrLen);
    EDDP_HWCWriteDataToRAM(pDDB, nApiCtrlSdtStringHostAddr, pString, nStrLen);

    EDDP_HWCWriteSdtEntry(pDDB, nApiCtrlSdtBaseHostAddr, nSdtIdx, nSdtValue);

    EDDP_NRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_HWCUpdateSdtString()");

    return (EDD_STS_OK);
}




/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :  EDDP_HWCGetHWFilterVersion                    +*/
/*+  Input/Output          :  LSA_UINT8*              pDestBuffer           +*/
/*+                           LSA_UINT8               nBufCnt               +*/
/*+                                                                         +*/
/*+  Result                :  EDD_RSP                                       +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDestBuffer           : String buffer for reading                (in)  +*/
/*+  nBufCnt               : Size of pDestBuffer                      (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: read version of hwfiltercode --> must be zero terminated  +*/
/*+  string in ASM code !!!                                                 +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HWCGetHWFilterVersion(
    LSA_UINT8 *     pDestBuffer,
    LSA_UINT8       nBufCnt)
{
    if (pDestBuffer && (nBufCnt >= EDDP_HW_FILTER_VER_SIZE))
    {
        EDDP_MEMCOPY_COMMON((EDD_COMMON_MEM_PTR_TYPE)pDestBuffer, &eddp_hwfiltercode[EDDP_HW_FILTER_VER], EDDP_HW_FILTER_VER_SIZE);
        return EDD_STS_OK;
    }
    return (EDD_STS_ERR_PARAM);
}

/*****************************************************************************/
/*  end of file eddp_hwc.c                                                   */
/*****************************************************************************/

