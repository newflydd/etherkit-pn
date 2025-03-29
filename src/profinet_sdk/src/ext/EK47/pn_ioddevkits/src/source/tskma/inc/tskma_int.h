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
/*  F i l e               &F: tskma_int.h                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*   TSKMA internal header                                                   */
/*                                                                           */
/*****************************************************************************/


#ifndef TSKMA_INT_H_
#define TSKMA_INT_H_

//*----------------------------------------------------------------------------
//* opcodes
//*----------------------------------------------------------------------------

#define TSKMA_OPC_APP_INIT                                  0x00
#define TSKMA_OPC_APP_OPEN_01                               0x01
#define TSKMA_OPC_APP_OPEN_02                               0x02
#define TSKMA_OPC_APP_START                                 0x03
#define TSKMA_OPC_APP_STARTUP_STOPPED                       0x04
#define TSKMA_OPC_APP_CYCLIC_EA                             0x05
#define TSKMA_OPC_APP_BACKUP_MATCH_NOS                      0x06
#define TSKMA_OPC_APP_BLINK_TIMEOUT                         0x07
#define TSKMA_OPC_APP_RESET_TO_FACTORY                      0x08
#define TSKMA_OPC_APP_TRIGGER_PNPB                          0x09
#define TSKMA_OPC_APP_TRIGGER_PNDV                          0x0A
#define TSKMA_OPC_APP_INIT_EA                               0x0B

#define TSKMA_OPC_EDD_REQ_RESERVED                          0x10
#define TSKMA_OPC_EDD_REQ_REBOOT                            0x11

#define TSKMA_OPC_ORG_REQ_RESERVED                          0x20

#define TSKMA_OPC_PNO_REQ_OPEN                              0x30

#define TSKMA_OPC_TCP_OPEN                                  0x41
#define TSKMA_OPC_TCP_SOCK_TIMEOUT                          0x45
#define TSKMA_OPC_TCP_TCIP_TIMEOUT                          0x46

#define TSKMA_OPC_POF_REQ_RESERVED                          0x50

#define TSKMA_OPC_REM_INIT                                  0x61
#define TSKMA_OPC_REM_FWUM_FWUP_OPEN                        0x63
#define TSKMA_OPC_REM_FWUM_START                            0x64
#define TSKMA_OPC_REM_FWUP_START                            0x65
#define TSKMA_OPC_REM_FS_GET_START_PAR                      0x66
#define TSKMA_OPC_REM_FBS_GET_START_PAR                     0x67
#define TSKMA_OPC_REM_FBS_WRITE_SDB                         0x68
#define TSKMA_OPC_REM_FBS_DELETE_SDB                        0x69
#define TSKMA_OPC_REM_FS_WRITE_FILE                         0x6A
#define TSKMA_OPC_REM_FS_DELETE_FILE                        0x6B
#define TSKMA_OPC_REM_FS_RESET_TO_FACTORY                   0x6C
#define TSKMA_OPC_REM_FBS_RESET_TO_FACTORY                  0x6D

#define TSKMA_OPC_STP_PERFORM_COLD_START 0x71


#define TSKMA_MODULE_ID_TSKMA_IRTE_ISR  0x01
#define TSKMA_MODULE_ID_TSKMA_TASK_APP  0x02
#define TSKMA_MODULE_ID_TSKMA_TASK_EDD  0x03
#define TSKMA_MODULE_ID_TSKMA_TASK_IDL  0x04
#define TSKMA_MODULE_ID_TSKMA_TASK_ORG  0x05
#define TSKMA_MODULE_ID_TSKMA_TASK_PNO  0x06
#define TSKMA_MODULE_ID_TSKMA_TASK_POF  0x07
#define TSKMA_MODULE_ID_TSKMA_TASK_REM  0x08
#define TSKMA_MODULE_ID_TSKMA_TASK_STP  0x09
#define TSKMA_MODULE_ID_TSKMA_TASK_TCP  0x0A
#define TSKMA_MODULE_ID_TSKMA           0x0B
#define TSKMA_MODULE_ID_TSKMA_PDB       0x0C
#define TSKMA_MODULE_ID_TSKMA_DAT       0x0D


//*----------------------------------------------------------------------------
//* types
//*----------------------------------------------------------------------------

