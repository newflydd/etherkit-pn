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
/*  F i l e               &F: pndv_cm.c                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Operation of the RQB interface from CM                                   */
/*                                                                           */
/*****************************************************************************/


/*****************************************************************************/
/* contents:

    - pndv_cm_callback

    - pndv_in_cm_open_channel
    - pndv_in_cm_open_channel_done
    - pndv_in_cm_close_channel
    - pndv_in_cm_close_channel_done

    - pndv_in_cm_put_resource
    - pndv_in_cm_event_res_done
    - pndv_in_cm_al_res_done

    - pndv_in_cm_overload_info
    - pndv_in_cm_link_status_info
    - pndv_in_cm_dev_led_info
    - pndv_in_cm_pdsv_create
    - pndv_in_cm_pdsv_create_done
    - pndv_in_cm_pdsv_delete
    - pndv_in_cm_pdsv_delete_done
    - pndv_in_cm_sv_create
    - pndv_in_cm_sv_create_done
    - pndv_in_cm_sv_delete
    - pndv_in_cm_sv_delete_done
    - pndv_in_cm_sv_control_done
    - pndv_in_cm_check_dev_req
    - pndv_in_cm_sv_device_add
    - pndv_in_cm_sv_device_add_done
    - pndv_in_cm_sv_device_remove
    - pndv_in_cm_sv_device_remove_done
    - pndv_in_cm_sv_ap_add
    - pndv_in_cm_sv_ap_add_done
    - pndv_in_cm_sv_ap_remove
    - pndv_in_cm_sv_ap_remove_done

    - pndv_in_device_control
    - pndv_in_cm_sv_device_control_done

    - pndv_in_cm_sv_appl_ready
    - pndv_in_cm_sv_appl_ready_done

    - pndv_in_cm_sv_ar_control_done
    - pndv_in_cm_sv_ar_abort_done

    - pndv_in_cm_sv_event_prm_end_ind
    - pndv_in_cm_sv_event_prm_end_continue
    - pndv_in_cm_sv_event_ar_rtc3_ind
    - pndv_in_cm_sv_ar_rtc3_res
    - pndv_in_cm_sv_event_apdu_status_ind

    - pndv_in_cm_ar_in_data_ind


*/
/*****************************************************************************/
/* 2do:


*/
/*****************************************************************************/
/* include hierarchy */


#include "pndv_inc.h"

#include "pndv_int.h"
#include "pndv_dat.h"

#ifdef PNDV_CFG_ENABLE_R1
#include "pndv_r1_bridge.h"
#endif

#define PNDV_MODULE PNDV_ERR_MODULE_CM


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/**********                                                         **********/
/**********               SYSTEM INTERFACE FUNCTIONS                **********/
/**********                                                         **********/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/**********                                                         **********/
/**********                 CM INTERFACE FUNCTIONS                  **********/
/**********                                                         **********/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/


void pndv_cm_callback (CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    UnsignedOpt                     free_rqb,
                                    rqb_back;

    int                             tmp_int;

    free_rqb       = TRUE;
    rqb_back       = FALSE;

    /* ignore
    PNDV_RQB_GET_HANDLE( rqb_ptr);
    */



    switch ( PNDV_RQB_GET_OPCODE( rqb_ptr) )
    {

        /******************** CM Channel *********************/
        /******************* Confirmations *******************/

        case CM_OPC_OPEN_CHANNEL:
        {
            pndv_in_cm_open_channel_done( rqb_ptr);

            break;
        }

        case CM_OPC_CLOSE_CHANNEL:
        {
            pndv_in_cm_close_channel_done( rqb_ptr);

            break;
        }

        case CM_OPC_PD_PROVIDE_EVENT:
        {
            pndv_in_cm_event_res_done( rqb_ptr);

            break;
        }

        case CM_OPC_PD_PROVIDE_ALARM:
        {
            pndv_in_cm_al_res_done( rqb_ptr);

            break;
        }

        /******************** CM PD **************************/
        /******************* Confirmations *******************/

        case CM_OPC_PD_CREATE:
        {
            pndv_in_cm_pdsv_create_done( rqb_ptr );

            break;
        }

        case CM_OPC_PD_DELETE:
        {
            pndv_in_cm_pdsv_delete_done( rqb_ptr );

            break;
        }

        case CM_OPC_PD_LINK_STATUS_INFO:
        {
            rqb_back = pndv_in_cm_link_status_info( rqb_ptr);

            if ( TRUE == rqb_back )
            {
                /* RQB goes back to CM unchanged */

                free_rqb = FALSE;
            }
            else
            {
                /* if RQB does not go back, it will be released and allocated with the next open_channel  */

                free_rqb = TRUE;
            }

            break;
        }

        case CM_OPC_PD_REMAP:
        {
            //! we do not do that
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, PNDV_RQB_GET_OPCODE( rqb_ptr));
            break;
        }

        case CM_OPC_PD_PRM_READ:
        {
            //! we do not do that
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, PNDV_RQB_GET_OPCODE( rqb_ptr));
            break;
        }

        case CM_OPC_PD_PRM_BEGIN:
        {
            pndv_in_cm_pd_prm_begin_done( rqb_ptr );

            break;
        }

        case CM_OPC_PD_PRM_WRITE:
        {
            free_rqb = FALSE;

            pndv_in_cm_pd_prm_write_done( rqb_ptr );

            break;
        }

        case CM_OPC_PD_PRM_END:
        {
            pndv_in_cm_pd_prm_end_done( rqb_ptr );

            break;
        }

        case CM_OPC_PD_SYNC_LED_INFO:
        {
            free_rqb = FALSE;

            rqb_back = pndv_in_cm_sync_led_info(rqb_ptr);

            break;
        }

        /******************** Indications ********************/

        case CM_OPC_PD_ALARM_IND:
        {
            //! we don't want that at all
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, PNDV_RQB_GET_OPCODE( rqb_ptr));
            break;
        }

        case CM_OPC_PD_EVENT_APPL_READY_IND:
        {
            free_rqb = FALSE;

            pndv_in_cm_pd_event_appl_ready_ind( rqb_ptr);

            break;
        }

        /******************** CM Server **********************/
        /******************* Confirmations *******************/

        case CM_OPC_SV_CREATE:
        {
            pndv_in_cm_sv_create_done( rqb_ptr);

            break;
        }

        case CM_OPC_SV_DELETE:
        {
            pndv_in_cm_sv_delete_done( rqb_ptr);

            break;
        }

        case CM_OPC_SV_CONTROL:
        {
            /* Do not release statically created RQB and mark it as free */

            free_rqb = FALSE;

            PNDV_RQB_SET_OPCODE( rqb_ptr, 0);

            pndv_in_cm_sv_control_done( rqb_ptr);

            break;
        }

        case CM_OPC_SV_DEVICE_ADD:
        {
            pndv_in_cm_sv_device_add_done( rqb_ptr);

            break;
        }

        case CM_OPC_SV_DEVICE_REMOVE:
        {
            pndv_in_cm_sv_device_remove_done( rqb_ptr);

            break;
        }

        case CM_OPC_SV_DEVICE_CONTROL:
        {
            /* Do not release statically created RQB and mark it as free */

            free_rqb = FALSE;

            PNDV_RQB_SET_OPCODE( rqb_ptr, 0);

            pndv_in_cm_sv_device_control_done( rqb_ptr);

            break;
        }

        case CM_OPC_SV_DEVICE_LED_INFO:
        {
            /* Do not release statically created RQB */

            free_rqb = FALSE;

            rqb_back = pndv_in_cm_dev_led_info( rqb_ptr);

            break;
        }

        case CM_OPC_SV_DEVICE_PROVIDE_EVENT:
        {
            pndv_in_cm_sv_device_provide_event_done(rqb_ptr);
            break;
        }

        case CM_OPC_SV_DEVICE_PROVIDE_ALARM:
        {
            pndv_in_cm_sv_device_provide_alarm_done(rqb_ptr);
            break;
        }

#ifdef PNDV_CFG_USE_DEVICE_READ
        case CM_OPC_SV_DEVICE_READ:
        {
            free_rqb = FALSE;

            pndv_in_cm_sv_device_read_done(rqb_ptr);
            break;
        }
#endif
        case CM_OPC_SV_SUBMODULE_ADD:
        {
            /* Do not release statically created RQB and mark it as free */

            free_rqb = FALSE;

            pndv_pp_cm_submodule_add_done(rqb_ptr);

            break;
        }

        case CM_OPC_SV_SUBMODULE_REMOVE:
        {
            /* Do not release statically created RQB and mark it as free */

            free_rqb = FALSE;

            pndv_pp_cm_submodule_remove_done( rqb_ptr);

            break;
        }

#if (defined PNDV_CFG_USE_DIAG2)||(defined PNDV_CFG_USE_MULTIDIAG)
        case CM_OPC_SV_DIAG_ADD2:
        {
            free_rqb = FALSE;

            pndv_in_al_diag_add_done( rqb_ptr);
            break;
        }

        case CM_OPC_SV_DIAG_REMOVE2:
        {
            /* Do not release statically created RQB and mark it as free */

            free_rqb = FALSE;

            pndv_in_al_diag_remove2_done( rqb_ptr);;
            break;
        }
#endif
#if !(defined PNDV_CFG_USE_DIAG2)
        case CM_OPC_SV_DIAG_ADD:
        {
            free_rqb = FALSE;

            pndv_in_al_diag_add_done( rqb_ptr);

            break;

        }

        case CM_OPC_SV_DIAG_REMOVE:
        {
            /* Do not release statically created RQB and mark it as free */

            free_rqb = FALSE;

            pndv_in_al_diag_remove_done( rqb_ptr);

            break;
        }
#endif

        case CM_OPC_SV_ARSET_TRIGGER:
        {
            free_rqb = FALSE;
            rqb_back = FALSE;

            pndv_ar_cm_sr_ar_set_trigger_cnf(rqb_ptr);

            break;
        }
        case CM_OPC_SV_ARSET_ABORT:
        {
            free_rqb = FALSE;
            rqb_back = FALSE;

            pndv_ar_cm_sr_rdht_timeout_cnf(rqb_ptr);

            break;
        }
        case CM_OPC_SV_AR_APPL_READY:
        {
            /* Do not release statically created RQB and mark it as free */

            free_rqb = FALSE;

            PNDV_RQB_SET_OPCODE( rqb_ptr, 0);

            pndv_ar_cm_sv_appl_ready_done( rqb_ptr);

            break;
        }

        case CM_OPC_SV_AR_CONTROL:
        {
            pndv_in_cm_sv_ar_control_done( rqb_ptr);

            break;
        }

        case CM_OPC_SV_AR_ABORT:
        {
            /* Do not release statically created RQB and mark it as free */

            free_rqb = FALSE;

            pndv_ar_cm_ar_abort_done( rqb_ptr);

            break;
        }

        case CM_OPC_SV_AR_RIR_ACK:
        {
            free_rqb = FALSE;

            // confirmation to rir_ack arrived
            pndv_ar_cm_sv_rir_ack_cnf(rqb_ptr);
            break;
        }

        case CM_OPC_SV_AR_ALARM_SEND:
        {
            // only static rqbs used here
            free_rqb = FALSE;
            pndv_in_al_al_ack( rqb_ptr);

            break;
        }

        case CM_OPC_SV_AR_ALARM_ACK:
        {
            //! not used
            free_rqb = FALSE;
            rqb_back = TRUE;

            pndv_in_al_sv_alarm_ack_done( rqb_ptr);

            break;
        }

        /******************** Indications ********************/

        case CM_OPC_SV_AR_ALARM_IND:
        {
            /* can't actually come because CM_OPC_SV_CREATE
               the reject bit is set for all alarms
               -> CM acknowledges with PNIO_ERR */

            free_rqb = FALSE;
            rqb_back = TRUE;

            pndv_in_al_sv_alarm_ind( rqb_ptr);

            break;
        }

        case CM_OPC_SV_AR_CONNECT_IND:
        {
            free_rqb = FALSE;
            //! OK
            if (  (pndv_data.cfg.peri_cfg.sm_state == PNDV_SM_PERIBUS_STATE_UNKNOWN)
                &&(  (rqb_ptr->args.sv.ar_event->u.sv.ar_connect->ar_type != CM_AR_TYPE_SUPERVISOR)
                   ||(rqb_ptr->args.sv.ar_event->u.sv.ar_connect->nr_of_iocrs != 0)
                   )
                )
            {

                Unsigned16 ret_val;
                LSA_USER_ID_TYPE user_id;

                if (pndv_ar_do_service_queue(rqb_ptr, PNDV_PERI_SERVICE_IDENT_CON) != PNDV_OK)
                {
                    //! queueing not possible, no free resource, check PNDV_PERI_SERVICE_QUEUES_T elem
                    pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
                }

                /* start the timeout timer */
                user_id.uvar32 = 0;
                if (pndv_data.serv.con_serv_run == FALSE)
                {
                    pndv_data.serv.con_serv_run = TRUE;
                    PNDV_START_TIMER( &ret_val, pndv_data.connect_indication_timeout_timer_id, user_id, 33);
                    LSA_UNUSED_ARG(ret_val);
                }
                else
                {
                    /* don't restart the timer */
                }

                pndv_in_write_debug_buffer_all_add__(PNDV_DC_CM_CONNECT_IND_3, 0xffff,0);
            }
            else

            {
                pndv_ar_cm_connect_ind(rqb_ptr);
            }
            break;
        }

        case CM_OPC_SV_AR_DISCONNECT_IND:
        {
            //! OK
            free_rqb = FALSE;
            rqb_back = FALSE; /* rqb returned to cm - directly: non PERI-AR or not directly: PERI-AR */

            pndv_ar_cm_ar_disconnect_ind( rqb_ptr);

            break;
        }

        case CM_OPC_SV_AR_OWNERSHIP_IND:
        {
            //! indication resource allways go back to cm (inside function)
            free_rqb = FALSE;

            pndv_ar_cm_sv_ownership_ind( rqb_ptr);

            break;
        }

        case CM_OPC_SV_AR_PRM_END_IND:
        {
            free_rqb = FALSE;
            rqb_back = FALSE; /* RQB is returned in the function so that PRM_END.Rsp goes to the CM before a possibly following module Z / S */

            pndv_ar_cm_sv_event_prm_end_ind( rqb_ptr);

            break;
        }

        case CM_OPC_SV_AR_IN_DATA_IND:
        {
            free_rqb = FALSE;
            rqb_back = FALSE;

            pndv_ar_cm_ar_in_data_ind( rqb_ptr);

            break;
        }

        case CM_OPC_SV_AR_RIR_IND:
        {
            free_rqb = FALSE;
            rqb_back = FALSE; /* rqb returned to cm after input update */

            pndv_ar_cm_sv_event_ar_rir_ind( rqb_ptr);

            break;
        }

        case CM_OPC_SV_AR_READ_IND:
        case CM_OPC_SV_AR_WRITE_IND:
        {
            free_rqb = FALSE;
            rqb_back = FALSE;

            pndv_in_cm_read_write_ind(rqb_ptr);

            break;
        }

        case CM_OPC_SV_AR_DATA_STATUS_IND:
        {
            free_rqb = FALSE;
            rqb_back = TRUE;

            pndv_in_cm_sv_event_apdu_status_ind( rqb_ptr);

            break;
        }

        case CM_OPC_SV_AR_PDEVPRM_IND:
        case CM_OPC_SV_AR_PDEVPRM_RSP:
        case CM_OPC_SV_AR_PDEVPRM_READ:
        case CM_OPC_SV_AR_PDEVPRM_WRITE:
        {
            /* what do I do with it? Well let's see if it pops. */
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, PNDV_RQB_GET_OPCODE( rqb_ptr));
            break;
        }

        case CM_OPC_PD_SUBMODULE_CONTROL:
        {
            /* returning plug of a pdev subslot */
            pndv_pp_cm_pd_submodule_add_done(rqb_ptr);
            break;
        }

        case CM_OPC_PD_PTCP_MASTER_CONTROL:
        {
            /* what do I do with it? Well let's see if it pops. */
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, PNDV_RQB_GET_OPCODE( rqb_ptr));
            break;
        }

        case CM_OPC_SV_R1BRIDGE_SEND_PROVIDE:
        case CM_OPC_SV_R1BRIDGE_SEND_IND:
        case CM_OPC_SV_R1BRIDGE_SEND_RSP:
        case CM_OPC_SV_R1BRIDGE_RECEIVED:
        case CM_OPC_SV_R1BRIDGE_CONTROL:
        {
            free_rqb = FALSE;
            rqb_back = FALSE;


#ifdef PNDV_CFG_ENABLE_R1
            rqb_back = pndv_r1_bridge_cm_response(rqb_ptr);
#endif
            break;
        }

        /************************* reserved opcodes **********************/

        /* for a better compatibility all reserved and unused opcodes removed */

        /* ------------------------------------------------------------- */

        default:
        {
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, PNDV_RQB_GET_OPCODE( rqb_ptr));
            break;
        }

    } /* end switch */


    if ( TRUE == free_rqb )
    {
        PNDV_FREE_RQB( &tmp_int, rqb_ptr);

        if ( LSA_OK != tmp_int)
        {
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, tmp_int);
        }
    }

    else if ( TRUE == rqb_back )
    {
        if (  (CM_OPC_PD_LINK_STATUS_INFO == PNDV_RQB_GET_OPCODE( rqb_ptr))
            ||(CM_OPC_PD_SYNC_LED_INFO == PNDV_RQB_GET_OPCODE( rqb_ptr))
            )
        {
            PNDV_RQB_SET_HANDLE( rqb_ptr, pndv_data.cm_handle[PNDV_INDEX_PATH_CMPDSV]);
        }

        else
        {
            PNDV_RQB_SET_HANDLE( rqb_ptr, pndv_data.cm_handle[PNDV_INDEX_PATH_ACP_CM]);
        }

        PNDV_CM_REQUEST( rqb_ptr);
    }
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/**********                                                         **********/
/**********                DPM-Simu INTERFACE FUNCTIONS             **********/
/**********                                                         **********/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/



