/* 
 * 由光头强内核（原XF内核）开源
 因频道账号被盗用后决定开源，正版为光头强内核，
开源文件仅供学习交流，任何个人或组织可在遵循协议条款的前提下，自由下载、使用、修改、分发本项目代码及衍生作品，无需额外授权。使用过程中请保留原项目版权声明及开源协议文本，不得用于违法违规场景。本项目以“现状”提供，开发者不承担任何直接或间接的使用风险及法律责任，亦不提供商业技术支持。
删除此内容死全家，没几把
* 注入时间: 2025-11-16 20:14:49
 * 联系方式: TG:@GTQNHNB666
QQ群：784316342
 */
#ifndef CONFING_H
#define CONFING_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>
#include <regex>
#include <cstdlib>
#include <chrono>
#define BYTE16 0x00000010
using namespace std;

void WriteConfigToIni(const std::string& filePath, const std::string& section, const std::string& value);
bool ReadConfigFromIni(const std::string& filePath, const std::string& section, std::string& value);

// 函数前向声明
void saveConfig(bool forceNow = false);

extern bool LoadTouch;
extern bool ProtStart;
float MoveX =600,MoveY = 600;
float MovePos = 88;
extern Vector2 TriggerArea;
extern Vector2 FlyTriggerArea;
extern int Drivermodel;
extern int socket_fd;
float 范围大小 = 91.0f;
bool 野怪实体 = true; // 默认启用实体野怪
bool 兵线实体 = true; // 默认启用实体兵线
bool WriteGodEnabled = false;
bool WriteGod = false;
struct Config {
    
    
    
    float mapinterval = 0; //小地图间隔
    int BloodBarStyle = 0;//血条样式
    int Boxjudgment = 0;//方框样式
    int Panelstyle = 1;//面板样式
    int TroopEntity = 1;//兵线实体
    int exposedstyle = 0;//暴露样式
    int Wildentity = 0;
    float exposedY;//暴露
    
    
    float Txsize = 1.0f;//头像大小
    float EntityX = 0; //实体左右
    float EntityY = 0; //实体上下
    float Txabout = 0.0f;//头像左右
    float Txupdown = 0.0f;//头像上下
    float Mbabout = 0.0f;//面板左右
    float Mbupdown = 0.0f;//面板上下
    float God = 0.0f;//上帝值
    float RayDistance = 20.0f;//射线距离
    float Raythickness = 1.0f;
    
    float Killdistance = 7000;//斩杀距离
    float Punishment = 7000;//惩戒距离
    float purdistance = 7000;//净化距离
    float FlyDelay = 7000;//飞雷神延迟
    double distance[BYTE16]; //距敌
    
    bool Record = false;//录屏
    bool Pattern = true;//血量
    bool Ray = true;//射线
    bool Box = true;//方框
    bool Entity = true;//实体
    bool Home = true;//回城
    bool Skill = true;//技能
    bool Panel = true;//面板
    bool Bing = true;//兵线
    bool Creeps = true;//野怪
    bool CreepsMap = true;//野怪
    bool shared = false;//共享
    bool behead = false;//斩杀
    bool behPos = false;//斩杀位置
    bool Displayrange = false;//显示范围
    bool Wildiscipline = false;//惩戒
    bool Wildisciplinepos = false;//显示创建位置
    bool Wildisciplineange = false;//显示范围
    bool purification = false;//自动净化
    bool purificationpos = false; //自动净化位置
    bool purificationange = false;//自动净化范围
    bool fly = false;//飞雷神
    bool Flypos = false; //飞雷神位置
    
    // 新增显示开关
    bool ShowSkill1 = true;
    bool ShowSkill2 = true; 
    bool ShowSkill3 = true;
    
    // 兵线相关显示开关
    bool ShowMinionIndex = false;
    bool ShowMinionCoords = false; 
    bool ShowMinionType = false;
    
    // 暴露相关
    bool exposedEnabled = true; // 默认开启暴露显示
    
    // 地图相关
    float MapAvatarSize = 0.9f;
    
    // 小地图敌人显示配置
    bool ShowEnemyOnMinimap = true;  // 是否在小地图显示敌人
    bool ShowEnemyWhenNotExposed = true; // 是否显示未暴露的敌人
    bool ShowEnemyHealthOnMinimap = true; // 是否显示敌人血量
    bool ShowDefaultEnemyMarker = true; // 无头像时是否显示默认敌人标记
    
