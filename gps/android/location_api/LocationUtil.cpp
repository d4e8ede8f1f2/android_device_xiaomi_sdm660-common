
#include <LocationUtil.h>

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

using ::android::hardware::gnss::V1_0::GnssLocation;
using ::android::hardware::gnss::V1_0::GnssConstellationType;
using ::android::hardware::gnss::V1_0::GnssLocationFlags;

void convertGnssLocation(Location& in, GnssLocation& out)
{
    memset(&out, 0, sizeof(GnssLocation));
    if (in.flags & LOCATION_HAS_LAT_LONG_BIT)
        out.gnssLocationFlags |= GnssLocationFlags::HAS_LAT_LONG;
    if (in.flags & LOCATION_HAS_ALTITUDE_BIT)
        out.gnssLocationFlags |= GnssLocationFlags::HAS_ALTITUDE;
    if (in.flags & LOCATION_HAS_SPEED_BIT)
        out.gnssLocationFlags |= GnssLocationFlags::HAS_SPEED;
    if (in.flags & LOCATION_HAS_BEARING_BIT)
        out.gnssLocationFlags |= GnssLocationFlags::HAS_BEARING;
    if (in.flags & LOCATION_HAS_ACCURACY_BIT)
        out.gnssLocationFlags |= GnssLocationFlags::HAS_HORIZONTAL_ACCURACY;
    if (in.flags & LOCATION_HAS_VERTICAL_ACCURACY_BIT)
        out.gnssLocationFlags |= GnssLocationFlags::HAS_VERTICAL_ACCURACY;
    if (in.flags & LOCATION_HAS_SPEED_ACCURACY_BIT)
        out.gnssLocationFlags |= GnssLocationFlags::HAS_SPEED_ACCURACY;
    if (in.flags & LOCATION_HAS_BEARING_ACCURACY_BIT)
        out.gnssLocationFlags |= GnssLocationFlags::HAS_BEARING_ACCURACY;
    out.latitudeDegrees = in.latitude;
    out.longitudeDegrees = in.longitude;
    out.altitudeMeters = in.altitude;
    out.speedMetersPerSec = in.speed;
    out.bearingDegrees = in.bearing;
    out.horizontalAccuracyMeters = in.accuracy;
    out.verticalAccuracyMeters = in.verticalAccuracy;
    out.speedAccuracyMetersPerSecond = in.speedAccuracy;
    out.bearingAccuracyDegrees = in.bearingAccuracy;
    out.timestamp = static_cast<V1_0::GnssUtcTime>(in.timestamp);
}

void convertGnssLocation(const GnssLocation& in, Location& out)
{
    memset(&out, 0, sizeof(out));
    if (in.gnssLocationFlags & GnssLocationFlags::HAS_LAT_LONG)
        out.flags |= LOCATION_HAS_LAT_LONG_BIT;
    if (in.gnssLocationFlags & GnssLocationFlags::HAS_ALTITUDE)
        out.flags |= LOCATION_HAS_ALTITUDE_BIT;
    if (in.gnssLocationFlags & GnssLocationFlags::HAS_SPEED)
        out.flags |= LOCATION_HAS_SPEED_BIT;
    if (in.gnssLocationFlags & GnssLocationFlags::HAS_BEARING)
        out.flags |= LOCATION_HAS_BEARING_BIT;
    if (in.gnssLocationFlags & GnssLocationFlags::HAS_HORIZONTAL_ACCURACY)
        out.flags |= LOCATION_HAS_ACCURACY_BIT;
    if (in.gnssLocationFlags & GnssLocationFlags::HAS_VERTICAL_ACCURACY)
        out.flags |= LOCATION_HAS_VERTICAL_ACCURACY_BIT;
    if (in.gnssLocationFlags & GnssLocationFlags::HAS_SPEED_ACCURACY)
        out.flags |= LOCATION_HAS_SPEED_ACCURACY_BIT;
    if (in.gnssLocationFlags & GnssLocationFlags::HAS_BEARING_ACCURACY)
        out.flags |= LOCATION_HAS_BEARING_ACCURACY_BIT;
    out.latitude = in.latitudeDegrees;
    out.longitude = in.longitudeDegrees;
    out.altitude = in.altitudeMeters;
    out.speed = in.speedMetersPerSec;
    out.bearing = in.bearingDegrees;
    out.accuracy = in.horizontalAccuracyMeters;
    out.verticalAccuracy = in.verticalAccuracyMeters;
    out.speedAccuracy = in.speedAccuracyMetersPerSecond;
    out.bearingAccuracy = in.bearingAccuracyDegrees;
    out.timestamp = static_cast<uint64_t>(in.timestamp);
}

