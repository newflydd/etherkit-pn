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
/*  F i l e               &F: pndv_plau.h                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  check if configuration is plausible and                                  */
/*  set working definitions for further internal configuration               */
/*                                                                           */
/*****************************************************************************/


/*****************************************************************************/
/* contents:

    - COMMON

        - basic tool chain characteristics
        - basic code and data memory attributes

        - component detail configuration

    - SYSTEM

        - system interface macros

    - PNDV

        - code attributes
        - data attributes

*/
/*****************************************************************************/
/* reinclude protection */


#ifndef PNDV_PLAU_H
#define PNDV_PLAU_H


/*****************************************************************************/
/*****************************************************************************/
/*******************************                 *****************************/
/*******************************     COMMON      *****************************/
/*******************************                 *****************************/
/*****************************************************************************/
/*****************************************************************************/


/*****************************************************************************/
/* basic tool chain characteristics */


#ifndef PNDV_FUNC_PTR_NAME
    #define PNDV_FUNC_PTR_NAME(_NAME)   (* _NAME)
#endif
#ifndef PNDV_HOST_PTR_COMPARE_TYPE
    #define PNDV_HOST_PTR_COMPARE_TYPE                void *
    #error PNDV configuration: PNDV_HOST_PTR_COMPARE_TYPE not defined !
#endif


/*****************************************************************************/
/* component detail configuration */

#ifdef PNDV_CFG_USE_DFC
#ifndef PNDV_DFC_DS_REQ
    #define PNDV_DFC_DS_REQ( _RQB_PTR, _REQ_ID, _CON_ID ) dfc_read_write_ds_req( _RQB_PTR, _REQ_ID, _CON_ID  )
#endif

#ifndef PNDV_DFC_DS_DONE
    #define PNDV_DFC_DS_DONE( _DS_PTR, _RESP_ID )         dfc_read_write_ds_done(_DS_PTR, _RESP_ID)
#endif

#ifndef PNDV_DFC_RQB_DONE
    #define PNDV_DFC_RQB_DONE( _RQB_PTR, _ERROR_CODE )                                \
    {                                                                                 \
        pndv_set_rqb_err( _RQB_PTR, _ERROR_CODE );                                    \
        dfc_read_write_ds_done( (DFC_REQ_PTR) _RQB_PTR,  PNDV_DFC_RES_ID_PNDV );      \
    }
#endif

#ifndef PNDV_DFC_RES_ID_PERI
    #error PNDV configuration: PNDV_DFC_RES_ID_PERI not defined ! !
#endif
#ifndef PNDV_DFC_RES_ID_PNDV
    #error PNDV configuration: PNDV_DFC_RES_ID_PNDV not defined ! !
#endif
#ifndef PNDV_DFC_REQ_ID_PNDV_DS
    #error PNDV configuration: PNDV_DFC_REQ_ID_PNDV_DS not defined ! !
#endif
#ifndef PNDV_DFC_REQ_ID_PNDV_RQB
    #error PNDV configuration: PNDV_DFC_REQ_ID_PNDV_RQB not defined ! !
#endif
#ifndef PNDV_DFC_REQ_ID_CM
    #error PNDV configuration: PNDV_DFC_REQ_ID_CM not defined ! !
#endif

#else /* PNDV_CFG_USE_DFC */
    #define PNDV_DFC_REQ_ID_CM      0x815
    #define PNDV_DFC_RES_ID_PERI    0x815
    #ifdef PNDV_DFC_DS_REQ
        #error PNDV configuration: PNDV_DFC_DS_REQ is defined but DFC is not used!
    #else
    #define PNDV_DFC_DS_REQ( _RQB_PTR, _REQ_ID, _CON_ID )\
    {\
        if (_REQ_ID != PNDV_DFC_REQ_ID_CM)\
        {\
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);\
        }\
        else\
        {\
            pndv_peri_read_write_record( (void *)_RQB_PTR);\
        }\
    }
    #endif
    #ifdef PNDV_DFC_DS_DONE
        #error PNDV configuration: PNDV_DFC_DS_DONE is defined but DFC is not used!
    #else
    #define PNDV_DFC_DS_DONE( _DS_PTR, _RESP_ID )\
    {\
        if (_RESP_ID !=  PNDV_DFC_RES_ID_PERI)\
        {\
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);\
        }\
        else\
        {\
            pndv_dfc_cbf_cm( (CM_UPPER_RQB_PTR_TYPE) _DS_PTR, 0);\
        }\
    }
    #endif
    #define PNDV_CFG_PERI_QUEUE_DS_REQ
