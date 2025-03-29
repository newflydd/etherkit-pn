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
/*  F i l e               &F: eep_string.h                              :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Treatment functions for the internal EEPROM of the BA of the IM155-6PN,  */
/*                                                                           */
/*  - module-specific init string for the EEPROM                             */
/*    is included in "e check.c"                                             */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  H i s t o r y :                                                          */
/*                                                                           */
/*  date      person  comment                                                */
/*  -----------------------------------------------------------------------  */
/*                                                                           */
/*  2003-05-22   KG   porting to MSP430                                      */
/*  02.06.03  MaSi    define only for definition, not for initialization     */
/*  12.08.03  MaSi    Attribute now set via define                           */
/*                                                                           */
/*****************************************************************************/


const UBYTE eeprom_init_string[INIT_STRING_LEN] =
{
  'E','T','2','0','0','S','P','-',
  'B','A','_','C','U',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' '
};


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
