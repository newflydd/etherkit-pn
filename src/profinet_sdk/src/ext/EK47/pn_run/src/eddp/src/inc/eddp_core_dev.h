/** @file */
#ifndef EDDP_CORE_DEV_H                  /* ----- reinclude-protection ----- */
#define EDDP_CORE_DEV_H

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
/*  F i l e               &F: eddp_core_dev.h                           :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Device/Handle                                                            */
/*  Defines constants, types, macros and prototyping for EDD                 */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  15.08.08    JS    initial version.                                       */
/*  22.12.08    LRG   K32                                                    */
/*  17.01.11    JS    added MaxInterfaceCntOfAllEDD and MaxPortCntOfAllEDD   */
/*  02.03.16    HM    pUserCmdSharedMem added to DDB                         */
#endif
/*****************************************************************************/

/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/

/*===========================================================================*/
/*                                defines                                    */
/*===========================================================================*/


/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/


/*===========================================================================*/
/* Statistic management (General Services)                                   */
/*===========================================================================*/

typedef struct _EDD_GET_STATISTICS_MIB_TYPE  EDDP_LOCAL_MEM_ATTR * EDDP_LOCAL_GET_STATISTICS_PTR_TYPE;
typedef struct _EDD_GET_STATISTICS_MIB_TYPE  EDDP_GET_STATISTICS_TYPE;

/* SupportedCounters  :   Bitmask defining supported MIB Counters.      (out) */

#define EDDP_MIB2_SUPPORTED_COUNTERS_PORT_REV1      EDD_MIB_SUPPORT_INOCTETS            \
                                                |   EDD_MIB_SUPPORT_INUCASTPKTS         \
                                                |   EDD_MIB_SUPPORT_INNUCASTPKTS        \
                                                |   EDD_MIB_SUPPORT_INDISCARDS          \
                                                |   EDD_MIB_SUPPORT_INERRORS            \
                                                |   EDD_MIB_SUPPORT_OUTOCTETS           \
                                                |   EDD_MIB_SUPPORT_OUTUCASTPKTS        \
                                                |   EDD_MIB_SUPPORT_OUTNUCASTPKTS       \
                                                |   EDD_MIB_SUPPORT_OUTDISCARDS         \
                                                |   EDD_MIB_SUPPORT_INMULTICASTPKTS     \
                                                |   EDD_MIB_SUPPORT_INBROADCASTPKTS     \
                                                |   EDD_MIB_SUPPORT_OUTMULTICASTPKTS    \
                                                |   EDD_MIB_SUPPORT_OUTBROADCASTPKTS

#define EDDP_RECORD_REQUESTABLE_COUNTERS_PORT_REV1  EDD_MIB_SUPPORT_INOCTETS         \
                                                |   EDD_MIB_SUPPORT_OUTOCTETS        \
                                                |   EDD_MIB_SUPPORT_INDISCARDS       \
                                                |   EDD_MIB_SUPPORT_OUTDISCARDS      \
                                                |   EDD_MIB_SUPPORT_INERRORS

#define EDDP_MIB2_SUPPORTED_COUNTERS_PORT_REV2      EDD_MIB_SUPPORT_INOCTETS            \
                                                |   EDD_MIB_SUPPORT_INUCASTPKTS         \
                                                |   EDD_MIB_SUPPORT_INNUCASTPKTS        \
                                                |   EDD_MIB_SUPPORT_INDISCARDS          \
                                                |   EDD_MIB_SUPPORT_INERRORS            \
                                                |   EDD_MIB_SUPPORT_OUTOCTETS           \
                                                |   EDD_MIB_SUPPORT_OUTUCASTPKTS        \
                                                |   EDD_MIB_SUPPORT_OUTNUCASTPKTS       \
                                                |   EDD_MIB_SUPPORT_OUTDISCARDS         \
                                                |   EDD_MIB_SUPPORT_OUTERRORS           \
                                                |   EDD_MIB_SUPPORT_INMULTICASTPKTS     \
                                                |   EDD_MIB_SUPPORT_INBROADCASTPKTS     \
                                                |   EDD_MIB_SUPPORT_OUTMULTICASTPKTS    \
                                                |   EDD_MIB_SUPPORT_OUTBROADCASTPKTS

#define EDDP_RECORD_REQUESTABLE_COUNTERS_PORT_REV2  EDD_MIB_SUPPORT_INOCTETS         \
                                                |   EDD_MIB_SUPPORT_OUTOCTETS        \
                                                |   EDD_MIB_SUPPORT_INDISCARDS       \
                                                |   EDD_MIB_SUPPORT_OUTDISCARDS      \
                                                |   EDD_MIB_SUPPORT_INERRORS         \
                                                |   EDD_MIB_SUPPORT_OUTERRORS

