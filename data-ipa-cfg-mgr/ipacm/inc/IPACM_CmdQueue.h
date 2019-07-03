
#ifndef IPA_CONNTRACK_MESSAGE_H
#define IPA_CONNTRACK_MESSAGE_H

#include <pthread.h>
#include "IPACM_Defs.h"

/*---------------------------------------------------------------------------
	 Event data required by IPA_CM
---------------------------------------------------------------------------*/

typedef struct _ipacm_cmd_q_data {
	ipa_cm_event_id event;
	void *evt_data;
}ipacm_cmd_q_data;

typedef struct cmd_s
{
	void (*callback_ptr)(ipacm_cmd_q_data *);
	ipacm_cmd_q_data data;
}cmd_t;

class Message
{
private:
	Message *m_next;

public:
	cmd_t evt;

	Message()
	{
		m_next = NULL;
		evt.callback_ptr = NULL;
	}
	~Message() { }
	void setnext(Message *item) { m_next = item; }
	Message* getnext()       { return m_next; }
};

class MessageQueue
{

private:
	Message *Head;
	Message *Tail;
	Message* dequeue(void);
	static MessageQueue *inst_internal;
	static MessageQueue *inst_external;

	MessageQueue()
	{
		Head = NULL;
		Tail = NULL;
	}

public:

	~MessageQueue() { }
	void enqueue(Message *item);

	static void* Process(void *);
	static MessageQueue* getInstanceInternal();
	static MessageQueue* getInstanceExternal();

};

#endif  /* IPA_CONNTRACK_MESSAGE_H */
