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

#ifndef __EDDP_RZT2_H__                  /* ----- reinclude-protection ----- */
#define __EDDP_RZT2_H__

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

#define RZT2_NUMBER_OF_PORTS                 ETHER_CFG_MAX_PORTS
#define RZT2_ETHER_BUFFER_SIZE               ETHER_BUFFER_SIZE


#define RZT2_CRC_32_IEE802_3                  0x04C11DB7ul /* CRC-32 Polynom (IEEE802.3) */
#define RZT2_err                              ~(0u)        /* error value                */
#define RZT2_CR                               0x3          /* clock range; 3: 35-60 MHz  */
#define RZT2_PHY_ID                           BOARD_PHY_TYPE

#define RZT2_TIMEOUT_US                       10
#define RZT2_CRC_LENGTH                       4


#define EDDP_LL_CAST_TYPE(_PTR, _TYPE)		((_TYPE*)_PTR)

#define EDDP_LL_CAST_HDL(_PTR)				EDDP_LL_CAST_TYPE(_PTR, EDDP_LL_HANDLE_TYPE)
#define EDDP_LL_CAST_ETH(_PTR)				EDDP_LL_CAST_TYPE(_PTR, ether_instance_t)
#define EDDP_LL_CAST_PHY(_PTR)				EDDP_LL_CAST_TYPE(_PTR, ether_phy_instance_t)
#define EDDP_LL_CAST_SWI(_PTR)				EDDP_LL_CAST_TYPE(_PTR, ether_switch_instance_t)

#define EDDP_LL_ETH_CTRL_PTR(_PTR)			EDDP_LL_CAST_ETH(_PTR)->p_ctrl
#define EDDP_LL_PHY_PTR(_PTR, _PHY)			&(RZT2_ETH_CTRL_PTR(_PTR)->p_ether_cfg[_PHY]
#define EDDP_LL_PHY_API_PTR(_PTR, _PHY)		RZT2_PHY_PTR(_PTR, _PHY)->p_api
#define EDDP_LL_PHY_CTRL_PTR(_PTR, _PHY)	RZT2_PHY_PTR(_PTR)->p_ctrl
#define EDDP_LL_PHY_CFG_PTR(_PTR, _PHY)		RZT2_PHY_PTR(_PTR)->p_cfg


#define EDDP_LL_ETH_OPEN(_pIF)			_pIF->p_api->open(_pIF->p_ctrl, _pIF->p_cfg)
#define EDDP_LL_ETH_CLOSE(_pIF)			_pIF->p_api->close(_pIF->p_ctrl)
#define EDDP_LL_ETH_SHUTDOWN(_pIF)		_pIF->p_api->shutdown(_pIF->p_ctrl)


