/***********************************************************************************************************************
 * Copyright [2021-2024] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics Corporation and/or its affiliates and may only
 * be used with products of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.
 * Renesas products are sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for
 * the selection and use of Renesas products and Renesas assumes no liability.  No license, express or implied, to any
 * intellectual property right is granted by Renesas.  This software is protected under all applicable laws, including
 * copyright laws. Renesas reserves the right to change or discontinue this software and/or this documentation.
 * THE SOFTWARE AND DOCUMENTATION IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND
 * TO THE FULLEST EXTENT PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY,
 * INCLUDING WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE
 * SOFTWARE OR DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR
 * DOCUMENTATION (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER,
 * INCLUDING, WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY
 * LOST PROFITS, OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

#include "compiler.h"
#include "stdint.h"
#include "usriod_cfg.h"
#include "usrapp_cfg.h"   // example application spec. configuration settings
#include "version_dk.h"
#include "usriod_cfg.h"
#include "pniousrd.h"
#include "pnioerrx.h"
#include "pniousrd.h"
#include "bspadapt.h"
#include "iodapi_event.h"
#include "os.h"
#include "os_taskprio.h"
#include "usriod_diag.h"
#include "usriod_utils.h"
#include "usriod_im_func.h"
#include "iod_cfg1.h"
#include "iodapi_rema.h"
#include "nv_data.h"
#include "PnUsr_Api.h"
#include "logadapt.h"
#include "perform_measure.h"
#include "pnpb_sub_real.h"
#include "lsa_cfg.h"
#include "iodapi_event.h"
#include "rzt2_pns_rsk.h"
#include "bsp_api.h"

TS_PNS_OWNERSHIP_CTRL g_tPnsOwnershipCtrl = {LSA_FALSE, 0U, 0U, 0U, LSA_NULL};


TS_PNS_MODPROP g_tPnsModPropList[] =
{
	{0x00030100, {1, {0, 0, 0}}, {4,  {1, 0x8000, 0x8001, 0x8002}} },
	{0x10010000, {3, {1, 2, 3}}, {5,  {1, 2, 3, 4, 5}} },
	{0x10020000, {3, {1, 2, 3}}, {5,  {1, 2, 3, 4, 5}} },
	{0x10030000, {3, {1, 2, 3}}, {12, {1, 2, 3, 4, 5 , 6, 7, 8, 9, 10, 11, 12}}},
};


TS_PNS_SUBCFG g_tPnsSubCfgList[] =
{
	{0, 0x00030101,  0,  0, {1, {0x00030100}}},
	{0, 0x00038000,  0,  0, {1, {0x00030100}}},
	{0, 0x00038001,  0,  0, {1, {0x00030100}}},
	{0, 0x00038002,  0,  0, {1, {0x00030100}}},

	{0, 0x1001FFFF,  0,  0, {1, {0x10010000}}},
	{0, 0x1002FFFF,  0,  0, {1, {0x10020000}}},
	{0, 0x1003FFFF,  0,  0, {1, {0x10030000}}},

	{0, 0x00010001,  1,  0, {2, {0x10010000, 0x10030000}}},
	{0, 0x00010002,  2,  0, {2, {0x10010000, 0x10030000}}},
	{0, 0x00010004,  4,  0, {2, {0x10010000, 0x10030000}}},
	{0, 0x00010040, 64,  0, {2, {0x10010000, 0x10030000}}},

	{0, 0x00020001,  0,  1, {2, {0x10020000, 0x10030000}}},
	{0, 0x00020002,  0,  2, {2, {0x10020000, 0x10030000}}},
	{0, 0x00020004,  0,  4, {2, {0x10020000, 0x10030000}}},
	{0, 0x00020040,  0, 64, {2, {0x10020000, 0x10030000}}},

	{0, 0x00030001,  1,  1, {1, {0x10030000}}},
	{0, 0x00030002,  2,  2, {1, {0x10030000}}},
	{0, 0x00030004,  4,  4, {1, {0x10030000}}},
	{0, 0x00030040, 64, 64, {1, {0x10030000}}}
};


TS_PNS_ARINFO  g_tPnsArList [IOD_CFG_NUMOF_AR];
TS_PNS_USR_REC g_tUsrRecList[IOD_CFG_NUMOF_SUBSLOTS];

PNIO_VOID RZT2_PNS_ClearUsrRecAll(PNIO_UINT16 nSlot, PNIO_UINT16 nSubslot);


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
PNIO_UINT32 RZT2_PNS_UpdateOutputData(PNIO_UINT32 nArNum, TS_PNS_SUBPROP_PTR ptSub)
{
	PNIO_UINT32 nStat = PNIO_OK;

	if( (NULL != ptSub) && (NULL != ptSub->tOut.pData) && (0 < ptSub->tOut.nDataSize) )
	{
		switch(ptSub->nSubId)
		{
		case 0x00020001: // Output: 1 Byte
			if(PNIO_S_GOOD == ptSub->tOut.u8Iops)
			{
				uint8_t u8LedSoll = *((uint8_t*)ptSub->tOut.pData);
//				uint8_t u8LedIst = RZT2_PNS_GetLedStat(BSP_LED_1);
//				if((0x01U & u8LedSoll) != (0x01U & u8LedIst))
//				{
//					Bsp_EbSetLed(BSP_LED_1, (0x01U & u8LedSoll) );
//				}
                if(0x01U & u8LedSoll)
                {
                    Bsp_EbSetLed(BSP_LED_1, 1);
                }
                else
                {
                    Bsp_EbSetLed(BSP_LED_1, 0);
                }
				ptSub->tOut.u8Iocs = PNIO_S_GOOD;
			}
			break;
		case 0x00020002: // Output: 2 Byte
		case 0x00020004: // Output: 4 Byte
		case 0x00020040: // Universal Output: Up to 64 Byte
		case 0x00030001: // Universal I/O Output:  1 Byte
		case 0x00030002: // Universal I/O Output: up to 2 Byte
		case 0x00030004: // Universal I/O Output: up to 4 Byte
		case 0x00030040: // Universal I/O Output: up to64 Byte
			if(PNIO_S_GOOD == ptSub->tOut.u8Iops)
			{
				ptSub->tOut.u8Iocs = PNIO_S_GOOD;
			}
			break;
		default:
			ptSub->tOut.u8Iocs = PNIO_S_BAD;
			break;
		}
	}
	else
	{
		nStat = PNIO_ERR_PARAM;
	}
	return nStat;
}

PNIO_UINT32 RZT2_PNS_UpdateInputData(PNIO_UINT32 nArNum, TS_PNS_SUBPROP_PTR ptSub)
{
	PNIO_UINT32 nStat = PNIO_OK;
	static uint8_t u8Cnt = 0U;
	static PNIO_UINT16 u16Cnt = 0U;
	static PNIO_UINT32 u32Cnt = 0U;

	u8Cnt += 10;
	u16Cnt += 10;
	u32Cnt += 10;

	if( (NULL != ptSub) && (NULL != ptSub->tIn.pData) && (0 < ptSub->tIn.nDataSize) )
	{
		switch(ptSub->nSubId)
		{
		case 0x00010001: // Input: 1 Byte
		{
//			*((uint8_t*)ptSub->tIn.pData) = RZT2_PNS_GetLedStat(BSP_LED_GROUP);
            *((uint8_t*)ptSub->tIn.pData) = 0xBB;
			ptSub->tIn.u8Iops = PNIO_S_GOOD;
		}break;
		case 0x00010002: // Input: 2 Byte
		{
			PNIO_UINT16 u16Input = LSA_HTONS(u16Cnt);
			OsMemCpy(ptSub->tIn.pData, &u16Input, ptSub->tIn.nDataSize);
			ptSub->tIn.u8Iops = PNIO_S_GOOD;
		}break;
		case 0x00010004: // Input: 4 Byte
		{
			PNIO_UINT32 u32Input = LSA_HTONL(u32Cnt);
			OsMemCpy(ptSub->tIn.pData, &u32Input, ptSub->tIn.nDataSize);
			ptSub->tIn.u8Iops = PNIO_S_GOOD;
		}break;
		case 0x00010040: // Universal Input: Up to 64 Byte
			OsMemSet(ptSub->tIn.pData, ptSub->tIn.nDataSize, ptSub->tIn.nDataSize);
			ptSub->tIn.u8Iops = PNIO_S_GOOD;
			break;
		case 0x00030001: // Universal I/O Input: 1 Byte
		case 0x00030002: // Universal I/O Input: up to 2 Byte
		case 0x00030004: // Universal I/O Input: up to 4 Byte
		case 0x00030040: // Universal I/O Input: up to64 Byte
			ptSub->tIn.u8Iops = PNIO_S_GOOD;
			if( (NULL != ptSub->tOut.pData) && (0 < ptSub->tOut.nDataSize) && (PNIO_S_GOOD == ptSub->tOut.u8Iops) )
			{
				if(ptSub->tIn.nDataSize <= ptSub->tOut.nDataSize)
				{
					OsMemCpy(ptSub->tIn.pData, ptSub->tOut.pData, ptSub->tIn.nDataSize);
				}
				else
				{
					OsMemCpy(ptSub->tIn.pData, ptSub->tOut.pData, ptSub->tOut.nDataSize);
				}
			}
			break;
		default:
			ptSub->tIn.u8Iops = PNIO_S_BAD;
			break;
		}
	}
	else
	{
		nStat = PNIO_ERR_PARAM;
	}
	return nStat;
}

PNUSR_CODE_FAST PNIO_UINT32 RZT2_PNS_DataXchOutput (TS_PNS_ARINFO_PTR ptAr)
{
	int nCnt = 0;
	PNIO_UINT32 Status = PNIO_OK;
	PNPB_REAL_SUB *ptSub;
	TS_PNS_SUBPROP tSubProp;
	PNIO_UINT16 SubIndex;

	//OsEnterX (OS_MUTEX_PNPB_PLUG);
	//OsEnterX (OS_MUTEX_PNPB);

	while( nCnt < IOD_CFG_MAX_NUMOF_SUBSLOTS)
	{
		ptSub = &PnpbMod.Sub[nCnt++];

		 if( (PNPB_PLUGGED == ptSub->PlugState) && (0 < ptSub->SubIdent) &&
			 (0 < (PNIO_SUB_PROP_OUT & ptSub->DataDirection)) && (0 < OutDatLen[ptSub->SlotNum][ptSub->SubNum]))
		 {
			 SubIndex = PnUsr_get_subslot_index(ptSub->SlotNum, ptSub->SubNum);
			 OsMemSet(&tSubProp, 0, sizeof(tSubProp));
			 tSubProp.nApi   = ptSub->ApiNum;
			 tSubProp.nSlot  = ptSub->SlotNum;
			 tSubProp.nSslot = ptSub->SubNum;
			 tSubProp.nModId = ptSub->ModIdent;
			 tSubProp.nSubId = ptSub->SubIdent;
			 tSubProp.tOut.u8Iops    = OutDatIops[ptSub->SlotNum][SubIndex];
			 tSubProp.tOut.u8Iocs    = OutDatIocs[ptSub->SlotNum][SubIndex];
			 tSubProp.tOut.nDataSize = OutDatLen [ptSub->SlotNum][SubIndex];
			 tSubProp.tOut.pData     = &OutData[ptSub->SlotNum][SubIndex][0];
			 if(PNIO_OK == RZT2_PNS_UpdateOutputData(1, &tSubProp))
			 {
				 OutDatIocs[ptSub->SlotNum][SubIndex] = tSubProp.tOut.u8Iocs;
			 }
			 else
			 {
				 OutDatIocs[ptSub->SlotNum][SubIndex] = PNIO_S_BAD;
			 }
		 }
	}

	//OsExitX (OS_MUTEX_PNPB);
	//OsExitX (OS_MUTEX_PNPB_PLUG);

	return (Status);
}

PNUSR_CODE_FAST PNIO_UINT32 RZT2_PNS_DataXchInput (TS_PNS_ARINFO_PTR ptAr)
{
	int nCnt = 0;
	PNIO_UINT32 Status = PNIO_OK;
	PNPB_REAL_SUB *ptSub;
	TS_PNS_SUBPROP tSubProp;
	PNIO_UINT16 SubIndex;

	//OsEnterX (OS_MUTEX_PNPB_PLUG);
	//OsEnterX (OS_MUTEX_PNPB);

	while( nCnt < IOD_CFG_MAX_NUMOF_SUBSLOTS)
	{
		ptSub = &PnpbMod.Sub[nCnt++];

		 if( (PNPB_PLUGGED == ptSub->PlugState) && (0 < ptSub->SubIdent) )
		 {
			 if(0 < (PNIO_SUB_PROP_IN_OUT & ptSub->DataDirection))
			 {
				 if( (0 < (PNIO_SUB_PROP_IN & ptSub->DataDirection)) && (0 < InDatLen[ptSub->SlotNum][ptSub->SubNum]))
				 {
					// OsMemSet(&tSubProp, 0, sizeof(tSubProp));
					 SubIndex = PnUsr_get_subslot_index(ptSub->SlotNum, ptSub->SubNum);
					 tSubProp.nApi   = ptSub->ApiNum;
					 tSubProp.nSlot  = ptSub->SlotNum;
					 tSubProp.nSslot = ptSub->SubNum;
					 tSubProp.nModId = ptSub->ModIdent;
					 tSubProp.nSubId = ptSub->SubIdent;
					 tSubProp.tIn.u8Iops    = InDatIops[ptSub->SlotNum][SubIndex];
					 tSubProp.tIn.u8Iocs    = InDatIocs[ptSub->SlotNum][SubIndex];
					 tSubProp.tIn.nDataSize = InDatLen [ptSub->SlotNum][SubIndex];
					 tSubProp.tIn.pData     = &InData[ptSub->SlotNum][SubIndex][0];
					 if((0 < (PNIO_SUB_PROP_OUT & ptSub->DataDirection)) && (0 < OutDatLen[ptSub->SlotNum][ptSub->SubNum]))
					 {
						 tSubProp.tOut.u8Iops    = OutDatIops[ptSub->SlotNum][SubIndex];
						 tSubProp.tOut.u8Iocs    = OutDatIocs[ptSub->SlotNum][SubIndex];
						 tSubProp.tOut.nDataSize = OutDatLen [ptSub->SlotNum][SubIndex];
						 tSubProp.tOut.pData     = &OutData[ptSub->SlotNum][SubIndex][0];
					 }

					 if(PNIO_OK == RZT2_PNS_UpdateInputData(1, &tSubProp))
					 {
						InDatIops[ptSub->SlotNum][SubIndex] = tSubProp.tIn.u8Iops;
					 }
					 else
					 {
						 InDatIops[ptSub->SlotNum][SubIndex] = PNIO_S_BAD;
					 }
				 }
			 }
			 else
			 {
				 InDatIops[ptSub->SlotNum][SubIndex] = PNIO_S_GOOD;
			 }
		 }
	}

	//OsExitX (OS_MUTEX_PNPB);
	//OsExitX (OS_MUTEX_PNPB_PLUG);

	return (Status);
}

PNIO_UINT32 RZT2_PNS_GetModProp(PNIO_UINT32 nApi, PNIO_UINT32 nModId, TS_PNS_MODPROP_PTR ptModProp)
{
	PNIO_UINT32 nStat = PNIO_ERR_IOD_NO_MODULE;

	if( (NULL != ptModProp) && (0U < nModId) )
	{
		int nCnt = 0;
		PNIO_UINT16 nItems = (sizeof(g_tPnsModPropList) / sizeof(TS_PNS_MODPROP));

		while( (PNIO_OK != nStat) && (nCnt < nItems) )
		{
			if(nModId == g_tPnsModPropList[nCnt].nModId)
			{
				OsMemCpy(ptModProp, &g_tPnsModPropList[nCnt], sizeof(TS_PNS_MODPROP));
				nStat = PNIO_OK;
			}
			nCnt++;
		}
	}
	else
	{
		nStat = PNIO_ERR_PARAM;
	}

	return nStat;
}

PNIO_UINT32 RZT2_PNS_GetSubCfg(PNIO_UINT32 nApi, PNIO_UINT32 nModId, PNIO_UINT32 nSubId, TS_PNS_SUBCFG_PTR ptSubCfg)
{
	PNIO_UINT32 nStat = PNIO_ERR_IOD_NO_SUBMODULE;

	if( (NULL != ptSubCfg) && (0U < nModId) && (0U < nSubId) )
	{
		int nCnt = 0;
		TS_PNS_SUBCFG_PTR ptCfg = NULL;
		PNIO_UINT16 nItems = (sizeof(g_tPnsSubCfgList) / sizeof(TS_PNS_SUBCFG));

		while( (NULL == ptCfg) && (nCnt < nItems) )
		{
			if( (nApi   == g_tPnsSubCfgList[nCnt].nApi) &&
				(nSubId == g_tPnsSubCfgList[nCnt].nSubId) )
			{
				ptCfg = &g_tPnsSubCfgList[nCnt];
			}
			nCnt++;
		}


		if(NULL != ptCfg)
		{
			nCnt = 0;
			while( (PNIO_OK != nStat) && (nCnt < ptCfg->tModList.nCnt) && (nCnt < RZT2_PNS_MAX_MOD) )
			{
				if(nModId == ptCfg->tModList.nModIds[nCnt])
				{
					OsMemCpy(ptSubCfg, ptCfg, sizeof(TS_PNS_SUBCFG));
					nStat = PNIO_OK;
				}
				nCnt++;
			}
		}
	}
	else
	{
		nStat = PNIO_ERR_PARAM;
	}

	return nStat;
}

PNIO_UINT32 RZT2_PNS_CheckIfSubAllowed(PNIO_UINT32 nApi, PNIO_UINT16 nSlot, PNIO_UINT16 nSslot, PNIO_UINT32 nModId, PNIO_UINT32 nSubId)
{
	PNIO_UINT32 nStat = PNIO_ERR_PARAM;
	TS_PNS_MODPROP tModProp;
	TS_PNS_SUBCFG tSubCfg;
	int nCnt = 0;

	OsMemSet(&tModProp, 0, sizeof(tModProp));

	nStat = RZT2_PNS_GetModProp(nApi, nModId, &tModProp);

	if(PNIO_OK == nStat)
	{
		nStat = PNIO_ERR_IOD_INVALID_SLOT;
		while( (PNIO_OK != nStat) && (nCnt < tModProp.tSlotList.nCnt) )
		{
			if(nSlot == tModProp.tSlotList.nSlots[nCnt])
			{
				nStat = PNIO_OK;
			}
			nCnt++;
		}
	}

	if(PNIO_OK == nStat)
	{
		nCnt = 0;
		nStat = PNIO_ERR_IOD_INVALID_SUBSLOT;
		while( (PNIO_OK != nStat) && (nCnt < tModProp.tSslotList.nCnt) )
		{
			if(nSslot == tModProp.tSslotList.nSslots[nCnt])
			{
				nStat = PNIO_OK;
			}
			nCnt++;
		}
	}

	if(PNIO_OK == nStat)
	{
		OsMemSet(&tSubCfg, 0, sizeof(tSubCfg));
		nStat = RZT2_PNS_GetSubCfg(nApi, nModId, nSubId, &tSubCfg);
	}

	if(PNIO_OK == nStat)
	{
		if(0U < nSlot)
		{
			switch(nSubId)
			{
			case 0x1001FFFF:
			case 0x1002FFFF:
			case 0x1003FFFF:
				if(1U != nSslot)
				{
					nStat = PNIO_ERR_IOD_INVALID_SUBSLOT;
				}
				break;
			default:
				if(1U == nSslot)
				{
					nStat = PNIO_ERR_IOD_INVALID_SUBSLOT;
				}
				break;
			}


		}
		else
		{
			PNIO_UINT32 SubId = 0U;
			switch(nSslot)
			{
			case 1:      SubId = 0x00030101; break;
			case 0x8000: SubId = 0x00038000; break;
			case 0x8001: SubId = 0x00038001; break;
#if (1< IOD_CFG_PDEV_NUMOF_PORTS)
			case 0x8002: SubId = 0x00038002; break;
#endif
			default:
				break;
			}

			if(SubId != nSubId)
			{
				nStat = PNIO_ERR_IOD_INVALID_SUBSLOT;
			}
		}
	}

	return nStat;
}

PNIO_UINT8 RZT2_PNS_GetIm0Support(PNIO_UINT32 Api, PNIO_UINT16 Slot, PNIO_UINT16 Subslot, PNIO_UINT32 ModId, PNIO_UINT32 SubId)
{
	PNIO_UINT8 u8Im0Support = PNIO_IM0_SUBMODULE;

	if(0U < Slot)
	{
		if(1U == Subslot)
		{
			u8Im0Support |= PNIO_IM0_MODULE;
		}
	}
	else
	{
		if(1U == Subslot)
		{
			u8Im0Support |= PNIO_IM0_DEVICE;
		}
	}

	return u8Im0Support;
}

PNIO_UINT32 RZT2_PNS_BuildIm0(PNIO_UINT32 nApi, PNIO_UINT16 nSlot, PNIO_UINT16 nSslot, PNIO_UINT32 nModId, PNIO_UINT32 nSubId, IM0_DATA *pIm0)
{
	PNIO_UINT32 Status = PNIO_NOT_OK;
	PNIO_UINT16 SubIndex = PnUsr_get_subslot_index(nSlot, nSslot);

	if( (LSA_NULL != pIm0) && (nSlot < IOD_CFG_MAX_SLOT_NUMBER) && (SubIndex <= IOD_CFG_MAX_NUMOF_SUBSLOTS) )
	{
		int size;
		pIm0->VendorId = IOD_CFG_VENDOR_ID;                   // PNIO_UINT16     VendorId;            // VendorIDHigh, VendorIDLow
		IOD_CFG_DEV_ANNOTATION_ORDER_ID;// PNIO_UINT8      OrderId [20];        // Order_ID, visible, must be 20 bytes here (fill with blanks)
		IOD_CFG_IM0_SERIAL_NUMBER;     // PNIO_UINT8      SerNum  [16];        // IM_Serial_Number, visible string, must be 16 bytes here (fill with blanks)
		pIm0->HwRevision = DEVKIT_HW_REVISION;     // PNIO_UINT16     HwRevision;          // IM_Hardware_Revision
		pIm0->SwRevision.srp = DEVKIT_VERSION_PREFIX;  // PNIO_UINT8      SwRevPrefix;         // software revision prefix
		pIm0->SwRevision.fe  = DEVKIT_VERSION_HH;      // PNIO_UINT8      SwRevFuncEnhanc;     // IM_SW_Revision_Funct_Enhancement
		pIm0->SwRevision.bf  = DEVKIT_VERSION_H;      // PNIO_UINT8      SwRevBugFix;         // IM_SW_Revision_Bug_Fix
		pIm0->SwRevision.ic  = DEVKIT_VERSION_L;      // PNIO_UINT8      SwRevInternChange;   // IM_SW_Revision_Internal_Change
		pIm0->Revcnt         = DEVKIT_VERSION_LL;                   // PNIO_UINT16     Revcnt;              // IM_Revision_Counter, notifies a hw modification
		pIm0->ProfId 		 = 0x00;                   // PNIO_UINT16     ProfId;              // IM_Profile_ID, see Profile Guideline I&M functions
		pIm0->ProfSpecTyp 	 = 0x05;                   // PNIO_UINT16     ProfSpecTyp;         // IM_Profile_Spec_Type (e.g. 5="interface module")
		pIm0->VersMaj 		 = 0x01;                   // PNIO_UINT8      VersMaj;             // IM_Version major
		pIm0->VersMin 		 = 0x01;                   // PNIO_UINT8      VersMin;             // IM_Version minor (must be 1, do not change !!)
#if IOD_INCLUDE_IM5
		pIm0->ImXSupported = PNIO_SUPPORT_IM12345;    // PNIO_UINT16     ImSupported;         // IM_Supported, IM1...5 supported (bit list, bit 1...bit5, here: 0 ==> IM1..5  supported)
#else
		pIm0->ImXSupported = PNIO_SUPPORT_IM1234;     // PNIO_UINT16     ImSupported;         // IM_Supported, IM1...4 supported (bit list, bit 1...bit4, here: 0 ==> IM1..4  supported)
#endif

		OsMemSet(&pIm0->OrderId[0], ' ', IM0_ORDER_ID_LEN);
		size = (sizeof(IOD_CFG_DEV_ANNOTATION_ORDER_ID) -1);
		if(IM0_ORDER_ID_LEN < size)
		{
			size = IM0_ORDER_ID_LEN;
		}
		OsMemCpy(&pIm0->OrderId[0], (void*)IOD_CFG_DEV_ANNOTATION_ORDER_ID, size);

		OsMemSet(&pIm0->SerNum[0], ' ', IM0_SER_NUM_LEN);
		size = (sizeof(IOD_CFG_IM0_SERIAL_NUMBER) -1);
		if(IM0_SER_NUM_LEN < size)
		{
			size = IM0_SER_NUM_LEN;
		}
		OsMemCpy(&pIm0->SerNum[0], (void*)IOD_CFG_IM0_SERIAL_NUMBER, size);

		if(0U < nSlot)
		{
			pIm0->ProfSpecTyp = 0x3;
			if(1U < nSslot)
			{
				pIm0->ImXSupported = PNIO_SUPPORT_IM1234;
			}
			else
			{
				pIm0->ImXSupported = PNIO_SUPPORT_IM12345;
			}
		}
		else
		{
			pIm0->ImXSupported = PNIO_SUPPORT_IM12345;
		}
		Status = PNIO_OK;
	}

	return Status;
}

PNIO_VOID *RZT2_PNS_GetOwnershipCtrl(PNIO_UINT16 DevHdl)
{
	PNIO_VOID *pHandle = LSA_NULL;

	if(PNIO_SINGLE_DEVICE_HNDL == DevHdl)
	{
		pHandle  = &g_tPnsOwnershipCtrl;
	}

	return pHandle;
}

PNIO_VOID RZT2_PNS_OwnershipInit(PNIO_UINT32 DevHdl)
{
	TS_PNS_OWNERSHIP_CTRL_PTR ptOwnshipCtrl = (TS_PNS_OWNERSHIP_CTRL_PTR) RZT2_PNS_GetOwnershipCtrl(DevHdl);
	if(LSA_NULL != ptOwnshipCtrl)
	{
		ptOwnshipCtrl->bNewDap = LSA_FALSE;
		ptOwnshipCtrl->Status = PNIO_OK;
		ptOwnshipCtrl->nSubCnt = 0U;
		ptOwnshipCtrl->nSubMax = 0U;
	}
}

PNIO_VOID RZT2_PNS_OwnershipStart(PNIO_UINT32 DevHdl, PNIO_UINT32 ArNum, PNIO_UINT16 NumOfPluggedSub)
{
	TS_PNS_OWNERSHIP_CTRL_PTR ptOwnshipCtrl = (TS_PNS_OWNERSHIP_CTRL_PTR) RZT2_PNS_GetOwnershipCtrl(DevHdl);
	if(LSA_NULL != ptOwnshipCtrl)
	{
		if( (LSA_NULL != ptOwnshipCtrl->ptSubList) && (0U < ptOwnshipCtrl->nSubMax) )
		{
			OsFree(ptOwnshipCtrl->ptSubList);
			ptOwnshipCtrl->ptSubList = LSA_NULL;
		}

		ptOwnshipCtrl->bNewDap = LSA_FALSE;
		ptOwnshipCtrl->Status = PNIO_OK;
		ptOwnshipCtrl->nSubCnt = 0U;
		ptOwnshipCtrl->nSubMax = 0U;

		if(PNIO_OK == OsAlloc((void**)&ptOwnshipCtrl->ptSubList, 0, (NumOfPluggedSub * sizeof(TS_PNS_NV_SUBCFG))))
		{
			ptOwnshipCtrl->nSubMax = NumOfPluggedSub;
		}
	}
}

PNIO_VOID RZT2_PNS_OwnershipEnd(PNIO_UINT32 DevHdl, PNIO_UINT32 ArNum)
{
	TS_PNS_OWNERSHIP_CTRL_PTR ptOwnshipCtrl = (TS_PNS_OWNERSHIP_CTRL_PTR) RZT2_PNS_GetOwnershipCtrl(DevHdl);

	if( (LSA_NULL != ptOwnshipCtrl) && (LSA_NULL != ptOwnshipCtrl->ptSubList) &&
		( (2U + IOD_CFG_PDEV_NUMOF_PORTS) <= ptOwnshipCtrl->nSubCnt) &&
		(ptOwnshipCtrl->nSubCnt <= ptOwnshipCtrl->nSubMax) &&
		(PNIO_OK == ptOwnshipCtrl->Status) )
	{
		PNIO_UINT32 u32Size = (ptOwnshipCtrl->nSubCnt * sizeof(TS_PNS_NV_SUBCFG));
		Bsp_nv_data_store(PNIO_NVDATA_SUBCFG, (PNIO_VOID*)ptOwnshipCtrl->ptSubList, u32Size);
	}
}

// TS_PNS_NV_SUBCFG g_tSubCfg[IOD_CFG_NUMOF_SUBSLOTS];
PNIO_VOID RZT2_PNS_UpdateSubOwnership(PNIO_UINT32 DevHdl, PNIO_EXP_SUB *pExptSub, PNIO_UINT32 Status)
{
	TS_PNS_OWNERSHIP_CTRL_PTR ptOwnshipCtrl = (TS_PNS_OWNERSHIP_CTRL_PTR) RZT2_PNS_GetOwnershipCtrl(DevHdl);

	if( (LSA_NULL != ptOwnshipCtrl) && (LSA_NULL != ptOwnshipCtrl->ptSubList)  && (LSA_NULL != pExptSub) &&
		(0U < ptOwnshipCtrl->nSubMax) && (ptOwnshipCtrl->nSubCnt < ptOwnshipCtrl->nSubMax) )
	{
			ptOwnshipCtrl->ptSubList[ptOwnshipCtrl->nSubCnt].nApi   = pExptSub->ApiNum;
			ptOwnshipCtrl->ptSubList[ptOwnshipCtrl->nSubCnt].nSlot  = pExptSub->SlotNum;
			ptOwnshipCtrl->ptSubList[ptOwnshipCtrl->nSubCnt].nSslot = pExptSub->SubNum;
			ptOwnshipCtrl->ptSubList[ptOwnshipCtrl->nSubCnt].nModId = pExptSub->ModIdent;
			ptOwnshipCtrl->ptSubList[ptOwnshipCtrl->nSubCnt].nSubId = pExptSub->SubIdent;
			ptOwnshipCtrl->ptSubList[ptOwnshipCtrl->nSubCnt].nIn    = pExptSub->In.data_length;
			ptOwnshipCtrl->ptSubList[ptOwnshipCtrl->nSubCnt].nOut   = pExptSub->Out.data_length;
			if(PNIO_OK == ptOwnshipCtrl->Status)
			{
				ptOwnshipCtrl->Status = Status;
			}
		//	OsMemCpy(&g_tSubCfg[ptOwnshipCtrl->nSubCnt], &ptOwnshipCtrl->ptSubList[ptOwnshipCtrl->nSubCnt], sizeof(TS_PNS_NV_SUBCFG));
			ptOwnshipCtrl->nSubCnt++;
	}
}



PNIO_UINT32 RZT2_PNS_SubOwnershipInd(PNIO_UINT32 DevHdl, PNIO_UINT32 ArNum, PNIO_EXP_SUB *pExptSub)
{
	PNIO_UINT32 nStat = PNIO_ERR_PARAM;
	if( (0 < ArNum) && (NULL != pExptSub) )
	{
		TS_PNS_SUBCFG tSubCfg;
	//	TS_PNS_SUBPROP tSubProp;
		PNPB_REAL_SUB *ptRealSub = NULL;
		PNIO_DEV_ADDR tAddr;
		PNIO_UINT8  u8Im0Support = 0;
		PNIO_UINT16 ImXSupported = 0;
		IM0_DATA tIm0;

		//OsMemSet(&tSubProp, 0, sizeof(tSubProp));
		OsMemSet(&tSubCfg, 0, sizeof(tSubCfg));

		nStat = RZT2_PNS_CheckIfSubAllowed(pExptSub->ApiNum, pExptSub->SlotNum, pExptSub->SubNum, pExptSub->ModIdent, pExptSub->SubIdent);

		if(PNIO_OK == nStat)
		{
			nStat = RZT2_PNS_GetSubCfg(pExptSub->ApiNum, pExptSub->ModIdent, pExptSub->SubIdent, &tSubCfg);
		}

		if(PNIO_OK == nStat)
		{
			if( (pExptSub->In.data_length != tSubCfg.nInLen) || (pExptSub->Out.data_length != tSubCfg.nOutLen) )
			{
				switch(pExptSub->SubIdent)
				{
#ifdef PNS_SUBID_UNI_INPUT
				case PNS_SUBID_UNI_INPUT: // Universal Input
					 if( (tSubCfg.nInLen < pExptSub->In.data_length) || (0 < pExptSub->Out.data_length))
					 {
						 nStat = PNIO_ERR_IOD_INVALID_BUFLEN;
					 }
					break;
#endif
#ifdef PNS_SUBID_UNI_OUTPUT
				case PNS_SUBID_UNI_OUTPUT: // Universal Output
					if( (tSubCfg.nOutLen < pExptSub->Out.data_length)  || (0 < pExptSub->In.data_length))
					 {
						 nStat = PNIO_ERR_IOD_INVALID_BUFLEN;
					 }
					break;
#endif
#ifdef PNS_SUBID_UNI_IO
				case PNS_SUBID_UNI_IO: // Universal I/O
					if( (pExptSub->In.data_length > tSubCfg.nInLen) || (pExptSub->Out.data_length > tSubCfg.nOutLen) )
					{
						nStat = PNIO_ERR_IOD_INVALID_BUFLEN;
					}
					break;
#endif
				default:
					nStat = PNIO_ERR_IOD_INVALID_BUFLEN;
					break;
				}
			}
		}

		RZT2_PNS_UpdateSubOwnership(DevHdl, pExptSub, nStat);

		if(PNIO_OK == nStat)
		{
			tAddr.Geo.Slot    = pExptSub->SlotNum;        // slot
			tAddr.Geo.Subslot = pExptSub->SubNum;         // subslot

			ptRealSub = pnpb_sub_real_getp(pExptSub->ApiNum, pExptSub->SlotNum, pExptSub->SubNum);

			if(NULL != ptRealSub)
			{
				if(ptRealSub->ModIdent == pExptSub->ModIdent)
				{
					if(ptRealSub->SubIdent != pExptSub->SubIdent)
					{
						nStat = PNIO_sub_pull(PNIO_SINGLE_DEVICE_HNDL, pExptSub->ApiNum, &tAddr);
						if(PNIO_OK == nStat)
						{
							RZT2_PNS_ClearUsrRecAll(pExptSub->SlotNum, pExptSub->SubNum);
						}
					}
				}
				else
				{
					if( ( 1 < pExptSub->SlotNum) && ( 1 == pExptSub->SubNum))
					{
						int nCnt = 0;
						while( (PNIO_OK == nStat) && (nCnt < IOD_CFG_MAX_NUMOF_SUBSLOTS) )
						{
							 if( (PnpbMod.Sub[nCnt].PlugState == PNPB_PLUGGED) &&
								 (PnpbMod.Sub[nCnt].ApiNum == pExptSub->ApiNum) &&
								 (ptRealSub->ModIdent == PnpbMod.Sub[nCnt].ModIdent) )
							 {
								 tAddr.Geo.Slot    = PnpbMod.Sub[nCnt].SlotNum;        // slot
								 tAddr.Geo.Subslot = PnpbMod.Sub[nCnt].SubNum;         // subslot
								 nStat = PNIO_sub_pull(PNIO_SINGLE_DEVICE_HNDL, pExptSub->ApiNum, &tAddr);
							 }
							 nCnt++;
						}
					}
					else
					{
						nStat = PNIO_ERR_IOD_INVALID_SUBSLOT;
					}
				}

				ptRealSub = pnpb_sub_real_getp(pExptSub->ApiNum, pExptSub->SlotNum, pExptSub->SubNum);
			}
		}

		if( (PNIO_OK == nStat) && (NULL == ptRealSub) )
		{
			IM0_DATA * pIm0 = LSA_NULL;
			tAddr.Geo.Slot    = pExptSub->SlotNum;        // slot
			tAddr.Geo.Subslot = pExptSub->SubNum;         // subslot

			u8Im0Support = RZT2_PNS_GetIm0Support(pExptSub->ApiNum, pExptSub->SlotNum, pExptSub->SubNum, pExptSub->ModIdent, pExptSub->SubIdent);

			if( (0U < u8Im0Support) &&
				(PNIO_OK == RZT2_PNS_BuildIm0(pExptSub->ApiNum, pExptSub->SlotNum, pExptSub->SubNum, pExptSub->ModIdent, pExptSub->SubIdent, &tIm0)) )
			{
				pIm0 = &tIm0;
			}

			if(PNIO_OK == PNIO_sub_plug(  PNIO_SINGLE_DEVICE_HNDL,
										  pExptSub->ApiNum, &tAddr,
										  pExptSub->ModIdent,
										  pExptSub->SubIdent,
										  pExptSub->In.data_length,
										  pExptSub->Out.data_length,
										  u8Im0Support,
										  pIm0, PNIO_S_GOOD) )
			{
			//	RZT2_PNS_AddSub();
			}
		}

		if(PNIO_OK == nStat)
		{
			pExptSub->IsWrongSubmod = PNIO_FALSE;
		}
		else
		{
			pExptSub->IsWrongSubmod = PNIO_TRUE;
		}

	}

	return nStat;
}



PNIO_VOID *RZT2_PNS_GetUsrRecHandle(PNIO_UINT16 nSlot, PNIO_UINT16 nSubslot, PNIO_UINT16 RecordIndex)
{
	TS_PNS_USR_REC_PTR ptRec = LSA_NULL;
	PNIO_UINT16 nCnt = 0U;

	while( (LSA_NULL == ptRec) && (nCnt < IOD_CFG_NUMOF_SUBSLOTS) )
	{
		if( (nSlot == g_tUsrRecList[nCnt].nSlot) &&
			(nSubslot == g_tUsrRecList[nCnt].nSubslot) &&
			(RecordIndex == g_tUsrRecList[nCnt].nIndex) )
		{
			ptRec = &g_tUsrRecList[nCnt];
		}
		nCnt++;
	}

	return ptRec;
}


PNIO_VOID *RZT2_PNS_AllocUsrRec(PNIO_UINT16 nSlot, PNIO_UINT16 nSubslot, PNIO_UINT16 RecordIndex)
{
	PNIO_UINT16 nCnt = 0U;
	TS_PNS_USR_REC_PTR ptRec = RZT2_PNS_GetUsrRecHandle(nSlot, nSubslot, RecordIndex);

	while( (LSA_NULL == ptRec) && (nCnt < IOD_CFG_NUMOF_SUBSLOTS) )
	{
		if( (0 == g_tUsrRecList[nCnt].nSlot) && (0 == g_tUsrRecList[nCnt].nSubslot) )
		{
			ptRec = &g_tUsrRecList[nCnt];
			ptRec->nSlot = nSlot;
			ptRec->nSubslot = nSubslot;
			ptRec->nIndex = RecordIndex;
		}
		nCnt++;
	}

	return ptRec;
}

PNIO_VOID RZT2_PNS_ClearUsrRecAll(PNIO_UINT16 nSlot, PNIO_UINT16 nSubslot)
{
	PNIO_UINT16 nCnt = 0U;

	while( (nCnt < IOD_CFG_NUMOF_SUBSLOTS) )
	{
		if( (nSlot == g_tUsrRecList[nCnt].nSlot) &&
			(nSubslot == g_tUsrRecList[nCnt].nSubslot) )
		{
			OsMemSet(&g_tUsrRecList[nCnt], 0, sizeof(TS_PNS_USR_REC));
		}
		nCnt++;
	}
}
LSA_BOOL g_RecWrite = LSA_FALSE;
PNIO_UINT32  RZT2_PNS_RecordWrite
(
	PNIO_UINT32			DevHndl,        // device handle
	PNIO_UINT32			Api,            // application process identifier
    PNIO_UINT16         ArNum,          // ar - number
    PNIO_UINT16 		SessionKey,	    // ar session number
	PNIO_UINT32			SequenceNum,    // CLRPC sequence number
	PNIO_DEV_ADDR		*pAddr,			// geographical or logical address
	PNIO_UINT32			RecordIndex,    // record index
	PNIO_UINT32			*pBufLen,   	// [in, out] in: length to write, out: length, written by user
	PNIO_UINT8			*pBuffer,		// [in] buffer pointer
	PNIO_ERR_STAT		*pPnioState		// 4 byte PNIOStatus (ErrCode, ErrDecode, ErrCode1, ErrCode2), see IEC61158-6
)
{
	PNIO_UINT32 nStat = PNIO_OK;
	PNIO_UINT8  ErrCode1  = 0U;



	if( (LSA_NULL != pAddr) && (0U < pAddr->ad.Geo1.Slot) &&
		(LSA_NULL != pnpb_sub_real_getp(Api, pAddr->ad.Geo1.Slot, pAddr->ad.Geo1.Subslot)) )
	{
		if( (NULL != pBuffer) && (NULL != pBufLen) && (0U < *((PNIO_UINT32*)pBufLen)) )
		{
			TS_PNS_USR_REC_PTR ptRec =(TS_PNS_USR_REC_PTR) RZT2_PNS_AllocUsrRec(pAddr->ad.Geo1.Slot, pAddr->ad.Geo1.Subslot, RecordIndex);
			if(LSA_NULL != ptRec)
			{
				switch(RecordIndex)
				{
				case 4096U:
					if(1U == pAddr->ad.Geo1.Subslot)
					{
						if(LSA_FALSE == g_RecWrite)
						{
							g_RecWrite = LSA_TRUE;
						}

						if( PNS_USR_MOD_REC_LEN <= *((PNIO_UINT32*)pBufLen) )
						{
							ptRec->nDataSize = (PNIO_UINT16) (*((PNIO_UINT32*)pBufLen));
							OsMemSet(&ptRec->abData[0], 0, sizeof(ptRec->abData));
							OsMemCpy(&ptRec->abData[0], pBuffer, ptRec->nDataSize);
						}
						else
						{
							ErrCode1  = 0xB1U;	/*!< Error Class: Access, Error Code: write length error */
						}
					}
					else
					{
						ErrCode1  = 0xB2U;	/*!< Error Class: Access, Error Code: invalid slot */
					}
					break;
				case 0x8030U:
					if(1U < pAddr->ad.Geo1.Subslot)
					{
						if( sizeof(REC8030_ISOMDATA) <= (*((PNIO_UINT32*)pBufLen)) )
						{
							REC8030_ISOMDATA tPrm;
							REC8030_ISOMDATA *ptIso  = (REC8030_ISOMDATA*)pBuffer;

							tPrm.BlockHeader.Type    = (PNIO_UINT16) OsNtohs(ptIso->BlockHeader.Type);
							tPrm.BlockHeader.Len     = (PNIO_UINT16) OsNtohs(ptIso->BlockHeader.Len);
							tPrm.BlockHeader.Version = (PNIO_UINT16) OsNtohs(ptIso->BlockHeader.Version);

							if( (ISO_HDR_TYPE == tPrm.BlockHeader.Type) &&
								(ISO_HDR_VERSION == tPrm.BlockHeader.Version) &&
								(tPrm.BlockHeader.Len == (sizeof(REC8030_ISOMDATA) - sizeof(REC_IO_BLOCKHDR)) ) )
							{
								PNIO_UINT32 u32TimeCycle = (PNIO_UINT32)(OsNtohs(ptIso->TimeDataCycle) * 31250U);
								tPrm.SlotNumber			 = (PNIO_UINT16) OsNtohs(ptIso->SlotNumber);
								tPrm.SubSlotNumber		 = (PNIO_UINT16) OsNtohs(ptIso->SlotNumber);
								tPrm.TimeDataCycle       = (PNIO_UINT16) OsNtohs(ptIso->TimeDataCycle);
								tPrm.TimeIOInput         = (PNIO_UINT32) OsNtohl(ptIso->TimeIOInput);
								tPrm.TimeIOOutput        = (PNIO_UINT32) OsNtohl(ptIso->TimeIOOutput);
								tPrm.TimeIOInputValid    = (PNIO_UINT32) OsNtohl(ptIso->TimeIOInputValid);
								tPrm.TimeIOOutputValid   = (PNIO_UINT32) OsNtohl(ptIso->TimeIOOutputValid);
								tPrm.ControllerApplicationCycleFactor = (PNIO_UINT16) OsNtohs(ptIso->ControllerApplicationCycleFactor);

								if( (0U < tPrm.ControllerApplicationCycleFactor) &&
									(0U < tPrm.TimeDataCycle) &&
									(0U < tPrm.TimeIOInput)   &&
									(0U < tPrm.TimeIOOutput)  &&
									(ISO_T_DC_MIN       <= u32TimeCycle)  &&
									(ISO_T_DC_MAX       >= u32TimeCycle)  &&
									(ISO_T_IO_INPUTMIN  <= tPrm.TimeIOInput)  &&
									(ISO_T_DC_MAX       >= tPrm.TimeIOInput)  &&
									(ISO_T_IO_OUTPUTMIN <= tPrm.TimeIOOutput) &&
									(ISO_T_DC_MAX       >= tPrm.TimeIOOutput) &&
									(tPrm.TimeIOInput  <= u32TimeCycle) &&
									(tPrm.TimeIOOutput <= u32TimeCycle) )
								{
									ptRec->nDataSize = sizeof(REC8030_ISOMDATA);
									OsMemSet(&ptRec->abData[0], 0, sizeof(ptRec->abData));
									OsMemCpy(&ptRec->abData[0], &tPrm, ptRec->nDataSize);
								}
								else
								{
									ErrCode1  = 0xB8U;	/*!< Error Class: Access, Error Code: invalid parameter */
								}
							}
							else
							{
								ErrCode1  = 0xB9U;	/*!< Error Class: Access, Error Code: invalid type */
							}
						}
						else
						{
							ErrCode1  = 0xB1U;	/*!< Error Class: Access, Error Code: write length error */
						}
					}
					else
					{
						ErrCode1  = 0xB2U;	/*!< Error Class: Access, Error Code: invalid slot */
					}
					break;
				default:
					ErrCode1  = 0xB0U;  // invalid Index
					break;
				}
			}
			else
			{
				ErrCode1 = 0xC0U;	/*!< Error Class: Resource, Error Code: read constraint conflict */
			}
		}
		else
		{
			ErrCode1  = 0xB1U;	/*!< Error Class: Access, Error Code: write length error */
		}
	}
	else
	{
		ErrCode1  = 0xB2U;	/*!< Error Class: Access, Error Code: invalid slot */
	}

	if(0U < ErrCode1)
	{
		if(LSA_NULL != pPnioState)
		{
			pPnioState->ErrCode   = 0xDF; // IODReadRes with ErrorDecode = PNIORW
			pPnioState->ErrDecode = 0x80; // PNIORW
			pPnioState->ErrCode1  = ErrCode1;
		}
		nStat = PNIO_NOT_OK;
	}

	return nStat;
}

