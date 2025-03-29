
#ifndef EDDP_USR_SYSRED_INTERN_H           /* ----- reinclude-protection ----- */
#define EDDP_USR_SYSRED_INTERN_H

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
    /*  F i l e               &F: eddp_usr_sysred_intern.h                  :F&  */
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
    /*  Date        Who         What                                             */
#ifdef EDDP_MESSAGE
    /*  29.01.13    z00301au    initial version.                                 */
#endif
    /*****************************************************************************/
#ifdef EDDP_CFG_SYSRED_API_SUPPORT


    /*===========================================================================*/
    /*                              Defines                                      */
    /*===========================================================================*/

#define EDDP_IO_UNDEFINED_INSTANCE_HANDLE 0xFFFFFFFFUL
#define EDDP_IO_UNKNOWN_PROV_ID        0xFFFF

    /*===========================================================================*/
    /*                              Macros                                       */
    /*===========================================================================*/
    // write 32bit value to PN-IP memory
#ifndef EDDP_USR_SYSRED_MEM32_WRITE
#define EDDP_USR_SYSRED_MEM32_WRITE(pMem, Value)   \
    *((volatile EDD_COMMON_MEM_U32_PTR_TYPE)(pMem)) = EDDP_HAL_H2P_32((LSA_UINT32)(Value))
#endif

    // write 8bit value to PN-IP memory
#ifndef EDDP_USR_SYSRED_MEM8_WRITE
#define EDDP_USR_SYSRED_MEM8_WRITE(pMem, Value)   \
    *((volatile EDD_COMMON_MEM_U8_PTR_TYPE)(pMem)) = ((LSA_UINT8)(Value))
#endif

    // read 32bit value from PN-IP memory
#ifndef EDDP_USR_SYSRED_MEM32_READ
#define EDDP_USR_SYSRED_MEM32_READ(pMem)   \
    EDDP_HAL_P2H_32(*((volatile EDD_COMMON_MEM_U32_PTR_TYPE)(pMem)))
#endif

   // read 8bit value from PN-IP memory
#ifndef EDDP_USR_SYSRED_MEM8_READ
#define EDDP_USR_SYSRED_MEM8_READ(pMem)   \
    (*((volatile EDD_COMMON_MEM_U8_PTR_TYPE)(pMem)))
#endif

    // mapping of view Host <---> PNIP
#define EDDP_USR_SYSRED_H2P_ADDR(HostAddr, PnipBase)       (LSA_UINT32)((LSA_UINT32)(HostAddr) - (LSA_UINT32)(PnipBase))
#define EDDP_USR_SYSRED_P2H_ADDR(PnipAddr, PnipBase)       (LSA_UINT32)((LSA_UINT32)(PnipAddr) + (LSA_UINT32)(PnipBase))

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_UserSysRedInternFatalError(
    EDDP_FATAL_ERROR_TYPE   Error,
    LSA_UINT16              ModuleID,
    LSA_UINT32              Line);

#define EDDP_USR_SYSRED_FATAL(Error)    \
{                                       \
    EDDP_UserSysRedInternFatalError(    \
            Error,                      \
            EDDP_MODULE_ID,             \
            __LINE__);                  \
}

// check for TRUE condition assertion
#ifdef EDDP_CFG_DEBUG_ASSERT
    #define EDDP_USR_SYSRED_ASSERT_FALSE(Cond_)                                                                                 \
    {                                                                                                                           \
        if( (Cond_) )                                                                                                           \
        {                                                                                                                       \
            EDDP_USR_SYSRED_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);                                                                \
        }                                                                                                                       \
    }
#else
    #define EDDP_USR_SYSRED_ASSERT_FALSE(Condition) {LSA_UNUSED_ARG((Condition));}
#endif

// check for FALSE condition assertion
#ifdef EDDP_CFG_DEBUG_ASSERT
    #define EDDP_USR_SYSRED_ASSERT(Cond_)                                                                                       \
    {                                                                                                                           \
        if( !(Cond_) )                                                                                                          \
        {                                                                                                                       \
            EDDP_USR_SYSRED_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);                                                                \
        }                                                                                                                       \
    }
#else
    #define EDDP_USR_SYSRED_ASSERT(Condition) {LSA_UNUSED_ARG((Cond_));}
#endif

    /*===========================================================================*/
    /*                              Types                                        */
    /*===========================================================================*/



/*** Providerhandling ***/
typedef struct _EDDP_IO_PROVIDER_LIST_ENTRY_TYPE
{
    LSA_UINT16                        provider_id;                                  // ID of the Provider.
    LSA_UINT8                       * pDataStatus;                                  // Pointer to the location of the DataStatus (8Bit). Host address
    LSA_UINT16                        ARSetID;                                      // ID of the ARSet. (1...4)
    LSA_UINT16                      * pARSet;                                       // Pointer to the ARSet within the gSharedRAM. Host address
} EDDP_IO_PROVIDER_LIST_ENTRY_TYPE;

typedef struct _EDDP_IO_CB_TYPE
{
    LSA_UINT32                        InstanceHandle;                               // Number of the interface. 1 ... EDD_INTERFACE_ID_MAX
    LSA_UINT32                        TraceIdx;                                     // Trace Index, given in Setup function
    LSA_VOID*                         pPnipBase;                                    // pointer to the Base Adr of the PNIP RAM.
    EDDP_GSHAREDMEM_TYPE            * pGSharedRAM;                                  // Pointer to a shared memory on the HOST system. This memory is shared between the EDDP and the application that runs the SysRed API
    LSA_UINT16                        Hardware;                                      // the hardware where the EDDP is running on
    EDDP_IO_PROVIDER_LIST_ENTRY_TYPE  provider_list[EDDP_MAX_NR_APPL_PROVIDERS];    // Store Providers with their pDataStatus
} EDDP_IO_CB_TYPE;


    /*****************************************************************************/
    /*  end of file eddp_usr_sysred_intern.h                                      */
    /*****************************************************************************/
#endif //EDDP_CFG_SYSRED_API_SUPPORT
#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_USR_SYSRED_INTERN_H */
