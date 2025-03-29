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

#ifdef EDDS_MESSAGE
#endif
/*****************************************************************************/

/*===========================================================================*/
/* trace module data                                                         */
/*===========================================================================*/
#define LTRC_ACT_MODUL_ID  22
#define EDDS_MODULE_ID     LTRC_ACT_MODUL_ID  /* Ids 16..31 reserved for LL */

/**
 * \addtogroup RZT2
 * \ingroup LLIF
 * @{
 */

/*===========================================================================*/
/* includes                                                                  */
/*===========================================================================*/
#include "os.h"
#include "lsas_inc.h"
#include "lsas_int.h"
#include "lsas_dat.h"
#include "eddp_inc.h"
#include "eddp_int.h"           /* internal header  */
#include "pnio_types.h"
//#include "common_data.h"
#include "ether_config.h"
#include "eddp_types.h"
#include "eddp_llif.h"

#include "pnip_reg.h"
#include "pnip_xrsl.h"
#include "eddp_cmd_usr.h"
#include "eddp_hal_usr.h"


//#pragma data_alignment=4
//#pragma section="pnip_mem"
//LSA_UINT8 g_pnip_mem[0x200000] __attribute__((used, section("pnip_mem")));

#define EDDP_SYS_HDL		0xDEU

#define PNIP_IP_VERSION			0x120200
#define PNIP_IP_DEVELOPMENT		0x4A02823

#ifndef EDDP_PNIP_INITDONE_MSK
#define EDDP_PNIP_INITDONE_MSK  \
    (PNIP_A0_REG_INITDONE__MSK_API_DONE         | \
     PNIP_A0_REG_INITDONE__MSK_SYNC_DONE        | \
     PNIP_A0_REG_INITDONE__MSK_CMD_DONE         | \
     PNIP_A0_REG_INITDONE__MSK_PACK_DONE        | \
     PNIP_A0_REG_INITDONE__MSK_STATISTIC_DONE   | \
     PNIP_F0_REG_INITDONE__MSK_QUEUE_DONE       | \
     PNIP_F0_REG_INITDONE__MSK_VLAN_DONE        | \
     PNIP_A0_REG_INITDONE__MSK_FDB_DONE)
#endif


#define PNIP_LINK_UP	0x00004
#define PNIP_LINK_DOWN	0x00000

#define PNIP_LINK_SPEED_10	0x00004
#define PNIP_LINK_SPEED_100	0x00000

#define PNIP_LINK_MODE_FULL	0x00010
#define PNIP_LINK_MODE_HALF	0x00000


LSA_UINT32 PNIP_REG32_READ (LSA_UINT32, volatile EDD_COMMON_MEM_U32_PTR_TYPE);
LSA_VOID   PNIP_REG32_WRITE(LSA_UINT32, volatile EDD_COMMON_MEM_U32_PTR_TYPE, LSA_UINT32);

#define PNIP_MEM32_READ(_REG)			*((LSA_UINT32*)(U_PNIP__BASE + _REG))
#define PNIP_MEM32_WRITE(_REG, _VAL)	*((LSA_UINT32*)(U_PNIP__BASE + _REG)) = (LSA_UINT32)_VAL

#define PNIP_HAL_REG32_READ(_REG)			PNIP_REG32_READ(EDDP_SYS_HDL,  (volatile EDD_COMMON_MEM_U32_PTR_TYPE)(U_PNIP__BASE + _REG))
//#define PNIP_HAL_REG32_WRITE(_REG, _VAL)	PNIP_REG32_WRITE(EDDP_SYS_HDL, (volatile EDD_COMMON_MEM_U32_PTR_TYPE)(U_PNIP__BASE + _REG), _VAL)



typedef struct s_mac_xxx
{
	LSA_UINT32 Low;
	LSA_UINT32 High;
}T_MAC32, *T_MAC32_PTR;

typedef struct _s_pnip_portctrl
{
	LSA_UINT32 bTransferRate: 1U;      	/* Bit-0 */
	LSA_UINT32 bFowardingMode: 1U;		/* Bit-1 */
	LSA_UINT32 bRxdFragmentation: 1U;	/* Bit-2 */
	LSA_UINT32 bTxdFragmentation: 1U;	/* Bit-3 */
	LSA_UINT32 bFastForwarding: 1U;		/* Bit-4 */
	LSA_UINT32 b5: 1U;	/* Bit-5 */
	LSA_UINT32 b6: 1U;	/* Bit-6 */
	LSA_UINT32 b7: 1U;	/* Bit-7 */
}TS_PNIP_PORTCTRL, *TS_PNIP_PORTCTRL_PTR;

typedef union _u_pnip_portctrl
{
	TS_PNIP_PORTCTRL type;
	LSA_UINT32      dw;
}TU_PNIP_PORTCTRL, *TU_PNIP_PORTCTRL_PTR;

extern LSA_VOID eddp_interrupt(EDDP_HANDLE, LSA_UINT32);


LSA_VOID PNIP_HAL_PHY_CTRL(LSA_UINT32 u32Val);

LSA_VOID PNIP_Reset(LSA_VOID);



// T_PNIP_CTRL g_tPnipCtrl;

LSA_VOID PNIP_Init(LSA_VOID)
{
	PNIP_Reset();
}

