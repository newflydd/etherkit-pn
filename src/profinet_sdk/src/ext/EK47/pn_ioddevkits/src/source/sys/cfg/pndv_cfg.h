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
/*  F i l e               &F: pndv_cfg.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  configure pndv.                                                          */
/*                                                                           */
/*****************************************************************************/



/*****************************************************************************/
/* contents:

    COMMON

      basic tool chain characteristics

      component detail configuration
      device configuration
      peripherie configuration

    SYSTEM

      system interface macros

    PNDV

      code attributes
      data attributes

*/
/*****************************************************************************/
/* reinclude protection */


#ifndef PNDV_CFG_H
#define PNDV_CFG_H


/*****************************************************************************/
/*****************************************************************************/
/*******************************                ******************************/
/*******************************     COMMON     ******************************/
/*******************************                ******************************/
/*****************************************************************************/
/*****************************************************************************/

#ifdef ENABLE_S2_REDUNDANCY
#define PNDV_CFG_ENABLE_S2
#define PNDV_S2_REDUNDANCY_EDGE_WHILE_DISCONNECTING_FIX
#endif

#ifdef ENABLE_DR
#define PNDV_CFG_ENABLE_CIR
#endif

#if IOD_INCLUDE_AMR
#define PNDV_CFG_IS_ASSET_MANAGEMENT_SUPPORTED  TRUE
#else
#define PNDV_CFG_IS_ASSET_MANAGEMENT_SUPPORTED  FALSE
#endif

#define PNDV_NO_SPECIAL_TREATMENT_FOR_RW_RECORD_INDEX_1

/*****************************************************************************/
/* basic tool chain characteristics */
/*****************************************************************************/

#define PNDV_HOST_PTR_COMPARE_TYPE             LSA_HOST_PTR_COMPARE_TYPE

#define PNDV_PROCESSOR_DEFAULT_ALIGN                     4
#define PNDV_PROCESSOR_PTR_SIZE                          4

#define PNDV_MAX_IO_LEN									1440
#define PNDV_MAX_OF_AR									IOD_CFG_NUMOF_IO_AR
#define PNDV_MAX_IO_INPUT_LEN                           PNDV_MAX_IO_LEN  /**< sum of gross data bytes over all input CRs. see GSDML/MaxInputLength */
#define PNDV_MAX_IO_OUTPUT_LEN                          PNDV_MAX_IO_LEN  /**< sum of gross data bytes over all output CRs. see GSDML/MaxOutputLength*/
#ifdef IM_SUPPORT_PN_S2_REDUNDANCY
    /** sum of gross data bytes over all CRs. In case of S2, CR sizes belonging to one ARset are added up in CM's plausibility check as well! */
    /*#define PNDV_MAX_IO_IN_OUT_LEN                      (4*PNDV_MAX_IO_LEN)	*/
#define PNDV_MAX_IO_IN_OUT_LEN							( (PNDV_MAX_IO_INPUT_LEN+PNDV_MAX_IO_OUTPUT_LEN) * PNDV_MAX_OF_AR )
#else
    /** sum of gross data bytes over all CRs. In non redundant case, it's the sum of input and output CR length */
    #define PNDV_MAX_IO_IN_OUT_LEN                      ( (PNDV_MAX_IO_INPUT_LEN+PNDV_MAX_IO_OUTPUT_LEN) * PNDV_MAX_OF_AR )
#endif
/*****************************************************************************/
/* component detail configuration */
/*****************************************************************************/
#define PNDV_AL_STAL_SOE_USI_MIN	0x00
#define PNDV_AL_STAL_SOE_USI_MAX	0xffff

#define PNDV_AL_STAL_RS_LOW_WATERMARK_USI   0x8300
#define PNDV_AL_STAL_RS_TIMEOUT_USI         0x8301
#define PNDV_AL_STAL_RS_OVERFLOW_USI        0x8302
#define PNDV_AL_STAL_RS_EVENT_USI           0x8303



// #define PNDV_CFG_DEBUG_ENABLE
#define PNDV_CFG_DEBUG_ELEMENT_NUMBER                0 // 4000

#if (PNIO_TRACE != PNIO_TRACE_NONE)
#define PNDV_TRACE_MSG(_subsys, _level, msg, para1, para2, para3) 												\
	if ((PNIO_TRACE_COMPILE_LEVEL >= _level) && (SubsysTrace[TRACE_SUBSYS_PNDV_LOWER].level >= _level)) 		\
    	DK_TRACE_03(LTRC_ACT_MODUL_ID , __LINE__, TRACE_SUBSYS_PNDV_LOWER, _level, msg, para1, para2, para3)

#ifdef PNDV_TRACE_MSG
	#define PNDV_TRACE_MSG_GET_STR(_level, _p_deb_str, _deb_code, _detail_1, _detail_2, _detail_3)  			\
		if ((PNIO_TRACE_COMPILE_LEVEL >= _level) && (SubsysTrace[TRACE_SUBSYS_PNDV_LOWER].level >= _level)) 	\
			PNDV_TRACE_MSG_GET_DEBUG_STR(_p_deb_str, _deb_code, _detail_1, _detail_2, _detail_3)
#endif //PNDV_TRACE_MSG
#else //(PNIO_TRACE != PNIO_TRACE_NONE)
#define PNDV_TRACE_MSG(_subsys, _level, msg, para1, para2, para3)
#define PNDV_TRACE_MSG_GET_STR(_level, _p_deb_str, _deb_code, _detail_1, _detail_2, _detail_3)
#endif //(PNIO_TRACE != PNIO_TRACE_NONE)


    //#define PNDV_CFG_DEBUG_USE_IM_TRC
#ifndef PNDV_CFG_DEBUG_USE_IM_TRC
        #define PNDV_TRC_STRUC(_TRC_TE_STRUC,_TRC_ELEMENT_NUMBER)    TRC_STRUC(_TRC_TE_STRUC,_TRC_ELEMENT_NUMBER)/* no ";" here! */
        #define PNDV_TRC_TP_INIT(_TRC,_TRC_ELEMENT_NUMBER,_TRC_ID)   TRC_TP_INIT(_TRC,_TRC_ELEMENT_NUMBER,_TRC_ID)
        #define PNDV_TRC_TEH_FILL(_TRC)                              TRC_TEH_FILL(_TRC)
        #define PNDV_TRC_TE_ACCESS(_TRC)                             TRC_TE_ACCESS(_TRC)
