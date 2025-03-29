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
/*  F i l e               &F: tskma_cfg.h                               :F&  */
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


#ifndef TSKMA_CFG_H_
#define TSKMA_CFG_H_


/*
 * basic configuration
 *
 */

#define TSKMA_IRTE_BASE 0x10000000


/*===========================================================================*/
/* LSA-HEADER and LSA-TRAILER                                                */
/*===========================================================================*/

#define TSKMA_RQB_HEADER      LSA_RQB_HEADER(TSKMA_MAIL_ELEM_S_PTR_T)

#define TSKMA_RQB_TRAILER     LSA_RQB_TRAILER

#define TSKMA_RQB_SET_NEXT_RQB_PTR(rb,v)      LSA_RQB_SET_NEXT_RQB_PTR(rb,v)
#define TSKMA_RQB_SET_PREV_RQB_PTR(rb,v)      LSA_RQB_SET_PREV_RQB_PTR(rb,v)
#define TSKMA_RQB_SET_OPCODE(rb,v)            LSA_RQB_SET_OPCODE(rb,v)
#define TSKMA_RQB_SET_HANDLE(rb,v)            LSA_RQB_SET_HANDLE(rb,v)
#define TSKMA_RQB_SET_RESPONSE(rb,v)          LSA_RQB_SET_RESPONSE(rb,v)
#define TSKMA_RQB_SET_USER_ID_PTR(rb,v)       LSA_RQB_SET_USER_ID_PTR(rb,v)
#define TSKMA_RQB_SET_METHOD_PTR(rb, v)       LSA_RQB_SET_METHOD_PTR(rb,v)

#define TSKMA_RQB_GET_NEXT_RQB_PTR(rb)        LSA_RQB_GET_NEXT_RQB_PTR(rb)
#define TSKMA_RQB_GET_PREV_RQB_PTR(rb)        LSA_RQB_GET_PREV_RQB_PTR(rb)
#define TSKMA_RQB_GET_OPCODE(rb)              LSA_RQB_GET_OPCODE(rb)
#define TSKMA_RQB_GET_HANDLE(rb)              LSA_RQB_GET_HANDLE(rb)
#define TSKMA_RQB_GET_RESPONSE(rb)            LSA_RQB_GET_RESPONSE(rb)
#define TSKMA_RQB_GET_USER_ID_PTR(rb)         LSA_RQB_GET_USER_ID_PTR(rb)



#if (PNIOD_PLATFORM & PNIOD_PLATFORM_DKSW)

	#define TSKMA_EDD_INT_REQUEST(_RQB_PTR_) 	edds_system(_RQB_PTR_);

#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200_400)

	#define TSKMA_EDD_INT_REQUEST(_RQB_PTR_) 	eddi_request(_RQB_PTR_);

#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P)

	#define TSKMA_EDD_INT_REQUEST(_RQB_PTR_) 	eddp_request(_RQB_PTR_);
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_RZT2_EDDP)

	#define TSKMA_EDD_INT_REQUEST(_RQB_PTR_) 	eddp_request(_RQB_PTR_);

#else

	#define TSKMA_EDD_SYSTEM(_RQB_PTR_)

#endif


#define TSKMA_FATAL_ERROR(length,error_ptr)		\
{                                               \
    LSAS_FATAL_ERROR( error_ptr);               \
}
/**
 *  @brief static os or not
 *
 *  If defined tskma is used with an os that only supports static tasks
 *  (defined and fixed with compile time). If not defined tasks are created
 *  dynamicly at runtime
 *
 */
//#define TSKMA_USE_STATIC_OS

#define TSKMA_ALLOC_MEM(_MEM_PTR_PTR, _LENGTH) OsAllocFX((void**)_MEM_PTR_PTR, _LENGTH, MEMPOOL_CACHED)

#define TSKMA_FREE_MEM(_MEM_PTR)  OsFreeX(_MEM_PTR, MEMPOOL_CACHED);

#define TSKMA_MEMSET(_DST_PTR, _VAL, _SIZE) OsMemSet(_DST_PTR, _VAL, _SIZE)