typedef struct _RZT2_GMAC_STATUS_TYPE
{
	uint32_t TI:  1U;	/* b0:  TI  R/W: Transmit Interrupt This bit indicates that the frame transmission is complete. When transmission is complete, Bit 31 (OWN) of TDES0 is reset, and the specific frame status information is updated in the descriptor. */
	uint32_t TPS: 1U;  	/* b1:  TPS R/W: Transmit Process Stopped This bit is set when the transmission is stopped. */
	uint32_t TU:  1U;   /* b2:  TU  R/W: Transmit Buffer Unavailable This bit indicates that the host owns the Next Descriptor in the Transmit List and the DMA cannot acquire it. Transmission is suspended. Bits[22:20] explain the Transmit Process state transitions. To resume processing Transmit descriptors, the host should change the ownership of the descriptor by setting TDES0[31] and then issue a Transmit Poll Demand command. */
	uint32_t TJT: 1U;	/* b3:  TJT R/W: Transmit Jabber Timeout This bit indicates that the Transmit Jabber Timer expired, which happens when the frame size exceeds 2,048 (10,240 bytes when the Jumbo frame is enabled). When the Jabber Timeout occurs, the transmission process is aborted and placed in the Stopped state. This causes the Transmit Jabber Timeout TDES0[14] flag to assert. */
	uint32_t OVF: 1U;  	/* b4:  OVF R/W: Receive Overflow This bit indicates that the Receive Buffer had an Overflow during frame reception. If the partial frame is transferred to the application, the overflow status is set in RDES0[11]. */
	uint32_t UNF: 1U;  	/* b5:  UNF R/W: Transmit Underflow This bit indicates that the Transmit Buffer had an Underflow during frame transmission. Transmission is suspended and an Underflow Error TDES0[1] is set. */
	uint32_t RI:  1U;  	/* b6:  RI  R/W: Receive Interrupt This bit indicates that the frame reception is complete. When reception is complete, the Bit 31 of RDES1 (Disable Interrupt on Completion) is reset in the last Descriptor, and the specific frame status information is updated in the descriptor. The reception remains in the Running state. */
	uint32_t RU:  1U;  	/* b7:  RU  R/W: Receive Buffer Unavailable This bit indicates that the host owns the Next Descriptor in the Receive List and the DMA cannot acquire it. The Receive Process is suspended. To resume processing Receive descriptors, the host should change the ownership of the descriptor and issue a Receive Poll Demand command. If no Receive Poll Demand is issued, the Receive Process resumes when the next recognized incoming frame is received. This bit is set only when the previous Receive Descriptor is owned by the DMA. */
	uint32_t RPS: 1U;  	/* b8:  RPS R/W: Receive Process Stopped This bit is asserted when the Receive Process enters the Stopped state. */
	uint32_t RWT: 1U;   /* b9:  RWT R/W: Receive Watchdog Timeout When set, this bit indicates that the Receive Watchdog Timer expired while receiving the current frame and the current frame is truncated after the watchdog timeout. */
	uint32_t ETI: 1U;   /* b10: ETI R/W: Early Transmit Interrupt This bit indicates that the frame to be transmitted is fully transferred to the MTL Transmit FIFO.  */
	uint32_t b11: 1U;   /* b11: R:   Reserved */
	uint32_t b12: 1U;   /* b12: R:   Reserved */
	uint32_t FBI: 1U;   /* b13: FBI R/W: Fatal Bus Error Interrupt This bit indicates that a bus error occurred, as described in Bits[25:23]. When this bit is set, the corresponding DMA engine disables all of its bus accesses. */
	uint32_t ERI: 1U;   /* b14: ERI R/W: Early Receive Interrupt cleared when the software writes 1 to this bit or Bit 6 (RI) of this register is set (whichever occurs earlier). */
	uint32_t AIS: 1U;   /* b15: AIS R/W: Abnormal Interrupt Summary Abnormal Interrupt Summary bit value is the logical OR of the following bits when the
										  corresponding interrupt bits are enabled in Interrupt Enable Register (Interrupt_Enable):
											- Interrupt_Enable [1]: Transmit Process Stopped
											- Interrupt_Enable [3]: Transmit Jabber Timeout
											- Interrupt_Enable [4]: Receive FIFO Overflow
											- Interrupt_Enable [5]: Transmit Underflow
											- Interrupt_Enable [7]: Receive Buffer Unavailable
											- Interrupt_Enable [8]: Receive Process Stopped
											- Interrupt_Enable [9]: Receive Watchdog Timeout
											- Interrupt_Enable [10]: Early Transmit Interrupt
											- Interrupt_Enable [13]: Fatal Bus Error
										  Only unmasked bits affect the Abnormal Interrupt Summary bit. This is a sticky bit and must be cleared each time a corresponding bit, which causes AIS to be set, is cleared. */
	uint32_t NIS: 1U;   /* b116: NIS R/W: Normal Interrupt Summary Normal Interrupt Summary bit value is the logical OR of the following bits when the
											corresponding interrupt bits are enabled in Interrupt Enable Register (Interrupt_Enable):
											- Interrupt_Enable [0]: Transmit Interrupt
											- Interrupt_Enable [2]: Transmit Buffer Unavailable
											- Interrupt_Enable [6]: Receive Interrupt
											- Interrupt_Enable [14]: Early Receive Interrupt
											Only unmasked bits (interrupts for which interrupt enable is set in Interrupt Enable Register (Interrupt_Enable)) affect the Normal Interrupt Summary bit.
											This is a sticky bit and must be cleared (by writing 1 to this bit) each time a corresponding bit, which causes NIS to be set, is cleared. */
	uint32_t RS: 3U;   /* bits[19-17]: RS R/W: Received Process State
												This field indicates the Receive DMA FSM state. This field does not generate an interrupt.
												000: Stopped: Reset or Stop Receive Command issued
												001: Running: Fetching Receive Transfer Descriptor
												010: Reserved for future use
												011: Running: Waiting for receive packet
												100: Suspended: Receive Descriptor Unavailable
												101: Running: Closing Receive Descriptor
												110: TIME_STAMP write state
												111: Running: Transferring the receive packet data from receive buffer to host memory */
	uint32_t TS: 3U;   /* bits[22-20]: TS R/W:  Transmit Process State
												This field indicates the Transmit DMA FSM state. This field does not generate an
												000: Stopped; Reset or Stop Transmit Command issued
												001: Running; Fetching Transmit Transfer Descriptor
												010: Running; Waiting for status
												011: Running; Reading Data from host memory buffer and queuing it to transmit buffer (Tx FIFO)
												100: TIME_STAMP write state
												101: Reserved for future use
												110: Suspended; Transmit Descriptor Unavailable or Transmit Buffer Underflow
												111: Running; Closing Transmit Descriptor*/
	uint32_t EB: 3U;   /* bits[25-23]: EB R/W:  Error Bits
												This field indicates the type of error that caused a Bus Error, for example, error
												response on the AXI interface. This field is valid only when Bit 13 (FBI) is set. This
												field does not generate an interrupt.
													000: Error during Rx DMA Write Data Transfer
													001 and 010: Reserved
													011: Error during Tx DMA Read Data Transfer
													100: Error during Rx DMA Descriptor Write Access
													101: Error during Tx DMA Descriptor Write Access
													110: Error during Rx DMA Descriptor Read Access
													111: Error during Tx DMA Descriptor Read Access*/
	uint32_t GMI: 1U;   /* b27: GMI R/W: GMAC MMC Interrupt: This bit reflects an interrupt event in the MMC module of the GMAC. The software must read the corresponding registers in the GMAC to get the exact cause of interrupt and clear the source of interrupt to make this bit as 1’b0. The interrupt signal from the GMAC subsystem (GMAC[1]_SBD_Int) is high when this bit is high. */
	uint32_t GPI: 1U;   /* b28: GPI R/W: GMAC PMT Interrupt: This bit indicates an interrupt event in the PMT module of the GMAC. The software must read the PMT Control and Status Register in the GMAC to get the exact cause of interrupt and clear its source to reset this bit to 1’b0. The interrupt signal from the GMAC subsystem (GMAC[1]_SBD_Int) is high when this bit is high. Note) The GPI and GMAC[1]_PMT_Int interrupts are generated in different clock domains. */
	uint32_t TTI: 1U;   /* b29 TTI R/W:  Timestamp Trigger Interrupt: This bit indicates an interrupt event in the Timestamp Generator block of GMAC. The software must read the corresponding registers in the GMAC to get the exact cause of interrupt and clear its source to reset this bit to 1’b0. When this bit is high, the interrupt signal from the GMAC subsystem (GMAC[1]_SBD_Int) is high. */
	uint32_t GLPII: 1U; /* b30: GLPII R/W:  GMAC LPI Interrupt: This bit indicates an interrupt event in the LPI logic of the GMAC. To reset this bit to 1’b0, the software must read the corresponding registers in the GMAC to get the exact cause of the interrupt and clear its source. When this bit is high, the interrupt signal from the GMAC (GMAC[1]_SBD_Int) is high.*/
	uint32_t b31: 1U;   /* b31: ETI R/W:  Reserved */
}RZT2_GMAC_STATUS_TYPE, *RZT2_GMAC_STATUS_PTR_TYPE;

