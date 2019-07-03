
#ifndef __IOSOBSERVER_H__
#define __IOSOBSERVER_H__

#include  <list>
#include <string>
#include <IDataItemObserver.h>
#include <IDataItemSubscription.h>
#include <IFrameworkActionReq.h>

using namespace std;

namespace loc_core
{

/**
 * @brief IOsObserver interface
 * @details IOsObserver interface;
 *          In OS dependent code this type serves as a handle to
 *          an OS independent instance of this interface.
 */
class IOsObserver :
                public IDataItemObserver,
                public IDataItemSubscription,
                public IFrameworkActionReq {

public:

    // To set the subscription object
    virtual void setSubscriptionObj(IDataItemSubscription *subscriptionObj) = 0;

    // To set the framework action request object
    virtual void setFrameworkActionReqObj(IFrameworkActionReq *frameworkActionReqObj) = 0;

    // IDataItemObserver Overrides
    inline virtual void getName (string & /*name*/) {}
    inline virtual void notify (const std::list <IDataItemCore *> & /*dlist*/) {}

    // IDataItemSubscription Overrides
    inline virtual void subscribe
    (
        const std :: list <DataItemId> & /*l*/,
        IDataItemObserver * /*client*/
    ){}
    inline virtual void updateSubscription
    (
        const std :: list <DataItemId> & /*l*/,
        IDataItemObserver * /*client*/
    ){}
    inline virtual void requestData
    (
        const std :: list <DataItemId> & /*l*/,
        IDataItemObserver * /*client*/
    ){}
    inline virtual void unsubscribe
    (
        const std :: list <DataItemId> & /*l*/,
        IDataItemObserver * /*client*/
    ){}
    inline virtual void unsubscribeAll (IDataItemObserver * /*client*/){}

    // IFrameworkActionReq Overrides
    inline virtual void turnOn (DataItemId /*dit*/, int /*timeOut*/){}
    inline virtual void turnOff (DataItemId /*dit*/) {}
#ifdef USE_GLIB
    inline virtual bool connectBackhaul() {}
    inline virtual bool disconnectBackhaul() {}
#endif

    /**
     * @brief Destructor
     * @details Destructor
     */
    virtual ~IOsObserver () {}
};

} // namespace loc_core

#endif // #ifndef __IOSOBSERVER_H__
