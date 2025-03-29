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
/*  F i l e               &F: mrp_out.c                                 :F&  */
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

/*****************************************************************************/
/* contents:

    - internal functions

        - MRP_ALLOC_UPPER_RQB_LOCAL
        - MRP_FREE_UPPER_RQB_LOCAL
        - MRP_ALLOC_LOCAL_MEM
        - MRP_FREE_LOCAL_MEM
        - MRP_REQUEST_LOCAL
        - MRP_GET_PATH_INFO
        - MRP_RELEASE_PATH_INFO
        - MRP_ALLOC_TIMER
        - MRP_START_TIMER
        - MRP_STOP_TIMER
        - MRP_FREE_TIMER
        - MRP_ENTER
        - MRP_EXIT
        - MRP_FATAL_ERROR
        - MRP_MEMSET
        - MRP_MEMCPY
        - MRP_MEMCMP
        - MRP_HTON32
        - MRP_NTOH32
        - MRP_HTON16
        - MRP_NTOH16
        - MRP_GET_MS_CNT_VAL
        - MRP_ASSERT

    - system interface functions


    - user interface functions


*/
/*****************************************************************************/
/* 2do:

    -

*/
/*****************************************************************************/
/* include hierarchy */

//#define LTRC_ACT_MODUL_ID  17
//#define MRP_MODULE_ID      LTRC_ACT_MODUL_ID

#define LSAS_MODULE_ID 19

//#include "mrp_inc.h"
#include "lsas_inc.h"
#include "lsas_int.h"

/*===========================================================================*/
/*                                prototyping                                */
/*===========================================================================*/


LSA_VOID    MRP_ALLOC_UPPER_RQB_LOCAL(MRP_UPPER_RQB_PTR_TYPE  *upper_rqb_ptr_ptr,
                                                             LSA_UINT16 length)
{
    LSAS_ALLOC_COMP_MEM(upper_rqb_ptr_ptr,length, LSA_COMP_ID_MRP);
}


LSA_VOID    MRP_FREE_UPPER_RQB_LOCAL(LSA_UINT16 *  ret_val_ptr, MRP_UPPER_RQB_PTR_TYPE upper_rqb_ptr)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr,upper_rqb_ptr, LSA_COMP_ID_MRP);
}


LSA_VOID    MRP_EDD_ALLOC_LOWER_RQB(    MRP_EDD_LOWER_RQB_PTR_TYPE  *  lower_rqb_ptr_ptr,
                                        LSA_UINT16                                    length,
                                        LSA_SYS_PTR_TYPE                              sys_ptr)
{
    LSAS_ALLOC_COMP_MEM(lower_rqb_ptr_ptr,length, LSA_COMP_ID_MRP);
}

LSA_VOID    MRP_EDD_FREE_LOWER_RQB(     LSA_UINT16                                 *ret_val_ptr,
                                        MRP_EDD_LOWER_RQB_PTR_TYPE                  lower_rqb_ptr,
                                        LSA_SYS_PTR_TYPE                            sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr,lower_rqb_ptr, LSA_COMP_ID_MRP);
}


LSA_VOID    MRP_ALLOC_LOCAL_MEM(MRP_LOCAL_MEM_PTR_TYPE  *local_mem_ptr_ptr, LSA_UINT16 length)
{
    LSAS_ALLOC_COMP_MEM(local_mem_ptr_ptr,length, LSA_COMP_ID_MRP);
}


LSA_VOID    MRP_FREE_LOCAL_MEM(LSA_UINT16 *  ret_val_ptr, MRP_LOCAL_MEM_PTR_TYPE local_mem_ptr)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr,local_mem_ptr, LSA_COMP_ID_MRP);
}

/*=============================================================================
* function name:  MRP_REQUEST_UPPER_DONE
*
* function:       returns a finished request to the upper layer
*
* parameters:     MRP_UPPER_CBF_PTR_TYPE mrp_request_upper_done_ptr:
*                     pointer to mrp_request_upper_done_ptr, given by
*                     mrp_open_channel()
*
*                 MRP_UPPER_RQB_PTR_TYPE  upper_rqb_ptr:  pointer to upper RQB
*                 LSA_SYS_PTR_TYPE           sys_ptr:     system-pointer
*
* return value:   LSA_VOID
*
*===========================================================================*/

