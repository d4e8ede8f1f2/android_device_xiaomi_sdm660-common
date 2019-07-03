
#include "GnssAdapter.h"
#include "location_interface.h"

static GnssAdapter* gGnssAdapter = NULL;

static void initialize();
static void deinitialize();

static void addClient(LocationAPI* client, const LocationCallbacks& callbacks);
static void removeClient(LocationAPI* client);
static void requestCapabilities(LocationAPI* client);

static uint32_t startTracking(LocationAPI* client, LocationOptions& options);
static void updateTrackingOptions(LocationAPI* client, uint32_t id, LocationOptions& options);
static void stopTracking(LocationAPI* client, uint32_t id);

static void gnssNiResponse(LocationAPI* client, uint32_t id, GnssNiResponse response);
static uint32_t gnssDeleteAidingData(GnssAidingData& data);
static void gnssUpdateXtraThrottle(const bool enabled);

static void setControlCallbacks(LocationControlCallbacks& controlCallbacks);
static uint32_t enable(LocationTechnologyType techType);
static void disable(uint32_t id);
static uint32_t* gnssUpdateConfig(GnssConfig config);

static void injectLocation(double latitude, double longitude, float accuracy);
static void injectTime(int64_t time, int64_t timeReference, int32_t uncertainty);

static void agpsInit(const AgpsCbInfo& cbInfo);
static void agpsDataConnOpen(AGpsExtType agpsType, const char* apnName, int apnLen, int ipType);
static void agpsDataConnClosed(AGpsExtType agpsType);
static void agpsDataConnFailed(AGpsExtType agpsType);
static void getDebugReport(GnssDebugReport& report);
static void updateConnectionStatus(bool connected, int8_t type);

static void odcpiInit(const OdcpiRequestCallback& callback);
static void odcpiInject(const Location& location);

static const GnssInterface gGnssInterface = {
    sizeof(GnssInterface),
    initialize,
    deinitialize,
    addClient,
    removeClient,
    requestCapabilities,
    startTracking,
    updateTrackingOptions,
    stopTracking,
    gnssNiResponse,
    setControlCallbacks,
    enable,
    disable,
    gnssUpdateConfig,
    gnssDeleteAidingData,
    gnssUpdateXtraThrottle,
    injectLocation,
    injectTime,
    agpsInit,
    agpsDataConnOpen,
    agpsDataConnClosed,
    agpsDataConnFailed,
    getDebugReport,
    updateConnectionStatus,
    odcpiInit,
    odcpiInject,
};

#ifndef DEBUG_X86
extern "C" const GnssInterface* getGnssInterface()
#else
const GnssInterface* getGnssInterface()
#endif // DEBUG_X86
{
   return &gGnssInterface;
}

static void initialize()
{
    if (NULL == gGnssAdapter) {
        gGnssAdapter = new GnssAdapter();
    }
}

static void deinitialize()
{
    if (NULL != gGnssAdapter) {
        delete gGnssAdapter;
        gGnssAdapter = NULL;
    }
}

static void addClient(LocationAPI* client, const LocationCallbacks& callbacks)
{
    if (NULL != gGnssAdapter) {
        gGnssAdapter->addClientCommand(client, callbacks);
    }
}

static void removeClient(LocationAPI* client)
{
    if (NULL != gGnssAdapter) {
        gGnssAdapter->removeClientCommand(client);
    }
}

static void requestCapabilities(LocationAPI* client)
{
    if (NULL != gGnssAdapter) {
        gGnssAdapter->requestCapabilitiesCommand(client);
    }
}

static uint32_t startTracking(LocationAPI* client, LocationOptions& options)
{
    if (NULL != gGnssAdapter) {
        return gGnssAdapter->startTrackingCommand(client, options);
    } else {
        return 0;
    }
}

static void updateTrackingOptions(LocationAPI* client, uint32_t id, LocationOptions& options)
{
    if (NULL != gGnssAdapter) {
        gGnssAdapter->updateTrackingOptionsCommand(client, id, options);
    }
}

static void stopTracking(LocationAPI* client, uint32_t id)
{
    if (NULL != gGnssAdapter) {
        gGnssAdapter->stopTrackingCommand(client, id);
    }
}

static void gnssNiResponse(LocationAPI* client, uint32_t id, GnssNiResponse response)
{
    if (NULL != gGnssAdapter) {
        gGnssAdapter->gnssNiResponseCommand(client, id, response);
    }
}

static void setControlCallbacks(LocationControlCallbacks& controlCallbacks)
{
    if (NULL != gGnssAdapter) {
        return gGnssAdapter->setControlCallbacksCommand(controlCallbacks);
    }
}

static uint32_t enable(LocationTechnologyType techType)
{
    if (NULL != gGnssAdapter) {
        return gGnssAdapter->enableCommand(techType);
    } else {
        return 0;
    }
}

static void disable(uint32_t id)
{
    if (NULL != gGnssAdapter) {
        return gGnssAdapter->disableCommand(id);
    }
}

static uint32_t* gnssUpdateConfig(GnssConfig config)
{
    if (NULL != gGnssAdapter) {
        return gGnssAdapter->gnssUpdateConfigCommand(config);
    } else {
        return NULL;
    }
}

static uint32_t gnssDeleteAidingData(GnssAidingData& data)
{
    if (NULL != gGnssAdapter) {
        return gGnssAdapter->gnssDeleteAidingDataCommand(data);
    } else {
        return 0;
    }
}

static void gnssUpdateXtraThrottle(const bool enabled)
{
    if (NULL != gGnssAdapter) {
        gGnssAdapter->gnssUpdateXtraThrottleCommand(enabled);
    }
}

static void injectLocation(double latitude, double longitude, float accuracy)
{
   if (NULL != gGnssAdapter) {
       gGnssAdapter->injectLocationCommand(latitude, longitude, accuracy);
   }
}

static void injectTime(int64_t time, int64_t timeReference, int32_t uncertainty)
{
   if (NULL != gGnssAdapter) {
       gGnssAdapter->injectTimeCommand(time, timeReference, uncertainty);
   }
}

static void agpsInit(const AgpsCbInfo& cbInfo) {

    if (NULL != gGnssAdapter) {
        gGnssAdapter->initAgpsCommand(cbInfo);
    }
}
static void agpsDataConnOpen(
        AGpsExtType agpsType, const char* apnName, int apnLen, int ipType) {

    if (NULL != gGnssAdapter) {
        gGnssAdapter->dataConnOpenCommand(
                agpsType, apnName, apnLen, (AGpsBearerType)ipType);
    }
}
static void agpsDataConnClosed(AGpsExtType agpsType) {

    if (NULL != gGnssAdapter) {
        gGnssAdapter->dataConnClosedCommand(agpsType);
    }
}
static void agpsDataConnFailed(AGpsExtType agpsType) {

    if (NULL != gGnssAdapter) {
        gGnssAdapter->dataConnFailedCommand(agpsType);
    }
}

static void getDebugReport(GnssDebugReport& report) {

    if (NULL != gGnssAdapter) {
        gGnssAdapter->getDebugReport(report);
    }
}

static void updateConnectionStatus(bool connected, int8_t type) {
    if (NULL != gGnssAdapter) {
        gGnssAdapter->getSystemStatus()->eventConnectionStatus(connected, type);
    }
}

static void odcpiInit(const OdcpiRequestCallback& callback)
{
    if (NULL != gGnssAdapter) {
        gGnssAdapter->initOdcpiCommand(callback);
    }
}

static void odcpiInject(const Location& location)
{
    if (NULL != gGnssAdapter) {
        gGnssAdapter->injectOdcpiCommand(location);
    }
}
