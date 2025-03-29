#ifndef EDDP_HAL_USR_H                   /* ----- reinclude-protection ----- */
#define EDDP_HAL_USR_H

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
/*  F i l e               &F: eddp_hal_usr.h                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Internal headerfile for debugging                                        */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  20.10.08    SF    initial version.                                       */
#endif
/*****************************************************************************/


/*

EDDP-View                                                         PNIP View
                                PNIP Memory 2MB
                                *---------------*
                                :               :
                                :               :
                                |---------------|
                                | ApiCtrl-RAM   |
                                |     *-------* |
                                |     :       : |
                                |     :       : |
                                |     *-------* |
              EDDP PNIP Address |     |       | |
                        --------|-----| SDT   |-|----------------------------------------------
                                |     |       | |       A                               A
EDDP_HALGetRAMSubAddress()----------->*-------* |       |                               |
                                |     |       | |       |                               |
                                |     | Code  | |  EDDP_HALGetPnipRAMOffsetToRAM()      |
                                |     |       | |       |                               |
EDDP_HALGetRAMBaseAddress()---->|-----*-------*-|<--------------                        |
                                |               |                                       |
                                :               :                                       |
                                :               :                                       |
                                |---------------|                                       |
                                |   CMD-RAM     |                                       |
                                |---------------|                       EDDP_HALGetPnipOffsetToStart()
                                :               :                                       |
                                :               :                                       |
                                |---------------|                                       |
                                | Statistic-RAM |                                       |
                                |---------------|                                       |
                                :               :                                       |
                                :               :                                       |
EDDP_HALGetPnipBaseAddress()--->*---------------*<-- 0 ----------------------------------------


*/


/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/

// special return for: EDDP_HALGetPnipOffsetToStart(), EDDP_HALGetPnipRAMOffsetToRAM()
#define EDDP_HAL_RAM_BAD_PNIP_ADDR      0xFFFFFFFF

// specific signature for overwrite scan
#define EDDP_HAL_BORDER_TOKEN           0xAA55CCDD



/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* placeholder structure for RT commands which use extra paramater blocks    */
/* --> used with Rev3                                                        */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_HAL_API_RT_CTRL_COMMON_CMD_ARG_TYPE    EDD_COMMON_MEM_ATTR *   EDDP_HAL_API_RT_CTRL_COMMON_CMD_ARG_PTR_TYPE;
typedef struct _EDDP_HAL_API_RT_CTRL_COMMON_CMD_ARG_TYPE
{
    LSA_UINT32      ExternalCmdRam_ApiCtrlRam[EDDP_CMD_MAX_ACW_CNT_PER_CMD_REV3*2];
    /* Note: External command param is 64 bit, so factor 2 is needed                                            */
} EDDP_HAL_API_RT_CTRL_COMMON_CMD_ARG_TYPE;
/* ExternalCmdRam_ApiCtrlRam     : for PpmAcwInOut                                                              */

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* NRT structure definition for HAL-module                                   */
/* three parts: common, always, optional                                     */
/* Use64Aligned with beginning of every part                                 */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#define EDDP_HAL_NRT_HWFC_RESERVED_SIZE     (EDDP_HW_FILTER_MEM_OFFS)   // reserved RAM (including reserved bytes)


#if (EDDP_HWFC_SIZE > EDDP_HAL_NRT_HWFC_RESERVED_SIZE)  // simple test for RAM overflow
    #error "overflow memory space for HW filter"
#endif


/*---------------------------------------------------------------------------*/
/* Common for IFA/B: HW FilterCode                                           */
/*---------------------------------------------------------------------------*/

typedef struct _EDDP_HAL_APICTRL_NRT_COMMON_TYPE    EDDP_LOCAL_MEM_ATTR *   EDDP_HAL_LOCAL_APICTRL_NRT_COMMON_PTR_TYPE;
typedef struct _EDDP_HAL_APICTRL_NRT_COMMON_TYPE
{
    LSA_UINT8       ApiCtrlRamCodeMem[EDDP_HAL_NRT_HWFC_RESERVED_SIZE];     // for FilterCode shared for IFA/B
} EDDP_HAL_APICTRL_NRT_COMMON_TYPE;



/*---------------------------------------------------------------------------*/
/* always for IFA/B: RcvDmacw, SndDmacw, RamStore, RamSdt                    */
/*---------------------------------------------------------------------------*/

