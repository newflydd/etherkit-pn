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
/*  F i l e               &F: trace_dk.c                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Logging Output function into a circular buffer                           */
/*                                                                           */
/*  THIS MODULE HAS TO BE MODIFIED BY THE PNIO USER                          */
/*                                                                           */
/*****************************************************************************/

#include "compiler.h"
#include "os.h"
#include "pniousrd.h"
#include "pnio_trace.h"

#if   ((PNIO_TRACE == PNIO_TRACE_DK_CONSOLE)||(PNIO_TRACE == PNIO_TRACE_DK_MEM)||(PNIO_TRACE == PNIO_TRACE_DK_MEMXT)||(PNIO_TRACE == PNIO_TRACE_DK_LSA))

    // *------------ defines -----------------*


    // *------------ internal functions  ----------*

    // *------------ public data  ----------*

    SUBS_TRACE_LEVEL SubsysTrace [TRACE_SUBSYS_NUM ];

    // *------------ local data  ----------*

	// *----------------------------------------------------------------*
	// *   initializes TRACE DK module
	// *----------------------------------------------------------------*
	// *  initializes TRACE DK module and presets all trace level and output
	// *  devices
	// *
	// *  input :  ---
	// *  out   :  ---
	// *----------------------------------------------------------------*
	void TrcDkInit (void)
	{
        #if 1  // default
            TrcDkSetAllLevel     (PNIO_LOG_ERROR);
            TrcDkSetAllMinLevel  (DEV_MEM,   PNIO_LOG_NOTE_HIGH);
            TrcDkSetAllMinLevel  (DEV_STD,   PNIO_LOG_CHAT_HIGH);
            TrcDkSetAllMinLevel  (DEV_LSA,   PNIO_LOG_NOTE_HIGH);
            #if (PNIOD_PLATFORM & PNIOD_PLATFORM_DKSW)
                TrcDkSetLevel        (TRACE_SUBSYS_EDDS_UPPER, PNIO_LOG_WARNING_HIGH);
            #endif
        #else
            TrcDkSetAllLevel     (PNIO_LOG_ERROR);
            TrcDkSetAllMinLevel  (DEV_MEM,   PNIO_LOG_NOTE_HIGH);
            TrcDkSetAllMinLevel  (DEV_STD,   PNIO_LOG_CHAT_HIGH);
            TrcDkSetAllMinLevel  (DEV_LSA,   PNIO_LOG_NOTE_HIGH);
            TrcDkSetLevel        (TRACE_SUBSYS_IOD_MSG, PNIO_LOG_NOTE_HIGH);
            TrcDkSetLevel        (TRACE_SUBSYS_EDDS_UPPER, PNIO_LOG_WARNING_HIGH);
            // ToWe DEBUG
            TrcDkSetPackageLevel     (TRACE_SUBSYS_CM_LOWER,   PNIO_LOG_NOTE_HIGH);
            //TrcDkSetPackageLevel     (TRACE_SUBSYS_PNDV_LOWER, PNIO_LOG_NOTE);
            //TrcDkSetPackageLevel     (TRACE_SUBSYS_PNPB_LOWER, PNIO_LOG_NOTE);
            TrcDkSetPackageLevel     (TRACE_SUBSYS_EDDS_LOWER, PNIO_LOG_NOTE_HIGH);
        #endif
	}


	// *----------------------------------------------------------------*
	// *   TrcDkSetLevel
	// *----------------------------------------------------------------*
	// *   set trace level for one subsystem
	// *
	// *  input :  subsystem    subsystem ID
	// *           Trace level  specifies trace level for trace output
	// *  out   :  PNIO_OK, PNIO_NOT_OK
	// *----------------------------------------------------------------*
	void  TrcDkSetLevel (PNIO_UINT32 Subsys, PNIO_UINT32 TraceLevel)
	{
		if ((TraceLevel > PNIO_LOG_CHAT)|| (TraceLevel == 0))
		{
			PNIO_printf ("ERROR: (TrcDkSetLevel) invalid trace level %d\n", TraceLevel);
			return;
		}

		if (Subsys >= TRACE_SUBSYS_NUM)
        {
			PNIO_printf ("ERROR: (TrcDkSetLevel) invalid subsystem %d\n", Subsys);
			return;
        }
		else
		{
		    SubsysTrace[Subsys].level = (PNIO_UINT8) TraceLevel;
		}
        #if (PNIO_TRACE == PNIO_TRACE_DK_LSA)
            trace_config.maxtracelevel[Subsys] =  (PNIO_UINT16) TraceLevel;
        #endif
	}


	// *----------------------------------------------------------------*
	// *   TrcDkSetAllLevel (PNIO_UINT32 TraceLevel)
	// *----------------------------------------------------------------*
	// *   set trace level for all subsystems of all packages
	// *
	// *  input :  Trace level  specifies trace level for trace output
	// *  out   :  PNIO_OK, PNIO_NOT_OK
	// *----------------------------------------------------------------*
	void  TrcDkSetAllLevel (PNIO_UINT32 TraceLevel)
	{
		int Subsys;

       if ((TraceLevel > PNIO_LOG_CHAT)|| (TraceLevel == 0))
        {
             PNIO_printf ("ERROR: (TrcDkSetAllLevel) invalid trace level %d\n", TraceLevel);
             return;
        }

		for (Subsys = 0; Subsys < TRACE_SUBSYS_NUM; Subsys++)
		{
			TrcDkSetLevel (Subsys, TraceLevel);
		}
	}

	// *----------------------------------------------------------------*
	// *   TrcDkSetPackageLevel (PNIO_UINT32 Subsys, PNIO_UINT32 TraceLevel)
	// *----------------------------------------------------------------*
	// *   set trace level for all subsystems of one lsa-package
	// *
	// *  input :  subsystem    musb be TRACE_SUBSYS_xxx_LOWER (xxx = ACP, CM,...)
	// *           Trace level  specifies trace level for trace output
	// *  out   :  PNIO_OK, PNIO_NOT_OK
	// *----------------------------------------------------------------*
	void  TrcDkSetPackageLevel (PNIO_UINT32 Subsys, PNIO_UINT32 TraceLevel)
	{
		PNIO_UINT32 FirstSubsys = 0, LastSubsys = 0;

        if ((TraceLevel > PNIO_LOG_CHAT)|| (TraceLevel == 0))
        {
             PNIO_printf ("ERROR: (TrcDkSetPackageLevel) invalid trace level %d\n", TraceLevel);
             return;
        }

        if (Subsys > TRACE_SUBSYS_NUM)
        {
             PNIO_printf ("ERROR: (TrcDkSetPackageLevel) invalid subsystem %d\n", Subsys);
             return;
        }

		switch (Subsys)
		{
            case TRACE_SUBSYS_APPL_LOWER:
                 FirstSubsys = TRACE_SUBSYS_APPL_LOWER;
                 LastSubsys  = TRACE_SUBSYS_APPL_UPPER;
                 break;
			case TRACE_SUBSYS_ACP_LOWER:
				 FirstSubsys = TRACE_SUBSYS_ACP_LOWER;
				 LastSubsys  = TRACE_SUBSYS_ACP_UPPER;
				 break;
			case TRACE_SUBSYS_CLRPC_LOWER:
				 FirstSubsys = TRACE_SUBSYS_CLRPC_LOWER;
				 LastSubsys  = TRACE_SUBSYS_CLRPC_UPPER;
				 break;
			case TRACE_SUBSYS_CM_LOWER:
				 FirstSubsys = TRACE_SUBSYS_CM_LOWER;
				 LastSubsys  = TRACE_SUBSYS_CM_UPPER;
				 break;
			case TRACE_SUBSYS_DCP_LOWER:
				 FirstSubsys = TRACE_SUBSYS_DCP_LOWER;
				 LastSubsys  = TRACE_SUBSYS_DCP_UPPER;
				 break;
            #if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200_400)
                case TRACE_SUBSYS_EDDI_LOWER:
                     FirstSubsys = TRACE_SUBSYS_EDDI_LOWER;
                     LastSubsys  = TRACE_SUBSYS_EDDI_UPPER;
                     break;
            #elif (PNIOD_PLATFORM & (PNIOD_PLATFORM_EB200P | PNIOD_PLATFORM_RZT2_EDDP ) )
                case TRACE_SUBSYS_EDDP_LOWER:
                     FirstSubsys = TRACE_SUBSYS_EDDP_LOWER;
                     LastSubsys  = TRACE_SUBSYS_EDDP_UPPER;
                     break;
            #elif (PNIOD_PLATFORM & PNIOD_PLATFORM_DKSW)
           	    case TRACE_SUBSYS_EDDS_LOWER:
               	     FirstSubsys = TRACE_SUBSYS_EDDS_LOWER;
               	     LastSubsys  = TRACE_SUBSYS_EDDS_UPPER;
               	     break;
            #endif
			case TRACE_SUBSYS_GSY_LOWER:
				 FirstSubsys = TRACE_SUBSYS_GSY_LOWER;
				 LastSubsys  = TRACE_SUBSYS_GSY_UPPER;
				 break;
            case TRACE_SUBSYS_IOD_LOWER:
                 FirstSubsys = TRACE_SUBSYS_IOD_LOWER;
                 LastSubsys  = TRACE_SUBSYS_IOD_UPPER;
                 break;
			case TRACE_SUBSYS_LLDP_LOWER:
				 FirstSubsys = TRACE_SUBSYS_LLDP_LOWER;
				 LastSubsys  = TRACE_SUBSYS_LLDP_UPPER;
				 break;
			case TRACE_SUBSYS_MRP_LOWER:
				 FirstSubsys = TRACE_SUBSYS_MRP_LOWER;
				 LastSubsys  = TRACE_SUBSYS_MRP_UPPER;
				 break;
            case TRACE_SUBSYS_NARE_LOWER:
                 FirstSubsys = TRACE_SUBSYS_NARE_LOWER;
                 LastSubsys  = TRACE_SUBSYS_NARE_UPPER;
                 break;
			case TRACE_SUBSYS_OHA_LOWER:
				 FirstSubsys = TRACE_SUBSYS_OHA_LOWER;
				 LastSubsys  = TRACE_SUBSYS_OHA_UPPER;
				 break;
            case TRACE_SUBSYS_PNDV_LOWER:
                 FirstSubsys = TRACE_SUBSYS_PNDV_LOWER;
                 LastSubsys  = TRACE_SUBSYS_PNDV_UPPER;
                 break;
            case TRACE_SUBSYS_PNPB_LOWER:
                 FirstSubsys = TRACE_SUBSYS_PNPB_LOWER;
                 LastSubsys  = TRACE_SUBSYS_PNPB_UPPER;
                 break;
			case TRACE_SUBSYS_POF_LOWER:
				 FirstSubsys = TRACE_SUBSYS_POF_LOWER;
				 LastSubsys  = TRACE_SUBSYS_POF_UPPER;
				 break;
			case TRACE_SUBSYS_SOCK_LOWER:
				 FirstSubsys = TRACE_SUBSYS_SOCK_LOWER;
				 LastSubsys  = TRACE_SUBSYS_SOCK_UPPER;
				 break;
            case TRACE_SUBSYS_TCIP_LOWER:
                 FirstSubsys = TRACE_SUBSYS_TCIP_LOWER;
                 LastSubsys  = TRACE_SUBSYS_TCIP_UPPER;
                 break;
            case TRACE_SUBSYS_TSKMSG_LOWER:
                 FirstSubsys = TRACE_SUBSYS_TSKMSG_LOWER;
                 LastSubsys  = TRACE_SUBSYS_TSKMSG_UPPER;
                 break;

			default:
			     PNIO_printf ("DK_TRACE ERROR: invalid SUBSYSTEM %d\n", Subsys);
                 return;   // do nothing
		}

		for (Subsys = FirstSubsys; Subsys <= LastSubsys; Subsys++)
			TrcDkSetLevel (Subsys, TraceLevel);
	}





	// *----------------------------------------------------------------*
	// *   TrcDkSetMinLevel
	// *----------------------------------------------------------------*
	// *   set minimal output level for one output device and
	// *   for one subsystems of one lsa-package
	// *
	// *   input :  subsystem    musb be TRACE_SUBSYS_xxx_LOWER (xxx = ACP, CM,...)
	// *            outDev       specifies the output device (DEV_STD, DEV_MEM or both)
	// *            MinLevel     value of minimum output level
	// *   out   :  ---
	// *----------------------------------------------------------------*
	void  TrcDkSetMinLevel (PNIO_UINT32 Subsys, TRC_OUTP_DEVICE OutDev, PNIO_UINT32 MinLevel)
	{
        if ((MinLevel > PNIO_LOG_CHAT) || (MinLevel == 0))
        {
             PNIO_printf ("ERROR: invalid trace level %d\n", MinLevel);
             return;
        }
		if (Subsys >= TRACE_SUBSYS_NUM)
        {
             PNIO_printf ("ERROR: invalid subsystem %d\n", Subsys);
             return;
        }
		    if (OutDev & DEV_STD)
		    {
			    SubsysTrace[Subsys].MinLevel_StdOut = (PNIO_UINT8) MinLevel;
		    }
		    if (OutDev & DEV_MEM)
		    {
		        SubsysTrace[Subsys].MinLevel_Mem    = (PNIO_UINT8) MinLevel;
		    }
            #if (PNIO_TRACE == PNIO_TRACE_DK_LSA)
		        if (OutDev & DEV_LSA)
		        {
			        SubsysTrace[Subsys].MinLevel_Lsa = (PNIO_UINT8) MinLevel;
		            trace_config.maxtracelevel[Subsys] =  (PNIO_UINT16) MinLevel;
		        }
           #endif
	}


	// *----------------------------------------------------------------*
	// *   TrcDkSetAllMinLevel (TRC_OUTP_DEVICE OutDev, PNIO_UINT32 MinLevel)
	// *----------------------------------------------------------------*
	// *   set output device for all subsystems of all lsa-packages
	// *
	// *   input :  OutDev       specifies output device Stdout or Mem or both
	// *   out   :  ---
	// *----------------------------------------------------------------*
	void  TrcDkSetAllMinLevel (TRC_OUTP_DEVICE OutDev, PNIO_UINT32 MinLevel)
	{
		int Subsys;

        if ((MinLevel > PNIO_LOG_CHAT) || (MinLevel == 0))
        {
              PNIO_printf ("ERROR: invalid trace level %d\n", MinLevel);
              return;
         }
		for (Subsys = 0; Subsys < TRACE_SUBSYS_NUM; Subsys++)
		{
			TrcDkSetMinLevel (Subsys, OutDev, MinLevel);
		}
	}

	// *----------------------------------------------------------------*
	// *   TrcDkSetPackageMinLevel(PNIO_UINT32 Subsys, TRC_OUTP_DEVICE OutDev,  PNIO_UINT32 MinLevel)
	// *----------------------------------------------------------------*
	// *   set output device for all subsystems of one lsa-package
	// *
	// *   input :  subsystem    musb be TRACE_SUBSYS_xxx_LOWER (xxx = ACP, CM,...)
	// *            OutDev       specifies output device Stdout or Mem or both
	// *   out   :  ---
	// *----------------------------------------------------------------*
	void  TrcDkSetPackageMinLevel(PNIO_UINT32 Subsys, TRC_OUTP_DEVICE OutDev,  PNIO_UINT32 MinLevel)
	{
		PNIO_UINT32 FirstSubsys = 0, LastSubsys = 0;
        if ((MinLevel > PNIO_LOG_CHAT) || (MinLevel == 0))
        {
             PNIO_printf ("ERROR: invalid trace level %d\n", MinLevel);
             return;
        }

        if (Subsys > TRACE_SUBSYS_NUM)
        {
             PNIO_printf ("ERROR: invalid subsystem %d\n", Subsys);
             return;
        }

		switch (Subsys)
		{
            case TRACE_SUBSYS_APPL_LOWER:
                 FirstSubsys = TRACE_SUBSYS_APPL_LOWER;
                 LastSubsys  = TRACE_SUBSYS_APPL_UPPER;
                 break;
			case TRACE_SUBSYS_ACP_LOWER:
				 FirstSubsys = TRACE_SUBSYS_ACP_LOWER;
				 LastSubsys  = TRACE_SUBSYS_ACP_UPPER;
				 break;
			case TRACE_SUBSYS_CLRPC_LOWER:
				 FirstSubsys = TRACE_SUBSYS_CLRPC_LOWER;
				 LastSubsys  = TRACE_SUBSYS_CLRPC_UPPER;
				 break;
			case TRACE_SUBSYS_CM_LOWER:
				 FirstSubsys = TRACE_SUBSYS_CM_LOWER;
				 LastSubsys  = TRACE_SUBSYS_CM_UPPER;
				 break;
			case TRACE_SUBSYS_DCP_LOWER:
				 FirstSubsys = TRACE_SUBSYS_DCP_LOWER;
				 LastSubsys  = TRACE_SUBSYS_DCP_UPPER;
				 break;
            #if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200_400)
                case TRACE_SUBSYS_EDDI_LOWER:
                     FirstSubsys = TRACE_SUBSYS_EDDI_LOWER;
                     LastSubsys  = TRACE_SUBSYS_EDDI_UPPER;
                     break;
            #elif (PNIOD_PLATFORM & (PNIOD_PLATFORM_EB200P | PNIOD_PLATFORM_RZT2_EDDP) )
                case TRACE_SUBSYS_EDDP_LOWER:
                     FirstSubsys = TRACE_SUBSYS_EDDP_LOWER;
                     LastSubsys  = TRACE_SUBSYS_EDDP_UPPER;
                     break;
            #endif
            case TRACE_SUBSYS_GSY_LOWER:
                 FirstSubsys = TRACE_SUBSYS_GSY_LOWER;
                 LastSubsys  = TRACE_SUBSYS_GSY_UPPER;
                 break;
            case TRACE_SUBSYS_IOD_LOWER:
                 FirstSubsys = TRACE_SUBSYS_IOD_LOWER;
                 LastSubsys  = TRACE_SUBSYS_IOD_UPPER;
                 break;
            case TRACE_SUBSYS_LLDP_LOWER:
                 FirstSubsys = TRACE_SUBSYS_LLDP_LOWER;
                 LastSubsys  = TRACE_SUBSYS_LLDP_UPPER;
                 break;
			case TRACE_SUBSYS_MRP_LOWER:
				 FirstSubsys = TRACE_SUBSYS_MRP_LOWER;
				 LastSubsys  = TRACE_SUBSYS_MRP_UPPER;
				 break;
            case TRACE_SUBSYS_NARE_LOWER:
                 FirstSubsys = TRACE_SUBSYS_NARE_LOWER;
                 LastSubsys  = TRACE_SUBSYS_NARE_UPPER;
                 break;
			case TRACE_SUBSYS_OHA_LOWER:
				 FirstSubsys = TRACE_SUBSYS_OHA_LOWER;
				 LastSubsys  = TRACE_SUBSYS_OHA_UPPER;
				 break;
            case TRACE_SUBSYS_PNDV_LOWER:
                 FirstSubsys = TRACE_SUBSYS_PNDV_LOWER;
                 LastSubsys  = TRACE_SUBSYS_PNDV_UPPER;
                 break;
            case TRACE_SUBSYS_PNPB_LOWER:
                 FirstSubsys = TRACE_SUBSYS_PNPB_LOWER;
                 LastSubsys  = TRACE_SUBSYS_PNPB_UPPER;
                 break;
 			case TRACE_SUBSYS_POF_LOWER:
				 FirstSubsys = TRACE_SUBSYS_POF_LOWER;
				 LastSubsys  = TRACE_SUBSYS_POF_UPPER;
				 break;
			case TRACE_SUBSYS_SOCK_LOWER:
				 FirstSubsys = TRACE_SUBSYS_SOCK_LOWER;
				 LastSubsys  = TRACE_SUBSYS_SOCK_UPPER;
				 break;
           case TRACE_SUBSYS_TCIP_LOWER:
                 FirstSubsys = TRACE_SUBSYS_TCIP_LOWER;
                 LastSubsys  = TRACE_SUBSYS_TCIP_UPPER;
                 break;
           case TRACE_SUBSYS_TSKMSG_LOWER:
                 FirstSubsys = TRACE_SUBSYS_TSKMSG_LOWER;
                 LastSubsys  = TRACE_SUBSYS_TSKMSG_UPPER;
                 break;
			default:
                 PNIO_printf ("DK_TRACE ERROR: invalid SUBSYSTEM %d\n", Subsys);
			     return;   // do nothing
		}

		for (Subsys = FirstSubsys; Subsys <= LastSubsys; Subsys++)
			TrcDkSetMinLevel (Subsys, OutDev, MinLevel);
	}


#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
