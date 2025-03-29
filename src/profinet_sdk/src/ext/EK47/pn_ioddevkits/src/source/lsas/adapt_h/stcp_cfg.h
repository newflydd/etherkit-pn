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
/*  F i l e               &F: stcp_cfg.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*                      Frame for file "stcp_cfg.h".                         */
/*                      ==============================                       */
/*                                                                           */
/*  Configuration for stcp:                                                  */
/*  Defines constants, types and macros for stcp.                            */
/*                                                                           */
/*  This file has to be overwritten during system integration, because       */
/*  some includes depend on the different system, compiler or                */
/*  operating system.                                                        */
/*                                                                           */
/*****************************************************************************/

#ifndef STCP_CFG_H
#define STCP_CFG_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________
 *
 *  Date        Version        Who  What
 *  1999-07-29  Original            Version by Tankurt Mevluet of UMC
 *  2002-01-15  P00.00.00.000  st   Adaptation to global LSA header
 *  2002-02-19                 st   Macros rewritten / compiler error fixed
 * 2002-04-03                  st   global LSA header    missing compiler switches
 *                                  incorporated and there names aligned
 * 2002-04-09                  st   Definitions of the lower interface of
 *                                  stcp_low.h moved to here as these things
 *                                  have to be set / STCP_MAX_TPDU_SIZE
 * 2002-04-25                  st   Comments for socket interfaces
 *                                  constants / constants for RFC, which must not
 *                                  be changed, moved to stcp_low.h
 * 2002-05-22                  st   RCV_MODE_NO_REQ    new constants, functions,
 *                                  interfaces for RCV_MODE_NO_REQ:
 *                                  asynchronous UPPER_RQB and UPPER_MEM,
 *                                  lower interface also based on RQB (SOCK),
 *                                  internal functions for processing (1st part)
 *                                  Consider connection abort
 * 2002-05-23                  st   global LSA header: apply current changes
 * 2002-06-13                  st   Accesses to SOCK-Rqb / Adaptation to the SOCK interface
 * 2002-06-18                  st   SOCK interface    Definition for undefined,
 *                                  unused socket descriptor
 * 2002-06-20                  st   SOCK interface    Prototypes for access macros to SOCK interface
 * 2002-06-21                  st   SOCK interface    Access macros to lower RQB of SOCK /
 *                                  Adaptation to the SOCK interface
 * 2002-06-25                  st   SOCK interface    SOCKADDR -> use SOCK_SOCKADDR_TYPE of
 *                                  lower interface (==> no conversions necessary)
 * 2002-06-18                  st   SOCK interface    constant for INVALID-Sock-Fd (instead of UNUSED)
 * 2002-07-25                  st   SOCK interface    Compiler error corrected
 * 2002-08-01                  st   global LSA header    CN_ID for STCP must never be greater than
 *                                  UINT16 because it is transferred in the protocol in a UINT16!!
 *                                  Constants for socket interface that
 *                                  are no longer used were removed
 * 2002-11-07                  st   STCP_USER_ID_CN_ID is replaced by L4_USER_ID_CN_ID
 *                                  STCP_LOW_RQB_USER_ID was removed, was only a local implementation attempt
 * 2002-11-11                  st   STCP_LOWER_IN_FCT_ATTR / STCP_LOWER_OUT_FCT_ATTR: define was missing
 *                                  Description STCP_NEGOCIATE_ID appended as invalid cn_id
 *                                  STCP_COPY_IP_ADDR has been moved to stcp_int.h
 * 2002-12-04                  st   old functions and defines deleted
 * 2002-12-06                  st   new constant for offset in the lower data buffer
 * 2003-01-16                  st   Macro STCP_LOW_RQB_GET_RECV_PDU_PTR included into the file
 *                                  stcp_cfg.h. Then the macros exist in full
 *                                  and there is no suspicion that a macro
 *                                  has been removed since the last version
 * 2003-02-18                  st   new opcode for EXCEPTION_NOTIFY
 * 2003-02-19                  st   new macros for EXCEPTION_NOTIFY added to the list
 * 2003-04-03                  st   max. Length for subnet and address cannot be set,
 *                                  therefore moved to stcp_low
 * 2003-04-17                  st   Macros and constants for trace points
 * 2003-05-16                  st   STCP_LOW_RSP_OK_EOM does not exist ==> removed
 * 2003-07-30                  st   for trace entries "why a connection aborts"
 *                                  defined new level and new macros
 * 2003-07-31                  st   Correction of the trace macros
 * 2003-08-12                  st   new macros to access the LSA_COMPONENT_ID
 * 2003-08-19                  st   new "dummy" macro for ..._TRACE_BARRAY macro (not
 *                                  yet defined by the LSA team, not yet implemented in the LTRC)
 * 2003-09-04                  st   TRACE macros were removed because they are now defined in stcp_trc.h
 * 2003-09-09                  st   Merge br_entw_2 to main
 * 2003-09-09                  st   ..TRACE_BARRAY becomes ...TRACE_BYTE_ARRAY and is described in stcp_trc.h
 * 2004-01-28                  st   Adaptation to new header (LSA_GLOB_P2.1.0.0_0.3.1.1 ): LSA_TRACE_LEVEL is now defined
 *                                  in lsa_cfg.txt, therefore the definition can be deleted here
 * 2004-02-09                  st   CLOSE_CHANNEL    new opcode CANCEL_CONNECTS
 * 2004-02-18                  st   Comments corrected: Spelling mistakes "Kommunikaitonskanals" And "Paramter" /
 *                                  Comment "read location" was not correct everywhere
 * 2004-04-28                  st   removed unnecessary macros for access to outdated error structure
 *                                  (STCP_LOW_RQB_GET_ERROR_LOCATION, STCP_LOW_RQB_GET_ERROR_LSA_COMONENT_ID,
 *                                  STCP_LOW_RQB_GET_ERROR_LOWER_OPCODE, STCP_LOW_RQB_GET_ERROR_LOWER_RESPONSE,
 *                                  STCP_LOW_RQB_GET_ERROR_ERROR_CODE_ID)
 *                                  new macro  STCP_LOW_RQB_GET_ERR_ADDR
 * 2004-07-12                  st   Adaptation to LSA_GLOB_P2.1.0.0_0.4.1.1
 *                                  C++ parentheses and copyright
 * 2004-08-26                  st   STCP_MAX_TIMER and STCP_MAX_HW_DEVICES do not continue to be
 *                                  used ==> delete
 * 2005-08-18                  st   Defines, which were not used, have been removed:
 *                                  STCP_LISTEN_ID_1/ STCP_LOW_RSP_DEV_ERROR /
 *                                  STCP_LOW_RSP_OWN_ADDR_ERROR / STCP_MAX_TSAP_ID_LEN /
 *                                  STCP_MAX_STADDR_LEN / STCP_MAX_SUBNET_ID_LEN
 *                                   Details:     Compiler switch STCP_DETAILS_INTERNAL is no longer
 *                                  supportted, STCP_DETAILS_EXTERNAL has to be used
 *                                  or only the procedure of external details
 * 2005-08-19                   st  Special characters	file changes for it to be also readable in Notepad
 *                                  Replaced special characters
 * 2005-11-30                   st  AP00269115  quick SOCK interface
 *                                  Access macros to LOWER-RQB extended and adapted because of
 *                                  new args structures for the new opcodes /
 *                                  defines for LOW_OPC adapted to new opcodes
 * 2006-03-22                   st  - CLOSE_CHANNEL error	state CHA_STATE_CANCEL and
 *                                  opcode CANCEL_CONNECT no longer exist ==> delete constants
 *                                  - compiler switch	Switch STCP_INTERNICHE, STCP_FUSION, ...
 *                                  for the different compilers removed, have not been used in a long time
 *                                  - in addition to the opcodes the responses to the lower interface
 *                                  initialized with SOCK values
 * 2006-08-03                   st  Removed unused elements: all prototypes and access macros, which used to contain SELECT
 *                                  SOCK_SOCKADDR_TYPE: instead STCP_SOCKADDR_TYPE is now
 *                                  used, which has to be converted to the type of SOCK /
 *                                  do not directly use SOCK_SEND_ALL_, convert to STCP constants /
 *                                  introduced new empty macro STCP_FILE_SYSTEM_EXTENSION
 * 2006-10-18                   st  Translation into english
 * 2007-07-30                   st  Beschreibung der STCP_MAX_TPDU_SIZE erweitert um die Abhaengigkeiten
 * 2007-07-31                   st  more detailed description for STCP_MAX_TPDU_SIZE
 * 2007-08-21                   st  for internal use only: Date and Version in file header set to XXXX
 * 2008-01-22                   st  SOCK liefert ADDR_INUSE wenn der Port schon belegt ist durch
 *                                  T-Verbindungen ==> im LISTEN abfangen, da bei Systemen mit
 *                                  CLOSE_CHANNEL ein anderer durchaus den RFC1006-Port schon belegt haben koennte
 * 2008-01-28                   st  Korrektur beim Konstantennamen von SOCK
 * 2009-08-13                   st  AP00859984	Makros RQB_SET_NEXT und RQB_GET_NEXT ungenutzt
 *                                  ==> Zugriffsmakros auf L4-RQB-Header
 * 2009-08-18                   st  Zugriffsmakro auf SOCK-RQB fuer EXCEPTION_NOTIFY anpassen: "args.except."
 *                                  wurde umbenannt in args.except_rq, weil "except" in C++ reserviert ist
 * 2009-08-18                   st  SOCK-Schnittstelle hat sich geaendert, LSA_SOCK_P04.01.00.00_00.03.02.01:
 *                                  Zugriffsmakro auf SOCK-RQB fuer EXCEPTION_NOTIFY anpassen: "args.except."
 *                                  wurde umbenannt in args.except_rq, weil "except" in C++ reserviert ist *
 * 2009-10-06                   st  Umlaute aus Datei entfernt
 * 2011-06-22                   st  new macro STCP_REQUEST_UPPER_DONE() for calling the CBF
 * 2013-04-16                   st  AP01504830	Zugriffsmakros angepasst:
 *                                  - Timer-Werte: verwendete Makros bleiben erhalten, nur in cfg leer definiert,
 *                                    damit auch auf aeltere SOCK-Staende aufgesetzt werden koennte
 *                                  - neue Zugriffsmakros auf die interface_id
 * 2013-11-25                   st  834874	nicht mehr notwendige Makros fuer TIME-Werte geloescht
 * 2014-05-09                   st  64 Bit	Vorgabe fuer Zugriffsmakros korrigiert, da sie bei 64 Bit zum falschen
 *                                  Ergebnis fuehren: Zeiger duerfen nicht auf LSA_UINT32 gecastet werden
