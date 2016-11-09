LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
TARGET_ARCH_ABI := arm64-v8a
LOCAL_MODULE := isp-driver-test
LOCAL_MODULE_TAGS := optional

LOCAL_CFLAGS += -Wno-missing-braces
LOCAL_C_INCLUDES := $(LOCAL_PATH)//include/uapi

LOCAL_SRC_FILES := MediaLib.c \
		v4l2subdev.c \
		mediactl.c \
		ion.c \
		ovmem-ion.c \
		vbmem_lib.c \
		CameraAPI.c\
		drv_test.c

LOCAL_LDFLAGS := -static

include $(BUILD_EXECUTABLE)
