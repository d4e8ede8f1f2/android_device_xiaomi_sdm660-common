
//////////////////////////////////////////////////////////////////////////////////

#ifndef IPACM_HEADER_H
#define IPACM_HEADER_H

#include <stdint.h>
#include "linux/msm_ipa.h"

//////////////////////////////////////////////////////////////////////////////////

class IPACM_Header
{
private:
	int m_fd;
public:
	bool AddHeader(struct ipa_ioc_add_hdr   *pHeaderTable);
	bool DeleteHeader(struct ipa_ioc_del_hdr *pHeaderTable);
	bool GetHeaderHandle(struct ipa_ioc_get_hdr *pHeaderStruct);
	bool CopyHeader(struct ipa_ioc_copy_hdr *pCopyHeaderStruct);
	bool Commit();
	bool Reset();
	bool DeleteHeaderHdl(uint32_t hdr_hdl);
	bool AddHeaderProcCtx(struct ipa_ioc_add_hdr_proc_ctx* pHeader);
	bool DeleteHeaderProcCtx(uint32_t hdl);

	IPACM_Header();
	~IPACM_Header();
	bool DeviceNodeIsOpened();
};

#endif
