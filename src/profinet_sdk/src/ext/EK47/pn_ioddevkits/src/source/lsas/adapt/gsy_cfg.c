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
/*  F i l e               &F: gsy_cfg.c                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  system adaption of this component                                        */
/*                                                                           */
/*****************************************************************************/

/* ***************************************************************************
*
*   H i s t o r y :
*   see /_doc/im151_3pn_hist.doc
*
* ****************************************************************************/

#define LSAS_MODULE_ID  20

#include "lsas_inc.h"
#include "lsas_int.h"

#include "gsy_trc.h"
#include "gsy_pls.h"
#include "gsy_int.h"

/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/


/******************************************************************************
 *                                                                            *
 *                                                                            *
 *  function prototypes in gsy_usr.h                                          *
 *                                                                            *
 *                                                                            *
 ******************************************************************************/

#ifdef GSY_DO_TIMEOUT_EXTERN
/*=============================================================================
 * function name:  GSY_DO_TIMER_REQUEST
 *
 * function:       Sends the spezified RQB to gsy_request(). This
 *                 function is called within DCP timer handler to signal
 *                 a Timer event to the main context of GSY.
 *                 Systemadaption has to call gsy_request() with the specified
 *                 RQB. Before calling, the systemadaption should change to
 *                 taskstate.
 *
 * parameters:     GSY_UPPER_RQB_PTR_TYPE   pRQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_DO_TIMER_REQUEST
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_DO_TIMER_REQUEST(
    GSY_UPPER_RQB_PTR_TYPE  pRQB
)
{

    LSAS_RQB_GSY_REQUEST(pRQB, gsy_request);
}
#endif

/*=============================================================================
 * function name:  GSY_ALLOC_UPPER_RQB_LOCAL
 *
 * function:       allocate an upper-RQB for local use
 *
 * parameters:     GSY_UPPER_RQB_PTR_TYPE  ...  *  upper_rqb_ptr_ptr:
 *                                   return value: pointer to local-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: No memory
 *                                                 available
 *                 LSA_UINT16                      length:   length of RQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_ALLOC_UPPER_RQB_LOCAL
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_ALLOC_UPPER_RQB_LOCAL(
    GSY_UPPER_RQB_PTR_TYPE  GSY_LOCAL_MEM_ATTR *  upper_rqb_ptr_ptr,
    LSA_UINT16                                    length
)
{
    LSAS_ALLOC_COMP_MEM(upper_rqb_ptr_ptr, length, LSA_COMP_ID_GSY);
}
#endif

/*=============================================================================
 * function name:  GSY_FREE_UPPER_RQB_LOCAL
 *
 * function:       free an upper-RQB allocated with GSY_ALLOC_UPPER_RQB_LOCAL
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      upper-RQB or
 *                 GSY_UPPER_RQB_PTR_TYPE  upper_rqb_ptr:  pointer to upper-
 *                                                            RQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_FREE_UPPER_RQB_LOCAL
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_FREE_UPPER_RQB_LOCAL(
    LSA_UINT16                 GSY_LOCAL_MEM_ATTR *  ret_val_ptr,
    GSY_UPPER_RQB_PTR_TYPE                           upper_rqb_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, upper_rqb_ptr, LSA_COMP_ID_GSY);
}
#endif
#endif /* GSY_DO_TIMEOUT_EXTERN */

/*=============================================================================
 * function name:  GSY_ALLOC_UPPER_RQB
 *
 * function:       allocate an upper-RQB
 *
 * parameters:     GSY_UPPER_RQB_PTR_TYPE  ...  *  upper_rqb_ptr_ptr:
 *                                   return value: pointer to upper-RQB-pointer
 *                                                 or LSA_NULL: The allocation
 *                                                 will be asynchronous, see
 *                                                 gsy_upper_rqb().
 *                 LSA_USER_ID_TYPE                   user_id:  id of GSY
 *                 LSA_UINT16                         length:   length of RQB
 *                 LSA_SYS_PTR_TYPE                   sys_ptr:  system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_ALLOC_UPPER_RQB
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_ALLOC_UPPER_RQB(
    GSY_UPPER_RQB_PTR_TYPE  GSY_LOCAL_MEM_ATTR *  upper_rqb_ptr_ptr,
    LSA_USER_ID_TYPE                                    user_id,
    LSA_UINT16                                          length,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(upper_rqb_ptr_ptr, length, LSA_COMP_ID_GSY);

    LSA_UNUSED_ARG( user_id );
    LSA_UNUSED_ARG( sys_ptr );
}
#endif

/*=============================================================================
 * function name:  GSY_FREE_UPPER_RQB
 *
 * function:       free an upper-RQB
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      upper-RQB or
 *                                                      wrong system-pointer
 *                 GSY_UPPER_RQB_PTR_TYPE  upper_rqb_ptr:  pointer to upper-
 *                                                            RQB
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_FREE_UPPER_RQB
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_FREE_UPPER_RQB(
    LSA_UINT16                 GSY_LOCAL_MEM_ATTR *  ret_val_ptr,
    GSY_UPPER_RQB_PTR_TYPE                           upper_rqb_ptr,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, upper_rqb_ptr, LSA_COMP_ID_GSY);

    LSA_UNUSED_ARG( sys_ptr );
}
#endif

/*=============================================================================
 * function name:  GSY_ALLOC_UPPER_MEM
 *
 * function:       allocate an upper-memory
 *
 * parameters:     GSY_UPPER_MEM_PTR_TYPE  ...  *  upper_mem_ptr_ptr:
 *                                   return value: pointer to upper-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: The allocation
 *                                                 will be asynchronous, see
 *                                                 gsy_upper_mem().
 *                 LSA_USER_ID_TYPE                   user_id:  id of GSY
 *                 LSA_UINT16                         length:   length of
 *                                                              memory
 *                 LSA_SYS_PTR_TYPE                   sys_ptr:  system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_ALLOC_UPPER_MEM
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_ALLOC_UPPER_MEM(
    GSY_UPPER_MEM_PTR_TYPE  GSY_LOCAL_MEM_ATTR *  upper_mem_ptr_ptr,
    LSA_USER_ID_TYPE                                    user_id,
    LSA_UINT16                                          length,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(upper_mem_ptr_ptr, length, LSA_COMP_ID_GSY);

    LSA_UNUSED_ARG( user_id );
    LSA_UNUSED_ARG( sys_ptr );
}
#endif

/*=============================================================================
 * function name:  GSY_FREE_UPPER_MEM
 *
 * function:       free an upper-memory
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      upper-memory or
 *                                                      wrong system-pointer
 *                 GSY_UPPER_MEM_PTR_TYPE  upper_mem_ptr:  pointer to upper-
 *                                                            memory
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_FREE_UPPER_MEM
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_FREE_UPPER_MEM(
    LSA_UINT16                 GSY_LOCAL_MEM_ATTR *  ret_val_ptr,
    GSY_UPPER_MEM_PTR_TYPE                           upper_mem_ptr,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, upper_mem_ptr, LSA_COMP_ID_GSY);

    LSA_UNUSED_ARG( sys_ptr );
}
#endif

/*=============================================================================
 * function name:  GSY_RQB_ERROR
 *
 * function:       handle a RQB that cannot be returned to upper callback
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                 GSY_UPPER_RQB_PTR_TYPE  upper_rqb_ptr:  pointer to upper
 *                                                            request block
 * return value:   LSA_VOID
 *===========================================================================*/

