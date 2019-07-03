
#ifndef __LOC_DELAY_H__
#define __LOC_DELAY_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include <stddef.h>
#include <stdint.h>
#include <loc_pla.h>
/*
    user_data: client context pointer, passthrough. Originally received
               from calling client when loc_timer_start() is called.
    result:    0 if timer successfully timed out; else timer failed.
*/
typedef void (*loc_timer_callback)(void *user_data, int32_t result);


/*
    delay_msec:         timeout value for the timer.
    cb_func:            callback function pointer, implemented by client.
                        Can not be NULL.
    user_data:          client context pointer, passthrough.  Will be
                        returned when loc_timer_callback() is called.
    wakeOnExpire:       true if to wake up CPU (if sleeping) upon timer
                                expiration and notify the client.
                        false if to wait until next time CPU wakes up (if
                                 sleeping) and then notify the client.
    Returns the handle, which can be used to stop the timer
                        NULL, if timer start fails (e.g. if cb_func is NULL).
*/
void* loc_timer_start(uint64_t delay_msec,
                      loc_timer_callback cb_func,
                      void *user_data,
                      bool wake_on_expire=false);

/*
    handle becomes invalid upon the return of the callback
*/
void loc_timer_stop(void*& handle);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__LOC_DELAY_H__
