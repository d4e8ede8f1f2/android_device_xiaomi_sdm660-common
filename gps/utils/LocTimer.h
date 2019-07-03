
#ifndef __LOC_TIMER_CPP_H__
#define __LOC_TIMER_CPP_H__

#include <stddef.h>
#include <loc_pla.h>

// opaque class to provide service implementation.
class LocTimerDelegate;
class LocSharedLock;

// LocTimer client must extend this class and implementthe callback.
// start() / stop() methods are to arm / disarm timer.
class LocTimer
{
    LocTimerDelegate* mTimer;
    LocSharedLock* mLock;
    // don't really want mLock to be manipulated by clients, yet LocTimer
    // has to have a reference to the lock so that the delete of LocTimer
    // and LocTimerDelegate can work together on their share resources.
    friend class LocTimerDelegate;

public:
    LocTimer();
    virtual ~LocTimer();

    // timeOutInMs:  timeout delay in ms
    // wakeOnExpire: true if to wake up CPU (if sleeping) upon timer
    //                        expiration and notify the client.
    //               false if to wait until next time CPU wakes up (if
    //                        sleeping) and then notify the client.
    // return:       true on success;
    //               false on failure, e.g. timer is already running.
    bool start(uint32_t timeOutInMs, bool wakeOnExpire);

    // return:       true on success;
    //               false on failure, e.g. timer is not running.
    bool stop();

    //  LocTimer client Should implement this method.
    //  This method is used for timeout calling back to client. This method
    //  should be short enough (eg: send a message to your own thread).
    virtual void timeOutCallback() = 0;
};

#endif //__LOC_DELAY_H__
