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
/*  F i l e               &F: pndv_r1_bridge.h                          :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/*
 * pndv_r1_bridge.h
 *
 *  Created on: 03.02.2015
 *      Author: cn3dit09
 */

#ifndef PNDV_R1_BRIDGE_H_
#define PNDV_R1_BRIDGE_H_

//////////////////////////////////////
///exported C like function prototypes
//////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif
void        pndv_r1_bridge_in_handle_provide(CM_UPPER_RQB_PTR_TYPE rqb_ptr);
UnsignedOpt pndv_r1_bridge_in_handle_ind(CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_r1_bridge_in_data_receive_ind(Unsigned16 rcve_length, Unsigned8 *rcve_data);
void        pndv_r1_bridge_in_handle_req_response(CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_r1_bridge_in_handle_control_response(CM_UPPER_RQB_PTR_TYPE rqb_ptr);
UnsignedOpt pndv_r1_bridge_cm_response(CM_UPPER_RQB_PTR_TYPE rqb_ptr);
void        pndv_r1_bridge_cm_provide_res(void);
void        pndv_r1_bridge_in_partner_found_master_ind();
void        pndv_r1_bridge_in_partner_found_standby_ind();
void        pndv_r1_bridge_in_partner_lost_ind();
void        pndv_r1_bridge_in_send_data_done();
int         pndv_r1_bridge_enable_bridge(void);

void pndv_r1_bridge_init(int im_position);
void pndv_r1_bridge_open(PNDV_R1_OPEN_DATA_PTR open_data_ptr);

bool pndv_r1_bridge_backplane_state_active(void);
void pndv_r1_bridge_backplane_state_inactive(void);


#ifdef __cplusplus
}
#endif

//////////////////////////
///selector id definitions
//////////////////////////
#define PNDV_R1_BRIDGE_SELECTOR_ID_NOTIFY         PPPN_SELECTOR_ID_NOTIFY  /* always 0 */
#define PNDV_R1_BRIDGE_SELECTOR_ID_CM_DATA        1
#define PNDV_R1_BRIDGE_SELECTOR_ID_CM_SERVICE_ACK 2
#define PNDV_R1_BRIDGE_SELECTOR_ID_MAX            3

////////////////////////////
///buffer length definitions
////////////////////////////
#define PNDV_R1_BRIDGE_PPPN_DATA_SIZE_NOTIFY         (10 + 4) /* + 4 bytes of CM's internal version */
#define PNDV_R1_BRIDGE_PPPN_DATA_SIZE_CM_DATA        IM_PDB_MAX_RPC_DATA_LEN + 0x180 /* 0xff80 */
#define PNDV_R1_BRIDGE_PPPN_DATA_SIZE_CM_SERVICE_ACK 1
#define PNDV_R1_BRIDGE_CM_VERSION                    CM_SV_R1BRIDGE_VERSION

typedef enum PNDV_R1_BRIDGE_STATE_E
{
    PNDV_R1_BRIDGE_STATE_UNKNOWN = 0,
    PNDV_R1_BRIDGE_STATE_INIT    = 1,
    PNDV_R1_BRIDGE_STATE_OPEN    = 2,
    PNDV_R1_BRIDGE_STATE_START   = 3

}   PNDV_R1_BRIDGE_STATE_T;

//////////////////////////////////////////////////////////////////////////////////////////////////
///enum describing the needed conditions which must be present i. o. t. use the PPPN communication
//////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum PNDV_R1_BRIDGE_COUPLING_CONDITIONS_E
{
    ///a partner must be found
    PNDV_R1_BRIDGE_PARTNER_FOUND =      (0x00000001 << 0),

    ///the remote bridge has got a compatible version
    PNDV_R1_BRIDGE_COMPATIBLE_VERSION = (0x00000001 << 1),

    ///the local bridge (PNDV) must be fully set up and working
    PNDV_R1_BRIDGE_DEVICE_ACTIVATED =   (0x00000001 << 2)

}   PNDV_R1_BRIDGE_COUPLING_CONDITIONS_T;

///////////////////////////////////////////////
///mask of the needed conditions for using PPPN
///////////////////////////////////////////////
#define PNDV_R1_BRIDGE_NEEDED_COUPLING_CONDITIONS_MASK ( (int) ( PNDV_R1_BRIDGE_COMPATIBLE_VERSION | PNDV_R1_BRIDGE_PARTNER_FOUND | PNDV_R1_BRIDGE_DEVICE_ACTIVATED ) )

#define PNDV_R1_BRIDGE_ADD_NEEDED_COUPLING_CONDITION(__COND_BIT) pndv_r1_bridge_data.couplingConditions |= (__COND_BIT)

#define PNDV_R1_BRIDGE_REMOVE_NEEDED_COUPLING_CONDITION(__COND_BIT) pndv_r1_bridge_data.couplingConditions &= ( ~(__COND_BIT) )

#define PNDV_R1_BRIDGE_IS_COUPLING_ENABLE()  ((pndv_r1_bridge_data.couplingConditions & PNDV_R1_BRIDGE_NEEDED_COUPLING_CONDITIONS_MASK) == PNDV_R1_BRIDGE_NEEDED_COUPLING_CONDITIONS_MASK )

#ifdef __cplusplus

///////////////////////
///forward declarations
///////////////////////
class PNDVR1BridgeManager;
class SWOPppnManager;

typedef struct PNDV_R1_BRIDGE_DATA_S
{
    PNDV_R1_BRIDGE_STATE_T sm;
    int                    couplingConditions;
    unsigned int           activeAtBackplane;
    int                    aufdatenIsRunning;
    int                    im_position;

    uint8_t                partnerCMversion[4];

    PNDVR1BridgeManager    *R1BridgeManager;

    CM_UPPER_RQB_PTR_TYPE  ind_rqb_ptr;

}   PNDV_R1_BRIDGE_DATA_T;


//////////////////////////////////////
///manager class needed for using PPPN
//////////////////////////////////////
class PNDVR1BridgeManager : public SWOPppnManagerInterface
{
public:

    PNDVR1BridgeManager();
    virtual ~PNDVR1BridgeManager();

    virtual void        open_channel_done();
    virtual void        partner_found(Unsigned8 *data_ptr, Unsigned16 data_len);
    virtual void        partner_lost();

    virtual Unsigned16  set_version(Unsigned8 *data_ptr);    //!< user has to fill in the own component version data

    virtual void        send_confirmation(Unsigned8 selector_id);  //!< a send req is done
    virtual void        send_repetition(Unsigned8 selector_id);  //!< after send_done a pending request can be repeted

    virtual void        rcve_indication (Unsigned8 selector_id, Unsigned16 rcve_length, Unsigned8 *rcve_data);  //!< a receive from partner is occure

    bool sendData(uint16_t length);
    uint8_t* getSendBuffer();

    void start();
    void openPPPNchannels();

    void setPPPNdata(uint8_t pppnHandle, uint8_t pppnSendPrio);
    SWOPppnManager* getPppnManager() { return _pppnManager; };

protected:

    void sendServiceAck();

    bool checkVersion(uint8_t *data_ptr, uint16_t data_len);

    ////////////////////////////////////////////////////////
    ///the data length values of the needed PPPN connections
    ////////////////////////////////////////////////////////
    uint16_t _selectorLength[PNDV_R1_BRIDGE_SELECTOR_ID_MAX];

    /////////////////////////////
    ///data needed for using PPPN
    /////////////////////////////
    uint8_t _pppnHandle;
    uint8_t _pppnSendPrio;

    //////////////////////////////////////////////////////
    ///manager class used for using communication via PPPN
    //////////////////////////////////////////////////////
    SWOPppnManager *_pppnManager;

    ////////////////////////////////////////////////////////////////
    ///flag indicating if a ack towards the remote bridge is running
    ////////////////////////////////////////////////////////////////
    bool _isServiceAckSending;

    ///////////////////////////////////////////////////////////
    ///flag indicating if a request of the local bridge is sent
    ///////////////////////////////////////////////////////////
    bool _isRequestSent;
};

#endif

#endif /* PNDV_R1_BRIDGE_H_ */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