*/

/*****************************************************************************/

/*===========================================================================*/
/*                             compiler-switches                             */
/*===========================================================================*/

/*===========================================================================*/
/*                              basic attributes                             */
/*===========================================================================*/

#define STCP_ATTR_NEAR
#define STCP_ATTR_FAR
#define STCP_ATTR_SHUGE
#define STCP_ATTR_HUGE



/*===========================================================================*/
/*                         code- and data-attributes                         */
/*===========================================================================*/

/*=========================  code attributes  =========================*/
/* Local functions */
#define STCP_LOCAL_FCT_ATTR                     STCP_ATTR_FAR

/* input functions from the system */
#define STCP_SYSTEM_IN_FCT_ATTR                 STCP_ATTR_FAR

/* output functions to the system */
#define STCP_SYSTEM_OUT_FCT_ATTR                STCP_ATTR_FAR

/* input functions from user */
#define STCP_UPPER_IN_FCT_ATTR                  STCP_ATTR_FAR

/*  output functions to the user */
#define STCP_UPPER_OUT_FCT_ATTR                 STCP_ATTR_FAR

/* input functions from the layer 2 */
#define STCP_LOWER_IN_FCT_ATTR                  STCP_ATTR_HUGE  /* lower component to STCP */

/* output functions to the layer 2 */
#define STCP_LOWER_OUT_FCT_ATTR                 STCP_ATTR_HUGE  /* STCP to lower component */
/*=========================  data attributes  =========================*/
/* Local data */
#define STCP_LOCAL_MEM_ATTR                     STCP_ATTR_FAR

/* Data buffer of system */
#define STCP_SYSTEM_MEM_ATTR                    STCP_ATTR_FAR

/* Data buffer of the lower level */
#define STCP_LOWER_MEM_ATTR                     STCP_ATTR_FAR

/* largest common data attribute */
#define STCP_COMMON_MEM_ATTR                    STCP_ATTR_FAR


#define STCP_LOWER_RQB_ATTR                     STCP_ATTR_HUGE  /* lower-rqb data */


/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/
#define STCP_LOWER_RQB_TYPE         SOCK_RQB_TYPE        /* lower-RQB */

#define	STCP_SOCKADDR_TYPE          SOCK_SOCKADDR_TYPE   /* type of a socket address */

/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/

/*****************************************************************************/
/* General properties                                                        */
/*****************************************************************************/
/*=============================================================================
 *       set local_mem static or dynamic:
 * static (no use of STCP_ALLOC_LOCAL_MEM):
#define STCP_LOCAL_MEM_STATIC
 * or dynamic (use of STCP_ALLOC_LOCAL_MEM):
#define STCP_LOCAL_MEM_DYNAMIC
 *===========================================================================*/
#define STCP_LOCAL_MEM_STATIC

/*=============================================================================
 *       clear global data:
 * clear (STCP_DATA_TYPE stcp_data = {0};):
#define STCP_CLEAR_GLOBAL_DATA
 * or no clear:
#define STCP_NO_CLEAR_GLOBAL_DATA
 *===========================================================================*/
#define STCP_NO_CLEAR_GLOBAL_DATA



/****************************************************************************/
/* Limits                                                                   */
/****************************************************************************/

/* review - werte */

/* max. length of a transport PDU in bytes */
/* This value must be set to the maximum of all values set for rfc_tpdu_size in the
 * detail_ptrs of all channels, but caution, there are different formats!
 * For example: rfc_tpdu_size for channel 1 is STCP_TPDU_SIZE_512_OCTETS and
 * rfc_tpdu_size for channel 2 is STCP_TPDU_SIZE_1024_OCTETS then
 * STCP_MAX_TPDU_SIZE must be set to 1024!
 * STCP will allocate memory for the LOWER interface with the length
 * STCP_MAX_TPDU_SIZE + STCP_LOW_MEM_OFFSET + 8 bytes! This length will be the
 * maximun length used for requests to the lower layer component!
 */