typedef union _RZT2_GMAC_STATUS
{
	uint32_t u32;
	RZT2_GMAC_STATUS_TYPE tBits;
}RZT2_GMAC_STATUS, *RZT2_GMAC_STATUS_PTR;




LSA_EXTERN EDDP_HDB_NRT_TYPE g_EDDP_HDB_NRT[EDDP_CFG_MAX_CHANNELS];

LSA_EXTERN LSA_UINT8   g_EDDP_PHY_MDI_X_MAPPING[EDDP_CFG_MAX_DEVICES][EDDP_MAX_PORT_CNT];
LSA_EXTERN LSA_UINT8   g_EDDP_PHY_ISWIRELESS[EDDP_CFG_MAX_DEVICES][EDDP_MAX_PORT_CNT];
LSA_EXTERN LSA_UINT16  g_EDDP_PHY_MAUTYPE[EDDP_CFG_MAX_DEVICES][EDDP_MAX_PORT_CNT];
LSA_EXTERN LSA_UINT8   g_EDDP_PHY_MEDIATYPE[EDDP_CFG_MAX_DEVICES][EDDP_MAX_PORT_CNT];	//array port-index = HWPortIdx
LSA_EXTERN LSA_UINT8   g_EDDP_PHY_OPTICALTRANSTYPE[EDDP_CFG_MAX_DEVICES][EDDP_MAX_PORT_CNT];
LSA_EXTERN LSA_UINT8   g_EDDP_PHY_FXTRANSCEIVERTYPE[EDDP_CFG_MAX_DEVICES][EDDP_MAX_PORT_CNT];
LSA_EXTERN LSA_UINT8   g_EDDP_PHY_LINK_SPEED_MODE_CONFIGURED[EDDP_CFG_MAX_DEVICES][EDDP_MAX_PORT_CNT];

