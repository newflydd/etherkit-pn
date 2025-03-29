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
/*  F i l e               &F: eddp_crt_cons_acw.c                       :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  CRT (Cycle Real Time) for EDDP  -> Consumer ACWs                         */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who         What                                             */
#ifdef EDDP_MESSAGE
/*  02.12.09    SF          Erstellung                                       */
#endif
/*****************************************************************************/


/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/

#define LTRC_ACT_MODUL_ID   33
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_CRT_CONS_ACW */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"           /* eddp headerfiles */
#include "eddp_int.h"           /* internal header  */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)





#if defined (EDDP_CFG_HW_ERTEC200P_SUPPORT)
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTConsUpdateSrcAddrCsaInConsAcwHwApi              +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                        +*/
/*+                             LSA_UINT16                  ConsumerID                  +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ConsumerID     : Index in consumer array                                           +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function makes an update of: ACW.CSA and ACW.SrcAddr             +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsUpdateSrcAddrCsaInConsAcwHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ConsumerID)
{
    LSA_UINT64                          ACW_Word64;
    EDDP_CRT_CONSUMER_PTR_TYPE          pConsumer;
    EDD_MAC_ADR_TYPE                    *pMacAddr;
    EDD_IP_ADR_TYPE                     *pIpAddr;
    LSA_UINT16                          CSA_active;

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTConsUpdateSrcAddrCsaInConsAcwHwApi(ConsumerID=%d)", 
        ConsumerID);

    EDDP_ASSERT_FALSE(ConsumerID >= pDDB->CRT.CfgParam.ConsumerMaxNumber);
    EDDP_ASSERT_FALSE(pDDB->CRT.Cons.pAcwPtrArray[ConsumerID]->IsAcwContainer);

    pMacAddr    = LSA_NULL;
    pIpAddr     = LSA_NULL;
    CSA_active  = 0;
    pConsumer   = &pDDB->CRT.Cons.pConsEntry[ConsumerID];

    // ***** RT_class: RT-Frame or RToverUDP-Frame ------------------------------------------------------
    if (pConsumer->Param.RTClass == EDDP_CRT_RTCLASS_RT)
    {   // ***** class set by service: EDD_SRV_SRT_CONSUMER_ADD
        pMacAddr = &pConsumer->Param.SrcMAC;
        CSA_active = pMacAddr->MacAdr[0] | pMacAddr->MacAdr[1] | pMacAddr->MacAdr[2] | pMacAddr->MacAdr[3] | pMacAddr->MacAdr[4] | pMacAddr->MacAdr[5];
    }
    else
    {   // ***** class set by service: EDD_SRV_SRT_UDP_CONSUMER_ADD
        pIpAddr = &pConsumer->Param.SrcIP;
        CSA_active = pIpAddr->IPAdr[0] | pIpAddr->IPAdr[1] | pIpAddr->IPAdr[2] | pIpAddr->IPAdr[3];
    }

    /* ----------------------------------------------------------------------------------------------- */
    /* Word1:                                                                                          */
    /* FrameID, ACW_nextoffset, ACW_DG_offset, OpC, A, O, res, E_sel, FID, DGM, CSA, RT, ACW_DG_order  */
    /*                                                                          ^                      */
    /* ----------------------------------------------------------------------------------------------- */
    ACW_Word64 = EDDP_HAL_MEM64_READ(pDDB, &((PNIP_ACW_RCV_PTR_TYPE)(pConsumer->ConsAcwTypeInfo.pHwAcwApi))->ACW_Word1);
    // ***** En_CSA: enable/disable check source address ------------------------------------------------
    if ( CSA_active )
    {   // ***** enable check source address if not 00.00.00.00.00.00.00
        EDDP_SET_BIT_VALUE64( ACW_Word64, PNIP_ACW_RCV_WORD1_EN_CSA_ON_VALUE,  PNIP_ACW_RCV_WORD1_EN_CSA_MASK, PNIP_ACW_RCV_WORD1_EN_CSA_SHIFT);
    }
    else
    {   // ***** disable check source address if 00.00.00.00.00.00.00
        EDDP_SET_BIT_VALUE64( ACW_Word64, PNIP_ACW_RCV_WORD1_EN_CSA_OFF_VALUE, PNIP_ACW_RCV_WORD1_EN_CSA_MASK, PNIP_ACW_RCV_WORD1_EN_CSA_SHIFT);
    }
    // ***** write HOST value to APICtrl-RAM ------------------------------------------------------------
    EDDP_HAL_MEM64_WRITE (pDDB, &((PNIP_ACW_RCV_PTR_TYPE)(pConsumer->ConsAcwTypeInfo.pHwAcwApi))->ACW_Word1, ACW_Word64);

    /* ----------------------------------------------------------------------------------------------- */
    /* Word2:                                                                                          */
    /* MAC_SA,                Length, IP_D_sel                                                         */
    /* S-IP-Addr, UDP-S-Port, Length, IP_D_sel                                                         */
    /* ^                      ^                                                                        */
    /* ----------------------------------------------------------------------------------------------- */
    ACW_Word64 = EDDP_HAL_MEM64_READ(pDDB, &((PNIP_ACW_RCV_PTR_TYPE)(pConsumer->ConsAcwTypeInfo.pHwAcwApi))->ACW_Word2);
    ACW_Word64 &= (~PNIP_ACW_RCV_WORD2_MAC_SA_ALL_MASK);
    // ***** MAC_SA / S-IP-Addr + UDP-S-Port ------------------------------------------------------------
    if ( pMacAddr != LSA_NULL )
    {   // ***** RT-Frame: SrcMAC[0..5]                   --> from service: EDD_SRV_SRT_CONSUMER_ADD
        // ***** Note: fill MAC in reverse order, because PNIP reads reverse !!!!
        ACW_Word64 |= ( ((LSA_UINT64)pMacAddr->MacAdr[5] << PNIP_ACW_RCV_WORD2_MAC_SA_0_SHIFT) |
                        ((LSA_UINT64)pMacAddr->MacAdr[4] << PNIP_ACW_RCV_WORD2_MAC_SA_1_SHIFT) |
                        ((LSA_UINT64)pMacAddr->MacAdr[3] << PNIP_ACW_RCV_WORD2_MAC_SA_2_SHIFT) |
                        ((LSA_UINT64)pMacAddr->MacAdr[2] << PNIP_ACW_RCV_WORD2_MAC_SA_3_SHIFT) |
                        ((LSA_UINT64)pMacAddr->MacAdr[1] << PNIP_ACW_RCV_WORD2_MAC_SA_4_SHIFT) |
                        ((LSA_UINT64)pMacAddr->MacAdr[0] << PNIP_ACW_RCV_WORD2_MAC_SA_5_SHIFT) );
    }
    if ( pIpAddr != LSA_NULL )
    {   // ***** RToverUDP-Frame: SrcIP[0..3] + SrcPort   --> from service: EDD_SRV_SRT_UDP_CONSUMER_ADD
        // ***** Note: fill IP in reverse order, because PNIP reads reverse !!!!
        ACW_Word64 |= ( ((LSA_UINT64)pIpAddr->IPAdr[3] << PNIP_ACW_RCV_WORD2_SRCIP_ADDRESS_0_SHIFT) |
                        ((LSA_UINT64)pIpAddr->IPAdr[2] << PNIP_ACW_RCV_WORD2_SRCIP_ADDRESS_1_SHIFT) |
                        ((LSA_UINT64)pIpAddr->IPAdr[1] << PNIP_ACW_RCV_WORD2_SRCIP_ADDRESS_2_SHIFT) |
                        ((LSA_UINT64)pIpAddr->IPAdr[0] << PNIP_ACW_RCV_WORD2_SRCIP_ADDRESS_3_SHIFT) );
        // ***** UDP-SourcePort
        ACW_Word64 |= ((LSA_UINT64)EDDP_CRT_ETHER_TYPE << PNIP_ACW_RCV_WORD2_UDP_SP_SHIFT);
    }
    EDDP_HAL_MEM64_WRITE (pDDB, &((PNIP_ACW_RCV_PTR_TYPE)(pConsumer->ConsAcwTypeInfo.pHwAcwApi))->ACW_Word2, ACW_Word64);
}
#endif

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTConsForceBackupDataStatusHwApi                  +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer               +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  pConsumer      : pointer to consumer                                               +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Patching APDU-DataStatus to BACKUP in the APICtrl-RAM for given       +*/
/*+               consumer.                                                             +*/
/*+                                                                                     +*/
/*+               Only for SRD-Consumer!                                                +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsForceBackupDataStatusHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer)
{
    LSA_UINT8       APDUDataStatus;

    LSA_UNUSED_ARG(pDDB);

    EDDP_ASSERT_NO_NULL_PTR(pConsumer);
    EDDP_ASSERT((pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_SRD_ITEM));   // only for SRD-Consumer

    // read current APDU-DataStatus from APICtrl-RAM
    APDUDataStatus = EDDP_HAL_MEM8_READ(pDDB, &pConsumer->ConsAcwTypeInfo.pHwApduApi->DataStatus);
    // patch it to BACKUP
    EDDP_SET_BIT_VALUE8(APDUDataStatus, PNIP_ADPU_RCV_STATE_BACKUP, PNIP_ADPU_RCV_STATE_MASK, PNIP_ADPU_RCV_STATE_SHIFT);
    // and write the value back to APICtrl-RAM
    EDDP_HAL_MEM8_WRITE(pDDB, &pConsumer->ConsAcwTypeInfo.pHwApduApi->DataStatus, APDUDataStatus);
}





#if defined (EDDP_CFG_HW_ERTEC200P_SUPPORT)
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTConsInitConsAcwHwApi                            +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                        +*/
/*+                             LSA_UINT16                  ConsumerID                  +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ConsumerID     : Index in consumer array                                           +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Init the receive ACW in ApiCtrlRAM for CRT-Communication by Service   +*/
/*+               EDD_SRV_SRT_CONSUMER_ADD                                              +*/
/*+                                                                                     +*/
/*+               Call this function only after coping of all Consumer-Add parameter!   +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsInitConsAcwHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ConsumerID)
{
    LSA_UINT64                          ACW_Word64;
    EDD_UPPER_MEM_U8_PTR_TYPE           User_Data_Ptr;
    LSA_UINT32                          Pnip_Addr;
    LSA_UINT32                          APICtrlRAMOffs;
    EDDP_CRT_CONSUMER_PTR_TYPE          pConsumer;

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTConsInitConsAcwHwApi(ConsumerID=%d)", ConsumerID);

    EDDP_ASSERT_FALSE(ConsumerID >= pDDB->CRT.CfgParam.ConsumerMaxNumber);
    EDDP_ASSERT_FALSE(pDDB->CRT.Cons.pAcwPtrArray[ConsumerID]->IsAcwContainer); // Container ACW not allowed here !!!

    pConsumer = &pDDB->CRT.Cons.pConsEntry[ConsumerID];
    EDDP_ASSERT_FALSE(pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_DFP_ITEM);         // ACW_DG used instead !!!

    /* --------------------------------------------------------------------------------------------------- */
    /* Word1:                                                                                              */
    /* FrameID, ACW_nextoffset, ACW_DG_offset, OpC, A, O, DOR, DO, E_sel, FID, DGM, CSA, RT, ACW_DG_order  */
    /* ^                                                                  ^         ^    ^                 */
    /* --------------------------------------------------------------------------------------------------- */

    // ***** Init value (MRT=off) -----------------------------------------------------------------------
    ACW_Word64 = (  ((LSA_UINT64)PNIP_R2_ACW_RCV_WORD1_ACW_NEXTOFFSET_INIT_VALUE<< PNIP_ACW_RCV_WORD1_ACW_NEXTOFFSET_SHIFT     )   |   /* ACW_nextoffset: Offset of next ACW in APICtrl-RAM (init ACW_nextoffset to begin as last ACW) */
                    ((LSA_UINT64)PNIP_R2_ACW_RCV_WORD1_ACW_DG_OFFSET_INIT_VALUE << PNIP_ACW_RCV_WORD1_ACW_DG_OFFSET_SHIFT      )   |   /* ACW_DG_offset: Offset of first ACW_DG in APICtrl-RAM which belongs to this ACW (init ACW_DG_offset to begin as last ACW) */
                    ((LSA_UINT64)PNIP_ACW_RCV_WORD1_OPCODE_ACW_VALUE         << PNIP_ACW_RCV_WORD1_OPCODE_SHIFT             )   |   /* Opcode for ACW Receive */
                    ((LSA_UINT64)PNIP_ACW_RCV_WORD1_ACTIVE_INIT_VALUE        << PNIP_ACW_RCV_WORD1_ACTIVE_SHIFT             )   |   /* Active Bit (Activate/Deactivate this ACW - will be set by command "ACW_on_off" (CONSUMER_CONTROL) */
                    ((LSA_UINT64)PNIP_ACW_RCV_WORD1_ONESHOT_BIT_INIT_VALUE   << PNIP_ACW_RCV_WORD1_ONESHOT_BIT_SHIFT        )   |   /* Oneshot-Bit: older-newer-detection */
                    ((LSA_UINT64)PNIP_ACW_RCV_WORD1_DP2_AFTER_RED_OFF_VALUE  << PNIP_ACW_RCV_WORD1_DP2_AFTER_RED_BIT_SHIFT  )   |   /* DP2_AFTER_RED */
                    ((LSA_UINT64)PNIP_ACW_RCV_WORD1_DP2_ONLY_OFF_VALUE       << PNIP_ACW_RCV_WORD1_DP2_ONLY_BIT_SHIFT       )   |   /* DP2_ONLY */
                    ((LSA_UINT64)PNIP_ACW_RCV_WORD1_ETHERTYPE_SELECT_INDEX0_VALUE << PNIP_ACW_RCV_WORD1_ETHERTYPE_SELECT_SHIFT) |   /* EtherType: Index 0-3 to select one of Register "Local_SPort_Etype_0-3" */
                    ((LSA_UINT64)PNIP_ACW_RCV_WORD1_FID_REDUNDANT_OFF_VALUE  << PNIP_ACW_RCV_WORD1_FID_REDUNDANT_SHIFT      )       /* FID_redundant: Bit (Activate/Deactivate the redundant frames) */
                 );

    // FID_redundant (if MRPD) and if FrameID(x+1) exists in PDIRData_A
    if (   (EDDP_IS_IRT_REDUNDANT_FRAMEID(pConsumer->Param.FrameID))
        && (EDDP_PRMPDIRDataIsRedundantFrameIDCoupleAvailable(pDDB, pConsumer->Param.FrameID, LSA_TRUE))
       )
    {
        // redundant: configure for FrameID: (x) and (x+1)
        EDDP_SET_BIT_VALUE64(ACW_Word64, PNIP_ACW_RCV_WORD1_FID_REDUNDANT_ON_VALUE, PNIP_ACW_RCV_WORD1_FID_REDUNDANT_MASK, PNIP_ACW_RCV_WORD1_FID_REDUNDANT_SHIFT);
    }

    // DP2_AFTER_RED
    if (pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_DP2ACW)
    {
        // ***** need observation for red frames (DP2)
        if (pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_2CR_MODE)
        {   // DP2Only=0, DP2only_set_after_RiDP=1 
            // --> listen to green and red frames
            // --> first frame enables ConsumerAutoStop
            // --> first red frame enables red only 
            EDDP_SET_BIT_VALUE64(ACW_Word64, PNIP_ACW_RCV_WORD1_DP2_AFTER_RED_ON_VALUE, PNIP_ACW_RCV_WORD1_DP2_AFTER_RED_BIT_MASK, PNIP_ACW_RCV_WORD1_DP2_AFTER_RED_BIT_SHIFT);
        }
        else
        {   // DP2Only=1    
            // --> listen to red frames only
            // --> first red frame enables ConsumerAutoStop
            EDDP_SET_BIT_VALUE64(ACW_Word64, PNIP_ACW_RCV_WORD1_DP2_ONLY_ON_VALUE, PNIP_ACW_RCV_WORD1_DP2_ONLY_BIT_MASK, PNIP_ACW_RCV_WORD1_DP2_ONLY_BIT_SHIFT);
        }
    }

    // ***** FrameID to be received ---------------------------------------------------------------------
    EDDP_SET_BIT_VALUE64(ACW_Word64, pConsumer->Param.FrameID, PNIP_ACW_RCV_WORD1_FRAMEID_MASK, PNIP_ACW_RCV_WORD1_FRAMEID_SHIFT);

    // ***** RT_class: RT-Frame or RToverUDP-Frame ------------------------------------------------------
    switch (pConsumer->Param.RTClass)
    {
        case EDDP_CRT_RTCLASS_RT:       // --> from service: EDD_SRV_SRT_CONSUMER_ADD
            // ***** Bit for info: RT_class = RT-Frame
            EDDP_SET_BIT_VALUE64(ACW_Word64, PNIP_ACW_RCV_WORD1_RT_CLASS_RT_VALUE, PNIP_ACW_RCV_WORD1_RT_CLASS_MASK, PNIP_ACW_RCV_WORD1_RT_CLASS_SHIFT);
            break;
        case EDDP_CRT_RTCLASS_UDP:      // --> from service: EDD_SRV_SRT_UDP_CONSUMER_ADD
            // ***** Bit for info: RT_class = RToverUDP-Frame
            EDDP_SET_BIT_VALUE64(ACW_Word64, PNIP_ACW_RCV_WORD1_RT_CLASS_UDP_VALUE, PNIP_ACW_RCV_WORD1_RT_CLASS_MASK, PNIP_ACW_RCV_WORD1_RT_CLASS_SHIFT);
            break;
        default:
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTConsInitConsAcwHwApi(): FATAL -> RTClass unknown! RTClass=0x%X", 
                pConsumer->Param.RTClass);
            EDDP_FATAL(EDDP_FATAL_ERR_SERVICE);
            break;
    }
    // ***** CSA: enable/disable check source address ---------------------------------------------------
    // ***** set this bit later in EDDP_CRTConsUpdateSrcAddrCsaInConsAcwHwApi()

    // ***** write HOST value to APICtrl-RAM ------------------------------------------------------------
    EDDP_HAL_MEM64_WRITE (pDDB, &((PNIP_ACW_RCV_PTR_TYPE)(pConsumer->ConsAcwTypeInfo.pHwAcwApi))->ACW_Word1, ACW_Word64);


    /* ----------------------------------------------------------------------------------------------- */
    /* Word2:                                                                                          */
    /* MAC_SA,                Length, IP_D_sel                                                         */
    /* S-IP-Addr, UDP-S-Port, Length, IP_D_sel                                                         */
    /* ^                      ^                                                                        */
    /* ----------------------------------------------------------------------------------------------- */

    // ***** Init value ---------------------------------------------------------------------------------
    ACW_Word64 = 0;
    // ***** MAC_SA / S-IP-Addr + UDP-S-Port ------------------------------------------------------------
    // ***** set SrcAddr later in EDDP_CRTConsUpdateSrcAddrCsaInConsAcwHwApi()

    // ***** DataLength: data length of using data between FrameID and APDU-Status ----------------------
    EDDP_SET_BIT_VALUE64(ACW_Word64, pConsumer->Param.DataLen, PNIP_ACW_RCV_WORD2_LENGTH_MASK, PNIP_ACW_RCV_WORD2_LENGTH_SHIFT);
    // ***** IP_DA_select -------------------------------------------------------------------------------
    EDDP_SET_BIT_VALUE64(ACW_Word64, PNIP_ACW_RCV_WORD2_IP_DA_SELECT_INDEX0_VALUE, PNIP_ACW_RCV_WORD2_IP_DA_SELECT_MASK, PNIP_ACW_RCV_WORD2_IP_DA_SELECT_SHIFT);
    // write HOST value to APICtrl-RAM ------------------------------------------------------------------
    EDDP_HAL_MEM64_WRITE(pDDB, &((PNIP_ACW_RCV_PTR_TYPE)(pConsumer->ConsAcwTypeInfo.pHwAcwApi))->ACW_Word2, ACW_Word64);

    // ***** set CSA and ScrIP+Port or ScrMAC in the ACW
    pDDB->CRT.Cons.AcwHandler.FctUpdateSrcAddrCsaAcwHwApi (pDDB, ConsumerID);

    /* ----------------------------------------------------------------------------------------------- */
    /* Word3:                                                                                          */
    /* ScoreBoard_offset, APDUStatus_offset, res, DC, WDR, DHTR, MRTR                                  */
    /* ^                  ^                           ^    ^                                           */
    /* ----------------------------------------------------------------------------------------------- */

    // ***** Init value ---------------------------------------------------------------------------------
    ACW_Word64 = (  ((LSA_UINT64)PNIP_R2_ACW_RCV_WORD3_SCOREBOARD_OFFSET_OFF_VALUE << PNIP_ACW_RCV_WORD3_SCOREBOARD_OFFSET_SHIFT) | /* tbd.: Scoreboard_offset: offset in APICtrl-RAM of struct Timer-Scoreboard */
                    ((LSA_UINT64)PNIP_R2_ACW_RCV_WORD3_APDUSTATUS_OFFSET_OFF_VALUE << PNIP_ACW_RCV_WORD3_APDUSTATUS_OFFSET_SHIFT) | /* tbd.: APDUStatus_offset: offset in APICtrl-RAM of APDU-Status */
                    ((LSA_UINT64)PNIP_ACW_RCV_WORD3_DP2_CHECK_OFF_VALUE         << PNIP_ACW_RCV_WORD3_DP2_CHECK_SHIFT        ) | /* DP2_CHECK */
                    ((LSA_UINT64)PNIP_ACW_RCV_WORD3_MR_RELOADVALUE_OFF_VALUE    << PNIP_ACW_RCV_WORD3_MR_RELOADVALUE_SHIFT   )   /* MR_ReloadValue of MediaRedundancy-Timer */
                 );

    // ***** ScoreBoard_offset --------------------------------------------------------------------------
    // APICtrlRAMOffs = Offset >> 3
    APICtrlRAMOffs = EDDP_CRTGetPnipAddrShift (pDDB, pConsumer->pHwTSBApi, EDDP_CRT_SHIFT_VALUE_3); // Address must be 64Bit aligned!
    EDDP_SET_BIT_VALUE64(ACW_Word64, APICtrlRAMOffs, PNIP_R2_ACW_RCV_WORD3_SCOREBOARD_OFFSET_MASK, PNIP_ACW_RCV_WORD3_SCOREBOARD_OFFSET_SHIFT);
    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTConsInitConsAcwHwApi(): SB_OFFS=0x%X",
        (APICtrlRAMOffs << EDDP_CRT_SHIFT_VALUE_3));

    // ***** APDUStatus_offset (Bit 0,1 shifted out) ----------------------------------------------------
    // APICtrlRAMOffs = Offset >> 2
    APICtrlRAMOffs = EDDP_CRTGetPnipAddrShift (pDDB, pConsumer->ConsAcwTypeInfo.pHwApduApi, EDDP_CRT_SHIFT_VALUE_2); // Address must be 32Bit aligned!
    EDDP_SET_BIT_VALUE64(ACW_Word64, APICtrlRAMOffs, PNIP_R2_ACW_RCV_WORD3_APDUSTATUS_OFFSET_MASK, PNIP_ACW_RCV_WORD3_APDUSTATUS_OFFSET_SHIFT);
    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTConsInitConsAcwHwApi(): APDU_OFFS=0x%X", 
        (APICtrlRAMOffs << EDDP_CRT_SHIFT_VALUE_2));

    // ***** Phase_check: monitoring of Consumer-Scoreboard ON/OFF --------------------------------------
    if (pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_DP2ACW)
    {
        EDDP_SET_BIT_VALUE64(ACW_Word64, PNIP_ACW_RCV_WORD3_DP2_CHECK_ON_VALUE, PNIP_ACW_RCV_WORD3_DP2_CHECK_MASK, PNIP_ACW_RCV_WORD3_DP2_CHECK_SHIFT);
    }

    // ***** WD_ReloadValue of Watchdog-Timer -----------------------------------------------------------
    EDDP_SET_BIT_VALUE64(ACW_Word64, EDDP_CRT_CONSUMER_WDT_OFF, PNIP_ACW_RCV_WORD3_WD_RELOADVALUE_MASK, PNIP_ACW_RCV_WORD3_WD_RELOADVALUE_SHIFT);
    // ***** DH_ReloadValue of DataHold-Time ------------------------------------------------------------
    //old    EDDP_SET_BIT_VALUE64( ACW_Word64, (pConsumer->Param.DataHoldFactor+1), PNIP_ACW_RCV_WORD3_DH_RELOADVALUE_MASK, PNIP_ACW_RCV_WORD3_DH_RELOADVALUE_SHIFT);
    EDDP_SET_BIT_VALUE64(ACW_Word64, (pConsumer->Param.HwDataHoldFactor), PNIP_ACW_RCV_WORD3_DH_RELOADVALUE_MASK, PNIP_ACW_RCV_WORD3_DH_RELOADVALUE_SHIFT);
    // ***** MR_ReloadValue of MR-Time ------------------------------------------------------------
    EDDP_SET_BIT_VALUE64(ACW_Word64, (pConsumer->Param.MediaRedFactor), PNIP_ACW_RCV_WORD3_MR_RELOADVALUE_MASK, PNIP_ACW_RCV_WORD3_MR_RELOADVALUE_SHIFT);
    // ***** write HOST value to APICtrl-RAM ------------------------------------------------------------
    EDDP_HAL_MEM64_WRITE(pDDB, &((PNIP_ACW_RCV_PTR_TYPE)(pConsumer->ConsAcwTypeInfo.pHwAcwApi))->ACW_Word3, ACW_Word64);

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTConsInitConsAcwHwApi(): DHT=%d", pConsumer->Param.HwDataHoldFactor);

    /* ----------------------------------------------------------------------------------------------- */
    /* Word4:                                                                                          */
    /* Data-Ptr, DataStart_offset, DataLength, CT, EZD, CR_N, BM, AppG                                 */
    /* ^         ^                 ^           ^   ^    ^     ^   ^                                    */
    /* ----------------------------------------------------------------------------------------------- */

    // ***** Init value ---------------------------------------------------------------------------------
    ACW_Word64 = 0;
    // ***** Data_Ptr -----------------------------------------------------------------------------------
    // to prevent compiler warning
    User_Data_Ptr   = 0;
    Pnip_Addr       = EDDP_CRT_INVALID_VALUE_32;

    if (pConsumer->Param.DataOffset == EDD_DATAOFFSET_UNDEFINED)
    {
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTConsInitConsAcwHwApi(): FATAL -> DataOffset=UNDEFINED!");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {        
        if (pConsumer->Param.CRNumber == EDD_CRNUMBER_NOT_USED)
        {
            // use extarnel RAM
            EDDP_ASSERT_NO_NULL_PTR(pDDB->CRT.pIOBaseAdrExtRam);
            User_Data_Ptr = ((pDDB->CRT.pIOBaseAdrExtRam) + (pConsumer->Param.DataOffset));
            // ***** convert upper memory pointer to 32-Bit physical pnip address
            EDDP_IO_EXTRAM_ADDR_UPPER_TO_PNIP (pDDB->hSysDev, User_Data_Ptr, &Pnip_Addr);
        }
        else
        {
            // use PERIF
            EDDP_ASSERT_NO_NULL_PTR(pDDB->CRT.pIOBaseAdrPerif);
            User_Data_Ptr = ((pDDB->CRT.pIOBaseAdrPerif) + (pConsumer->Param.DataOffset));
            // ***** convert upper memory pointer to 32-Bit physical pnip address
            EDDP_IO_PERIF_ADDR_UPPER_TO_PNIP (pDDB->hSysDev, User_Data_Ptr, &Pnip_Addr);
        }
    }

    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTConsInitConsAcwHwApi(): CRNumber=%d, DataOffset(Host=0x%X, PNIP=0x%X)", 
        pConsumer->Param.CRNumber, pConsumer->Param.DataOffset, Pnip_Addr);

    EDDP_SET_BIT_VALUE64(ACW_Word64, Pnip_Addr, PNIP_R2_ACW_RCV_WORD4_DATA_PTR_MASK, PNIP_R2_ACW_RCV_WORD4_DATA_PTR_SHIFT);
    // ***** DataStart_offset ---------------------------------------------------------------------------
    EDDP_SET_BIT_VALUE64(ACW_Word64, pConsumer->Param.Partial_DataOffset, PNIP_ACW_RCV_WORD4_DATASTART_OFFSET_MASK, PNIP_ACW_RCV_WORD4_DATASTART_OFFSET_SHIFT);
    // ***** DataLength ---------------------------------------------------------------------------------
    EDDP_SET_BIT_VALUE64(ACW_Word64, pConsumer->Param.Partial_DataLen, PNIP_ACW_RCV_WORD4_DATALENGTH_MASK, PNIP_ACW_RCV_WORD4_DATALENGTH_SHIFT);
    // ***** CT_to_RWB: Cut-Through-Mode for this ACW ---------------------------------------------------
    if (pConsumer->Param.ForwardMode == EDD_FORWARDMODE_CT)
    {
        EDDP_CRTConsumerSetCtRwb (pDDB, pConsumer);
        EDDP_SET_BIT_VALUE64(ACW_Word64, 0x1, PNIP_R2_ACW_RCV_WORD4_CT_TO_RWB_MASK, PNIP_R2_ACW_RCV_WORD4_CT_TO_RWB_SHIFT);
    }
    else
    {
        EDDP_SET_BIT_VALUE64(ACW_Word64, 0x0, PNIP_R2_ACW_RCV_WORD4_CT_TO_RWB_MASK, PNIP_R2_ACW_RCV_WORD4_CT_TO_RWB_SHIFT);
    }

    // ***** EnableZeroData: ON/OFF ---------------------------------------------------------------------
    if ( (EDDP_CRT_IS_CRNUMBER_IN_PERIF_RANGE(pConsumer->Param.CRNumber)) && (pConsumer->Param.ClearOnMISS) )
    {   // ***** can be used for PerIf only --> CRNumber: 1..27 !!!
        EDDP_SET_BIT_VALUE64(ACW_Word64, PNIP_ACW_RCV_WORD4_EN_ZERO_DATA_ON_VALUE, PNIP_ACW_RCV_WORD4_EN_ZERO_DATA_MASK, PNIP_ACW_RCV_WORD4_EN_ZERO_DATA_SHIFT);
    }
    // ***** CRNumber -----------------------------------------------------------------------------------
    EDDP_SET_BIT_VALUE64(ACW_Word64, pConsumer->Param.CRNumber, PNIP_R2_ACW_RCV_WORD4_CR_NUMBER_MASK, PNIP_R2_ACW_RCV_WORD4_CR_NUMBER_SHIFT);

    // ***** Buffer_Mode (only used in PN-IP Rev1) ------------------------------------------------------
    #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
    if (EDDP_HAL_HW_PNIP_REV1 == EDDP_HALGetPnipRevision(pDDB))
    {
        EDDP_SET_BIT_VALUE64(ACW_Word64, PNIP_ACW_RCV_WORD4_BUFFER_MODE_NORMAL_VALUE, PNIP_ACW_RCV_WORD4_BUFFER_MODE_MASK, PNIP_ACW_RCV_WORD4_BUFFER_MODE_SHIFT);
    }
    #endif
    
    // ***** AppGroup -----------------------------------------------------------------------------------
    EDDP_SET_BIT_VALUE64(ACW_Word64, pConsumer->Param.AppGroup, PNIP_ACW_RCV_WORD4_APP_GROUP_MASK, PNIP_ACW_RCV_WORD4_APP_GROUP_SHIFT);
    // ***** write HOST value to APICtrl-RAM ------------------------------------------------------------
    EDDP_HAL_MEM64_WRITE(pDDB, &((PNIP_ACW_RCV_PTR_TYPE)(pConsumer->ConsAcwTypeInfo.pHwAcwApi))->ACW_Word4, ACW_Word64);

    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    /* ACW.CpmSwData --> some meta data for EDDP SW only !!!                                           */
    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    EDDP_HAL_MEM16_WRITE(pDDB, &((PNIP_ACW_RCV_PTR_TYPE)(pConsumer->ConsAcwTypeInfo.pHwAcwApi))->CpmSwData.AcwId, ConsumerID);

    /* ----------------------------------------------------------------------------------------------- */
    /* Note: TSB and CSB will be init on activation of Consumer                                        */
    /* ----------------------------------------------------------------------------------------------- */
}
#endif

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTConsDeInitConsAcwHwApi                          +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                        +*/
/*+                             LSA_UINT16                  ConsumerID                  +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ConsumerID     : Index in consumer array                                           +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function clears the ACW + ScoreBoard of consumer in APICtrl-RAM  +*/
/*+                                                                                     +*/
/*+               Consumer must be used in CRT!                                         +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsDeInitConsAcwHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ConsumerID)
{
    EDD_RSP                         Response;
    LSA_UINT32                      RamSubTsbCtrl;
    LSA_UINT32                      RamSubCsb0Ctrl;
    LSA_UINT32                      AdrTemp1, AdrTemp2;
    LSA_UINT32                      IntOffs;
    LSA_UINT16                      HwAcwSize;
    EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer;

    EDDP_ASSERT_FALSE(ConsumerID >= pDDB->CRT.CfgParam.ConsumerMaxNumber);
    // ***** Container ACW is not allowed here --> ConsumerID as IDX too high !!!
    EDDP_ASSERT_FALSE(pDDB->CRT.Cons.pAcwPtrArray[ConsumerID]->IsAcwContainer); 

    HwAcwSize = pDDB->CRT.CfgHwAllocParamApi.HWCpmAcwSize;
    pConsumer = &pDDB->CRT.Cons.pConsEntry[ConsumerID];

    // ***** Note: we must tolerate here with flag EDDP_CONS_FLAG_IS_DFP_ITEM, because the DG uses 
    //             the TSB/CSB from this SW consumer.  --> all HW pointers are valid so we can run MemSet below
    Response        = EDD_STS_OK;
    RamSubTsbCtrl   = EDDP_CRT_INVALID_VALUE_32;    // init to prevent compiler warning
    RamSubCsb0Ctrl  = EDDP_CRT_INVALID_VALUE_32;
//    RamSubCsb1Ctrl  = EDDP_CRT_INVALID_VALUE_32;

    RamSubTsbCtrl   = EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_CONS_TSB;     // IFA
    RamSubCsb0Ctrl  = EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_CONS_CSB0;    // IFA
//    RamSubCsb1Ctrl  = EDDP_HAL_RAMSUB_APICTRL_CRT_IFB_CONS_CSB1;    // IFA

    // ***** pAcwApi (clear field for given ConsumerID only)
    AdrTemp1 = (LSA_UINT32) pDDB->CRT.Cons.pHwBaseAcwApi;
    AdrTemp2 = (LSA_UINT32) pConsumer->ConsAcwTypeInfo.pHwAcwApi;

    EDDP_ASSERT_FALSE(AdrTemp1 > AdrTemp2);
    IntOffs = AdrTemp2 - AdrTemp1;
    Response = EDDP_HALMemset (pDDB, EDDP_HAL_RAMTYPE_API_CTRL, EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_CONS_ACW, IntOffs, HwAcwSize, 0);
    EDDP_ASSERT_FALSE(Response != EDD_STS_OK);

    // ***** pApduApi (clear field for given ConsumerID only)
    AdrTemp1 = (LSA_UINT32) pDDB->CRT.Cons.pHwBaseApduApi;
    AdrTemp2 = (LSA_UINT32) pConsumer->ConsAcwTypeInfo.pHwApduApi;

    EDDP_ASSERT_FALSE(AdrTemp1 > AdrTemp2);
    IntOffs = AdrTemp2 - AdrTemp1;
    Response = EDDP_HALMemset (pDDB, EDDP_HAL_RAMTYPE_API_CTRL, EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_CONS_APDU, IntOffs, (sizeof(PNIP_ADPU_RCV_TYPE)), 0);

    EDDP_ASSERT_FALSE(Response != EDD_STS_OK);

    // ***** pTSBApi (clear field for given ConsumerID only)
    AdrTemp1 = (LSA_UINT32) pDDB->CRT.Cons.pHwBaseTSBApi;
    AdrTemp2 = (LSA_UINT32) pConsumer->pHwTSBApi;

    EDDP_ASSERT_FALSE(AdrTemp1 > AdrTemp2);
    IntOffs = AdrTemp2 - AdrTemp1;
    Response = EDDP_HALMemset (pDDB, EDDP_HAL_RAMTYPE_API_CTRL, RamSubTsbCtrl, IntOffs, (sizeof(PNIP_CPM_TSB_TYPE)), 0);
    EDDP_ASSERT_FALSE(Response != EDD_STS_OK);

    // ***** pCSB0Api (clear field for given ConsumerID only)
    AdrTemp1 = (LSA_UINT32) pDDB->CRT.Cons.pHwBaseCSB0Api;
    AdrTemp2 = (LSA_UINT32) pConsumer->pHwCSB0Api;

    EDDP_ASSERT_FALSE(AdrTemp1 > AdrTemp2);
    IntOffs = AdrTemp2 - AdrTemp1;
    Response = EDDP_HALMemset (pDDB, EDDP_HAL_RAMTYPE_API_CTRL, RamSubCsb0Ctrl, IntOffs, (sizeof(PNIP_CPM_CSB_TYPE)), 0);

    EDDP_ASSERT_FALSE(Response != EDD_STS_OK);

    // ***** pCSB1Api (clear field for given ConsumerID only)
    AdrTemp1 = (LSA_UINT32) pDDB->CRT.Cons.pHwBaseCSB1Api;
    AdrTemp2 = (LSA_UINT32) pConsumer->pHwCSB1Api;

    EDDP_ASSERT_FALSE(AdrTemp1 > AdrTemp2);

    EDDP_ASSERT_FALSE(Response != EDD_STS_OK);
}





#if defined (EDDP_CFG_HW_ERTEC200P_SUPPORT)
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTConsBasicInitConsAcwDgHwApi                     +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT16                      DgID                    +*/
/*+                             LSA_UINT16                      NextOffs_0              +*/
/*+                             LSA_UINT16                      NextOffs_1              +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  DgID           : Index in consumer DG array                                        +*/
/*+  NextOffs_0     : linking (for DFP)                                                 +*/
/*+  NextOffs_1     : linking (for DFP)                                                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Init the receive ACW_DG in ApiCtrlRAM for CRT-DFP-Communication with  +*/
/*+               basic settings.                                                       +*/
/*+                                                                                     +*/
/*+               Call this function before EDDP_CRTConsFinalInitConsAcwDgHwApi() !!!   +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsBasicInitConsAcwDgHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  DgID,
    LSA_UINT32                  NextOffs_0,
    LSA_UINT32                  NextOffs_1)
{
    LSA_UINT64                      ACW_Word64;
    LSA_UINT32                      DgStatusOffs;
    EDDP_CRT_CONSUMER_DG_PTR_TYPE   pDgEntry;

    EDDP_ASSERT_FALSE(DgID >= pDDB->CRT.CfgParam.ConsumerDGMaxNumber);
    pDgEntry = &pDDB->CRT.Cons.pConsDgEntry[DgID];
    EDDP_ASSERT_NO_NULL_PTR(pDgEntry->pSubFrameData);

    // Word1:  -, O, A, OpC, DgNextOffs1, DgNextOffs0, DgLen, -, DgPos
    // ***** Init value ---------------------------------------------------------------------------------
    ACW_Word64 = (  ((LSA_UINT64)PNIP_ACWDG_RCV_WORD1_OPCODE_ACW_DG_VALUE           << PNIP_ACWDG_RCV_WORD1_OPCODE_SHIFT                ) |   // OPCODE = 1
                    ((LSA_UINT64)PNIP_ACWDG_RCV_WORD1_ACTIVE_INIT_VALUE             << PNIP_ACWDG_RCV_WORD1_ACTIVE_SHIFT                ) |   // ACTIVE = 0
                    ((LSA_UINT64)PNIP_ACWDG_RCV_WORD1_ONESHOT_INIT_VALUE            << PNIP_ACWDG_RCV_WORD1_ONESHOT_BIT_SHIFT           )     // ONESHOT_BIT = 0
                 );
    EDDP_SET_BIT_VALUE64( ACW_Word64, NextOffs_0,                          PNIP_R2_ACWDG_RCV_WORD1_ACW_DG_NEXT_OFFSET0_MASK,   PNIP_ACWDG_RCV_WORD1_ACW_DG_NEXT_OFFSET0_SHIFT);    // NEXT_OFFSET0
    EDDP_SET_BIT_VALUE64( ACW_Word64, NextOffs_1,                          PNIP_R2_ACWDG_RCV_WORD1_ACW_DG_NEXT_OFFSET1_MASK,   PNIP_ACWDG_RCV_WORD1_ACW_DG_NEXT_OFFSET1_SHIFT);    // NEXT_OFFSET1
    EDDP_SET_BIT_VALUE64( ACW_Word64, pDgEntry->pSubFrameData->Position,   PNIP_ACWDG_RCV_WORD1_DG_POS_T_MASK,                 PNIP_ACWDG_RCV_WORD1_DG_POS_T_SHIFT);               // DG_POS_T
    EDDP_SET_BIT_VALUE64( ACW_Word64, pDgEntry->pSubFrameData->DataLength, PNIP_ACWDG_RCV_WORD1_DG_LEN_MASK,                   PNIP_ACWDG_RCV_WORD1_DG_LEN_SHIFT);                 // DG_LEN
    // ***** write HOST value to APICtrl-RAM ------------------------------------------------------------
    EDDP_HAL_MEM64_WRITE(pDDB, &((PNIP_ACW_DG_RCV_PTR_TYPE)(pDgEntry->pHwAcwDgApi))->ACWDG_Word1, ACW_Word64);

    // Word2:  Mrt, Dht, Wdt, DC,-, DgStatusOffs, ScoreBoardOffs
    // ***** Init value ---------------------------------------------------------------------------------
    ACW_Word64 = (  ((LSA_UINT64)PNIP_R2_ACWDG_RCV_WORD2_SCOREBOARD_OFFSET_OFF_VALUE   << PNIP_ACWDG_RCV_WORD2_SCOREBOARD_OFFSET_SHIFT     ) |   // SCOREBOARD_OFFSET = FFFF
                    ((LSA_UINT64)PNIP_ACWDG_RCV_WORD2_DP2_CHECK_ON_VALUE            << PNIP_ACWDG_RCV_WORD2_DP2_CHECK_SHIFT             ) |   // DP2_CHECK = 1
                    ((LSA_UINT64)PNIP_ACWDG_RCV_WORD2_WD_RELOADVALUE_OFF_VALUE      << PNIP_ACWDG_RCV_WORD2_WD_RELOADVALUE_SHIFT        ) |   // WDT = FF
                    ((LSA_UINT64)PNIP_ACWDG_RCV_WORD2_MR_RELOADVALUE_OFF_VALUE      << PNIP_ACWDG_RCV_WORD2_MR_RELOADVALUE_SHIFT        )     // MRT = FF
                 );
    DgStatusOffs = EDDP_CRTGetPnipAddrShift(pDDB, pDgEntry->pHwDgStatusApi, EDDP_CRT_SHIFT_VALUE_1); // Address must be 16Bit aligned!
    EDDP_SET_BIT_VALUE64( ACW_Word64, DgStatusOffs, PNIP_R2_ACWDG_RCV_WORD2_DG_STATUS_OFFSET_MASK,   PNIP_ACWDG_RCV_WORD2_DG_STATUS_OFFSET_SHIFT);    // position of DG_Status
    // ***** write HOST value to APICtrl-RAM ------------------------------------------------------------
    EDDP_HAL_MEM64_WRITE(pDDB, &((PNIP_ACW_DG_RCV_PTR_TYPE)(pDgEntry->pHwAcwDgApi))->ACWDG_Word2, ACW_Word64);

    // Word3:  AppG, BM, CrNr, EZD, CT, -, DataLen, -, DataStartOffs, DataPtr
    // ***** Init value ---------------------------------------------------------------------------------
    ACW_Word64 = 0;
    // ***** write HOST value to APICtrl-RAM ------------------------------------------------------------
    EDDP_HAL_MEM64_WRITE(pDDB, &((PNIP_ACW_DG_RCV_PTR_TYPE)(pDgEntry->pHwAcwDgApi))->ACWDG_Word3, ACW_Word64);

    /* ----------------------------------------------------------------------------------------------- */
    /* ACWDG.CpmSwData --> some meta data for EDDP SW only !!!                                         */
    /* ----------------------------------------------------------------------------------------------- */
    EDDP_HAL_MEM16_WRITE(pDDB, &((PNIP_ACW_DG_RCV_PTR_TYPE)(pDgEntry->pHwAcwDgApi))->CpmSwData.AcwId, DgID);

    // ***** write HOST value to APICtrl-RAM ------------------------------------------------------------
    EDDP_HAL_MEM8_WRITE(pDDB, &pDgEntry->pHwDgStatusApi->CycleCounter, 0x00);
    // ***** write HOST value to APICtrl-RAM ------------------------------------------------------------
    EDDP_HAL_MEM8_WRITE(pDDB, &pDgEntry->pHwDgStatusApi->DataStatus, 0x80);   // surrogat
}
#endif


