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
/*  C o m p o n e n t     &C: EDDP (EthernetDeviceDriver for PN-IP)     :C&  */
/*                                                                           */
/*  F i l e               &F: eddp_swi_fdb.c                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP SWITCH functions for FDB-Control            */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  14.01.09    AB    initial version                                        */
/*  16.09.09    lrg   lrg0809: split MAC addresses of announce frames to be  */
/*                             able to change forwarding for SyncId 0 and 1  */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID   83
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_SWI_FDB */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"            /* eddp headerfiles */
#include "eddp_int.h"            /* internal header  */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)

/*===========================================================================*/
/*                                global Variables                           */
/*===========================================================================*/

/*===========================================================================*/
/*                                  Types                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                Constants                                  */
/*===========================================================================*/


/* ------------------------------------------------------------------------ */
/*  MultiCast-Table                                                         */
/* ------------------------------------------------------------------------ */
/*  V           : Valid                                                     */
/*  FH          : FrameHandling                                             */
/*  R           : Redundancy                                                */
/*  F           : Fragmentation                                             */
/*  BD          : BridgeDelay_Enable (only used in PN-IP Rev1)              */
/*  UE          : UseType-Extension (only used in PN-IP Rev3) replaces BD   */
/*  PM          : PriorityMapping                                           */
/*  TPAPI       : TxPort API                                                */
/*  LDAPI       : Learning Disable API                                      */
/*  MEAPI       : ManagementEN API                                          */
/*  MPAPI       : ManagementPrio API                                        */
/*  TP1..4      : TxPort 1..4                                               */
/*  LD1..4      : Learning Disable Ports                                    */
/*  ME1..4      : ManagementEN Port                                         */
/*  UseType     : use type                                                  */
/*  DTC         : Default Traffic Class                                     */
/*  API-Type    : API-Type                                                  */
/*  Resource-Type:                                                          */
/*  FFW         : Fast Forwarding                                           */
/*  Static      :                                                           */
/*  RefCnt      :                                                           */
/*  ForwardingRule_API      : for handling at start up                      */
/*  ForwardingRule_Ports    : for handling at start up                      */
/*  SpecialHandling         : for handling at start up                      */
/* ------------------------------------------------------------------------ */
/*  last update : 2013-04-19                                                */
/* ------------------------------------------------------------------------ */
#if ( (defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT)) || (defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)) )
const   EDDP_SWI_MULTICAST_ENTRY_TYPE   EDDP_SWI_multicast_table_ERTEC200P [] =
{
//     MAC,       LoMAC, HiMAC, V,(FrameHandling),                  R,F,BD,PM,TPAPI,LDAPI,MEAPI,MPAPI,TP1,LD1,ME1,MP1,TP2,LD2,ME2,MP2,TP3,LD3,ME3,MP3,TP4,LD4,ME4,MP4,(UseType),                        DTC, (API-Type),                    (Resource-Type),                   FFW,(Static),  RefCnt,(ForwardingRule_API),             (ForwardingRule_Ports),             (SpecialHandling)
/*01*/{0x01005E40,0xF800,0xFBFF,1,(EDDP_MCFCT_FRAME_HANDLING_NRT),  0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_OTHER),   (EDDP_MCFCT_RESOURCE_TYPE_RT_H),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_NO_SPECIAL_HANDLING)   },
/*02*/{0x010ECF00,0x0000,0x0000,1,(EDDP_MCFCT_FRAME_HANDLING_NRT),  0,1,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_DCP),     (EDDP_MCFCT_RESOURCE_TYPE_OTHER),  0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_PORTS),       (EDDP_SWI_FDB_NO_SPECIAL_HANDLING)   },
/*03*/{0x010ECF00,0x0001,0x0001,1,(EDDP_MCFCT_FRAME_HANDLING_NRT),  0,1,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_DCP),     (EDDP_MCFCT_RESOURCE_TYPE_OTHER),  0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_PORTS),       (EDDP_SWI_FDB_NO_SPECIAL_HANDLING)   },
/*04*/{0x010ECF00,0x0101,0x0101,1,(EDDP_MCFCT_FRAME_HANDLING_RT_H), 0,0,0, 0, 0x0,  1,    0,    0,    0,  1,  0,  0,  0,  1,  0,  0,  0,  1,  0,  0,  0,  1,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_CT),   (EDDP_MCFCT_RESOURCE_TYPE_RT_H),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_FILTER),    (EDDP_SWI_FDB_FORWARD_FILTER),      (EDDP_SWI_FDB_NO_SPECIAL_HANDLING)   },
/*05*/{0x010ECF00,0x0102,0x0102,1,(EDDP_MCFCT_FRAME_HANDLING_NRT),  0,0,0, 0, 0x0,  1,    0,    0,    0,  1,  0,  0,  0,  1,  0,  0,  0,  1,  0,  0,  0,  1,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_CT),   (EDDP_MCFCT_RESOURCE_TYPE_RT_H),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_FILTER),    (EDDP_SWI_FDB_FORWARD_FILTER),      (EDDP_SWI_FDB_NO_SPECIAL_HANDLING)   },
//     MAC,       LoMAC, HiMAC, V,(FrameHandling),                  R,F,BD,PM,TPAPI,LDAPI,MEAPI,MPAPI,TP1,LD1,ME1,MP1,TP2,LD2,ME2,MP2,TP3,LD3,ME3,MP3,TP4,LD4,ME4,MP4,(UseType),                        DTC, (API-Type),                    (Resource-Type),                   FFW,(Static),  RefCnt,(ForwardingRule_API),             (ForwardingRule_Ports),             (SpecialHandling)
    /* 0x010ECF00 0x0200-0x0200 */
