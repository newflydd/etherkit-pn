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
/*  F i l e               &F: eddp_i2c.c                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP I2C-API                                     */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  26.04.10    SFR   initial version.                                       */
/*  06.12.10    JS    changed EDD_CFG_MAX_PORT_CNT ot EDDP_MAX_PORT_CNT      */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID  131
#define EDDP_MODULE_ID     LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_I2C */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"           /* eddp headerfiles */
#include "eddp_int.h"           /* internal header  */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)
#ifdef LSAS_CFG_USE_POF
/*===========================================================================*/
/*                                 defines                                   */
/*===========================================================================*/

//#define EDDP_I2C_TRACE		//enables additional EDDP I2C trace (error traces are always enabled)

// ***** map traces into the CORE subsystem
#define EDDP_I2C_TRACE_00   EDDP_CORE_TRACE_00
#define EDDP_I2C_TRACE_01   EDDP_CORE_TRACE_01
#define EDDP_I2C_TRACE_02   EDDP_CORE_TRACE_02
#define EDDP_I2C_TRACE_03   EDDP_CORE_TRACE_03
#define EDDP_I2C_TRACE_04   EDDP_CORE_TRACE_04
#define EDDP_I2C_TRACE_05   EDDP_CORE_TRACE_05
#define EDDP_I2C_TRACE_06   EDDP_CORE_TRACE_06
#define EDDP_I2C_TRACE_07   EDDP_CORE_TRACE_07
#define EDDP_I2C_TRACE_08   EDDP_CORE_TRACE_08
#define EDDP_I2C_TRACE_09   EDDP_CORE_TRACE_09

// ***** wait 2us
#define EDDP_I2C_WAIT_2US(_pDDB)   EDDP_HALWaitNsTicks(_pDDB, 2*EDDP_HAL_1US_TO)

// ***** states returned by HW into the status register
#define EDDP_I2C_STATE_START_0       0x08     // START condition transmitted
#define EDDP_I2C_STATE_START_N       0x10     // Repeated START condition transmitted
#define EDDP_I2C_STATE_ADR_WR        0x18     // Address + write bit transmitted, ACK received
#define EDDP_I2C_STATE_WR_DATA_ACK   0x28     // Data byte transmitted in master mode, ACK received
#define EDDP_I2C_STATE_WR_DATA_NAK   0x30     // Data byte transmitted in master mode, ACK not received
#define EDDP_I2C_STATE_ADR_RD        0x40     // Address + read bit transmitted, ACK received
#define EDDP_I2C_STATE_RD_DATA_ACK   0x50     // Data byte received in master mode, ACK transmitted
#define EDDP_I2C_STATE_RD_DATA_NAK   0x58     // Data byte received in master mode, NAK transmitted
#define EDDP_I2C_STATE_IDLE          0xF8     // No relevant status information, IFLG=0

// ***** commands to the HW statemachine of the MI2C-IP
#define EDDP_I2C_CMD_IDLE        (PNIP_REG_MI2C_CNTR__MSK_IEN | PNIP_REG_MI2C_CNTR__MSK_ENAB)
#define EDDP_I2C_CMD_START       (EDDP_I2C_CMD_IDLE | PNIP_REG_MI2C_CNTR__MSK_STA)
#define EDDP_I2C_CMD_WR_DATA     (EDDP_I2C_CMD_IDLE | 0)
#define EDDP_I2C_CMD_RD_DATA_ACK (EDDP_I2C_CMD_IDLE | PNIP_REG_MI2C_CNTR__MSK_AAK)
#define EDDP_I2C_CMD_RD_DATA_NAK (EDDP_I2C_CMD_IDLE | 0)
#define EDDP_I2C_CMD_STOP        (EDDP_I2C_CMD_IDLE | PNIP_REG_MI2C_CNTR__MSK_STP)

#define EDDP_I2C_BUS_HIGHZ (PNIP_REG_I2C_CONTROL__MSK_SDA_I | PNIP_REG_I2C_CONTROL__MSK_SCL_I | PNIP_REG_I2C_CONTROL__MSK_SW_SDA_O | PNIP_REG_I2C_CONTROL__MSK_SW_SCL_O)

/*===========================================================================*/
/*                                  macros                                   */
/*===========================================================================*/

// ***** check macros after I2C command
#define EDDP_I2C_WAIT_IFLG(pDDB, ToCnt)             {ToCnt = 0x1000; while(ToCnt) { if (EDDP_HAL_REG32_READ(pDDB, PNIP_REG_MI2C_CNTR) & PNIP_REG_MI2C_CNTR__MSK_IFLG) break; ToCnt--; }}
#define EDDP_I2C_WAIT_STATUS(pDDB, ToCnt, Stat)     {ToCnt = 0x1000; while(ToCnt) { if (EDDP_HAL_REG32_READ(pDDB, PNIP_REG_MI2C_STAT) == Stat) break; ToCnt--; }}

/*===========================================================================*/
/*                                global Variables                           */
/*===========================================================================*/

static  LSA_BOOL  m_I2CSetupExecuted = LSA_FALSE;


/*===========================================================================*/
/*                            local function declaration                     */
/*===========================================================================*/

static  EDD_RSP  EDD_UPPER_IN_FCT_ATTR  eddp_I2C_Read_Offset_internal(
                                                        EDDP_LOCAL_DDB_PTR_TYPE         const  pDDB,
                                                        LSA_UINT8			            const  I2CDevAddr,
                                                        LSA_UINT8			            const  I2COffsetCnt,
                                                        LSA_UINT8			            const  I2COffset1,
                                                        LSA_UINT8			            const  I2COffset2,
                                                        LSA_UINT32                      const  Size,
                                                        LSA_UINT8  EDD_UPPER_MEM_ATTR       *  pBuf );

