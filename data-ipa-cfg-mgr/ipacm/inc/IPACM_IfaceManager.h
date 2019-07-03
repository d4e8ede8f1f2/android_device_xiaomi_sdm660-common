
#ifndef IPACM_IFACEMANAGER_H
#define IPACM_IFACEMANAGER_H

#include <stdio.h>
#include <IPACM_CmdQueue.h>

#include "IPACM_Routing.h"
#include "IPACM_Filtering.h"
#include "IPACM_Listener.h"
#include "IPACM_Iface.h"

#define IPA_INSTANCE_NOT_FOUND  0
#define IPA_INSTANCE_FOUND  1

/* queue */
typedef struct _iface_instances
{
    /* Linux interface id */
	int ipa_if_index;
	IPACM_Listener *obj;
	_iface_instances *next;
}  iface_instances;


class IPACM_IfaceManager : public IPACM_Listener
{

public:

  IPACM_IfaceManager();

  void event_callback(ipa_cm_event_id event,
                      void *data);

  /* api for all iface instances to de-register instances */
  static int deregistr(IPACM_Listener *param);


private:
	int create_iface_instance(ipacm_ifacemgr_data *);

    /* api to register instances */
	int registr(int ipa_if_index, IPACM_Listener *obj);

	int SearchInstance(int ipa_if_index);

	static iface_instances *head;

};

#endif /* IPACM_IFACEMANAGER_H */