/*06*/{0x010ECF00,0x0200,0x0200,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*07*/{0x010ECF00,0x0201,0x0201,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*08*/{0x010ECF00,0x0202,0x0202,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*09*/{0x010ECF00,0x0203,0x0203,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*10*/{0x010ECF00,0x0204,0x0204,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*11*/{0x010ECF00,0x0205,0x0205,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*12*/{0x010ECF00,0x0206,0x0206,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*13*/{0x010ECF00,0x0207,0x0207,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*14*/{0x010ECF00,0x0208,0x0208,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*15*/{0x010ECF00,0x0209,0x0209,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*16*/{0x010ECF00,0x020A,0x020A,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*17*/{0x010ECF00,0x020B,0x020B,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*18*/{0x010ECF00,0x020C,0x020C,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*19*/{0x010ECF00,0x020D,0x020D,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*20*/{0x010ECF00,0x020E,0x020E,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*21*/{0x010ECF00,0x020F,0x020F,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*22*/{0x010ECF00,0x0210,0x0210,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*23*/{0x010ECF00,0x0211,0x0211,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*24*/{0x010ECF00,0x0212,0x0212,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*25*/{0x010ECF00,0x0213,0x0213,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*26*/{0x010ECF00,0x0214,0x0214,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*27*/{0x010ECF00,0x0215,0x0215,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*28*/{0x010ECF00,0x0216,0x0216,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*29*/{0x010ECF00,0x0217,0x0217,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*30*/{0x010ECF00,0x0218,0x0218,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*31*/{0x010ECF00,0x0219,0x0219,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*32*/{0x010ECF00,0x021A,0x021A,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*33*/{0x010ECF00,0x021B,0x021B,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*34*/{0x010ECF00,0x021C,0x021C,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*35*/{0x010ECF00,0x021D,0x021D,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*36*/{0x010ECF00,0x021E,0x021E,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
/*37*/{0x010ECF00,0x021F,0x021F,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_GROUP_RT_2_QUER)       },
    /* 0x010ECF00 0x021F-0x021F */
//     MAC,       LoMAC, HiMAC, V,(FrameHandling),                  R,F,BD,PM,TPAPI,LDAPI,MEAPI,MPAPI,TP1,LD1,ME1,MP1,TP2,LD2,ME2,MP2,TP3,LD3,ME3,MP3,TP4,LD4,ME4,MP4,(UseType),                        DTC, (API-Type),                    (Resource-Type),                   FFW,(Static),  RefCnt,(ForwardingRule_API),             (ForwardingRule_Ports),             (SpecialHandling)
/*38*/{0x010ECF00,0x0220,0x02FF,1,(EDDP_MCFCT_FRAME_HANDLING_RT_L), 0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_RT_SF),   (EDDP_MCFCT_RESOURCE_TYPE_RT_L),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_NO_SPECIAL_HANDLING)   },
/*39*/{0x010ECF00,0x0400,0x0400,1,(EDDP_MCFCT_FRAME_HANDLING_NRT),  0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_PTCP),    (EDDP_MCFCT_RESOURCE_TYPE_PTCP),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_FILTER),      (EDDP_SWI_FDB_NO_SPECIAL_HANDLING)   },
/*40*/{0x010ECF00,0x0401,0x0401,1,(EDDP_MCFCT_FRAME_HANDLING_NRT),  0,0,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_PTCP),    (EDDP_MCFCT_RESOURCE_TYPE_PTCP),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_FILTER),      (EDDP_SWI_FDB_NO_SPECIAL_HANDLING)   },
/*41*/{0x010ECF00,0x0420,0x0420,1,(EDDP_MCFCT_FRAME_HANDLING_NRT),  0,0,0, 0, 0x0,  1,    1,    1,    0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  (EDDP_MCFCT_USE_TYPE_PTCP_CLOCK), 0,   (EDDP_MCFCT_API_TYPE_PTCP),    (EDDP_MCFCT_RESOURCE_TYPE_PTCP),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_NO_SPECIAL_HANDLING)   },
/*42*/{0x010ECF00,0x0421,0x0421,1,(EDDP_MCFCT_FRAME_HANDLING_NRT),  0,0,0, 0, 0x0,  1,    1,    1,    0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  (EDDP_MCFCT_USE_TYPE_PTCP_TIME),  0,   (EDDP_MCFCT_API_TYPE_PTCP),    (EDDP_MCFCT_RESOURCE_TYPE_PTCP),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_NO_SPECIAL_HANDLING)   },
/*43*/{0x010ECF00,0x0440,0x0440,1,(EDDP_MCFCT_FRAME_HANDLING_NRT),  0,0,0, 0, 0x0,  1,    1,    1,    0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  (EDDP_MCFCT_USE_TYPE_FUP_CLOCK),  0,   (EDDP_MCFCT_API_TYPE_PTCP),    (EDDP_MCFCT_RESOURCE_TYPE_PTCP),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_NO_SPECIAL_HANDLING)   },
/*44*/{0x010ECF00,0x0441,0x0441,1,(EDDP_MCFCT_FRAME_HANDLING_NRT),  0,0,0, 0, 0x0,  1,    1,    1,    0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  (EDDP_MCFCT_USE_TYPE_FUP_TIME),   0,   (EDDP_MCFCT_API_TYPE_PTCP),    (EDDP_MCFCT_RESOURCE_TYPE_PTCP),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_NO_SPECIAL_HANDLING)   },
/*45*/{0x010ECF00,0x0480,0x0480,1,(EDDP_MCFCT_FRAME_HANDLING_NRT),  0,0,0, 0, 0x0,  1,    1,    1,    0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  (EDDP_MCFCT_USE_TYPE_PTCP_CLOCK), 0,   (EDDP_MCFCT_API_TYPE_PTCP),    (EDDP_MCFCT_RESOURCE_TYPE_PTCP),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_NO_SPECIAL_HANDLING)   },
/*46*/{0x010ECF00,0x0481,0x0481,1,(EDDP_MCFCT_FRAME_HANDLING_NRT),  0,0,0, 0, 0x0,  1,    1,    1,    0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  (EDDP_MCFCT_USE_TYPE_PTCP_TIME),  0,   (EDDP_MCFCT_API_TYPE_PTCP),    (EDDP_MCFCT_RESOURCE_TYPE_PTCP),   0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_NO_SPECIAL_HANDLING)   },

/* HSYNC */
/*47*/{0x010ECF00,0x0500,0x0500,1,(EDDP_MCFCT_FRAME_HANDLING_NRT),  0,0,0, 0, 0x0,  1,    1,    1,    0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_OTHER),   (EDDP_MCFCT_RESOURCE_TYPE_OTHER),  0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_FILTER),       (EDDP_SWI_FDB_GROUP_HSYNC)          },

//     MAC,       LoMAC, HiMAC, V,(FrameHandling),                  R,F,BD,PM,TPAPI,LDAPI,MEAPI,MPAPI,TP1,LD1,ME1,MP1,TP2,LD2,ME2,MP2,TP3,LD3,ME3,MP3,TP4,LD4,ME4,MP4,(UseType),                        DTC, (API-Type),                    (Resource-Type),                   FFW,(Static),  RefCnt,(ForwardingRule_API),             (ForwardingRule_Ports),             (SpecialHandling)
/*48*/{0x01154E00,0x0001,0x0001,1,(EDDP_MCFCT_FRAME_HANDLING_NRT),  0,0,0, 0, 0x0,  1,    1,    1,    0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_MRP),     (EDDP_MCFCT_RESOURCE_TYPE_MRP),    0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_PORTS),       (EDDP_SWI_FDB_GROUP_MRP_1)           },
/*49*/{0x01154E00,0x0002,0x0002,1,(EDDP_MCFCT_FRAME_HANDLING_NRT),  0,0,0, 0, 0x0,  1,    1,    1,    0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_MRP),     (EDDP_MCFCT_RESOURCE_TYPE_MRP),    0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_PORTS),       (EDDP_SWI_FDB_GROUP_MRP_2)           },
/*50*/{0x01154E00,0x0003,0x0003,1,(EDDP_MCFCT_FRAME_HANDLING_NRT),  0,0,0, 0, 0x0,  1,    1,    1,    0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_MRP),     (EDDP_MCFCT_RESOURCE_TYPE_MRP),    0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_PORTS),       (EDDP_SWI_FDB_GROUP_MRP_IC_1)        },
/*51*/{0x01154E00,0x0004,0x0004,1,(EDDP_MCFCT_FRAME_HANDLING_NRT),  0,0,0, 0, 0x0,  1,    1,    1,    0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_MRP),     (EDDP_MCFCT_RESOURCE_TYPE_MRP),    0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_PORTS),       (EDDP_SWI_FDB_GROUP_MRP_IC_2)        },
/*52*/{0x01154E00,0x0005,0x0005,1,(EDDP_MCFCT_FRAME_HANDLING_NRT),  0,0,0, 0, 0x0,  1,    1,    1,    0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_MRP),     (EDDP_MCFCT_RESOURCE_TYPE_MRP),    0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_PORTS),       (EDDP_SWI_FDB_GROUP_MRP_IC_3)        },
//     MAC,       LoMAC, HiMAC, V,(FrameHandling),                  R,F,BD,PM,TPAPI,LDAPI,MEAPI,MPAPI,TP1,LD1,ME1,MP1,TP2,LD2,ME2,MP2,TP3,LD3,ME3,MP3,TP4,LD4,ME4,MP4,(UseType),                        DTC, (API-Type),                    (Resource-Type),                   FFW,(Static),  RefCnt,(ForwardingRule_API),             (ForwardingRule_Ports),             (SpecialHandling)
/*53*/{0x01216C00,0x0001,0x0001,1,(EDDP_MCFCT_FRAME_HANDLING_NRT),  0,0,0, 0, 0x0,  1,    1,    1,    0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_MRP),     (EDDP_MCFCT_RESOURCE_TYPE_MRP),    0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_FILTER),      (EDDP_SWI_FDB_GROUP_DLR_1)           },
/*54*/{0x01216C00,0x0002,0x0002,1,(EDDP_MCFCT_FRAME_HANDLING_NRT),  0,0,0, 0, 0x0,  1,    1,    1,    0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_MRP),     (EDDP_MCFCT_RESOURCE_TYPE_MRP),    0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_FILTER),      (EDDP_SWI_FDB_GROUP_DLR_2)           },
/*55*/{0x01216C00,0x0003,0x0003,1,(EDDP_MCFCT_FRAME_HANDLING_NRT),  0,0,0, 0, 0x0,  1,    1,    1,    0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_MRP),     (EDDP_MCFCT_RESOURCE_TYPE_MRP),    0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_FILTER),      (EDDP_SWI_FDB_GROUP_DLR_3)           },
/*56*/{0x0180C200,0x0000,0x0000,1,(EDDP_MCFCT_FRAME_HANDLING_NRT),  0,1,0, 0, 0x0,  1,    0,    0,    0,  1,  0,  0,  0,  1,  0,  0,  0,  1,  0,  0,  0,  1,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_OTHER),   (EDDP_MCFCT_RESOURCE_TYPE_OTHER),  0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_PORTS),       (EDDP_SWI_FDB_SPECIAL_HANDLING_RSTP) },
/*57*/{0x0180C200,0x0001,0x000D,1,(EDDP_MCFCT_FRAME_HANDLING_NRT),  0,1,0, 0, 0x0,  1,    0,    0,    0,  1,  0,  0,  0,  1,  0,  0,  0,  1,  0,  0,  0,  1,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_OTHER),   (EDDP_MCFCT_RESOURCE_TYPE_OTHER),  0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_FILTER),      (EDDP_SWI_FDB_NO_SPECIAL_HANDLING)   },
/*58*/{0x0180C200,0x000E,0x000E,1,(EDDP_MCFCT_FRAME_HANDLING_NRT),  0,1,0, 0, 0x0,  1,    1,    0,    0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_OTHER),   (EDDP_MCFCT_RESOURCE_TYPE_OTHER),  0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API),         (EDDP_SWI_FDB_NO_SPECIAL_HANDLING)   },
/*59*/{0x0180C200,0x000F,0x0010,1,(EDDP_MCFCT_FRAME_HANDLING_NRT),  0,1,0, 0, 0x0,  1,    1,    0,    0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_OTHER),   (EDDP_MCFCT_RESOURCE_TYPE_OTHER),  0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_FILTER),      (EDDP_SWI_FDB_NO_SPECIAL_HANDLING)   },

