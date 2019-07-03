
#include <string.h>
#include <sys/types.h>

#include "OffloadStatistics.h"


/* ------------------------------ PUBLIC ------------------------------------ */
OffloadStatistics::OffloadStatistics() {
    this->upstream = "UNSET";
    this->rx = 0;
    this->tx = 0;
} /* OffloadStatistics */

OffloadStatistics::OffloadStatistics
(
    std::string upstream
) {
    this->upstream = upstream;
    this->rx = 0;
    this->tx =0;
} /* OffloadStatistics */

uint64_t OffloadStatistics::getTotalRxBytes() {
    return rx;
} /* getTotalRxBytes */

uint64_t OffloadStatistics::getTotalTxBytes() {
    return tx;
} /* getTotalTxBytes */
