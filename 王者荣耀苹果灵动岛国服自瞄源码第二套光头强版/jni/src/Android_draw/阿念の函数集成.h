/* 
 * 由光头强内核（原XF内核）开源
 因频道账号被盗用后决定开源，正版为光头强内核，
开源文件仅供学习交流，任何个人或组织可在遵循协议条款的前提下，自由下载、使用、修改、分发本项目代码及衍生作品，无需额外授权。使用过程中请保留原项目版权声明及开源协议文本，不得用于违法违规场景。本项目以“现状”提供，开发者不承担任何直接或间接的使用风险及法律责任，亦不提供商业技术支持。
删除此内容死全家，没几把
* 注入时间: 2025-11-16 20:14:49
 * 联系方式: TG:@GTQNHNB666
QQ群：784316342
 */
#include "imgui.h"
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <memory>
#include <chrono>
#include <functional>

struct AnianLei {
    std::string message;
    float displayTime;
    float currentTime;
    float slideProgress;
    ImVec2 position;
    bool isNew;
    float scale;
    float targetY;
    float alpha;
    int index;
};

static std::vector<AnianLei> AnianLeis;
static float 阿念のDis参数 = 3.0f;
static float 阿念の滑出速度 = 4.0f;
// 动态计算右侧起始位置，在文字传入函数中设置
static ImVec2 阿念の窗口大小 = ImVec2(30, 20);
static float 阿念の字体大小 = 1.4f;

static float 阿念の间隔 = 20.0f;

// iOS 风格颜色
static ImVec4 iosBackground = ImVec4(0.95f, 0.95f, 0.97f, 1.0f);
static ImVec4 iosBorder = ImVec4(0.78f, 0.78f, 0.8f, 1.0f);
static ImVec4 iosText = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
static ImVec4 iosRed = ImVec4(1.0f, 0.23f, 0.19f, 1.0f);
static ImVec4 iosYellow = ImVec4(1.0f, 0.8f, 0.0f, 1.0f);
static ImVec4 iosGreen = ImVec4(0.34f, 0.8f, 0.03f, 1.0f);

void 文字传入(const std::string& message) {
    AnianLei newMsg;
    newMsg.message = message;
    newMsg.displayTime = 阿念のDis参数;
    newMsg.currentTime = 0.0f;
    newMsg.slideProgress = 0.0f;
    
    // 动态计算起始位置（从屏幕右侧外开始）
    float screenWidth = ImGui::GetIO().DisplaySize.x;
    newMsg.position = ImVec2(screenWidth + 50, 20); // 从屏幕右侧外50像素开始
    
    newMsg.isNew = true;
    newMsg.scale = 0.0f;
    newMsg.alpha = 0.0f;
    newMsg.index = AnianLeis.size();
    
    if (AnianLeis.empty()) {
        newMsg.targetY = 20.0f;
    } else {
        const auto& lastMsg = AnianLeis.back();
        ImVec2 lastTextSize = ImGui::CalcTextSize(lastMsg.message.c_str());
        lastTextSize.x *= 阿念の字体大小;
        lastTextSize.y *= 阿念の字体大小;
        float lastMessageHeight = lastTextSize.y + 阿念の窗口大小.y * 2.0f + 30.0f;
        newMsg.targetY = lastMsg.targetY + lastMessageHeight + 阿念の间隔;
    }
    
    AnianLeis.push_back(newMsg);
}