void convertGnssConstellationType(GnssSvType& in, GnssConstellationType& out)
{
    switch(in) {
        case GNSS_SV_TYPE_GPS:
            out = GnssConstellationType::GPS;
            break;
        case GNSS_SV_TYPE_SBAS:
            out = GnssConstellationType::SBAS;
            break;
        case GNSS_SV_TYPE_GLONASS:
            out = GnssConstellationType::GLONASS;
            break;
        case GNSS_SV_TYPE_QZSS:
            out = GnssConstellationType::QZSS;
            break;
        case GNSS_SV_TYPE_BEIDOU:
            out = GnssConstellationType::BEIDOU;
            break;
        case GNSS_SV_TYPE_GALILEO:
            out = GnssConstellationType::GALILEO;
            break;
        case GNSS_SV_TYPE_UNKNOWN:
        default:
            out = GnssConstellationType::UNKNOWN;
            break;
    }
}

void convertGnssEphemerisType(GnssEphemerisType& in, GnssDebug::SatelliteEphemerisType& out)
{
    switch(in) {
        case GNSS_EPH_TYPE_EPHEMERIS:
            out = GnssDebug::SatelliteEphemerisType::EPHEMERIS;
            break;
        case GNSS_EPH_TYPE_ALMANAC:
            out = GnssDebug::SatelliteEphemerisType::ALMANAC_ONLY;
            break;
        case GNSS_EPH_TYPE_UNKNOWN:
        default:
            out = GnssDebug::SatelliteEphemerisType::NOT_AVAILABLE;
            break;
    }
}

void convertGnssEphemerisSource(GnssEphemerisSource& in, GnssDebug::SatelliteEphemerisSource& out)
{
    switch(in) {
        case GNSS_EPH_SOURCE_DEMODULATED:
            out = GnssDebug::SatelliteEphemerisSource::DEMODULATED;
            break;
        case GNSS_EPH_SOURCE_SUPL_PROVIDED:
            out = GnssDebug::SatelliteEphemerisSource::SUPL_PROVIDED;
            break;
        case GNSS_EPH_SOURCE_OTHER_SERVER_PROVIDED:
            out = GnssDebug::SatelliteEphemerisSource::OTHER_SERVER_PROVIDED;
            break;
        case GNSS_EPH_SOURCE_LOCAL:
        case GNSS_EPH_SOURCE_UNKNOWN:
        default:
            out = GnssDebug::SatelliteEphemerisSource::OTHER;
            break;
    }
}

void convertGnssEphemerisHealth(GnssEphemerisHealth& in, GnssDebug::SatelliteEphemerisHealth& out)
{
    switch(in) {
        case GNSS_EPH_HEALTH_GOOD:
            out = GnssDebug::SatelliteEphemerisHealth::GOOD;
            break;
        case GNSS_EPH_HEALTH_BAD:
            out = GnssDebug::SatelliteEphemerisHealth::BAD;
            break;
        case GNSS_EPH_HEALTH_UNKNOWN:
        default:
            out = GnssDebug::SatelliteEphemerisHealth::UNKNOWN;
            break;
    }
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