#define EDDP_MIB2_SUPPORTED_COUNTERS_PORT_REV3      EDD_MIB_SUPPORT_INOCTETS            \
                                                |   EDD_MIB_SUPPORT_INUCASTPKTS         \
                                                |   EDD_MIB_SUPPORT_INNUCASTPKTS        \
                                                |   EDD_MIB_SUPPORT_INDISCARDS          \
                                                |   EDD_MIB_SUPPORT_INERRORS            \
                                                |   EDD_MIB_SUPPORT_OUTOCTETS           \
                                                |   EDD_MIB_SUPPORT_OUTUCASTPKTS        \
                                                |   EDD_MIB_SUPPORT_OUTNUCASTPKTS       \
                                                |   EDD_MIB_SUPPORT_OUTDISCARDS         \
                                                |   EDD_MIB_SUPPORT_OUTERRORS           \
                                                |   EDD_MIB_SUPPORT_INMULTICASTPKTS     \
                                                |   EDD_MIB_SUPPORT_INBROADCASTPKTS     \
                                                |   EDD_MIB_SUPPORT_OUTMULTICASTPKTS    \
                                                |   EDD_MIB_SUPPORT_OUTBROADCASTPKTS    \
                                                |   EDD_MIB_SUPPORT_INHCOCTETS          \
                                                |   EDD_MIB_SUPPORT_INHCUCASTPKTS       \
                                                |   EDD_MIB_SUPPORT_INHCMULTICASTPKTS   \
                                                |   EDD_MIB_SUPPORT_INHCBROADCASTPKTS   \
                                                |   EDD_MIB_SUPPORT_OUTHCOCTETS         \
                                                |   EDD_MIB_SUPPORT_OUTHCUCASTPKTS      \
                                                |   EDD_MIB_SUPPORT_OUTHCMULTICASTPKTS  \
                                                |   EDD_MIB_SUPPORT_OUTHCBROADCASTPKTS

#define EDDP_RECORD_REQUESTABLE_COUNTERS_PORT_REV3  EDD_MIB_SUPPORT_INOCTETS         \
                                                |   EDD_MIB_SUPPORT_OUTOCTETS        \
                                                |   EDD_MIB_SUPPORT_INDISCARDS       \
                                                |   EDD_MIB_SUPPORT_OUTDISCARDS      \
                                                |   EDD_MIB_SUPPORT_INERRORS         \
                                                |   EDD_MIB_SUPPORT_OUTERRORS

#define EDDP_MIB2_SUPPORTED_COUNTERS_IF             EDD_MIB_SUPPORT_INUCASTPKTS    \
                                                |   EDD_MIB_SUPPORT_INOCTETS       \
                                                |   EDD_MIB_SUPPORT_INNUCASTPKTS   \
                                                |   EDD_MIB_SUPPORT_INDISCARDS     \
                                                |   EDD_MIB_SUPPORT_INUNKNOWNPROTOS\
                                                |   EDD_MIB_SUPPORT_OUTOCTETS      \
                                                |   EDD_MIB_SUPPORT_OUTUCASTPKTS   \
                                                |   EDD_MIB_SUPPORT_OUTNUCASTPKTS  \
                                                |   EDD_MIB_SUPPORT_OUTDISCARDS    \
                                                |   EDD_MIB_SUPPORT_OUTERRORS      \
                                                |   EDD_MIB_SUPPORT_INMULTICASTPKTS     \
                                                |   EDD_MIB_SUPPORT_INBROADCASTPKTS     \
                                                |   EDD_MIB_SUPPORT_OUTMULTICASTPKTS    \
                                                |   EDD_MIB_SUPPORT_OUTBROADCASTPKTS

#define EDDP_RECORD_REQUESTABLE_COUNTERS_IF         EDD_MIB_SUPPORT_INDISCARDS       \
                                                |   EDD_MIB_SUPPORT_OUTDISCARDS      \
                                                |   EDD_MIB_SUPPORT_OUTERRORS

