
#ifndef __LOC_SOCKET__
#define __LOC_SOCKET__

#include <string>
#include <memory>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <LocThread.h>

namespace loc_util {

class LocIpcSender;

class LocIpc {
friend LocIpcSender;
public:
    inline LocIpc() : mIpcFd(-1), mRunnable(nullptr) {}
    inline virtual ~LocIpc() { stopListening(); }

    // Listen for new messages in current thread. Calling this funciton will
    // block current thread. The listening can be stopped by calling stopListening().
    //
    // Argument name is the path of the unix local socket to be listened.
    // The function will return true on success, and false on failure.
    bool startListeningBlocking(const std::string& name);

    // Create a new LocThread and listen for new messages in it.
    // Calling this function will return immediately and won't block current thread.
    // The listening can be stopped by calling stopListening().
    //
    // Argument name is the path of the unix local socket to be be listened.
    // The function will return true on success, and false on failure.
    bool startListeningNonBlocking(const std::string& name);

    // Stop listening to new messages.
    void stopListening();

    // Send out a message.
    // Call this function to send a message in argument data to socket in argument name.
    //
    // Argument name contains the name of the target unix socket. data contains the
    // message to be sent out. Convert your message to a string before calling this function.
    // The function will return true on success, and false on failure.
    static bool send(const char name[], const std::string& data);
    static bool send(const char name[], const uint8_t data[], uint32_t length);

protected:
    // Callback function for receiving incoming messages.
    // Override this function in your derived class to process incoming messages.
    // For each received message, this callback function will be called once.
    // This callback function will be called in the calling thread of startListeningBlocking
    // or in the new LocThread created by startListeningNonBlocking.
    //
    // Argument data contains the received message. You need to parse it.
    inline virtual void onReceive(const std::string& /*data*/) {}

    // LocIpc client can overwrite this function to get notification
    // when the socket for LocIpc is ready to receive messages.
    inline virtual void onListenerReady() {}

private:
    static bool sendData(int fd, const sockaddr_un& addr,
            const uint8_t data[], uint32_t length);

    int mIpcFd;
    LocThread mThread;
    LocRunnable *mRunnable;
};

class LocIpcSender {
public:
    // Constructor of LocIpcSender class
    //
    // Argument destSocket contains the full path name of destination socket.
    // This class hides generated fd and destination address object from user.
    inline LocIpcSender(const char* destSocket):
            LocIpcSender(std::make_shared<int>(::socket(AF_UNIX, SOCK_DGRAM, 0)), destSocket) {
        if (-1 == *mSocket) {
            mSocket = nullptr;
        }
    }

    // Replicate a new LocIpcSender object with new destination socket.
    inline LocIpcSender* replicate(const char* destSocket) {
        return (nullptr == mSocket) ? nullptr : new LocIpcSender(mSocket, destSocket);
    }

    inline ~LocIpcSender() {
        if (nullptr != mSocket && mSocket.unique()) {
            ::close(*mSocket);
        }
    }

    // Send out a message.
    // Call this function to send a message
    //
    // Argument data and length contains the message to be sent out.
    // Return true when succeeded
    inline bool send(const uint8_t data[], uint32_t length) {
        bool rtv = false;
        if (nullptr != mSocket && nullptr != data) {
            rtv = LocIpc::sendData(*mSocket, mDestAddr, data, length);
        }
        return rtv;
    }

private:
    std::shared_ptr<int> mSocket;
    struct sockaddr_un mDestAddr;

    inline LocIpcSender(
            const std::shared_ptr<int>& mySocket, const char* destSocket) : mSocket(mySocket) {
        if ((nullptr != mSocket) && (-1 != *mSocket) && (nullptr != destSocket)) {
            mDestAddr.sun_family = AF_UNIX;
            snprintf(mDestAddr.sun_path, sizeof(mDestAddr.sun_path), "%s", destSocket);
        }
    }
};

}

#endif //__LOC_SOCKET__
