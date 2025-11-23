LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := aaa.sh
LOCAL_CPPFLAGS := -w -std=c++20 -O2 -ffast-math







# VK渲染编译选项
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR
LOCAL_CPPFLAGS += -DIMGUI_IMPL_VULKAN_NO_PROTOTYPES

# 禁用ImGui调试和错误检查
#LOCAL_CPPFLAGS += -DIMGUI_DISABLE_DEBUG_TOOLS

# 添加LLVM混淆选项在这里（内存不足时请关闭）
#LOCAL_CPPFLAGS += -mllvm -sub -mllvm -sobf
#LOCAL_CPPFLAGS += -mllvm -sub -mllvm -sobf -mllvm -split -mllvm -bcf  -mllvm -fla
LOCAL_C_INCLUDES +=$(LOCAL_PATH)/include
LOCAL_C_INCLUDES +=$(LOCAL_PATH)/include/Android_draw
LOCAL_C_INCLUDES +=$(LOCAL_PATH)/include/Android_Graphics
LOCAL_C_INCLUDES +=$(LOCAL_PATH)/include/Android_my_imgui
LOCAL_C_INCLUDES +=$(LOCAL_PATH)/include/Android_touch
LOCAL_C_INCLUDES +=$(LOCAL_PATH)/include/ImGui
LOCAL_C_INCLUDES +=$(LOCAL_PATH)/include/ImGui/backends
LOCAL_C_INCLUDES +=$(LOCAL_PATH)/include/native_surface
LOCAL_C_INCLUDES +=$(LOCAL_PATH)/include/native_surface/aosp
LOCAL_C_INCLUDES +=$(LOCAL_PATH)/include/proxy
LOCAL_C_INCLUDES +=$(LOCAL_PATH)/src/kma
LOCAL_C_INCLUDES +=$(LOCAL_PATH)/src/proxy
LOCAL_C_INCLUDES +=$(LOCAL_PATH)/src/proxyres
LOCAL_C_INCLUDES +=$(LOCAL_PATH)/src/utils
LOCAL_LDFLAGS += $(LOCAL_PATH)/lib/*.a

# 收集源文件，排除备份文件和临时文件
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/*.c)
FILE_LIST += $(filter-out %copy.cpp %最新 %.bak %.cpp6 %.cpp无 %有问题%, $(wildcard $(LOCAL_PATH)/src/Android_draw/*.c*))
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/Android_Graphics/*.c*)
# 排除 my_img1ui.cpp，它是 my_imgui.cpp 的重复文件
FILE_LIST += $(filter-out $(LOCAL_PATH)/src/Android_my_imgui/my_img1ui.cpp, $(wildcard $(LOCAL_PATH)/src/Android_my_imgui/*.c*))
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/Android_touch/*.c*)
# 排除 src/ImGui/ 中的重复后端文件，使用 backends/ 中的新版本
FILE_LIST += $(filter-out $(LOCAL_PATH)/src/ImGui/stb_image.cpp $(LOCAL_PATH)/src/ImGui/imgui_impl_android.cpp $(LOCAL_PATH)/src/ImGui/imgui_impl_opengl3.cpp, $(wildcard $(LOCAL_PATH)/src/ImGui/*.c*))
# 添加 ImGui backends 目录，包含完整的后端实现（Android, OpenGL3, Vulkan）
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/ImGui/backends/*.c*)
FILE_LIST += $(filter-out %66, $(wildcard $(LOCAL_PATH)/src/proxy/*.c*))
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/proxyres/*.c*)
# 排除utils目录中的cJSON.c，使用proxyres中的版本
FILE_LIST += $(filter-out $(LOCAL_PATH)/src/utils/cJSON.c, $(wildcard $(LOCAL_PATH)/src/utils/*.c*))
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_LDFLAGS += -lEGL -lGLESv2 -lGLESv3 -landroid -llog
LOCAL_LDFLAGS += $(LOCAL_PATH)/include/lib/driver.a
LOCAL_LDLIBS += -lz #freetype需要
include $(BUILD_EXECUTABLE)

# 引入freetype静态库 #
include $(CLEAR_VARS)
LOCAL_MODULE := lib_git_freetype
LOCAL_SRC_FILES := src/ImGui/misc/git_freetype/$(TARGET_ARCH_ABI)/libfreetype.a
include $(PREBUILT_STATIC_LIBRARY)

# 控制流扁平化
# -mllvm -fla：激活控制流扁平化
# -mllvm -split：激活基本块分割。在一起使用时改善展平。
# -mllvm -split_num=3：如果激活了传递，则在每个基本块上应用3次。默认值：1

# 指令替换
# -mllvm -sub：激活指令替换
# -mllvm -sub_loop=3：如果激活了传递，则在函数上应用3次。默认值：1

# 虚假控制流程
# -mllvm -bcf：激活虚假控制流程
# -mllvm -bcf_loop=3：如果激活了传递，则在函数上应用3次。默认值：1
# -mllvm -bcf_prob=40：如果激活了传递，基本块将以40％的概率进行模糊处理。默认值：30

# 字符串加密
# -mllvm -sobf：编译时候添加选项开启字符串加密
# -mllvm -seed=0xabcdefg：指定随机数生成器种子流程

# 如果你对强度没有要求，只是单纯的简单混淆，可以使用下面这个.
# #FAST PROTECTION + SMALL SIZE:
# LOCAL_CPPFLAGS += -mllvm -sub -mllvm -sobf -mllvm -split -mllvm -bcf  -mllvm -fla 

# 如果你想高强度且不嫌打包时间慢可以使用下面.
# #SLOW PROTECTION + BIG SIZE :
# LOCAL_CPPFLAGS += -mllvm -sub -mllvm -sub_loop=3 -mllvm -sobf -mllvm -split -mllvm -split_num=3 -mllvm -bcf -mllvm -bcf_loop=1 -mllvm -bcf_prob=10 -mllvm -fla