    // 小地图兵线显示配置
    bool ShowMinionsOnMinimap = true; // 是否在小地图显示兵线
    bool ShowMinionTypeMarker = false; // 是否显示兵线类型标记
    bool ShowCannonMinionSpecial = true; // 是否为炮车兵线特殊标记
    
    // 小地图野怪显示配置
    bool ShowWildOnMinimap = true; // 是否在小地图显示野怪
    bool ShowWildHealthPercent = true; // 是否显示野怪血量百分比
    bool ShowWildCD = true; // 是否显示野怪CD倒计时
    float WildAvatarSize = 1.0f; // 野怪图标大小
    
    // 顶上技能面板增强配置
    bool ShowPanelForAllEnemies = true; // 为所有敌人显示面板（不论视野状态）
    bool ShowPanelHP = true; // 在面板中显示血量数值
    bool ShowPanelSkillCD = true; // 在面板中显示技能CD
    
    // 暴露提示增强配置
    bool ShowExposedWarning = true; // 显示暴露警告动画
    bool ShowExposedSound = false; // 暴露声音提示（预留）
    
    bool Physicalskills = true; // 默认启用视野判断
    bool Dot = false;
    bool Movement = false;
    bool Movesize = false;
    
    int chainIndex = 0;  // 当前锁链索引
    bool showChainSelector = false; // 显示锁链选择器
    
    ImVec4 color1 = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // 红色
    ImVec4 color2 = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // 绿色
    ImVec4 color3 = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // 射线颜色 - 红色
    ImVec4 color4 = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // 方框颜色 - 红色    
    ImVec4 color5 = ImVec4(0.0f, 255.0f, 255.0f, 150.0f);//天蓝
    ImVec4 color6 = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // 绿色
    ImVec4 color7 = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // 兵线颜色 - 红色
    ImVec4 color8 = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // 野怪颜色 - 绿色
    ImVec4 color9 = ImVec4(0.0f, 255.0f, 255.0f, 150.0f);//天蓝
    ImVec4 color10 = ImVec4(0.0f, 255.0f, 255.0f, 150.0f);//天蓝
    ImVec4 color11 = ImVec4(0.0f, 255.0f, 255.0f, 150.0f);//天蓝
    ImVec4 color12 = ImVec4(0.0f, 255.0f, 255.0f, 150.0f);//天蓝
    // 新增颜色配置
    ImVec4 Skill1Color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // 一技能颜色
    ImVec4 Skill2Color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // 二技能颜色
    ImVec4 Skill3Color = ImVec4(0.0f, 0.0f, 1.0f, 1.0f); // 三技能颜色
    

};

Config config;
int 显示全部技能 = 0; // 0: 只显示小技能, 1: 显示全部技能

// 函数声明
void createAndWriteStatusFile(const char* status);

// 函数实现
void createAndWriteStatusFile(const char* status) {
    // 创建并写入状态文件
    std::ofstream statusFile("/data/状态文件.txt");
    if (statusFile.is_open()) {
        statusFile << status << std::endl;
        statusFile.close();
    }
}

struct touch {
float PunishmentX = 600;
float PunishmentY = 600;
float beheadX = 600;
float beheadY = 600;
float purificationX = 600;
float purificationY = 600;
float flyX = 600;
float flyY = 600;
float Space2X =600;
float Space2Y =600;
float 典韦解控一技能X =600;
float 典韦解控一技能Y =600;
float 典韦解控三技能X =600;
float 典韦解控三技能Y =600;
};
touch Touchobj;

struct FlyTouchobj {
float size = 88;
float X = 600;
float Y = 600;
};
FlyTouchobj Fly;

struct Aimobj {
bool Start;
bool Pos;
bool vision;
bool scope;
bool Showtarget;
bool trigger;
bool Leftport;
bool Circle = false;

float Predictionsed = 3.0f;
bool Enable;
int mode = 0;
int select_mode = 0;
float angle;
float distance;
float Skillsize = 150.0f;

float TriggerpositionX=2072;
float TriggerpositionY=875;
float Smoothness = 0.9;
float Bulletspeed = 12500;

int updateInterval = 1;       // 触摸更新间隔(毫秒)
int minTriggerInterval = 60;  // 最小触发间隔(毫秒)
bool debugPredict = false;     // 是否显示预测调试信息
float responseSpeed = 0.9f;  // 响应速度提高到0.9
bool adaptiveSmoothing = true; // 保持自适应平滑
bool directionDetection = true; // 保持方向变化检测
bool extremeResponse = true;  // 添加极速响应模式选项



};
Aimobj Aim;

