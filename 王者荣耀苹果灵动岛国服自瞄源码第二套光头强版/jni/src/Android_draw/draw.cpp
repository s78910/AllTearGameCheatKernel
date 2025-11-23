#include "draw.h"
#include "DrawPlayer.hpp"
#include "Font.h"
#include "../../include/proxy/proxy.h"  // 添加代理功能头文件
#include "Layout.hpp"
#include "PTlogo.h"
#include "SXlogo.h"
#include "ZYlogo.h"
#include "imgui_whitecloud.h"
#include "阿念の函数集成.h"
#include "设置图标.h"
#include "xishi.h"
#include "Confing.h"
#include "Aim.h"
#include <dirent.h>
#include <map>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <fstream>
#include <fcntl.h>
#include <linux/input.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_LINE_LENGTH 1024 // 定义MAX_LINE_LENGTH宏
/* 
 * 由光头强内核（原XF内核）开源
 因频道账号被盗用后决定开源，正版为光头强内核，
开源文件仅供学习交流，任何个人或组织可在遵循协议条款的前提下，自由下载、使用、修改、分发本项目代码及衍生作品，无需额外授权。使用过程中请保留原项目版权声明及开源协议文本，不得用于违法违规场景。本项目以“现状”提供，开发者不承担任何直接或间接的使用风险及法律责任，亦不提供商业技术支持。
删除此内容死全家，没几把
* 注入时间: 2025-11-16 20:14:49
 * 联系方式: TG:@GTQNHNB666
QQ群：784316342
 */
// 悬浮窗动画相关变量
static float windowScale = 1.0f;        // 窗口缩放比例 (0.0 - 1.0) - VK渲染修复：直接设为1.0避免首次打开尺寸过小
static float windowAlpha = 1.0f;        // 窗口透明度 (0.0 - 1.0) - VK渲染修复：直接设为1.0
static float animationTime = 0.0f;      // 动画时间
static bool isWindowOpening = false;    // 窗口是否正在打开
static bool isWindowClosing = false;    // 窗口是否正在关闭
static bool windowVisible = true;       // 窗口是否可见 - VK渲染修复：默认可见
static float animationDuration = 0.5f;  // 动画持续时间（秒）

// 设置图标纹理
static TextureInfoa 设置图标纹理;

// 悬浮窗背景纹理
static TextureInfoa xishi背景纹理;

// 自动换装配置结构体
struct EquipmentConfig {
    bool isRecording = false;
    std::string currentRecording;
    int recordingStep = 0;
    std::map<std::string, std::vector<ImVec2>> actions;
    std::map<std::string, ImVec2> iconPositions;
    std::map<std::string, bool> iconVisible;
    float delay = 0.04f;
    bool isDragging = false;
    std::string draggedIcon;
    ImVec2 dragStartPos;
    bool showHint = false;
    std::chrono::time_point<std::chrono::steady_clock> hintStartTime;
    float dragThreshold = 5.0f;

    void SaveConfig();
    void LoadConfig();
    bool HasRecorded(const std::string& equipName) const;
};

// 换装功能全局变量
EquipmentConfig equipmentConfig;
bool 配置提示显示 = false;
std::chrono::time_point<std::chrono::steady_clock> 配置提示开始时间;
int 配置提示类型 = 0;
bool MenuSic = true;  // 菜单显示控制变量

// EquipmentConfig 方法实现
void EquipmentConfig::SaveConfig() {
    system("mkdir -p /data/光头强配置/配置/");

    // 保存每个装备的坐标点
    for (const auto& [equipName, points] : actions) {
        std::string filePath = "/data/光头强配置/配置/" + equipName + "_points";
        std::ofstream outFile(filePath);
        if (outFile) {
            for (const auto& point : points) {
                outFile << point.x << " " << point.y << "\n";
            }
        }
    }

    // 保存图标位置
    std::ofstream posFile("/data/光头强配置/配置/icon_positions");
    if (posFile) {
        for (const auto& [equipName, pos] : iconPositions) {
            posFile << equipName << " " << pos.x << " " << pos.y << "\n";
        }
    }

    // 保存图标可见状态
    std::ofstream visFile("/data/光头强配置/配置/icon_visible");
    if (visFile) {
        for (const auto& [equipName, visible] : iconVisible) {
            visFile << equipName << " " << visible << "\n";
        }
    }

    // 保存延迟设置
    std::ofstream delayFile("/data/光头强配置/配置/delay");
    if (delayFile) {
        delayFile << delay;
    }
}

void EquipmentConfig::LoadConfig() {
    // 加载装备动作坐标
    for (const auto& equip : {"复活", "名刀", "金身", "炽热"}) {
        std::string filePath = "/data/光头强配置/配置/" + std::string(equip) + "_points";
        std::ifstream inFile(filePath);
        if (inFile) {
            std::vector<ImVec2> points;
            float x, y;
            while (inFile >> x >> y) {
                points.push_back(ImVec2(x, y));
            }
            actions[equip] = points;
        }
    }
/* 
 * 由光头强内核（原XF内核）开源
 因频道账号被盗用后决定开源，正版为光头强内核，
开源文件仅供学习交流，任何个人或组织可在遵循协议条款的前提下，自由下载、使用、修改、分发本项目代码及衍生作品，无需额外授权。使用过程中请保留原项目版权声明及开源协议文本，不得用于违法违规场景。本项目以“现状”提供，开发者不承担任何直接或间接的使用风险及法律责任，亦不提供商业技术支持。
删除此内容死全家，没几把
* 注入时间: 2025-11-16 20:14:49
 * 联系方式: TG:@GTQNHNB666
QQ群：784316342
 */
    // 加载图标位置（带默认值）
    std::ifstream posFile("/data/光头强配置/配置/icon_positions");
    if (posFile) {
        std::string equipName;
        float x, y;
        while (posFile >> equipName >> x >> y) {
            iconPositions[equipName] = ImVec2(x, y);
        }
    }
    else {
        iconPositions["复活"] = ImVec2(100, 100);
        iconPositions["名刀"] = ImVec2(100, 220);
        iconPositions["金身"] = ImVec2(100, 340);
        iconPositions["炽热"] = ImVec2(100, 460);
    }

    // 加载图标可见状态
    std::ifstream visFile("/data/光头强配置/配置/icon_visible");
    if (visFile) {
        std::string equipName;
        bool visible;
        while (visFile >> equipName >> visible) {
            iconVisible[equipName] = visible;
        }
    }

    // 加载延迟设置
    std::ifstream delayFile("/data/光头强配置/配置/delay");
    if (delayFile) {
        delayFile >> delay;
    }
}

bool EquipmentConfig::HasRecorded(const std::string& equipName) const {
    auto it = actions.find(equipName);
    return it != actions.end() && !it->second.empty();
}

// 自动换装功能实现
void 自动换装() 
{
    // 装备录制逻辑
    if (equipmentConfig.isRecording && ImGui::IsMouseClicked(0)) 
    {
        ImVec2 mousePos = ImGui::GetMousePos();
        equipmentConfig.actions[equipmentConfig.currentRecording].push_back(mousePos);
        equipmentConfig.recordingStep++;
    }

    // 渲染悬浮图标和交互处理
    for (auto& pair : equipmentConfig.iconVisible) 
    {
        if (pair.second && !equipmentConfig.actions[pair.first].empty()) 
        {
            const std::string& equipmentName = pair.first;
            ImVec2& iconPos = equipmentConfig.iconPositions[equipmentName];
            const float iconSize = 990.0f;
            const float cornerRadius = 6495.0f;

            // 绘制图标
            ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
            draw_list->AddRectFilled(iconPos, ImVec2(iconPos.x + iconSize, iconPos.y + iconSize), ImColor(30, 30, 45, 220), cornerRadius);
            draw_list->AddRect(iconPos, ImVec2(iconPos.x + iconSize, iconPos.y + iconSize), ImColor(100, 150, 255, 200), cornerRadius, 0, 3.0f);

            // 绘制文字（优化：使用静态缓冲区避免每帧创建 std::string）
            static char displayName[32];
            if (equipmentName.length() > 6) {
                snprintf(displayName, sizeof(displayName), "%.6s..", equipmentName.c_str());
            } else {
                snprintf(displayName, sizeof(displayName), "%s", equipmentName.c_str());
            }
            ImVec2 textSize = ImGui::CalcTextSize(displayName);
            ImVec2 textPos = ImVec2(iconPos.x + (iconSize - textSize.x) * 0.5f, iconPos.y + (iconSize - textSize.y) * 0.5f);
            draw_list->AddText(textPos, ImColor(0, 0, 0, 950), displayName);
            draw_list->AddText(ImVec2(textPos.x - 1, textPos.y - 1), ImColor(255, 235, 655), displayName);

            // 交互检测
            ImVec2 mousePos = ImGui::GetMousePos();
            bool isHovered = mousePos.x >= iconPos.x && mousePos.x <= iconPos.x + iconSize && 
                            mousePos.y >= iconPos.y && mousePos.y <= iconPos.y + iconSize;

            if (isHovered) 
            {
                draw_list->AddRect(iconPos, ImVec2(iconPos.x + iconSize, iconPos.y + iconSize), 
                                 ImColor(255, 255, 0, 255), cornerRadius, 0, 3.0f);
            }

            // 记录按下位置
            if (isHovered && ImGui::IsMouseClicked(0)) 
            {
                equipmentConfig.draggedIcon = equipmentName;
                equipmentConfig.dragStartPos = mousePos;
                equipmentConfig.isDragging = false;  // 初始不标记为拖动
            }

            // 检测拖动距离，超过阈值才进入拖动模式
            if (equipmentConfig.draggedIcon == equipmentName && ImGui::IsMouseDown(0)) 
            {
                ImVec2 delta = ImGui::GetMouseDragDelta(0);
                float dragDistance = sqrtf(delta.x * delta.x + delta.y * delta.y);
                
                if (dragDistance > equipmentConfig.dragThreshold) 
                {
                    equipmentConfig.isDragging = true;  // 超过阈值才标记为拖动
                    iconPos.x = equipmentConfig.dragStartPos.x + delta.x - iconSize / 2;
                    iconPos.y = equipmentConfig.dragStartPos.y + delta.y - iconSize / 2;
                }
            }

            // 点击执行换装（释放时检测是否在图标内且未拖动）
            if (equipmentConfig.draggedIcon == equipmentName && ImGui::IsMouseReleased(0) && !equipmentConfig.isDragging) 
            {
                auto actionsCopy = equipmentConfig.actions[equipmentName];
                int screenDirection = 屏幕方向;
                int screenWidth = yxpx;
                int screenHeight = yxpy;
                float delay = equipmentConfig.delay;

                std::thread([actionsCopy, screenDirection, screenWidth, screenHeight, delay]() 
                {
                    for (size_t i = 0; i < actionsCopy.size(); ++i) 
                    {
                        const auto& pos = actionsCopy[i];
                        auto [x, y] = (screenDirection == 3) ? std::make_tuple(pos.y, screenWidth - pos.x)
                                                            : std::make_tuple(screenHeight - pos.y, pos.x);

                        Touch::Down(x, y);
                        std::this_thread::sleep_for(std::chrono::milliseconds(2));
                        Touch::Up();

                        if (i != actionsCopy.size() - 1) 
                        {
                            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(delay * 1000)));
                        }
                    }
                }).detach();
            }
        }
    }

    // 结束拖动，重置所有拖动相关状态
    if (ImGui::IsMouseReleased(0)) 
    {
        equipmentConfig.isDragging = false;
        equipmentConfig.draggedIcon = "";  // 清空拖动图标标记
    }

    // 录制提示窗口
    if (equipmentConfig.showHint) 
    {
        MenuSic = false;
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - equipmentConfig.hintStartTime).count();

        if (elapsed >= 10) 
        {
            equipmentConfig.showHint = false;
            equipmentConfig.isRecording = false;
            MenuSic = true;
        }
        else 
        {
            ImVec2 windowSize = ImVec2(550, 280);
            // 计算窗口位置 - 屏幕正上方中间
            ImVec2 windowPos = ImVec2((ImGui::GetIO().DisplaySize.x - windowSize.x) * 0.5f, 20);  // 20像素从顶部开始

            // 添加窗口动画效果
            float animProgress = std::min(1.0f, elapsed / 2.0f);
            float windowAlpha = 0.9f * animProgress;
            float scale = 0.95f + 0.05f * sinf(ImGui::GetTime() * 3.0f);

            ImGui::SetNextWindowPos(windowPos);
            ImGui::SetNextWindowSize(windowSize);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.08f, 0.09f, 0.12f, windowAlpha));
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.2f, 0.5f, 0.8f, 0.6f));
            // ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
            // ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);

            if (ImGui::Begin("##EquipmentRecording", nullptr, 
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar)) 
            {
                // 标题部分
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);
                ImGui::SetCursorPosX((windowSize.x - ImGui::CalcTextSize("装备录制中").x) * 0.5f);
                
                // 带阴影的文字效果
                ImVec2 textPos = ImGui::GetCursorScreenPos();
                ImDrawList* drawList = ImGui::GetWindowDrawList();
                drawList->AddText(ImVec2(textPos.x + 2, textPos.y + 2), IM_COL32(0, 0, 0, 150), "装备录制中");
                ImGui::TextColored(ImVec4(0.2f, 0.8f, 1.0f, 1.0f), "装备录制中");
                
                // 分隔线
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                ImGui::Separator();
                ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.3f, 0.6f, 0.9f, 0.6f));

                // 内容表格
                if (ImGui::BeginTable("##RecTable", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_SizingFixedFit)) 
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::SetCursorPosX(30);
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
                    ImGui::Text("当前装备: %s", equipmentConfig.currentRecording.c_str());
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                    ImGui::Text("录制进度: %d步", equipmentConfig.recordingStep);
                    ImGui::PopStyleColor();

                    ImGui::TableSetColumnIndex(1);
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
                    char timeText[32];
                    snprintf(timeText, sizeof(timeText), "剩余时间: %d秒", 10 - (int)elapsed);
                    
                    // 带阴影的时间文字
                    textPos = ImGui::GetCursorScreenPos();
                    drawList->AddText(ImVec2(textPos.x + 1, textPos.y + 1), IM_COL32(0, 0, 0, 150), timeText);
                    ImGui::TextColored(ImVec4(0.3f, 0.9f, 0.5f, 1.0f), "%s", timeText);

                    ImGui::EndTable();
                }
                ImGui::PopStyleColor(); // 分隔线颜色

                // 进度条
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
                ImGui::SetCursorPosX(30);
                float progress = (10.0f - elapsed) / 10.0f;
                
                // 自定义进度条样式
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.2f, 0.25f, 0.8f));
                ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.2f, 0.6f, 1.0f, 0.8f));
               // ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
                
                ImGui::ProgressBar(progress, ImVec2(windowSize.x - 60, 30), "");
                
                // 在进度条上添加文字
                ImVec2 barPos = ImGui::GetItemRectMin();
                ImVec2 barSize = ImGui::GetItemRectSize();
                char progressText[16];
                snprintf(progressText, sizeof(progressText), "%.0f%%", progress * 100);
                ImVec2 textSize = ImGui::CalcTextSize(progressText);
                drawList->AddText(ImVec2(barPos.x + (barSize.x - textSize.x) * 0.5f, 
                                        barPos.y + (barSize.y - textSize.y) * 0.5f), 
                                IM_COL32(255, 255, 255, 220), progressText);
                
                //ImGui::PopStyleVar();
                ImGui::PopStyleColor(2);

                // 底部提示文字
                ImGui::SetCursorPosY(windowSize.y - 50);
                ImGui::Separator();
                ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.3f, 0.6f, 0.9f, 0.6f));

                char bottomText[64];
                snprintf(bottomText, sizeof(bottomText), "系统将在 %d 秒后自动结束录制", 10 - (int)elapsed);
                ImGui::SetCursorPosX((windowSize.x - ImGui::CalcTextSize(bottomText).x) * 0.5f);
                
                // 带阴影的底部文字
                textPos = ImGui::GetCursorScreenPos();
                drawList->AddText(ImVec2(textPos.x + 1, textPos.y + 1), IM_COL32(0, 0, 0, 150), bottomText);
                ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.3f, 1.0f), "%s", bottomText);
                
                ImGui::PopStyleColor(); // 分隔线颜色
            }
            ImGui::End();
            //ImGui::PopStyleVar(2);
            ImGui::PopStyleColor(2);
        }
    }
    
    // 配置提示显示
    if (配置提示显示) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - 配置提示开始时间).count();
        
        if (elapsed >= 3) {  // 显示3秒后自动隐藏
            配置提示显示 = false;
        } else {
            // 根据提示类型显示不同消息
            const char* tipText;
            ImVec4 bgColor;
            
            if (配置提示类型 == 0) {
                tipText = "配置保存成功!";
                bgColor = ImVec4(0.1f, 0.7f, 0.1f, 0.9f); // 绿色
            } else if (配置提示类型 == 1) {
                tipText = "图标删除成功!";
                bgColor = ImVec4(0.7f, 0.3f, 0.1f, 0.9f); // 橙色
            } else if (配置提示类型 == 2) {
                tipText = "缺失图标已生成!";
                bgColor = ImVec4(0.1f, 0.5f, 0.7f, 0.9f); // 蓝色
            } else if (配置提示类型 == 3) {
                tipText = "所有图标已生成!";
                bgColor = ImVec4(0.5f, 0.1f, 0.7f, 0.9f); // 紫色
            } else {
                tipText = "操作完成!";
                bgColor = ImVec4(0.1f, 0.5f, 0.7f, 0.9f); // 蓝色
            }
            
            ImVec2 windowSize = ImVec2(300, 100);
            ImVec2 windowPos = ImVec2((ImGui::GetIO().DisplaySize.x - windowSize.x) * 0.5f, 
                                     (ImGui::GetIO().DisplaySize.y - windowSize.y) * 0.5f);
            
            ImGui::SetNextWindowPos(windowPos);
            ImGui::SetNextWindowSize(windowSize);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, bgColor);
           // ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
            
            if (ImGui::Begin("##ConfigTip", nullptr, 
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar)) {
                
                ImGui::SetCursorPos(ImVec2((windowSize.x - ImGui::CalcTextSize(tipText).x) * 0.5f, 
                                          (windowSize.y - ImGui::GetTextLineHeight()) * 0.5f));
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%s", tipText);
            }
            ImGui::End();
         //   ImGui::PopStyleVar();
            ImGui::PopStyleColor();
        }
    }
}
         





// 缓动函数 - 二次贝塞尔曲
float EaseInOutQuad(float t) {
    return t < 0.5f ? 2.0f * t * t : 1.0f - pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
}

