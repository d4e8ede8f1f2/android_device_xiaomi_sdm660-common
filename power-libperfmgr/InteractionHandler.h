
#ifndef INTERACTIONHANDLER_H
#define INTERACTIONHANDLER_H

#include <condition_variable>
#include <mutex>
#include <thread>

#include <perfmgr/HintManager.h>

using ::android::perfmgr::HintManager;

enum interaction_state {
    INTERACTION_STATE_UNINITIALIZED,
    INTERACTION_STATE_IDLE,
    INTERACTION_STATE_INTERACTION,
    INTERACTION_STATE_WAITING,
};

struct InteractionHandler {
    InteractionHandler(std::shared_ptr<HintManager> const & hint_manager);
    ~InteractionHandler();
    bool Init();
    void Exit();
    void Acquire(int32_t duration);

 private:
    void Release();
    void WaitForIdle(int32_t wait_ms, int32_t timeout_ms);
    void AbortWaitLocked();
    void Routine();

    void PerfLock();
    void PerfRel();

    long long CalcTimespecDiffMs(struct timespec start, struct timespec end);

    enum interaction_state mState;

    int mIdleFd;
    int mEventFd;

    int32_t mWaitMs;
    int32_t mMinDurationMs;
    int32_t mMaxDurationMs;
    int32_t mDurationMs;

    struct timespec mLastTimespec;

    std::unique_ptr<std::thread> mThread;
    std::mutex mLock;
    std::condition_variable mCond;
    std::shared_ptr<HintManager> mHintManager;
};

#endif //INTERACTIONHANDLER_H
