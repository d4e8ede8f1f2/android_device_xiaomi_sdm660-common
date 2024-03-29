
#ifndef LOC_API_BASE_H
#define LOC_API_BASE_H

#include <stddef.h>
#include <ctype.h>
#include <gps_extended.h>
#include <LocationAPI.h>
#include <MsgTask.h>
#include <log_util.h>

namespace loc_core {
class ContextBase;

int hexcode(char *hexstring, int string_size,
            const char *data, int data_size);
int decodeAddress(char *addr_string, int string_size,
                  const char *data, int data_size);

#define MAX_ADAPTERS          10
#define MAX_FEATURE_LENGTH    100

#define TO_ALL_ADAPTERS(adapters, call)                                \
    for (int i = 0; i < MAX_ADAPTERS && NULL != (adapters)[i]; i++) {  \
        call;                                                          \
    }

#define TO_1ST_HANDLING_ADAPTER(adapters, call)                              \
    for (int i = 0; i <MAX_ADAPTERS && NULL != (adapters)[i] && !(call); i++);

enum xtra_version_check {
    DISABLED,
    AUTO,
    XTRA2,
    XTRA3
};

class LocAdapterBase;
struct LocSsrMsg;
struct LocOpenMsg;

class LocApiProxyBase {
public:
    inline LocApiProxyBase() {}
    inline virtual ~LocApiProxyBase() {}
    inline virtual void* getSibling2() { return NULL; }
};

class LocApiBase {
    friend struct LocSsrMsg;
    //LocOpenMsg calls open() which makes it necessary to declare
    //it as a friend
    friend struct LocOpenMsg;
    friend class ContextBase;
    const MsgTask* mMsgTask;
    ContextBase *mContext;
    LocAdapterBase* mLocAdapters[MAX_ADAPTERS];
    uint64_t mSupportedMsg;
    uint8_t mFeaturesSupported[MAX_FEATURE_LENGTH];

protected:
    virtual enum loc_api_adapter_err
        open(LOC_API_ADAPTER_EVENT_MASK_T mask);
    virtual enum loc_api_adapter_err
        close();
    LOC_API_ADAPTER_EVENT_MASK_T getEvtMask();
    LOC_API_ADAPTER_EVENT_MASK_T mMask;
    LocApiBase(const MsgTask* msgTask,
               LOC_API_ADAPTER_EVENT_MASK_T excludedMask,
               ContextBase* context = NULL);
    inline virtual ~LocApiBase() { close(); }
    bool isInSession();
    const LOC_API_ADAPTER_EVENT_MASK_T mExcludedMask;

public:
    inline void sendMsg(const LocMsg* msg) const {
        mMsgTask->sendMsg(msg);
    }
    static bool needReport(const UlpLocation& ulpLocation,
                           enum loc_sess_status status,
                           LocPosTechMask techMask);

    void addAdapter(LocAdapterBase* adapter);
    void removeAdapter(LocAdapterBase* adapter);

    // upward calls
    void handleEngineUpEvent();
    void handleEngineDownEvent();
    void reportPosition(UlpLocation& location,
                        GpsLocationExtended& locationExtended,
                        enum loc_sess_status status,
                        LocPosTechMask loc_technology_mask =
                                  LOC_POS_TECH_MASK_DEFAULT);
    void reportSv(GnssSvNotification& svNotify);
    void reportSvMeasurement(GnssSvMeasurementSet &svMeasurementSet);
    void reportSvPolynomial(GnssSvPolynomial &svPolynomial);
    void reportStatus(LocGpsStatusValue status);
    void reportNmea(const char* nmea, int length);
    void reportXtraServer(const char* url1, const char* url2,
                          const char* url3, const int maxlength);
    void requestXtraData();
    void requestTime();
    void requestLocation();
    void requestATL(int connHandle, LocAGpsType agps_type);
    void releaseATL(int connHandle);
    void requestSuplES(int connHandle);
    void reportDataCallOpened();
    void reportDataCallClosed();
    void requestNiNotify(GnssNiNotification &notify, const void* data);
    void saveSupportedMsgList(uint64_t supportedMsgList);
    void reportGnssMeasurementData(GnssMeasurementsNotification& measurements, int msInWeek);
    void saveSupportedFeatureList(uint8_t *featureList);
    void reportWwanZppFix(LocGpsLocation &zppLoc);
    void reportOdcpiRequest(OdcpiRequestInfo& request);

    // downward calls
    // All below functions are to be defined by adapter specific modules:
    // RPC, QMI, etc.  The default implementation is empty.

