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
/*  F i l e               &F: eddp_crt_info.c                           :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  CRT (Cycle Real Time) for EDDP  -> Information Functions                 */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who         What                                             */
#ifdef EDDP_MESSAGE
/*  17.11.08    Sergeev     Erstellung                                       */
#endif
/*****************************************************************************/


/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/

#define LTRC_ACT_MODUL_ID   27
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_CRT_INFO */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"           /* eddp headerfiles */
#include "eddp_int.h"           /* internal header  */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)



/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTGetProviderAcwPhase                            +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE                pDDB            +*/
/*+                      LSA_UINT16                             Properties      +*/
/*+                      LSA_UINT16                             FrameID         +*/
/*+                                                                             +*/
/*+  Result         :    LSA_UINT8                                              +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  Properties     : RTClass1/2/3/UDP/DFP/REDUNDANT                            +*/
/*+  FrameID        : FrameID                                                   +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function gets the ROG phase for provider ACW dependent   +*/
/*+               of "Properties" from PROVIDER_ADD.                            +*/
/*+                                                                             +*/
/*+               If Properties is invalid                                      +*/
/*+                     ProviderAcwPhase = PHASE_INVALID                        +*/
/*+                                                                             +*/
/*+               If the FrameID does not match to "Properties"                 +*/
/*+                     ProviderAcwPhase = PHASE_INVALID                        +*/
/*+                                                                             +*/
/*+               If Properties is valid                                        +*/
/*+                     ProviderAcwPhase = Reg, Orange or Green                 +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT8 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetProviderAcwPhase(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  Properties,
    LSA_UINT16                  FrameID)
{
    LSA_UINT8           ProviderAcwPhase;
    LSA_UINT8           RTClass;

    RTClass = EDDP_CRTGetRTClass (pDDB, FrameID);

    if ((RTClass == EDDP_CRT_FRAMEID_RTCLASS_INVALID) && (FrameID != EDD_FRAME_ID_UNDEFINED))
    {
        ProviderAcwPhase = EDDP_CRT_PROV_ACW_PHASE_INVALID;
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTGetProviderAcwPhase(): ERROR -> FrameID(0x%X) is invalid!", 
            FrameID);
    }
    else
    {
        switch (Properties & EDD_CSRT_PROV_PROP_RTCLASS_MASK)
        {
            case EDD_CSRT_PROV_PROP_RTCLASS_1:
            case EDD_CSRT_PROV_PROP_RTCLASS_UDP:
            {
                if ((RTClass != EDDP_CRT_FRAMEID_RTCLASS_1) && (FrameID != EDD_FRAME_ID_UNDEFINED))
                {
                    ProviderAcwPhase = EDDP_CRT_PROV_ACW_PHASE_INVALID;
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTGetProviderAcwPhase(): ERROR -> FrameID(0x%X) does not match to RTClass1", 
                        FrameID);
                }
                else
                {
                    ProviderAcwPhase = EDDP_CRT_PROV_ACW_PHASE_G;
                }
            }
            break;

            case EDD_CSRT_PROV_PROP_RTCLASS_2:
            {
                if ((RTClass != EDDP_CRT_FRAMEID_RTCLASS_2) && (FrameID != EDD_FRAME_ID_UNDEFINED))
                {
                    ProviderAcwPhase = EDDP_CRT_PROV_ACW_PHASE_INVALID;
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTGetProviderAcwPhase(): ERROR -> FrameID(0x%X) does not match to RTClass2", 
                        FrameID);
                }
                else
                {
                    ProviderAcwPhase = EDDP_CRT_PROV_ACW_PHASE_O;
                }
            }
            break;

            case EDD_CSRT_PROV_PROP_RTCLASS_3:
            {
                if ((RTClass != EDDP_CRT_FRAMEID_RTCLASS_3) && (FrameID != EDD_FRAME_ID_UNDEFINED))
                {
                    ProviderAcwPhase = EDDP_CRT_PROV_ACW_PHASE_INVALID;
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTGetProviderAcwPhase(): ERROR -> FrameID(0x%X) does not match to RTClass3", 
                        FrameID);
                }
                else
                {
                    ProviderAcwPhase = EDDP_CRT_PROV_ACW_PHASE_R;
                }
            }
            break;

            default:
                ProviderAcwPhase = EDDP_CRT_PROV_ACW_PHASE_INVALID;
            break;
        }
    }

    return (ProviderAcwPhase);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTGetTBaseIndex                                  +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE            pDDB                +*/
