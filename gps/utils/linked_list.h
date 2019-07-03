
#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdbool.h>
#include <stdlib.h>

/** Linked List Return Codes */
typedef enum
{
  eLINKED_LIST_SUCCESS                             = 0,
     /**< Request was successful. */
  eLINKED_LIST_FAILURE_GENERAL                     = -1,
     /**< Failed because of a general failure. */
  eLINKED_LIST_INVALID_PARAMETER                   = -2,
     /**< Failed because the request contained invalid parameters. */
  eLINKED_LIST_INVALID_HANDLE                      = -3,
     /**< Failed because an invalid handle was specified. */
  eLINKED_LIST_UNAVAILABLE_RESOURCE                = -4,
     /**< Failed because an there were not enough resources. */
  eLINKED_LIST_INSUFFICIENT_BUFFER                 = -5,
     /**< Failed because an the supplied buffer was too small. */
}linked_list_err_type;

/*===========================================================================
FUNCTION    linked_list_init

DESCRIPTION
   Initializes internal structures for linked list.

   list_data: State of list to be initialized.

DEPENDENCIES
   N/A

RETURN VALUE
   Look at error codes above.

SIDE EFFECTS
   N/A

===========================================================================*/
linked_list_err_type linked_list_init(void** list_data);

/*===========================================================================
FUNCTION    linked_list_destroy

DESCRIPTION
   Destroys internal structures for linked list.

   p_list_data: State of list to be destroyed.

DEPENDENCIES
   N/A

RETURN VALUE
   Look at error codes above.

SIDE EFFECTS
   N/A

===========================================================================*/
linked_list_err_type linked_list_destroy(void** list_data);

/*===========================================================================
FUNCTION    linked_list_add

DESCRIPTION
   Adds an element to the head of the linked list. The passed in data pointer
   is not modified or freed. Passed in data_obj is expected to live throughout
   the use of the linked_list (i.e. data is not allocated internally)

   p_list_data:  List to add data to the head of.
   data_obj:     Pointer to data to add into list
   dealloc:      Function used to deallocate memory for this element. Pass NULL
                 if you do not want data deallocated during a flush operation

DEPENDENCIES
   N/A

RETURN VALUE
   Look at error codes above.

SIDE EFFECTS
   N/A

===========================================================================*/
linked_list_err_type linked_list_add(void* list_data, void *data_obj, void (*dealloc)(void*));

/*===========================================================================
FUNCTION    linked_list_remove

DESCRIPTION
   Retrieves data from the list tail. data_obj is the tail element from the list
   passed in by linked_list_add.

   p_list_data:  List to remove the tail from.
   data_obj:     Pointer to data removed from list

DEPENDENCIES
   N/A

RETURN VALUE
   Look at error codes above.

SIDE EFFECTS
   N/A

===========================================================================*/
linked_list_err_type linked_list_remove(void* list_data, void **data_obj);

/*===========================================================================
FUNCTION    linked_list_empty

DESCRIPTION
   Tells whether the list currently contains any elements

   p_list_data:  List to check if empty.

DEPENDENCIES
   N/A

RETURN VALUE
   0/FALSE : List contains elements
   1/TRUE  : List is Empty
   Otherwise look at error codes above.

SIDE EFFECTS
   N/A

===========================================================================*/
int linked_list_empty(void* list_data);

/*===========================================================================
FUNCTION    linked_list_flush

DESCRIPTION
   Removes all elements from the list and deallocates them using the provided
   dealloc function while adding elements.

   p_list_data:  List to remove all elements from.

DEPENDENCIES
   N/A

RETURN VALUE
   Look at error codes above.

SIDE EFFECTS
   N/A

===========================================================================*/
linked_list_err_type linked_list_flush(void* list_data);

/*===========================================================================
FUNCTION    linked_list_search

DESCRIPTION
   Searches for an element in the linked list.

   p_list_data:  List handle.
   data_p:       to be stored with the data found; NUll if no match.
                 if data_p passed in as NULL, then no write to it.
   equal:        Function ptr takes in a list element, and returns
                 indication if this the one looking for.
   data_0:       The data being compared against.
   rm_if_found:  Should data be removed if found?

DEPENDENCIES
   N/A

RETURN VALUE
   Look at error codes above.

SIDE EFFECTS
   N/A

===========================================================================*/
linked_list_err_type linked_list_search(void* list_data, void **data_p,
                                        bool (*equal)(void* data_0, void* data),
                                        void* data_0, bool rm_if_found);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __LINKED_LIST_H__ */
