
#ifndef GNSS_ADAPTER_H
#define GNSS_ADAPTER_H

#include <LocAdapterBase.h>
#include <LocDualContext.h>
#include <UlpProxyBase.h>
#include <LocationAPI.h>
#include <Agps.h>
#include <SystemStatus.h>
#include <XtraSystemStatusObserver.h>

#define MAX_URL_LEN 256
#define NMEA_SENTENCE_MAX_LENGTH 200
#define GLONASS_SV_ID_OFFSET 64
#define MAX_SATELLITES_IN_USE 12
#define LOC_NI_NO_RESPONSE_TIME 20
#define LOC_GPS_NI_RESPONSE_IGNORE 4
#define ODCPI_EXPECTED_INJECTION_TIME_MS 10000

class GnssAdapter;

class OdcpiTimer : public LocTimer {
public:
    OdcpiTimer(GnssAdapter* adapter) :
            LocTimer(), mAdapter(adapter), mActive(false) {}

    inline void start() {
        mActive = true;
        LocTimer::start(ODCPI_EXPECTED_INJECTION_TIME_MS, false);
    }
    inline void stop() {
        mActive = false;
        LocTimer::stop();
    }
    inline void restart() {
        stop();
        start();
    }
    inline bool isActive() {
        return mActive;
    }

private:
    // Override
    virtual void timeOutCallback() override;

    GnssAdapter* mAdapter;
    bool mActive;
};

typedef struct {
    pthread_t               thread;        /* NI thread */
    uint32_t                respTimeLeft;  /* examine time for NI response */
    bool                    respRecvd;     /* NI User reponse received or not from Java layer*/
    void*                   rawRequest;
    uint32_t                reqID;         /* ID to check against response */
    GnssNiResponse          resp;
    pthread_cond_t          tCond;
    pthread_mutex_t         tLock;
    GnssAdapter*            adapter;
} NiSession;
typedef struct {
    NiSession session;    /* SUPL NI Session */
    NiSession sessionEs;  /* Emergency SUPL NI Session */
    uint32_t reqIDCounter;
} NiData;

typedef enum {
    NMEA_PROVIDER_AP = 0, // Application Processor Provider of NMEA
    NMEA_PROVIDER_MP      // Modem Processor Provider of NMEA
} NmeaProviderType;
typedef struct {
    GnssSvType svType;
    const char* talker;
    uint64_t mask;
    uint32_t svIdOffset;
} NmeaSvMeta;

using namespace loc_core;

namespace loc_core {
    class SystemStatus;
}

class GnssAdapter : public LocAdapterBase {

    /* ==== ULP ============================================================================ */
    UlpProxyBase* mUlpProxy;

    /* ==== CLIENT ========================================================================= */
    typedef std::map<LocationAPI*, LocationCallbacks> ClientDataMap;
    ClientDataMap mClientData;

    /* ==== TRACKING ======================================================================= */
    LocationSessionMap mTrackingSessions;
    LocPosMode mUlpPositionMode;
    GnssSvUsedInPosition mGnssSvIdUsedInPosition;
    bool mGnssSvIdUsedInPosAvail;

    /* ==== CONTROL ======================================================================== */
    LocationControlCallbacks mControlCallbacks;
    uint32_t mPowerVoteId;
    uint32_t mNmeaMask;

    /* ==== NI ============================================================================= */
    NiData mNiData;

    /* ==== AGPS =========================================================================== */
    // This must be initialized via initAgps()
    AgpsManager mAgpsManager;
    AgpsCbInfo mAgpsCbInfo;
    void initAgps(const AgpsCbInfo& cbInfo);

    /* ==== ODCPI ========================================================================== */
    OdcpiRequestCallback mOdcpiRequestCb;
    bool mOdcpiRequestActive;
    OdcpiTimer mOdcpiTimer;
    OdcpiRequestInfo mOdcpiRequest;
    void odcpiTimerExpire();

    /* === SystemStatus ===================================================================== */
    SystemStatus* mSystemStatus;
    std::string mServerUrl;
    XtraSystemStatusObserver mXtraObserver;

    /*==== CONVERSION ===================================================================*/
    static void convertOptions(LocPosMode& out, const LocationOptions& options);
    static void convertLocation(Location& out, const LocGpsLocation& locGpsLocation,
                                const GpsLocationExtended& locationExtended,
                                const LocPosTechMask techMask);
    static void convertLocationInfo(GnssLocationInfoNotification& out,
                                    const GpsLocationExtended& locationExtended);

public:

    GnssAdapter();
    virtual inline ~GnssAdapter() { delete mUlpProxy; }

    /* ==== SSR ============================================================================ */
    /* ======== EVENTS ====(Called from QMI Thread)========================================= */
    virtual void handleEngineUpEvent();
    /* ======== UTILITIES ================================================================== */
    void restartSessions();