LSA_EXTERN void start_delay_timer(uint32_t delay_time);
LSA_EXTERN int ethsw_port_fdb_add(int portmask, uint8_t *addr, uint16_t vid);
LSA_EXTERN LSA_VOID lsas_edds_scheduler_edd_task(LSAS_EDD_RQB_TYPE *rqb);
LSA_EXTERN PNIO_UINT32 OsTaskNotifyTake(PNIO_UINT32 TaskId);
LSA_EXTERN PNIO_VOID OsTaskNotifyGiveFromISR(PNIO_UINT32 TaskId);

//LSA_EXTERN void    ether_enable_icu(ether_instance_ctrl_t * const p_instance_ctrl);
//LSA_EXTERN void    ether_disable_icu(ether_instance_ctrl_t * const p_instance_ctrl);
LSA_EXTERN void ether_reset_mac(volatile R_GMAC_Type * const p_reg);
LSA_EXTERN void ether_init_descriptors(ether_instance_ctrl_t * const p_instance_ctrl);
LSA_EXTERN void ether_config_ethernet(ether_instance_ctrl_t const * const p_instance_ctrl, const uint8_t mode);
LSA_EXTERN void ether_pause_resolution(uint32_t const local_ability, uint32_t const partner_ability, uint32_t *ptx_pause, uint32_t *prx_pause);
LSA_EXTERN void ether_configure_mac(ether_instance_ctrl_t * const p_instance_ctrl, const uint8_t mac_addr[], const uint8_t mode);
LSA_EXTERN fsp_err_t ether_buffer_get(ether_instance_ctrl_t * const p_instance_ctrl, void ** const p_buffer, uint32_t * p_buffer_size);
//RZT2_FUNC_ATTR void      ether_disable (ether_instance_ctrl_t * const p_instance_ctrl);
LSA_EXTERN PNIO_VOID PnUsr_DeactivateIoDatXch    (void);

extern LSA_VOID PNIP_HAL_REG32_WRITE(LSA_UINT32 u32Reg, LSA_UINT32 u32Val);
extern LSA_VOID PNIP_HAL_PHY_SET_POWER(LSA_UINT16 HWPortID, LSA_UINT16 PwrCtrl);
extern LSA_VOID PNIP_HAL_PHY_SET_LINK(LSA_UINT16 HWPortID, LSA_UINT16 LinkStatus, LSA_UINT16 Speed, LSA_UINT16  Duplexity);
extern EDD_RSP  EDDP_NRTRecv(EDD_UPPER_RQB_PTR_TYPE pRQB, EDDP_LOCAL_DDB_PTR_TYPE pDDB, EDDP_LOCAL_HDB_PTR_TYPE pHDB);

