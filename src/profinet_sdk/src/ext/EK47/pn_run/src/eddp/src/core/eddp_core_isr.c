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
/*  F i l e               &F: eddp_core_isr.c                           :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP interrupt handling                          */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  30.10.08    js    initial version.                                       */
/*  15.07.09    js    changed default EOI times                              */
/*  10.01.11    js    bugfix on disabling interface interrupts. missing &    */
/*  12.01.11    js    bugfix on event handling within eddp_interrupt()       */
/*  13.01.11    am    extra trace info in eddp_interrupt()                   */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID  10                 
#define EDDP_MODULE_ID     LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_CORE_ISR */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"            /* edd headerfiles */
#include "eddp_int.h"            /* internal header */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID) 

/*===========================================================================*/
/*                                defines                                    */
/*===========================================================================*/

/* enables some sanity checks */
#define EDDP_ISR_SANITY_CHECKS 
#undef  EDDP_ISR_USE_HOOKS

/* for use with EDDP_CoreISRGetInternalGroupMask */
#define EDDP_ISR_MSK_LOW  0  
#define EDDP_ISR_MSK_MID  1  
#define EDDP_ISR_MSK_HIGH 2  

/*---------------------------------------------------------------------------*/
/* Defines LOW/MID/HIGH Group interrupt events enabled within Device Setup   */
/* by Interrupthandler (so the user dont needs to enable it)                 */
/*---------------------------------------------------------------------------*/

/* ----------------------------------------------------- */
/*                       ERTEC200P                       */ 
/* ----------------------------------------------------- */

#if defined (EDDP_CFG_HW_ERTEC200P_SUPPORT)
#define EDDP_ISR_GROUP_INTERRUPTS_DEVICE_ENABLE_LOW                     (   \
 ((LSA_UINT32)1 << EDDP_INT_STATUSCHANGE)                               |   \
 ((LSA_UINT32)1 << EDDP_INT_REQDONE_IFA)                                |   \
  /*((LSA_UINT32)0 << EDDP_INT_REQDONE_IFB)                             |*/ \
    ((LSA_UINT32)0 << EDDP_INT_ARCVCMD_DONE)                            |   \
    ((LSA_UINT32)0 << EDDP_INT_ASNDCMD_DONE)                            |   \
    ((LSA_UINT32)0 << EDDP_INT_CPMCMD_DONE)                             |   \
    ((LSA_UINT32)0 << EDDP_INT_PPMCMD_DONE)                             |   \
    ((LSA_UINT32)0 << EDDP_INT_PACKCMD_DONE)                            |   \
    ((LSA_UINT32)0 << EDDP_INT_SYNCCMD_DONE)                            |   \
    ((LSA_UINT32)0 << EDDP_INT_FDBCMD_DONE)                             |   \
    ((LSA_UINT32)0 << EDDP_INT_VLANCMD_DONE)                            |   \
    ((LSA_UINT32)0 << EDDP_INT_FILTERCMD_DONE)                          |   \
 ((LSA_UINT32)1 << EDDP_INT_MAC_ERR)                                    |   \
 ((LSA_UINT32)1 << EDDP_INT_REQQUEUE_ERR)                               |   \
 ((LSA_UINT32)1 << EDDP_INT_CPM_ERR)                                    |   \
 ((LSA_UINT32)1 << EDDP_INT_PPM_ERR)                                    |   \
 ((LSA_UINT32)1 << EDDP_INT_PACK_ERR)                                   |   \
 ((LSA_UINT32)1 << EDDP_INT_IRT_DELAYSTATUS)                            |   \
 ((LSA_UINT32)1 << EDDP_INT_FRAGFAULT)                                  |   \
 ((LSA_UINT32)1 << EDDP_INT_PHASETRANSFERERROR)                         |   \
 ((LSA_UINT32)1 << EDDP_INT_PHASECROSSED)                               |   \
    ((LSA_UINT32)0 << 22)                                               |   \
    ((LSA_UINT32)0 << 23)                                               |   \
 ((LSA_UINT32)1 << EDDP_INT_QCW_EMPTY)                                  |   \
 ((LSA_UINT32)1 << EDDP_INT_HOL_EXPIRED_IFA)                            |   \
 /*((LSA_UINT32)1 << EDDP_INT_HOL_EXPIRED_IFB)                            |*/   \
 ((LSA_UINT32)1 << EDDP_INT_HOL_EXPIRED_PA)                             |   \
 ((LSA_UINT32)1 << EDDP_INT_HOL_EXPIRED_PB)                             |   \
 ((LSA_UINT32)1 << EDDP_INT_FRAMEAGING_EXPIRED)                         |   \
    ((LSA_UINT32)0 << 30)                                               |   \
 ((LSA_UINT32)1 << EDDP_INT_INBOUND_LATE_ERR)                           |   \
 ((LSA_UINT32)0) ) // dummy zero to keep lookout of lines above identical 

/* NOTE: following events are enabled with IF setup         */
/*       see also EDDP_ISR_GROUP_INTERRUPTS_IF_0_ENABLE_MID */
/*       - EDDP_INT_CSCOREBOARD_CHANGED_IFA                 */
/*       - EDDP_INT_PPM_NOT_FINISHED_IFA                    */
/*       - EDDP_INT_PPM_HOL_NOT_EMPTY_IFA                   */
/*       - EDDP_INT_ACYC_SND_IFA                            */
#define EDDP_ISR_GROUP_INTERRUPTS_DEVICE_ENABLE_MID                                     (   \
 ((LSA_UINT32)1 << (EDDP_INT_APIFILTER_ERR              - EDDP_ISR_LOW_MAX_EVENTS))     |   \
    ((LSA_UINT32)0 << ((1+EDDP_ISR_LOW_MAX_EVENTS)      - EDDP_ISR_LOW_MAX_EVENTS))     |   \
    ((LSA_UINT32)0 << (EDDP_INT_ACYC_RCV_IFA            - EDDP_ISR_LOW_MAX_EVENTS))     |   \
    ((LSA_UINT32)0 << (EDDP_INT_ACYC_SND_IFA            - EDDP_ISR_LOW_MAX_EVENTS))     |   \
    /*((LSA_UINT32)0 << (EDDP_INT_ACYC_RCV_IFB            - EDDP_ISR_LOW_MAX_EVENTS))     |*/   \
    /*((LSA_UINT32)0 << (EDDP_INT_ACYC_SND_IFB            - EDDP_ISR_LOW_MAX_EVENTS))     |*/   \
 ((LSA_UINT32)1 << (EDDP_INT_ACYC_RCV_ERR               - EDDP_ISR_LOW_MAX_EVENTS))     |   \
 ((LSA_UINT32)1 << (EDDP_INT_ACYC_SND_ERR               - EDDP_ISR_LOW_MAX_EVENTS))     |   \
    ((LSA_UINT32)0 << ((8+EDDP_ISR_LOW_MAX_EVENTS)      - EDDP_ISR_LOW_MAX_EVENTS))     |   \
    ((LSA_UINT32)0 << (EDDP_INT_CPM_OUTDATASTART        - EDDP_ISR_LOW_MAX_EVENTS))     |   \
    ((LSA_UINT32)0 << (EDDP_INT_CPM_OUTDATAVALID        - EDDP_ISR_LOW_MAX_EVENTS))     |   \
    ((LSA_UINT32)0 << (EDDP_INT_CPM_OUTDATAERR          - EDDP_ISR_LOW_MAX_EVENTS))     |   \
    ((LSA_UINT32)0 << ((12+EDDP_ISR_LOW_MAX_EVENTS)     - EDDP_ISR_LOW_MAX_EVENTS))     |   \
    ((LSA_UINT32)0 << (EDDP_INT_CPM_DMA_ERR             - EDDP_ISR_LOW_MAX_EVENTS))     |   \
    ((LSA_UINT32)0 << (EDDP_INT_CSCOREBOARD_CHANGED_IFA - EDDP_ISR_LOW_MAX_EVENTS))     |   \
    /*((LSA_UINT32)0 << (EDDP_INT_CSCOREBOARD_CHANGED_IFB - EDDP_ISR_LOW_MAX_EVENTS))     |*/   \
    ((LSA_UINT32)0 << (EDDP_INT_PPM_NOT_FINISHED_IFA    - EDDP_ISR_LOW_MAX_EVENTS))     |   \
    /*((LSA_UINT32)0 << (EDDP_INT_PPM_NOT_FINISHED_IFB    - EDDP_ISR_LOW_MAX_EVENTS))     |*/   \
    ((LSA_UINT32)0 << (EDDP_INT_PPM_HOL_NOT_EMPTY_IFA   - EDDP_ISR_LOW_MAX_EVENTS))     |   \
    /*((LSA_UINT32)0 << (EDDP_INT_PPM_HOL_NOT_EMPTY_IFB   - EDDP_ISR_LOW_MAX_EVENTS))     |*/   \
    ((LSA_UINT32)0 << ((20+EDDP_ISR_LOW_MAX_EVENTS)     - EDDP_ISR_LOW_MAX_EVENTS))     |   \
    ((LSA_UINT32)0 << (EDDP_INT_PPM_DMA_ERR             - EDDP_ISR_LOW_MAX_EVENTS))     |   \
    ((LSA_UINT32)0 << ((22+EDDP_ISR_LOW_MAX_EVENTS)     - EDDP_ISR_LOW_MAX_EVENTS))     |   \
    ((LSA_UINT32)0 << ((23+EDDP_ISR_LOW_MAX_EVENTS)     - EDDP_ISR_LOW_MAX_EVENTS))     |   \
    ((LSA_UINT32)0 << (EDDP_HW_EVENT0                   - EDDP_ISR_LOW_MAX_EVENTS))     |   \
    ((LSA_UINT32)0 << (EDDP_HW_EVENT1                   - EDDP_ISR_LOW_MAX_EVENTS))     |   \
    ((LSA_UINT32)0 << (EDDP_HW_EVENT2                   - EDDP_ISR_LOW_MAX_EVENTS))     |   \
    ((LSA_UINT32)0 << (EDDP_HW_EVENT3                   - EDDP_ISR_LOW_MAX_EVENTS))     |   \
 ((LSA_UINT32)1 << (EDDP_SW_EVENT0                      - EDDP_ISR_LOW_MAX_EVENTS))     |   \
    ((LSA_UINT32)0 << (EDDP_SW_EVENT1                   - EDDP_ISR_LOW_MAX_EVENTS))     |   \
    ((LSA_UINT32)0 << (EDDP_SW_EVENT2                   - EDDP_ISR_LOW_MAX_EVENTS))     |   \
    ((LSA_UINT32)0 << (EDDP_SW_EVENT3                   - EDDP_ISR_LOW_MAX_EVENTS))     |   \
    ((LSA_UINT32)0) ) // dummy zero to keep lookout of lines above identical 

 
#define EDDP_ISR_GROUP_INTERRUPTS_DEVICE_ENABLE_HIGH			                                        (   \
    ((LSA_UINT32)0 << (EDDP_INT_PLL_IN_A    - (EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS )))    |   \
    ((LSA_UINT32)0 << (EDDP_INT_PLL_IN_B    - (EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS )))    |   \
    ((LSA_UINT32)0 << (EDDP_INT_PLL_IN_Time - (EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS )))    |   \
    ((LSA_UINT32)0 << (EDDP_INT_PLL0        - (EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS )))    |   \
    ((LSA_UINT32)0 << (EDDP_INT_PLL1        - (EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS )))    |   \
    ((LSA_UINT32)0 << (EDDP_INT_PLL2        - (EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS )))    |   \
    ((LSA_UINT32)0 << (EDDP_INT_PLL3        - (EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS )))    |   \
    ((LSA_UINT32)0 << (EDDP_INT_PLL4        - (EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS )))    |   \
    ((LSA_UINT32)0 << (EDDP_INT_PLL5        - (EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS )))    |   \
    ((LSA_UINT32)0 << (EDDP_INT_PLL6        - (EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS )))    |   \
    ((LSA_UINT32)0 << (EDDP_INT_PLL7        - (EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS )))    |   \
    ((LSA_UINT32)0 << (EDDP_INT_PLL8        - (EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS )))    |   \
    ((LSA_UINT32)0 << (EDDP_INT_PLL9        - (EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS )))    |   \
    ((LSA_UINT32)0 << (13))                                                                             |   \
    ((LSA_UINT32)0 << (14))                                                                             |   \
    ((LSA_UINT32)0 << (EDDP_INT_PTCP_WD     - (EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS )))    |   \
 ((LSA_UINT32)1 << (EDDP_INT_WD              - (EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS )))    |   \
    ((LSA_UINT32)0 << (EDDP_INT_Timer_0     - (EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS )))    |   \
    ((LSA_UINT32)0 << (EDDP_INT_Timer_1     - (EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS )))    |   \
    ((LSA_UINT32)0 << (EDDP_INT_Timer_2     - (EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS )))    |   \
    ((LSA_UINT32)0 << (EDDP_INT_I2C         - (EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS )))    |   \
    ((LSA_UINT32)0 << (21))                                                                             |   \
    ((LSA_UINT32)0) ) // dummy zero to keep lookout of lines above identical 

#endif


/*---------------------------------------------------------------------------*/
/* Defines LOW/MID/HIGH Group interrupt events enabled within Interface      */
/* Setup of IF0 by Interrupthandler (so the user does not need to enable it) */
/* and disabled on shutdown                                                  */
/*---------------------------------------------------------------------------*/

/* ----------------------------------------------------- */
/*                       ERTEC200P                       */ 
/* ----------------------------------------------------- */

#if defined (EDDP_CFG_HW_ERTEC200P_SUPPORT)
#define EDDP_ISR_GROUP_INTERRUPTS_IF_0_ENABLE_LOW   0