#define EDDP_FILTER_ARP_SIZE                4       // holds an ip address       (4 byte)
#define EDDP_FILTER_DCP_SIZE                (EDD_DCP_MAX_SET_DCP_FILTER_TLV_LEN + 1)  // holds an DCP string  (AliasName: 259 byte = TypeHi, TypeLo, LenHi, LenLo (1+1+1+1 Byte) NoS (240 byte) + seperator '.' (1 byte) + Port Name (12 byte)); Plus 1 Byte for 4 Byte Alignment
#define EDDP_FILTER_DCP_FULL_SIZE           (EDDP_FILTER_DCP_SIZE+4) // in HERA APIPstTable the Offset for Type is 264 Byte
#define EDDP_FILTER_DCPHELLO_LEN_SIZE       2       // holds 16Bit length of DCP-HELLO string (2 byte)
#define EDDP_FILTER_DCPHELLO_STR_SIZE       (EDD_DCP_MAX_DCP_HELLO_NAME_OF_STATION) // holds an DCP-HELLO string (240 byte)
#define EDDP_FILTER_DCPHELLO_STR_OFFS       4       // 2 bytes of LEN and 2 padding bytes for 32bit alignment
#define EDDP_FILTER_DCPHELLO_FULL_SIZE      (EDD_DCP_MAX_DCP_HELLO_NAME_OF_STATION + 4)     // holds: 16Bit length, 2 padding and DCP-HELLO string (Type (2 byte) + Length (2 byte) + NoS(240 byte))

typedef struct _EDDP_HAL_APICTRL_NRT_IF_MGM_TYPE        EDDP_LOCAL_MEM_ATTR *    EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE;
typedef struct _EDDP_HAL_APICTRL_NRT_IF_MGM_TYPE
{
    PNIP_DMACW_RCV_DESCR_TYPE   ApiCtrlRamRcvDmacw[EDDP_NRT_RX_DMACW_CNT];      // IFA/B with 32 RX entries a 64 bit
    PNIP_DMACW_SND_DESCR_TYPE   ApiCtrlRamSndDmacw[EDDP_NRT_TX_DMACW_CNT];      // IFA/B with 16 TX entries a 64 bit
    LSA_UINT16                  ApiCtrlRamStore[EDDP_HW_FILTER_MEM_SIZE/2];     // for StoreMemory IFA/B used in Filtercode by Idx
    LSA_UINT32                  ApiCtrlRamSdt[EDDP_SDT_SIZE/4];                 // for StringDescriptorTable IFA/B --> ControlEntry for SCMP opcode
    LSA_UINT8                   ApiCtrlRamArpIf[EDDP_FILTER_ARP_SIZE];          // Arp: one IP address (4byte) for interface
} EDDP_HAL_APICTRL_NRT_IF_MGM_TYPE;


/*---------------------------------------------------------------------------*/
/* filter strings for IFace: Arp, Dcp                                        */
/*---------------------------------------------------------------------------*/
//typedef struct _EDDP_HAL_APICTRL_NRT_IF_DCP_FILTER_TYPE    EDDP_LOCAL_MEM_ATTR *    EDDP_HAL_LOCAL_APICTRL_NRT_IF_DCP_FILTER_PTR_TYPE;
#define EDDP_APIPSTRAM_PSTABLE_SIZE 0x2000                  // 16 * 512Byte = 8KiB
typedef struct _EDDP_HAL_APIPST_DCP_FILTER_TABLE_TYPE
{
    LSA_UINT8       ApiPstRamPSTable[EDDP_APIPSTRAM_PSTABLE_SIZE];    // Dcp: Entry for interface and entries for ports
} EDDP_HAL_APIPSTRAM_PSTABLE_TYPE;

typedef struct _EDDP_HAL_APICTRL_NRT_IF_DCP_FILTER_TYPE
{
    LSA_UINT8       ApiCtrlRamDcp[EDDP_FILTER_DCP_SIZE];    // Dcp: Entry for interface and entries for ports
} EDDP_HAL_APICTRL_NRT_IF_DCP_FILTER_TYPE;


/*---------------------------------------------------------------------------*/
/* optional for IFace: Dcp-Hello-String                                      */
/*---------------------------------------------------------------------------*/
//typedef struct _EDDP_HAL_APICTRL_NRT_IF_DCPHELLO_FILTER_TYPE    EDDP_LOCAL_MEM_ATTR *    EDDP_HAL_LOCAL_APICTRL_NRT_IF_DCPHELLO_FILTER_PTR_TYPE;
typedef struct _EDDP_HAL_APICTRL_NRT_IF_DCPHELLO_FILTER_TYPE
{
    LSA_UINT8       ApiCtrlRamDcpHello[EDDP_FILTER_DCPHELLO_FULL_SIZE];     // DcpHello used in Controller only
} EDDP_HAL_APICTRL_NRT_IF_DCPHELLO_FILTER_TYPE;





