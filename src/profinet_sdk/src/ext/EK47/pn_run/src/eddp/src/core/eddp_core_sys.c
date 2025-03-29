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
/*  F i l e               &F: eddp_core_sys.c                           :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP system input-functions                      */
/*                                                                           */
/*                          - edd_system()                                   */
/*                          - edd_init()                                     */
/*                          - edd_undo_init()                                */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  15.08.08    js    initial version.                                       */
/*  02.11.09    sf    renamed from eddp_sys.c --> eddp_core_sys.c but the    */ 
/*                    h-file eddp_sys.h was not renamed (common API) !!!     */
/*  26.03.10    MA    added eddp_K32GetData                                  */
/*  18.10.10    JS    added K32TraceLevel                                    */
/*  17.01.11    JS    added MaxInterfaceCntOfAllEDD and MaxPortCntOfAllEDD   */
/*  21.01.11    JS    used 0 for K32TraceLevel instead of LSA-define         */
/*  08.02.12    VS    added ApplTimer parameters for TransferEnd mode in     */
/*                    function _eddp_InitIPBWithCRTDefaults()                */
/*  09.03.12    VS    added Hardware configuration parameters                */
/*                    "HWCfg.IRQx_Control" in function                       */
/*                    eddp_InitDPBWithDefaults()                             */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID  1                 
#define EDDP_MODULE_ID     LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_CORE_SYS */ 

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"            /* edd headerfiles */
#include "eddp_int.h"            /* internal header */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID) 

/*===========================================================================*/
/*                                global Variables                           */
/*===========================================================================*/

EDDP_LOCAL_INFO_PTR_TYPE        pEDDPInfo;          /* main EDDP management */

#ifdef EDDP_CFG_LOCAL_MEM_STATIC
EDDP_LOCAL_MEM_ATTR  EDDP_INFO_TYPE  g_EDDPInfo;
#endif

/*===========================================================================*/
/*                             external functions                            */
/*===========================================================================*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_init                                   +*/
/*+  Input/Output          :    ---                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Result:      EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_RESOURCE                                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Initializes the EDD.                                      +*/
/*+               The number of handles the edd can manage is defined by    +*/
/*+               EDDP_CFG_HANDLE_CNT. The memory for handle-management is  +*/
/*+               static or will be allocated depending on                  +*/
/*+               EDD_LOCAL_MEM_STATIC. The memory for devices will be      +*/
/*+               allocated on device open.                                 +*/
/*+                                                                         +*/
/*+               Note: Must only be called if not already initialized, not +*/
/*+                     checked!                                            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_SYSTEM_IN_FCT_ATTR eddp_init(LSA_VOID)
{
    EDD_RSP         Response;
    LSA_UINT16      i;
    LSA_UINT32      Index;

    EDDP_SYSTEM_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT,"[H:--] --> eddp_init()");

    Response = EDD_STS_OK;

    /* check for correct endianess define  */

    i = 0x1234;
    #ifndef EDDP_CFG_BIG_ENDIAN 
    if ( *(LSA_UINT8*)&i != 0x34 )
    {
    #else
    if ( *(LSA_UINT8*)&i != 0x12 )
    {
    #endif
	    EDDP_FATAL(EDDP_FATAL_ERR_ENDIANES);
    }

    /* alloc main management or use static */

    #ifdef EDDP_CFG_LOCAL_MEM_STATIC
    pEDDPInfo = &g_EDDPInfo;
    #else
    pEDDPInfo = EDDP_GlbAllocLocalMem(sizeof(EDDP_INFO_TYPE));
    #endif

    if (LSA_HOST_PTR_ARE_EQUAL(pEDDPInfo, LSA_NULL))
    {
  	    Response = EDD_STS_ERR_RESOURCE;
    }
    else
    {
        EDDP_MEMSET_COMMON((EDD_COMMON_MEM_PTR_TYPE)pEDDPInfo, 0, sizeof(EDDP_INFO_TYPE));
       
        for ( Index=0; Index < EDDP_CFG_MAX_CHANNELS; Index++ )
        {
	        pEDDPInfo->HDB[Index].InUse = LSA_FALSE;
	    }

        EDDP_CoreTimerMgmSetup();
    }            

    EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT,"[H:--] <-- eddp_init(): Response=0x%X",
        Response);

    return (Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_undo_init                              +*/
/*+  Input/Output          :    ---                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Result:      EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_SEQUENCE                                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Uninizializes EDDP                                        +*/
/*+               This is only possible if no device is open                +*/
/*+               EDD_STS_ERR_SEQUENCE if device is open.                   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_SYSTEM_IN_FCT_ATTR eddp_undo_init(LSA_VOID)
{
    EDD_RSP     Response;

    EDDP_SYSTEM_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] --> eddp_undo_init()");

    Response = EDD_STS_OK;

    if (pEDDPInfo->DDBUsedCnt)
    {
	    EDDP_SYSTEM_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] eddp_undo_init(): FATAL -> device(s) still open!");
        EDDP_FATAL(EDDP_FATAL_ERR_UNDO_INIT_FAILED);
    }

    EDDP_CoreTimerMgmFree();

    #ifndef EDDP_CFG_LOCAL_MEM_STATIC
    EDDP_GlbFreeLocalMem(pEDDPInfo);
    #endif

    EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] <-- eddp_undo_init(): Response=0x%X",
        Response);

    return (Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_system                                 +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE     pRQB             +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         Opcode:     EDD_OPC_SYSTEM                  +*/
/*+     LSA_HANDLE_TYPE         Handle:     Request-dependend               +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user (not used)           +*/
/*+     EDD_SERVICE             Service:    EDD_SRV_DEV_xxxx                +*/
/*+     EDD_RSP                 Status:     Return status                   +*/
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:     Depend on kind of request.      +*/
/*+                                                                         +*/
/*+     valid services: all EDD_SRV_DEV_xxx - Services                      +*/
/*+                                                                         +*/
/*+  RQB-return values in Requestblock                                      +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         opcode:     EDD_OPC_SYSTEM                  +*/
/*+     LSA_HANDLE_TYPE         Handle:                                     +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user                      +*/
/*+     LSA_COMP_ID_TYPE        CompId:     unchanged                       +*/
/*+     EDD_RSP                 Service:    unchanged                       +*/
/*+     LSA_RESPONSE_TYPE       Status:     EDD_STS_OK                      +*/
/*+                                         EDD_STS_ERR_PARAM               +*/
/*+                                         EDD_STS_ERR_SERVICE             +*/
/*+                                         EDD_STS_ERR_OPCODE              +*/
/*+                                         others                          +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Handles devicerequest which will be used in systemadaption+*/
/*+               This functions have a RQB-Parameter structure.            +*/
/*+                                                                         +*/
/*+  Note on error-handling:                                                +*/
/*+                                                                         +*/
/*+               In some cases it is not possible to call the call-back-   +*/
/*+               function of the requestor to confirm the request. in this +*/
/*+               case we call the output-macro EDDP_RQB_ERROR with the RQB  +*/
/*+               to notify this error. This erros are most likely caused   +*/
/*+               by a implementation error an are of the type              +*/
/*+                                                                         +*/
/*+               EDD_STS_ERR_OPCODE                                        +*/
/*+               EDD_STS_ERR_SERVICE                                       +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDD_UPPER_IN_FCT_ATTR eddp_system(
    EDD_UPPER_RQB_PTR_TYPE      pRQB)
{
    EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] --> eddp_system(pRQB=0x%X)", pRQB);

    EDDP_ENTER_LOW_S();

    /*---------------------------------------------------------------------------*/
    /* Check for valid parameters. Here we cant use the call-back-function       */
    /* on error.                                                                 */
    /*---------------------------------------------------------------------------*/
    if (EDD_RQB_GET_OPCODE(pRQB) != EDD_OPC_SYSTEM)
    {
	    EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_UNEXP, "[H:--] eddp_system(): Invalid opcode (0x%X)",
            EDD_RQB_GET_OPCODE(pRQB));
	    EDD_RQB_SET_RESPONSE(pRQB, EDD_STS_ERR_OPCODE);
	    EDDP_RQB_ERROR(pRQB);
    }
    else
    {
	    switch (EDD_RQB_GET_SERVICE(pRQB))
	    {
	        /*-----------------------------------------------------------------------*/
	        /* Interrupt-request from eddp_interrupt.                                */
	        /*-----------------------------------------------------------------------*/
    	    case EDDP_SRV_DEV_INTERRUPT:
            {
	    	    EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] --> Request: EDDP_SRV_DEV_INTERRUPT (pRQB=0x%X)",
                    pRQB);
    		    EDDP_CoreISRRequest(pRQB);
		        EDDP_SYSTEM_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] <-- Request: EDDP_SRV_DEV_INTERRUPT finished.");
            }
		    break;

	        /*-----------------------------------------------------------------------*/
	        /* Timer-request from eddp_timeout                                       */
	        /*-----------------------------------------------------------------------*/
    	    case EDDP_SRV_DEV_TIMER:
            {
	    	    EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] --> Request: EDDP_SRV_DEV_TIMER (pRQB=0x%X)",
                    pRQB);
                EDDP_CoreTimerRequest(pRQB);
		        EDDP_SYSTEM_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] <-- Request: EDDP_SRV_DEV_TIMER finished.");
            }
		    break;

	        /*-----------------------------------------------------------------------*/
	        /* Trigger-request from edd                                              */
	        /*-----------------------------------------------------------------------*/
    	    case EDDP_SRV_DEV_TRIGGER:
            {
	    	    EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] --> Request: EDDP_SRV_DEV_TRIGGER (pRQB=0x%X)",
                    pRQB);
    		    EDDP_SYSTEM_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] eddp_system(): FATAL -> EDDP_SRV_DEV_TRIGGER not supported yet!");
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
		        EDDP_SYSTEM_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] <-- Request: EDDP_SRV_DEV_TRIGGER finished.");
            }
		    break;

	        /*-----------------------------------------------------------------------*/
	        /* Device open/close/setup/shutdown etc                                  */
	        /*-----------------------------------------------------------------------*/
	        case EDDP_SRV_DEV_OPEN:
            {
    		    EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] --> Request: EDDP_SRV_DEV_OPEN (pRQB=0x%X)",
                    pRQB);
	    	    EDDP_CoreDevOpenRequest(pRQB);
            }
		    break;

    	    case EDDP_SRV_DEV_SETUP:
            {
	    	    EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] --> Request: EDDP_SRV_DEV_SETUP (pRQB=0x%X)",
                    pRQB);
		        EDDP_CoreDevSetupRequest(pRQB);
            }
    		break;

	        case EDDP_SRV_DEV_IF_SETUP:
            {
    		    EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] --> Request: EDDP_SRV_DEV_IF_SETUP (pRQB=0x%X)",
                    pRQB);
	    	    //EDDP_CoreDevIFSetupRequest(pRQB);
                EDDP_SYSTEM_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] eddp_system(): FATAL -> EDDP_SRV_DEV_IF_SETUP not supported anymore!");
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            }
		    break;

    	    case EDDP_SRV_DEV_IF_SHUTDOWN:
            {
	    	    EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] --> Request: EDDP_SRV_DEV_IF_SHUTDOWN (pRQB=0x%X)",
                    pRQB);
		        //EDDP_CoreDevIFShutdownRequest(pRQB);
                EDDP_SYSTEM_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] eddp_system(): FATAL -> EDDP_SRV_DEV_IF_SHUTDOWN not supported anymore!");
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            }
    		break;

    	    case EDDP_SRV_DEV_SHUTDOWN:
            {
	    	    EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] --> Request: EDDP_SRV_DEV_SHUTDOWN (pRQB=0x%X)",
                    pRQB);
		        EDDP_CoreDevShutdownRequest(pRQB);
            }
		    break;

    	    case EDDP_SRV_DEV_CLOSE:
            {
	    	    EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] --> Request: EDDP_SRV_DEV_CLOSE (pRQB=0x%X)",
                    pRQB);
    		    EDDP_CoreDevCloseRequest(pRQB);
            }
	    	break;

	        /*-----------------------------------------------------------------------*/
	        /* default                                                               */
	        /*-----------------------------------------------------------------------*/
            default:
            {
	    	    EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_UNEXP, "[H:--] --> eddp_system: Invalid service (0x%X)",
                    EDD_RQB_GET_OPCODE(pRQB));
		        EDD_RQB_SET_RESPONSE(pRQB, EDD_STS_ERR_SERVICE); 
		        EDDP_RQB_ERROR(pRQB);
            }
		    break;
	    } /* switch */
    } /* else */

    EDDP_EXIT_LOW_S();

    EDDP_SYSTEM_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] <-- eddp_system()");
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_ll_PhyRegisterWrite                    +*/
/*+  Input/Output          :    EDDP_HANDLE		hDDB						+*/
/*+						   :	LSA_UINT32		HwPortID					+*/
/*+						   :	LSA_UINT32		RegisterNumber				+*/
/*+						   :	LSA_UINT16		Value						+*/
/*+  Result                :    EDD_RSP		                                +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Writes 'Value' to PHY register 'RegisterNumber' on Port	+*/
/*+				  'HwPortID'.												+*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP  EDDP_LOCAL_FCT_ATTR  eddp_ll_PhyRegisterWrite(  
    EDDP_HANDLE     hDDB,
    LSA_UINT32		HwPortID,
    LSA_UINT32		RegisterNumber,
    LSA_UINT32		Value)
{
	EDD_RSP					    Response;
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB;

    EDDP_SYSTEM_TRACE_03(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] --> eddp_ll_PhyRegisterWrite(HwPortID=%d, RegisterNumber=%d, Value=0x%X)", 
        HwPortID, RegisterNumber, Value);

    Response = EDDP_CoreDevGetDDB (hDDB, &pDDB);

	if (Response == EDD_STS_OK)
	{
        if ((HwPortID > 0) && (HwPortID <= pDDB->SWI.PortCnt))
        {
		    Response = EDDP_SWI_PHY_Reg_Write (pDDB, HwPortID, RegisterNumber, Value);
        }
        else
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_SYSTEM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] eddp_ll_PhyRegisterWrite(): ERROR -> HwPortID is not between 1..%d!", 
                pDDB->SWI.PortCnt);
        }
	}
	else
	{
        EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] eddp_ll_PhyRegisterWrite(): ERROR -> hDDB(0x%X) not valid!", 
            hDDB);
	}

    EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] <-- eddp_ll_PhyRegisterWrite(): Response=0x%X", Response);

    return (Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_ll_PhyRegisterRead                     +*/