PNIO_UINT32  RZT2_PNS_RecordRead
(
	PNIO_UINT32			DevHndl,        // device handle
	PNIO_UINT32			Api,            // application process identifier
    PNIO_UINT16         ArNum,          // ar - number
    PNIO_UINT16 		SessionKey,	    // ar session number
	PNIO_UINT32			SequenceNum,    // CLRPC sequence number
	PNIO_DEV_ADDR		*pAddr,			// geographical or logical address
	PNIO_UINT32			RecordIndex,    // record index
	PNIO_UINT32			*pBufLen,   	// [in, out] in: length to write, out: length, written by user
	PNIO_UINT8			*pBuffer,		// [in] buffer pointer
	PNIO_ERR_STAT		*pPnioState		// 4 byte PNIOStatus (ErrCode, ErrDecode, ErrCode1, ErrCode2), see IEC61158-6
)
{
	PNIO_UINT32 nStat = PNIO_OK;
	PNIO_UINT8  ErrCode1  = 0U;

	if( (LSA_NULL != pAddr) && (0U < pAddr->ad.Geo1.Slot) &&
		(LSA_NULL != pnpb_sub_real_getp(Api, pAddr->ad.Geo1.Slot, pAddr->ad.Geo1.Subslot)) )
	{
		if( (NULL != pBuffer) && (NULL != pBufLen) && (0U < *((PNIO_UINT32*)pBufLen)) )
		{
			TS_PNS_USR_REC_PTR ptRec =(TS_PNS_USR_REC_PTR) RZT2_PNS_GetUsrRecHandle(pAddr->ad.Geo1.Slot, pAddr->ad.Geo1.Subslot, RecordIndex);

			switch(RecordIndex)
			{
			case 4096U:
				if(1U == pAddr->ad.Geo1.Subslot)
				{
					if( PNS_USR_MOD_REC_LEN <= *((PNIO_UINT32*)pBufLen) )
					{
						if( (LSA_NULL != ptRec) && (0U < ptRec->nDataSize))
						{
							OsMemCpy(pBuffer, &ptRec->abData[0], ptRec->nDataSize);
							*((PNIO_UINT32*)pBufLen) = ptRec->nDataSize;
						}
						else
						{
							/* Fill buffer with default values!!!!! */
							OsMemSet(pBuffer, 0, PNS_USR_MOD_REC_LEN);
							*((PNIO_UINT32*)pBufLen) = PNS_USR_MOD_REC_LEN;
						}
					}
					else
					{
						ErrCode1  = 0xC0U;	/*!< Error Class: Resource, Error Code: read constraint conflict */
					}
				}
				else
				{
					ErrCode1  = 0xB2U;	/*!< Error Class: Access, Error Code: invalid slot */
				}
				break;
			case 0x8030U:
				if(1U < pAddr->ad.Geo1.Subslot)
				{
					if( sizeof(REC8030_ISOMDATA) <= (*((PNIO_UINT32*)pBufLen)) )
					{
						REC8030_ISOMDATA *ptIso  = (REC8030_ISOMDATA*)pBuffer;
						*((PNIO_UINT32*)pBufLen) = sizeof(REC8030_ISOMDATA);

						if( (LSA_NULL != ptRec) && (0U < ptRec->nDataSize))
						{
							REC8030_ISOMDATA *ptPrm    = (REC8030_ISOMDATA*)&ptRec->abData[0U];
							ptIso->BlockHeader.Type    = (PNIO_UINT16) OsHtons(ptPrm->BlockHeader.Type);
							ptIso->BlockHeader.Len     = (PNIO_UINT16) OsHtons(ptPrm->BlockHeader.Len);
							ptIso->BlockHeader.Version = (PNIO_UINT16) OsHtons(ptPrm->BlockHeader.Version);
							ptIso->SlotNumber		   = (PNIO_UINT16) OsHtons(ptPrm->SlotNumber);
							ptIso->SubSlotNumber	   = (PNIO_UINT16) OsHtons(ptPrm->SlotNumber);
							ptIso->TimeDataCycle       = (PNIO_UINT16) OsHtons(ptPrm->TimeDataCycle);
							ptIso->TimeIOInput         = (PNIO_UINT32) OsNtohl(ptPrm->TimeIOInput);
							ptIso->TimeIOOutput        = (PNIO_UINT32) OsNtohl(ptPrm->TimeIOOutput);
							ptIso->TimeIOInputValid    = (PNIO_UINT32) OsNtohl(ptPrm->TimeIOInputValid);
							ptIso->TimeIOOutputValid   = (PNIO_UINT32) OsNtohl(ptPrm->TimeIOOutputValid);
							ptIso->ControllerApplicationCycleFactor = (PNIO_UINT16) OsHtons(ptPrm->ControllerApplicationCycleFactor);
						}
						else
						{
							OsMemSet(pBuffer, 0, sizeof(REC8030_ISOMDATA));
							ptIso->BlockHeader.Type    = (PNIO_UINT16) OsHtons(ISO_HDR_TYPE);
							ptIso->BlockHeader.Version = (PNIO_UINT16) OsHtons(ISO_HDR_VERSION);
							ptIso->BlockHeader.Len     = 0U;
						}

					}
					else
					{
						ErrCode1  = 0xB1U;	/*!< Error Class: Access, Error Code: write length error */
					}
				}
				else
				{
					ErrCode1  = 0xB2U;	/*!< Error Class: Access, Error Code: invalid slot */
				}
			break;
			default:
				ErrCode1  = 0xB0U;  // invalid Index
				break;
			}
		}
		else
		{
			ErrCode1  = 0xA0U;	/*!< Error Class: Application, Error Code: read error */
		}
	}
	else
	{
		ErrCode1  = 0xB2U;	/*!< Error Class: Access, Error Code: invalid slot */
	}

	if(0U < ErrCode1)
	{
		if(LSA_NULL != pPnioState)
		{
			pPnioState->ErrCode   = 0xDE; // IODReadRes with ErrorDecode = PNIORW
			pPnioState->ErrDecode = 0x80; // PNIORW
			pPnioState->ErrCode1  = ErrCode1;
		}
		nStat = PNIO_NOT_OK;
	}
	return nStat;
}

