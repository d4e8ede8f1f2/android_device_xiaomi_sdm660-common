
#ifndef LOC_ENG_NMEA_H
#define LOC_ENG_NMEA_H

#include <gps_extended.h>
#include <vector>
#include <string>
#define NMEA_SENTENCE_MAX_LENGTH 200

void loc_nmea_generate_sv(const GnssSvNotification &svNotify,
                              std::vector<std::string> &nmeaArraystr);

void loc_nmea_generate_pos(const UlpLocation &location,
                               const GpsLocationExtended &locationExtended,
                               unsigned char generate_nmea,
                               std::vector<std::string> &nmeaArraystr);

#define DEBUG_NMEA_MINSIZE 6
#define DEBUG_NMEA_MAXSIZE 4096
inline bool loc_nmea_is_debug(const char* nmea, int length) {
    return ((nullptr != nmea) &&
            (length >= DEBUG_NMEA_MINSIZE) && (length <= DEBUG_NMEA_MAXSIZE) &&
            (nmea[0] == '$') && (nmea[1] == 'P') && (nmea[2] == 'Q') && (nmea[3] == 'W'));
}

#endif // LOC_ENG_NMEA_H
