/* 
 * 由光头强内核（原XF内核）开源
 因频道账号被盗用后决定开源，正版为光头强内核，
开源文件仅供学习交流，任何个人或组织可在遵循协议条款的前提下，自由下载、使用、修改、分发本项目代码及衍生作品，无需额外授权。使用过程中请保留原项目版权声明及开源协议文本，不得用于违法违规场景。本项目以“现状”提供，开发者不承担任何直接或间接的使用风险及法律责任，亦不提供商业技术支持。
删除此内容死全家，没几把
* 注入时间: 2025-11-16 20:14:49
 * 联系方式: TG:@GTQNHNB666
QQ群：784316342
 */
//#include "network_program.h"
#include "../include/proxy/proxy.h"  // 添加代理功能头文件
//#include "weiyan/Util.h"  // 添加微验功能头文件 - 文件不存在，已注释
//#include "weiyan/json.hpp"  // 添加JSON解析头文件 - 文件不存在，已注释
using namespace std;
//using json = nlohmann::json;  // weiyan/json.hpp 不存在，已注释
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <fstream>
#include <string.h>
#include <signal.h>  // 添加信号处理头文件
#include <time.h>
#include <malloc.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <random>
#include <ctime>
#include "draw.h"
#include "Android_draw/频道验证.h"
#include "Android_draw/obfuscate.h"
#include "Android_draw/timer.h"  // FPS限制功能
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// 微验所需的函数声明已在Util.h中定义
using namespace std;
static bool 触摸驱动;

// FPS限制已在draw.h/draw.cpp中定义为全局变量
// 移除局部声明，使用全局版本

// 重新初始化触摸系统函数实现
void 重新初始化触摸系统() {
    // 先关闭当前的触摸系统
    Touch::Close();
    
    // 根据当前单透模式状态决定是否使用只读模式
    bool readOnlyMode = 单透开启; // 单透开启时使用只读模式，不创建触摸文件
    
    // 重新初始化触摸系统
    Touch::Init({(float)::abs_ScreenX, (float)::abs_ScreenY}, readOnlyMode);
    Touch::setOrientation(displayInfo.orientation);
}  
static bool isDirectoryExist(const std::string& path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        return false;
    } else if (info.st_mode & S_IFDIR) {
        return true;
    } else {
        return false;
    }
}







// 微验库已在上方包含

// 信号处理函数
void signal_handler(int sig) {
    printf("接收到信号 %d，正在清理资源...\n", sig);
    
    // 设置退出标志而不是立即退出
    static bool cleanup_done = false;
    if (cleanup_done) return;
    cleanup_done = true;
    
    // stop_proxy_thread();  // 函数未定义，已注释
    
    // 强制清理所有资源
    system("killall -9 v2fly 2>/dev/null");
    system("killall -9 v2run.sh 2>/dev/null");
    
    printf("防掉框已关闭，程序退出\n");
    
    // 恢复默认信号处理器并重新发送信号
    signal(sig, SIG_DFL);
    raise(sig);
}


int main(){
    // 注册信号处理器
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGKILL, signal_handler);

    // 清理旧的网络配置
    // stop_iptables();     // 函数未定义，已注释 - 停止现有的iptables规则
    // stop_box();          // 函数未定义，已注释 - 停止现有的代理服务



//获取远程变量();
printf("\n----------------------------------\n");
system("mkdir -p  /data/光头强配置");
system("mkdir -p  /data/光头强解析/解析配置");
system("rm -rf  /data/data/artjx");

// // 执行频道验证
// printf("\033[35;1m正在进行频道验证...\n");
// 频道验证();

// 优化居中与层次，提升视觉协调度


// 重定向 stderr 屏蔽无关错误输出（保留原有功能）
std::freopen("/dev/null", "w", stderr);


    screen_config();
    // 设置游戏绘制分辨率（abs_ScreenX=长边，abs_ScreenY=短边）
    ::abs_ScreenX = (::displayInfo.height > ::displayInfo.width ? ::displayInfo.height : ::displayInfo.width);
    ::abs_ScreenY = (::displayInfo.height < ::displayInfo.width ? ::displayInfo.height : ::displayInfo.width);

    // 设置渲染窗口分辨率（创建方形窗口，两边都取长边）
    ::native_window_screen_x = (displayInfo.height > displayInfo.width ? displayInfo.height : displayInfo.width);
    ::native_window_screen_y = (displayInfo.height > displayInfo.width ? displayInfo.height : displayInfo.width);
    

// 渲染模式选择模块
char renderMode[32] = {0};
// 延迟2秒后显示选择界面
std::this_thread::sleep_for(std::chrono::seconds(2));
// 美化选择界面，颜色区分选项
std::cout << "\n========================================" << std::endl;
std::cout << "🎮 渲染模式选择" << std::endl;
std::cout << "----------------------------------------" << std::endl;
std::cout << "\033[1;33mℹ️  共享网址：光头强\033[0m" << std::endl; // 黄色提示网址
std::cout << "----------------------------------------" << std::endl;
std::cout << "请选择渲染核心（输入对应数字）：" << std::endl;
std::cout << "  [\033[1;34m1\033[0m] OpenGL（传统稳定模式）" << std::endl;
std::cout << "  [\033[1;35m2\033[0m] Vulkan推荐（高性能模式）" << std::endl;
std::cout << "----------------------------------------" << std::endl;
std::cout << "你的选择：";

scanf("%s", renderMode);
std::cout << "========================================\n" << std::endl;

