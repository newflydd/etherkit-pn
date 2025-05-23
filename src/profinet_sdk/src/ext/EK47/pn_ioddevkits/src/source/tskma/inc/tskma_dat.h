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
/*  F i l e               &F: tskma_dat.h                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*   TSKMA data header                                                       */
/*                                                                           */
/*****************************************************************************/

#ifndef TSKMA_DAT_H_
#define TSKMA_DAT_H_

#include "tskma_list.h"

#ifndef TSKMA_DAT_EXT_ATTR
    #define TSKMA_DAT_EXT_ATTR extern
#else
    #define TSKMA_DAT_EXT_ATTR_DECLARED
#endif

typedef struct TSKMA_DATA_S
{
    struct
    {
		struct tskma_list_cb     res_list;		// queued receive resources
		LSA_UINT32				 res_max;		// max number of queued receive resources
    } nrt_rcv;
    struct
    {
		LSA_UINT32                	done;    /* while cold_start.done == LSA_FALSE, all message boxes are read blocked to force a strict startup sequence */
		TSKMA_COLD_START_SM_E_T   	sm;
		LSAS_RQB_TYPE       		lsas_rqb;
		LSAS_RQB_ARGS_TYPE  		lsas_rqb_args;
		TSKMA_RQB_S_T       		tskma_rqb;
		TSKMA_RQB_S_PTR_T 			app_rqb_ptr;
    } cold_start;
    TSKMA_TS_SCHEDULING_STATE_E_T   ts_state;           /* sceduling state for internal timeslicing */
    struct
    {
        LSAS_RQB_TYPE       cyclic_ea_rqb;
        LSAS_RQB_ARGS_TYPE  cyclic_ea_rqb_args;
    } rqb;
    struct
    {
        struct
        {
            TSKMA_RQB_S_T  trigger_pndv_to_pnpb;
            TSKMA_RQB_S_T  trigger_pnpb_to_pndv;
        } rqb;
    } app;
} TSKMA_DATA_S_T;

typedef TSKMA_DATA_S_T * TSKMA_DATA_S_PTR_T;

#ifndef TSKMA_DAT_EXT_ATTR
#define TSKMA_DAT_EXT_ATTR  extern
#endif

TSKMA_DAT_EXT_ATTR TSKMA_DATA_S_T tskma_data;


extern volatile LSA_UINT32 tskma_parity_error_count;
extern volatile LSA_UINT32 tskma_parity_error_source;

extern volatile LSA_UINT32 tskma_access_error_count;
extern volatile LSA_UINT32 tskma_access_error_source;

LSA_VOID tskma_dat_dummy_func(void);

#endif /* TSKMA_DAT_H_ */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
