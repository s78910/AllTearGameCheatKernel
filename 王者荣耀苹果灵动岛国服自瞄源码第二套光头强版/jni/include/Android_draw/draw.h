/* 
 * 由光头强内核（原XF内核）开源
 因频道账号被盗用后决定开源，正版为光头强内核，
开源文件仅供学习交流，任何个人或组织可在遵循协议条款的前提下，自由下载、使用、修改、分发本项目代码及衍生作品，无需额外授权。使用过程中请保留原项目版权声明及开源协议文本，不得用于违法违规场景。本项目以“现状”提供，开发者不承担任何直接或间接的使用风险及法律责任，亦不提供商业技术支持。
删除此内容死全家，没几把
* 注入时间: 2025-11-16 20:14:49
 * 联系方式: TG:@GTQNHNB666
QQ群：784316342
 */
#ifndef NATIVESURFACE_DRAW_H
#define NATIVESURFACE_DRAW_H

#include <iostream>
#include <thread>
#include <chrono>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <android/native_window.h>
#include <EGL/eglext.h>
#include <GLES3/gl3platform.h>
#include <GLES3/gl3ext.h>
#include <GLES3/gl32.h>
#include "ANativeWindowCreator.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_android.h"
#include "AndroidImgui.h"
#include "GraphicsManager.h"
#include "TouchHelperA.h"//触摸
#include "timer.h"

// namespace
using namespace std;
using namespace std::chrono_literals;

extern bool g_Initialized;
extern EGLDisplay display;
extern EGLConfig eglConfig;  // 重命名避免与 Confing.h 中的 config 冲突
extern EGLSurface surface;
extern EGLContext context;
extern ANativeWindow *native_window;

// VK渲染支持
extern std::unique_ptr<AndroidImgui> graphics;
extern ANativeWindow *window;


// 视图XY
extern int native_window_screen_x, native_window_screen_y;
extern int screen_x, screen_y;
// 屏幕信息
extern android::ANativeWindowCreator::DisplayInfo displayInfo;
// 绝对屏幕X _ Y
extern int abs_ScreenX, abs_ScreenY;

extern int GetEventCount();
extern int 音量();

extern int 版本;

extern char Mes9[256];
extern char Mes10[256];
extern char Mes11[256];
extern char Mes12[256];
extern char Mes13[256];
extern char Mes14[256];
extern char Mes15[256];
extern void screen_config();
extern int init_egl(int _screen_x,int _screen_y, bool log = false);
extern void ImGui_init();
extern void init_VK_drawdata();  // VK渲染字体初始化（模仿wz自瞄的init_My_drawdata）

// FPS控制全局变量
extern float FPS;
extern timer FPS限制;
extern float FPF显示;

extern void drawBegin();
extern void tick();
extern void drawEnd();
extern void SaveAllConfig();
extern void LoadAllConfig();
extern void shutdown();
extern int 读取();


void DrawPlayer();
extern void 获取远程变量();
extern long 人物偏移,矩阵偏移;
extern void 获取坐标();

// 单透模式全局变量声明
extern bool 单透开启;

// 防录屏模式全局变量声明
extern bool permeate_record;
extern bool permeate_record_ini;

// 重新初始化触摸系统函数声明
extern void 重新初始化触摸系统();

void DrawInit();
int DrawInt();
#endif

