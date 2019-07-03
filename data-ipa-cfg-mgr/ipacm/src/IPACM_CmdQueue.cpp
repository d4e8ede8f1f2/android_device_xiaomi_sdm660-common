
#include <string.h>
#include "IPACM_CmdQueue.h"
#include "IPACM_Log.h"
#include "IPACM_Iface.h"

pthread_mutex_t mutex    = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond_var = PTHREAD_COND_INITIALIZER;

MessageQueue* MessageQueue::inst_internal = NULL;
MessageQueue* MessageQueue::inst_external = NULL;

MessageQueue* MessageQueue::getInstanceInternal()
{
	if(inst_internal == NULL)
	{
		inst_internal = new MessageQueue();
		if(inst_internal == NULL)
		{
			IPACMERR("unable to create internal Message Queue instance\n");
			return NULL;
		}
	}

	return inst_internal;
}

MessageQueue* MessageQueue::getInstanceExternal()
{
	if(inst_external == NULL)
	{
		inst_external = new MessageQueue();
		if(inst_external == NULL)
		{
			IPACMERR("unable to create external Message Queue instance\n");
			return NULL;
		}
	}

	return inst_external;
}

void MessageQueue::enqueue(Message *item)
{
	if(!Head)
	{
		Tail = item;
		Head = item;
	}
	else
	{
		if(Tail == NULL)
		{
			IPACMDBG("Tail is null\n");
			Head->setnext(item);
		}
		else
		{
			Tail->setnext(item);
		}
		Tail = item;
	}
}


Message* MessageQueue::dequeue(void)
{
	if(Head == NULL)
	{
		return NULL;
	}
	else
	{
		Message *tmp = Head;
		Head = Head->getnext();

		return tmp;
	}
}


void* MessageQueue::Process(void *param)
{
	MessageQueue *MsgQueueInternal = NULL;
	MessageQueue *MsgQueueExternal = NULL;
	Message *item = NULL;
	param = NULL;
	const char *eventName = NULL;

	IPACMDBG("MessageQueue::Process()\n");

	MsgQueueInternal = MessageQueue::getInstanceInternal();
	if(MsgQueueInternal == NULL)
	{
		IPACMERR("unable to start internal cmd queue process\n");
		return NULL;
	}

	MsgQueueExternal = MessageQueue::getInstanceExternal();
	if(MsgQueueExternal == NULL)
	{
		IPACMERR("unable to start external cmd queue process\n");
		return NULL;
	}

	while(1)
	{
		if(pthread_mutex_lock(&mutex) != 0)
		{
			IPACMERR("unable to lock the mutex\n");
			return NULL;
		}

		item = MsgQueueInternal->dequeue();
		if(item == NULL)
		{
			item = MsgQueueExternal->dequeue();
			if(item)
			{
				eventName = IPACM_Iface::ipacmcfg->getEventName(item->evt.data.event);
				if (eventName != NULL)
				{
					IPACMDBG("Get event %s from external queue.\n",
							eventName);
				}
			}
		}
		else
		{
			eventName = IPACM_Iface::ipacmcfg->getEventName(item->evt.data.event);
			if (eventName != NULL)
			{
				IPACMDBG("Get event %s from internal queue.\n",
					eventName);
			}
		}

		if(item == NULL)
		{
			IPACMDBG("Waiting for Message\n");

			if(pthread_cond_wait(&cond_var, &mutex) != 0)
			{
				IPACMERR("unable to lock the mutex\n");

				if(pthread_mutex_unlock(&mutex) != 0)
				{
					IPACMERR("unable to unlock the mutex\n");
					return NULL;
				}

				return NULL;
			}

			if(pthread_mutex_unlock(&mutex) != 0)
			{
				IPACMERR("unable to unlock the mutex\n");
				return NULL;
			}

		}
		else
		{
			if(pthread_mutex_unlock(&mutex) != 0)
			{
				IPACMERR("unable to unlock the mutex\n");
				return NULL;
			}

			IPACMDBG("Processing item %pK event ID: %d\n",item,item->evt.data.event);
			item->evt.callback_ptr(&item->evt.data);
			delete item;
			item = NULL;
		}

	} /* Go forever until a termination indication is received */

}
