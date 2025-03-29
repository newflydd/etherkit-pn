
#ifndef LSA_CFG_H                        /* ----- reinclude-protection ----- */
#define LSA_CFG_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

/*****************************************************************************/
/*  Copyright (C) 2010 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
/*  This program is protected by German copyright law and international      */
/*  treaties.                                                                */
/*  The use of this software including but not limited to its Source Code    */
/*  is subject to restrictions as agreed in the license agreement between    */
/*  you and Siemens.                                                         */
/*  Copying or distribution is not allowed unless expressly permitted        */
/*  according to your license agreement with Siemens.                        */
/*****************************************************************************/
/*                                                                           */
/*  P r o j e c t         &P: Layer Stack Architecture (LSA)    :P&          */
/*                                                                           */
/*  P a c k a g e         &W: LSA-Global                        :W&          */
/*                                                                           */
/*  C o m p o n e n t     &C: Global Definitions                :C&          */
/*                                                                           */
/*  F i l e               &F: lsa_cfg.h/txt                     :F&          */
/*                                                                           */
/*  V e r s i o n         &V: LSA_GLOB_P02.01.00.00_00.07.02.01 :V&          */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2006-06-28                        :D&          */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*                        Frame for file "lsa_cfg.h".                        */
/*                        ===========================                        */
/*                                                                           */
/*  Configuration for LSA:                                                   */
/*  Defines constants, types and macros for LSA.                             */
/*                                                                           */
/*  This file has to be overwritten during system integration, because       */
/*  some definitions depend on the different system, compiler or             */
/*  operating system.                                                        */
/*                                                                           */
/*****************************************************************************//*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Version        Who  What                                     */
/*  2001-07-26  P02.00.01.001  ws   Basis-Typ LSA_INT neu definiert.         */
/*  2001-07-26  P02.00.01.001  ws   Einige Kommentare ergaenzt.              */
/*  2001-07-26  P02.00.01.001  ws   *_USER_ID_TYPE von lsa_cfg.h nach        */
/*                                  lsa_sys.h verlagert.                     */
/*  2001-07-26  P02.00.01.001  ws   VOID-Cast bei LSA_NULL weggelassen.      */
/*  2001-07-27  P02.00.01.001  ws   __LSA_CFG_H geaendert nach LSA_CFG_H.    */
/*  2001-07-27  P02.00.01.001  ws   LSA_HOST_PTR_COMPARE_TYPE neu definiert. */
/*  2001-07-27  P02.00.01.001  ws   LSA_VOID_PTR_TYPE neu definiert.         */
/*  2001-07-30  P02.00.01.001  ws   Schutz vor erneutem Includieren an den   */
/*                                  Dateianfang verlagert.                   */
/*  2001-09-27  P02.00.01.001  ws   lsa_cfg.h umbenannt nach lsa_cfg.txt     */
/*  2001-10-08  P02.00.01.001  ws   Kommentare ergaenzt.                     */
/*  2001-12-04  P02.00.01.001  ws   Weil user-id eine union ist ergaenzt:    */
/*                                  Attribut LSA_COMMON_MEM_ATTR,            */
/*                                  Konstante LSA_USER_ID_UINT8_UNUSED,      */
/*                                  Makros LSA_USER_ID_INIT() und            */
/*                                  LSA_USER_ID_ARE_EQUAL().                 */
/*  2002-01-29  P02.00.01.002  ws   Kommentare ergaenzt.                     */
/*  2002-01-29  P02.00.01.002  ws   Lokale Parameter in Makros mit "_"       */
/*                                  gekennzeichnet.                          */
/*  2002-01-30  P02.00.01.002  ws   LSA_USER_ID_ARE_EQUAL geaendert: Rueck-  */
/*                                  gabewert per Zeiger zurueckgeben.        */
/*  2002-01-30  P02.00.01.002  ws   Makro LSA_USER_ID_HANDLE() neu.          */
/*  2002-05-17  P2.0.0.0_0.1.4.1 ws LSA_USER_ID_HANDLE() geaendert: Auf die  */
/*                                  union nicht mehr per Pointer zugreifen,  */
/*                                  sondern ueber den Strukturoperator.      */
/*  2002-07-26  V2.0.0.0_0.1.6.1 ws ARTS 51557: LSA_HOST_PTR_ARE_EQUAL() von */
/*                                  lsa_sys.h nach lsa_cfg.h verlagert.      */
/*  2002-10-28  P2.1.0.0_0.1.1.1 ws ARTS 141840: LSA_CONST_MEM_ATTR neu.     */
/*  2002-10-28  P2.1.0.0_0.1.1.1 ws ARTS 141708: LSA_UINT neu.               */
/*  2003-06-05  P2.1.0.0_0.2.1.1 ws ARTS+ AP00035493: Compiler-Warnings      */
/*                                  verhindern: Makro LSA_UNUSED_ARG neu.    */
/*  2003-11-24  P2.1.0.0_0.3.1.1 ws Trace neu: Konstanten LSA_TRACE_LEVEL_*  */
/*                                  und Makros LSA_(INST_)TRACE_* neu.       */
/*  2004-06-30  P2.1.0.0_0.4.1.1 ws ARTS+ AP00132068: Fuer C++-Compiler      */
/*                                  Datei mit C-Bindung gekennzeichnet;      */
/*                                  ausserdem LSA_EXTERN angepasst.          */
/*  2004-06-30  P2.1.0.0_0.4.1.1 ws ARTS+ AP00167458: Copyright abgeaendert. */
/*  2004-11-30  P2.1.0.0_0.4.2.1 ws Wegen Tasking-Compiler im Kommentar      */
/*                                  einige Umlaute durch andere Zeichen      */
/*                                  ersetzt.                                 */
/*  2005-01-04  P2.1.0.0_0.4.3.1 ws ARTS+ AP00197761: timer_id:              */
/*                                  LSA_TIMER_ID_TYPE neu.                   */
/*  2005-07-22  P2.1.0.0_0.5.1.1 ws Trace: Neu: Pseudo-Makros als Scanner-   */
/*                                  Kennung fuer die symbolische Anzeige von */
/*                                  Konstanten hier fuer den Compiler als    */
/*                                  Leer-Makros definiert.                   */
/*  2006-03-27  P2.1.0.0_0.7.1.1 ws ARTS+ AP00200674: Endianess: Compiler-   */
/*                                  schalter LSA_HOST_ENDIANESS_BIG und      */
/*                                  LSA_HOST_ENDIANESS_LITTLE neu.           */
/*  2006-03-28  P2.1.0.0_0.7.1.1 ws ARTS+ AP00268519: LSA_RQB_HEADER-        */
/*                                  Definition neu: RQB-Header-Definition    */
/*                                  von prefix_cfg.txt und l4_cfg.txt nach   */
/*                                  lsa_cfg.txt verlagert.                   */
/*  2006-04-05  P2.1.0.0_0.7.1.1 ws ARTS+ AP00277026: 64 Bit-Datentypen neu. */
/*  2007-03-22  P2.1.0.0_0.8.1.1 ws ARTS+ AP00424201: Variable priority im   */
/*                                  LSA_RQB_HEADER neu.                      */
/*  2007-07-05  P2.1.0.0_0.9.1.1 ws Namensumbenennung: Instanziierter Trace  */
/*                                  -> Indizierter Trace: Neue Makros        */
/*                                  definiert und alte Makros auf die neuen  */
/*                                  Makros abgebildet. Kommentare angepasst. */
/*  2007-07-05  P2.1.0.0_0.9.1.1 ws Wegen Trace-Scanner-Problem bei der      */
/*                                  Definition von LSA_RQB_HEADER einige     */
/*                                  Backslashes geloescht.                   */
/*  2007-08-08 P2.1.0.0_0.10.1.1 ws ARTS+ AP00498913: LSA_CHAR neu:          */
/*                                  Kommentar ergaenzt.                      */
/*  2007-08-08 P2.1.0.0_0.10.1.1 ws ARTS+ AP00501849: LSA_VOID_PTR_TYPE      */
/*                                  ergaenzt um LSA_COMMON_MEM_ATTR.         */
/*  2008-01-25 P2.1.0.0_0.11.1.1 ws ARTS+ AP00565054: Wegen 64 Bit-Plattform */
/*                                  Konstante LSA_RQB_ERROR_CODE_SIZE        */
/*                                  angepasst: Hier Makros LSA_MIN() und     */
/*                                  LSA_MAX() neu.                           */
/*  2008-12-01 P2.1.0.0_0.11.2.1 ws Im Kommentar Schreibfehler korrigiert.   */
/*                                                                           */
/*****************************************************************************/



