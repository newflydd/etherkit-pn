
#ifndef EDDP_ISR_H                       /* ----- reinclude-protection ----- */
#define EDDP_ISR_H

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
/*  F i l e               &F: eddp_core_isr.h                           :F&  */
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
/*  defines constants, types, macros and prototyping for EDDP Interrupt      */
/*  handling                                                                 */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  15.08.08    JS    initial version.                                       */
/*  01.04.09    JS    adapted to PNSpec V1.2                                 */
#endif
/*****************************************************************************/


/*===========================================================================*/
/*                                defines                                    */
/*===========================================================================*/


#define EDDP_ISR_LOW_MAX_EVENTS    32
#define EDDP_ISR_LOW_MAX_MSK       0xFFFFFFFFl   /* 32 Events */

#define EDDP_ISR_MID_MAX_EVENTS    32
#define EDDP_ISR_MID_MAX_MSK       0xFFFFFFFFl   /* 32 Events */

#define EDDP_ISR_HIGH_MAX_EVENTS   22
#define EDDP_ISR_HIGH_MAX_MSK      0x003FFFFFl   /* 22 Events */


#define EDDP_ISR_MAX_EVENTS        (EDDP_ISR_LOW_MAX_EVENTS + \
                                    EDDP_ISR_MID_MAX_EVENTS + \
                                    EDDP_ISR_HIGH_MAX_EVENTS)


/*---------------------------------------------------------------------------*/
/* Interrupt Events (must match EDDP_IsrFctTable!)                           */
/*---------------------------------------------------------------------------*/

/* LOW ----------------------------------------------------------------------*/
/* PHY polling */
#define EDDP_INT_STATUSCHANGE              0

/* Commandhandling */
#define EDDP_INT_REQDONE_IFA               1 
#define EDDP_INT_ARCVCMD_DONE              3
#define EDDP_INT_ASNDCMD_DONE              4
#define EDDP_INT_CPMCMD_DONE               5
#define EDDP_INT_PPMCMD_DONE               6
#define EDDP_INT_PACKCMD_DONE              7
#define EDDP_INT_SYNCCMD_DONE              8
#define EDDP_INT_FDBCMD_DONE               9
#define EDDP_INT_VLANCMD_DONE             10
#define EDDP_INT_FILTERCMD_DONE           11

/* Errorhandling */
#define EDDP_INT_PPM_TXD_UNDERRUN_REV3    12 /* Rev3 only! */
#define EDDP_INT_MAC_ERR                  13
#define EDDP_INT_REQQUEUE_ERR             14
#define EDDP_INT_CPM_ERR                  15
#define EDDP_INT_PPM_ERR                  16
#define EDDP_INT_PACK_ERR                 17
#define EDDP_INT_IRT_DELAYSTATUS          18
#define EDDP_INT_FRAGFAULT                19
#define EDDP_INT_PHASETRANSFERERROR       20
#define EDDP_INT_PHASECROSSED             21
#define EDDP_INT_PPM_LIFETIME_ERR_REV3    22 /* Rev3 only! */
#define EDDP_INT_PPM_SEQID_ERR_REV3       23 /* Rev3 only! */

/* Ressources */	
#define EDDP_INT_QCW_EMPTY                24
#define EDDP_INT_HOL_EXPIRED_IFA          25
#define EDDP_INT_HOL_EXPIRED_PA           27
#define EDDP_INT_HOL_EXPIRED_PB           28
#define EDDP_INT_FRAMEAGING_EXPIRED       29

#define EDDP_INT_INBOUND_LATE_ERR         31

/* MID ----------------------------------------------------------------------*/
/* API filter */         
#define EDDP_INT_APIFILTER_ERR            (0 + EDDP_ISR_LOW_MAX_EVENTS)
/* azyklisches API	*/
#define EDDP_INT_ACYC_RCV_IFA             (2 + EDDP_ISR_LOW_MAX_EVENTS)
#define EDDP_INT_ACYC_SND_IFA             (3 + EDDP_ISR_LOW_MAX_EVENTS)
#define EDDP_INT_ACYC_RCV_ERR             (6 + EDDP_ISR_LOW_MAX_EVENTS)
#define EDDP_INT_ACYC_SND_ERR             (7 + EDDP_ISR_LOW_MAX_EVENTS)

