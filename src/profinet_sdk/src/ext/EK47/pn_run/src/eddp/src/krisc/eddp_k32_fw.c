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
/*  F i l e               &F: eddp_k32_fw.c                             :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP KRISC32 firmware communication functions    */
/*                                                                           */
/*****************************************************************************/
#ifdef EDDP_MESSAGE
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
/*  2008-12-22  lrg   initial version.                                       */
/*  2009-05-19  lrg   LTRC_ACT_MODUL_ID                                      */
/*  2011-02-14  lrg   Remove global edd_usr.h                                */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID  122
#define EDDP_MODULE_ID     LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_K32_FW */

#include "eddp_inc.h"               /* eddp headerfiles */
#ifndef K32_KRISC32OS //140211lrg
#include "eddp_int.h"               /* internal header  */
#endif

#include "edd_cfg.h"
#include "eddp_cfg.h"
#include "eddp_k32_cfg.h"
#include "eddp_k32_fw.h"

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)

#ifdef K32_KRISC32OS //lrg
#include "krisc32_hal.h" //lrg
#else //lrg

// Attention: HDBs[0..EDDP_CFG_MAX_CHANNELS] used in Openchannel() are over all devices/interfaces !!!
#ifdef EDDP_CFG_LOCAL_MEM_STATIC
//  Note: EDDP_CFG_MAX_CHANNELS is over all interfaces !!!
    EDDP_LOCAL_MEM_ATTR EDDP_HDB_K32_TYPE    g_EDDP_HDB_K32[EDDP_CFG_MAX_CHANNELS];
#endif

/*===========================================================================*/
/*                        public  functions                                  */
/*===========================================================================*/

/*=============================================================================
 * function name:  EDDP_K32_ISR_SW_EVENT0
 *
 * function:       Interrupt callback funktion.
 *
 * parameters:     -
 *
 * return value:   none
 *
 * NOTE: this funktion is listed in EDDP_IsrFctTable[]
 *       and should not called directly.
 *===========================================================================*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_K32_ISR_SW_EVENT0(
	EDDP_LOCAL_DDB_PTR_TYPE  pDDB,
	LSA_UINT32               EventNr )
{
	LSA_BOOL retval;
	LSA_BOOL handled = LSA_FALSE;
	K32_RQB_PTR_TYPE pRRQB;

    LSA_UNUSED_ARG(EventNr);       // handle unused parameter

	do
	{
		retval = EDDP_K32_RRQB_GET_ACK(pDDB,&pRRQB);
		if(pRRQB)
		{
			EDDP_K32_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] Got ACK-RRQB @%p, ACK-RRQB->opcode 0x%02x", pRRQB, pRRQB->Hdr.Opcode);
            if(!EDDP_K32AckRQB(pDDB,pRRQB))
            {
				EDDP_K32_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] Shutdown pending, ending RRQB schedule ahead of time - that's OK.");
				return;/*shutdown is now pending, no access to RRQB queues any more ! */
			}
			handled = LSA_TRUE;
		}
    }
    while(retval);
	do
	{
		retval = EDDP_K32_RRQB_GET(pDDB,&pRRQB);
		if(pRRQB)
		{
			EDDP_K32_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] Got RRQB @%p, RRQB->opcode 0x%02x", pRRQB, pRRQB->Hdr.Opcode);
            if(!EDDP_K32GetRQB(pDDB,pRRQB))
            {
				EDDP_K32_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] Shutdown pending, ending RRQB schedule ahead of time - that's OK.");
				return;/*shutdown is now pending, no access to RRQB queues any more ! */
			}

			handled = LSA_TRUE;
		}
    }
    while(retval);

	if(!handled)
	{
		EDDP_K32_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_WARN, "[H:--] No RRQB/ACK-RRQB in queue! (Event %d)",
            EventNr);
	}
}