/*===========================================================================*/
/* basic types for lsa according ANSI C1999 Standard (ISO/IEC 9899:1999)     */
/*                                                                           */
/* ANSI C1999: "The header <stdint.h> declares sets of integer types having  */
/* specified widths."                                                        */
/* For example: uint8_t, uint16_t, uint32_t, int8_t, int16_t, int32_t        */
/*===========================================================================*/
#include "compiler.h"	// &&&2do for DK4.0.0
#include "lsa_types.h"


#if (PNIO_TRACE == PNIO_TRACE_DK_LSA)
    #define LSA_TRACE_ENABLE
#elif defined (LSA_TRACE_ENABLE)
    #undef  LSA_TRACE_ENABLE
#endif


/*=============================================================================
 *       Endianess:
 * If you have a computer system whitch stors most significant byte at the
 * lowest address of the word/doubleword: Define this
#define LSA_HOST_ENDIANESS_BIG
 * else define this
#define LSA_HOST_ENDIANESS_LITTLE
 *===========================================================================*/

#if PNIO_BIG_ENDIAN
	#define LSA_HOST_ENDIANESS_BIG
#else
    #define LSA_HOST_ENDIANESS_LITTLE
#endif

/*===========================================================================*/
/*                          data-attribute for lsa                           */
/*===========================================================================*/

#define LSA_COMMON_MEM_ATTR     /* greatest common data attribute */
#define LSA_CONST_MEM_ATTR      /* const data attribute */



/*=====  types for system integration  =====*/
/*=============================================================================
 *       system-path for the channels
 * examples:
#define LSA_SYS_PATH_TYPE           LSA_UINT8
#define LSA_SYS_PATH_TYPE           LSA_UINT16
#define LSA_SYS_PATH_TYPE           LSA_UINT32
#define LSA_SYS_PATH_TYPE           LSA_VOID_PTR_TYPE
 *===========================================================================*/
#define LSA_SYS_PATH_TYPE           LSAS_PATH_TYPE


/*=============================================================================
 *      system-pointer for the channels
 * examples:
#define LSA_SYS_PTR_TYPE            LSA_UINT16
#define LSA_SYS_PTR_TYPE            LSA_UINT32
#define LSA_SYS_PTR_TYPE            LSA_VOID_PTR_TYPE
 *===========================================================================*/
#define LSA_SYS_PTR_TYPE            LSAS_SYSTEM_PTR


/*=====  attributes for system integration  =====*/
/*  this atribute exists only within the CPU3XX - LSA - integration */
/*=============================================================================
 *       attribute for local memory reference to store timer_id
 * Examples:
#define LSA_LOC_TIMER_MEM_ATTR
#define LSA_LOC_TIMER_MEM_ATTR      shuge
#define LSA_LOC_TIMER_MEM_ATTR      huge
=============================================================================*/