/* zyklisches API */
#define EDDP_INT_CPM_OUTDATASTART                          (9  + EDDP_ISR_LOW_MAX_EVENTS)
#define EDDP_INT_TSB_NBUF_OVERRUN_ERR_IFA_REV3             (9  + EDDP_ISR_LOW_MAX_EVENTS) /* Rev3 only! */
#define EDDP_INT_CPM_OUTDATAVALID                          (10 + EDDP_ISR_LOW_MAX_EVENTS)
#define EDDP_INT_CPM_OUTDATAERR                            (11 + EDDP_ISR_LOW_MAX_EVENTS)
#define EDDP_INT_TSB_NOT_FINISHED_ERR_IFA_REV3             (11 + EDDP_ISR_LOW_MAX_EVENTS) /* Rev3 only! */
#define EDDP_INT_CPM_DMA_ERR                               (13 + EDDP_ISR_LOW_MAX_EVENTS)
#define EDDP_INT_CSCOREBOARD_CHANGED_IFA                   (14 + EDDP_ISR_LOW_MAX_EVENTS) 
#define EDDP_INT_PPM_NOT_FINISHED_IFA                      (16 + EDDP_ISR_LOW_MAX_EVENTS)
#define EDDP_INT_PPM_HOL_NOT_EMPTY_IFA                     (18 + EDDP_ISR_LOW_MAX_EVENTS)
#define EDDP_INT_PPM_ACWLISTCREATION_NOT_FINISHED_IFA_REV3 (20 + EDDP_ISR_LOW_MAX_EVENTS) /* Rev3 only! */
#define EDDP_INT_PPM_DMA_ERR                               (21 + EDDP_ISR_LOW_MAX_EVENTS)

/* specific Interrupt-Events */
#define EDDP_HW_EVENT0                    (24 + EDDP_ISR_LOW_MAX_EVENTS)
#define EDDP_HW_EVENT1                    (25 + EDDP_ISR_LOW_MAX_EVENTS)
#define EDDP_HW_EVENT2                    (26 + EDDP_ISR_LOW_MAX_EVENTS)
#define EDDP_HW_EVENT3                    (27 + EDDP_ISR_LOW_MAX_EVENTS)
#define EDDP_SW_EVENT0                    (28 + EDDP_ISR_LOW_MAX_EVENTS)
#define EDDP_SW_EVENT1                    (29 + EDDP_ISR_LOW_MAX_EVENTS)
#define EDDP_SW_EVENT2                    (30 + EDDP_ISR_LOW_MAX_EVENTS)
#define EDDP_SW_EVENT3                    (31 + EDDP_ISR_LOW_MAX_EVENTS)


/* HIGH ---------------------------------------------------------------------*/
/* PLL */
#define EDDP_INT_PLL_IN_A                 (0  + EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS)
#define EDDP_INT_PLL_IN_B                 (1  + EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS)
#define EDDP_INT_PLL_IN_Time              (2  + EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS)
#define EDDP_INT_PLL0                     (3  + EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS)
#define EDDP_INT_PLL1                     (4  + EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS)
#define EDDP_INT_PLL2                     (5  + EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS)
#define EDDP_INT_PLL3                     (6  + EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS)
#define EDDP_INT_PLL4                     (7  + EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS)
#define EDDP_INT_PLL5                     (8  + EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS)
#define EDDP_INT_PLL6                     (9  + EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS)
#define EDDP_INT_PLL7                     (10 + EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS)
#define EDDP_INT_PLL8                     (11 + EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS)
#define EDDP_INT_PLL9                     (12 + EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS)
#define EDDP_INT_PTCP_WD                  (15 + EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS)
 
/* KRISC-32 Support	 */
#define EDDP_INT_WD                       (16 + EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS)
#define EDDP_INT_Timer_0                  (17 + EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS)
#define EDDP_INT_Timer_1                  (18 + EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS)
#define EDDP_INT_Timer_2                  (19 + EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS)
#define EDDP_INT_I2C                      (20 + EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS)
 


/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* Interrupthandler configuration:                                           */
/* constant table with EDDP interrupthandlers                                */
/*---------------------------------------------------------------------------*/

typedef struct _EDDP_ISR_EVENT_FCT_TABLE_TYPE
{
    LSA_UINT16  HandlerType;
    LSA_UINT16  IntType;    
    LSA_VOID    LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, ISREventHandler)
                           (EDDP_LOCAL_DDB_PTR_TYPE   pDDB,
                            LSA_UINT32                EventNr);
} EDDP_ISR_EVENT_FCT_TABLE_TYPE;

typedef const EDDP_ISR_EVENT_FCT_TABLE_TYPE   EDDP_LOCAL_MEM_ATTR *    EDDP_ISR_EVENT_FCT_TABLE_PTR_TYPE;
 