/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* CRT structure definition for HAL-module                                   */
/* Use 64 Aligned only to begin of first element in struct                   */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/* -------------------------------- */
/* DDB specific container           */
/* -------------------------------- */
// ACW Array Provider
typedef struct _EDDP_HAL_APICTRL_CRT_PROV_ACW
{
    // here only 1 ACW: all ACWs are put in _EDDP_HALInitApiCtrlRam()
    PNIP_ACW_SND_TYPE           AcwArray[1];
} EDDP_HAL_APICTRL_CRT_PROV_ACW;

// ACW_DG Array Provider
typedef struct _EDDP_HAL_APICTRL_CRT_PROV_ACW_DG
{
    // here only 1 ACW_DG: all ACW_DGs are put in _EDDP_HALInitApiCtrlRam()
    PNIP_ACWDG_SND_TYPE         AcwDGArray[1];
} EDDP_HAL_APICTRL_CRT_PROV_ACW_DG;

// ACW Array Consumer
typedef struct _EDDP_HAL_APICTRL_CRT_CONS_ACW
{
    // here only 1 ACW: all ACWs are put in _EDDP_HALInitApiCtrlRam()
    PNIP_ACW_RCV_TYPE           AcwArray[1];
} EDDP_HAL_APICTRL_CRT_CONS_ACW;

// ACW APDU Consumer status
typedef struct _EDDP_HAL_APICTRL_CRT_CONS_APDU
{
    // here only 1 entry: all entries are put in _EDDP_HALInitApiCtrlRam()
    PNIP_ADPU_RCV_TYPE          ApduArray[1];
} EDDP_HAL_APICTRL_CRT_CONS_APDU;

// ACW_DG Array Consumer
typedef struct _EDDP_HAL_APICTRL_CRT_CONS_ACW_DG
{
    // here only 1 ACW: all ACWs are put in _EDDP_HALInitApiCtrlRam()
    PNIP_ACW_DG_RCV_TYPE         AcwDgArray[1];
} EDDP_HAL_APICTRL_CRT_CONS_ACW_DG;

// ACW DG Consumer status
typedef struct _EDDP_HAL_APICTRL_CRT_CONS_DG_STATUS
{
    // here only 1 entry: all entries are put in _EDDP_HALInitApiCtrlRam()
    PNIP_DG_STATUS_RCV_TYPE      DgStatusArray[1];
} EDDP_HAL_APICTRL_CRT_CONS_DG_STATUS;

/* -------------------------------- */
/* DDB specific container - IFA     */
/* -------------------------------- */
// TBase Array
typedef struct _EDDP_HAL_APICTRL_CRT_TBASE_ARRAY
{
    PNIP_ACW_SND_TBASE_TYPE     TBaseArray[EDDP_CRT_MAX_COUNT_TBASE];
} EDDP_HAL_APICTRL_CRT_TBASE_ARRAY;

// Trog Array
typedef struct _EDDP_HAL_APICTRL_CRT_TROG_ARRAY
{
    // here only 1 Trog: all Trogs are put in _EDDP_HALInitApiCtrlRam()
    PNIP_ACW_SND_TROG_TYPE      TrogArray[1];
} EDDP_HAL_APICTRL_CRT_TROG_ARRAY;

// DataStatus Array PNIP Revision 1
#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
typedef struct _EDDP_HAL_PNIP_R1_APICTRL_CRT_DS_ARRAY
{
    // here only 1 DataStatus: all DataStatus are put in _EDDP_HALInitApiCtrlRam()
    PNIP_R1_ACW_SND_DS_TYPE        DataStatusArray[1];
} EDDP_HAL_PNIP_R1_APICTRL_CRT_DS_ARRAY;
#endif

// DataStatus Array PNIP Revision 2 and 3
#if defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
typedef struct _EDDP_HAL_PNIP_R2_APICTRL_CRT_DS_ARRAY
{
    // here only 1 DataStatus: all DataStatus are put in _EDDP_HALInitApiCtrlRam()
    PNIP_R2_ACW_SND_DS_TYPE        DataStatusArray[1];
} EDDP_HAL_PNIP_R2_APICTRL_CRT_DS_ARRAY;
#endif

