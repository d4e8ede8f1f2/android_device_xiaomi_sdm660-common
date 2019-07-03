
#ifndef ULP_PROXY_BASE_H
#define ULP_PROXY_BASE_H

#include <gps_extended.h>
#include <LocationAPI.h>

namespace loc_core {

class LocAdapterBase;

class UlpProxyBase {
public:
    LocPosMode mPosMode;
    bool mFixSet;
    inline UlpProxyBase() {
        mPosMode.mode = LOC_POSITION_MODE_INVALID;
        mFixSet = false;
    }
    inline virtual ~UlpProxyBase() {}
    inline virtual bool sendStartFix() { mFixSet = true; return false; }
    inline virtual bool sendStopFix() { mFixSet = false; return false; }
    inline virtual bool sendFixMode(LocPosMode &params) {
        mPosMode = params;
        return false;
    }

    inline virtual bool reportPosition(const UlpLocation &location,
                                       const GpsLocationExtended &locationExtended,
                                       enum loc_sess_status status,
                                       LocPosTechMask loc_technology_mask) {
        (void)location;
        (void)locationExtended;
        (void)status;
        (void)loc_technology_mask;
        return false;
    }
    inline virtual bool reportSv(const GnssSvNotification& svNotify) {
        (void)svNotify;
        return false;
    }
    inline virtual bool reportSvMeasurement(GnssSvMeasurementSet &svMeasurementSet) {
        (void)svMeasurementSet;
        return false;
    }

    inline virtual bool reportSvPolynomial(GnssSvPolynomial &svPolynomial)
    {
       (void)svPolynomial;
       return false;
    }
    inline virtual bool reportStatus(LocGpsStatusValue status) {

        (void)status;
        return false;
    }
    inline virtual void setAdapter(LocAdapterBase* adapter) {

        (void)adapter;
    }
    inline virtual void setCapabilities(unsigned long capabilities) {

        (void)capabilities;
    }
    inline virtual bool reportBatchingSession(const LocationOptions& options, bool active)
    {
         (void)options;
         (void)active;
         return false;
    }
    inline virtual bool reportPositions(const UlpLocation* ulpLocations,
                                        const GpsLocationExtended* extendedLocations,
                                        const uint32_t* techMasks,
                                        const size_t count)
    {
        (void)ulpLocations;
        (void)extendedLocations;
        (void)techMasks;
        (void)count;
        return false;
    }
    inline virtual bool reportDeleteAidingData(LocGpsAidingData aidingData)
    {
       (void)aidingData;
       return false;
    }
    inline virtual bool reportNmea(const char* nmea, int length)
    {
        (void)nmea;
        (void)length;
        return false;
    }
};

} // namespace loc_core

#endif // ULP_PROXY_BASE_H