#define EDDP_ISR_GROUP_INTERRUPTS_IF_0_ENABLE_MID                                           (   \
 ((LSA_UINT32)1 << (EDDP_INT_CSCOREBOARD_CHANGED_IFA    - EDDP_ISR_LOW_MAX_EVENTS))         |   \
 ((LSA_UINT32)1 << (EDDP_INT_PPM_NOT_FINISHED_IFA       - EDDP_ISR_LOW_MAX_EVENTS))         |   \
 ((LSA_UINT32)1 << (EDDP_INT_PPM_HOL_NOT_EMPTY_IFA      - EDDP_ISR_LOW_MAX_EVENTS))         |   \
 ((LSA_UINT32)1 << (EDDP_INT_ACYC_SND_IFA               - EDDP_ISR_LOW_MAX_EVENTS))         |   \
 ((LSA_UINT32)0) ) // dummy zero to keep lookout of lines above identical 

#define EDDP_ISR_GROUP_INTERRUPTS_IF_0_ENABLE_HIGH  0
#endif

/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* Interrupthandler configuration                                            */
/* Note: With interrupts not used by EDDP use                                */
/*       EDDP_ISR_HANDLER_TYPE_EXTERN,EDDP_ISR_TYPE_GROUP (GROUP dont care)  */
/*       Single Interrupts must be assigned within EDDP_CoreISRDeviceOpen    */
/*       with configured IRQ!                                                */
/* Note: Order must match defines within eddp_isr.h!                         */
/*---------------------------------------------------------------------------*/

#if defined (EDDP_CFG_HW_ERTEC200P_SUPPORT)
static const EDDP_ISR_EVENT_FCT_TABLE_TYPE EDDP_IsrFctTable_Rev12[EDDP_ISR_MAX_EVENTS] = 
{

   /* LOW ======================================*/
   /* PHY polling                               */
   /* 0         INT_StatusChange                */ { EDDP_ISR_HANDLER_TYPE_LOW   , EDDP_ISR_TYPE_GROUP, EDDP_GEN_ISR_LinkChange     },
   /* Commandhandling                           */
   /* 1         INT_ReqDone_IFA                 */ { EDDP_ISR_HANDLER_TYPE_LOW   , EDDP_ISR_TYPE_GROUP, EDDP_CMD_ISR_ReqDone_IFA    },
   /* 2         INT_ReqDone_IFB                 */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 3         INT_ARcvCmd_Done                */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 4         INT_ASndCmd_Done                */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 5         INT_CPMCmd_Done                 */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 6         INT_PPMCmd_Done                 */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 7         INT_PackCmd_Done                */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 8         INT_SYNCCmd_Done                */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 9         INT_FDBCmd_Done                 */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 10        INT_VLANCmd_Done                */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 11        INT_FilterCmd_Done              */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 12        not used                        */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* Errorsignaling                            */
   /* 13        INT_MAC_ERR                     */ { EDDP_ISR_HANDLER_TYPE_LOW,    EDDP_ISR_TYPE_GROUP, EDDP_SWI_ISR_MacErr         },
   /* 14        INT_ReqQueue_ERR                */ { EDDP_ISR_HANDLER_TYPE_LOW,    EDDP_ISR_TYPE_GROUP, EDDP_CMD_ISR_ReqQueue_ERR   },
   /* 15        INT_CPM_ERR                     */ { EDDP_ISR_HANDLER_TYPE_LOW,    EDDP_ISR_TYPE_GROUP, EDDP_CRT_ISR_Cpm_Err        },
   /* 16        INT_PPM_ERR                     */ { EDDP_ISR_HANDLER_TYPE_LOW,    EDDP_ISR_TYPE_GROUP, EDDP_CRT_ISR_Ppm_Err        },
   /* 17        INT_Pack_ERR                    */ { EDDP_ISR_HANDLER_TYPE_LOW,    EDDP_ISR_TYPE_GROUP, EDDP_CRT_ISR_Pack_ERR       },
   /* 18        INT_IRT_DelayStatus             */ { EDDP_ISR_HANDLER_TYPE_LOW,    EDDP_ISR_TYPE_GROUP, EDDP_CRT_ISR_DelayStatus    },
   /* 19        INT_FragFault                   */ { EDDP_ISR_HANDLER_TYPE_LOW,    EDDP_ISR_TYPE_GROUP, EDDP_SWI_ISR_FragmentationFault},
   /* 20        INT_PhaseTransferError          */ { EDDP_ISR_HANDLER_TYPE_LOW,    EDDP_ISR_TYPE_GROUP, EDDP_CRT_ISR_PhaseTransferError},
   /* 21        INT_PhaseCrossed                */ { EDDP_ISR_HANDLER_TYPE_LOW,    EDDP_ISR_TYPE_GROUP, EDDP_CRT_ISR_PhaseCrossed   },
   /* 22        not used                        */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 23        not used                        */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* Ressources                                */
   /* 24        INT_QCW_Empty                   */ { EDDP_ISR_HANDLER_TYPE_LOW,    EDDP_ISR_TYPE_GROUP, EDDP_SWI_ISR_QCW_Empty      },
   /* 25        INT_HOL_Expired_IFA             */ { EDDP_ISR_HANDLER_TYPE_LOW,    EDDP_ISR_TYPE_GROUP, EDDP_SWI_ISR_HOL_Expired_IFA},
   /* 26        INT_HOL_Expired_IFB             */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 27        INT_HOL_Expired_PA              */ { EDDP_ISR_HANDLER_TYPE_LOW,    EDDP_ISR_TYPE_GROUP, EDDP_SWI_ISR_HOL_Expired_PA },
   /* 28        INT_HOL_Expired_PB              */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 29        INT_FrameAging_Expired          */ { EDDP_ISR_HANDLER_TYPE_LOW,    EDDP_ISR_TYPE_GROUP, EDDP_SWI_ISR_FrameAging_Expired},
   /* 30        not used                        */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 31        INT_InBound_Late_ERR            */ { EDDP_ISR_HANDLER_TYPE_LOW,    EDDP_ISR_TYPE_GROUP, EDDP_CRT_ISR_InBound_Late_ERR},
   /* MED ======================================*/
   /* API filter                                */
   /* 0         INT_APIFilter_ERR               */ { EDDP_ISR_HANDLER_TYPE_LOW,    EDDP_ISR_TYPE_GROUP, EDDP_NRT_ISR_APIFilter_ERR  },
   /* 1         not used                        */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* azyclical API                             */
   /* 2         INT_Acyc_Rcv_IFA                */ { EDDP_ISR_HANDLER_TYPE_LOW   , EDDP_ISR_TYPE_SINGLE,EDDP_NRT_ISR_Acyc_Rcv_IFA   },

   /* 3         INT_Acyc_Snd_IFA                */ { EDDP_ISR_HANDLER_TYPE_LOW   , EDDP_ISR_TYPE_GROUP, EDDP_NRT_ISR_Acyc_Snd_IFA   },
   /* 4         INT_Acyc_Rcv_IFB                */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP ,EDDP_CoreISRFatalHandler    },
   /* 5         INT_Acyc_Snd_IFB                */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 6         INT_Acyc_Rcv_ERR                */ { EDDP_ISR_HANDLER_TYPE_LOW,    EDDP_ISR_TYPE_GROUP, EDDP_NRT_ISR_Acyc_Rcv_ERR   },
   /* 7         INT_Acyc_Snd_ERR                */ { EDDP_ISR_HANDLER_TYPE_LOW,    EDDP_ISR_TYPE_GROUP, EDDP_NRT_ISR_Acyc_Snd_ERR   },
   /* 8         not used                        */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* cyclical API                              */
   /* 9         INT_CPM_OutDataStart            */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 10        INT_CPM_OutDataValid            */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 11        INT_CPM_OutDataErr              */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 12        not used                        */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 13        INT_CPM_DMA_Err                 */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 14        INT_CScoreboard_changed_IFA     */ { EDDP_ISR_HANDLER_TYPE_LOW,    EDDP_ISR_TYPE_GROUP, EDDP_CRT_ISR_CSB_changed_IFA},
   /* 15        INT_CScoreboard_changed_IFB     */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 16        INT_PPM_not_finished_IFA        */ { EDDP_ISR_HANDLER_TYPE_LOW,    EDDP_ISR_TYPE_GROUP, EDDP_CRT_ISR_Ppm_not_finished_IFA},
   /* 17        INT_PPM_not_finished_IFB        */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler},
   /* 18        INT_PPM_HOL_not_empty_IFA       */ { EDDP_ISR_HANDLER_TYPE_LOW,    EDDP_ISR_TYPE_GROUP, EDDP_CRT_ISR_Ppm_HOL_not_empty_IFA},
   /* 19        INT_PPM_HOL_not_empty_IFB       */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler},
   /* 20        not used                        */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 21        INT_PPM_DMA_Err                 */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 22        not used                        */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 23        not used                        */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* specificaly interrupt events              */
   /* 24        HW-Event0                       */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 25        HW-Event1                       */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 26        HW-Event2                       */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 27        HW-Event3                       */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 28        SW-Event0                       */ { EDDP_ISR_HANDLER_TYPE_LOW   , EDDP_ISR_TYPE_GROUP, EDDP_K32_ISR_SW_EVENT0      },
   /* 29        SW-Event1                       */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 30        SW-Event2                       */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 31        SW-Event3                       */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* HIGH =====================================*/
   /* PLL                                       */
   /* 0         INT_PLL_IN_A                    */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 1         INT_PLL_IN_B                    */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 2         INT_PLL_IN_Time                 */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 3         INT_PLL0                        */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 4         INT_PLL1                        */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 5         INT_PLL2                        */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 6         INT_PLL3                        */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 7         INT_PLL4                        */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 8         INT_PLL5                        */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 9         INT_PLL6                        */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 10        INT_PLL7                        */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 11        INT_PLL8                        */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 12        INT_PLL9                        */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 13        not used                        */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 14        not used                        */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 15        INT_PTCP_WD                     */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* KRISC-32 Support                          */
   /* 16        INT_WD                          */ { EDDP_ISR_HANDLER_TYPE_LOW,    EDDP_ISR_TYPE_GROUP, EDDP_K32_ISR_WD             },
   /* 17        INT_Timer_0                     */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 18        INT_Timer_1                     */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 19        INT_Timer_2                     */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 20        INT_I2C                         */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
   /* 21        not used                        */ { EDDP_ISR_HANDLER_TYPE_EXTERN, EDDP_ISR_TYPE_GROUP, EDDP_CoreISRFatalHandler    },
};
#endif


/*===========================================================================*/
/*                                global Variables                           */
/*===========================================================================*/


/*===========================================================================*/
/*                                external functions                         */
/*===========================================================================*/