static  EDD_RSP  EDD_UPPER_IN_FCT_ATTR  eddp_I2C_Write_Offset_internal(
                                                        EDDP_LOCAL_DDB_PTR_TYPE         const  pDDB,
                                                        LSA_UINT8			            const  I2CDevAddr,
                                                        LSA_UINT8			            const  I2COffsetCnt,
                                                        LSA_UINT8			            const  I2COffset1,
                                                        LSA_UINT8			            const  I2COffset2,
                                                        LSA_UINT32                      const  Size,
                                                        LSA_UINT8  EDD_UPPER_MEM_ATTR       *  pBuf );


/*===========================================================================*/
/*                           internal helper functions                       */
/*===========================================================================*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_I2CFixupBus                            +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB           +*/
/*+                                                                         +*/
/*+  Result                :    LSA_BOOL                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+  Results    : LSA_TRUE                                                  +*/
/*+               LSA_FALSE                                                 +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: Toggle the SCL line until SDA read HighZ.                 +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static LSA_BOOL EDDP_I2CFixupBus( EDDP_LOCAL_DDB_PTR_TYPE const pDDB )
{
    // Note: Toggle the SCL line until SDA read HighZ
    LSA_UINT32  Reg, Bus, Timeout;

    Timeout = 100;
    Bus     = (EDDP_HAL_REG32_READ(pDDB, PNIP_REG_I2C_CONTROL) & EDDP_I2C_BUS_HIGHZ);
    Reg     = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_EX_CONTROL_1);

    while (Timeout && (Bus != EDDP_I2C_BUS_HIGHZ))
    {
        // ***** Toggle SCL if HW state machine hangs until bus is HighZ
        Reg ^= PNIP_REG_EX_CONTROL_1__MSK_SCL_TOGGLE;
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_EX_CONTROL_1, Reg);
        EDDP_I2C_WAIT_2US(pDDB);
        Bus = (EDDP_HAL_REG32_READ(pDDB, PNIP_REG_I2C_CONTROL) & EDDP_I2C_BUS_HIGHZ);
        if (Reg & PNIP_REG_EX_CONTROL_1__MSK_SCL_TOGGLE)
        {   // toggle until Bus == FF and Reg without PNIP_REG_EX_CONTROL_1__MSK_SCL_TOGGLE bit !!!
            Bus = EDDP_I2C_BUS_HIGHZ-1;
        }
        Timeout--;
    }

    if (Bus == EDDP_I2C_BUS_HIGHZ) //&& !(Reg & PNIP_REG_EX_CONTROL_1__MSK_SCL_TOGGLE))
    {
        return LSA_TRUE;
    }
    return LSA_FALSE;
}
/*---------------------- end [subroutine] ---------------------------------*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_I2CSelectHwPort						+*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB           +*/
/*+                        :    LSA_UINT32                   HwPortID       +*/
/*+                                                                         +*/
/*+  Result                :    LSA_BOOL                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+  HwPortID   : selects the Port as HW index                         (in) +*/
/*+                                                                         +*/
/*+  Results    : LSA_TRUE                                                  +*/
/*+               LSA_FALSE                                                 +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: Switch multiplexer to the given port.                     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static  LSA_BOOL  EDDP_I2CSelectHwPort( EDDP_LOCAL_DDB_PTR_TYPE  const  pDDB,
                                        LSA_UINT32				 const  HwPortID )
{
    if ((HwPortID >= 1) && (HwPortID <= EDDP_MAX_PORT_CNT))
    {
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_I2C_SELECT, (HwPortID-1)<<PNIP_REG_I2C_SELECT_SHFT_I2C_SELECTOR);
        return LSA_TRUE;
    }
    return LSA_FALSE;
}
/*---------------------- end [subroutine] ---------------------------------*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_I2C_DoMI2C_Reset                       +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB           +*/
/*+                                                                         +*/
/*+  Result                :    LSA_BOOL                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+  Results    : LSA_TRUE                                                  +*/
/*+               LSA_FALSE                                                 +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: Resets the MI2C module within the PNIP.                   +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static  LSA_BOOL  EDDP_I2C_DoMI2C_Reset( EDDP_LOCAL_DDB_PTR_TYPE  const  pDDB )
{
    LSA_UINT16  Timeout = 500;

    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_XADDR, 0);
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_SOFTWARE_RESET, PNIP_REG_MI2C_SOFTWARE_RESET__MSK_SOFTRESET);
    EDDP_I2C_WAIT_2US(pDDB);

    while (Timeout)
    {
        if (EDDP_HAL_REG32_READ(pDDB, PNIP_REG_MI2C_XADDR))
        {
            return LSA_TRUE;
        }
        Timeout--;
    }
    return LSA_FALSE;
}
/*---------------------- end [subroutine] ---------------------------------*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_I2C_Init                               +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB           +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_HW                                            +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: Initialization of most common I2C registers.              +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_I2C_Init( EDDP_LOCAL_DDB_PTR_TYPE const pDDB )
{
    LSA_UINT32  Val;

    EDDP_I2C_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "EDDP_I2C_Init()");

    EDDP_I2CSelectHwPort(pDDB, 1); //select Port_0

    // ***** enabled with 0 !!!!
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_I2C_ENABLE, 0<<PNIP_REG_I2C_ENABLE_SHFT_I2C_PARAMENABLE);
    // ***** set lines to tristate
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_I2C_CONTROL, PNIP_REG_I2C_CONTROL__MSK_SW_SCL_O | PNIP_REG_I2C_CONTROL__MSK_SW_SDA_O);
    // ***** set 100kBit/s
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_I2C_CLOCKCONTROL, PNIP_REG_I2C_CLOCKCONTROL_RST__VAL); 
    // ***** Mode=per action, Disable Wr/Rd
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_EX_CONTROL_1, 0); 
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_EX_CONTROL_2, 0); 
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_EX_CONTROL_3, 0); 
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_EX_CONTROL_4, 0);

    Val = (EDDP_HAL_REG32_READ(pDDB, PNIP_REG_I2C_CONTROL) & EDDP_I2C_BUS_HIGHZ);

    if (Val != EDDP_I2C_BUS_HIGHZ)
    {
        if (!EDDP_I2CFixupBus(pDDB))
        {
            EDDP_I2C_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "EDDP_I2C_Init(): ERROR -> HW error: missing high level on SDA --> check pullups on I2C bus !!!");
            return EDD_STS_ERR_HW;
        }
    }

    // ***** reset mi2c module
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_SOFTWARE_RESET, PNIP_REG_MI2C_SOFTWARE_RESET__MSK_SOFTRESET);
    EDDP_I2C_WAIT_2US(pDDB);

    m_I2CSetupExecuted = LSA_TRUE;

    return EDD_STS_OK;
}
/*---------------------- end [subroutine] ---------------------------------*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_I2C_DeInit                             +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB           +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                    (in) +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: DeInitialization of most common I2C registers.            +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_I2C_DeInit( EDDP_LOCAL_DDB_PTR_TYPE const pDDB )
{
    EDDP_I2C_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "EDDP_I2C_DeInit()");

    // ***** disabled with 1 !!!!
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_I2C_ENABLE, 1<<PNIP_REG_I2C_ENABLE_SHFT_I2C_PARAMENABLE);
    // ***** Mode=per action, Disable Wr/Rd
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_EX_CONTROL_1, 0); 
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_EX_CONTROL_2, 0); 
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_EX_CONTROL_3, 0); 
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_EX_CONTROL_4, 0);
    EDDP_I2CSelectHwPort(pDDB, 1);
    m_I2CSetupExecuted = LSA_FALSE;
    return EDD_STS_OK;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*=============================================================================
 * function name: eddp_I2C_Select()
 *
 * function:      Selects an I2C port for the following I2C transfers
 *
 * parameters:    ...
 *
 * return value:  LSA_BOOL
 *
 *===========================================================================*/
