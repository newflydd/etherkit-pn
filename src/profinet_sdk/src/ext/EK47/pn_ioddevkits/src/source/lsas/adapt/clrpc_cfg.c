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
/*  F i l e               &F: clrpc_cfg.c                               :F&  */
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

#define LSAS_MODULE_ID  13

//#include "clrpc_inc.h" /* see clrpc_cfg.h for platform things */

#include "lsas_inc.h"
#include "lsas_int.h"

#ifdef CLRPC_PLATFORM_PC
# include <sys/timeb.h>
# include <sys/types.h>
# include <time.h>
#endif

/*-----------------------------------------------------------------------------
//---------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/

void clrpc_pcpnio_startstop(int start)
{
    if (start) {
        static CLRPC_INIT_TYPE clrpc_init_param; // not defined by LSA if necessarily static. static is safe.

        clrpc_init_param.oneshot_lsa_time_base   = LSA_TIME_BASE_100MS;
        clrpc_init_param.oneshot_lsa_time_factor = 1;
        clrpc_init(&clrpc_init_param);
    }
    else {
        clrpc_undo_init();
    }
}

/*-----------------------------------------------------------------------------
//    System Layer (prototypes in clrpc_sys.h)
//---------------------------------------------------------------------------*/

#ifndef CLRPC_GET_PATH_INFO
LSA_VOID    CLRPC_GET_PATH_INFO(
    LSA_UINT16               *  ret_val_ptr,
    LSA_SYS_PTR_TYPE         *  sys_ptr_ptr,
    CLRPC_DETAIL_PTR_TYPE    *  detail_ptr_ptr,
    LSA_SYS_PATH_TYPE                               path
#ifdef CLRPC_PLATFORM_PC
); /* by design a function in main! */
#else
)
{
    LSA_GET_PATH_INFO(ret_val_ptr, sys_ptr_ptr, detail_ptr_ptr, path,
                      LSAS_PATH_LAYER_CLRPC);
}
#endif
#else
# error "by design a function"
#endif


#ifndef CLRPC_RELEASE_PATH_INFO
LSA_VOID    CLRPC_RELEASE_PATH_INFO(
    LSA_UINT16              *  ret_val_ptr,
    LSA_SYS_PTR_TYPE                               sys_ptr,
    CLRPC_DETAIL_PTR_TYPE                          detail_ptr
#ifdef CLRPC_PLATFORM_PC
); /* by design a function in main! */
#else
)
{
    LSA_RELEASE_PATH_INFO(ret_val_ptr, sys_ptr, detail_ptr);
}
#endif
#else
# error "by design a function"
#endif

/*---------------------------------------------------------------------------*/

#ifndef CLRPC_ALLOC_LOCAL_MEM
LSA_VOID    CLRPC_ALLOC_LOCAL_MEM(
    CLRPC_LOCAL_MEM_PTR_TYPE   *  local_mem_ptr_ptr,
    LSA_UINT32                                        length
)
{
    LSAS_ALLOC_COMP_MEM(local_mem_ptr_ptr, length, LSA_COMP_ID_CLRPC);
}
#else
# error "by design a function"
#endif

#ifndef CLRPC_FREE_LOCAL_MEM
LSA_VOID    CLRPC_FREE_LOCAL_MEM(
    LSA_UINT16                  *  ret_val_ptr,
    CLRPC_LOCAL_MEM_PTR_TYPE                           local_mem_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, local_mem_ptr, LSA_COMP_ID_CLRPC);
}
#else
# error "by design a function"
#endif

/*---------------------------------------------------------------------------*/

#ifndef CLRPC_ALLOC_TIMER
LSA_VOID
CLRPC_ALLOC_TIMER(
    LSA_UINT16   *  ret_val_ptr,
    LSA_UINT16   *  timer_id_ptr,
    LSA_UINT16                          timer_type,
    LSA_UINT16                          time_base)
{
    LSAS_ALLOC_TIMER(ret_val_ptr, timer_id_ptr, timer_type, time_base,
                    clrpc_timeout, LSA_COMP_ID_CLRPC);
}
#else
# error "by design a function"
#endif

#ifndef CLRPC_START_TIMER
LSA_VOID
CLRPC_START_TIMER(
    LSA_UINT16         *  ret_val_ptr,
    LSA_UINT16                                timer_id,
    LSA_USER_ID_TYPE                          user_id,
    LSA_UINT16                                time)
{
    LSAS_START_TIMER(ret_val_ptr, timer_id, user_id, time,
            LSA_COMP_ID_CLRPC);
}
#else
# error "by design a function"
#endif

