
/***********************************************************************************************************************
 * Copyright [2020] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics America Inc. and may only be used with products
 * of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.  Renesas products are
 * sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for the selection and use
 * of Renesas products and Renesas assumes no liability.  No license, express or implied, to any intellectual property
 * right is granted by Renesas. This software is protected under all applicable laws, including copyright laws. Renesas
 * reserves the right to change or discontinue this software and/or this documentation. THE SOFTWARE AND DOCUMENTATION
 * IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND TO THE FULLEST EXTENT
 * PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY, INCLUDING WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE SOFTWARE OR
 * DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.  TO THE MAXIMUM
 * EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR DOCUMENTATION
 * (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER, INCLUDING,
 * WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY LOST PROFITS,
 * OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE POSSIBILITY
 * OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup ETHER_SWITCH
 * @{
 **********************************************************************************************************************/

#define ETHSW_NR_PORTS 4
#define TC_QOPT_BITMASK 8

#define ETHSW_TIMER0 0
#define ETHSW_TIMER1 1     
     
struct ethsw_lk_entry {
        uint8_t mac[6]; 
        void *macaddr;
        int port; 
        uint16_t vid;
        bool noarp;
        uint16_t addr;
        bool valid;
};


struct taprio_sched_entries {
        uint32_t interval;         // offset of schedule entry in ns 
        uint8_t port_mask;        // Port Mask with one-hot encoding 
        uint8_t gate_mask;        // Gate Mask with one-hot encoding 
        uint8_t hold_req;          // One bit enable Hold request for frame preemption , 
        uint8_t gate_mode;          // One bit enable gate mode , only on newer devices, 
        uint8_t trigger_mode;          // One bit enable trigger mode , 
        uint8_t in_ct_enable;          // One bit enable cut through , 
        uint8_t out_ct_enable;          // One bit enable cut through , 
        uint8_t red_phase;          // Indicate Red Phase in IRT applications
        uint8_t inc_1;          // enable increment 1 , 
        uint8_t inc_0;          // enable increment 0 ,
        uint8_t gpio;           // enable gpio pin, each bit one gpio 
};

#define L2_EN_LEARNING  0x02
#define L2_EN_AGING  0x04
#define L2_EN_MIRATION  0x08
#define L2_DISCARD_UNKNOWN  0x010
#define L2_INDIPENDANT_VLAN  0x080

#define INVLD_ID_FLOOD 0x01    //  
#define INVLD_ID_LRN_ENA 0x02
#define INVLD_ID_PRIO 0x02
#define INVLD_ID_PRIO_VLD 0x080


#define UNKWN_VLAN_FLOOD  0x01   // Matching frames with unmatching VLAN use forwarding mask 
#define UNKWN_VLAN_LRN_ENA  0x02   // Allow learning for frames with inknown VLANs
#define UNKWN_VLAN_PRIO_VLD  0x080   // Priority in unkwn_vlan_prio is valid


void ia_fdb_id_config(uint8_t ia_config, uint8_t invalid_prio, uint8_t port_flood_mask, uint32_t ialk_id_min,  uint32_t ialk_id_max, uint32_t ialk_id_sub );
void ia_fdb_enable(uint8_t port_ena_mask, uint8_t cut_through_ena_mask);

// Defines for Pattern matcher
#define  MATCH_NOT 0x01      // Invert decision
#define  MGMTFWD 0x02      // Forward frame only to management port
#define  DISCARD 0x04      // Discard frame if match
#define  SET_PRIO 0x08     // Set priority to value in priority field
#define  MODE_12BYTE (0x00 << 4)          // Set mode to 12byte legacy match
#define  MODE_2BYTE_OFFSET (0x01 << 4)          // Set mode to 2byte table lookup
#define  MODE_2BYTE_RANGE (0x02 << 4)  // Set mode to 2byte offset compare
#define  MODE_2BYTE_RANGE_NOT (0x03 << 4)  // Same as 2BYTE_RAGE but inverted 
#define TIMER_SEL_OVR    0x40    // Force forwarding to port in port
#define FORCE_FORWARD    0x80    // Force forwarding to port in port
#define MATCH_RED  0x200         // Skip VLAN tag in comparison if VLAN tag is detected. 
#define MATCH_NOT_RED  0x400         // Skip VLAN tag in comparison if VLAN tag is detected. 
#define VLAN_SKIP  0x800         // Skip VLAN tag in comparison if VLAN tag is detected. 
#define LEARNING_DIS  0x8000     // Disable learning for this frame        
#define IMC_TRIGGER  (0x01 << 22)   //   
#define IMC_TRIGGER_DLY  (0x01 << 23)   //   
#define SWAP_BYTES  (0x01 << 24)   //   
#define MATCH_LT  (0x01 << 25)   //  Match Length and Type field in frame with value in length_type. 

