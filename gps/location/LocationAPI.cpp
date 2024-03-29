
#define LOG_TAG "LocSvc_LocationAPI"

#include <location_interface.h>
#include <dlfcn.h>
#include <loc_pla.h>
#include <log_util.h>
#include <pthread.h>
#include <map>
#include <loc_misc_utils.h>

typedef const GnssInterface* (getGnssInterface)();
typedef const FlpInterface* (getFlpInterface)();
typedef const GeofenceInterface* (getGeofenceInterface)();

typedef std::map<LocationAPI*, LocationCallbacks> LocationClientMap;
typedef struct {
    LocationClientMap clientData;
    LocationControlAPI* controlAPI;
    LocationControlCallbacks controlCallbacks;
    GnssInterface* gnssInterface;
    GeofenceInterface* geofenceInterface;
    FlpInterface* flpInterface;
} LocationAPIData;
static LocationAPIData gData = {};
static pthread_mutex_t gDataMutex = PTHREAD_MUTEX_INITIALIZER;
static bool gGnssLoadFailed = false;
static bool gFlpLoadFailed = false;
static bool gGeofenceLoadFailed = false;

template <typename T1, typename T2>
static const T1* loadLocationInterface(const char* library, const char* name) {
    void* libhandle = nullptr;
    T2* getter = (T2*)dlGetSymFromLib(libhandle, library, name);
    if (nullptr == getter) {
        return (const T1*) getter;
    }else {
        return (*getter)();
    }
}

static bool needsGnssTrackingInfo(LocationCallbacks& locationCallbacks)
{
    return (locationCallbacks.gnssLocationInfoCb != nullptr ||
            locationCallbacks.gnssSvCb != nullptr ||
            locationCallbacks.gnssNmeaCb != nullptr ||
            locationCallbacks.gnssMeasurementsCb != nullptr);
}

static bool isGnssClient(LocationCallbacks& locationCallbacks)
{
    return (locationCallbacks.gnssNiCb != nullptr ||
            locationCallbacks.trackingCb != nullptr ||
            locationCallbacks.gnssMeasurementsCb != nullptr);
}

static bool isFlpClient(LocationCallbacks& locationCallbacks)
{
    return (locationCallbacks.trackingCb != nullptr ||
            locationCallbacks.batchingCb != nullptr);
}

static bool isGeofenceClient(LocationCallbacks& locationCallbacks)
{
    return (locationCallbacks.geofenceBreachCb != nullptr ||
            locationCallbacks.geofenceStatusCb != nullptr);
}


LocationAPI*
LocationAPI::createInstance(LocationCallbacks& locationCallbacks)
{
    if (nullptr == locationCallbacks.capabilitiesCb ||
        nullptr == locationCallbacks.responseCb ||
        nullptr == locationCallbacks.collectiveResponseCb) {
        return NULL;
    }

    LocationAPI* newLocationAPI = new LocationAPI();
    bool requestedCapabilities = false;

    pthread_mutex_lock(&gDataMutex);

    if (isGnssClient(locationCallbacks)) {
        if (NULL == gData.gnssInterface && !gGnssLoadFailed) {
            gData.gnssInterface =
                (GnssInterface*)loadLocationInterface<GnssInterface,
                    getGnssInterface>("libgnss.so", "getGnssInterface");
            if (NULL == gData.gnssInterface) {
                gGnssLoadFailed = true;
                LOC_LOGW("%s:%d]: No gnss interface available", __func__, __LINE__);
            } else {
                gData.gnssInterface->initialize();
            }
        }
        if (NULL != gData.gnssInterface) {
            gData.gnssInterface->addClient(newLocationAPI, locationCallbacks);
            if (!requestedCapabilities) {
                gData.gnssInterface->requestCapabilities(newLocationAPI);
                requestedCapabilities = true;
            }
        }
    }

    if (isFlpClient(locationCallbacks)) {
        if (NULL == gData.flpInterface && !gFlpLoadFailed) {
            gData.flpInterface =
                (FlpInterface*)loadLocationInterface<FlpInterface,
                   getFlpInterface>("libflp.so", "getFlpInterface");
            if (NULL == gData.flpInterface) {
                gFlpLoadFailed = true;
                LOC_LOGW("%s:%d]: No flp interface available", __func__, __LINE__);
            } else {
                gData.flpInterface->initialize();
            }
        }
        if (NULL != gData.flpInterface) {
            gData.flpInterface->addClient(newLocationAPI, locationCallbacks);
            if (!requestedCapabilities) {
                gData.flpInterface->requestCapabilities(newLocationAPI);
                requestedCapabilities = true;
            }
        }
    }

    if (isGeofenceClient(locationCallbacks)) {
        if (NULL == gData.geofenceInterface && !gGeofenceLoadFailed) {
            gData.geofenceInterface =
               (GeofenceInterface*)loadLocationInterface<GeofenceInterface,
                getGeofenceInterface>("libgeofence.so", "getGeofenceInterface");
            if (NULL == gData.geofenceInterface) {
                gGeofenceLoadFailed = true;
                LOC_LOGW("%s:%d]: No geofence interface available", __func__, __LINE__);
            } else {
                gData.geofenceInterface->initialize();
            }
        }
        if (NULL != gData.geofenceInterface) {
            gData.geofenceInterface->addClient(newLocationAPI, locationCallbacks);
            if (!requestedCapabilities) {
                gData.geofenceInterface->requestCapabilities(newLocationAPI);
                requestedCapabilities = true;
            }
        }
    }

    gData.clientData[newLocationAPI] = locationCallbacks;

    pthread_mutex_unlock(&gDataMutex);

    return newLocationAPI;
}