/* HandlerType:                                                              */
/*                                                                           */
/*                                                                           */
/* EDDP_ISR_HANDLER_TYPE_EXTERN: Not used by EDDP                            */
/*                                                                           */
/* EDDP_ISR_HANDLER_TYPE_DIRECT: The interrupt shall be handled within ISR   */
/*                                                                           */
/* EDDP_ISR_HANDLER_TYPE_LOW:    The interrupt shall be handled within LOW   */
/*                               Prio context of EDDP and has to be tranfered*/
/*                               with an internal RQB                        */

#define EDDP_ISR_HANDLER_TYPE_EXTERN 0 
#define EDDP_ISR_HANDLER_TYPE_DIRECT 1 
#define EDDP_ISR_HANDLER_TYPE_LOW    2 

/* IntType:                                                                  */

#define EDDP_ISR_TYPE_GROUP          0      /* Group interrupt               */
#define EDDP_ISR_TYPE_SINGLE         1      /* Single interrupt              */


/*===========================================================================*/
/*                                                                           */
/* Interrupt Management infos                                                */
/*                                                                           */
/*===========================================================================*/

/* --------------------------------------------------------------------------*/
/* Groupinfo. Will be setup on startup with Mask and GroupHandler            */
/*            Mask defines on which events the GroupHandler has              */
/*            to be called. This is done with GroupInterrupts                */
/* --------------------------------------------------------------------------*/

typedef struct _EDDP_GROUP_EVENTS_TYPE
{
    EDDP_HAL_ISR_GRP_REGS_TYPE Msk;      /* Interrupt event masks. Used to */
                                         /* to check if the GroupHandler   */
                                         /* has to be called for Event     */
	                                     /* 1 if the handler shall be      */
	                                     /* called                         */

    LSA_VOID    LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, GroupHandler)
                           (EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
                            EDDP_HAL_ISR_GRP_REGS_PTR_TYPE  pEvents);
} EDDP_GROUP_EVENTS_TYPE;

typedef struct _EDDP_GROUP_EVENTS_TYPE    EDDP_LOCAL_MEM_ATTR *    EDDP_GROUP_EVENTS_PTR_TYPE;

/* Group: InternRQB, InternDirekt, Hook */
#define EDDP_ISR_GROUP_EVENT_IDX_INTERN_LOW    0
#define EDDP_ISR_GROUP_EVENT_IDX_INTERN_DIRECT 1
#define EDDP_ISR_GROUP_EVENT_IDX_INTERN_UNEXP  2
#define EDDP_ISR_GROUP_EVENT_CNT               3

/* IntUserEnabledEventMsk[] and IntISREnabledEventMsk[] together build up the*/
/* HW-Mask currently set within Hardware Mask-Register:                      */            
/*                                                                           */
/* HWMask = ~(IntUserEnabledEventMsk & IntISREnabledEventMsk)                */
/*                                                                           */
/* 0: Interrupt is not masked                                                */
/* 1: Interrupt is masked                                                    */
/*                                                                           */
/* IntUserEnabledEventMsk will be modified if the user enables/disables an   */
/* interrupt used by EDDP                                                    */
/* IntISREnabledEventMsk will be modified if the interrupthandler temporary  */
/* Msk or UnMsk certain events.                                              */
/*                                                                           */
/* IntUserEnabledEventMsk   on Startup is "0" for each event.                */
/* IntISREnabledEventMsk    on Startup is "1" for each event.                */
/*                                                                           */

typedef struct _EDDP_ISR_GROUP_INFO_TYPE
{  
    EDDP_GROUP_EVENTS_TYPE      Events[EDDP_ISR_GROUP_EVENT_CNT];
    EDDP_INTERNAL_REQUEST_TYPE  IntRQB;                  /* Internal RQB for LOW transition */
    EDDP_HAL_ISR_GRP_REGS_TYPE  IntUserEnabledEventMsk;  /* Contains "1" for each Event     */
                                                         /* enabled by user (nrt,crt etc)   */
    EDDP_HAL_ISR_GRP_REGS_TYPE  IntISREnabledEventMsk;   /* Contains "1" for each Event     */
                                                         /* Enabled within ISR              */
} EDDP_ISR_GROUP_INFO_TYPE;

typedef struct _EDDP_ISR_GROUP_INFO_TYPE    EDDP_LOCAL_MEM_ATTR *    EDDP_ISR_GROUP_INFO_PTR_TYPE;


