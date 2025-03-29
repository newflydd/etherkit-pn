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
/*  F i l e               &F: usriod_main_isoapp.c                      :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Main application program for IRT with an isochronous application         */
/*  uses the Standard Interface (SI) for IO data access                      */
/*                                                                           */
/*  Features:                                                                */
/*    - sets GPIO's 5 and 7 according to ISOM-record 0x8030                  */
/*                                                                           */
/*    - includes simple terminal application, it performs                    */
/*        * messages via printf                                              */
/*        * starts executing typical API-commands via terminal-keyboard      */
/*        * connect terminal (e.g. Hyperterminal) via RS232 interface        */
/*                                                                           */
/*  To use this application example, set #define EXAMPL_DEV_CONFIG_VERSION 3 */
/*  in file \application\usriod_app.h                                        */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*  note: this application is a simple demo application. To keep it simple,  */
/*        the return values of the IodApixx functions are often not analyzed.*/
/*        But it is strongly recommended, to do this in a real application.  */
/*                                                                           */
/*  THIS MODULE HAS TO BE MODIFIED BY THE PNIO USER                          */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*                                                                           */
/*  Date        Version        Who  What                                     */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/

// *--------------------------------------------------------------------------
// *  FUNCTIONAL DESCRIPTION
// *
// *  This example consists of 2 tasks and one callback-function:
// *    -  a main task (mainAppl() function) , that starts the PNIO stack, plugs
// *       all modules and submodules and waits on console input afterwards.
// *       Console input commands can be used for example plug/pull module/submodule,
// *       send alarm, activate/passivate device,...
// *
// *    -  a cyclic task Task_CycleIO(), that performs IO data access. This task waits on a
// *       trigger event in an endless loop. For every trigger event one
// *       IO-read and  IO-write is performed.
// *
// *    -  a callback function, which is connected to a TransEnd-interrupt.
// *       When executed, the callback function sends a trigger via message to Task_CycleIO,
// *       that starts one IO data exchange in Task_CycleIO().
// *
// *   example settings:
// *   =================
// *         DAP:           Slot 0, Subslot 1,              has own IM0 data
// *         Network-IF:    Slot 0, Subslot 0x8000,         no own IM0 data
// *         Network Ports: Slot 0, Subslot 0x8001, 0x8002  no own IM0 data
// *         Input Module:  Slot 1, Subslot 1, 64 Byte input IRT,  ModID = 0x50, no own IM0 data
// *         Output Module: Slot 2, Subslot 1, 64 Byte output IRT, ModID = 0x51, no own IM0 data
// *
// *
// *--------------------------------------------------------------------------
// * NOTES:
// *  1) RT and IRT have the same application interface for IO data exchange.
// *
// *  2) In our example only the DAP has its own IM0 data (this is mandatory). All other modules
// *     (PDEV, IO-modules) don't have its own IM0 data (this is an option), so they have to respond
// *     with the IM data of the next higher level. In our example only the DAP has IM0 data,
// *     so all other subslots respond with the IM0 data of the DAP.
// *
// *--------------------------------------------------------------------------
#include "compiler.h"
#include "usriod_cfg.h"
#include "usrapp_cfg.h"   // example application spec. configuration settings

#if (EXAMPL_DEV_CONFIG_VERSION == 3)
#include "version_dk.h"
#include "usriod_cfg.h"
#include "pniousrd.h"
#include "bspadapt.h"
#include "iodapi_event.h"
#include "os.h"
#include "os_taskprio.h"
#include "usriod_diag.h"
#include "usriod_utils.h"
#include "usriod_im_func.h"
#include "pnio_trace.h"
#include "iod_cfg1.h"
#include "iodapi_rema.h"
#include "nv_data.h"
#include "PnUsr_Api.h"
#include "logadapt.h"
#include "perform_measure.h"
#include "stdint.h"
#include "bsp_api.h"
#include "r_ioport.h"

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P)
        #include "hama_com.h"
        #include <cyg/hal/ertec200p_reg.h>
        #include <cyg/kernel/kapi.h>
        #include <cyg/hal/hama_timer.h>
    #endif

/*===========================================================================*/

#if ( (MODULE_ID_DAP_1PORT == MODULE_ID_DAP) && ((IOD_INCLUDE_MRPD==1) || (IOD_INCLUDE_MRP==1)) )
        #error ("DAP4 does not support MRP and MRPD features")
    #endif
// *------------ external data  ----------*
extern PNIO_UINT32 PnioLogDest;            // destination for logging messages
extern PNIO_UINT8 OutData[NUMOF_SLOTS][NUMOF_SUBSLOTS + 1][NUMOF_BYTES_PER_SUBSLOT];
extern PNIO_UINT8 OutDatIops[NUMOF_SLOTS][NUMOF_SUBSLOTS + 1];
extern PNIO_UINT8 InData[NUMOF_SLOTS][NUMOF_SUBSLOTS + 1][NUMOF_BYTES_PER_SUBSLOT];
extern ioport_instance_ctrl_t g_ioport_ctrl;

// *------------ external functions  ----------*
extern void PrintAllUsedIoData(void);              // print io data on the terminal
extern void ChangeAllInputData(PNIO_UINT8 DiffVal);              // change io data values
extern PNIO_UINT32 TcpReceiveAndFlashFirmware(void);
#if(PNIO_TRACE != PNIO_TRACE_NONE)
    extern PNIO_UINT32 TcpTraceUpload (void);
#endif

// *------------ internal functions  ----------*

// *------------ public data  ----------*

// *------------ application synchronization  ----------*
PNIO_ISO_OBJ_HNDL IsoGpioNewCycHndl = 0;
PNIO_ISO_OBJ_HNDL ApplTransEndHndl = 0;

// *------------ local data  ----------*
static PNUSR_DEVICE_INSTANCE PnUsrDev;
static PNIO_UINT32 LastRemoteApduStatus = 0;

#if EXAMPLE_IPAR_SERVER
/* upload alarm for iPar server*/
static PNIO_UINT8 UploadAlarmData[] = { 0x0F, 0x02,                          // blocktype
		0x00, 0x14,                          // length without blocktype, length
		0x01, 0x00,                          // version
		0x00, 0x00,                          // padding
		0x01, 0x00, 0x00, 0x01,                // iPar_Req_Header (01 = upload)
		0x00, 0x00, 0x00, 0x04,                // Max_Segm_Size
		0x00, 0x00, 0x00, EXAMPL_IPAR_REC_IND, // Transfer_Index (record index)
		0x00, 0x00, 0x00, EXAMPL_IPAR_UPL_LEN  // number of bytes to store
		};

/* retrieval alarm for iPar server */
static PNIO_UINT8 RetrievalAlarmData[] = { 0x0F, 0x02,                          // blocktype
		0x00, 0x14,                          // length without blocktype, length
		0x01, 0x00,                          // version
		0x00, 0x00,                          // padding
		0x01, 0x00, 0x00, 0x03,                // iPar_Req_Header  (03 = retrieval)
		0x00, 0x00, 0x00, 0x04,                // number of bytes to store in iPar server
		0x00, 0x00, 0x00, EXAMPL_IPAR_REC_IND, // Transfer_Index  (record index = 50h in this example)
		0x00, 0x00, 0x00, EXAMPL_IPAR_RTV_LEN  // max. number of bytes to restore, 0= take all from upload
		};
#endif

