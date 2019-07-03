
#ifndef BATCHING_API_CLINET_H
#define BATCHING_API_CLINET_H

#include <android/hardware/gnss/1.0/IGnssBatching.h>
#include <android/hardware/gnss/1.0/IGnssBatchingCallback.h>
#include <pthread.h>

#include <LocationAPIClientBase.h>

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

class BatchingAPIClient : public LocationAPIClientBase
{
public:
    BatchingAPIClient(const sp<V1_0::IGnssBatchingCallback>& callback);
    ~BatchingAPIClient();
    int getBatchSize();
    int startSession(const V1_0::IGnssBatching::Options& options);
    int updateSessionOptions(const V1_0::IGnssBatching::Options& options);
    int stopSession();
    void getBatchedLocation(int last_n_locations);
    void flushBatchedLocations();

    inline LocationCapabilitiesMask getCapabilities() { return mLocationCapabilitiesMask; }

    // callbacks
    void onCapabilitiesCb(LocationCapabilitiesMask capabilitiesMask) final;
    void onBatchingCb(size_t count, Location* location, BatchingOptions batchOptions) final;

private:
    sp<V1_0::IGnssBatchingCallback> mGnssBatchingCbIface;
    uint32_t mDefaultId;
    LocationCapabilitiesMask mLocationCapabilitiesMask;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
#endif // BATCHING_API_CLINET_H