#define LSA_LOC_TIMER_MEM_ATTR

#define LSA_ALLOC_MEM(_mem_ptr_ptr, _length, module_id)                     \
{                                                                           \
    apma_alloc_mem((void**)_mem_ptr_ptr, _length, module_id);               \
}                                                                           \

#define LSA_ALLOC_PACKET_MEM(_mem_ptr_ptr, _length, module_id)              \
{                                                                           \
    apma_alloc_packet_mem((void**)_mem_ptr_ptr, _length, module_id);        \
}
#define LSA_FREE_MEM(_ret_val_ptr, _mem_ptr, module_id)                     \
{                                                                           \
    int ret_value;                                                          \
    apma_free_mem(&ret_value, _mem_ptr, module_id);                         \
    *(_ret_val_ptr) = (LSA_UINT16) ret_value;                               \
}                                                                           \


/*=============================================================================
 *      lsa common request block type
 *===========================================================================*/

/*=====  definition of parameter types  =====*/
#define LSA_OPCODE_TYPE             LSA_UINT8    /* for opcode */
#define LSA_HANDLE_TYPE             LSA_UINT8    /* for channel-handle */
                      /* Use only the types LSA_UINT8, LSA_UINT16 and        */
                      /* LSA_UINT32, otherwise it can't be used as an index. */
#define LSA_COMP_ID_TYPE            LSA_UINT16   /* for lsa-component-id, */
                                                 /* see lsa_sys.h */
/* Hint: LSA_RESPONSE_TYPE, LSA_USER_ID_TYPE and LSA_PRIORITY_TYPE
 *       definitions see lsa_sys.h
 */

  /*====  Definition of LSA-global RQB-HEADER:  =====*/
/* The order of the parameters can be changed!
 * All parameters are mandatory!
 * Do not change names of the types and names of variables!
 * The length of the LSA-RQB-HEADER must be 2/4-byte-aligned according to
 * the system! If necessary, dummy-bytes must be added!
 * If necessary during system integration, additional parameters can be added!
 * For example:
    LSA_COMP_ID_TYPE                comp_id;
 */

      #define LSA_RQB_HEADER(QUEUE_PTR_TYPE_)                                                      \
      /*                            |                                                           */\
      /*                            |                                                           */\
      /*       +--------------------+                                                           */\
      /*       |                                                                                */\
      /*       \/                                                                               */\
          QUEUE_PTR_TYPE_             next_rqb_ptr;               /* mandatory */                 \
                                      /* pointer to next RQB */                                   \
          QUEUE_PTR_TYPE_             prev_rqb_ptr;               /* mandatory */                 \
                                      /* pointer to previous RQB */                               \
          LSA_COMP_ID_TYPE            comp_id;                    /* only for lsa */              \
          LSA_OPCODE_TYPE             opcode;                     /* mandatory */                 \
                                      /* req: opcode, see *_usr.h */                              \
                                      /* cnf: unchanged           */                              \
          LSA_HANDLE_TYPE             handle;                     /* mandatory */                 \
                                      /* req: if *_OPC_OPEN_CHANNEL: -     */                     \
                                      /*      else: channel-handle of      */                     \
                                      /*            component              */                     \
                                      /* cnf:       channel-handle of user */                     \
          LSA_USER_ID_TYPE            user_id;                    /* mandatory */                 \
                                      /* req: id of user */                                       \
                                      /* cnf: unchanged  */                                       \
          LSA_UINT16                  align1;                     /* for 32-Bit alignment */      \
          LSA_UINT16                  response;                   /* mandatory */                 \
          LSA_VOID                    (*method_ptr) (void *);                                     \


/*====  Definition of LSA-RQB-TRAILER:  =====*/
/* If necessary during system integration, additional parameters can be
 * added!
 */
#define LSA_RQB_TRAILER
    /* nothing by default */


/*=============================================================================
 *      for pointer-compare, see LSA_HOST_PTR_ARE_EQUAL()
 * Type must correspond to the memory need of a pointer on the respective
 * target system.
 * examples:
#define LSA_HOST_PTR_COMPARE_TYPE   LSA_UINT8
#define LSA_HOST_PTR_COMPARE_TYPE   LSA_UINT16
#define LSA_HOST_PTR_COMPARE_TYPE   LSA_UINT32
 *===========================================================================*/
#define LSA_HOST_PTR_COMPARE_TYPE   LSA_UINT32

/*=============================================================================
 *      timer_id type
 * See parameter timer_id at *_ALLOC_TIMER, *_START_TIMER, *_STOP_TIMER,
 * *_FREE_TIMER and *_timeout().
 * examples:
#define LSA_TIMER_ID_TYPE   LSA_UINT16
#define LSA_TIMER_ID_TYPE   LSA_UINT32
 *===========================================================================*/
#define LSA_TIMER_ID_TYPE   LSA_UINT16



/*===========================================================================*/
/*                         basic constants for lsa                           */
/*===========================================================================*/

#define LSA_FALSE                   ((LSA_BOOL) 0)
                               /* for variables of type LSA_BOOL: must be 0! */
#define LSA_TRUE                    1
                        /* for variables of type LSA_BOOL: must be (!FALSE)! */
#define LSA_NULL                    0
#define LSA_OK                      1                /* only allowed: 0 or 1 */
#define LSA_EXTERN                  extern
#define LSA_USER_ID_UINT8_UNUSED    0xff
#define LSA_RESULT                   LSA_UINT16



/*===========================================================================*/
/*              additional constants for amos - integration                  */
/*===========================================================================*/


