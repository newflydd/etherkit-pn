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


/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "common_data.h"
#include "ether_config.h"
#include "eddp_cfg.h"

#if 0
    #include "bsp_api.h"
    #include "r_ether_rzt2.h"
	#include "ETHSW_SFR.h"
    #include "r_ether_api_rzt2.h"
    #include "r_ether_phy_rzt2.h"
    #include "r_ether_phy_api_rzt2.h"
    #include "r_ether_switch_rzt2.h"
    #include "r_ether_switch_api_rzt2.h"
    #include "r_uart_api.h"
    #include "hal_data.h"
#endif
#include "r_ether_switch_rzt2_ext.h"
#include "bspadapt.h"

    #define UNCACHED_BUFFER
    #define Delay_usec(usecs_delay) R_BSP_SoftwareDelay(usecs_delay, BSP_DELAY_UNITS_MICROSECONDS)
#define TDMA_TCV_MAX_DEPTH (512)    // Depth of TCV Vector table for TDMA Controller
#define TDMA_SEQ_MAX_DEPTH (4096)    // Depth of Sequence Vector Table for TDMA Controller
#define PTPCLK 125000000
#define PTPCLK_CYCLE (1000000000/PTPCLK)
/* ether TX Profinet Frame struct            */
   typedef struct
{
    uint8_t     des_address[6];
    uint8_t     sur_address[6];
    uint16_t    mgmt_tag;
    uint16_t    mgmt_tag_data;
    uint16_t    mgmt_tag_data20;
    uint16_t    mgmt_tag_data21;    
    uint16_t    type;
    uint16_t    frameid;
    uint8_t     data[44];         // Needed to match cycle count, if VLAN is used reduce by 2
    uint16_t    cycle_cnt;
    uint8_t     data_status;
    uint8_t     transfer_status;
    
} ether_frame_rtc3_t;   // Profinet IRT

#if 0
#pragma location="nocache"
#pragma data_alignment=4
ether_frame_rtc3_t   ether_tx_frame_pn_rtc3 = 
{
    .des_address        = { 0x01, 0x0e, 0xcf, 0x00, 0x01, 0x01 },
    .sur_address        = { 0x74, 0x90, 0x50, 0x10, 0xe9, 0x07 },
    .mgmt_tag           = 0x01E0, 
    .mgmt_tag_data      = 0x0080a,       // Destination queue 2 to transmit at red2 time, enable timestamping 
    .mgmt_tag_data20    = 0x00,
    .mgmt_tag_data21    = 0x00,
    .type               = 0x9288,
    .frameid            = 0x0001,     // Bytes swappes due to endianess
    .data               = {0xaa,0xff, 0xbb, 0xff, 0xff},   // Data IOPS Data IOPS, IOCS up to 34 bytes 
    .cycle_cnt          = 0x0000,
    .data_status        = 0x0034,      // No Probs, Process running, Data Valid, Primary State
    .transfer_status    = 0x0000    
};
#endif

extern ether_instance_descriptor_t tx_descriptor_red[5];

extern ether_instance_descriptor_t tx_descriptor_base;
extern fsp_err_t R_IOPORT_PinWrite (ioport_ctrl_t * const p_ctrl, bsp_io_port_pin_t pin, bsp_io_level_t level);
void pps_out(uint8_t timer, uint32_t start_sec, uint32_t start, uint32_t period);
void ptp_out(uint8_t timer, uint32_t start_sec, uint32_t start_ns, uint32_t period, uint8_t number);
void ethsw_enable_pattern_matcher(uint8_t portmask, uint32_t pattern_mask);


void ETHSW_DisableTimer(uint16_t timer_id);

// Used to handle a new cycle time or TDMA setting in the red phasejust before the new cycle starts
typedef struct  tdma_update_data  {
bool update_tdma;                  // True, if a new TDMA configuration is available 
uint32_t cycle_time;               // New cycle time, 0 if no change
uint32_t num_phases;               // amount of phases
uint32_t start_address;            // Start address in TDMA TCV table
uint32_t end_address;              // End address in TDMA TCV table
} tdma_update_data_t; 

extern tdma_update_data_t tdma_update_data; 


/* Configure the Lookup table for L2 frames 
enable: 0 for disable, 1 for enable
config: ored defines as requested
disc_unknown_map: discard unknown frames for each port as in the map*/
void ethsw_fdb_init(uint8_t enable, uint32_t config, uint8_t disc_unknown_map) {
        R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
  
    p_reg_ethsw->LK_CTRL = config | (disc_unknown_map << 16) | enable; 
  
}

/* Remove entries for forwarding lookup table associated to port_nr.
 * Remove only dynamic entries (no static entries), i.e. only entries added in
 * learning mode.
 */
void ethsw_switch_fdb_flush_port(int port_nr)
{
        R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
	uint32_t val;

	val = ETHSW_LK_DELETE_PORT | ETHSW_LK_WAIT_COMPLETE;
	val = val | ETHSW_LK_DH_PORT_NUM(port_nr); //  ETHSW_LK_PORT_NUM(port_nr);
	p_reg_ethsw-> LK_ADDR_CTRL = val;
}


/* Remove entries for forwarding lookup table
 * int type: Selects which type of entries are deleted, can be CLEAR_ALL, CLER_DYNAMIC, CLEAR_STATIC
 */
void ethsw_switch_fdb_flush(int type)
{
        R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
	uint32_t val;

        if (type == ETHSW_MAC_CLEAR_ALL) {
        val = p_reg_ethsw->LK_CTRL;
	val |=  ETHSW_LK_CLEAR_TABLE;
	p_reg_ethsw-> LK_CTRL = val;
          
        return;  
        }
        else if (type == ETHSW_MAC_CLEAR_DYNAMIC) {
        val = p_reg_ethsw->LK_ADDR_CTRL;
	val |=  ETHSW_LK_CLEAR_DYNAMIC | ETHSW_LK_WAIT_COMPLETE;
	p_reg_ethsw-> LK_ADDR_CTRL = val;
        }
        else if (type == ETHSW_MAC_CLEAR_STATIC){
        val = p_reg_ethsw->LK_ADDR_CTRL;
	val |=  ETHSW_LK_CLEAR_STATIC | ETHSW_LK_WAIT_COMPLETE;
	p_reg_ethsw-> LK_ADDR_CTRL = val;          
        }
}

/*Writes the mac address into the datahi/datalo register of the switch. The value in datahi is used to write the upper 16 control bits in datahi */
void ethsw_fdb_mac_write( uint32_t datahi, const unsigned char *macaddr )
{
        R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
        
        p_reg_ethsw->LK_DATA_LO = macaddr[3]<<24|macaddr[2]<<16|macaddr[1]<<8|macaddr[0];
        p_reg_ethsw->LK_DATA_HI = datahi | macaddr[5]<<8|macaddr[4];
        return; 
}

/* lookup mac address in lookup table, returns 1 if found, 0 otherwise 
 * lk_addr is updated with found address or empty address for this mac address if no entry found.  */
int ethsw_lk_lookup(const unsigned char *macaddr, uint16_t *lk_address, uint16_t vid)
{
        R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;

	uint32_t val, datahi; 
	do   // wait until previous transfer is finished
		{
                        val = p_reg_ethsw->LK_ADDR_CTRL;
		}   while (val & (uint32_t)ETHSW_LK_BUSY);
	ethsw_fdb_mac_write(0, macaddr);

        p_reg_ethsw->LK_DATA_HI2 = (vid << 8);
        p_reg_ethsw->LK_ADDR_CTRL = ETHSW_LK_LOOKUP;
	do  
		{
			val = p_reg_ethsw->LK_ADDR_CTRL;
		} while (val & (uint32_t)ETHSW_LK_BUSY) ;

	*lk_address =  (uint16_t) (val & ETHSW_LK_ADDR_MASK);  // If no entry found the switch will return anempty address that can be used
	datahi = p_reg_ethsw->LK_DATA_HI;
	if (datahi & ETHSW_LK_DH_VALID) {
		return 1; 
	}
	return 0;
}

/* Delete an entry from the FDB for the respective port / address/vid combination  */
int ethsw_port_fdb_del(int port, unsigned char *addr, uint16_t vid)
{
        uint16_t lk_address;
        R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;

	if (!ethsw_lk_lookup(addr, &lk_address, vid)) {
		return -1;   // No maching entry found
	}
        p_reg_ethsw->LK_DATA_HI2 = vid << 8;
	p_reg_ethsw->LK_ADDR_CTRL = ETHSW_LK_CLEAR | lk_address;
	
        return 1;
}

/* Add an entry to the lookup table, these are always static */
int ethsw_port_fdb_add(int port, unsigned char *addr, uint16_t vid)
{
        uint16_t lk_address;
        R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;

	if (ethsw_lk_lookup(addr, &lk_address, vid)) {
		return -1;    // Address already used 
	}
	ethsw_fdb_mac_write(ETHSW_LK_DH_PORT_NUM(port)| ETHSW_LK_DH_STATIC | ETHSW_LK_DH_VALID, addr);
        p_reg_ethsw->LK_DATA_HI2 = vid <<8;
	p_reg_ethsw->LK_ADDR_CTRL = ETHSW_LK_WRITE | lk_address;		

        return ((int) lk_address);
}

// Add a group of mac addresses to the FDP starting at the given MAC address counting up the amount of addresses as given in range
int ethsw_port_fdb_add_range(int portmask, uint8_t *addr, uint16_t vid, uint16_t range)  {

  int cnt;
  uint8_t * p_addr;
  p_addr = &addr[0];

  for (cnt = 0; cnt < range; cnt++) {
    ethsw_port_fdb_add(portmask, p_addr, vid);   // Add Profinet MAC address
    *(p_addr+5) += 1;
  }

  return cnt;
}
#if 0
/*
Dump the FDB table into a buffer. 
port: 
lk_entry_array: Struct to hold one FDB entry
lk_entry_size:   size of buffer to hold the read entries for display
*/
int ethsw_port_fdb_dump(int port, struct ethsw_lk_entry *lk_entry_array, int lk_entry_size)
{
	struct ethsw_lk_entry *lk_entry;
        int lk_entry_count = 0;
        R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;

	unsigned int count = 0;
	uint32_t val, datalo, datahi, datahi2;

        lk_entry = lk_entry_array;
        do {
                p_reg_ethsw->LK_ADDR_CTRL = ETHSW_LK_READ | count;
		do  {
                        val = p_reg_ethsw->LK_ADDR_CTRL;
		}	while (val & (uint32_t)ETHSW_LK_BUSY); 
                datahi = p_reg_ethsw->LK_DATA_HI;
		if (datahi & ETHSW_LK_DH_VALID) {
                datalo = p_reg_ethsw->LK_DATA_LO;
                  if (datahi & (0x01 << (port + 21))) {
				//datahi2 = readl(priv->regs + ETHSW_LK_DATA_HI2);
				datahi2 = p_reg_ethsw->LK_DATA_HI2;
				lk_entry->mac[0] = datalo & 0x000000ff; 
				lk_entry->mac[1] = (datalo & 0x0000ff00) >> 8; 
				lk_entry->mac[2] = (datalo & 0x00ff0000) >> 16; 
				lk_entry->mac[3] = (datalo & 0xff000000) >> 24; 
				lk_entry->mac[4] = (datahi & 0x000000ff); 
				lk_entry->mac[5] = (datahi & 0x0000ff00) >> 8; 
                                lk_entry->addr = count;
				lk_entry->valid = 1;
				lk_entry->vid = (datahi2 & 0x000fff00) >> 8;
				lk_entry->noarp = (datahi & ETHSW_LK_DH_STATIC) ? 1 : 0;
                                if (lk_entry_count++ < lk_entry_size) {
                                  lk_entry++;
                                }
				//cb(lk_entry.mac, lk_entry.vid, lk_entry.noarp, data);
			}
		}
        } while (count++ < ETHER_SWITCH_LOOK_UP_SIZE);

        return (lk_entry_count);
}



/*  Configures the learning process of the IA FDB 
priority:  Priority to use for found for an ID outside the defined range 
ia_config: Configuration of fdb using the defines above
port_flood_mask:  ports to which fraes with invalid ID should be forwarded  */ 
void ia_fdb_id_config(uint8_t ia_config, uint8_t invalid_prio, uint8_t port_flood_mask, uint32_t ialk_id_min,  uint32_t ialk_id_max, uint32_t ialk_id_sub ) {
  R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
#if 1
  p_reg_ethsw->IDISC_VLAN4 = port_flood_mask << 16 | invalid_prio << 4 | ia_config; // IALK_ID_CONFIG = port_flood_mask << 16 | invalid_prio << 4 | ia_config;
  p_reg_ethsw->IDISC_UNTAGGED3 = ialk_id_min; //  IALK_ID_MIN = ialk_id_min;
  p_reg_ethsw->IDISC_BLOCKED3  = ialk_id_max; // IALK_ID_MAX = ialk_id_max;
  p_reg_ethsw->ODISC4 = ialk_id_sub; // IALK_ID_SUB = ialk_id_sub;
#else
  p_reg_ethsw-> IALK_ID_CONFIG = port_flood_mask << 16 | invalid_prio << 4 | ia_config;
    p_reg_ethsw-> IALK_ID_MIN = ialk_id_min;
    p_reg_ethsw-> IALK_ID_MAX = ialk_id_max;
    p_reg_ethsw-> IALK_ID_SUB = ialk_id_sub;
#endif
  
}
/*  Enable the IA FDB Must be done after confuguration 

cut_through_ena_mask:   enable cut through on the port set within the mask
  */ 
void ia_fdb_enable(uint8_t port_ena_mask, uint8_t cut_through_ena_mask) {
  R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
    
  //p_reg_ethsw-> IALK_CONTROL = cut_through_ena_mask << 16 | port_ena_mask;
  p_reg_ethsw->ODISC3 = cut_through_ena_mask << 16 | port_ena_mask;
  
}


/*  Configures the VLAN setting of the IA FDB 
vlan_config: Configuration of fdb using the defines above
priority:  Priority to use for found for an ID outside the defined range 
vlans_amount: amount of enabled vlans, must be a power of two, maximum 8

vlan_flood_mask:  ports to which fraes with invalid VLAN should be forwarded  
Note: Learning is not implemented
*/ 
void ia_fdb_vlan_config(uint8_t vlan_config, uint8_t unknwn_vlan_prio, uint8_t vlans_amount, uint8_t vlan_flood_mask, struct ialk_vlanid_struct *ialk_vlanid_p ) {
  R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
  struct ialk_vlanid_struct *ialk_vlanid_t;  
  
  p_reg_ethsw-> IALK_VLAN_CONFIG = vlan_flood_mask << 16 | vlans_amount << 8 | unknwn_vlan_prio << 4 | vlan_config;
  
  ialk_vlanid_t = ialk_vlanid_p; 
  
  int i; 
  for (i= 0; i< vlans_amount; i++) {
    p_reg_ethsw-> IALK_VLANID[i] = ialk_vlanid_t->vlanid_prio_vld << 31 | ialk_vlanid_t->vlanid_prio << 28 | ialk_vlanid_t->vlanid_flood_mask << 16 | ialk_vlanid_t->vlanid_ena << 12 | ialk_vlanid_t->vlanid;  
    *ialk_vlanid_t++;
  }
}

uint8_t ia_fdb_get_vlan_width () {
  R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
  
  uint8_t vlans_amount; 

 // vlans_amount = (p_reg_ethsw-> IALK_VLAN_CONFIG >> 8) & 0x0f;
  vlans_amount = (p_reg_ethsw-> IDISC_UNTAGGED4 >> 8) & 0x0f;
  if (vlans_amount == 8) 
    return (3); 
  else if (vlans_amount == 4) 
    return (2);
  else if (vlans_amount == 2) 
    return (1);
  else if (vlans_amount == 1) 
    return (0);  
  else if (vlans_amount == 0) 
    return (0);
  else return (0xff); 
  
}

/* Gets the index for the requested VLANID from the VLANID table. If the VLAN table is empty , it returns 0, if 
   the VLANID is not found it returns 0x0ff  */

uint8_t ia_fdb_get_vlan_index (uint8_t vlanid) {
  R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
  
  int i;
  uint8_t vlans_amount; 
  uint8_t vlanid_width; 

  //vlans_amount = (p_reg_ethsw-> IALK_VLAN_CONFIG >> 8) & 0x0f;
  vlans_amount = (p_reg_ethsw-> IDISC_UNTAGGED4 >> 8) & 0x0f;
  if (vlans_amount == 0 ) return 0;
  /* Get index of reqeusted VLAN in VLAN table */
  for (i=0; i< vlans_amount; i++) {
    if ((p_reg_ethsw-> IALK_VLANID[i] & 0x0fff) == vlanid)  return (i);
  }
  return (0xff);   
}


/* modify  a stream in the fdb table  
The FDB table address is a combination of the lower three octets of the MAC and the index of the VLANID in the VLAn table. Depending of the amount of vlans in the 
vlanid table there are 3, 2, 1 or 0 bits in the FDB table address. 
The VLANID table must be initialized before adding values to the FDB table
 */