template<typename T>
void parseConfig(const std::string& line, const std::string& key, T& value) {
    if (line.find(key) != std::string::npos) {
        std::istringstream iss(line.substr(line.find(key) + key.length()));
        if constexpr (std::is_same_v<T, bool>) {
            std::string val;
            iss >> val;
            value = (val == "开");
        } else if constexpr (std::is_same_v<T, float>) {
            iss >> value;
        } else if constexpr (std::is_same_v<T, ImVec4>) {
            std::regex colorRegex(R"((\d+\.\d+)\s*G:\s*(\d+\.\d+)\s*B:\s*(\d+\.\d+)\s*A:\s*(\d+\.\d+))");

            std::smatch matches;
            std::string colorStr = line.substr(line.find(key) + key.length());
            if (std::regex_search(colorStr, matches, colorRegex)) {
                if (matches.size() >= 5) {
                    value.x = std::stof(matches[1].str());
                    value.y = std::stof(matches[2].str());
                    value.z = std::stof(matches[3].str());
                    value.w = std::stof(matches[4].str());
                }
            }
        } else if constexpr (std::is_same_v<T, int>) {
            iss >> value;
        }
    }
}



void loadConfig() {
    // 使用ReadConfigFromIni从INI文件读取自瞄配置
    std::string value;
    int loadCount = 0;
    
    // 读取Aim结构体配置
    if (ReadConfigFromIni("/data/光头强配置/自瞄.ini", "自瞄开关", value)) {
        Aim.Start = (value == "1");
        loadCount++;
    }
    if (ReadConfigFromIni("/data/光头强配置/自瞄.ini", "自瞄模式", value)) {
        Aim.mode = atoi(value.c_str());
        loadCount++;
    }
    if (ReadConfigFromIni("/data/光头强配置/自瞄.ini", "目标模式", value)) {
        Aim.select_mode = atoi(value.c_str());
        loadCount++;
    }
    if (ReadConfigFromIni("/data/光头强配置/自瞄.ini", "自瞄距离", value)) {
        Aim.distance = atof(value.c_str());
        loadCount++;
    }
    if (ReadConfigFromIni("/data/光头强配置/自瞄.ini", "平滑度", value)) {
        Aim.Smoothness = atof(value.c_str());
        loadCount++;
    }
    if (ReadConfigFromIni("/data/光头强配置/自瞄.ini", "预判幅度", value)) {
        Aim.Predictionsed = atof(value.c_str());
        loadCount++;
    }
    if (ReadConfigFromIni("/data/光头强配置/自瞄.ini", "子弹速度", value)) {
        Aim.Bulletspeed = atof(value.c_str());
        loadCount++;
    }
    if (ReadConfigFromIni("/data/光头强配置/自瞄.ini", "触发位置X", value)) {
        Aim.TriggerpositionX = atof(value.c_str());
        loadCount++;
    }
    if (ReadConfigFromIni("/data/光头强配置/自瞄.ini", "触发位置Y", value)) {
        Aim.TriggerpositionY = atof(value.c_str());
        loadCount++;
    }
    if (ReadConfigFromIni("/data/光头强配置/自瞄.ini", "触发范围大小", value)) {
        范围大小 = atof(value.c_str());
        loadCount++;
    }
    if (ReadConfigFromIni("/data/光头强配置/自瞄.ini", "视野判断", value)) {
        Aim.vision = (value == "1");
        loadCount++;
    }
    if (ReadConfigFromIni("/data/光头强配置/自瞄.ini", "目标信息", value)) {
        Aim.Showtarget = (value == "1");
        loadCount++;
    }
    if (ReadConfigFromIni("/data/光头强配置/自瞄.ini", "自瞄范围", value)) {
        Aim.Pos = (value == "1");
        loadCount++;
    }
    if (ReadConfigFromIni("/data/光头强配置/自瞄.ini", "距离圈", value)) {
        Aim.Circle = (value == "1");
        loadCount++;
    }
    if (ReadConfigFromIni("/data/光头强配置/自瞄.ini", "双指触发", value)) {
        Aim.trigger = (value == "1");
        loadCount++;
    }
    if (ReadConfigFromIni("/data/光头强配置/自瞄.ini", "充电方向", value)) {
        Aim.Leftport = (value == "1");
        loadCount++;
    }
    if (ReadConfigFromIni("/data/光头强配置/自瞄.ini", "英雄优化", value)) {
        Aim.Enable = (value == "1");
        loadCount++;
    }
    if (ReadConfigFromIni("/data/光头强配置/自瞄.ini", "自瞄范围开关", value)) {
        Aim.scope = (value == "1");
        loadCount++;
    }
    if (ReadConfigFromIni("/data/光头强配置/自瞄.ini", "自瞄角度", value)) {
        Aim.angle = atof(value.c_str());
        loadCount++;
    }
    if (ReadConfigFromIni("/data/光头强配置/自瞄.ini", "技能大小", value)) {
        Aim.Skillsize = atof(value.c_str());
        loadCount++;
    }
    if (ReadConfigFromIni("/data/光头强配置/自瞄.ini", "更新间隔", value)) {
        Aim.updateInterval = atoi(value.c_str());
        loadCount++;
    }
    if (ReadConfigFromIni("/data/光头强配置/自瞄.ini", "最小触发间隔", value)) {
        Aim.minTriggerInterval = atoi(value.c_str());
        loadCount++;
    }
    if (ReadConfigFromIni("/data/光头强配置/自瞄.ini", "调试预测", value)) {
        Aim.debugPredict = (value == "1");
        loadCount++;
    }
    if (ReadConfigFromIni("/data/光头强配置/自瞄.ini", "响应速度", value)) {
        Aim.responseSpeed = atof(value.c_str());
        loadCount++;
    }
    if (ReadConfigFromIni("/data/光头强配置/自瞄.ini", "自适应平滑", value)) {
        Aim.adaptiveSmoothing = (value == "1");
        loadCount++;
    }
    if (ReadConfigFromIni("/data/光头强配置/自瞄.ini", "方向检测", value)) {
        Aim.directionDetection = (value == "1");
        loadCount++;
    }
    if (ReadConfigFromIni("/data/光头强配置/自瞄.ini", "极速响应", value)) {
        Aim.extremeResponse = (value == "1");
        loadCount++;
    }
    
    // 应用触发区域
    TriggerArea.x = Aim.TriggerpositionX;
    TriggerArea.y = Aim.TriggerpositionY;
    
    // 如果没有加载到配置，使用默认值
    if (loadCount == 0) {
        // 设置默认值
        Aim.Start = false;
        Aim.mode = 0;
        Aim.select_mode = 0;
        Aim.distance = 15000.0f;
        Aim.Smoothness = 0.8f;
        Aim.Predictionsed = 3.0f;
        Aim.Bulletspeed = 12500.0f;
        Aim.TriggerpositionX = 1655.0f;
        Aim.TriggerpositionY = 700.0f;
        Aim.vision = false;
        Aim.Showtarget = true;
        Aim.Pos = true;
        Aim.Circle = true;
        Aim.scope = true;
        Aim.trigger = false;
        Aim.Leftport = false;
        Aim.Enable = false;
        Aim.angle = 45.0f;
        Aim.Skillsize = 1.0f;
        Aim.updateInterval = 16;
        Aim.minTriggerInterval = 50;
        Aim.debugPredict = false;
        Aim.responseSpeed = 1.0f;
        Aim.adaptiveSmoothing = false;
        Aim.directionDetection = false;
        Aim.extremeResponse = false;
        范围大小 = 91.0f;
        
        // 保存默认配置
        saveConfig(true);
    }
}

