
#ifndef LOC_API_ADAPTER_BASE_H
#define LOC_API_ADAPTER_BASE_H

#include <gps_extended.h>
#include <UlpProxyBase.h>
#include <ContextBase.h>
#include <LocationAPI.h>
#include <map>

typedef struct LocationSessionKey {
    LocationAPI* client;
    uint32_t id;
    inline LocationSessionKey(LocationAPI* _client, uint32_t _id) :
        client(_client), id(_id) {}
} LocationSessionKey;
inline bool operator <(LocationSessionKey const& left, LocationSessionKey const& right) {
    return left.id < right.id || (left.id == right.id && left.client < right.client);
}
inline bool operator ==(LocationSessionKey const& left, LocationSessionKey const& right) {
    return left.id == right.id && left.client == right.client;
}
inline bool operator !=(LocationSessionKey const& left, LocationSessionKey const& right) {
    return left.id != right.id || left.client != right.client;
}
typedef std::map<LocationSessionKey, LocationOptions> LocationSessionMap;

namespace loc_core {

class LocAdapterProxyBase;

class LocAdapterBase {
private:
    static uint32_t mSessionIdCounter;
protected:
    LOC_API_ADAPTER_EVENT_MASK_T mEvtMask;
    ContextBase* mContext;
    LocApiBase* mLocApi;
    LocAdapterProxyBase* mLocAdapterProxyBase;
    const MsgTask* mMsgTask;
    inline LocAdapterBase(const MsgTask* msgTask) :
        mEvtMask(0), mContext(NULL), mLocApi(NULL),
        mLocAdapterProxyBase(NULL), mMsgTask(msgTask) {}
public:
    inline virtual ~LocAdapterBase() { mLocApi->removeAdapter(this); }
    LocAdapterBase(const LOC_API_ADAPTER_EVENT_MASK_T mask,
                   ContextBase* context, LocAdapterProxyBase *adapterProxyBase = NULL);
    inline LOC_API_ADAPTER_EVENT_MASK_T
        checkMask(LOC_API_ADAPTER_EVENT_MASK_T mask) const {
        return mEvtMask & mask;
    }

    inline LOC_API_ADAPTER_EVENT_MASK_T getEvtMask() const {
        return mEvtMask;
    }

    inline void sendMsg(const LocMsg* msg) const {
        mMsgTask->sendMsg(msg);
    }

    inline void sendMsg(const LocMsg* msg) {
        mMsgTask->sendMsg(msg);
    }

    inline void updateEvtMask(LOC_API_ADAPTER_EVENT_MASK_T event,
                              loc_registration_mask_status status)
    {
        switch(status) {
            case (LOC_REGISTRATION_MASK_ENABLED):
                mEvtMask = mEvtMask | event;
                break;
            case (LOC_REGISTRATION_MASK_DISABLED):
                mEvtMask = mEvtMask &~ event;
                break;
            case (LOC_REGISTRATION_MASK_SET):
                mEvtMask = event;
                break;
        }
        mLocApi->updateEvtMask();
    }

    inline bool isFeatureSupported(uint8_t featureVal) {
        return mLocApi->isFeatureSupported(featureVal);
    }

    uint32_t generateSessionId();

    // This will be overridden by the individual adapters
    // if necessary.
    inline virtual void setUlpProxyCommand(UlpProxyBase* ulp) {

        (void)ulp;
    }
    virtual void handleEngineUpEvent();
    virtual void handleEngineDownEvent();
    inline virtual void setPositionModeCommand(LocPosMode& posMode) {

        (void)posMode;
    }
    virtual void startTrackingCommand() {}
    virtual void stopTrackingCommand() {}
    virtual void getZppCommand() {}
    virtual void reportPositionEvent(const UlpLocation& location,
                                     const GpsLocationExtended& locationExtended,
                                     enum loc_sess_status status,
                                     LocPosTechMask loc_technology_mask,
                                     bool fromUlp=false);
    virtual void reportSvEvent(const GnssSvNotification& svNotify, bool fromUlp=false);
    virtual void reportNmeaEvent(const char* nmea, size_t length, bool fromUlp=false);
    virtual void reportSvMeasurementEvent(GnssSvMeasurementSet &svMeasurementSet);
    virtual void reportSvPolynomialEvent(GnssSvPolynomial &svPolynomial);
    virtual void reportStatus(LocGpsStatusValue status);
    virtual bool reportXtraServer(const char* url1, const char* url2,
                                  const char* url3, const int maxlength);
    virtual bool requestXtraData();
    virtual bool requestTime();
    virtual bool requestLocation();
    virtual bool requestATL(int connHandle, LocAGpsType agps_type);
    virtual bool releaseATL(int connHandle);
    virtual bool requestSuplES(int connHandle);
    virtual bool reportDataCallOpened();
    virtual bool reportDataCallClosed();
    virtual bool requestNiNotifyEvent(const GnssNiNotification &notify, const void* data);
    inline virtual bool isInSession() { return false; }
    ContextBase* getContext() const { return mContext; }
    virtual void reportGnssMeasurementDataEvent(const GnssMeasurementsNotification& measurements,
                                                int msInWeek);
    virtual bool reportWwanZppFix(LocGpsLocation &zppLoc);
    virtual bool reportOdcpiRequestEvent(OdcpiRequestInfo& request);
};

} // namespace loc_core

#endif //LOC_API_ADAPTER_BASE_H
