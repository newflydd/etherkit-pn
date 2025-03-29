#include "anyip_inc.h"
#include "ipport.h"
#include "tcpport.h"
#include "bsdsock.h"

#if ANYIP_CLOCK_RATE_HZ != TPS
#error clock rate is different!
#endif

#if ANYIP_ETHHDR_BIAS != ETHHDR_BIAS
#error ETHHDR_BIAS is different!
#endif

#if ANYIP_ECONNABORTED != INICHE_ECONNABORTED
#error (incompatible ECONNABORTED)
#endif

#if (ANYIP_EMSGSIZE != INICHE_EMSGSIZE)
#error (incompatible ANYIP_EMSGSIZE)
#endif

#if (ANYIP_EHOSTUNREACH != ENP_NO_ROUTE)
#error (incompatible ANYIP_EHOSTUNREACH)
#endif

#if ANYIP_AF_INET != 2
#error (incompatible AF_INET)
#endif

#if ANYIP_SENDBUFFER_SIZE != BIGBUFSIZE
#error (incompatible SENDBUFFER_SIZE)
#endif

