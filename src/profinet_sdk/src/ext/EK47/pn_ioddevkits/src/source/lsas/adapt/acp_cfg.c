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
/*  F i l e               &F: acp_cfg.c                                 :F&  */
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

/* ****************************************************************************
*
*   H i s t o r y :
*   see /_doc/im151_3pn_hist.doc
*
* ****************************************************************************/

#define LSAS_MODULE_ID  11

//#include "acp_inc.h"

#include "lsas_inc.h"
#include "lsas_int.h"

/*------------------------------------------------------------------------------
//    Upper Layer
//----------------------------------------------------------------------------*/

/*=============================================================================
 * function name:  ACP_REQUEST_UPPER_DONE
 *
 * function:       returns a finished request to the upper layer
 *
 * parameters:     ACP_UPPER_CALLBACK_FCT_PTR_TYPE
 *                                         acp_request_upper_done_ptr:
 *                     pointer to acp_request_upper_done_ptr, given by
 *                     acp_open_channel()
 *
 *                 ACP_UPPER_RQB_PTR_TYPE  upper_rqb_ptr:  pointer to upper-RQB
 *                 LSA_SYS_PTR_TYPE        sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *
 *===========================================================================*/

LSA_VOID    ACP_REQUEST_UPPER_DONE(
    ACP_UPPER_CALLBACK_FCT_PTR_TYPE  acp_request_upper_done_ptr,
    ACP_UPPER_RQB_PTR_TYPE           upper_rqb_ptr,
    LSA_SYS_PTR_TYPE                 sys_ptr
)
{
    acp_request_upper_done_ptr(upper_rqb_ptr);
}

#ifndef ACP_ALLOC_UPPER_RQB
# error "must be a macro because of file and line info!"
#endif

#ifndef ACP_FREE_UPPER_RQB
# error "must be a macro because of file and line info!"
#endif

#ifndef ACP_ALLOC_UPPER_MEM
# error "must be a macro because of file and line info!"
#endif

#ifndef ACP_FREE_UPPER_MEM
# error "must be a macro because of file and line info!"
#endif


LSA_VOID   ACP_REQUEST_LOCAL(ACP_UPPER_RQB_PTR_TYPE  upper_rqb_ptr)
{
    LSAS_RQB_ACP_REQUEST( upper_rqb_ptr, acp_request);
}


/*------------------------------------------------------------------------------
//    Lower Layer
//----------------------------------------------------------------------------*/

#ifndef ACP_ALLOC_LOWER_RQB
# error "must be a macro because of file and line info!"
#endif

#ifndef ACP_FREE_LOWER_RQB
# error "must be a macro because of file and line info!"
#endif

#ifndef ACP_ALLOC_LOWER_MEM
# error "must be a macro because of file and line info!"
#endif

#ifndef ACP_FREE_LOWER_MEM
# error "must be a macro because of file and line info!"
#endif

#ifndef ACP_ALLOC_NRT_SEND_MEM
# error "must be a macro because of file and line info!"
#endif

#ifndef ACP_FREE_NRT_SEND_MEM
# error "must be a macro because of file and line info!"
#endif

#ifndef ACP_ALLOC_NRT_RECV_MEM
# error "must be a macro because of file and line info!"
#endif

#ifndef ACP_FREE_NRT_RECV_MEM
# error "must be a macro because of file and line info!"
#endif

LSA_VOID  ACP_OPEN_CHANNEL_LOWER(ACP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSA_UNUSED_ARG(sys_ptr);

    DCP_ETH_RQB_CBF_PTR_SET(lower_rqb_ptr, lsas_edd_to_acp_cbf);
#ifdef LSAS_CFG_USE_EDDI
    LSAS_RQB_EDD_REQUEST( lower_rqb_ptr, eddi_open_channel);
#endif
#ifdef LSAS_CFG_USE_EDDP
    LSAS_RQB_EDD_REQUEST( lower_rqb_ptr, eddp_open_channel);
#endif
#ifdef LSAS_CFG_USE_EDDS
    LSAS_RQB_EDD_REQUEST( lower_rqb_ptr, edds_open_channel);
#endif
}