LSA_VOID PNIP_Reset(LSA_VOID)
{
	__attribute__((unused))LSA_UINT32 u32PnipBase = U_PNIP__BASE;
	OsMemSet((LSA_VOID*)U_PNIP__BASE, 0, 0x200000);

	PNIP_MEM32_WRITE(PNIP_REG_IP_VERSION, PNIP_IP_VERSION);
	PNIP_MEM32_WRITE(PNIP_REG_IP_DEVELOPMENT, PNIP_IP_DEVELOPMENT);
	PNIP_MEM32_WRITE(PNIP_REG_INITDONE, EDDP_PNIP_INITDONE_MSK);

	PNIP_MEM32_WRITE(PNIP_REG_PORTCONTROL_P1, PNIP_REG_PORTCONTROL_P1_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_PORTCONTROL_P2, PNIP_REG_PORTCONTROL_P1_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_PORTCONTROL_P3, PNIP_REG_PORTCONTROL_P1_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_PORTCONTROL_P4, PNIP_REG_PORTCONTROL_P1_RST__VAL);

	PNIP_MEM32_WRITE(PNIP_REG_FLOWCONTROL_P1, PNIP_REG_FLOWCONTROL_P1_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_FLOWCONTROL_P2, PNIP_REG_FLOWCONTROL_P1_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_FLOWCONTROL_P3, PNIP_REG_FLOWCONTROL_P1_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_FLOWCONTROL_P4, PNIP_REG_FLOWCONTROL_P1_RST__VAL);

	PNIP_MEM32_WRITE(PNIP_REG_API_FORWARDINGMODE_P1, PNIP_REG_API_FORWARDINGMODE_P1_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_API_FORWARDINGMODE_P2, PNIP_REG_API_FORWARDINGMODE_P1_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_API_FORWARDINGMODE_P3, PNIP_REG_API_FORWARDINGMODE_P1_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_API_FORWARDINGMODE_P4, PNIP_REG_API_FORWARDINGMODE_P1_RST__VAL);

	PNIP_MEM32_WRITE(PNIP_REG_FDBPARAMSTRUCT_BASE, PNIP_REG_FDBPARAMSTRUCT_BASE_RST__VAL);

	PNIP_MEM32_WRITE(PNIP_REG_MC_DEFAULT_P1, PNIP_REG_MC_DEFAULT_P1_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_MC_DEFAULT_P2, PNIP_REG_MC_DEFAULT_P2_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_MC_DEFAULT_P3, PNIP_REG_MC_DEFAULT_P3_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_MC_DEFAULT_P4, PNIP_REG_MC_DEFAULT_P4_RST__VAL);

	PNIP_MEM32_WRITE(PNIP_REG_MC_DEFAULT_API, PNIP_REG_MC_DEFAULT_API_RST__VAL);


	PNIP_MEM32_WRITE(PNIP_REG_PHYOUI1_SELECT_P1, PNIP_REG_PHYOUI1_SELECT_P1_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_PHYOUI1_SELECT_P2, PNIP_REG_PHYOUI1_SELECT_P2_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_PHYOUI1_SELECT_P3, PNIP_REG_PHYOUI1_SELECT_P3_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_PHYOUI1_SELECT_P4, PNIP_REG_PHYOUI1_SELECT_P4_RST__VAL);

	PNIP_MEM32_WRITE(PNIP_REG_SYNCPARAMSTRUCT_BASE, PNIP_REG_SYNCPARAMSTRUCT_BASE_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_PHASEBASE_TIME, PNIP_REG_PHASEBASE_TIME_RST__VAL);

	PNIP_MEM32_WRITE(PNIP_REG_FRAMECLASS_DEFAULT, PNIP_REG_FRAMECLASS_DEFAULT_RST__VAL);

	PNIP_MEM32_WRITE(PNIP_REG_HOL_BARRIER_PA_0, PNIP_REG_HOL_BARRIER_PA_0_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_HOL_BARRIER_PA_1, PNIP_REG_HOL_BARRIER_PA_1_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_HOL_BARRIER_PA_2, PNIP_REG_HOL_BARRIER_PA_2_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_HOL_BARRIER_PA_3, PNIP_REG_HOL_BARRIER_PA_3_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_HOL_BARRIER_PA_4, PNIP_REG_HOL_BARRIER_PA_4_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_HOL_BARRIER_PA_5, PNIP_REG_HOL_BARRIER_PA_5_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_HOL_BARRIER_PA_6, PNIP_REG_HOL_BARRIER_PA_6_RST__VAL);

	PNIP_MEM32_WRITE(PNIP_REG_AUTOSTOP_BASETIME, PNIP_REG_AUTOSTOP_BASETIME_RST__VAL);

	PNIP_MEM32_WRITE(PNIP_R2F0_REG_PPM_FSO_LEADTIME, PNIP_R2F0_REG_PPM_FSO_LEADTIME_RST__VAL);

	PNIP_MEM32_WRITE(PNIP_REG_TSB_IFA, PNIP_REG_TSB_IFA_RST__VAL);

	PNIP_MEM32_WRITE(PNIP_REG_CSB_0_IFA, PNIP_REG_CSB_0_IFA_RST__VAL);

	PNIP_MEM32_WRITE(PNIP_REG_ACW_RBASEPTR_IFA, PNIP_REG_ACW_RBASEPTR_IFA_RST__VAL);

	PNIP_MEM32_WRITE(PNIP_REG_PHYSTATUS1_SELECT_P1, PNIP_REG_PHYSTATUS1_SELECT_P1_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_PHYSTATUS1_SELECT_P2, PNIP_REG_PHYSTATUS1_SELECT_P2_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_PHYSTATUS1_SELECT_P3, PNIP_REG_PHYSTATUS1_SELECT_P3_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_PHYSTATUS1_SELECT_P4, PNIP_REG_PHYSTATUS1_SELECT_P4_RST__VAL);

	PNIP_MEM32_WRITE(PNIP_REG_PHYSTATUS2_SELECT_P1, PNIP_REG_PHYSTATUS2_SELECT_P1_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_PHYSTATUS2_SELECT_P2, PNIP_REG_PHYSTATUS2_SELECT_P2_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_PHYSTATUS2_SELECT_P3, PNIP_REG_PHYSTATUS2_SELECT_P3_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_PHYSTATUS2_SELECT_P4, PNIP_REG_PHYSTATUS2_SELECT_P4_RST__VAL);

	PNIP_MEM32_WRITE(PNIP_REG_PHYSTATUS3_SELECT_P1, PNIP_REG_PHYSTATUS3_SELECT_P1_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_PHYSTATUS3_SELECT_P2, PNIP_REG_PHYSTATUS3_SELECT_P2_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_PHYSTATUS3_SELECT_P3, PNIP_REG_PHYSTATUS3_SELECT_P3_RST__VAL);
	PNIP_MEM32_WRITE(PNIP_REG_PHYSTATUS3_SELECT_P4, PNIP_REG_PHYSTATUS3_SELECT_P4_RST__VAL);

	PNIP_MEM32_WRITE(PNIP_REG_APPLCOMPARE_2_1, PNIP_REG_APPLCOMPARE_2_1_RST__VAL);

}