void
LocationAPI::destroy()
{
    delete this;
}

LocationAPI::LocationAPI()
{
    LOC_LOGD("LOCATION API CONSTRUCTOR");
}

LocationAPI::~LocationAPI()
{
    LOC_LOGD("LOCATION API DESTRUCTOR");
    pthread_mutex_lock(&gDataMutex);

    auto it = gData.clientData.find(this);
    if (it != gData.clientData.end()) {
        if (isGnssClient(it->second) && NULL != gData.gnssInterface) {
            gData.gnssInterface->removeClient(it->first);
        }
        if (isFlpClient(it->second) && NULL != gData.flpInterface) {
            gData.flpInterface->removeClient(it->first);
        }
        if (isGeofenceClient(it->second) && NULL != gData.geofenceInterface) {
            gData.geofenceInterface->removeClient(it->first);
        }
        gData.clientData.erase(it);
    } else {
        LOC_LOGE("%s:%d]: Location API client %p not found in client data",
                 __func__, __LINE__, this);
    }

    pthread_mutex_unlock(&gDataMutex);
}

void
LocationAPI::updateCallbacks(LocationCallbacks& locationCallbacks)
{
    if (nullptr == locationCallbacks.capabilitiesCb ||
        nullptr == locationCallbacks.responseCb ||
        nullptr == locationCallbacks.collectiveResponseCb) {
        return;
    }

    pthread_mutex_lock(&gDataMutex);

    if (isGnssClient(locationCallbacks)) {
        if (NULL == gData.gnssInterface && !gGnssLoadFailed) {
            gData.gnssInterface =
                (GnssInterface*)loadLocationInterface<GnssInterface,
                    getGnssInterface>("libgnss.so", "getGnssInterface");
            if (NULL == gData.gnssInterface) {
                gGnssLoadFailed = true;
                LOC_LOGW("%s:%d]: No gnss interface available", __func__, __LINE__);
            } else {
                gData.gnssInterface->initialize();
            }
        }
        if (NULL != gData.gnssInterface) {
            // either adds new Client or updates existing Client
            gData.gnssInterface->addClient(this, locationCallbacks);
        }
    }

    if (isFlpClient(locationCallbacks)) {
        if (NULL == gData.flpInterface && !gFlpLoadFailed) {
            gData.flpInterface =
                (FlpInterface*)loadLocationInterface<FlpInterface,
                    getFlpInterface>("libflp.so", "getFlpInterface");
            if (NULL == gData.flpInterface) {
                gFlpLoadFailed = true;
                LOC_LOGW("%s:%d]: No flp interface available", __func__, __LINE__);
            } else {
                gData.flpInterface->initialize();
            }
        }
        if (NULL != gData.flpInterface) {
            // either adds new Client or updates existing Client
            gData.flpInterface->addClient(this, locationCallbacks);
        }
    }

    if (isGeofenceClient(locationCallbacks)) {
        if (NULL == gData.geofenceInterface && !gGeofenceLoadFailed) {
            gData.geofenceInterface =
                (GeofenceInterface*)loadLocationInterface<GeofenceInterface,
                getGeofenceInterface>("libgeofence.so", "getGeofenceInterface");
            if (NULL == gData.geofenceInterface) {
                gGeofenceLoadFailed = true;
                LOC_LOGW("%s:%d]: No geofence interface available", __func__, __LINE__);
            } else {
                gData.geofenceInterface->initialize();
            }
        }
        if (NULL != gData.geofenceInterface) {
            // either adds new Client or updates existing Client
            gData.geofenceInterface->addClient(this, locationCallbacks);
        }
    }

    gData.clientData[this] = locationCallbacks;

    pthread_mutex_unlock(&gDataMutex);
}

