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
/*  F i l e               &F: pndv_r1.h                                 :F&  */
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
 * pndv_r1.h
 *
 *  Created on: 23.05.2015
 *      Author: cn3rae00
 */

#ifndef PNDV_R1_H_
#define PNDV_R1_H_

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifdef PNDV_CFG_ENABLE_R1

#ifdef __cplusplus
extern "C"
{
#endif

void pndv_r1_check_all_deactivate_done ();
bool pndv_r1_sendPrmEndData            (uint16_t slot, uint16_t subslot, uint16_t ar_idx, Boolean more_follows);
void pndv_r1_rcve_prm_end              (Unsigned16 slot_nr, Unsigned16 subslot_nr, Unsigned8 ar_set_nr);

#ifdef __cplusplus
}
#endif


typedef enum PNDV_R1_STATE_E
{
    PNDV_R1_STATE_UNKNOWN = 0,
    PNDV_R1_STATE_INIT    = 11,
    PNDV_R1_STATE_OPEN    = 12

}   PNDV_R1_STATE_T;

////////////////////////////////////////////////////////////////
///the coupling state of the remote PNDV running on the other IM
////////////////////////////////////////////////////////////////
typedef enum pndv_r1_partner_state_e
{
    PNDV_R1_PARTNER_STATE_UNKNOWN = 0xffffffff,
    PNDV_R1_PARTNER_STATE_PRESENT = 0x00000001,
    PNDV_R1_PARTNER_STATE_ABSENT  = 0x00000002

}   pndv_r1_partner_state_t;


//////////////////////////
///selector id definitions
//////////////////////////
#define PNDV_R1_SELECTOR_ID_NOTIFY         PPPN_SELECTOR_ID_NOTIFY  /* always 0 */
#define PNDV_R1_SELECTOR_ID_PRM_END_LR     1
#define PNDV_R1_SELECTOR_ID_PRM_END_RL     2
#define PNDV_R1_SELECTOR_ID_MAX            3

/* Version of notify-data */
#define PNDV_R1_PPPN_NOTIFY_VERSION        1

////////////////////////////
///buffer length definitions
////////////////////////////

#define PNDV_R1_PPPN_DATA_SIZE_NOTIFY                 (10)

#define PNDV_R1_PPPN_DATA_SIZE_PRM_END_HEADER      (2/*arIdx*/ + PNDV_R1_DATA_STREAM_BYTE_SIZE_COMMAND_HEADER)
#define PNDV_R1_PPPN_DATA_SIZE_PRM_END_ONE_SUBSLOT (4)
#define PNDV_R1_PPPN_DATA_SIZE_PRM_END_MAX         (  PNDV_R1_PPPN_DATA_SIZE_PRM_END_HEADER  \
                                                   + (PNDV_R1_PPPN_DATA_SIZE_PRM_END_ONE_SUBSLOT * PNDV_MAX_SV_ENTITY))




//////////////////////////////////////////////////////////////////////////////////////////////////
///enum describing the needed conditions which must be present i. o. t. use the PPPN communication
//////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum PNDV_R1_COUPLING_CONDITIONS_E
{
    ///a partner must be found
    PNDV_R1_PARTNER_FOUND =      (0x00000001 << 0),

    ///the remote im has got a compatible version
    PNDV_R1_COMPATIBLE_VERSION = (0x00000001 << 1),

}   PNDV_R1_COUPLING_CONDITIONS_T;

///////////////////////////////////////////////
///mask of the needed conditions for using PPPN
///////////////////////////////////////////////
#define PNDV_R1_NEEDED_COUPLING_CONDITIONS_MASK ( (int) ( PNDV_R1_COMPATIBLE_VERSION | PNDV_R1_PARTNER_FOUND  ) )

#define PNDV_R1_ADD_NEEDED_COUPLING_CONDITION(__COND_BIT)     pndv_r1_data.couplingConditions |= (__COND_BIT)

#define PNDV_R1_REMOVE_NEEDED_COUPLING_CONDITION(__COND_BIT)  pndv_r1_data.couplingConditions &= ( ~(__COND_BIT) )

#define PNDV_R1_IS_COUPLING_ENABLE()  ((pndv_r1_data.couplingConditions & PNDV_R1_NEEDED_COUPLING_CONDITIONS_MASK) == PNDV_R1_NEEDED_COUPLING_CONDITIONS_MASK )


