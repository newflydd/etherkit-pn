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
/*  F i l e               &F: pniobase.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  data type definitions for PNIO                                           */
/*                                                                           */
/*****************************************************************************/
#ifndef PNIOBASE_H
#define PNIOBASE_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif


#ifdef __cplusplus
  #define PNIO_EXTRN  extern "C"
#else
  #define PNIO_EXTRN  extern
#endif

    #include "compiler.h"

    #include "lsa_types.h"
    #include "pnio_types.h"

    // *----------------------------------------------------------------------------------------*
    // *	PNIO basic data types
    // *----------------------------------------------------------------------------------------*


    // *----------------------------------------------------------------------------------------*
    // *	PNIO derived data types
    // *----------------------------------------------------------------------------------------*
    typedef     PNIO_VOID*      PNIO_VOID_PTR_TYPE;

    typedef union
    {
        PNIO_UINT32     u32;
        PNIO_UINT16     u16[2];
        PNIO_UINT8      u8[4];
    }  PNIO_VAR32;

    typedef union
    {
        PNIO_UINT16     u16;
        PNIO_UINT8      u8[2];
    }  PNIO_VAR16;



    // *----------------------------------------------------------------------------------------*
    // *	PNIO extended data types
    // *----------------------------------------------------------------------------------------*
    #ifdef  USE_DEF_VA_LIST
    	typedef va_list  PNIO_va_list;
    #else
        typedef char*    PNIO_va_list;
    #endif

    typedef struct
    {
		    union
		    {
				    struct { PNIO_UINT8	s_b1,s_b2,s_b3,s_b4; } S_un_b;
				    struct { PNIO_UINT16	s_w1,s_w2; } S_un_w;
				    PNIO_UINT32 S_addr;										/* IP-Adresse */
		    } S_un;
    } PNIO_IP_ADDR ;

    typedef struct
    {
		    PNIO_INT16		sin_family;					/* Konstante fuer TCPIP: SOCK_AF_INET */
		    PNIO_UINT16		sin_port;					/* Port-Nummer */
		    PNIO_IP_ADDR	sin_addr;					/* unsigned long: IP-Adresse in Netz-Darstellung */
		    PNIO_INT8		sin_zero[8];				/* Reserviert: 0 */
    }OS_SOCKADDR_IN ;

    // *----------------------------------------------------------------------------------------*
    // *	macros
    // *----------------------------------------------------------------------------------------*

    #define PNIO_UNUSED_ARG(arg_)	{arg_ = arg_;}


    typedef enum
    {
        PNIO_ADDR_LOG = 0,		// logical addressing, not supported
        PNIO_ADDR_GEO = 1,		// geographic addressing (slotnumber, subslot-number)
        PNIO_ADDR_HND = 2		// Handle addressing (slothandle, subslot-handle) for faster access than GEO
    } PNIO_ADDR_TYPE;

    typedef enum
    {
        PNIO_IO_IN  = 0,    // input modules, write to pnio stack
        PNIO_IO_OUT = 1     // output modules, read from pnio stack
    } PNIO_IO_TYPE;

	#ifndef PNIO_IOXS_USE_ENUM_TYPE
		#define PNIO_S_BAD  0x00		/* io provider or consumer state = BAD  */
		#define PNIO_S_GOOD 0x80      	/* io provider or consumer state = GOOD */
		#define PNIO_IOXS 	PNIO_UINT8  /* shouldn't be ENUM, as other values can occur from IOC */
	#else
		typedef enum
		{
			PNIO_S_BAD  = 0x00,     /* io provider or consumer state = BAD    */
			PNIO_S_GOOD = 0x80      /* io provider or consumer state = GOOD   */
 		} PNIO_IOXS;                /* represents PNIO IO remote/local status */
	#endif


    /* PNIO_ERR_STAT:
     * a) The first four parameters represent the PNIO Status.
     *    For details refer to IEC 61158-6 chapter 3.2.10.6 and 3.2.11.66
     * b) The last two parameters correspond to AdditionalValue1 and AdditionalValue2,
     *    see IEC 61158-6 chapter 3.2.11.50:
     *    The values shall contain additional user information within negative responses.
     *    The value zero indicates no further information.
     *    For positive read responses, the value 1 of the field AdditionalValue1 indicates
     *    that the Record Data Object contains more data than have been read.
     */
    typedef ATTR_PNIO_PACKED_PRE struct
    {
        PNIO_UINT8     ErrCode;   /* ErrorCode: Most significant word, most significant byte of PNIO Status */
        PNIO_UINT8     ErrDecode; /* ErrorDecode: Most significant word, least significant byte of PNIO Status */
        PNIO_UINT8     ErrCode1;  /* ErrorDecode: Least significant word, most significant byte of PNIO Status */
        PNIO_UINT8     ErrCode2;  /* ErrorCode2: Least significant word, least significant byte of PNIO Status */
        PNIO_UINT16    AddValue1;
        PNIO_UINT16    AddValue2;
    }  ATTR_PNIO_PACKED PNIO_ERR_STAT;



#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