uint32_t
LocationAPI::startTracking(LocationOptions& locationOptions)
{
    uint32_t id = 0;
    pthread_mutex_lock(&gDataMutex);

    auto it = gData.clientData.find(this);
    if (it != gData.clientData.end()) {
        if (gData.flpInterface != NULL && locationOptions.minDistance > 0) {
            id = gData.flpInterface->startTracking(this, locationOptions);
        } else if (gData.gnssInterface != NULL && needsGnssTrackingInfo(it->second)) {
            id = gData.gnssInterface->startTracking(this, locationOptions);
        } else if (gData.flpInterface != NULL) {
            id = gData.flpInterface->startTracking(this, locationOptions);
        } else if (gData.gnssInterface != NULL) {
            id = gData.gnssInterface->startTracking(this, locationOptions);
        } else {
            LOC_LOGE("%s:%d]: No gnss/flp interface available for Location API client %p ",
                     __func__, __LINE__, this);
        }
    } else {
        LOC_LOGE("%s:%d]: Location API client %p not found in client data",
                 __func__, __LINE__, this);
    }

    pthread_mutex_unlock(&gDataMutex);
    return id;
}

void
LocationAPI::stopTracking(uint32_t id)
{
    pthread_mutex_lock(&gDataMutex);

    auto it = gData.clientData.find(this);
    if (it != gData.clientData.end()) {
        // we don't know if tracking was started on flp or gnss, so we call stop on both, where
        // stopTracking call to the incorrect interface will fail without response back to client
        if (gData.gnssInterface != NULL) {
            gData.gnssInterface->stopTracking(this, id);
        }
        if (gData.flpInterface != NULL) {
            gData.flpInterface->stopTracking(this, id);
        }
        if (gData.flpInterface == NULL && gData.gnssInterface == NULL) {
            LOC_LOGE("%s:%d]: No gnss/flp interface available for Location API client %p ",
                     __func__, __LINE__, this);
        }
    } else {
        LOC_LOGE("%s:%d]: Location API client %p not found in client data",
                 __func__, __LINE__, this);
    }

    pthread_mutex_unlock(&gDataMutex);
}

void
LocationAPI::updateTrackingOptions(uint32_t id, LocationOptions& locationOptions)
{
    pthread_mutex_lock(&gDataMutex);

    auto it = gData.clientData.find(this);
    if (it != gData.clientData.end()) {
        // we don't know if tracking was started on flp or gnss, so we call update on both, where
        // updateTracking call to the incorrect interface will fail without response back to client
        if (gData.gnssInterface != NULL) {
            gData.gnssInterface->updateTrackingOptions(this, id, locationOptions);
        }
        if (gData.flpInterface != NULL) {
            gData.flpInterface->updateTrackingOptions(this, id, locationOptions);
        }
        if (gData.flpInterface == NULL && gData.gnssInterface == NULL) {
            LOC_LOGE("%s:%d]: No gnss/flp interface available for Location API client %p ",
                     __func__, __LINE__, this);
        }
    } else {
        LOC_LOGE("%s:%d]: Location API client %p not found in client data",
                 __func__, __LINE__, this);
    }

    pthread_mutex_unlock(&gDataMutex);
}

