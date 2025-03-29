#ifndef __ANYIP_TCIP_H__
#define __ANYIP_TCIP_H__

/**************************************************************************//**
@file		anyip_tcip.h
@brief		Contains functions for IP stack managment excluding interface and
			address managment.																			
******************************************************************************/
#include "anyip_inc.h"

#if (ANYIP_CFG_USE_TCPKEEPTIME == 1)
/**************************************************************************//**
@ingroup		anyip_tcip_mgmt
@brief          Sets an internal parameter of the TCP stack.
@details        Sets the TCP initial connect keep alive time value.
				If an connection is idle (no segments received) for 
				tcp_connection_timeout amount of time,
				but not yet established, then we drop the connection.

@param[in]      tcp_connection_timeout: time value in seconds (must be >= 0)

@return         0 in case of success
@return         -1 in case of errors (errno is set accordingly)
******************************************************************************/
int anyip_tcip_set_tcp_keepinittime(int tcp_connection_timeout /* seconds */);

/**************************************************************************//**
@ingroup		anyip_tcip_mgmt
@brief          Sets an internal parameter of the TCP stack.
@details        Sets the TCP keep alive probe interval.

@param[in]      tcp_keepintvl: time value in seconds (must be >= 0)

@return         0 in case of success
@return         -1 in case of errors (errno is set accordingly)
******************************************************************************/
int anyip_tcip_set_tcp_keepintvl(int tcp_keepintvl          /* seconds */);

/**************************************************************************//**
@ingroup		anyip_tcip_mgmt
@brief          Sets an internal parameter of the TCP stack.
@details        Sets the TCP maximum keepalive probes before dropping the 
				connection.

@param[in]      tcp_keepcount: the count value

@return         0 in case of success
@return         -1 in case of errors (errno is set accordingly)
******************************************************************************/
int anyip_tcip_set_tcp_keepcount(int tcp_keepcount);

/**************************************************************************//**
@ingroup		anyip_tcip_mgmt
@brief          Sets an internal parameter of the TCP stack.
@details        Sets the maximum possible TCP retransmission timer value.

@param[in]      tcp_retransmit_timeout: time value in seconds (must be >= 0)

@return         0 in case of success
@return         -1 in case of errors (errno is set accordingly)
******************************************************************************/
int anyip_tcip_set_tcp_rexmtmax(int tcp_retransmit_timeout /* seconds */);

/**************************************************************************//**
@ingroup		anyip_tcip_mgmt
@brief          Sets an internal parameter of the TCP stack.
@details        Sets the TCP idle timeout before the stack starts keepalive 
				probing. Once the connection is established, if the connection 
				is idle for tcp_keepidle_timeout time (and keepalives have 
				been enabled on the socket), we begin to probe the connection.

@param[in]      tcp_keepidle_timeout: time value in seconds (must be >= 0)

@return         0 in case of success
@return         -1 in case of errors (errno is set accordingly)
******************************************************************************/
int anyip_tcip_set_tcp_keepidle(int tcp_keepidle_timeout   /* seconds */);

/**************************************************************************//**
@ingroup		anyip_tcip_mgmt
@brief          Returns an internal parameter of the TCP stack.
@details        Returns the TCP initial connect keep alive time value.
				If an connection is idle (no segments received) for 
				tcp_connection_timeout amount of time, but not yet 
				established, then we drop the connection.

@return         tcp_connection_timeout in seconds in case of success
@return         -1 in case of errors (errno is set accordingly)
******************************************************************************/
int anyip_tcip_get_tcp_keepinittime(void);

/**************************************************************************//**
@ingroup		anyip_tcip_mgmt
@brief          Returns an internal parameter of the TCP stack.
@details        Returns the TCP keep alive probe interval.

@return         tcp_keepintvl in seconds in case of success
@return         -1 in case of errors (errno is set accordingly)
******************************************************************************/
int anyip_tcip_get_tcp_keepintvl(void);

