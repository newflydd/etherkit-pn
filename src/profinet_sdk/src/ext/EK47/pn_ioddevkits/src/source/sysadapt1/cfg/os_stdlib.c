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
/*  F i l e               &F: os_stdlib.c                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/* OS abstraction layer implementation for a  windows operating system                                                                         */
/*                                                                           */
/*****************************************************************************/

#define USE_DEF_VA_LIST				// must be defined before include

#include "compiler.h"
#if (INCLUDE_DK_STDLIB_HANDLER == 1)

#if 0
    #include "compiler_stdlibs.h"
 
    #include "os.h"
	#include "trc_if.h"
	#include "lsa_cfg.h"
    #include "logadapt.h"

	#include <stdlib.h>
	#include <stdio.h>
    #include <math.h>
#else
   #define LTRC_ACT_MODUL_ID   112
    #define	IOD_MODULE_ID       112

    #define LOG_LSA_PACK_ID     LSA_COMP_ID_IODAPI
    #define LOG_LSA_MOD_ID      IOD_MODULE_ID

	#include "stdarg.h"
    #include "compiler_stdlibs.h"  // include standard header files
    #include "pniousrd.h"
    #include "edd_inc.h"
    #include "os.h"
    #include "os_cfg.h"
    #include "os_taskprio.h"
    #include "pnio_trace.h"
	#include "logadapt.h"
    #include "bspadapt.h"
	#include <cyg/kernel/kapi.h>
	#include <stdlib.h>
	#include <stdio.h>
    #include <math.h>
    #include "usriod_cfg.h"

	#include "trc_if.h"
	#include "lsa_cfg.h"
	#include "lsa_sys.h"
#endif

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_DKSW_ATMEL)
	#include <cyg/hal/hal_io.h>
	#include <cyg/hal/var_io.h>
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200_400)
    #include "tskma_int.h"
    #include "edd_usr.h"
    #include "ertec_inc.h"
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200P)
    #include "tskma_int.h"
    #include "edd_usr.h"
    #include "ertec_inc.h"
	//#include "ertec_irq.h"
