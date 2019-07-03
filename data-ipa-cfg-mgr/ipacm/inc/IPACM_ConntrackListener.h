
#ifndef IPACM_CONNTRACK_LISTENER
#define IPACM_CONNTRACK_LISTENER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>

#include "IPACM_CmdQueue.h"
#include "IPACM_Conntrack_NATApp.h"
#include "IPACM_Listener.h"
#ifdef CT_OPT
#include "IPACM_LanToLan.h"
#endif

#define MAX_IFACE_ADDRESS 50
#define MAX_STA_CLNT_IFACES 10
#define STA_CLNT_SUBNET_MASK 0xFFFFFF00

using namespace std;

typedef struct _nat_entry_bundle
{
	struct nf_conntrack *ct;
	enum nf_conntrack_msg_type type;
	nat_table_entry *rule;
	bool isTempEntry;

}nat_entry_bundle;

class IPACM_ConntrackListener : public IPACM_Listener
{

private:
	bool isCTReg;
	bool isNatThreadStart;
	bool WanUp;
	NatApp *nat_inst;

	int NatIfaceCnt;
	int StaClntCnt;
	NatIfaces *pNatIfaces;
	uint32_t nat_iface_ipv4_addr[MAX_IFACE_ADDRESS];
	uint32_t nonnat_iface_ipv4_addr[MAX_IFACE_ADDRESS];
	uint32_t sta_clnt_ipv4_addr[MAX_STA_CLNT_IFACES];
	IPACM_Config *pConfig;
#ifdef CT_OPT
	IPACM_LanToLan *p_lan2lan;
#endif

	void ProcessCTMessage(void *);
	void ProcessTCPorUDPMsg(struct nf_conntrack *,
	enum nf_conntrack_msg_type, u_int8_t);
	void TriggerWANUp(void *);
	void TriggerWANDown(uint32_t);
	int  CreateNatThreads(void);
	bool AddIface(nat_table_entry *, bool *);
	void AddORDeleteNatEntry(const nat_entry_bundle *);
	void PopulateTCPorUDPEntry(struct nf_conntrack *, uint32_t, nat_table_entry *);
	void CheckSTAClient(const nat_table_entry *, bool *);
	int CheckNatIface(ipacm_event_data_all *, bool *);
	void HandleNonNatIPAddr(void *, bool);

#ifdef CT_OPT
	void ProcessCTV6Message(void *);
	void HandleLan2Lan(struct nf_conntrack *,
		enum nf_conntrack_msg_type, nat_table_entry* );
#endif

public:
	char wan_ifname[IPA_IFACE_NAME_LEN];
	uint32_t wan_ipaddr;
	bool isStaMode;
	IPACM_ConntrackListener();
	void event_callback(ipa_cm_event_id, void *data);
	inline bool isWanUp()
	{
		return WanUp;
	}

	void HandleNeighIpAddrAddEvt(ipacm_event_data_all *);
	void HandleNeighIpAddrDelEvt(uint32_t);
	void HandleSTAClientAddEvt(uint32_t);
	void HandleSTAClientDelEvt(uint32_t);
	int  CreateConnTrackThreads(void);
};

extern IPACM_ConntrackListener *CtList;

#endif /* IPACM_CONNTRACK_LISTENER */