/**************************************************************************//**
@ingroup		anyip_tcip_mgmt
@brief          Returns an internal parameter of the TCP stack.
@details        Returns the TCP maximum keepalive probes before dropping 
				the connection

@return         tcp_keepcount: the count value
@return         -1 in case of errors (errno is set accordingly)
******************************************************************************/
int anyip_tcip_get_tcp_keepcount(void);

/**************************************************************************//**
@ingroup		anyip_tcip_mgmt
@brief          Returns an internal parameter of the TCP stack.
@details        Returns the maximum possible TCP retransmission timer value.

@return         tcp_retransmit_timeout: time value in seconds
@return         -1 in case of errors (errno is set accordingly)
******************************************************************************/
int anyip_tcip_get_tcp_rexmtmax(void);

/**************************************************************************//**
@ingroup		anyip_tcip_mgmt
@brief          Returns an internal parameter of the TCP stack.
@details        Returns the TCP idle timeout before the stack starts 
				keepalive probing.

@return         tcp_keepidle_timeout: time value in seconds
@return         -1 in case of errors (errno is set accordingly)
******************************************************************************/
int anyip_tcip_get_tcp_keepidle(void);
#endif /* (ANYIP_CFG_USE_TCPKEEPTIME == 1) */

/**************************************************************************//**
@ingroup		anyip_tcip_mgmt
@brief          Initializes the MIB2 system variables.
@detail			Sets the system variables sysLocation, sysName, sysContact,
				and sysDescription to default values.
******************************************************************************/
void anyip_tcip_mib2_sysinit(void);

/**************************************************************************//**
@ingroup	anyip_tcip_mgmt
@brief		Sets the value for MIB-2 variable sysName 
			(OID=1.3.6.1.2.1.1.5)
@details	This function is needed to provide the user with the possibility
			to set the sysName OID (e.g. from Remanent storage). It copies 
			sysName_length bytes from parameter p_sysName to a stack-specific 
			storage for the system name. The string is terminated with \0. 
			If the string is too long nothing is copied.

@note		String length must be <= 255, according to the sysName 
			object type: DisplayString (SIZE (0..255)).

@param[in]	p_sysName:		string for system description
@param[in]	sysName_length:	length of p_sysName without trailing \0

@return		0: in case of success
@return		-1: in case of an error (string too long)
******************************************************************************/
int anyip_tcip_set_mib2_sysName(const char *p_sysName, unsigned int sysName_length);

/**************************************************************************//**
@ingroup	anyip_tcip_mgmt
@brief		Sets the value for MIB-2 variable sysDescr 
			(OID=1.3.6.1.2.1.1.1)
@details	This function is needed to provide the user with the possibility
			to set the sysDescr OID (e.g. from Remanent storage). It copies 
			sysDescr_length bytes from parameter p_sysDescr to a stack-specific 
			storage for the system description. The string is terminated with \0. 
			If the string is too long nothing is copied.

@note		This function is used to set the sysDescr internally as 
			sysDescr is not writable via snmp requests.

@note		String length must be <= 255, according to the sysDescr 
			object type: DisplayString (SIZE (0..255)).

@param[in]	p_sysDescr:			string for system description
@param[in]	sysDescr_length:	length of p_sysDescr without trailing \0

@return		0: in case of success
@return		-1: in case of an error (string too long)
******************************************************************************/
int anyip_tcip_set_mib2_sysDescr(const char *p_sysDescr, unsigned int sysDescr_length);

/**************************************************************************//**
@ingroup	anyip_tcip_mgmt
@brief		Sets the value for MIB-2 variable sysContact 
			(OID=1.3.6.1.2.1.1.4)
@details	This function is needed to provide the user with the possibility
			to set the sysContact OID (e.g. from Remanent storage). It copies 
			sysContact_length bytes from parameter p_sysContact to a 
			stack-specific storage for the system contact. The string is 
			terminated with \0. If the string is too long nothing is copied.

@note		String length must be <= 255, according to the sysContact 
			object type: DisplayString (SIZE (0..255)).

@param[in]	p_sysContact:		string for system contact
@param[in]	sysContact_length:	length of p_sysContact without trailing \0

@return		0: in case of success
@return		-1: in case of an error (string too long)
******************************************************************************/
int anyip_tcip_set_mib2_sysContact(const char *p_sysContact, unsigned int sysContact_length);

