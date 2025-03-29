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

#ifndef __RZT2_PNS_RSK_H__                  /* ----- reinclude-protection ----- */
#define __RZT2_PNS_RSK_H__

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif






extern PNPB_REAL PnpbMod;
extern PNIO_UINT32 PnUsr_get_subslot_index(PNIO_UINT32 slot, PNIO_UINT32 subslot);


extern PNIO_UINT8     OutData		  [NUMOF_SLOTS][NUMOF_SUBSLOTS+1][NUMOF_BYTES_PER_SUBSLOT];
#if !IOD_INCLUDE_REC8028_8029
extern PNIO_UINT8     OutSubstVal	  [NUMOF_SLOTS][NUMOF_SUBSLOTS+1][NUMOF_BYTES_PER_SUBSLOT];
#endif
extern PNIO_UINT16    OutDatLen	  [NUMOF_SLOTS][NUMOF_SUBSLOTS+1];
extern PNIO_UINT8     OutDatIocs	  [NUMOF_SLOTS][NUMOF_SUBSLOTS+1];
extern PNIO_UINT8     OutDatIops	  [NUMOF_SLOTS][NUMOF_SUBSLOTS+1];
//extern PNIO_UINT8     OutDatIops_old [NUMOF_SLOTS][NUMOF_SUBSLOTS+1];

// *** Input Data  (IO Device ==> IO Controller)
extern PNIO_UINT8     InData		  [NUMOF_SLOTS][NUMOF_SUBSLOTS+1][NUMOF_BYTES_PER_SUBSLOT];
extern PNIO_UINT16    InDatLen		  [NUMOF_SLOTS][NUMOF_SUBSLOTS+1];
extern PNIO_UINT8     InDatIops	  [NUMOF_SLOTS][NUMOF_SUBSLOTS+1];
extern PNIO_UINT8     InDatIocs	  [NUMOF_SLOTS][NUMOF_SUBSLOTS+1];
// extern PNIO_UINT8     InDatIocs_old  [NUMOF_SLOTS][NUMOF_SUBSLOTS+1];

#define RZT2_PNS_MAX_SLOT	IOD_CFG_MAX_SLOT_NUMBER
#define RZT2_PNS_MAX_SSLOT	IOD_CFG_MAX_NUMOF_SUBSL_PER_SLOT
#define RZT2_PNS_MAX_MOD	4

#define PNS_SUBID_UNI_INPUT		0x00010040
#define PNS_SUBID_UNI_OUTPUT	0x00020040
#define PNS_SUBID_UNI_IO		0x00030040

typedef struct _s_pns_slotlist
{
	int nCnt;
	PNIO_UINT16 nSlots[RZT2_PNS_MAX_SLOT];
}TS_PNS_SLOTLIST, *TS_PNS_SLOTLIST_PTR;

typedef struct _s_pns_sslotlist
{
	int nCnt;
	PNIO_UINT16 nSslots[RZT2_PNS_MAX_SSLOT];
}TS_PNS_SSLOTLIST, *TS_PNS_SSLOTLIST_PTR;

typedef struct _s_pns_modlist
{
	int nCnt;
	PNIO_UINT32 nModIds[RZT2_PNS_MAX_MOD];
}TS_PNS_MODLIST, *TS_PNS_MODLIST_PTR;

typedef struct _s_pns_modprop
{
	PNIO_UINT32 nModId;
	TS_PNS_SLOTLIST  tSlotList;
	TS_PNS_SSLOTLIST tSslotList;
}TS_PNS_MODPROP, *TS_PNS_MODPROP_PTR;

typedef struct _s_pns_subcfg
{
	PNIO_UINT32 nApi;
	PNIO_UINT32 nSubId;
	PNIO_UINT16 nInLen;
	PNIO_UINT16 nOutLen;
	TS_PNS_MODLIST tModList;
}TS_PNS_SUBCFG, *TS_PNS_SUBCFG_PTR;

typedef struct _s_pns_iodata
{
	uint8_t  u8Iocs;
	uint8_t  u8Iops;
	PNIO_UINT16 nDataSize;
	void*    pData;
}TS_PNS_IODATA, *TS_PNS_IODATA_PTR;


typedef struct _s_pns_subprop
{
	PNIO_UINT32 nApi;
	PNIO_UINT16 nSlot;
	PNIO_UINT16 nSslot;
	PNIO_UINT32 nModId;
	PNIO_UINT32 nSubId;
	TS_PNS_IODATA tIn;
	TS_PNS_IODATA tOut;
}TS_PNS_SUBPROP, *TS_PNS_SUBPROP_PTR;

typedef struct _s_pns_ownership_ctrl
{
	PNIO_UINT8           bNewDap;
	PNIO_UINT32          Status;
	PNIO_UINT16          nSubMax;
	PNIO_UINT16          nSubCnt;
	TS_PNS_NV_SUBCFG_PTR ptSubList;
}TS_PNS_OWNERSHIP_CTRL, *TS_PNS_OWNERSHIP_CTRL_PTR;

#define PNS_USR_MOD_REC_LEN	16
#define PNS_USR_MAX_REC_LEN	40
typedef struct _s_pns_usr_record
{
	PNIO_UINT16 nIndex;
	PNIO_UINT16 nSlot;
	PNIO_UINT16 nSubslot;
	PNIO_UINT16 nDataSize;
	PNIO_UINT8  abData[PNS_USR_MAX_REC_LEN];
}TS_PNS_USR_REC, *TS_PNS_USR_REC_PTR;

#define GSD_T_DC_Base	1U
#define GSD_T_DC_Min	4U
#define GSD_T_DC_Max	1024U
#define GSD_T_IO_Base	125000U
#define GSD_T_IO_InputMin	1U
#define GSD_T_IO_OuputMin	1U

#define ISO_HDR_VERSION	0x0100U
#define ISO_HDR_TYPE	0x0204U
#define ISO_T_DC_BASE 		(GSD_T_DC_Base * 31250U)
#define ISO_T_DC_MIN  		(GSD_T_DC_Min * (GSD_T_DC_Base * 31250U))
#define ISO_T_DC_MAX		(GSD_T_DC_Max * (GSD_T_DC_Base * 31250U))
#define ISO_T_IO_BASE 		(GSD_T_IO_Base)
#define ISO_T_IO_INPUTMIN	(GSD_T_IO_InputMin * GSD_T_IO_Base)
#define ISO_T_IO_OUTPUTMIN 	(GSD_T_IO_OuputMin * GSD_T_IO_Base)


typedef ATTR_PNIO_PACKED_PRE struct s_pns_iso_time
{
	PNIO_UINT32 T_DC_Base;
	PNIO_UINT32 T_DC_Min;
	PNIO_UINT32 T_DC_Max;
	PNIO_UINT32 T_IO_Base;
	PNIO_UINT32 T_IO_InputMin;
	PNIO_UINT32 T_IO_OuputMin;
}ATTR_PNIO_PACKED TS_PNS_ISO_TIME, *TS_PNS_ISO_TIME_PTR;

ATTR_PNIO_PACKED_PRE struct s_pns_iso_setup
{
	PNIO_UINT16 u16Slot;
	PNIO_UINT16 u16Subslot;
	PNIO_UINT16 u16Cacf;
	PNIO_UINT16 u16CycleFactor;
	PNIO_UINT32 u32TimeInput;
	PNIO_UINT32 u32TimeOutput;
	PNIO_UINT32 u32TimeInputValid;
	PNIO_UINT32 u32TimeOutputValid;
}ATTR_PNIO_PACKED ;


//=================================================================================================

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif  /* __RZT2_PNS_RSK_H__ */