uint8_t ia_fdb_mod(uint32_t streamid, uint16_t vlanid, uint8_t portmap, uint8_t enable ) {
  R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
  uint8_t vlan_index = 0;

  /* Check if vlan_ref is programmed in the list of vlanids and get index */

  uint8_t vlan_width = ia_fdb_get_vlan_width ();
  if (vlan_width) {  
    vlan_index = ia_fdb_get_vlan_index (vlanid);
    if (vlan_index == 0xff) {
      return (0xff);    // VLANID not found
    }
  }
  // Store stream, vlanid and portmap
  uint32_t address = (streamid << vlan_width) | vlan_index;
  p_reg_ethsw-> IALK_TBL_ADDR =  address;
  p_reg_ethsw-> IALK_TBL_DATA = portmap << 1 | enable;    // Enable/Disable stream and store portmap
  return(1);  
}


/* Read out a value in the ia_fdb table, returns the stored value, i. e. the portmap : enable  
*/

uint8_t ia_fdb_read(uint32_t streamid, uint8_t vlanid) {
  R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;

  uint8_t vlans_amount; 
  uint8_t vlanid_width; 
  uint8_t vlan_index; 

  vlan_index = ia_fdb_get_vlan_index (vlanid);
  p_reg_ethsw-> IALK_TBL_ADDR = streamid << ia_fdb_get_vlan_width () | vlan_index; 
  return (  p_reg_ethsw-> IALK_TBL_DATA); 
  
}

/* Read out a value in the ia_fdb table, returns the stored value, i. e. the portmap : enable  
*/

uint8_t ia_fdb_read_idx(uint32_t streamid, uint8_t vlanidx) {
  R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;

  uint8_t vlans_amount; 
  uint8_t vlanid_width; 
  uint8_t vlan_index; 

  p_reg_ethsw-> IALK_TBL_ADDR = streamid << ia_fdb_get_vlan_width () | vlanidx; 
  return (  p_reg_ethsw-> IALK_TBL_DATA); 
  
}

#define PORTMAP 1
#define TAGMAP 0


/* Reads a value from the vlan table, 
offset: Adress in vlan table
portmap: if portmap is 1 it reads the portmap, otherwise the tagmap  */
uint32_t ethsw_vlan_table_read(int offset, bool portmap ) {
        R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
        uint32_t val;
                val = p_reg_ethsw->VLAN_RES_TABLE[offset];
		if ((portmap && (val & ETHSW_VLAN_TAG_READ)) || ((!portmap) && (~(val & ETHSW_VLAN_TAG_READ)))){
                        p_reg_ethsw->VLAN_RES_TABLE[offset] |=  ETHSW_VLAN_TAG_READ;
                        val = p_reg_ethsw->VLAN_RES_TABLE[offset];
		}	
		return (val);
}


/* Generally enables VLAN on this port,  i. e. block it for all frames if enable== true, 
otherwise remove port rom all VLANs, i. e. enable reception for this port. 
To use VLAN the port must be enabled for this specific VLANs later */
void ethsw_enable_port_vlan( int port, bool enable ) {
  R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
  int i; 
  uint32_t val;

	for (i=0; i < ETHSW_VLAN_ID_TABLE_DEPTH; i++) {
		val = ethsw_vlan_table_read(i, PORTMAP );
	 	if (enable) val = val & ~(1 << (port));
		else val = val | (1 << (port));
                p_reg_ethsw->VLAN_RES_TABLE[i] = ETHSW_VLAN_PORT_WRITE | val;
	}
        val = p_reg_ethsw->VLAN_IN_MODE_ENA; 
	if (enable)
            p_reg_ethsw->VLAN_IN_MODE_ENA = (0x01 << port) | val;
	else 
            p_reg_ethsw->VLAN_IN_MODE_ENA = ~(0x01 << port) & val;

        val = p_reg_ethsw->VLAN_OUT_MODE; 
        if (enable)
            p_reg_ethsw->VLAN_OUT_MODE = (0x3 << (port*2)) | val;
	else 
            p_reg_ethsw->VLAN_OUT_MODE = ~(0x3 << (port*2)) & val;
		
	return ;
}


/* Adjust the three mask registers so a egress port can be globally disabled. 
unint8_t port:  Port number for the respective port that should be adjusted
bool mask:   If 0, port is disabled, if 1 port is enabled to transmit *cast frames from other ports
uint8_t cast_type: Mask for the three types of cast   0x04: Broadcast, 0x02 Multicast, 0x01 Unicast
*/

void ethsw_port_mask_adjust( uint8_t port, uint8_t cast_type, bool enable ) {
        R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
	uint32_t val;
	if (!enable) {
          if (cast_type & 0x01) {
		val = (p_reg_ethsw->UCAST_DEFAULT_MASK) & (~(1 << port));
		p_reg_ethsw->UCAST_DEFAULT_MASK = val;  // disable Unicast default
          }
          if (cast_type & 0x02) {
		val = (p_reg_ethsw->MCAST_DEFAULT_MASK) & (~(1 << port));
		p_reg_ethsw->MCAST_DEFAULT_MASK = val;  // disable Multicast default
          }

          if (cast_type & 0x04) {
		val = (p_reg_ethsw->BCAST_DEFAULT_MASK) & (~(1 << port));
		p_reg_ethsw->BCAST_DEFAULT_MASK = val;  // disable Broadcast default
          }
	}
	else {
          if (cast_type & 0x01) {
                val = (p_reg_ethsw->UCAST_DEFAULT_MASK)  | (1 << port);
		p_reg_ethsw->UCAST_DEFAULT_MASK = val;  // enable Unicast default
          }

          if (cast_type & 0x02) {
                val = (p_reg_ethsw->MCAST_DEFAULT_MASK)  | (1 << port);
		p_reg_ethsw->MCAST_DEFAULT_MASK = val;  // enable Multicast default
          }

          if (cast_type & 0x04) {
                val = (p_reg_ethsw->BCAST_DEFAULT_MASK)  | (1 << port);
		p_reg_ethsw->BCAST_DEFAULT_MASK = val;  // enable Broadcast default
          }
	}
}


/* Enable/Disable VLAN filtering for port to block/free all traffic
bool vlan_filtering: If 1 the port will block all frames unless spcifically enabled for the respective VLAN
*/
int ethsw_vlan_filtering(int port, bool vlan_filtering)
{
        R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
	uint32_t vlan_verify; 

        vlan_verify = p_reg_ethsw->VLAN_VERIFY; 
        
        if (vlan_filtering) {
                /* The port is being kept as VLAN-unaware port when bridge is
                 * set up with vlan_filtering not being set, Otherwise, the
                 * port and the corresponding CPU port is required the setup
                 * for becoming a VLAN-aware port.
                 */
		vlan_verify = (1 << port) | (1<< (port+16)) | vlan_verify;
                p_reg_ethsw->VLAN_VERIFY = vlan_verify;
		ethsw_enable_port_vlan( port, 1 );
		return 0;
        }
        p_reg_ethsw->VLAN_VERIFY =  ~((1 << port) | (1<<( port+16))) & vlan_verify;  // disable VLAN verification and dropping of other frames
	ethsw_enable_port_vlan( port, 0 );

        return 0;
}


/* Scans the VID Table for the given vid, returns offset if found, 
otherwise it returns a number > ETHSW_VLAN_ID_TABLE_DEPTH indicating the first empty space by ading ETHSW_VLAN_TABLE_DEPTH to the first empty address*/
int ethsw_vlan_find_vid(int vid ) {
        R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
int i; 
uint32_t val;
bool empty_addr_found = 0;
uint16_t empty_addr = 0;    // Adress of first empty vid
uint16_t vid_read = 0;

	for (i=0; i < ETHSW_VLAN_ID_TABLE_DEPTH; i++) {
                val = p_reg_ethsw->VLAN_RES_TABLE[i];
		vid_read = (val & ETHSW_VLAN_ID_MASK) >> ETHSW_VLAN_ID_OFFSET;
		if (vid_read == vid)
			return (i);
		if ((!empty_addr_found) && (vid_read == (ETHSW_VLAN_ID_MASK >> ETHSW_VLAN_ID_OFFSET))) {      /* Found first empty line */
			empty_addr = i; 
	       		empty_addr_found = 1;	
		}
	}
	return (empty_addr + ETHSW_VLAN_ID_TABLE_DEPTH);
}

/* Add the port to the respective VLAN 
int port: Portnumber
bool pvid:   If 1 VID will also be set for the port as a private VID. In this case no range is allowed.
bool untagged: if 1, each frame entering the port will get the PVID added, when leaving the VID is removed
bool discard:   if 1, frames with unmatching VID are discared.
vid_start ... vid_end:  A range can be set to filter incomming frames. Is also set, when the PVID is set

*/
int ethsw_port_vlan_add(int port, bool pvid, bool untagged, bool discard, int vid_begin, int vid_end)
{
        R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
        uint16_t vid;
	uint16_t vid_addr;
	uint32_t val; 

	bool empty = 0;     // vid not found in table
	
	if (pvid) {
        	if (vid_begin != vid_end) {
       	       		return (-1);      // No range possible for pvid
		}
        	p_reg_ethsw->SYSTEM_TAGINFO[port] = vid_begin  ;  // write pvid for this port 
                p_reg_ethsw->VLAN_IN_MODE_ENA |= (1<<port);       // Enable input processing
                p_reg_ethsw->VLAN_IN_MODE |= (0<<(port*2));    // Add VID on input
                p_reg_ethsw->VLAN_OUT_MODE |= (1<<(port*2));    // Strip VID on exit
        }
        
        p_reg_ethsw->VLAN_VERIFY |= (1<<port);   // Forward only frame with matching VID    
        if (discard) 
            p_reg_ethsw->VLAN_VERIFY |= (1<<(port+16));   // Discard frames without matching VID / PVID    
        else
            p_reg_ethsw->VLAN_VERIFY &= ~(1<<(port+16));   // Do not discard frames without matching VID / PVID    
          
	for (vid = vid_begin; vid <= vid_end; ++vid) {
		vid_addr = ethsw_vlan_find_vid( vid ); 
		if (vid_addr > ETHSW_VLAN_ID_TABLE_DEPTH-1) {
			empty = 1;
			vid_addr -= ETHSW_VLAN_ID_TABLE_DEPTH;
		}
		val = ethsw_vlan_table_read(vid_addr, TAGMAP ) & ~(ETHSW_VLAN_TAG_READ);
		if (untagged) {
			val &= ~(0x01 << port);
		}
		else {
			val |= (0x01 << port);
		}
      		p_reg_ethsw->VLAN_RES_TABLE[vid_addr]  = val | ETHSW_VLAN_TAG_WRITE;  

		val = ethsw_vlan_table_read(vid_addr, PORTMAP );
		if (empty) 
			val = (vid << ETHSW_VLAN_ID_OFFSET) | (0x01 << port);
		else
			val |= (0x01 << port);
                p_reg_ethsw->VLAN_RES_TABLE[vid_addr]  = val | ETHSW_VLAN_PORT_WRITE;  
	}

	return (1);
}


/* Remove the port from the listed VLANs 
Returns:
-1 if wrong range
-2: VLAn entry not found
0: Success */
int ethsw_port_vlan_del(int port, bool pvid, bool untagged, bool discard, int vid_begin, int vid_end)
{
        R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
        uint16_t vid;
        uint16_t vid_addr;
        uint32_t val;



       if (pvid && (vid_begin != vid_end)) {
               return -1;      // No range possible for pvid
        }

        if (pvid) {
                p_reg_ethsw->SYSTEM_TAGINFO[port] = 0;
        }
       
        for (vid = vid_begin; vid <= vid_end; ++vid) {
                vid_addr = ethsw_vlan_find_vid( vid );
                if (vid_addr > ETHSW_VLAN_ID_TABLE_DEPTH-1) {
			return -2;    // vid not found
                }
                val = ethsw_vlan_table_read(vid_addr, TAGMAP ) & ~(ETHSW_VLAN_TAG_READ);
                val &= ~(0x01 << port);
                p_reg_ethsw->VLAN_RES_TABLE[vid_addr]  = val | ETHSW_VLAN_TAG_WRITE;  

                val = ethsw_vlan_table_read(vid_addr, PORTMAP );
                val &= ~(0x01 << port);
                p_reg_ethsw->VLAN_RES_TABLE[vid_addr]  = val | ETHSW_VLAN_PORT_WRITE;
        }
	return 0;
}


 /*     Maps the VLAN Priority to the queues. 
        unit8_t port;           Input port number 
	uint8_t	num_tc;             Number of queues/traffic classes, max. 8 
	uint8_t	prio_tc_map[TC_QOPT_BITMASK + 1];   Priority mapping to queue starting with VID 0 up to VID 8 
        Needs to be extended for the other priorization options the switch offers*/
void ethsw_setup_mqprio(uint8_t port, uint8_t num_tc, uint8_t prio_tc_map[TC_QOPT_BITMASK])
{
        R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
        uint32_t val = 0;
        int i;

        for (i=TC_QOPT_BITMASK; i > 0; i--) {
          val = (val << 3) | (prio_tc_map[i-1] & 0x07); 
        }
        p_reg_ethsw->VLAN_PRIORITY[port] = val;
        p_reg_ethsw->PRIORITY_CFG[port] |= 0x01;     // enable VLAN Priority resolution
        
        return;
}
#endif

 /*     Control the gates of the queues for egress ports. 
        unit8_t port_mask;            
	uint16_t	queue_gate;  2 Bit Values for each gate, can be
                    GATE_HOLD : 00 Hold current state
                    GATE_TOGGLE : 01 toggle current state
                    GATE_CLOSE : 10 Close queue
                    GATE_OPEN : 11 Open Queue

        */
void ethsw_setup_qgates(uint8_t port_mask, uint16_t queue_gate )
{
        R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;

        p_reg_ethsw->MMCTL_QGATE = queue_gate << 16 | port_mask;
        
        return;
}

#define ETHSW_MMCTL_QUEUE_MAX 8
#define QUEUE_GATE_CLOSE 0x2
void ethsw_queue_flush(uint8_t port, uint8_t queue, uint8_t action)
{
    R_ETHSW_Type *p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
    uint32_t queue_gate = 0;
    uint32_t reg_val;
    
    reg_val  = (uint32_t)(port   << R_ETHSW_MMCTL_QFLUSH_PORT_MASK_Pos);
    reg_val |= (uint32_t)(queue  << R_ETHSW_MMCTL_QFLUSH_QUEUE_MASK_Pos);
    reg_val |= (uint32_t)(action << R_ETHSW_MMCTL_QFLUSH_ACTION_Pos);
    p_reg_ethsw->MMCTL_QFLUSH = reg_val;

    for(uint8_t i=0; i<ETHSW_MMCTL_QUEUE_MAX; i++)
    {
        if((1 << i) & queue)
        {
            queue_gate |= (QUEUE_GATE_CLOSE << (i * 2));
        }
    }
    
    reg_val  = (uint32_t)(port       << R_ETHSW_MMCTL_QGATE_PORT_MASK_Pos);
    reg_val |= (uint32_t)(queue_gate << R_ETHSW_MMCTL_QGATE_QUEUE_GATE_Pos);
    p_reg_ethsw->MMCTL_QGATE = reg_val;
}

#if 0

 /*     Control the Frame Preemption 802.3BR 
        unit8_t port_mask      Force a preempt request for the respecive output port by software             
        unit8_t queue_mask     Selection for queues that are peemptable 
        unit8_t close_preempt_mask     Close the queues when gate close and preempt preemptable frames when still transmitting even if no higher prio fra,e
        unit8_t command     Combination of PREEMPT defines as requeired. If PREEMPT_ENA is 0 preemption is completely disabled and all bits reset  
        uint8_t minfragsize64    Minimum Fragment size in increments of 64 Bytes, 0 corresponds to 64 Bytes

	uint16_t	queue_gate;  2 Bit Values for each gate, can be
                    GATE_HOLD : 00 Hold current state
                    GATE_TOGGLE : 01 toggle current state
                    GATE_CLOSE : 10 Close queue
                    GATE_OPEN : 11 Open Queue
        Note: Cut Through Mode must be enabled to make preemption work
       Note: Make sure the ethsw_taprio_init() is run somewhere to initialize the timer. 
        */