#define STCP_MAX_TPDU_SIZE          APMA_PDB_S7DT_MAX_TPDU_SIZE

/* Maximum number of LSA channels (or STCP_CHANNEL data structures):
 * Every user of the STCP layer logs on with open_channel for the
 * use of one channel. The user specifies the required
 * system path, a handle to identify its request blocks and
 * the callback function to return request blocks.
 */
/* Warning: If STCP_MAX_CHANNELS >= 256 then the LSA_HANDLE_TYPE has to be of type
 * LSA_UINT16 or LSA_UINT32!
 * If STCP_MAX_CHANNELS >= 65536 then the LSA_HANDLE_TYPE has to be of type
 * LSA_UINT32!
 */
#define STCP_MAX_CHANNELS           APMA_PDB_STCP_MAX_CHANNELS

/* Maximum number of connections.
 * A maximum of STCP_MAX_CONNECTIONS TCP connections
 * and the corresponding number of  S7 connections can be opened.
 * For each S7 connection a  STCP_CN structure exists and
 * for each TCP connection a STCP_DEVICE structure exists.
 * An additional STCP_DEVICE structure is used for listen
 * socket calls.
 */
/* Warning: If STCP_MAX_CONNECTIONS >= 256 then L4_CN_ID_TYPE has to be of type
 * LSA_UINT16!
 * For STCP the L4_CN_ID_TYPE must never be greater than LSA_UINT16 because the cn_id
 * is transferred in the protocol (RFC1006) in an LSA_UINT16 element!
 */
#define STCP_MAX_CONNECTIONS        APMA_PDB_S7DT_MAX_CONNECTIONS            /* max. permitted: 65530 */

/* invalid lower Device_id, has to be at least STCP_MAX_CONNECTIONS + 1
 */
#define STCP_NO_DEVICE              (STCP_MAX_CONNECTIONS + 1)

/* invalid socket descriptor
 * has to be coordinated with the TCP/IP stack or with SOCK
 */
#define STCP_SOCK_INVALID           ((LSA_HANDLE_TYPE)-1)

/* Reserved connection IDs.
 */
#define STCP_NEGOTIATE_ID          ((L4_CN_ID_TYPE)(STCP_MAX_CONNECTIONS + 1))       /* invalid CN-ID */


/* Offset in the lower data buffer from which data can exist
 */
#define STCP_LOW_MEM_OFFSET        0


/*===========================================================================*/
/*                                 macros                                    */
/*===========================================================================*/

/****************************************************************************/
/* Macro definitions                                                        */
/****************************************************************************/
/* The output functions of the S7 transport can be defined
 * as macros, if required.
 */

// zzxch stcp : ??? clarify setting here see src\_prj\stcp_cfg.c:

// #define STCP_ENTER()
// #define STCP_EXIT()


/*=============================================================================
 * Name:        STCP_FILE_SYSTEM_EXTENSION(module_id_)
 * Function:    Can be used by the system adaptor to add something to the
 *              source files.
 *
 *              The content of this macro is the responsibility of the system adaptor.
 *              Internal component data must not be changed!
 *
 * Parameter:   Module ID
 *
 *              No prototype exists for this macro!
 *===========================================================================*/
 #define STCP_FILE_SYSTEM_EXTENSION(module_id_)


/************************************************************************/
/*            TRACE            TRACE            TRACE                   */
/************************************************************************/

/*----------------------------------------------------------------------*/
/* The trace macros are included in the stcp_trc.h!                     */
/* There you will also find a description of the events which are traced*/
/* (in the various cases).                                              */
/* The following macros are included there and have to be implemented   */
/* here, if the adaptation to the LTRC does not suit the system:        */
/*----------------------------------------------------------------------*/
/*
#define STCP_MEMORY_TRACE_00(level, msg)
#define STCP_MEMORY_TRACE_01(level, msg, para1)
#define STCP_MEMORY_TRACE_02(level, msg, para1, para2)
#define STCP_MEMORY_TRACE_03(level, msg, para1, para2, para3)
#define STCP_MEMORY_TRACE_04(level, msg, para1, para2, para3, para4)
#define STCP_MEMORY_TRACE_05(level, msg, para1, para2, para3, para4, para5)
#define STCP_MEMORY_TRACE_06(level, msg, para1, para2, para3, para4, para5, para6)
#define STCP_MEMORY_TRACE_07(level, msg, para1, para2, para3, para4, para5, para6, para7)
#define STCP_MEMORY_TRACE_08(level, msg, para1, para2, para3, para4, para5, para6, para7, para8)
#define STCP_MEMORY_TRACE_09(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9)
#define STCP_MEMORY_TRACE_10(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10)
#define STCP_MEMORY_TRACE_11(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11)
#define STCP_MEMORY_TRACE_12(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12)
#define STCP_MEMORY_TRACE_13(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13)
#define STCP_MEMORY_TRACE_14(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14)
#define STCP_MEMORY_TRACE_15(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14, para15)
#define STCP_MEMORY_TRACE_16(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14, para15, para16)
#define STCP_UPPER_TRACE_00(level, msg)
#define STCP_UPPER_TRACE_01(level, msg, para1)
#define STCP_UPPER_TRACE_02(level, msg, para1, para2)
#define STCP_UPPER_TRACE_03(level, msg, para1, para2, para3)
#define STCP_UPPER_TRACE_04(level, msg, para1, para2, para3, para4)
#define STCP_UPPER_TRACE_05(level, msg, para1, para2, para3, para4, para5)
#define STCP_UPPER_TRACE_06(level, msg, para1, para2, para3, para4, para5, para6)
#define STCP_UPPER_TRACE_07(level, msg, para1, para2, para3, para4, para5, para6, para7)
#define STCP_UPPER_TRACE_08(level, msg, para1, para2, para3, para4, para5, para6, para7, para8)
#define STCP_UPPER_TRACE_09(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9)
#define STCP_UPPER_TRACE_10(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10)
#define STCP_UPPER_TRACE_11(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11)
#define STCP_UPPER_TRACE_12(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12)
#define STCP_UPPER_TRACE_13(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13)
#define STCP_UPPER_TRACE_14(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14)
#define STCP_UPPER_TRACE_15(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14, para15)
#define STCP_UPPER_TRACE_16(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14, para15, para16)
#define STCP_LOWER_TRACE_00(level, msg)
#define STCP_LOWER_TRACE_01(level, msg, para1)
#define STCP_LOWER_TRACE_02(level, msg, para1, para2)
#define STCP_LOWER_TRACE_03(level, msg, para1, para2, para3)
#define STCP_LOWER_TRACE_04(level, msg, para1, para2, para3, para4)
#define STCP_LOWER_TRACE_05(level, msg, para1, para2, para3, para4, para5)
#define STCP_LOWER_TRACE_06(level, msg, para1, para2, para3, para4, para5, para6)
#define STCP_LOWER_TRACE_07(level, msg, para1, para2, para3, para4, para5, para6, para7)
#define STCP_LOWER_TRACE_08(level, msg, para1, para2, para3, para4, para5, para6, para7, para8)
#define STCP_LOWER_TRACE_09(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9)
#define STCP_LOWER_TRACE_10(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10)
#define STCP_LOWER_TRACE_11(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11)
#define STCP_LOWER_TRACE_12(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12)
#define STCP_LOWER_TRACE_13(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13)
#define STCP_LOWER_TRACE_14(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14)
#define STCP_LOWER_TRACE_15(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14, para15)
#define STCP_LOWER_TRACE_16(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14, para15, para16)
#define STCP_SYSTEM_TRACE_00(level, msg)
#define STCP_SYSTEM_TRACE_01(level, msg, para1)
#define STCP_SYSTEM_TRACE_02(level, msg, para1, para2)
#define STCP_SYSTEM_TRACE_03(level, msg, para1, para2, para3)
#define STCP_SYSTEM_TRACE_04(level, msg, para1, para2, para3, para4)
#define STCP_SYSTEM_TRACE_05(level, msg, para1, para2, para3, para4, para5)
#define STCP_SYSTEM_TRACE_06(level, msg, para1, para2, para3, para4, para5, para6)
#define STCP_SYSTEM_TRACE_07(level, msg, para1, para2, para3, para4, para5, para6, para7)
#define STCP_SYSTEM_TRACE_08(level, msg, para1, para2, para3, para4, para5, para6, para7, para8)
#define STCP_SYSTEM_TRACE_09(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9)
#define STCP_SYSTEM_TRACE_10(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10)
#define STCP_SYSTEM_TRACE_11(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11)
#define STCP_SYSTEM_TRACE_12(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12)
#define STCP_SYSTEM_TRACE_13(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13)
#define STCP_SYSTEM_TRACE_14(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14)
#define STCP_SYSTEM_TRACE_15(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14, para15)
#define STCP_SYSTEM_TRACE_16(level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14, para15, para16)
#define STCP_PROTOCOL_TRACE_00(dev_id, level, msg)
#define STCP_PROTOCOL_TRACE_01(dev_id, level, msg, para1)
#define STCP_PROTOCOL_TRACE_02(dev_id, level, msg, para1, para2)
#define STCP_PROTOCOL_TRACE_03(dev_id, level, msg, para1, para2, para3)
#define STCP_PROTOCOL_TRACE_04(dev_id, level, msg, para1, para2, para3, para4)
#define STCP_PROTOCOL_TRACE_05(dev_id, level, msg, para1, para2, para3, para4, para5)
#define STCP_PROTOCOL_TRACE_06(dev_id, level, msg, para1, para2, para3, para4, para5, para6)
#define STCP_PROTOCOL_TRACE_07(dev_id, level, msg, para1, para2, para3, para4, para5, para6, para7)
#define STCP_PROTOCOL_TRACE_08(dev_id, level, msg, para1, para2, para3, para4, para5, para6, para7, para8)
#define STCP_PROTOCOL_TRACE_09(dev_id, level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9)
#define STCP_PROTOCOL_TRACE_10(dev_id, level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10)
#define STCP_PROTOCOL_TRACE_11(dev_id, level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11)
#define STCP_PROTOCOL_TRACE_12(dev_id, level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12)
#define STCP_PROTOCOL_TRACE_13(dev_id, level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13)
#define STCP_PROTOCOL_TRACE_14(dev_id, level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14)
#define STCP_PROTOCOL_TRACE_15(dev_id, level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14, para15)
#define STCP_PROTOCOL_TRACE_16(dev_id, level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14, para15, para16)
#define STCP_PROTOCOL_TRACE_BYTE_ARRAY(dev_id, level, msg, ptr, len)
*/

