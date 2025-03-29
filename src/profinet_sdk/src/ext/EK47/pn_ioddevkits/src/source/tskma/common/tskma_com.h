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
/*  F i l e               &F: tskma_com.h                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*   TSKMA interface header                                         		 */
/*                                                                           */
/*****************************************************************************/


#ifndef TSKMA_COM_H_
#define TSKMA_COM_H_

#ifndef TSKMA_COM_EXT_ATTR
    #define TSKMA_COM_EXT_ATTR extern
#else
    #define TSKMA_COM_EXT_ATTR_DECLARE
#endif

/* defining the type of a mail element */

typedef LSA_VOID_PTR_TYPE TSKMA_VOID_PTR_TYPE;

typedef struct TSKMA_MAIL_ELEM_S * TSKMA_MAIL_ELEM_S_PTR_T;

typedef struct TSKMA_MAIL_ELEM_S
{
    TSKMA_RQB_HEADER
    /* don't use any more elements, this is only intended to access the rqb header
     * other elements are private !!
     */
} TSKMA_MAIL_ELEM_S_T;


typedef struct TSKMA_RQB_S * TSKMA_RQB_S_PTR_T;

typedef struct TSKMA_RQB_S
{
    TSKMA_RQB_HEADER
    LSA_BOOL  static_rqb_in_use; // can be used with static rqbs to indicate that it is in use
    TSKMA_RQB_TRAILER
} TSKMA_RQB_S_T;


//=================================================================================================
#define TSKMA_TASK_NUM_APP                    0 /* application task */
#define TSKMA_TASK_ID_APPLICATION             tskma_com_data.task_info[TSKMA_TASK_NUM_APP].task_id
//-------------------------------------------------------------------------------------------------
// !!! IMPORTANT: all tasks below the application task are time slicing tasks if enabled !!!
//-------------------------------------------------------------------------------------------------
#define TSKMA_TASK_NUM_EDD                    1 /* EDD task */
#define TSKMA_TASK_ID_EDD                     tskma_com_data.task_info[TSKMA_TASK_NUM_EDD].task_id
//-------------------------------------------------------------------------------------------------
#define TSKMA_TASK_NUM_ORG                    2 /* EDD-ORG task */
#define TSKMA_TASK_ID_ORG                     tskma_com_data.task_info[TSKMA_TASK_NUM_ORG].task_id
//-------------------------------------------------------------------------------------------------
#define TSKMA_TASK_NUM_PNO                    3 /* pnio task  */
#define TSKMA_TASK_ID_PNIO                    tskma_com_data.task_info[TSKMA_TASK_NUM_PNO].task_id
//-------------------------------------------------------------------------------------------------
#define TSKMA_TASK_NUM_TCP                    4 /* TCP/IP task */
#define TSKMA_TASK_ID_TCPIP                   tskma_com_data.task_info[TSKMA_TASK_NUM_TCP].task_id
//-------------------------------------------------------------------------------------------------
#define TSKMA_TASK_NUM_POF                    5 /* POF task */
#define TSKMA_TASK_ID_POF                     tskma_com_data.task_info[TSKMA_TASK_NUM_POF].task_id
//-------------------------------------------------------------------------------------------------
#define TSKMA_TASK_NUM_REM                    6 /* Remanence task */
#define TSKMA_TASK_ID_REMA                    tskma_com_data.task_info[TSKMA_TASK_NUM_REM].task_id
//-------------------------------------------------------------------------------------------------
#define TSKMA_TASK_NUM_STP                    7 /* startup and control task */
#define TSKMA_TASK_ID_STARTUP                 tskma_com_data.task_info[TSKMA_TASK_NUM_STP].task_id
//-------------------------------------------------------------------------------------------------
#define TSKMA_TASK_NUM_IDL                    8 /* idle task */
#define TSKMA_TASK_ID_IDLE                    tskma_com_data.task_info[TSKMA_TASK_NUM_IDL].task_id
//-------------------------------------------------------------------------------------------------
#define TSKMA_TASK_NUM                        9
//=================================================================================================


typedef struct TSKMA_COM_TASKINFO_S
{
    LSA_UINT32  task_id;            	/* must be a unique id given by os (or os adaption) at task creation */
    LSA_UINT32  task_prio;          	/* priority of the task  */
    LSA_VOID   (*task_ptr)(LSA_VOID);   /* pointer to the task function */
    LSA_INT8*   task_name_ptr;      	/* task name (optional, max. 20 bytes */

} TSKMA_COM_TASKINFO_TYPE;

typedef struct TSKMA_COM_DATA_S
{
    TSKMA_COM_TASKINFO_TYPE task_info[TSKMA_TASK_NUM];
} TSKMA_COM_DATA_TYPE;

typedef TSKMA_COM_DATA_TYPE * TSKMA_COM_DATA_TYPE_PTR;
TSKMA_COM_EXT_ATTR TSKMA_COM_DATA_TYPE  tskma_com_data;