// *-------------------------------------------------------------------------------------------
// * list of IO subslot (including also PDEV subslots), that shall be plugged during startup.
// *
// * list order:  DAP first, then PDEV, then IO submodules:
// *    DAP                              (mandatory)
// *    PDEF-Interface                   (mandatory)
// *    PDEV port 1 .... PDEV port n     (mandatory)
// *    IO submodules                    (optional)
// *
// *
// * Note, that I&M0 support for the DAP is mandatory, but optional for all other submoduls.
// * In this case they have to respond to an IM0 read request with the IM0 data of the DAP, that is
// * a proxy for the device related IM0 data here.
// *
// * IO subslots can optionally be plugged or pulled later.
// * DAP and PDEV subslots can not be pulled.
// *
// *
// * RESTRICTIONS  (see also example IoSubList[] below)
// * ============
// * 1. exact one submodule must be the proxy for the device (must be INTERFACE subslot 0x8000)
// * 2. IM1...4 is supported
// *
// *
// *-------------------------------------------------------------------------------------------
// TODO: by dingding, change IoSubList
static PNIO_SUB_LIST_ENTRY *IoSubList;
//static PNIO_SUB_LIST_ENTRY IoSubList[] = {
//// Api  Slot	Subslot,    ModId,              SubId,                InLen,  OutLen, I&M0 support
//		{ 0, 0, 1, MODULE_ID_DAP, SUBMOD_ID_DEFAULT, 0, 0, PNIO_IM0_SUBMODULE + PNIO_IM0_DEVICE }, { 0, 0, 0x8000, MODULE_ID_DAP, SUBMOD_ID_PDEV_IF, 0, 0, PNIO_IM0_SUBMODULE }, // PDEV interface
//		{ 0, 0, 0x8001, MODULE_ID_DAP, SUBMOD_ID_PDEV_PORT, 0, 0, PNIO_IM0_SUBMODULE },   // PDEV port1
//#if (IOD_CFG_PDEV_NUMOF_PORTS >= 2)
//		{ 0, 0, 0x8002, MODULE_ID_DAP, SUBMOD_ID_PDEV_PORT, 0, 0, PNIO_IM0_SUBMODULE },   // PDEV port2
//#endif
//#if (IOD_CFG_PDEV_NUMOF_PORTS >= 3)
//		{  0,   0,      0x8003,     MODULE_ID_DAP,  	SUBMOD_ID_PDEV_PORT,  0,      0,      PNIO_IM0_SUBMODULE},   // PDEV port3
//		#endif
//#if (IOD_CFG_PDEV_NUMOF_PORTS >= 4)
//		{  0,   0,      0x8004,     MODULE_ID_DAP,  	SUBMOD_ID_PDEV_PORT,  0,      0,      PNIO_IM0_SUBMODULE},   // PDEV port4
//		#endif
//		{ 0, 1, 1, IO_MODULE_64_BYTE_IN_IRT, SUBMOD_ID_DEFAULT, 64, 0, PNIO_IM0_SUBMODULE }, // IO subslot
//		{ 0, 2, 1, IO_MODULE_64_BYTE_OUT_IRT, SUBMOD_ID_DEFAULT, 0, 64, PNIO_IM0_SUBMODULE }    // IO subslot
//};