/*+                      LSA_UINT16                         ReductionUser       +*/
/*+                      LSA_UINT16                         PhaseUser           +*/
/*+                                                                             +*/
/*+  Result         :    LSA_UINT16                                             +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  ReductionUser  : Reduction in User-Format  : 1...16384                     +*/
/*+  PhaseUser      : Phase in User-Format      : 1...ReductionUser             +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function calculates the Index in TBase-Array             +*/
/*+                 -> no check ReductionUser/PhaseUser, it must be valid !     +*/
/*+                                                                             +*/
/*+               ReductionUser [1...512]   -> TBaseEntry[Reduction + Phase]    +*/
/*+                 TBaseIndex = ((ReductionUser - 1) + (PhaseUser - 1))        +*/
/*+                                                                             +*/
/*+               ReductionUser [1024...16384]  -> example:                     +*/
/*+                 ReductionUser = 2048                                        +*/
/*+                 PhaseUser     = 187                                         +*/
/*+                 TBaseIndex-Start: next after previous reduction + phase     +*/
/*+                                     = ( (512-1)+(512-1)+1 + (1*64) )        +*/
/*+                 possible PNIP phases: 1, 33, 65, 97, 129, 161, 193 ... 2017 +*/
/*+                 TBaseIndex within reduction:                                +*/
/*+                     -> ((PhaseUser - 1) / PhaseOffsetUdp)                   +*/
/*+                                                                             +*/
/*+                 Table of ReductionUser+PhaseUser | TBaseIndex-HW            +*/
/*+                     --------------------------------                        +*/
/*+                     ReductionUser+Phase | TBaseIndex-HW                     +*/
/*+                     --------------------------------                        +*/
/*+                         1 + 1           |   0                               +*/
/*+                     --------------------------------                        +*/
/*+                         2 + 1           |   1                               +*/
/*+                         2 + 2           |   2                               +*/
/*+                     --------------------------------                        +*/
/*+                         4 + 1           |   3                               +*/
/*+                         4 + 2           |   4                               +*/
/*+                         4 + 3           |   5                               +*/
/*+                         4 + 4           |   6                               +*/
/*+                     --------------------------------                        +*/
/*+                         8 + 1           |   7                               +*/
/*+                     --------------------------------                        +*/
/*+                         ...             |   ...                             +*/
/*+                     --------------------------------                        +*/
/*+                         512 + 1         |   511                             +*/
/*+                         512 + 512       |   1022                            +*/
/*+                     --------------------------------                        +*/
/*+                         1024 + 1        |   1023                            +*/
/*+                         1024 + 1024     |   1086                            +*/
/*+                     --------------------------------                        +*/
/*+                         2048 + 1        |   1087                            +*/
/*+                         2048 + 2048     |   1150                            +*/
/*+                     --------------------------------                        +*/
/*+                         4096 + 1        |   1151                            +*/
/*+                         4096 + 4096     |   1214                            +*/
/*+                     --------------------------------                        +*/
/*+                         8192 + 1        |   1215                            +*/
/*+                         8192 + 8192     |   1278                            +*/
/*+                     --------------------------------                        +*/
/*+                         16384 + 1       |   1279                            +*/
/*+                         16384 + 16384   |   1342                            +*/
/*+                     --------------------------------                        +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetTBaseIndex(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ReductionUser,
    LSA_UINT16                  PhaseUser)
{
    LSA_UINT16          TBaseIndex;
    LSA_UINT16          TBaseIndexBaseUdp;
    LSA_UINT16          PhaseOffsetUdp;

    LSA_UNUSED_ARG(pDDB);   // handle unused parameter

    EDDP_ASSERT_FALSE((ReductionUser < EDDP_CRT_REDUCTION_MIN) || (ReductionUser > EDDP_CRT_REDUCTION_MAX));
    EDDP_ASSERT_FALSE(PhaseUser > ReductionUser);

    TBaseIndex          = 0;
    TBaseIndexBaseUdp   = 0;    // first position of 1024 reduction (first UDP position)
    PhaseOffsetUdp      = 0;    // offset within the UDP reduction

    if (ReductionUser <= EDDP_CRT_REDUCTION_512)
    {
        /* -------------------------------- */
        /* ReductionUser [1...512]          */
        /* -------------------------------- */
        TBaseIndex = ((ReductionUser - 1) + (PhaseUser - 1));
    }
    else
    {
        /* -------------------------------- */
        /* ReductionUser [1024...16384]     */
        /* -------------------------------- */
        // base of UDP TBaseIndex: first position of 1024 reduction
        TBaseIndexBaseUdp = ( ((EDDP_CRT_REDUCTION_512 - 1) + (EDDP_CRT_REDUCTION_512 - 1)) + 1 );

        switch (ReductionUser)
        {
            case EDDP_CRT_REDUCTION_1024:
            {
                /* -------------------------------------------------------------------------------- */
                /* Reg. "PNIP_REG_ACW_T_PO1K_IFA"                                                   */
                /* TBaseIndex-Start = TBaseIndexBaseUdp + 0*64                                      */
                /* possible phases: 0, 16, 32, 48 ... 1008                                          */
                /* -------------------------------------------------------------------------------- */
                TBaseIndex      = (TBaseIndexBaseUdp + (EDDP_CRT_ACW_T_PO1)*(EDDP_CRT_MAX_PHASE_U1024_U16384));
                PhaseOffsetUdp  = EDDP_CRT_PHASE_OFFSET_ACW_T_PO1;
            }
            break;

            case EDDP_CRT_REDUCTION_2048:
            {
                /* -------------------------------------------------------------------------------- */
                /* Reg. "PNIP_REG_ACW_T_PO2K_IFA"                                                   */
                /* TBaseIndex-Start = TBaseIndexBaseUdp + 1*64                                      */
                /* possible phases: 1, 33, 65, 97, 129, 161, 193 ... 2017                           */
                /* -------------------------------------------------------------------------------- */
                TBaseIndex      = (TBaseIndexBaseUdp + (EDDP_CRT_ACW_T_PO2)*(EDDP_CRT_MAX_PHASE_U1024_U16384));
                PhaseOffsetUdp  = EDDP_CRT_PHASE_OFFSET_ACW_T_PO2;
            }
            break;

            case EDDP_CRT_REDUCTION_4096:
            {
                /* -------------------------------------------------------------------------------- */
                /* Reg. "PNIP_REG_ACW_T_PO4K_IFA"                                                   */
                /* TBaseIndex-Start = TBaseIndexBaseUdp + 2*64                                      */
                /* possible phases: 2, 66, 130, 194, 258 ... 4034                                   */
                /* -------------------------------------------------------------------------------- */
                TBaseIndex      = (TBaseIndexBaseUdp + (EDDP_CRT_ACW_T_PO4)*(EDDP_CRT_MAX_PHASE_U1024_U16384));
                PhaseOffsetUdp  = EDDP_CRT_PHASE_OFFSET_ACW_T_PO4;
            }
            break;

            case EDDP_CRT_REDUCTION_8192:
            {
                /* -------------------------------------------------------------------------------- */
                /* Reg. "PNIP_REG_ACW_T_PO8K_IFA"                                                   */
                /* TBaseIndex-Start = TBaseIndexBaseUdp + 3*64                                      */
                /* possible phases: 3, 131, 259, 387 ... 8067                                       */
                /* -------------------------------------------------------------------------------- */
                TBaseIndex      = (TBaseIndexBaseUdp + (EDDP_CRT_ACW_T_PO8)*(EDDP_CRT_MAX_PHASE_U1024_U16384));
                PhaseOffsetUdp  = EDDP_CRT_PHASE_OFFSET_ACW_T_PO8;
            }
            break;

            case EDDP_CRT_REDUCTION_16384:
            {
                /* -------------------------------------------------------------------------------- */
                /* Reg. "PNIP_REG_ACW_T_PO16K_IFA"                                                  */
                /* TBaseIndex-Start = TBaseIndexBaseUdp + 4*64                                      */
                /* possible phases: 4, 260, 516, 772 ... 16132                                      */
                /* -------------------------------------------------------------------------------- */
                TBaseIndex      = (TBaseIndexBaseUdp + (EDDP_CRT_ACW_T_PO16)*(EDDP_CRT_MAX_PHASE_U1024_U16384));
                PhaseOffsetUdp  = EDDP_CRT_PHASE_OFFSET_ACW_T_PO16;
            }
            break;

            default:
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTGetTBaseIndex(): FATAL -> Unsupported ReductionUser(%d)!", 
                    ReductionUser);
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            }
            break;
        }

        /* ------------------------------------------------ */
        /* increase TBaseIndex to phase of PNIP [0..63]     */
        /* ------------------------------------------------ */
        TBaseIndex += ((PhaseUser - 1) / PhaseOffsetUdp);
    }

    EDDP_ASSERT_FALSE(TBaseIndex >= EDDP_CRT_MAX_COUNT_TBASE);

    return (TBaseIndex);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTGetReductionNrFromReductionRatio               +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE            pDDB                +*/