/*===========================================================================*/
/*                                 functions                                 */
/*===========================================================================*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreISRControlInterrupt                +*/
/*+  Input                      EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_BOOL                    Enable          +*/
/*+                             LSA_UINT32                  EventNr         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  pDDB        : Pointer to DeviceDescriptionBlock                   (in) +*/
/*+  Enable      : Enable (LSA_TRUE) or disable (LSA_FALSE) interrupt  (in) +*/
/*+  EventNr     : EventNr (Interrupt) to enable                       (in) +*/
/*+                EDDP_INT_STATUSCHANGE                                    +*/
/*+                EDDP_INT_xxx         etc                                 +*/
/*+                :                                                        +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Enables or disables the Interrupt "EventNr"               +*/
/*+                                                                         +*/
/*+               Can be used for all group interrupts (single interrupts   +*/
/*+               not supported yet)                                        +*/
/*+                                                                         +*/
/*+               We dont check if the interrupt belongs to an active       +*/
/*+               interface!                                                +*/
/*+               Function shall only be called after Interrupthandler was  +*/
/*+               setup and before interrupthandler was shutdown!           +*/
/*+                                                                         +*/
/*+               Note: This function can not be used within                +*/
/*+                     eddp_interrupt() context! Only within LOW context.  +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreISRControlInterrupt(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_BOOL                    Enable,
	LSA_UINT32                  EventNr)
{
	LSA_UINT32                         Msk;
    EDDP_ISR_GROUP_INFO_PTR_TYPE       pGroup;
    EDDP_LOCAL_DDB_CORE_ISR_PTR_TYPE   pIsrInfo;
    EDDP_HAL_ISR_GRP_REGS_TYPE         EventMask;
    EDDP_ISR_EVENT_FCT_TABLE_TYPE const * pIsrFuncTable;

	/* Interupthandler must be active! */
	EDDP_ASSERT(pDDB->Core.Isr.Active);

	pIsrInfo = &pDDB->Core.Isr; 
    pGroup   = &pIsrInfo->Group;
    pIsrFuncTable = pIsrInfo->pIsrFuncTable;

	EventMask.Low  = 0;
	EventMask.Mid  = 0;
	EventMask.High = 0;

	if (EventNr < EDDP_ISR_MAX_EVENTS)
	{
		if (pIsrFuncTable[EventNr].HandlerType == EDDP_ISR_HANDLER_TYPE_EXTERN)
		{
	   	   	EDDP_LOWER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_CoreISRControlInterrupt(): FATAL -> Invalid EventNr! (%d)", EventNr);
		   	EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
		}
		else
		{
			/* GroupInterrupt */
			if (pIsrFuncTable[EventNr].IntType == EDDP_ISR_TYPE_GROUP)
			{
				if (EventNr >= EDDP_ISR_LOW_MAX_EVENTS+EDDP_ISR_MID_MAX_EVENTS) /* High */
				{
					Msk = (LSA_UINT32)1 << (EventNr - (EDDP_ISR_LOW_MAX_EVENTS+EDDP_ISR_MID_MAX_EVENTS));
					EventMask.High |= Msk;
				}
				else 
				{
					if (EventNr >= EDDP_ISR_LOW_MAX_EVENTS) /* Mid */
					{
						Msk = (LSA_UINT32)1 << (EventNr - (EDDP_ISR_LOW_MAX_EVENTS));
						EventMask.Mid |= Msk;
					}
					else /* Low */
					{
						Msk = (LSA_UINT32)1 << EventNr;
						EventMask.Low |= Msk;
					}
				}

				/* Protection because IntISREnabledEventMsk is modified within ISR */
			    EDDP_ENTER_INT_S();

                /* Modify UserEnabled-Masks (1=enabled,0=disabled) */
                if (Enable)
                {
				    pGroup->IntUserEnabledEventMsk.Low   |= EventMask.Low;
				    pGroup->IntUserEnabledEventMsk.Mid	 |=	EventMask.Mid;	
				    pGroup->IntUserEnabledEventMsk.High  |= EventMask.High;
                }
                else
                {
				    pGroup->IntUserEnabledEventMsk.Low   &= ~EventMask.Low;
				    pGroup->IntUserEnabledEventMsk.Mid	 &=	~EventMask.Mid;	
				    pGroup->IntUserEnabledEventMsk.High  &= ~EventMask.High;
                }
                
			    /* Ack all interrupt if currently pending to avoid old interrupts */
                /* in interrupts are enabled.                                     */

				EDDP_HALIsrAckGroupIRQ (pDDB, &EventMask);

				/* Determine group interrupts enabled and not masked by Interrupthandler */
				/* Note: Hardware MSK Register needs 0 for enabled!                      */

				EventMask.Low  = ~ (pGroup->IntUserEnabledEventMsk.Low  & pGroup->IntISREnabledEventMsk.Low );
				EventMask.Mid  = ~ (pGroup->IntUserEnabledEventMsk.Mid  & pGroup->IntISREnabledEventMsk.Mid );
				EventMask.High = ~ (pGroup->IntUserEnabledEventMsk.High & pGroup->IntISREnabledEventMsk.High);

			    /* Setup mask registers for group interrupts */
				EDDP_HALIsrSetGroupMsk (pDDB, &EventMask);

			    EDDP_EXIT_INT_S();
			}
			else
			{
			    EDDP_LOWER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_CoreISRControlInterrupt(): FATAL -> Invalid EventNr(%d)!", EventNr);
       		    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
			}
		}

	}
	else
	{
   	   	EDDP_LOWER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_CoreISRControlInterrupt(): FATAL -> Invalid EventNr! (%d)", EventNr);
       	EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
	}
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreISREnableDeviceInterrupts          +*/
/*+  Input                      EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  pDDB        : Pointer to DeviceDescriptionBlock                   (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Enables/unmaskes all Device global interrupts not enabled +*/
/*+               by individual user handling. Will be called on DeviceSetup+*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreISREnableDeviceInterrupts(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDDP_ISR_GROUP_INFO_PTR_TYPE   pGroup;
    EDDP_HAL_ISR_GRP_REGS_TYPE     EventMask;

    pGroup = &pDDB->Core.Isr.Group;

    /* ----------------------------------------------------------------------*/
    /* Enable all Group Interrupts which will not be enabled extern by user  */
    /* ----------------------------------------------------------------------*/

	/* Protection because IntISREnabledEventMsk is modified within ISR */
    EDDP_ENTER_INT_S();

    /* determine revision depending interrupt event mask */
    switch (EDDP_HALGetPnipRevision(pDDB))
    {
        #if defined (EDDP_CFG_HW_ERTEC200P_SUPPORT)
        case EDDP_HAL_HW_PNIP_REV1:
        case EDDP_HAL_HW_PNIP_REV2:
        {
            pGroup->IntUserEnabledEventMsk.Low  |= EDDP_ISR_GROUP_INTERRUPTS_DEVICE_ENABLE_LOW;
            pGroup->IntUserEnabledEventMsk.Mid  |= EDDP_ISR_GROUP_INTERRUPTS_DEVICE_ENABLE_MID;
            pGroup->IntUserEnabledEventMsk.High |= EDDP_ISR_GROUP_INTERRUPTS_DEVICE_ENABLE_HIGH;
        }
        break;
        #endif
            
        default:
        {
            EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, 
                "[H:--] EDDP_CoreISREnableDeviceInterrupts(): FATAL -> Version(%d) of PNIP is unknown",     
                EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }  

    EDDP_LOWER_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"[H:--] EDDP_CoreISREnableDeviceInterrupts(): New: (L=0x%X, M=0x%X, H=0x%X)",
		pGroup->IntUserEnabledEventMsk.Low, pGroup->IntUserEnabledEventMsk.Mid, pGroup->IntUserEnabledEventMsk.High);

    /* Determine group interrupts enabled and not masked */
	/* Note: Hardware MSK Register needs 0 for enabled!  */
    EventMask.Low  = ~ (pGroup->IntUserEnabledEventMsk.Low  & pGroup->IntISREnabledEventMsk.Low );
    EventMask.Mid  = ~ (pGroup->IntUserEnabledEventMsk.Mid  & pGroup->IntISREnabledEventMsk.Mid );
    EventMask.High = ~ (pGroup->IntUserEnabledEventMsk.High & pGroup->IntISREnabledEventMsk.High);

    EDDP_LOWER_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"[H:--] EDDP_CoreISREnableDeviceInterrupts(): Sum: (L=0x%X, M=0x%X, H=0x%X)",
		EventMask.Low, EventMask.Mid, EventMask.High);

     /* Setup mask registers for group interrupts */
    EDDP_HALIsrSetGroupMsk (pDDB, &EventMask);

    EDDP_EXIT_INT_S();

}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreISRDisableDeviceInterrupts         +*/
/*+  Input                      EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  pDDB        : Pointer to DeviceDescriptionBlock                   (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Disables/maskes all Group interrupts                      +*/
/*+               Will be called on Device Shutdown                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreISRDisableDeviceInterrupts(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDDP_ISR_GROUP_INFO_PTR_TYPE   pGroup;
    EDDP_HAL_ISR_GRP_REGS_TYPE     EventMask;

    pGroup = &pDDB->Core.Isr.Group;

    /* ----------------------------------------------------------------------*/
    /* Disable all Group Interrupts.                                         */
	/* (we disable all here to be sure)                                      */
    /* ----------------------------------------------------------------------*/

	/* Protection because IntISREnabledEventMsk is modified within ISR */
    EDDP_ENTER_INT_S();

    pGroup->IntUserEnabledEventMsk.Low  = 0;
    pGroup->IntUserEnabledEventMsk.Mid  = 0;
    pGroup->IntUserEnabledEventMsk.High = 0;

    /* Determine group interrupts enabled and not temporary masked */
	/* Note: Hardware MSK Register needs 0 for enabled!            */
    EventMask.Low  = ~ (pGroup->IntUserEnabledEventMsk.Low  & pGroup->IntISREnabledEventMsk.Low );
    EventMask.Mid  = ~ (pGroup->IntUserEnabledEventMsk.Mid  & pGroup->IntISREnabledEventMsk.Mid );
    EventMask.High = ~ (pGroup->IntUserEnabledEventMsk.High & pGroup->IntISREnabledEventMsk.High);

    EDDP_LOWER_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"[H:--] EDDP_CoreISRDisableDeviceInterrupts(): L=0x%X, M=0x%X, H=0x%X",
		EventMask.Low, EventMask.Mid, EventMask.High);

     /* Setup mask registers for group interrupts */
    EDDP_HALIsrSetGroupMsk (pDDB, &EventMask);

    EDDP_EXIT_INT_S();

    /* ----------------------------------------------------------------------*/
    /* Disable Single Interrupts                                             */
    /* ----------------------------------------------------------------------*/

	/* Nothing yet. -> Rx already disabled on Interface Shutdown */
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreISREnableDeviceIFInterrupts        +*/
/*+  Input                      EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  pDDB        : Pointer to DeviceDescriptionBlock                   (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Enables/unmaskes all Interface interrupts not enabled     +*/
/*+               by individual user handling. Will be called on            +*/
/*+               Interface Setup                                           +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreISREnableDeviceIFInterrupts(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDDP_ISR_GROUP_INFO_PTR_TYPE       pGroup;
    EDDP_LOCAL_DDB_CORE_ISR_PTR_TYPE   pIsrInfo;
    EDDP_HAL_ISR_GRP_REGS_TYPE         EventMask;

	pIsrInfo = &pDDB->Core.Isr; 
    pGroup   = &pIsrInfo->Group;

    EventMask.Low  = 0;
    EventMask.Mid  = 0;
    EventMask.High = 0;

    /* determine revision depending interrupt event mask */
    switch (EDDP_HALGetPnipRevision(pDDB))
    {
        #if defined (EDDP_CFG_HW_ERTEC200P_SUPPORT)
        case EDDP_HAL_HW_PNIP_REV1:
        case EDDP_HAL_HW_PNIP_REV2:
        {
            EventMask.Low  = EDDP_ISR_GROUP_INTERRUPTS_IF_0_ENABLE_LOW;
            EventMask.Mid  = EDDP_ISR_GROUP_INTERRUPTS_IF_0_ENABLE_MID;
            EventMask.High = EDDP_ISR_GROUP_INTERRUPTS_IF_0_ENABLE_HIGH;
        }
        break;
        #endif
            
        default:
        {
            EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, 
                "[H:--] EDDP_CoreISREnableDeviceIFInterrupts(): FATAL -> Version(%d) of PNIP is unknown",     
                EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }  
    	
    /* ----------------------------------------------------------------------*/
    /* Enable all Group Interrupts which will not be enabled extern by user  */
    /* ----------------------------------------------------------------------*/

	/* Protection because IntISREnabledEventMsk is modified within ISR */
    EDDP_ENTER_INT_S();

    pGroup->IntUserEnabledEventMsk.Low  |= EventMask.Low;
    pGroup->IntUserEnabledEventMsk.Mid  |= EventMask.Mid;
    pGroup->IntUserEnabledEventMsk.High |= EventMask.High;

    EDDP_LOWER_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"[H:--] EDDP_CoreISREnableDeviceIFInterrupts(): New: (L=0x%X, M=0x%X, H=0x%X)",
        EventMask.Low, EventMask.Mid, EventMask.High);

    /* Determine group interrupts enabled and not masked */
	/* Note: Hardware MSK Register needs 0 for enabled!  */
    EventMask.Low  = ~ (pGroup->IntUserEnabledEventMsk.Low  & pGroup->IntISREnabledEventMsk.Low );
    EventMask.Mid  = ~ (pGroup->IntUserEnabledEventMsk.Mid  & pGroup->IntISREnabledEventMsk.Mid );
    EventMask.High = ~ (pGroup->IntUserEnabledEventMsk.High & pGroup->IntISREnabledEventMsk.High);

    EDDP_LOWER_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"[H:--] EDDP_CoreISREnableDeviceIFInterrupts(): Sum: (L=0x%X, M=0x%X, H=0x%X)",
		EventMask.Low, EventMask.Mid, EventMask.High);

     /* Setup mask registers for group interrupts */
    EDDP_HALIsrSetGroupMsk (pDDB, &EventMask);

    EDDP_EXIT_INT_S();

    /* ----------------------------------------------------------------------*/
	/* Enable Single Interrupt for acyclical recv                            */
    /* ----------------------------------------------------------------------*/
	EDDP_ASSERT_FALSE(pDDB->Core.Isr.IRQAcycRcv > EDDP_ISR_MAX_SINGLE_INT)

	EDDP_HALIsrEnableMUXIRQ (pDDB, pDDB->Core.Isr.IRQAcycRcv, pIsrInfo->Single[pDDB->Core.Isr.IRQAcycRcv].EventNr, pDDB->Core.Isr.IRQ_WaitTime_AcycRcv);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreISRDisableDeviceIFInterrupts       +*/
/*+  Input                      EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  pDDB        : Pointer to DeviceDescriptionBlock                   (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Disables/maskes all Interface specific interrupts         +*/
/*+               Will be called on Interface Shutdown                      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreISRDisableDeviceIFInterrupts(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{

    EDDP_ISR_GROUP_INFO_PTR_TYPE       pGroup;
    EDDP_LOCAL_DDB_CORE_ISR_PTR_TYPE   pIsrInfo;
    EDDP_HAL_ISR_GRP_REGS_TYPE         EventMask;

	pIsrInfo = &pDDB->Core.Isr; 
    pGroup   = &pIsrInfo->Group;

    EventMask.Low  = 0;
    EventMask.Mid  = 0;
    EventMask.High = 0;

    /* determine revision depending interrupt event mask */
    switch (EDDP_HALGetPnipRevision(pDDB))
    {
        #if defined (EDDP_CFG_HW_ERTEC200P_SUPPORT)
        case EDDP_HAL_HW_PNIP_REV1:
        case EDDP_HAL_HW_PNIP_REV2:
        {
            EventMask.Low  = EDDP_ISR_GROUP_INTERRUPTS_IF_0_ENABLE_LOW;
            EventMask.Mid  = EDDP_ISR_GROUP_INTERRUPTS_IF_0_ENABLE_MID;
            EventMask.High = EDDP_ISR_GROUP_INTERRUPTS_IF_0_ENABLE_HIGH;
        }
        break;
        #endif
            
        default:
        {
            EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, 
                "[H:--] EDDP_CoreISRDisableDeviceIFInterrupts(): FATAL -> Version(%d) of PNIP is unknown",     
                EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }     
    
    /* ----------------------------------------------------------------------*/
    /* Disable all Group Interrupts of Interface                             */
    /* ----------------------------------------------------------------------*/

	/* Protection because IntISREnabledEventMsk is modified within ISR */
    EDDP_ENTER_INT_S();

    pGroup->IntUserEnabledEventMsk.Low  &= ~EventMask.Low;
    pGroup->IntUserEnabledEventMsk.Mid  &= ~EventMask.Mid;
    pGroup->IntUserEnabledEventMsk.High &= ~EventMask.High;

    /* Determine group interrupts enabled and not masked */
	/* Note: Hardware MSK Register needs 0 for enabled!  */
    EventMask.Low  = ~ (pGroup->IntUserEnabledEventMsk.Low  & pGroup->IntISREnabledEventMsk.Low );
    EventMask.Mid  = ~ (pGroup->IntUserEnabledEventMsk.Mid  & pGroup->IntISREnabledEventMsk.Mid );
    EventMask.High = ~ (pGroup->IntUserEnabledEventMsk.High & pGroup->IntISREnabledEventMsk.High);

    EDDP_LOWER_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"[H:--] EDDP_CoreISRDisableDeviceIFInterrupts(): L=0x%X, M=0x%X, H=0x%X",
		EventMask.Low, EventMask.Mid, EventMask.High);

     /* Setup mask registers for group interrupts */
    EDDP_HALIsrSetGroupMsk (pDDB, &EventMask);

    EDDP_EXIT_INT_S();

    /* ----------------------------------------------------------------------*/
    /* Disable Single Interrupts                                             */
    /* ----------------------------------------------------------------------*/
	EDDP_ASSERT_FALSE(pDDB->Core.Isr.IRQAcycRcv > EDDP_ISR_MAX_SINGLE_INT)

	EDDP_HALIsrDisableMUXIRQ (pDDB, pDDB->Core.Isr.IRQAcycRcv);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreISRGetRQBInUseCnt                  +*/
