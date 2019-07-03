
#ifndef IPACM_LISTENER_H
#define IPACM_LISTENER_H

#include "IPACM_Defs.h"
#include "IPACM_CmdQueue.h"

/* abstract class notifier */
class IPACM_Listener
{
public:
	virtual void event_callback(ipa_cm_event_id event, void *data) = 0;
	virtual ~IPACM_Listener(void) {};
};

#endif /* IPACM_LISTENER_H */
