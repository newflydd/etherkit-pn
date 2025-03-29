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
/*  F i l e               &F: iom_kram.c                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  software module handling the ERTEC200 KRAM                               */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*                                                                           */
/*   H i s t o r y :                                                         */
/*   see /_doc/im151_3pn_hist.doc                                            */
/*                                                                           */
/*****************************************************************************/


/*****************************************************************************/
/* content:

    - iom_kram_init
    - iom_kram_allocate_iocr_memory
    - iom_kram_complete_iocr_information
    - iom_kram_free_iocr_memory
    - iom_kram_free_mem_local
    - iom_kram_consumer_lock
    - iom_kram_consumer_unlock
    - iom_kram_consumer_lock_unbuffered
    - iom_kram_consumer_unlock_unbuffered
    - iom_kram_update_consumer_data
    - iom_kram_get_provider_buffer
    - iom_kram_provider_lock
    - iom_kram_provider_unlock
    - iom_kram_provider_lock_unbuffered
    - iom_kram_provider_unlock_unbuffered
    - iom_kram_provider_lock_section
    - iom_kram_provider_unlock_section
    - iom_kram_update_provider_data

*/
/*****************************************************************************/
/* 2do:

    -

*/
/*****************************************************************************/
/* include hierarchy */

#define IOM_LOCAL_ERR_MODULE IOM_MODULE_KRAM

#include "iom_inc.h"
#include "iom_int.h"

#ifdef IOM_CFG_IRTE

/********************************************************************************/
/*                                                                              */
/*         io-memory management model - handling the ARs independently          */
/*                                                                              */
/*------------------------------------------------------------------------------*/
/*                                                                              */
/*          ___KRAM____  <----- start of first ar growing from top to middle    */
/*         |           |        (IOM_CFG_KRAM_DATA_START)                       */
/*         |           |                                                        */
/*         |  1st AR   |                                                        */
/*         |           |                                                        */
/*         |           |                                                        */
/*         |...........| <----- iom_proc_image.fragment[0].current_data_offset  */
/*         |           |                                                        */
/*         | currently |                                                        */
/*         | free      |                                                        */
/*         | io-memory |                                                        */
/*         |           |                                                        */
/*         |...........| <----- iom_proc_image.fragment[1].current_data_offset  */
/*         |           |                                                        */
/*         |  2nd AR   |                                                        */
/*         |___________| <----- end of second ar growing from middle to bottom  */
/*                              (IOM_CFG_KRAM_PROCESS_IMAGE_SIZE)                    */
/*                                                                              */
/********************************************************************************/

/* internal defines */
#define IOM_KRAM_AR_IDX_NO_AR              0xFFFFFFFF
#define IOM_KRAM_AR_IDX_AR_NOT_ALLOWED     0xFFFFFFFE

typedef enum iom_kram_ar_state_e
{
    IOM_KRAM_AR_STATE_EMPTY,
    IOM_KRAM_AR_STATE_AR_INFO,
    IOM_KRAM_AR_STATE_ESTABLISHED,
    IOM_KRAM_AR_STATE_ESTABLISHED_UNBUFFERED,
    IOM_KRAM_AR_STATE_POWERING_DOWN
} iom_kram_ar_state_t;

#define IOM_KRAM_UNUSED_PROV_ID                 0xFFFF


/* data */

typedef struct iom_kram_fragment_s
{
    iom_kram_ar_state_t    state;               /* state machine */
    Unsigned32             ar_idx;              /* IOM_KRAM_AR_IDX_NO_AR: unused,                         */
                                                /* IOM_KRAM_AR_IDX_AR_NOT_ALLOWED: no second ar used,     */
                                                /* otherwise: ar number                                    */
    Unsigned32             rt_class;            /* RTC1, RTC2, RTC3 */

    Unsigned32             flags;               /* consumer buffer size with data / ioxs / apdu rounded to 8 byte alignment */

    Unsigned8 *            unkramed_buffer_ptr; /* pointing to begin (AR0) or end (AR1) of free unkramed buffer            */
    Unsigned32             current_data_offset; /* offset for new iocr                  */

    struct
    {
        Unsigned32         buffer_size;         /* buffer size with data / ioxs / apdu rounded to 4 byte alignment */
        Unsigned32         buffer_size_no_padding;/* buffer size with data / ioxs / apdu; not rounded */

        Unsigned32         buffer_begin_asic;   /* the asic buffer begins at this offset */

        Unsigned8 *        src_buffer_ptr;      /* absolute address consumer source (asic)                 */
        Unsigned8 *        dst_buffer_ptr_next; /* absolute address consumer destination (copy asic->sdram)*/
        Unsigned8 *        dst_buffer_ptr_user; /* absolute address consumer destination (user)            */

        Unsigned16         cycle_counter;       /* rtc1/ rtc2 new buffer model: provider id for this provider */
        Unsigned8          lock;                /* 0: buffer not locked by user, otherwise !0              */
        Unsigned8          new_data;            /* 0: now new output data, otherwise !0                    */
    } cons;

    struct
    {
        Unsigned32         buffer_size;         /* buffer size with data / ioxs / apdu rounded to 4 byte alignment */
        Unsigned32         buffer_size_no_padding;/* buffer size with data / ioxs / apdu; not rounded */

        Unsigned32         buffer_begin_asic;   /* the asic (triple-) buffer begins at this offset         */

        Unsigned32         buffer_offset_user;  /* current data offset (user buffer) related to process data begin */

        Unsigned8 *        dst_buffer_ptr;      /* absolute address provider destination (asic)            */
        Unsigned8 *        src_buffer_ptr_user; /* absolute address provider source (user)                 */
        Unsigned8 *        src_buffer_ptr_next; /* absolute address provider source (next to user)         */

        Unsigned16         id;                  /* rtc1/ rtc2 new buffer model: provider id for this provider */
        Unsigned16         session_key;         /* bypassed AR-session_key, needed for setting data state  */
        Unsigned8          lock;                /* 0: buffer not locked by user, otherwise !0              */
        Unsigned8          new_data;            /* 0: now new input data, otherwise !0                     */

        #ifndef EDDI_CFG_USE_SW_IIIB_IF_PROV

            EDDI_RT_PROVIDER_BUFFER_REQ_TYPE  req_buf;
            Unsigned8          cmd_ok;          /* 0: now new command or edd service went wrong, otherwise !0 */

        #endif
    } prov;

    Unsigned32      consumer_lock;
    Unsigned32      provider_lock;
    Unsigned32      consumer_new_data;
    Unsigned32      provider_new_data;
    Unsigned32      provider_cmd_ok;

} iom_kram_fragment_t;

typedef union
{
    LSA_UINT32 both_values;
    struct
    {
        LSA_UINT16 cycle_counter;
        LSA_UINT8  data_status;
        LSA_UINT8  transfer_status;
    } as;
} iom_kram_control_t;

struct
{
    iom_kram_fragment_t         fragment[2];    /* 2 dynamic fragments can be managed */
    iom_init_struct_t           init_data;      /* init data overhanded by user       */
    Unsigned16                  session_key_prim_ar;    /* session key of the primary ar  */

    struct
    {
        EDDI_GSHAREDMEM_TYPE           gshared_mem;
    } eddi;
} iom_kram_proc_image;


/* code */

/*******************************************************************************/
/* this macro waits 3µs                                                        */
/*******************************************************************************/