/*+  Input                      EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  pDDB        : Pointer to DeviceDescriptionBlock                   (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Returns the number of RQBs "inUse"                        +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_CoreISRGetRQBInUseCnt(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDDP_LOCAL_DDB_CORE_ISR_PTR_TYPE   pIsrInfo;
    LSA_UINT32                         Cnt = 0;
    LSA_UINT32                         i;

    pIsrInfo = &pDDB->Core.Isr;

    if (pIsrInfo->Group.IntRQB.InUse)
    {
        Cnt++;
    }

    for ( i=0; i< EDDP_ISR_SINGLE_INFO_CNT; i++)
    {
        if (pIsrInfo->Single[i].IntRQB.InUse)
        {
            Cnt++;
        }
    }

    return(Cnt);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreISRFatalHandler                    +*/
/*+  Input                      EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+                             LSA_UINT32                       EventNr    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  pDDB        : Pointer to DeviceDescriptionBlock                   (in) +*/
/*+  EventNr     : EventNr (Interrupt)                                 (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Handler for fatal Interrupts (interrupts which shall not  +*/
/*+               occur!)                                                   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreISRFatalHandler(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT32                  EventNr)
{
    LSA_UNUSED_ARG(pDDB);
#if (EDDP_CFG_TRACE_MODE ==  0)
    LSA_UNUSED_ARG(EventNr);
#endif
   	EDDP_LOWER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_CoreISRFatalHandler(): FATAL -> Unexpected interrupt! (EventNr: %d)", EventNr);
   	EDDP_FATAL(EDDP_FATAL_ERR_UNEXP_INTERRUPT);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreISRTestHandler                     +*/
/*+  Input                      EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+                             LSA_UINT32                       EventNr    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  pDDB        : Pointer to DeviceDescriptionBlock                   (in) +*/
/*+  EventNr     : EventNr (Interrupt)                                 (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Handler for simple Interrupts test (indication test)      +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreISRTestHandler(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT32                  EventNr)
{
    LSA_UNUSED_ARG(pDDB);
#if (EDDP_CFG_TRACE_MODE ==  0)
    LSA_UNUSED_ARG(EventNr);
#endif
   	EDDP_LOWER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN,"[H:--] EDDP_CoreISRTestHandler(): Indication for EventNr: %d", EventNr);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreISRGroupEventCallHandler           +*/
/*+  Input                      EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+                             LSA_UINT32                       HandlerType+*/
/*+                             EDDP_HAL_ISR_GRP_REGS_PTR_TYPE   pEvents    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  pDDB        : Pointer to DeviceDescriptionBlock                   (in) +*/
/*+  HandlerType : Priority the Broaker is called from                      +*/
/*+                                                                         +*/
/*+               EDDP_ISR_HANDLER_TYPE_LOW    : Called from LOW-Handler    +*/
/*+               EDDP_ISR_HANDLER_TYPE_DIRECT : Called from ISR Context    +*/
/*+                                                                         +*/
/*+  pEvents     : Low/Mid/High-Bitmask of events                   (in)    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Called from INT-Handler for GroupInterrupts to call       +*/
/*+               the user interrupthandler.                                +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreISRGroupEventCallHandler(
	EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
	LSA_UINT32                         HandlerType,
    EDDP_HAL_ISR_GRP_REGS_PTR_TYPE     pEvents)
{
    EDDP_LOCAL_DDB_CORE_ISR_PTR_TYPE   pIsrInfo;
    LSA_UINT32                         Idx;
    LSA_UINT32                         Events;
    EDDP_ISR_EVENT_FCT_TABLE_TYPE const * pIsrFuncTable;

	EDDP_LOWER_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CoreISRGroupEventCallHandler(HandlerType=0x%X, EventLow=0x%X, EventMid=0x%X, EventHigh=0x%X)",
        HandlerType, pEvents->Low, pEvents->Mid, pEvents->High);

    pIsrInfo = &pDDB->Core.Isr;
    pIsrFuncTable = pIsrInfo->pIsrFuncTable;

    /*-----------------------------------------------------------------------*/
    /* Low-Events                                                            */
    /*-----------------------------------------------------------------------*/

    Idx = 0;

    /* Mask out events not enabled */
    Events = pEvents->Low & pIsrInfo->Group.IntUserEnabledEventMsk.Low;

    while (Events)                        
    {
        /* Only if Match and Event is enabled */
        if (Events & 1)
        {
            #ifdef EDDP_ISR_SANITY_CHECKS
            if (pIsrFuncTable[Idx].HandlerType != HandlerType)
            {
        	   	EDDP_LOWER_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_CoreISRGroupEventCallHandler(): FATAL -> Invalid HandlerType! EDDP_IsrFctTable[%d].HandlerType(0x%X) != HandlerType(0x%X)", 
                    Idx, pIsrFuncTable[Idx].HandlerType, HandlerType);
            	EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
            }
            /* Call SingleInterruptHandler */
            EDDP_ASSERT_NO_NULL_FCT_PTR(pIsrFuncTable[Idx].ISREventHandler);
            #endif
            pIsrFuncTable[Idx].ISREventHandler(pDDB,Idx);
        }
        Events >>= 1;
        Idx++;
    }

    /*-----------------------------------------------------------------------*/
    /* Mid-Events                                                            */
    /*-----------------------------------------------------------------------*/

    Idx = EDDP_ISR_LOW_MAX_EVENTS;

    /* Mask out events not enabled */
    Events = pEvents->Mid & pIsrInfo->Group.IntUserEnabledEventMsk.Mid;

    while (Events)                        
    {
        if (Events & 1)
        {
            #ifdef EDDP_ISR_SANITY_CHECKS
            if (pIsrFuncTable[Idx].HandlerType != HandlerType)
            {
        	   	EDDP_LOWER_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_CoreISRGroupEventCallHandler(): FATAL -> Invalid HandlerType! EDDP_IsrFctTable[%d].HandlerType(0x%X) != HandlerType(0x%X)", 
                    Idx, pIsrFuncTable[Idx].HandlerType, HandlerType);
            	EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
            }
            /* Call SingleInterruptHandler */
            EDDP_ASSERT_NO_NULL_FCT_PTR(pIsrFuncTable[Idx].ISREventHandler);
            #endif
            pIsrFuncTable[Idx].ISREventHandler(pDDB,Idx);
        }
        Events >>= 1;
        Idx++;
    }

    /*-----------------------------------------------------------------------*/
    /* High-Events                                                           */
    /*-----------------------------------------------------------------------*/

    Idx = EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS;

    /* Mask out events not enabled */
    Events  = pEvents->High  & pIsrInfo->Group.IntUserEnabledEventMsk.High;
    Events &= EDDP_ISR_HIGH_MAX_MSK;

    while (Events)                        
    {
        if (Events & 1)
        {
            #ifdef EDDP_ISR_SANITY_CHECKS
            if (Idx >= EDDP_ISR_MAX_EVENTS)
            {
        	   	EDDP_LOWER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_CoreISRGroupEventCallHandler(): FATAL -> Invalid Idx! Idx=%d", Idx);
            	EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
            }
            if (pIsrFuncTable[Idx].HandlerType != HandlerType)
            {
        	   	EDDP_LOWER_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_CoreISRGroupEventCallHandler(): FATAL -> Invalid HandlerType! EDDP_IsrFctTable[%d].HandlerType(0x%X) != HandlerType(0x%X)", 
                    Idx, pIsrFuncTable[Idx].HandlerType, HandlerType);
            	EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
            }
            /* Call SingleInterruptHandler */
            EDDP_ASSERT_NO_NULL_FCT_PTR(pIsrFuncTable[Idx].ISREventHandler);
            #endif
            pIsrFuncTable[Idx].ISREventHandler(pDDB,Idx);
        }
        Events >>= 1;
        Idx++;
    }

	EDDP_LOWER_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CoreISRGroupEventCallHandler()");
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreISRHandler_GroupLOW                +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+                             EDDP_HAL_ISR_GRP_REGS_PTR_TYPE   pEvents    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB        : Pointer to DeviceDescriptionBlock                   (in) +*/
/*+  pEvents     : Low/Mid/High-Bitmask of events pending for LOW      (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This is the ISR-Handler function called to handle Group   +*/
/*+               Interrupts which have to be forwareded to LOW-Prio with   +*/
/*+               internal RQB. Calls EDDP_DO_INTERRUPT() macro.            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreISRHandler_GroupLOW(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    EDDP_HAL_ISR_GRP_REGS_PTR_TYPE     pEvents)
{
    EDDP_ISR_GROUP_INFO_PTR_TYPE    pGroup;
    EDDP_HAL_ISR_GRP_REGS_TYPE      EventMask;

    EDDP_LOWER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CoreISRHandler_GroupLOW(pEvents=0x%X)", pEvents);

    pGroup = &pDDB->Core.Isr.Group;

    /*-----------------------------------------------------------------------*/
    /* First we mask all group interrupts handled within LOW context         */
    /*-----------------------------------------------------------------------*/
    /* Disable masks for group interrupts handled LOW */
	/* 1 = enabled, 0 = disabled */
    pGroup->IntISREnabledEventMsk.Low  &= ~pGroup->Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_LOW].Msk.Low; 
    pGroup->IntISREnabledEventMsk.Mid  &= ~pGroup->Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_LOW].Msk.Mid; 
    pGroup->IntISREnabledEventMsk.High &= ~pGroup->Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_LOW].Msk.High; 

    /* Determine group interrupts enabled and not masked */
	/* Note: Hardware MSK Register needs 0 for enabled!  */
    EventMask.Low  = ~(pGroup->IntUserEnabledEventMsk.Low  & pGroup->IntISREnabledEventMsk.Low );
    EventMask.Mid  = ~(pGroup->IntUserEnabledEventMsk.Mid  & pGroup->IntISREnabledEventMsk.Mid );
    EventMask.High = ~(pGroup->IntUserEnabledEventMsk.High & pGroup->IntISREnabledEventMsk.High);

    /* Setup mask registers for group interrupts */
    EDDP_HALIsrSetGroupMsk (pDDB, &EventMask);

    /*-----------------------------------------------------------------------*/
    /* Now we send the RQB to LOW context via Output macro                   */
    /*-----------------------------------------------------------------------*/
    /* only if RQB currently not in use. Maybe in use if an event was pending*/
    /* but dont care because we will handle it with the RQB already underway */

    if (!(pGroup->IntRQB.InUse))
    {
        #ifdef EDDP_ISR_SANITY_CHECKS
        EDDP_ASSERT_NO_NULL_PTR(pGroup->IntRQB.pRQB);
        #endif
        pGroup->IntRQB.InUse = LSA_TRUE; /* RQB is underway.. */
    	EDDP_DO_INTERRUPT_REQUEST(pDDB->hSysDev, pGroup->IntRQB.pRQB);
    }
    else
    {
        EDDP_LOWER_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CoreISRHandler_GroupLOW(): RQB inUse or shutdown!");
    }

    LSA_UNUSED_ARG(pEvents);

    EDDP_LOWER_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CoreISRHandler_GroupLOW()");
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreISRHandler_GroupDirect             +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+                             EDDP_HAL_ISR_GRP_REGS_PTR_TYPE   pEvents    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  pEvents    : Low/Mid/High-Bitmask of events handled directly     (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This is the ISR-Handler function called to handle Group   +*/
/*+               Interrupts which have to be executed directly within ISR  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreISRHandler_GroupDirect(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    EDDP_HAL_ISR_GRP_REGS_PTR_TYPE     pEvents)
{

    EDDP_LOWER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] IN: EDDP_CoreISRHandler_GroupDirect(pEvents=0x%X)", pEvents);

    /* Ack all Group interrupts handled directly */
    EDDP_HALIsrAckGroupIRQ (pDDB, pEvents);

    EDDP_CoreISRGroupEventCallHandler (pDDB, EDDP_ISR_HANDLER_TYPE_DIRECT, pEvents);

    EDDP_LOWER_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] OUT: EDDP_CoreISRHandler_GroupDirect()");
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreISRHandler_GroupUnexp              +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+                             EDDP_HAL_ISR_GRP_REGS_PTR_TYPE   pEvents    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  pEvents    : Low/Mid/High-Bitmask of events handled directly     (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This is the ISR-Handler function called to handle Group   +*/
/*+               Interrupts which are not expected by the Interrupthandler!+*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreISRHandler_GroupUnexp(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    EDDP_HAL_ISR_GRP_REGS_PTR_TYPE     pEvents)
{
    LSA_UNUSED_ARG(pDDB);
#if (EDDP_CFG_TRACE_MODE ==  0)
    LSA_UNUSED_ARG(pEvents);
#endif
   	EDDP_LOWER_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_CoreISRHandler_GroupUnexp(): FATAL -> Unexpected Group Interrupt (L=0x%X, M=0x%X, H=0x%X)!",
		pEvents->Low, pEvents->Mid, pEvents->High);

	EDDP_FATAL(EDDP_FATAL_ERR_UNEXP_INTERRUPT);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreISRHandler_SingleLOW               +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32                     IntSource    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  IntSource  : Source Of Interrupt: 2..15 (only SingleInterrupts)        +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This is the ISR-Handler function called to handle single  +*/
/*+               Interrupts which have to be forwarded to LOW-Prio with    +*/
/*+               internal RQB. Calls EDDP_DO_INTERRUPT() macro.            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreISRHandler_SingleLOW(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  IntSource)
{
    EDDP_ISR_SINGLE_INFO_PTR_TYPE   pSingle;

    EDDP_LOWER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] IN: EDDP_CoreISRHandler_SingleLOW(IntSource=0x%X)", IntSource);

    #ifdef EDDP_ISR_SANITY_CHECKS
    EDDP_ASSERT_FALSE(IntSource >= EDDP_ISR_SINGLE_INFO_CNT);
    #endif

    pSingle = &pDDB->Core.Isr.Single[IntSource];

    #if defined  (EDDP_CFG_ISR_POLLING_MODE)    
    {
        //Only send DO RQB if interrupt is pending
        LSA_UINT32 IsrValue = EDDP_HAL_REG32_READ(pDDB, pSingle->IsrRegOffset);

        if (0 == (IsrValue & pSingle->IsrRegMask) )
        {
            EDDP_LOWER_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW,"[H:--] OUT: EDDP_CoreISRHandler_SingleLOW(): Single IRQ not pending!");
            return;
        }
    }
    #endif //(EDDP_CFG_ISR_POLLING_MODE)    

    /* forward RQB only if not already in use  */
    if ( ! pSingle->IntRQB.InUse ) 
    {
        #ifdef EDDP_ISR_SANITY_CHECKS
        EDDP_ASSERT_NO_NULL_PTR(pSingle->IntRQB.pRQB);
        #endif
        pSingle->IntRQB.InUse = LSA_TRUE; /* RQB is underway.. */

        #if defined  (EDDP_CFG_ISR_POLLING_MODE)    
        //Acknowledge interrupt. Ack is needed to detect further pending ints.
        EDDP_HAL_REG32_WRITE(pDDB, pSingle->AckRegOffset, pSingle->AckRegValue);
        #endif //(EDDP_CFG_ISR_POLLING_MODE)    

        EDDP_DO_INTERRUPT_REQUEST(pDDB->hSysDev,pSingle->IntRQB.pRQB);  
    }
    else
    {
        EDDP_LOWER_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW,"[H:--] EDDP_CoreISRHandler_SingleLOW(): RQB in use or shutdown!");
    }

    EDDP_LOWER_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] OUT: EDDP_CoreISRHandler_SingleLOW()");
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreISRHandler_SingleDirect            +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32                     IntSource    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  IntSource  : Source Of Interrupt: 2..15 (only SingleInterrupts)        +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This is the ISR-Handler function called to handle single  +*/
/*+               Interrupts which have to be executed directly within ISR  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreISRHandler_SingleDirect(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  IntSource)
{
    EDDP_LOCAL_DDB_CORE_ISR_PTR_TYPE   pIsrInfo;
    LSA_UINT32                         Idx;
    EDDP_ISR_EVENT_FCT_TABLE_TYPE const * pIsrFuncTable;

    EDDP_LOWER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] IN: EDDP_CoreISRHandler_SingleDirect(IntSource=0x%X)", IntSource);

    pIsrInfo = &pDDB->Core.Isr;
    pIsrFuncTable = pIsrInfo->pIsrFuncTable;

    Idx = pIsrInfo->Single[IntSource].EventNr;

    #ifdef EDDP_ISR_SANITY_CHECKS
    if (Idx >= EDDP_ISR_MAX_EVENTS)
    {
   	    EDDP_LOWER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_CoreISRHandler_SingleDirect(): FATAL -> Invalid Idx! (Idx=%d)", Idx);
   	    EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }

    if (pIsrFuncTable[Idx].HandlerType != EDDP_ISR_HANDLER_TYPE_DIRECT )
    {
   	    EDDP_LOWER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_CoreISRHandler_SingleDirect(): FATAL -> Invalid HandlerType! (Idx=%d)", Idx);
   	    EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }
    EDDP_ASSERT_NO_NULL_FCT_PTR(pIsrFuncTable[Idx].ISREventHandler);
    #endif

    #if defined  (EDDP_CFG_ISR_POLLING_MODE)    
    {
        //Only call handler if interrupt is pending
        LSA_UINT32 IsrValue = EDDP_HAL_REG32_READ(pDDB, pIsrInfo->Single[IntSource].IsrRegOffset);

        if (0 == (IsrValue & pIsrInfo->Single[IntSource].IsrRegMask) )
        {
            EDDP_LOWER_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW,"[H:--] EDDP_CoreISRHandler_SingleDirect(): Single IRQ not pending!");
            return;
        }
        else
        {
            //Acknowledge interrupt. Ack is needed to detect further pending ints.
            EDDP_HAL_REG32_WRITE(pDDB, pIsrInfo->Single[IntSource].AckRegOffset, pIsrInfo->Single[IntSource].AckRegValue);
        }
    }
    #endif //(EDDP_CFG_ISR_POLLING_MODE)    

    /* Call SingleInterruptHandler */
    pIsrFuncTable[Idx].ISREventHandler(pDDB,Idx);

    EDDP_LOWER_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] OUT: EDDP_CoreISRHandler_SingleDirect()");
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreISRHandler_SingleUnexp             +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32                     IntSource    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  IntSource  : Source Of Interrupt: 2..15 (only SingleInterrupts)        +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This is the ISR-Handler function called to handle single  +*/
/*+               Interrupts which are not expected.                        +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreISRHandler_SingleUnexp(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  IntSource)
{
    LSA_UNUSED_ARG(pDDB);
#if (EDDP_CFG_TRACE_MODE ==  0)
    LSA_UNUSED_ARG(IntSource);
#endif
   	EDDP_LOWER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_CoreISRHandler_SingleUnexp(): FATAL -> Unexpected Single Interrupt Source(%d)!",
        IntSource);

	EDDP_FATAL(EDDP_FATAL_ERR_UNEXP_INTERRUPT);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreISRFreeInternalRQBs                +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function frees the internal RQBs used by ISR         +*/