#define EDDP_MIB2_SUPPORTED_COUNTERS_IF_REV3        EDD_MIB_SUPPORT_INOCTETS            \
                                                |   EDD_MIB_SUPPORT_INUCASTPKTS         \
                                                |   EDD_MIB_SUPPORT_INNUCASTPKTS        \
                                                |   EDD_MIB_SUPPORT_INDISCARDS          \
                                                |   EDD_MIB_SUPPORT_INERRORS            \
                                                |   EDD_MIB_SUPPORT_INUNKNOWNPROTOS     \
                                                |   EDD_MIB_SUPPORT_OUTOCTETS           \
                                                |   EDD_MIB_SUPPORT_OUTUCASTPKTS        \
                                                |   EDD_MIB_SUPPORT_OUTNUCASTPKTS       \
                                                |   EDD_MIB_SUPPORT_OUTDISCARDS         \
                                                |   EDD_MIB_SUPPORT_OUTERRORS           \
                                                |   EDD_MIB_SUPPORT_INMULTICASTPKTS     \
                                                |   EDD_MIB_SUPPORT_INBROADCASTPKTS     \
                                                |   EDD_MIB_SUPPORT_OUTMULTICASTPKTS    \
                                                |   EDD_MIB_SUPPORT_OUTBROADCASTPKTS    \
                                                |   EDD_MIB_SUPPORT_INHCOCTETS          \
                                                |   EDD_MIB_SUPPORT_INHCUCASTPKTS       \
                                                |   EDD_MIB_SUPPORT_INHCMULTICASTPKTS   \
                                                |   EDD_MIB_SUPPORT_INHCBROADCASTPKTS   \
                                                |   EDD_MIB_SUPPORT_OUTHCOCTETS         \
                                                |   EDD_MIB_SUPPORT_OUTHCUCASTPKTS      \
                                                |   EDD_MIB_SUPPORT_OUTHCMULTICASTPKTS  \
                                                |   EDD_MIB_SUPPORT_OUTHCBROADCASTPKTS

#define EDDP_RECORD_REQUESTABLE_COUNTERS_IF_REV3    EDD_MIB_SUPPORT_INOCTETS         \
                                                |   EDD_MIB_SUPPORT_OUTOCTETS        \
                                                |   EDD_MIB_SUPPORT_INDISCARDS       \
                                                |   EDD_MIB_SUPPORT_OUTDISCARDS      \
                                                |   EDD_MIB_SUPPORT_INERRORS         \
                                                |   EDD_MIB_SUPPORT_OUTERRORS

#define EDDP_RECORD_SUPPORTED_COUNTERS_ALL          EDD_RECORD_SUPPORT_INOCTETS         \
                                                |   EDD_RECORD_SUPPORT_OUTOCTETS        \
                                                |   EDD_RECORD_SUPPORT_INDISCARDS       \
                                                |   EDD_RECORD_SUPPORT_OUTDISCARDS      \
                                                |   EDD_RECORD_SUPPORT_INERRORS         \
                                                |   EDD_RECORD_SUPPORT_OUTERRORS

#define EDDP_RECORD_SUPPORTED_COUNTERS_IF_REV1_2    EDD_RECORD_SUPPORT_INDISCARDS       \
                                                |   EDD_RECORD_SUPPORT_OUTDISCARDS      \
                                                |   EDD_RECORD_SUPPORT_OUTERRORS

#define EDDP_RECORD_SUPPORTED_COUNTERS_PORT_REV1    EDD_RECORD_SUPPORT_INOCTETS         \
                                                |   EDD_RECORD_SUPPORT_OUTOCTETS        \
                                                |   EDD_RECORD_SUPPORT_INDISCARDS       \
                                                |   EDD_RECORD_SUPPORT_OUTDISCARDS      \
                                                |   EDD_RECORD_SUPPORT_INERRORS


typedef struct _EDDP_HDB_GEN_TYPE       EDDP_LOCAL_MEM_ATTR *    EDDP_LOCAL_HDB_GEN_PTR_TYPE;
typedef struct _EDDP_HDB_GEN_TYPE
{   
    EDDP_GET_STATISTICS_TYPE    Statistic[EDDP_MAX_PORT_CNT+1];     // ports as bitmask used by the interface
    LSA_HANDLE_TYPE             Handle;                             // idx of this ptr in core
} EDDP_HDB_GEN_TYPE;


/*===========================================================================*/
/* Global Timer management                                                   */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* This is used to asign a LSA-Timer ID to an static RQB                     */
/* This management is deviceindependend and used for all devices.            */
/*---------------------------------------------------------------------------*/
                                                
typedef volatile struct _EDDP_TIMER_INFO_TYPE  EDDP_LOCAL_MEM_ATTR *EDDP_LOCAL_TIMER_INFO_PTR_TYPE;
typedef volatile struct _EDDP_TIMER_INFO_TYPE
{
    LSA_BOOL                        InUse;
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB;       /* pDDB used by timer           */
    LSA_BOOL                        Running;    /* LSA_TRUE if Timer is running */
    LSA_TIMER_ID_TYPE               TimerID;
    EDDP_INTERNAL_REQUEST_TYPE      IntRQB;
    LSA_VOID                        LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, Cbf) 
                                               (EDDP_INTERNAL_REQUEST_PTR_TYPE   pIntRQB);    
} EDDP_TIMER_INFO_TYPE;