/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/**********                                                         **********/
/**********                   INTERNAL FUNCTIONS                    **********/
/**********                                                         **********/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/


/**
 *  @brief open channel to cm
 *
 *  @param[in]     channel_idx channel index
 *
 *  internal service function to open a channel to cm
 *  confirmation in pndv_in_cm_open_channel_done()
 *
 */
void pndv_in_cm_open_channel (Unsigned16 channel_idx)
{
    union
    {
        CM_UPPER_RQB_PTR_TYPE rqb;
        LSA_VOID_PTR_TYPE     void_;
    }   ptr;

    PNDV_ALLOC_RQB( &ptr.void_, sizeof(CM_RQB_TYPE));

    if ( pndv_host_ptr_is_nil__(ptr.rqb) )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
    }

    PNDV_ALLOC_MEM( ((LSA_VOID_PTR_TYPE)&ptr.rqb->args.channel.open), sizeof(CM_OPEN_CHANNEL_TYPE) );

    if ( pndv_host_ptr_is_nil__(ptr.rqb->args.channel.open) )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
    }

    ptr.rqb->args.channel.open->handle                    = CM_INVALID_HANDLE;
    ptr.rqb->args.channel.open->handle_upper              = channel_idx;                        /* my handle */
    ptr.rqb->args.channel.open->sys_path                  = pndv_data.sys_path_cm[channel_idx];
    ptr.rqb->args.channel.open->cm_request_upper_done_ptr = pndv_cm_callback;

    pndv_in_write_debug_buffer_all__(PNDV_DC_CM_OPEN_CH,channel_idx);

    PNDV_RQB_SET_HANDLE( ptr.rqb, 0);
    PNDV_RQB_SET_OPCODE( ptr.rqb, CM_OPC_OPEN_CHANNEL);

    PNDV_CM_OPEN_CHANNEL_LOWER( ptr.rqb);
}


/*****************************************************************************/


/**
 *  @brief confirmation to open channel service
 *
 *  @param[in]     rqb_ptr pointer to request block with confirmation information
 *
 *  called by pndv_cm_callback(), confirmation to a open channel request
 *
 */
void pndv_in_cm_open_channel_done (CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    UnsignedOpt response;
    int         tmp_int;

    response = PNDV_RQB_GET_RESPONSE( rqb_ptr);

    pndv_in_write_debug_buffer_all__(PNDV_DC_CM_OPEN_CH_DONE, PNDV_RQB_GET_HANDLE(rqb_ptr));

    if (   (CM_OK           != response)
        && (CM_OK_REPLACED  != response)
       )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, response);
    }

    if ( PNDV_SM_W_OPEN_CHANNELS_DONE == pndv_data.sm )
    {
        pndv_data.cm_handle[PNDV_RQB_GET_HANDLE(rqb_ptr)] = rqb_ptr->args.channel.open->handle;

        PNDV_FREE_MEM( &tmp_int, rqb_ptr->args.channel.open);

        if ( LSA_OK != tmp_int)
        {
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, tmp_int);
        }

        if ( (PNDV_MAX_CHANNEL - 1)== PNDV_RQB_GET_HANDLE(rqb_ptr) )
        {
            /* last open channel is through */

            pndv_sm(PNDV_SM_EVENT_CHANNELS_OPEN_DONE);

        }
        else
        {
            /* open more channels */

            pndv_in_cm_open_channel(PNDV_RQB_GET_HANDLE(rqb_ptr)+1);
        }
    }
    else
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, pndv_data.sm);
    }
}


/*****************************************************************************/


/**
 *  @brief close a channel to cm
 *
 *  @param[in]     channel_idx Index of the channel that should be closed
 *
 *  This functions sends a close request of a channel to cm (see CM_OPC_CLOSE_CHANNEL).
 *
 *
 */
void pndv_in_cm_close_channel (Unsigned16 channel_idx)
{
    union
    {
        CM_UPPER_RQB_PTR_TYPE rqb;
        LSA_VOID_PTR_TYPE     void_;
    }   ptr;

    PNDV_ALLOC_RQB( &ptr.void_, sizeof(CM_RQB_TYPE));

    if ( pndv_host_ptr_is_nil__(ptr.rqb) )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
    }

    pndv_in_write_debug_buffer_all__(PNDV_DC_CM_CLOSE_CH,channel_idx);

    PNDV_RQB_SET_HANDLE( ptr.rqb, pndv_data.cm_handle[channel_idx]);
    PNDV_RQB_SET_OPCODE( ptr.rqb, CM_OPC_CLOSE_CHANNEL);

    PNDV_CM_CLOSE_CHANNEL_LOWER( ptr.rqb);
}




/**
 *  @brief callback to channel close
 *
 *  @param[in]     rqb_ptr Request block of the close channel request
 *
 *  This function handles the callback of a channel close (see CM_OPC_CLOSE_CHANNEL)
 *
 */
void pndv_in_cm_close_channel_done (CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    UnsignedOpt response;

    response = PNDV_RQB_GET_RESPONSE( rqb_ptr);

    pndv_in_write_debug_buffer_all__(PNDV_DC_CM_CLOSE_CH_DONE, PNDV_RQB_GET_HANDLE(rqb_ptr));

    if (   (CM_OK           != response)
        && (CM_OK_REPLACED  != response)
       )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, response);
    }


    switch ( PNDV_RQB_GET_HANDLE(rqb_ptr) )
    {
        case PNDV_INDEX_PATH_ACP_CM:
            pndv_sm(PNDV_SM_EVENT_ACP_CLOSE_CHANNEL_DONE);
            break;
#ifndef PNDV_USE_PSI
        case PNDV_INDEX_PATH_CMPDSV:
            pndv_sm(PNDV_SM_EVENT_PD_SV_CLOSE_CHANNEL_DONE);
            break;
        case PNDV_INDEX_PATH_EDD_CLRPC_CM:
            // last channel which was closed
            pndv_sm(PNDV_SM_EVENT_STOP_DONE);
            break;
#else
        case PNDV_INDEX_PATH_CMPDSV:
            pndv_sm(PNDV_SM_EVENT_STOP_DONE);
            break;
#endif
        default:
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, response);
            break;
    }
}


/*****************************************************************************/


/**
 *  @brief send pd event resources to cm
 *
 *  This function is called during pndv startup after opening the channels.
 *  It is used to send some indication resources (CM_OPC_PD_PROVIDE_EVENT)
 *  to CM_PD
 */
void pndv_in_cm_put_resource (Unsigned32 num_event_resources, Unsigned32 num_alarm_resources)
{
    union
    {
        CM_UPPER_RQB_PTR_TYPE rqb;
        LSA_VOID_PTR_TYPE     void_;
    }   ptr;

    UnsignedOpt           i;

    /* Event-Ressourcen CM-PD */
    /* --------------------------------------------------------------------- */

    for ( i = 0; i < num_event_resources; i++ )
    {
        PNDV_ALLOC_RQB( &ptr.void_, sizeof(CM_RQB_TYPE));

        if ( pndv_host_ptr_is_nil__(ptr.rqb) )
        {
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
        }

        PNDV_ALLOC_MEM( ((LSA_VOID_PTR_TYPE)&ptr.rqb->args.pd.event), sizeof(CM_EVENT_TYPE));

        if ( pndv_host_ptr_is_nil__(ptr.rqb->args.pd.event) )
        {
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
        }

        pndv_in_write_debug_buffer_all__(PNDV_DC_CM_PD_RES_PROV,i);

        PNDV_RQB_SET_HANDLE( ptr.rqb, pndv_data.cm_handle[PNDV_INDEX_PATH_CMPDSV]);
        PNDV_RQB_SET_OPCODE( ptr.rqb, CM_OPC_PD_PROVIDE_EVENT);

        PNDV_CM_REQUEST( ptr.rqb);
    }

    /* PD Alarm resources: we don't need these alarms (port data change notif. / time data changed / sync data change / redundancy / ...)
     * -> don't provide resources */
}


/*****************************************************************************/


/**
 *  @brief callback for provide event resources
 *
 *  @param[in]     rqb_ptr Pointer to request block
 *
 *  This callback handles CM_OPC_PD_PROVIDE_EVENT.
 *  Only triggered when resources should be freed.
 *  "Used" Resources change there opcode to indicate something.
 *
 */
void pndv_in_cm_event_res_done (CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    UnsignedOpt response;
    int         tmp_int;

    response = PNDV_RQB_GET_RESPONSE( rqb_ptr);

    pndv_in_write_debug_buffer_all__(PNDV_DC_CM_RES_BACK,0);

    if ( CM_OK_CANCELLED != response )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, response);
    }

    PNDV_FREE_MEM( &tmp_int, rqb_ptr->args.pd.event);

    if ( LSA_OK != tmp_int)
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, tmp_int);
    }
}

/**
 *  @brief callback for provide alarm resoruces
 *
 *  @param[in]     rqb_ptr Pointer to request block
 *
 *  This callback handles CM_OPC_ALARM_IND_RES_PROVIDE.
 *  Only triggered when resources should be freed.
 *  "Used" Resources change there opcode to indicate something.
 *
 */
void pndv_in_cm_al_res_done (CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    UnsignedOpt response;
    int         tmp_int;

    response = PNDV_RQB_GET_RESPONSE( rqb_ptr);

    pndv_in_write_debug_buffer_al__(PNDV_DC_CM_AL_RES_BACK,0);

    if ( CM_OK_CANCELLED != response )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, response);
    }

    PNDV_FREE_MEM( &tmp_int, rqb_ptr->args.pd.alarm);

    if ( LSA_OK != tmp_int)
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, tmp_int);
    }
}

/**
 *  @brief callback for device provide event resources
 *
 *  @param[in]     rqb_ptr Pointer to request block
 *
 *  This callback handles CM_OPC_SV_DEVICE_PROVIDE_EVENT.
 *  Only triggered when resources should be freed.
 *  "Used" Resources change there opcode to indicate something.
 *
 */
void pndv_in_cm_sv_device_provide_event_done(CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    UnsignedOpt response;
    int         tmp_int;

    pndv_in_write_debug_buffer_all__(PNDV_DC_CM_SV_DEVICE_PROVIDE_EVENT_DONE, PNDV_RQB_GET_HANDLE(rqb_ptr));

    response = PNDV_RQB_GET_RESPONSE( rqb_ptr);


    if ( CM_OK_CANCELLED != response )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, response);
    }

    PNDV_FREE_MEM( &tmp_int, rqb_ptr->args.sv.dev_provide_event);

    if ( LSA_OK != tmp_int)
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, tmp_int);
    }
}

/**
 *  @brief callback for device provide alarm resources
 *
 *  @param[in]     rqb_ptr Pointer to request block
 *
 *  This callback handles CM_OPC_SV_DEVICE_PROVIDE_ALARM.
 *  Only triggered when resources should be freed.
 *  "Used" Resources change there opcode to indicate something.
 *
 */
void pndv_in_cm_sv_device_provide_alarm_done(CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    UnsignedOpt response;
    int         tmp_int;

    pndv_in_write_debug_buffer_all__(PNDV_DC_CM_SV_DEVICE_PROVIDE_ALARM_DONE, PNDV_RQB_GET_HANDLE(rqb_ptr));

    response = PNDV_RQB_GET_RESPONSE( rqb_ptr);


    if ( CM_OK_CANCELLED != response )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, response);
    }

    PNDV_FREE_MEM( &tmp_int, rqb_ptr->args.sv.dev_provide_alarm);

    if ( LSA_OK != tmp_int)
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, tmp_int);
    }
}


/*****************************************************************************/


/**
 *  @brief callback for link status info
 *
 *  @param[in]  rqb_ptr Pointer to request block
 *
 *  This callback handles CM_OPC_PD_LINK_STATUS_INFO.
 *
 */