/*=============================================================================
 * function name:  EDDP_K32_ISR_WD
 *
 * function:       Interrupt callback funktion.
 *
 * parameters:     -
 *
 * return value:   none
 *
 * NOTE: this funktion is listed in EDDP_IsrFctTable[]
 *       and should not called directly.
 *===========================================================================*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_K32_ISR_WD(
	EDDP_LOCAL_DDB_PTR_TYPE  pDDB,
	LSA_UINT32               EventNr )
{
    EDDP_K32_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL,"[H:--] KRISC32 WATCHDOG TIMEOUT! (EventNr: %d)",
        EventNr);
	EDDP_FATAL(EDDP_FATAL_ERR_KRISC32_WATCHDOG);

    LSA_UNUSED_ARG(pDDB);
    LSA_UNUSED_ARG(EventNr);       // handle unused parameter
}


/**
 *  set configuration after downloading data
 *
 * \param physicalPNIPK32Address    physical address of pnip in k32 system
 * 
 * \note This function must be called after FW download but before reset release!  
 */
LSA_VOID EDDP_K32_FW_SetConfigData(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32      physicalPNIPK32Address,
	LSA_UINT32      physicalK32DTCMPNIPAddress
    /* LSA_UINT32      xxx more config data maybe later... */
)
{
    _K32_LIVE_TRACE * K32LiveTraceStruct = (_K32_LIVE_TRACE * )pDDB->K32.pK32TCMBase;
    // running on an ARM966 with TCM?
    if(K32LiveTraceStruct)
    {
        
        EDDP_HAL_MEM32_WRITE(pDDB,&(K32LiveTraceStruct->startUpConfig[0]),physicalPNIPK32Address);
        EDDP_HAL_MEM32_WRITE(pDDB,&(K32LiveTraceStruct->startUpConfig[1]),physicalK32DTCMPNIPAddress);
        /* EDDP_HAL_MEM32_WRITE(pDDB,&(K32LiveTraceStruct->startUpConfig[2]),xxx); more config data maybe later... */
    }
}


/*=============================================================================
 * function name:  EDDP_K32_FW_DOWNLOAD
 *
 * function:       Download firmware into KRISC32.
 *
 * parameters:     none
 *
 *
 * return value:   Zero on success, otherwise the address (of the firmware
 *                 image) where the download error occurs.
 *===========================================================================*/
