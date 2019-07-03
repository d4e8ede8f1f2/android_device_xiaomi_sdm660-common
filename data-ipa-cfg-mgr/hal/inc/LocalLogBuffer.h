
#ifndef _LOCAL_LOG_BUFFER_H_
#define _LOCAL_LOG_BUFFER_H_
/* External Includes */
#include <deque>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <vector>

/* Namespace pollution avoidance */
using ::std::deque;
using ::std::string;
using ::std::stringstream;
using ::std::vector;


class LocalLogBuffer {
public:
    class FunctionLog {
    public:
        FunctionLog(string /* funcName */);
        FunctionLog(const FunctionLog& /* other */);
        void addArg(string /* kw */, string /* arg */);
        void addArg(string /* kw */, vector<string> /* args */);
        void addArg(string /* kw */, uint64_t /* arg */);
        void setResult(bool /* success */, string /* msg */);
        void setResult(vector<unsigned int> /* ret */);
        void setResult(uint64_t /* rx */, uint64_t /* tx */);
        string toString();
    private:
        void maybeAddArgsComma();
        const string mName;
        bool mArgsProvided;
        stringstream mSSArgs;
        stringstream mSSReturn;
    }; /* FunctionLog */
    LocalLogBuffer(string /* name */, int /* maxLogs */);
    void addLog(FunctionLog /* log */);
    void toLogcat();
private:
    deque<FunctionLog> mLogs;
    const string mName;
    const size_t mMaxLogs;
}; /* LocalLogBuffer */
#endif /* _LOCAL_LOG_BUFFER_H_ */