/*===========================================================================*/
/* Locking. Reentrance checks. Used by ENTER/EXIT Macros                     */
/*===========================================================================*/

typedef volatile struct _EDDP_LOCKING_TYPE
{
    LSA_UINT32      EDDPLockSemaLOW;  /* 0 = free, 1 = used */
    LSA_UINT32      EDDPLockSemaMAIN; /* 0 = free, 1 = used */
    LSA_UINT32      EDDPLockSemaINT;  /* 0 = free, 1 = used */
    LSA_UINT32      EDDPLockSemaTIME; /* 0 = free, 1 = used */
    LSA_UINT32                   EDDPLockSemaI2C;  /* 0 = free, 1 = used */
} EDDP_LOCKING_TYPE;


/*===========================================================================*/
/*                                                                           */
/* Handle-management-structures                                              */
/*                                                                           */
/* (this structure is asigned to every open channel. Each component (NRT,SRT */
/* etc) can have individual parameters within management structure only used */
/* in component.                                                             */
/*===========================================================================*/

typedef struct _EDDP_HDB_TYPE
{
    LSA_BOOL                            InUse;
    LSA_UINT32                          UsedComp;
    LSA_SYS_PATH_TYPE                   SysPath;
    LSA_SYS_PTR_TYPE                    pSys;
    EDDP_DETAIL_PTR_TYPE                pDetail;
    LSA_HANDLE_TYPE                     UpperHandle;
    LSA_HANDLE_TYPE                     Handle;     /* Handle idx. */
    LSA_VOID                            LSA_FCT_PTR(EDD_UPPER_OUT_FCT_ATTR, Cbf)
                                                   (EDD_UPPER_RQB_PTR_TYPE pRQB);
                                                    /* Link-Indication Requests */
    EDDP_LOCAL_DDB_PTR_TYPE             pDDB;       /* link to devicemanagement */

    EDDP_RQB_QUEUE_TYPE                 LinkIndReq[EDDP_MAX_PORT_CNT+1];        /* index 0 is AUTO */
    LSA_UINT16                          LinkIndReqType[EDDP_MAX_PORT_CNT+1];    /* Type of service for each PortID */

    EDDP_LOCAL_HDB_NRT_PTR_TYPE         pNRT;       /* NRT handle management      */
    EDDP_LOCAL_HDB_K32_PTR_TYPE         pK32;       /* K32 handle management      */
    EDDP_LOCAL_HDB_CRT_PTR_TYPE         pCRT;       /* CRT handle management      */
    EDDP_LOCAL_HDB_GEN_PTR_TYPE         pGEN;       /* General handle management  */
    EDDP_LOCAL_HDB_SWI_PTR_TYPE         pSWI;       /* SWI handle management      */
    EDDP_LOCAL_HDB_PRM_PTR_TYPE         pPRM;       /* PRM handle management      */
} EDDP_HDB_TYPE;


/* defines for UsedComp (can be ored) */
#define EDDP_COMP_NRT    0x0001
#define EDDP_COMP_CRT    0x0004    /* cyclicRT */
#define EDDP_COMP_SWI    0x0008
#define EDDP_COMP_PRM    0x0010
#define EDDP_COMP_K32    0x0020
#define EDDP_COMP_GEN    0x0040

/* defines for LinkIndReqType */
#define EDDP_LINK_IND_PROVIDE_UNDEFINED     0x0000
#define EDDP_LINK_IND_PROVIDE_STANDARD      0x0001
#define EDDP_LINK_IND_PROVIDE_EXTENDED      0x0002


// global RQB-Handler for all services of cycle API
typedef struct _EDDP_GLB_CRT_RQB_HANDLER_TYPE   EDDP_LOCAL_MEM_ATTR *       EDDP_GLB_CRT_RQB_HANDLER_PTR_TYPE;
typedef struct _EDDP_GLB_CRT_RQB_HANDLER_TYPE
{
    EDDP_RQB_QUEUE_TYPE             RqbQueue;
    EDDP_RQB_QUEUE_TYPE             RqbInProgressQueue;
} EDDP_GLB_CRT_RQB_HANDLER_TYPE;
/* RqbQueue           : all RQBs are queued in this queue                    */
/* RqbInProgressQueue : all in work RQBs are queued in this queue            */

/** Set to LSA_FALSE because there is no error yet */
#define EDDP_PRM_ERR_VALID_DEFAULT      LSA_FALSE