// RBase Array
typedef struct _EDDP_HAL_APICTRL_CRT_RBASE_ARRAY
{
    PNIP_ACW_RCV_RBASE_TYPE     RBaseArray[EDDP_CRT_MAX_COUNT_RBASE];
} EDDP_HAL_APICTRL_CRT_RBASE_ARRAY;

typedef struct _EDDP_HAL_APICTRL_CRT_CONS_TSB
{
    // here only 1 entry: all entries are put in _EDDP_HALInitApiCtrlRam()
    PNIP_CPM_TSB_TYPE      TsbArray[1];
} EDDP_HAL_APICTRL_CRT_CONS_TSB;

typedef struct _EDDP_HAL_APICTRL_CRT_CONS_CSB
{
    // here only 1 entry: all entries are put in _EDDP_HALInitApiCtrlRam()
    PNIP_CPM_CSB_TYPE      CsbArray[1];
} EDDP_HAL_APICTRL_CRT_CONS_CSB;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* FDB-Table structure definition for HAL-module                             */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_HAL_RAMSUB_FDB_TABLE_PARAM_STRUCT_ERTEC200P_TYPE
{
	LSA_UINT64				Param_User_ID_0_0;
	LSA_UINT64				Param_User_ID_0_1;
	LSA_UINT64				Param_User_ID_1_0;
	LSA_UINT64				Param_User_ID_1_1;
	LSA_UINT64				Param_User_ID_2_0;
	LSA_UINT64				Param_User_ID_2_1;
	LSA_UINT64				Param_User_ID_3_0;
	LSA_UINT64				Param_User_ID_3_1;
	LSA_UINT64				Param_User_ID_4_0;
	LSA_UINT64				Param_User_ID_4_1;
	LSA_UINT64				Param_User_ID_5_0;
	LSA_UINT64				Param_User_ID_5_1;
	LSA_UINT64				Param_User_ID_6_0;
	LSA_UINT64				Param_User_ID_6_1;
	LSA_UINT64				Param_User_ID_7_0;
	LSA_UINT64				Param_User_ID_7_1;
} EDDP_HAL_RAMSUB_FDB_TABLE_PARAM_STRUCT_ERTEC200P_TYPE;

typedef struct _EDDP_HAL_RAMSUB_FDB_TABLE_MC_TABLE_A_TYPE
{
	LSA_UINT32      MC_Table_A [EDDP_SWI_FDB_SIZE_OF_MC_TABLE_32BIT];
} EDDP_HAL_RAMSUB_FDB_TABLE_MC_TABLE_A_TYPE;

typedef struct _EDDP_HAL_RAMSUB_FDB_TABLE_MC_TABLE_B_TYPE
{
	LSA_UINT32		MC_Table_B [EDDP_SWI_FDB_SIZE_OF_MC_TABLE_32BIT];
} EDDP_HAL_RAMSUB_FDB_TABLE_MC_TABLE_B_TYPE;

typedef struct _EDDP_HAL_RAMSUB_FDB_TABLE_UC_TABLE_A_ERTEC200P_TYPE
{
	LSA_UINT32		UC_Table_A [EDDP_SWI_FDB_SIZE_OF_UC_TABLE_A_32BIT_ERTEC_200P];
} EDDP_HAL_RAMSUB_FDB_TABLE_UC_TABLE_A_ERTEC200P_TYPE;

typedef struct _EDDP_HAL_RAMSUB_FDB_TABLE_IRT_TABLE_A_TYPE
{
	LSA_UINT32		IRT_Table_A [EDDP_SWI_FDB_SIZE_OF_IRT_TABLE_A_32BIT];
} EDDP_HAL_RAMSUB_FDB_TABLE_IRT_TABLE_A_TYPE;




/*===========================================================================*/
/*                                 macros                                    */
/*===========================================================================*/


/*===========================================================================*/
/*  Conversion of PNIP (little endian) from/to Host byte order               */
/*===========================================================================*/

#ifdef EDDP_CFG_BIG_ENDIAN

    // Host To PNIP
    #define EDDP_HAL_H2P_64(Value)  ( EDDP_SWAP64(Value) )
    #define EDDP_HAL_H2P_32(Value)  ( EDDP_SWAP32(Value) )
    #define EDDP_HAL_H2P_16(Value)  ( EDDP_SWAP16(Value) )

    // PNIP To Host
    #define EDDP_HAL_P2H_64(Value)  ( EDDP_SWAP64(Value) )
    #define EDDP_HAL_P2H_32(Value)  ( EDDP_SWAP32(Value) )
    #define EDDP_HAL_P2H_16(Value)  ( EDDP_SWAP16(Value) )

