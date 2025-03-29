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
/*  F i l e               &F: pndv_cfg.c                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  system adaption of this component                                        */
/*                                                                           */
/*****************************************************************************/

/* ****************************************************************************
*
*   H i s t o r y :
*   see /_doc/im151_3pn_hist.doc
*
* ****************************************************************************/


/*****************************************************************************/
/* contents:

    - pndv_maint_led_control
    - pndv_check_maint_info



*/

/*****************************************************************************/
/* 2do:


*/

/*****************************************************************************/
/* include hierarchy */

#define LSAS_MODULE_ID  25

//#include "pndv_inc.h"
#include "lsas_inc.h"
#include "lsas_int.h"

//#include "trace_cfg.h"
//#include "pndv_com.h"

#define PNDV_MAINT_LED  3


void pndv_maint_led_control( LSA_UINT32 sf_led_mode)
{
    /* Abhaengigkeit der Maint-LED von der SF-LED                       */
    /* Besonderheit ecoPN: Maint-LED und SF-LED unter einem Lichtleiter */
    /* es darf immer nur eine leuchten -> SF-LED hat Prioritaet         */

    if ( LSA_TRUE == sf_led_mode )
    {
        if ( LSA_TRUE == pndv_glob_data.save_maint_on )
        {
            pndv_glob_data.maint_on = LSA_FALSE;

            pndv_debug_extern( 0xF0 | PNDV_MAINT_LED, 0 );

            PNDV_LED_CTRL_MAINT_OFF();
        }
    }
    else
    {
        /* wenn SF-Ereignis verschwindet und Maintenance noch anliegt, muss diese auf ON gehen */
        if ( LSA_TRUE == pndv_glob_data.save_maint_on )
        {
            pndv_glob_data.maint_on = LSA_TRUE;

            pndv_debug_extern( 0xF0 | PNDV_MAINT_LED, 1 );

            PNDV_LED_CTRL_MAINT_ON();
        }
    }
}


/*****************************************************************************/


void pndv_check_maint_info( void )
{
    /* Maintenance Verhalten nach SPH PDEV V2.0i09 */

    switch ( pndv_glob_data.led_maint_info & ~CM_MAINTENANCE_STATUS_QUA_BITS )
    {
        case CM_MAINTENANCE_STATUS_DEMANDED:
//TODO: -OS- reinsert next line
        //case (CM_MAINTENANCE_STATUS_DEMANDED | CM_MAINTENANCE_STATUS_REQUIRED):
        case ~CM_MAINTENANCE_STATUS_QUA_BITS:
        {
            /* maintenance demanded -> MAINT-LED ON */

            pndv_glob_data.save_maint_on = TRUE;

            if ( TRUE != pndv_glob_data.maint_on )
            {
                pndv_glob_data.maint_on = TRUE;

                pndv_debug_extern( 0xF0 | PNDV_MAINT_LED, 1 );

                PNDV_LED_CTRL_MAINT_ON();
            }

            break;
        }
        default :
        {
            /* maintenance required = CM_MAINTENANCE_STATUS_REQUIRED -> MAINT-LED OFF */
            /* no maintenance       = 0                              -> MAINT-LED OFF */

            pndv_glob_data.save_maint_on = FALSE;

            if ( FALSE != pndv_glob_data.maint_on )
            {
                pndv_glob_data.maint_on = FALSE;

                pndv_debug_extern( 0xF0 | PNDV_MAINT_LED, 0 );

                PNDV_LED_CTRL_MAINT_OFF();
            }

            break;
        }
    }
}

#ifndef PNDV_CM_OPEN_CHANNEL_LOWER
void PNDV_CM_OPEN_CHANNEL_LOWER(CM_UPPER_RQB_PTR_TYPE _rqb)
{
    _rqb->args.channel.open->cm_request_upper_done_ptr
                = lsas_cm_to_appl_cbf;

    LSAS_RQB_CM_REQUEST(_rqb, cm_open_channel);
}
#endif

/*****************************************************************************/
/*  end of file.                                                             */
/*****************************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
