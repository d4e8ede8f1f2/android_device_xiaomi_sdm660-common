
#ifndef LOC_CORE_LOG_H
#define LOC_CORE_LOG_H

#include <ctype.h>
#include <gps_extended.h>

#ifdef __cplusplus
extern "C"
{
#endif

const char* loc_get_gps_status_name(LocGpsStatusValue gps_status);
const char* loc_get_position_mode_name(LocGpsPositionMode mode);
const char* loc_get_position_recurrence_name(LocGpsPositionRecurrence recur);
const char* loc_get_aiding_data_mask_names(LocGpsAidingData data);
const char* loc_get_agps_type_name(LocAGpsType type);
const char* loc_get_ni_type_name(LocGpsNiType type);
const char* loc_get_ni_response_name(LocGpsUserResponseType response);
const char* loc_get_ni_encoding_name(LocGpsNiEncodingType encoding);
const char* loc_get_agps_bear_name(AGpsBearerType bear);
const char* loc_get_server_type_name(LocServerType type);
const char* loc_get_position_sess_status_name(enum loc_sess_status status);
const char* loc_get_agps_status_name(LocAGpsStatusValue status);

#ifdef __cplusplus
}
#endif

#endif /* LOC_CORE_LOG_H */