#else

    // Host To PNIP
    #define EDDP_HAL_H2P_64(Value)  (Value)
    #define EDDP_HAL_H2P_32(Value)  (Value)
    #define EDDP_HAL_H2P_16(Value)  (Value)

    // PNIP To Host
    #define EDDP_HAL_P2H_64(Value)  (Value)
    #define EDDP_HAL_P2H_32(Value)  (Value)
    #define EDDP_HAL_P2H_16(Value)  (Value)

#endif



/*===========================================================================*/
/*  macro for mapping of view Host <---> PNIP                                */
/*===========================================================================*/

// input: Host-addr, output: PNIP-addr
#define EDDP_HAL_H2P_ADDR(pDDB, HostAddr)   (LSA_UINT32)((LSA_UINT32)HostAddr - (LSA_UINT32)pDDB->pPnipBase)    // PNIP start @ 0
// input: PNIP-addr, output: Host-addr
#define EDDP_HAL_P2H_ADDR(pDDB, PnipAddr)   (LSA_UINT32)((LSA_UINT32)PnipAddr + (LSA_UINT32)pDDB->pPnipBase)    // Host needs offset


#if(PNIOD_PLATFORM & PNIOD_PLATFORM_RZT2_EDDP)
 #define EDDP_USE_K32_DLL

extern  LSA_UINT32 PNIP_REG32_READ (LSA_UINT32, volatile EDD_COMMON_MEM_U32_PTR_TYPE);
extern  LSA_VOID   PNIP_REG32_WRITE(LSA_UINT32, volatile EDD_COMMON_MEM_U32_PTR_TYPE, LSA_UINT32);

extern  LSA_UINT32 PNIP_REG64_READ (LSA_UINT32, volatile EDD_COMMON_MEM_U64_PTR_TYPE);
extern  LSA_VOID   PNIP_REG64_WRITE(LSA_UINT32, volatile EDD_COMMON_MEM_U64_PTR_TYPE, LSA_UINT64);

//EDDP_MEM32_WRITE

#define EDDP_REG32_READ		PNIP_REG32_READ
#define EDDP_REG32_WRITE	PNIP_REG32_WRITE

#endif

/*===========================================================================*/
/*  lowlevel access macros for HW                                            */
/*===========================================================================*/

// HW access with macros for higher execution speed

// RD 32bit Register,               eg: EDDP_HAL_REG32_READ(pDDB, PNIP_REG_IP_VERSION)
#ifndef EDDP_REG32_READ
#define EDDP_HAL_REG32_READ(pDDB, RegOffs)          \
            EDDP_HAL_P2H_32(*((volatile EDD_COMMON_MEM_U32_PTR_TYPE)(((volatile EDD_COMMON_MEM_U8_PTR_TYPE)(pDDB->pPnipBase)) + (RegOffs))))
#else
#define EDDP_HAL_REG32_READ(pDDB, RegOffs)          \
    EDDP_REG32_READ(pDDB->hSysDev,((volatile EDD_COMMON_MEM_U32_PTR_TYPE)(((volatile EDD_COMMON_MEM_U8_PTR_TYPE)(pDDB->pPnipBase)) + (RegOffs))))
#endif

// WR 32bit Register,               eg: EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_FILTERENABLED_IFA, 0x01234567)
#ifndef EDDP_REG32_WRITE
#define EDDP_HAL_REG32_WRITE(pDDB, RegOffs, Value)  \
            *((volatile EDD_COMMON_MEM_U32_PTR_TYPE)((volatile EDD_COMMON_MEM_U8_PTR_TYPE)(pDDB->pPnipBase) + (RegOffs))) = EDDP_HAL_H2P_32((LSA_UINT32)(Value))
#else
#define EDDP_HAL_REG32_WRITE(pDDB, RegOffs, Value)  \
        EDDP_REG32_WRITE(pDDB->hSysDev,((volatile EDD_COMMON_MEM_U32_PTR_TYPE)((volatile EDD_COMMON_MEM_U8_PTR_TYPE)(pDDB->pPnipBase) + (RegOffs))), Value)
#endif

// RD 32bit Memory,                 eg: EDDP_HAL_MEM32_READ(pDDB, (EDD_COMMON_MEM_U32_PTR_TYPE)((volatile EDD_COMMON_MEM_U8_PTR_TYPE)(pDDB->pPnipBase) + PNIP_RAM_APICTRL_START))
#ifndef EDDP_MEM32_READ
#define EDDP_HAL_MEM32_READ(pDDB, pMem)             \
            EDDP_HAL_P2H_32(*((volatile EDD_COMMON_MEM_U32_PTR_TYPE)(pMem)))
