
#ifndef IPACM_EvtDispatcher_H
#define IPACM_EvtDispatcher_H

#include <stdio.h>
#include <IPACM_CmdQueue.h>
#include "IPACM_Defs.h"
#include "IPACM_Listener.h"

/* queue */
typedef struct _cmd_evts
{
	ipa_cm_event_id event;
	IPACM_Listener *obj;
	//int ipa_interface_index;
	_cmd_evts *next;
}  cmd_evts;



class IPACM_EvtDispatcher
{
public:

	/* api for all iface instances to register events */
	static int registr(ipa_cm_event_id event, IPACM_Listener *obj);

	/* api for all iface instances to de-register events */
	static int deregistr(IPACM_Listener *obj);

	static int PostEvt(ipacm_cmd_q_data *);
	static void ProcessEvt(ipacm_cmd_q_data *);

private:
	static cmd_evts *head;
};

#endif /* IPACM_EvtDispatcher_H */