void saveConfig(bool forceNow) {
    // 防抖：距离上次保存至少300ms才执行（除非强制保存）
    static auto lastSaveTime = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastSaveTime).count();
    
    if (!forceNow && elapsed < 300) {
        // 间隔太短，跳过保存避免卡顿
        TriggerArea.x = Aim.TriggerpositionX;
        TriggerArea.y = Aim.TriggerpositionY;
        FlyTriggerArea.x = Fly.X;
        FlyTriggerArea.y = Fly.Y;
        return;
    }
    
    // 一次性构建所有配置内容
    std::string content;
    content.reserve(1024);
    char buffer[32];
    
    // 批量添加所有配置项
    content += "自瞄开关="; content += Aim.Start ? "1" : "0"; content += "\n";
    
    sprintf(buffer, "%d", Aim.mode);
    content += "自瞄模式="; content += buffer; content += "\n";
    
    sprintf(buffer, "%d", Aim.select_mode);
    content += "目标模式="; content += buffer; content += "\n";
    
    sprintf(buffer, "%.0f", Aim.distance);
    content += "自瞄距离="; content += buffer; content += "\n";
    
    sprintf(buffer, "%.2f", Aim.Smoothness);
    content += "平滑度="; content += buffer; content += "\n";
    
    sprintf(buffer, "%.1f", Aim.Predictionsed);
    content += "预判幅度="; content += buffer; content += "\n";
    
    sprintf(buffer, "%.0f", Aim.Bulletspeed);
    content += "子弹速度="; content += buffer; content += "\n";
    
    sprintf(buffer, "%.0f", Aim.TriggerpositionX);
    content += "触发位置X="; content += buffer; content += "\n";
    
    sprintf(buffer, "%.0f", Aim.TriggerpositionY);
    content += "触发位置Y="; content += buffer; content += "\n";
    
    sprintf(buffer, "%.1f", 范围大小);
    content += "触发范围大小="; content += buffer; content += "\n";
    
    content += "视野判断="; content += Aim.vision ? "1" : "0"; content += "\n";
    content += "目标信息="; content += Aim.Showtarget ? "1" : "0"; content += "\n";
    content += "自瞄范围="; content += Aim.Pos ? "1" : "0"; content += "\n";
    content += "距离圈="; content += Aim.Circle ? "1" : "0"; content += "\n";
    content += "双指触发="; content += Aim.trigger ? "1" : "0"; content += "\n";
    content += "充电方向="; content += Aim.Leftport ? "1" : "0"; content += "\n";
    content += "英雄优化="; content += Aim.Enable ? "1" : "0"; content += "\n";
    content += "自瞄范围开关="; content += Aim.scope ? "1" : "0"; content += "\n";
    
    sprintf(buffer, "%.1f", Aim.angle);
    content += "自瞄角度="; content += buffer; content += "\n";
    
    sprintf(buffer, "%.1f", Aim.Skillsize);
    content += "技能大小="; content += buffer; content += "\n";
    
    sprintf(buffer, "%d", Aim.updateInterval);
    content += "更新间隔="; content += buffer; content += "\n";
    
    sprintf(buffer, "%d", Aim.minTriggerInterval);
    content += "最小触发间隔="; content += buffer; content += "\n";
    
    content += "调试预测="; content += Aim.debugPredict ? "1" : "0"; content += "\n";
    
    sprintf(buffer, "%.2f", Aim.responseSpeed);
    content += "响应速度="; content += buffer; content += "\n";
    
    content += "自适应平滑="; content += Aim.adaptiveSmoothing ? "1" : "0"; content += "\n";
    content += "方向检测="; content += Aim.directionDetection ? "1" : "0"; content += "\n";
    content += "极速响应="; content += Aim.extremeResponse ? "1" : "0"; content += "\n";
    
    // 一次性写入文件
    std::ofstream outFile("/data/光头强配置/自瞄.ini", std::ios::out | std::ios::trunc);
    if (outFile.is_open()) {
        outFile << content;
        outFile.close();
        lastSaveTime = now; // 更新最后保存时间
    }
    
    // 应用触发区域
    TriggerArea.x = Aim.TriggerpositionX;
    TriggerArea.y = Aim.TriggerpositionY;
    FlyTriggerArea.x = Fly.X;
    FlyTriggerArea.y = Fly.Y;
}