// 防录屏功能选择（仅OpenGL模式支持）
bool enableAntiRecord = false;
if (strcmp(renderMode, "1") == 0) {
    char antiRecordChoice[32] = {0};
    // 延迟1秒显示防录屏选择
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "========================================" << std::endl;
    std::cout << "🛡️ OpenGL 防录屏功能" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "是否开启防录屏（可能影响部分设备兼容性）：" << std::endl;
    std::cout << "  [\033[1;32m1\033[0m] 开启（保护画面不被录制）" << std::endl;
    std::cout << "  [\033[1;31m2\033[0m] 关闭（默认，兼容性更佳）" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "你的选择：";
    // 输入合法性校验
    while (true) {
        scanf("%s", antiRecordChoice);
        if (strcmp(antiRecordChoice, "1") == 0 || strcmp(antiRecordChoice, "2") == 0) {
            break;
        }
        std::cout << "\033[1;31m❌ 输入无效！请输入1或2：\033[0m";
        while (getchar() != '\n'); // 清空错误输入
    }
    enableAntiRecord = (strcmp(antiRecordChoice, "1") == 0);
    std::cout << "========================================\n" << std::endl;
}


// 模式初始化逻辑
if (strcmp(renderMode, "2") == 0) {
    // 延迟2秒后显示初始化提示
    std::this_thread::sleep_for(std::chrono::seconds(2));
    // 重要操作提示
    std::cout << "\033[1;31m⚠️  警告：请先开启辅助，再启动王者荣耀！\033[0m" << std::endl;
    std::cout << "\033[1;31m⚠️  不听者后果自负，将导致解析无效！\033[0m" << std::endl;
    // 正版提示（文明表述）
    std::cout << "\033[1;33m📢 提示：破解版软件可能导致解析失效，且存在账号安全风险，建议使用正版软件保护自身权益！\033[0m\n" << std::endl;
    // Vulkan 渲染初始化
    std::cout << "\033[1;35m🔧 正在初始化 Vulkan 渲染核心...\033[0m" << std::endl;
    ::graphics = GraphicsManager::getGraphicsInterface(GraphicsManager::VULKAN);
    ::window = android::ANativeWindowCreator::Create("test", native_window_screen_x, native_window_screen_y, permeate_record);
    graphics->Init_Render(::window, native_window_screen_x, native_window_screen_y);
    
    ::init_VK_drawdata();  // 初始化绘制资源（字体、音量键监听等）
    
    // 延迟2秒后显示初始化结果
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "\033[1;32m✅ Vulkan 渲染初始化成功！\033[0m\n" << std::endl;
} else {
    // 延迟2秒后显示初始化提示
    std::this_thread::sleep_for(std::chrono::seconds(2));
    // 重要操作提示
    std::cout << "\033[1;31m⚠️  警告：请先开启辅助，再启动王者荣耀！\033[0m" << std::endl;
    std::cout << "\033[1;31m⚠️  不听者后果自负，将导致解析无效！\033[0m" << std::endl;
    // 正版提示（文明表述）
    std::cout << "\033[1;33m📢 提示：破解版软件可能导致解析失效，且存在账号安全风险，建议使用正版软件保护自身权益！\033[0m\n" << std::endl;
    // OpenGL 渲染初始化（默认选择）
    std::cout << "\033[1;34m🔧 正在初始化 OpenGL 渲染核心...\033[0m" << std::endl;
    if (init_egl(::native_window_screen_x, ::native_window_screen_y)) {
        ImGui_init();
        // 延迟2秒后显示初始化结果
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "\033[1;32m✅ OpenGL 渲染初始化成功！\033[0m\n" << std::endl;
    } else {
        // 延迟2秒后显示错误信息
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cerr << "\033[1;31m❌ OpenGL 渲染初始化失败，程序退出！\033[0m" << std::endl;
        exit(1);
    }
}



	// 根据单透模式状态决定是否使用只读模式
	bool readOnlyMode = 单透开启; // 单透开启时使用只读模式，不创建触摸文件
	Touch::Init({(float)::abs_ScreenX, (float)::abs_ScreenY}, readOnlyMode);

	Touch::setOrientation(displayInfo.orientation);
    
    // 添加初始化延迟，确保系统稳定
    usleep(500);
    
    // FPS限制 - 使用全局变量
    // FPS = 120.0f 已在draw.cpp中定义为全局变量
    // 可以在UI中动态调整
    
    // 初始化FPS控制（只需执行一次）
    ::FPS限制.AotuFPS_init();
    ::FPS限制.setAffinity();
    
    // 屏幕方向监控变量
    static uint32_t orientation = -1;
    
    while (true) {
        // 每帧FPS控制
        ::FPS限制.SetFps((int)::FPS);
        ::FPF显示 = ::FPS限制.AotuFPS();
        
        // 每帧更新屏幕配置
        screen_config();
        if (orientation != displayInfo.orientation) {
            orientation = displayInfo.orientation;
            Touch::setOrientation(displayInfo.orientation);
        }
        
        // 渲染循环
        if (graphics != nullptr) {
            // Vulkan渲染
            graphics->NewFrame();
            tick();
            graphics->EndFrame();
        } else {
            // OpenGL渲染 - 传统路径
            drawBegin();
            tick();
            drawEnd();
        }
    }
   
    // 清理资源（模仿wz自瞄）
    shutdown();
    
    // VK渲染模式专用清理
    if (graphics != nullptr) {
        graphics->Shutdown();
        android::ANativeWindowCreator::Destroy(::window);
    }

    return 0;
    
}