#define IOM_KRAM_WAIT()                                                        \
{                                                                               \
    /* there are 5 assembly instructions served in 50ns */                      \
    /* this means 3µs wait time -> wait:= 600           */                      \
    volatile LSA_UINT32 wait = 600;                                             \
                                                                                \
    while(wait--);                                                              \
}


/*******************************************************************************/
/* this macro swaps two buffers                                                */
/*******************************************************************************/

#define IOM_KRAM_SWAP_BUFFERS(_ptr1, _ptr2)                                    \
{                                                                               \
    Unsigned8 *tmp = (_ptr1);                                                   \
                                                                                \
    (_ptr1) = (_ptr2);                                                          \
                                                                                \
    (_ptr2) = tmp;                                                              \
}

/* prototype */
LSA_VOID    iom_kram_free_mem_local    (iom_kram_fragment_t *  fragment_ptr);

/*******************************************************************************/
/* this function is called during startup close to edd initialisation          */
/*******************************************************************************/
LSA_VOID iom_kram_init  (iom_init_struct_t * init_data)
{
    LSA_RESULT ret_val;

    /* init fragments of the image buffer */
    LSA_MEMSET(&iom_kram_proc_image.fragment[0], 0, 2 * sizeof(iom_kram_fragment_t));
    iom_kram_proc_image.fragment[0].ar_idx      =
    iom_kram_proc_image.fragment[1].ar_idx      = IOM_KRAM_AR_IDX_NO_AR;
    iom_kram_proc_image.fragment[0].rt_class    =
    iom_kram_proc_image.fragment[1].rt_class    = CM_IOCR_PROP_RT_CLASS_MASK;
    iom_kram_proc_image.fragment[0].prov.id     =
    iom_kram_proc_image.fragment[1].prov.id     = IOM_KRAM_UNUSED_PROV_ID;
    iom_kram_proc_image.fragment[0].state       =
    iom_kram_proc_image.fragment[1].state       = IOM_KRAM_AR_STATE_EMPTY;

    iom_kram_proc_image.fragment[1].current_data_offset = IOM_CFG_KRAM_PROCESS_IMAGE_SIZE;

    LSA_MEMCPY(&iom_kram_proc_image.init_data, init_data, sizeof(iom_init_struct_t));

    if( iom_kram_proc_image.init_data.properties & IOM_KRAM_PROP_FORBID_SECOND_AR )
    {
        iom_kram_proc_image.fragment[1].ar_idx = IOM_KRAM_AR_IDX_AR_NOT_ALLOWED;
    }

    /* set pointers for managing the unkramed buffer */
    iom_kram_proc_image.fragment[0].unkramed_buffer_ptr = iom_kram_proc_image.init_data.unkramed_buffer_address;
    iom_kram_proc_image.fragment[1].unkramed_buffer_ptr = iom_kram_proc_image.init_data.unkramed_buffer_address+iom_kram_proc_image.init_data.unkramed_buffer_size;

    iom_kram_proc_image.session_key_prim_ar = 0;    /* not determined so far */

    #if ((defined EDDI_CFG_USE_SW_IIIB_IF_PROV) || (defined EDDI_CFG_ERTEC_400))
        /* init 3b provider for rtc1 / rtc2 */
        EDDI_BUFFER_REQUEST_SETUP(ret_val, EDD_INTERFACE_ID_MIN-1 , 0, IOM_CFG_KRAM_DATA_START,
                IOM_CFG_KRAM_IRTE_REG_START, &iom_kram_proc_image.eddi.gshared_mem, 0, 0, 0);

        if (ret_val != EDD_STS_OK)
        {
            iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
        }
    #endif
}


/*******************************************************************************/
/* returning a buffer offset to pndv. pndv bypasses the offset as opaque data  */
/* to cm. cm continues in adding a provider or consumer.                       */
/*******************************************************************************/