uint32_t
LocationAPI::startBatching(LocationOptions& locationOptions, BatchingOptions &batchingOptions)
{
    uint32_t id = 0;
    pthread_mutex_lock(&gDataMutex);

    if (gData.flpInterface != NULL) {
        id = gData.flpInterface->startBatching(this, locationOptions, batchingOptions);
    } else {
        LOC_LOGE("%s:%d]: No flp interface available for Location API client %p ",
                 __func__, __LINE__, this);
    }

    pthread_mutex_unlock(&gDataMutex);
    return id;
}

void
LocationAPI::stopBatching(uint32_t id)
{
    pthread_mutex_lock(&gDataMutex);

    if (gData.flpInterface != NULL) {
        gData.flpInterface->stopBatching(this, id);
    } else {
        LOC_LOGE("%s:%d]: No flp interface available for Location API client %p ",
                 __func__, __LINE__, this);
    }

    pthread_mutex_unlock(&gDataMutex);
}

void
LocationAPI::updateBatchingOptions(uint32_t id,
        LocationOptions& locationOptions, BatchingOptions& batchOptions)
{
    pthread_mutex_lock(&gDataMutex);

    if (gData.flpInterface != NULL) {
        gData.flpInterface->updateBatchingOptions(this,
                                                  id,
                                                  locationOptions,
                                                  batchOptions);
    } else {
        LOC_LOGE("%s:%d]: No flp interface available for Location API client %p ",
                 __func__, __LINE__, this);
    }

    pthread_mutex_unlock(&gDataMutex);
}

void
LocationAPI::getBatchedLocations(uint32_t id, size_t count)
{
    pthread_mutex_lock(&gDataMutex);

    if (gData.flpInterface != NULL) {
        gData.flpInterface->getBatchedLocations(this, id, count);
    } else {
        LOC_LOGE("%s:%d]: No flp interface available for Location API client %p ",
                 __func__, __LINE__, this);
    }

    pthread_mutex_unlock(&gDataMutex);
}

uint32_t*
LocationAPI::addGeofences(size_t count, GeofenceOption* options, GeofenceInfo* info)
{
    uint32_t* ids = NULL;
    pthread_mutex_lock(&gDataMutex);

    if (gData.geofenceInterface != NULL) {
        ids = gData.geofenceInterface->addGeofences(this, count, options, info);
    } else {
        LOC_LOGE("%s:%d]: No geofence interface available for Location API client %p ",
                 __func__, __LINE__, this);
    }

    pthread_mutex_unlock(&gDataMutex);
    return ids;
}

void
LocationAPI::removeGeofences(size_t count, uint32_t* ids)
{
    pthread_mutex_lock(&gDataMutex);

    if (gData.geofenceInterface != NULL) {
        gData.geofenceInterface->removeGeofences(this, count, ids);
    } else {
        LOC_LOGE("%s:%d]: No geofence interface available for Location API client %p ",
                 __func__, __LINE__, this);
    }

    pthread_mutex_unlock(&gDataMutex);
}

void
LocationAPI::modifyGeofences(size_t count, uint32_t* ids, GeofenceOption* options)
{
    pthread_mutex_lock(&gDataMutex);

    if (gData.geofenceInterface != NULL) {
        gData.geofenceInterface->modifyGeofences(this, count, ids, options);
    } else {
        LOC_LOGE("%s:%d]: No geofence interface available for Location API client %p ",
                 __func__, __LINE__, this);
    }

    pthread_mutex_unlock(&gDataMutex);
}

void
LocationAPI::pauseGeofences(size_t count, uint32_t* ids)
{
    pthread_mutex_lock(&gDataMutex);

    if (gData.geofenceInterface != NULL) {
        gData.geofenceInterface->pauseGeofences(this, count, ids);
    } else {
        LOC_LOGE("%s:%d]: No geofence interface available for Location API client %p ",
                 __func__, __LINE__, this);
    }

    pthread_mutex_unlock(&gDataMutex);
}

void
LocationAPI::resumeGeofences(size_t count, uint32_t* ids)
{
    pthread_mutex_lock(&gDataMutex);

    if (gData.geofenceInterface != NULL) {
        gData.geofenceInterface->resumeGeofences(this, count, ids);
    } else {
        LOC_LOGE("%s:%d]: No geofence interface available for Location API client %p ",
                 __func__, __LINE__, this);
    }

    pthread_mutex_unlock(&gDataMutex);
}