/** @brief Detailed information on error return
 *
 * This structure is filled when return value is error_state and can be used by the calling function for tracing
 */
typedef struct _EDDP_PRM_DETAIL_ERR_TYPE
{
    /** @brief  EDD-PortID of the port related to the error
     *
     * Set to 0 if the error is not related to a port
     */
    LSA_UINT16                      ErrPortID;
    
    /** @brief Index of the record that caused the error
     *
     * Set to 0xFFFF (unspecific) if the error is caused by more than one record and selecting one would not be helpful ot track down the problem.
     */
    LSA_UINT32                      ErrIndex;
    /** @brief Offset into "record_data" (counting from 0) of record "errIndex" at port "errPortID" where the error was detected
     *
     * Set to 0 if an offset cannot be specified.
     */
    LSA_UINT32                      ErrOffset;
    /** @brief Flag if Error is Faulty
     *
     * Set to LSA_TRUE if the (normative) text "Wrong or too many parameters are written" is helpful to track down the problem.\n
     * Set to LSA_FALSE if the (normative) text "No or too less parameters are written" is helpful to track down the problem.\n
     * Set to LSA_TRUE if "err_index" == 0xFFFF (sanity)
     */
    LSA_UINT8                       ErrFault;
    /** @brief Flag if Error is Valid
     *
     * Set to LSA_TRUE if Error is Valid\n
     * Set to LSA_FALSE if Error is not Valid\n
     */
    LSA_BOOL                        ErrValid;
    /** @brief offset value for MaxBridgeDelay */
    LSA_UINT32                      OffsetMaxBridgeDelay;
    /** @brief offset value for Fragmentationon */
    LSA_UINT32                      OffsetFragmentationOn;
    /** @brief offset value for SendClockFactor */
    LSA_UINT32                      OffsetSendClockFactor;    
    /** @brief offset value for MAUType */
    LSA_UINT32                      OffsetMAUType;
} EDDP_PRM_DETAIL_ERR_TYPE;

typedef struct _EDDP_PRM_DETAIL_ERR_TYPE    EDDP_LOCAL_MEM_ATTR *    EDDP_PRM_DETAIL_ERR_PTR_TYPE;

/** macro to set the ErrorOffset
 *
 * @param   pDetailErr      pointer to struct EDDP_PRM_DETAIL_ERR_TYPE
 * @param   _ErrOffset      LSA_UINT32 value for ErrOffset
 */
#define EDDP_PRM_SET_ERR_OFFSET(pDetailErr, _ErrOffset) \
{ \
    ((EDDP_PRM_DETAIL_ERR_PTR_TYPE)(pDetailErr))->ErrOffset = (_ErrOffset); \
}

/** macro to set all values for PrmDetailErr
 *
 * @param   pDetailErr      pointer to struct EDDP_PRM_DETAIL_ERR_TYPE
 * @param   _ErrPortID      LSA_UINT16 value for ErrPortID
 * @param   _ErrIndex       LSA_UINT32 value for ErrIndex
 * @param   _ErrOffset      LSA_UINT32 value for ErrOffset
 * @param   _ErrFault       LSA_UINT8 value for ErrFault
 * @param   _ErrValid       LSA_UINT8 value for ErrValid
 */
#define EDDP_PRM_SET_DETAIL_ERR(pDetailErr, _ErrPortID, _ErrIndex, _ErrOffset, _ErrFault, _ErrValid) \
{ \
    ((EDDP_PRM_DETAIL_ERR_PTR_TYPE)(pDetailErr))->ErrPortID = (_ErrPortID); \
    ((EDDP_PRM_DETAIL_ERR_PTR_TYPE)(pDetailErr))->ErrIndex  = (_ErrIndex); \
    ((EDDP_PRM_DETAIL_ERR_PTR_TYPE)(pDetailErr))->ErrOffset = (_ErrOffset); \
    ((EDDP_PRM_DETAIL_ERR_PTR_TYPE)(pDetailErr))->ErrFault  = (_ErrFault); \
    ((EDDP_PRM_DETAIL_ERR_PTR_TYPE)(pDetailErr))->ErrValid  = (_ErrValid); \
}

/** macro to first init PrmDetailErr with default values
 *
 * @param   pDetailErr      pointer to struct EDDP_PRM_DETAIL_ERR_TYPE
 */