/*+               No RQB must be "InUse"! (e.g. within Systemadpation)      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreISRFreeInternalRQBs(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT32                         i;
    EDDP_LOCAL_DDB_CORE_ISR_PTR_TYPE   pIsrInfo;

    EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] IN: EDDP_CoreISRFreeInternalRQBs()");

    pIsrInfo = &pDDB->Core.Isr;

    /* ----------------------------------------------------------------------*/
    /* Free Group RQB                                                        */
    /* ----------------------------------------------------------------------*/
    if (pIsrInfo->Group.IntRQB.InUse)
    {
       	EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_CoreISRFreeInternalRQBs(): FATAL -> RQB of group still in use!! cannot free!");
      	EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }

    /* free if not LSA_NULL (LSA_NULL if not used/allocated) */
    if ( !(LSA_HOST_PTR_ARE_EQUAL(pIsrInfo->Group.IntRQB.pRQB,LSA_NULL)) )
    {
        EDDP_GlbFreeInternalRQB(pIsrInfo->Group.IntRQB.pRQB);
        pIsrInfo->Group.IntRQB.pRQB = LSA_NULL;
    }

    /* ----------------------------------------------------------------------*/
    /* Free Single RQB                                                       */
    /* ----------------------------------------------------------------------*/
    i = 0;

    while (i < EDDP_ISR_SINGLE_INFO_CNT)
    {
        if (pIsrInfo->Single[i].IntRQB.InUse)
        {
           	EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_CoreISRFreeInternalRQBs(): FATAL -> RQB %d still in use!! cannot free!", i);
        	EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
        }

        /* free if no LSA_NULL (LSA_NULL if not used/allocated) */
        if ( ! (LSA_HOST_PTR_ARE_EQUAL(pIsrInfo->Single[i].IntRQB.pRQB,LSA_NULL)) )
        {
            EDDP_GlbFreeInternalRQB(pIsrInfo->Single[i].IntRQB.pRQB);
            pIsrInfo->Single[i].IntRQB.pRQB = LSA_NULL;
        }
        i++;
    }

    EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] OUT: EDDP_CoreISRFreeInternalRQBs()");

}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreISRAllocInternalRQBs               +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function allocates the internal RQBs used by ISR     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreISRAllocInternalRQBs(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP                            Response;
    EDD_UPPER_RQB_PTR_TYPE             pRQB;
    LSA_UINT32                         i;
    EDDP_LOCAL_DDB_CORE_ISR_PTR_TYPE   pIsrInfo;

    EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] IN: EDDP_CoreISRAllocInternalRQBs()");

    Response = EDD_STS_OK;
    pIsrInfo = &pDDB->Core.Isr;

    /* ----------------------------------------------------------------------*/
    /* Allocate Group-RQBs                                                   */
    /* ----------------------------------------------------------------------*/
    pRQB = EDDP_GlbAllocInternalRQB();

    if (LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL))
    {
   	    Response = EDD_STS_ERR_RESOURCE;
        EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CoreISRAllocInternalRQBs(): ERROR -> Allocation RQB memory failed.");
    }
    else
    {
   		/* setup RQB */
        EDDP_GlbSetupInternalRQB(pRQB,
                                 EDDP_SRV_DEV_INTERRUPT,
                                 0, /* handle dont care */
                                 pDDB->hDDB,
                                 //0, /* hIDB dont care */
                                 EDDP_ISR_RQB_USER_ID_GROUP, 
                                 0);/* param not used for Group */
          
        pIsrInfo->Group.IntRQB.InUse = LSA_FALSE;
        pIsrInfo->Group.IntRQB.pRQB  = pRQB;    
    }

    /* ----------------------------------------------------------------------*/
    /* Allocate Single-RQBs                                                  */
    /* ----------------------------------------------------------------------*/
    i = 0;

    while ((Response == EDD_STS_OK ) && (i<EDDP_ISR_SINGLE_INFO_CNT))
    {
        /* Only if SingleInterrupt used */
        if (pIsrInfo->Single[i].EventNr != EDDP_EVENT_NR_UNUSED)
        {
            pRQB = EDDP_GlbAllocInternalRQB();
    
            if (LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL))
            {
        	    Response = EDD_STS_ERR_RESOURCE;
                EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
                EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CoreISRAllocInternalRQBs(): ERROR -> Allocation RQB memory failed.");
            }
            else
            {
        		/* setup RQB */
                EDDP_GlbSetupInternalRQB(pRQB,
                                         EDDP_SRV_DEV_INTERRUPT,
                                         0, /* handle dont care */
                                         pDDB->hDDB,
                                         // 0, /* hIDB dont care */
                                         EDDP_ISR_RQB_USER_ID_SINGLE, /* ID = index */
                                         i);/* param is single-interrupt Nr 2..15 */
            
                pIsrInfo->Single[i].IntRQB.InUse = LSA_FALSE;
                pIsrInfo->Single[i].IntRQB.pRQB  = pRQB;    
            }
        }
        else
        {
            pIsrInfo->Single[i].IntRQB.InUse = LSA_FALSE;         
            pIsrInfo->Single[i].IntRQB.pRQB  = LSA_NULL;            
        }
        i++;
    }

    if (Response != EDD_STS_OK)
    {
        EDDP_CoreISRFreeInternalRQBs (pDDB);
    }

    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] OUT: EDDP_CoreISRAllocInternalRQBs(Response=0x%X)", Response);

    return(Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreISRGetInternalGroupMask            +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_CORE_ISR_PTR_TYPE  pIsrInfo  +*/
/*+                             LSA_UINT32                        Idx       +*/
/*+                             LSA_UINT32  EDDP_LOCAL_MEM_ATTR * pMskLOW   +*/
/*+                             LSA_UINT32  EDDP_LOCAL_MEM_ATTR * pMskDirect+*/
/*+                             LSA_UINT32  EDDP_LOCAL_MEM_ATTR * pMskUnexp +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pIsrInfo   : Pointer to ISR management within DDB                (in)  +*/
/*+  Idx        : EDD_ISR_MSK_LOW                                     (in)  +*/
/*+               EDD_ISR_MSK_MID                                           +*/
/*+               EDD_ISR_MSK_HIGH                                          +*/
/*+                                                                         +*/
/*+ *pMskLOW    : Mask of interrupt events for idx for RQB handling   (out) +*/
/*+ *pMskDirect : Mask of interrupt events for idx for direct call    (out) +*/
/*+ *pMskUnexp  : Mask of interrupt events for idx for unexpected call(out) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Determines the Eventmasks to be used for Group interrupt  +*/
/*+               handling.                                                 +*/
/*+                                                                         +*/
/*+               -> GroupInterrupts forwarded by RQB to LOW context        +*/
/*+               -> GroupInterrupts called directly within ISR             +*/
/*+               -> GroupInterrupts unexpected by interrupthandler         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreISRGetInternalGroupMask(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    LSA_UINT32                         Idx,
    LSA_UINT32  EDDP_LOCAL_MEM_ATTR  * pMskLOW,
    LSA_UINT32  EDDP_LOCAL_MEM_ATTR  * pMskDirect,
    LSA_UINT32  EDDP_LOCAL_MEM_ATTR  * pMskUnexp)
{
    LSA_UINT32                         i,Offset,Cnt = 0;
    LSA_UINT32                         Msk,MskLOW,MskDirect;
    EDDP_LOCAL_DDB_CORE_ISR_PTR_TYPE   pIsrInfo;
    EDDP_ISR_EVENT_FCT_TABLE_TYPE const * pIsrFuncTable = LSA_NULL;

    pIsrInfo = &pDDB->Core.Isr;
    pIsrFuncTable = pIsrInfo->pIsrFuncTable;
    
    /* select fct-table to scan */
    switch (Idx)
    {
        case EDDP_ISR_MSK_LOW:
            Offset = 0;
            pIsrFuncTable = &pIsrFuncTable[Offset];
            Cnt  = EDDP_ISR_LOW_MAX_EVENTS;
            break;
        case EDDP_ISR_MSK_MID:
            Offset = EDDP_ISR_LOW_MAX_EVENTS;
            pIsrFuncTable = &pIsrFuncTable[Offset];
            Cnt = EDDP_ISR_MID_MAX_EVENTS;
            break;
        case EDDP_ISR_MSK_HIGH:
            Offset = EDDP_ISR_LOW_MAX_EVENTS + EDDP_ISR_MID_MAX_EVENTS;
            pIsrFuncTable = &pIsrFuncTable[Offset];
            Cnt = EDDP_ISR_HIGH_MAX_EVENTS;
            break;
        default:
	   	   	EDDP_LOWER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_CoreISRGetInternalGroupMask(): FATAL -> Invalid Idx! (%d)",
                Idx);
		   	EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
            break;
    }

    MskLOW    = 0;
    MskDirect = 0;
    Msk       = 1;

    /* build up bitmasks */
    for ( i=0; i < Cnt; i++ )
    {
        /* Only Group-Interrupts */
        if (pIsrFuncTable[i].IntType == EDDP_ISR_TYPE_GROUP)
        {
            /* check which handlertype (LOW/Direct) */
            switch (pIsrFuncTable[i].HandlerType)
            {
                case EDDP_ISR_HANDLER_TYPE_LOW:
                   MskLOW    |= Msk;
                   break;
                case EDDP_ISR_HANDLER_TYPE_DIRECT:
                   MskDirect |= Msk;
                   break;
                case EDDP_ISR_HANDLER_TYPE_EXTERN:
                default:
                   break;
            }
        }

        Msk <<= 1;
    }

    *pMskLOW    = MskLOW;
    *pMskDirect = MskDirect;
	*pMskUnexp  = ~(MskLOW | MskDirect);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreISRSetupSingleIRQ                  +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                        :    LSA_UINT32                     IRQNr        +*/
