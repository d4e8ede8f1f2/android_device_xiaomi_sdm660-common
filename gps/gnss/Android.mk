
ifneq ($(BOARD_VENDOR_QCOM_GPS_LOC_API_HARDWARE),)
ifneq ($(BUILD_TINY_ANDROID), true)

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libgnss
LOCAL_VENDOR_MODULE := true
LOCAL_MODULE_TAGS := optional

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libdl \
	libgps.utils \
	liblog \
	libloc_core \
	libutils

LOCAL_SRC_FILES += \
	Agps.cpp \
	GnssAdapter.cpp \
	location_gnss.cpp \
	XtraSystemStatusObserver.cpp

LOCAL_CFLAGS += \
	-fno-short-enums \

ifeq ($(TARGET_BUILD_VARIANT), user)
 LOCAL_CFLAGS += -DTARGET_BUILD_VARIANT_USER
endif

LOCAL_HEADER_LIBRARIES := \
	libgps.utils_headers \
    liblocation_api_headers \
	libloc_core_headers \
	libloc_pla_headers

LOCAL_CFLAGS += $(GNSS_CFLAGS)

LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)

endif
endif