LSA_BOOL iom_kram_allocate_iocr_memory(LSA_UINT32 ar_idx, LSA_UINT16 ar_set_nr, LSA_UINT16 session_key, CM_UPPER_SV_AR_CONNECT_IOCR_PTR_TYPE iocr_ptr)
{
    iom_kram_fragment_t * fragment_ptr;
    Unsigned16            iocr_properties;
    volatile LSA_UINT32   buffer_size, buffer_size_total, unkramed_buffer_size;
    iom_kram_ar_state_t   new_state;
    LSA_UINT16            ret_value_ptr;
    Unsigned8 *           tmp_unkramed_buffer_ptr = NULL;

    ret_value_ptr = FALSE;

    buffer_size = iocr_ptr->io_buffer_size;
    buffer_size = (buffer_size + 4 + 3) & 0xfffffffc;   /* APDU Status + alignment */

    iocr_properties = iocr_ptr->iocr_properties & CM_IOCR_PROP_RT_CLASS_MASK;

    if( (iocr_ptr->iocr_type == CM_IOCR_TYPE_INPUT) || (iocr_ptr->iocr_type == CM_IOCR_TYPE_MULTICAST_PROVIDER) )
    {
        /* provider */

        #ifndef IOM_CFG_IRTE400
        /* providers need triple buffers */
            buffer_size_total = buffer_size * 3;
        #else
            buffer_size_total = buffer_size;
        #endif

        /* unkramed buffer not needed */
        unkramed_buffer_size = 0;
    }
    else
    {
        /* consumer */
        buffer_size_total = buffer_size;

        if( iom_kram_proc_image.init_data.properties & IOM_KRAM_PROP_USE_CONS_EX_BUFFERS )
        {
            /* exchange consumer buffer */
            unkramed_buffer_size = buffer_size * 2;
        }
        else
        {
            /* no exchange consumer buffer */
            unkramed_buffer_size = buffer_size;
        }

    }


    /* check for free kram space and whether the desired buffer may be oversized compared to the maximum application iocr size */
    if(    (buffer_size_total    <= (iom_kram_proc_image.fragment[1].current_data_offset - iom_kram_proc_image.fragment[0].current_data_offset))
        && (unkramed_buffer_size <= (iom_kram_proc_image.fragment[1].unkramed_buffer_ptr - iom_kram_proc_image.fragment[0].unkramed_buffer_ptr))
      )
    {
        /* first cr (ar_idx not set) or second cr of the first ar (ar_idx is set to ar number) */
        if( (iom_kram_proc_image.fragment[0].ar_idx == IOM_KRAM_AR_IDX_NO_AR) || (iom_kram_proc_image.fragment[0].ar_idx == ar_idx) )
        {
            fragment_ptr = & iom_kram_proc_image.fragment[0];
        }
        /* first cr (ar_idx not set) or second cr of the second ar (ar_idx is set to ar number) */
        else if( (iom_kram_proc_image.fragment[1].ar_idx == IOM_KRAM_AR_IDX_NO_AR) || (iom_kram_proc_image.fragment[1].ar_idx == ar_idx) )
        {
            fragment_ptr = & iom_kram_proc_image.fragment[1];

            /* mind that second ar grows from middle to bottom -> set current_data_offset at first */
            fragment_ptr -> current_data_offset = fragment_ptr -> current_data_offset - buffer_size_total;

            fragment_ptr -> unkramed_buffer_ptr = fragment_ptr -> unkramed_buffer_ptr - unkramed_buffer_size;
            tmp_unkramed_buffer_ptr = fragment_ptr -> unkramed_buffer_ptr;
        }
        else
        {
            fragment_ptr = 0;
        }

        if(fragment_ptr != 0)
        {
        	new_state = IOM_KRAM_AR_STATE_EMPTY;
        	switch(fragment_ptr -> state)
            {
                case IOM_KRAM_AR_STATE_EMPTY: /* first iocr memory to be allocated */
                {
                    new_state = IOM_KRAM_AR_STATE_AR_INFO;
                    break;
                }
                case IOM_KRAM_AR_STATE_AR_INFO: /* second iocr to be completed */
                {
                    new_state = IOM_KRAM_AR_STATE_ESTABLISHED;
                    break;
                }
                default: /* error */
                {
                    iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
                    break;
                }
            }

            fragment_ptr -> ar_idx = ar_idx;
            fragment_ptr -> rt_class = iocr_properties;

            /* pool fragment not used -> change the pool size */
            if( (iocr_ptr -> iocr_type ==CM_IOCR_TYPE_INPUT) || (iocr_ptr -> iocr_type ==CM_IOCR_TYPE_MULTICAST_PROVIDER) )
            {
                if /* provider not allocated so far */
                  (fragment_ptr -> prov.buffer_size == 0)
                {
                    fragment_ptr -> prov.buffer_size = buffer_size;
                    fragment_ptr -> prov.buffer_size_no_padding = iocr_ptr->io_buffer_size + 4;     /* IO data + APDU */
                    fragment_ptr -> prov.buffer_begin_asic  =
                    fragment_ptr -> prov.buffer_offset_user = fragment_ptr -> current_data_offset;
                }
                else
                {
                    iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
                }

            }
            else if( (iocr_ptr -> iocr_type ==CM_IOCR_TYPE_OUTPUT) || (iocr_ptr -> iocr_type ==CM_IOCR_TYPE_MULTICAST_CONSUMER) )
            {
                if /* consumer not allocated so far */
                  (fragment_ptr -> cons.buffer_size == 0)
                {
                    fragment_ptr -> cons.buffer_size = buffer_size;
                    fragment_ptr -> cons.buffer_size_no_padding = iocr_ptr->io_buffer_size + 4;
                    fragment_ptr -> cons.buffer_begin_asic = fragment_ptr -> current_data_offset;
                }
                else
                {
                    iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
                }
            }
            else
            {
                iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
            }

            IOM_MEMSET((void *)(IOM_CFG_KRAM_DATA_START + fragment_ptr -> current_data_offset), 0, buffer_size_total );

#if 0
            if( (iocr_ptr -> iocr_type ==CM_IOCR_TYPE_INPUT) || (iocr_ptr -> iocr_type ==CM_IOCR_TYPE_MULTICAST_PROVIDER) )
            {
                * (Unsigned8 *)(IOM_CFG_KRAM_DATA_START + fragment_ptr -> current_data_offset + 2)                =
                * (Unsigned8 *)(IOM_CFG_KRAM_DATA_START + fragment_ptr -> current_data_offset + 2 + buffer_size)  =
                * (Unsigned8 *)(IOM_CFG_KRAM_DATA_START + fragment_ptr -> current_data_offset + 2 + 2*buffer_size)= 0x35;
            }
#endif
            iocr_ptr->eddx_data_offset     = fragment_ptr -> current_data_offset;
            iocr_ptr->eddx_cr_number       =
            iocr_ptr->eddp_ct_outdatastart =
            iocr_ptr->eddp_forward_mode    =
            iocr_ptr->iocr_user_id.uvar32  = 0;

            if(fragment_ptr == & iom_kram_proc_image.fragment[0])
            {
                /* mind that first ar grows from top to middle -> set current_data_offset at last */
                fragment_ptr -> current_data_offset = fragment_ptr -> current_data_offset + buffer_size_total;

                tmp_unkramed_buffer_ptr = fragment_ptr -> unkramed_buffer_ptr;
                fragment_ptr -> unkramed_buffer_ptr = fragment_ptr -> unkramed_buffer_ptr + unkramed_buffer_size;
            }

            if( (iocr_ptr -> iocr_type ==CM_IOCR_TYPE_INPUT) || (iocr_ptr -> iocr_type ==CM_IOCR_TYPE_MULTICAST_PROVIDER) )
            {
                iocr_ptr->eddi_buffer_properties = EDD_PROV_BUFFER_PROP_IRTE_IMG_ASYNC;

                if(fragment_ptr -> rt_class == CM_IOCR_PROP_RT_CLASS_3)
                {
                    #if defined EDDI_CFG_USE_SW_IIIB_IF_PROV /* edd model: triple buffer and their exchange by edd-software */

                        /* no call of iom_kram_update_provider_data -> no dst buffer */
                        fragment_ptr -> prov.dst_buffer_ptr = 0;

                        /* the first buffer of the triple is the user buffer */
                                                fragment_ptr -> prov.src_buffer_ptr_user =
                                                    (Unsigned8 *)(IOM_CFG_KRAM_DATA_START + fragment_ptr->prov.buffer_offset_user + sizeof(iom_apdu_status_t));

                        /* no buffer exchange by iom_kram, no provider_src_buffer_ptr_next set */
                        fragment_ptr -> prov.src_buffer_ptr_next = 0;

                    #else /* the edd does not use triple buffer but there will be used 3 kram buffers                           */
                          /*    buffer #1: always at the asic, buffer #2 and buffer #3 are assigned to be user and next-to-user */
                          /*    buffer #2 and buffer #3 will be exchanged by iom_kram-software                                 */

                        fragment_ptr -> prov.dst_buffer_ptr =
                            (Unsigned8 *)(IOM_CFG_KRAM_DATA_START + fragment_ptr->prov.buffer_offset_user + sizeof(iom_apdu_status_t));

                        #ifndef IOM_CFG_IRTE400
                            /* the second buffer of the triple is the user buffer */
                            fragment_ptr -> prov.src_buffer_ptr_user =
                                (Unsigned8 *)(IOM_CFG_KRAM_DATA_START + fragment_ptr->prov.buffer_offset_user + buffer_size + sizeof(iom_apdu_status_t));

                            /* the third buffer of the triple is the next to user buffer */
                            fragment_ptr -> prov.src_buffer_ptr_next =
                                (Unsigned8 *)(IOM_CFG_KRAM_DATA_START + fragment_ptr->prov.buffer_offset_user + (2*buffer_size) + sizeof(iom_apdu_status_t));
                        #endif

                    #endif
                }
                else
                {
                    #if defined EDDI_CFG_USE_SW_IIIB_IF_PROV /* edd model: triple buffer exchange by software */

                        /* no call of iom_kram_update_provider_data -> no dst buffer */
                        fragment_ptr -> prov.dst_buffer_ptr = 0;

                        /* the first buffer of the triple is the user buffer */
                        fragment_ptr -> prov.src_buffer_ptr_user =
                            (Unsigned8 *)(IOM_CFG_KRAM_DATA_START + fragment_ptr -> prov.buffer_offset_user + sizeof(iom_apdu_status_t) );

                        /* no buffer exchange by iom_kram, no provider_src_buffer_ptr_next set */
                        fragment_ptr -> prov.src_buffer_ptr_next = 0;

                    #else /* edd model: triple buffer exchange by command interface: used by pnpn coupler */
                          /*    on rtc1 and rtc2 a user buffer will be delivered with   */
                          /*    edd-service eddi_ProviderBufferRequest: assume NIL here  */

                        #ifndef IOM_CFG_IRTE400
                            /* no call of iom_kram_update_provider_data (buffer exchange by hardware) -> no dst buffer */
                            fragment_ptr -> prov.dst_buffer_ptr = 0;
                            fragment_ptr -> prov.src_buffer_ptr_user =
                            fragment_ptr -> prov.src_buffer_ptr_next = 0;
                        #else
                            fragment_ptr -> prov.dst_buffer_ptr =
                                (Unsigned8 *)(IOM_CFG_KRAM_DATA_START + fragment_ptr -> prov.buffer_offset_user + sizeof(iom_apdu_status_t));
                        #endif

                    #endif
                }

                {
                    LSA_UINT8   local_prov_type;
                    LSA_UINT16  local_provider_id;

                    if (ar_set_nr == 0)
                    {
                        local_prov_type = EDD_PROVADD_TYPE_DEFAULT;
                    }
                    else
                    {
                        local_prov_type = EDD_PROVADD_TYPE_SYSRED;
                    }

                    #if defined EDDI_CFG_USE_SW_IIIB_IF_PROV

                    if( EDD_STS_OK != eddi_ProviderBufferRequest_Add(EDD_INTERFACE_ID_MIN-1, &local_provider_id, iocr_ptr -> eddi_cw_offset,
                                                                                                 iocr_ptr -> eddx_data_offset,
                                                                                                 0 /* DataStatusOffset, don't care on 3B SW*/,
                                                                                                 0 /* GroupID, reserved */,
                                                                                                 ar_set_nr,
                                                                                                 iom_kram_proc_image.session_key_prim_ar /* PrimaryARID, ? */,
                                                                                                 local_prov_type,
                                                                                                 iocr_ptr -> io_buffer_size))
                    {
                        iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
                    }

                    /* now the provider ident and the corresponding acw is known */
                    fragment_ptr -> prov.id =  local_provider_id;

                    fragment_ptr->prov.session_key = session_key;


                    #else

                        fragment_ptr->prov.req_buf.hDDB          = iom_kram_proc_image.init_data.hDDB;
                        fragment_ptr->prov.req_buf.ProviderID    = fragment_ptr->prov.id;

                    #endif
                }
            }
            else if( (iocr_ptr -> iocr_type ==CM_IOCR_TYPE_OUTPUT) || (iocr_ptr -> iocr_type ==CM_IOCR_TYPE_MULTICAST_CONSUMER) )
            {
                iocr_ptr->eddi_buffer_properties = EDD_CONS_BUFFER_PROP_IRTE_IMG_ASYNC;

                /* set kram buffer pointer */
                fragment_ptr -> cons.src_buffer_ptr = (Unsigned8 *)(IOM_CFG_KRAM_DATA_START + fragment_ptr -> cons.buffer_begin_asic);

                //#ifndef IOM_CFG_IRTE400
                /* set unkram buffer pointer */
                fragment_ptr -> cons.dst_buffer_ptr_next     = tmp_unkramed_buffer_ptr;

                if( iom_kram_proc_image.init_data.properties & IOM_KRAM_PROP_USE_CONS_EX_BUFFERS )
                {
                    tmp_unkramed_buffer_ptr                  = tmp_unkramed_buffer_ptr + fragment_ptr -> cons.buffer_size;
                    fragment_ptr -> cons.dst_buffer_ptr_user = tmp_unkramed_buffer_ptr;
                }
                else
                {
                    fragment_ptr -> cons.dst_buffer_ptr_user = 0;
                }
                //#endif
            }

            fragment_ptr -> state = new_state;

            ret_value_ptr = TRUE;
        }
    }
    else
    {
        ret_value_ptr = FALSE;
    }

    return ret_value_ptr;
}