/* provided as macro in gsy_cfg.h */
#ifndef GSY_RQB_ERROR
LSA_VOID  GSY_UPPER_OUT_FCT_ATTR  GSY_RQB_ERROR(
    GSY_UPPER_RQB_PTR_TYPE                      upper_rqb_ptr
)
{
    /* !! must not be a function, by design !! */
}
#endif


/******************************************************************************
 *                                                                            *
 *                                                                            *
 *  function prototypes in gsy_sys.h                                          *
 *                                                                            *
 *                                                                            *
 ******************************************************************************/

/*====  GSY out functions  =====*/

/*=============================================================================
 * function name:  GSY_DRIFT_GET
 *
 * function:       read drift correction interval and direction
 *
 *
 * parameters:     pInterval      for return of drift interval in nanoseconds
 *                                              to de corrected by +- 1 nanosecond
 *                 pPlus               "        if LSA_TRUE: correction is +1 ns
 *                                              if LSA_FALSE: correction is -1 ns
 *
 * return value:   -
 *===========================================================================*/
#ifdef GSY_DRIFT_EXTERNAL
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_DRIFT_GET(
    LSA_UINT32  GSY_LOCAL_MEM_ATTR *    pNanos,
    LSA_BOOL    GSY_LOCAL_MEM_ATTR *    pPlus
)
{
    /* tbd */
}
#endif

/*=============================================================================
 * function name:  GSY_SYNC_GET
 *
 * function:       read PTCP time and reference timestamp
 *
 *
 * parameters:     pSeconds      for return of time seconds
 *                 pNanoSeconds        "       time nanoseconds (max.999.999.999)
 *                 pTimestamp          "       reference time stamp
 *                                             = act. value of the send timestamp
 *                                               counter of the system channels
 *                                               device
 *                 SyncId        0=clock, 1=time...
 *                 sys_ptr       of user channel: for channel identification
 *
 *
 * return value:   LSA_RET_OK or LSA_RET_ERR_PARAM or...
 *===========================================================================*/
#ifndef GSY_SYNC_GET
LSA_UINT16  GSY_SYSTEM_OUT_FCT_ATTR  GSY_SYNC_GET(
    LSA_UINT16    GSY_LOCAL_MEM_ATTR *    pTimeEpoch,
    LSA_UINT32  GSY_LOCAL_MEM_ATTR *    pSeconds,
    LSA_UINT32  GSY_LOCAL_MEM_ATTR *    pNanoSeconds,
    LSA_UINT32  GSY_LOCAL_MEM_ATTR *    pTimeStamp,
    LSA_UINT8                            SyncId,
    LSA_SYS_PTR_TYPE                    sys_ptr
)
{
    LSA_UINT16              ret_val = GSY_RET_ERR_PARAM;

#ifdef LSAS_CFG_USE_EDDI
    EDDI_SYNC_GET_SET_TYPE  SyncGet;
    LSA_UINT64              Nanoseconds;
    LSA_RESULT              edd_ret_val;


    LSA_UNUSED_ARG(sys_ptr);

    /* Wird der Bereich der sync_id eingehalten */
    if (SyncId <= 1)
    {
        /* Derzeit nur Clock-Sync unterstuetzt */
        if (0 == SyncId)
        {
            edd_ret_val = eddi_SyncGet(lsas_com_get_hddb(), &SyncGet, LSA_NULL);
            if(EDD_STS_OK == edd_ret_val)
            {
              // AG: Calculate the current time from the cycle counter * 31250 + Timestamp in Ticks sinse start of cycle
                Nanoseconds = (LSA_UINT64)(SyncGet.CycleCounterHigh & 0xFFFF);
                Nanoseconds <<= 32;
                Nanoseconds |= (LSA_UINT64)SyncGet.CycleCounterLow;
                Nanoseconds *=  (LSA_UINT64) 31250;          // -AG Nanoseconds now contains the starttime of the current cycle in ns
                *pSeconds = (LSA_UINT32)(Nanoseconds/(LSA_UINT64) 1000000000);   // Calculate second vaule 
                *pNanoSeconds = (LSA_UINT32)(Nanoseconds % (LSA_UINT64) 1000000000);
                *pTimeStamp = SyncGet.CycleBeginTimeStampInTicks;  // Time since start of last cycle 
                ret_val = GSY_RET_OK;
                *pTimeEpoch = 0;

            }
        }
    }
#endif

#ifdef LSAS_CFG_USE_EDDP

    //Das Makro wird nicht ausgef�hrt, wenn die Synchronisation in Hardware l�uft (KRISC32) -> siehe GSY-Doku

#ifdef RZT2_RSK_PNS

		LSA_UINT32 ethsw_get_time(LSA_UINT8 timer);
		LSA_UINT32 ethsw_get_time_sec(LSA_UINT16 timer);
       LSA_UNUSED_ARG(SyncId);

    LSA_UINT32 ns = 0; 
       *pTimeEpoch     = 0;
       LSA_UINT8 timer = 0;
       ns   = ethsw_get_time(timer);
       *pSeconds       = ethsw_get_time_sec(timer);
       
       *pTimeStamp   = ns % 4000000;    // pTimeStamp is time since beginning of last cycle
       *pNanoSeconds = ns - (*pTimeStamp);      // AG   must be updated to real cycle time Time when last ccle started 
 
       ret_val = LSA_RET_OK;
#endif

#endif

#ifdef LSAS_CFG_USE_EDDS
    {
        *pTimeEpoch   = 0;
        *pSeconds     = 0;
        *pNanoSeconds = 0;
        *pTimeStamp   = 0;
        LSA_UNUSED_ARG(SyncId);
        ret_val = LSA_RET_OK;
    }


    LSA_UNUSED_ARG(sys_ptr);

#endif


    return(ret_val);
}

#endif
/*=============================================================================
 * function name:  GSY_SYNC_GET_EXT_PLL_IN
 *
 * function:       read PTCP time and reference timestamp
 *
 *
 * parameters:     pTimestampPLL for return of time stamp of last PLL_IN signal
 *                 pSeconds                    time seconds
 *                 pNanoSeconds        "       time nanoseconds (max.999.999.999)
 *                 pTimestamp          "       reference time stamp
 *                                             = act. value of the send timestamp
 *                                               counter of the system channels
 *                                               device
 *                 SyncId        0=clock, 1=time...
 *                 sys_ptr       of user channel: for channel identification
 *
 *
 * return value:   LSA_RET_OK or LSA_RET_ERR_PARAM or...
 *===========================================================================*/