static  LSA_BOOL  EDDP_LOCAL_FCT_ATTR  eddp_I2C_Select( EDDP_LOCAL_DDB_PTR_TYPE  const  pDDB,
                                                        LSA_UINT16               const  PortId,
                                                        LSA_UINT16          	 const  I2CMuxSelect )
{
    LSA_UINT16  const  I2CPortIdStored    = pDDB->I2C.I2CPortIdStored;
    LSA_UINT16  const  I2CMuxSelectStored = pDDB->I2C.I2CMuxSelectStored;

    #if defined (EDDP_I2C_TRACE)
    EDDP_I2C_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "eddp_I2C_Select(pDDB:0x%X PortId:0x%X I2CMuxSelect:0x%X)",
                      pDDB, PortId, I2CMuxSelect);
    #endif

    //PortId is already checked before

    if //PortId of I2C transfer changed?
       (PortId != I2CPortIdStored)
    {
        LSA_UINT32  const  HwPortId = pDDB->SWI.PortMap.PortID_x_to_HardwarePort_y[PortId - 1];

        pDDB->I2C.I2CPortIdStored = PortId;

        //switch PNIP-internal I2C multiplexer to desired port
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_I2C_SELECT, (HwPortId-1UL)<<PNIP_REG_I2C_SELECT_SHFT_I2C_SELECTOR);
    }

    if //I2CMuxSelect of I2C transfer activated and changed?
       (   (I2CMuxSelect)
        && (I2CMuxSelect != I2CMuxSelectStored))
    {
        LSA_UINT8  bRetVal; //preset

        pDDB->I2C.I2CMuxSelectStored = I2CMuxSelect;

        //switch external I2C multiplexer to desired port
        EDDP_I2C_SELECT(&bRetVal, pDDB->hSysDev, PortId, I2CMuxSelect);

        return (EDD_I2C_DEVICE_ACTIVATED == bRetVal)?LSA_TRUE:LSA_FALSE;
    }
    else
    {
        return LSA_TRUE;
    }
}
/*---------------------- end [subroutine] ---------------------------------*/


/*=============================================================================
 * function name: eddp_I2C_Read_Offset()
 *
 * function:      API-function: reads n bytes from the I2C device of the selected port
 *
 * parameters:    ...
 *
 * return value:  EDD_RSP
 *
 *===========================================================================*/