/*+                      LSA_UINT16                         ReductionRatio      +*/
/*+                                                                             +*/
/*+  Result         :    LSA_UINT8                                              +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  ReductionRatio : Reduction in User-Format  : 1...16384                     +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function calculates the ReductionNr(1..15) from given    +*/
/*+               ReductionRatio(1...16384).                                    +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT8 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetReductionNrFromReductionRatio(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ReductionRatio)
{
    LSA_UINT8           ReductionNr = 0;

    LSA_UNUSED_ARG(pDDB);   // handle unused parameter

    switch (ReductionRatio)
    {
        case EDDP_CRT_REDUCTION_1:
            ReductionNr = 1;
        break;

        case EDDP_CRT_REDUCTION_2:
            ReductionNr = 2;
        break;

        case EDDP_CRT_REDUCTION_4:
            ReductionNr = 3;
        break;

        case EDDP_CRT_REDUCTION_8:
            ReductionNr = 4;
        break;

        case EDDP_CRT_REDUCTION_16:
            ReductionNr = 5;
        break;

        case EDDP_CRT_REDUCTION_32:
            ReductionNr = 6;
        break;

        case EDDP_CRT_REDUCTION_64:
            ReductionNr = 7;
        break;

        case EDDP_CRT_REDUCTION_128:
            ReductionNr = 8;
        break;

        case EDDP_CRT_REDUCTION_256:
            ReductionNr = 9;
        break;

        case EDDP_CRT_REDUCTION_512:
            ReductionNr = 10;
        break;

        case EDDP_CRT_REDUCTION_1024:
            ReductionNr = 11;
        break;

        case EDDP_CRT_REDUCTION_2048:
            ReductionNr = 12;
        break;

        case EDDP_CRT_REDUCTION_4096:
            ReductionNr = 13;
        break;

        case EDDP_CRT_REDUCTION_8192:
            ReductionNr = 14;
        break;

        case EDDP_CRT_REDUCTION_16384:
            ReductionNr = 15;
        break;

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTGetReductionNrFromReductionRatio(): FATAL -> Unsupported ReductionRatio(%d)!", 
                ReductionRatio);
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }

    EDDP_ASSERT_FALSE(!(EDDP_CRT_IS_REDUCTIONNR_IN_VALID_RANGE(ReductionNr)));

    return (ReductionNr);
}





#if defined(EDDP_CFG_HW_ERTEC200P_SUPPORT)
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTGetRBaseIndex                                  +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE            pDDB                +*/
/*+                      LSA_UINT16                         FrameID             +*/
/*+                                                                             +*/
/*+  Result         :    LSA_UINT16                                             +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  FrameID        : FrameID                                                   +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function calculates the Index in RBase-Array             +*/
/*+                 -> no check FrameID, it must be valid !                     +*/
/*+                                                                             +*/
/*+               Example:                                                      +*/
/*+                 FrameID = 0xc00b                                            +*/
/*+                 EDDP_CRT_MAX_RINDEX_SIZE = 3    -> use Bit(3:1) in FrameID  +*/
/*+                                                                             +*/
/*+                 Bit(3:1) in FrameID                                         +*/
/*+                 FrameID-Bits    = 15 14 13 ... 4 (3 2 1) 0                  +*/
/*+                 0xc00b-Bits     = 1  1  0  ... 0 (1 0 1) 1                  +*/
/*+                 RBaseIndex      = 5              (1 0 1)                    +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetRBaseIndex(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  FrameID)
{
    LSA_UINT32          i;
    LSA_UINT16          RBaseIndex;
    LSA_UINT16          MaskCnt;
    LSA_UINT16          Mask;

    LSA_UNUSED_ARG(pDDB);   // handle unused parameter

    EDDP_ASSERT_FALSE((FrameID < EDDP_CRT_FRAMEID_MIN) || (FrameID > EDDP_CRT_FRAMEID_MAX));

    MaskCnt = 0;
    Mask    = 0;

    // calculate the mask for FrameID-Bits [MAX_RINDEX_SIZE...1]
    for ( i=1; i <= EDDP_CRT_MAX_RINDEX_SIZE; i++ )
    {
        MaskCnt = EDDP_GET_2_POTENZ(i);
        EDDP_SET_BIT_VALUE16 (Mask, 1, MaskCnt, i);
    }

    // get value of this FrameID-Bits
    RBaseIndex = EDDP_GET_BIT_VALUE (FrameID, Mask, 1);

    EDDP_ASSERT_FALSE(RBaseIndex >= EDDP_CRT_MAX_COUNT_RBASE);

    return (RBaseIndex);
}
#endif

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTGetNewDataStatus                               +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                   +*/
/*+                      LSA_UINT8                       CurrentDataStatus      +*/
/*+                      LSA_UINT8                       DataStatusMask         +*/
/*+                      LSA_UINT8                       NewDataStatus          +*/
/*+                                                                             +*/
/*+  Result         :    LSA_UINT8                                              +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                     +*/
/*+  CurrentDataStatus  : CurrentDataStatus                                     +*/
/*+  DataStatusMask     : Mask of DataStatus                                    +*/
/*+  NewDataStatus      : NewDataStatus                                         +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function calculates the new DataStatus                   +*/
/*+               between CurrentDataStatus and NewDataStatus.                  +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT8 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetNewDataStatus(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT8                   CurrentDataStatus,
    LSA_UINT8                   DataStatusMask,
    LSA_UINT8                   NewDataStatus)
{
    LSA_UNUSED_ARG(pDDB);   // handle unused parameter

    NewDataStatus = ( (CurrentDataStatus & ~DataStatusMask) | (NewDataStatus & DataStatusMask) );

    return (NewDataStatus);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTGetProvAPDUDataStatus                          +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                       +*/