//     MAC,       LoMAC, HiMAC, V,(FrameHandling),                  R,F,BD,PM,TPAPI,LDAPI,MEAPI,MPAPI,TP1,LD1,ME1,MP1,TP2,LD2,ME2,MP2,TP3,LD3,ME3,MP3,TP4,LD4,ME4,MP4,(UseType),                        DTC, (API-Type),                    (Resource-Type),                   FFW,(Static),  RefCnt,(ForwardingRule_API),             (ForwardingRule_Ports),             (SpecialHandling)
/*60*/{0xFFFFFFFF,0xFFFF,0xFFFF,1,(EDDP_MCFCT_FRAME_HANDLING_NRT),  0,1,0, 0, 0x0,  0,    0,    0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  (EDDP_MCFCT_USE_TYPE_STANDARD),   0,   (EDDP_MCFCT_API_TYPE_OTHER),   (EDDP_MCFCT_RESOURCE_TYPE_OTHER),  0,  (LSA_TRUE),0,     (EDDP_SWI_FDB_FORWARD_PORTS),     (EDDP_SWI_FDB_FORWARD_API_PORTS),   (EDDP_SWI_FDB_NO_SPECIAL_HANDLING)   }
};
#endif



/*===========================================================================*/
/*                                 functions                                 */
/*===========================================================================*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname:             EDDP_SWI_fdb_setup                           +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB			+*/
/*+                                                                         +*/
/*+  pDDB         Pointer to Device Description Block                       +*/
/*+																			+*/
/*+  Result:      LSA_VOID                                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function sets up FDB-Control.						+*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWI_fdb_setup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT32      ParamStruct_Address;
    LSA_UINT32      UC_Table_A_Base_Address;
    LSA_UINT32      UC_Table_Size;

    // set up UC_Table_Base_A register
    UC_Table_A_Base_Address = (LSA_UINT32) EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_FDB_TABLE, EDDP_HAL_RAMSUB_FDB_TABLE_UC_TABLE_A, &UC_Table_Size);
    UC_Table_A_Base_Address = EDDP_HAL_H2P_ADDR(pDDB, UC_Table_A_Base_Address);
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_UC_TABLE_BASE_A, UC_Table_A_Base_Address);

    // set UC_LFSR_Range for IF A. Only certain values are allowed.
    switch (UC_Table_Size)
    {
        /* There is a macro that checks PNIP_R2A0_REG_UC_LFSR_RANGE_A, PNIP_R2F0_REG_UC_LFSR_RANGE_A,  PNIP_R3P2_REG_UC_LFSR_RANGE_A and PNIP_R3P4_REG_UC_LFSR_RANGE has the same value. */
        /* Checked in eddp_hal_pnip_val.h. We use the R2 version. */
        case 128:   EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2F0_REG_UC_LFSR_RANGE_A, 0x0000000);    break;
        case 256:   EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2F0_REG_UC_LFSR_RANGE_A, 0x0000001);    break;
        case 512:   EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2F0_REG_UC_LFSR_RANGE_A, 0x0000003);    break;
        case 1024:  EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2F0_REG_UC_LFSR_RANGE_A, 0x0000007);    break;
        case 2048:  EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2F0_REG_UC_LFSR_RANGE_A, 0x000000F);    break;
        case 4096:  EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2F0_REG_UC_LFSR_RANGE_A, 0x000001F);    break;
        case 8192:  EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2F0_REG_UC_LFSR_RANGE_A, 0x000003F);    break;
        case 16384: EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2F0_REG_UC_LFSR_RANGE_A, 0x000007F);    break;
        case 32768: EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2F0_REG_UC_LFSR_RANGE_A, 0x00000FF);    break;
        case 65536: EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2F0_REG_UC_LFSR_RANGE_A, 0x00001FF);    break;
        default:
        {
            EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
            EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_fdb_setup(): ERROR -> UC_Table_A size not valid!");
            return (EDD_STS_ERR_PARAM);
        }
        break;
    }

    // set up FDBParamStruct_Base register
    ParamStruct_Address = (LSA_UINT32) EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_FDB_TABLE, EDDP_HAL_RAMSUB_FDB_TABLE_PARAM_STRUCT, 0);
    ParamStruct_Address = EDDP_HAL_H2P_ADDR(pDDB, ParamStruct_Address);
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_FDBPARAMSTRUCT_BASE, ParamStruct_Address);

    // set up all MC_Default_Px registers
    _EDDP_SWI_Setup_MC_Default_Px (pDDB);

    switch(EDDP_HALGetChipVersion(pDDB))
    {
    
    #if ( (defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT)) || (defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)) )
    case EDDP_HAL_HW_ERTEC200P:
        {
            // Check if MC is big enough for all static entries
            if ( (sizeof(EDDP_SWI_multicast_table_ERTEC200P) / sizeof(EDDP_SWI_MULTICAST_ENTRY_TYPE)) > pDDB->SWI.MC_Table_Max_Entries )
            {
                EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_fdb_setup(): ERROR -> MC table is not big enough for all static MC entries!");
                EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_RESOURCE);
                return (EDD_STS_ERR_RESOURCE);
            }
        }
        break;
    #endif

    default:
        {
            EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
            EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_fdb_setup(): FATAL Hardware not supported!");
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }

    // fill shadow MC-Table with static MC-Entries and set up all TxPort_x fields in the MC Table
    _EDDP_SWI_Setup_Shadow_MC_Table (pDDB);

    // write shadow MC table values to MC Table A
    _EDDP_SWI_switch_MC_table (pDDB);

    return (EDD_STS_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_SWI_switch_MC_table                   +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE            pDDB     +*/
/*+						   :	EDDP_SWI_MULTICAST_ENTRY_PTR_TYPE  pMC_Table+*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:  This function copies the new/modified MC table from the  +*/
/*+ 			   Host/ARM to the FDB RAM and enables it by changing the   +*/
/*+                register value containing the MC table base address.     +*/
/*+                The PNIP_REG_MC_TABLE_RANGE register is also updated.    +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR _EDDP_SWI_switch_MC_table(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT32                          i;
    LSA_UINT32                          MC_Table_Base_Address;
    LSA_UINT32                          MC_Entry_Address;
    LSA_UINT64                          MC_Entry;
    EDDP_SWI_MULTICAST_ENTRY_PTR_TYPE   pMC_Table_Shadow;

    pMC_Table_Shadow = pDDB->SWI.pMC_Table_Shadow;

    if (pDDB->SWI.States.MC_Table_A_Active)
    {
        // get MC Table B base address
        MC_Table_Base_Address = (LSA_UINT32) EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_FDB_TABLE, EDDP_HAL_RAMSUB_FDB_TABLE_MC_TABLE_B, 0);
        // toggle MC_Table_A_Active status
        pDDB->SWI.States.MC_Table_A_Active = LSA_FALSE;
    }
    else
    {
        // get MC Table A base address
        MC_Table_Base_Address = (LSA_UINT32) EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_FDB_TABLE, EDDP_HAL_RAMSUB_FDB_TABLE_MC_TABLE_A, 0);
        // toggle MC_Table_A_Active status
        pDDB->SWI.States.MC_Table_A_Active = LSA_TRUE;
    }

    //set MC_Entry_Address to first entry
    MC_Entry_Address = MC_Table_Base_Address;

    for ( i=0; i < pDDB->SWI.MC_Table_Shadow_Entries_Count; i++ )
    {
        MC_Entry = ( ((LSA_UINT64)pMC_Table_Shadow[i].HighMAC)  << 0  |
                     ((LSA_UINT64)pMC_Table_Shadow[i].LowMAC)   << 16 |
                     ((LSA_UINT64)pMC_Table_Shadow[i].MAC)      << 32
                   );
        // write first 64bit of MC entry
        EDDP_HAL_MEM64_WRITE(pDDB, MC_Entry_Address, MC_Entry);

        MC_Entry = (((LSA_UINT64)pMC_Table_Shadow[i].Valid)                << 0  |
                    ((LSA_UINT64)pMC_Table_Shadow[i].FrameHandling)        << 1  |
                    ((LSA_UINT64)pMC_Table_Shadow[i].Redundancy)           << 3  |
                    ((LSA_UINT64)pMC_Table_Shadow[i].Fragmentation)        << 4  |
                    ((LSA_UINT64)pMC_Table_Shadow[i].BridgeDelay_Enable)   << 5  |
                    ((LSA_UINT64)pMC_Table_Shadow[i].PriorityMapping)      << 7  |
                    ((LSA_UINT64)pMC_Table_Shadow[i].TxPort_API)           << 8  |
                    ((LSA_UINT64)pMC_Table_Shadow[i].LD_API)               << 13 |
                    ((LSA_UINT64)pMC_Table_Shadow[i].MgmtENable_API)       << 14 |
                    ((LSA_UINT64)pMC_Table_Shadow[i].MgmtPrio_API)         << 15 |
                    ((LSA_UINT64)pMC_Table_Shadow[i].TxPort_1)             << 16 |
                    ((LSA_UINT64)pMC_Table_Shadow[i].LD_1)                 << 21 |
                    ((LSA_UINT64)pMC_Table_Shadow[i].MgmtENable_1)         << 22 |
                    ((LSA_UINT64)pMC_Table_Shadow[i].MgmtPrio_1)           << 23 |
                    ((LSA_UINT64)pMC_Table_Shadow[i].TxPort_2)             << 24 |
                    ((LSA_UINT64)pMC_Table_Shadow[i].LD_2)                 << 29 |
                    ((LSA_UINT64)pMC_Table_Shadow[i].MgmtENable_2)         << 30 |
                    ((LSA_UINT64)pMC_Table_Shadow[i].MgmtPrio_2)           << 31 |
                    ((LSA_UINT64)pMC_Table_Shadow[i].TxPort_3)             << 32 |
                    ((LSA_UINT64)pMC_Table_Shadow[i].LD_3)                 << 37 |
                    ((LSA_UINT64)pMC_Table_Shadow[i].MgmtENable_3)         << 38 |
                    ((LSA_UINT64)pMC_Table_Shadow[i].MgmtPrio_3)           << 39 |
                    ((LSA_UINT64)pMC_Table_Shadow[i].TxPort_4)             << 40 |
                    ((LSA_UINT64)pMC_Table_Shadow[i].LD_4)                 << 45 |
                    ((LSA_UINT64)pMC_Table_Shadow[i].MgmtENable_4)         << 46 |
                    ((LSA_UINT64)pMC_Table_Shadow[i].MgmtPrio_4)           << 47 |
                    ((LSA_UINT64)pMC_Table_Shadow[i].UseType)              << 48 |
                    ((LSA_UINT64)pMC_Table_Shadow[i].DefaultTrafficClass)  << 51 |
                    ((LSA_UINT64)pMC_Table_Shadow[i].APIType)              << 54 |
                    ((LSA_UINT64)pMC_Table_Shadow[i].ResourceType)         << 59 |
                    ((LSA_UINT64)pMC_Table_Shadow[i].FastForwarding)       << 63  );
        // set address to next 8 byte
        MC_Entry_Address = MC_Entry_Address + 8;
        // write second 64bit of MC entry
        EDDP_HAL_MEM64_WRITE(pDDB, MC_Entry_Address, MC_Entry);
        // set address to next 8 byte
        MC_Entry_Address = MC_Entry_Address + 8;
    }

    // set MC_Table_Range
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MC_TABLE_RANGE, pDDB->SWI.MC_Table_Shadow_Entries_Count);

    // set up MC_Table_Base_Address
    MC_Table_Base_Address = EDDP_HAL_H2P_ADDR(pDDB, MC_Table_Base_Address);
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MC_TABLE_BASE, MC_Table_Base_Address);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_SWI_Setup_Shadow_MC_Table            +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE            pDDB     +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:  This function copies the static MC-Entries from          +*/
/*+                EDDP_SWI_multicast_table to the lokal shadow MC-Table    +*/
/*+                and configures all TxPort fields within the MC table     +*/
/*+ 			   (on Host/ARM) corresponding to the port-to-interface     +*/
/*+                grouping.                                                +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR _EDDP_SWI_Setup_Shadow_MC_Table(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT32      i, k, j;
    LSA_UINT32      HwPortID;
    LSA_UINT8       TxPort_Value;
	
	i=0;

    // Copy static MC-Entries into lokal shadow MC-Table
    switch(EDDP_HALGetChipVersion(pDDB))
    {
    #if ( (defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT)) || (defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)) )
    case EDDP_HAL_HW_ERTEC200P:
        {
            for ( i=0; i < (sizeof(EDDP_SWI_multicast_table_ERTEC200P) / sizeof(EDDP_SWI_MULTICAST_ENTRY_TYPE)); i++ )
            {
                pDDB->SWI.pMC_Table_Shadow[i] = EDDP_SWI_multicast_table_ERTEC200P[i];
            }
        }
        break;
    #endif

    default:
        {
            EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
            EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] _EDDP_SWI_Setup_Shadow_MC_Table(): ERROR -> Hardware not supported");
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }
    pDDB->SWI.MC_Table_Shadow_Entries_Count = i;

    for ( k=0; k < pDDB->SWI.MC_Table_Shadow_Entries_Count; k++ )
    {
        /* Remove HSYNC MC Addr if not supported */
        if (EDDP_SWI_FDB_GROUP_HSYNC == pDDB->SWI.pMC_Table_Shadow[k].SpecialHandling)
        {
            if (EDD_FEATURE_ENABLE != pDDB->FeatureSupport.AdditionalForwardingRulesSupported)
            {
                /* HSYNC role "none" --> remove HSYNC mac from table */
                EDDP_GEN_Move_MC_Table_Up_Down(pDDB, k, LSA_TRUE);
            }
        }

        /* Remove MRP interconnect MC Addr´s if not supported */
        if (EDDP_SWI_FDB_GROUP_MRP_IC_1 == pDDB->SWI.pMC_Table_Shadow[k].SpecialHandling)
        {
            if (EDD_FEATURE_ENABLE != pDDB->FeatureSupport.MRPInterconnFwdRulesSupported)
            {
                /* No MRP IC forwarder  --> remove addr from table */
                EDDP_GEN_Move_MC_Table_Up_Down(pDDB, k, LSA_TRUE);
            }
        }
        /* Remove MRP interconnect MC Addr´s if not supported */
        if (EDDP_SWI_FDB_GROUP_MRP_IC_2 == pDDB->SWI.pMC_Table_Shadow[k].SpecialHandling)
        {
            if (EDD_FEATURE_ENABLE != pDDB->FeatureSupport.MRPInterconnFwdRulesSupported)
            {
                /* No MRP IC forwarder  --> remove addr from table */
                EDDP_GEN_Move_MC_Table_Up_Down(pDDB, k, LSA_TRUE);
            }
        }
        /* Remove MRP interconnect MC Addr´s if not supported */
        if (EDDP_SWI_FDB_GROUP_MRP_IC_3 == pDDB->SWI.pMC_Table_Shadow[k].SpecialHandling)
        {
            if (EDD_FEATURE_ENABLE != pDDB->FeatureSupport.MRPInterconnFwdRulesSupported)
            {
                /* No MRP IC forwarder  --> remove addr from table */
                EDDP_GEN_Move_MC_Table_Up_Down(pDDB, k, LSA_TRUE);
            }
        }

        /* Remove DLR entries if EIP/DLR not supported */
        if (EDD_FEATURE_ENABLE != pDDB->FeatureSupport.DLRExtensions)
        {
            if (   (EDDP_SWI_FDB_GROUP_DLR_1 == pDDB->SWI.pMC_Table_Shadow[k].SpecialHandling)
                || (EDDP_SWI_FDB_GROUP_DLR_2 == pDDB->SWI.pMC_Table_Shadow[k].SpecialHandling)
                || (EDDP_SWI_FDB_GROUP_DLR_3 == pDDB->SWI.pMC_Table_Shadow[k].SpecialHandling) )
            {
                /* No DLR support --> remove addr from table */
                EDDP_GEN_Move_MC_Table_Up_Down(pDDB, k, LSA_TRUE);
            }
        }

        // skip the following steps if the last entry in the table has just been removed
        if (k != pDDB->SWI.MC_Table_Shadow_Entries_Count)
        {
            for (j = 1; j <= pDDB->SWI.PortCnt; j++)
            {
                HwPortID = EDDP_SWI_get_HwPortID(pDDB, j);

                /* ------------------------------------ */
                /* set TxPort_1..4                      */
                /* ------------------------------------ */
                TxPort_Value = 0;   // initialize variable

                if (   (pDDB->SWI.pMC_Table_Shadow[k].ForwardingRule_Ports == EDDP_SWI_FDB_FORWARD_API_PORTS)
                    || (pDDB->SWI.pMC_Table_Shadow[k].ForwardingRule_Ports == EDDP_SWI_FDB_FORWARD_API)
                    )
                {
                    TxPort_Value |= 0x1;    // forwarding to API
                }

                if (   (pDDB->SWI.pMC_Table_Shadow[k].ForwardingRule_Ports == EDDP_SWI_FDB_FORWARD_API_PORTS)
                    || (pDDB->SWI.pMC_Table_Shadow[k].ForwardingRule_Ports == EDDP_SWI_FDB_FORWARD_PORTS)
                    )
                {
                    TxPort_Value |= pDDB->SWI.PortParams[HwPortID].TxPort;  // forwarding to all other Ports of this IF
                }

                if (pDDB->SWI.pMC_Table_Shadow[k].ForwardingRule_Ports == EDDP_SWI_FDB_FORWARD_FILTER)
                {
                    // TxPort_Value = 0; comment is here just to illustrate all possible forwarding rules
                }

                if (pDDB->SWI.pMC_Table_Shadow[k].SpecialHandling == EDDP_SWI_FDB_SPECIAL_HANDLING_RSTP)
                {
                    // In case this MC entry belongs to the rapid spanning tree protocol we configure the forwarding rule according to the
                    // parameter set by system adaption at DeviceOpen.
                    // TxPort or Filter
                    TxPort_Value = pDDB->SWI.Multicast_Bridge_IEEE802RSTP_Forward ? pDDB->SWI.PortParams[HwPortID].TxPort : 0;
                }
                else if ((EDD_FEATURE_ENABLE == pDDB->FeatureSupport.DLRExtensions)
                    && ((EDDP_SWI_FDB_GROUP_MRP_1 == pDDB->SWI.pMC_Table_Shadow[k].SpecialHandling)
                        || (EDDP_SWI_FDB_GROUP_MRP_2 == pDDB->SWI.pMC_Table_Shadow[k].SpecialHandling))
                    )
                {
                    /* Manipulate MRP entries if DLR supported (and thus MRP not supported) */
                    TxPort_Value = pDDB->SWI.PortParams[HwPortID].TxPort;  // forwarding to all other Ports of this IF, no forwarding to API
                }

                // copy "TxPort_Value" to SWI
                switch (HwPortID)
                {
                    case 1:
                        pDDB->SWI.pMC_Table_Shadow[k].TxPort_1 = TxPort_Value;
                    break;

                    case 2:
                        pDDB->SWI.pMC_Table_Shadow[k].TxPort_2 = TxPort_Value;
                    break;
                    default:
                    {
                        // this cannot happen -> wrong implementation!
                        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] _EDDP_SWI_Setup_Shadow_MC_Table(): FATAL -> HWPortID(%d) is not valid!",
                            HwPortID);
                        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                    }
                    break;
                }

                /* ------------------------------------ */
                /* set TxPort_API                       */
                /* ------------------------------------ */
                if (pDDB->SWI.pMC_Table_Shadow[k].ForwardingRule_API == EDDP_SWI_FDB_FORWARD_PORTS)
                {
                    // forward local frames to all mapped ports of this device
                    pDDB->SWI.pMC_Table_Shadow[k].TxPort_API |= pDDB->SWI.PortParams[HwPortID].TxPort;
                }
                // else: EDDP_SWI_FDB_FORWARD_FILTER
            }

            if (   (EDD_FEATURE_ENABLE == pDDB->FeatureSupport.DLRExtensions)
                && (   (EDDP_SWI_FDB_GROUP_MRP_1 == pDDB->SWI.pMC_Table_Shadow[k].SpecialHandling)
                    || (EDDP_SWI_FDB_GROUP_MRP_2 == pDDB->SWI.pMC_Table_Shadow[k].SpecialHandling))
                    )
                {
                    /* Manipulate MRP entries if DLR supported (and thus MRP not supported) */
                    pDDB->SWI.pMC_Table_Shadow[k].APIType        = EDDP_MCFCT_API_TYPE_OTHER;
                    pDDB->SWI.pMC_Table_Shadow[k].ResourceType   = EDDP_MCFCT_RESOURCE_TYPE_OTHER;
                    pDDB->SWI.pMC_Table_Shadow[k].MgmtENable_1   = 0;
                    pDDB->SWI.pMC_Table_Shadow[k].MgmtENable_2   = 0;
                    pDDB->SWI.pMC_Table_Shadow[k].MgmtENable_3   = 0;
                    pDDB->SWI.pMC_Table_Shadow[k].MgmtENable_4   = 0;
                    pDDB->SWI.pMC_Table_Shadow[k].MgmtENable_API = 0;
                    pDDB->SWI.pMC_Table_Shadow[k].MgmtPrio_1     = 0;
                    pDDB->SWI.pMC_Table_Shadow[k].MgmtPrio_2     = 0;
                    pDDB->SWI.pMC_Table_Shadow[k].MgmtPrio_3     = 0;
                    pDDB->SWI.pMC_Table_Shadow[k].MgmtPrio_4     = 0;
                    pDDB->SWI.pMC_Table_Shadow[k].MgmtPrio_API   = 0;
            }
        }
    }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_SWI_Setup_MC_Default_Px               +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE            pDDB     +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:  This function configures the default MC settings for each+*/
