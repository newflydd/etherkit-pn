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
/*  F i l e               &F: l4_usr.h                                  :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  User Interface for LSA - common L4 Transport:                            */
/*  Defines constants, types and macros for L4.                              */
/*                                                                           */
/*****************************************************************************/

#ifndef L4_USR_H                         /* ----- reinclude-protection ----- */
#define L4_USR_H       

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

/***********************************************************************************/
/*                                                                                 */
/*  H i s t o r y :                                                                */
/*  ______________________________________________________________________________ */
/*                                                                                 */
/*  Date        Version        Who  What                                           */
/*  2000-11-23  P 01.00.01.03  ws   For some variables, the type of which is       */
/*                                  a pointer, the identifier "_ptr" was           */
/*                                  missing in the variable name. This complements.*/
/*                                  Subject: ta_buffer, t_params and buffer        */
/*                                  from L4-RQB. The corresponding macros          */
/*                                  have been adjusted accordingly:                */
/*                                  L4_RQB_SET/GET_TA_BUFFER_PTR,                  */
/*                                  L4_RQB_SET/GET_T_PARAMS_PTR und                */
/*                                  L4_RQB_SET/GET_STATISTIC_BUFFER_PTR            */
/*  2000-11-23  P 01.00.01.03  ws   Macros L4_RQB_SET/GET_STATE_TA_BUFFER_PTR      */
/*                                  were missing. Added.                           */
/*  2001-03-12  P01.00.01.004  ws   Opcode L4_OPC_WITHDRAW_AWAIT_CONNECT_TG        */
/*                                  new                                            */
/*  2001-07-26  P02.00.01.001  ws   All RQB macros deleted.                        */
/*  2001-07-26  P02.00.01.001  ws   Several L4 names replaced by LSA names.        */
/*  2001-07-27  P02.00.01.001  ws   Opcodes L4_OPC_GET_PARAMS,                     */
/*                                  L4_OPC_SET_PARAMS, L4_OPC_GET_STATISTIC,       */
/*                                  L4_OPC_ROUTE_ADD, L4_OPC_ROUTE_REM,            */
/*                                  L4_OPC_ROUTE_DEL, L4_OPC_REMOTE_STATE          */
/*                                  and L4_OPC_SET_RESOURCES and associated        */
/*                                  RQB structures and RQB macros deleted.         */
/*  2001-07-27  P02.00.01.001  ws   __L4_USER_H changed to L4_USER_H.              */
/*  2001-07-27  P02.00.01.001  ws   Casts deleted.                                 */
/*  2001-07-27  P02.00.01.001  ws   Represent numbers of the constants             */
/*                                  L4_DPT_FN_ * hexadecimal instead of decimal.   */
/*  2001-07-30  P02.00.01.001  ws   Protection against re-inclusion moved to       */
/*                                  the beginning of the file.                     */
/*  2001-09-26  P02.00.01.001  ws   L4_UPPER_TA_BUFFER_PTR_TYPE no longer          */
/*                                  points to LSA_VOID but to                      */
/*                                  struct l4_rqb_tag.                             */
/*  2001-09-26  P02.00.01.001  ws   Deleted in L4_TA_BUFFER_TYPE alignment.        */
/*  2001-09-28  P02.00.01.001  ws   L4_RSP_OK_HIGHEST and L4_RSP_ERR_HIGHEST       */
/*                                  deleted.                                       */
/*  2001-10-08  P02.00.01.001  ws   Comments added.                                */
/*  2001-10-09  P02.00.01.001  ws   Definition of L4_ERR_CODE_ID_UNUSED            */
/*                                  changed to avoid "zero".                       */
/*  2001-12-03  P02.00.01.001  ws   L4_RQB_ARGS_CONN_TYPE extended                 */
/*                                  by next_target_staddr parameter.               */
/*  2002-01-31  P02.00.01.002  ws   Comments added and changed.                    */
/*                                  RQB description significantly expanded.        */
/*  2002-03-11  P02.00.01.003  ws   Comments of "length", "offset" and "ptr"       */
/*                                  in the structure                               */
/*                                  L4_RQB_ARGS_DATA_BUFFER_TYPE changed.          */
/*  2002-07-02  P2.0.0.0_0.1.5.1 ws ARTS 99656: Defines L4_RET_ * new.             */
/*                                  Is only required if an L4 function             */
/*                                  interface is used instead of L4-RQBs           */
/*                                  for special users.                             */
/*  2003-05-06  P2.1.0.0_0.2.1.1 ws ARTS + AP00007688 and AP00007692:              */
/*                                  RQB error structure changed:                   */
/*                                  L4_RQB_ERROR_TYPE deleted;                     */
/*                                  LSA_RQB_LOCAL_ERROR_TYPE,                      */
/*                                  L4_RQB_ABORT_REASON_TYPE new;                  */
/*                                  L4_LOCATION_LOC and L4_LOCATION_REM            */
/*                                  deleted; L4_ERR_CODE_ID_ * renamed             */
/*                                  to L4_PROTOCOL_ERROR_CODE_ID_ *.               */
/*  2003-05-19  P2.1.0.0_0.2.1.1 ws ARTS + AP00007733: standardize response        */
/*                                   values: comments adapted;                     */
/*                                  L4_OPC_DS_READ and L4_OPC_DS_WRITE             */
/*                                  deleted.                                       */
/*  2003-06-05  P2.1.0.0_0.2.1.1 ws ARTS+ AP00011658: Variable response            */
/*                                  belongs to the RQB header: Variable            */
/*                                  response moved from l4_usr.h to l4_cfg.h.      */
/*  2003-08-26  P2.1.0.0_0.2.2.1 ws ARTS+ AP00104436: Incorrectly deleted          */
/*                                  constants L4_DPT_ * implemented again.         */
/*  2003-12-08  P2.1.0.0_0.3.1.1 ws ARTS+ AP00132507: Fixed nested comments        */
/*                                  at L4_TA_BUFFER_TYPE                           */
/*  2003-12-08  P2.1.0.0_0.3.1.1 ws ARTS+ AP00007733: Standardize response         */
/*                                  values: constants L4_RSP_ERR_SYNTAX,           */
/*                                  L4_RET_ERR_SYNTAX, L4_RSP_ERR_DEV and          */
/*                                  L4_RET_ERR_DEV deleted because                 */
/*                                  unnecessary; Comment on                        */
/*                                  L4_RSP_ERR_SYS_PATH and                        */
/*                                  L4_RET_ERR_SYS_PATH added; List of which       */
/*                                  response values occur with which opcode        */
/*                                  may be updated.                                */
/*  2003-12-09  P2.1.0.0_0.3.1.1 ws ARTS+ AP00125986: Compiler warning by          */
/*                                  forward declaration                            */
/*                                  "struct l4_rqb_tag;" eliminated.               */
/*  2004-06-30  P2.1.0.0_0.4.1.1 ws ARTS+ AP00132068: Marked for C++               */
/*                                  compiler file with C binding.                  */
/*  2004-06-30  P2.1.0.0_0.4.1.1 ws ARTS+ AP00167479: LSA_RSP_* new: Defined       */
/*                                  where possible L4_RSP_ * on LSA_RSP_ *         */
/*  2004-06-30  P2.1.0.0_0.4.1.1 ws ARTS+ AP00167458: Copyright changed.           */
/*  2004-07-01  P2.1.0.0_0.4.1.1 ws ARTS+ AP00007733: Standardize response         */
/*                                  values: L4_RSP_ERR_PARAM added in the          */
/*                                  comment for several opcodes.                   */
/*  2005-07-22  P2.1.0.0_0.5.1.1 ws Trace: New: Pseudo macros as scanner           */
/*                                  identification for the symbolic                */
/*                                  display of constants.                          */
/*  2005-07-25  P2.1.0.0_0.5.1.1 ws ARTS+ AP00240150: New:                         */
/*                                  L4_TA_BUFFER_HEADER_SIZE                       */
/*  2005-11-14  P2.1.0.0_0.6.1.1 ws ARTS+ AP00256160: Due to component OUC         */
/*                                  opcodes, constants, an L4_RQB_ARGS             */
/*                                  element added and two existing                 */
/*                                  L4_RQB_ARGS elements expanded.                 */
/*  2005-11-15  P2.1.0.0_0.6.1.1 ws ARTS+ AP00256160: In the comment at            */
/*                                  L4_OPC_AWAIT_DISCONNECT Response               */
/*                                  L4_RSP_OK added.                               */
/*  2006-03-29  P2.1.0.0_0.7.1.1 ws At L4_RQB_ABORT_REASON_TYPE                    */
/*                                  comment added.                                 */
/*  2006-10-11  P2.1.0.0_0.7.3.1 ws ARTS+ AP00370825: "Abort-reason-type           */
/*                                  within RQB" for the DPT interface              */
/*                                  changed comments.                              */
/*  2006-10-11  P2.1.0.0_0.7.3.1 ws ARTS+ AP00370850: Comment adjusted             */
/*                                  in the structure L4_RQB_ARGS_CONN_TYPE         */
/*                                  for the parameter ta_buffer_ptr.               */
/*  2007-03-22  P2.1.0.0_0.8.1.1 ws ARTS+ AP00407162: Only for LSA component       */
/*                                  KA and KA users added:                         */
/*                                  constants L4_CN_TYPE_ * new and structure      */
/*                                  L4_RQB_ARGS_CN_TYPE extended                   */
/*                                  by variables cn_type and cpu_id.               */
/*  2007-07-05  P2.1.0.0_0.9.1.1 ws ARTS+ AP00469994: Only for LSA component       */
/*                                  OUC and OUC users:                             */
/*                                  constant L4_PROTOCOL_TYPE_UDP_MULTICAST.       */
/*  2007-07-05  P2.1.0.0_0.9.1.1 ws ARTS+ AP00485927: For all components           */
/*                                  related to DP transport, NRS-PDU               */
/*                                  constants for byte protocol_error_code         */
/*                                  [0] have been added.                           */
/*  2007-08-08 P2.1.0.0_0.10.1.1 ws ARTS+ AP00407162: Only for LSA component       */
/*                                  KA and KA users added:                         */
/*                                  Structure L4_RQB_ARGS_CN_TYPE extended         */
/*                                  by variable upper_cn_ref.                      */
/*  2008-01-25 P2.1.0.0_0.11.1.1 ws ARTS+ AP00568912: Response                     */
/*                                  L4_RSP_ERR_ADDRESS_IN_USE new.                 */
/*  2008-12-01 P2.1.0.0_0.11.2.1 ws Corrected spelling mistakes in the comment.    */
/*  2008-12-03 P2.1.0.0_0.11.2.1 ws ARTS+ AP00743700: Corrected the tag name       */
/*                                  for the structures L4_RQB_ARGS_DATA_TYPE       */
/*                                  and L4_RQB_ARGS_UDP_DATA_TYPE                  */
/*  2008-11-20 P2.1.0.0_0.12.1.1 ws ARTS+ AP00543735: Functionality                */
/*                                  "coupling and updating (AuA)" new:             */
/*                                  opcodes, response, ARGS structure added.       */
/*  2009-01-27 P2.1.0.0_0.12.1.1 ws ARTS+ AP00609374: Constant                     */
/*                                  L4_MAX_TA_BUFFER_LENGTH new.                   */
/*  2009-04-03 P2.1.0.0_0.12.3.1 ws ARTS+ AP00240150/AP00609374:                   */
/*                                  L4_TA_BUFFER_TYPE changed, definition          */
/*                                  of L4_TA_BUFFER_HEADER_SIZE deleted.           */
/*  2010-02-05 P2.1.0.0_0.12.4.1 ws ARTS+ AP00908402: For LSA component            */
/*                                  BPLX and BPLX users:                           */
/*                                  Defines L4_PROTOCOL_TYPE_ABPPLUS_L2 and        */
/*                                  L4_PROTOCOL_TYPE_ABPPLUS_L4 new.               */
/*  2010-06-04 P2.1.0.0_0.13.1.1 ws ARTS+ AP00960468: Callback function:           */
/*                                  Typedef                                        */
/*                                  L4_UPPER_CALLBACK_FCT_PTR_TYPE new.            */
/*                                  L4_RQB_ARGS_CHANNEL_TYPE formally              */
/*                                  adapted.                                       */
/*  2010-06-07 P2.1.0.0_0.13.1.1 ws ARTS+ AP00960512: Response                     */
/*                                  L4_RSP_ERR_RESOURCE with the opcodes           */
/*                                  L4_OPC_AWAIT_DISCONNECT and                    */
/*                                  L4_OPC_AWAIT_CLOSE added.                      */
/*  2010-06-07 P2.1.0.0_0.13.1.1 ws ARTS+ AP00960561: Response                     */
/*                                  L4_RSP_ERR_LOC_NET with the                    */
/*                                  opcodes L4_OPC_AWAIT_CONNECT_U and             */
/*                                  L4_OPC_AWAIT_CONNECT_T added.                  */
/*                                                                                 */
/***********************************************************************************/