/*******************************************************************************/
/* this function releases the buffer after an ar is down                       */
/*******************************************************************************/

LSA_VOID    iom_kram_free_iocr_memory (LSA_UINT32 ar_idx)
{
    iom_kram_fragment_t *  fragment_ptr = NULL;

    if(iom_kram_proc_image.fragment[0].ar_idx == ar_idx)
    {
        fragment_ptr = & iom_kram_proc_image.fragment[0];
    }
    else if(iom_kram_proc_image.fragment[1].ar_idx == ar_idx)
    {
        fragment_ptr = & iom_kram_proc_image.fragment[1];
    }
    else
    {
        fragment_ptr = 0;
    }

    if(fragment_ptr != 0)
    {
        /* AR mit und ohne Nutzdaten, mit freizugebendem Nutzdatenpuffer */

        switch(fragment_ptr -> state)
        {
            case IOM_KRAM_AR_STATE_EMPTY:
            case IOM_KRAM_AR_STATE_POWERING_DOWN:
            {
                /* nothing to do */

                break;
            }

            case IOM_KRAM_AR_STATE_AR_INFO:
            case IOM_KRAM_AR_STATE_ESTABLISHED:
            case IOM_KRAM_AR_STATE_ESTABLISHED_UNBUFFERED:
            {
                fragment_ptr -> state = IOM_KRAM_AR_STATE_POWERING_DOWN;

#ifdef IOM_SYNC_CR_RELEASE_BUFFERS
                iom_kram_free_mem_local(fragment_ptr);
#endif

                break;
            }
            default: /* error */
            {
                iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);

                break;
            }
        }
    }
    else
    {
        /* AR ohne Nutzdatenpuffer */
        IOM_IOCR_MEMORY_FREED(ar_idx);
    }
}


LSA_VOID    iom_kram_free_mem_local    (iom_kram_fragment_t *  fragment_ptr)
{
#ifdef IOM_SYNC_CR_RELEASE_BUFFERS
    if( (fragment_ptr->cons.lock) || (fragment_ptr->prov.lock) )
    {
        iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
    }
#else
    if( (!fragment_ptr->cons.lock) && (!fragment_ptr->prov.lock) )
#endif
    {
        Unsigned32 tmp_ar_idx = fragment_ptr -> ar_idx;

#ifdef IOM_SYNC_CR_RELEASE_BUFFERS
        IOM_ENTER();
#endif

        if( fragment_ptr -> prov.id != IOM_KRAM_UNUSED_PROV_ID )
        {
            #if defined EDDI_CFG_USE_SW_IIIB_IF_PROV

                if(eddi_ProviderBufferRequest_Remove(EDD_INTERFACE_ID_MIN-1, fragment_ptr -> prov.id) != EDD_STS_OK)
                {
                    iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
                }

            #endif
        }

        LSA_MEMSET(fragment_ptr, 0, sizeof(iom_kram_fragment_t));
        fragment_ptr -> ar_idx      = IOM_KRAM_AR_IDX_NO_AR;
        fragment_ptr -> rt_class    = CM_IOCR_PROP_RT_CLASS_MASK;
        fragment_ptr -> prov.id     = IOM_KRAM_UNUSED_PROV_ID;
        fragment_ptr -> state       = IOM_KRAM_AR_STATE_EMPTY;

        if(fragment_ptr == & iom_kram_proc_image.fragment[0])
        {
            fragment_ptr -> unkramed_buffer_ptr = iom_kram_proc_image.init_data.unkramed_buffer_address;

        }
        else
        {
            fragment_ptr -> current_data_offset = IOM_CFG_KRAM_PROCESS_IMAGE_SIZE;
            fragment_ptr -> unkramed_buffer_ptr = iom_kram_proc_image.init_data.unkramed_buffer_address+iom_kram_proc_image.init_data.unkramed_buffer_size;
        }

#ifdef IOM_SYNC_CR_RELEASE_BUFFERS
        IOM_EXIT();
#endif

        IOM_IOCR_MEMORY_FREED(tmp_ar_idx);
    }
}