    virtual void* getSibling();
    virtual LocApiProxyBase* getLocApiProxy();
    virtual enum loc_api_adapter_err
        startFix(const LocPosMode& posMode);
    virtual enum loc_api_adapter_err
        stopFix();
    virtual LocationError
        deleteAidingData(const GnssAidingData& data);
    virtual enum loc_api_adapter_err
        enableData(int enable);
    virtual enum loc_api_adapter_err
        setAPN(char* apn, int len);
    virtual enum loc_api_adapter_err
        injectPosition(double latitude, double longitude, float accuracy);
    virtual enum loc_api_adapter_err
        injectPosition(const Location& location);
    virtual enum loc_api_adapter_err
        setTime(LocGpsUtcTime time, int64_t timeReference, int uncertainty);
    virtual enum loc_api_adapter_err
        setXtraData(char* data, int length);
    virtual enum loc_api_adapter_err
        requestXtraServer();
    virtual enum loc_api_adapter_err
        atlOpenStatus(int handle, int is_succ, char* apn, AGpsBearerType bear, LocAGpsType agpsType);
    virtual enum loc_api_adapter_err
        atlCloseStatus(int handle, int is_succ);
    virtual enum loc_api_adapter_err
        setPositionMode(const LocPosMode& posMode);
    virtual LocationError
        setServer(const char* url, int len);
    virtual LocationError
        setServer(unsigned int ip, int port,
                  LocServerType type);
    virtual LocationError
        informNiResponse(GnssNiResponse userResponse, const void* passThroughData);
    virtual LocationError setSUPLVersion(GnssConfigSuplVersion version);
    virtual enum loc_api_adapter_err
        setNMEATypes (uint32_t typesMask);
    virtual LocationError setLPPConfig(GnssConfigLppProfile profile);
    virtual enum loc_api_adapter_err
        setSensorControlConfig(int sensorUsage, int sensorProvider);
    virtual enum loc_api_adapter_err
        setSensorProperties(bool gyroBiasVarianceRandomWalk_valid,
                            float gyroBiasVarianceRandomWalk,
                            bool accelBiasVarianceRandomWalk_valid,
                            float accelBiasVarianceRandomWalk,
                            bool angleBiasVarianceRandomWalk_valid,
                            float angleBiasVarianceRandomWalk,
                            bool rateBiasVarianceRandomWalk_valid,
                            float rateBiasVarianceRandomWalk,
                            bool velocityBiasVarianceRandomWalk_valid,
                            float velocityBiasVarianceRandomWalk);
    virtual enum loc_api_adapter_err
        setSensorPerfControlConfig(int controlMode,
                               int accelSamplesPerBatch,
                               int accelBatchesPerSec,
                               int gyroSamplesPerBatch,
                               int gyroBatchesPerSec,
                               int accelSamplesPerBatchHigh,
                               int accelBatchesPerSecHigh,
                               int gyroSamplesPerBatchHigh,
                               int gyroBatchesPerSecHigh,
                               int algorithmConfig);
    virtual LocationError
        setAGLONASSProtocol(GnssConfigAGlonassPositionProtocolMask aGlonassProtocol);
    virtual LocationError setLPPeProtocolCp(GnssConfigLppeControlPlaneMask lppeCP);
    virtual LocationError setLPPeProtocolUp(GnssConfigLppeUserPlaneMask lppeUP);
    virtual enum loc_api_adapter_err
        getWwanZppFix();
    virtual enum loc_api_adapter_err
        getBestAvailableZppFix(LocGpsLocation & zppLoc);
    virtual enum loc_api_adapter_err
        getBestAvailableZppFix(LocGpsLocation & zppLoc, GpsLocationExtended & locationExtended,
                LocPosTechMask & tech_mask);
    virtual int initDataServiceClient(bool isDueToSsr);
    virtual int openAndStartDataCall();
    virtual void stopDataCall();
    virtual void closeDataCall();
    virtual void releaseDataServiceClient();
    virtual void installAGpsCert(const LocDerEncodedCertificate* pData,
                                 size_t length,
                                 uint32_t slotBitMask);
    inline virtual void setInSession(bool inSession) {

        (void)inSession;
    }
    inline bool isMessageSupported (LocCheckingMessagesID msgID) const {

        // confirm if msgID is not larger than the number of bits in
        // mSupportedMsg
        if ((uint64_t)msgID > (sizeof(mSupportedMsg) << 3)) {
            return false;
        } else {
            uint32_t messageChecker = 1 << msgID;
            return (messageChecker & mSupportedMsg) == messageChecker;
        }
    }

    void updateEvtMask();

    virtual LocationError setGpsLock(GnssConfigGpsLock lock);
    /*
      Returns
      Current value of GPS Lock on success
      -1 on failure
     */
    virtual int getGpsLock(void);

    virtual LocationError setXtraVersionCheck(uint32_t check);
    /*
      Check if the modem support the service
     */
    virtual bool gnssConstellationConfig();

    /*
       Check if a feature is supported
      */
    bool isFeatureSupported(uint8_t featureVal);
};

typedef LocApiBase* (getLocApi_t)(const MsgTask* msgTask,
                                  LOC_API_ADAPTER_EVENT_MASK_T exMask,
                                  ContextBase *context);

} // namespace loc_core

#endif //LOC_API_BASE_H
