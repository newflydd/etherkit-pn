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
/*  F i l e               &F: ecos_ertec_os_utils.c                     :F&  */
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
#include "compiler.h"

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_POSIX_EB200P )
    #include "compiler_stdlibs.h"
    #include "os.h"
    #include "os_cfg.h"
    #include "os_taskprio.h"
    #include "pniousrd.h"
    #include "edd_inc.h"
    #include "nv_data.h"
    #include "trc_if.h"
    #include "lsa_cfg.h"
    #include "lsa_sys.h"
    #include "tskma_cfg.h"
    #include "logadapt.h"
    #include "usriod_cfg.h"

    #include <pthread.h>
    #include <semaphore.h>
    #include <signal.h>
    #include <time.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <signal.h>
    #include <errno.h>
    #include <mqueue.h>
    #include <math.h>
    #include <unistd.h>
    #include <sys/types.h>

    #include "posix_os.h"
    #include "posix_dat.h"

    #include "ecos_ertec_os_utils.h"

    #include "ertec200p_reg.h"
    #include "tskma_int.h"
    #include "edd_usr.h"
    #include "ertec_inc.h"

    #include <cyg/kernel/kapi.h>

    OS_CODE_FAST void OsIntDisable()
    {
       cyg_interrupt_disable();
    }

    OS_CODE_FAST void OsIntEnable()
    {
       cyg_interrupt_enable();
    }

    void OsReboot (void)
    {

        EDD_RQB_TYPE  *pReBootEvent;
        OsAlloc ((void**) &pReBootEvent, 0, sizeof (EDD_RQB_TYPE));

        //Reboot functionality executed in EDD task context
        LSAS_RQB_SET_OPCODE  ( pReBootEvent, TSKMA_OPC_EDD_REQ_REBOOT );
        LSAS_RQB_EDD_REQUEST ( pReBootEvent, tskma_task_edd_request   );

        //Reboot in EDD task - nothing more to do here
        while (1) {}

    }


    void OsRebootService (void)
    {

        OsWait_ms (500); //time for console output
        cyg_interrupt_disable();
        lsas_com_shutdown_network();
        // * ------------------------------------------------
        // * bit 1: resets all IPs except PN-IP and KRISC32
        // * bit 2: resets only PNIP and KRISC
        // * bit 5: resets KRISC32 core system
        // * bit 6: resets ARM926EJ-S core system
        // * bit 7: resets SDMMC module
        // * bits 31-16: duration of the reset pulse
        // * ------------------------------------------------
        *((LSA_UINT32*) U_SCRB__ASYN_RES_CTRL_REG) = 0x80000006;
        while (1)
        {
            OsWait_ms (500);
            PNIO_printf ("still alive...\n");   // should not occur...
        }

     }
#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