LSA_UINT32 EDDP_K32_FW_DOWNLOAD(
#ifndef K32_KRISC32OS
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
#endif
    LSA_UINT8                 * code,
    LSA_UINT32                  code_length,
    LSA_UINT8                 * data,
    LSA_UINT32                  data_length)
{
#if defined(EDDP_USE_K32_DLL)
	LSA_UNUSED_ARG(pDDB);
	LSA_UNUSED_ARG(code);
	LSA_UNUSED_ARG(code_length);
	LSA_UNUSED_ARG(data);
	LSA_UNUSED_ARG(data_length);
#else
	volatile LSA_UINT8* pDTCM = (LSA_UINT8 *)pDDB->K32.pK32TCMBase;
	LSA_UINT8 const * pData = data;
	LSA_UINT8 const * pCode = code;
    /* @note tcm and tile size are different in ertec200+ Rev1/Rev2!!! */
	LSA_UINT32 tile_size = pDDB->K32.tileSize;
	LSA_UINT32 tcm_size  = pDDB->K32.tcmSize;
	/* start of itcm tile in dtcm */
	volatile LSA_UINT8* pITCM;
	{
        #if (EDDP_CFG_TRACE_MODE != 0)
		LSA_UINT32 codelen = ((code_length +(tile_size-1))/tile_size)*tile_size;
		LSA_UINT32 datalen = tcm_size - codelen;
        #endif  
		EDDP_K32_TRACE_04(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] --- Downloading Firmware (%d/%d byte code, %d/%d byte data)",
            code_length, codelen, data_length, datalen);
	}

	pDTCM += 0;	/* start of dtcm */

	///* copy data */
	//EDDP_MEMCOPY_COMMON(pDTCM,pData,data_length);
	///* fill rest of data segment with zero */
	//EDDP_MEMSET_COMMON(pDTCM+data_length,0x00000000,tile_size - (data_length % tile_size));
	//data_length = 0;	/* no more data to copy */

	//pDTCM += tcm_size-tile_size;	/* start of itcm tile in dtcm */
	//while(code_length > 0)
	//{
	//	if(code_length >= tile_size)
	//	{
	//		EDDP_MEMCOPY_COMMON(pDTCM,pCode,tile_size);
	//		code_length -= tile_size;
 //       }
 //       else
	//	{
	//		EDDP_MEMCOPY_COMMON(pDTCM,pCode,code_length);
	//		EDDP_MEMSET_COMMON(pDTCM+code_length,0x00000000,tile_size-code_length);
	//		code_length = 0; /* no more code to copy */
	//	}
	//	pDTCM-=tile_size;	/* step to next itcm tile (tile_size back in dtcm!)*/
	//	pCode+=tile_size;
	//}

    {
		LSA_INT elements;

		/* copy data */
		for(elements = data_length; elements > 0; --elements)
		{
		    EDDP_HAL_MEM8_WRITE(pDDB,pDTCM,*pData);

			if(*pData != EDDP_HAL_MEM8_READ(pDDB,pDTCM))
			{
				return (LSA_UINT32)(pDTCM - (LSA_UINT8*)pDDB->K32.pK32TCMBase);
			}
			++pDTCM;    /* increment pointer */
			++pData;    /* increment pointer */
		}

		/* fill rest of data segment with zero */
		for(elements = tile_size - (data_length % tile_size); elements > 0; --elements)
		{
		    EDDP_HAL_MEM8_WRITE(pDDB,pDTCM,0x00);

			if(0x00 != EDDP_HAL_MEM8_READ(pDDB,pDTCM))
			{
				return (LSA_UINT32)(pDTCM - (LSA_UINT8*)pDDB->K32.pK32TCMBase);
			}
			++pDTCM;    /* increment pointer */
		}
		data_length = 0;	/* no more data to copy */

		pDTCM = (LSA_UINT8*)pDDB->K32.pK32TCMBase + (tcm_size - tile_size);	/* get ITCM pointer, (start of itcm tile in dtcm ) */
		/* copy code */
		while(code_length > 0)
		{
			pITCM = pDTCM;		/* restore ITCM pointer */
			if(code_length >= tile_size)
			{
				for(elements = tile_size; elements > 0; --elements)
				{
				    EDDP_HAL_MEM8_WRITE(pDDB,pITCM,*pCode);

					if(*pCode != EDDP_HAL_MEM8_READ(pDDB,pITCM))
					{
						return (LSA_UINT32)(pITCM - (LSA_UINT8*)pDDB->K32.pK32TCMBase);
					}
					++pITCM;    /* increment pointer */
					++pCode;    /* increment pointer */
				}
				code_length -= tile_size;
            }
            else
			{
				for(elements = code_length; elements > 0; --elements)
				{
				    EDDP_HAL_MEM8_WRITE(pDDB,pITCM,*pCode);

					if(*pCode != EDDP_HAL_MEM8_READ(pDDB,pITCM))
					{
						return (LSA_UINT32)(pITCM - (LSA_UINT8*)pDDB->K32.pK32TCMBase);
					}
					++pITCM;    /* increment pointer */
					++pCode;    /* increment pointer */
				}
				/* fill rest of code segment with zero */
				for(elements = tile_size-code_length; elements > 0; --elements)
				{
				    EDDP_HAL_MEM8_WRITE(pDDB,pITCM, 0x00);
					if(0x00 != EDDP_HAL_MEM8_READ(pDDB,pITCM))
					{
						return (LSA_UINT32)(pITCM - (LSA_UINT8*)pDDB->K32.pK32TCMBase);
					}
					++pITCM;    /* increment pointer */
				}
				code_length = 0; /* no more code to copy */
			}
			pDTCM -= tile_size;	/* step to next itcm tile (tile_size back in dtcm!)*/
		}
	}