PNIO_UINT32 RZT2_PNS_GetDefaultSubCfg(TS_PNS_NV_SUBCFG_PTR ptSubCfgList, PNIO_UINT16 *pItems)
{
	PNIO_UINT32 Status = PNIO_OK;
	PNIO_UINT16 Items = *((PNIO_UINT16*)pItems);
	if( (LSA_NULL != ptSubCfgList) && (LSA_NULL != pItems) && (3U <= Items) )
	{
		PNIO_UINT8 nCnt = 0;
		TS_PNS_NV_SUBCFG_PTR ptList = ptSubCfgList;

		ptList[nCnt].nApi   = 0;
		ptList[nCnt].nSlot  = 0;
		ptList[nCnt].nSslot = 1;
		ptList[nCnt].nModId = 0x00030100;
		ptList[nCnt].nSubId = 0x00030101;
		ptList[nCnt].nIn    = 0;
		ptList[nCnt].nOut   = 0;
		nCnt++;

		ptList[nCnt].nApi   = 0;
		ptList[nCnt].nSlot  = 0;
		ptList[nCnt].nSslot = 0x8000;
		ptList[nCnt].nModId = 0x00030100;
		ptList[nCnt].nSubId = 0x00038000;
		ptList[nCnt].nIn    = 0;
		ptList[nCnt].nOut   = 0;
		nCnt++;

		ptList[nCnt].nApi   = 0;
		ptList[nCnt].nSlot  = 0;
		ptList[nCnt].nSslot = 0x8001;
		ptList[nCnt].nModId = 0x00030100;
		ptList[nCnt].nSubId = 0x00038001;
		ptList[nCnt].nIn    = 0;
		ptList[nCnt].nOut   = 0;
		nCnt++;

#if (IOD_CFG_PDEV_NUMOF_PORTS >= 2)
		if(nCnt < Items)
		{
			ptList[nCnt].nApi   = 0;
			ptList[nCnt].nSlot  = 0;
			ptList[nCnt].nSslot = 0x8002;
			ptList[nCnt].nModId = 0x00030100;
			ptList[nCnt].nSubId = 0x00038002;
			ptList[nCnt].nIn    = 0;
			ptList[nCnt].nOut   = 0;
			nCnt++;
		}
#endif

#if (IOD_CFG_PDEV_NUMOF_PORTS >= 3)
		if(nCnt < Items)
		{
			ptList[nCnt].nApi   = 0;
			ptList[nCnt].nSlot  = 0;
			ptList[nCnt].nSslot = 0x8003;
			ptList[nCnt].nModId = 0x00030100;
			ptList[nCnt].nSubId = 0x00038003;
			ptList[nCnt].nIn    = 0;
			ptList[nCnt].nOut   = 0;
			nCnt++;
		}
#endif
#if (IOD_CFG_PDEV_NUMOF_PORTS >= 4)
		if(nCnt < Items)
		{
			ptList[nCnt].nApi   = 0;
			ptList[nCnt].nSlot  = 0;
			ptList[nCnt].nSslot = 0x8004;
			ptList[nCnt].nModId = 0x00030100;
			ptList[nCnt].nSubId = 0x00038004;
			ptList[nCnt].nIn    = 0;
			ptList[nCnt].nOut   = 0;
			nCnt++;
		}
#endif
		if(nCnt < Items)
		{
			ptList[nCnt].nApi   = 0;
			ptList[nCnt].nSlot  = 1;
			ptList[nCnt].nSslot = 1;
			ptList[nCnt].nModId = 0x10010000;
			ptList[nCnt].nSubId = 0x1001FFFF;
			ptList[nCnt].nIn    = 0;
			ptList[nCnt].nOut   = 0;
			nCnt++;
		}

		if(nCnt < Items)
		{
			ptList[nCnt].nApi   = 0;
			ptList[nCnt].nSlot  = 1;
			ptList[nCnt].nSslot = 2;
			ptList[nCnt].nModId = 0x10010000;
			ptList[nCnt].nSubId = 0x00010001;
			ptList[nCnt].nIn    = 1;
			ptList[nCnt].nOut   = 0;
			nCnt++;
		}
		if(nCnt < Items)
		{
			ptList[nCnt].nApi   = 0;
			ptList[nCnt].nSlot  = 2;
			ptList[nCnt].nSslot = 1;
			ptList[nCnt].nModId = 0x10020000;
			ptList[nCnt].nSubId = 0x1002FFFF;
			ptList[nCnt].nIn    = 0;
			ptList[nCnt].nOut   = 0;
			nCnt++;
		}

		if(nCnt < Items)
		{
			ptList[nCnt].nApi   = 0;
			ptList[nCnt].nSlot  = 2;
			ptList[nCnt].nSslot = 2;
			ptList[nCnt].nModId = 0x10020000;
			ptList[nCnt].nSubId = 0x00020001;
			ptList[nCnt].nIn    = 0;
			ptList[nCnt].nOut   = 1;
			nCnt++;
		}

		*((PNIO_UINT16*)pItems) = nCnt;
	}
	else
	{
		Status = PNIO_NOT_OK;
	}

	return Status;

}