LSA_UINT32 PNIP_REG32_READ (LSA_UINT32 hSysDev, volatile EDD_COMMON_MEM_U32_PTR_TYPE pAddr)
{
	__attribute__((unused))LSA_UINT32 u32Addr = (LSA_UINT32)pAddr;
	LSA_UINT32 u32Val = 0U;

	if( (U_PNIP__BASE <= (LSA_UINT32)pAddr) && ((LSA_UINT32)pAddr <= (U_PNIP__BASE + 0x200000)) )
	{
		LSA_UINT32 u32Reg = ((LSA_UINT32)pAddr - U_PNIP__BASE);
		switch(u32Reg)
		{
		case PNIP_REG_STATUSCHANGE: // EDDP_GEN_ISR_LinkChange: read STATUSCHANGE to get reason(s) of this interrupt

			u32Val = *((LSA_UINT32*)pAddr);
			break;
		case PNIP_REG_PHYSTATUS1_P1:
		case PNIP_REG_PHYSTATUS1_P2:
		case PNIP_REG_PHYSTATUS1_P3:
		case PNIP_REG_PHYSTATUS1_P4:
		case PNIP_REG_PHYSTATUS2_P1:
		case PNIP_REG_PHYSTATUS2_P2:
		case PNIP_REG_PHYSTATUS2_P3:
		case PNIP_REG_PHYSTATUS2_P4:
		case PNIP_REG_PHYSTATUS3_P1:
		case PNIP_REG_PHYSTATUS3_P2:
		case PNIP_REG_PHYSTATUS3_P3:
		case PNIP_REG_PHYSTATUS3_P4:
			u32Val = *((LSA_UINT32*)pAddr);
			break;
		case PNIP_REG_RCT_DELAY: // ***** 250MHz free running counter  --> granularity is 4ns, range 0ns .. 4,29s
		{
			// --> Get from RZ/T2 Switch!!!!
			static LSA_UINT32 SwiNsCnt = 0;
			u32Val = ++SwiNsCnt;
		}break;
		case PNIP_REG_INITDONE: 		//	u32Val = EDDP_PNIP_INITDONE_MSK;
		case PNIP_REG_IP_VERSION: 		// u32Val = PNIP_IP_VERSION; 		break;
		case PNIP_REG_IP_DEVELOPMENT:	// u32Val = PNIP_IP_DEVELOPMENT; 	break;
			u32Val = *((LSA_UINT32*)pAddr);
			break;
		case PNIP_REG_I2C_CONTROL:
			u32Val = (PNIP_REG_I2C_CONTROL__MSK_SDA_I |
					  PNIP_REG_I2C_CONTROL__MSK_SCL_I |
					  PNIP_REG_I2C_CONTROL__MSK_SW_SDA_O |
					  PNIP_REG_I2C_CONTROL__MSK_SW_SCL_O);
			break;

		case PNIP_REG_MAC_SA_LOW_P1:  // 0x00016010
		case PNIP_REG_MAC_SA_LOW_P2:  // 0x00016018
		case PNIP_REG_MAC_SA_LOW_P3:  // 0x00016020
		case PNIP_REG_MAC_SA_HIGH_P1: // 0x00016014
		case PNIP_REG_MAC_SA_HIGH_P2: // 0x0001601C
		case PNIP_REG_MAC_SA_HIGH_P3: // 0x00016024
			u32Val = *((LSA_UINT32*)pAddr);
			break;

		case PNIP_REG_PORTCONTROL_P1:
		case PNIP_REG_PORTCONTROL_P2:
		case PNIP_REG_PORTCONTROL_P3:
		case PNIP_REG_PORTCONTROL_P4:
			// EDDP_SET_BIT_VALUE32(reg_val, 0x00000001, PNIP_REG_PORTCONTROL_P1__MSK_PTCP_FWD_DELAY, PNIP_REG_PORTCONTROL_P1_SHFT_PTCP_FWD_DELAY);
			//  EDDP_SET_BIT_VALUE32(reg_val, 0x00000001, PNIP_REG_PORTCONTROL_P1__MSK_TX_REDGUARD_DP2, PNIP_REG_PORTCONTROL_P1_SHFT_TX_REDGUARD_DP2);
			//u32Val = PNIP_REG_PORTCONTROL_P1_RST__VAL;
			u32Val = *((LSA_UINT32*)pAddr);
			break;

		default:
			u32Val = *((LSA_UINT32*)pAddr);
			break;
		}
	}
	return u32Val;
}