/*+                port corresponding to the port-to-interface mapping.     +*/
/*+                This mapping information is then stored in DDB for later +*/
/*+                usage.                                                   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR _EDDP_SWI_Setup_MC_Default_Px(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT32      i,j,m;
    LSA_UINT32      TxPort;
    LSA_UINT8       TxPort_P1 = 0, TxPort_P2 = 0;
    LSA_UINT8       TxPort_P3 = 0, TxPort_P4 = 0;

    // set MC_Default_Px registers according to the "interface-to-HWPortID-mapping"
    for ( j=1; j <= pDDB->SWI.PortCnt; j++ )
    {
        TxPort = EDDP_SWI_get_HwPortID (pDDB, j);

        for ( m=1; m <= pDDB->SWI.PortCnt; m++ )
        {
            switch (EDDP_SWI_get_HwPortID(pDDB, m))
            {
                case 1:
                {
                    TxPort_P1 |= (0x1 << TxPort);
                    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MC_DEFAULT_P1, TxPort_P1);
                }
                break;

                case 2:
                {
                    TxPort_P2 |= (0x1 << TxPort);
                    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MC_DEFAULT_P2, TxPort_P2);
                }
                break;
                default:
                {
                    // this cannot happen -> wrong implementation!
                    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] _EDDP_SWI_Setup_MC_Default_Px(): FATAL -> HWPortID(%d) is not valid!",
                        EDDP_SWI_get_HwPortID(pDDB, m));
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                }
                break;
            }
        }
    }

    // write PNIP_REG_MC_DEFAULT_API: forward local frames to all mapped ports of this device
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MC_DEFAULT_API, (TxPort_P1 | TxPort_P2 | TxPort_P3 | TxPort_P4 ));

    // store all calculated TxPort settings for each port in DDB
    for ( i=1; i <= EDDP_MAX_PORT_CNT; i++ )
    {
        pDDB->SWI.PortParams[i].TxPort = (i==1 ? TxPort_P1 : 
                                         (i==2 ? TxPort_P2 : 
                                         (i==3 ? TxPort_P3 : 
                                         (i==4 ? TxPort_P4 : 0))));
    }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname        : EDDP_SWI_Change_MC_Entry_Parameters              +*/