Boolean pndv_in_cm_link_status_info (CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    UnsignedOpt response,
                port,
                tmp_port_cnt;
    Boolean     rqb_back;
    int         tmp_int;

    tmp_port_cnt = 0;
    rqb_back = TRUE;

    response = PNDV_RQB_GET_RESPONSE( rqb_ptr);

    if ( CM_OK_CANCELLED == response)
    {
        rqb_back = FALSE; /* Channel was closed RQB kept and released in pndv_cm_callback */
    }
    else
    {
        if (   (CM_OK           != response)
            && (CM_OK_REPLACED  != response)
           )
        {
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, response);
        }

    }

    pndv_in_write_debug_buffer_all_add__(PNDV_DC_CM_LINK_STATUS_IND,
                                         rqb_ptr->args.pd.link_status->status,
                                         (((Unsigned32)rqb_ptr->args.pd.link_status->port_id)     |
                                          ((Unsigned32)rqb_ptr->args.pd.link_status->speed) << 16 |
                                          ((Unsigned32)rqb_ptr->args.pd.link_status->mode)  << 24)
                                        );

/*
    LSA_UINT16 interface_id; / * out(!), see channel-info * /
    LSA_UINT16 port_id;      / * CM_PORT_ID_AUTO or 1..n (see channel-info) * /
    LSA_UINT8  status;       / * see cm_link_status_enum, out: never "unknown" * /

                CM_LINK_STATUS_UNKNOWN     = ACP_LINK_STATUS_UNKNOWN,
                CM_LINK_STATUS_UP          = ACP_LINK_STATUS_UP,
                CM_LINK_STATUS_DOWN        = ACP_LINK_STATUS_DOWN,
                CM_LINK_STATUS_UP_CLOSED   = ACP_LINK_STATUS_UP_CLOSED, / * same as "down" for normal communication *
                CM_LINK_STATUS_UP_DISABLED = ACP_LINK_STATUS_UP_DISABLED / * same as "down" for normal communication * /

    LSA_UINT8  speed;        / * see cm_link_speed_enum, out: valid if "up" * /

                CM_LINK_SPEED_UNKNOWN = ACP_LINK_SPEED_UNKNOWN,
                CM_LINK_SPEED_10_M    = ACP_LINK_SPEED_10_M,
                CM_LINK_SPEED_100_M   = ACP_LINK_SPEED_100_M,
                CM_LINK_SPEED_1_G     = ACP_LINK_SPEED_1_G,
                CM_LINK_SPEED_10_G    = ACP_LINK_SPEED_10_G

    LSA_UINT8  mode;         / * see cm_link_mode_enum, out: valid if "up" * /

                CM_LINK_MODE_UNKNOWN     = ACP_LINK_MODE_UNKNOWN,
                CM_LINK_MODE_HALF_DUPLEX = ACP_LINK_MODE_HALF_DUPLEX,
                CM_LINK_MODE_FULL_DUPLEX = ACP_LINK_MODE_FULL_DUPLEX

*/

    /* Save PortLinkStatus in pndv_data */

    pndv_data.port_link_status[(rqb_ptr->args.pd.link_status->port_id)-1] = rqb_ptr->args.pd.link_status->status;

    /* Test all link states on! = CM_LINK_STATUS_UP -> BF_LED control */

    for (port = 0; port < PNDV_MAX_PORT; port++)
    {
        if ( CM_LINK_STATUS_UP != pndv_data.port_link_status[port] )
        {
            tmp_port_cnt++;
        }
    }

    /* BF-LED 0.5 Hz flashing if at least 1 port UP */
    /* BF-LED ON if no port UP */

    if ( PNDV_MAX_PORT == tmp_port_cnt )
    {
        PNDV_LED_CTRL_BUS_FAULT();

    }
    else
    {
        PNDV_CFG_MEASUREMENT_LINK_UP();

        PNDV_LED_CTRL_BUS_OK();
    }

    if ( FALSE == rqb_back )
    {
        /* Channel was closed RQB will release in pndv_cm_callback 
           -> also release argument memory
        */

        PNDV_FREE_MEM( &tmp_int, rqb_ptr->args.pd.link_status);

        if ( LSA_OK != tmp_int)
        {
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, tmp_int);
        }
    }

    return(rqb_back);
}

void pndv_in_cm_provide_led_info_rqb( void )
{
    CM_UPPER_RQB_PTR_TYPE tmp_rqb_ptr;

    pndv_in_write_debug_buffer_all__(PNDV_DC_CM_LED_INFO_REQ, 0);

    /*********************** device led info ********************************/

    tmp_rqb_ptr = &pndv_data.rqb.device_led_info;

    if ( 0 != PNDV_RQB_GET_OPCODE( tmp_rqb_ptr) )
    {
        /* RQB in use */

        pndv_in_fatal_error( PNDV_MODULE, __LINE__, PNDV_RQB_GET_OPCODE( tmp_rqb_ptr));
    }

    tmp_rqb_ptr->args.sv.dev_led_info->device_nr  = PNDV_CM_DEVICE_NO; /* device number */
    tmp_rqb_ptr->args.sv.dev_led_info->led_info   = 0;      /* bit set = some submodules have diagnosis information */
    tmp_rqb_ptr->args.sv.dev_led_info->maint_info = 0;      /* bit set = some submodules have maintenance or qualified information */
    /*
     * bit  0 = Maintenance-Required
     * bit  1 = Maintenance-Demanded
     * bit  2 = Qualifier_2
     * ...
     * bit 31 = Qualifier_31
     *
     * NOTE: see PNIO-Spec "Values for MaintenanceStatus"
     */

    PNDV_RQB_SET_HANDLE( tmp_rqb_ptr, pndv_data.cm_handle[PNDV_INDEX_PATH_ACP_CM]);
    PNDV_RQB_SET_OPCODE( tmp_rqb_ptr, CM_OPC_SV_DEVICE_LED_INFO);

    PNDV_CM_REQUEST( tmp_rqb_ptr);

    /*********************** sync led info **********************************/
    tmp_rqb_ptr = &pndv_data.rqb.sync_led_info;

    if ( 0 != PNDV_RQB_GET_OPCODE( tmp_rqb_ptr) )
    {
        /* RQB in use */

        pndv_in_fatal_error( PNDV_MODULE, __LINE__, PNDV_RQB_GET_OPCODE( tmp_rqb_ptr));
    }

    tmp_rqb_ptr->args.pd.sync_led_info->sync_led_info   = 0;
    tmp_rqb_ptr->args.pd.sync_led_info->edd_port_id     = 0; // interface sync
    tmp_rqb_ptr->args.pd.sync_led_info->is_rate_valid   = LSA_FALSE;
    tmp_rqb_ptr->args.pd.sync_led_info->local_prio      = 0;
    tmp_rqb_ptr->args.pd.sync_led_info->pll_state       = CM_PD_PLL_STATE_UNKNOWN;
    tmp_rqb_ptr->args.pd.sync_led_info->rcv_sync_prio   = 0;

    PNDV_RQB_SET_HANDLE( tmp_rqb_ptr, pndv_data.cm_handle[PNDV_INDEX_PATH_CMPDSV]);
    PNDV_RQB_SET_OPCODE( tmp_rqb_ptr, CM_OPC_PD_SYNC_LED_INFO);

    PNDV_CM_REQUEST( tmp_rqb_ptr);
}


/**
 *  @brief callback for device led info
 *
 *  @param[in]  rqb_ptr Pointer to request block
 *
 *  This callback handles CM_OPC_SV_DEVICE_LED_INFO.
 *
 */
Boolean pndv_in_cm_dev_led_info (CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    UnsignedOpt response;
    Boolean     rqb_back;

    rqb_back = FALSE;

    response = PNDV_RQB_GET_RESPONSE( rqb_ptr);

    pndv_in_write_debug_buffer_all_add__(PNDV_DC_CM_DEV_LED_INFO_IND,
                                         rqb_ptr->args.sv.dev_led_info->led_info,
                                         rqb_ptr->args.sv.dev_led_info->maint_info);

    if ( CM_OK == response)
    {
        /* save state */
        pndv_glob_data.led_maint_info   = rqb_ptr->args.sv.dev_led_info->maint_info;
        pndv_glob_data.led_info         = rqb_ptr->args.sv.dev_led_info->led_info;

        /* Control SF LED */
        pndv_in_check_led_info();

        /* Control maintenance LED */
        pndv_check_maint_info();

        /* RQB is returned in pndv_cm_callback */

        rqb_back = TRUE;
    }
    else
    {
        if ( CM_OK_CANCELLED != response)
        {
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, response);
        }

        PNDV_RQB_SET_OPCODE( &pndv_data.rqb.device_led_info, 0);

        /* else: with CM_OK_CANCELLED there is no device -> keep RQB and mark as free 
           rqb_back = FALSE has already been set above*/
    }

    return( rqb_back);
}

/**
 *  @brief callback for sync led info
 *
 *  @param[in]  rqb_ptr Pointer to request block
 *
 *  This callback handles CM_OPC_PD_SYNC_LED_INFO.
 *
 */
Boolean pndv_in_cm_sync_led_info (CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    UnsignedOpt response;
    Boolean     rqb_back;
    UnsignedOpt port_nr;

    rqb_back = FALSE;

    response = PNDV_RQB_GET_RESPONSE( rqb_ptr);

    pndv_in_write_debug_buffer_all_add__(PNDV_DC_CM_SYNC_LED_INFO_IND,
                                         rqb_ptr->args.pd.sync_led_info->sync_led_info,
                                         rqb_ptr->args.pd.sync_led_info->edd_port_id);

    port_nr = rqb_ptr->args.pd.sync_led_info->edd_port_id;

    if ( port_nr != 0)
    {
        // not supported
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, port_nr);
    }

    if ( CM_OK == response)
    {
        pndv_glob_data.sync_led_info = rqb_ptr->args.pd.sync_led_info->sync_led_info;

        pndv_in_check_sync_info();

        rqb_back = TRUE;
    }
    else
    {
        if ( CM_OK_CANCELLED != response)
        {
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, response);
        }

        PNDV_RQB_SET_OPCODE( &pndv_data.rqb.sync_led_info, 0);

    }

    return( rqb_back);
}


/**
 *  @brief create the cmpd server
 *
 *  setup and create the cm physical device server CM_OPC_PD_CREATE.
 *
 */
void pndv_in_cm_pdsv_create (void)
{
    UnsignedOpt port_count;

    union
    {
        CM_UPPER_RQB_PTR_TYPE rqb;
        LSA_VOID_PTR_TYPE     void_;
    }   ptr;

    PNDV_ALLOC_RQB( &ptr.void_, sizeof(CM_RQB_TYPE));

    if ( pndv_host_ptr_is_nil__(ptr.rqb) )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
    }

    PNDV_ALLOC_MEM( ((LSA_VOID_PTR_TYPE)&ptr.rqb->args.pd.create),( sizeof(CM_PD_CREATE_TYPE) + ( pndv_data.cfg.pd.port_count_used * sizeof(CM_PD_MAP_TYPE)))); /* NOTE: the cm-user must alloc additional memory for the ports */

    if ( pndv_host_ptr_is_nil__(ptr.rqb->args.pd.create) )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
    }

    ptr.rqb->args.pd.create->max_alarm_data_length          = CM_ALARM_DATA_LENGTH_MIN;
    ptr.rqb->args.pd.create->map_count                      = pndv_data.cfg.pd.port_count_used + 1;  /* Number of Ports + Interface */
    ptr.rqb->args.pd.create->oem_data                         = (CM_UPPER_OEM_DATA_PTR_TYPE) pndv_data.oem_data_ptr;

    PNDV_COPY_BYTE( &ptr.rqb->args.pd.create->annotation[0],
                    pndv_data.annotation_ptr,
                    CLRPC_MAX_ANNOTATION_SIZE);
    for (port_count = 0; port_count < pndv_data.cfg.pd.port_count_used + 1/*interface*/; port_count++)
    {
        ptr.rqb->args.pd.create->map[port_count].slot_nr                    = PNDV_IM_SLOT_NO;
        ptr.rqb->args.pd.create->map[port_count].mod_ident                  = pndv_data.cfg.pd.im_mod_ident;
        ptr.rqb->args.pd.create->map[port_count].subslot_nr                 = pndv_data.cfg.pd.interface_subslot_nr + port_count;
        ptr.rqb->args.pd.create->map[port_count].sub_ident                  = pndv_data.cfg.pd.port[port_count].submod_id;
        ptr.rqb->args.pd.create->map[port_count].im0_bits                   = pndv_data.cfg.pd.port[port_count].im_0_bits;

        if (port_count == 0)
        {
            // special case interface, must be COMPACT
            ptr.rqb->args.pd.create->map[port_count].pdev_properties        = CM_PDEV_PROP_TYPE_COMPACT|CM_PDEV_PROP_STATE_PLUGGED;
        }
        else
        {
#if (PNDV_CFG_PORT_IS_MODULAR == 1)
            ptr.rqb->args.pd.create->map[port_count].pdev_properties        = CM_PDEV_PROP_TYPE_MODULAR| (PNDV_CFG_IS_PDEV_PORT_PLUGGED(port_count) ? CM_PDEV_PROP_STATE_PLUGGED : CM_PDEV_PROP_STATE_PULLED);
#else
            ptr.rqb->args.pd.create->map[port_count].pdev_properties        = CM_PDEV_PROP_TYPE_COMPACT|CM_PDEV_PROP_STATE_PLUGGED;
#endif
        }
    }

    pndv_in_write_debug_buffer_all__(PNDV_DC_CM_PDSV_CREATE,0);

    PNDV_RQB_SET_HANDLE( ptr.rqb, pndv_data.cm_handle[PNDV_INDEX_PATH_CMPDSV]);
    PNDV_RQB_SET_OPCODE( ptr.rqb, CM_OPC_PD_CREATE);

    PNDV_CM_REQUEST( ptr.rqb);
}


/**
 *  @brief callback to created cmpd server
 *
 *  @param[in]  rqb_ptr Pointer to request block
 *
 *  This callback handles CM_OPC_PD_CREATE.
 *
 */
void pndv_in_cm_pdsv_create_done (CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    union
    {
        CM_UPPER_RQB_PTR_TYPE rqb;
        LSA_VOID_PTR_TYPE     void_;
    }   ptr;


    UnsignedOpt response, i;
    int         tmp_int;

    response = PNDV_RQB_GET_RESPONSE( rqb_ptr);

    if ( CM_OK != response )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, response);
    }

    // provide the requested number of PD event resources to CM.
    // They are used for CM_OPC_PD_EVENT_APPL_READY_IND signaling, later on ...
    pndv_in_cm_put_resource(rqb_ptr->args.pd.create->nr_of_event_resources , rqb_ptr->args.pd.create->nr_of_alarm_resources);

    PNDV_FREE_MEM( &tmp_int, rqb_ptr->args.pd.create);

    if ( LSA_OK != tmp_int)
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, tmp_int);
    }

    /* link_status ressource */
    /* --------------------------------------------------------------------- */

    for ( i = 0; i < PNDV_MAX_PORT ; i++ )
    {
        PNDV_ALLOC_RQB( &ptr.void_, sizeof(CM_RQB_TYPE));

        if ( pndv_host_ptr_is_nil__(ptr.rqb) )
        {
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
        }

        PNDV_ALLOC_MEM( ((LSA_VOID_PTR_TYPE)&ptr.rqb->args.pd.link_status), sizeof(CM_PD_LINK_STATUS_INFO_TYPE));

        if ( pndv_host_ptr_is_nil__(ptr.rqb->args.pd.link_status) )
        {
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
        }

      /*ptr.rqb->args.pd.link_status->interface_id  dont care*/
        ptr.rqb->args.pd.link_status->port_id      = i+1;
        ptr.rqb->args.pd.link_status->status       = CM_LINK_STATUS_UNKNOWN;
        ptr.rqb->args.pd.link_status->speed        = CM_LINK_SPEED_UNKNOWN;
        ptr.rqb->args.pd.link_status->mode         = CM_LINK_MODE_UNKNOWN;
#ifndef PNDV_CFG_LINK_STATE_WITHOUT_AUTONEG
        ptr.rqb->args.pd.link_status->autoneg      = CM_LINK_AUTONEG_UNKNOWN;
#endif

        pndv_in_write_debug_buffer_al__(PNDV_DC_CM_LINK_STATUS_RES_PROV,i+1);

        PNDV_RQB_SET_HANDLE( ptr.rqb, pndv_data.cm_handle[PNDV_INDEX_PATH_CMPDSV]);
        PNDV_RQB_SET_OPCODE( ptr.rqb, CM_OPC_PD_LINK_STATUS_INFO);

        PNDV_CM_REQUEST( ptr.rqb);
    }

    pndv_sm(PNDV_SM_EVENT_CREATE_PD_SERVER_DONE);

}

