
#ifndef EDDP_USR_CMD_INTERN_H           /* ----- reinclude-protection ----- */
#define EDDP_USR_CMD_INTERN_H

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
/*  F i l e               &F: eddp_usr_cmd_intern.h                     :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Internal headerfile. includes all internal headers. prototypes           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  26.04.12    VS    initial version.                                       */
#endif
/*****************************************************************************/


/*===========================================================================*/
/*                              Defines                                      */
/*===========================================================================*/
// mask of CmdQueue[USER_SYNC] in register "ConfirmingQueue -> Bit(12) = CmdQueue[12]
#define EDDP_USR_CMD_CONFIRMINGQUEUE_USER_SYNC_MASK             0x00001000

// defines for register "ApplStatus"
#define EDDP_USER_CMD_REG_APPL_STATUS_VALUE_FREE                0x00
#define EDDP_USER_CMD_REG_APPL_STATUS_VALUE_PTCP_CLOCK_IFA      0x01

// register "IP_Development.Inkr(15:11)"
//#define EDDP_USER_CMD_REG_IP_DEVELOPMENT_INKR_REV1              0x4     // Inkr = "00100" (Rev1)
//#define EDDP_USER_CMD_REG_IP_DEVELOPMENT_INKR_REV2              0x5     // Inkr = "00101" (Rev2)
// register "IP_Version.Version(15:8)"
#define EDDP_USER_CMD_REG_IP_VERSION_VERSION_REV1               0x1     // Rev1 = "00000001"
#define EDDP_USER_CMD_REG_IP_VERSION_VERSION_REV2               0x2     // Rev2 = "00000010"
#define EDDP_USER_CMD_REG_IP_VERSION_VERSION_REV3               0x3     // Rev3 = "00000011"


/*===========================================================================*/
/*                              Macros                                       */
/*===========================================================================*/
// write 32bit value to PN-IP memory
#ifndef EDDP_USR_CMD_MEM32_WRITE
#define EDDP_USR_CMD_MEM32_WRITE(pMem, Value)   \
            *((volatile EDD_COMMON_MEM_U32_PTR_TYPE)(pMem)) = EDDP_HAL_H2P_32((LSA_UINT32)(Value))
#endif

// read 32bit value from PN-IP memory
#ifndef EDDP_USR_CMD_MEM32_READ
#define EDDP_USR_CMD_MEM32_READ(pMem)   \
            EDDP_HAL_P2H_32(*((volatile EDD_COMMON_MEM_U32_PTR_TYPE)(pMem)))
#endif

// mapping of view Host <---> PNIP
#define EDDP_USR_CMD_H2P_ADDR(HostAddr, PnipBase)       (LSA_UINT32)((LSA_UINT32)(HostAddr) - (LSA_UINT32)(PnipBase))
#define EDDP_USR_CMD_P2H_ADDR(PnipAddr, PnipBase)       (LSA_UINT32)((LSA_UINT32)(PnipAddr) + (LSA_UINT32)(PnipBase))

// macro for fatal error to call the fatal function
#define EDDP_USR_CMD_FATAL(Error)       \
{                                       \
    EDDP_UserCmdInternFatalError(       \
            Error,                      \
            EDDP_MODULE_ID,             \
            __LINE__);                  \
}

// check for TRUE condition assertion
#ifdef EDDP_CFG_DEBUG_ASSERT
    #define EDDP_USR_CMD_ASSERT_FALSE(Cond_)                                                                                \
    {                                                                                                                       \
        if( (Cond_) )                                                                                                       \
        {                                                                                                                   \
            EDDP_USR_CMD_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);                                                               \
        }                                                                                                                   \
    }
#else
    #define EDDP_USR_CMD_ASSERT_FALSE(Condition) {LSA_UNUSED_ARG((Condition));}
#endif

// check for FALSE condition assertion
#ifdef EDDP_CFG_DEBUG_ASSERT
    #define EDDP_USR_CMD_ASSERT(Cond_)                                                                                      \
    {                                                                                                                       \
        if( !(Cond_) )                                                                                                      \
        {                                                                                                                   \
            EDDP_USR_CMD_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);                                                               \
        }                                                                                                                   \
    }
#else
    #define EDDP_USR_CMD_ASSERT(Cond_) {LSA_UNUSED_ARG((Cond_));}
#endif

/*===========================================================================*/
/*                              Types                                        */
/*===========================================================================*/

// parameters of current command
typedef struct _EDDP_USER_CMD_CURRENT_CMD_PARAM_TYPE
{
    LSA_UINT32          CmdOpcode;
    LSA_UINT8           ApplTimerMask;
} EDDP_USER_CMD_CURRENT_CMD_PARAM_TYPE;
/* CmdOpcode                : CmdOpcode of user command is current in processing                    */
#define EDDP_USER_CMD_CURRENT_CMD_OPCODE_INVALID            0xFFFFFFFF
/* ApplTimerMask            : bits for selected ApplicationTimer(1/2/3)                             */