    /* ==== ULP ============================================================================ */
    /* ======== COMMANDS ====(Called from ULP Thread)==================================== */
    virtual void setUlpProxyCommand(UlpProxyBase* ulp);
    /* ======== UTILITIES ================================================================== */
    void setUlpProxy(UlpProxyBase* ulp);
    inline UlpProxyBase* getUlpProxy() { return mUlpProxy; }

    /* ==== CLIENT ========================================================================= */
    /* ======== COMMANDS ====(Called from Client Thread)==================================== */
    void addClientCommand(LocationAPI* client, const LocationCallbacks& callbacks);
    void removeClientCommand(LocationAPI* client);
    void requestCapabilitiesCommand(LocationAPI* client);
    /* ======== UTILITIES ================================================================== */
    void saveClient(LocationAPI* client, const LocationCallbacks& callbacks);
    void eraseClient(LocationAPI* client);
    void updateClientsEventMask();
    void stopClientSessions(LocationAPI* client);
    LocationCallbacks getClientCallbacks(LocationAPI* client);
    LocationCapabilitiesMask getCapabilities();
    void broadcastCapabilities(LocationCapabilitiesMask);
    LocationError setSuplHostServer(const char* server, int port);

    /* ==== TRACKING ======================================================================= */
    /* ======== COMMANDS ====(Called from Client Thread)==================================== */
    uint32_t startTrackingCommand(LocationAPI* client, LocationOptions& options);
    void updateTrackingOptionsCommand(LocationAPI* client, uint32_t id, LocationOptions& options);
    void stopTrackingCommand(LocationAPI* client, uint32_t id);
    /* ======================(Called from ULP Thread)======================================= */
    virtual void setPositionModeCommand(LocPosMode& locPosMode);
    virtual void startTrackingCommand();
    virtual void stopTrackingCommand();
    virtual void getZppCommand();
    /* ======== RESPONSES ================================================================== */
    void reportResponse(LocationAPI* client, LocationError err, uint32_t sessionId);
    /* ======== UTILITIES ================================================================== */
    bool hasTrackingCallback(LocationAPI* client);
    bool hasMeasurementsCallback(LocationAPI* client);
    bool isTrackingSession(LocationAPI* client, uint32_t sessionId);
    void saveTrackingSession(LocationAPI* client, uint32_t sessionId,
                             const LocationOptions& options);
    void eraseTrackingSession(LocationAPI* client, uint32_t sessionId);
    bool setUlpPositionMode(const LocPosMode& mode);
    LocPosMode& getUlpPositionMode() { return mUlpPositionMode; }
    LocationError startTrackingMultiplex(const LocationOptions& options);
    LocationError startTracking(const LocationOptions& options);
    LocationError stopTrackingMultiplex(LocationAPI* client, uint32_t id);
    LocationError stopTracking();
    LocationError updateTrackingMultiplex(LocationAPI* client, uint32_t id,
                                          const LocationOptions& options);

    /* ==== NI ============================================================================= */
    /* ======== COMMANDS ====(Called from Client Thread)==================================== */
    void gnssNiResponseCommand(LocationAPI* client, uint32_t id, GnssNiResponse response);
    /* ======================(Called from NI Thread)======================================== */
    void gnssNiResponseCommand(GnssNiResponse response, void* rawRequest);
    /* ======== UTILITIES ================================================================== */
    bool hasNiNotifyCallback(LocationAPI* client);
    NiData& getNiData() { return mNiData; }

    /* ==== CONTROL ======================================================================== */
    /* ======== COMMANDS ====(Called from Client Thread)==================================== */
    uint32_t enableCommand(LocationTechnologyType techType);
    void disableCommand(uint32_t id);
    void setControlCallbacksCommand(LocationControlCallbacks& controlCallbacks);
    void readConfigCommand();
    void setConfigCommand();
    uint32_t* gnssUpdateConfigCommand(GnssConfig config);
    uint32_t gnssDeleteAidingDataCommand(GnssAidingData& data);
    void gnssUpdateXtraThrottleCommand(const bool enabled);

    void initDefaultAgpsCommand();
    void initAgpsCommand(const AgpsCbInfo& cbInfo);
    void dataConnOpenCommand(AGpsExtType agpsType,
            const char* apnName, int apnLen, AGpsBearerType bearerType);
    void dataConnClosedCommand(AGpsExtType agpsType);
    void dataConnFailedCommand(AGpsExtType agpsType);

    /* ========= ODCPI ===================================================================== */
    /* ======== COMMANDS ====(Called from Client Thread)==================================== */
    void initOdcpiCommand(const OdcpiRequestCallback& callback);
    void injectOdcpiCommand(const Location& location);
    /* ======== UTILITIES ================================================================== */
    void initOdcpi(const OdcpiRequestCallback& callback);
    void injectOdcpi(const Location& location);
    void odcpiTimerExpireEvent();

