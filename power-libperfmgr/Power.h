
#ifndef ANDROID_HARDWARE_POWER_V1_3_POWER_H
#define ANDROID_HARDWARE_POWER_V1_3_POWER_H

#include <atomic>
#include <thread>

#include <android/hardware/power/1.3/IPower.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <perfmgr/HintManager.h>

#include "InteractionHandler.h"

namespace android {
namespace hardware {
namespace power {
namespace V1_3 {
namespace implementation {

using ::android::hardware::power::V1_0::Feature;
using ::android::hardware::power::V1_3::IPower;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::InteractionHandler;
using PowerHint_1_0 = ::android::hardware::power::V1_0::PowerHint;
using PowerHint_1_2 = ::android::hardware::power::V1_2::PowerHint;
using PowerHint_1_3 = ::android::hardware::power::V1_3::PowerHint;
using ::android::perfmgr::HintManager;

constexpr char kPowerHalInitProp[] = "vendor.powerhal.init";

struct Power : public IPower {
    // Methods from ::android::hardware::power::V1_0::IPower follow.

    Power();

    Return<void> setInteractive(bool /* interactive */) override;
    Return<void> powerHint(PowerHint_1_0 hint, int32_t data) override;
    Return<void> setFeature(Feature feature, bool activate) override;
    Return<void> getPlatformLowPowerStats(getPlatformLowPowerStats_cb _hidl_cb) override;

    // Methods from ::android::hardware::power::V1_1::IPower follow.
    Return<void> getSubsystemLowPowerStats(getSubsystemLowPowerStats_cb _hidl_cb) override;
    Return<void> powerHintAsync(PowerHint_1_0 hint, int32_t data) override;

    // Methods from ::android::hardware::power::V1_2::IPower follow.
    Return<void> powerHintAsync_1_2(PowerHint_1_2 hint, int32_t data) override;

    // Methods from ::android::hardware::power::V1_3::IPower follow.
    Return<void> powerHintAsync_1_3(PowerHint_1_3 hint, int32_t data) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

 private:
    static bool isSupportedGovernor();

    std::shared_ptr<HintManager> mHintManager;
    std::unique_ptr<InteractionHandler> mInteractionHandler;
    std::atomic<bool> mSustainedPerfModeOn;
    std::atomic<bool> mReady;
    std::thread mInitThread;
};

}  // namespace implementation
}  // namespace V1_3
}  // namespace power
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_POWER_V1_3_POWER_H