/*===========================================================================*/
/*                            constants for l4                               */
/*===========================================================================*/

/*=====  opcodes within RQB  =====*/
PTRC_SUBLABEL_LIST_OPEN("L4_OPC")
#define L4_OPC_OPEN                       0
#define L4_OPC_CONNECT                    1
#define L4_OPC_AWAIT_CONNECT_T            2
#define L4_OPC_AWAIT_CONNECT_U            3
#define L4_OPC_ACCEPT                     4
#define L4_OPC_SEND                       5    /* send data without */
                                               /* "End Of Message"  */
#define L4_OPC_SEND_EOM                   6    /* send data with   */
                                               /* "End Of Message" */
#define L4_OPC_RECEIVE                    7    /* receive data */
#define L4_OPC_DISCONNECT                 8
#define L4_OPC_AWAIT_DISCONNECT          11
#define L4_OPC_CLOSE                     12
#define L4_OPC_AWAIT_CLOSE               13
#define L4_OPC_OPEN_CHANNEL              22
#define L4_OPC_CLOSE_CHANNEL             23
#define L4_OPC_AWAIT_CONNECT_NT          24
#define L4_OPC_AWAIT_CONNECT_NU          25
#define L4_OPC_AWAIT_CONNECT_TG          26
#define L4_OPC_AWAIT_CONNECT_R           27
#define L4_OPC_WITHDRAW_AWAIT_CONNECT_N  30
#define L4_OPC_REC_SEND                  33    /* send record without */
                                               /* "End Of Message"    */
#define L4_OPC_REC_SEND_EOM              34    /* send record with */
                                               /* "End Of Message" */
#define L4_OPC_REC_RECEIVE               35    /* receive record */
#define L4_OPC_WITHDRAW_AWAIT_CONNECT_TG 36
#define L4_OPC_UDP_SEND_EOM              37     
#define L4_OPC_UDP_RECEIVE               38
#define L4_OPC_AUA_STOP_CN               40    /* Aua: Stop connection */
#define L4_OPC_AUA_UPDATE_CN             41    /* Aua: Update connection */
#define L4_OPC_AUA_UNDO_UPDATE_CN        42    /* Aua: Undo update of */
                                               /*      connection     */
#define L4_OPC_AUA_START_CN              43    /* Aua: Start connection */
#define L4_OPC_UNUSED                  0xff
PTRC_SUBLABEL_LIST_CLOSE("L4_OPC")


/*=====  response codes within RQB  =====*/
PTRC_SUBLABEL_LIST_OPEN("L4_RSP")
#define L4_RSP_OK                     LSA_RSP_OK 
                                               /* without errors */
#define L4_RSP_OK_ACTIVE              0x02     /* RQB is active */  
#define L4_RSP_OK_EOM                 0x03     /* without errors with */
                                               /* "End Of Message"    */

#define L4_RSP_ERR_RESOURCE        /* 0x84 */ LSA_RSP_ERR_RESOURCE 
                                               /* no local resources       */
                                               /* available (memory, LSAP, */
                                               /* timer, ...)              */
#define L4_RSP_ERR_REF                0x85     /* wrong cn-id */
#define L4_RSP_ERR_PARAM           /* 0x86 */ LSA_RSP_ERR_PARAM
                                               /* any parameters in the RQB */
                                               /* are faulty, incl. opcode  */
#define L4_RSP_ERR_SEQUENCE        /* 0x87 */ LSA_RSP_ERR_SEQUENCE
                                               /* wrong state */
#define L4_RSP_ERR_LOC_ABORT          0x88     /* abort of a running request */
                                               /* by a consequence request   */
