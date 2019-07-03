
#ifndef _BDROID_BUILDCFG_H
#define _BDROID_BUILDCFG_H

#include <cutils/properties.h>
#include <string.h>

static inline const char* BtmGetDefaultName()
{
    char product_model[PROPERTY_VALUE_MAX];
    property_get("ro.product.model", product_model, "");

    if (strstr(product_model, "Mi A2"))
        return "Mi A2";
    if (strstr(product_model, "MI 6X"))
        return "MI 6X";
    if (strstr(product_model, "MI PAD 4"))
        return "MI PAD 4";
    if (strstr(product_model, "MI PAD 4 PLUS"))
        return "MI PAD 4 PLUS";

    // Fallback to ro.product.model
    return "";
}
#undef PROPERTY_VALUE_MAX

#define BTM_DEF_LOCAL_NAME BtmGetDefaultName()
#define BLUETOOTH_QTI_SW TRUE
// Disables read remote device feature
#define MAX_ACL_CONNECTIONS   16
#define MAX_L2CAP_CHANNELS    16
#define BLE_VND_INCLUDED   TRUE
// skips conn update at conn completion
#define BT_CLEAN_TURN_ON_DISABLED 1

/* Increasing SEPs to 12 from 6 to support SHO/MCast i.e. two streams per codec */
#define AVDT_NUM_SEPS 12

#endif
