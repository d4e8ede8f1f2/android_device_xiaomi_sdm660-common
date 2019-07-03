
#ifndef IPACM_NEIGHBOR_H
#define IPACM_NEIGHBOR_H

#include <stdio.h>
#include <IPACM_CmdQueue.h>
#include <linux/msm_ipa.h>
#include "IPACM_Routing.h"
#include "IPACM_Filtering.h"
#include "IPACM_Listener.h"
#include "IPACM_Iface.h"

#define IPA_MAX_NUM_NEIGHBOR_CLIENTS  100

struct ipa_neighbor_client
{
	uint8_t mac_addr[6];
	int iface_index;
	uint32_t v4_addr;
	int ipa_if_num;
	/* add support for handling L2TP clients which associated with eth0 vlan interface */
	char iface_name[IPA_IFACE_NAME_LEN];
};

class IPACM_Neighbor : public IPACM_Listener
{

public:

	IPACM_Neighbor();

	void event_callback(ipa_cm_event_id event,
											void *data);

private:

	int num_neighbor_client;

	int circular_index;

	ipa_neighbor_client neighbor_client[IPA_MAX_NUM_NEIGHBOR_CLIENTS];

};

#endif /* IPACM_NEIGHBOR_H */