#define L4_RSP_ERR_REM_ABORT          0x89     /* connection abort by a */
                                               /* remote partner        */
#define L4_RSP_ERR_LOC_NET            0x8a     /* general problems with    */
                                               /* local/virtual network or */
                                               /* local/virtual station    */
#define L4_RSP_ERR_REM_NET            0x8b     /* remote station isn't */
                                               /* obtainable           */
#define L4_RSP_ERR_PROTOCOL           0x8c     /* unexpected or into himself */
                                               /* faulty PDU of the remote   */
                                               /* partner                    */
#define L4_RSP_ERR_TIMEOUT            0x8d     /* a layer 4 supervision */ 
                                               /* time has occurred     */
#define L4_RSP_ERR_ROUTE              0x8f     /* no routing information */
                                               /* available              */
#define L4_RSP_ERR_OWN_ADDR           0x90     /* addressing error: remote */
                                               /* address is own address   */
#define L4_RSP_ERR_SYS_PATH        /* 0x91 */ LSA_RSP_ERR_SYS_PATH
                                               /* error at *_GET_PATH_INFO() */
                                               /* or if the contents of the  */
                                               /* detail-pointer isn't       */
                                               /* correct or inconsistent    */
#define L4_RSP_ERR_LOWER_LAYER        0x92     /* error message of the lower */
                                               /* layer or in context with   */
                                               /* the lower layer            */
#define L4_RSP_ERR_WITHDRAW           0x93     /* WITHDRAW couldn't be       */
                                               /* executed since either no   */
                                               /* suitable AWAIT_CONNECT was */
                                               /* there or the AWAIT_CONNECT */
                                               /* couldn't be fetched back   */
                                               /* of the lower layer         */
#define L4_RSP_ERR_ADDRESS_IN_USE     0x94     /* addressing error: Address  */
                                               /* is already in use          */
#define L4_RSP_ERR_AUA_UPDATE         0x95     /* update was not possible    */
#define L4_RSP_UNUSED              /* 0xff */ LSA_RSP_UNUSED
                                               /* unused */
PTRC_SUBLABEL_LIST_CLOSE("L4_RSP")

/* use of L4_RSP_*:                                                          */
/*                                                                           */
/* if "opcode invalid":                                                      */
/* L4_RSP_ERR_PARAM                                                          */
/*                                                                           */
/* if L4_OPC_OPEN_CHANNEL:                                                   */
/* L4_RSP_OK, (L4_RSP_ERR_SEQUENCE,) L4_RSP_ERR_PARAM, L4_RSP_ERR_RESOURCE,  */
/* L4_RSP_ERR_SYS_PATH, L4_RSP_ERR_LOWER_LAYER                               */
/*                                                                           */
/* if L4_OPC_CLOSE_CHANNEL:                                                  */
/* L4_RSP_OK, L4_RSP_ERR_PARAM, L4_RSP_ERR_SEQUENCE                          */
/*                                                                           */
/* if L4_OPC_OPEN:                                                           */
/* L4_RSP_OK, L4_RSP_ERR_PARAM, L4_RSP_ERR_SEQUENCE, L4_RSP_ERR_RESOURCE,    */
/* L4_RSP_ERR_LOC_ABORT                                                      */
/*                                                                           */
/* if L4_OPC_CONNECT:                                                        */
/* L4_RSP_OK, L4_RSP_ERR_PARAM, L4_RSP_ERR_REF, L4_RSP_ERR_SEQUENCE,         */
/* L4_RSP_ERR_ROUTE, L4_RSP_ERR_RESOURCE, L4_RSP_ERR_LOC_NET,                */
/* L4_RSP_ERR_REM_NET, L4_RSP_ERR_OWN_ADDR, L4_RSP_ERR_LOC_ABORT,            */
/* L4_RSP_ERR_REM_ABORT, L4_RSP_ERR_PROTOCOL, L4_RSP_ERR_TIMEOUT,            */
/* L4_RSP_ERR_ADDRESS_IN_USE                                                 */
/*                                                                           */
/* if L4_OPC_AWAIT_CONNECT_U:                                                */
/* L4_RSP_OK, L4_RSP_ERR_PARAM, L4_RSP_ERR_REF, L4_RSP_ERR_SEQUENCE,         */
/* L4_RSP_ERR_LOC_ABORT, L4_RSP_ERR_LOC_NET                                  */
/*                                                                           */
/* if L4_OPC_AWAIT_CONNECT_T:                                                */
/* L4_RSP_OK, L4_RSP_ERR_PARAM, L4_RSP_ERR_REF, L4_RSP_ERR_SEQUENCE,         */
/* L4_RSP_ERR_LOC_ABORT, L4_RSP_ERR_LOC_NET, L4_RSP_ERR_ADDRESS_IN_USE       */
/*                                                                           */
/* if L4_OPC_AWAIT_CONNECT_NU, L4_OPC_AWAIT_CONNECT_NT,                      */
/*    L4_OPC_AWAIT_CONNECT_R, L4_OPC_AWAIT_CONNECT_TG:                       */
/* L4_RSP_OK, L4_RSP_ERR_PARAM, L4_RSP_ERR_SEQUENCE, L4_RSP_ERR_LOC_ABORT,   */
/* L4_RSP_ERR_RESOURCE                                                       */
/*                                                                           */
/* if L4_OPC_WITHDRAW_AWAIT_CONNECT_N, L4_OPC_WITHDRAW_AWAIT_CONNECT_TG:     */
/* L4_RSP_OK, L4_RSP_ERR_PARAM, L4_RSP_ERR_SEQUENCE, L4_RSP_ERR_WITHDRAW     */
/*                                                                           */
/* if L4_OPC_ACCEPT:                                                         */
/* L4_RSP_OK, L4_RSP_ERR_PARAM, L4_RSP_ERR_REF, L4_RSP_ERR_SEQUENCE,         */
/* L4_RSP_ERR_RESOURCE, L4_RSP_ERR_LOC_ABORT, L4_RSP_ERR_REM_ABORT,          */
/* L4_RSP_ERR_LOC_NET, L4_RSP_ERR_REM_NET, L4_RSP_ERR_PROTOCOL,              */
/* L4_RSP_ERR_TIMEOUT                                                        */
/*                                                                           */
/* if L4_OPC_SEND, L4_OPC_SEND_EOM:                                          */
/* L4_RSP_OK, L4_RSP_ERR_PARAM, L4_RSP_ERR_REF, L4_RSP_ERR_SEQUENCE,         */
/* L4_RSP_ERR_RESOURCE, L4_RSP_ERR_LOC_ABORT, L4_RSP_ERR_REM_ABORT,          */
/* L4_RSP_ERR_LOC_NET, L4_RSP_ERR_REM_NET, L4_RSP_ERR_PROTOCOL,              */
/* L4_RSP_ERR_TIMEOUT                                                        */
/*                                                                           */
/* if L4_OPC_REC_SEND, L4_OPC_REC_SEND_EOM:                                  */
/* L4_RSP_OK, L4_RSP_ERR_PARAM, L4_RSP_ERR_REF, L4_RSP_ERR_SEQUENCE,         */
/* L4_RSP_ERR_LOC_ABORT, L4_RSP_ERR_REM_ABORT, L4_RSP_ERR_LOC_NET,           */
/* L4_RSP_ERR_REM_NET, L4_RSP_ERR_PROTOCOL, L4_RSP_ERR_TIMEOUT               */
/*                                                                           */
/* if L4_OPC_UDP_SEND_EOM:                                                   */
/* L4_RSP_OK, L4_RSP_ERR_PARAM, L4_RSP_ERR_REF, L4_RSP_ERR_SEQUENCE          */
/*                                                                           */
/* if L4_OPC_RECEIVE, L4_OPC_REC_RECEIVE:                                    */
/* L4_RSP_OK, L4_RSP_OK_EOM, L4_RSP_ERR_PARAM, L4_RSP_ERR_REF,               */
/* L4_RSP_ERR_SEQUENCE, L4_RSP_ERR_LOC_ABORT, L4_RSP_ERR_REM_ABORT,          */
/* L4_RSP_ERR_LOC_NET, L4_RSP_ERR_REM_NET, L4_RSP_ERR_PROTOCOL,              */
/* L4_RSP_ERR_TIMEOUT                                                        */
/*                                                                           */
/* if L4_OPC_UDP_RECEIVE:                                                    */
/* L4_RSP_OK_EOM, L4_RSP_ERR_PARAM, L4_RSP_ERR_REF, L4_RSP_ERR_SEQUENCE      */
/*                                                                           */
/* if L4_OPC_DISCONNECT, L4_OPC_CLOSE:                                       */
/* L4_RSP_OK, L4_RSP_ERR_PARAM, L4_RSP_ERR_REF, L4_RSP_ERR_SEQUENCE,         */
/* L4_RSP_ERR_LOC_ABORT                                                      */
/*                                                                           */
/* if L4_OPC_AWAIT_DISCONNECT:                                               */
/* L4_RSP_OK, L4_RSP_ERR_REF, L4_RSP_ERR_PARAM, L4_RSP_ERR_SEQUENCE,         */
/* L4_RSP_ERR_RESOURCE, L4_RSP_ERR_LOC_ABORT, L4_RSP_ERR_REM_ABORT,          */
/* L4_RSP_ERR_LOC_NET, L4_RSP_ERR_REM_NET, L4_RSP_ERR_PROTOCOL,              */
/* L4_RSP_ERR_TIMEOUT                                                        */
/*                                                                           */
/* if L4_OPC_AWAIT_CLOSE:                                                    */
/* L4_RSP_OK, L4_RSP_ERR_REF, L4_RSP_ERR_PARAM, L4_RSP_ERR_SEQUENCE,         */
/* L4_RSP_ERR_RESOURCE, L4_RSP_ERR_LOC_ABORT, L4_RSP_ERR_REM_ABORT,          */
/* L4_RSP_ERR_LOC_NET, L4_RSP_ERR_REM_NET, L4_RSP_ERR_PROTOCOL,              */
/* L4_RSP_ERR_TIMEOUT                                                        */
/*                                                                           */
/* if L4_OPC_AUA_STOP_CN:                                                    */
/* L4_RSP_OK, L4_RSP_ERR_PARAM, L4_RSP_ERR_SEQUENCE                          */
/*                                                                           */
/* if L4_OPC_AUA_UPDATE_CN:                                                  */
/* L4_RSP_OK, L4_RSP_ERR_PARAM, L4_RSP_ERR_SEQUENCE, L4_RSP_ERR_AUA_UPDATE   */
/*                                                                           */
/* if L4_OPC_AUA_UNDO_UPDATE_CN:                                             */
/* L4_RSP_OK, L4_RSP_ERR_PARAM, L4_RSP_ERR_SEQUENCE                          */
/*                                                                           */
/* if L4_OPC_AUA_START_CN:                                                   */
/* L4_RSP_OK, L4_RSP_ERR_PARAM, L4_RSP_ERR_SEQUENCE                          */


