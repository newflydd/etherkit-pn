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
/*  F i l e               &F: posix_memory.c                            :F&  */
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
 * @file     posix_print.c
 * @brief    functions for process memory handling
 *
 * interface for process memory handling
 *
 */
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*  interface for process memory handling                                    */
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
     * @brief allocate memory
     *
     * @param[out]          void**           pointer for allocated pointer to memory
     * @param[in]          PNIO_UINT32      length of memory
     * @param[in]          PNIO_UINT32      pool where memory will be allocated ( not currently used )
     *
     * @return         PNIO_UINT32      LSA_RET_OK              memory was freed
     *                                  LSA_RET_ERR_RESOURCE    no resources to allocate memory
     *                                  PNIO_NOT_OK             incorrect parameters
     *
     */
    PNIO_UINT32 OsAllocFX ( void** ppMem, PNIO_UINT32 Length, PNIO_UINT32 PoolId)
    {
        LSA_UNUSED_ARG (PoolId);

        OS_ASSERT (OsInitialized == PNIO_TRUE);

        if (Length == 0)
         {
              LSA_TRACE_02  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "Alloc: pMem = 0x%x, Length = %d\n", *ppMem, Length);
             *ppMem = 0;

             return (PNIO_NOT_OK);
         }

        *ppMem = malloc(Length);

        if(LSA_NULL == *ppMem)
        {
            return LSA_RET_ERR_RESOURCE;
        }

        return LSA_RET_OK;
    }

    /**
     * @brief allocate memory
     *
     * @param[out]          void**           pointer for allocated pointer to memory
     * @param[in]          PNIO_UINT32      length of memory
     *
     * @return        PNIO_UINT32      LSA_RET_OK              memory was freed
     *                                  LSA_RET_ERR_RESOURCE    no resources to allocate memory
     *                                  PNIO_NOT_OK             incorrect parameters
     *
     */
    PNIO_UINT32 OsAllocF (void** ppMem, PNIO_UINT32 Length)
    {
         OS_ASSERT (OsInitialized == PNIO_TRUE);

         if (Length == 0)
          {
               LSA_TRACE_02  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "Alloc: pMem = 0x%x, Length = %d\n", *ppMem, Length);
              *ppMem = 0;

              return PNIO_NOT_OK;
          }

         *ppMem = malloc(Length);

         if(LSA_NULL == *ppMem)
         {
             return LSA_RET_ERR_RESOURCE;
         }

         return LSA_RET_OK;
    }


    /**
     * @brief allocate memory and fill memory with value
     *
     * @param[out]          void**           pointer for allocated pointer to memory
     * @param[in]          unsigned char    value which allocated memory will be filled
     * @param[in]          PNIO_UINT32      length of memory
     * @param[in]          PNIO_UINT32      pool where memory will be allocated ( not currently used )
     *
     * @return         PNIO_UINT32      LSA_RET_OK              memory was freed
     *                                  LSA_RET_ERR_RESOURCE    no resources to allocate memory
     *                                  PNIO_NOT_OK             incorrect parameters
     *
     */
    PNIO_UINT32 OsAllocX (void** ppMem, PNIO_UINT8 Value, PNIO_UINT32 Length, PNIO_UINT32 PoolId )
    {
        LSA_UNUSED_ARG (PoolId);

        if (Length == 0)
        {
            LSA_TRACE_03  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "Alloc: pMem = 0x%x, Length = %d, Pool = %d\n", *ppMem, Length, PoolId);
            *ppMem = 0;
            return (PNIO_NOT_OK);
        }

        *ppMem = malloc(Length);

        if(LSA_NULL == *ppMem)
        {
            return (LSA_RET_ERR_RESOURCE);
        }

        OsMemSet (*ppMem, (PNIO_UINT32) Value, Length);

        return (LSA_RET_OK);
    }

    /**
     * @brief allocate memory and fill memory with value
     *
     * @param[out]          void**           pointer for allocated pointer to memory
     * @param[in]          unsigned char    value which allocated memory will be filled
     * @param[in]          PNIO_UINT32      length of memory
     *
     * @return         PNIO_UINT32      LSA_RET_OK              memory was freed
     *                                  LSA_RET_ERR_RESOURCE    no resources to allocate memory
     *                                  PNIO_NOT_OK             incorrect parameters
     *
     */
    PNIO_UINT32 OsAlloc ( void** ppMem, unsigned char Value, PNIO_UINT32 Length )
    {
        if (Length == 0)
        {
            LSA_TRACE_02  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "Alloc: pMem = 0x%x, Length = %d \n", *ppMem, Length );
            *ppMem = 0;
            return (PNIO_NOT_OK);
        }

        *ppMem = malloc(Length);

        if(LSA_NULL == *ppMem)
        {
            return (LSA_RET_ERR_RESOURCE);
        }

        OsMemSet (*ppMem, (PNIO_UINT32) Value, Length);

        return (LSA_RET_OK);
    }

    /**
     * @brief Free allocated memory
     *
     * @param[in]         void*            pointer to memory
     * @param[in]          PNIO_UINT32      pool where memory is allocated ( not currently used )
     *
     * @return         PNIO_UINT32      LSA_RET_OK      memory was freed
     *
     */
    PNIO_UINT32 OsFreeX (void* pMem, PNIO_UINT32 PoolId)
    {
        LSA_UNUSED_ARG (PoolId);
        OS_ASSERT (OsInitialized == PNIO_TRUE);

        if (pMem)
        {
           free(pMem);
        }

        return (LSA_RET_OK);
    }

    /**
     * @brief Free allocated memory
     *
     * @param[in]          void*            pointer to memory
     *
     * @return         PNIO_UINT32      LSA_RET_OK      memory was freed
     */
    PNIO_UINT32 OsFree (void* pMem)
    {
        OS_ASSERT (OsInitialized == PNIO_TRUE);

        if (pMem)
        {
           free(pMem);
        }

        return (LSA_RET_OK);
    }

    /**
     * @brief test of valid pointer
     *
     * @param[in]           PNIO_VOID*    pointer to memory
     *
     * @return         PNIO_VOID*     PNIO_OK         pointer to memory
     *
     */
    PNIO_VOID*  OsTestPointer(void * ptr)
    {
        if( ptr != 0 )
        { /* especially 0xCCCCCCCC */
            if ((PNIO_INT32) ptr >= 0x80000000L)
                LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "invalid pointer\n");
        }
        return (ptr);
    }
#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
