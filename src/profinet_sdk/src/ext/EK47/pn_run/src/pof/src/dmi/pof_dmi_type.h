#ifndef POF_DMI_TYPE_H      /* ----- reinclude-protection ----- */
#define POF_DMI_TYPE_H

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
/*  C o m p o n e n t     &C: POF (Polymer Optical Fiber)               :C&  */
/*                                                                           */
/*  F i l e               &F: pof_dmi_type.h                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Internal headerfile                                                      */
/*  Defines internal constants, types, data, macros and prototyping for      */
/*  POF.                                                                     */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
/*****************************************************************************/

#ifdef __cplusplus      /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/
#define POF_STRESSED_LINK_PENALTY_0_1_DB    16      //= 1,6 dB

/* Flags for PROCESS-OMA */
#define POF_FLAG_RX_OMA_MARGIN_ALARM        0x20    //in Flagbit_113
#define POF_FLAG_RX_OMA_MARGIN_WARN         0x20    //in Flagbit_117

/* Digital Diagnostic Memory Map, DMI - MAP */
#define I2C_DMI_MAP_A0_SIZE_8               256UL
#define I2C_DMI_MAP_A2_SIZE_16              128UL
#define I2C_DMI_MAP_A2_SIZE_8               256UL

typedef struct _POF_DMI_MAP_A2_TYPE
{
  /*  00       */ LSA_UINT16   Temp_H_alarm;     
  /*  02       */ LSA_UINT16   Temp_L_alarm;
  /*  04       */ LSA_UINT16   Temp_H_warning;     
  /*  06       */ LSA_UINT16   Temp_L_warning;

  /*  08       */ LSA_UINT16   VCC_H_alarm;     
  /*  10       */ LSA_UINT16   VCC_L_alarm;
  /*  12       */ LSA_UINT16   VCC_H_warning;     
  /*  14       */ LSA_UINT16   VCC_L_warning;

  /*  16       */ LSA_UINT16   Tx_Bias_H_alarm;     
  /*  18       */ LSA_UINT16   Tx_Bias_L_alarm;
  /*  20       */ LSA_UINT16   Tx_Bias_H_warning;     
  /*  22       */ LSA_UINT16   Tx_Bias_L_warning;

  /*  24       */ LSA_UINT16   Tx_Pwr_H_alarm;     
  /*  26       */ LSA_UINT16   Tx_Pwr_L_alarm;
  /*  28       */ LSA_UINT16   Tx_Pwr_H_warning;     
  /*  30       */ LSA_UINT16   Tx_Pwr_L_warning;

  /*  32       */ LSA_UINT16   Rx_Pwr_H_alarm;     
  /*  34       */ LSA_UINT16   Rx_Pwr_L_alarm;
  /*  36       */ LSA_UINT16   Rx_Pwr_H_warning;     
  /*  38       */ LSA_UINT16   Rx_Pwr_L_warning;

  /*  40       */ LSA_UINT16   PD_warning;     
  /*  42       */ LSA_UINT16   PD_1_5_dB_warning;
  /*  44       */ LSA_UINT16   PD_4_5_dB_warning;     
  /*  46       */ LSA_UINT16   PD_7_5_dB_warning;

  /*  48 - 55  */ LSA_UINT8    Reserved48_55[8];
  /*  56 - 94  */ LSA_UINT8    Note_6[39];
  /*  95       */ LSA_UINT8    Checksum;

  /*  96       */ LSA_UINT16   RealTime_Temperature;
  /*  98       */ LSA_UINT16   RealTime_VCC;
  /* 100       */ LSA_UINT16   RealTime_Tx_Bias;
  /* 102       */ LSA_UINT16   Calibrated_Tx_Pwr;
  /* 104       */ LSA_UINT16   RealTime_Rx_Pwr;
  /* 106       */ LSA_UINT8    RealTime_Rx_OMA_Margin;
  /* 107 - 109 */ LSA_UINT8    Reserved_107_109[3];
  /* 110       */ LSA_UINT8    StatusControl;
  /* 111       */ LSA_UINT8    Reserved_111;
  /* 112       */ LSA_UINT8    Flagbit_112;
  /* 113       */ LSA_UINT8    Flagbit_113;
  /* 114 - 115 */ LSA_UINT8    Reserved114_115[2];
  /* 116       */ LSA_UINT8    Flagbit_116;
  /* 117       */ LSA_UINT8    Flagbit_117;
  /* 118       */ LSA_UINT8    Reserved_118;
  /* 119 - 127 */ LSA_UINT8    Vendorspecific_119_127[9];
  /* 128 - 247 */ LSA_UINT8    Customerspecific[120];
  /* 248 - 255 */ LSA_UINT8    Vendorspecific_248_255[8];

  /* 256 Length */

} POF_DMI_MAP_A2_TYPE;

