
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <log_util.h>
#include "LocIpc.h"

namespace loc_util {

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "LocSvc_LocIpc"

#define LOC_MSG_BUF_LEN 8192
#define LOC_MSG_HEAD "$MSGLEN$"
#define LOC_MSG_ABORT "LocIpcMsg::ABORT"

class LocIpcRunnable : public LocRunnable {
friend LocIpc;
public:
    LocIpcRunnable(LocIpc& locIpc, const std::string& ipcName)
            : mLocIpc(locIpc), mIpcName(ipcName) {}
    bool run() override {
        if (!mLocIpc.startListeningBlocking(mIpcName)) {
            LOC_LOGe("listen to socket failed");
        }

        return false;
    }
private:
     LocIpc& mLocIpc;
     const std::string mIpcName;
};

bool LocIpc::startListeningNonBlocking(const std::string& name) {
    mRunnable = new LocIpcRunnable(*this, name);
    std::string threadName("LocIpc-");
    threadName.append(name);
    return mThread.start(threadName.c_str(), mRunnable);
}

bool LocIpc::startListeningBlocking(const std::string& name) {
    bool stopRequested = false;
    int fd = socket(AF_UNIX, SOCK_DGRAM, 0);

    if (fd < 0) {
        LOC_LOGe("create socket error. reason:%s", strerror(errno));
        return false;
    }

    if ((unlink(name.c_str()) < 0) && (errno != ENOENT)) {
       LOC_LOGw("unlink socket error. reason:%s", strerror(errno));
    }

    struct sockaddr_un addr = { .sun_family = AF_UNIX };
    snprintf(addr.sun_path, sizeof(addr.sun_path), "%s", name.c_str());

    umask(0157);

    if (::bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        LOC_LOGe("bind socket error. reason:%s", strerror(errno));
    } else {
        mIpcFd = fd;

        // inform that the socket is ready to receive message
        onListenerReady();

        ssize_t nBytes = 0;
        std::string msg = "";
        std::string abort = LOC_MSG_ABORT;
        while (1) {
            msg.resize(LOC_MSG_BUF_LEN);
            nBytes = ::recvfrom(fd, (void*)(msg.data()), msg.size(), 0, NULL, NULL);
            if (nBytes < 0) {
                LOC_LOGe("cannot read socket. reason:%s", strerror(errno));
                break;
            } else if (0 == nBytes) {
                continue;
            }

            if (strncmp(msg.data(), abort.c_str(), abort.length()) == 0) {
                LOC_LOGi("recvd abort msg.data %s", msg.data());
                stopRequested = true;
                break;
            }

            if (strncmp(msg.data(), LOC_MSG_HEAD, sizeof(LOC_MSG_HEAD) - 1)) {
                // short message
                msg.resize(nBytes);
                onReceive(msg);
            } else {
                // long message
                size_t msgLen = 0;
                sscanf(msg.data(), LOC_MSG_HEAD"%zu", &msgLen);
                msg.resize(msgLen);
                size_t msgLenReceived = 0;
                while ((msgLenReceived < msgLen) && (nBytes > 0)) {
                    nBytes = recvfrom(fd, (void*)&(msg[msgLenReceived]),
                                      msg.size() - msgLenReceived, 0, NULL, NULL);
                    msgLenReceived += nBytes;
                }
                if (nBytes > 0) {
                    onReceive(msg);
                } else {
                    LOC_LOGe("cannot read socket. reason:%s", strerror(errno));
                    break;
                }
            }
        }
    }

    if (::close(fd)) {
        LOC_LOGe("cannot close socket:%s", strerror(errno));
    }
    unlink(name.c_str());

    return stopRequested;
}

void LocIpc::stopListening() {
    const char *socketName = nullptr;

    if (mIpcFd >= 0) {
        std::string abort = LOC_MSG_ABORT;
        socketName = (reinterpret_cast<LocIpcRunnable *>(mRunnable))->mIpcName.c_str();
        send(socketName, abort);
        mIpcFd = -1;
    }
    if (mRunnable) {
        mRunnable = nullptr;
    }
}

bool LocIpc::send(const char name[], const std::string& data) {
    return send(name, (const uint8_t*)data.c_str(), data.length());
}

bool LocIpc::send(const char name[], const uint8_t data[], uint32_t length) {

    bool result = true;
    int fd = ::socket(AF_UNIX, SOCK_DGRAM, 0);
    if (fd < 0) {
        LOC_LOGe("create socket error. reason:%s", strerror(errno));
        return false;
    }

    struct sockaddr_un addr = { .sun_family = AF_UNIX };
    snprintf(addr.sun_path, sizeof(addr.sun_path), "%s", name);

    result = sendData(fd, addr, data, length);

    (void)::close(fd);
    return result;
}


bool LocIpc::sendData(int fd, const sockaddr_un &addr, const uint8_t data[], uint32_t length) {

    bool result = true;

    if (length <= LOC_MSG_BUF_LEN) {
        if (::sendto(fd, data, length, 0,
                (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            LOC_LOGe("cannot send to socket. reason:%s", strerror(errno));
            result = false;
        }
    } else {
        std::string head = LOC_MSG_HEAD;
        head.append(std::to_string(length));
        if (::sendto(fd, head.c_str(), head.length(), 0,
                (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            LOC_LOGe("cannot send to socket. reason:%s", strerror(errno));
            result = false;
        } else {
            size_t sentBytes = 0;
            while(sentBytes < length) {
                size_t partLen = length - sentBytes;
                if (partLen > LOC_MSG_BUF_LEN) {
                    partLen = LOC_MSG_BUF_LEN;
                }
                ssize_t rv = ::sendto(fd, data + sentBytes, partLen, 0,
                        (struct sockaddr*)&addr, sizeof(addr));
                if (rv < 0) {
                    LOC_LOGe("cannot send to socket. reason:%s", strerror(errno));
                    result = false;
                    break;
                }
                sentBytes += rv;
            }
        }
    }
    return result;
}

}