void
LocationAPI::gnssNiResponse(uint32_t id, GnssNiResponse response)
{
    pthread_mutex_lock(&gDataMutex);

    if (gData.gnssInterface != NULL) {
        gData.gnssInterface->gnssNiResponse(this, id, response);
    } else {
        LOC_LOGE("%s:%d]: No gnss interface available for Location API client %p ",
                 __func__, __LINE__, this);
    }

    pthread_mutex_unlock(&gDataMutex);
}

LocationControlAPI*
LocationControlAPI::createInstance(LocationControlCallbacks& locationControlCallbacks)
{
    LocationControlAPI* controlAPI = NULL;
    pthread_mutex_lock(&gDataMutex);

    if (nullptr != locationControlCallbacks.responseCb && NULL == gData.controlAPI) {
        if (NULL == gData.gnssInterface && !gGnssLoadFailed) {
            gData.gnssInterface =
                (GnssInterface*)loadLocationInterface<GnssInterface,
                    getGnssInterface>("libgnss.so", "getGnssInterface");
            if (NULL == gData.gnssInterface) {
                gGnssLoadFailed = true;
                LOC_LOGW("%s:%d]: No gnss interface available", __func__, __LINE__);
            } else {
                gData.gnssInterface->initialize();
            }
        }
        if (NULL != gData.gnssInterface) {
            gData.controlAPI = new LocationControlAPI();
            gData.controlCallbacks = locationControlCallbacks;
            gData.gnssInterface->setControlCallbacks(locationControlCallbacks);
            controlAPI = gData.controlAPI;
        }
    }

    pthread_mutex_unlock(&gDataMutex);
    return controlAPI;
}

void
LocationControlAPI::destroy()
{
    delete this;
}

LocationControlAPI::LocationControlAPI()
{
    LOC_LOGD("LOCATION CONTROL API CONSTRUCTOR");
}

LocationControlAPI::~LocationControlAPI()
{
    LOC_LOGD("LOCATION CONTROL API DESTRUCTOR");
    pthread_mutex_lock(&gDataMutex);

    gData.controlAPI = NULL;

    pthread_mutex_unlock(&gDataMutex);
}

uint32_t
LocationControlAPI::enable(LocationTechnologyType techType)
{
    uint32_t id = 0;
    pthread_mutex_lock(&gDataMutex);

    if (gData.gnssInterface != NULL) {
        id = gData.gnssInterface->enable(techType);
    } else {
        LOC_LOGE("%s:%d]: No gnss interface available for Location Control API client %p ",
                 __func__, __LINE__, this);
    }

    pthread_mutex_unlock(&gDataMutex);
    return id;
}

void
LocationControlAPI::disable(uint32_t id)
{
    pthread_mutex_lock(&gDataMutex);

    if (gData.gnssInterface != NULL) {
        gData.gnssInterface->disable(id);
    } else {
        LOC_LOGE("%s:%d]: No gnss interface available for Location Control API client %p ",
                 __func__, __LINE__, this);
    }

    pthread_mutex_unlock(&gDataMutex);
}

uint32_t*
LocationControlAPI::gnssUpdateConfig(GnssConfig config)
{
    uint32_t* ids = NULL;
    pthread_mutex_lock(&gDataMutex);

    if (gData.gnssInterface != NULL) {
        ids = gData.gnssInterface->gnssUpdateConfig(config);
    } else {
        LOC_LOGE("%s:%d]: No gnss interface available for Location Control API client %p ",
                 __func__, __LINE__, this);
    }

    pthread_mutex_unlock(&gDataMutex);
    return ids;
}

uint32_t
LocationControlAPI::gnssDeleteAidingData(GnssAidingData& data)
{
    uint32_t id = 0;
    pthread_mutex_lock(&gDataMutex);

    if (gData.gnssInterface != NULL) {
        id = gData.gnssInterface->gnssDeleteAidingData(data);
    } else {
        LOC_LOGE("%s:%d]: No gnss interface available for Location Control API client %p ",
                 __func__, __LINE__, this);
    }

    pthread_mutex_unlock(&gDataMutex);
    return id;
}