// 更新窗口动画
void UpdateWindowAnimation() {
    float deltaTime = ImGui::GetIO().DeltaTime;
    
    if (isWindowOpening || isWindowClosing) {
        animationTime += deltaTime;
        float progress = animationTime / animationDuration;
        
        if (progress >= 1.0f) {
            progress = 1.0f;
            if (isWindowOpening) {
                isWindowOpening = false;
                windowVisible = true;
            } else if (isWindowClosing) {
                isWindowClosing = false;
                windowVisible = false;
            }
        }
        
        // 应用缓动函数
        float easedProgress = EaseInOutQuad(progress);
        
        if (isWindowOpening) {
            windowScale = easedProgress;
            windowAlpha = easedProgress;
        } else if (isWindowClosing) {
            windowScale = 1.0f - easedProgress;
            windowAlpha = 1.0f - easedProgress;
        }
    }
}

// 开始打开窗口动画
void StartOpenAnimation() {
    if (!isWindowOpening) {  // 只要不是正在打开就可以开始打开动画
        isWindowOpening = true;
        isWindowClosing = false;
        animationTime = 0.0f;
        windowScale = 0.0f;
        windowAlpha = 0.0f;
        windowVisible = false;  // 重置可见状态
    }
}

// 开始关闭窗口动画
void StartCloseAnimation() {
    if (!isWindowClosing) {  // 只要不是正在关闭就可以开始关闭动画
        isWindowClosing = true;
        isWindowOpening = false;
        animationTime = 0.0f;
    }
}

// 垂直切换动画变量（贝塞尔风格）
static float homepage = 0.0f;  // 当前动画位置
static float homepages = 0.0f; // 目标动画位置
static float tab0 = 0.0f;      // 第一个标签页位置（绘制区域）
static float tab1 = -400.0f;   // 第二个标签页位置（绘制设置）
static float tab2 = -800.0f;   // 第三个标签页位置（设置区域）
static float tab3 = -1200.0f;  // 第四个标签页位置（触摸区域）
static float tab4 = -1600.0f;  // 第五个标签页位置（内存区域）
static float tab5 = -2000.0f;  // 第六个标签页位置（换装区域）

// 横向切换动画变量（设置页面内部）
static float settingPage = 0.0f;   // 当前横向位置
static float settingPages = 0.0f;  // 目标横向位置
static float settingTab0 = 0.0f;   // 绘制调节
static float settingTab1 = -600.0f;   // 显示设置
static float settingTab2 = -1200.0f;  // 颜色设置
static float settingTab3 = -1800.0f;  // 共享设置

// 动态窗口大小变量（贝塞尔风格）
static float windowWidth = 1300.0f;     // 当前窗口宽度
static float windowHeight = 900.0f;     // 当前窗口高度
static float targetWidth = 1300.0f;     // 目标窗口宽度
static float targetHeight = 900.0f;     // 目标窗口高度

// 退出程序确认状态
static bool exitConfirmSelected = false;  // 退出按钮是否被选中确认状态

// 现代化方形圆角Checkbox函数 - 带渐变色彩、阴影效果和悬停状态
void CustomCheckbox(const char* label, bool* value, ImVec2 size = ImVec2(120, 40), TextureInfoa* icon = nullptr) {
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 text_size = ImGui::CalcTextSize(label);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    // 计算图标和方形位置
    float icon_size = 24.0f;  // 图标大小
    float icon_spacing = 8.0f;  // 图标与checkbox间距
    float box_size = 32.0f;  // 方形大小（从24增加到32）
    float corner_radius = 8.0f;  // 圆角半径（相应增加）
    
    float start_x = pos.x + 8;
    if (icon != nullptr) {
        start_x += icon_size + icon_spacing;  // 如果有图标，checkbox向右偏移
    }
    
    ImVec2 box_min = ImVec2(start_x, pos.y + (size.y - box_size) * 0.5f);
    ImVec2 box_max = ImVec2(box_min.x + box_size, box_min.y + box_size);
    
    // 检测悬停状态
    ImVec2 button_size = ImVec2(box_size + text_size.x + 20, size.y);
    bool hovered = ImGui::IsMouseHoveringRect(pos, ImVec2(pos.x + button_size.x, pos.y + button_size.y));
    
    // 绘制阴影效果
    ImVec2 shadow_min = ImVec2(box_min.x + 2, box_min.y + 2);
    ImVec2 shadow_max = ImVec2(box_max.x + 2, box_max.y + 2);
    draw_list->AddRectFilled(shadow_min, shadow_max, IM_COL32(0, 0, 0, 30), corner_radius);
    
    // 选择颜色（带悬停效果）
    ImU32 bg_color, border_color, text_color;
    if (*value) {
        // 选中状态：蓝色渐变
        bg_color = hovered ? IM_COL32(70, 130, 255, 255) : IM_COL32(50, 120, 255, 255);
        border_color = IM_COL32(30, 100, 255, 255);
        text_color = IM_COL32(50, 120, 255, 255);
    } else {
        // 未选中状态：白色/灰色
        bg_color = hovered ? IM_COL32(245, 245, 245, 255) : IM_COL32(255, 255, 255, 255);
        border_color = hovered ? IM_COL32(150, 150, 150, 255) : IM_COL32(200, 200, 200, 255);
        text_color = IM_COL32(80, 80, 80, 255);
    }
    
    // 绘制方形背景（渐变效果通过多层矩形实现）
    if (*value) {
        // 蓝色渐变效果
        draw_list->AddRectFilled(box_min, box_max, bg_color, corner_radius);
        ImVec2 gradient_min = ImVec2(box_min.x + 2, box_min.y + 2);
        ImVec2 gradient_max = ImVec2(box_max.x - 2, box_max.y - 8);
        draw_list->AddRectFilled(gradient_min, gradient_max, IM_COL32(80, 140, 255, 100), corner_radius * 0.8f);
    } else {
        draw_list->AddRectFilled(box_min, box_max, bg_color, corner_radius);
    }
    
    // 绘制方形边框
    draw_list->AddRect(box_min, box_max, border_color, corner_radius, 0, 2.0f);
    
    // 如果选中，绘制勾选标记
    if (*value) {
        // 绘制白色勾选标记（带平滑端点）- 适配更大尺寸
        ImVec2 box_center = ImVec2((box_min.x + box_max.x) * 0.5f, (box_min.y + box_max.y) * 0.5f);
        ImVec2 check_start = ImVec2(box_center.x - 10, box_center.y - 2);
        ImVec2 check_mid = ImVec2(box_center.x - 3, box_center.y + 6);
        ImVec2 check_end = ImVec2(box_center.x + 10, box_center.y - 8);
        
        // 使用AddPolyline绘制平滑的勾选标记（线条更粗）
        ImVec2 check_points[] = {check_start, check_mid, check_end};
        draw_list->AddPolyline(check_points, 3, IM_COL32(255, 255, 255, 255), ImDrawFlags_None, 4.0f);
    }
    
    // 绘制图标（如果提供）
    if (icon != nullptr && icon->textureId != 0) {
        ImVec2 icon_min = ImVec2(pos.x + 8, pos.y + (size.y - icon_size) * 0.5f);
        ImVec2 icon_max = ImVec2(icon_min.x + icon_size, icon_min.y + icon_size);
        
        // 绘制图标阴影
        ImVec2 icon_shadow_min = ImVec2(icon_min.x + 1, icon_min.y + 1);
        ImVec2 icon_shadow_max = ImVec2(icon_max.x + 1, icon_max.y + 1);
        draw_list->AddImage((ImTextureID)icon->textureId, icon_shadow_min, icon_shadow_max, 
                           ImVec2(0, 0), ImVec2(1, 1), IM_COL32(0, 0, 0, 30));
        
        // 绘制主图标
        ImU32 icon_color = hovered ? IM_COL32(255, 255, 255, 255) : IM_COL32(200, 200, 200, 255);
        draw_list->AddImage((ImTextureID)icon->textureId, icon_min, icon_max, 
                           ImVec2(0, 0), ImVec2(1, 1), icon_color);
    }
    
    // 绘制文字标签（带阴影效果）
    ImVec2 text_pos = ImVec2(box_max.x + 12, pos.y + (size.y - text_size.y) * 0.5f);
    
    // 文字阴影
    draw_list->AddText(ImVec2(text_pos.x + 1, text_pos.y + 1), IM_COL32(0, 0, 0, 50), label);
    // 主文字
    draw_list->AddText(text_pos, text_color, label);
    
    // 创建不可见按钮用于点击检测
    if (ImGui::InvisibleButton(label, button_size)) {
        *value = !(*value);
    }
}

// 贝塞尔风格的圆角边框按钮函数
bool CustomButton(const char* label, ImVec2 size = ImVec2(120, 40)) {
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 text_size = ImGui::CalcTextSize(label);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    // 按钮状态检测
    bool hovered = ImGui::IsMouseHoveringRect(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    bool clicked = false;
    
    // 绘制圆角矩形背景（透明或半透明）
    ImU32 bg_color = hovered ? IM_COL32(255,255,255,30) : IM_COL32(255,255,255,10);
    draw_list->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bg_color, 15.0f);
    


    
    // 绘制圆角边框
    ImU32 border_color = IM_COL32(0,0,0,255); // 黑色边框
    draw_list->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), border_color, 15.0f, ImDrawFlags_RoundCornersAll, 2.0f);
    
    // 绘制文字（居中）
    ImVec2 text_pos = ImVec2(
        pos.x + (size.x - text_size.x) * 0.5f,
        pos.y + (size.y - text_size.y) * 0.5f
    );
    draw_list->AddText(text_pos, IM_COL32(0,0,0,255), label);
    
    // 创建不可见按钮用于点击检测
    if (ImGui::InvisibleButton(label, size)) {
        clicked = true;
    }
    
    return clicked;
}

// 贝塞尔风格的自定义滑块函数
void CustomSliderInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0) {
    // ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 10.0f); // 滑块手柄圆角
    // ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f); // 滑条背景圆角 
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.86f, 0.86f, 0.86f, 1.0f)); // 滑条背景
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.86f, 0.86f, 0.86f, 1.0f)); // 滑条激活背景
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.86f, 0.86f, 0.86f, 1.0f)); // 滑条悬停背景
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.196f, 0.588f, 1.0f, 1.0f)); // 滑块手柄颜色（蓝色）
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.196f, 0.588f, 1.0f, 1.0f)); // 滑块手柄激活颜色 
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f)); // 标签文本颜色（黑色）
    
    ImGui::Text("%s", label); // 显示标签
    ImGui::PopStyleColor(); // 释放标签文本颜色
    
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // 滑条数值文本颜色（白色）
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y - 13.0f)); // 缩小上下间距
    
    // 使用##前缀创建隐藏ID，避免与Text的label冲突
    char slider_id[256];
    snprintf(slider_id, sizeof(slider_id), "##slider_%s", label);
    ImGui::SliderInt(slider_id, v, v_min, v_max, format, flags); // 创建滑条
    
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y - 13.0f)); // 缩小上下间距
    ImGui::PopStyleColor(3); // 匹配上面的3次 PushStyleColor
  //  ImGui::PopStyleVar(2); // 释放圆角样式
}

// 注意：Checkbox函数已在imgui_whitecloud.h中定义，这里不需要重复定义

// 注意：SliderFloat函数已在imgui_whitecloud.h中定义，这里不需要重复定义

// 保留原有的CustomSliderFloat函数以保持兼容性
void CustomSliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.2f", ImGuiSliderFlags flags = 0) {
    SliderFloat(label, v, v_min, v_max, format, flags, 0); // 调用新的SliderFloat函数
}

// 贝塞尔风格的窗口样式设置
void SetBezierStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    // ImGui::StyleColorsLight(); // 注释掉亮色主题以保持透明设置
    
    // 主窗口背景设置为透明
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    
    // 子窗口背景设置为白色
    style.Colors[ImGuiCol_ChildBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    
    // 设置圆角
    style.ChildRounding = 25.0f;        // 子窗口圆角增大
    style.FrameRounding = 25.0f;        // 框架圆角增大
    style.WindowRounding = 20.0f;       // 主窗口圆角增大
    style.TabRounding = 15.0f;          // 标签页圆角
    style.PopupRounding = 20.0f;        // 弹出窗口圆角
    
    // 设置边框
    style.ChildBorderSize = 1.0f;
    style.WindowBorderSize = 3.0f;
    style.FrameBorderSize = 2.0f;
    
    // 标题居中
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    
    // 滚动条样式
    style.ScrollbarRounding = 20.0f;    // 滚动条圆角增大
    style.ScrollbarSize = 25.0f;
    
    // 滑块样式
    style.GrabMinSize = 30.0f;
    style.GrabRounding = 999.0f;        // 保持完全圆形
    
    // 按钮样式
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);  // 按钮文字居中
    
    // 设置透明度
    ImGui::SetNextWindowBgAlpha(0.0f);
    
    // 设置文字颜色为黑色（适合白色背景）
    style.Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
    
    // 设置按钮颜色
    style.Colors[ImGuiCol_Button] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
    
    // 设置更大的内边距
    style.WindowPadding = ImVec2(30, 30);
    style.ItemSpacing = ImVec2(12, 8);
    style.ItemInnerSpacing = ImVec2(8, 6);
}

std::string loadTextFromFile(const std::string& filename) {
    std::string content;
    std::ifstream file(filename);
    if (file.is_open()) {
        content.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
    }
    return content;
}

int 版本;

static bool 触摸驱动 = true;
static bool IsBall = true;

// 在全局变量部分添加雪花相关的静态变量
static std::vector<ImVec2> snowflakes;          // 雪花位置坐标
static std::vector<float> snowflakeSpeeds;      // 雪花下落速度
static std::vector<float> snowflakeSizes;       // 雪花大小
static std::vector<float> snowflakeHorizontalSpeeds; // 雪花水平移动速度
static int snowflakeCount=20;                 // 雪花数量
static float snowflakeMaxSpeed=3.0f;            // 最大下落速度
static float snowflakeMinSpeed=0.5f;            // 最小下落速度
static float snowflakeMaxSize=5.0f;             // 最大雪花尺寸
static float snowflakeMinSize=1.0f;             // 最小雪花尺寸
static float snowflakeMaxHorizontalSpeed=1.0f;  // 最大水平速度
static float snowflakeMinHorizontalSpeed=-1.0f; // 最小水平速度(负值表示向左)

// timer FPS限制;  // 已移除，使用全局定义（第865行）
bool 林徽因 = true;   // 直接显示主界面
bool 公告 = false;   // 不显示登录界面
bool 退出 = false;   // 初始化退出状态
bool active = false;

int GetEventCount() {
    DIR *dir = opendir("/dev/input/");
    dirent *ptr = NULL;
    int count = 0;
    while ((ptr = readdir(dir)) != NULL) {
        if (strstr(ptr->d_name, "event"))
            count++;
    }
    return count ? count : -1;
}



int GetEventCount3()
{
    DIR *dir = opendir("/dev/input/");
    dirent *ptr = NULL;
    int count = 0;
    while ((ptr = readdir(dir)) != NULL)
    {
        if (strstr(ptr->d_name, "event"))
            count++;
    }
    return count ? count : -1;
}
bool kang = true;



int 音量()
{
    int EventCount = GetEventCount3();
    if (EventCount < 0)
    {
        printf("未找到输入设备\n");
        return -1;
    }

    int *fdArray = (int *)malloc(EventCount * sizeof(int));

    for (int i = 0; i < EventCount; i++)
    {
        char temp[128];
        sprintf(temp, "/dev/input/event%d", i);
        fdArray[i] = open(temp, O_RDWR | O_NONBLOCK);
    }

    input_event ev;
    int count = 0; // 记录按下音量键的次数

    while (1)
    {
        for (int i = 0; i < EventCount; i++)
        {
            memset(&ev, 0, sizeof(ev));
            read(fdArray[i], &ev, sizeof(ev));
            if (ev.type == EV_KEY && ev.code == KEY_VOLUMEUP && ev.value == 1 && kang == true)
            {
          MemuSwitch = true;
                    IsBall = true;  
		
            }
            else if (ev.type == EV_KEY && ev.code == KEY_VOLUMEDOWN && ev.value == 1 && kang == true)
            {
           	IsBall = false;
            }
        }

        usleep(3000);
    }

    return 0;
}

bool permeate_record = false;
bool permeate_record_ini = false;

// FPS控制全局变量定义
float FPS = 120.0f;
timer FPS限制;
float FPF显示 = 0.0f;

bool g_Initialized = false;
EGLDisplay display = EGL_NO_DISPLAY;
EGLConfig eglConfig;  // 重命名避免冲突
EGLSurface surface = EGL_NO_SURFACE;
EGLContext context = EGL_NO_CONTEXT;
ANativeWindow *native_window;
int screen_x = 0, screen_y = 0;
int abs_ScreenX = 0, abs_ScreenY = 0;
int native_window_screen_x = 0, native_window_screen_y = 0;
android::ANativeWindowCreator::DisplayInfo displayInfo{0};
bool status = false;

// VK渲染支持
std::unique_ptr<AndroidImgui> graphics = nullptr;
ANativeWindow *window = nullptr;

// ===== 自瞄系统全局变量定义 =====
struct ObjectData object = {0};
struct MatrixData Matrixindex = {{0}};
bool LoadTouch = false;
int Drivermodel = 0;

