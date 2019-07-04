
ifneq ($(BOARD_VENDOR_QCOM_GPS_LOC_API_HARDWARE),)
ifneq ($(BUILD_TINY_ANDROID), true)

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SHARED_LIBRARIES := \
	libdl \
	libcutils \
	liblog \
	libutils

LOCAL_SRC_FILES += \
	linked_list.c \
	LocHeap.cpp \
	LocIpc.cpp \
	LocThread.cpp \
	LocTimer.cpp \
	loc_cfg.cpp \
	loc_log.cpp \
	loc_misc_utils.cpp \
	loc_nmea.cpp \
	loc_target.cpp \
	MsgTask.cpp \
	msg_q.c

LOCAL_CFLAGS += \
	-D_ANDROID_ \
	-fno-short-enums

ifeq ($(TARGET_BUILD_VARIANT),user)
 LOCAL_CFLAGS += -DTARGET_BUILD_VARIANT_USER
endif

LOCAL_LDFLAGS += -Wl,--export-dynamic

LOCAL_HEADER_LIBRARIES := \
	liblocation_api_headers \
	libloc_pla_headers \
	libutils_headers

LOCAL_MODULE := libgps.utils
LOCAL_VENDOR_MODULE := true
LOCAL_MODULE_TAGS := optional

LOCAL_PRELINK_MODULE := false

LOCAL_CFLAGS += $(GNSS_CFLAGS)

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libgps.utils_headers
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)
include $(BUILD_HEADER_LIBRARY)

endif
endif