/*+                      LSA_UINT16                  ProviderID                 +*/
/*+                                                                             +*/
/*+  Result         :    LSA_UINT8                                              +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                     +*/
/*+  ProviderID         : ProviderID                                            +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function calculates the APDUDataStatus between           +*/
/*+               GroupDataStatus and LocalDataStatus of this provider.         +*/
/*+               ProviderID must be valid.                                     +*/
/*+                                                                             +*/
/*+               If this provider is a DFP provider, so the LocalDataStatus    +*/
/*+               is the DG-DataStatus from Subframe.                           +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT8 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetProvAPDUDataStatus(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID)
{
    LSA_UINT8                       APDUDataStatus, GroupDataStatus, LocalDataStatus;
    EDDP_CRT_PROVIDER_PTR_TYPE      pProvider;
    EDDP_CRT_PROVIDER_DS_PTR_TYPE   pDataStatus;

    EDDP_ASSERT_FALSE(ProviderID >= pDDB->CRT.CfgParam.ProviderMaxNumber);

    APDUDataStatus  = 0;
    pProvider       = &pDDB->CRT.Prov.pProvEntry[ProviderID];
    pDataStatus     = &pDDB->CRT.Prov.pDSEntry[pProvider->DataStatusId];
    GroupDataStatus = pDDB->CRT.Prov.pProvGroupEntry[pDataStatus->DS_Selector].GroupDataStatus;

    switch (EDDP_HALGetPnipRevision(pDDB))
    {
        #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
        case EDDP_HAL_HW_PNIP_REV1:
        {
            // if DFP provider, this is DG-DataStatus from Subframe
            LocalDataStatus = EDDP_HAL_MEM8_READ(pDDB, &(pDataStatus->pHwDSApiPnipR1->DataStatus));
            APDUDataStatus  = ( ((GroupDataStatus | LocalDataStatus) & EDD_CSRT_DSTAT_WIN_MASK) | (GroupDataStatus & LocalDataStatus) );

            EDDP_CRT_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTGetProvAPDUDataStatus(ProviderID=%d, GroupDataStatus=0x%X, LocalDataStatus=0x%X): APDUDataStatus=0x%X, ProviderFlags=0x%X", 
                ProviderID, GroupDataStatus, LocalDataStatus, APDUDataStatus, pProvider->ProviderFlags);
        }
        break;
        #endif

        #if defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
        case EDDP_HAL_HW_PNIP_REV2:
        case EDDP_HAL_HW_PNIP_REV3:
        {
            // if DFP provider, this is DG-DataStatus from Subframe
            LocalDataStatus = EDDP_HAL_MEM8_READ(pDDB, &(pDataStatus->pHwDSApiPnipR2->DataStatus));
            APDUDataStatus  = ( ((GroupDataStatus | LocalDataStatus) & EDD_CSRT_DSTAT_WIN_MASK) | (GroupDataStatus & LocalDataStatus) );
            if(EDDP_HALGetPnipRevision(pDDB) == EDDP_HAL_HW_PNIP_REV3)
            {
              if (pProvider->ProviderFlags & EDDP_CRT_PROV_FLAGS_IS_SYSRED)
              {
                  // use DataStatus_Saved for System-Redundancy-Provider
                  LocalDataStatus = EDDP_HAL_MEM8_READ(pDDB, &(pDataStatus->pHwDSApiPnipR2->DataStatus_Saved));
                  APDUDataStatus = ((APDUDataStatus & ~EDD_CSRT_DSTAT_C_WIN_MASK) | (LocalDataStatus & EDD_CSRT_DSTAT_C_WIN_MASK));
              }
            }
            EDDP_CRT_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTGetProvAPDUDataStatus(ProviderID=%d, GroupDataStatus=0x%X, LocalDataStatus=0x%X): APDUDataStatus=0x%X, ProviderFlags=0x%X", 
                ProviderID, GroupDataStatus, LocalDataStatus, APDUDataStatus, pProvider->ProviderFlags);
        }
        break;
        #endif

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTGetProvAPDUDataStatus(): FATAL -> Version(%d) of PNIP is unknown", 
                EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }

    return (APDUDataStatus);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTGetConsCycleCounterForIndication               +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE    pDDB                        +*/