LSA_VOID   MRP_REQUEST_UPPER_DONE(
  MRP_UPPER_CALLBACK_FCT_PTR_TYPE mrp_request_upper_done_ptr,
  MRP_UPPER_RQB_PTR_TYPE          upper_rqb_ptr,
  LSA_SYS_PTR_TYPE                sys_ptr
  )
{
    mrp_request_upper_done_ptr(upper_rqb_ptr);
}

LSA_VOID    MRP_EDD_ALLOC_LOWER_MEM(    MRP_EDD_LOWER_MEM_PTR_TYPE  *lower_mem_ptr_ptr,
                                        LSA_UINT16                   length,
                                        LSA_SYS_PTR_TYPE             sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(lower_mem_ptr_ptr,length, LSA_COMP_ID_MRP);
}

LSA_VOID    MRP_OHA_ALLOC_LOWER_MEM(MRP_LOCAL_MEM_PTR_TYPE  *local_mem_ptr_ptr, LSA_UINT16 length)
{
    LSAS_ALLOC_COMP_MEM(local_mem_ptr_ptr,length, LSA_COMP_ID_MRP);
}


LSA_VOID    MRP_EDD_FREE_LOWER_MEM(     LSA_UINT16                  *  ret_val_ptr,
                                        MRP_EDD_LOWER_MEM_PTR_TYPE     lower_mem_ptr_ptr,
                                        LSA_SYS_PTR_TYPE               sysptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr,lower_mem_ptr_ptr, LSA_COMP_ID_MRP);
}

LSA_VOID    MRP_OHA_FREE_LOWER_MEM(LSA_UINT16 *  ret_val_ptr, MRP_LOCAL_MEM_PTR_TYPE local_mem_ptr)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr,local_mem_ptr, LSA_COMP_ID_MRP);
}


LSA_VOID  MRP_EDD_ALLOC_LOWER_RX_MEM(     MRP_EDD_LOWER_MEM_PTR_TYPE  *lower_mem_ptr_ptr,
                                          LSA_UINT32                   length,
                                          LSA_SYS_PTR_TYPE             sys_ptr
)
{
    LSAS_ALLOC_PACKET_MEM(lower_mem_ptr_ptr,length, LSA_COMP_ID_MRP);
}

LSA_VOID  MRP_EDD_ALLOC_LOWER_TX_MEM(  MRP_EDD_LOWER_MEM_PTR_TYPE *lower_mem_ptr_ptr,
                                       LSA_UINT32                  length,
                                       LSA_SYS_PTR_TYPE            sys_ptr
)
{
    LSAS_ALLOC_PACKET_MEM(lower_mem_ptr_ptr,length, LSA_COMP_ID_MRP);
}


LSA_VOID  MRP_EDD_FREE_LOWER_RX_MEM(  LSA_UINT16                 *  ret_val_ptr,
                                      MRP_EDD_LOWER_MEM_PTR_TYPE    lower_mem_ptr_ptr,
                                      LSA_SYS_PTR_TYPE              sys_pt
)
{
    LSAS_FREE_PACKET_MEM(ret_val_ptr,lower_mem_ptr_ptr, LSA_COMP_ID_MRP);
}

LSA_VOID  MRP_EDD_FREE_LOWER_TX_MEM(  LSA_UINT16                 *  ret_val_ptr,
                                      MRP_EDD_LOWER_MEM_PTR_TYPE    lower_mem_ptr_ptr,
                                      LSA_SYS_PTR_TYPE              sys_ptr
)
{
    LSAS_FREE_PACKET_MEM(ret_val_ptr,lower_mem_ptr_ptr, LSA_COMP_ID_MRP);
}



LSA_VOID    MRP_REQUEST_LOCAL(MRP_UPPER_RQB_PTR_TYPE pRQB)
{

    LSAS_RQB_MRP_REQUEST(pRQB, mrp_request);
}