#endif

/* @brief Enables the usage of dfc reuse module for record routing
 *
 * DFC is a reuse module for routing records to different firmware parts that
 * can handle them.
 *
 */
//#define PNDV_CFG_USE_DFC

/* @brief enable internal record sink
 *
 *  The internal record responder is used for special handling of read i/o
 *  records (0x8028, 0x8029) for all subslots of the device.
 *  This is used in interface modules (e.g. IM151-3, IM153-4 etc.).
 */
#ifdef PNDV_CFG_USE_DFC
    #define PNDV_CFG_ENABLE_RECORD_RESPONDER
#endif

#ifdef PNDV_CFG_USE_DFC
#define PNDV_DFC_DS_REQ( _RQB_PTR, _REQ_ID, _CON_ID ) dfc_read_write_ds_req( (DFC_REQ_PTR)_RQB_PTR, _REQ_ID, _CON_ID  )

#define PNDV_DFC_DS_DONE( _DS_PTR, _RESP_ID )         dfc_read_write_ds_done(_DS_PTR, _RESP_ID)

#define PNDV_DFC_RQB_DONE( _RQB_PTR, _ERROR_CODE )                                  \
{                                                                                   \
    pndv_set_rqb_err( _RQB_PTR, _ERROR_CODE );                                      \
    dfc_read_write_ds_done( (DFC_REQ_PTR) _RQB_PTR,  PNDV_DFC_RES_ID_PNDV );        \
}

#define PNDV_DFC_OH_DONE( _RQB_PTR, _ERROR_CODE )                                  \
{                                                                                   \
    pndv_set_rqb_err( _RQB_PTR, _ERROR_CODE );                                      \
    dfc_read_write_ds_done( (DFC_REQ_PTR) _RQB_PTR,  PNDV_DFC_RES_ID_OH );        \
}
#endif

#define PNDV_CFG_TRIGGER_ENABLE

#define PNDV_GET_ENTITY_NR(_API, _SLOT, _SUBSLOT, _AR_IDX) pndv_peri_get_entity_index(_SLOT, _SUBSLOT, _AR_IDX)/*pndv_cantor(_SLOT, _SUBSLOT)pnpb_get_entity_index((_SLOT), (_SUBSLOT))*/

/*****************************************************************************/
/* feature confirguration */
/*****************************************************************************/

/*
 * Must be defined if the device does not support any shared ar while in
 * isochronous  mode.
 */
//#define PNDV_CFG_ISOM_NO_SHARED
/*
 * Defines the record index (prm record) that configures the isochronous mode.
 * PNDV is listening for this index to detect the isochronous ar.
 */
//#define PNDV_ISOM_DS_NR_NORM        APMA_ISOM_DS_NR_NORM
/*
 * If defined red phase informations provided with the RIR are copied to the interface
 * see PNDV_IFACE_STRUCT.ar
 */
#ifdef FBPB_USE_RED_INT_INFO
    #define PNDV_CFG_USE_RED_INT_INFO
#endif

/* @brief Enable the use of ADD2/REMOVE2 opcodes instead of ADD/REMOVE
 *
 * If defined enables the usage of CM_OPC_SV_DIAG_ADD2/REMOVE2 instead of CM_OPC_SV_DIAG_ADD/REMOVE
 * for both diag types (channel and generic).
 */
//#define PNDV_CFG_USE_DIAG2

/* @brief Enable the use of ADD2/REMOVE2 opcodes instead of ADD/REMOVE only for channel diag
 *
 * If defined enables the usage of CM_OPC_SV_DIAG_ADD2/REMOVE2 instead of CM_OPC_SV_DIAG_ADD/REMOVE
 * only for channel diag. Generic diag will still use ADD/REMOVE to be able to use diag_tag to distinguish the
 * diags. Therefor it is not allowed to use diag_tag == 1.
 */
//#define PNDV_CFG_USE_MULTIDIAG

#define PNDV_CFG_USE_USERTAG
/*****************************************************************************/
/* device configuration */
/*****************************************************************************/

#define PNDV_RESET_REASON_NONE                          (Unsigned32)0x00000000
#define PNDV_RESET_REASON_CODE_ERR_PRM_CONSISTENCY      (Unsigned32)0x00000001
#define PNDV_RESET_REASON_CODE_RESTART_AFTER_FATAL      (Unsigned32)0x00000002

#define PNDV_CM_DEVICE_NO           DEV_CFG_IM_DEVICE_NO
#define PNDV_CM_AR_NO               CM_CFG_MAX_SV_ARS
#define PNDV_CM_API_NO_DEFAULT      0
#define PNDV_CM_VENDOR_ID           DEV_CFG_IM_VENDOR_ID
#define PNDV_CM_DEVICE_INDEX        PNDV_CM_DEVICE_NO
#define PNDV_CM_DEVICE_INSTANZ      PNDV_CM_DEVICE_NO

#define PNDV_CFG_MAX_PORT_CNT		EDD_CFG_MAX_PORT_CNT

#define PNDV_IM_SUBMODUL_1_ID       0
#define PNDV_IM_HAS_GLOB_PARA       0 /* 0 IM has no global para, 1 IM has global para */

#define PNDV_MAX_ARS_RT             DEV_CFG_MAX_ARS_RT
#define PNDV_MAX_ARS_DA             DEV_CFG_MAX_ARS_DA
#define PNDV_MAX_AR_SET             1                   /**< max count of system redundancy ar_sets (ARs that belong to the same redundant projecting)
                                                             should be half of PNDV_CM_AR_NO or less */

/* IM Module ID                                     */
/* bit4 .. bit0 -> issue of IM                      */
/* bit7 .. bit5 -> component of IM - Standard/HF    */
/* bit31.. bit8 -> IM..                             */

#define PNDV_IM_MODUL_MASK                      0x000000FF
#define PNDV_IM_MODUL_COMP_MASK                 0x000000E0
#define PNDV_IM_MODUL_ISSUE_MASK                0x0000001F
#define PNDV_IM_MODUL_MIN_SOLL_MASK             0x000000FF

#define PNDV_MODUL_ID_MODUL_SPEC_BIT_MASK       0x80000000
#define PNDV_MODUL_ID_MODUL_KENNMASK            0x000000FF
#define PNDV_MODUL_ID_PRM_BYTE1_MASK            0x00FF0000
#define PNDV_MODUL_ID_PRM_BYTE1_SHIFT_POS       16


