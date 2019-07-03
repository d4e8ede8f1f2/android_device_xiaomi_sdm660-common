
#define LOG_TAG "android.hardware.power@1.3-service.sdm660-libperfmgr"

#include <android/log.h>
#include <hidl/HidlTransportSupport.h>

#include "Power.h"

using android::sp;
using android::status_t;
using android::OK;

using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

using android::hardware::power::V1_3::IPower;
using android::hardware::power::V1_3::implementation::Power;

int main(int /* argc */, char** /* argv */) {
    ALOGI("Power HAL is starting");

    android::sp<IPower> service = new Power();
    if (service == nullptr) {
        ALOGE("Could not create an instance of Power HAL");
        return 1;
    }

    configureRpcThreadpool(1, true /* callerWillJoi n*/);

    status_t status = service->registerAsService();
    if (status != OK) {
        ALOGE("Could not register Power HAL service");
        return 1;
    }

    ALOGI("Power HAL service is ready");
    joinRpcThreadpool();

    // In normal operation, we don't expect the thread pool to exit
    ALOGE("Power HAL service is shutting down");
    return 1;
}
