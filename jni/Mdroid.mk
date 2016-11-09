#
#DRV_TEST
#

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
include $(LOCAL_PATH)/../CameraConfig.mk

LOCAL_SRC_FILES := \
		MediaLib.c \
		v4l2subdev.c \
		mediactl.c \
		ion.c \
		ovmem-ion.c \
		vbmem_lib.c \
		CameraAPI.c\
		drv_test.c
# put the MACROS you depend on here
LOCAL_CFLAGS += -D ANDROID \
		-I $(LOCAL_PATH)/include/uapi \
		-D LINUX 
 #               -Werror
LOCAL_SHARED_LIBRARIES += \
		libdl \
		libcutils 
#		libMalloc
#		libcameraengine

LOCAL_LDFLIBS += -ldl
#    -mabi=aapcs-linux


# put your module name here
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := Drv_test

include $(BUILD_EXECUTABLE)
