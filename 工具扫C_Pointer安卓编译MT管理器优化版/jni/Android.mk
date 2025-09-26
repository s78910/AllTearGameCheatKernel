LOCAL_PATH := $(call my-dir)
#泪心修复 @TearGame
include $(CLEAR_VARS)
LOCAL_MODULE := cpointer_baiyi

LOCAL_CFLAGS := -std=c17 -w
LOCAL_CFLAGS += -fvisibility=hidden

LOCAL_CPPFLAGS := -std=c++17 -w
LOCAL_CPPFLAGS += -fvisibility=hidden
LOCAL_CPPFLAGS += -fexceptions
LOCAL_CPPFLAGS += -frtti
LOCAL_CPPFLAGS += -fno-strict-aliasing

#引入头文件到全局#   
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/utils
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/utils/threadtool

FILE_LIST += $(wildcard $(LOCAL_PATH)/src/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/chainer/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/memtool/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/utils/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/utils/threadtool/*.c*)

LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_LDLIBS := -llog -landroid -lm -lc -ldl

include $(BUILD_EXECUTABLE) #可执行文件