void 动态初始化(float deltaTime) {
    std::sort(AnianLeis.begin(), AnianLeis.end(), 
        [](const AnianLei& a, const AnianLei& b) {
            return a.index < b.index;
        });
    
    for (auto it = AnianLeis.begin(); it != AnianLeis.end(); ) {
        it->currentTime += deltaTime;
        it->slideProgress = ImMin(it->slideProgress + deltaTime * 阿念の滑出速度, 1.0f);
        
        if (it->scale < 1.0f) {
            it->scale = ImMin(it->scale + deltaTime * 3.0f, 1.0f);
        }
        
        if (it->currentTime < 0.3f) {
            it->alpha = it->currentTime / 0.3f;
        } else if (it->currentTime > it->displayTime - 0.5f) {
            it->alpha = (it->displayTime - it->currentTime) / 0.5f;
        } else {
            it->alpha = 1.0f;
        }
        
        float moveSpeed = 8.0f;
        if (std::abs(it->position.y - it->targetY) > 0.1f) {
            it->position.y = it->position.y + (it->targetY - it->position.y) * moveSpeed * deltaTime;
        } else {
            it->position.y = it->targetY;
        }
        
        float easedT = it->slideProgress < 0.5f ? 2.0f * it->slideProgress * it->slideProgress : 1.0f - pow(-2.0f * it->slideProgress + 2.0f, 2.0f) / 2.0f;        
        
        // 计算目标X位置（右上角对齐）
        ImVec2 textSize = ImGui::CalcTextSize(it->message.c_str());
        float messageWidth = textSize.x * 阿念の字体大小 + 阿念の窗口大小.x * 2.0f;
        float screenWidth = ImGui::GetIO().DisplaySize.x;
        float targetX = screenWidth - messageWidth - 40; // 距离右边缘40像素
        
        // 使用消息自己的初始X位置
        float startX = ImGui::GetIO().DisplaySize.x + 50; // 屏幕右侧外
        it->position.x = startX + (targetX - startX) * easedT;
        
        if (it->currentTime >= it->displayTime) {
            int removedIndex = it->index;
            it = AnianLeis.erase(it);
            
            float currentY = 20.0f;
            for (auto& msg : AnianLeis) {
                msg.targetY = currentY;
                
                ImVec2 textSize = ImGui::CalcTextSize(msg.message.c_str());
                textSize.x *= 阿念の字体大小;
                textSize.y *= 阿念の字体大小;
                float messageHeight = textSize.y + 阿念の窗口大小.y * 2.0f + 30.0f;
                
                currentY += messageHeight + 阿念の间隔;
            }
        } else {
            ++it;
        }
    }
}

void 三色球(const ImVec2& min, float scale, float alpha) {
    ImDrawList* drawList = ImGui::GetForegroundDrawList();
    float radius = 6.0f * scale;
    float spacing = 10.0f * scale;    
    ImVec2 redPos(min.x + radius + 15.0f * scale, min.y + radius + 12.0f * scale);
    ImVec2 yellowPos(redPos.x + radius * 2 + spacing, redPos.y);
    ImVec2 greenPos(yellowPos.x + radius * 2 + spacing, redPos.y);    
    drawList->AddCircleFilled(redPos, radius, ImGui::GetColorU32(ImVec4(iosRed.x, iosRed.y, iosRed.z, alpha * 0.9f)));
    drawList->AddCircleFilled(yellowPos, radius, ImGui::GetColorU32(ImVec4(iosYellow.x, iosYellow.y, iosYellow.z, alpha * 0.9f)));
    drawList->AddCircleFilled(greenPos, radius, ImGui::GetColorU32(ImVec4(iosGreen.x, iosGreen.y, iosGreen.z, alpha * 0.9f)));
}

void 矩形(const ImVec2& min, const ImVec2& max, float alpha, float scale) {
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();    
    ImVec4 bgColor = ImVec4(iosBackground.x, iosBackground.y, iosBackground.z, alpha * 0.98f);
    drawList->AddRectFilled(min, max, ImGui::GetColorU32(bgColor), 12.0f * scale);    
    ImVec4 borderColor = ImVec4(iosBorder.x, iosBorder.y, iosBorder.z, alpha);
    drawList->AddRect(min, max, ImGui::GetColorU32(borderColor), 12.0f * scale, 0, 1.5f * scale);    
    ImVec2 titleBarMin = ImVec2(min.x, min.y);
    ImVec2 titleBarMax = ImVec2(max.x, min.y + 30.0f * scale);
    ImVec4 titleBarColor = ImVec4(0.9f, 0.9f, 0.91f, alpha);
    drawList->AddRectFilled(titleBarMin, titleBarMax, ImGui::GetColorU32(titleBarColor), 12.0f * scale, ImDrawFlags_RoundCornersTop);    
    ImVec4 separatorColor = ImVec4(iosBorder.x, iosBorder.y, iosBorder.z, alpha * 0.8f);
    drawList->AddLine(ImVec2(min.x, titleBarMax.y), ImVec2(max.x, titleBarMax.y), ImGui::GetColorU32(separatorColor), 1.0f * scale); 
    三色球(ImVec2(min.x + 15.0f * scale, min.y + 5.0f * scale), scale, alpha);
}