void ethsw_setup_frame_preemption(uint8_t port_mask, uint8_t queue_mask, uint8_t close_preempt_mask, uint8_t minfragsize64, uint32_t command)
{
        R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;

        
         p_reg_ethsw->MMCTL_PREEMPT_QUEUES = close_preempt_mask << 8 | queue_mask;  // Select the queues that hold preemptable traffic
        //p_reg_ethsw->MMCTL_HOLD_CTRL = port_mask;  // Force a preempt request  by software
        if (port_mask & 0x01) { 
          if (command & 0x01) {
            p_reg_ethsw->BR_CONTROL_P0 =  0x0a << 8 | minfragsize64 << 4 | command;   // Enable Preemption
            p_reg_ethsw->MMCTL_CTFL_P0_3_ENA |=  queue_mask;   // Enable Cut Through Frame Length Table, this is needed to do the frame length measurement for frame preemption   
          }
          else 
            p_reg_ethsw->BR_CONTROL_P0 =  0; 
        }
        if (port_mask & 0x02) {  
          if (command & 0x01) {
            p_reg_ethsw->BR_CONTROL_P1 =  0x0a << 8 |  minfragsize64 << 4 | command;   // Enable Preemption
            p_reg_ethsw->MMCTL_CTFL_P0_3_ENA |=  (queue_mask << 8);   // Enable Cut Through Frame Length Table, this is needed to do the frame length measurement for frame preemption   
          }
          else 
            p_reg_ethsw->BR_CONTROL_P1 =  0; 
        }
        if (port_mask & 0x04) {  
          if (command & 0x01) {
            p_reg_ethsw->BR_CONTROL_P2 =  0x0a << 8 | minfragsize64 << 4 | command;   // Enable Preemption
            p_reg_ethsw->MMCTL_CTFL_P0_3_ENA |=  (queue_mask << 16);   // Enable Cut Through Frame Length Table, this is needed to do the frame length measurement for frame preemption   
          }
          else 
            p_reg_ethsw->BR_CONTROL_P2 =  0; 
        }
// Currently Port 3 (Management port)  does not support frame preemption
        //        if (port_mask & 0x08) {  
//          p_reg_ethsw->BR_CONTROL_P3 =  0x0a << 8 | minfragsize64 << 4 | verify_dis << 1 | 0x01;   // Enable Preemption
//          p_reg_ethsw->MMCTL_CTFL_P0_3_ENA |=  (queue_mask << 24);   // Enable Cut Through Frame Length Table, this is needed to do the frame length measurement for frame preemption   
//        }
        return;
}

/* Read the current priorisaton scheme for debugging */
uint32_t ethsw_read_mqprio(uint8_t port)
{
        R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;

        return (p_reg_ethsw->VLAN_PRIORITY[port]);
}
#endif

/* Enable previously defined patterns based on portmask and pattern. 
*/
void ethsw_enable_pattern_matcher(uint8_t portmask, uint32_t pattern_mask )
{
        R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
        int i;
        uint32_t *rxmatch_config;
        rxmatch_config = (uint32_t*) &(p_reg_ethsw->RXMATCH_CONFIG[0]);
        
        p_reg_ethsw->PATTERN_IRQ_STAT_ACK = 0xffff;    // Clear all old interrupt bits
          
        for (i = 0; i< ETHSW_NR_PORTS; i++) {
          if ((0x01 << i) & portmask) 
            *(rxmatch_config+i) |= pattern_mask;   // Enable pattern match for this pattern for this port
        }
}

/* Disable previously defined patterns based on portmask and pattern. 

*/
void ethsw_disable_pattern_matcher(uint8_t portmask, uint32_t pattern_mask )
{
        R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
        int i;
        uint32_t *rxmatch_config;
        rxmatch_config = (uint32_t*) &(p_reg_ethsw->RXMATCH_CONFIG[0]);
        
        p_reg_ethsw->PATTERN_IRQ_STAT_ACK = 0xffff;    // Clear all old interrupt bits
          
        for (i = 0; i< ETHSW_NR_PORTS; i++) {
          if ((0x01 << i) & portmask) 
            *(rxmatch_config+i) &= ~pattern_mask;   // Disable pattern match for this pattern for this port
        }
}

void ethsw_read_pattern_matcher(uint8_t pattern_num, uint32_t *ptrn_cmp)
{
    R_ETHSW_Type *p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
    p_reg_ethsw->PATTERN_SEL = pattern_num;

    *ptrn_cmp = p_reg_ethsw->PTRN_CMP_30;
    ptrn_cmp++;
    *ptrn_cmp = p_reg_ethsw->PTRN_CMP_74;
    ptrn_cmp++;
    *ptrn_cmp = p_reg_ethsw->PTRN_CMP_118;
}

 /*     add a Pattern matcher logic to the queues. 
        unit8_t portmask;           Input port mask, set to 1 for each port that this should be enabled for  
	uint8_t	num_pattern;             Number of pattern  (one out of 12 available pattern comparators, if 0xff do not write pattern)  
	uint32_t	action;   action to be done on pattern match can be any of :
#define  MATCH_NOT 0x01      // Invert decision
#define  MGMTFWD 0x02      // Forward frame only to management port
#define  DISCARD 0x04      // Discard frame if match
#define  SET_PRIO 0x08     // Set priority to value in priority field
#define  MODE_12BYTE 0x00          // Set mode to 12byte legacy match
#define  MODE_2BYTE_OFFSET 0x10          // Set mode to 2byte table lookup
#define  MODE_2BYTE_RANGE 0x20  // Set mode to 2byte offset compare
#define  MODE_2BYTE_RANGE_NOT 0x30  // Same as 2BYTE_RAGE but inverted 
#define FORCE_FORWARD    0x80    // Force forwarding to port in port
#define VLAN_SKIP  0x800         // Skip VLAN tag in comparison if VLAN tag is detected. 
#define MATCH_LT  (0x01 << 25)   //  Match Length and Type field in frame with value in length_type. 
#define LEARNING_DIS  0x8000     // Disable learning for this frame          
     uint8_t priority;        Changed value for the new priority/queue 
        uint8_t port_forward_mask     Destination port if FORCE_FORWARD is set, set to 1 for each port that this should be enabled for
        uint32_t *ptrn_cmp       Pointer to array containing the compare value. Can be 4-Words for MODE_12BYTE or 8 for MODE_2BYTE_OFFSET, unused for RANGE modes
        uint32_t *ptrn_msk,      Pointer to array containing the mask value. Can be 4-Words for MODE_12BYTE, the lower 16 bits of a Word for MODE_2BYTE_OFSET or  MODE_2BYTE_RANGE,
        uint32_t length_type,    Length and Type Value in Frame if comparison enabled by MATCH_LT flag. 
        uint8_t offset,          Offset in bytes for start of cmparison for MODE_2BYTE_OFFSET and MODE_2BYTE_RANGE
uint16_t and_mask,               AND mask for combining multiple pattern matcher (match only if pattern n and pattern m and pattern x match).
uint32_t max_min                 Minumum and maximum range value for comparison of two bytes in MODE:2BYTE_RANGE
uint8_t timer                    Timer to use for the TIMER_SEL_OVR function

*/
          
void ethsw_setup_mqprio_pattern_add(uint8_t portmask, uint8_t pattern_num, uint32_t action, uint8_t priority, uint8_t port_forward_mask, 
                                    uint32_t *ptrn_cmp, uint32_t *ptrn_msk, 
                                    uint16_t length_type, uint8_t offset, uint16_t and_mask, uint32_t max_min, uint8_t timer, uint8_t queuesel  )
{
        R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
        int i;
        uint32_t *rxmatch_config;
        rxmatch_config = (uint32_t*) &(p_reg_ethsw->RXMATCH_CONFIG[0]);
        
        for (i = 0; i< ETHSW_NR_PORTS; i++) {
          if ((0x01 << i) & portmask) 
            *(rxmatch_config+i) |= (0x01 << pattern_num) ;   // Enable pattern match for this pattern for this port
        }

        uint32_t *patternctrlp; 
        patternctrlp = (uint32_t *) & ( p_reg_ethsw->PATTERN_CTRL[0]);
        
        *(patternctrlp + pattern_num) = action | (priority << 12) | (port_forward_mask << 16) | (timer << 26) | (queuesel << 28);
        
        if (pattern_num == 0xff) 
            return;
            
        p_reg_ethsw->PATTERN_SEL = pattern_num;
        if ((action & 0x30) == MODE_12BYTE) { 
          p_reg_ethsw->PTRN_CMP_30 = *ptrn_cmp++;
          p_reg_ethsw->PTRN_CMP_74 = *ptrn_cmp++;
          p_reg_ethsw->PTRN_CMP_118 = *ptrn_cmp;
          p_reg_ethsw->PTRN_MSK_30 = *ptrn_msk++;
          p_reg_ethsw->PTRN_MSK_74 = *ptrn_msk++;
          p_reg_ethsw->PTRN_MSK_118 = *ptrn_msk++;
        }
        else if ((action & 0x30) == MODE_2BYTE_OFFSET) {
          p_reg_ethsw->PTRN_CMP_30 = *ptrn_cmp++;
          p_reg_ethsw->PTRN_CMP_74 = *ptrn_cmp++;
          p_reg_ethsw->PTRN_CMP_118 = *ptrn_cmp++;
          p_reg_ethsw->PTRN_MSK_30 = (*ptrn_msk & 0x0000ffff ) | (length_type << 16);
          p_reg_ethsw->PTRN_MSK_74 = (and_mask << 16)  | offset;
          p_reg_ethsw->PTRN_MSK_118 = *ptrn_cmp++;
          
        }
        else if (((action & 0x30) == MODE_2BYTE_RANGE) || ((action & 0x30) == MODE_2BYTE_RANGE_NOT)) {
          p_reg_ethsw->PTRN_CMP_30 = max_min;
          p_reg_ethsw->PTRN_MSK_30 = (*ptrn_msk & 0x0000ffff) | (length_type << 16);
          p_reg_ethsw->PTRN_MSK_74 = (and_mask << 16)  | offset;
        }
        
        return;
}
//#endif


void ETHSW_ResetTimer(uint8_t timer) 
{
    R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;

    if (timer == ETHSW_TIMER0) {
        p_reg_ethsw->ATIME_INC0  = 0x0808;    // Increment value per clock
        p_reg_ethsw->ATIME_SEC0  = 0x0;     // Reset timer
        p_reg_ethsw->ATIME0      = 0x0;     // Restet timer
        p_reg_ethsw->ATIME_CTRL0 = 0x01;    // Enable timer
    }
    else 
    {
        p_reg_ethsw->ATIME_EVT_PERIOD1 = 0x0; // 32bit freerun timer
        p_reg_ethsw->ATIME_SEC1 = 0x0;      // Reset timer
        p_reg_ethsw->ATIME_INC1 = 0x0808;     // Increment value per clock
        p_reg_ethsw->ATIME1 = 0x0;          // Restet timer
        p_reg_ethsw->ATIME_CTRL1 = 0x01;    // Enable timer
    }
}

void ETHSW_EnableTimer(uint8_t timer)
{
    R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
    
    if (timer == 0) {
        p_reg_ethsw->ATIME_CTRL0 |= ((1<<9)|(1));    // Enable timer
    }
    else 
    {
        p_reg_ethsw->ATIME_CTRL1 |= ((1<<9)|(1));    // Enable timer
    }  
}

/*  Init enables the timer. Should be replaced later by the timesync setup   */
void ethsw_taprio_init (uint8_t timer)
{
    R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;

    if (timer == ETHSW_TIMER0)
    {
        p_reg_ethsw->ATIME_INC0  = PTPCLK_CYCLE | (PTPCLK_CYCLE<<8);    // Increment value per clock 
        p_reg_ethsw->ATIME_CTRL0 = 0x01;    // Enable timer, 
    }
    else if (timer == ETHSW_TIMER1)
    {
        p_reg_ethsw->ATIME_INC1  = PTPCLK_CYCLE | (PTPCLK_CYCLE<<8);    // Increment value per clock 
        p_reg_ethsw->ATIME_CTRL1 = 0x01;    // Enable timer
    }
}

uint32_t ethsw_get_time(uint8_t timer) 
{
    R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;

    if (timer == ETHSW_TIMER0)
    {
        p_reg_ethsw->ATIME_CTRL0 |= 0x01 << 11 | 0x01;      // Enable timer, latch timer value
        while (p_reg_ethsw->ATIME_CTRL0 & 0x01 << 11) {}    // Wait for latch ready
        return( p_reg_ethsw->ATIME0);                       // timer 0 nanosecond
    }
    else if (timer == ETHSW_TIMER1) 
    {
        p_reg_ethsw->ATIME_CTRL1 = 0x01 << 11 | 0x01;       // Enable timer, latch timer value
        while (p_reg_ethsw->ATIME_CTRL1 & 0x01 << 11) {}    // Wait for latch ready
        return( p_reg_ethsw->ATIME1);                       // timer 1 nanosecond
    }
    else
    {
        return(0);
    }
}

// use only after calling ethsw_get_time to fetch sec value
uint32_t ethsw_get_time_sec(uint8_t timer)
{
    R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;

    if (timer == ETHSW_TIMER0)
    {
        return( p_reg_ethsw->ATIME_SEC0);                 // timer 0 osecond
    }
    else if (timer == ETHSW_TIMER1) {
        return( p_reg_ethsw->ATIME_SEC1);                 // timer 0 osecond
    }
    else
    {
        return(0);
    }
}

/* This function sets up a basic scheduleing scheme 
port:   Port mask for which the time aware transmission should be done
enable:  1 to enable, 0 to disable (not implemented)
timer_sel: Number of the timer to use
base_time: Start time in ns when scheme should start, 
cycle_time: in ns. 
num_entries: Amount of scheduling entries, maximum 8
*entries: Pointer to struct array of scheduling entries
reset: for the initialization reset should be used. It stops/restarts the timer.
Note that the Priority scheme must be enabled first, the default is not sufficient, it ust be explicitely enabled.
returns the address of the first valid entry
There are three situations under which the scheduler needs to restart: 
1. New start after hardware reset: newstart is set, the TDMA controller is started at the next time for cycle start
2. Restart of TDMA controller after timer set due to new timer sync. In this case the configuration stays, only ethsw_tdma_restart() needs to be called
3. New values for cycle_time, fso, green phase etc. . Restart must be synced to ongoing cycle, should wait for end of cycle with all actions. 

 */

uint32_t ethsw_taprio_qopt_set (uint8_t                       portmask,
                                uint8_t                       enable,
                                uint8_t                       timer_sel,
                                uint32_t                      base_time,
                                uint32_t                      cycle_time,
                                uint32_t                      num_phases,                                
                                uint8_t                       num_entries,
                                struct taprio_sched_entries * entries, 
                                uint8_t                       reset)
{
    struct taprio_sched_entries * entry; 
    int i;
    uint32_t startaddress = 0;    // First valid entry for THIS page
    uint32_t start        = 1;
    bool newstart = 1; 
    uint32_t curr_time_sec;
    uint32_t interval_start;
    R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;

    if (p_reg_ethsw->TDMA_CONFIG_b.TDMA_ENA)   // If already running we need to use mirror table and switch over at the end. 
    {
      newstart = 0; 
      startaddress = (p_reg_ethsw->TCV_SEQ_LAST & 0x3ff) + p_reg_ethsw->TDMA_TCV_START + 1;
      if (startaddress > TDMA_TCV_MAX_DEPTH - num_entries)
      {
        startaddress = 0; 
      }
    }
    entry = entries;
    
    for (i=0; i<num_entries; i++)
    {
        p_reg_ethsw->TCV_SEQ_ADDR = startaddress + i;   // Write address in sequencer, no auto increment
        if (i>0) start = 0;
        p_reg_ethsw->TCV_SEQ_CTRL = (entry->gpio        << 22)| // GPIO
                                    ((startaddress + i) << 2) | // TCV_DATA_INDEX
                                    start;                      // START

        p_reg_ethsw->TCV_D_ADDR   = startaddress + i;           // Write address in TCV_Data table,
        p_reg_ethsw->TCV_D_OFFSET = entry->interval; 
        p_reg_ethsw->TCV_D_CTRL   = (entry->port_mask     << 16)|   // MASK[3:0]
                                    (entry->gate_mask     << 8) |   // QGATE[7:0]
                                    (entry->hold_req      << 7) |   // HOLD_REQ
                                    (entry->gate_mode     << 6) |   // GATE_MODE
                                    (entry->trigger_mode  << 5) |   // TRIGGER_MODE
                                    (entry->in_ct_enable  << 4) |   // IN_CT_ENA
                                    (entry->out_ct_enable << 3) |   // OUT_CT_ENA
                                    (entry->red_phase     << 2) |   // RED_PERIOD
                                    (entry->inc_1         << 1) |   // INC_CTR1
                                    (entry->inc_0         << 0) ;   // INC_CTR0
        entry = ++entries;
    }

    p_reg_ethsw->TDMA_ENA_CTRL = (portmask & 0x0b) | (0x01<<(16+4));   // Enable TDMA mode for ports 0, 1 and 3, 2 is usually not used (only as mirror port)
                                                                     // Disable control for queue 4 
    p_reg_ethsw->TDMA_MODULO   = 1000000000;        // Wrap around at 1 sec / 1000000000ns 

    if (newstart == 1) 
    {
      p_reg_ethsw->TDMA_CONFIG_b.TIMER_SEL = timer_sel;//((timer_sel & 0x03) << 2);   // Set the timer
      uint32_t curr_time = ethsw_get_time(ETHSW_TIMER0);     // Fetch current time
      curr_time_sec = ethsw_get_time_sec(ETHSW_TIMER0);
      if (curr_time_sec & 0x01) {    // Adjust for next seconds value
        curr_time += 1000000000; 
      }
      interval_start = curr_time - (curr_time % (cycle_time * num_phases));    // elspsed time since last cycle start 
      interval_start += (cycle_time * num_phases);  // Set to next interval start
      if (interval_start > 1000000000) 
      {
        interval_start -= 1000000000; 
      }
      p_reg_ethsw->TDMA_TCV_START = startaddress;
      p_reg_ethsw->TCV_SEQ_LAST   = num_entries - 1;
      p_reg_ethsw->TDMA_CYCLE    = cycle_time * num_phases;
      p_reg_ethsw->TDMA_START = interval_start;        // Starttime at next next interval start + adju
      p_reg_ethsw->TDMA_CONFIG_b.TDMA_ENA = 1 ;
      tdma_update_data.update_tdma = 0;
      // Start Phase counter
      p_reg_ethsw->TDMA_CTR1 = 0x100 | (((num_phases-1) << 16));    // Overflow at NumPhases 
      pps_out(ETHSW_TIMER0, curr_time_sec, interval_start, cycle_time * num_phases);
    }
    else 
    {
      tdma_update_data.cycle_time = cycle_time;    // Prepar switchover in red phase
      tdma_update_data.start_address = startaddress; 
      tdma_update_data.num_phases = num_phases;       
      tdma_update_data.end_address = num_entries - 1; 
      tdma_update_data.update_tdma = 1; 
    }
    
    return(startaddress);
}


