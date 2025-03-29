
#ifndef EDDP_CORE_DBG_H                  /* ----- reinclude-protection ----- */
#define EDDP_CORE_DBG_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

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
/*  F i l e               &F: eddp_core_dbg.h                           :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Internal headerfile for debugging                                        */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  19.08.08    JS    initial version.                                       */
/*  22.12.08    LRG   K32_TRACE                                              */
/*  10.03.09    LRG   SYNC_TRACE removed                                     */
/*  20.10.09    LRG   EDDP_TRACE_BYTE_ARRAY statt EDDP_DUMP_FRAME            */
/*  22.10.09    SFR   tactical switch to LTRC only                           */
	
#endif
/*****************************************************************************/



/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/


/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* Internal error information of last error occured. Will be set with        */
/* EDDP_DBG_SET_DETAIL_ERR. Only valid if bSet = LSA_TRUE. Will be cleared   */
/* on a request cbf call. Intended to hold the ModuleID/Line of a error of   */
/* the current request. Maybe returned within err-structure of RQB.          */
/* Because this is hold global the request/rqb asigned to this error is not  */
/* 100% sure but most likely the one next finised.                           */
/* Hold within DDB.                                                          */
/*---------------------------------------------------------------------------*/

typedef struct _EDDP_DBG_DETAIL_ERR_TYPE
{
    LSA_BOOL                bSet;
    EDD_RSP                 Response;  /* EDDP response code */
    LSA_UINT32              LineNr; 
    LSA_UINT32              ModuleId; 
} EDDP_DBG_DETAIL_ERR_TYPE;

typedef struct _EDDP_DBG_DETAIL_ERR_TYPE    EDDP_LOCAL_MEM_ATTR *    EDDP_DBG_DETAIL_ERR_PTR_TYPE;

/*===========================================================================*/
/*                                  data                                     */
/*===========================================================================*/

/*===========================================================================*/
/*                                 macros                                    */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* Macro to record error information (line/moduleId).                        */
/*---------------------------------------------------------------------------*/

#ifdef EDDP_CFG_DETAIL_ERROR_INFO
    #define EDDP_DBG_SET_DETAIL_ERR(pDDB,Response)                   EDDP_DbgSetDetailError(pDDB, Response, EDDP_MODULE_ID, __LINE__)
    #define EDDP_DBG_SET_DETAIL_ERR_LOCAL(pErrDetail,Response)       EDDP_DbgSetDetailErrorLocal(pErrDetail, Response, EDDP_MODULE_ID, __LINE__)
    #define EDDP_DBG_COPY_DETAIL_ERR_DDB_LOCAL(pDDB,pErrDetail)      { *pErrDetail = pDDB->ErrDetail; } 
#else
    /* Note: if (pDDB) {} is inserted to prevent compiler warning "unused argument" if */
    /*       pDDB is only used with this macros within function.                       */
    #define EDDP_DBG_SET_DETAIL_ERR(pDDB,Response)                   {if (pDDB) {}}
    #define EDDP_DBG_SET_DETAIL_ERR_LOCAL(pDDB,Response)             {if (pDDB) {}}
    #define EDDP_DBG_COPY_DETAIL_ERR_DDB_LOCAL(pDDB,pErrDetail)      {if (pDDB) {}} 
#endif

/*===========================================================================*/
/*                                prototyping                                */
/*===========================================================================*/




/*****************************************************************************/
/*  end of file EDDP_CORE_DBG.H                                              */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_CORE_DBG_H */
