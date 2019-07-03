
#define LOG_TAG "LocSvc_GnssMeasurementInterface"

#include <log_util.h>
#include <MeasurementAPIClient.h>
#include "GnssMeasurement.h"

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

void GnssMeasurement::GnssMeasurementDeathRecipient::serviceDied(
        uint64_t cookie, const wp<IBase>& who) {
    LOC_LOGE("%s] service died. cookie: %llu, who: %p",
            __FUNCTION__, static_cast<unsigned long long>(cookie), &who);
    if (mGnssMeasurement != nullptr) {
        mGnssMeasurement->close();
    }
}

GnssMeasurement::GnssMeasurement() {
    mGnssMeasurementDeathRecipient = new GnssMeasurementDeathRecipient(this);
    mApi = new MeasurementAPIClient();
}

GnssMeasurement::~GnssMeasurement() {
    if (mApi) {
        delete mApi;
        mApi = nullptr;
    }
}

// Methods from ::android::hardware::gnss::V1_0::IGnssMeasurement follow.

Return<IGnssMeasurement::GnssMeasurementStatus> GnssMeasurement::setCallback(
        const sp<V1_0::IGnssMeasurementCallback>& callback)  {

    Return<IGnssMeasurement::GnssMeasurementStatus> ret =
        IGnssMeasurement::GnssMeasurementStatus::ERROR_GENERIC;
    if (mGnssMeasurementCbIface != nullptr) {
        LOC_LOGE("%s]: GnssMeasurementCallback is already set", __FUNCTION__);
        return IGnssMeasurement::GnssMeasurementStatus::ERROR_ALREADY_INIT;
    }

    if (callback == nullptr) {
        LOC_LOGE("%s]: callback is nullptr", __FUNCTION__);
        return ret;
    }
    if (mApi == nullptr) {
        LOC_LOGE("%s]: mApi is nullptr", __FUNCTION__);
        return ret;
    }

    mGnssMeasurementCbIface = callback;
    mGnssMeasurementCbIface->linkToDeath(mGnssMeasurementDeathRecipient, 0);

    return mApi->measurementSetCallback(callback);

}

Return<void> GnssMeasurement::close()  {
    if (mApi == nullptr) {
        LOC_LOGE("%s]: mApi is nullptr", __FUNCTION__);
        return Void();
    }

    if (mGnssMeasurementCbIface != nullptr) {
        mGnssMeasurementCbIface->unlinkToDeath(mGnssMeasurementDeathRecipient);
        mGnssMeasurementCbIface = nullptr;
    }
    mApi->measurementClose();

    return Void();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