/*+  Input/Output          :    EDDP_HANDLE		hDDB						+*/
/*+						   :	LSA_UINT32		HwPortID					+*/
/*+						   :	LSA_UINT32		RegisterNumber				+*/
/*+						   :	LSA_UINT16	   *pValue						+*/
/*+  Result                :    EDD_RSP		                                +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Reads PHY register 'RegisterNumber' on Port 'HwPortID'	+*/
/*+				  into *pValue.												+*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP  EDDP_LOCAL_FCT_ATTR  eddp_ll_PhyRegisterRead(  
    EDDP_HANDLE		hDDB,
    LSA_UINT32		HwPortID,
    LSA_UINT32		RegisterNumber,
    LSA_UINT32	   *pValue)
{
	EDD_RSP					    Response;
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB;

    EDDP_SYSTEM_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] --> eddp_ll_PhyRegisterRead(HwPortID=%d, RegisterNumber=%d)", 
        HwPortID, RegisterNumber);

    Response = EDDP_CoreDevGetDDB (hDDB, &pDDB);

	if (Response == EDD_STS_OK)
	{
        if ((HwPortID > 0) && (HwPortID <= pDDB->SWI.PortCnt))
        {
            Response = EDDP_SWI_PHY_Reg_Read (pDDB, HwPortID, RegisterNumber, pValue);
        }
        else
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_SYSTEM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] eddp_ll_PhyRegisterRead(): ERROR -> HwPortID is not between 1..%d!", 
                pDDB->SWI.PortCnt);
        }
	}
	else
	{
        EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] eddp_ll_PhyRegisterRead(): ERROR -> hDDB(0x%X) not valid!", 
            hDDB);
	}

    EDDP_SYSTEM_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] <-- eddp_ll_PhyRegisterRead(): Value=0x%X, Response=0x%X", 
        *pValue, Response);

    return (Response);
}

#define EDDP_SWICHAR_SWITCHINGMEMORY_GRANULARITY 128
#define EDDP_SWICHAR_QUEUEGROUPCOUNT 5

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_FillSwitchingCharacs                   +*/
/*+  Input/Output          :    EDD_UPPER_SWICHAR_PTR_TYPE  pSwiChar        +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pSwiChar              :  Pointer for SwitchingCharacteristics    (i/o) +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Fill all pSwiChar properties with valid values.           +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static LSA_VOID eddp_FillSwitchingCharacs(EDD_UPPER_SWICHAR_PTR_TYPE  pSwiChar)
{
    LSA_UINT i;
    
    pSwiChar->Asic.SupportedRTClasses = EDD_SWICHAR_RTCLASS_RTC1 + EDD_SWICHAR_RTCLASS_RTC2 + EDD_SWICHAR_RTCLASS_RTC3;
    pSwiChar->Asic.ForwardingMode = EDD_SWICHAR_FWD_MODE_STOREANDFORWARD;
    pSwiChar->Asic.QueueMemoryStrategy = EDD_SWICHAR_QUEUE_STRATEGY_DYNAMIC;
    pSwiChar->Asic.QueueGroupCount = EDDP_SWICHAR_QUEUEGROUPCOUNT;
    pSwiChar->Asic.SwitchingMemoryGranularity = EDDP_SWICHAR_SWITCHINGMEMORY_GRANULARITY;
    pSwiChar->Asic.Latency_ns = EDDP_PRM_MAXBRIDGEDELAY_ABSOLUTE_STANDARD_LONG_ERTEC200P_SPEC;
    
    
    // Switching of normal acyclic network traffic
    pSwiChar->Queue[0].PrioMappingPerQueueGroup = 
            EDD_SWICHAR_FRAME_PRIO_VLAN0 + 
            EDD_SWICHAR_FRAME_PRIO_VLAN1 +
            EDD_SWICHAR_FRAME_PRIO_VLAN2 +
            EDD_SWICHAR_FRAME_PRIO_VLAN3 +
            EDD_SWICHAR_FRAME_PRIO_VLAN4 +
            EDD_SWICHAR_FRAME_PRIO_VLAN5 +
            EDD_SWICHAR_FRAME_PRIO_VLAN6 +
            EDD_SWICHAR_FRAME_PRIO_VLAN7;
    
    // Switching of RT alarms
    pSwiChar->Queue[1].PrioMappingPerQueueGroup = 
            EDD_SWICHAR_FRAME_PRIO_ASRT_LOW +
            EDD_SWICHAR_FRAME_PRIO_ASRT_HIGH;
    
    // Switching of management and ORG communication
    pSwiChar->Queue[2].PrioMappingPerQueueGroup = 
            EDD_SWICHAR_FRAME_PRIO_HSYNC +
            EDD_SWICHAR_FRAME_PRIO_MGMT_LOW +
            EDD_SWICHAR_FRAME_PRIO_MGMT_HIGH;
    
    // Switching of cyclic RT communication
    pSwiChar->Queue[3].PrioMappingPerQueueGroup = 
            EDD_SWICHAR_FRAME_PRIO_RTC1 +
            EDD_SWICHAR_FRAME_PRIO_RTC2;
    
    // Switching of cyclic IRT communication
    pSwiChar->Queue[4].PrioMappingPerQueueGroup = 
            EDD_SWICHAR_FRAME_PRIO_RTC3;
    
    // Normale Frame Buffer Size (OTHER)
    pSwiChar->Queue[0].QueueGroupMemory = EDDP_SWI_QDB_BARRIER_P_RES_0_ERTEC200P_REV2 * EDDP_SWICHAR_SWITCHINGMEMORY_GRANULARITY;
    
    // RT Frame Buffer Size (RTA)
    pSwiChar->Queue[1].QueueGroupMemory = EDDP_SWI_QDB_BARRIER_P_RES_1_ERTEC200P_REV2 * EDDP_SWICHAR_SWITCHINGMEMORY_GRANULARITY;
    
    // Mangement and ORG Frame Buffer Size
    pSwiChar->Queue[2].QueueGroupMemory = 
            (       EDDP_SWI_QDB_BARRIER_P_RES_2_ERTEC200P_REV2 +   // MRP
                    EDDP_SWI_QDB_BARRIER_P_RES_3_ERTEC200P_REV2 +   // LLDP
                    EDDP_SWI_QDB_BARRIER_P_RES_4_ERTEC200P_REV2     // PTCP
            ) * EDDP_SWICHAR_SWITCHINGMEMORY_GRANULARITY;
    
    // Cyclic RT Frame Buffer Size (RT_L)
    pSwiChar->Queue[3].QueueGroupMemory = EDDP_SWI_QDB_BARRIER_P_RES_5_ERTEC200P_REV2 * EDDP_SWICHAR_SWITCHINGMEMORY_GRANULARITY;
    
    // cyclic IRT Frame Buffer Size (RT_H, RT_H1)
    pSwiChar->Queue[4].QueueGroupMemory = EDDP_SWI_QDB_BARRIER_P_RES_6_ERTEC200P_REV2 * EDDP_SWICHAR_SWITCHINGMEMORY_GRANULARITY;
    
    for(i = 0; i< EDD_CFG_MAX_PORT_CNT; i++)
    {
        pSwiChar->Port[i].SupportedTransmissionRates = EDD_AUTONEG_CAP_100BASETXFD;
        pSwiChar->Port[i].SchedulerStrategy = EDD_SWICHAR_SCHED_STRATEGY_WEIGHTEDROUNDROBIN;
    }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_SwiCharOutPutTrace                     +*/
