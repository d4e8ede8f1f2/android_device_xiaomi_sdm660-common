
ifneq ($(BOARD_VENDOR_QCOM_GPS_LOC_API_HARDWARE),)
ifneq ($(BUILD_TINY_ANDROID), true)

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := liblocation_api
LOCAL_VENDOR_MODULE := true
LOCAL_MODULE_TAGS := optional

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libdl \
	libgps.utils \
	liblog \
	libutils

LOCAL_SRC_FILES += \
	LocationAPI.cpp \
	LocationAPIClientBase.cpp

LOCAL_CFLAGS += \
	-fno-short-enums

LOCAL_HEADER_LIBRARIES := \
	libloc_pla_headers \
	libgps.utils_headers

LOCAL_PRELINK_MODULE := false

LOCAL_CFLAGS += $(GNSS_CFLAGS)
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := liblocation_api_headers
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)
include $(BUILD_HEADER_LIBRARY)

endif
endif