/**
 *  @brief delete the cmpd server
 *
 *  delete the cm physical device server CM_OPC_PD_DELETE.
 *
 */
void pndv_in_cm_pdsv_delete (void)
{
    union
    {
        CM_UPPER_RQB_PTR_TYPE rqb;
        LSA_VOID_PTR_TYPE     void_;
    }   ptr;

    PNDV_ALLOC_RQB( &ptr.void_, sizeof(CM_RQB_TYPE));

    if ( pndv_host_ptr_is_nil__(ptr.rqb) )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
    }

    pndv_in_write_debug_buffer_all__(PNDV_DC_CM_PDSV_DELETE,0);

    PNDV_RQB_SET_HANDLE( ptr.rqb, pndv_data.cm_handle[PNDV_INDEX_PATH_CMPDSV]);
    PNDV_RQB_SET_OPCODE( ptr.rqb, CM_OPC_PD_DELETE);

    PNDV_CM_REQUEST( ptr.rqb);
}

/**
 *  @brief callback to delete cmpd server
 *
 *  @param[in]  rqb_ptr Pointer to request block
 *
 *  This callback handles CM_OPC_PD_DELETE.
 *
 *
 */
void pndv_in_cm_pdsv_delete_done (CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    UnsignedOpt response;

    response = PNDV_RQB_GET_RESPONSE( rqb_ptr);

    if ( CM_OK != response )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, response);
    }

    pndv_sm(PNDV_SM_EVENT_DELETE_PD_SERVER_DONE);

}



/**
 *  @brief create the cm server
 *
 *  setup and create the cm server CM_OPC_SV_CREATE.
 *
 */
void pndv_in_cm_sv_create (void)
{
    union
    {
        CM_UPPER_RQB_PTR_TYPE rqb;
        LSA_VOID_PTR_TYPE     void_;
    }   ptr;

    PNDV_ALLOC_RQB( &ptr.void_, sizeof(CM_RQB_TYPE));

    if ( pndv_host_ptr_is_nil__(ptr.rqb) )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
    }

    PNDV_ALLOC_MEM( ((LSA_VOID_PTR_TYPE)&ptr.rqb->args.sv.create), sizeof(CM_SV_CREATE_TYPE));

    if ( pndv_host_ptr_is_nil__(ptr.rqb->args.sv.create) )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
    }


    /* application must respond within this number of seconds */
    /* or call will be cancelled, 0=default */
    ptr.rqb->args.sv.create->enable_AlarmResponder               =  LSA_FALSE;  /* alle Alarme werden durch CM automatisch quittiert */
    /* LSA_FALSE ... all received alarms will be acknowledged internally with CM_PNIO_ERR_AlarmTypeNotSupported (AP01035514) */
    /* LSA_TRUE .... all received alarms will be indicated to the cm-user (who will acknowledge them) */
    ptr.rqb->args.sv.create->can_RecordDataReadQuery    = LSA_FALSE;
    /* LSA_FALSE ... reject a record-read with a RecordDataReadQuery-block */
    /* LSA_TRUE .... accept a record-read with a RecordDataReadQuery-block */
    /*               and the cm-user will handle it correctly */
    ptr.rqb->args.sv.create->can_ARFSUDataAdjust        = PNDV_HANDLE_ARFSU_DATA_ADJUST;
    /* LSA_FALSE ... the ARFSUDataAdjust record is handled internally */
    /* LSA_TRUE .... the ARFSUDataAdjust record will be indicated to the cm-user */
    /*               and the cm-user will handle it correctly */
    ptr.rqb->args.sv.create->enable_DataStatusIndication  = LSA_TRUE;
    /* LSA_FALSE ... CM_OPC_SV_AR_DATA_STATUS_IND is not indicated to the cm-user */
    /* LSA_TRUE .... CM_OPC_SV_AR_DATA_STATUS_IND is indicated to the cm-user */
    ptr.rqb->args.sv.create->IOXS_Required = PNDV_NO_DISCARD_IOXS_SUPPORT; /* corresponds to the GSDML key "IOXS_Required" (V6.0i2.1x, TIA 1218229) */
    /* LSA_FALSE ... SubmoduleProperties.DiscardIOXS is supported and the cm-user will handle it correctly */
    /* LSA_TRUE .... SubmoduleProperties.DiscardIOXS is not supported */


    pndv_in_write_debug_buffer_all__(PNDV_DC_CM_SV_CREATE,0);

    PNDV_RQB_SET_HANDLE( ptr.rqb, pndv_data.cm_handle[PNDV_INDEX_PATH_ACP_CM]);
    PNDV_RQB_SET_OPCODE( ptr.rqb, CM_OPC_SV_CREATE);

    PNDV_CM_REQUEST( ptr.rqb);
}


/**
 *  @brief callback to created cm server
 *
 *  @param[in]  rqb_ptr Pointer to request block
 *
 *  This callback handles CM_OPC_SV_CREATE.
 *
 */
void pndv_in_cm_sv_create_done (CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    UnsignedOpt response;
    int         tmp_int;

    response = PNDV_RQB_GET_RESPONSE( rqb_ptr);

    if ( CM_OK != response )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, response);
    }

    PNDV_FREE_MEM( &tmp_int, rqb_ptr->args.sv.create);

    if ( LSA_OK != tmp_int)
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, tmp_int);
    }

    pndv_sm(PNDV_SM_EVENT_CREATE_CM_SERVER_DONE);

}


/**
 *  @brief delete the cm server
 *
 *  delete the cm server CM_OPC_SV_DELETE.
 *
 */
void pndv_in_cm_sv_delete (void)
{
    union
    {
        CM_UPPER_RQB_PTR_TYPE rqb;
        LSA_VOID_PTR_TYPE     void_;
    }   ptr;

    PNDV_ALLOC_RQB( &ptr.void_, sizeof(CM_RQB_TYPE));

    if ( pndv_host_ptr_is_nil__(ptr.rqb) )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
    }

    pndv_in_write_debug_buffer_all__(PNDV_DC_CM_SV_DELETE,0);

    PNDV_RQB_SET_HANDLE( ptr.rqb, pndv_data.cm_handle[PNDV_INDEX_PATH_ACP_CM]);
    PNDV_RQB_SET_OPCODE( ptr.rqb, CM_OPC_SV_DELETE);

    PNDV_CM_REQUEST( ptr.rqb);
}


/**
 *  @brief callback to delete cmpd server
 *
 *  @param[in]  rqb_ptr Pointer to request block
 *
 *  This callback handles CM_OPC_SV_DELETE.
 *
 *
 */
void pndv_in_cm_sv_delete_done (CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    UnsignedOpt response;

    response = PNDV_RQB_GET_RESPONSE( rqb_ptr);

    if ( CM_OK != response )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, response);
    }

    pndv_sm(PNDV_SM_EVENT_CM_SV_DELETE_DONE);

}

/**
 *  @brief callback for control requests
 *
 *  @param[in]  rqb_ptr Pointer to request block
 *
 *  Callback for control requests CM_OPC_SV_CONTROL.
 *  Service not used atm.
 *
 */
void pndv_in_cm_sv_control_done (CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    UnsignedOpt response;

    response = PNDV_RQB_GET_RESPONSE( rqb_ptr);

    pndv_in_write_debug_buffer_all__(PNDV_DC_CM_CONTROL_STATION_DONE, 0);

    if ( CM_OK != response )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, response);
    }
}

/**
 *  @brief check if requests are pending
 *
 *
 *  Check if there are some pending request.
 *
 */
void pndv_in_cm_check_dev_req (void)
{
    if (   (0 != pndv_data.diag.cm_dev_req.pending )
        && (0 == pndv_data.diag.cm_dev_req_running )
       )
    {
        /* Acceleration query
           -> is there anything to do and
           -> may I now (nothing is currently running)
        */

        if ( PNDV_CM_DEV_REQ_DEVICE_ADD & pndv_data.diag.cm_dev_req.pending )
        {
            pndv_data.diag.cm_dev_req.pending &= ~PNDV_CM_DEV_REQ_DEVICE_ADD;

            pndv_in_cm_sv_device_add();
        }

        if ( PNDV_CM_DEV_REQ_DEVICE_RMV & pndv_data.diag.cm_dev_req.pending )
        {
            pndv_data.diag.cm_dev_req.pending &= ~PNDV_CM_DEV_REQ_DEVICE_RMV;

            pndv_in_cm_sv_device_remove();
        }

        if ( PNDV_CM_DEV_REQ_DEVICE_CONTROL & pndv_data.diag.cm_dev_req.pending )
        {
            pndv_data.diag.cm_dev_req.pending &= ~PNDV_CM_DEV_REQ_DEVICE_CONTROL;

            pndv_in_device_control(pndv_data.device_control);
        }

        /* not used
        if ( PNDV_CM_DEV_REQ_SUBMODULE_RMV & pndv_data.diag.cm_dev_req.pending )
        {
            pndv_data.diag.cm_dev_req.pending &= ~PNDV_CM_DEV_REQ_SUBMODULE_RMV;

        }
        */
    }
}


/**
 *  @brief register the device to cm
 *
 *
 *  setup and add the device to cm CM_OPC_SV_DEVICE_ADD.
 *
 */