#define TSKMA_MEMCPY(_DST_PTR, _SRC_PTR, _SIZE) OsMemCpy(_DST_PTR, _SRC_PTR, _SIZE)

#define TSKMA_MEMCMP(_DST_PTR, _SRC_PTR, _SIZE) OsMemCmp(_DST_PTR, _SRC_PTR, _SIZE)

/*
 * OS interfacing macros
 */

/*
 * OS interfacing macros - binary semaphores (mutex)
 *
 * _RET_VAL is expected to be LSA_OK if allocation was successfull. LSA_NULL in
 * all other cases.
 */

#define TSKMA_OS_SEM_B_CREATE(_SEM_ID_PTR, _RET_VAL)\
{\
    PNIO_UINT32 __ret_val__;\
    __ret_val__ = OsAllocSemB((PNIO_UINT32*)_SEM_ID_PTR);\
    if (__ret_val__ == PNIO_OK)\
    {\
        _RET_VAL = LSA_OK;\
    }\
    else\
    {\
        _RET_VAL = LSA_NULL;\
    }\
}

#define TSKMA_OS_SEM_B_FREE(_SEM_ID, _RET_VAL)\
{\
    PNIO_UINT32 __ret_val__;\
    __ret_val__ = OsFreeSemB(_SEM_ID);\
    if (__ret_val__ == PNIO_OK)\
    {\
        _RET_VAL = LSA_OK;\
    }\
    else\
    {\
        _RET_VAL = LSA_NULL;\
    }\
}

#define TSKMA_OS_SEM_B_TAKE(_SEM_ID, _RET_VAL)\
{\
    PNIO_UINT32 __ret_val__;\
    __ret_val__ = OsTakeSemB(_SEM_ID);\
    if (__ret_val__ == PNIO_OK)\
    {\
        _RET_VAL = LSA_OK;\
    }\
    else\
    {\
        _RET_VAL = LSA_NULL;\
    }\
}

#define TSKMA_OS_SEM_B_GIVE(_SEM_ID, _RET_VAL)\
{\
    PNIO_UINT32 __ret_val__;\
    __ret_val__ = OsGiveSemB(_SEM_ID);\
    if (__ret_val__ == PNIO_OK)\
    {\
        _RET_VAL = LSA_OK;\
    }\
    else\
    {\
        _RET_VAL = LSA_NULL;\
    }\
}

/*
 * OS interfacing macros - thread related
 */

/**
 *  @brief create task
 *
 *  @param[in]  _FCT_PTR pointer to the thread function
 *  @param[in]  _PRIO priority of the thread
 *  @param[out] _ID_PTR pointer where the thread id is returned to
 *  @param[out] _RET_VAL_PTR pointer where the return value is stored
 *
 *  This macro is used to create a task on a dynamic os. It is expected
 *  that the task is running by exiting this macro.
 *  Is TSKMA is used with an static os this macro may remain empty.
 *
 */
#define TSKMA_OS_TASK_CREATE(_FCT_PTR, _PRIO, _ID_PTR, _TSK_NAME, _RET_VAL_PTR)\
{\
    LSA_UINT32 l_ret_val;\
    l_ret_val = OsCreateThread (_FCT_PTR,                     /* [in ]pointer to task handler */ \
                                (PNIO_UINT8*)_TSK_NAME,       /* Taskname for Debugging */       \
                                _PRIO,                        /* [in ]task priority */           \
                                _ID_PTR);                     /* [out] pointer to Task ID */     \
    *_RET_VAL_PTR = l_ret_val; \
}


#define TSKMA_OS_TASK_START(_ID_PTR, _RET_VAL_PTR)\
{\
    *_RET_VAL_PTR  = OsStartThread(_ID_PTR);         /* Start the thread */ \
}


/**
 *  @brief brief_description
 *
 *  @param[in]  _TASK_ID id of the task that was returned on thread creation
 *  @param[in]  _TASK_NUM internal number of the task (TSKMA_TASK_NUM_xxx)
 *
 *  This macro is called once at the start of a task to create a message box.
 *  If the used os implicitly creates a message box on task creation this macro
 *  can remain empty.
 *
 */