/************************************************************************/
/*            TRACE            TRACE            TRACE                   */
/************************************************************************/
// not used        TRACE_SUBSYS_STCP_LOWER,
// not used        TRACE_SUBSYS_STCP_MEMORY,
// not used        TRACE_SUBSYS_STCP_SYSTEM,
// not used        TRACE_SUBSYS_STCP_UPPER,
// not used         TRACE_SUBSYS_STCP_PROTOCOL,


// zzxch stcp : >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//    zzxch  ??? clarify skip only 2 using channel_pr sy-Ptr
// #ifdef CFG_STCP_FW_COMP_ON
//empty macros, for finding compile  error

// #define STCP_SYSTEM_TRACE_02(level, msg, para1, para2)
// #define STCP_SYSTEM_TRACE_03(level, msg, para1, para2, para3)

//   other default setting
//   LSA_IDX_TRACE_00(TRACE_SUBSYS_STCP_PROTOCOL, dev_id, level, msg)
//   adaptname and skip  dev_id


#define STCP_PROTOCOL_TRACE_00(dev_id, level, msg)\
      LSA_TRACE_00(TRACE_SUBSYS_STCP_PROTOCOL, level, msg)

#define STCP_PROTOCOL_TRACE_01(dev_id, level, msg, para1)\
      LSA_TRACE_01(TRACE_SUBSYS_STCP_PROTOCOL, level, msg, para1)


#define STCP_PROTOCOL_TRACE_02(dev_id, level, msg, para1, para2)\
      LSA_TRACE_02(TRACE_SUBSYS_STCP_PROTOCOL, level, msg, para1, para2)

#define STCP_PROTOCOL_TRACE_03(dev_id, level, msg, para1, para2, para3)\
      LSA_TRACE_03(TRACE_SUBSYS_STCP_PROTOCOL, level, msg, para1, para2, para3)

#define STCP_PROTOCOL_TRACE_04(dev_id, level, msg, para1, para2, para3, para4)\
      LSA_TRACE_04(TRACE_SUBSYS_STCP_PROTOCOL, level, msg, para1, para2, para3, para4)

#define STCP_PROTOCOL_TRACE_05(dev_id, level, msg, para1, para2, para3, para4, para5)\
      LSA_TRACE_05(TRACE_SUBSYS_STCP_PROTOCOL, level, msg, para1, para2, para3, para4, para5)

#define STCP_PROTOCOL_TRACE_06(dev_id, level, msg, para1, para2, para3, para4, para5, para6)\
      LSA_TRACE_06(TRACE_SUBSYS_STCP_PROTOCOL, level, msg, para1, para2, para3, para4, para5, para6)

#define STCP_PROTOCOL_TRACE_07(dev_id, level, msg, para1, para2, para3, para4, para5, para6, para7)\
      LSA_TRACE_07(TRACE_SUBSYS_STCP_PROTOCOL, level, msg, para1, para2, para3, para4, para5, para6, para7)

#define STCP_PROTOCOL_TRACE_08(dev_id, level, msg, para1, para2, para3, para4, para5, para6, para7, para8)\
      LSA_TRACE_08(TRACE_SUBSYS_STCP_PROTOCOL, level, msg, para1, para2, para3, para4, para5, para6, para7, para8)

#define STCP_PROTOCOL_TRACE_09(dev_id, level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9)\
      LSA_TRACE_09(TRACE_SUBSYS_STCP_PROTOCOL, level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9)

#define STCP_PROTOCOL_TRACE_10(dev_id, level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10)\
      LSA_TRACE_10(TRACE_SUBSYS_STCP_PROTOCOL, level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10)

#define STCP_PROTOCOL_TRACE_11(dev_id, level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11)\
      LSA_TRACE_11(TRACE_SUBSYS_STCP_PROTOCOL, level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11)

#define STCP_PROTOCOL_TRACE_12(dev_id, level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12)\
      LSA_TRACE_12(TRACE_SUBSYS_STCP_PROTOCOL, level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12)

