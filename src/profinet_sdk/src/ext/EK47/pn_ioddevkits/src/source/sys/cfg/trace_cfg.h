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
/*  F i l e               &F: trace_cfg.h                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  configure trace.                                                         */
/*                                                                           */
/*****************************************************************************/



/*****************************************************************************/
/* contents:

    SYSTEM

      system interface macros

    TRACE

      code attributes
      data attributes

*/
/*****************************************************************************/
/* reinclude protection */


#ifndef TRACE_CFG_H
#define TRACE_CFG_H


/* basic tool chain characteristics */
#define TRACE_DEFAULT_BSS_SECTION_NAME ".bss_sys_trace"


/*****************************************************************************/
/*****************************************************************************/
/*******************************                ******************************/
/*******************************     SYSTEM     ******************************/
/*******************************                ******************************/
/*****************************************************************************/
/*****************************************************************************/

#define TRACE_BUFFER_SIZE       65000	/* LSA-tracebuffer (*Unsigned32) */

/*****************************************************************************/
/* basic data type publication */

#define TRACE_UINT8                             LSA_UINT8
#define TRACE_UINT8_PTR                         LSA_UINT8 *
#define TRACE_UINT16                            LSA_UINT16
#define TRACE_UINT32                            LSA_UINT32
#define TRACE_CHAR                              char

/*****************************************************************************/
/* system interface macros */

/* DISable interrupts */
#define TRACE_ENTER()           OsIntDisable()	/* hama_lock(HAMA_SEMA_TRACE); */

/* ENable interrupts */
#define TRACE_EXIT()            OsIntEnable()	/* hama_unlock(HAMA_SEMA_TRACE); */

/* copy _len bytes from _src to _dst address */
#define TRACE_MEMCPY(_dst, _src, _len)                                          \
{                                                                               \
    LSA_MEMCPY((_dst), (_src), (_len));                                         \
}

/* overwrite _dst with _len _val */
#define TRACE_MEMSET(_dst, _val, _len)       \
{                                            \
    LSA_MEMSET(_dst, _val, _len);            \
}

#define TRACE_CKSUM_EXTERN extern unsigned short ccksum (void *ptr, unsigned words);
/* function to calculate checksum for tracebuffer header */
#define TRACE_CKSUM(_buff, _len)    ~ccksum((_buff), (_len)>>1)

#define TRACE_GOTO_FATAL_ERROR(_ERROR_DETAIL_PTR)                                                           \
{                                                                                                           \
    /*apma_spec_build_fatal_error(APMA_PDB_FW_MODULE_ID_TRACE, TRACE_MODULE_ID, (LSA_UINT16)__LINE__, 0);*/ \
}

/* let the user handle non-defined opcodes */
#define TRACE_OPCODE_HANDLER_EXTERN(opcode, param)

#define TRACE_WRITE_CFG(CFG_PTR, LEN)       	\
{                                           	\
    /* apma_trace_write_cfg(CFG_PTR, LEN); */	\
}

#define TRACE_READ_CFG(CFG_PTR, LEN)        	\
{                                           	\
    /* apma_trace_read_cfg(CFG_PTR, LEN); */   	\
	(CFG_PTR)->software_revision[0] = 0;		\
	(CFG_PTR)->software_revision[1] = 0;		\
	(CFG_PTR)->software_revision[2] = 0;		\
	(CFG_PTR)->software_revision[3] = 0;		\
}





/*****************************************************************************/
/*****************************************************************************/
/* autoread-table - uebernahme aus "debug" */
#define TRACE_FILENAME_MAXLEN       24

typedef struct
{
  TRACE_UINT8_PTR                   mem_ptr;
  TRACE_UINT32                      mem_len;
  TRACE_CHAR                        fname[TRACE_FILENAME_MAXLEN];

} TRACE_AUTOREAD_ENTRY;

typedef TRACE_AUTOREAD_ENTRY        * TRACE_AUTOREAD_ENTRY_PTR;

/* get autoread-table */
/*
#define TRACE_GET_AUTOREAD_TABLE(_PTR)                                        \
{                                                                             \
    _PTR = (TRACE_AUTOREAD_ENTRY_PTR) apma_get_debug_autoread_table();        \
}
*/

/*****************************************************************************/
/*****************************************************************************/



/*****************************************************************************/
/*****************************************************************************/
/*********************************             *******************************/
/*********************************     TRACE   *******************************/
/*********************************             *******************************/
/*****************************************************************************/
/*****************************************************************************/

/* 2 timers for trace header */
/**  1st timer */
#define    TRACE_TIMER_HIGH                     0

/**  2nd timer */
#define    TRACE_TIMER_LOW 						OsGetTime_ms()	/* miliseconds */