void pndv_in_cm_sv_device_add (void)
{
    union
    {
        CM_UPPER_RQB_PTR_TYPE rqb;
        LSA_VOID_PTR_TYPE     void_;
    }   ptr;

    if ( 0 == pndv_data.diag.cm_dev_req_running )
    {
        PNDV_ALLOC_RQB( &ptr.void_, sizeof(CM_RQB_TYPE));

        if ( pndv_host_ptr_is_nil__(ptr.rqb) )
        {
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
        }

        PNDV_ALLOC_MEM( ((LSA_VOID_PTR_TYPE)&ptr.rqb->args.sv.dev_add), sizeof(CM_SV_DEVICE_ADD_TYPE));

        if ( pndv_host_ptr_is_nil__(ptr.rqb->args.sv.dev_add) )
        {
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
        }

#ifdef PNDV_CM_FILL_DEVICE_ADD_TYPE
        /* if you encounter errors in the part below, because of an older PNIO-stack, you can use this
         * define to fill out the structure yourself.
         */
        PNDV_CM_FILL_DEVICE_ADD_TYPE(ptr.rqb->args.sv.dev_add);
#else
        ptr.rqb->args.sv.dev_add->device_nr = PNDV_CM_DEVICE_NO;              /* device handle 1..max_devices, see "create server" */


        ptr.rqb->args.sv.dev_add->device_id                     = pndv_data.cfg.device_id;      /* see construction of PN IO object UUID */
        ptr.rqb->args.sv.dev_add->vendor_id                     = PNDV_CM_VENDOR_ID;            /* see construction of PN IO object UUID */
        ptr.rqb->args.sv.dev_add->instance                      = PNDV_CM_DEVICE_INSTANZ;       /* see construction of PN IO object UUID */
        PNDV_COPY_BYTE( &ptr.rqb->args.sv.dev_add->annotation[0],
                        pndv_data.annotation_ptr,
                        CLRPC_MAX_ANNOTATION_SIZE);

        #ifdef PNDV_CFG_ENABLE_R1
        if( pndv_r1_bridge_enable_bridge() )
        {
            ptr.rqb->args.sv.dev_add->max_ars_IOC                = PNDV_MAX_ARS_RT_R1;              /* see pndv_cfg.h */
        }
        else
        #endif
        {
            ptr.rqb->args.sv.dev_add->max_ars_IOC                = PNDV_MAX_ARS_RT;              /* see pndv_cfg.h */
        }
        ptr.rqb->args.sv.dev_add->max_ars_IOS                    = 0;
        ptr.rqb->args.sv.dev_add->max_ars_DAC                    = PNDV_MAX_ARS_DA;              /* see pndv_cfg.h */
            /*
             * max_ars_RT + max_ars_DA >= 1
             * max_ars_RT + max_ars_DA <= CM_CFG_MAX_SV_ARS (see cm_cfg.h)
             *
             * "RT" means ARs with IOCRProperties.RTClass == 1/2/3/UDP
             * "DA" means ARs with ARProperties.DeviceAccess == 1 (ARs without IOCRs)
             *
             * the "implicit AR" exists always (thus the name).
             */
        ptr.rqb->args.sv.dev_add->max_slot_nr                   = PNDV_CM_SV_MAX_SLOT_NR;            /* 1..CM_CFG_MAX_SV_SLOTS, see cm_cfg.h */
        ptr.rqb->args.sv.dev_add->max_subslot_nr                = PNDV_CM_SV_MAX_SUBSLOT_NR;
        ptr.rqb->args.sv.dev_add->max_nr_of_subslots            = PNDV_CM_SV_SUBSLOT_COUNT;
            /*
             * the submodules of an AR (expected config) are checked against these bounds
             * if slot_nr > max_slot_nr then the AR is rejected
             * if subslot_nr > max_subslot_nr then the AR is rejected
             * if number of submodules > max_nr_of_subslots then the AR is rejected
             * note that there is no "max_api"
             * memory usage is thus bounded by O(max_nr_of_subslots * max_ars_RT)
             */
        ptr.rqb->args.sv.dev_add->alarm_send_max_length         = PNDV_ALARM_SEND_MAX_LENGTH; /* size of the biggest alarm sent (V5.2i1, AP01232054) */
            /*
             * usecase: the user specifies the size of the biggest alarm he will ever send (CM_OPC_SV_AR_ALARM_SEND)
             * a value less than 172 (= CM_ALARM_DATA_LENGTH_MIN - CM_ALARM_OFFSET_DATA) will be rounded up
             *
             * req: 0..1404 (= CM_ALARM_DATA_LENGTH_MAX - CM_ALARM_OFFSET_DATA)
             * cnf: effective value (different from .req in case of rounding up)
             *
             * if alarm_send_max_length > CM_SV_AR_CONNECT_TYPE::alarm_send_max_length then the AR is rejected
             */
        ptr.rqb->args.sv.dev_add->contains_pdev                 = LSA_TRUE;
            /*
             * LSA_FALSE ... 0x8ipp submodules cannot be added.
             * LSA_TRUE .... 0x8ipp submodules can be added (this device is the pdev-device, AP00995373)
             */
        ptr.rqb->args.sv.dev_add->central_pdev                  = LSA_FALSE;
            /*
             * LSA_FALSE ... the PDEV can be owned and parameterized by an AR (if the device "contains_pdev").
             * LSA_TRUE .... the PDEV cannot be owned by an AR and is parameterized via CM-PD only (AP01066848).
             */

        ptr.rqb->args.sv.dev_add->parking_mode                  = LSA_TRUE;

#ifdef PNDV_CFG_ENABLE_R1
        if( pndv_r1_bridge_enable_bridge() )
        {
            ptr.rqb->args.sv.dev_add->parking_mode                  = LSA_FALSE;
        }
#endif
            /*
             * for FSU (fast startup) devices only (AP00654135, AP00975630)
             * the use-case is described in the documentation
             *
             * LSA_FALSE ... normal mode
             * LSA_TRUE .... parking mode
             */
        ptr.rqb->args.sv.dev_add->write_multiple_parallelism    = 1;
            /*
             * number of parallel CM_OPC_SV_AR_WRITE_IND per AR (AP00786836, CM V4.2i2.1)
             * this parameter is effective for "real time ARs" only (not for "device access ARs")
             * 0 ... reserved
             * 1 ... one write-indication at a time (compatibility mode)
             *       the cm-user sees no difference between a write-single and a write-multiple
             * n ... at most n write-indications at a time
             *       the cm-user must be able to handle the parallel indications
             *       the write-indications can be responded out-of-order
             */
        if( PNDV_MAX_ARS_RT <= 1)
        {
            ptr.rqb->args.sv.dev_add->SRD_enabled = LSA_FALSE;
            ptr.rqb->args.sv.dev_add->R1_enabled = LSA_FALSE; //R1 review + open: HIF new parameters, pcIOX sdb2ini / asom2ini, CM documentation (also dependencies on other things)
            ptr.rqb->args.sv.dev_add->R1_bridge_max = 0;
            ptr.rqb->args.sv.dev_add->CIR_enabled = LSA_FALSE;
            ptr.rqb->args.sv.dev_add->gsd_SharedDeviceSupported = LSA_FALSE; /* see GSDML/SharedDeviceSupported (TIA 1587591) */
        }
        else
        {
#ifndef PNDV_CFG_ENABLE_S2
            // workaround: no RTC3 support if SRD_enabled
            ptr.rqb->args.sv.dev_add->SRD_enabled = LSA_FALSE;
            ptr.rqb->args.sv.dev_add->R1_enabled = LSA_FALSE; //R1 review + open: HIF new parameters, pcIOX sdb2ini / asom2ini, CM documentation (also dependencies on other things)
            ptr.rqb->args.sv.dev_add->R1_bridge_max = 0;
            ptr.rqb->args.sv.dev_add->CIR_enabled = LSA_FALSE;
            ptr.rqb->args.sv.dev_add->gsd_SharedDeviceSupported = LSA_TRUE; /* see GSDML/SharedDeviceSupported (TIA 1587591) */
#else
            ptr.rqb->args.sv.dev_add->SRD_enabled = LSA_TRUE;
            ptr.rqb->args.sv.dev_add->R1_enabled = LSA_FALSE; //R1 review + open: HIF new parameters, pcIOX sdb2ini / asom2ini, CM documentation (also dependencies on other things)
            ptr.rqb->args.sv.dev_add->R1_bridge_max = 0;
            ptr.rqb->args.sv.dev_add->CIR_enabled = LSA_FALSE;// default = FALSE - see below
            ptr.rqb->args.sv.dev_add->gsd_SharedDeviceSupported = LSA_TRUE; /* see GSDML/SharedDeviceSupported (TIA 1587591) */

            #ifdef PNDV_CFG_ENABLE_R1
            if( pndv_r1_bridge_enable_bridge() )
            {
                ptr.rqb->args.sv.dev_add->R1_enabled = LSA_TRUE; //R1 review + open: HIF new parameters, pcIOX sdb2ini / asom2ini, CM documentation (also dependencies on other things)
                ptr.rqb->args.sv.dev_add->R1_bridge_max = PNDV_R1_BRIDGE_PPPN_DATA_SIZE_CM_DATA;
                  /*
                   * R1_enabled
                   *   LSA_FALSE ... normal mode
                   *   LSA_TRUE .... functionality "R1" enabled
                   *   R1 can be enabled only if SRD is enabled
                   *
                   * R1_bridge_max ... maximum "transport unit" of the R1-Bridge
                   *   set to 0 if R1_enabled == LSA_FALSE
                   *   set to 0x0000FF80 for MaxSupportedRecordSize == 0xFF00 (or less)
                   *   set to 0x00010080 for MaxSupportedRecordSize == 0xFFFF (or less)
                   */
                ptr.rqb->args.sv.dev_add->gsd_SharedDeviceSupported = LSA_FALSE; /* see GSDML/SharedDeviceSupported (TIA 1587591) */
            }
            #endif

            #ifdef PNDV_CFG_ENABLE_CIR
            ptr.rqb->args.sv.dev_add->CIR_enabled = LSA_TRUE;
            #endif
#endif

        }

        #ifdef PNDV_CFG_ENABLE_RS_INFO_BLOCK_SUPPORT
        ptr.rqb->args.sv.dev_add->gsd_ReportingSystemSupported  = LSA_TRUE; /* see GSDML/ReportingSystem (TIA 1645525) */
        #else
        ptr.rqb->args.sv.dev_add->gsd_ReportingSystemSupported  = LSA_FALSE;
        #endif

        #ifdef PNDV_CFG_DISABLE_SHARED
        ptr.rqb->args.sv.dev_add->gsd_SharedDeviceSupported = LSA_FALSE;
        #endif

            /*
             * the use-cases are described in the documentation
             *
             * SRD_enabled
             *   LSA_FALSE ... normal mode
             *   LSA_TRUE .... functionality "System Redundancy" enabled
             *
             * CIR_enabled
             *   LSA_FALSE ... normal mode
             *   LSA_TRUE .... functionality "Configuration in Run" enabled
             *   CIR can be enabled only if SRD is enabled
             *
             * R1_enabled
             *   LSA_FALSE ... normal mode
             *   LSA_TRUE .... functionality "R1" enabled
             *   R1 can be enabled only if SRD is enabled
             */

        ptr.rqb->args.sv.dev_add->PDevPrmInd_enabled = LSA_FALSE;
            /*
             * the use-case is described in the documentation
             * see CM_OPC_SV_AR_PDEVPRM_IND/_RSP
             */

        ptr.rqb->args.sv.dev_add->skip_check_100Mbps_FDX = LSA_FALSE;
            /*
             * LSA_FALSE ... normative behavior
             * LSA_TRUE .... non-normative behavior: skip "PdevCheckFailed" of CMDEV (see PNIO-Spec)
             */

        ptr.rqb->args.sv.dev_add->min_device_interval_31250ns = 0; /* e.g., 32 -> 32*31250ns = 1ms (AP01451871) */
            /*
             * 0 ... no limitation
             * n ... see GSDML/MinDeviceInterval
             *
             * if AR::send_clock * AR::reduction_ratio < min_device_interval then the AR is rejected
             */

        /* TIA 1453731 */
        /* GSDML/NumberOfAR ... see max_ars_IOC */
        ptr.rqb->args.sv.dev_add->gsd_MaxInputLength   = PNDV_MAX_IO_INPUT_LEN;   /* see GSDML/MaxInputLength */
        ptr.rqb->args.sv.dev_add->gsd_MaxOutputLength  = PNDV_MAX_IO_OUTPUT_LEN; /* see GSDML/MaxOutputLength */
        ptr.rqb->args.sv.dev_add->gsd_MaxDataLength    = PNDV_MAX_IO_IN_OUT_LEN;   /* see GSDML/MaxDataLength or calculated as defined in GSDML */
        #ifdef PNDV_CFG_ENABLE_R1
        if( pndv_r1_bridge_enable_bridge() )
        {
            ptr.rqb->args.sv.dev_add->gsd_NumberOfInputCR  = PNDV_MAX_ARS_RT_R1;   /* see GSDML/NumberOfInputCR (per device) */
            ptr.rqb->args.sv.dev_add->gsd_NumberOfOutputCR = PNDV_MAX_ARS_RT_R1;   /* see GSDML/NumberOfOutputCR (per device) */
        }
        else
        #endif
        {
            ptr.rqb->args.sv.dev_add->gsd_NumberOfInputCR  = PNDV_MAX_ARS_RT;   /* see GSDML/NumberOfInputCR (per device) */
            ptr.rqb->args.sv.dev_add->gsd_NumberOfOutputCR = PNDV_MAX_ARS_RT;   /* see GSDML/NumberOfOutputCR (per device) */
        }
        ptr.rqb->args.sv.dev_add->gsd_NumberOfAdditionalInputCR   = 0; /* see GSDML/NumberOfAdditionalInputCR */
        ptr.rqb->args.sv.dev_add->gsd_NumberOfAdditionalOutputCR  = 0; /* see GSDML/NumberOfAdditionalOutputCR */
        ptr.rqb->args.sv.dev_add->gsd_NumberOfMulticastProviderCR = 0; /* see GSDML/NumberOfAdditionalMulticastProviderCR (note that "Additional" is misleading) */
        ptr.rqb->args.sv.dev_add->gsd_NumberOfMulticastConsumerCR = 0; /* see GSDML/NumberOfMulticastConsumerCR */
        ptr.rqb->args.sv.dev_add->gsd_AssetManagementSupported = PNDV_CFG_IS_ASSET_MANAGEMENT_SUPPORTED;    /* if true, the AMFilterData block is appended to
                                                                                                               the I&M0FilterData blocks when reading 0xF840. User can read 0xF880. */

        ptr.rqb->args.sv.dev_add->nr_of_event_resources         = 0;
            /*
             * req: don't care
             * cnf: the cm-user must provide this many CM_OPC_SV_DEVICE_PROVIDE_EVENT resources
             */
        ptr.rqb->args.sv.dev_add->nr_of_alarm_resources         = 0;
            /*
             * req: don't care
             * cnf: the cm-user must provide this many CM_OPC_SV_DEVICE_PROVIDE_ALARM resources
             */
#endif

        pndv_in_write_debug_buffer_all__(PNDV_DC_CM_DV_ADD,0);

        PNDV_RQB_SET_HANDLE( ptr.rqb, pndv_data.cm_handle[PNDV_INDEX_PATH_ACP_CM]);
        PNDV_RQB_SET_OPCODE( ptr.rqb, CM_OPC_SV_DEVICE_ADD);

        pndv_data.diag.cm_dev_req_running = PNDV_CM_DEV_REQ_DEVICE_ADD;

        PNDV_CM_REQUEST( ptr.rqb);
    }
    else
    {
        pndv_data.diag.cm_dev_req.pending |= PNDV_CM_DEV_REQ_DEVICE_ADD;
    }
}


/**
 *  @brief callback to device add
 *
 *  @param[in]  rqb_ptr Pointer to request block
 *
 *  Callback to handle CM_OPC_SV_DEVICE_ADD.
 *  Also provides some device event (CM_OPC_SV_DEVICE_PROVIDE_EVENT)
 *  and alarm resources (CM_OPC_SV_DEVICE_PROVIDE_ALARM) to cm.
 *
 */
void pndv_in_cm_sv_device_add_done (CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    CM_UPPER_RQB_PTR_TYPE local_rqb_ptr;



    UnsignedOpt response;
    int         tmp_int;
    LSA_UINT16  local_nr_alarm_resources, local_nr_event_resources;
    UnsignedOpt           i;

    response = PNDV_RQB_GET_RESPONSE( rqb_ptr);

    if ( CM_OK != response )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
    }

    local_nr_event_resources = rqb_ptr->args.sv.dev_add->nr_of_event_resources;
    local_nr_alarm_resources = rqb_ptr->args.sv.dev_add->nr_of_alarm_resources;

    PNDV_FREE_MEM( &tmp_int, rqb_ptr->args.sv.dev_add);

    /* Event-Ressourcen CM-SV */
    /* --------------------------------------------------------------------- */

    for ( i = 0; i < local_nr_event_resources ; i++ )
    {
        PNDV_ALLOC_RQB( &local_rqb_ptr, sizeof(CM_RQB_TYPE));

        if ( pndv_host_ptr_is_nil__(local_rqb_ptr) )
        {
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
        }

        PNDV_ALLOC_MEM( ((LSA_VOID_PTR_TYPE)&local_rqb_ptr->args.sv.dev_provide_event) , sizeof(CM_EVENT_TYPE));

        if ( pndv_host_ptr_is_nil__(local_rqb_ptr->args.sv.dev_provide_event) )
        {
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
        }

        pndv_in_write_debug_buffer_all__(PNDV_DC_CM_RES_PROV,i);

        local_rqb_ptr->args.sv.dev_provide_event->device_nr = PNDV_CM_DEVICE_NO;

        PNDV_RQB_SET_HANDLE( local_rqb_ptr, pndv_data.cm_handle[PNDV_INDEX_PATH_ACP_CM]);
        PNDV_RQB_SET_OPCODE( local_rqb_ptr, CM_OPC_SV_DEVICE_PROVIDE_EVENT);

        PNDV_CM_REQUEST( local_rqb_ptr );
    }


    /* Event-Ressourcen CM-SV */
    /* --------------------------------------------------------------------- */

    for ( i = 0; i < local_nr_alarm_resources ; i++ )
    {
        PNDV_ALLOC_RQB( &local_rqb_ptr, sizeof(CM_RQB_TYPE));

        if ( pndv_host_ptr_is_nil__(local_rqb_ptr) )
        {
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
        }

        PNDV_ALLOC_MEM( ((LSA_VOID_PTR_TYPE)&local_rqb_ptr->args.sv.dev_provide_alarm), sizeof(CM_ALARM_TYPE));

        if ( pndv_host_ptr_is_nil__(local_rqb_ptr->args.sv.dev_provide_alarm) )
        {
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
        }

        pndv_in_write_debug_buffer_al__(PNDV_DC_CM_AL_RES_PROV,i);

        local_rqb_ptr->args.sv.dev_provide_alarm->device_nr = PNDV_CM_DEVICE_NO;

        PNDV_RQB_SET_HANDLE( local_rqb_ptr, pndv_data.cm_handle[PNDV_INDEX_PATH_ACP_CM]);
        PNDV_RQB_SET_OPCODE( local_rqb_ptr, CM_OPC_SV_DEVICE_PROVIDE_ALARM);

        PNDV_CM_REQUEST( local_rqb_ptr );
    }

    /* r1-bridge res */
    /* --------------------------------------------------------------------- */

#ifdef PNDV_CFG_ENABLE_R1
    pndv_r1_bridge_cm_provide_res();