// * --------------------------------------------
// * List of IM0 data
// * ---------------------------
// * Api, Slot, Subslot         in machine format
// * Vendor ID, OrderID,..,...  in BIG ENDIAN FORMAT
// * --------------------------------------------
// TODO: by dingding, change Im0List
//static PNIO_IM0_LIST_ENTRY Im0List[] = { // * --------------------------------------------
//										 // * IM0 Data for the DAP
//										 // * (optional)
//										 // * --------------------------------------------
//		{ 0,                          // PNIO_UINT32     Api;                 // api number
//				0,                          // PNIO_UINT32     Slot;                // slot number (1..0x7fff)
//				1,                          // PNIO_UINT32     Subslot;             // subslot number (1..0x7fff)
//				{ IOD_CFG_VENDOR_ID,                   // PNIO_UINT16     VendorId;            // VendorIDHigh, VendorIDLow
//						IOD_CFG_DEV_ANNOTATION_ORDER_ID,                   // PNIO_UINT8      OrderId [20];        // Order_ID, visible, must be 20 bytes here (fill with blanks)
//						IOD_CFG_IM0_SERIAL_NUMBER,     // PNIO_UINT8      SerNum  [16];        // IM_Serial_Number, visible string, must be 16 bytes here (fill with blanks)
//						DEVKIT_HW_REVISION,     // PNIO_UINT16     HwRevision;          // IM_Hardware_Revision
//						{ DEVKIT_VERSION_PREFIX,  // PNIO_UINT8      SwRevPrefix;         // software revision prefix
//								DEVKIT_VERSION_HH,      // PNIO_UINT8      SwRevFuncEnhanc;     // IM_SW_Revision_Funct_Enhancement
//								DEVKIT_VERSION_H,      // PNIO_UINT8      SwRevBugFix;         // IM_SW_Revision_Bug_Fix
//								DEVKIT_VERSION_L },      // PNIO_UINT8      SwRevInternChange;   // IM_SW_Revision_Internal_Change
//						DEVKIT_VERSION_LL,                   // PNIO_UINT16     Revcnt;              // IM_Revision_Counter, notifies a hw modification
//						0x00,                   // PNIO_UINT16     ProfId;              // IM_Profile_ID, see Profile Guideline I&M functions
//						0x05,                   // PNIO_UINT16     ProfSpecTyp;         // IM_Profile_Spec_Type (e.g. 3="io module")
//						0x01,                   // PNIO_UINT8      VersMaj;             // IM_Version major
//						0x01,                   // PNIO_UINT8      VersMin;             // IM_Version minor
//#if IOD_INCLUDE_IM5
//						PNIO_SUPPORT_IM12345    // PNIO_UINT16     ImSupported;         // IM_Supported, IM1...5 supported (bit list, bit 1...bit5, here: 0 ==> IM1..5  supported)
//#else
//			    PNIO_SUPPORT_IM1234     // PNIO_UINT16     ImSupported;         // IM_Supported, IM1...4 supported (bit list, bit 1...bit4, here: 0 ==> IM1..4  supported)
//#endif
//				} },
//		// * --------------------------------------------
//		// * IM0 Data for the slot 0 interface
//		// * (optional)
//		// * --------------------------------------------
//				{ 0,                          // PNIO_UINT32     Api;                 // api number
//						0,                          // PNIO_UINT32     Slot;                // slot number (1..0x7fff)
//						0x8000,                          // PNIO_UINT32     Subslot;             // subslot number (1..0x7fff)
//						{ IOD_CFG_VENDOR_ID,                   // PNIO_UINT16     VendorId;            // VendorIDHigh, VendorIDLow
//								IOD_CFG_DEV_ANNOTATION_ORDER_ID,                   // PNIO_UINT8      OrderId [20];        // Order_ID, visible, must be 20 bytes here (fill with blanks)
//								IOD_CFG_IM0_SERIAL_NUMBER,     // PNIO_UINT8      SerNum  [16];        // IM_Serial_Number, visible string, must be 16 bytes here (fill with blanks)
//								DEVKIT_HW_REVISION,     // PNIO_UINT16     HwRevision;          // IM_Hardware_Revision
//								{ DEVKIT_VERSION_PREFIX,  // PNIO_UINT8      SwRevPrefix;         // software revision prefix
//										DEVKIT_VERSION_HH,      // PNIO_UINT8      SwRevFuncEnhanc;     // IM_SW_Revision_Funct_Enhancement
//										DEVKIT_VERSION_H,      // PNIO_UINT8      SwRevBugFix;         // IM_SW_Revision_Bug_Fix
//										DEVKIT_VERSION_L },      // PNIO_UINT8      SwRevInternChange;   // IM_SW_Revision_Internal_Change
//								DEVKIT_VERSION_LL,                   // PNIO_UINT16     Revcnt;              // IM_Revision_Counter, notifies a hw modification
//								0x00,                   // PNIO_UINT16     ProfId;              // IM_Profile_ID, see Profile Guideline I&M functions
//								0x05,                   // PNIO_UINT16     ProfSpecTyp;         // IM_Profile_Spec_Type (e.g. 5="interface module")
//								0x01,                   // PNIO_UINT8      VersMaj;             // IM_Version major
//								0x01,                   // PNIO_UINT8      VersMin;             // IM_Version minor (must be 1, do not change !!)
//#if IOD_INCLUDE_IM5
//								PNIO_SUPPORT_IM12345    // PNIO_UINT16     ImSupported;         // IM_Supported, IM1...5 supported (bit list, bit 1...bit5, here: 0 ==> IM1..5  supported)
//#else
//			   PNIO_SUPPORT_IM1234     // PNIO_UINT16     ImSupported;         // IM_Supported, IM1...4 supported (bit list, bit 1...bit4, here: 0 ==> IM1..4  supported)
//#endif
//						} },
//				// * --------------------------------------------
//				// * IM0 Data for the slot 0 port 1
//				// * (optional)
//				// * --------------------------------------------
//				{ 0,                          // PNIO_UINT32     Api;                 // api number
//						0,                          // PNIO_UINT32     Slot;                // slot number (1..0x7fff)
//						0x8001,                          // PNIO_UINT32     Subslot;             // subslot number (1..0x7fff)
//						{ IOD_CFG_VENDOR_ID,                   // PNIO_UINT16     VendorId;            // VendorIDHigh, VendorIDLow
//								IOD_CFG_DEV_ANNOTATION_ORDER_ID,                   // PNIO_UINT8      OrderId [20];        // Order_ID, visible, must be 20 bytes here (fill with blanks)
//								IOD_CFG_IM0_SERIAL_NUMBER,     // PNIO_UINT8      SerNum  [16];        // IM_Serial_Number, visible string, must be 16 bytes here (fill with blanks)
//								DEVKIT_HW_REVISION,     // PNIO_UINT16     HwRevision;          // IM_Hardware_Revision
//								{ DEVKIT_VERSION_PREFIX,  // PNIO_UINT8      SwRevPrefix;         // software revision prefix
//										DEVKIT_VERSION_HH,      // PNIO_UINT8      SwRevFuncEnhanc;     // IM_SW_Revision_Funct_Enhancement
//										DEVKIT_VERSION_H,      // PNIO_UINT8      SwRevBugFix;         // IM_SW_Revision_Bug_Fix
//										DEVKIT_VERSION_L },      // PNIO_UINT8      SwRevInternChange;   // IM_SW_Revision_Internal_Change
//								DEVKIT_VERSION_LL,                   // PNIO_UINT16     Revcnt;              // IM_Revision_Counter, notifies a hw modification
//								0x00,                   // PNIO_UINT16     ProfId;              // IM_Profile_ID, see Profile Guideline I&M functions
//								0x05,                   // PNIO_UINT16     ProfSpecTyp;         // IM_Profile_Spec_Type (e.g. 5="interface module")
//								0x01,                   // PNIO_UINT8      VersMaj;             // IM_Version major
//								0x01,                   // PNIO_UINT8      VersMin;             // IM_Version minor (must be 1, do not change !!)
//#if IOD_INCLUDE_IM5
//								PNIO_SUPPORT_IM12345    // PNIO_UINT16     ImSupported;         // IM_Supported, IM1...5 supported (bit list, bit 1...bit5, here: 0 ==> IM1..5  supported)
//#else
//			   PNIO_SUPPORT_IM1234     // PNIO_UINT16     ImSupported;         // IM_Supported, IM1...4 supported (bit list, bit 1...bit4, here: 0 ==> IM1..4  supported)
//#endif
//						} },
//#if (IOD_CFG_PDEV_NUMOF_PORTS >= 2)
//				// * --------------------------------------------
//				// * IM0 Data for the slot 0 port 2
//				// * (optional)
//				// * --------------------------------------------
//				{ 0,                          // PNIO_UINT32     Api;                 // api number
//						0,                          // PNIO_UINT32     Slot;                // slot number (1..0x7fff)
//						0x8002,                          // PNIO_UINT32     Subslot;             // subslot number (1..0x7fff)
//						{ IOD_CFG_VENDOR_ID,                   // PNIO_UINT16     VendorId;            // VendorIDHigh, VendorIDLow
//								IOD_CFG_DEV_ANNOTATION_ORDER_ID,                   // PNIO_UINT8      OrderId [20];        // Order_ID, visible, must be 20 bytes here (fill with blanks)
//								IOD_CFG_IM0_SERIAL_NUMBER,     // PNIO_UINT8      SerNum  [16];        // IM_Serial_Number, visible string, must be 16 bytes here (fill with blanks)
//								DEVKIT_HW_REVISION,     // PNIO_UINT16     HwRevision;          // IM_Hardware_Revision
//								{ DEVKIT_VERSION_PREFIX,  // PNIO_UINT8      SwRevPrefix;         // software revision prefix
//										DEVKIT_VERSION_HH,      // PNIO_UINT8      SwRevFuncEnhanc;     // IM_SW_Revision_Funct_Enhancement
//										DEVKIT_VERSION_H,      // PNIO_UINT8      SwRevBugFix;         // IM_SW_Revision_Bug_Fix
//										DEVKIT_VERSION_L },      // PNIO_UINT8      SwRevInternChange;   // IM_SW_Revision_Internal_Change
//								DEVKIT_VERSION_LL,                   // PNIO_UINT16     Revcnt;              // IM_Revision_Counter, notifies a hw modification
//								0x00,                   // PNIO_UINT16     ProfId;              // IM_Profile_ID, see Profile Guideline I&M functions
//								0x05,                   // PNIO_UINT16     ProfSpecTyp;         // IM_Profile_Spec_Type (e.g. 5="interface module")
//								0x01,                   // PNIO_UINT8      VersMaj;             // IM_Version major
//								0x01,                   // PNIO_UINT8      VersMin;             // IM_Version minor (must be 1, do not change !!)
//#if IOD_INCLUDE_IM5
//								PNIO_SUPPORT_IM12345    // PNIO_UINT16     ImSupported;         // IM_Supported, IM1...5 supported (bit list, bit 1...bit5, here: 0 ==> IM1..5  supported)
//#else
//			   PNIO_SUPPORT_IM1234     // PNIO_UINT16     ImSupported;         // IM_Supported, IM1...4 supported (bit list, bit 1...bit4, here: 0 ==> IM1..4  supported)
//#endif
//						} },
//#endif
//				// * --------------------------------------------
//				// * IM0 Data for the slot 1 (64 byte input)
//				// * (mandatory, because proxy for device)
//				// * --------------------------------------------
//				{ 0,                          // PNIO_UINT32     Api;                 // api number
//						1,                          // PNIO_UINT32     Slot;                // slot number (1..0x7fff)
//						1,                          // PNIO_UINT32     Subslot;             // subslot number (1..0x7fff)
//						{ IOD_CFG_VENDOR_ID,                   // PNIO_UINT16     VendorId;            // VendorIDHigh, VendorIDLow
//								IOD_CFG_DEV_ANNOTATION_ORDER_ID,                   // PNIO_UINT8      OrderId [20];        // Order_ID, visible, must be 20 bytes here (fill with blanks)
//								IOD_CFG_IM0_SERIAL_NUMBER,     // PNIO_UINT8      SerNum  [16];        // IM_Serial_Number, visible string, must be 16 bytes here (fill with blanks)
//								DEVKIT_HW_REVISION,     // PNIO_UINT16     HwRevision;          // IM_Hardware_Revision
//								{ 'V',                    // PNIO_UINT8      SwRevPrefix;         // software revision prefix
//										4,                      // PNIO_UINT8      SwRevFuncEnhanc;     // IM_SW_Revision_Funct_Enhancement
//										0,                      // PNIO_UINT8      SwRevBugFix;         // IM_SW_Revision_Bug_Fix
//										0 },                     // PNIO_UINT8      SwRevInternChange;   // IM_SW_Revision_Internal_Change
//								0x01,                   // PNIO_UINT16     Revcnt;              // IM_Revision_Counter, notifies a hw modification
//								0x00,                   // PNIO_UINT16     ProfId;              // IM_Profile_ID, see Profile Guideline I&M functions (default: 0)
//								0x03,                   // PNIO_UINT16     ProfSpecTyp;         // IM_Profile_Spec_Type (e.g. 3="io module")
//								0x01,                   // PNIO_UINT8      VersMaj;             // IM_Version major (must be 1, do not change !!)
//								0x01,                   // PNIO_UINT8      VersMin;             // IM_Version minor (must be 1, do not change !!)
//								PNIO_SUPPORT_IM1234 } },                  // PNIO_UINT16     ImSupported;         // IM_Supported, IM1...4 supported (bit list, bit 1...bit4, here: 0 ==> IM1..15 not supported)
//				// * --------------------------------------------
//				// * IM0 Data for the slot 2 (64 byte input)
//				// * (mandatory, because proxy for device)
//				// * --------------------------------------------
//				{ 0,                          // PNIO_UINT32     Api;                 // api number
//						2,                          // PNIO_UINT32     Slot;                // slot number (1..0x7fff)
//						1,                          // PNIO_UINT32     Subslot;             // subslot number (1..0x7fff)
//						{ IOD_CFG_VENDOR_ID,                   // PNIO_UINT16     VendorId;            // VendorIDHigh, VendorIDLow
//								IOD_CFG_DEV_ANNOTATION_ORDER_ID,                   // PNIO_UINT8      OrderId [20];        // Order_ID, visible, must be 20 bytes here (fill with blanks)
//								IOD_CFG_IM0_SERIAL_NUMBER,     // PNIO_UINT8      SerNum  [16];        // IM_Serial_Number, visible string, must be 16 bytes here (fill with blanks)
//								DEVKIT_HW_REVISION,     // PNIO_UINT16     HwRevision;          // IM_Hardware_Revision
//								{ 'V',                    // PNIO_UINT8      SwRevPrefix;         // software revision prefix
//										4,                      // PNIO_UINT8      SwRevFuncEnhanc;     // IM_SW_Revision_Funct_Enhancement
//										0,                      // PNIO_UINT8      SwRevBugFix;         // IM_SW_Revision_Bug_Fix
//										0 },                     // PNIO_UINT8      SwRevInternChange;   // IM_SW_Revision_Internal_Change
//								0x01,                   // PNIO_UINT16     Revcnt;              // IM_Revision_Counter, notifies a hw modification
//								0x00,                   // PNIO_UINT16     ProfId;              // IM_Profile_ID, see Profile Guideline I&M functions (default: 0)
//								0x03,                   // PNIO_UINT16     ProfSpecTyp;         // IM_Profile_Spec_Type (e.g. 3="io module")
//								0x01,                   // PNIO_UINT8      VersMaj;             // IM_Version major (must be 1, do not change !!)
//								0x01,                   // PNIO_UINT8      VersMin;             // IM_Version minor (must be 1, do not change !!)
//								PNIO_SUPPORT_IM1234 } }                  // PNIO_UINT16     ImSupported;         // IM_Supported, IM1...4 supported (bit list, bit 1...bit4, here: 0 ==> IM1..15 not supported)
//
//		};