/**********/
/* STATUS */
/**********/
/* uptime (32bit) */
#define TRACE_UPTIME                            OsGetTime_ms()		/*APMA_GET_MS_COUNT()*/
/* FW-version: (4 * 8bit) */
#define TRACE_FW_VERSION_0                      DEVKIT_VERSION_HH
#define TRACE_FW_VERSION_1                      DEVKIT_VERSION_H
#define TRACE_FW_VERSION_2                      DEVKIT_VERSION_L
#define TRACE_FW_VERSION_3                      DEVKIT_VERSION_LL



/**************/
/* TRACELEVEL */
/**************/
/* for TRACE_DEFAULTCONFIG_TRACELEVEL
use one of the following: (see _cfg\lsa_cfg.h)
LSA_TRACE_LEVEL_OFF         1
LSA_TRACE_LEVEL_FATAL       2
LSA_TRACE_LEVEL_ERROR       3
LSA_TRACE_LEVEL_UNEXP       4
LSA_TRACE_LEVEL_WARN        5
LSA_TRACE_LEVEL_NOTE_HIGH   6
LSA_TRACE_LEVEL_NOTE        7
LSA_TRACE_LEVEL_NOTE_LOW    8
LSA_TRACE_LEVEL_CHAT        9
*/
/* no trace during trace_init/trace_open */
#define TRACE_NO_TRACE                          LSA_TRACE_LEVEL_OFF


/* default config */
#define TRACE_DEFAULTCONFIG_TRACELEVEL          LSA_TRACE_LEVEL_NOTE_HIGH
#define TRACE_DEFAULTCONFIG_ERRORMODE           TRACE_ERRORMODE_FATAL



/*****************************************************************************/
/*****************************************************************************/
/*********************************             *******************************/
/*********************************   TRACE 2   *******************************/
/*********************************             *******************************/
/*****************************************************************************/
/*****************************************************************************/

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*******************                                      ********************/
/*******************            trace settings            ********************/
/*******************                                      ********************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/


#ifndef TRC_ONLY_TRACE_SCANNER

/* empty pseudomakros */

#define TRC_DECLARE_OPEN(p)
#define TRC_DECLARE_CLOSE(p)
#define TRC_EVENT_LIST_LINK(p)
#define TRC_SUBLABEL_LIST_LINK(p)
#define TRC_EVENT_LIST_OPEN(p)
#define TRC_EVENT_LIST_CLOSE(p)
#define TRC_EXPLAIN(p)
#define TRC_ID_LIST_OPEN(p)
#define TRC_ID_LIST_CLOSE(p)
#define TRC_PROCESS_LIST_OPEN(p)
#define TRC_PROCESS_LIST_CLOSE(p)
#define TRC_PROCESSOR_LIST_OPEN(p)
#define TRC_PROCESSOR_LIST_CLOSE(p)
#define TRC_STRUCT_LINK(p)
#define TRC_SUBLABEL_LIST_OPEN(p)
#define TRC_SUBLABEL_LIST_CLOSE(p)
#define TRC_STRUCT_OPEN(p)
#define TRC_STRUCT_CLOSE(p)

#endif /*#ifndef TRC_ONLY_TRACE_SCANNER*/

/*****************************************************************************/

/* Konstantendefinitionen */

/* Byteordering in den Trace-Puffern */
#define TRC_BYTE_ORDERING 0x01
/* big endian: 0x00 */
/* little endian: 0x01 */

/*****************************************************************************/
/* Version des TPH */

#define TRC_TPH_INK2 		0x1
#define TRC_TPH_VERSION 	TRC_TPH_INK2

/*****************************************************************************/
/* Version des TEH */

#define TRC_TEH_INK2        0x1		/* action is logged with 32bit timestamp_low, on overflow timestamp_high is logged with action 0xFF */
#define TRC_TEH_S7400_64BIT 0x2		/* TRC_TIMESTAMP_RESOLUTION is not considered by trcbin2csv.exe in this mode. */
#define TRC_TEH_LSASUBSYS   0x3		/* LSA-Trace (64bit - 2 Zeitstempel)              */
#define TRC_TEH_ET200SPMOD  0x4		/* ET200 SP Module 								  */
#define TRC_TEH_LSASUBSYS2  0x5		/* LSA trace offering S7-400-trace-like timestamp features */

// only used with TRC_TP_INIT, e.g. PNDV , but not used for LSA components
#define TRC_TEH_VERSION     TRC_TEH_INK2


/*****************************************************************************/

/* Version der Trace-Syntax */