/*+                      LSA_UINT16                 ConsumerID                  +*/
/*+                                                                             +*/
/*+  Result         :    LSA_UINT16                                             +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  ConsumerID     : Index in consumer array                                   +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function reads the CycleCounter of given Consumer for    +*/
/*+               the consumer indication.                                      +*/
/*+                                                                             +*/
/*+               CycleCounter:                                                 +*/
/*+                 - not DFP         : APDU-CycleCounter(16Bit) from frame     +*/
/*+                 - DFP without CRC : APDU-CycleCounter(16Bit) from pack frame+*/
/*+                 - DFP with CRC    : DG-CycleCounter(8Bit) from Subframe     +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetConsCycleCounterForIndication(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ConsumerID)
{
    LSA_UINT16                              CycleCounter;
    LSA_UINT16                              AcwContainerIdx, AcwMaxNumber;
    EDDP_CRT_CONSUMER_PTR_TYPE              pConsumer;
    EDDP_CRT_CONSUMER_CONTAINER_PTR_TYPE    pConsContainerEntry;
    EDDP_CRT_CONS_ACW_INFO_PTR_TYPE         pAcwPtr;
    EDDP_CRT_CONSUMER_DG_PTR_TYPE           pDgEntry;

    EDDP_ASSERT_FALSE(ConsumerID >= pDDB->CRT.CfgParam.ConsumerMaxNumber);

    CycleCounter    = 0;
    pConsumer       = &pDDB->CRT.Cons.pConsEntry[ConsumerID];

    /* -------------------------------------------------------------------------------------------------------- */
    /* Not DFP: APDU-CycleCounter(16Bit) from frame                                                             */
    /* NOTE: on BigEndian:    EDDP_HAL_MEM16_READ --> EDDP_SWAP16  +  EDDP_SWAP16   = CycleCounter              */
    /*       on LittleEndian: EDDP_HAL_MEM16_READ --> Value        +  EDDP_SWAP16   = SWAP16(CycleCounter)      */
    /* -------------------------------------------------------------------------------------------------------- */
    if (!(pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_DFP_ITEM))
    {
        CycleCounter = EDDP_SWAP16(EDDP_HAL_MEM16_READ(pDDB, &pConsumer->ConsAcwTypeInfo.pHwApduApi->CycleCounter));
    }

    /* -------------------------------------------------------------------------------------------------------- */
    /* DFP: only if this DFP consumer has connection to DG (is active)                                          */
    /* -------------------------------------------------------------------------------------------------------- */
    if ((pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_DFP_ITEM) && (pConsumer->DgIdx != EDDP_CRT_CHAIN_IDX_END))
    {
        // init pointer to this DG
        pDgEntry = &pDDB->CRT.Cons.pConsDgEntry[pConsumer->DgIdx];
        // this DG must be connected to a pack group (ACW container)
        EDDP_ASSERT_FALSE(pDgEntry->DgGrpIdx >= pDDB->CRT.CfgParam.ConsumerContainerMaxNumber);
        // init pointer to this pack group (ACW container)
        pConsContainerEntry = &pDDB->CRT.Cons.pConsContainerEntry[pDgEntry->DgGrpIdx];
        // check whether this ACW is a container (pack group)
        pAcwPtr = (EDDP_CRT_CONS_ACW_INFO_PTR_TYPE) pConsContainerEntry;
        EDDP_ASSERT_FALSE(!(pAcwPtr->IsAcwContainer));

        // with CRC ?
        if (pConsContainerEntry->HasChksum)
        {
            // DFP with CRC: DG-CycleCounter(8Bit) from Subframe
            CycleCounter = EDDP_HAL_MEM8_READ(pDDB, &pDgEntry->pHwDgStatusApi->CycleCounter);
        }
        else
        {
            // DFP without CRC: APDU-CycleCounter(16Bit) from pack frame
            // get and check the AcwContainerIdx
            AcwContainerIdx = pConsContainerEntry->ConsAcwTypeInfo.AcwIdx;
            AcwMaxNumber    = pDDB->CRT.CfgParam.ConsumerMaxNumber;
            AcwMaxNumber   += pDDB->CRT.CfgParam.ConsumerContainerMaxNumber;
            EDDP_ASSERT_FALSE(AcwContainerIdx >= AcwMaxNumber);
            CycleCounter = EDDP_SWAP16(EDDP_HAL_MEM16_READ(pDDB, &pDDB->CRT.Cons.pHwBaseApduApi[AcwContainerIdx].CycleCounter));
        }
    }

    EDDP_CRT_TRACE_07(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTGetConsCycleCounterForIndication(ConsumerID=%d): CycleCounter=0x%X/%d, Consumer-State=0x%X, FrameID=0x%X, ConsumerFlags=0x%X, DgIdx=%d", 
        ConsumerID, CycleCounter, CycleCounter, pConsumer->State, pConsumer->Param.FrameID, pConsumer->ConsumerFlags, pConsumer->DgIdx);

    return (CycleCounter);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTGetConsAPDUStatus                              +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE                    pDDB        +*/
/*+                      EDD_UPPER_CSRT_GET_APDU_STATUS_PTR_TYPE    pGetApduStatus+*/
/*+                                                                             +*/
/*+  Result         :    LSA_VOID                                               +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  pGetApduStatus : address of GET_APDU_STATUS                                +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: Use this function for service GET_APDU_STATUS (consumer).     +*/
/*+                                                                             +*/
/*+               Write valid CycleCounter and DataStatus only if the consumer  +*/
/*+               frame was received once (is or was in AGAIN).                 +*/
/*+                                                                             +*/
/*+               Valid CycleCounter:                                           +*/
/*+                 - not DFP         : APDU-CycleCounter(16Bit) from frame     +*/
/*+                 - DFP without CRC : APDU-CycleCounter(16Bit) from pack frame+*/
/*+                 - DFP with CRC    : DG-CycleCounter(8Bit) from Subframe     +*/
/*+                                                                             +*/
/*+               Valid DataStatus:                                             +*/
/*+                 - not DFP         : APDU-DataStatus from frame              +*/
/*+                 - DFP without CRC : DG-DataStatus from Subframe             +*/
/*+                 - DFP with CRC    : DG-DataStatus from Subframe             +*/
/*+                                                                             +*/
/*+               Note for DFP:                                                 +*/
/*+                 - if consumer has received once (was in AGAIN) but is no    +*/
/*+                   more active the APDU-Status is not present.               +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTGetConsAPDUStatus(
    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB,
    EDD_UPPER_CSRT_GET_APDU_STATUS_PTR_TYPE     pGetApduStatus)
{
    LSA_UINT16                                  AcwContainerIdx, AcwMaxNumber;
    EDDP_CRT_CONSUMER_PTR_TYPE                  pConsumer;
    EDDP_CRT_CONSUMER_CONTAINER_PTR_TYPE        pConsContainerEntry;
    EDDP_CRT_CONS_ACW_INFO_PTR_TYPE             pAcwPtr;
    EDDP_CRT_CONSUMER_DG_PTR_TYPE               pDgEntry;

    EDDP_ASSERT_NO_NULL_PTR(pGetApduStatus);
    pGetApduStatus->Present                     = EDD_CSRT_APDU_STATUS_NOT_PRESENT;
    pGetApduStatus->APDUStatus.CycleCnt         = 0;
    pGetApduStatus->APDUStatus.DataStatus       = 0;
    pGetApduStatus->APDUStatus.TransferStatus   = 0;

    EDDP_ASSERT_FALSE(pGetApduStatus->ID >= pDDB->CRT.CfgParam.ConsumerMaxNumber);
    pConsumer = &pDDB->CRT.Cons.pConsEntry[pGetApduStatus->ID];

    // only if this consumer frame was received once
    if (pConsumer->ConsumerFlags & EDDP_CONS_FLAG_AGAIN_RCVD)
    {
        /* -------------------------------------------------------------------- */
        /* Not DFP                                                              */
        /* -------------------------------------------------------------------- */
        if (!(pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_DFP_ITEM))
        {
            // APDU-CycleCounter(16Bit) from frame
            pGetApduStatus->APDUStatus.CycleCnt = EDDP_SWAP16(EDDP_HAL_MEM16_READ(pDDB, &pConsumer->ConsAcwTypeInfo.pHwApduApi->CycleCounter));
            // APDU-DataStatus from frame
            pGetApduStatus->APDUStatus.DataStatus = EDDP_HAL_MEM8_READ(pDDB, &pConsumer->ConsAcwTypeInfo.pHwApduApi->DataStatus);

            pGetApduStatus->Present = EDD_CSRT_APDU_STATUS_PRESENT;
        }

        /* -------------------------------------------------------------------- */
        /* DFP: only if this DFP consumer has connection to DG (is active)      */
        /* -------------------------------------------------------------------- */
        if ((pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_DFP_ITEM) && (pConsumer->DgIdx != EDDP_CRT_CHAIN_IDX_END))
        {
            // init pointer to this DG
            pDgEntry = &pDDB->CRT.Cons.pConsDgEntry[pConsumer->DgIdx];
            // this DG must be connected to a pack group (ACW container)
            EDDP_ASSERT_FALSE(pDgEntry->DgGrpIdx >= pDDB->CRT.CfgParam.ConsumerContainerMaxNumber);
            // init pointer to this pack group (ACW container)
            pConsContainerEntry = &pDDB->CRT.Cons.pConsContainerEntry[pDgEntry->DgGrpIdx];
            // check whether this ACW is a container (pack group)
            pAcwPtr = (EDDP_CRT_CONS_ACW_INFO_PTR_TYPE) pConsContainerEntry;
            EDDP_ASSERT_FALSE(!(pAcwPtr->IsAcwContainer));

            // with CRC ?
            if (pConsContainerEntry->HasChksum)
            {
                // DFP with CRC: DG-CycleCounter(8Bit) from Subframe
                pGetApduStatus->APDUStatus.CycleCnt = EDDP_HAL_MEM8_READ(pDDB, &pDgEntry->pHwDgStatusApi->CycleCounter);
            }
            else
            {
                // DFP without CRC: APDU-CycleCounter(16Bit) from pack frame
                // get and check the AcwContainerIdx
                AcwContainerIdx = pConsContainerEntry->ConsAcwTypeInfo.AcwIdx;
                AcwMaxNumber    = pDDB->CRT.CfgParam.ConsumerMaxNumber;
                AcwMaxNumber   += pDDB->CRT.CfgParam.ConsumerContainerMaxNumber;
                EDDP_ASSERT_FALSE(AcwContainerIdx >= AcwMaxNumber);
                pGetApduStatus->APDUStatus.CycleCnt = EDDP_SWAP16(EDDP_HAL_MEM16_READ(pDDB, &pDDB->CRT.Cons.pHwBaseApduApi[AcwContainerIdx].CycleCounter));
            }
            // DG-DataStatus from Subframe
            pGetApduStatus->APDUStatus.DataStatus = EDDP_HAL_MEM8_READ(pDDB, &pDgEntry->pHwDgStatusApi->DataStatus);

            pGetApduStatus->Present = EDD_CSRT_APDU_STATUS_PRESENT;
        }
    }

    EDDP_CRT_TRACE_09(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTGetConsAPDUStatus(ConsumerID=%d): Present=0x%X, CycleCnt=0x%X/%d, DataStatus=0x%X, Consumer-State=0x%X, ConsumerFlags=0x%X, FrameID=0x%X, DgIdx=%d", 
        pGetApduStatus->ID, 
        pGetApduStatus->Present, 
        pGetApduStatus->APDUStatus.CycleCnt, 
        pGetApduStatus->APDUStatus.CycleCnt, 
        pGetApduStatus->APDUStatus.DataStatus, 
        pConsumer->State, 
        pConsumer->ConsumerFlags, 
        pConsumer->Param.FrameID, 
        pConsumer->DgIdx);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTGetHWCycleCounter                              +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                   +*/
