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
/*  F i l e               &F: edd_cfg.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Configuration for EDD:                                                   */
/*  Defines constants, types and macros for edd.                             */
/*                                                                           */
/*  This file has to be overwritten during system integration, because       */
/*  some definitions depend on the different system, compiler or             */
/*  operating system.                                                        */
/*                                                                           */
/*****************************************************************************/

#ifndef EDD_CFG_H                        /* ----- reinclude-protection ----- */
#define EDD_CFG_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif


/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDD_MESSAGE
/*  13.08.08    JS    initial version.                                       */
/*  27.09.10    JS    added parameter EDD_CFG_MAX_INTERFACE_CNT              */
/*  17.01.11    JS    removed revisions from header                          */
/*                    added EDD_CFG_MAX_PORT_CNT_ALL_INTERFACE               */
/*  31.03.11    UL    added EDD_CFG_MAX_NR_PROVIDERS                         */
/*                    added	EDD_CFG_MAX_NR_ARSETS						     */
/*  08.11.11    MA    added EDD_CFG_RX_FRAME_BUFFER_LENGTH                   */
/*  07.02.12    JS    added EDD_CFG_MAX_MRP_INSTANCE_CNT                     */
/*  22.01.14    AH    removed EDD_CFG_MAX_NR_PROVIDERS                       */
#endif
/*****************************************************************************/

/*===========================================================================*/
/*                              defines                                      */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* Maximum number of interfaces of all EDDs integrated                       */
/* Note: This parameter is intended to be used by upper components to limit  */
/*       the number of supported EDD interfaces. This parameter is not used  */
/*       by the EDD.                                                         */
/*---------------------------------------------------------------------------*/

// TODO: by dingding, EDD_CFG_MAX_INTERFACE_CNT
// #define EDD_CFG_MAX_INTERFACE_CNT        1
#define EDD_CFG_MAX_INTERFACE_CNT        2


/*---------------------------------------------------------------------------*/
/* Maximum number of ports supported by EDD API services                     */
/* Note: This must be the maximum port count possbile of a EDD device!       */
/*       This define is used to define the EDD User API array sizes.         */
/*       If one EDD supports more ports, the EDD will signal an runtime      */
/*       fatal error!                                                        */
/* Note: With EDDP we can have up to 4 ports per Device.                     */
/*---------------------------------------------------------------------------*/

#define EDD_CFG_MAX_PORT_CNT     2   // 4-> LSAS_CFG_PORT_CNT ??   /* defines array size for the EDD-API and must be set   to the maximum supported port count of the EDD. 4 for EDDI */

/*---------------------------------------------------------------------------*/
/* Maximum number of ports over all interfaces of all EDDs                   */
/* Maximum: EDD_CFG_MAX_INTERFACE_CNT * EDD_CFG_MAX_PORT_CNT                 */
/* Note: If the maximum number of all ports over all interfaces is known this*/
/*       define can be used to define the limit. Applications can use this   */
/*       define to limit local memory management.                            */
/* Note: The EDD does not use/validate this define!                          */
/*---------------------------------------------------------------------------*/

#define EDD_CFG_MAX_PORT_CNT_ALL_INTERFACE   (EDD_CFG_MAX_INTERFACE_CNT * LSAS_CFG_PORT_CNT)

/*---------------------------------------------------------------------------*/
/* Maximum number of MRP instances supported with a EDD interface            */
/*---------------------------------------------------------------------------*/

#define EDD_CFG_MAX_MRP_INSTANCE_CNT    (((LSAS_CFG_PORT_CNT-1)/2)+1)

/*---------------------------------------------------------------------------*/
/* Maximum number of MRP interconnection instances supported by EDD interface*/
/*---------------------------------------------------------------------------*/

#define EDD_CFG_MAX_MRP_IN_INSTANCE_CNT    1

/*---------------------------------------------------------------------------*/
/* Because of the zero-copy - interface we must take care of receive buffer  */
/* size requirements from the ethernetcontroller within application.         */
/*                                                                           */
/* EDD_CFG_RX_FRAME_BUFFER_LENGTH defines the maximum receive framebuffer    */
/* requirement of all Ethernetcontroller within the system. This define is   */
/* set to the EDD define EDD_FRAME_BUFFER_LENGTH which shall be used to      */
/* allocate receive buffers by any EDD application.                          */
/*                                                                           */
/* Current requirements                                                      */
/*                                                                           */
/* EDDP:         1536                                                        */
/* EDDI:         1536                                                        */
/* EDDS:         LLIF/MAC dependend                                          */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifdef LSAS_CFG_USE_EDDS
#if(PNIOD_PLATFORM ==  PNIOD_PLATFORM_RZT2_EDDS)
	#define EDD_CFG_RX_FRAME_BUFFER_LENGTH      1536
#else
	#define EDD_CFG_RX_FRAME_BUFFER_LENGTH      2048    /* according to PSI, Intel LLIF */
#endif
#else
	#define EDD_CFG_RX_FRAME_BUFFER_LENGTH      1536
#endif

/*---------------------------------------------------------------------------*/
/* Host-Byte-Order format:                                                   */
/* big-endian   : define EDD_CFG_BIG_ENDIAN                                  */
/* little-endian: undef EDD_CFG_BIG_ENDIAN  (e.g. intel,ibc32)               */
/*---------------------------------------------------------------------------*/

    #undef EDD_CFG_BIG_ENDIAN