/////////////////////////////////////
///the supported Rx coupling commands
/////////////////////////////////////
#define PNDV_R1_COUPLING_EVENT_NO                                          ( (uint8_t) 0 )
#define PNDV_R1_COUPLING_EVENT_PLUG_PRM_END                             ( (uint8_t) 1 )
#define PNDV_R1_COUPLING_EVENT_PLUG_PRM_END_DONE                        ( (uint8_t) 2 )


///////////////////////////////////
///the header of a coupling command
///////////////////////////////////
typedef struct pndv_r1_command_header_s
{
    //////////////////////////////////////////////////////
    ///the number of data elements contained in the buffer
    //////////////////////////////////////////////////////
    uint16_t nrOfElements;

    ///////////////////////
    ///the coupling command
    ///////////////////////
    uint8_t command;

}   pndv_r1_command_header_t;

#define PNDV_R1_DATA_STREAM_BYTE_SIZE_COMMAND_HEADER             ( (uint16_t) 3 )

////////////////////////////////////////////////////////////////////////////////
///set of macros used for converting from structured format to big endian stream
////////////////////////////////////////////////////////////////////////////////
#define PNDV_R1_UINT32_TO_STREAM(__STREAM_PTR, UINT32_PTR)\
{\
    ( (uint8_t*) (__STREAM_PTR) )[0] = ( ( (uint8_t*) (UINT32_PTR) )[3] );\
    ( (uint8_t*) (__STREAM_PTR) )[1] = ( ( (uint8_t*) (UINT32_PTR) )[2] );\
    ( (uint8_t*) (__STREAM_PTR) )[2] = ( ( (uint8_t*) (UINT32_PTR) )[1] );\
    ( (uint8_t*) (__STREAM_PTR) )[3] = ( ( (uint8_t*) (UINT32_PTR) )[0] );\
}

#define PNDV_R1_UINT16_TO_STREAM(__STREAM_PTR, UINT16_PTR)\
{\
    ( (uint8_t*) (__STREAM_PTR) )[0] = ( ( (uint8_t*) (UINT16_PTR) )[1] );\
    ( (uint8_t*) (__STREAM_PTR) )[1] = ( ( (uint8_t*) (UINT16_PTR) )[0] );\
}

#define PNDV_R1_STREAM_TO_UINT32(__STREAM_PTR, UINT32_PTR) PNDV_R1_UINT32_TO_STREAM(UINT32_PTR, __STREAM_PTR)

#define PNDV_R1_STREAM_TO_UINT16(__STREAM_PTR, UINT16_PTR) PNDV_R1_UINT16_TO_STREAM(UINT16_PTR, __STREAM_PTR)

///////////////////////////////////////////////////////
///put the coupling command header into the send stream
///////////////////////////////////////////////////////
#define PNDV_R1_COMMAND_HEADER_ADD_DATA_TO_STREAM(__STREAM_PTR, COMMAND_HEADER_PTR)\
{\
    PNDV_R1_UINT16_TO_STREAM((__STREAM_PTR), &( (COMMAND_HEADER_PTR)->nrOfElements ) );\
    (__STREAM_PTR) += 2;\
    (__STREAM_PTR)[0] = (COMMAND_HEADER_PTR)->command;\
    (__STREAM_PTR) += 1;\
}

//////////////////////////////////////////////////////////////////////
///get the coupling command header from the data stream to a structure
//////////////////////////////////////////////////////////////////////
#define PNDV_R1_COMMAND_HEADER_GET_DATA_FROM_STREAM(__STREAM_PTR, COMMAND_HEADER_PTR)\
{\
    PNDV_R1_STREAM_TO_UINT16((__STREAM_PTR), ( &(COMMAND_HEADER_PTR)->nrOfElements ) );\
    (COMMAND_HEADER_PTR)->command = (__STREAM_PTR)[2];\
}

//////////////////////////////////////////////////////////////////////
///put the prm-end header into the send stream
//////////////////////////////////////////////////////////////////////
#define PNDV_R1_ADD_PRM_END_HEADER_TO_STREAM(__STREAM_PTR, COMMAND_HEADER_PTR, __AR_IDX)\
{\
    PNDV_R1_COMMAND_HEADER_ADD_DATA_TO_STREAM(__STREAM_PTR, COMMAND_HEADER_PTR);\
    PNDV_R1_UINT16_TO_STREAM((__STREAM_PTR), &(__AR_IDX));\
    (__STREAM_PTR) += 2;\
}

