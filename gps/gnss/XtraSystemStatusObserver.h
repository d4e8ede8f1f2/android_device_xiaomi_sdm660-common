
#ifndef XTRA_SYSTEM_STATUS_OBS_H
#define XTRA_SYSTEM_STATUS_OBS_H

#include <cinttypes>
#include <MsgTask.h>
#include <LocIpc.h>
#include <LocTimer.h>

using namespace std;
using loc_core::IOsObserver;
using loc_core::IDataItemObserver;
using loc_core::IDataItemCore;
using loc_util::LocIpc;

class XtraSystemStatusObserver : public IDataItemObserver, public LocIpc{
public :
    // constructor & destructor
    inline XtraSystemStatusObserver(IOsObserver* sysStatObs, const MsgTask* msgTask):
            mSystemStatusObsrvr(sysStatObs), mMsgTask(msgTask),
            mGpsLock(-1), mConnections(0), mXtraThrottle(true), mReqStatusReceived(false),
            mDelayLocTimer(*this), mIsConnectivityStatusKnown (false) {
        subscribe(true);
        startListeningNonBlocking(LOC_IPC_HAL);
        mDelayLocTimer.start(100 /*.1 sec*/,  false);
    }
    inline virtual ~XtraSystemStatusObserver() {
        subscribe(false);
        stopListening();
    }

    // IDataItemObserver overrides
    inline virtual void getName(string& name);
    virtual void notify(const list<IDataItemCore*>& dlist);

    bool updateLockStatus(uint32_t lock);
    bool updateConnections(uint64_t allConnections);
    bool updateTac(const string& tac);
    bool updateMccMnc(const string& mccmnc);
    bool updateXtraThrottle(const bool enabled);
    inline const MsgTask* getMsgTask() { return mMsgTask; }
    void subscribe(bool yes);

protected:
    void onReceive(const std::string& data) override;

private:
    IOsObserver*    mSystemStatusObsrvr;
    const MsgTask* mMsgTask;
    int32_t mGpsLock;
    uint64_t mConnections;
    string mTac;
    string mMccmnc;
    bool mXtraThrottle;
    bool mReqStatusReceived;
    bool mIsConnectivityStatusKnown;

    class DelayLocTimer : public LocTimer {
        XtraSystemStatusObserver& mXSSO;
    public:
        DelayLocTimer(XtraSystemStatusObserver& xsso) : mXSSO(xsso) {}
        void timeOutCallback() override {
            mXSSO.send(LOC_IPC_XTRA, "halinit");
        }
    } mDelayLocTimer;

    bool onStatusRequested(int32_t xtraStatusUpdated);
};

#endif
