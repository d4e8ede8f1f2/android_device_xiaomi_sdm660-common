
#define LOG_TAG "LocSvc_utils_ll"

#include "linked_list.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <loc_pla.h>
#include <log_util.h>

typedef struct list_element {
   struct list_element* next;
   struct list_element* prev;
   void* data_ptr;
   void (*dealloc_func)(void*);
}list_element;

typedef struct list_state {
   list_element* p_head;
   list_element* p_tail;
} list_state;

/* ----------------------- END INTERNAL FUNCTIONS ---------------------------------------- */

/*===========================================================================

  FUNCTION:   linked_list_init

  ===========================================================================*/
linked_list_err_type linked_list_init(void** list_data)
{
   if( list_data == NULL )
   {
      LOC_LOGE("%s: Invalid list parameter!\n", __FUNCTION__);
      return eLINKED_LIST_INVALID_PARAMETER;
   }

   list_state* tmp_list;
   tmp_list = (list_state*)calloc(1, sizeof(list_state));
   if( tmp_list == NULL )
   {
      LOC_LOGE("%s: Unable to allocate space for list!\n", __FUNCTION__);
      return eLINKED_LIST_FAILURE_GENERAL;
   }

   tmp_list->p_head = NULL;
   tmp_list->p_tail = NULL;

   *list_data = tmp_list;

   return eLINKED_LIST_SUCCESS;
}

/*===========================================================================

  FUNCTION:   linked_list_destroy

  ===========================================================================*/
linked_list_err_type linked_list_destroy(void** list_data)
{
   if( list_data == NULL )
   {
      LOC_LOGE("%s: Invalid list parameter!\n", __FUNCTION__);
      return eLINKED_LIST_INVALID_HANDLE;
   }

   list_state* p_list = (list_state*)*list_data;

   linked_list_flush(p_list);

   free(*list_data);
   *list_data = NULL;

   return eLINKED_LIST_SUCCESS;
}

/*===========================================================================

  FUNCTION:   linked_list_add

  ===========================================================================*/
linked_list_err_type linked_list_add(void* list_data, void *data_obj, void (*dealloc)(void*))
{
   if( list_data == NULL )
   {
      LOC_LOGE("%s: Invalid list parameter!\n", __FUNCTION__);
      return eLINKED_LIST_INVALID_HANDLE;
   }

   if( data_obj == NULL )
   {
      LOC_LOGE("%s: Invalid input parameter!\n", __FUNCTION__);
      return eLINKED_LIST_INVALID_PARAMETER;
   }

   list_state* p_list = (list_state*)list_data;
   list_element* elem = (list_element*)malloc(sizeof(list_element));
   if( elem == NULL )
   {
      LOC_LOGE("%s: Memory allocation failed\n", __FUNCTION__);
      return eLINKED_LIST_FAILURE_GENERAL;
   }

   /* Copy data to newly created element */
   elem->data_ptr = data_obj;
   elem->next = NULL;
   elem->prev = NULL;
   elem->dealloc_func = dealloc;

   /* Replace head element */
   list_element* tmp = p_list->p_head;
   p_list->p_head = elem;
   /* Point next to the previous head element */
   p_list->p_head->next = tmp;

   if( tmp != NULL )
   {
      tmp->prev = p_list->p_head;
   }
   else
   {
      p_list->p_tail = p_list->p_head;
   }

   return eLINKED_LIST_SUCCESS;
}

/*===========================================================================

  FUNCTION:   linked_list_remove

  ===========================================================================*/