// *----------------------------------------------------------------*
// *
// *  PrintHelp (void)
// *
// *----------------------------------------------------------------*
static void PrintHelp(void) {
	// *---------------------------------------
	// *      help
	// *---------------------------------------
	PNIO_ConsolePrintf("\n\nCOMMAND LIST    ");
	PrintDevkitVersion();

	PNIO_ConsolePrintf("\nCONTROL:\n");
#if (PNIOD_PLATFORM == PNIOD_PLATFORM_RZT2_EDDP)
	PNIO_ConsolePrintf("'N'      set MAC address and store in flash \n");
	PNIO_ConsolePrintf("'o'      set IP address and store into flash \n");
	PNIO_ConsolePrintf("'0'      Perform System Reboot\n");
#else
        PNIO_ConsolePrintf ("'a'      send process alarm on slot1, subslot1\n");
		PNIO_ConsolePrintf ("'h'      send status alarm on slot1, subslot1\n");
        PNIO_ConsolePrintf ("'B / b'  send channel diag-alarm 'line break' appear/disapp. on slot1, subsl.1\n");
        PNIO_ConsolePrintf ("'C / c'  send generic diag-alarm  appear/disapp. on slot0, subsl.1\n");
        PNIO_ConsolePrintf ("'E / e'  send extended channel diag-alarm appear/disapp. on slot2, subsl.1\n");
        PNIO_ConsolePrintf ("'Q / q'  increment/decrement input data values\n");
        PNIO_ConsolePrintf ("'f'      download firmware via TCP and store into flash\n");
		#if(PNIO_TRACE != PNIO_TRACE_NONE)
        	PNIO_ConsolePrintf ("'F'      upload trace buffers via TCP\n");
		#endif
        #if EXAMPLE_IPAR_SERVER
            PNIO_ConsolePrintf ("'G'      send upload alarm (backup) to ipar-server \n");
            PNIO_ConsolePrintf ("'g'      send retrieval alarm (restore) to ipar-server  \n");
        #endif
        PNIO_ConsolePrintf ("'H'      print plugged submodules \n");
        PNIO_ConsolePrintf ("'j'      print cyclic io data \n");
		#if(PNIO_TRACE != PNIO_TRACE_NONE)
        	PNIO_ConsolePrintf ("'K'      set RS232 output trace level \n");
        	PNIO_ConsolePrintf ("'k'      set MEM output trace level \n");
		#endif
        #if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P)
            PNIO_ConsolePrintf ("'L'      set LED (GPIO 0..31, (-1):all) on \n");
            PNIO_ConsolePrintf ("'l'      set LED (GPIO 0..31, (-1):all) off \n");
        #endif
		PNIO_ConsolePrintf ("'n'      set device name and store in flash \n");
		PNIO_ConsolePrintf ("'N'      set MAC address and store in flash \n");
		PNIO_ConsolePrintf ("'o'      set IP address and store into flash \n");
        PNIO_ConsolePrintf ("'P'      enable user printf output on serial console\n");
        PNIO_ConsolePrintf ("'p'      disable user printf output\n");
        PNIO_ConsolePrintf ("'S'      plug a submodule\n");
        PNIO_ConsolePrintf ("'s'      pull a submodule\n");
		#if(PNIO_TRACE != PNIO_TRACE_NONE)
        	PNIO_ConsolePrintf ("'T'      set trace level for a package \n");
        	PNIO_ConsolePrintf ("'t'      set trace level for a single subsystem \n");
		#endif
        #if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P)
            PNIO_ConsolePrintf ("'W'      activate   application timer-isr  and PLL-Signal on GPIO1, 2 \n");
            PNIO_ConsolePrintf ("'w'      deactivate application timer-isr  and PLL-Signals \n");
            PNIO_ConsolePrintf ("' '      print ISR counter\n");
            PNIO_ConsolePrintf ("'y'      print last remote APDU state\n");
        #endif
        #ifdef INCLUDE_PERFORMANCE_MEASURE
            PNIO_ConsolePrintf ("'z'      measure system performance\n");
        #endif
        PNIO_ConsolePrintf ("'d'      perform stop of PN device \n");
        PNIO_ConsolePrintf ("'D'      perform start of PN device \n");
        PNIO_ConsolePrintf ("'0'      Perform System Reboot\n");
#endif
	PNIO_ConsolePrintf("\n");
}

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P)
        unsigned int timer_val;
        unsigned int NumOfTimerIsrCalls = 0;
        unsigned int NumOfTransEndIsrCalls = 0;

        void appl_timer_isr()
        {
            NumOfTimerIsrCalls++;
          timer_val = EVMA_GET_CYCLE_TIME();
        }

        void appl_trans_end_isr()
        {
          NumOfTransEndIsrCalls++;
        }
    #endif

// *----------------------------------------------------------------*
// *
// *  MainAppl(void)
// *
// *----------------------------------------------------------------*
// *
// *  main application function
// *   - starts the pnio stack
// *   - starts user interface task
// *   - handles user inputs and starts the selected test functions
// *
// *  Input:    argc            not used yet
// *            argv            not used yet
// *  Output:   ----
// *
// *----------------------------------------------------------------*
void MainAppl(void)

