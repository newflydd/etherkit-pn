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
/*  F i l e               &F: lsas_sys.h                                :F&  */
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


/**
 * @file    lsas_sys.h
 * @brief   brief description
 * @author  cn3dit09
 * @version
 * @date    30.03.2012
 *
 * long description
 *
 */

/*****************************************************************************/

#ifndef LSAS_SYS_H_
#define LSAS_SYS_H_

/****************************************************************************

        PATH-Layer related typedefs

*****************************************************************************/
/* basic pointer defines */

typedef LSA_VOID   * LSAS_VOID_PTR;
typedef LSA_UINT8  * LSAS_UNSIGNED8_PTR;
typedef LSA_UINT16 * LSAS_UNSIGNED16_PTR;
typedef LSA_UINT32 * LSAS_UNSIGNED32_PTR;


/*****************************************************************************/
/* system service
*/


/*
    path (call parameter):
*/


typedef LSA_UINT32 LSAS_PATH_TYPE;


/*
    layer (call parameter):
*/


typedef LSA_UINT32 LSAS_LAYER_TYPE;


/*
    system_ptr_ptr (call, return parameter):
*/


typedef struct
{
    LSAS_PATH_TYPE  path;
    LSAS_LAYER_TYPE layer;

}   LSAS_STRUC_SYSTEM_PTR;



typedef LSAS_STRUC_SYSTEM_PTR   LSAS_SYSTEM_PTR;
typedef LSAS_SYSTEM_PTR       * LSAS_SYSTEM_PTR_PTR;

/*
    detail_ptr_ptr (call, return parameter):
*/

typedef LSAS_VOID_PTR           LSAS_DETAIL_PTR;
typedef LSAS_DETAIL_PTR       * LSAS_DETAIL_PTR_PTR;

typedef struct
{
    LSA_UINT32                  hSysDev;
    LSA_VOID                   *hDDB;
    LSA_UINT32                  HwPortIndex;
}   LSAS_E400_LED_PARAM;

typedef struct lsas_sys_tag
{
	LSA_UINT16 hd_nr;
	LSA_UINT16 pnio_if_nr;
	LSA_UINT16 edd_comp_id;
	LSA_BOOL   shm_if;
} LSAS_SYSDEV_TYPE, *LSAS_SYSDEV_TYPE_PTR;

/* function prototypes that are used in other cfg files */

LSA_VOID  lsas_set_pll_mode_out( LSA_UINT16                      * result,
                                 LSA_UINT32                        pIRTE,
                                 LSA_UINT32                        location,
                                 LSA_UINT16                        pllmod);

#ifndef LSAS_EDDI_I2C_SCL_LOW_HIGH
LSA_VOID  LSAS_EDDI_I2C_SCL_LOW_HIGH( LSA_UINT8         const  Level,
                                      LSA_UINT32        const  sys_ptr );
#endif

#ifndef LSAS_EDDI_I2C_SDA_LOW_HIGH
LSA_VOID  LSAS_EDDI_I2C_SDA_LOW_HIGH( LSA_UINT8         const  Level,
                                      LSA_UINT32        const  sys_ptr );
#endif

#ifndef LSAS_EDDI_I2C_SDA_READ
LSA_VOID  LSAS_EDDI_I2C_SDA_READ( LSA_UINT8           *  const  value_ptr,
                                  LSA_UINT32             const  sys_ptr );
#endif

#ifndef LSAS_EDDI_I2C_SELECT
LSA_VOID LSAS_EDDI_I2C_SELECT( LSA_UINT8           *  const  ret_val_ptr,
                               LSA_UINT32             const  sys_ptr,
                               LSA_UINT16             const  PortId,
                               LSA_UINT16             const  I2CMuxSelect);
#endif

#ifndef LSAS_EDDI_LL_LED_BLINK_BEGIN
LSA_VOID  LSAS_EDDI_LL_LED_BLINK_BEGIN( LSA_UINT32                  hSysDev,
                                        LSA_UINT32                  phyTransceiver,
                                        LSA_UINT32                  HwPortIndex
                                        );
#endif


#ifndef LSAS_EDDI_LL_LED_BLINK_END
LSA_VOID  LSAS_EDDI_LL_LED_BLINK_END(   LSA_UINT32                  hSysDev,
                                        LSA_UINT32                  phyTransceiver,
                                        LSA_UINT32                  HwPortIndex
                                        );
#endif

#ifndef LSAS_EDDI_LL_LED_BLINK_SET_MODE
LSA_VOID LSAS_EDDI_LL_LED_BLINK_SET_MODE( LSA_UINT32                hSysDev,
                                          LSA_VOID                  *hDDB,
                                          LSA_UINT32                HwPortIndex,
                                          LSA_UINT16                LEDMode
                                          );
#endif

#ifndef LSAS_POF_FO_LED
LSA_VOID  LSAS_POF_FO_LED( LSA_UINT16        const PortId,
                           LSA_UINT8         const ON,
                           LSA_SYS_PTR_TYPE  sys_ptr);
#endif

#ifndef LSAS_GET_UTC100NS_CLKSEQ
LSA_VOID LSAS_GET_UTC100NS_CLKSEQ(  LSA_UINT32 * utc_low_ptr,
                                    LSA_UINT32 * utc_high_ptr,
                                    LSA_UINT16 * clkseq_ptr
                                  );
#endif


#endif /* LSAS_SYS_H_ */

/*** end of file *************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