/*---------------------------------------------------------------------------*/
/* Max number of entrys in one indication resource for cyclic SRT.           */
/* (see service EDD_SRV_SRT_IND_PROVIDE)                                     */
/* (1..0xFFFF)                                                               */
/*---------------------------------------------------------------------------*/

    #define EDD_CFG_CSRT_MAX_INDICATIONS    64

/*---------------------------------------------------------------------------*/
/* Max number of provider groups supported.                                  */
/* (1..0xFFFF)                                                               */
/*---------------------------------------------------------------------------*/

    #define EDD_CFG_CSRT_MAX_PROVIDER_GROUP 7


/*---------------------------------------------------------------------------*/
/* Max number of LSA_UINT16-Entries per EDD instance in System Redundancy    */
/* Shared Memory.                                                            */
/* (1..x): it must be >= 1 because EDD uses that for the definition of the   */
/*         array size in Shared Memory                                       */
/*---------------------------------------------------------------------------*/

#define EDD_CFG_MAX_SRSHM_SIZE       1

/*===========================================================================*/
/*                              basic attributes                             */
/*===========================================================================*/

#define EDD_ATTR_NEAR
#define EDD_ATTR_FAR
#define EDD_ATTR_SHUGE
#define EDD_ATTR_HUGE

/*===========================================================================*/
/*                         code- and data-attributes                         */
/*===========================================================================*/

/*=====  code attributes  =====*/

#define EDD_UPPER_IN_FCT_ATTR    EDD_ATTR_HUGE  /* prefix-user to prefix */
#define EDD_UPPER_OUT_FCT_ATTR   EDD_ATTR_HUGE  /* prefix to prefix-user */

/*=====  data attributes  =====*/

#define EDD_UPPER_RQB_ATTR       EDD_ATTR_HUGE  /* upper-rqb data */
#define EDD_UPPER_MEM_ATTR       EDD_ATTR_HUGE  /* upper-memory data */
#define EDD_COMMON_MEM_ATTR      EDD_ATTR_HUGE  /* greatest common data   */
                                                /* attribute (matches all)*/

/*---------------------------------------------------------------------------*/
/* Definition of RQB-HEADER:                                                 */
/*---------------------------------------------------------------------------*/
#ifdef LSA_RQB_HEADER
 #define EDD_RQB_HEADER       LSA_RQB_HEADER(EDD_UPPER_RQB_PTR_TYPE)
#else
 #error "LSA-Header not defined, please change include order."
#endif

/*---------------------------------------------------------------------------*/
/* Definition of RQB-TRAILER             */
/*                                                                           */
/* If necessary during system integration, additional parameters can be   */
/* added!                  */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#define EDD_RQB_TRAILER     LSA_RQB_TRAILER

/*---------------------------------------------------------------------------*/
/* Set and get parameter of the rqb-header                                   */
/*---------------------------------------------------------------------------*/

#define EDD_RQB_SET_NEXT_RQB_PTR(_pRQB, _Value)   LSA_RQB_SET_NEXT_RQB_PTR(_pRQB, _Value)
#define EDD_RQB_GET_NEXT_RQB_PTR(_pRQB)           LSA_RQB_GET_NEXT_RQB_PTR(_pRQB)
#define EDD_RQB_SET_PREV_RQB_PTR(_pRQB, _Value)   LSA_RQB_SET_PREV_RQB_PTR(_pRQB, _Value)
#define EDD_RQB_GET_PREV_RQB_PTR(_pRQB)           LSA_RQB_GET_PREV_RQB_PTR(_pRQB)
#define EDD_RQB_SET_OPCODE(_pRQB, _Value)         LSA_RQB_SET_OPCODE(_pRQB, _Value)
#define EDD_RQB_GET_OPCODE(_pRQB)                 LSA_RQB_GET_OPCODE(_pRQB)
#define EDD_RQB_SET_HANDLE(_pRQB, _Value)         LSA_RQB_SET_HANDLE(_pRQB, _Value)
#define EDD_RQB_GET_HANDLE(_pRQB)                 LSA_RQB_GET_HANDLE(_pRQB)
#define EDD_RQB_SET_USERID_UVAR32(_pRQB, _Value)  LSA_RQB_SET_USER_ID_UVAR32(_pRQB, _Value)
#define EDD_RQB_GET_USERID_UVAR32(_pRQB)          LSA_RQB_GET_USER_ID_UVAR32(_pRQB)
#define EDD_RQB_GET_USERID(_pRQB)                 ( (_pRQB)->user_id )
#define EDD_RQB_SET_USERID_PTR(_pRQB, _Value)     LSA_RQB_SET_USER_ID_PTR(_pRQB, _Value)
#define EDD_RQB_GET_USERID_PTR(_pRQB)             LSA_RQB_GET_USER_ID_PTR(_pRQB)
#define EDD_RQB_SET_RESPONSE(_pRQB, _Value)       LSA_RQB_SET_RESPONSE(_pRQB, _Value)
#define EDD_RQB_GET_RESPONSE(_pRQB)               LSA_RQB_GET_RESPONSE(_pRQB)

/*===========================================================================*/
/*                                 macros                                    */
/*===========================================================================*/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif //EDD_CFG_H


/*****************************************************************************/
/*  end of file edd_cfg.h/txt                                                */
/*****************************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
