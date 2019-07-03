
#ifndef __SYSTEM_STATUS_OSOBSERVER__
#define __SYSTEM_STATUS_OSOBSERVER__

#include <cinttypes>
#include <string>
#include <list>
#include <map>
#include <new>
#include <vector>

#include <MsgTask.h>
#include <DataItemId.h>
#include <IOsObserver.h>
#include <loc_pla.h>
#include <log_util.h>
#include <LocUnorderedSetMap.h>

namespace loc_core
{
/******************************************************************************
 SystemStatusOsObserver
******************************************************************************/
using namespace std;
using namespace loc_util;

// Forward Declarations
class IDataItemCore;
class SystemStatus;
class SystemStatusOsObserver;
typedef map<IDataItemObserver*, list<DataItemId>> ObserverReqCache;
typedef LocUnorderedSetMap<IDataItemObserver*, DataItemId> ClientToDataItems;
typedef LocUnorderedSetMap<DataItemId, IDataItemObserver*> DataItemToClients;
typedef unordered_map<DataItemId, IDataItemCore*> DataItemIdToCore;
typedef unordered_map<DataItemId, int> DataItemIdToInt;

struct ObserverContext {
    IDataItemSubscription* mSubscriptionObj;
    IFrameworkActionReq* mFrameworkActionReqObj;
    const MsgTask* mMsgTask;
    SystemStatusOsObserver* mSSObserver;

    inline ObserverContext(const MsgTask* msgTask, SystemStatusOsObserver* observer) :
            mSubscriptionObj(NULL), mFrameworkActionReqObj(NULL),
            mMsgTask(msgTask), mSSObserver(observer) {}
};

// Clients wanting to get data from OS/Framework would need to
// subscribe with OSObserver using IDataItemSubscription interface.
// Such clients would need to implement IDataItemObserver interface
// to receive data when it becomes available.
class SystemStatusOsObserver : public IOsObserver {

public:
    // ctor
    inline SystemStatusOsObserver(SystemStatus* systemstatus, const MsgTask* msgTask) :
            mSystemStatus(systemstatus), mContext(msgTask, this),
            mAddress("SystemStatusOsObserver"),
            mClientToDataItems(MAX_DATA_ITEM_ID), mDataItemToClients(MAX_DATA_ITEM_ID)
#ifdef USE_GLIB
            , mBackHaulConnectReqCount(0)
#endif
    {
    }

    // dtor
    ~SystemStatusOsObserver();

    template <typename CINT, typename COUT>
    static COUT containerTransfer(CINT& s);
    template <typename CINT, typename COUT>
    inline static COUT containerTransfer(CINT&& s) {
        return containerTransfer<CINT, COUT>(s);
    }

    // To set the subscription object
    virtual void setSubscriptionObj(IDataItemSubscription* subscriptionObj);

    // To set the framework action request object
    inline void setFrameworkActionReqObj(IFrameworkActionReq* frameworkActionReqObj) {
        mContext.mFrameworkActionReqObj = frameworkActionReqObj;
#ifdef USE_GLIB
        if (mBackHaulConnectReqCount > 0) {
            connectBackhaul();
            mBackHaulConnectReqCount = 0;
        }
#endif
    }

    // IDataItemSubscription Overrides
    inline virtual void subscribe(const list<DataItemId>& l, IDataItemObserver* client) override {
        subscribe(l, client, false);
    }
    virtual void updateSubscription(const list<DataItemId>& l, IDataItemObserver* client) override;
    inline virtual void requestData(const list<DataItemId>& l, IDataItemObserver* client) override {
        subscribe(l, client, true);
    }
    virtual void unsubscribe(const list<DataItemId>& l, IDataItemObserver* client) override;
    virtual void unsubscribeAll(IDataItemObserver* client) override;

    // IDataItemObserver Overrides
    virtual void notify(const list<IDataItemCore*>& dlist) override;
    inline virtual void getName(string& name) override {
        name = mAddress;
    }

    // IFrameworkActionReq Overrides
    virtual void turnOn(DataItemId dit, int timeOut = 0) override;
    virtual void turnOff(DataItemId dit) override;
#ifdef USE_GLIB
    virtual bool connectBackhaul() override;
    virtual bool disconnectBackhaul();
#endif

private:
    SystemStatus*                                    mSystemStatus;
    ObserverContext                                  mContext;
    const string                                     mAddress;
    ClientToDataItems                                mClientToDataItems;
    DataItemToClients                                mDataItemToClients;
    DataItemIdToCore                                 mDataItemCache;
    DataItemIdToInt                                  mActiveRequestCount;

    // Cache the subscribe and requestData till subscription obj is obtained
    void cacheObserverRequest(ObserverReqCache& reqCache,
            const list<DataItemId>& l, IDataItemObserver* client);
#ifdef USE_GLIB
    // Cache the framework action request for connect/disconnect
    int         mBackHaulConnectReqCount;
#endif

    void subscribe(const list<DataItemId>& l, IDataItemObserver* client, bool toRequestData);

    // Helpers
    void sendCachedDataItems(const unordered_set<DataItemId>& s, IDataItemObserver* to);
    bool updateCache(IDataItemCore* d);
    inline void logMe(const unordered_set<DataItemId>& l) {
        IF_LOC_LOGD {
            for (auto id : l) {
                LOC_LOGD("DataItem %d", id);
            }
        }
    }
};

} // namespace loc_core

#endif //__SYSTEM_STATUS__
