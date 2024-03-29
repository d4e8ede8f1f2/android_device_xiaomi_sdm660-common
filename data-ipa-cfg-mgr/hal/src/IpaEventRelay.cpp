
#define LOG_TAG "IPAHALService/IpaEventRelay"
/* External Includes */
#include <log/log.h>

/* HIDL Includes */
#include <android/hardware/tetheroffload/control/1.0/ITetheringOffloadCallback.h>

/* Internal Includes */
#include "IpaEventRelay.h"

/* Namespace pollution avoidance */
using ::android::hardware::tetheroffload::control::V1_0::ITetheringOffloadCallback;
using ::android::hardware::tetheroffload::control::V1_0::OffloadCallbackEvent;


IpaEventRelay::IpaEventRelay(
        const ::android::sp<ITetheringOffloadCallback>& cb) : mFramework(cb) {
} /* IpaEventRelay */

void IpaEventRelay::onOffloadStarted() {
    ALOGI("onOffloadStarted()");
    auto ret = mFramework->onEvent(OffloadCallbackEvent::OFFLOAD_STARTED);
    if (!ret.isOk()) {
        ALOGE("Triggering OffloadStarted Callback failed.");
    }
} /* onOffloadStarted */

void IpaEventRelay::onOffloadStopped(StoppedReason reason) {
    ALOGI("onOffloadStopped(%d)", reason);
    if( reason == StoppedReason::REQUESTED ) {
        /*
         * No way to communicate this to Framework right now, they make an
         * assumption that offload is stopped when they remove the
         * configuration.
         */
    }
    else if ( reason == StoppedReason::ERROR ) {
        auto ret = mFramework->onEvent(OffloadCallbackEvent::OFFLOAD_STOPPED_ERROR);
        if (!ret.isOk()) {
            ALOGE("Triggering OffloadStopped Callback failed.");
        }
    }
    else if ( reason == StoppedReason::UNSUPPORTED ) {
        auto ret = mFramework->onEvent(OffloadCallbackEvent::OFFLOAD_STOPPED_UNSUPPORTED);
        if (!ret.isOk()) {
            ALOGE("Triggering OffloadStopped Callback failed.");
        }
    }
    else {
        ALOGE("Unknown stopped reason(%d)", reason);
    }
} /* onOffloadStopped */

void IpaEventRelay::onOffloadSupportAvailable() {
    ALOGI("onOffloadSupportAvailable()");
    auto ret = mFramework->onEvent(OffloadCallbackEvent::OFFLOAD_SUPPORT_AVAILABLE);
    if (!ret.isOk()) {
        ALOGE("Triggering OffloadSupportAvailable Callback failed.");
    }
} /* onOffloadSupportAvailable */

void IpaEventRelay::onLimitReached() {
    ALOGI("onLimitReached()");
    auto ret = mFramework->onEvent(OffloadCallbackEvent::OFFLOAD_STOPPED_LIMIT_REACHED);
    if (!ret.isOk()) {
        ALOGE("Triggering LimitReached Callback failed.");
    }
} /* onLimitReached */