/* In diesem Inkrement wird ausschlie�lich die */
/* Trace-Syntax f�r den Trace-Scanner Stufe 2 der S7-400 */
/* (TRC_SYNTAX_STUFE_2) verwendet. */

#define TRC_SYNTAX_VERSION TRC_SYNTAX_STUFE_2
#define TRC_SYNTAX_STUFE_2 0x5

/*****************************************************************************/

/* Alignment in der Baugruppe */
#define TRC_ALIGNMENT 32		/* 16 ToWe */

/*****************************************************************************/

/* Aufl�sung des Zeitstempels in ns */
#define TRC_TIMESTAMP_RESOLUTION 	1000000                 /* TRACE_TIMER_LOW is in 1ms */

/*****************************************************************************/

/* Maximaler Wert des Zeitstempels */
/* >0 bei downcounting, ==0 bei upcounting */
#define TRC_MAX_TIMESTAMP 0
//#define TRC_MAX_TIMESTAMP 0xFFFFFFFF

/*****************************************************************************/

/* Definition Trace-IDs (Liste von Trace-IDs) */


/* Mit den Pseudomakros TRC_ID_LIST_OPEN("") und */
/* TRC_ID_LIST_CLOSE("") werden die Trace-ID-Definitionen */
/* geklammert. Diese Klammerung ist notwendig! */

TRC_ID_LIST_OPEN("") /*umklammerndes Pseudomakro */
                          /*    Unsigned8   trace_id; */
#define TRC_ID_PNDV                0x0014 /* APMA_PDB_FW_MODULE_ID_PNDV */
#define TRC_ID_S7PM                0x0017 /* APMA_PDB_FW_MODULE_ID_S7PM */
#define TRC_ID_TRACE               0x0064 /* Trace-ID TRC               */
#define TRC_ID_UDMA                0x001D /* APMA_PDB_FW_MODULE_ID_UDMA */
TRC_ID_LIST_CLOSE("") /*umklammerndes Pseudomakro */
//#define TRC_ID_PNPB                0x0015 /* APMA_PDB_FW_MODULE_ID_PNPB => stored into LSA trace*/

/* Fuer <MODUL_x> steht der Modulpraefix (in Gro�buchstaben) */

/*****************************************************************************/


#define TRC_PROCESSOR_ID 0x0
TRC_PROCESSOR_LIST_OPEN ("")
  #define TRC_PROCESSOR_DEFAULT TRC_PROCESSOR_ID
TRC_PROCESSOR_LIST_CLOSE ("")


/*****************************************************************************/


#define TRC_PROCESS_ID 0x0
TRC_PROCESS_LIST_OPEN ("TRC_PROCESSOR_DEFAULT")
  #define TRC_PROCESS_DEFAULT TRC_PROCESS_ID
TRC_PROCESS_LIST_CLOSE ("TRC_PROCESSOR_DEFAULT")


/*****************************************************************************/


#define TRC_STRUC(_TRC_TE_STRUC,_TRC_ELEMENT_NUMBER)                          \
                                                                              \
    TRC_TPH_TYPE tph;                                                         \
                                                                              \
    union                                                                     \
    {                                                                         \
        TRC_TE_COUNTER_TYPE te_counter;                                       \
                                                                              \
        struct                                                                \
        {                                                                     \
            TRC_TEH_TYPE    teh;                                              \
            _TRC_TE_STRUC element;                                            \
                                                                              \
        }   te;                                                               \
                                                                              \
    }   buffer[ _TRC_ELEMENT_NUMBER];


/*****************************************************************************/
/* S7-400 Trace 															 */
/*****************************************************************************/

#define TRC_TP_INIT(_TRC,_TRC_ELEMENT_NUMBER,_TRC_ID)                         \
{                                                                             \
    _TRC.tph.tph_size             = sizeof( _TRC.tph);                        \
    _TRC.tph.tph_version          = TRC_TPH_VERSION;                          \
    _TRC.tph.byte_ordering        = TRC_BYTE_ORDERING;                        \
    _TRC.tph.trace_id             = _TRC_ID;                                  \
                                                                              \
    _TRC.tph.entry_size           = sizeof( _TRC.buffer[0]);                  \
    _TRC.tph.teh_size             = sizeof( TRC_TEH_TYPE);                    \
    _TRC.tph.teh_version          = TRC_TEH_VERSION;                          \
    _TRC.tph.timestamp_size       = sizeof( TRC_TEH_TYPE);                    \
                                                                              \
    _TRC.tph.trc_syntax_version   = TRC_SYNTAX_VERSION;                       \
    _TRC.tph.processor_id         = TRC_PROCESSOR_ID;                         \
                                                                              \
    _TRC.tph.process_id           = TRC_PROCESS_ID;                           \
                                                                              \
    _TRC.tph.alignment            = TRC_ALIGNMENT;                            \
                                                                              \
    _TRC.tph.debug_element_number = _TRC_ELEMENT_NUMBER;                      \
    _TRC.tph.buffer_index         = 0;                                        \
                                                                              \
    _TRC.tph.timestamp_resolution = TRC_TIMESTAMP_RESOLUTION;          		  \
    _TRC.tph.max_timestamp        = TRC_MAX_TIMESTAMP;                 		  \
    _TRC.tph.last_counter         = 0;                                        \
                                                                              \
    LSA_MEMSET((void*)&_TRC.buffer[0].te.teh.timestamp,0,sizeof(_TRC.buffer));\
}