// Restarts the TDMA controller at the beginning of the next cycle to syncronize after a change of TDMA config
void ethsw_tdma_restart ()
{
    R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
    uint32_t cycle_time;

    if(p_reg_ethsw->TDMA_CONFIG_b.TDMA_ENA)
    {
        p_reg_ethsw->TDMA_CONFIG_b.TDMA_ENA = 0 ;      // Disable TDMA Controller
        p_reg_ethsw->MMCTL_QFLUSH = 0x03FF000F;        // Flush EtherSwitch Queue
    }

    
    cycle_time = p_reg_ethsw->TDMA_CYCLE;      // get actual cycle_time.
    uint32_t cycle_start = ethsw_get_time(ETHSW_TIMER0);     // Fetch current time
    uint32_t cycle_start_sec = ethsw_get_time_sec(ETHSW_TIMER0);
    cycle_start += 1000;      // Assume later time so we do not restart just ns before action
    cycle_start = cycle_start - (cycle_start % cycle_time);    //  
    cycle_start += cycle_time;                         // Set to next cycle start 
    p_reg_ethsw->TDMA_START = cycle_start;        // Starttime at next next cycle start + adjust

    p_reg_ethsw->TDMA_CTR0_b.TDMA_CTR0 = 0;
    p_reg_ethsw->TDMA_CONFIG_b.TDMA_ENA = 1 ;      // Enable and Now start the whole thing    
    ptp_out(ETHSW_TIMER0, cycle_start_sec, cycle_start, cycle_time, 0x0);
}



/* Reads the current schedule for debugging  Needs an array of entries to store the data 
returns the number of found entries 
Note: The number f entries is derived from the last_entry register which is 0 when there is one entry. Thus if there is only one or zero entries 
this number can be wrong */
uint32_t  ethsw_taprio_qopt_get (struct taprio_sched_entries *entries)
{
    uint32_t num_entries;
    uint32_t tcv_d_idx;
    int      i;
    R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
    struct taprio_sched_entries *entry; 

    num_entries = (p_reg_ethsw->TCV_SEQ_LAST & 0x00ff) + 1; 
    entry = entries;

    for (i=0; i<num_entries; i++)
    {
        p_reg_ethsw->TCV_SEQ_ADDR = i;    // Write address in sequencer, no auto increment
        p_reg_ethsw->TCV_SEQ_CTRL = (uint32_t) (0x01 << 31) ;   // Read | TCV_DATA_INDEX  
        tcv_d_idx =  (p_reg_ethsw->TCV_SEQ_CTRL & 0x000003fc) >> 2;

        p_reg_ethsw->TCV_D_ADDR = tcv_d_idx;         // Read address in TCV_Data table, no auto increment
        entry->interval  = p_reg_ethsw->TCV_D_OFFSET;
        entry->gate_mask = p_reg_ethsw->TCV_D_CTRL;  // Enable Gateing for the ports in portmask for the gates in gatemask and enable 
        entry = ++entries;
    }

    return(num_entries);
}

uint32_t ethsw_tda_counter_value_get()
{
    R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
    return (uint32_t)(p_reg_ethsw->TDMA_CTR0);
}

/*  Set up a cnfiguration for Profinet IRT in switch mode 
From the example in MTIP switch spec. 

Four queues, queues 2+3 used for IRT traffic  RTC2, queue s0+1for all other traffic. 

Three periods: RED for isochronous traffic, GREEN forn non isochronous traffic in Cut Through, Yellow guard band,  
1. Enable pattern matcher to identify RTC3 Frames  (Ethertype 0x8892 and FrameD between StartOfredFrameID and EndOfredFrameID)
2. Set up scheduler to define transmitting periods
3. Set up default broadcast port mask BCAST_DEFAULT, UCAST_DEFAULT and MCAST_DEFAULT
4. Set up FDB table
5. Adjust Rate Limiter for Broadcast Frames
6. set up redundancy 
7. Forward 
 Messages (add bridge delay ) 
8. NRT with VLAN 4...7  ?



What about PTP Messages ?
LLDP Pakets in highest Prio Queue. Same with MRP frames

DCP Protocol in 
NRT  has VLAN Priority between 4 and 7



*/

/* Initialize PTP out signal (PPS)*/
void pps_out(uint8_t timer, uint32_t start_sec, uint32_t start, uint32_t period)
{

    R_ETHSS_Type *p_reg_access = (void *)R_ETHSS;
    R_ETHSW_PTP_Type *p_ptp_reg_access = (void *)R_ETHSW_PTP;

    // unlock write to cfg register
    if (p_reg_access->PRCMD != 1)
    {
        p_reg_access->PRCMD = (uint32_t)0x00A5;
        p_reg_access->PRCMD = (uint32_t)1;
        p_reg_access->PRCMD = (uint32_t)0x0fffe;
        p_reg_access->PRCMD = (uint32_t)1;
    }

    // Enable output on pins
    p_reg_access->PTPMCTRL = 0x00010000 | timer; // Set to 0
    p_ptp_reg_access->SWTM[0].EN = 0x0;          // Disable for reconfig first
    p_ptp_reg_access->SWTM[0].WTH = 40000 / 8;   // 40 us pulsewidth
    p_ptp_reg_access->SWTM[0].PSEC = 0x01;       // Pulse period sec
    p_ptp_reg_access->SWTM[0].PNS = period;      // Pulse period ns -> default 1s
    p_ptp_reg_access->SWTM[0].STSEC = start_sec; // Pulse Start time in seconds (set to 0)
    p_ptp_reg_access->SWTM[0].STNS = start;      // Pulse Start time in nanoseconds
    p_ptp_reg_access->SWTM[0].EN = 0x01;         // Enable Pulse generation

    p_ptp_reg_access->SWPTPOUTSEL |= 0x01; // Use TDMA_GPIO4 as output
    // lock
    p_reg_access->PRCMD = (uint32_t)0;
}

/* Configure PTP signal 
timer: timer to use, can be timer 0 or timer 1
start_sec: starttime seconds
start_ns: starttime nanoseconds
period: period in ns
number: which of the 4 outputs to use 
Note: period must ve devidable by 10
period must be a full fraction of ATIME_EVT_PERIOD (which is typically set to 1000000000 ns = 1s
period must be > width (80 ns) 
*/

void ptp_out(uint8_t timer, uint32_t start_sec, uint32_t start_ns, uint32_t period, uint8_t number) {

    R_ETHSS_Type *p_reg_access = (void *)R_ETHSS;
    R_ETHSW_PTP_Type *p_ptp_reg_access = (void *)R_ETHSW_PTP;
#if 0 
    p_reg_access->PRCMD.LONG = (uint32_t) 0x0;
    // unlock write to cfg register
//    while (p_reg_access->PRCMD != 1) {
      p_reg_access->PRCMD = (uint32_t) 0x000000A5;
      p_reg_access->PRCMD = (uint32_t) 0x00000001;
      p_reg_access->PRCMD = (uint32_t) 0x0000fffe;
      p_reg_access->PRCMD = (uint32_t) 0x00000001;
//    }
#endif
     
     R_ETHER_ACCESS_Protection (p_reg_access, R_ETHER_ACCES_PROTECT_UNLOCK);
     
    p_reg_access->PTPMCTRL = 0x00010000 | timer;       //  
    if (number == 0) {
      p_ptp_reg_access->SWTM[0].EN = 0x0;       // Disable for reconfig first 
      p_ptp_reg_access->SWTM[0].WTH =   10000;   // 80 ns pulsewidth  (10 clocks) 
      p_ptp_reg_access->SWTM[0].PSEC = 0x0;   // Pulse period sec
      p_ptp_reg_access->SWTM[0].PNS = period;   // Pulse period ns     
      p_ptp_reg_access->SWTM[0].STSEC = start_sec;   // Pulse Start time in seconds (set to 0)
      p_ptp_reg_access->SWTM[0].STNS = start_ns;   // Pulse Start time in nanoseconds 
      p_ptp_reg_access->SWTM[0].MAXP = 1000000000;   // Overflow value of timer     
      p_ptp_reg_access->SWTM[0].EN = 0x01;    // Enable Pulse generation
    }
    else if (number == 1) {
      p_ptp_reg_access->SWTM[1].EN = 0x0;       // Disable for reconfig first 
      p_ptp_reg_access->SWTM[1].WTH =   80;   // 80 ns pulsewidth 
      p_ptp_reg_access->SWTM[1].PSEC = 0x0;   // Pulse period sec
      p_ptp_reg_access->SWTM[1].PNS = period;   // Pulse period ns -> default 1s    
      p_ptp_reg_access->SWTM[1].STSEC = start_sec;   // Pulse Start time in seconds (set to 0)
      p_ptp_reg_access->SWTM[1].STNS = start_ns;   // Pulse Start time in nanoseconds 
      p_ptp_reg_access->SWTM[1].MAXP = 0;   // Overflow value of timer 
      p_ptp_reg_access->SWTM[1].EN = 0x01;    // Enable Pulse generation
    }
    else if (number == 2) {
      p_ptp_reg_access->SWTM[2].EN = 0x0;       // Disable for reconfig first 
      p_ptp_reg_access->SWTM[2].WTH =   80;   // 80 ns pulsewidth 
      p_ptp_reg_access->SWTM[2].PSEC = 0x0;   // Pulse period sec
      p_ptp_reg_access->SWTM[2].PNS = period;   // Pulse period ns -> default 1s    
      p_ptp_reg_access->SWTM[2].STSEC = start_sec;   // Pulse Start time in seconds (set to 0)
      p_ptp_reg_access->SWTM[2].STNS = start_ns;   // Pulse Start time in nanoseconds 
      p_ptp_reg_access->SWTM[2].MAXP = 0;   // Overflow value of timer 
      p_ptp_reg_access->SWTM[2].EN = 0x01;    // Enable Pulse generation
    }
    else if (number == 3) {
      p_ptp_reg_access->SWTM[3].EN = 0x0;       // Disable for reconfig first 
      p_ptp_reg_access->SWTM[3].WTH =   80;   // 80 ns pulsewidth 
      p_ptp_reg_access->SWTM[3].PSEC = 0x0;   // Pulse period sec
      p_ptp_reg_access->SWTM[3].PNS = period;   // Pulse period ns -> default 1s    
      p_ptp_reg_access->SWTM[3].STSEC = start_sec;   // Pulse Start time in seconds (set to 0)
      p_ptp_reg_access->SWTM[3].STNS = start_ns;   // Pulse Start time in nanoseconds 
      p_ptp_reg_access->SWTM[3].MAXP = 0;   // Overflow value of timer 
      p_ptp_reg_access->SWTM[3].EN = 0x01;    // Enable Pulse generation
    }
     R_ETHER_ACCESS_Protection (p_reg_access, R_ETHER_ACCES_PROTECT_LOCK);
    
}


/***********************************************************************************************************************
 * Function Name: ether_switch_enable_ptp0_icu
 * Description  :
 * Arguments    : p_instance_ctrl -
 *                    pointer to control structure
 * Return Value : none
 ***********************************************************************************************************************/
void ether_switch_enable_ptp0_icu(void)
{
    R_BSP_IrqDetectTypeSet(ETHSW_PTPOUT0_IRQn, 1); // Set the int to rising edge detection
    R_BSP_IrqCfgEnable(ETHSW_PTPOUT0_IRQn, DEFAULT_INT_PRIORITY, NULL);
}

uint32_t testcount = 0; 
uint32_t test_sec = 0; 
uint32_t test_ns = 0; 
extern int64_t TimetStamp; 

/***********************************************************************************************************************
 * Function Name: ether_switch_ptp_out0
 * Description  : Interrupt handler for Timer PTP Out signal 
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
// void ether_switch_ptpout0_intr(void) {


// }

/***********************************************************************************************************************
 * Function Name: ether_switch_enable_tdma0_icu
 * Description  :  Enable TDMA0 interrupt
 * Arguments    : p_instance_ctrl -
 *                    pointer to control structure
 * Return Value : none
 ***********************************************************************************************************************/
//static void ether_switch_enable_ptp0_icu (ether_switch_instance_ctrl_t * const p_instance_ctrl)
void ether_switch_enable_tdma0_icu (void)
{
    /** Configure the Ethernet Switch interrupt. */
/*    R_BSP_IrqCfgEnable(p_instance_ctrl->p_switch_cfg->irq,
                         p_instance_ctrl->p_switch_cfg->interrupt_priority,
                         p_instance_ctrl);  */

    R_BSP_IrqDetectTypeSet (ETHSW_TDMAOUT0_IRQn, 1);   // Set the int to rising edge detection
    R_BSP_IrqCfgEnable(ETHSW_TDMAOUT0_IRQn, DEFAULT_INT_PRIORITY, NULL);
}
/* End of function ether_switch_enable_ptp0_icu() */

#ifndef RZT2_RSK_PNS
uint8_t red_delay_error = 0;
uint32_t red_count = 0;
uint32_t status_red = 0;
uint32_t g_CyclCnt = 0U;
/***********************************************************************************************************************
 * Function Name: ether_switch_ptp_out0
 * Description  : Interrupt handler for Timer TDMA Out signal 
 * Arguments    : none
 * Return Value : none
 * This interrupt is used in Profinet to trigger before (by GMAC_DELAY) the start of the RED2 period 
   It will then start the transmission of the red frames, which are located in fixed positions, so they 
   need not be copied ore changed. 
   Note: There is a delay between the opening of the queue at the beginning of the RED2 period and the time the 
         frames are actually transmitted by  1240ns. So frames are always transmitted late by this amount. 
 ***********************************************************************************************************************/
void ether_switch_tdmaout0_intr(void)
{
	// ether_switch_callback_args_t callback_arg;

	//   R_BSP_IrqClearPending(IRQ_ETHSW_TDMA0_INTR);

	ether_instance_ctrl_t * p_instance_ctrl =  g_ether0.p_ctrl;
	ether_switch_instance_ctrl_t *ptSwCtrl = g_ether0.p_cfg->p_ether_switch_instance->p_ctrl;

	R_ETHSW_Type * p_reg_switch = (R_ETHSW_Type *)ptSwCtrl->p_reg_switch;
	volatile  R_GMAC_Type *p_reg_etherc = (R_GMAC_Type *) p_instance_ctrl->p_reg_etherc;
	p_instance_ctrl->p_tx_descriptor_base = p_instance_ctrl->p_tx_descriptor_red;
	/* Initialize Transmit Descriptor List Address to clear previous active descriptor in GMAC*/
	p_reg_etherc->Transmit_Descriptor_List_Address_b.TDESLA_32bit = (uint32_t) p_instance_ctrl->p_tx_descriptor_base >> 2;

      p_reg_etherc->Operation_Mode_b.ST    = 1;     /* (Re-)Start TX */
    if( (0U < EDDP_LL_ISR_RedStart(g_CyclCnt) ) &&
    	(0U < p_instance_ctrl->p_tx_descriptor_red->des1) &&
    	(0U < p_instance_ctrl->p_tx_descriptor_red->des2) )
    {
		status_red = p_reg_etherc->Status;
		red_count++;
		// Update Descriptor
		if (p_instance_ctrl->p_tx_descriptor_base->des0 & ETHER_TDES0_OWN)
		{
			red_delay_error++;
		//return;
		}   // descriptor not released by DMA

	     // disables transmission of red frames for testing
		p_instance_ctrl->p_tx_descriptor_base->des3 = (uint32_t) p_instance_ctrl->p_tx_descriptor_red;  // Address of next descriptor, now red , changed by interrupts
		p_instance_ctrl->p_tx_descriptor_base->des0 |= ETHER_TDES0_OWN;
		p_instance_ctrl->p_tx_descriptor_red->des0 |= ETHER_TDES0_OWN;

		// Trigger transmission
		__asm("dmb");
		p_reg_etherc->Transmit_Poll_Demand = 0;    /* */
    }
    
    g_CyclCnt++;

#if 0                // disable test and time measurements
     uint32_t starttime = ethsw_get_time(0); 
     uint32_t startqueueclosed = p_reg_switch->MMCTL_QCLOSED_STATUS_P0_3;
     uint32_t startqueuebusy = p_reg_switch->MMCTL_P0_3_QUEUE_STATUS;
     //    while ((p_reg_switch->TS_FIFO_STATUS & 0x02) == 0) {};   // Wailt fr TS_Valid
     uint32_t queuearrivaltime = 0;
     uint32_t queueopentime = 0; 
     while ((queuearrivaltime == 0) || (queueopentime == 0)) {
        if ((p_reg_switch->MMCTL_P0_3_QUEUE_STATUS != 0) && (queuearrivaltime == 0)) {  //frame to arrive in queue
           queuearrivaltime = ethsw_get_time(0); 
        }
        if (((p_reg_switch->MMCTL_QCLOSED_STATUS_P0_3 & 0x0400) == 0) && (queueopentime == 0)) { // queue to open
            queueopentime = ethsw_get_time(0); 
        } 
     }
     uint8_t err_late = 0;
     if (queuearrivaltime > queueopentime) err_late = 1; 
     uint32_t leave_count = 0;
     while (p_reg_switch->MMCTL_P0_3_QUEUE_STATUS != 0) {    // wait for frame to have left queue
       leave_count++;
     };
     uint32_t queueleavetime = ethsw_get_time(0); 
     uint32_t gmac_delay = queuearrivaltime - starttime;
     int32_t queueopendelay = queueopentime - queuearrivaltime;
     int32_t queuedelay = queueleavetime - queuearrivaltime;

     p_reg_switch->TS_FIFO_READ_CTRL = 0x01;    // Read from Port number 1
     while ((p_reg_switch->TS_FIFO_READ_CTRL & 0x010) == 0) {};   // Wailt for TS_Valid
     uint32_t txtime = p_reg_switch->TS_FIFO_READ_TIME_STAMP; 
     uint32_t diff = txtime - starttime; 
     int32_t txdelay = txtime - queueopentime;
     int32_t resttime = txtime - queuearrivaltime;
     uint32_t tdma_load_next =  p_reg_switch->TDMA_LOAD_NEXT; // Time of next green phase start    
     uint32_t tx_time_target = tdma_load_next - (GREEN_START_INIT - FOS_INIT);
     int32_t tx_error = txtime - tx_time_target; 
#endif 
}
#endif