LSA_VOID EDDP_LL_CheckMailbox(EDDP_LL_HANDLE_PTR_TYPE pHandle, LSA_UINT32 Timeout);
LSA_VOID EDDP_LL_LINK_CHANGE_IND(EDDP_LL_HANDLE_PTR_TYPE pHandle);
EDD_RSP  EDDP_LL_PHY_CTRL(EDDP_LL_HANDLE_PTR_TYPE pHandle, LSA_UINT32 PortID, LSA_UINT32 PhyCtrl, LSA_BOOL bUpdateReg);
EDD_RSP  EDDP_LL_LINK_CTRL(EDDP_LL_HANDLE_PTR_TYPE pHandle, LSA_UINT16 PortID, LSA_UINT32 LinkCtrl);
EDD_RSP  EDDP_LL_ExeCmd(EDDP_LL_HANDLE_PTR_TYPE pHandle, EDDP_LOCAL_DDB_PTR_TYPE pDDB, EDDP_CMD_PARAM_PTR_TYPE pParam);
LSA_VOID EDDP_LL_FilterFrame(EDDP_LL_HANDLE_PTR_TYPE pHandle, LSA_UINT16 PortID, LSA_VOID *pvFrame, LSA_UINT16 Size,  LSA_UINT32 pvTimeStamp);
EDD_RSP EDDP_LL_LinkStatToEddp(LSA_UINT32 LinkCfg, LSA_UINT32 *pSpeed, LSA_UINT32 *pMode, LSA_UINT32 *pMAUType);
EDD_RSP EDDP_LL_LinkStatToHost(LSA_UINT32 EddpLinkCfg, LSA_UINT32 *pSpeed, LSA_UINT32 *pMode, LSA_UINT32 *pMAUType, LSA_UINT32 *pHostLinkCfg);


LSA_EXTERN LSA_UINT16 iom_perif_consumer_lock(LSA_VOID **buffer, LSA_VOID **apdu_status_ptr, LSA_UINT32 ar_idx);
LSA_EXTERN LSA_UINT16 iom_perif_consumer_unlock(LSA_UINT32 ar_idx);
LSA_EXTERN LSA_VOID   iom_perif_provider_lock(LSA_VOID **buffer, LSA_UINT32 ar_idx);
LSA_EXTERN LSA_UINT16 iom_perif_provider_unlock(LSA_UINT32 ar_idx);


LSA_EXTERN PNIO_UINT32 NumOfAr;
LSA_EXTERN LSA_UINT16 g_NumOfRtc3Ar;
LSA_EXTERN LSA_UINT16 g_NumOfRtcxAr;
extern PNIO_UINT32   TskId_CycleIO;
//extern iom_data_t iom_data;

extern ether_instance_descriptor_t     tx_descriptors[NUM_TX_DESCRIPTOR];
#define g_tx_descriptors_nrt           tx_descriptors

#define TX_DESCR_ADDR_RTC3_FIRST	((LSA_UINT32)&g_tx_descriptors_rtc3[0])
#define TX_DESCR_ADDR_RTC3_LAST		((LSA_UINT32)&g_tx_descriptors_rtc3[(DEV_CFG_MAX_ARS_RTC3 -1U)])

#define TX_DESCR_ADDR_RTCX_FIRST	((LSA_UINT32)&g_tx_descriptors_rtcx[0])
#define TX_DESCR_ADDR_RTCX_LAST		((LSA_UINT32)&g_tx_descriptors_rtcx[(DEV_CFG_MAX_ARS_RTC1_RTC2 -1U)])

#define TX_DESCR_ADDR_NRT_FIRST		((LSA_UINT32)&g_tx_descriptors_nrt[0])
#define TX_DESCR_ADDR_NRT_LAST		((LSA_UINT32)&g_tx_descriptors_nrt[(NUM_TX_DESCRIPTOR -1U)])



#define EDDP_LL_PHASE_RED			(0x01U)
#define EDDP_LL_PHASE_GREEN			(0x02U)
#define EDDP_LL_PHASE_YELLOW		(0x03U)

extern uint32_t * ptp_time_stamp_nanos[2*NUM_TX_DESCRIPTOR];

extern    void (* p_ptp_callback)(                        ///< pointer of callback function called when an PTP Interupt occurs
               ether_switch_ptp_event_t   event,
               uint8_t                    port,
               uint32_t                   nanos,
               ether_switch_timestamp_t * p_timestamp);

LSA_VOID ethSendEvt(ether_switch_ptp_event_t event, uint8_t port, uint32_t nanos, ether_switch_timestamp_t * p_timestamp);