#define LSA_ALLOC_TIMER(_ret_val_ptr, _timer_id_ptr, _timer_type, _time_base, _cbf, _module_id)\
{                                                                               \
    *(_timer_id_ptr) = st_alloc_timer((_timer_type), (_cbf), (_time_base), (_module_id));     \
                                                                                \
    switch( *(_timer_id_ptr) )                                                  \
    {                                                                           \
         case ST_ERR_NO_TIMER:                                                  \
         case ST_ERR_PARAMETER:                                                 \
            * (_ret_val_ptr) = LSA_RET_ERR_PARAM;                               \
            break;                                                              \
        default:                                                                \
         {                                                                      \
            * (_ret_val_ptr) = LSA_RET_OK;                                      \
          /*  timer_cb_elements[ *(_timer_id_ptr) ].cbf = (_cbf); */                \
          /*  timer_cb_elements[ *(_timer_id_ptr) ].timer_id = *(_timer_id_ptr); */ \
         }                                                                      \
            break;                                                              \
    }                                                                           \
}

#define LSA_START_TIMER(_ret_val_ptr, _timer_id, _user_id, _time, _module_id)               \
{                                                                               \
    * (_ret_val_ptr) = st_start_timer((_timer_id), (_time), (_user_id), (_module_id));        \
                                                                                \
    switch( * (_ret_val_ptr) )                                                  \
    {                                                                           \
        case ST_OK:                                                             \
        case ST_OK_TIMER_RETRIGGERED:                                           \
        {                                                                       \
            * (_ret_val_ptr) = LSA_RET_OK;                                      \
          /*  timer_cb_elements[ (_timer_id) ].user_id = (_user_id);  */            \
        }                                                                       \
            break;                                                              \
        default:                                                                \
            * (_ret_val_ptr) = LSA_RET_ERR_PARAM;                               \
            break;                                                              \
    }                                                                           \
}

#define LSA_STOP_TIMER(_ret_val_ptr, _timer_id, _module_id)                     \
{                                                                               \
    * (_ret_val_ptr) = st_stop_timer( (_timer_id), (_module_id) );              \
                                                                                \
    switch( * (_ret_val_ptr) )                                                  \
    {                                                                           \
        case ST_OK:                                                             \
        case ST_OK_TIMER_ALREADY_STOPPED:                                                             \
            * (_ret_val_ptr) = LSA_RET_OK;                                      \
            break;                                                              \
        default:                                                                \
            * (_ret_val_ptr) = LSA_RET_ERR_PARAM;                               \
            break;                                                              \
    }                                                                           \
}

#define LSA_FREE_TIMER(_ret_val_ptr, _timer_id, _module_id)                     \
{                                                                               \
    * (_ret_val_ptr) = st_free_timer( (_timer_id), (_module_id) );              \
                                                                                \
    switch( * (_ret_val_ptr) )                                                  \
    {                                                                           \
        case ST_OK:                                                             \
            * (_ret_val_ptr) = LSA_RET_OK;                                      \
            break;                                                              \
        default:                                                                \
            * (_ret_val_ptr) = LSA_RET_ERR_PARAM;                               \
            break;                                                              \
    }                                                                           \
}                                                                               \


/*=============================================================================
 *      lsa-trace: trace levels
 * If the LSA component LTRC is used for trace in the target system, nothing
 * needs here to be changed. If the LSA component LTRC isn't used for trace in
 * the target system, then the LSA trace levels can be redefined here.
 *===========================================================================*/
#define LSA_TRACE_LEVEL_OFF         1
                                       /* This trace entry is never created! */
#define LSA_TRACE_LEVEL_FATAL       2
                                       /* Serious not tradable error         */
                                       /* occurred!                          */
#define LSA_TRACE_LEVEL_ERROR       3
                                       /* Serious tradable error occurred!   */
#define LSA_TRACE_LEVEL_UNEXP       4
                                       /* Unexpected situation occurred!     */
#define LSA_TRACE_LEVEL_WARN        5
                                       /* Warning: Program exits productive  */
                                       /* branch!                            */
#define LSA_TRACE_LEVEL_NOTE_HIGH   6
                                       /* Important information productive   */
                                       /* case: High level!                  */
#define LSA_TRACE_LEVEL_NOTE        7
                                       /* Important informationproductive    */
                                       /* case: Normal level!                */
#define LSA_TRACE_LEVEL_NOTE_LOW    8
                                       /* Important information productive   */
                                       /* case: Low level!                   */
#define LSA_TRACE_LEVEL_CHAT        9
                                       /* Important code places within the   */
                                       /* software!                          */



/*===========================================================================*/
/*                           basic macros for lsa                            */
/*===========================================================================*/

/*=============================================================================
 *       function pointer for system integration
 * examples:
 *
 * for BSO-Tasking-compiler:
#define LSA_FCT_PTR(code_attr_, fct_ptr_) \
    code_attr_ (* fct_ptr_)
 *
 * or for Visual C++-compiler:
#define LSA_FCT_PTR(code_attr_, fct_ptr_) \
    (* fct_ptr_)
 *
 * or for other compilers:
#define LSA_FCT_PTR(code_attr_, fct_ptr_) \
    (code_attr_ * fct_ptr_)
 *===========================================================================*/
#define LSA_FCT_PTR(code_attr_, fct_ptr_)   code_attr_ (* fct_ptr_)



/*====  for user_id:  =====*/