/***********************************************************************************************************************
 * Function Name: ether_switch_enable_tdma1_icu
 * Description  :
 * Arguments    : p_instance_ctrl -
 *                    pointer to control structure
 * Return Value : none
 ***********************************************************************************************************************/
//static void ether_switch_enable_ptp0_icu (ether_switch_instance_ctrl_t * const p_instance_ctrl)
void ether_switch_enable_tdma1_icu (void)
{
    /** Configure the Ethernet Switch interrupt. */
/*    R_BSP_IrqCfgEnable(p_instance_ctrl->p_switch_cfg->irq,
                       p_instance_ctrl->p_switch_cfg->interrupt_priority,
                       p_instance_ctrl);  */
     R_BSP_IrqDetectTypeSet (ETHSW_TDMAOUT1_IRQn, 1);   // Set the int to rising edge detection
      R_BSP_IrqCfgEnable(ETHSW_TDMAOUT1_IRQn, DEFAULT_INT_PRIORITY, NULL);
}                                      /* End of function ether_switch_enable_ptp0_icu() */
#ifndef RZT2_RSK_PNS
uint32_t green_delay_error = 0;
uint32_t green_count = 0;
uint32_t test_count = 0;
uint32_t test_count2 = 0;
uint32_t lastdes0 = 0;
uint32_t status_green = 0;
uint32_t trans_green = 0;
uint32_t difftimecnt = 0;
uint8_t num_frames = 0;

 uint32_t starttime;
 uint32_t endtime;
 uint32_t diff2time;

/***********************************************************************************************************************
 * Function Name: ether_switch_tdmaout1_intr
 * Description  : Interrupt handler for Timer TDMA Out 1 signal 
 * Arguments    : none
 * Return Value : none
  * This interrupt is used in Profinet to trigger before (by GMAC_DELAY) the start of the Green period 
    It will then start the transmission of the green frames. 
 ***********************************************************************************************************************/
void ether_switch_tdmaout1_intr(void) {
  
   // ether_switch_callback_args_t callback_arg;
   // uint32_t                     ulInt_Stat_Ack_Val;
    R_ETHSW_Type          *p_reg_switch;
    volatile  R_GMAC_Type *p_reg_etherc;
    uint32_t test = 0;

    //p_reg_switch = (R_ETHSW_Type *)p_instance_ctrl->p_reg_switch;
    ether_instance_ctrl_t * p_instance_ctrl =  g_ether0.p_ctrl;

     ether_instance_descriptor_t * p_ether_first_used = p_instance_ctrl->p_tx_descriptor;    // Initialize to current address (first empty)

     // ether_switch_instance_ctrl_t *ptSwCtrl = g_ether0.p_cfg->p_ether_switch_instance->p_ctrl;

    //disable trasmission of green frames for testing
    //   R_BSP_IrqClearPending(IRQ_ETHSW_TDMA0_INTR);
    //p_reg_switch = (R_ETHSW_Type *)p_instance_ctrl->p_reg_switch;
    p_reg_etherc = (R_GMAC_Type *) p_instance_ctrl->p_reg_etherc;
    // Check if Link is up
    status_green = p_reg_etherc->Status; 

    EDDP_LL_ISR_GreenStart(g_CyclCnt);

 //   if (p_instance_ctrl->link_establish_status[1] == ETHER_LINK_ESTABLISH_STATUS_DOWN) return;
    num_frames = 0;
    green_count++;

    // Check if we have control over base Descriptor (we should or something went terriby wrong)
    if ((p_instance_ctrl->p_tx_descriptor_base->des0) & ETHER_TDES0_OWN)
    {
       green_delay_error++;  
       //return;    // We do not have control over the DMa controller of the GMAC, give up.
    };       // Wail t until descriptor released by DMA


    // Find first used descriptor in green chain
    // Reduce one from current empty descriptor to point to last probably used
    if (p_ether_first_used == p_instance_ctrl->p_ether_cfg->p_tx_descriptors)
    {  // at bottom, need to turn around to reduce further
         p_ether_first_used = p_instance_ctrl->p_ether_cfg->p_tx_descriptor_top;  // Set to top
    }
    else
    {
       p_ether_first_used--;
    }

    lastdes0 = p_ether_first_used->des0;

    test = ETHER_TDES0_OWN & (p_ether_first_used->des0);

    p_ether_first_used->des0 |= ETHER_TDES0_TER;    // Set End of Ring bit on last used descriptor, so DMA will go back to base descriptor for next round

    while ( (p_ether_first_used->des0 & ETHER_TDES0_OWN) && (num_frames < NUM_TX_DESCRIPTOR) )
    {   // Count down until not ready for transmission
      num_frames++;
      if (p_ether_first_used == p_instance_ctrl->p_ether_cfg->p_tx_descriptors)
      {  // reached bottom, need to turn around to reduce further
         p_ether_first_used = p_instance_ctrl->p_ether_cfg->p_tx_descriptor_top;  // Set to top
       }
       else
       {
          p_ether_first_used--;
          if (p_ether_first_used == p_instance_ctrl->p_tx_descriptor) return;                // Looped around once, nothing there
       }
     }

     // We are now at the first empty just below the first used, now increment by one to reach the first busy one
     if (p_ether_first_used == p_instance_ctrl->p_ether_cfg->p_tx_descriptor_top) {  // we are at the top       
       p_ether_first_used = p_instance_ctrl->p_ether_cfg->p_tx_descriptors;                  // set to base des
     }
     else {
       p_ether_first_used++;   // return back to first valid
     }

    if (!((p_ether_first_used->des0) & ETHER_TDES0_OWN)) {   // Busy flag was deleted while in this function
       test_count2++;
       return;
    }
     
     p_instance_ctrl->p_tx_descriptor_base->des3 = (uint32_t) p_ether_first_used;  // Address of next available open green descriptor  
     p_instance_ctrl->p_tx_descriptor_base->des0 |= ETHER_TDES0_OWN;
    trans_green++;

    // Trigger transmission
    __asm("dmb");
     p_reg_etherc->Transmit_Poll_Demand = 0;    /* */

#if 0
     starttime = ethsw_get_time(0); 
  //   while(p_instance_ctrl->p_tx_descriptor_base->des0 & 0x80000000) {};   // This is not always working
     endtime = ethsw_get_time(0); 
     diff2time = endtime-starttime; 
     if ((diff2time > 2000) & (diff2time < 100000)) {
        difftimecnt++;
        return;
     }
#endif

}
#endif

/***********************************************************************************************************************
 * Function Name: ether_switch_enable_tdma2_icu
 * Description  :
 * Arguments    : p_instance_ctrl -
 *                    pointer to control structure
 * Return Value : none
 ***********************************************************************************************************************/
//static void ether_switch_enable_ptp0_icu (ether_switch_instance_ctrl_t * const p_instance_ctrl)
void ether_switch_enable_tdma2_icu (void)
{
    /** Configure the Ethernet Switch interrupt. */
/*    R_BSP_IrqCfgEnable(p_instance_ctrl->p_switch_cfg->irq,
                       p_instance_ctrl->p_switch_cfg->interrupt_priority,
                       p_instance_ctrl);  */
     R_BSP_IrqDetectTypeSet (ETHSW_TDMAOUT2_IRQn, 1);   // Set the int to rising edge detection
      R_BSP_IrqCfgEnable(ETHSW_TDMAOUT2_IRQn, DEFAULT_INT_PRIORITY, NULL);
}                                      /* End of function ether_switch_enable_ptp0_icu() */

/***********************************************************************************************************************
 * Function Name: ether_switch_enable_tdma3_icu
 * Description  :
 * Arguments    : p_instance_ctrl -
 *                    pointer to control structure
 * Return Value : none
 ***********************************************************************************************************************/
void ether_switch_enable_tdma3_icu (void)
{
    /** Configure the Ethernet Switch interrupt. */
/*    R_BSP_IrqCfgEnable(p_instance_ctrl->p_switch_cfg->irq,
                       p_instance_ctrl->p_switch_cfg->interrupt_priority,
                       p_instance_ctrl);  */
     R_BSP_IrqDetectTypeSet (ETHSW_TDMAOUT3_IRQn, 1);   // Set the int to rising edge detection
      R_BSP_IrqCfgEnable(ETHSW_TDMAOUT3_IRQn, DEFAULT_INT_PRIORITY, NULL);
}                                      /* End of function ether_switch_enable_ptp0_icu() */


 

uint32_t status_yellow = 0;
uint32_t status_yellow_axi = 0;
   ether_instance_descriptor_t * p_tx_descriptor_active; 
#ifndef RZT2_RSK_PNS
/***********************************************************************************************************************
 * Function Name: ether_switch_tdmaout2_intr
 * Description  : Interrupt handler for Timer TDMA Out 2 signal 
 * Arguments    : none
 * Return Value : none
  * This interrupt is used in Profinet to trigger before (by GMAC_DELAY) the start of the Yellow period 
    It will then start the transmission of remaininng green frames if that can be finished before the start of te red period. 
 ***********************************************************************************************************************/
void ether_switch_tdmaout2_intr(void) {
  
    R_ETHSW_Type               * p_reg_switch;
    ether_instance_ctrl_t *p_instance_ctrl =  g_ether0.p_ctrl;
    volatile  R_GMAC_Type *p_reg_etherc = (R_GMAC_Type *) p_instance_ctrl->p_reg_etherc;

    ether_instance_descriptor_t * p_tx_descriptor_active; 
    // Check if GMAC has ongoing transmissions (should not be the case or something went wrong)
    status_yellow = p_reg_etherc->Status; 

    EDDP_LL_ISR_YellowStart(g_CyclCnt);

  //  if (status_yellow & 0x02) return;     // Transmission was stopped in green phase, all is nice, nothing to do
         
	p_tx_descriptor_active = (ether_instance_descriptor_t *) p_reg_etherc->Current_Host_Transmit_Descriptor;
    if (p_tx_descriptor_active == p_instance_ctrl->p_tx_descriptor_red) return; // Last descriptor anyway
    // We need to stop transmission now so in the upcomming red phase the DMA is free and can immidiately start te transission.
    // This is done by clearing the "OWN" bit in te DES0 descriptor of the next descriptor. 
    // The DMA will then reurn to the base descriptor and wait there for it to be activated in the red phase

    // Increment descriptor by one to next valid
     if (p_tx_descriptor_active == p_instance_ctrl->p_ether_cfg->p_tx_descriptor_top) {  // we are at the top       
       p_tx_descriptor_active = p_instance_ctrl->p_ether_cfg->p_tx_descriptors;                  // set to base des
     }
     else {
       p_tx_descriptor_active++;   // 
     }
    status_yellow_axi = p_reg_etherc->AXI_Status;

    if ((p_tx_descriptor_active->des0 & 0x80000000) != 0x80000000){
      return;   // Next descriptor is empty anyway nothing to be done
    }
    // Now stop ongoing transmission
    	p_reg_etherc->Operation_Mode_b.ST    = 0;     /* Stop TX */

    
   // p_tx_descriptor_active->des0 &= 0x7fffffff;    // Clear next descriptor to interrupt transmission
   // p_instance_ctrl->p_tx_descriptor_drop = p_tx_descriptor_active;
    
    

}
#endif



#ifndef RZT2_RSK_PNS
/***********************************************************************************************************************
 * Function Name: ether_switch_tdmaout2_intr
 * Description  : Interrupt handler for Timer TDMA Out 1 signal 
 * Arguments    : none
 * Return Value : none
  * This interrupt is used in Profinet to trigger before (by GMAC_DELAY) the start of the Yellow period 
    It will then start the transmission of remaininng green frames if that can be finished before the start of te red period. 
 ***********************************************************************************************************************/
void ether_switch_tdmaout3_intr(void) {
  
   // ether_switch_callback_args_t callback_arg;
   // uint32_t                     ulInt_Stat_Ack_Val;
    R_ETHSW_Type               * p_reg_switch;
    //p_reg_switch = (R_ETHSW_Type *)p_instance_ctrl->p_reg_switch;
    ether_instance_ctrl_t * p_instance_ctrl =  g_ether0.p_ctrl;
     // ether_switch_instance_ctrl_t *ptSwCtrl = g_ether0.p_cfg->p_ether_switch_instance->p_ctrl;

}
#endif

// Enble TX Timestamp interrupt  
/***********************************************************************************************************************
 * Function Name: ether_switch_enable_TX_timestamp_icu
 * Description  :
 * Arguments    : portmask   Mask for the port for which timestamps should be taken
 * Return Value : none
 ***********************************************************************************************************************/
void ether_switch_enable_tx_timestamp_icu (uint8_t portmask)
{
    /** Configure the Ethernet Switch interrupt. */
   // R_ETHSW_Type               * p_reg_switch;
  R_ETHSW_Type* p_reg_switch = (void *)R_ETHSW;

    p_reg_switch->TSM_CONFIG |= (0x01 | (portmask << 16));    // Enable TX Timestamp interrupt for these ports
    p_reg_switch->INT_CONFIG |= (1<<29);    // Enable global TSM interrupt 
}

/* Fetch the time of the last rising edge of the PTP out signal 
returns the seconds in the upper 32 bits, the ns in the lower 32 bits  */
uint64_t ptp_latch(){

    R_ETHSW_PTP_Type *p_ptp_reg_access = (void *)R_ETHSW_PTP;
    uint64_t retval = 0; 
    

    retval = ((uint64_t) p_ptp_reg_access->SWTM[0].LATSEC) << 32;
    retval = retval | p_ptp_reg_access->SWTM[0].LATNS; // SWTMLATNS0 --> TODO: SWTMLATSNS0 ????;

    return (retval);
}

#define RTC3_RANGE 0x0FFF0100     //

// define Pattern matcher for RTC Frames. Looks for the first unused paternmatcher of the 16 and uses it.
// Note that this will have higher priority if other pattern matchers are initiated previously.
// Note2: Must be called after the Profinet setup as these patternmatchers are on a fixed location
// Returns the number of the patternmatcher that was used, of none available 0xff.
// Also enables corresponding Patternmatcher interrupt