/*+  Input/Output          :    EDD_UPPER_SWICHAR_PTR_TYPE  pSwiChar        +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pSwiChar              :  Pointer for SwitchingCharacteristics    (i/o) +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: OutPuts all pSwiChar parameter in the Trace               +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static LSA_VOID eddp_SwiCharOutPutTrace(EDD_UPPER_SWICHAR_PTR_TYPE  pSwiChar)
{
    LSA_UINT i;
    
    EDDP_SYSTEM_TRACE_03(
            EDDP_UNDEF_TRACE_IDX, 
            LSA_TRACE_LEVEL_CHAT, 
            "SWITCHING_CHARACTERISTICS: pSwiChar->Asic: SupportedRTClasses=%u ForwardingMode=%u QueueMemoryStrategy=%u",
            pSwiChar->Asic.SupportedRTClasses,
            pSwiChar->Asic.ForwardingMode,
            pSwiChar->Asic.QueueMemoryStrategy);
    EDDP_SYSTEM_TRACE_03(
            EDDP_UNDEF_TRACE_IDX, 
            LSA_TRACE_LEVEL_CHAT, 
            "SWITCHING_CHARACTERISTICS: pSwiChar->Asic: QueueGroupCount=%u SwitchingMemoryGranularity=%u Latency_ns=%u",
            pSwiChar->Asic.QueueGroupCount,
            pSwiChar->Asic.SwitchingMemoryGranularity,
            pSwiChar->Asic.Latency_ns);
    
    for(i = 0; i<pSwiChar->Asic.QueueGroupCount; i++)
    {
        EDDP_SYSTEM_TRACE_03(
                EDDP_UNDEF_TRACE_IDX, 
                LSA_TRACE_LEVEL_CHAT,
                "SWITCHING_CHARACTERISTICS: pSwiChar->Queue[%d]: QueueGroupMemory=%8x PrioMappingPerQueueGroup=%8x",
                i,
                pSwiChar->Queue[i].QueueGroupMemory,
                pSwiChar->Queue[i].PrioMappingPerQueueGroup
                );   
    }
    
    for(i = 0; i < EDD_CFG_MAX_PORT_CNT; i++)
    {
        EDDP_SYSTEM_TRACE_03(
                EDDP_UNDEF_TRACE_IDX, 
                LSA_TRACE_LEVEL_CHAT,
                "SWITCHING_CHARACTERISTICS: pSwiChar->Port[%d]: SupportedTransmissionRates=%4x SchedulerStrategy=%4x",
                i,
                pSwiChar->Port[i].SupportedTransmissionRates,
                pSwiChar->Port[i].SchedulerStrategy
                );   
    }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_InitDPBWithDefaults                    +*/