/*=============================================================================
 *       writes a handle into the user-id and reads a handle from the user-id
 *
 * for    #define LSA_HANDLE_TYPE  LSA_UINT8:
#define LSA_USER_ID_HANDLE(lsa_user_id_) \
    ( (lsa_user_id_).uvar8 )
 *
 * or for #define LSA_HANDLE_TYPE  LSA_UINT16:
#define LSA_USER_ID_HANDLE(lsa_user_id_) \
    ( (lsa_user_id_).uvar16 )
 *
 * or for #define LSA_HANDLE_TYPE  LSA_UINT32:
#define LSA_USER_ID_HANDLE(lsa_user_id_) \
    ( (lsa_user_id_).uvar32 )
 *===========================================================================*/
#define LSA_USER_ID_HANDLE(lsa_user_id_) \
    ( (lsa_user_id_).uvar8 )

/* initialize user-id as unused
 */
#define LSA_USER_ID_INIT(lsa_user_id_ptr_)                      \
{                                                               \
    LSA_INT                         i_;                         \
    LSA_UINT8 LSA_COMMON_MEM_ATTR * ptr_;                       \
                                                                \
    ptr_ = (LSA_UINT8 LSA_COMMON_MEM_ATTR *)(lsa_user_id_ptr_); \
                                                                \
    for (i_ = sizeof(LSA_USER_ID_TYPE); i_ > 0; i_--)           \
    {                                                           \
         *ptr_++ = LSA_USER_ID_UINT8_UNUSED;                    \
    }                                                           \
}

/* compare two user-ids
 */
#define LSA_USER_ID_ARE_EQUAL(ret_val_ptr_, lsa_user_id_ptr1_,    \
                                            lsa_user_id_ptr2_)    \
{                                                                 \
    LSA_INT                         i_;                           \
    LSA_UINT8 LSA_COMMON_MEM_ATTR * ptr1_;                        \
    LSA_UINT8 LSA_COMMON_MEM_ATTR * ptr2_;                        \
                                                                  \
    ptr1_ = (LSA_UINT8 LSA_COMMON_MEM_ATTR *)(lsa_user_id_ptr1_); \
    ptr2_ = (LSA_UINT8 LSA_COMMON_MEM_ATTR *)(lsa_user_id_ptr2_); \
    *(ret_val_ptr_) = LSA_TRUE;                                   \
                                                                  \
    for (i_ = sizeof(LSA_USER_ID_TYPE); i_ > 0; i_--)             \
    {                                                             \
        if (*ptr1_++ != *ptr2_++)                                 \
        {                                                         \
            *(ret_val_ptr_) = LSA_FALSE;                          \
            break;                                                \
        }                                                         \
    }                                                             \
}


/*====  for pointer compare:  =====*/

/* So that pointer comparisons are executed correctly also at the segmented
 * memory model (this means page and offset of the pointers are compared with
 * each other), all LSA-components use the following macro.
 * The macro usually doesn't have to be changed. It must be adjusted in certain
 * cases only at the BSO-Tasking/80C166-compiler.
 */
#define LSA_HOST_PTR_ARE_EQUAL(ptr1_,  ptr2_) \
   ( (LSA_HOST_PTR_COMPARE_TYPE)(ptr1_) == (LSA_HOST_PTR_COMPARE_TYPE)(ptr2_) )


/*====  for unused function parameters:  =====*/

/* If in a LSA component a input parameter of a function is unused, it comes to
 * a compiler warning. That may happen, because of compiler switches in that
 * function. The following macro invoked in that function prevents the compiler
 * warning, if the macro uses the input parameter formally.
 */
#define LSA_UNUSED_ARG(arg_)  {(void)(arg_);}


/*====  for comparison:  =====*/
#define LSA_MIN(x_, y_)     (((x_) <= (y_)) ? (x_) : (y_))
#define LSA_MAX(x_, y_)     (((x_) >= (y_)) ? (x_) : (y_))



/*=============================================================================
 *  set and get parameter of the rqb-header:
 *  this macros exists only within the CPU3XX - LSA - integration
 *===========================================================================*/
#define LSA_RQB_SET_NEXT_RQB_PTR(rqb_ptr_, value_)      ( (rqb_ptr_)->next_rqb_ptr = (value_) )
#define LSA_RQB_GET_NEXT_RQB_PTR(rqb_ptr_)              ( (rqb_ptr_)->next_rqb_ptr )

#define LSA_RQB_SET_PREV_RQB_PTR(rqb_ptr_, value_)      ( (rqb_ptr_)->prev_rqb_ptr = (value_) )
#define LSA_RQB_GET_PREV_RQB_PTR(rqb_ptr_)              ( (rqb_ptr_)->prev_rqb_ptr )

#define LSA_RQB_SET_OPCODE(rqb_ptr_, value_)            ( (rqb_ptr_)->opcode = (value_) )
#define LSA_RQB_GET_OPCODE(rqb_ptr_)                    ( (rqb_ptr_)->opcode )

#define LSA_RQB_SET_HANDLE(rqb_ptr_, value_)            ( (rqb_ptr_)->handle = (value_) )
#define LSA_RQB_GET_HANDLE(rqb_ptr_)                    ( (rqb_ptr_)->handle )

#define LSA_RQB_SET_RESPONSE(rqb_ptr_, value_)          ( (rqb_ptr_)->response = (value_) )
#define LSA_RQB_GET_RESPONSE(rqb_ptr_)                  ( (rqb_ptr_)->response )

#define LSA_RQB_SET_USER_ID_UVAR32(rqb_ptr_, value_)    ( (rqb_ptr_)->user_id.uvar32 = (value_) )
#define LSA_RQB_GET_USER_ID_UVAR32(rqb_ptr_)            ( (rqb_ptr_)->user_id.uvar32 )

#define LSA_RQB_SET_USER_ID_UVAR16(rqb_ptr_, value_)    ( (rqb_ptr_)->user_id.uvar16 = (value_) )
#define LSA_RQB_GET_USER_ID_UVAR16(rqb_ptr_)            ( (rqb_ptr_)->user_id.uvar16 )