#endif /* defined(EDDP_USE_K32_DLL) */

    return 0x00000000;
}
#endif  //lrg

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    K32_HAL_RRQB_GET                            +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB          +*/
/*+                             K32_RQB_PTR_TYPE*             ppRRQB        +*/
/*+                             volatile LSA_VOID_PTR_TYPE    pRRQBQ        +*/
/*+  Result                :    LSA_BOOL     								+*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  ppRRQB     : Pointer to Reduced RQB                             (out)  +*/
/*+  pRRQBQ     : void Pointer to RRQB-Queue                          (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:                                                           +*/
/*+    get next element from RRQB queue that should be processed            +*/
/*+    and acknowledged.                                                    +*/
/*+    returns LSA_TRUE if more RRQBs are available in RRQB queue.          +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_BOOL K32_HAL_RRQB_GET(
#ifndef K32_KRISC32OS
			EDDP_LOCAL_DDB_PTR_TYPE pDDB,
#endif /* K32_KRISC32OS */
			K32_RQB_PTR_TYPE* ppRRQB,
			volatile LSA_VOID_PTR_TYPE pRRQBQ
		)
{
	volatile RRQB_QUEUE* pRRQBQueue = (volatile RRQB_QUEUE*)pRRQBQ;
	LSA_UINT8 tmpOpcode;
	LSA_INT leftElements;
	LSA_UINT32 nextAck,freeElements;

    #ifndef K32_KRISC32OS
        LSA_UNUSED_ARG(pDDB);       // handle unused parameter
    #endif /* K32_KRISC32OS */

    nextAck = EDDP_HAL_MEM32_READ(pDDB,&(pRRQBQueue->iNextAck));
    freeElements = EDDP_HAL_MEM32_READ(pDDB,&(pRRQBQueue->iFree));

	/* how many elements left for acknowledge? */
	leftElements = freeElements - nextAck;
	K32_CIRCULAR_ADJUST(leftElements,K32_MAX_RRQB);

	tmpOpcode = EDDP_HAL_MEM8_READ(pDDB,&(pRRQBQueue->slot[nextAck].Hdr.Opcode));

	if((leftElements == 0) && (K32_OPC_NO_OPERATION != tmpOpcode))
	{
		leftElements = K32_MAX_RRQB;
	}

	/* AP01482568:EDDP/KRISC32 RRQB Queue Handling: check/clear bit to get/ack RRQB
	*/
	if ((0 < leftElements)
	&&  (K32_OPC_TO_DO_BIT == (tmpOpcode & K32_OPC_TO_DO_BIT)))
	{
		tmpOpcode &= ~K32_OPC_TO_DO_BIT;
		EDDP_HAL_MEM8_WRITE(pDDB,&(pRRQBQueue->slot[nextAck].Hdr.Opcode),tmpOpcode);
		*ppRRQB = (K32_RQB_PTR_TYPE)&pRRQBQueue->slot[nextAck];
		return ((0 < leftElements-1) ? LSA_TRUE : LSA_FALSE);
    }
    else
	{
		*ppRRQB = 0;
		return LSA_FALSE;
	}
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    K32_HAL_RRQB_SEND                           +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB          +*/
/*+                             K32_RQB_PTR_TYPE*             ppRRQB        +*/
/*+                             volatile LSA_VOID_PTR_TYPE    pRRQBQ        +*/
/*+  Result                :    LSA_BOOL     								+*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  ppRRQB     : Pointer to Reduced RQB                             (out)  +*/
/*+  pRRQBQ     : void Pointer to RRQB-Queue                          (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:                                                           +*/
/*+    Send a RRQB.                                                         +*/
/*+    returns LSA_TRUE if insert into queue,                               +*/
/*+      LSA_FALSE if queue is temporary full.                              +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_BOOL K32_HAL_RRQB_SEND(
#ifndef K32_KRISC32OS
			EDDP_LOCAL_DDB_PTR_TYPE pDDB,
#endif /* K32_KRISC32OS */
			K32_RQB_PTR_TYPE pRRQB,
			volatile LSA_VOID_PTR_TYPE pRRQBQ
		)
{
    LSA_UINT8 tmpOpcode;
    LSA_UINT32 freeElement;
	volatile RRQB_QUEUE* pRRQBQueue = (volatile RRQB_QUEUE*)pRRQBQ;

	freeElement = EDDP_HAL_MEM32_READ(pDDB,&(pRRQBQueue->iFree));

	tmpOpcode = EDDP_HAL_MEM8_READ(pDDB, &(pRRQBQueue->slot[freeElement].Hdr.Opcode));
	if(K32_OPC_NO_OPERATION == tmpOpcode)	/* queue is NOT full ... */
	{
		/* write RRQB */
#ifndef K32_KRISC32OS
		EDDP_K32CopyUpperRQB(pDDB,pRRQB, &(pRRQBQueue->slot[freeElement]));
#else
		EDDP_MEMCOPY_COMMON((LSA_VOID_PTR_TYPE)(&(pRRQBQueue->slot[freeElement])),pRRQB,sizeof(K32_RQB_TYPE));
#endif
		/* AP01482568:EDDP/KRISC32 RRQB Queue Handling: set bit to get/ack RRQB
		 * Note: read opcode again!
		 */
		tmpOpcode = EDDP_HAL_MEM8_READ(pDDB, &(pRRQBQueue->slot[freeElement].Hdr.Opcode));
		tmpOpcode |= K32_OPC_TO_DO_BIT;
		EDDP_HAL_MEM8_WRITE(pDDB,&(pRRQBQueue->slot[freeElement].Hdr.Opcode),tmpOpcode);

		/* new send element in queue */
        K32_CIRCULAR_INC(freeElement,K32_MAX_RRQB);
        EDDP_HAL_MEM32_WRITE(pDDB,&(pRRQBQueue->iFree),freeElement);

#ifndef K32_KRISC32OS
		/* set IRQ1 event60 */
		EDDP_HAL_REG32_WRITE(pDDB,PNIP_REG_IRQ1_ACTIVATE,1);
#else
		/* set IRQ2/3 event60 */
		K32_WRITEREG(PNIP_REG_IRQ2_ACTIVATE,1);
		K32_WRITEREG(PNIP_REG_IRQ3_ACTIVATE,1);
#endif /* K32_KRISC32OS */
		return LSA_TRUE;
    }
    else	/* queue is temporary full ... */
	{
		return LSA_FALSE;
	}
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    K32_HAL_RRQB_ACK                            +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB          +*/
/*+                             K32_RQB_PTR_TYPE*             ppRRQB        +*/
/*+                             volatile LSA_VOID_PTR_TYPE    pRRQBQ        +*/
/*+  Result                :    LSA_BOOL     								+*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  ppRRQB     : Pointer to Reduced RQB                             (out)  +*/
/*+  pRRQBQ     : void Pointer to RRQB-Queue                          (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:                                                           +*/
/*+    Acknowledge a processed RRQB.                                        +*/
/*+    returns the number of remaining elements in the RRQBs queue.         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_INT32 K32_HAL_RRQB_ACK(
#ifndef K32_KRISC32OS
			EDDP_LOCAL_DDB_PTR_TYPE pDDB,
#endif /* K32_KRISC32OS */
			K32_RQB_PTR_TYPE pRRQB,
			volatile LSA_VOID_PTR_TYPE pRRQBQ
		)
{
    LSA_UINT8 tmpOpcode;
    LSA_UINT32 nextAck;
	volatile RRQB_QUEUE* pRRQBQueue = (volatile RRQB_QUEUE*)pRRQBQ;

	nextAck = EDDP_HAL_MEM32_READ(pDDB,&(pRRQBQueue->iNextAck));

	tmpOpcode = EDDP_HAL_MEM8_READ(pDDB,&(pRRQBQueue->slot[nextAck].Hdr.Opcode));
	/* element in queue and acknowledgeable ? */
	if((&(pRRQBQueue->slot[nextAck]) == pRRQB) && (K32_OPC_NO_OPERATION != tmpOpcode) && (K32_OPC_TO_DO_BIT != (tmpOpcode & K32_OPC_TO_DO_BIT)))
	{
		LSA_INT32 elements;

		/* AP01482568:EDDP/KRISC32 RRQB Queue Handling: set bit to get/ack RRQB
		*/
		tmpOpcode |= K32_OPC_TO_DO_BIT;
		EDDP_HAL_MEM8_WRITE(pDDB,&(pRRQBQueue->slot[nextAck].Hdr.Opcode),tmpOpcode);
        K32_CIRCULAR_INC(nextAck,K32_MAX_RRQB);
        EDDP_HAL_MEM32_WRITE(pDDB,&(pRRQBQueue->iNextAck),nextAck);

		elements = EDDP_HAL_MEM32_READ(pDDB,&(pRRQBQueue->iFree)) - nextAck;
		K32_CIRCULAR_ADJUST(elements,K32_MAX_RRQB);

#ifndef K32_KRISC32OS
		/* set IRQ1 event60 */
		EDDP_HAL_REG32_WRITE(pDDB,PNIP_REG_IRQ1_ACTIVATE,1);
#else
		/* set IRQ2/3 event60 */
		K32_WRITEREG(PNIP_REG_IRQ2_ACTIVATE,1);
		K32_WRITEREG(PNIP_REG_IRQ3_ACTIVATE,1);
#endif /* K32_KRISC32OS */

		return elements;
	}
	return -1;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    K32_HAL_RRQB_GET_ACK                        +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB          +*/
/*+                             K32_RQB_PTR_TYPE*             ppRRQB        +*/
/*+                             volatile LSA_VOID_PTR_TYPE    pRRQBQ        +*/
/*+  Result                :    LSA_BOOL     								+*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  ppRRQB     : Pointer to Reduced RQB                             (out)  +*/
/*+  pRRQBQ     : void Pointer to RRQB-Queue                          (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:                                                           +*/
/*+    get next ACK-element from RRQB queue that should be processed        +*/
/*+    and finalized.                                                       +*/
/*+    returns LSA_TRUE if more ACK-RRQBs are available in RRQB queue.      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_BOOL K32_HAL_RRQB_GET_ACK(
#ifndef K32_KRISC32OS
			EDDP_LOCAL_DDB_PTR_TYPE pDDB,
#endif /* K32_KRISC32OS */
			K32_RQB_PTR_TYPE* ppRRQB,
			volatile LSA_VOID_PTR_TYPE pRRQBQ
		)
{
    LSA_UINT8 tmpOpcode;
	volatile RRQB_QUEUE* pRRQBQueue;
	LSA_UINT32 begin;
	LSA_INT leftElements;
	LSA_INT lastElement;

	pRRQBQueue = (volatile RRQB_QUEUE*)pRRQBQ;

    #ifndef K32_KRISC32OS
        LSA_UNUSED_ARG(pDDB);       // handle unused parameter
    #endif /* K32_KRISC32OS */

	/* how many elements left to finalize? */
    begin = EDDP_HAL_MEM32_READ(pDDB,&(pRRQBQueue->iBegin));
	leftElements = EDDP_HAL_MEM32_READ(pDDB,&(pRRQBQueue->iNextAck)) - begin;
	K32_CIRCULAR_ADJUST(leftElements,K32_MAX_RRQB);
    lastElement = begin -1;
	K32_CIRCULAR_ADJUST(lastElement,K32_MAX_RRQB);
	tmpOpcode = EDDP_HAL_MEM8_READ(pDDB,&(pRRQBQueue->slot[lastElement].Hdr.Opcode));
	if((leftElements == 0) && (K32_OPC_NO_OPERATION != tmpOpcode))
	{
		leftElements = K32_MAX_RRQB;
	}

	/* AP01482568:EDDP/KRISC32 RRQB Queue Handling: check/clear bit to get/ack RRQB
	*/
	{
	    volatile K32_RQB_TYPE* pRRQB = &pRRQBQueue->slot[begin];
	    tmpOpcode = EDDP_HAL_MEM8_READ(pDDB,&(pRRQB->Hdr.Opcode));
        if ((0 < leftElements)
        &&  (K32_OPC_TO_DO_BIT == (tmpOpcode & K32_OPC_TO_DO_BIT)))
        {
            tmpOpcode &= ~K32_OPC_TO_DO_BIT;
            EDDP_HAL_MEM8_WRITE(pDDB,&(pRRQB->Hdr.Opcode),tmpOpcode);
            *ppRRQB = (K32_RQB_TYPE*)pRRQB;
            return ((0 < leftElements-1) ? LSA_TRUE : LSA_FALSE);
        }
        else
        {
            *ppRRQB = 0;
            return LSA_FALSE;
        }
	}
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    K32_HAL_RRQB_FINALIZE                       +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB          +*/
/*+                             K32_RQB_PTR_TYPE*             ppRRQB        +*/
/*+                             volatile LSA_VOID_PTR_TYPE    pRRQBQ        +*/
/*+  Result                :    LSA_BOOL     								+*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  ppRRQB     : Pointer to Reduced RQB                             (out)  +*/
/*+  pRRQBQ     : void Pointer to RRQB-Queue                          (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:                                                           +*/
/*+    Finalize a processed ACK-RRQB.                                       +*/
/*+    returns the number of remaining ACK-elements in the RRQBs queue.     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_INT32 K32_HAL_RRQB_FINALIZE(
#ifndef K32_KRISC32OS
			EDDP_LOCAL_DDB_PTR_TYPE pDDB,
#endif /* K32_KRISC32OS */
			K32_RQB_PTR_TYPE pRRQB,
			volatile LSA_VOID_PTR_TYPE pRRQBQ
		)
{
    LSA_UINT8 tmpOpcode;
    LSA_UINT32 begin,nextAck;
	volatile RRQB_QUEUE* pRRQBQueue;

	pRRQBQueue = (volatile RRQB_QUEUE*)pRRQBQ;

    #ifndef K32_KRISC32OS
        LSA_UNUSED_ARG(pDDB);       // handle unused parameter
    #endif /* K32_KRISC32OS */

    begin = EDDP_HAL_MEM32_READ(pDDB,&(pRRQBQueue->iBegin));
    nextAck = EDDP_HAL_MEM32_READ(pDDB,&(pRRQBQueue->iNextAck));
    tmpOpcode = EDDP_HAL_MEM8_READ(pDDB,&(pRRQBQueue->slot[begin].Hdr.Opcode));

	/* element in queue and ready to finalize ?*/
    if((&pRRQBQueue->slot[begin] == pRRQB) && (K32_OPC_NO_OPERATION != tmpOpcode))
	{
		LSA_INT32 elements;
		EDDP_HAL_MEM8_WRITE(pDDB,&(pRRQBQueue->slot[begin].Hdr.Opcode),K32_OPC_NO_OPERATION);
        K32_CIRCULAR_INC(begin,K32_MAX_RRQB);
        EDDP_HAL_MEM32_WRITE(pDDB,&(pRRQBQueue->iBegin),begin);

		elements = nextAck - begin;
		K32_CIRCULAR_ADJUST(elements,K32_MAX_RRQB);
		return elements;
	}
	return -1;
}

