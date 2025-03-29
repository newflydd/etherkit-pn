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
/*  F i l e               &F: pnpb_com.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  export information.                                                      */
/*                                                                           */
/*****************************************************************************/


/*****************************************************************************/
/* contents:

*/
/*****************************************************************************/
/* reinclude protection */


#ifndef PNPB_COM_H
#define PNPB_COM_H


#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif


#define PNPB_INVALID_INDEX16     0xffff
#define PNPB_INVALID_INDEX32     0xffffffff
#define PNPB_INVALID_INDEX       PNPB_INVALID_INDEX32

/*****************************************************************************/
/*****************************************************************************/
/*******************************                 *****************************/
/*******************************     COMMON      *****************************/
/*******************************                 *****************************/
/*****************************************************************************/
/*****************************************************************************/

// *------------------------------------------
// * return codes
// *------------------------------------------

#define PNPB_OK                              ((PNIO_UINTOpt)0x01)
#define PNPB_ERR_NOT_SUPPORTED               ((PNIO_UINTOpt)0x02)

// *------------------------------------------
// * global pnpb settings
// *------------------------------------------

#define PNPB_IM_SLOT_NR                             0
#define PNPB_FIRST_PERI_SLOT_NR                     1
#define PNPB_MAX_MODULES                            31
#define PNPB_MAX_SLOTS                              32 /* 32 Slots = 30 Modules + 1 PS + 1 IM -> == CM_CFG_MAX_SV_SLOTS */
#define PNPB_MAX_TOTAL_PERI_SUBSLOTS                256
#define PNPB_MAX_NR_OF_AR                           3  /* (0, 1  controller AR, 2: supervisor)*/

/* PNPB system redundancy addition for ErtecDevKit43 and later*/
#if(IOD_INCLUDE_S2_REDUNDANCY == 1)
#define PNPB_SYSTEM_REDUNDANCY
#endif

#ifdef PNPB_SYSTEM_REDUNDANCY
#define PNPB_MAX_AR		IOD_CFG_NUMOF_AR
#define PNPB_MAX_S2_AR_SETS	(PNPB_MAX_AR/2)
#define REDUNDANT_PRIMARY_STATUS_BIT 	0x00010000


typedef enum
{
    PNPB_IO_DATA_STATE_P_B_BACKUP             = 0,
    PNPB_IO_DATA_STATE_P_B_PRIMARY            = 1,
    PNPB_IO_DATA_STATE_P_B_BACKUP_PROCEEDING  = 2,
    PNPB_IO_DATA_STATE_P_B_PRIMARY_PROCEEDING = 3,
    PNPB_IO_DATA_STATE_P_B_UNDETERMINED      = 0xFF
} PNPB_IO_DATA_STATE_P_B;

typedef enum
{
    PNPB_S2_STATE_NO_AR_OR_ALL_BACKUP        = 0,
    PNPB_S2_STATE_PRIMARY                    = 1,
    PNPB_S2_STATE_HOLD_O_DATA                = 2
} PNPB_S2_STATE;




	extern PNIO_AR_TYPE pnpb_ArType[ PNPB_MAX_AR + 1 ];

	typedef enum
	{
		PNPB_S2_AR_PRESENT,
		PNPB_NO_S2_AR
	}PNPB_S2_PRESENT;
	extern PNPB_S2_PRESENT pnpb_s2_present;

	typedef struct PNPB_DATA
	{
		struct
		{
			struct
			{
				PNIO_UINT32				rdht[PNPB_MAX_S2_AR_SETS];
				PNIO_UINT32				rdht_running_timer_val[PNPB_MAX_S2_AR_SETS];
				PNPB_IO_DATA_STATE_P_B 	primary_backup_state[ PNPB_MAX_AR + 1 ];	/* holds the current state of the primary bit for each ar                 */
				PNIO_UINT16 			current_io_controller_idx[PNPB_MAX_S2_AR_SETS];				/* the ar index of the latest controller that went from backup to primary */
				PNPB_S2_STATE           state[PNPB_MAX_S2_AR_SETS];                              	/* the state machine due to figure #30 of the draft spec                  */
			}s2;
		}io;
		struct
		{
			LSAS_RQB_TYPE rqb;
			PNIO_UINT8 rqb_in_use;
			PNIO_UINT16	ident;
		}timer[PNPB_MAX_S2_AR_SETS];
		struct
		{
			PNIO_UINT32	im_module_id;
			PNIO_UINT16	component_id;
		}init;

	}PNPB_DATA;

	extern PNPB_DATA pnpb_data;



	extern PNIO_EXP 					PnpbExp[IOD_CFG_NUMOF_AR];
	extern PNIO_UINT32					Pnpb_ar_sr_set[IOD_CFG_NUMOF_AR];

