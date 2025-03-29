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
/*  C o m p o n e n t     &C: MRP (Media Redundancy Protocol)           :C&  */
/*                                                                           */
/*  F i l e               &F: mrp_rec.c                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  implements interface function for CM-PD.                                 */
/*                                                                           */
/*  CM-PD calls requests in following sequence:                              */
/*                                                                           */
/*         CM-PD             MRP                                             */
/*                                                                           */
/*   (1)       ---PREPARE-->                                                 */
/*                                                                           */
/*   (2)       ---WRITE---->          (n times)                              */
/*                                                                           */
/*   (3)       ----END----->                                                 */
/*                                                                           */
/*   (4)       ---COMMIT--->                                                 */
/*                                                                           */
/*   (async)   ----READ---->          (n times)                              */
/*                                                                           */
/*   (async)    -INDICATION_PROVIDE->         (n times)                      */
/*   (async)   <-INDICATION_DONE----          (n times)                      */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef MRP_MESSAGE
/*  2008-01-07  mk    syntax check of MRP_DomainName.                        */
/*                    Padding of PDInterfaceMrpDataReal revised.             */
/*                    mrm_stop with wait time. Added private records in check*/
/*                    of prm_read. Starvation handling for alarms revised.   */
/*                    Capability check extended.                             */
/*  2008-01-09  mk    Capability check revised.                              */
/*  2008-02-29  mk    ARD/BAD_PRJ values added.                              */
/*                    ReadCommand for B-Buffer.                              */
/*                    PRM_END consistency check revised.                     */
/*  2008-03-04  mk    PrivInterfaeMrpDataAdjust add MRP_Diag.                */
/*                    CheckRecord and MRC will be denied.                    */
/*  2008-04-16  mk    new traces added. STDBY reconfig revised.              */
/*  2008-06-06  mk    mrp_mgr_start port order chg for isolated/static ports.*/ 
/*                    alarm specifier changed:                               */ 
/*                    single side rx: MR -> ERROR                            */ 
/*                    multi rm: MR -> MD FATAL traces added.                 */
/*  2008-06-11  mk    topoCtrl added.                                        */
/*  2008-06-13  mk    stdby and mrp not allowd now.                          */
/*  2008-06-30  mk    allow hsr port mix up.                                 */
/*  2008-07-15  mk    check request handling revised.                        */
/*                    Traces added for parameter.                            */
/*                    hsr mgr call with correct port order.                  */
/*  2008-10-20  mk    replaced MRP_REC_ENUM_PARSE_TYPE in function           */
/*                    definitions because of compiler warnings.              */
/*                    morphing added. prm_indication portID check revised.   */
/*  2009-02-10  ds    some bugfixes for diag. handling,                      */
/*                    ringOpen detection, morphing                           */
/*  2009-03-19  ds    handling of state morphing corrected,                  */
/*                    ringport handling corrected, alarm handling corrected, */
/*                    user ind. handling corrected,                          */
/*                    MRP role handling corrected                            */
/*  2009-04-02  ds    CloseChannel failure fixed                             */
/*  2009-05-06  ds    Don't stop ARD, if only ringports changed              */
/*                    some additional alarm traces                           */
/*                    alarm handling reworked in case                        */
/*                    of overflow situations                                 */
/*  2009-06-22  ds    MRP compilable with switch #undef MRP_CFG_PLUGIN_0_MRM */
/*  2009-07-02  ds    include hierarchy changed                              */
/*  2009-10-15  ds    cleanups for LL services (fwdCtrl)                     */
/*  2009-10-22  ds    static timers not supported any longer, both MRP and   */
/*                    HSR use AUTO timers                                    */
/*  2009-11-11  ds    fixed pcIOX problem closeChannel in morphing state     */
/*  2009-12-03  ds    PRM_END rejection introduced, if PDInterfaceMrpDataAdj */
/*                    DS is missing                                          */
/*  2009-12-09  ds    mode change bugfixes MRM-->ARD (e.g. if mode changed to*/
/*                    ARD for the first time after reset)                    */
/*  2009-12-11  ds    DBs for MRMext and ARD                                 */
/*  2010-01-14  ds    new DS layout of DS PDInterfaceMrpDataReal according   */
/*                    to new block version low=1 of IEC 61158-6-10           */
/*  2010-02-16  ds    MRP based on 1 ms (HW) timer instead of 10 ms base     */
/*                    timer constants cleaned up, timer handling corrected   */
/*  2010-05-25  ds    new OHA upper channel related adaptations              */
/*  2010-06-28  ds    return DS return code MRP_RSP_ERR_PRM_INDEX instead of */
/*                    raising FATAL error, if unknown DS is read             */
/*  2010-07-19  ds    new feature MRP MIB implemented according to           */
/*                    IEC 62439-2                                            */
/*  2010-07-24  ds    bugfix in mrp_rec_activate_role: make mrp_rec.c        */
/*                    compilable, even if MRP_CFG_PLUGIN_2_HSR not set       */
/*  2011-07-21  ds    added an additional topologyChange request             */
/*                    if role is reconfigured from MRC to MRM (Auto), because*/
/*                    it won't be sent otherwise                             */
/*  2012-05-02  ds    introduction of N MRP instances per interface          */
/*  2018-10-19  hh    introduction of N MRP interconnection instances        */
/*                                                                           */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID   8
#define MRP_MODULE_ID      LTRC_ACT_MODUL_ID

#include "mrp_int.h"

MRP_FILE_SYSTEM_EXTENSION(MRP_MODULE_ID)



/*---------------------------------------------------------------------------*/
/*                            forward declarations                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                                globals                                    */
/*---------------------------------------------------------------------------*/


/*===========================================================================
* FUNCTION : mrp_rec_get_block_header
*----------------------------------------------------------------------------
* PURPOSE  : get block header information
*----------------------------------------------------------------------------
* RETURNS  : blockLength
*----------------------------------------------------------------------------
* INPUTS   : *pData=record data array
* OUTPUTS  : *pBlockType=type of block
*            *pBlockVersion=version of block
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_UINT16 mrp_rec_get_block_header (LSA_UINT8 *pData, LSA_UINT16 *pBlockType, LSA_UINT16 *pBlockVersion)
{
  LSA_UINT16 blockLength;

  MRP_REC_DB_GET16_NTOH(pData, *pBlockType);
  MRP_REC_DB_GET16_NTOH(pData, blockLength);
  MRP_REC_DB_GET16_NTOH(pData, *pBlockVersion);

  LSA_UNUSED_ARG(pData); /* pData is incremented in the previous MACRO, but not used afterwards => creates lint informational (e438). */

  return blockLength;
}

/*===========================================================================
* FUNCTION : mrp_rec_map_edd_role_cap_inst0
*----------------------------------------------------------------------------
* PURPOSE  : maps the EDD MRP role capability for the first MRP instance
*            to MRP enum role capability
*----------------------------------------------------------------------------
* RETURNS  : LSA_UINT16 roleCap
*----------------------------------------------------------------------------
* INPUTS   : pChUsr = user channel reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_UINT16 mrp_rec_map_edd_role_cap_inst0 (const MRP_CONST_CHANNEL_TYPE_USER pChUsr)
{
  LSA_UINT16 roleCap;
  MRP_IF_DB_TYPE *pDBif = (MRP_IF_DB_TYPE *)pChUsr->pChSysDB->pInterfacePara;

  roleCap = MRP_CAPABILITY_ROLE_OFF;

  if (pDBif->mrpSupportedRoleInst0 & EDD_MRP_ROLE_CAP_CLIENT)
    roleCap |= MRP_CAPABILITY_ROLE_CLIENT;

#ifdef MRP_CFG_PLUGIN_0_MRM
  if (pDBif->mrpSupportedRoleInst0 & EDD_MRP_ROLE_CAP_MANAGER)
    roleCap |= MRP_CAPABILITY_ROLE_MGR;
#endif

#ifdef MRP_CFG_PLUGIN_1_MRM_EXT
  if (pDBif->mrpSupportedRoleInst0 & EDD_MRP_ROLE_CAP_AUTOMANAGER)
    roleCap |= MRP_CAPABILITY_ROLE_AUTO_MGR;
#endif

  return roleCap;
}

/*===========================================================================
* FUNCTION : mrp_rec_map_edd_multi_role_cap
*----------------------------------------------------------------------------
* PURPOSE  : maps the EDD MRP role capability to MRP enum role capability,
*            if the device supports multiple MRP instances
*----------------------------------------------------------------------------
* RETURNS  : LSA_UINT16 roleCap
*----------------------------------------------------------------------------
* INPUTS   : pChUsr = user channel reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_UINT16 mrp_rec_map_edd_multi_role_cap (const MRP_CONST_CHANNEL_TYPE_USER pChUsr)
{
  LSA_UINT16 roleCap;
  MRP_IF_DB_TYPE *pDBif = (MRP_IF_DB_TYPE *)pChUsr->pChSysDB->pInterfacePara;

  roleCap = MRP_CAPABILITY_ROLE_OFF;

  if (pDBif->mrpSupportedMultiRole & EDD_MRP_ROLE_CAP_CLIENT)
    roleCap |= MRP_CAPABILITY_ROLE_CLIENT;

#ifdef MRP_CFG_PLUGIN_0_MRM
  if (pDBif->mrpSupportedMultiRole & EDD_MRP_ROLE_CAP_MANAGER)
    roleCap |= MRP_CAPABILITY_ROLE_MGR;
#endif

#ifdef MRP_CFG_PLUGIN_1_MRM_EXT
  if (pDBif->mrpSupportedMultiRole & EDD_MRP_ROLE_CAP_AUTOMANAGER)
    roleCap |= MRP_CAPABILITY_ROLE_AUTO_MGR;
#endif

  return roleCap;
}

/*===========================================================================
* FUNCTION : mrp_rec_map_edd_ic_role_cap_inst0
*----------------------------------------------------------------------------
* PURPOSE  : maps the EDD MRP interconnection role capability for the first 
*            MRP interconnection instance to MRP interconnection enum role capability
*----------------------------------------------------------------------------
* RETURNS  : LSA_UINT16 roleCap
*----------------------------------------------------------------------------
* INPUTS   : pChUsr = user channel reference
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_UINT16 mrp_rec_map_edd_ic_role_cap_inst0(const MRP_CONST_CHANNEL_TYPE_USER pChUsr)
{
  LSA_UINT16 roleCap;
  MRP_IF_DB_TYPE *pDBif = (MRP_IF_DB_TYPE *)pChUsr->pChSysDB->pInterfacePara;

  roleCap = MRP_IC_CAPABILITY_ROLE_OFF;

  if (pDBif->mrpInSupportedRoleInst0 & EDD_SUPPORTED_MRP_INTERCONN_ROLE_CAP_CLIENT)
  {
    if (pDBif->mrpInFwdRulesSupported)
    {
      roleCap |= MRP_IC_CAPABILITY_ROLE_CLIENT;
    }
    else
    {
      MRP_SYSTEM_TRACE_00(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_WARN, "mrp_rec_map_edd_ic_role_cap_inst0: client role supported, but no InFwdRules");
    }
  }

  if (pDBif->mrpInSupportedRoleInst0 & EDD_SUPPORTED_MRP_INTERCONN_ROLE_CAP_MANAGER)
  {
    if (pDBif->mrpInFwdRulesSupported)
    {
      roleCap |= MRP_IC_CAPABILITY_ROLE_MGR;
    }
    else
    {
      MRP_SYSTEM_TRACE_00(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_WARN, "mrp_rec_map_edd_ic_role_cap_inst0: manager role supported, but no InFwdRules");
    }   
  }

  return roleCap;
}

/*===========================================================================
* FUNCTION : mrp_rec_map_edd_def_role
*----------------------------------------------------------------------------
* PURPOSE  : map the EDD default MRP role to MRP enum default role
*----------------------------------------------------------------------------
* RETURNS  : LSA_UINT16 defRole
*----------------------------------------------------------------------------
* INPUTS   : pChUsr = user channel reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_UINT16 mrp_rec_map_edd_def_role (const MRP_CONST_CHANNEL_TYPE_USER pChUsr)
{
  LSA_UINT16 defRole;
  MRP_IF_DB_TYPE *pDBif = (MRP_IF_DB_TYPE *)pChUsr->pChSysDB->pInterfacePara;

  switch (pDBif->mrpDefaultRoleInst0)
  {
  case EDD_MRP_ROLE_NONE:
    defRole = MRP_PROJ_ROLE_OFF;
    break;

  case EDD_MRP_ROLE_CLIENT:
    defRole = MRP_PROJ_ROLE_CLIENT;
    break;

  case EDD_MRP_ROLE_MANAGER:
    defRole = MRP_PROJ_ROLE_MGR;
    break;

  case EDD_MRP_ROLE_AUTOMANAGER:
    defRole = MRP_PROJ_ROLE_AUTO_MGR;
    break;

  default:
    defRole = MRP_PROJ_ROLE_OFF;
    break;
  }

  return defRole;
}

/*===========================================================================
* FUNCTION : mrp_rec_map_admin_role_to_proj_role
*----------------------------------------------------------------------------
* PURPOSE  : get the admitted admin role out of proj parameters
*----------------------------------------------------------------------------
* RETURNS  : MRP_PROJ_ROLE_ENUM_T
*----------------------------------------------------------------------------
* INPUTS   : pCh          = channel reference for system or user channel
*            isNewProj    = indicate, if new or old provisioning is used
*            adminRole    = MRP admin role
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
MRP_PROJ_ROLE_ENUM_T mrp_rec_map_admin_role_to_proj_role (const MRP_CONST_CHANNEL_TYPE_COMMON pCh, const LSA_UINT8 isNewProj, const MRP_ADMIN_ROLE_ENUM_T adminRole)
{
  MRP_PROJ_ROLE_ENUM_T projRole;

  LSA_UNUSED_ARG(pCh);   /* hhr: to please the compiler in case traces are disabled. */

  projRole = MRP_PROJ_ROLE_OFF;

  if (isNewProj)
  {
    /*lint -e788*/
    switch (adminRole)
    {
      case MRP_ADMIN_ROLE_OFF:
        projRole = MRP_PROJ_ROLE_OFF;
      break;
      
      case MRP_ADMIN_ROLE_CLIENT:
        projRole = MRP_PROJ_ROLE_CLIENT;
      break;
      
#ifdef MRP_CFG_PLUGIN_0_MRM
      case MRP_ADMIN_ROLE_MGR:
        projRole = MRP_PROJ_ROLE_MGR;
      break;
      
 #ifdef MRP_CFG_PLUGIN_1_MRM_EXT
      case MRP_ADMIN_ROLE_AUTO_MGR:
        projRole = MRP_PROJ_ROLE_AUTO_MGR;
      break;
 #endif
#endif
      
      default:
        MRP_SYSTEM_TRACE_01(pCh->pChSysDB->traceIdx, LSA_TRACE_LEVEL_FATAL, "mrp_rec_map_admin_role_to_proj_role: adminRole=%d with new dataset version not allowed", adminRole);
      break;
    }
  }
  else /* old dataset versions */
  {
    switch (adminRole)
    { 
      case MRP_ADMIN_ROLE_OFF:
        projRole = MRP_PROJ_ROLE_OFF;
      break;

      case MRP_ADMIN_ROLE_CLIENT:
        projRole = MRP_PROJ_ROLE_CLIENT;
      break;

#ifdef MRP_CFG_PLUGIN_0_MRM
 #ifndef MRP_CFG_PLUGIN_1_MRM_EXT      
      case MRP_ADMIN_ROLE_MGR:
        projRole = MRP_PROJ_ROLE_MGR;
      break;
 #endif
      
 #ifdef MRP_CFG_PLUGIN_1_MRM_EXT      
      case MRP_ADMIN_ROLE_AUTO_MGR:
        projRole = MRP_PROJ_ROLE_MGR;
      break;

      case MRP_ADMIN_ROLE_MGR:
        /*fall through*/
  #endif
#endif

      default:
        MRP_SYSTEM_TRACE_01(pCh->pChSysDB->traceIdx, LSA_TRACE_LEVEL_FATAL, "mrp_rec_map_admin_role_to_proj_role: projRole=%d with old dataset version not allowed", projRole);
      break;  
    }
  }
  MRP_SYSTEM_TRACE_01(pCh->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_map_admin_role_to_proj_role: projRole=%d", projRole);

  return projRole;
}

/*===========================================================================
* FUNCTION : mrp_rec_map_proj_role_to_admin_role
*----------------------------------------------------------------------------
* PURPOSE  : get the admitted admin role out of proj parameters
*----------------------------------------------------------------------------
* RETURNS  : MRP_ADMIN_ROLE_ENUM_T
*----------------------------------------------------------------------------
* INPUTS   : pCh       = channel reference for system or user channel
*            isNewProj = indicate, if new or old provisioning is used
*            projRole  = MRP role within provisioning dataset
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
MRP_ADMIN_ROLE_ENUM_T mrp_rec_map_proj_role_to_admin_role (const MRP_CONST_CHANNEL_TYPE_COMMON pCh, const LSA_UINT8 isNewProj, const MRP_PROJ_ROLE_ENUM_T projRole)
{
  MRP_ADMIN_ROLE_ENUM_T adminRole;

  LSA_UNUSED_ARG(pCh);   /* hhr: to please the compiler in case traces are disabled. */

  adminRole = MRP_ADMIN_ROLE_OFF;

  if (isNewProj)
  {
    switch (projRole)
    {
      case MRP_PROJ_ROLE_OFF:
        adminRole = MRP_ADMIN_ROLE_OFF;
      break;
      
      case MRP_PROJ_ROLE_CLIENT:
        adminRole = MRP_ADMIN_ROLE_CLIENT;
      break;
      
#ifdef MRP_CFG_PLUGIN_0_MRM
      case MRP_PROJ_ROLE_MGR:
        adminRole = MRP_ADMIN_ROLE_MGR;
      break;
      
 #ifdef MRP_CFG_PLUGIN_1_MRM_EXT      
      case MRP_PROJ_ROLE_AUTO_MGR:
        adminRole = MRP_ADMIN_ROLE_AUTO_MGR;
      break;
 #endif
#endif
      
      default:
        MRP_SYSTEM_TRACE_01(pCh->pChSysDB->traceIdx, LSA_TRACE_LEVEL_FATAL, "mrp_rec_map_proj_role_to_admin_role: projRole=%d with new dataset version not allowed", projRole);
      break;
    }
  }
  else /* old dataset versions */
  {
    switch (projRole)
    {
      case MRP_PROJ_ROLE_OFF:
        adminRole = MRP_ADMIN_ROLE_OFF;
      break;

      case MRP_PROJ_ROLE_CLIENT:
        adminRole = MRP_ADMIN_ROLE_CLIENT;
      break;

#ifdef MRP_CFG_PLUGIN_0_MRM
      case MRP_PROJ_ROLE_MGR:
 #ifdef MRP_CFG_PLUGIN_1_MRM_EXT      
        adminRole = MRP_ADMIN_ROLE_AUTO_MGR;
 #else
        adminRole = MRP_ADMIN_ROLE_MGR;
 #endif
      break;


 #ifdef MRP_CFG_PLUGIN_1_MRM_EXT      
      case MRP_PROJ_ROLE_AUTO_MGR:
        /*fall through*/
 #endif
#endif

      default:
        MRP_SYSTEM_TRACE_01(pCh->pChSysDB->traceIdx, LSA_TRACE_LEVEL_FATAL, "mrp_rec_map_proj_role_to_admin_role: projRole=%d with old dataset version not allowed", projRole);
      break;  
    }
  }
  MRP_SYSTEM_TRACE_01(pCh->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_map_proj_role_to_admin_role: adminRole=%d", adminRole);

  return adminRole;
}


/*===========================================================================
* FUNCTION : mrp_rec_get_rport_cnt
*----------------------------------------------------------------------------
* PURPOSE  : get the number of ports indicated as ringports
*----------------------------------------------------------------------------
* RETURNS  : rportCnt = number of ringports
*----------------------------------------------------------------------------
* INPUTS   : pChUsr = user channel reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_UINT16 mrp_rec_get_rport_cnt (const MRP_CONST_CHANNEL_TYPE_USER pChUsr)
{
  LSA_UINT16 rportCnt, i;
  MRP_IF_DB_TYPE *pDBif = (MRP_IF_DB_TYPE *)pChUsr->pChSysDB->pInterfacePara;

  for (i = 0, rportCnt = 0; i < pDBif->numOfPorts; i++)
  {
    if (pDBif->pPortData[i].rpType == EDD_MRP_RING_PORT || pDBif->pPortData[i].rpType == EDD_MRP_RING_PORT_DEFAULT)
      rportCnt++;
  }

  return rportCnt;
}

/*===========================================================================
* FUNCTION : mrp_rec_map_edd_ports_to_rports
*----------------------------------------------------------------------------
* PURPOSE  : map the edd port array to a rport array, in which the default
*            rports are sorted to array index[0] and[1]
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChUsr = user channel reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_rec_map_edd_ports_to_rports (const MRP_CONST_CHANNEL_TYPE_USER pChUsr)
{
  LSA_UINT16 i, k;
  MRP_IF_DB_TYPE *pDBif = (MRP_IF_DB_TYPE *)pChUsr->pChSysDB->pInterfacePara;
  MRP_REC_DB_TYPE *pDBrec = (MRP_REC_DB_TYPE *)pChUsr->pChUsrDB->pRec;

  /* map the default ringports first */
  for (i = 0, k = 0; i < pDBif->numOfPorts; i++)
  {
    if (pDBif->pPortData[i].rpType == EDD_MRP_RING_PORT_DEFAULT)
    {
      pDBrec->pRPortArray[k] = (LSA_UINT16)(i + 1); /* PortID starts from 1 and maps directly to (i + 1) */
      k++;
    }
  }
  MRP_FATAL1(k <= 2);
  /* and now map the ports, which could be ringports by provisioning */
  for (i = 0; i < pDBif->numOfPorts; i++)
  {
    if (pDBif->pPortData[i].rpType == EDD_MRP_RING_PORT)
    {
      pDBrec->pRPortArray[k] = (LSA_UINT16)(i + 1); /* PortID starts from 1 and maps directly to (i + 1) */
      k++;
    }
    MRP_FATAL1(k <= pDBrec->NumOfPossibleRports);
  }
}


/*===========================================================================
* FUNCTION : mrp_rec_map_edd_ports_to_inports
*----------------------------------------------------------------------------
* PURPOSE  : map the edd port array to a inport array
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChUsr = user channel reference
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_VOID mrp_rec_map_edd_ports_to_inports(const MRP_CONST_CHANNEL_TYPE_USER pChUsr)
{
  LSA_UINT16 i, k;
  MRP_IF_DB_TYPE *pDBif = (MRP_IF_DB_TYPE *)pChUsr->pChSysDB->pInterfacePara;
  MRP_REC_DB_TYPE *pDBrec = (MRP_REC_DB_TYPE *)pChUsr->pChUsrDB->pRec;

  /* map the interconnection ringports */
  for (i = 0, k = 0; i < pDBif->numOfPorts; i++)
  {
      if (pDBif->pPortData[i].inPortType == EDD_SUPPORTS_MRP_INTERCONN_PORT_CONFIG_YES)
      {
          pDBrec->pInPortArray[k] = (LSA_UINT16)(i + 1); /* PortID starts from 1 and maps directly to (i + 1) */
          k++;
      }
      MRP_FATAL1(k <= pDBrec->NumOfPossibleInPorts);
  }
}

/*===========================================================================
* FUNCTION : mrp_rec_parse_helper_prio
*----------------------------------------------------------------------------
* PURPOSE  : check correct prio values
*----------------------------------------------------------------------------
* RETURNS  : error (MRP_RSP_OK,MRP_RSP_ERR_PARAM)
*----------------------------------------------------------------------------
* INPUTS   : Prio=MRP_Prio
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_UINT16 mrp_rec_parse_helper_prio (LSA_UINT16 Prio)
{
  if (((0x0000 < Prio) && (0x1000 > Prio)) || ((0x7000 < Prio) && (0x8000 > Prio)) || ((0x8000 < Prio) && (0x9000 > Prio)) || 
       ((0xE000 < Prio) && (0xF000 > Prio)) || (0xF000 < Prio)) 
  {
    return MRP_RSP_ERR_PARAM;
  }
  return MRP_RSP_OK;
}

#ifdef MRP_CFG_PLUGIN_0_MRM
/*===========================================================================
* FUNCTION : mrp_rec_get_mrp_edition
*----------------------------------------------------------------------------
* PURPOSE  : Get MRP edition to be used for MRA frame send procedure, 
*            based on versions of provisioning data set 
*----------------------------------------------------------------------------
* RETURNS  : MRP_MRM_EDITION_ENUM_T edition: edition of IEC62439-2, the MRA
             conforms to
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec        = reference to record data structure
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static MRP_MRM_EDITION_ENUM_T mrp_rec_get_mrp_edition (MRP_REC_DB_TYPE *pDBrec)
{
  LSA_UINT8 ed2_flag;
  MRP_MRM_EDITION_ENUM_T edition;
  MRP_REC_DB_IF_SET_TYPE *pRecIfWork;

  pRecIfWork = &pDBrec->work;
  ed2_flag = mrp_sys_get_ed2_flag (pDBrec->pChSys);

  if ( !ed2_flag || pRecIfWork->RecIfAdjustHead.head.blockVersionLow == MRP_REC_DATA_BLK_VERS_LOW0)
    edition = MRP_MRM_EDITION_1;
  else
    edition = MRP_MRM_EDITION_2_COMPATIBLE;

  return edition;
}
#endif

/*===========================================================================
* FUNCTION : mrp_rec_parse_helper_domain_name
*----------------------------------------------------------------------------
* PURPOSE  : parse in record MRP_LengthDomainName/MRPIC_LengthDomainName, 
*            MRP_DomainName/MRPIC_DomainName and padding for it. 
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_BLOCK)
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec        = reference to record data structure
*            **ppData       = record data array reference
*            **pDomainName  = LSA_NULL = no padding parsing
* OUTPUTS  : 
*            **ppData                 = record data array reference
*            *lengthDomainName        = length of domain name
*            *lengthDomainNamePadding = length of domain name padding
*            **pDomainName            = pointer to domain name reference in buffer
*----------------------------------------------------------------------------
* COMMENTS : used to parse MRP_LengthDomainName/MRPIC_LengthDomainName and 
*            MRP_DomainName/MRPIC_DomainName and field in InterfaceMrpDataAdjust/
*            PortMrpIcDataAdjust and to calculate lengthName and padding which is 
*            needed to align the block.
*==========================================================================*/
static LSA_UINT16 mrp_rec_parse_helper_domain_name (const MRP_REC_DB_TYPE *pDBrec, 
                                                                LSA_UINT8 **ppData, 
                                                                LSA_UINT8 *lengthDomainName,
                                                                LSA_UINT8 *lengthDomainNamePadding,
                                                                LSA_UINT8 **pDomainName)
{
  LSA_UINT8  LengthDomainName;
  LSA_UINT8  LengthDomainNamePadding;
  LSA_UINT8 *pParseData = *ppData;
  LSA_UINT16 idx;
  LSA_UINT16 lbl;
  LSA_UINT16 nr_of_labels;
  LSA_UINT8 looks_like_ipaddr;

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBrec->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

#if (MRP_CFG_TRACE_MODE == 0)
  LSA_UNUSED_ARG(pDBrec);
#endif
#if (MRP_CFG_TRACE_MODE == 1)
  LSA_UNUSED_ARG(pDBrec);
#endif
  *lengthDomainName = LengthDomainName = *pParseData;

  if ((LengthDomainName == 0) ||  LengthDomainName > MRP_DEFAULT_DOMAIN_NAME_SIZE_MAX) 
  {
    MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: LengthDomainName (0x%x)", LengthDomainName);

    *ppData = pParseData;
    return MRP_RSP_ERR_PRM_BLOCK;
  }

  pParseData++;

  /* calculate padding for MRP_DomainName */
  *lengthDomainNamePadding = LengthDomainNamePadding = (LSA_UINT8)((~LengthDomainName)&3);
  if (!LSA_HOST_PTR_ARE_EQUAL(pDomainName, LSA_NULL)) 
  {
    /* set reference to MRP_DomainName */
    *pDomainName = pParseData;
    pParseData += LengthDomainName;
    /* check padding for MRP_DomainName */
    while (LengthDomainNamePadding) 
    {
      if (*pParseData != 0) 
      {
        MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: DomainNamePadding (0x%x)", *pParseData);
        
        *ppData = pParseData;
        return MRP_RSP_ERR_PRM_BLOCK;
      }
      LengthDomainNamePadding--;
      pParseData++;
    }
  }
  else 
  {
    return MRP_RSP_OK;
  }
  /* check syntax */
  /*
   * a station-name has the following syntax:
   * (see AP00216830)
   *
   * 1 or more labels, separated by[.]
   * total length is 1-240
   * label-length is 1-63
   * labels consist of[a-z0-9-]
   * labels do not start with[-]
   * labels do not end with[-]
   * the first (leftmost) label does not match with "port-xyz" with x, y, z = 0...9
   * the first (leftmost) label does not match with "port-xyz-abcde" with x, y, z, a, b, c, d, e = 0...9
   * station-names do not have the form n.n.n.n, n = 0...999
   * labels do only start with 'xn-' if RFC 3490 is applied
   */

  MRP_ASSERT( /* ascii */
    '-' == 0x2d
    && '.' == 0x2e
    && '0' == 0x30
    && 'a' == 0x61
    && 'A' == 0x41
    && '0' < '9'
    && 'a' < 'z'
    && 'A' < 'Z'
  );

  nr_of_labels = 0;
  looks_like_ipaddr = LSA_TRUE;
  /* start of label */
  lbl = 0;

  for (idx = 0; idx <= LengthDomainName; ++idx) 
  {
    LSA_UINT8 ch;

    if( idx < LengthDomainName) 
    {
      ch = (*pDomainName)[idx];
    }
    else 
    {
      ch='.'; /* fake a dot at the end */
    }
    if ( '.' == ch) 
    {
      LSA_UINT16 len = idx - lbl;

      if (len < 1 || len > MRP_REC_DATA_LEN_DOMAIN_NAME_MAX) 
      {
        MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: DomainName label is too short or too long (idx=%u)", idx);

        *ppData = *pDomainName + idx;
        return MRP_RSP_ERR_PRM_BLOCK;
      }
      MRP_ASSERT(idx > 0);
      if( '-' == (*pDomainName)[idx - 1]) 
      {
        MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: DomainName label ends with '-' (idx=%u)", idx);

        *ppData = *pDomainName + idx - 1;
        return MRP_RSP_ERR_PRM_BLOCK;
      }
      if (len > 3) 
      {
        /* note: port-names are longer than 3 chars */
        /* the label has more than 3 characters */
        looks_like_ipaddr = LSA_FALSE; 

        if (!nr_of_labels) 
        {
          /* the first (leftmost) label matches a port-name */
          /*
           * port-names have the following forms (see the definition of station-name below)
           *
           * "port-xyz" with x, y, z = 0...9
           * "port-xyz-abcde" with x, y, z, a, b, c, d, e = 0...9
           */         
          if (len >= 8
              && (((*pDomainName)[0] == 'p') || ((*pDomainName)[0] == 'P')) 
              && (((*pDomainName)[1] == 'o') || ((*pDomainName)[1] == 'O'))
              && (((*pDomainName)[2] == 'r') || ((*pDomainName)[2] == 'R'))
              && (((*pDomainName)[3] == 't') || ((*pDomainName)[3] == 'T'))
              &&  ((*pDomainName)[4] == '-')
              && (((*pDomainName)[5] >= '0') && ((*pDomainName)[5] <= '9'))
              && (((*pDomainName)[6] >= '0') && ((*pDomainName)[6] <= '9'))
              && (((*pDomainName)[7] >= '0') && ((*pDomainName)[7] <= '9'))) 
          {
            if (len == 8) 
            {
              MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: DomainName label matches with[port-xyz] with x, y, z = 0...9 (idx=%u)", idx);
              
              *ppData = *pDomainName + idx;
              return MRP_RSP_ERR_PRM_BLOCK;
            }
            if (len == 14
                &&  ((*pDomainName)[ 8] == '-') 
                && (((*pDomainName)[ 9] >= '0') && ((*pDomainName)[ 9] <= '9'))
                && (((*pDomainName)[10] >= '0') && ((*pDomainName)[10] <= '9'))
                && (((*pDomainName)[11] >= '0') && ((*pDomainName)[11] <= '9'))
                && (((*pDomainName)[12] >= '0') && ((*pDomainName)[12] <= '9'))
                && (((*pDomainName)[13] >= '0') && ((*pDomainName)[13] <= '9'))) 
            {
              MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: DomainName label matches with[port-xyz-abcde] with x, y, z, a, b, c, d, e = 0...9 (idx=%u)", idx);
              
              *ppData = *pDomainName + idx;
              return MRP_RSP_ERR_PRM_BLOCK;
            }          
          }
        }				
      }
      nr_of_labels += 1;
      /* start of the next label */
      lbl = idx + 1;
      continue;
    }
    if( ('0' <= ch) && ('9' >= ch)) 
    {
      continue;
    }
    /* the label contains not only digits */
    looks_like_ipaddr = LSA_FALSE; 
    if( ('a' <= ch) && ('z' >= ch)) 
    {
      continue;
    }
    if( ('A' <= ch) && ('Z' >= ch)) 
    {
      /* for backwards-compatibility: allow upper-case here, convert in cm_station_name_copy() */
      continue;
    }
    if( idx == lbl) 
    {
      MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: DomainName does not start with a letter or digit (idx=%u)", idx);

      *ppData = *pDomainName + idx;
      return MRP_RSP_ERR_PRM_BLOCK;
    }
    if( '-' == ch) 
    {
      continue;
    }
    MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: DomainName contains an illegal char (idx=%u)", idx);

    *ppData = *pDomainName + idx;
    return MRP_RSP_ERR_PRM_BLOCK;
  }
  if( looks_like_ipaddr && 4 == nr_of_labels) 
  {
    MRP_UPPER_TRACE_00(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: DomainName looks like an ip-addr");

    *ppData = *pDomainName + idx;
    return MRP_RSP_ERR_PRM_BLOCK;
  }
  return MRP_RSP_OK;
}


#ifdef MRP_CFG_PLUGIN_2_HSR
/*===========================================================================
* FUNCTION : mrp_rec_parse_helper_stby
*----------------------------------------------------------------------------
* PURPOSE  : parse in record PrivInterfaceStdbyDataAdjust and
*            PrivInterfaceStdbyDataReal calculate padding for it.
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_BLOCK)
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec        = reference to record data structure
*            **ppData       = record data array reference
*            maxLength      = maximum data length
* OUTPUTS  : 
*            **ppData       = record data array reference
*            *pLength       = length
*            *lengthPadding = length of padding
*            **pName        = pointer to name reference in buffer
*----------------------------------------------------------------------------
* COMMENTS : used to parse MRP_LengthDomainName/MRP_DomainName field in 
*            InterfaceMrpDataAdjust and to calculate lengthName and padding
*            which is need to align the block.
*==========================================================================*/
static LSA_UINT16 mrp_rec_parse_helper_stby (const MRP_REC_DB_TYPE *pDBrec,
                                                                LSA_UINT8 **ppData,
                                                                LSA_UINT8 maxLength,
                                                                LSA_UINT8 *pLength,
                                                                LSA_UINT8 *lengthPadding,
                                                                LSA_UINT8 **pName)
{
  LSA_UINT8  Length;
  LSA_UINT8  LengthPadding;
  LSA_UINT8 *pParseData = *ppData;

#if (MRP_CFG_TRACE_MODE == 0)
  LSA_UNUSED_ARG(pDBrec);
#endif
#if (MRP_CFG_TRACE_MODE == 1)
  LSA_UNUSED_ARG(pDBrec);
#endif
  *pLength = Length = *pParseData;

  if (maxLength < Length) 
  {
    MRP_UPPER_TRACE_01(pDBrec->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: StBy Length (%u)", Length);

    *ppData = pParseData;
    return MRP_RSP_ERR_PRM_BLOCK;
  }

  pParseData++;

  /* calculate padding  */
  *lengthPadding = LengthPadding = (LSA_UINT8)((~Length)&3);
  /* set reference */
  *pName      = pParseData;
  pParseData += Length;

  /* check padding */
  while (LengthPadding) 
  {
    if (0 != *pParseData) 
    {
      MRP_UPPER_TRACE_01(pDBrec->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: Padding (0x%x)", *pParseData);

      *ppData = pParseData;
      return MRP_RSP_ERR_PRM_BLOCK;
    }
    LengthPadding--;
    pParseData++;
  }
  return MRP_RSP_OK;
}
#endif

/*===========================================================================
* FUNCTION : mrp_rec_parse_blk_type_inst_data_adjust
*----------------------------------------------------------------------------
* PURPOSE  : parse blockType MRP_REC_DATA_BLK_TYPE_INST_DATA_ADJUST of dataset
*            PDInterfaceMrpDataAdjust
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_BLOCK)
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec             = internal data base reference
*            parseLevel          = state of parsing
*            *pNumOfMrpInstances = max. number of MRP instances
*            blockType           = type of block
*            blockLen            = blockLength of block (including type and length fields)
*            blockVersion        = version of blockType
* OUTPUTS  : **ppData            = record data array reference
*            *pLength            = length of block
*            *pInstance          = MRP instance for which the dataset is parsed
*----------------------------------------------------------------------------
* COMMENTS : after correct parsing, *pParseData points to the end of block and
*            *pLength contains the block length.
*==========================================================================*/
static LSA_UINT16 mrp_rec_parse_blk_type_inst_data_adjust (MRP_REC_DB_TYPE *pDBrec, 
                                                                       LSA_INT32 parseLevel,
                                                                       LSA_UINT8 **ppData, 
                                                                       LSA_UINT16 *pLength,
                                                                       const LSA_UINT16 *pNumOfMrpInstances,
                                                                       LSA_UINT16 *pInstance,
                                                                       LSA_UINT16 blockType,
                                                                       LSA_UINT16 blockLen,
                                                                       LSA_UINT16 blockVersion)
{
  LSA_UINT16 mrpMaxInstance, instance, rsp, i;
  LSA_UINT8  *pDomainUUID;
  LSA_UINT16 Role, Padding;
  LSA_UINT8  LengthDomainName;
  LSA_UINT8  LengthDomainNamePadding;
  LSA_UINT8  *pDomainName;
  MRP_REC_DB_INST_SET_TYPE *pInstPara;
  LSA_UINT8 *pParseData = *ppData;

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBrec->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  mrpMaxInstance = mrp_sys_get_num_of_instances(pDBrec->pChSys);
  instance   = 0;

  if (parseLevel == MRP_REC_ENUM_PARSE_VALUE) 
  {
    MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> parsing MrpInstanceDataAdjust (blockType=0x%x, (blockLen + 4)=%d, blockVersion=0x%x)", blockType, blockLen, blockVersion);
  }

  if (blockVersion == MRP_REC_DATA_BLK_VERS_HIGH1_LOW0)
  {
    /* calc length of dataset including bytes after blockHeader and padding bytes after MRP role */
    /* dataset pointer points to BLK_HEADER_PADDING */
    if (parseLevel == MRP_REC_ENUM_PARSE_LEN)
    {
      if (*pLength < (MRP_REC_DATA_LEN_BLK_HEADER + MRP_REC_DATA_LEN_BLK_HEADER_PADDING + MRP_REC_DATA_LEN_UUID_AND_ROLE + MRP_REC_DATA_LEN_ROLE_PADDING)) 
      {
        MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_DATA: blockType=0x%x, blockLen(=%d) mismatch", blockType, *pLength);

        *ppData = pParseData - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
        return MRP_RSP_ERR_PRM_DATA;
      }
    }

    pParseData += (MRP_REC_DATA_LEN_BLK_HEADER_PADDING + MRP_REC_DATA_LEN_UUID_AND_ROLE + MRP_REC_DATA_LEN_ROLE_PADDING);
    
    rsp = mrp_rec_parse_helper_domain_name (pDBrec, &pParseData, &LengthDomainName, &LengthDomainNamePadding, LSA_NULL);

    if (rsp != MRP_RSP_OK) 
    {
      *ppData = pParseData;
      return rsp;
    }
    pParseData -= (MRP_REC_DATA_LEN_BLK_HEADER_PADDING + MRP_REC_DATA_LEN_UUID_AND_ROLE + MRP_REC_DATA_LEN_ROLE_PADDING);

    if (parseLevel >= MRP_REC_ENUM_PARSE_VALUE) 
    {
      MRP_REC_DB_GET16_NTOH(pParseData, instance); /* padding byte and MRP_Instance after BlockHeader */
      if (instance >= mrpMaxInstance || *pNumOfMrpInstances > mrpMaxInstance) /* *pNumOfMrpInstances is set with content of field NumberOfMrpInstances from blockType PDInterfaceMrpDataAdjust (0x211) */
      {
        MRP_UPPER_TRACE_04(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: blockType=0x%x, instance value(=0x%x) or numOfMrpInstances=%d wrong, maxInst=%d", blockType, instance, *pNumOfMrpInstances, mrpMaxInstance);

        *ppData = pParseData - sizeof(LSA_UINT16);
        return MRP_RSP_ERR_PRM_BLOCK;
      }
      if (parseLevel >= MRP_REC_ENUM_PARSE_WRITE)
      {
        if (*pNumOfMrpInstances > 1)
        {
          for (i = 0; i < mrpMaxInstance; i++)
          {
            pInstPara = &pDBrec->pInstData[i].para;
            if (pInstPara->RecInstanceAdjust.Written)
            {
              if (pInstPara->RecInstanceAdjust.head.v.v_new.instance == (LSA_UINT8)instance)
              {
                MRP_UPPER_TRACE_04(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: blockType=0x%x, instance(=0x%x) value already used, numOfMrpInstances=%d, maxInst=%d", blockType, instance, *pNumOfMrpInstances, mrpMaxInstance);

                *ppData = pParseData - sizeof(LSA_UINT16);
                return MRP_RSP_ERR_PRM_BLOCK;
              }
            }
          }
        }
      }

      pDomainUUID = pParseData;

      if (parseLevel >= MRP_REC_ENUM_PARSE_WRITE)
      {
        if (*pNumOfMrpInstances > 1)
        {
          for (i = 0; i < mrpMaxInstance; i++)
          {
            pInstPara = &pDBrec->pInstData[i].para;
            if (pInstPara->RecInstanceAdjust.Written)
            {
              if (!MRP_MEMCMP(pInstPara->RecInstanceAdjust.DomainUUID, pDomainUUID, sizeof(pInstPara->RecInstanceAdjust.DomainUUID)))
              {
                MRP_UPPER_TRACE_04(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: blockType=0x%x, domainUUID not different between two inst (%d and %d), numOfMrpInstances=%d", blockType, i, instance, *pNumOfMrpInstances);

                *ppData = pParseData;
                return MRP_RSP_ERR_PRM_BLOCK;
              }
            }
          }
        }
      }

      pParseData += MRP_REC_DATA_LEN_UUID;

      MRP_REC_DB_GET16_NTOH(pParseData, Role);
      pDBrec->tmpParsedProjRole = Role;
      
      if (!(Role == MRP_PROJ_ROLE_OFF ||  Role == MRP_PROJ_ROLE_CLIENT || Role == MRP_PROJ_ROLE_MGR || Role == MRP_PROJ_ROLE_AUTO_MGR)) 
      {
        MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: blockType=0x%x, MRP_Role (%u) not supported", instance, blockType, Role);

        *ppData = pParseData - sizeof(LSA_UINT16);
        return MRP_RSP_ERR_PRM_BLOCK;
      }
      
      if (*pNumOfMrpInstances == 1)
      {
#ifdef MRP_CFG_PLUGIN_0_MRM
        if ( (Role == MRP_PROJ_ROLE_MGR      && !(MRP_CAPABILITY_ROLE_MGR      & pDBrec->SupportedRoleInst0)) || 
             (Role == MRP_PROJ_ROLE_AUTO_MGR && !(MRP_CAPABILITY_ROLE_AUTO_MGR & pDBrec->SupportedRoleInst0))
          ) 
        {
          MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: blockType=0x%x, MRP_Role (%u) not supported", instance, blockType, Role);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        } 
 #ifndef MRP_CFG_PLUGIN_1_MRM_EXT
        if (Role == MRP_PROJ_ROLE_AUTO_MGR)
        {
          MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: blockType=0x%x, MRP_Role (%u) not supported", instance, blockType, Role);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }  
 #endif
#else
        if (!(Role == MRP_PROJ_ROLE_OFF || Role == MRP_PROJ_ROLE_CLIENT)) 
        {
          MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: blockType=0x%x, MRP_Role (%u) not supported", instance, blockType, Role);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
#endif     
        if (Role == MRP_PROJ_ROLE_CLIENT && !(MRP_CAPABILITY_ROLE_CLIENT & pDBrec->SupportedRoleInst0)) 
        {
          MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: blockType=0x%x, MRP_Role (%u) not supported", instance, blockType, Role);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
      }
      else /* numOfMrpInstances > 1 */
      {
#ifdef MRP_CFG_PLUGIN_0_MRM
        if ( (Role == MRP_PROJ_ROLE_MGR      && !(MRP_CAPABILITY_ROLE_MGR      & pDBrec->SupportedMultiRole)) || 
             (Role == MRP_PROJ_ROLE_AUTO_MGR && !(MRP_CAPABILITY_ROLE_AUTO_MGR & pDBrec->SupportedMultiRole))
           ) 
        {
          MRP_UPPER_TRACE_04(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: blockType=0x%x, MRP_Role (%u) for multi inst(=%d) not supported", instance, blockType, Role, *pNumOfMrpInstances);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }                
 #ifndef MRP_CFG_PLUGIN_1_MRM_EXT
        if (Role == MRP_PROJ_ROLE_AUTO_MGR)
        {
          MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: blockType=0x%x, MRP_Role (%u) not supported", instance, blockType, Role);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }  
 #endif
#else
        if (!(Role == MRP_PROJ_ROLE_OFF || Role == MRP_PROJ_ROLE_CLIENT)) 
        {
          MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: blockType=0x%x, MRP_Role (%u) not supported", instance, blockType, Role);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
#endif
        if (Role == MRP_PROJ_ROLE_CLIENT && !(MRP_CAPABILITY_ROLE_CLIENT & pDBrec->SupportedMultiRole))
        {
          MRP_UPPER_TRACE_04(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: blockType=0x%x, MRP_Role (%u) for multi inst(=%d). not supported", instance, blockType, Role, *pNumOfMrpInstances);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;        
        }
      }

      MRP_REC_DB_GET16_NTOH(pParseData, Padding);
      if (Padding != 0) 
      {
        MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: Padding(0x%x) after MRP_Role wrong", instance, Padding);

        *ppData = pParseData - sizeof(LSA_UINT16);
        return MRP_RSP_ERR_PRM_BLOCK;
      }
      if ( (rsp = mrp_rec_parse_helper_domain_name (pDBrec, &pParseData, &LengthDomainName, &LengthDomainNamePadding, &pDomainName)) != MRP_RSP_OK) 
      {
        *ppData = pParseData;
        return rsp;
      }

      if (parseLevel >= MRP_REC_ENUM_PARSE_WRITE)
      {
        if (*pNumOfMrpInstances > 1)
        {
          for (i = 0; i < mrpMaxInstance; i++)
          {
            pInstPara = &pDBrec->pInstData[i].para;
            if (pInstPara->RecInstanceAdjust.Written)
            {
              if (LengthDomainName == pInstPara->RecInstanceAdjust.LengthDomainName &&
                  !MRP_MEMCMP(pInstPara->RecInstanceAdjust.DomainName, pDomainName, LengthDomainName))
              {
                MRP_UPPER_TRACE_04(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: blockType=0x%x, domainName not different between two inst (%d and %d), numOfMrpInstances=%d", blockType, i, instance, *pNumOfMrpInstances);

                *ppData = pParseData + sizeof(LengthDomainName);
                return MRP_RSP_ERR_PRM_BLOCK;
              }
            }
          }
        }
      }

      pParseData += sizeof(LengthDomainName) + LengthDomainName + LengthDomainNamePadding;

      if (parseLevel >= MRP_REC_ENUM_PARSE_WRITE) 
      {
        MRP_UPPER_TRACE_BYTE_ARRAY(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> parsing MrpInstanceDataAdjust[inst=see following trace] DomainUUID=", pDomainUUID, 6);
        MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> parsing MrpInstanceDataAdjust[inst=%d] Role=%u, LengthDomainName=%u", instance, Role, LengthDomainName);

        if (LengthDomainName >= 6)
        {
          MRP_UPPER_TRACE_07(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> parsing MrpInstanceDataAdjust[inst=%d] pDomainName=%c%c%c%c%c%c",
            instance, pDomainName[0], pDomainName[1], pDomainName[2], pDomainName[3], pDomainName[4], pDomainName[5]);
        }
        else
        {
          if (LengthDomainName >= 2)
          {
            MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> parsing MrpInstanceDataAdjust[inst=%d] pDomainName=%c%c", instance, pDomainName[0], pDomainName[1]);
          }
        }
        MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> parsing MrpInstanceDataAdjust[inst=%d] completed successfully", instance);
        pInstPara = &pDBrec->pInstData[instance].para;

        /* save instance related dataset contents */
        MRP_MEMCPY(pInstPara->RecInstanceAdjust.DomainUUID, pDomainUUID, sizeof(pInstPara->RecInstanceAdjust.DomainUUID));
        pInstPara->RecInstanceAdjust.Role                           = Role;
        pInstPara->RecInstanceAdjust.LengthDomainName               = LengthDomainName;
        MRP_MEMCPY(pInstPara->RecInstanceAdjust.DomainName, pDomainName, LengthDomainName);
        pInstPara->RecInstanceAdjust.LengthDomainNamePadding        = LengthDomainNamePadding;
        
        pInstPara->RecInstanceAdjust.head.v.v_new.blockType         = blockType;
        pInstPara->RecInstanceAdjust.head.v.v_new.blockLength       = blockLen - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN; /* blockLen includes length of type and length field */
        pInstPara->RecInstanceAdjust.head.v.v_new.blockVersionHigh  = MRP_REC_DATA_BLK_VERS_HIGH1;
        pInstPara->RecInstanceAdjust.head.v.v_new.blockVersionLow   = MRP_REC_DATA_BLK_VERS_LOW0;
        pInstPara->RecInstanceAdjust.head.v.v_new.padding           = 0;
        pInstPara->RecInstanceAdjust.head.v.v_new.instance          = (LSA_UINT8)instance;

        *ppData -= MRP_REC_DATA_LEN_BLK_HEADER;
        pInstPara->RecInstanceAdjust.err_offset_base                = MRP_REC_DATA_OFFSET_GET(pDBrec->tmpParsedBaseAddr, *ppData);
        pInstPara->RecInstanceAdjust.Written                        = LSA_TRUE;
        
        pInstPara->adminRole                                        = mrp_rec_map_proj_role_to_admin_role(pDBrec->pChSys, LSA_TRUE, (MRP_PROJ_ROLE_ENUM_T)Role);
      }
    }
    /* *pLength = length calculated without blockType and blockLength field (=4 Bytes). This bytes will be added at the end of this function!!!           */
    /* MRP_REC_DATA_BLK_LEN_IF_DATA_ADJ_VERS0_MIN (25): blockVersion (2) + padding (2) + uuid (16) + mrp_role (2) + padding (2) + lengthDomainNameField (1) */
    *pLength = (LSA_UINT16)(MRP_REC_DATA_BLK_LEN_IF_DATA_ADJ_VERS0_MIN + LengthDomainName + LengthDomainNamePadding);
  }
  else /* blockVersion == MRP_REC_DATA_BLK_VERS_HIGH1_LOW1 */
  {
    *ppData = pParseData - sizeof(LSA_UINT16);

    return MRP_RSP_ERR_PRM_BLOCK;
  }
  
  *ppData    = pParseData;
  pInstance[MRP_INSTANCE_INDEX] = instance;

  return MRP_RSP_OK;
}


/*===========================================================================
* FUNCTION : mrp_rec_parse_blk_type_if_data_adjust
*----------------------------------------------------------------------------
* PURPOSE  : parse blockType MRP_REC_DATA_BLK_TYPE_IF_DATA_ADJUST of dataset
*            PDInterfaceMrpDataAdjust
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_BLOCK)
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec             = internal data base reference
*            parseLevel          = state of parsing
*            blockType           = type of block
*            blockLen            = blockLength of block (including type and length fields)
*            blockVersion        = version of blockType
* OUTPUTS  : **ppData            = record data array reference
*            *pLength            = length of block
*            *pNumOfMrpInstances = max. number of MRP instances
*----------------------------------------------------------------------------
* COMMENTS : after correct parsing, *pParseData points to the end of block and
*            *pLength contains the block length.
*==========================================================================*/
static LSA_UINT16 mrp_rec_parse_blk_type_if_data_adjust (MRP_REC_DB_TYPE *pDBrec, 
                                                                     LSA_INT32 parseLevel,
                                                                     LSA_UINT8 **ppData, 
                                                                     LSA_UINT16 *pLength,
                                                                     LSA_UINT16 *pNumOfMrpInstances,
                                                                     LSA_UINT16 blockType,
                                                                     LSA_UINT16 blockLen,
                                                                     LSA_UINT16 blockVersion)
{
  LSA_UINT16 instance;
  LSA_UINT16 rsp;
  LSA_UINT8  *pDomainUUID;
  LSA_UINT16 Role;
  LSA_UINT16 Padding, numOfMrpInstances, maxMrpInstance;
  LSA_UINT8  LengthDomainName;
  LSA_UINT8  LengthDomainNamePadding;
  LSA_UINT8  *pDomainName;
  MRP_IF_DATA_ADJUST_HEAD_TYPE *pIfAdjPara;
  MRP_REC_DB_INST_SET_TYPE *pInstPara;
  LSA_UINT8 *pParseData = *ppData;

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBrec->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  pIfAdjPara        = &pDBrec->para.RecIfAdjustHead;
  numOfMrpInstances = 1;

  if (parseLevel == MRP_REC_ENUM_PARSE_VALUE) 
  {
    MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> parsing PDInterfaceMrpDataAdjust (blockType=0x%x, (blockLen + 4)=%d, blockVersion=0x%x)", blockType, blockLen, blockVersion);
  }

  if (blockVersion == MRP_REC_DATA_BLK_VERS_HIGH1_LOW0)
  {   
    instance          = 0; /* old dataset version --> instance is always 0! */
    /* calc length of dataset including bytes after blockHeader and padding bytes after MRP role */
    /* dataset pointer points to BLK_HEADER_PADDING */
    if (parseLevel == MRP_REC_ENUM_PARSE_LEN)
    {
      if (*pLength < (MRP_REC_DATA_LEN_BLK_HEADER + MRP_REC_DATA_LEN_BLK_HEADER_PADDING + MRP_REC_DATA_LEN_UUID_AND_ROLE + MRP_REC_DATA_LEN_ROLE_PADDING)) 
      {
        MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_DATA: blockType=0x%x, blockLength(=%d) mismatch", blockType, *pLength);

        *ppData = pParseData - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
        return MRP_RSP_ERR_PRM_DATA;
      }
    }

    pParseData += (MRP_REC_DATA_LEN_BLK_HEADER_PADDING + MRP_REC_DATA_LEN_UUID_AND_ROLE + MRP_REC_DATA_LEN_ROLE_PADDING);

    rsp = mrp_rec_parse_helper_domain_name (pDBrec, &pParseData, &LengthDomainName, &LengthDomainNamePadding, LSA_NULL);
    
    if (rsp != MRP_RSP_OK) 
    {
      *ppData = pParseData;
      return rsp;
    }
    pParseData -= (MRP_REC_DATA_LEN_BLK_HEADER_PADDING + MRP_REC_DATA_LEN_UUID_AND_ROLE + MRP_REC_DATA_LEN_ROLE_PADDING);

    if (parseLevel >= MRP_REC_ENUM_PARSE_VALUE) 
    {
      MRP_REC_DB_GET16_NTOH(pParseData, Padding); /* padding bytes after BlockHeader */
      if (Padding != 0) 
      {
        MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: blockType=0x%x, Padding (0x%x) after BlockHeader wrong", blockType, Padding);

        *ppData = pParseData - sizeof(LSA_UINT16);
        return MRP_RSP_ERR_PRM_BLOCK;
      }
      pDomainUUID = pParseData;
      pParseData += MRP_REC_DATA_LEN_UUID;

      MRP_REC_DB_GET16_NTOH(pParseData, Role);
      pDBrec->tmpParsedProjRole = Role;
#ifdef MRP_CFG_PLUGIN_0_MRM
      if (Role == MRP_PROJ_ROLE_MGR && !( (MRP_CAPABILITY_ROLE_MGR & pDBrec->SupportedRoleInst0) || (MRP_CAPABILITY_ROLE_AUTO_MGR & pDBrec->SupportedRoleInst0))) 
      {
        MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: blockType=0x%x, MRP_Role (%u) not supported", blockType, Role);

        *ppData = pParseData - sizeof(LSA_UINT16);
        return MRP_RSP_ERR_PRM_BLOCK;
      }
      if (!(Role == MRP_PROJ_ROLE_OFF ||  Role == MRP_PROJ_ROLE_CLIENT || Role == MRP_PROJ_ROLE_MGR)) 
      {
        MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: blockType=0x%x, MRP_Role (%u) not supported", blockType, Role);

        *ppData = pParseData - sizeof(LSA_UINT16);
        return MRP_RSP_ERR_PRM_BLOCK;
      }
#else
      if (!(Role == MRP_PROJ_ROLE_OFF || Role == MRP_PROJ_ROLE_CLIENT)) 
      {
        MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: blockType=0x%x, MRP_Role (%u) not supported", blockType, Role);

        *ppData = pParseData - sizeof(LSA_UINT16);
        return MRP_RSP_ERR_PRM_BLOCK;
      }
#endif
      if (Role == MRP_PROJ_ROLE_CLIENT && !(MRP_CAPABILITY_ROLE_CLIENT & pDBrec->SupportedRoleInst0)) 
      {
        MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: blockType=0x%x, MRP_Role (%u) not supported", blockType, Role);

        *ppData = pParseData - sizeof(LSA_UINT16);
        return MRP_RSP_ERR_PRM_BLOCK;
      }
      MRP_REC_DB_GET16_NTOH(pParseData, Padding);
      if (Padding != 0) 
      {
        MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: Padding(0x%x) after MRP_Role wrong", Padding);

        *ppData = pParseData - sizeof(LSA_UINT16);
        return MRP_RSP_ERR_PRM_BLOCK;
      }
      if ( (rsp = mrp_rec_parse_helper_domain_name (pDBrec, &pParseData, &LengthDomainName, &LengthDomainNamePadding, &pDomainName)) != MRP_RSP_OK) 
      {
        *ppData = pParseData;
        return rsp;
      }
      pParseData += sizeof(LengthDomainName) + LengthDomainName + LengthDomainNamePadding;

      if (parseLevel >= MRP_REC_ENUM_PARSE_WRITE) 
      {
        MRP_UPPER_TRACE_08(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> parsing PDInterfaceMrpDataAdjust DomainUUID=0x%x:0x%x:0x%x:0x%x:0x%x:0x%x:..., Role=%u, LengthDomainName=%u",
          pDomainUUID[0], pDomainUUID[1], pDomainUUID[2], pDomainUUID[3], pDomainUUID[4], pDomainUUID[5], Role, LengthDomainName);

        if (LengthDomainName >= 6)
        {
          MRP_UPPER_TRACE_06(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> parsing PDInterfaceMrpDataAdjust pDomainName=%c%c%c%c%c%c",
            pDomainName[0], pDomainName[1], pDomainName[2], pDomainName[3], pDomainName[4], pDomainName[5]);
        }
        else
        {
          if (LengthDomainName >= 2)
          {
            MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> parsing PDInterfaceMrpDataAdjust pDomainName=%c%c", pDomainName[0], pDomainName[1]);
          }
        }
        pInstPara = &pDBrec->pInstData[instance].para;
 
        /* save instance related dataset contents */
        MRP_MEMCPY(pInstPara->RecInstanceAdjust.DomainUUID, pDomainUUID, sizeof(pInstPara->RecInstanceAdjust.DomainUUID));
        pInstPara->RecInstanceAdjust.Role                    = Role;
        pInstPara->RecInstanceAdjust.LengthDomainName        = LengthDomainName;
        MRP_MEMCPY(pInstPara->RecInstanceAdjust.DomainName, pDomainName, LengthDomainName);
        pInstPara->RecInstanceAdjust.LengthDomainNamePadding = LengthDomainNamePadding;
        pInstPara->RecInstanceAdjust.head.v.v_old.dummy      = 0;

        pInstPara->RecInstanceAdjust.Written                 = LSA_TRUE;
        pInstPara->adminRole                                 = mrp_rec_map_proj_role_to_admin_role(pDBrec->pChSys, LSA_FALSE, (MRP_PROJ_ROLE_ENUM_T)Role);

        /* save dataset header to interface related data struct to be able to distinguish between old and new dataset format */
        pIfAdjPara->head.blockType        = blockType;
        pIfAdjPara->head.blockLength      = blockLen - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN; /* blockLen includes length of type and length field */
        pIfAdjPara->head.blockVersionHigh = MRP_REC_DATA_BLK_VERS_HIGH1;
        pIfAdjPara->head.blockVersionLow  = MRP_REC_DATA_BLK_VERS_LOW0;
        pIfAdjPara->head.v.v_old.padding1 = 0;
        pIfAdjPara->head.v.v_old.padding2 = 0;

        pIfAdjPara->Written               = LSA_TRUE;
      }
    }
    /* *pLength = length calculated without blockType and blockLength field (=4 Bytes). This bytes will be added at the end of this function!!!           */
    /* MRP_REC_DATA_BLK_LEN_IF_DATA_ADJ_VERS0_MIN (25): blockVersion (2) + padding (2) + uuid (16) + mrp_role (2) + padding (2) + lengthDomainNameField (1) */
    *pLength = (LSA_UINT16)(MRP_REC_DATA_BLK_LEN_IF_DATA_ADJ_VERS0_MIN + LengthDomainName + LengthDomainNamePadding);
  }
  else /* blockVersion == MRP_REC_DATA_BLK_VERS_HIGH1_LOW1 */
  {
   /* calc min. length of dataset for one MRP instance including bytes after blockHeader and padding bytes after MRP role */
    /* dataset pointer points to BLK_HEADER_PADDING */
    if (*pLength < ( 2 * (MRP_REC_DATA_LEN_BLK_HEADER + MRP_REC_DATA_LEN_BLK_HEADER_PADDING) + MRP_REC_DATA_LEN_UUID_AND_ROLE + MRP_REC_DATA_LEN_ROLE_PADDING)) 
    {
      MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_DATA: blockType=0x%x, blockLength(=%d) mismatch", blockType, *pLength);

      *ppData = pParseData - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
      return MRP_RSP_ERR_PRM_DATA;
    }
    if (parseLevel >= MRP_REC_ENUM_PARSE_VALUE) 
    {
      MRP_REC_DB_GET16_NTOH(pParseData, numOfMrpInstances);
      
      maxMrpInstance = mrp_sys_get_num_of_instances(pDBrec->pChSys);
      
      if (!numOfMrpInstances || numOfMrpInstances > maxMrpInstance) 
      {
        MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: NumOfMrpInstances (0x%x) invalid, maxMrpInstance=%d", numOfMrpInstances, maxMrpInstance);

        *ppData = pParseData - sizeof(LSA_UINT16);
        return MRP_RSP_ERR_PRM_BLOCK;
      }
        
      if (parseLevel >= MRP_REC_ENUM_PARSE_WRITE) 
      {
        /* save dataset header to interface related data struct to be able to distinguish between old and new dataset format */
        pIfAdjPara->head.blockType        = blockType;
        pIfAdjPara->head.blockLength      = blockLen - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN; /* blockLen includes length of type and length field */
        pIfAdjPara->head.blockVersionHigh = MRP_REC_DATA_BLK_VERS_HIGH1;
        pIfAdjPara->head.blockVersionLow  = MRP_REC_DATA_BLK_VERS_LOW1;
        pIfAdjPara->head.v.v_new.padding  = 0;
        pIfAdjPara->head.v.v_new.numOfMrpInstances = (LSA_UINT8)numOfMrpInstances;

        pIfAdjPara->Written               = LSA_TRUE;
      }
    }
    /* *pLength = length calculated without blockType and blockLength field (=4 Bytes). This bytes will be added in function mrp_rec_parse_interface_data_adjust!!! */
    /* MRP_REC_DATA_BLK_LEN_IF_DATA_ADJ_VERS1_MIN (4): blockVersion (2) + padding (1) + NumberOfMrpInstances (1) */
    *pLength = (LSA_UINT16)(MRP_REC_DATA_BLK_LEN_IF_DATA_ADJ_VERS1_MIN);
  }

  *ppData             = pParseData;
  *pNumOfMrpInstances = numOfMrpInstances;
  
  return MRP_RSP_OK;
}


/*===========================================================================
* FUNCTION : mrp_rec_parse_interface_data_adjust
*----------------------------------------------------------------------------
* PURPOSE  : parse PDInterfaceMrpDataAdjust
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_BLOCK)
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec=internal data base reference
*            **ppData            = record data array reference
*            parseLevel          = state of parsing
*            blockType           = type of block
*            blockLen            = blockLength of block (including type and length fields)
*            blockVersion        = version of blockType
* OUTPUTS  : **ppData            = record data array reference
*            *pLength            = length of block
*            *pNumOfMrpInstances = max. number of MRP instances
*            *pInstance          = MRP instance, for which parameters are parsed
*----------------------------------------------------------------------------
* COMMENTS : after correct parsing, *ppData points to the end of record and
*            *pLength contains the block length.
*==========================================================================*/
static LSA_UINT16 mrp_rec_parse_interface_data_adjust (MRP_REC_DB_TYPE *pDBrec, 
                                                       LSA_INT32 parseLevel,
                                                       LSA_UINT8 **ppData, 
                                                       LSA_UINT16 *pLength,
                                                       LSA_UINT16 *pNumOfMrpInstances,
                                                       LSA_UINT16 *pInstance,
                                                       LSA_UINT16 blockType,
                                                       LSA_UINT16 blockLen,
                                                       LSA_UINT16 blockVersion)
{
  LSA_UINT8 *pParseData = *ppData;
  LSA_UINT16 rc;

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBrec->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  switch (blockType) 
  {
    case MRP_REC_DATA_BLK_TYPE_IF_DATA_ADJUST:
    {
      rc = mrp_rec_parse_blk_type_if_data_adjust (pDBrec, parseLevel, &pParseData, pLength, pNumOfMrpInstances, blockType, blockLen, blockVersion);
      
      if (rc != MRP_RSP_OK)
      {
        *ppData = pParseData;
        return rc;
      }
    }
    break;

    case MRP_REC_DATA_BLK_TYPE_INST_DATA_ADJUST:  /* only present, if PDInterfaceMrpDataAdjust (blockVersionHigh = 1, blockVersionLow = 1) */
    {
      rc = mrp_rec_parse_blk_type_inst_data_adjust (pDBrec, parseLevel, &pParseData, pLength, pNumOfMrpInstances, pInstance, blockType, blockLen, blockVersion);
      
      if (rc != MRP_RSP_OK)
      {          
        *ppData = pParseData;
        return rc;  
      }
    }
    break;
      
    case MRP_REC_DATA_BLK_TYPE_MANAGER_PARAMS:
    {
      LSA_UINT16 Prio, TOPchgT_10ms, TOPNRmax, TSTshortTms, TSTdefaultTms, TSTNRmax, Padding;
      MRP_REC_DB_INST_SET_TYPE *pInstPara;

      if (parseLevel == MRP_REC_ENUM_PARSE_VALUE) 
      {
        MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> parsing MrpManagerParams[inst=%d] (blockType=0x%x)", pInstance[MRP_INSTANCE_INDEX], blockType);
      }

      if (parseLevel >= MRP_REC_ENUM_PARSE_VALUE) 
      {
        if (pDBrec->tmpParsedProjRole == MRP_PROJ_ROLE_OFF || pDBrec->tmpParsedProjRole == MRP_PROJ_ROLE_CLIENT)
        {
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: ManagerParams available although MRP_Role = %d", pInstance[MRP_INSTANCE_INDEX], pDBrec->tmpParsedProjRole);

          *ppData = pParseData - MRP_REC_DATA_LEN_BLK_HEADER; /* pointer was incremented by MRP_REC_DATA_LEN_BLK_HEADER before function was called, redirect it here to get correct err_offset */
          return MRP_RSP_ERR_PRM_BLOCK;
        }
        MRP_REC_DB_GET16_NTOH(pParseData, Prio);
        if (mrp_rec_parse_helper_prio(Prio) != MRP_RSP_OK)
        {
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: MRP_Prio (0x%x)", pInstance[MRP_INSTANCE_INDEX], Prio);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
        MRP_REC_DB_GET16_NTOH(pParseData, TOPchgT_10ms);
        if (TOPchgT_10ms > 100) 
        { 
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: MRP_TOPchgT (%u)", pInstance[MRP_INSTANCE_INDEX], TOPchgT_10ms);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
        MRP_REC_DB_GET16_NTOH(pParseData, TOPNRmax);
        if (TOPNRmax == 0 || TOPNRmax > 6) 
        {
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: MRP_TOPNRmax (%u)", pInstance[MRP_INSTANCE_INDEX], TOPNRmax);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
        MRP_REC_DB_GET16_NTOH(pParseData, TSTshortTms);
        if (TSTshortTms == 0 || TSTshortTms > 500 || (TSTshortTms % MRP_TIMER_TIME_BASE_DEZ)) 
        {
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: MRP_TSTshortT (%u)", pInstance[MRP_INSTANCE_INDEX], TSTshortTms);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
        MRP_REC_DB_GET16_NTOH(pParseData, TSTdefaultTms);
        if (TSTdefaultTms == 0 || TSTdefaultTms > 1000 || (TSTdefaultTms % MRP_TIMER_TIME_BASE_DEZ)) 
        {
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: MRP_TSTdefaultT (%u)", pInstance[MRP_INSTANCE_INDEX], TSTdefaultTms);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
        MRP_REC_DB_GET16_NTOH(pParseData, TSTNRmax);
        if (TSTNRmax < 1 || TSTNRmax > 10) 
        {
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: MRP_TSTNRmax (%u)", pInstance[MRP_INSTANCE_INDEX], TSTNRmax);
          
          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
        MRP_REC_DB_GET16_NTOH(pParseData, Padding);
        if (Padding != 0) 
        {
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: Padding (0x%x)", pInstance[MRP_INSTANCE_INDEX], Padding);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
        if (parseLevel >= MRP_REC_ENUM_PARSE_WRITE) 
        {
          MRP_UPPER_TRACE_07(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> parsing PDInterfaceMrpDataAdjust[inst=%d] Prio=0x%x, TOPchgT=%u *10ms, TOPNRmax=%u, TSTshortT=%u ms, TSTdefaultT=%u ms, TSTNRmax=%u, ", pInstance[MRP_INSTANCE_INDEX], Prio, TOPchgT_10ms, TOPNRmax, TSTshortTms, TSTdefaultTms, TSTNRmax);
          
          pInstPara = &pDBrec->pInstData[pInstance[MRP_INSTANCE_INDEX]].para;
          
          pInstPara->RecInstanceAdjust.mgr.Prio          = Prio;
          pInstPara->RecInstanceAdjust.mgr.TOPchgT_10ms  = TOPchgT_10ms;
          pInstPara->RecInstanceAdjust.mgr.TOPNRmax      = TOPNRmax;
          pInstPara->RecInstanceAdjust.mgr.TSTshortTms   = TSTshortTms;
          pInstPara->RecInstanceAdjust.mgr.TSTdefaultTms = TSTdefaultTms;
          pInstPara->RecInstanceAdjust.mgr.TSTNRmax      = TSTNRmax;

          *ppData -= MRP_REC_DATA_LEN_BLK_HEADER;
          pInstPara->RecInstanceAdjust.mgr.err_offset_base = MRP_REC_DATA_OFFSET_GET(pDBrec->tmpParsedBaseAddr, *ppData);

          pInstPara->RecInstanceAdjust.mgr.Written       = LSA_TRUE;
        }
      }
      *pLength = MRP_REC_DATA_BLK_LEN_MANAGER_PARAMS;
    }
    break;

    case MRP_REC_DATA_BLK_TYPE_CLIENT_PARAMS:
    {
      LSA_UINT16 LNKdownT_ms, LNKupT_ms, LNKNRmax;
      MRP_REC_DB_INST_SET_TYPE *pInstPara;

      if (parseLevel == MRP_REC_ENUM_PARSE_VALUE) 
      {
        MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW,">> parsing MrpClientParams[inst=%d] (0x%x)", pInstance[MRP_INSTANCE_INDEX], blockType);
      }

      if (parseLevel >= MRP_REC_ENUM_PARSE_VALUE) 
      {
        if (pDBrec->tmpParsedProjRole == MRP_PROJ_ROLE_OFF || pDBrec->tmpParsedProjRole == MRP_PROJ_ROLE_MGR)
        {
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: ClientParams available although MRP_Role = %d", pInstance[MRP_INSTANCE_INDEX], pDBrec->tmpParsedProjRole);

          *ppData = pParseData - MRP_REC_DATA_LEN_BLK_HEADER; /* pointer was incremented by MRP_REC_DATA_LEN_BLK_HEADER before function was called, redirect it here to get correct err_offset */
          return MRP_RSP_ERR_PRM_BLOCK;
        }
        MRP_REC_DB_GET16_NTOH(pParseData, LNKdownT_ms);
        
        if (LNKdownT_ms > 1000) 
        {
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: MRP_LNKdownT (%u)", pInstance[MRP_INSTANCE_INDEX], LNKdownT_ms);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
        MRP_REC_DB_GET16_NTOH(pParseData, LNKupT_ms); 
         
        if (LNKupT_ms > 1000) 
        {
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: MRP_LNKupT (%u)", pInstance[MRP_INSTANCE_INDEX], LNKupT_ms);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
        MRP_REC_DB_GET16_NTOH(pParseData, LNKNRmax);
        
        if (LNKNRmax == 0 || LNKNRmax > 5)
        {
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: MRP_LNKNRmax (%u)", pInstance[MRP_INSTANCE_INDEX], LNKNRmax);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
        if (parseLevel >= MRP_REC_ENUM_PARSE_WRITE) 
        {
          MRP_UPPER_TRACE_04(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> parsing PDInterfaceMrpDataAdjust[inst=%d] LNKdownT=0x%x ms, LNKupT=%u ms, LNKNRmax=%u", pInstance[MRP_INSTANCE_INDEX], LNKdownT_ms, LNKupT_ms, LNKNRmax);
          
          pInstPara = &pDBrec->pInstData[pInstance[MRP_INSTANCE_INDEX]].para;
          
          pInstPara->RecInstanceAdjust.client.LNKdownT_ms = LNKdownT_ms;
          pInstPara->RecInstanceAdjust.client.LNKupT_ms   = LNKupT_ms;
          pInstPara->RecInstanceAdjust.client.LNKNRmax    = LNKNRmax;

          *ppData -= MRP_REC_DATA_LEN_BLK_HEADER;
          pInstPara->RecInstanceAdjust.client.err_offset_base = MRP_REC_DATA_OFFSET_GET(pDBrec->tmpParsedBaseAddr, *ppData);

          pInstPara->RecInstanceAdjust.client.Written     = LSA_TRUE;
        }
      }
      *pLength = MRP_REC_DATA_BLK_LEN_CLIENT_PARAMS;
    }
    break;

    default:
    {
      MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: unknown block type 0x%x", pInstance[MRP_INSTANCE_INDEX], blockType);
      *pLength = 0;

      *ppData = pParseData - MRP_REC_DATA_LEN_BLK_HEADER; /* pointer was incremented by MRP_REC_DATA_LEN_BLK_HEADER before function was called, redirect it here to get correct err_offset */
      return MRP_RSP_ERR_PRM_BLOCK;    
    }
    /*    break; */
  }
  
  if (MRP_REC_ENUM_PARSE_LEN == parseLevel) 
  {
    /* set pParseData to next BlockHeader */
    pParseData += (*pLength) - MRP_REC_DATA_LEN_BLK_VERSION /* VersionHighLow */;
  }
  (*pLength) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
  *ppData = pParseData;
  
  return MRP_RSP_OK;
}


#ifdef MRP_CFG_PLUGIN_2_HSR
/*===========================================================================
* FUNCTION : mrp_rec_parse_priv_hsr_adjust
*----------------------------------------------------------------------------
* PURPOSE  : parse PrivInterfaceHsrDataAdjust 
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_BLOCK)
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec    = internal data base reference
*            parseLevel = state of parsing
*            **ppData   = record data array reference
*            blockType  = type of block
* OUTPUTS  : **ppData   = record data array reference
*            *pLength    = length of block
*----------------------------------------------------------------------------
* COMMENTS : after correct parsing, *ppData points to the end of record and
*            *pLength have the block length.
*==========================================================================*/
static LSA_UINT16 mrp_rec_parse_priv_hsr_adjust (MRP_REC_DB_TYPE *pDBrec,
                                                 LSA_INT32 parseLevel,
                                                 LSA_UINT8 **ppData, 
                                                 LSA_UINT16 *pLength,
                                                 LSA_UINT16 blockType)
{
  LSA_UINT16 numOfPorts;
  LSA_UINT8 *pParseData = *ppData;

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBrec->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  numOfPorts = mrp_sys_get_num_of_ports(pDBrec->pChSys);

  switch (blockType) 
  {
  case MRP_REC_DATA_BLK_TYPE_PRV_HSR_ADJUST:
    {
      LSA_UINT16 Padding;
      LSA_UINT16 Role;
      LSA_UINT16 PortA, PortB;

      if (parseLevel == MRP_REC_ENUM_PARSE_VALUE) 
      {
        MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW,">> parsing PrivInterfaceHsrDataAdjust (0x%x)", blockType);
      }

      if (parseLevel >= MRP_REC_ENUM_PARSE_VALUE) 
      {
        MRP_REC_DB_GET16_NTOH(pParseData, Padding);
        if (Padding != 0) 
        {
          MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: Padding (0x%x)", Padding);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
        MRP_REC_DB_GET16_NTOH(pParseData, Role);
        pDBrec->tmpParsedProjRole = Role;
        if (Role > MRP_REC_DATA_HSR_ROLE_INVALIDATE_PROJ) 
        {
          MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: Role (%u)", Role);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
        MRP_REC_DB_GET16_NTOH(pParseData, PortA);
        if (PortA > numOfPorts) 
        {
          MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: PortA (%u)", PortA);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
        MRP_REC_DB_GET16_NTOH(pParseData, PortB);
        if (PortB > numOfPorts) 
        {
          MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: PortB (%u)", PortB);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
        if (MRP_REC_DATA_HSR_ROLE_CL == Role || MRP_REC_DATA_HSR_ROLE_MGR == Role) 
        {
          if (0 == PortA || 0 == PortB || PortA == PortB) 
          {
            MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: PortA=%u PortB=%u mismatch", PortA, PortB);

            *ppData = pParseData - sizeof(LSA_UINT16);
            return MRP_RSP_ERR_PRM_BLOCK;
          }
        }
        if (MRP_REC_ENUM_PARSE_WRITE <= parseLevel) 
        {
          MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> parsing PrivInterfaceHsrDataAdjust Role=0x%x, PortA=%u, PortB=%u", Role, PortA, PortB);
          pDBrec->pInstData[0].para.RecPrivHsrAdjust.Written = LSA_TRUE;
          pDBrec->pInstData[0].para.RecPrivHsrAdjust.Role = Role;
          pDBrec->pInstData[0].para.RecPrivHsrAdjust.PortA = PortA;
          pDBrec->pInstData[0].para.RecPrivHsrAdjust.PortB = PortB;
        }
      }
      *pLength = MRP_REC_DATA_BLK_LEN_PRV_HSR_ADJUST;
    }
    break;

  default:
    MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: unknown block type 0x%x", blockType);
    *pLength = 0;
    return MRP_RSP_ERR_PRM_BLOCK;
/*    break; */
  }
  if (MRP_REC_ENUM_PARSE_LEN == parseLevel) 
  {
    /* set pParseData to next BlockHeader */
    pParseData += (*pLength) - MRP_REC_DATA_LEN_BLK_VERSION /* VersionHighLow */;
  }
  (*pLength) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
  *ppData = pParseData;
  
  return MRP_RSP_OK;
}


/*===========================================================================
* FUNCTION : mrp_rec_parse_priv_stby_adjust
*----------------------------------------------------------------------------
* PURPOSE  : parse PrivInterfaceStdbyDataAdjust 
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_BLOCK)
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec    = internal data base reference
*            parseLevel = state of parsing
*            **ppData   = record data array reference
*            blockType  = type of block
* OUTPUTS  : **ppData   = record data array reference
*            *pLength   = length of block
*----------------------------------------------------------------------------
* COMMENTS : after correct parsing, *ppData points to the end of record and
*            *pLength contains the block length.
*==========================================================================*/
static LSA_UINT16 mrp_rec_parse_priv_stby_adjust (const MRP_REC_DB_TYPE *pDBrec,
                                                  LSA_INT32 parseLevel,
                                                  LSA_UINT8 **ppData, 
                                                  LSA_UINT16 *pLength,
                                                  LSA_UINT16 blockType)
{
  LSA_UINT16 numOfPorts;
  LSA_UINT8 *pParseData = *ppData;
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBrec->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  numOfPorts = mrp_sys_get_num_of_ports(pDBrec->pChSys);

  switch (blockType) 
  {
  case MRP_REC_DATA_BLK_TYPE_PRV_STBY_ADJUST:
    {
      LSA_UINT8  LengthConName;
      LSA_UINT8  LengthConNamePadding;
      LSA_UINT8  *pConName;
      LSA_UINT8  PortCnt;
      LSA_UINT8  PortCntPadding;
      LSA_UINT8  *pPortID;
      LSA_UINT16 Padding;
      LSA_UINT16 rsp;
      LSA_UINT32 Capability;
      LSA_UINT32 WaitForPartner;
      LSA_UINT16 i;

      LengthConNamePadding = LengthConName = 0;
      PortCntPadding = PortCnt = 0;

      if (MRP_REC_ENUM_PARSE_VALUE == parseLevel) 
      {
        MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW,">> parsing PrivInterfaceStdbyDataAdjust (0x%x)", blockType);
      }

      if (MRP_REC_ENUM_PARSE_LEN == parseLevel) 
      {
        /* min len */
        if (*pLength < MRP_REC_DATA_BLK_LEN_PRV_STBY_ADJUST_MIN + 4) 
        {
          MRP_UPPER_TRACE_00(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_DATA: blockLength mismatch");

          *ppData = pParseData - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
          return MRP_RSP_ERR_PRM_DATA;
        }
        /* point to LengthConName */
        pParseData += 2;

        rsp = mrp_rec_parse_helper_stby (pDBrec, &pParseData, MRP_STDBY_NAME_SIZE_MAX, &LengthConName, &LengthConNamePadding, &pConName);

        if (rsp != MRP_RSP_OK) 
        {
          *ppData = pParseData;
          return rsp;
        }
        /* test if record still has right length */
        if (((MRP_REC_DATA_BLK_LEN_PRV_STBY_ADJUST_MIN + 4 - 3/* sub 3 byte max. ConNamePadding */) + LengthConName + LengthConNamePadding) > *pLength) 
        {
          MRP_UPPER_TRACE_00(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_DATA: blockLength mismatch");

          *ppData = pParseData - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
          return MRP_RSP_ERR_PRM_DATA;
        }
        /* test PortCnt */
        pParseData += LengthConName + LengthConNamePadding + 1;

        rsp = mrp_rec_parse_helper_stby (pDBrec, &pParseData, (LSA_UINT8)numOfPorts, &PortCnt, &PortCntPadding, &pPortID);
        
        if (rsp != MRP_RSP_OK)
        {
          *ppData = pParseData;
          return rsp;
        }
        /* test if record still has right length */
        if (((MRP_REC_DATA_BLK_LEN_PRV_STBY_ADJUST_MIN + 4 - 6/* sub 6 byte max. ConNamePadding+PortCntPadding */) + LengthConName + LengthConNamePadding + PortCnt + PortCntPadding) > *pLength) 
        {
          MRP_UPPER_TRACE_00(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_DATA: blockLength mismatch");

          *ppData = pParseData - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
          return MRP_RSP_ERR_PRM_DATA;
        }
      }

      if (MRP_REC_ENUM_PARSE_VALUE <= parseLevel) 
      {
        MRP_REC_DB_GET16_NTOH(pParseData, Padding);

        if (0 != Padding) 
        {
          MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: Padding (0x%x)", Padding);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
        rsp = mrp_rec_parse_helper_stby (pDBrec, &pParseData, MRP_STDBY_NAME_SIZE_MAX, &LengthConName, &LengthConNamePadding, &pConName);

        if (rsp != MRP_RSP_OK) 
        {
          *ppData = pParseData;
          return rsp;
        }

        pParseData += LengthConName + LengthConNamePadding + 1;

        rsp = mrp_rec_parse_helper_stby (pDBrec, &pParseData, (LSA_UINT8)numOfPorts, &PortCnt, &PortCntPadding, &pPortID);

        if (rsp != MRP_RSP_OK)
        {
          *ppData = pParseData;
          return rsp;
        }
        else if (MRP_REC_ENUM_PARSE_VALUE == parseLevel) 
        {
          for (i = 0; i < PortCnt; i++) 
          {
            if (pPortID[i] == 0 || pPortID[i] > numOfPorts) 
            {
              MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: PortID (0x%x)", pPortID[i]);

              *ppData = pParseData + i;
              return MRP_RSP_ERR_PRM_BLOCK;
            }
          }
        }

        if ((0 == PortCnt && 0 != LengthConName) || (0 != PortCnt && 0 == LengthConName)) 
        {
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: PortCnt=%u/ConName=%u", PortCnt, LengthConName);

          *ppData = pParseData;
          return MRP_RSP_ERR_PRM_BLOCK;
        }

        pParseData += PortCnt + PortCntPadding + 1;

        MRP_REC_DB_GET32_NTOH(pParseData, Capability);
        if (3 < Capability) 
        {
          MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: StdbyCapability=%08X", Capability);

          *ppData = pParseData - sizeof(LSA_UINT32);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
        MRP_REC_DB_GET32_NTOH(pParseData, WaitForPartner);
        if (60000 < WaitForPartner)
        {
          MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: StdbypWaitForPartner=%u", WaitForPartner);

          *ppData = pParseData - sizeof(LSA_UINT32);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
        if (MRP_REC_ENUM_PARSE_WRITE <= parseLevel) 
        {
          MRP_UPPER_TRACE_04(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> parsing PrivInterfaceStdbyDataAdjust LengthConName=%u, PortCnt=%u, Capability=0x%x, WaitForPartner=%u", LengthConName, PortCnt, Capability, WaitForPartner);
          
          pDBrec->pInstData[0].para.RecPrivStByAdjust.Written = LSA_TRUE;
          pDBrec->pInstData[0].para.RecPrivStByAdjust.LengthConName = LengthConName;
          pDBrec->pInstData[0].para.RecPrivStByAdjust.LengthConNamePadding = LengthConNamePadding;
          /* preset ConName to provide EOS */
          MRP_MEMSET(pDBrec->pInstData[0].para.RecPrivStByAdjust.ConName, 0, sizeof(pDBrec->pInstData[0].para.RecPrivStByAdjust.ConName));
          if (LengthConName) 
          {
            MRP_MEMCPY(pDBrec->pInstData[0].para.RecPrivStByAdjust.ConName, pConName, LengthConName);
          }
          pDBrec->pInstData[0].para.RecPrivStByAdjust.PortCnt = PortCnt;
          pDBrec->pInstData[0].para.RecPrivStByAdjust.PortCntPadding = PortCntPadding;

          if (PortCnt) 
          {
            for (i = 0; i < PortCnt; i++)
            {
              pDBrec->pInstData[0].para.RecPrivStByAdjust.pPortID[i] = pPortID[i];
            }
          }
          else 
          {
            for (i = 0; i < numOfPorts; i++)
            {
              pDBrec->pInstData[0].para.RecPrivStByAdjust.pPortID[i] = 0;
            }
          }
          pDBrec->pInstData[0].para.RecPrivStByAdjust.Capability = Capability;
          pDBrec->pInstData[0].para.RecPrivStByAdjust.WaitForPartner = WaitForPartner;
        }
      }
      *pLength = (LSA_UINT16)((MRP_REC_DATA_BLK_LEN_PRV_STBY_ADJUST_MIN - 6/* sub 6 byte padding */) + LengthConName + LengthConNamePadding + PortCnt + PortCntPadding);
    }
    break;
  default:
    {
      MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: unknown block type 0x%x", blockType);
      *pLength = 0;

      *ppData = pParseData - MRP_REC_DATA_LEN_BLK_HEADER; /* pointer was incremented by MRP_REC_DATA_LEN_BLK_HEADER before function was called, redirect it here to get correct err_offset */
      return MRP_RSP_ERR_PRM_BLOCK;
      /*    break; */
    }
  }
  if (MRP_REC_ENUM_PARSE_LEN == parseLevel) 
  {
    /* set pParseData to next BlockHeader */
    pParseData += (*pLength) - MRP_REC_DATA_LEN_BLK_VERSION /* VersionHighLow */;
  }
  (*pLength) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
  *ppData = pParseData;
  
  return MRP_RSP_OK;
}


/*===========================================================================
* FUNCTION : mrp_rec_parse_priv_hsr_config
*----------------------------------------------------------------------------
* PURPOSE  : parse PrivInterfaceHsrConfig 
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_BLOCK)
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec    = internal data base reference
*            parseLevel = state of parsing
*            **ppData   = record data array reference
*            blockType  = type of block
* OUTPUTS  : **ppData   = record data array reference
*            *pLength   = length of block
*----------------------------------------------------------------------------
* COMMENTS : after correct parsing, *ppData points to the end of record and
*            *pLength contains the block length.
*==========================================================================*/
static LSA_UINT16 mrp_rec_parse_priv_hsr_config (const MRP_REC_DB_TYPE *pDBrec,
                                                  LSA_INT32 parseLevel,
                                                  LSA_UINT8 **ppData, 
                                                  LSA_UINT16 *pLength,
                                                  LSA_UINT16 blockType)
{
  LSA_UINT16 numOfPorts, rc;
  LSA_UINT8 *pParseData = *ppData;
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBrec->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  numOfPorts = mrp_sys_get_num_of_ports(pDBrec->pChSys);

  switch (blockType) 
  {
  case MRP_REC_DATA_BLK_TYPE_PRV_HSR_CONFIG:
    {
      LSA_UINT16 Padding;
      LSA_UINT16 HSRConfig;
      LSA_UINT16 StByConfig;

      if (MRP_REC_ENUM_PARSE_VALUE == parseLevel) 
      {
        MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> parsing PrivInterfaceHsrConfig (0x%x)", blockType);
      }

      if (MRP_REC_ENUM_PARSE_VALUE <= parseLevel) 
      {
        MRP_REC_DB_GET16_NTOH(pParseData, Padding);
        if (0 != Padding) 
        {
          MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: Padding (0x%x)", Padding);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
        MRP_REC_DB_GET16_NTOH(pParseData, HSRConfig);
        if (1 < HSRConfig) 
        {
          MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: HSRConfig=%u", HSRConfig);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
        MRP_REC_DB_GET16_NTOH(pParseData, StByConfig);
        if (1 < StByConfig) 
        {
          MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: HSRConfig=%u", StByConfig);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
        if (MRP_REC_ENUM_PARSE_WRITE <= parseLevel) 
        {
          LSA_UINT16 changeOver;
          LSA_UINT32 maxDelay;
          MRP_STBY_PARAM_T StdbyState;

          MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)&StdbyState.pPorts, (LSA_UINT16)(sizeof(LSA_UINT8) * numOfPorts));
          MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(StdbyState.pPorts, LSA_NULL));  
          /* preset zero */
          MRP_MEMSET(StdbyState.pPorts, 0, (LSA_UINT16)(sizeof(LSA_UINT8) * numOfPorts));

          mrp_mgr_get_statistic (pDBrec->pChSys, &changeOver, &maxDelay, HSRConfig);
          StdbyState.Config = StByConfig;
          mrp_stby_get_params (pDBrec->pChSys, &StdbyState);

          MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)StdbyState.pPorts);
          MRP_FATAL1(rc == LSA_RET_OK);
          StdbyState.pPorts = LSA_NULL;
        }
      }
      *pLength = MRP_REC_DATA_BLK_LEN_PRV_HSR_CONFIG;
    }
    break;
  default:
    {
      MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: unknown block type 0x%x", blockType);
      *pLength = 0;

      *ppData = pParseData - MRP_REC_DATA_LEN_BLK_HEADER; /* pointer was incremented by MRP_REC_DATA_LEN_BLK_HEADER before function was called, redirect it here to get correct err_offset */
      return MRP_RSP_ERR_PRM_BLOCK;
      /*    break; */
    }
  }
  if (MRP_REC_ENUM_PARSE_LEN == parseLevel) 
  {
    /* set pParseData to next BlockHeader */
    pParseData += (*pLength) - MRP_REC_DATA_LEN_BLK_VERSION /* VersionHighLow */;
  }
  (*pLength) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
  *ppData = pParseData;
  
  return MRP_RSP_OK;
}
#endif


#ifdef MRP_CFG_PLUGIN_1_MRM_EXT
/*===========================================================================
* FUNCTION : mrp_rec_parse_priv_data_adjust
*----------------------------------------------------------------------------
* PURPOSE  : parse PrivMrpDataAdjust
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_BLOCK)
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec    = internal data base reference
*            parseLevel = state of parsing
*            **ppData   = record data array reference
*            blockType  = type of block
* OUTPUTS  : **ppData   = record data array reference
*            *pLength   = length of block
*----------------------------------------------------------------------------
* COMMENTS : after correct parsing, *ppData points to the end of record and
*            *pLength contains the block length.
*==========================================================================*/
static LSA_UINT16 mrp_rec_parse_priv_data_adjust (const MRP_REC_DB_TYPE *pDBrec, 
                                                  LSA_INT32 parseLevel,
                                                  LSA_UINT8 **ppData, 
                                                  LSA_UINT16 *pLength,
                                                  LSA_UINT16 blockType)
{
  LSA_UINT8 *pParseData = *ppData;
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBrec->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  switch (blockType) 
  {
  case MRP_REC_DATA_BLK_TYPE_PRV_DATA_ADJUST:
    {
      LSA_UINT16 Padding;
      LSA_UINT16 MONdefaultT_ms;
      LSA_UINT16 MONNRmax;

      if (MRP_REC_ENUM_PARSE_VALUE == parseLevel) 
      {
        MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW,">> parsing PrivMrpDataAdjust (0x%x)", blockType);
      }

      if (MRP_REC_ENUM_PARSE_VALUE <= parseLevel) 
      {
        MRP_REC_DB_GET16_NTOH(pParseData, Padding);
        if (0 != Padding) 
        {
          MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: Padding (0x%x)", Padding);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
        MRP_REC_DB_GET16_NTOH(pParseData, MONdefaultT_ms);
        if (0 == MONdefaultT_ms || 1000 < MONdefaultT_ms) 
        {
          MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: MRP_MONdefaultT (%u)", MONdefaultT_ms);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
        MRP_REC_DB_GET16_NTOH(pParseData, MONNRmax);
        if (0 == MONNRmax || 1 == MONNRmax || 10 < MONNRmax) 
        {
          MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: MRP_MONNRmax (%u)", MONNRmax);

          *ppData = pParseData - sizeof(LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
        if (MRP_REC_ENUM_PARSE_WRITE <= parseLevel) 
        {
          pDBrec->pInstData[0].para.RecPrivDataAdjust.MONdefaultT_ms = MONdefaultT_ms;
          pDBrec->pInstData[0].para.RecPrivDataAdjust.MONNRmax = MONNRmax;
          pDBrec->pInstData[0].para.RecPrivDataAdjust.Written = LSA_TRUE;
        }
      }
      *pLength = MRP_REC_DATA_BLK_LEN_PRV_DATA_ADJUST;
    }
    break;

  default:
    {
      MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: unknown block type 0x%x", blockType);
      *pLength = 0;

      *ppData = pParseData - MRP_REC_DATA_LEN_BLK_HEADER; /* pointer was incremented by MRP_REC_DATA_LEN_BLK_HEADER before function was called, redirect it here to get correct err_offset */
      return MRP_RSP_ERR_PRM_BLOCK;
      /*    break; */
    }
  }
  if (MRP_REC_ENUM_PARSE_LEN == parseLevel) 
  {
    /* set pParseData to next BlockHeader */
    pParseData += (*pLength) - MRP_REC_DATA_LEN_BLK_VERSION /* VersionHighLow */;
  }
  (*pLength) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
  *ppData = pParseData;
  
  return MRP_RSP_OK;
}
#endif


/*===========================================================================
* FUNCTION : mrp_rec_parse_port_data_adjust
*----------------------------------------------------------------------------
* PURPOSE  : parse PDPortMrpDataAdjust
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_BLOCK)
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec      = internal data base reference
*            parseLevel   = state of parsing
*            **ppData     = record data array reference
*            blockType    = type of block
*            blockLen     = blockLength of block (including type and length fields)
*            blockVersion = version of blockType
*            portID       = edd port id (1..MAX_PORTS)
* OUTPUTS  : **ppData     = record data array reference
*            *pLength     = length of block
*----------------------------------------------------------------------------
* COMMENTS : after correct parsing, *ppData points to the end of record and
*            *pLength contains the block length.
*==========================================================================*/
static LSA_UINT16 mrp_rec_parse_port_data_adjust (const MRP_REC_DB_TYPE *pDBrec, 
                                                              LSA_INT32 parseLevel,
                                                              LSA_UINT8 **ppData, 
                                                              LSA_UINT16 *pLength,
                                                              LSA_UINT16 blockType,
                                                              LSA_UINT16 blockLen,
                                                              LSA_UINT16 blockVersion,
                                                              LSA_UINT16 portID)
{
  LSA_UINT16 i, idx;
  LSA_UINT16 instance, mrpMaxInstance;
  LSA_UINT8 *pParseData = *ppData;
  MRP_REC_DB_INST_SET_TYPE *pInstPara;

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBrec->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  i = 0;
  
  switch (blockType) 
  {
    case MRP_REC_DATA_BLK_TYPE_PORT_DATA_ADJUST:
    {
      if (MRP_REC_ENUM_PARSE_VALUE <= parseLevel) 
      {
        if (MRP_REC_ENUM_PARSE_VALUE == parseLevel) 
        {
          MRP_UPPER_TRACE_04(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> parsing PDPortMrpDataAdjust(portID=%d), blockType=0x%x, (blockLen + 4)=%d, blockVersion=0x%x)", portID, blockType, blockLen, blockVersion);
        }

        MRP_REC_DB_GET16_NTOH(pParseData, instance);

        if (blockVersion == MRP_REC_DATA_BLK_VERS_HIGH1_LOW0)
        {
          if (instance != 0) /* 2 padding bytes have to be 0! */
          {
            MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: blockType=0x%x, blockVersion=0x%x, wrong instance value in DS (0x%x)", blockType, blockVersion, instance);

            *ppData = pParseData - sizeof(LSA_UINT16);
            return MRP_RSP_ERR_PRM_BLOCK;
          }      
        }
        else /* blockVersion == MRP_REC_DATA_BLK_VERS_HIGH1_LOW1 */
        {
          mrpMaxInstance = mrp_sys_get_num_of_instances(pDBrec->pChSys);

          if (instance >= mrpMaxInstance) /* probably a comparison to pIfAdjPara->head.v.v_new.numOfMrpInstances might be better! */
          {
            MRP_UPPER_TRACE_04(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: blockType=0x%x, blockVersion=0x%x, instance value in DS (0x%x) invalid, mrpMaxInstance=%d", blockType, blockVersion, instance, mrpMaxInstance);

            *ppData = pParseData - sizeof(LSA_UINT16);
            return MRP_RSP_ERR_PRM_BLOCK;        
          }
        }
        
        for (idx = 0; idx < pDBrec->NumOfPossibleRports; idx++) 
        {
          if (pDBrec->pRPortArray[idx] == portID)  /* if the port is a possible rport */
            break;
        }
        if (idx < pDBrec->NumOfPossibleRports)
        {
          pInstPara = &pDBrec->pInstData[instance].para;
          
          if (parseLevel >= MRP_REC_ENUM_PARSE_WRITE) 
          {
            for (i = 0; i < MRP_DEFAULT_VAL_INST_RPORT_CNT; i++)
            {
              if (pInstPara->RecPortAdjust[i].PortID == portID || pInstPara->RecPortAdjust[i].PortID == 0)
              {
                MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, ">> storing PDPortMrpDataAdjust[inst=%d], (portID=%d)", instance, portID);
                
                pInstPara->RecPortAdjust[i].PortID    = portID;
                MRP_MEMCPY(pInstPara->RecPortAdjust[i].DomainUUID, pParseData, sizeof(pInstPara->RecPortAdjust->DomainUUID));

                /* always use new dataset header, because difference is only the instance field, which is always 0 in old dataset version */
                pInstPara->RecPortAdjust[i].head.v.v_new.blockType        = blockType;
                pInstPara->RecPortAdjust[i].head.v.v_new.blockLength      = blockLen - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN; /* blockLen includes length of type and length field */
                pInstPara->RecPortAdjust[i].head.v.v_new.blockVersionHigh = MRP_REC_DATA_BLK_VERS_HIGH1;
                pInstPara->RecPortAdjust[i].head.v.v_new.padding          = 0;
                pInstPara->RecPortAdjust[i].head.v.v_new.instance         = (LSA_UINT8)instance;

                if (blockVersion == MRP_REC_DATA_BLK_VERS_HIGH1_LOW0)
                {
                  pInstPara->RecPortAdjust[i].head.v.v_new.blockVersionLow  = MRP_REC_DATA_BLK_VERS_LOW0;
                }
                else
                {
                  pInstPara->RecPortAdjust[i].head.v.v_new.blockVersionLow  = MRP_REC_DATA_BLK_VERS_LOW1;
                }

                pInstPara->RecPortAdjust[i].Written = LSA_TRUE;
                
                pParseData += sizeof(pInstPara->RecPortAdjust->DomainUUID);
                break;
              }
            }
          }
        }
        if (idx >= pDBrec->NumOfPossibleRports || i >= MRP_DEFAULT_VAL_INST_RPORT_CNT) 
        {
          /* unknown port */
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID[inst=%d]: port id out of range (port=%u) or too many portAdjustDS written", instance, portID);

          return MRP_RSP_ERR_PRM_PORTID;
        }
      }
      *pLength = MRP_REC_DATA_BLK_LEN_PORT_DATA_ADJUST;    
    }
    break;
    
    default:
      {
        MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: unknown block type 0x%x", blockType);
        *pLength = 0;

        *ppData = pParseData - MRP_REC_DATA_LEN_BLK_HEADER; /* pointer was incremented by MRP_REC_DATA_LEN_BLK_HEADER before function was called, redirect it here to get correct err_offset */
        return MRP_RSP_ERR_PRM_BLOCK;
        /*    break; */
      }
  }
  
  if (parseLevel == MRP_REC_ENUM_PARSE_LEN) 
  {
    /* set pParseData to next BlockHeader */
    pParseData += (*pLength) - MRP_REC_DATA_LEN_BLK_VERSION /* VersionHighLow */;
  }
  (*pLength) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
  *ppData = pParseData;
  
  return MRP_RSP_OK;
}


/*===========================================================================
* FUNCTION : mrp_rec_parse_blk_type_inst_data_check
*----------------------------------------------------------------------------
* PURPOSE  : parse blockType MRP_REC_DATA_BLK_TYPE_INST_DATA_CHECK of dataset
*            PDInterfaceMrpDataCheck
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_BLOCK)
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec             = internal data base reference
*            parseLevel          = state of parsing
*            *pNumOfMrpInstances = max. number of MRP instances
*            blockType           = type of block
*            blockLen            = blockLength of block (including type and length fields)
*            blockVersion        = version of blockType
* OUTPUTS  : **ppData            = record data array reference
*            *pLength            = length of block
*            *pInstance          = MRP instance for which the dataset is parsed
*----------------------------------------------------------------------------
* COMMENTS : after correct parsing, *pParseData points to the end of block and
*            *pLength contains the block length.
*==========================================================================*/
static LSA_UINT16 mrp_rec_parse_blk_type_inst_data_check (const MRP_REC_DB_TYPE *pDBrec, 
                                                                      LSA_INT32 parseLevel,
                                                                      LSA_UINT8 **ppData, 
                                                                      LSA_UINT16 *pLength,
                                                                      const LSA_UINT16 *pNumOfMrpInstances,
                                                                      LSA_UINT16 *pInstance,
                                                                      LSA_UINT16 blockType,
                                                                      LSA_UINT16 blockLen,
                                                                      LSA_UINT16 blockVersion)
{
  LSA_UINT32 Check;
  LSA_UINT16 instance, mrpMaxInstance, i;
  LSA_UINT8  *pDomainUUID;
  MRP_REC_DB_INST_SET_TYPE *pInstPara;
  LSA_UINT8 *pParseData = *ppData;

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBrec->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  mrpMaxInstance = mrp_sys_get_num_of_instances(pDBrec->pChSys);
  instance       = 0;

  if (parseLevel == MRP_REC_ENUM_PARSE_VALUE) 
  {
    MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> parsing MrpInstanceDataCheck (blockType=0x%x, (blockLen + 4)=%d, blockVersion=0x%x)", blockType, blockLen, blockVersion);
  }

  if (blockVersion == MRP_REC_DATA_BLK_VERS_HIGH1_LOW0)
  {
    if (parseLevel >= MRP_REC_ENUM_PARSE_VALUE) 
    {
      MRP_REC_DB_GET16_NTOH(pParseData, instance); /* padding byte and MRP_Instance after BlockHeader */
      
      if (instance >= mrpMaxInstance || *pNumOfMrpInstances > mrpMaxInstance) /* *pNumOfMrpInstances is set with numOfMrpInstances from blockType PDInterfaceMrpDataCheck (0x213) */
      {
        MRP_UPPER_TRACE_04(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: blockType=0x%x, instance value(0x%x) or numOfMrpInstances=%d wrong, maxInst=%d", blockType, instance, *pNumOfMrpInstances, mrpMaxInstance);

        *ppData = pParseData - sizeof(LSA_UINT16);
        return MRP_RSP_ERR_PRM_BLOCK;
      }

      if (parseLevel >= MRP_REC_ENUM_PARSE_WRITE)
      {
        if (*pNumOfMrpInstances > 1)
        {
          for (i = 0; i < mrpMaxInstance; i++)
          {
            pInstPara = &pDBrec->pInstData[i].para;
            if (pInstPara->RecInstanceCheck.Written)
            {
              if (pInstPara->RecInstanceCheck.head.v.v_new.instance == (LSA_UINT8)instance)
              {
                MRP_UPPER_TRACE_04(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: blockType=0x%x, instance(=0x%x) value already used, numOfMrpInstances=%d, maxInst=%d", blockType, instance, *pNumOfMrpInstances, mrpMaxInstance);

                *ppData = pParseData - sizeof(LSA_UINT16);
                return MRP_RSP_ERR_PRM_BLOCK;
              }
            }
          }
        }
      }

      pDomainUUID = pParseData;

      if (parseLevel >= MRP_REC_ENUM_PARSE_WRITE)
      {
        if (*pNumOfMrpInstances > 1)
        {
          for (i = 0; i < mrpMaxInstance; i++)
          {
            pInstPara = &pDBrec->pInstData[i].para;
            if (pInstPara->RecInstanceCheck.Written)
            {
              if (!MRP_MEMCMP(pInstPara->RecInstanceCheck.DomainUUID, pDomainUUID, sizeof(pInstPara->RecInstanceCheck.DomainUUID)))
              {
                MRP_UPPER_TRACE_04(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: blockType=0x%x, domainUUID not different between two inst (%d and %d), numOfMrpInstances=%d", blockType, i, instance, *pNumOfMrpInstances);

                *ppData = pParseData;
                return MRP_RSP_ERR_PRM_BLOCK;
              }
            }
          }
        }
      }

      pParseData += MRP_REC_DATA_LEN_UUID;

      MRP_REC_DB_GET32_NTOH(pParseData, Check);

      if (MRP_REC_DATA_CHECK_MASK & Check) 
      {
        MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: blockType=0x%x, MRP_Check (0x%x) invalid", instance, blockType, Check);

        *ppData = pParseData - sizeof(LSA_UINT32);
        return MRP_RSP_ERR_PRM_BLOCK;
      }

      if (parseLevel >= MRP_REC_ENUM_PARSE_WRITE) 
      {
        MRP_UPPER_TRACE_07(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> parsing MrpInstanceDataCheck[inst=%d] DomainUUID=0x%x:0x%x:0x%x:0x%x:0x%x:0x%x:...",
          instance, pDomainUUID[0], pDomainUUID[1], pDomainUUID[2], pDomainUUID[3], pDomainUUID[4], pDomainUUID[5]);
        MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> parsing MrpInstanceDataCheck[inst=%d] Check=%u, completed successfully", instance, Check);

        pInstPara = &pDBrec->pInstData[instance].para;

        MRP_MEMCPY(pInstPara->RecInstanceCheck.DomainUUID, pDomainUUID, sizeof(pInstPara->RecInstanceCheck.DomainUUID));
        pInstPara->RecInstanceCheck.Check                          = Check;
        
        pInstPara->RecInstanceCheck.head.v.v_new.blockType         = blockType;
        pInstPara->RecInstanceCheck.head.v.v_new.blockLength       = blockLen - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN; /* blockLen includes length of type and length field */
        pInstPara->RecInstanceCheck.head.v.v_new.blockVersionHigh  = MRP_REC_DATA_BLK_VERS_HIGH1;
        pInstPara->RecInstanceCheck.head.v.v_new.blockVersionLow   = MRP_REC_DATA_BLK_VERS_LOW0;
        pInstPara->RecInstanceCheck.head.v.v_new.padding           = 0;
        pInstPara->RecInstanceCheck.head.v.v_new.instance          = (LSA_UINT8)instance;

        *ppData -= MRP_REC_DATA_LEN_BLK_HEADER;
        pInstPara->RecInstanceCheck.err_offset_base                = MRP_REC_DATA_OFFSET_GET(pDBrec->tmpParsedBaseAddr, *ppData);
        pInstPara->RecInstanceCheck.Written                        = LSA_TRUE;
      }
    }
    *pLength = MRP_REC_DATA_BLK_LEN_IF_DATA_CHECK;
  }
  else /* blockVersion == MRP_REC_DATA_BLK_VERS_HIGH1_LOW1 */
  {
    *ppData = pParseData - sizeof(LSA_UINT16);
    return MRP_RSP_ERR_PRM_BLOCK;
  }

  *ppData    = pParseData;
  pInstance[MRP_INSTANCE_INDEX] = instance;

  return MRP_RSP_OK;
}


/*===========================================================================
* FUNCTION : mrp_rec_parse_blk_type_if_data_check
*----------------------------------------------------------------------------
* PURPOSE  : parse blockType MRP_REC_DATA_BLK_TYPE_IF_DATA_CHECK of dataset
*            PDInterfaceMrpDataCheck
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_BLOCK)
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec             = internal data base reference
*            parseLevel          = state of parsing
*            blockType           = type of block
*            blockLen            = blockLength of block (including type and length fields)
*            blockVersion        = version of blockType
* OUTPUTS  : **ppData            = record data array reference
*            *pLength            = length of block
*            *pNumOfMrpInstances = max. number of MRP instances
*----------------------------------------------------------------------------
* COMMENTS : after correct parsing, *pParseData points to the end of block and
*            *pLength contains the block length.
*==========================================================================*/
static LSA_UINT16 mrp_rec_parse_blk_type_if_data_check (MRP_REC_DB_TYPE *pDBrec, 
                                                              LSA_INT32 parseLevel,
                                                              LSA_UINT8 **ppData, 
                                                              LSA_UINT16 *pLength,
                                                              LSA_UINT16 *pNumOfMrpInstances,
                                                              LSA_UINT16 blockType,
                                                              LSA_UINT16 blockLen,
                                                              LSA_UINT16 blockVersion)
{
  LSA_UINT16 instance;
  LSA_UINT16 Padding, numOfMrpInstances, maxMrpInstance;
  LSA_UINT8  DomainUUID[16];
  LSA_UINT32 Check;
  MRP_IF_DATA_CHECK_HEAD_TYPE *pIfCheckPara;
  MRP_REC_DB_INST_SET_TYPE *pInstPara;
  LSA_UINT8 *pParseData = *ppData;

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBrec->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  pIfCheckPara      = &pDBrec->para.RecIfCheckHead;
  numOfMrpInstances = 1;

  if (parseLevel == MRP_REC_ENUM_PARSE_VALUE) 
  {
    MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> parsing PDInterfaceMrpDataCheck (blockType=0x%x, (blockLen + 4)=%d, blockVersion=0x%x)", blockType, blockLen, blockVersion);
  }

  if (blockVersion == MRP_REC_DATA_BLK_VERS_HIGH1_LOW0)
  {
    instance          = 0; /* old dataset version --> instance is always 0! */

    if (parseLevel >= MRP_REC_ENUM_PARSE_VALUE) 
    {
      MRP_REC_DB_GET16_NTOH(pParseData, Padding);
      
      if (Padding != 0)
      {
        MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: blockType=0x%x, Padding (0x%x) after BlockHeader wrong", blockType, Padding);

        *ppData = pParseData - sizeof(LSA_UINT16);
        return MRP_RSP_ERR_PRM_BLOCK;
      }
      MRP_MEMCPY(DomainUUID, pParseData, MRP_REC_DATA_LEN_UUID);
      pParseData += MRP_REC_DATA_LEN_UUID;
      
      MRP_REC_DB_GET32_NTOH(pParseData, Check);

      if (MRP_REC_DATA_CHECK_MASK & Check) 
      {
        MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: blockType=0x%x, MRP_Check (0x%x) invalid", blockType, Check);
        
        *ppData = pParseData - sizeof(LSA_UINT32);
        return MRP_RSP_ERR_PRM_BLOCK;
      }
      
      if (parseLevel >= MRP_REC_ENUM_PARSE_WRITE) 
      {
        MRP_UPPER_TRACE_07(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> parsing PDInterfaceMrpDataCheck DomainUUID=0x%x:0x%x:0x%x:0x%x:0x%x:0x%x:..., Check=%u",
          DomainUUID[0], DomainUUID[1], DomainUUID[2], DomainUUID[3], DomainUUID[4], DomainUUID[5], Check);
        
        pInstPara = &pDBrec->pInstData[instance].para;
        
        MRP_MEMCPY(pInstPara->RecInstanceCheck.DomainUUID, DomainUUID, sizeof(pInstPara->RecInstanceCheck.DomainUUID));
        pInstPara->RecInstanceCheck.Check              = Check;
        pInstPara->RecInstanceCheck.head.v.v_old.dummy = 0;
        
        pInstPara->RecInstanceCheck.Written = LSA_TRUE;
        
        /* save dataset header to interface related data struct to be able to distinguish between old and new dataset format */
        pIfCheckPara->head.blockType        = blockType;
        pIfCheckPara->head.blockLength      = blockLen - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN; /* blockLen includes length of type and length field */
        pIfCheckPara->head.blockVersionHigh = MRP_REC_DATA_BLK_VERS_HIGH1;
        pIfCheckPara->head.blockVersionLow  = MRP_REC_DATA_BLK_VERS_LOW0;
        pIfCheckPara->head.v.v_old.padding1 = 0;
        pIfCheckPara->head.v.v_old.padding2 = 0;

        pIfCheckPara->Written               = LSA_TRUE;       
      }
    }
    *pLength = MRP_REC_DATA_BLK_LEN_IF_DATA_CHECK;
  }
  else /* blockVersion == MRP_REC_DATA_BLK_VERS_HIGH1_LOW1 */
  {
    /* new dataset version, but no MrpInstanceDataAdjust block parsed yet! */
    /* calc min. length of dataset for one MRP instance including bytes after blockHeader and padding bytes after MRP role */
    /* dataset pointer points to BLK_HEADER_PADDING */
    if (*pLength < ( 2 * (MRP_REC_DATA_LEN_BLK_HEADER + MRP_REC_DATA_LEN_BLK_HEADER_PADDING) + MRP_REC_DATA_LEN_UUID + MRP_REC_DATA_LEN_CHECK)) 
    {
      MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_DATA: blockType=0x%x, blockLen(=%d) mismatch", blockType, *pLength);

      *ppData = pParseData - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
      return MRP_RSP_ERR_PRM_DATA;
    }
    if (parseLevel >= MRP_REC_ENUM_PARSE_VALUE) 
    {
      MRP_REC_DB_GET16_NTOH(pParseData, numOfMrpInstances);

      maxMrpInstance = mrp_sys_get_num_of_instances(pDBrec->pChSys);

      if (!numOfMrpInstances || numOfMrpInstances > maxMrpInstance) 
      {
        MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: NumOfMrpInstances (0x%x) invalid, maxMrpInstance=%d", numOfMrpInstances, maxMrpInstance);

        *ppData = pParseData - sizeof(LSA_UINT16);
        return MRP_RSP_ERR_PRM_BLOCK;
      }
        
      if (parseLevel >= MRP_REC_ENUM_PARSE_WRITE) 
      {
        /* save dataset header to interface related data struct to be able to distinguish between old and new dataset format */
        pIfCheckPara->head.blockType        = blockType;
        pIfCheckPara->head.blockLength      = blockLen - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN; /* blockLen includes length of type and length field */
        pIfCheckPara->head.blockVersionHigh = MRP_REC_DATA_BLK_VERS_HIGH1;
        pIfCheckPara->head.blockVersionLow  = MRP_REC_DATA_BLK_VERS_LOW1;
        pIfCheckPara->head.v.v_new.padding  = 0;
        pIfCheckPara->head.v.v_new.numOfMrpInstances = (LSA_UINT8)numOfMrpInstances;

        pIfCheckPara->Written               = LSA_TRUE;
      }
    }
    *pLength = MRP_REC_DATA_BLK_LEN_IF_DATA_CHECK_VERS1;    
  }
    
  *ppData             = pParseData;
  *pNumOfMrpInstances = numOfMrpInstances;

  return MRP_RSP_OK;
}


/*===========================================================================
* FUNCTION : mrp_rec_parse_interface_data_check
*----------------------------------------------------------------------------
* PURPOSE  : parse PDInterfaceMrpDataCheck
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_BLOCK)
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec     = internal data base reference
*            parseLevel  = state of parsing
*            **ppData    = record data array reference
*            blockType   = type of block
*            blockLen    = block length
*            blockVersion= block version
* OUTPUTS  : **ppData    = record data array reference
*            *pLength    = length of block
*            *pNumOfMrpInstances = max. number of MRP instances
*            *pInstance          = MRP instance for which the dataset is parsed
*----------------------------------------------------------------------------
* COMMENTS : after correct parsing, *ppData points to the end of record and
*            *pLength contains the block length.
*==========================================================================*/
static LSA_UINT16 mrp_rec_parse_interface_data_check (MRP_REC_DB_TYPE *pDBrec, 
                                                            LSA_INT32 parseLevel,
                                                            LSA_UINT8 **ppData, 
                                                            LSA_UINT16 *pLength,
                                                            LSA_UINT16 *pNumOfMrpInstances,
                                                            LSA_UINT16 *pInstance,
                                                            LSA_UINT16 blockType,
                                                            LSA_UINT16 blockLen,
                                                            LSA_UINT16 blockVersion)
{
  LSA_UINT16 rc;
  LSA_UINT8 *pParseData = *ppData;

  switch (blockType) 
  {
    case MRP_REC_DATA_BLK_TYPE_IF_DATA_CHECK:
    {
      rc = mrp_rec_parse_blk_type_if_data_check (pDBrec, parseLevel, &pParseData, pLength, pNumOfMrpInstances, blockType, blockLen, blockVersion);

      if (rc != MRP_RSP_OK)
      {
        *ppData = pParseData;
        return rc;
      }
    }
    break;
    
    case MRP_REC_DATA_BLK_TYPE_INST_DATA_CHECK:
    {
      rc = mrp_rec_parse_blk_type_inst_data_check (pDBrec, parseLevel, &pParseData, pLength, pNumOfMrpInstances, pInstance, blockType, blockLen, blockVersion);

      if (rc != MRP_RSP_OK)
      {
        *ppData = pParseData;
        return rc;    
      }
    }
    break;

    default:
      {
        MRP_UPPER_TRACE_01(pDBrec->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: unknown block type 0x%x", blockType);
        *pLength = 0;

        *ppData = pParseData - MRP_REC_DATA_LEN_BLK_HEADER; /* pointer was incremented by MRP_REC_DATA_LEN_BLK_HEADER before function was called, redirect it here to get correct err_offset */
        return MRP_RSP_ERR_PRM_BLOCK;
        /*    break; */
      }
  }
  
  if (MRP_REC_ENUM_PARSE_LEN == parseLevel) 
  {
    /* set pParseData to next BlockHeader */
    pParseData += (*pLength) - MRP_REC_DATA_LEN_BLK_VERSION /* VersionHighLow */;
  }
  (*pLength) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
  *ppData = pParseData;
  
  return MRP_RSP_OK;
}


/*===========================================================================
* FUNCTION : mrp_rec_parse_ic_port_data_adjust
*----------------------------------------------------------------------------
* PURPOSE  : parse PDPortMrpIcDataAdjust
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_BLOCK)
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec      = internal data base reference
*            parseLevel   = state of parsing
*            **ppData     = record data array reference
*            blockType    = type of block
*            blockLen     = blockLength of block (including type and length fields)
*            blockVersion = version of blockType
*            portID       = edd port id (1..MAX_PORTS)
* OUTPUTS  : **ppData     = record data array reference
*            *pLength     = length of block
*----------------------------------------------------------------------------
* COMMENTS : after correct parsing, *ppData points to the end of record and
*            *pLength contains the block length.
*==========================================================================*/
static LSA_UINT16 mrp_rec_parse_ic_port_data_adjust (const MRP_REC_DB_TYPE *pDBrec,
                                                            LSA_INT32 parseLevel,
                                                            LSA_UINT8 **ppData,
                                                            LSA_UINT16 *pLength,
                                                            LSA_UINT16 blockType,
                                                            LSA_UINT16 blockLen,
                                                            LSA_UINT16 blockVersion,
                                                            LSA_UINT16 portID)
 {
  LSA_UINT16 idx;
  LSA_UINT8 *pParseData = *ppData;
  LSA_UINT16 rsp, padding;
  LSA_UINT16 instance, mrpInMaxInstance, inInstance;
  LSA_UINT16 DomainID;
  LSA_UINT16 Role;
  LSA_UINT8  LengthDomainName;
  LSA_UINT8  LengthDomainNamePadding;
  LSA_UINT8  *pDomainName;
  LSA_UINT16 TOPchgT = 0, TOPNRmax = 0, LinkStatusChangeT = 0, LinkStatusNRmax = 0; /* MIM params */
  LSA_UINT16 LNKdownT = 0, LNKupT = 0, LNKNRmax = 0, MICPosition = 0; /* MIC params */
  LSA_UINT16 StartDelay = 0;
  MRP_REC_DB_INST_SET_TYPE *pInstPara;
  MRP_IF_DB_TYPE *pDBif;

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBrec->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pDBrec->pChSys);
  instance = MRP_DEFAULT_INSTANCE;

  switch (blockType)
  {
    case MRP_REC_DATA_BLK_TYPE_IC_PORT_DATA_ADJUST:
    {
      if (parseLevel == MRP_REC_ENUM_PARSE_VALUE)
      {
        MRP_UPPER_TRACE_04 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> parsing PDPortMrpIcDataAdjust(portID=%d), blockType=0x%x, (blockLen + 4)=%d, blockVersion=0x%x)",
                            portID, blockType, blockLen, blockVersion);
      }
      else if (parseLevel == MRP_REC_ENUM_PARSE_LEN)
      {
        if (*pLength < (MRP_REC_DATA_LEN_BLK_HEADER + MRP_REC_DATA_LEN_BLK_HEADER_PADDING + MRP_REC_DATA_LEN_IC_DOMAINID + MRP_REC_DATA_LEN_ROLE))
        {
          MRP_UPPER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_DATA: blockType=0x%x, blockLength(=%d) mismatch", blockType, *pLength);

          *ppData = pParseData - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
          return MRP_RSP_ERR_PRM_DATA;
        }
      }

      if (blockVersion == MRP_REC_DATA_BLK_VERS_HIGH1_LOW0)
      {
        pParseData += (MRP_REC_DATA_LEN_BLK_HEADER_PADDING + MRP_REC_DATA_LEN_IC_DOMAINID);

        MRP_REC_DB_GET16_NTOH (pParseData, Role);

        if (!(Role == MRP_IC_PROJ_ROLE_OFF || Role == MRP_IC_PROJ_ROLE_CLIENT || Role == MRP_IC_PROJ_ROLE_MGR))
        {
          MRP_UPPER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: blockType=0x%x, MRP_IC_Role (%u) not supported", blockType, Role);

          *ppData = pParseData - sizeof (LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
        if (Role == MRP_IC_PROJ_ROLE_MGR && !(pDBrec->SupportedInRoleInst0 & MRP_IC_CAPABILITY_ROLE_MGR))
        {
          MRP_UPPER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: blockType=0x%x, MRP_IC_Role (%u) not supported", blockType, Role);

          *ppData = pParseData - sizeof (LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
        if (Role == MRP_IC_PROJ_ROLE_CLIENT && !(pDBrec->SupportedInRoleInst0 & MRP_IC_CAPABILITY_ROLE_CLIENT))
        {
          MRP_UPPER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: blockType=0x%x, MRP_IC_Role (%u) not supported", blockType, Role);

          *ppData = pParseData - sizeof (LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }

        rsp = mrp_rec_parse_helper_domain_name (pDBrec, &pParseData, &LengthDomainName, &LengthDomainNamePadding, LSA_NULL);

        if (rsp != MRP_RSP_OK)
        {
          *ppData = pParseData;
          return rsp;
        }

        pParseData -= (MRP_REC_DATA_LEN_BLK_HEADER_PADDING + MRP_REC_DATA_LEN_IC_DOMAINID + MRP_REC_DATA_LEN_ROLE);

        if (parseLevel >= MRP_REC_ENUM_PARSE_VALUE)
        {
          MRP_REC_DB_GET16_NTOH (pParseData, padding);

          if (padding != 0) /* 2 padding bytes have to be 0! */
          {
            MRP_UPPER_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: blockType=0x%x, blockVersion=0x%x, wrong padding value in DS (0x%x)",
                                blockType, blockVersion, padding);

            *ppData = pParseData - sizeof (LSA_UINT16);
            return MRP_RSP_ERR_PRM_BLOCK;
          }

          for (idx = 0; idx < pDBrec->NumOfPossibleInPorts; idx++)
          {
            if (pDBrec->pInPortArray[idx] == portID)  /* if the port is a possible inport */
              break;
          }

          if (idx < pDBrec->NumOfPossibleInPorts) /* Matching INport found */
          {
            MRP_REC_DB_GET16_NTOH (pParseData, DomainID);
            MRP_REC_DB_GET16_NTOH (pParseData, Role); /* checked already */

            if ((rsp = mrp_rec_parse_helper_domain_name (pDBrec, &pParseData, &LengthDomainName, &LengthDomainNamePadding, &pDomainName)) != MRP_RSP_OK)
            {
              *ppData = pParseData;
              return rsp;
            }
            pParseData += sizeof (LengthDomainName) + LengthDomainName + LengthDomainNamePadding;

            if (Role == MRP_IC_PROJ_ROLE_CLIENT)
            {
              MRP_REC_DB_GET16_NTOH (pParseData, LNKdownT);
              if (LNKdownT == 0 || LNKdownT > 1000)
              {
                MRP_UPPER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: MRPIC_LNKdownT (%u)", instance, LNKdownT);

                *ppData = pParseData - sizeof (LSA_UINT16);
                return MRP_RSP_ERR_PRM_BLOCK;
              }

              MRP_REC_DB_GET16_NTOH (pParseData, LNKupT);
              if (LNKupT == 0 || LNKupT > 1000)
              {
                MRP_UPPER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: MRPIC_LNKupT (%u)", instance, LNKupT);

                *ppData = pParseData - sizeof (LSA_UINT16);
                return MRP_RSP_ERR_PRM_BLOCK;
              }

              MRP_REC_DB_GET16_NTOH (pParseData, LNKNRmax);
              if (LNKNRmax == 0 || LNKNRmax > 100)
              {
                MRP_UPPER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: MRPIC_LNKNRmax (%u)", instance, LNKNRmax);

                *ppData = pParseData - sizeof (LSA_UINT16);
                return MRP_RSP_ERR_PRM_BLOCK;
              }

              MRP_REC_DB_GET16_NTOH (pParseData, StartDelay);
              if (StartDelay == 0 || (StartDelay > 0xEA60 && StartDelay < 0xFFFF))
              {
                MRP_UPPER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: MRPIC_StartDelay (%u)", instance, StartDelay);

                *ppData = pParseData - sizeof (LSA_UINT16);
                return MRP_RSP_ERR_PRM_BLOCK;
              }

              MRP_REC_DB_GET16_NTOH (pParseData, MICPosition);
              if (MICPosition > 1)
              {
                MRP_UPPER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: MICPosition (%u)", instance, MICPosition);

                *ppData = pParseData - sizeof (LSA_UINT16);
                return MRP_RSP_ERR_PRM_BLOCK;
              }

              MRP_REC_DB_GET16_NTOH (pParseData, padding);
              LSA_UNUSED_ARG(padding);
            }
            else if (Role == MRP_IC_PROJ_ROLE_MGR)
            {
              MRP_REC_DB_GET16_NTOH (pParseData, TOPchgT);
              if (TOPchgT > 100)
              {
                MRP_UPPER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: MRPIC_TOPchgT (%u)", instance, TOPchgT);

                *ppData = pParseData - sizeof (LSA_UINT16);
                return MRP_RSP_ERR_PRM_BLOCK;
              }

              MRP_REC_DB_GET16_NTOH (pParseData, TOPNRmax);
              if (TOPNRmax == 0 || TOPNRmax > 10)
              {
                MRP_UPPER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: MRPIC_TOPNRmax (%u)", instance, TOPNRmax);

                *ppData = pParseData - sizeof (LSA_UINT16);
                return MRP_RSP_ERR_PRM_BLOCK;
              }

              MRP_REC_DB_GET16_NTOH (pParseData, LinkStatusChangeT);
              if (LinkStatusChangeT == 0 || LinkStatusChangeT > 1000)
              {
                MRP_UPPER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: MRPIC_LinkStatusChangeT (%u)", instance, LinkStatusChangeT);

                *ppData = pParseData - sizeof (LSA_UINT16);
                return MRP_RSP_ERR_PRM_BLOCK;
              }

              MRP_REC_DB_GET16_NTOH (pParseData, LinkStatusNRmax);
              if (LinkStatusNRmax < 8 || LinkStatusNRmax > 100)
              {
                MRP_UPPER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: MRPIC_LinkStatusNRmax (%u)", instance, LinkStatusNRmax);

                *ppData = pParseData - sizeof (LSA_UINT16);
                return MRP_RSP_ERR_PRM_BLOCK;
              }

              MRP_REC_DB_GET16_NTOH (pParseData, StartDelay);
              if (StartDelay == 0 || (StartDelay > 0xEA60 && StartDelay < 0xFFFF))
              {
                MRP_UPPER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: MRPIC_StartDelay (%u)", instance, StartDelay);

                *ppData = pParseData - sizeof (LSA_UINT16);
                return MRP_RSP_ERR_PRM_BLOCK;
              }

              MRP_REC_DB_GET16_NTOH (pParseData, padding);
              LSA_UNUSED_ARG(padding);
            }

            if (parseLevel >= MRP_REC_ENUM_PARSE_WRITE)
            {
              MRP_UPPER_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, ">> parsing PDPortMrpIcDataAdjust DomainID=0x%x, Role=%u, LengthDomainName=%u", DomainID, Role, LengthDomainName);

              if (LengthDomainName >= 6)
              {
                MRP_UPPER_TRACE_06 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> parsing PDPortMrpIcDataAdjust pDomainName=%c%c%c%c%c%c",
                                    pDomainName[0], pDomainName[1], pDomainName[2], pDomainName[3], pDomainName[4], pDomainName[5]);
              }
              else
              {
                if (LengthDomainName >= 2)
                {
                  MRP_UPPER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> parsing PDPortMrpIcDataAdjust pDomainName=%c%c", pDomainName[0], pDomainName[1]);
                }
              }

              if (Role == MRP_IC_PROJ_ROLE_MGR)
              {
                MRP_UPPER_TRACE_05 (traceIdx, LSA_TRACE_LEVEL_NOTE, ">> parsing PDPortMrpIcDataAdjust TOPchgT=%u *10ms, TOPNRmax=%u, LinkStatusChangeT=%u ms, LinkStatusNRmax=%u, StartDelay=%u *100ms",
                                    TOPchgT, TOPNRmax, LinkStatusChangeT, LinkStatusNRmax, StartDelay);
              }
              else if (Role == MRP_IC_PROJ_ROLE_CLIENT)
              {
                MRP_UPPER_TRACE_05 (traceIdx, LSA_TRACE_LEVEL_NOTE, ">> parsing PDPortMrpIcDataAdjust LNKdownT=%u ms, LNKupT=%u ms, LNKNRmax=%u, StartDelay=%u *100ms, MICPosition=%u",
                                    LNKdownT, LNKupT, LNKNRmax, StartDelay, MICPosition);
              }

              /* get the instance parameters */
              pInstPara = &pDBrec->pInstData[instance].para;

              MRP_UPPER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, ">> storing PDPortMrpIcDataAdjust[inst=%d], (portID=%d)", instance, portID);

              pDBif = (MRP_IF_DB_TYPE *)pDBrec->pChSys->pChSysDB->pInterfacePara;
              /* Search for a free/unused inInstance, this does an inplicit check for mrpInMaxInstance */
              /* At this point the order of the MRP_IN instances of the PRM data is not important. Later on during "PRM_commit" when the data is copied from the "para"-data-set to "work"-data-set the order is changed if necessary. */
              for (inInstance = 0; inInstance < (mrpInMaxInstance - pDBif->mrpRstpInstances); inInstance++)
              {
                /* check if the instance is free/unused */
                if (pInstPara->pRecIcPortAdjust[inInstance].Written != LSA_TRUE)
                {
                  /* free instance found, break loop and continue with the flow */
                  break;
                }
              }
              /* no free instance found => ERROR */
              if (inInstance >= mrpInMaxInstance)
              {
                MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> MRP_RSP_ERR_PRM_DATA[inInst=%d]: To many inInstances (including RSTP+ instances)! inInstances must be < mrpInMaxInstance=%d", inInstance, mrpInMaxInstance);

                return MRP_RSP_ERR_PRM_DATA;
              }
              
              pInstPara->pRecIcPortAdjust[inInstance].PortID = portID;
              pInstPara->pRecIcPortAdjust[inInstance].DomainID = DomainID;
              pInstPara->pRecIcPortAdjust[inInstance].Role = Role;
              pInstPara->pRecIcPortAdjust[inInstance].LengthDomainName = LengthDomainName;
              MRP_MEMCPY(pInstPara->pRecIcPortAdjust[inInstance].DomainName, pDomainName, LengthDomainName);
              pInstPara->pRecIcPortAdjust[inInstance].LengthDomainNamePadding = LengthDomainNamePadding;

              if (Role == MRP_IC_PROJ_ROLE_MGR)
              {
                pInstPara->pRecIcPortAdjust[inInstance].mim.TOPchgT_10ms = TOPchgT;
                pInstPara->pRecIcPortAdjust[inInstance].mim.TOPNRmax = TOPNRmax;
                pInstPara->pRecIcPortAdjust[inInstance].mim.LinkStatusChangeT_ms = LinkStatusChangeT;
                pInstPara->pRecIcPortAdjust[inInstance].mim.LinkStatusNRmax = LinkStatusNRmax;
                pInstPara->pRecIcPortAdjust[inInstance].mim.StartDelay_100ms = StartDelay;
              
                pInstPara->pRecIcPortAdjust[inInstance].mim.Written = LSA_TRUE;
              }
              else if (Role == MRP_IC_PROJ_ROLE_CLIENT)
              {
                pInstPara->pRecIcPortAdjust[inInstance].mic.LNKdownT_ms = LNKdownT;
                pInstPara->pRecIcPortAdjust[inInstance].mic.LNKupT_ms = LNKupT;
                pInstPara->pRecIcPortAdjust[inInstance].mic.LNKNRmax = LNKNRmax;
                pInstPara->pRecIcPortAdjust[inInstance].mic.StartDelay_100ms = StartDelay;
                pInstPara->pRecIcPortAdjust[inInstance].mic.MICPosition = MICPosition;

                pInstPara->pRecIcPortAdjust[inInstance].mic.Written = LSA_TRUE;
              }

              /* always use new dataset header, because difference is only the instance field, which is always 0 in old dataset version */
              pInstPara->pRecIcPortAdjust[inInstance].head.v.v_new.blockType = blockType;
              pInstPara->pRecIcPortAdjust[inInstance].head.v.v_new.blockLength = blockLen - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN; /* blockLen includes length of type and length field */
              pInstPara->pRecIcPortAdjust[inInstance].head.v.v_new.blockVersionHigh = MRP_REC_DATA_BLK_VERS_HIGH1;
              pInstPara->pRecIcPortAdjust[inInstance].head.v.v_new.blockVersionLow = MRP_REC_DATA_BLK_VERS_LOW0;
              pInstPara->pRecIcPortAdjust[inInstance].head.v.v_new.padding = 0;
              pInstPara->pRecIcPortAdjust[inInstance].head.v.v_new.instance = (LSA_UINT8)instance;

              pInstPara->pRecIcPortAdjust[inInstance].Written = LSA_TRUE;
            }
          }
          if (idx >= pDBrec->NumOfPossibleInPorts)
          {
            /* unknown port */
            MRP_UPPER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID[inst=%d]: port id out of range or not a possible IN port! (port=%u)", instance, portID);

            return MRP_RSP_ERR_PRM_PORTID;
          }
        }
        *pLength = MRP_REC_DATA_BLK_LEN_IC_PORT_DATA_ADJUST_MIN + LengthDomainName + LengthDomainNamePadding;

        if (Role == MRP_IC_PROJ_ROLE_CLIENT)
        {
          *pLength += MRP_REC_DATA_BLK_LEN_IC_MIC_PARAMS;
        }
        else if (Role == MRP_IC_PROJ_ROLE_MGR)
        {
          *pLength += MRP_REC_DATA_BLK_LEN_IC_MIM_PARAMS;
        }
      }
      else
      {
        *ppData = pParseData - sizeof (LSA_UINT16);
        return MRP_RSP_ERR_PRM_BLOCK;
      }
    }
    break;

    default:
    {
      MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: unknown block type 0x%x", blockType);
      *pLength = 0;

      *ppData = pParseData - MRP_REC_DATA_LEN_BLK_HEADER; /* pointer was incremented by MRP_REC_DATA_LEN_BLK_HEADER before function was called, redirect it here to get correct err_offset */
      return MRP_RSP_ERR_PRM_BLOCK;
      /*    break; */
    }
  }

  if (MRP_REC_ENUM_PARSE_LEN == parseLevel)
  {
    /* set pParseData to next block */
    pParseData += (*pLength) - MRP_REC_DATA_LEN_BLK_VERSION;
  }
  (*pLength) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
  *ppData = pParseData;

  return MRP_RSP_OK;
}


/*===========================================================================
* FUNCTION : mrp_rec_parse_ic_port_data_check
*----------------------------------------------------------------------------
* PURPOSE  : parse PDPortMrpIcDataCheck
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_BLOCK)
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec      = internal data base reference
*            parseLevel   = state of parsing
*            **ppData     = record data array reference
*            blockType    = type of block
*            blockLen     = blockLength of block (including type and length fields)
*            blockVersion = version of blockType
*            portID       = edd port id (1..MAX_PORTS)
* OUTPUTS  : **ppData     = record data array reference
*            *pLength     = length of block
*----------------------------------------------------------------------------
* COMMENTS : after correct parsing, *ppData points to the end of record and
*            *pLength contains the block length.
*==========================================================================*/
static LSA_UINT16 mrp_rec_parse_ic_port_data_check (const MRP_REC_DB_TYPE *pDBrec,
                                                              LSA_INT32 parseLevel,
                                                              LSA_UINT8 **ppData,
                                                              LSA_UINT16 *pLength,
                                                              LSA_UINT16 blockType,
                                                              LSA_UINT16 blockLen,
                                                              LSA_UINT16 blockVersion,
                                                              LSA_UINT16 portID)
{
  LSA_UINT32 Check;
  LSA_UINT8 *pParseData = *ppData;
  LSA_UINT16 instance, idx, inInstance, mrpInMaxInstance;
  LSA_UINT16 padding;
  MRP_REC_DB_INST_SET_TYPE *pInstPara;
  MRP_IF_DB_TYPE *pDBif;

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBrec->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pDBrec->pChSys);
  instance = MRP_DEFAULT_INSTANCE;

  switch (blockType)
  {
    case MRP_REC_DATA_BLK_TYPE_IC_PORT_DATA_CHECK:
    {
      if (MRP_REC_ENUM_PARSE_VALUE <= parseLevel)
      {
        if (MRP_REC_ENUM_PARSE_VALUE == parseLevel)
        {
          MRP_UPPER_TRACE_04(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> parsing PDPortMrpIcDataCheck(portID=%d), blockType=0x%x, (blockLen + 4)=%d, blockVersion=0x%x)",
                             portID, blockType, blockLen, blockVersion);
        }

        if (blockVersion == MRP_REC_DATA_BLK_VERS_HIGH1_LOW0)
        {
          MRP_REC_DB_GET16_NTOH (pParseData, padding);

          if (padding != 0) /* 2 padding bytes have to be 0! */
          {
            MRP_UPPER_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: blockType=0x%x, blockVersion=0x%x, wrong padding value in DS (0x%x)",
                                blockType, blockVersion, padding);

            *ppData = pParseData - sizeof (LSA_UINT16);
            return MRP_RSP_ERR_PRM_BLOCK;
          }

          for (idx = 0; idx < pDBrec->NumOfPossibleInPorts; idx++)
          {
            if (pDBrec->pInPortArray[idx] == portID)  /* if the port is a possible inport */
              break;
          }

          if (idx < pDBrec->NumOfPossibleInPorts)
          {
            MRP_REC_DB_GET32_NTOH (pParseData, Check);

            if (MRP_REC_IC_PORT_DATA_CHECK_MASK & Check)
            {
              MRP_UPPER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: MRPIC_Check (0x%x) invalid", instance, Check);

              *ppData = pParseData - sizeof (LSA_UINT32);
              return MRP_RSP_ERR_PRM_BLOCK;
            }

            if (parseLevel >= MRP_REC_ENUM_PARSE_WRITE)
            {
              pInstPara = &pDBrec->pInstData[instance].para;

              pDBif = (MRP_IF_DB_TYPE *)pDBrec->pChSys->pChSysDB->pInterfacePara;
              /* Search for a free/unused inInstance, this does an inplicit check for mrpInMaxInstance */
              /* At this point the order of the MRP_IN instances of the PRM data is not important. Also the relation ship to pRecIcPortAdjust is not important, it will be checked during the data consistency check. */
              for (inInstance = 0; inInstance < (mrpInMaxInstance - pDBif->mrpRstpInstances); inInstance++)
              {
                /* check if the instance is free/unused */
                if (pInstPara->pRecIcPortCheck[inInstance].Written != LSA_TRUE)
                {
                  /* free instance found, break loop and continue with the flow */
                  break;
                }
              }
              /* no free instance found => ERROR */
              if (inInstance >= mrpInMaxInstance)
              {
                MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK[inst=%d]: No free inInstance found!", instance);

                *ppData = pParseData - sizeof(LSA_UINT16);
                return MRP_RSP_ERR_PRM_BLOCK;
              }

              MRP_UPPER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, ">> storing PDPortMrpIcDataCheck[inInst=%d], (portID=%d)", inInstance, portID);

              pInstPara->pRecIcPortCheck[inInstance].PortID = portID;
              pInstPara->pRecIcPortCheck[inInstance].Check = Check;

              /* always use new dataset header, because difference is only the instance field, which is always 0 in old dataset version */
              pInstPara->pRecIcPortCheck[inInstance].head.v.v_new.blockType = blockType;
              pInstPara->pRecIcPortCheck[inInstance].head.v.v_new.blockLength = blockLen - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN; /* blockLen includes length of type and length field */
              pInstPara->pRecIcPortCheck[inInstance].head.v.v_new.blockVersionHigh = MRP_REC_DATA_BLK_VERS_HIGH1;
              pInstPara->pRecIcPortCheck[inInstance].head.v.v_new.blockVersionLow = MRP_REC_DATA_BLK_VERS_LOW0;
              pInstPara->pRecIcPortCheck[inInstance].head.v.v_new.padding = 0;
              pInstPara->pRecIcPortCheck[inInstance].head.v.v_new.instance = (LSA_UINT8)instance;

              pInstPara->pRecIcPortCheck[inInstance].Written = LSA_TRUE;
            }
          }
          if (idx >= pDBrec->NumOfPossibleInPorts)
          {
            /* unknown port */
            MRP_UPPER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID[inst=%d]: port id out of range (port=%u)", instance, portID);

            return MRP_RSP_ERR_PRM_PORTID;
          }
        }
        else
        {
          *ppData = pParseData - sizeof (LSA_UINT16);
          return MRP_RSP_ERR_PRM_BLOCK;
        }
      }
      *pLength = MRP_REC_DATA_BLK_LEN_IC_PORT_DATA_CHECK;
    }
    break;

    default:
    {
      MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_BLOCK: unknown block type 0x%x", blockType);
      *pLength = 0;

      *ppData = pParseData - MRP_REC_DATA_LEN_BLK_HEADER; /* pointer was incremented by MRP_REC_DATA_LEN_BLK_HEADER before function was called, redirect it here to get correct err_offset */
      return MRP_RSP_ERR_PRM_BLOCK;
      /*    break; */
    }
  }

  if (MRP_REC_ENUM_PARSE_LEN == parseLevel)
  {
    /* set pParseData to next BlockHeader */
    pParseData += (*pLength) - MRP_REC_DATA_LEN_BLK_VERSION /* VersionHighLow */;
  }
  (*pLength) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
  *ppData = pParseData;

  return MRP_RSP_OK;

}

#ifdef MRP_CFG_PLUGIN_2_HSR
/*===========================================================================
* FUNCTION : mrp_rec_read_priv_hsr_adjust
*----------------------------------------------------------------------------
* PURPOSE  : read PrivInterfaceHsrDataAdjust
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_DATA)
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec = internal data base reference
*            *pData       = record data array
*            readAdmin    = LSA_TRUE: read para DB, LSA_FALSE: read work DB
*            recDataLen   = length of pData array
* OUTPUTS  : *lengthTotal = length of block
*----------------------------------------------------------------------------
* COMMENTS :  
*==========================================================================*/
static LSA_UINT16 mrp_rec_read_priv_hsr_adjust (const MRP_REC_DB_TYPE *pDBrec,
                                                            LSA_UINT8 *pData, 
                                                            LSA_UINT8 readAdmin,  
                                                            LSA_UINT32 recDataLen,
                                                            LSA_UINT16 *lengthTotal)
{
   LSA_UINT16 length;
  MRP_REC_DB_INST_SET_TYPE *pRecInstDB;

#if (MRP_CFG_TRACE_MODE == 0)
  LSA_UNUSED_ARG(pDBrec);
#endif
#if (MRP_CFG_TRACE_MODE == 1)
  LSA_UNUSED_ARG(pDBrec);
#endif
  length = *lengthTotal = 0;

  if (readAdmin)
  {
    pRecInstDB = &pDBrec->pInstData[0].para;  
  }
  else
  {
    pRecInstDB = &pDBrec->pInstData[0].work;  
  }

  if (pRecInstDB->RecPrivHsrAdjust.Written) 
  {
    /* length check */
    (*lengthTotal) += MRP_REC_DATA_BLK_LEN_PRV_HSR_ADJUST;
    (*lengthTotal) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
    
    if (*lengthTotal > recDataLen) 
    {
      MRP_UPPER_TRACE_02(pDBrec->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_DATA: blockType=0x%x, out of record_data_length (need=%u)", MRP_REC_DATA_BLK_TYPE_PRV_HSR_ADJUST, *lengthTotal);
      *lengthTotal = 0;
      
      return MRP_RSP_ERR_PRM_DATA;
    }
    
    MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_PRV_HSR_ADJUST);
    
    length = (LSA_UINT16)(*lengthTotal - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN);
    
    MRP_REC_DB_PUT16_HTON(pData, length);
    MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW0);
    MRP_REC_DB_PUT16_HTON(pData, 0); /* padding */      
    MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecPrivHsrAdjust.Role); 
    MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecPrivHsrAdjust.PortA); 
    MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecPrivHsrAdjust.PortB); 
  }

  LSA_UNUSED_ARG(pData); /* pData is incremented in the previous MACRO, but not used afterwards => creates lint informational (e438). */

  return MRP_RSP_OK;
}


/*===========================================================================
* FUNCTION : mrp_rec_read_priv_stby_adjust
*----------------------------------------------------------------------------
* PURPOSE  : read PrivInterfaceStdbyDataAdjust
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_DATA)
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec = internal data base reference
*            *pData       = record data array
*            readAdmin    = LSA_TRUE: read para DB, LSA_FALSE: read work DB
*            recDataLen   = length of pData array
* OUTPUTS  : *lengthTotal = length of block
*----------------------------------------------------------------------------
* COMMENTS :  
*==========================================================================*/
static LSA_UINT16 mrp_rec_read_priv_stby_adjust (const MRP_REC_DB_TYPE *pDBrec,
                                                              LSA_UINT8 *pData, 
                                                              LSA_UINT8 readAdmin,  
                                                              LSA_UINT32 recDataLen,
                                                              LSA_UINT16 *lengthTotal)
{
  LSA_UINT16 length;
  MRP_REC_DB_INST_SET_TYPE *pRecInstDB;

#if (MRP_CFG_TRACE_MODE == 0)
  LSA_UNUSED_ARG(pDBrec);
#endif
#if (MRP_CFG_TRACE_MODE == 1)
  LSA_UNUSED_ARG(pDBrec);
#endif



  if (readAdmin)
  {
    pRecInstDB = &pDBrec->pInstData[0].para;  
  }
  else
  {
    pRecInstDB = &pDBrec->pInstData[0].work;  
  }
  
  length = *lengthTotal = 0;
  
  if (pRecInstDB->RecPrivStByAdjust.Written) 
  {
    /* length check */
    (*lengthTotal) += (MRP_REC_DATA_BLK_LEN_PRV_STBY_ADJUST_MIN - 8) /* sub 8 byte padding in min length */
      +pRecInstDB->RecPrivStByAdjust.LengthConName
      +pRecInstDB->RecPrivStByAdjust.LengthConNamePadding
      +pRecInstDB->RecPrivStByAdjust.PortCnt
      +pRecInstDB->RecPrivStByAdjust.PortCntPadding;
      
    (*lengthTotal) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;

    if (*lengthTotal > recDataLen) 
    {
      MRP_UPPER_TRACE_02(pDBrec->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_DATA: blockType=0x%x, out of record_data_length (need=%u)", MRP_REC_DATA_BLK_TYPE_PRV_STBY_ADJUST, *lengthTotal);
      *lengthTotal = 0;
      
      return MRP_RSP_ERR_PRM_DATA;
    }
    MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_PRV_STBY_ADJUST);
    
    length = (LSA_UINT16)(*lengthTotal - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN);
    
    MRP_REC_DB_PUT16_HTON(pData, length);
    MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW0);
    MRP_REC_DB_PUT16_HTON(pData, 0); /* padding */     
     
    *pData = pRecInstDB->RecPrivStByAdjust.LengthConName; 
    pData++;
    MRP_MEMCPY(pData,pRecInstDB->RecPrivStByAdjust.ConName, pRecInstDB->RecPrivStByAdjust.LengthConName);
    pData += pRecInstDB->RecPrivStByAdjust.LengthConName;
    MRP_MEMSET(pData, 0, pRecInstDB->RecPrivStByAdjust.LengthConNamePadding);
    pData += pRecInstDB->RecPrivStByAdjust.LengthConNamePadding;
    *pData = pRecInstDB->RecPrivStByAdjust.PortCnt;
    pData++;
    MRP_MEMCPY(pData, &pRecInstDB->RecPrivStByAdjust.pPortID[0], pRecInstDB->RecPrivStByAdjust.PortCnt);
    pData += pRecInstDB->RecPrivStByAdjust.PortCnt;
    MRP_MEMSET(pData, 0, pRecInstDB->RecPrivStByAdjust.PortCntPadding);
    pData += pRecInstDB->RecPrivStByAdjust.PortCntPadding;
    
    MRP_REC_DB_PUT32_HTON(pData, pRecInstDB->RecPrivStByAdjust.Capability);

    MRP_REC_DB_PUT32_HTON(pData, pRecInstDB->RecPrivStByAdjust.WaitForPartner);
  }

  LSA_UNUSED_ARG(pData); /* pData is incremented in the previous MACRO, but not used afterwards => creates lint informational (e438). */

  return MRP_RSP_OK;
}


/*===========================================================================
* FUNCTION : mrp_rec_read_priv_hsr_real
*----------------------------------------------------------------------------
* PURPOSE  : read PrivInterfaceHsrDataReal
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_DATA)
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec=internal data base reference
*            *pData=record data array
*            recDataLen=length of pData array
* OUTPUTS  : *lengthTotal=length of block
*----------------------------------------------------------------------------
* COMMENTS :  
*==========================================================================*/
static LSA_UINT16 mrp_rec_read_priv_hsr_real (const MRP_REC_DB_TYPE *pDBrec,
                                                          LSA_UINT8 *pData, 
                                                          LSA_UINT32 recDataLen,
                                                          LSA_UINT16 *lengthTotal)
{
  LSA_UINT16 portA;
  LSA_UINT16 portB; 
  MRP_MODE_T mode;
  LSA_UINT16 state;
  LSA_UINT16 diag;
  LSA_UINT16 changeOver;
  LSA_UINT32 maxDelay;
  LSA_UINT16 length;

  *lengthTotal = 0;
  /* length check */
  (*lengthTotal) += MRP_REC_DATA_BLK_LEN_PRV_HSR_REAL;
  (*lengthTotal) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;

  if (*lengthTotal > recDataLen) 
  {
    MRP_UPPER_TRACE_01(pDBrec->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> read_priv_hsr_real: ERR_PRM_DATA: out of record_data_length (need=%u)", *lengthTotal);
    *lengthTotal = 0;
    return MRP_RSP_ERR_PRM_DATA;
  }

  MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_PRV_HSR_REAL);
  
  length = (LSA_UINT16)(*lengthTotal - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN);
  
  MRP_REC_DB_PUT16_HTON(pData, length);
  MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW0);
  MRP_REC_DB_PUT16_HTON(pData, 0); /* padding */      
  mrp_bs_get_ring_ports_and_mode (pDBrec->pChSys, &portA, &portB, &mode);

  MRP_REC_DB_PUT16_HTON(pData, mode);
  MRP_REC_DB_PUT16_HTON(pData, portA);
  MRP_REC_DB_PUT16_HTON(pData, portB);
  (void)(mrp_mgr_get_state (pDBrec->pChSys, &state, &diag, LSA_FALSE));

  MRP_REC_DB_PUT16_HTON(pData, state);
  MRP_REC_DB_PUT16_HTON(pData, diag);
  mrp_mgr_get_statistic (pDBrec->pChSys, &changeOver, &maxDelay, 0);

  MRP_REC_DB_PUT16_HTON(pData, changeOver);
  MRP_REC_DB_PUT32_HTON(pData, maxDelay);

  LSA_UNUSED_ARG(pData); /* pData is incremented in the previous MACRO, but not used afterwards => creates lint informational (e438). */

  return MRP_RSP_OK;
}


/*===========================================================================
* FUNCTION : mrp_rec_read_priv_stby_real
*----------------------------------------------------------------------------
* PURPOSE  : read PrivInterfaceStdbyDataReal
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_DATA)
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec=internal data base reference
*            *pData=record data array
*            recDataLen=length of pData array
* OUTPUTS  : *lengthTotal=length of block
*----------------------------------------------------------------------------
* COMMENTS :  
*==========================================================================*/
static LSA_UINT16 mrp_rec_read_priv_stby_real (const MRP_REC_DB_TYPE *pDBrec,
                                                            LSA_UINT8 *pData, 
                                                            LSA_UINT32 recDataLen,
                                                            LSA_UINT16 *lengthTotal)
{
  MRP_STBY_PARAM_T StdbyState;
  LSA_UINT8 ConNameLen;
  LSA_UINT8 LengthPaddingConName;
  LSA_UINT8 LengthPaddingPortCnt;
  LSA_UINT8 LengthPaddingEnd;
  LSA_INT32 i;
  LSA_UINT16 length, numOfPorts;
  LSA_UINT16 rc, resp;

  length = *lengthTotal = 0;
  /* length check */
  StdbyState.Config = 0; /* no reset counter */
  numOfPorts = mrp_sys_get_num_of_ports(pDBrec->pChSys);

  MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)&StdbyState.pPorts, (LSA_UINT16)(sizeof(LSA_UINT8) * numOfPorts));
  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(StdbyState.pPorts, LSA_NULL));  
  /* preset zero */
  MRP_MEMSET(StdbyState.pPorts, 0, (LSA_UINT16)(sizeof(LSA_UINT8) * numOfPorts));

  mrp_stby_get_params (pDBrec->pChSys, &StdbyState);

  for(ConNameLen = 0; StdbyState.ConName[ConNameLen] != 0; ConNameLen++)
    ;

  LengthPaddingConName = (LSA_UINT8)((~ConNameLen)&3);
  LengthPaddingPortCnt = (LSA_UINT8)((~StdbyState.PortCnt)&3);
  (*lengthTotal) += ((MRP_REC_DATA_BLK_LEN_PRV_STBY_REAL_MIN - 8) + ConNameLen + LengthPaddingConName + StdbyState.PortCnt + LengthPaddingPortCnt);
  LengthPaddingEnd = 3 & (4 - (*lengthTotal));
  (*lengthTotal) += LengthPaddingEnd;
  (*lengthTotal) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;

  if (*lengthTotal > recDataLen) 
  {
    MRP_UPPER_TRACE_01(pDBrec->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> read_priv_stdby_real: ERR_PRM_DATA: out of record_data_length (need=%u)", *lengthTotal);
    *lengthTotal = 0;
    resp = MRP_RSP_ERR_PRM_DATA;
  }
  else
  {
    MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_PRV_STBY_REAL);
    
    length = (LSA_UINT16)(*lengthTotal - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN);
    
    MRP_REC_DB_PUT16_HTON(pData, length);
    MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW0);
    MRP_REC_DB_PUT16_HTON(pData, 0); /* padding */

    *pData = ConNameLen;
    pData++;

    for (i = 0; i < ConNameLen; i++) 
    {
      *pData = StdbyState.ConName[i];
      pData++;
    }

    for (i = 0; i < LengthPaddingConName; i++) 
    {
      *pData = 0;
      pData++;
    }
    *pData = StdbyState.PortCnt;
    pData++;

    for (i = 0; i < StdbyState.PortCnt; i++) 
    {
      *pData = StdbyState.pPorts[i];
      pData++;
    }

    for (i = 0; i < LengthPaddingPortCnt; i++) 
    {
      *pData = 0;
      pData++;
    }
    MRP_MEMCPY(pData, StdbyState.PartnerID, 6);
    pData += 6;
    MRP_MEMCPY(pData, StdbyState.MyID, 6);
    pData += 6;
    MRP_REC_DB_PUT16_HTON(pData, StdbyState.MyState);
    MRP_REC_DB_PUT16_HTON(pData, StdbyState.MyDiag);
    MRP_REC_DB_PUT16_HTON(pData, StdbyState.ChangeOver);

    for (i = 0; i < LengthPaddingEnd; i++) 
    {
      *pData = 0;
      pData++;
    }
    resp = MRP_RSP_OK;
  }
  MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)StdbyState.pPorts);
  MRP_FATAL1(rc == LSA_RET_OK);
  StdbyState.pPorts = LSA_NULL;

  return resp;
}


/*===========================================================================
* FUNCTION : mrp_rec_read_priv_ard_real
*----------------------------------------------------------------------------
* PURPOSE  : read PrivInterfaceStdbyDataReal
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_DATA)
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec = internal data base reference
*            *pData=record data array
*            recDataLen=length of pData array
* OUTPUTS  : *lengthTotal=length of block
*----------------------------------------------------------------------------
* COMMENTS :  
*==========================================================================*/
static LSA_UINT16 mrp_rec_read_priv_ard_real (const MRP_REC_DB_TYPE *pDBrec,
                                                          LSA_UINT8 *pData, 
                                                          LSA_UINT32 recDataLen,
                                                          LSA_UINT16 *lengthTotal)
{
  LSA_UINT8 ardRunState;
  LSA_BOOL ardHsrClientByFallback;
  LSA_UINT16 ardMode;
  LSA_UINT16 length;

  *lengthTotal = 0;
  /* length check */
  (*lengthTotal) += MRP_REC_DATA_BLK_LEN_PRV_ARD_REAL;
  (*lengthTotal) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
  if (*lengthTotal > recDataLen) 
  {
    MRP_UPPER_TRACE_01(pDBrec->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> read_priv_ard_real: ERR_PRM_DATA: out of record_data_length (need=%u)", *lengthTotal);
    *lengthTotal = 0;
    
    return MRP_RSP_ERR_PRM_DATA;
  }

  MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_PRV_ARD_REAL);
  
  length = (LSA_UINT16)(*lengthTotal - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN);
  
  MRP_REC_DB_PUT16_HTON(pData, length);
  MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW0);
  MRP_REC_DB_PUT16_HTON(pData, 0); /* padding */
  ardMode = MRP_REC_DATA_ARD_STATE_OFF;

  mrp_ard_get_state (pDBrec->pChSys, &ardRunState, &ardHsrClientByFallback);

  if (MRP_OPERATION_ON == ardRunState) 
  {
    if (ardHsrClientByFallback) 
    {
      ardMode = MRP_REC_DATA_ARD_STATE_ON_HSR_CLIENT;
    }
    else 
    {
      ardMode = MRP_REC_DATA_ARD_STATE_ON_MRP;
    }
  }
  MRP_REC_DB_PUT16_HTON(pData, ardMode);
  MRP_REC_DB_PUT16_HTON(pData, 0); /* padding */
  
  LSA_UNUSED_ARG(pData); /* pData is incremented in the previous MACRO, but not used afterwards => creates lint informational (e438). */

  return MRP_RSP_OK;
}
#endif


#ifdef MRP_CFG_PLUGIN_1_MRM_EXT
/*===========================================================================
* FUNCTION : mrp_rec_read_priv_data_adjust
*----------------------------------------------------------------------------
* PURPOSE  : read PrivMrpDataAdjust
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_DATA)
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec = internal data base reference
*            *pData       = record data array
*            readAdmin    = LSA_TRUE: read para DB, LSA_FALSE: read work DB
*            recDataLen = length of pData array
* OUTPUTS  : *lengthTotal = length of block
*----------------------------------------------------------------------------
* COMMENTS :  
*==========================================================================*/
static LSA_UINT16 mrp_rec_read_priv_data_adjust (const MRP_REC_DB_TYPE *pDBrec,
                                                            LSA_UINT8 *pData, 
                                                            LSA_UINT8 readAdmin,  
                                                            LSA_UINT32 recDataLen,
                                                            LSA_UINT16 *lengthTotal)
{
  LSA_UINT16 length;
  MRP_REC_DB_INST_SET_TYPE *pRecInstDB;

#if (MRP_CFG_TRACE_MODE == 0)
  LSA_UNUSED_ARG(pDBrec);
#endif
#if (MRP_CFG_TRACE_MODE == 1)
  LSA_UNUSED_ARG(pDBrec);
#endif

  if (readAdmin)
  {
    pRecInstDB = &pDBrec->pInstData[0].para;  
  }
  else
  {
    pRecInstDB = &pDBrec->pInstData[0].work;  
  }
  length = *lengthTotal = 0;
  
  if (pRecInstDB->RecPrivDataAdjust.Written) 
  {
    /* length check */
    (*lengthTotal) += MRP_REC_DATA_BLK_LEN_PRV_DATA_ADJUST;
    (*lengthTotal) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
    
    if (*lengthTotal > recDataLen) 
    {
      MRP_UPPER_TRACE_02(pDBrec->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> read_priv_data_adjust: ERR_PRM_DATA: blockType=0x%x, out of record_data_length (need=%u)", MRP_REC_DATA_BLK_TYPE_PRV_DATA_ADJUST, *lengthTotal);
      *lengthTotal = 0;
      
      return MRP_RSP_ERR_PRM_DATA;
    }
    MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_PRV_DATA_ADJUST);
    
    length = (LSA_UINT16)(*lengthTotal - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN);
    
    MRP_REC_DB_PUT16_HTON(pData, length);
    MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW0);
    MRP_REC_DB_PUT16_HTON(pData, 0); /* padding */      
    MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecPrivDataAdjust.MONdefaultT_ms); 
    MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecPrivDataAdjust.MONNRmax); 

    LSA_UNUSED_ARG(pData); /* pData is incremented in the previous MACRO, but not used afterwards => creates lint informational (e438). */
  }
  return MRP_RSP_OK;
}
#endif


/*===========================================================================
* FUNCTION : mrp_rec_read_priv_data_real
*----------------------------------------------------------------------------
* PURPOSE  : read PrivMrpDataReal
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_DATA)
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec = internal data base reference
*            *pData  = record data array
*            recDataLen   = length of pData array
* OUTPUTS  : *lengthTotal = length of block
*----------------------------------------------------------------------------
* COMMENTS :  
*==========================================================================*/
static LSA_UINT16 mrp_rec_read_priv_data_real (const MRP_REC_DB_TYPE *pDBrec,  
                                                           LSA_UINT8 *pData, 
                                                           LSA_UINT32 recDataLen,
                                                           LSA_UINT16 *lengthTotal)
{
  LSA_UINT16 length;
#ifdef MRP_CFG_PLUGIN_0_MRM
  MRP_MRM_PARAMS_TYPE paramsMRM;
  MRP_MRM_STATE_TYPE mrpState;
  LSA_UINT32 MrpDiag;
#endif

#if !defined(MRP_CFG_PLUGIN_0_MRM)
  LSA_UNUSED_ARG(pDBrec);  
#endif

  *lengthTotal = 0;
  /* length check */
  (*lengthTotal) += MRP_REC_DATA_BLK_LEN_PRV_DATA_REAL;
  (*lengthTotal) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
  if (*lengthTotal > recDataLen) 
  {
    MRP_UPPER_TRACE_01(pDBrec->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> read_priv_data_real: ERR_PRM_DATA: out of record_data_length (need=%u)", *lengthTotal);
    *lengthTotal = 0;
    return MRP_RSP_ERR_PRM_DATA;
  }
#ifdef MRP_CFG_PLUGIN_0_MRM
  mrp_mrm_get_params (pDBrec->pChSys, 0, &paramsMRM);
#endif
  MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_PRV_DATA_REAL);
  
  length = (LSA_UINT16)(*lengthTotal - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN);
  
  MRP_REC_DB_PUT16_HTON(pData, length);
  MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW0);
  MRP_REC_DB_PUT16_HTON(pData, 0); /* padding */   
     
#ifdef MRP_CFG_PLUGIN_0_MRM
  MRP_REC_DB_PUT16_HTON(pData, paramsMRM.Transitions); 
  MRP_REC_DB_PUT16_HTON(pData, 0); /* padding */      
  MRP_REC_DB_PUT32_HTON(pData, paramsMRM.MaxDelay);
  MRP_REC_DB_PUT16_HTON(pData, paramsMRM.NewMgrPrio); 
  MRP_MEMCPY(pData, paramsMRM.NewMgr, 6);
  
  pData += 6;
  MRP_REC_DB_PUT16_HTON(pData, paramsMRM.NewMgrCnt); 
  MRP_REC_DB_PUT16_HTON(pData, 0); /* padding */

  (void)(mrp_mrm_get_state (pDBrec->pChSys, 0, &mrpState, LSA_FALSE));

  MrpDiag = 0;
  
  if (!mrpState.mrmVotingLost) 
  {
    if (mrpState.roleMgr)
    {
      MrpDiag |= MRP_RED_DIAG_ROLE_MRM;
      if (paramsMRM.operRole == MRP_OPER_ROLE_AUTO_MGR)
      {
        MrpDiag |= MRP_RED_DIAG_ROLE_MRM_AUTO;        
      }
    }
    else
      MrpDiag |= MRP_RED_DIAG_ROLE_MRC;
      
    if (mrpState.roleMgr && mrpState.singleSideRx) /* if single side receive diag is detected, it has higher priority than ring open diag */
    {
      MrpDiag |= MRP_RED_DIAG_MRM_SINGLE_SIDE_RCV;
    }
    else
    {
      if (mrpState.roleMgr && mrpState.ringOpen)
        MrpDiag |= MRP_RED_DIAG_MRM_RING_OPEN;
    }

    if (mrpState.roleMgr && mrpState.multiRM) 
    {
      MrpDiag |= MRP_RED_DIAG_MRM_OTHER;
    }
    if (mrpState.roleMgr && mrpState.morphing) 
    {
      MrpDiag |= MRP_RED_DIAG_MRM_MORPHING;
    }
  }

  MRP_REC_DB_PUT32_HTON(pData, MrpDiag); 
#else
  MRP_REC_DB_PUT16_HTON(pData, 0); /* Transitions */
  MRP_REC_DB_PUT16_HTON(pData, 0); /* padding */      
  MRP_REC_DB_PUT32_HTON(pData, 0); /* MRMMaxDelay */
  MRP_REC_DB_PUT16_HTON(pData, 0); /* NewMgrPrio */
  MRP_MEMSET(pData, 0, 6);          /* NewMgr */
  pData += 6;
  MRP_REC_DB_PUT16_HTON(pData, 0); /* NewMgrTransitions */
  MRP_REC_DB_PUT16_HTON(pData, 0); /* padding */
  MRP_REC_DB_PUT32_HTON(pData, 0); /* Diag */
#endif
  MRP_REC_DB_PUT16_HTON(pData, 0); 
  MRP_REC_DB_PUT16_HTON(pData, 0); /* padding */

  LSA_UNUSED_ARG(pData); /* pData is incremented in the previous MACRO, but not used afterwards => creates lint informational (e438). */

  return MRP_RSP_OK;
}


/*===========================================================================
* FUNCTION : mrp_rec_read_interface_data_adjust
*----------------------------------------------------------------------------
* PURPOSE  : read PDInterfaceMrpDataAdjust
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_DATA)
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec = internal data base reference
*            *pData       = record data array
*            readAdmin    = LSA_TRUE: read para DB, LSA_FALSE: read work DB
*            recDataLen   = length of pData array
* OUTPUTS  : *lengthTotal = length of block
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_UINT16 mrp_rec_read_interface_data_adjust (MRP_REC_DB_TYPE *pDBrec,  
                                                            LSA_UINT8 *pData, 
                                                            LSA_UINT8 readAdmin,  
                                                            LSA_UINT32 recDataLen,
                                                            LSA_UINT16 *lengthTotal)
{
  LSA_UINT16 i, k, l;
  LSA_UINT16 mrpMaxInstance;
  MRP_REC_DB_INST_SET_TYPE *pRecInstDB;
  MRP_REC_DB_IF_SET_TYPE *pRecIfDB;
#ifdef MRP_CFG_PLUGIN_0_MRM
  MRP_MRM_DB_TYPE *pDBmrm;

  MRP_MRM_DB_TYPE *pDBmrmBase = (MRP_MRM_DB_TYPE *)pDBrec->pChSys->pChSysDB->pMrm;
#endif

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBrec->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  *lengthTotal = 0;
  mrpMaxInstance = mrp_sys_get_num_of_instances(pDBrec->pChSys);

  if (!mrpMaxInstance)
    return MRP_RSP_ERR_PRM_DATA;

  if (readAdmin)
  {
    pRecIfDB   = &pDBrec->para;
    pRecInstDB = &pDBrec->pInstData[0].para;  
  }
  else
  {
    pRecIfDB   = &pDBrec->work;
#ifdef MRP_CFG_PLUGIN_0_MRM
    pDBmrm = &pDBmrmBase[0];

    if (pDBmrm->Morphing)
      pRecInstDB   = &pDBrec->pInstData[0].saveWork;
    else
#endif
      pRecInstDB = &pDBrec->pInstData[0].work;  
  }

  if (pRecIfDB->RecIfAdjustHead.Written)
  {
    if (pRecIfDB->RecIfAdjustHead.head.blockVersionLow == MRP_REC_DATA_BLK_VERS_LOW0)
    {
      /* length check */
      if (pRecInstDB->RecInstanceAdjust.Written) 
      {
        (*lengthTotal)  = pRecIfDB->RecIfAdjustHead.head.blockLength;
        (*lengthTotal) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
        
        if (*lengthTotal > recDataLen) 
        {
          MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> read_interface_data_adjust: ERR_PRM_DATA (blockType=0x%x): out of record_data_length(=%d), needed=%u",
            MRP_REC_DATA_BLK_TYPE_IF_DATA_ADJUST, recDataLen, *lengthTotal);
          *lengthTotal = 0;
          
          return MRP_RSP_ERR_PRM_DATA;
        }
      }
      
      if (pRecInstDB->RecInstanceAdjust.Written) 
      {
        MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_IF_DATA_ADJUST);
        MRP_REC_DB_PUT16_HTON(pData, pRecIfDB->RecIfAdjustHead.head.blockLength);
        MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW0);
        MRP_REC_DB_PUT16_HTON(pData, 0); /* padding */  
            
        MRP_MEMCPY(pData, pRecInstDB->RecInstanceAdjust.DomainUUID, sizeof(pRecInstDB->RecInstanceAdjust.DomainUUID));

        pData += sizeof(pRecInstDB->RecInstanceAdjust.DomainUUID);

        MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecInstanceAdjust.Role);
        MRP_REC_DB_PUT16_HTON(pData, 0); /* padding */  

        /* DomainName */
        *pData = pRecInstDB->RecInstanceAdjust.LengthDomainName;
        pData += MRP_REC_DATA_LEN_DOMAIN_NAME_FIELD;
        
        MRP_MEMCPY(pData, pRecInstDB->RecInstanceAdjust.DomainName, pRecInstDB->RecInstanceAdjust.LengthDomainName);
        pData += pRecInstDB->RecInstanceAdjust.LengthDomainName;

        for (i = 0; i < pRecInstDB->RecInstanceAdjust.LengthDomainNamePadding; i++) 
        {
          *pData = 0;
          pData++;
        }
        if (pRecInstDB->RecInstanceAdjust.client.Written) 
        {
          if (pRecInstDB->RecInstanceAdjust.Role != MRP_PROJ_ROLE_CLIENT) 
          {
            MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, ">> read_interface_data_adjust (blockType=0x%x): INCONSISTENCY: Role=%d, but client data available",
              MRP_REC_DATA_BLK_TYPE_IF_DATA_ADJUST, pRecInstDB->RecInstanceAdjust.Role);
          }
          MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_CLIENT_PARAMS);
          MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_LEN_CLIENT_PARAMS);
          MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW0);
          MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecInstanceAdjust.client.LNKdownT_ms);
          MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecInstanceAdjust.client.LNKupT_ms);
          MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecInstanceAdjust.client.LNKNRmax);
        }
        if (pRecInstDB->RecInstanceAdjust.mgr.Written) 
        {
          if (pRecInstDB->RecInstanceAdjust.Role != MRP_PROJ_ROLE_MGR) 
          {
            MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, ">> read_interface_data_adjust (blockType=0x%x): INCONSISTENCY: Role=%d, but manager data available",
              MRP_REC_DATA_BLK_TYPE_IF_DATA_ADJUST, pRecInstDB->RecInstanceAdjust.Role);
          }
          MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_MANAGER_PARAMS);
          MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_LEN_MANAGER_PARAMS);
          MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW0);
          MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecInstanceAdjust.mgr.Prio);
          MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecInstanceAdjust.mgr.TOPchgT_10ms);
          MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecInstanceAdjust.mgr.TOPNRmax);
          MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecInstanceAdjust.mgr.TSTshortTms);
          MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecInstanceAdjust.mgr.TSTdefaultTms);
          MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecInstanceAdjust.mgr.TSTNRmax);
          MRP_REC_DB_PUT16_HTON(pData, 0); /* padding */
        }
      }  
    }
    else /* MRP_REC_DATA_BLK_VERS_LOW1 */
    {
      (*lengthTotal)  = pRecIfDB->RecIfAdjustHead.head.blockLength;
      (*lengthTotal) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;    
    
      if (*lengthTotal > recDataLen) 
      {
        MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> read_interface_data_adjust: ERR_PRM_DATA (blockType=0x%x): out of record_data_length(=%d), needed=%u",
          MRP_REC_DATA_BLK_TYPE_IF_DATA_ADJUST, recDataLen, *lengthTotal);
        *lengthTotal = 0;

        return MRP_RSP_ERR_PRM_DATA;
      }      

      MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_IF_DATA_ADJUST);
      MRP_REC_DB_PUT16_HTON(pData, pRecIfDB->RecIfAdjustHead.head.blockLength);
      MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW1);
      MRP_REC_DB_PUT16_HTON(pData, pRecIfDB->RecIfAdjustHead.head.v.v_new.numOfMrpInstances); /* NumOfMrpInstances */
      
      for (i = 0, k = 0; i < mrpMaxInstance; i++)
      {
        if (readAdmin)
        {
          pRecInstDB   = &pDBrec->pInstData[i].para;  
        }
        else
        {
#ifdef MRP_CFG_PLUGIN_0_MRM
          pDBmrm = &pDBmrmBase[i];

          if (pDBmrm->Morphing)
            pRecInstDB   = &pDBrec->pInstData[i].saveWork;
          else
#endif
            pRecInstDB   = &pDBrec->pInstData[i].work;
        }  
        
        if (pRecInstDB->RecInstanceAdjust.Written) 
        {
          MRP_ASSERT(k < pRecIfDB->RecIfAdjustHead.head.v.v_new.numOfMrpInstances);
          k++;
          
          MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_INST_DATA_ADJUST);
          MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecInstanceAdjust.head.v.v_new.blockLength);
          MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW0);
          MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecInstanceAdjust.head.v.v_new.instance); /* MRP_Instance */      
          MRP_MEMCPY(pData, pRecInstDB->RecInstanceAdjust.DomainUUID, sizeof(pRecInstDB->RecInstanceAdjust.DomainUUID));

          pData += sizeof(pRecInstDB->RecInstanceAdjust.DomainUUID);

          MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecInstanceAdjust.Role);
          MRP_REC_DB_PUT16_HTON(pData, 0); /* padding */  

          /* DomainName */
          *pData = pRecInstDB->RecInstanceAdjust.LengthDomainName;
          pData += MRP_REC_DATA_LEN_DOMAIN_NAME_FIELD;

          MRP_MEMCPY(pData, pRecInstDB->RecInstanceAdjust.DomainName, pRecInstDB->RecInstanceAdjust.LengthDomainName);
          pData += pRecInstDB->RecInstanceAdjust.LengthDomainName;

          for (l = 0; l < pRecInstDB->RecInstanceAdjust.LengthDomainNamePadding; l++) 
          {
            *pData = 0;
            pData++;
          }
          if (pRecInstDB->RecInstanceAdjust.client.Written) 
          {
            if (pRecInstDB->RecInstanceAdjust.Role != MRP_PROJ_ROLE_CLIENT) 
            {
              MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, ">> read_interface_data_adjust (blockType=0x%x): INCONSISTENCY: Role=%d, but client data available",
                MRP_REC_DATA_BLK_TYPE_INST_DATA_ADJUST, pRecInstDB->RecInstanceAdjust.Role);
            }
            MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_CLIENT_PARAMS);
            MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_LEN_CLIENT_PARAMS);
            MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW0);
            MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecInstanceAdjust.client.LNKdownT_ms);
            MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecInstanceAdjust.client.LNKupT_ms);
            MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecInstanceAdjust.client.LNKNRmax);
          }
          if (pRecInstDB->RecInstanceAdjust.mgr.Written) 
          {
            if (pRecInstDB->RecInstanceAdjust.Role != MRP_PROJ_ROLE_MGR && pRecInstDB->RecInstanceAdjust.Role != MRP_PROJ_ROLE_AUTO_MGR) 
            {
              MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, ">> read_interface_data_adjust (blockType=0x%x): INCONSISTENCY: Role=%d, but manager data available",
                MRP_REC_DATA_BLK_TYPE_INST_DATA_ADJUST, pRecInstDB->RecInstanceAdjust.Role);
            }
            MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_MANAGER_PARAMS);
            MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_LEN_MANAGER_PARAMS);
            MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW0);
            MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecInstanceAdjust.mgr.Prio);
            MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecInstanceAdjust.mgr.TOPchgT_10ms);
            MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecInstanceAdjust.mgr.TOPNRmax);
            MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecInstanceAdjust.mgr.TSTshortTms);
            MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecInstanceAdjust.mgr.TSTdefaultTms);
            MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecInstanceAdjust.mgr.TSTNRmax);
            MRP_REC_DB_PUT16_HTON(pData, 0); /* padding */
          }
        }
      }      
    }
  }

  LSA_UNUSED_ARG(pData); /* pData is incremented in the previous MACRO, but not used afterwards => creates lint informational (e438). */

  return MRP_RSP_OK;
}


/*===========================================================================
* FUNCTION : mrp_rec_read_port_data_adjust
*----------------------------------------------------------------------------
* PURPOSE  : parse PDPortMrpDataAdjust
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_DATA)
*----------------------------------------------------------------------------
* INPUTS   : 
*            *pDBrec     = internal data base reference
*            *pData  = record data array
*            readAdmin   = LSA_TRUE: read para DB, LSA_FALSE: read work DB
*            recDataLen  = length of pData array
*            portID      = edd port id (1..MAX_PORTS)
* OUTPUTS  : *lengthTotal= length of block
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_UINT16 mrp_rec_read_port_data_adjust (const MRP_REC_DB_TYPE *pDBrec,
                                                             LSA_UINT8 *pData, 
                                                             LSA_UINT8 readAdmin,  
                                                             LSA_UINT16 NumOfPossibleRports,
                                                             LSA_UINT32 recDataLen,
                                                             LSA_UINT32 portID,
                                                             LSA_UINT16 *lengthTotal)
{
  LSA_UINT16 length, i, k;
  LSA_UINT16 mrpMaxInstance;
  LSA_UINT8 found;
  MRP_REC_DB_INST_SET_TYPE *pRecInstDB;
#ifdef MRP_CFG_PLUGIN_0_MRM
  MRP_MRM_DB_TYPE *pDBmrm;

  MRP_MRM_DB_TYPE *pDBmrmBase = (MRP_MRM_DB_TYPE *)pDBrec->pChSys->pChSysDB->pMrm;
#endif
  mrpMaxInstance = mrp_sys_get_num_of_instances (pDBrec->pChSys);

  if (!mrpMaxInstance)
     return MRP_RSP_ERR_PRM_DATA;

  if (readAdmin)
  {
    pRecInstDB = &pDBrec->pInstData[0].para;  
  }
  else
  {
    pRecInstDB = &pDBrec->pInstData[0].work;  
  }

  length = *lengthTotal = 0;
  k = 0;

  for (i = 0; i < NumOfPossibleRports; i++)
  {
    if (pDBrec->pRPortArray[i] == portID)
      break;
  }

  if (portID == 0 || i >= NumOfPossibleRports) 
  {
    MRP_UPPER_TRACE_01(pDBrec->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, ">> read_port_data_adjust: ERR_PRM_PORTID: port id out of range (port=%u)", portID);


    return MRP_RSP_ERR_PRM_PORTID;
  }
  
  for (i = 0, found = LSA_FALSE; i < mrpMaxInstance  && !found; i++)
  {
    if (readAdmin)
    {
      pRecInstDB = &pDBrec->pInstData[i].para;        
    }
    else
    {
#ifdef MRP_CFG_PLUGIN_0_MRM
      pDBmrm = &pDBmrmBase[i];

      if (pDBmrm->Morphing)
        pRecInstDB   = &pDBrec->pInstData[i].saveWork;
      else
#endif
        pRecInstDB   = &pDBrec->pInstData[i].work;
    }
    /* value check */
    for (k = 0; k < MRP_DEFAULT_VAL_INST_RPORT_CNT; k++) 
    {
      if (pRecInstDB->RecPortAdjust[k].PortID == portID) 
      {
        found = LSA_TRUE;
        break;
      }
    }  
  }
  /* length check */
  if (k < MRP_DEFAULT_VAL_INST_RPORT_CNT && pRecInstDB->RecPortAdjust[k].Written) 
  {
    (*lengthTotal) += MRP_REC_DATA_BLK_LEN_PORT_DATA_ADJUST;
    (*lengthTotal) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
    
    if (*lengthTotal > recDataLen) 
    {
      MRP_UPPER_TRACE_02(pDBrec->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> read_port_data_adjust: ERR_PRM_DATA: out of record_data_length(=%d), needed=%u", recDataLen, *lengthTotal);
      *lengthTotal = 0;
      
      return MRP_RSP_ERR_PRM_DATA;
    }
    
    if (pRecInstDB->RecPortAdjust[k].Written) 
    {
      MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_PORT_DATA_ADJUST);

      length = (LSA_UINT16)(*lengthTotal - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN);

      MRP_REC_DB_PUT16_HTON(pData, length);

      if (pRecInstDB->RecPortAdjust[k].head.v.v_new.blockVersionLow == MRP_REC_DATA_BLK_VERS_LOW0)
      {
        MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW0);
        MRP_REC_DB_PUT16_HTON(pData, 0); /* padding */          
      }
      else
      {
        MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW1);
        MRP_REC_DB_PUT16_HTON(pData, (LSA_UINT16)pRecInstDB->RecPortAdjust[k].head.v.v_new.instance); /* MRP_Instance */          
      }

      MRP_MEMCPY(pData, pRecInstDB->RecPortAdjust[k].DomainUUID, sizeof(pRecInstDB->RecPortAdjust->DomainUUID));              
      pData += sizeof(pRecInstDB->RecPortAdjust->DomainUUID);
    }    
  }

  LSA_UNUSED_ARG(pData); /* pData is incremented in the previous MACRO, but not used afterwards => creates lint informational (e438). */

  return MRP_RSP_OK;
}


/*===========================================================================
* FUNCTION : mrp_rec_read_interface_data_check
*----------------------------------------------------------------------------
* PURPOSE  : parse PDInterfaceMrpDataCheck
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PRM_DATA)
*----------------------------------------------------------------------------
* INPUTS   : 
*            *pDBrec      = internal data base reference
*            *pData       = record data array
*            readAdmin    = LSA_TRUE: read para DB, LSA_FALSE: read work DB
*            recDataLen=length of pData array
* OUTPUTS  : *lengthTotal=length of block
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_UINT16 mrp_rec_read_interface_data_check (MRP_REC_DB_TYPE *pDBrec,
                                                          LSA_UINT8 *pData, 
                                                          LSA_UINT8 readAdmin,  
                                                          LSA_UINT32 recDataLen,
                                                          LSA_UINT16 *lengthTotal)
{
  LSA_UINT16 i, k;
  LSA_UINT16 mrpMaxInstance;
  MRP_REC_DB_INST_SET_TYPE *pRecInstDB;
  MRP_REC_DB_IF_SET_TYPE *pRecIfDB;
#ifdef MRP_CFG_PLUGIN_0_MRM
  MRP_MRM_DB_TYPE *pDBmrm;

  MRP_MRM_DB_TYPE *pDBmrmBase = (MRP_MRM_DB_TYPE *)pDBrec->pChSys->pChSysDB->pMrm;
#endif

  *lengthTotal = 0;
  mrpMaxInstance = mrp_sys_get_num_of_instances(pDBrec->pChSys);

  if (!mrpMaxInstance)
     return MRP_RSP_ERR_PRM_DATA;

  if (readAdmin)
  {
    pRecIfDB   = &pDBrec->para;
    pRecInstDB = &pDBrec->pInstData[0].para;  
  }
  else
  {
    pRecIfDB   = &pDBrec->work;
#ifdef MRP_CFG_PLUGIN_0_MRM
    pDBmrm = &pDBmrmBase[0];

    if (pDBmrm->Morphing)
      pRecInstDB   = &pDBrec->pInstData[0].saveWork;
    else
#endif          
      pRecInstDB = &pDBrec->pInstData[0].work;  
  }
  
  if (pRecIfDB->RecIfCheckHead.Written)
  {
    if (pRecIfDB->RecIfCheckHead.head.blockVersionLow == MRP_REC_DATA_BLK_VERS_LOW0)
    {
      /* length check */
      if (pRecInstDB->RecInstanceCheck.Written) 
      {
        (*lengthTotal) += pRecIfDB->RecIfCheckHead.head.blockLength;
        (*lengthTotal) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;

        if (*lengthTotal > recDataLen) 
        {
          MRP_UPPER_TRACE_03(pDBrec->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> read_interface_data_check: ERR_PRM_DATA: blockType=0x%x, out of record_data_length(=%d), needed=%u", 
                             MRP_REC_DATA_BLK_TYPE_IF_DATA_CHECK, recDataLen, *lengthTotal);
          *lengthTotal = 0;

          return MRP_RSP_ERR_PRM_DATA;
        }
      }
      if (pRecInstDB->RecInstanceCheck.Written) 
      {
        MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_IF_DATA_CHECK);
        MRP_REC_DB_PUT16_HTON(pData, pRecIfDB->RecIfCheckHead.head.blockLength);
        MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW0);
        MRP_REC_DB_PUT16_HTON(pData, 0); /* padding */
        
        MRP_MEMCPY(pData, pRecInstDB->RecInstanceCheck.DomainUUID, sizeof(pRecInstDB->RecInstanceCheck.DomainUUID));  
        pData += sizeof(pRecInstDB->RecInstanceCheck.DomainUUID);
        
        MRP_REC_DB_PUT32_HTON(pData, pRecInstDB->RecInstanceCheck.Check);
      }    
    }
    else  /* blockVersionLow == MRP_REC_DATA_BLK_VERS_LOW1 */
    {
      (*lengthTotal) += pRecIfDB->RecIfCheckHead.head.blockLength;
      (*lengthTotal) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
      
      if (*lengthTotal > recDataLen) 
      {
        MRP_UPPER_TRACE_03(pDBrec->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> read_interface_data_check: ERR_PRM_DATA: blockType=0x%x, out of record_data_length(=%d), needed=%u", MRP_REC_DATA_BLK_TYPE_IF_DATA_CHECK, recDataLen, *lengthTotal);
        *lengthTotal = 0;

        return MRP_RSP_ERR_PRM_DATA;
      }

      MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_IF_DATA_CHECK);
      MRP_REC_DB_PUT16_HTON(pData, pRecIfDB->RecIfCheckHead.head.blockLength);
      MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW1);
      MRP_REC_DB_PUT16_HTON(pData, pRecIfDB->RecIfCheckHead.head.v.v_new.numOfMrpInstances); /* NumOfMrpInstances */
      
      for (i = 0, k = 0; i < mrpMaxInstance; i++)
      {
        if (readAdmin)
        {
          pRecInstDB   = &pDBrec->pInstData[i].para;  
        }
        else
        {
#ifdef MRP_CFG_PLUGIN_0_MRM
          pDBmrm = &pDBmrmBase[i];

          if (pDBmrm->Morphing)
            pRecInstDB   = &pDBrec->pInstData[i].saveWork;
          else
#endif          
            pRecInstDB   = &pDBrec->pInstData[i].work;  
        }        
        
        if (pRecInstDB->RecInstanceCheck.Written) 
        {
          MRP_ASSERT(k < pRecIfDB->RecIfCheckHead.head.v.v_new.numOfMrpInstances);
          k++;
          
          MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_INST_DATA_CHECK);
          MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->RecInstanceCheck.head.v.v_new.blockLength);
          MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW0);
          MRP_REC_DB_PUT16_HTON(pData, i);                     /* MRP_Instance */

          MRP_MEMCPY(pData, pRecInstDB->RecInstanceCheck.DomainUUID, sizeof(pRecInstDB->RecInstanceCheck.DomainUUID));  
          pData += sizeof(pRecInstDB->RecInstanceCheck.DomainUUID);

          MRP_REC_DB_PUT32_HTON(pData, pRecInstDB->RecInstanceCheck.Check);      
        }
      }
    }
  }
  LSA_UNUSED_ARG(pData); /* pData is incremented in the previous MACRO, but not used afterwards => creates lint informational (e438). */

  return MRP_RSP_OK;
}


/*===========================================================================
* FUNCTION : mrp_rec_read_interface_data_real
*----------------------------------------------------------------------------
* PURPOSE  : parse PDInterfaceMrpDataReal
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_DATA)
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec=internal data base reference
*            *pData=record data array
*            recDataLen=length of pData array
* OUTPUTS  : *lengthTotal=length of block
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_UINT16 mrp_rec_read_interface_data_real (MRP_REC_DB_TYPE *pDBrec,  
                                                          LSA_UINT8 *pData, 
                                                          LSA_UINT32 recDataLen,
                                                          LSA_UINT16 *lengthTotal)
{
#ifdef MRP_CFG_PLUGIN_0_MRM
  MRP_MRM_PARAMS_TYPE paramsMRM;
  MRP_MRM_STATE_TYPE mrpState;
#endif
  MRP_MRC_PARAMS_TYPE paramsMRC;
  LSA_UINT16 Role;
  LSA_UINT16 RingState;
  LSA_UINT16 length, instLength[EDD_CFG_MAX_MRP_INSTANCE_CNT];
  LSA_UINT8  DomainUUID[16];
  LSA_UINT16 Version = MRP_PACKET_VAL_VERSION;
  LSA_UINT16 i, k;
  LSA_UINT16 mrpMaxInstance;
  MRP_REC_DB_INST_SET_TYPE *pRecInstDB;
  MRP_REC_DB_IF_SET_TYPE *pRecIfWork;

  /* mrm params */
  Role      = MRP_OPER_ROLE_OFF;
  RingState = MRP_REC_DATA_INST_RING_STATE_OPEN;
  
  mrpMaxInstance = mrp_sys_get_num_of_instances(pDBrec->pChSys);

  if (!mrpMaxInstance)
    return MRP_RSP_ERR_PRM_DATA;

  pRecIfWork = &pDBrec->work;
  pRecInstDB = &pDBrec->pInstData[0].work;
  
  for (i = 0; i < EDD_CFG_MAX_MRP_INSTANCE_CNT; i++)
    instLength[i] = 0; 
    
  i = 0;  
  
  /* if PDInterfaceMrpDataAdjust dataset has old version -> deliver PDInterfaceMrpDataReal with old version */
  if (pRecIfWork->RecIfAdjustHead.head.blockVersionLow == MRP_REC_DATA_BLK_VERS_LOW0)
  {
    MRP_MEMCPY(DomainUUID, pRecInstDB->RecInstanceAdjust.DomainUUID, sizeof(DomainUUID));
  
#ifdef MRP_CFG_PLUGIN_0_MRM
    (void)(mrp_mrm_get_state (pDBrec->pChSys, i, &mrpState, LSA_FALSE));
    mrp_mrm_get_params (pDBrec->pChSys, i, &paramsMRM);

    if (!mrpState.ringOpen) 
    {
      RingState = MRP_REC_DATA_INST_RING_STATE_CLOSED;
    }
    if (MRP_OPERATION_ON == paramsMRM.Mode) 
    {
      MRP_MEMCPY(DomainUUID, paramsMRM.DomainUUID, sizeof(DomainUUID));

      if (mrpState.roleMgr) 
      {
        Role = MRP_OPER_ROLE_MGR;
      }
      else 
      {
        Role = MRP_OPER_ROLE_CLIENT;
      }
    }
#endif
    /* mrc params */
    mrp_mrc_get_params (pDBrec->pChSys, i, &paramsMRC);

    if (MRP_OPERATION_ON == paramsMRC.Mode) 
    {
      Role = MRP_OPER_ROLE_CLIENT;
      MRP_MEMCPY(DomainUUID, paramsMRC.DomainUUID, sizeof(DomainUUID));
    }

    /* length check */
    *lengthTotal = 0;
    (*lengthTotal) = (LSA_UINT16)(MRP_REC_DATA_BLK_LEN_IF_DATA_REAL_VERS1_MIN + pRecInstDB->RecInstanceAdjust.LengthDomainName + pRecInstDB->RecInstanceAdjust.LengthDomainNamePadding);
    (*lengthTotal) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;

    if (Role == MRP_OPER_ROLE_CLIENT) 
    {
      (*lengthTotal) += MRP_REC_DATA_BLK_LEN_CLIENT_PARAMS;
      (*lengthTotal) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN; /* BlockHeader length */   
    }
    else if (Role == MRP_OPER_ROLE_MGR) 
    {
      (*lengthTotal) += MRP_REC_DATA_BLK_LEN_MANAGER_PARAMS;
      (*lengthTotal) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
      
      (*lengthTotal) += MRP_REC_DATA_BLK_LEN_RING_STATE_DATA;
      (*lengthTotal) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN; /* BlockHeader length */
    }
    if (*lengthTotal > recDataLen) 
    {
      MRP_UPPER_TRACE_02(pDBrec->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> read_interface_data_real: ERR_PRM_DATA: out of record_data_length(=%d), needed=%u", recDataLen, *lengthTotal);
      *lengthTotal = 0;
      
      return MRP_RSP_ERR_PRM_DATA;
    }
    MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_IF_DATA_REAL);

    length = (LSA_UINT16)(*lengthTotal - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN);

    MRP_REC_DB_PUT16_HTON(pData, length);
    MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW1);
    MRP_REC_DB_PUT16_HTON(pData, 0); /* padding */
    MRP_MEMCPY(pData, DomainUUID, sizeof(DomainUUID));

    pData += sizeof(DomainUUID);
    MRP_REC_DB_PUT16_HTON(pData, Role);
  /*----------------------------------------*/
  /* version LOW0 of IEC standard:          */
  /* MRP_ROLE, MRP_DomainLength, MRP_Domain, Padding, MRP_Version, MRP-Subblocks */
  /*----------------------------------------*/
  /*----------------------------------------*/
  /* version LOW1 of IEC standard:          */
  /* MRP_ROLE, MRP_Version, MRP_DomainLength, MRP_Domain, Padding, MRP-Subblocks */
  /*----------------------------------------*/
    MRP_REC_DB_PUT16_HTON(pData, Version);

  /* DomainName */
    *pData = pRecInstDB->RecInstanceAdjust.LengthDomainName;
    pData += MRP_REC_DATA_LEN_DOMAIN_NAME_FIELD;
    
    MRP_MEMCPY(pData, pRecInstDB->RecInstanceAdjust.DomainName, pRecInstDB->RecInstanceAdjust.LengthDomainName);
    pData += pRecInstDB->RecInstanceAdjust.LengthDomainName;

    for (k = 0; k < pRecInstDB->RecInstanceAdjust.LengthDomainNamePadding; k++) 
    {
      *pData = 0;
      pData++;
    }
    /* moved with version LOW1 of IEC standard, version LOW0 is not supported any longer! */
    /* MRP_REC_DB_PUT16_HTON(pData, Version);   */

    if (MRP_OPER_ROLE_CLIENT == Role) 
    {
      MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_CLIENT_PARAMS);
      MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_LEN_CLIENT_PARAMS);
      MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW0);
      MRP_REC_DB_PUT16_HTON(pData, paramsMRC.LNKdownT_ms);
      MRP_REC_DB_PUT16_HTON(pData, paramsMRC.LNKupT_ms);
      MRP_REC_DB_PUT16_HTON(pData, paramsMRC.LNKNRmax);
    }
#ifdef MRP_CFG_PLUGIN_0_MRM
    else 
    {
      if (MRP_OPER_ROLE_MGR == Role) 
      {
        MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_MANAGER_PARAMS);
        MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_LEN_MANAGER_PARAMS);
        MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW0);
        MRP_REC_DB_PUT16_HTON(pData, paramsMRM.Prio);
        MRP_REC_DB_PUT16_HTON(pData, paramsMRM.TOPchgT_10ms);
        MRP_REC_DB_PUT16_HTON(pData, paramsMRM.TOPNRmax);
        MRP_REC_DB_PUT16_HTON(pData, paramsMRM.TSTshortTms);
        MRP_REC_DB_PUT16_HTON(pData, paramsMRM.TSTdefaultTms);
        MRP_REC_DB_PUT16_HTON(pData, paramsMRM.TSTNRmax);
        MRP_REC_DB_PUT16_HTON(pData, 0); /* padding */
        
        /* RING_STATE_DATA are only delivered, if MRM is active */
        MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_RING_STATE_DATA);
        MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_LEN_RING_STATE_DATA);
        MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW0);
        MRP_REC_DB_PUT16_HTON(pData, RingState);
      }
    }  
#endif
  }
  else /* blockVersionLow = MRP_REC_DATA_BLK_VERS_LOW1 */
  {
    /* length check */
    *lengthTotal = 0;
    (*lengthTotal) += MRP_REC_DATA_BLK_LEN_IF_DATA_REAL_VERS2_MIN;
    (*lengthTotal) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;    
    
    for (i = 0; i < mrpMaxInstance; i++)
    {
      instLength[i] = 0;
      pRecInstDB = &pDBrec->pInstData[i].work;

      Role      = MRP_OPER_ROLE_OFF;
      RingState = MRP_REC_DATA_INST_RING_STATE_OPEN;
     
#ifdef MRP_CFG_PLUGIN_0_MRM
      (void)(mrp_mrm_get_state (pDBrec->pChSys, i, &mrpState, LSA_FALSE));
      mrp_mrm_get_params (pDBrec->pChSys, i, &paramsMRM);

      if (!mrpState.ringOpen) 
      {
        RingState = MRP_REC_DATA_INST_RING_STATE_CLOSED;
      }
      if (MRP_OPERATION_ON == paramsMRM.Mode) 
      {
        if (mrpState.roleMgr) 
        {
          if (paramsMRM.operRole == MRP_OPER_ROLE_MGR)
            Role = MRP_OPER_ROLE_MGR;
          else
            Role = MRP_OPER_ROLE_AUTO_MGR;
        }
        else 
        {
          Role = MRP_OPER_ROLE_CLIENT;
        }
      }
#endif
      /* mrc params */
      mrp_mrc_get_params (pDBrec->pChSys, i, &paramsMRC);

      if (MRP_OPERATION_ON == paramsMRC.Mode) 
      {
        Role = MRP_OPER_ROLE_CLIENT;
      }
            
      (*lengthTotal) += (LSA_UINT16)(MRP_REC_DATA_BLK_LEN_IF_DATA_REAL_VERS1_MIN + pRecInstDB->RecInstanceAdjust.LengthDomainName + pRecInstDB->RecInstanceAdjust.LengthDomainNamePadding);
      (*lengthTotal) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
      
      instLength[i] += (LSA_UINT16)(MRP_REC_DATA_BLK_LEN_IF_DATA_REAL_VERS1_MIN + pRecInstDB->RecInstanceAdjust.LengthDomainName + pRecInstDB->RecInstanceAdjust.LengthDomainNamePadding);
      instLength[i] += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
      
      if (Role == MRP_OPER_ROLE_CLIENT) 
      {
        (*lengthTotal) += MRP_REC_DATA_BLK_LEN_CLIENT_PARAMS;
        (*lengthTotal) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN; /* BlockHeader length */  
         
        instLength[i] += MRP_REC_DATA_BLK_LEN_CLIENT_PARAMS;
        instLength[i] += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN; /* BlockHeader length */   
      }
      else if (Role == MRP_OPER_ROLE_MGR || Role == MRP_OPER_ROLE_AUTO_MGR) 
      {
        (*lengthTotal) += MRP_REC_DATA_BLK_LEN_MANAGER_PARAMS;
        (*lengthTotal) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;

        (*lengthTotal) += MRP_REC_DATA_BLK_LEN_RING_STATE_DATA;
        (*lengthTotal) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN; /* BlockHeader length */
        
        instLength[i] += MRP_REC_DATA_BLK_LEN_MANAGER_PARAMS;
        instLength[i] += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;

        instLength[i] += MRP_REC_DATA_BLK_LEN_RING_STATE_DATA;
        instLength[i] += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN; /* BlockHeader length */
      }
    }
    if (*lengthTotal > recDataLen) 
    {
      MRP_UPPER_TRACE_02(pDBrec->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> read_interface_data_real: ERR_PRM_DATA: out of record_data_length(=%d), needed=%u", recDataLen, *lengthTotal);
      *lengthTotal = 0;

      return MRP_RSP_ERR_PRM_DATA;
    }
    
    /* now copy data of PDInterfaceMrpDataReal dataset to buffer */
    MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_IF_DATA_REAL);

    length = (LSA_UINT16)(*lengthTotal - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN);

    MRP_REC_DB_PUT16_HTON(pData, length);
    MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW2);
    MRP_REC_DB_PUT16_HTON(pData, mrpMaxInstance);        /* NumberOfMrpInstances */
    
    for (i = 0; i < mrpMaxInstance; i++)
    {
      pRecInstDB = &pDBrec->pInstData[i].work;

      Role      = MRP_OPER_ROLE_OFF;
      MRP_MEMCPY(DomainUUID, pRecInstDB->RecInstanceAdjust.DomainUUID, sizeof(DomainUUID));
#ifdef MRP_CFG_PLUGIN_0_MRM
      (void)(mrp_mrm_get_state (pDBrec->pChSys, i, &mrpState, LSA_FALSE));
      mrp_mrm_get_params (pDBrec->pChSys, i, &paramsMRM);

      RingState = mrpState.ringOpen ? MRP_REC_DATA_INST_RING_STATE_OPEN : MRP_REC_DATA_INST_RING_STATE_CLOSED;

      if (MRP_OPERATION_ON == paramsMRM.Mode) 
      {
        MRP_MEMCPY(DomainUUID, paramsMRM.DomainUUID, sizeof(DomainUUID));

        if (mrpState.roleMgr) 
        {
          if (paramsMRM.operRole == MRP_OPER_ROLE_MGR)
            Role = MRP_OPER_ROLE_MGR;
          else
            Role = MRP_OPER_ROLE_AUTO_MGR;
        }
        else 
        {
          Role = MRP_OPER_ROLE_CLIENT;
        }
      }
#endif
      /* mrc params */
      mrp_mrc_get_params (pDBrec->pChSys, i, &paramsMRC);

      if (MRP_OPERATION_ON == paramsMRC.Mode) 
      {
        Role = MRP_OPER_ROLE_CLIENT;
        MRP_MEMCPY(DomainUUID, paramsMRC.DomainUUID, sizeof(DomainUUID));
      }    
    
      MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_INST_DATA_REAL);
      MRP_REC_DB_PUT16_HTON(pData, instLength[i] - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN);
      MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW0);
      MRP_REC_DB_PUT16_HTON(pData, i);                   /* MRP_Instance */    
        
      MRP_MEMCPY(pData, DomainUUID, sizeof(DomainUUID));

      pData += sizeof(DomainUUID);
      MRP_REC_DB_PUT16_HTON(pData, Role);

      /* version LOW0 of blockType MrpInstanceDataReal (0x232) of IEC standard:      */
      /* MRP_ROLE, MRP_Version, MRP_DomainLength, MRP_Domain, Padding, MRP-Subblocks */
      /*----------------------------------------*/
      MRP_REC_DB_PUT16_HTON(pData, Version);

      /* DomainName */
      *pData = pRecInstDB->RecInstanceAdjust.LengthDomainName;
      pData += MRP_REC_DATA_LEN_DOMAIN_NAME_FIELD;
      
      MRP_MEMCPY(pData, pRecInstDB->RecInstanceAdjust.DomainName, pRecInstDB->RecInstanceAdjust.LengthDomainName);
      pData += pRecInstDB->RecInstanceAdjust.LengthDomainName;

      for (k = 0; k < pRecInstDB->RecInstanceAdjust.LengthDomainNamePadding; k++) 
      {
        *pData = 0;
        pData++;
      }
      /* moved with version LOW1 of IEC standard, version LOW0 is not supported any longer! */
      /* MRP_REC_DB_PUT16_HTON(pData, Version);   */

      if (Role == MRP_OPER_ROLE_CLIENT) 
      {
        MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_CLIENT_PARAMS);
        MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_LEN_CLIENT_PARAMS);
        MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW0);
        MRP_REC_DB_PUT16_HTON(pData, paramsMRC.LNKdownT_ms);
        MRP_REC_DB_PUT16_HTON(pData, paramsMRC.LNKupT_ms);
        MRP_REC_DB_PUT16_HTON(pData, paramsMRC.LNKNRmax);
      }
#ifdef MRP_CFG_PLUGIN_0_MRM
      else 
      {
        if (Role == MRP_OPER_ROLE_MGR || Role == MRP_OPER_ROLE_AUTO_MGR) 
        {
          MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_MANAGER_PARAMS);
          MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_LEN_MANAGER_PARAMS);
          MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW0);
          MRP_REC_DB_PUT16_HTON(pData, paramsMRM.Prio);
          MRP_REC_DB_PUT16_HTON(pData, paramsMRM.TOPchgT_10ms);
          MRP_REC_DB_PUT16_HTON(pData, paramsMRM.TOPNRmax);
          MRP_REC_DB_PUT16_HTON(pData, paramsMRM.TSTshortTms);
          MRP_REC_DB_PUT16_HTON(pData, paramsMRM.TSTdefaultTms);
          MRP_REC_DB_PUT16_HTON(pData, paramsMRM.TSTNRmax);
          MRP_REC_DB_PUT16_HTON(pData, 0); /* padding */
        
          /* RingStateData are only delivered, if MRM is active */
          MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_RING_STATE_DATA);
          MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_LEN_RING_STATE_DATA);
          MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW0);
          MRP_REC_DB_PUT16_HTON(pData, RingState);
        }
      }        
#else
    LSA_UNUSED_ARG(RingState);
#endif
    }
  }
  LSA_UNUSED_ARG(pData); /* pData is incremented in the previous MACRO, but not used afterwards => creates lint informational (e438). */

  return MRP_RSP_OK;
}


/*===========================================================================
* FUNCTION : mrp_rec_read_port_data_real
*----------------------------------------------------------------------------
* PURPOSE  : parse PDPortMrpDataReal
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_DATA)
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec=internal data base reference
*            *pData=record data array
*            recDataLen=length of pData array
*            portID=edd port id (1..MAX_PORTS)
* OUTPUTS  : *lengthTotal=length of block
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_UINT16 mrp_rec_read_port_data_real (const MRP_REC_DB_TYPE *pDBrec,  
                                                           LSA_UINT8 *pData, 
                                                           LSA_UINT32 recDataLen,
                                                           LSA_UINT32 portID,
                                                           LSA_UINT16 *lengthTotal)
{
#ifdef MRP_CFG_PLUGIN_0_MRM
  MRP_MRM_PARAMS_TYPE paramsMRM;
#endif
  MRP_MRC_PARAMS_TYPE paramsMRC;
  LSA_UINT8 found;
  MRP_OPER_ROLE_ENUM_T operRole;
  LSA_UINT16 RPort_1, RPort_2;
  LSA_UINT8  *pDomainUUID;
  LSA_UINT16 length, i, k, instance;
  LSA_UINT32 idx;
  LSA_UINT16 mrpMaxInstance;
  MRP_REC_DB_INST_SET_TYPE *pRecInstDB;

  length = *lengthTotal = 0;
  mrpMaxInstance = mrp_sys_get_num_of_instances (pDBrec->pChSys);
  pRecInstDB = &pDBrec->pInstData[0].work;  
  k = 0;    
  instance = 0;  

  for (idx = 0; idx < pDBrec->NumOfPossibleRports; idx++) 
  {
    if (pDBrec->pRPortArray[idx] == portID)
      break;
  }  
  if (portID == 0 || idx >= pDBrec->NumOfPossibleRports) 
  {
    MRP_UPPER_TRACE_01(pDBrec->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, ">> read_port_data_real: ERR_PRM_PORTID: port id out of range (port=%u)", portID);

    return MRP_RSP_ERR_PRM_PORTID;
  }
  
  for (i = 0, found = LSA_FALSE; i < mrpMaxInstance && !found; i++)
  {
    pRecInstDB = &pDBrec->pInstData[i].work;        
    /* value check */
    for (k = 0; k < MRP_DEFAULT_VAL_INST_RPORT_CNT; k++) 
    {
      if (pRecInstDB->RecPortAdjust[k].PortID == portID) 
      {
        found = LSA_TRUE;
        instance = i;
        break;
      }
    }  
  }
  if (k >= MRP_DEFAULT_VAL_INST_RPORT_CNT) 
  {
    MRP_UPPER_TRACE_01(pDBrec->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, ">> read_port_data_real: port currently not used as ringport (portID=%u)", portID);
  }

  operRole = MRP_OPER_ROLE_OFF;
  RPort_1 = RPort_2 = 0;
  pDomainUUID = LSA_NULL;
  
#ifdef MRP_CFG_PLUGIN_0_MRM
  mrp_mrm_get_params (pDBrec->pChSys, instance, &paramsMRM);
  
  if (MRP_OPERATION_ON == paramsMRM.Mode) 
  {
    pDomainUUID = paramsMRM.DomainUUID;
    operRole    = paramsMRM.operRole;
    RPort_1 = paramsMRM.RPort_1;
    RPort_2 = paramsMRM.RPort_2;
  }
#endif
  mrp_mrc_get_params (pDBrec->pChSys, instance, &paramsMRC);
  
  if (MRP_OPERATION_ON == paramsMRC.Mode) 
  {
    pDomainUUID = paramsMRC.DomainUUID;
    operRole    = MRP_OPER_ROLE_CLIENT;
    RPort_1 = paramsMRC.RPort_1;
    RPort_2 = paramsMRC.RPort_2;
  }
  
  /* length check */
  if (operRole != MRP_OPER_ROLE_OFF && (RPort_1 == portID || RPort_2 == portID)) 
  {
    (*lengthTotal) += MRP_REC_DATA_BLK_LEN_PORT_DATA_REAL;
    (*lengthTotal) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
    
    if (*lengthTotal > recDataLen) 
    {
      MRP_UPPER_TRACE_02(pDBrec->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> read_port_data_real: ERR_PRM_DATA: out of record_data_length(=%d), needed=%u", recDataLen, *lengthTotal);
      *lengthTotal = 0;
      
      return MRP_RSP_ERR_PRM_DATA;
    }
    
    MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_PORT_DATA_REAL);
    
    length = (LSA_UINT16)(*lengthTotal - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN);
    MRP_REC_DB_PUT16_HTON(pData, length);
    
    if (pRecInstDB->RecPortAdjust[k].head.v.v_new.blockVersionLow == MRP_REC_DATA_BLK_VERS_LOW0)
    {
      MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW0);
      MRP_REC_DB_PUT16_HTON(pData, 0); /* padding */    
    }
    else
    {
      MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW1);
      MRP_REC_DB_PUT16_HTON(pData, (LSA_UINT16)pRecInstDB->RecPortAdjust[k].head.v.v_new.instance); /* MRP_Instance */              
    }
    MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDomainUUID, LSA_NULL));
    
    MRP_MEMCPY(pData, pDomainUUID, sizeof(pRecInstDB->RecPortAdjust->DomainUUID));  
    pData += sizeof(pRecInstDB->RecPortAdjust->DomainUUID);
  }
  LSA_UNUSED_ARG(pData); /* pData is incremented in the previous MACRO, but not used afterwards => creates lint informational (e438). */

  return MRP_RSP_OK;
}

/*===========================================================================
* FUNCTION : mrp_rec_read_ic_port_data_adjust
*----------------------------------------------------------------------------
* PURPOSE  : parse PDPortMrpIcDataAdjust
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_DATA)
*----------------------------------------------------------------------------
* INPUTS   :
*            *pDBrec     = internal data base reference
*            *pData  = record data array
*            readAdmin   = LSA_TRUE: read para DB, LSA_FALSE: read work DB
*            recDataLen  = length of pData array
*            portID      = edd port id (1..MAX_PORTS)
* OUTPUTS  : *lengthTotal= length of block
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_UINT16 mrp_rec_read_ic_port_data_adjust (const MRP_REC_DB_TYPE *pDBrec,
                                                             LSA_UINT8 *pData,
                                                             LSA_UINT8 readAdmin,
                                                             LSA_UINT32 recDataLen,
                                                             LSA_UINT32 portID,
                                                             LSA_UINT16 *lengthTotal)
{
  LSA_UINT16 i, k, mrpInMaxInstance;
  MRP_REC_DB_INST_SET_TYPE *pRecInstDB;
  MRP_IF_DB_TYPE *pDBif;
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBrec->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pDBrec->pChSys);

  if (readAdmin)
  {
    pRecInstDB = &pDBrec->pInstData[0].para;
  }
  else
  {
    pRecInstDB = &pDBrec->pInstData[0].work;
  }

  for (i = 0; i < pDBrec->NumOfPossibleInPorts; i++)
  {
    if (pDBrec->pInPortArray[i] == portID)
      break;
  }

  if (portID == 0 || i >= pDBrec->NumOfPossibleInPorts)
  {
    MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, ">> read_ic_port_data_adjust: ERR_PRM_PORTID: port id out of range (port=%u)", portID);
    return MRP_RSP_ERR_PRM_PORTID;
  }

  pDBif = (MRP_IF_DB_TYPE *)pDBrec->pChSys->pChSysDB->pInterfacePara;
  for (k = 0; k < (mrpInMaxInstance - pDBif->mrpRstpInstances) /* only check inInstances */; k++)
  {
    if (pRecInstDB->pRecIcPortAdjust[k].PortID == portID)
    {
      /* inInstance found because of fitting portID */
      break;
    }
  }

  /* no inInstance found => ERROR */
  if(k >= mrpInMaxInstance)
  {
    MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, ">> read_ic_port_data_adjust: MRP_RSP_ERR_PRM_INDEX: There is no inInstance configured for this port (port=%u)", portID);
    return MRP_RSP_ERR_PRM_INDEX;
  }

  if(pRecInstDB->pRecIcPortAdjust[k].Written)
  {
    (*lengthTotal)  = pRecInstDB->pRecIcPortAdjust[k].head.v.v_new.blockLength;
    (*lengthTotal) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;

    if (*lengthTotal > recDataLen)
    {
      MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> read_ic_port_data_adjust: ERR_PRM_DATA (blockType=0x%x): out of record_data_length(=%d), needed=%u",
        MRP_REC_DATA_BLK_TYPE_IF_DATA_ADJUST, recDataLen, *lengthTotal);
      *lengthTotal = 0;

      return MRP_RSP_ERR_PRM_DATA;
    }

    MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_IC_PORT_DATA_ADJUST);
    MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->pRecIcPortAdjust[k].head.v.v_new.blockLength);
    MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW0);  
    MRP_REC_DB_PUT16_HTON(pData, 0); 

    MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->pRecIcPortAdjust[k].DomainID);
    MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->pRecIcPortAdjust[k].Role);

    /* DomainName */
    *pData = pRecInstDB->pRecIcPortAdjust[k].LengthDomainName;
    pData += MRP_REC_DATA_LEN_DOMAIN_NAME_FIELD;

    MRP_MEMCPY(pData, pRecInstDB->pRecIcPortAdjust[k].DomainName, pRecInstDB->pRecIcPortAdjust[k].LengthDomainName);
    pData += pRecInstDB->pRecIcPortAdjust[k].LengthDomainName;

    for (i = 0; i < pRecInstDB->pRecIcPortAdjust[k].LengthDomainNamePadding; i++)
    {
      *pData = 0;
      pData++;
    }

    if(pRecInstDB->pRecIcPortAdjust[k].mic.Written)
    {
      if (pRecInstDB->pRecIcPortAdjust[k].Role != MRP_IC_PROJ_ROLE_CLIENT)
      {
        MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, ">> mrp_rec_read_ic_port_data_adjust (blockType=0x%x): INCONSISTENCY: Role=%d, but client data available",
            MRP_REC_DATA_BLK_TYPE_IC_PORT_DATA_ADJUST, pRecInstDB->pRecIcPortAdjust[k].Role);
      }

      MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->pRecIcPortAdjust[k].mic.LNKdownT_ms); 
      MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->pRecIcPortAdjust[k].mic.LNKupT_ms);
      MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->pRecIcPortAdjust[k].mic.LNKNRmax);
      MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->pRecIcPortAdjust[k].mic.StartDelay_100ms);
      MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->pRecIcPortAdjust[k].mic.MICPosition);
      MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->pRecIcPortAdjust[k].mic.Padding);
    }

    if(pRecInstDB->pRecIcPortAdjust[k].mim.Written)
    {
      if (pRecInstDB->pRecIcPortAdjust[k].Role != MRP_IC_PROJ_ROLE_MGR)
      {
        MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, ">> mrp_rec_read_ic_port_data_adjust (blockType=0x%x): INCONSISTENCY: Role=%d, but manager data available",
            MRP_REC_DATA_BLK_TYPE_IC_PORT_DATA_ADJUST, pRecInstDB->pRecIcPortAdjust[k].Role);
      }

      MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->pRecIcPortAdjust[k].mim.TOPchgT_10ms);
      MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->pRecIcPortAdjust[k].mim.TOPNRmax);
      MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->pRecIcPortAdjust[k].mim.LinkStatusChangeT_ms);
      MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->pRecIcPortAdjust[k].mim.LinkStatusNRmax);
      MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->pRecIcPortAdjust[k].mim.StartDelay_100ms);
      MRP_REC_DB_PUT16_HTON(pData, pRecInstDB->pRecIcPortAdjust[k].mim.Padding);
    }
  }

  LSA_UNUSED_ARG(pData); /* pData is incremented in the previous MACRO, but not used afterwards => creates lint informational (e438). */

  return MRP_RSP_OK;
}

/*===========================================================================
* FUNCTION : mrp_rec_read_ic_port_data_check
*----------------------------------------------------------------------------
* PURPOSE  : parse PDPortMrpIcDataCheck
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_DATA)
*----------------------------------------------------------------------------
* INPUTS   :
*            *pDBrec     = internal data base reference
*            *pData  = record data array
*            readAdmin   = LSA_TRUE: read para DB, LSA_FALSE: read work DB
*            recDataLen  = length of pData array
*            portID      = edd port id (1..MAX_PORTS)
* OUTPUTS  : *lengthTotal= length of block
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_UINT16 mrp_rec_read_ic_port_data_check (const MRP_REC_DB_TYPE *pDBrec,
                                                             LSA_UINT8 *pData,
                                                             LSA_UINT8 readAdmin,
                                                             LSA_UINT32 recDataLen,
                                                             LSA_UINT16 portID,
                                                             LSA_UINT16 *lengthTotal)
{
  LSA_UINT16 length, mrpInMaxInstance;
  LSA_UINT16 idx, k;
  MRP_REC_DB_INST_SET_TYPE *pRecInstDB;
  MRP_IF_DB_TYPE *pDBif;

  mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pDBrec->pChSys);

  if (readAdmin)
  {
    pRecInstDB = &pDBrec->pInstData[0].para;
  }
  else
  {
    pRecInstDB = &pDBrec->pInstData[0].work;
  }

  length = *lengthTotal = 0;

  for (idx = 0; idx < pDBrec->NumOfPossibleInPorts; idx++)
  {
    if (pDBrec->pInPortArray[idx] == portID)
      break;
  }

  if (portID == 0 || idx >= pDBrec->NumOfPossibleInPorts)
  {
    MRP_UPPER_TRACE_01(pDBrec->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, ">> read_ic_port_data_check: ERR_PRM_PORTID: port id out of range (port=%u)", portID);
    return MRP_RSP_ERR_PRM_PORTID;
  }

  pDBif = (MRP_IF_DB_TYPE *)pDBrec->pChSys->pChSysDB->pInterfacePara;
  for (k = 0; k < (mrpInMaxInstance - pDBif->mrpRstpInstances) /* only check inInstances */; k++)
  {
    if (pRecInstDB->pRecIcPortAdjust[k].PortID == portID)
    {
      /* inInstance found because of fitting portID */
      break;
    }
  }

  /* no inInstance found => ERROR */
  if(k >= mrpInMaxInstance)
  {
    MRP_UPPER_TRACE_01(pDBrec->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, ">> read_ic_port_data_check: MRP_RSP_ERR_PRM_INDEX: There is no inInstance configured for this port (port=%u)", portID);
    return MRP_RSP_ERR_PRM_INDEX;
  }
  else
  {
    (*lengthTotal) += MRP_REC_DATA_BLK_LEN_IC_PORT_DATA_CHECK;
    (*lengthTotal) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;

    if (*lengthTotal > recDataLen)
    {
      MRP_UPPER_TRACE_02(pDBrec->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> read_ic_port_data_check: ERR_PRM_DATA: out of record_data_length(=%d), needed=%u", recDataLen, *lengthTotal);
      *lengthTotal = 0;

      return MRP_RSP_ERR_PRM_DATA;
    }

    if (pRecInstDB->pRecIcPortCheck[k].Written)
    {
      MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_IC_PORT_DATA_CHECK);

      length = (LSA_UINT16)(*lengthTotal - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN);

      MRP_REC_DB_PUT16_HTON(pData, length);

      MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW0);  
      MRP_REC_DB_PUT16_HTON(pData, 0); /* padding */

      MRP_REC_DB_PUT32_HTON(pData, pRecInstDB->pRecIcPortCheck[k].Check);
    }
  }

  LSA_UNUSED_ARG(pData); /* pData is incremented in the previous MACRO, but not used afterwards => creates lint informational (e438). */

  return MRP_RSP_OK;
}


/*===========================================================================
* FUNCTION : mrp_rec_read_ic_port_data_real
*----------------------------------------------------------------------------
* PURPOSE  : parse PDPortMrpIcDataReal
*----------------------------------------------------------------------------
* RETURNS  : RQB-Response (MRP_RSP_OK, MRP_RSP_ERR_PARAM, MRP_RSP_ERR_PRM_DATA)
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec=internal data base reference
*            *pData=record data array
*            recDataLen=length of pData array
*            portID=edd port id (1..MAX_PORTS)
* OUTPUTS  : *lengthTotal=length of block
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_UINT16 mrp_rec_read_ic_port_data_real (const MRP_REC_DB_TYPE *pDBrec,
                                                           LSA_UINT8 *pData,
                                                           LSA_UINT32 recDataLen,
                                                           LSA_UINT16 portID,
                                                           LSA_UINT16 *lengthTotal)
{
  LSA_UINT16 k, i;
  LSA_UINT16 Version = MRP_PACKET_VAL_VERSION;
  LSA_UINT16 length;
  MRP_REC_DB_INST_SET_TYPE *pRecInstDB;

  MRP_IN_MIC_PARAMS_TYPE paramsMIC;
  MRP_IN_MIM_PARAMS_TYPE paramsMIM;
  LSA_UINT8 InterconnectionOpen;
  LSA_UINT16 InterconnectionState = MRP_REC_IC_PORT_DATA_IC_STATE_OPEN;
  LSA_UINT16 idx;
  LSA_UINT16 Role = MRP_IC_PROJ_ROLE_OFF;
  LSA_UINT16 DomainID = 0x0000;   /* No MRP Interconnection Domain defined */
  LSA_UINT16 mrpInMaxInstance, inInstance;
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBrec->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  *lengthTotal = 0;
  mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pDBrec->pChSys);
  pRecInstDB = &pDBrec->pInstData[0].work;

  for (idx = 0; idx < pDBrec->NumOfPossibleInPorts; idx++)
  {
    if (pDBrec->pInPortArray[idx] == portID)
      break;
  }

  if (portID == 0 || idx >= pDBrec->NumOfPossibleInPorts)
  {
    MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, ">> read_ic_port_data_real: ERR_PRM_PORTID: port id out of range (port=%u)", portID);
    return MRP_RSP_ERR_PRM_PORTID;
  }

  for (i = 0; i < mrpInMaxInstance; i++)
  {
    if (pRecInstDB->pRecIcPortAdjust[i].PortID == portID)
      break;
  }

  if (i >= mrpInMaxInstance) /* If this is true, (i = mrpInMaxInstance) we did not find an entry for this port, in the for loop above. */
  {
    MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, ">> read_ic_port_data_real: ERR_PRM_PORTID: unknown port id (port=%u)", portID);
    return MRP_RSP_ERR_PRM_PORTID;
  }

  inInstance = mrp_in_sys_map_iport_to_inInstance(pDBrec->pChSys, portID);
  
  if (inInstance == MRP_IN_INSTANCE_NONE)
  {
    MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, ">> read_ic_port_data_check: ERR_PRM_PORTID: no inInstance found for this port (port=%u)", portID);
    return MRP_RSP_ERR_PRM_PORTID;
  }

  mrp_in_mim_get_params (pDBrec->pChSys, inInstance, &paramsMIM);  

  if (MRP_IN_OPERATION_ON == paramsMIM.runState)  
  {
    DomainID = paramsMIM.InID;
    Role = MRP_IC_PROJ_ROLE_MGR;

    mrp_in_mim_get_state(pDBrec->pChSys, inInstance, &InterconnectionOpen);
    
    if (!InterconnectionOpen)
    {
      InterconnectionState = MRP_REC_IC_PORT_DATA_IC_STATE_CLOSED;
    }
  }

  mrp_in_mic_get_params (pDBrec->pChSys, inInstance, &paramsMIC);

  if(MRP_IN_OPERATION_ON == paramsMIC.runState)
  {
    if (Role == MRP_IC_PROJ_ROLE_MGR)
    {
      MRP_UPPER_TRACE_00(traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, ">> read_ic_port_data_real: ERR_PRM_DATA: MIC and MIM both with role MRP_OPERATION_ON");

      return MRP_RSP_ERR_PRM_DATA;
    }

    DomainID = paramsMIC.InID;
    Role = MRP_IC_PROJ_ROLE_CLIENT;
  }

  (*lengthTotal) += MRP_REC_DATA_BLK_LEN_IC_PORT_DATA_REAL_MIN;
  (*lengthTotal) += pRecInstDB->pRecIcPortAdjust[i].LengthDomainName + pRecInstDB->pRecIcPortAdjust[i].LengthDomainNamePadding;
  (*lengthTotal) += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;

  if (Role == MRP_IC_PROJ_ROLE_CLIENT)
  {
    (*lengthTotal) += MRP_REC_DATA_BLK_LEN_IC_MIC_PARAMS;
  }
  else if (Role == MRP_IC_PROJ_ROLE_MGR)
  {
    (*lengthTotal) += MRP_REC_DATA_BLK_LEN_IC_MIM_PARAMS;
    (*lengthTotal) += MRP_REC_DATA_BLK_LEN_IC_STATE_DATA;
  }

  if (*lengthTotal > recDataLen)
  {
    MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> read_ic_port_data_real: ERR_PRM_DATA: out of record_data_length(=%d), needed=%u", recDataLen, *lengthTotal);
    *lengthTotal = 0;

    return MRP_RSP_ERR_PRM_DATA;
  }

  MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_TYPE_IC_PORT_DATA_REAL);

  length = (LSA_UINT16)(*lengthTotal - MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN);

  MRP_REC_DB_PUT16_HTON(pData, length);
  MRP_REC_DB_PUT16_HTON(pData, MRP_REC_DATA_BLK_VERS_HIGH1_LOW0);
  MRP_REC_DB_PUT16_HTON(pData, 0); /* padding */

  MRP_REC_DB_PUT16_HTON(pData, DomainID); 
  MRP_REC_DB_PUT16_HTON(pData, Role);
  MRP_REC_DB_PUT16_HTON(pData, Version);

  *pData = pRecInstDB->pRecIcPortAdjust[i].LengthDomainName;
  pData += MRP_REC_DATA_LEN_DOMAIN_NAME_FIELD;

  MRP_MEMCPY(pData, pRecInstDB->pRecIcPortAdjust[i].DomainName, pRecInstDB->pRecIcPortAdjust[i].LengthDomainName);
  pData += pRecInstDB->pRecIcPortAdjust[i].LengthDomainName;

  for (k = 0; k < pRecInstDB->pRecIcPortAdjust[i].LengthDomainNamePadding; k++)
  {
    *pData = 0;
    pData++;
  }

  if (Role == MRP_IC_PROJ_ROLE_CLIENT)
  {
    MRP_REC_DB_PUT16_HTON(pData, paramsMIC.MRPIC_LNKdownT_ms);
    MRP_REC_DB_PUT16_HTON(pData, paramsMIC.MRPIC_LNKupT_ms);
    MRP_REC_DB_PUT16_HTON(pData, paramsMIC.MRPIC_LNKNRmax);
    MRP_REC_DB_PUT16_HTON(pData, paramsMIC.MRPIC_StartDelay_100ms);
    MRP_REC_DB_PUT16_HTON(pData, paramsMIC.MRPIC_MICPosition);
    MRP_REC_DB_PUT16_HTON(pData, 0);
  }
  else if (Role == MRP_IC_PROJ_ROLE_MGR)
  {
    MRP_REC_DB_PUT16_HTON(pData, paramsMIM.MRPIC_TOPchgT_10ms);
    MRP_REC_DB_PUT16_HTON(pData, paramsMIM.MRPIC_TOPNRmax);
    MRP_REC_DB_PUT16_HTON(pData, paramsMIM.MRPIC_LinkStatusChangeT_ms);
    MRP_REC_DB_PUT16_HTON(pData, paramsMIM.MRPIC_LinkStatusNRmax);
    MRP_REC_DB_PUT16_HTON(pData, paramsMIM.MRPIC_StartDelay_100ms);
    MRP_REC_DB_PUT16_HTON(pData, 0);

    MRP_REC_DB_PUT16_HTON(pData, InterconnectionState); 
  }

  LSA_UNUSED_ARG(pData); /* pData is incremented in the previous MACRO, but not used afterwards => creates lint informational (e438). */

  return MRP_RSP_OK;
}

/*===========================================================================
* FUNCTION : mrp_rec_set_if_para_defaults
*----------------------------------------------------------------------------
* PURPOSE  : set interface related para record-database-set to defaults
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec   = internal data base reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_rec_set_if_para_defaults (MRP_REC_DB_TYPE *pDBrec)
{
  MRP_REC_DB_IF_SET_TYPE *pRecIfPara;

  pRecIfPara = &pDBrec->para;

  MRP_UPPER_TRACE_00(pDBrec->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_set_if_para_defaults entered");

  MRP_MEMSET(&pRecIfPara->RecIfAdjustHead, 0, sizeof(pRecIfPara->RecIfAdjustHead));
  MRP_MEMSET(&pRecIfPara->RecIfCheckHead, 0, sizeof(pRecIfPara->RecIfCheckHead));

  pRecIfPara->RecIfAdjustHead.head.blockType                 = MRP_REC_DATA_BLK_TYPE_IF_DATA_ADJUST;
  pRecIfPara->RecIfAdjustHead.head.blockLength               = MRP_REC_DATA_BLK_LEN_IF_DATA_ADJ_VERS0_MIN;
  pRecIfPara->RecIfAdjustHead.head.blockVersionHigh          = MRP_REC_DATA_BLK_VERS_HIGH1;
  pRecIfPara->RecIfAdjustHead.head.blockVersionLow           = MRP_REC_DATA_BLK_VERS_LOW1;
  pRecIfPara->RecIfAdjustHead.head.v.v_new.padding           = 0;
  pRecIfPara->RecIfAdjustHead.head.v.v_new.numOfMrpInstances = 1;

  pRecIfPara->RecIfCheckHead.head.blockType                 = MRP_REC_DATA_BLK_TYPE_IF_DATA_CHECK;
  pRecIfPara->RecIfCheckHead.head.blockLength               = MRP_REC_DATA_BLK_LEN_IF_DATA_CHECK;
  pRecIfPara->RecIfCheckHead.head.blockVersionHigh          = MRP_REC_DATA_BLK_VERS_HIGH1;
  pRecIfPara->RecIfCheckHead.head.blockVersionLow           = MRP_REC_DATA_BLK_VERS_LOW1;
  pRecIfPara->RecIfCheckHead.head.v.v_new.padding           = 0;
  pRecIfPara->RecIfCheckHead.head.v.v_new.numOfMrpInstances = 1;
}


/*===========================================================================
* FUNCTION : mrp_rec_set_inst_para_defaults
*----------------------------------------------------------------------------
* PURPOSE  : set instance related para record-database-set to defaults
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec   = internal data base reference
*             instance = affected MRP/HSR instance
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_rec_set_inst_para_defaults (const MRP_REC_DB_TYPE *pDBrec, LSA_UINT16 instance, MRP_REC_ENUM_DEFAULT_TYPE setting)
{
  LSA_UINT16 idx, mrpInMaxInstance;
  LSA_UINT8  *domainNameHelper, domainLength;
  MRP_REC_DB_INST_SET_TYPE *pRecInstPara;
#ifdef MRP_CFG_PLUGIN_2_HSR
  LSA_UINT16 i;
  LSA_UINT16 numOfPorts;
#endif

  mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pDBrec->pChSys);
  domainNameHelper = &domainLength;
  pRecInstPara = &pDBrec->pInstData[instance].para;

  MRP_MEMSET(&pRecInstPara->RecInstanceCheck, 0, sizeof(pRecInstPara->RecInstanceCheck));
  MRP_MEMSET(pRecInstPara->RecInstanceCheck.DomainUUID, 0xFF, sizeof(pRecInstPara->RecInstanceCheck.DomainUUID));
  pRecInstPara->RecInstanceCheck.head.v.v_new.blockType        = MRP_REC_DATA_BLK_TYPE_INST_DATA_CHECK;
  pRecInstPara->RecInstanceCheck.head.v.v_new.blockLength      = MRP_REC_DATA_BLK_LEN_IF_DATA_CHECK;
  pRecInstPara->RecInstanceCheck.head.v.v_new.blockVersionHigh = MRP_REC_DATA_BLK_VERS_HIGH1;
  pRecInstPara->RecInstanceCheck.head.v.v_new.blockVersionLow  = MRP_REC_DATA_BLK_VERS_LOW0;
  pRecInstPara->RecInstanceCheck.head.v.v_new.padding          = 0;
  pRecInstPara->RecInstanceCheck.head.v.v_new.instance         = (LSA_UINT8)instance;
  
  MRP_MEMCPY(&pRecInstPara->RecInstanceUuidCheck, &pRecInstPara->RecInstanceCheck, sizeof(pRecInstPara->RecInstanceCheck));
  
  MRP_MEMSET(&pRecInstPara->RecInstanceAdjust, 0, sizeof(pRecInstPara->RecInstanceAdjust));
  MRP_MEMSET(pRecInstPara->RecInstanceAdjust.DomainUUID, 0xFF, sizeof(pRecInstPara->RecInstanceAdjust.DomainUUID));
  pRecInstPara->RecInstanceAdjust.head.v.v_new.blockType        = MRP_REC_DATA_BLK_TYPE_INST_DATA_ADJUST;
  pRecInstPara->RecInstanceAdjust.head.v.v_new.blockLength      = MRP_REC_DATA_BLK_LEN_IF_DATA_ADJ_VERS0_MIN;
  pRecInstPara->RecInstanceAdjust.head.v.v_new.blockVersionHigh = MRP_REC_DATA_BLK_VERS_HIGH1;
  pRecInstPara->RecInstanceAdjust.head.v.v_new.blockVersionLow  = MRP_REC_DATA_BLK_VERS_LOW0;
  pRecInstPara->RecInstanceAdjust.head.v.v_new.padding          = 0;
  pRecInstPara->RecInstanceAdjust.head.v.v_new.instance         = (LSA_UINT8)instance;

  pRecInstPara->RecInstanceAdjust.mgr.Prio = MRP_DEFAULT_VAL_MRM_PRIO;

  if (setting == MRP_REC_ENUM_DEFAULT_FROM_IF_DETAILS && !instance) 
  {
    MRP_UPPER_TRACE_01(pDBrec->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_set_inst_para_defaults[inst=%d]: setting = MRP_REC_ENUM_DEFAULT_FROM_IF_DETAILS", instance);
    pRecInstPara->RPort_1                    = pDBrec->DefaultRPort_1;
    pRecInstPara->RPort_2                    = pDBrec->DefaultRPort_2;
    pRecInstPara->RecInstanceAdjust.Role     = pDBrec->DefaultRoleInst0;
    pRecInstPara->adminRole                  = mrp_rec_map_proj_role_to_admin_role (pDBrec->pChSys, LSA_TRUE, (MRP_PROJ_ROLE_ENUM_T)pDBrec->DefaultRoleInst0);
    
    if (pRecInstPara->adminRole == MRP_ADMIN_ROLE_AUTO_MGR)
      pRecInstPara->RecInstanceAdjust.mgr.Prio = pDBrec->DefaultPrio;
  }
  else 
  {
    MRP_UPPER_TRACE_01(pDBrec->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_set_inst_para_defaults[inst=%d]: setting = MRP_REC_ENUM_DEFAULT_FROM_STD (=MRP_OFF)", instance);
    pRecInstPara->RPort_1                    = 0;
    pRecInstPara->RPort_2                    = 0;
    pRecInstPara->RecInstanceAdjust.Role     = MRP_PROJ_ROLE_OFF;
    pRecInstPara->adminRole                  = MRP_ADMIN_ROLE_OFF;
  }
  *domainNameHelper = sizeof(MRP_DEFAULT_VAL_MRP_DEFAULT_DOMAIN_NAME) - 1;
  MRP_FATAL1(MRP_RSP_OK == mrp_rec_parse_helper_domain_name (pDBrec, &domainNameHelper, &pRecInstPara->RecInstanceAdjust.LengthDomainName,
                                                            &pRecInstPara->RecInstanceAdjust.LengthDomainNamePadding, LSA_NULL));
  MRP_MEMCPY(pRecInstPara->RecInstanceAdjust.DomainName, (LSA_VOID_PTR_TYPE)MRP_DEFAULT_VAL_MRP_DEFAULT_DOMAIN_NAME, sizeof(MRP_DEFAULT_VAL_MRP_DEFAULT_DOMAIN_NAME)-1);
  
  pRecInstPara->RecInstanceAdjust.mgr.TOPchgT_10ms  = MRP_DEFAULT_VAL_MRM_TOP_CHG_10MS_T_CNT;
  pRecInstPara->RecInstanceAdjust.mgr.TOPNRmax      = MRP_DEFAULT_VAL_MRM_TOP_NR_MAX;
  pRecInstPara->RecInstanceAdjust.mgr.TSTshortTms   = MRP_DEFAULT_VAL_MRM_TST_SHORT_T_MS;
  pRecInstPara->RecInstanceAdjust.mgr.TSTdefaultTms = MRP_DEFAULT_VAL_MRM_TST_DEFAULT_T_MS;
  pRecInstPara->RecInstanceAdjust.mgr.TSTNRmax      = MRP_DEFAULT_VAL_MRM_TST_NR_MAX;
  pRecInstPara->RecInstanceAdjust.client.LNKdownT_ms= MRP_DEFAULT_VAL_MRC_LNK_DOWN_T_MS;
  pRecInstPara->RecInstanceAdjust.client.LNKupT_ms  = MRP_DEFAULT_VAL_MRC_LNK_UP_T_MS;
  pRecInstPara->RecInstanceAdjust.client.LNKNRmax   = MRP_DEFAULT_VAL_MRC_LNK_NR_MAX;

  for (idx = 0; idx < MRP_DEFAULT_VAL_INST_RPORT_CNT; idx++) 
  {
    pRecInstPara->RecPortAdjust[idx].Written                       = LSA_FALSE;
    
    pRecInstPara->RecPortAdjust[idx].head.v.v_new.blockType        = MRP_REC_DATA_BLK_TYPE_PORT_DATA_ADJUST;
    pRecInstPara->RecPortAdjust[idx].head.v.v_new.blockLength      = MRP_REC_DATA_BLK_LEN_PORT_DATA_ADJUST;
    pRecInstPara->RecPortAdjust[idx].head.v.v_new.blockVersionHigh = MRP_REC_DATA_BLK_VERS_HIGH1;
    pRecInstPara->RecPortAdjust[idx].head.v.v_new.blockVersionLow  = MRP_REC_DATA_BLK_VERS_LOW1;
    pRecInstPara->RecPortAdjust[idx].head.v.v_new.padding          = 0;
    pRecInstPara->RecPortAdjust[idx].head.v.v_new.instance         = (LSA_UINT8)instance;
    MRP_MEMSET(pRecInstPara->RecPortAdjust[idx].DomainUUID, 0xFF, sizeof(pRecInstPara->RecPortAdjust->DomainUUID));
        
    /* pDBrec->RecPortAdjust[idx].PortID; one time init in mrp_rec_init */
    if (setting == MRP_REC_ENUM_DEFAULT_FROM_IF_DETAILS && !instance) 
    {
      if (!idx)
        pRecInstPara->RecPortAdjust[idx].PortID = pDBrec->DefaultRPort_1;
      else
        pRecInstPara->RecPortAdjust[idx].PortID = pDBrec->DefaultRPort_2;
    }
    else 
    {
      pRecInstPara->RecPortAdjust[idx].PortID   = 0;
    }
  }

  /* MRP IN data just exists for MRP instance 0 and if mrpInMaxInstance != 0*/
  if(mrpInMaxInstance && !instance){

    /* clear all data */
    MRP_MEMSET(pRecInstPara->pRecIcPortCheck, 0, sizeof(MRP_IC_PORT_DATA_CHECK_TYPE) * mrpInMaxInstance);
    MRP_MEMSET(pRecInstPara->pRecIcPortAdjust, 0, sizeof(MRP_IC_PORT_DATA_ADJUST_TYPE) * mrpInMaxInstance);
    for (idx = 0; idx < mrpInMaxInstance; idx++)
    {
      /* initialize certain data */
      pRecInstPara->pRecIcPortCheck[idx].head.v.v_new.blockType        = MRP_REC_DATA_BLK_TYPE_IC_PORT_DATA_CHECK;
      pRecInstPara->pRecIcPortCheck[idx].head.v.v_new.blockLength      = MRP_REC_DATA_BLK_LEN_IC_PORT_DATA_CHECK;
      pRecInstPara->pRecIcPortCheck[idx].head.v.v_new.blockVersionHigh = MRP_REC_DATA_BLK_VERS_HIGH1;
      pRecInstPara->pRecIcPortCheck[idx].head.v.v_new.blockVersionLow  = MRP_REC_DATA_BLK_VERS_LOW0;
      pRecInstPara->pRecIcPortCheck[idx].head.v.v_new.padding          = 0;
      pRecInstPara->pRecIcPortCheck[idx].head.v.v_new.instance         = (LSA_UINT8)instance;

      pRecInstPara->pRecIcPortAdjust[idx].head.v.v_new.blockType        = MRP_REC_DATA_BLK_TYPE_IC_PORT_DATA_ADJUST;
      pRecInstPara->pRecIcPortAdjust[idx].head.v.v_new.blockLength      = MRP_REC_DATA_BLK_LEN_IC_PORT_DATA_ADJUST_MIN;
      pRecInstPara->pRecIcPortAdjust[idx].head.v.v_new.blockVersionHigh = MRP_REC_DATA_BLK_VERS_HIGH1;
      pRecInstPara->pRecIcPortAdjust[idx].head.v.v_new.blockVersionLow  = MRP_REC_DATA_BLK_VERS_LOW0;
      pRecInstPara->pRecIcPortAdjust[idx].head.v.v_new.padding          = 0;
      pRecInstPara->pRecIcPortAdjust[idx].head.v.v_new.instance         = (LSA_UINT8)instance;

      pRecInstPara->pRecIcPortAdjust[idx].Role                          = MRP_IC_PROJ_ROLE_OFF;
      pRecInstPara->pRecIcPortAdjust[idx].PortID                        = 0;

      pRecInstPara->pRecIcPortAdjust[idx].DomainID                      = 0x0000;    /* No MRP Interconnection Domain defined */
      *domainNameHelper                                           = sizeof(MRP_DEFAULT_VAL_MRP_IC_DEFAULT_DOMAIN_NAME) - 1;

      MRP_FATAL1(MRP_RSP_OK == mrp_rec_parse_helper_domain_name(pDBrec, &domainNameHelper, &pRecInstPara->pRecIcPortAdjust[idx].LengthDomainName,
          &pRecInstPara->pRecIcPortAdjust[idx].LengthDomainNamePadding, LSA_NULL));

      MRP_MEMCPY(pRecInstPara->pRecIcPortAdjust[idx].DomainName, (LSA_VOID_PTR_TYPE)MRP_DEFAULT_VAL_MRP_IC_DEFAULT_DOMAIN_NAME, sizeof(MRP_DEFAULT_VAL_MRP_IC_DEFAULT_DOMAIN_NAME) - 1);

      pRecInstPara->pRecIcPortAdjust[idx].mim.TOPchgT_10ms         = MRP_IC_DEFAULT_VAL_MIM_TOP_CHG_10MS_T_CNT;
      pRecInstPara->pRecIcPortAdjust[idx].mim.TOPNRmax             = MRP_IC_DEFAULT_VAL_MIM_TOP_NR_MAX;
      pRecInstPara->pRecIcPortAdjust[idx].mim.LinkStatusChangeT_ms = MRP_IC_DEFAULT_VAL_MIM_LNK_STATUS_CHANGE_T_MS;
      pRecInstPara->pRecIcPortAdjust[idx].mim.LinkStatusNRmax      = MRP_IC_DEFAULT_VAL_MIM_LNK_STATUS_NR_MAX;
      pRecInstPara->pRecIcPortAdjust[idx].mim.StartDelay_100ms     = MRP_IC_DEFAULT_VAL_START_DELAY_100MS_T_CNT;

      pRecInstPara->pRecIcPortAdjust[idx].mic.LNKdownT_ms          = MRP_IC_DEFAULT_VAL_MIC_LNK_DOWN_T_MS;
      pRecInstPara->pRecIcPortAdjust[idx].mic.LNKupT_ms            = MRP_IC_DEFAULT_VAL_MIC_LNK_UP_T_MS;
      pRecInstPara->pRecIcPortAdjust[idx].mic.LNKNRmax             = MRP_IC_DEFAULT_VAL_MIC_LNK_NR_MAX;
      pRecInstPara->pRecIcPortAdjust[idx].mic.StartDelay_100ms     = MRP_IC_DEFAULT_VAL_START_DELAY_100MS_T_CNT;
    }
  }

#ifdef MRP_CFG_PLUGIN_2_HSR
  numOfPorts = mrp_sys_get_num_of_ports(pDBrec->pChSys);

  MRP_MEMSET(&pRecInstPara->RecPrivHsrAdjust, 0, sizeof(pRecInstPara->RecPrivHsrAdjust));

  for (i = 0; i < numOfPorts; i++)
  {
    pRecInstPara->RecPrivStByAdjust.pPortID[i]        = 0;
  }
  pRecInstPara->RecPrivStByAdjust.Written              = 0; 
  pRecInstPara->RecPrivStByAdjust.LengthConName        = 0;
  pRecInstPara->RecPrivStByAdjust.LengthConNamePadding = 0;
  pRecInstPara->RecPrivStByAdjust.PortCnt              = 0;
  pRecInstPara->RecPrivStByAdjust.PortCntPadding       = 0;
  pRecInstPara->RecPrivStByAdjust.Capability           = 0;
  pRecInstPara->RecPrivStByAdjust.WaitForPartner       = 0;

  MRP_MEMSET(pRecInstPara->RecPrivStByAdjust.ConName, 0, sizeof(pRecInstPara->RecPrivStByAdjust.ConName));
#endif
#ifdef MRP_CFG_PLUGIN_1_MRM_EXT
  pRecInstPara->RecPrivDataAdjust.Written              = LSA_FALSE;
  pRecInstPara->RecPrivDataAdjust.MONdefaultT_ms       = MRP_DEFAULT_VAL_MRM_MON_DEFAULT_T_MS;
  pRecInstPara->RecPrivDataAdjust.MONNRmax             = MRP_DEFAULT_VAL_MRM_MON_NR_MAX;
#endif
}

/*===========================================================================
* FUNCTION : mrp_rec_set_if_para2work
*----------------------------------------------------------------------------
* PURPOSE  : take interface para record-database-set 
*            to work record-database-set
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec   = internal data base reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_rec_set_if_para2work (MRP_REC_DB_TYPE *pDBrec)
{
  MRP_REC_DB_IF_SET_TYPE *pRecIfSaveWork, *pRecIfPara, *pRecIfWork;

  pRecIfSaveWork = &pDBrec->saveWork;
  pRecIfPara     = &pDBrec->para;
  pRecIfWork     = &pDBrec->work;

  MRP_UPPER_TRACE_00(pDBrec->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_set_if_para2work entered");

  /* save current working database, probably it will be needed again */
  /* in case of morphing */
  MRP_MEMCPY(&pRecIfSaveWork->RecIfAdjustHead, &pRecIfWork->RecIfAdjustHead, sizeof(pRecIfSaveWork->RecIfAdjustHead));
  MRP_MEMCPY(&pRecIfSaveWork->RecIfCheckHead, &pRecIfWork->RecIfCheckHead, sizeof(pRecIfSaveWork->RecIfCheckHead));

  /* activate provisioned data */
  MRP_MEMCPY(&pRecIfWork->RecIfAdjustHead, &pRecIfPara->RecIfAdjustHead, sizeof(pRecIfWork->RecIfAdjustHead));
  MRP_MEMCPY(&pRecIfWork->RecIfCheckHead, &pRecIfPara->RecIfCheckHead, sizeof(pRecIfWork->RecIfCheckHead));
}

/*===========================================================================
* FUNCTION : mrp_rec_set_inst_para2work
*----------------------------------------------------------------------------
* PURPOSE  : take para record-database-set to work record-database-set
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : *pDBrec=internal data base reference
*             instance = affected MRP/HSR instance
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_rec_set_inst_para2work (MRP_REC_DB_TYPE *pDBrec, LSA_UINT16 instance)
{
  LSA_UINT32 idx;
  MRP_REC_DB_INST_SET_TYPE *pRecInstSaveWork, *pRecInstPara, *pRecInstWork;
  MRP_REC_DB_IF_SET_TYPE *pRecIfWork;
  LSA_UINT16 i, mrpInMaxInstance;
#ifdef MRP_CFG_PLUGIN_2_HSR
  LSA_UINT16 numOfPorts;

  numOfPorts = mrp_sys_get_num_of_ports(pDBrec->pChSys);
#endif

  mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pDBrec->pChSys);

  pRecInstSaveWork  = &pDBrec->pInstData[instance].saveWork;
  pRecInstPara      = &pDBrec->pInstData[instance].para;
  pRecInstWork      = &pDBrec->pInstData[instance].work;
  pRecIfWork        = &pDBrec->work;

  MRP_UPPER_TRACE_01(pDBrec->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_set_inst_para2work[inst=%d] entered", instance);

  /* save current working database, probably it will be needed again */
  /* in case of morphing */
  pRecInstSaveWork->RPort_1   = pRecInstWork->RPort_1;
  pRecInstSaveWork->RPort_2   = pRecInstWork->RPort_2;
  pRecInstSaveWork->adminRole = pRecInstWork->adminRole;
  
  MRP_MEMCPY(&pRecInstSaveWork->RecInstanceAdjust, &pRecInstWork->RecInstanceAdjust, sizeof(pRecInstSaveWork->RecInstanceAdjust));

  for (idx = 0; idx < MRP_DEFAULT_VAL_INST_RPORT_CNT; idx++) 
  {
    MRP_MEMCPY(&pRecInstSaveWork->RecPortAdjust[idx], &pRecInstWork->RecPortAdjust[idx], sizeof(MRP_PORT_DATA_ADJUST_TYPE));
  }
  MRP_MEMCPY(&pRecInstSaveWork->RecInstanceCheck, &pRecInstWork->RecInstanceCheck, sizeof(pRecInstSaveWork->RecInstanceCheck));
  MRP_MEMCPY(&pRecInstSaveWork->RecInstanceUuidCheck, &pRecInstWork->RecInstanceCheck, sizeof(pRecInstSaveWork->RecInstanceUuidCheck));
  
#ifdef MRP_CFG_PLUGIN_1_MRM_EXT
  MRP_MEMCPY(&pRecInstSaveWork->RecPrivDataAdjust, &pRecInstWork->RecPrivDataAdjust, sizeof(pRecInstSaveWork->RecPrivDataAdjust));
#endif

  if (mrpInMaxInstance && !instance)
  {
    MRP_MEMCPY(pRecInstSaveWork->pRecIcPortAdjust, pRecInstWork->pRecIcPortAdjust, ((LSA_UINT16)sizeof(MRP_IC_PORT_DATA_ADJUST_TYPE) * mrpInMaxInstance));
    MRP_MEMCPY(pRecInstSaveWork->pRecIcPortCheck, pRecInstWork->pRecIcPortCheck, ((LSA_UINT16)sizeof(MRP_IC_PORT_DATA_CHECK_TYPE) * mrpInMaxInstance));
  }

#ifdef MRP_CFG_PLUGIN_2_HSR
  MRP_MEMCPY(&pRecInstSaveWork->RecPrivHsrAdjust, &pRecInstWork->RecPrivHsrAdjust, sizeof(pRecInstSaveWork->RecPrivHsrAdjust));

  for (i = 0; i < numOfPorts; i++)
  {
    pRecInstSaveWork->RecPrivStByAdjust.pPortID[i] = pRecInstWork->RecPrivStByAdjust.pPortID[i];
  }
  pRecInstSaveWork->RecPrivStByAdjust.Written              = pRecInstWork->RecPrivStByAdjust.Written;
  MRP_MEMCPY(pRecInstSaveWork->RecPrivStByAdjust.ConName, pRecInstWork->RecPrivStByAdjust.ConName, sizeof(pRecInstSaveWork->RecPrivStByAdjust.ConName));
  pRecInstSaveWork->RecPrivStByAdjust.LengthConName        = pRecInstWork->RecPrivStByAdjust.LengthConName;
  pRecInstSaveWork->RecPrivStByAdjust.LengthConNamePadding = pRecInstWork->RecPrivStByAdjust.LengthConNamePadding;
  pRecInstSaveWork->RecPrivStByAdjust.PortCnt              = pRecInstWork->RecPrivStByAdjust.PortCnt;
  pRecInstSaveWork->RecPrivStByAdjust.PortCntPadding       = pRecInstWork->RecPrivStByAdjust.PortCntPadding;
  pRecInstSaveWork->RecPrivStByAdjust.Capability           = pRecInstWork->RecPrivStByAdjust.Capability;
  pRecInstSaveWork->RecPrivStByAdjust.WaitForPartner       = pRecInstWork->RecPrivStByAdjust.WaitForPartner;
#endif

  /* activate provisioned data */
  MRP_MEMCPY(&pRecInstWork->RecInstanceAdjust, &pRecInstPara->RecInstanceAdjust, sizeof(pRecInstWork->RecInstanceAdjust));

  pRecInstPara->RPort_1 = pRecInstPara->RecPortAdjust[0].PortID;
  pRecInstPara->RPort_2 = pRecInstPara->RecPortAdjust[1].PortID;
  
  if (pRecIfWork->RecIfAdjustHead.head.blockVersionLow == MRP_REC_DATA_BLK_VERS_LOW0)
    pRecInstPara->adminRole = mrp_rec_map_proj_role_to_admin_role(pDBrec->pChSys, LSA_FALSE, (MRP_PROJ_ROLE_ENUM_T)pRecInstPara->RecInstanceAdjust.Role);
  else  /* new dataset version of PDInterfaceMrpDataAdjust dataset */
    pRecInstPara->adminRole = mrp_rec_map_proj_role_to_admin_role(pDBrec->pChSys, LSA_TRUE, (MRP_PROJ_ROLE_ENUM_T)pRecInstPara->RecInstanceAdjust.Role);

  mrp_mib_write_domain_name (pDBrec->pChSys, instance, pRecInstWork->RecInstanceAdjust.DomainName, pRecInstWork->RecInstanceAdjust.LengthDomainName);
  mrp_mib_write_domain_uuid (pDBrec->pChSys, instance, pRecInstWork->RecInstanceAdjust.DomainUUID);
  
  for (idx = 0; idx < MRP_DEFAULT_VAL_INST_RPORT_CNT; idx++) 
  {
    MRP_MEMCPY(&pRecInstWork->RecPortAdjust[idx], &pRecInstPara->RecPortAdjust[idx], sizeof(MRP_PORT_DATA_ADJUST_TYPE));
  }
  pRecInstWork->RPort_1   = pRecInstPara->RPort_1;
  pRecInstWork->RPort_2   = pRecInstPara->RPort_2;
  pRecInstWork->adminRole = pRecInstPara->adminRole;
  
  MRP_MEMCPY(&pRecInstWork->RecInstanceCheck, &pRecInstPara->RecInstanceCheck, sizeof(pRecInstWork->RecInstanceCheck));
  MRP_MEMCPY(&pRecInstWork->RecInstanceUuidCheck, &pRecInstPara->RecInstanceCheck, sizeof(pRecInstWork->RecInstanceUuidCheck));
  
#ifdef MRP_CFG_PLUGIN_1_MRM_EXT
  MRP_MEMCPY(&pRecInstWork->RecPrivDataAdjust, &pRecInstPara->RecPrivDataAdjust, sizeof(pRecInstWork->RecPrivDataAdjust));
#endif

  if (mrpInMaxInstance && !instance)
  {
    LSA_UINT16 destIdx;

    /* STEP1: initialize/reset */
    for(i = 0; i < mrpInMaxInstance; i++)
    {
      /* initialize/reset "Copied" value for "work" set */
      pRecInstWork->pRecIcPortAdjust[i].InUse   = LSA_FALSE;
    }
    /* STEP2: copy already running instances to their corrcect "work" index/instance. This decouples the order of the received PRM values for the MRP_IN
              instances and the working instances. */
    for(i = 0; i < mrpInMaxInstance; i++)
    {
      destIdx = mrp_in_sys_map_iport_to_inInstance (pDBrec->pChSys, pRecInstPara->pRecIcPortAdjust[i].PortID);
      if(destIdx != MRP_IN_INSTANCE_NONE)
      {
        MRP_MEMCPY(&pRecInstWork->pRecIcPortAdjust[destIdx], &pRecInstPara->pRecIcPortAdjust[i], sizeof(MRP_IC_PORT_DATA_ADJUST_TYPE));
        MRP_MEMCPY(&pRecInstWork->pRecIcPortCheck[destIdx], &pRecInstPara->pRecIcPortCheck[i], sizeof(MRP_IC_PORT_DATA_CHECK_TYPE));
        pRecInstWork->pRecIcPortAdjust[destIdx].InUse   = LSA_TRUE;
        pRecInstPara->pRecIcPortAdjust[i].InUse         = LSA_TRUE;
      }
    }
    /* STEP3: copy the remaining instances to a free "work" index. An overflow cannot happen becaus the number of instances was already checked
              during checking of the PRM data (during filling of the param data set). */
    for(i = 0; i < mrpInMaxInstance; i++)
    {
      if(pRecInstPara->pRecIcPortAdjust[i].InUse == LSA_FALSE)  /* src data was not yet copied */
      {
        for(destIdx = 0; destIdx < mrpInMaxInstance; destIdx++)
        {
          if(pRecInstWork->pRecIcPortAdjust[destIdx].InUse == LSA_FALSE) /* free entry found */
          {
            break;
          }
        }
        MRP_MEMCPY(&pRecInstWork->pRecIcPortAdjust[destIdx], &pRecInstPara->pRecIcPortAdjust[i], sizeof(MRP_IC_PORT_DATA_ADJUST_TYPE));
        MRP_MEMCPY(&pRecInstWork->pRecIcPortCheck[destIdx], &pRecInstPara->pRecIcPortCheck[i], sizeof(MRP_IC_PORT_DATA_CHECK_TYPE));
        pRecInstWork->pRecIcPortAdjust[destIdx].InUse   = LSA_TRUE;
        pRecInstPara->pRecIcPortAdjust[i].InUse         = LSA_TRUE;
      }
    }
  }

#ifdef MRP_CFG_PLUGIN_2_HSR
  MRP_MEMCPY(&pRecInstWork->RecPrivHsrAdjust, &pRecInstPara->RecPrivHsrAdjust, sizeof(pRecInstWork->RecPrivHsrAdjust));

  for (i = 0; i < numOfPorts; i++)
  {
    pRecInstWork->RecPrivStByAdjust.pPortID[i] = pRecInstPara->RecPrivStByAdjust.pPortID[i];
  }
  pRecInstWork->RecPrivStByAdjust.Written              = pRecInstPara->RecPrivStByAdjust.Written;
  MRP_MEMCPY(pRecInstWork->RecPrivStByAdjust.ConName, pRecInstPara->RecPrivStByAdjust.ConName, sizeof(pRecInstWork->RecPrivStByAdjust.ConName));
  pRecInstWork->RecPrivStByAdjust.LengthConName        = pRecInstPara->RecPrivStByAdjust.LengthConName;
  pRecInstWork->RecPrivStByAdjust.LengthConNamePadding = pRecInstPara->RecPrivStByAdjust.LengthConNamePadding;
  pRecInstWork->RecPrivStByAdjust.PortCnt              = pRecInstPara->RecPrivStByAdjust.PortCnt;
  pRecInstWork->RecPrivStByAdjust.PortCntPadding       = pRecInstPara->RecPrivStByAdjust.PortCntPadding;
  pRecInstWork->RecPrivStByAdjust.Capability           = pRecInstPara->RecPrivStByAdjust.Capability;
  pRecInstWork->RecPrivStByAdjust.WaitForPartner       = pRecInstPara->RecPrivStByAdjust.WaitForPartner;
#endif
}

/*===========================================================================
* FUNCTION : mrp_rec_restore_saved_settings
*----------------------------------------------------------------------------
* PURPOSE  : use previous provisioning because of morphing actions (re-
*            provisioning is deferred)
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChUsr   = reference to PRM user channel
*            instance = affected MRP/HSR instance
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_rec_restore_saved_settings (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance)
{
  LSA_INT32 idx;
  LSA_UINT16 dummyRingport;
  MRP_INST_DATA_ADJUST_TYPE dummyRecInstanceAdjust;
  MRP_PORT_DATA_ADJUST_TYPE dummyRecPortAdjust;
  MRP_INST_DATA_CHECK_TYPE dummyRecInstanceCheck;
#ifdef MRP_CFG_PLUGIN_1_MRM_EXT
  MRP_IF_PRV_DATA_ADJUST_TYPE dummyRecPrivDataAdjust;
#endif
#ifdef MRP_CFG_PLUGIN_2_HSR
  MRP_IF_HSR_ADJUST_TYPE dummyRecPrivHsrAdjust;
  MRP_IF_STBY_ADJUST_TYPE dummyRecPrivStByAdjust;
#endif
  MRP_REC_DB_INST_SET_TYPE *pRecInstSaveWork, *pRecInstWork;
  MRP_REC_DB_TYPE *pDBrec=(MRP_REC_DB_TYPE *)pChUsr->pChUsrDB->pRec;

  pRecInstWork     = &pDBrec->pInstData[instance].work;
  pRecInstSaveWork = &pDBrec->pInstData[instance].saveWork;

  MRP_UPPER_TRACE_01(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_restore_saved_settings[inst=%d] entered", instance);

  /* enable old ringport settings again, because of morphing state */
  dummyRingport             = pRecInstWork->RPort_1;
  pRecInstWork->RPort_1     = pRecInstSaveWork->RPort_1;
  pRecInstSaveWork->RPort_1 = dummyRingport;

  dummyRingport             = pRecInstWork->RPort_2;
  pRecInstWork->RPort_2     = pRecInstSaveWork->RPort_2;
  pRecInstSaveWork->RPort_2 = dummyRingport;

  dummyRecInstanceAdjust              = pRecInstWork->RecInstanceAdjust;
  pRecInstWork->RecInstanceAdjust     = pRecInstSaveWork->RecInstanceAdjust;
  pRecInstSaveWork->RecInstanceAdjust = dummyRecInstanceAdjust;

  mrp_mib_write_domain_name (pDBrec->pChSys, instance, pRecInstWork->RecInstanceAdjust.DomainName, pRecInstWork->RecInstanceAdjust.LengthDomainName);
  mrp_mib_write_domain_uuid (pDBrec->pChSys, instance, pRecInstWork->RecInstanceAdjust.DomainUUID);

  /* MRM Alarm settings will be restored in function mrp_rec_restore_alarm_settings */
  /* only UUID checks of OHA have to be requested here, because alarm setting of MRP module is not affected */
  dummyRecInstanceCheck                  = pRecInstWork->RecInstanceUuidCheck;
  pRecInstWork->RecInstanceUuidCheck     = pRecInstSaveWork->RecInstanceUuidCheck;
  pRecInstSaveWork->RecInstanceUuidCheck = dummyRecInstanceCheck;
  
  for (idx = 0; idx < MRP_DEFAULT_VAL_INST_RPORT_CNT; idx++) 
  {  
    dummyRecPortAdjust                   = pRecInstWork->RecPortAdjust[idx];
    pRecInstWork->RecPortAdjust[idx]     = pRecInstSaveWork->RecPortAdjust[idx];
    pRecInstSaveWork->RecPortAdjust[idx] = dummyRecPortAdjust;
  }
#ifdef MRP_CFG_PLUGIN_1_MRM_EXT
  dummyRecPrivDataAdjust                 = pRecInstWork->RecPrivDataAdjust;
  pRecInstWork->RecPrivDataAdjust        = pRecInstSaveWork->RecPrivDataAdjust;
  pRecInstSaveWork->RecPrivDataAdjust    = dummyRecPrivDataAdjust;
#endif

#ifdef MRP_CFG_PLUGIN_2_HSR
  dummyRecPrivHsrAdjust                  = pRecInstWork->RecPrivHsrAdjust;
  pRecInstWork->RecPrivHsrAdjust         = pRecInstSaveWork->RecPrivHsrAdjust;
  pRecInstSaveWork->RecPrivHsrAdjust     = dummyRecPrivHsrAdjust;

  dummyRecPrivStByAdjust                 = pRecInstWork->RecPrivStByAdjust;
  pRecInstWork->RecPrivStByAdjust        = pRecInstSaveWork->RecPrivStByAdjust;
  pRecInstSaveWork->RecPrivStByAdjust    = dummyRecPrivStByAdjust;
#endif
}

/*===========================================================================
* FUNCTION : mrp_rec_restore_alarm_settings
*----------------------------------------------------------------------------
* PURPOSE  : use previous alarm settings because of morphing actions
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChUsr   = reference to PRM user channel
*            instance = affected MRP/HSR instance
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_rec_restore_alarm_settings (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance)
{
  MRP_INST_DATA_CHECK_TYPE dummyInstanceCheck;
  MRP_REC_DB_INST_SET_TYPE *pRecInstSaveWork, *pRecInstWork;
  MRP_REC_DB_TYPE *pDBrec = (MRP_REC_DB_TYPE *)pChUsr->pChUsrDB->pRec;

  pRecInstWork     = &pDBrec->pInstData[instance].work;
  pRecInstSaveWork = &pDBrec->pInstData[instance].saveWork;
  
  dummyInstanceCheck                 = pRecInstWork->RecInstanceCheck;
  pRecInstWork->RecInstanceCheck     = pRecInstSaveWork->RecInstanceCheck;
  pRecInstSaveWork->RecInstanceCheck = dummyInstanceCheck;
}


/*===========================================================================
* FUNCTION : mrp_rec_prm_prepare
*----------------------------------------------------------------------------
* PURPOSE  : parameter prepare
*----------------------------------------------------------------------------
* RETURNS  : MRP_RET_OK
*----------------------------------------------------------------------------
* INPUTS   : pChUsr = user channel reference
*            pRQB   = upper rqb reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : prepare signals the start of provisioning sequence
*            Any parameterization received before with PREPARE, but without END
*            is discarded.
*==========================================================================*/
LSA_UINT16 mrp_rec_prm_prepare (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB)
{
  LSA_UINT16 i, mrpMaxInstance;
  MRP_REC_DB_TYPE *pDBrec = (MRP_REC_DB_TYPE *)pChUsr->pChUsrDB->pRec;

  if (pChUsr->detailptr->PathType == MRP_PATH_TYPE_USER       &&
      pChUsr->detailptr->type.user.isPrmInterface == MRP_IS_PRM_INTERFACE_ON)
  {
    pDBrec->PrmEnd = LSA_FALSE;

    mrpMaxInstance = mrp_sys_get_num_of_instances(pChUsr);

    MRP_UPPER_TRACE_00(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_prm_prepare entered");
    pDBrec->PrmState = MRP_REC_ENUM_STATE_PRM_PREPARE;

    mrp_rec_set_if_para_defaults(pDBrec);
    for (i = 0; i < mrpMaxInstance; i++)
    {
      mrp_rec_set_inst_para_defaults (pDBrec, i, MRP_REC_ENUM_DEFAULT_FROM_STD);    
    }
    MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_OK);
    mrp_usr_call_upper_cbf_proc_next_req (pChUsr, pRQB);

    return MRP_RET_OK;
  }
  else
  {
    MRP_UPPER_TRACE_02(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_SEQUENCE: invalid channel for PRM_PREPARE (pathType=%d, isPrmIf=%d)", pChUsr->detailptr->PathType, pChUsr->detailptr->type.user.isPrmInterface);
    MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_SEQUENCE);

    return MRP_RET_ERR;
  }
}


/*===========================================================================
* FUNCTION : mrp_rec_prm_write
*----------------------------------------------------------------------------
* PURPOSE  : parameter write 
*----------------------------------------------------------------------------
* RETURNS  : MRP_RET_OK, MRP_RET_ERR
*----------------------------------------------------------------------------
* INPUTS   : pChUsr     = user channel reference
*            pRQB       = upper rqb reference
* OUTPUTS  : 
*            err_offset = offset into "record_data" of record "record_index",
*                         specifying the position of an error within the record
*----------------------------------------------------------------------------
* COMMENTS : WRITE ahead of PREPARE or after END is a sequence error (a bug in CM-PD).
*            RQB response:
*            MRP_RSP_OK 
*            MRP_RSP_ERR_SEQUENCE  
*            MRP_RSP_ERR_PARAM      
*            MRP_RSP_ERR_PRM_DATA   block structure wrong or doesn't match
*                                   delivered length field
*            MRP_RSP_ERR_PRM_BLOCK  block type/version unknown or block data
*                                   out of committed range
*            MRP_RSP_ERR_PRM_INDEX  index unknown
*            MRP_RSP_ERR_PRM_PORTID RQ attributes "record_index" and 
*                                   "edd_port_id" are no valid combination
*                                   (e.g. "edd_port_id" == 0, "record_index" 
*                                   is only defined for ports.)
*==========================================================================*/
LSA_UINT16 mrp_rec_prm_write (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB)
{
  LSA_INT32 parseLevel;
  LSA_UINT32 errOffset;
  LSA_UINT16 rc, numOfMrpInstances, mrpMaxInstance;
  MRP_UPPER_MEM_UINT8_PTR_TYPE pData;
  LSA_UINT16 blockType, blockVersion, blockLen;
  LSA_UINT16 length, lengthTotal;
  LSA_UINT8 useDefaultOffset;
  MRP_UPPER_ARGS_PTR_TYPE pArgs = MRP_RQB_GET_ARGS_PTR(pRQB);
  MRP_IF_DB_TYPE *pDBif = (MRP_IF_DB_TYPE *)pChUsr->pChSysDB->pInterfacePara;
  MRP_REC_DB_TYPE *pDBrec = (MRP_REC_DB_TYPE *)pChUsr->pChUsrDB->pRec;
  LSA_UINT16  pInstance[2];
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChUsr->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  /*
  pArgs->ctrl.prm_write.slot_number;
  pArgs->ctrl.prm_write.subslot_number;    
  */
  pInstance[MRP_INSTANCE_INDEX]     = 0;
  pInstance[MRP_IN_INSTANCE_INDEX]  = MRP_IN_INSTANCE_NONE;
  numOfMrpInstances = 1;
  useDefaultOffset  = LSA_FALSE;
  mrpMaxInstance    = mrp_sys_get_num_of_instances(pChUsr);

  errOffset         = MRP_PRM_ERR_OFFSET_DEFAULT;

  MRP_UPPER_TRACE_00(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_prm_write entered");

  MRP_REC_WRITE_ARGS_ERR_OFFSET_SET (pArgs, errOffset);
  pDBrec->tmpParsedProjRole = MRP_PROJ_ROLE_OFF;
  pDBrec->tmpParsedBaseAddr = pArgs->ctrl.prm_write.record_data;

  if (pChUsr->detailptr->PathType == MRP_PATH_TYPE_USER       &&
      pChUsr->detailptr->type.user.isPrmInterface == MRP_IS_PRM_INTERFACE_ON)
  {
    if (pDBrec->PrmEnd) 
    {
      MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_SEQUENCE: PRM_WRITE missing PRM_PREPARE or PRM_COMMIT, errOffset=%d", errOffset);
      MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_SEQUENCE);

      return MRP_RET_ERR;
    }
    /* check min length of buffer */
    if (pArgs->ctrl.prm_write.record_data_length < MRP_REC_DATA_LEN_MIN || pArgs->ctrl.prm_write.record_data_length > MRP_REC_DATA_LEN_MAX) 
    {
      MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_DATA: minLength/maxLength mismatch, len=0x%x, errOffset=%d",
        pArgs->ctrl.prm_write.record_data_length, errOffset);
      MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_PRM_DATA);

      return MRP_RET_ERR;
    }
    if (!mrpMaxInstance)
    {
      MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_INDEX: MRP/HSR provisioning not allowed, while mrpMaxInstance = 0!, errOffset=%d", errOffset);
      MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_PRM_INDEX);

      return MRP_RET_ERR;
    }


    for (parseLevel = 0; parseLevel < MRP_REC_ENUM_PARSE_END; parseLevel++) 
    {
      pData = pArgs->ctrl.prm_write.record_data;
      blockLen  = mrp_rec_get_block_header (pData, &blockType, &blockVersion);
      blockLen += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN; /* add length of BlockType, BlockLength */
      
      if (blockLen < MRP_REC_DATA_LEN_MIN || blockLen != pArgs->ctrl.prm_write.record_data_length) 
      {
        MRP_UPPER_TRACE_04(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_DATA: BlockType=0x%x, MinLength or BlockLength mismatch, (blockLen + 4)=0x%x, record_data_len=0x%x, errOffset=%d",
          blockType, blockLen, pArgs->ctrl.prm_write.record_data_length, errOffset);
        MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_PRM_DATA);

        return MRP_RET_ERR;
      }
      
      /* now parse the dataset to be written */
      length = (LSA_UINT16)pArgs->ctrl.prm_write.record_data_length;
      lengthTotal = 0;

      do 
      {
        pData += MRP_REC_DATA_LEN_BLK_HEADER; /*offset BlockHeader: BlockType(u16), BlockLength(u16), BlockVersionHigh(u8), BlockVersionLow(u8)*/
        
        switch (pArgs->ctrl.prm_write.record_index) 
        {
#ifdef MRP_CFG_PLUGIN_2_HSR
          case MRP_REC_DATA_IDX_PRV_HSR_ADJUST:
            if (0 != pArgs->ctrl.prm_write.edd_port_id) 
            {
              rc = MRP_RSP_ERR_PRM_PORTID;
              MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: IF-DS(=0x%x), edd_port_id(=%u)",
                pArgs->ctrl.prm_write.record_index, pArgs->ctrl.prm_write.edd_port_id);

              useDefaultOffset = LSA_TRUE;
            }
            else 
              rc = mrp_rec_parse_priv_hsr_adjust (pDBrec, parseLevel, &pData, &length,blockType);
            break;        

          case MRP_REC_DATA_IDX_PRV_STBY_ADJUST:
            if (0 != pArgs->ctrl.prm_write.edd_port_id) 
            {
              rc = MRP_RSP_ERR_PRM_PORTID;
              MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: IF-DS(=0x%x), edd_port_id(=%u)",
                pArgs->ctrl.prm_write.record_index, pArgs->ctrl.prm_write.edd_port_id);

              useDefaultOffset = LSA_TRUE;
            }
            else 
              rc = mrp_rec_parse_priv_stby_adjust (pDBrec, parseLevel, &pData, &length, blockType);
            break;

          case MRP_REC_DATA_IDX_PRV_HSR_CONFIG:
            if (0 != pArgs->ctrl.prm_write.edd_port_id) 
            {
              rc = MRP_RSP_ERR_PRM_PORTID;
              MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: IF-DS(=0x%x), edd_port_id(=%u)",
                pArgs->ctrl.prm_write.record_index, pArgs->ctrl.prm_write.edd_port_id);

              useDefaultOffset = LSA_TRUE;
            }
            else 
              rc = mrp_rec_parse_priv_hsr_config (pDBrec, parseLevel, &pData, &length, blockType);
            break;
#endif
#ifdef MRP_CFG_PLUGIN_1_MRM_EXT
          case MRP_REC_DATA_IDX_PRV_DATA_ADJUST:
            if (0 != pArgs->ctrl.prm_write.edd_port_id) 
            {
              rc = MRP_RSP_ERR_PRM_PORTID;
              MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: IF-DS(=0x%x), edd_port_id(=%u)",
                pArgs->ctrl.prm_write.record_index, pArgs->ctrl.prm_write.edd_port_id);

              useDefaultOffset = LSA_TRUE;
            }
            else 
              rc = mrp_rec_parse_priv_data_adjust (pDBrec, parseLevel, &pData, &length, blockType);
            break;
#endif
          case MRP_REC_DATA_IDX_IF_DATA_ADJUST:
            if (0 != pArgs->ctrl.prm_write.edd_port_id) 
            {
              rc = MRP_RSP_ERR_PRM_PORTID;
              MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: IF-DS(=0x%x), edd_port_id(=%u)",
                pArgs->ctrl.prm_write.record_index, pArgs->ctrl.prm_write.edd_port_id);

              useDefaultOffset = LSA_TRUE;
            }
            else 
              rc = mrp_rec_parse_interface_data_adjust (pDBrec, parseLevel, &pData, &length, &numOfMrpInstances, pInstance, blockType, blockLen, blockVersion);
            break;

          case MRP_REC_DATA_IDX_PORT_DATA_ADJUST:
            if (0 == pArgs->ctrl.prm_write.edd_port_id) 
            {
              rc = MRP_RSP_ERR_PRM_PORTID;
              MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: Port-DS(=0x%x), edd_port_id(=%u)",
                pArgs->ctrl.prm_write.record_index, pArgs->ctrl.prm_write.edd_port_id);

              useDefaultOffset = LSA_TRUE;
            }
            else 
            {
              if (pDBif->pPortData[pArgs->ctrl.prm_write.edd_port_id - 1].rpType == EDD_MRP_NO_RING_PORT)
              {
                rc = MRP_RSP_ERR_PRM_INDEX;
                MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_INDEX: write requ for DS rec_id = 0x%x to port(edd_port_id = %u) configured as EDD_MRP_NO_RING_PORT",
                  pArgs->ctrl.prm_write.record_index, pArgs->ctrl.prm_write.edd_port_id);

                useDefaultOffset = LSA_TRUE;
              }
              else
              {
                if (pDBif->pPortData[pArgs->ctrl.prm_write.edd_port_id - 1].moduleState == MRP_PRM_PORTMODULE_PLUG_PREPARE)
                {
                  rc = MRP_RSP_ERR_SEQUENCE;
                  MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_SEQUENCE: write requ for DS rec_id = 0x%x to port(edd_port_id = %u) although port in state PLUG_PREPARE",
                    pArgs->ctrl.prm_write.record_index, pArgs->ctrl.prm_write.edd_port_id);

                  useDefaultOffset = LSA_TRUE;
                }
                else
                {
                  rc = mrp_rec_parse_port_data_adjust (pDBrec, parseLevel, &pData, &length, blockType, blockLen, blockVersion, pArgs->ctrl.prm_write.edd_port_id);
                }
              }
            }
            break;


          case MRP_REC_DATA_IDX_IF_DATA_CHECK:
            if (0 != pArgs->ctrl.prm_write.edd_port_id) 
            {
              rc = MRP_RSP_ERR_PRM_PORTID;
              MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: IF-DS(=0x%x), edd_port_id(=%u)",
                pArgs->ctrl.prm_write.record_index, pArgs->ctrl.prm_write.edd_port_id);

              useDefaultOffset = LSA_TRUE;
            }
            else 
              rc = mrp_rec_parse_interface_data_check (pDBrec, parseLevel, &pData, &length, &numOfMrpInstances, pInstance, blockType, blockLen, blockVersion);
            break;

          case MRP_REC_DATA_IDX_IC_PORT_DATA_ADJUST:
            if (0 == pArgs->ctrl.prm_write.edd_port_id)
            {
              rc = MRP_RSP_ERR_PRM_PORTID;
              MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: Port-DS(=0x%x), edd_port_id(=%u)",
                pArgs->ctrl.prm_write.record_index, pArgs->ctrl.prm_write.edd_port_id);

              useDefaultOffset = LSA_TRUE;
            }
            else
            {
              if (pDBif->pPortData[pArgs->ctrl.prm_write.edd_port_id - 1].inPortType == EDD_SUPPORTS_MRP_INTERCONN_PORT_CONFIG_NO)
              {
                rc = MRP_RSP_ERR_PRM_INDEX;
                MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_INDEX: write requ for DS rec_id = 0x%x to port(edd_port_id = %u) configured as EDD_SUPPORTS_MRP_INTERCONN_PORT_CONFIG_NO",
                  pArgs->ctrl.prm_write.record_index, pArgs->ctrl.prm_write.edd_port_id);

                useDefaultOffset = LSA_TRUE;
              }
              else
              {
                if (pDBif->pPortData[pArgs->ctrl.prm_write.edd_port_id - 1].moduleState == MRP_PRM_PORTMODULE_PLUG_PREPARE)
                {
                  rc = MRP_RSP_ERR_SEQUENCE;
                  MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_SEQUENCE: write requ for DS rec_id = 0x%x to port(edd_port_id = %u) although port in state PLUG_PREPARE",
                    pArgs->ctrl.prm_write.record_index, pArgs->ctrl.prm_write.edd_port_id);

                  useDefaultOffset = LSA_TRUE;
                }
                else
                {
                    rc = mrp_rec_parse_ic_port_data_adjust(pDBrec, parseLevel, &pData, &length, blockType, blockLen, blockVersion, pArgs->ctrl.prm_write.edd_port_id);
                }
              }
            }
            break;

          case MRP_REC_DATA_IDX_IC_PORT_DATA_CHECK:
            if (0 == pArgs->ctrl.prm_write.edd_port_id)
            {
              rc = MRP_RSP_ERR_PRM_PORTID;
              MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: Port-DS(=0x%x), edd_port_id(=%u)",
                pArgs->ctrl.prm_write.record_index, pArgs->ctrl.prm_write.edd_port_id);

              useDefaultOffset = LSA_TRUE;
            }
            else
            {
              if (pDBif->pPortData[pArgs->ctrl.prm_write.edd_port_id - 1].inPortType == EDD_SUPPORTS_MRP_INTERCONN_PORT_CONFIG_NO)
              {
                rc = MRP_RSP_ERR_PRM_INDEX;
                MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_INDEX: write requ for DS rec_id = 0x%x to port(edd_port_id = %u) configured as EDD_SUPPORTS_MRP_INTERCONN_PORT_CONFIG_NO",
                  pArgs->ctrl.prm_write.record_index, pArgs->ctrl.prm_write.edd_port_id);

                useDefaultOffset = LSA_TRUE;
              }
              else
              {
                if (pDBif->pPortData[pArgs->ctrl.prm_write.edd_port_id - 1].moduleState == MRP_PRM_PORTMODULE_PLUG_PREPARE)
                {
                  rc = MRP_RSP_ERR_SEQUENCE;
                  MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_SEQUENCE: write requ for DS rec_id = 0x%x to port(edd_port_id = %u) although port in state PLUG_PREPARE",
                    pArgs->ctrl.prm_write.record_index, pArgs->ctrl.prm_write.edd_port_id);

                  useDefaultOffset = LSA_TRUE;
                }
                else
                {
                  rc = mrp_rec_parse_ic_port_data_check(pDBrec, parseLevel, &pData, &length, blockType, blockLen, blockVersion, pArgs->ctrl.prm_write.edd_port_id);
                }
              }
            }
            break;

          default:
            MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_INDEX: unknown index (0x%x), errOffset=%d", pArgs->ctrl.prm_write.record_index, errOffset);
            MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_PRM_INDEX);
            
            return MRP_RET_ERR;
            /*          break; */
        }
        if (rc != MRP_RSP_OK) 
        {
          MRP_RQB_SET_RESPONSE(pRQB, rc);
          
          if (!useDefaultOffset)
            errOffset = MRP_REC_DATA_OFFSET_GET(pArgs->ctrl.prm_write.record_data, pData);
          
          MRP_REC_WRITE_ARGS_ERR_OFFSET_SET (pArgs, errOffset);

          MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> mrp_rec_prm_write: errOffset=%d returned", errOffset);

          return MRP_RET_ERR;
        }
        lengthTotal = (LSA_UINT16)(lengthTotal + length);
        if (lengthTotal > pArgs->ctrl.prm_write.record_data_length) 
        {
          MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_DATA: record_data_length(=0x%x) less than lengthTotal(=0x%x), errOffset=%d",
            pArgs->ctrl.prm_write.record_data_length, lengthTotal, errOffset);
          MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_PRM_DATA);

          return MRP_RET_ERR;
        }
        if (lengthTotal == pArgs->ctrl.prm_write.record_data_length) 
        {
          break;
        }
        
        /* get next blockheader */
        blockLen = mrp_rec_get_block_header (pData, &blockType, &blockVersion);
        length   = blockLen;
        blockLen += MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN;
        
        if ((LSA_UINT32)(lengthTotal + blockLen) > pArgs->ctrl.prm_write.record_data_length) 
        {
          MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_DATA: record_data_length(=0x%x) mismatch with parsed len(=0x%x), errOffset=%d",
            pArgs->ctrl.prm_write.record_data_length, (lengthTotal + blockLen), errOffset);
          MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_PRM_DATA);

          return MRP_RET_ERR;
        }
      }while (length);
    }
    MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_OK);
    
    MRP_UPPER_TRACE_00(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_prm_write completed successfully");
    pDBrec->PrmState = MRP_REC_ENUM_STATE_PRM_WRITE;

    mrp_usr_call_upper_cbf_proc_next_req (pChUsr, pRQB);

    return MRP_RET_OK;
  }
  else
  {
    MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_SEQUENCE: invalid channel for PRM_WRITE (pathType=%d, isPrmIf=%d)", pChUsr->detailptr->PathType, pChUsr->detailptr->type.user.isPrmInterface);
    MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_SEQUENCE);

    return MRP_RET_ERR;
  }
}

/*===========================================================================
* FUNCTION : mrp_rec_check_prm_vers_consistency
*----------------------------------------------------------------------------
* PURPOSE  : check consistency of dataset versions at PRM_END state
*----------------------------------------------------------------------------
* RETURNS  : MRP_RET_OK, MRP_RET_ERR
*----------------------------------------------------------------------------
* INPUTS   : pChUsr = user channel reference
* OUTPUTS  : 
*            *prmEndRsp = detailed error info in case of negative acknowledgement
*            MRP_RET_OK, MRP_RET_ERR
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT16 mrp_rec_check_prm_vers_consistency (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, MRP_PRM_END_TYPE *pPrmEndRsp)
{
  LSA_UINT16 idx, instance, inInstance;
  LSA_UINT16 mrpMaxInstance, mrpInMaxInstance;
  LSA_UINT8  portAdjustDsWritten[EDD_CFG_MAX_MRP_INSTANCE_CNT];
  MRP_REC_DB_INST_SET_TYPE *pRecInstPara;
  MRP_REC_DB_IF_SET_TYPE *pRecIfPara;
  MRP_REC_DB_TYPE *pDBrec = (MRP_REC_DB_TYPE *)pChUsr->pChUsrDB->pRec;
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChUsr->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  mrpMaxInstance = mrp_sys_get_num_of_instances(pChUsr);
  pRecIfPara     = &pDBrec->para;
  mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pDBrec->pChSys);
  
  MRP_UPPER_TRACE_00(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_check_prm_vers_consistency entered");
 
  for (instance = 0; instance < mrpMaxInstance; instance++)
  {
    /* check if any PortAdjust DS is written */
    portAdjustDsWritten[instance] = LSA_FALSE;

    pRecInstPara = &pDBrec->pInstData[instance].para;

    for (idx = 0; idx < MRP_DEFAULT_VAL_INST_RPORT_CNT; idx++) 
    {
      if (pRecInstPara->RecPortAdjust[idx].Written) 
      {
        portAdjustDsWritten[instance] = LSA_TRUE;
      }
    }

    if (portAdjustDsWritten[instance])
    {
      if (pRecInstPara->RecInstanceAdjust.Written)
      {
        for (idx = 0; idx < MRP_DEFAULT_VAL_INST_RPORT_CNT; idx++) 
        {
          if (pRecInstPara->RecPortAdjust[idx].Written) 
          {
            if (pRecIfPara->RecIfAdjustHead.head.blockVersionLow != pRecInstPara->RecPortAdjust[idx].head.v.v_new.blockVersionLow)
            {
              MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: DS version low mismatch: ifAdjust=%d, recPortAdjust=%d",
                instance, pRecIfPara->RecIfAdjustHead.head.blockVersionLow, pRecInstPara->RecPortAdjust[idx].head.v.v_new.blockVersionLow);

              pPrmEndRsp->err_index  = MRP_REC_DATA_IDX_IF_DATA_ADJUST;
              pPrmEndRsp->err_offset = MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN + 1;

              return MRP_RET_ERR;      
            }
            if (pRecInstPara->RecInstanceCheck.Written && 
                pRecIfPara->RecIfCheckHead.head.blockVersionLow != pRecInstPara->RecPortAdjust[idx].head.v.v_new.blockVersionLow)
            {
              MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: DS version low mismatch: ifCheck=%d, recPortAdjust=%d",
                instance, pRecIfPara->RecIfCheckHead.head.blockVersionLow, pRecInstPara->RecPortAdjust[idx].head.v.v_new.blockVersionLow);

              pPrmEndRsp->err_index  = MRP_REC_DATA_IDX_IF_DATA_CHECK;
              pPrmEndRsp->err_offset = MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN + 1;

              return MRP_RET_ERR;      
            }
          }
        }
        if (pRecInstPara->RecInstanceCheck.Written && pRecIfPara->RecIfCheckHead.head.blockVersionLow != pRecIfPara->RecIfAdjustHead.head.blockVersionLow)
        {
          MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: DS version low mismatch: ifCheck=%d, ifAdjust=%d",
            instance, pRecIfPara->RecIfCheckHead.head.blockVersionLow, pRecIfPara->RecIfAdjustHead.head.blockVersionLow);
          
          pPrmEndRsp->err_index  = MRP_REC_DATA_IDX_IF_DATA_CHECK;
          pPrmEndRsp->err_offset = MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN + 1;

          return MRP_RET_ERR;        
        }        
      }
    }

    if (instance == MRP_DEFAULT_INSTANCE)
    {
      for (inInstance = 0; inInstance < mrpInMaxInstance; inInstance++)
      {
        /* MRP Interconnection */
        if (pRecInstPara->pRecIcPortAdjust[inInstance].Written && pRecInstPara->pRecIcPortCheck[inInstance].Written)
        {
          if(pRecInstPara->pRecIcPortCheck[inInstance].head.v.v_new.blockVersionLow != pRecInstPara->pRecIcPortAdjust[inInstance].head.v.v_new.blockVersionLow)
          {
            MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: DS version low mismatch: recIcPortCheck=%d, recIcPortAdjust=%d", instance, pRecInstPara->pRecIcPortCheck[inInstance].head.v.v_new.blockVersionLow, pRecInstPara->pRecIcPortAdjust[inInstance].head.v.v_new.blockVersionLow);

            pPrmEndRsp->err_index = MRP_REC_DATA_IDX_IC_PORT_DATA_CHECK;
            pPrmEndRsp->err_offset = MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN + 1;

            return MRP_RET_ERR;
          }
        }
      }
    }
  }   

  return MRP_RET_OK;
}  


/*===========================================================================
* FUNCTION : mrp_rec_check_prm_uuid_consistency
*----------------------------------------------------------------------------
* PURPOSE  : check for duplicates of domainUUID between different MRP datasets 
*            and MRP instances at PRM_END state
*----------------------------------------------------------------------------
* RETURNS  : MRP_RET_OK, MRP_RET_ERR
*----------------------------------------------------------------------------
* INPUTS   : pChUsr = user channel reference
* OUTPUTS  : 
*            *prmEndRsp = detailed error info in case of negative acknowledgement
*            MRP_RET_OK, MRP_RET_ERR
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT16 mrp_rec_check_prm_uuid_consistency (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, MRP_PRM_END_TYPE *pPrmEndRsp)
{
  LSA_UINT16 idx, idx2, i, k;
  LSA_UINT16 mrpMaxInstance;
  MRP_REC_DB_INST_SET_TYPE *pRecInstPara, *pRecInst2Para;
  MRP_REC_DB_TYPE *pDBrec = (MRP_REC_DB_TYPE *)pChUsr->pChUsrDB->pRec;
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChUsr->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  mrpMaxInstance = mrp_sys_get_num_of_instances(pChUsr);
  
  MRP_UPPER_TRACE_00(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_check_prm_uuid_consistency entered");
  
  for (i = 0; i < mrpMaxInstance; i++)
  {
    pRecInstPara = &pDBrec->pInstData[i].para;

    /* check UUID consistency within the same MRP instance */
    for (idx = 0; idx < MRP_DEFAULT_VAL_INST_RPORT_CNT; idx++) 
    {
      if (pRecInstPara->RecPortAdjust[idx].Written) 
      {
        if (pRecInstPara->RecInstanceAdjust.Written)
        {
          if (MRP_MEMCMP(pRecInstPara->RecInstanceAdjust.DomainUUID, pRecInstPara->RecPortAdjust[idx].DomainUUID, sizeof(pRecInstPara->RecInstanceAdjust.DomainUUID))) 
          {
            MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: DomainUUID comparison failed (IfAdjust - PortAdjust)", i);
            
            pPrmEndRsp->err_index   = MRP_REC_DATA_IDX_PORT_DATA_ADJUST;
            pPrmEndRsp->err_port_id = pRecInstPara->RecPortAdjust[idx].PortID;
            pPrmEndRsp->err_fault   = MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS;
            pPrmEndRsp->err_offset  = MRP_REC_DATA_LEN_BLK_HEADER + MRP_REC_DATA_LEN_BLK_HEADER_PADDING;

            return MRP_RET_ERR;
          }        
        }
        if (pRecInstPara->RecInstanceCheck.Written)
        {
          if (MRP_MEMCMP(pRecInstPara->RecInstanceCheck.DomainUUID, pRecInstPara->RecPortAdjust[idx].DomainUUID, sizeof(pRecInstPara->RecInstanceCheck.DomainUUID))) 
          {
            MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: DomainUUID comparison failed (IfCheck - PortAdjust)", i);

            pPrmEndRsp->err_index   = MRP_REC_DATA_IDX_IF_DATA_CHECK;
            pPrmEndRsp->err_fault   = MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS;
            pPrmEndRsp->err_offset = pRecInstPara->RecInstanceCheck.err_offset_base + MRP_REC_DATA_LEN_BLK_HEADER + MRP_REC_DATA_LEN_BLK_HEADER_PADDING;

            return MRP_RET_ERR;
          }
        }
      }
    }
    if (pRecInstPara->RecInstanceAdjust.Written && pRecInstPara->RecInstanceCheck.Written)
    {
      if (MRP_MEMCMP(pRecInstPara->RecInstanceCheck.DomainUUID, pRecInstPara->RecInstanceAdjust.DomainUUID, sizeof(pRecInstPara->RecInstanceCheck.DomainUUID))) 
      {
        MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: DomainUUID comparison failed (IfCheck - IfAdjust)", i);
        
        pPrmEndRsp->err_index   = MRP_REC_DATA_IDX_IF_DATA_CHECK;
        pPrmEndRsp->err_fault   = MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS;
        pPrmEndRsp->err_offset = pRecInstPara->RecInstanceCheck.err_offset_base + MRP_REC_DATA_LEN_BLK_HEADER + MRP_REC_DATA_LEN_BLK_HEADER_PADDING;

        return MRP_RET_ERR;
      }
    }

    /* check UUID consistency within different MRP instances */
    for (k = i + 1; k < mrpMaxInstance; k++)
    {
      pRecInst2Para = &pDBrec->pInstData[k].para;
      
      for (idx = 0; idx < MRP_DEFAULT_VAL_INST_RPORT_CNT; idx++) 
      {
        if (pRecInstPara->RecPortAdjust[idx].Written)
        {
          for (idx2 = 0; idx2 < MRP_DEFAULT_VAL_INST_RPORT_CNT; idx2++) 
          {
            if (pRecInst2Para->RecPortAdjust[idx2].Written) 
            {
              if (!MRP_MEMCMP(pRecInstPara->RecPortAdjust[idx].DomainUUID, pRecInst2Para->RecPortAdjust[idx2].DomainUUID, sizeof(pRecInstPara->RecPortAdjust[idx].DomainUUID))) 
              {
                MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY: DomainUUID must be different between PortAdjust[inst0=%d] and PortAdjust[inst1=%d]", i, k);
                
                pPrmEndRsp->err_index   = MRP_REC_DATA_IDX_PORT_DATA_ADJUST;
                pPrmEndRsp->err_port_id = pRecInst2Para->RecPortAdjust[idx2].PortID;
                pPrmEndRsp->err_fault   = MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS;
                pPrmEndRsp->err_offset  = MRP_REC_DATA_LEN_BLK_HEADER + MRP_REC_DATA_LEN_BLK_HEADER_PADDING;

                return MRP_RET_ERR;
              }
              if (pRecInstPara->RecPortAdjust[idx].PortID == pRecInst2Para->RecPortAdjust[idx2].PortID)
              {
                MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY: PortID(=%d) must be different between PortAdjust[inst0=%d] and PortAdjust[inst1=%d]", pRecInstPara->RecPortAdjust[idx].PortID, i, k);
                
                pPrmEndRsp->err_index   = MRP_REC_DATA_IDX_PORT_DATA_ADJUST;
                pPrmEndRsp->err_port_id = pRecInst2Para->RecPortAdjust[idx2].PortID;
                pPrmEndRsp->err_fault   = MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS;

                return MRP_RET_ERR;              
              }
            }
          }        
        }
      }
    }
  }  
  
  return MRP_RET_OK;
}  


/*===========================================================================
* FUNCTION : mrp_rec_check_prm_consistency
*----------------------------------------------------------------------------
* PURPOSE  : check consistency of parameterization at PRM_END state
*----------------------------------------------------------------------------
* RETURNS  : MRP_RET_OK, MRP_RET_ERR
*----------------------------------------------------------------------------
* INPUTS   : pChUsr = user channel reference
* OUTPUTS  : 
*            *prmEndRsp = detailed error info in case of negative acknowledgement
*            MRP_RET_OK, MRP_RET_ERR
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT16 mrp_rec_check_prm_consistency (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, MRP_PRM_END_TYPE *pPrmEndRsp)
{
  LSA_UINT16 idx, i, k, l, portID, numOfPorts, retVal;
  LSA_UINT16 portAdjustDsCnt, mrpMaxInstance, mrpInMaxInstance;
  MRP_REC_DB_INST_SET_TYPE *pRecInstPara;
  MRP_REC_DB_TYPE *pDBrec = (MRP_REC_DB_TYPE *)pChUsr->pChUsrDB->pRec;
  MRP_IF_DB_TYPE *pDBif   = (MRP_IF_DB_TYPE *)pChUsr->pChSysDB->pInterfacePara;

  MRP_IC_PORT_DATA_CHECK_TYPE *pCopyRecIcPortCheck;
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChUsr->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  mrpMaxInstance = mrp_sys_get_num_of_instances(pChUsr);
  numOfPorts     = mrp_sys_get_num_of_ports (pDBrec->pChSys);
  mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pDBrec->pChSys);

  MRP_UPPER_TRACE_00(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_check_prm_consistency entered");
  
  for (k = 0; k < numOfPorts; k++)
  {
    if (pDBif->pPortData[k].moduleState == MRP_PRM_PORTMODULE_PULLED && 
        (pDBif->pPortData[k].rpType      != EDD_MRP_NO_RING_PORT ||
         pDBif->pPortData[k].inPortType   == EDD_SUPPORTS_MRP_INTERCONN_PORT_CONFIG_YES))  /* only respond with NOT_APPLICABLE, if the port is a default ringport or could be configured as ringport or is an interconnection port */
    {
      pPrmEndRsp->portparamsNotApplicable[k] = MRP_PRM_PORT_PARAMS_NOT_APPLICABLE;
      
      portID = k + 1;
      MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_check_prm_consistency[port=%d]: Port params not applicable, because port NOT present", portID);
  
      LSA_UNUSED_ARG(portID);   /* hhr: to please the compiler in case traces are disabled. */

      MRP_FATAL1(pPrmEndRsp->isModularPort[k] == MRP_PORT_IS_MODULAR); /* compact port is pulled -> should never happen*/
    }
  }

  /* check for availability of mandatory MRP records */
  for (i = 0; i < mrpMaxInstance; i++)
  {
    pRecInstPara = &pDBrec->pInstData[i].para;
    portAdjustDsCnt = 0;

    for (idx = 0; idx < MRP_DEFAULT_VAL_INST_RPORT_CNT; idx++)
    {
      if (pRecInstPara->RecPortAdjust[idx].Written)
      {
        portAdjustDsCnt++;
      }
    }
    if (portAdjustDsCnt && portAdjustDsCnt != MRP_DEFAULT_VAL_INST_RPORT_CNT)
    {
      MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: PortAdjust records received/required=%d/2!", i, portAdjustDsCnt);

      if (portAdjustDsCnt < MRP_DEFAULT_VAL_INST_RPORT_CNT)
      {
        pPrmEndRsp->err_index = MRP_REC_DATA_IDX_PORT_DATA_ADJUST;
        pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_NO_OR_TOO_LESS_PARAMS;
      }
      else
      {
        pPrmEndRsp->err_index = MRP_REC_DATA_IDX_PORT_DATA_ADJUST;
        pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS;
      }

      return MRP_RET_ERR;
    }

    if (portAdjustDsCnt)
    {
      if (!pRecInstPara->RecInstanceAdjust.Written)
      {
        pPrmEndRsp->err_index = MRP_REC_DATA_IDX_IF_DATA_ADJUST;
        pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_NO_OR_TOO_LESS_PARAMS;

#ifdef MRP_CFG_PLUGIN_2_HSR
        if (i == 0)
        {
          if (!pRecInstPara->RecPrivHsrAdjust.Written)
          {
            MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: PortAdjustRec written, but NO HSR/MRP InterfaceAdjustRec", i);

            return MRP_RET_ERR;
          }
        }
        else  /* no HSR allowed for instances > 0 */
        {
          MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: PortAdjustRec written, but NO InterfaceAdjustRec", i);

          return MRP_RET_ERR;
        }
#else
        MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: PortAdjustRec written, but NO InterfaceAdjustRec", i);

        return MRP_RET_ERR;
#endif
      }
      else /* pRecInstPara->RecInstanceAdjust.Written */
      {
        if (pRecInstPara->RecInstanceAdjust.Role == MRP_PROJ_ROLE_OFF)
        {
          pPrmEndRsp->err_index = MRP_REC_DATA_IDX_PORT_DATA_ADJUST;
          pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS;

          if (i == 0)
          {
#ifdef MRP_CFG_PLUGIN_2_HSR
            if (!pRecInstPara->RecPrivHsrAdjust.Written)
            {
              MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: PortAdjustRec written, InterfaceAdjustRole=OFF, but NO PrivHsrAdjustRec written", i);

              pPrmEndRsp->err_index = MRP_REC_DATA_IDX_PRV_DATA_ADJUST;
              pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_NO_OR_TOO_LESS_PARAMS;

              return MRP_RET_ERR;
            }
#else
            MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: PortAdjustRec written, but InterfaceAdjustRole=OFF", i);

            return MRP_RET_ERR;
#endif
          }
          else
          {
            MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: PortAdjustRec written, but InterfaceAdjustRole=OFF", i);

            return MRP_RET_ERR;
          }
        }
      }
    }
    else  /* portAdjustCnt == 0 */
    {
      if (pRecInstPara->RecInstanceCheck.Written && !pRecInstPara->RecInstanceAdjust.Written)
      {
        MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_UNEXP, ">> ERR_PRM_CONSISTENCY[inst=%d]: InterfaceCheckRec written, but no InterfaceAdjustRec", i);

        pPrmEndRsp->err_index = MRP_REC_DATA_IDX_IF_DATA_ADJUST;
        pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_NO_OR_TOO_LESS_PARAMS;

        return MRP_RET_ERR;
      }
      if (pRecInstPara->RecInstanceCheck.Written && pRecInstPara->RecInstanceAdjust.Written)
      {
        if (pRecInstPara->RecInstanceAdjust.Role == MRP_PROJ_ROLE_OFF && pRecInstPara->RecInstanceCheck.Check != 0)
        {
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_UNEXP, ">> ERR_PRM_CONSISTENCY[inst=%d]: InterfaceAdjustRec (role = MRP_OFF), but InterfaceCheckRec(Check=0x%x) written", i, pRecInstPara->RecInstanceCheck.Check);

          pPrmEndRsp->err_index = MRP_REC_DATA_IDX_IF_DATA_CHECK;
          pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS;
          pPrmEndRsp->err_offset = pRecInstPara->RecInstanceCheck.err_offset_base + MRP_REC_DATA_LEN_BLK_HEADER + MRP_REC_DATA_LEN_BLK_HEADER_PADDING + MRP_REC_DATA_LEN_UUID;

          return MRP_RET_ERR;
        }
      }
      if (pRecInstPara->RecInstanceAdjust.Written && pRecInstPara->RecInstanceAdjust.Role != MRP_PROJ_ROLE_OFF)
      {
        MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: InterfaceAdjustRec written (role=%d), but no PortAdjustRec", i, pRecInstPara->RecInstanceAdjust.Role);

        pPrmEndRsp->err_index = MRP_REC_DATA_IDX_PORT_DATA_ADJUST;
        pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_NO_OR_TOO_LESS_PARAMS;

        return MRP_RET_ERR;
      }
    }

    if ((i == MRP_DEFAULT_INSTANCE) && mrpInMaxInstance)
    {
      /* make a local copy of pRecIcPortCheck. It is needed to change the order of the written records if needed (portID does not fit to pRecIcPortAdjust data)*/
      /* the data copy "pCopyRecIcPortCheck" is unchanged so all checks have to be based on it!! */
      MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pCopyRecIcPortCheck), (LSA_UINT16)(sizeof(MRP_IC_PORT_DATA_CHECK_TYPE) * mrpInMaxInstance));
      MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pCopyRecIcPortCheck, LSA_NULL));
      MRP_MEMCPY(pCopyRecIcPortCheck, pRecInstPara->pRecIcPortCheck, ((LSA_UINT16)sizeof(MRP_IC_PORT_DATA_CHECK_TYPE) * mrpInMaxInstance));

      for (k = 0; k < mrpInMaxInstance; k++)
      {
        /* MRP Interconnection */
        LSA_UINT16 inInstance;

        if (pRecInstPara->pRecIcPortAdjust[k].Written)
        {
          /* MRP Interconnection: check the consistency of pRecIcPortAdjust data by comparing the ID and the PortID. */
          /* Compare the current instance with the remaining instances. */
          for (inInstance = k + 1; inInstance < mrpInMaxInstance; inInstance++)
          {
            /* check if the instance is free/unused */
            if (pRecInstPara->pRecIcPortAdjust[inInstance].Written != LSA_TRUE)
            {
              /* nothing to check: leave the check loop. */
              continue;
            }

            if (pRecInstPara->pRecIcPortAdjust[k].DomainID == pRecInstPara->pRecIcPortAdjust[inInstance].DomainID)         /* Same DomainID twice!! */
            {
              MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inInst=%d]: DomainID same as[inInst=%d]!", k, inInstance);

              pPrmEndRsp->err_index = MRP_REC_DATA_IDX_IC_PORT_DATA_ADJUST;
              pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS;

              MRP_FREE_LOCAL_MEM(&retVal, (MRP_LOCAL_MEM_PTR_TYPE)pCopyRecIcPortCheck);
              MRP_FATAL1(retVal == LSA_RET_OK);
              return MRP_RET_ERR;
            }

            if (pRecInstPara->pRecIcPortAdjust[k].PortID == pRecInstPara->pRecIcPortAdjust[inInstance].PortID)          /* Same portID twice!! */
            {
              MRP_UPPER_TRACE_04(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inInst=%d]: interconnection port (%u) matches[inInst=%d] port (%u)",
                  k, pRecInstPara->pRecIcPortAdjust[k].PortID, inInstance, pRecInstPara->pRecIcPortAdjust[inInstance].PortID);

              pPrmEndRsp->err_index = MRP_REC_DATA_IDX_IC_PORT_DATA_ADJUST;
              pPrmEndRsp->err_port_id = pRecInstPara->pRecIcPortAdjust[k].PortID;
              pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS;

              MRP_FREE_LOCAL_MEM(&retVal, (MRP_LOCAL_MEM_PTR_TYPE)pCopyRecIcPortCheck);
              MRP_FATAL1(retVal == LSA_RET_OK);
              return MRP_RET_ERR;
            }
          }
        }

        if (pCopyRecIcPortCheck[k].Written) /* always check for the data copy because it is not changed/reordered */
        {
          /* check the consistency of  by pRecIcPortCheck data by comparing the PortID. */
          /* Compare the current instance with the remaining instances. */
          for (inInstance = k + 1; inInstance < mrpInMaxInstance; inInstance++)
          {
            if (pCopyRecIcPortCheck[inInstance].Written != LSA_TRUE)
            {
              /* nothing to check: leave the check loop. */
              continue;
            }

            if (pCopyRecIcPortCheck[k].PortID == pCopyRecIcPortCheck[inInstance].PortID)          /* Same portID twice!! */
            {
              MRP_UPPER_TRACE_04(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inInst=%d]: interconnection pRecIcPortCheck.port (%u) matches[inInst=%d] pRecIcPortCheck.port (%u)",
                  k, pCopyRecIcPortCheck[k].PortID, inInstance, pCopyRecIcPortCheck[inInstance].PortID);

              pPrmEndRsp->err_index = MRP_REC_DATA_IDX_IC_PORT_DATA_CHECK;
              pPrmEndRsp->err_port_id = pCopyRecIcPortCheck[k].PortID;
              pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS;
              pPrmEndRsp->err_offset = MRP_REC_DATA_LEN_BLK_HEADER + MRP_REC_DATA_LEN_BLK_HEADER_PADDING;

              MRP_FREE_LOCAL_MEM(&retVal, (MRP_LOCAL_MEM_PTR_TYPE)pCopyRecIcPortCheck);
              MRP_FATAL1(retVal == LSA_RET_OK);
              return MRP_RET_ERR;
            }
          }

          /* Check the consistency of the pRecIcPortCheck per inInstance. If the pRecIcPortCheck was not written in the same order as pRecIcPortAdjust it is reordered. */

          inInstance = MRP_IN_INSTANCE_NONE;

          /* Search for correct inInstance, based on the portID */
          for (l = 0; l < mrpInMaxInstance; l++)
          {

            if (pCopyRecIcPortCheck[k].PortID == pRecInstPara->pRecIcPortAdjust[l].PortID)         /* Search for the correct portID */
            {
              /* inInstance found */
              inInstance  = l;
              if(k != l)
              {
                /* copy data to correct inInstance */
                MRP_MEMCPY(&pRecInstPara->pRecIcPortCheck[l], &pCopyRecIcPortCheck[k], sizeof(MRP_IC_PORT_DATA_CHECK_TYPE));
              }
              else
              {
                /* nothing to copy */
              }
              break;
            }
          }

          /* no fitting inInstance found. */
          if (inInstance == MRP_IN_INSTANCE_NONE)
          {
            MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_UNEXP, ">> ERR_PRM_CONSISTENCY[inInst=%d]: IcPortCheckRec written, but no IcPortAdjustRec", k);

            pPrmEndRsp->err_index = MRP_REC_DATA_IDX_IC_PORT_DATA_CHECK;
            pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS;
            pPrmEndRsp->err_offset = MRP_REC_DATA_LEN_BLK_HEADER + MRP_REC_DATA_LEN_BLK_HEADER_PADDING;

            MRP_FREE_LOCAL_MEM(&retVal, (MRP_LOCAL_MEM_PTR_TYPE)pCopyRecIcPortCheck);
            MRP_FATAL1(retVal == LSA_RET_OK);
            return MRP_RET_ERR;
          }
          else if (pRecInstPara->pRecIcPortAdjust[k].Role == MRP_IC_PROJ_ROLE_OFF && pRecInstPara->pRecIcPortCheck[k].Check != 0)
          {
            MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_UNEXP, ">> ERR_PRM_CONSISTENCY[inInst=%d]: IcPortAdjustRec (role = MRP_IC_PROJ_ROLE_OFF), but IcPortCheckRec(Check=0x%x) written", k, pRecInstPara->pRecIcPortCheck[k].Check);

            pPrmEndRsp->err_index = MRP_REC_DATA_IDX_IC_PORT_DATA_CHECK;
            pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS;
            pPrmEndRsp->err_offset = MRP_REC_DATA_LEN_BLK_HEADER + MRP_REC_DATA_LEN_BLK_HEADER_PADDING;

            MRP_FREE_LOCAL_MEM(&retVal, (MRP_LOCAL_MEM_PTR_TYPE)pCopyRecIcPortCheck);
            MRP_FATAL1(retVal == LSA_RET_OK);
            return MRP_RET_ERR;
          }
        }
      }

      MRP_FREE_LOCAL_MEM(&retVal, (MRP_LOCAL_MEM_PTR_TYPE)pCopyRecIcPortCheck);
      MRP_FATAL1(retVal == LSA_RET_OK);

    }
  }
  
  retVal = mrp_rec_check_prm_vers_consistency (pChUsr, pPrmEndRsp);
  if (retVal != MRP_RET_OK)
    return MRP_RET_ERR;
    
  retVal = mrp_rec_check_prm_uuid_consistency (pChUsr, pPrmEndRsp);
  if (retVal != MRP_RET_OK)
    return MRP_RET_ERR;
  
  for (i = 0; i < mrpMaxInstance; i++)
  {
    pRecInstPara = &pDBrec->pInstData[i].para;

    /* mrp on */
    if (pRecInstPara->RecInstanceAdjust.Role != MRP_PROJ_ROLE_OFF) 
    {
      for (idx = 0; idx < MRP_DEFAULT_VAL_INST_RPORT_CNT; idx++) 
      {
        if (!MRP_MEMCMP(pRecInstPara->RecPortAdjust[idx].DomainUUID, pRecInstPara->RecInstanceAdjust.DomainUUID, sizeof(pRecInstPara->RecPortAdjust->DomainUUID))) 
        {
          if ( (idx % 2) == 0) 
          {
            pRecInstPara->RPort_1 = pRecInstPara->RecPortAdjust[idx].PortID;
          }
          else
          {
            pRecInstPara->RPort_2 = pRecInstPara->RecPortAdjust[idx].PortID;
          }
        }
      }

      /* check if we have check record for this domain */
      if (pRecInstPara->RecInstanceCheck.Written) 
      {
        if ((pRecInstPara->RecInstanceCheck.Check & MRP_REC_DATA_CHECK_MRM) && MRP_PROJ_ROLE_CLIENT == pRecInstPara->RecInstanceAdjust.Role) 
        {
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: wrong setting of MRP_Check in PDInterfaceMrpDataCheck (doesn't match to MRP_Role), ifAdjustRole=%d", i, pRecInstPara->RecInstanceAdjust.Role);
          
          pPrmEndRsp->err_index = MRP_REC_DATA_IDX_IF_DATA_CHECK;
          pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS;
          pPrmEndRsp->err_offset = pRecInstPara->RecInstanceCheck.err_offset_base + MRP_REC_DATA_LEN_BLK_HEADER + MRP_REC_DATA_LEN_BLK_HEADER_PADDING + MRP_REC_DATA_LEN_UUID;

          return MRP_RET_ERR;
        }
      }
    }

    if (i == MRP_DEFAULT_INSTANCE)
    {
      for (k = 0; k < mrpInMaxInstance; k++)
      {
        /* mrp interconnection on */
        if (pRecInstPara->pRecIcPortAdjust[k].Written && pRecInstPara->pRecIcPortAdjust[k].Role != MRP_IC_PROJ_ROLE_OFF)
        {
          if (pRecInstPara->pRecIcPortCheck[k].Written)
          {
            if ((pRecInstPara->pRecIcPortCheck[k].Check & MRP_REC_IC_PORT_DATA_CHECK_MIM) && MRP_IC_PROJ_ROLE_CLIENT == pRecInstPara->pRecIcPortAdjust[k].Role)
            {
              MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: wrong setting of MRPIC_Check in PDPortMrpIcDataCheck (doesn't match to MRP_Role), IcPortAdjustRole=%d", i, pRecInstPara->pRecIcPortAdjust[k].Role);

              pPrmEndRsp->err_index = MRP_REC_DATA_IDX_IC_PORT_DATA_CHECK;
              pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS;
              pPrmEndRsp->err_offset = MRP_REC_DATA_LEN_BLK_HEADER + MRP_REC_DATA_LEN_BLK_HEADER_PADDING ;

              return MRP_RET_ERR;
            }
          }

          if (pRecInstPara->RecInstanceAdjust.Written && pRecInstPara->RecInstanceAdjust.Role != MRP_PROJ_ROLE_OFF)
          {
            if (pRecInstPara->RPort_1 == pRecInstPara->pRecIcPortAdjust[k].PortID || pRecInstPara->RPort_2 == pRecInstPara->pRecIcPortAdjust[k].PortID)
            {
              MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: interconnection port (%u) matches mrp ring port (%u)",
                  i, pRecInstPara->pRecIcPortAdjust[k].PortID, pRecInstPara->RPort_1 == pRecInstPara->pRecIcPortAdjust[k].PortID ? pRecInstPara->RPort_1: pRecInstPara->RPort_2);

              pPrmEndRsp->err_index = MRP_REC_DATA_IDX_IC_PORT_DATA_ADJUST;
              pPrmEndRsp->err_port_id = pRecInstPara->pRecIcPortAdjust[k].PortID;
              pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS;

              return MRP_RET_ERR;
            }
          }
          else
          {
            MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: cannot activate mrp interconnection without mrp ring redundancy", i);

            pPrmEndRsp->err_index = MRP_REC_DATA_IDX_IC_PORT_DATA_ADJUST;
            pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS;

            return MRP_RET_ERR;
          }
        }
      }
    }

#ifdef MRP_CFG_PLUGIN_2_HSR
    if (i == 0) /* HSR provisioning is only applied to instance = 0 */
    {
      /* hsr ? */
      if (pRecInstPara->RecInstanceAdjust.Written && pRecInstPara->RecInstanceAdjust.Role != MRP_PROJ_ROLE_OFF)
      {
        if (MRP_REC_DATA_HSR_ROLE_CL == pRecInstPara->RecPrivHsrAdjust.Role || 
            MRP_REC_DATA_HSR_ROLE_MGR == pRecInstPara->RecPrivHsrAdjust.Role) 
        {
          MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: cannot configure mrp and hsr together", i);
          
          pPrmEndRsp->err_index = MRP_REC_DATA_IDX_PRV_HSR_ADJUST;
          pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS;
          pPrmEndRsp->err_offset = MRP_REC_DATA_LEN_BLK_HEADER + MRP_REC_DATA_LEN_BLK_HEADER_PADDING;

          return MRP_RET_ERR;
        }
        /* stdby activated ? */
        if (pRecInstPara->RecPrivStByAdjust.Written && pRecInstPara->RecPrivStByAdjust.PortCnt) 
        {
          MRP_UPPER_TRACE_04(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: cannot configure mrp and stdby together, rport1=%d, rport2=%d, StByPort=%d",
            i, pRecInstPara->RPort_1, pRecInstPara->RPort_2, pRecInstPara->RecPrivStByAdjust.pPortID[pRecInstPara->RecPrivStByAdjust.PortCnt - 1]);

          pPrmEndRsp->err_index = MRP_REC_DATA_IDX_PRV_STBY_ADJUST;
          pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS;

          return MRP_RET_ERR;
        }

        /* as mrp interconnection can only become active with an active mrp ring redundancy configuration, an
           invalid combination of mrp interconnection with hsr or standby is prevented here implicit */
      }

      if (MRP_REC_DATA_HSR_ROLE_INVALIDATE_PROJ == pRecInstPara->RecPrivHsrAdjust.Role) 
      {
        MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_UNEXP, ">> ERR_PRM_CONSISTENCY[inst=%d]: by user request", i);

        pPrmEndRsp->err_index = MRP_REC_DATA_IDX_PRV_HSR_ADJUST;
        pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS;
        pPrmEndRsp->err_offset = MRP_REC_DATA_LEN_BLK_HEADER + MRP_REC_DATA_LEN_BLK_HEADER_PADDING;

        return MRP_RET_ERR;
      }
      /* hsr on */
      if (MRP_REC_DATA_HSR_ROLE_OFF != pRecInstPara->RecPrivHsrAdjust.Role) 
      {
        for (idx = pRecInstPara->RecPrivStByAdjust.PortCnt; idx > 0; idx--) 
        {
          if (pRecInstPara->RecPrivStByAdjust.pPortID[idx - 1] == pRecInstPara->RecPrivHsrAdjust.PortA) 
          {
            MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: hsr ring port (%u) matches stdby port (%u)",
              i, pRecInstPara->RecPrivHsrAdjust.PortA, pRecInstPara->RecPrivStByAdjust.pPortID[idx - 1]);

            pPrmEndRsp->err_index = MRP_REC_DATA_IDX_PRV_HSR_ADJUST;
            pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS;
            pPrmEndRsp->err_offset = MRP_REC_DATA_LEN_BLK_HEADER + MRP_REC_DATA_LEN_BLK_HEADER_PADDING + MRP_REC_DATA_LEN_ROLE;
            
            return MRP_RET_ERR;
          }
          if (pRecInstPara->RecPrivStByAdjust.pPortID[idx - 1] == pRecInstPara->RecPrivHsrAdjust.PortB) 
          {
            MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: hsr ring port (%u) matches stdby port (%u)",
              i, pRecInstPara->RecPrivHsrAdjust.PortB, pRecInstPara->RecPrivStByAdjust.pPortID[idx - 1]);

            pPrmEndRsp->err_index = MRP_REC_DATA_IDX_PRV_HSR_ADJUST;
            pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS;
            pPrmEndRsp->err_offset = MRP_REC_DATA_LEN_BLK_HEADER + MRP_REC_DATA_LEN_BLK_HEADER_PADDING + MRP_REC_DATA_LEN_ROLE + MRP_REC_DATA_LEN_RPORT;
            
            return MRP_RET_ERR;
          }
        }
        if (MRP_REC_DATA_HSR_ROLE_ARD == pRecInstPara->RecPrivHsrAdjust.Role) 
        {
          if (!pRecInstPara->RecInstanceAdjust.Written)
          {
            MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: ARD mode, but PDInterfaceMrpDataAdjust record missing", i);
            
            pPrmEndRsp->err_index = MRP_REC_DATA_IDX_IF_DATA_ADJUST;
            pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_NO_OR_TOO_LESS_PARAMS;

            return MRP_RET_ERR; 
          }
          else
          {
             if (pRecInstPara->adminRole != MRP_ADMIN_ROLE_AUTO_MGR)
             {
               MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: ard missing auto-mrm proj", i);
               
               pPrmEndRsp->err_index = MRP_REC_DATA_IDX_IF_DATA_ADJUST;
               pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS;
               pPrmEndRsp->err_offset = pRecInstPara->RecInstanceAdjust.err_offset_base + MRP_REC_DATA_LEN_BLK_HEADER + MRP_REC_DATA_LEN_BLK_HEADER_PADDING + MRP_REC_DATA_LEN_UUID;

               return MRP_RET_ERR;
             }
          }
          if (pRecInstPara->RPort_1 != pRecInstPara->RecPrivHsrAdjust.PortA && pRecInstPara->RPort_1 != pRecInstPara->RecPrivHsrAdjust.PortB)
          {
            MRP_UPPER_TRACE_04(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: ard ports (%u,%u) mismatch mrm port (%u)",
              i, pRecInstPara->RecPrivHsrAdjust.PortA, pRecInstPara->RecPrivHsrAdjust.PortB, pRecInstPara->RPort_1);
            
            pPrmEndRsp->err_index = MRP_REC_DATA_IDX_PORT_DATA_ADJUST;
            pPrmEndRsp->err_port_id = pRecInstPara->RPort_1;
            pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS;

            return MRP_RET_ERR;
          }
          if (pRecInstPara->RPort_2 != pRecInstPara->RecPrivHsrAdjust.PortA && pRecInstPara->RPort_2 != pRecInstPara->RecPrivHsrAdjust.PortB)
          {
            MRP_UPPER_TRACE_04(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: ard ports (%u,%u) mismatch mrm port (%u)",
              i, pRecInstPara->RecPrivHsrAdjust.PortA, pRecInstPara->RecPrivHsrAdjust.PortB, pRecInstPara->RPort_2);
            
            pPrmEndRsp->err_index = MRP_REC_DATA_IDX_PORT_DATA_ADJUST;
            pPrmEndRsp->err_port_id = pRecInstPara->RPort_2;
            pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS;

            return MRP_RET_ERR;
          }

          for (k = 0; k < mrpInMaxInstance; k++)
          {
            if (pRecInstPara->pRecIcPortAdjust[k].Written && pRecInstPara->pRecIcPortAdjust[k].Role != MRP_IC_PROJ_ROLE_OFF)
            {
              MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: cannot configure ARD mode and mrp interconnection together", i);

              pPrmEndRsp->err_index = MRP_REC_DATA_IDX_IC_PORT_DATA_ADJUST;
              pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS;

              return MRP_RET_ERR;
            }
          }
        }
        pRecInstPara->RPort_1 = pRecInstPara->RecPrivHsrAdjust.PortA;
        pRecInstPara->RPort_2 = pRecInstPara->RecPrivHsrAdjust.PortB;
      }
      /* stdby */
      if (pRecInstPara->RecPrivStByAdjust.Written && pRecInstPara->RecPrivStByAdjust.PortCnt) 
      {
        if (MRP_REC_DATA_HSR_ROLE_OFF == pRecInstPara->RecPrivHsrAdjust.Role   &&
            MRP_PROJ_ROLE_OFF == pRecInstPara->RecInstanceAdjust.Role) 
        {
          MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY[inst=%d]: cannot activate stdby without ring redundancy", i);

          pPrmEndRsp->err_index = MRP_REC_DATA_IDX_PRV_STBY_ADJUST;
          pPrmEndRsp->err_fault = MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS;

          return MRP_RET_ERR;
        }
      }
    }
#endif
  }

  return MRP_RET_OK;
}

/*===========================================================================
* FUNCTION : mrp_rec_prm_end
*----------------------------------------------------------------------------
* PURPOSE  : parameter end
*----------------------------------------------------------------------------
* RETURNS  : MRP_RET_OK, MRP_RET_ERR
*----------------------------------------------------------------------------
* INPUTS   : pChUsr     = user channel reference
*            pRQB       = upper rqb reference
* OUTPUTS  : 
*            err_port_id= EDD-PortID of the port related to the error
*            err_index  = index of the record that caused the error
*            err_offset = offset into "record_data" of record "err_index",
*                         specifying the position of an error within the record
*            err_fault  = Set to LSA_TRUE if the (normative) text “Wrong or too 
*                         many parameters are written” is helpful to track down 
*                         the problem. Set to LSA_FALSE if the (normative) text 
*                         “No or too less parameters are written” is helpful 
*                         to track down the problem.
*----------------------------------------------------------------------------
* COMMENTS : END followed by END is a sequence error (a bug in CM-PD).
*            RQB response:
*            MRP_RSP_OK
*            MRP_RSP_ERR_SEQUENCE
*            MRP_RSP_ERR_PRM_CONSISTENCY
*==========================================================================*/
LSA_UINT16 mrp_rec_prm_end (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB)
{
  LSA_UINT16 retVal, i, numOfPorts, portID;
  MRP_PRM_END_TYPE prmEndRsp;
  MRP_UPPER_ARGS_PTR_TYPE pArgs = MRP_RQB_GET_ARGS_PTR(pRQB);
  MRP_REC_DB_TYPE *pDBrec = (MRP_REC_DB_TYPE *)pChUsr->pChUsrDB->pRec;
  MRP_IF_DB_TYPE *pDBif   = (MRP_IF_DB_TYPE *)pChUsr->pChSysDB->pInterfacePara;
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChUsr->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  retVal         = MRP_RET_ERR;
  numOfPorts     = mrp_sys_get_num_of_ports (pDBrec->pChSys);
  
  MRP_UPPER_TRACE_00(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_prm_end entered");

  prmEndRsp.err_port_id  = MRP_PRM_ERR_PORT_ID_DEFAULT;
  prmEndRsp.err_offset   = MRP_PRM_ERR_OFFSET_DEFAULT;
  prmEndRsp.err_index    = MRP_PRM_ERR_INDEX_DEFAULT;
  prmEndRsp.err_fault    = MRP_PRM_ERR_FAULT_DEFAULT;

  MRP_REC_PRM_END_ARGS_ERR_OFFSET_SET (pArgs, prmEndRsp.err_offset);
  MRP_REC_PRM_END_ARGS_ERR_INDEX_SET (pArgs, prmEndRsp.err_index);
  MRP_REC_PRM_END_ARGS_ERR_PORT_ID_SET (pArgs, prmEndRsp.err_port_id);
  MRP_REC_PRM_END_ARGS_ERR_FAULT_SET (pArgs, prmEndRsp.err_fault);

  if (prmEndRsp.err_fault != MRP_PRM_ERR_FAULT_NO_OR_TOO_LESS_PARAMS && prmEndRsp.err_fault != MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS)
    retVal = MRP_RSP_ERR_PARAM; 

  /* if a port is PLUGGED, port params can always be applied in MRP -> portparamsNotApplicable is 0 in this case */
  MRP_MEMSET (&prmEndRsp.portparamsNotApplicable[0], MRP_PRM_PORT_PARAMS_APPLICABLE, (LSA_UINT32)(sizeof(LSA_UINT8) * numOfPorts));
  for (i = 0; i < numOfPorts; i++)
  {
    /* update the prmEndRsp structure with parameter isModularPort for each port */
    prmEndRsp.isModularPort[i] = pArgs->ctrl.prm_end.isModularPort[i];

    /* update the internal PortData array with parameter isModularPort for each port in any case - no matter, if positive or negative PRM_END acknowledgement */
    pDBif->pPortData[i].isModularPort = prmEndRsp.isModularPort[i];
  }

  if (pChUsr->detailptr->PathType == MRP_PATH_TYPE_USER  && pChUsr->detailptr->type.user.isPrmInterface == MRP_IS_PRM_INTERFACE_ON)
  {
    /* prm end ? */
    if (pDBrec->PrmEnd) 
    {
      MRP_UPPER_TRACE_00(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_SEQUENCE: PRM_END missing PRM_PREPARE, prmEndRsp.err = DEFAULT");

      MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_SEQUENCE);

      return retVal;
    }
    else
    {
      for (i = 0; i < numOfPorts; i++)
      {
        if (pDBif->pPortData[i].moduleState == MRP_PRM_PORTMODULE_PLUG_PREPARE)
        {
          if (prmEndRsp.isModularPort[i] == MRP_PORT_IS_MODULAR &&  /* modular port */
              (pDBif->pPortData[i].rpType != EDD_MRP_NO_RING_PORT ||
               pDBif->pPortData[i].inPortType == EDD_SUPPORTS_MRP_INTERCONN_PORT_CONFIG_YES))   /* respond with consistency error only, if the port could be configured as ringport or is a default ringport of interconnection port */
          {
            portID = i + 1;
            MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_SEQUENCE[port=%d]: PLUG_PREPARE must not be followed by PRM_END", portID);
            
            MRP_REC_PRM_END_ARGS_ERR_PORT_ID_SET (pArgs, portID);

            MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_SEQUENCE);

            return retVal;
          }
        }
      }
    }
    
    retVal = mrp_rec_check_prm_consistency(pChUsr, &prmEndRsp);
    
    for (i = 0; i < numOfPorts; i++)
    {   
      /* copy the (internal) parameter portparamsNotApplicable for each port to PRM_END response data structure */
      pArgs->ctrl.prm_end.portparamsNotApplicable[i] = prmEndRsp.portparamsNotApplicable[i];
    }

    if (retVal == MRP_RET_OK)
    {
      pDBrec->PrmEnd = LSA_TRUE;
      pDBrec->PrmState = MRP_REC_ENUM_STATE_PRM_END;

      MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_OK);

      MRP_UPPER_TRACE_00(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_prm_end completed successfully");

      mrp_usr_call_upper_cbf_proc_next_req (pChUsr, pRQB);    
    }
    else
    {
      MRP_REC_PRM_END_ARGS_ERR_OFFSET_SET (pArgs, prmEndRsp.err_offset);
      MRP_REC_PRM_END_ARGS_ERR_INDEX_SET (pArgs, prmEndRsp.err_index);
      MRP_REC_PRM_END_ARGS_ERR_PORT_ID_SET (pArgs, prmEndRsp.err_port_id);
      MRP_REC_PRM_END_ARGS_ERR_FAULT_SET (pArgs, prmEndRsp.err_fault);

      if (prmEndRsp.err_fault != MRP_PRM_ERR_FAULT_NO_OR_TOO_LESS_PARAMS && prmEndRsp.err_fault != MRP_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS)
        retVal = MRP_RSP_ERR_PARAM; 

      MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_PRM_CONSISTENCY);    

      MRP_UPPER_TRACE_04(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_CONSISTENCY: prmEndRsp.err_ind=0x%x, .err_port_id=%d, .err_offset=%d, .err_fault=%d", prmEndRsp.err_index,prmEndRsp.err_port_id, prmEndRsp.err_offset, prmEndRsp.err_fault);
    }
    return retVal;    
  }
  else
  {
    MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_SEQUENCE: invalid channel for PRM_END (pathType=%d, isPrmIf=%d), prmEndRsp.err = DEFAULT", pChUsr->detailptr->PathType, pChUsr->detailptr->type.user.isPrmInterface);
    MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_SEQUENCE);

    return retVal;
  }
}


/*===========================================================================
* FUNCTION : mrp_rec_prm_change_port
*----------------------------------------------------------------------------
* PURPOSE  : MRP is informed about a change of the module_state of a port
*----------------------------------------------------------------------------
* RETURNS  : MRP_RET_OK, MRP_RET_ERR
*----------------------------------------------------------------------------
* INPUTS   : pChUsr     = user channel reference
*            pRQB       = upper rqb reference
* OUTPUTS  : retVal     = error code
*----------------------------------------------------------------------------
* COMMENTS : If a pulled submodule is pulled again, this will not lead to an error.
*            If a plugged submodule is plugged again, this will not lead to an error.
*            RQB response:
*            MRP_RSP_OK
*            MRP_RSP_ERR_SEQUENCE
*==========================================================================*/
LSA_UINT16 mrp_rec_prm_change_port (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB)
{
  LSA_UINT16 retVal, numOfPorts;
  MRP_UPPER_ARGS_PTR_TYPE pArgs = MRP_RQB_GET_ARGS_PTR(pRQB);
  MRP_REC_DB_TYPE *pDBrec = (MRP_REC_DB_TYPE *)pChUsr->pChUsrDB->pRec;
  MRP_IF_DB_TYPE *pDBif   = (MRP_IF_DB_TYPE *)pChUsr->pChSysDB->pInterfacePara;
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChUsr->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  retVal         = MRP_RET_ERR;
  numOfPorts     = mrp_sys_get_num_of_ports(pChUsr);

  MRP_UPPER_TRACE_00(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_prm_change_port entered");

  if (pChUsr->detailptr->PathType == MRP_PATH_TYPE_USER  && pChUsr->detailptr->type.user.isPrmInterface == MRP_IS_PRM_INTERFACE_ON)
  {
    if (pChUsr->state != MRP_CH_STATE_READY) /* reject the service, if the PRM user channel is NOT in READY state */
    {
      MRP_UPPER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_SEQUENCE: PRM_CHANGE_PORT[state=%d] for port=%d called for channel in state=%d",
        pArgs->ctrl.prm_change_port.module_state, pArgs->ctrl.prm_change_port.edd_port_id, pChUsr->state);

      MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_SEQUENCE);

      return retVal;
    }
    else
    {
      /* the service must not be triggered between PRM_END and PRM_COMMIT */
      if (pDBrec->PrmState == MRP_REC_ENUM_STATE_PRM_END || pDBrec->PrmState == MRP_REC_ENUM_STATE_PRM_COMMIT_START) 
      {
        MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_SEQUENCE: PRM_CHANGE_PORT[state=%d] for port=%d called between PRM_END and PRM_COMMIT",
          pArgs->ctrl.prm_change_port.module_state, pArgs->ctrl.prm_change_port.edd_port_id);

        MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_SEQUENCE);

        return retVal;
      }
      if (pArgs->ctrl.prm_change_port.edd_port_id == 0 || pArgs->ctrl.prm_change_port.edd_port_id > numOfPorts)
      {
        MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PARAM: PRM_CHANGE_PORT called for invalid port=%d", pArgs->ctrl.prm_change_port.edd_port_id);

        MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_PARAM);

        return retVal;
      }

      switch (pDBif->pPortData[pArgs->ctrl.prm_change_port.edd_port_id -1].moduleState) /* moduleState of the port module, which was already stored in MRP */
      {
        case MRP_PRM_PORTMODULE_PULLED:
        {
          if (pArgs->ctrl.prm_change_port.module_state == MRP_PRM_PORTMODULE_PULLED)
          {
            MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "mrp_rec_prm_change_port: moduleState of port=%d NOT changed, because already in PULLED", 
                               pArgs->ctrl.prm_change_port.edd_port_id);
          }

          if (pArgs->ctrl.prm_change_port.module_state == MRP_PRM_PORTMODULE_PLUG_COMMIT)
          {
            MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_SEQUENCE: PRM_CHANGE_PORT[state=%d] for port=%d must not follow after PULLED", 
                               pArgs->ctrl.prm_change_port.module_state, pArgs->ctrl.prm_change_port.edd_port_id);

            MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_SEQUENCE);

            return retVal;
          }
        }
        break;

        case MRP_PRM_PORTMODULE_PLUG_PREPARE:
        {
          if (pArgs->ctrl.prm_change_port.module_state == MRP_PRM_PORTMODULE_PLUG_PREPARE)
          {
            MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "mrp_rec_prm_change_port: moduleState of port=%d NOT changed, because already in PLUG_PREPARE", 
                               pArgs->ctrl.prm_change_port.edd_port_id);
          }
          
          if (pArgs->ctrl.prm_change_port.module_state == MRP_PRM_PORTMODULE_PULLED)
          {
            MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_SEQUENCE: PRM_CHANGE_PORT[state=%d] for port=%d must not follow after PLUG_PREPARE", 
                               pArgs->ctrl.prm_change_port.module_state, pArgs->ctrl.prm_change_port.edd_port_id);

            MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_SEQUENCE);

            return retVal;
          }
        }
        break;

        case MRP_PRM_PORTMODULE_PLUG_COMMIT:
        {
          if (pArgs->ctrl.prm_change_port.module_state == MRP_PRM_PORTMODULE_PLUG_PREPARE)
          {
            MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_SEQUENCE: PRM_CHANGE_PORT[state=%d] for port=%d must not follow after PLUG_COMMIT", 
                               pArgs->ctrl.prm_change_port.module_state, pArgs->ctrl.prm_change_port.edd_port_id);

            MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_SEQUENCE);

            return retVal;
          }
        }
        break;

        default:
        {
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_rec_prm_change_port: PRM_CHANGE_PORT[state=%d] for port=%d unknown",
                             pArgs->ctrl.prm_change_port.module_state, pArgs->ctrl.prm_change_port.edd_port_id);
        }
        break;
      }

      retVal = MRP_RET_OK;
    }

    if (retVal == MRP_RET_OK)   /*lint !e774*/
    {
      /* store the new modul_state */
      pDBif->pPortData[pArgs->ctrl.prm_change_port.edd_port_id -1].moduleState = pArgs->ctrl.prm_change_port.module_state;

      if (pArgs->ctrl.prm_change_port.module_state == MRP_PRM_PORTMODULE_PLUG_PREPARE)
      {
        /* output parameter port_params_not_applicable: from MRP's point of view, port datasets are always applicable */
        pArgs->ctrl.prm_change_port.port_params_not_applicable = MRP_PRM_PORT_PARAMS_APPLICABLE;
      }
      else
      {
        if (pArgs->ctrl.prm_change_port.module_state == MRP_PRM_PORTMODULE_PLUG_COMMIT)
        {
          if (pArgs->ctrl.prm_change_port.apply_default_portparams == MRP_PRM_APPLY_DEFAULT_PORT_PARAMS)
          {
            MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "mrp_rec_prm_change_port: PRM_CHANGE_PORT[PLUG_COMMIT] for port=%d with apply_defaults=1 -> apply already received port datasets", 
                               pArgs->ctrl.prm_change_port.edd_port_id);
          }         
        }
      }

      MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_OK);

      MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_prm_change_port: port=%d changed to moduleState=%d", 
                         pArgs->ctrl.prm_change_port.edd_port_id, pArgs->ctrl.prm_change_port.module_state);

      mrp_usr_call_upper_cbf_proc_next_req (pChUsr, pRQB);    
    }
    else
    {
      MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_PRM_CONSISTENCY);    
    }
    return retVal;    
  }
  else
  {
    MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_SEQUENCE: service PRM_CHANGE_PORT only allowed for PRM user channel (pathType=%d, isPrmIf=%d)", pChUsr->detailptr->PathType, pChUsr->detailptr->type.user.isPrmInterface);
    MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_SYS_PATH);

    return retVal;
  }
}


/*===========================================================================
* FUNCTION : mrp_rec_stop_modules
*----------------------------------------------------------------------------
* PURPOSE  : stops any MRP/HSR module, which has to be stopped
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChUsr        = reference to usr channel
*            instance      = affected MRP/HSR instance
*            *pModuleState = pointer to module state structure
*            *pInModuleState = pointer to interconnection module state structure
* OUTPUTS  : *pRunningModules = current runinfo of any module
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
/*lint -e{818}*/
/* pModuleState is just manipulate with HRP module*/
LSA_VOID mrp_rec_stop_modules (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance, MRP_REC_MODULE_STATE_TYPE *pModuleState, MRP_REC_IN_MODULE_STATE_TYPE *pInModuleState, LSA_UINT32 *pRunningModules)
{
  LSA_UINT16 inInstance, mrpInMaxInstance;
  MRP_REC_DB_TYPE *pDBrec = (MRP_REC_DB_TYPE *)pChUsr->pChUsrDB->pRec;
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChUsr->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pDBrec->pChSys);

  /* only for MRP instance 0!!*/
  if(!instance)
  {
    /* reset general variable for MIC or MIM is running */
    *pRunningModules &= ~MRP_REC_DBG_ROLE_MIC;
    *pRunningModules &= ~MRP_REC_DBG_ROLE_MIM;
    
    /* Check for all MRP interconnection instances if their role as MIC or MIM needs to be stopped. */
    for (inInstance = 0; inInstance < mrpInMaxInstance; inInstance++)
    {
      /* Check all MRP interconnection instances if they are running as MIC and marked as to be stopped */
      if ((pInModuleState->moduleStateMIC[inInstance].stop)
#ifdef MRP_CFG_PLUGIN_0_MRM
          &&
          !(pModuleState[MRP_REC_ENUM_MODULE_STATE_MRM].stop && pModuleState[MRP_REC_ENUM_MODULE_STATE_MRM].roleChangeRestricted)
#endif
         )
      {
        MRP_UPPER_TRACE_01 (traceIdx, LSA_TRACE_LEVEL_NOTE, "***** >> stopping *** MIC[inInst=%d] ***", inInstance);
        mrp_in_mic_stop (pDBrec->pChSys, inInstance);

        pInModuleState->moduleStateMIC[inInstance].running = LSA_FALSE;
      }

      /* Check all MRP interconnection instances if they are running as MIM and marked as to be stopped */
      if ((pInModuleState->moduleStateMIM[inInstance].stop)
#ifdef MRP_CFG_PLUGIN_0_MRM
          &&
          !(pModuleState[MRP_REC_ENUM_MODULE_STATE_MRM].stop && pModuleState[MRP_REC_ENUM_MODULE_STATE_MRM].roleChangeRestricted)
#endif
         )
      {
        MRP_UPPER_TRACE_01 (traceIdx, LSA_TRACE_LEVEL_NOTE, "***** >> stopping *** MIM[inInst=%d] ***", inInstance);
        mrp_in_mim_stop (pDBrec->pChSys, inInstance);

        pInModuleState->moduleStateMIM[inInstance].running = LSA_FALSE;
      }

      if(pInModuleState->moduleStateMIC[inInstance].running == LSA_TRUE)
      {
        /* at least one MIC is still running*/
        *pRunningModules |= MRP_REC_DBG_ROLE_MIC;
      }
      if(pInModuleState->moduleStateMIM[inInstance].running == LSA_TRUE)
      {
        /* at least one MIM is still running*/
        *pRunningModules |= MRP_REC_DBG_ROLE_MIM;
      }
    }
  }

#ifdef MRP_CFG_PLUGIN_2_HSR
  if (pModuleState[MRP_REC_ENUM_MODULE_STATE_ARD].running && !pModuleState[MRP_REC_ENUM_MODULE_STATE_ARD].stop) 
  { 
    /* ard running => don't restart other modules */
    pModuleState[MRP_REC_ENUM_MODULE_STATE_HSR_RC].stop  = LSA_FALSE;
    pModuleState[MRP_REC_ENUM_MODULE_STATE_HSR_RC].start = LSA_FALSE;

#ifdef MRP_CFG_PLUGIN_0_MRM
    pModuleState[MRP_REC_ENUM_MODULE_STATE_MRM].stop = LSA_FALSE;
    pModuleState[MRP_REC_ENUM_MODULE_STATE_MRM].start = LSA_FALSE;
#endif
  }
  if (pModuleState[MRP_REC_ENUM_MODULE_STATE_ARD].stop)  
  { 
    /* AP00816761: if only ringports have been changed AND MRM is active AND morphing state will be entered, there's no need to stop/start ARD */
    if (!(pModuleState[MRP_REC_ENUM_MODULE_STATE_ARD].paramChanged && pModuleState[MRP_REC_ENUM_MODULE_STATE_MRM].roleChangeRestricted))
    {
      MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "***** >> stopping *** ARD[inst=%d] ***", instance);
      mrp_ard_stop (pDBrec->pChSys); 
      *pRunningModules &= ~MRP_REC_DBG_ROLE_ARD; 
    }
    else
    {
      MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "ARD[inst=%d] NOT stopped, because of ringport change AND morphing state", instance);
    }
  }
  if (pModuleState[MRP_REC_ENUM_MODULE_STATE_STBY].stop)  
  { 
    MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "***** >> stopping *** STDBY[inst=%d] ***", instance);
    mrp_stby_stop (pDBrec->pChSys);
    mrp_stby_reset_partner_var (pDBrec->pChSys);
    *pRunningModules &= ~MRP_REC_DBG_ROLE_STB; 

    if (!pModuleState[MRP_REC_ENUM_MODULE_STATE_STBY].start) 
    { 
      mrp_stby_ctrl (pDBrec->pChSys, LSA_FALSE);
    }
  }
  if (pModuleState[MRP_REC_ENUM_MODULE_STATE_HSR_RC].stop)  
  { 
    MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "***** >> stopping *** HSR Client[inst=%d] ***", instance);
    mrp_rc_stop (pDBrec->pChSys); 
    *pRunningModules &= ~MRP_REC_DBG_ROLE_HSR_CL; 
  }
  if (pModuleState[MRP_REC_ENUM_MODULE_STATE_HSR_MGR].stop)  
  { 
    MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "***** >> stopping *** HSR Mgr[inst=%d] ***", instance);
    mrp_mgr_stop (pDBrec->pChSys);           
    *pRunningModules &= ~MRP_REC_DBG_ROLE_HSR_MGR; 
  }
#endif
  if (pModuleState[MRP_REC_ENUM_MODULE_STATE_MRC].stop)  
  { 
    MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "***** >> stopping *** MRC[inst=%d] ***", instance);
    mrp_mrc_stop (pDBrec->pChSys, instance);  
    *pRunningModules &= ~MRP_REC_DBG_ROLE_MRC; 
  }
#ifdef MRP_CFG_PLUGIN_0_MRM
  if ((pModuleState[MRP_REC_ENUM_MODULE_STATE_MRM].stop  && !pModuleState[MRP_REC_ENUM_MODULE_STATE_MRM].roleChangeRestricted) ||
      (pModuleState[MRP_REC_ENUM_MODULE_STATE_MRM].stop  && pChUsr->state == MRP_CH_STATE_CLOSING)
     )
  { 
    MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "***** >> stopping *** MRM[inst=%d] ***", instance);
    mrp_mrm_stop (pDBrec->pChSys, instance); 
    *pRunningModules &= ~MRP_REC_DBG_ROLE_MRM; 
    *pRunningModules &= ~MRP_REC_DBG_ROLE_AUTO_MRM; 
  }
#endif
}
/*===========================================================================
* FUNCTION : mrp_rec_activate_role
*----------------------------------------------------------------------------
* PURPOSE  : activates role from record data
*            also the global structure 
*            "pDBrec->pInstData[instance].activeRedInModules" based on the 
*            MRP instance is updated
*----------------------------------------------------------------------------
* RETURNS  : LSA_UINT32 runningModules = any MRP/HSR modules running when 
*                                        returning from function
*----------------------------------------------------------------------------
* INPUTS   : pChUsr = reference to usr channel
*            instance = affected MRP/HSR instance
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
/*lint -e{818}*/
/* pChUsr is just manipulate with MRM module MRP_CFG_PLUGIN_0_MRM */
LSA_UINT32 mrp_rec_activate_role (const MRP_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance)
{
  MRP_REC_MODULE_STATE_TYPE moduleState[MRP_REC_ENUM_MODULE_STATE_COUNT];
  MRP_REC_IN_MODULE_STATE_TYPE inState;
  MRP_REC_MODULE_STATE_TYPE * pInModuleState = LSA_NULL;
  LSA_UINT32 moduleIdx, runningModules, runInfo;
  LSA_UINT8 anyModuleStarted;
  LSA_UINT8 checkUUID;
  MRP_REC_DB_INST_SET_TYPE *pRecInstWork;
  MRP_REC_DB_TYPE *pDBrec = (MRP_REC_DB_TYPE *)pChUsr->pChUsrDB->pRec;
  MRP_MRC_DB_TYPE *pDBmrc;
  LSA_UINT16 inInstance, mrpInMaxInstance;
#ifdef MRP_CFG_PLUGIN_2_HSR  
  LSA_UINT8 *pStdbyPortTblConf;
  LSA_UINT16 rc; 
  LSA_UINT16 numOfPorts;
#endif
  MRP_MRC_DB_TYPE *pDBmrcBase = (MRP_MRC_DB_TYPE *)pDBrec->pChSys->pChSysDB->pMrc;

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChUsr->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */
  
#ifdef MRP_CFG_PLUGIN_0_MRM
  LSA_UINT8 roleMgr   = LSA_FALSE;
  LSA_UINT8 roleChangeDelayed = LSA_FALSE;
  MRP_MRM_DB_TYPE *pDBmrm;

  MRP_MRM_DB_TYPE *pDBmrmBase = (MRP_MRM_DB_TYPE *)pDBrec->pChSys->pChSysDB->pMrm;

  MRP_REC_DB_INST_SET_TYPE *pRecSaveInstWork  = &pDBrec->pInstData[instance].saveWork;

  pDBmrm = &pDBmrmBase[instance];
#endif
  pDBmrc = &pDBmrcBase[instance];

  pRecInstWork     = &pDBrec->pInstData[instance].work;
  /* initialize the local variables with 0 */
  MRP_MEMSET (moduleState, 0, sizeof(MRP_REC_MODULE_STATE_TYPE)*MRP_REC_ENUM_MODULE_STATE_COUNT);
  MRP_MEMSET (&inState, 0, sizeof(MRP_REC_IN_MODULE_STATE_TYPE));

  mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pChUsr);

#ifdef MRP_CFG_PLUGIN_2_HSR
  numOfPorts = mrp_sys_get_num_of_ports(pDBrec->pChSys);

  MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)&pStdbyPortTblConf, (LSA_UINT16)(sizeof(LSA_UINT8) * numOfPorts));
  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pStdbyPortTblConf, LSA_NULL));
  /* preset zero */
  MRP_MEMSET(pStdbyPortTblConf, 0, (LSA_UINT16)(sizeof(LSA_UINT8) * numOfPorts));
#endif

  MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_activate_role entered[inst=%d]", instance);

  anyModuleStarted = LSA_FALSE;
  runningModules = 0;
  checkUUID = MRP_CHECK_UUID_OFF;

  for (moduleIdx = 0; moduleIdx < MRP_REC_ENUM_MODULE_STATE_COUNT; moduleIdx++) 
  {
    runInfo = 0;

    switch (moduleIdx) 
    {
#ifdef MRP_CFG_PLUGIN_2_HSR
      case MRP_REC_ENUM_MODULE_STATE_ARD:
      {
        mrp_ard_get_module_state(pChUsr, instance, &moduleState[moduleIdx]);

        if (moduleState[moduleIdx].running) 
        { 
          runInfo |= MRP_REC_DBG_ROLE_ARD; 
        }
      }
      break;

      case MRP_REC_ENUM_MODULE_STATE_STBY:
      {
        mrp_stby_get_module_state (pChUsr, instance, &moduleState[moduleIdx], pStdbyPortTblConf);
        
        if (moduleState[moduleIdx].running) 
        { 
          runInfo |= MRP_REC_DBG_ROLE_STB; 
        }
      }
      break;

      case MRP_REC_ENUM_MODULE_STATE_HSR_RC:
      {
        mrp_rc_get_module_state (pChUsr, instance, &moduleState[moduleIdx], moduleState[MRP_REC_ENUM_MODULE_STATE_ARD].stop);
                          
        if (moduleState[moduleIdx].running) 
        { 
          runInfo |= MRP_REC_DBG_ROLE_HSR_CL; 
        }
      }
      break;

      case MRP_REC_ENUM_MODULE_STATE_HSR_MGR:
      {
        mrp_mgr_get_module_state (pChUsr, instance, &moduleState[moduleIdx]);
      
        if (moduleState[moduleIdx].running) 
        { 
          runInfo |= MRP_REC_DBG_ROLE_HSR_MGR; 
        }
      }
      break;
    
#endif
      case MRP_REC_ENUM_MODULE_STATE_MRC:
      {
        mrp_mrc_get_module_state (pChUsr, instance, &moduleState[moduleIdx]);
                
        if (moduleState[moduleIdx].running) 
        { 
          runInfo |= MRP_REC_DBG_ROLE_MRC; 
        }
      }
      break;

#ifdef MRP_CFG_PLUGIN_0_MRM
      case MRP_REC_ENUM_MODULE_STATE_MRM:
      {
      
#ifdef MRP_CFG_PLUGIN_2_HSR
        mrp_mrm_get_module_state (pChUsr, instance, &roleMgr, &moduleState[moduleIdx], moduleState[MRP_REC_ENUM_MODULE_STATE_ARD].stop);
#else
        mrp_mrm_get_module_state (pChUsr, instance, &roleMgr, &moduleState[moduleIdx], 0);
#endif   
        if (moduleState[moduleIdx].running) 
        { 
          /* if MRM is running, it's not the provisioning which shall be started now (pRecInstWork), but the provisioning, which was started already before!!! */
          /* the previous provisioning is stored in pRecSaveInstWork */
          if (pRecSaveInstWork->adminRole == MRP_ADMIN_ROLE_AUTO_MGR)
          {
            runInfo |= MRP_REC_DBG_ROLE_AUTO_MRM; 
          }
          else
            runInfo |= MRP_REC_DBG_ROLE_MRM; 
        }
      }
      break;
#endif
      case MRP_REC_ENUM_MODULE_STATE_MIC: 
      {
        /* only MRP instance 0 can have MRP_IN instances */
        if (!instance)
        {
          /* Get the status of all MRP_IN instances. This is needed to decide if an instance needs
            to be stopped, started or if the configuration changed (stop => change config => start again). */
          for (inInstance = 0; inInstance < mrpInMaxInstance; inInstance++)
          {
            mrp_in_mic_get_module_state(pChUsr, instance, inInstance, &inState.moduleStateMIC[inInstance], (moduleState[MRP_REC_ENUM_MODULE_STATE_MRC].stop /* the MRP stop state will also stop the MRP_IN instances*/
#ifdef MRP_CFG_PLUGIN_0_MRM
                                        | moduleState[MRP_REC_ENUM_MODULE_STATE_MRM].stop
#endif
                                       ));

            /* General MIC/MIM module state: at least one MIC/MIM is running/started*/
            moduleState[moduleIdx].running  |= inState.moduleStateMIC[inInstance].running;
            moduleState[moduleIdx].start    |= inState.moduleStateMIC[inInstance].start;
          
            if (inState.moduleStateMIC[inInstance].running)
            {
              /* at least one MIC is running*/
              runInfo |= MRP_REC_DBG_ROLE_MIC;
            }
          }
        }
      }
      break;

      case MRP_REC_ENUM_MODULE_STATE_MIM: 
      {
        /* only MRP instance 0 can have MRP_IN instances */
        if (!instance)
        {
          /* Get the status of all MRP_IN instances. This is needed to decide if an instance needs
            to be stopped, started or if the configuration changed (stop => change config => start again). */
          for (inInstance = 0; inInstance < mrpInMaxInstance; inInstance++)
          {
            mrp_in_mim_get_module_state(pChUsr, instance, inInstance, &inState.moduleStateMIM[inInstance], (moduleState[MRP_REC_ENUM_MODULE_STATE_MRC].stop /* the MRP stop state will also stop the MRP_IN instances*/
#ifdef MRP_CFG_PLUGIN_0_MRM
                                        | moduleState[MRP_REC_ENUM_MODULE_STATE_MRM].stop
#endif
                                        ));

            /* General MIC/MIM module state: at least one MIC/MIM is running/started*/
            moduleState[moduleIdx].running  |= inState.moduleStateMIM[inInstance].running;
            moduleState[moduleIdx].start    |= inState.moduleStateMIM[inInstance].start;

            if (inState.moduleStateMIM[inInstance].running)
            {
              /* at least one MIM is running*/
              runInfo |= MRP_REC_DBG_ROLE_MIM;
            }
          }
        }
      }
      break;

    default:
      MRP_ASSERT(0);
      break;
    }

    /* save overall state of all modules */      
    anyModuleStarted |= moduleState[moduleIdx].start;
    runningModules |= runInfo; 
    
    MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "***mrp_rec_activate_role[inst=%d], modIdx=%d:***", instance, moduleIdx);

    if((moduleIdx == MRP_REC_ENUM_MODULE_STATE_MIC) || (moduleIdx == MRP_REC_ENUM_MODULE_STATE_MIM))
    {
      if (moduleIdx == MRP_REC_ENUM_MODULE_STATE_MIC)
      {
        pInModuleState  = &(inState.moduleStateMIC[0]);
      }
      else
      {
        pInModuleState  = &(inState.moduleStateMIM[0]);
      }

      for (inInstance = 0; inInstance < mrpInMaxInstance; inInstance++)
      {
        pInModuleState  = &pInModuleState[inInstance];
        MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "***mrp_rec_activate_role[inInst=%d], modIdx=%d:***", inInstance, moduleIdx);
        MRP_UPPER_TRACE_07(traceIdx, LSA_TRACE_LEVEL_NOTE, "conf=%d, running=%d, start=%d, stop=%d, paramChg=%d, rpChg=%d, roleChgRestrict=%d",
          pInModuleState->configured, pInModuleState->running, pInModuleState->start, pInModuleState->stop, pInModuleState->paramChanged, pInModuleState->rpChanged, pInModuleState->roleChangeRestricted);
      }
    }
    else
    {
      MRP_UPPER_TRACE_07(traceIdx, LSA_TRACE_LEVEL_NOTE, "conf=%d, running=%d, start=%d, stop=%d, paramChg=%d, rpChg=%d, roleChgRestrict=%d",
        moduleState[moduleIdx].configured, moduleState[moduleIdx].running, moduleState[moduleIdx].start, moduleState[moduleIdx].stop, moduleState[moduleIdx].paramChanged, moduleState[moduleIdx].rpChanged, moduleState[moduleIdx].roleChangeRestricted);
    }
  }

  /* save the MRP admin role to MRP MIB */
  switch (pRecInstWork->adminRole)
  {
#ifdef MRP_CFG_PLUGIN_1_MRM_EXT      
    case MRP_ADMIN_ROLE_AUTO_MGR:
      pDBmrm->edition = mrp_rec_get_mrp_edition(pDBrec);

      if (pDBmrm->edition == MRP_MRM_EDITION_2_COMPATIBLE)
        mrp_mib_write_admin_role (pDBrec->pChSys, instance, MRP_MIB_DOMAIN_ROLE_AUTO_MANAGER_COMP);
      else
        mrp_mib_write_admin_role (pDBrec->pChSys, instance, MRP_MIB_DOMAIN_ROLE_MANAGER);
      if (pRecInstWork->adminRole != pRecSaveInstWork->adminRole && !moduleState[MRP_REC_ENUM_MODULE_STATE_MRM].start)
      {
        /* update pDBmrm, because MRM is already active, but role changes between MRM <--> AutoMRM */
        pDBmrm->adminRole = pRecInstWork->adminRole;
        pDBmrm->operRole  = MRP_OPER_ROLE_AUTO_MGR;
      }
      break;
#endif
#ifdef MRP_CFG_PLUGIN_0_MRM      
    case MRP_ADMIN_ROLE_MGR:
      pDBmrm->edition = mrp_rec_get_mrp_edition(pDBrec);

      mrp_mib_write_admin_role (pDBrec->pChSys, instance, MRP_MIB_DOMAIN_ROLE_MANAGER);
      if (pRecInstWork->adminRole != pRecSaveInstWork->adminRole && !moduleState[MRP_REC_ENUM_MODULE_STATE_MRM].start)
      {
        /* update pDBmrm, because MRM is already active, but role changes between MRM <--> AutoMRM */
        pDBmrm->adminRole = pRecInstWork->adminRole;
        pDBmrm->operRole  = MRP_OPER_ROLE_MGR;
      }
    break;
#endif

    case MRP_ADMIN_ROLE_CLIENT:
      mrp_mib_write_admin_role (pDBrec->pChSys, instance, MRP_MIB_DOMAIN_ROLE_CLIENT);
    break;

    case MRP_ADMIN_ROLE_OFF:
      mrp_mib_write_admin_role (pDBrec->pChSys, instance, MRP_MIB_DOMAIN_ROLE_DISABLE);
    break;

    default:
      MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_rec_activate_role[inst=%d]: unknown adminRole=%d", instance, pRecInstWork->adminRole);
    break;
  }

  /*************** stop modules ****************************************/
  /* If the MRP instance 0 is marked as "to stop" also all MRP_IN instances are stopped before stopping the MRP instance 0 */
  /* Note: For each start the EDD service "EDD_SRV_MULTICAST" is called and a counter is increased. For each stop the same EDD service is 
    called and the counter is decreased. When the counter is 0 the forwarding to the host is stopped. */
  mrp_rec_stop_modules (pChUsr, instance, &moduleState[0], &inState, &runningModules);

  /*************** start modules ***************************************/
#ifdef MRP_CFG_PLUGIN_2_HSR
  if (moduleState[MRP_REC_ENUM_MODULE_STATE_ARD].start) 
  { 
    /* AP00816761: if only ringports have been changed AND MRM is active AND morphing state will be entered, there's no need to stop/start ARD */
    if (!(moduleState[MRP_REC_ENUM_MODULE_STATE_ARD].paramChanged && moduleState[MRP_REC_ENUM_MODULE_STATE_MRM].roleChangeRestricted) &&
        !(moduleState[MRP_REC_ENUM_MODULE_STATE_MRM].paramChanged && moduleState[MRP_REC_ENUM_MODULE_STATE_MRM].rpChanged && moduleState[MRP_REC_ENUM_MODULE_STATE_MRM].roleChangeRestricted)
       )
    {
      MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "***** >> starting *** ARD[inst=%d] ***", instance);
      
      /* if Auto-MRM is activated */
      if (moduleState[MRP_REC_ENUM_MODULE_STATE_MRM].configured && moduleState[MRP_REC_ENUM_MODULE_STATE_MRM].running)
      {
        if (!roleMgr && !moduleState[MRP_REC_ENUM_MODULE_STATE_MRM].start)
          mrp_ard_start (pDBrec->pChSys, pRecInstWork->RPort_1, pRecInstWork->RPort_2, LSA_TRUE);
        else
          mrp_ard_start (pDBrec->pChSys, pRecInstWork->RPort_1, pRecInstWork->RPort_2, LSA_FALSE); 
      }
      else
      {
        mrp_ard_start (pDBrec->pChSys, pRecInstWork->RPort_1, pRecInstWork->RPort_2, LSA_FALSE); 
      }
      runningModules |= MRP_REC_DBG_ROLE_ARD;
    }
    else
    {
      MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "ARD[inst=%d] NOT started, because of ringport change AND morphing state", instance);
    }
  }
  if (moduleState[MRP_REC_ENUM_MODULE_STATE_HSR_RC].start && !moduleState[MRP_REC_ENUM_MODULE_STATE_MRM].roleChangeRestricted) 
  { 
    MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "***** >> starting *** HSR client[inst=%d] ***", instance);
    mrp_rc_start (pDBrec->pChSys, pRecInstWork->RecPrivHsrAdjust.PortA, pRecInstWork->RecPrivHsrAdjust.PortB); 
    runningModules |= MRP_REC_DBG_ROLE_HSR_CL;
  }
  if (moduleState[MRP_REC_ENUM_MODULE_STATE_HSR_MGR].start && !moduleState[MRP_REC_ENUM_MODULE_STATE_MRM].roleChangeRestricted) 
  { 
    MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "***** >> starting *** HSR manager[inst=%d] ***", instance);
    mrp_mgr_start (pDBrec->pChSys, pRecInstWork->RecPrivHsrAdjust.PortA, pRecInstWork->RecPrivHsrAdjust.PortB); 
    runningModules |= MRP_REC_DBG_ROLE_HSR_MGR;
  }
  if (moduleState[MRP_REC_ENUM_MODULE_STATE_STBY].start && !moduleState[MRP_REC_ENUM_MODULE_STATE_MRM].roleChangeRestricted) 
  { 
    MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "***** >> starting *** STANDBY[inst=%d] ***", instance);
    mrp_stby_start (pDBrec->pChSys, pRecInstWork->RecPrivStByAdjust.ConName, LSA_NULL, 0, pStdbyPortTblConf, pRecInstWork->RecPrivStByAdjust.Capability, pRecInstWork->RecPrivStByAdjust.WaitForPartner);
    mrp_stby_ctrl (pDBrec->pChSys, LSA_TRUE);
    runningModules |= MRP_REC_DBG_ROLE_STB;
  }
  MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pStdbyPortTblConf);
  MRP_FATAL1(rc == LSA_RET_OK);

#endif
  if (moduleState[MRP_REC_ENUM_MODULE_STATE_MRC].start
#ifdef MRP_CFG_PLUGIN_0_MRM
      && !moduleState[MRP_REC_ENUM_MODULE_STATE_MRM].roleChangeRestricted
#endif
     ) 
  { 
    MRP_MRC_PARAMS_TYPE params;

    MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "***** >> starting *** MRC[inst=%d] ***", instance);
    params.RPort_1    = pRecInstWork->RPort_1;
    params.RPort_2    = pRecInstWork->RPort_2;
    params.PRM_RPort  = params.RPort_1;
    params.SEC_RPort  = params.RPort_2;

    params.Mode       = MRP_OPERATION_ON;
    params.SetDefaults= MRP_SET_DEFAULTS_OFF;
    MRP_MEMCPY(params.DomainUUID, pRecInstWork->RecInstanceAdjust.DomainUUID, sizeof(params.DomainUUID));
    params.LNKdownT_ms = pRecInstWork->RecInstanceAdjust.client.LNKdownT_ms;
    params.LNKupT_ms   = pRecInstWork->RecInstanceAdjust.client.LNKupT_ms;
    params.LNKNRmax    = pRecInstWork->RecInstanceAdjust.client.LNKNRmax;
    params.CheckUUID   = pRecInstWork->RecInstanceUuidCheck.Check & MRP_REC_DATA_CHECK_UUID ? MRP_CHECK_UUID_ON : MRP_CHECK_UUID_OFF;

    mrp_mrc_start (pDBrec->pChSys, instance, &params); 
    runningModules |= MRP_REC_DBG_ROLE_MRC;
  }
#ifdef MRP_CFG_PLUGIN_0_MRM
  if (moduleState[MRP_REC_ENUM_MODULE_STATE_MRM].start || moduleState[MRP_REC_ENUM_MODULE_STATE_MRM].roleChangeRestricted) 
  { 
    MRP_MRM_PARAMS_TYPE params;

    params.RPort_1    = pRecInstWork->RPort_1;
    params.RPort_2    = pRecInstWork->RPort_2;
    params.PRM_RPort  = params.RPort_1;
    params.SEC_RPort  = params.RPort_2;

    params.Mode       = MRP_OPERATION_ON;
    params.SetDefaults= MRP_SET_DEFAULTS_OFF;
    params.SoftStart  = MRP_SET_SOFT_START_OFF;
    params.edition    = mrp_rec_get_mrp_edition (pDBrec);
    
    params.adminRole  = pRecInstWork->adminRole;

    if (moduleState[MRP_REC_ENUM_MODULE_STATE_MRM].start)
    {
      if (params.adminRole == MRP_ADMIN_ROLE_AUTO_MGR)
        params.operRole  = MRP_OPER_ROLE_AUTO_MGR;
      else
        params.operRole  = MRP_OPER_ROLE_MGR;      
    }
    else /* roleChangeRestricted == LSA_TRUE */
    {
      params.operRole = pDBmrm->operRole;
    }

    MRP_MEMCPY(params.DomainUUID, pRecInstWork->RecInstanceAdjust.DomainUUID, sizeof(params.DomainUUID));
    params.Prio          = pRecInstWork->RecInstanceAdjust.mgr.Prio;
    params.TOPchgT_10ms  = pRecInstWork->RecInstanceAdjust.mgr.TOPchgT_10ms;
    params.TOPNRmax      = pRecInstWork->RecInstanceAdjust.mgr.TOPNRmax;
    params.TSTshortTms   = pRecInstWork->RecInstanceAdjust.mgr.TSTshortTms;
    params.TSTdefaultTms = pRecInstWork->RecInstanceAdjust.mgr.TSTdefaultTms;
    params.TSTNRmax      = pRecInstWork->RecInstanceAdjust.mgr.TSTNRmax;
#ifdef MRP_CFG_PLUGIN_1_MRM_EXT
    params.MONdefaultT_ms= pRecInstWork->RecPrivDataAdjust.MONdefaultT_ms;
    params.MONNRmax      = pRecInstWork->RecPrivDataAdjust.MONNRmax;
#endif
    params.CheckUUID     = pRecInstWork->RecInstanceUuidCheck.Check & MRP_REC_DATA_CHECK_UUID ? MRP_CHECK_UUID_ON : MRP_CHECK_UUID_OFF;
    if (moduleState[MRP_REC_ENUM_MODULE_STATE_MRM].roleChangeRestricted) 
    {
      if ((moduleState[MRP_REC_ENUM_MODULE_STATE_MRM].stop && !moduleState[MRP_REC_ENUM_MODULE_STATE_MRM].start)
        || moduleState[MRP_REC_ENUM_MODULE_STATE_MRC].start) 
      {
        /* change to MRC or OFF */
        if (moduleState[MRP_REC_ENUM_MODULE_STATE_MRC].start) 
        {
          MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "***** >> starting *** MRC[inst=%d] requested ***", instance);
        }
        else 
        {
          /* MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE,"***** >> starting *** NOTHING *** (running 0x%x)",MRP_REC_DBG_ROLE_MRM); */
        }
        roleChangeDelayed = mrp_mrm_morphing (pDBrec->pChSys, instance, LSA_NULL, pChUsr);
      }
      else 
      {
        roleChangeDelayed = mrp_mrm_morphing (pDBrec->pChSys, instance, &params, pChUsr);
      }

      if (roleChangeDelayed)
      {
        MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_activate_role: MRM[inst=%d] role change DEFERRED", instance);
        mrp_rec_restore_saved_settings (pChUsr, instance);
          /* enable saved alarm settings here, because previous alarm provisioning is still valid as long as MRP ring is closed*/
        mrp_rec_restore_alarm_settings (pChUsr, instance);

        if (pDBmrm->CheckUUID == MRP_CHECK_UUID_ON)
          mrp_oha_multi_usr_write_curr_ringstate_data (pDBrec->pChSys, instance, MRP_USR_ENUM_TOPO_CTRL_ADD_CHECK);
        else
          mrp_oha_multi_usr_write_curr_ringstate_data (pDBrec->pChSys, instance, MRP_USR_ENUM_TOPO_CTRL_ADD);

        mrp_oha_multi_usr_ringstate_ind_done (pDBrec->pChSys, instance);
      }
      else
      {
        MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_activate_role: MRM[inst=%d] role change EXECUTED, no morphing", instance);

        /* if MRM is already active, but role changes between MRM <--> AUTO_MRM, retrigger RED_STATE_INDICATION, too */
        if (params.adminRole == MRP_ADMIN_ROLE_AUTO_MGR)
        {
          mrp_usr_write_red_start_state (pDBrec->pChSys, instance, MRP_USR_ENUM_STATE_MRM, params.RPort_1, params.RPort_2, LSA_NULL, LSA_TRUE);
          mrp_usr_write_red_start_state (pDBrec->pChSys, instance, MRP_USR_ENUM_STATE_AUTO_MRM, params.RPort_1, params.RPort_2, LSA_NULL, LSA_TRUE);
          mrp_ll_red_state_handler (pDBrec->pChSys, instance);
        }
        else
        {
          if (params.adminRole == MRP_ADMIN_ROLE_MGR)
          {
            mrp_usr_write_red_start_state (pDBrec->pChSys, instance, MRP_USR_ENUM_STATE_MRM, params.RPort_1, params.RPort_2, LSA_NULL, LSA_TRUE);
            mrp_usr_write_red_start_state (pDBrec->pChSys, instance, MRP_USR_ENUM_STATE_AUTO_MRM, params.RPort_1, params.RPort_2, LSA_NULL, LSA_FALSE);
            mrp_ll_red_state_handler (pDBrec->pChSys, instance);
          }
        }
      }
    }
    else
    {
      MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_activate_role[inst=%d]: role change NOT restricted", instance);
      MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "***** >> starting *** MRM[inst=%d] ***", instance);

      if (params.adminRole == MRP_ADMIN_ROLE_AUTO_MGR)
      {
        mrp_usr_write_red_start_state (pDBrec->pChSys, instance, MRP_USR_ENUM_STATE_AUTO_MRM, params.RPort_1, params.RPort_2, LSA_NULL, LSA_TRUE);
        mrp_ll_red_state_handler (pDBrec->pChSys, instance);
      }
      else
      {
        if (params.adminRole == MRP_ADMIN_ROLE_MGR)
        {
          mrp_usr_write_red_start_state (pDBrec->pChSys, instance, MRP_USR_ENUM_STATE_AUTO_MRM, params.RPort_1, params.RPort_2, LSA_NULL, LSA_FALSE);
          mrp_ll_red_state_handler (pDBrec->pChSys, instance);
        }
      }
      mrp_mrm_start (pDBrec->pChSys, instance, &params); 
      if (params.adminRole == MRP_ADMIN_ROLE_AUTO_MGR) 
        runningModules |= MRP_REC_DBG_ROLE_AUTO_MRM;
      else
        runningModules |= MRP_REC_DBG_ROLE_MRM;
      
      if (moduleState[MRP_REC_ENUM_MODULE_STATE_MRC].stop)
      {
        /* ### 21.07.2011: review this!!! */
        /* if MRC was stopped before, then force a topology change to be sent during MRM initialization!! */
        MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_activate_role[inst=%d]: force sending of a topology_change_req, because MRC has just been stopped", instance);

        mrp_mrm_topology_change_req (pDBmrm);
      }
    }
  }
#endif

  /* only MRP instance 0 can have MRP_IN instances */
  if (!instance)
  {
    /* activate all In instances */
    for (inInstance = 0; inInstance < mrpInMaxInstance; inInstance++)
    {
      /* check all MIC instances if they need to be started */
      if (inState.moduleStateMIC[inInstance].start 
#ifdef MRP_CFG_PLUGIN_0_MRM
         && !roleChangeDelayed 
#endif
      )
      {
        MRP_IN_MIC_PARAMS_TYPE params;

        MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "***** >> starting *** MIC[inInst=%d] ***", inInstance);

        params.adminRole         = (MRP_IN_ADMIN_ROLE_ENUM_T)pRecInstWork->pRecIcPortAdjust[inInstance].Role;
        params.operRole          = MRP_IN_OPER_ROLE_MIC;

        params.InID              = pRecInstWork->pRecIcPortAdjust[inInstance].DomainID;
        params.IPort             = pRecInstWork->pRecIcPortAdjust[inInstance].PortID;

        params.MRPIC_LNKdownT_ms = pRecInstWork->pRecIcPortAdjust[inInstance].mic.LNKdownT_ms;
        params.MRPIC_LNKNRmax    = pRecInstWork->pRecIcPortAdjust[inInstance].mic.LNKNRmax;
        params.MRPIC_LNKupT_ms   = pRecInstWork->pRecIcPortAdjust[inInstance].mic.LNKupT_ms;

        params.MRPIC_StartDelay_100ms = pRecInstWork->pRecIcPortAdjust[inInstance].mic.StartDelay_100ms;
        params.MRPIC_MICPosition      = pRecInstWork->pRecIcPortAdjust[inInstance].mic.MICPosition;

        /* the DomainUUID valid for the MRP interconnection instance is the DomainUUID provisioned with the record for the MRP instance */
        MRP_MEMCPY (params.DomainUUID, pRecInstWork->RecInstanceAdjust.DomainUUID, sizeof (params.DomainUUID));

        params.SetDefaults      = MRP_SET_DEFAULTS_OFF;
        params.SoftStart        = MRP_SET_SOFT_START_OFF;

        params.MRPIC_CheckDomainID = pRecInstWork->pRecIcPortCheck[inInstance].Check & MRP_REC_IC_PORT_DATA_CHECK_DOMAINID ? MRP_IC_CHECK_DOMAINID_ON : MRP_IC_CHECK_DOMAINID_OFF;

        mrp_in_mic_start (pDBrec->pChSys, inInstance, &params); 

        /* at least one MIC is running*/
        runningModules |= MRP_REC_DBG_ROLE_MIC;
      }

      /* check all MIM instances if they need to be started */
      if (inState.moduleStateMIM[inInstance].start 
#ifdef MRP_CFG_PLUGIN_0_MRM
          && !roleChangeDelayed
#endif
          )
      {
        MRP_IN_MIM_PARAMS_TYPE params;

        MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "***** >> starting *** MIM[inInst=%d] ***", inInstance);

        params.adminRole = (MRP_IN_ADMIN_ROLE_ENUM_T)pRecInstWork->pRecIcPortAdjust[inInstance].Role;
        params.operRole  = MRP_IN_OPER_ROLE_MIM;

        params.InID      = pRecInstWork->pRecIcPortAdjust[inInstance].DomainID;
        params.IPort     = pRecInstWork->pRecIcPortAdjust[inInstance].PortID;

        params.MRPIC_LinkStatusChangeT_ms = pRecInstWork->pRecIcPortAdjust[inInstance].mim.LinkStatusChangeT_ms;
        params.MRPIC_LinkStatusNRmax      = pRecInstWork->pRecIcPortAdjust[inInstance].mim.LinkStatusNRmax;
        params.MRPIC_StartDelay_100ms     = pRecInstWork->pRecIcPortAdjust[inInstance].mim.StartDelay_100ms;
        params.MRPIC_TOPchgT_10ms         = pRecInstWork->pRecIcPortAdjust[inInstance].mim.TOPchgT_10ms;
        params.MRPIC_TOPNRmax             = pRecInstWork->pRecIcPortAdjust[inInstance].mim.TOPNRmax;

        /* the DomainUUID valid for the MRP interconnection instance is the DomainUUID provisioned with the record for the MRP instance */
        MRP_MEMCPY (params.DomainUUID, pRecInstWork->RecInstanceAdjust.DomainUUID, sizeof (params.DomainUUID));

        params.SetDefaults                = MRP_SET_DEFAULTS_OFF;
        params.SoftStart                  = MRP_SET_SOFT_START_OFF;

        params.MRPIC_CheckDomainID = pRecInstWork->pRecIcPortCheck[inInstance].Check & MRP_REC_IC_PORT_DATA_CHECK_DOMAINID ? MRP_IC_CHECK_DOMAINID_ON : MRP_IC_CHECK_DOMAINID_OFF;

        mrp_in_mim_start (pDBrec->pChSys, inInstance, &params); 

        /* at least one MIC is running*/
        runningModules |= MRP_REC_DBG_ROLE_MIM;
      }
    }
  }

  /* config topo control */
  if ( (moduleState[MRP_REC_ENUM_MODULE_STATE_MRC].topoCtrl != MRP_USR_ENUM_TOPO_CTRL_NONE && 
        moduleState[MRP_REC_ENUM_MODULE_STATE_MRC].running && (!moduleState[MRP_REC_ENUM_MODULE_STATE_MRC].stop && !moduleState[MRP_REC_ENUM_MODULE_STATE_MRC].start)))
  {
    if (moduleState[MRP_REC_ENUM_MODULE_STATE_MRC].topoCtrl == MRP_USR_ENUM_TOPO_CTRL_ADD_CHECK)
      checkUUID = MRP_CHECK_UUID_ON;

    pDBmrc->CheckUUID = checkUUID;

    mrp_edd_req_oha_topo_change_ind (pDBrec->pChSys, instance, checkUUID);        
  }
#ifdef MRP_CFG_PLUGIN_0_MRM
  if ( (moduleState[MRP_REC_ENUM_MODULE_STATE_MRM].topoCtrl != MRP_USR_ENUM_TOPO_CTRL_NONE &&
        moduleState[MRP_REC_ENUM_MODULE_STATE_MRM].running && (!moduleState[MRP_REC_ENUM_MODULE_STATE_MRM].stop && !moduleState[MRP_REC_ENUM_MODULE_STATE_MRM].start)))
  {
    if (moduleState[MRP_REC_ENUM_MODULE_STATE_MRM].topoCtrl == MRP_USR_ENUM_TOPO_CTRL_ADD_CHECK)
      checkUUID = MRP_CHECK_UUID_ON;
         
    mrp_mrm_set_check_uuid (pDBmrm, checkUUID);
      
    mrp_edd_req_oha_topo_change_ind (pDBrec->pChSys, instance, checkUUID);
  }
#endif

  if (!anyModuleStarted) 
  {
    /* running see MRP_REC_DBG_ROLE_ARD,... */
    MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "***** >> mrp_rec_activate_role[inst=%d] starting *** NOTHING *** (runningModules=0x%x)", instance, runningModules);
    
    if (!runningModules)
    {
      mrp_usr_write_red_start_state (pDBrec->pChSys, instance, MRP_USR_ENUM_STATE_ALL, 0, 0, LSA_NULL, LSA_FALSE);
      mrp_ll_red_state_handler (pDBrec->pChSys, instance);
      mrp_oha_multi_usr_write_curr_ringstate_data (pDBrec->pChSys, instance, MRP_USR_ENUM_TOPO_CTRL_NONE);
    }

    mrp_oha_multi_usr_ringstate_ind_done (pDBrec->pChSys, instance);
  }
  else
  {
    MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "***** >> mrp_rec_activate_role[inst=%d] runningModules=0x%x", instance, runningModules);
  }

  mrp_ll_event_handler (pDBrec->pChSys, instance);

  return runningModules;
}


/*===========================================================================
* FUNCTION : mrp_rec_prm_commit
*----------------------------------------------------------------------------
* PURPOSE  : parameter commit replace active record set with parameter 
*            record set
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChUsr=user channel reference
*            pRQB=upper rqb reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : COMMIT without previous END causes a sequence error (a bug in CM-PD).
*            COMMIT in case of inconsistency causes a sequence error (a bug in CM-PD).
*            
*            NOTE:
*            This function is used as central supervision function for MRP functionality!!
*
*            RQB response:
*            MRP_RSP_OK
*            MRP_RSP_ERR_SEQUENCE
*==========================================================================*/
LSA_VOID mrp_rec_prm_commit (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB)
{
  LSA_UINT16 numOfPorts, mrpMaxInstance, instance, mrpInMaxInstance, inInstance;
  LSA_UINT8 runFlag;
  MRP_REC_DB_INST_SET_TYPE *pRecInstPara, *pRecInstWork;
  MRP_UPPER_ARGS_PTR_TYPE pArgs = MRP_RQB_GET_ARGS_PTR(pRQB);
  MRP_REC_DB_TYPE *pDBrec = (MRP_REC_DB_TYPE *)pChUsr->pChUsrDB->pRec;
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChUsr->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  numOfPorts     = mrp_sys_get_num_of_ports (pDBrec->pChSys);
  mrpMaxInstance = mrp_sys_get_num_of_instances(pChUsr);
  mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pDBrec->pChSys);
  
  MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_prm_commit entered, mrpMaxInstance=%d, numOfPorts=%d", mrpMaxInstance, numOfPorts);

  if (pChUsr->detailptr->PathType == MRP_PATH_TYPE_USER && pChUsr->detailptr->type.user.isPrmInterface == MRP_IS_PRM_INTERFACE_ON)
  {
    if (pChUsr->state == MRP_CH_STATE_CLOSING) 
    {
      /* shutdown */
      pDBrec->PrmEnd = LSA_TRUE;
      pDBrec->PrmState = MRP_REC_ENUM_STATE_PRM_END;

      mrp_rec_set_if_para_defaults(pDBrec);
      for (instance = 0; instance < mrpMaxInstance; instance++)
      {
        mrp_rec_set_inst_para_defaults (pDBrec, instance, MRP_REC_ENUM_DEFAULT_FROM_STD);
      }    
    }

    if (!pDBrec->PrmEnd) 
    {
      MRP_UPPER_TRACE_00(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_SEQUENCE: PRM_COMMIT missing PRM_END");
      MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_SEQUENCE);

      return;
    }

    /* set defaults */
    MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_OK);
    
      
    if (pChUsr->state != MRP_CH_STATE_CLOSING) 
    {
      LSA_INT32 idx;
      LSA_UINT8  written[EDD_CFG_MAX_MRP_INSTANCE_CNT];
      
      for (instance = 0; instance < EDD_CFG_MAX_MRP_INSTANCE_CNT; instance++)
        written[instance] = LSA_FALSE;

      if (!pDBrec->para.RecIfAdjustHead.Written)
      {
        mrp_rec_set_if_para_defaults(pDBrec);
      }
      for (instance = 0; instance < mrpMaxInstance; instance++)
      {
        pRecInstPara = &pDBrec->pInstData[instance].para;
  
        /* first check if any DS is written */
        written[instance] = LSA_FALSE;
        for (idx = 0; idx < MRP_DEFAULT_VAL_INST_RPORT_CNT; idx++) 
        {
          if (pRecInstPara->RecPortAdjust[idx].Written) 
          {
            written[instance] = LSA_TRUE;

            if (pRecInstPara->RecPortAdjust[idx].PortID && pArgs->ctrl.prm_commit.applyDefaultPortparams[pRecInstPara->RecPortAdjust[idx].PortID - 1] == MRP_PRM_APPLY_DEFAULT_PORT_PARAMS)
            {
              MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "mrp_rec_prm_commit[inst=%d]: PortAdjust WRITTEN, but apply DEFAULTs requested for port=%d", instance, pRecInstPara->RecPortAdjust[idx].PortID);
            }
            else
            {
              MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_prm_commit[inst=%d]: PortAdjust WRITTEN, apply record params requested for port=%d", instance, pRecInstPara->RecPortAdjust[idx].PortID);
            }
          }
        }
        if (pRecInstPara->RecInstanceAdjust.Written) 
        {
          written[instance] = LSA_TRUE;
        }
        if (pRecInstPara->RecInstanceCheck.Written) 
        {
          written[instance] = LSA_TRUE;
        }
        if (instance == MRP_DEFAULT_INSTANCE) /* only for MRP instance "MRP_DEFAULT_INSTANCE" MRP interconnection data exists! */
        {
          for (inInstance = 0; inInstance < mrpInMaxInstance; inInstance++)
          {
            if (pRecInstPara->pRecIcPortAdjust[inInstance].Written)
            {
              written[instance] = LSA_TRUE;
            }
            if (pRecInstPara->pRecIcPortCheck[inInstance].Written)
            {
                written[instance] = LSA_TRUE;
            }
          }
        }
        /* private data n/a
        if (pRecInstPara->RecPrivDataAdjust.Written) 
        {
          written=LSA_TRUE;
        }
        */
#ifdef MRP_CFG_PLUGIN_2_HSR
        if (pRecInstPara->RecPrivHsrAdjust.Written) 
        {
          written[instance] = LSA_TRUE;
        }
        if (pRecInstPara->RecPrivStByAdjust.Written) 
        {
          written[instance] = LSA_TRUE;
        }
#endif
        if (!written[instance]) 
        {
          MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_prm_commit[inst=%d]: no datasets received, set defaults from detailptr", instance);
          /* no configuration received, use defaults from detailptr */
          mrp_rec_set_inst_para_defaults (pDBrec, instance, MRP_REC_ENUM_DEFAULT_FROM_IF_DETAILS);
        }
      }

#ifdef MRP_CFG_PLUGIN_0_MRM
      for (instance = 0; instance < mrpMaxInstance; instance++)
      {
        /* receiving a prm_commit for a new provisioning stops the morphing timer, because          */
        /* the decision whether to keep morphing state is made in function mrp_rec_activate_role    */
        mrp_mrm_reset_morphing (pDBrec->pChSys, instance);
      }
#endif

      mrp_rec_set_if_para2work(pDBrec);
      for (instance = 0; instance < mrpMaxInstance; instance++)
      {
        mrp_rec_set_inst_para2work (pDBrec, instance);
      }
      
      mrp_rec_set_if_para_defaults(pDBrec);

      /* reset alarms */
      MRP_MEMSET(&pDBrec->commitCnf, 0, sizeof(pDBrec->commitCnf));
      MRP_MEMSET(&pDBrec->commitCnfIndicated, 0, sizeof(pDBrec->commitCnfIndicated));

      if (mrpMaxInstance)
      {
        for (instance = 0; instance < mrpMaxInstance; instance++)
        {
          pRecInstWork = &pDBrec->pInstData[instance].work;
        
          MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_prm_commit[inst=%d]: post processing, init para with defaults from std", instance);
          mrp_rec_set_inst_para_defaults (pDBrec, instance, MRP_REC_ENUM_DEFAULT_FROM_STD);
        
          /* reset alarms */
          MRP_MEMSET(&pDBrec->pInstData[instance].Alarm, 0, sizeof(pDBrec->pInstData[instance].Alarm));
          MRP_MEMSET(&pDBrec->pInstData[instance].AlarmIndicated, 0, sizeof(pDBrec->pInstData[instance].AlarmIndicated));
          MRP_MEMSET(pDBrec->pAlarmOutOfResource, MRP_ALARM_OUT_OF_RESOURCE_FALSE,(LSA_UINT16)(sizeof(LSA_UINT8) * (numOfPorts + 1)));
          pDBrec->pInstData[instance].AlarmBlocked = LSA_TRUE;

          /* set functions */
          pDBrec->pInstData[instance].activeRedModules = mrp_rec_activate_role (pChUsr, instance);
          
          runFlag = mrp_sys_cyclic_timer_state_check (0);
          if (runFlag)
          {
            MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_prm_commit[inst=%d]: cyclic comp. timer running", instance);
            /* start alarm timer and finish callback */
            mrp_sys_timer_stop (pDBrec->pInstData[instance].TimerIdAlarm);
            if (pChUsr->state != MRP_CH_STATE_CLOSING && (pRecInstWork->RecInstanceCheck.Check & MRP_REC_DATA_CHECK_MRM)) 
            {
              mrp_sys_timer_start (pDBrec->pInstData[instance].TimerIdAlarm, pDBrec->pInstData[instance].TimerAlarmTimeTcnt);
            }
          }
          else
          {
            MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_prm_commit[inst=%d]: cyclic comp. timer stopped", instance);
            /* stop alarm timer */
            mrp_sys_timer_stop (pDBrec->pInstData[instance].TimerIdAlarm);
          }
        }
      }
      else
      {
        MRP_MEMSET(pDBrec->pAlarmOutOfResource, MRP_ALARM_OUT_OF_RESOURCE_FALSE, (LSA_UINT16)(sizeof(LSA_UINT8) * (numOfPorts + 1)));
        (void)(mrp_sys_cyclic_timer_state_check (0));
      }
    }
    else /* pChUsr->state == MRP_CH_STATE_CLOSING: ensure, that alarm timer is stopped */
    {
      for (instance = 0; instance < mrpMaxInstance; instance++)
      {
        mrp_sys_timer_stop (pDBrec->pInstData[instance].TimerIdAlarm);
      }
      runFlag = mrp_sys_cyclic_timer_state_check (0);
      if (runFlag)
      {
        MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_prm_commit: pChUsr=0x%x, chState= CLOSING, cyclic comp. timer running", pChUsr);
      }
      else
      {
        MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_prm_commit: pChUsr=0x%x, chState= CLOSING, cyclic comp. timer stopped", pChUsr);
      }
    }
    
    /* check first PRM_COMMIT */
    if (pDBrec->firstPrmCommit) 
    {
      pDBrec->firstPrmCommit = LSA_FALSE;
    }
    
    if (mrpMaxInstance)
    {

      if (pChUsr->state == MRP_CH_STATE_CLOSING) 
      {
        /* always use this function with parameter instance = 0! */
        mrp_edd_init_finish_close (pDBrec->pChSys, MRP_DEFAULT_INSTANCE, pChUsr);  /* close USR channel */
      }
      else 
      {
        /* always use this function with parameter instance = 0! */
        mrp_edd_init_finish_upper (pDBrec->pChSys, MRP_DEFAULT_INSTANCE, pChUsr);
        pDBrec->PrmState = MRP_REC_ENUM_STATE_PRM_COMMIT;
      }    
    }
    else /* mrpMaxInstance == 0 */
    {
      MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pDBrec->pChSys->pChSysDB->pEdd;
      LSA_UINT16 instanceArray[2]; /* This array defines the used MRP and MRP_IN instances. */

      instanceArray[MRP_INSTANCE_INDEX] = MRP_DEFAULT_INSTANCE;
      instanceArray[MRP_IN_INSTANCE_INDEX] = MRP_IN_INSTANCE_DEFAULT;

      if (pChUsr->state == MRP_CH_STATE_CLOSING) 
      {
        MRP_PROGRAM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "call mrp_edd_req_finish_close[inst=%d] for mrpMaxInstance=0", MRP_DEFAULT_INSTANCE);
        pDBedd->pChUsr_finishClose = pChUsr;
        mrp_edd_req_finish_close(pDBrec->pChSys, instanceArray, MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_START);
      }
      else
      {
        MRP_PROGRAM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "call mrp_edd_req_finish_upper[inst=%d] for mrpMaxInstance=0", MRP_DEFAULT_INSTANCE);
        pDBedd->pChUsr_finishUpper = pChUsr;
        mrp_edd_req_finish_upper(pDBrec->pChSys, instanceArray, MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_START);      
        pDBrec->PrmState = MRP_REC_ENUM_STATE_PRM_COMMIT;
      }  
    }
    return;
  }
  else
  {
    MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_SEQUENCE: invalid channel for PRM_COMMIT (pathType=%d, isPrmIf=%d)", pChUsr->detailptr->PathType, pChUsr->detailptr->type.user.isPrmInterface);
    MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_SEQUENCE);

    return;
  }
}


/*===========================================================================
* FUNCTION : mrp_rec_prm_read
*----------------------------------------------------------------------------
* PURPOSE  : parameter read 
*----------------------------------------------------------------------------
* RETURNS  : MRP_RET_OK, MRP_RET_ERR
*----------------------------------------------------------------------------
* INPUTS   : pChUsr=user channel reference
*            pRQB=upper rqb reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : read from active configuration
* 
*            RQB response: MRP_RSP_OK, MRP_RSP_ERR_SEQUENCE, MRP_RSP_ERR_PARAM
*==========================================================================*/
LSA_UINT16 mrp_rec_prm_read (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB)
{
  LSA_UINT16 lengthTotal;
  LSA_UINT8 readAdmin;
  MRP_UPPER_MEM_UINT8_PTR_TYPE pData;
  LSA_UINT16 rc = MRP_RSP_ERR;
  MRP_REC_DB_TYPE *pDBrec = (MRP_REC_DB_TYPE *)pChUsr->pChUsrDB->pRec;
  MRP_IF_DB_TYPE *pDBif = (MRP_IF_DB_TYPE *)pChUsr->pChSysDB->pInterfacePara;
  MRP_UPPER_ARGS_PTR_TYPE pArgs = MRP_RQB_GET_ARGS_PTR(pRQB);
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChUsr->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  /* n/a
  pArgs->ctrl.prm_read.slot_number;
  pArgs->ctrl.prm_read.subslot_number;
   */
  if (pDBrec != LSA_NULL && mrp_sys_get_num_of_instances(pChUsr))
  {
    pData = pArgs->ctrl.prm_read.record_data;
    lengthTotal = 0;
    if (
#ifdef MRP_CFG_PLUGIN_2_HSR
        (pChUsr->detailptr->PathType == MRP_PATH_TYPE_USER) &&
#endif
        (pDBrec->SupportedRoleInst0 == MRP_CAPABILITY_ROLE_OFF) 
       ) 
    {
      if (!(
#ifdef MRP_CFG_PLUGIN_2_HSR
        MRP_REC_DATA_IDX_PRV_HSR_ADJUST == pArgs->ctrl.prm_read.record_index
        ||  MRP_REC_DATA_IDX_PRV_STBY_ADJUST == pArgs->ctrl.prm_read.record_index
        ||  MRP_REC_DATA_IDX_PRV_HSR_REAL == pArgs->ctrl.prm_read.record_index
        ||  MRP_REC_DATA_IDX_PRV_STBY_REAL == pArgs->ctrl.prm_read.record_index
        ||  MRP_REC_DATA_IDX_PRV_ARD_REAL == pArgs->ctrl.prm_read.record_index
        ||  MRP_REC_DATA_IDX_PRV_HSR_CONFIG == pArgs->ctrl.prm_read.record_index ||
#endif
        MRP_REC_DATA_IDX_PRV_DATA_ADJUST == pArgs->ctrl.prm_read.record_index
        ||  MRP_REC_DATA_IDX_PRV_DATA_REAL == pArgs->ctrl.prm_read.record_index
        ||  MRP_REC_DATA_IDX_IF_DATA_ADJUST == pArgs->ctrl.prm_read.record_index
        ||  MRP_REC_DATA_IDX_IF_DATA_REAL == pArgs->ctrl.prm_read.record_index
        ||  MRP_REC_DATA_IDX_IF_DATA_CHECK == pArgs->ctrl.prm_read.record_index
        ||  MRP_REC_DATA_IDX_PORT_DATA_ADJUST == pArgs->ctrl.prm_read.record_index
        ||  MRP_REC_DATA_IDX_PORT_DATA_REAL == pArgs->ctrl.prm_read.record_index
        ||  MRP_REC_DATA_IDX_IC_PORT_DATA_ADJUST == pArgs->ctrl.prm_read.record_index
        ||  MRP_REC_DATA_IDX_IC_PORT_DATA_CHECK == pArgs->ctrl.prm_read.record_index
        ||  MRP_REC_DATA_IDX_IC_PORT_DATA_REAL == pArgs->ctrl.prm_read.record_index))
      {
        MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_FATAL, ">> ERR_PRM_INDEX: rec_id (=%u)", pArgs->ctrl.prm_read.record_index);
      }
      rc = MRP_RSP_ERR_PRM_INDEX;
    }
    else 
    {
      if (MRP_RQB_GET_OPCODE(pRQB) == MRP_OPC_PRM_READ_ADMIN_DATA) 
      {
        readAdmin = LSA_TRUE;
      }
      else
        readAdmin = LSA_FALSE;
        
      switch (pArgs->ctrl.prm_read.record_index) 
      {
  #ifdef MRP_CFG_PLUGIN_2_HSR
      case MRP_REC_DATA_IDX_PRV_HSR_ADJUST:
        if (pArgs->ctrl.prm_read.edd_port_id != 0) 
        {
          rc = MRP_RSP_ERR_PRM_PORTID;
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: rec_id = 0x%x, edd_port_id (=%u)",
            pArgs->ctrl.prm_read.record_index, pArgs->ctrl.prm_read.edd_port_id);
        }
        else 
          rc = mrp_rec_read_priv_hsr_adjust (pDBrec, pData, readAdmin, pArgs->ctrl.prm_read.record_data_length, &lengthTotal);
        break;

      case MRP_REC_DATA_IDX_PRV_STBY_ADJUST:
        if (pArgs->ctrl.prm_read.edd_port_id != 0) 
        {
          rc = MRP_RSP_ERR_PRM_PORTID;
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: rec_id = 0x%x, edd_port_id (=%u)",
            pArgs->ctrl.prm_read.record_index, pArgs->ctrl.prm_read.edd_port_id);
        }
        else 
          rc = mrp_rec_read_priv_stby_adjust (pDBrec, pData, readAdmin, pArgs->ctrl.prm_read.record_data_length, &lengthTotal);
        break;

      case MRP_REC_DATA_IDX_PRV_HSR_REAL:
        if (pArgs->ctrl.prm_read.edd_port_id != 0) 
        {
          rc = MRP_RSP_ERR_PRM_PORTID;
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: rec_id = 0x%x, edd_port_id (=%u)",
            pArgs->ctrl.prm_read.record_index, pArgs->ctrl.prm_read.edd_port_id);
        }
        else 
          rc = mrp_rec_read_priv_hsr_real (pDBrec, pData, pArgs->ctrl.prm_read.record_data_length, &lengthTotal);
        break;

      case MRP_REC_DATA_IDX_PRV_STBY_REAL:
        if (pArgs->ctrl.prm_read.edd_port_id != 0) 
        {
          rc = MRP_RSP_ERR_PRM_PORTID;
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: rec_id = 0x%x, edd_port_id (=%u)",
            pArgs->ctrl.prm_read.record_index, pArgs->ctrl.prm_read.edd_port_id);
        }
        else 
          rc = mrp_rec_read_priv_stby_real (pDBrec, pData, pArgs->ctrl.prm_read.record_data_length, &lengthTotal);
        break;

      case MRP_REC_DATA_IDX_PRV_ARD_REAL:
        if (pArgs->ctrl.prm_read.edd_port_id != 0) 
        {
          rc = MRP_RSP_ERR_PRM_PORTID;
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: rec_id = 0x%x, edd_port_id (=%u)",
            pArgs->ctrl.prm_read.record_index, pArgs->ctrl.prm_read.edd_port_id);
        }
        else 
          rc = mrp_rec_read_priv_ard_real (pDBrec, pData, pArgs->ctrl.prm_read.record_data_length, &lengthTotal);
        break;
  #endif
  #ifdef MRP_CFG_PLUGIN_1_MRM_EXT
      case MRP_REC_DATA_IDX_PRV_DATA_ADJUST:
        if (pArgs->ctrl.prm_read.edd_port_id != 0) 
        {
          rc = MRP_RSP_ERR_PRM_PORTID;
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: rec_id = 0x%x, edd_port_id (=%u)",
            pArgs->ctrl.prm_read.record_index, pArgs->ctrl.prm_read.edd_port_id);
        }
        else 
          rc = mrp_rec_read_priv_data_adjust (pDBrec, pData, readAdmin, pArgs->ctrl.prm_read.record_data_length, &lengthTotal);
        break;
#endif
      case MRP_REC_DATA_IDX_PRV_DATA_REAL:
        if (pArgs->ctrl.prm_read.edd_port_id != 0) 
        {
          rc = MRP_RSP_ERR_PRM_PORTID;
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: rec_id = 0x%x, edd_port_id (=%u)",
            pArgs->ctrl.prm_read.record_index, pArgs->ctrl.prm_read.edd_port_id);
        }
        else 
          rc = mrp_rec_read_priv_data_real (pDBrec, pData, pArgs->ctrl.prm_read.record_data_length, &lengthTotal);
        break;

      case MRP_REC_DATA_IDX_IF_DATA_ADJUST:
        if (pArgs->ctrl.prm_read.edd_port_id != 0) 
        {
          rc = MRP_RSP_ERR_PRM_PORTID;
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: rec_id = 0x%x, edd_port_id (=%u)",
            pArgs->ctrl.prm_read.record_index, pArgs->ctrl.prm_read.edd_port_id);
        }
        else 
          rc = mrp_rec_read_interface_data_adjust (pDBrec, pData, readAdmin, pArgs->ctrl.prm_read.record_data_length, &lengthTotal);
        break;

      case MRP_REC_DATA_IDX_PORT_DATA_ADJUST:
        if (pArgs->ctrl.prm_read.edd_port_id == 0) 
        {
          rc = MRP_RSP_ERR_PRM_PORTID;
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: rec_id = 0x%x, edd_port_id (= %u)",
            pArgs->ctrl.prm_read.record_index, pArgs->ctrl.prm_read.edd_port_id);
        }
        else /* pArgs->ctrl.prm_read.edd_port_id != 0 */
        {
          if (pDBif->pPortData[pArgs->ctrl.prm_read.edd_port_id - 1].rpType == EDD_MRP_NO_RING_PORT)
          {
            rc = MRP_RSP_ERR_PRM_INDEX;
            MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_INDEX: read requ for DS rec_id = 0x%x to port (edd_port_id = %u) configured as EDD_MRP_NO_RING_PORT",
              pArgs->ctrl.prm_read.record_index, pArgs->ctrl.prm_read.edd_port_id);
          }
          else
          {
            if (pDBif->pPortData[pArgs->ctrl.prm_read.edd_port_id - 1].moduleState == MRP_PRM_PORTMODULE_PULLED || pDBif->pPortData[pArgs->ctrl.prm_read.edd_port_id - 1].moduleState == MRP_PRM_PORTMODULE_PLUG_PREPARE)
            {
              rc = MRP_RSP_ERR_PRM_PORTID;
              MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: read requ for DS rec_id = 0x%x to PULLED port (edd_port_id = %u)",
                pArgs->ctrl.prm_read.record_index, pArgs->ctrl.prm_read.edd_port_id);
            }
            else
            {
              rc = mrp_rec_read_port_data_adjust (pDBrec, pData, readAdmin, pDBrec->NumOfPossibleRports, pArgs->ctrl.prm_read.record_data_length, pArgs->ctrl.prm_read.edd_port_id, &lengthTotal);
            }
          }
        }
        break;

      case MRP_REC_DATA_IDX_IF_DATA_CHECK:
        if (pArgs->ctrl.prm_read.edd_port_id != 0) 
        {
          rc = MRP_RSP_ERR_PRM_PORTID;
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: rec_id = 0x%x, edd_port_id (=%u)",
            pArgs->ctrl.prm_read.record_index, pArgs->ctrl.prm_read.edd_port_id);
        }
        else 
          rc = mrp_rec_read_interface_data_check (pDBrec, pData, readAdmin, pArgs->ctrl.prm_read.record_data_length, &lengthTotal);
        break;

      case MRP_REC_DATA_IDX_IF_DATA_REAL:
        if (pArgs->ctrl.prm_read.edd_port_id != 0) 
        {
          rc = MRP_RSP_ERR_PRM_PORTID;
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: rec_id = 0x%x, edd_port_id (=%u)",
            pArgs->ctrl.prm_read.record_index, pArgs->ctrl.prm_read.edd_port_id);
        }
        else 
          rc = mrp_rec_read_interface_data_real (pDBrec, pData, pArgs->ctrl.prm_read.record_data_length, &lengthTotal);
        break;

      case MRP_REC_DATA_IDX_PORT_DATA_REAL:
        if (pArgs->ctrl.prm_read.edd_port_id == 0) 
        {
          rc = MRP_RSP_ERR_PRM_PORTID;
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: rec_id = 0x%x, edd_port_id (=%u)",
            pArgs->ctrl.prm_read.record_index, pArgs->ctrl.prm_read.edd_port_id);
        }
        else /* pArgs->ctrl.prm_read.edd_port_id != 0 */
        {
          if (pDBif->pPortData[pArgs->ctrl.prm_read.edd_port_id - 1].rpType == EDD_MRP_NO_RING_PORT)
          {
            rc = MRP_RSP_ERR_PRM_INDEX;
            MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_INDEX: read requ for DS rec_id = 0x%x to port (edd_port_id = %u) configured as EDD_MRP_NO_RING_PORT",
              pArgs->ctrl.prm_read.record_index, pArgs->ctrl.prm_read.edd_port_id);
          }
          else
          {
            if (pDBif->pPortData[pArgs->ctrl.prm_read.edd_port_id - 1].moduleState == MRP_PRM_PORTMODULE_PULLED || pDBif->pPortData[pArgs->ctrl.prm_read.edd_port_id - 1].moduleState == MRP_PRM_PORTMODULE_PLUG_PREPARE)
            {
              rc = MRP_RSP_ERR_PRM_PORTID;
              MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: read requ for DS rec_id = 0x%x to PULLED port (edd_port_id = %u)",
                pArgs->ctrl.prm_read.record_index, pArgs->ctrl.prm_read.edd_port_id);
            }
            else 
            {
              rc = mrp_rec_read_port_data_real (pDBrec, pData, pArgs->ctrl.prm_read.record_data_length, pArgs->ctrl.prm_read.edd_port_id, &lengthTotal);
            }
          }
        }
        break;

      case MRP_REC_DATA_IDX_IC_PORT_DATA_ADJUST:
        if (pArgs->ctrl.prm_read.edd_port_id == 0)
        {
          rc = MRP_RSP_ERR_PRM_PORTID;
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: rec_id = 0x%x, edd_port_id (= %u)",
            pArgs->ctrl.prm_read.record_index, pArgs->ctrl.prm_read.edd_port_id);
        }
        else
        { 
          if (pDBif->pPortData[pArgs->ctrl.prm_read.edd_port_id - 1].inPortType == EDD_SUPPORTS_MRP_INTERCONN_PORT_CONFIG_NO)
          {
            rc = MRP_RSP_ERR_PRM_INDEX;
            MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_INDEX: read requ for DS rec_id = 0x%x to port(edd_port_id = %u) configured as EDD_SUPPORTS_MRP_INTERCONN_PORT_CONFIG_NO",
              pArgs->ctrl.prm_read.record_index, pArgs->ctrl.prm_read.edd_port_id);
          }
          else
          {
            if (pDBif->pPortData[pArgs->ctrl.prm_read.edd_port_id - 1].moduleState == MRP_PRM_PORTMODULE_PULLED || pDBif->pPortData[pArgs->ctrl.prm_read.edd_port_id - 1].moduleState == MRP_PRM_PORTMODULE_PLUG_PREPARE)
            {
              rc = MRP_RSP_ERR_PRM_PORTID;
              MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: read requ for DS rec_id = 0x%x to PULLED port (edd_port_id = %u)",
                pArgs->ctrl.prm_read.record_index, pArgs->ctrl.prm_read.edd_port_id);
            }
            else
            {
              rc = mrp_rec_read_ic_port_data_adjust(pDBrec, pData, readAdmin, pArgs->ctrl.prm_read.record_data_length, pArgs->ctrl.prm_read.edd_port_id, &lengthTotal);
            }
          }
        }
        break;

      case MRP_REC_DATA_IDX_IC_PORT_DATA_CHECK:
        if (pArgs->ctrl.prm_read.edd_port_id == 0)
        {
          rc = MRP_RSP_ERR_PRM_PORTID;
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: rec_id = 0x%x, edd_port_id (= %u)",
            pArgs->ctrl.prm_read.record_index, pArgs->ctrl.prm_read.edd_port_id);
        }
        else
        {
          if (pDBif->pPortData[pArgs->ctrl.prm_read.edd_port_id - 1].inPortType == EDD_SUPPORTS_MRP_INTERCONN_PORT_CONFIG_NO)
          {
            rc = MRP_RSP_ERR_PRM_INDEX;
            MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_INDEX: read requ for DS rec_id = 0x%x to port(edd_port_id = %u) configured as EDD_SUPPORTS_MRP_INTERCONN_PORT_CONFIG_NO",
              pArgs->ctrl.prm_read.record_index, pArgs->ctrl.prm_read.edd_port_id);
          }
          else
          {
            if (pDBif->pPortData[pArgs->ctrl.prm_read.edd_port_id - 1].moduleState == MRP_PRM_PORTMODULE_PULLED || pDBif->pPortData[pArgs->ctrl.prm_read.edd_port_id - 1].moduleState == MRP_PRM_PORTMODULE_PLUG_PREPARE)
            {
              rc = MRP_RSP_ERR_PRM_PORTID;
              MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: read requ for DS rec_id = 0x%x to PULLED port (edd_port_id = %u)",
                pArgs->ctrl.prm_read.record_index, pArgs->ctrl.prm_read.edd_port_id);
            }
            else
            {
                rc = mrp_rec_read_ic_port_data_check(pDBrec, pData, readAdmin, pArgs->ctrl.prm_read.record_data_length, pArgs->ctrl.prm_read.edd_port_id, &lengthTotal);
            }
          }
        }
        break;

      case MRP_REC_DATA_IDX_IC_PORT_DATA_REAL:
        if (pArgs->ctrl.prm_read.edd_port_id == 0)
        {
          rc = MRP_RSP_ERR_PRM_PORTID;
          MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: rec_id = 0x%x, edd_port_id (= %u)",
            pArgs->ctrl.prm_read.record_index, pArgs->ctrl.prm_read.edd_port_id);
        }
        else
        {
          if (pDBif->pPortData[pArgs->ctrl.prm_read.edd_port_id - 1].inPortType == EDD_SUPPORTS_MRP_INTERCONN_PORT_CONFIG_NO)
          {
            rc = MRP_RSP_ERR_PRM_INDEX;
            MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_INDEX: read requ for DS rec_id = 0x%x to port(edd_port_id = %u) configured as EDD_SUPPORTS_MRP_INTERCONN_PORT_CONFIG_NO",
              pArgs->ctrl.prm_read.record_index, pArgs->ctrl.prm_read.edd_port_id);
          }
          else
          {
            if (pDBif->pPortData[pArgs->ctrl.prm_read.edd_port_id - 1].moduleState == MRP_PRM_PORTMODULE_PULLED || pDBif->pPortData[pArgs->ctrl.prm_read.edd_port_id - 1].moduleState == MRP_PRM_PORTMODULE_PLUG_PREPARE)
            {
              rc = MRP_RSP_ERR_PRM_PORTID;
              MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_PRM_PORTID: read requ for DS rec_id = 0x%x to PULLED port (edd_port_id = %u)",
                pArgs->ctrl.prm_read.record_index, pArgs->ctrl.prm_read.edd_port_id);
            }
            else
            {
              rc = mrp_rec_read_ic_port_data_real (pDBrec, pData, pArgs->ctrl.prm_read.record_data_length, pArgs->ctrl.prm_read.edd_port_id, &lengthTotal);
            }
          }
        }
        break;

      default:
        MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> unknown rec_id = 0x%x", pArgs->ctrl.prm_read.record_index);
        rc = MRP_RSP_ERR_PRM_INDEX;
        break;
      }
    }
    pArgs->ctrl.prm_read.record_data_length = lengthTotal;
    MRP_RQB_SET_RESPONSE(pRQB, rc);
    
    if (MRP_RQB_GET_OPCODE(pRQB) != MRP_OPC_PRM_READ_ADMIN_DATA) 
    {
      mrp_usr_call_upper_cbf_proc_next_req (pChUsr, pRQB);
    }
    return MRP_RET_OK;
  }
  else
  {
    MRP_UPPER_TRACE_00(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_SEQUENCE: no PRM_READ possible because no DB available");
    MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_SEQUENCE);

    return MRP_RET_ERR;
  }
}


/*===========================================================================
* FUNCTION : mrp_rec_prm_ind_provide_rqb
*----------------------------------------------------------------------------
* PURPOSE  : provide prm indication rqb to enable mrp diag signaling to usr
*----------------------------------------------------------------------------
* RETURNS  : MRP_RET_OK
*----------------------------------------------------------------------------
* INPUTS   : pChUsr  = user channel reference
*            pRQB    = upper rqb reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT16 mrp_rec_prm_ind_provide_rqb (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB)
{
  LSA_UINT16 portID, numOfPorts;
  MRP_UPPER_ARGS_PTR_TYPE pArgs;
  MRP_REC_DB_TYPE *pDBrec = (MRP_REC_DB_TYPE *)pChUsr->pChUsrDB->pRec;

  numOfPorts = mrp_sys_get_num_of_ports (pDBrec->pChSys);

  if (pChUsr->detailptr->PathType                 == MRP_PATH_TYPE_USER  &&
      pChUsr->detailptr->type.user.isPrmInterface == MRP_IS_PRM_INTERFACE_ON)
  {
    if (pChUsr->state != MRP_CH_STATE_CLOSING) 
    {
      MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_OK);

      pArgs = MRP_RQB_GET_ARGS_PTR(pRQB);
      portID = pArgs->state.prm_ind.edd_port_id;

      if (portID > numOfPorts) 
      {
        MRP_UPPER_TRACE_01(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> mrp_rec_prm_ind_provide_rqb: ERR_PARAM (edd_port_id (=%u))", portID);
        MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_PARAM);

        return MRP_RET_ERR;
      }
      MRP_RQB_PUSH_TO_BOTTOM(pDBrec->ppRqbIndicationBot[portID], pDBrec->ppRqbIndicationTop[portID], pRQB);
      pChUsr->pRQB = LSA_NULL;

      /* test starvation */
      if (portID == 0) 
      {
        if (pDBrec->pAlarmOutOfResource[0]) 
        {
          pDBrec->pAlarmOutOfResource[0] = MRP_ALARM_OUT_OF_RESOURCE_FALSE;
          (void)(mrp_rec_prm_ind_done (pChUsr));
        }
      }
      else 
      {
        if (pDBrec->pAlarmOutOfResource[portID]) 
        {
          pDBrec->pAlarmOutOfResource[portID] = MRP_ALARM_OUT_OF_RESOURCE_FALSE;
          (void)(mrp_rec_prm_ind_port_done (pChUsr, portID));
        }
      }
      return MRP_RET_OK;
    }
    else
    {
      MRP_UPPER_TRACE_00(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> mrp_rec_prm_ind_provide_rqb: ERR_SEQUENCE (channel in CLOSING state)");
      MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_SEQUENCE);

      return MRP_RET_ERR;
    }
  }
  else
  {
    MRP_UPPER_TRACE_02(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> ERR_SEQUENCE: invalid channel for prm_ind_provide_rqb (pathType=%d, isPrmIf=%d)", pChUsr->detailptr->PathType, pChUsr->detailptr->type.user.isPrmInterface);
    MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_SEQUENCE);

    return MRP_RET_ERR;
  }
}


/*===========================================================================
* FUNCTION : mrp_rec_timer_cbf_alarm
*----------------------------------------------------------------------------
* PURPOSE  : delayed alarm notification
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChUsr    = user channel reference
*            pInstance = pointer to (MRP) instance, for which the timer cbf is called  
*            uid       = user id
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_rec_timer_cbf_alarm (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, LSA_USER_ID_TYPE uid)
{
  LSA_UINT16 i;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  MRP_REC_DB_TYPE *pDBrec = LSA_NULL;
  MRP_CHANNEL_TYPE_USER pChUsr = LSA_NULL;

  LSA_UNUSED_ARG(uid);

  MRP_ASSERT(pInstance[MRP_INSTANCE_INDEX] != MRP_TIMER_INTERFACE);

  for (i = 0; i < MRP_MAX_USER_CHANNELS_PER_IF; i++)
  {
    pChUsr = pDBedd->pChUsr[i];

    if (pDBedd->pChUsr[i]->state != MRP_CH_STATE_FREE && 
        pDBedd->pChUsr[i]->detailptr->type.user.isPrmInterface == MRP_IS_PRM_INTERFACE_ON)
    {
      pDBrec = (MRP_REC_DB_TYPE *)pDBedd->pChUsr[i]->pChUsrDB->pRec;
      break;
    }
  }

  MRP_ASSERT (!LSA_HOST_PTR_ARE_EQUAL(pDBrec, LSA_NULL));
  pDBrec->pInstData[pInstance[MRP_INSTANCE_INDEX]].AlarmBlocked = LSA_FALSE;
  
  mrp_rec_event_req (pChUsr, pInstance[MRP_INSTANCE_INDEX]);
}


/*===========================================================================
* FUNCTION : mrp_rec_prm_ind_helper
*----------------------------------------------------------------------------
* PURPOSE  : evaluate indications
*----------------------------------------------------------------------------
* RETURNS  : LSA_FALSE = no indication, LSA_TRUE = indication
*----------------------------------------------------------------------------
* INPUTS   : pChUsr  = user channel reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_UINT8 mrp_rec_prm_ind_helper (const MRP_CONST_CHANNEL_TYPE_USER pChUsr,
                                                    MRP_ALARM_TYPE *alarm, 
                                                    MRP_ALARM_TYPE *alarmIndicated, 
                                                    MRP_PRM_INDICATION_DIAG_ENTRY_TYPE *diag, 
                                                    LSA_UINT16 extChannelErrorType)
{  
#if (MRP_CFG_TRACE_MODE == 0)
  LSA_UNUSED_ARG(pChUsr);
#endif
#if (MRP_CFG_TRACE_MODE == 1)
  LSA_UNUSED_ARG(pChUsr);
#endif

  if (alarm->property  && (alarm->property != alarmIndicated->property)) 
  {
    if ( ((alarm->property & MRM_ALARM_PROP_ERROR_DISAPPEAR)       == MRM_ALARM_PROP_ERROR_DISAPPEAR) && 
         ((alarmIndicated->property & MRM_ALARM_PROP_ERROR_APPEAR) != MRM_ALARM_PROP_ERROR_APPEAR) &&
         extChannelErrorType != 0x0000 /* commitCnf handling should not be affected!!! */
       )
    {
      MRP_UPPER_TRACE_00(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "Attempt to clear a diag, which is not yet set --> not executed");
      /* if a diag is requested to be cleared, which was not yet raised due to lack of resources, then it doesn't have to be cleared!!! */
      /* Otherwise, an exception in cm_pd5.c (line 1227) is raised!!! See RQ AP00813881, too!!!                                         */
      return LSA_FALSE;
    }
    else
    {
      if (!LSA_HOST_PTR_ARE_EQUAL (diag, LSA_NULL)) 
      {
        diag->ChannelProperties = alarm->property;
        diag->ChannelErrorType    = MRP_DIAG_CHAN_ERR_TYPE_REDUNDANCY_MISMATCH;
        diag->ExtChannelErrorType = extChannelErrorType;
        diag->ExtChannelAddValue = 0;
        /* save last indication */
        alarmIndicated->property = alarm->property;
        /* reset signaled events */
        alarm->property           = MRM_ALARM_PROP_UNCHANGED;
      }
      return LSA_TRUE;
    }  
  }  
  return LSA_FALSE;
}


/*===========================================================================
* FUNCTION : mrp_rec_prm_ind_done
*----------------------------------------------------------------------------
* PURPOSE  : indicate changes of given values (Expected-Config) to CM-PD
*----------------------------------------------------------------------------
* RETURNS  : LSA_FALSE=no rqb, LSA_TRUE=indicated
*----------------------------------------------------------------------------
* INPUTS   : pChUsr=user channel reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : Alarm indication:
*            (1) indication is done, if a check record is available (with commit)
*            (2) any change (appear/disappear) of ExtChannelError has to be indicated
*            (3) if new PRM is done (COMMIT), indications have to be retriggered
*
*            NOTE:
*            If no resources are available, the last indication has to be retained.
*            If there's a difference to the last indicated message, the retained
*            message has to be indicated.
*            The difference check is done after Commit.CNF (NOT after receipt of 
*            Commit.REQ).
*==========================================================================*/
LSA_UINT8 mrp_rec_prm_ind_done (const MRP_CONST_CHANNEL_TYPE_USER pChUsr)
{
  MRP_UPPER_ARGS_PTR_TYPE pArgs;
  MRP_RQB_TYPE *pRQB;
  LSA_UINT16 idx, mrpMaxInstance, i;
  LSA_UINT8 newDiag = LSA_FALSE;
  MRP_REC_DB_TYPE *pDBrec = (MRP_REC_DB_TYPE *)pChUsr->pChUsrDB->pRec;
  MRP_REC_INST_DATA_TYPE *pDBrecInst;
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChUsr->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  mrpMaxInstance = mrp_sys_get_num_of_instances(pChUsr);

  if (pDBrec != LSA_NULL)
  {
    if (mrpMaxInstance)
    {
      for (i = 0; i < mrpMaxInstance && !newDiag; i++)
      {
        pDBrecInst = &pDBrec->pInstData[i];
        newDiag = (  mrp_rec_prm_ind_helper (pChUsr, &pDBrec->commitCnf, &pDBrec->commitCnfIndicated, LSA_NULL, 0x0000)
                  || mrp_rec_prm_ind_helper (pChUsr, &pDBrecInst->Alarm.morphing, &pDBrecInst->AlarmIndicated.morphing, LSA_NULL, (LSA_UINT16)(MRP_DIAG_EXT_CHAN_ERR_TYPE_MGR_ROLE_FAIL + i * 16))        
                  || mrp_rec_prm_ind_helper (pChUsr, &pDBrecInst->Alarm.singleSideRx, &pDBrecInst->AlarmIndicated.singleSideRx, LSA_NULL, (LSA_UINT16)(MRP_DIAG_EXT_CHAN_ERR_TYPE_MRP_RING_OPEN + i * 16))
                  || mrp_rec_prm_ind_helper (pChUsr, &pDBrecInst->Alarm.ringOpen, &pDBrecInst->AlarmIndicated.ringOpen, LSA_NULL, (LSA_UINT16)(MRP_DIAG_EXT_CHAN_ERR_TYPE_MRP_RING_OPEN + i * 16))
                  || mrp_rec_prm_ind_helper (pChUsr, &pDBrecInst->Alarm.multiRM, &pDBrecInst->AlarmIndicated.multiRM, LSA_NULL, (LSA_UINT16)(MRP_DIAG_EXT_CHAN_ERR_TYPE_MULTIPLE_MGR + i * 16)));    
      }
    }
    else
    {
      newDiag = (mrp_rec_prm_ind_helper (pChUsr, &pDBrec->commitCnf, &pDBrec->commitCnfIndicated, LSA_NULL, 0x0000));
    }

    if (newDiag || pChUsr->state == MRP_CH_STATE_CLOSING) 
    {
      /* indicate interface related alarms at index[0] */
      MRP_RQB_POP_FROM_TOP(pDBrec->ppRqbIndicationBot[0], pDBrec->ppRqbIndicationTop[0], pRQB);

      if (LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL)) 
      {
        pDBrec->pAlarmOutOfResource[0] = MRP_ALARM_OUT_OF_RESOURCE_TRUE;

        MRP_UPPER_TRACE_00(traceIdx, LSA_TRACE_LEVEL_WARN, "prm_ind_done(interface): NO resources!");

        for (i = 0; i < mrpMaxInstance; i++)
        {
          pDBrecInst = &pDBrec->pInstData[i];
          MRP_UPPER_TRACE_05(traceIdx, LSA_TRACE_LEVEL_NOTE, "al_state[inst=%d]: singleSideRx/ringOpen/morph/multiRM=%d/%d/%d/%d",
            i, pDBrecInst->Alarm.singleSideRx.state, pDBrecInst->Alarm.ringOpen.state, pDBrecInst->Alarm.morphing.state, pDBrecInst->Alarm.multiRM.state);
          MRP_UPPER_TRACE_05(traceIdx, LSA_TRACE_LEVEL_NOTE, "al_prop[inst=%d]: singleSideRx/ringOpen/morph/multiRM=0x%x/%x/%x/%x",
            i, pDBrecInst->Alarm.singleSideRx.property, pDBrecInst->Alarm.ringOpen.property, 
            pDBrecInst->Alarm.morphing.property, pDBrecInst->Alarm.multiRM.property);        

          LSA_UNUSED_ARG(pDBrecInst);   /* hhr: to please the compiler in case traces are disabled. */
        }

        return LSA_FALSE;
      }
      pArgs = MRP_RQB_GET_ARGS_PTR(pRQB);
      pArgs->state.prm_ind.edd_port_id = 0;
      idx = 0;
      
      (void)(mrp_rec_prm_ind_helper (pChUsr, &pDBrec->commitCnf, &pDBrec->commitCnfIndicated, &pArgs->state.prm_ind.diag[idx], 0x0000));
      
      for (i = 0; i < mrpMaxInstance; i++)
      {
        pDBrecInst = &pDBrec->pInstData[i];
        if (mrp_rec_prm_ind_helper (pChUsr, &pDBrecInst->Alarm.morphing, &pDBrecInst->AlarmIndicated.morphing, &pArgs->state.prm_ind.diag[idx], (LSA_UINT16)(MRP_DIAG_EXT_CHAN_ERR_TYPE_MGR_ROLE_FAIL + i * 16))) 
        {
          idx++;
        }
        if (mrp_rec_prm_ind_helper (pChUsr, &pDBrecInst->Alarm.singleSideRx, &pDBrecInst->AlarmIndicated.singleSideRx, &pArgs->state.prm_ind.diag[idx], (LSA_UINT16)(MRP_DIAG_EXT_CHAN_ERR_TYPE_MRP_RING_OPEN + i * 16))) 
        {
          idx++;
        }
        if (mrp_rec_prm_ind_helper(pChUsr, &pDBrecInst->Alarm.ringOpen, &pDBrecInst->AlarmIndicated.ringOpen, &pArgs->state.prm_ind.diag[idx], (LSA_UINT16)(MRP_DIAG_EXT_CHAN_ERR_TYPE_MRP_RING_OPEN + i * 16)))
        {
          idx++;
        }
        if (mrp_rec_prm_ind_helper (pChUsr, &pDBrecInst->Alarm.multiRM, &pDBrecInst->AlarmIndicated.multiRM, &pArgs->state.prm_ind.diag[idx], (LSA_UINT16)(MRP_DIAG_EXT_CHAN_ERR_TYPE_MULTIPLE_MGR + i * 16)))
        {
          idx++;
        }
        MRP_UPPER_TRACE_05(traceIdx, LSA_TRACE_LEVEL_NOTE, "al_state[inst=%d]: singleSideRx/ringOpen/morph/multiRM=%d/%d/%d/%d",
          i, pDBrecInst->Alarm.singleSideRx.state, pDBrecInst->Alarm.ringOpen.state, pDBrecInst->Alarm.morphing.state, 
          pDBrecInst->Alarm.multiRM.state);
        MRP_UPPER_TRACE_05(traceIdx, LSA_TRACE_LEVEL_NOTE, "al_prop[inst=%d]: singleSideRx/ringOpen/morph/multiRM=0x%x/%x/%x/%x",
          i, pDBrecInst->AlarmIndicated.singleSideRx.property, pDBrecInst->AlarmIndicated.ringOpen.property,
          pDBrecInst->AlarmIndicated.morphing.property, pDBrecInst->AlarmIndicated.multiRM.property);             
      }
      MRP_ASSERT(MRP_PRM_INDICATION_DIAG_MAX_ENTRIES >= idx);
      pArgs->state.prm_ind.diag_cnt = idx;
      
      if (pChUsr->state == MRP_CH_STATE_CLOSING) 
      {
        MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "prm_ind_done in CLOSING state, diag_cnt=%d reset", pArgs->state.prm_ind.diag_cnt);
        
        pArgs->state.prm_ind.diag_cnt = 0;
        MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_OK_CANCEL);
      }

      mrp_usr_call_upper_cbf (pChUsr, pRQB);

      return LSA_TRUE;
    }
    else
    {
      MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "prm_ind_done FALSE, newDiag=%d, pChUsr->state=%d", newDiag, pChUsr->state);
      return LSA_FALSE;
    }
  }
  else
  {
    MRP_UPPER_TRACE_00(traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_rec_prm_ind_done: invalid call, NO PRM_DB available");
      
    return LSA_FALSE;
  }
}


/*===========================================================================
* FUNCTION : mrp_rec_prm_ind_port_done
*----------------------------------------------------------------------------
* PURPOSE  : port based commit.Cnf
*----------------------------------------------------------------------------
* RETURNS  : LSA_FALSE=no rqb, LSA_TRUE=indicated
*----------------------------------------------------------------------------
* INPUTS   : pChUsr=user channel reference
*            portID=edd port id
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT8 mrp_rec_prm_ind_port_done (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 portID)
{
  MRP_UPPER_ARGS_PTR_TYPE pArgs;
  MRP_RQB_TYPE *pRQB;
  MRP_REC_DB_TYPE *pDBrec = (MRP_REC_DB_TYPE *)pChUsr->pChUsrDB->pRec;

  if (pDBrec != LSA_NULL)
  {
    MRP_RQB_POP_FROM_TOP(pDBrec->ppRqbIndicationBot[portID], pDBrec->ppRqbIndicationTop[portID], pRQB);
    if (LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL)) 
    {
      if (pChUsr->state != MRP_CH_STATE_CLOSING) 
      {
        MRP_UPPER_TRACE_01(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_WARN, ">> mrp_rec_prm_ind_port_done: PRM_INDICATION(Commit.Cnf): out of resource (portID=%u)", portID);
      }
      pDBrec->pAlarmOutOfResource[portID] = MRP_ALARM_OUT_OF_RESOURCE_TRUE;
      return LSA_FALSE;
    }
    if (pChUsr->state == MRP_CH_STATE_CLOSING) 
    {
      MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_OK_CANCEL);
    }
    pArgs = MRP_RQB_GET_ARGS_PTR(pRQB);
    /* pArgs->state.prm_ind.edd_port_id=portID; */
    pArgs->state.prm_ind.diag_cnt = 0;
    mrp_usr_call_upper_cbf (pChUsr, pRQB);

    return LSA_TRUE;
  }
  else
  {
    MRP_UPPER_TRACE_01(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_rec_prm_ind_port_done: invalid call for portID=%d, NO PRM_DB available", portID);
    return LSA_FALSE;
  }
}


/*===========================================================================
* FUNCTION : mrp_rec_prm_ind_commit_cnf_done
*----------------------------------------------------------------------------
* PURPOSE  : do commit.cnf for interface and all ports
*----------------------------------------------------------------------------
* RETURNS  : LSA_FALSE=no rqb, LSA_TRUE=indicated
*----------------------------------------------------------------------------
* INPUTS   : pChUsr=user channel reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_rec_prm_ind_commit_cnf_done (const MRP_CONST_CHANNEL_TYPE_USER pChUsr)
{
  LSA_UINT16 portID, numOfPorts;
  MRP_REC_DB_TYPE *pDBrec = (MRP_REC_DB_TYPE *)pChUsr->pChUsrDB->pRec;

  if (!LSA_HOST_PTR_ARE_EQUAL(pDBrec, LSA_NULL))
  {
    numOfPorts = mrp_sys_get_num_of_ports (pDBrec->pChSys);

    /*  set PrmEnd here to allow additional write after commit without Prepare */
    pDBrec->PrmEnd = LSA_FALSE;
    
    pDBrec->commitCnf.property = MRM_ALARM_PROP_ERROR_DISAPPEAR;
      
    if (!mrp_rec_prm_ind_done (pChUsr)) 
    {
      MRP_UPPER_TRACE_00(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_WARN, ">> PRM_INDICATION (Commit.Cnf): out of resource (portID=0)");
    }

    for (portID = 1; portID < numOfPorts + 1; portID++) 
    {
      (void)(mrp_rec_prm_ind_port_done (pChUsr, portID));
    }
  }
  else
  {
    MRP_UPPER_TRACE_00(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_rec_prm_ind_commit_cnf_done: invalid call, NO PRM_DB available");
  }
}


/*===========================================================================
* FUNCTION : mrp_rec_event_req
*----------------------------------------------------------------------------
* PURPOSE  : event request
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChUsr=user channel reference
*            instance = affected MRP/HSR instance
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*            Precondition for DiagnosisObjects is, that mrp-domain monitoring is
*            activated for MRM in provisioning.
*            DiagnosiObject "Manager role fail" on interface submodule disables any
*            other underlying diagnosisObject.
*            Only the active MRM creates diagnosisObject "multiple managers".
*            The diagnosisQualifier of this diagnosisObject is set to
*            maintenanceDemanded or error. This depends on whether the additional
*            MRM is detected in it's own ring or in an adjacent ring. 
*            Only the active MRM monitors the state of the MRP ring and raises
*            a diagnosisObject "Ring open", if the ring is open.
*            If the MRP ring with NRT traffic is closed, there's a check, whether the
*            ring is closed for RT traffic as well (depending on, whether the RT redundancy
*            is activated).
*            If RT redundancy is not activated, the MRM raises the diagnosisObject
*            "RT redundancy lost".
*==========================================================================*/
LSA_VOID mrp_rec_event_req (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance)
{
  MRP_REC_DB_TYPE *pDBrec;
  MRP_REC_INST_DATA_TYPE *pDBrecInst;
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChUsr->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  if (pChUsr->detailptr->PathType == MRP_PATH_TYPE_USER &&
      pChUsr->detailptr->type.user.isPrmInterface == MRP_IS_PRM_INTERFACE_ON)
  {
    pDBrec = (MRP_REC_DB_TYPE *)pChUsr->pChUsrDB->pRec;
    pDBrecInst  = &pDBrec->pInstData[instance];

    MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBrec, LSA_NULL));

    if (pDBrecInst->AlarmBlocked) 
    {
      return;
    }
    /* evaluate check recored */
    if (!pDBrecInst->work.RecInstanceCheck.Written || (pDBrecInst->work.RecInstanceCheck.Written && !(pDBrecInst->work.RecInstanceCheck.Check & MRP_REC_DATA_CHECK_MRM))) 
    {
      /* alarm disabled! */
      return;
    }
    /* evaluate manager role state */
#ifdef MRP_CFG_PLUGIN_0_MRM
    {
      MRP_MRM_STATE_TYPE mrpState;
      (void)(mrp_mrm_get_state (pDBrec->pChSys, instance, &mrpState, LSA_FALSE));
      if (mrpState.mrmVotingLost) 
      {
        pDBrecInst->Alarm.roleClient.state = LSA_TRUE; /* This state will not be reported as diag alarm, but only written to event log */
      }
      else 
      {
        pDBrecInst->Alarm.roleClient.state = LSA_FALSE;  /* This state will not be reported as diag alarm, but only written to event log */
      }

      if (mrpState.roleMgr && mrpState.morphing)
      {
        pDBrecInst->Alarm.morphing.state = LSA_TRUE;
      }
      else
      {
        pDBrecInst->Alarm.morphing.state = LSA_FALSE;
      }

      if (mrpState.roleMgr && mrpState.singleSideRx) 
      {
        pDBrecInst->Alarm.singleSideRx.state = LSA_TRUE;
      }
      else 
      {
        pDBrecInst->Alarm.singleSideRx.state = LSA_FALSE;
      }

      if (mrpState.roleMgr && mrpState.ringOpen) 
      {
        pDBrecInst->Alarm.ringOpen.state = LSA_TRUE;
      }
      else 
      {
        pDBrecInst->Alarm.ringOpen.state = LSA_FALSE;
      }

      if (mrpState.roleMgr && mrpState.multiRM) 
      {
        pDBrecInst->Alarm.multiRM.state = LSA_TRUE;
      }
      else 
      {
        pDBrecInst->Alarm.multiRM.state = LSA_FALSE;
      }
    }
  #endif /* MRP_CFG_PLUGIN_0_MRM */
    if (pDBrecInst->Alarm.roleClient.state && !pDBrecInst->Alarm.roleClient.preState) 
    { /* appear */ 
      /* This state will not be reported as diag alarm, but only written to event log */
      pDBrecInst->Alarm.roleClient.preState = pDBrecInst->Alarm.roleClient.state;
      pDBrecInst->Alarm.roleClient.property = MRM_ALARM_PROP_UNCHANGED;
      MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_event_req[inst=%d]: operational MRA voted out to Client state", instance);
    }
    if (!pDBrecInst->Alarm.roleClient.state && pDBrecInst->Alarm.roleClient.preState) 
    { /* disappear */ 
      /* This state will not be reported as diag alarm, but only written to event log */
      pDBrecInst->Alarm.roleClient.preState = pDBrecInst->Alarm.roleClient.state;
      pDBrecInst->Alarm.roleClient.property = MRM_ALARM_PROP_UNCHANGED;
      MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_event_req[inst=%d]: MRA (MRC) has fallen back to operational MRA", instance);
    }
    if (pDBrecInst->Alarm.roleClient.state) 
    {
      pDBrecInst->Alarm.morphing.state = LSA_FALSE;
      pDBrecInst->Alarm.singleSideRx.state = LSA_FALSE;
      pDBrecInst->Alarm.ringOpen.state = LSA_FALSE;
      pDBrecInst->Alarm.multiRM.state = LSA_FALSE;
    }
    else 
    {
  #ifndef MRP_CFG_PLUGIN_0_MRM
      pDBrecInst->Alarm.morphing.state = LSA_FALSE;
      pDBrecInst->Alarm.singleSideRx.state = LSA_FALSE;
      pDBrecInst->Alarm.ringOpen.state = LSA_FALSE;
      pDBrecInst->Alarm.multiRM.state = LSA_FALSE;
  #endif /* MRP_CFG_PLUGIN_0_MRM */
    }
    if (pDBrecInst->Alarm.morphing.state && !pDBrecInst->Alarm.morphing.preState) 
    { /* appear */ 
      pDBrecInst->Alarm.morphing.preState = pDBrecInst->Alarm.morphing.state;
      pDBrecInst->Alarm.morphing.property = MRM_ALARM_PROP_MD_APPEAR;
      MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_event_req[inst=%d]: diag morphing SET", instance);
    }
    if (!pDBrecInst->Alarm.morphing.state && pDBrecInst->Alarm.morphing.preState) 
    { /* disappear */ 
      pDBrecInst->Alarm.morphing.preState = pDBrecInst->Alarm.morphing.state;
      pDBrecInst->Alarm.morphing.property = MRM_ALARM_PROP_MD_DISAPPEAR;
      MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_event_req[inst=%d]: diag morphing CLEARED", instance);
    }

    /* pDBrecInst->Alarm.singleSideRx.property = MRM_ALARM_PROP_UNCHANGED; */
    if (pDBrecInst->Alarm.singleSideRx.state && !pDBrecInst->Alarm.singleSideRx.preState) 
    { /* appear */ 
      /* no signaling of ringOpen if singleSideRx */
      pDBrecInst->Alarm.ringOpen.state = LSA_FALSE;
      pDBrecInst->Alarm.singleSideRx.preState = pDBrecInst->Alarm.singleSideRx.state;
      pDBrecInst->Alarm.singleSideRx.property = MRM_ALARM_PROP_ERROR_APPEAR;
      MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_event_req[inst=%d]: diag singleSideRx SET", instance);
    }
    if (!pDBrecInst->Alarm.singleSideRx.state && pDBrecInst->Alarm.singleSideRx.preState) 
    { /* disappear */ 
      pDBrecInst->Alarm.singleSideRx.preState = pDBrecInst->Alarm.singleSideRx.state;
      pDBrecInst->Alarm.singleSideRx.property = MRM_ALARM_PROP_ERROR_DISAPPEAR;
      MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_event_req[inst=%d]: diag singleSideRx CLEARED", instance);
    }
    /* pDBrecInst->Alarm.ringOpen.property = MRM_ALARM_PROP_UNCHANGED; */
    if (pDBrecInst->Alarm.ringOpen.state && !pDBrecInst->Alarm.ringOpen.preState) 
    { /* appear */ 
      pDBrecInst->Alarm.ringOpen.preState = pDBrecInst->Alarm.ringOpen.state;
      pDBrecInst->Alarm.ringOpen.property = MRM_ALARM_PROP_MD_APPEAR;
      MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_event_req[inst=%d]: diag ringOpen SET", instance);
    }
    if (!pDBrecInst->Alarm.ringOpen.state && pDBrecInst->Alarm.ringOpen.preState) 
    { /* disappear */ 
      pDBrecInst->Alarm.ringOpen.preState = pDBrecInst->Alarm.ringOpen.state;
      pDBrecInst->Alarm.ringOpen.property = MRM_ALARM_PROP_MD_DISAPPEAR;
      MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_event_req[inst=%d]: diag ringOpen CLEARED", instance);
    }
    /* pDBrecInst->Alarm.multiRM.property = MRM_ALARM_PROP_UNCHANGED; */
    if (pDBrecInst->Alarm.multiRM.state && !pDBrecInst->Alarm.multiRM.preState) 
    { /* appear */ 
      pDBrecInst->Alarm.multiRM.preState = pDBrecInst->Alarm.multiRM.state;
      pDBrecInst->Alarm.multiRM.property = MRM_ALARM_PROP_MD_APPEAR;
      MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_event_req[inst=%d]: diag multiRM SET", instance);
    }
    if (!pDBrecInst->Alarm.multiRM.state && pDBrecInst->Alarm.multiRM.preState) 
    { /* disappear */ 
      pDBrecInst->Alarm.multiRM.preState = pDBrecInst->Alarm.multiRM.state;
      pDBrecInst->Alarm.multiRM.property = MRM_ALARM_PROP_MD_DISAPPEAR;
      MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_event_req[inst=%d]: diag multiRM CLEARED", instance);
    }

    (void)(mrp_rec_prm_ind_done (pChUsr));
  }
}


/*===========================================================================
* FUNCTION : mrp_rec_init
*----------------------------------------------------------------------------
* PURPOSE  : init record usr data
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChUsr = user channel reference
*            mrpMaxInstance = max. number of MRP instances 
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_rec_init (const MRP_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpInMaxInstance)
{
  LSA_UINT16 instance, k, numOfPorts;
  LSA_UINT32 timerIdx;
  MRP_REC_DB_TYPE *pDBrec = LSA_NULL;
  MRP_REC_INST_DATA_TYPE *pDBrecInst;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChUsr->pChSysDB->pEdd;
  MRP_IF_DB_TYPE *pDBif;
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChUsr->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  MRP_PROGRAM_TRACE_00(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_init entered");

  switch (pChUsr->detailptr->PathType)
  {
  case MRP_PATH_TYPE_USER:
    MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBrec), sizeof(MRP_REC_DB_TYPE));
    MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBrec, LSA_NULL));
    pChUsr->pChUsrDB->pRec = pDBrec;

    /* preset zero */
    MRP_MEMSET(pDBrec, 0, sizeof(MRP_REC_DB_TYPE));

    numOfPorts = mrp_sys_get_num_of_ports (pChUsr); /* each user channel has a reference to DBsys ! */

    /* check MRP_Prio */
    if (mrp_rec_parse_helper_prio (pChUsr->detailptr->type.user.PrioDefault) == MRP_RSP_ERR_PARAM) 
    {
      MRP_PROGRAM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_FATAL, ">> value not allowed PrioDefault=0x%x", pChUsr->detailptr->type.user.PrioDefault);
      MRP_FATAL;
    }

    /**/
    pDBrec->pChSys = pDBedd->pChSys;
    pDBrec->pChPrmUsr = pChUsr;
    pDBrec->firstPrmCommit = LSA_TRUE;

    /**/
    MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBrec->ppRqbIndicationBot), (LSA_UINT16)(sizeof(MRP_RQB_TYPE*) * (numOfPorts + 1)));
    MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBrec->ppRqbIndicationBot, LSA_NULL));
    
    MRP_MEMSET(pDBrec->ppRqbIndicationBot, 0, (LSA_UINT16)(sizeof(MRP_RQB_TYPE*) * (numOfPorts + 1)));
    /**/
    MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBrec->ppRqbIndicationTop), (LSA_UINT16)(sizeof(MRP_RQB_TYPE*) * (numOfPorts + 1)));
    MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBrec->ppRqbIndicationTop, LSA_NULL));
    
    MRP_MEMSET(pDBrec->ppRqbIndicationTop, 0, (LSA_UINT16)(sizeof(MRP_RQB_TYPE*) * (numOfPorts + 1)));

    pDBrec->PrmEnd   = LSA_FALSE;
    pDBrec->PrmState = MRP_REC_ENUM_STATE_PRM_INIT;
    
    MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBrec->pAlarmOutOfResource), (LSA_UINT16)(sizeof(LSA_UINT8)*(numOfPorts + 1)));
    MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBrec->pAlarmOutOfResource, LSA_NULL));

    MRP_MEMSET(pDBrec->pAlarmOutOfResource, MRP_ALARM_OUT_OF_RESOURCE_FALSE,(LSA_UINT16)(sizeof(LSA_UINT8) * (numOfPorts + 1)));
    
    pDBrec->DefaultPrio = pChUsr->detailptr->type.user.PrioDefault;
    pDBrec->SupportedRoleInst0 = mrp_rec_map_edd_role_cap_inst0 (pChUsr);
    pDBrec->SupportedMultiRole = mrp_rec_map_edd_multi_role_cap(pChUsr);
    pDBrec->DefaultRoleInst0   = mrp_rec_map_edd_def_role (pChUsr);
    pDBrec->NumOfPossibleRports = mrp_rec_get_rport_cnt (pChUsr);

    pDBrec->SupportedInRoleInst0 = mrp_rec_map_edd_ic_role_cap_inst0(pChUsr);
    pDBrec->NumOfPossibleInPorts = mrp_sys_get_inport_cnt (pChUsr);
    
    pDBrec->DefaultRPort_1 = 0;
    pDBrec->DefaultRPort_2 = 0;

    if (mrpMaxInstance && !pDBrec->NumOfPossibleRports)
    {
      MRP_PROGRAM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_FATAL, "mrp_rec_init: inconsistency in initialization: mrpMaxInstance=%d, NumOfPossibleRports must not be 0!", mrpMaxInstance);
      MRP_FATAL;
    }

    if (pDBrec->NumOfPossibleRports) 
    {
      MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBrec->pRPortArray), (LSA_UINT16)(sizeof(LSA_UINT16) * pDBrec->NumOfPossibleRports));
      MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBrec->pRPortArray, LSA_NULL));
      MRP_MEMSET(pDBrec->pRPortArray, 0, (LSA_UINT16)(sizeof(LSA_UINT16) * pDBrec->NumOfPossibleRports));   

      mrp_rec_map_edd_ports_to_rports (pChUsr);
      
      if (pDBrec->DefaultRoleInst0 != MRP_PROJ_ROLE_OFF) 
      {
        pDBrec->DefaultRPort_1 = pDBrec->pRPortArray[0];
        pDBrec->DefaultRPort_2 = pDBrec->pRPortArray[1];
      }
    }

    pDBif = (MRP_IF_DB_TYPE *)pDBrec->pChSys->pChSysDB->pInterfacePara;
    /* According to the User Story the following check needs to be done. */
    if (pDBrec->SupportedInRoleInst0 != EDD_SUPPORTED_MRP_INTERCONN_ROLE_CAP_NONE && (mrpInMaxInstance - pDBif->mrpRstpInstances) > pDBrec->NumOfPossibleInPorts)
    {
      MRP_PROGRAM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_FATAL, "mrp_rec_init: inconsistency in initialization: mrpInMaxInstance=%d (incl. RSTP+), RSTP+Instances=%d, NumOfPossibleInPorts=%d, NumOfPossibleInPorts must be equal or higher than the max number of MRP interconnection instances configured (excluding RSTP+)!", mrpInMaxInstance, pDBif->mrpRstpInstances, pDBrec->NumOfPossibleInPorts);
      MRP_FATAL;
    }

    /* this is to identify and store the possible MRP_IN-ports. There is NO default port for MRP_IN, so nothing else to be done. */
    if (pDBrec->NumOfPossibleInPorts)
    {
      MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBrec->pInPortArray), (LSA_UINT16)(sizeof(LSA_UINT16) * pDBrec->NumOfPossibleInPorts));
      MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBrec->pInPortArray, LSA_NULL));
      MRP_MEMSET(pDBrec->pInPortArray, 0, (LSA_UINT16)(sizeof(LSA_UINT16) * pDBrec->NumOfPossibleInPorts));

      mrp_rec_map_edd_ports_to_inports(pChUsr);
    }
      
    if (mrpMaxInstance)
    {
      MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBrec->pInstData), (LSA_UINT16)(sizeof(MRP_REC_INST_DATA_TYPE) * mrpMaxInstance));
      MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBrec->pInstData, LSA_NULL));
    }

    for (instance = 0; instance < mrpMaxInstance; instance++)
    {
      pDBrecInst = &pDBrec->pInstData[instance];
      MRP_MEMSET(pDBrecInst, 0, sizeof(MRP_REC_INST_DATA_TYPE));
      
      if (pDBrec->NumOfPossibleRports) 
      {
        MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBrecInst->para.RecPortAdjust), (LSA_UINT16)(sizeof(MRP_PORT_DATA_ADJUST_TYPE) * MRP_DEFAULT_VAL_INST_RPORT_CNT));
        MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBrec->pInstData[instance].para.RecPortAdjust, LSA_NULL));
        MRP_MEMSET(pDBrecInst->para.RecPortAdjust, 0, (LSA_UINT16)(sizeof(MRP_PORT_DATA_ADJUST_TYPE) * MRP_DEFAULT_VAL_INST_RPORT_CNT));   

        MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBrecInst->work.RecPortAdjust), (LSA_UINT16)(sizeof(MRP_PORT_DATA_ADJUST_TYPE) * MRP_DEFAULT_VAL_INST_RPORT_CNT));
        MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBrecInst->work.RecPortAdjust, LSA_NULL));
        MRP_MEMSET(pDBrecInst->work.RecPortAdjust, 0, (LSA_UINT16)(sizeof(MRP_PORT_DATA_ADJUST_TYPE) * MRP_DEFAULT_VAL_INST_RPORT_CNT));   

        MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBrecInst->saveWork.RecPortAdjust), (LSA_UINT16)(sizeof(MRP_PORT_DATA_ADJUST_TYPE) * MRP_DEFAULT_VAL_INST_RPORT_CNT));
        MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBrecInst->saveWork.RecPortAdjust, LSA_NULL));
        MRP_MEMSET(pDBrecInst->saveWork.RecPortAdjust, 0, (LSA_UINT16)(sizeof(MRP_PORT_DATA_ADJUST_TYPE) * MRP_DEFAULT_VAL_INST_RPORT_CNT));   

        for (k = 0; k < MRP_DEFAULT_VAL_INST_RPORT_CNT; k++)
        {
          MRP_FATAL1((pDBrec->pRPortArray[k] != 0) && (pDBrec->pRPortArray[k] <= numOfPorts));
          
          if (instance == MRP_DEFAULT_INSTANCE) /* default instance */
          {
            pDBrecInst->work.RecPortAdjust[k].PortID     = pDBrec->pRPortArray[k]; /*default rport1 */
            pDBrecInst->para.RecPortAdjust[k].PortID     = pDBrec->pRPortArray[k]; /*default rport2 */
            pDBrecInst->saveWork.RecPortAdjust[k].PortID = pDBrecInst->work.RecPortAdjust[k].PortID;           
          }
          else
          {
            pDBrecInst->work.RecPortAdjust[k].PortID     = 0;
            pDBrecInst->para.RecPortAdjust[k].PortID     = 0;
            pDBrecInst->saveWork.RecPortAdjust[k].PortID = 0;        
          }
        }
        if (!instance)
          MRP_FATAL1(pDBrecInst->work.RecPortAdjust[0].PortID != pDBrecInst->work.RecPortAdjust[1].PortID);
      }
        

#ifdef MRP_CFG_PLUGIN_2_HSR
      MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)&pDBrec->pInstData[instance].para.RecPrivStByAdjust.pPortID, (LSA_UINT16)(sizeof(LSA_UINT8) * numOfPorts));
      MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBrec->pInstData[instance].para.RecPrivStByAdjust.pPortID, LSA_NULL));
      MRP_MEMSET(pDBrec->pInstData[instance].para.RecPrivStByAdjust.pPortID, 0, (LSA_UINT16)(sizeof(LSA_UINT8) * numOfPorts));   

      MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)&pDBrec->pInstData[instance].work.RecPrivStByAdjust.pPortID, (LSA_UINT16)(sizeof(LSA_UINT8) * numOfPorts));
      MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBrec->pInstData[instance].work.RecPrivStByAdjust.pPortID, LSA_NULL));
      MRP_MEMSET(pDBrec->pInstData[instance].work.RecPrivStByAdjust.pPortID, 0, (LSA_UINT16)(sizeof(LSA_UINT8) * numOfPorts));   

      MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)&pDBrec->pInstData[instance].saveWork.RecPrivStByAdjust.pPortID, (LSA_UINT16)(sizeof(LSA_UINT8) * numOfPorts));
      MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBrec->pInstData[instance].saveWork.RecPrivStByAdjust.pPortID, LSA_NULL));
      MRP_MEMSET(pDBrec->pInstData[instance].saveWork.RecPrivStByAdjust.pPortID, 0, (LSA_UINT16)(sizeof(LSA_UINT8) * numOfPorts));   
#endif

      pDBrec->pInstData[instance].AlarmBlocked = LSA_TRUE;
      MRP_MEMSET(&pDBrec->pInstData[instance].AlarmIndicated, 0, sizeof(pDBrec->pInstData[instance].AlarmIndicated));
      MRP_MEMSET(&pDBrec->pInstData[instance].Alarm, 0, sizeof(pDBrec->pInstData[instance].Alarm));
    
      pDBrec->pInstData[instance].TimerIdAlarm = mrp_sys_timer_get (pChUsr, MRP_TIMER_POOL_INSTANCE, instance, LSA_TIMER_TYPE_ONE_SHOT, MRP_TIMER_ENUM_MRP_ALARM, (MRP_TIMER_CBF_FCT_PTR_TYPE)mrp_rec_timer_cbf_alarm);
      timerIdx = pDBrec->pInstData[instance].TimerIdAlarm % MRP_MAX_NO_OF_TIMER_IDS_PER_INTERFACE;
      MRP_PROGRAM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_init[inst=%d]: timer_get called for TimerIdAlarm: id=%d, idx=%d",
        instance, pDBrec->pInstData[instance].TimerIdAlarm, timerIdx); 

      LSA_UNUSED_ARG(timerIdx);   /* hhr: to please the compiler in case traces are disabled. */

      pDBrec->pInstData[instance].TimerAlarmTimeTcnt = MRP_DEFAULT_VAL_MRM_ALARM_DELAY_T_MS/MRP_TIMER_TIME_BASE_DEZ;
    }
    
    /* allocate and initialize data sets for MRP_IN instanzes*/
    if (mrpInMaxInstance)
    {
      /* pInstData[MRP_DEFAULT_INSTANCE].para data*/
      MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBrec->pInstData[MRP_DEFAULT_INSTANCE].para.pRecIcPortAdjust), (LSA_UINT16)(sizeof(MRP_IC_PORT_DATA_ADJUST_TYPE) * mrpInMaxInstance));
      MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBrec->pInstData[MRP_DEFAULT_INSTANCE].para.pRecIcPortAdjust, LSA_NULL));
      /* initialization is not needed here because it will be done in "mrp_rec_set_inst_para_defaults". */

      MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBrec->pInstData[MRP_DEFAULT_INSTANCE].para.pRecIcPortCheck), (LSA_UINT16)(sizeof(MRP_IC_PORT_DATA_CHECK_TYPE) * mrpInMaxInstance));
      MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBrec->pInstData[MRP_DEFAULT_INSTANCE].para.pRecIcPortCheck, LSA_NULL));
      /* initialization is not needed here because it will be done in "mrp_rec_set_inst_para_defaults". */

      /* pInstData[MRP_DEFAULT_INSTANCE].work data*/
      MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBrec->pInstData[MRP_DEFAULT_INSTANCE].work.pRecIcPortAdjust), (LSA_UINT16)(sizeof(MRP_IC_PORT_DATA_ADJUST_TYPE) * mrpInMaxInstance));
      MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBrec->pInstData[MRP_DEFAULT_INSTANCE].work.pRecIcPortAdjust, LSA_NULL));
      /* initialization is not needed here because it will be done in "mrp_rec_set_inst_para_defaults". */

      MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBrec->pInstData[MRP_DEFAULT_INSTANCE].work.pRecIcPortCheck), (LSA_UINT16)(sizeof(MRP_IC_PORT_DATA_CHECK_TYPE) * mrpInMaxInstance));
      MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBrec->pInstData[MRP_DEFAULT_INSTANCE].work.pRecIcPortCheck, LSA_NULL));
      /* initialization is not needed here because it will be done in "mrp_rec_set_inst_para_defaults". */

      /* pInstData[MRP_DEFAULT_INSTANCE].saveWork data*/
      MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBrec->pInstData[MRP_DEFAULT_INSTANCE].saveWork.pRecIcPortAdjust), (LSA_UINT16)(sizeof(MRP_IC_PORT_DATA_ADJUST_TYPE) * mrpInMaxInstance));
      MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBrec->pInstData[MRP_DEFAULT_INSTANCE].saveWork.pRecIcPortAdjust, LSA_NULL));
      /* initialization is not needed here because it will be done in "mrp_rec_set_inst_para_defaults". */

      MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBrec->pInstData[MRP_DEFAULT_INSTANCE].saveWork.pRecIcPortCheck), (LSA_UINT16)(sizeof(MRP_IC_PORT_DATA_CHECK_TYPE) * mrpInMaxInstance));
      MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBrec->pInstData[MRP_DEFAULT_INSTANCE].saveWork.pRecIcPortCheck, LSA_NULL));
      /* initialization is not needed here because it will be done in "mrp_rec_set_inst_para_defaults". */
    }


    /* set default values for interface parameters */
    mrp_rec_set_if_para_defaults (pDBrec);
    mrp_rec_set_if_para2work(pDBrec);
    
    /* set default values for instance parameters */
    for (instance = 0; instance < mrpMaxInstance; instance++)
    {
      mrp_rec_set_inst_para_defaults (pDBrec, instance, MRP_REC_ENUM_DEFAULT_FROM_STD);
      mrp_rec_set_inst_para2work (pDBrec, instance);
    }
    break;

  case MRP_PATH_TYPE_USER_2:
    /* don't alloc new instance for USER_2 */
    break;

  default:
    MRP_FATAL;
    break;
  }

}


/*===========================================================================
* FUNCTION : mrp_rec_deinit
*----------------------------------------------------------------------------
* PURPOSE  : deinit record usr data
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChUsr=user channel reference
*            mrpMaxInstance = max. number of MRP instances 
*            mrpInMaxInstance = max. number of MRP IN instances 
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_rec_deinit (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpInMaxInstance)
{
  LSA_UINT16  rc, instance;
  MRP_REC_DB_TYPE *pDBrec = (MRP_REC_DB_TYPE *)pChUsr->pChUsrDB->pRec;

  MRP_PROGRAM_TRACE_01(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_rec_deinit entered, mrpMaxInstance=%d", mrpMaxInstance);

  switch(pChUsr->detailptr->PathType)
  {
  case MRP_PATH_TYPE_USER:
    MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBrec->ppRqbIndicationBot);
    MRP_FATAL1(rc == LSA_RET_OK);
    pDBrec->ppRqbIndicationBot = LSA_NULL;
    
    MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBrec->ppRqbIndicationTop);
    MRP_FATAL1(rc == LSA_RET_OK);
    pDBrec->ppRqbIndicationTop = LSA_NULL;
    
    MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBrec->pAlarmOutOfResource);
    MRP_FATAL1(rc == LSA_RET_OK);
    pDBrec->pAlarmOutOfResource = LSA_NULL;
    
    if (pDBrec->NumOfPossibleRports) 
    {
      if (!(LSA_HOST_PTR_ARE_EQUAL(pDBrec->pRPortArray, LSA_NULL)) )
      {
        MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBrec->pRPortArray);
        MRP_FATAL1(rc == LSA_RET_OK);

        pDBrec->pRPortArray = LSA_NULL;
      }
    }

    if (pDBrec->NumOfPossibleInPorts)
    {
      if (!(LSA_HOST_PTR_ARE_EQUAL(pDBrec->pInPortArray, LSA_NULL)))
      {
        MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBrec->pInPortArray);
        MRP_FATAL1(rc == LSA_RET_OK);

        pDBrec->pInPortArray = LSA_NULL;
      }
    }

    /* free all local MEM related to the MRP IN instanzes */
    if (mrpInMaxInstance)
    {
        /* pInstData->para data*/
        MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBrec->pInstData[MRP_DEFAULT_INSTANCE].para.pRecIcPortAdjust);
        MRP_FATAL1(rc == LSA_RET_OK);
        pDBrec->pInstData[MRP_DEFAULT_INSTANCE].para.pRecIcPortAdjust = LSA_NULL;
        MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBrec->pInstData[MRP_DEFAULT_INSTANCE].para.pRecIcPortCheck);
        MRP_FATAL1(rc == LSA_RET_OK);
        pDBrec->pInstData[MRP_DEFAULT_INSTANCE].para.pRecIcPortCheck = LSA_NULL;

        /* pInstData->work data*/
        MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBrec->pInstData[MRP_DEFAULT_INSTANCE].work.pRecIcPortAdjust);
        MRP_FATAL1(rc == LSA_RET_OK);
        pDBrec->pInstData[MRP_DEFAULT_INSTANCE].work.pRecIcPortAdjust = LSA_NULL;
        MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBrec->pInstData[MRP_DEFAULT_INSTANCE].work.pRecIcPortCheck);
        MRP_FATAL1(rc == LSA_RET_OK);
        pDBrec->pInstData[MRP_DEFAULT_INSTANCE].work.pRecIcPortCheck = LSA_NULL;

        /* pInstData->saveWork data*/
        MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBrec->pInstData[MRP_DEFAULT_INSTANCE].saveWork.pRecIcPortAdjust);
        MRP_FATAL1(rc == LSA_RET_OK);
        pDBrec->pInstData[MRP_DEFAULT_INSTANCE].saveWork.pRecIcPortAdjust = LSA_NULL;
        MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBrec->pInstData[MRP_DEFAULT_INSTANCE].saveWork.pRecIcPortCheck);
        MRP_FATAL1(rc == LSA_RET_OK);
        pDBrec->pInstData[MRP_DEFAULT_INSTANCE].saveWork.pRecIcPortCheck = LSA_NULL;
    }

    for (instance = 0; instance < mrpMaxInstance; instance++)
    {
      mrp_sys_timer_free (pDBrec->pInstData[instance].TimerIdAlarm);
      
      if (pDBrec->NumOfPossibleRports) 
      {
        MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBrec->pInstData[instance].para.RecPortAdjust);
        MRP_FATAL1(rc == LSA_RET_OK);
        
        MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBrec->pInstData[instance].work.RecPortAdjust);
        MRP_FATAL1(rc == LSA_RET_OK);
        
        MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBrec->pInstData[instance].saveWork.RecPortAdjust);
        MRP_FATAL1(rc == LSA_RET_OK);
      }
        
#ifdef MRP_CFG_PLUGIN_2_HSR
      MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBrec->pInstData[instance].para.RecPrivStByAdjust.pPortID);
      MRP_FATAL1(rc == LSA_RET_OK);
        
      MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBrec->pInstData[instance].work.RecPrivStByAdjust.pPortID);
      MRP_FATAL1(rc == LSA_RET_OK);
        
      MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBrec->pInstData[instance].saveWork.RecPrivStByAdjust.pPortID);
      MRP_FATAL1(rc == LSA_RET_OK);
#endif    

      MRP_MEMSET(&(pDBrec->pInstData[instance]), 0, sizeof(MRP_REC_INST_DATA_TYPE));
    }

    if (mrpMaxInstance)
    {
      MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBrec->pInstData);
      MRP_FATAL1(rc == LSA_RET_OK);
    }
    pDBrec->pInstData = LSA_NULL;

    pDBrec->pChSys = LSA_NULL;
    pDBrec->pChPrmUsr = LSA_NULL;
    pDBrec->NumOfPossibleRports = 0;

    MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBrec);
    MRP_FATAL1(rc == LSA_RET_OK);

    pChUsr->pChUsrDB->pRec = LSA_NULL;
    break;

  case MRP_PATH_TYPE_USER_2:
    /* no instance available for USER_2 or OHA user path */
    break;

  default:
    MRP_FATAL;
    break;
  }

}
