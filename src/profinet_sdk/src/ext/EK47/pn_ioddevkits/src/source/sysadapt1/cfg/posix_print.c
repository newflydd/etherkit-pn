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
/*  F i l e               &F: posix_print.c                             :F&  */
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
 * @brief    functions for Terminal IO operation
 *
 * interface for stdin and stdout handling
 *
 */

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*                                                                           */
/*  Date        Version    What                                              */
/*---------------------------------------------------------------------------*/

#include "compiler.h"

#define USE_DEF_VA_LIST     1

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

#define OS_PRINT_INDEXES	200
    static PNIO_UINT8  os_print_buffer[OS_PRINT_INDEXES][512] __attribute__ ((section(".uncached_mem")));
    static PNIO_UINT32 os_print_read_index = 0;
    static PNIO_UINT32 os_print_write_index = 0;
    /**
     * @name posix terminal IO operations
     * @{ */

    /**
     * @brief print send char array in variable list by message to print thread
     *
     * @param[in]   fmt         PNIO_INT8*       text which should be sent to pBuf
     * @param[in]   argptr      PNIO_va_list     variable list
     *
     * @return  void
     *
     */
    static void OsPrint (PNIO_INT8 * fmt, PNIO_va_list argptr)
    {
            PNIO_INT32 Len = 0;
            PNIO_UINT8* pConsBuf = LSA_NULL;

            if (PrintPostQueueEnable)
            {
                    pConsBuf = os_print_buffer[os_print_write_index];
                    os_print_write_index += 1;
                    if ( os_print_write_index >= OS_PRINT_INDEXES)
                    {
                        os_print_write_index = 0;
                    }

                    if ( pConsBuf )
                    {
                        Len  = vsnprintf( (char*) pConsBuf, 500, fmt, argptr);

                        if (Len > 500)
                        {
                            if ( printf ("WARNING: OsPrint() message to long (%d bytes) \n", (int) Len) < 0 )
                                LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "error printf caused by : %d\n", errno );

                            *(pConsBuf+500-1) = 0;    // terminate with zero
                        }
                        else if ( Len < 0 )
                        {
                            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "error printf caused by : %d\n", errno );
                            return;
                        }
                        else
                        {
                            *(pConsBuf+Len) = 0;    // terminate with zero
                        }

                        OsSendMessage (TskId_PrintPost, (void*)pConsBuf, OS_MBX_PRIO_NORM);
                    }
                    else
                    {
                        LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "error printf caused by : %d\n", errno );
                    }
                }
    }

    /**
     * @brief print char array with to stdout
     *
     * @param[in]   fmt         PNIO_INT8*       text which should be sent to pBuf
     *
     * @return  void
     *
     */
    void PNIO_printf(PNIO_INT8 * fmt, ...)
    {
        switch (PnioLogDest)
        {
            case 0:
                    break;
            case 1:
                    {
                        va_list argptr = {0};
                        va_start(argptr, fmt);
                        OsPrint (fmt, argptr);
                        va_end(argptr);
                    }
                    break;
            case 2:
                    {
                        va_list argptr = {0};
                        va_start(argptr, fmt);
                        OsPrint (fmt, argptr);
                        //TRACE_PRINT (fmt, (PNIO_va_list)argptr);
                        va_end(argptr);
                    }
                    break;
            #if INCLUDE_LOGADAPT
            case 3:
                    {
                        va_list argptr = {0};
                        va_start(argptr, fmt);
                        LogAdaptConsPrint (fmt, argptr);
                        va_end(argptr);
                    }
                    break;
            #endif
            default:
            {

            }
        }
    }

    /**
     * @brief print char array of console terminal with to stdout
     *
     * @param[in]   fmt         PNIO_INT8*       text which should be sent to pBuf
     *
     * @return  void
     *
     */
    void PNIO_ConsoleLog(PNIO_INT8 * fmt, ...)
    {
        if (PnioLogDest == 1)
        {
            va_list argptr = {0};

            va_start(argptr, fmt);

            OsPrint (fmt, argptr);

            va_end(argptr);
        }
    }

    /**
     * @brief print char array of traces with to stdout
     *
     * @param[in]   fmt         PNIO_INT8*       text which should be sent to pBuf
     *
     * @return  void
     *
     */
    void PNIO_TrcPrintf(PNIO_INT8 * fmt, ...)
    {
        switch (PnioLogDest)
        {
            case 0:
                    break;
            case 1: // **** print on stdout ****
                    {
                        va_list argptr = {0};
                        va_start(argptr, fmt);
                        OsPrint (fmt, argptr);
                        va_end (argptr);
                    }
                    break;
            case 2: // **** print into tracebuffer memory ****
                    {
                        va_list argptr = {0};
                        va_start(argptr, fmt);
                        OsPrint (fmt, argptr);
                        //TRACE_PRINT (fmt, argptr);
                        va_end (argptr);
                    }
                    break;
            #if INCLUDE_LOGADAPT
                case 3:
                {
                    PNIO_va_list argptr;
                    va_start(argptr, fmt);
                    LogAdaptTrcPrint (fmt, argptr);
                    va_end(argptr);
                }
                break;
            #endif

            default:
                    break;

        }
    }

    /**
     * @brief print to char array with char array from variable list
     *
     * @param[out]  pBuf        PNIO_UINT8*      buffer which should be filled by this function
     * @param[in]   count       PNIO_UINT32      length of output buffer
     * @param[in]   fmt         PNIO_INT8*       text which should be sent to pBuf
     * @param[in]   argptr      PNIO_va_list     variable list
     *
     * @return  PNIO_UINT32                  transmitted number of bytes
     *
     */
    PNIO_UINT32 PNIO_vsnprintf(PNIO_UINT8* pBuf, PNIO_UINT32 count, PNIO_INT8* fmt, PNIO_va_list argptr)
    {
        PNIO_INT32 ret;

        ret = vsnprintf ( (char *) pBuf, count, (const char * ) fmt, argptr);

        if (ret < 0)
            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "error vsprintf caused by : %d\n", errno );

        return ( (PNIO_UINT32) ret );
    }

    /**
     * @brief print to char array with char array from variable list
     *
     * @param[out]   pBuf       PNIO_UINT8*      buffer which should be filled by this function
     * @param[in]   fmt         PNIO_INT8*       text which should be sent to pBuf
     * @param[in]   argptr      PNIO_va_list     variable list
     *
     * @return  PNIO_UINT32                  transmitted number of bytes
     *
     */
    PNIO_UINT32 PNIO_vsprintf(PNIO_UINT8* pBuf, PNIO_INT8* fmt, PNIO_va_list argptr)
    {
        PNIO_INT32 ret;

        ret = vsprintf ( (char *) pBuf, (const char * ) fmt, argptr);

        if (ret < 0)
            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "error vsprintf caused by : %d\n", errno );

        return ( (PNIO_UINT32) ret );
    }

    /**
     * @brief print to char array with char array from variable list limited by count
     *
     * @param[out]   pBuf       PNIO_UINT8*    buffer which should be filled by this function
     * @param[in]   count       PNIO_UINT32    count of bytes which will be transmitted
     * @param[in]   fmt         PNIO_INT8*     text which should be sent to pBuf
     *
     * @return  PNIO_INT32                transmitted number of bytes
     *
     */
    PNIO_UINT32 PNIO_snprintf (PNIO_UINT8* str, PNIO_UINT32 count, PNIO_UINT8* fmt, ...)
    {
        PNIO_va_list argptr;
        PNIO_INT32 ret;

        va_start(argptr, (char*)(fmt));

        ret = PNIO_vsnprintf (str, count, (char*)(fmt), argptr);

        if (ret < 0)
            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "error PNIO_snprintf : %d\n", errno );

        va_end(argptr);

        return ( (PNIO_UINT32) ret );
    }

    /**
     * @brief print to char array with char array from variable list
     *
     * @param[out]   pBuf       PNIO_UINT8*    buffer which should be filled by this function
     * @param[in]   fmt         PNIO_INT8*     text which should be sent to pBuf
     *
     * @return  PNIO_UINT32                 transmitted number of bytes
     *
     */
    PNIO_UINT32 PNIO_sprintf(PNIO_UINT8* pBuf, PNIO_INT8* fmt, ...)
    {
        PNIO_INT32 Cnt = 0;
        PNIO_va_list argptr;

        va_start(argptr, fmt);

        Cnt = vsprintf ( (char *) pBuf, (const char * ) fmt, argptr);

        if ( Cnt < 0 )
            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "vsprintf failed : %d\n", errno );

        va_end(argptr);

        return ( (PNIO_UINT32) Cnt );
    }

    /**
     * @brief printf to stdout string from variable list
     *
     * @param[in]   fmt         PNIO_INT8*     text which should be sent to stdout
     *
     * @return  void
     *
     */
    void PNIO_ConsolePrintf(PNIO_INT8 * fmt, ...)
    {
        PNIO_va_list argptr;

        va_start(argptr, fmt);

        if ( vprintf (fmt, argptr) < 0 )
            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "vprintf error caused by %d\n", errno);

        va_end(argptr);
    }

    /**
     * @brief Print names of occupied threads
     *
     * @return         void
     *
     */
    void OsPrintTaskProp (void)
    {
       int TskId;
       for (TskId= 0; TskId < MAXNUM_OF_TASKS; TskId++)
       {
            if (TaskProp[TskId].Occupied)
            {
                PNIO_printf ("TaskID=%d,  Name=%s\n",
                              TskId,
                              TaskProp[TskId].TaskName);
            }
       }
    }

    /**
     * @brief Thread with low priority which prints to stdout demanded strings
     *
     * @return void
     */
    PNIO_VOID PrintRS232PostTask (PNIO_VOID)
    {

        PNIO_INT8* pMsg = LSA_NULL;
        LSA_UINT32 taskID;

        OsWaitOnEnable();
        taskID = OsGetThreadId();

        while (1)
        {
          OsReadMessageBlocked ((void**)&pMsg, taskID);

          if (pMsg == 0)
          {
             if ( printf ("ERROR: PrintRS232PostTask() pMsg=0\n") < 0 )
                 LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "printf error caused by %d\n", errno);
          }
          else
          {
             if ( printf ("%s", pMsg) < 0 )
                 LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "printf error caused by %d\n", errno);

             if ( fflush(stdout) == EOF )
                 LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "fflush error caused by %d\n", errno);

             os_print_read_index += 1;
             if ( os_print_read_index >= OS_PRINT_INDEXES)
             {
                 os_print_read_index = 0;
             }

          }
        }
    }

    /**
     * @brief get formated string from input
     *
     * @param[in]   pText       PNIO_INT8*     text which should be sent to stdout
     * @param[in]   *pStr   	PNIO_UINT32*   pointer to input buffer
     *
     * @return  PNIO_UINT32 - length of read string
     *
     */
    PNIO_UINT32 OsKeyScanString( PNIO_INT8* pText, PNIO_UINT8* pStr, PNIO_UINT32 MaxLen )
    {
    	PNIO_INT32 len;
    	PNIO_UINT32 NewChar;
    	if( pText )
        {
    		PNIO_printf( ( char* ) pText );
        }
    	for( len = 0; len < MaxLen; )
    	{
    		NewChar = getchar(  );
    		if( 0x0a == NewChar )	/* enter */
    		{
    			/* ignore first enter */
    			if( 0 == len )
    			{
    				/* do nothing, not even increment len */
    			}
    			else
    			{	/* some chars, then enter = valid input ended by enter */
					pStr[ len ] = 0x00;	/* \0 */
					break;
    			}
    		}
    		else
    		{
    			pStr[ len ] = NewChar;
    			len++;
    		}
    	}
    	if( len == MaxLen )
    	{
    		PNIO_printf( "ERROR: Input sequence too long\n" );
    		return 0;
    	}
    	return len;	/* do not count last \0 character */
    }

    /**
     * @brief get formated number from input
     *
     * @param[in]   pText       PNIO_INT8*     text which should be sent to stdout
     * @param[in]   InputBase   PNIO_UINT32    format input for output number 16 : hexadecimal, 10 : decimal
     *
     * @return  PNIO_UINT32 - returned hexadecimal / decimal number
     *
     */
    PNIO_UINT32 OsKeyScan32 (PNIO_INT8* pText, PNIO_UINT32 InputBase)
    {
       PNIO_UINT32 val32 = 0;

       if (pText)
           PNIO_printf ((char*) pText);

       if (InputBase == 16)
       {
           if ( scanf ("%x", (int *) &val32) == EOF )
               LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "scanf error caused by %d\n", errno);
       }
       else if (InputBase == 10)
       {
           if ( scanf ("%d", (int *) &val32) == EOF )
               LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "scanf error caused by %d\n", errno);
       }
       else
       {
           LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "no valid InputBase\n");
       }

       return (val32);
    }

    /**
     * @brief get character from input
     *
     *
     * @return  PNIO_UINT8 - returned character
     *
     */
    PNIO_UINT8  OsGetChar (void)
    {
        PNIO_INT8 Char = 0;

        while ((Char == 0x0a) || (Char == 0x00))   // hide carriage return and NUL
        {
            Char = getchar();

            if ( Char == EOF )
                LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "getchar error caused by %d\n", errno);
        }
        return ( ( PNIO_UINT8 ) Char );
    }

    /**
     * @brief Store trace buffer to flash memory
     *
     * @param[in]   const char *            name of first block in flash memory where will be stored
     * @param[in]   void *                  address of data
     * @param[in]   PNIO_UINT32             size of data
     * @param[in]   const char *            name of second block in flash memory where will be stored
     * @param[in]   void *                  address of data
     * @param[in]   PNIO_UINT32             size of data
     *
     * @return         PNIO_UINT32     PNIO_OK         stored
     *                                 PNIO_NOT_OK     not stored
     *
     */
    PNIO_UINT32 OsStoreTraceBuffer( const char * pBlock1Name, void* pBlock1Start, PNIO_UINT32 Block1Size,
                                    const char * pBlock2Name, void* pBlock2Start, PNIO_UINT32 Block2Size)
    {
        FILE* fp;

        if (pBlock1Name)
        {
            fp = fopen (pBlock1Name, "w+b");
            if (!fp)
            {
                return (PNIO_NOT_OK);
            }
            fwrite( (const void *) pBlock1Start, 1, Block1Size, fp);
            fclose (fp);
        }

        if (pBlock2Name)
        {
            fp = fopen (pBlock2Name, "w+b");
            if (!fp)
            {
                return (PNIO_NOT_OK);
            }
            fwrite( (const void *) pBlock2Start, 1, Block2Size, fp);
            fclose (fp);
        }

        return (PNIO_OK);
    }
    /** @} */
#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
