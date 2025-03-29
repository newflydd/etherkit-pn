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
/*  F i l e               &F: main_posix.c                              :F&  */
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
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_POSIX_EB200P)

    #include "compiler_stdlibs.h"
    #include "lsa_cfg.h"
    #include "os.h"
    #include "os_taskprio.h"
    #include "pniousrd.h"
	#include <cyg/hal/ertec200p_reg.h>

    extern void MainAppl   (void);

    LSA_UINT32 TskId_Main;

    int main ( void )
    {
        PNIO_UINT32 Status;

        // *------------------------------------------------*
        // *  initialize PNIO stack
        // *------------------------------------------------*
        PNIO_init ();

        #ifdef INCLUDE_PERFORMANCE_MEASURE
		#include "perform_measure.h"
        // *----------------------------------------------------------
        // * Do the reference performance measure before starting the
        // * pnio stack. This is a test-option only and not necessary
        // * for PNIO.
        // *----------------------------------------------------------
        {
            Status = InitPerformanceMeasure ();
            if (Status != LSA_OK)
            {
                PNIO_printf ("error in InitPerformanceMeasure()\n");
            }
        }
        #endif // INCLUDE_PERFORMANCE_MEASURE

        // *------------------------------------------------*
        // *  create and start PROFINET user application
        // *------------------------------------------------*
        Status = OsCreateThread (MainAppl, (PNIO_UINT8*)"MainAppl", TASK_PRIO_MAIN, &TskId_Main);
        if (Status != PNIO_OK)
        {
            printf ("error OsCreateThread\n");
        }

        Status = OsCreateMsgQueue (TskId_Main);
        if (Status != PNIO_OK)
        {
            printf ("error OsCreateThread\n");
        }

        Status = OsStartThread (TskId_Main);
        if (Status != PNIO_OK)
        {
            printf ("error OsCreateThread\n");
        }

        while (1)/*lint !e716 while(1) */
        {
            OsWait_ms(1000); // for debug
        }
        return (0); /*lint !e527 Unreachable code */
    }

#endif

 
/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
