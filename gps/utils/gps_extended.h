
#ifndef GPS_EXTENDED_H
#define GPS_EXTENDED_H

/**
 * @file
 * @brief C++ declarations for GPS types
 */

#include <gps_extended_c.h>
#if defined(USE_GLIB) || defined(OFF_TARGET)
#include <string.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



struct LocPosMode
{
    LocPositionMode mode;
    LocGpsPositionRecurrence recurrence;
    uint32_t min_interval;
    uint32_t preferred_accuracy;
    uint32_t preferred_time;
    bool share_position;
    char credentials[14];
    char provider[8];
    LocPosMode(LocPositionMode m, LocGpsPositionRecurrence recr,
               uint32_t gap, uint32_t accu, uint32_t time,
               bool sp, const char* cred, const char* prov) :
        mode(m), recurrence(recr),
        min_interval(gap < GPS_MIN_POSSIBLE_FIX_INTERVAL_MS ?
                     GPS_MIN_POSSIBLE_FIX_INTERVAL_MS : gap),
        preferred_accuracy(accu), preferred_time(time),
        share_position(sp) {
        memset(credentials, 0, sizeof(credentials));
        memset(provider, 0, sizeof(provider));
        if (NULL != cred) {
            memcpy(credentials, cred, sizeof(credentials)-1);
        }
        if (NULL != prov) {
            memcpy(provider, prov, sizeof(provider)-1);
        }
    }

    inline LocPosMode() :
        mode(LOC_POSITION_MODE_MS_BASED),
        recurrence(LOC_GPS_POSITION_RECURRENCE_PERIODIC),
        min_interval(GPS_DEFAULT_FIX_INTERVAL_MS),
        preferred_accuracy(50), preferred_time(120000),
        share_position(true) {
        memset(credentials, 0, sizeof(credentials));
        memset(provider, 0, sizeof(provider));
    }

    inline bool equals(const LocPosMode &anotherMode) const
    {
        return anotherMode.mode == mode &&
            anotherMode.recurrence == recurrence &&
            anotherMode.min_interval == min_interval &&
            anotherMode.preferred_accuracy == preferred_accuracy &&
            anotherMode.preferred_time == preferred_time &&
            !strncmp(anotherMode.credentials, credentials, sizeof(credentials)-1) &&
            !strncmp(anotherMode.provider, provider, sizeof(provider)-1);
    }

    void logv() const;
};

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* GPS_EXTENDED_H */