/*+  Input/Output          :    EDDP_UPPER_DPB_PTR_TYPE     pDPB            +*/
/*+                        :	LSA_UINT16                  Hardware        +*/
/*+                        :	LSA_UINT16                  EDDPLocation    +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDPB        : Pointer to DeviceParameterBlock                    (i/o) +*/
/*+  Hardware    : HW type to set into pDPB (e.g: EDDP_HW_ERTEC200P)   (in) +*/
/*+  EDDPLocation: Loc. to set into pDPB, e.g: EDDP_LOCATION_EXT_HOST  (in) +*/
/*+                                                                         +*/
/*+  Result:      EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Note:        Expect preinitialized pDPB with minimum:                  +*/
/*+               - valid pDPB->pIF[x]                                      +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Write default settings, given parameters into pDPB and    +*/
/*+               its pDPB->pIF[x]  	                                    +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR eddp_InitDPBWithDefaults(
    EDDP_UPPER_DPB_PTR_TYPE     pDPB,
    LSA_UINT16                  Hardware,
    LSA_UINT16                  EDDPLocation,
    LSA_UINT16                  Interface,
    EDD_UPPER_SWICHAR_PTR_TYPE  pSwiChar)
{
    EDD_RSP            Response;
    LSA_UINT32         i;
    LSA_BOOL           bIsIOC = LSA_FALSE;
    EDD_SWICHAR_TYPE   swiCharLocal;

    Response = EDD_STS_OK;

    if (LSA_HOST_PTR_ARE_EQUAL(pDPB, LSA_NULL))
    {   // expect valid pointer
        EDDP_SYSTEM_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] eddp_InitDPBWithDefaults(): ERROR -> pDPB is NULL!");
        return EDD_STS_ERR_PARAM;
    }

    // ***** basic cfg
    pDPB->Hardware      = Hardware;
    pDPB->HardwareIF    = Interface;
    pDPB->EDDPLocation  = EDDPLocation;
    pDPB->K32TraceLevel = 0; /* use 0 instead of LSA_TRACE_LEVEL_OFF to be sure it is 0 */

    pDPB->MaxInterfaceCntOfAllEDD    = EDD_CFG_MAX_INTERFACE_CNT;
    pDPB->MaxPortCntOfAllEDD         = EDD_CFG_MAX_PORT_CNT_ALL_INTERFACE;
    pDPB->GSharedRAM_BaseAddr        = LSA_NULL;
    pDPB->UserCmdSharedMem_BaseAddr  = LSA_NULL;
    pDPB->pPnipBase                  = LSA_NULL;
    pDPB->pK32TCMBase                = LSA_NULL;
    pDPB->pK32ATCMBase               = LSA_NULL;
    pDPB->pK32BTCMBase               = LSA_NULL;
    pDPB->pK32DDR3Base               = LSA_NULL;
    pDPB->CRT.pIOBaseAdrPerif        = LSA_NULL;
    pDPB->CRT.pIOBaseAdrExtRam       = LSA_NULL;
    pDPB->CRT.pCPMBaseAddrFastMem    = LSA_NULL;
    pDPB->CRT.pCPMBaseAddrSlowMem    = LSA_NULL;
    pDPB->CRT.CPMBaseAddrFastMemPNIP = 0;
    pDPB->CRT.CPMBaseAddrSlowMemPNIP = 0;
    pDPB->CRT.pPPMBaseAddrFastMem    = LSA_NULL;
    pDPB->CRT.pPPMBaseAddrSlowMem    = LSA_NULL;
    pDPB->CRT.PPMBaseAddrFastMemPNIP = 0;
    pDPB->CRT.PPMBaseAddrSlowMemPNIP = 0;
    pDPB->CRT.PPM_Lifetime64FastMem  = 0;
    pDPB->CRT.PPM_Lifetime64SlowMem  = 0;
    
    pDPB->SWI.PhyAddressOffset       = 0;

       
    /*-----------------------------------------------------------------------*/
    /* Fill SwitchingCharacterisics & OutPut it as Trace                     */
    /*-----------------------------------------------------------------------*/
    eddp_FillSwitchingCharacs(&swiCharLocal);
    
    eddp_SwiCharOutPutTrace(&swiCharLocal);
    
    if(!LSA_HOST_PTR_ARE_EQUAL(pSwiChar, LSA_NULL))
    {
        *pSwiChar = swiCharLocal;
    }
    
    
    /*-----------------------------------------------------------------------*/
    /* Setup device global parts and check for valid hardware                */
    /*-----------------------------------------------------------------------*/
    switch (Hardware)
    {
        case EDDP_HW_ERTEC200P:
        {
            // configuration for hardware parameter: for register IRQx_Control
            pDPB->HWCfg.IRQx_Control.IRQ2_Level     = EDDP_IRQX_CONTROL_IRQ_LEVEL_CFG_HIGH;
            pDPB->HWCfg.IRQx_Control.IRQ2_Length    = 0;
            pDPB->HWCfg.IRQx_Control.IRQ3_Level     = EDDP_IRQX_CONTROL_IRQ_LEVEL_CFG_HIGH;
            pDPB->HWCfg.IRQx_Control.IRQ3_Length    = 0;
            pDPB->HWCfg.RxLocalApplicationDelayExtNs = 0; /* don't care */
            pDPB->HWCfg.TxLocalApplicationFeedExtNs  = 0; /* don't care */
            
            // configuration for CRT
            pDPB->CRT.ProviderMaxNumber = EDDP_CRT_MAX_PROVIDER_ERTEC200P;
            pDPB->CRT.ConsumerMaxNumber = EDDP_CRT_MAX_CONSUMER_ERTEC200P;
            
            if(EDDP_HAL_HW_IF_B == Interface)
            {
                /* ERTEC200P does't support IFB */
                Response = EDD_STS_ERR_PARAM;
                EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR,"[H:--] eddp_InitDPBWithDefaults(): ERROR -> ERTEC200P supports no IFB! (%d)!",
                    Hardware);
            }
        }
        break;

        case EDDP_HW_ERTEC200P_FPGA:
        {
            // configuration for hardware parameter: for register IRQx_Control
            pDPB->HWCfg.IRQx_Control.IRQ2_Level     = EDDP_IRQX_CONTROL_IRQ_LEVEL_CFG_HIGH;
            pDPB->HWCfg.IRQx_Control.IRQ2_Length    = 7;
            pDPB->HWCfg.IRQx_Control.IRQ3_Level     = EDDP_IRQX_CONTROL_IRQ_LEVEL_CFG_HIGH;
            pDPB->HWCfg.IRQx_Control.IRQ3_Length    = 7;
            pDPB->HWCfg.RxLocalApplicationDelayExtNs = 0; /* don't care */
            pDPB->HWCfg.TxLocalApplicationFeedExtNs  = 0; /* don't care */
            
            // configuration for CRT
            pDPB->CRT.ProviderMaxNumber = EDDP_CRT_MAX_PROVIDER_ERTEC200P;
            pDPB->CRT.ConsumerMaxNumber = EDDP_CRT_MAX_CONSUMER_ERTEC200P;
            
            if(EDDP_HAL_HW_IF_B == Interface)
            {
                /* ERTEC200P does't support IFB */
                Response = EDD_STS_ERR_PARAM;
                EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR,"[H:--] eddp_InitDPBWithDefaults(): ERROR -> ERTEC200P_FPGA supports no IFB! (%d)!",
                    Hardware);
            }
        }
        break;
                
        case EDDP_HW_HERA:
		case EDDP_HW_HERA_FPGA:
        {
            // NOTE: there is no differentiation between FPGA and Asic for IRQx_Length
            
            // configuration for hardware parameter: for register IRQx_Control
            pDPB->HWCfg.IRQx_Control.IRQ2_Level     = EDDP_IRQX_CONTROL_IRQ_LEVEL_CFG_HIGH;
            pDPB->HWCfg.IRQx_Control.IRQ2_Length    = 0;
            pDPB->HWCfg.IRQx_Control.IRQ3_Level     = EDDP_IRQX_CONTROL_IRQ_LEVEL_CFG_HIGH;
            pDPB->HWCfg.IRQx_Control.IRQ3_Length    = 3;
            
            if(EDDP_HW_HERA == Hardware)
            {
                pDPB->HWCfg.RxLocalApplicationDelayExtNs = EDDP_CRT_CPM_TRANSFEREND_DELAY_ASIC_REV3;
                pDPB->HWCfg.TxLocalApplicationFeedExtNs  = EDDP_CRT_PPM_FETCHTIME_DELTA_ASIC_REV3;
            }
            else
            {
                pDPB->HWCfg.RxLocalApplicationDelayExtNs = EDDP_CRT_CPM_TRANSFEREND_DELAY_FPGA_REV3;
                pDPB->HWCfg.TxLocalApplicationFeedExtNs  = EDDP_CRT_PPM_FETCHTIME_DELTA_FPGA_REV3;
            }

            // configuration for CRT
            pDPB->CRT.ProviderMaxNumber = EDDP_CRT_MAX_PROVIDER_CNT_HERA;
            pDPB->CRT.ConsumerMaxNumber = EDDP_CRT_MAX_CONSUMER_CNT_HERA;

            bIsIOC = LSA_TRUE;

            // set offset of phy address to HWPortID
            if (EDDP_HAL_HW_IF_B == Interface) 
            {
                pDPB->SWI.PhyAddressOffset = 2;
                pDPB->SWI.PortMap.PortID_x_to_HardwarePort_y[0] = 1;
                pDPB->SWI.PortMap.PortID_x_to_HardwarePort_y[1] = 2;
                pDPB->SWI.PortMap.PortID_x_to_HardwarePort_y[2] = EDDP_PORT_NOT_CONNECTED;
                pDPB->SWI.PortMap.PortID_x_to_HardwarePort_y[3] = EDDP_PORT_NOT_CONNECTED;
                pDPB->SWI.PortMap.PortID_x_to_ASICPort_y[0] = 3;
                pDPB->SWI.PortMap.PortID_x_to_ASICPort_y[1] = 4;
                pDPB->SWI.PortMap.PortID_x_to_ASICPort_y[2] = PNIP_R3_REG_PORT_MAPPING_RSTV_P3_MAPPING;
                pDPB->SWI.PortMap.PortID_x_to_ASICPort_y[3] = PNIP_R3_REG_PORT_MAPPING_RSTV_P4_MAPPING;
                pDPB->SWI.PortMap.PortID_x_to_SMIPort_y[0] = 3;
                pDPB->SWI.PortMap.PortID_x_to_SMIPort_y[1] = 3;
                pDPB->SWI.PortMap.PortID_x_to_SMIPort_y[2] = 0;
                pDPB->SWI.PortMap.PortID_x_to_SMIPort_y[3] = 0;
            }
            else
            {
                pDPB->SWI.PortMap.PortID_x_to_HardwarePort_y[0] = 1;
                pDPB->SWI.PortMap.PortID_x_to_HardwarePort_y[1] = 2;
                pDPB->SWI.PortMap.PortID_x_to_HardwarePort_y[2] = 3;
                pDPB->SWI.PortMap.PortID_x_to_HardwarePort_y[3] = 4;                
                pDPB->SWI.PortMap.PortID_x_to_ASICPort_y[0] = 1;
                pDPB->SWI.PortMap.PortID_x_to_ASICPort_y[1] = 2;
                pDPB->SWI.PortMap.PortID_x_to_ASICPort_y[2] = 3;
                pDPB->SWI.PortMap.PortID_x_to_ASICPort_y[3] = 4;
                pDPB->SWI.PortMap.PortID_x_to_SMIPort_y[0] = 1;
                pDPB->SWI.PortMap.PortID_x_to_SMIPort_y[1] = 1;
                pDPB->SWI.PortMap.PortID_x_to_SMIPort_y[2] = 1;
                pDPB->SWI.PortMap.PortID_x_to_SMIPort_y[3] = 1;
            }
        }
        break;  

        default:
        {
            // not supported Hardware
            Response = EDD_STS_ERR_PARAM;
            EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR,"[H:--] eddp_InitDPBWithDefaults(): ERROR -> Hardware not supported! (%d)!",
                Hardware);
        }
        break;
    }

    // setup device global hardwareindependend parts 
    if (Response == EDD_STS_OK)
    {
        // ***** cfg for SWI
        pDPB->SWI.Multicast_Bridge_IEEE802RSTP_Forward = EDD_FEATURE_ENABLE;
    }

    /*-----------------------------------------------------------------------*/
    /* Setup interface parts                                                 */
    /*-----------------------------------------------------------------------*/
     if (Response == EDD_STS_OK)
    {
        pDPB->TraceIdx = 0;

        // ***** cfg for NRT
        // write default settings for TX
        if (bIsIOC)
        {
            pDPB->NRT.TxItemsInTcpRing   = 50;       // IP_TCP (if used eg: 32 items)
            pDPB->NRT.TxItemsInUdpRing   = 50;       // IP_UDP
        }
        else
        {
            pDPB->NRT.TxItemsInTcpRing   = 0;        // IP_TCP (if used eg: 32 items)
            pDPB->NRT.TxItemsInUdpRing   = 32;       // IP_UDP
        }
        pDPB->NRT.TxItemsInDefaultRing   = 8;        // LEN_TSYNC, default
        pDPB->NRT.TxItemsInIpMiscRing    = 16;       // IP_IGMP, IP_ICMP, ARP
        pDPB->NRT.TxItemsInDcpRing       = 8;        // DCP: (UDP)DCP, (UDP)DCP_HELLO
        pDPB->NRT.TxItemsInAlarmRing     = 16;       // ALARM: UDP_ASRT, ASRT
        pDPB->NRT.TxItemsInMrpRing       = 4;        // MRP: MRP, LEN_STDBY, LEN_HSR
        pDPB->NRT.TxItemsInLldpRing      = 4;        // LLDP
        pDPB->NRT.TxItemsInPtcpRing      = 8;        // SYNC: PTCP-Announce
        pDPB->NRT.TxSndTimeout           = 0x7D00;   // watchdog for timeout = 1000ms
        
        pDPB->NRT.FeedInLoadLimitationActive = EDD_SYS_FILL_ACTIVE; // FILL is active
        pDPB->NRT.IO_Configured              = EDD_SYS_IO_CONFIGURED_ON; // IO_Configured is active

        // write default settings for RX
        if (bIsIOC)
        {
            pDPB->NRT.RxItemsInTcpRing   = 50;      // DMA-CW Idx for TCP (if used eg: 32 items)
            pDPB->NRT.RxItemsInUdpRing   = 50;      // DMA-CW Idx for UDP
        }
        else
        {
            pDPB->NRT.RxItemsInTcpRing   = 0;        // DMA-CW Idx for TCP (if used eg: 32 items)
            pDPB->NRT.RxItemsInUdpRing   = 32;       // DMA-CW Idx for UDP
        }
        pDPB->NRT.RxItemsInOtherRing     = 0;        // DMA-CW Idx for OTHER, LEN_OTHER, IP_OTHER
        pDPB->NRT.RxItemsInIpMiscRing    = 16;       // DMA-CW Idx for ARP
        pDPB->NRT.RxItemsInAlarmRing     = 16;       // DMA-CW Idx for (ALARM) ASRT, UDP_ASRT
        pDPB->NRT.RxItemsInDcpRing       = 8;        // DMA-CW Idx for DCP, DCP_HELLO, UDP_DCP, UDP_DCP_HELLO, LEN_DCP
        pDPB->NRT.RxItemsInPtcpRing      = 8;        // DMA-CW Idx for PTCP_ANNOUNCE
        pDPB->NRT.RxItemsInLldpRing      = 4;        // DMA-CW Idx for LLDP
        pDPB->NRT.RxItemsInMrpRing       = 4;        // DMA-CW Idx for MRP, LEN_STDBY, LEN_HSR
        pDPB->NRT.RxItemsInMiscRing      = 4;        // DMA-CW Idx for IP_IGMP, IP_ICMP, LEN_TSYNC, LEN_SINEC_FWL
        pDPB->NRT.RxDropLogging          = EDD_FEATURE_DISABLE;
        pDPB->NRT.RxFilterUDP_Unicast    = EDD_SYS_UDP_WHITELIST_FILTER_OFF;
        pDPB->NRT.RxFilterUDP_Broadcast  = EDD_SYS_UDP_WHITELIST_FILTER_OFF;

        // ***** cfg for CRT
        pDPB->CRT.ApplTimerMode                 = EDDP_APPL_TIMER_CFG_MODE_NONE;
        pDPB->CRT.ApplTimerReductionRatio       = 1;
        pDPB->CRT.TransferEndCorrectionValue    = 0;
        pDPB->CRT.isTransferEndCorrectionPos    = EDDP_DPB_TRANSFERENDCORRECTIONVALUE_IS_POS;

        // ***** cfg for SWI
        // set ring ports of MRP to defaults
        pDPB->SWI.MaxMRP_Instances          = 0;
        pDPB->SWI.MRPDefaultRoleInstance0   = EDD_MRP_ROLE_NONE;
        pDPB->SWI.MRPSupportedRole          = 0;
        pDPB->SWI.MRPSupportedMultipleRole  = 0;
        pDPB->SWI.MRAEnableLegacyMode       = EDD_MRA_DISABLE_LEGACY_MODE;
        pDPB->SWI.MaxMRPInterconn_Instances = 0;
        pDPB->SWI.SupportedMRPInterconnRole = 0;

        for ( i=0; i < EDDP_MAX_PORT_CNT; i++ )
        {
            pDPB->SWI.MRPRingPort[i]                    = EDD_MRP_NO_RING_PORT;
            pDPB->SWI.SupportsMRPInterconnPortConfig[i] = EDD_SUPPORTS_MRP_INTERCONN_PORT_CONFIG_NO;
        }

        pDPB->FeatureSupport.bShortPreambleSupported = EDD_PORT_LONG_PREAMBLE;
	    pDPB->FeatureSupport.bMRPDSupported = EDD_FEATURE_DISABLE;
	    pDPB->FeatureSupport.FragmentationtypeSupported = EDD_DPB_FEATURE_FRAGTYPE_SUPPORTED_NO;
	    pDPB->FeatureSupport.IRTForwardingModeSupported = EDD_DPB_FEATURE_IRTFWDMODE_SUPPORTED_ABSOLUTE;
	    pDPB->FeatureSupport.MaxDFPFrames = 0;
	    
	    pDPB->FeatureSupport.AdditionalForwardingRulesSupported = EDD_FEATURE_DISABLE;
	    pDPB->FeatureSupport.ApplicationExist = EDD_FEATURE_DISABLE;
        pDPB->FeatureSupport.MRPInterconnFwdRulesSupported = EDD_FEATURE_DISABLE;
        pDPB->FeatureSupport.MRPInterconnOriginatorSupported = EDD_FEATURE_DISABLE;

        pDPB->FeatureSupport.EIPExtensions = EDD_FEATURE_DISABLE;
        pDPB->FeatureSupport.DLRExtensions = EDD_FEATURE_DISABLE;
     }

    return (Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_InitDSBWithDefaults                    +*/
/*+  Input/Output          :    EDDP_UPPER_DPB_PTR_TYPE		pDPB            +*/
/*+                        :	EDDP_UPPER_DSB_PTR_TYPE		pDSB            +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDPB        : Pointer to DeviceParameterBlock                     (in) +*/
/*+  pDSB        : Pointer to DeviceSetupBlock                        (i/o) +*/
/*+                                                                         +*/
/*+  Result:      EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Note:        Expect initialized pDPB with minimum:                     +*/
/*+               - Hardware, EDDPLocation,                                 +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Write default settings into pDSB  	                    +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP  EDDP_LOCAL_FCT_ATTR eddp_InitDSBWithDefaults(
    EDDP_UPPER_DPB_PTR_TYPE pDPB,
    EDDP_UPPER_DSB_PTR_TYPE pDSB)
{
    LSA_UINT8 Idx, Pattern;
    EDD_RSP   Response;

    Response = EDD_STS_OK;

    if (LSA_HOST_PTR_ARE_EQUAL(pDPB, LSA_NULL))
    {   // expect valid pointer
        EDDP_SYSTEM_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR,"[H:--] eddp_InitDSBWithDefaults(): ERROR -> pDPB is NULL!");
        return EDD_STS_ERR_PARAM;
    }

    if (LSA_HOST_PTR_ARE_EQUAL(pDSB, LSA_NULL))
    {   // expect valid pointer
        EDDP_SYSTEM_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR,"[H:--] eddp_InitDSBWithDefaults(): ERROR -> pDSB is NULL!");
        return EDD_STS_ERR_PARAM;
    }

    /*-----------------------------------------------------------------------*/
    /* Setup hardwaredependend parts and check for valid hardware            */
    /*-----------------------------------------------------------------------*/
    switch (pDPB->Hardware)
    {
        case EDDP_HW_ERTEC200P_FPGA:
        case EDDP_HW_ERTEC200P:
#if(PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P)
            /* ok */
            pDSB->physicalK32PNIPBase = 0x10600000; /* this is the physical address of PNIP in K32 address system (K32 in ERTEC200P) */
			pDSB->physicalPNIPK32DTCMBase = 0x04000000; /* this is the physical address of K32 Data TCM (DTCM) in PNIP address system (K32 in ERTEC200P) */
#else
 #ifdef _RENESAS_RZT_
			pDSB->physicalK32PNIPBase = 0x78200000; /* this is the physical address of PNIP in K32 address system (K32 in ERTEC200P) */
			pDSB->physicalPNIPK32DTCMBase = 0x78280000;
 #endif
 #ifdef _RENESAS_RZN_
			pDSB->physicalK32PNIPBase = 0x74200000; /* this is the physical address of PNIP in K32 address system (K32 in ERTEC200P) */
			pDSB->physicalPNIPK32DTCMBase = 0x74280000;
 #endif
#endif
			break;
        case EDDP_HW_HERA_FPGA:
		    pDSB->physicalK32PNIPBase = 0x00000000;     /* this is the physical address of PNIP in K32 address system. not used yet.*/
			pDSB->physicalPNIPK32DTCMBase = 0x00000000; /* this is the physical address of K32 Data TCM (BTCM) in PNIP address system. not used yet. */
        break;

        default:
        {
            // ***** not supported Hardware
            EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR,"[H:--] eddp_InitDSBWithDefaults(): ERROR -> Hardware not supported! (%d)!",
                pDPB->Hardware);
            Response = EDD_STS_ERR_PARAM;
        }
        break;
    }

    /*-----------------------------------------------------------------------*/
    /* setup hardwareindependend parts                                       */
    /*-----------------------------------------------------------------------*/
    if (Response == EDD_STS_OK)
    {
        Pattern = 0x00;

        for ( Idx=0; Idx<EDDP_MAX_PORT_CNT; Idx++ )
        {
            // delete array "Port_MAC_Address"
            EDDP_MEMSET_COMMON((EDD_COMMON_MEM_PTR_TYPE)&pDSB->PortParams[Idx].Port_MAC_Address, Pattern, sizeof(EDD_MAC_ADR_TYPE));

            pDSB->PortParams[Idx].IsWireless = EDD_PORT_IS_NOT_WIRELESS;

            if (Idx & 1)
            {   // ***** Port2 + 4: LSA_FALSE 
                pDSB->PortParams[Idx].IsMDIX = EDD_PORT_MDIX_DISABLED;
            }
            else
            {   // ***** Port1 + 3: LSA_TRUE
                pDSB->PortParams[Idx].IsMDIX = EDD_PORT_MDIX_ENABLED;
            }
            
            pDSB->PortParams[Idx].GBitSupport = EDD_FEATURE_DISABLE;
        }

        /*-----------------------------------------------------------------------*/
        /* Setup default wait times for group interrupts                         */
        /*-----------------------------------------------------------------------*/    
        pDSB->InterruptHandling.EOI_WaitTime = 977; /* EOI-WaitTime              */
                                                    /* in 1024ns ticks           */
                                                    /* 977 = 1ms                 */

        /*-----------------------------------------------------------------------*/
        /* Setup default wait times for single interrupts                        */
        /*-----------------------------------------------------------------------*/    
        pDSB->InterruptHandling.IRQ_WaitTime_AcycRcv = 31;  /* MUX-WaitTime for AcycRcv  */
                                                            /* in 32.768us ticks         */
                                                            /* 31 = 1ms                  */
    }

    return (Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_SetIRQ_WaitTime_to_MAX                 +*/
/*+  Input/Output          :    EDDP_HANDLE             hDDB                +*/
/*+						   :    LSA_UINT32              IRQ_Number          +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  hDDB        : EDDP Device Handle                                  (in) +*/
/*+  IRQ_Number  : IRQ Number                                          (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function sets IRQ_WaitTime of register               +*/
/*+               IRQxControl_MUXn to maximum value. Needed for limiting    +*/
/*+               interrupt load for receiving acyclical frames.            +*/
/*+               The right register is selected interally.                 +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR eddp_SetIRQ_WaitTime_to_MAX(
    EDDP_HANDLE             hDDB,
    LSA_UINT32              IRQ_Number)
{
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB;
    
	EDDP_ASSERT_FALSE(IRQ_Number > EDDP_HAL_ISR_MUX_CNT);
    
    if (EDDP_CoreDevGetDDB(hDDB, &pDDB) != EDD_STS_OK)
    {
	   	EDDP_LOWER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] eddp_SetIRQ_WaitTime_AcycRcv_to_MAX(): FATAL -> Invalid hDDB (0x%X)!", 
            hDDB);
    	EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }

    {
        LSA_UINT32 MuxReg = EDDP_HAL_REG32_READ(pDDB, EDDP_HalIsrMUXRegSubSys[pDDB->Core.Hal.Isr.CPUSubys].IRQControlMux[IRQ_Number - 2]);

        MuxReg = MuxReg | PNIP_REG_IRQ1CONTROL_MUX1__MSK_IRQ_WAITTIME;

        EDDP_HAL_REG32_WRITE(pDDB,
            EDDP_HalIsrMUXRegSubSys[pDDB->Core.Hal.Isr.CPUSubys].IRQControlMux[IRQ_Number - 2],
            MuxReg);
    }
}    

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_K32GetData                             +*/
/*+  Input/Output          :    EDDP_HANDLE             hDDB                +*/
/*+						   :    LSA_UINT8**             pData               +*/
/*+  Result                :    LSA_INT*                pLength             +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  hDDB        : EDDP Device Handle                                  (in) +*/
/*+  pData       : pointer of internal KRISC32 data                   (out) +*/
/*+  pLength     : length of internal KRISC32 data                    (out) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Get pointer and length of KRISC32 internal data.          +*/
/*+               This function is typically called in an FatalError        +*/
/*+               handler. For later analysis, the internal KRISC32 data    +*/
/*+               may be dumped to a file that could be opend with          +*/
/*+               PNExplorer or k32log.                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_SYSTEM_IN_FCT_ATTR eddp_K32GetData (
	EDDP_HANDLE						hDDB,
	LSA_UINT8**						pData,
	LSA_INT*						pLength)
{
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB;
    
	EDDP_ASSERT_FALSE(((0 == pData) || (0 == pLength)));
    
    if (EDDP_CoreDevGetDDB(hDDB, &pDDB) != EDD_STS_OK)
    {
	   	EDDP_LOWER_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL,"[H:--] eddp_K32GetData(): FATAL -> Invalid hDDB (0x%X)!", 
            hDDB);
    	EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
		return EDD_STS_ERR_PARAM;
    }

	if ((EDDP_HW_ERTEC200P == pDDB->Hardware) || (EDDP_HW_ERTEC200P_FPGA == pDDB->Hardware))
    {
        if (!LSA_HOST_PTR_ARE_EQUAL(pDDB->K32.pK32TCMBase, LSA_NULL))
        {
            *pData = (LSA_UINT8 *)pDDB->K32.pK32TCMBase;
            *pLength = (pDDB->K32.tcmSize - (pDDB->K32.tileSize * EDDP_HAL_REG32_READ(pDDB,PNIP_REG_KRISC_TCM_MAP)));
            return EDD_STS_OK;
        }
    }

	return (EDD_STS_ERR_NOT_SUPPORTED);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_SRTProviderGetCycleCounter             +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_HANDLE                 hDDB            +*/
/*+                             EDDP_CYCLE_COUNTER_PTR_TYPE pCycleCounter   +*/
/*+                                                                         +*/
/*+  Result                :    EDD_STS_OK                                  +*/
/*+                             EDD_STS_ERR_PARAM                           +*/
/*+                             EDD_STS_ERR_SEQUENCE                        +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  hDDB           : Interface handle                                      +*/
/*+  pCycleCounter  : Pointer to address for CycleCounter                   +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description:  Using this function, the current SRT-CycleCounter        +*/
/*+                of the EDDP can be read out for this interface.          +*/
/*+                                                                         +*/
/*+                The CycleCounter has a time base of 31.25us.             +*/
/*+                It is provided as a 64 bit value                         +*/
/*+                (consisting of high and low 32 bit value).               +*/
/*+                                                                         +*/
/*+                EDDP gets the CycleCounter from PNIP-Register            +*/
/*+                 - PNIP_REG_CYCLECOUNTER_A for Interface(0)              +*/
/*+                 - PNIP_REG_CYCLECOUNTER_B for Interface(1)              +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_SYSTEM_IN_FCT_ATTR eddp_SRTProviderGetCycleCounter(
    EDDP_HANDLE			      	    hDDB,
    EDDP_CYCLE_COUNTER_PTR_TYPE     pCycleCounter)
{
    EDD_RSP                     Response;
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB;

    Response    = EDD_STS_OK;
    pDDB        = (EDDP_LOCAL_DDB_PTR_TYPE) hDDB;

    EDDP_SYSTEM_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] --> eddp_SRTProviderGetCycleCounter()");

    /* ---------------------------- */
    /* check parameter              */
    /* ---------------------------- */
    if (LSA_HOST_PTR_ARE_EQUAL(pDDB, LSA_NULL))
    {
        Response = EDD_STS_ERR_SEQUENCE;
        EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] eddp_SRTProviderGetCycleCounter(): ERROR -> pDDB=NULL, Response=0x%X", 
            Response);
    }
    else
    {
        if (LSA_HOST_PTR_ARE_EQUAL(pCycleCounter, LSA_NULL))
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] eddp_SRTProviderGetCycleCounter(): ERROR -> pCycleCounter=NULL, Response=0x%X", 
                Response);
        }
    }

    /* ---------------------------- */
    /* get CycleCounter             */
    /* ---------------------------- */
    if (Response == EDD_STS_OK)
    {
        pCycleCounter->High = 0;
        pCycleCounter->Low  = 0;

        // set only CycleCounter-Low
        pCycleCounter->Low = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_CYCLECOUNTER_A);  // IFA

        EDDP_CORE_TRACE_03(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] eddp_SRTProviderGetCycleCounter(High=0x%X, Low=0x%X): Response=0x%X",
            pCycleCounter->High, pCycleCounter->Low, Response);
    }

    EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] <-- eddp_SRTProviderGetCycleCounter(): Response=0x%X", Response);

    return (Response);
}