#define STCP_PROTOCOL_TRACE_13(dev_id, level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13)\
      LSA_TRACE_13(TRACE_SUBSYS_STCP_PROTOCOL, level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13)

#define STCP_PROTOCOL_TRACE_14(dev_id, level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14)\
      LSA_TRACE_14(TRACE_SUBSYS_STCP_PROTOCOL, level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14)

#define STCP_PROTOCOL_TRACE_15(dev_id, level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14, para15)\
      LSA_TRACE_15(TRACE_SUBSYS_STCP_PROTOCOL, level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14, para15)

#define STCP_PROTOCOL_TRACE_16(dev_id, level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14, para15, para16)\
      LSA_TRACE_16(TRACE_SUBSYS_STCP_PROTOCOL, level, msg, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14, para15, para16)


#define STCP_PROTOCOL_TRACE_BYTE_ARRAY(dev_id, level, msg, ptr, len)
/* emppty*/


// zzxch stcp : <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

/*--------------------------------------------------------------------------*/
/* Filling in / reading the request block of the lower layer                */
/*--------------------------------------------------------------------------*/
/* Initialization of the request block */
#define STCP_LOW_RQB_INIT(low_rqb_ptr_)        \
{                                              \
                                               \
}

/*----------------------------------*/
/* Read/Write the header of the RQB */
/*----------------------------------*/
/* Write the pointer to the next RQB */
#define STCP_LOW_RQB_SET_NEXT(low_rqb_ptr_, value_) LSA_RQB_SET_NEXT_RQB_PTR(low_rqb_ptr_, value_)

/* Read the pointer to the next RQB */
#define STCP_LOW_RQB_GET_NEXT(low_rqb_ptr_)         LSA_RQB_GET_NEXT_RQB_PTR(low_rqb_ptr_)

/* Read handle */
#define STCP_LOW_RQB_GET_HANDLE(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> handle)

/*Write handle */
#define STCP_LOW_RQB_SET_HANDLE(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> handle = (value_))

/* Read opcode */
#define STCP_LOW_RQB_GET_OPCODE(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> opcode)

/* Write opcode */
#define STCP_LOW_RQB_SET_OPCODE(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> opcode = (value_))

/* Read user_id */
#define STCP_LOW_RQB_GET_USER_ID(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> user_id)

/* Write user_id */
#define STCP_LOW_RQB_SET_USER_ID(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> user_id = (value_))

/* General: Read response */
#define STCP_LOW_RQB_GET_RESPONSE(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> response)

/* General: Write response */
#define STCP_LOW_RQB_SET_RESPONSE(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> response = (value_) )
/*=============================================================================
 *       initialize user-id as unused
 * for    LSA_UINT16 user_id:
#define STCP_LOW_RQB_USER_ID_INIT(low_rqb_ptr_) \
    ((low_rqb_ptr_) -> user_id) =  0xffff
 *
 * or for LSA_USER_ID_TYPE user_id:
#define STCP_LOW_RQB_USER_ID_INIT(low_rqb_ptr_) \
    LSA_USER_ID_INIT(&((low_rqb_ptr_) -> user_id))
 *===========================================================================*/
#define STCP_LOW_RQB_USER_ID_INIT(low_rqb_ptr_) \
    LSA_USER_ID_INIT(&((low_rqb_ptr_) -> user_id))

/*=============================================================================
 *       write cn-id into user-id
 * for    LSA_UINT16 user_id:
#define STCP_LOW_RQB_SET_USER_ID_CN_ID(low_rqb_ptr_, value_) \
    ( ((low_rqb_ptr_) -> user_id) = (LSA_UINT16)(value_) )
 *
 * or for LSA_USER_ID_TYPE user_id:
#define STCP_LOW_RQB_SET_USER_ID_CN_ID(low_rqb_ptr_, value_) \
    ( L4_USER_ID_CN_ID((low_rqb_ptr_) -> user_id) = (value_) )
 *===========================================================================*/
#define STCP_LOW_RQB_SET_USER_ID_CN_ID(low_rqb_ptr_, value_) \
    ( L4_USER_ID_CN_ID((low_rqb_ptr_) -> user_id) = (value_) )

/*=============================================================================
 *       read cn-id from user-id
 * for    LSA_UINT16 user_id:
#define STCP_LOW_RQB_GET_USER_ID_CN_ID(low_rqb_ptr_) \
    ( (L4_CN_ID_TYPE)((low_rqb_ptr_) -> user_id) )
 *
 * or for LSA_USER_ID_TYPE user_id:
#define STCP_LOW_RQB_GET_USER_ID_CN_ID(low_rqb_ptr_) \
    ( L4_USER_ID_CN_ID((low_rqb_ptr_) -> user_id) )
 *===========================================================================*/
#define STCP_LOW_RQB_GET_USER_ID_CN_ID(low_rqb_ptr_) \
    ( L4_USER_ID_CN_ID((low_rqb_ptr_) -> user_id) )

/*=============================================================================
 *       write device-id into user-id
 * for    LSA_UINT16 user_id:
#define STCP_LOW_RQB_SET_USER_ID_DEVICE_ID(low_rqb_ptr_, value_) \
    ( ((low_rqb_ptr_) -> user_id) = (LSA_UINT16)(value_) )
 *
 * or for LSA_USER_ID_TYPE user_id:
#define STCP_LOW_RQB_SET_USER_ID_DEVICE_ID(low_rqb_ptr_, value_) \
    ( STCP_USER_ID_DEVICE_ID((low_rqb_ptr_) -> user_id) = (value_) )
 *===========================================================================*/
#define STCP_LOW_RQB_SET_USER_ID_DEVICE_ID(low_rqb_ptr_, value_) \
    ( STCP_USER_ID_DEVICE_ID((low_rqb_ptr_) -> user_id) = (value_) )

/*=============================================================================
 *       read device-id from user-id
 * for    LSA_UINT16 user_id:
#define STCP_LOW_RQB_GET_USER_ID_DEVICE_ID(low_rqb_ptr_) \
    ( (STCP_DEV_ID)((low_rqb_ptr_) -> user_id) )
 *
 * or for LSA_USER_ID_TYPE user_id:
#define STCP_LOW_RQB_GET_USER_ID_DEVICE_ID(low_rqb_ptr_) \
    ( STCP_USER_ID_DEVICE_ID((low_rqb_ptr_) -> user_id) )
 *===========================================================================*/
#define STCP_LOW_RQB_GET_USER_ID_DEVICE_ID(low_rqb_ptr_) \
    ( STCP_USER_ID_DEVICE_ID((low_rqb_ptr_) -> user_id) )

/*---------------------------*/
/* Open channel              */
/*---------------------------*/

/* Read the handle of the upper level */
#define STCP_LOW_RQB_GET_CHA_OPEN_HANDLE_UPPER(low_rqb_ptr_)\
    ((low_rqb_ptr_) -> args.channel.handle_upper)

/* Write own "upper_handle" */
#define STCP_LOW_RQB_SET_CHA_OPEN_HANDLE_UPPER(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.channel.handle_upper = (value_))

/* Write path of the communication channel */
#define STCP_LOW_RQB_SET_CHA_OPEN_PATH(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.channel.sys_path = (value_))