/*******************************************************************************/
/* this function delivers a new consumer buffer to the application             */
/*******************************************************************************/

LSA_UINT16 iom_kram_consumer_lock(LSA_VOID * * consumer_buffer, iom_apdu_status_t **apdu_status_ptr, LSA_UINT32 ar_idx)
{
    iom_kram_fragment_t *  fragment_ptr = NULL;
    LSA_UINT16            ret_value = IOM_OK;

    if(iom_kram_proc_image.fragment[0].ar_idx == ar_idx)
    {
        fragment_ptr = & iom_kram_proc_image.fragment[0];
    }
    else if(iom_kram_proc_image.fragment[1].ar_idx == ar_idx)
    {
        fragment_ptr = & iom_kram_proc_image.fragment[1];
    }
    else
    {
        return IOM_INVALID_BUFFER;
    }

    switch(fragment_ptr -> state)
    {
        case IOM_KRAM_AR_STATE_ESTABLISHED: /* in data ex */
        {
            if(!fragment_ptr->cons.lock)
            {
                fragment_ptr->cons.lock = 1;

                if( iom_kram_proc_image.init_data.properties & IOM_KRAM_PROP_USE_CONS_EX_BUFFERS )
                {
                    IOM_ENTER_IO();

                    if( fragment_ptr->cons.new_data )
                    {
                        /* ready for new output data while user is treating the data (next- and user-buffer system) */
                        fragment_ptr->cons.new_data = 0;

                        /* swap buffers */
                        IOM_KRAM_SWAP_BUFFERS(fragment_ptr -> cons.dst_buffer_ptr_next, fragment_ptr -> cons.dst_buffer_ptr_user);

                        ret_value = IOM_OK_NEW_BUFFER;
                    }
                    else
                    {
                        ret_value = IOM_OK_OLD_BUFFER;
                    }

                    IOM_EXIT_IO();

                    /* return pointer without the apdu state (4 bytes at the beginning) */
                    * consumer_buffer = fragment_ptr -> cons.dst_buffer_ptr_user + sizeof(iom_apdu_status_t);
                }
                else
                {
                    if( fragment_ptr->cons.new_data )
                    {
                        ret_value = IOM_OK_NEW_BUFFER;
                    }
                    else
                    {
                        ret_value = IOM_OK_OLD_BUFFER;
                    }

                    /* return pointer without the apdu state (4 bytes at the beginning) */
                    * consumer_buffer = fragment_ptr -> cons.dst_buffer_ptr_next + sizeof(iom_apdu_status_t);
                }

                if(apdu_status_ptr != LSA_NULL)
                {
                    *apdu_status_ptr = (iom_apdu_status_t*) (*consumer_buffer - sizeof(iom_apdu_status_t));
                }
            }
            else
            {
                iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);

                /* avoid warning */
                ret_value = IOM_BUFFER_LOCKED;
            }

            break;
        }

        default:

            ret_value = IOM_INVALID_BUFFER;
    }

    return ret_value;
}

/*******************************************************************************/
/* this function releases a consumer buffer from the application               */
/*******************************************************************************/

LSA_UINT16 iom_kram_consumer_unlock(LSA_UINT32 ar_idx)
{
    iom_kram_fragment_t *  fragment_ptr = NULL;

    if(iom_kram_proc_image.fragment[0].ar_idx == ar_idx)
    {
        fragment_ptr = & iom_kram_proc_image.fragment[0];
    }
    else if(iom_kram_proc_image.fragment[1].ar_idx == ar_idx)
    {
        fragment_ptr = & iom_kram_proc_image.fragment[1];
    }
    else
    {
        iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
    }

    if(fragment_ptr->cons.lock)
    {
        fragment_ptr->cons.lock = 0;

        if( ! (iom_kram_proc_image.init_data.properties & IOM_KRAM_PROP_USE_CONS_EX_BUFFERS) )
        {
            /* from now ready for new output data user has treated the data (user-buffer system) */
            fragment_ptr->cons.new_data = 0;
        }
    }
    else
    {
        iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
        return IOM_BUFFER_UNLOCKED;
    }
    return IOM_OK;
}

#if defined (IOM_CFG_RTC3_UNBUFFERED)

/*******************************************************************************/
/* this function delivers a kram consumer buffer to the application            */
/*******************************************************************************/

LSA_UINT16 iom_kram_consumer_lock_unbuffered(LSA_VOID * * consumer_buffer, iom_apdu_status_t **apdu_status_ptr, LSA_UINT32 ar_idx)
{
    iom_kram_fragment_t * fragment_ptr = NULL;
    LSA_UINT16            ret_value    = IOM_OK;

    if(iom_kram_proc_image.fragment[0].ar_idx == ar_idx)
    {
        fragment_ptr = & iom_kram_proc_image.fragment[0];
    }
    else if(iom_kram_proc_image.fragment[1].ar_idx == ar_idx)
    {
        fragment_ptr = & iom_kram_proc_image.fragment[1];
    }
    else
    {
        return IOM_INVALID_BUFFER;
    }

    switch(fragment_ptr -> state)
    {
        case IOM_KRAM_AR_STATE_ESTABLISHED: /* in data ex */
        case IOM_KRAM_AR_STATE_ESTABLISHED_UNBUFFERED:
        {
            if( (!fragment_ptr->cons.lock) && (fragment_ptr -> rt_class == CM_IOCR_PROP_RT_CLASS_3) )
            {
                fragment_ptr->cons.lock = 1;

                /* return pointer to internal kram buffer without the apdu state (4 bytes at the beginning) */
                * consumer_buffer = fragment_ptr -> cons.src_buffer_ptr + sizeof(iom_apdu_status_t);

                fragment_ptr -> state = IOM_KRAM_AR_STATE_ESTABLISHED_UNBUFFERED;

                if(apdu_status_ptr)
                {
                    *apdu_status_ptr = (iom_apdu_status_t*) fragment_ptr -> cons.src_buffer_ptr;
                }

                ret_value = IOM_OK_NEW_BUFFER;
            }
            else
            {
                iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);

                /* avoid warning */
                ret_value = IOM_INVALID_BUFFER;
            }

            break;
        }

        default:

            ret_value = IOM_INVALID_BUFFER;
    }

    return ret_value;
}

/*******************************************************************************/
/* this function releases a consumer buffer from the application               */
/*******************************************************************************/

LSA_UINT16 iom_kram_consumer_unlock_unbuffered(LSA_UINT32 ar_idx)
{
    iom_kram_fragment_t *  fragment_ptr = NULL;

    if(iom_kram_proc_image.fragment[0].ar_idx == ar_idx)
    {
        fragment_ptr = & iom_kram_proc_image.fragment[0];
    }
    else if(iom_kram_proc_image.fragment[1].ar_idx == ar_idx)
    {
        fragment_ptr = & iom_kram_proc_image.fragment[1];
    }
    else
    {
        iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
    }

    if( (fragment_ptr->cons.lock) && (fragment_ptr -> rt_class == CM_IOCR_PROP_RT_CLASS_3) )
    {
        fragment_ptr->cons.lock = 0;

        /* from now ready for new output data user has treated the data (user-buffer system) */
        fragment_ptr->cons.new_data = 1;
    }
    else
    {
        iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
        return IOM_BUFFER_UNLOCKED;
    }

    return IOM_OK;
}

