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
/*  F i l e               &F: sock_cfg.c                                :F&  */
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

#define LSAS_MODULE_ID  27

//#include "sock_inc.h"

#include "lsas_inc.h"
#include "lsas_int.h"

/******************************************************************************
 *                                                                            *
 * prototypes in sock_sys.h                                                   *
 *                                                                            *
 ******************************************************************************/

/******************************************************************************
 * Ausgangs-Funktionen                                                        *
 ******************************************************************************/

/*=============================================================================
 * function name:  SOCK_GET_PATH_INFO
 *
 * function:       get system-pointer and detail-pointer of a communication
 *                 channel
 * Funktion: Ermitteln des SYS- und DETAIL-Pointers fuer einen
 *           Kommunikations-Kanals. Die beiden Pointer werden direkt
 *           in den Lokal-Datenbereich des SOCK geschreiben.
 *
 * parameters:     LSA_UINT16              ...  *  ret_val_ptr:
 *                                      return value: LSA_RET_OK
 *                                                    LSA_RET_ERR_SYS_PATH
 *                 LSA_SYS_PTR_TYPE        ...  *  sys_ptr_ptr:
 *                                      return value: pointer to system-pointer
 *                 PREFIX_DETAIL_PTR_TYPE  ...  *  detail_ptr_ptr:
 *                                      return value: pointer to detail-pointer
 *                 LSA_SYS_PATH_TYPE               path: path of a
 *                                                       communication channel
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID    SOCK_GET_PATH_INFO(
    LSA_UINT16                   *ret_val_ptr,
    LSA_SYS_PTR_TYPE             *sys_ptr_ptr,
    SOCK_DETAIL_PTR_TYPE         *detail_ptr_ptr,
    LSA_SYS_PATH_TYPE             path
)
{
    LSA_GET_PATH_INFO(ret_val_ptr, sys_ptr_ptr,
                      detail_ptr_ptr, path, LSAS_PATH_LAYER_SOCK);
}

/*=============================================================================
 * function name:  SOCK_RELEASE_PATH_INFO
 *
 * function:       release system-pointer and detail-pointer of a communication
 *                 channel
 * Funktion:        Path-Info wieder freigeben
 *
 * parameters:     LSA_UINT16              ...  *  ret_val_ptr:
 *                                      return value: LSA_RET_OK
 *                                                    LSA_RET_ERR_SYS_PATH
 *                 LSA_SYS_PTR_TYPE        ...  *  sys_ptr_ptr:
 *                                      return value: pointer to system-pointer
 *                 PREFIX_DETAIL_PTR_TYPE  ...  *  detail_ptr_ptr:
 *                                      return value: pointer to detail-pointer
 *                 LSA_SYS_PATH_TYPE               path: path of a
 *                                                       communication channel
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID    SOCK_RELEASE_PATH_INFO(
    LSA_UINT16             *ret_val_ptr,
    LSA_SYS_PTR_TYPE        sys_ptr,
    SOCK_DETAIL_PTR_TYPE    detail_ptr
)
{
    LSA_RELEASE_PATH_INFO(ret_val_ptr, sys_ptr, detail_ptr);
}


/*=============================================================================
 * Funktion: Kopieren globalen Speicherbereich.
 *             Die Speicherbereiche von Source und Destination duerfen nicht ueberlappen!
 *
 * Returns:  LSA_VOID
 *===========================================================================*/

LSA_VOID    SOCK_COPY_COMMON_MEM(
    LSA_VOID   *  src_ptr,
    LSA_VOID   *  dst_ptr,
    LSA_UINT16    length        /* Laenge in Bytes */
)
{
    LSAS_MEMCPY(dst_ptr, src_ptr ,length);
}