/* --------------------------------------------------------------------------*/
/* SingleInfo. Will be setup on startup with SingleHandler                   */
/* --------------------------------------------------------------------------*/

typedef struct _EDDP_ISR_SINGLE_INFO_TYPE
{
    LSA_UINT32                      EventNr;                       /* EventNr (0..79). from config for */
                                                                   /* SingleInt.                       */
 	EDDP_INTERNAL_REQUEST_TYPE      IntRQB;                        /* Internal RQB for LOW transition  */
                                                                   /* only present if EventNr <>       */
                                                                   /* EDDP_EVENT_NR_UNUSED             */
    #if defined  (EDDP_CFG_ISR_POLLING_MODE)    
    LSA_UINT32                      AckRegOffset;                  /* Offset of acknowledge register          */
    LSA_UINT32                      AckRegValue;                   /* Value for acknowledging single int      */
    LSA_UINT32                      IsrRegOffset;                  /* Offset of ISR register                  */
    LSA_UINT32                      IsrRegMask;                    /* Mask for checking if single int occured */
    #endif //(EDDP_CFG_ISR_POLLING_MODE)

    LSA_VOID    LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, SingleHandler)
                           (EDDP_LOCAL_DDB_PTR_TYPE   pDDB,
                            LSA_UINT32                IntSource);
} EDDP_ISR_SINGLE_INFO_TYPE;

typedef struct _EDDP_ISR_SINGLE_INFO_TYPE    EDDP_LOCAL_MEM_ATTR *    EDDP_ISR_SINGLE_INFO_PTR_TYPE;

/* EventNr: */
#define EDDP_EVENT_NR_UNUSED  0xFFFFFFFF

/* --------------------------------------------------------------------------*/
/* Interrupt Management main infos (located within DDB)                      */
/* --------------------------------------------------------------------------*/

/* Min/Max Single Int */
#define EDDP_ISR_MIN_SINGLE_INT               2  
#define EDDP_ISR_MAX_SINGLE_INT              15  

#define EDDP_ISR_SINGLE_INFO_CNT (EDDP_ISR_MAX_SINGLE_INT) 

typedef struct _EDDP_DDB_CORE_ISR_TYPE
{
    LSA_BOOL                         Active;                /* ISR-Handler active?          */
    LSA_BOOL                         ShutdownPending;       /* TRUE if on shutdown if we    */
                                                            /* have to wait for all RQBS    */
                                                            /* no more in use.              */
    EDDP_ISR_GROUP_INFO_TYPE         Group;                 /* For GroupInterrupt handling  */

    LSA_UINT32                       IRQAcycRcv;            /* comes from IPB */
    LSA_UINT32                       IRQ_WaitTime_AcycRcv;  /* comes from ISB */

                                                            /* Single Interrupt handling    */
    EDDP_ISR_SINGLE_INFO_TYPE        Single[EDDP_ISR_SINGLE_INFO_CNT];  /* index 0 and 1 not used ! */
    
    EDDP_ISR_EVENT_FCT_TABLE_TYPE const * pIsrFuncTable;     /* revision dependent interrupt function table */

} EDDP_DDB_CORE_ISR_TYPE;

typedef struct _EDDP_DDB_CORE_ISR_TYPE    EDDP_LOCAL_MEM_ATTR *    EDDP_LOCAL_DDB_CORE_ISR_PTR_TYPE;


/*===========================================================================*/
/*                                prototoypes                                */
/*===========================================================================*/

LSA_EXTERN LSA_VOID  EDDP_LOCAL_FCT_ATTR EDDP_CoreISRRequest(
    EDD_UPPER_RQB_PTR_TYPE      pRQB);

LSA_EXTERN EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreISRDeviceShutdown(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_EXTERN EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreISRDeviceSetup(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_UPPER_DSB_PTR_TYPE     pDSB);

LSA_EXTERN EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreISRDeviceClose(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_EXTERN EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreISRDeviceOpen(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_UPPER_DPB_PTR_TYPE     pDPB);

LSA_EXTERN EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreISRDeviceIFSetup(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_UPPER_DSB_PTR_TYPE     pDSB);

LSA_EXTERN EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreISRDeviceIFShutdown(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreISRControlInterrupt(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_BOOL                    Enable,
	LSA_UINT32                  EventNr);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreISRFatalHandler(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT32                  EventNr);

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreISRTestHandler(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT32                  EventNr);

/*****************************************************************************/
/*  end of file eddp_core_isr.h                                              */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_ISR_H */