LSA_VOID PNIP_HAL_REG32_WRITE(LSA_UINT32 u32Reg, LSA_UINT32 u32Val)
{
	LSA_UINT32 *pAddr = (LSA_UINT32*)(U_PNIP__BASE + u32Reg);

	if( (U_PNIP__BASE <= (LSA_UINT32)pAddr) && ((LSA_UINT32)pAddr <= (U_PNIP__BASE + 0x200000)) )
	{
		switch(u32Reg)
		{
		case PNIP_REG_MD_DATA: // EDDP_SWI_PHY_Reg_Write:
			*((LSA_UINT32*)pAddr) = u32Val;
			break;
		case PNIP_REG_MD_CA: // ***** Write MD_CA to invoke SMI write command
			*((LSA_UINT32*)pAddr) = u32Val;
			if(0U < u32Val)
			{
				// ADDRESS: 0x0000001F, PHY_ADDRESS: 0x000003E0, WRITE: 0x00000400, BUSY:0x00000800, WAITTIME_PREAMBLE:0x0003F000
				*((LSA_UINT32*)pAddr) = (u32Val & 0xFFFFF7FF);	// ~0x00000800: ~BUSY
			}

			break;
		case PNIP_REG_STATUSCHANGE: // EDDP_GEN_ISR_LinkChange
			*((LSA_UINT32*)pAddr) = u32Val;
			break;
		case PNIP_REG_CONFIRMINGQUEUE:
		case PNIP_REG_TRIGGERREQQUEUE:
			// trigger queue processing
			// EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_TRIGGERREQQUEUE, (0x1 << EDDP_CMD_INDEX_HOST_SYNC));
			*((LSA_UINT32*)pAddr) = u32Val;
			break;
		case PNIP_REG_IRQ2ACK_LOW_0:
		case PNIP_REG_IRQ2ACK_MID_0:
		case PNIP_REG_IRQ2ACK_HIGH_0:
			*((LSA_UINT32*)pAddr) = u32Val;
			break;
		case PNIP_REG_IRQ2MASK_LOW_0:
		case PNIP_REG_IRQ2MASK_MID_0:
		case PNIP_REG_IRQ2MASK_HIGH_0:
			*((LSA_UINT32*)pAddr) = u32Val;
			break;

		case PNIP_REG_MAC_SA_LOW_P1: // 0x00016010
		case PNIP_REG_MAC_SA_LOW_P2: // 0x00016018
		case PNIP_REG_MAC_SA_LOW_P3: // 0x00016020
			*((LSA_UINT32*)pAddr) = u32Val;
			break;
		case PNIP_REG_MAC_SA_HIGH_P1: // 0x00016014
		case PNIP_REG_MAC_SA_HIGH_P2: // 0x0001601C
		case PNIP_REG_MAC_SA_HIGH_P3: // 0x00016024
			*((LSA_UINT32*)pAddr) = u32Val;
			break;
        /* ----------------------------------------------------------------------*/
        /* Setup PNIP_REG_API_FORWARDINGMODE_Px                                  */
        /* Queue 15 will be set to CutThrough.All other Queues to Store&Forward. */
        /* ----------------------------------------------------------------------*/
		case PNIP_REG_API_FORWARDINGMODE_P1: // 0x00001048
		case PNIP_REG_API_FORWARDINGMODE_P2: // 0x00002048
		case PNIP_REG_API_FORWARDINGMODE_P3: // 0x00003048
		case PNIP_REG_API_FORWARDINGMODE_P4: // 0x00004048
		{
			*((LSA_UINT32*)pAddr) = u32Val;
			// PNIP_REG_API_FORWARDINGMODE_P1_SHFT_API_FWMODE
			switch(u32Val)
			{
			case PNIP_REG_API_FORWARDINGMODE_P1__MSK_API_FWMODE_14:
			case PNIP_REG_API_FORWARDINGMODE_P1__MSK_API_FWMODE_15:

				break;
			default:
				break;

			}
		}break;


		/* ----------------------------------------------------------------------*/
	    // set up MAC address registers for RxPort filtering
		case PNIP_REG_MAC_SA_LOW_IFA:
		case PNIP_REG_MAC_SA_HIGH_IFA:
		case PNIP_REG_UC_AGING_A:

			// EDDP_SWI_Setup_Phase_List_Base_Addr:
		case EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P1:
		case EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P2:
		case EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P3:
		case EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_RX_P4:

		case EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P1:
		case EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P2:
		case EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P3:
		case EDDP_HAL_RAMSUB_SYNC_PHASE_LIST_TX_P4:

		case PNIP_REG_PHASEBASE_RXD_P1:
		case PNIP_REG_PHASEBASE_RXD_P2:
		case PNIP_REG_PHASEBASE_RXD_P3:
		case PNIP_REG_PHASEBASE_RXD_P4:

		case PNIP_REG_PHASEBASE_TXD_P1:
		case PNIP_REG_PHASEBASE_TXD_P2:
		case PNIP_REG_PHASEBASE_TXD_P3:
		case PNIP_REG_PHASEBASE_TXD_P4:
	        // write PNIP_REG_PHASEBASE_RXD_PX register : phase_list_rx_reg_address
			// write PNIP_REG_PHASEBASE_TXD_PX register : phase_list_tx_reg_address




        /* ---------------------------- */
        /* set PortControl_Px           */
        /* ---------------------------- */
		case PNIP_REG_PORTCONTROL_P1:
		case PNIP_REG_PORTCONTROL_P2:
		case PNIP_REG_PORTCONTROL_P3:
		case PNIP_REG_PORTCONTROL_P4:
			// for PNIP-Revision(2 and 3) the "Tx_RedGuard_DP2" is activated
			*((LSA_UINT32*)pAddr) = u32Val;
			break;
		/* ---------------------------- */
		/* set FlowControl_Px           */
		/* ---------------------------- */
		case PNIP_REG_FLOWCONTROL_P1: // 0x00001004
		case PNIP_REG_FLOWCONTROL_P2: // 0x00002004
		case PNIP_REG_FLOWCONTROL_P3: // 0x00003004
		case PNIP_REG_FLOWCONTROL_P4: // 0x00004004
			*((LSA_UINT32*)pAddr) = u32Val;
			// EDDP_SET_BIT_VALUE32(reg_val, 0x00000001, PNIP_REG_FLOWCONTROL_P1__MSK_DISABLEPORT, PNIP_REG_FLOWCONTROL_P1_SHFT_DISABLEPORT); /* Link-Down        */
			break;
		/* ---------------------------- */
		/* set Port_Grouping            */
		/* ---------------------------- */
		case PNIP_REG_PORT_GROUPING:
			*((LSA_UINT32*)pAddr) = u32Val;
			break;
		/* ---------------------------- */
		/* set FrameAging_Time_Px       */
		/* ---------------------------- */
		case PNIP_REG_FRAMEAGING_TIME_P1: // 0x00021304
		case PNIP_REG_FRAMEAGING_TIME_P2: // 0x00022304
		case PNIP_REG_FRAMEAGING_TIME_P3: // 0x00023304
		case PNIP_REG_FRAMEAGING_TIME_P4: // 0x00024304
			*((LSA_UINT32*)pAddr) = u32Val;
			/*
			  	EDDP_SET_BIT_VALUE32(reg_val, 0x00000001, PNIP_REG_FLOWCONTROL_P1__MSK_RXBOUNDARY_DP2, PNIP_REG_FLOWCONTROL_P1_SHFT_RXBOUNDARY_DP2);
				EDDP_SET_BIT_VALUE32(reg_val, 0x00000001, PNIP_REG_FLOWCONTROL_P1__MSK_TXBOUNDARY_DP2, PNIP_REG_FLOWCONTROL_P1_SHFT_TXBOUNDARY_DP2);
				EDDP_SET_BIT_VALUE32(reg_val, 0x00000001, PNIP_REG_FLOWCONTROL_P1__MSK_TXBOUNDARY_RED2GREEN_DP2, PNIP_REG_FLOWCONTROL_P1_SHFT_TXBOUNDARY_RED2GREEN_DP2);
			 */
			break;
		case PNIP_REG_MACCONTROL_P1:
		case PNIP_REG_MACCONTROL_P2:
		case PNIP_REG_MACCONTROL_P3:
		case PNIP_REG_MACCONTROL_P4:
			*((LSA_UINT32*)pAddr) = u32Val;
			// val = (PNIP_REG_MACCONTROL_P1__MSK_TRANSMIT_ENABLE | PNIP_REG_MACCONTROL_P1__MSK_RECEIVE_ENABLE);
			break;
		/* ---------------------------- */
		/* set IRT_DelaySafety          */
		/* ---------------------------- */
		case PNIP_REG_IRT_DELAYSAFETY:
		/* ---------------------------- */
		/* set SafetyMargin             */
		/* ---------------------------- */
		case PNIP_REG_SAFETYMARGIN:
		/* ---------------------------- */
		/* set IRQx_Control             */
		/* ---------------------------- */
		case PNIP_REG_IRQ2_CONTROL:
		case PNIP_REG_IRQ3_CONTROL:
		case PNIP_REG_UC_TABLE_BASE_A:

		case PNIP_REG_FDBPARAMSTRUCT_BASE:

			// _EDDP_SWI_Setup_MC_Default_Px
		case PNIP_REG_MC_DEFAULT_P1:
		case PNIP_REG_MC_DEFAULT_P2:
		case PNIP_REG_MC_DEFAULT_P3:
		case PNIP_REG_MC_DEFAULT_P4:
		case PNIP_REG_MC_DEFAULT_API:
		   // write PNIP_REG_MC_DEFAULT_API: forward local frames to all mapped ports of this device
			//EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MC_DEFAULT_API, (TxPort_P1 | TxPort_P2 | TxPort_P3 | TxPort_P4 ));

		case PNIP_REG_MC_TABLE_RANGE: // _EDDP_SWI_switch_MC_table
		case PNIP_REG_MC_TABLE_BASE:
			*((LSA_UINT32*)pAddr) = u32Val;
			break;
		/* Enable all PHYs (release reset) and set MDIO_HOLD */
		case PNIP_REG_PHY_CONTROL: /* 0x00008000: EDDP_SWIDeviceSetup1: Enable all PHYs (release reset) and set MDIO_HOLD */
			//EDDP_SET_BIT_VALUE32(reg_val, PNIP_REG_PHY_CONTROL_RSTV_TRANSFERRATE_DIV, PNIP_REG_PHY_CONTROL__MSK_TRANSFERRATE_DIV, PNIP_REG_PHY_CONTROL_SHFT_TRANSFERRATE_DIV);
#if 0
			if(0U < (PNIP_R2_REG_PHY_CONTROL__MSK_PHY_ACTIVATE & u32Val))
			{
				PNIP_HAL_REG32_WRITE(PNIP_REG_STATUSCHANGE, (PNIP_REG_STATUSCHANGE__MSK_PHY_PWRUP_P1 | PNIP_REG_STATUSCHANGE__MSK_PHY_PWRUP_P2) );
			}
			else
			{
				PNIP_HAL_REG32_WRITE(PNIP_REG_STATUSCHANGE, 0xFFFFFFFF & (~(PNIP_REG_STATUSCHANGE__MSK_PHY_PWRUP_P1 | PNIP_REG_STATUSCHANGE__MSK_PHY_PWRUP_P2)) );
			}
			PNIP_HAL_PHY_CTRL(u32Val);
#endif
			*((LSA_UINT32*)pAddr) = u32Val;

			break;
		case PNIP_REG_PHYOUI1_SELECT_P1: // // set PHY_OUI1_Select_PX
		case PNIP_REG_PHYOUI1_SELECT_P2:
		case PNIP_REG_PHYOUI1_SELECT_P3:
		case PNIP_REG_PHYOUI1_SELECT_P4:

			// set PHYCommand_PX | OUI1/2 polling is now active; when PHY is ready an interrupt is fired and EDDP_SWI_FinishPHYSetup() is called
			// select the right register address for current port number
		case PNIP_REG_PHYCOMMAND_P1:

		// set PHYStatus1_Select_PX (link status)
		//     u32Val = (InitData.LinkStatusRegAddress | 0x00000020/*Polling enable*/ | (InitData.LinkStatusBitMask << 16));
		case PNIP_REG_PHYSTATUS1_SELECT_P1:
		case PNIP_REG_PHYSTATUS1_SELECT_P2:
		case PNIP_REG_PHYSTATUS1_SELECT_P3:
		case PNIP_REG_PHYSTATUS1_SELECT_P4:

		//     u32Val = (InitData.SpeedRegAddress | 0x00000020/*Polling enable*/ | (InitData.SpeedBitMask << 16) );
		case PNIP_REG_PHYSTATUS2_SELECT_P1:
		case PNIP_REG_PHYSTATUS2_SELECT_P2:
		case PNIP_REG_PHYSTATUS2_SELECT_P3:
		case PNIP_REG_PHYSTATUS2_SELECT_P4:

		//     u32Val = (InitData.DuplexityRegAddress | 0x00000020/*Polling enable*/ | (InitData.DuplexityBitMask << 16) );
		case PNIP_REG_PHYSTATUS3_SELECT_P1:
		case PNIP_REG_PHYSTATUS3_SELECT_P2:
		case PNIP_REG_PHYSTATUS3_SELECT_P3:
		case PNIP_REG_PHYSTATUS3_SELECT_P4:

		case PNIP_REG_SYNCPARAMSTRUCT_BASE:  // set up base address for SYNC ParamStruct
		case PNIP_REG_PHASEBASE_TIME: 	     // set up base address for Time-List (PNIP_REG_PHASEBASE_TIME)

		case PNIP_REG_FRAMECLASS_0_1: // ...  EDDP_SWI_write_frameclass
		case PNIP_REG_FRAMECLASS_0_20:
		case PNIP_REG_FRAMECLASS_1_1: // ...
		case PNIP_REG_FRAMECLASS_1_20:
		case PNIP_REG_FRAMECLASS_2_1: // ...
		case PNIP_REG_FRAMECLASS_2_20:

		case PNIP_REG_FRAMECLASS_DEFAULT: // //set FrameClass_Default register , merge all values for PNIP_REG_FRAMECLASS_DEFAULT register

		    // set Traffic_Mode register to topology based frame handling
		    // EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_TRAFFIC_MODE, EDDP_SWI_REG_TRAFFIC_MODE_INIT);
		case PNIP_REG_TRAFFIC_MODE:


		case PNIP_REG_UC_LEARNINGMODE: // 0x00011028:  set up "UC_LearningMode"

		case PNIP_REG_HOL_CONTROL_PA:  // 0x00027004: EDDP_SWI_Set_HOL_Barriers: This functions sets all HOL barriers according to the number of interfaces and used hardware.
		case PNIP_REG_HOL_CONTROL_IFA: // 0x00020144:
		case PNIP_REG_HOL_BARRIER_PA_0:
		case PNIP_REG_HOL_BARRIER_PA_1:
		case PNIP_REG_HOL_BARRIER_PA_2:
		case PNIP_REG_HOL_BARRIER_PA_3:
		case PNIP_REG_HOL_BARRIER_PA_4:
		case PNIP_REG_HOL_BARRIER_PA_5:
		case PNIP_REG_HOL_BARRIER_PA_6:

		case PNIP_REG_HOL_BARRIER_IFA_0:
		case PNIP_REG_HOL_BARRIER_IFA_7:

		case PNIP_REG_QUEUE14_MAPPING: //// set prio7 for Queue14 (RTC1, RTC2 and RToverUDP)


		case PNIP_R2_REG_FILTERCODEBASE_IFA_00: // EDDP_HWCInitFilterCode
			// special filter (for Queue 14/15) to detect wrong switching of RT-frames into NRT instead into CRT
			//EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2_REG_FILTERCODEBASE_IFA_14, nApiCtrlCodeAddr+EDDP_API_TYPE_RT_CT);
			//EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2_REG_FILTERCODEBASE_IFA_15, nApiCtrlCodeAddr+EDDP_API_TYPE_RT_SF);
		case PNIP_R2_REG_FILTERCODEBASE_IFA_14: // EDDP_HWCInitFilterCode
		case PNIP_R2_REG_FILTERCODEBASE_IFA_15: // EDDP_HWCInitFilterCode


		case PNIP_REG_FILTERERRSTATUS_IFA: // EDDP_HWCSetRegisterToDefaults
		case PNIP_REG_FILTERERREVENT_IFA:
		case PNIP_REG_LOGCONTROL_IFA: // --> 0:Disable

		case PNIP_R2_REG_DROPCOUNT_IFA_00:// ..
		case PNIP_R2_REG_DROPCOUNT_IFA_01:
		case PNIP_R2_REG_DROPCOUNT_IFA_02:
		case PNIP_R3_REG_DROPCOUNT_IFA_15:


		case PNIP_R2_REG_SDTABLEBASE_IFA:

		case PNIP_REG_LOCAL_IP_HEADER: // init PNIP_REG_LOCAL_IP_HEADER.IP_TTL
		case PNIP_REG_AUTOSTOP_BASETIME:

		case PNIP_R2F0_REG_PPM_FSO_LEADTIME:

		case PNIP_R2_REG_QUEUETYPE_SELECT: // 0x00000600
		    /* Init PNIP_F0_REG_PPM_FSO_LEADTIME...                                      */
		    /* ...and setup the API-Types used for CPM receiption without API-HW-Filter. */
		    /* We dont use the HW-Filter with CPM RT-Frames! We queue all RT             */
		    /* Frames within Queue14/15 to local API.                                    */
		    /* Queue 14 is used for Store&Forward, Queue 15 for CutThrough.              */
			/*RegValueTemp = (PNIP_R2_REG_QUEUETYPE_SELECT__MSK_APITYPE_VALID14 | EDDP_CRT_API_TYPE_RT_SF << PNIP_R2_REG_QUEUETYPE_SELECT_SHFT_APITYPE_QUEUE14 |
							PNIP_R2_REG_QUEUETYPE_SELECT__MSK_APITYPE_VALID15 | EDDP_CRT_API_TYPE_RT_CT << PNIP_R2_REG_QUEUETYPE_SELECT_SHFT_APITYPE_QUEUE15 |
							PNIP_R2_REG_QUEUETYPE_SELECT__MSK_Q15_SELECT);
			*/


		/* ---------------------------------------------------- */
		/* PPM register                                         */
		/* ---------------------------------------------------- */
		case PNIP_REG_APPLDATASTATUS_IFA_0: // ...
		case PNIP_REG_APPLDATASTATUS_IFA_7:
		case PNIP_REG_DATASTATUS_MASK_IFA:
		case PNIP_REG_ACW_T_PO1K_IFA:
		case PNIP_REG_DG_DATASTATUS_IFA_0:
		case PNIP_REG_DG_DATASTATUS_MASK_IFA:

		/* ---------------------------------------------------- */
		/* CPM register                                         */
		/* ---------------------------------------------------- */
		case PNIP_REG_TSB_IFA: // TIMER_SCOREBOARD_BASEPTR  | PNIP_REG_TSB_IFA__MSK_TSB_ENTRIES

		case PNIP_REG_CSB_0_IFA: // CONSUMER_SCOREBOARD_BASEPTR
		case PNIP_REG_CSB_1_IFA: // CONSUMER_SCOREBOARD_BASEPTR

		case PNIP_R3_REG_MRT_RR_RELOADVALUE_IFA:
		case PNIP_REG_ACW_RBASEPTR_IFA:
		case PNIP_REG_ACW_RINDEXSIZE_IFA:

		case PNIP_REG_ACW_TBASEPTR_IFA:

		case PNIP_REG_KRISC_TCM_MAP:

		case PNIP_REG_IRQ1_ACTIVATE:
		case PNIP_REG_IRQ2_ACTIVATE:
		case PNIP_REG_IRQ3_ACTIVATE:

		case PNIP_REG_I2C_SELECT:
		case PNIP_REG_I2C_ENABLE:
		case PNIP_REG_I2C_CLOCKCONTROL:
		case PNIP_REG_EX_CONTROL_1:
		case PNIP_REG_EX_CONTROL_2:
		case PNIP_REG_EX_CONTROL_3:
		case PNIP_REG_EX_CONTROL_4:
		case PNIP_REG_I2C_CONTROL:
		case PNIP_REG_MI2C_SOFTWARE_RESET:

		/* ---------------------------------------- */
		/* Write "TransferEndNs" to register        */
		/* ---------------------------------------- */
		// EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_APPLCOMPARE_2_1, pDDB->PRM.TransferEndNs);
		case PNIP_REG_APPLCOMPARE_2_1:

		default:
			*((LSA_UINT32*)pAddr) = u32Val;
			break;
		}
	}

}