#ifndef GSY_SYNC_GET_EXT_PLL_IN
LSA_UINT16  GSY_SYSTEM_OUT_FCT_ATTR  GSY_SYNC_GET_EXT_PLL_IN(
    LSA_UINT32  GSY_LOCAL_MEM_ATTR *    pTimeStampPLL,
    LSA_UINT32  GSY_LOCAL_MEM_ATTR *    pSeconds,
    LSA_UINT32  GSY_LOCAL_MEM_ATTR *    pNanoSeconds,
    LSA_UINT32  GSY_LOCAL_MEM_ATTR *    pTimeStamp,
    LSA_UINT8                           SyncId,
    LSA_SYS_PTR_TYPE                    sys_ptr
)
{
    LSA_UINT16              ret_val = GSY_RET_ERR_PARAM;
#ifdef LSAS_CFG_USE_EDDI
    EDDI_SYNC_GET_SET_TYPE  SyncGet;
    LSA_UINT64              Nanoseconds;
    LSA_RESULT              edd_ret_val;


    LSA_UNUSED_ARG(sys_ptr);

    /* Wird der Bereich der sync_id eingehalten */
    if (SyncId <= 1)
    {
        /* Derzeit nur Clock-Sync unterstuetzt */
        if (0 == SyncId)
        {
            edd_ret_val = eddi_SyncGet(lsas_com_get_hddb(), &SyncGet, pTimeStampPLL);
            if(EDD_STS_OK == edd_ret_val)
            {
                Nanoseconds = (LSA_UINT64)(SyncGet.CycleCounterHigh & 0xFFFF);
                Nanoseconds <<= 32;
                Nanoseconds |= (LSA_UINT64)SyncGet.CycleCounterLow;
                Nanoseconds *=  (LSA_UINT64) 31250;
                *pSeconds = (LSA_UINT32)(Nanoseconds/(LSA_UINT64) 1000000000);
                *pNanoSeconds = (LSA_UINT32)(Nanoseconds % (LSA_UINT64) 1000000000);
                *pTimeStamp = SyncGet.CycleBeginTimeStampInTicks;
                ret_val = GSY_RET_OK;

            }
        }
    }
#endif

#ifdef LSAS_CFG_USE_EDDP
    //Das Makro wird nicht ausgef�hrt, wenn die Synchronisation in Hardware l�uft (KRISC32) -> siehe GSY-Doku
#ifdef RZT2_RSK_PNS
    LSA_UNUSED_ARG(pTimeStampPLL);
    LSA_UNUSED_ARG(pSeconds);
    LSA_UNUSED_ARG(pNanoSeconds);
    LSA_UNUSED_ARG(pTimeStamp);
    LSA_UNUSED_ARG(SyncId);
    LSA_UNUSED_ARG(sys_ptr);

    ret_val =  LSA_RET_OK;
#endif

#endif

#ifdef LSAS_CFG_USE_EDDS
    LSA_UNUSED_ARG(pTimeStampPLL);
    LSA_UNUSED_ARG(pSeconds);
    LSA_UNUSED_ARG(pNanoSeconds);
    LSA_UNUSED_ARG(pTimeStamp);
    LSA_UNUSED_ARG(SyncId);
    LSA_UNUSED_ARG(sys_ptr);

    ret_val =  LSA_RET_OK;
#endif
    return(ret_val);
}
#endif

/*=============================================================================
 * function name:  GSY_SYNC_SET
 *
 * function:       set PTCP time with reference timestamp
 *
 *
 * parameters:     Seconds       time seconds
 *                 NanoSeconds   time nanoseconds (max.999.999.999)
 *                 Timestamp     reference time stamp
 *                               = value of the recv timestamp
 *                                 counter of the system channels device               376
 *
 *                 SyncId        0=clock, 1=time...
 *                 sys_ptr       of user channel: for channel identification
 *
 *
 * return value:   LSA_RET_OK or LSA_RET_ERR_PARAM or...
 *===========================================================================*/
#ifndef GSY_SYNC_SET
LSA_UINT16  GSY_SYSTEM_OUT_FCT_ATTR  GSY_SYNC_SET(
    LSA_UINT16                            EpochNumber,
    LSA_UINT32                          Seconds,
    LSA_UINT32                          NanoSeconds,
    LSA_UINT32                          TimeStamp,
    LSA_INT32                           RcfInterval,
    LSA_UINT8                           SyncId,
    LSA_SYS_PTR_TYPE                    sys_ptr
)
{
    LSA_UNUSED_ARG(EpochNumber);
    LSA_UNUSED_ARG(Seconds);
    LSA_UNUSED_ARG(NanoSeconds);
    LSA_UNUSED_ARG(TimeStamp);
    LSA_UNUSED_ARG(RcfInterval);
    LSA_UNUSED_ARG(SyncId);
    LSA_UNUSED_ARG(sys_ptr);
    return (LSA_RET_ERR_PARAM);
}
#endif


/*=============================================================================
 * function name:  GSY_RATE_SET
 *
 * function:       report actual Value of master rate
 *
 *
 * parameters:     RateInterval  Rate interval of lokal time to master time in ns
 *                               (to be corrected by +/- one nanosecond, 0=unkown)
 *                                +100 ==> 101, -100 ==> 99
 *                 RateValid     RateInterval value is valid or not
 *                 SyncId        0=clock, 1=time...
 *                 sys_ptr       of user channel: for channel identification
 *
 *
 * return value:   LSA_RET_OK or LSA_RET_ERR_PARAM
 *===========================================================================*/
LSA_UINT16  GSY_SYSTEM_OUT_FCT_ATTR  GSY_RATE_SET(
    LSA_INT32                            RateInterval,
    LSA_UINT8                            RateValid,
    LSA_UINT8                            SyncId,
    LSA_SYS_PTR_TYPE                    sys_ptr
)
{
    LSA_UINT16 RetVal = GSY_RET_OK;

    if ((SyncId == GSY_SYNCID_CLOCK) || (SyncId > GSY_SYNCID_MAX))
    {
        RetVal = LSA_RET_ERR_PARAM;
    }
//    tst_ChList[ChIdx].Rate[SyncId].Value = RateInterval;
//    tst_ChList[ChIdx].Rate[SyncId].Valid = RateValid;

    return(RetVal);
}

/*=============================================================================
 * function name:  GSY_SYNC_ADJUST
 *
 * function:       adjust clock for PTCP time
 *
 *
 * parameters:     AdjustInterval    correction interval in nanoseconds
 *                                   (to be corrected by +/- one nanosecond)
 *                                   +100 ==> 101, -100 ==> 99
 *                 SyncId        0=clock, 1=time...
 *                 sys_ptr       of user channel: for channel identification
 *
 * return value:   LSA_RET_OK or LSA_RET_ERR_PARAM or...
 *===========================================================================*/
LSA_UINT16  GSY_SYSTEM_OUT_FCT_ATTR  GSY_SYNC_ADJUST(
    LSA_INT32                           AdjustInterval,
    LSA_UINT8                           SyncId,
    LSA_SYS_PTR_TYPE                    sys_ptr
)
{
    LSA_UINT16  RetVal = GSY_RET_ERR_PARAM;

#ifdef LSAS_CFG_USE_EDDI

    if(( SyncId == GSY_SYNCID_CLOCK )
    || ( SyncId == GSY_SYNCID_TIME ))
    {
        EDDI_HANDLE hDDB = lsas_com_get_hddb();

        RetVal = eddi_SyncAdjust( hDDB, AdjustInterval);

        if( RetVal != EDD_STS_OK )
        {

            LSAS_ASSERT( 0);

            RetVal = LSA_RET_ERR_PARAM; // unreachable
        }
    }
    else
    {
        RetVal = LSA_RET_ERR_PARAM;
    }
#endif
#ifdef LSAS_CFG_USE_EDDP
    LSA_UNUSED_ARG(AdjustInterval);
    LSA_UNUSED_ARG(SyncId);

    RetVal = LSA_RET_OK;
    //Das Makro wird nicht ausgef�hrt, wenn die Synchronisation in Hardware l�uft (KRISC32) -> siehe GSY-Doku
#endif
#ifdef LSAS_CFG_USE_EDDS
    {
        LSA_UNUSED_ARG(AdjustInterval);
        LSA_UNUSED_ARG(SyncId);

        RetVal = LSA_RET_OK;
    }
#endif

    return(RetVal);
}