/*+  Input/Output        : EDDP_LOCAL_DDB_PTR_TYPE             pDDB         +*/
/*+                      : EDDP_SWI_MULTICAST_ENTRY_PTR_TYPE   pMC_Entry    +*/
/*+                      : LSA_UINT32                          Index        +*/
/*+                                                                         +*/
/*+  Result              : LSA_VOID                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                (in) +*/
/*+  pMC_Entry      : Pointer to a MC entry                            (in) +*/
/*+  Index          : Index of MC entry in MC table (PNIP)             (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function changes the MC entry settings on the        +*/
/*+               selected entry (MC_Table_Index) by overwriting them with  +*/
/*+				  the settings specified in pMC_Entry.                      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Change_MC_Entry_Parameters(
	EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_SWI_MULTICAST_ENTRY_PTR_TYPE       pMC_Entry,
    LSA_UINT32                              Index)
{
    LSA_UINT32      MC_Entry_Address;
    LSA_UINT64      MC_Parameters;

	if (Index >= pDDB->SWI.MC_Table_Shadow_Entries_Count)
	{
		// Parameter Index is out of Range
        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_Change_MC_Entry_Parameters(): FATAL -> Index(%d) is not valid!",
                        Index);
		EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
	}

    MC_Entry_Address    = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_MC_TABLE_BASE);
    MC_Entry_Address    = EDDP_HAL_P2H_ADDR(pDDB, MC_Entry_Address);
    MC_Entry_Address    = MC_Entry_Address + (16 * Index) + 8;  // each MC entry is 16 bytes long; +8bytes to address forwarding parameters

    MC_Parameters = (((LSA_UINT64)pMC_Entry->Valid)                << 0  |
                     ((LSA_UINT64)pMC_Entry->FrameHandling)        << 1  |
                     ((LSA_UINT64)pMC_Entry->Redundancy)           << 3  |
                     ((LSA_UINT64)pMC_Entry->Fragmentation)        << 4  |
                     ((LSA_UINT64)pMC_Entry->BridgeDelay_Enable)   << 5  |
                     ((LSA_UINT64)pMC_Entry->PriorityMapping)      << 7  |
                     ((LSA_UINT64)pMC_Entry->TxPort_API)           << 8  |
                     ((LSA_UINT64)pMC_Entry->LD_API)               << 13 |
                     ((LSA_UINT64)pMC_Entry->MgmtENable_API)       << 14 |
                     ((LSA_UINT64)pMC_Entry->MgmtPrio_API)         << 15 |
                     ((LSA_UINT64)pMC_Entry->TxPort_1)             << 16 |
                     ((LSA_UINT64)pMC_Entry->LD_1)                 << 21 |
                     ((LSA_UINT64)pMC_Entry->MgmtENable_1)         << 22 |
                     ((LSA_UINT64)pMC_Entry->MgmtPrio_1)           << 23 |
                     ((LSA_UINT64)pMC_Entry->TxPort_2)             << 24 |
                     ((LSA_UINT64)pMC_Entry->LD_2)                 << 29 |
                     ((LSA_UINT64)pMC_Entry->MgmtENable_2)         << 30 |
                     ((LSA_UINT64)pMC_Entry->MgmtPrio_2)           << 31 |
                     ((LSA_UINT64)pMC_Entry->TxPort_3)             << 32 |
                     ((LSA_UINT64)pMC_Entry->LD_3)                 << 37 |
                     ((LSA_UINT64)pMC_Entry->MgmtENable_3)         << 38 |
                     ((LSA_UINT64)pMC_Entry->MgmtPrio_3)           << 39 |
                     ((LSA_UINT64)pMC_Entry->TxPort_4)             << 40 |
                     ((LSA_UINT64)pMC_Entry->LD_4)                 << 45 |
                     ((LSA_UINT64)pMC_Entry->MgmtENable_4)         << 46 |
                     ((LSA_UINT64)pMC_Entry->MgmtPrio_4)           << 47 |
                     ((LSA_UINT64)pMC_Entry->UseType)              << 48 |
                     ((LSA_UINT64)pMC_Entry->DefaultTrafficClass)  << 51 |
                     ((LSA_UINT64)pMC_Entry->APIType)              << 54 |
                     ((LSA_UINT64)pMC_Entry->ResourceType)         << 59 |
                     ((LSA_UINT64)pMC_Entry->FastForwarding)       << 63  );

    // write second 64bit of MC entry
    EDDP_HAL_MEM64_WRITE(pDDB, MC_Entry_Address, MC_Parameters);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname        : EDDP_SWI_MC_Entry_Set_To_Default                 +*/