#define EDDP_PRM_INIT_DETAIL_ERR(pDetailErr) \
{ \
    ((EDDP_PRM_DETAIL_ERR_PTR_TYPE)(pDetailErr))->ErrPortID = EDD_PRM_ERR_PORT_ID_DEFAULT; \
    ((EDDP_PRM_DETAIL_ERR_PTR_TYPE)(pDetailErr))->ErrIndex  = EDD_PRM_ERR_INDEX_DEFAULT; \
    ((EDDP_PRM_DETAIL_ERR_PTR_TYPE)(pDetailErr))->ErrOffset = EDD_PRM_ERR_OFFSET_DEFAULT; \
    ((EDDP_PRM_DETAIL_ERR_PTR_TYPE)(pDetailErr))->ErrFault  = EDD_PRM_ERR_FAULT_DEFAULT; \
    ((EDDP_PRM_DETAIL_ERR_PTR_TYPE)(pDetailErr))->ErrValid  = EDDP_PRM_ERR_VALID_DEFAULT; \
}

/*===========================================================================*/
/*                                                                           */
/* Device-description block (DDB)                                            */
/*                                                                           */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* Interrupt Management infos                                                */
/*---------------------------------------------------------------------------*/

/* see eddp_isr.h */

/*---------------------------------------------------------------------------*/
/* Timer Management infos                                                    */
/*---------------------------------------------------------------------------*/

typedef struct _EDDP_DDB_CORE_TIMER_TYPE    EDDP_LOCAL_MEM_ATTR *    EDDP_LOCAL_DDB_CORE_TIMER_PTR_TYPE;
typedef struct _EDDP_DDB_CORE_TIMER_TYPE
{
    LSA_BOOL                    ShutdownPending; /* TRUE if on shutdown if we*/
                                                 /* have to wait for all RQBS*/
                                                 /* no more in use.          */
    LSA_UINT32                  RQBInUseCnt;     /* Number of TimerRQBs      */
                                                 /* in Use (via DO-Macro) on */
                                                 /* shutdown.                */
} EDDP_DDB_CORE_TIMER_TYPE;


/*---------------------------------------------------------------------------*/
/* HAL   Management infos                                                    */
/*---------------------------------------------------------------------------*/

/* see eddp_hal_inc.h */


typedef struct _EDDP_DDB_CORE_TYPE    EDDP_LOCAL_MEM_ATTR *    EDDP_LOCAL_DDB_CORE_PTR_TYPE;
typedef struct _EDDP_DDB_CORE_TYPE
{
    LSA_UINT32                             CmpOpen;         /* OpenComponents bitfield   */
    LSA_UINT32                             CmpSetup;        /* SetupComponent bitfield   */
    LSA_UINT32                             HandleCnt;       /* Number of Handles in use  */
                                                            
                                                            /* --- Setup --------------- */
    EDDP_DDB_CORE_SM_SETUP_TYPE            Setup;           /* DEV_SETUP Statemachine    */
    EDDP_DDB_CORE_SM_SHUTDOWN_TYPE         Shutdown;        /* DEV_SHUTDOWN Statemachine */
    EDDP_DDB_CORE_TIMER_TYPE               Timer;           /* --- Timerhandling ------- */
    EDDP_DDB_CORE_ISR_TYPE                 Isr;             /* --- Interrupthandling --- */
    EDDP_DDB_CORE_HAL_TYPE                 Hal;             /* --- HAL           ------- */
    EDDP_LOCAL_GET_STATISTICS_PTR_TYPE     pShadowStatistic; /* Memory for all Ports and an Interface,
                                                                used for the synchronous get+reset statistic counters*/
} EDDP_DDB_CORE_TYPE;


/* CmpOpen */
#define EDDP_DDB_CMP_CORE_OPEN         0x00000001
#define EDDP_DDB_CMP_NRT_OPEN          0x00000002
#define EDDP_DDB_CMP_CRT_OPEN          0x00000004
#define EDDP_DDB_CMP_SWI_OPEN          0x00000008
#define EDDP_DDB_CMP_PRM_OPEN          0x00000010
#define EDDP_DDB_CMP_GEN_OPEN          0x00000020
#define EDDP_DDB_CMP_K32_OPEN          0x00000040
#define EDDP_DDB_CMP_CMD_OPEN          0x00000080

/* CmpSetup Mask */
#define EDDP_DDB_CMP_CORE_SETUP        0x00000001
#define EDDP_DDB_CMP_CORE_ISR_SETUP    0x00000002
#define EDDP_DDB_CMP_CORE_TIMER_SETUP  0x00000004
#define EDDP_DDB_CMP_NRT_SETUP         0x00000008
#define EDDP_DDB_CMP_SWI_SETUP1        0x00000010
#define EDDP_DDB_CMP_SWI_SETUP2        0x00000020 
#define EDDP_DDB_CMP_CRT_SETUP         0x00000040
#define EDDP_DDB_CMP_GEN_SETUP         0x00000080
#define EDDP_DDB_CMP_K32_SETUP         0x00000100
#define EDDP_DDB_CMP_CMD_SETUP         0x00000200
#define EDDP_DDB_CMP_HAL_SETUP         0x00000400