LSA_VOID  ACP_CLOSE_CHANNEL_LOWER(ACP_LOWER_RQB_PTR_TYPE lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSA_UNUSED_ARG(sys_ptr);
#ifdef LSAS_CFG_USE_EDDI
    LSAS_RQB_EDD_REQUEST( lower_rqb_ptr, eddi_close_channel);
#endif

#ifdef LSAS_CFG_USE_EDDP
    LSAS_RQB_EDD_REQUEST( lower_rqb_ptr, eddp_close_channel);
#endif

#ifdef LSAS_CFG_USE_EDDS
    LSAS_RQB_EDD_REQUEST( lower_rqb_ptr, edds_close_channel);
#endif

}


LSA_VOID  ACP_REQUEST_LOWER(ACP_LOWER_RQB_PTR_TYPE lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSA_UNUSED_ARG(sys_ptr);

    switch( ACP_LL_GET_SERVICE(lower_rqb_ptr) )
    {
#if 0
        case EDD_SRV_NRT_SEND:
          case EDD_SRV_NRT_RECV:
          case EDD_SRV_NRT_CANCEL:
        {
            edd_request(lower_rqb_ptr);
        }
        break;
#endif
        case EDD_SRV_NRT_SEND_TIMESTAMP:
        {
            LSAS_ASSERT(0);
        }
        break;

        default:
        {
#ifdef LSAS_CFG_USE_EDDI
            if(LSAS_EDD_CHECK_RCV(lower_rqb_ptr, eddi_request))
            {
                LSAS_RQB_EDD_REQUEST( lower_rqb_ptr, eddi_request);
            }
            /* LSAS_EDD_CHECK_RCV must care about rqb routing if it returns false */
#endif
#ifdef LSAS_CFG_USE_EDDP
            if(LSAS_EDD_CHECK_RCV(lower_rqb_ptr, eddp_request))
            {
                LSAS_RQB_EDD_REQUEST( lower_rqb_ptr, eddp_request);
            }
            /* LSAS_EDD_CHECK_RCV must care about rqb routing if it returns false */
#endif
#ifdef LSAS_CFG_USE_EDDS
            if(LSAS_EDD_CHECK_RCV(lower_rqb_ptr, edds_request))
            {
                LSAS_RQB_EDD_REQUEST( lower_rqb_ptr, edds_request);
            }
            /* LSAS_EDD_CHECK_RCV must care about rqb routing if it returns false */
#endif
        }
    }
}


LSA_VOID  ACP_SYNC_REQUEST_LOWER(ACP_LOWER_RQB_PTR_TYPE lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSA_UNUSED_ARG(sys_ptr);

    /* !!! ??? Reentrantschutz */
#ifdef LSAS_CFG_USE_EDDI
    eddi_request(lower_rqb_ptr);
#endif
#ifdef LSAS_CFG_USE_EDDP
    eddp_request(lower_rqb_ptr);
#endif
#ifdef LSAS_CFG_USE_EDDS
    edds_request(lower_rqb_ptr);
#endif
    }

/*------------------------------------------------------------------------------
//    System Layer
//----------------------------------------------------------------------------*/

#ifndef ACP_ALLOC_LOCAL_MEM
# error "must be a macro because of file and line info!"
#endif

#ifndef ACP_FREE_LOCAL_MEM
# error "must be a macro because of file and line info!"
#endif

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

LSA_VOID  ACP_ALLOC_TIMER(
    LSA_UINT16   *  ret_val_ptr,
    LSA_UINT16   *  timer_id_ptr,
    LSA_UINT16                        timer_type,
    LSA_UINT16                        time_base)
{
    LSAS_ALLOC_TIMER(ret_val_ptr, timer_id_ptr, timer_type, time_base, acp_timeout, LSA_COMP_ID_ACP);
}