/*+  Input/Output        : EDDP_LOCAL_DDB_PTR_TYPE           pDDB           +*/
/*+                      : LSA_UINT32                        Index          +*/
/*+                                                                         +*/
/*+  Result              : LSA_VOID                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                (in) +*/
/*+  Index          : Index of MC entry in MC table (PNIP and shadow)  (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function restores the default MC entry settings for  +*/
/*+               a static MC entry. Changes are made both on the shadow    +*/
/*+               MC table and active MC table on the PNIP (FDB ram).       +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_MC_Entry_Set_To_Default(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  Index)
{
    LSA_UINT32      n;
    LSA_UINT32      HwPortID;
    LSA_UINT8       TxPort_Value;

	if (Index >= pDDB->SWI.MC_Table_Shadow_Entries_Count)
	{
		// Parameter Index is out of Range
        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_MC_Entry_Set_To_Default(): FATAL -> Index(%d) is not valid!",
                        Index);
		EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
	}

    for ( n=1; n <= pDDB->SWI.PortCnt; n++ )
    {
        HwPortID = EDDP_SWI_get_HwPortID (pDDB, n);

        /* ------------------------------------ */
        /* set TxPort_1..4                      */
        /* ------------------------------------ */
        TxPort_Value = 0;   // initialize variable

        if (   (pDDB->SWI.pMC_Table_Shadow[Index].ForwardingRule_Ports == EDDP_SWI_FDB_FORWARD_API_PORTS)
            || (pDDB->SWI.pMC_Table_Shadow[Index].ForwardingRule_Ports == EDDP_SWI_FDB_FORWARD_API)
            || (pDDB->SWI.pMC_Table_Shadow[Index].RefCnt > 0) /* somebody still uses this MC entry */
            )
        {
            TxPort_Value |= 0x001;  // forwarding to API
        }

        if (   (pDDB->SWI.pMC_Table_Shadow[Index].ForwardingRule_Ports == EDDP_SWI_FDB_FORWARD_API_PORTS)
            || (pDDB->SWI.pMC_Table_Shadow[Index].ForwardingRule_Ports == EDDP_SWI_FDB_FORWARD_PORTS)
            )
        {
            TxPort_Value |= pDDB->SWI.PortParams[HwPortID].TxPort;  // forwarding to all other Ports of this IF
        }

        if (pDDB->SWI.pMC_Table_Shadow[Index].ForwardingRule_Ports == EDDP_SWI_FDB_FORWARD_FILTER)
        {
            // TxPort_Value = 0; comment is here just to illustrate all possible forwarding rules
        }

        if (pDDB->SWI.pMC_Table_Shadow[Index].SpecialHandling == EDDP_SWI_FDB_SPECIAL_HANDLING_RSTP)
        {
            // In case this MC entry belongs to the rapid spanning tree protocol we configure the forwarding rule according to the
            // parameter set by system adaption at DeviceOpen.
            TxPort_Value = pDDB->SWI.Multicast_Bridge_IEEE802RSTP_Forward ? pDDB->SWI.PortParams[HwPortID].TxPort : 0;
        }

        // copy "TxPort_Value" to SWI
        switch (HwPortID)
        {
            case 1:
                pDDB->SWI.pMC_Table_Shadow[Index].TxPort_1 = TxPort_Value;
            break;

            case 2:
                pDDB->SWI.pMC_Table_Shadow[Index].TxPort_2 = TxPort_Value;
            break;
            default:
            {
                // this cannot happen -> wrong implementation!
                EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_MC_Entry_Set_To_Default(): FATAL -> HWPortID(%d) is not valid!",
                    HwPortID);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }

        /* ------------------------------------ */
        /* set TxPort_API                       */
        /* ------------------------------------ */
        if (pDDB->SWI.pMC_Table_Shadow[Index].ForwardingRule_API == EDDP_SWI_FDB_FORWARD_PORTS)
        {
            // forward local frames to all mapped ports of this device
            pDDB->SWI.pMC_Table_Shadow[Index].TxPort_API |= pDDB->SWI.PortParams[HwPortID].TxPort;
        }
        // else: EDDP_SWI_FDB_FORWARD_FILTER
    }

    // write changes to active MC table in FDB ram (PNIP)
    EDDP_SWI_Change_MC_Entry_Parameters (pDDB, &pDDB->SWI.pMC_Table_Shadow[Index], Index);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname        : EDDP_SWI_Change_Forwarding                       +*/
/*+  Input/Output        : EDDP_LOCAL_DDB_PTR_TYPE           pDDB           +*/
/*+                      : LSA_UINT32                        Index          +*/
/*+                      : LSA_UINT32                        PortID         +*/
/*+                      : LSA_BOOL                          Forward        +*/
/*+                                                                         +*/
/*+  Result              : EDD_RSP                                          +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                (in) +*/
/*+  Index          : Index of MC entry in MC table (PNIP and shadow)  (in) +*/
/*+  PortID         : PortID to which the forwarding changes apply     (in) +*/
/*+  Forward        : LSA_TRUE : Forward to this port                  (in) +*/
/*+                   LSA_FALSE: Do not forward to this port                +*/
/*+                                                                         +*/
/*+  Results        : EDD_STS_OK                                            +*/
/*+                 : EDD_STS_ERR_PARAM                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function changes the forwarding rules for a given    +*/
/*+               MC entry.                                                 +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWI_Change_Forwarding(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  Index,
    EDD_RQB_PORTID_MODE_TYPE    PortIDMode,
    LSA_BOOL                    Forward)
{
    LSA_UINT32          HwPortID_loop;
    LSA_UINT32          HwPortID_forward;
    EDD_RSP             Status;
    LSA_UINT8           i;

	if (Index >= pDDB->SWI.MC_Table_Shadow_Entries_Count)
	{
		// Parameter Index is out of Range
        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_Change_Forwarding(): FATAL -> Index(%d) is not valid!",
                        Index);
		EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
	}

    Status              = EDD_STS_OK;
    HwPortID_forward    = EDDP_SWI_get_HwPortID (pDDB, PortIDMode.DstPortID);

    if (HwPortID_forward != 0)  // check if EDDP_SWI_get_HwPortID returned with no error
    {
        i = 1;
        do
        {
            HwPortID_loop = EDDP_SWI_get_HwPortID (pDDB, i);

            switch (HwPortID_loop)
            {
                case 1:
                {
					if (Forward)
						pDDB->SWI.pMC_Table_Shadow[Index].TxPort_1 = (pDDB->SWI.pMC_Table_Shadow[Index].TxPort_1 | (0x1 << HwPortID_forward));
					else
						pDDB->SWI.pMC_Table_Shadow[Index].TxPort_1 = (pDDB->SWI.pMC_Table_Shadow[Index].TxPort_1 & ~(0x1 << HwPortID_forward));
                }
                break;

                case 2:
                {
					if (Forward)
						pDDB->SWI.pMC_Table_Shadow[Index].TxPort_2 = (pDDB->SWI.pMC_Table_Shadow[Index].TxPort_2 | (0x1 << HwPortID_forward));
					else
						pDDB->SWI.pMC_Table_Shadow[Index].TxPort_2 = (pDDB->SWI.pMC_Table_Shadow[Index].TxPort_2 & ~(0x1 << HwPortID_forward));
                }
                break;

                default:
                {
                    // this cannot happen -> wrong implementation!
                    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_Change_Forwarding(): FATAL -> HWPortID(%d) is not valid!",
                        HwPortID_loop);
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                }
                break;
            }
            
            ++i;
        }
        while(pDDB->SWI.PortCnt >= i);

        EDDP_SWI_Change_MC_Entry_Parameters (pDDB, &pDDB->SWI.pMC_Table_Shadow[Index], Index);
    }
    else
    {
        Status = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Status);
        EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_Change_Forwarding(): ERROR -> No valid PortID!");
    }

    return (Status);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname        : EDDP_SWI_ChangeAnnounceFwd                       +*/
