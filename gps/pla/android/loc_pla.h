
#ifndef __LOC_PLA__
#define __LOC_PLA__

#ifdef __cplusplus
#include <utils/SystemClock.h>
#define uptimeMillis android::uptimeMillis
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <cutils/properties.h>
#include <cutils/threads.h>
#include <cutils/sched_policy.h>

#define LOC_PATH_GPS_CONF_STR      "/vendor/etc/gps.conf"
#define LOC_PATH_IZAT_CONF_STR     "/vendor/etc/izat.conf"
#define LOC_PATH_FLP_CONF_STR      "/vendor/etc/flp.conf"
#define LOC_PATH_LOWI_CONF_STR     "/vendor/etc/lowi.conf"
#define LOC_PATH_SAP_CONF_STR      "/vendor/etc/sap.conf"
#define LOC_PATH_APDR_CONF_STR     "/vendor/etc/apdr.conf"
#define LOC_PATH_XTWIFI_CONF_STR   "/vendor/etc/xtwifi.conf"
#define LOC_PATH_QUIPC_CONF_STR    "/vendor/etc/quipc.conf"

#ifdef __cplusplus
}
#endif /*__cplusplus */

#endif /* __LOC_PLA__ */
