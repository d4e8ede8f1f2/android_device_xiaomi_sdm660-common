
#ifndef __LOC_ENG_CONTEXT__
#define __LOC_ENG_CONTEXT__

#include <stdbool.h>
#include <ctype.h>
#include <dlfcn.h>
#include <ContextBase.h>

namespace loc_core {

class LocDualContext : public ContextBase {
    static const MsgTask* mMsgTask;
    static ContextBase* mFgContext;
    static ContextBase* mBgContext;
    static ContextBase* mInjectContext;
    static const MsgTask* getMsgTask(LocThread::tCreate tCreator,
                                     const char* name, bool joinable = true);
    static const MsgTask* getMsgTask(const char* name, bool joinable = true);
    static pthread_mutex_t mGetLocContextMutex;

protected:
    LocDualContext(const MsgTask* msgTask,
                   LOC_API_ADAPTER_EVENT_MASK_T exMask);
    inline virtual ~LocDualContext() {}

public:
    static const char* mLBSLibName;
    static const LOC_API_ADAPTER_EVENT_MASK_T mFgExclMask;
    static const LOC_API_ADAPTER_EVENT_MASK_T mBgExclMask;
    static const char* mLocationHalName;

    static ContextBase* getLocFgContext(LocThread::tCreate tCreator, LocMsg* firstMsg,
                                        const char* name, bool joinable = true);
    inline static ContextBase* getLocFgContext(const char* name, bool joinable = true) {
        return getLocFgContext(NULL, NULL, name, joinable);
    }
    static ContextBase* getLocBgContext(LocThread::tCreate tCreator, LocMsg* firstMsg,
                                        const char* name, bool joinable = true);
    inline static ContextBase* getLocBgContext(const char* name, bool joinable = true) {
        return getLocBgContext(NULL, NULL, name, joinable);
    }

    static void injectFeatureConfig(ContextBase *context);
};

}

#endif //__LOC_ENG_CONTEXT__