/*+                             LSA_UINT32                     EventNr      +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : pointer to DDB                                      (in)  +*/
/*+  IRQNr      : IRQ-Nummer EDDP_ISR_MIN_SINGLE_INT..                (in)  +*/
/*+                       .. EDDP_ISR_MAX_SINGLE_INT                        +*/
/*+  EventNr    : InterruptEvent-Nr. 0.. <EDDP_ISR_MAX_EVENTS         (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_RESOURCE                                      +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Setup the single-interrupthandler to the servicehandler   +*/
/*+               If IRQ already used we set a RESOURCE error               +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreISRSetupSingleIRQ(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT32                  IRQNr,
	LSA_UINT32                  EventNr)
{
    EDDP_LOCAL_DDB_CORE_ISR_PTR_TYPE   pIsrInfo;
	EDD_RSP                            Response;
	EDDP_ISR_EVENT_FCT_TABLE_TYPE const * pIsrFuncTable;

	Response = EDD_STS_OK;
    pIsrInfo = &pDDB->Core.Isr;
    pIsrFuncTable = pIsrInfo->pIsrFuncTable;

	/* check for valid IRQNr range */
	if (   (IRQNr >= EDDP_ISR_MIN_SINGLE_INT)
        && (IRQNr < EDDP_ISR_MAX_SINGLE_INT)
       )
	{
		/* check if IRQ not already used */
		if (pIsrInfo->Single[IRQNr].EventNr == EDDP_EVENT_NR_UNUSED)
		{
			EDDP_ASSERT_FALSE(EventNr >= EDDP_ISR_MAX_EVENTS);
            EDDP_ASSERT_FALSE(pIsrFuncTable[EventNr].IntType != EDDP_ISR_TYPE_SINGLE);

			pIsrInfo->Single[IRQNr].EventNr = EventNr;

#if defined  (EDDP_CFG_ISR_POLLING_MODE)    
            {
                //Set values for faster check and acknowledge
                LSA_UINT32 Msk;

                EDDP_HALIsrGetSingleOffsets(pDDB, EventNr, &pIsrInfo->Single[IRQNr].AckRegOffset, &pIsrInfo->Single[IRQNr].IsrRegOffset, &Msk);
                pIsrInfo->Single[IRQNr].AckRegValue = Msk;
                pIsrInfo->Single[IRQNr].IsrRegMask = Msk;
            }
#endif
            
			/* Set the right Handler for the Interrupt depending on IntType */
			switch (pIsrFuncTable[EventNr].HandlerType)
            {
				case EDDP_ISR_HANDLER_TYPE_DIRECT:
				{
					pIsrInfo->Single[IRQNr].SingleHandler = EDDP_CoreISRHandler_SingleDirect;
				}
				break;

				case EDDP_ISR_HANDLER_TYPE_LOW:
				{
					pIsrInfo->Single[IRQNr].SingleHandler = EDDP_CoreISRHandler_SingleLOW;
				}
				break;
	
                default:
				{
  	   				EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_CoreISRSetupSingleIRQ(): FATAL -> Invalid HandlerType(0x%X) within EDDP_IsrFctTable for Idx %d",
  	   				    pIsrFuncTable[EventNr].HandlerType, EventNr);
                    EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
				}
				break;
			}
		}
		else
		{
			Response = EDD_STS_ERR_RESOURCE;
			EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
			EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] IN: EDDP_CoreISRSetupSingleIRQ(): ERROR -> IRQAcycRcv (%d) already used DPB for IF", IRQNr); 
		}
	}
	else
	{
		Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
	    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] IN: EDDP_CoreISRSetupSingleIRQ(): ERROR -> Invalid IRQAcycRcv (%d) within DPB for IF", IRQNr); 
	}

	return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreISRDeviceOpen                      +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             EDDP_UPPER_DPB_PTR_TYPE        pDPB         +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDD_SRV_DEV_OPEN                                          +*/
/*+               Initializes all ISR management structures within DDB      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_EXTERN EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreISRDeviceOpen(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_UPPER_DPB_PTR_TYPE     pDPB)
{
    EDD_RSP                            Response = EDD_STS_OK;
    EDDP_LOCAL_DDB_CORE_ISR_PTR_TYPE   pIsrInfo;
    LSA_UINT32                         i;

#if (EDDP_CFG_TRACE_MODE ==  0)
    LSA_UNUSED_ARG(pDPB);
#endif

    EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] IN: EDDP_CoreISRDeviceOpen(pDDB=0x%X, pDPB=0x%X)", pDDB, pDPB);

    pIsrInfo = &pDDB->Core.Isr;

    /* Note: DDB init with 0 by Core ! */

    /* ----------------------------------------------------------------------*/
    /* Setup some variables                                                  */
    /* ----------------------------------------------------------------------*/
    pIsrInfo->Active          = LSA_FALSE;
    pIsrInfo->ShutdownPending = LSA_FALSE;

    /* ----------------------------------------------------------------------*/
    /* Setup revision dependent interrupt function table                     */
    /* NOTE: interrupt function table must be determined first here          */
    /* ----------------------------------------------------------------------*/    
    
    /* determine ISR function table depending on revision */
    switch (EDDP_HALGetPnipRevision(pDDB))
    {
        #if defined (EDDP_CFG_HW_ERTEC200P_SUPPORT)
        case EDDP_HAL_HW_PNIP_REV1:
        case EDDP_HAL_HW_PNIP_REV2:
        {
            pIsrInfo->pIsrFuncTable = EDDP_IsrFctTable_Rev12;
        }
        break;
        #endif
        default:
        {
            EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, 
                "[H:--] EDDP_CoreISRDeviceOpen(): FATAL -> Version(%d) of PNIP is unknown",     
                EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }    
    
    /* ----------------------------------------------------------------------*/
    /* No interrupt enabled on startup by default                            */
    /* ----------------------------------------------------------------------*/
    pIsrInfo->Group.IntUserEnabledEventMsk.Low  = 0;
    pIsrInfo->Group.IntUserEnabledEventMsk.Mid  = 0;
    pIsrInfo->Group.IntUserEnabledEventMsk.High = 0;

    /* ----------------------------------------------------------------------*/
    /* All interrupts enabled by ISR handler on Startup by default           */
    /* ----------------------------------------------------------------------*/
    pIsrInfo->Group.IntISREnabledEventMsk.Low  = 0xFFFFFFFFl;
    pIsrInfo->Group.IntISREnabledEventMsk.Mid  = 0xFFFFFFFFl;
    pIsrInfo->Group.IntISREnabledEventMsk.High = 0xFFFFFFFFl;

    /* ----------------------------------------------------------------------*/
    /* Setup GroupInfo Masks for Group interrupts                            */
    /* ----------------------------------------------------------------------*/
    EDDP_CoreISRGetInternalGroupMask(
        pDDB,
        EDDP_ISR_MSK_LOW,
        &pIsrInfo->Group.Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_LOW].Msk.Low,
        &pIsrInfo->Group.Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_DIRECT].Msk.Low, 
        &pIsrInfo->Group.Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_UNEXP].Msk.Low);

    EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW,"[H:--] EDDP_CoreISRDeviceOpen(): LowMask-Low=0x%X, LowMask-Direct=0x%X, LowMask-Unexp=0x%X", 
        pIsrInfo->Group.Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_LOW].Msk.Low,
        pIsrInfo->Group.Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_DIRECT].Msk.Low,
        pIsrInfo->Group.Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_UNEXP].Msk.Low);

    EDDP_CoreISRGetInternalGroupMask(
        pDDB,
        EDDP_ISR_MSK_MID,
        &pIsrInfo->Group.Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_LOW].Msk.Mid,
        &pIsrInfo->Group.Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_DIRECT].Msk.Mid,
        &pIsrInfo->Group.Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_UNEXP].Msk.Mid);

    EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW,"[H:--] EDDP_CoreISRDeviceOpen(): MidMask-Low=0x%X, MidMask-Direct=0x%X, MidMask-Unexp=0x%X", 
        pIsrInfo->Group.Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_LOW].Msk.Mid,
        pIsrInfo->Group.Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_DIRECT].Msk.Mid, 
        pIsrInfo->Group.Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_UNEXP].Msk.Mid);

    EDDP_CoreISRGetInternalGroupMask(
        pDDB,
        EDDP_ISR_MSK_HIGH,
        &pIsrInfo->Group.Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_LOW].Msk.High,
        &pIsrInfo->Group.Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_DIRECT].Msk.High,
        &pIsrInfo->Group.Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_UNEXP].Msk.High);

    EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW,"[H:--] EDDP_CoreISRDeviceOpen(): HighMask-Low=0x%X, HighMask-Direct=0x%X, HighMask-Unexp=0x%X", 
        pIsrInfo->Group.Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_LOW].Msk.High,
        pIsrInfo->Group.Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_DIRECT].Msk.High,
        pIsrInfo->Group.Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_UNEXP].Msk.High);

    /* ----------------------------------------------------------------------*/
    /* Setup GroupInfo handler functions                                     */
    /* ----------------------------------------------------------------------*/

    pIsrInfo->Group.Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_LOW].GroupHandler    = EDDP_CoreISRHandler_GroupLOW;
    pIsrInfo->Group.Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_DIRECT].GroupHandler = EDDP_CoreISRHandler_GroupDirect;
    pIsrInfo->Group.Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_UNEXP].GroupHandler  = EDDP_CoreISRHandler_GroupUnexp;
    
    /* ----------------------------------------------------------------------*/
    /* Setup Single Interrupt handler functions                              */
    /* ----------------------------------------------------------------------*/

    /* Default: assume no single interrupt used internally */
    for ( i=0; i < EDDP_ISR_SINGLE_INFO_CNT; i++ )
    {
        pIsrInfo->Single[i].EventNr       = EDDP_EVENT_NR_UNUSED;  /* Idx into EDDP_IsrFctTable */
        pIsrInfo->Single[i].SingleHandler = EDDP_CoreISRHandler_SingleUnexp;
    }

	/* Setup management for all Single-Interrupts we use */
	/* Rx-Ifa */
	Response = EDDP_CoreISRSetupSingleIRQ (pDDB, pDPB->HWCfg.IRQAcycRcv, EDDP_INT_ACYC_RCV_IFA);

	if (Response == EDD_STS_OK)
	{
		/* save for later use */
		pDDB->Core.Isr.IRQAcycRcv = pDPB->HWCfg.IRQAcycRcv;
	}

	/* ----------------------------------------------------------------------*/
	/* Allocate internal RQBs for each Interrupt Source                      */
	/* ----------------------------------------------------------------------*/
	if (Response == EDD_STS_OK)
	{
	    EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW,"[H:--] EDDP_CoreISRDeviceOpen(): Allocating internal RQBs"); 
	    Response = EDDP_CoreISRAllocInternalRQBs(pDDB);
	}

    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] OUT: EDDP_CoreISRDeviceOpen(Response=0x%X)", Response);

    return(Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreISRDeviceClose                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDD_SRV_DEV_CLOSE                                         +*/
/*+               Frees all ISR management structures within DDB            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_EXTERN EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreISRDeviceClose(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP     Response = EDD_STS_OK;

    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] IN: EDDP_CoreISRDeviceClose(pDDB=0x%X)", pDDB);
    EDDP_CoreISRFreeInternalRQBs (pDDB);
    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] OUT: EDDP_CoreISRDeviceClose(Response=0x%X)", Response);

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreISRDeviceSetup                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             EDDP_UPPER_DSB_PTR_TYPE        pDSB         +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  pDSB       : Pointer to DeviceSetupBlock                         (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDD_SRV_DEV_SETUP                                         +*/
/*+               Initializes ISR management on setup device.               +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_EXTERN EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreISRDeviceSetup(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_UPPER_DSB_PTR_TYPE     pDSB)
{
    EDD_RSP     Response = EDD_STS_OK;

#if (EDDP_CFG_TRACE_MODE ==  0)
    LSA_UNUSED_ARG(pDSB);
#endif

    EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] IN: EDDP_CoreISRDeviceSetup(pDDB=0x%X, pDSB=0x%X)", pDDB, pDSB);

    if ( (EDDP_ISR_MAX_EOI_WAITTIME < pDSB->InterruptHandling.EOI_WaitTime ) || (EDDP_ISR_MIN_EOI_WAITTIME > pDSB->InterruptHandling.EOI_WaitTime) )
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CoreISRDeviceSetup(): ERROR -> EOI_WaitTime(0x%x) not in Range 0x%x..0x%x!", 
            pDSB->InterruptHandling.EOI_WaitTime, EDDP_ISR_MIN_EOI_WAITTIME, EDDP_ISR_MAX_EOI_WAITTIME);
    }
    else
    {
        /* ----------------------------------------------------------------------*/
        /* Copy EOI_WaitTime from DSB to DDB                                     */
        /* ----------------------------------------------------------------------*/
        pDDB->Core.Hal.Isr.EOI_WaitTime = pDSB->InterruptHandling.EOI_WaitTime;

        /* ----------------------------------------------------------------------*/
        /* Setup HW ISR registers to defaults (disabled)                         */
        /* ----------------------------------------------------------------------*/
        EDDP_HALIsrInit(pDDB);

        /* ----------------------------------------------------------------------*/
        /* Activate Interrupthandling                                            */
        /* ----------------------------------------------------------------------*/
        pDDB->Core.Isr.Active = LSA_TRUE;

        /* ----------------------------------------------------------------------*/
        /* Enable Device gloabal interrupts not enabled by user.                 */
        /* ----------------------------------------------------------------------*/
	    EDDP_CoreISREnableDeviceInterrupts(pDDB);
    }

    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] OUT: EDDP_CoreISRDeviceSetup(Response=0x%X)", Response);
    
    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreISRDeviceShutdown                  +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE           pDDB      +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_OK_PENDING                                        +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDDP_SRV_DEV_SHUTDOWN or EDDP_SRV_DEV_SETUP on error      +*/