/*===== for abort-reason-type within RQB  =====*/

/* See SPH Kommunikation Band 1 Docu-Nr. 720053 / 1.03 V2.3 and DPV1 standard:  */
/* One has to differentiate between the coding                                  */
/* - an ABORT (DISCONNECT) PDU and                                              */
/* - an NRS PDU.                                                                */
/* With an ABORT PDU, the connection is cleared down,                           */
/* while with an NRS PDU only the respective service is rejected.               */
/* The connection itself remains                                                */
/* The codes for the ABORT PDUs are mentioned here. These codes stand for       */
/* the use of locally generated "ABORT PDUs". The ABORT PDUs consist of         */
/* Function_Num, Subnet and Instance / Reason code and these relevant codes     */
/* are shown here.                                                              */
/* The coding of the Error_Decode, Error_Code_1 and Error_Code_2 elements       */
/* of the NRS-PDU are only incomplete here. The function_codes of an NRS-PDU    */
/* are listed here so that a user of the Layer 4 interface can interpret them.  */

/* protocol_error_code_id */
/* how to interpret the following protocol_error_code[...] */
PTRC_SUBLABEL_LIST_OPEN("L4_PROTOCOL_ERROR_CODE_ID")
#define L4_PROTOCOL_ERROR_CODE_ID_DPT     0x0001             /* dp-transport */
#define L4_PROTOCOL_ERROR_CODE_ID_UNUSED  0xffff 
PTRC_SUBLABEL_LIST_CLOSE("L4_PROTOCOL_ERROR_CODE_ID")

/* protocol_error_code[...] for L4_PROTOCOL_ERROR_CODE_ID_DPT */
/* ABORT-PDU according norm:                                         */
/* for L4_OPC_CONNECT, L4_OPC_DISCONNECT and L4_OPC_AWAIT_DISCONNECT */

/* protocol_error_code[0]: function code */
/* --> Bit 7 = 0, Bit 6 = 1, Bit 5 = 0      */  
/* (Note: Bit 7 = 1 for NRS PDUs): */
#define L4_DPT_FUNCTION_NUM_EXTENSION 0x40
#define L4_DPT_ERR_EXTENSION          0x80

/* --> Bits 4 to 0: */
PTRC_SUBLABEL_LIST_OPEN("L4_DPT_FN")
#define L4_DPT_FN_CONNECT             0x17
#define L4_DPT_FN_DATA                0x11
#define L4_DPT_FN_DATA_TRANSPORT      0x11
#define L4_DPT_FN_DISCONNECT          0x18
#define L4_DPT_FN_IDLE                0x08
#define L4_DPT_FN_WAIT                0x08
#define L4_DPT_FN_PICKUP              0x09
#define L4_DPT_FN_RESSOURCE           0x16
#define L4_DPT_FN_DS_READ             0x1e
#define L4_DPT_FN_DS_WRITE            0x1f
PTRC_SUBLABEL_LIST_CLOSE("L4_DPT_FN")

PTRC_SUBLABEL_LIST_OPEN("L4_DPT_OPC")
#define L4_DPT_OPC_CONNECT         (L4_DPT_FN_CONNECT        | L4_DPT_FUNCTION_NUM_EXTENSION)
#define L4_DPT_OPC_DATA            (L4_DPT_FN_DATA           | L4_DPT_FUNCTION_NUM_EXTENSION)
#define L4_DPT_OPC_DATA_TRANSPORT  (L4_DPT_FN_DATA_TRANSPORT | L4_DPT_FUNCTION_NUM_EXTENSION)
#define L4_DPT_OPC_DISCONNECT      (L4_DPT_FN_DISCONNECT     | L4_DPT_FUNCTION_NUM_EXTENSION)
#define L4_DPT_OPC_IDLE            (L4_DPT_FN_IDLE           | L4_DPT_FUNCTION_NUM_EXTENSION)
#define L4_DPT_OPC_WAIT            (L4_DPT_FN_WAIT           | L4_DPT_FUNCTION_NUM_EXTENSION)
#define L4_DPT_OPC_PICKUP          (L4_DPT_FN_PICKUP         | L4_DPT_FUNCTION_NUM_EXTENSION)
#define L4_DPT_OPC_RESSOURCE       (L4_DPT_FN_RESSOURCE      | L4_DPT_FUNCTION_NUM_EXTENSION)
#define L4_DPT_OPC_DS_READ         (L4_DPT_FN_DS_READ        | L4_DPT_FUNCTION_NUM_EXTENSION) 
#define L4_DPT_OPC_DS_READ_NRS     (L4_DPT_FN_DS_READ        | L4_DPT_FUNCTION_NUM_EXTENSION | L4_DPT_ERR_EXTENSION) 
#define L4_DPT_OPC_DS_WRITE        (L4_DPT_FN_DS_WRITE       | L4_DPT_FUNCTION_NUM_EXTENSION) 
#define L4_DPT_OPC_DS_WRITE_NRS    (L4_DPT_FN_DS_WRITE       | L4_DPT_FUNCTION_NUM_EXTENSION | L4_DPT_ERR_EXTENSION) 
PTRC_SUBLABEL_LIST_CLOSE("L4_DPT_OPC")

/* protocol_error_code[1]: location where the error has been detected */
PTRC_SUBLABEL_LIST_OPEN("L4_DPT_DR_SUBNET")
#define L4_DPT_DR_SUBNET_LOC      0x01
#define L4_DPT_DR_SUBNET_REM      0x02
PTRC_SUBLABEL_LIST_CLOSE("L4_DPT_DR_SUBNET")


/* protocol_error_code[2]: reason code for abort  */
PTRC_SUBLABEL_LIST_OPEN("L4_DPT_DR_REASON")
/* --> Bits 7 und 6: Reserved */