/*+  Input/Output        : EDDP_LOCAL_DDB_PTR_TYPE           pDDB           +*/
/*+                      : LSA_UINT32                        Index          +*/
/*+                      : LSA_UINT32                        PortID         +*/
/*+                      : LSA_BOOL                          Forward        +*/
/*+                                                                         +*/
/*+  Result              : EDD_RSP                                          +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                (in) +*/
/*+  Index          : Index of MC entry in MC table (PNIP and shadow)  (in) +*/
/*+  PortID         : PortID to which the forwarding changes apply     (in) +*/
/*+  Forward        : LSA_TRUE : Forward to this port                  (in) +*/
/*+                   LSA_FALSE: Do not forward to this port                +*/
/*+                                                                         +*/
/*+  Results        : EDD_STS_OK                                            +*/
/*+                 : EDD_STS_ERR_PARAM                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function changes the forwarding rules for a given    +*/
/*+               MC entry.                                                 +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWI_ChangeAnnounceFwd(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  Index,
    LSA_UINT32                  PortID,
    LSA_BOOL                    Forward,
    LSA_UINT8                   SyncId)
{
    LSA_UINT32          HwPortID_loop;
    LSA_UINT32          HwPortID_forward;
    LSA_UINT32          HwPortID_egress;
    EDD_RSP             Status;
    LSA_UINT8           i, j;
    LSA_BOOL			DoChange;

	if (Index >= pDDB->SWI.MC_Table_Shadow_Entries_Count)
	{
		// Parameter Index is out of Range
        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_ChangeAnnounceFwd(): FATAL -> Index(%d) is not valid!",
                        Index);
		EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
	}

    Status              = EDD_STS_OK;
    HwPortID_forward    = EDDP_SWI_get_HwPortID (pDDB, PortID);

    if (HwPortID_forward != 0)  // check if EDDP_SWI_get_HwPortID returned with no error
    {
        for ( i=1; i <= pDDB->SWI.PortCnt; i++ )
        {
            HwPortID_loop = EDDP_SWI_get_HwPortID (pDDB, i);

			/* Forward only if no boundary is set for announce frames
			*/
			if ((i == PortID) || (SyncId >= EDDP_PRM_DOMAIN_BOUNDARY_COUNT) || !pDDB->SWI.AnnounceBoundary[i][SyncId])
			{
				DoChange = LSA_TRUE;
			}
			else
			{
			    DoChange = LSA_FALSE;
			}

            switch (HwPortID_loop)
            {
                case 1:
                {
					if (i != PortID)
					{
						// Add or remove forwarding from port <i> to port <PortID>
						if (Forward)
						{
							if (DoChange)
							{
								// remove boundary
								pDDB->SWI.pMC_Table_Shadow[Index].TxPort_1 = (pDDB->SWI.pMC_Table_Shadow[Index].TxPort_1 | (LSA_UINT8)(0x1U << HwPortID_forward));
							}
						}
						else
						{
							// set boundary
							pDDB->SWI.pMC_Table_Shadow[Index].TxPort_1 = (pDDB->SWI.pMC_Table_Shadow[Index].TxPort_1 & ~(0x1U << HwPortID_forward));
						}
					}
					else
					{
						// Add or remove forwarding from port <PortID> to other ports (ingress boundary for PortID)
						if (!Forward)
						{
							//set boundary: only API left
							pDDB->SWI.pMC_Table_Shadow[Index].TxPort_1 = (pDDB->SWI.pMC_Table_Shadow[Index].TxPort_1 & 0x1);
						}
						// else remove egress boundaries
						else
						{
                            for (j=1; j <= pDDB->SWI.PortCnt; j++)
                            {
                                HwPortID_egress = EDDP_SWI_get_HwPortID (pDDB, j);
                                if (((SyncId >= EDDP_PRM_DOMAIN_BOUNDARY_COUNT) || !pDDB->SWI.AnnounceBoundary[j][SyncId])
                                && (0 != pDDB->SWI.LinkStatus[j].LineDelayInTicksMeasured))
                                {
                                    // remove boundary
                                    pDDB->SWI.pMC_Table_Shadow[Index].TxPort_1 = (pDDB->SWI.pMC_Table_Shadow[Index].TxPort_1 | (LSA_UINT8)(0x1U << HwPortID_egress));
                                }
                            }
						}
					}
                }
                break;

                case 2:
                {
					if (i != PortID)
					{
						// Add or remove forwarding from port <i> to port <PortID>
						if (Forward)
						{
							if (DoChange)
							{
								// remove boundary
								pDDB->SWI.pMC_Table_Shadow[Index].TxPort_2 = (pDDB->SWI.pMC_Table_Shadow[Index].TxPort_2 | (LSA_UINT8)(0x1U << HwPortID_forward));
							}
						}
						else
						{
							// set boundary
							pDDB->SWI.pMC_Table_Shadow[Index].TxPort_2 = (pDDB->SWI.pMC_Table_Shadow[Index].TxPort_2 & ~(0x1U << HwPortID_forward));
						}
					}
					else
					{
						// Add or remove forwarding from port <PortID> to other ports (ingress boundary for PortID)
						if (!Forward)
						{
							//set boundary: only API left
							pDDB->SWI.pMC_Table_Shadow[Index].TxPort_2 = (pDDB->SWI.pMC_Table_Shadow[Index].TxPort_2 & 0x1);
						}
                        // 040620lrg: else remove egress boundaries
                        else
                        {
                            for (j=1; j <= pDDB->SWI.PortCnt; j++)
                            {
                                HwPortID_egress = EDDP_SWI_get_HwPortID (pDDB, j);
                                if (((SyncId >= EDDP_PRM_DOMAIN_BOUNDARY_COUNT) || !pDDB->SWI.AnnounceBoundary[j][SyncId])
                                && (0 != pDDB->SWI.LinkStatus[j].LineDelayInTicksMeasured))
                                {
                                    // remove boundary
                                    pDDB->SWI.pMC_Table_Shadow[Index].TxPort_2 = (pDDB->SWI.pMC_Table_Shadow[Index].TxPort_2 | (LSA_UINT8)(0x1U << HwPortID_egress));
                                }
                            }
                        }
					}
                }
                break;

                default:
                {
                    // this cannot happen -> wrong implementation!
                    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_ChangeAnnounceFwd(): FATAL -> HWPortID(%d) is not valid!",
                        HwPortID_loop);
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                }
                break;
            }
        }

        EDDP_SWI_Change_MC_Entry_Parameters (pDDB, &pDDB->SWI.pMC_Table_Shadow[Index], Index);
    }
    else
    {
        Status = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Status);
        EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_ChangeAnnounceFwd(): ERROR -> No valid PortID!");
    }

    return (Status);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname        : EDDP_SWI_MC_Entry_Get_Index                      +*/