int init_egl(int _screen_x, int _screen_y, bool log) {
     char YN[32] = {0};
    printf("\n\033[0m开启防录屏模式 [1]开启 [2]关闭:\033[0m ");
    scanf("%s", YN);  // 从标准输入流中读取字符串    
    printf("\n");  // 添加换行

    if (strstr(YN, "1") != NULL) {
 //   printf("\n防录屏已开启");
            status = true;
    } else {
     //   printf("\n防录屏已关启");
            status = false;        
    }

    native_window =
            android::ANativeWindowCreator::Create("念", _screen_x, _screen_y, status);
    ANativeWindow_acquire(native_window);
    // ANativeWindow_acquire(native_window);
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY) {
        printf("eglGetDisplay error=%u\n", glGetError());
        return -1;
    }
    if (log) {
        printf("eglGetDisplay ok\n");
    }
    if (eglInitialize(display, 0, 0) != EGL_TRUE) {
        printf("eglInitialize error=%u\n", glGetError());
        return -1;
    }
    if (log) {
        printf("eglInitialize ok\n");
    }
    EGLint num_config = 0;
    const EGLint attribList[] = {EGL_SURFACE_TYPE,
                                 EGL_WINDOW_BIT,
                                 EGL_RENDERABLE_TYPE,
                                 EGL_OPENGL_ES2_BIT,
                                 EGL_BLUE_SIZE,
                                 5, //-->delete
                                 EGL_GREEN_SIZE,
                                 6, //-->delete
                                 EGL_RED_SIZE,
                                 5, //-->delete
                                 EGL_BUFFER_SIZE,
                                 32, //-->new field
                                 EGL_DEPTH_SIZE,
                                 16,
                                 EGL_STENCIL_SIZE,
                                 8,
                                 EGL_NONE};
    if (eglChooseConfig(display, attribList, nullptr, 0, &num_config) !=
        EGL_TRUE) {
        printf("eglChooseConfig  error=%u\n", glGetError());
        return -1;
    }
    if (log) {
        printf("num_config=%d\n", num_config);
    }
    if (!eglChooseConfig(display, attribList, &eglConfig, 1, &num_config)) {
        printf("eglChooseConfig  error=%u\n", glGetError());
        return -1;
    }
    if (log) {
        printf("eglChooseConfig ok\n");
    }
    EGLint egl_format;
    eglGetConfigAttrib(display, eglConfig, EGL_NATIVE_VISUAL_ID, &egl_format);
    ANativeWindow_setBuffersGeometry(native_window, 0, 0, egl_format);
    const EGLint attrib_list[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
    context = eglCreateContext(display, eglConfig, EGL_NO_CONTEXT, attrib_list);
    if (context == EGL_NO_CONTEXT) {
        printf("eglCreateContext  error = %u\n", glGetError());
        return -1;
    }
    if (log) {
        printf("eglCreateContext ok\n");
    }
    surface = eglCreateWindowSurface(display, eglConfig, native_window, nullptr);
    if (surface == EGL_NO_SURFACE) {
        printf("eglCreateWindowSurface  error = %u\n", glGetError());
        return -1;
    }
    if (log) {
        printf("eglCreateWindowSurface ok\n");
    }
    if (!eglMakeCurrent(display, surface, surface, context)) {
        printf("eglMakeCurrent  error = %u\n", glGetError());
        return -1;
    }
    if (log) {
        printf("eglMakeCurrent ok\n");
    }
    return 1;
}


#include <fstream>
#include <iostream>
#include <string>

void WriteConfigToIni(const std::string& filePath, const std::string& section, const std::string& value) {
    system("mkdir -p /data/光头强配置");
    
    std::ifstream inFile(filePath);
    std::map<std::string, std::string> configMap;
    std::string line;
    
    if (inFile.is_open()) {
        while (getline(inFile, line)) {
            size_t equalPos = line.find('=');
            if (equalPos != std::string::npos) {
                std::string key = line.substr(0, equalPos);
                std::string val = line.substr(equalPos + 1);
                key.erase(0, key.find_first_not_of(" \t\r\n"));
                key.erase(key.find_last_not_of(" \t\r\n") + 1);
                val.erase(0, val.find_first_not_of(" \t\r\n"));
                val.erase(val.find_last_not_of(" \t\r\n") + 1);
                configMap[key] = val;
            }
        }
        inFile.close();
    }
    
    configMap[section] = value;
    
    std::ofstream outFile(filePath, std::ios::out | std::ios::trunc);
    if (outFile.is_open()) {
        for (const auto& pair : configMap) {
            outFile << pair.first << "=" << pair.second << "\n";
        }
        outFile.close();
    }
}

bool ReadConfigFromIni(const std::string& filePath, const std::string& section, std::string& value) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (getline(file, line)) {
        size_t equalPos = line.find('=');
        if (equalPos != std::string::npos) {
            std::string key = line.substr(0, equalPos);
            key.erase(0, key.find_first_not_of(" \t\r\n"));
            key.erase(key.find_last_not_of(" \t\r\n") + 1);
            
            if (key == section) {
                value = line.substr(equalPos + 1);
                value.erase(0, value.find_first_not_of(" \t\r\n"));
                value.erase(value.find_last_not_of(" \t\r\n") + 1);
                file.close();
                return true;
            }
        }
    }
    file.close();
    return false;
}





char *读取文件(char *path) {
    FILE *fp;
    if ((fp = fopen(path, "r")) == NULL) {
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    int filesize = ftell(fp);
    char *str;
    str = (char *) malloc(filesize);
    rewind(fp);
    fread(str, 1, filesize, fp);
    str[filesize] = '\0';
    fclose(fp);
    return str;
}

int 读取() {
   std::string value;
   std::string value1;
   std::string value2;
   std::string value3;
   std::string value4;
   std::string value5;
 std::string value6;
 std::string value7;
 std::string value8;
 std::string value9;
 std::string value10;
std::string value11;
std::string value12;
std::string value13;
std::string value14;
std::string value15;
std::string value16;
std::string value17;
std::string value18;
std::string value19;
std::string value20;
std::string value21;
std::string value22;
std::string value23;
std::string value24;
std::string value25;
std::string value26;
std::string value27;
// 读取所有配置（INI文件方式）
LoadAllConfig(); 

///////////旧的读取方式（保留兼容）
if (ReadConfigFromIni("/data/光头强配置/配置.ini", "兵线绘制", value)) {
    兵线 = (value == "1");
} 
if (ReadConfigFromIni("/data/光头强配置/配置.ini", "地图头像", value1)) {
    地图 = (value1 == "1");
} 
if (ReadConfigFromIni("/data/光头强配置/配置.ini", "人物方框", value2)) {
    方框 = (value2 == "1");
} 
if (ReadConfigFromIni("/data/光头强配置/配置.ini", "方框内技能计时", value3)) {
    方框技能 = (value3 == "1");
}
std::string value_暴露不绘;
if (ReadConfigFromIni("/data/光头强配置/配置.ini", "暴露不绘小地图头像", value_暴露不绘)) {
    暴露不绘小地图头像 = (value_暴露不绘 == "1");
}
std::string value_暴露视野;
if (ReadConfigFromIni("/data/光头强配置/配置.ini", "暴露视野不绘", value_暴露视野)) {
    暴露视野不绘 = (value_暴露视野 == "1");
} 
if (ReadConfigFromIni("/data/光头强配置/配置.ini", "技能计时面板", value4)) {
    顶上技能2 = (value4 == "1");
} 
if (ReadConfigFromIni("/data/光头强配置/配置.ini", "野怪计时", value6)) {
    野怪 = (value6 == "1");
} 
if (ReadConfigFromIni("/data/光头强配置/配置.ini", "射线开关", value7)) {
    射线 = (value7 == "1");
} 
if (ReadConfigFromIni("/data/光头强配置/配置.ini", "自身视野判断", value8)) {
    自身 = (value8 == "1");
} 
if (ReadConfigFromIni("/data/光头强配置/配置.ini", "兵线实体", value9)) {
    实体兵线 = (value9 == "1");
} 
if (ReadConfigFromIni("/data/光头强配置/配置.ini", "实体技能显示", value11)) {
    实体技能 = (value11 == "1");
} 
if (ReadConfigFromIni("/data/光头强配置/配置.ini", "血量", value10)) {
    血量 = (value10 == "1");
} 
if (ReadConfigFromIni("/data/光头强配置/配置.ini", "野怪血量显示", value12)) {
    野血 = (value12 == "1");
} 

///////////滑动调区域
     if (ReadConfigFromIni("配置.ini", "视距调节", value10)) {
        shangdi = atof(value10.c_str()); // 将字符串转换为浮点数
    }
   if (ReadConfigFromIni("/data/光头强配置/头像左右.ini", "头像左右", value11)) {
        SmallMapX = atof(value11.c_str()); // 将字符串转换为浮点数
    }
    if (ReadConfigFromIni("/data/光头强配置/头像上下.ini", "头像上下", value12)) {
        SmallMapY = atof(value12.c_str()); // 将字符串转换为浮点数
    }
    if (ReadConfigFromIni("/data/光头强配置/实体左右.ini", "实体左右", value13)) {
        SmallHPX = atof(value13.c_str()); 
    }
if (ReadConfigFromIni("/data/光头强配置/实体上下.ini", "实体上下", value14)) {
        SmallHPY = atof(value14.c_str()); 
    }
if (ReadConfigFromIni("/data/光头强配置/头像大小.ini", "头像大小", value15)) {
        touxiangdaxiao = atof(value15.c_str()); 
    }
if (ReadConfigFromIni("/data/光头强配置/血量大小.ini", "血量大小", value16)) {
        xiaodituxue = atof(value16.c_str()); 
    }
if (ReadConfigFromIni("/data/光头强配置/地图比例.ini", "地图缩放比例", value17)) {
        jiange = atof(value17.c_str()); 
    }
if (ReadConfigFromIni("/data/光头强配置/技能左右.ini", "技能计时左右", value18)) {
        jinenglanzX = atof(value18.c_str()); 
    }
    if (ReadConfigFromIni("/data/光头强配置/技能上下.ini", "技能计时上下", value19)) {
        jinenglanzY = atof(value19.c_str()); 
    }
    if (ReadConfigFromIni("/data/光头强配置/技能左右.ini", "技能计时左右", value20)) {
        jinenglanzX = atof(value20.c_str()); 
    }
if (ReadConfigFromIni("/data/光头强配置/视野左右.ini", "自身视野左右", value21)) {
        屏幕x  = atof(value21.c_str()); 
    }
    if (ReadConfigFromIni("/data/光头强配置/视野上下.ini", "自身视野上下", value22)) {
        屏幕y = atof(value22.c_str()); 
    }
if (ReadConfigFromIni("/data/光头强配置/配置.ini", "斩杀值", value23)) {
        zhansha = atof(value23.c_str()); 
    }
if (ReadConfigFromIni("/data/光头强配置/触摸左右.ini", "触摸位置左右", value24)) {
        zhsX= atof(value24.c_str()); 
    }
    if (ReadConfigFromIni("/data/光头强配置/触摸上下.ini", "触摸位置上下", value25)) {
        zhsY= atof(value25.c_str()); 
    }
if (ReadConfigFromIni("/data/光头强配置/帧率.ini", "帧率数值", value26)) {
        FPS= atof(value26.c_str()); 
    }

// 共享设置配置读取
std::string shareValue1, shareValue2, shareValue3, shareValue4, shareValue5, shareValue6, shareValue7, shareValue8;
if (ReadConfigFromIni("/data/光头强配置/共享配置.ini", "共享左右", shareValue1)) {
    ESPMenu.小地图左右调整 = atoi(shareValue1.c_str());
}
if (ReadConfigFromIni("/data/光头强配置/共享配置.ini", "共享上下", shareValue2)) {
    ESPMenu.小地图上下调整 = atoi(shareValue2.c_str());
}
if (ReadConfigFromIni("/data/光头强配置/共享配置.ini", "共享野怪左右", shareValue3)) {
    ESPMenu.野怪左右调整 = atoi(shareValue3.c_str());
}
if (ReadConfigFromIni("/data/光头强配置/共享配置.ini", "共享野怪上下", shareValue4)) {
    ESPMenu.野怪上下调整 = atoi(shareValue4.c_str());
}
if (ReadConfigFromIni("/data/光头强配置/共享配置.ini", "共享兵线左右", shareValue5)) {
    ESPMenu.兵线左右调整 = atoi(shareValue5.c_str());
}
if (ReadConfigFromIni("/data/光头强配置/共享配置.ini", "共享兵线上下", shareValue6)) {
    ESPMenu.兵线上下调整 = atoi(shareValue6.c_str());
}
if (ReadConfigFromIni("/data/光头强配置/共享配置.ini", "共享分辨率x", shareValue7)) {
    ESPMenu.分辨率X = atoi(shareValue7.c_str());
}
if (ReadConfigFromIni("/data/光头强配置/共享配置.ini", "共享分辨率y", shareValue8)) {
    ESPMenu.分辨率Y = atoi(shareValue8.c_str());
}

    string LoadFile = "/data/光头强配置/颜色配置.ini";
    pdys = ReadDrawSet((总颜色 *) &颜色配置, LoadFile.c_str());

    return pdys;
}




#include <arpa/inet.h>
#include <cstdlib>
#include <ctime>
#include <errno.h>
#include <fstream>
#include <iostream>
#include <iostream>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;
// 用于存储当前选择的选项


// 按钮标签数组
const char* verticalMenuOptions[] = {"绘制区域", "绘制设置", "设置区域", "触摸区域", "内存区域", "换装区域", "录屏工具"};

// 按钮数量
int buttonCount = sizeof(verticalMenuOptions) / sizeof(verticalMenuOptions[0]);

// 当前选中的标签索引
int currentSelectedIndex = 0;

// 激活按钮的颜色
ImVec4 activeColor = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);

// 非激活按钮的颜色
ImVec4 inactiveColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

void OnButtonClick(int index) {
    currentSelectedIndex = index; // 更新当前选中的标签索引
    // 根据索引设置 MenuTab
    MenuTab = index + 1; // 假设 MenuTab 从 1 开始
    
    // 设置动画目标位置（贝塞尔风格垂直切换）
    switch(index) {
     
        case 1: 
            homepages = -400.0f; // 绘制设置 - 向上偏移400px
            targetWidth = 1150.0f; targetHeight = 800.0f; // 绘制设置 - 中等大小
            break;
        case 2: 
            homepages = -800.0f; // 设置区域 - 向上偏移800px
            targetWidth = 1200.0f; targetHeight = 850.0f; // 设置区域 - 更大
            break;
        case 3: 
            homepages = -1200.0f; // 触摸区域 - 向上偏移1200px
            targetWidth = 1000.0f; targetHeight = 700.0f; // 触摸区域 - 较小
            break;
        case 4: 
            homepages = -1600.0f; // 内存区域 - 向上偏移1600px
            targetWidth = 1150.0f; targetHeight = 800.0f; // 内存区域 - 中等
            break;
        case 5: 
            homepages = -2000.0f; // 换装区域 - 向上偏移2000px
            targetWidth = 1200.0f; targetHeight = 850.0f; // 换装区域 - 较大
          
            break;
        case 7: 
            homepages = -2800.0f; // 录屏工具 - 向上偏移2800px
            targetWidth = 1000.0f; targetHeight = 750.0f; // 录屏工具 - 标准大小
            break;
        default: 
            homepages = 0.0f;
            targetWidth = 1300.0f; targetHeight = 900.0f;
            break;
    }
}


// 当前选中的标签索引
int currentSelectedTabIndex = 0;

// 按钮标签数组
const char* tabLabels[] = {"绘制调节", "显示设置", "颜色设置","共享设置"};

// 按钮数量
int tabCount = sizeof(tabLabels) / sizeof(tabLabels[0]);

void OnTabChange(int index) {
    currentSelectedTabIndex = index; // 更新当前选中的标签索引
    MenuTab2 = index + 1; // 假设 MenuTab2 从 1 开始
    
    // 设置横向动画目标位置和窗口大小
    switch(index) {
        case 0: 
            settingPages = 0.0f;    // 绘制调节 - 无偏移
            // 绘制调节需要更多空间
            if (MenuTab == 2) { targetWidth = 1300.0f; targetHeight = 900.0f; }
            break;
        case 1: 
            settingPages = -600.0f; // 显示设置 - 向左偏移600px
            // 显示设置较简单
            if (MenuTab == 2) { targetWidth = 1150.0f; targetHeight = 800.0f; }
            break;
        case 2: 
            settingPages = -1200.0f; // 颜色设置 - 向左偏移1200px
            // 颜色设置需要中等空间
            if (MenuTab == 2) { targetWidth = 1200.0f; targetHeight = 850.0f; }
            break;
        case 3: 
            settingPages = -1800.0f; // 共享设置 - 向左偏移1800px
            // 共享设置需要最大空间
            if (MenuTab == 2) { targetWidth = 1350.0f; targetHeight = 950.0f; }
            break;
        default: 
            settingPages = 0.0f; 
            break;
    }
}




// 在你的主循环中调用 ShowComboAndCallFunction
void 写出文件(char *aaa, char *bbb) {

    std::ofstream ofs;

    ofs.open(aaa, std::ios::out);

    ofs << bbb;

    ofs.close(); 
}


 
        

 
void ImGui_init() {
    if (g_Initialized) {
        return;
    }
    
 // 初始化雪花 - 现在在菜单显示时才真正初始化
    snowflakes.clear();
    snowflakeSpeeds.clear();
    snowflakeSizes.clear();
    snowflakeHorizontalSpeeds.clear();
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    
    ImGuiIO &io = ImGui::GetIO();
    io.IniFilename = NULL;
    
    // 禁用ImGui错误输出（OpenGL模式）
    io.ConfigErrorRecovery = true;
    io.ConfigErrorRecoveryEnableAssert = false;
    io.ConfigErrorRecoveryEnableDebugLog = false;
    io.ConfigErrorRecoveryEnableTooltip = false;
    
    /*switch (Color[0])
    {
        case 0:
           // ImGui::StyleColorsRed();
                   ImGui::StyleColorsDark;
        break;
        case 1:
            ImGui::StyleColorsBlue();
        break;
        case 2:
            ImGui::StyleColorsOrange();
        break;
        }
  */
    new std::thread(音量);
    获取头像2();
    获取图标();
    获取其他图标();
    
DrawInit();
// 移除启动时的游戏进程初始化，改为在开启绘制时执行
    //PT = ImAgeHeadFile1(PTlogo, sizeof(PTlogo));
    //ZY = ImAgeHeadFile1(ZYlogo, sizeof(ZYlogo));
    //SX = ImAgeHeadFile1(SXlogo, sizeof(SXlogo));
    int pdys = 读取();
    pid = getPID("com.tencent.tmgp.sgame");
    //获取坐标();
    yxpx = abs_ScreenX;
    yxpy = abs_ScreenY;
    // gotpid();
    ImGui_ImplAndroid_Init(native_window);
    ImGui_ImplOpenGL3_Init("#version 300 es");

    ImFontConfig font_cfg;
    font_cfg.SizePixels = 20.0f;
    io.Fonts->AddFontFromMemoryTTF((void *) font_v, font_v_size, 31.0f, NULL,
                                   io.Fonts->GetGlyphRangesChineseFull());
    io.Fonts->AddFontDefault(&font_cfg);
    
    // 构建字体图谱
    io.Fonts->Build();

    ImGui::GetStyle().ScaleAllSizes(3.0f);
    g_Initialized = true;
}

// VK渲染初始化
void init_VK_drawdata() {
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle* style = &ImGui::GetStyle();
    
    // 禁用ImGui错误输出（避免Debug##Default窗口错误干扰）
    io.ConfigErrorRecovery = true;
    io.ConfigErrorRecoveryEnableAssert = false;
    io.ConfigErrorRecoveryEnableDebugLog = false;
    io.ConfigErrorRecoveryEnableTooltip = false;
    
    // 启动音量键监听线程
    new std::thread(音量);
    
    // 加载默认中文字体（带完整中文支持）
    ImFontConfig font_cfg;
    font_cfg.SizePixels = 20.0f;
    io.Fonts->AddFontFromMemoryTTF((void *) font_v, font_v_size, 31.0f, NULL,
                                   io.Fonts->GetGlyphRangesChineseFull());
    io.Fonts->AddFontDefault(&font_cfg);
    
    // 构建字体图谱
    io.Fonts->Build();
    
    // 样式设置
    ImGui::GetStyle().ScaleAllSizes(3.0f);
    ImGui::SetNextWindowBgAlpha(1.0f);
    
    style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style->FramePadding = ImVec2(12, 12);
    style->FrameRounding = 8.0f;
    style->FrameBorderSize = 0.0f;
    style->WindowBorderSize = 0.0f;
    style->ScrollbarRounding = 0.0f;
    style->WindowMenuButtonPosition = ImGuiDir_Left;
    style->ScrollbarSize = 20.0f;
    style->GrabRounding = 7.0f;
    style->GrabMinSize = 0.0f;
    
    // 读取配置（VK模式）
    int pdys = 读取();
   
}

