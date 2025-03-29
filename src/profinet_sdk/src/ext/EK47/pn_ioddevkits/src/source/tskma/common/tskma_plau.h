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
/*  F i l e               &F: tskma_plau.h                              :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*   TSKMA plausibilty checks                                        		 */
/*                                                                           */
/*****************************************************************************/


#ifndef TSKMA_PLAU_H_
#define TSKMA_PLAU_H_

/* check generall setup plausibilty */
#ifdef TSKMA_USE_STATIC_OS
    #ifdef TSKMA_OS_TASK_CREATE
        #ifndef TSKMA_OS_NO_DYN_WARN
            #warning Dynamic OS macros used with Static OS, this is not intended to be and may be an error. \
                     To suppress this warning define TSKMA_OS_NO_DYN_WARN in your config file.
        #endif
    #endif
#endif

/*
 * check PRIO settings or set set default values
 *
 */

#ifndef TSKMA_TASK_PRIO_APP
    #define TSKMA_TASK_PRIO_APP                    0 /* application task */
#endif
/**********************************************************************************
 * !!! IMPORTANT: all tasks below the application task are ts tasks if enabled!!! *
 **********************************************************************************/
#ifndef TSKMA_TASK_PRIO_EDD
    #define TSKMA_TASK_PRIO_EDD                    1 /* EDD task */
#endif
#ifndef TSKMA_TASK_PRIO_ORG
    #define TSKMA_TASK_PRIO_ORG                    2 /* EDD-ORG task */
#endif
#ifndef TSKMA_TASK_PRIO_PNO
    #define TSKMA_TASK_PRIO_PNO                    3 /* pnio task  */
#endif
#ifndef TSKMA_TASK_PRIO_TCP
    #define TSKMA_TASK_PRIO_TCP                    4 /* TCP/IP task */
#endif
#ifndef TSKMA_TASK_PRIO_POF
    #define TSKMA_TASK_PRIO_POF                    5 /* POF task */
#endif
#ifndef TSKMA_TASK_PRIO_REM
    #define TSKMA_TASK_PRIO_REM                    6 /* Remanence task */
#endif
#ifndef TSKMA_TASK_PRIO_STP
    #define TSKMA_TASK_PRIO_STP                    7 /* startup task */
#endif
#ifndef TSKMA_TASK_PRIO_IDL
    #define TSKMA_TASK_PRIO_IDL                    8 /* idle task */
#endif

#ifndef TSKMA_FATAL
#define TSKMA_FATAL(ec_0)       tskma_fatal (TSKMA_MODULE_ID, __LINE__, ec_0)
#endif

#ifndef TSKMA_CHECK_FATAL
#define TSKMA_CHECK_FATAL(ec_0) if (LSA_OK != ec_0) tskma_fatal (TSKMA_MODULE_ID, __LINE__, ec_0)
#endif

#ifndef TSKMA_ASSERT
#define TSKMA_ASSERT(cond)      if(!(cond)) tskma_fatal (TSKMA_MODULE_ID, __LINE__, 0)
#endif

#ifndef TSKMA_INSTALL_INT_HANDLER
    #define TSKMA_INSTALL_INT_HANDLER()
#endif

#ifndef TSKMA_START_WATCHDOG
    #define TSKMA_START_WATCHDOG()
#endif

#endif /* TSKMA_PLAU_H_ */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