/*=============================================================================
 * function name:  EDDP_K32_FW_DOWNLOAD_HERA
 *
 * function:       Download firmware into KRISC32.
 *
 * parameters:     fw_image is the complete firmware image as a byte array.
 *                 length is the length of the firmware image in byte.
 *                 dst destination address.
 *
 * return value:   Zero on success, otherwise the address (of the firmware
 *                 image) where the download error occurs.
 *===========================================================================*/
LSA_UINT32 EDDP_K32_FW_DOWNLOAD_HERA(
    const LSA_UINT8*         fw_image,
    LSA_UINT32               length,
    volatile LSA_UINT8*      dst)
#ifdef EDDP_K32_FW_DOWNLOAD_HERA_8BIT
{
    LSA_UINT32 ByteCounter;
    
    for(ByteCounter=0 ; ByteCounter < length ; ++ByteCounter)
    {
        *dst = fw_image[ByteCounter];
        ++dst;
    }

    dst = dst - ByteCounter;

    for(ByteCounter=0 ; ByteCounter < length ; ++ByteCounter)
    { 
        if(*dst != fw_image[ByteCounter])
        {
            return (LSA_UINT32)dst;
        }
        else
        {
            ++dst;
        }
    }

    return 0;
}
#else
{
    volatile LSA_UINT8* dst8;
    const LSA_UINT8* src8;
    LSA_UINT32 Counter;
    LSA_UINT32 DwCount = (LSA_UINT32)(length / 4);
    LSA_UINT32 DwRest = (LSA_UINT32)(length % 4);
    volatile LSA_UINT32* dst32 = (LSA_UINT32*)dst;
    const LSA_UINT32* src32 = (LSA_UINT32*)fw_image;
    
    for(Counter=0 ; Counter < DwCount ; ++Counter)
    {
        *dst32 = *src32;
        ++dst32;
        ++src32;
    }
	dst8 = (LSA_UINT8*)dst32;
	src8 = (LSA_UINT8*)src32;
    for(Counter=0 ; Counter < DwRest ; ++Counter)
    {
        *dst8 = *src8;
        ++dst8;
        ++src8;
    }

    dst32 = (LSA_UINT32*)dst;
    src32 = (LSA_UINT32*)fw_image;
    
    for(Counter=0 ; Counter < DwCount ; ++Counter)
    {
        if(*dst32 != *src32)
        {
            return (LSA_UINT32)dst32;
        }
        else
        {
			++dst32;
			++src32;
        }
    }
	dst8 = (LSA_UINT8*)dst32;
	src8 = (LSA_UINT8*)src32;
    for(Counter=0 ; Counter < DwRest ; ++Counter)
    {
        if(*dst8 != *src8)
        {
            return (LSA_UINT32)dst8;
        }
        else
        {
            ++dst8;
            ++src8;
		}
    }

    return 0;
}
#endif

