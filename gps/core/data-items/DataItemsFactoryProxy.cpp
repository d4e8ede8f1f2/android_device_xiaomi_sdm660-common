
#define LOG_TAG "DataItemsFactoryProxy"

#include <dlfcn.h>
#include <DataItemId.h>
#include <IDataItemCore.h>
#include <DataItemsFactoryProxy.h>
#include <loc_pla.h>
#include <log_util.h>

namespace loc_core
{
void* DataItemsFactoryProxy::dataItemLibHandle = NULL;
get_concrete_data_item_fn* DataItemsFactoryProxy::getConcreteDIFunc = NULL;

IDataItemCore* DataItemsFactoryProxy::createNewDataItem(DataItemId id)
{
    IDataItemCore *mydi = nullptr;

    if (NULL != getConcreteDIFunc) {
        mydi = (*getConcreteDIFunc)(id);
    }
    else {
        // first call to this function, symbol not yet loaded
        if (NULL == dataItemLibHandle) {
            LOC_LOGD("Loaded library %s",DATA_ITEMS_LIB_NAME);
            dataItemLibHandle = dlopen(DATA_ITEMS_LIB_NAME, RTLD_NOW);
            if (NULL == dataItemLibHandle) {
                // dlopen failed.
                const char * err = dlerror();
                if (NULL == err)
                {
                    err = "Unknown";
                }
                LOC_LOGE("%s:%d]: failed to load library %s; error=%s",
                     __func__, __LINE__, DATA_ITEMS_LIB_NAME, err);
            }
        }

        // load sym - if dlopen handle is obtained and symbol is not yet obtained
        if (NULL != dataItemLibHandle) {
            getConcreteDIFunc = (get_concrete_data_item_fn * )
                                    dlsym(dataItemLibHandle, DATA_ITEMS_GET_CONCRETE_DI);
            if (NULL != getConcreteDIFunc) {
                LOC_LOGD("Loaded function %s : %p",DATA_ITEMS_GET_CONCRETE_DI,getConcreteDIFunc);
                mydi = (*getConcreteDIFunc)(id);
            }
            else {
                // dlysm failed.
                const char * err = dlerror();
                if (NULL == err)
                {
                    err = "Unknown";
                }
                LOC_LOGE("%s:%d]: failed to find symbol %s; error=%s",
                         __func__, __LINE__, DATA_ITEMS_GET_CONCRETE_DI, err);
            }
        }
    }
    return mydi;
}

void DataItemsFactoryProxy::closeDataItemLibraryHandle()
{
    if (NULL != dataItemLibHandle) {
        dlclose(dataItemLibHandle);
        dataItemLibHandle = NULL;
    }
}

} // namespace loc_core