{
	PNIO_BOOL ConsoleProtection = PNIO_TRUE;
	PNIO_UINT32 Status = PNIO_OK;
	PNIO_UINT32 exitAppl = PNIO_FALSE;
	PNIO_BOOL isDeviceRunning = PNIO_FALSE;

	PNIO_UINT8 ProcessAlarmData[] = { "Process Alarm Slot 1, Subslot 1 " };		    // dummy data for process alarm
	PNIO_UINT8 StatusAlarmData[] = { 'a', 'b', 'c', 'd' };           // dummy data for status alarm
	PNIO_UINT8 DiagAlarmData[] = { "Diagnostic Alarm dummy data" };	// dummy data for diagnostic alarm

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P)
			PNIO_ISO_OBJ_HNDL  ApplGpio1Hndl     = 0;
			PNIO_ISO_OBJ_HNDL  ApplGpio2Hndl     = 0;
			PNIO_ISO_OBJ_HNDL  ApplTimHndl       = 0;

            //init USER LEDs (GPIO[0-7,16-23] = LED 20 to 35)
            REG32(U_GPIO__GPIO_PORT_MODE_0_L) &= ~( 0x0000FFFF );     //function == GPIO for  0- 7
            REG32(U_GPIO__GPIO_PORT_MODE_0_H) &= ~( 0x0000FFFF );     //function == GPIO for 16-23
            REG32(U_GPIO__GPIO_IOCTRL_0) &= ~( 0x00FF00FF );          //output
            REG32(U_GPIO__GPIO_OUT_CLEAR_0) |= 0x00FF00FF;          //clear outputs == LEDs off

            //init special purpose LEDs (GPIO[25-31] = LED 11 - 17)
            REG32(U_GPIO__GPIO_PORT_MODE_0_H) &= ~( 0xfffc0000 );     //function == GPIO for 25-31
            REG32(U_GPIO__GPIO_IOCTRL_0) &= ~( 0xFE000000 );          //output
            REG32(U_GPIO__GPIO_OUT_CLEAR_0) |= 0xFE000000;          //clear outputs == LEDs off

            if( MODE_SPI_0x03 == ( REG32( U_SCRB__BOOT_REG ) &0xf ) )
    		{

            	REG32(U_GPIO__GPIO_IOCTRL_0) &= ( ~( 1 << 31 ) );
            	REG32(U_GPIO__GPIO_IOCTRL_0) &= ( ~( 1 << 24 ) );
            	REG32(U_GPIO__GPIO_IOCTRL_0) &= ( ~( 1 << 26 ) );
            	REG32(U_GPIO__GPIO_IOCTRL_0) |= ( 1 << 27 );
            	REG32(U_GPIO__GPIO_PORT_MODE_0_H) |= ( 1 << 16 );
            	REG32(U_GPIO__GPIO_PORT_MODE_0_H) |= ( 1 << 20 );
            	REG32(U_GPIO__GPIO_PORT_MODE_0_H) |= ( 1 << 22 );
            	REG32(U_GPIO__GPIO_OUT_SET_0) = ( 1 << 31 );

            //in case of SPI boot, init GPIO 8, 10, 11 for LEDS
    		REG32(U_GPIO__GPIO_PORT_MODE_0_L) &= ~( 0x00F30000 );     //function == GPIO for 25-31
    		REG32(U_GPIO__GPIO_IOCTRL_0) &= ~( 0x00000D00 );          //output
    		REG32(U_GPIO__GPIO_OUT_CLEAR_0) |= 0x00000D00;          //clear outputs == LEDs off
    		}
        #endif

	// *-----------------------------------------------------
	// *     set startup parameter for the device
	// *     Note: in this simple example we suppose,
	// *     the DAP has no MRP capability.
	// *     If MPR shall be supported, the PN controller must
	// *     be capabable to send an MRP configuration record,
	// *     even if MRP is not activated.
	// *     More info to this topic see example App1_Standard,
	// *     file usriod_main.c
	// *-----------------------------------------------------
	PnUsrDev.VendorId = IOD_CFG_VENDOR_ID;                       // Vendor ID, requested from PROFIBUS/PROFINET organization (PI)
	PnUsrDev.DeviceId = IOD_CFG_DEVICE_ID;                       // Device ID, must be unique inside one Vendor ID
	PnUsrDev.MaxNumOfSubslots = IOD_CFG_NUMOF_SUBSLOTS;          // maximum number of subslots per slot, managable by PN Stack
	PnUsrDev.pDevType = IOD_CFG_DEVICE_TYPE;             // see also GSDML file, product family

	PNIO_UINT8 IoSubListNum = 0;

	// TODO: changed by dingding, MCP64 config
	PnUsrDev.DeviceId = 0x0308;
	IoSubListNum = 6;
	// SPILEN = 16;
	IoSubList = (PNIO_SUB_LIST_ENTRY*) malloc(sizeof(PNIO_SUB_LIST_ENTRY) * IoSubListNum);
	// 								  Api  Slot	Subslot,    ModId,     SubId, InLen, OutLen, I&M0 support
	PNIO_SUB_LIST_ENTRY tempEntry0 = { 0, 0, 1, 0x20000, 0x2, 0, 0, PNIO_IM0_NOTHING };
	PNIO_SUB_LIST_ENTRY tempEntry1 = { 0, 0, 0x8000, 0x20000, 0x3, 0, 0, PNIO_IM0_NOTHING };
	PNIO_SUB_LIST_ENTRY tempEntry2 = { 0, 0, 0x8001, 0x20000, 0x4, 0, 0, PNIO_IM0_NOTHING };
	PNIO_SUB_LIST_ENTRY tempEntry3 = { 0, 0, 0x8002, 0x20000, 0x4, 0, 0, PNIO_IM0_NOTHING };
	PNIO_SUB_LIST_ENTRY tempEntry4 = { 0, 1, 1, 0xf001, 0x1, 16, 12, PNIO_IM0_NOTHING };
	PNIO_SUB_LIST_ENTRY tempEntry5 = { 0, 2, 1, 0xf080, 0x80, 4, 0, PNIO_IM0_NOTHING };

	IoSubList[0] = tempEntry0;
	IoSubList[1] = tempEntry1;
	IoSubList[2] = tempEntry2;
	IoSubList[3] = tempEntry3;
	IoSubList[4] = tempEntry4;
	IoSubList[5] = tempEntry5;

	// **** startup the PN stack ****
	// *-----------------------------------------------------------
	// *     setup device stack and plug all io modules
	// *     (number of plugged modules <= IOD_CFG_NUMOF_SUBSLOTS)
	// *-----------------------------------------------------------
	Status = PnUsr_DeviceSetup(&PnUsrDev,                                  // device specific configuration
			&IoSubList[0],                              // list of plugged submodules (real configuration), including PDEV
			sizeof(IoSubList) / sizeof(IoSubList[0]), // number of entries in IoSubList
			&Im0List[0],                                // list of IM0 data sets
			sizeof(Im0List) / sizeof(Im0List[0]));     // number of entries in Im0List

	// *----------------------------------------------------------
	// * print startup result message
	// *----------------------------------------------------------
	if (Status == PNIO_OK) {
		PNIO_ConsolePrintf("SYSTEM STARTUP FINISHED, OK\n");
		isDeviceRunning = PNIO_TRUE;
	} else
		PNIO_ConsolePrintf("##ERROR AT SYSTEM STARTUP\n");

#if (INCLUDE_LOGADAPT == 1)
            OsWait_ms (1000);
            LogAdaptInit ();    // &&&2do  for LOGADAPT
        #endif

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P)
	        // *----------------------------------------------------------
            // * activate NewCycle on GPIO 0
            // *----------------------------------------------------------
            Bsp_SetGpioMode (0,             // GPIO 0
                             01,            // alternate function 01 = PNPLL_OUT
                             GPIO_DIR_OUT); // direction output
        #endif

	// *----------------------------------------------------------
	// *endless loop: wait on key pressed
	// *----------------------------------------------------------
	while (exitAppl == PNIO_FALSE) {
#if 0
		if(PNIO_OK == Status)
		{
                    /* Output */
                    if(OutDatIops[2][2] == PNIO_S_GOOD)
                    {
                        /* Get Output1Byte module data */
                        uint8_t u8Output1Byte = *((uint8_t*)&OutData[2][2][0]);
                        /* LED0 Update */
                        if(0x01U & u8Output1Byte)
                        {
                            Bsp_EbSetLed(BSP_LED_1, 1);
                        }
                        else
                        {
                            Bsp_EbSetLed(BSP_LED_1, 0);
                        }
                        /* LED1 Update */
                        if(0x02U & u8Output1Byte)
                        {
                            Bsp_EbSetLed(BSP_LED_2, 1);
                        }
                        else
                        {
                            Bsp_EbSetLed(BSP_LED_2, 0);
                        }
                    }
                    /* Input */
                    uint8_t u8SW1=0, u8SW2=0;
                    /* SW1 Read */
#ifdef _RENESAS_RZT_
                    R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_10_PIN_5, (bsp_io_level_t*) &u8SW1);
#endif
#ifdef _RENESAS_RZN_
                    R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_16_PIN_3, (bsp_io_level_t*) &u8SW1);
#endif
                    /* SW2 Read */
#ifdef _RENESAS_RZT_
                    R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_16_PIN_3, (bsp_io_level_t*) &u8SW2);
#endif
#ifdef _RENESAS_RZN_
                    R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_05_PIN_4, (bsp_io_level_t*) &u8SW2);
#endif
                    /* Set Input1Byte module data */ 
                    *((uint8_t*)&InData[1][2][0]) = (uint8_t)(0xA0 | u8SW2 << 1 | u8SW1 << 0);
                }
                OsWait_ms(100);