uint8_t patternmatcher_RTC_int(uint8_t port_mask, uint16_t frameid_range_min, uint16_t frameid_range_max) {
    R_ETHSW_Type               * p_reg_switch;
    __attribute__((unused))ether_instance_ctrl_t * p_instance_ctrl =  g_ether0.p_ctrl;
    ether_switch_instance_ctrl_t *ptSwCtrl = g_ether0.p_cfg->p_ether_switch_instance->p_ctrl;
    p_reg_switch = (R_ETHSW_Type *)ptSwCtrl->p_reg_switch;
    uint32_t action;
    action = MODE_2BYTE_RANGE | MATCH_LT | VLAN_SKIP | SWAP_BYTES;    // 2-Byte Range compare mode,
    uint32_t max_min = frameid_range_max << 16 | frameid_range_min;;     // Frame ID range
    uint32_t length_type = 0x8892;     //  Profinet
    uint32_t offset = 2;                // Check profinet ID after Type field -> two bytes behind source address
    uint32_t pattern_num = 0;       // Initial pattern number
    uint8_t priority = 0;
    uint8_t queuesel = 0;
    uint8_t port_forward_mask = 0;
    uint16_t and_mask = 0;
    uint32_t ptrn_cmp[8] = {0,0,0,0,0,0,0,0};
    uint32_t ptrn_msk[8] = {0xffff,0,0,0,0,0,0,0};
    uint8_t timer = 0;


    // Find first unused patternmatcher
        uint32_t *patternctrlp;
        patternctrlp = (uint32_t *) & ( p_reg_switch->PATTERN_CTRL[0]);

        while (*patternctrlp != 0) {
          pattern_num++;
          if(pattern_num == 16) return(0xff);
          patternctrlp++;
        }

     ethsw_setup_mqprio_pattern_add(port_mask, pattern_num, action,  priority,  port_forward_mask, &ptrn_cmp[0], &ptrn_msk[0],
                                    length_type, offset, and_mask,  max_min, timer , queuesel );


     // enable patternmatcher Interrupt
      p_reg_switch->PATTERN_IRQ_CONTROL|= 1 << pattern_num;    // Enable Int for this Pattern Number
      p_reg_switch->INT_CONFIG_b.PATTERN_INT  = 1;       // Enable global Pattern interrupt

			return 0;
}

 #define R_PORT_SR     ((R_PORT_COMMON_Type *) R_PORT_SR_BASE)
 #define R_PTADR       ((R_PTADR_Type *) R_PTADR_BASE)
 #define R_PORT_SR_BASE     0x81030000UL
 #define R_PTADR_BASE       0x81030C00UL
uint32_t pattern_int_cnt = 0; 
uint32_t pattern_int_val = 0; 
int pin_17_3_val = 0; 
//Interrupt handler for Pattern matcher. Called from General switch interrupt in ether_switch_isr_intr()
void ether_switch_isr_pattern(ether_switch_instance_ctrl_t * p_instance_ctrl)
{
        R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;

        pattern_int_val = p_reg_ethsw->PATTERN_IRQ_STAT_ACK; 
  p_reg_ethsw->PATTERN_IRQ_STAT_ACK = p_reg_ethsw->PATTERN_IRQ_STAT_ACK;  // CLear Interrupt Flag
  // Invert value of Pin 17.3  (connected to CN1 pin 7)
    //uint32_t val =  R_BSP_PinRead((bsp_io_port_pin_t) BSP_IO_PORT_03_PIN_3);
   pattern_int_cnt++; 
#if 0
  R_BSP_PinAccessEnable();

    uint32_t pin_bit = 3;
    uint32_t lvl = pin_17_3_val;
    uint32_t pin  = BSP_IO_PORT_03_PIN_3; 
    uint32_t port_num = (uint32_t) (pin >> 8);
    uint32_t aselp = (((R_PTADR->RSELP[pin >> 8]) >> pin_bit) & 0x00000001UL);
    uint32_t pm_val = R_PORT_SR->PM[port_num]; 
    uint32_t p_val = R_PORT_SR->P[port_num];
    if(0U == aselp)
    {
        R_PORT_SR->PM[port_num] = pm_val | (BSP_IO_PM_OUTPUT << (2U * pin_bit));
        R_PORT_SR->P[port_num] = (p_val & (~(1U << pin_bit))) | (lvl << pin_bit);
    }
    else if(1U == aselp)
    {
        R_PORT_NSR->PM[port_num] = (R_PORT_NSR->PM[port_num]) | (BSP_IO_PM_OUTPUT << (2U * pin_bit));
        R_PORT_NSR->P[port_num] = ((R_PORT_NSR->P[port_num]) & (~(1U << pin_bit))) | (lvl << pin_bit);
    }
    //R_BSP_PinWrite((bsp_io_port_pin_t) BSP_IO_PORT_03_PIN_3, (bsp_io_level_t) pin_17_3_val);
    pin_17_3_val ^= 1; 
    pm_val = R_PORT_SR->PM[port_num]; 
    p_val = R_PORT_SR->P[port_num];
    R_BSP_PinAccessDisable();
#endif
    
  return;
}





 /*     add a Pattern matcher logic to the queues. 
        uint8_t portmask;           Input port mask, set to 1 for each port that this should be enabled for  
	uint8_t	num_pattern;             Number of pattern  (one out of 12 available pattern comparators, if 0xff do not write pattern)  
	uint32_t	action;   action to be done on pattern match can be any of :
#define  MGMTFWD 0x02      // Forward frame only to management port
#define  DISCARD 0x04      // Discard frame if match
#define  SET_PRIO 0x08     // Set priority to value in priority field
#define  MODE_12BYTE 0x00          // Set mode to 12byte legacy match
#define  MODE_2BYTE_OFFSET 0x10     //  Set mode to 2byte offset compare    
#define  MODE_2BYTE_RANGE 0x20  // 
#define  MODE_2BYTE_RANGE_NOT 0x30  // Same as 2BYTE_RANGE but inverted 
#define FORCE_FORWARD    0x80    // Force forwarding to port in port
#define VLAN_SKIP  0x800         // Skip VLAN tag in comparison if VLAN tag is detected. 
#define MATCH_LT  (0x01 << 25)   //  Match Length and Type field in frame with value in length_type. 
#define LEARNING_DIS  0x8000     // Disable learning for this frame          
      uint8_t priority;        Changed value for the new priority/queue 
        uint8_t port_forward_mask     Destination port if FORCE_FORWARD is set, set to 1 for each port that this should be enabled for
        uint32_t *ptrn_cmp       Pointer to array containing the compare value. Can be 4-Words for MODE_12BYTE or 8 for MODE_2BYTE_OFFSET, unused for RANGE modes
        uint32_t *ptrn_msk,      Pointer to array containing the mask value. Can be 4-Words for MODE_12BYTE, the lower 16 bits of a Word for MODE_2BYTE_OFSET or  MODE_2BYTE_RANGE,
        uint32_t length_type,    Length and Type Value in Frame if comparison enabled by MATCH_LT flag. 
        uint8_t offset,          Offset in bytes for start of cmparison for MODE_2BYTE_OFFSET and MODE_2BYTE_RANGE
uint16_t and_mask,               AND mask for combining multiple pattern matcher (match only if pattern n and pattern m and pattern x match).
uint32_t max_min                 Minumum and maximum range value for comparison of two bytes in MODE:2BYTE_RANGE

red2_start: Time when the CPU may inject local RED frames towards the end of the Red Phase. 

*/


void ETHSW_DisableTimer(uint16_t timer_id)
{
    R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;

    if (timer_id == ETHSW_TIMER0) {
      p_reg_ethsw->ATIME_CTRL0 = 0x00;      // Disable timer 0
    }
    else {
      p_reg_ethsw->ATIME_CTRL1 = 0x00;      // Disable timer  1
    }
}

void ETHSW_DisableIrqTdmaAll(void)
{
	R_BSP_IrqDisable(ETHSW_TDMAOUT0_IRQn);
	R_BSP_IrqDisable(ETHSW_TDMAOUT1_IRQn);
	R_BSP_IrqDisable(ETHSW_TDMAOUT2_IRQn);
	R_BSP_IrqDisable(ETHSW_TDMAOUT3_IRQn);

	R_BSP_IrqClearPending(ETHSW_TDMAOUT0_IRQn);
	R_BSP_IrqClearPending(ETHSW_TDMAOUT1_IRQn);
	R_BSP_IrqClearPending(ETHSW_TDMAOUT2_IRQn);
	R_BSP_IrqClearPending(ETHSW_TDMAOUT3_IRQn);
}

void ETHSW_DisableIrq(uint32_t IrqType)
{
	R_BSP_IrqDisable(IrqType);
}

void setup_pattern_matcher(uint8_t portmask) 
{
    R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
  //  uint8_t     port_mask   = 0x03; // Enable for all ports, expecially frames from the management port 3 need to be checked an queued
//    uint8_t     pattern_num = 0;    // Pattern number

    __attribute__((unused))uint8_t     enable      = 1;
    __attribute__((unused))uint8_t     init_setup = 0; 
    uint8_t     timer       = ETHSW_TIMER0;

        /* Pattern matcher to drop frames not received in their respective phases (i. e. IRT in Red phase, NRT in Green/Yellow Phase). 
        As it is not possible to seperate the Green Phase start timing for Port 0 and Port 1 special handling is required: 
          Basically one of the two ports must close the RED phase sooner that the other. In this case the RED phase is set to the longer Period of the two. 
          In this case the frames arriving on the"early" port (i. e. the one which finishes the RED phase sooner) needs to handle the frames manually: 
          - IRT frames are must be dropped in that period. 
          - NRT frames must NOT be dropped. 
        To enable this a special interrupt genrated by the TDMA controller is called at the end of the first shorter RED phase. This interrupt disables the respective Pattern matchers for the respective port and enables 
        special ones to handle this situation. 
        The following actions are done: 
         At end of early Red Phase: 
         - Disable Pattern matcher 0 for the "early" port.   (This filters NRT frames in the RED period so by doing this we disable filrtering of the NRT frames ). 
              This is later reenabled in the Yellow Interrupt. 
         - Disable Pattern matcher 2 for the early port and enable new Pattern matcher 2a for the early port  (Filters IRT in Green phase, 2a is pattern matcher 2 without the MATCH_NOT_RED filter) 
              This is reenabled in the yellow interrupt. 
       */
        
        uint32_t configval = p_reg_ethsw->TDMA_CONFIG;

        /* 1. Match all frames in the not-red period. This pattern matcher will match for all frames in the GREEN
         *    period so they are not discarded when none of the other pattern matchers hit, which would causes
         *    the "match-not" in pattern_matcher0 to discard such frames (special function of pattern matcher pattern 0).
         *    In the RED period this pattern matcher does not hit so only RED frames are allowed and any other is discarded.
         */
        #if 0   // 
        {
            uint32_t    action      = MODE_2BYTE_RANGE | MATCH_NOT_RED | VLAN_SKIP ;
            uint32_t    max_min     = 0xFFFF0000;   // Find Frames with Frame ID
            uint32_t    length_type = 0;
            uint8_t     priority    = 0;
            uint8_t     offset      = 2;

            uint8_t     port_forward_mask = 0;
            uint16_t    and_mask    = 0; 
            uint8_t     queuesel    = 0;

            uint32_t    ptrn_cmp[8] = {         0,0,0,0,0,0,0,0};
            uint32_t    ptrn_msk[8] = {0x0000ffff,0,0,0,0,0,0,0};

            ethsw_setup_mqprio_pattern_add (portmask, PATTERN_ALL_GREEN, action, priority, port_forward_mask, &ptrn_cmp[0], &ptrn_msk[0],
                                            length_type, offset, and_mask,  max_min, timer , queuesel );
        }
        #endif

        
#if 0   
        // This matches all frames in all periods ?
        {
            uint32_t    action      = MODE_2BYTE_RANGE | VLAN_SKIP ;
            uint32_t    max_min     = 0xFFFF0000;   // Find Frames with Frame ID
            uint32_t    length_type = 0;
            uint8_t     priority    = 0;
            uint8_t     offset      = 2;

            uint8_t     port_forward_mask = 0;
            uint16_t    and_mask    = 0; 
            uint8_t     queuesel    = 0;

            uint32_t    ptrn_cmp[8] = {         0,0,0,0,0,0,0,0};
            uint32_t    ptrn_msk[8] = {0x0000ffff,0,0,0,0,0,0,0};

            ethsw_setup_mqprio_pattern_add (portmask, PATTERN_ALL_GREEN_IN_RED, action, priority, port_forward_mask, &ptrn_cmp[0], &ptrn_msk[0],
                                            length_type, offset, and_mask,  max_min, timer , queuesel );
         }
#endif
        
        /* 2. Discard all RTC3 frames received outside the RED period
         */
        #if 1   // 
        {
            uint32_t    action      = MODE_2BYTE_RANGE | MATCH_LT | MATCH_NOT_RED | LEARNING_DIS | SWAP_BYTES | DISCARD;
            uint32_t    max_min     = RTC3_RANGE;   // Find Frames with Frame ID
            uint32_t    length_type = 0x8892;       //  Profinet
            uint8_t     priority    = 0;
            uint8_t     offset      = 2;

            uint32_t    ptrn_cmp[8] = {         0,0,0,0,0,0,0,0};
            uint32_t    ptrn_msk[8] = {0x0000ffff,0,0,0,0,0,0,0};

            uint8_t     port_mask   = 0x03;         // Not enabled for management port as these are transmitted before the RED period
            uint8_t     port_forward_mask = 0;
            uint16_t    and_mask    = 0; 
            uint8_t     queuesel    = 0;

            ethsw_setup_mqprio_pattern_add (port_mask, PATTERN_DISC_IRT_NRED, action, priority, port_forward_mask, &ptrn_cmp[0], &ptrn_msk[0],
                                            length_type, offset, and_mask,  max_min, timer , queuesel );
        }
        #endif

               /* 2a. Discard all RTC3 frames, used for the early green port during the mixed (red/green) phase 
         */
        #if 1
        {
            uint32_t    action      = MODE_2BYTE_RANGE | MATCH_LT | VLAN_SKIP | LEARNING_DIS | SWAP_BYTES | DISCARD;
            uint32_t    max_min     = RTC3_RANGE;   // Find Frames with Frame ID
            uint32_t    length_type = 0x8892;       //  Profinet
            uint8_t     priority    = 0;
            uint8_t     offset      = 2;

            uint32_t    ptrn_cmp[8] = {         0,0,0,0,0,0,0,0};
            uint32_t    ptrn_msk[8] = {0x0000ffff,0,0,0,0,0,0,0};

            uint8_t     port_mask   = 0x0;    // Disabled at start, is enabled in early green Interrupt
            uint8_t     port_forward_mask = 0;
            uint16_t    and_mask    = 0; 
            uint8_t     queuesel    = 0;

            ethsw_setup_mqprio_pattern_add (port_mask, PATTERN_DISC_IRT, action, priority, port_forward_mask, &ptrn_cmp[0], &ptrn_msk[0],
                                            length_type, offset, and_mask,  max_min, timer , queuesel );
        }
        #endif
 
        
/* 3. Accept all RTC3 frames received in the RED period and set the output queue to RED with SET_PRIO
         RTC3 frames are only forwarded to the adjacent port, to receive a special pattern matcher must be used. 
              RTC3 Frames from the management port come before the REDPeriod so they are not covered by this pattern matcher 
         */
#if 1
        {
            uint32_t    action      = MODE_2BYTE_RANGE | MATCH_LT | MATCH_RED | VLAN_SKIP | LEARNING_DIS | SET_PRIO | SWAP_BYTES;
            uint32_t    max_min     = RTC3_RANGE;   // Find Frames with Frame ID
            uint32_t    length_type = 0x8892;       //  Profinet
            uint8_t     priority    = 3;
            uint8_t     offset      = 2;

            uint32_t    ptrn_cmp[8] = {         0,0,0,0,0,0,0,0};
            uint32_t    ptrn_msk[8] = {0x0000ffff,0,0,0,0,0,0,0};

            uint8_t     port_mask   = 0x03;         // Enable for external ports, RED frames from management port re handled further down
            uint8_t     port_forward_mask = 0;
            uint16_t    and_mask    = 0; 
            uint8_t     queuesel    = 0;

            ethsw_setup_mqprio_pattern_add (port_mask, PATTERN_IRT_SET_QUEUE, action, priority, port_forward_mask, &ptrn_cmp[0], &ptrn_msk[0],
                                            length_type, offset, and_mask,  max_min, timer , queuesel );
        }
#endif
        /* 4. Match PTCP frames (basically all profinet frames) for timestamping for timer. 
             Currently omitted as this is already done in software (Management TAG) 
         */
        #if 0
        {
            uint32_t    action      = MODE_2BYTE_RANGE | MATCH_LT | MATCH_NOT_RED | VLAN_SKIP | SWAP_BYTES | TIMER_SEL_OVR;
            uint32_t    max_min     = RTC3_RANGE;   // Find Frames with Frame ID
            uint32_t    length_type = 0x8892;       //  Profinet
            uint8_t     priority    = 0;
            uint8_t     offset      = 2;

            uint32_t    ptrn_cmp[8] = {         0,0,0,0,0,0,0,0};
            uint32_t    ptrn_msk[8] = {0x0000ffff,0,0,0,0,0,0,0};

            uint8_t     port_forward_mask = 0;
            uint16_t    and_mask    = 0; 
            uint8_t     queuesel    = 0;

            ethsw_setup_mqprio_pattern_add (portmask, pattern_num, action, priority, port_forward_mask, &ptrn_cmp[0], &ptrn_msk[0],
                                            length_type, offset, and_mask,  max_min, timer , queuesel );
        }
        #endif
        /*  Delay Response, Delay Request, DelFU frames are mapped to timer 1 (freerunning not synchronized just syntonzied) 
             
         */
#if 1
        {
#define DEL_REXX_RANGE 0xff43ff40
            uint32_t    action      = MODE_2BYTE_RANGE | MATCH_LT | VLAN_SKIP | SWAP_BYTES | TIMER_SEL_OVR;
            uint32_t    max_min     = DEL_REXX_RANGE;   // Find Frames with Frame ID
            uint32_t    length_type = 0x8892;       //  Profinet
            uint8_t     priority    = 0;
            uint8_t     offset      = 2;

            uint32_t    ptrn_cmp[8] = {         0,0,0,0,0,0,0,0};
            uint32_t    ptrn_msk[8] = {0x0000ffff,0,0,0,0,0,0,0};

            uint8_t     port_forward_mask = 0;
            uint16_t    and_mask    = 0; 
            uint8_t     queuesel    = 0;
            uint8_t     port_mask   = 0x03;         // Not enabled for management port as these are transmitted before the RED period

            ethsw_setup_mqprio_pattern_add (port_mask, PATTERN_TIMER_SET_PDELRX, action, priority, port_forward_mask, &ptrn_cmp[0], &ptrn_msk[0],
                                            length_type, offset, and_mask,  max_min, ETHSW_TIMER1 , queuesel );
        }
        #endif
#if 1
        /* 5.  Set the output queue to queue 1 with SET_PRIO for RTC1 frames.   
         */

        {
            uint32_t    action      = MODE_2BYTE_RANGE | MATCH_LT | VLAN_SKIP | LEARNING_DIS | SET_PRIO | SWAP_BYTES;
            uint32_t    max_min     = 0xFBFF8000;
            uint32_t    length_type = 0x8892;
            uint8_t     priority    = 1;
            uint8_t     offset      = 2;

            uint32_t    ptrn_cmp[8] = {         0,0,0,0,0,0,0,0};
            uint32_t    ptrn_msk[8] = {0x0000ffff,0,0,0,0,0,0,0};

            uint8_t     port_mask   = 0x03;
            uint8_t     port_forward_mask = 0;
            uint16_t    and_mask    = 0; 
            uint8_t     queuesel    = 0;

            ethsw_setup_mqprio_pattern_add (port_mask, PATTERN_RT_SET_QUEUE, action, priority, port_forward_mask, &ptrn_cmp[0], &ptrn_msk[0],
                                            length_type, offset, and_mask,  max_min, timer , queuesel );
        }
#endif            
        /* Discard all RTC3 frames received in the RED period */
        {
            uint32_t    action      = MODE_2BYTE_RANGE | MATCH_LT | MATCH_RED | VLAN_SKIP | LEARNING_DIS | SWAP_BYTES | DISCARD;
            uint32_t    max_min     = RTC3_RANGE;   // Find Frames with Frame ID
            uint32_t    length_type = 0x8892;       //  Profinet
            uint8_t     priority    = 0;
            uint8_t     offset      = 2;

            uint32_t    ptrn_cmp[8] = {         0,0,0,0,0,0,0,0};
            uint32_t    ptrn_msk[8] = {0x0000ffff,0,0,0,0,0,0,0};

            uint8_t     port_mask   = 0x0;    // Disabled at start, is enabled in early green Interrupt
            uint8_t     port_forward_mask = 0;
            uint16_t    and_mask    = 0; 
            uint8_t     queuesel    = 0;

            ethsw_setup_mqprio_pattern_add (port_mask, PATTERN_DISC_IRT_RED, action, priority, port_forward_mask, &ptrn_cmp[0], &ptrn_msk[0],
                                            length_type, offset, and_mask,  max_min, timer , queuesel );
        }

    p_reg_ethsw->MMCTL_QFLUSH = 0x01020008; // Queue1 Flush
    p_reg_ethsw->MMCTL_QGATE  = 0x00080008; // Queue1 Close AG Removed, blocks RTC1 frames from reaching the Management port
     // enable patternmatcher Interrupt
      p_reg_ethsw->PATTERN_IRQ_CONTROL|= PATTERN_DISC_IRT_MASK | PATTERN_DISC_IRT_NRED_MASK;    // Enable Int for this Pattern Number
      p_reg_ethsw->INT_CONFIG_b.PATTERN_INT  = 1;       // Enable global Pattern interrupt
}   // setup_pattern_matcher