/*****************************************************************************/
/* peripherie configuration */
/*****************************************************************************/

/* Schalter bei Compactdevice Eco-PN / Motorstarter definieren */
// #define PNDV_CFG_ET200_COMPACT

/* Schalter definieren, wenn Peripherieslots als IuM-faehig eingetragen werden sollen */
#define PNDV_CFG_IM0_BITS_FILTERLISTE

#define PNDV_IM_SLOT_NO                 IOD_CFG_DAP_SLOT_NUMBER
#define PNDV_IM_DEFAULT_SUBSLOT_NR      IOD_CFG_DAP_SUBSLOT_NUMBER
/* expected configuration settings */
#define PNDV_CM_SV_MAX_SLOT_NR			IOD_CFG_MAX_SLOT_NUMBER/* 0x0 ... 0x7fff, highest value for slot number */
#define PNDV_CM_SV_MAX_SUBSLOT_NR		IOD_CFG_MAX_NUMOF_SUBSL_PER_SLOT/* 0x0 ... 0x7fff, highest value for subslot number */
#define PNDV_CM_SV_SUBSLOT_COUNT		IOD_CFG_MAX_NUMOF_SUBSLOTS/* maximum count of used subslots in an ar */

#define PNDV_MAX_SV_ENTITY              PNDV_CM_SV_SUBSLOT_COUNT
#define PNDV_MAX_SV_SUBSLOTS            PNDV_CM_SV_SUBSLOT_COUNT

#define PNDV_AL_PRAL_INFO_MAX_LEN       IOD_CFG_MAX_PROCESS_ALARM_LEN /* to be verified */ /*HySo: not found in norm, set same as UPAL_LEN (becuase used example generates fatal error)*/
#define PNDV_AL_PRAL_INFO_LEN           IOD_CFG_MAX_PROCESS_ALARM_LEN
#define PNDV_AL_UPAL_INFO_LEN           64
#define PNDV_AL_STAL_INFO_LEN           4 /* dummy value  */
#define PNDV_AL_URAL_INFO_LEN           IOD_CFG_MAX_UPLOAED_RETRIEVAL_INFO_LEN  /* MAX(Upload&Retrival = 20, IParam = 24)  */
#define PNDV_MAX_CHN_DIAG_PER_SLOT_AND_MOMENT   130 /* 64 Kanale mit jeweils 2 Diagnosen + 2 f�r die gesamte BG  */
#define PNDV_MAX_EXT_DIAG_PER_SLOT_AND_MOMENT   130
#define PNDV_MAX_GENERIC_DIAG_NUMBER    1
#define PNDV_MAX_GENERIC_DIAG_DATA_LENGTH       IOD_CFG_MAX_GEN_DIAG_DATA_LEN

//#define PNDV_AL_PRAL_INFO_LEN       sizeof(pnpb_additional_process_alarm_info_t)
//#define PNDV_AL_UPAL_INFO_LEN       6
//#define PNDV_MAX_CHN_DIAG_PER_SLOT_AND_MOMENT   PNPB_MAX_CHN_DIAG_PER_SLOT_AND_MOMENT
//#define PNDV_MAX_EXT_DIAG_PER_SLOT_AND_MOMENT   PNPB_MAX_EXT_DIAG_PER_SLOT_AND_MOMENT
//#define PNDV_MAX_GENERIC_DIAG_NUMBER PNPB_MAX_GENERIC_DIAG_NUMBER

/* PNDV_PERI_CHECK_API aktuell nur bei Compact-Devices (defined PNDV_CFG_ET200_COMPACT)
     -> Soll-Ist-Vergleich innerhalb der Check_Ind wird zu PERI gegeben
   Funktionalitaet muss spaeter uebers Interface gehen */
#define PNDV_PERI_CHECK_API( _RET_VAL_PTR, _SOLL_API, _IST_API, _SLOT)        \
{                                                                             \
    *_RET_VAL_PTR = fct__check_api( _SOLL_API, _IST_API, _SLOT );             \
}

/* PNDV_PERI_CHECK_ID aktuell nur bei Compact-Devices (defined PNDV_CFG_ET200_COMPACT)
     -> Soll-Ist-Vergleich innerhalb der Check_Ind wird zu PERI gegeben
   Funktionalitaet muss spaeter uebers Interface gehen */
#define PNDV_PERI_CHECK_ID( _RET_VAL_PTR, _SOLL_ID, _IST_ID, _SLOT)           \
{                                                                             \
    *_RET_VAL_PTR = fct__check_id( _SOLL_ID, _IST_ID, _SLOT );                \
}

#define PNDV_IFACE_CMD_CNT      (5 * (IOD_CFG_MAX_NUMOF_SUBSLOTS + 1) ) //256

#define PNDV_HANDLE_ARFSU_DATA_ADJUST          LSA_TRUE
#define PNDV_NO_DISCARD_IOXS_SUPPORT  		   LSA_FALSE

/*****************************************************************************/
/* Records */
#define PNDV_MAX_RD_IO_DATA_LEN         IOD_CFG_MAX_IO_NET_LENGTH_PER_SUBSLOT /* maximum size for netto IO-Data of rd-input -output-records */
#define PNDV_MAX_RECORD_DATA_LEN        IOD_CFG_MAX_RECORD_LENGTH

/* es wird von den Err-codes nur der Low-Teil genommen */
#define PNDV_EC1_REC_NON               (Unsigned8)0
#define PNDV_EC1_REC_WRITE             (Unsigned8)0xA1 /* Bus-Zugriffsfehler bei DS schreiben */
#define PNDV_EC1_REC_BUSY              (Unsigned8)0xA7
#define PNDV_EC1_REC_NOT_SUPPORTED     (Unsigned8)0xA9
#define PNDV_EC1_REC_INDEX_INVALID     (Unsigned8)0xB0 /* Baugruppe kennt den Datensatz nicht oder Baugruppe nicht miniprotokollfaehig */
#define PNDV_EC1_REC_WRITE_LEN         (Unsigned8)0xB1 /* die angegebene Datensatzlange ist falsch */
#define PNDV_EC1_REC_SLOT_INVALID      (Unsigned8)0xB2 /* Den Steckplatz gibt es nicht bzw. das Modul steckt nicht */
#define PNDV_EC1_REC_STATE_CONFLICT    (Unsigned8)0xB5
#define PNDV_EC1_REC_ACCESS_DENIED     (Unsigned8)0xB6
#define PNDV_EC1_REC_INVALID_RANGE     (Unsigned8)0xB7
#define PNDV_EC1_REC_INVALID_PARAMETER (Unsigned8)0xB8
#define PNDV_EC1_REC_INVALID_TYPE      (Unsigned8)0xB9
#define PNDV_EC1_REC_WRITE_BACKUP      (Unsigned8)0xBA