#else   // ERTEC SDK
		// *-----------------------------------
		// * wait on key pressed by the user
		// *-----------------------------------
		PNIO_INT32 PressedKey = OsGetChar();

		// prevent from execution of command when console output is disabled
		if (ConsoleProtection && (!PnioLogDest) && (PressedKey != '?') && (PressedKey != 'P') && (PressedKey != '&')) {
			continue;
		}

		switch (PressedKey) {

		PNIO_DEV_ADDR Addr;		                        // location (module/submodule)

		// ***-------------------------------------------------------------------***
		// ***       A L A R M S
		// ***-------------------------------------------------------------------***
	case 'a':
		// *---------------------------------------
		// *      send process alarm
		// *      Note: only one process alarm at a time is possible
		// *      Wait before sending a new alarm,
		// *      until the previous one has been confirmed.
		// *---------------------------------------
		Addr.Geo.Slot = EXAMPL_1_SLOTNUM;	            // slot number
		Addr.Geo.Subslot = EXAMPL_1_SUBSLOTNUM;			// subslot number
		Status = PNIO_process_alarm_send(PNIO_SINGLE_DEVICE_HNDL,
		PNIO_DEFAULT_API, &Addr,						    // location (slot, subslot)
				&ProcessAlarmData[0],		    // alarm data
				sizeof(ProcessAlarmData),     // alarm data length
				EXAMPL_1_USER_STRUCT_IDENT,	// 0...0x7fff: user struct. is manufac. specific
				EXAMPL_1_DIAG_TAG);	        // user defined handle

		if (Status != PNIO_OK)
			PNIO_ConsolePrintf("Error %x occured\n", PNIO_get_last_error());
		break;

	case 'h':
		// *---------------------------------------
		// *      send status alarm
		// *      Note: only one status alarm at a time is possible
		// *      Wait before sending a new alarm,
		// *      until the previous one has been confirmed.
		// *---------------------------------------
		Addr.Geo.Slot = EXAMPL_1_SLOTNUM;	            // slot number
		Addr.Geo.Subslot = EXAMPL_1_SUBSLOTNUM;			// subslot number
		Status = PNIO_status_alarm_send(PNIO_SINGLE_DEVICE_HNDL,
		PNIO_DEFAULT_API, &Addr,						    // location (slot, subslot)
				&StatusAlarmData[0],		    // alarm data
				sizeof(StatusAlarmData),      // alarm data length
				EXAMPL_1_USER_STRUCT_IDENT,	// 0...0x7fff: user struct. is manufac. specific
				EXAMPL_1_DIAG_TAG);	        // user defined handle

		if (Status != PNIO_OK)
			PNIO_ConsolePrintf("Error %x occured\n", PNIO_get_last_error());

		break;

	case 'B':
		// *----------------------------------------------
		// *      provide standard channel diagnostic
		// *      and send alarm appears, "line break"
		// *      Note: only one diagnosis alarm at a time is possible
		// *      Wait before sending a new alarm,
		// *      until the previous one has been confirmed.
		// *      (see also PNO document "diagnosis guideline" (on devkit CD)
		// *-----------------------------------------------
		Status = UsrChanDiag(DIAG_CHANPROP_SPEC_ERR_APP,          // new alarm appears
				EXAMPL_2_SLOTNUM,                    // slot number
				EXAMPL_2_SUBSLOTNUM,                 // subslot number
				EXAMPL_2_CHANNELNUM,                 // channel number
				EXAMPL_2_CHAN_ERR_TYPE,              // error number (see IEC61158)
				EXAMPL_2_IO_DIRECTION,               // channel is a input module
				EXAMPL_2_IO_DATA_TYPE,               // channel data type is BYTE
				EXAMPL_2_DIAG_TAG	     	          // user defined tag
				);
		if (Status != PNIO_OK)
			PNIO_ConsolePrintf("Error %x occured\n", PNIO_get_last_error());

		break;

	case 'b':
		// *-----------------------------------------------
		// *      provide standard channel diagnostic
		// *      and send alarm disappears, "line break"
		// *      (no more errors pending)
		// *      Note: only one diagnosis alarm at a time is possible
		// *      Wait before sending a new alarm,
		// *      until the previous one has been confirmed.
		// *      (see also PNO document "diagnosis guideline" (on devkit CD)
		// *-----------------------------------------------
		Status = UsrChanDiag(DIAG_CHANPROP_SPEC_ERR_DISAPP,   // alarm disappears
				EXAMPL_2_SLOTNUM,                // slot number
				EXAMPL_2_SUBSLOTNUM,             // subslot number
				EXAMPL_2_CHANNELNUM,             // channel number
				EXAMPL_2_CHAN_ERR_TYPE,          // error number (see IEC61158)
				EXAMPL_2_IO_DIRECTION,           // channel is a input module
				EXAMPL_2_IO_DATA_TYPE,               // channel data type is BYTE
				EXAMPL_2_DIAG_TAG	     	          // user defined tag
				);
		if (Status != PNIO_OK)
			PNIO_ConsolePrintf("Error %x occured\n", PNIO_get_last_error());
		break;

	case 'C':
		// *-----------------------------------------------
		// *      send generic diagnostic
		// *      alarm appears
		// *      Note: only one diagnosis alarm at a time is possible
		// *      Wait before sending a new alarm,
		// *      until the previous one has been confirmed.
		// *-----------------------------------------------
		Status = UsrGenericDiag(DIAG_CHANPROP_SPEC_ERR_APP,    // alarm disappears
				EXAMPL_4_SLOTNUM,              // slot number
				EXAMPL_4_SUBSLOTNUM,           // subslot number
				EXAMPL_4_CHANNELNUM,           // channel number
				&DiagAlarmData[0],		    	// alarm data
				sizeof(DiagAlarmData),	    // alarm data length
				EXAMPL_4_DIAG_TAG,             // user defined tag
				EXAMPL_4_USER_STRUCT_IDENT,    // UserStructureIdentifier, manufacturer specific, 0...0x7fff, see IEC 61158
				EXAMPL_4_IO_DIRECTION,	        // input/output/input-output
				EXAMPL_4_IO_DATA_TYPE);	    // channel data type

		if (Status != PNIO_OK)
			PNIO_printf("Error %x occured\n", PNIO_get_last_error());

		break;

	case 'c':
		// *-----------------------------------------------
		// *      send generic diagnostic
		// *      alarm disappears (no more errors
		// *      pending)
		// *      Note: only one diagnosis alarm at a time is possible
		// *      Wait before sending a new alarm,
		// *      until the previous one has been confirmed.
		// *-----------------------------------------------
		Status = UsrGenericDiag(DIAG_CHANPROP_SPEC_ERR_DISAPP, // alarm disappears
				EXAMPL_4_SLOTNUM,              // slot number
				EXAMPL_4_SUBSLOTNUM,           // subslot number
				EXAMPL_4_CHANNELNUM,           // channel number
				NULL,          		    	// no alarm data, don't care
				0,                     	    // alarm data size = 0, don't care
				EXAMPL_4_DIAG_TAG,             // user defined tag
				EXAMPL_4_USER_STRUCT_IDENT,    // UserStructureIdentifier, manufacturer specific, 0...0x7fff, see IEC 61158
				EXAMPL_4_IO_DIRECTION,         // input/output/input-output
				EXAMPL_4_IO_DATA_TYPE);        // channel data type

		if (Status != PNIO_OK)
			PNIO_printf("Error %x occured\n", PNIO_get_last_error());

		break;

	case 'E':
		// *-----------------------------------------------
		// *      provide extended channel diagnostic
		// *      and send alarm appears, "line break"
		// *      Note: only one diagnosis alarm at a time is possible
		// *      Wait before sending a new alarm,
		// *      until the previous one has been confirmed.
		// *      (see also PNO document "diagnosis guideline" (on devkit CD)
		// *-----------------------------------------------
		Status = UsrExtDiag(DIAG_CHANPROP_SPEC_ERR_APP,          // new alarm appears
				EXAMPL_3_SLOTNUM,                    // slot number
				EXAMPL_3_SUBSLOTNUM,                 // subslot number
				EXAMPL_3_CHANNELNUM,                 // channel number
				EXAMPL_3_CHAN_ERR_TYPE,              // error number (see IEC61158)
				EXAMPL_3_EXT_CHAN_ERR_TYPE,          // ExtChannelErrorType (see IEC61158)
				EXAMPL_3_EXT_CHAN_ADD_VALUE,         // ExtChannelAddValue (see IEC61158)
				EXAMPL_3_IO_DIRECTION,               // channel is a input module
				EXAMPL_3_IO_DATA_TYPE,               // channel data type is BYTE
				EXAMPL_3_DIAG_TAG     	        	  // user defined tag
				);
		if (Status != PNIO_OK)
			PNIO_ConsolePrintf("Error %x occured\n", PNIO_get_last_error());

		break;

	case 'e':
		// *---------------------------------------
		// *      provide standard channel diagnostic
		// *      and send alarm disappears, "line break"
		// *      (no more errors pending)
		// *      Note: only one diagnosis alarm at a time is possible
		// *      Wait before sending a new alarm,
		// *      until the previous one has been confirmed.
		// *      (see also PNO document "diagnosis guideline" (on devkit CD)
		// *---------------------------------------
		Status = UsrExtDiag(DIAG_CHANPROP_SPEC_ERR_DISAPP,   // alarm disappears
				EXAMPL_3_SLOTNUM,                // slot number
				EXAMPL_3_SUBSLOTNUM,             // subslot number
				EXAMPL_3_CHANNELNUM,             // channel number
				EXAMPL_3_CHAN_ERR_TYPE,          // error number (see IEC61158)
				EXAMPL_3_EXT_CHAN_ERR_TYPE,      // ExtChannelErrorType (see IEC61158)
				EXAMPL_3_EXT_CHAN_ADD_VALUE,     // ExtChannelAddValue (see IEC61158)
				EXAMPL_3_IO_DIRECTION,           // channel is a input module
				EXAMPL_3_IO_DATA_TYPE,               // channel data type is BYTE
				EXAMPL_3_DIAG_TAG     	        	  // user defined tag
				);
		if (Status != PNIO_OK)
			PNIO_ConsolePrintf("Error %x occured\n", PNIO_get_last_error());
		break;

		// ***-------------------------------------------------------------------***
		// ***       P L U G  /  P U L L
		// ***-------------------------------------------------------------------***
	case 'S':
		// *---------------------------------------
		// * plug a single submodule  (here: Sub1 in Mod1)
		// *---------------------------------------
		PNIO_ConsolePrintf("Plug Submodule 1 of Module 1...\n");
		Addr.Geo.Slot = 1;			// slot 1
		Addr.Geo.Subslot = 1;			// subslot 1
		Status = PNIO_sub_plug(PNIO_SINGLE_DEVICE_HNDL,
		PNIO_DEFAULT_API, &Addr,			                    // location (slot, subslot)
				IO_MODULE_64_BYTE_IN_IRT,
				SUBMOD_ID_DEFAULT,	                // submodule identifier
				64,                                 // submodule input data length
				0,                                  // submodule output data length
				PNIO_IM0_SUBMODULE,                 // Submodule supports IM0
				(IM0_DATA*) NULL,
				PNIO_S_GOOD);                        // initial iops value, only for submodules without io data

		if (Status != PNIO_OK)
			PNIO_ConsolePrintf("Error %x occured\n", PNIO_get_last_error());

		PNIO_ConsolePrintf("done\n");
		break;

	case 's':
		// *---------------------------------------
		// * pull a submodule  (here: Sub1 in Mod1)
		// *---------------------------------------
		PNIO_ConsolePrintf("Pull Submodule 1 of Module 1...\n");
		Addr.Geo.Slot = 1;			// slot 1
		Addr.Geo.Subslot = 1;			// subslot 1
		Status = PNIO_sub_pull(PNIO_SINGLE_DEVICE_HNDL,
		PNIO_DEFAULT_API, &Addr);			// location (slot, subslot)
		if (Status != PNIO_OK)
			PNIO_ConsolePrintf("Error %x occured\n", PNIO_get_last_error());
		PNIO_ConsolePrintf("done\n");
		break;

		// ***-------------------------------------------------------------------***
		// ***       T E S T   U T I L I T I E S
		// ***-------------------------------------------------------------------***
	case 'f':
		Status = TcpReceiveAndFlashFirmware();
		break;

#if EXAMPLE_IPAR_SERVER
	case 'G':
		PNIO_ConsolePrintf("Send upload alarm for module 1...\n");
		Addr.Geo.Slot = EXAMPL_IPAR_SLOTNUM;
		Addr.Geo.Subslot = EXAMPL_IPAR_SUBSLOTNUM;
		Status = PNIO_upload_retrieval_alarm_send(PNIO_SINGLE_DEVICE_HNDL,
		PNIO_DEFAULT_API, &Addr,                        // geographical or logical address
				&UploadAlarmData[0],          // AlarmItem.Data
				sizeof(UploadAlarmData),     // length of AlarmItem.Data
				0x1357);
		break;

	case 'g':
		PNIO_ConsolePrintf("Send retrieval alarm for module 1...\n");
		Addr.Geo.Slot = EXAMPL_IPAR_SLOTNUM;
		Addr.Geo.Subslot = EXAMPL_IPAR_SUBSLOTNUM;
		Status = PNIO_upload_retrieval_alarm_send(PNIO_SINGLE_DEVICE_HNDL,
		PNIO_DEFAULT_API, &Addr,                        // geographical or logical address
				&RetrievalAlarmData[0],       // AlarmItem.Data
				sizeof(RetrievalAlarmData),  // length of AlarmItem.Data
				0x1358);
		break;
#endif

	case 'H':
		PrintSubmodules();
		break;

	case 'j':
		// *---------------------------------------
		// *      print io data
		// *---------------------------------------
		PrintAllUsedIoData();
		break;

	case 'Q':
		// *---------------------------------------
		// *      increment io data values
		// *---------------------------------------
		PNIO_ConsolePrintf("increment input data values\n");
		ChangeAllInputData(1);
		break;

	case 'q':
		// *---------------------------------------
		// *      decrement io data values
		// *---------------------------------------
		PNIO_ConsolePrintf("decrement input data values\n");
		ChangeAllInputData(-1);
		break;

	case 'N':
		// *---------------------------------------
		// *      input MAC address
		// *---------------------------------------
	{
		PNIO_ConsolePrintf("input mac address\n");
		InputAndStoreMacAddress();
	}

		break;

	case 'n':
		// *---------------------------------------
		// *      input device name
		// *---------------------------------------
	{
		PNIO_ConsolePrintf("input device name\n");
		InputAndStoreDeviceName();
	}

		break;

	case 'o':
		// *---------------------------------------
		// *      input ip address
		// *---------------------------------------
	{
		PNIO_ConsolePrintf("input ip address\n");
		InputAndStoreIpAddress();
	}

		break;
#if(PNIO_TRACE != PNIO_TRACE_NONE)
                case 'F':
                    	Status = TcpTraceUpload ();
                    	break;

                case 'T':
                        // *---------------------------------------
                        // *  set TRACE LEVEL for all
                        // *  all subsystems of a package
                        // *---------------------------------------
                        {
                            PNIO_UINT32 LowerSubsys;
                            PNIO_UINT32 AllPackages = 0;
                            PNIO_UINT32 NewTraceLevel;

                            PNIO_ConsolePrintf ("set Package trace level\n");

                            AllPackages = OsKeyScan32 ("Press 1 for all Packages or 0 for specific one = ", 10);
                            if (AllPackages == 1)
                            {
                            	NewTraceLevel  = OsKeyScan32 ("TraceLevel  = ", 10);
                            	TrcDkSetAllLevel (NewTraceLevel);
                            }
                            else
                            {
                            	LowerSubsys    = OsKeyScan32 ("LowerSubsys = ", 10);

                            	NewTraceLevel  = OsKeyScan32 ("TraceLevel  = ", 10);

                            	if ((NewTraceLevel == 0) || (LowerSubsys == 0 ))
                            	{
                            		PNIO_ConsolePrintf ("Invalid Input\n");
                            	}
                            	else
                            	{
                            		TrcDkSetPackageLevel (LowerSubsys, NewTraceLevel);
                            	}
                            }
                        }

                        break;

                case 't':
                        // *---------------------------------------
                        // *  set TRACE Level for a single subsystem
                        // *---------------------------------------
                        {
                            PNIO_UINT32 Subsys;
                            PNIO_UINT32 NewTraceLevel;
                            PNIO_ConsolePrintf ("set single subsystem trace level\n");
                            Subsys         = OsKeyScan32 ("Subsys     = ", 10);
                            NewTraceLevel  = OsKeyScan32 ("TraceLevel = ", 10);
                            TrcDkSetLevel (Subsys, NewTraceLevel);
                        }

                        break;
                case 'K':
                        // *---------------------------------------
                        // *  RS232 TRACE LEVEL
                        // *---------------------------------------
                        {
                            PNIO_UINT32 NewTraceLevel;
                            NewTraceLevel  = OsKeyScan32 ("RS232 TRACELEVEL = ", 10);
                            TrcDkSetAllMinLevel  (DEV_STD, NewTraceLevel);
                         }

                        break;

                case 'k':
                        // *---------------------------------------
                        // *  MEM TRACE LEVEL
                        // *---------------------------------------
                        {
                            PNIO_UINT32 NewTraceLevel;
                            NewTraceLevel  = OsKeyScan32 ("MEM TRACELEVEL = ", 10);
                            TrcDkSetAllMinLevel  (DEV_MEM, NewTraceLevel);
                         }

                        break;

                case '$':
                {
                        // *---------------------------------------
                        // *
                        // *---------------------------------------
                		TrcStoreBuf();
                        break;
                }
#endif
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P)
                case 'L':
                        // *---------------------------------------
                        // *  set LED on
                        // *---------------------------------------
                        SetGpioLed(1);
                		break;

                case 'l':
                        // *---------------------------------------
                        // *  set LED off
                        // *---------------------------------------
                	    SetGpioLed(0);
                		break;