#endif

    if ( LSA_OK != tmp_int)
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, tmp_int);
    }


    pndv_data.diag.cm_dev_req_running = 0;

    /* Call of pndv_in_al_check_pp_req not necessary, the pndv_sm not yet in ..AP_ADD */

    pndv_sm(PNDV_SM_EVENT_DEVICE_ADD_DONE);

    /* cm_dev_req_running is 0 -> possibly other dev req rep. */
    pndv_in_cm_check_dev_req();
}


/**
 *  @brief remove device from cm
 *
 *  Remove the device from cm CM_OPC_SV_DEVICE_REMOVE.
 *
 */
void pndv_in_cm_sv_device_remove (void)
{
    union
    {
        CM_UPPER_RQB_PTR_TYPE rqb;
        LSA_VOID_PTR_TYPE     void_;
    }   ptr;

    if ( 0 == pndv_data.diag.cm_dev_req_running )
    {
        PNDV_ALLOC_RQB( &ptr.void_, sizeof(CM_RQB_TYPE));

        if ( pndv_host_ptr_is_nil__(ptr.rqb) )
        {
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
        }

        PNDV_ALLOC_MEM( ((LSA_VOID_PTR_TYPE)&ptr.rqb->args.sv.dev_remove), sizeof(CM_SV_DEVICE_REMOVE_TYPE));

        if ( pndv_host_ptr_is_nil__(ptr.rqb->args.sv.dev_remove) )
        {
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
        }

        ptr.rqb->args.sv.dev_remove->device_nr = PNDV_CM_DEVICE_NO;              /* device handle 1..max_devices, see "create server" */

        pndv_in_write_debug_buffer_all__(PNDV_DC_CM_DV_REMOVE,0);

        PNDV_RQB_SET_HANDLE( ptr.rqb, pndv_data.cm_handle[PNDV_INDEX_PATH_ACP_CM]);
        PNDV_RQB_SET_OPCODE( ptr.rqb, CM_OPC_SV_DEVICE_REMOVE);

        pndv_data.diag.cm_dev_req_running = PNDV_CM_DEV_REQ_DEVICE_RMV;

        PNDV_CM_REQUEST( ptr.rqb);
    }
    else
    {
        pndv_data.diag.cm_dev_req.pending |= PNDV_CM_DEV_REQ_DEVICE_RMV;
    }
}

/**
 *  @brief callback to device remove
 *
 *  Callback to device remove from cm CM_OPC_SV_DEVICE_REMOVE.
 *
 */
void pndv_in_cm_sv_device_remove_done (CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    UnsignedOpt response;
    int         tmp_int;

    response = PNDV_RQB_GET_RESPONSE( rqb_ptr);

    if ( CM_OK != response )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, response);
    }

    PNDV_FREE_MEM( &tmp_int, rqb_ptr->args.sv.dev_remove);

    if ( LSA_OK != tmp_int)
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, tmp_int);
    }

    pndv_data.diag.cm_dev_req_running = 0;
    /* Device was just removed so that all device requests would lead to an error 
     * and are no longer relevant anyway. Calling pndv_in_cm_check_devreq () is therefore 
     * no longer necessary.
     */
    pndv_data.diag.cm_dev_req.pending = 0;
    /* Call of pndv_in_al_check_pp_req not necessary, the pndv_sm not yet in ..AP_ADD */

    pndv_sm(PNDV_SM_EVENT_DEVICE_REMOVE_DONE);

}

/**
 *  @brief callback for device read request
 *
 *  @param[in]  rqb_ptr Pointer to request block
 *
 *  Callback for device read request CM_OPC_SV_DEVICE_READ.
 *  Service not used atm.
 *
 */
#ifdef PNDV_CFG_USE_DEVICE_READ
void pndv_in_cm_sv_device_read_done (CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    UnsignedOpt                 response;
    PNDV_RQB_PTR_TYPE           local_rqb_ptr;
    LSA_UINT16                  ret_val;

    response = PNDV_RQB_GET_RESPONSE( rqb_ptr);

    if ( CM_OK != response )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, response);
    }

    local_rqb_ptr = pndv_data.usr.dev_read.usr_rqb;

    local_rqb_ptr->args->dev_read.pnio_status = rqb_ptr->args.sv.dev_read->pnio_status;
    local_rqb_ptr->args->dev_read.data_length = rqb_ptr->args.sv.dev_read->data_length;

    PNDV_FREE_MEM(&ret_val, rqb_ptr->args.sv.dev_read);
    if(ret_val != PNDV_OK)
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, ret_val);
    }

    PNDV_FREE_MEM(&ret_val, rqb_ptr);
    if(ret_val != PNDV_OK)
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, ret_val);
    }

    PNDV_RQB_SET_RESPONSE(local_rqb_ptr, PNDV_OK);

    pndv_data.usr.dev_read.service_running = FALSE;

    PNDV_CFG_USR_REQ_DONE(local_rqb_ptr);

}
#endif

/**
 *  @brief starts local prm phase (PD) at startup
 *
 *
 *  Send CM_OPC_PD_PRM_PREPARE to indicate start of the local
 *  startup parameter phase.
 *
 */
void pndv_in_cm_pd_prm_begin (void)
{
    union
    {
        CM_UPPER_RQB_PTR_TYPE rqb;
        LSA_VOID_PTR_TYPE     void_;
    }   ptr;

    if ( PNDV_SM_DEVICE_ACTIVE > pndv_data.sm )
    {
        PNDV_ALLOC_RQB( &ptr.void_, sizeof(CM_RQB_TYPE));

        if ( pndv_host_ptr_is_nil__(ptr.rqb) )
        {
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
        }

        PNDV_RQB_SET_HANDLE( ptr.rqb, pndv_data.cm_handle[PNDV_INDEX_PATH_CMPDSV]);
        PNDV_RQB_SET_OPCODE( ptr.rqb, CM_OPC_PD_PRM_BEGIN);

        pndv_in_write_debug_buffer_all__(PNDV_DC_CM_PD_PRM_PREPARE,0);

        PNDV_CM_REQUEST( ptr.rqb);
    }
    else
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, pndv_data.sm );
    }
}