// VK兼容的纹理加载函数实现（moved from include.h）
TextureInfoa createTexturePNGFromMem_VK(const unsigned char *buf, int len) {
    TextureInfoa textureInfo;
    
    // VK渲染模式：使用graphics接口加载纹理
    if (graphics != nullptr) {
        TextureInfo vkTex = graphics->LoadTextureFromMemory((void*)buf, len);
        textureInfo.textureId = (GLuint*)vkTex.DS;
        textureInfo.w = vkTex.w;
        textureInfo.h = vkTex.h;
        return textureInfo;
    }
    
    // OpenGL渲染模式：使用原有GL函数
    int w, h, n;
    stbi_uc *data = stbi_load_from_memory(buf, len, &w, &h, &n, 0);
    GLuint texture;
    glGenTextures(1, &texture);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    if (n == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    stbi_image_free(data);
    textureInfo.textureId = (GLuint *) texture;
    textureInfo.w = w;
    textureInfo.h = h;
    return textureInfo;
}

int huitu = 0;

bool 单透开启 = false;

 void tick() {
    
    // VK渲染模式下的UI资源懒加载（第一次进入tick时加载，不包括DrawInit）
    static bool vk_ui_resources_loaded = false;
    if (!vk_ui_resources_loaded && graphics != nullptr) {
        // 在第一次渲染循环时加载UI资源（此时VK已完全初始化）
        获取头像2();
        获取图标();
        获取其他图标();
        
        // 初始化游戏绘制分辨率变量
        yxpx = abs_ScreenX;
        yxpy = abs_ScreenY;
        
        // 注意：不在这里调用DrawInit()，由用户点击"启用绘制"按钮手动触发
        vk_ui_resources_loaded = true;
    }
    
    // 特定分辨率自动调整小地图头像位置（只执行一次）
    static bool resolutionChecked = false;
    if (!resolutionChecked) {
        // 调试输出实际分辨率
        // printf("[分辨率检测] abs_ScreenX=%d, abs_ScreenY=%d\n", abs_ScreenX, abs_ScreenY);
        
        if (abs_ScreenX == 1264 && abs_ScreenY == 2780) {
            SmallMapX = 53;
            SmallMapY = 12;
            WriteConfigToIni("/data/光头强配置/头像左右.ini", "头像左右", "53");
            WriteConfigToIni("/data/光头强配置/头像上下.ini", "头像上下", "12");
            // printf("[分辨率调整] 检测到1264x2780，小地图头像已设置为左右53上下12并保存到配置文件\n");
        } else if (abs_ScreenX == 2780 && abs_ScreenY == 1264) {
            SmallMapX = 53;
            SmallMapY = 12;
            WriteConfigToIni("/data/光头强配置/头像左右.ini", "头像左右", "53");
            WriteConfigToIni("/data/光头强配置/头像上下.ini", "头像上下", "12");
            // printf("[分辨率调整] 检测到2780x1264，小地图头像已设置为左右53上下12并保存到配置文件\n");
        } else if (abs_ScreenX == 1080 && abs_ScreenY == 2376) {
            SmallMapX = 31;
            SmallMapY = 10;
            WriteConfigToIni("/data/光头强配置/头像左右.ini", "头像左右", "31");
            WriteConfigToIni("/data/光头强配置/头像上下.ini", "头像上下", "10");
            // printf("[分辨率调整] 检测到1080x2376，小地图头像已设置为左右31上下10并保存到配置文件\n");
        } else if (abs_ScreenX == 2376 && abs_ScreenY == 1080) {
            SmallMapX = 31;
            SmallMapY = 10;
            WriteConfigToIni("/data/光头强配置/头像左右.ini", "头像左右", "31");
            WriteConfigToIni("/data/光头强配置/头像上下.ini", "头像上下", "10");
            // printf("[分辨率调整] 检测到2376x1080，小地图头像已设置为左右31上下10并保存到配置文件\n");
        }
        
        resolutionChecked = true;
    }

     // 如果雪花未初始化，进行初始化
        if(snowflakes.empty()) {
            for(int i=0;i<snowflakeCount;++i) {
                // 初始位置将在菜单显示时设置
                snowflakes.push_back(ImVec2(0,0));
                snowflakeSpeeds.push_back(((float)rand()/RAND_MAX)*(snowflakeMaxSpeed-snowflakeMinSpeed)+snowflakeMinSpeed);
                snowflakeSizes.push_back(((float)rand()/RAND_MAX)*(snowflakeMaxSize-snowflakeMinSize)+snowflakeMinSize);
                snowflakeHorizontalSpeeds.push_back(((float)rand()/RAND_MAX)*(snowflakeMaxHorizontalSpeed-snowflakeMinHorizontalSpeed)+snowflakeMinHorizontalSpeed);
            }
        }

    
    // FPS限制.AotuFPS_init();
    // float 帧率 = 1000.0f / FPS限制.AotuFPS();
    
    // 更新窗口动画
    UpdateWindowAnimation();
    
    // 监控IsBall变量变化，触发动画（音量键控制的是IsBall）
    static bool lastIsBall = true;  // 初始值应该和IsBall一致
    static bool firstRun = true;
    
    // 第一次运行时的初始化
    if (firstRun) {
        lastIsBall = IsBall;
        if (IsBall && 林徽因) {
            StartOpenAnimation();
        } else {
            // 如果初始就是隐藏状态，设置为完全隐藏
            windowScale = 0.0f;
            windowAlpha = 0.0f;
            windowVisible = false;
        }
        firstRun = false;
    }
    
    // 检测IsBall状态变化（音量键控制）
    if (IsBall != lastIsBall) {
        if (IsBall && 林徽因) {
            // 从隐藏变为显示 - 触发打开动画
            StartOpenAnimation();
        } else {
            // 从显示变为隐藏 - 触发关闭动画
            StartCloseAnimation();
        }
        lastIsBall = IsBall;
    }
    //FPS限制.setAffinity();
    
    // FPS实时显示
    // static int frameCounter = 0;
    // frameCounter++;
    // 
    // float actualFPS = 1000.0f / (FPF显示 + 0.001f);  // 避免除0
    // 
    // // 添加更详细的性能信息
    // char perfInfo[256];
    // snprintf(perfInfo, sizeof(perfInfo), 
    //     "渲染FPS: %d | 帧时 间: %.2fms | 帧计数: %d | 设置FPS: %.0f",
    //     (int)actualFPS, FPF显示, frameCounter, FPS);
    // 
    // ImGui::GetForegroundDrawList()->AddText(ImVec2(10, 10), IM_COL32(0, 255, 0, 255), perfInfo);
    
    ImGui::GetStyle().FrameRounding = 2.0f; // 设置控件边框的圆角为10像素
    ImGui::GetStyle().GrabRounding = 2.0f; // 设置控件抓取的圆角为10像素
    ImGui::GetStyle().WindowRounding = 20.0f; // 设置窗口圆角
    ImGui::GetStyle().WindowBorderSize = 2.0f; // 设置窗口边框大小
    ImGui::GetStyle().ChildRounding = 15.0f; // 设置子窗口圆角
    ImGui::GetStyle().ChildBorderSize = 1.0f; // 设置子窗口边框大小
    // 边框颜色保持默认，让它和背景协调
    ImGui::SetNextWindowBgAlpha(0.5f); // 设置窗口的背景透明度为 0.5（50% 不透明）

    ImGuiIO &io = ImGui::GetIO();

    static ImVec4 clear_color = ImVec4(0, 0, 0, 0);
    ImGuiStyle &Style = ImGui::GetStyle();

    static float ANIM_SPEED = 0.35f;
    static float Velua = 1.0f;  // 修复：VK渲染首次打开窗口大小问题，直接设为完整尺寸
Velua = ImClamp(Velua + (io.DeltaTime / ANIM_SPEED) * (IsBall ? 1.0f : -1.0f), 0.0f, 1.0f);
// 增加窗口大小的比例因子
const float sizeIncreaseFactor = 1.13f; // 增加10%

// 根据Velua的值和增加因子来设置窗口大小
ImGui::SetWindowSize("念", {745 * Velua * sizeIncreaseFactor, 550 * Velua * sizeIncreaseFactor});

// 更新条件判断，因为窗口大小已经增加
if ((745 * Velua * sizeIncreaseFactor) <= 745 && (550 * Velua * sizeIncreaseFactor) <= 550) {
    // 窗口大小在限制范围内，可以继续使用
} else if ((745 * Velua * sizeIncreaseFactor) >= 100 && (550 * Velua * sizeIncreaseFactor) >= 100) {
    // 窗口大小仍然符合最小尺寸要求
}

// 更新MemuSwitch的条件判断，因为窗口大小已经增加
if ((745 * Velua * sizeIncreaseFactor) <= 100 && !IsBall) {
    BallSwitch = true;MemuSwitch = false;
}
 	if (BallSwitch)
    	{
    	/*	Style.WindowRounding = 70.0f;
    		if (ImGui::Begin("Ball", &BallSwitch,  ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar))
        	{
				auto Pos = ImGui::GetWindowPos();
    			Window = ImGui::GetCurrentWindow();
    			DrawPT({Pos.x + 50, Pos.y + 50}, 70);
    			if (ImGui::IsItemActive()) {
    				if (!IsDown) {
    					IsDown = true;
    					ImagePos = ImGui::GetWindowPos();
    				}
    			} else if (IsDown) {
    				IsDown = false;
    				if(ImagePos.x == ImGui::GetWindowPos().x && ImagePos.y == ImGui::GetWindowPos().y) {
    					IsBall = true;
    					MemuSwitch = true;
    					BallSwitch = false;
    			    	ImGui::SetWindowPos("念", Pos, ImGuiCond_Always);
    				}
    			}
    		}
    		ImGui::End();*/
    	}
    if (MemuSwitch) {
        // 应用贝塞尔风格样式
        SetBezierStyle();
        Style.WindowRounding = 10.0f;
        
        // 动态窗口大小动画更新
        windowWidth += (targetWidth - windowWidth) * 0.1f;
        windowHeight += (targetHeight - windowHeight) * 0.1f;
        
        // 垂直切换动画更新（贝塞尔风格）
        homepage += (homepages - homepage) * 0.15f;
        
        // 横向切换动画更新（设置页面内部）
        settingPage += (settingPages - settingPage) * 0.15f;
        
        // 应用动画缩放和透明度
        float animatedWidth = windowWidth * windowScale;
        float animatedHeight = windowHeight * windowScale;
        
        // 设置动画窗口尺寸和透明度
        ImGui::SetNextWindowSize(ImVec2(animatedWidth, animatedHeight), ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.0f); // 设置窗口完全透明，才能看到背景绘制列表的内容
        
        // 窗口居中显示（缩放时保持居中）
        // ImVec2 screenCenter = ImVec2(displayInfo.width * 0.5f, displayInfo.height * 0.5f);
        // ImVec2 windowPos = ImVec2(screenCenter.x - animatedWidth * 0.5f, screenCenter.y - animatedHeight * 0.5f);
        // ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
        ImVec2 screenCenter = ImVec2(displayInfo.width * 0.5f, displayInfo.height * 0.5f);
ImVec2 windowPos = ImVec2(screenCenter.x - animatedWidth * 0.5f, screenCenter.y - animatedHeight * 0.5f);
ImGui::SetNextWindowPos(windowPos, ImGuiCond_Once); // 仅首次居中，之后可自由拖动

        // 设置主窗口圆角（已在SetBezierStyle中设置）
        
        // 渲染条件：IsBall和林徽因都为true或者正在播放关闭动画
        if (((IsBall && 林徽因 && (windowVisible || isWindowOpening)) || isWindowClosing)) {
            if (ImGui::Begin("念", &MemuSwitch,
                             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoScrollbar)) 
            {
                                        // 获取菜单窗口的位置和大小
            ImVec2 menuPos = ImGui::GetWindowPos();
            ImVec2 menuSize = ImGui::GetWindowSize();
            
            // 懒加载悬浮窗背景纹理（VK渲染需要在此处加载）
            if (xishi背景纹理.textureId == 0) {
                xishi背景纹理 = createTexturePNGFromMem(xishi, sizeof(xishi));
            }
            
            // 绘制悬浮窗背景图（使用窗口绘制列表，在所有内容之前绘制）
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            
            // 使用通道分离：0=背景，1=前景内容
            draw_list->ChannelsSplit(2);
            draw_list->ChannelsSetCurrent(0); // 切换到背景通道
            
            // 如果背景图纹理加载成功，直接绘制背景图
            if (xishi背景纹理.textureId != 0) {
                draw_list->AddImage(
                    (ImTextureID)xishi背景纹理.textureId,
                    menuPos,
                    ImVec2(menuPos.x + menuSize.x, menuPos.y + menuSize.y),
                    ImVec2(0, 0), ImVec2(1, 1),
                    IM_COL32(255, 255, 255, 255)  // 完全不透明
                );
            } else {
                // 如果纹理未加载，显示蓝紫色底色
                draw_list->AddRectFilled(
                    menuPos,
                    ImVec2(menuPos.x + menuSize.x, menuPos.y + menuSize.y),
                    IM_COL32(80, 60, 120, 250),
                    20.0f  // 圆角
                );
            }
            
            draw_list->ChannelsSetCurrent(1); // 切换回前景通道，后续UI在前景绘制
            
            // 初始化标签页位置（贝塞尔风格动画）
            float windowHeight = menuSize.y;
            float windowWidth = menuSize.x;
            tab0 = 0.0f;                    // 绘制区域在顶部
            tab1 = windowHeight * 0.8f;    // 绘制设置
            tab2 = windowHeight * 1.6f;    // 设置区域
            tab3 = windowHeight * 2.4f;    // 触摸区域
            tab4 = windowHeight * 3.2f;    // 内存区域
            tab5 = windowHeight * 4.0f;    // 换装区域
        
            
            // 初始化横向标签页位置（设置页面内部）
            settingTab0 = 0.0f;                     // 绘制调节在左侧
            settingTab1 = windowWidth * 0.25f;     // 显示设置
            settingTab2 = windowWidth * 0.5f;      // 颜色设置
            settingTab3 = windowWidth * 0.75f;     // 共享设置
            
            // 平滑动画更新
            homepage += (homepages - homepage) * 0.15f;
            settingPage += (settingPages - settingPage) * 0.15f;
            
            // 更新和绘制雪花 - 只在菜单区域内
            for(int i=0;i<snowflakeCount;++i) {
                ImVec2& pos = snowflakes[i];
                float speed = snowflakeSpeeds[i];
                float size = snowflakeSizes[i];
                float horizontalSpeed = snowflakeHorizontalSpeeds[i];
                
                // 更新雪花位置
                pos.y += speed;
                pos.x += horizontalSpeed;
                
                // 检查雪花是否移出菜单区域
                bool outOfMenu = pos.y > (menuPos.y + menuSize.y + size) ||
                                pos.x < (menuPos.x - size) ||
                                pos.x > (menuPos.x + menuSize.x + size);
                
                // 如果雪花移出菜单区域，重新生成一个新的雪花
                if(outOfMenu) {
                    float startX = menuPos.x + ((float)rand()/RAND_MAX)*menuSize.x;
                    float startY = menuPos.y - snowflakeMaxSize;
                    pos = ImVec2(startX, startY);
                    
                    // 随机决定雪花从哪边开始(1/3概率从左侧，1/3概率从右侧)
                    int startSide = rand()%3;
                    if(startSide == 1) {
                        pos = ImVec2(menuPos.x - snowflakeMaxSize, menuPos.y + ((float)rand()/RAND_MAX)*menuSize.y);
                    }
                    else if(startSide == 2) {
                        pos = ImVec2(menuPos.x + menuSize.x + snowflakeMaxSize, menuPos.y + ((float)rand()/RAND_MAX)*menuSize.y);
                    }
                }
                
                // 只在菜单区域内绘制雪花
                if(pos.x >= menuPos.x - size && pos.x <= menuPos.x + menuSize.x + size &&
                   pos.y >= menuPos.y - size && pos.y <= menuPos.y + menuSize.y + size) {
                    ImGui::GetWindowDrawList()->AddCircleFilled(pos, size, IM_COL32(255,255,255,255));
                }
            }
 


     if(林徽因) {                  
            // 临时设置子窗口背景为透明，让背景图可见
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

 
            auto Pos = ImGui::GetWindowPos();
            Window = ImGui::GetCurrentWindow();
           // ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {25, 25});
            if (ImGui::BeginChild("##左侧菜单标题", ImVec2(220, 0), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NavFlattened))
        {
ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0)); 
ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0)); 
ImGui::TextColored(ImVec4(0.3f, 0.7f, 1.0f, 1.0f), "");
ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "");   

ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5); // 设置光标位置
ImGui::Separator();

// 绘制区域按钮
if (currentSelectedIndex == 0) {
    ImGui::PushStyleColor(ImGuiCol_Button, activeColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);
} else {
    ImGui::PushStyleColor(ImGuiCol_Button, inactiveColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, inactiveColor);
}
if (CustomButton("绘制区域", ImVec2(ImGui::GetContentRegionAvail().x, 81))) {
    OnButtonClick(0);
    文字传入("已切换到绘制区域");
}
ImGui::PopStyleColor(2);
ImGui::Separator();

// 绘制设置按钮
if (currentSelectedIndex == 1) {
    ImGui::PushStyleColor(ImGuiCol_Button, activeColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);
} else {
    ImGui::PushStyleColor(ImGuiCol_Button, inactiveColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, inactiveColor);
}
if (CustomButton("绘制设置", ImVec2(ImGui::GetContentRegionAvail().x, 81))) {
    OnButtonClick(1);
    文字传入("已切换到绘制设置");
}
ImGui::PopStyleColor(2);
ImGui::Separator();

// 设置区域按钮
if (currentSelectedIndex == 2) {
    ImGui::PushStyleColor(ImGuiCol_Button, activeColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);
} else {
    ImGui::PushStyleColor(ImGuiCol_Button, inactiveColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, inactiveColor);
}

// 绘制设置图标在按钮左边
if (设置图标纹理.textureId != 0) {
    ImVec2 button_pos = ImGui::GetCursorScreenPos();
    ImVec2 icon_size = ImVec2(24, 24);  // 图标大小
    ImVec2 icon_pos = ImVec2(button_pos.x + 15, button_pos.y + (81 - icon_size.y) * 0.5f);  // 图标位置
    
    // 绘制图标
    ImU32 icon_color = (currentSelectedIndex == 2) ? IM_COL32(255, 255, 255, 255) : IM_COL32(150, 150, 150, 255);
    ImGui::GetWindowDrawList()->AddImage(
        (ImTextureID)设置图标纹理.textureId,
        icon_pos,
        ImVec2(icon_pos.x + icon_size.x, icon_pos.y + icon_size.y),
        ImVec2(0, 0), ImVec2(1, 1),
        icon_color
    );
}

if (CustomButton("设置区域", ImVec2(ImGui::GetContentRegionAvail().x, 81))) {
    OnButtonClick(2);
    文字传入("已切换到设置区域");
}
ImGui::PopStyleColor(2);
ImGui::Separator();

// 触摸区域按钮
if (currentSelectedIndex == 3) {
    ImGui::PushStyleColor(ImGuiCol_Button, activeColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);
} else {
    ImGui::PushStyleColor(ImGuiCol_Button, inactiveColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, inactiveColor);
}
if (CustomButton("触摸区域", ImVec2(ImGui::GetContentRegionAvail().x, 81))) {
    OnButtonClick(3);
    文字传入("已切换到触摸区域");
}
ImGui::PopStyleColor(2);
ImGui::Separator();

// 内存区域按钮
if (currentSelectedIndex == 4) {
    ImGui::PushStyleColor(ImGuiCol_Button, activeColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);
} else {
    ImGui::PushStyleColor(ImGuiCol_Button, inactiveColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, inactiveColor);
}
if (CustomButton("内存区域", ImVec2(ImGui::GetContentRegionAvail().x, 81))) {
    OnButtonClick(4);
    文字传入("已切换到内存区域");
}
ImGui::PopStyleColor(2);
ImGui::Separator();

// 换装区域按钮
if (currentSelectedIndex == 5) {
    ImGui::PushStyleColor(ImGuiCol_Button, activeColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);
} else {
    ImGui::PushStyleColor(ImGuiCol_Button, inactiveColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, inactiveColor);
}
if (CustomButton("换装区域", ImVec2(ImGui::GetContentRegionAvail().x, 81))) {
    OnButtonClick(5);
    文字传入("已切换到换装区域");
}
ImGui::PopStyleColor(2);
ImGui::Separator();



// 录屏工具按钮
if (currentSelectedIndex == 7) {
    ImGui::PushStyleColor(ImGuiCol_Button, activeColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);
} else {
    ImGui::PushStyleColor(ImGuiCol_Button, inactiveColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, inactiveColor);
}
if (CustomButton("录屏工具", ImVec2(ImGui::GetContentRegionAvail().x, 81))) {
    OnButtonClick(7);
    文字传入("已切换到录屏工具");
}
ImGui::PopStyleColor(2);

/*if (ImGui::Button("隐藏窗口", ImVec2(170, 75))) {
    IsBall = false;
}*/
ImGui::PopStyleColor(2);

                ImGui::Separator();//横线分割
            }  // 结束BeginChild("##左侧菜单标题")的if
         ImGui::EndChild();
        // ImGui::PopStyleVar(1);  // 恢复WindowPadding，必须在EndChild后无条件执行
	
    	ImGui::SameLine();
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
    			ImGui::SameLine();
    			//	  ImGui::SameLine();
	
    			//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {40 , 25});	
            if (ImGui::BeginChild("##右菜单", ImVec2(-1, -1), false,
                                  ImGuiWindowFlags_NoScrollbar)) {
                
                //DrawSX({Pos.x + 500, Pos.y + 350}, 400);  
                
                    switch (MenuTab) {
                        case 1: {
                            // 应用绘制区域的动画偏移
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + homepage);
           
        // 绘制控制菜单（两行布局）
        
        // 渐变字标题 
        渐变字("光头强解密内核开源", ImColor(255, 160, 255, 255), ImColor(97, 206, 255, 255), 60.0f, true);
        ImGui::Spacing();
        
        // 第一排：开启绘制按钮（左对齐，右边顶满）
        
// 新增：人物共享功能相关变量（若未定义需在全局/类内声明）
bool buttoClicked = false;
// 假设 ESPMenu 是已定义的结构体/类，包含 "是否开启共享" 成员；imei 是已定义的房间号字符串

// ---------------------- 开启绘制按钮（铺满整个绘制区域宽度）----------------------
float contentWidth = ImGui::GetContentRegionAvail().x;
float fullWidthButtonWidth = contentWidth; // 使用全部可用宽度

bool openSelected = (huitu == 0);
if (openSelected) {
    ImGui::PushStyleColor(ImGuiCol_Button, activeColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);
} else {
    ImGui::PushStyleColor(ImGuiCol_Button, inactiveColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, inactiveColor);
}

if (CustomButton("启用绘制", ImVec2(fullWidthButtonWidth, 120))) {
    DrawInit();
    
    // 在开启绘制时尝试初始化游戏进程
    if (InitializeGameProcess()) {
        huitu = 1;
        文字传入("绘制功 能已开启，游戏进程绑定成功");
    } else {
        huitu = 1; // 即使游戏进程未找到也允许开启绘制
        文字传入("绘制功能已开启，请启动游戏后重新点击开启绘制");
    }
}
ImGui::PopStyleColor(2);

// ---------------------- 新增：人物共享按钮（优化逻辑+适配布局）----------------------
ImGui::Spacing(); // 与上方按钮保持统一间距

bool shareSelected = ESPMenu.是否开启共享;
if (shareSelected) {
    ImGui::PushStyleColor(ImGuiCol_Button, activeColor);    // 开启时用激活色
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);
} else {
    ImGui::PushStyleColor(ImGuiCol_Button, inactiveColor);  // 关闭时用未激活色
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, inactiveColor);
}