#endif

/*******************************************************************************/
/* this function gets new output data from the ertec200                        */
/* called in interrupt context before application wakeup                       */
/*******************************************************************************/

LSA_VOID iom_kram_update_consumer_data ()
{
    iom_kram_fragment_t *  fragment_ptr;

    for(fragment_ptr = & iom_kram_proc_image.fragment[0]; fragment_ptr <= & iom_kram_proc_image.fragment[1]; fragment_ptr++ )
    {
        switch(fragment_ptr -> state)
        {
            case IOM_KRAM_AR_STATE_ESTABLISHED: /* in data ex */
            {
                if(   (iom_kram_proc_image.init_data.properties & IOM_KRAM_PROP_USE_CONS_EX_BUFFERS) /* if there are two dest buffers then copy always */
                   || ((!fragment_ptr->cons.lock) && ( !fragment_ptr->cons.new_data ))              /* buffer not locked and data are old */
                  )
                {
                    if( (fragment_ptr -> rt_class == CM_IOCR_PROP_RT_CLASS_1) || (fragment_ptr -> rt_class == CM_IOCR_PROP_RT_CLASS_2) )
                    {
                        Unsigned32 i = 3;

                        volatile iom_kram_control_t control_01, control_02;

                        //OsIntDisable();
                        while(i--)
                        {
                            /* copy apdu state into first var */
                            control_01.both_values = * (volatile LSA_UINT32 *) (fragment_ptr -> cons.src_buffer_ptr);

                            if /* old telegram or invalid data */
                              (    (fragment_ptr -> cons.cycle_counter == control_01.as.cycle_counter)
                                || !(control_01.as.data_status & 0x4)
                              )
                            {
                                break;
                            }

                            IOM_KRAM_WAIT();

                            /* copy the full buffer with apdu state */
                            LSA_MEMCPY(fragment_ptr -> cons.dst_buffer_ptr_next, fragment_ptr -> cons.src_buffer_ptr, fragment_ptr -> cons.buffer_size);
                            
                            /* copy apdu state into second var */
                            control_02.both_values = * (volatile LSA_UINT32 *) (fragment_ptr -> cons.src_buffer_ptr);

                            if(control_01.as.cycle_counter == control_02.as.cycle_counter)
                            {
                                /* ok */
                                fragment_ptr->cons.new_data = 1;

                                fragment_ptr -> cons.cycle_counter = control_01.as.cycle_counter;

                                break;
                            }
                        }
                        //OsIntEnable();
                    }
                    else if(fragment_ptr -> rt_class == CM_IOCR_PROP_RT_CLASS_3)
                    {
                        /* copy the full buffer with apdu state */
                        //OsIntDisable();
                        LSA_MEMCPY(fragment_ptr -> cons.dst_buffer_ptr_next, fragment_ptr -> cons.src_buffer_ptr, fragment_ptr -> cons.buffer_size);
                        //OsIntEnable();

                        /* rtc3 data always new after start op */
                        fragment_ptr->cons.new_data = 1;
                    }
                }

                break;
            }

            case IOM_KRAM_AR_STATE_ESTABLISHED_UNBUFFERED:
            {
                break;
            }

            case IOM_KRAM_AR_STATE_POWERING_DOWN: /* check for user_buffer unlock */
            {
#ifndef IOM_SYNC_CR_RELEASE_BUFFERS
                iom_kram_free_mem_local(fragment_ptr);
#endif

                break;
            }

            default:
            {
                break;
            }
        }
    }
}

/*******************************************************************************/
/* this function delivers a new provider buffer to the application             */
/*******************************************************************************/

LSA_VOID iom_kram_provider_lock(LSA_VOID * * provider_buffer, LSA_UINT32 ar_idx)
{
    iom_kram_fragment_t *  fragment_ptr = NULL;

    if(iom_kram_proc_image.fragment[0].ar_idx == ar_idx)
    {
        fragment_ptr = & iom_kram_proc_image.fragment[0];
    }
    else if(iom_kram_proc_image.fragment[1].ar_idx == ar_idx)
    {
        fragment_ptr = & iom_kram_proc_image.fragment[1];
    }
    else
    {
        iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
    }

    switch(fragment_ptr -> state)
    {
        case IOM_KRAM_AR_STATE_ESTABLISHED: /* in data ex */
        {
            if(!fragment_ptr->prov.lock)
            {
                #if defined EDDI_CFG_USE_SW_IIIB_IF_PROV

                    fragment_ptr->prov.lock = 1;

                #else

                    if(fragment_ptr -> rt_class == CM_IOCR_PROP_RT_CLASS_3)
                    {
                        fragment_ptr->prov.lock = 1;
                    }
                    else
                    {
                        fragment_ptr->prov.req_buf.pBuffer = 0;
                        //fragment_ptr->prov.req_buf.Service = EDD_SRV_SRT_PROVIDER_LOCK_NEW;

                        if(eddi_ProviderBufferRequest(&fragment_ptr->prov.req_buf) == EDD_STS_OK)
                        {
                            fragment_ptr->prov.lock = 1;

                            fragment_ptr->prov.cmd_ok = 1;
                        }
                        else
                        {
                            iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
                        }

                        fragment_ptr -> prov.src_buffer_ptr_user = fragment_ptr -> prov.req_buf.pBuffer;
                    }

                #endif

                /* return pointer without the apdu state (4 byte at the beginning) */
                * provider_buffer = fragment_ptr -> prov.src_buffer_ptr_user;
            }
            else
            {
                iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
            }

            break;
        }

        default:
        {
            iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
        }
    }
}

/*******************************************************************************/
/* this function releases a provider buffer from the application           */
/*******************************************************************************/

LSA_UINT16  iom_kram_provider_unlock(LSA_UINT32 ar_idx)
{
    iom_kram_fragment_t *  fragment_ptr = NULL;
    LSA_UINT16             ret_value = IOM_OK_OLD_BUFFER;

    if(iom_kram_proc_image.fragment[0].ar_idx == ar_idx)
    {
        fragment_ptr = & iom_kram_proc_image.fragment[0];
    }
    else if(iom_kram_proc_image.fragment[1].ar_idx == ar_idx)
    {
        fragment_ptr = & iom_kram_proc_image.fragment[1];
    }
    else
    {
        iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
    }

    if(fragment_ptr->prov.lock)
    {
        #if defined EDDI_CFG_USE_SW_IIIB_IF_PROV

            /* buffer exchange */
            switch( eddi_ProviderBufferRequest_Xchange(EDD_INTERFACE_ID_MIN-1, fragment_ptr -> prov.id, &fragment_ptr -> prov.buffer_offset_user) )
            {
                case EDD_STS_OK:
                {
                    fragment_ptr->prov.new_data = 0;

                    ret_value = IOM_OK_NEW_BUFFER;

                    break;
                }
                case EDD_STS_OK_PENDING:
                {
                    /* user changed too early */
                    fragment_ptr->prov.new_data = 1;

                    break;
                }
                case EDD_STS_ERR_SEQUENCE:
                {
                    break;
                }
                default:
                {
                    iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
                }
            }

            /* new user buffer now hold in fragment_ptr -> provider_buffer_offset */
            fragment_ptr -> prov.src_buffer_ptr_user = (Unsigned8 *)(IOM_CFG_KRAM_DATA_START + fragment_ptr -> prov.buffer_offset_user+4);

        #else

            if(fragment_ptr -> rt_class == CM_IOCR_PROP_RT_CLASS_3)
            {
                IOM_ENTER_IO();

                IOM_KRAM_SWAP_BUFFERS(fragment_ptr -> prov.src_buffer_ptr_user, fragment_ptr -> prov.src_buffer_ptr_next);

                fragment_ptr->prov.new_data = 1;

                IOM_EXIT_IO();
            }
            else
            {
                if( fragment_ptr->prov.cmd_ok )
                {
                    fragment_ptr->prov.cmd_ok = 0;

                    /* fragment_ptr->prov_buf.pBuffer remains buffer address from lock service */
                    //fragment_ptr->prov.req_buf.Service    = EDD_SRV_SRT_PROVIDER_UNLOCK;

                    eddi_ProviderBufferRequest(&fragment_ptr->prov.req_buf);
                }
            }

        #endif

        fragment_ptr->prov.lock = 0;
    }
    else
    {
        iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
        return IOM_BUFFER_UNLOCKED;
    }

    return (ret_value);
}