#endif

	case 'P':
		// *---------------------------------------
		// *  Set user printf on RS232 console
		// *---------------------------------------
		PNIO_ConsolePrintf("enable serial console output\n");
		PnioLogDest = 1;       // 0:none, 1:Console, 3: UDP console
		PNIO_ConsolePrintf("Set serial  console output\n");
		break;

	case 'p':
		// *---------------------------------------
		// *  disable console output
		// *---------------------------------------
		PNIO_ConsolePrintf("disable console output\n");
		PnioLogDest = 0;       // 0:none, 1:Console, 3: UDP console
		break;
	case 'D':
		if (isDeviceRunning == PNIO_TRUE) {
			PNIO_ConsolePrintf("PN device is already started..\n");
		} else {
			PNIO_ConsolePrintf("PN device will be started..\n");
			PNIO_device_start(PNIO_SINGLE_DEVICE_HNDL);
			isDeviceRunning = PNIO_TRUE;
		}
		break;
	case 'd':
		if (isDeviceRunning == PNIO_FALSE) {
			PNIO_ConsolePrintf("PN device is already stopped..\n");
		} else {
			PNIO_ConsolePrintf("PN device will be stoped..\n");
			PNIO_device_stop(PNIO_SINGLE_DEVICE_HNDL);
			isDeviceRunning = PNIO_FALSE;
		}
		break;
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P)
                    case 'W':
                          // *---------------------------------------
                          // *  alloc timer event isr
                          // *---------------------------------------
                          {
                            PNIO_ConsolePrintf ("alloc timer event isr ....");
                            Status = PNIO_IsoActivateIsrObj(appl_timer_isr, 50000, &ApplTimHndl);
                            if (Status != PNIO_OK)
                                PNIO_ConsolePrintf ("ERROR occured\n");
                            else

                            {
                                PNIO_ConsolePrintf ("OK\n");
                                PNIO_UINT32 Delay;
                                PNIO_ConsolePrintf ("alloc timer event and events for  GPIO 1, 2 ....");

                                Delay = OsKeyScan32 ("Delay1 [nsec] = ", 10);
                                Status = PNIO_IsoActivateGpioObj(1, Delay,     ISO_GPIO_LOW_ACTIVE, &ApplGpio1Hndl);
                                if (Status != PNIO_OK)
                                    PNIO_ConsolePrintf ("ERROR occured\n");
                                else
                                    PNIO_ConsolePrintf ("OK\n");

                                Delay = OsKeyScan32 ("Delay2 [nsec] = ", 10);
                                Status = PNIO_IsoActivateGpioObj(2, Delay,     ISO_GPIO_LOW_ACTIVE, &ApplGpio2Hndl);
                                if (Status != PNIO_OK)
                                    PNIO_ConsolePrintf ("ERROR occured\n");
                                else
                                    PNIO_ConsolePrintf ("OK\n");
                            }
                          }
                          break;
                    case 'w':
                          // *---------------------------------------
                          // *  free timer event isr
                          // *---------------------------------------
                          {
                               PNIO_ConsolePrintf ("free timer event isr ....");
                               Status = PNIO_IsoFreeObj (ApplTimHndl);
                               if (Status == PNIO_OK)
                                   PNIO_ConsolePrintf ("OK\n");
                               else
                                   PNIO_ConsolePrintf ("ERROR occured\n");

                          }
                          {
                               PNIO_ConsolePrintf ("timer GPIO1 eventGPIO....");
                               Status = PNIO_IsoFreeObj (ApplGpio1Hndl);
                               if (Status == PNIO_OK)
                                   PNIO_ConsolePrintf ("OK\n");
                               else
                                   PNIO_ConsolePrintf ("ERROR occured\n");

                               PNIO_ConsolePrintf ("timer GPIO2 eventGPIO....");
                               Status = PNIO_IsoFreeObj (ApplGpio2Hndl);
                               if (Status == PNIO_OK)
                                   PNIO_ConsolePrintf ("OK\n");
                               else
                                   PNIO_ConsolePrintf ("ERROR occured\n");

                               // ***** set back to GPIO *****
                               Bsp_SetGpioMode (1,             // GPIO 1
                                                00,            // no alternate function 00 = GPIO
                                                GPIO_DIR_OUT); // direction output

                               Bsp_SetGpioMode (2,             // GPIO 2
                                                00,            // no alternate function 00 = GPIO
                                                GPIO_DIR_OUT); // direction output

                          }
                          break;

                    case ' ':
                          // *---------------------------------------
                          // *  print isr counter
                          // *---------------------------------------
                          PNIO_ConsolePrintf ("ApplTimCounter = %d  AppTransEndCounter = %d\n",
                                       NumOfTimerIsrCalls,
                                       NumOfTransEndIsrCalls);
                          break;

                    case 'y':
                            // *---------------------------------------
                            // *  print last APDU-Status
                            // *---------------------------------------
                            PNIO_ConsolePrintf ("last remote APDU State (AR=1) = 0x%x\n", LastRemoteApduStatus);
                            break;
                #endif

	case 'X':
		// *---------------------------------------
		// *  activate Cycle IO data exchange
		// *---------------------------------------
		PNIO_ConsolePrintf("activate Cycle IO data exchange\n");
		PnUsr_ActivateIoDatXch();
		break;

	case 'x':
		// *---------------------------------------
		// *  deactivate Cycle IO data exchange
		// *---------------------------------------
		PNIO_ConsolePrintf("deactivate Cycle IO data exchange\n");
		PnUsr_DeactivateIoDatXch();
		break;

