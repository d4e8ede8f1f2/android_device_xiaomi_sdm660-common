
#include <string.h>
#include <pthread.h>
#include <IPACM_EvtDispatcher.h>
#include <IPACM_Neighbor.h>
#include "IPACM_CmdQueue.h"
#include "IPACM_Defs.h"


extern pthread_mutex_t mutex;
extern pthread_cond_t  cond_var;

cmd_evts *IPACM_EvtDispatcher::head = NULL;
extern uint32_t ipacm_event_stats[IPACM_EVENT_MAX];

int IPACM_EvtDispatcher::PostEvt
(
	 ipacm_cmd_q_data *data
)
{
	Message *item = NULL;
	MessageQueue *MsgQueue = NULL;

	if(data->event < IPA_EXTERNAL_EVENT_MAX)
	{
		IPACMDBG("Insert event into external queue.\n");
		MsgQueue = MessageQueue::getInstanceExternal();
	}
	else
	{
		IPACMDBG("Insert event into internal queue.\n");
		MsgQueue = MessageQueue::getInstanceInternal();
	}
	if(MsgQueue == NULL)
	{
		IPACMERR("unable to retrieve MsgQueue instance\n");
		return IPACM_FAILURE;
	}

	item = new Message();
	if(item == NULL)
	{
		IPACMERR("unable to create new message item\n");
		return IPACM_FAILURE;
	}

	item->evt.callback_ptr = IPACM_EvtDispatcher::ProcessEvt;
	memcpy(&item->evt.data, data, sizeof(ipacm_cmd_q_data));

	if(pthread_mutex_lock(&mutex) != 0)
	{
		IPACMERR("unable to lock the mutex\n");
		return IPACM_FAILURE;
	}

	IPACMDBG("Enqueing item\n");
	MsgQueue->enqueue(item);
	IPACMDBG("Enqueued item %pK\n", item);

	if(pthread_cond_signal(&cond_var) != 0)
	{
		IPACMDBG("unable to lock the mutex\n");
		/* Release the mutex before you return failure */
		if(pthread_mutex_unlock(&mutex) != 0)
		{
			IPACMERR("unable to unlock the mutex\n");
			return IPACM_FAILURE;
		}
		return IPACM_FAILURE;
	}

	if(pthread_mutex_unlock(&mutex) != 0)
	{
		IPACMERR("unable to unlock the mutex\n");
		return IPACM_FAILURE;
	}

	return IPACM_SUCCESS;
}

void IPACM_EvtDispatcher::ProcessEvt(ipacm_cmd_q_data *data)
{

	cmd_evts *tmp = head, tmp1;

	if(head == NULL)
	{
		IPACMDBG("Queue is empty\n");
	}

	while(tmp != NULL)
	{
	        memcpy(&tmp1, tmp, sizeof(tmp1));
		if(data->event == tmp1.event)
		{
			ipacm_event_stats[data->event]++;
			tmp1.obj->event_callback(data->event, data->evt_data);
			IPACMDBG(" Find matched registered events\n");
		}
	        tmp = tmp1.next;
	}

	IPACMDBG(" Finished process events\n");
			
	if(data->evt_data != NULL)
	{
		IPACMDBG("free the event:%d data: %pK\n", data->event, data->evt_data);
		free(data->evt_data);
	}
	return;
}

int IPACM_EvtDispatcher::registr(ipa_cm_event_id event, IPACM_Listener *obj)
{
	cmd_evts *tmp = head,*nw;

	nw = (cmd_evts *)malloc(sizeof(cmd_evts));
	if(nw != NULL)
	{
		nw->event = event;
		nw->obj = obj;
		nw->next = NULL;
	}
	else
	{
		return IPACM_FAILURE;
	}

	if(head == NULL)
	{
		head = nw;
	}
	else
	{
		while(tmp->next)
		{
			tmp = tmp->next;
		}
		tmp->next = nw;
	}
	return IPACM_SUCCESS;
}


int IPACM_EvtDispatcher::deregistr(IPACM_Listener *param)
{
	cmd_evts *tmp = head,*tmp1,*prev = head;

	while(tmp != NULL)
	{
		if(tmp->obj == param)
		{
			tmp1 = tmp;
			if(tmp == head)
			{
				head = head->next;
			}
			else if(tmp->next == NULL)
			{
				prev->next = NULL;
			}
			else
			{
				prev->next = tmp->next;
			}

			tmp = tmp->next;
			free(tmp1);
		}
		else
		{
			prev = tmp;
			tmp = tmp->next;
		}
	}
	return IPACM_SUCCESS;
}