#define LSA_RQB_SET_USER_ID_UVAR8(rqb_ptr_, value_)     ( (rqb_ptr_)->user_id.uvar8 = (value_) )
#define LSA_RQB_GET_USER_ID_UVAR8(rqb_ptr_)             ( (rqb_ptr_)->user_id.uvar8 )

#define LSA_RQB_SET_USER_ID_UVAR16_ARRAY_LOW(rqb_ptr_, value_)  ( (rqb_ptr_)->user_id.uvar16_array[0] = (value_) )
#define LSA_RQB_SET_USER_ID_UVAR16_ARRAY_HIGH(rqb_ptr_, value_) ( (rqb_ptr_)->user_id.uvar16_array[1] = (value_) )

#define LSA_RQB_GET_USER_ID_UVAR16_ARRAY_LOW(rqb_ptr_)          ( (rqb_ptr_)->user_id.uvar16_array[0] )
#define LSA_RQB_GET_USER_ID_UVAR16_ARRAY_HIGH(rqb_ptr_)         ( (rqb_ptr_)->user_id.uvar16_array[1] )

#define LSA_RQB_SET_USER_ID_UVAR8_ARRAY_2(rqb_ptr_, value_)     {(rqb_ptr_)->user_id.uvar8_array[2] = (value_);}
#define LSA_RQB_GET_USER_ID_UVAR8_ARRAY_2(rqb_ptr_)             ((rqb_ptr_)->user_id.uvar8_array[2])

#define LSA_RQB_SET_USER_ID_UVAR8_ARRAY_3(rqb_ptr_, value_)     {(rqb_ptr_)->user_id.uvar8_array[3] = (value_);}
#define LSA_RQB_GET_USER_ID_UVAR8_ARRAY_3(rqb_ptr_)             ((rqb_ptr_)->user_id.uvar8_array[3])

#define LSA_RQB_SET_USER_ID_PTR(rqb_ptr_, value_)               ( (rqb_ptr_)->user_id.void_ptr = (value_) )
#define LSA_RQB_GET_USER_ID_PTR(rqb_ptr_)                       ( (rqb_ptr_)->user_id.void_ptr )

#define LSA_RQB_SET_METHOD_PTR(rqb_ptr_, value_)                ( (rqb_ptr_)->method_ptr = (void (*) (void *)) (value_) )
#define LSA_RQB_GET_METHOD_PTR(rqb_ptr_)                        ( (rqb_ptr_)->method_ptr )


/* If necessary, other user-id-macros must be added! */


#define LSA_RQB_SET_COMP_ID(rqb_ptr_, value_)                   ( (rqb_ptr_)->comp_id = (value_) )
#define LSA_RQB_GET_COMP_ID(rqb_ptr_)                           ( (rqb_ptr_)->comp_id )

/* For mq-Debugging */
#if defined (DEBUG_MQ_RQB_HEADER)
  #define LSA_RQB_SET_MQ_SEND_ID(rqb_ptr_, value_)              ( (rqb_ptr_)->mq_send_id = (value_) )
  #define LSA_RQB_GET_MQ_SEND_ID(rqb_ptr_)                      ( (rqb_ptr_)->mq_send_id )

  #define LSA_RQB_SET_MQ_RECEIVE_ID(rqb_ptr_, value_)           ( (rqb_ptr_)->mq_receive_id = (value_) )
  #define LSA_RQB_GET_MQ_RECEIVE_ID(rqb_ptr_)                   ( (rqb_ptr_)->mq_receive_id )

  #define LSA_RQB_SET_MQ_I_IP(rqb_ptr_, value_)                 ( (rqb_ptr_)->mq_i.i.ip  = (value_) )
  #define LSA_RQB_GET_MQ_I_IP(rqb_ptr_)                         ( (rqb_ptr_)->mq_i.i.ip )

  #define LSA_RQB_SET_MQ_I_CSP(rqb_ptr_, value_)                ( (rqb_ptr_)->mq_i.i.csp = (value_) )
  #define LSA_RQB_GET_MQ_I_CSP(rqb_ptr_, value_)                ( (rqb_ptr_)->mq_i.i.csp )

  #define LSA_RQB_SET_MQ_I_PTR(rqb_ptr_, value_)                ( (rqb_ptr_)->mq_i.ptr = (value_) )
  #define LSA_RQB_GET_MQ_I_PTR(rqb_ptr_, value_)                ( (rqb_ptr_)->mq_i.ptr )

#endif //( DEBUG_MQ_RQB_HEADER )



/*=============================================================================
 *  macros to correct size to alignment (thanks BETR)
 *  this macros exists only within the CPU3XX - LSA - integration
 *===========================================================================*/

// corrects size to an even value (increments size by 1 byte if necessary)
//      returns: corrected size
#define LSA_EVEN_SIZE__(size)                                                                   \
                                                                                                \
        (((size) & 1)                                                                           \
                ? ((size) + 1)                                                                  \
                : (size))                                                                       \

// corrects size to an even value (increments size by 1 byte if necessary)
//      returns: corrected size

#define LSA_DWORD_SIZE__(size)                                                                  \
                                                                                                \
        (((size) & 3)                                                                           \
                ? ((((size) + 3)>>2)<<2)                                                        \
                : (size))                                                                       \

                                                                                               \

#define LSA_SWAP_U16(short_ptr)                                                                 \
{                                                                                               \
    *short_ptr = ((*short_ptr & 0xFF00) >> 8)                                                   \
               | ((*short_ptr & 0x00FF) << 8);                                                  \
}