/*====  LSA out functions  =====*/

/*=============================================================================
 * function name:  GSY_INIT_SYS_PTR
 *
 * function:       initialize system-pointer
 *
 *                 Attention:
 *                 The LSA component only may call this macro when the
 *                 component hasn't call GSY_GET_PATH_INFO() yet.
 *                 The real value assignment then must be made by the call of
 *                 GSY_GET_PATH_INFO().
 *
 * parameters:     LSA_SYS_PTR_TYPE  ...  *  sys_ptr_ptr:
 *                                      return value: pointer to system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_INIT_SYS_PTR
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_INIT_SYS_PTR(
    LSA_SYS_PTR_TYPE  GSY_LOCAL_MEM_ATTR *  sys_ptr_ptr
)
{
    /* not needed by system yet */
    LSA_UNUSED_ARG( sys_ptr_ptr );
}
#endif

/*=============================================================================
 * function name:  GSY_SYS_PTR_IS_INITIALIZED
 *
 * function:       Is the system-pointer initialized
 *                 (by GSY_INIT_SYS_PTR())?
 *
 * parameters:     LSA_BOOL  ...  *  ret_val_ptr:
 *                                        return value: LSA_TRUE   yes
 *                                                      LSA_FALSE  no
 *                 LSA_SYS_PTR_TYPE  sys_ptr:   system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_SYS_PTR_IS_INITIALIZED
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_SYS_PTR_IS_INITIALIZED(
    LSA_UINT8          GSY_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_SYS_PTR_TYPE                           sys_ptr
)
{
    /* not needed by system yet */
    LSA_UNUSED_ARG( ret_val_ptr );
    LSA_UNUSED_ARG( sys_ptr );
}
#endif

/*=============================================================================
 * function name:  GSY_GET_PATH_INFO
 *
 * function:       get system-pointer and detail-pointer of a communication
 *                 channel
 *
 * parameters:     LSA_UINT16              ...  *  ret_val_ptr:
 *                                      return value: LSA_RET_OK
 *                                                    LSA_RET_ERR_SYS_PATH
 *                 LSA_SYS_PTR_TYPE        ...  *  sys_ptr_ptr:
 *                                      return value: pointer to system-pointer
 *                 GSY_DETAIL_PTR_TYPE  ...  *  detail_ptr_ptr:
 *                                      return value: pointer to detail-pointer
 *                 LSA_SYS_PATH_TYPE               path: path of a
 *                                                       communication channel
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_GET_PATH_INFO
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_GET_PATH_INFO(
    LSA_UINT16              GSY_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_SYS_PTR_TYPE        GSY_LOCAL_MEM_ATTR *  sys_ptr_ptr,
    GSY_DETAIL_PTR_TYPE  GSY_LOCAL_MEM_ATTR *  detail_ptr_ptr,
    LSA_SYS_PATH_TYPE                                path
)
{
    LSA_GET_PATH_INFO(ret_val_ptr, sys_ptr_ptr, detail_ptr_ptr,
                      path, LSAS_PATH_LAYER_GSY);
}
#endif

/*=============================================================================
 * function name:  GSY_RELEASE_PATH_INFO
 *
 * function:       release system-pointer and detail-pointer of a communication
 *                 channel
 *
 * parameters:     LSA_UINT16      ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no release because of
 *                                                      wrong system-pointer or
 *                                                      wrong detail-pointer
 *                 LSA_SYS_PTR_TYPE        sys_ptr:     system-pointer
 *                 GSY_DETAIL_PTR_TYPE  detail_ptr:  detail-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_RELEASE_PATH_INFO
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_RELEASE_PATH_INFO(
    LSA_UINT16              GSY_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_SYS_PTR_TYPE        sys_ptr,
    GSY_DETAIL_PTR_TYPE  detail_ptr
)
{
    LSA_RELEASE_PATH_INFO(ret_val_ptr, sys_ptr, detail_ptr);
}
#endif

/*=============================================================================
 * function name:  GSY_ALLOC_LOCAL_MEM
 *
 * function:       allocate a local-memory
 *
 * parameters:     GSY_LOCAL_MEM_PTR_TYPE  ...  *  local_mem_ptr_ptr:
 *                                   return value: pointer to local-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: No memory
 *                                                 available
 *                 LSA_UINT16                         length: length of memory
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_ALLOC_LOCAL_MEM
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_ALLOC_LOCAL_MEM(
    GSY_LOCAL_MEM_PTR_TYPE  GSY_LOCAL_MEM_ATTR *  local_mem_ptr_ptr,
    LSA_UINT16                                          length
)
{
    LSAS_ALLOC_COMP_MEM(local_mem_ptr_ptr, length, LSA_COMP_ID_GSY);
}
#endif

/*=============================================================================
 * function name:  GSY_FREE_LOCAL_MEM
 *
 * function:       free a local-memory
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      local-memory
 *                 GSY_LOCAL_MEM_PTR_TYPE  local_mem_ptr:  pointer to local-
 *                                                            memory
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_FREE_LOCAL_MEM
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_FREE_LOCAL_MEM(
    LSA_UINT16                 GSY_LOCAL_MEM_ATTR *  ret_val_ptr,
    GSY_LOCAL_MEM_PTR_TYPE                           local_mem_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, local_mem_ptr, LSA_COMP_ID_GSY);
}
#endif

/*=============================================================================
 * function name:  GSY_MEMSET_LOCAL
 *
 * function:       fills memory with value
 *
 * parameters:      GSY_LOCAL_MEM_PTR_TYPE         pMem
 *                  LSA_UINT8                      Value
 *                  LSA_UINT32                     Length
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_MEMSET_LOCAL
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_MEMSET_LOCAL(
    GSY_LOCAL_MEM_PTR_TYPE              pMem,
    LSA_UINT8                           Value,
    LSA_UINT32                          Length
)
{
    LSAS_MEMSET( pMem, Value, Length );
}
#endif

/*=============================================================================
 * function name:  GSY_MEMSET_UPPER
 *
 * function:       fills memory with value
 *
 * parameters:      GSY_UPPER_MEM_PTR_TYPE         pMem
 *                  LSA_UINT8                      Value
 *                  LSA_UINT32                     Length
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_MEMSET_UPPER
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_MEMSET_UPPER(
    GSY_UPPER_MEM_PTR_TYPE              pMem,
    LSA_UINT8                           Value,
    LSA_UINT32                          Length
)
{
    LSAS_MEMSET( pMem, Value, Length );
}
#endif

/*=============================================================================
 * function name:  GSY_LOWER_TO_UPPER_MEM
 *
 * function:       convert lower-memory to upper-memory
 *
 * parameters:     GSY_UPPER_MEM_PTR_TYPE  ...  *  upper_mem_ptr_ptr:
 *                                   return value: pointer to upper-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: conversion
 *                                                 wasn't possible
 *                 GSY_LOWER_MEM_PTR_TYPE          lower_mem_ptr:
 *                                                 pointer to lower-memory
 *                 LSA_SYS_PTR_TYPE                   sys_ptr:  system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_LOWER_TO_UPPER_MEM
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_LOWER_TO_UPPER_MEM(
    GSY_UPPER_MEM_PTR_TYPE  GSY_LOCAL_MEM_ATTR *  upper_mem_ptr_ptr,
    GSY_LOWER_MEM_PTR_TYPE                           lower_mem_ptr,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    *upper_mem_ptr_ptr = lower_mem_ptr;

    LSA_UNUSED_ARG( sys_ptr );
}
#endif

/*=============================================================================
 * function name:  GSY_UPPER_TO_LOWER_MEM
 *
 * function:       convert upper-memory to lower-memory
 *
 * parameters:     GSY_LOWER_MEM_PTR_TYPE  ...  *  lower_mem_ptr_ptr:
 *                                   return value: pointer to lower-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: conversion
 *                                                 wasn't possible
 *                 GSY_UPPER_MEM_PTR_TYPE          upper_mem_ptr:
 *                                                 pointer to upper-memory
 *                 LSA_SYS_PTR_TYPE                   sys_ptr:  system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_UPPER_TO_LOWER_MEM
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_UPPER_TO_LOWER_MEM(
    GSY_LOWER_MEM_PTR_TYPE  GSY_LOCAL_MEM_ATTR *  lower_mem_ptr_ptr,
    GSY_UPPER_MEM_PTR_TYPE                           upper_mem_ptr,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    /* not needed by system yet */
    *lower_mem_ptr_ptr = upper_mem_ptr;

    LSA_UNUSED_ARG( sys_ptr );
}
#endif

