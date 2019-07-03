
#ifndef MEASUREMENT_API_CLINET_H
#define MEASUREMENT_API_CLINET_H

#include <mutex>
#include <android/hardware/gnss/1.0/IGnssMeasurement.h>
#include <android/hardware/gnss/1.0/IGnssMeasurementCallback.h>
#include <LocationAPIClientBase.h>
#include <hidl/Status.h>

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

using ::android::sp;

class MeasurementAPIClient : public LocationAPIClientBase
{
public:
    MeasurementAPIClient();
    virtual ~MeasurementAPIClient();
    MeasurementAPIClient(const MeasurementAPIClient&) = delete;
    MeasurementAPIClient& operator=(const MeasurementAPIClient&) = delete;

    // for GpsMeasurementInterface
    Return<V1_0::IGnssMeasurement::GnssMeasurementStatus> measurementSetCallback(
            const sp<V1_0::IGnssMeasurementCallback>& callback);
    void measurementClose();
    Return<IGnssMeasurement::GnssMeasurementStatus> startTracking();

    // callbacks we are interested in
    void onGnssMeasurementsCb(GnssMeasurementsNotification gnssMeasurementsNotification) final;

private:
    std::mutex mMutex;
    sp<V1_0::IGnssMeasurementCallback> mGnssMeasurementCbIface;

    bool mTracking;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
#endif // MEASUREMENT_API_CLINET_H