/* Read callback funtion */
#define STCP_LOW_RQB_GET_CHA_OPEN_CBF(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> args.channel.sock_request_upper_done_ptr)

/* Write callback function */
#define STCP_LOW_RQB_SET_CHA_OPEN_CBF(low_rqb_ptr_, value_)\
    ((low_rqb_ptr_) -> args.channel.sock_request_upper_done_ptr = (value_))

/* Read the handle of the lower layer*/
#define STCP_LOW_RQB_GET_CHA_OPEN_HANDLE(low_rqb_ptr_)\
    ((low_rqb_ptr_) -> args.channel.handle)

/* Write the handle of the lower layer */
#define STCP_LOW_RQB_SET_CHA_OPEN_HANDLE(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.channel.handle = (value_))

/*---------------------------*/
/* Close channel             */
/*---------------------------*/
/* not yet in 1st step       */


/*---------------------------*/
/* Initialize lists          */
/*---------------------------*/
/* Enter socket descriptor   */
#define STCP_LOW_RQB_SET_LISTEN_SOCK_FD(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.listen.sock_fd = (value_))

/* Read socket descriptor */
#define STCP_LOW_RQB_GET_LISTEN_SOCK_FD(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> args.listen.sock_fd)

/* Enter backlog parameter */
#define STCP_LOW_RQB_SET_LISTEN_BACKLOG(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.listen.backlog = (value_))

/*Read backlog parameter */
#define STCP_LOW_RQB_GET_LISTEN_BACKLOG(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> args.listen.backlog)

/* Enter SOCKADDR structure */
#define STCP_LOW_RQB_SET_LISTEN_SOCKADDR(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.listen.sockaddr = (value_))

/* Read SOCKADDR structure */
#define STCP_LOW_RQB_GET_LISTEN_SOCKADDR(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> args.listen.sockaddr)

// zzxch stcp : >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// zzxch stcp : ??? clarify setting old values not longer used
// zzxch stcp : /* Write timer_on  */
// zzxch stcp :#define STCP_LOW_RQB_SET_LISTEN_TIMER_ON(low_rqb_ptr_, value_)    \
// zzxch stcp :    ((low_rqb_ptr_) -> args.listen.timer_on = (value_))
// zzxch stcp :
// zzxch stcp :/* Read timer_on  */
// zzxch stcp :#define STCP_LOW_RQB_GET_LISTEN_TIMER_ON(low_rqb_ptr_)    \
// zzxch stcp :    ((low_rqb_ptr_) -> args.listen.timer_on)
// zzxch stcp :
// zzxch stcp : <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
/*---------------------------*/
/* Open connection           */
/*---------------------------*/
/* Enter socket descriptor */
#define STCP_LOW_RQB_SET_OPEN_SOCK_FD(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.open.sock_fd = (value_))

/* Read socket descriptor */
#define STCP_LOW_RQB_GET_OPEN_SOCK_FD(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> args.open.sock_fd)

/*---------------------------------*/
/* Active connection establishment */
/*---------------------------------*/
/* Enter socket descriptor */
#define STCP_LOW_RQB_SET_CONNECT_SOCK_FD(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.connect_with_open.sock_fd = (value_))

/* Read socket descriptor */
#define STCP_LOW_RQB_GET_CONNECT_SOCK_FD(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> args.connect_with_open.sock_fd)

/* Write local address */
#define STCP_LOW_RQB_SET_CONNECT_LOC_ADDR(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.connect_with_open.loc_addr = (value_))

/* Read local address */
#define STCP_LOW_RQB_GET_CONNECT_LOC_ADDR(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> args.connect_with_open.loc_addr)

/* Write remote address */
#define STCP_LOW_RQB_SET_CONNECT_REM_ADDR(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.connect_with_open.rem_addr = (value_))

/* Read remote address */
#define STCP_LOW_RQB_GET_CONNECT_REM_ADDR(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> args.connect_with_open.rem_addr)

// zzxch stcp : >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// zzxch stcp : ??? clarify setting old values not longer used
// zzxch stcp : /* Write timer_on  */
// zzxch stcp : #define STCP_LOW_RQB_SET_CONNECT_TIMER_ON(low_rqb_ptr_, value_)    \
// zzxch stcp :     ((low_rqb_ptr_) -> args.connect_with_open.timer_on = (value_))
// zzxch stcp :
// zzxch stcp : /* Read timer_on  */
// zzxch stcp : #define STCP_LOW_RQB_GET_CONNECT_TIMER_ON(low_rqb_ptr_)    \
// zzxch stcp :     ((low_rqb_ptr_) -> args.connect_with_open.timer_on)
// zzxch stcp :
// zzxch stcp : <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
/*----------------------------------*/
/* Passive connection establishment */
/*----------------------------------*/
/* Enter socket descriptor */
#define STCP_LOW_RQB_SET_ACCEPT_SOCK_FD(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.conn.sock_fd = (value_))

/* Read socket descriptor */
#define STCP_LOW_RQB_GET_ACCEPT_SOCK_FD(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> args.conn.sock_fd)

/* Write local address */
#define STCP_LOW_RQB_SET_ACCEPT_LOC_ADDR(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.conn.loc_addr = (value_))

/* Read local address */
#define STCP_LOW_RQB_GET_ACCEPT_LOC_ADDR(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> args.conn.loc_addr)

/* Write remote address */
#define STCP_LOW_RQB_SET_ACCEPT_REM_ADDR(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.conn.rem_addr = (value_))

/* Read local address */
#define STCP_LOW_RQB_GET_ACCEPT_REM_ADDR(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> args.conn.rem_addr)

// zzxch stcp : >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// zzxch stcp : ??? clarify setting old values not longer used
// zzxch stcp :#define STCP_LOW_RQB_SET_ACCEPT_TIMER_ON(low_rqb_ptr_, value_)    \
// zzxch stcp :    ((low_rqb_ptr_) -> args.conn.timer_on = (value_))
// zzxch stcp :
// zzxch stcp :/* Read timer_on  */
// zzxch stcp :#define STCP_LOW_RQB_GET_ACCEPT_TIMER_ON(low_rqb_ptr_)    \
// zzxch stcp :    ((low_rqb_ptr_) -> args.conn.timer_on)
// zzxch stcp :
// zzxch stcp : <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

/* Write interface_id */
#define STCP_LOW_RQB_SET_ACCEPT_INTERFACE_ID(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.conn.InterfaceID = (value_))

/* Read interface_id */
#define STCP_LOW_RQB_GET_ACCEPT_INTERFACE_ID(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> args.conn.InterfaceID)

/*---------------------------*/
/* Send                      */
/*---------------------------*/
/* Enter socket descriptor */
#define STCP_LOW_RQB_SET_SEND_SOCK_FD(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.data.sock_fd = (value_))

/* Read socket descriptor */
#define STCP_LOW_RQB_GET_SEND_SOCK_FD(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> args.data.sock_fd)

/* Enter data length */
#define STCP_LOW_RQB_SET_SEND_DATA_LEN(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.data.data_length = (value_))

/* Read data length */
#define STCP_LOW_RQB_GET_SEND_DATA_LEN(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> args.data.data_length)

/* Enter buffer length */
#define STCP_LOW_RQB_SET_SEND_BUFFER_LEN(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.data.buffer_length = (value_))