/*=============================================================================
 * function name:  eddp_GetTimeFromPNIPRegister
 *
 * function:       Read time from PNIP register OCT_Time in nanoseconds
 *
 * parameters:     EDDP_LOCAL_DDB_PTR_TYPE    pDDB
 *                 LSA_UINT64               * pNanoSeconds
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID EDDP_LOCAL_FCT_ATTR eddp_GetTimeFromPNIPRegister (
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT64                * pNanoSeconds)
{
    LSA_UINT32 highTime;
    LSA_UINT32 lowTime;
    LSA_UINT32 firstTime;
    
    // First read high 32bit part of time... 
    firstTime = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_OCTHIGH_TIME);
    // ...then read low 32bit part of time... 
    lowTime = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_OCTLOW_TIME);
    // ...and read again high 32bit part of time...
    highTime = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_OCTHIGH_TIME);
    // ...to detect a possible wrap around
    if(highTime != firstTime)
    {
        // If wrap around occurred, read low 32bit part of time again
        lowTime = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_OCTLOW_TIME);
    }

    // Now store time into 64bit nanoseconds
    *pNanoSeconds   = highTime;
    *pNanoSeconds <<= 32;
    *pNanoSeconds  |= lowTime;
}

/*=============================================================================
 * function name:  eddp_GetSyncTime
 *
 * function:       Get current PNIO TimeStamp, UTCOffset and SyncState
 *                 Read PTCP time from PN-IP register OCT_Time
 *                 Read EpochNumber, UTCOffset and SyncState from KRISC32 module
 *                 The function may be called from interrupt context
 *
 * parameters:     EDDP_HANDLE                      hDDB
 *                 EDDP_TIMESTAMP_INFO_PTR_TYPE     pTimestampInfo
 *
 * return value:   EDD_RSP
*===========================================================================*/
EDD_RSP EDDP_SYSTEM_IN_FCT_ATTR eddp_GetSyncTime (
    EDDP_HANDLE						hDDB,
    EDDP_TIMESTAMP_INFO_PTR_TYPE	pTimestampInfo)
{
    EDDP_LOCAL_DDB_PTR_TYPE pDDB;
    EDD_RSP                 Response = EDD_STS_OK;
    
	Response = EDDP_CoreDevGetDDB(hDDB, &pDDB);
	if (EDD_STS_OK != Response)
	{
		Response = EDD_STS_ERR_PARAM;
	   	EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR,"[H:--] eddp_GetSyncTime(): ERROR -> Invalid hDDB=0x%X!", 
			hDDB);
	}
    else if (LSA_HOST_PTR_ARE_EQUAL(pTimestampInfo, LSA_NULL))
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] eddp_GetSyncTime(): ERROR -> pTimestampInfo=NULL hDDB=0x%X!", 
            hDDB);
    }

    if (EDD_STS_OK == Response) 
    {
        // Read time from PNIP register OCT_Time
        LSA_UINT64 nanoseconds;

        EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE UserCmdInfoBlockAddr;
        LSA_UINT32 TimeGetSequenceID, TimeGetSequenceIDCopy;
        LSA_UINT32 TimeGetStatusSyncState, TimeGetEpochUTCOffset;
        LSA_UINT32 TimeGetMACAddressLo, TimeGetMACAddressHi;

        // get address of UserCmd-Info-Block in host format
        UserCmdInfoBlockAddr = (EDDP_CMD_RAM_USER_CMD_INFO_BLOCK_PTR_TYPE) EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_USER_CMD_INFO_BLOCK, 0);

        do {
            Response = EDD_STS_OK_PENDING;

            // at first, get SequenceID from UserCmd-Info-Block
            TimeGetSequenceID = EDDP_HAL_MEM32_READ(pDDB, &UserCmdInfoBlockAddr->TimeGetSequenceID);

            // get current SOE Status and SyncState from UserCmd-Info-Block
            TimeGetStatusSyncState = EDDP_HAL_MEM32_READ(pDDB, &UserCmdInfoBlockAddr->TimeGetStatusSyncState);

            // get current UTC offset and PTCP epoch from UserCmd-Info-Block
            TimeGetEpochUTCOffset = EDDP_HAL_MEM32_READ(pDDB, &UserCmdInfoBlockAddr->TimeGetEpochUTCOffset);

            // get MAC Address from UserCmd-Info-Block
            TimeGetMACAddressLo = EDDP_HAL_MEM32_READ(pDDB, &UserCmdInfoBlockAddr->TimeGetMACAddressLo);
            TimeGetMACAddressHi = EDDP_HAL_MEM32_READ(pDDB, &UserCmdInfoBlockAddr->TimeGetMACAddressHi);

            // Read time from PNIP register OCT_Time
            eddp_GetTimeFromPNIPRegister(pDDB, &nanoseconds);

            // last action, get the copy of SequenceID from UserCmd-Info-Block
            TimeGetSequenceIDCopy = EDDP_HAL_MEM32_READ(pDDB, &UserCmdInfoBlockAddr->TimeGetSequenceIDCopy);
            if (TimeGetSequenceID == TimeGetSequenceIDCopy)
            {
                // and read the SequenceID again
                TimeGetSequenceID = EDDP_HAL_MEM32_READ(pDDB, &UserCmdInfoBlockAddr->TimeGetSequenceID);
                if (TimeGetSequenceID == TimeGetSequenceIDCopy)
                {
                    Response = EDD_STS_OK;
                }
            }

            // during a time set operation, SequenceID and it's copy are different
            if (EDD_STS_OK != Response)
            {
                //LSA_UINT32 diff;

                // get old Timestamp data from DDB
                TimeGetSequenceID        = pDDB->K32.TimestampInfo.SequenceID;
                TimeGetStatusSyncState   = pDDB->K32.TimestampInfo.TimesStamp.Status;
                TimeGetStatusSyncState <<= 16;
                TimeGetStatusSyncState  |= pDDB->K32.TimestampInfo.SyncState;
                TimeGetEpochUTCOffset    = pDDB->K32.TimestampInfo.TimesStamp.SecondsHigh;
                TimeGetEpochUTCOffset  <<= 16;
                TimeGetEpochUTCOffset   |= pDDB->K32.TimestampInfo.UTCOffset;
                EDDP_MAC_ADDR_EUI64_TO_HI_LO(pDDB->K32.TimestampInfo.MasterIdent, &TimeGetMACAddressHi, &TimeGetMACAddressLo);

                // Time before the invalidation of the InfoBlock by incrementing the SequenceID
                nanoseconds   = EDDP_HAL_MEM32_READ(pDDB, &UserCmdInfoBlockAddr->TimeGetOctHighTimeOld);
                nanoseconds <<= 32;
                nanoseconds  |= EDDP_HAL_MEM32_READ(pDDB, &UserCmdInfoBlockAddr->TimeGetOctLowTimeOld);

                // Time difference since the invalidation of the InfoBlock
                //diff  = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_RCT_DELAY);
                //diff -= EDDP_HAL_MEM32_READ(pDDB, &UserCmdInfoBlockAddr->TimeGetRctDelayOld);
                // Calculate the time 
                //nanoseconds += diff;

                // The SequenceID has to be inconsistent at the end of this sequence, otherwise a simple re-read of the timestamp is sufficient
                TimeGetSequenceIDCopy = EDDP_HAL_MEM32_READ(pDDB, &UserCmdInfoBlockAddr->TimeGetSequenceIDCopy);
                if (TimeGetSequenceID != TimeGetSequenceIDCopy)
                {
                    Response = EDD_STS_OK;
                }
            }
        } while (EDD_STS_OK != Response);

        // fill out the timestamp struct
        pTimestampInfo->SyncState = (LSA_UINT16)TimeGetStatusSyncState;
        pTimestampInfo->UTCOffset = (LSA_UINT16)TimeGetEpochUTCOffset;
        pTimestampInfo->SequenceID = TimeGetSequenceID;
        pTimestampInfo->TimesStamp.Status = (LSA_UINT16)(TimeGetStatusSyncState >> 16);
        pTimestampInfo->TimesStamp.SecondsHigh = (LSA_UINT16)(TimeGetEpochUTCOffset >> 16);
        pTimestampInfo->TimesStamp.Nanoseconds = (LSA_UINT32)(nanoseconds % 1000000000);
        pTimestampInfo->TimesStamp.SecondsLow = (LSA_UINT32)(nanoseconds / 1000000000);

        switch (pTimestampInfo->TimesStamp.Status)
        {
            case EDDP_PNIO_STATUS_NOT_SYNC:     // RUN_A
                // As long as NOT_SYNC, the SeqID must be manipulated.
                if (pTimestampInfo->SequenceID > 0)
                {
                    pTimestampInfo->SequenceID--;
                }
                EDDP_MEMSET_COMMON(pTimestampInfo->MasterIdent, 0, sizeof(pTimestampInfo->MasterIdent));
                EDDP_MEMSET_COMMON(pTimestampInfo->DomainIdent, 0, sizeof(pTimestampInfo->DomainIdent));
                break;

            case EDDP_PNIO_STATUS_LOCAL_SYNC:   // RUN_L
                if (EDDP_PNIO_STATUS_NOT_SYNC == pDDB->K32.TimestampInfo.TimesStamp.Status)
                {
                    // In case of being never in Sync until now, we need the MasterIdent anyway
                    EDDP_MAC_ADDR_HI_LO_TO_EUI64(pTimestampInfo->MasterIdent, TimeGetMACAddressHi, TimeGetMACAddressLo);
                    EDDP_MEMCOPY_COMMON(pTimestampInfo->DomainIdent, pDDB->K32.TimestampInfo.DomainIdent, sizeof(pTimestampInfo->DomainIdent));
                }
                else 
                {
                    EDDP_MEMSET_COMMON(pTimestampInfo->MasterIdent, 0, sizeof(pTimestampInfo->MasterIdent));
                    EDDP_MEMSET_COMMON(pTimestampInfo->DomainIdent, 0, sizeof(pTimestampInfo->DomainIdent));
                }
                break;

            case EDDP_PNIO_STATUS_MASTER_SYNC:  // RUN_C
                EDDP_MAC_ADDR_HI_LO_TO_EUI64(pTimestampInfo->MasterIdent, TimeGetMACAddressHi, TimeGetMACAddressLo);
                EDDP_MEMCOPY_COMMON(pTimestampInfo->DomainIdent, pDDB->K32.TimestampInfo.DomainIdent, sizeof(pTimestampInfo->DomainIdent));
                break;

            default:
                Response = EDD_STS_ERR_OPCODE;
                EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] eddp_GetSyncTime(): ERROR -> Invalid TimesStamp.Status=0x%X!",
                    pTimestampInfo->TimesStamp.Status);
                break;
        }

        EDDP_K32_TRACE_06(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] eddp_GetSyncTime(): Status=%u Epoch=%u Seconds=%u.%09u UTCOffset=%u SyncState=0x%04x", 
            pTimestampInfo->TimesStamp.Status,pTimestampInfo->TimesStamp.SecondsHigh,pTimestampInfo->TimesStamp.SecondsLow, pTimestampInfo->TimesStamp.Nanoseconds,
            pTimestampInfo->UTCOffset,pTimestampInfo->SyncState);
        EDDP_K32_TRACE_09(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] eddp_GetSyncTime(): SequenceID=%d TimeMaster=%02x-%02x-%02x-%02x %02x-%02x-%02x-%02x", 
            pTimestampInfo->SequenceID, 
            pTimestampInfo->MasterIdent[0], pTimestampInfo->MasterIdent[1], pTimestampInfo->MasterIdent[2], pTimestampInfo->MasterIdent[3], 
            pTimestampInfo->MasterIdent[4], pTimestampInfo->MasterIdent[5], pTimestampInfo->MasterIdent[6], pTimestampInfo->MasterIdent[7]);
    }
    return (Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_SysRed_ShM_Array_Get                   +*/
/*+  Input/Output          :    EDDP_HANDLE         hDDB                    +*/
/*+                             LSA_UINT16          ShMIndex                +*/
/*+                             LSA_UINT16*         pShMValue               +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+  hDDB      : the instance handle.                   (in)                +*/
/*+  ShMIndex  : shared memory array index              (in)                +*/
/*+                  [0...EDD_CFG_MAX_SRSHM_SIZE-1]                         +*/
/*+  pShMValue : pointer to where, the read value shall be written to (out) +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_EXCP                                          +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: get the value to ShMValue of the element with the index   +*/ 
/*+               ShMIndex in the sr shared memory array                    +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP eddp_SysRed_ShM_Array_Get (
    EDDP_HANDLE         hDDB,
    LSA_UINT16          const ShMIndex,
    LSA_UINT16*         const pShMValue)
{
    EDD_RSP                     Response;
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB;

    Response    = EDD_STS_OK;
    pDDB        = (EDDP_LOCAL_DDB_PTR_TYPE) hDDB;

    EDDP_SYSTEM_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] --> eddp_SysRed_ShM_Array_Get(hDDB=0x%X, srshm_index=%d)",
                         hDDB, ShMIndex);

    if (LSA_HOST_PTR_ARE_EQUAL(pDDB, LSA_NULL))
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] eddp_SysRed_ShM_Array_Get(): ERROR -> pDDB=NULL, Response=0x%X",
                           Response);
        
    }else if(LSA_HOST_PTR_ARE_EQUAL(pDDB->pGSharedRAM,LSA_NULL))
    {
        Response = EDD_STS_ERR_EXCP;
        EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] eddp_SysRed_ShM_Array_Get(): ERROR -> pGSharedRAM=NULL, Response=0x%X",
                           Response);

    }else if( ShMIndex >= EDD_CFG_MAX_SRSHM_SIZE )
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_CORE_TRACE_03(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] eddp_SysRed_ShM_Array_Get(): ERROR -> srshm_index=%d invalid, shall be (0...%d), Response=0x%X",
            ShMIndex, EDD_CFG_MAX_SRSHM_SIZE-1, Response);
    }
    if(EDD_STS_OK == Response)
    {
        *pShMValue = LE_TO_H_S(((LSA_UINT16*)pDDB->pGSharedRAM)[ShMIndex]);
    }

    EDDP_SYSTEM_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] <-- eddp_SysRed_ShM_Array_Get(): pShMValue=%d, Response=0x%X", *pShMValue, Response);
    return (Response);


}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    eddp_GetMIB2Counters                                    +*/
/*+  Input/Output          :    EDDP_HANDLE                               hDDB          +*/
/*+                             LSA_UINT32                                PortID        +*/
/*+                             LSA_UINT8                                 Reset         +*/
/*+                             EDD_UPPER_GET_STATISTICS_MIB_PTR_TYPE     pMIB          +*/
/*+                                                                                     +*/
/*+  Result                :    EDD_RSP                                                 +*/
/*+-------------------------------------------------------------------------            +*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+  hDDB      : Device handle (refer to service EDDP_SRV_DEV_OPEN) (in)                +*/
/*+  PortID    : Port number (0= local interface, 1 = first port, 2 = second ..) (in)   +*/
/*+                  [0...MaxPortCnt]                                                   +*/
/*+  Reset     : EDD_STATISTICS_RESET_VALUES: Reset statistics-counters for             +*/
/*+                     this interface and the port                                     +*/
/*+                     specified in PortID, after having read the statistics-counters  +*/
/*+                     (�get-then-reset�).                                             +*/
/*+              EDD_STATISTICS_DO_NOT_RESET_VALUES: Do not reset statistic-counters    +*/
/*+              (in)                                                                   +*/
/*+                                                                                     +*/
/*+  Results    : EDD_STS_OK                                                            +*/
/*+               EDD_STS_ERR_PARAM                                                     +*/
/*+               EDD_STS_ERR_NOT_SUPPORTED                                             +*/
/*+-------------------------------------------------------------------------------------+*/
/*+  Description: Get / reset MIB2 counters for this instance                           +*/ 
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP eddp_GetMIB2Counters (
        EDDP_HANDLE                           const hDDB,
        LSA_UINT32                            const PortID,
        LSA_UINT8                             const Reset,
        EDD_UPPER_GET_STATISTICS_MIB_PTR_TYPE const pMIB)
{
    EDD_RSP                                Response = EDD_STS_OK;
    EDDP_LOCAL_DDB_PTR_TYPE                pDDB     = LSA_NULL;
    LSA_UINT32                             HwPortID = EDDP_MAX_PORT_CNT;
    

    pDDB        = (EDDP_LOCAL_DDB_PTR_TYPE) hDDB;
    EDDP_SYSTEM_TRACE_04(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] --> eddp_GetMIB2Counters(hDDB=0x%X, PortID=%d, Reset=%d, pMIB==0x%X)",
                         hDDB, PortID, Reset, pMIB);

    //validate paramter
    if (   (    LSA_HOST_PTR_ARE_EQUAL(pDDB, LSA_NULL)                          )
        || (    LSA_HOST_PTR_ARE_EQUAL(pMIB, LSA_NULL)                          )
        || (    PortID > pDDB->SWI.PortCnt                                      )
        || (    (EDD_STATISTICS_DO_NOT_RESET_VALUES != Reset                    )
             && (EDD_STATISTICS_RESET_VALUES        != Reset                    )
           ) 
       )
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_CORE_TRACE_04(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] eddp_GetMIB2Counters(): ERROR -> "
                "pDDB=0x%X, pMIB=0x%X, PortID=%d, Reset=%d, Response=EDD_STS_ERR_PARAM",
                 pDDB,pMIB,PortID,Reset);
    }
    else if (   ( pDDB->FeatureSupport.EIPExtensions == EDD_FEATURE_DISABLE       )
             || ( EDDP_HALGetPnipRevision(pDDB)      != EDDP_HAL_HW_PNIP_REV2     )
            )
    {
        Response = EDD_STS_ERR_NOT_SUPPORTED;
        EDDP_CORE_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] eddp_GetMIB2Counters(): ERROR -> "
                " EIPExtensions=0x%X, EDDP_HALGetPnipRevision(pDDB)=0x%X, Response=EDD_STS_ERR_NOT_SUPPORTED",
                 pDDB->FeatureSupport.EIPExtensions,EDDP_HALGetPnipRevision(pDDB));
    }
    else if(PortID)
    {
        //read statistics for a port with the user port id PortID
        HwPortID = EDDP_SWI_get_HwPortID (pDDB, PortID);
        EDDP_GEN_GetPortStatistics (pDDB, pMIB, HwPortID);
    }
    else
    {
        //read statistics for the interface
        EDDP_GEN_GetIfaceStatistics (pDDB, pMIB);
    }
    if(Response == EDD_STS_OK)
    {
        EDDP_ASSERT( pDDB->Core.pShadowStatistic != LSA_NULL );
        Response = EDDP_GEN_SubStatistics (pDDB, pMIB,&(pDDB->Core.pShadowStatistic[PortID]), Reset);
    }
    
    EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] <-- eddp_GetMIB2Counters(): Response=0x%X",Response);
    return Response;
}

/*****************************************************************************/
/*  end of file EDDP_CORE_SYS.C                                              */
/*****************************************************************************/