EDD_RSP  EDD_UPPER_IN_FCT_ATTR  eddp_I2C_Read_Offset( EDDP_HANDLE  		   	         const  hDDB,          //DDB handle (refer to service EDDP_SRV_DEV_OPEN)
                                                      LSA_UINT32	                 const  PortId,        //User-PortId 1...4
                                                      LSA_UINT16		             const  I2CMuxSelect,  //can be used for HW-multiplexer-control
                                                      LSA_UINT8			             const  I2CDevAddr,    //0...127
                                                      LSA_UINT8			             const  I2COffsetCnt,  //0...2
                                                      LSA_UINT8			             const  I2COffset1,    //optional: 0...255
                                                      LSA_UINT8			             const  I2COffset2,    //optional: 0...255
                                                      LSA_UINT16                     const  RetryCnt,      //0: disabled,    1..65535
                                                      LSA_UINT16                     const  RetryTime_us,  //0: no waiting,  1..65535
                                                      LSA_UINT32                     const  Size,      	   //in bytes, 1...300
                                                      LSA_UINT8  EDD_UPPER_MEM_ATTR      *  pBuf )    	   //pointer to buffer
{
    EDD_RSP                         EDDResponse = EDD_STS_OK;
    LSA_BOOL                        bRetVal;
    EDDP_LOCAL_DDB_PTR_TYPE  const  pDDB        = (EDDP_LOCAL_DDB_PTR_TYPE)hDDB;
    LSA_UINT16                      RetryCount  = RetryCnt;

    #if defined (EDDP_I2C_TRACE)
    EDDP_I2C_TRACE_09(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "eddp_I2C_Read_Offset(pDDB:0x%X PortId:%d I2CMuxSelect:0x%X I2CDevAddr:%2Xh I2COffsetCnt:0x%X I2COffset1:0x%X I2COffset2:0x%X Size:%d pBuf:0x%X)",
                      pDDB, PortId, I2CMuxSelect, I2CDevAddr, I2COffsetCnt, I2COffset1, I2COffset2, Size, pBuf);
    #endif

    //plausible parameters
    if (   (PortId       == 0)
        || (PortId       >  pDDB->SWI.PortCnt)
        || (I2COffsetCnt >  (LSA_UINT8)2)
        || (Size         >  EDD_I2C_MAX_DATA_SIZE))
    {
        EDDP_I2C_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "eddp_I2C_Read_Offset(PortId:0x%X PortCnt:0x%X I2COffsetCnt:0x%X Size:0x%X MAX_DATA_SIZE:0x%X): ERROR -> invalid parameters",
                          PortId, pDDB->SWI.PortCnt, I2COffsetCnt, Size, EDD_I2C_MAX_DATA_SIZE);
        return EDD_STS_ERR_PARAM;
    }

    if (!m_I2CSetupExecuted)
    {
        if //error occured in EDDP_I2C_Init()?
           (pDDB->I2C.I2CInitResult != EDD_STS_OK)
        {
            EDDP_I2C_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "eddp_I2C_Read_Offset(I2CInitResult:0x%X): ERROR -> error occured in EDDP_I2C_Init()", pDDB->I2C.I2CInitResult);
            return pDDB->I2C.I2CInitResult;
        }
        else
        {
            EDDP_I2C_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "eddp_I2C_Read_Offset(): ERROR -> expect EDDP_I2C_Init() before");
            return EDD_STS_ERR_SEQUENCE;
        }
    }

    if (Size == 0)
    {
        return EDD_STS_OK;
    }

    EDDP_ENTER_I2C_S();
    bRetVal = eddp_I2C_Select(pDDB, (LSA_UINT16)PortId, I2CMuxSelect);

    if (!bRetVal)
    {
        EDDP_EXIT_I2C_S();
        EDDP_I2C_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "eddp_I2C_Read_Offset(PortId:0x%X I2CMuxSelect:0x%X bRetVal:0x%X): FATAL -> eddp_I2C_Select() wrong",
                          PortId, I2CMuxSelect, bRetVal);
        EDDP_FATAL(EDDP_FATAL_ERR_SERVICE);
        return EDD_STS_ERR_EXCP;
    }

    for (;;)
    {
        EDDResponse = eddp_I2C_Read_Offset_internal(pDDB, I2CDevAddr, I2COffsetCnt, I2COffset1, I2COffset2, Size, pBuf); //internal EDDP function
        if (    (  (EDD_STS_ERR_PROTOCOL == EDDResponse) || (EDD_STS_ERR_HW == EDDResponse)  )
            &&  (RetryCount > 0) )
        {
            RetryCount--;
            if (RetryTime_us > 0)
                EDDP_WAIT_10_NS(pDDB->hSysDev, RetryTime_us * 100UL);
        }
        else
            break;
    }

    EDDP_EXIT_I2C_S();

    return EDDResponse;
}
/*---------------------- end [subroutine] ---------------------------------*/

/*=============================================================================
 * function name: eddp_I2C_Read_Offset_internal()
 *
 * function:      internal function: reads n bytes from the I2C device of the selected port
 *
 * parameters:    ...
 *
 * return value:  EDD_RSP
 *
 *===========================================================================*/