// Defines for Queu State MMCTL_QGATE   
#define GATE_HOLD 0x00     // Hold current state
#define GATE_TOGGLE 0x01   // toggle current state
#define GATE_CLOSE 0x2    // Close queue
#define GATE_OPEN 0x3    // Open Queue

extern int64_t TimetStamp;   /* countup integration time (ms) */
extern int32_t time_milli;
extern int32_t time_sec  ;

int ethsw_port_vlan_del(int port, bool pvid, bool untagged, bool discard, int vid_begin, int vid_end);
int ethsw_port_vlan_add(int port, bool pvid, bool untagged, bool discard, int vid_begin, int vid_end);
void ethsw_enable_port_vlan( int port, bool enable );
int ethsw_port_fdb_dump(int port, struct ethsw_lk_entry *lk_entry_array, int lk_entry_size);
int ethsw_port_fdb_add(int port, unsigned char *addr, uint16_t vid);
int ethsw_port_fdb_del(int port, unsigned char *addr, uint16_t vid);
void ethsw_switch_fdb_flush(int type);
void ethsw_switch_fdb_flush_port(int port_nr);
//void ethsw_taprio_qopt_set ( uint8_t portmask, uint8_t enable, uint8_t timer_sel, uint32_t base_time, 
//                       uint32_t cycle_time, uint8_t num_entries, struct taprio_sched_entries *entries );
uint32_t ethsw_taprio_qopt_set (uint8_t portmask, uint8_t enable, uint8_t timer_sel, uint32_t base_time, uint32_t cycle_time, uint32_t num_phases, uint8_t num_entries, struct taprio_sched_entries * entries, uint8_t reset);
uint32_t  ethsw_taprio_qopt_get (struct taprio_sched_entries *entries);
void ethsw_taprio_init (uint8_t timer);
void ethsw_setup_mqprio(uint8_t port, uint8_t num_tc, uint8_t prio_tc_map[TC_QOPT_BITMASK]);
uint32_t ethsw_read_mqprio(uint8_t port);

uint32_t ethsw_get_time(uint8_t timer);
void ethsw_queue_flush(uint8_t port, uint8_t queue, uint8_t action);
void ethsw_read_pattern_matcher(uint8_t pattern_num, uint32_t *ptrn_cmp);
void ethsw_setup_mqprio_pattern_add(uint8_t portmask, uint8_t num_pattern, uint32_t action, uint8_t priority, uint8_t port_forward_mask, 
                                    uint32_t *ptrn_cmp, uint32_t *ptrn_msk, 
                                    uint16_t length_type, uint8_t offset, uint16_t and_mask, uint32_t max_min, uint8_t timer , uint8_t queuesel );
void setup_pattern_matcher_ARP(uint8_t *ip_address);
void setup_pattern_matcher_RT(uint16_t frameId, uint32_t ArIdx);
void setup_pattern_matcher_IRT_Advanced(uint16_t frameId);
void setup_pattern_matcher_IRT_Legacy(uint16_t frameId);
void ethsw_setup_qgates(uint8_t port_mask, uint16_t queue_gate );
uint8_t patternmatcher_RTC_int(uint8_t port_mask, uint16_t frameid_range_min, uint16_t frameid_range_max);
void ethsw_setup_frame_preemption(uint8_t port_mask, uint8_t queue_mask, uint8_t close_preempt_mask, uint8_t minfragsize64, uint32_t command);
void ETHSW_ResetTimer(uint8_t timer);
void ethsw_tdma_restart();
uint32_t ethsw_tda_counter_value_get();
void ether_switch_isr_pattern(ether_switch_instance_ctrl_t * p_instance_ctrl);
int clock_adjdrift(int diffsign, uint32_t drift, uint32_t duration, uint8_t timer);
int clock_adjtime(int diffsign, uint32_t delta, uint32_t interval);
uint32_t adjust_time_drift (uint32_t duration);
fsp_err_t R_ETHER_SWITCH_LinkStatusGet(ether_phy_ctrl_t * const p_ctrl);

#define PREEMPT_RESPONSE_DIS   0x04
#define PREEMPT_VERIFY_DIS   0x02
#define PREEMPT_ENA   0x01
#define PREEMPT_RX_BR_SMD_DIS (0x01 << 17)
#define PREEMPT_RX_STRICT (0x01 << 18)
#define PREEMPT_TX_MCRC_INV (0x01 << 19)
#define PREEMPT_RX_MCRC_INV (0x01 << 20)

