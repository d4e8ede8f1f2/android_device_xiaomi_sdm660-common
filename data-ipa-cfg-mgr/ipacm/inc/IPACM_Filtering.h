
#ifndef IPACM_FILTERING_H
#define IPACM_FILTERING_H

#include <stdint.h>
#include <linux/msm_ipa.h>
#include <IPACM_Defs.h>
#include <linux/rmnet_ipa_fd_ioctl.h>

class IPACM_Filtering
{
public:
	IPACM_Filtering();
	~IPACM_Filtering();
	bool AddFilteringRule(struct ipa_ioc_add_flt_rule const *ruleTable);
	bool AddFilteringRuleAfter(struct ipa_ioc_add_flt_rule_after const *ruleTable);
	bool DeleteFilteringRule(struct ipa_ioc_del_flt_rule *ruleTable);
	bool Commit(enum ipa_ip_type ip);
	bool Reset(enum ipa_ip_type ip);
	bool DeviceNodeIsOpened();
	bool DeleteFilteringHdls(uint32_t *flt_rule_hdls,
													 ipa_ip_type ip,
													 uint8_t num_rules);

	bool AddWanDLFilteringRule(struct ipa_ioc_add_flt_rule const *rule_table_v4, struct ipa_ioc_add_flt_rule const * rule_table_v6, uint8_t mux_id);
	bool SendFilteringRuleIndex(struct ipa_fltr_installed_notif_req_msg_v01* table);
	bool ModifyFilteringRule(struct ipa_ioc_mdfy_flt_rule* ruleTable);
	ipa_filter_action_enum_v01 GetQmiFilterAction(ipa_flt_action action);

private:
	static const char *DEVICE_NAME;
	int fd; /* File descriptor of the IPA device node /dev/ipa */
};

#endif //IPACM_FILTERING_H