/*=============================================================================
 * function name:  GSY_LOWER_TO_LOWER_MEM
 *
 * function:       convert lower-memory to lower-memory
 *
 * parameters:     GSY_LOWER_MEM_PTR_TYPE  ...  *  lower_mem_ptr_ptr:
 *                                   return value: pointer to lower-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: conversion
 *                                                 wasn't possible
 *                 GSY_LOWER_MEM_PTR_TYPE          lower_mem_ptr:
 *                                                 pointer to lower-memory
 *                 LSA_SYS_PTR_TYPE                   src_sys_ptr:
 *                                                 system-pointer of
 *                                                 source-memory
 *                 LSA_SYS_PTR_TYPE                   dst_sys_ptr:
 *                                                 system-pointer of
 *                                                 destination-memory
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_LOWER_TO_LOWER_MEM
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_LOWER_TO_LOWER_MEM(
    GSY_LOWER_MEM_PTR_TYPE  GSY_LOCAL_MEM_ATTR *  lower_mem_ptr_ptr,
    GSY_LOWER_MEM_PTR_TYPE                           lower_mem_ptr,
    LSA_SYS_PTR_TYPE                                    dst_sys_ptr
)
{
    /* not needed by system yet */
    *lower_mem_ptr_ptr = lower_mem_ptr;

    LSA_UNUSED_ARG( dst_sys_ptr );

}
#endif

/*=============================================================================
 * function name:  GSY_COPY_COMMON_MEM
 *
 * function:       copy common memory
 *                 The memory areas of source and destination mustn't overlap.
 *
 * parameters:     LSA_VOID  GSY_COMMON_MEM_ATTR *  src_common_mem_ptr:
 *                                                 void-pointer to source-
 *                                                 common-memory
 *                 LSA_VOID  GSY_COMMON_MEM_ATTR *  dst_common_mem_ptr:
 *                                                 void-pointer to destination-
 *                                                 common-memory
 *                 LSA_UINT16                          length: number of bytes
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_COPY_COMMON_MEM
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_COPY_COMMON_MEM(
    LSA_VOID  GSY_COMMON_MEM_ATTR *  src_common_mem_ptr,
    LSA_VOID  GSY_COMMON_MEM_ATTR *  dst_common_mem_ptr,
    LSA_UINT16                          length
)
{
    LSAS_MEMCPY( dst_common_mem_ptr, src_common_mem_ptr, length );
}
#endif

/*=============================================================================
 * function name:  GSY_COPY_LOCAL_TO_UPPER_MEM
 *
 * function:       copy local data to upper memory
 *
 * parameters:     GSY_LOCAL_MEM_PTR_TYPE   src_ptr:    pointer to lower
 *                                                      receive PDU memory
 *                 GSY_UPPER_MEM_PTR_TYPE   dst_ptr:    pointer to upper memory
 *                 LSA_UINT16               length:     number of bytes to copy
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_COPY_LOCAL_TO_UPPER_MEM
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR   GSY_COPY_LOCAL_TO_UPPER_MEM(
    GSY_LOCAL_MEM_PTR_TYPE          src_ptr,
    GSY_UPPER_MEM_PTR_TYPE          dst_ptr,
    LSA_UINT16                      length,
    LSA_SYS_PTR_TYPE                sys_ptr
)
{
    LSAS_MEMCPY( dst_ptr, src_ptr, length );

    LSA_UNUSED_ARG( sys_ptr );
}
#endif

/*=============================================================================
 * function name:  GSY_COPY_UPPER_TO_LOCAL_MEM
 *
 * function:       copy local data to upper memory
 *
 * parameters:     GSY_UPPER_MEM_PTR_TYPE   src_ptr:    pointer to upper memory
 *                 GSY_LOCAL_MEM_PTR_TYPE   dst_ptr:    pointer to local memory
 *                 LSA_UINT16               length:     number of bytes to copy
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_COPY_UPPER_TO_LOCAL_MEM
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR   GSY_COPY_UPPER_TO_LOCAL_MEM(
    GSY_UPPER_MEM_PTR_TYPE          src_ptr,
    GSY_LOCAL_MEM_PTR_TYPE          dst_ptr,
    LSA_UINT16                      length,
    LSA_SYS_PTR_TYPE                sys_ptr
)
{
    LSAS_MEMCPY( dst_ptr, src_ptr, length );

    LSA_UNUSED_ARG( sys_ptr );
}
#endif


/*=============================================================================
 * function name:  GSY_ALLOC_TIMER
 *
 * function:       allocate a timer
 *
 * parameters:     LSA_UINT16  ...  *  ret_val_ptr:
 *                           return value: LSA_RET_OK            timer has been
 *                                                               allocated
 *                                         LSA_RET_ERR_NO_TIMER  no timer has
 *                                                               been allocated
 *                 LSA_UINT16  ...  *  timer_id_ptr:
 *                           return value: pointer to id of timer
 *                 LSA_UINT16          timer_type:  LSA_TIMER_TYPE_ONE_SHOT or
 *                                                  LSA_TIMER_TYPE_CYCLIC
 *                 LSA_UINT16          time_base:   LSA_TIME_BASE_1MS,
 *                                                  LSA_TIME_BASE_10MS,
 *                                                  LSA_TIME_BASE_100MS,
 *                                                  LSA_TIME_BASE_1S,
 *                                                  LSA_TIME_BASE_10S or
 *                                                  LSA_TIME_BASE_100S
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_ALLOC_TIMER
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_ALLOC_TIMER(
    LSA_UINT16  GSY_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_UINT16  GSY_LOCAL_MEM_ATTR *  timer_id_ptr,
    LSA_UINT16                           timer_type,
    LSA_UINT16                           time_base
)
{
    LSAS_ALLOC_TIMER(ret_val_ptr, timer_id_ptr, timer_type, time_base,
                    gsy_timeout, LSA_COMP_ID_GSY);
}
#endif

/*=============================================================================
 * function name:  GSY_START_TIMER
 *
 * function:       start a timer
 *
 * parameters:     LSA_UINT16  ...  *  ret_val_ptr:
 *                   return value: LSA_RET_OK                  timer has been
 *                                                             started
 *                                 LSA_RET_OK_TIMER_RESTARTED  timer has been
 *                                                             restarted
 *                                 LSA_RET_ERR_PARAM           timer hasn�t
 *                                                             been started
 *                                                             because of wrong
 *                                                             timer-id
 *                                 After the expiration of the running time
 *                                 system will call gsy_timeout().
 *                 LSA_UINT16          timer_id:  id of timer
 *                 LSA_USER_ID_TYPE    user_id:   id of GSY
 *                 LSA_UINT16          time:      running time
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_START_TIMER
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_START_TIMER(
    LSA_UINT16        GSY_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_UINT16                                 timer_id,
    LSA_USER_ID_TYPE                           user_id,
    LSA_UINT16                                 time
)
{
    LSAS_START_TIMER(ret_val_ptr, timer_id, user_id,
                    time, LSA_COMP_ID_GSY);
}
#endif

/*=============================================================================
 * function name:  GSY_STOP_TIMER
 *
 * function:       stop a timer
 *
 * parameters:     LSA_UINT16  ...  *  ret_val_ptr:
 *                   return value: LSA_RET_OK                    timer has been
 *                                                               stopped
 *                                 LSA_RET_OK_TIMER_NOT_RUNNING  timer was not
 *                                                               running; timer
 *                                                               is stopped
 *                                 LSA_RET_ERR_PARAM             timer hasn�t
 *                                                               stopped
 *                                                               because
 *                                                               of wrong
 *                                                               timer-id
 *                 LSA_UINT16          timer_id:  id of timer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_STOP_TIMER
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_STOP_TIMER(
    LSA_UINT16  GSY_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_UINT16                           timer_id
)
{
    LSAS_STOP_TIMER(ret_val_ptr, timer_id, LSA_COMP_ID_GSY);
}
#endif

/*=============================================================================
 * function name:  GSY_FREE_TIMER
 *
 * function:       free a timer
 *
 * parameters:     LSA_UINT16  ...  *  ret_val_ptr:
 *                return value: LSA_RET_OK                    timer has been
 *                                                            deallocated
 *                              LSA_RET_ERR_TIMER_IS_RUNNING  because timer is
 *                                                            running timer has
 *                                                            not been
 *                                                            deallocated
 *                              LSA_RET_ERR_PARAM             no deallocation
 *                                                            because of wrong
 *                                                            timer-id
 *                 LSA_UINT16          timer_id:  id of timer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_FREE_TIMER
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_FREE_TIMER(
    LSA_UINT16  GSY_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_UINT16                           timer_id
)
{
    LSAS_FREE_TIMER(ret_val_ptr, timer_id, LSA_COMP_ID_GSY);
}
#endif

/*=============================================================================
 * function name:  GSY_ENTER
 *
 * function:       set reentrance lock
 *
 * parameters:     LSA_VOID
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_ENTER(
    LSA_VOID
)
{
    LSAS_GSY_ENTER();
}

/*=============================================================================
 * function name:  GSY_EXIT
 *
 * function:       cancel reentrance lock
 *
 * parameters:     LSA_VOID
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_EXIT(
    LSA_VOID
)
{
    LSAS_GSY_EXIT();
}

/*=============================================================================
 * function name:  GSY_FATAL_ERROR
 *
 * function:       notify a fatal-error
 *
 * parameters:     LSA_UINT16                   length:     number of bytes
 *                 GSY_FATAL_ERROR_PTR_TYPE  error_ptr:  pointer to
 *                                                          LSA_FATAL_ERROR
 *
 * return value:   LSA_VOID
 *===========================================================================*/

