
#ifndef _PREFIX_PARSER_H_
#define _PREFIX_PARSER_H_

/* External Includes */
#include <string.h>
#include <sys/types.h>
#include <vector>

/* Internal Includes */
#include "IOffloadManager.h"

/* Avoiding namespace pollution */
using IP_FAM = ::IOffloadManager::IP_FAM;
using Prefix = ::IOffloadManager::Prefix;

using ::std::string;
using ::std::vector;


class PrefixParser {
public:
    PrefixParser();
    bool add(vector<string> /* in */);
    bool add(string /* in */);
    bool addV4(vector<string> /* in */);
    bool addV4(string /* in */);
    bool addV6(vector<string> /* in */);
    bool addV6(string /* in */);
    int size();
    bool allAreFullyQualified();
    Prefix getFirstPrefix();
    Prefix getFirstPrefix(IP_FAM);
    string getLastErrAsStr();
private:
    bool add(string /* in */, IP_FAM /* famHint */);
    bool add(vector<string> /* in */, IP_FAM /* famHint */);
    static IP_FAM guessIPFamily(string /* in */);
    static bool splitIntoAddrAndMask(string /* in */, string& /* addr */,
            string& /* mask */);
    static int parseSubnetMask(string /* in */, IP_FAM /* famHint */);
    static bool parseV4Addr(string /* in */, Prefix& /* out */);
    static bool parseV6Addr(string /* in */, Prefix& /* out */);
    static bool populateV4Mask(int /* mask */, Prefix& /* out */);
    static bool populateV6Mask(int /* mask */, Prefix& /* out */);
    static uint32_t createMask(int /* mask */);
    static Prefix makeBlankPrefix(IP_FAM /* famHint */);
    bool isMaskValid(int /* mask */, IP_FAM /* fam */);
    static const uint32_t FULLY_QUALIFIED_MASK = ~0;
    vector<Prefix> mPrefixes;
    string mLastErr;
}; /* PrefixParser */
#endif /* _PREFIX_PARSER_H_ */