typedef struct TSKMA_SEM_RES_S
{
        /* control IDs for semaphores */
        LSA_UINT32         id_pnio_wait_rema_prm_done;  /* sema for writing oha remanence requests (export sema)*/

        LSA_UINT32         id_edd_lock_com;             /* sema to lock edd com (export sema )*/

        LSA_UINT32         id_fs_wait_write_file_done;  /* sema for fs write and delete operations */
        LSA_UINT32         id_fbs_wait_write_file_done; /* sema for fbs write and delete operations */

        LSA_UINT32         id_pnio_wait_rema_station_prm_done; /* sema for writing station prm data*/

        LSA_UINT32         id_pnio_wait_oh_crc16_done; /* sema for writing station crc data*/
        LSA_UINT32         id_pnio_wait_oh_control_done; /* sema for writing station oh control data*/

        LSA_UINT32         id_pnio_wait_nrt_rcv_resources; /* sema for accessing nrt_rcv_resources */

} TSKMA_SEM_RES_TYPE;


TSKMA_COM_EXT_ATTR TSKMA_SEM_RES_TYPE 	tskma_sem_res;

#define TSKMA_SEM_RES_EDD_COM_ACCESS 	tskma_sem_res.id_edd_lock_com

typedef struct TSKMA_LOCK_CHECK_S
{
    LSA_UINT32           gsy;
    LSA_UINT32           pndv;
    LSA_UINT32           application;
    LSA_UINT32           pnio;
    LSA_UINT32           tcpip;
    LSA_UINT32           pof;
    LSA_UINT32           fs_fbs;
    LSA_UINT32           lldp;
} TSKMA_LOCK_CHECK_TYPE;

TSKMA_COM_EXT_ATTR TSKMA_LOCK_CHECK_TYPE 	tskma_lock_check;

#define TSKMA_LOCK_CHECK_GSY tskma_lock_check.gsy
#define TSKMA_LOCK_CHECK_PND tskma_lock_check.pndv
#define TSKMA_LOCK_CHECK_APP tskma_lock_check.application
#define TSKMA_LOCK_CHECK_PNO tskma_lock_check.pnio
#define TSKMA_LOCK_CHECK_TCP tskma_lock_check.tcpip
#define TSKMA_LOCK_CHECK_POF tskma_lock_check.pof
#define TSKMA_LOCK_CHECK_FSB tskma_lock_check.fs_fbs
#define TSKMA_LOCK_CHECK_LLD tskma_lock_check.lldp

#define TSKMA_LOCK_SET_GSY(_VAL)  tskma_lock_check.gsy = (_VAL)
#define TSKMA_LOCK_SET_PND(_VAL)  tskma_lock_check.pndv = (_VAL)
#define TSKMA_LOCK_SET_APP(_VAL)  tskma_lock_check.application = (_VAL)
#define TSKMA_LOCK_SET_PNO(_VAL)  tskma_lock_check.pnio = (_VAL)
#define TSKMA_LOCK_SET_TCP(_VAL)  tskma_lock_check.tcpip = (_VAL)
#define TSKMA_LOCK_SET_POF(_VAL)  tskma_lock_check.pof = (_VAL)
#define TSKMA_LOCK_SET_FSB(_VAL)  tskma_lock_check.fs_fbs = (_VAL)
#define TSKMA_LOCK_SET_LLD(_VAL)  tskma_lock_check.lldp = (_VAL)

#define TSKMA_RET_ERROR     (LSA_UINT32)0x00000000
#define TSKMA_RET_OK        (LSA_UINT32)0x00000001


LSA_VOID 	tskma_isr_00 (LSA_UINT32 int_source);
LSA_VOID 	tskma_isr_01 (LSA_UINT32 int_source);

LSA_VOID 	tskma_parity_error_isr(LSA_UINT32 int_source);
LSA_VOID 	tskma_access_error_isr(LSA_UINT32 int_source);

LSA_VOID 	tskma_init	(LSA_VOID);
LSA_UINT32 	tskma_open	(LSA_VOID);
LSA_VOID 	tskma_start	(LSA_VOID);

LSA_VOID 	tskma_task_app_send_pndv_trigger(LSA_VOID);
LSA_VOID 	tskma_task_app_send_pnpb_trigger(LSA_VOID);
LSA_VOID 	tskma_task_app_pnpb_open_done   (LSA_VOID);

LSA_VOID 	tskma_task_stp_perform_cold_start_sm (LSA_VOID);

LSA_BOOL 	tskma_task_pno_queue_nrt_resource (LSA_VOID* lower_rqb_ptr, LSA_UINT16 modul_id, LSA_VOID* method_ptr);
LSA_VOID 	tskma_task_pno_serve_nrt_resource_list (LSA_VOID *rqb_ptr_);


#endif /* TSKMA_COM_H_ */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
