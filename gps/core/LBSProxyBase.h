
#ifndef IZAT_PROXY_BASE_H
#define IZAT_PROXY_BASE_H
#include <gps_extended.h>
#include <MsgTask.h>

namespace loc_core {

class LocApiBase;
class LocAdapterBase;
class ContextBase;

class LBSProxyBase {
    friend class ContextBase;
    inline virtual LocApiBase*
        getLocApi(const MsgTask* msgTask,
                  LOC_API_ADAPTER_EVENT_MASK_T exMask,
                  ContextBase* context) const {

        (void)msgTask;
        (void)exMask;
        (void)context;
        return NULL;
    }
protected:
    inline LBSProxyBase() {}
public:
    inline virtual ~LBSProxyBase() {}
    inline virtual void requestUlp(LocAdapterBase* adapter,
                                   unsigned long capabilities) const {

        (void)adapter;
        (void)capabilities;
    }
    inline virtual bool hasAgpsExtendedCapabilities() const { return false; }
    inline virtual bool hasCPIExtendedCapabilities() const { return false; }
    inline virtual void modemPowerVote(bool power) const {

        (void)power;
    }
    virtual void injectFeatureConfig(ContextBase* context) const {

        (void)context;
    }
    inline virtual bool hasNativeXtraClient() const { return false; }
    inline virtual IzatDevId_t getIzatDevId() const { return 0; }
};

typedef LBSProxyBase* (getLBSProxy_t)();

} // namespace loc_core

#endif // IZAT_PROXY_BASE_H