#else
#define EDDP_HAL_MEM32_READ(pDDB, pMem)             \
        EDDP_MEM32_READ(pDDB->hSysDev,(volatile EDD_COMMON_MEM_U32_PTR_TYPE)(pMem))
#endif

// WR 32bit Memory,                 eg: EDDP_HAL_MEM32_WRITE(pDDB, (EDD_COMMON_MEM_U32_PTR_TYPE)((volatile EDD_COMMON_MEM_U8_PTR_TYPE)(pDDB->pPnipBase) + PNIP_RAM_APICTRL_START), 0x01234567)
#ifndef EDDP_MEM32_WRITE
#define EDDP_HAL_MEM32_WRITE(pDDB, pMem, Value)     \
            *((volatile EDD_COMMON_MEM_U32_PTR_TYPE)(pMem)) = EDDP_HAL_H2P_32((LSA_UINT32)(Value))
#else
#define EDDP_HAL_MEM32_WRITE(pDDB, pMem, Value)     \
        EDDP_MEM32_WRITE(pDDB->hSysDev,(volatile EDD_COMMON_MEM_U32_PTR_TYPE)(pMem),Value)
#endif

// RD 64bit Memory,                 eg: EDDP_HAL_MEM64_READ(pDDB, (EDD_COMMON_MEM_U64_PTR_TYPE)((volatile EDD_COMMON_MEM_U8_PTR_TYPE)(pDDB->pPnipBase) + PNIP_RAM_APICTRL_START))
#ifndef EDDP_MEM64_READ
#define EDDP_HAL_MEM64_READ(pDDB, pMem)             \
            EDDP_HAL_P2H_64(*((volatile EDD_COMMON_MEM_U64_PTR_TYPE)(pMem)))
#else
#define EDDP_HAL_MEM64_READ(pDDB, pMem)             \
        EDDP_MEM64_READ(pDDB->hSysDev,(volatile EDD_COMMON_MEM_U64_PTR_TYPE)(pMem))
#endif

// WR 64bit Memory,                 eg: EDDP_HAL_MEM64_WRITE(pDDB, (EDD_COMMON_MEM_U64_PTR_TYPE)((volatile EDD_COMMON_MEM_U8_PTR_TYPE)(pDDB->pPnipBase) + PNIP_RAM_APICTRL_START), 0x01234567)
#ifndef EDDP_MEM64_WRITE
#define EDDP_HAL_MEM64_WRITE(pDDB, pMem, Value)     \
            *((volatile EDD_COMMON_MEM_U64_PTR_TYPE)(pMem)) = EDDP_HAL_H2P_64((LSA_UINT64)(Value))
#else
#define EDDP_HAL_MEM64_WRITE(pDDB, pMem, Value)     \
        EDDP_MEM64_WRITE(pDDB->hSysDev,(volatile EDD_COMMON_MEM_U64_PTR_TYPE)(pMem),Value)
#endif

// RD 16bit Memory,                     eg: EDDP_HAL_MEM16_READ(pDDB, (EDD_COMMON_MEM_U8_PTR_TYPE)((volatile EDD_COMMON_MEM_U8_PTR_TYPE)(pDDB->pPnipBase) + PNIP_RAM_APICTRL_START))
#ifndef EDDP_MEM16_READ
#define EDDP_HAL_MEM16_READ(pDDB, pMem)             \
            EDDP_HAL_P2H_16(*((volatile EDD_COMMON_MEM_U16_PTR_TYPE)(pMem)))
#else
#define EDDP_HAL_MEM16_READ(pDDB, pMem)             \
        EDDP_MEM16_READ(pDDB->hSysDev,(volatile EDD_COMMON_MEM_U16_PTR_TYPE)(pMem))
#endif

// WR 16bit Memory,                     eg: EDDP_HAL_MEM16_WRITE(pDDB, (EDD_COMMON_MEM_U8_PTR_TYPE)((volatile EDD_COMMON_MEM_U8_PTR_TYPE)(pDDB->pPnipBase) + PNIP_RAM_APICTRL_START), 0x0123)
#ifndef EDDP_MEM16_WRITE
#define EDDP_HAL_MEM16_WRITE(pDDB, pMem, Value)     \
            *((volatile EDD_COMMON_MEM_U16_PTR_TYPE)(pMem)) = EDDP_HAL_H2P_16((LSA_UINT16)(Value))