struct ialk_vlanid_struct{
        uint16_t vlanid;             // VLANID for this VLAN. 0 matches all
        uint8_t  vlanid_ena;         // Enables this VLAnid
        uint8_t vlanid_lrn_ena;     // Enable hardware learning for this id
        uint8_t vlanid_flood_mask;    //  forwarding mask to us for frames matching vlanid but not IA table entry
        uint8_t  vlanid_prio;        // Priority for found responses
        uint8_t vlanid_prio_vld;     // Value in vlanid_prio is valid
} ;

#define PROFINET_CYCLE_TIME_INIT 1000000
#define FOS_INIT (6200-400)
#define GREEN_START_INIT PROFINET_CYCLE_TIME_INIT/4
#define YELLOW_START_INIT (PROFINET_CYCLE_TIME_INIT - GREEN_START_INIT)//250000U)    // Yellow Period is always 125 us
#define YELLOW_PHASE_DURATION 125000
#define GMAC_DELAY 70000       // delay through GMAC by which the frames are transmitted early to make sure they arrive on time at the switch
                              // Note: A value below 200 is too short to generate the INT
                              // For a single red frame of 160 Bytes and Bus Burst sizes of 8 this needs to be larger than 7500. 
                              // For a red frame of 64 Bytes and Bus Burst sizes of 8 this needs to be larger than 5500. 

// Defines for queue flush
#define QFLUSH_PORT_MASK_0    0x1
#define QFLUSH_PORT_MASK_1    0x2
#define QFLUSH_PORT_MASK_3    0x8
#define QFLUSH_QUEUE_MASK_0   0x01
#define QFLUSH_QUEUE_MASK_1   0x02
#define QFLUSH_QUEUE_MASK_2   0x04
#define QFLUSH_QUEUE_MASK_3   0x08
#define QFLUSH_QUEUE_MASK_4   0x10
#define QFLUSH_QUEUE_MASK_5   0x20
#define QFLUSH_QUEUE_MASK_6   0x40
#define QFLUSH_QUEUE_MASK_7   0x80
#define QFLUSH_QUEUE_MASK_ALL 0xFF
#define QFLUSH_ACTION_DISABLE 0x0    // Disable flush
#define QFLUSH_ACTION_CLOSE1  0x1    // Flush when queue is closed, discarding any new frames
#define QFLUSH_ACTION_CLOSE2  0x2    // Flush on queue close until empty, then stops flushing
#define QFLUSH_ACTION_TRIGGER 0x3    // TRIGGER flush until empty, then return to current flush mode

// Defines for pattern matcher
// Note: Order of the last three must stay the same 
//#define PATTERN_DISC_NRT_RED 0x0
//#define PATTERN_ALL_GREEN 0x01
//#define PATTERN_ALL_GREEN_IN_RED 0x02
#define PATTERN_ARP_FORWARD                              0x00
#define PATTERN_RT_SET_QUEUE                             0x01
#define PATTERN_IRT_SET_QUEUE                            0x02
#define PATTERN_IRT_SET_QUEUE_DLAY                       0x03
#define PATTERN_DISC_IRT                                 0x04
#define PATTERN_RTC_DISC_OWN_OUTPUT_CR_RT                0x05
#define PATTERN_RTC_DISC_OWN_OUTPUT_CR_IRT               0x06
#define PATTERN_DISC_IRT_NRED                            0x07
#define PATTERN_TIMER_SET_PDELRX                         0x08
#define PATTERN_DISC_IRT_RED                             0x09
#define PATTERN_ARP_UPPER_IPADDRESS                      0x0a
#define PATTERN_ARP_LOWER_IPADDRESS                      0x0b

//#define PATTERN_DISC_NRT_RED_MASK 0x01
//#define PATTERN_ALL_GREEN_MASK 0x02
//#define PATTERN_ALL_GREEN_IN_RED_MASK 0x04
#define PATTERN_ARP_FORWARD_MASK                         0x0001
#define PATTERN_RT_SET_QUEUE_MASK                        0x0002
#define PATTERN_IRT_SET_QUEUE_MASK                       0x0004
#define PATTERN_IRT_SET_QUEUE_DLAY_MASK                  0x0008
#define PATTERN_DISC_IRT_MASK                            0x0010
#define PATTERN_RTC_DISC_OWN_OUTPUT_CR_RT_MASK           0x0020
#define PATTERN_RTC_DISC_OWN_OUTPUT_CR_IRT_MASK          0x0040
#define PATTERN_DISC_IRT_NRED_MASK                       0x0080
#define PATTERN_TIMER_SET_PDELRX_MASK                    0x0100
#define PATTERN_DISC_IRT_RED_MASK                        0x0200
#define PATTERN_ARP_UPPER_IPADDRESS_MASK                 0x0400
#define PATTERN_ARP_LOWER_IPADDRESS_MASK                 0x0800