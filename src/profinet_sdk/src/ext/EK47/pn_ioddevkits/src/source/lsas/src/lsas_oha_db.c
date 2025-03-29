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
/*  F i l e               &F: lsas_oha_db.c                             :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Implements system integration of OHA using the pcPNIO framework          */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*   H i s t o r y :                                                         */
/*   see /_doc/im151_3pn_hist.doc                                            */
/*****************************************************************************/

#define LSAS_MODULE_ID  6

#include "lsas_inc.h"

#include "lsas_int.h"
#include "lsas_dat.h"


/*===========================================================================*/
/*                               OHA_DB                                      */
/*===========================================================================*/


#define DCP_SERVER_PBC

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    lsas_oha_db_init                            +*/
/*+  Input/Output          :    ---                                         +*/
/*+  Result                :    LSA_UINT16                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Result:      OHA_OK                                                    +*/
/*+               OHA_ERR_RESOURCE                                          +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Initializes the OHA-DATABASE                              +*/
/*+                                                                         +*/
/*+               Note: Must only be called if not already initialized, not +*/
/*+                     checked!                                            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void lsas_oha_db_init(LSAS_RQB_PTR_TYPE rqb_ptr)
{
    LSA_UINT16  port_id;
    char*       if_descr [1+OHA_CFG_MAX_PORTS];

    LSA_UINT8   chassis_name[100];
    LSA_UINT16  chassis_pos = 0;
    LSA_UINT16  status = OHA_ERR_PARAM;

    if_descr[0] = LSAS_CFG_OHA_IF_DESCR_INTERFACE;
    if_descr[1] = LSAS_CFG_OHA_IF_DESCR_PORT_01;
    #if (OHA_CFG_MAX_PORTS > 1)
    if_descr[2] = LSAS_CFG_OHA_IF_DESCR_PORT_02;
    #endif
    #if (OHA_CFG_MAX_PORTS > 2)
    if_descr[3] = LSAS_CFG_OHA_IF_DESCR_PORT_03;
    #endif
    #if (OHA_CFG_MAX_PORTS > 3)
    if_descr[4] = LSAS_CFG_OHA_IF_DESCR_PORT_04;
    #endif

	// --- chassis_name ---
    LSAS_MEMSET(chassis_name,' ', sizeof(rqb_ptr->args->oha_db_init.station_type));  // fill with blanks 25+1 and copy station_type
    LSAS_MEMCPY(chassis_name, (char *)rqb_ptr->args->oha_db_init.station_type, LSA_STRLEN((char *)rqb_ptr->args->oha_db_init.station_type));         /* device type*/
    chassis_pos += sizeof(rqb_ptr->args->oha_db_init.station_type);
    LSAS_MEMCPY(&chassis_name[chassis_pos], (char *)rqb_ptr->args->oha_db_init.order_id, LSA_STRLEN((char *)rqb_ptr->args->oha_db_init.order_id));       /* order id */
    chassis_pos += LSA_STRLEN((char *)rqb_ptr->args->oha_db_init.order_id);
    chassis_name[chassis_pos++] = ' ';
    LSAS_MEMCPY(&chassis_name[chassis_pos], (char *)rqb_ptr->args->oha_db_init.serial_number, LSA_STRLEN(rqb_ptr->args->oha_db_init.serial_number));     /* serial number : 16 characters */
    chassis_pos += LSA_STRLEN((char *)rqb_ptr->args->oha_db_init.serial_number);
    chassis_name[chassis_pos++] = ' ';
    LSAS_SPRINTF(&chassis_name[chassis_pos], "%5d", rqb_ptr->args->oha_db_init.hw_revision);                                            /* ASCII HW revision: 5 characters, 1 number */
    chassis_pos += 5;
    chassis_name[chassis_pos++] = ' ';
    LSAS_MEMCPY(&chassis_name[chassis_pos], (char *)rqb_ptr->args->oha_db_init.sw_revision, LSA_STRLEN(rqb_ptr->args->oha_db_init.sw_revision));         /* ASCII SW revision */
    chassis_pos += LSA_STRLEN(rqb_ptr->args->oha_db_init.sw_revision);                                                                          /* requested format: "B20  0 11", for example */
    chassis_name[chassis_pos] = '\0';

	if(chassis_pos >= 100)
	{
		LSAS_FATAL(status);
	}
	status = oha_init_interface(rqb_ptr->args->oha_db_init.interface_id,
								rqb_ptr->args->oha_db_init.vendor_id,
								rqb_ptr->args->oha_db_init.device_id,
								rqb_ptr->args->oha_db_init.instance,
								(rqb_ptr->args->oha_db_init.instance != 0xFFFF) ? LSA_TRUE : LSA_FALSE,
								DCP_PN_DEVICE,
								rqb_ptr->args->oha_db_init.station_type,
								LSA_STRLEN((char*)rqb_ptr->args->oha_db_init.station_type),
								LSA_FALSE, /* True: READ_ONLY IO-Controller, False: READ_WRITE IO-Device */
								chassis_name, LSA_STRLEN((char*)chassis_name),
								LLDP_TLV_SYSCAB_STATION_ONLY + LLDP_TLV_SYSCAB_BRIDGE,
								LLDP_TLV_SYSCAB_STATION_ONLY + LLDP_TLV_SYSCAB_BRIDGE,
								rqb_ptr->args->oha_db_init.sys_description,
								LSA_STRLEN((char*)rqb_ptr->args->oha_db_init.sys_description),
								(LSA_UINT8*)if_descr[0], LSA_STRLEN((char*) if_descr[0])
		 	 	 	 	 	 	);
	if (OHA_OK != status)
	{
		LSAS_FATAL(status);
	}

	for ( port_id = 1; port_id <= OHA_CFG_MAX_PORTS; port_id++ )
	{
		LSA_UINT8  port_name[33];
        LSAS_SPRINTF(port_name, "port-%03d", (LSA_UINT32)port_id);

		status = oha_init_port (rqb_ptr->args->oha_db_init.interface_id, port_id,
								port_name, LSA_STRLEN((char*) port_name),
								(LSA_UINT8*)if_descr[port_id], LSA_STRLEN((char*) if_descr[port_id]));
		if (OHA_OK != status)
		{
			LSAS_FATAL(status);
		}
	}
}



/*****************************************************************************/
/*  end of file APMA_OHA.C                                                   */
/*****************************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
