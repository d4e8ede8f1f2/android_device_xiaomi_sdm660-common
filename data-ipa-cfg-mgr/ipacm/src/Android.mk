
BOARD_PLATFORM_LIST := \
	msm8909 \
	msm8916 \
	msm8917

BOARD_IPAv3_LIST := \
	msm8998 \
	msmnile \
	sdm710 \
	sdm845 \
	$(MSMSTEPPE) \
	$(TRINKET)

ifneq ($(call is-board-platform-in-list, $(BOARD_PLATFORM_LIST)), true)
ifneq (, $(filter $(QCOM_BOARD_PLATFORMS), $(TARGET_BOARD_PLATFORM)))
ifneq (, $(filter aarch64 arm arm64, $(TARGET_ARCH)))

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../src \
	$(LOCAL_PATH)/../inc

LOCAL_HEADER_LIBRARIES := generated_kernel_headers

LOCAL_CFLAGS := \
	-DFEATURE_IPA_ANDROID \
	-DFEATURE_IPACM_RESTART \
	-DFEATURE_ETH_BRIDGE_LE

LOCAL_CFLAGS += -DFEATURE_IPACM_HAL -Wall -Werror -Wno-error=macro-redefined
ifneq (,$(filter eng, $(TARGET_BUILD_VARIANT)))
LOCAL_CFLAGS += -DDEBUG
endif

ifeq ($(call is-board-platform-in-list, $(BOARD_IPAv3_LIST)), true)
LOCAL_CFLAGS += -DFEATURE_IPA_V3
endif

filetoadd = bionic/libc/kernel/arch-arm/asm/posix_types.h
LOCAL_CFLAGS += $(shell if [ -a $(filetoadd) ] ; then echo -include $(filetoadd) ; fi ;)
filetoadd = bionic/libc/kernel/arch-arm/asm/byteorder.h
LOCAL_CFLAGS += $(shell if [ -a $(filetoadd) ] ; then echo -include $(filetoadd) ; fi ;)

LOCAL_SRC_FILES := \
	IPACM_CmdQueue.cpp \
	IPACM_Config.cpp \
	IPACM_Conntrack_NATApp.cpp\
	IPACM_ConntrackClient.cpp \
	IPACM_ConntrackListener.cpp \
	IPACM_EvtDispatcher.cpp \
	IPACM_Filtering.cpp \
	IPACM_Header.cpp \
	IPACM_Iface.cpp \
	IPACM_IfaceManager.cpp \
	IPACM_Lan.cpp \
	IPACM_LanToLan.cpp \
	IPACM_Log.cpp \
	IPACM_Main.cpp \
	IPACM_Neighbor.cpp \
	IPACM_Netlink.cpp \
	IPACM_OffloadManager.cpp \
	IPACM_Routing.cpp \
	IPACM_Wan.cpp \
	IPACM_Wlan.cpp \
	IPACM_Xml.cpp

LOCAL_MODULE := ipacm
LOCAL_CLANG := false
LOCAL_MODULE_TAGS := optional

LOCAL_SHARED_LIBRARIES := \
	android.hardware.tetheroffload.config@1.0 \
	android.hardware.tetheroffload.control@1.0 \
	libbase \
	libcutils \
	libdl \
	libhardware \
	libhardware_legacy \
	libhidlbase \
	libhidltransport \
	libhwbinder \
	libipanat \
	liblog \
	libnetfilter_conntrack \
	libnfnetlink \
	liboffloadhal \
	libutils \
	libxml2

LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR_EXECUTABLES)

LOCAL_CLANG := true
include $(BUILD_EXECUTABLE)

define ADD_TEST

include $(CLEAR_VARS)
LOCAL_MODULE := $1
LOCAL_SRC_FILES := $1
LOCAL_MODULE_CLASS := ipacm
LOCAL_MODULE_TAGS := debug
LOCAL_MODULE_PATH := $(TARGET_OUT_ETC)
include $(BUILD_PREBUILT)

endef

include $(CLEAR_VARS)
LOCAL_MODULE := IPACM_cfg.xml
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR_ETC)
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := $(LOCAL_MODULE)
LOCAL_MODULE_OWNER := ipacm
include $(BUILD_PREBUILT)

endif
endif
endif