/*+                      LSA_UINT8                       RTClass                +*/
/*+                                                                             +*/
/*+  Result         :    LSA_UINT16                                             +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  RTClass        : RT or RToverUDP                                           +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function gets the CycleCounter from PNIP-Register.       +*/
/*+                                                                             +*/
/*+               If RT-Class = RT-Frame                                        +*/
/*+               get CycleCounter from Register "PNIP_REG_RT_CCOUNTER_A/B"     +*/
/*+                                                                             +*/
/*+               If RT-Class = RToverUDP-Frame                                 +*/
/*+               get CycleCounter from Register "PNIP_REG_RTOVERUDP_CCOUNTER_A/B"+*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetHWCycleCounter(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT8                   RTClass)
{
    LSA_UINT16      RegCycleCounter;

    RegCycleCounter = EDDP_CRT_INVALID_VALUE_16;

    switch (RTClass)
    {
        case EDDP_CRT_RTCLASS_RT:
        {
            RegCycleCounter = (LSA_UINT16) EDDP_HAL_REG32_READ(pDDB, PNIP_REG_RT_CCOUNTER_A);    // IFA
        }
        break;

        case EDDP_CRT_RTCLASS_UDP:
        {
            RegCycleCounter = (LSA_UINT16) EDDP_HAL_REG32_READ(pDDB, PNIP_REG_RTOVERUDP_CCOUNTER_A); // IFA
        }
        break;

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTGetHWCycleCounter(): FATAL -> RTClass(0x%X) unknown!", 
                RTClass);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }

    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTGetHWCycleCounter(RTClass=0x%X): RegCycleCounter=0x%X/%d", 
        RTClass, RegCycleCounter, RegCycleCounter);

    return (RegCycleCounter);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTGetAPDUStatus                       +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+                                                                         +*/