/*=============================================================================
 * function name:  SOCK_ALLOC_LOCAL_MEM
 *
 * function:       allocate a local-memory
 * Funktion: Allokieren von Lokal-Speicher fuer interne Verwaltungs-
 *           Information des SOCK.
 *
 * parameters:     SOCK_LOCAL_MEM_PTR_TYPE  ...  *  local_mem_ptr_ptr:
 *                                   return value: pointer to local-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: No memory
 *                                                 available
 *                 LSA_UINT16                         length: length of memory
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID    SOCK_ALLOC_LOCAL_MEM(
    LSA_VOID SOCK_LOCAL_MEM_PTR_TYPE   *  local_mem_ptr_ptr,
    LSA_UINT16                            length
)
{
    LSAS_ALLOC_COMP_MEM(local_mem_ptr_ptr, length, LSA_COMP_ID_SOCK);
}

/*=============================================================================
 * function name:  SOCK_FREE_LOCAL_MEM
 *
 * function:       free a local-memory
 * Funktion: Freigeben eines Lokal-Speichers.
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      local-memory
 *                 SOCK_LOCAL_MEM_PTR_TYPE  local_mem_ptr:  pointer to local-
 *                                                            memory
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  SOCK_FREE_LOCAL_MEM(
    LSA_UINT16                      *  ret_val_ptr,
    LSA_VOID SOCK_LOCAL_MEM_PTR_TYPE   local_mem_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, local_mem_ptr, LSA_COMP_ID_SOCK);
}

/*=============================================================================
 * function name:  SOCK_ENTER
 *
 * function:       set reentrance lock
 * Funktion: Beginn eines nicht unterbrechbaren Teils.
 *
 * parameters:     LSA_VOID
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  SOCK_ENTER(
    LSA_VOID
)
{
    LSAS_OS_LOCK_ON_TCPIP_TASK();
}

/*=============================================================================
 * function name:  SOCK_EXIT
 *
 * function:       cancel reentrance lock
 * Funktion: Ende des nicht unterbrechbaren Teils.
 *
 * parameters:     LSA_VOID
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  SOCK_EXIT(
    LSA_VOID
)
{
    LSAS_OS_RELEASE_TCPIP_TASK();
}

/*=============================================================================
 * function name:  SOCK_FATAL_ERROR
 *
 * function:       notify a fatal-error
 *
 * parameters:     LSA_UINT16                   length:     number of bytes
 *                 SOCK_FATAL_ERROR_PTR_TYPE  error_ptr:  pointer to
 *                                                          LSA_FATAL_ERROR
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  SOCK_FATAL_ERROR(
    LSA_UINT16                   length,
    SOCK_FATAL_ERROR_PTR_TYPE    error_ptr
)
{
    LSAS_FATAL_ERROR( error_ptr);
}

/*=============================================================================
 * function name:  SOCK_REQUEST_UPPER_DONE
 *
 * function:       returns a finished request to the upper layer
 *
 * parameters:     SOCK_UPPER_CALLBACK_FCT_PTR_TYPE sock_request_upper_done_ptr:
 *                     pointer to sock_request_upper_done_ptr, given by
 *                     sock_open_channel()
 *
 *                 SOCK_UPPER_RQB_PTR_TYPE  upper_rqb_ptr:  pointer to upper-RQB
 *                 LSA_SYS_PTR_TYPE         sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *
 *===========================================================================*/

LSA_VOID    SOCK_REQUEST_UPPER_DONE(
    SOCK_UPPER_CALLBACK_FCT_PTR_TYPE  sock_request_upper_done_ptr,
    SOCK_UPPER_RQB_PTR_TYPE           upper_rqb_ptr,
    LSA_SYS_PTR_TYPE                  sys_ptr
)
{
    sock_request_upper_done_ptr(upper_rqb_ptr);
}

/*=============================================================================
 * function name:  SOCK_DNS_LOOKUP
 *
 *
 * parameters:     name
 *
 * return value:   retval, ip
 *===========================================================================*/
#ifndef SOCK_DNS_LOOKUP
LSA_VOID  SOCK_DNS_LOOKUP(
    LSA_UINT16 * retval, LSA_UINT32 * ip, char * name
)
{
    /* declared as macro in sock_low.h */
}
#endif

/*=============================================================================
 * function name:  SOCK_DNS_QUERY
 *
 *
 * parameters:     name
 *
 * return value:   retval, ip
 *===========================================================================*/
#ifndef SOCK_DNS_QUERY
LSA_VOID  SOCK_DNS_QUERY(
    LSA_UINT16 * retval, char * name, LSA_UINT32 * ip
)
{
    /* declared as macro in sock_low.h */
}
#endif

/*=============================================================================
 * function name:  SOCK_DNS_SERVERS
 *
 * parameters:
 *
 * return value:   retval = 1, wenn mindestens ein Server in der Server-Struktur
 *===========================================================================*/
#ifndef SOCK_DNS_SERVERS
LSA_VOID  SOCK_DNS_SERVERS(
    LSA_UINT16 * retval
)
{
    /* declared as macro in sock_low.h */
}
#endif



/*=============================================================================
 * function name:  SOCK_CHECK_STACK_BLOCKED
 *
 * parameters:  cnt: Anzahl der zu pruefenden Sockets,
 *              selp: Pointer auf In-Struktur mit sock_fd
 *
 * return value:   blocking_mbcnt: Anzahl der meist verbrauchten Bloecke
 *                 blocking_sock: Sock_fd der die meisten Bloecke hat
 *===========================================================================*/

 /* Struktur fuer Socket-Select -- forward declaration*/
typedef struct sel
{
    LSA_UINT16 se_inflags;
    LSA_UINT16 se_outflags;
    LSA_INT32  se_fd;
    LSA_INT32  se_udp;
    LSA_UINT32 se_user;
    LSA_UINT32 se_2reserved;
} SOCKSEL;


SOCK_EXCHANGE_TYPE SOCK_INTERLOCKED_EXCHANGE(SOCK_EXCHANGE_TYPE volatile *ptr, SOCK_EXCHANGE_TYPE val)
{

	SOCK_EXCHANGE_TYPE ret_val;

	LSAS_SOCK_ENTER();

    ret_val = *ptr;

    *ptr = val;

    LSAS_SOCK_EXIT();

    return ret_val;
}

LSA_VOID  SOCK_REQUEST_LOCAL(
    SOCK_UPPER_RQB_PTR_TYPE rqb_ptr
)
{
    LSAS_RQB_SOCK_REQUEST_HIGH(rqb_ptr, sock_request);
}

