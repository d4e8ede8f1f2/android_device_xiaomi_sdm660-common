
#define LOG_NDEBUG 0
#define LOG_TAG "LocSvc_MsgTask"

#include <unistd.h>
#include <MsgTask.h>
#include <msg_q.h>
#include <log_util.h>
#include <loc_log.h>
#include <loc_pla.h>

static void LocMsgDestroy(void* msg) {
    delete (LocMsg*)msg;
}

MsgTask::MsgTask(LocThread::tCreate tCreator,
                 const char* threadName, bool joinable) :
    mQ(msg_q_init2()), mThread(new LocThread()) {
    if (!mThread->start(tCreator, threadName, this, joinable)) {
        delete mThread;
        mThread = NULL;
    }
}

MsgTask::MsgTask(const char* threadName, bool joinable) :
    mQ(msg_q_init2()), mThread(new LocThread()) {
    if (!mThread->start(threadName, this, joinable)) {
        delete mThread;
        mThread = NULL;
    }
}

MsgTask::~MsgTask() {
    msg_q_flush((void*)mQ);
    msg_q_destroy((void**)&mQ);
}

void MsgTask::destroy() {
    LocThread* thread = mThread;
    msg_q_unblock((void*)mQ);
    if (thread) {
        mThread = NULL;
        delete thread;
    } else {
        delete this;
    }
}

void MsgTask::sendMsg(const LocMsg* msg) const {
    if (msg) {
        msg_q_snd((void*)mQ, (void*)msg, LocMsgDestroy);
    } else {
        LOC_LOGE("%s: msg is NULL", __func__);
    }
}

void MsgTask::prerun() {
    // make sure we do not run in background scheduling group
     set_sched_policy(gettid(), SP_FOREGROUND);
}

bool MsgTask::run() {
    LocMsg* msg;
    msq_q_err_type result = msg_q_rcv((void*)mQ, (void **)&msg);
    if (eMSG_Q_SUCCESS != result) {
        LOC_LOGE("%s:%d] fail receiving msg: %s\n", __func__, __LINE__,
                 loc_get_msg_q_status(result));
        return false;
    }

    msg->log();
    // there is where each individual msg handling is invoked
    msg->proc();

    delete msg;

    return true;
}