#if IOD_INCLUDE_STORM_FILTER_ARP_HW
void setup_pattern_matcher_ARP(uint8_t *ip_address)
{
    /* Set ARP frame forwarding */
    {
        uint32_t    action      = MODE_2BYTE_RANGE | FORCE_FORWARD | LEARNING_DIS | SWAP_BYTES | MATCH_LT;
        uint32_t    max_min     = 0xFFFF0000;
        uint32_t    length_type = 0x0806; // ARP
        uint8_t     priority    = 0;
        uint8_t     offset      = 0;

        uint32_t    ptrn_cmp[8] = {         0,0,0,0,0,0,0,0};
        uint32_t    ptrn_msk[8] = {0x0000ffff,0,0,0,0,0,0,0};

        uint8_t     port_mask   = 0x03;
        uint8_t     port_forward_mask = 0x03;
        uint16_t    and_mask    = 0; 
        uint8_t     queuesel    = 0;

        ethsw_setup_mqprio_pattern_add (port_mask, PATTERN_ARP_FORWARD, action, priority, port_forward_mask, &ptrn_cmp[0], &ptrn_msk[0],
                                        length_type, offset, and_mask,  max_min, ETHSW_TIMER0 , queuesel );
    }

    /* Match the upper 16 bits of IP address of ARP frame */
    {
        uint32_t    action      = MGMTFWD | MODE_2BYTE_RANGE | LEARNING_DIS | SWAP_BYTES | MATCH_LT;
        uint16_t    upper_ipadr = ((uint16_t)ip_address[0] << 8) | ip_address[1];
        uint32_t    max_min     = (uint32_t)((((uint32_t)upper_ipadr) << 16) | upper_ipadr);
        uint32_t    length_type = 0x0806; // ARP
        uint8_t     priority    = 0;
        uint8_t     offset      = 26;

        uint32_t    ptrn_cmp[8] = {         0,0,0,0,0,0,0,0};
        uint32_t    ptrn_msk[8] = {0x0000ffff,0,0,0,0,0,0,0};

        uint8_t     port_mask   = 0x03;
        uint8_t     port_forward_mask = 0;
        uint16_t    and_mask    = PATTERN_ARP_UPPER_IPADDRESS_MASK | PATTERN_ARP_LOWER_IPADDRESS_MASK;
        uint8_t     queuesel    = 0;

        ethsw_setup_mqprio_pattern_add (port_mask, PATTERN_ARP_UPPER_IPADDRESS, action, priority, port_forward_mask, &ptrn_cmp[0], &ptrn_msk[0],
                                        length_type, offset, and_mask,  max_min, ETHSW_TIMER0 , queuesel );
    }

    /* Match the lower 16 bits of IP address of ARP frame */
    {
        uint32_t    action      = MGMTFWD | MODE_2BYTE_RANGE | LEARNING_DIS | SWAP_BYTES | MATCH_LT;
        uint16_t    lower_ipadr = ((uint16_t)ip_address[2] << 8) | ip_address[3];
        uint32_t    max_min     = (uint32_t)((((uint32_t)lower_ipadr) << 16) | lower_ipadr);
        uint32_t    length_type = 0x0806; // ARP
        uint8_t     priority    = 0;
        uint8_t     offset      = 28;

        uint32_t    ptrn_cmp[8] = {         0,0,0,0,0,0,0,0};
        uint32_t    ptrn_msk[8] = {0x0000ffff,0,0,0,0,0,0,0};

        uint8_t     port_mask   = 0x03;
        uint8_t     port_forward_mask = 0;
        uint16_t    and_mask    = PATTERN_ARP_UPPER_IPADDRESS_MASK | PATTERN_ARP_LOWER_IPADDRESS_MASK;
        uint8_t     queuesel    = 0;

        ethsw_setup_mqprio_pattern_add (port_mask, PATTERN_ARP_LOWER_IPADDRESS, action, priority, port_forward_mask, &ptrn_cmp[0], &ptrn_msk[0],
                                        length_type, offset, and_mask,  max_min, ETHSW_TIMER0 , queuesel );
    }
}
#endif /* IOD_INCLUDE_STORM_FILTER_ARP_HW */

void setup_pattern_matcher_RT(uint16_t frameId, uint32_t ArIdx) 
{
    uint32_t action            = MODE_2BYTE_OFFSET | VLAN_SKIP | LEARNING_DIS | SWAP_BYTES | MATCH_LT;
    uint32_t max_min           = 0x00000000;
    uint32_t length_type       = 0x8892;
    uint8_t  priority          = 0;
    uint8_t  offset            = 2;
    uint32_t ptrn_cmp[8]       = {         0,0,0,0,0,0,0,0};
    uint32_t ptrn_msk[8]       = {0x0000ffff,0,0,0,0,0,0,0};
    uint8_t  port_mask         = 0x3;
    uint8_t  port_forward_mask = 0x0;
    uint16_t and_mask          = 0;
    uint8_t  queuesel          = 0;

    ethsw_read_pattern_matcher(PATTERN_RTC_DISC_OWN_OUTPUT_CR_RT, &ptrn_cmp[0]);

    uint16_t *p_cmp = (uint16_t *)&ptrn_cmp[0];
    p_cmp += ArIdx;
    *p_cmp = frameId;

    ethsw_setup_mqprio_pattern_add (port_mask, PATTERN_RTC_DISC_OWN_OUTPUT_CR_RT, action, priority, port_forward_mask, &ptrn_cmp[0], &ptrn_msk[0],
                                    length_type, offset, and_mask, max_min, ETHSW_TIMER0, queuesel);
}

void setup_pattern_matcher_IRT_Advanced(uint16_t frameId)
{
    uint32_t action            = MGMTFWD | MODE_2BYTE_RANGE | MATCH_RED | VLAN_SKIP | LEARNING_DIS | SWAP_BYTES | MATCH_LT;
    uint32_t max_min           = (uint32_t)((((uint32_t)frameId) << 16) | frameId);
    uint32_t length_type       = 0x8892;
    uint8_t  priority          = 0;
    uint8_t  offset            = 2;
    uint32_t ptrn_cmp[8]       = {         0,0,0,0,0,0,0,0};
    uint32_t ptrn_msk[8]       = {0x0000ffff,0,0,0,0,0,0,0};
    uint8_t  port_mask         = 0x3;
    uint8_t  port_forward_mask = 0x0;
    uint16_t and_mask          = 0;
    uint8_t  queuesel          = 0;

    ethsw_setup_mqprio_pattern_add (port_mask, PATTERN_RTC_DISC_OWN_OUTPUT_CR_IRT, action, priority, port_forward_mask, &ptrn_cmp[0], &ptrn_msk[0],
                                    length_type, offset, and_mask, max_min, ETHSW_TIMER0, queuesel);
}

void setup_pattern_matcher_IRT_Legacy(uint16_t frameId)
{
    uint32_t action            = MODE_2BYTE_RANGE | MATCH_RED | VLAN_SKIP | LEARNING_DIS | SWAP_BYTES | MATCH_LT;
    uint32_t max_min           = (uint32_t)((((uint32_t)frameId) << 16) | frameId);
    uint32_t length_type       = 0x8892;
    uint8_t  priority          = 0;
    uint8_t  offset            = 2;
    uint32_t ptrn_cmp[8]       = {         0,0,0,0,0,0,0,0};
    uint32_t ptrn_msk[8]       = {0x0000ffff,0,0,0,0,0,0,0};
    uint8_t  port_mask         = 0x3;
    uint8_t  port_forward_mask = 0x0;
    uint16_t and_mask          = 0;
    uint8_t  queuesel          = 0;

    ethsw_setup_mqprio_pattern_add (port_mask, PATTERN_RTC_DISC_OWN_OUTPUT_CR_IRT, action, priority, port_forward_mask, &ptrn_cmp[0], &ptrn_msk[0],
                                    length_type, offset, and_mask, max_min, ETHSW_TIMER0, queuesel);
}

// Configure switch to accept only frames used for Profinet as used by Profinet software
/* Setup is as follows:
    Unicast frames: Matching MAC Adresses are forwarded to port 3 (management port), all others are only forwarded to the other external port
                    The unicast frames mst be added manually to the table this is not done within this module.
    Multicast frames: Multicast frames are blocked for port 3, only dedicated multicast addresses used for Profinet are forwarded to port 3.
    Broadcast adresses: Currently all broadcast frames are forwarded to all ports. There is no filtering
    Unfortunately there is no way to add ranges to the Forwarding Database, so the ranges used by Profinet are addedone by one.
          This takes approx. 1.5 k entries, so only 500 remain.

    */

void profinet_filter_conf(void)
{
    R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
//    int portmask  = 0x0f;
    uint16_t vid = 0;
#if 1
    uint32_t portmask = ( (1 << ETHER_SWITCH_PORT(0)) |
    									  (1 << ETHER_SWITCH_PORT(1)) |
    			//						  (1 << ETHER_SWITCH_PORT(2)) |
    									  (1 << ETHER_SWITCH_PORT(3))) ;
#endif
    /* The switch forwarding is set up such that it will forward all local destigned frames using a static forwarding to port 3, 
        all other unicast frames are only forwarded to ports 0 and 1. Thus the default unicast is only set for ports 0 and 1. 
        Same applies for learning which is only enabled for ports 0 and 1*/
    
    // Set up the static routes for the own MAC addresses
    p_reg_ethsw->MCAST_DEFAULT_MASK0 = 0x03;    // Exclude Port 3 from multicast (except for those listed below)
    p_reg_ethsw->UCAST_DEFAULT_MASK0 = 0x03;    // Exclude Port 3 from unicast (except for those explicitely added to the FDB)
    p_reg_ethsw->INPUT_LEARN_BLOCK = 0x08 << 16;    // Exclude Port 3 from learning
    // Now add the own MAC addresses to the Forwarding database statically. 
    uint8_t macaddr[6]; 
    Bsp_GetPortMacAddr  (&macaddr[0], 0); 
    ethsw_port_fdb_add(0x08, macaddr, vid);   // 
    Bsp_GetPortMacAddr  (&macaddr[0], 1); 
    ethsw_port_fdb_add(0x08, macaddr, vid);   // 
    Bsp_GetMacAddr  (&macaddr[0]); 
    ethsw_port_fdb_add(0x08, macaddr, vid);   // 
    
    // Set up the Frame Forwarding Database. These Muticast IP are statically forwarded to all ports including Port 3
    uint8_t addr[6] = {0x01,0x0e,0x0cf,0,0,0};   // Base address, modified to match the settings for the following filters
    ethsw_port_fdb_add_range(portmask, &addr[0], vid, 2);   // DCP Identify

    addr[4] = 0x01;
    addr[5] = 0x01;
    ethsw_port_fdb_add_range(portmask, &addr[0], vid, 2);   // RT_CLASS_3
    addr[4] = 0x02;
    addr[5] = 0x00;
    ethsw_port_fdb_add_range(portmask, &addr[0], vid, 256);   // RT_CLASS_2
    addr[4] = 0x04;
//    ethsw_port_fdb_add_range(portmask, &addr[0], vid, 64);   // PTCP RTASync, PTCP FollowUp
    ethsw_port_fdb_add_range(0x08, &addr[0], vid, 160);   // PTCP RTASync, PTCP FollowUp only to internal management port
    addr[4] = 0x05;
    addr[5] = 0x00;
    ethsw_port_fdb_add(portmask, &addr[0], vid);   // MRRT
    addr[1] = 0x15;
    addr[2] = 0x4e;
    addr[3] = 0x00;
    addr[4] = 0x00;
    addr[5] = 0x01;
    ethsw_port_fdb_add_range(portmask, &addr[0], vid, 2);   // MC_Test, MC_COntrol
    // 01:80:C2:00:00:00/0E  RSTP, LLDP PTCP Delay are BPDU Management frames and always forwarded, make sure that MGMT_CONFIG is set to 0x043 to enable this
    addr[1] = 0x00;
    addr[2] = 0x5E;
    addr[3] = 0x40;
    addr[4] = 0xF8;
    addr[5] = 0x00;
    ethsw_port_fdb_add_range(portmask, &addr[0], vid, 256);   // RT_CLASS_UDP
    addr[4] = 0xF9;
    addr[5] = 0x00;
    ethsw_port_fdb_add_range(portmask, &addr[0], vid, 256);   // RT_CLASS_UDP
    addr[4] = 0xFA;
    addr[5] = 0x00;
    ethsw_port_fdb_add_range(portmask, &addr[0], vid, 256);   // RT_CLASS_UDP
    addr[4] = 0xFB;
    addr[5] = 0x00;
    ethsw_port_fdb_add_range(portmask, &addr[0], vid, 256);   // RT_CLASS_UDP


}