#else
#define EDDP_HAL_MEM16_WRITE(pDDB, pMem, Value)     \
        EDDP_MEM16_WRITE(pDDB->hSysDev,(volatile EDD_COMMON_MEM_U16_PTR_TYPE)(pMem),Value)
#endif

// RD 8bit Memory,                      eg: EDDP_HAL_MEM8_READ(pDDB, (EDD_COMMON_MEM_U8_PTR_TYPE)((volatile EDD_COMMON_MEM_U8_PTR_TYPE)(pDDB->pPnipBase) + PNIP_RAM_APICTRL_START))
#ifndef EDDP_MEM8_READ
#define EDDP_HAL_MEM8_READ(pDDB, pMem)              \
            *((volatile EDD_COMMON_MEM_U8_PTR_TYPE)(pMem))
#else
#define EDDP_HAL_MEM8_READ(pDDB, pMem)              \
        EDDP_MEM8_READ(pDDB->hSysDev,(volatile EDD_COMMON_MEM_U8_PTR_TYPE)(pMem))
#endif

// WR 8bit Memory,                      eg: EDDP_HAL_MEM8_WRITE(pDDB, (EDD_COMMON_MEM_U8_PTR_TYPE)((volatile EDD_COMMON_MEM_U8_PTR_TYPE)(pDDB->pPnipBase) + PNIP_RAM_APICTRL_START), 0x01)
#ifndef EDDP_MEM8_WRITE
#define EDDP_HAL_MEM8_WRITE(pDDB, pMem, Value)      \
            *((volatile EDD_COMMON_MEM_U8_PTR_TYPE)(pMem)) = (LSA_UINT8)(Value)
#else
#define EDDP_HAL_MEM8_WRITE(pDDB, pMem, Value)      \
        EDDP_MEM8_WRITE(pDDB->hSysDev,(volatile EDD_COMMON_MEM_U8_PTR_TYPE)(pMem),Value)
#endif

//RD HC-Counter consistent
#define EDDP_READ_HC_COUNTER_CONSISTENT(pDDB, pMem_H, pMem_L, Low, High, High_New, HC_Val)  \
            High = EDDP_HAL_MEM32_READ(pDDB, pMem_H); \
            Low =  EDDP_HAL_MEM32_READ(pDDB, pMem_L); \
            High_New = EDDP_HAL_MEM32_READ(pDDB, pMem_H); \
            if (High_New != High) { \
                Low =  EDDP_HAL_MEM32_READ(pDDB, pMem_L); } \
            HC_Val = (((LSA_UINT64)High_New) << 32) | ((LSA_UINT64)Low & 0xFFFFFFFF);

//RD HC-Counter consistent
#define EDDP_READ_HC_IF_COUNTER_CONSISTENT(pDDB, pMem_H, pMem_L, Low, High, High_New, HC_Val)  \
            High = EDDP_HAL_REG32_READ(pDDB, pMem_H); \
            Low =  EDDP_HAL_REG32_READ(pDDB, pMem_L); \
            High_New = EDDP_HAL_REG32_READ(pDDB, pMem_H); \
            if (High_New != High) { \
                Low =  EDDP_HAL_REG32_READ(pDDB, pMem_L); } \
            HC_Val = (((LSA_UINT64)High_New) << 32) | ((LSA_UINT64)Low & 0xFFFFFFFF);

