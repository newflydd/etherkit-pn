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
/*  F i l e               &F: dcp_inc.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*                      Frame for file "dcp_inc.h".                          */
/*                      ==============================                       */
/*                                                                           */
/*  Include-File:                                                            */
/*  Includes of dcp and of the different systems, compilers or               */
/*  operating system.                                                        */
/*                                                                           */
/*  The current values are EXAMPLES - working for Win32 based module test    */
/*                                                                           */
/*  This file has to be overwritten during system integration, because       */
/*  some includes depend on the different system, compiler or                */
/*  operating system.                                                        */
/*                                                                           */
/*****************************************************************************/
#ifndef _DCP_INC_H                     /* ----- reinclude-protection ----- */
#define _DCP_INC_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Version                   Who  What                          */
/*  2002-07-17  P00.00.00.00_00.01.00.00  lrg  from LSA_GLOB_P02.00.00.00_00 */
/*                                             .01.04.01                     */
/*  2002-10-31  P01.00.00.00_00.01.01.01  lrg  1.Increment:Identify Response */
/*  2002-11-14  P01.00.00.00_00.02.00.00  lrg  2.Increment: GET/SET Response */
/*                                             dcp_cfg.h = first DCP include */
/*  2003-01-27  P01.00.00.00_00.03.00.00  lrg  3.Increment:                  */
/*                                             Reihefolge: pck, low, sys     */
/*                                                      -> low, sys, pck     */
/*                                             Neu: tcp_ip.h                 */
/*  2003-04-17 	P01.00.00.00_00.04.00.00  lrg  4.Increment: CD7 -> DCP       */
/*                                             Neu: dcp_pls.h                */
/*  2003-07-16 	P01.00.00.00_00.07.01.00  lrg  LSA_GLOB 2.1.0.0_0.2.1.1      */
/*  2003-12-15 	P02.00.00.00_00.01.00.00  lrg  Version 2                     */
/*  2004-02-09  P02.00.00.00_00.02.00.00  lrg  dcp_usr.h vor dcp_pck.h       */
/*  2004-06-24  P03.00.00.00_00.01.00.00  lrg  Version 3                     */
/*  2004-12-22 	P03.03.00.00_00.02.03.00  lrg  LSA_GLOB 2.1.0.0_0.4.2.1      */
/*  2005-02-16 	P03.04.00.00_00.04.00.00  lrg  LSA-Trace eingebaut           */
/*  2007-03-30  P04.00.00.00_00.01.01.02  lrg  #include tcp_ip.h entfernt    */
/*  2008-06-20  P04.00.00.00_00.80.02.03 lrg: DCP_MESSAGE                    */
/*  2008-12-04  P04.01.00.00_00.02.01.01 lrg: Package Version (PNIO)         */
/*  2010-08-24  P05.00.00.00_00.01.02.02 lrg: AP00999104 DCP_CFG_TRACE_MODE  */
/*              Always include dcp_trc.h                                     */
/*  2011-06-29  S05.01.00.00_00.03.03.02 lrg: AP01138397 ...TRACE_LEVEL_CHAT */
/*  2012-11-07  P05.02.00.00_00.04.08.01 AP01069471 DCP_REQUEST_UPPER_DONE   */
/*              and all other functions with DCP_SYSTEM_OUT_FCT_ATTR moved   */
/*              from dcp_usr.h to dcp_sys.h                                  */
/*                                                                           */
/*****************************************************************************/

/* SYSTEM Includes
*/

/* LSA Includes
*/
#include "glob_sys.h"

#include "lsas_cfg.h"
#include "ltrc_if.h"

/* Lower Layer Includes
*/
#include "edd_cfg.h"
#include "edd_usr.h"

/* DCP Includes
*/
#include "dcp_cfg.h"
#include "dcp_low.h"

#include "dcp_trc.h"
#include "dcp_usr.h"
#include "dcp_sys.h"
#include "dcp_pls.h"

/*- include system embedding layer ------------------------------------------*/
/* lsas */

#include "lsas_com.h"
#include "lsas_path_com.h"

/*****************************************************************************/
/*  end of file DCP_INC.H                                                    */
/*****************************************************************************/
#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif  /* of DCP_INC_H */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
