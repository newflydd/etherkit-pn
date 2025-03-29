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
/*  F i l e               &F: gdma_inc.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/

#ifndef GDMA_INC_H_
#define GDMA_INC_H_

/* LSA Includes
*/
#include "glob_sys.h"

#include "lsas_cfg.h"

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EBx00)
//#include "ertec200p_reg.h"
//#include "ertec_irq.h"
#include "hama_com.h"
#include "ertec_reg.h"
#endif

#include "bspadapt.h"

#include "gdma_cfg.h"
#include "gdma_com.h"

//#pragma ghs section bss=".bss_sys_gdma"
//#pragma ghs section text=".text_sys_gdma"
//#pragma ghs section rodata=".rodata_sys_gdma"

#endif /* GDMA_INC_H_ */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
