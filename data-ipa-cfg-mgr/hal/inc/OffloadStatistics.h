
#ifndef _OFFLOAD_STATISTICS_H_
#define _OFFLOAD_STATISTICS_H_

#include <string>
#include <sys/types.h>


class OffloadStatistics {
public:
    uint64_t rx;
    uint64_t tx;

    OffloadStatistics();
    OffloadStatistics(std::string /* upstream */);

    uint64_t getTotalRxBytes();
    uint64_t getTotalTxBytes();
private:
    std::string upstream;
}; /* OffloadStatistics */
#endif /* _OFFLOAD_STATISTICS_H_ */