#if defined (IOM_CFG_RTC3_UNBUFFERED)

/*******************************************************************************/
/* this function delivers a kram provider buffer to the application            */
/*******************************************************************************/
LSA_VOID iom_kram_provider_lock_unbuffered(LSA_VOID * * provider_buffer, LSA_UINT32 ar_idx)
{
    iom_kram_fragment_t * fragment_ptr = NULL;

    if(iom_kram_proc_image.fragment[0].ar_idx == ar_idx)
    {
        fragment_ptr = & iom_kram_proc_image.fragment[0];
    }
    else if(iom_kram_proc_image.fragment[1].ar_idx == ar_idx)
    {
        fragment_ptr = & iom_kram_proc_image.fragment[1];
    }
    else
    {
        iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
    }

    switch(fragment_ptr -> state)
    {
        case IOM_KRAM_AR_STATE_ESTABLISHED: /* in data ex */
        case IOM_KRAM_AR_STATE_ESTABLISHED_UNBUFFERED:
        {
            if( (!fragment_ptr->prov.lock) && (fragment_ptr -> rt_class == CM_IOCR_PROP_RT_CLASS_3) )
            {
                fragment_ptr->prov.lock = 1;

                /* return pointer without the apdu state (4 byte at the beginning) */
                /* provide the second buffer here because this one is used by the ASIC */
                * provider_buffer = fragment_ptr -> prov.src_buffer_ptr_user + fragment_ptr -> prov.buffer_size;

                fragment_ptr -> state = IOM_KRAM_AR_STATE_ESTABLISHED_UNBUFFERED;
            }
            else
            {
                iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
            }

            break;
        }

        default:
            iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
    }
}

/*******************************************************************************/
/* this function releases a provider buffer from the application           */
/*******************************************************************************/

LSA_UINT16 iom_kram_provider_unlock_unbuffered(LSA_UINT32 ar_idx)
{
    iom_kram_fragment_t *  fragment_ptr = NULL;

    if(iom_kram_proc_image.fragment[0].ar_idx == ar_idx)
    {
        fragment_ptr = & iom_kram_proc_image.fragment[0];
    }
    else if(iom_kram_proc_image.fragment[1].ar_idx == ar_idx)
    {
        fragment_ptr = & iom_kram_proc_image.fragment[1];
    }
    else
    {
        iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
    }

    if( (fragment_ptr->prov.lock) && (fragment_ptr -> rt_class == CM_IOCR_PROP_RT_CLASS_3) )
    {
        fragment_ptr->prov.new_data = 1;

        fragment_ptr->prov.lock = 0;
    }
    else
    {
        iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
        return IOM_INVALID_BUFFER;
    }
    return IOM_OK_NEW_BUFFER;
}

#endif



/*******************************************************************************/
/* this function changes the data state of the provider buffer                 */
/*******************************************************************************/

LSA_UINT32 iom_kram_provider_set_data_state  (LSA_UINT32 ar_idx, LSA_UINT8 value, LSA_UINT8 mask)
{
    LSA_RESULT  ret_val;

    iom_kram_fragment_t *  fragment_ptr = NULL;

    if(iom_kram_proc_image.fragment[0].ar_idx == ar_idx)
    {
        fragment_ptr = & iom_kram_proc_image.fragment[0];
    }
    else if(iom_kram_proc_image.fragment[1].ar_idx == ar_idx)
    {
        fragment_ptr = & iom_kram_proc_image.fragment[1];
    }
    else
    {
        /* ar does not exist anymore -> ignore */
        return(0);
    }

    ret_val = eddi_ProviderBufferRequest_SetDS(EDD_INTERFACE_ID_MIN-1, fragment_ptr -> prov.id, value, mask, iom_kram_proc_image.session_key_prim_ar);

    if( ret_val != EDD_STS_OK )
    {
        iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, ret_val);
    }

    return(0);
}

/*******************************************************************************/
/* this function sets the session key of the primary ar                        */
/*******************************************************************************/

LSA_VOID iom_kram_set_session_key_primary_arid  (LSA_UINT32 ar_idx)
{
    switch(ar_idx)
    {
        case 0:
        {
            iom_kram_proc_image.session_key_prim_ar = iom_kram_proc_image.fragment[0].prov.session_key;
            break;
        }
        case 1:
        {
            iom_kram_proc_image.session_key_prim_ar = iom_kram_proc_image.fragment[1].prov.session_key;
            break;
        }
        case 0xFF:
        {
            iom_kram_proc_image.session_key_prim_ar = 0;
            break;
        }
        default:
        {
            iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
        }
    }
}


LSA_UINT16 iom_kram_get_session_key_primary_arid  (LSA_UINT16 ARSetID)
{
    return iom_kram_proc_image.session_key_prim_ar;
}

/*******************************************************************************/
/* this function passes new input data to the ertec200                         */
/* called in interrupt context before application wakeup                       */
/*******************************************************************************/

void iom_kram_update_provider_data (void)
{
    /* if the command interface for provider buffer exchange ist set, rtc3 uses only one buffer. */
    /* after the transfer end interrupt the user buffer will be copied into the asic buffer      */
    #ifndef EDDI_CFG_USE_SW_IIIB_IF_PROV
    iom_kram_fragment_t *  fragment_ptr;

    for(fragment_ptr = & iom_kram_proc_image.fragment[0]; fragment_ptr <= & iom_kram_proc_image.fragment[1]; fragment_ptr++ )
    {
        switch(fragment_ptr -> state)
        {
            case IOM_KRAM_AR_STATE_ESTABLISHED: /* in data ex */
            {
                if( (fragment_ptr -> rt_class == CM_IOCR_PROP_RT_CLASS_3) && (fragment_ptr->prov.new_data) )
                {
                    /* copy the full buffer without apdu state - provider_buffer_size contains apdu state */
                    LSA_MEMCPY(fragment_ptr -> prov.dst_buffer_ptr, fragment_ptr -> prov.src_buffer_ptr_next, fragment_ptr -> prov.buffer_size-4);

                    fragment_ptr->prov.new_data = 0;
                }
                break;
            }

            case IOM_KRAM_AR_STATE_ESTABLISHED_UNBUFFERED: /* in data ex */
            {
                break;
            }

            case IOM_KRAM_AR_STATE_POWERING_DOWN: /* check for user_buffer unlock */
            {
                /* as this routine is not a must for being called freeing memory  */
                /* only performed during update consumer data within interrupt    */

                break;
            }

            case IOM_KRAM_AR_STATE_EMPTY:
            case IOM_KRAM_AR_STATE_AR_INFO:
            default:
            {
            	break;
            }
        }
    }
    #endif
}