/* CmpSetup Mask for interface setup states within device setup state machine */
#define EDDP_IDB_CMP_CORE_SETUP        0x00001000
#define EDDP_IDB_CMP_CORE_ISR_SETUP    0x00002000
#define EDDP_IDB_CMP_NRT_SETUP         0x00004000
#define EDDP_IDB_CMP_SWI_SETUP         0x00008000
#define EDDP_IDB_CMP_CRT_SETUP         0x00010000
#define EDDP_IDB_CMP_GEN_SETUP         0x00020000
#define EDDP_IDB_CMP_K32_SETUP         0x00040000
#define EDDP_IDB_CMP_PRM_SETUP         0x00080000


/* --------------------------------------------------------------------------*/
/* Hardware configuration                                                    */
/* --------------------------------------------------------------------------*/
typedef struct _EDDP_DDB_HW_CFG_IRQX_CONTROL_TYPE
{
    LSA_UINT8       IRQ2_Level;
    LSA_UINT8       IRQ2_Length;
    LSA_UINT8       IRQ3_Level;
    LSA_UINT8       IRQ3_Length;
} EDDP_DDB_HW_CFG_IRQX_CONTROL_TYPE;
/* IRQx_Level       : For configuration of PNIP-Register "IRQx_Control.IRQ_Level"   - see <eddp_sys.h>      */
/* IRQx_Length      : For configuration of PNIP-Register "IRQx_Control.IRQx_Length" - see <eddp_sys.h>      */
// min. value for IRQx_Control.IRQ3_Length for HERA (same value for Asic and FPGA)
#define EDDP_HW_CFG_IRQX_CONTROL_IRQ3_LENGTH_MIN_REV3           3
// max. value for IRQx_Control.IRQ_Length
#define EDDP_HW_CFG_IRQX_CONTROL_IRQ_LENGTH_MAX                 7
// default value for IRQx_Control.IRQ_Length if Hardware=FPGA
#define EDDP_HW_CFG_IRQX_CONTROL_IRQ_LENGTH_FPGA_DEFAULT        (EDDP_HW_CFG_IRQX_CONTROL_IRQ_LENGTH_MAX)


typedef struct _EDDP_DDB_HW_CFG_TYPE
{
    EDDP_DDB_HW_CFG_IRQX_CONTROL_TYPE       IRQx_Control;
} EDDP_DDB_HW_CFG_TYPE;


/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Device-description block                                                  */
/*                                                                           */
/*---------------------------------------------------------------------------*/
typedef struct _EDDP_DDB_TYPE
{
    LSA_UINT32                      InterfaceID;                /* Interface ID provided by  */
                                                                /* system adaption           */
    LSA_UINT32                      TraceIdx;                   /* TraceIdx ID               */
    EDDP_HANDLE                     hDDB;                       /* Device-handle (currently  */
                                                                /* pointer) to EDD_DDB_TYPE  */
    EDDP_SYS_HANDLE                 hSysDev;                    /* from DPB                  */
    LSA_UINT32                      DevId;                      /* EDD internal DeviceID.    */
                                                                /* 0..x for Debugging        */
    EDD_COMMON_MEM_PTR_TYPE         pPnipBase;                  /* Pointer to PNIP internal  */
                                                                /* 2 MByte RAM region        */
    LSA_UINT16                      EDDPLocation;               /* Location of EDDP          */
                                                                /* EDDP_LOCATION_LOCAL       */
                                                                /* EDDP_LOCATION_EXT_HOST    */
    LSA_UINT32                      Hardware;                   /* internal HW identifier    */
                                                                /* #defines see DPB.         */
                                                                /* e.g. EDDP_HW_ERTEC200P    */
    LSA_UINT32                      HardwareType;               /* external HW identifier    */
                                                                /* #defines see edd_usr.h    */
    LSA_UINT16                      MaxPortCntOfAllEDD;         /* from DPB     */
    LSA_UINT16                      MaxInterfaceCntOfAllEDD;    /* from DPB     */
    LSA_BOOL                        DevIsSetup;                 /* LSA_TRUE if device_setup  */
                                                                /* was successful and device */
                                                                /* is setup.                 */
    #ifdef EDDP_CFG_DETAIL_ERROR_INFO
    EDDP_DBG_DETAIL_ERR_TYPE        ErrDetail;                  /* Detail error information  */
                                                                /* of last error             */
    #endif

    EDDP_DDB_CORE_TYPE              Core;                       /* Core Device management    */
    EDDP_DDB_SWI_TYPE               SWI;                        /* Switch device management  */
    EDDP_DDB_NRT_TYPE               NRT;                        /* NRT-specific management   */
    EDDP_DDB_K32_TYPE               K32;                        /* K32-specific management   */
    EDDP_DDB_CMD_TYPE               CMD;                        /* CMD-specific management   */
    EDDP_DDB_CRT_TYPE               CRT;                        /* CRT-specific management   */
    EDDP_DDB_PRM_TYPE               PRM;                        /* PRM-specific management   */
    EDDP_DDB_I2C_TYPE               I2C;                        /* I2C-specific management   */

    EDDP_GLB_CRT_RQB_HANDLER_TYPE   GlbCrtRQBHandler;           /* RQB-Hanlder for all services of cycle API */
    EDDP_PRM_DETAIL_ERR_TYPE        PrmDetailErr;               /**< detailed information on error */

    EDDP_DDB_HW_CFG_TYPE            HWCfg;                      /* Hardware parameter        */
    EDDP_GSHAREDMEM_TYPE           *pGSharedRAM;                /* Base of 2proc shared mem for SRD API  */ 
    EDDP_USERCMDSHAREDMEM_TYPE     *pUserCmdSharedMem;          /* Shared Mem for EDDP UserCmd communication */ 

    EDDP_SWI_PHY_STS_SM_TYPE        PhyStatusSm[EDDP_MAX_PORT_CNT];                /* statemachine for swi and core */
    EDDP_PRM_PORT_MODULE_SM_TYPE    PortModuleSm[EDDP_MAX_PORT_CNT]; /* statemachine for PRM PortModule states */

    EDDP_DPB_FEATURE_TYPE           FeatureSupport;             /* Supported features */
} EDDP_DDB_TYPE;