/*=============================================================================
* function name:  EDDP_K32_FW_DOWNLOAD_HERA_64BIT
*
* function:       Download firmware into KRISC32.
*
* parameters:     fw_image is the complete firmware image as a byte array.
*                 length is the length of the firmware image in byte.
*                 dst destination address.
*
* return value:   Zero on success, otherwise the address (of the firmware
*                 image) where the download error occurs.
*===========================================================================*/
LSA_UINT32 EDDP_K32_FW_DOWNLOAD_HERA_64BIT(
    const LSA_UINT8*         fw_image,
    LSA_UINT32               length,
    volatile LSA_UINT8*      dst)
{
    LSA_UINT64 Counter;
    LSA_UINT64 LLwCount = (LSA_UINT64)((length+7) / 8);
    volatile LSA_UINT64* dst64 = (LSA_UINT64*)dst;
    const LSA_UINT64* src64 = (LSA_UINT64*)fw_image;

    for (Counter = 0; Counter < LLwCount; ++Counter)
    {
        *dst64 = *src64;
        ++dst64;
        ++src64;
    }

    dst64 = (LSA_UINT64*)dst;
    src64 = (LSA_UINT64*)fw_image;

    for (Counter = 0; Counter < LLwCount; ++Counter)
    {
        if (*dst64 != *src64)
        {
            return (LSA_UINT32)dst64;
        }
        else
        {
            ++dst64;
            ++src64;
        }
    }

    return 0;
}