void saveConfigOld() {
    system("mkdir -p /data/光头强配置");
    
    ofstream configFile("/data/光头强配置/Config.cpp");
    if (configFile.is_open()) {
        configFile << "int //功能配置//\n";
        
        configFile << "# 血量[头像] == " << (config.Pattern ? "开" : "关") << "\n";
        configFile << "# 射线[实体] == " << (config.Ray ? "开" : "关") << "\n";
        configFile << "# 方框[实体] == " << (config.Box ? "开" : "关") << "\n";
        configFile << "# 实体[头像] == " << (config.Entity ? "开" : "关") << "\n";
        configFile << "# 回城[绘制] == " << (config.Home ? "开" : "关") << "\n";
        configFile << "# 技能[实体] == " << (config.Skill ? "开" : "关") << "\n";
        configFile << "# 技能[视野] == " << (config.Physicalskills ? "开" : "关") << "\n";        
        configFile << "# 面板[绘制] == " << (config.Panel ? "开" : "关") << "\n";
        configFile << "# 野怪实体[实体] == " << (野怪实体 ? "开" : "关") << "\n";
        configFile << "# 兵线实体[实体] == " << (兵线实体 ? "开" : "关") << "\n";
        configFile << "# 兵线[绘制] == " << (config.Bing ? "开" : "关") << "\n";
        configFile << "# 野怪[绘制] == " << (config.Creeps ? "开" : "关") << "\n";
        configFile << "# 录屏[开关] == " << (permeate_record ? "开" : "关") << "\n";
        configFile << "# 模拟[触摸] == " << (LoadTouch ? "开" : "关") << "\n";
        configFile << "# 自动[斩杀] == " << (config.behead ? "开" : "关") << "\n";
        configFile << "# 斩杀[位置] == " << (config.behPos ? "开" : "关") << "\n";
        configFile << "# 斩杀[范围] == " << (config.Displayrange ? "开" : "关") << "\n";
        configFile << "# 自动[惩戒] == " << (config.Wildiscipline ? "开" : "关") << "\n";
        configFile << "# 惩戒[位置] == " << (config.Wildisciplinepos ? "开" : "关") << "\n";
        configFile << "# 惩戒[范围] == " << (config.Wildisciplineange ? "开" : "关") << "\n";
        configFile << "# 自动[净化] == " << (config.purification ? "开" : "关") << "\n";
        configFile << "# 净化[位置] == " << (config.purificationpos ? "开" : "关") << "\n";
        configFile << "# 净化[范围] == " << (config.purificationange ? "开" : "关") << "\n";
        configFile << "# 显示一技能 == " << (config.ShowSkill1 ? "开" : "关") << "\n";
        configFile << "# 显示二技能 == " << (config.ShowSkill2 ? "开" : "关") << "\n";
        configFile << "# 显示三技能 == " << (config.ShowSkill3 ? "开" : "关") << "\n";
        configFile << "# 暴露[显示] == " << (config.exposedEnabled ? "开" : "关") << "\n";
        configFile << "# 自动[飞雷神] == " << (config.fly ? "开" : "关") << "\n";
        configFile << "# 飞雷神[位置] == " << (config.Flypos ? "开" : "关") << "\n";
        configFile << "# 野怪[血量]地图 == " << (config.CreepsMap ? "开" : "关") << "\n";
        
        // 小地图显示配置
        configFile << "# 小地图[敌人显示] == " << (config.ShowEnemyOnMinimap ? "开" : "关") << "\n";
        configFile << "# 小地图[未暴露敌人] == " << (config.ShowEnemyWhenNotExposed ? "开" : "关") << "\n";
        configFile << "# 小地图[敌人血量] == " << (config.ShowEnemyHealthOnMinimap ? "开" : "关") << "\n";
        configFile << "# 小地图[兵线显示] == " << (config.ShowMinionsOnMinimap ? "开" : "关") << "\n";
        configFile << "# 小地图[野怪显示] == " << (config.ShowWildOnMinimap ? "开" : "关") << "\n";
        configFile << "# 小地图[野怪血量] == " << (config.ShowWildHealthPercent ? "开" : "关") << "\n";
        configFile << "# 小地图[野怪CD] == " << (config.ShowWildCD ? "开" : "关") << "\n";
        
        configFile << "\nint //参数配置//\n";
        configFile << "# 头像[大小] == " << config.Txsize << "\n";
        configFile << "# 小地图[头像大小] == " << config.MapAvatarSize << "\n";
        configFile << "# 地图[左右] == " << config.Txabout << "\n";
        configFile << "# 地图[上下] == " << config.Txupdown << "\n";
        configFile << "# 面板[左右] == " << config.Mbabout << "\n";
        configFile << "# 面板[上下] == " << config.Mbupdown << "\n";
        configFile << "# 实体[左右] == " << config.EntityX << "\n";
        configFile << "# 实体[上下] == " << config.EntityY << "\n";
        configFile << "# 相机[高度] == " << config.God << "\n";
        configFile << "# 射线[距离] == " << config.RayDistance << "\n";
        configFile << "# 射线[粗细] == " << config.Raythickness << "\n";
        configFile << "# 暴露[上下] == " << config.exposedY << "\n";
       
        configFile << "# 地图[缩放] == " << config.mapinterval << "\n";
        configFile << "# 血量[样式] == " << config.BloodBarStyle << "\n";
        configFile << "# 方框[判断] == " << config.Boxjudgment << "\n";
        configFile << "# 面板[样式] == " << config.Panelstyle << "\n";
        configFile << "# 暴露[样式] == " << config.exposedstyle << "\n";
        configFile << "# 兵线[实体] == " << config.TroopEntity << "\n";
        configFile << "# 野怪[实体] == " << config.Wildentity << "\n";
        
        
        
        configFile << "\nint //触摸配置//\n";
        configFile << "# 斩杀距离 == " << config.Killdistance << "\n";
        configFile << "# 惩戒距离 == " << config.Punishment << "\n";        
        configFile << "# 净化距离 == " << config.purdistance << "\n";        
        configFile << "# 斩杀位置X == " << Touchobj.beheadX << "\n";
        configFile << "# 斩杀位置Y == " << Touchobj.beheadY << "\n";
        configFile << "# 惩戒位置X == " << Touchobj.PunishmentX << "\n";
        configFile << "# 惩戒位置Y == " << Touchobj.PunishmentY << "\n";
        configFile << "# 净化位置X == " << Touchobj.purificationX << "\n";
        configFile << "# 净化位置Y == " << Touchobj.purificationY << "\n";        
        configFile << "# 飞雷神延迟 == " << config.FlyDelay << "\n";       
        configFile << "# 飞雷神位置X == " << Touchobj.flyX << "\n";
        configFile << "# 飞雷神位置Y == " << Touchobj.flyY << "\n";
        configFile << "# 飞雷神[坐标X] == " << Fly.X << "\n";       
        configFile << "# 飞雷神[坐标Y] == " << Fly.Y << "\n";     
        configFile << "# 飞雷神[二技能X] == " << Touchobj.Space2X << "\n";
        configFile << "# 飞雷神[二技能Y] == " << Touchobj.Space2Y << "\n";
        configFile << "# 典韦一技能解控位置X == " << Touchobj.典韦解控一技能X << "\n";
        configFile << "# 典韦一技能解控位置Y == " << Touchobj.典韦解控一技能Y << "\n";
        configFile << "# 典韦三技能解控位置X == " << Touchobj.典韦解控三技能X << "\n";
        configFile << "# 典韦三技能解控位置Y == " << Touchobj.典韦解控三技能Y << "\n";
        
        configFile << "\nint //自瞄配置//\n";
        configFile << "# 自瞄[开关] == " << (Aim.Start ? "开" : "关") << "\n";
        configFile << "# 自瞄[位置] == " << (Aim.Pos ? "开" : "关") << "\n";       
        configFile << "# 自瞄[范围] == " << (Aim.scope ? "开" : "关") << "\n";
        configFile << "# 自瞄[距离] == " << Aim.distance << "\n";
        configFile << "# 视野[判断] == " << (Aim.vision ? "开" : "关") << "\n";
        configFile << "# 技能[大小] == " << 范围大小 << "\n";
        configFile << "# 自瞄[角度] == " << Aim.angle << "\n";
        configFile << "# 自瞄[类型] == " << Aim.mode << "\n";
        configFile << "# 自瞄[平滑] == " << Aim.Smoothness << "\n";
        configFile << "# 自瞄[优化] == " << (Aim.Enable ? "开" : "关") << "\n";
        configFile << "# 子弹[速度] == " << Aim.Bulletspeed << "\n";        
        configFile << "# 目标[类型] == " << Aim.select_mode << "\n";
        configFile << "# 锁定[参数] == " << (unlock1 ? "开" : "关") << "\n";
        configFile << "# 显示[目标] == " << (Aim.Showtarget ? "开" : "关") << "\n";
        configFile << "# 双指[触发] == " << (Aim.trigger ? "开" : "关") << "\n";
        configFile << "# 充电[方向] == " << (Aim.Leftport ? "开" : "关") << "\n";        
        configFile << "# 预判[幅度] == " << Aim.Predictionsed << "\n";
        
        configFile << "# 自瞄[技能X] == " << Aim.TriggerpositionX << "\n";
        configFile << "# 自瞄[技能Y] == " << Aim.TriggerpositionY << "\n";
        
        
        
        
        configFile << "\nint //颜色配置//\n";
        configFile << fixed << setprecision(6); // 设置格式
        configFile << "# 一技能颜色 = R: " << config.Skill1Color.x << " G: " << config.Skill1Color.y << " B: " << config.Skill1Color.z << " A: " << config.Skill1Color.w << "\n";
        configFile << "# 二技能颜色 = R: " << config.Skill2Color.x << " G: " << config.Skill2Color.y << " B: " << config.Skill2Color.z << " A: " << config.Skill2Color.w << "\n"; 
        configFile << "# 三技能颜色 = R: " << config.Skill3Color.x << " G: " << config.Skill3Color.y << " B: " << config.Skill3Color.z << " A: " << config.Skill3Color.w << "\n";
        configFile << "# 血量[暴露] = R: " << config.color1.x << " G: " << config.color1.y << " B: " << config.color1.z << " A: " << config.color1.w << "\n";
        configFile << "# 血量[安全] = R: " << config.color2.x << " G: " << config.color2.y << " B: " << config.color2.z << " A: " << config.color2.w << "\n";
        configFile << "# 射线[射线] = R: " << config.color3.x << " G: " << config.color3.y << " B: " << config.color3.z << " A: " << config.color3.w << "\n";
        configFile << "# 方框[方框] = R: " << config.color4.x << " G: " << config.color4.y << " B: " << config.color4.z << " A: " << config.color4.w << "\n";
        configFile << "# 实体[大招] = R: " << config.color5.x << " G: " << config.color5.y << " B: " << config.color5.z << " A: " << config.color5.w << "\n";
        configFile << "# 实体[技能] = R: " << config.color6.x << " G: " << config.color6.y << " B: " << config.color6.z << " A: " << config.color6.w << "\n";
        configFile << "# 兵线[圆点] = R: " << config.color7.x << " G: " << config.color7.y << " B: " << config.color7.z << " A: " << config.color7.w << "\n";
        configFile << "# 野怪[圆点] = R: " << config.color8.x << " G: " << config.color8.y << " B: " << config.color8.z << " A: " << config.color8.w << "\n";
        configFile << "# 野怪[血量] = R: " << config.color9.x << " G: " << config.color9.y << " B: " << config.color9.z << " A: " << config.color9.w << "\n";
        configFile << "# 野怪[陷阱] = R: " << config.color10.x << " G: " << config.color10.y << " B: " << config.color10.z << " A: " << config.color10.w << "\n";
        configFile << "# 视野[暴露] = R: " << config.color11.x << " G: " << config.color11.y << " B: " << config.color11.z << " A: " << config.color11.w << "\n";
        configFile << "# 自瞄[落点] = R: " << config.color12.x << " G: " << config.color12.y << " B: " << config.color12.z << " A: " << config.color12.w << "\n";
        
        configFile << "\nint //使用驱动//\n";
        configFile << "# 驱动[类型] == " << Drivermodel << "\n";
        configFile << "/* \n等于0 -- RT驱动\n 等于1 -- RT新节点驱动\n 等于2 -- RTHOOK驱动 \n 等于3 -- QX11.4驱动 \n 等于4 -- GT2.2驱动\n 等于5 -- DitPro驱动\n 等于6 -- 纯C读写\n*/" << "\n";
        
        
     //   Aim.Skillsize = Aim.Skillsize;   // 把配置值写进去

        TriggerArea.x = Aim.TriggerpositionX;
        TriggerArea.y = Aim.TriggerpositionY;
        FlyTriggerArea.x = Fly.X;
        FlyTriggerArea.y = Fly.Y;
        configFile.close();
    }
}


/*// ...

// 以下是在程序中使用配置的示例
int main() {
    loadConfig(); // 加载配置

    // 根据配置执行程序逻辑
    if (config.Hide) {
        // 如果Hide为true，则隐藏窗口
    }

    // ...

    saveConfig(); // 保存配置
    return 0;
}
*/

#endif // CONFING_H