/* --> Bits 5 und 4: */
#define L4_DPT_DR_INST_L2         0x00  /* instance (FDL)                    */
#define L4_DPT_DR_INST_L4         0x10  /* (DDLM)                            */
#define L4_DPT_DR_INST_USER       0x20  /* (USER)                            */

/* --> Bits 3 bis 0: */
#define L4_DPT_DR_DPT_SEQUENCE    0x01  /* sequence error                    */
#define L4_DPT_DR_DPT_REQUEST     0x02  /* invalid request PDU received      */
#define L4_DPT_DR_DPT_TIME        0x03  /* timeout of the connection         */ 
#define L4_DPT_DR_DPT_RESPONSE    0x04  /* invalid response PDU received     */
#define L4_DPT_DR_DPT_USER        0x05  /* invalid service from USER         */
#define L4_DPT_DR_DPT_STO         0x06  /* send_timeout requested too small  */ 
#define L4_DPT_DR_DPT_ADDR        0x07  /* invalid additional address info   */ 
#define L4_DPT_DR_DPT_OCON        0x08  /* waiting for FDL_DATA_REPLY.con    */
#define L4_DPT_DR_DPT_RESOURCE    0x0f  /* resource error                    */

#define L4_DPT_DR_USER_PROTOCOL   0x0b  /* layer7 protocol error             */ 
#define L4_DPT_DR_USER_ADDR       0x0c  /* invalid additional address info   */ 
#define L4_DPT_DR_USER_OK         0x0d  /* valid user abort                  */
#define L4_DPT_DR_USER_RESTART    0x0e  /* restart                           */
#define L4_DPT_DR_USER_RESOURCE   0x0f  /* resource error                    */
PTRC_SUBLABEL_LIST_CLOSE("L4_DPT_DR_REASON")

/* protocol_error_code[3]: reserved     */
PTRC_SUBLABEL_LIST_OPEN("L4_DPT_ERR_CODE")
#define L4_DPT_ERR_CODE_RESERVED  0x00
PTRC_SUBLABEL_LIST_CLOSE("L4_DPT_ERR_CODE")


/*=============================================================================
 *      function return-values
 *      If you use RQBs, you don't need this here. Use L4_RSP_* instead.
 *===========================================================================*/
PTRC_SUBLABEL_LIST_OPEN("L4_RET")
#define L4_RET_OK               L4_RSP_OK             /* without errors */
#define L4_RET_OK_ACTIVE        L4_RSP_OK_ACTIVE      /* function is active */  
#define L4_RET_OK_EOM           L4_RSP_OK_EOM         /* without errors with */
                                                      /* "End Of Message"    */

#define L4_RET_ERR_RESOURCE     L4_RSP_ERR_RESOURCE   /* no local resources */
                                                      /* available (memory, */
                                                      /* LSAP, timer, ...) */
#define L4_RET_ERR_REF          L4_RSP_ERR_REF        /* wrong cn-id */
#define L4_RET_ERR_PARAM        L4_RSP_ERR_PARAM      /* any parameters in   */
                                                      /* the RQB are faulty, */
                                                      /* incl. opcode        */
#define L4_RET_ERR_SEQUENCE     L4_RSP_ERR_SEQUENCE   /* wrong state */
#define L4_RET_ERR_LOC_ABORT    L4_RSP_ERR_LOC_ABORT  /* abort of a running  */
                                                      /* request by a        */
                                                      /* consequence request */
#define L4_RET_ERR_REM_ABORT    L4_RSP_ERR_REM_ABORT  /* connection abort by */
                                                      /* a remote partner    */
#define L4_RET_ERR_LOC_NET      L4_RSP_ERR_LOC_NET    /* general problems   */
                                                      /* with local/virtual */
                                                      /* network or local/  */
                                                      /* virtual station    */
#define L4_RET_ERR_REM_NET      L4_RSP_ERR_REM_NET    /* remote station   */
                                                      /* isn't obtainable */
#define L4_RET_ERR_PROTOCOL     L4_RSP_ERR_PROTOCOL   /* unexpected or into */
                                                      /* himself faulty PDU */
                                                      /* of the remote      */
                                                      /* partner            */
#define L4_RET_ERR_TIMEOUT      L4_RSP_ERR_TIMEOUT    /* a layer 4 super- */
                                                      /* vision time has  */
                                                      /* occurred         */
#define L4_RET_ERR_ROUTE        L4_RSP_ERR_ROUTE      /* no routing  */
                                                      /* information */
                                                      /* available   */
#define L4_RET_ERR_OWN_ADDR     L4_RSP_ERR_OWN_ADDR   /* addressing error: */
                                                      /* remote address    */
                                                      /* is own address    */
#define L4_RET_ERR_SYS_PATH     L4_RSP_ERR_SYS_PATH   /* error at           */
                                                      /* *_GET_PATH_INFO()  */
                                                      /* or if the contents */
                                                      /* of the detail-     */
                                                      /* pointer isn't      */
                                                      /* correct or         */
                                                      /* inconsistent       */
#define L4_RET_ERR_LOWER_LAYER  L4_RSP_ERR_LOWER_LAYER/* error message of    */
                                                      /* the lower layer or  */
                                                      /* in context with the */
                                                      /* lower layer         */
#define L4_RET_ERR_WITHDRAW     L4_RSP_ERR_WITHDRAW   /* WITHDRAW couldn't   */
                                                      /* be executed since   */
                                                      /* either no suitable  */
                                                      /* AWAIT_CONNECT was   */
                                                      /* there or the        */
                                                      /* AWAIT_CONNECT       */
                                                      /* couldn't be fetched */
                                                      /* back of the lower   */
                                                      /* layer               */
#define L4_RET_ERR_ADDRESS_IN_USE  L4_RSP_ERR_ADDRESS_IN_USE 
                                                      /* addressing error:   */
                                                      /* Address is already  */
                                                      /* in use              */
#define L4_RET_ERR_AUA_UPDATE      L4_RSP_ERR_AUA_UPDATE
                                                      /* update was not      */
                                                      /* possible            */

#define L4_RET_UNUSED           L4_RSP_UNUSED         /* unused */
PTRC_SUBLABEL_LIST_CLOSE("L4_RET")


/*=====  protocol types within RQB  =====*/
PTRC_SUBLABEL_LIST_OPEN("L4_PROTOCOL_TYPE")
/* Used only by LSA component OUC: */
#define L4_PROTOCOL_TYPE_RFC1006        1  /* Connection uses RFC1006        */
                                           /* protocol (LSA component STCP). */
#define L4_PROTOCOL_TYPE_TCP_NATIVE     2  /* Connection uses TCP interface */
                                           /* of LSA component SOCK.        */
#define L4_PROTOCOL_TYPE_UDP            3  /* Connection uses UDP interface */
                                           /* of LSA component SOCK.        */
#define L4_PROTOCOL_TYPE_UDP_MULTICAST  4  /* Connection uses UDP Multicast  */ 
                                           /* interface of LSA component SOCK*/

/* Used only by LSA component BPLx: */
#define L4_PROTOCOL_TYPE_ABPPLUS_L2  0x11  /* Connection uses ABP+        */
                                           /* (=Abstract BackPlane Plus)  */
                                           /* data link layer2 connection */
                                           /* protocol                    */ 
#define L4_PROTOCOL_TYPE_ABPPLUS_L4  0x12  /* Connection uses ABP+       */
                                           /* (=Abstract BackPlane Plus) */
                                           /* transport link layer4      */
                                           /* connection protocol        */ 

/* May be used for all other LSA layer 4 components: */
#define L4_PROTOCOL_TYPE_DEFAULT     0xff  /* Connection uses default */
                                           /* protocol type.          */
PTRC_SUBLABEL_LIST_CLOSE("L4_PROTOCOL_TYPE")

                                                    
/*=====  receive modes within RQB  =====*/
/* (It's used only by LSA component OUC.) */
PTRC_SUBLABEL_LIST_OPEN("L4_RQB_RCV_MODE")
#define L4_RQB_RCV_MODE_UNUSED   0  /* Because of the used protocol_type no */
                                    /* rcv_mode can be set.                 */
#define L4_RQB_RCV_MODE_AD_HOC   1  /* All data which the socket interface  */
                                    /* has received in a pass are           */
                                    /* immediately given high. One does not */
                                    /* wait for a certain number.           */
