
#include "ipa_nat_test.h"
#include "ipa_nat_drv.h"

int ipa_nat_test000(int total_entries, u32 tbl_hdl, u8 sep)
{

	int ret;
	u32 pub_ip_add = 0x011617c0;   /* "192.23.22.1" */

	ret = ipa_nat_add_ipv4_tbl(pub_ip_add, total_entries, &tbl_hdl);
	if (0 != ret)
	{
		IPAERR("unable to create ipv4 nat table and returning Error:%d\n", ret);
		return -1;
	}
	IPADBG("create nat ipv4 table successfully() \n");

	IPADBG("calling ipa_nat_del_ipv4_tbl() \n");
	ret = ipa_nat_del_ipv4_tbl(tbl_hdl);
	if (0 != ret)
	{
		IPAERR("Unable to delete ipv4 nat table %d\n", ret);
		return -1;
	}
	IPADBG("deleted ipv4 nat table successfully. Test passed \n");

	return 0;
}