#define TSKMA_OS_MBOX_CREATE(_TASK_ID, _TASK_NUM)\
{\
    OsCreateMsgQueue(_TASK_ID);\
}

#define TSKMA_OS_WAIT_ON_TASKSTART()\
{\
    OsWaitOnEnable() ;\
}

#define TSKMA_OS_READ_MAIL_BLOCKED(_RQB_PTR_PTR, _TASK_NUM)\
{\
    OsReadMessageBlocked(_RQB_PTR_PTR, tskma_com_data.task_info[_TASK_NUM].task_id);\
}

#define TSKMA_OS_READ_MAIL(_RQB_PTR_PTR, _TASK_NUM)\
{\
    /*OsReadMessage(_RQB_PTR_PTR, tskma_com_data.task_info[_TASK_NUM].task_id);*/\
    TSKMA_OS_READ_MAIL_BLOCKED(_RQB_PTR_PTR, _TASK_NUM);\
}

#define TSKMA_OS_SEND_MAIL(_TASK_NUM, _RQB_PTR)\
{\
    OsSendMessage (tskma_com_data.task_info[_TASK_NUM].task_id, _RQB_PTR, OS_MBX_PRIO_NORM);\
}

#define TSKMA_IM_MODULE_ID()            PnpbGetModId(0)
#define TSKMA_IM_SUBMODULE_ID_IF()      PnpbGetSubmodId(0)
#define TSKMA_IM_SUBMODULE_ID_P(idx)    PnpbGetSubmodId((idx))
#define TSKMA_IM_SUBMODULE_NUM_P()      PnpbGetSubmodCnt()
#define TSKMA_IM_DEVICE_ID()            PnpbGetDeviceId();
#define TSKMA_IM_INTERFACE_ID()         (LSA_UINT16)0x0001

#define TSKMA_IM_SUBMODULE_NR_INTERFACE()   (LSA_UINT16)0x8000


#define TSKMA_IM_SUBMODULE_NR_PORTS_USED IOD_CFG_PDEV_NUMOF_PORTS


#define TSKMA_IM_VENDOR_ID()        PnpbGetVendorId();
#define TSKMA_IM_GET_IP_SUITE(_RET_PTR, _IP_PTR, _SBN_PTR, _GTW_PTR)\
{\
     PnpbNvGetIpSuite((PNIO_UINT32*)_IP_PTR, (PNIO_UINT32*)_SBN_PTR, (PNIO_UINT32*)_GTW_PTR); \
    *(_RET_PTR) = LSA_TRUE; \
}
#define TSKMA_IM_GET_NAME_OF_STATION(_NOS_PTR, _BUF_SIZE)\
{\
        PnpbNvGetStationName ((PNIO_UINT8*)_NOS_PTR, (LSA_UINT32)_BUF_SIZE); \
}
#define TSKMA_IM_GET_STATION_TYPE(_STATION_TYPE_PTR, _BUF_LEN)\
{\
        PnpbNvGetStationType ((PNIO_UINT8*)_STATION_TYPE_PTR, (LSA_UINT32)_BUF_LEN); \
}
#define TSKMA_IM_GET_ORDER_ID(_ORDER_ID_PTR, _BUF_LEN)  \
{                                                       \
	PnpbNvGetOrderId ((PNIO_UINT8*)_ORDER_ID_PTR, (LSA_UINT32)_BUF_LEN); \
}
#define TSKMA_IM_GET_SERIAL_NUMBER(_SERIAL_NUMBER_PTR, _BUF_LEN)            \
{                                                                           \
	PnpbNvGetSerialNumber ((PNIO_UINT8*)_SERIAL_NUMBER_PTR, (LSA_UINT32)_BUF_LEN); \
}
#define TSKMA_IM_GET_HW_REVISION		PnpbNvGetHwRevision()