//////////////////////////////////////////////////////////////////////
///get the prm-end header from stream
//////////////////////////////////////////////////////////////////////
#define PNDV_R1_GET_PRM_END_HEADER_FROM_STREAM(__STREAM_PTR, COMMAND_HEADER_PTR, __AR_IDX)\
{\
    PNDV_R1_COMMAND_HEADER_GET_DATA_FROM_STREAM(__STREAM_PTR, COMMAND_HEADER_PTR);\
    (__STREAM_PTR) += PNDV_R1_DATA_STREAM_BYTE_SIZE_COMMAND_HEADER;\
    PNDV_R1_STREAM_TO_UINT16((__STREAM_PTR), &(__AR_IDX));\
    (__STREAM_PTR) += 2;\
}

/////////////////////////////////////////////////////////////////////////////////////
///macro used for putting the data of a prm-end of a plugged submodule to the send stream
/////////////////////////////////////////////////////////////////////////////////////
#define PNDV_R1_ADD_PRM_END_DATA_TO_STREAM(__STREAM_PTR, __SLOT, __SUBSLOT)\
{\
    PNDV_R1_UINT16_TO_STREAM((__STREAM_PTR), &(__SLOT));\
    (__STREAM_PTR) += 2;\
    PNDV_R1_UINT16_TO_STREAM((__STREAM_PTR), &(__SUBSLOT));\
    (__STREAM_PTR) += 2;\
}

/////////////////////////////////////////////////////////////////////////////////////
///macro used for getting the data of a prm-end of a plugged submodule from the rec stream
/////////////////////////////////////////////////////////////////////////////////////
#define PNDV_R1_GET_PRM_END_DATA_FROM_STREAM(__STREAM_PTR, __SLOT, __SUBSLOT)\
{\
    PNDV_R1_STREAM_TO_UINT16((__STREAM_PTR), &(__SLOT));\
    (__STREAM_PTR) += 2;\
    PNDV_R1_STREAM_TO_UINT16((__STREAM_PTR), &(__SUBSLOT));\
    (__STREAM_PTR) += 2;\
}

#ifdef __cplusplus

///////////////////////
///forward declarations
///////////////////////
class PNDVR1Manager;
class SWOPppnManager;

typedef struct PNDV_R1_DATA_S
{
    PNDV_R1_STATE_T        sm;
    int                    couplingConditions;
    unsigned int           activeAtBackplane;
    int                    aufdatenIsRunning;
    int                    im_position;
    int                    deactivateDonePending;

    PNDVR1Manager         *R1Manager;

    CM_UPPER_RQB_PTR_TYPE  ind_rqb_ptr;

}   PNDV_R1_DATA_T;


#define PNDV_R1_CONNECTION_EMPTY              ((Unsigned16)0x0000)      /* no send block alloc */
#define PNDV_R1_CONNECTION_READY_SEND         ((Unsigned16)0x0001)
#define PNDV_R1_CONNECTION_WAIT_SEND_DONE     ((Unsigned16)0x0002)
#define PNDV_R1_CONNECTION_WAIT_RECV          ((Unsigned16)0x0003)
#define PNDV_R1_CONNECTION_ERROR              ((Unsigned16)0xFFFF)

#define PNDV_R1_CONNECTION_NEW_DATA_REQ_PEND  ((Unsigned16)0x0002) /* partner has requested the data -> quit is nesaccary */
#define PNDV_R1_CONNECTION_NEW_DATA_PEND      ((Unsigned16)0x0004)
#define PNDV_R1_CONNECTION_NEW_REQ_PEND       ((Unsigned16)0x0008)


class PNDVR1PppnConnection
{
public:
            PNDVR1PppnConnection()
            : _pppnManager(0),
              _selector(0),
              _connState(PNDV_R1_CONNECTION_EMPTY),
              _beginnerAtThisIM(false),
              _pending(0)
            {
            };

            virtual ~PNDVR1PppnConnection() {};

            void       setPppnManger(SWOPppnManager* pppnManger)  {_pppnManager = pppnManger;};

            void       setConnState(Unsigned16 connState)  {_connState = connState;};
            Unsigned16 getConnState(void)                  {return(_connState);};

            void       setPending(Unsigned16 pending)      {_pending = pending;};
            Unsigned16 getPending(void)                    {return(_pending);};

            void       setSelector(Unsigned8 selector)     {_selector = selector;};