typedef void (*set_mgtag_data_fptr) (ether_instance_ctrl_t * tag,
	                                       void (**)(ether_switch_ptp_event_t,uint8_t,ether_switch_timestamp_t *)
	                                       );
// Used to handle a new cycle time or TDMA setting in the red phasejust before the new cycle starts
typedef struct tdma_update_data {
bool update_tdma;                  // True, if a new TDMA configuration is available 
uint32_t cycle_time;               // New cycle time, 0 if no change
uint32_t num_phases;               // amount of phases
uint32_t start_address;            // Start address in TDMA TCV table
uint32_t end_address;              // End address in TDMA TCV table
} tdma_update_data_t; 

#define RZT2_MIB2_SUPPORTED_COUNTERS_PORT ( EDD_MIB_SUPPORT_INOCTETS            \
                                            | EDD_MIB_SUPPORT_INUCASTPKTS         \
                                            | EDD_MIB_SUPPORT_INNUCASTPKTS        \
                                            | EDD_MIB_SUPPORT_INDISCARDS          \
                                            | EDD_MIB_SUPPORT_INERRORS            \
                                            | EDD_MIB_SUPPORT_OUTOCTETS           \
                                            | EDD_MIB_SUPPORT_OUTUCASTPKTS        \
                                            | EDD_MIB_SUPPORT_OUTNUCASTPKTS       \
                                            | EDD_MIB_SUPPORT_OUTDISCARDS         \
                                            | EDD_MIB_SUPPORT_OUTERRORS           \
                                            | EDD_MIB_SUPPORT_INMULTICASTPKTS     \
                                            | EDD_MIB_SUPPORT_INBROADCASTPKTS     \
                                            | EDD_MIB_SUPPORT_OUTMULTICASTPKTS    \
                                            | EDD_MIB_SUPPORT_OUTBROADCASTPKTS    \
                                            | EDD_MIB_SUPPORT_INHCOCTETS          \
                                            | EDD_MIB_SUPPORT_INHCUCASTPKTS       \
                                            | EDD_MIB_SUPPORT_INHCMULTICASTPKTS   \
                                            | EDD_MIB_SUPPORT_INHCBROADCASTPKTS   \
                                            | EDD_MIB_SUPPORT_OUTHCOCTETS         \
                                            | EDD_MIB_SUPPORT_OUTHCUCASTPKTS      \
                                            | EDD_MIB_SUPPORT_OUTHCMULTICASTPKTS  \
                                            | EDD_MIB_SUPPORT_OUTHCBROADCASTPKTS   )

#define RZT2_MIB2_SUPPORTED_COUNTERS_IF   ( EDD_MIB_SUPPORT_INOCTETS            \
                                            | EDD_MIB_SUPPORT_INUCASTPKTS         \
                                            | EDD_MIB_SUPPORT_INNUCASTPKTS        \
                                            | EDD_MIB_SUPPORT_OUTOCTETS           \
                                            | EDD_MIB_SUPPORT_OUTUCASTPKTS        \
                                            | EDD_MIB_SUPPORT_OUTNUCASTPKTS       \
                                            | EDD_MIB_SUPPORT_INMULTICASTPKTS     \
                                            | EDD_MIB_SUPPORT_INBROADCASTPKTS     \
                                            | EDD_MIB_SUPPORT_OUTMULTICASTPKTS    \
                                            | EDD_MIB_SUPPORT_OUTBROADCASTPKTS    \
                                            | EDD_MIB_SUPPORT_INHCOCTETS          \
                                            | EDD_MIB_SUPPORT_INHCUCASTPKTS       \
                                            | EDD_MIB_SUPPORT_INHCMULTICASTPKTS   \
                                            | EDD_MIB_SUPPORT_INHCBROADCASTPKTS   \
                                            | EDD_MIB_SUPPORT_OUTHCOCTETS         \
                                            | EDD_MIB_SUPPORT_OUTHCUCASTPKTS      \
                                            | EDD_MIB_SUPPORT_OUTHCMULTICASTPKTS  \
                                            | EDD_MIB_SUPPORT_OUTHCBROADCASTPKTS   )


#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif  /* __EDDP_RZT2_H__ */
