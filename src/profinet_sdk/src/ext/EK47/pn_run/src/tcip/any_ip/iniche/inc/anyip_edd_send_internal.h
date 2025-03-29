#ifndef __ANYIP_EDD_SEND_INTERNAL_H__
#define __ANYIP_EDD_SEND_INTERNAL_H__

#include "anyip_inc.h"

/* internal functions */
void iniche_free_remaining_send_packets(ANYIP_IFNET_PTR ifp);
int iniche_is_send_resource_available(ANYIP_IFNET_PTR ifp);

#endif  /* __ANYIP_EDD_SEND_INTERNAL_H__ */
