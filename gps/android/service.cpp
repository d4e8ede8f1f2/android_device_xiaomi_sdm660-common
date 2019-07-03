
#define LOG_TAG "android.hardware.gnss@1.1-service-qti"

#include <android/hardware/gnss/1.1/IGnss.h>
#include <hidl/LegacySupport.h>

using android::hardware::gnss::V1_0::IGnss;
using android::hardware::defaultPassthroughServiceImplementation;

int main() {
    return defaultPassthroughServiceImplementation<IGnss>();
}
