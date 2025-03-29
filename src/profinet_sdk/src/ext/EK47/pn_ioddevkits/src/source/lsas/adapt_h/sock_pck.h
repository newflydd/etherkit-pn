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
/*  F i l e               &F: sock_pck.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Internal headerfile                                                      */
/*  Defines internal types and data (and constants) for sock,                */
/*                                                                           */
/*****************************************************************************/

#ifndef SOCK_PCK_H                     /* ----- reinclude-protection ----- */
#define SOCK_PCK_H

/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Version        	Who  	What                                 */
/*  09.07.02    MG4387          first implementation						 */
/*                                                                           */
/*****************************************************************************/

#include "pnio_pck1_on.h"


/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/



/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/
/* ************************************************************************* */
/* diese PDUen muessen dicht im Speicher liegen, es darf keine Füllbytes geben! */
/* ************************************************************************* */

typedef struct rfc_crcc_tpdu_tag {
    LSA_UINT8 tpkt_version;   /* 0x03: RFC1006_VERS */
    LSA_UINT8 tpkt_reserved;  /* 0x00 */
    LSA_UINT16 tpkt_length;   /* Laenge des gesamten Pakets */
    LSA_UINT8 tpkt_rfc_li;    /* Laenge des Headers von Code bis Parameter */
    LSA_UINT8 tpkt_opcode;    /* 0xE0 bzw. 0xD0: Opcode fuer CR bzw. CC */
    LSA_UINT16 tpkt_dst;      /* CN_ID des Empfaengers der PDU (CR:0x00) */
    LSA_UINT16 tpkt_src;      /* CN_ID des Senders der PDU */
    LSA_UINT8 tpkt_class;     /* 0x00 fuer TP0: Transport Klasse 0 */
    LSA_UINT8 tpkt_para[1];   /* Parameter Puffer */
} RFC1006_CRCC_PDU;

typedef struct rfc_data_tpdu_tag {
    LSA_UINT8 tpkt_version;   /* 0x03 */
    LSA_UINT8 tpkt_reserved;  /* 0x00 */
    LSA_UINT16 tpkt_length;   /* Laenge des gesamten Pakets */
    LSA_UINT8 tpkt_rfc_li;    /* 0x02: Laenge des Headers von Code bis EOT */
    LSA_UINT8 tpkt_opcode;    /* 0xF0: Opcode fuer DT */
    LSA_UINT8 tpkt_seqnr_eot; /* EOT Markierung; Sequence-Nr ist immer 0. */
    LSA_UINT8 tpkt_data[1];   /* Daten Puffer */
} RFC1006_DATA_PDU;



/*===========================================================================*/
/*                                  data                                     */
/*===========================================================================*/

#if (SOCK_MODULE_ID == 1)
#endif

#include "pnio_pck_off.h"
/*****************************************************************************/
/*  end of file SOCK_PCK.H                                                 */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
#endif  /* of SOCK_PCK_H */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