LSA_VOID PNIP_REG32_WRITE(LSA_UINT32 hSysDev, volatile EDD_COMMON_MEM_U32_PTR_TYPE pAddr, LSA_UINT32 u32Val)
{
	if( (U_PNIP__BASE <= (LSA_UINT32)pAddr) && ((LSA_UINT32)pAddr <= (U_PNIP__BASE + 0x200000)) )
	{
		LSA_UINT32 u32Reg = ((LSA_UINT32)pAddr - U_PNIP__BASE);
		
		/** Save the previous value of specified regsiter */
		LSA_UINT32 u32Val_prev = PNIP_MEM32_READ(u32Reg);
		
		switch(u32Reg)
		{

		case PNIP_REG_MD_DATA: // EDDP_SWI_PHY_Reg_Write:
			*((LSA_UINT32*)pAddr) = u32Val;
			break;
		case PNIP_REG_MD_CA: // ***** Write MD_CA to invoke SMI write command
			// ADDRESS: 0x0000001F, PHY_ADDRESS: 0x000003E0, WRITE: 0x00000400, BUSY:0x00000800, WAITTIME_PREAMBLE:0x0003F000
			//PNIP_HAL_REG32_WRITE(PNIP_REG_MD_CA, u32Val & 0xFFFFF7FF);
			PNIP_HAL_REG32_WRITE(PNIP_REG_MD_CA, u32Val & 0xFFFFF7FF); // ~0x00000800: ~BUSY
			break;
		case PNIP_REG_STATUSCHANGE: // EDDP_GEN_ISR_LinkChange
			PNIP_HAL_REG32_WRITE(PNIP_REG_STATUSCHANGE, (PNIP_HAL_REG32_READ(PNIP_REG_STATUSCHANGE)) & ~u32Val);
			break;
						
		case PNIP_REG_CSB_CONTROL_IFA:
			
			/** 
			 * Assert CHANGE_ENABLE bit
			 * NOTE: 
			 * 	According to usage of this bit, 
			 * 	the CHANGE_ENABLE bit may be set to 0 immediately after changing the CSB_SELECT.
			 * 	Therefore, the CHANGE_ENABLE bit operation is unnecessary.
			 */
			if ( u32Val & PNIP_REG_CSB_CONTROL_IFA__MSK_CSB_CHANGE_ENABLE )
			{			
				/** Check if the previous scoreboard selection is 1. */
				if( u32Val_prev & PNIP_REG_CSB_CONTROL_IFA__MSK_CSB_SELECT )
				{
					/** Set the new selection to 0. */
					PNIP_MEM32_WRITE(u32Reg, u32Val_prev & ~PNIP_REG_CSB_CONTROL_IFA__MSK_CSB_SELECT );
				}
				/** Otherwise, the previous selection is 0. */
				else
				{
					/** Set the new selection to 1.*/
					PNIP_MEM32_WRITE(u32Reg, u32Val_prev | PNIP_REG_CSB_CONTROL_IFA__MSK_CSB_SELECT ) ;
				}
			}
			break;
			
		default:
			PNIP_HAL_REG32_WRITE(u32Reg, u32Val);
			break;
		}
	}
}