/**************************************************************************//**
@ingroup	anyip_tcip_mgmt
@brief		Sets the value for MIB-2 variable sysLocation 
			(OID=1.3.6.1.2.1.1.4)
@details	This function is needed to provide the user with the possibility
			to set the sysLocation OID (e.g. from Remanent storage). It copies 
			sysLocation_length bytes from parameter p_sysLocation to a 
			stack-specific storage for the system location. The string is 
			terminated with \0. If the string is too long nothing is copied.

@note		String length must be <= 255, according to the sysLocation 
			object type: DisplayString (SIZE (0..255)).

@param[in]	p_sysLocation:		string for system location
@param[in]	sysLocation_length:	length of p_sysLocation without trailing \0

@return		0: in case of success
@return		-1: in case of an error (string too long)
******************************************************************************/
int anyip_tcip_set_mib2_sysLocation(const char *p_sysLocation, unsigned int sysLocation_length);

/**************************************************************************//**
@ingroup	anyip_tcip_mgmt
@brief		Returns value of MIB-2 sysName
@details	Returns value of the MIB-2 variable sysName
			(OID = 1.3.6.1.2.1.1.5)

@note		String length must be <= 255, according to the sysName
			object type: DisplayString(SIZE(0..255))

@return		String containing sysName
******************************************************************************/
char *anyip_tcip_get_mib2_sysName(void);

/**************************************************************************//**
@ingroup	anyip_tcip_mgmt
@brief		Returns value of MIB-2 sysDescr
@details	Returns value of the MIB-2 variable sysDescr
			(OID = 1.3.6.1.2.1.1.1)

@note		String length must be <= 255, according to the sysDescr 
			object type: DisplayString(SIZE(0..255))

@return		String containing sysDescr
******************************************************************************/
char *anyip_tcip_get_mib2_sysDescr(void);

/**************************************************************************//**
@ingroup	anyip_tcip_mgmt
@brief		Returns value of MIB-2 sysContact
@details	Returns value of the MIB-2 variable sysContact
			(OID = 1.3.6.1.2.1.1.4)

@note		String length must be <= 255, according to the sysContact 
			object type: DisplayString(SIZE(0..255))

@return		String containing sysContact
******************************************************************************/
char *anyip_tcip_get_mib2_sysContact(void);

/**************************************************************************//**
@ingroup	anyip_tcip_mgmt
@brief		Returns value of MIB-2 sysLocation
@details	Returns value of the MIB-2 variable sysLocation
			(OID = 1.3.6.1.2.1.1.6)

@note		String length must be <= 255, according to the sysLocation 
			object type: DisplayString(SIZE(0..255))

@return		String containing sysLocation
******************************************************************************/
char *anyip_tcip_get_mib2_sysLocation(void);

/**************************************************************************//**
@ingroup	anyip_tcip_mgmt
@brief		Callback function that is called, if snmp writeable system 
			variables are written.

@details	This function is the callback function for updating the SOCK
			mirror of system group variables. The value of the parameter is
			compared with the current internal values. If the values match,
			SOCK is updated.
			The variables are :
@li			sysContact(OID = 1.3.6.1.2.1.1.4)
@li			sysName(OID = 1.3.6.1.2.1.1.5)
@li			sysLocation(OID = 1.3.6.1.2.1.1.6)

@note		This function MUST be used if the above mentioned OIDs are 
			written via SNMP.
@note		This function is declared here, but implemented in tcip_mib.c. 
			No further implmentation is necessary.

@param[in]	snmp_data: string containing the value of the written data
******************************************************************************/
void tcip_mib2_written(const void * snmp_data);

#endif /* __ANYIP_TCIP_H__ */