//void PNDV_GET_PRM_LIST (PNDV_PDEV_PRM_RQB_QUEUE_PTR_T prm_queue_ptr)
void pndv_in_cm_pd_build_prm_list (PNDV_PDEV_PRM_RQB_QUEUE_PTR_T prm_queue_ptr)
{
    PNDV_PRM_REQUEST_ELEMENT_PTR_T local_elem_ptr;

    /* prepare rqb for multiple pdev data */
    local_elem_ptr = PNDV_LIST_FIRST(&(prm_queue_ptr->free_list), PNDV_PRM_REQUEST_ELEMENT_PTR_T);
    if ( local_elem_ptr == LSA_NULL )
    {
        /* no elements in free list, must not be here */

        pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
    }
    else
    {
        Unsigned32    * data_ptr = 0;
        Unsigned32      data_length;
        /* Are there permanently stored PDEV data records? */

        PNDV_GET_PD_DS ( &data_ptr, &data_length );

        if (data_ptr)
        {

            CM_UPPER_RQB_PTR_TYPE    rqb_ptr = local_elem_ptr->rqb_ptr;

            PNDV_ALLOC_MEM( ((LSA_VOID_PTR_TYPE)&rqb_ptr->args.pd.prm_rdwr->record_data), data_length);

            rqb_ptr->args.pd.prm_rdwr->record_index         = PNDV_RECORD_INDEX_MULTIPLE_PDEV;
            rqb_ptr->args.pd.prm_rdwr->record_data_length   = data_length;
                                                              /* Address of the first data element of the record_data */
                                                              /* in its original structure, the ptr stands on the record_data */
            rqb_ptr->args.pd.prm_rdwr->mk_remanent          = TRUE;
            PNDV_COPY_BYTE( (CM_COMMON_MEM_U8_PTR_TYPE)rqb_ptr->args.pd.prm_rdwr->record_data, (CM_COMMON_MEM_U8_PTR_TYPE)data_ptr, data_length);

            PNDV_LIST_REMOVE_ENTRY(&local_elem_ptr->link);
            PNDV_LIST_INSERT_TAIL(&(prm_queue_ptr->in_use), &local_elem_ptr->link);
        }
    }

    /* prepare rqb for multiple snmp data */
    local_elem_ptr = PNDV_LIST_FIRST(&(prm_queue_ptr->free_list), PNDV_PRM_REQUEST_ELEMENT_PTR_T);
    if ( local_elem_ptr == LSA_NULL )
    {
        /* no elements in free list, must not be here */

        pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
    }
    else
    {
        Unsigned8    * name_ptr = 0;
        Unsigned32      name_length=0;
        Unsigned8    * contact_ptr = 0;
        Unsigned32      contact_length=0;
        Unsigned8    * location_ptr = 0;
        Unsigned32      location_length=0;

        /* Are there permanently stored PDEV data records? */

        PNDV_GET_SNMP_DATA ( &name_ptr, &name_length, &contact_ptr, &contact_length, &location_ptr, &location_length );

        if (name_ptr || contact_ptr || location_ptr)
        {
            CM_UPPER_RQB_PTR_TYPE        rqb_ptr = local_elem_ptr->rqb_ptr;
            CM_COMMON_MEM_U8_PTR_TYPE    data_copy_ptr;
            Unsigned32                   copy_pos=0;
            Unsigned32                     total_rec_len = 0;

            /* calc length */

            if (name_ptr)
            {
                total_rec_len = (total_rec_len + name_length + 4 /*snmp data header*/ + PNDV_PRM_REC_BLOCK_LEN_SNMP + 3) >>2;
                total_rec_len = total_rec_len<<2;
            }
            if (contact_ptr)
            {
                total_rec_len = (total_rec_len + contact_length + 4 /*snmp data header*/ + PNDV_PRM_REC_BLOCK_LEN_SNMP + 3) >>2;
                total_rec_len = total_rec_len<<2;
            }
            if (location_ptr)
            {
                total_rec_len = (total_rec_len + location_length + 4 /*snmp data header*/ + PNDV_PRM_REC_BLOCK_LEN_SNMP + 3) >>2;
                total_rec_len = total_rec_len<<2;
            }

            PNDV_ALLOC_MEM( ((LSA_VOID_PTR_TYPE)&rqb_ptr->args.pd.prm_rdwr->record_data), total_rec_len);

            data_copy_ptr = rqb_ptr->args.pd.prm_rdwr->record_data;


            if (name_ptr)
            {
                Unsigned32 BlockLength;
                Unsigned32 copy_pos_data_end;

                BlockLength =  name_length + 4;

                /* BlockType / Index = 0x00011003 */
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x01;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x10;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x01;
                /* API = 0x00000000 */
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                /* Slot = 0x0000 */
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                /* SubSlot = 0x8000 */
                data_copy_ptr[copy_pos++] = (Unsigned8)((pndv_data.cfg.pd.interface_subslot_nr) >> 8);
                data_copy_ptr[copy_pos++] = (Unsigned8)((pndv_data.cfg.pd.interface_subslot_nr));
                /* BlockLength */
                data_copy_ptr[copy_pos++] = (Unsigned8)((BlockLength) >> 24);
                data_copy_ptr[copy_pos++] = (Unsigned8)((BlockLength) >> 16);
                data_copy_ptr[copy_pos++] = (Unsigned8)((BlockLength) >> 8);
                data_copy_ptr[copy_pos++] = (Unsigned8)((BlockLength));
                /* Comp_ID = 0x0000 */
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                /* Reserved = 0x0000 */
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                /* SNMP-Data type 0x0004 */
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x04;
                /* SNMP-Data length */
                data_copy_ptr[copy_pos++] = (Unsigned8)((name_length) >> 8);
                data_copy_ptr[copy_pos++] = (Unsigned8)((name_length));
                /* SNMP-Data */
                PNDV_COPY_BYTE( (CM_COMMON_MEM_U8_PTR_TYPE)&data_copy_ptr[copy_pos], (CM_COMMON_MEM_U8_PTR_TYPE)name_ptr, name_length);
                copy_pos          = copy_pos + name_length;
                copy_pos_data_end = copy_pos;
                while(copy_pos < ((copy_pos_data_end + 3) & ~0x03))
                {
                    data_copy_ptr[copy_pos++] = (Unsigned8)(0x00);
                }
            }

            if (contact_ptr)
            {
                Unsigned32 BlockLength;
                Unsigned32 copy_pos_data_end;

                BlockLength =  contact_length + 4;

                /* BlockType / Index = 0x00011003 */
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x01;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x10;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x02;
                /* API = 0x00000000 */
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                /* Slot = 0x0000 */
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                /* SubSlot = 0x8000 */
                data_copy_ptr[copy_pos++] = (Unsigned8)((pndv_data.cfg.pd.interface_subslot_nr) >> 8);
                data_copy_ptr[copy_pos++] = (Unsigned8)((pndv_data.cfg.pd.interface_subslot_nr));
                /* BlockLength */
                data_copy_ptr[copy_pos++] = (Unsigned8)((BlockLength) >> 24);
                data_copy_ptr[copy_pos++] = (Unsigned8)((BlockLength) >> 16);
                data_copy_ptr[copy_pos++] = (Unsigned8)((BlockLength) >> 8);
                data_copy_ptr[copy_pos++] = (Unsigned8)((BlockLength));
                /* Comp_ID = 0x0000 */
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                /* Reserved = 0x0000 */
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                /* SNMP-Data type 0x0004 */
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x04;
                /* SNMP-Data length */
                data_copy_ptr[copy_pos++] = (Unsigned8)((contact_length) >> 8);
                data_copy_ptr[copy_pos++] = (Unsigned8)((contact_length));
                /* SNMP-Data */
                PNDV_COPY_BYTE( (CM_COMMON_MEM_U8_PTR_TYPE)&data_copy_ptr[copy_pos], (CM_COMMON_MEM_U8_PTR_TYPE)contact_ptr, contact_length);
                copy_pos          = copy_pos + contact_length;
                copy_pos_data_end = copy_pos;
                while(copy_pos < ((copy_pos_data_end + 3) & ~0x03))
                {
                    data_copy_ptr[copy_pos++] = (Unsigned8)(0x00);
                }
            }

            if (location_ptr)
            {
                Unsigned32 BlockLength;
                Unsigned32 copy_pos_data_end;

                BlockLength =  location_length + 4;

                /* BlockType / Index = 0x00011003 */
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x01;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x10;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x03;
                /* API = 0x00000000 */
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                /* Slot = 0x0000 */
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                /* SubSlot = 0x8000 */
                data_copy_ptr[copy_pos++] = (Unsigned8)((pndv_data.cfg.pd.interface_subslot_nr) >> 8);
                data_copy_ptr[copy_pos++] = (Unsigned8)((pndv_data.cfg.pd.interface_subslot_nr));
                /* BlockLength */
                data_copy_ptr[copy_pos++] = (Unsigned8)((BlockLength) >> 24);
                data_copy_ptr[copy_pos++] = (Unsigned8)((BlockLength) >> 16);
                data_copy_ptr[copy_pos++] = (Unsigned8)((BlockLength) >> 8);
                data_copy_ptr[copy_pos++] = (Unsigned8)((BlockLength));
                /* Comp_ID = 0x0000 */
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                /* Reserved = 0x0000 */
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                /* SNMP-Data type 0x0004 */
                data_copy_ptr[copy_pos++] = (Unsigned8)0x00;
                data_copy_ptr[copy_pos++] = (Unsigned8)0x04;
                /* SNMP-Data length */
                data_copy_ptr[copy_pos++] = (Unsigned8)((location_length) >> 8);
                data_copy_ptr[copy_pos++] = (Unsigned8)((location_length));
                /* SNMP-Data */
                PNDV_COPY_BYTE( (CM_COMMON_MEM_U8_PTR_TYPE)&data_copy_ptr[copy_pos], (CM_COMMON_MEM_U8_PTR_TYPE)location_ptr, location_length);
                copy_pos          = copy_pos + location_length;
                copy_pos_data_end = copy_pos;
                while(copy_pos < ((copy_pos_data_end + 3) & ~0x03))
                {
                    data_copy_ptr[copy_pos++] = (Unsigned8)(0x00);
                }
            }


            rqb_ptr->args.pd.prm_rdwr->record_index         = PNDV_RECORD_INDEX_MULTIPLE_SNMP;
            rqb_ptr->args.pd.prm_rdwr->slot_nr              = PNDV_IM_SLOT_NO;
            rqb_ptr->args.pd.prm_rdwr->subslot_nr           = pndv_data.cfg.pd.interface_subslot_nr;

#ifdef PNDV_CFG_ENABLE_R1
            if( pndv_r1_bridge_enable_bridge() )
            {
                rqb_ptr->args.pd.prm_rdwr->subslot_nr           = 0x8000+(PNDV_IM_SLOT_NO*0x1000);
            }
            /*else
            {
                single IM always uses 0x8000
            }*/
#endif
            rqb_ptr->args.pd.prm_rdwr->record_data_length   = total_rec_len;
                                                              /* Address of the first data element of the record_data */
                                                              /* in its original structure, the ptr stands on the record_data */
            rqb_ptr->args.pd.prm_rdwr->mk_remanent          = TRUE;

            PNDV_LIST_REMOVE_ENTRY(&local_elem_ptr->link);
            PNDV_LIST_INSERT_TAIL(&(prm_queue_ptr->in_use), &local_elem_ptr->link);
        }
    }

    /* prepare rqb for ip suite data */
    local_elem_ptr = PNDV_LIST_FIRST(&(prm_queue_ptr->free_list), PNDV_PRM_REQUEST_ELEMENT_PTR_T);
    if ( local_elem_ptr == LSA_NULL )
    {
        /* no elements in free list, must not be here */

        pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
    }
    else
    {
        Unsigned32    * data_ptr = 0;
        Unsigned32      data_length=0;
        /* Are there permanently stored PDEV data records? */

        PNDV_GET_IP_DATA ( &data_ptr, &data_length );

        if (data_ptr)
        {
            CM_UPPER_RQB_PTR_TYPE        rqb_ptr = local_elem_ptr->rqb_ptr;
            CM_COMMON_MEM_U8_PTR_TYPE    data_copy_ptr;
            Unsigned32                   copy_pos=0;
            Unsigned32                     total_rec_len = (PNDV_PRM_REC_BLOCK_LEN_IP_SUITE + data_length + 3)>>2;

            total_rec_len = total_rec_len<<2;

            PNDV_ALLOC_MEM( ((LSA_VOID_PTR_TYPE)&rqb_ptr->args.pd.prm_rdwr->record_data), PNDV_PRM_REC_BLOCK_HEADER_LEN + total_rec_len);

            data_copy_ptr = rqb_ptr->args.pd.prm_rdwr->record_data;


            /* prepare record data header */
            data_copy_ptr[copy_pos++] = (Unsigned8)(PNDV_PRM_REC_BLOCK_TYPE_IP_SUITE >> 8);
            data_copy_ptr[copy_pos++] = (Unsigned8)(PNDV_PRM_REC_BLOCK_TYPE_IP_SUITE);
            data_copy_ptr[copy_pos++] = (Unsigned8)((total_rec_len) >> 8);
            data_copy_ptr[copy_pos++] = (Unsigned8)((total_rec_len));
            data_copy_ptr[copy_pos++] = (Unsigned8)(PNDV_RM_REC_BLOCK_VERS_H);
            data_copy_ptr[copy_pos++] = (Unsigned8)(PNDV_RM_REC_BLOCK_VERS_L);
            data_copy_ptr[copy_pos++] = (Unsigned8)(0x00);
            data_copy_ptr[copy_pos++] = (Unsigned8)(0x00);
            PNDV_COPY_BYTE( (CM_COMMON_MEM_U8_PTR_TYPE)&data_copy_ptr[copy_pos], (CM_COMMON_MEM_U8_PTR_TYPE)data_ptr, data_length);


            rqb_ptr->args.pd.prm_rdwr->record_index         = PNDV_RECORD_INDEX_IP_SUITE;
            rqb_ptr->args.pd.prm_rdwr->slot_nr              = PNDV_IM_SLOT_NO;
            rqb_ptr->args.pd.prm_rdwr->subslot_nr           = pndv_data.cfg.pd.interface_subslot_nr;

#ifdef PNDV_CFG_ENABLE_R1
            if( pndv_r1_bridge_enable_bridge() )
            {
                rqb_ptr->args.pd.prm_rdwr->subslot_nr           = 0x8000+(PNDV_IM_SLOT_NO*0x1000);
            }
            /*else
            {
                single IM always uses 0x8000
            }*/
#endif

            rqb_ptr->args.pd.prm_rdwr->record_data_length   = PNDV_PRM_REC_BLOCK_HEADER_LEN + total_rec_len;
                                                              /* Adresse vom ersten Datenelement der record_data */
                                                              /* in Originalstruktur steht hier der ptr auf die record_data */
            rqb_ptr->args.pd.prm_rdwr->mk_remanent          = TRUE;

            PNDV_LIST_REMOVE_ENTRY(&local_elem_ptr->link);
            PNDV_LIST_INSERT_TAIL(&(prm_queue_ptr->in_use), &local_elem_ptr->link);
        }
    }

    /* prepare rqb for name of station data */
    local_elem_ptr = PNDV_LIST_FIRST(&(prm_queue_ptr->free_list), PNDV_PRM_REQUEST_ELEMENT_PTR_T);
    if ( local_elem_ptr == LSA_NULL )
    {
        /* no elements in free list, must not be here */

        pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
    }
    else
    {
        Unsigned32    * data_ptr = 0;
        Unsigned32      data_length=0;
        /* Are there permanently stored PDEV data records? */

        PNDV_GET_NOS_DATA ( &data_ptr, &data_length );

        if (data_ptr)
        {
            CM_UPPER_RQB_PTR_TYPE        rqb_ptr = local_elem_ptr->rqb_ptr;
            CM_COMMON_MEM_U8_PTR_TYPE    data_copy_ptr;
            Unsigned32                   copy_pos=0;
            Unsigned32                     total_rec_len = (PNDV_PRM_REC_BLOCK_LEN_NAME_OF_STATION + data_length + 3)>>2;

            total_rec_len = total_rec_len<<2;

            PNDV_ALLOC_MEM( ((LSA_VOID_PTR_TYPE)&rqb_ptr->args.pd.prm_rdwr->record_data), PNDV_PRM_REC_BLOCK_HEADER_LEN + total_rec_len);

            data_copy_ptr = rqb_ptr->args.pd.prm_rdwr->record_data;


            /* prepare record data header */
            data_copy_ptr[copy_pos++] = (Unsigned8)(PNDV_PRM_REC_BLOCK_TYPE_NAME_OF_STATION >> 8);
            data_copy_ptr[copy_pos++] = (Unsigned8)(PNDV_PRM_REC_BLOCK_TYPE_NAME_OF_STATION);
            data_copy_ptr[copy_pos++] = (Unsigned8)((total_rec_len) >> 8);
            data_copy_ptr[copy_pos++] = (Unsigned8)((total_rec_len));
            data_copy_ptr[copy_pos++] = (Unsigned8)(PNDV_RM_REC_BLOCK_VERS_H);
            data_copy_ptr[copy_pos++] = (Unsigned8)(PNDV_RM_REC_BLOCK_VERS_L);
            data_copy_ptr[copy_pos++] = (Unsigned8)(0x00);
            data_copy_ptr[copy_pos++] = (Unsigned8)(0x00);
            data_copy_ptr[copy_pos++] = (Unsigned8)(data_length >> 8);
            data_copy_ptr[copy_pos++] = (Unsigned8)(data_length);
            data_copy_ptr[copy_pos++] = (Unsigned8)(0x00);
            data_copy_ptr[copy_pos++] = (Unsigned8)(0x00);
            PNDV_COPY_BYTE( (CM_COMMON_MEM_U8_PTR_TYPE)&data_copy_ptr[copy_pos], (CM_COMMON_MEM_U8_PTR_TYPE)data_ptr, data_length);
            copy_pos += data_length;
            while(copy_pos < total_rec_len + PNDV_PRM_REC_BLOCK_HEADER_LEN)
            {
                data_copy_ptr[copy_pos++] = (Unsigned8)(0x00);
            }

            rqb_ptr->args.pd.prm_rdwr->record_index         = PNDV_RECORD_INDEX_NAME_OF_STATION;
            rqb_ptr->args.pd.prm_rdwr->slot_nr              = PNDV_IM_SLOT_NO;
            rqb_ptr->args.pd.prm_rdwr->subslot_nr           = pndv_data.cfg.pd.interface_subslot_nr;

#ifdef PNDV_CFG_ENABLE_R1
            if( pndv_r1_bridge_enable_bridge() )
            {
                rqb_ptr->args.pd.prm_rdwr->subslot_nr           = 0x8000+(PNDV_IM_SLOT_NO*0x1000);
            }
            /*else
            {
                single IM always uses 0x8000
            }*/
#endif

            rqb_ptr->args.pd.prm_rdwr->record_data_length   = PNDV_PRM_REC_BLOCK_HEADER_LEN + total_rec_len;
                                                              /* Address of the first data element of the record_data */
                                                              /* in its original structure, the ptr stands on the record_data */
            rqb_ptr->args.pd.prm_rdwr->mk_remanent          = TRUE;

            PNDV_LIST_REMOVE_ENTRY(&local_elem_ptr->link);
            PNDV_LIST_INSERT_TAIL(&(prm_queue_ptr->in_use), &local_elem_ptr->link);
        }
    }
}


/**
 *  @brief callback to prm prepare (PD)
 *
 *  @param[in]  rqb_ptr Pointer to request block
 *
 *  Callback to prm prepare request CM_OPC_PD_PRM_PREPARE.
 *  Also starts sending local parameter to cm.
 *
 */
void pndv_in_cm_pd_prm_begin_done (CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    UnsignedOpt response;

    response = PNDV_RQB_GET_RESPONSE( rqb_ptr);

    if ( CM_OK != response )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, response);
    }

    pndv_in_write_debug_buffer_all_add__(PNDV_DC_CM_PD_PRM_PREPARE_DONE, pndv_data.cmpd_prm_default, pndv_data.reset_reason);

    if(    ( TRUE == pndv_data.cmpd_prm_default)
        || ( PNDV_RESET_REASON_CODE_ERR_PRM_CONSISTENCY == pndv_data.reset_reason )
      )
    {
        /* After the parameterization has been canceled, an immediate PNDV_DC_CM_PD_PRM_END takes place */
        /* -> default parameterization of PDEV */

        pndv_in_cm_pd_prm_end ( );
    }
    else
    {
        if ( TRUE == pndv_data.stop_req )
        {
            pndv_in_stop();
        }
        else
        {
            /* init prm record list */
            PNDV_GET_PRM_LIST (&pndv_data.prm_queue);

            /* check for prm records to send */
            pndv_in_cm_pd_prm_write();
        }
    }
}

/**
 *  @brief write parameter to physical device
 *
 *  @param[in]  pData Pointer of data to be writen
 *  @param[in]  DataLength Lenght of data to be writen
 *
 *  Sends the given data to cm using PNDV_DC_CM_PD_PRM_WRITE request.
 *  Data is expected to be a multiple pdev record.
 *
 */
void pndv_in_cm_pd_prm_write()
{
    PNDV_PRM_REQUEST_ELEMENT_PTR_T local_elem_ptr;
    /* Are there permanently stored PDEV data records? */

    local_elem_ptr = PNDV_LIST_FIRST(&pndv_data.prm_queue.in_use, PNDV_PRM_REQUEST_ELEMENT_PTR_T);
    if ( local_elem_ptr == LSA_NULL )
    {
        /* all elements processed or no remanently stored PDEV data sets in flash */

        pndv_in_cm_pd_prm_end ( );
    }
    else
    {
        CM_UPPER_RQB_PTR_TYPE    rqb_ptr = local_elem_ptr->rqb_ptr;

        PNDV_LIST_REMOVE_ENTRY(&local_elem_ptr->link);

        PNDV_RQB_SET_HANDLE( rqb_ptr, pndv_data.cm_handle[PNDV_INDEX_PATH_CMPDSV]);
        PNDV_RQB_SET_OPCODE( rqb_ptr, CM_OPC_PD_PRM_WRITE);

        pndv_in_write_debug_buffer_all_add__(PNDV_DC_CM_PD_PRM_WRITE, 0, rqb_ptr->args.pd.prm_rdwr->record_index);

        PNDV_CM_REQUEST(rqb_ptr);

        //! append to free list
        PNDV_LIST_INSERT_TAIL(&pndv_data.prm_queue.free_list, &local_elem_ptr->link);
    }
}


/**
 *  @brief callback to prm write
 *
 *  @param[in]  rqb_ptr Pointer to request block
 *
 *  Callback to prm write request (CM_OPC_PD_PRM_WRITE).
 *  Paramter where send to cm. If data was unplausible default parameters are
 *  prepared otherwise the prm phase will be ended.
 *
 */
void pndv_in_cm_pd_prm_write_done (CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    UnsignedOpt response;

    response = PNDV_RQB_GET_RESPONSE( rqb_ptr);

    if ( CM_OK != response )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, response);
    }

    if ( CM_PNIO_ERR_NONE != rqb_ptr->args.pd.prm_rdwr->cm_pnio_err )
    {
        /* Written DS was inconsistent in content or incorrect with regard to slot, subslot, index.  */
        /* Resetting PNDV_DC_CM_PD_PRM_PREPARE causes the ongoing parameterization to be canceled     */

        pndv_data.cmpd_prm_default  = TRUE;

        // pndv_in_cm_pd_prm_abort();
    }

    pndv_in_write_debug_buffer_all__(PNDV_DC_CM_PD_PRM_WRITE_DONE, rqb_ptr->args.pd.prm_rdwr->record_index);

    if ( TRUE == pndv_data.stop_req )
    {
        pndv_in_stop();
    }
    else
    {
        /* check are there more parameters to write */
        pndv_in_cm_pd_prm_write();
    }
}