/*****************************************************************************/
/* write teh and special trace entry for time overflow if necessary */

/*
#define TRC_TEH_FILL(_TRC)                                                    \
{                                                                             \
    TRC_TPH_UPDATE( _TRC.tph)                                                 \
                                                                              \
    _TRC.buffer[ _TRC.tph.buffer_index].te.teh.clock_high = TRACE_TIMER_HIGH; \
    _TRC.buffer[ _TRC.tph.buffer_index].te.teh.clock_low  = TRACE_TIMER_LOW;  \
}
*/
#define TRC_TEH_FILL(_TRC)                                                    \
{                                                                             \
    LSA_UINT32 clk_high = TRACE_TIMER_HIGH;                                   \
    Unsigned32 clk_low  = TRACE_TIMER_LOW;                                    \
                                                                              \
/*  TRCSYS_READ_TRC_TIMER( &clk_high, &clk_low);                          */  \
                                                                              \
    if ( _TRC.tph.last_counter != clk_high )                                  \
    {                                                                         \
        TRC_TPH_UPDATE( _TRC.tph)                                             \
        _TRC.buffer[ _TRC.tph.buffer_index].te_counter.event_id       = 0xFF; \
        _TRC.buffer[ _TRC.tph.buffer_index].te_counter.global_counter = _TRC.tph.last_counter;\
        _TRC.tph.last_counter                                         = clk_high;\
    }                                                                         \
                                                                              \
    TRC_TPH_UPDATE( _TRC.tph)                                                 \
                                                                              \
    _TRC.buffer[ _TRC.tph.buffer_index].te.teh.timestamp = clk_low;           \
}


/*****************************************************************************/


#define TRC_TPH_UPDATE(_TPH)    (_TPH.debug_element_number - 1) <= _TPH.buffer_index ? _TPH.buffer_index = 0 : _TPH.buffer_index++;


/*****************************************************************************/


#define TRC_TE_ACCESS(_TRC)     _TRC.buffer[ _TRC.tph.buffer_index].te.element


/*****************************************************************************/
/* Datenstruktur der Trace-Puffer-Header (TPH) */

typedef struct
{
   Unsigned8   tph_size;
   Unsigned8   tph_version;
   Unsigned8   byte_ordering;
   Unsigned8   trace_id;

   Unsigned8   entry_size;
   Unsigned8   teh_size;
   Unsigned8   teh_version;
   Unsigned8   timestamp_size;

   Unsigned8   trc_syntax_version;
   Unsigned8   processor_id;
   Unsigned8   process_id;
   Unsigned8   alignment;

   Unsigned16  debug_element_number;
   Unsigned16  buffer_index;

   Unsigned32  timestamp_resolution;

   Unsigned32  max_timestamp;

   Unsigned32  last_counter;
} TRC_TPH_TYPE;

/*****************************************************************************/

/* Datentyp des Zeitstempels */

typedef Unsigned32 TRC_TIMESTAMP_TYPE;

/*****************************************************************************/

/* Datenstruktur der Trace-Eintrags-Header (TEH) */

typedef struct
{
    TRC_TIMESTAMP_TYPE timestamp;

}   TRC_TEH_TYPE;


/*****************************************************************************/

/* Datenstruktur eines eines speziellen Trace-Eintrags mit 32-Bit-Zeitstempel�berlauf */

typedef struct
{
   TRC_TEH_TYPE teh;
   Unsigned8 event_id;
   Unsigned8 dummy;
   Unsigned32 global_counter;
} TRC_TE_COUNTER_TYPE;

/*****************************************************************************/
/*****************************************************************************/
/*******************************                 *****************************/
/*******************************       END       *****************************/
/*******************************                 *****************************/
/*****************************************************************************/
/*****************************************************************************/



/*****************************************************************************/
/* reinclude-protection */


#else
    #pragma message ("The header TRACE_CFG.H is included twice or more !")
#endif


/*** end of file *************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