void RZT2_PNS_Init(PNIO_UINT32 DevHdl)
{
	OsMemSet(&g_tPnsArList[0],  0, sizeof(g_tPnsArList));
	OsMemSet(&g_tUsrRecList[0], 0, sizeof(g_tUsrRecList));
	RZT2_PNS_OwnershipInit(DevHdl);
}

#if 0
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
void MainAppl (void)
{
	PNUSR_DEVICE_INSTANCE PnUsrDev;
	PNIO_UINT32 Status = PNIO_OK;
	OS_MSG_PTR_TYPE ptMsg = NULL;
	TS_PNS_NV_SUBCFG_PTR ptSubCfgList = LSA_NULL;
	PNIO_SUB_LIST_ENTRY *pSubList = LSA_NULL;
	PNIO_IM0_LIST_ENTRY *pIm0List = LSA_NULL;
	TS_PNS_NV_SUBCFG_PTR ptSubCfg = LSA_NULL;
	PNIO_UINT16 MaxNumOfSubslots = 0U;
	PNIO_UINT32 u32Size = 0U;
	PNIO_UINT16 SubCnt = 0U;
	PNIO_UINT16 Im0Cnt = 0U;
	PNIO_UINT16 nCnt = 0U;

	PNIO_init();

	RZT2_PNS_Init(PNIO_SINGLE_DEVICE_HNDL);

//	calc_chksum();

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
	PnUsrDev.VendorId            = IOD_CFG_VENDOR_ID;                       // Vendor ID, requested from PROFIBUS/PROFINET organization (PI)
	PnUsrDev.DeviceId            = IOD_CFG_DEVICE_ID;                       // Device ID, must be unique inside one Vendor ID
	PnUsrDev.MaxNumOfSubslots    = IOD_CFG_NUMOF_SUBSLOTS;          // maximum number of subslots per slot, managable by PN Stack
	PnUsrDev.pDevType            = IOD_CFG_DEVICE_TYPE;             // see also GSDML file, product family

	MaxNumOfSubslots = IOD_CFG_MAX_NUMOF_SUBSLOTS;

	Status = Bsp_nv_data_restore(PNIO_NVDATA_SUBCFG, (PNIO_VOID**)&ptSubCfgList, &u32Size);

	if( (PNIO_OK == Status) &&  (LSA_NULL != ptSubCfgList) &&
		(sizeof(TS_PNS_NV_SUBCFG) <= u32Size) &&
		( 0U == (u32Size % sizeof(TS_PNS_NV_SUBCFG))) &&
		( (2U + IOD_CFG_PDEV_NUMOF_PORTS) <= (u32Size / sizeof(TS_PNS_NV_SUBCFG))) )
	{
		MaxNumOfSubslots = (u32Size / sizeof(TS_PNS_NV_SUBCFG));
		while( (PNIO_OK == Status) && (nCnt < MaxNumOfSubslots) )
		{
			ptSubCfg = &ptSubCfgList[nCnt++];
			if( (0U < ptSubCfg->nSslot) && (0U < ptSubCfg->nModId) && (0U < ptSubCfg->nSubId) )
			{
				Status = RZT2_PNS_CheckIfSubAllowed(ptSubCfg->nApi, ptSubCfg->nSlot, ptSubCfg->nSslot, ptSubCfg->nModId, ptSubCfg->nSubId);
			}
		}
	}
	else
	{
		Status = PNIO_NOT_OK;
	}

	if( (PNIO_OK != Status) || (LSA_NULL == ptSubCfgList) )
	{
		if(LSA_NULL != ptSubCfgList)
		{
			OsFree(ptSubCfgList);
			ptSubCfgList = LSA_NULL;
		}

		MaxNumOfSubslots = IOD_CFG_MAX_NUMOF_SUBSLOTS;
		u32Size = (MaxNumOfSubslots * sizeof(TS_PNS_NV_SUBCFG));
		Status = OsAlloc((void**)&ptSubCfgList, 0, u32Size);

		if(PNIO_OK == Status)
		{
			Status = RZT2_PNS_GetDefaultSubCfg(ptSubCfgList, &MaxNumOfSubslots);
		}
	}

	if(PNIO_OK == Status)
	{
		Status = OsAlloc((void**)&pSubList , 0, (MaxNumOfSubslots * sizeof(PNIO_SUB_LIST_ENTRY)));
		if(LSA_NULL != pSubList)
		{
			OsAlloc((void**)&pIm0List, 0, (MaxNumOfSubslots * sizeof(PNIO_IM0_LIST_ENTRY)));
		}
		else
		{
			Status = PNIO_NOT_OK;
		}
	}

	if(PNIO_OK == Status)
	{
		PNIO_UINT8 u8Im0Support = 0U;
		nCnt = 0U;

		while(nCnt < MaxNumOfSubslots)
		{
			Status = PNIO_NOT_OK;
			u8Im0Support = 0U;
			ptSubCfg = &ptSubCfgList[nCnt++];
			if( (0U < ptSubCfg->nSslot) && (0U < ptSubCfg->nModId) && (0U < ptSubCfg->nSubId) )
			{
				Status = RZT2_PNS_CheckIfSubAllowed(ptSubCfg->nApi, ptSubCfg->nSlot, ptSubCfg->nSslot, ptSubCfg->nModId, ptSubCfg->nSubId);
			}

			if( (PNIO_OK == Status) && (LSA_NULL != pIm0List) )
			{
				u8Im0Support = RZT2_PNS_GetIm0Support(ptSubCfg->nApi, ptSubCfg->nSlot, ptSubCfg->nSslot, ptSubCfg->nModId, ptSubCfg->nSubId);
				if( (0U < u8Im0Support) &&
					(PNIO_OK == RZT2_PNS_BuildIm0(ptSubCfg->nApi, ptSubCfg->nSlot, ptSubCfg->nSslot, ptSubCfg->nModId, ptSubCfg->nSubId, &pIm0List[Im0Cnt].Im0Dat)) )
				{
					pIm0List[Im0Cnt].Api = ptSubCfg->nApi;
					pIm0List[Im0Cnt].Slot = ptSubCfg->nSlot;
					pIm0List[Im0Cnt].Subslot = ptSubCfg->nSslot;
					Im0Cnt++;
				}
			}

			if(PNIO_OK == Status)
			{
				pSubList[SubCnt].Api = ptSubCfg->nApi;
				pSubList[SubCnt].Slot = ptSubCfg->nSlot;
				pSubList[SubCnt].Subslot = ptSubCfg->nSslot;
				pSubList[SubCnt].ModId = ptSubCfg->nModId;
				pSubList[SubCnt].SubId = ptSubCfg->nSubId;
				pSubList[SubCnt].InDatLen = ptSubCfg->nIn;
				pSubList[SubCnt].OutDatLen = ptSubCfg->nOut;
				pSubList[SubCnt].Im0Support = u8Im0Support;
				SubCnt++;
			}
		}

	}

   // **** startup the PN stack ****
   // *-----------------------------------------------------------
   // *     setup device stack and plug all io modules
   // *     (number of plugged modules <= IOD_CFG_NUMOF_SUBSLOTS)
   // *-----------------------------------------------------------
   //lint -e{866} Unusual use of '+' in argument to sizeof
	if(PNIO_OK == Status)
	{
		Status = PnUsr_DeviceSetup (&PnUsrDev,                                  // device specific configuration
								   pSubList,                              // list of plugged submodules (real configuration), including PDEV
								   SubCnt, // number of entries in IoSubList
								   pIm0List,                                // list of IM0 data sets
								   Im0Cnt);     // number of entries in Im0List
	}
	// *----------------------------------------------------------
	// * print startup result message
	// *----------------------------------------------------------
   if (Status == PNIO_OK)
   {
	   PNIO_ConsolePrintf ("SYSTEM STARTUP FINISHED, OK\n");
	   Bsp_EbSetLed(BSP_LED_MOD_RUN, 1);
	   Bsp_EbSetLed(BSP_LED_MOD_ERR, 0);
   }
   else
   {
	   PNIO_ConsolePrintf ("##ERROR AT SYSTEM STARTUP\n");
	   Bsp_EbSetLed(BSP_LED_MOD_ERR, 1);
   }

	// *----------------------------------------------------------
	// *endless loop: wait on key pressed
	// *----------------------------------------------------------


	if(LSA_NULL != pSubList)
	{
		OsFree(pSubList);
	}

	if(LSA_NULL != pIm0List)
	{
		OsFree(pIm0List);
	}

	if(LSA_NULL != ptSubCfgList)
	{
		OsFree(ptSubCfgList);
	}

	while(1)
	{
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
                /* SW1 Read */
#ifdef _RENESAS_RZT_
            uint8_t u8SW1 = R_BSP_PinRead(BSP_IO_PORT_10_PIN_5);
#endif
#ifdef _RENESAS_RZN_
            uint8_t u8SW1 = R_BSP_PinRead(BSP_IO_PORT_16_PIN_3);
#endif
                /* SW2 Read */
#ifdef _RENESAS_RZT_
            uint8_t u8SW2 = R_BSP_PinRead(BSP_IO_PORT_16_PIN_3);
#endif
#ifdef _RENESAS_RZN_
            uint8_t u8SW2 = R_BSP_PinRead(BSP_IO_PORT_05_PIN_4);
#endif
            /* Set Input1Byte module data */
            *((uint8_t*)&InData[1][2][0]) = (uint8_t)(0xA0 | u8SW2 << 1 | u8SW1 << 0);
		}
		OsWait_ms(100);
	}
}
#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