/**
 *  @brief indicate end of parameter phase (PD)
 *
 *
 *  Send a CM_OPC_PD_PRM_END request to cm to indicate the end of
 *  pdev paramter phase.
 *
 */
void pndv_in_cm_pd_prm_end (void)
{
    union
    {
        CM_UPPER_RQB_PTR_TYPE rqb;
        LSA_VOID_PTR_TYPE     void_;
    }   ptr;

    PNDV_ALLOC_RQB( &ptr.void_, sizeof(CM_RQB_TYPE));

    if ( pndv_host_ptr_is_nil__(ptr.rqb) )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
    }

    PNDV_ALLOC_MEM( ((LSA_VOID_PTR_TYPE)&ptr.rqb->args.pd.prm_end), sizeof(CM_EVENT_TYPE));

    if ( pndv_host_ptr_is_nil__(ptr.rqb->args.pd.event) )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
    }

    ptr.rqb->args.pd.prm_end->set_mrp_off = LSA_FALSE;

    PNDV_RQB_SET_HANDLE( ptr.rqb, pndv_data.cm_handle[PNDV_INDEX_PATH_CMPDSV]);
    PNDV_RQB_SET_OPCODE( ptr.rqb, CM_OPC_PD_PRM_END);

    pndv_in_write_debug_buffer_all__(PNDV_DC_CM_PD_PRM_END,0);

    PNDV_CM_REQUEST( ptr.rqb);
}


/**
 *  @brief callback to prm end (PD)
 *
 *  @param[in]  rqb_ptr Pointer to request block
 *
 *  Callback to parameter end request of the physical device
 *  (CM_OPC_PD_PRM_END).
 *
 */
void pndv_in_cm_pd_prm_end_done (CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    UnsignedOpt response;
    int         tmp_int;

    response = PNDV_RQB_GET_RESPONSE( rqb_ptr);

    if ( CM_OK != response )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, response);
    }

    PNDV_FREE_MEM( &tmp_int, rqb_ptr->args.pd.prm_end);

    if ( LSA_OK != tmp_int)
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, tmp_int);
    }

    /* Reset flag for possibly triggered default parameterization */
    pndv_data.cmpd_prm_default  = FALSE;

    pndv_in_write_debug_buffer_all__(PNDV_DC_CM_PD_PRM_END_DONE,0);
}

/**
 *  @brief Handle appl ready indication
 *
 *  @param[in]  rqb_ptr Pointer to request block
 *
 *  Handler for appl ready indication (CM_OPC_PD_EVENT_APPL_READY_IND).
 *  Resets device on inconsistent parameter error or gives CM_OPC_PD_EVENT_APPL_READY_RSP
 *  to cm.
 *
 */
void pndv_in_cm_pd_event_appl_ready_ind (CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    UnsignedOpt response;

    response = PNDV_RQB_GET_RESPONSE( rqb_ptr);

    pndv_in_write_debug_buffer_al__(PNDV_DC_CM_PD_EVENT_APPL_READY_IND,response);

    switch ( response )
    {
        case CM_OK:
        case CM_OK_RESET_TO_FACTORY:
        {
            break;
        }

        case CM_ERR_PRM_CONSISTENCY:
        {

            if( pndv_data.sm != PNDV_SM_DEVICE_ACTIVE) /* don't reset device, this indication is not the result of setting the startup parameter */
            {
                PNDV_RESET_DEVICE(PNDV_RESET_REASON_CODE_ERR_PRM_CONSISTENCY);
            }
            break;
        }

        default:
        {
            /* 070104 AnRe here a specific evaluation of the .u.pd.appl_ready.diff_list has to be done -> CM-Spec 9.13 ??? */
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, response);
            break;
        }
    }

    pndv_in_write_debug_buffer_al__(PNDV_DC_CM_PD_EVENT_APPL_READY_IND,0);

    /* Parameterization of the PDEV submodules after plugging them into pndv_in_cm_submodule_add_done completed  */
    /* -> switch state and possibly insert peri-module                                                     */
    /* From basis 5.3 there is also an indication with every AR setup / PDEV parameterization.             */
    /* --------------------------------------------------------------------------------------------------- */

    /* PDEV is ready, device can be activated */
    if( pndv_data.sm != PNDV_SM_DEVICE_ACTIVE) /* don't trigger pndv_sm after device activate, this indications are a result of an ar establishment */
    {
        pndv_sm(PNDV_SM_EVENT_PRM_PDEV_DONE);
    }

    PNDV_RQB_SET_HANDLE( rqb_ptr, pndv_data.cm_handle[PNDV_INDEX_PATH_CMPDSV]);
    PNDV_RQB_SET_OPCODE( rqb_ptr, CM_OPC_PD_EVENT_APPL_READY_RSP);

    PNDV_CM_REQUEST( rqb_ptr);
}


/**
 *  @brief send device control request to cm
 *
 *  @param[in]  device_control Control command to be send (CM_SV_DEVICE_CONTROL_CMD_ACTIVATE,CM_SV_DEVICE_CONTROL_CMD_PASSIVATE)
 *
 *  Sends the requested device control (CM_OPC_SV_DEVICE_CONTROL)
 *  command (activate or passivate) to cm.
 *  Only activation is allowed at the moment.
 *
 */
void pndv_in_device_control (Unsigned8 device_control)
{
    CM_UPPER_RQB_PTR_TYPE tmp_rqb_ptr;

    if ( 0 == pndv_data.diag.cm_dev_req_running )
    {
        switch ( device_control )
        {
            case CM_SV_DEVICE_CONTROL_CMD_ACTIVATE:
                    if (   (  (PNDV_SM_W_ADD_IM_SUBMODULES_DONE     <= pndv_data.sm )
                              && (PNDV_SM_DEVICE_ACTIVE         >= pndv_data.sm )
                            )
                       )
                    {
                        pndv_data.rqb.dev_control.args.sv.dev_control->device_nr = PNDV_CM_DEVICE_NO;

                        if ( 0 != PNDV_RQB_GET_OPCODE(&pndv_data.rqb.dev_control) )
                        {
                            pndv_in_fatal_error( PNDV_MODULE, __LINE__, PNDV_RQB_GET_OPCODE(&pndv_data.rqb.dev_control));
                        }

                        pndv_data.rqb.dev_control.args.sv.dev_control->cmd = device_control;

                        pndv_in_write_debug_buffer_all__(PNDV_DC_CM_DV_AKT, device_control);

                        PNDV_RQB_SET_HANDLE( &pndv_data.rqb.dev_control, pndv_data.cm_handle[PNDV_INDEX_PATH_ACP_CM]);
                        PNDV_RQB_SET_OPCODE( &pndv_data.rqb.dev_control, CM_OPC_SV_DEVICE_CONTROL);

                        tmp_rqb_ptr = &pndv_data.rqb.dev_control;

                        pndv_data.diag.cm_dev_req_running = PNDV_CM_DEV_REQ_DEVICE_CONTROL;

                        PNDV_CM_REQUEST( tmp_rqb_ptr);
                    }
                    else
                    {
                        pndv_in_fatal_error( PNDV_MODULE, __LINE__, pndv_data.sm);
                    }
                break;
            case CM_SV_DEVICE_CONTROL_CMD_PASSIVATE:
                pndv_data.rqb.dev_control.args.sv.dev_control->device_nr = PNDV_CM_DEVICE_NO;

                if ( 0 != PNDV_RQB_GET_OPCODE(&pndv_data.rqb.dev_control) )
                {
                    pndv_in_fatal_error( PNDV_MODULE, __LINE__, PNDV_RQB_GET_OPCODE(&pndv_data.rqb.dev_control));
                }

                pndv_data.rqb.dev_control.args.sv.dev_control->cmd = device_control;

                pndv_in_write_debug_buffer_all__(PNDV_DC_CM_DV_AKT, device_control);

                PNDV_RQB_SET_HANDLE( &pndv_data.rqb.dev_control, pndv_data.cm_handle[PNDV_INDEX_PATH_ACP_CM]);
                PNDV_RQB_SET_OPCODE( &pndv_data.rqb.dev_control, CM_OPC_SV_DEVICE_CONTROL);

                tmp_rqb_ptr = &pndv_data.rqb.dev_control;

                pndv_data.diag.cm_dev_req_running = PNDV_CM_DEV_REQ_DEVICE_CONTROL;

                PNDV_CM_REQUEST( tmp_rqb_ptr);

                break;
            default:
                pndv_in_fatal_error( PNDV_MODULE, __LINE__, pndv_data.sm);
                break;
        }
    }
    else
    {
        pndv_data.diag.cm_dev_req.pending |= PNDV_CM_DEV_REQ_DEVICE_CONTROL;
    }
}

/**
 *  @brief handles the device control response
 *
 *  @param[in]  rqb_ptr Pointer to request block
 *
 *  Handles the response to the CM_OPC_SV_DEVICE_CONTROL request.
 *
 */
void pndv_in_cm_sv_device_control_done (CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    PNDV_IFACE_CMD_ENTRY_T tmp_event;

    UnsignedOpt response;

    if ( pndv_data.device_control == CM_SV_DEVICE_CONTROL_CMD_ACTIVATE )
    {
        response = PNDV_RQB_GET_RESPONSE( rqb_ptr);

        if ( CM_OK != response )
        {
            pndv_in_fatal_error( PNDV_MODULE, __LINE__, response);
        }

        pndv_data.diag.cm_dev_req_running = 0;

        /* pp-req prevented due to running dev_req (pndv_data.diag.cm_dev_req_running). */

        // indicate device aktivation to peri
        tmp_event.cmd = PNDV_EV_TO_PERI_CM_DV_AKT_IND;
        pndv_in_peri_write_coupl_event( tmp_event );

        #ifdef PNDV_CFG_ENABLE_R1
            pndv_r1_bridge_start();
        #endif

        /* cm_dev_req_running ist 0 -> evtl. andere dev req wdh. */
        pndv_in_cm_check_dev_req();
    }
    else
    {
         response = PNDV_RQB_GET_RESPONSE( rqb_ptr);

         if ( CM_OK != response )
         {
             pndv_in_fatal_error( PNDV_MODULE, __LINE__, response);
         }

         pndv_data.diag.cm_dev_req_running = 0;

         // indicate device aktivation to peri
         tmp_event.cmd = PNDV_EV_TO_PERI_CM_DV_DEAKT_IND;
         pndv_in_peri_write_coupl_event( tmp_event );
    }
}



/**
 *  @brief handler for ar control
 *
 *  @param[in]  rqb_ptr Pointer to request block
 *
 *  Handles the response to CM_OPC_SV_AR_CONTROL.
 *  The request is not used at the moment.
 *  It is obsolet as the EDD-DataStatus-Api is used
 *  to control the data state.
 *
 */
void pndv_in_cm_sv_ar_control_done (CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    /* my change of APDU state done */

    UnsignedOpt response;

    response = PNDV_RQB_GET_RESPONSE( rqb_ptr);

    if (   (CM_OK           != response)
        && (CM_ERR_SESSION  != response)
       )
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, response);
    }
}

/**
 *  @brief Handler for apdu status indications
 *
 *  @param[in]  rqb_ptr Pointer to request block
 *
 *  Handles incomming  ar data status indications (CM_OPC_SV_AR_DATA_STATUS_IND).
 *  Indication is responded without evaluation. APDU state changes are detected in
 *  another way.
 *
 */
void pndv_in_cm_sv_event_apdu_status_ind  (CM_UPPER_RQB_PTR_TYPE rqb_ptr)
{
    /* APDU state was changed from external */

    UnsignedOpt  response;

    response = PNDV_RQB_GET_RESPONSE( rqb_ptr);

    if ( CM_OK != response)
    {
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, response);
    }

    /* apdu state will be checked with synchroneousely incoming output data */
    /* here at this point the apdu state is ignored */

    PNDV_RQB_SET_OPCODE( rqb_ptr, CM_OPC_SV_AR_DATA_STATUS_RSP);
}

#ifdef PNDV_CFG_USE_DEVICE_READ
void pndv_cm_in_device_read_req(PNDV_RQB_PTR_TYPE rqb_ptr)
{
    CM_UPPER_RQB_PTR_TYPE local_rqb_ptr;

    if (PNDV_RQB_GET_OPCODE(rqb_ptr) == PNDV_OPC_DEVICE_READ)
    {

        if (pndv_data.usr.dev_read.service_running != TRUE)
        {

            pndv_data.usr.dev_read.service_running = TRUE;
            pndv_data.usr.dev_read.usr_rqb         = rqb_ptr;

            PNDV_ALLOC_MEM(&local_rqb_ptr, sizeof(CM_RQB_TYPE));
            if(local_rqb_ptr == 0)
            {
                /* no memory */
                pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
            }
            PNDV_ALLOC_MEM(&local_rqb_ptr->args.sv.dev_read, sizeof(CM_SV_DEVICE_READ_TYPE))
            if(local_rqb_ptr->args.sv.dev_read == 0)
            {
                /* no memory */
                pndv_in_fatal_error( PNDV_MODULE, __LINE__, 0);
            }

            local_rqb_ptr->args.sv.dev_read->device_nr          = rqb_ptr->args->dev_read.device_nr; /* device number */

            local_rqb_ptr->args.sv.dev_read->data_length        = rqb_ptr->args->dev_read.data_length ; /* req: size of 'data' (including CM_RECORD_OFFSET), cnf: CM_RECORD_OFFSET +number of bytes read */
            local_rqb_ptr->args.sv.dev_read->data               = rqb_ptr->args->dev_read.data;

            local_rqb_ptr->args.sv.dev_read->api                = rqb_ptr->args->dev_read.api;
            local_rqb_ptr->args.sv.dev_read->slot_nr            = rqb_ptr->args->dev_read.slot_nr;
            local_rqb_ptr->args.sv.dev_read->subslot_nr         = rqb_ptr->args->dev_read.subslot_nr;
            local_rqb_ptr->args.sv.dev_read->record_index       = rqb_ptr->args->dev_read.record_index;  /* see PNIO-Spec */
            local_rqb_ptr->args.sv.dev_read->target_ar_uuid     = rqb_ptr->args->dev_read.target_ar_uuid;

            local_rqb_ptr->args.sv.dev_read->pnio_status        = 0; /* cnf: pnio error status, see PNIO-Spec */

            PNDV_RQB_SET_HANDLE( local_rqb_ptr, pndv_data.cm_handle[PNDV_INDEX_PATH_ACP_CM]);
            PNDV_RQB_SET_OPCODE( local_rqb_ptr, CM_OPC_SV_DEVICE_READ);

            PNDV_CM_REQUEST( local_rqb_ptr);
        }
        else
        {
            /* still another request running */
            PNDV_RQB_SET_RESPONSE(rqb_ptr, PNDV_ERR_SEQUENCE);

            PNDV_CFG_USR_REQ_DONE(rqb_ptr);
        }
    }
    else
    {
        /* wrong opcode */
        pndv_in_fatal_error( PNDV_MODULE, __LINE__, PNDV_RQB_GET_OPCODE(rqb_ptr));
    }

}
#endif

/*****************************************************************************/
/*  end of file.                                                             */
/*****************************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