if (CustomButton(
    ESPMenu.是否开启共享 ? "关闭人物共享" : "开启人物共享", 
    ImVec2(fullWidthButtonWidth, 120) // 宽度使用全宽度，高度匹配原有75px风格
)) {
    ESPMenu.是否开启共享 = !ESPMenu.是否开启共享; // 直接切换共享状态
    createSocket(); // 点击即创建Socket，无需通过 buttoClicked 中转
    文字传入(ESPMenu.是否开启共享 ? "人物共享已开启" : "人物共享已关闭");
}
ImGui::PopStyleColor(2);

// 显示房间号：紧跟共享按钮，左对齐保持整洁

// ---------------------- 新增：单透按钮 ----------------------
ImGui::Spacing(); // 与上方按钮保持统一间距

bool dantouSelected = 单透开启;
if (dantouSelected) {
    ImGui::PushStyleColor(ImGuiCol_Button, activeColor);    // 开启时用激活色
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);
} else {
    ImGui::PushStyleColor(ImGuiCol_Button, inactiveColor);  // 关闭时用未激活色
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, inactiveColor);
}

if (CustomButton(
    单透开启 ? "关闭单透" : "开启单透", 
    ImVec2(fullWidthButtonWidth, 120) // 宽度使用全宽度，高度匹配75px风格
)) {
    单透开启 = !单透开启; // 切换单透状态
    
    // 开启单透时自动关闭自瞄
    if (单透开启 && Aim.Start) {
        Aim.Start = false;
        文字传入("单透模式已开启，自瞄已自动关闭（请点击保存配置）");
    } else {
        文字传入(单透开启 ? "单透模式已开启" : "单透模式已关闭");
    }
    
    // 重新初始化触摸系统以应用新的只读模式设置
    重新初始化触摸系统();
    
    // 提供用户反馈
    if (单透开启) {
        printf("[单透模式] 已开启，触摸文件创建已禁用\n");
    } else {
        printf("[单透模式] 已关闭，触摸文件创建已启用\n");
    }
}
ImGui::PopStyleColor(2);

// ---------------------- 关闭绘制+退出按钮（同一排，平分宽度）----------------------
ImGui::Spacing();

// 计算每个按钮的宽度（平分全宽度，中间留间距）
float buttonSpacing = 10.0f; // 按钮间距
float halfButtonWidth = (fullWidthButtonWidth - buttonSpacing) / 2.0f; // 平分宽度

// 关闭绘制按钮
bool closeSelected = (huitu == 1);
if (closeSelected) {
    ImGui::PushStyleColor(ImGuiCol_Button, activeColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);
} else {
    ImGui::PushStyleColor(ImGuiCol_Button, inactiveColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, inactiveColor);
}

if (CustomButton("关闭绘制", ImVec2(halfButtonWidth, 120))) {
    huitu = 0;
    文字传入("绘制功能已关闭");
}
ImGui::PopStyleColor(2);

// 同一行放置退出按钮
ImGui::SameLine(0, buttonSpacing);

// 退出程序按钮
ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 0.6f));
ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.3f, 0.3f, 0.8f));

if (CustomButton("退出程序", ImVec2(halfButtonWidth, 120))) {
    文字传入("程序即将退出");
    // 停止代理线程并清理资源
    // printf("正在停止代理系统...\n");
    // stop_proxy_thread();  // 函数未定义，已注释
    
    // 清理ImGui资源
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();
    
    // 清理EGL资源
    if (display != EGL_NO_DISPLAY) {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (context != EGL_NO_CONTEXT) {
            eglDestroyContext(display, context);
        }
        if (surface != EGL_NO_SURFACE) {
            eglDestroySurface(display, surface);
        }
        eglTerminate(display);
    }
    
    // 释放窗口资源
    if (native_window) {
        ANativeWindow_release(native_window);
    }
    
    printf("防掉框已关闭，程序退出\n");
    
    // 使用更温和的退出方式
    fflush(stdout);
    fflush(stderr);
    _Exit(0);  // 使用_Exit避免atexit处理器但比_exit更标准
}
ImGui::PopStyleColor(2);

ImGui::Spacing();

ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.8f, 0.6f));
ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.7f, 0.9f, 0.8f));
if (CustomButton("保存配置", ImVec2(fullWidthButtonWidth, 120))) {
    SaveAllConfig();
    文字传入("配置已保存");
}
ImGui::PopStyleColor(2);
        
       
 
    // 添加帧率和性能信息显示（贝塞尔风格）
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    // 性能信息标题
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    ImGui::TextColored(ImVec4(50.0f/255.0f,150.0f/255.0f,255.0f/255.0f,255.0f/255.0f), "频道：@ GTQNHNB666");
    
    ImGui::Spacing();
    
    // 帧率信息（优化：每30帧更新一次）
    static char fpsText[128] = " 当前帧率: 60.0 FPS   渲染耗时: 16.667ms";
    static int fpsCounter = 0;
    if (fpsCounter++ % 30 == 0) {
        snprintf(fpsText, sizeof(fpsText), 
                " 当前帧率: %.1f FPS   渲染耗时: %.3fms", 
                ImGui::GetIO().Framerate, 
                ImGui::GetIO().DeltaTime * 1000.0f);
    }
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    ImGui::Text("%s", fpsText);
    
    // 设备信息（优化：使用静态缓冲区）
    static char deviceText[256];
    static bool deviceTextInitialized = false;
    if (!deviceTextInitialized) {
        snprintf(deviceText, sizeof(deviceText), 
                " 设备分辨率: %dx%d", screen_x, screen_y);
        deviceTextInitialized = true;
    }
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    ImGui::Text("%s", deviceText);
    ImGui::SameLine(); 
    ImGui::Text("房间号: %s ", imei);
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
   
    ImGui::PopStyleColor(3);
    
    ImGui::Spacing();