/*===========================================================================*/
/*                                prototyping                                */
/*===========================================================================*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_HALGetPnipRevision_Ext(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_HALGetPnipRevision(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_HALPnipPortCfg(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_HALGetChipType(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_HALGetChipVersion(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_HALGetChipStep(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_HALGetChipInterface(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);
/*---------------------------------------------------------------------------*/
/* Memoryhandling                                                            */
/*---------------------------------------------------------------------------*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HALDeviceOpen(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_UPPER_DPB_PTR_TYPE     pDPB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HALDeviceSetup(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_UPPER_DSB_PTR_TYPE     pDSB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HALDeviceShutdown(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HALMemset(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT32                  RAMType,
	LSA_UINT32                  RAMSubType,
    LSA_UINT32                  IntOffs,
    LSA_UINT32                  Length,
    LSA_UINT8                   Pattern);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define EDDP_HAL_32BIT_ALIGNMENT        0x0003      // compare bits for 32 bit/4 byte alignment
#define EDDP_HAL_64BIT_ALIGNMENT        0x0007      // compare bits for 64 bit/8 byte alignment
#define EDDP_HAL_128BIT_ALIGNMENT       0x000F      // compare bits for 128 bit/16 byte alignment
#define EDDP_HAL_256BIT_ALIGNMENT       0x001F      // compare bits for 256 bit/32 byte alignment
#define EDDP_HAL_512BIT_ALIGNMENT       0x003F      // compare bits for 512 bit/64 byte alignment
#define EDDP_HAL_1024BIT_ALIGNMENT      0x007F      // compare bits for 1024 bit/128 byte alignment

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_HALTrimAlignment(
    LSA_UINT32      Address,
    LSA_UINT32      Alignment);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HALTraceLayout(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HALTestRamBorder(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT32                  RAMType,
	LSA_UINT32                  RAMSubType);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_HALGetVersion(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
	EDD_COMMON_MEM_U32_PTR_TYPE     pPnipVersion,
    EDD_COMMON_MEM_U32_PTR_TYPE     pPnipDevelopment);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_COMMON_MEM_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_HALGetPnipBaseAddress(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_COMMON_MEM_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_HALGetRAMBaseAddress(
	EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
	LSA_UINT32            	        RAMType,
	EDD_COMMON_MEM_U32_PTR_TYPE     pRAMSize);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_COMMON_MEM_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_HALGetRAMSubAddress(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
	LSA_UINT32                      RAMType,
	LSA_UINT32                      RAMSubType,
    EDD_COMMON_MEM_U32_PTR_TYPE     pRAMSize);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_HALGetPnipOffsetToStart(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_COMMON_MEM_PTR_TYPE     pRAMAddr);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_HALGetPnipRAMOffsetToRAM(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT32                  RAMType,
    EDD_COMMON_MEM_PTR_TYPE     pRAMAddr);



/*---------------------------------------------------------------------------*/
/* Timeout handling                                                          */
/*---------------------------------------------------------------------------*/
#define EDDP_HAL_100NS_TO         100
#define EDDP_HAL_1US_TO          1000
#define EDDP_HAL_10US_TO        10000
#define EDDP_HAL_100US_TO      100000
#define EDDP_HAL_1MS_TO       1000000
#define EDDP_HAL_10MS_TO     10000000
#define EDDP_HAL_100MS_TO   100000000

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_HALWaitNsTicks(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  nTicks);

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_HALGetNsCounter(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_HALDiffNsCounter(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  nStart);


/*---------------------------------------------------------------------------*/
/* Interrupthandling                                                         */
/*---------------------------------------------------------------------------*/
LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_HALIsrInit(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB);

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_HALIsrGetGroupMsk(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_HAL_ISR_GRP_REGS_PTR_TYPE          pEvents);

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_HALIsrSetGroupMsk(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_HAL_ISR_GRP_REGS_PTR_TYPE          pEvents);

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_HALIsrAckGroupIRQ(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_HAL_ISR_GRP_REGS_PTR_TYPE          pEvents);

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_HALIsrGetGroupIRQ(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_HAL_ISR_GRP_REGS_PTR_TYPE          pEvents);

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_HALIsrGroupEOI(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB);

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_HALIsrGetEvents(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
	EDDP_HAL_ISR_GRP_REGS_PTR_TYPE          pEvents);

#if defined  (EDDP_CFG_ISR_POLLING_MODE)    
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_HALIsrGetSingleOffsets(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    LSA_UINT32                              EventNr,
    LSA_UINT32                            * pAckRegOffset,
    LSA_UINT32                            * pIsrRegOffset,
    LSA_UINT32                            * pMsk);
#endif //(EDDP_CFG_ISR_POLLING_MODE)    
    
LSA_EXTERN LSA_UINT64 EDDP_LOCAL_FCT_ATTR EDDP_HALGetClockA(
EDDP_LOCAL_DDB_PTR_TYPE                 pDDB);

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_HALIsrEnableMUXIRQ(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    LSA_UINT32                              IRQNr,
    LSA_UINT32                              EventNr,
    LSA_UINT32                              IRQWaitTime);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_HALIsrRetriggerMUXIRQ_WaitTime(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
	LSA_UINT32                              IRQNr,
	LSA_UINT32                              EventNr,
	LSA_UINT32                              IRQWaitTime);

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_HALIsrDisableMUXIRQ(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
	LSA_UINT32                              IRQNr);

/*****************************************************************************/
/*  end of file EDDP_HAL_USR.H                                               */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_HAL_USR_H */