/* Read buffer length */
#define STCP_LOW_RQB_GET_SEND_BUFFER_LEN(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> args.data.buffer_length)

/* Enter flags */
#define STCP_LOW_RQB_SET_SEND_FLAGS(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.data.flags = (value_))

/* Read flags */
#define STCP_LOW_RQB_GET_SEND_FLAGS(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> args.data.flags)

/* Enter offset */
#define STCP_LOW_RQB_SET_SEND_OFFSET(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.data.offset = (value_))

/* Read offset */
#define STCP_LOW_RQB_GET_SEND_OFFSET(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> args.data.offset)

/* Enter buffer pointer */
#define STCP_LOW_RQB_SET_SEND_BUFFER_PTR(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.data.buffer_ptr = (value_))

/* Read buffer pointer */
#define STCP_LOW_RQB_GET_SEND_BUFFER_PTR(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> args.data.buffer_ptr)

// zzxch stcp : >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// zzxch stcp : ??? clarify setting old values not longer used
// zzxch stcp :/* Enter timer_val */
// zzxch stcp :#define STCP_LOW_RQB_SET_SEND_TIMER_VAL(low_rqb_ptr_, value_)    \
// zzxch stcp :    ((low_rqb_ptr_) -> args.data.timer_val = (value_))
// zzxch stcp :
// zzxch stcp :/* Read timer_val */
// zzxch stcp :#define STCP_LOW_RQB_GET_SEND_TIMER_VAL(low_rqb_ptr_)    \
// zzxch stcp :
// zzxch stcp : <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

/*---------------------------*/
/* receive                   */
/*---------------------------*/
/* Enter socket descriptor */
#define STCP_LOW_RQB_SET_RECV_SOCK_FD(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.data.sock_fd = (value_))

/* Read socket descriptor */
#define STCP_LOW_RQB_GET_RECV_SOCK_FD(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> args.data.sock_fd)

/* Enter data length */
#define STCP_LOW_RQB_SET_RECV_DATA_LEN(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.data.data_length = (value_))

/* Read data length */
#define STCP_LOW_RQB_GET_RECV_DATA_LEN(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> args.data.data_length)

/* Enter buffer length */
#define STCP_LOW_RQB_SET_RECV_BUFFER_LEN(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.data.buffer_length = (value_))

/* Read buffer length */
#define STCP_LOW_RQB_GET_RECV_BUFFER_LEN(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> args.data.buffer_length)

/* Enter flags */
#define STCP_LOW_RQB_SET_RECV_FLAGS(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.data.flags = (value_))

/* Read flags */
#define STCP_LOW_RQB_GET_RECV_FLAGS(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> args.data.flags)

/* Enter offset */
#define STCP_LOW_RQB_SET_RECV_OFFSET(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.data.offset = (value_))

/* Read offset */
#define STCP_LOW_RQB_GET_RECV_OFFSET(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> args.data.offset)

/* Enter buffer pointer */
#define STCP_LOW_RQB_SET_RECV_BUFFER_PTR(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.data.buffer_ptr = (value_))

/* Read buffer pointer */
#define STCP_LOW_RQB_GET_RECV_BUFFER_PTR(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> args.data.buffer_ptr)

/* Read PDU pointer */
#define STCP_LOW_RQB_GET_RECV_PDU_PTR(low_rqb_ptr_) \
    (L4_UPPER_MEM_PTR_TYPE)                                  \
	( (LSA_UINT8 L4_UPPER_MEM_ATTR *)((low_rqb_ptr_) -> args.data.buffer_ptr) + \
      ((low_rqb_ptr_) -> args.data.offset) )

// zzxch stcp : >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// zzxch stcp : ??? clarify setting old values not longer used
// zzxch stcp : /* Enter timer_val */
// zzxch stcp : #define STCP_LOW_RQB_SET_RECV_TIMER_VAL(low_rqb_ptr_, value_)    \
// zzxch stcp :     ((low_rqb_ptr_) -> args.data.timer_val = (value_))
// zzxch stcp :
// zzxch stcp : /* Read timer_val */
// zzxch stcp : #define STCP_LOW_RQB_GET_RECV_TIMER_VAL(low_rqb_ptr_)    \
// zzxch stcp :    ((low_rqb_ptr_) -> args.data.timer_val)
// zzxch stcp :
// zzxch stcp : <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
/*----------------------------*/
/* Passively close connection */
/*----------------------------*/
/* Enter socket descriptor */
#define STCP_LOW_RQB_SET_EXCEPTION_SOCK_FD(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.except_rq.sock_fd = (value_))
// zzxch stcp : >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// wrong old
//    ((low_rqb_ptr_) -> args.except.sock_fd = (value_))
// zzxch stcp : <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

/* Read socket descriptor */
#define STCP_LOW_RQB_GET_EXCEPTION_SOCK_FD(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> args.except_rq.sock_fd)
// zzxch stcp : >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// wrong old
//    ((low_rqb_ptr_) -> args.except.sock_fd)
// zzxch stcp : <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

/*---------------------------*/
/* Closing a socket          */
/*---------------------------*/
/* Enter socket descriptor   */
#define STCP_LOW_RQB_SET_CLOSE_SOCK_FD(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> args.close.sock_fd = (value_))

/* Read socket descriptor */
#define STCP_LOW_RQB_GET_CLOSE_SOCK_FD(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> args.close.sock_fd)



/*----------------------------*/
/* Access to error structure  */
/*----------------------------*/
#define STCP_LOW_RQB_GET_ERR_ADDR(low_rqb_ptr_)    \
    (&(low_rqb_ptr_) -> err)

#define STCP_LOW_RQB_GET_ERR_LSA_COMPONENT_ID(low_rqb_ptr_)    \
    ((low_rqb_ptr_) -> err.lsa_component_id)

#define STCP_LOW_RQB_SET_ERR_LSA_COMPONENT_ID(low_rqb_ptr_, value_)    \
    ((low_rqb_ptr_) -> err.lsa_component_id = (value_))



/*****************************************************************************/
/* opcodes for the interface to the lower layer                              */
/*****************************************************************************/

#define STCP_LOW_OPC_OPEN_CHANNEL               SOCK_OPC_OPEN_CHANNEL
#define STCP_LOW_OPC_CLOSE_CHANNEL              SOCK_OPC_CLOSE_CHANNEL
#define STCP_LOW_OPC_OPEN                       SOCK_OPC_OPEN
#define STCP_LOW_OPC_LISTEN                     SOCK_OPC_LISTEN
#define STCP_LOW_OPC_CONNECT                    SOCK_OPC_CONNECT_WITH_OPEN  /* before the CONNECT of STCP an OPEN is performed! */
#define STCP_LOW_OPC_ACCEPT_NOTIFY              SOCK_OPC_ACCEPT_NOTIFY
#define STCP_LOW_OPC_SEND                       SOCK_OPC_SEND
#define STCP_LOW_OPC_RECEIVE                    SOCK_OPC_RECEIVE
#define STCP_LOW_OPC_CLOSE                      SOCK_OPC_CLOSE
#define STCP_LOW_OPC_SELECT                     SOCK_OPC_SELECT
#define STCP_LOW_OPC_EXCEPTION_NOTIFY           SOCK_OPC_EXCEPTION_NOTIFY
#define STCP_LOW_OPC_UNUSED                     SOCK_OPC_UNUSED

