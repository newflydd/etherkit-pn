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
/*  F i l e               &F: pnpb_peri.h                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
#ifndef PNPB_PERI_H
#define PNPB_PERI_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

#include "pnpb_peri_real.h"


typedef PNIO_VOID    (*PNIO_CBF_CMD_CONF)	// asynchronous confirmation
		(
		 PNIO_UINT32 	UsrHnd,				// [in]  user defined handle from corresponding request
		 PNIO_UINT32 	Status 				// [in]  error value:  PNIO_OK, PNIO_NOT_OK
        );



#define PERIF_HND_NUMOF_ELEMS_PER_ENTRY   2

PNIO_UINT32 ReadPerifTag  (PNIO_ALARM_TYPE PerifCmd, PNIO_UINT32 ValIndex);
PNIO_VOID   StorePerifTag (PNIO_ALARM_TYPE PerifCmd, PNIO_UINT32 ValIndex, PNIO_UINT32 Val);

PNDV_IFACE_CMD_ENTRY_T  pnpb_read_event_from_pndv   (PNIO_UINT32* PerifCmdInd);


PNIO_VOID   pnpb_write_event_to_pndv 
                        (PNIO_UINT8           cmd, 
                         PNIO_UINT8           add_1, 
                         PNIO_UINT16          add_2,
                         PNIO_VOID*          pCmdProp);

PNIO_VOID  PnpbReqSync  (PNIO_UINT8  cmd, 
                         PNIO_UINT8  add_1, 
                         PNIO_UINT16 add_2,
                         PNIO_UINT32 SemId,
                         PNIO_CBF_CMD_CONF  pCbfUsr);

void         Pndv_TriggerPnpb    (void);
void         pnpb_peri_init      (void);
void         pnpb_process_service_requests  (void);

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