/*+  Input/Output        : EDDP_LOCAL_DDB_PTR_TYPE           pDDB           +*/
/*+                      : EDD_MAC_ADR_TYPE                  MAC_Address    +*/
/*+                                                                         +*/
/*+  Result              : LSA_UINT32                                       +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB             : Pointer to DeviceDescriptionBlock              (in) +*/
/*+  MAC_AddressIndex : MAC address of desired MC-Entry                (in) +*/
/*+                                                                         +*/
/*+  Results          : 0..pDDB->SWI.MC_Table_Shadow_Entries_Count -1       +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function returns the shadow MC table index for the   +*/
/*+               given MAC address.                                        +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_SWI_MC_Entry_Get_Index(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_MAC_ADR_TYPE            MAC_Address)
{
    LSA_BOOL        MC_entry_found = LSA_FALSE;
    LSA_UINT32      Index;
    LSA_UINT32      MAC_upper;
    LSA_UINT16      MAC_lower;

    MAC_lower = ((LSA_UINT32)MAC_Address.MacAdr[5])       |
                ((LSA_UINT32)MAC_Address.MacAdr[4]) << 8  ;

    MAC_upper = ((LSA_UINT32)MAC_Address.MacAdr[3])       |
                ((LSA_UINT32)MAC_Address.MacAdr[2]) << 8  |
                ((LSA_UINT32)MAC_Address.MacAdr[1]) << 16 |
                ((LSA_UINT32)MAC_Address.MacAdr[0]) << 24 ;

    for ( Index=0; Index < pDDB->SWI.MC_Table_Shadow_Entries_Count; Index++ )
    {
        // we only compare for MAC and LowMAC so we can also modify MAC address bands.
        if ( (MAC_upper == pDDB->SWI.pMC_Table_Shadow[Index].MAC) && (MAC_lower == pDDB->SWI.pMC_Table_Shadow[Index].LowMAC) )
        {
            // found
            MC_entry_found = LSA_TRUE;
            break;
        }
    }

    if ( !(MC_entry_found) )
    {
        EDDP_SWI_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_MC_Entry_Get_Index(): FATAL -> given MAC(%2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X) not found!",
            MAC_Address.MacAdr[0],
            MAC_Address.MacAdr[1],
            MAC_Address.MacAdr[2],
            MAC_Address.MacAdr[3],
            MAC_Address.MacAdr[4],
            MAC_Address.MacAdr[5]);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    return (Index);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname        : EDDP_SWI_MC_Entry_Set_DomainBoundary             +*/
/*+  Input/Output        : EDDP_LOCAL_DDB_PTR_TYPE      pDDB                +*/
/*+                      : LSA_UINT32                   Index               +*/
/*+                      : LSA_UINT32                   PortID              +*/
/*+                      : LSA_BOOL                     Ingress             +*/
/*+                      : LSA_BOOL                     Egress              +*/
/*+                      : LSA_BOOL                     isDomainBoundaryAnnounce+*/
/*+                                                                         +*/
/*+  Result              : EDD_RSP                                          +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB             : Pointer to DeviceDescriptionBlock              (in) +*/
/*+  Index            : Index of a MC-Entry                            (in) +*/
/*+  PortID           : Port ID of affected port                       (in) +*/
/*+  Ingress          : Enable ingress filter for this port            (in) +*/
/*+  Egress           : Enable egress filter for this port             (in) +*/
/*+  isDomainBoundaryAnnounce : Special handling for "DomainBoundaryAnnounce"+*/
/*+                                                                         +*/
/*+  Results          : EDD_STS_OK                                          +*/
/*+                   : EDD_STS_ERR_PARAM                                   +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function only enables the filter (DomainBoundary)    +*/
/*                for a specific MC entry (selected via Index) at the given +*/
/*+               port in inbound direction and outbound direction.         +*/
/*+               It does not disable any filter.                           +*/
/*+               Use EDDP_SWI_MC_Entry_Set_To_Default before               +*/
/*+               setting new filters.                                      +*/
/*+                                                                         +*/
/*+               Special handling for "DomainBoundaryAnnounce":            +*/
/*+               -> use only the combination of Egress and Ingress!        +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWI_MC_Entry_Set_DomainBoundary(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  Index,
    LSA_UINT32                  PortID,
    LSA_BOOL                    Ingress,
    LSA_BOOL                    Egress,
    LSA_BOOL                    isDomainBoundaryAnnounce)
{
    EDD_RSP         Response = EDD_STS_OK;
    LSA_UINT32      i;
    LSA_UINT32      HwPortID_forward;
    LSA_UINT32      HwPortID_loop;

	if (Index >= pDDB->SWI.MC_Table_Shadow_Entries_Count)
	{
		// Parameter Index is out of Range
        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_MC_Entry_Set_DomainBoundary(): FATAL -> Index(%d) is not valid!",
                        Index);
		EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
	}

    if ((PortID > 0) && (PortID <= pDDB->SWI.PortCnt))
    {
        HwPortID_forward = EDDP_SWI_get_HwPortID (pDDB, PortID);

        if (isDomainBoundaryAnnounce)
        {
            /* -------------------------------------------- */
            /* DomainBoundary for announce frames           */
            /* Egress and Ingress must be set               */
            /* -------------------------------------------- */
            if ((Egress) && (Ingress))
            {
                /* -------------------- */
                /* Egress filter        */
                /* -------------------- */
                // Egress filter is active      -> set TxPort_API to no send
                pDDB->SWI.pMC_Table_Shadow[Index].TxPort_API = (pDDB->SWI.pMC_Table_Shadow[Index].TxPort_API & ~(0x1 << HwPortID_forward));

                for ( i=1; i <= pDDB->SWI.PortCnt; i++ )
                {
                    HwPortID_loop = EDDP_SWI_get_HwPortID (pDDB, i);

                    // Egress filter is active  -> set TxPort_X to no send
                    switch (HwPortID_loop)
                    {
                        case 1:
                            pDDB->SWI.pMC_Table_Shadow[Index].TxPort_1 = (pDDB->SWI.pMC_Table_Shadow[Index].TxPort_1 & ~(0x1 << HwPortID_forward));
                        break;

                        case 2:
                            pDDB->SWI.pMC_Table_Shadow[Index].TxPort_2 = (pDDB->SWI.pMC_Table_Shadow[Index].TxPort_2 & ~(0x1 << HwPortID_forward));
                        break;

                        default:
                        {
                            // this cannot happen -> wrong implementation!
                            EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_MC_Entry_Set_DomainBoundary(): FATAL -> HWPortID(%d) is not valid!",
                                HwPortID_loop);
                            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                        }
                        break;
                    }
                }

                /* -------------------- */
                /* Ingress filter       */
                /* -------------------- */
                // Ingress filter is active     -> set TxPort_X to '0' (no receive)
                switch (HwPortID_forward)
                {
                    case 1:
                        pDDB->SWI.pMC_Table_Shadow[Index].TxPort_1 = 0x00;    // no receive
                    break;

                    case 2:
                        pDDB->SWI.pMC_Table_Shadow[Index].TxPort_2 = 0x00;    // no receive
                    break;

                    default:
                    {
                        // this cannot happen -> wrong implementation!
                        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_MC_Entry_Set_DomainBoundary(): FATAL -> HWPortID(%d) is not valid!",
                            HwPortID_forward);
                        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                    }
                    break;
                }
            }
        }
        else
        {
            /* -------------------------------------------- */
            /* DomainBoundary for not announce frames       */
            /* Egress or Ingress must be set                */
            /* -------------------------------------------- */

            /* -------------------- */
            /* Egress filter        */
            /* -------------------- */
            if (Egress)
            {
                // Egress filter is active      -> set TxPort_API to no send
                pDDB->SWI.pMC_Table_Shadow[Index].TxPort_API = (pDDB->SWI.pMC_Table_Shadow[Index].TxPort_API & ~(0x1 << HwPortID_forward));

                for ( i=1; i <= pDDB->SWI.PortCnt; i++ )
                {
                    HwPortID_loop = EDDP_SWI_get_HwPortID (pDDB, i);

                    // Egress filter is active  -> set TxPort_X to no send
                    switch (HwPortID_loop)
                    {
                        case 1:
                            pDDB->SWI.pMC_Table_Shadow[Index].TxPort_1 = (pDDB->SWI.pMC_Table_Shadow[Index].TxPort_1 & ~(0x1 << HwPortID_forward));
                        break;

                        case 2:
                            pDDB->SWI.pMC_Table_Shadow[Index].TxPort_2 = (pDDB->SWI.pMC_Table_Shadow[Index].TxPort_2 & ~(0x1 << HwPortID_forward));
                        break;

                        default:
                        {
                            // this cannot happen -> wrong implementation!
                            EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_MC_Entry_Set_DomainBoundary(): FATAL -> HWPortID(%d) is not valid!",
                                HwPortID_loop);
                            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                        }
                        break;
                    }
                }
            }

            /* -------------------- */
            /* Ingress filter       */
            /* -------------------- */
            if (Ingress)
            {
                // Ingress filter is active     -> set TxPort_X to '0' (no receive)
                switch (HwPortID_forward)
                {
                    case 1:
                        pDDB->SWI.pMC_Table_Shadow[Index].TxPort_1 = 0x00;
                    break;

                    case 2:
                        pDDB->SWI.pMC_Table_Shadow[Index].TxPort_2 = 0x00;
                    break;

                    default:
                    {
                        // this cannot happen -> wrong implementation!
                        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_MC_Entry_Set_DomainBoundary(): FATAL -> HWPortID(%d) is not valid!",
                            HwPortID_forward);
                        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                    }
                    break;
                }
            }
        }

        // change MC entry on hardware
        EDDP_SWI_Change_MC_Entry_Parameters (pDDB, &pDDB->SWI.pMC_Table_Shadow[Index], Index);
    }
    else
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWI_MC_Entry_Set_Ingress_and_Egress(): ERROR -> given PortID(%d) is out of range!",
            PortID);
    }

    return (Response);
}

/*****************************************************************************/
/*  end of file eddp_swi_fdb.c                                               */
/*****************************************************************************/
