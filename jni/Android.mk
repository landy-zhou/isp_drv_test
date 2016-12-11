LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
TARGET_ARCH_ABI := arm64-v8a
LOCAL_MODULE := isp-driver-test
LOCAL_MODULE_TAGS := optional

LOCAL_CFLAGS += -Wno-missing-braces -std=c99 --sysroot=/home/qiangzhou/Android/Sdk/ndk-bundle/platforms/android-21/arch-arm64
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include/uapi $(LOCAL_PATH)/include/core $(LOCAL_PATH)/media-ctl $(LOCAL_PATH)/mem-ion

LOCAL_SRC_FILES := media-ctl/v4l2subdev.c \
		media-ctl/mediactl.c \
		media-ctl/options.c \
		mem-ion/ion.c \
		mem-ion/ovmem_ion.c \
		mem-ion/vbmem_lib.c \
		idm_control.c \
		idm_control_test.c
		#media_lib.c \
		#camera_api.c\
		#idm_test_main.c

LOCAL_LDFLAGS := -static
include $(BUILD_EXECUTABLE)