LSA_VOID  SOCK_DO_TIMER_REQUEST(
    SOCK_UPPER_RQB_PTR_TYPE  upper_rqb_ptr
)
{
    LSAS_RQB_SOCK_REQUEST_HIGH(upper_rqb_ptr, sock_request);
}

/*=============================================================================
 * function name:  SOCK_SNMP_AGT_PARSE
 *
 * parameters:      precv_buffer, len, psnmp_buffer, snmp_len
 *
 * return value:    pret_val
 *===========================================================================*/
#ifndef SOCK_SNMP_AGT_PARSE
LSA_VOID
SOCK_SNMP_AGT_PARSE(
        LSA_UINT8 * precv_buffer,
        LSA_UINT    len,
        LSA_UINT8 * psnmp_buffer,
        LSA_UINT    snmp_len,
        LSA_UINT16   * pret_val
)
{
#if    SOCK_CFG_INTERNICHE
#else
   *pret_val =  snmp_agt_parse(precv_buffer,len,psnmp_buffer, snmp_len);
#endif
}
#endif

/*===========================================================================*/
/*====                    out functions to EDD                           ====*/
/*===========================================================================*/

LSA_UINT16 sock_version ( LSA_UINT16 length, LSA_VERSION_TYPE *version )
{
  LSA_UINT8 *p_version, *p_v;
  LSA_UINT16 i;

  /* Initialize version struct */
  LSA_VERSION_TYPE v = {
                        PNIO_LSA_COMPONENT_ID, {'L','S','A','_','S','O','C','K','\0','\0'},
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

#ifdef LSAS_CFG_USE_EXTERNAL_TCPIP_STACK 
#ifndef SOCK_SET_IPSUITE
LSA_BOOL SOCK_SET_IPSUITE(
	LSA_UINT32 interface_id,  /* id to requesting interface */
	SockIF_IPAddr ip_addr,    /* IP address to be set */
	SockIF_IPAddr net_mask,   /* associated net mask */
	SockIF_IPAddr gateway     /* associated gateway address */
)
{
	LSAS_SET_IPSUITE (interface_id, ip_addr.s_addr, net_mask.s_addr, gateway.s_addr );
}
#endif

#ifndef SOCK_GET_MIB2_IFINDEX
LSA_BOOL SOCK_SYSTEM_OUT_FCT_ATTR
SOCK_GET_MIB2_IFINDEX(
	LSA_UINT32 IfID,
	LSA_UINT16 PortID,
	LSA_UINT32* p_IfIndex
)
{
	return LSA_TRUE;
}
#endif

#ifndef SOCK_SET_MIB2_IFGROUP_DATA
LSA_BOOL SOCK_SYSTEM_OUT_FCT_ATTR
SOCK_SET_MIB2_IFGROUP_DATA(
	LSA_UINT32 interface_id,
	LSA_UINT16 PortId,
	SOCK_NAME_TYPE ifDescr
)
{
	return LSA_TRUE;
}
#endif

#ifndef SOCK_SET_MIB2_SYSGROUP_DATA
LSA_BOOL SOCK_SYSTEM_OUT_FCT_ATTR
SOCK_SET_MIB2_SYSGROUP_DATA(
	SOCK_NAME_TYPE sysName,
	SOCK_NAME_TYPE sysDescr,
	SOCK_NAME_TYPE sysContact,
	SOCK_NAME_TYPE sysLocation
)
{
	return LSA_TRUE;
}
#endif


#ifndef SOCK_TCPIP_GET_TIMEPARAMS
LSA_BOOL SOCK_SYSTEM_OUT_FCT_ATTR
SOCK_TCPIP_GET_TIMEPARAMS(
	LSA_UINT32 *ptrKEEPALIVETIME,
	LSA_UINT32 *ptrCONNECTIONTIMEOUT,
	LSA_UINT32 * ptrRETRANSTIMEOUT,
	LSA_UINT32 * ptrRETRANSCOUNT
)
{
	return LSA_TRUE;
}
#endif

#ifndef SOCK_TCPIP_SET_TIMEPARAMS
LSA_BOOL SOCK_SYSTEM_OUT_FCT_ATTR
SOCK_TCPIP_SET_TIMEPARAMS(
	LSA_UINT32 KEEPALIVETIME,
	LSA_UINT32 CONNECTIONTIMEOUT,
	LSA_UINT32 RETRANSTIMEOUT,
	LSA_UINT32 RETRANSCOUNT
)
{
	return LSA_TRUE;
}
#endif

#ifndef SOCK_GET_ANY_MAC_ADDR
LSA_BOOL SOCK_SYSTEM_OUT_FCT_ATTR
SOCK_GET_ANY_MAC_ADDR(
		LSA_UINT32*     ptr_interface_id,
		LSA_UINT8       SOCK_SYS_DATA_ATTR  *ptr_mac_addr,
		LSA_UINT32      mac_addr_size
)
{
	return LSA_TRUE;
}
#endif
#endif
/*****************************************************************************/
/*  end of file.                                                             */
/*****************************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