#define TSKMA_IM_GET_SW_REVISION(_SW_REVISION_PTR, _BUF_LEN)                    \
{                                                                               \
	PnpbNvGetSwRevision ((PNIO_UINT8*)_SW_REVISION_PTR, (LSA_UINT32)_BUF_LEN); 	\
}
#define TSKMA_IM_GET_SNMP_SYS_DESCR(_SNMP_SYS_DESCR_PTR, _BUF_LEN)              \
{                                                                               \
    TSKMA_ASSERT( PnpbNvGetpSnmpData()->SysDescrLen < _BUF_LEN);       			\
	TSKMA_MEMCPY((_SNMP_SYS_DESCR_PTR), PnpbNvGetpSnmpData()->SysDescr, PnpbNvGetpSnmpData()->SysDescrLen+1); \
}

#define TSKMA_INIT_TIMER_0()\
{\
    /* init a hardware timer here */\
}

#define TSKMA_SERVE_WD()\
{\
    /* trigger WD Timer if used */\
}

#define TSKMA_UPDATE_CONSUMER_DATA()\
{\
    /* update io consumer buffer now */\
    iom_update_consumer_data();\
}

#define TSKMA_UPDATE_PROVIDER_DATA()\
{\
}

#define TSKMA_OS_TASK_WAKE_UP(_TASK_NUM)\
{\
  /* wake the given task */\
}


#define  TSKMA_IM_GET_ANNOTATION_STR_PTR() PnpbNvGetAnnotation()

#if 0
    #define TSKMA_TASK_INIT_STATE(_TASK_NUM)\
    {\
        while(!tskma_data.cold_start.done)\
        {\
            union\
            {\
                TSKMA_MAIL_ELEM_S_PTR_T     rqb_ptr;\
                TSKMA_VOID_PTR_TYPE         void_ptr;\
            }msg;\
            TSKMA_OS_READ_MAIL_BLOCKED(&msg.void_ptr, _TASK_NUM);\
            LSA_RQB_GET_METHOD_PTR(msg.rqb_ptr) (msg.void_ptr);\
        }\
    }
#else
    extern void  TSKMA_TASK_INIT_STATE (LSA_UINT32 TaskNum);
#endif



#define TSKMA_WAKE_NEXT_TS_TASK(_TASK_NUM)

#define TSKMA_IDLE_LOOP_CHECK() OsWait_ms(1000)

#define TSKMA_OPEN_DONE()

#define TSKMA_TASK_PRIO_APP                    TASK_PRIO_PNPB_OWN_IND   /* application task */
#define TSKMA_TASK_PRIO_EDD                    TASK_PRIO_EDD            /* EDD task */
#define TSKMA_TASK_PRIO_ORG                    TASK_PRIO_ORG            /* EDD-ORG task */
#define TSKMA_TASK_PRIO_PNO                    TASK_PRIO_PNO            /* pnio task  */
#define TSKMA_TASK_PRIO_TCP                    TASK_PRIO_IP_STACK       /* TCP/IP task */
#define TSKMA_TASK_PRIO_POF                    TASK_PRIO_POF            /* POF task */
#define TSKMA_TASK_PRIO_REM                    TASK_PRIO_REM            /* Remanence task */
#define TSKMA_TASK_PRIO_STP                    TASK_PRIO_STARTUP        /* startup task */
#define TSKMA_TASK_PRIO_IDL                    TASK_PRIO_IDLE           /* idle task */

#define TSKMA_TASK_NAME_APP                    "t_APP"
#define TSKMA_TASK_NAME_EDD                    "t_EDD"
#define TSKMA_TASK_NAME_ORG                    "t_ORG"
#define TSKMA_TASK_NAME_PNO                    "t_PNO"
#define TSKMA_TASK_NAME_TCP                    "t_TCP"
#define TSKMA_TASK_NAME_POF                    "t_POF"
#define TSKMA_TASK_NAME_REM                    "t_REM"
#define TSKMA_TASK_NAME_STP                    "t_STP"
#define TSKMA_TASK_NAME_IDL                    "t_IDL"


#include "tskma_plau.h"

#endif /* TSKMA_CFG_H_ */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