            bool       isThisIMBeginner()                  {return (_beginnerAtThisIM);};
            void       setThisAsBeginner()                 {_beginnerAtThisIM = true;};
            void       setOtherAsBeginner()                {_beginnerAtThisIM = false;};

            void       fillHeader(Unsigned8 *sendPtr, Unsigned8 opc, Unsigned8 kind, Unsigned16 sendLen);
protected:
            SWOPppnManager        *_pppnManager;

            Unsigned8  _selector;
            Unsigned16 _connState;
            bool       _beginnerAtThisIM;
            Unsigned16 _pending;

};


//////////////////////////////////////

typedef enum PNDVR1_PRM_END_SEND_STATE_E
{
    PNDVR1_PRM_END_SEND_STATE_NON                          = 0,
    PNDVR1_PRM_END_SEND_STATE_FILL_BUFFER_WITH_PARTNER     = 1,
    PNDVR1_PRM_END_SEND_STATE_FILL_BUFFER_WITHOUT_PARTNER  = 2,
    PNDVR1_PRM_END_SEND_STATE_WAIT_ACK_FROM_PARTNER        = 3,
    PNDVR1_PRM_END_SEND_STATE_GET_BACK_BUFFER              = 4

}   PNDVR1_PRM_END_SEND_STATE_T;

//////////////////////////////////////
///manager class needed for using PPPN
//////////////////////////////////////
class PNDVR1Manager : public SWOPppnManagerInterface
{
public:

    PNDVR1Manager();
    virtual ~PNDVR1Manager();

    virtual void        open_channel_done();
    virtual void        partner_found(Unsigned8 *data_ptr, Unsigned16 data_len);
    virtual void        partner_lost();

    virtual Unsigned16  set_version(Unsigned8 *data_ptr);    //!< user has to fill in the own component version data

    virtual void        send_confirmation(Unsigned8 selector_id);  //!< a send req is done
    virtual void        send_repetition(Unsigned8 selector_id);  //!< after send_done a pending request can be repeted

    virtual void        rcve_indication (Unsigned8 selector_id, Unsigned16 rcve_length, Unsigned8 *rcve_data);  //!< a receive from partner is occure

    bool     sendData         (uint8_t selector_id, uint16_t length, Boolean isAnswerOfRcveIndication);
    uint8_t* getSendBuffer    (uint8_t selector_id);
    bool     sendPrmEndData   (uint16_t slot, uint16_t subslot, uint16_t ar_idx, Boolean more_follows);
    void     rcvePrmEnd       (Unsigned8 selector_id, Unsigned16 rcve_length, Unsigned8 *rcve_data);
    void     rcvePrmEndData   ();
    void     sendPrmEndRsp    (Unsigned8 selector_id);

    void     openPPPNchannels();

    void     connection_state_init();

    PNDVR1PppnConnection* getConnection(UnsignedOpt selector)  {return(&_connection[selector]);};

    void setPPPNdata(uint8_t pppnHandle, uint8_t pppnSendPrio);
    SWOPppnManager* getPppnManager() { return _pppnManager; };

protected:

    bool checkVersion(uint8_t *data_ptr, uint16_t data_len);

    ////////////////////////////////////////////////////////
    ///the data length values of the needed PPPN connections
    ////////////////////////////////////////////////////////
    uint16_t               _selectorLength[PNDV_R1_SELECTOR_ID_MAX];
    PNDVR1PppnConnection   _connection[PNDV_R1_SELECTOR_ID_MAX];

    /////////////////////////////
    ///data needed for using PPPN
    /////////////////////////////
    uint8_t _pppnHandle;
    uint8_t _pppnSendPrio;

    ///////////////////////////////////////
    ///the state of the remote running PNDV
    ///////////////////////////////////////
    pndv_r1_partner_state_t _partnerState;

    LSA_VERSION_TYPE       _version_partner;

    /* prm end stuff */
    pndv_r1_command_header_t         _prmEndSendHeader;
    uint8_t                         *_prmEndSendBuffer;
    uint16_t                         _prmEndSendLength;
    PNDVR1_PRM_END_SEND_STATE_T      _prmEndSendState;



    //////////////////////////////////////////////////////
    ///manager class used for using communication via PPPN
    //////////////////////////////////////////////////////
    SWOPppnManager *_pppnManager;
};

#endif /*__cplusplus*/

#endif /*PNDV_CFG_ENABLE_R1*/

#endif /* PNDV_R1_H_ */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