static  EDD_RSP  EDD_UPPER_IN_FCT_ATTR  eddp_I2C_Read_Offset_internal(
                                                        EDDP_LOCAL_DDB_PTR_TYPE         const  pDDB,	       //pDDB
                                                        LSA_UINT8			            const  I2CDevAddr,     //0...127
                                                        LSA_UINT8			            const  I2COffsetCnt,   //0...2
                                                        LSA_UINT8			            const  I2COffset1,     //optional: 0...255
                                                        LSA_UINT8			            const  I2COffset2,     //optional: 0...255
                                                        LSA_UINT32                      const  Size,      	   //in bytes, 1...300
                                                        LSA_UINT8  EDD_UPPER_MEM_ATTR       *  pBuf )    	   //pointer to buffer
{
    LSA_UINT32      Cmd;
    LSA_UINT32      State;
    LSA_UINT32      ToCnt;          //Timeout count
    LSA_UINT32      Len         = Size;
    LSA_UINT32      BufIdx      = 0;
    
    //PageRead: START, I2CDevAddr, ByteAddr, Rd, Rd, ... , [STOP]

    // ***** send START:  IFLG bit will be set, Code=08h
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, EDDP_I2C_CMD_START);             // Start 
    EDDP_I2C_WAIT_IFLG(pDDB, ToCnt);
    if (ToCnt) //Timeout not yet elapsed?
    {
        EDDP_I2C_WAIT_STATUS(pDDB, ToCnt, EDDP_I2C_STATE_START_0);                  // START condition transmitted
    }

    if (!ToCnt) //Timeout elapsed?
    {
        // ***** try to restart the HW state machine
        EDDP_I2C_DoMI2C_Reset(pDDB);
        EDDP_I2C_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "eddp_I2C_Read_Offset_internal(): ERROR -> START condition");
        return EDD_STS_ERR_HW;
    }
    EDDP_I2C_WAIT_2US(pDDB);

    // ***** send DEV-Address + Ack:  IFLG bit will be set, Code=18h
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_DATA, (I2CDevAddr & (LSA_UINT8)0xFE));
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, EDDP_I2C_CMD_WR_DATA);           // DEV
    EDDP_I2C_WAIT_IFLG(pDDB, ToCnt);

    if (ToCnt) //Timeout not yet elapsed?
    {
        EDDP_I2C_WAIT_STATUS(pDDB, ToCnt, EDDP_I2C_STATE_ADR_WR);                   // Address + write bit transmitted, ACK received
    }

    if (!ToCnt) //Timeout elapsed?
    {
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, EDDP_I2C_CMD_STOP);          // Stop
        EDDP_I2C_WAIT_2US(pDDB);
        EDDP_I2C_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "eddp_I2C_Read_Offset_internal(): ERROR -> I2CDevAddr");
        return EDD_STS_ERR_PROTOCOL;
    }
    EDDP_I2C_WAIT_2US(pDDB);

    switch //by I2COffsetCnt (already checked by value)
            (I2COffsetCnt)
    {
        case 1:
        {
            // ***** send E2-Address + Ack:  IFLG bit will be set, Code=28h
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_DATA, I2COffset1);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, EDDP_I2C_CMD_WR_DATA);   // E2
            EDDP_I2C_WAIT_IFLG(pDDB, ToCnt);

            if (ToCnt) //Timeout not yet elapsed?
            {
                EDDP_I2C_WAIT_STATUS(pDDB, ToCnt, EDDP_I2C_STATE_WR_DATA_ACK);      // Data byte transmitted in master mode, ACK received
            }

            if (!ToCnt) //Timeout elapsed?
            {
                EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, EDDP_I2C_CMD_STOP);  // Stop
                EDDP_I2C_WAIT_2US(pDDB);
                EDDP_I2C_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "eddp_I2C_Read_Offset(): ERROR -> I2COffset1");
                return EDD_STS_ERR_PROTOCOL;
            }
            EDDP_I2C_WAIT_2US(pDDB);

            break;
        }
        case 2:
        {
            // ***** send E2-Address + Ack:  IFLG bit will be set, Code=28h
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_DATA, I2COffset1);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, EDDP_I2C_CMD_WR_DATA);   // E2
            EDDP_I2C_WAIT_IFLG(pDDB, ToCnt);

            if (ToCnt) //Timeout not yet elapsed?
            {
                EDDP_I2C_WAIT_STATUS(pDDB, ToCnt, EDDP_I2C_STATE_WR_DATA_ACK);      // Data byte transmitted in master mode, ACK received
            }

            if (!ToCnt) //Timeout elapsed?
            {
                EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, EDDP_I2C_CMD_STOP);  // Stop
                EDDP_I2C_WAIT_2US(pDDB);
                EDDP_I2C_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "eddp_I2C_Read_Offset_internal(): ERROR -> I2COffset1");
                return EDD_STS_ERR_PROTOCOL;
            }
            EDDP_I2C_WAIT_2US(pDDB);

            // ***** send E2-Address + Ack:  IFLG bit will be set, Code=28h
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_DATA, I2COffset2);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, EDDP_I2C_CMD_WR_DATA);   // E2
            EDDP_I2C_WAIT_IFLG(pDDB, ToCnt);

            if (ToCnt) //Timeout not yet elapsed?
            {
                EDDP_I2C_WAIT_STATUS(pDDB, ToCnt, EDDP_I2C_STATE_WR_DATA_ACK);      // Data byte transmitted in master mode, ACK received
            }

            if (!ToCnt) //Timeout elapsed?
            {
                EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, EDDP_I2C_CMD_STOP);  // Stop
                EDDP_I2C_WAIT_2US(pDDB);
                EDDP_I2C_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "eddp_I2C_Read_Offset_internal(): ERROR -> I2COffset2");
                return EDD_STS_ERR_PROTOCOL;
            }
            EDDP_I2C_WAIT_2US(pDDB);

            break;
        }
        default: break;
    }

    // ***** send next START:  IFLG bit will be set, Code=10h
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, EDDP_I2C_CMD_START);             // Start 
    EDDP_I2C_WAIT_IFLG(pDDB, ToCnt);

    if (ToCnt) //Timeout not yet elapsed?
    {
        EDDP_I2C_WAIT_STATUS(pDDB, ToCnt, EDDP_I2C_STATE_START_N);                  // Repeated START condition transmitted
    }

    if (!ToCnt) //Timeout elapsed?
    {
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_DATA, 0xFF);
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, EDDP_I2C_CMD_STOP);          // Stop
        EDDP_I2C_WAIT_2US(pDDB);
        EDDP_I2C_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "eddp_I2C_Read_Offset_internal(): ERROR -> START condition");
        return EDD_STS_ERR_PROTOCOL;
    }
    EDDP_I2C_WAIT_2US(pDDB);

    // ***** send DEV-Address + Ack:  IFLG bit will be set, Code=40h
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_DATA, I2CDevAddr | 1);
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, EDDP_I2C_STATE_ADR_RD);          // DEV
    EDDP_I2C_WAIT_IFLG(pDDB, ToCnt);

    if (ToCnt) //Timeout not yet elapsed?
    {
        EDDP_I2C_WAIT_STATUS(pDDB, ToCnt, EDDP_I2C_STATE_ADR_RD);                   // Address + read bit transmitted, ACK received
    }

    if (!ToCnt) //Timeout elapsed?
    {
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, EDDP_I2C_CMD_STOP);          // Stop
        EDDP_I2C_WAIT_2US(pDDB);
        EDDP_I2C_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "eddp_I2C_Read_Offset_internal(): ERROR -> I2CDevAddr");
        return EDD_STS_ERR_PROTOCOL;
    }
    EDDP_I2C_WAIT_2US(pDDB);

    while (Len)
    {
        if (Len == 1UL)
        {   // ***** last byte
            Cmd   = EDDP_I2C_CMD_RD_DATA_NAK;       // send Dummy + Send Nak: IFLG bit will be set, Code=50h
            State = EDDP_I2C_STATE_RD_DATA_NAK;     // Data byte received in master mode, NAK transmitted
        }
        else
        {   // ***** pending byte exists
            Cmd   = EDDP_I2C_CMD_RD_DATA_ACK;       // send Dummy + Send Ack: IFLG bit will be set, Code=50h
            State = EDDP_I2C_STATE_RD_DATA_ACK;     // Data byte received in master mode, ACK transmitted
        }

        // send Dummy + Send Nak/Ack:  IFLG bit will be set, Code=50h/58h
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_DATA, 0xFF);
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, Cmd);                                // D[?]
        EDDP_I2C_WAIT_IFLG(pDDB, ToCnt);
        pBuf[BufIdx] = (LSA_UINT8)EDDP_HAL_REG32_READ(pDDB, PNIP_REG_MI2C_DATA);

        if (ToCnt) //Timeout not yet elapsed?
        {
            EDDP_I2C_WAIT_STATUS(pDDB, ToCnt, State);                                       // Data byte received in master mode, NAK/ACK transmitted
        }

        if (!ToCnt) //Timeout elapsed?
        {
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, EDDP_I2C_CMD_STOP);              // Stop
            EDDP_I2C_WAIT_2US(pDDB);
            EDDP_I2C_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "eddp_I2C_Read_Offset_internal(): ERROR -> Ack/Nack");
            return EDD_STS_ERR_PROTOCOL;
        }
        Len--;
        BufIdx++;

    } //end of while loop

    // ***** send Stop:  IFLG bit will not be set, Code=F8h
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, EDDP_I2C_CMD_STOP);                      // Stop
    EDDP_I2C_WAIT_2US(pDDB);
    EDDP_I2C_WAIT_STATUS(pDDB, ToCnt, EDDP_I2C_STATE_IDLE);                                 // No relevant status information, IFLG=0

    if (!ToCnt) //Timeout elapsed?
    {
        EDDP_I2C_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "eddp_I2C_Read_Offset_internal(): ERROR -> STOP condition");
        return EDD_STS_ERR_PROTOCOL;
    }

    return EDD_STS_OK;
}
/*---------------------- end [subroutine] ---------------------------------*/