#if 0
LSA_VOID PNIP_HAL_PHY_CTRL(LSA_UINT32 u32Val)
{
	if(0U < (PNIP_R2_REG_PHY_CONTROL__MSK_PHY_ACTIVATE & u32Val) )
	{
		// eddp_interrupt(lsas_com_get_hddb(), 0);
		EDDP_LOCAL_DDB_PTR_TYPE   pDDB = (EDDP_LOCAL_DDB_PTR_TYPE)lsas_com_get_hddb();
		EDDP_LOCAL_DDB_CORE_ISR_PTR_TYPE   pIsrInfo;
		EDDP_ISR_GROUP_INFO_PTR_TYPE    pGroup;
		LSA_UINT32 dwReg = PNIP_HAL_REG32_READ(PNIP_REG_STATUSCHANGE);

		if(0U < (PNIP_R2_REG_PHY_CONTROL__MSK_PHY_ACTIVATE & u32Val))
		{
			PNIP_HAL_REG32_WRITE(PNIP_REG_STATUSCHANGE, (PNIP_REG_STATUSCHANGE__MSK_PHY_PWRUP_P1 | PNIP_REG_STATUSCHANGE__MSK_PHY_PWRUP_P2) );
			//EDDP_LL_EVENT_IND(EDDP_LL_EV_PHY_ON);
		}
		else
		{
			PNIP_HAL_REG32_WRITE(PNIP_REG_STATUSCHANGE, (PNIP_REG_STATUSCHANGE__MSK_PHY_PWRUP_P1 | PNIP_REG_STATUSCHANGE__MSK_PHY_PWRUP_P2) );
			//EDDP_LL_EVENT_IND(EDDP_LL_EV_PHY_OFF);
		}

		if(0U < (PNIP_R2_REG_PHY_CONTROL__MSK_PHY_ACTIVATE & u32Val))
		//if (!(pGroup->IntRQB.InUse))
		{
			PNIP_HAL_REG32_WRITE(PNIP_REG_IRQ2EVENT_LOW_0, 1);

			pGroup = &pDDB->Core.Isr.Group;
			pGroup->IntRQB.InUse = LSA_TRUE; /* RQB is underway.. */
			EDDP_DO_INTERRUPT_REQUEST(pDDB->hSysDev, pGroup->IntRQB.pRQB);
		}

	}
	else
	{

	}
}
#endif


