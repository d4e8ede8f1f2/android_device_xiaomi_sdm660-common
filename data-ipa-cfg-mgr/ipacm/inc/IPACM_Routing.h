
#ifndef IPACM_ROUTING_H
#define IPACM_ROUTING_H

#include <stdint.h>
#include <linux/msm_ipa.h>
#include <IPACM_Defs.h>

using namespace std;

class IPACM_Routing
{
public:
	IPACM_Routing();
	~IPACM_Routing();

	bool AddRoutingRule(struct ipa_ioc_add_rt_rule *ruleTable);
	bool DeleteRoutingRule(struct ipa_ioc_del_rt_rule *ruleTable);

	bool Commit(enum ipa_ip_type ip);
	bool Reset(enum ipa_ip_type ip);

	bool GetRoutingTable(struct ipa_ioc_get_rt_tbl *routingTable);

	bool DeviceNodeIsOpened();
	bool DeleteRoutingHdl(uint32_t rt_rule_hdl, ipa_ip_type ip);

	bool ModifyRoutingRule(struct ipa_ioc_mdfy_rt_rule *);

private:
	static const char *DEVICE_NAME;
	int m_fd; /* File descriptor of the IPA device node /dev/ipa */

	bool PutRoutingTable(uint32_t routingTableHandle);
};

#endif //IPACM_ROUTING_H