/* defines used to refer pDDB->IFId. */
#define EDDP_IF_A           0
//#define EDDP_IF_B           1

/*===========================================================================*/
/*                                                                           */
/* Basetype of EDDP management                                               */
/*                                                                           */
/* Notes: The Structure will be allocated on eddp_init.                      */
/*        Individual DDBs will be allocated on DeviceOpen.                   */
/*                                                                           */
/*        HDBs (Channels) and Timer management is global for all EDDP Devices*/
/*                                                                           */
/*===========================================================================*/

/* Timers used by EDDP: */
/* LED-Blink                    :  Dev: -   IF: x */
/*                              :  Dev:     IF:   */
/* PRM-FrameDrop                :  Dev: -   IF: x */
/*                              :  Dev:     IF:   */
/* CRT: INT_Ppm_HOL_not_empty   :  Dev: -   IF: x */
/*                              :  Dev:     IF:   */
/* CRT: INT_Ppm_not_finished    :  Dev: -   IF: x */
/*                              :  Dev:     IF:   */
/* CRT: INT_InBound_Late_ERR    :  Dev: x   IF: - */
/*                              :  Dev:     IF:   */
/* CRT: Wrkar_PackGrp_Disable   :  Dev: -   IF: x */
/*                              :  Dev:     IF:   */
/* ---------------------------------------------- */
/* Sum                          :  Dev: 1   IF: 5 */

#define EDDP_DEVICE_TIMERS      6       /* Device global timers             */

#define EDDP_MAX_TIMER          (EDDP_CFG_MAX_DEVICES * EDDP_DEVICE_TIMERS)

typedef struct _EDDP_INFO_TYPE  EDDP_LOCAL_MEM_ATTR * EDDP_LOCAL_INFO_PTR_TYPE;
typedef struct _EDDP_INFO_TYPE
{
    LSA_UINT32                      DDBUsedCnt;                 /* number of used DDB   */
    LSA_UINT32                      HDBUsedCnt;                 /* number of used HDB   */
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB[EDDP_CFG_MAX_DEVICES]; /* ptr-array of devices */
    EDDP_HDB_TYPE                   HDB[EDDP_CFG_MAX_CHANNELS]; /* LSA-Handlemanagement */
    LSA_UINT32                      TimerUsedCnt;               /* Number of Timer in use         */
    EDDP_TIMER_INFO_TYPE            Timer[EDDP_MAX_TIMER];      /* poiner to array of TimerInfos  */
    EDDP_LOCKING_TYPE               Locking;                    /* Reentrance check/locking       */
} EDDP_INFO_TYPE;


/*****************************************************************************/
/*  end of file eddp_core_dev.h                                              */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_CORE_DEV_H */