/*************************************************************************************/
/* this function provides the (frame-)sizes of the CRs of an AR WITHOUT padding.     */
/* For the KRAM module, the allocated buffers sizes are rounded to 4 byte boundaries!*/
/*************************************************************************************/

LSA_BOOL iom_kram_get_iocr_buffer_sizes(LSA_UINT32 ar_idx, LSA_UINT32 *input_cr_buffer_size, LSA_UINT32 *output_cr_buffer_size)
{
    iom_kram_fragment_t *  fragment_ptr = LSA_NULL;

    if( !(input_cr_buffer_size && output_cr_buffer_size) )
    {
        iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
    }

    if(iom_kram_proc_image.fragment[0].ar_idx == ar_idx)
    {
        fragment_ptr = & iom_kram_proc_image.fragment[0];
    }
    else if(iom_kram_proc_image.fragment[1].ar_idx == ar_idx)
    {
        fragment_ptr = & iom_kram_proc_image.fragment[1];
    }

    if(fragment_ptr)
    {
        switch(fragment_ptr -> state)
        {
            case IOM_KRAM_AR_STATE_ESTABLISHED: /* in data ex */
            case IOM_KRAM_AR_STATE_ESTABLISHED_UNBUFFERED:
            {
                *output_cr_buffer_size = fragment_ptr->cons.buffer_size_no_padding-4;
                *input_cr_buffer_size  = fragment_ptr->prov.buffer_size_no_padding-4;

                break;
            }

            default:

                return LSA_FALSE;
        }
    }
    else
    {
        /* Buffer size request might belong to an AR without cyclic data. This can happen if there's a shared conflict
         * on the head module with an Advanced Startup AR involved */
        *output_cr_buffer_size = 0;
        *input_cr_buffer_size  = 0;
    }

    return LSA_TRUE;
}


//#################################################################################################
//#################################################################################################
//#################################################################################################
#ifdef IOM_CFG_IRTE400

/*******************************************************************************/
/* this function delivers a new provider buffer to the application             */
/*******************************************************************************/

LSA_VOID iom_kram_provider_lock_section(LSA_VOID * * provider_buffer, LSA_UINT32 ar_idx, LSA_UINT32 offset, LSA_UINT32 length)
{
    iom_kram_fragment_t *  fragment_ptr = NULL;

    if(iom_kram_proc_image.fragment[0].ar_idx == ar_idx)
    {
        fragment_ptr = & iom_kram_proc_image.fragment[0];
    }
    else if(iom_kram_proc_image.fragment[1].ar_idx == ar_idx)
    {
        fragment_ptr = & iom_kram_proc_image.fragment[1];
    }
    else
    {
        iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
    }

    switch(fragment_ptr -> state)
    {
        case IOM_KRAM_AR_STATE_ESTABLISHED: /* in data ex */
        {
            if(!fragment_ptr->prov.lock)
            {
                #if defined EDDI_CFG_USE_SW_IIIB_IF_PROV

                    fragment_ptr->prov.lock = 1;

                #else
                    if (EDD_STS_OK != eddi_KRAMWriteLock(EDD_INTERFACE_ID_MIN-1, offset, length))
                    {
                        iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
                    }
                    fragment_ptr->prov.lock   = 1;
                    fragment_ptr->prov.cmd_ok = 1;
                #endif

                // just use the USER-buffer (no SWAP), just allocate 1x KRAM
                // return pointer without the apdu state (4 byte at the beginning)
                *provider_buffer = fragment_ptr -> prov.dst_buffer_ptr;
            }
            else
            {
                iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
            }

            break;
        }

        default:
        {
            iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
        }
    }
}

/*******************************************************************************/
/* this function releases a provider buffer from the application           */
/*******************************************************************************/

LSA_UINT16  iom_kram_provider_unlock_section(LSA_UINT32 ar_idx)
{
    iom_kram_fragment_t *  fragment_ptr = NULL;
    LSA_UINT16             ret_value = IOM_OK_OLD_BUFFER;

    if(iom_kram_proc_image.fragment[0].ar_idx == ar_idx)
    {
        fragment_ptr = & iom_kram_proc_image.fragment[0];
    }
    else if(iom_kram_proc_image.fragment[1].ar_idx == ar_idx)
    {
        fragment_ptr = & iom_kram_proc_image.fragment[1];
    }
    else
    {
        iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
    }

    if(fragment_ptr->prov.lock)
    {
        #if defined EDDI_CFG_USE_SW_IIIB_IF_PROV

            /* buffer exchange */
            switch( eddi_ProviderBufferRequest_Xchange(EDD_INTERFACE_ID_MIN-1, fragment_ptr -> prov.id, &fragment_ptr -> prov.buffer_offset_user) )
            {
                case EDD_STS_OK:
                {
                    fragment_ptr->prov.new_data = 0;

                    ret_value = IOM_OK_NEW_BUFFER;

                    break;
                }
                case EDD_STS_OK_PENDING:
                {
                    /* user changed too early */
                    fragment_ptr->prov.new_data = 1;

                    break;
                }
                case EDD_STS_ERR_SEQUENCE:
                {
                    break;
                }
                default:
                {
                    iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
                }
            }

            /* new user buffer now hold in fragment_ptr -> provider_buffer_offset */
            fragment_ptr -> prov.src_buffer_ptr_user = (Unsigned8 *)(IOM_CFG_KRAM_DATA_START + fragment_ptr -> prov.buffer_offset_user+4);

        #else
            if( fragment_ptr->prov.cmd_ok )
            {
                fragment_ptr->prov.cmd_ok = 0;

                if (EDD_STS_OK != eddi_KRAMWriteUnLock(EDD_INTERFACE_ID_MIN-1))
                {
                    iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
                }
            }
        #endif

        fragment_ptr->prov.lock = 0;
    }
    else
    {
        iom_kram_in_fatal_error(IOM_LOCAL_ERR_MODULE, __LINE__, 0);
        return IOM_BUFFER_UNLOCKED;
    }

    return (ret_value);
}


#endif // IOM_CFG_IRTE400

// ################################################################################################


static LSA_FATAL_ERROR_TYPE iom_kram_error_descriptor;

LSA_VOID iom_kram_in_fatal_error( LSA_UINT8 error_module, LSA_UINT16 error_line, LSA_UINT32 error_code)
{
    iom_kram_error_descriptor.lsa_component_id = IOM_LSA_COMPONENT_ID;

    iom_kram_error_descriptor.module_id        = error_module;
    iom_kram_error_descriptor.line             = error_line;

    iom_kram_error_descriptor.error_code[ 0]   = error_code;
    iom_kram_error_descriptor.error_code[ 1]   =
    iom_kram_error_descriptor.error_code[ 2]   =
    iom_kram_error_descriptor.error_code[ 3]   = 0;

    iom_kram_error_descriptor.error_data_length = sizeof( iom_kram_proc_image);
    iom_kram_error_descriptor.error_data_ptr    =        &iom_kram_proc_image;

    IOM_FATAL_ERROR( &iom_kram_error_descriptor);
}

#endif  // ifdef IOM_CFG_IRTE
/*** end of file *************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