ImGui::Separator();
                        }
                        /* 
 * 由光头强内核（原XF内核）开源
 因频道账号被盗用后决定开源，正版为光头强内核，
开源文件仅供学习交流，任何个人或组织可在遵循协议条款的前提下，自由下载、使用、修改、分发本项目代码及衍生作品，无需额外授权。使用过程中请保留原项目版权声明及开源协议文本，不得用于违法违规场景。本项目以“现状”提供，开发者不承担任何直接或间接的使用风险及法律责任，亦不提供商业技术支持。
删除此内容死全家，没几把
* 注入时间: 2025-11-16 20:14:49
 * 联系方式: TG:@GTQNHNB666
QQ群：784316342
 */
                            break;
                        case 2: {
                            // 应用绘制设置区域的动画偏移
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (homepage - (-400.0f)));
                            
                            // 添加绘制设置区域标题
                            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); // 额外的左边距
                            ImGui::TextColored(ImVec4(50.0f/255.0f,150.0f/255.0f,255.0f/255.0f,255.0f/255.0f), "绘制设置");
                            ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
                            ImVec2 start = ImVec2(cursor_pos.x - 10, cursor_pos.y + ImGui::GetFrameHeight() / 2 - 30);
                            ImVec2 end = ImVec2(start.x + ImGui::GetContentRegionAvail().x, start.y);
                            ImGui::GetWindowDrawList()->AddLine(start, end, ImGui::GetColorU32(ImVec4(50.0f/255.0f,150.0f/255.0f,255.0f/255.0f,255.0f/255.0f)), 6.0f);
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);

                            // 第一行 - 3个功能（贝塞尔圆形风格，塞满整个宽度）
                            float availableWidth = ImGui::GetContentRegionAvail().x; // 使用全部可用宽度
                            
                            // 计算每个checkbox应该占用的宽度（平分整个宽度）
                            float checkboxAreaWidth = availableWidth / 3.0f; // 三等分
                            
                            // 计算实际的checkbox内容宽度
                            float checkbox1ContentWidth = 32 + ImGui::CalcTextSize("地图头像").x + 20;
                            float checkbox2ContentWidth = 32 + ImGui::CalcTextSize("人物方框").x + 20;
                            float checkbox3ContentWidth = 32 + ImGui::CalcTextSize("方框技能").x + 20;
                            
                            // 计算每个checkbox在其区域内的居中偏移
                            float offset1 = (checkboxAreaWidth - checkbox1ContentWidth) / 2.0f;
                            float offset2 = (checkboxAreaWidth - checkbox2ContentWidth) / 2.0f;
                            float offset3 = (checkboxAreaWidth - checkbox3ContentWidth) / 2.0f;

                            // 第一个checkbox - 在第一个区域内居中
                            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset1);
                            CustomCheckbox("地图头像", &地图, ImVec2(140, 45));
                            if (ImGui::IsItemClicked()) {
                                std::string value1 = 地图 ? "1" : "0"; 
                                WriteConfigToIni("/data/光头强配置/配置.ini", "地图头像", value1);
                                文字传入(地图 ? "地图头像已开启" : "地图头像已关闭");
                            }
                            // 第二个checkbox - 移动到第二个区域并居中
                            ImGui::SameLine();
                            ImGui::SetCursorPosX(checkboxAreaWidth + offset2);
                            CustomCheckbox("人物方框", &方框, ImVec2(140, 45));
                            if (ImGui::IsItemClicked()) {
                                std::string value2 = 方框 ? "1" : "0"; 
                                WriteConfigToIni("/data/光头强配置/配置.ini", "人物方框", value2);
                                文字传入(方框 ? "人物方框已开启" : "人物方框已关闭");
                            }
                            // 第三个checkbox - 移动到第三个区域并居中
                            ImGui::SameLine();
                            ImGui::SetCursorPosX(checkboxAreaWidth * 2 + offset3);
                            CustomCheckbox("方框技能", &方框技能, ImVec2(140, 45));
                            if (ImGui::IsItemClicked()) {
                                std::string value3 = 方框技能 ? "1" : "0"; 
                                WriteConfigToIni("/data/光头强配置/配置.ini", "方框内技能计时", value3);
                                文字传入(方框技能 ? "方框技能已开启" : "方框技能已关闭");
                            }

                            ImGui::Spacing(); // 增加垂直间距
                            ImGui::Spacing(); // 再增加一行间距
                            ImGui::Spacing(); // 再增加一行间距

                            // 第二行 - 3个功能（贝塞尔圆形风格，塞满整个宽度）
                            // 计算第二行checkbox的内容宽度和偏移
                            float checkbox4ContentWidth = 32 + ImGui::CalcTextSize("技能计时").x + 20;
                            float checkbox5ContentWidth = 32 + ImGui::CalcTextSize("兵线绘制").x + 20;
                            float checkbox6ContentWidth = 32 + ImGui::CalcTextSize("野怪计时").x + 20;
                            
                            float offset4 = (checkboxAreaWidth - checkbox4ContentWidth) / 2.0f;
                            float offset5 = (checkboxAreaWidth - checkbox5ContentWidth) / 2.0f;
                            float offset6 = (checkboxAreaWidth - checkbox6ContentWidth) / 2.0f;
                            
                            // 第一个checkbox - 在第一个区域内居中
                            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset4);
                            CustomCheckbox("技能计时", &顶上技能2, ImVec2(140, 45));
                            if (ImGui::IsItemClicked()) {
                                std::string value4 = 顶上技能2 ? "1" : "0"; 
                                WriteConfigToIni("/data/光头强配置/配置.ini", "技能计时面板", value4);
                                文字传入(顶上技能2 ? "技能计时已开启" : "技能计时已关闭");
                            }
                            // 第二个checkbox - 移动到第二个区域并居中
                            ImGui::SameLine();
                            ImGui::SetCursorPosX(checkboxAreaWidth + offset5);
                            CustomCheckbox("兵线绘制", &兵线, ImVec2(140, 45));
                            if (ImGui::IsItemClicked()) {
                                std::string value = 兵线 ? "1" : "0"; 
                                WriteConfigToIni("/data/光头强配置/配置.ini", "兵线绘制", value);
                                文字传入(兵线 ? "兵线绘制已开启" : "兵线绘制已关闭");
                            }
                            // 第三个checkbox - 移动到第三个区域并居中
                            ImGui::SameLine();
                            ImGui::SetCursorPosX(checkboxAreaWidth * 2 + offset6);
                            CustomCheckbox("野怪计时", &野怪, ImVec2(140, 45));
                            if (ImGui::IsItemClicked()) {
                                std::string value6 = 野怪 ? "1" : "0"; 
                                WriteConfigToIni("/data/光头强配置/配置.ini", "野怪计时", value6);
                                文字传入(野怪 ? "野怪计时已开启" : "野怪计时已关闭");
                            }

                            ImGui::Spacing(); // 增加垂直间距
                            ImGui::Spacing(); // 再增加一行间距
                            ImGui::Spacing(); // 再增加一行间距

                            // 第三行 - 剩余功能（贝塞尔圆形风格，塞满整个宽度）
                            // 计算第三行checkbox的内容宽度和偏移
                            float checkbox7ContentWidth = 32 + ImGui::CalcTextSize("人物射线").x + 20;
                            float checkbox8ContentWidth = 32 + ImGui::CalcTextSize("自身视野").x + 20;
                            float checkbox9ContentWidth = 32 + ImGui::CalcTextSize("兵线实体").x + 20;
                            
                            float offset7 = (checkboxAreaWidth - checkbox7ContentWidth) / 2.0f;
                            float offset8 = (checkboxAreaWidth - checkbox8ContentWidth) / 2.0f;
                            float offset9 = (checkboxAreaWidth - checkbox9ContentWidth) / 2.0f;
                            
                            // 第一个checkbox - 在第一个区域内居中
                            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset7);
                            CustomCheckbox("人物射线", &射线, ImVec2(140, 45));
                            if (ImGui::IsItemClicked()) {
                                std::string value7 = 射线 ? "1" : "0"; 
                                WriteConfigToIni("/data/光头强配置/配置.ini", "射线开关", value7);
                                文字传入(射线 ? "人物射线已开启" : "人物射线已关闭");
                            }
                            // 第二个checkbox - 移动到第二个区域并居中
                            ImGui::SameLine();
                            ImGui::SetCursorPosX(checkboxAreaWidth + offset8);
                            CustomCheckbox("自身视野", &自身, ImVec2(140, 45));
                            if (ImGui::IsItemClicked()) {
                                std::string value8 = 自身 ? "1" : "0"; 
                                WriteConfigToIni("/data/光头强配置/配置.ini", "自身视野判断", value8);
                                文字传入(自身 ? "自身视野已开启" : "自身视野已关闭");
                            }
                            // 第三个checkbox - 移动到第三个区域并居中
                            ImGui::SameLine();
                            ImGui::SetCursorPosX(checkboxAreaWidth * 2 + offset9);
                            CustomCheckbox("兵线实体", &实体兵线, ImVec2(140, 45));
                            if (ImGui::IsItemClicked()) {
                                std::string value9 = 实体兵线 ? "1" : "0"; 
                                WriteConfigToIni("/data/光头强配置/配置.ini", "兵线实体", value9);
                                文字传入(实体兵线 ? "兵线实体已开启" : "兵线实体已关闭");
                            }

                            ImGui::Spacing(); // 增加垂直间距
                            ImGui::Spacing(); // 再增加一行间距
                            ImGui::Spacing(); // 再增加一行间距

                            // 血量相关功能一行显示（三等分布局）
                            float checkboxAreaWidth2 = availableWidth / 3.0f; // 三等分
                            
                            // 计算每个checkbox的内容宽度和偏移
                            float checkboxBloodContentWidth = 32 + ImGui::CalcTextSize("血量显示").x + 20;
                            float checkboxSkillContentWidth = 32 + ImGui::CalcTextSize("实体技能").x + 20;
                            float checkboxWildContentWidth = 32 + ImGui::CalcTextSize("野怪血量").x + 20;
                            
                            float offsetBlood = (checkboxAreaWidth2 - checkboxBloodContentWidth) / 2.0f;
                            float offsetSkill = (checkboxAreaWidth2 - checkboxSkillContentWidth) / 2.0f;
                            float offsetWild = (checkboxAreaWidth2 - checkboxWildContentWidth) / 2.0f;

                            // 第一个checkbox - 血量显示
                            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetBlood);
                            CustomCheckbox("血量显示", &血量, ImVec2(140, 45));
                            if (ImGui::IsItemClicked()) {
                                std::string value10 = 血量 ? "1" : "0"; 
                                WriteConfigToIni("/data/光头强配置/配置.ini", "血量", value10);
                                文字传入(血量 ? "血量显示已开启" : "血量显示已关闭");
                            }
                            
                            // 第二个checkbox - 实体技能
                            ImGui::SameLine();
                            ImGui::SetCursorPosX(checkboxAreaWidth2 + offsetSkill);
                            CustomCheckbox("实体技能", &实体技能, ImVec2(140, 45));
                            if (ImGui::IsItemClicked()) {
                                std::string value11 = 实体技能 ? "1" : "0"; 
                                WriteConfigToIni("/data/光头强配置/配置.ini", "实体技能显示", value11);
                                文字传入(实体技能 ? "实体技能已开启" : "实体技能已关闭");
                            }
                            
                            // 第三个checkbox - 野怪血量
                            ImGui::SameLine();
                            ImGui::SetCursorPosX(checkboxAreaWidth2 * 2 + offsetWild);
                            CustomCheckbox("野怪血量", &野血, ImVec2(140, 45));
                            if (ImGui::IsItemClicked()) {
                                std::string value12 = 野血 ? "1" : "0"; 
                                WriteConfigToIni("/data/光头强配置/配置.ini", "野怪血量显示", value12);
                                文字传入(野血 ? "野怪血量已开启" : "野怪血量已关闭");
                            }

                            ImGui::Separator();
                            
                            // 暴露不绘功能区域（二等分布局）
                            float checkboxAreaWidth3 = availableWidth / 2.0f; // 二等分
                            
                            // 计算每个checkbox的内容宽度和偏移
                            float checkboxMap3ContentWidth = 32 + ImGui::CalcTextSize("暴露不绘小地图").x + 20;
                            float checkboxBox3ContentWidth = 32 + ImGui::CalcTextSize("暴露视野不绘方框").x + 20;
                            
                            float offsetMap3 = (checkboxAreaWidth3 - checkboxMap3ContentWidth) / 2.0f;
                            float offsetBox3 = (checkboxAreaWidth3 - checkboxBox3ContentWidth) / 2.0f;
                            
                            // 第一个checkbox - 暴露不绘小地图
                            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetMap3);
                            CustomCheckbox("暴露不绘小地图", &暴露不绘小地图头像, ImVec2(160, 45));
                            if (ImGui::IsItemClicked()) {
                                std::string value = 暴露不绘小地图头像 ? "1" : "0";
                                WriteConfigToIni("/data/光头强配置/配置.ini", "暴露不绘小地图头像", value);
                                文字传入(暴露不绘小地图头像 ? "暴露不绘小地图已开启" : "暴露不绘小地图已关闭");
                            }
                            
                            // 第二个checkbox - 暴露视野不绘方框
                            ImGui::SameLine();
                            ImGui::SetCursorPosX(checkboxAreaWidth3 + offsetBox3);
                            CustomCheckbox("暴露视野不绘方框", &暴露视野不绘, ImVec2(160, 45));
                            if (ImGui::IsItemClicked()) {
                                std::string value = 暴露视野不绘 ? "1" : "0";
                                WriteConfigToIni("/data/光头强配置/配置.ini", "暴露视野不绘", value);
                                文字传入(暴露视野不绘 ? "暴露视野不绘方框已开启" : "暴露视野不绘方框已关闭");
                            }

                            ImGui::Separator();
                        }
                            break;
                        case 3: {
                            // 应用设置区域的动画偏移
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (homepage - (-800.0f)));
                            
                            // 添加设置区域标题和图标
                            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); // 额外的左边距
                            
                           
                            ImVec2 pos = ImGui::GetWindowPos();
                            ImVec2 spacing = ImVec2(10, 10); // 间距
                            if (设置图标纹理.textureId) {
                                ImGui::GetWindowDrawList()->AddImage(
                                    (ImTextureID)设置图标纹理.textureId,
                                    pos + ImVec2(spacing.x + 15, spacing.y + 10),      // 起始位置
                                    pos + ImVec2(spacing.x + 55, spacing.y + 50),      // 结束位置 (40x40图标)
                                    ImVec2(0, 0), ImVec2(1, 1),                        // UV坐标
                                    ImGui::GetColorU32(ImVec4(50.0f/255.0f,150.0f/255.0f,255.0f/255.0f,255.0f/255.0f))
                                );
                            }
                            
                          
                            ImGui::GetWindowDrawList()->AddText(
                                ImGui::GetFont(), 20.0f, 
                                pos + ImVec2(70, 15), // 文字位置（图标右边）
                                ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), // 白色文字
                                "基础设置功能"
                            );
                            ImGui::GetWindowDrawList()->AddText(
                                ImGui::GetFont(), 12.0f, 
                                pos + ImVec2(70, 35), // 副标题位置
                                ImGui::GetColorU32(ImVec4(50.0f/255.0f,150.0f/255.0f,255.0f/255.0f,255.0f/255.0f)), // 蓝色副标题
                                "系统配置和界面设置"
                            );
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 60); // 调整光标位置
                            
                                      ImGui::BeginChild("##设置a", ImVec2(-1, 50), false
                            );
                            {
for (int i = 0; i < tabCount; ++i) {
        if (i == currentSelectedTabIndex) {
            // 如果是当前选中的标签，则设置为激活颜色
            ImGui::PushStyleColor(ImGuiCol_Button, activeColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);
        } else {
            // 否则设置为非激活颜色（透明）
            ImGui::PushStyleColor(ImGuiCol_Button, inactiveColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, inactiveColor);
        }

        if (CustomButton(tabLabels[i], ImVec2(120, 50))) {
            OnTabChange(i);
            // 优化：避免创建临时 std::string 对象
            static char tabMessage[128];
            snprintf(tabMessage, sizeof(tabMessage), "已切换到 %s", tabLabels[i]);
            文字传入(tabMessage);
        }

        ImGui::PopStyleColor(2); // 恢复默认样式
        if (i < tabCount - 1) {
            ImGui::SameLine();
        }
    }
                            }
                            ImGui::EndChild();
                           // ImGui::SameLine();
                           ImGui::Separator();//横线分割
                            ImGui::BeginChild("##设置a-content", ImVec2(-1, -1), false, 
                                              ImGuiWindowFlags_HorizontalScrollbar);
                            {
                                // 声明偏移变量在switch外面
                                float offsetX = 0.0f;
                                float currentY = 0.0f;
                                
                                switch (MenuTab2) {
                                    case 1: {
                                        // 应用绘制调节的横向偏移（使用子窗口，支持负偏移）
                                        offsetX = settingPage;
                                        currentY = ImGui::GetCursorPosY();
                                        ImGui::SetCursorPos(ImVec2(offsetX + 20, currentY));
                                        ImGui::BeginChild("##绘制调节内容", ImVec2(600, -1), false);
                                        
                                        CustomCheckbox("辅助框", &辅助方框, ImVec2(140, 45));
                                        if (ImGui::IsItemClicked()) {
                                            文字传入(辅助方框 ? "辅助框已开启" : "辅助框已关闭");
                                        }
                                   
                                     //   ImGui::Separator();

                 // 优化：使用静态缓冲区，松手后再保存
                 static char configBuffer[64];
                 
                 CustomSliderInt("头像左右", &SmallMapX, -200, 200, "%d", 2);
                 if (ImGui::IsItemDeactivatedAfterEdit()) {  // 松手后才保存
                     sprintf(configBuffer, "%d", SmallMapX);
                     WriteConfigToIni("/data/光头强配置/头像左右.ini", "头像左右", configBuffer);
                 }

                 CustomSliderInt("头像上下", &SmallMapY, -200, 200, "%d", 2);
                 if (ImGui::IsItemDeactivatedAfterEdit()) {
                     sprintf(configBuffer, "%d", SmallMapY);
                     WriteConfigToIni("/data/光头强配置/头像上下.ini", "头像上下", configBuffer);
                 }

                 CustomSliderInt("头像大小", &touxiangdaxiao, -300, 300, "%d", 2);
                 if (ImGui::IsItemDeactivatedAfterEdit()) {
                     sprintf(configBuffer, "%d", touxiangdaxiao);
                     WriteConfigToIni("/data/光头强配置/头像大小.ini", "头像大小", configBuffer);
                 }

                 CustomSliderInt("血量大小", &xiaodituxue, -80, 80, "%d", 2);
                 if (ImGui::IsItemDeactivatedAfterEdit()) {
                     sprintf(configBuffer, "%d", xiaodituxue);
                     WriteConfigToIni("/data/光头强配置/血量大小.ini", "血量大小", configBuffer);
                 }

                 CustomSliderInt("地图缩放比例", &jiange, -100, 100, "%d", 2);
                 if (ImGui::IsItemDeactivatedAfterEdit()) {
                     sprintf(configBuffer, "%d", jiange);
                     WriteConfigToIni("/data/光头强配置/地图比例.ini", "地图缩放比例", configBuffer);
                 }

                 CustomSliderInt("实体左右", &SmallHPX, -300, 300, "%d", 2);
                 if (ImGui::IsItemDeactivatedAfterEdit()) {
                     sprintf(configBuffer, "%d", SmallHPX);
                     WriteConfigToIni("/data/光头强配置/实体左右.ini", "实体左右", configBuffer);
                 }

                 CustomSliderInt("实体上下", &SmallHPY, -300, 300, "%d", 2);
                 if (ImGui::IsItemDeactivatedAfterEdit()) {
                     sprintf(configBuffer, "%d", SmallHPY);
                     WriteConfigToIni("/data/光头强配置/实体上下.ini", "实体上下", configBuffer);
                 }

                 CustomSliderInt("技能计时左右", &jinenglanzX, -2000, 2000, "%d", 2);
                 if (ImGui::IsItemDeactivatedAfterEdit()) {
                     sprintf(configBuffer, "%d", jinenglanzX);
                     WriteConfigToIni("/data/光头强配置/技能左右.ini", "技能计时左右", configBuffer);
                 }

                 CustomSliderInt("技能计时上下", &jinenglanzY, -500, 500, "%d", 2);
                 if (ImGui::IsItemDeactivatedAfterEdit()) {
                     sprintf(configBuffer, "%d", jinenglanzY);
                     WriteConfigToIni("/data/光头强配置/技能上下.ini", "技能计时上下", configBuffer);
                 }

                 CustomSliderInt("自身视野左右", &屏幕x, -100, 100, "%d");
                 if (ImGui::IsItemDeactivatedAfterEdit()) {
                     sprintf(configBuffer, "%d", 屏幕x);
                     WriteConfigToIni("/data/光头强配置/视野左右.ini", "自身视野左右", configBuffer);
                 }

                 CustomSliderInt("自身视野上下", &屏幕y, -200, 200, "%d");
                 if (ImGui::IsItemDeactivatedAfterEdit()) {
                     sprintf(configBuffer, "%d", 屏幕y);
                     WriteConfigToIni("/data/光头强配置/视野上下.ini", "自身视野上下", configBuffer);
                 }

                                        ImGui::EndChild(); // 关闭绘制调节内容子窗口
                                        break;


                                    case 2: {
                                        // 应用显示设置的横向偏移（使用子窗口，支持负偏移）
                                        offsetX = settingPage - (-600.0f);
                                        currentY = ImGui::GetCursorPosY();
                                        ImGui::SetCursorPos(ImVec2(offsetX + 20, currentY));
                                        ImGui::BeginChild("##显示设置内容", ImVec2(600, -1), false);

                 CustomSliderFloat("帧率调节", &FPS, 10.0f, 144.0f, "%.0f");
                 if (ImGui::IsItemDeactivatedAfterEdit()) {  // 优化：松手后才设置
                     float fps = FPS * 2.3f;
                     sprintf(configBuffer, "%d", FPS);
                     WriteConfigToIni("/data/光头强配置/帧率.ini", "帧率数值", configBuffer);
                     FPS限制.SetFps(fps);
                 }


                                        //ImGui::Checkbox("10v10", &十人);

                                        ImGui::EndChild(); // 关闭显示设置内容子窗口

                                    }
                                        break;





                                    case 3: {
                                        // 应用颜色设置的横向偏移（使用子窗口，支持负偏移）
                                        offsetX = settingPage - (-1200.0f);
                                        currentY = ImGui::GetCursorPosY();
                                        ImGui::SetCursorPos(ImVec2(offsetX + 20, currentY));
                                        ImGui::BeginChild("##颜色设置内容", ImVec2(600, -1), false);
                                        
                                        ImGui::ColorEdit3("血量颜色", (float *) &颜色配置.血量颜色);
                                        ImGui::ColorEdit3("无视野血量",
                                                          (float *) &颜色配置.无血量颜色);
                                        ImGui::ColorEdit3("射线颜色", (float *) &颜色配置.射线颜色);
                                        ImGui::ColorEdit3("方框颜色", (float *) &颜色配置.方框颜色);
                                        ImGui::Separator();
                                        if (CustomButton("保存配置", ImVec2(315, 65))) {
                                            string SaveFile;
                                            SaveFile = "/data/光头强配置/颜色配置.ini";
                                            pdys = SaveDrawSet((总颜色 *) &颜色配置,
                                                               SaveFile.c_str());
                                            文字传入("颜色配置已保存");
                                 
                                        }
                               
                                        
                                        if (pdys == 1) {
                                            if (CustomButton("重置配置", ImVec2(315, 65))) {
                                                remove("/data/光头强配置/颜色配置.ini");
                                                pdys = 0;
                                                string LoadFile = "/data/光头强配置/颜色配置.ini";
                                                pdys = ReadDrawSet((总颜色 *) &颜色配置,
                                                                   LoadFile.c_str());
                                                文字传入("颜色配置已重置");
                                                
                                            }
                                        }

                                        ImGui::EndChild(); // 关闭颜色设置内容子窗口

                                    }
                                        break;
                                 case 4: {
                                    // 应用共享设置的横向偏移（使用子窗口，支持负偏移）
                                    offsetX = settingPage - (-1800.0f);
                                    currentY = ImGui::GetCursorPosY();
                                    ImGui::SetCursorPos(ImVec2(offsetX + 20, currentY));
                                    ImGui::BeginChild("##共享设置内容", ImVec2(600, -1), false);
                                    
                 CustomSliderInt("共享左右", &ESPMenu.小地图左右调整,-150,300);
                 if (ImGui::IsItemEdited()){char urlls[2560];
                 sprintf(urlls, "%d", ESPMenu.小地图左右调整);
                 WriteConfigToIni("/data/光头强配置/共享配置.ini", "共享左右", urlls);}
        
          	   CustomSliderInt("共享上下", &ESPMenu.小地图上下调整,-200,200);
          	   if (ImGui::IsItemEdited()){char urlls[2560];
                 sprintf(urlls, "%d", ESPMenu.小地图上下调整);
                 WriteConfigToIni("/data/光头强配置/共享配置.ini", "共享上下", urlls);}
        
               CustomSliderInt("共享野怪左右", &ESPMenu.野怪左右调整,-200,200);
               if (ImGui::IsItemEdited()){char urlls[2560];
                 sprintf(urlls, "%d", ESPMenu.野怪左右调整);
                 WriteConfigToIni("/data/光头强配置/共享配置.ini", "共享野怪左右", urlls);}
        
               CustomSliderInt("共享野怪上下", &ESPMenu.野怪上下调整,-200,200);
               if (ImGui::IsItemEdited()){char urlls[2560];
                  sprintf(urlls, "%d", ESPMenu.野怪上下调整);
                  WriteConfigToIni("/data/光头强配置/共享配置.ini", "共享野怪上下", urlls);}
        
                CustomSliderInt("共享兵线左右", &ESPMenu.兵线左右调整,-200,200);
                if (ImGui::IsItemEdited()){char urlls[2560];
                 sprintf(urlls, "%d", ESPMenu.兵线左右调整);
                 WriteConfigToIni("/data/光头强配置/共享配置.ini", "共享兵线左右", urlls);}
        
               CustomSliderInt("共享兵线上下", &ESPMenu.兵线上下调整,-200,200);
               if (ImGui::IsItemEdited()){char urlls[2560];
                 sprintf(urlls, "%d", ESPMenu.兵线上下调整);
                 WriteConfigToIni("/data/光头强配置/共享配置.ini", "共享兵线上下", urlls);}
        
               CustomSliderInt("共享分辨率x", &ESPMenu.分辨率X,1000,4000);
               if (ImGui::IsItemEdited()){char urlls[2560];
                 sprintf(urlls, "%d", ESPMenu.分辨率X);
                 WriteConfigToIni("/data/光头强配置/共享配置.ini", "共享分辨率x", urlls);}
        
               CustomSliderInt("共享分辨率y", &ESPMenu.分辨率Y,1000,4000);
               if (ImGui::IsItemEdited()){char urlls[2560];
                 sprintf(urlls, "%d", ESPMenu.分辨率Y);
                 WriteConfigToIni("/data/光头强配置/共享配置.ini", "共享分辨率y", urlls);}
               
                                 ImGui::EndChild(); // 关闭共享设置内容子窗口
                                 }
                                 break;
                                }
                                // 结束MenuTab2的switch
                            }
                            // 结束BeginChild("##设置a-content")的if块
                            }
                            ImGui::EndChild();  // 结束##设置a-content
                        }  // 结束case 3
                            break;
                        case 4: {
                            // 应用触摸区域的动画偏移
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (homepage - (-1200.0f)));
         
            if (触摸驱动) {
              绘制触摸 = true;
              
              if (ImGui::BeginChild("##触摸功能", ImVec2(0, 0), false, ImGuiWindowFlags_NavFlattened))
              {
                  ImGui::Text("触摸功能");
                  ImGui::Separator();
          /*    ImGui::Text("召唤师: %s ", 召唤师.c_str());
              ImGui::SameLine();
              ImGui::Text("技能: %s", 召唤师技能.c_str());
              // ImGui::Separator();*/
              // 触摸功能checkbox（贝塞尔圆形风格）
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
              CustomCheckbox("自动斩杀", &自动斩杀, ImVec2(140, 45));
              if (ImGui::IsItemClicked()) {
                文字传入(自动斩杀 ? "自动斩杀已开启" : "自动斩杀已关闭");
              }
              if (自动斩杀) {
                自动惩戒 = false;
                自动净化 = false;
              }
              
              ImGui::Spacing();
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
             if(版本==1){
              CustomCheckbox("自动惩戒", &自动惩戒, ImVec2(140, 45));
              }else{
              CustomCheckbox("自动惩戒", &自动惩戒, ImVec2(140, 45)); 
              }
              if (ImGui::IsItemClicked()) {
                文字传入(自动惩戒 ? "自动惩戒已开启" : "自动惩戒已关闭");
              }
              
              if (自动惩戒) {
                自动斩杀 = false;
                自动净化 = false;
              }
              
              ImGui::Spacing();
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
              CustomCheckbox("自动解控", &自动净化, ImVec2(140, 45));
              if (ImGui::IsItemClicked()) {
                文字传入(自动净化 ? "自动解控已开启" : "自动解控已关闭");
              }
              if (自动净化) {
                自动斩杀 = false;
                自动惩戒 = false;
                ImGui::Separator();
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20); // 子选项增加更多缩进
                CustomCheckbox("附近有人触发解控", &有人解控, ImVec2(170, 45));
                if (ImGui::IsItemClicked()) {
                  文字传入(有人解控 ? "附近有人触发解控已开启" : "附近有人触发解控已关闭");
                }
              }
              
              ImGui::ItemSize(ImVec2(0, 1));
              ImGui::Separator();
              if (自动惩戒) {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20); // 子选项增加更多缩进
                CustomCheckbox("附近有人时惩戒", &智能不惩, ImVec2(170, 45));
                if (ImGui::IsItemClicked()) {
                  文字传入(智能不惩 ? "附近有人时惩戒已开启" : "附近有人时惩戒已关闭");
                }
                ImGui::Separator();
              }
              if (自动斩杀) {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20); // 子选项增加更多缩进
                CustomCheckbox("自动计算斩杀线", &智能斩杀, ImVec2(170, 45));
                if (ImGui::IsItemClicked()) {
                  文字传入(智能斩杀 ? "自动计算斩杀线已开启" : "自动计算斩杀线已关闭");
                }
                            if (!智能斩杀) {
                SliderFloat("斩杀百分比", &zhansha, 10.0f, 20.0f, "%.1f");
                if (ImGui::IsItemEdited()) {
                   char urlls[2560];
    sprintf(urlls, "%.1f", zhansha);
    WriteConfigToIni("/data/光头强配置/配置.ini", "斩杀值", urlls);
                }
              }
                ImGui::Separator();
              }
         
              ImGui::ItemSize(ImVec2(0, 1));
       
          /*    if (自动净化) {
              ImGui::Separator();
              ImGui::Columns(2, "CheckboxColumns", false); // 创建两列
              ImGui::RadioButton("净化", &jk, 0);
             ImGui::RadioButton("二技能", &jk, 2);
              ImGui::NextColumn();
              ImGui::RadioButton("一技能", &jk, 1);
              ImGui::RadioButton("三技能", &jk, 3);
                ImGui::Separator();
             
              ImGui::Columns(1); // 重置列回单列
              ImGui::Separator();
              }*/
              /*if(ImGui::SliderInt("惩戒值", &chengjie, 0, 5000, "%.0f", 2)) {
                  char urlls[2560];
                                          sprintf(urlls, "%d", chengjie);
                                          写出文件("/data/嫣/惩戒值",urlls);
                                  }*/
              CustomSliderInt("触摸位置左右", &zhsX, 0, yxpx, "%d", 2);
              if (ImGui::IsItemEdited()) {
         char urlls[2560];
    sprintf(urlls, "%d", zhsX);
    WriteConfigToIni("/data/光头强配置/触摸左右.ini", "触摸位置左右", urlls);
              }
              
              CustomSliderInt("触摸位置上下", &zhsY, 0, yxpy, "%d", 2);
              if (ImGui::IsItemEdited()) { char urlls[2560];
    sprintf(urlls, "%d", zhsY);
    WriteConfigToIni("/data/光头强配置/触摸上下.ini", "触摸位置上下", urlls);
              }
              
              // ==================== 自瞄功能配置区域 ====================
              ImGui::Separator();
              ImGui::Spacing();
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
              ImGui::TextColored(ImVec4(0.2f, 0.8f, 1.0f, 1.0f), "自瞄功能");
              ImGui::Separator();
              
              // 基础开关区域（第一行：3个）
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
              CustomCheckbox("自瞄开关", &Aim.Start, ImVec2(140, 45));
              if (ImGui::IsItemClicked()) {
                  文字传入(Aim.Start ? "自瞄功能已开启" : "自瞄功能已关闭");
              }
              
              ImGui::SameLine();
              CustomCheckbox("显示位置", &Aim.Pos, ImVec2(140, 45));
              if (ImGui::IsItemClicked()) {
                  文字传入(Aim.Pos ? "位置显示已开启" : "位置显示已关闭");
              }
              
              ImGui::SameLine();
              CustomCheckbox("显示目标", &Aim.Showtarget, ImVec2(140, 45));
              if (ImGui::IsItemClicked()) {
                  文字传入(Aim.Showtarget ? "目标显示已开启" : "目标显示已关闭");
              }
              
              ImGui::Spacing();
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
              
              // 基础开关区域（第二行：3个）
              CustomCheckbox("视野检测", &Aim.vision, ImVec2(140, 45));
              if (ImGui::IsItemClicked()) {
                  文字传入(Aim.vision ? "视野检测已开启" : "视野检测已关闭");
              }
              
              ImGui::SameLine();
              CustomCheckbox("范围检测", &Aim.scope, ImVec2(140, 45));
              if (ImGui::IsItemClicked()) {
                  文字传入(Aim.scope ? "范围检测已开启" : "范围检测已关闭");
              }
              
              ImGui::SameLine();
              CustomCheckbox("圆形显示", &Aim.Circle, ImVec2(140, 45));
              if (ImGui::IsItemClicked()) {
                  文字传入(Aim.Circle ? "圆形显示已开启" : "圆形显示已关闭");
              }
              
              ImGui::Spacing();
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
              
              // 基础开关区域（第三行：3个）
              CustomCheckbox("触发模式", &Aim.trigger, ImVec2(140, 45));
              if (ImGui::IsItemClicked()) {
                  文字传入(Aim.trigger ? "触发模式已开启" : "触发模式已关闭");
              }
              
              ImGui::SameLine();
              CustomCheckbox("左端口", &Aim.Leftport, ImVec2(140, 45));
              if (ImGui::IsItemClicked()) {
                  文字传入(Aim.Leftport ? "左端口已开启" : "左端口已关闭");
              }
              
              ImGui::SameLine();
              CustomCheckbox("启用", &Aim.Enable, ImVec2(140, 45));
              if (ImGui::IsItemClicked()) {
                  文字传入(Aim.Enable ? "已启用" : "已禁用");
              }
              
              ImGui::Separator();
              ImGui::Spacing();
              
              // 模式选择区域
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
              ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.2f, 1.0f), "模式设置");
              ImGui::Spacing();
              
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
              CustomSliderInt("自瞄模式", &Aim.mode, 0, 5, "模式 %d");
              if (ImGui::IsItemEdited()) {
                  文字传入("自瞄模式已更改");
              }
              
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
              CustomSliderInt("选择模式", &Aim.select_mode, 0, 3, "选择 %d");
              if (ImGui::IsItemEdited()) {
                  文字传入("选择模式已更改");
              }
              
              ImGui::Separator();
              ImGui::Spacing();
              
              // 基础参数区域
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
              ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.8f, 1.0f), "基础参数");
              ImGui::Spacing();
              
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
              SliderFloat("预测速度", &Aim.Predictionsed, 0.1f, 10.0f, "%.2f");
              if (ImGui::IsItemEdited()) {
                  文字传入("预测速度已调整");
              }
              
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
              SliderFloat("角度", &Aim.angle, 0.0f, 360.0f, "%.1f°");
              if (ImGui::IsItemEdited()) {
                  文字传入("角度已调整");
              }
              
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
              SliderFloat("距离", &Aim.distance, 0.0f, 5000.0f, "%.1f");
              if (ImGui::IsItemEdited()) {
                  文字传入("距离已调整");
              }
              
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
              SliderFloat("技能大小", &Aim.Skillsize, 0.0f, 500.0f, "%.1f");
              if (ImGui::IsItemEdited()) {
                  文字传入("技能大小已调整");
              }
              
              ImGui::Separator();
              ImGui::Spacing();
              
              // 触发位置参数
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
              ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.2f, 1.0f), "触发位置");
              ImGui::Spacing();
              
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
              CustomSliderInt("触发X坐标", (int*)&Aim.TriggerpositionX, 0, 4000, "X: %d");
              if (ImGui::IsItemEdited()) {
                  文字传入("触发X坐标已调整");
              }
              
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
              CustomSliderInt("触发Y坐标", (int*)&Aim.TriggerpositionY, 0, 2000, "Y: %d");
              if (ImGui::IsItemEdited()) {
                  文字传入("触发Y坐标已调整");
              }
              
              ImGui::Separator();
              ImGui::Spacing();
              
              // 高级参数区域
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
              ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.8f, 1.0f), "高级参数");
              ImGui::Spacing();
              
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
              SliderFloat("平滑度", &Aim.Smoothness, 0.0f, 1.0f, "%.2f");
              if (ImGui::IsItemEdited()) {
                  文字传入("平滑度已调整");
              }
              
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
              CustomSliderInt("子弹速度", (int*)&Aim.Bulletspeed, 0, 50000, "速度: %d");
              if (ImGui::IsItemEdited()) {
                  文字传入("子弹速度已调整");
              }
              
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
              SliderFloat("响应速度", &Aim.responseSpeed, 0.0f, 1.0f, "%.2f");
              if (ImGui::IsItemEdited()) {
                  文字传入("响应速度已调整");
              }
              
              ImGui::Separator();
              ImGui::Spacing();
              
              // 性能优化区域
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
              ImGui::TextColored(ImVec4(0.6f, 0.3f, 1.0f, 1.0f), "性能优化");
              ImGui::Spacing();
              
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
              CustomSliderInt("更新间隔(ms)", &Aim.updateInterval, 1, 100, "%d ms");
              if (ImGui::IsItemEdited()) {
                  文字传入("更新间隔已调整");
              }
              
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
              CustomSliderInt("最小触发间隔(ms)", &Aim.minTriggerInterval, 10, 500, "%d ms");
              if (ImGui::IsItemEdited()) {
                  文字传入("最小触发间隔已调整");
              }
              
              ImGui::Separator();
              ImGui::Spacing();
              
              // 高级选项开关
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
              ImGui::TextColored(ImVec4(0.8f, 0.4f, 0.2f, 1.0f), "高级选项");
              ImGui::Spacing();
              
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
              CustomCheckbox("预测调试", &Aim.debugPredict, ImVec2(140, 45));
              if (ImGui::IsItemClicked()) {
                  文字传入(Aim.debugPredict ? "预测调试已开启" : "预测调试已关闭");
              }
              
              ImGui::SameLine();
              CustomCheckbox("自适应平滑", &Aim.adaptiveSmoothing, ImVec2(140, 45));
              if (ImGui::IsItemClicked()) {
                  文字传入(Aim.adaptiveSmoothing ? "自适应平滑已开启" : "自适应平滑已关闭");
              }
              
              ImGui::Spacing();
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
              
              CustomCheckbox("方向检测", &Aim.directionDetection, ImVec2(140, 45));
              if (ImGui::IsItemClicked()) {
                  文字传入(Aim.directionDetection ? "方向检测已开启" : "方向检测已关闭");
              }
              
              ImGui::SameLine();
              CustomCheckbox("极速响应", &Aim.extremeResponse, ImVec2(140, 45));
              if (ImGui::IsItemClicked()) {
                  文字传入(Aim.extremeResponse ? "极速响应已开启" : "极速响应已关闭");
              }
              
              ImGui::Separator();
            }

          } break;
                        case 5: {
                            // 应用内存区域的动画偏移
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (homepage - (-1600.0f)));
             
                 // 上帝开关（贝塞尔圆形风格）
                 ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); // 添加左边距
                 CustomCheckbox("上帝开关", &上帝, ImVec2(140, 45));
                 if (ImGui::IsItemClicked()) {
                   文字传入(上帝 ? "上帝开关已开启" : "上帝开关已关闭");
                 }        
              
       // 视距调节滑动条（贝塞尔风格）
       ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); // 添加左边距
       SliderFloat("视距调节", &shangdi, 0.0f, 2.5f, "%.2f");
       if (ImGui::IsItemEdited()) {
           char urlls[2560];
           sprintf(urlls, "%.2f", shangdi);
           WriteConfigToIni("/data/光头强配置/配置.ini", "视距调节", urlls);
       }       
              
              ImGui::Separator();
                      ImGui::Text("\n\n\n\n\n");   
                      ImGui::Text("光头强开源"); 
              
              }  // 结束 BeginChild("##触摸功能") 的 if
              ImGui::EndChild();  // 结束 ##触摸功能
            }
                            break;


                        case 8: {
                            // 录屏工具菜单
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (homepage - (-2800.0f)));
                            
                            // 渐变字标题
                            渐变字("录屏工具", ImColor(255, 160, 255, 255), ImColor(97, 206, 255, 255), 60.0f, true);
                            ImGui::Spacing();
                            ImGui::Separator();
                            
                            // 录制时长滑块
                            SliderFloat("录制时长(秒)", &录屏时长, 10, 300, "%.0f");
                            ImGui::Spacing();
                            
                            // 强制截图按钮
                            if (ImGui::Button("强制截图", ImVec2(-1, 60))) {
                                std::srand(static_cast<unsigned int>(std::time(nullptr)));
                                std::string randomDigits = std::to_string(std::rand() % 1000);
                                std::string filePath = "/storage/emulated/0/XF截屏" + randomDigits + ".jpg";
                                std::string command = "su -c screencap -p " + filePath;
                                system(command.c_str());
                                文字传入("截图已保存");
                            }
                            ImGui::Spacing();
                            
                            // 强制录屏按钮
                            if (ImGui::Button("强制录屏", ImVec2(-1, 60))) {
                                if (!isRecording.load()) {
                                    isRecording.store(true);
                                    recordingTimeElapsed.store(0);
                                    std::thread([](int duration) {
                                        std::string fileName = "/sdcard/XF录制.mp4";
                                        std::string command = "screenrecord --time-limit " + std::to_string(duration) + " " + fileName + " &";
                                        system(command.c_str());
                                        
                                        for (int i = 0; i < duration && isRecording.load(); ++i) {
                                            std::this_thread::sleep_for(std::chrono::seconds(1));
                                            recordingTimeElapsed.fetch_add(1);
                                        }
                                        isRecording.store(false);
                                    }, (int)录屏时长).detach();
                                    文字传入("录屏已开始");
                                }
                            }
                            ImGui::Spacing();
                            
                            // 结束录制按钮
                            if (ImGui::Button("结束录制", ImVec2(-1, 60)) && isRecording.load()) {
                                system("pkill -l SIGINT screenrecord");
                                isRecording.store(false);
                                文字传入("录屏已停止");
                            }
                            ImGui::Spacing();
                            
                            // 显示录制状态
                            ImGui::Separator();
                            if (isRecording.load()) {
                                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), 
                                    "录制中... %d / %d 秒", recordingTimeElapsed.load(), (int)录屏时长);
                            } else {
                                ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "未录制");
                            }
                            
                            ImGui::Spacing();
                            ImGui::Separator();
                            ImGui::TextWrapped("提示: 截图保存在 /storage/emulated/0/\n录屏保存在 /sdcard/");
                        }
                    }
                }
                ImGui::EndChild();  // 结束 ##右菜单
            }  // 结束 BeginChild("##右菜单") 的 if
           // ImGui::PopStyleVar(1);  // 恢复 WindowPadding 样式（必须在if(林徽因)内）
           
            // 恢复子窗口背景样式
            ImGui::PopStyleColor(1);  // 弹出ChildBg颜色设置
        }  // 结束if(林徽因)的if
        
            // 合并绘制通道（背景和前景）
            ImGui::GetWindowDrawList()->ChannelsMerge();
            
            }  // 结束 if (Begin("念")) 的 if
            ImGui::End();  // End()在外层条件if内，Begin的if外
        }  // 结束外层条件if

    if (IsWin) {
        IsWin = false;
        IsLoGin = false;
        BallSwitch = false;
        ImGui::SetWindowSize("Ball", {100.0f, 100.0f});
    }
    char fkhs[1000];
        sprintf(fkhs,"",ImGui::GetIO().Framerate);
        ImGui::GetForegroundDrawList()->AddText(ImVec2(300, 1000),ImColor(225,255,255),fkhs);
        
        
    if (huitu == 1) {
        DrawPlayer(); // 调用绘图  直接写在这边的话有点影响观看了
        
    }
    
    // 调用自动换装功能
    自动换装();
    
    // 显示文字传入通知（不包括渲染部分）
    {
        ImGuiIO &io_notify = ImGui::GetIO();
        动态初始化(io_notify.DeltaTime);
        主体显示();
    }
}  // 结束tick()函数
void screen_config() {
    ::displayInfo = android::ANativeWindowCreator::GetDisplayInfo();
    ::screen_x = displayInfo.width;
    ::screen_y = displayInfo.height;
}

