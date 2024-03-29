
#ifndef DBG
    #define DBG false
#endif /* DBG */
#define LOG_TAG "IPAHALService/CtUpdateAmbassador"

/* External Includes */
#include <arpa/inet.h>
#include <log/log.h>

/* HIDL Includes */
#include <android/hardware/tetheroffload/control/1.0/ITetheringOffloadCallback.h>

/* Internal Includes */
#include "CtUpdateAmbassador.h"

/* Namespace pollution avoidance */
using ::android::hardware::tetheroffload::control::V1_0::ITetheringOffloadCallback;
using ::android::hardware::tetheroffload::control::V1_0::NetworkProtocol;
using HALIpAddrPortPair = ::android::hardware::tetheroffload::control::V1_0::IPv4AddrPortPair;
using HALNatTimeoutUpdate = ::android::hardware::tetheroffload::control::V1_0::NatTimeoutUpdate;

using IpaIpAddrPortPair = ::IOffloadManager::ConntrackTimeoutUpdater::IpAddrPortPair;
using IpaNatTimeoutUpdate = ::IOffloadManager::ConntrackTimeoutUpdater::NatTimeoutUpdate;
using IpaL4Protocol = ::IOffloadManager::ConntrackTimeoutUpdater::L4Protocol;


CtUpdateAmbassador::CtUpdateAmbassador(
        const ::android::sp<ITetheringOffloadCallback>& cb) : mFramework(cb) {
} /* CtUpdateAmbassador */

void CtUpdateAmbassador::updateTimeout(IpaNatTimeoutUpdate in) {
    if (DBG) {
        ALOGD("updateTimeout(src={%#010X, %#04X}, dest={%#010X, %#04X}, Proto=%d)",
                in.src.ipAddr, in.src.port, in.dst.ipAddr, in.dst.port,
                in.proto);
    }
    HALNatTimeoutUpdate out;
    if (!translate(in, out)) {
        /* Cannot log the input outside of DBG flag because it contains sensitive
         * information.  This will lead to a two step debug if the information
         * cannot be gleaned from IPACM logs.  The other option is to improve this
         * with the use of our local log.  That would likely still be hard to
         * instruct testers to collect logs, because, assuming timeout updates
         * are numerous, it will overrun the ring quickly.  Therefore, the tester
         * would have to know the exact moment as issue occurred.  Or we make the
         * ring massive.  This would lead to a significant memory overhead.
         * Because of this overhead, we would likely not want to check in a change
         * with it and once we provide a debug build for increasing buffer size,
         * why not just define the DBG flag?
         */
        ALOGE("Failed to translate timeout event :(");
    } else {
        auto ret = mFramework->updateTimeout(out);
        if (!ret.isOk()) {
            ALOGE("Triggering updateTimeout Callback failed.");
        }
    }
} /* updateTimeout */

bool CtUpdateAmbassador::translate(IpaNatTimeoutUpdate in, HALNatTimeoutUpdate &out) {
    return translate(in.src, out.src)
            && translate(in.dst, out.dst)
            && L4ToNetwork(in.proto, out.proto);
} /* translate */

bool CtUpdateAmbassador::translate(IpaIpAddrPortPair in, HALIpAddrPortPair& out) {
    char ipAddrStr[INET_ADDRSTRLEN];

    if (inet_ntop(AF_INET, &(in.ipAddr), ipAddrStr, INET_ADDRSTRLEN) == nullptr) {
        /* errno would be valid here with EAFNOSUPPORT or ENOSPC, neither should really
         * be possible in our scenario though.
         */
        return false;
    }

    out.addr = ipAddrStr;
    out.port = in.port;

    return true;
} /* translate */

bool CtUpdateAmbassador::L4ToNetwork(IpaL4Protocol in, NetworkProtocol &out) {
    bool ret = false;
    switch(in) {
        case IpaL4Protocol::TCP:
            ret = true;
            out = NetworkProtocol::TCP;
            break;
        case IpaL4Protocol::UDP:
            ret = true;
            out = NetworkProtocol::UDP;
            break;
        default:
            ret = false;
            break;
    }
    return ret;
} /* L4ToNetwork */