#define LSA_SWAP_U32(long_ptr)                                                                  \
{                                                                                               \
    *long_ptr = ((*long_ptr & 0xFF000000) >> 24)                                                \
              | ((*long_ptr & 0x00FF0000) >>  8)                                                \
              | ((*long_ptr & 0x0000FF00) <<  8)                                                \
              | ((*long_ptr & 0x000000FF) << 24);                                               \
}

#ifdef PNIOD_PLATFORM		// devkit ??

	#define LSA_MEMSET(dst,val,len)                                                                 \
	{                                                                                               \
		OsMemSet (dst,val,len);                                                                        \
	}                                                                                               \

	#define LSA_MEMCPY(dst,src,len)                                                                 \
	{                                                                                               \
		OsMemCpy(dst,src,len);                                                                        \
	}                                                                                               \

	#define LSA_MEMCMP(_a,_b,_len)                                                                  \
	(                                                                                               \
		OsMemCmp((void *)(_a), (void *)(_b), _len) ? LSA_FALSE : LSA_TRUE                 \
	)                                                                                               \

	#define LSA_STRCPY(dst,src)                                                                     \
	{                                                                                               \
		OsStrCpy(dst,src);                                                                            \
	}                                                                                               \

	#define LSA_STRLEN(str)                                                                         \
	(                                                                                               \
		OsStrLen((char*)str)                                                                                 \
	)                                                                                               \

	#define LSA_SSCANF                                                                              \
	(                                                                                               \
		OsScanf                                                                                      \
	)                                                                                               \

	#define LSA_SPRINTF                                                                             \
																									\
		PNIO_sprintf                                                                                     \

	#define LSA_SRAND(base)                                                                         \
	{                                                                                               \
		OsSrand(base);                                                                                \
	}                                                                                               \

	#define LSA_RAND()                                                                              \
	(                                                                                               \
		OsRand()                                                                                      \
	)                                                                                               \

	#define LSA_HTONS(host_short)                                                                   \
	(                                                                                               \
		((host_short & 0xFF00) >> 8) | ((host_short & 0x00FF) << 8)                                 \
	)                                                                                               \

	#define LSA_HTONL(host_long)                                                                    \
	(                                                                                               \
		OsHtonl (host_long)                                               \
	)                                                                                               \

	#define LSA_NTOHS(network_short)                                                                \
	(                                                                                               \
		OsNtohs (network_short)                                                \
	)                                                                                               \

	#define LSA_NTOHL(network_long)                                                                 \
	(                                                                                               \
		OsNtohl (network_long)                                               \
	)
#else
	#define LSA_MEMSET(dst,val,len)                                                                 \
	{                                                                                               \
		memset(dst,val,len);                                                                        \
	}                                                                                               \

	#define LSA_MEMCPY(dst,src,len)                                                                 \
	{                                                                                               \
		memcpy(dst,src,len);                                                                        \
	}                                                                                               \

	#define LSA_MEMCMP(_a,_b,_len)                                                                  \
	(                                                                                               \
		memcmp((const void *)(_a), (const void *)(_b), _len) ? LSA_FALSE : LSA_TRUE                 \
	)                                                                                               \

	#define LSA_STRCPY(dst,src)                                                                     \
	{                                                                                               \
		strcpy(dst,src);                                                                            \
	}                                                                                               \

	#define LSA_STRLEN(str)                                                                         \
	(                                                                                               \
		strlen((char*)str)                                                                                 \
	)                                                                                               \

	#define LSA_SSCANF                                                                              \
	(                                                                                               \
		sscanf                                                                                      \
	)                                                                                               \

	#define LSA_SPRINTF                                                                             \
																									\
		sprintf                                                                                     \

	#define LSA_SRAND(base)                                                                         \
	{                                                                                               \
		srand(base);                                                                                \
	}                                                                                               \

	#define LSA_RAND()                                                                              \
	(                                                                                               \
		rand()                                                                                      \
	)                                                                                               \

	#define LSA_HTONS(host_short)                                                                   \
	(                                                                                               \
		((host_short & 0xFF00) >> 8) | ((host_short & 0x00FF) << 8)                                 \
	)                                                                                               \

	#define LSA_HTONL(host_long)                                                                    \
	(                                                                                               \
			((host_long & 0xFF000000) >> 24)                                                        \
		  | ((host_long & 0x00FF0000) >>  8)                                                        \
		  | ((host_long & 0x0000FF00) <<  8)                                                        \
		  | ((host_long & 0x000000FF) << 24)                                                        \
	)                                                                                               \

	#define LSA_NTOHS(network_short)                                                                \
	(                                                                                               \
			((network_short & 0xFF00) >> 8)                                                         \
		  | ((network_short & 0x00FF) << 8)                                                         \
	)                                                                                               \

	#define LSA_NTOHL(network_long)                                                                 \
	(                                                                                               \
			((network_long & 0xFF000000) >> 24)                                                     \
		  | ((network_long & 0x00FF0000) >>  8)                                                     \
		  | ((network_long & 0x0000FF00) <<  8)                                                     \
		  | ((network_long & 0x000000FF) << 24)                                                     \
	)

#endif

//typedef struct lsa_fatal_error_tag LSA_FATAL_ERROR_TYPE;
//typedef LSA_FATAL_ERROR_TYPE * APMA_HUGE_LSA_ERROR_PTR;
//typedef Unsigned16 APMA_PDB_FW_MODULE_ID_TYPE;

//extern    void    apma_spec_fatal_error   (APMA_PDB_FW_MODULE_ID_TYPE fw_module_id, APMA_HUGE_LSA_ERROR_PTR error_detail_ptr);