/*****************************************************************************/
/* Responses for the interface to the lower layer                            */
/*****************************************************************************/

#define STCP_LOW_RSP_OK                         SOCK_RSP_OK
#define STCP_LOW_RSP_ACTIVE                     SOCK_RSP_OK_ACTIVE
#define STCP_LOW_RSP_SYNTAX_ERROR               SOCK_RSP_ERR_SYNTAX
#define STCP_LOW_RSP_RESOURCE_ERROR             SOCK_RSP_ERR_RESOURCE
#define STCP_LOW_RSP_REF_ERROR                  SOCK_RSP_ERR_REF
#define STCP_LOW_RSP_PARAM_ERROR                SOCK_RSP_ERR_PARAM
#define STCP_LOW_RSP_SEQUENCE_ERROR             SOCK_RSP_ERR_SEQUENCE
#define STCP_LOW_RSP_LOCAL_ABORT                SOCK_RSP_ERR_LOC_ABORT
#define STCP_LOW_RSP_REMOTE_ABORT               SOCK_RSP_ERR_REM_ABORT
#define STCP_LOW_RSP_LOC_NET_ERROR              SOCK_RSP_ERR_LOC_NET
#define STCP_LOW_RSP_REM_NET_ERROR              SOCK_RSP_ERR_REM_NET
#define STCP_LOW_RSP_PROTOCOL_ERROR             SOCK_RSP_ERR_PROTOCOL
#define STCP_LOW_RSP_TIMEOUT_ERROR              SOCK_RSP_ERR_TIMEOUT
#define STCP_LOW_RSP_PATH_ERROR                 SOCK_RSP_ERR_SYS_PATH
#define STCP_LOW_RSP_LOWER_LAYER_ERROR          SOCK_RSP_ERR_LOWER_LAYER
// new file setting ??? #define STCP_LOW_RSP_ADDR_INUSE_ERROR           SOCK_RSP_ADDR_INUSE
#define STCP_LOW_RSP_ADDR_INUSE_ERROR           SOCK_RSP_ERR_ADDR_INUSE


/* ************************************************************************* */
/* Definitions for the lower interface                                       */
/* ************************************************************************* */

/* Identifier for the protocol family/address family */
#define STCP_AF_INET                SOCK_AF_INET


#define STCP_LOW_SEND_ALL       SOCK_SEND_ALL			/* send all data, only then send the confirmation */
#define STCP_LOW_READ_ALL       SOCK_READ_ALL			/* receive required number of data */


// zzxch stcp : >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
/*--------------------------------------------------------------------------*/
/* Filling in / reading the request block of the upper layer                */
/*--------------------------------------------------------------------------*/
/*----------------------------------*/
/* Read/Write the header of the RQB */
/*----------------------------------*/
/* Write the pointer to the next RQB */
#define STCP_UPPER_RQB_SET_NEXT_RQB_PTR(upper_rqb_ptr_, value_) LSA_RQB_SET_NEXT_RQB_PTR(upper_rqb_ptr_, value_)

/* Read the pointer to the next RQB */
#define STCP_UPPER_RQB_GET_NEXT_RQB_PTR(upper_rqb_ptr_)         LSA_RQB_GET_NEXT_RQB_PTR(upper_rqb_ptr_)

/* Read handle */
#define STCP_UPPER_RQB_GET_HANDLE(upper_rqb_ptr_)    \
    ((upper_rqb_ptr_) -> handle)

/*Write handle */
#define STCP_UPPER_RQB_SET_HANDLE(upper_rqb_ptr_, value_)    \
    ((upper_rqb_ptr_) -> handle = (value_))

/* Read opcode */
#define STCP_UPPER_RQB_GET_OPCODE(upper_rqb_ptr_)    \
    ((upper_rqb_ptr_) -> opcode)

/* Write opcode */
#define STCP_UPPER_RQB_SET_OPCODE(upper_rqb_ptr_, value_)    \
    ((upper_rqb_ptr_) -> opcode = (value_))

// zzxch stcp : ?? check value
/* Read user_id */
#define STCP_UPPER_RQB_GET_USER_ID(upper_rqb_ptr_)    \
    ((upper_rqb_ptr_) -> user_id)

// zzxch stcp : ?? check value
/* Write user_id */
#define STCP_UPPER_RQB_SET_USER_ID(upper_rqb_ptr_, value_)    \
    ((upper_rqb_ptr_) -> user_id = (value_))

/* Get user_id as Uvar32*/
#define STCP_UPPER_RQB_GET_USER_ID_UVAR32(upper_rqb_ptr_)   LSA_RQB_GET_USER_ID_UVAR32(upper_rqb_ptr_)

/* General: Read response */
#define STCP_UPPER_RQB_GET_RESPONSE(upper_rqb_ptr_)    \
    ((upper_rqb_ptr_) -> response)

/* General: Write response */
#define STCP_UPPER_RQB_SET_RESPONSE(upper_rqb_ptr_, value_)    \
    ((upper_rqb_ptr_) -> response = (value_) )




// zzxch stcp : ??? clarify setting
/*=============================================================================
 * function name:  STCP_LOCAL_CNS_ENABLED
 *
 * function:	Checks if connection establishment ist enabled by the system
 *              In an H-System (highly available system) the establishment of
 *              incoming local connections may temporarily be disabled while
 *              the system is being updated to redundancy.
 *              In systems which never disable incoming connections this macro
 *              must always return the value LSA_TRUE.
 *
 * parameters:     ...  *  ret_val_ptr:
 *                             Returns: LSA_TRUE	connection establishment is allowed
 *		                            	LSA_FALSE	connection establishment is not allowed
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#define STCP_LOCAL_CNS_ENABLED(mem_lsa_bool_ptr_)   *(mem_lsa_bool_ptr_) = LSA_TRUE


// zzxch stcp : ??? clarify setting
// zzxch skip old_structure ::               apma_pdb_data_lsa.sock_ldb_stcp_stcp.device_id                      = 0; /* Device_id for this channel */
#define STCP_GET_DEVICE_ID(dev_ret_val_, device_id_, loc_addr_, interface_id_, sys_ptr_ )   \
      *(device_id_)   = 0;  /*  dependencies from inputvalues ??? how to identify ????   */ \
      *(dev_ret_val_) = LSA_RET_OK;


// zzxch stcp : ??? temporary  clarify compile
//    #ifndef STCP_REQUEST_UPPER_DONE
//    LSA_VOID  STCP_UPPER_OUT_FCT_ATTR  STCP_REQUEST_UPPER_DONE(
//        L4_UPPER_CALLBACK_FCT_PTR_TYPE  stcp_request_upper_done_ptr,
//        L4_UPPER_RQB_PTR_TYPE           upper_rqb_ptr
//     );
//    #endif
#define STCP_REQUEST_UPPER_DONE(   stcp_request_upper_done_ptr  , upper_rqb_ptr_ )\
  LSAS_RQB_S7PN_REQUEST(upper_rqb_ptr_, stcp_request_upper_done_ptr);
// ????

#define STCP_FATAL_ERROR(_length, _error_ptr)                                \
{                                                                           \
    LSAS_FATAL_ERROR( _error_ptr);                 \
}                                                                           \

/*****************************************************************************/
/*  end of file STCP_CFG.H                                                   */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of STCP_CFG_H */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