/* provided as macro in gsy_cfg.h */
#ifndef GSY_FATAL_ERROR
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_FATAL_ERROR(LSA_UINT16 length, GSY_FATAL_ERROR_PTR_TYPE  error_ptr)
{
     LSAS_FATAL_ERROR( error_ptr);
}
#endif

/******************************************************************************
 *                                                                            *
 *                                                                            *
 *  function prototypes in gsy_low.h                                          *
 *                                                                            *
 *                                                                            *
 ******************************************************************************/

/*====  out functions  =====*/

/*=============================================================================
 * function name:  GSY_OPEN_CHANNEL_LOWER
 *
 * function:       open a communication channel to lower layer (EDD)
 *
 * parameters:     GSY_LOWER_RQB_PTR_TYPE  lower_rqb_ptr:  pointer to lower-
 *                                                            RQB
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *
 * RQB-parameters:
 *     RQB-header:
 *     LSA_OPCODE_TYPE    opcode:        *_OPC_OPEN_CHANNEL
 *     LSA_HANDLE_TYPE    handle:        channel-handle of prefix
 *     LSA_USER_ID_TYPE   user-id:       id of prefix
 *     RQB-args:
 *     LSA_HANDLE_TYPE    handle_upper:  channel-handle of prefix
 *     LSA_SYS_PATH_TYPE  sys_path:      system-path of channel
 *     LSA_VOID  LSA_FCT_PTR(GSY_LOWER_IN_FCT_ATTR, gsy_request_lower_done_ptr)
 *                                    (GSY_LOWER_RQB_PTR_TYPE lower_rqb_ptr)
 *                                       pointer to callback-function
 *
 * RQB-return values via callback-function:
 *     RQB-args:
 *     LSA_HANDLE_TYPE    handle:        channel-handle of lower layer
 *     All other RQB-parameters are unchanged.
 *===========================================================================*/
#ifndef GSY_OPEN_CHANNEL_LOWER
LSA_VOID  GSY_LOWER_OUT_FCT_ATTR  GSY_OPEN_CHANNEL_LOWER(
    GSY_LOWER_RQB_PTR_TYPE     lower_rqb_ptr,
    LSA_SYS_PTR_TYPE           sys_ptr
)
{
    LSA_UNUSED_ARG( sys_ptr );

    ((EDD_UPPER_OPEN_CHANNEL_PTR_TYPE)EDD_RQB_GET_PPARAM(lower_rqb_ptr))->Cbf
        = lsas_edd_to_gsy_cbf;
#ifdef LSAS_CFG_USE_EDDI
    LSAS_RQB_EDD_REQUEST(lower_rqb_ptr, eddi_open_channel);
#endif
#ifdef LSAS_CFG_USE_EDDP
    LSAS_RQB_EDD_REQUEST(lower_rqb_ptr, eddp_open_channel);
#endif
#ifdef LSAS_CFG_USE_EDDS
    LSAS_RQB_EDD_REQUEST(lower_rqb_ptr, edds_open_channel);
#endif
}
#endif

/*=============================================================================
 * function name:  GSY_CLOSE_CHANNEL_LOWER
 *
 * function:       close a communication channel to lower layer
 *
 * parameters:     GSY_LOWER_RQB_PTR_TYPE  lower_rqb_ptr:  pointer to lower-
 *                                                            RQB
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *
 * RQB-parameters:
 *     RQB-header:
 *     LSA_OPCODE_TYPE   opcode:   *_OPC_CLOSE_CHANNEL
 *     LSA_HANDLE_TYPE   handle:   channel-handle of lower layer
 *     LSA_USER_ID_TYPE  user-id:  id of prefix
 *
 * RQB-return values via callback-function:
 *     RQB-header:
 *     LSA_HANDLE_TYPE   handle:    channel-handle of prefix
 *     All other RQB-parameters are unchanged.
 *===========================================================================*/
