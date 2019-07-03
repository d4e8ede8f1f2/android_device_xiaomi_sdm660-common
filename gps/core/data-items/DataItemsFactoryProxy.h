
#ifndef __DATAITEMFACTORYBASE__
#define __DATAITEMFACTORYBASE__

#include <DataItemId.h>
#include <IDataItemCore.h>

namespace loc_core
{

#define DATA_ITEMS_LIB_NAME "libdataitems.so"
#define DATA_ITEMS_GET_CONCRETE_DI "getConcreteDataItem"

typedef IDataItemCore * (get_concrete_data_item_fn)(DataItemId);

class DataItemsFactoryProxy {
public:
    static IDataItemCore* createNewDataItem(DataItemId id);
    static void closeDataItemLibraryHandle();
    static void *dataItemLibHandle;
    static get_concrete_data_item_fn *getConcreteDIFunc;
};

} // namespace loc_core

#endif //__DATAITEMFACTORYBASE__
