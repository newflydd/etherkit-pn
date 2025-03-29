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
/*  F i l e               &F: oha_inc.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Include-File:                                                            */
/*  Includes of OHA and of the different systems, compilers or               */
/*  operating system.                                                        */
/*                                                                           */
/*  This file has to be overwritten during system integration, because       */
/*  some includes depend on the different system, compiler or                */
/*  operating system.                                                        */
/*                                                                           */
/*****************************************************************************/

#ifndef OHA_INC_H                        /* ----- reinclude-protection ----- */
#define OHA_INC_H


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
/*  20.07.04    VE    initial version.                                       */
/*                                                                           */
/*****************************************************************************/


/*-inlcude your global systsem header here ----------------------------------*/


/* the following LSA-headers or headers including these headers have to      */
/* be included here.                                                         */

#include "glob_sys.h"


/*---------------------------------------------------------------------------*/
#include "lsas_cfg.h"

#include "oha_cfg.h"

/* interface to EDD -------------------------------------------------------- */
/*   #include "edd_cfg.h"  */
/*   #include "edd_usr.h"  */

#include "edd_inc.h"


/* interface to NARE ------------------------------------------------------- */
#ifndef OHA_CFG_NO_NARE
#include "nare_inc.h"
#endif

/* interface to MRP -------------------------------------------------------- */
#ifndef OHA_CFG_NO_MRP
#include "mrp_cfg.h"
#include "mrp_sys.h"
#include "mrp_usr.h"
#include "mrp_out.h"
#include "mrp_pls.h"
#endif

#ifndef OHA_CFG_NO_SOCK
/* interface to SOCK ------------------------------------------------------- */
#include "sock_inc.h"
#endif

#ifdef OHA_CFG_USE_SNMPX
/* interface to SNMPX ------------------------------------------------------ */
#include "snmpx_inc.h"
#endif

/* interface to DCP -------------------------------------------------------- */
#include "dcp_inc.h"

/* interface to LLDP ------------------------------------------------------- */
#include "lldp_inc.h"

/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
#include "oha_usr.h"

#include "oha_sys.h"
#include "oha_trc.h"
#include "oha_pck.h"

#include "oha_low.h"

/*- include system embedding layer ------------------------------------------*/
/* lsas */

#include "lsas_com.h"
#include "lsas_path_com.h"

    LSA_UINT16 oha_db_write_ip_suite(
        LSA_UINT16                    NicId,
        OHA_IP_TYPE					  IpAddrPtr,
        OHA_IP_TYPE					  SubnetMaskPtr,
        OHA_IP_TYPE  				  RouterAddrPtr,
        LSA_INT                       AddrLen,
        LSA_UINT8                     AccessControl,
        LSA_BOOL                      Remanent );
    LSA_UINT16 oha_db_write_station_name(
        LSA_UINT16                    NicId,
        OHA_COMMON_MEM_U8_PTR_TYPE    NamePtr,
        LSA_INT                       NameLen,
        LSA_UINT8                     AccessControl,
        LSA_BOOL                      Remanent
        );

/*****************************************************************************/
/*  end of file OHA_INC.H                                                    */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif


#endif  /* of OHA_INC_H */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
