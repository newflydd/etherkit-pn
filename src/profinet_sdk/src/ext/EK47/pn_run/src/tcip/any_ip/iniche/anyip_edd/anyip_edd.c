#include "anyip_edd.h"

#include "ipport.h"
#include "tcpport.h"
#include "net.h"

void anyip_edd_set_statistic_counters(ANYIP_IFNET_PTR ifp,
	long inOctets,
	long inUcastPkts,
	long inNUcastPkts,
	long inDiscards,
	long inErrors,
	long inUnknownProtos,
	long outOctets,
	long outUcastPkts,
	long outNUcastPkts,
	long outDiscards,
	long outErrors,
	long outQLen,
	unsigned long supportedCounters)
{
	NET my_ifp = (NET)ifp;
	my_ifp->mib.ifInOctets = (unsigned long)inOctets;
	my_ifp->mib.ifInUcastPkts = (unsigned long)inUcastPkts;
	my_ifp->mib.ifInNUcastPkts = (unsigned long)inNUcastPkts;
	my_ifp->mib.ifInDiscards = (unsigned long)inDiscards;
	my_ifp->mib.ifInErrors = (unsigned long)inErrors;
	my_ifp->mib.ifInUnknownProtos = (unsigned long)inUnknownProtos;
	my_ifp->mib.ifOutOctets = (unsigned long)outOctets;
	my_ifp->mib.ifOutUcastPkts = (unsigned long)outUcastPkts;
	my_ifp->mib.ifOutNUcastPkts = (unsigned long)outNUcastPkts;
	my_ifp->mib.ifOutDiscards = (unsigned long)outDiscards;
	my_ifp->mib.ifOutErrors = (unsigned long)outErrors;
	my_ifp->mib.ifOutQLen = (unsigned long)outQLen;
	my_ifp->mib.SupportedCounters = supportedCounters; /* see Request ID 216015 */
}