LSA_VOID    MRP_EDD_REQUEST_LOWER(    MRP_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
                                      LSA_SYS_PTR_TYPE        sys_ptr
)
{
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



LSA_VOID    MRP_EDD_OPEN_CHANNEL_LOWER(MRP_EDD_LOWER_RQB_TYPE * lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    MRP_EDD_LOWER_OPEN_CHANNEL_PTR_TYPE open_channel_para = MRP_EDD_LOWER_GET_PPARAM_OPEN_CHANNEL(lower_rqb_ptr);

    open_channel_para->Cbf = (void (*) (EDD_UPPER_RQB_PTR_TYPE))lsas_edd_to_mrp_cbf;
#ifdef LSAS_CFG_USE_EDDI
    LSAS_RQB_EDD_REQUEST(lower_rqb_ptr, eddi_open_channel);
#endif
#ifdef LSAS_CFG_USE_EDDP
    LSAS_RQB_EDD_REQUEST(lower_rqb_ptr, eddp_open_channel);
#endif
#ifdef LSAS_CFG_USE_EDDS
    LSAS_RQB_EDD_REQUEST(lower_rqb_ptr, edds_open_channel);
#endif
    LSA_UNUSED_ARG(sys_ptr);
}


LSA_VOID    MRP_EDD_CLOSE_CHANNEL_LOWER(MRP_EDD_LOWER_RQB_TYPE * lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSA_UNUSED_ARG(sys_ptr);
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


LSA_VOID    MRP_GET_PATH_INFO(LSA_UINT16 *ret_val_ptr, LSA_SYS_PTR_TYPE  *sys_ptr_ptr,
                              MRP_DETAIL_PTR_TYPE *detail_ptr_ptr, LSA_SYS_PATH_TYPE path)
{
    LSA_GET_PATH_INFO(ret_val_ptr, sys_ptr_ptr, detail_ptr_ptr, path, LSAS_PATH_LAYER_MRP);
}


LSA_VOID    MRP_RELEASE_PATH_INFO(LSA_UINT16 *  ret_val_ptr, LSA_SYS_PTR_TYPE sys_ptr, MRP_DETAIL_PTR_TYPE detail_ptr)
{
    LSA_RELEASE_PATH_INFO(ret_val_ptr, sys_ptr, detail_ptr);
}


/*=============================================================================*/
/*                             TIMER-FUNCTIONS                                 */
/*=============================================================================*/

LSA_VOID    MRP_ALLOC_TIMER(LSA_UINT16 *ret_val_ptr, LSA_UINT16 *timer_id_ptr,
                            LSA_UINT16 timer_type, LSA_UINT16 time_base)
{
    LSAS_ALLOC_TIMER(ret_val_ptr, timer_id_ptr, timer_type, time_base, mrp_timeout, LSA_COMP_ID_MRP);
}



LSA_VOID    MRP_START_TIMER(LSA_UINT16 *ret_val_ptr, LSA_UINT16 timer_id, LSA_USER_ID_TYPE user_id, LSA_UINT16 time)
{
    LSAS_START_TIMER(ret_val_ptr, timer_id, user_id, time, LSA_COMP_ID_MRP);
}


LSA_VOID    MRP_STOP_TIMER(LSA_UINT16  *ret_val_ptr, LSA_UINT16 timer_id)
{
    LSAS_STOP_TIMER(ret_val_ptr, timer_id, LSA_COMP_ID_MRP);
}

LSA_VOID    MRP_FREE_TIMER(LSA_UINT16  *ret_val_ptr, LSA_UINT16 timer_id)
{
    LSAS_FREE_TIMER(ret_val_ptr, timer_id, LSA_COMP_ID_MRP);
}


LSA_VOID    MRP_ENTER   (LSA_VOID)
{

}


LSA_VOID    MRP_EXIT    (LSA_VOID)
{

}


LSA_VOID    MRP_FATAL_ERROR (LSA_UINT16 length, MRP_FATAL_ERROR_PTR_TYPE error_ptr)
{
      LSA_UNUSED_ARG(length);
    LSAS_FATAL_ERROR( error_ptr);
}




/*===========================================================================*/
/* non-LSA-predefined             prototyping                                */
/*===========================================================================*/

LSA_VOID    MRP_MEMSET  (MRP_LOCAL_MEM_PTR_TYPE pMem, LSA_UINT8 Value, LSA_UINT32 Length)
{
    LSAS_MEMSET(pMem,Value, Length);            // lint !e668  Possibly passing a null pointer to function
}


LSA_VOID    MRP_MEMCPY(MRP_LOCAL_MEM_PTR_TYPE pMemDst, MRP_LOCAL_MEM_PTR_TYPE pMemSrc, LSA_UINT length)
{
    LSAS_MEMCPY(pMemDst,pMemSrc,length);        // lint !e668  Possibly passing a null pointer to function
}

LSA_INT     MRP_MEMCMP(MRP_LOCAL_MEM_PTR_TYPE pBuf1, MRP_LOCAL_MEM_PTR_TYPE pBuf2, LSA_UINT length)
{
    return ( LSAS_MEMCMP ((const void *)(pBuf1), (const void *)(pBuf2), length) );
}


LSA_UINT32  MRP_HTON32  (LSA_UINT32 value)
{
    return LSAS_HTONL(value);
}


LSA_UINT32  MRP_NTOH32  (LSA_UINT32 value)
{
    return LSAS_NTOHL(value);
}


LSA_UINT16  MRP_HTON16  (LSA_UINT16 value)
{
    return LSAS_HTONS(value);
}


LSA_UINT16  MRP_NTOH16  (LSA_UINT16 value)
{
    return LSAS_NTOHS(value);
}

#ifndef MRP_PUT16_HTON
    LSA_VOID
    MRP_PUT16_HTON ( /* put and convert from host byte order to network byte order */
        LSA_VOID_PTR_TYPE base,
        LSA_INT offset,
        LSA_UINT16 val
    ) {
        LSA_UINT8 *ptr = base;
        ptr += offset;
        /* val = htons (val); */
        val = MRP_HTON16 (val);
        MRP_MEMCPY (ptr, & val, sizeof(val));
    }
#endif

#ifndef MRP_GET16_NTOH
    LSA_UINT16
    MRP_GET16_NTOH( /* get and convert from network byte order to host byte order */
        LSA_VOID_PTR_TYPE base,
        LSA_INT offset
    ) {
        LSA_UINT16  val;
        LSA_UINT8   *ptr = base;
        ptr += offset;
        MRP_MEMCPY (& val, ptr, sizeof(val));
        /* val = ntohs (val); */
        val = MRP_NTOH16 (val);
        return val;
    }
#endif



#ifndef MRP_PUT32_HTON
    LSA_VOID
    MRP_PUT32_HTON ( /* put and convert from host byte order to network byte order */
        LSA_VOID_PTR_TYPE base,
        LSA_INT offset,
        LSA_UINT32 val
    ) {
        LSA_UINT8 *ptr = base;
        ptr += offset;
        /* val = htonl (val); */
        val = MRP_HTON32 (val);
        MRP_MEMCPY (ptr, & val, sizeof(val));
    }
#endif

#ifndef MRP_GET32_NTOH
    LSA_UINT32
    MRP_GET32_NTOH( /* get and convert from network byte order to host byte order */
        LSA_VOID_PTR_TYPE base,
        LSA_INT offset
    ) {
        LSA_UINT32  val;
        LSA_UINT8   *ptr = base;
        ptr += offset;
        MRP_MEMCPY (& val, ptr, sizeof(val));
        /* val = ntohl (val); */
        val = MRP_NTOH32 (val);
        return val;
    }
#endif


LSA_VOID    MRP_GET_MS_CNT_VAL  (LSA_UINT32 *counterVal)
{
    LSAS_GET_MS_COUNT(counterVal);
}


LSA_VOID    MRP_ASSERT  (LSA_INT cond)
{
    if( ! (cond) )
    {
        LSAS_ASSERT( 0);
    }
}

LSA_UINT16 mrp_version ( LSA_UINT16 length, LSA_VERSION_TYPE *version )
{
  LSA_UINT8 *p_version, *p_v;
  LSA_UINT16 i;

  /* Initialize version struct */
  LSA_VERSION_TYPE v = {
                        PNIO_LSA_COMPONENT_ID, {'L','S','A','_','M','R','P','\0','\0','\0'},
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
