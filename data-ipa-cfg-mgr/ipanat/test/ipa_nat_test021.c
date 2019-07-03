
#include "ipa_nat_test.h"
#include "ipa_nat_drv.h"

int ipa_nat_test021(int total_entries, int reg)
{

	int ret, i;
	u32 tbl_hdl;
	u32 pub_ip_add = 0x011617c0;   /* "192.23.22.1" */

	IPADBG("%s():\n",__FUNCTION__);

	for(i=0; i<reg; i++)
	{
		IPADBG("executing %d th time:\n",i);

		IPADBG("calling ipa_nat_add_ipv4_tbl() \n");
		ret = ipa_nat_add_ipv4_tbl(pub_ip_add, total_entries, &tbl_hdl);
		if (0 != ret)
		{
			IPAERR("unable to create ipv4 nat table and returning Error:%d\n", ret);
			IPADBG("executed %d times:\n",i);
			return -1;
		}
		IPADBG("create nat ipv4 table successfully() \n");

		IPADBG("calling ipa_nat_del_ipv4_tbl() \n");
		ret = ipa_nat_del_ipv4_tbl(tbl_hdl);
		if (0 != ret)
		{
			IPAERR("Unable to delete ipv4 nat table %d\n", ret);
			IPADBG("executed %d times:\n",i);
			return -1;
		}
		IPADBG("deleted ipv4 nat table successfully. Test passed \n");
	}

	IPADBG("executed %d times:\n",(i+1));
	return 0;
}