/*new macros*/
#define PNPB_IN_SET_DATA_STATE(_ar_no, _value, _mask) 	                                                                                                                       \
{                                                                                                                                                                              \
	PNPB_SYS_TRACE_03(LSA_TRACE_LEVEL_NOTE, "set_data_state [arno:%x arstate:%x ar_set:%x]", _ar_no, Pnpb.ArState[_ar_no], Pnpb_ar_sr_set[_ar_no] );                          \
	iom_provider_set_data_state(_ar_no, _value, _mask);                                                                                                                        \
}

#define PNPB_IN_SET_PRIMARY_ARID(_ar_no)	iom_set_session_key_primary_arid(_ar_no)

#define pnpb_ar_in_data__(_idx, _ArState)                ( ((_idx) < PNPB_MAX_AR) && ((_ArState[(_idx)] == PNPB_AR_INDATA_IND) || (_ArState[(_idx)] == PNPB_AR_SM_INDATA_IND) ) )
#define pnpb_ar_prm_ok__(_idx, _ArState)                ( ((_idx) < PNPB_MAX_AR) && ((_ArState[(_idx)] >= PNPB_AR_PARAM_END_FINISHED) ) )
#define pnpb_max__(_a,_b)                       ( (_a) > (_b) ? (_a) : (_b) )
#define pnpb_host_ptr_is_nil__(_ptr1)           ( (_ptr1) == 0 )

#define PNPB_AR_OTHER(_ar)								pnpb_second_ar_of_arset( _ar )
#define PNPB_AR_SET(_ar, _psr)							{															\
															*_psr = Pnpb_ar_sr_set[ _ar ];							\
															if( 0 == *_psr )										\
															{														\
																/*0 is for non redundant - this should not happen*/	\
																PNPB_SYS_TRACE_01( LSA_TRACE_LEVEL_FATAL, "Trying to obtain sr_set of non-redundant AR %x", _ar );	\
															}														\
														}

/*function prototypes*/
	PNIO_UINT32 	pnpb_initiate_s2_data_read  		(PNIO_UINT32   DevHndl);
	PNIO_UINT32 	pnpb_initiate_s2_data_write 		(PNIO_UINT32   DevHndl);
	void 			pnpb_io_s2_ar_set_trigger_cnf		(PNIO_UINT32 ar_idx, PNIO_UINT8 edge);
	void    		pnpb_io_s2_ar_set_trigger_req    	(Unsigned32 ar_idx);
	PNIO_UINT8    	pnpb_io_s2_ar_release 				(Unsigned32 ar_idx);
	void            pnpb_rdht_timeout                   (PNIO_UINT32 ArSet);
	void    		pnpb_trigger_rdht_timeout 			(LSA_UINT16 timer_id, LSA_USER_ID_TYPE user_id);
	PNIO_UINT32		pnpb_second_ar_of_arset				(PNIO_UINT32 first_ar);

#else
extern    PNIO_EXP  PnpbExp[IOD_CFG_NUMOF_AR];
#endif

extern PNDV_IFACE_STRUCT_PTR    	pPnpbIf;
// *------------------------------------------
// * debug feature settings
// *------------------------------------------
typedef enum pnpb_debug_mode_def_e
{
  PNPB_DEBUG_OFF           = 0,      /* no trace active */
  PNPB_DEBUG_ON_ALL

} pnpb_debug_mode_def_t;

