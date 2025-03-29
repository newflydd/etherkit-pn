#ifndef EDDP_I2C_H                       /* ----- reinclude-protection ----- */
#define EDDP_I2C_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

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
/*  C o m p o n e n t     &C: EDDP (EthernetDeviceDriver for PN-IP)     :C&  */
/*                                                                           */
/*  F i l e               &F: eddp_i2c.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP I2C-USER-API                                */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  26.04.10    SFr   initial version.                                       */
#endif
/*****************************************************************************/


/*===========================================================================*/
/*                              constants                                    */
/*===========================================================================*/


/*===========================================================================*/
/*                               defines                                     */
/*===========================================================================*/


/*===========================================================================*/
/*                               types                                       */
/*===========================================================================*/


/* ============================================================================ */
/*                          main struct for EDDP-I2C                            */
/*          DDB-I2C Device management structure for one Device                  */
/* ============================================================================ */
typedef struct _EDDP_DDB_I2C_TYPE       EDDP_LOCAL_MEM_ATTR *       EDDP_DDB_I2C_PTR_TYPE;
typedef struct _EDDP_DDB_I2C_TYPE
{
    LSA_UINT16      I2CPortIdStored;    //stored PortId from last I2C transfer (0 = invalid)
    LSA_UINT16      I2CMuxSelectStored; //stored I2CMuxSelect from last I2C transfer (0 = invalid)
    EDD_RSP         I2CInitResult;      //result of EDDP_I2C_Init() from startup

} EDDP_DDB_I2C_TYPE;


/*===========================================================================*/
/*                              macros                                       */
/*===========================================================================*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_I2C_Init                               +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB           +*/
/*+                                                                         +*/
/*+  Result                :    LSA_RESULT                                  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_HW                                            +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: Initialization of most common I2C registers.              +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_I2C_Init(EDDP_LOCAL_DDB_PTR_TYPE const pDDB);


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_I2C_DeInit                             +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB           +*/
/*+                                                                         +*/
/*+  Result                :    LSA_RESULT                                  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: DeInitialization of most common I2C registers.            +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_I2C_DeInit(EDDP_LOCAL_DDB_PTR_TYPE const pDDB);


/*
Note:
The read/write functions are declared within eddp_sys.h

Implementation example:
=======================

#define I2C_DEVADDR_E2          0xA0   //first E2 page  (internal addresses see defines below)
#define I2C_E2_SIG_STRING_OFS   20
#define I2C_E2_SIG_STRING_LEN   10
#define I2C_DEVADDR_DMI         0xA2   //second E2 page (internal addresses see defines below)
#define I2C_DMI_CUSTOMER_OFS    128

static LSA_UINT8 I2C_Buf[I2C_E2_SIG_STRING_LEN];

EDDP_HANDLE hIDB;               //IDB handle

...
    pRQB->Service = EDDP_SRV_DEV_OPEN;
    eddp_system(pRQB);      //calls EDDP_I2C_Init() internally
	hIDB = pHDB->pDDB;      //get IDB handle (refer to service EDDP_SRV_DEV_OPEN)
...

   // ***** try to read the signature string
   if (eddp_I2C_Read_Offset(hIDB, 1, 1, I2C_DEVADDR_E2, 1, I2C_E2_SIG_STRING_OFS, 0, I2C_E2_SIG_STRING_LEN, 0, 0, &I2C_Buf[0]) == EDD_STS_OK)
   { // ***** print the string: 'AVAGO TECH'
     I2C_Buf[I2C_E2_SIG_STRING_LEN] = 0;
     printf("return String: '%s'\n", &I2C_Buf[0]);

     // ***** initialization of two bytes in user writeable space
     I2C_Buf[0] = '0';
     I2C_Buf[1] = '0';
	 if (eddp_I2C_Write_Offset(hIDB, 1, 1, I2C_DEVADDR_DMI, 1, I2C_DMI_CUSTOMER_OFS, 0, 0, 0, 2, &I2C_Buf[0]) == EDD_STS_OK)
     { // ***** '00' written --> read back
       if (eddp_I2C_Read_Offset(hIDB, 1, 1, I2C_DEVADDR_DMI, 1, I2C_DMI_CUSTOMER_OFS, 0, 0, 0, 2, &I2C_Buf[0]) == EDD_STS_OK)
       { // ***** print the string: '00'
         I2C_Buf[2] = 0;
         printf("return String: '%s'\n", &I2C_Buf[0]);
         // ***** patch two bytes in user writeable space to see write modification
         I2C_Buf[0] = '1'; 
         I2C_Buf[1] = '1';
	     if (eddp_I2C_Write_Offset(hIDB, 1, 1, I2C_DEVADDR_DMI, 1, I2C_DMI_CUSTOMER_OFS, 0, 0, 0, 2, &I2C_Buf[0]) == EDD_STS_OK)
         { // ***** '11' written --> read back
           if (eddp_I2C_Read_Offset(hIDB, 1, 1, I2C_DEVADDR_DMI, 1, I2C_DMI_CUSTOMER_OFS, 0, 0, 0, 2, &I2C_Buf[0]) == EDD_STS_OK)
           { // ***** print the string: '11'
             I2C_Buf[2] = 0;
             printf("return String: '%s'\n", &I2C_Buf[0]);
           }
         }
       }
     }
   }
*/


/*****************************************************************************/
/*  end of file eddp_i2c.h                                                   */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_I2C_H */
