
#ifndef __IFRAMEWORKACTIONREQ_H__
#define __IFRAMEWORKACTIONREQ_H__

#include  <DataItemId.h>

namespace loc_core
{

/**
 * @brief IFrameworkActionReq interface
 * @details IFrameworkActionReq interface;
 *          Defines an interface for operations such as turnOn, turnOff a
 *          framework module described by the data item. Framework module
 *          could be bluetooth, wifi etc.
 *          Must be implemented by OS dependent code.
 *
 */
class IFrameworkActionReq {

public:
    /**
     * @brief Turn on the framework module described by the data item.
     * @details  Turn on the framework module described by the data item;
     *          An IFrameworkActionReq implementer invokes this method to
     *          turn on the framework module described by the data item.
     *          Framework module could be bluetooth, wifi etc.
     *
     * @param dit DataItemId
     * @param timeout Timeout after which to turn off the framework module.
     */
    virtual void turnOn (DataItemId dit, int timeOut = 0) = 0;

    /**
     * @brief Turn off the framework module described by the data item.
     * @details  Turn off the framework module described by the data item;
     *          An IFrameworkActionReq implementer invokes this method to
     *          turn off the framework module described by the data item.
     *          Framework module could be bluetooth, wifi etc.
     *
     * @param dit DataItemId
     */
    virtual void turnOff (DataItemId dit) = 0;

#ifdef USE_GLIB
    /**
     * @brief Setup WWAN backhaul
     * @details  Setup WWAN backhaul
     *
     * @param None
     */
    virtual bool connectBackhaul() = 0;

    /**
     * @brief Disconnects the WWANbackhaul
     * @details Disconnects the WWANbackhaul, only if it was setup by us
     *
     * @param None
     */
    virtual bool disconnectBackhaul() = 0;
#endif

    /**
     * @brief Destructor
     * @details Destructor
     */
    virtual ~IFrameworkActionReq () {}
};

} // namespace loc_core

#endif // #ifndef __IFRAMEWORKACTIONREQ_H__