#endif

    #define LTRC_ACT_MODUL_ID   107
    #define	IOD_MODULE_ID       LTRC_ACT_MODUL_ID


    // *-----------------------------------------------------
    // * external functions
    // *-----------------------------------------------------
    extern void DebugExpand (void);

    // *-----------------------------------------------------
    // * static variables
    // *-----------------------------------------------------
    PNIO_UINT32  PnioLogDest = 1;   // debug logging   0:none, 1:Console, 2:Tracebuffer
 

    // ******************************************************************************
    // * OsHtons
    // ******************************************************************************
    PNIO_UINT16 OsHtons(PNIO_UINT16 val16)
    {
        #if PNIO_BIG_ENDIAN
            return (val16);
        #else
            return(((val16 & 0xFF)<<8 ) | ((val16 & 0xFF00)>>8));
        #endif
    }

    // ******************************************************************************
    // * OsHtonl
    // ******************************************************************************
    PNIO_UINT32 OsHtonl (PNIO_UINT32 val32)
    {
        #if PNIO_BIG_ENDIAN
            return (val32);
        #else
	        return(((val32 & 0xFF)<<24 ) | ((val32 & 0xFF00)<<8) | ((val32 & 0xFF0000)>>8) | ((val32 & 0xFF000000)>>24));
        #endif
    }

    // ******************************************************************************
    // * OsNtohs
    // ******************************************************************************
    PNIO_UINT16 OsNtohs(PNIO_UINT16 val16)
    {
        #if PNIO_BIG_ENDIAN
            return (val16);
        #else
	        return(((val16 & 0xFF)<<8 ) | ((val16 & 0xFF00)>>8));
        #endif
    }

    // ******************************************************************************
    // * OsNtohl
    // ******************************************************************************
    PNIO_UINT32 OsNtohl (PNIO_UINT32 val32)
    {
        #if PNIO_BIG_ENDIAN
            return (val32);
        #else
	        return(((val32 & 0xFF)<<24 ) | ((val32 & 0xFF00)<<8) | ((val32 & 0xFF0000)>>8) | ((val32 & 0xFF000000)>>24));
        #endif
    }

    #if 0
        // ******************************************************************************
        // * OsAtol
        // ******************************************************************************
        long OsAtol(const char *txt)    /* don't let the compiler truncate longs */
        {
            return (atol (txt));
        }
    #endif

    // ******************************************************************************
    // * OsMemCpy 
    // ******************************************************************************
    void OsMemCpy (void* pDst, void* pSrc, PNIO_UINT32 Len)
    {
		if (Len == 0)
			return;

	    if (((PNIO_UINT8*)pDst >= ((PNIO_UINT8*)pSrc + Len)) || (((PNIO_UINT8*)pDst + Len) <= (PNIO_UINT8*)pSrc))
		{
			;
		}
		else
		{
			//PNIO_printf ("warning: memcopy overlap Src=0x%x, Dst=0x%x, size=%d\n", pSrc, pDst, Len);
			#if 1
			{
				unsigned int i;
				if (pSrc < pDst)
				{
					for (i = Len; i != 0; i--)
					{
						*(((PNIO_UINT8*)pDst) + (i-1)) = *(((PNIO_UINT8*)pSrc) + (i-1));
					}
					return;
				}
				else
				{
					for (i = 0; i < Len; i++)
					{
						*(((PNIO_UINT8*)pDst) + i) = *(((PNIO_UINT8*)pSrc) + i);
					}
					return;
				}
			}
			#endif
		}




        if ((Len < 32) || ((PNIO_UINT32)pSrc%4) || ((PNIO_UINT32)pDst%4))
        {
            memcpy (pDst, pSrc, Len);
            return;
        }
        else
        {
            PNIO_UINT32 Rest = Len % 4;
            PNIO_UINT32 i;
            for (i = 0; i < (Len>>2); i++)
            {
                *(((PNIO_UINT32*)pDst) + i) = *(((PNIO_UINT32*)pSrc) + i);
            }

            switch (Rest)
            {
                case 3: *(((PNIO_UINT8*)pDst) + (Len-3)) = *(((PNIO_UINT8*)pSrc) + (Len-3));
                case 2: *(((PNIO_UINT8*)pDst) + (Len-2)) = *(((PNIO_UINT8*)pSrc) + (Len-2));
                case 1: *(((PNIO_UINT8*)pDst) + (Len-1)) = *(((PNIO_UINT8*)pSrc) + (Len-1));
                case 0: break;
            }
        }
    }

    // ******************************************************************************
    // * OsMemMove 
    // ******************************************************************************
    PNIO_VOID* OsMemMove (void* pDst, void* pSrc, PNIO_UINT32 size_t)
    {
        return (memmove (pDst, pSrc, size_t));
    }

    // ******************************************************************************
    // * OsMemSet 
    // ******************************************************************************
    void OsMemSet (void* pDst, PNIO_UINT32 val, PNIO_UINT32 size_t)
    {
        memset (pDst, val, size_t);
    }

    // ******************************************************************************
    // * OsMemCmp 
    // ******************************************************************************
    PNIO_INT32 OsMemCmp (void* pBuf1, void* pBuf2, PNIO_UINT32 size_t)
    {
        return (memcmp (pBuf1, pBuf2, size_t));
    }


    // ******************************************************************************
    // * OsStrCmp 
    // ******************************************************************************
    PNIO_INT32 OsStrCmp (void* pBuf1, void* pBuf2)
    {
        return (strcmp (pBuf1, pBuf2));
    }


    // ******************************************************************************
    // * OsStrnCmp 
    // ******************************************************************************
    PNIO_INT32 OsStrnCmp (void* pBuf1, void* pBuf2, PNIO_UINT32 size_t)
    {
        return (strncmp (pBuf1, pBuf2, size_t));
    }

    // ******************************************************************************
    // * OsStrCpy
    // ******************************************************************************
    PNIO_INT8* OsStrCpy (void* pDst, void* pSrc)
    {
        return (strcpy (pDst, pSrc));
    }

    // ******************************************************************************
    // * OsStrnCpy
    // ******************************************************************************
    PNIO_INT8* OsStrnCpy (void* pDst, void* pSrc, PNIO_UINT32 size_t)
    {
        return (strncpy (pDst, pSrc, size_t));
    }

    // ******************************************************************************
    // * OsStrChr
    // ******************************************************************************
    PNIO_INT8* OsStrChr (void* pBuf, PNIO_UINT32 c)
    {
        return (strchr (pBuf, c));
    }

    // ******************************************************************************
    // * OsStrLen
    // ******************************************************************************
    PNIO_UINT32 OsStrLen (void* pBuf)
    {
        return (strlen (pBuf));
    }

    // ******************************************************************************
    // * OsAtoi
    // ******************************************************************************
    PNIO_INT32 OsAtoi (void* pStr)
    {
        return (atoi (pStr));
    }


    // ******************************************************************************
    // * OsTestPointer
    // * This function checks, if a address pointer is invalid. If the pointer is
    // * ok, the pointer address is returned, otherwise an error handler is executed.
    // * There may be a need to modify the conditions, e.g. altera arm platforms use
    // * bit 31 to specify a cached/noncached memory.
    // ******************************************************************************
    PNIO_VOID*  OsTestPointer(void * ptr) 
    {
	    if( ptr != 0 ) 
        { /* especially 0xCCCCCCCC */
		    if ((PNIO_INT32) ptr >= 0x80000000L)
                LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "invalid pointer\n");
	    }
        return (ptr);
    }


    // *----------------------------------------------------------------*
    // *    
    // *  PNIO_Print (void) 
    // *    
    // *----------------------------------------------------------------*
    // *    
    // *  output of debug logging
    // *    
    // *  Input:    fmt			format string for vfprintf
    // *            argptr		pointer to print-arguments
    // *  Output:   ----
    // *          
    // *----------------------------------------------------------------*
    static void OsPrint (PNIO_INT8 * fmt, 
				     PNIO_va_list argptr)
    {
	    vprintf(fmt, argptr);
	   // fflush(stdout);
    }

     // *----------------------------------------------------------------*
     // *
     // *  PNIO_ConsolePrintf (void)
     // *
     // *----------------------------------------------------------------*
     // *
     // *  printf on standard console
     // *  indenpendent from log level and log destination
     // *
     // *  Input:    like the  printf() function
     // *  Output:   ----
     // *
     // *----------------------------------------------------------------*
 	void PNIO_ConsolePrintf(PNIO_INT8 * fmt, ...)
 	{
        #if 1
            switch (PnioLogDest)
            {
                case 0:
                case 1:
	                    {
		                    PNIO_va_list argptr;
		                    va_start(argptr, fmt);
		                    OsPrint (fmt, argptr);
		                    va_end(argptr);
	                    }
                        break;
			    case 2:
					    {
						    va_list argptr;
						    va_start(argptr, fmt);
						    OsPrint (fmt, argptr);
		                    //TRACE_PRINT (fmt, (PNIO_va_list)argptr);
						    va_end(argptr);
					    }
                        break;

                #if INCLUDE_LOGADAPT
                    case 3:
	                        {
		                        PNIO_va_list argptr;
		                        va_start(argptr, fmt);
		                        LogAdaptConsPrint (fmt, argptr);
		                        va_end(argptr);
	                        }
                            break;
                #endif

                default:
                        break;
            }
        #else
		    va_list argptr;
		    va_start(argptr, fmt);
		    OsPrint (fmt, argptr);
		    va_end(argptr);
        #endif
 	}

    // *----------------------------------------------------------------*
    // *
    // *  PNIO_ConsoleLog (void)
    // *
    // *----------------------------------------------------------------*
    // *
    // *  printf on standard console
    // *  if PnioLogDest is Console (1)
    // *
    // *  Input:    like the  printf() function
    // *  Output:   ----
    // *
    // *----------------------------------------------------------------*
	void PNIO_ConsoleLog(PNIO_INT8 * fmt, ...)
	{
		if (PnioLogDest == 1)
		{
			va_list argptr;
			va_start(argptr, fmt);
			OsPrint (fmt, argptr);
			va_end(argptr);
		}
	}


    // *----------------------------------------------------------------*
    // *    
    // *  PNIO_printf (void) 
    // *    
    // *----------------------------------------------------------------*
    // *    
    // *  printf on standard console
    // *    
    // *  Input:    like the  printf() function		
    // *  Output:   ----
    // *          
    // *----------------------------------------------------------------*
    void PNIO_printf(PNIO_INT8 * fmt, ...) 
    {
        switch (PnioLogDest)
        {
            case 0:
                    break;
            case 1:
	                {
		                PNIO_va_list argptr;
		                va_start(argptr, fmt);
		                OsPrint (fmt, argptr);
		                va_end(argptr);
	                }
                    break;
			case 2:
					{
						va_list argptr;
						va_start(argptr, fmt);
						OsPrint (fmt, argptr);
		                //TRACE_PRINT (fmt, (PNIO_va_list)argptr);
						va_end(argptr);
					}
                    break;

            #if INCLUDE_LOGADAPT
                case 3:
	                    {
		                    PNIO_va_list argptr;
		                    va_start(argptr, fmt);
		                    LogAdaptConsPrint (fmt, argptr);
		                    va_end(argptr);
	                    }
                        break;
            #endif

            default:
                    break;
        }
    }


    // *----------------------------------------------------------------*
    // *    
    // *  PNIO_vsprintf (void) 
    // *    
    // *----------------------------------------------------------------*
    // *    
    // *  printf on standard console
    // *    
    // *  Input:    like the  printf() function		
    // *  Output:   ----
    // *          
    // *----------------------------------------------------------------*
    PNIO_UINT32 PNIO_vsprintf(PNIO_UINT8* pBuf, PNIO_INT8* fmt, PNIO_va_list argptr) 
    {
        return (vsprintf (pBuf, fmt, argptr));
    }

    // *----------------------------------------------------------------*
    // *    
    // *  PNIO_sprintf (void) 
    // *    
    // *----------------------------------------------------------------*
    // *    
    // *  printf on buffer
    // *    
    // *  Input:    like the  printf() function		
    // *  Output:   ----
    // *          
    // *----------------------------------------------------------------*
    PNIO_UINT32 PNIO_sprintf(PNIO_UINT8* pBuf, PNIO_INT8* fmt, ...) 
    {
        PNIO_UINT32 Cnt = 0;
	    PNIO_va_list argptr;
	    va_start(argptr, fmt);

 	    Cnt = vsprintf(pBuf, fmt, argptr);
	    va_end(argptr);
        return Cnt;
    }

    // *----------------------------------------------------------------*
    // *    
    // *  PNIO_log10 (double)
    // *
    // *----------------------------------------------------------------*
    // *
    // *  logarithm on basis 10
    // *
    // *  Input:    values
    // *  Output:   log10 of value
    // *
    // *----------------------------------------------------------------*
    double PNIO_log10(double value)
    {
    	return log10(value);
    }


    // *----------------------------------------------------------------*
    // *
    // *  PNIO_pow (double,double)
    // *
    // *----------------------------------------------------------------*
    // *
    // *  returns base raised to the exp power (base^exp)
    // *
    // *  Input:    base
    // *  Input:    exp
    // *  Output:   pow(base,exp)
    // *
    // *----------------------------------------------------------------*
    double PNIO_pow(double base, double exp)
    {
    	return pow(base, exp);
    }

    // *----------------------------------------------------------------*
    // *
    // *  PNIO_snprintf (void) 
    // *    
    // *----------------------------------------------------------------*
    // *    
    // *  printf on buffer
    // *          
    // *----------------------------------------------------------------*
    PNIO_UINT32 PNIO_snprintf (PNIO_UINT8* str, PNIO_UINT32 count, PNIO_UINT8* fmt, ...)
    {
	    va_list argptr;
	    LSA_INT ret;
	    LSA_UNUSED_ARG(count);

        OsMemSet (str, 0, count);   // first set destination buffer to 00
	    va_start(argptr, (char*)(fmt));
	    ret = PNIO_vsprintf (str, (char*)(fmt), argptr);
	    if (strlen(str) >= count)
            LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "error PNIO_snprintf\n");

	    va_end(argptr);
	    return ret;
    }

    // *----------------------------------------------------------------*
    // *    
    // *  PNIO_TrcPrintf (void) 
    // *    
    // *----------------------------------------------------------------*
    // *    
    // *  printf of debug logging
    // *    
    // *  Input:    like the  printf() function		
    // *  Output:   ----
    // *          
    // *----------------------------------------------------------------*
    void PNIO_TrcPrintf(PNIO_INT8 * fmt, ...) 
    {
        switch (PnioLogDest)
        {
            case 0:
                    break;
            case 1:
	                {
		                PNIO_va_list argptr;
		                va_start(argptr, fmt);
		                OsPrint (fmt, argptr);
		                va_end(argptr);
	                }
                    break;
			case 2:
					{
						va_list argptr;
						va_start(argptr, fmt);
						OsPrint (fmt, argptr);
		                //TRACE_PRINT (fmt, (PNIO_va_list)argptr);
						va_end(argptr);
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


    // *--------------------------------------------*
    // *   OsGetChar (void)
    // *   reads a character from the keyboard of
    // *   the terminal (used only for the example
    // *   application interface
    // *--------------------------------------------*
    PNIO_UINT8  OsGetChar (void)
    {
        PNIO_UINT8 Char;
        switch (PnioLogDest)
        {
            #if INCLUDE_LOGADAPT
                case 3: 
                        {
                            Char = LogAdaptGetChar ();
                        }
                        break;
            #endif

            default:
                    {
                        static Reentrant = 0;
                        Char = (_getch());
                        if ((Char == 0x13) && (Reentrant == 0)) // ctrl e
                        {
                            Reentrant = 1;
                            DebugExpand ();
                            Reentrant = 0;
                        }
                    }
                    break;

        }
        return (Char);
    }

    
    // *----------------------------------------------------------------*
    // *    
    // *  OsKeyScan32 (PNIO_UINT8* pText, PNIO_UINT32 InputBase)
    // *    
    // *----------------------------------------------------------------*
    // *  prints the text and reads a value from console
    // *  new io data exchange.
    // *    
    // *  Input:    pText       Textstring to print	
    // *            InputBase   10:  decimal base (default), 16: hex base, others: default
    // *  Output:   return      Value
    // *----------------------------------------------------------------*
    PNIO_UINT32 OsKeyScan32 (PNIO_INT8* pText, PNIO_UINT32 InputBase)
    {
       PNIO_UINT32 val32;

       if (pText)
	       PNIO_printf ((char*) pText);
   

        switch (PnioLogDest)
        {
            #if INCLUDE_LOGADAPT
                case 3: 
                        {
                           PNIO_UINT8 AsciiBuf[10];
                           LogAdapGetStrInp(AsciiBuf, InputBase, sizeof (AsciiBuf));
                           if (InputBase == 16)
	                           sscanf (AsciiBuf, "%x", &val32); 
                           else
	                           sscanf (AsciiBuf, "%d", &val32); 
                           break;

                        }
                        break;
            #endif

            default:
               if (InputBase == 16)
	               scanf ("%x", &val32); 
               else
	               scanf ("%d", &val32); 
               break;

        }
       
       return (val32);
    }


#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