#endif /* PNDV_CFG_USE_DFC */

#ifdef PNDV_CFG_DEBUG_ENABLE

    #ifdef PNDV_CFG_DEBUG_ELEMENT_NUMBER

        #if (((PNDV_CFG_DEBUG_ELEMENT_NUMBER) < 1) || ((PNDV_CFG_DEBUG_ELEMENT_NUMBER) > 65535))

            #error PNDV configuration: number of debug elements PNDV_CFG_DEBUG_ELEMENT_NUMBER invalid (1<=x<=65535) !

        #endif

    #else

        #error PNDV configuration: number of debug elements PNDV_CFG_DEBUG_ELEMENT_NUMBER not defined ! !

    #endif

#endif /*#ifdef PNDV_CFG_DEBUG_ENABLE*/

#ifndef PNDV_MAX_SUBS_PER_VMOD
    #define PNDV_MAX_SUBS_PER_VMOD 1
#endif


#ifdef PNDV_IM_HAS_GLOB_PARA
 #if PNDV_IM_HAS_GLOB_PARA == 0 || PNDV_IM_HAS_GLOB_PARA ==1
 #else
  #error PNDV_IM_HAS_GLOB_PARA must be ether 0 or 1
 #endif
#else
 #error PNDV_IM_HAS_GLOB_PARA not defined
#endif

#ifndef PNDV_CFG_MAX_PORT_CNT
 #define PNDV_CFG_MAX_PORT_CNT 4
#endif

#ifdef PNDV_CFG_USE_DIAG2
 #ifdef PNDV_CFG_USE_MULTIDIAG
  #error PNDV_CFG_DIAG2 and PNDV_CFG_MULTIDIAG can not be used at the same time!
 #endif
#endif

#ifndef PNDV_CFG_IS_ASSET_MANAGEMENT_SUPPORTED
  #define PNDV_CFG_IS_ASSET_MANAGEMENT_SUPPORTED  FALSE
#endif
/*****************************************************************************/
/*****************************************************************************/
/*******************************                ******************************/
/*******************************     SYSTEM     ******************************/
/*******************************                ******************************/
/*****************************************************************************/
/*****************************************************************************/


/*****************************************************************************/
/* system interface macros */


#ifndef PNDV_ENTER
    #define PNDV_ENTER(_SYS_PTR)
#endif
#ifndef PNDV_EXIT
    #define PNDV_EXIT(_SYS_PTR)
#endif


#ifndef PNDV_FATAL_ERROR
    #define PNDV_FATAL_ERROR(_ERROR_DETAIL_PTR)
#endif


#ifndef PNDV_WRITE_COUPLE_EVENT
    #define PNDV_WRITE_COUPLE_EVENT(_EVENT)
#endif


#ifndef PNDV_COPY_BYTE
#define PNDV_COPY_BYTE(_DEST_PTR,_SRC_PTR,_LEN)                               \
{                                                                             \
    register Unsigned8 * tmp_dest_ptr;                                        \
    register Unsigned8 * tmp_src_ptr;                                         \
    register Signed32    tmp_count;                                           \
                                                                              \
    tmp_dest_ptr = (Unsigned8 *) (_DEST_PTR);                                 \
    tmp_src_ptr  = (Unsigned8 *) (_SRC_PTR);                                  \
    tmp_count    = (Signed32)    (_LEN);                                      \
                                                                              \
    for (   /* no initial settings */                                         \
         ;  tmp_count--                                                       \
         ; *tmp_dest_ptr = *tmp_src_ptr, tmp_dest_ptr++, tmp_src_ptr++        \
        )                                                                     \
    {                                                                         \
        /* nothing to do */                                                   \
    }                                                                         \
}
#endif