#ifdef PNDV_CFG_USE_DFC
#define PNDV_DFC_RES_ID_PERI            APMA_PDB_DFC_RES_ID_PERI
#define PNDV_DFC_RES_ID_PNDV            APMA_PDB_DFC_RES_ID_PNDV
#define PNDV_DFC_RES_ID_OH              APMA_PDB_DFC_RES_ID_OH
#define PNDV_DFC_REQ_ID_PNDV_DS         APMA_PDB_DFC_REQ_ID_PNDV_DS
#define PNDV_DFC_REQ_ID_PNDV_RQB        APMA_PDB_DFC_REQ_ID_PNDV_RQB
#define PNDV_DFC_REQ_ID_CM              APMA_PDB_DFC_REQ_ID_CM
#define PNDV_DFC_REQ_ID_PNDV_DS1        APMA_PDB_DFC_REQ_ID_PNDV_DS1
#endif

#define PNDV_PARA_RECORD_INDEX_FIX      0//APMA_PDB_DFC_PARA_DS_NUMBER
#define PNDV_RECORD_ARFSU_DATA_ADJUST   0//APMA_PDB_DFC_RECORD_ARFSU_DATA_ADJUST
#define PNDV_RECORD_INDEX_RD_INP        0//APMA_PDB_DFC_RD_INP_DS_NUMBER
#define PNDV_RECORD_INDEX_RD_OUTP       0//APMA_PDB_DFC_RD_OUTP_DS_NUMBER

#ifdef PNDV_OH_ENABLE

/* pndv_oh.c */
#define PNDV_RECORD_INDEX_OH_PARAM      0//APMA_PDB_DFC_PARA_DS_2_NUMBER
#define PNDV_RECORD_INDEX_OH_CTRL       0//APMA_PDB_DFC_OH_STR_NUMBER
#define PNDV_RECORD_INDEX_OH_MELD       0//APMA_PDB_DFC_OH_DIAG_NUMBER
/* Reservemodul */
/*  In der Modulkennung werden das Konfigurations- (low Byte) und Identregister (high Byte) zusammengefasst
    Konfigurationsregister gleich Null bedeutet Reservemodul
*/
#define PNDV_OH_ET200S_RES_MODUL_MSK    0x000000FF
#define PNDV_OH_ET200S_RES_MODUL        0x00000000
#define PNDV_OH_ET200S_DUMMY_MODUL      0x00000000  /* module_id zum Stecken abgewaehlter Module im CM */

#endif /* PNDV_OH_ENABLE */


/* PNDV_PERI_RECORD_INDEX_RD_INP, PNDV_PERI_RECORD_INDEX_RD_OUTP must be different from PNDV_RECORD_INDEX_RD_INP, PNDV_RECORD_INDEX_RD_OUTP
   because PNDV creates a local record for PERI */
#define PNDV_PERI_RECORD_INDEX_RD_INP   0//APMA_PDB_DFC_RD_INP_DS_NUMBER_PERI
#define PNDV_PERI_RECORD_INDEX_RD_OUTP  0//APMA_PDB_DFC_RD_OUTP_DS_NUMBER_PERI
/*****************************************************************************/
/* check cfg with OH */

#define PNDV_CFG_NO_REAL_MODULE         0xFFFFFFFE
#define PNDV_CFG_NO_SOLL_MODULE         0x0         /* PNDV zeigt mit Uebergabe der Sollkonfig an, dass der Slot nicht zu dieser AR gehoert */

#ifdef PNDV_OH_ENABLE

/* pndv_oh.c */
/* Module-ID fuer abgewaehlte Module beim Optionenhandling
  PNPB soll diese module_id:
  PNDV_CMP_RES_OK liefern
  die IOXS-Vorgabe in pndv_data.iface_ptr->peri_para_ready beachten
*/
#define PNDV_RECORD_INDEX_OH_PARAM      APMA_PDB_DFC_PARA_DS_2_NUMBER
#define PNDV_RECORD_INDEX_OH_CTRL       APMA_PDB_DFC_OH_STR_NUMBER
#define PNDV_RECORD_INDEX_OH_MELD       APMA_PDB_DFC_OH_DIAG_NUMBER

/* Reservemodul */
/*  In der Modulkennung werden das Konfigurations- (low Byte) und Identregister (high Byte) zusammengefasst
    Konfigurationsregister gleich Null bedeutet Reservemodul
*/
#define PNDV_OH_RES_MODUL_MSK           0x000000FF
#define PNDV_OH_RES_MODUL               0x00000000
#define PNDV_OH_DUMMY_MODUL             0x00000000  /* module_id zum Stecken abgewaehlter Module im CM */

#endif /* PNDV_OH_ENABLE */

#define PNDV_DS_STRUC_FN_DS_READ        DEB_FN_DS_READ
#define PNDV_DS_STRUC_FN_DS_WRITE       DEB_FN_DS_WRITE
#define PNDV_DS_STRUC_FN_ERROR_BASE     DEB_FN_ERROR_BASE


/* opaque data */

#define PNDV_MAX_IOCR_PER_AR            2                               /* max count of iocr's */
#define PNDV_OPAQUE_EDD_XRT_PROPERTIES  EDD_XRT_PROPERTIES_DEFAULT;     /* must be EDD_XRT_PROPERTIES_DEFAULT for ERTEC200 */
/* !note: dpb->KRam.offset_ProcessImageEnd set in apma_edd_system_dev_open(EDD_RQB_DDB_INI_TYPE dpb) */



/*****************************************************************************/
/*****************************************************************************/
/*******************************                ******************************/
/*******************************     SYSTEM     ******************************/
/*******************************                ******************************/
/*****************************************************************************/
/*****************************************************************************/


/*****************************************************************************/
/* system interface macros */


/* lock and unlock of all instances which use led-functions */