#ifndef GSY_CLOSE_CHANNEL_LOWER
LSA_VOID  GSY_LOWER_OUT_FCT_ATTR  GSY_CLOSE_CHANNEL_LOWER(
    GSY_LOWER_RQB_PTR_TYPE     lower_rqb_ptr,
    LSA_SYS_PTR_TYPE           sys_ptr
)
{
    LSA_UNUSED_ARG( sys_ptr );
#ifdef LSAS_CFG_USE_EDDI
    LSAS_RQB_EDD_REQUEST(lower_rqb_ptr, eddi_close_channel);
#endif
#ifdef LSAS_CFG_USE_EDDP
    LSAS_RQB_EDD_REQUEST(lower_rqb_ptr, eddp_close_channel);
#endif
#ifdef LSAS_CFG_USE_EDDS
    LSAS_RQB_EDD_REQUEST(lower_rqb_ptr, edds_close_channel);
#endif
}
#endif

/*=============================================================================
 * function name:  GSY_REQUEST_LOWER
 *
 * function:       send a request to a communication channel to lower layer
 *
 * parameters:     GSY_LOWER_RQB_PTR_TYPE  lower_rqb_ptr:  pointer to lower-
 *                                                            RQB
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *
 * RQB-parameters:
 *     RQB-header:
 *     LSA_OPCODE_TYPE   opcode:    *_OPC_*
 *     LSA_HANDLE_TYPE   handle:    channel-handle of lower layer
 *     LSA_USER_ID_TYPE  user-id:   id of prefix
 *     RQB-args:                    Depend on kind of request.
 *
 * RQB-return values via callback-function:
 *     RQB-header:
 *     LSA_OPCODE_TYPE   opcode:    *_OPC_*
 *     LSA_HANDLE_TYPE   handle:    channel-handle of prefix
 *     LSA_USER_ID_TYPE  user-id:   id of prefix
 *     All other RQB-parameters depend on kind of request.
 *===========================================================================*/
#ifndef GSY_REQUEST_LOWER
LSA_VOID  GSY_LOWER_OUT_FCT_ATTR  GSY_REQUEST_LOWER(
    GSY_LOWER_RQB_PTR_TYPE     lower_rqb_ptr,
    LSA_SYS_PTR_TYPE           sys_ptr
)
{
    LSA_UNUSED_ARG( sys_ptr );

#ifdef LSAS_CFG_USE_EDDI
            LSAS_RQB_EDD_REQUEST_HIGH(lower_rqb_ptr, eddi_request);
#endif
#ifdef LSAS_CFG_USE_EDDP
            LSAS_RQB_EDD_REQUEST_HIGH(lower_rqb_ptr, eddp_request);
#endif
#ifdef LSAS_CFG_USE_EDDS
            LSAS_RQB_EDD_REQUEST_HIGH(lower_rqb_ptr, edds_request);
#endif
}
#endif

/*=============================================================================
 * function name:  GSY_ALLOC_LOWER_RQB
 *
 * function:       allocate a lower-RQB
 *
 * parameters:     GSY_LOWER_RQB_PTR_TYPE  ...  *  lower_rqb_ptr_ptr:
 *                                   return value: pointer to lower-RQB-pointer
 *                                                 or LSA_NULL: The allocation
 *                                                 will be asynchronous, see
 *                                                 gsy_lower_rqb().
 *                 LSA_USER_ID_TYPE                   user_id:  id of prefix
 *                 LSA_UINT16                         length:   length of RQB
 *                 LSA_SYS_PTR_TYPE                   sys_ptr:  system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_ALLOC_LOWER_RQB
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_ALLOC_LOWER_RQB(
    GSY_LOWER_RQB_PTR_TYPE  GSY_LOCAL_MEM_ATTR *        lower_rqb_ptr_ptr,
    LSA_USER_ID_TYPE                                    user_id,
    LSA_UINT16                                          length,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(lower_rqb_ptr_ptr, length, LSA_COMP_ID_GSY);

    LSA_UNUSED_ARG( user_id );
    LSA_UNUSED_ARG( sys_ptr );
}
#endif

/*=============================================================================
 * function name:  GSY_FREE_LOWER_RQB
 *
 * function:       free a lower-RQB
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      lower-RQB or
 *                                                      wrong system-pointer
 *                 GSY_LOWER_RQB_PTR_TYPE  lower_rqb_ptr:  pointer to lower-
 *                                                            RQB
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_FREE_LOWER_RQB
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_FREE_LOWER_RQB(
    LSA_UINT16                 GSY_LOCAL_MEM_ATTR *  ret_val_ptr,
    GSY_LOWER_RQB_PTR_TYPE                           lower_rqb_ptr,
    LSA_SYS_PTR_TYPE                                 sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_rqb_ptr, LSA_COMP_ID_GSY);

    LSA_UNUSED_ARG( sys_ptr );
}
#endif

/*=============================================================================
 * function name:  GSY_ALLOC_LOWER_MEM
 *
 * function:       allocate a lower-memory
 *
 * parameters:     GSY_LOWER_MEM_PTR_TYPE  ...  *  lower_mem_ptr_ptr:
 *                                   return value: pointer to lower-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: The allocation
 *                                                 will be asynchronous, see
 *                                                 gsy_lower_mem().
 *                 LSA_USER_ID_TYPE                   user_id:  id of prefix
 *                 LSA_UINT16                         length:   length of
 *                                                              memory
 *                 LSA_SYS_PTR_TYPE  sys_ptr:                   system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_ALLOC_LOWER_MEM
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_ALLOC_LOWER_MEM(
    GSY_LOWER_MEM_PTR_TYPE  GSY_LOCAL_MEM_ATTR *     lower_mem_ptr_ptr,
    LSA_USER_ID_TYPE                                 user_id,
    LSA_UINT16                                       length,
    LSA_SYS_PTR_TYPE                                 sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(lower_mem_ptr_ptr, length, LSA_COMP_ID_GSY);

    LSA_UNUSED_ARG( user_id );
    LSA_UNUSED_ARG( sys_ptr );
}
#endif

/*=============================================================================
 * function name:  GSY_FREE_LOWER_MEM
 *
 * function:       free a lower-memory
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      lower-memory or
 *                                                      wrong system-pointer
 *                 GSY_LOWER_MEM_PTR_TYPE  lower_mem_ptr:  pointer to lower-
 *                                                            memory
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_FREE_LOWER_MEM
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_FREE_LOWER_MEM(
    LSA_UINT16                 GSY_LOCAL_MEM_ATTR *  ret_val_ptr,
    GSY_LOWER_MEM_PTR_TYPE                           lower_mem_ptr,
    LSA_SYS_PTR_TYPE                                 sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_mem_ptr, LSA_COMP_ID_GSY);

    LSA_UNUSED_ARG( sys_ptr );
}
#endif

/*=============================================================================
 * function name:  GSY_ALLOC_LOWER_TXMEM
 *
 * function:       allocate a lower memory for send PDUs
 *
 * parameters:     GSY_LOWER_TXMEM_PTR_TYPE  ...  *     lower_mem_ptr_ptr:
 *                                   return value: pointer to lower-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: The allocation
 *                                                 will be asynchronous, see
 *                                                 gsy_lower_mem().
 *                 LSA_USER_ID_TYPE                   user_id:  id of dcp
 *                 LSA_UINT16                         length:   length of
 *                                                              memory
 *                 LSA_SYS_PTR_TYPE  sys_ptr:                   system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_ALLOC_LOWER_TXMEM
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_ALLOC_LOWER_TXMEM(
    GSY_LOWER_TXMEM_PTR_TYPE  GSY_LOCAL_MEM_ATTR *      lower_mem_ptr_ptr,
    LSA_USER_ID_TYPE                                    user_id,
    LSA_UINT16                                          length,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    LSAS_ALLOC_PACKET_MEM(lower_mem_ptr_ptr, length, LSA_COMP_ID_GSY);

    LSA_UNUSED_ARG( user_id );
    LSA_UNUSED_ARG( sys_ptr );
}
#endif

/*=============================================================================
 * function name:  GSY_FREE_LOWER_TXMEM
 *
 * function:       free a lower memory for send PDUs
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      lower-memory or
 *                                                      wrong system-pointer
 *                 GSY_LOWER_MEM_PTR_TYPE  lower_mem_ptr:     pointer to lower-
 *                                                            memory
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_FREE_LOWER_TXMEM
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_FREE_LOWER_TXMEM(
    LSA_UINT16                 GSY_LOCAL_MEM_ATTR *     ret_val_ptr,
    GSY_LOWER_TXMEM_PTR_TYPE                            lower_mem_ptr,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    LSAS_FREE_PACKET_MEM(ret_val_ptr, lower_mem_ptr, LSA_COMP_ID_GSY);
    *(ret_val_ptr) = LSA_RET_OK;
}
#endif

/*=============================================================================
 * function name:  GSY_ALLOC_LOWER_RXMEM
 *
 * function:       allocate a lower memory for receive PDUs
 *
 * parameters:     GSY_LOWER_RXMEM_PTR_TYPE  ...  *     lower_mem_ptr_ptr:
 *                                   return value: pointer to lower-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: The allocation
 *                                                 will be asynchronous, see
 *                                                 gsy_lower_mem().
 *                 LSA_USER_ID_TYPE                   user_id:  id of dcp
 *                 LSA_UINT16                         length:   length of
 *                                                              memory
 *                 LSA_SYS_PTR_TYPE  sys_ptr:                   system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_ALLOC_LOWER_RXMEM
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_ALLOC_LOWER_RXMEM(
    GSY_LOWER_RXMEM_PTR_TYPE  GSY_LOCAL_MEM_ATTR *      lower_mem_ptr_ptr,
    LSA_USER_ID_TYPE                                    user_id,
    LSA_UINT16                                          length,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    LSAS_ALLOC_PACKET_MEM(lower_mem_ptr_ptr, length, LSA_COMP_ID_GSY);

    LSA_UNUSED_ARG( user_id );
    LSA_UNUSED_ARG( sys_ptr );
}
#endif

/*=============================================================================
 * function name:  GSY_FREE_LOWER_RXMEM
 *
 * function:       free a lower memory for receive PDUs
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      lower-memory or
 *                                                      wrong system-pointer
 *                 GSY_LOWER_RXMEM_PTR_TYPE  lower_mem_ptr:     pointer to lower-
 *                                                            memory
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef GSY_FREE_LOWER_RXMEM
LSA_VOID  GSY_SYSTEM_OUT_FCT_ATTR  GSY_FREE_LOWER_RXMEM(
    LSA_UINT16                 GSY_LOCAL_MEM_ATTR *     ret_val_ptr,
    GSY_LOWER_RXMEM_PTR_TYPE                            lower_mem_ptr,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    LSAS_FREE_PACKET_MEM(ret_val_ptr, lower_mem_ptr, LSA_COMP_ID_GSY);
    *( ret_val_ptr ) = LSA_RET_OK;
}
#endif

/*=============================================================================
 * function name:  GSY_GET_TIMESTAMP
 *
 * function:       get timestamp from ERTEC register
 *
 *
 * parameters:     -
 *                 sys_ptr       of system channel: for channel identification
 *
 *
 * return value:   timestamp
 *===========================================================================*/
