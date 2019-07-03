
#include "ipa_nat_test.h"
#include "ipa_nat_drv.h"

int ipa_nat_test004(int total_entries, u32 tbl_hdl, u8 sep)
{
	int ret = 0;
	u32 tbl_hdl1 = 0;
	u32 pub_ip_add = 0x011617c0;   /* "192.23.22.1" */

	IPADBG("%s():\n",__FUNCTION__);

	if(sep)
	{
		ret = ipa_nat_add_ipv4_tbl(pub_ip_add, total_entries, &tbl_hdl);
		CHECK_ERR(ret);

		ret = ipa_nat_del_ipv4_tbl(tbl_hdl);
		CHECK_ERR(ret);

		ret = ipa_nat_del_ipv4_tbl(tbl_hdl1);
		if(ret == 0)
		{
			IPAERR("able to delete table using invalid table handle\n");
			return -1;
		}
	}
	return 0;
}
