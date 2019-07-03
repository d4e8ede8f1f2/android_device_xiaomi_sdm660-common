
#ifndef GEOFENCE_API_CLINET_H
#define GEOFENCE_API_CLINET_H


#include <android/hardware/gnss/1.0/IGnssGeofenceCallback.h>
#include <LocationAPIClientBase.h>

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

using ::android::sp;

class GeofenceAPIClient : public LocationAPIClientBase
{
public:
    GeofenceAPIClient(const sp<V1_0::IGnssGeofenceCallback>& callback);
    virtual ~GeofenceAPIClient() = default;

    void geofenceAdd(uint32_t geofence_id, double latitude, double longitude,
            double radius_meters, int32_t last_transition, int32_t monitor_transitions,
            uint32_t notification_responsiveness_ms, uint32_t unknown_timer_ms);
    void geofencePause(uint32_t geofence_id);
    void geofenceResume(uint32_t geofence_id, int32_t monitor_transitions);
    void geofenceRemove(uint32_t geofence_id);
    void geofenceRemoveAll();

    // callbacks
    void onGeofenceBreachCb(GeofenceBreachNotification geofenceBreachNotification) final;
    void onGeofenceStatusCb(GeofenceStatusNotification geofenceStatusNotification) final;
    void onAddGeofencesCb(size_t count, LocationError* errors, uint32_t* ids) final;
    void onRemoveGeofencesCb(size_t count, LocationError* errors, uint32_t* ids) final;
    void onPauseGeofencesCb(size_t count, LocationError* errors, uint32_t* ids) final;
    void onResumeGeofencesCb(size_t count, LocationError* errors, uint32_t* ids) final;

private:
    sp<V1_0::IGnssGeofenceCallback> mGnssGeofencingCbIface;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
#endif // GEOFENCE_API_CLINET_H
