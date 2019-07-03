
#define LOG_TAG "LocSvc__AGnssRilInterface"

#include <log_util.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sstream>
#include <string>
#include "Gnss.h"
#include "AGnssRil.h"
#include <DataItemConcreteTypesBase.h>

typedef void* (getLocationInterface)();

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {


AGnssRil::AGnssRil(Gnss* gnss) : mGnss(gnss) {
    ENTRY_LOG_CALLFLOW();
}

AGnssRil::~AGnssRil() {
    ENTRY_LOG_CALLFLOW();
}

Return<bool> AGnssRil::updateNetworkState(bool connected, NetworkType type, bool /*roaming*/) {
    ENTRY_LOG_CALLFLOW();

    // for XTRA
    if (nullptr != mGnss && ( nullptr != mGnss->getGnssInterface() )) {
        int8_t typeout = loc_core::NetworkInfoDataItemBase::TYPE_UNKNOWN;
        switch(type)
        {
            case IAGnssRil::NetworkType::MOBILE:
                typeout = loc_core::NetworkInfoDataItemBase::TYPE_MOBILE;
                break;
            case IAGnssRil::NetworkType::WIFI:
                typeout = loc_core::NetworkInfoDataItemBase::TYPE_WIFI;
                break;
            case IAGnssRil::NetworkType::MMS:
                typeout = loc_core::NetworkInfoDataItemBase::TYPE_MMS;
                break;
            case IAGnssRil::NetworkType::SUPL:
                typeout = loc_core::NetworkInfoDataItemBase::TYPE_SUPL;
                break;
            case IAGnssRil::NetworkType::DUN:
                typeout = loc_core::NetworkInfoDataItemBase::TYPE_DUN;
                break;
            case IAGnssRil::NetworkType::HIPRI:
                typeout = loc_core::NetworkInfoDataItemBase::TYPE_HIPRI;
                break;
            case IAGnssRil::NetworkType::WIMAX:
                typeout = loc_core::NetworkInfoDataItemBase::TYPE_WIMAX;
                break;
            default:
                typeout = loc_core::NetworkInfoDataItemBase::TYPE_UNKNOWN;
                break;
        }
        mGnss->getGnssInterface()->updateConnectionStatus(connected, typeout);
    }
    return true;
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