/*
 * +LinkStatus: EDDP_LL_PHY_LINKSTATUSBITMASK:
 * 		- EDD_LINK_UP:   0x00004,
 * 		- EDD_LINK_DOWN: 0x00000
 *
 * +Speed: EDDP_LL_PHY_SPEEDBITMASK:
 * 		- EDD_LINK_SPEED_10:  0x00004,
 * 		- EDD_LINK_SPEED_100: 0x00000
 *
 * 	+Duplexity: EDDP_LL_PHY_DUPLEXITYBITMASK
 * 		- EDD_LINK_MODE_FULL: 0x0010,
 * 		- EDD_LINK_MODE_HALF: 0x0000,
 *
 * */

LSA_VOID PNIP_HAL_PHY_SET_LINK(LSA_UINT16 HWPortID, LSA_UINT16 LinkStatus, LSA_UINT16 Speed, LSA_UINT16  Duplexity)
{
	LSA_UINT32 Status1 = 0U;
	LSA_UINT32 Status2 = 0U;
	LSA_UINT32 Status3 = 0U;
	LSA_UINT32 StatusChange =  PNIP_HAL_REG32_READ(PNIP_REG_STATUSCHANGE);

	switch(HWPortID)
	{
	case 1U:
		Status1 = PNIP_REG_PHYSTATUS1_P1;
		Status2 = PNIP_REG_PHYSTATUS2_P1;
		Status3 = PNIP_REG_PHYSTATUS3_P1;
		StatusChange |=  (PNIP_REG_STATUSCHANGE__MSK_STATUSCHANGE1_P1|PNIP_REG_STATUSCHANGE__MSK_STATUSCHANGE2_P1|PNIP_REG_STATUSCHANGE__MSK_STATUSCHANGE3_P1);
		break;
	case 2U:
		Status1 = PNIP_REG_PHYSTATUS1_P2;
		Status2 = PNIP_REG_PHYSTATUS2_P2;
		Status3 = PNIP_REG_PHYSTATUS3_P2;
		StatusChange |=  (PNIP_REG_STATUSCHANGE__MSK_STATUSCHANGE1_P2|PNIP_REG_STATUSCHANGE__MSK_STATUSCHANGE2_P2|PNIP_REG_STATUSCHANGE__MSK_STATUSCHANGE3_P2);
		break;
	case 3U:
		Status1 = PNIP_REG_PHYSTATUS1_P3;
		Status2 = PNIP_REG_PHYSTATUS2_P3;
		Status3 = PNIP_REG_PHYSTATUS3_P3;
		StatusChange |=  (PNIP_REG_STATUSCHANGE__MSK_STATUSCHANGE1_P3|PNIP_REG_STATUSCHANGE__MSK_STATUSCHANGE2_P3|PNIP_REG_STATUSCHANGE__MSK_STATUSCHANGE3_P3);
		break;
	case 4U:
		Status1 = PNIP_REG_PHYSTATUS1_P4;
		Status2 = PNIP_REG_PHYSTATUS2_P4;
		Status3 = PNIP_REG_PHYSTATUS3_P4;
		StatusChange |=  (PNIP_REG_STATUSCHANGE__MSK_STATUSCHANGE1_P4|PNIP_REG_STATUSCHANGE__MSK_STATUSCHANGE2_P4|PNIP_REG_STATUSCHANGE__MSK_STATUSCHANGE3_P4);
		break;
	default:
		break;
	}

	if(EDD_LINK_UP == LinkStatus)
	{
		PNIP_HAL_REG32_WRITE(Status1, PNIP_LINK_UP & PNIP_REG_PHYSTATUS1_P1__MSK_PHY_STATUS1);

	    if(EDD_LINK_SPEED_100 == Speed)
	    {
	    	PNIP_HAL_REG32_WRITE(Status2, EDD_LINK_SPEED_100 & PNIP_REG_PHYSTATUS2_P1__MSK_PHY_STATUS2);
	    }
	    else
	    {
	    	PNIP_HAL_REG32_WRITE(Status2, EDD_LINK_SPEED_10 & PNIP_REG_PHYSTATUS2_P1__MSK_PHY_STATUS2);
	    }

	    if(EDD_LINK_MODE_FULL == Duplexity)
	    {
	    	PNIP_HAL_REG32_WRITE(Status3, PNIP_LINK_MODE_FULL & PNIP_REG_PHYSTATUS3_P1__MSK_PHY_STATUS3);
	    }
	    else
	    {
	    	PNIP_HAL_REG32_WRITE(Status3, PNIP_LINK_MODE_HALF & PNIP_REG_PHYSTATUS3_P1__MSK_PHY_STATUS3);
	    }
	}
	else
	{
		PNIP_HAL_REG32_WRITE(Status1, PNIP_LINK_DOWN & PNIP_REG_PHYSTATUS1_P1__MSK_PHY_STATUS1);
		PNIP_HAL_REG32_WRITE(Status2, 0);
		PNIP_HAL_REG32_WRITE(Status2, 0);
	}


    PNIP_HAL_REG32_WRITE(PNIP_REG_STATUSCHANGE, StatusChange);

  //  PNIP_HAL_REG32_WRITE(PNIP_REG_STATUSCHANGE, Duplexity & PNIP_REG_PHYSTATUS3_P1__MSK_PHY_STATUS3);


}


