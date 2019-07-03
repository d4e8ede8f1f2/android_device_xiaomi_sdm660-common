
#define LOG_NDEBUG 0
#define LOG_TAG "LocSvc_DualCtx"

#include <cutils/sched_policy.h>
#include <unistd.h>
#include <LocDualContext.h>
#include <msg_q.h>
#include <log_util.h>
#include <loc_log.h>

namespace loc_core {

// nothing exclude for foreground
const LOC_API_ADAPTER_EVENT_MASK_T
LocDualContext::mFgExclMask = 0;
// excluded events for background clients
const LOC_API_ADAPTER_EVENT_MASK_T
LocDualContext::mBgExclMask =
    (LOC_API_ADAPTER_BIT_SATELLITE_REPORT |
     LOC_API_ADAPTER_BIT_NMEA_1HZ_REPORT |
     LOC_API_ADAPTER_BIT_NMEA_POSITION_REPORT |
     LOC_API_ADAPTER_BIT_IOCTL_REPORT |
     LOC_API_ADAPTER_BIT_STATUS_REPORT |
     LOC_API_ADAPTER_BIT_GEOFENCE_GEN_ALERT |
     LOC_API_ADAPTER_BIT_GNSS_MEASUREMENT);

const MsgTask* LocDualContext::mMsgTask = NULL;
ContextBase* LocDualContext::mFgContext = NULL;
ContextBase* LocDualContext::mBgContext = NULL;
ContextBase* LocDualContext::mInjectContext = NULL;
// the name must be shorter than 15 chars
const char* LocDualContext::mLocationHalName = "Loc_hal_worker";
#ifndef USE_GLIB
const char* LocDualContext::mLBSLibName = "liblbs_core.so";
#else
const char* LocDualContext::mLBSLibName = "liblbs_core.so.1";
#endif

pthread_mutex_t LocDualContext::mGetLocContextMutex = PTHREAD_MUTEX_INITIALIZER;

const MsgTask* LocDualContext::getMsgTask(LocThread::tCreate tCreator,
                                          const char* name, bool joinable)
{
    if (NULL == mMsgTask) {
        mMsgTask = new MsgTask(tCreator, name, joinable);
    }
    return mMsgTask;
}

inline
const MsgTask* LocDualContext::getMsgTask(const char* name, bool joinable) {
    return getMsgTask((LocThread::tCreate)NULL, name, joinable);
}

ContextBase* LocDualContext::getLocFgContext(LocThread::tCreate tCreator,
            LocMsg* firstMsg, const char* name, bool joinable)
{
    pthread_mutex_lock(&LocDualContext::mGetLocContextMutex);
    LOC_LOGD("%s:%d]: querying ContextBase with tCreator", __func__, __LINE__);
    if (NULL == mFgContext) {
        LOC_LOGD("%s:%d]: creating msgTask with tCreator", __func__, __LINE__);
        const MsgTask* msgTask = getMsgTask(tCreator, name, joinable);
        mFgContext = new LocDualContext(msgTask,
                                        mFgExclMask);
    }
    if(NULL == mInjectContext) {
        LOC_LOGD("%s:%d]: mInjectContext is FgContext", __func__, __LINE__);
        mInjectContext = mFgContext;
        injectFeatureConfig(mInjectContext);
    }
    pthread_mutex_unlock(&LocDualContext::mGetLocContextMutex);

    if (firstMsg) {
        mFgContext->sendMsg(firstMsg);
    }

    return mFgContext;
}

ContextBase* LocDualContext::getLocBgContext(LocThread::tCreate tCreator,
            LocMsg* firstMsg, const char* name, bool joinable)
{
    pthread_mutex_lock(&LocDualContext::mGetLocContextMutex);
    LOC_LOGD("%s:%d]: querying ContextBase with tCreator", __func__, __LINE__);
    if (NULL == mBgContext) {
        LOC_LOGD("%s:%d]: creating msgTask with tCreator", __func__, __LINE__);
        const MsgTask* msgTask = getMsgTask(tCreator, name, joinable);
        mBgContext = new LocDualContext(msgTask,
                                        mBgExclMask);
    }
    if(NULL == mInjectContext) {
        LOC_LOGD("%s:%d]: mInjectContext is BgContext", __func__, __LINE__);
        mInjectContext = mBgContext;
        injectFeatureConfig(mInjectContext);
    }
    pthread_mutex_unlock(&LocDualContext::mGetLocContextMutex);

    if (firstMsg) {
        mBgContext->sendMsg(firstMsg);
    }

    return mBgContext;
}

void LocDualContext :: injectFeatureConfig(ContextBase *curContext)
{
    LOC_LOGD("%s:%d]: Enter", __func__, __LINE__);
    if(curContext == mInjectContext) {
        LOC_LOGD("%s:%d]: Calling LBSProxy (%p) to inject feature config",
                 __func__, __LINE__, ((LocDualContext *)mInjectContext)->mLBSProxy);
        ((LocDualContext *)mInjectContext)->mLBSProxy->injectFeatureConfig(curContext);
    }
    LOC_LOGD("%s:%d]: Exit", __func__, __LINE__);
}

LocDualContext::LocDualContext(const MsgTask* msgTask,
                               LOC_API_ADAPTER_EVENT_MASK_T exMask) :
    ContextBase(msgTask, exMask, mLBSLibName)
{
}

}