void drawBegin() {
    // 如果使用了GraphicsManager，则调用其NewFrame方法
    if (graphics != nullptr) {
        graphics->NewFrame();
    } else {
        // 否则使用传统的OpenGL渲染
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplAndroid_NewFrame();  // 新版本不需要参数
        ImGui::NewFrame();
    }

    screen_config();
    
    // 初始化设置图标纹理（只初始化一次）
    static bool icon_initialized = false;
    if (!icon_initialized && 设置图标纹理.textureId == 0) {
        设置图标纹理 = createTexturePNGFromMem(设置图标, sizeof(设置图标));
        icon_initialized = true;
    }
    
    // 悬浮窗背景纹理在tick()中懒加载（VK渲染需要）

    static uint32_t orientation = -1;
    if (orientation != displayInfo.orientation) {
        orientation = displayInfo.orientation;
        Touch::setOrientation(displayInfo.orientation);
        屏幕方向 = displayInfo.orientation;
        // cout << " width:" << displayInfo.width << " height:" <<
        // displayInfo.height << " orientation:" << displayInfo.orientation << endl;
    }
}

void drawEnd() {
    ImGuiIO &io = ImGui::GetIO();
    
    // 重新启用优化后的状态通知功能
    动态初始化(io.DeltaTime);
    主体显示();
    
    // 如果使用了GraphicsManager，则调用其EndFrame方法
    if (graphics != nullptr) {
        graphics->EndFrame();
    } else {
        // 否则使用传统的OpenGL渲染
        glViewport(0.0f, 0.0f, (int) io.DisplaySize.x, (int) io.DisplaySize.y);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT); // GL_DEPTH_BUFFER_BIT
        glFlush();
        if (display == EGL_NO_DISPLAY) {
            return;
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        eglSwapBuffers(display, surface);
    }
}