/*+               Shutdown the ISR managment of Device.                     +*/
/*+                                                                         +*/
/*+               If some timer RQBs still in use (e.g. because the shutdown+*/
/*+               comes in before the RQB from the DO-Macro) we have to wait+*/
/*+               for all RQBs to be free -> PENDING.                       +*/
/*+                                                                         +*/
/*+               Note that all Interrupts must be disabled by application  +*/
/*+               if this function is called! So it must be called after the+*/
/*+               shutdown of the other functions!                          +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_EXTERN EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreISRDeviceShutdown(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP                            Response = EDD_STS_OK;
    EDDP_LOCAL_DDB_CORE_ISR_PTR_TYPE   pIsrInfo;
    EDDP_HAL_ISR_GRP_REGS_TYPE         Events;

    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] IN: EDDP_CoreISRDeviceShutdown(pDDB=0x%X)", pDDB);
    
    pIsrInfo = &pDDB->Core.Isr;

    /* ---------------------------------------------------------------------*/
    /* Disable all (remaining) interrupts. Interface Interrupts shall       */
    /* already be disabled on interface shutdown.                           */
    /* ---------------------------------------------------------------------*/
	EDDP_CoreISRDisableDeviceInterrupts(pDDB);

    /* ----------------------------------------------------------------------*/
    /* ACK all pending Events (maybe set because of EOI?)                    */
    /* ----------------------------------------------------------------------*/
	Events.Low  = 0xFFFFFFFFl;
	Events.Mid  = 0xFFFFFFFFl;
	Events.High = 0xFFFFFFFFl;

	EDDP_HALIsrAckGroupIRQ (pDDB, &Events);

    /* ---------------------------------------------------------------------*/
    /* stop interrupthandling within eddp_interrupt()                       */
    /* ---------------------------------------------------------------------*/
    pDDB->Core.Isr.Active = LSA_FALSE;

    /* ---------------------------------------------------------------------*/
    /* if any RQB still in use (within systemadaption) we must wait..       */
    /* ---------------------------------------------------------------------*/
    if (EDDP_CoreISRGetRQBInUseCnt(pDDB))
    {
        pIsrInfo->ShutdownPending = LSA_TRUE;

        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"[H:--] EDDP_CoreISRDeviceShutdown(): RQB underway. Delay shutdown!");
        Response = EDD_STS_OK_PENDING;
    }
    else
    {
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW,"[H:--] EDDP_CoreISRDeviceShutdown(): NO RQB underway. Shutting down successful!");
    }

    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] OUT: EDDP_CoreISRDeviceShutdown(Response=0x%X)", Response);
    
    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreISRDeviceIFSetup                   +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE           pDDB      +*/
/*+                             EDDP_UPPER_DSB_PTR_TYPE           pDSB      +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  pDSB       : Pointer to DeviceSetupBlock                         (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDD_SRV_DEV_SETUP                                         +*/
/*+               Initializes ISR management on interface setup.            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_EXTERN EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreISRDeviceIFSetup(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_UPPER_DSB_PTR_TYPE     pDSB)
{
	EDD_RSP  Response;

#if (EDDP_CFG_TRACE_MODE ==  0)
    LSA_UNUSED_ARG(pDSB);
#endif

    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] IN: EDDP_CoreISRDeviceIFSetup(pDSB=0x%X)", pDSB);

	Response = EDD_STS_OK;

    if (pDSB->InterruptHandling.IRQ_WaitTime_AcycRcv > EDDP_ISR_MAX_IRQ_WAITTIME)
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CoreISRDeviceIFSetup(): ERROR -> IRQ_WaitTime(0x%x) > 0x%x!", 
            pDSB->InterruptHandling.IRQ_WaitTime_AcycRcv, EDDP_ISR_MAX_IRQ_WAITTIME);
    }
    else
    {
        /* ----------------------------------------------------------------------*/
        /* Copy IRQ_WaitTime from DSB to DDB                                     */
        /* ----------------------------------------------------------------------*/
        pDDB->Core.Isr.IRQ_WaitTime_AcycRcv = pDSB->InterruptHandling.IRQ_WaitTime_AcycRcv;

        /* ----------------------------------------------------------------------*/
        /* Enable Interface interrupts not enabled by user.                      */
        /* ----------------------------------------------------------------------*/
	    EDDP_CoreISREnableDeviceIFInterrupts (pDDB);
    }

    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] OUT: EDDP_CoreISRDeviceIFSetup(Response=0x%X)", Response);
    
    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreISRDeviceIFShutdown                +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE           pDDB      +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDDP_SRV_DEV_IF_SHUTDOWN or EDDP_SRV_DEV_IF_SETUP (only if+*/
/*+               previous setup shall be aborted)                          +*/
/*+               Shutdown Interface of XXX                                 +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_EXTERN EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreISRDeviceIFShutdown(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
	EDD_RSP  Response;

    EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] IN: EDDP_CoreISRDeviceIFShutdown()");

	Response = EDD_STS_OK;

    /* ----------------------------------------------------------------------*/
    /* Disables Interface interrupts not disabled by user.                   */
    /* ----------------------------------------------------------------------*/
	EDDP_CoreISRDisableDeviceIFInterrupts (pDDB);

    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] OUT: EDDP_CoreISRDeviceIFShutdown(Response=0x%X)", Response);
    
    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreISRRequest                         +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE     pRQB             +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         Opcode:     EDD_OPC_SYSTEM                  +*/
/*+     LSA_HANDLE_TYPE         Handle:     not used                        +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user (not used)           +*/
/*+     EDD_SERVICE             Service:    EDDP_SRV_DEV_INTERRUPT          +*/
/*+     EDD_RSP                 Status:     not used                        +*/
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:     EDDP_UPPER_DEV_INTERNAL_PTR_TYPE+*/
/*+                                                                         +*/
/*+  pParam points to EDDP_RQB_DEV_INTERNAL_TYPE                            +*/
/*+                                                                         +*/
/*+     hDDB     : Device Handle                                            +*/
/*+     ID       : spezifies the interrupt type:                            +*/
/*+                 EDDP_ISR_RQB_USER_ID_GROUP                              +*/
/*+                 EDDP_ISR_RQB_USER_ID_SINGLE                             +*/
/*+     Param    : with SINGLE: Interrupt nr 2..15                          +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Request initiated from eddp_interrupt() for further LOW   +*/
/*+               prio handling of interrupt events.                        +*/
/*+                                                                         +*/
/*+               Triggered with EDDP_DO_INTERRUPT()                        +*/
/*+                                                                         +*/
/*+               Handles GROUP and SINGLE Interrupts within LOW context    +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_EXTERN LSA_VOID  EDDP_LOCAL_FCT_ATTR EDDP_CoreISRRequest(
    EDD_UPPER_RQB_PTR_TYPE      pRQB)
{
    EDD_RSP                            Response;
    EDDP_UPPER_DEV_INTERNAL_PTR_TYPE   pRqbParam;
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB;
    
    EDDP_LOWER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CoreISRRequest(pRQB=0x%X)", pRQB);

    Response = EDD_STS_OK;

    if (LSA_HOST_PTR_ARE_EQUAL(pRQB->pParam, LSA_NULL))
    {
        Response = EDD_STS_ERR_PARAM;
	    EDDP_CORE_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CoreISRRequest(): ERROR -> RQB->pParam is NULL!");
	    EDD_RQB_SET_RESPONSE(pRQB, EDD_STS_ERR_PARAM);
	    EDDP_RQB_ERROR(pRQB);
    }
    else
    {
        pRqbParam = (EDDP_UPPER_DEV_INTERNAL_PTR_TYPE) pRQB->pParam;

        EDDP_LOWER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CoreISRRequest(): Interrupt Event! (UserId=%d, Param=%d)",
             pRqbParam->UserID, pRqbParam->Param);

        if (EDDP_CoreDevGetDDB(pRqbParam->hDDB,&pDDB) != EDD_STS_OK)
        {
    	   	EDDP_LOWER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CoreISRRequest(): FATAL -> Invalid hDDB (0x%X).",
                pRqbParam->hDDB);
	    	EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
        }

        switch (pRqbParam->UserID) 
        {
            /*---------------------------------------------------------------*/
            case EDDP_ISR_RQB_USER_ID_GROUP:
            /*---------------------------------------------------------------*/
            {
                EDDP_ISR_GROUP_INFO_PTR_TYPE  pGroup;
                EDDP_HAL_ISR_GRP_REGS_TYPE    Events;

                pGroup = &pDDB->Core.Isr.Group;

                /* RQB can now be used again..*/
                pGroup->IntRQB.InUse = LSA_FALSE;

                #ifdef EDDP_ISR_SANITY_CHECKS
                EDDP_ASSERT(LSA_HOST_PTR_ARE_EQUAL(pGroup->IntRQB.pRQB,pRQB));
                #endif

                /* ------------------------------------------------------*/
                /* Get Pending IRQ Events                                */
                /* Note: Group Interrupts handled LOW masked out so the  */
                /*       IRQ Register is empty! We read the EVENT        */
                /*       Register instead                                */
                /* ------------------------------------------------------*/
                EDDP_HALIsrGetEvents(pDDB,&Events);

                /* Mask out so only LOW events which are enabled will remain */
                Events.Low  &= pGroup->Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_LOW].Msk.Low  & pGroup->IntUserEnabledEventMsk.Low;
                Events.Mid  &= pGroup->Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_LOW].Msk.Mid  & pGroup->IntUserEnabledEventMsk.Mid;
                Events.High &= pGroup->Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_LOW].Msk.High & pGroup->IntUserEnabledEventMsk.High;

                /* ------------------------------------------------------*/
                /* Ack all Group interrupts handled LOW and enabled      */
                /* ------------------------------------------------------*/
                EDDP_HALIsrAckGroupIRQ(pDDB,&Events);

                /* ------------------------------------------------------*/
                /* call Handler for all LOW events                       */
                /* ------------------------------------------------------*/
                EDDP_CoreISRGroupEventCallHandler(pDDB, EDDP_ISR_HANDLER_TYPE_LOW, &Events);

                /* ------------------------------------------------------*/
                /* UnMsk Group interrupts handled within LOW context     */
                /* ------------------------------------------------------*/
                EDDP_ENTER_INT_S();

                /* Setup masks for group interrupts */
				/* Msk = 1 means enabled!           */
                pGroup->IntISREnabledEventMsk.Low  |= pGroup->Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_LOW].Msk.Low; 
                pGroup->IntISREnabledEventMsk.Mid  |= pGroup->Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_LOW].Msk.Mid; 
                pGroup->IntISREnabledEventMsk.High |= pGroup->Events[EDDP_ISR_GROUP_EVENT_IDX_INTERN_LOW].Msk.High; 

                /* Determine group interrupts masked.               */
				/* Note: Hardware MSK Register needs 0 for enabled! */

                Events.Low  = ~ (pGroup->IntUserEnabledEventMsk.Low  & pGroup->IntISREnabledEventMsk.Low );
                Events.Mid  = ~ (pGroup->IntUserEnabledEventMsk.Mid  & pGroup->IntISREnabledEventMsk.Mid );
                Events.High = ~ (pGroup->IntUserEnabledEventMsk.High & pGroup->IntISREnabledEventMsk.High);

                /* Setup mask registers for group interrupts */
                EDDP_HALIsrSetGroupMsk (pDDB, &Events);

                EDDP_EXIT_INT_S();
            }
            break;

            /*---------------------------------------------------------------*/
            case EDDP_ISR_RQB_USER_ID_SINGLE:
            /*---------------------------------------------------------------*/
            {
                EDDP_LOCAL_DDB_CORE_ISR_PTR_TYPE   pIsrInfo;
                LSA_UINT32                         Idx;
                EDDP_ISR_EVENT_FCT_TABLE_TYPE const * pIsrFuncTable;

                pIsrInfo = &pDDB->Core.Isr;
                pIsrFuncTable = pIsrInfo->pIsrFuncTable;

                #ifdef EDDP_ISR_SANITY_CHECKS
                EDDP_ASSERT_FALSE(pRqbParam->Param > EDDP_ISR_MAX_SINGLE_INT);
                #endif
                /* Param is used for IntSource 2..15 */
                Idx = pIsrInfo->Single[pRqbParam->Param].EventNr;

                /* RQB can now be used again..*/
                pIsrInfo->Single[pRqbParam->Param].IntRQB.InUse = LSA_FALSE;

                #ifdef EDDP_ISR_SANITY_CHECKS
                EDDP_ASSERT(LSA_HOST_PTR_ARE_EQUAL(pIsrInfo->Single[pRqbParam->Param].IntRQB.pRQB,pRQB));

                if (Idx >= EDDP_ISR_MAX_EVENTS )
                {
   	                EDDP_LOWER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CoreISRRequest(): FATAL -> Invalid Idx! (Idx=%d)", Idx);
   	                EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
                }

                if (pIsrFuncTable[Idx].HandlerType != EDDP_ISR_HANDLER_TYPE_LOW )
                {
   	                EDDP_LOWER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CoreISRRequest(): FATAL -> Invalid HandlerType! (Idx=%d)", Idx);
   	                EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
                }

                EDDP_ASSERT_NO_NULL_FCT_PTR(pIsrFuncTable[Idx].ISREventHandler);
                #endif

                /* ------------------------------------------------------*/
                /* call Handler for LOW event                            */
                /* ------------------------------------------------------*/
                pIsrFuncTable[Idx].ISREventHandler(pDDB,Idx);

                /* ------------------------------------------------------*/
                /* retrigger IRQ_WaitTime                                */
                /* ------------------------------------------------------*/
                if (pRqbParam->Param == pDDB->Core.Isr.IRQAcycRcv)
                {
                    // IRQ belongs to IF A
                    #if !defined  (EDDP_CFG_ISR_POLLING_MODE) 
                        {
                            // Omit retriggering and checking the waittime when in polling mode
                            EDDP_HALIsrRetriggerMUXIRQ_WaitTime (pDDB, pRqbParam->Param, Idx, pDDB->Core.Isr.IRQ_WaitTime_AcycRcv);
                        }
                    #endif //(EDDP_CFG_ISR_POLLING_MODE)    
                }
                else
                {
   	                EDDP_LOWER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_CoreISRRequest(): FATAL -> Could not find IRQ_WaitTime for retriggering! (Idx=%d)", Idx);
   	                EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
                }
            }
            break;

            /*---------------------------------------------------------------*/
            default:
            /*---------------------------------------------------------------*/
            {
                EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_CoreISRRequest(): FATAL -> Invalid UserID! 0x%X", pRqbParam->UserID);
	            EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
            }
            break;
        } /* switch */

        /* ------------------------------------------------------------------*/
        /* Special check for Shutdown. If we currently shutting down and     */
        /* waiting for pending RQBs..                                        */
        /* ------------------------------------------------------------------*/
        if (pDDB->Core.Isr.ShutdownPending)
        {
            if (!(EDDP_CoreISRGetRQBInUseCnt(pDDB)))
            {
                pDDB->Core.Isr.ShutdownPending = LSA_FALSE;

           	    EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"[H:--] EDDP_CoreISRRequest(): asynchronous shutdown successful. All RQBs now free!");
                EDDP_CoreCBFDeviceShutdown(pDDB,EDD_STS_OK);
            }
        }
    }

    EDDP_LOWER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CoreISRRequest(Response=0x%X)", Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_interrupt                              +*/