#define LSA_FATAL_ERROR(module_id, error_ptr)                                                   \
{                                                                                               \
    apma_spec_fatal_error(module_id, error_ptr);                                                \
}                                                                                               \

#define LSA_GET_PATH_INFO(ret_val_ptr,sys_ptr_ptr,detail_ptr_ptr,path,layer)                        \
{                                                                                                   \
    *(ret_val_ptr) = lsas_get_path_info(sys_ptr_ptr,(LSAS_DETAIL_PTR_PTR)detail_ptr_ptr,path,layer);\
}                                                                                                   \

#define LSA_RELEASE_PATH_INFO(ret_val_ptr,sys_ptr,detail_ptr)                                   \
{                                                                                               \
    *(ret_val_ptr) = lsas_release_path_info(sys_ptr,(LSAS_DETAIL_PTR)detail_ptr);               \
}                                                                                               \

typedef LSA_VOID LSA_FCT_PTR(LSA_COMMON_MEM_ATTR, LSA_CBF_TYPE)(LSA_VOID_PTR_TYPE);


/*====  for lsa-trace:  =====*/

/*=============================================================================
 * If the LSA component LTRC is used for trace in the target system, nothing
 * needs here to be changed. If the LSA component LTRC isn't used for trace in
 * the target system, then the LSA trace macros can be redefined here.
 *
 * Note: There aren't any prototypes for these macros so that the target system
 * defines the pointer size and not LSA.
 * A function can be nevertheless invoked within the macro.
 * Attention: The LSA components mustn't cast the parameters at the call.
 *
 * macro names:   LSA_TRACE_XX           (XX = 00, 01, 02, 03, ..., 16)
 *                LSA_INST_TRACE_XX      (XX = 00, 01, 02, 03, ..., 16)
 *
 * macros:        Trace entry of 0, 1, 2, ... or 16 parameters
 *
 * macro names:   LSA_TRACE_BYTE_ARRAY
 *                LSA_INST_TRACE_BYTE_ARRAY
 *
 * macros:        Trace entry of a byte array
 *
 * parameters:    subsys_:  subsystem of a component: TRACE_SUBSYS_PREFIX_*
 *                inst_:    instance
 *                level_:   LSA_TRACE_LEVEL_OFF, LSA_TRACE_LEVEL_FATAL,
 *                          LSA_TRACE_LEVEL_ERROR, LSA_TRACE_LEVEL_UNEXP,
 *                          LSA_TRACE_LEVEL_WARN, LSA_TRACE_LEVEL_NOTE_HIGH,
 *                          LSA_TRACE_LEVEL_NOTE, LSA_TRACE_LEVEL_NOTE_LOW
 *                          or LSA_TRACE_LEVEL_CHAT
 *                msg_:     string similar to "printf"
 *                para1_:   parameter 1
 *                para2_:   parameter 2
 *                 ...        ...
 *                para16_:  parameter 16
 *                ptr_:     pointer to byte array
 *                len_:     number of bytes of the array
 *
 * return value:  LSA_VOID
 *===========================================================================*/

#ifdef IM_HW_FLASH_32BIT
 #define LSA_TRACE_COMPILE_LEVEL LSA_TRACE_LEVEL_NOTE
#else
   #define LSA_TRACE_COMPILE_LEVEL     PNIO_TRACE_COMPILE_LEVEL
#endif

                                                                                               
/*****************************************************************************/
/*  The following part doesn't belong to LSA. But the following pseudo trace */
/*  macros must be once defined in a system, because otherwise LSA software  */
/*  can't be compiled. The component LTRC (or the LTRC successor component   */
/*  PTRC = platform trace) will contain these declarations in future.        */
/*****************************************************************************/

/*=============================================================================
 * First, two macros that can be used to define regions of code that
 * are invisible for the trace scanner.  Their foremost application is
 * to hide the definitions of all pseudo macros defined in this file.
 *===========================================================================*/
#define PTRC_IGNORE_OPEN()
#define PTRC_IGNORE_CLOSE()

/*===========================================================================*/
/* Definition of pseudo macros for trace definitions                                                                                */
/*===========================================================================*/
PTRC_IGNORE_OPEN()

/* Defines a descriptive string for a symbolic constant */
#define PTRC_EXPLAIN(p)

/* Two macros to parenthesize lists of symbolic constants */
#define PTRC_SUBLABEL_LIST_OPEN(p)
#define PTRC_SUBLABEL_LIST_CLOSE(p)

/* Connect symbolic constants with its user data */
#define PTRC_META_01(s1)
#define PTRC_META_02(s1,s2)
#define PTRC_META_03(s1,s2,s3)
#define PTRC_META_04(s1,s2,s3,s4)
#define PTRC_META_05(s1,s2,s3,s4,s5)
#define PTRC_META_06(s1,s2,s3,s4,s5,s6)
#define PTRC_META_07(s1,s2,s3,s4,s5,s6,s7)
#define PTRC_META_08(s1,s2,s3,s4,s5,s6,s7,s8)
#define PTRC_META_09(s1,s2,s3,s4,s5,s6,s7,s8,s9)
#define PTRC_META_10(s1,s2,s3,s4,s5,s6,s7,s8,s9,s10)
#define PTRC_META_11(s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11)
#define PTRC_META_12(s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12)
#define PTRC_META_13(s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13)
#define PTRC_META_14(s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13,s14)
#define PTRC_META_15(s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13,s14,s15)
#define PTRC_META_16(s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13,s14,s15,s16)

PTRC_IGNORE_CLOSE()
/*****************************************************************************/
/*  end of pseudo trace makro definitions.                                   */
/*****************************************************************************/



/*****************************************************************************/
/*  end of file LSA_CFG.H                                                    */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of LSA_CFG_H */