#ifndef PNDV_COMPARE_BYTE
#define PNDV_COMPARE_BYTE(_RETURN_VALUE,_PTR_1,_PTR_2,_LEN)                   \
{                                                                             \
    register Unsigned8 * tmp_ptr_1;                                           \
    register Unsigned8 * tmp_ptr_2;                                           \
    register Signed32    tmp_count;                                           \
                                                                              \
    tmp_ptr_1       = (Unsigned8 *) (_PTR_1);                                 \
    tmp_ptr_2       = (Unsigned8 *) (_PTR_2);                                 \
    tmp_count       = (Signed32)    (_LEN);                                   \
    (_RETURN_VALUE) =                FALSE;                                   \
                                                                              \
    for (  /* no initial settings */                                          \
         ; tmp_count--                                                        \
         ; tmp_ptr_1++, tmp_ptr_2++                                           \
        )                                                                     \
    {                                                                         \
        if ( *tmp_ptr_1 != *tmp_ptr_2 )                                       \
        {                                                                     \
            (_RETURN_VALUE) = TRUE;                                           \
                                                                              \
            break;                                                            \
        }                                                                     \
    }                                                                         \
}
#endif

#ifndef PNDV_GET_ENTITY_NR
    /* using internal entity admin */
    #define PNDV_GET_ENTITY_NR(_API, _SLOT, _SUBSLOT, _AR_IDX) pndv_peri_get_entity_index(_SLOT, _SUBSLOT, _AR_IDX)
#endif

#ifndef PNDV_MAX_PD_SLOTS
    #define PNDV_MAX_PD_SLOTS 3
#endif

#ifndef PNDV_HANDLE_ARFSU_DATA_ADJUST
    #define PNDV_HANDLE_ARFSU_DATA_ADJUST LSA_TRUE
#endif

#ifndef PNDV_CM_SV_MAX_SUBSLOT_NR
    #define PNDV_CM_SV_MAX_SUBSLOT_NR 0x7fff
#endif


#ifndef PNDV_CM_SV_SUBSLOT_COUNT
    #error PNDV_CM_SV_SUBSLOT_COUNT, maximum count of used subslots in an ar not defined!
#endif

#ifndef PNDV_ALARM_SEND_MAX_LENGTH
    #define PNDV_ALARM_SEND_MAX_LENGTH 0
#endif

#ifndef PNDV_MAX_GENERIC_DIAG_NUMBER
    #error That value must be at least 1 !
#else

    #if PNDV_MAX_GENERIC_DIAG_NUMBER == 0
        #error That value must be at least 1 !
    #else
        #ifndef PNDV_MAX_GENERIC_DIAG_DATA_LENGTH
            #define PNDV_MAX_GENERIC_DIAG_DATA_LENGTH 12
        #endif
    #endif

#endif

#ifndef PNDV_PD_MAX_PRM_REC_COUNT
    #define PNDV_PD_MAX_PRM_REC_COUNT 4 /* OHA_RECORD_INDEX_MULTIPLE_PDEV, OHA_RECORD_INDEX_MULTIPLE_SNMP, OHA_RECORD_INDEX_IP_SUITE, OHA_RECORD_INDEX_NAME_OF_STATION */
#endif

#ifndef PNDV_GET_PRM_LIST
 #define PNDV_GET_PRM_LIST pndv_in_cm_pd_build_prm_list
#endif


#ifdef PNDV_CFG_ENABLE_R1

    #if PNDV_MAX_ARS_RT <= 1
        #error PNDV_MAX_ARS_RT to low but R1 enabled!
    #endif

    #ifndef PNDV_CFG_ENABLE_S2
        #error "pndv: settup not supported, define S2 if R1 is defined"
    #endif

    #if (PNDV_MAX_ARS_RT_R1 > PNDV_MAX_ARS_RT)
        #error "pndv: settup not supported, RT ARs for R1 must be less or equal to max RT-ARs"
    #endif

#endif /* PNDV_CFG_ENABLE_R1 */

#ifdef PNDV_CFG_ENABLE_CIR
    #ifndef PNDV_CFG_ENABLE_S2
        #error "pndv: settup not supported, cir possible with S2 only"
    #endif
#endif



#ifndef PNDV_MAX_IOCR_PER_AR
    #define PNDV_MAX_IOCR_PER_AR 2
#endif


#ifndef PNDV_CFG_PORT_IS_MODULAR
 #define PNDV_CFG_PORT_IS_MODULAR 0
#endif
/*****************************************************************************/
/* reinclude-protection */


#else
    #pragma message ("The header PNDV_PLAU.H is included twice or more !")
#endif


/*****************************************************************************/
/*  end of file.                                                             */
/*****************************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
