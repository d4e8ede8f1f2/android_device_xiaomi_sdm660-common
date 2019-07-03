
#ifndef __MSG_TASK__
#define __MSG_TASK__

#include <LocThread.h>

struct LocMsg {
    inline LocMsg() {}
    inline virtual ~LocMsg() {}
    virtual void proc() const = 0;
    inline virtual void log() const {}
};

class MsgTask : public LocRunnable {
    const void* mQ;
    LocThread* mThread;
    friend class LocThreadDelegate;
protected:
    virtual ~MsgTask();
public:
    MsgTask(LocThread::tCreate tCreator, const char* threadName = NULL, bool joinable = true);
    MsgTask(const char* threadName = NULL, bool joinable = true);
    // this obj will be deleted once thread is deleted
    void destroy();
    void sendMsg(const LocMsg* msg) const;
    // Overrides of LocRunnable methods
    // This method will be repeated called until it returns false; or
    // until thread is stopped.
    virtual bool run();

    // The method to be run before thread loop (conditionally repeatedly)
    // calls run()
    virtual void prerun();

    // The method to be run after thread loop (conditionally repeatedly)
    // calls run()
    inline virtual void postrun() {}
};

#endif //__MSG_TASK__
