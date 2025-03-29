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
/*  F i l e               &F: posix_utils.c                             :F&  */
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
 * @file     posix_utils.c
 * @brief    interface for memory, string, converts, math utils
 *
 * interface for memory, string, converts, math utils
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
    #include "os_utils.h"
    #include "os_taskprio.h"
    #include "pniousrd.h"
	#include "nv_data.h"
	#include "trc_if.h"
	#include "lsa_cfg.h"
	#include "lsa_sys.h"
    #include "tskma_cfg.h"
    #include "logadapt.h"
    #include "usriod_cfg.h"

    /**
     * @brief converts the unsigned integer hostlong from host byte order to network byte order
     *
     * @param[in]   PNIO_UINT32     value to be converted
     *
     * @return      PNIO_UINT32     converted result
     *
     */
    OS_CODE_FAST PNIO_UINT32 OsHtonl ( PNIO_UINT32 val32 )
    {
        return htonl(val32);
    }

    /**
     * @brief converts the unsigned short integer hostshort from host byte order to network byte order
     *
     * @param[in]   PNIO_UINT16     value to be converted
     *
     * @return      PNIO_UINT16     converted result
     *
     */
    OS_CODE_FAST PNIO_UINT16 OsHtons ( PNIO_UINT16 val16 )
    {
        return htons(val16);
    }

    /**
     * @brief converts the unsigned integer netlong from network byte order to host byte order
     *
     * @param[in]   PNIO_UINT32     value to be converted
     *
     * @return         PNIO_UINT32  converted result
     *
     */
    OS_CODE_FAST PNIO_UINT32 OsNtohl ( PNIO_UINT32 val32 )
    {
        return ntohl(val32);
    }

    /**
     * @brief converts the unsigned short netlong from network byte order to host byte order
     *
     * @param[in]   PNIO_UINT16     value to be converted
     *
     * @return         PNIO_UINT16  converted result
     *
     */
    OS_CODE_FAST PNIO_UINT16 OsNtohs ( PNIO_UINT16 val16 )
    {
        return ntohs(val16);
    }

    /**
     * @brief memory copy
     *
     * @param[in]   PNIO_VOID *         address of destination memory
     * @param[in]   const PNIO_VOID *   address of source memory
     * @param[in]   PNIO_UINT32         length of copy
     *
     * @return         PNIO_VOID *  address of destination memory
     *
     */
    OS_CODE_FAST PNIO_VOID * OsMemCpy ( PNIO_VOID * pDst, const PNIO_VOID * pSrc, PNIO_UINT32 size )
    {
        if ( size == 0 )
            return pDst;

        if (((PNIO_UINT8*)pDst >= ((PNIO_UINT8*)pSrc + size)) || (((PNIO_UINT8*)pDst + size) <= (PNIO_UINT8*)pSrc))
        {
            ;
        }
        else
        {
            unsigned int i;
            if (pSrc < pDst)
            {
                for (i = size; i != 0; i--)
                {
                    *(((PNIO_UINT8*)pDst) + (i-1)) = *(((PNIO_UINT8*)pSrc) + (i-1));
                }
                return pDst;
            }
            else
            {
                for (i = 0; i < size; i++)
                {
                    *(((PNIO_UINT8*)pDst) + i) = *(((PNIO_UINT8*)pSrc) + i);
                }
                return pDst;
            }
        }

        return memcpy(pDst, pSrc, size);
    }

    /**
     * @brief memory move ( still copy )
     *
     * @param[in]   PNIO_VOID *         address of destination memory
     * @param[in]   const PNIO_VOID *   address of source memory
     * @param[in]   PNIO_UINT32         length of copy
     *
     * @return      PNIO_VOID *         address of destination memory
     *
     */
    OS_CODE_FAST PNIO_VOID * OsMemMove(PNIO_VOID * pDst, const PNIO_VOID * pSrc, PNIO_UINT32 size)
    {
        return memmove(pDst,pSrc,size);
    }

    /**
     * @brief fill memory
     *
     * @param[in]   PNIO_VOID *         address of destination memory
     * @param[in]   PNIO_INT            filled value
     * @param[in]   PNIO_UINT32         length of copy
     *
     * @return      PNIO_VOID *  address of destination memory
     *
     */
    OS_CODE_FAST PNIO_VOID * OsMemSet(PNIO_VOID * pDst,PNIO_INT val,PNIO_UINT32 size)
    {
        return memset(pDst,val,size);
    }

    /**
     * @brief compare memories
     *
     * @param[in]   PNIO_VOID *         address of memory
     * @param[in]   PNIO_VOID *         address of memory
     * @param[in]   PNIO_UINT32         length
     *
     * @return         PNIO_INT     =  0    memories are equal
     *                              != 0    memories aren't equal
     *
     */
    OS_CODE_FAST PNIO_INT OsMemCmp(const PNIO_VOID * pBuf1,const PNIO_VOID * pBuf2, PNIO_UINT32 size)
    {
        return memcmp(pBuf1,pBuf2,size);
    }

    /**
     * @brief compare strings
     *
     * @param[in]   const PNIO_INT8 *   address of strings
     * @param[in]   const PNIO_INT8 *   address of strings
     *
     * @return         PNIO_INT     =  0    memories are equal
     *                              != 0    memories aren't equal
     *
     */
    OS_CODE_FAST PNIO_INT OsStrCmp(const PNIO_INT8 * pBuf1, const PNIO_INT8 *pBuf2)
    {
        return strcmp(pBuf1,pBuf2);
    }

    /**
     * @brief compare strings in defined length
     *
     * @param[in]   const PNIO_INT8 *   address of strings
     * @param[in]   const PNIO_INT8 *   address of strings
     * @param[in]   PNIO_UINT32         length
     *
     * @return         PNIO_INT     =  0    memories are equal
     *                              != 0    memories aren't equal
     *
     */
    OS_CODE_FAST PNIO_INT OsStrnCmp(const PNIO_INT8 *pBuf1, const PNIO_INT8 *pBuf2, PNIO_UINT32 size)
    {
        return strncmp(pBuf1,pBuf2,size);
    }

    /**
     * @brief string's copy
     *
     * @param[in]   PNIO_INT8 *         address of destination string
     * @param[in]   const PNIO_INT8 *   address of source string
     * @param[in]   PNIO_UINT32         length of copy
     *
     * @return      PNIO_INT8 *  address of destination memory
     *
     */
    OS_CODE_FAST PNIO_INT8 * OsStrCpy(PNIO_INT8 *pDst,const PNIO_INT8 *pSrc)
    {
        return strcpy(pDst,pSrc);
    }

    /**
     * @brief string's copy in defined length
     *
     * @param[in]   PNIO_INT8 *         address of destination string
     * @param[in]   const PNIO_INT8 *   address of source string
     * @param[in]   PNIO_UINT32         length of copy
     *
     * @return      PNIO_INT8 *  address of destination strings
     *
     */
    OS_CODE_FAST PNIO_INT8 * OsStrnCpy(PNIO_INT8 *pDst,const PNIO_INT8 *pSrc, PNIO_UINT32 size)
    {
        return strncpy(pDst,pSrc,size);
    }

    /**
     * get length of string
     *
     * @PARAM   PNIO_INT8 *         address of string
     *
     * @RETURN         PNIO_UINT32  number of characters contained in string
     *
     */
    OS_CODE_FAST PNIO_UINT32 OsStrLen(const PNIO_INT8 *pBuf)
    {
        return strlen(pBuf);
    }

    /**
     * @brief get pointer to the first occurrence of character in string
     *
     * @param[in]   PNIO_INT8 *         address of string
     * @param[in]   PNIO_INT            character
     *
     * @return         PNIO_INT8 *  pointer to the first occurrence of character in string
     *
     */
    OS_CODE_FAST PNIO_INT8 * OsStrChr(const PNIO_INT8 *pBuf, PNIO_INT c)
    {
        return strchr(pBuf,c);
    }

    /**
     * @brief get random number
     *
     *
     * @return         PNIO_INT     random number
     *
     */
    PNIO_INT  OsRand( void )
    {
        return rand();
    }

    /**
     * @brief Init pseudo-random number generator
     *
     * @param[in]      PNIO_UINT       seed
     *
     * @return         PNIO_INT     random number
     *
     */
    PNIO_VOID OsSrand(PNIO_UINT val)
    {
        srand(val);
    }

    /**
     * @brief Convert string to integer
     *
     * @param[in]   const PNIO_INT8 *   input string
     *
     * @return      PNIO_INT     integer
     *
     */
    PNIO_INT OsAtoi(const PNIO_INT8 *pStr)
    {
        return atoi(pStr);
    }

    /**
     * @brief math log10
     *
     * @param[in]      double       value
     *
     * @return         double     log10 value
     *
     */
    double PNIO_log10(double value)
    {
        return log10(value);
    }

    /**
     * @brief math pow
     *
     * @param[in]   double       base
     * @param[in]   double       exponent
     *
     * @return      double     base^exponent
     *
     */
    double PNIO_pow(double base, double exponent)
    {
        return pow(base, exponent);
    }

#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