/*+  Input                      EDD_HANDLE      hDDB                        +*/
/*+                             LSA_UINT32      IntSource                   +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  hDDB       : EDDP-Devicehandle (from open device)                      +*/
/*+  IntSource  : Source of interrupt:                                      +*/
/*+                                                                         +*/
/*+               EDDP_INT_SOURCE_GROUP  : Group interrupt                  +*/
/*+               2..15                  : Single interrupt 2..15           +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Called from systemadaption on interrupt. Called within    +*/
/*+               ISR handler.                                              +*/
/*+                                                                         +*/
/*+               Handles all PNIP Interrupts and initiates further handling+*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_SYSTEM_IN_FCT_ATTR eddp_interrupt(
	EDDP_HANDLE     hDDB,
    LSA_UINT32      IntSource)
{
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB;
    EDDP_LOCAL_DDB_CORE_ISR_PTR_TYPE   pIsrInfo;

#if defined(EDDP_CFG_USE_ISR_EXTRA_TRACE)
	EDDP_HAL_ISR_GRP_REGS_TYPE    Events;
	EDDP_HAL_ISR_GRP_REGS_TYPE    EventMask;
	LSA_UINT64	k32_Time;
	LSA_UINT64	Nanos;
	LSA_UINT	k32_Nanos;
	LSA_UINT	k32_Seconds;
	LSA_UINT	k32_Minutes;
	LSA_UINT	k32_Hours;
#endif /* defined(EDDP_CFG_USE_ISR_EXTRA_TRACE) */
	
#ifdef EDDP_CFG_USE_ENTER_EXIT_INT_WITHIN_ISR  
    EDDP_ENTER_INT_I();
#endif

	EDDP_LOWER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] --> eddp_interrupt(hDDB=0x%X, IntSource=0x%X)", hDDB, IntSource);
	
    /* get/check pDDB from hDDB */   
    if (EDDP_CoreDevGetDDB(hDDB,&pDDB) != EDD_STS_OK)
    {
	   	EDDP_LOWER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL,"[H:--] eddp_interrupt(): FATAL -> Invalid hDDB (0x%X).",
            hDDB);
		EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        #if defined(EDDP_CFG_USE_ISR_EXTRA_TRACE)
		k32_Time = EDDP_HALGetClockA(pDDB);
		Nanos = k32_Time;

		k32_Hours = (LSA_UINT)(Nanos / 3600000000000ULL);
		if (k32_Hours > 0)
			Nanos -=  k32_Hours * 3600000000000ULL;
		k32_Minutes = (LSA_UINT)(Nanos / 60000000000ULL);
		if (k32_Minutes > 0)
			Nanos -=  k32_Minutes * 60000000000ULL;
		k32_Seconds = (LSA_UINT)(Nanos / 1000000000ULL);
		if (k32_Seconds > 0)
			Nanos -=  k32_Seconds * 1000000000ULL;
		k32_Nanos = (LSA_UINT)Nanos;

		EDDP_LOWER_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] >>> eddp_interrupt(hDDB: 0x%X, IntSource: 0x%X, k32Time: %04u:%02u:%02u.%09u)", 
	        hDDB, IntSource,
			k32_Hours,k32_Minutes,k32_Seconds,k32_Nanos
		);

		/* dummy read to log ISR events */
		EDDP_HALIsrGetEvents(pDDB,&Events);
		/* dummy read to log ISR masks */
		EDDP_HALIsrGetGroupMsk(pDDB,&EventMask);
        #endif /* defined(EDDP_CFG_USE_ISR_EXTRA_TRACE) */

        pIsrInfo = &pDDB->Core.Isr;

        /* only handle interrupt if handler is active. The systemadaption should */
        /* not call this function if the handler is inactive but for sure..      */
        if (pIsrInfo->Active)
        {
            /* ------------------------------------------------------------------*/
            /* check if Group interrupt */
            /* ------------------------------------------------------------------*/
            if (IntSource == EDDP_INT_SOURCE_GROUP)
            {
                LSA_UINT32                      i;
                EDDP_HAL_ISR_GRP_REGS_TYPE      Events;
                EDDP_HAL_ISR_GRP_REGS_TYPE      HelpEvents;

                /* Get Pending IRQ Events for GroupInterrupts */
                EDDP_HALIsrGetGroupIRQ (pDDB, &Events);

                #ifdef EDDP_ISR_USE_HOOKS
                /* application hook */
                EDDP_ISR_HOOK_GROUP(pDDB->hSysDev,Events.Msk.Low,Events.Msk.Mid,Events.Msk.High);
                #endif

                /* check all Group Events for match and call GroupHandler */
                for ( i=0 ; i < EDDP_ISR_GROUP_EVENT_CNT; i++ )
                {
                    HelpEvents.Low  = ( Events.Low  ) & ( pIsrInfo->Group.Events[i].Msk.Low );
                    HelpEvents.Mid  = ( Events.Mid  ) & ( pIsrInfo->Group.Events[i].Msk.Mid );
                    HelpEvents.High = ( Events.High ) & ( pIsrInfo->Group.Events[i].Msk.High);

                    /* if any bit matches call handler */
                    if ((HelpEvents.Low ) || (HelpEvents.Mid ) || (HelpEvents.High))
                    {
                        EDDP_ASSERT_NO_NULL_FCT_PTR(pIsrInfo->Group.Events[i].GroupHandler);
                        pIsrInfo->Group.Events[i].GroupHandler(pDDB,(EDDP_HAL_ISR_GRP_REGS_PTR_TYPE)&HelpEvents);
                    }
                }

                /* EOI for GroupInterrupt */
                #if !defined  (EDDP_CFG_ISR_POLLING_MODE)    
                // EOI is not needed in polling mode
                EDDP_HALIsrGroupEOI(pDDB);
                #endif //(EDDP_CFG_ISR_POLLING_MODE)    
            }
            else
            {
                /* ------------------------------------------------------------------*/
                /* check if single interrupt */
                /* ------------------------------------------------------------------*/
                if ((IntSource >= EDDP_ISR_MIN_SINGLE_INT) && (IntSource < EDDP_ISR_MAX_SINGLE_INT))
                {
                    /* application hook */
                    #ifdef EDDP_ISR_USE_HOOKS
                    EDDP_ISR_HOOK_SINGLE(pDDB->hSysDev,IntSource);
                    #endif

                    /* Call SingleInterruptHandler */
                    EDDP_ASSERT_NO_NULL_FCT_PTR(pIsrInfo->Single[IntSource].SingleHandler);
                    pIsrInfo->Single[IntSource].SingleHandler(pDDB,IntSource);
                }
                /* ------------------------------------------------------------------*/
                /* unexpected IntSource. ignore                                      */
                /* ------------------------------------------------------------------*/
                else
                {
            	   	EDDP_LOWER_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP,"[H:--] eddp_interrupt(): Invalid IntSource! (0x%X).", IntSource);
                }
                /* ------------------------------------------------------------------*/
            }   

        } /* if active */
        else
        {
            #if !defined  (EDDP_CFG_ISR_POLLING_MODE)    
            EDDP_LOWER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE,"[H:--] eddp_interrupt(): Handler called while inactive! (0x%X).",IntSource);
            #endif //(EDDP_CFG_ISR_POLLING_MODE)    
        }

        #if defined(EDDP_CFG_USE_ISR_EXTRA_TRACE)
		k32_Time = EDDP_HALGetClockA(pDDB);
		Nanos = k32_Time;

		k32_Hours = (LSA_UINT)(Nanos / 3600000000000ULL);
		if (k32_Hours > 0)
			Nanos -=  k32_Hours * 3600000000000ULL;
		k32_Minutes = (LSA_UINT)(Nanos / 60000000000ULL);
		if (k32_Minutes > 0)
			Nanos -=  k32_Minutes * 60000000000ULL;
		k32_Seconds = (LSA_UINT)(Nanos / 1000000000ULL);
		if (k32_Seconds > 0)
			Nanos -=  k32_Seconds * 1000000000ULL;
		k32_Nanos = (LSA_UINT)Nanos;

		EDDP_LOWER_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] <<< eddp_interrupt(hDDB: 0x%X, IntSource: 0x%X, k32Time: %04u:%02u:%02u.%09u)", 
			hDDB, IntSource,
			k32_Hours,k32_Minutes,k32_Seconds,k32_Nanos
		);
        #endif /* defined(EDDP_CFG_USE_ISR_EXTRA_TRACE) */
    }

	EDDP_LOWER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] <-- eddp_interrupt(hDDB=0x%X, IntSource=0x%X)", hDDB, IntSource);

    #ifdef EDDP_CFG_USE_ENTER_EXIT_INT_WITHIN_ISR
    EDDP_EXIT_INT_I();
    #endif
}


/*****************************************************************************/
/*  end of file eddp_core_isr.c                                              */
/*****************************************************************************/