typedef enum TSKMA_COLD_START_SM_E
{
	TSKMA_COLD_START_SM_CLOSED,
	TSKMA_COLD_START_SM_ALLOC,
	TSKMA_COLD_START_SM_APP_INIT,
	TSKMA_COLD_START_SM_FS_FBS_INIT,
	TSKMA_COLD_START_SM_INIT_PNIO_STACK,
	TSKMA_COLD_START_SM_FS_FBS_OPEN,
	TSKMA_COLD_START_SM_FS_OPEN,
	TSKMA_COLD_START_SM_FS_GET_START_PAR,
	TSKMA_COLD_START_SM_SETUP_PNIO_STACK,
	TSKMA_COLD_START_SM_APP_OPEN_01,
	TSKMA_COLD_START_SM_FS_FBS_FWUM_FWUP_OPEN,
	TSKMA_COLD_START_SM_FWUM_FWUP_OPEN_DONE,
	TSKMA_COLD_START_SM_FS_FBS_FWUM_START,
	TSKMA_COLD_START_SM_FBS_GET_START_PAR,
	TSKMA_COLD_START_SM_APPL_RESET_TO_FACTORY,
	TSKMA_COLD_START_SM_FS_RESET_TO_FACTORY,
	TSKMA_COLD_START_SM_FBS_RESET_TO_FACTORY,
	TSKMA_COLD_START_SM_APP_BACKUP_MATCH_NOS,
	TSKMA_COLD_START_SM_TCP_OPEN,
	TSKMA_COLD_START_SM_PNO_REQ_OPEN,
	TSKMA_COLD_START_SM_OPEN_PNIO_STACK,
	TSKMA_COLD_START_SM_APP_OPEN_02,
	TSKMA_COLD_START_SM_APP_START,
	TSKMA_COLD_START_SM_FS_FBS_FWUP_START,
	TSKMA_COLD_START_SM_INIT_TIMER,
	TSKMA_COLD_START_SM_APP_CYCLIC_EA,
	TSKMA_COLD_START_SM_DONE,

} TSKMA_COLD_START_SM_E_T;


typedef enum TSKMA_TS_SCHEDULING_STATE_E
{
    TSKMA_TS_SCHEDULING_STATE_APPLICATION_TASK_RUNNING,
    TSKMA_TS_SCHEDULING_STATE_APPLICATION_TASK_FINISHED,
    TSKMA_TS_SCHEDULING_STATE_APPLICATION_TASK_SUSPENDED,
    TSKMA_TS_SCHEDULING_STATE_TS_TASK_FINISHED

} TSKMA_TS_SCHEDULING_STATE_E_T;


//*----------------------------------------------------------------------------
//* prototypes
//*----------------------------------------------------------------------------

LSA_VOID 	tskma_pdb_setup_pndv	(LSA_VOID);
LSA_VOID 	tskma_pdb_open			(LSA_VOID);

LSA_VOID    tskma_task_app_init				(LSA_VOID);
LSA_VOID    tskma_task_app_open_1			(LSA_VOID);
LSA_VOID    tskma_task_app_open_2			(LSA_VOID);
LSA_VOID    tskma_task_app_start			(LSA_VOID);
LSA_VOID    tskma_task_app_startup_stopped	(LSA_VOID);
LSA_VOID    tskma_task_app_cyclic_ea		(LSA_VOID);
LSA_VOID    tskma_task_app_backup_match_nos	(LSA_VOID);
LSA_VOID    tskma_task_app_blink_timeout	(LSA_VOID);
LSA_VOID    tskma_task_app_reset_to_factory	(LSA_VOID);

LSA_VOID    tskma_task_app_request (TSKMA_RQB_S_PTR_T rqb_ptr);
LSA_VOID    tskma_task_edd_request (TSKMA_RQB_S_PTR_T rqb_ptr);
LSA_VOID    tskma_task_idl_request (TSKMA_RQB_S_PTR_T rqb_ptr);
LSA_VOID    tskma_task_org_request (TSKMA_RQB_S_PTR_T rqb_ptr);
LSA_VOID    tskma_task_pno_request (TSKMA_RQB_S_PTR_T rqb_ptr);
LSA_VOID    tskma_task_pof_request (TSKMA_RQB_S_PTR_T rqb_ptr);
LSA_VOID    tskma_task_rem_request (TSKMA_RQB_S_PTR_T rqb_ptr);
LSA_VOID    tskma_task_stp_request (TSKMA_RQB_S_PTR_T rqb_ptr);
LSA_VOID    tskma_task_tcp_request (TSKMA_RQB_S_PTR_T rqb_ptr);

LSA_VOID    tskma_task_app (LSA_VOID);
LSA_VOID    tskma_task_edd (LSA_VOID);
LSA_VOID    tskma_task_idl (LSA_VOID);
LSA_VOID    tskma_task_org (LSA_VOID);
LSA_VOID    tskma_task_pno (LSA_VOID);
LSA_VOID    tskma_task_pof (LSA_VOID);
LSA_VOID    tskma_task_rem (LSA_VOID);
LSA_VOID    tskma_task_stp (LSA_VOID);
LSA_VOID    tskma_task_tcp (LSA_VOID);

LSA_VOID 	tskma_fatal (LSA_UINT16 tskma_module_id, LSA_INT line, LSA_UINT32 ec_0);

#endif /* TSKMA_INT_H_ */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
