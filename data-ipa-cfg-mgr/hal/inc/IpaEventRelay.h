
#ifndef _IPA_EVENT_RELAY_H_
#define _IPA_EVENT_RELAY_H_
/* External Includes */
#include <hidl/HidlTransportSupport.h>

/* HIDL Includes */
#include <android/hardware/tetheroffload/control/1.0/ITetheringOffloadCallback.h>

/* Internal Includes */
#include "IOffloadManager.h"

/* Namespace pollution avoidance */
using ::android::hardware::tetheroffload::control::V1_0::ITetheringOffloadCallback;


class IpaEventRelay : public IOffloadManager::IpaEventListener {
public:
    IpaEventRelay(const ::android::sp<ITetheringOffloadCallback>& /* cb */);
    /* ----------------------- IPA EVENT LISTENER --------------------------- */
    void onOffloadStarted();
    void onOffloadStopped(StoppedReason /* reason */);
    void onOffloadSupportAvailable();
    void onLimitReached();
private:
    const ::android::sp<ITetheringOffloadCallback>& mFramework;
}; /* IpaEventRelay */
#endif /* _IPA_EVENT_RELAY_H_ */