#ifndef CLRPC_STOP_TIMER
LSA_VOID
CLRPC_STOP_TIMER(
    LSA_UINT16   *  ret_val_ptr,
    LSA_UINT16                          timer_id)
{
    LSAS_STOP_TIMER(ret_val_ptr, timer_id, LSA_COMP_ID_CLRPC);
}
#else
# error "by design a function"
#endif

#ifndef CLRPC_FREE_TIMER
LSA_VOID
CLRPC_FREE_TIMER(
    LSA_UINT16   *  ret_val_ptr,
    LSA_UINT16                          timer_id)
{
    LSAS_FREE_TIMER(ret_val_ptr, timer_id, LSA_COMP_ID_CLRPC);
}
#else
# error "by design a function"
#endif

/*---------------------------------------------------------------------------*/

#ifdef CLRPC_ENTER
# error "who defined it? not used!"
#endif

#ifdef CLRPC_EXIT
# error "who defined it? not used!"
#endif


/*-----------------------------------------------------------------------------
//    Upper Layer (prototypes in clrpc_usr.h)
//---------------------------------------------------------------------------*/

#ifndef CLRPC_REQUEST_UPPER_DONE
LSA_VOID  CLRPC_REQUEST_UPPER_DONE(
    CLRPC_UPPER_CALLBACK_FCT_PTR_TYPE  clrpc_request_upper_done_ptr,
    CLRPC_UPPER_RQB_PTR_TYPE           upper_rqb_ptr,
    LSA_SYS_PTR_TYPE                   sys_ptr
)
{
    clrpc_request_upper_done_ptr(upper_rqb_ptr);
}
#else
# error "by design a function"
#endif

#ifndef CLRPC_ALLOC_UPPER_RQB
LSA_VOID    CLRPC_ALLOC_UPPER_RQB(
    CLRPC_UPPER_RQB_PTR_TYPE   *  upper_rqb_ptr_ptr,
    LSA_USER_ID_TYPE                                  user_id,
    LSA_UINT16                                        length,
    LSA_SYS_PTR_TYPE                                  sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(upper_rqb_ptr_ptr, length, LSA_COMP_ID_CLRPC);

    LSA_UNUSED_ARG( user_id );
    LSA_UNUSED_ARG( sys_ptr );
}
#else
# error "by design a function"
#endif

#ifndef CLRPC_FREE_UPPER_RQB
LSA_VOID    CLRPC_FREE_UPPER_RQB(
    LSA_UINT16                  *  ret_val_ptr,
    CLRPC_UPPER_RQB_PTR_TYPE                           upper_rqb_ptr,
    LSA_SYS_PTR_TYPE                                   sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, upper_rqb_ptr, LSA_COMP_ID_CLRPC);

    LSA_UNUSED_ARG( sys_ptr );
}
#else
# error "by design a function"
#endif

/*---------------------------------------------------------------------------*/

#ifndef CLRPC_ALLOC_UPPER_MEM
LSA_VOID    CLRPC_ALLOC_UPPER_MEM(
    CLRPC_UPPER_MEM_PTR_TYPE   *  upper_mem_ptr_ptr,
    LSA_USER_ID_TYPE                                  user_id,
    LSA_UINT32                                        length,
    LSA_SYS_PTR_TYPE                                  sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(upper_mem_ptr_ptr, length, LSA_COMP_ID_CLRPC);

    LSA_UNUSED_ARG( user_id );
    LSA_UNUSED_ARG( sys_ptr );
}

#else
# error "by design a function"
#endif

#ifndef CLRPC_FREE_UPPER_MEM
LSA_VOID    CLRPC_FREE_UPPER_MEM(
    LSA_UINT16                  *  ret_val_ptr,
    CLRPC_UPPER_MEM_PTR_TYPE                           upper_mem_ptr,
    LSA_SYS_PTR_TYPE                                   sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, upper_mem_ptr, LSA_COMP_ID_CLRPC);

    LSA_UNUSED_ARG( sys_ptr );
}
#else
# error "by design a function"
#endif

/*---------------------------------------------------------------------------*/

LSA_VOID    CLRPC_REQUEST_LOCAL(
    CLRPC_UPPER_RQB_PTR_TYPE  upper_rqb_ptr
)
{
    LSAS_RQB_CLRPC_REQUEST( upper_rqb_ptr, clrpc_request);
}

/*-----------------------------------------------------------------------------
//    Lower Layer (prototypes in clrpc_low.h)
//---------------------------------------------------------------------------*/

typedef LSA_VOID LSA_FCT_PTR(/* attr */, sock_request_upper_done_ptr_type)(SOCK_UPPER_RQB_PTR_TYPE upper_rqb_ptr);