void shutdown() {
    if (!g_Initialized) {
        return;
    }
    
    // 停止代理线程
    // printf("程序退出，正在清理代理线程...\n");
    // stop_proxy_thread();  // 函数未定义，已注释
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();
    if (display != EGL_NO_DISPLAY) {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (context != EGL_NO_CONTEXT) {
            eglDestroyContext(display, context);
        }
        if (surface != EGL_NO_SURFACE) {
            eglDestroySurface(display, surface);
        }
        eglTerminate(display);
    }
    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
    surface = EGL_NO_SURFACE;
    ANativeWindow_release(native_window);
}

void SaveAllConfig() {
    printf("[配置保存] 开始保存到INI文件...\n");
    
    WriteConfigToIni("/data/光头强配置/配置.ini", "兵线绘制", 兵线 ? "1" : "0");
    WriteConfigToIni("/data/光头强配置/配置.ini", "地图头像", 地图 ? "1" : "0");
    WriteConfigToIni("/data/光头强配置/配置.ini", "人物方框", 方框 ? "1" : "0");
    WriteConfigToIni("/data/光头强配置/配置.ini", "方框内技能计时", 方框技能 ? "1" : "0");
    WriteConfigToIni("/data/光头强配置/配置.ini", "技能计时面板", 顶上技能2 ? "1" : "0");
    WriteConfigToIni("/data/光头强配置/配置.ini", "野怪计时", 野怪 ? "1" : "0");
    WriteConfigToIni("/data/光头强配置/配置.ini", "射线开关", 射线 ? "1" : "0");
    WriteConfigToIni("/data/光头强配置/配置.ini", "自身视野判断", 自身 ? "1" : "0");
    WriteConfigToIni("/data/光头强配置/配置.ini", "兵线实体", 实体兵线 ? "1" : "0");
    WriteConfigToIni("/data/光头强配置/配置.ini", "实体技能显示", 实体技能 ? "1" : "0");
    WriteConfigToIni("/data/光头强配置/配置.ini", "血量", 血量 ? "1" : "0");
    WriteConfigToIni("/data/光头强配置/配置.ini", "野怪血量显示", 野血 ? "1" : "0");
    WriteConfigToIni("/data/光头强配置/配置.ini", "暴露不绘小地图", 暴露不绘小地图头像 ? "1" : "0");
    WriteConfigToIni("/data/光头强配置/配置.ini", "暴露视野不绘", 暴露视野不绘 ? "1" : "0");
    WriteConfigToIni("/data/光头强配置/配置.ini", "暗化", 暗化 ? "1" : "0");
    WriteConfigToIni("/data/光头强配置/配置.ini", "头像常显", 头像常显 ? "1" : "0");
    WriteConfigToIni("/data/光头强配置/配置.ini", "视野方框", 视野方框 ? "1" : "0");
    WriteConfigToIni("/data/光头强配置/配置.ini", "方框技能不绘", 方框技能不绘 ? "1" : "0");
    
    WriteConfigToIni("/data/光头强配置/配置.ini", "自动斩杀", 自动斩杀 ? "1" : "0");
    WriteConfigToIni("/data/光头强配置/配置.ini", "自动惩戒", 自动惩戒 ? "1" : "0");
    WriteConfigToIni("/data/光头强配置/配置.ini", "自动净化", 自动净化 ? "1" : "0");
    WriteConfigToIni("/data/光头强配置/配置.ini", "有人解控", 有人解控 ? "1" : "0");
    WriteConfigToIni("/data/光头强配置/配置.ini", "智能不惩", 智能不惩 ? "1" : "0");
    WriteConfigToIni("/data/光头强配置/配置.ini", "智能斩杀", 智能斩杀 ? "1" : "0");
    
    char configBuffer[32];
    
    sprintf(configBuffer, "%.0f", FPS);
    WriteConfigToIni("/data/光头强配置/帧率.ini", "帧率数值", configBuffer);
    
    sprintf(configBuffer, "%d", SmallMapX);
    WriteConfigToIni("/data/光头强配置/头像左右.ini", "头像左右", configBuffer);
    
    sprintf(configBuffer, "%d", SmallMapY);
    WriteConfigToIni("/data/光头强配置/头像上下.ini", "头像上下", configBuffer);
    
    sprintf(configBuffer, "%d", SmallHPX);
    WriteConfigToIni("/data/光头强配置/实体左右.ini", "实体左右", configBuffer);
    
    sprintf(configBuffer, "%d", SmallHPY);
    WriteConfigToIni("/data/光头强配置/实体上下.ini", "实体上下", configBuffer);
    
    sprintf(configBuffer, "%d", touxiangdaxiao);
    WriteConfigToIni("/data/光头强配置/头像大小.ini", "头像大小", configBuffer);
    
    sprintf(configBuffer, "%d", xiaodituxue);
    WriteConfigToIni("/data/光头强配置/血量大小.ini", "血量大小", configBuffer);
    
    sprintf(configBuffer, "%d", jiange);
    WriteConfigToIni("/data/光头强配置/地图比例.ini", "地图缩放比例", configBuffer);
    
    sprintf(configBuffer, "%d", jinenglanzX);
    WriteConfigToIni("/data/光头强配置/技能左右.ini", "技能计时左右", configBuffer);
    
    sprintf(configBuffer, "%d", jinenglanzY);
    WriteConfigToIni("/data/光头强配置/技能上下.ini", "技能计时上下", configBuffer);
    
    sprintf(configBuffer, "%d", 屏幕x);
    WriteConfigToIni("/data/光头强配置/视野左右.ini", "自身视野左右", configBuffer);
    
    sprintf(configBuffer, "%d", 屏幕y);
    WriteConfigToIni("/data/光头强配置/视野上下.ini", "自身视野上下", configBuffer);
    
    sprintf(configBuffer, "%d", zhsX);
    WriteConfigToIni("/data/光头强配置/触摸左右.ini", "触摸位置左右", configBuffer);
    
    sprintf(configBuffer, "%d", zhsY);
    WriteConfigToIni("/data/光头强配置/触摸上下.ini", "触摸位置上下", configBuffer);
    
    sprintf(configBuffer, "%.2f", shangdi);
    WriteConfigToIni("/data/光头强配置/配置.ini", "视距调节", configBuffer);
    
    sprintf(configBuffer, "%.2f", zhansha);
    WriteConfigToIni("/data/光头强配置/配置.ini", "斩杀值", configBuffer);
    
    sprintf(configBuffer, "%d", ESPMenu.小地图左右调整);
    WriteConfigToIni("/data/光头强配置/共享配置.ini", "共享左右", configBuffer);
    
    sprintf(configBuffer, "%d", ESPMenu.小地图上下调整);
    WriteConfigToIni("/data/光头强配置/共享配置.ini", "共享上下", configBuffer);
    
    sprintf(configBuffer, "%d", ESPMenu.野怪左右调整);
    WriteConfigToIni("/data/光头强配置/共享配置.ini", "共享野怪左右", configBuffer);
    
    sprintf(configBuffer, "%d", ESPMenu.野怪上下调整);
    WriteConfigToIni("/data/光头强配置/共享配置.ini", "共享野怪上下", configBuffer);
    
    sprintf(configBuffer, "%d", ESPMenu.兵线左右调整);
    WriteConfigToIni("/data/光头强配置/共享配置.ini", "共享兵线左右", configBuffer);
    
    sprintf(configBuffer, "%d", ESPMenu.兵线上下调整);
    WriteConfigToIni("/data/光头强配置/共享配置.ini", "共享兵线上下", configBuffer);
    
    sprintf(configBuffer, "%d", ESPMenu.分辨率X);
    WriteConfigToIni("/data/光头强配置/共享配置.ini", "共享分辨率x", configBuffer);
    
    sprintf(configBuffer, "%d", ESPMenu.分辨率Y);
    WriteConfigToIni("/data/光头强配置/共享配置.ini", "共享分辨率y", configBuffer);
    
    // 保存自瞄配置
    saveConfig(true); 
    
    printf("[配置保存] 所有配置已保存到INI文件\n");
}


void LoadAllConfig() {
    static bool config_loaded = false;
    if (config_loaded) {
        return;
    }
    
    std::string value;
    
    if (ReadConfigFromIni("/data/光头强配置/配置.ini", "兵线绘制", value)) {
        兵线 = (value == "1");
    }
    if (ReadConfigFromIni("/data/光头强配置/配置.ini", "地图头像", value)) {
        地图 = (value == "1");
    }
    if (ReadConfigFromIni("/data/光头强配置/配置.ini", "人物方框", value)) {
        方框 = (value == "1");
    }
    if (ReadConfigFromIni("/data/光头强配置/配置.ini", "方框内技能计时", value)) {
        方框技能 = (value == "1");
    }
    if (ReadConfigFromIni("/data/光头强配置/配置.ini", "技能计时面板", value)) {
        顶上技能2 = (value == "1");
    }
    if (ReadConfigFromIni("/data/光头强配置/配置.ini", "野怪计时", value)) {
        野怪 = (value == "1");
    }
    if (ReadConfigFromIni("/data/光头强配置/配置.ini", "射线开关", value)) {
        射线 = (value == "1");
    }
    if (ReadConfigFromIni("/data/光头强配置/配置.ini", "自身视野判断", value)) {
        自身 = (value == "1");
    }
    if (ReadConfigFromIni("/data/光头强配置/配置.ini", "兵线实体", value)) {
        实体兵线 = (value == "1");
    }
    if (ReadConfigFromIni("/data/光头强配置/配置.ini", "实体技能显示", value)) {
        实体技能 = (value == "1");
    }
    if (ReadConfigFromIni("/data/光头强配置/配置.ini", "血量", value)) {
        血量 = (value == "1");
    }
    if (ReadConfigFromIni("/data/光头强配置/配置.ini", "野怪血量显示", value)) {
        野血 = (value == "1");
    }
    if (ReadConfigFromIni("/data/光头强配置/配置.ini", "暴露不绘小地图", value)) {
        暴露不绘小地图头像 = (value == "1");
    }
    if (ReadConfigFromIni("/data/光头强配置/配置.ini", "暴露视野不绘", value)) {
        暴露视野不绘 = (value == "1");
    }
    if (ReadConfigFromIni("/data/光头强配置/配置.ini", "暗化", value)) {
        暗化 = (value == "1");
    }
    if (ReadConfigFromIni("/data/光头强配置/配置.ini", "头像常显", value)) {
        头像常显 = (value == "1");
    }
    if (ReadConfigFromIni("/data/光头强配置/配置.ini", "视野方框", value)) {
        视野方框 = (value == "1");
    }
    if (ReadConfigFromIni("/data/光头强配置/配置.ini", "方框技能不绘", value)) {
        方框技能不绘 = (value == "1");
    }
    if (ReadConfigFromIni("/data/光头强配置/配置.ini", "自动斩杀", value)) {
        自动斩杀 = (value == "1");
    }
    if (ReadConfigFromIni("/data/光头强配置/配置.ini", "自动惩戒", value)) {
        自动惩戒 = (value == "1");
    }
    if (ReadConfigFromIni("/data/光头强配置/配置.ini", "自动净化", value)) {
        自动净化 = (value == "1");
    }
    if (ReadConfigFromIni("/data/光头强配置/配置.ini", "有人解控", value)) {
        有人解控 = (value == "1");
    }
    if (ReadConfigFromIni("/data/光头强配置/配置.ini", "智能不惩", value)) {
        智能不惩 = (value == "1");
    }
    if (ReadConfigFromIni("/data/光头强配置/配置.ini", "智能斩杀", value)) {
        智能斩杀 = (value == "1");
    }
    
    if (ReadConfigFromIni("/data/光头强配置/帧率.ini", "帧率数值", value)) {
        FPS = atof(value.c_str());
        // 应用FPS到限制器
        float fps = FPS * 2.3f;
        FPS限制.SetFps(fps);
    }
    if (ReadConfigFromIni("/data/光头强配置/头像左右.ini", "头像左右", value)) {
        SmallMapX = atoi(value.c_str());
    }
    if (ReadConfigFromIni("/data/光头强配置/头像上下.ini", "头像上下", value)) {
        SmallMapY = atoi(value.c_str());
    }
    if (ReadConfigFromIni("/data/光头强配置/实体左右.ini", "实体左右", value)) {
        SmallHPX = atoi(value.c_str());
    }
    if (ReadConfigFromIni("/data/光头强配置/实体上下.ini", "实体上下", value)) {
        SmallHPY = atoi(value.c_str());
    }
    if (ReadConfigFromIni("/data/光头强配置/头像大小.ini", "头像大小", value)) {
        touxiangdaxiao = atoi(value.c_str());
    }
    if (ReadConfigFromIni("/data/光头强配置/血量大小.ini", "血量大小", value)) {
        xiaodituxue = atoi(value.c_str());
    }
    if (ReadConfigFromIni("/data/光头强配置/地图比例.ini", "地图缩放比例", value)) {
        jiange = atoi(value.c_str());
    }
    if (ReadConfigFromIni("/data/光头强配置/技能左右.ini", "技能计时左右", value)) {
        jinenglanzX = atoi(value.c_str());
    }
    if (ReadConfigFromIni("/data/光头强配置/技能上下.ini", "技能计时上下", value)) {
        jinenglanzY = atoi(value.c_str());
    }
    if (ReadConfigFromIni("/data/光头强配置/视野左右.ini", "自身视野左右", value)) {
        屏幕x = atoi(value.c_str());
    }
    if (ReadConfigFromIni("/data/光头强配置/视野上下.ini", "自身视野上下", value)) {
        屏幕y = atoi(value.c_str());
    }
    if (ReadConfigFromIni("/data/光头强配置/触摸左右.ini", "触摸位置左右", value)) {
        zhsX = atoi(value.c_str());
    }
    if (ReadConfigFromIni("/data/光头强配置/触摸上下.ini", "触摸位置上下", value)) {
        zhsY = atoi(value.c_str());
    }
    if (ReadConfigFromIni("/data/光头强配置/配置.ini", "视距调节", value)) {
        shangdi = atof(value.c_str());
    }
    if (ReadConfigFromIni("/data/光头强配置/配置.ini", "斩杀值", value)) {
        zhansha = atof(value.c_str());
    }
    if (ReadConfigFromIni("/data/光头强配置/共享配置.ini", "共享左右", value)) {
        ESPMenu.小地图左右调整 = atoi(value.c_str());
    }
    if (ReadConfigFromIni("/data/光头强配置/共享配置.ini", "共享上下", value)) {
        ESPMenu.小地图上下调整 = atoi(value.c_str());
    }
    if (ReadConfigFromIni("/data/光头强配置/共享配置.ini", "共享野怪左右", value)) {
        ESPMenu.野怪左右调整 = atoi(value.c_str());
    }
    if (ReadConfigFromIni("/data/光头强配置/共享配置.ini", "共享野怪上下", value)) {
        ESPMenu.野怪上下调整 = atoi(value.c_str());
    }
    if (ReadConfigFromIni("/data/光头强配置/共享配置.ini", "共享兵线左右", value)) {
        ESPMenu.兵线左右调整 = atoi(value.c_str());
    }
    if (ReadConfigFromIni("/data/光头强配置/共享配置.ini", "共享兵线上下", value)) {
        ESPMenu.兵线上下调整 = atoi(value.c_str());
    }
    if (ReadConfigFromIni("/data/光头强配置/共享配置.ini", "共享分辨率x", value)) {
        ESPMenu.分辨率X = atoi(value.c_str());
    }
    if (ReadConfigFromIni("/data/光头强配置/共享配置.ini", "共享分辨率y", value)) {
        ESPMenu.分辨率Y = atoi(value.c_str());
    }
    
    // 读取自瞄配置
    loadConfig();
    
    config_loaded = true;
}
