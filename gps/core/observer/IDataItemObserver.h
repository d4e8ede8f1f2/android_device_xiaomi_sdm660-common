
#ifndef __IDATAITEMOBSERVER_H__
#define __IDATAITEMOBSERVER_H__

#include  <list>
#include <string>

using namespace std;

namespace loc_core
{
class IDataItemCore;

/**
 * @brief IDataItemObserver interface
 * @details IDataItemObserver interface;
 *          In OS dependent code this type serves as a handle to an OS independent instance of this interface.
 */
class IDataItemObserver {

public:

    /**
     * @brief Gets name of Data Item Observer
     * @details Gets name of Data Item Observer
     *
     * @param name reference to name of Data Item Observer
     */
    virtual void getName (string & name) = 0;

    /**
     * @brief Notify updated values of Data Items
     * @details Notifys updated values of Data items
     *
     * @param dlist List of updated data items
     */
    virtual void notify (const std :: list <IDataItemCore *> & dlist)  = 0;

    /**
     * @brief Destructor
     * @details Destructor
     */
    virtual ~IDataItemObserver () {}
};

} // namespace loc_core

#endif // #ifndef __IDATAITEMOBSERVER_H__