#if defined (EDDP_CFG_HW_ERTEC200P_SUPPORT)
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTConsFinalInitConsAcwDgHwApi                     +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer               +*/
/*+                             EDDP_CRT_CONSUMER_DG_PTR_TYPE   pDgEntry                +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  pConsumer      : Pointer to consumer                                               +*/
/*+  pDgEntry       : Pointer to connected DG entry                                     +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Setup the receive ACW_DG in ApiCtrlRAM for CRT-DFP-Communication with +*/
/*+               final settings.                                                       +*/
/*+                                                                                     +*/
/*+               Call this function after EDDP_CRTConsBasicInitConsAcwDgHwApi() !!!    +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsFinalInitConsAcwDgHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer,
    EDDP_CRT_CONSUMER_DG_PTR_TYPE   pDgEntry)
{
    LSA_UINT64                      ACW_Word64;
    LSA_UINT32                      APICtrlRAMOffs;
    EDD_UPPER_MEM_U8_PTR_TYPE       User_Data_Ptr;
    LSA_UINT32                      Pnip_Addr;

    EDDP_ASSERT_NO_NULL_PTR(pConsumer);
    EDDP_ASSERT_NO_NULL_PTR(pDgEntry);

    // ***** nothing to do for ACWDG_Word1

    // ***** update: MRT, DHT, ScoreBoardoffset
    ACW_Word64 = EDDP_HAL_MEM64_READ(pDDB, &((PNIP_ACW_DG_RCV_PTR_TYPE)(pDgEntry->pHwAcwDgApi))->ACWDG_Word2);
    EDDP_SET_BIT_VALUE64(ACW_Word64, pConsumer->Param.MediaRedFactor,   PNIP_ACWDG_RCV_WORD2_MR_RELOADVALUE_MASK, PNIP_ACWDG_RCV_WORD2_MR_RELOADVALUE_SHIFT);
    EDDP_SET_BIT_VALUE64(ACW_Word64, pConsumer->Param.HwDataHoldFactor, PNIP_ACWDG_RCV_WORD2_DH_RELOADVALUE_MASK, PNIP_ACWDG_RCV_WORD2_DH_RELOADVALUE_SHIFT);

    // APICtrlRAMOffs = 0xFFFF;
    // ***** ScoreBoard_offset --------------------------------------------------------------------------
    // APICtrlRAMOffs = Offset >> 3
    APICtrlRAMOffs = EDDP_CRTGetPnipAddrShift (pDDB, pConsumer->pHwTSBApi, EDDP_CRT_SHIFT_VALUE_3); // Address must be 64Bit aligned!
    EDDP_SET_BIT_VALUE64(ACW_Word64, APICtrlRAMOffs, PNIP_R2_ACWDG_RCV_WORD2_SCOREBOARD_OFFSET_MASK, PNIP_ACWDG_RCV_WORD2_SCOREBOARD_OFFSET_SHIFT);

    // ***** write HOST value to APICtrl-RAM ------------------------------------------------------------
    EDDP_HAL_MEM64_WRITE(pDDB, &((PNIP_ACW_DG_RCV_PTR_TYPE)(pDgEntry->pHwAcwDgApi))->ACWDG_Word2, ACW_Word64);

    // ***** update: AppG, CrNr, ENZ, CT, DataLen, StartOffs, DataPtr
    ACW_Word64 = EDDP_HAL_MEM64_READ(pDDB, &((PNIP_ACW_DG_RCV_PTR_TYPE)(pDgEntry->pHwAcwDgApi))->ACWDG_Word3);

    // ***** CT_to_RWB: Cut-Through-Mode for this ACW ---------------------------------------------------
    if (pConsumer->Param.ForwardMode == EDD_FORWARDMODE_CT)
    {
        EDDP_CRTConsumerSetCtRwb(pDDB, pConsumer);
        EDDP_SET_BIT_VALUE64(ACW_Word64, 0x1, PNIP_R2_ACWDG_RCV_WORD3_CT_TO_RWB_MASK, PNIP_R2_ACWDG_RCV_WORD3_CT_TO_RWB_SHIFT);
    }
    else
    {
        EDDP_SET_BIT_VALUE64(ACW_Word64, 0x0, PNIP_R2_ACWDG_RCV_WORD3_CT_TO_RWB_MASK, PNIP_R2_ACWDG_RCV_WORD3_CT_TO_RWB_SHIFT);
    }

    // ***** AppGroup -----------------------------------------------------------------------------------
    EDDP_SET_BIT_VALUE64(ACW_Word64, pConsumer->Param.AppGroup, PNIP_R2_ACWDG_RCV_WORD3_APP_GROUP_MASK, PNIP_R2_ACWDG_RCV_WORD3_APP_GROUP_SHIFT);
    // ***** CRNumber -----------------------------------------------------------------------------------
    EDDP_SET_BIT_VALUE64(ACW_Word64, pConsumer->Param.CRNumber, PNIP_R2_ACWDG_RCV_WORD3_CR_NUMBER_MASK, PNIP_R2_ACWDG_RCV_WORD3_CR_NUMBER_SHIFT);
   
    // ***** Partial_DataLen ----------------------------------------------------------------------------
    EDDP_SET_BIT_VALUE64(ACW_Word64, pConsumer->Param.Partial_DataLen, PNIP_ACWDG_RCV_WORD3_DATALENGTH_MASK, PNIP_ACWDG_RCV_WORD3_DATALENGTH_SHIFT);
    // ***** Partial_DataOffset -------------------------------------------------------------------------
    EDDP_SET_BIT_VALUE64(ACW_Word64, pConsumer->Param.Partial_DataOffset, PNIP_ACWDG_RCV_WORD3_DATASTART_OFFSET_MASK, PNIP_ACWDG_RCV_WORD3_DATASTART_OFFSET_SHIFT);

    // ***** Data_Ptr -----------------------------------------------------------------------------------
    EDDP_ASSERT_FALSE(pConsumer->Param.DataOffset == EDD_DATAOFFSET_UNDEFINED)
    if (pConsumer->Param.CRNumber == EDD_CRNUMBER_NOT_USED)
    {   // ***** use extarnel RAM
        EDDP_ASSERT_NO_NULL_PTR(pDDB->CRT.pIOBaseAdrExtRam);
        User_Data_Ptr = ((pDDB->CRT.pIOBaseAdrExtRam) + (pConsumer->Param.DataOffset));
        // ***** convert upper memory pointer to 32-Bit physical pnip address
        EDDP_IO_EXTRAM_ADDR_UPPER_TO_PNIP (pDDB->hSysDev, User_Data_Ptr, &Pnip_Addr);
    }
    else
    {   // ***** use PERIF
        EDDP_ASSERT_NO_NULL_PTR(pDDB->CRT.pIOBaseAdrPerif);
        User_Data_Ptr = ((pDDB->CRT.pIOBaseAdrPerif) + (pConsumer->Param.DataOffset));
        // ***** convert upper memory pointer to 32-Bit physical pnip address
        EDDP_IO_PERIF_ADDR_UPPER_TO_PNIP (pDDB->hSysDev, User_Data_Ptr, &Pnip_Addr);
    }
    EDDP_SET_BIT_VALUE64(ACW_Word64, Pnip_Addr, PNIP_R2_ACWDG_RCV_WORD3_DATA_PTR_MASK, PNIP_R2_ACWDG_RCV_WORD3_DATA_PTR_SHIFT);

    // ***** write HOST value to APICtrl-RAM ------------------------------------------------------------
    EDDP_HAL_MEM64_WRITE(pDDB, &((PNIP_ACW_DG_RCV_PTR_TYPE)(pDgEntry->pHwAcwDgApi))->ACWDG_Word3, ACW_Word64);
}
#endif

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTConsDeInitConsAcwDgHwApi                        +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                        +*/
/*+                             LSA_UINT16                  DgID                        +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  DgID           : Index in consumer DG array                                        +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: DeInit the receive ACW_DG in ApiCtrlRAM (memset with 0).              +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsDeInitConsAcwDgHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  DgID)
{
    EDD_RSP                         Response;
    LSA_UINT32                      AdrTemp1, AdrTemp2;
    LSA_UINT32                      IntOffs;
    LSA_UINT16                      HwAcwDgSize;
    EDDP_CRT_CONSUMER_DG_PTR_TYPE   pDgEntry;

    EDDP_ASSERT_FALSE(DgID >= pDDB->CRT.CfgParam.ConsumerDGMaxNumber);
    HwAcwDgSize = pDDB->CRT.CfgHwAllocParamApi.HWCpmAcwDgSize;
    pDgEntry = &pDDB->CRT.Cons.pConsDgEntry[DgID];

    // ***** pAcwDgApi (clear field for given ConsDgID only)
    AdrTemp1 = (LSA_UINT32) pDDB->CRT.Cons.pHwBaseAcwDgApi;
    AdrTemp2 = (LSA_UINT32) pDgEntry->pHwAcwDgApi;

    EDDP_ASSERT_FALSE(AdrTemp1 > AdrTemp2);
    IntOffs     = AdrTemp2 - AdrTemp1;
    Response    = EDDP_HALMemset (pDDB, EDDP_HAL_RAMTYPE_API_CTRL, EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_CONS_ACW_DG, IntOffs, HwAcwDgSize, 0);
    EDDP_ASSERT_FALSE(Response != EDD_STS_OK)

    // ***** pAcwDgApi (clear field for given ConsDgID only)
    AdrTemp1 = (LSA_UINT32) pDDB->CRT.Cons.pHwBaseDgStatusApi;
    AdrTemp2 = (LSA_UINT32) pDgEntry->pHwDgStatusApi;

    EDDP_ASSERT_FALSE(AdrTemp1 > AdrTemp2);
    IntOffs     = AdrTemp2 - AdrTemp1;
    Response    = EDDP_HALMemset (pDDB, EDDP_HAL_RAMTYPE_API_CTRL, EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_CONS_DG_STATUS, IntOffs, (sizeof(PNIP_DG_STATUS_RCV_TYPE)), 0);
    EDDP_ASSERT_FALSE(Response != EDD_STS_OK)
}





#if defined (EDDP_CFG_HW_ERTEC200P_SUPPORT)
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTConsInitContainerAcwHwApi                       +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                        +*/
/*+                             LSA_UINT16                  AcwID                       +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  AcwID          : Index into container array (ContainerID + ConsumerMax !!!)        +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Init the receive ACW in ApiCtrlRAM for CRT-Communication from PRM     +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsInitContainerAcwHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID)
{
    LSA_UINT64                              ACW_Word64;
    LSA_UINT16                              AcwMaxNumber;
    LSA_UINT32                              APICtrlRAMOffs;
    EDDP_CRT_CONSUMER_CONTAINER_PTR_TYPE    pConsContainerEntry;
    EDDP_CRT_CONS_ACW_INFO_PTR_TYPE         pAcwPtr;
    EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE         pPDIRFrameData;
    LSA_UINT32                              DgOffset, DgId;

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTConsInitContainerAcwHwApi(AcwID=%d):", AcwID);

    AcwMaxNumber    = pDDB->CRT.CfgParam.ConsumerMaxNumber;
    AcwMaxNumber   += pDDB->CRT.CfgParam.ConsumerContainerMaxNumber;
    EDDP_ASSERT_FALSE(AcwID >= AcwMaxNumber);

    pAcwPtr = pDDB->CRT.Cons.pAcwPtrArray[AcwID];
    EDDP_ASSERT_FALSE(!(pAcwPtr->IsAcwContainer));

    pConsContainerEntry = (EDDP_CRT_CONSUMER_CONTAINER_PTR_TYPE) pAcwPtr;
    pPDIRFrameData      = pConsContainerEntry->pPDIRFrameData;
    EDDP_ASSERT_NO_NULL_PTR(pPDIRFrameData);

    // ***** link from container to first DG (step direction is: UP or DOWN)
    DgId        = pConsContainerEntry->ConsDgList.BeginIdx;
    EDDP_ASSERT_FALSE(DgId == EDDP_CRT_CHAIN_IDX_END);
    DgOffset    = EDDP_CRTGetPnipAddrShift (pDDB, pDDB->CRT.Cons.pConsDgEntry[DgId].pHwAcwDgApi, EDDP_CRT_SHIFT_VALUE_3); // Address must be 64Bit aligned!

    /* --------------------------------------------------------------------------------------------------- */
    /* Word1:                                                                                              */
    /* FrameID, ACW_nextoffs, ACW_DG_offs, OpC, A, O, DOR, DO, E_sel, FID, DGM, CSA, RT, ACW_DG_order, DWC */
    /* ^                      ^            =0                         ^         =0       ^             ^   */
    /* --------------------------------------------------------------------------------------------------- */

    // ***** Init value ---------------------------------------------------------------------------------
    {   // ***** normal: MRT=off
        ACW_Word64 = ( ((LSA_UINT64)PNIP_R2_ACW_RCV_WORD1_ACW_NEXTOFFSET_INIT_VALUE<< PNIP_ACW_RCV_WORD1_ACW_NEXTOFFSET_SHIFT     )    |   /* ACW_nextoffset: Offset of next ACW in APICtrl-RAM (init ACW_nextoffset to begin as last ACW) */
                       ((LSA_UINT64)PNIP_R2_ACW_RCV_WORD1_ACW_DG_OFFSET_INIT_VALUE << PNIP_ACW_RCV_WORD1_ACW_DG_OFFSET_SHIFT      )    |   /* ACW_DG_offset: Offset of first ACW_DG in APICtrl-RAM which belongs to this ACW (init ACW_DG_offset to begin as last ACW) */
                       ((LSA_UINT64)PNIP_ACW_RCV_WORD1_OPCODE_ACW_VALUE         << PNIP_ACW_RCV_WORD1_OPCODE_SHIFT             )    |   /* Opcode for ACW Receive */
                       ((LSA_UINT64)PNIP_ACW_RCV_WORD1_ACTIVE_INIT_VALUE        << PNIP_ACW_RCV_WORD1_ACTIVE_SHIFT             )    |   /* Active Bit (Activate/Deactivate this ACW - will be set by command "ACW_on_off" (CONSUMER_CONTROL) */
                       ((LSA_UINT64)PNIP_ACW_RCV_WORD1_ONESHOT_BIT_INIT_VALUE   << PNIP_ACW_RCV_WORD1_ONESHOT_BIT_SHIFT        )    |   /* Oneshot-Bit: older-newer-detection */
                       ((LSA_UINT64)PNIP_ACW_RCV_WORD1_DP2_AFTER_RED_OFF_VALUE  << PNIP_ACW_RCV_WORD1_DP2_AFTER_RED_BIT_SHIFT  )    |   /* DP2_AFTER_RED */
                       ((LSA_UINT64)PNIP_ACW_RCV_WORD1_DP2_ONLY_ON_VALUE        << PNIP_ACW_RCV_WORD1_DP2_ONLY_BIT_SHIFT       )    |   /* DP2_ONLY */
                       ((LSA_UINT64)PNIP_ACW_RCV_WORD1_ETHERTYPE_SELECT_INDEX0_VALUE << PNIP_ACW_RCV_WORD1_ETHERTYPE_SELECT_SHIFT)  |   /* EtherType: Index 0-3 to select one of Register "Local_SPort_Etype_0-3" */
                       ((LSA_UINT64)PNIP_ACW_RCV_WORD1_FID_REDUNDANT_OFF_VALUE  << PNIP_ACW_RCV_WORD1_FID_REDUNDANT_SHIFT      )    |   /* FID_redundant: Bit (Activate/Deactivate the redundant frames) */
                       ((LSA_UINT64)PNIP_ACW_RCV_WORD1_DG_RCV_MODE_OFF_VALUE    << PNIP_ACW_RCV_WORD1_DG_RCV_MODE_SHIFT        )    |   /* DG_Rcv_Mode: pack frame contains more or equal/less datagrams than ACW_DGs */
                       ((LSA_UINT64)PNIP_ACW_RCV_WORD1_EN_CSA_OFF_VALUE         << PNIP_ACW_RCV_WORD1_EN_CSA_SHIFT             )    |   /* Compare SA-MAC: off */
                       ((LSA_UINT64)PNIP_ACW_RCV_WORD1_RT_CLASS_RT_VALUE        << PNIP_ACW_RCV_WORD1_RT_CLASS_SHIFT           )    |   /* RT_class = RT-Frame */
                       ((LSA_UINT64)PNIP_ACW_RCV_WORD1_ACW_DG_ORDER_UP_VALUE    << PNIP_ACW_RCV_WORD1_ACW_DG_ORDER_SHIFT       )    |   /* default: ascending order for receiving of ACW_DGs */
                       ((LSA_UINT64)PNIP_ACW_RCV_WORD1_PACK_WO_DGCC_ON_VALUE    << PNIP_ACW_RCV_WORD1_PACK_WO_DGCC_SHIFT)               /* default: with Chksum */
                     );
    }

    // ***** FrameID to be received ---------------------------------------------------------------------
    EDDP_SET_BIT_VALUE64(ACW_Word64, pPDIRFrameData->FrameID, PNIP_ACW_RCV_WORD1_FRAMEID_MASK, PNIP_ACW_RCV_WORD1_FRAMEID_SHIFT);
    // ***** DgOffset to DG list ------------------------------------------------------------------------
    EDDP_SET_BIT_VALUE64(ACW_Word64, DgOffset, PNIP_R2_ACW_RCV_WORD1_ACW_DG_OFFSET_MASK, PNIP_ACW_RCV_WORD1_ACW_DG_OFFSET_SHIFT);
    // ***** clr flag if no Chksum ----------------------------------------------------------------------
    if (!(pConsContainerEntry->HasChksum))
    {
        EDDP_SET_BIT_VALUE64(ACW_Word64, PNIP_ACW_RCV_WORD1_PACK_WO_DGCC_OFF_VALUE, PNIP_ACW_RCV_WORD1_PACK_WO_DGCC_MASK, PNIP_ACW_RCV_WORD1_PACK_WO_DGCC_SHIFT);
    }
    // ***** set flag if used for redundant frames ------------------------------------------------------
    if (pConsContainerEntry->IsRedundant)
    {   // ***** configure for FrameID: (x) and (x+1)
        EDDP_SET_BIT_VALUE64(ACW_Word64, PNIP_ACW_RCV_WORD1_FID_REDUNDANT_ON_VALUE, PNIP_ACW_RCV_WORD1_FID_REDUNDANT_MASK, PNIP_ACW_RCV_WORD1_FID_REDUNDANT_SHIFT);
    }
    // ***** set flag if last DG used as DgOffset -------------------------------------------------------
    if (pConsContainerEntry->Acw_DG_Order == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_DOWN)
    {   // ***** last DG (step direction is: DOWN)
        EDDP_SET_BIT_VALUE64(ACW_Word64, PNIP_ACW_RCV_WORD1_ACW_DG_ORDER_DOWN_VALUE, PNIP_ACW_RCV_WORD1_ACW_DG_ORDER_MASK, PNIP_ACW_RCV_WORD1_ACW_DG_ORDER_SHIFT);
    }
    // ***** write HOST value to APICtrl-RAM ------------------------------------------------------------
    EDDP_HAL_MEM64_WRITE(pDDB, &((PNIP_ACW_RCV_PTR_TYPE)(pAcwPtr->pHwAcwApi))->ACW_Word1, ACW_Word64);

    /* ----------------------------------------------------------------------------------------------- */
    /* Word2:                                                                                          */
    /* MAC_SA,                Length, IP_D_sel                                                         */
    /* =0                     ^                                                                        */
    /* ----------------------------------------------------------------------------------------------- */

    // ***** Init value ---------------------------------------------------------------------------------
    ACW_Word64 = 0;
    // ***** DataLength: data length of using data between FrameID and APDU-Status ----------------------
    //       --> used by HW if ACW is configured without CC/DCS
    EDDP_SET_BIT_VALUE64(ACW_Word64, pPDIRFrameData->DataLength, PNIP_ACW_RCV_WORD2_LENGTH_MASK, PNIP_ACW_RCV_WORD2_LENGTH_SHIFT);
    // write HOST value to APICtrl-RAM ------------------------------------------------------------------
    EDDP_HAL_MEM64_WRITE(pDDB, &((PNIP_ACW_RCV_PTR_TYPE)(pAcwPtr->pHwAcwApi))->ACW_Word2, ACW_Word64);

    /* ----------------------------------------------------------------------------------------------- */
    /* Word3:                                                                                          */
    /* ScoreBoard_offset, APDUStatus_offset, res, DC, WDR, DHTR, MRTR                                  */
    /*                    ^                                                                            */
    /* ----------------------------------------------------------------------------------------------- */

    // ***** Init value ---------------------------------------------------------------------------------
    ACW_Word64 = ( ((LSA_UINT64)PNIP_R2_ACW_RCV_WORD3_SCOREBOARD_OFFSET_OFF_VALUE << PNIP_ACW_RCV_WORD3_SCOREBOARD_OFFSET_SHIFT) | /* tbd.: Scoreboard_offset: offset in APICtrl-RAM of struct Timer-Scoreboard */
                   ((LSA_UINT64)PNIP_R2_ACW_RCV_WORD3_APDUSTATUS_OFFSET_OFF_VALUE << PNIP_ACW_RCV_WORD3_APDUSTATUS_OFFSET_SHIFT) | /* tbd.: APDUStatus_offset: offset in APICtrl-RAM of APDU-Status */
                   ((LSA_UINT64)PNIP_ACW_RCV_WORD3_DP2_CHECK_ON_VALUE          << PNIP_ACW_RCV_WORD3_DP2_CHECK_SHIFT        ) | /* DP2_CHECK */
                   ((LSA_UINT64)PNIP_ACW_RCV_WORD3_MR_RELOADVALUE_OFF_VALUE    << PNIP_ACW_RCV_WORD3_MR_RELOADVALUE_SHIFT   )   /* MR_ReloadValue of MediaRedundancy-Timer */
                 );

    // ***** APDUStatus_offset (Bit 0,1 shifted out) ----------------------------------------------------
    // APICtrlRAMOffs = Offset >> 2
    // Note: only used by HW if pConsContainerEntry->HasChksum = 0
    APICtrlRAMOffs = EDDP_CRTGetPnipAddrShift (pDDB, pAcwPtr->pHwApduApi, EDDP_CRT_SHIFT_VALUE_2); // Address must be 32Bit aligned!
    EDDP_SET_BIT_VALUE64(ACW_Word64, APICtrlRAMOffs, PNIP_R2_ACW_RCV_WORD3_APDUSTATUS_OFFSET_MASK, PNIP_ACW_RCV_WORD3_APDUSTATUS_OFFSET_SHIFT);
    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTConsInitContainerAcwHwApi(): APDU_OFFS=0x%X", 
        (APICtrlRAMOffs << EDDP_CRT_SHIFT_VALUE_2));

    // ***** write HOST value to APICtrl-RAM ------------------------------------------------------------
    EDDP_HAL_MEM64_WRITE(pDDB, &((PNIP_ACW_RCV_PTR_TYPE)(pAcwPtr->pHwAcwApi))->ACW_Word3, ACW_Word64);

    // ***** Word4 not used here !!!                                                                                          */

    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    /* ACW.CpmSwData --> some meta data for EDDP SW only !!!                                           */
    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    EDDP_HAL_MEM16_WRITE(pDDB, &((PNIP_ACW_RCV_PTR_TYPE)(pAcwPtr->pHwAcwApi))->CpmSwData.AcwId, AcwID);
}
#endif

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTConsDeInitContainerAcwHwApi                     +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                        +*/
/*+                             LSA_UINT16                  AcwID                       +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  AcwID          : Index into container array (ContainerID + ConsumerMax !!!)        +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function clears the ACW of container in APICtrl-RAM              +*/
/*+                                                                                     +*/
/*+               Container must be used in CRT!                                        +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsDeInitContainerAcwHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID)
{
    EDD_RSP                                 Response;
    LSA_UINT16                              AcwMaxNumber, HwAcwSize;
    EDDP_CRT_CONS_ACW_INFO_PTR_PTR_TYPE     pCommonAcwPtr;
    LSA_UINT32                              AdrTemp1, IntOffs;

    pCommonAcwPtr = pDDB->CRT.Cons.pAcwPtrArray;
    AcwMaxNumber  = pDDB->CRT.CfgParam.ConsumerMaxNumber;
    AcwMaxNumber += pDDB->CRT.CfgParam.ConsumerContainerMaxNumber;
    HwAcwSize     = pDDB->CRT.CfgHwAllocParamApi.HWCpmAcwSize;
    EDDP_ASSERT_FALSE(AcwID >= AcwMaxNumber);

    // ***** pHwAcwApi (clear field for given AcwID only)
    AdrTemp1 = (LSA_UINT32) pDDB->CRT.Cons.pHwBaseAcwApi;
    IntOffs = (LSA_UINT32) pCommonAcwPtr[AcwID]->pHwAcwApi;
    EDDP_ASSERT_FALSE(AdrTemp1 > IntOffs);
    IntOffs -= AdrTemp1;
    Response = EDDP_HALMemset (pDDB, EDDP_HAL_RAMTYPE_API_CTRL, EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_CONS_ACW, IntOffs, HwAcwSize, 0);
    EDDP_ASSERT_FALSE(Response != EDD_STS_OK);

    // ***** pHwApduApi (clear field for given AcwID only)
    AdrTemp1 = (LSA_UINT32) pDDB->CRT.Cons.pHwBaseApduApi;
    IntOffs = (LSA_UINT32) pCommonAcwPtr[AcwID]->pHwApduApi;
    EDDP_ASSERT_FALSE(AdrTemp1 > IntOffs);
    IntOffs -= AdrTemp1;
    Response = EDDP_HALMemset (pDDB, EDDP_HAL_RAMTYPE_API_CTRL, EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_CONS_APDU, IntOffs, (sizeof(PNIP_ADPU_RCV_TYPE)), 0);
    EDDP_ASSERT_FALSE(Response != EDD_STS_OK);
}





#if defined (EDDP_CFG_HW_ERTEC200P_SUPPORT)
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTConsUpdateAcwNextOffsetAcwHwApi                     +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                            +*/
/*+                             LSA_UINT16                  AcwID                           +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                     +*/
/*+  AcwID          : Consumer/Container in RBaseList which get New_ACW_nextoffset          +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Update ACW.ACW_nextoffset by Consumer-Add                                 +*/
/*+                                                                                         +*/
/*+               Call this function only, if this ACW is already added to ConsRBaseList!   +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsUpdateAcwNextOffsetAcwHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID)
{
    EDDP_CRT_CONS_ACW_INFO_PTR_PTR_TYPE     pCommonAcwPtr;
    LSA_UINT16                              AcwMaxNumber;
    LSA_UINT32                              New_ACW_nextoffset;     // New_ACW_nextoffset = Offset of next ACW in APICtrl-RAM
    LSA_UINT64                              ACW_Word_Temp;

    AcwMaxNumber  = pDDB->CRT.CfgParam.ConsumerMaxNumber;
    AcwMaxNumber += pDDB->CRT.CfgParam.ConsumerContainerMaxNumber;
    EDDP_ASSERT_FALSE(AcwID >= AcwMaxNumber);

    pCommonAcwPtr       = pDDB->CRT.Cons.pAcwPtrArray;
    New_ACW_nextoffset  = pCommonAcwPtr[AcwID]->RBaseLink.Next;

    if (New_ACW_nextoffset == EDDP_CRT_CHAIN_IDX_END)
    {   // ***** update last ACW in ACW list
        New_ACW_nextoffset = PNIP_R2_ACW_RCV_WORD1_ACW_NEXTOFFSET_INIT_VALUE;
    }
    else
    {   // ***** address (offset) of next ACW in APICtrl-RAM
        New_ACW_nextoffset = EDDP_CRTGetPnipAddrShift (
                                    pDDB, 
                                    (EDD_COMMON_MEM_PTR_TYPE) pCommonAcwPtr[New_ACW_nextoffset]->pHwAcwApi, 
                                    EDDP_CRT_SHIFT_VALUE_3);
    }

    // ***** get actual ACW_Word1 of this ACW
    ACW_Word_Temp = EDDP_HAL_MEM64_READ(pDDB, &((PNIP_ACW_RCV_PTR_TYPE)(pCommonAcwPtr[AcwID]->pHwAcwApi))->ACW_Word1);
    // ***** update ACW_Word1 with New_ACW_nextoffset
    EDDP_SET_BIT_VALUE64( ACW_Word_Temp, New_ACW_nextoffset, PNIP_R2_ACW_RCV_WORD1_ACW_NEXTOFFSET_MASK, PNIP_ACW_RCV_WORD1_ACW_NEXTOFFSET_SHIFT);
    // ***** and write ACW_Word1 back to APICtrl-RAM
    EDDP_HAL_MEM64_WRITE(pDDB, &((PNIP_ACW_RCV_PTR_TYPE)(pCommonAcwPtr[AcwID]->pHwAcwApi))->ACW_Word1, ACW_Word_Temp);
}
#endif

#if defined (EDDP_CFG_HW_ERTEC200P_SUPPORT)
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTConsSetupLegacyStartupAcwHwApi                  +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer               +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  pConsumer      : Pointer to consumer                                               +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Setup the receive ACW for legacy startup mode.                        +*/
/*+                                                                                     +*/
/*+               Only called for legacy consumer. This function is called on first     +*/
/*+               activation of consumer!                                               +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsSetupLegacyStartupAcwHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer)
{
    LSA_UINT64 HW_Word64;
    
    /* NOTE: when HAL macros does not use pDDB */
    LSA_UNUSED_ARG(pDDB);
    
    HW_Word64 = EDDP_HAL_MEM64_READ(pDDB, &((PNIP_ACW_RCV_PTR_TYPE)(pConsumer->ConsAcwTypeInfo.pHwAcwApi))->ACW_Word1);
    EDDP_SET_BIT_VALUE64( HW_Word64, PNIP_ACW_RCV_WORD1_DP2_ONLY_OFF_VALUE, PNIP_ACW_RCV_WORD1_DP2_ONLY_BIT_MASK, PNIP_ACW_RCV_WORD1_DP2_ONLY_BIT_SHIFT);
    EDDP_HAL_MEM64_WRITE (pDDB, &((PNIP_ACW_RCV_PTR_TYPE)(pConsumer->ConsAcwTypeInfo.pHwAcwApi))->ACW_Word1, HW_Word64);
}
#endif
