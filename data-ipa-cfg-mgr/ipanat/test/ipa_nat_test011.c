
#include "ipa_nat_test.h"
#include "ipa_nat_drv.h"

int ipa_nat_test011(int total_entries, u32 tbl_hdl, u8 sep)
{
	int ret;
	u32 rule_hdl, rule_hdl1, rule_hdl2;
	ipa_nat_ipv4_rule ipv4_rule, ipv4_rule1, ipv4_rule2;
	u32 pub_ip_add = 0x011617c0;   /* "192.23.22.1" */

	ipv4_rule.target_ip = 0xC1171601; /* 193.23.22.1 */
	ipv4_rule.target_port = 1234;
	ipv4_rule.private_ip = 0xC2171601; /* 194.23.22.1 */
	ipv4_rule.private_port = 5678;
	ipv4_rule.protocol = IPPROTO_TCP;
	ipv4_rule.public_port = 9050;

	ipv4_rule1.target_ip = 0xF1181601;
	ipv4_rule1.target_port = 1555;
	ipv4_rule1.private_ip = 0xF2151601;
	ipv4_rule1.private_port = 5999;
	ipv4_rule1.protocol = IPPROTO_TCP;
	ipv4_rule1.public_port = 9111;

	ipv4_rule2.target_ip = 0xC1166602;
	ipv4_rule2.target_port = 1555;
	ipv4_rule2.private_ip = 0xC2155602;
	ipv4_rule2.private_port = 5777;
	ipv4_rule2.protocol = IPPROTO_TCP;
	ipv4_rule2.public_port = 9000;

	IPADBG("%s():\n",__FUNCTION__);

	if(sep)
	{
		ret = ipa_nat_add_ipv4_tbl(pub_ip_add, total_entries, &tbl_hdl);
		CHECK_ERR(ret);
	}

	ret = ipa_nat_add_ipv4_rule(tbl_hdl, &ipv4_rule, &rule_hdl);
	CHECK_ERR(ret);

	ret = ipa_nat_add_ipv4_rule(tbl_hdl, &ipv4_rule1, &rule_hdl1);
	CHECK_ERR(ret);

	ret = ipa_nat_add_ipv4_rule(tbl_hdl, &ipv4_rule2, &rule_hdl2);
	CHECK_ERR(ret);

	ret = ipa_nat_del_ipv4_rule(tbl_hdl, rule_hdl1);
	CHECK_ERR(ret);

	ret = ipa_nat_del_ipv4_rule(tbl_hdl, rule_hdl);
	CHECK_ERR(ret);

	ret = ipa_nat_del_ipv4_rule(tbl_hdl, rule_hdl2);
	CHECK_ERR(ret);

	if(sep)
	{
		ret = ipa_nat_del_ipv4_tbl(tbl_hdl);
		CHECK_ERR(ret);
	}

	return 0;
}