#define L4_RQB_RCV_MODE_ALL      2  /* One waits for the given number of     */
                                    /* data. Only if this number is reached, */
                                    /* the data are given high.              */
PTRC_SUBLABEL_LIST_CLOSE("L4_RQB_RCV_MODE")


/*=====  connection types within RQB  =====*/
/* (It's used only by LSA component KA and KA user.) */
PTRC_SUBLABEL_LIST_OPEN("L4_CN_TYPE")
#define L4_CN_TYPE_STANDARD      1  /* part of a standard connection */
#define L4_CN_TYPE_H             2  /* part of a highly available connection */
#define L4_CN_TYPE_UNUSED     0xff  /* unused */
PTRC_SUBLABEL_LIST_CLOSE("L4_CN_TYPE")


/*=====  size of l4 transport address buffer  =====*/
#define L4_MAX_TA_BUFFER_LENGTH                                               \
        (L4_TA_BUFFER_HEADER_SIZE + L4_MAX_TA_BUFFER_DYN_SIZE)



/*===========================================================================*/
/*                               types for l4                                */
/*===========================================================================*/

/*=============================================================================
 *      l4 common request block type                   
 *===========================================================================*/

/*=====  forward declaration  =====*/
struct l4_rqb_tag;

#define L4_UPPER_RQB_PTR_TYPE                      /* pointer to upper-RQB */ \
    struct l4_rqb_tag        L4_UPPER_RQB_ATTR *

#define L4_UPPER_MEM_PTR_TYPE                      /* pointer to upper-mem */ \
    LSA_VOID                 L4_UPPER_MEM_ATTR *

#define L4_UPPER_TA_BUFFER_PTR_TYPE          /* pointer to upper-ta-buffer */ \
    struct l4_ta_buffer_tag  L4_UPPER_TA_BUFFER_ATTR *


/*=====  arguments of l4 request block  =====*/

/* for L4_OPC_OPEN_CHANNEL, see *_open_channel() */
typedef  LSA_VOID  LSA_FCT_PTR(L4_UPPER_OUT_FCT_ATTR, 
                               L4_UPPER_CALLBACK_FCT_PTR_TYPE)(
    L4_UPPER_RQB_PTR_TYPE  upper_rqb_ptr
);

typedef struct l4_rqb_args_channel_tag
{
    LSA_HANDLE_TYPE                 handle;
                                    /* req: -                                */
                                    /* cnf: channel-handle of l4             */
                                    /* At all later RQBs user will write it  */
                                    /* into handle of L4_RQB_HEADER.         */
    LSA_HANDLE_TYPE                 handle_upper; 
                                    /* req: channel-handle of user           */
                                    /* cnf: -                                */
                                    /* At all confirmations l4 writes it     */
                                    /* into handle of L4_RQB_HEADER.         */
    LSA_SYS_PATH_TYPE               sys_path;
                                    /* req: system-path of channel           */
                                    /* cnf: -                                */
    L4_UPPER_CALLBACK_FCT_PTR_TYPE  l4_request_upper_done_ptr;
                                    /* req: pointer to callback-function     */
                                    /* cnf: -                                */
} L4_RQB_ARGS_CHANNEL_TYPE;

/* for L4_OPC_CLOSE_CHANNEL, see *_close_channel().
 * L4_RQB_ARGS_TYPE isn't needed.
 */

/* for L4_OPC_OPEN, L4_OPC_AWAIT_DISCONNECT, L4_OPC_DISCONNECT, 
 * L4_OPC_AWAIT_CLOSE, L4_OPC_CLOSE, see *_request() 
 */
typedef struct l4_rqb_args_cn_tag
{
    L4_CN_ID_TYPE                   cn_id;
                                    /* if L4_OPC_OPEN:                       */
                                    /*       req: -                          */
                                    /*       cnf: id of l4-connection        */
                                    /* else: req: id of l4-connection        */ 
                                    /*       cnf: -                          */
    LSA_UINT8                       cn_type;  
                                    /* for KA and KA user and L4_OPC_OPEN    */
                                    /* only:                                 */
                                    /* req: Connection type:                 */
                                    /*      - L4_CN_TYPE_STANDARD            */
                                    /*      - L4_CN_TYPE_H                   */
                                    /* cnf: unchanged                        */
    LSA_UINT8                       cpu_id; 
                                    /* for KA and KA user and L4_OPC_OPEN    */
                                    /* only:                                 */
                                    /* req: System ID of the CPU on which    */
                                    /*      the transport connection ends.   */
                                    /* cnf: unchanged                        */
    LSA_USER_ID_TYPE                upper_cn_ref;
                                    /* for KA and KA user and L4_OPC_OPEN    */
                                    /* only:                                 */
                                    /* req: Unique connection reference of   */
                                    /*      the user.                        */
                                    /* cnf: unchanged                        */
} L4_RQB_ARGS_CN_TYPE;

/* for L4_OPC_AWAIT_CONNECT_U, L4_OPC_AWAIT_CONNECT_T, L4_OPC_AWAIT_CONNECT_NU, 
 * L4_OPC_AWAIT_CONNECT_NT, L4_OPC_AWAIT_CONNECT_R, L4_OPC_AWAIT_CONNECT_TG,
 * L4_OPC_CONNECT, L4_OPC_ACCEPT, see *_request()
 */