LSA_VOID PNIP_HAL_PHY_SET_POWER(LSA_UINT16 HWPortID, LSA_UINT16 PwrCtrl)
{
	LSA_UINT32 StatusChange =  PNIP_HAL_REG32_READ(PNIP_REG_STATUSCHANGE);

	LSA_UINT32 RegMask = 0;

	switch(HWPortID)
	{
	case 1U: RegMask = PNIP_REG_STATUSCHANGE__MSK_PHY_PWRUP_P1; break;
	case 2U: RegMask = PNIP_REG_STATUSCHANGE__MSK_PHY_PWRUP_P2; break;
	case 3U: RegMask = PNIP_REG_STATUSCHANGE__MSK_PHY_PWRUP_P3; break;
	case 4U: RegMask = PNIP_REG_STATUSCHANGE__MSK_PHY_PWRUP_P4; break;
	default:
		break;
	}

	if(0U < RegMask)
	{
		//PNIP_HAL_PHY_SET_LINK(HWPortID, EDD_LINK_DOWN, 0U, 0U);
		StatusChange =  PNIP_HAL_REG32_READ(PNIP_REG_STATUSCHANGE);
		if(EDDP_PHY_POWERUP == PwrCtrl)
		{
			StatusChange |= RegMask;
		}
		else
		{
			StatusChange &= (~RegMask);
		}
		PNIP_HAL_REG32_WRITE(PNIP_REG_STATUSCHANGE, StatusChange);
	}

}


#if 0

LSA_VOID PNIP_STATUSCHANGE_IND(LSA_UINT16 HWPortID, LSA_UINT32 Status)
{
	LSA_UINT32 reg_address;
    // select the right register address for current HwPortID "i"
    reg_address = (HWPortID==1 ? PNIP_REG_PHYSTATUS1_P1 :
                  (HWPortID==2 ? PNIP_REG_PHYSTATUS1_P2 :
                  (HWPortID==3 ? PNIP_REG_PHYSTATUS1_P3 :
                  (HWPortID==4 ? PNIP_REG_PHYSTATUS1_P4 : 0))));
    PNIP_REG32_WRITE(reg_address, LinkStatus & PNIP_REG_PHYSTATUS1_P1__MSK_PHY_STATUS1);

    // select the right register address for current HwPortID "i"
    reg_address = (HWPortID==1 ? PNIP_REG_PHYSTATUS2_P1 :
                  (HWPortID==2 ? PNIP_REG_PHYSTATUS2_P2 :
                  (HWPortID==3 ? PNIP_REG_PHYSTATUS2_P3 :
                  (HWPortID==4 ? PNIP_REG_PHYSTATUS2_P4 : 0))));
    PNIP_REG32_WRITE(reg_address, Speed & PNIP_REG_PHYSTATUS2_P1__MSK_PHY_STATUS2);

    // select the right register address for current HwPortID "i"
    reg_address = (HWPortID==1 ? PNIP_REG_PHYSTATUS3_P1 :
                  (HWPortID==2 ? PNIP_REG_PHYSTATUS3_P2 :
                  (HWPortID==3 ? PNIP_REG_PHYSTATUS3_P3 :
                  (HWPortID==4 ? PNIP_REG_PHYSTATUS3_P4 : 0))));
    PNIP_REG32_WRITE(reg_address, Duplexity & PNIP_REG_PHYSTATUS3_P1__MSK_PHY_STATUS3);
}

#endif


/**
 *  EVMA_WRITE_PNIP_PLL_OUT_CONTROL is used to set the Multiplexer of the PNIP PNPLL module.
 *  @param x        defines the multiplexer output index (0-20)
 *  @param value    defines the 32 bit register value to be written
 */
LSA_VOID EVMA_WRITE_PNIP_PLL_OUT_CONTROL(LSA_UINT32 index, LSA_UINT32 value)
{

}

/**
 *  EVMA_WRITE_PNIP_APPLTIMER2_COMP_TIME is used to set a comparator value of application timer 2
 *  @param x        defines the comparator index (0(1) - 6)
 *  @param value    defines the 32 bit register value to be written
 */
LSA_VOID EVMA_WRITE_PNIP_APPLTIMER2_COMP_TIME(LSA_UINT32 index, LSA_UINT32 value)
{

}
/**
 * EVMA_READ_PNIP_APPLTIMER2_TIME reads the current timer value of application timer 2
 */
LSA_UINT32 EVMA_READ_PNIP_APPLTIMER2_TIME(LSA_VOID)
{
	return 0;
}
/**
 * EVMA_READ_PNIP_APPLTIMER2_LENGTH reads the cycle length of application timer 2
 */
LSA_UINT32 EVMA_READ_PNIP_APPLTIMER2_LENGTH(LSA_VOID)
{
	return 0;
}
/**
 * EVMA_READ_PNIP_APPLTIMER2_COMP_TIME reads a comparator value of application timer 2. 0(1) <= x <= 6 (= comparator index)
 */
LSA_UINT32 EVMA_READ_PNIP_APPLTIMER2_COMP_TIME(LSA_UINT32 comparator)
{
	return 0;
}



/*****************************************************************************/
/* end of file rzt2_edd.c                                                  */
/*****************************************************************************/