#define PNDV_ENTER()                                                          \
{                                                                             \
    hama_lock(HAMA_SEMA_PNDV);                                                \
}

#define PNDV_EXIT()                                                           \
{                                                                             \
    hama_unlock(HAMA_SEMA_PNDV);                                              \
}

#define PNDV_NTOHS(_VAL) 	LSAS_NTOHS(_VAL)
#define PNDV_NTOHL(_VAL)	LSAS_NTOHL(_VAL)

#define PNDV_OPEN_DONE(_RESULT)                                               \
{                                                                             \
    UnsignedOpt result = (_RESULT);                                           \
                                                                              \
    if(result != PNDV_OK)                                                     \
    {                                                                         \
        LSAS_ASSERT( 0);                      \
    }                                                                         \
}                                                                             \


#define PNDV_STOP_DONE(_RESULT)                                               \
{                                                                             \
    PNDV_IFACE_CMD_ENTRY_T stop_done_event;                                   \
                                                                              \
    stop_done_event.cmd = PNDV_EV_TO_PERI_PNDV_STOP_DONE;                     \
                                                                              \
    pndv_in_write_debug_buffer_all__(PNDV_DC_STOP_DONE, _RESULT);             \
                                                                              \
    pndv_in_peri_write_coupl_event( stop_done_event);                         \
}                                                                             \

#define PNDV_TRIGGER_PERFRORM_SERV()

#define PNDV_TRIGGER_INTERFACE_RESPONDER()  tskma_task_app_send_pnpb_trigger()

#define PNDV_REQUEST_LOCAL(_rqb)                                              \
{                                                                             \
    LSA_RQB_SET_METHOD_PTR(_rqb, pndv_request);                               \
	OsSendMessage  ( 														  \
                            TSKMA_TASK_ID_APPLICATION,             /* id of the  task (references the task message queue) */ \
                            _rqb,                     /* parameter 2 (request block)*/ \
                            OS_MBX_PRIO_NORM);      /* message priority */ \
}


/*******************************************************************************/
/* interface between pndv and ertec200 i/o data handling                       */
/*******************************************************************************/

/* returning data offset for input or output cr memory / iocr pointer and the ar-index is given */
#define PNDV_ALLOCATE_IOCR_MEMORY(_ar_idx, _ar_set_nr, _session_id, _iocr_ptr)            iom_allocate_iocr_memory((_ar_idx), (_ar_set_nr), (_session_id), (_iocr_ptr))

#define PNDV_COMPLETE_IOCR_INFORMATION(_ar_idx, _ar_set_nr, _session_id, _iocr_ptr) (_iocr_ptr==_iocr_ptr) /*TRUE*/

/* release input and output cr memory of the given ar -> offline indication */
#define PNDV_FREE_IOCR_MEMORY(_ar_idx)                                                    iom_free_iocr_memory(_ar_idx)


/*****************************************************************************/
/* fatal error handling */

#define PNDV_FATAL_ERROR(_ERROR_DETAIL_PTR)                                   \
{                                                                             \
    LSAS_FATAL_ERROR( _ERROR_DETAIL_PTR);          \
}                                                                             \

#define PNDV_RESET_DEVICE(_REASON)                                            \
{                                                                             \
    /* apma_control_option_set((_REASON), TRUE);    */                        \
    /* => connect this to the device reset          */                        \
}                                                                             \

/* PNDV_INDICATE_ALL_ARS_GONE is called, when the device lost its last AR.
 * Use case: info is necessary in RTF case. There, PNIO stack has to firstly abort all ARs before ASIC reset is performed by system adaption */
#define PNDV_INDICATE_ALL_ARS_GONE()

//#define PNDV_IS_MODUL_ID_LT_V7(_soll_id) ( ((_soll_id & IM_MODULE_ID_VERSION_MASK) < IM_MODULE_ID_ISSUE_RELEASE) ? 1 : 0 )

#define PNDV_IS_SUPPORTED_OLD_PROJECTING(_soll_id) (_soll_id!=_soll_id) /* (LSA_FALSE) or ( PNDV_IS_MODUL_ID_LT_V7( (_soll_id) ) )*/

/**
 *  @brief brief_description
 *
 *  This macro is called during pndv startup and it should return the startup condition
 *  whether it is a normal startup or a startup after reset etc.
 *  See also PNDV_RESET_REASON_xxx defines
 *
 */
#define PNDV_GET_RESET_REASON_CODE()   PNDV_RESET_REASON_NONE

/*****************************************************************************/
/* copy data */

#define PNDV_COPY_BYTE(_DEST_PTR,_SRC_PTR,_LEN)                               \
{                                                                             \
    LSAS_MEMCPY((void*)(_DEST_PTR),(void*)(_SRC_PTR),_LEN);                   \
}

#define PNDV_MEMSET(_DST_PTR,_VAL,_LEN)                                       \
{                                                                             \
    LSAS_MEMSET((void*)(_DST_PTR),_VAL,_LEN);                                 \
}

/*#define PNDV_COMPARE_BYTE(_RETURN_VALUE,_PTR_1,_PTR_2,_LEN) */

/*****************************************************************************/
/* memory allocation */

#define PNDV_ALLOC_MEM(_local_mem_ptr_ptr, _length)\
{\
    pndv_data.alloc_mem_counter++;\
    LSAS_ALLOC_COMP_MEM( (_local_mem_ptr_ptr), (_length), (APMA_PDB_FW_MODULE_ID_PNDV) );\
}

#define PNDV_ALLOC_RQB(_local_mem_ptr_ptr, _length)\
{\
    pndv_data.alloc_rqb_counter++;\
    LSAS_ALLOC_COMP_MEM(_local_mem_ptr_ptr, _length, APMA_PDB_FW_MODULE_ID_PNDV);\
}

#define PNDV_FREE_MEM(_ret_val_ptr, _local_mem_ptr)\
{\
    pndv_data.alloc_mem_counter--;\
    LSAS_FREE_COMP_MEM(_ret_val_ptr, _local_mem_ptr, APMA_PDB_FW_MODULE_ID_PNDV);\
}

#define PNDV_FREE_RQB(_ret_val_ptr, _local_mem_ptr)\
{\
    pndv_data.alloc_rqb_counter--;\
    LSAS_FREE_COMP_MEM(_ret_val_ptr, _local_mem_ptr, APMA_PDB_FW_MODULE_ID_PNDV);\
}
/*****************************************************************************/
/* timer allocation */