typedef struct l4_rqb_args_conn_tag
{
    L4_CN_ID_TYPE                   cn_id;
                                    /* if L4_OPC_AWAIT_CONNECT_U,            */
                                    /* L4_OPC_AWAIT_CONNECT_T,               */
                                    /* L4_OPC_CONNECT, L4_OPC_ACCEPT:        */ 
                                    /*       req: id of l4-connection        */
                                    /*       cnf: -                          */
                                    /* else: req: -                          */
                                    /*       cnf: id of l4-connection        */
    LSA_UINT16                      max_tpdu_size; 
                                    /* for S7L4 and L4_OPC_AWAIT_CONNECT_T,  */
                                    /* L4_OPC_AWAIT_CONNECT_NT,              */
                                    /* L4_OPC_CONNECT, L4_OPC_ACCEPT only:   */ 
                                    /* req: maximum length of the            */
                                    /*      transportation PDUs              */
                                    /* cnf: -                                */
    LSA_UINT16                      persistance_count;
                                    /* for S7L4 and L4_OPC_CONNECT only:     */
                                    /* req: repeat counter                   */
                                    /* cnf: -                                */
    LSA_UINT16                      timeout_1;
                                    /* for S7L4 and L4_OPC_AWAIT_CONNECT_T,  */
                                    /* L4_OPC_AWAIT_CONNECT_NT,              */
                                    /* L4_OPC_CONNECT, L4_OPC_ACCEPT only:   */
                                    /* req: supervision time for reactions   */
                                    /*      of the remote station            */
                                    /* cnf: -                                */
    LSA_UINT16                      timeout_2;
                                    /* for DP/T-M and L4_OPC_CONNECT only:   */
                                    /* req: supervision time for the         */
                                    /*      connection between master and    */
                                    /*      slave                            */
                                    /* cnf: -                                */
    L4_UPPER_TA_BUFFER_PTR_TYPE     ta_buffer_ptr;
                                    /* for L4_OPC_AWAIT_CONNECT_U,           */ 
                                    /* L4_OPC_AWAIT_CONNECT_T,               */ 
                                    /* L4_OPC_AWAIT_CONNECT_NU,              */ 
                                    /* L4_OPC_AWAIT_CONNECT_NT,              */ 
                                    /* L4_OPC_AWAIT_CONNECT_R,               */ 
                                    /* L4_OPC_AWAIT_CONNECT_TG and           */ 
                                    /* L4_OPC_CONNECT only:                  */
                                    /* req: pointer to ta-buffer             */
                                    /* cnf: unchanged                        */
    LSA_UINT16                      features_supported;
                                    /* for DP/T-M and L4_OPC_CONNECT only:   */
                                    /*     req: -                            */
                                    /*     cnf: protocol variant of master   */
                                    /*           and slave                   */
                                    /* for V1SL and L4_OPC_AWAIT_CONNECT_U,  */ 
                                    /* L4_OPC_AWAIT_CONNECT_T,               */
                                    /* L4_OPC_AWAIT_CONNECT_NU,              */
                                    /* L4_OPC_AWAIT_CONNECT_NT,              */
                                    /* L4_OPC_AWAIT_CONNECT_TG only:         */
                                    /*     req: -                            */
                                    /*     cnf: protocol variant of master   */
                                    /* for V1SL and L4_OPC_ACCEPT only:      */ 
                                    /*     req: protocol variant of slave    */
                                    /*     cnf: -                            */
    LSA_UINT16                      profile_features;
                                    /* for DP/T-M and L4_OPC_CONNECT only:   */
                                    /*     req: profile services of master   */
                                    /*     cnf: profile services of slave    */
                                    /* for V1SL and L4_OPC_AWAIT_CONNECT_U,  */ 
                                    /* L4_OPC_AWAIT_CONNECT_T,               */
                                    /* L4_OPC_AWAIT_CONNECT_NU,              */
                                    /* L4_OPC_AWAIT_CONNECT_NT,              */
                                    /* L4_OPC_AWAIT_CONNECT_TG only:         */
                                    /*     req: -                            */
                                    /*     cnf: profile services of master   */
                                    /* for V1SL and L4_OPC_ACCEPT only:      */ 
                                    /*     req: profile services of slave    */
                                    /*     cnf: -                            */
    LSA_UINT16                      profile_ident_nr;
                                    /* for DP/T-M and L4_OPC_CONNECT only:   */
                                    /*     req: protocol profile of master   */
                                    /*     cnf: protocol profile of slave    */
                                    /* for V1SL and L4_OPC_AWAIT_CONNECT_U,  */ 
                                    /* L4_OPC_AWAIT_CONNECT_T,               */
                                    /* L4_OPC_AWAIT_CONNECT_NU,              */
                                    /* L4_OPC_AWAIT_CONNECT_NT,              */
                                    /* L4_OPC_AWAIT_CONNECT_TG only:         */
                                    /*     req: -                            */
                                    /*     cnf: protocol profile of master   */
                                    /* for V1SL and L4_OPC_ACCEPT only:      */ 
                                    /*     req: protocol profile of slave    */
                                    /*     cnf: -                            */
    LSA_UINT16                      next_target_staddr;
                                    /* for PC/TS-adapter and S7L4 and TG and */
                                    /* L4_OPC_AWAIT_CONNECT_TG only:         */ 
                                    /* req: -                                */
                                    /* cnf: next target stationaddress       */
    LSA_UINT8                       protocol_type;
                                    /* req: Which lower layer interface      */ 
                                    /*      the connection uses or which     */
                                    /*      protocol is used:                */
                                    /*      for OUC only:                    */
                                    /*      - L4_PROTOCOL_TYPE_RFC1006:      */
                                    /*        RFC1006                        */
                                    /*      - L4_PROTOCOL_TYPE_TCP_NATIVE:   */
                                    /*        TCP/IP-native                  */
                                    /*      - L4_PROTOCOL_TYPE_UDP:          */
                                    /*        UDP                            */
                                    /*      - L4_PROTOCOL_TYPE_UDP_MULTICAST:*/
                                    /*        UDP Multicast                  */
                                    /*      for BPLx only:                   */
                                    /*      - L4_PROTOCOL_TYPE_ABPPLUS_L2:   */
                                    /*        ABP+ layer 2                   */
                                    /*      - L4_PROTOCOL_TYPE_ABPPLUS_L4:   */
                                    /*        ABP+ layer 4                   */
                                    /*      all other components may use:    */
                                    /*      - L4_PROTOCOL_TYPE_DEFAULT:      */
                                    /*        Default protocol type          */  
} L4_RQB_ARGS_CONN_TYPE;

/* for L4_OPC_WITHDRAW_AWAIT_CONNECT_N, L4_OPC_WITHDRAW_AWAIT_CONNECT_TG,
 * see *_request(). L4_RQB_ARGS_TYPE isn't needed.
 */
 
/* for L4_OPC_SEND, L4_REC_OPC_SEND, L4_OPC_SEND_EOM, L4_OPC_REC_SEND_EOM,
 * L4_OPC_RECEIVE, L4_OPC_REC_RECEIVE, see *_request()
 */
typedef struct l4_rqb_args_data_buffer_tag
{
                               /* length of buffer >= "length" + "offset"    */
    LSA_UINT16                      length;
                               /* if L4_OPC_RECEIVE, L4_OPC_REC_RECEIVE:     */
                               /*      for *_RCV_MODE_REQ_WITH_BUFFER:       */
                               /*          req: number of bytes of memory    */
                               /*               size for valid data in this  */
                               /*               buffer                       */
                               /*          cnf: number of bytes of valid     */
                               /*               data in this buffer          */
                               /*      for *_RCV_MODE_REQ_WITHOUT_BUFFER:    */
                               /*          req: irrelevant                   */
                               /*          cnf: number of bytes of valid     */
                               /*               data in this buffer          */
                               /*      for *_RCV_MODE_NO_REQ:                */
                               /*          cnf: number of bytes of valid     */
                               /*               data in this buffer          */
                               /* else: req: number of bytes of valid data   */
                               /*            in this buffer                  */
                               /*       cnf: -                               */
    LSA_UINT16                      offset;
                               /* if L4_OPC_RECEIVE, L4_OPC_REC_RECEIVE:     */
                               /*      for *_RCV_MODE_REQ_WITH_BUFFER:       */
                               /*          req: place for first valid data   */
                               /*               in this buffer               */
                               /*          cnf: unchanged: first valid data  */
                               /*               in this buffer               */
                               /*      for *_RCV_MODE_REQ_WITHOUT_BUFFER:    */
                               /*          req: irrelevant                   */
                               /*          cnf: first valid data in this     */
                               /*               buffer                       */
                               /*      for *_RCV_MODE_NO_REQ:                */
                               /*          cnf: first valid data in this     */
                               /*               buffer                       */
                               /* else: req: first valid data in this buffer */
                               /*       cnf: -                               */
    L4_UPPER_MEM_PTR_TYPE           ptr;
                               /* if L4_OPC_RECEIVE, L4_OPC_REC_RECEIVE:     */
                               /*      for *_RCV_MODE_REQ_WITH_BUFFER:       */
                               /*          req: pointer to buffer            */
                               /*          cnf: unchanged                    */
                               /*      for *_RCV_MODE_REQ_WITHOUT_BUFFER:    */
                               /*          req: irrelevant                   */
                               /*          cnf: pointer to buffer            */
                               /*      for *_RCV_MODE_NO_REQ:                */
                               /*          cnf: pointer to buffer            */
                               /* else: req: pointer to buffer               */
                               /*       cnf: unchanged                       */
} L4_RQB_ARGS_DATA_BUFFER_TYPE;

typedef struct l4_rqb_args_data_tag
{
    L4_CN_ID_TYPE                   cn_id;
                                  /* req: id of l4-connection                */
                                  /* cnf: -                                  */
    LSA_UINT8                       rqb_rcv_mode;
                                  /* for OUC and L4_OPC_RECEIVE only:        */
                                  /* if L4_PROTOCOL_TYPE_TCP_NATIVE:         */
                                  /*     req: Distinction, whether the data, */
                                  /*          which have been received in a  */
                                  /*          pass, should be immediately    */
                                  /*          given high or whether exactly  */
                                  /*          one certain number of data     */
                                  /*          shall be received:             */
                                  /*          - L4_RQB_RCV_MODE_AD_HOC       */
                                  /*          - L4_RQB_RCV_MODE_ALL          */
                                  /* if L4_PROTOCOL_TYPE_RFC1006:            */
                                  /*     req: only L4_RQB_RCV_MODE_UNUSED    */
                                  /* cnf: -                                  */
    LSA_UINT16                      num_buffers; /*  <= L4_MAX_RQB_BUFFERS   */
                                  /* if L4_OPC_RECEIVE, L4_OPC_REC_RECEIVE:  */
                                  /*      for *_RCV_MODE_REQ_WITH_BUFFER:    */
                                  /*          req: number of buffers         */
                                  /*          cnf: unchanged                 */
                                  /*      for *_RCV_MODE_REQ_WITHOUT_BUFFER: */
                                  /*          req: 0                         */
                                  /*          cnf: number of buffers         */
                                  /*      for *_RCV_MODE_NO_REQ:             */
                                  /*          cnf: number of buffers         */
                                  /* else: req: number of buffers            */
                                  /*       cnf: unchanged                    */
    L4_RQB_ARGS_DATA_BUFFER_TYPE    buffer[L4_MAX_RQB_BUFFERS];
                                /* buffers, see L4_RQB_ARGS_DATA_BUFFER_TYPE */
} L4_RQB_ARGS_DATA_TYPE;