void 主体显示() {
    if (AnianLeis.empty()) return;
    
    // 为每个消息创建独立的小窗口，避免全屏覆盖
    for (auto& msg : AnianLeis) {
        if (msg.slideProgress > 0.01f && msg.alpha > 0.01f) {
            ImVec2 textSize = ImGui::CalcTextSize(msg.message.c_str());            
            ImVec2 bgMin = msg.position;
            ImVec2 bgMax = ImVec2(bgMin.x + textSize.x * 阿念の字体大小 + 阿念の窗口大小.x * 2.0f, bgMin.y + textSize.y * 阿念の字体大小 + 阿念の窗口大小.y * 2.0f + 30.0f);            
            
            if (bgMin.x < ImGui::GetIO().DisplaySize.x && bgMax.y > 0) {
                // 为每个消息创建独立的小窗口
                std::string windowName = "##Notification" + std::to_string(msg.index);
                
                ImGui::SetNextWindowPos(bgMin);
                ImGui::SetNextWindowSize(ImVec2(bgMax.x - bgMin.x, bgMax.y - bgMin.y));
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0)); // 完全透明背景
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0)); // 无边框
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                
                if (ImGui::Begin(windowName.c_str(), nullptr, 
                    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
                    ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | 
                    ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings |
                    ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoFocusOnAppearing |
                    ImGuiWindowFlags_NoBackground)) {
                    
                    ImDrawList* drawList = ImGui::GetWindowDrawList();
                    
                    // 绘制背景矩形
                    ImVec4 bgColor = ImVec4(iosBackground.x, iosBackground.y, iosBackground.z, msg.alpha * 0.98f);
                    drawList->AddRectFilled(bgMin, bgMax, ImGui::GetColorU32(bgColor), 12.0f * msg.scale);
                    
                    // 绘制边框
                    ImVec4 borderColor = ImVec4(iosBorder.x, iosBorder.y, iosBorder.z, msg.alpha);
                    drawList->AddRect(bgMin, bgMax, ImGui::GetColorU32(borderColor), 12.0f * msg.scale, 0, 1.5f * msg.scale);
                    
                    // 绘制标题栏
                    ImVec2 titleBarMin = ImVec2(bgMin.x, bgMin.y);
                    ImVec2 titleBarMax = ImVec2(bgMax.x, bgMin.y + 30.0f * msg.scale);
                    ImVec4 titleBarColor = ImVec4(0.9f, 0.9f, 0.91f, msg.alpha);
                    drawList->AddRectFilled(titleBarMin, titleBarMax, ImGui::GetColorU32(titleBarColor), 12.0f * msg.scale, ImDrawFlags_RoundCornersTop);
                    
                    // 绘制分割线
                    ImVec4 separatorColor = ImVec4(iosBorder.x, iosBorder.y, iosBorder.z, msg.alpha * 0.8f);
                    drawList->AddLine(ImVec2(bgMin.x, titleBarMax.y), ImVec2(bgMax.x, titleBarMax.y), ImGui::GetColorU32(separatorColor), 1.0f * msg.scale);
                    
                    // 绘制三色球
                    float radius = 6.0f * msg.scale;
                    float spacing = 10.0f * msg.scale;
                    ImVec2 redPos(bgMin.x + radius + 15.0f * msg.scale, bgMin.y + radius + 12.0f * msg.scale);
                    ImVec2 yellowPos(redPos.x + radius * 2 + spacing, redPos.y);
                    ImVec2 greenPos(yellowPos.x + radius * 2 + spacing, redPos.y);
                    drawList->AddCircleFilled(redPos, radius, ImGui::GetColorU32(ImVec4(iosRed.x, iosRed.y, iosRed.z, msg.alpha * 0.9f)));
                    drawList->AddCircleFilled(yellowPos, radius, ImGui::GetColorU32(ImVec4(iosYellow.x, iosYellow.y, iosYellow.z, msg.alpha * 0.9f)));
                    drawList->AddCircleFilled(greenPos, radius, ImGui::GetColorU32(ImVec4(iosGreen.x, iosGreen.y, iosGreen.z, msg.alpha * 0.9f)));
                    
                    // 绘制文字
                    ImVec2 textPos = ImVec2(bgMin.x + (bgMax.x - bgMin.x - textSize.x * 阿念の字体大小) / 2.0f, bgMin.y + 30.0f + (bgMax.y - bgMin.y - 30.0f - textSize.y * 阿念の字体大小) / 2.0f);
                    ImVec4 textColor = ImVec4(iosText.x, iosText.y, iosText.z, msg.alpha);
                    drawList->AddText(ImGui::GetFont(), ImGui::GetFontSize() * 阿念の字体大小, textPos, ImGui::GetColorU32(textColor), msg.message.c_str());
                }  // 结束if(Begin)
                ImGui::End();  // End()必须在if块外，但仍然会被调用
                
                ImGui::PopStyleVar(2);
                ImGui::PopStyleColor(2);
            }
        }
    }
}