#ifndef GSY_GET_TIMESTAMP
LSA_UINT32  GSY_SYSTEM_OUT_FCT_ATTR  GSY_GET_TIMESTAMP(LSA_SYS_PTR_TYPE sys_ptr)
{
#ifdef LSAS_CFG_USE_EDDI
    LSA_UINT32 timestamp;

    LSA_UNUSED_ARG( sys_ptr );



    #ifdef LSAS_CFG_USE_EDDS
        timestamp = 0;
    #else
        {
    EDDI_HANDLE hDDB = lsas_com_get_hddb();

    if( hDDB != 0 ) {

        LSA_UINT16 rc = eddi_GetTimeStamp(hDDB, &timestamp);

        if( rc != EDD_STS_OK ) {

            LSAS_ASSERT( 0);
            timestamp = 0; // unreachable
        }
    }
        else {

            LSAS_ASSERT( 0);
            timestamp = 0; // unreachable
        }
        }
    #endif

    LSA_UNUSED_ARG(sys_ptr);

    return timestamp;

    /*return( ((IRTE_SW_MAKRO_REV6_T *)IRTE_BASE)->s11000_Zyklussync.x11418_Clock_Cnt_Val );*/
#endif
#ifdef LSAS_CFG_USE_EDDP
    // not used with KRISC32??
    LSA_UINT32 ethsw_get_time(LSA_UINT8 timer);
    return ethsw_get_time(0);
#endif
#ifdef LSAS_CFG_USE_EDDS
    // not used
    return 0;
#endif

}
#endif

LSA_INT32  GSY_SYSTEM_OUT_FCT_ATTR  GSY_GET_EXT_PLL_OFFSET(LSA_UINT8 SyncId, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSA_INT32   RealOffsetNs = 0;
#ifdef LSAS_CFG_USE_EDDI
    LSA_UNUSED_ARG(sys_ptr);

    switch( eddi_SyncGetRealOffsetNs(lsas_com_get_hddb(), &RealOffsetNs) )
    {
        case EDD_STS_OK:
        {
            break;
        }
        case EDD_STS_ERR_HW:
        {
            RealOffsetNs = (LSA_INT32) -32000;
            break;
        }
        default:
        {
            LSAS_ASSERT( 0);
        }
    }
#endif
#ifdef LSAS_CFG_USE_EDDP
    // not used with KRISC32??
#endif
#ifdef LSAS_CFG_USE_EDDS
    // not used
#endif
    return(RealOffsetNs);
}


LSA_UINT16 gsy_version ( LSA_UINT16 length, LSA_VERSION_TYPE *version )
{
  LSA_UINT8 *p_version, *p_v;
  LSA_UINT16 i;

  /* Initialize version struct */
  LSA_VERSION_TYPE v = {
                        PNIO_LSA_COMPONENT_ID, {'L','S','A','_','G','S','Y','\0','\0','\0'},
                        PNIO_KIND,             PNIO_VERSION,
                        PNIO_DISTRIBUTION,     PNIO_FIX,
                        PNIO_HOTFIX,           PNIO_PROJECT_NUMBER,
                        PNIO_INCREMENT,        PNIO_INTEGRATION_COUNTER,
                        PNIO_GEN_COUNTER
                       };

  /* Adapt length if necessary */
  if (length > sizeof(v))
  {
    length = sizeof(v);
  }

  /* Initialize pointers and copy version structure */
  p_version = (LSA_UINT8 *) version;
  p_v       = (LSA_UINT8 *) &v;
  for ( i=0;i<length; i++ ) { p_version[i] = p_v[i]; };

  return (LSA_UINT16)(sizeof(v) - length);
}

/*** end of file *************************************************************/
 
/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
