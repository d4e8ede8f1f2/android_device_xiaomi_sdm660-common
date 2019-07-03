
#ifndef __MSG_Q_H__
#define __MSG_Q_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdlib.h>

/** Linked List Return Codes */
typedef enum
{
  eMSG_Q_SUCCESS                             = 0,
     /**< Request was successful. */
  eMSG_Q_FAILURE_GENERAL                     = -1,
     /**< Failed because of a general failure. */
  eMSG_Q_INVALID_PARAMETER                   = -2,
     /**< Failed because the request contained invalid parameters. */
  eMSG_Q_INVALID_HANDLE                      = -3,
     /**< Failed because an invalid handle was specified. */
  eMSG_Q_UNAVAILABLE_RESOURCE                = -4,
     /**< Failed because an there were not enough resources. */
  eMSG_Q_INSUFFICIENT_BUFFER                 = -5,
     /**< Failed because an the supplied buffer was too small. */
}msq_q_err_type;

/*===========================================================================
FUNCTION    msg_q_init

DESCRIPTION
   Initializes internal structures for message queue.

   msg_q_data: pointer to an opaque Q handle to be returned; NULL if fails

DEPENDENCIES
   N/A

RETURN VALUE
   Look at error codes above.

SIDE EFFECTS
   N/A

===========================================================================*/
msq_q_err_type msg_q_init(void** msg_q_data);

/*===========================================================================
FUNCTION    msg_q_init2

DESCRIPTION
   Initializes internal structures for message queue.

DEPENDENCIES
   N/A

RETURN VALUE
   opaque handle to the Q created; NULL if create fails

SIDE EFFECTS
   N/A

===========================================================================*/
const void* msg_q_init2();

/*===========================================================================
FUNCTION    msg_q_destroy

DESCRIPTION
   Releases internal structures for message queue.

   msg_q_data: State of message queue to be released.

DEPENDENCIES
   N/A

RETURN VALUE
   Look at error codes above.

SIDE EFFECTS
   N/A

===========================================================================*/
msq_q_err_type msg_q_destroy(void** msg_q_data);

/*===========================================================================
FUNCTION    msg_q_snd

DESCRIPTION
   Sends data to the message queue. The passed in data pointer
   is not modified or freed. Passed in msg_obj is expected to live throughout
   the use of the msg_q (i.e. data is not allocated internally)

   msg_q_data: Message Queue to add the element to.
   msgp:       Pointer to data to add into message queue.
   dealloc:    Function used to deallocate memory for this element. Pass NULL
               if you do not want data deallocated during a flush operation

DEPENDENCIES
   N/A

RETURN VALUE
   Look at error codes above.

SIDE EFFECTS
   N/A

===========================================================================*/
msq_q_err_type msg_q_snd(void* msg_q_data, void* msg_obj, void (*dealloc)(void*));

/*===========================================================================
FUNCTION    msg_q_rcv

DESCRIPTION
   Retrieves data from the message queue. msg_obj is the oldest message received
   and pointer is simply removed from message queue.

   msg_q_data: Message Queue to copy data from into msgp.
   msg_obj:    Pointer to space to copy msg_q contents to.

DEPENDENCIES
   N/A

RETURN VALUE
   Look at error codes above.

SIDE EFFECTS
   N/A

===========================================================================*/
msq_q_err_type msg_q_rcv(void* msg_q_data, void** msg_obj);

/*===========================================================================
FUNCTION    msg_q_flush

DESCRIPTION
   Function removes all elements from the message queue.

   msg_q_data: Message Queue to remove elements from.

DEPENDENCIES
   N/A

RETURN VALUE
   Look at error codes above.

SIDE EFFECTS
   N/A

===========================================================================*/
msq_q_err_type msg_q_flush(void* msg_q_data);

/*===========================================================================
FUNCTION    msg_q_unblock

DESCRIPTION
   This function will stop use of the message queue. All waiters will wake up
   and likely receive nothing from the queue resulting in a negative return
   value. The message queue can no longer be used until it is destroyed
   and initialized again after calling this function.

   msg_q_data: Message queue to unblock.

DEPENDENCIES
   N/A

RETURN VALUE
   Look at error codes above.

SIDE EFFECTS
   N/A

===========================================================================*/
msq_q_err_type msg_q_unblock(void* msg_q_data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MSG_Q_H__ */