typedef struct _SFP_DMI_MAP_A2_TYPE    //?? adjust
{
  /*  00       */ LSA_UINT16   Temp_H_alarm;     
  /*  02       */ LSA_UINT16   Temp_L_alarm;
  /*  04       */ LSA_UINT16   Temp_H_warning;     
  /*  06       */ LSA_UINT16   Temp_L_warning;

  /*  08       */ LSA_UINT16   VCC_H_alarm;     
  /*  10       */ LSA_UINT16   VCC_L_alarm;
  /*  12       */ LSA_UINT16   VCC_H_warning;     
  /*  14       */ LSA_UINT16   VCC_L_warning;

  /*  16       */ LSA_UINT16   Tx_Bias_H_alarm;     
  /*  18       */ LSA_UINT16   Tx_Bias_L_alarm;
  /*  20       */ LSA_UINT16   Tx_Bias_H_warning;     
  /*  22       */ LSA_UINT16   Tx_Bias_L_warning;

  /*  24       */ LSA_UINT16   Tx_Pwr_H_alarm;     
  /*  26       */ LSA_UINT16   Tx_Pwr_L_alarm;
  /*  28       */ LSA_UINT16   Tx_Pwr_H_warning;     
  /*  30       */ LSA_UINT16   Tx_Pwr_L_warning;

  /*  32       */ LSA_UINT16   Rx_Pwr_H_alarm;     
  /*  34       */ LSA_UINT16   Rx_Pwr_L_alarm;
  /*  36       */ LSA_UINT16   Rx_Pwr_H_warning;     
  /*  38       */ LSA_UINT16   Rx_Pwr_L_warning;

  /*  40       */ LSA_UINT16   PD_warning;     
  /*  42       */ LSA_UINT16   PD_1_5_dB_warning;
  /*  44       */ LSA_UINT16   PD_4_5_dB_warning;     
  /*  46       */ LSA_UINT16   PD_7_5_dB_warning;

  /*  48 - 55  */ LSA_UINT8    Reserved48_55[8];
  /*  56 - 94  */ LSA_UINT8    Note_6[39];
  /*  95       */ LSA_UINT8    Checksum;

  /*  96       */ LSA_UINT16   RealTime_Temperature;
  /*  98       */ LSA_UINT16   RealTime_VCC;
  /* 100       */ LSA_UINT16   RealTime_Tx_Bias;
  /* 102       */ LSA_UINT16   Calibrated_Tx_Pwr;
  /* 104       */ LSA_UINT16   RealTime_Rx_Pwr;
  /* 106       */ LSA_UINT8    RealTime_Rx_OMA_Margin;
  /* 107 - 109 */ LSA_UINT8    Reserved_107_109[3];
  /* 110       */ LSA_UINT8    StatusControl;
  /* 111       */ LSA_UINT8    Reserved_111;
  /* 112       */ LSA_UINT8    Flagbit_112;
  /* 113       */ LSA_UINT8    Flagbit_113;
  /* 114 - 115 */ LSA_UINT8    Reserved114_115[2];
  /* 116       */ LSA_UINT8    Flagbit_116;
  /* 117       */ LSA_UINT8    Flagbit_117;
  /* 118       */ LSA_UINT8    Reserved_118;
  /* 119 - 127 */ LSA_UINT8    Vendorspecific_119_127[9];
  /* 128 - 247 */ LSA_UINT8    Customerspecific[120];
  /* 248 - 255 */ LSA_UINT8    Vendorspecific_248_255[8];

  /* 256 Length */

} SFP_DMI_MAP_A2_TYPE;


typedef union _I2C_A2_MIRROR_TYPE
{
  LSA_UINT8           Array_8 [I2C_DMI_MAP_A2_SIZE_8];
  LSA_UINT16          Array_16[I2C_DMI_MAP_A2_SIZE_16];
  union
  {
      POF_DMI_MAP_A2_TYPE ElementPOF;                   /* Description see SFF-8472 MSA */
      SFP_DMI_MAP_A2_TYPE ElementSFP;                   /* Description see SFF-8472*/
  } u;
} I2C_A2_MIRROR_TYPE;

typedef struct _I2C_DMI_MAP_TYPE
{
  LSA_UINT8           A0_Mirror[I2C_DMI_MAP_A0_SIZE_8];
  I2C_A2_MIRROR_TYPE  A2_Hardware;
  I2C_A2_MIRROR_TYPE  A2_Mirror;
  LSA_BOOL            SFP_AlarmFlagsSupported;
  LSA_BOOL            SFP_TX_FaultSupported;
  LSA_BOOL            SFP_RX_LossSupported;
} I2C_DMI_MAP_TYPE;

/*===========================================================================*/
/*                                prototyping                                */
/*===========================================================================*/


/*****************************************************************************/
/*  end of file pof_dmi_type.h                                               */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of POF_DMI_TYPE_H */
