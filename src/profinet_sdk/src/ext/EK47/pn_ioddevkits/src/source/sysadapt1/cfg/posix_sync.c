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
/*  F i l e               &F: posix_sync.c                              :F&  */
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
 * @file     posix_sync.c
 * @brief    functions for synchronization handling
 *
 * interface for synchronization handling
 *
 */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*                                                                           */
/*  Date        Version    What                                              */
/*---------------------------------------------------------------------------*/

#include "compiler.h"
#if (INCLUDE_POSIX_INTERFACE)
    #include "compiler_stdlibs.h"
    #include "os.h"
    #include "os_cfg.h"
    #include "os_taskprio.h"
    #include "pniousrd.h"
    #include "nv_data.h"
    #include "trc_if.h"
    #include "lsa_cfg.h"
    #include "lsa_sys.h"
    #include "tskma_cfg.h"
    #include "logadapt.h"
    #include "usriod_cfg.h"

    #include "posix_os.h"
    #include "posix_dat.h"

    /**
     *  @brief Create and Init semaphore
     *
     *  @param[in]      PNIO_VOID *     address of created semahore
     *  @param[in]      PNIO_BOOL       settings for semaphore, shared between processes
     *  @param[in]      PNIO_UINT32     initial value of semaphore
     *
     *  @return         PNIO_UINT32     PNIO_OK     semaphore was created
     *                                  PNIO_NOT_OK semaphore was not created
     */
    PNIO_UINT32 OsCreateSem( PNIO_VOID *pSem, PNIO_BOOL shared, PNIO_UINT32 value )
    {
        PNIO_UINT32 ret;

        ret = sem_init ( (sem_t *) pSem, shared, value);

        if (ret != 0)
        {
            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "sem_init failed with %d\n", ret);
            return (PNIO_NOT_OK);
        }

        return (PNIO_OK);
    }

    /**
     * @brief alloc semaphore
     *
     * @param[in]      PNIO_UINT32     id of semaphore
     *
     * @return         PNIO_UINT32      PNIO_OK     semaphore was allocated
     *                                  PNIO_NOT_OK semaphore was not allocated
     *
     */
    PNIO_UINT32 OsAllocSemB (PNIO_UINT32 *pSemId)
    {
        int i;

        OS_ASSERT (OsInitialized == PNIO_TRUE);

        OsIntDisable();
        for (i = 0; i < MAXNUM_OF_POSIX_SEMAPH; i++)
        {
            if (Semaph[i].Allocated == LSA_FALSE)
            {
                Semaph[i].Allocated = LSA_TRUE;
                OsIntEnable();
                *pSemId = i;
                return (PNIO_OK);
            }
        }

        OsIntEnable();
        LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "no more semaphore ressources\n");
        return (PNIO_NOT_OK);
    }

    /**
     * @brief take semaphore
     *
     * @param[in]      PNIO_UINT32     id of semaphore
     *
     * @return         PNIO_UINT32      PNIO_OK     semaphore was taken
     *
     */
    OS_CODE_FAST PNIO_UINT32 OsTakeSemB (PNIO_UINT32 SemId)
    {
        PNIO_UINT32 Status;
        Status = sem_wait (&Semaph[SemId].SemHndl);
        if (Status)
        {
            LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "OsTakeSemB\n");
        }
        return (PNIO_OK);
    }

    /**
     * @brief give semaphore
     *
     * @param[in]     PNIO_UINT32     id of semaphore
     *
     * @RETURN         PNIO_UINT32      PNIO_OK     semaphore was given
     *
     */
    OS_CODE_FAST PNIO_UINT32 OsGiveSemB (PNIO_UINT32 SemId)
    {
        PNIO_UINT32 Status;
        Status = sem_post (&Semaph[SemId].SemHndl);
        if (Status)
            LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "OsGiveSemB\n");

        return (PNIO_OK);
    }

    /**
     * @brief free semaphore, still will be in memory
     *
     * @param[in]   PNIO_UINT32     id of semaphore
     *
     * @return      PNIO_UINT32      PNIO_OK     semaphore was released
     *
     */
    PNIO_UINT32 OsFreeSemB (PNIO_UINT32 SemId)
    {
        OS_ASSERT (OsInitialized == PNIO_TRUE);

        Semaph[SemId].Allocated = LSA_FALSE;
        return (PNIO_OK);
    }

    /**
     * @brief Create mutex with property : INHERIT protocol and ERRORCHECK behavior
     *
     * @param[in]   PNIO_VOID *     addr of created mutex
     *
     * @return    PNIO_UINT32     PNIO_OK     mutex was created
     *                              PNIO_NOT_OK mutex was not created
     */
    PNIO_UINT32 OsCreateMutex ( PNIO_VOID * addr )
    {
        PNIO_UINT32 ret;
        pthread_mutexattr_t mattr;

        ret = pthread_mutexattr_init(&mattr);
        if (ret != 0)
        {
            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "pthread_mutexattr_init failed with %d\n", ret );
        }

        ret = pthread_mutexattr_setprotocol (&mattr, PTHREAD_PRIO_INHERIT );
        if (ret != 0)
        {
            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "pthread_mutexattr_setprotocol failed with %d\n", ret );
        }

        ret = pthread_mutexattr_settype (&mattr, PTHREAD_MUTEX_ERRORCHECK );
        if (ret != 0)
        {
            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "pthread_mutexattr_settype failed with %d\n", ret );
        }

        ret = pthread_mutex_init ( (pthread_mutex_t * ) addr, &mattr);

        if (ret != 0)
        {
            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "pthread_mutex_init failed with %d\n", ret );
        }

        return (PNIO_OK);
    }

    /**
     * @brief lock mutex
     *
     * @param[in]   PNIO_UINT32     id of mutex
     *
     * @return         void
     *
     */
    OS_CODE_FAST void OsEnterX(PNIO_UINT32 MutexId)
    {
        PNIO_INT32 ret = 0;

        OS_ASSERT (OsInitialized == PNIO_TRUE);

        if (MutexId >= MAXNUM_OF_NAMED_MUTEXES)
        {
            LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "Error OsEnterX (): invalid MutexId\n");
        }
        else
        {
            if ( ( ret = pthread_mutex_trylock(&StatMutex[MutexId].Id) ) != 0)
            {
                if ( ret == EBUSY )
                {
                    if ( ( ret = pthread_mutex_lock (&StatMutex[MutexId].Id) ) != 0)
                    {
                        LSA_TRACE_04  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "%d:%d lock sync error = ret : %x, %x\n\n", MutexId, StatMutex[MutexId].num, ret, pthread_self());
                    }
                }
                else
                {
                    LSA_TRACE_04  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "%d:%d lock sync error = ret : %x, %x\n\n", MutexId, StatMutex[MutexId].num, ret, pthread_self());
                }
            }
        }
    }

    /**
     * @brief unlock mutex
     *
     * @param[in]   PNIO_UINT32     id of mutex
     *
     * @return         void
     *
     */
    OS_CODE_FAST void OsExitX (PNIO_UINT32 MutexId)
    {
        PNIO_INT32 ret = 0;

        OS_ASSERT (OsInitialized == PNIO_TRUE);

        if (MutexId >= MAXNUM_OF_NAMED_MUTEXES)
        {
            LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "Error OsEnterX (): invalid MutexId\n");
        }
        else
        {
            if ( ( ret = pthread_mutex_unlock (&StatMutex[MutexId].Id) ) != 0 )
            {
                LSA_TRACE_04  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "%d:%d unlock sync error = ret : %x, %x\n\n", MutexId, StatMutex[MutexId].num, ret, pthread_self());
            }
        }
    }

    /**
     * @brief lock default mutex
     *
     * @return         void
     *
     */
    OS_CODE_FAST void OsEnter(void)
    {
        OsEnterX (OS_MUTEX_DEFAULT);
    }

    /**
     * @brief unlock default mutex
     *
     * @return         void
     *
     */
    OS_CODE_FAST void OsExit (void)
    {
        OsExitX (OS_MUTEX_DEFAULT);
    }

    /**
     * @brief lock default mutex
     *
     * @return void
     *
     */
    OS_CODE_FAST void OsEnterShort(void)
    {
        OsEnter();
    }

    /**
     * @brief unlock default mutex
     *
     * @return void
     *
     */
    OS_CODE_FAST void OsExitShort (void)
    {
        OsExit();
    }
#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
