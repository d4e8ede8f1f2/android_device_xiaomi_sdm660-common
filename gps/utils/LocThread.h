
#ifndef __LOC_THREAD__
#define __LOC_THREAD__

#include <stddef.h>
#include <pthread.h>

// abstract class to be implemented by client to provide a runnable class
// which gets scheduled by LocThread
class LocRunnable {
public:
    inline LocRunnable() {}
    inline virtual ~LocRunnable() {}

    // The method to be implemented by thread clients
    // and be scheduled by LocThread
    // This method will be repeated called until it returns false; or
    // until thread is stopped.
    virtual bool run() = 0;

    // The method to be run before thread loop (conditionally repeatedly)
    // calls run()
    inline virtual void prerun() {}

    // The method to be run after thread loop (conditionally repeatedly)
    // calls run()
    inline virtual void postrun() {}
};

// opaque class to provide service implementation.
class LocThreadDelegate;

// A utility class to create a thread and run LocRunnable
// caller passes in.
class LocThread {
    LocThreadDelegate* mThread;
public:
    inline LocThread() : mThread(NULL) {}
    virtual ~LocThread();

    typedef pthread_t (*tCreate)(const char* name, void* (*start)(void*), void* arg);
    // client starts thread with a runnable, which implements
    // the logics to fun in the created thread context.
    // The thread could be either joinable or detached.
    // runnable is an obj managed by client. Client creates and
    //          frees it (but must be after stop() is called, or
    //          this LocThread obj is deleted).
    //          The obj will be deleted by LocThread if start()
    //          returns true. Else it is client's responsibility
    //          to delete the object
    // Returns 0 if success; false if failure.
    bool start(tCreate creator, const char* threadName, LocRunnable* runnable, bool joinable = true);
    inline bool start(const char* threadName, LocRunnable* runnable, bool joinable = true) {
        return start(NULL, threadName, runnable, joinable);
    }

    // NOTE: if this is a joinable thread, this stop may block
    // for a while until the thread is joined.
    void stop();

    // thread status check
    inline bool isRunning() { return NULL != mThread; }
};

#endif //__LOC_THREAD__