LSA_VOID  ACP_START_TIMER(
    LSA_UINT16         *  ret_val_ptr,
    LSA_UINT16                              timer_id,
    LSA_USER_ID_TYPE                        user_id,
    LSA_UINT16                              time)
{
    LSAS_START_TIMER(ret_val_ptr, timer_id, user_id, time, LSA_COMP_ID_ACP);
}


LSA_VOID  ACP_STOP_TIMER(
    LSA_UINT16   *  ret_val_ptr,
    LSA_UINT16                        timer_id)
{
    LSAS_STOP_TIMER(ret_val_ptr, timer_id, LSA_COMP_ID_ACP);
}


LSA_VOID  ACP_FREE_TIMER(
    LSA_UINT16   *  ret_val_ptr,
    LSA_UINT16                        timer_id)
{
    LSAS_FREE_TIMER(ret_val_ptr, timer_id, LSA_COMP_ID_ACP);
}


/*----------------------------------------------------------------------------*/

LSA_VOID  ACP_ENTER(LSA_VOID)
{
    LSAS_OS_LOCK_ON_PNIO_TASK();
}


LSA_VOID  ACP_EXIT(LSA_VOID)
{
    LSAS_OS_RELEASE_PNIO_TASK();
}

/*----------------------------------------------------------------------------*/


LSA_VOID  ACP_FATAL_ERROR(LSA_UINT16 length, ACP_FATAL_ERROR_PTR_TYPE error_ptr)
{
      LSA_UNUSED_ARG(length);
    LSAS_FATAL_ERROR( error_ptr);
}


/*----------------------------------------------------------------------------*/

LSA_VOID   ACP_PUT8_HTON(LSA_VOID_PTR_TYPE base, LSA_INT offset, LSA_UINT8 val)
{
    LSA_UINT8 *ptr = base;
    ptr += offset;
    ACP_MEMCPY (ptr, & val, sizeof(val));
}

LSA_UINT8   ACP_GET8_NTOH(LSA_VOID_PTR_TYPE base, LSA_INT offset)
{
    return *(LSA_UINT8*)( (LSA_UINT8*)base + offset );
}

LSA_VOID   ACP_PUT16_HTON(LSA_VOID_PTR_TYPE base, LSA_INT offset, LSA_UINT16 val)
{
    LSA_UINT8 *ptr = base;
    ptr += offset;
    /* val = htons (val); */
    val = LSAS_HTONS (val);
    ACP_MEMCPY (ptr, & val, sizeof(val));
}

LSA_UINT16   ACP_GET16_NTOH(LSA_VOID_PTR_TYPE base, LSA_INT offset)
{
    LSA_UINT16  val;
    LSA_UINT8   *ptr = base;
    ptr += offset;
    ACP_MEMCPY (& val, ptr, sizeof(val));
    /* val = ntohs (val); */
    val = LSAS_NTOHS (val);
    return val;
}

LSA_VOID   ACP_PUT32_HTON(LSA_VOID_PTR_TYPE base, LSA_INT offset, LSA_UINT32 val)
{
    LSA_UINT8 *ptr = base;
    ptr += offset;
    /* val = htonl (val); */
    val = LSAS_HTONL (val);
    ACP_MEMCPY (ptr, & val, sizeof(val));
}

LSA_UINT32   ACP_GET32_NTOH(LSA_VOID_PTR_TYPE base, LSA_INT offset)
{
    LSA_UINT32  val;
    LSA_UINT8   *ptr = base;
    ptr += offset;
    ACP_MEMCPY (& val, ptr, sizeof(val));
    /* val = ntohl (val); */
    val = LSAS_NTOHL (val);
    return val;
}


LSA_UINT16 acp_version ( LSA_UINT16 length, LSA_VERSION_TYPE *version )
{
  LSA_UINT8 *p_version, *p_v;
  LSA_UINT16 i;

  /* Initialize version struct */
  LSA_VERSION_TYPE v = {
                        PNIO_LSA_COMPONENT_ID, {'L','S','A','_','A','C','P','\0','\0','\0'},
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

/*****************************************************************************/
/*  end of file.                                                             */
/*****************************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
