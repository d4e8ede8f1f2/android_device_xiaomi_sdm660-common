
#define LOG_NIDEBUG 0
#define LOG_TAG "android.hardware.power@1.3-service.sdm660-libperfmgr"

#include <errno.h>
#include <inttypes.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>

#include <log/log.h>

#include "power-helper.h"

#ifndef RPM_SYSTEM_STAT
#define RPM_SYSTEM_STAT "/d/system_stats"
#endif

#ifndef WLAN_POWER_STAT
#define WLAN_POWER_STAT "/d/wlan0/power_stats"
#endif

#ifndef TAP_TO_WAKE_NODE
#define TAP_TO_WAKE_NODE "/proc/nvt_wake_gesture"
#endif

#ifndef TAP_TO_WAKE_NODE2
#define TAP_TO_WAKE_NODE2 "/proc/touchscreen/enable_dt2w"
#endif

#define ARRAY_SIZE(x) (sizeof((x))/sizeof((x)[0]))
#define LINE_SIZE 128

const char *rpm_stat_params[MAX_RPM_PARAMS] = {
    "count",
    "actual last sleep(msec)",
};

const char *master_stat_params[MAX_RPM_PARAMS] = {
    "Accumulated XO duration",
    "XO Count",
};

struct stat_pair rpm_stat_map[] = {
    { RPM_MODE_XO,   "RPM Mode:vlow", rpm_stat_params, ARRAY_SIZE(rpm_stat_params) },
    { RPM_MODE_VMIN, "RPM Mode:vmin", rpm_stat_params, ARRAY_SIZE(rpm_stat_params) },
    { VOTER_APSS,    "APSS",    master_stat_params, ARRAY_SIZE(master_stat_params) },
    { VOTER_MPSS,    "MPSS",    master_stat_params, ARRAY_SIZE(master_stat_params) },
    { VOTER_ADSP,    "ADSP",    master_stat_params, ARRAY_SIZE(master_stat_params) },
    { VOTER_SLPI,    "SLPI",    master_stat_params, ARRAY_SIZE(master_stat_params) },
};


const char *wlan_power_stat_params[] = {
    "cumulative_sleep_time_ms",
    "cumulative_total_on_time_ms",
    "deep_sleep_enter_counter",
    "last_deep_sleep_enter_tstamp_ms"
};

struct stat_pair wlan_stat_map[] = {
    { WLAN_POWER_DEBUG_STATS, "POWER DEBUG STATS", wlan_power_stat_params, ARRAY_SIZE(wlan_power_stat_params) },
};

static int sysfs_write(char *path, char *s)
{
    char buf[80];
    int len;
    int ret = 0;
    int fd = open(path, O_WRONLY);

    if (fd < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error opening %s: %s\n", path, buf);
        return -1 ;
    }

    len = write(fd, s, strlen(s));
    if (len < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error writing to %s: %s\n", path, buf);

        ret = -1;
    }

    close(fd);

    return ret;
}

void set_feature(feature_t feature, int state) {
    switch (feature) {
        case POWER_FEATURE_DOUBLE_TAP_TO_WAKE:
            sysfs_write(TAP_TO_WAKE_NODE, state ? "1" : "0");
            sysfs_write(TAP_TO_WAKE_NODE2, state ? "1" : "0");
            break;
        default:
            break;
    }
}

static int parse_stats(const char **params, size_t params_size,
                       uint64_t *list, FILE *fp) {
    ssize_t nread;
    size_t len = LINE_SIZE;
    char *line;
    size_t params_read = 0;
    size_t i;

    line = malloc(len);
    if (!line) {
        ALOGE("%s: no memory to hold line", __func__);
        return -ENOMEM;
    }

    while ((params_read < params_size) &&
        (nread = getline(&line, &len, fp) > 0)) {
        char *key = line + strspn(line, " \t");
        char *value = strchr(key, ':');
        if (!value || (value > (line + len)))
            continue;
        *value++ = '\0';

        for (i = 0; i < params_size; i++) {
            if (!strcmp(key, params[i])) {
                list[i] = strtoull(value, NULL, 0);
                params_read++;
                break;
            }
        }
    }
    free(line);

    return 0;
}


static int extract_stats(uint64_t *list, char *file,
                         struct stat_pair *map, size_t map_size) {
    FILE *fp;
    ssize_t read;
    size_t len = LINE_SIZE;
    char *line;
    size_t i, stats_read = 0;
    int ret = 0;

    fp = fopen(file, "re");
    if (fp == NULL) {
        ALOGE("%s: failed to open: %s Error = %s", __func__, file, strerror(errno));
        return -errno;
    }

    line = malloc(len);
    if (!line) {
        ALOGE("%s: no memory to hold line", __func__);
        fclose(fp);
        return -ENOMEM;
    }

    while ((stats_read < map_size) && (read = getline(&line, &len, fp) != -1)) {
        size_t begin = strspn(line, " \t");

        for (i = 0; i < map_size; i++) {
            if (!strncmp(line + begin, map[i].label, strlen(map[i].label))) {
                stats_read++;
                break;
            }
        }

        if (i == map_size)
            continue;

        ret = parse_stats(map[i].parameters, map[i].num_parameters,
                          &list[map[i].stat * MAX_RPM_PARAMS], fp);
        if (ret < 0)
            break;
    }
    free(line);
    fclose(fp);

    return ret;
}

int extract_platform_stats(uint64_t *list) {
    return extract_stats(list, RPM_SYSTEM_STAT, rpm_stat_map, ARRAY_SIZE(rpm_stat_map));
}

int extract_wlan_stats(uint64_t *list) {
    return extract_stats(list, WLAN_POWER_STAT, wlan_stat_map, ARRAY_SIZE(wlan_stat_map));
}