    /* ======== RESPONSES ================================================================== */
    void reportResponse(LocationError err, uint32_t sessionId);
    void reportResponse(size_t count, LocationError* errs, uint32_t* ids);
    /* ======== UTILITIES ================================================================== */
    LocationControlCallbacks& getControlCallbacks() { return mControlCallbacks; }
    void setControlCallbacks(const LocationControlCallbacks& controlCallbacks)
    { mControlCallbacks = controlCallbacks; }
    void setPowerVoteId(uint32_t id) { mPowerVoteId = id; }
    uint32_t getPowerVoteId() { return mPowerVoteId; }
    bool resolveInAddress(const char* hostAddress, struct in_addr* inAddress);
    virtual bool isInSession() { return !mTrackingSessions.empty(); }
    void initDefaultAgps();

    /* ==== REPORTS ======================================================================== */
    /* ======== EVENTS ====(Called from QMI/ULP Thread)===================================== */
    virtual void reportPositionEvent(const UlpLocation& ulpLocation,
                                     const GpsLocationExtended& locationExtended,
                                     enum loc_sess_status status,
                                     LocPosTechMask techMask,
                                     bool fromUlp=false);
    virtual void reportSvEvent(const GnssSvNotification& svNotify, bool fromUlp=false);
    virtual void reportNmeaEvent(const char* nmea, size_t length, bool fromUlp=false);
    virtual bool requestNiNotifyEvent(const GnssNiNotification& notify, const void* data);
    virtual void reportGnssMeasurementDataEvent(const GnssMeasurementsNotification& measurements,
                                                int msInWeek);
    virtual void reportSvMeasurementEvent(GnssSvMeasurementSet &svMeasurementSet);
    virtual void reportSvPolynomialEvent(GnssSvPolynomial &svPolynomial);

    virtual bool requestATL(int connHandle, LocAGpsType agps_type);
    virtual bool releaseATL(int connHandle);
    virtual bool requestSuplES(int connHandle);
    virtual bool reportDataCallOpened();
    virtual bool reportDataCallClosed();
    virtual bool reportOdcpiRequestEvent(OdcpiRequestInfo& request);

    /* ======== UTILITIES ================================================================= */
    bool needReport(const UlpLocation& ulpLocation,
            enum loc_sess_status status, LocPosTechMask techMask);
    void reportPosition(const UlpLocation &ulpLocation,
                        const GpsLocationExtended &locationExtended,
                        enum loc_sess_status status,
                        LocPosTechMask techMask);
    void reportSv(GnssSvNotification& svNotify);
    void reportNmea(const char* nmea, size_t length);
    bool requestNiNotify(const GnssNiNotification& notify, const void* data);
    void reportGnssMeasurementData(const GnssMeasurementsNotification& measurements);
    void reportOdcpiRequest(const OdcpiRequestInfo& request);

    /*======== GNSSDEBUG ================================================================*/
    bool getDebugReport(GnssDebugReport& report);
    /* get AGC information from system status and fill it */
    void getAgcInformation(GnssMeasurementsNotification& measurements, int msInWeek);

    /*==== SYSTEM STATUS ================================================================*/
    inline SystemStatus* getSystemStatus(void) { return mSystemStatus; }
    std::string& getServerUrl(void) { return mServerUrl; }
    void setServerUrl(const char* server) { mServerUrl.assign(server); }

    /*==== CONVERSION ===================================================================*/
    static uint32_t convertGpsLock(const GnssConfigGpsLock gpsLock);
    static GnssConfigGpsLock convertGpsLock(const uint32_t gpsLock);
    static uint32_t convertSuplVersion(const GnssConfigSuplVersion suplVersion);
    static GnssConfigSuplVersion convertSuplVersion(const uint32_t suplVersion);
    static uint32_t convertLppProfile(const GnssConfigLppProfile lppProfile);
    static GnssConfigLppProfile convertLppProfile(const uint32_t lppProfile);
    static uint32_t convertEP4ES(const GnssConfigEmergencyPdnForEmergencySupl);
    static uint32_t convertSuplEs(const GnssConfigSuplEmergencyServices suplEmergencyServices);
    static uint32_t convertLppeCp(const GnssConfigLppeControlPlaneMask lppeControlPlaneMask);
    static GnssConfigLppeControlPlaneMask convertLppeCp(const uint32_t lppeControlPlaneMask);
    static uint32_t convertLppeUp(const GnssConfigLppeUserPlaneMask lppeUserPlaneMask);
    static GnssConfigLppeUserPlaneMask convertLppeUp(const uint32_t lppeUserPlaneMask);
    static uint32_t convertAGloProt(const GnssConfigAGlonassPositionProtocolMask);
    static uint32_t convertSuplMode(const GnssConfigSuplModeMask suplModeMask);
    static void convertSatelliteInfo(std::vector<GnssDebugSatelliteInfo>& out,
                                     const GnssSvType& in_constellation,
                                     const SystemStatusReports& in);

    void injectLocationCommand(double latitude, double longitude, float accuracy);
    void injectTimeCommand(int64_t time, int64_t timeReference, int32_t uncertainty);

};

#endif //GNSS_ADAPTER_H