#define PNDV_USER_ID_TYPE LSA_UINT16

#define PNDV_ALLOC_TIMER(_RET_VAL_PTR, _TIMER_ID_PTR, _TIMER_TYPE, _TIME_BASE)\
{\
    LSAS_ALLOC_TIMER(_RET_VAL_PTR,_TIMER_ID_PTR,_TIMER_TYPE,_TIME_BASE,pndv_connect_ind_trigger_timeout, APMA_PDB_FW_MODULE_ID_PNDV);\
}

#define PNDV_START_TIMER(_RET_VAL_PTR, _TIMER_ID, _USER_ID, _TIME)\
{\
    LSAS_START_TIMER( _RET_VAL_PTR, _TIMER_ID, _USER_ID, _TIME, APMA_PDB_FW_MODULE_ID_PNDV );\
}

#define PNDV_STOP_TIMER(_RET_VAL_PTR, _TIMER_ID)\
{\
    LSAS_STOP_TIMER(_RET_VAL_PTR, _TIMER_ID, APMA_PDB_FW_MODULE_ID_PNDV);\
}

#define PNDV_FREE_TIMER(_RET_VAL_PTR, _TIMER_ID)\
{\
    LSAS_FREE_TIMER(_RET_VAL_PTR, _TIMER_ID, APMA_PDB_FW_MODULE_ID_PNDV);\
}


/*******************************************************************************/
/* Led- und Pin-Steuerung                                                      */
/*******************************************************************************/

/* muss leer definiert werden, wenn keine FSU-Messung */
#define PNDV_CFG_MEASUREMENT_LINK_UP()
#define PNDV_CFG_MEASUREMENT_APPL_READY()


/**** new led implementation ***/

/*
 * Network cable disconnected.
 *
 */
#define PNDV_LED_CTRL_BUS_FAULT()\
{\
    Bsp_EbSetLed (PNIO_LED_ERROR, 1); \
}

/*
 * Network cable connected.
 *
 */
#define PNDV_LED_CTRL_BUS_OK()\
{\
    Bsp_EbSetLed (PNIO_LED_ERROR, 0); \
}

/*
 * Data-AR goes online.
 *
 */
#define PNDV_LED_CTRL_DATA_AR_IN_DATA()\
{\
	Bsp_EbSetLed (PNIO_LED_RUN, 1);  \
}

/*
 * Data-AR goes offline.
 *
 */
#define PNDV_LED_CTRL_DATA_AR_OFFLINE()\
{\
    Bsp_EbSetLed (PNIO_LED_RUN, 0); \
}

/*
 * IRT AR is in sync.
 *
 */
#define PNDV_LED_CTRL_IN_SYNC()\
{\
    Bsp_EbSetLed (PNIO_LED_SYNC, 1);  \
}

/*
 * IRT AR is out of sync.
 *
 */
#define PNDV_LED_CTRL_OUT_OFF_SYNC()\
{\
    Bsp_EbSetLed (PNIO_LED_SYNC, 0);  \
}

/*
 * @brief indicate existing maint events
 *
 * Maintenance event existing.
 *
 */
#define PNDV_LED_CTRL_MAINT_ON()\
{\
    Bsp_EbSetLed (PNIO_LED_MAINT, 1); \
}

/*
 * @brif indicate non existing main events
 *
 * No maintenance events existing.
 *
 */
#define PNDV_LED_CTRL_MAINT_OFF()\
{\
    Bsp_EbSetLed (PNIO_LED_MAINT, 0); \
}

/*
 * System error existing.
 */
#define PNDV_LED_CTRL_SYS_ERROR()\
{\
    Bsp_EbSetLed (PNIO_LED_ERROR, 1); \
}

/*
 * No system error existing.
 */
#define PNDV_LED_CTRL_NO_SYS_ERROR()\
{\
    Bsp_EbSetLed (PNIO_LED_ERROR, 0); \
}


/* Makro bei Abhaengigkeit der Maint-LED von der SF-LED     */
/* besteht keine Abhaengigkeit, kann das Makro leer bleiben */
/* ecoPN -> Maint-LED und SF-LED unter einem Lichtleiter    */
#define PNDV_MAINT_LED_CONTROL(_SF_LED_MODE)

/*****************************************************************************/
/* remanent station parameter */


#define PNDV_GET_REMA_STATION_PRM_LEN( _DATA_LENGHT_PTR)                    \
{                                                                           \
    *(_DATA_LENGHT_PTR) = 0;                                                \
}

/**
 *  @brief write station prm data
 *
 *  @param[in]  _DATA_LENGTH data length to be written
 *
 *  Request to the system to write the data provided under pndv_data.rema_station_prm_ptr
 *  this pointer is given at pndv open with rema_station.prm_ptr
 *  If _DATA_LENGTH is "0" the data should be deleted.
 *
 */
#define PNDV_WRITE_REMA_STATION_PRM_TO_FLASH(_DATA_LENGTH)                  \
{                                                                           \
                    \
}

/*****************************************************************************/
/* remanent option-handling parameter */

#ifdef PNDV_OH_ENABLE

    #define PNDV_GET_OH_CRC16_EXP_CFG( _CRC16_EXP_CFG_PTR)    apma_read_oh_crc16( _CRC16_EXP_CFG_PTR);
    #define PNDV_WRITE_CRC16_EXP_CFG( _CRC16_EXP_CFG)         apma_write_oh_crc16_to_flash(_CRC16_EXP_CFG);
    #define PNDV_GET_OH_DATA_LEN( _DATA_LENGHT_PTR)           apma_read_oh_control_data_len(_DATA_LENGHT_PTR);
    #define PNDV_WRITE_OH_DATA( _DATA_LENGHT)                 apma_write_oh_control_data_to_flash(_DATA_LENGHT);

#else

    #define PNDV_GET_OH_CRC16_EXP_CFG( _CRC16_EXP_CFG_PTR)
    #define PNDV_WRITE_CRC16_EXP_CFG( _CRC16_EXP_CFG)
    #define PNDV_GET_OH_DATA_LEN( _DATA_LENGHT_PTR)
    #define PNDV_WRITE_OH_DATA( _DATA_LENGHT)

#endif
/*****************************************************************************/