// struct for management of one (single) handle
typedef struct _EDDP_USER_CMD_HANDLE_MNGM_SINGLE_TYPE   EDDP_LOCAL_MEM_ATTR *   EDDP_USER_CMD_HANDLE_MNGM_SINGLE_PTR_TYPE;
typedef struct _EDDP_USER_CMD_HANDLE_MNGM_SINGLE_TYPE
{
    LSA_BOOL                                InUseHandle;
    EDDP_USER_CMD_HANDLE_TYPE               HandleId;
    EDDP_USER_CMD_CURRENT_CMD_PARAM_TYPE    CurrentCmdParam;
    EDD_COMMON_MEM_PTR_TYPE                 pPnipBase;
    EDD_COMMON_MEM_PTR_TYPE                 pInfoBlockHostAddr;
} EDDP_USER_CMD_HANDLE_MNGM_SINGLE_TYPE;
/* InUseHandle              : LSA_TRUE -> this handle is used by user                               */
/* HandleId                 : Handle-ID                                                             */
/* CurrentCmdParam          : parameters of current command                                         */
/* pPnipBase                : pointer to internal memory of the PN-IP from HOST view                */
/* pInfoBlockHostAddr       : pointer to Cmd-Info-Block in host format                              */
#if 1
#define EDDP_USER_CMD_RAM_CMD_INFO_BLOCK_PNIP_ADDR          0x0D904     // CMD-Ram-Base(0x68000) + CMD_QUEUE_MGMT(0x100) + CMD_KRISC(0x800) + BorderCheck(0x4)
#else
#define EDDP_USER_CMD_RAM_CMD_INFO_BLOCK_PNIP_ADDR          0x68904     // CMD-Ram-Base(0x68000) + CMD_QUEUE_MGMT(0x100) + CMD_KRISC(0x800) + BorderCheck(0x4)
#endif

// main struct for handle management of EDDP user command intern
typedef struct _EDDP_USER_CMD_INT_MNGM_TYPE     EDDP_LOCAL_MEM_ATTR *   EDDP_USER_CMD_INT_MNGM_PTR_TYPE;
typedef struct _EDDP_USER_CMD_INT_MNGM_TYPE
{
    LSA_UINT16                                  EDDPLocation;
    LSA_UINT32                                  HandleUsedCnt;
    EDDP_USERCMDSHAREDMEM_TYPE                * pSharedMem;
    EDDP_USER_CMD_HANDLE_MNGM_SINGLE_TYPE       HandleMngm[EDDP_CFG_MAX_USER_CMD_HANDLES];
} EDDP_USER_CMD_INT_MNGM_TYPE;
/* EDDPLocation             : location of EDDP                                                  */
/* HandleUsedCnt            : number of used handles                                            */
/* pSharedMem               : pointer to the Shared Memory                                      */
/* HandleMngm               : array of all handle managements                                   */


/*===========================================================================*/
/*                          prototypes                                      */
/*===========================================================================*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_UserCmdInternTimeStampGet(
	EDDP_USER_CMD_HANDLE_TYPE       Handle,
 	EDDP_USER_CMD_PARAM_PTR_TYPE    pParam);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_UserCmdInternTimeSyncIndGet(
	EDDP_USER_CMD_HANDLE_TYPE       Handle,
 	EDDP_USER_CMD_PARAM_PTR_TYPE    pParam);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_UserCmd_CList_ReadTimeSyncInd( 
    EDDP_USERCMDSHAREDMEM_TYPE    * const pSharedMem,
    EDDP_TIMESTAMP_INFO_TYPE      * const pOldTimestamp,
    EDDP_TIMESTAMP_INFO_TYPE      * const pNewTimestamp);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_UserCmdInternFillCblAndParam(
	EDDP_USER_CMD_HANDLE_TYPE       Handle,
 	EDDP_USER_CMD_PARAM_PTR_TYPE    pParam);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_UserCmdInternWaitNsTicks(
    EDDP_USER_CMD_HANDLE_TYPE       Handle,
    LSA_UINT32                      nTicks);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_UserCmdInternReadCommandReturnValue(
	EDDP_USER_CMD_HANDLE_TYPE       Handle);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_UserCmdInternAreCheckBytesPresent(
    EDDP_USER_CMD_HANDLE_TYPE       Handle);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_UserCmdInternFatalError(
	EDDP_FATAL_ERROR_TYPE   Error,
	LSA_UINT16              ModuleID,
	LSA_UINT32              Line);

/*****************************************************************************/
/*  end of file eddp_usr_cmd_intern.h                                        */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_USR_CMD_INTERN_H */