/* for OUC and L4_OPC_UDP_SEND_EOM, L4_OPC_UDP_RECEIVE only, see *_request()
 */
typedef struct l4_rqb_args_udp_data_tag
{
    L4_CN_ID_TYPE                   cn_id;
                                  /* req: id of l4-connection                */
                                  /* cnf: -                                  */
    LSA_UINT16                      num_buffers; /*  <= L4_MAX_RQB_BUFFERS   */
                                  /* if L4_OPC_UDP_SEND_EOM:                 */
                                  /*       req: number of buffers            */
                                  /*       cnf: unchanged                    */
                                  /* if L4_OPC_UDP_RECEIVE:                  */
                                  /*      for *_RCV_MODE_REQ_WITH_BUFFER:    */
                                  /*          req: number of buffers         */
                                  /*          cnf: unchanged                 */
                                  /*      for *_RCV_MODE_REQ_WITHOUT_BUFFER: */
                                  /*          req: 0                         */
                                  /*          cnf: number of buffers         */
                                  /*      for *_RCV_MODE_NO_REQ:             */
                                  /*          cnf: number of buffers         */
    L4_RQB_ARGS_DATA_BUFFER_TYPE    buffer[L4_MAX_RQB_BUFFERS];
                                /* buffers, see L4_RQB_ARGS_DATA_BUFFER_TYPE */
    L4_UPPER_TA_BUFFER_PTR_TYPE     ta_buffer_ptr;
                                  /* if L4_OPC_UDP_SEND_EOM:                 */
                                  /*     req: pointer to ta-buffer with      */
                                  /*          address data                   */
                                  /*     cnf: unchanged                      */
                                  /* if L4_OPC_UDP_RECEIVE:                  */
                                  /*      for *_RCV_MODE_REQ_WITH_BUFFER:    */
                                  /*          req: pointer to ta-buffer      */
                                  /*               without address data      */
                                  /*          cnf: pointer to ta-buffer      */
                                  /*               with address data         */
                                  /*      for *_RCV_MODE_REQ_WITHOUT_BUFFER: */
                                  /*          req: irrelevant                */
                                  /*          cnf: pointer to ta-buffer      */
                                  /*               with address data         */
                                  /*      for *_RCV_MODE_NO_REQ:             */
                                  /*          cnf: pointer to ta-buffer      */
                                  /*               with address data         */
} L4_RQB_ARGS_UDP_DATA_TYPE;

/* for HV, DSIF, KA and L4_OPC_AUA_START_CN, L4_OPC_AUA_STOP_CN,
 * L4_OPC_AUA_UPDATE_CN, L4_OPC_AUA_UNDO_UPDATE_CN only, see *_request()
 */
typedef struct l4_rqb_args_aua_cn_tag
{
    LSA_UINT16                      cn_number;
                                            /* req: number of cn_ids in list */
                                            /* cnf: number of cn_ids in list */
    L4_CN_ID_TYPE                   cn_id[L4_MAX_AUA_CN_NUMBERS];
                                            /* req: id of l4-connection */
                                            /* cnf: id of l4-connection  */
} L4_RQB_ARGS_AUA_CN_TYPE;


/*=====  union of arguments of l4 request block  =====*/
typedef union l4_rqb_args_tag
{
    L4_RQB_ARGS_CHANNEL_TYPE        channel; /* see L4_RQB_ARGS_CHANNEL_TYPE */
    L4_RQB_ARGS_CN_TYPE             cn;      /* see L4_RQB_ARGS_CN_TYPE      */
    L4_RQB_ARGS_CONN_TYPE           conn;    /* see L4_RQB_ARGS_CONN_TYPE    */
    L4_RQB_ARGS_DATA_TYPE           data;    /* see L4_RQB_ARGS_DATA_TYPE    */
    L4_RQB_ARGS_UDP_DATA_TYPE       udp;     /* see L4_RQB_ARGS_UDP_DATA_TYPE*/
    L4_RQB_ARGS_AUA_CN_TYPE         aua;     /* see L4_RQB_ARGS_AUA_CN_TYPE  */
} L4_RQB_ARGS_TYPE;


/*=====  abort reason type of l4 request block  =====*/
/* used by DPT components only: */
typedef struct l4_rqb_abort_reason_tag
{
    LSA_UINT16  protocol_error_code_id; /* how to interpret the following    */
                                        /* protocol_error_code[...]          */
                                        /* see above                         */ 
    LSA_UINT8   protocol_error_code[L4_PROTOCOL_ERROR_CODE_ARRAY_LENGTH];
                                        /* see above                         */
} L4_RQB_ABORT_REASON_TYPE;


/*=====  l4 request block definition  =====*/
typedef struct l4_rqb_tag
{
    L4_RQB_HEADER                            /* L4-RQB-header, see file      */
                                             /* l4_cfg.h                     */
    LSA_RQB_LOCAL_ERROR_TYPE        err;     /* req: -                       */
                                             /* cnf: error from lower layer  */
                                             /* see LSA_RQB_LOCAL_ERROR_TYPE */
    L4_RQB_ARGS_TYPE                args;    /* see  L4_RQB_ARGS_TYPE        */
    L4_RQB_ABORT_REASON_TYPE        abort;   /* used by DPT components only: */
                                             /* if L4_OPC_DISCONNECT:        */
                                             /* req: abort reason            */
                                             /* cnf: -                       */
                                             /* if L4_OPC_CONNECT,           */
                                             /* L4_OPC_AWAIT_DISCONNECT:     */
                                             /* req: -                       */
                                             /* cnf: abort reason            */
                                             /* see L4_RQB_ABORT_REASON_TYPE */
    L4_RQB_TRAILER                           /* additional RQB-trailer, see  */
                                             /* file l4_cfg.h                */
} L4_RQB_TYPE;



/*=============================================================================
 *      l4 transport address buffer              
 *===========================================================================*/

typedef struct l4_ta_buffer_tag
{
    LSA_UINT16  length;            /* number of bytes of ta-buffer including */
                                   /* dynamic part of ta-buffer              */
    LSA_UINT8   device_id;         /* id of physical interface */
    LSA_UINT8   loc_subnet_id_len; /* length of local subnet-id array */
    LSA_UINT8   loc_staddr_len;    /* length of local station address array */
    LSA_UINT8   loc_tsap_id_len;   /* length of local tsap-id array */
    LSA_UINT8   loc_tsap_id_len2;  /* length of an optional 2. local tsap-id */
                                   /* array                                  */
    LSA_UINT8   rem_subnet_id_len; /* length of remote subnet-id array */
    LSA_UINT8   rem_staddr_len;    /* length of remote station address array */
    LSA_UINT8   rem_tsap_id_len;   /* length of remote tsap-id array */
    LSA_UINT8   rem_tsap_id_len2;  /* length of an optional 2. remote */
                                   /* tsap-id array                   */
    LSA_UINT8   next_staddr_len;   /* length of next station address array */
    LSA_UINT8   loc_subnet_id[L4_MAX_TA_BUFFER_DYN_SIZE]; /* = 92 LSA_UINT8s */
/*
    loc_subnet_id[L4_MAX_TA_BUFFER_DYN_SIZE]: dynamic part(!) contains:
    LSA_UINT8   loc_subnet_id[];      local subnet-id array
    LSA_UINT8   loc_staddr[];         local station address array
    LSA_UINT8   loc_tsap_id[];        local tsap-id array 
    LSA_UINT8   loc_tsap_id2[];       optional 2. local tsap-id array 
    LSA_UINT8   rem_subnet_id[];      remote subnet-id array 
    LSA_UINT8   rem_staddr[];         remote station address array 
    LSA_UINT8   rem_tsap_id[];        remote tsap-id array 
    LSA_UINT8   rem_tsap_id2[];       optional 2. remote tsap-id array
    LSA_UINT8   next_staddr[];        next station address array 
*/
} L4_TA_BUFFER_TYPE;



/*===========================================================================*/
/*                             macros for l4                                 */
/*===========================================================================*/



/*****************************************************************************/
/*  end of file l4_usr.h                                                     */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of L4_USR_H */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