#define PNDV_RECORD_INDEX_MULTIPLE_PDEV 	OHA_RECORD_INDEX_MULTIPLE_PDEV
#define PNDV_RECORD_INDEX_MULTIPLE_SNMP 	OHA_RECORD_INDEX_MULTIPLE_SNMP
#define PNDV_RECORD_INDEX_IP_SUITE 			OHA_RECORD_INDEX_IP_SUITE
#define PNDV_RECORD_INDEX_NAME_OF_STATION 	OHA_RECORD_INDEX_NAME_OF_STATION

#define PNDV_GET_PD_DS( _DATA_PTR_PTR, _DATA_LENGTH_PTR)                    \
{                                                                           \
    LSAS_CFG_REMA_READ_PDEV_DATA(_DATA_PTR_PTR, _DATA_LENGTH_PTR );        \
}


#define PNDV_GET_SNMP_DATA( _NAME_PTR_PTR, _NAME_LENGTH_PTR, _CONTACT_PTR_PTR, _CONTACT_LENGTH_PTR, _LOCATION_PTR_PTR, _LOCATION_LENGTH_PTR )\
{\
	LSAS_CFG_REMA_READ_SNMP_DATA( _NAME_PTR_PTR, _NAME_LENGTH_PTR, _CONTACT_PTR_PTR, _CONTACT_LENGTH_PTR, _LOCATION_PTR_PTR, _LOCATION_LENGTH_PTR );\
}
#define PNDV_GET_IP_DATA( _DATA_PTR_PTR, _DATA_LENGTH_PTR)\
{\
	LSAS_CFG_REMA_READ_IP_DATA(_DATA_PTR_PTR, _DATA_LENGTH_PTR )\
}


#define PNDV_GET_NOS_DATA( _DATA_PTR_PTR, _DATA_LENGTH_PTR) \
{\
	LSAS_CFG_REMA_READ_NOS_DATA(_DATA_PTR_PTR, _DATA_LENGTH_PTR )\
}

#if 0
#define PNDV_GET_PRM_LIST(_PRM_QUEUE_PTR)\
{\
    PNDV_PRM_REQUEST_ELEMENT_PTR_T local_elem_ptr;\
\
    local_elem_ptr = PNDV_LIST_FIRST(&((_PRM_QUEUE_PTR)->free_list), PNDV_PRM_REQUEST_ELEMENT_PTR_T);\
    if ( local_elem_ptr == LSA_NULL )\
    {\
        /* keine elemente in free list, darf hier nicht sein */\
\
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);\
    }\
    else\
    {\
        Unsigned32    * data_ptr = 0;\
        Unsigned32      data_length;\
        /* sind remanent gespeicherte PDEV Datensaetze vorhanden ? */\
\
        PNDV_GET_PD_DS ( &data_ptr, &data_length );\
\
        if (data_ptr)\
        {\
\
            CM_UPPER_RQB_PTR_TYPE    rqb_ptr = local_elem_ptr->rqb_ptr;\
\
            rqb_ptr->args.pd.prm_rdwr->record_index         = PNDV_RECORD_INDEX_MULTIPLE_PDEV;\
            rqb_ptr->args.pd.prm_rdwr->record_data_length   = data_length;\
                                                              /* Adresse vom ersten Datenelement der record_data */\
                                                              /* in Originalstruktur steht hier der ptr auf die record_data */\
            rqb_ptr->args.pd.prm_rdwr->record_data          = (CM_COMMON_MEM_U8_PTR_TYPE)data_ptr;\
\
            PNDV_LIST_INSERT_TAIL(&((_PRM_QUEUE_PTR)->in_use), &local_elem_ptr->link);\
        }\
    }\
}
#endif

/*****************************************************************************/
/* system interface parameters */


/*****************************************************************************/
/*****************************************************************************/
/*******************************                ******************************/
/*******************************       CM       ******************************/
/*******************************                ******************************/
/*****************************************************************************/
/*****************************************************************************/


/*****************************************************************************/
/* cm interface macros */

#if 0
#define PNDV_CM_OPEN_CHANNEL_LOWER(_rqb)                                            \
{                                                                                   \
    _rqb->args.channel.open->cm_request_upper_done_ptr                              \
                = lsas_cm_to_appl_cbf;                   \
                                                                                    \
    LSAS_RQB_CM_REQUEST(_rqb, cm_open_channel);                                  \
}
#endif

#define PNDV_CM_CLOSE_CHANNEL_LOWER(_rqb)                                           \
{                                                                                   \
    LSAS_RQB_CM_REQUEST(_rqb, cm_close_channel);                                 \
                                                                                    \
}

#define PNDV_CM_REQUEST(_rqb)                                       \
{                                                                   \
    LSAS_RQB_CM_REQUEST(_rqb, cm_request);\
}                                                                   \

#if 0
#define PNDV_CM_FILL_DEVICE_ADD_TYPE(_dev_add_ptr)                  \
{\
        (_dev_add_ptr)->device_nr = PNDV_CM_DEVICE_NO;             \
        (_dev_add_ptr)->device_id                     = pndv_data.cfg.device_id;      \
        (_dev_add_ptr)->vendor_id                     = PNDV_CM_VENDOR_ID;    \
        (_dev_add_ptr)->instance                      = PNDV_CM_DEVICE_INSTANZ;       \
        PNDV_COPY_BYTE( &(_dev_add_ptr)->annotation[0],   \
                        pndv_data.annotation_ptr,\
                        CLRPC_MAX_ANNOTATION_SIZE);\
        (_dev_add_ptr)->max_ars_IOC                    = PNDV_MAX_ARS_RT;              \
        (_dev_add_ptr)->max_ars_IOS                    = 0;\
        (_dev_add_ptr)->max_ars_DAC                    = PNDV_MAX_ARS_DA;              \
        (_dev_add_ptr)->max_slot_nr                   = PNDV_CM_SV_MAX_SLOT_NR;            \
        (_dev_add_ptr)->max_subslot_nr                = PNDV_MAX_SV_REAL_SUBSLOT_NR;\
        (_dev_add_ptr)->max_nr_of_subslots            = PNDV_MAX_SV_OVERALL_SUBSLOTS ;\
        (_dev_add_ptr)->alarm_send_max_length         = PNDV_ALARM_SEND_MAX_LENGTH; \
        (_dev_add_ptr)->contains_pdev                 = LSA_TRUE;\
        (_dev_add_ptr)->central_pdev                  = LSA_FALSE;\
        (_dev_add_ptr)->parking_mode                  = LSA_TRUE;\
        (_dev_add_ptr)->write_multiple_parallelism    = 1;\
        (_dev_add_ptr)->SRD_enabled = LSA_TRUE;\
        (_dev_add_ptr)->CIR_enabled = LSA_FALSE;\
        (_dev_add_ptr)->nr_of_event_resources         = 0;\
        (_dev_add_ptr)->nr_of_alarm_resources         = 0;\
}
#endif
/*****************************************************************************/
/*****************************************************************************/
/*********************************             *******************************/
/*********************************     RQB     *******************************/
/*********************************             *******************************/
/*****************************************************************************/
/*****************************************************************************/

