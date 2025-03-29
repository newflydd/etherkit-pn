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
/*  F i l e               &F: eddp_core_dbg.c                           :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP Debugging support functions                 */
/*                          (system dependend)                               */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  19.08.08    JS    initial version.                                       */
/*  10.03.09    LRG   SYNC_TRACE -> K32_TRACE                                */
#endif
/*****************************************************************************/


/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/

#define LTRC_ACT_MODUL_ID  5
#define EDDP_MODULE_ID     LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_CORE_DBG */


/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"            /* eddp headerfiles */
#include "eddp_int.h"            /* internal header  */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID) 


#ifdef EDDP_CFG_DETAIL_ERROR_INFO
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_DbgSetDetailError                      +*/
/*+  Input                 :    EDDP_LOCAL_DDB_PTR_TYPE   pDDB              +*/
/*+                        :    EDD_RSP                   Response          +*/
/*+                             LSA_UINT32                ModuleId          +*/
/*+                             LLSA_UINT32               LineNr            +*/
/*+  Result                :    ----                                        +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Records error information within DDB. maybe returned      +*/
/*+               on request confirmation. The structure typically holds    +*/
/*+               the error of the current request in progress. This is     +*/
/*+               no 100% true if we running concurrent request but most    +*/
/*+               likeley. bSet will be reset on any request confirmation.  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_DbgSetDetailError(
    EDDP_LOCAL_DDB_PTR_TYPE   pDDB,
    EDD_RSP                   Response,    
    LSA_UINT32                ModuleId,
    LSA_UINT32                LineNr )
{

    if ( !LSA_HOST_PTR_ARE_EQUAL(pDDB, LSA_NULL) )
    {
        if (pDDB->ErrDetail.bSet == 0)
        {
            pDDB->ErrDetail.Response    = Response;
            pDDB->ErrDetail.ModuleId    = ModuleId;
            pDDB->ErrDetail.LineNr      = LineNr;
            pDDB->ErrDetail.bSet        = LSA_TRUE;
        }
    }

}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_DbgSetDetailErrorLocal                 +*/
/*+  Input                 :    EDDP_DBG_DETAIL_ERR_PTR_TYPE pErrDetail     +*/
/*+                        :    EDD_RSP                   Response          +*/
/*+                             LSA_UINT32                ModuleId          +*/
/*+                             LLSA_UINT32               LineNr            +*/
/*+  Result                :    ----                                        +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Copys Response/ModuleId/LineNr to pErrDetail and sets bSet+*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_DbgSetDetailErrorLocal(
    EDDP_DBG_DETAIL_ERR_PTR_TYPE   pErrDetail,
    EDD_RSP                        Response,    
    LSA_UINT32                     ModuleId,
    LSA_UINT32                     LineNr )
{

    pErrDetail->Response    = Response;
    pErrDetail->ModuleId    = ModuleId;
    pErrDetail->LineNr      = LineNr;
    pErrDetail->bSet        = LSA_TRUE;

}

#endif


/*****************************************************************************/
/*  end of file eddp_core_dbg.c                                              */
/*****************************************************************************/


