
#ifndef __LOC_PLA__
#define __LOC_PLA__

#ifdef __cplusplus
#include <utils/SystemClock.h>
#define uptimeMillis android::uptimeMillis

extern "C" {
#endif

#include <cutils/properties.h>
#include <cutils/threads.h>
#include <cutils/sched_policy.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#ifndef OFF_TARGET
#include <glib.h>
#define strlcat g_strlcat
#define strlcpy g_strlcpy
#else
#define strlcat strncat
#define strlcpy strncpy
#endif

#define LOC_PATH_GPS_CONF_STR      "/etc/gps.conf"
#define LOC_PATH_IZAT_CONF_STR     "/etc/izat.conf"
#define LOC_PATH_FLP_CONF_STR      "/etc/flp.conf"
#define LOC_PATH_LOWI_CONF_STR     "/etc/lowi.conf"
#define LOC_PATH_SAP_CONF_STR      "/etc/sap.conf"
#define LOC_PATH_APDR_CONF_STR     "/etc/apdr.conf"
#define LOC_PATH_XTWIFI_CONF_STR   "/etc/xtwifi.conf"
#define LOC_PATH_QUIPC_CONF_STR    "/etc/quipc.conf"

#ifdef __cplusplus
}
#endif /*__cplusplus */

#endif /* __LOC_PLA__ */