// *------------------------------------------
// * data type describing the content of one process alarm
// *------------------------------------------
typedef struct pnpb_additional_process_alarm_info_s
{
    PNIO_UINT16      userStructureIdentifier;
    PNIO_UINT8       channelNumber;
    PNIO_UINT8       eventTyp;

}   pnpb_additional_process_alarm_info_t;

typedef pnpb_additional_process_alarm_info_t * pnpb_additional_process_alarm_info_ptr_t;

// *------------------------------------------
// * how many channel dignosis data elements can be contained in one PBUP diag alarm
// *------------------------------------------
#define PNPB_MAX_CHN_DIAG_PER_SLOT_AND_MOMENT 46

// *------------------------------------------
// * how many generic dignosis events can occur
// *------------------------------------------
#define PNPB_MAX_GENERIC_DIAG_NUMBER 7

// *------------------------------------------
// *open parameter type of the PNPB component
// *------------------------------------------
typedef struct pnpb_open_parameter_s
{
    PNIO_UINTOpt redundancy;
    PNDV_IFACE_STRUCT_PTR pndvInterface;
    void (*done_cbf)(void);

}   pnpb_open_parameter_t;

typedef pnpb_open_parameter_t  * pnpb_sys_parameter_ptr_t;

// *------------------------------------------
// * LSA version function type
// *------------------------------------------
typedef LSA_VERSION_TYPE  * pnpb_sys_version_ptr_t;

// *------------------------------------------
// *debug buffer data type
// *------------------------------------------
typedef struct pnpb_debug_info_s
{
    PNIO_UINT8   *traceDataPtr;
    PNIO_UINT32   traceDataLength;
    PNIO_UINT8   *localDataPtr;
    PNIO_UINT32   localDataLength;

}   pnpb_debug_info_t;

typedef pnpb_debug_info_t  * pnpb_sys_debug_info_ptr_t;


// *------------------------------------------
// * exported function prototypes
// *------------------------------------------
#ifdef __cplusplus
extern "C"
{
#endif

void            pnpb_varinit               (void);
void            pnpb_init                  (void);
void            pnpb_open                  (pnpb_sys_parameter_ptr_t parameter_ptr);
void            pnpb_close                 (void);
void            pnpb_start                 (void);
void            pnpb_stop                  (void);
void            pnpb_activate              (void);
void            pnpb_deactivate            (void);
void            pnbp_StartTaskPost         (void);
void            pnpb_perform_services      (void);
void            pnpb_handle_nau_irq        (void);
PNIO_UINT16     pnpb_get_free_entity_index (PNIO_UINT32   api,
                                            PNIO_UINT32   slot,
                                            PNIO_UINT32   sub,
                                            PNIO_UINT32* pEntityIndex);


#ifdef PNPB_ENVIRONMENT_ADONIS

void          pnpb_ms_asic_interrupt_isr(int param, void *ptr);
void          pnpb_io_provider_buffer_change_isr(int param, void *ptr);
void          pnpb_io_irte_isr_io( int param, void *ptr );
void          pnpb_io_irte_isr_io_isom( int param, void *ptr );

#else /* #ifdef PNPB_ENVIRONMENT_ADONIS */

void         pnpb_ms_asic_interrupt_isr(void);
void         pnpb_io_provider_buffer_change_isr(void);
void         pnpb_io_irte_isr_io(void);
void         pnpb_io_irte_isr_io_isom(void);

#endif /* #ifdef PNPB_ENVIRONMENT_ADONIS */

int     pnpb_is_interface_empty(void);

void pnpb_cbf_trigger_io_exchange (void);

#ifdef __cplusplus
}
#endif


/*****************************************************************************/
/*****************************************************************************/
/*******************************                 *****************************/
/*******************************       PNPB       *****************************/
/*******************************                 *****************************/
/*****************************************************************************/
/*****************************************************************************/


/*****************************************************************************/
/* reinclude-protection */


#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif


/*** end of file *************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