void ETHSW_InitSwitchTimer(void)
{
//#ifdef RZT2_RSK_SWI_TIMER     // Use timer from switch
#if 0
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_RESET);
    R_BSP_MODULE_START(FSP_IP_ETHER, p_cfg->channel);
    R_BSP_MODULE_START(FSP_IP_GMAC, p_cfg->channel);
    R_BSP_MODULE_START(FSP_IP_ETHSW, p_cfg->channel);
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_RESET);
    R_RWP_NS->PRCRN = 0xA502;
    R_SYSC_NS->MRCTLE_b.MRCTLE00 = 0;     // Release GMAC Reset
    R_SYSC_NS->MRCTLE_b.MRCTLE01 = 0;     // Release GMAC Reset
    R_SYSC_NS->MRCTLE_b.MRCTLE02 = 0;        // Release Ethernet Switch SS Reset
    R_SYSC_NS->MRCTLE_b.MRCTLE05 = 0;     // Release Ethernet SS Reset
    R_SYSC_NS->MRCTLE_b.MRCTLE06 = 0;        // Release MII Converter Reset
    R_RWP_NS->PRCRN = 0xA500;
#endif
/* Enable Ethernet switch Timer 0 and set PTP pulse generation logic to set pulse every 250us. This triggers the interrupt ether_switch_ptp_out0_intr*/

    //ptp_out(uint8_t timer, uint32_t start_sec, uint32_t start_ns, uint32_t period, uint8_t number) {
    __attribute__((unused))uint8_t channel = 0;

    // Set start time to the next start of the Profinet cycle
    uint32_t time_cur = ethsw_get_time (ETHSW_TIMER0);
    uint32_t time_cur_s = ethsw_get_time_sec (ETHSW_TIMER0);
    uint32_t time_remain = PROFINET_CYCLE_TIME_INIT - (time_cur % PROFINET_CYCLE_TIME_INIT);
    if (time_remain < 1000) time_remain += PROFINET_CYCLE_TIME_INIT;
    __attribute__((unused))uint32_t time_start =  time_remain + time_cur;
//    ether_switch_enable_ptp0_icu ();   // for WS2
    ethsw_taprio_init (ETHSW_TIMER0);
    //ptp_out(timer, 0, time_start, PROFINET_CYCLE_TIME_INIT, channel);
    ether_switch_enable_tdma3_icu();   // only WS1
    // Start timer 1 for the measurment and handling of the Cable Delay measurement (i. e. PDelay... fames) 
    // Timer is also run in 1 second overflow mode although 32bit would be possible so functions for timer 0 can be reused
    ethsw_taprio_init (ETHSW_TIMER1);

}


// Initializes structures required for Profinet, i. e. additional descriptors and memory and sets up the scheduler in te switch 
#if 0
void ETHSW_InitDescriptors(void)
{
  ether_instance_ctrl_t * p_instance_ctrl =  g_ether0.p_ctrl;
  R_GMAC_Type *p_reg_etherc = (R_GMAC_Type *) p_instance_ctrl->p_reg_etherc;

  p_instance_ctrl->p_tx_descriptor_base = &tx_descriptor_base;
  p_instance_ctrl->p_tx_descriptor_red = &tx_descriptor_red[0];

  // Initialize base descriptor. This is basically empty, it just points the GMAC to the first descriptor to use, either red_descriptor or standard descriptor
  p_instance_ctrl->p_tx_descriptor_base->des0 = (ETHER_TDES0_TCH);
  p_instance_ctrl->p_tx_descriptor_base->des1 = 0x0;  // As this is only a dummy descriptor Buffer empty, go to next descriptor
  p_instance_ctrl->p_tx_descriptor_base->des2 = 0x0;  // Address of buffer, as this is empty it is 0
  p_instance_ctrl->p_tx_descriptor_base->des3 = (uint32_t) p_instance_ctrl->p_tx_descriptor_red;  // Address of next descriptor, now red , changed by interrupts 
  
  // Initialize red descriptors  Set up in chain mode, currently 1 red frame, each 64 Bytes long
  // Last frame in chain OWN Bit set, FS and LS set (Frame fits in one descriptor) , TCH des2 points to nxt descriptor, TER -> End of ring next descriptor is base
  p_instance_ctrl->p_tx_descriptor_red->des0 = (ETHER_TDES0_LS| ETHER_TDES0_FS| ETHER_TDES0_TCH|ETHER_TDES0_TER);
  p_instance_ctrl->p_tx_descriptor_red->des1 = 0U; // sizeof (ether_frame_rtc3_t);     // Size of RED frame, assumed 64 for the time being.
  p_instance_ctrl->p_tx_descriptor_red->des2 = 0U; //(uint32_t) &ether_tx_frame_pn_rtc3;    //
  //p_instance_ctrl->p_tx_descriptor_red[0].des2 = (uint32_t) & (p_instance_ctrl->p_tx_descriptor_red[1]);    // Pointer to next frame   
  p_instance_ctrl->p_tx_descriptor_red->des3 = 0;     // Pointer to next descriptr (not used as this is the last) . 
  
  /* Initialize Transmit Descriptor List Address */
  p_reg_etherc->Transmit_Descriptor_List_Address_b.TDESLA_32bit =
                (uint32_t) p_instance_ctrl->p_tx_descriptor_base >> 2;

  return;
}
#endif

/*
  Set coarse time Atime0
*/
void clock_setTime(uint32_t sec, uint32_t nano)
{
   R_ETHSW_Type* p_reg_switch = (void *)R_ETHSW;
  
   p_reg_switch->ATIME_CORR0 = 0; // Disable any running correction.
   p_reg_switch->ATIME_SEC0 = sec;
   p_reg_switch->ATIME0 = nano;
}
/*
   adjusts the rate in which time progresses if freq is non-null. 
   The unit of the rate of adjustment is nanoseconds per second.
  interval: Duration of correction, typically before the next sync frame arrives
*/

int clock_adjtime(int diffsign, uint32_t delta, uint32_t interval)
{
   R_ETHSW_Type* p_reg_switch = (void *)R_ETHSW;
   __attribute__((unused))static int lastdiffsign = 0;
  
//    Value to used for performing offset corrections without changing the drift correction. 
//    The value is used in two different ways. 
//    ? If ATIME_OFFS_CORR is zero, the value is immediately added to the current timer 
//    value when this register is written. I.e. the value must be given in true 
//    nanoseconds. When a negative value is given, the value is effectively subtracted. 
//    ? If ATIME_OFFS_CORR is non-zero, the value defines how many offset corrections 
//    will be performed. After each correction, the value is decremented by 1 until it 
//    reaches zero. When it has reached zero no more offset corrections are performed. 
//    The register then is also reset to zero and if enabled an offset event interrupt is 
//    generated. 
//    Note) ATIME_OFFS_CORR must be written prior to writing this register to define the 
//    intended function. 
  
  
  volatile int brkp = 0;
  if (delta == 0) 
      return 0; 
  /* tmp contains distance between additions we need to do */
  uint32_t tmp = PTPCLK / delta;       // Achtung:  usec ist eigentlich ns  Anzahl Takte, nach denen man eine zustzlichen ns addiert
  
  uint32_t clock_dur = 1000000000 / PTPCLK;    
 // if(tmp > 0)
//   p_reg_switch->ATIME_CORR0 = tmp; 
 // else
//   brkp = 1;         // Echter Fehler, kann nicht sein.

  
  
  //Offset Correction
    if(diffsign == 1)
     p_reg_switch->ATIME_INC0 = (p_reg_switch->ATIME_INC0 & 0x0000FFFF)  | 0x00090000;   // Add correction value of +1
    else
     p_reg_switch->ATIME_INC0 = (p_reg_switch->ATIME_INC0 & 0x0000FFFF)  | 0x00070000;   // Add correction value of -1

     __attribute__((unused))uint32_t interval_ms = interval / 1000000;
     tmp = tmp/1000;
//     p_reg_switch->ATIME_OFFS_CORR0 = tmp * (interval_ms-100);     // Space in between, reduce to leave time for calculations
//     p_reg_switch->ATIME_OFFS_CORR0 = tmp * (interval_ms);     // Space in between, reduce to leave time for calculations
     p_reg_switch->ATIME_OFFS_CORR0 = interval / (clock_dur*delta);     // Space in between in clock cycles (so *8) , reduce to leave time for calculations     
     p_reg_switch->ATIME_OFFSET0 = delta;         // How often we increment

     //pf2("Offset: Diff: %d Corr: %d Off = %d ", diffsign, p_reg_switch->ATIME_OFFS_CORR0, p_reg_switch->ATIME_OFFSET0);

  return 0; 
  
}


/*
   adjusts the rate in which time progresses if freq is non-null. 
   The unit of the rate of adjustment is nanoseconds per second.
   duration: ointer to duration between two sync frames
   if diffsign is 1 it will speed up the clock, otherwise slow down.
   
*/
int clock_adjdrift(int diffsign, uint32_t drift, uint32_t duration, uint8_t timer)
{
   R_ETHSW_Type* p_reg_switch = (void *)R_ETHSW;
  
//    Value to used for performing offset corrections without changing the drift correction. 
//    The value is used in two different ways. 
//    ? If ATIME_OFFS_CORR is zero, the value is immediately added to the current timer 
//    value when this register is written. I.e. the value must be given in true 
//    nanoseconds. When a negative value is given, the value is effectively subtracted. 
//    ? If ATIME_OFFS_CORR is non-zero, the value defines how many offset corrections 
//    will be performed. After each correction, the value is decremented by 1 until it 
//    reaches zero. When it has reached zero no more offset corrections are performed. 
//    The register then is also reset to zero and if enabled an offset event interrupt is 
//    generated. 
//    Note) ATIME_OFFS_CORR must be written prior to writing this register to define the 
//    intended function. 
  
  volatile uint32_t * atime_corr = &(p_reg_switch->ATIME_CORR0); 
  volatile uint32_t * atime_inc = &(p_reg_switch->ATIME_INC0); 
  
  if (timer == 1)
  {
    atime_corr = &(p_reg_switch->ATIME_CORR1); 
    atime_inc = &(p_reg_switch->ATIME_INC1);        
  }
  volatile int brkp = 0;
  if (drift == 0) return 0; 
  if (drift > 4000)  // Avoid overflow
  {
    drift = drift / 100; 
    duration = duration/100; 
  }
  
  /* tmp is the amount of required additional 1ns corrections per second  */
  uint32_t tmp0 = duration/1000;   // convert measure duration to ms
  uint32_t adjcount_corr = 1000000 * drift / tmp0;    //  Required additional amount of corrections per second 
  //pf2("Drift Diff: %ld Tmp: %d tmp0: %d ",tx->time.tv_usec, tmp, tmp0);

  if (adjcount_corr < 1) return 1;   // Drift too small, nothing to be done

  // Fix Div/0  !!!!
  uint32_t adjcounts = 0;     // amount of 1ns adjustmenmts / second
   if (*atime_corr != 0) {
    adjcounts = PTPCLK / *atime_corr;   // Get previous amount of 1 ns adjustments per second
   }
                                      
  if(diffsign != 1)  {                // Slow down
    //pf2("Slowing");
    if (*atime_corr == 0) {     // First time
         *atime_inc  = (*atime_inc & 0x007F80FF) | (((*atime_inc & 0x7f) - 1) << 8);    // Incrementierwert rein
         *atime_corr = PTPCLK/adjcount_corr;     // Space in between
    }
    else if (  ((*atime_inc & 0x007f00) >> 8) < (*atime_inc & 0x07f)) 
    {  // Slowed down already
      if ( (0x7fffffff - adjcount_corr) > adjcounts)  {
          adjcounts += adjcount_corr;
          *atime_corr = PTPCLK / adjcounts; 
      }
      else
          *atime_corr = 0;   // Need to speed up into speed region, so turn off adjustment
    }
    else {                              // In speed up mode
      if (adjcounts > adjcount_corr)  
      {
          adjcounts -= adjcount_corr;
          *atime_corr = PTPCLK / adjcounts; 
      }
      else 
      {     // We are speeding, but cannot reduce the speed enough, so simply turn of the drift correction
         *atime_inc  = (*atime_inc & 0x007F80FF) | ((*atime_inc & 0x7f) << 8);    // Stable value
         *atime_corr = 0;     // Space in between
      }
    }
  }
  else {       // Speed up
   // pf2("Speeding");
    if (*atime_corr == 0) {     // First time
         *atime_inc  = (*atime_inc & 0x007F80FF) | (((*atime_inc & 0x7f) + 1) << 8);    // Incrementierwert rein
         *atime_corr = PTPCLK/adjcount_corr;     // Space in between
    }
    else if (  ((*atime_inc & 0x007f00) >> 8) > (*atime_inc & 0x07f)) {  // Sped up already
      if ( (0x7fffffff - adjcount_corr) > adjcounts)  {
          adjcounts += adjcount_corr;
          *atime_corr = PTPCLK / adjcounts; 
      }
    //  else {
    //    pf2("ERROR: Cannot speed up further" );
    //  }
    }
    else {                              // In slow down mode
      if (adjcounts > adjcount_corr)  {
          adjcounts -= adjcount_corr;
          *atime_corr = PTPCLK / adjcounts; 
      }
      else     
        *atime_corr = 0; 
    }
  }
  //pf2(" Drift: CORR0: %d ",p_reg_switch->ATIME_CORR0);

  return 0; 
  
}

/* Clear drift cerroction and reset to default (no drift correction */
void clear_drift_reg(uint8_t timer)
{
  R_ETHSW_Type* p_reg_switch = (void *)R_ETHSW;
  volatile uint32_t * atime_inc = &(p_reg_switch->ATIME_INC0); 
  volatile uint32_t * atime_corr = &(p_reg_switch->ATIME_CORR0); 

  if (timer == 1)
  {
    atime_inc = &(p_reg_switch->ATIME_INC1);    
    atime_corr = &(p_reg_switch->ATIME_CORR1);     
  }

  uint32_t corr_inc = (*atime_inc & 0x07f) << 8 ; 
  *atime_inc = (*atime_inc & 0x0ff00ff) | corr_inc; 
  *atime_corr = 0; 
}
#define CLOCK_CYCLE 8
/* This function adjust a time value (i. e. measured bridge delay) by the difference of the drift between two timers */
uint32_t adjust_time_drift (uint32_t duration)
{
      R_ETHSW_Type* p_reg_switch = (void *)R_ETHSW;

      uint32_t corr0 = p_reg_switch->ATIME_CORR0 * CLOCK_CYCLE; 
      int32_t inc0 =   p_reg_switch->ATIME_INC0;    
      inc0 = ((inc0 & 0x07f00) >> 8) - (inc0 & 0x07f);    // Contains +1 for speeding up and -1 for slowing down
      uint32_t corr1 = p_reg_switch->ATIME_CORR1 * CLOCK_CYCLE; 
      int32_t inc1 =   p_reg_switch->ATIME_INC1;    // Contains +1 for speeding up and -1 for slowing down
      inc1 = ((inc1 & 0x07f00) >> 8) - (inc1 & 0x07f); 
      int32_t count0 = inc0 * (int32_t) (duration/corr0);     // Contains amount of corrections during time period based on timer 0
      int32_t count1 = inc1 * (int32_t) (duration/corr1);     // Contains amount of corrections during time period based on timer 1
      uint32_t duration_new = (uint32_t) ((int32_t) duration - (count1 - count0)/8);  // Div by 8 empirically evaluated for bridge delay adjustment. 
      return(duration_new); 
}

/********************************************************************************************************************//**
 * @brief Returns the status of the physical link. Implements @ref ether_phy_api_t::linkStatusGet.
 *
 * @retval  FSP_SUCCESS                                 ETHER_PHY successfully get link 
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER_PHY control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened
 * @retval  FSP_ERR_ETHER_PHY_ERROR_LINK                PHY-LSI is not link up.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_SWITCH_LinkStatusGet(ether_phy_ctrl_t * const p_ctrl)
{
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type* p_reg_switch = (void *)R_ETHSW;

    uint32_t reg_1;
    const volatile uint32_t * p_reg;            /* Register */
    fsp_err_t err    = FSP_SUCCESS;

#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif
       
    uint8_t channel = p_instance_ctrl->p_ether_phy_cfg->channel; 

    p_reg = (const volatile uint32_t *) ((uint8_t *) &p_reg_switch->STATUS_P0 + ETHER_SWITCH_COUNTER_OFFSET * channel);
    uint8_t linkstate = (* p_reg & ETHER_SWITCH_PHY_LINK) ? ETHER_SWITCH_STATE_UP : ETHER_SWITCH_STATE_DOWN;
    
#ifdef PrintfDebug
		{
    uint32_t reg_0 = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_CONTROL);
    uint32_t reg_1 = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_STATUS);
    reg_1 = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_STATUS);
			char text_string[30];
			snprintf(&text_string[0], 30,  "Phy %d %d %x %x",p_instance_ctrl->p_ether_phy_cfg->channel, linkstate, reg_0, reg_1);
			printeth(text_string);
		}
#endif

    /* When the link isn't up, return error */
    if (linkstate == ETHER_SWITCH_STATE_DOWN)
    {
        /* Link is down */
        err = FSP_ERR_ETHER_PHY_ERROR_LINK;
    }
    else
    {
        /* Link is up */
        err = FSP_SUCCESS;
    }

    return err;

} /* End of function R_ETHER_PHY_LinkStatusGet() */
