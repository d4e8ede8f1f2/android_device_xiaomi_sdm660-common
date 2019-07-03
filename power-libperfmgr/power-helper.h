
#ifndef __POWER_HELPER_H__
#define __POWER_HELPER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <hardware/power.h>

enum stats_type {
    //Platform Stats
    RPM_MODE_XO = 0,
    RPM_MODE_VMIN,
    RPM_MODE_MAX,
    XO_VOTERS_START = RPM_MODE_MAX,
    VOTER_APSS = XO_VOTERS_START,
    VOTER_MPSS,
    VOTER_ADSP,
    VOTER_SLPI,
    MAX_PLATFORM_STATS,

    //WLAN Stats
    WLAN_POWER_DEBUG_STATS = 0,
    MAX_WLAN_STATS,
};

enum subsystem_type {
    SUBSYSTEM_WLAN = 0,

    //Don't add any lines after this line
    SUBSYSTEM_COUNT
};

enum wlan_sleep_states {
    WLAN_STATE_ACTIVE = 0,
    WLAN_STATE_DEEP_SLEEP,

    //Don't add any lines after this line
    WLAN_STATES_COUNT
};

enum wlan_power_params {
    CUMULATIVE_SLEEP_TIME_MS = 0,
    CUMULATIVE_TOTAL_ON_TIME_MS,
    DEEP_SLEEP_ENTER_COUNTER,
    LAST_DEEP_SLEEP_ENTER_TSTAMP_MS,

    //Don't add any lines after this line
    WLAN_POWER_PARAMS_COUNT
};


#define PLATFORM_SLEEP_MODES_COUNT RPM_MODE_MAX

#define MAX_RPM_PARAMS 2
#define XO_VOTERS (MAX_PLATFORM_STATS - XO_VOTERS_START)
#define VMIN_VOTERS 0

struct stat_pair {
    enum stats_type stat;
    const char *label;
    const char **parameters;
    size_t num_parameters;
};

int extract_platform_stats(uint64_t *list);
int extract_wlan_stats(uint64_t *list);
void set_feature(feature_t feature, int state);

#ifdef __cplusplus
}
#endif

#endif //__POWER_HELPER_H__