/*+  Result                :    EDD_STS_OK                                  +*/
/*+                             EDD_STS_ERR_PARAM                           +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: Get APDU-Status for specified provider or consumer.       +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTGetAPDUStatus(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP                                     Response;
    EDD_SERVICE                                 Service;
    EDDP_CRT_PROVIDER_PTR_TYPE                  pProvider;
    EDD_UPPER_CSRT_GET_APDU_STATUS_PTR_TYPE     pRqbParam;

    Response    = EDD_STS_OK;
    Service     = EDD_RQB_GET_SERVICE(pRQB);
    pRqbParam   = (EDD_UPPER_CSRT_GET_APDU_STATUS_PTR_TYPE) pRQB->pParam;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pHDB);
#endif

    EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_CRTGetAPDUStatus(pRQB=0x%X, Type=0x%X, ID=%d)", 
        pHDB->Handle, pRQB, pRqbParam->Type, pRqbParam->ID);

    // check GetAPDUStatus parameter
    Response = EDDP_CRTCheckGetAPDUStatusParam (pDDB, pRqbParam, Service);

    if (Response == EDD_STS_OK)
    {
        switch (pRqbParam->Type)
        {
            /* -------------------- */
            /* Provider             */
            /* -------------------- */
            case EDD_CSRT_TYPE_PROVIDER:
            {
                EDDP_ASSERT_FALSE(pRqbParam->ID >= pDDB->CRT.CfgParam.ProviderMaxNumber);
                pProvider = &pDDB->CRT.Prov.pProvEntry[pRqbParam->ID];

                pRqbParam->Present                      = EDD_CSRT_APDU_STATUS_PRESENT;
                pRqbParam->APDUStatus.CycleCnt          = EDDP_CRTGetHWCycleCounter (pDDB, pProvider->RTClass);
                pRqbParam->APDUStatus.DataStatus        = EDDP_CRTGetProvAPDUDataStatus (pDDB, pRqbParam->ID);
                pRqbParam->APDUStatus.TransferStatus    = 0;
            }
            break;

            /* -------------------- */
            /* Consumer             */
            /* -------------------- */
            case EDD_CSRT_TYPE_CONSUMER:
            {
                EDDP_CRTGetConsAPDUStatus (pDDB, pRqbParam);
            }
            break;

            /* -------------------- */
            /* Unknown Type         */
            /* -------------------- */
            default:
            {
                // this should have been stated in EDDP_CRTCheckGetAPDUStatusParam()
                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:%2X] EDDP_CRTGetAPDUStatus(): FATAL -> Type=0x%X invalid", 
                    pHDB->Handle, pRqbParam->Type);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTGetAPDUStatus(): Response=0x%X", Response);

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTGetPnipAddrShift                    +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDD_COMMON_MEM_PTR_TYPE     pAddrHost       +*/
/*+                             LSA_UINT16                  Shift           +*/
/*+                                                                         +*/
/*+  Result                :    LSA_UINT32                                  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pAddrHost  : APICtrl-RAM address in HOST format                        +*/
/*+  Shift      : Shift (1/2/3) for 16/32/64 Bit alignment                  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function                                             +*/
/*+                 - converts the HOST-APICtrl-RAM address to PNIP address +*/
/*+                 - gets the offset to base of APICtrl-RAM                +*/
/*+                 - and shifts right this offset at:                      +*/
/*+                         - 1 bit  if 16 bit aligned                      +*/
/*+                         - 2 bits if 32 bit aligned                      +*/
/*+                         - 3 bits if 64 bit aligned                      +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetPnipAddrShift(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_COMMON_MEM_PTR_TYPE     pAddrHost,
    LSA_UINT16                  Shift)
{
    LSA_UINT32      ApiAdrShift = 0;

    if (LSA_HOST_PTR_ARE_EQUAL(pAddrHost, LSA_NULL))
    {
        EDDP_CRT_TRACE_00(pDDB->TraceIdx,  LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTGetPnipAddrShift(): FATAL -> pAddrHost=NULL");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        ApiAdrShift = EDDP_HALGetPnipRAMOffsetToRAM (pDDB, EDDP_HAL_RAMTYPE_API_CTRL, pAddrHost);

        switch (Shift)
        {
            /* ---------------------------- */
            /* 16 bit (2 byte) alignment    */
            /* ---------------------------- */
            case EDDP_CRT_SHIFT_VALUE_1:
            {
                // check alignment
                if ( !(EDDP_GlbIsAligned(pDDB, ApiAdrShift, EDDP_GLB_ALIGNMENT_2_BYTE)) )
                {
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTGetPnipAddrShift(): FATAL -> ApiAdrShift(0x%X) is not 16Bit aligned!", 
                        ApiAdrShift);
                    EDDP_FATAL(EDDP_FATAL_ERR_ALIGNMENT);
                }
                else
                {
                    // it is 16Bit aligned -> cut off the first 1 bit
                    EDDP_SHIFT_VALUE_RIGHT (ApiAdrShift, EDDP_CRT_SHIFT_VALUE_1)
                }
            }
            break;

            /* ---------------------------- */
            /* 32 bit (4 byte) alignment    */
            /* ---------------------------- */
            case EDDP_CRT_SHIFT_VALUE_2:
            {
                // check alignment
                if ( !(EDDP_GlbIsAligned(pDDB, ApiAdrShift, EDDP_GLB_ALIGNMENT_4_BYTE)) )
                {
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTGetPnipAddrShift(): FATAL -> ApiAdrShift(0x%X) is not 32Bit aligned!", 
                        ApiAdrShift);
                    EDDP_FATAL(EDDP_FATAL_ERR_ALIGNMENT);
                }
                else
                {
                    // it is 32Bit aligned -> cut off the first 2 bits
                    EDDP_SHIFT_VALUE_RIGHT (ApiAdrShift, EDDP_CRT_SHIFT_VALUE_2)
                }
            }
            break;

            /* ---------------------------- */
            /* 64 bit (8 byte) alignment    */
            /* ---------------------------- */
            case EDDP_CRT_SHIFT_VALUE_3:
            {
                // check alignment
                if ( !(EDDP_GlbIsAligned(pDDB, ApiAdrShift, EDDP_GLB_ALIGNMENT_8_BYTE)) )
                {
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTGetPnipAddrShift(): FATAL -> ApiAdrShift(0x%X) is not 64Bit aligned!", 
                        ApiAdrShift);
                    EDDP_FATAL(EDDP_FATAL_ERR_ALIGNMENT);
                }
                else
                {
                    // 64Bit alignment -> cut off the first 3 bits
                    EDDP_SHIFT_VALUE_RIGHT (ApiAdrShift, EDDP_CRT_SHIFT_VALUE_3)
                }
            }
            break;

            default:
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTGetPnipAddrShift(): FATAL -> Shift value unknown! Shift=0x%X", 
                    Shift);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }

    return (ApiAdrShift);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTGetRTClass                                     +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE                pDDB            +*/
/*+                      LSA_UINT16                             FrameID         +*/
/*+                                                                             +*/
/*+  Result         :    LSA_UINT8                                              +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  FrameID        : FrameID                                                   +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function gets the RTClass dependent of FrameID.          +*/
/*+                                                                             +*/
/*+               Possible RTClasses:                                           +*/
/*+                 - RTClass1 (incl. RToverUDP)                                +*/
/*+                 - RTClass2                                                  +*/
/*+                 - RTClass3                                                  +*/
/*+                                                                             +*/
/*+               If the given FrameID is out of range, the RTCLASS_INVALID     +*/
/*+               is given back.                                                +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT8 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetRTClass(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  FrameID)
{
    LSA_UINT8       RTClass = EDDP_CRT_FRAMEID_RTCLASS_INVALID;

    LSA_UNUSED_ARG(pDDB);

    if ((FrameID >= EDDP_CRT_FRAMEID_RT_CLASS1_START) && (FrameID <= EDDP_CRT_FRAMEID_RT_CLASS1_STOP))
    {
        RTClass = EDDP_CRT_FRAMEID_RTCLASS_1;
    }
    else
    {
        if ((FrameID >= EDDP_CRT_FRAMEID_RT_CLASS2_START) && (FrameID <= EDDP_CRT_FRAMEID_RT_CLASS2_STOP))
        {
            RTClass = EDDP_CRT_FRAMEID_RTCLASS_2;
        }
        else
        {
            if ((FrameID >= EDDP_CRT_FRAMEID_RT_CLASS3_START) && (FrameID <= EDDP_CRT_FRAMEID_RT_CLASS3_STOP))
            {
                RTClass = EDDP_CRT_FRAMEID_RTCLASS_3;
            }
        }
    }

    return (RTClass);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTGetProviderMaxNumber                           +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE            pDDB                +*/
