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
/*  F i l e               &F: clrpc_cfg_lib.c                           :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/* system adaption of this component                                         */
/*                                                                           */
/*****************************************************************************/

/* ***************************************************************************
*
*   H i s t o r y :
*   see /_doc/im151_3pn_hist.doc
*
* ****************************************************************************/

#define LSAS_MODULE_ID  12

//#include "clrpc_inc.h" /* see clrpc_cfg.h for platform things */
#include "lsas_inc.h"
#include "lsas_int.h"

/*
 * NOTE: this implementation may not be very efficient...
 */


/*=============================================================================
 * function name:  CLRPC_SWAP_U16, CLRPC_SWAP_U32 and CLRPC_SWAP_UUID
 *
 * function:       swap "in place"
 *                 between "little" and "big" endian or vice versa
 *
 * documentation:  LSA_CLRPC_LSA-Interface.doc
 *
 *===========================================================================*/

#ifndef CLRPC_SWAP_U16
LSA_VOID
CLRPC_SWAP_U16(
    LSA_UINT16  * short_ptr /* correct alignment! */
)
{
    LSAS_SWAP_U16( short_ptr );
}
#else
# error "by design a function"
#endif

#ifndef CLRPC_SWAP_U32
LSA_VOID CLRPC_SWAP_U32(
    LSA_UINT32  * long_ptr /* correct alignment! */
)
{
    LSAS_SWAP_U32( long_ptr );
}
#else
# error "by design a function"
#endif

#ifndef CLRPC_SWAP_UUID
LSA_VOID CLRPC_SWAP_UUID(
    CLRPC_UUID_PTR_TYPE uuid_ptr /* correct alignment! */
)
{
    CLRPC_SWAP_U32(&((uuid_ptr)->time_low));
    CLRPC_SWAP_U16(&((uuid_ptr)->time_mid));
    CLRPC_SWAP_U16(&((uuid_ptr)->time_hi_and_version));
}
#else
# error "by design a function"
#endif


/*=============================================================================
 * function name:  CLRPC_HTONS ... host to network byte order, short integer
 *                 CLRPC_HTONL ... host to network byte order, long  integer
 *                 CLRPC_NTOHS ... network to host byte order, short integer
 *                 CLRPC_NTOHL ... network to host byte order, long  integer
 *
 * function:       byte order conversion
 *
 * documentation:  LSA_CLRPC_Detailspec.doc
 *
 *===========================================================================*/

#ifndef CLRPC_HTONS
LSA_UINT16 CLRPC_HTONS(
    LSA_UINT16 host_short
)
{
    return LSAS_HTONS(host_short);
}
#else
# error "by design a function"
#endif

#ifndef CLRPC_HTONL
LSA_UINT32 CLRPC_HTONL(
    LSA_UINT32 host_long
)
{
    return LSAS_HTONL(host_long);
}
#else
# error "by design a function"
#endif

#ifndef CLRPC_NTOHS
LSA_UINT16 CLRPC_NTOHS(
    LSA_UINT16 network_short
)
{
    return LSAS_NTOHS(network_short);
}
#else
# error "by design a function"
#endif

#ifndef CLRPC_NTOHL
LSA_UINT32 CLRPC_NTOHL(
    LSA_UINT32 network_long
)
{
    return LSAS_NTOHL(network_long);
}
#else
# error "by design a function"
#endif


/*=============================================================================
 * function name:  CLRPC_ PUT/GET 16_/UUID_ LITTLE
 *
 * function:       macros for memory access without alignment requirements
 *
 * important:      WE WILL IMPLEMENT variants for mem-attribs
 *                 when needed. All known platforms are "flat".
 *
 *===========================================================================*/

#ifndef CLRPC_PUT16_LITTLE
LSA_VOID CLRPC_PUT16_LITTLE( /* put, little endian byte order */
    LSA_VOID_PTR_TYPE ptr,
    LSA_UINT16 val
)
{
    LSA_UINT16 tmp = val;

    CLRPC_MEMCPY((ptr), &tmp, sizeof(tmp));
}
#else
# error "by design a function"
#endif

#ifndef CLRPC_GET16_LITTLE
LSA_UINT16 CLRPC_GET16_LITTLE( /* get, little endian byte order */
    LSA_VOID_PTR_TYPE ptr
)
{
    return *(LSA_UINT16 *)ptr;
}
#else
# error "by design a function"
#endif

#ifndef CLRPC_PUT_UUID_LITTLE
LSA_VOID CLRPC_PUT_UUID_LITTLE( /* put, convert into little endian byte order */
    LSA_VOID_PTR_TYPE ptr,
    CLRPC_UUID_PTR_TYPE uuid_ptr /* correct alignment! */
)
{
    CLRPC_UUID_TYPE tmp = *uuid_ptr;

    CLRPC_MEMCPY(ptr, &tmp, sizeof(tmp));
}
#else
# error "by design a function"
#endif

#ifndef CLRPC_GET_UUID_LITTLE
LSA_VOID CLRPC_GET_UUID_LITTLE( /* get, convert from little endian byte order */
    CLRPC_UUID_PTR_TYPE uuid_ptr, /* correct alignment! */
    LSA_VOID_PTR_TYPE ptr
)
{
    CLRPC_MEMCPY(uuid_ptr, ptr, sizeof(*uuid_ptr));
}
#else
# error "by design a function"
#endif

/*=============================================================================
 * function name:  CLRPC_MEMSET, CLRPC_MEMCPY,
 *                 CLRPC_STRLEN, CLRPC_STRCPY
 *                 CLRPC_SPRINTF, CLRPC_SSCANF
 *
 * function:       macros for standard C-lib functions
 *
 * important:      WE WILL IMPLEMENT variants for mem-attribs
 *                 when needed. All known platforms are "flat"
 *
 *===========================================================================*/

#ifndef CLRPC_MEMSET
LSA_VOID CLRPC_MEMSET(
    LSA_VOID_PTR_TYPE ptr,
    LSA_INT val,
    LSA_UINT len
)
{
    LSAS_MEMSET(ptr,val,len);
}
#else
# error "by design a function"
#endif

#ifndef CLRPC_MEMCPY
LSA_VOID CLRPC_MEMCPY(
    LSA_VOID_PTR_TYPE dst,
    LSA_VOID_PTR_TYPE src,
    LSA_UINT len
)
{
    LSAS_MEMCPY(dst,src,len);
}
#else
# error "by design a function"
#endif

#ifndef CLRPC_STRCPY
LSA_VOID CLRPC_STRCPY(
    CLRPC_COMMON_MEM_U8_PTR_TYPE dst,
    CLRPC_COMMON_MEM_U8_PTR_TYPE src
)
{
    LSAS_STRCPY((char *)dst,(const char *)src);
}
#else
# error "by design a function"
#endif

#ifndef CLRPC_STRLEN
LSA_UINT CLRPC_STRLEN(
    CLRPC_COMMON_MEM_U8_PTR_TYPE str
)
{
    return LSAS_STRLEN((const char *)str);
}
#else
# error "by design a function"
#endif



/*****************************************************************************/
/*  end of file.                                                             */
/*****************************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
