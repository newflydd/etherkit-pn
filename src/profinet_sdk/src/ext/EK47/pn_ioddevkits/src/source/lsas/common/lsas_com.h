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
/*  F i l e               &F: lsas_com.h                                :F&  */
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

/**
 * @file    lsas_com.h
 * @brief   brief description
 * @author  cn3dit09
 * @version
 * @date    24.02.2012
 *
 * long description
 *
 */

/*****************************************************************************/

#ifndef LSAS_COM_H_
#define LSAS_COM_H_

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

#define LSAS_FATAL_ERROR_TYPE  LSA_UINT32

#define LSAS_FATAL_ERR_NULL_PTR            (LSAS_FATAL_ERROR_TYPE)  0x01
#define LSAS_FATAL_ERR_MEM_FREE            (LSAS_FATAL_ERROR_TYPE)  0x02
#define LSAS_FATAL_ERR_GET_PATH_INFO       (LSAS_FATAL_ERROR_TYPE)  0x03
#define LSAS_FATAL_ERR_RELEASE_PATH_INFO   (LSAS_FATAL_ERROR_TYPE)  0x04
#define LSAS_FATAL_ERR_INCONSISTENZ        (LSAS_FATAL_ERROR_TYPE)  0x05
#define LSAS_FATAL_ERR_TIMER_RESOURCE      (LSAS_FATAL_ERROR_TYPE)  0x06
#define LSAS_FATAL_ERR_HANDLE_INVALID      (LSAS_FATAL_ERROR_TYPE)  0x07
#define LSAS_FATAL_ERR_EDD                 (LSAS_FATAL_ERROR_TYPE)  0x08
#define LSAS_FATAL_ERR_LOWER_DONE          (LSAS_FATAL_ERROR_TYPE)  0x09

/***********************************************
 * OPCODES
 ***********************************************/
#define LSAS_OPC_NONE                 0
#define LSAS_OPC_INIT_PNIO_STACK      1
#define LSAS_OPC_SETUP_PNIO_STACK     2
#define LSAS_OPC_OPEN_PNIO_STACK      3
#define LSAS_OPC_INIT_OHA_DB          4
#define LSAS_OPC_FS_OPEN              5

#define LSAS_INVALID_HANDLE           0
#include "sys_pck1.h"
typedef union LSAS_RQB_ARGS_S
{
    struct
    {
        LSA_UINT16 interface_id;
        LSA_UINT8  ip_suite_present;
        LSA_UINT8  ip[4];
        LSA_UINT8  subnet[4];
        LSA_UINT8  gateway[4];
        LSA_UINT8  name_of_station[255+1];
        LSA_UINT8  station_type[25+1];      // IEC 61158–6–10 / 4.10.2.2.1 DeviceType
        LSA_UINT8  order_id[20+1];          // MLFB, e.g. "6ES7 155-6AU00-0CN0 "
        LSA_UINT8  serial_number[16+1];
        LSA_UINT32 hw_revision;             // hw revision 0..99999
        LSA_UINT8  sw_revision[10+1];       // sw revision, e.g. "V  2  1  0"
        LSA_UINT16 vendor_id;
        LSA_UINT16 device_id;
        LSA_UINT16 instance;
        LSA_UINT8  sys_description[256];    // SNMP system description

    }oha_db_init;

}LSAS_RQB_ARGS_TYPE;
#include "sys_unpck.h"
typedef struct LSAS_RQB_S * LSAS_RQB_PTR_TYPE;

typedef LSAS_RQB_ARGS_TYPE *  LSAS_RQB_ARGS_PTR_TYPE;

typedef struct LSAS_RQB_S
{
    LSAS_RQB_HEADER

    LSAS_RQB_ARGS_PTR_TYPE  args;

    LSAS_RQB_TRAILER
}LSAS_RQB_TYPE;

typedef enum LSAS_EDDP_SM_STATE_E
{
    LSAS_EDDP_SM_NOT_USED               = 0,
    LSAS_EDDP_SM_CLOSED                 = 10,
    LSAS_EDDP_SM_WAIT_OPEN_DONE         = 20,
    LSAS_EDDP_SM_WAIT_SETUP_DONE        = 30,
    LSAS_EDDP_SM_DONE                   = 40
}LSAS_EDDP_SM_STATE_E_T;


typedef enum LSAS_EDDS_SM_STATE_E
{
    LSAS_EDDS_SM_NOT_USED               ,
    LSAS_EDDS_SM_CLOSED                 ,
    LSAS_EDDS_SM_ALLOC_MEM              ,
    LSAS_EDDS_SM_DEVICE_OPEN            ,
    LSAS_EDDS_SM_WAIT_OPEN_DONE         ,
    LSAS_EDDS_SM_INIT_NRT               ,
    LSAS_EDDS_SM_WAIT_INIT_NRT          ,
    LSAS_EDDS_SM_INIT_SRT               ,
    LSAS_EDDS_SM_WAIT_INIT_SRT          ,
    LSAS_EDDS_SM_DEVICE_SETUP           ,
    LSAS_EDDS_SM_WAIT_SETUP_DONE        ,
    LSAS_EDDS_SM_FREE_MEM               ,
    LSAS_EDDS_SM_DONE                   ,
} LSAS_EDDS_SM_STATE_E_T;


/****************************************************************************/

void    lsas_init                 (void);
void    lsas_com_init_pnio_stack  (void);
void    lsas_com_setup_pnio_stack (LSAS_RQB_PTR_TYPE);
void    lsas_com_open_pnio_stack  (void);
void    lsas_com_shutdown_network (void);
void    lsas_com_init_lsa_ldbs    (void);
void    lsas_request              (LSAS_RQB_PTR_TYPE);
#ifdef LSAS_CFG_USE_FS
void    lsas_com_fs_open          (void);
LSA_BOOL lsas_com_is_fs_device_open(void);
#endif
#ifdef LSAS_CFG_USE_STCP
extern  void    lsas_stcp_to_s7pn_cbf                           (L4_UPPER_RQB_PTR_TYPE rqb_ptr);
#endif

#ifdef LSAS_CFG_USE_EDDP
LSA_VOID lsas_eddp_system_ram_init(LSA_VOID);
LSA_VOID lsas_eddp_system_init_cmd_iface(LSA_VOID);

#endif

LSA_UINT16          lsas_get_path_info  (LSAS_SYSTEM_PTR_PTR system_ptr_ptr, LSAS_DETAIL_PTR *detail_ptr_ptr, LSAS_PATH_TYPE path, LSAS_LAYER_TYPE layer);
LSA_UINT16          lsas_release_path_info  (LSAS_SYSTEM_PTR system_ptr, LSAS_DETAIL_PTR detail_ptr);


/****************************************************************************/
// call back functions
/****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif


#endif /* LSAS_COM_H_ */

/*** end of file *************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
