
#ifndef ANDROID_HARDWARE_GNSS_V1_0_GNSSDEBUG_H
#define ANDROID_HARDWARE_GNSS_V1_0_GNSSDEBUG_H


#include <android/hardware/gnss/1.0/IGnssDebug.h>
#include <hidl/Status.h>

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

using ::android::hardware::gnss::V1_0::IGnssDebug;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::hidl_vec;
using ::android::hardware::hidl_string;
using ::android::sp;

/* Interface for GNSS Debug support. */
struct Gnss;
struct GnssDebug : public IGnssDebug {
    GnssDebug(Gnss* gnss);
    ~GnssDebug() {};

    /*
     * Methods from ::android::hardware::gnss::V1_0::IGnssDebug follow.
     * These declarations were generated from IGnssDebug.hal.
     */
    Return<void> getDebugData(getDebugData_cb _hidl_cb) override;

private:
    Gnss* mGnss = nullptr;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_GNSS_V1_0_GNSSDEBUG_H