#ifndef CLRPC_OPEN_CHANNEL_LOWER
LSA_VOID CLRPC_OPEN_CHANNEL_LOWER(
    CLRPC_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE          sys_ptr)
{
    LSA_UNUSED_ARG( sys_ptr );

    lower_rqb_ptr->args.channel.sock_request_upper_done_ptr = (sock_request_upper_done_ptr_type)lsas_sock_to_clrpc_cbf;

    LSAS_RQB_SOCK_REQUEST(lower_rqb_ptr, sock_open_channel);
}
#else
# error "by design a function"
#endif

#ifndef CLRPC_CLOSE_CHANNEL_LOWER
LSA_VOID CLRPC_CLOSE_CHANNEL_LOWER(
    CLRPC_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE          sys_ptr)
{
    LSA_UNUSED_ARG( sys_ptr );

    LSAS_RQB_SOCK_REQUEST(lower_rqb_ptr, sock_close_channel);
}
#else
# error "by design a function"
#endif

#ifndef CLRPC_REQUEST_LOWER
LSA_VOID CLRPC_REQUEST_LOWER(
    CLRPC_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE          sys_ptr)
{
    LSA_UNUSED_ARG(  sys_ptr );

    LSAS_RQB_SOCK_REQUEST(lower_rqb_ptr, sock_request);
}
#else
# error "by design a function"
#endif

/*---------------------------------------------------------------------------*/

#ifndef CLRPC_ALLOC_LOWER_RQB
LSA_VOID    CLRPC_ALLOC_LOWER_RQB(
    CLRPC_LOWER_RQB_PTR_TYPE   *  lower_rqb_ptr_ptr,
    LSA_USER_ID_TYPE              user_id,
    LSA_UINT16                    length,
    LSA_SYS_PTR_TYPE              sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(lower_rqb_ptr_ptr, length, LSA_COMP_ID_CLRPC);

    LSA_UNUSED_ARG( user_id );
    LSA_UNUSED_ARG( sys_ptr );
}
#endif

#ifndef CLRPC_FREE_LOWER_RQB
LSA_VOID    CLRPC_FREE_LOWER_RQB(
    LSA_UINT16                  *  ret_val_ptr,
    CLRPC_LOWER_RQB_PTR_TYPE       lower_rqb_ptr,
    LSA_SYS_PTR_TYPE               sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_rqb_ptr, LSA_COMP_ID_CLRPC);

    LSA_UNUSED_ARG( sys_ptr );
}
#endif

#ifndef CLRPC_ALLOC_LOWER_MEM
LSA_VOID    CLRPC_ALLOC_LOWER_MEM(
    CLRPC_LOWER_MEM_PTR_TYPE   *  lower_mem_ptr_ptr,
    LSA_USER_ID_TYPE              user_id,
    LSA_UINT16                    length,
    LSA_SYS_PTR_TYPE              sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(lower_mem_ptr_ptr, length, LSA_COMP_ID_CLRPC);

    LSA_UNUSED_ARG( user_id );
    LSA_UNUSED_ARG( sys_ptr );
}
#endif

#ifndef CLRPC_FREE_LOWER_MEM
LSA_VOID    CLRPC_FREE_LOWER_MEM(
    LSA_UINT16                  *  ret_val_ptr,
    CLRPC_LOWER_MEM_PTR_TYPE       lower_mem_ptr,
    LSA_SYS_PTR_TYPE               sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_mem_ptr, LSA_COMP_ID_CLRPC);

    LSA_UNUSED_ARG( sys_ptr );
}
#endif

/*---------------------------------------------------------------------------*/

LSA_UINT16 clrpc_version ( LSA_UINT16 length, LSA_VERSION_TYPE *version )
{
  LSA_UINT8 *p_version, *p_v;
  LSA_UINT16 i;

  /* Initialize version struct */
  LSA_VERSION_TYPE v = {
                        PNIO_LSA_COMPONENT_ID, {'L','S','A','_','C','L','R','P','C','\0'},
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

/*=============================================================================*/
/** \brief reads the value of a 100ns-counter

    This value will be used for creating a uuid or server-boot-time.
    The counter starts with 0 an counts the 100ns from the last power-on until now.
*//*===========================================================================*/
#ifndef CLRPC_GET_100NS_COUNTER
LSA_VOID    CLRPC_GET_100NS_COUNTER(
    LSA_UINT16   * ret_val_ptr,         /**< pointer for return value
                                                            - LSA_RET_OK    nor error
                                                            - LSA_RET_RESOURCE    counter could not be read */
    LSA_UINT64   * counter_value_ptr   /**< pointer for value of 100ns-counter */
)
{
    *ret_val_ptr = LSA_RET_OK;
    LSAS_OS_GET_UNIX_TIME(counter_value_ptr);

}
#endif
/*****************************************************************************/
/*  end of file.                                                             */
/*****************************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
