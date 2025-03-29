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
/*  F i l e               &F: evma_plau.h                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  this header checks the plausibility of the event manager's configuration */
/*                                                                           */
/*****************************************************************************/

#ifndef EVMA_PLAU_H_
#define EVMA_PLAU_H_

#ifdef EVMA_USE_PNIP
    #ifdef EVMA_USE_IRTE
        #error choose EVMA_USE_PNIP OR EVMA_USE_IRTE, not both of them!
    #endif
#endif

#ifdef EVMA_USE_IRTE
    #ifndef EVMA_IRTE_IRQ_INDEX
        #error  EVMA_IRTE_IRQ_INDEX is not defined! Choose 12 or 13 on the Ertec200, depending on what EDDI already uses.
    #else
        #if (EVMA_IRTE_IRQ_INDEX < 12) || (EVMA_IRTE_IRQ_INDEX > 13)
            #error EVMA_IRTE_IRQ_INDEX should be 12 or 13 on the Ertec200
        #endif
    #endif

    #if defined (EDDI_CFG_APPLSYNC_SHARED) || defined (EDDI_CFG_APPLSYNC_NEWCYCLE_SHARED) || defined (EDDI_CFG_APPLSYNC_SEPARATE)
        #error The EDDI seems to use the application timers - looks like a conflict ...
    #endif
#endif

#endif /* EVMA_PLAU_H_ */
 
/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
