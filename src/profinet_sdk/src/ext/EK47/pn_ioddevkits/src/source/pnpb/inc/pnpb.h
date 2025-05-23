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
/*  F i l e               &F: pnpb.h                                    :F&  */
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
#ifndef PNPB_H
#define PNPB_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif



#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif


// *------------------------------------------
// *PNPB state machine
// *------------------------------------------
typedef enum
{
    PNPB_OFF = 0,
    PNPB_INIT,
    PNPB_STOP,
    PNPB_OPEN,
    PNPB_PNDV_STARTED,
    PNPB_RDY_FOR_PLUG,
    PNPB_PDEV_STARTED,
    PNPB_OPERATING
}PNPB_STATE;

typedef enum
{
    PNPB_AR_OFFLINE = 0,
    PNPB_AR_CONNECT_IND,
    PNPB_AR_OWNERSHIP_STARTED,
    PNPB_AR_OWNERSHIP_FINISHED,

    PNPB_AR_PARAM_END_STARTED,
    PNPB_AR_PARAM_END_FINISHED,
    PNPB_AR_SM_INDATA_IND,
    PNPB_AR_INDATA_IND,
}PNPB_AR_STATE;

// *-------------------------------------------------
// *	typedef for PNPB instance data
// *-------------------------------------------------
typedef enum
{
    SEM_PNPB_PERIF = 0,
    SEM_PNPB_START,
    SEM_PNPB_PLUG,
    SEM_PNPB_DIAG_AL,
    SEM_PNPB_PR_AL,
    SEM_PNPB_GEN_AL,
    SEM_PNPB_RSP_TASK,
    SEM_PNPB_MAXNUM         // must be last element
} PNPB_SEM;

typedef struct _PNPB_INSTANCE
{
	// *** device identification
    PNPB_STATE		State;                 // not used yet
    PNIO_UINT32     SemId[SEM_PNPB_MAXNUM];
    PNIO_UINT32     SemEventAvailable;
	PNIO_UINT32		ArPending;				/* Number of last AR with connect RQ */
    PNPB_AR_STATE   ArState[ IOD_CFG_NUMOF_AR + 1 ];
    void            (*OpenDoneCbf)(void);
} PNPB_INSTANCE;


#define PNPB_ASSERT(cond) { \
	if (!(cond))            \
        PNPB_SYS_TRACE_00(LSA_TRACE_LEVEL_FATAL,  "PNPB assert occured"); }



PNPB_STATE      pnpb_get_state            (void);


PNIO_UINT32 pnpb_set_dev_state (PNIO_UINT32 DevHndl,    // device handle
								PNIO_UINT32 DevState);  // device state CLEAR/OPERATE



#endif


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
