
#ifndef __IDATAITEMCORE_H__
#define __IDATAITEMCORE_H__

#include <string>
#include <DataItemId.h>

namespace loc_core {

using namespace std;

/**
 * @brief IDataItemCore interface.
 * @details IDataItemCore interface.
 *
 */
class IDataItemCore {
public:
    /**
     * @brief Gets Data item id.
     * @details Gets Data item id.
     * @return Data item id.
     */
    virtual DataItemId getId () = 0;

    /**
     * @brief Stringify.
     * @details Stringify.
     *
     * @param valueStr Reference to string.
     */
    virtual void stringify (string & valueStr) = 0;

    /**
     * @brief copy.
     * @details copy.
     *
     * @param src Where to copy from.
     * @param dataItemCopied Boolean flag indicated whether or not copied.
     *
     * @return Zero for success or non zero for failure.
     */
    virtual int32_t copy (IDataItemCore * src, bool *dataItemCopied = nullptr) = 0;

    /**
     * @brief Destructor.
     * @details Destructor.
     */
    virtual ~IDataItemCore () {}
};

} // namespace loc_core

#endif // __IDATAITEMCORE_H__