/*=============================================================================
 * function name: eddp_I2C_Write_Offset()
 *
 * function:      API-function: writes n bytes to the I2C device of the selected port
 *
 * parameters:    ...
 *
 * return value:  EDD_RSP
 *
 *===========================================================================*/
EDD_RSP  EDD_UPPER_IN_FCT_ATTR  eddp_I2C_Write_Offset( EDDP_HANDLE  		   	      const  hDDB,          //DDB handle (refer to service EDDP_SRV_DEV_OPEN)
                                                       LSA_UINT32	                  const  PortId,        //User-PortId 1...4
                                                       LSA_UINT16		              const  I2CMuxSelect,  //can be used for HW-multiplexer-control
                                                       LSA_UINT8			          const  I2CDevAddr,    //0...127
                                                       LSA_UINT8			          const  I2COffsetCnt,  //0...2
                                                       LSA_UINT8			          const  I2COffset1,    //optional: 0...255
                                                       LSA_UINT8			          const  I2COffset2,    //optional: 0...255
                                                       LSA_UINT16                     const  RetryCnt,      //0: disabled,    1..65535
                                                       LSA_UINT16                     const  RetryTime_us,  //0: no waiting,  1..65535
                                                       LSA_UINT32                     const  Size,      	//in bytes, 1...300
                                                       LSA_UINT8  EDD_UPPER_MEM_ATTR      *  pBuf )    	    //pointer to buffer
{
    LSA_BOOL                        bRetVal;
    EDDP_LOCAL_DDB_PTR_TYPE  const  pDDB        = (EDDP_LOCAL_DDB_PTR_TYPE)hDDB;
    LSA_UINT16                      RetryCount  = RetryCnt;
    EDD_RSP                         EDDResponse = EDD_STS_OK;

    #if defined (EDDP_I2C_TRACE)
    EDDP_I2C_TRACE_09(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "eddp_I2C_Write_Offset(pDDB:0x%X PortId:%d I2CMuxSelect:0x%X I2CDevAddr:%2Xh I2COffsetCnt:0x%X I2COffset1:0x%X I2COffset2:0x%X Size:%d pBuf:0x%X)",
                      pDDB, PortId, I2CMuxSelect, I2CDevAddr, I2COffsetCnt, I2COffset1, I2COffset2, Size, pBuf);
    #endif

    //plausible parameters
    if (   (PortId       == 0)
        || (PortId       >  pDDB->SWI.PortCnt)
        || (I2COffsetCnt >  (LSA_UINT8)2)
        || (Size         >  EDD_I2C_MAX_DATA_SIZE))
    {
        EDDP_I2C_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "eddp_I2C_Write_Offset(PortId:0x%X PortCnt:0x%X I2COffsetCnt:0x%X Size:0x%X MAX_DATA_SIZE:0x%X): ERROR -> invalid parameters",
                          PortId, pDDB->SWI.PortCnt, I2COffsetCnt, Size, EDD_I2C_MAX_DATA_SIZE);
        return EDD_STS_ERR_PARAM;
    }

    if (!m_I2CSetupExecuted)
    {
        if //error occured in EDDP_I2C_Init()?
           (pDDB->I2C.I2CInitResult != EDD_STS_OK)
        {
            EDDP_I2C_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "eddp_I2C_Write_Offset(I2CInitResult:0x%X): ERROR -> error occured in EDDP_I2C_Init()", pDDB->I2C.I2CInitResult);
            return pDDB->I2C.I2CInitResult;
        }
        else
        {
            EDDP_I2C_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "eddp_I2C_Write_Offset(): ERROR -> expect EDDP_I2C_Init() before");
            return EDD_STS_ERR_SEQUENCE;
        }
    }

    if (Size == 0)
    {
        return EDD_STS_OK;
    }

    EDDP_ENTER_I2C_S();
    bRetVal = eddp_I2C_Select(pDDB, (LSA_UINT16)PortId, I2CMuxSelect);
    if (!bRetVal)
    {
        EDDP_EXIT_I2C_S();
        EDDP_I2C_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "eddp_I2C_Write_Offset(PortId:0x%X I2CMuxSelect:0x%X bRetVal:0x%X): FATAL -> eddp_I2C_Select() wrong",
                          PortId, I2CMuxSelect, bRetVal);
        EDDP_FATAL(EDDP_FATAL_ERR_SERVICE);
        return EDD_STS_ERR_EXCP;
    }

    for (;;)
    {
        EDDResponse = eddp_I2C_Write_Offset_internal(pDDB, I2CDevAddr, I2COffsetCnt, I2COffset1, I2COffset2, Size, pBuf); //internal EDDP function
        if (    (  (EDD_STS_ERR_PROTOCOL == EDDResponse) || (EDD_STS_ERR_HW == EDDResponse)  )
            &&  (RetryCount > 0))
        {
            RetryCount--;
            if (RetryTime_us > 0)
                EDDP_WAIT_10_NS(pDDB->hSysDev, RetryTime_us * 100UL);
        }
        else
            break;
    }

    EDDP_EXIT_I2C_S();
    return EDDResponse;
}
/*---------------------- end [subroutine] ---------------------------------*/