/*+                                                                             +*/
/*+  Result         :    LSA_UINT16                                             +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function gets the ProviderMaxNumber.                     +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetProviderMaxNumber(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    return (pDDB->CRT.CfgParam.ProviderMaxNumber);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTGetConsumerMaxNumber                           +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE            pDDB                +*/
/*+                                                                             +*/
/*+  Result         :    LSA_UINT16                                             +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function gets the ConsumerMaxNumber.                     +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetConsumerMaxNumber(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    return (pDDB->CRT.CfgParam.ConsumerMaxNumber);
}






/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTGetProviderAcwDGMaxNumber                      +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE            pDDB                +*/
/*+                                                                             +*/
/*+  Result         :    LSA_UINT16                                             +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function gets the ProviderAcwDGMaxNumber.                +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetProviderAcwDGMaxNumber(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    return (pDDB->CRT.CfgParam.ProviderAcwDGMaxNumber);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTGetConsumerAcwDGMaxNumber                      +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE            pDDB                +*/
/*+                                                                             +*/
/*+  Result         :    LSA_UINT16                                             +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function gets the ConsumerAcwDGMaxNumber.                +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetConsumerAcwDGMaxNumber(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    return (pDDB->CRT.CfgParam.ConsumerDGMaxNumber);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTGetApplTimerMode                               +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE            pDDB                +*/
/*+                                                                             +*/
/*+  Result         :    LSA_UINT8                                              +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function gets the ApplTimerMode.                         +*/
/*+                                                                             +*/
/*+               Possible values (defines) for ApplTimerMode see <eddp_sys.h>. +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT8 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetApplTimerMode(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    return (pDDB->CRT.CfgParam.ApplTimerMode);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTGetTransferEndCorrectionValue                  +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE            pDDB                +*/
/*+                                                                             +*/
/*+  Result         :    LSA_UINT32                                             +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function gets the TransferEndCorrectionValue.            +*/
/*+                                                                             +*/
/*+               Possible values for TransferEndCorrectionValue                +*/
/*+               see <eddp_sys.h>.                                             +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetTransferEndCorrectionValue(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    return (pDDB->CRT.CfgParam.TransferEndCorrectionValue);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTIsTransferEndCorrectionPos                     +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE            pDDB                +*/
/*+                                                                             +*/
/*+  Result         :    LSA_BOOL                                               +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function gets the isTransferEndCorrectionPos.            +*/
/*+                                                                             +*/
/*+               Possible values for isTransferEndCorrectionPos                +*/
/*+               see <eddp_sys.h>.                                             +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTIsTransferEndCorrectionPos(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    return (pDDB->CRT.CfgParam.isTransferEndCorrectionPos);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTGetFSOLeadtimeOrFetchtimeDelta                 +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                       +*/
/*+                                                                             +*/
/*+  Result         :    LSA_UINT32                                             +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to Device Description Block                       +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function gets the PPM FSO_Leadtime (ERTEC200P)           +*/
/*+               or PPM_FetchTimeDelta (HERA)                                  +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetFSOLeadtimeOrFetchtimeDelta(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT32      FSO_Leadtime = 0;

    switch (EDDP_HALGetPnipRevision_Ext(pDDB))
    {
        #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
        case EDDP_HAL_HW_PNIP_R1A1:
        {
            FSO_Leadtime = EDDP_CRT_PPM_FSO_LEADTIME_ERTEC200P_PNIP_REV1;
        }
        break;

        case EDDP_HAL_HW_PNIP_R1F1:
        {
            FSO_Leadtime = EDDP_CRT_PPM_FSO_LEADTIME_FPGA_PNIP_REV1;
        }
        break;
        #endif

        #ifdef EDDP_CFG_HW_PNIP_REV2_SUPPORT
        case EDDP_HAL_HW_PNIP_R2A1:
        {
            FSO_Leadtime = EDDP_CRT_PPM_FSO_LEADTIME_ERTEC200P_PNIP_REV2;
        }
        break;

        case EDDP_HAL_HW_PNIP_R2F1:
        {
            FSO_Leadtime = EDDP_CRT_PPM_FSO_LEADTIME_FPGA_PNIP_REV2;
        }
        break;
        #endif

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTGetFSOLeadtimeOrFetchtimeDelta(): FATAL -> PnipRevision(%d) is unknown", 
                EDDP_HALGetPnipRevision_Ext(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }

    return (FSO_Leadtime);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTGetConsMrtRrReloadValue                        +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                       +*/
/*+                                                                             +*/
/*+  Result         :    LSA_UINT32                                             +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function calculates the value for register               +*/
/*+               "MRT_RR_ReloadValue".                                         +*/
/*+               This register is not present in PN-IP Rev2.                   +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#if defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetConsMrtRrReloadValue(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT32      MRT_RR_ReloadValue = ( (500 + pDDB->SWI.CycleInfo.CycleBaseFactor - 1) / pDDB->SWI.CycleInfo.CycleBaseFactor );

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] EDDP_CRTGetConsMrtRrReloadValue(): MRT_RR_ReloadValue=%d, CycleBaseFactor=%d", 
        MRT_RR_ReloadValue, pDDB->SWI.CycleInfo.CycleBaseFactor);

    return (MRT_RR_ReloadValue);
}
#endif

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTGetNumberActiveConsumerProvider                +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE            pDDB                +*/
/*+                                                                             +*/
/*+  Result         :    LSA_UINT32                                             +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function gets the number of active consumer & provider.  +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetNumberActiveConsumerProvider(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    return (pDDB->CRT.Cons.ConsumerActCnt + pDDB->CRT.Prov.AcwActCnt);
}

