
#define LOG_TAG "IPAHALService/dump"

/* External Includes */
#include <log/log.h>
#include <deque>
#include <string>
#include <sys/types.h>
#include <vector>

/* Internal Includes */
#include "LocalLogBuffer.h"

/* Namespace pollution avoidance */
using ::std::deque;
using ::std::string;
using ::std::vector;


LocalLogBuffer::FunctionLog::FunctionLog(string funcName) : mName(funcName) {
    mArgsProvided = false;
} /* FunctionLog */

LocalLogBuffer::FunctionLog::FunctionLog(const FunctionLog& other) :
        mName(other.mName) {
    mArgsProvided = other.mArgsProvided;
    /* Is this right? How do you copy stringstreams without wizardry? */
    mSSArgs.str(other.mSSArgs.str());
    mSSReturn.str(other.mSSReturn.str());
} /* FunctionLog */

void LocalLogBuffer::FunctionLog::addArg(string kw, string arg) {
    maybeAddArgsComma();
    mSSArgs << kw << "=" << arg;
} /* addArg */

void LocalLogBuffer::FunctionLog::addArg(string kw, vector<string> args) {
    maybeAddArgsComma();
    mSSArgs << kw << "=[";
    for (size_t i = 0; i < args.size(); i++) {
        mSSArgs << args[i];
        if (i < (args.size() - 1))
            mSSArgs << ", ";
    }
    mSSArgs << "]";
} /* addArg */

void LocalLogBuffer::FunctionLog::addArg(string kw, uint64_t arg) {
    maybeAddArgsComma();
    mSSArgs << kw << "=" << arg;
} /* addArg */

void LocalLogBuffer::FunctionLog::maybeAddArgsComma() {
    if (!mArgsProvided) {
        mArgsProvided = true;
    } else {
        mSSArgs << ", ";
    }
} /* maybeAddArgsComma */

void LocalLogBuffer::FunctionLog::setResult(bool success, string msg) {
    mSSReturn << "[" << ((success) ? "success" : "failure") << ", " << msg
              << "]";
} /* setResult */

void LocalLogBuffer::FunctionLog::setResult(vector<unsigned int> ret) {
    mSSReturn << "[";
    for (size_t i = 0; i < ret.size(); i++) {
        mSSReturn << ret[i];
        if (i < (ret.size() - 1))
            mSSReturn << ", ";
    }
    mSSReturn << "]";
} /* setResult */

void LocalLogBuffer::FunctionLog::setResult(uint64_t rx, uint64_t tx) {
    mSSReturn << "[rx=" << rx << ", tx=" << tx << "]";
} /* setResult */

string LocalLogBuffer::FunctionLog::toString() {
    stringstream ret;
    ret << mName << "(" << mSSArgs.str() << ") returned " << mSSReturn.str();
    return ret.str();
} /* toString */

LocalLogBuffer::LocalLogBuffer(string name, int maxLogs) : mName(name),
        mMaxLogs(maxLogs) {
} /* LocalLogBuffer */

void LocalLogBuffer::addLog(FunctionLog log) {
    while (mLogs.size() > mMaxLogs)
        mLogs.pop_front();
    mLogs.push_back(log);
} /* addLog */

void LocalLogBuffer::toLogcat() {
    for (size_t i = 0; i < mLogs.size(); i++)
        ALOGD("%s: %s", mName.c_str(), mLogs[i].toString().c_str());
} /* toLogcat */