linked_list_err_type linked_list_remove(void* list_data, void **data_obj)
{
   if( list_data == NULL )
   {
      LOC_LOGE("%s: Invalid list parameter!\n", __FUNCTION__);
      return eLINKED_LIST_INVALID_HANDLE;
   }

   if( data_obj == NULL )
   {
      LOC_LOGE("%s: Invalid input parameter!\n", __FUNCTION__);
      return eLINKED_LIST_INVALID_PARAMETER;
   }

   list_state* p_list = (list_state*)list_data;
   if( p_list->p_tail == NULL )
   {
      return eLINKED_LIST_UNAVAILABLE_RESOURCE;
   }

   list_element* tmp = p_list->p_tail;

   /* Replace tail element */
   p_list->p_tail = tmp->prev;

   if( p_list->p_tail != NULL )
   {
      p_list->p_tail->next = NULL;
   }
   else
   {
      p_list->p_head = p_list->p_tail;
   }

   /* Copy data to output param */
   *data_obj = tmp->data_ptr;

   /* Free allocated list element */
   free(tmp);

   return eLINKED_LIST_SUCCESS;
}

/*===========================================================================

  FUNCTION:   linked_list_empty

  ===========================================================================*/
int linked_list_empty(void* list_data)
{
   if( list_data == NULL )
   {
      LOC_LOGE("%s: Invalid list parameter!\n", __FUNCTION__);
      return (int)eLINKED_LIST_INVALID_HANDLE;
   }
   else
   {
      list_state* p_list = (list_state*)list_data;
      return p_list->p_head == NULL ? 1 : 0;
   }
}

/*===========================================================================

  FUNCTION:   linked_list_flush

  ===========================================================================*/
linked_list_err_type linked_list_flush(void* list_data)
{
   if( list_data == NULL )
   {
      LOC_LOGE("%s: Invalid list parameter!\n", __FUNCTION__);
      return eLINKED_LIST_INVALID_HANDLE;
   }

   list_state* p_list = (list_state*)list_data;

   /* Remove all dynamically allocated elements */
   while( p_list->p_head != NULL )
   {
      list_element* tmp = p_list->p_head->next;

      /* Free data pointer if told to do so. */
      if( p_list->p_head->dealloc_func != NULL )
      {
         p_list->p_head->dealloc_func(p_list->p_head->data_ptr);
      }

      /* Free list element */
      free(p_list->p_head);

      p_list->p_head = tmp;
   }

   p_list->p_tail = NULL;

   return eLINKED_LIST_SUCCESS;
}

/*===========================================================================

  FUNCTION:   linked_list_search

  ===========================================================================*/
linked_list_err_type linked_list_search(void* list_data, void **data_p,
                                        bool (*equal)(void* data_0, void* data),
                                        void* data_0, bool rm_if_found)
{
   if( list_data == NULL || NULL == equal )
   {
      LOC_LOGE("%s: Invalid list parameter! list_data %p equal %p\n",
               __FUNCTION__, list_data, equal);
      return eLINKED_LIST_INVALID_HANDLE;
   }

   list_state* p_list = (list_state*)list_data;
   if( p_list->p_tail == NULL )
   {
      return eLINKED_LIST_UNAVAILABLE_RESOURCE;
   }

   list_element* tmp = p_list->p_head;

   if (NULL != data_p) {
     *data_p = NULL;
   }

   while (NULL != tmp) {
     if ((*equal)(data_0, tmp->data_ptr)) {
       if (NULL != data_p) {
         *data_p = tmp->data_ptr;
       }

       if (rm_if_found) {
         if (NULL == tmp->prev) {
           p_list->p_head = tmp->next;
         } else {
           tmp->prev->next = tmp->next;
         }

         if (NULL == tmp->next) {
           p_list->p_tail = tmp->prev;
         } else {
           tmp->next->prev = tmp->prev;
         }

         tmp->prev = tmp->next = NULL;

         // dealloc data if it is not copied out && caller
         // has given us a dealloc function pointer.
         if (NULL == data_p && NULL != tmp->dealloc_func) {
             tmp->dealloc_func(tmp->data_ptr);
         }
         free(tmp);
       }

       tmp = NULL;
     } else {
       tmp = tmp->next;
     }
   }

   return eLINKED_LIST_SUCCESS;
}