#define PNDV_FCTATTR_FLAT  /* nothing */
#define PNDV_MEMATTR_FLAT  /* nothing */


/*===========================================================================*/
/*                         code- and data-attributes                         */
/*===========================================================================*/

/*=====  code attributes  =====*/

#define PNDV_UPPER_IN_FCT_ATTR      PNDV_FCTATTR_FLAT   /* user to pndv */
#define PNDV_UPPER_OUT_FCT_ATTR     PNDV_FCTATTR_FLAT   /* pndv to user */
#define PNDV_SYSTEM_IN_FCT_ATTR     PNDV_FCTATTR_FLAT   /* system to pndv    */
#define PNDV_SYSTEM_OUT_FCT_ATTR    PNDV_FCTATTR_FLAT   /* pndv to system  */
#define PNDV_LOCAL_FCT_ATTR         PNDV_FCTATTR_FLAT   /* local function    */

/*=====  data attributes  =====*/

#define PNDV_UPPER_RQB_ATTR         PNDV_MEMATTR_FLAT   /* upper-rqb data    */
#define PNDV_UPPER_MEM_ATTR         PNDV_MEMATTR_FLAT   /* upper-memory data */
#define PNDV_SYSTEM_MEM_ATTR        PNDV_MEMATTR_FLAT   /* system-memory data*/
#define PNDV_LOCAL_MEM_ATTR         PNDV_MEMATTR_FLAT   /* local data        */
#define PNDV_COMMON_MEM_ATTR        PNDV_MEMATTR_FLAT   /* greatest common
                                                              data attribute */

    /* RQB handling */

#define PNDV_RQB_HEADER   LSA_RQB_HEADER(PNDV_RQB_PTR_TYPE)
#define PNDV_RQB_TRAILER  LSA_RQB_TRAILER

#define PNDV_RQB_SET_NEXT_RQB(_rb, _value)                  LSA_RQB_SET_NEXT_RQB_PTR(_rb, _value)
#define PNDV_RQB_SET_PREV_RQB(_rb, _value)                  LSA_RQB_SET_PREV_RQB_PTR(_rb, _value)
#define PNDV_RQB_SET_OPCODE(_rb, _value)                    LSA_RQB_SET_OPCODE(_rb, _value)
#define PNDV_RQB_SET_HANDLE(_rb, _value)                    LSA_RQB_SET_HANDLE(_rb, _value)
#define PNDV_RQB_SET_USER_ID_UVAR16_ARRAY_LOW(_rb, _value)  LSA_RQB_SET_USER_ID_UVAR16_ARRAY_LOW(_rb, _value)
#define PNDV_RQB_SET_USER_ID_UVAR16_ARRAY_HIGH(_rb, _value) LSA_RQB_SET_USER_ID_UVAR16_ARRAY_HIGH(_rb, _value)

#define PNDV_RQB_GET_NEXT_RQB(_rb)                          LSA_RQB_GET_NEXT_RQB_PTR(_rb)
#define PNDV_RQB_GET_PREV_RQB(_rb)                          LSA_RQB_GET_PREV_RQB_PTR(_rb)
#define PNDV_RQB_GET_OPCODE(_rb)                            LSA_RQB_GET_OPCODE(_rb)
#define PNDV_RQB_GET_HANDLE(_rb)                            LSA_RQB_GET_HANDLE(_rb)
#define PNDV_RQB_GET_USER_ID_UVAR16_ARRAY_LOW(_rb)          LSA_RQB_GET_USER_ID_UVAR16_ARRAY_LOW(_rb)
#define PNDV_RQB_GET_USER_ID_UVAR16_ARRAY_HIGH(_rb)         LSA_RQB_GET_USER_ID_UVAR16_ARRAY_HIGH(_rb)

#define PNDV_RQB_SET_RESPONSE(_rb, _value)                  LSA_RQB_SET_RESPONSE(_rb, _value)
#define PNDV_RQB_GET_RESPONSE(_rb)                          LSA_RQB_GET_RESPONSE(_rb)


/*****************************************************************************/
/*****************************************************************************/
/*********************************             *******************************/
/*********************************     PNDV    *******************************/
/*********************************             *******************************/
/*****************************************************************************/
/*****************************************************************************/


/*****************************************************************************/
/* mengengeruest */


#define PNDV_MAX_EVENT_RQB              (4*CM_CFG_MAX_SV_ARS) + 2 + 1/* 4 pro AR + 1xcycl_drop + 1xlink_state +1 RTC3 Follows Ind*/
#define PNDV_MAX_EVENT_CMPD_RQB         2                         /* CM_OPC_PD_EVENT_APPL_READY_IND / _RSP */
#define PNDV_MAX_LOW_PRIO_ALARM_RQB     (1*CM_CFG_MAX_SV_ARS)
#define PNDV_MAX_HIGH_PRIO_ALARM_RQB    (1*CM_CFG_MAX_SV_ARS)
#define PNDV_MAX_IOCR_PRO_AR            10

//extern Unsigned16 cksum_byte(Unsigned8 * buf_ptr, Unsigned32 len);
#define PNDV_CKSUM(buff_, len_)    ~cksum_byte(buff_, len_)

/*****************************************************************************/
/*****************************************************************************/
/*******************************                 *****************************/
/*******************************       END       *****************************/
/*******************************                 *****************************/
/*****************************************************************************/
/*****************************************************************************/


/*****************************************************************************/
/* internal configuration verification */


#include "pndv_plau.h"


/*****************************************************************************/
/* reinclude-protection */


#else
    #pragma message ("The header PNDV_CFG.H is included twice or more !")
#endif


/*****************************************************************************/
/*  end of file.                                                             */
/*****************************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
