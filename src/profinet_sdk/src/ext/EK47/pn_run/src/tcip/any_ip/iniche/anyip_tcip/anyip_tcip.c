#include "tcip_iniche_cfg.h"

#include "anyip_inc.h"
#include "anyip_tcip.h"

#include "ipport.h"
#include "tcpport.h"
#include "tcp_timr.h"
#include "snmpport.h"
#include "snmp_imp.h"

#if (ANYIP_CFG_USE_TCPKEEPTIME == 1)
int  tcip_ConnectionTimeout;
int  tcip_RetransmitCntMax;
int  tcip_RetransmitTimeout;
#endif /* (ANYIP_CFG_USE_TCPKEEPTIME == 1) */

extern char     sys_descr[SYS_STRING_MAX];
extern char     sysContact[SYS_STRING_MAX];
extern char     sysLocation[SYS_STRING_MAX];
extern char     sysName[SYS_STRING_MAX];

#if (ANYIP_CFG_USE_TCPKEEPTIME == 1)
/* ---------------------------------------------------------------------------------------------- */

int anyip_tcip_set_tcp_keepinittime(int tcp_connection_timeout /* seconds */)
{
	tcip_ConnectionTimeout = tcp_connection_timeout * PR_SLOWHZ;
	return 0;
}

/* ---------------------------------------------------------------------------------------------- */

int anyip_tcip_get_tcp_keepinittime(void)
{
	return (tcip_ConnectionTimeout / PR_SLOWHZ);
}

/* ---------------------------------------------------------------------------------------------- */

int anyip_tcip_set_tcp_keepintvl(int tcp_keepinterval /* seconds */)
{
	tcp_keepintvl = tcp_keepinterval * PR_SLOWHZ;
	return 0;
}

/* ---------------------------------------------------------------------------------------------- */

int anyip_tcip_get_tcp_keepintvl(void)
{
	return (tcp_keepintvl / PR_SLOWHZ);
}

/* ---------------------------------------------------------------------------------------------- */

int anyip_tcip_set_tcp_keepcount(int tcp_keepcount)
{
	tcip_RetransmitCntMax = tcp_keepcount;
	return 0;
}

/* ---------------------------------------------------------------------------------------------- */

int anyip_tcip_get_tcp_keepcount(void)
{
	return tcip_RetransmitCntMax;
}

/* ---------------------------------------------------------------------------------------------- */

int anyip_tcip_set_tcp_rexmtmax(int tcp_retransmit_timeout /* in seconds */)
{
	tcip_RetransmitTimeout = tcp_retransmit_timeout * PR_SLOWHZ;
	return 0;
}

/* ---------------------------------------------------------------------------------------------- */

int anyip_tcip_get_tcp_rexmtmax(void)
{
	return (tcip_RetransmitTimeout / PR_SLOWHZ);
}

/* ---------------------------------------------------------------------------------------------- */

int anyip_tcip_set_tcp_keepidle(int tcp_keepidle_timeout /* in seconds */)
{
	tcp_keepidle = tcp_keepidle_timeout * PR_SLOWHZ;
	return 0;
}

/* ---------------------------------------------------------------------------------------------- */

int anyip_tcip_get_tcp_keepidle(void)
{
	return (tcp_keepidle / PR_SLOWHZ);
}

#endif /* (ANYIP_CFG_USE_TCPKEEPTIME == 1) */
/* ---------------------------------------------------------------------------------------------- */
void anyip_tcip_mib2_sysinit(void)
{
	TCIP_MEMSET(sysName, 0, SYS_STRING_MAX);
	TCIP_MEMSET(sysContact, 0, SYS_STRING_MAX);
	TCIP_MEMSET(sysLocation, 0, SYS_STRING_MAX);
	TCIP_MEMSET(sys_descr, 0, SYS_STRING_MAX);
}

/* ---------------------------------------------------------------------------------------------- */
int anyip_tcip_set_mib2_sysName(const char *p_sysName, unsigned int sysName_length)
{
	if (sysName_length > SYS_STRING_MAX - 1) return -1;
	TCIP_MEMCPY(sysName, (void*)p_sysName, sysName_length);
	sysName[sysName_length] = 0;
	return 0;
}

/* ---------------------------------------------------------------------------------------------- */

int anyip_tcip_set_mib2_sysDescr(const char *p_sysDescr, unsigned int sysDescr_length)
{
	if (sysDescr_length > SYS_STRING_MAX - 1) return -1;
	TCIP_MEMCPY(sys_descr, (void*)p_sysDescr, sysDescr_length);
	sys_descr[sysDescr_length] = 0;
	return 0;
}

/* ---------------------------------------------------------------------------------------------- */

int anyip_tcip_set_mib2_sysContact(const char *p_sysContact, unsigned int sysContact_length)
{
	if (sysContact_length > SYS_STRING_MAX - 1) return -1;
	TCIP_MEMCPY(sysContact, (void*)p_sysContact, sysContact_length);
	sysContact[sysContact_length] = 0;
	return 0;
}

/* ---------------------------------------------------------------------------------------------- */

int anyip_tcip_set_mib2_sysLocation(const char *p_sysLocation, unsigned int sysLocation_length)
{
	if (sysLocation_length > SYS_STRING_MAX - 1) return -1;
	TCIP_MEMCPY(sysLocation, (void*)p_sysLocation, sysLocation_length);
	sysLocation[sysLocation_length] = 0;
	return 0;
}

/* ---------------------------------------------------------------------------------------------- */

char *anyip_tcip_get_mib2_sysName(void)
{
	return sysName;
}

/* ---------------------------------------------------------------------------------------------- */

char *anyip_tcip_get_mib2_sysDescr(void)
{
	return sys_descr;
}

/* ---------------------------------------------------------------------------------------------- */

char *anyip_tcip_get_mib2_sysContact(void)
{
	return sysContact;
}

/* ---------------------------------------------------------------------------------------------- */

char *anyip_tcip_get_mib2_sysLocation(void)
{
	return sysLocation;
}

/* ---------------------------------------------------------------------------------------------- */