static  EDD_RSP  EDD_UPPER_IN_FCT_ATTR  eddp_I2C_Write_Offset_internal(
                                                        EDDP_LOCAL_DDB_PTR_TYPE         const  pDDB,
                                                        LSA_UINT8			            const  I2CDevAddr,
                                                        LSA_UINT8			            const  I2COffsetCnt,
                                                        LSA_UINT8			            const  I2COffset1,
                                                        LSA_UINT8			            const  I2COffset2,
                                                        LSA_UINT32                      const  Size,
                                                        LSA_UINT8  EDD_UPPER_MEM_ATTR       *  pBuf )
{
    LSA_UINT32      ToCnt;          //Timeout count
    LSA_UINT32      Len         = Size;
    LSA_UINT32      BufIdx      = 0;

    //PageWrite: START, I2CDevAddr, ByteAddr, Wr, Wr, ... , STOP

    // ***** send START:  IFLG bit will be set, Code=08h
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, EDDP_I2C_CMD_START);             // Start
    EDDP_I2C_WAIT_IFLG(pDDB, ToCnt);

    if (ToCnt) //Timeout not yet elapsed?
    {
        EDDP_I2C_WAIT_STATUS(pDDB, ToCnt, EDDP_I2C_STATE_START_0);                  // START condition transmitted
    }

    if (!ToCnt) //Timeout elapsed?
    {
        // ***** try to restart the HW state machine
        EDDP_I2C_DoMI2C_Reset(pDDB);
        EDDP_I2C_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "eddp_I2C_Write_Offset_internal(): ERROR -> START condition");
        return EDD_STS_ERR_HW;
    }
    EDDP_I2C_WAIT_2US(pDDB);

    // ***** send DEV-Address + Ack:  IFLG bit will be set, Code=18h
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_DATA, I2CDevAddr);
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, EDDP_I2C_CMD_WR_DATA);           // DEV
    EDDP_I2C_WAIT_IFLG(pDDB, ToCnt);

    if (ToCnt) //Timeout not yet elapsed?
    {
        EDDP_I2C_WAIT_STATUS(pDDB, ToCnt, EDDP_I2C_STATE_ADR_WR);                   // Address + write bit transmitted, ACK received
    }

    if (!ToCnt) //Timeout elapsed?
    {
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, EDDP_I2C_CMD_STOP);          // Stop
        EDDP_I2C_WAIT_2US(pDDB);
        EDDP_I2C_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "eddp_I2C_Write_Offset_internal(): ERROR -> I2CDevAddr");
        return EDD_STS_ERR_PROTOCOL;
    }
    EDDP_I2C_WAIT_2US(pDDB);

    switch //by I2COffsetCnt (already checked by value)
            (I2COffsetCnt)
    {
        case 1:
        {
            // ***** send E2-Address + Ack:  IFLG bit will be set, Code=28h
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_DATA, I2COffset1);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, EDDP_I2C_CMD_WR_DATA);   // E2
            EDDP_I2C_WAIT_IFLG(pDDB, ToCnt);

            if (ToCnt) //Timeout not yet elapsed?
            {
                EDDP_I2C_WAIT_STATUS(pDDB, ToCnt, EDDP_I2C_STATE_WR_DATA_ACK);      // Data byte transmitted in master mode, ACK received
            }

            if (!ToCnt) //Timeout elapsed?
            {
                EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, EDDP_I2C_CMD_STOP);  // Stop
                EDDP_I2C_WAIT_2US(pDDB);
                EDDP_I2C_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "eddp_I2C_Write_Offset_internal(): ERROR -> I2COffset1");
                return EDD_STS_ERR_PROTOCOL;
            }
            EDDP_I2C_WAIT_2US(pDDB);

            break;
        }
        case 2:
        {
            // ***** send E2-Address + Ack:  IFLG bit will be set, Code=28h
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_DATA, I2COffset1);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, EDDP_I2C_CMD_WR_DATA);   // E2
            EDDP_I2C_WAIT_IFLG(pDDB, ToCnt);

            if (ToCnt) //Timeout not yet elapsed?
            {
                EDDP_I2C_WAIT_STATUS(pDDB, ToCnt, EDDP_I2C_STATE_WR_DATA_ACK);      // Data byte transmitted in master mode, ACK received
            }

            if (!ToCnt) //Timeout elapsed?
            {
                EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, EDDP_I2C_CMD_STOP);  // Stop
                EDDP_I2C_WAIT_2US(pDDB);
                EDDP_I2C_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "eddp_I2C_Write_Offset_internal(): ERROR -> I2COffset1");
                return EDD_STS_ERR_PROTOCOL;
            }
            EDDP_I2C_WAIT_2US(pDDB);

            // ***** send E2-Address + Ack:  IFLG bit will be set, Code=28h
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_DATA, I2COffset2);
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, EDDP_I2C_CMD_WR_DATA);   // E2
            EDDP_I2C_WAIT_IFLG(pDDB, ToCnt);

            if (ToCnt) //Timeout not yet elapsed?
            {
                EDDP_I2C_WAIT_STATUS(pDDB, ToCnt, EDDP_I2C_STATE_WR_DATA_ACK);      // Data byte transmitted in master mode, ACK received
            }

            if (!ToCnt) //Timeout elapsed?
            {
                EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, EDDP_I2C_CMD_STOP);  // Stop
                EDDP_I2C_WAIT_2US(pDDB);
                EDDP_I2C_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "eddp_I2C_Write_Offset_internal(): ERROR -> I2COffset2");
                return EDD_STS_ERR_PROTOCOL;
            }
            EDDP_I2C_WAIT_2US(pDDB);

            break;
        }
        default: break;
    }

    while (Len)
    {
        // ***** send DataByte + Ack:  IFLG bit will be set, Code=28h
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_DATA, pBuf[BufIdx]);                                     
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, EDDP_I2C_CMD_WR_DATA);               // D[?]
        EDDP_I2C_WAIT_IFLG(pDDB, ToCnt);

        if (ToCnt) //Timeout not yet elapsed?
        {
            EDDP_I2C_WAIT_STATUS(pDDB, ToCnt, EDDP_I2C_STATE_WR_DATA_ACK);                  // Data byte transmitted in master mode, ACK received
        }

        if (!ToCnt) //Timeout elapsed?
        {
            EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, EDDP_I2C_CMD_STOP);              // Stop
            EDDP_I2C_WAIT_2US(pDDB);
            EDDP_I2C_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "eddp_I2C_Write_Offset_internal(): ERROR -> Ack/Nack");
            return EDD_STS_ERR_PROTOCOL;
        }

        Len--;
        BufIdx++;

    } //end of while loop

    // ***** send Stop:  IFLG bit will not be set, Code=F8h
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_MI2C_CNTR, EDDP_I2C_CMD_STOP);                      // Stop
    EDDP_I2C_WAIT_2US(pDDB);
    EDDP_I2C_WAIT_STATUS(pDDB, ToCnt, EDDP_I2C_STATE_IDLE);                                 // No relevant status information, IFLG=0

    if (!ToCnt) //Timeout elapsed?
    {
        EDDP_I2C_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "eddp_I2C_Write_Offset_internal(): ERROR -> STOP condition");
        return EDD_STS_ERR_PROTOCOL;
    }

    return EDD_STS_OK;
}

#endif /* #ifdef LSAS_CFG_USE_POF */
/*****************************************************************************/
/*  end of file eddp_i2c.c                                                   */
/*****************************************************************************/
