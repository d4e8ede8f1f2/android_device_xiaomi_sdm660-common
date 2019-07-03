
#ifndef LOCATION_INTERFACE_H
#define LOCATION_INTERFACE_H

#include <LocationAPI.h>
#include <gps_extended_c.h>

struct GnssInterface {
    size_t size;
    void (*initialize)(void);
    void (*deinitialize)(void);
    void (*addClient)(LocationAPI* client, const LocationCallbacks& callbacks);
    void (*removeClient)(LocationAPI* client);
    void (*requestCapabilities)(LocationAPI* client);
    uint32_t (*startTracking)(LocationAPI* client, LocationOptions& options);
    void (*updateTrackingOptions)(LocationAPI* client, uint32_t id, LocationOptions& options);
    void (*stopTracking)(LocationAPI* client, uint32_t id);
    void (*gnssNiResponse)(LocationAPI* client, uint32_t id, GnssNiResponse response);
    void (*setControlCallbacks)(LocationControlCallbacks& controlCallbacks);
    uint32_t (*enable)(LocationTechnologyType techType);
    void (*disable)(uint32_t id);
    uint32_t* (*gnssUpdateConfig)(GnssConfig config);
    uint32_t (*gnssDeleteAidingData)(GnssAidingData& data);
    void (*gnssUpdateXtraThrottle)(const bool enabled);
    void (*injectLocation)(double latitude, double longitude, float accuracy);
    void (*injectTime)(int64_t time, int64_t timeReference, int32_t uncertainty);
    void (*agpsInit)(const AgpsCbInfo& cbInfo);
    void (*agpsDataConnOpen)(AGpsExtType agpsType, const char* apnName, int apnLen, int ipType);
    void (*agpsDataConnClosed)(AGpsExtType agpsType);
    void (*agpsDataConnFailed)(AGpsExtType agpsType);
    void (*getDebugReport)(GnssDebugReport& report);
    void (*updateConnectionStatus)(bool connected, int8_t type);
    void (*odcpiInit)(const OdcpiRequestCallback& callback);
    void (*odcpiInject)(const Location& location);
};

struct FlpInterface {
    size_t size;
    void (*initialize)(void);
    void (*deinitialize)(void);
    void (*addClient)(LocationAPI* client, const LocationCallbacks& callbacks);
    void (*removeClient)(LocationAPI* client);
    void (*requestCapabilities)(LocationAPI* client);
    uint32_t (*startTracking)(LocationAPI* client, LocationOptions& options);
    void (*updateTrackingOptions)(LocationAPI* client, uint32_t id, LocationOptions& options);
    void (*stopTracking)(LocationAPI* client, uint32_t id);
    uint32_t (*startBatching)(LocationAPI* client, LocationOptions&, BatchingOptions&);
    void (*stopBatching)(LocationAPI* client, uint32_t id);
    void (*updateBatchingOptions)(LocationAPI* client, uint32_t id, LocationOptions&,
            BatchingOptions&);
    void (*getBatchedLocations)(LocationAPI* client, uint32_t id, size_t count);
    void (*getPowerStateChanges)(void* powerStateCb);
};

struct GeofenceInterface {
    size_t size;
    void (*initialize)(void);
    void (*deinitialize)(void);
    void (*addClient)(LocationAPI* client, const LocationCallbacks& callbacks);
    void (*removeClient)(LocationAPI* client);
    void (*requestCapabilities)(LocationAPI* client);
    uint32_t* (*addGeofences)(LocationAPI* client, size_t count, GeofenceOption*, GeofenceInfo*);
    void (*removeGeofences)(LocationAPI* client, size_t count, uint32_t* ids);
    void (*modifyGeofences)(LocationAPI* client, size_t count, uint32_t* ids,
                            GeofenceOption* options);
    void (*pauseGeofences)(LocationAPI* client, size_t count, uint32_t* ids);
    void (*resumeGeofences)(LocationAPI* client, size_t count, uint32_t* ids);
};

#endif /* LOCATION_INTERFACE_H */
