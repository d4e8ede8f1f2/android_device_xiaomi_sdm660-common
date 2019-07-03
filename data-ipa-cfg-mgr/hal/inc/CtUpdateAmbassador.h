
#ifndef _CT_UPDATE_AMBASSADOR_H_
#define _CT_UPDATE_AMBASSADOR_H_
/* External Includes */
#include <hidl/HidlTransportSupport.h>

/* HIDL Includes */
#include <android/hardware/tetheroffload/control/1.0/ITetheringOffloadCallback.h>

/* Internal Includes */
#include "IOffloadManager.h"

/* Namespace pollution avoidance */
using ::android::hardware::tetheroffload::control::V1_0::ITetheringOffloadCallback;
using ::android::hardware::tetheroffload::control::V1_0::NetworkProtocol;
using HALIpAddrPortPair = ::android::hardware::tetheroffload::control::V1_0::IPv4AddrPortPair;
using HALNatTimeoutUpdate = ::android::hardware::tetheroffload::control::V1_0::NatTimeoutUpdate;

using IpaIpAddrPortPair = ::IOffloadManager::ConntrackTimeoutUpdater::IpAddrPortPair;
using IpaNatTimeoutUpdate = ::IOffloadManager::ConntrackTimeoutUpdater::NatTimeoutUpdate;
using IpaL4Protocol = ::IOffloadManager::ConntrackTimeoutUpdater::L4Protocol;


class CtUpdateAmbassador : public IOffloadManager::ConntrackTimeoutUpdater {
public:
    CtUpdateAmbassador(const ::android::sp<ITetheringOffloadCallback>& /* cb */);
    /* ------------------- CONNTRACK TIMEOUT UPDATER ------------------------ */
    void updateTimeout(IpaNatTimeoutUpdate /* update */);
private:
    static bool translate(IpaNatTimeoutUpdate /* in */, HALNatTimeoutUpdate& /* out */);
    static bool translate(IpaIpAddrPortPair /* in */, HALIpAddrPortPair& /* out */);
    static bool L4ToNetwork(IpaL4Protocol /* in */, NetworkProtocol& /* out */);
    const ::android::sp<ITetheringOffloadCallback>& mFramework;
}; /* CtUpdateAmbassador */
#endif /* _CT_UPDATE_AMBASSADOR_H_ */
