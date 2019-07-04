
ifneq ($(BOARD_VENDOR_QCOM_GPS_LOC_API_HARDWARE),)
ifneq ($(BUILD_TINY_ANDROID), true)

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libloc_core
LOCAL_VENDOR_MODULE := true
LOCAL_MODULE_TAGS := optional

ifeq ($(TARGET_DEVICE), apq8026_lw)
LOCAL_CFLAGS += -DPDK_FEATURE_SET
else ifeq ($(BOARD_VENDOR_QCOM_LOC_PDK_FEATURE_SET),true)
LOCAL_CFLAGS += -DPDK_FEATURE_SET
endif

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libdl \
	libgps.utils \
	liblog \
	libutils

LOCAL_SRC_FILES += \
	ContextBase.cpp \
	data-items/DataItemsFactoryProxy.cpp \
	LocAdapterBase.cpp \
	LocApiBase.cpp \
	LocDualContext.cpp \
	loc_core_log.cpp \
	SystemStatus.cpp \
	SystemStatusOsObserver.cpp

LOCAL_CFLAGS += \
	-D_ANDROID_ \
	-fno-short-enums

LOCAL_C_INCLUDES:= \
	$(LOCAL_PATH)/data-items \
	$(LOCAL_PATH)/data-items/common \
	$(LOCAL_PATH)/observer

LOCAL_HEADER_LIBRARIES := \
	libgps.utils_headers \
	liblocation_api_headers \
	libloc_pla_headers \
	libutils_headers

LOCAL_CFLAGS += $(GNSS_CFLAGS)

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libloc_core_headers
LOCAL_EXPORT_C_INCLUDE_DIRS := \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/data-items \
	$(LOCAL_PATH)/data-items/common \
	$(LOCAL_PATH)/observer
include $(BUILD_HEADER_LIBRARY)

endif
endif
