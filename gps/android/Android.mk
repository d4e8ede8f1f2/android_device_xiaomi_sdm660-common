
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := android.hardware.gnss@1.0-impl-qti
LOCAL_VENDOR_MODULE := true
LOCAL_MODULE_RELATIVE_PATH := hw

LOCAL_SRC_FILES := \
	AGnss.cpp \
	AGnssRil.cpp \
	Gnss.cpp \
	GnssBatching.cpp \
	GnssConfiguration.cpp \
	GnssDebug.cpp \
	GnssGeofencing.cpp \
	GnssMeasurement.cpp \
	GnssNi.cpp

LOCAL_SRC_FILES += \
	location_api/BatchingAPIClient.cpp \
	location_api/GnssAPIClient.cpp \
	location_api/GeofenceAPIClient.cpp \
	location_api/LocationUtil.cpp \
	location_api/MeasurementAPIClient.cpp

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/location_api

LOCAL_HEADER_LIBRARIES := \
	libgps.utils_headers \
	liblocation_api_headers \
	libloc_core_headers \
	libloc_pla_headers

LOCAL_SHARED_LIBRARIES := \
	android.hardware.gnss@1.0 \
	libcutils \
	libhidlbase \
	libhidltransport \
	libhwbinder \
	liblog \
	libutils

LOCAL_SHARED_LIBRARIES += \
	libdl \
	libgps.utils \
	libloc_core \
	liblocation_api

LOCAL_CFLAGS += $(GNSS_CFLAGS)
include $(BUILD_SHARED_LIBRARY)

BUILD_GNSS_HIDL_SERVICE := true
ifneq ($(BOARD_VENDOR_QCOM_LOC_PDK_FEATURE_SET), true)
ifneq ($(LW_FEATURE_SET), true)
BUILD_GNSS_HIDL_SERVICE := false
endif
endif

ifeq ($(BUILD_GNSS_HIDL_SERVICE), true)
include $(CLEAR_VARS)
LOCAL_MODULE := android.hardware.gnss@1.0-service-qti
LOCAL_VENDOR_MODULE := true
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_INIT_RC := android.hardware.gnss@1.0-service-qti.rc
LOCAL_SRC_FILES := service.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/location_api

LOCAL_HEADER_LIBRARIES := \
	libgps.utils_headers \
	liblocation_api_headers \
	libloc_core_headers \
	libloc_pla_headers

LOCAL_SHARED_LIBRARIES := \
	libbase \
	libcutils \
	libdl \
	liblog \
	libutils

LOCAL_SHARED_LIBRARIES += \
	android.hardware.gnss@1.0 \
	libhidlbase \
	libhidltransport \
	libhwbinder

LOCAL_CFLAGS += $(GNSS_CFLAGS)
include $(BUILD_EXECUTABLE)
endif