#ifdef INCLUDE_PERFORMANCE_MEASURE
                  case 'z':
                        // *---------------------------------------
                        // *      do performance measure
                        // *      (test option only)
                        // *---------------------------------------
                        ExecutePerformanceMeasure ();
                        break;
                #endif

	case '?':
		// *---------------------------------------
		// *   help: print all keyboard commands
		// *---------------------------------------
		PrintHelp();
		break;

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS)
                    case '!':
                    {
                        extern void OsPrintMemInfo();
                        extern void OsPrintThreads();
                        OsPrintMemInfo();
                        OsPrintThreads();
                        break;
                    }
                #endif

	case '0':
		// *---------------------------------------
		// *
		// *---------------------------------------
		PNIO_ConsolePrintf("OsReboot in 2 sec....\n");
		OsReboot();

		break;

	case '&':
		// *---------------------------------------
		// *   toggle console protection
		// *---------------------------------------
		ConsoleProtection ^= PNIO_TRUE;
		PNIO_ConsolePrintf("Console Protection %s\n", ConsoleProtection ? "ON" : "OFF");
		break;

	default:
		PNIO_ConsolePrintf("key 0x%x pressed, press key '?' to get more info\n", PressedKey);
		break;
		}
#endif
	}
}

// *----------------------------------------------------------------*
// *
// *  PnUsr_cbf_IoDatXch (void)
// *
// *----------------------------------------------------------------*
// *  cyclic exchange of IO data
// *
// *  This function performs the cyclic IO data exchange
// *  Every IO data exchange (one data read and one data write)
// *  it is called from the PNIO stack.
// *
// *----------------------------------------------------------------*
// *  Input:    ----
// *  Output:   ----
// *
// *----------------------------------------------------------------*
PNUSR_CODE_FAST OS_CODE_FAST PNIO_BOOL PnUsr_cbf_IoDatXch(void) {
	volatile PNIO_UINT32 Status;

	// *---------------------------------------------------
	// *  send input data to PNIO controller
	// *---------------------------------------------------
	Status = PNIO_initiate_data_write(PNIO_SINGLE_DEVICE_HNDL);		// input data
	switch (Status) {
	case PNIO_OK:
		break;
	case PNIO_NOT_OK:
		break;
	default:
		PNIO_ConsolePrintf("Error 0x%x at PNIO_initiate_data_write()\n", Status);
		break;
	}

	// *---------------------------------------------------
	// *  read output data from PNIO controller
	// *---------------------------------------------------
	if (Status != PNIO_NOT_OK) {
		Status = PNIO_initiate_data_read(PNIO_SINGLE_DEVICE_HNDL); // output data
		LastRemoteApduStatus = PNIO_get_last_apdu_status(PNIO_SINGLE_DEVICE_HNDL, 1);  // read last APDU state from first AR
	}

	switch (Status) {
	case PNIO_OK:
		break;
	case PNIO_NOT_OK:
		break;
	default:
		PNIO_ConsolePrintf("Error 0x%x at PNIO_initiate_data_read()\n", Status);
		break;
	}

	// *---------------------------------------------------
	// *  implement user software here
	// *  (e.g. write physical outputs)
	// *---------------------------------------------------

	return (Status);
}

#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
