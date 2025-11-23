//QQ交流群1027850475
//by风华绝代
//倒卖死妈
#include "draw.h"

#include "My_font/zh_Font.h"
#include "My_font/fontawesome-brands.h"
#include "My_font/fontawesome-regular.h"
#include "My_font/fontawesome-solid.h"
#include "My_font/gui_icon.h"
   
#include "My_icon/pic_ZhenAiKun_png.h"
//#include "kernel.h"
#include "sys读写.h"
//#include "变量.h"
bool permeate_record = false;
bool permeate_record_ini = false;
struct Last_ImRect LastCoordinate = {0, 0, 0, 0};


// 全局变量定义（放在文件顶部）

// 在文件顶部添加
#include <chrono>
#include <thread>
#include <mutex>
#include <vector>
#include <tuple>
#include <mutex>  // 添加这行

// 修改后的全局变量声明
//extern std::mutex g_PlayerDataMutex;

// 在draw_Gui.cpp顶部添加


// 在全局变量区域添加：
static bool 绘制触摸 = true;
static float sdX = 0.0f; 
static float sdY = 0.0f;
static ImU32 Paint_chired = IM_COL32(255, 0, 0, 150); // 红色半透明

// 在Layout_tick_UI函数末尾（ImGui::End()之后）添加：




std::unique_ptr<AndroidImgui> graphics;
ANativeWindow *window = NULL; 
android::ANativeWindowCreator::DisplayInfo displayInfo;// 屏幕信息
ImGuiWindow *g_window = NULL;// 窗口信息
int abs_ScreenX = 0, abs_ScreenY = 0;// 绝对屏幕X _ Y
int native_window_screen_x = 0, native_window_screen_y = 0;

TextureInfo Aekun_image{};

ImFont* zh_font = NULL;
ImFont* icon_font_0 = NULL;
ImFont* icon_font_1 = NULL;
ImFont* icon_font_2 = NULL;



bool M_Android_LoadFont(float SizePixels) {
    ImGuiIO &io = ImGui::GetIO();
    
    //ImFontConfig config; //oppo字体部分
    //config.FontDataOwnedByAtlas = false;
    //config.SizePixels = SizePixels;
    //config.OversampleH = 1;
    //::zh_font = io.Fonts->AddFontFromMemoryTTF((void *)OPPOSans_H, OPPOSans_H_size, 0.0f, &config, io.Fonts->GetGlyphRangesChineseFull());    
    ////io.Fonts->AddFontDefault(&config);

	static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.OversampleH = 3.0;
    icons_config.OversampleV = 3.0;		
    icons_config.SizePixels = SizePixels;
    //icons_config.GlyphOffset.y += 7.0f; // 通过 GlyphOffset 调整单个字形偏移量，向下偏移 size 像素
	::icon_font_0 = io.Fonts->AddFontFromMemoryCompressedTTF((const void *)&font_awesome_brands_compressed_data, sizeof(font_awesome_brands_compressed_data), 0.0f, &icons_config, icons_ranges);
	::icon_font_1 = io.Fonts->AddFontFromMemoryCompressedTTF((const void *)&font_awesome_regular_compressed_data, sizeof(font_awesome_regular_compressed_data), 0.0f, &icons_config, icons_ranges);
	::icon_font_2 = io.Fonts->AddFontFromMemoryCompressedTTF((const void *)&font_awesome_solid_compressed_data, sizeof(font_awesome_solid_compressed_data), 0.0f, &icons_config, icons_ranges);

    io.Fonts->AddFontDefault();
    return zh_font != nullptr;
}
void init_My_drawdata() {
    ImGui::StyleColorsLight(); //白色
    ImGui::My_Android_LoadSystemFont(25.0f); //(加载系统字体 安卓15完美适配)
    M_Android_LoadFont(25.0f); //加载字体(还有图标)
    ImGui::GetStyle().ScaleAllSizes(3.25f);
    ::Aekun_image = graphics->LoadTextureFromMemory((void *)picture_ZhenAiKun_PNG_H, sizeof(picture_ZhenAiKun_PNG_H));
}


void screen_config() {
    ::displayInfo = android::ANativeWindowCreator::GetDisplayInfo();
}

void drawBegin() {
    if (::permeate_record_ini) {
        LastCoordinate.Pos_x = ::g_window->Pos.x;
        LastCoordinate.Pos_y = ::g_window->Pos.y;
        LastCoordinate.Size_x = ::g_window->Size.x;
        LastCoordinate.Size_y = ::g_window->Size.y;

        graphics->Shutdown();
        android::ANativeWindowCreator::Destroy(::window);
        ::window = android::ANativeWindowCreator::Create("test_sysGui", native_window_screen_x, native_window_screen_y, permeate_record);
        graphics->Init_Render(::window, native_window_screen_x, native_window_screen_y);
        ::init_My_drawdata(); //初始化绘制数据
    } 


    static uint32_t orientation = -1;
    screen_config();
    if (orientation != displayInfo.orientation) {
        orientation = displayInfo.orientation;
        Touch::setOrientation(displayInfo.orientation);
        if (g_window != NULL) {
            g_window->Pos.x = 100;
            g_window->Pos.y = 125;        
        }        
        //cout << " width:" << displayInfo.width << " height:" << displayInfo.height << " orientation:" << displayInfo.orientation << endl;
    }
}


std::unordered_map<int, std::string> heroNames = {
// s10
    //一费卡
  {11190,"[库奇"},{11191,"[莉莉娅"},{11192,"[亚索"},{11193,"[塔姆"},{11194,"[奥拉夫"},{11195,"[奎桑提"},{11196,"[塔里克"},{11197,"[娜美"},{11198,"[凯南"},{11199,"[伊芙琳"},{11200,"[蔚"},{11201,"[金克丝"},{11202,"[安妮"},
 // --2费
  {12177,"[厄斐琉斯"},{12178,"[萨勒芬妮"},{12179,"[卡莎"},{12180,"[凯尔"},{12181,"[纳尔"},{12183,"[卡特琳娜"},{12184,"[潘森"},{12185,"[古拉加斯"},{12186,"[贾克斯"},{12187,"[赛纳"},{12188,"[图奇"},{12191,"[盖伦"},{12192,"[巴德"},
 // --3费
  {13194,"[厄加特"},{13195,"[永恩"},{13196,"[阿木木"},{13197,"[璐璐"},{13198,"[莫德凯撒"},{13199,"[拉克丝"},{13200,"[锐雯"},{13201,"[厄运小姐"},{13202,"[莎弥拉"},{13203,"[瑟提"},{13206,"[妮蔻"},{13207,"[艾克"},{13208,"[薇古丝"},
// --4费
  {14185,"[崔斯特"},{14186,"[扎克"},{14187,"[阿狸"},{14188,"[伊泽瑞尔"},{14189,"[卡尔萨斯"},{14190,"[阿卡丽"},{14191,"[劫"},{14192,"[波比"},{14194,"[凯特琳"},{14195,"[锤石"},{14197,"[佛耶戈"},{14198,"[布里茨"},
 // --5费
  {15158,"[奇亚娜"},{15160,"[吉格斯"},{15161,"[凯隐"},{15162,"[约里克"},{15163,"[卢锡安"},{15164,"[烬"},{15165,"[俄洛伊"},{15168,"[娑娜"},{15171,"[伊莉丝"},{15172,"[阿萝拉"},{15173,"[格温"},
  //s15
  //1
  {11290,"[亚克托斯"},{11291,"[芮尔"},{11292,"[盖伦"},{11293,"[扎克"},{11294,"[纳亚菲利"},{11295,"[纳尔"},{11296,"[墨菲特"},{11297,"[辛德拉"},{11298,"[卢锡安"},{11299,"[凯南"},{11300,"[卡莉斯塔"},{11301,"[希维尔"},{11302,"[伊泽瑞尔"},{11303,"[凯尔"},
//2
{12380,"[普朗克"},{12381,"[慎"},{12382,"[可酷伯"},{12383,"[洛"},{12384,"[赵信"},{12385,"[蔚"},{12386,"[蒙多"},{12387,"[卡莎"},{12388,"[卡特琳娜"},{12389,"[霞"},{12390,"[烬"},{12391,"[拉克斯"},{12392,"[迦娜"},
//3
{13290,"[佛耶戈"},{13291,"[乌迪尔"},{13292,"[斯维因"},{13293,"[亚索"},{13294,"[杰斯"},{13295,"[妮蔻"},{13296,"[阿狸"},{13297,"[赛娜"},{13298,"[吉格斯"},{13299,"[凯特琳"},{13300,"[德莱厄斯"},{13301,"[玛尔扎哈"},{13302,"[璐璐"},
//4
{14280,"[蕾欧娜"},{14281,"[瑟提"},{14282,"[阿卡丽"},{14283,"[波比"},{14284,"[嘉文四世"},{14285,"[沃利贝尔"},{14286,"[奎桑提"},{14287,"[艾希"},{14288,"[莎弥拉"},{14289,"[瑞兹"},{14290,"[卡尔玛"},{14291,"[金克丝"},{14292,"[悠米"},
//5
{15241,"[布隆"},{15242,"[格温"},{15243,"[永恩"},{15244,"[崔斯特"},{15245,"[萨勒芬妮"},{15246,"[婕拉"},{15247,"[韦鲁斯"},{15251,"[李青"},
    
//  s9
  
  
  {11170,"[雷克顿"},{11171,"[沙弥拉"},{11172,"[卡西奥佩娅"},{11173,"[崔丝塔娜"},{11174,"[烬"},{11175,"[波比"},{11176,"[凯尔"},{11177,"[奥利安娜"},{11178,"[佛耶戈"},{11179,"[玛尔扎哈"},{11180,"[艾瑞莉娅"},{11181,"[茂凯"},{11182,"[科加斯"},

{12159,"[瑟提"},{12160,"[克烈"},{12161,"[提莫"},{12162,"[索拉卡"},{12163,"[艾希"},{12164,"[沃里克"},{12165,"[斯维因"},{12166,"[劫"},{12167,"[蔚"},{12168,"[加里奥"},{12169,"[卡萨丁"},{12170,"[金克丝"},{12171,"[塔莉亚"},

{13175,"[雷克塞"},{13176,"[德莱尔斯"},{13177,"[盖伦"},{13178,"[艾克"},{13179,"[杰斯"},{13180,"[丽桑卓"},{13181,"[塔里克"},{13182,"[阿克尚"},{13183,"[维克兹"},{13185,"[卡莉斯塔"},{13186,"[娑娜"},{13187,"[卡尔玛"},{13188,"[卡特琳娜"},

{14166,"[亚索"},{14167,"[拉克丝"},{14168,"[厄加特"},{14169,"[卡莎"},{14170,"[格温"},{14171,"[内瑟斯"},{14172,"[厄斐琉斯"},{14173,"[阿兹尔"},{14174,"[慎"},{14176,"[嘉文四世"},{13184,"[泽丽"},{14178,"[瑟庄妮"},

{15133,"[阿狸"},{15134,"[黑默丁格"},{15135,"[卑尔维斯"},{15136,"[亚克托斯"},{15137,"[奎桑提"},{15138,"[赛娜"},{15139,"[赛恩"},{15149,"[瑞兹"},

};

std::string getHeroName(int id) {
    auto it = heroNames.find(id);
    if (it != heroNames.end()) {
        return it->second;
    }
    return std::to_string(id);
}



#include <mutex>
std::mutex draw_mutex;  // 全局互斥锁

int DrawInt()
{
//DrawInt();

PACKAGENAME* bm="com.tencent.jkchess";
    pid = getPID(bm);
    
Game = get_module_base(pid,"libil2cpp.so");
long int Game666 = get_module_base(pid,"libil2cpp.so")+0xD9BA000;
long int gamebss=getbss("libil2cpp.so");
//0xD9BA000
// 尝试第一条指针链

//libil2cpp.so:bss[1] + 0x259F0 -> + 0xC0 -> + 0xE0 -> + 0x0 -> + 0x10 -> + 0x240 -> + 0x0



//数组头=getZZ(getZZ(getZZ(getZZ(getZZ(getZZ(Game666+0x259F0)+0xC0)+0xE0)+0x0)+0x10)+0x240)+0x30;


//libil2cpp.so:bss[1]+0x4BCB0+0xB0+0x50+0xA0+0x0

//libil2cpp.so:bss[1][Cb]+0x30638+0xB0+0x0+0x220+0x20+0x36C
数组头=getZZ(getZZ(getZZ(getZZ(gamebss+0x30638)+0xB0)+0x0)+0x220);



printf("%打印真人数组头:%lx\n", 数组头);

//libil2cpp.so:bss[1]+0x1502E0+0xD0+0x138+0x160+0xB0+0x0

指针头 = getZZ(getZZ(getZZ(getZZ(getZZ(getZZ(getZZ(getZZ(Game+0xD9E77E8)+0xB0)+0x8)+0x10)+0xA8)+0x28)+0x80)+0xB0);

五费 = getZZ(指针头+0x10);
四费 = getZZ(指针头+0x80);
三费 = getZZ(指针头+0xf0);
二费 = getZZ(指针头+0x160);
一费 = getZZ(指针头+0x1d0);

return 0;

}

// 全局玩家数据互斥锁
std::mutex g_PlayerDataMutex;
// 玩家数据容器（名称，血量，等级，金币）
std::vector<std::tuple<std::string, int, int, int>> g_PlayerData;
// 玩家窗口显示标志
bool g_ShowPlayerWindow = false;


// 在全局变量区域添加
static int selectedPlayerIndex = -1;  // 当前选中的玩家索引
static std::mutex shop_mutex;        // 商店数据互斥锁
static int shopCards[5] = {0};       // 存储商店中的5张牌

struct ShopData {
  int card1;
  int card2;
  int card3;
  int card4;
  int card5;
  time_t updateTime; // 更新时间戳
};

ShopData GetCurrentShopData() {
  std::lock_guard<std::mutex> lock(shop_mutex);
  return {
      shopCards[0],
      shopCards[1],
      shopCards[2],
      shopCards[3],
      shopCards[4],
      time(nullptr)
  };
}







void PlayerDataUpdateThread() {
    while (true) {
        std::vector<std::tuple<std::string, int, int, int>> tempData;
        long int selfPlayerObj = 0; // 存储自身玩家对象地址
        
        // 遍历内存获取玩家数据（最多9个玩家）
        for (int ii = 0; ii < 9; ii++) {
            // 1. 获取玩家对象指针
            long int playerObj = getZZ(数组头 + (ii * 0x20));
            if (!playerObj) break;
            
            // 2. 读取玩家名称
            char nameBuffer[256] = {0};
            long int namePtr = getZZ(getZZ(playerObj + 0x268) + 0x28) + 0x14;
            getUTF8(nameBuffer, namePtr);

            // 3. 读取玩家属性
            int health = getDword(playerObj + 0x36C);  // 血量
            int level  = getDword(playerObj + 0x370);  // 等级
            int gold   = getDword(playerObj + 0x37C);  // 金币

            // 4. 存储完整玩家数据
            tempData.emplace_back(
                std::string(nameBuffer),  // 玩家名称
                health,                   // 血量
                level,                    // 等级
                gold                      // 金币
            );
            
            // 5. 如果这是选中的玩家，读取商店数据
            if (selectedPlayerIndex == ii) {
                long int shopPtr = getZZ(getZZ(playerObj + 0x40) + 0x1c0) + 0x20;
                if (shopPtr) {
                    std::lock_guard<std::mutex> lock(shop_mutex);
                    shopCards[0] = getDword(shopPtr + 0x0);
                    shopCards[1] = getDword(shopPtr + 0x4);
                    shopCards[2] = getDword(shopPtr + 0x8);
                    shopCards[3] = getDword(shopPtr + 0xC);
                    shopCards[4] = getDword(shopPtr + 0x10);
                }
            }
        }
        
        // 原子化更新全局数据
        {
            std::lock_guard<std::mutex> lock(g_PlayerDataMutex);
            g_PlayerData.swap(tempData);
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}







// 显示棋子库存的通用函数
void DisplayHeroStock(uintptr_t costAddress, const char* costName) {
    // 添加静态变量存储字体大小
    static float fontSize = 1.3f;
    
    if (costAddress != 0) {
        int count = getDword(costAddress + 0x18);
        
        // 添加字体大小调节滑块
        ImGui::SliderFloat("字体大小", &fontSize, 0.5f, 3.0f, "%.1f");
        
        // 使用滑块设置的字体大小
        ImGui::SetWindowFontScale(fontSize);
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%s", costName);
        ImGui::Separator();
        
        uintptr_t listHead = costAddress + 0x30;
        
        // 每行显示5个英雄
        const int itemsPerRow = 5;
        const float itemWidth = 120.0f;   // 增大宽度
        const float itemSpacing = 25.0f;  // 增大间距
        
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(itemSpacing, 15));
        
        for (int i = 0; i < count; i++) {
            long int current = getZZ(listHead + (i * 0x20));
            if (current == 0) continue;
            
            int charId = getDword(current + 0x10);
            int currentStock = getDword(current + 0x1C);
            int maxStock = getDword(current + 0x20);
            
            // 使用 getHeroName 获取英雄名称
            std::string heroName = getHeroName(charId);
            
            // 开始每个英雄的显示组
            ImGui::BeginGroup();
            ImGui::PushItemWidth(itemWidth);
            
            // 英雄名称（白色加粗）
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%s", heroName.c_str());
            
            // 库存数量（白色）
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%d/%d", currentStock, maxStock);
            
            ImGui::PopItemWidth();
            ImGui::EndGroup();
            
            // 每行显示5个后换行
            if ((i + 1) % itemsPerRow != 0 && i != count - 1) {
                ImGui::SameLine(0, itemSpacing);
            }
        }
        
        ImGui::PopStyleVar();
        ImGui::SetWindowFontScale(1.0f); // 恢复默认字体大小
    } else {
        ImGui::SetWindowFontScale(fontSize);
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s地址无效!", costName);
        ImGui::SetWindowFontScale(1.0f);
    }
}

#include <thread>
#include <chrono>  // 添加 chrono 头文件

void UpdateAddress() {
    auto AdrThread = new std::thread([]{
        while(true) {
            DrawInt();
            using namespace std::chrono_literals;  // 启用时间字面量
            std::this_thread::sleep_for(500ms);    // 使用毫秒或 0.5s (需要 C++14)
        }
    });
    AdrThread->detach();
}

void UpdateAddress1() {
    auto AdrThread = new std::thread([]{
        while(true) {
            PlayerDataUpdateThread();
            using namespace std::chrono_literals;  // 启用时间字面量
            std::this_thread::sleep_for(500ms);    // 使用毫秒或 0.5s (需要 C++14)
        }
    });
    AdrThread->detach();
}

void Layout_tick_UI(bool* main_thread_flag) {
    // 静态控制变量
    static bool show_inventory_window = false;
    static int current_inventory_page = 0;
static bool g_ShowShopWindow = false;
    // 主窗口
    ImGui::Begin("hi jcc I'm Li.", main_thread_flag);
    
    // 窗口位置记忆
    if (::permeate_record_ini) {
        ImGui::SetWindowPos({LastCoordinate.Pos_x, LastCoordinate.Pos_y});
        ImGui::SetWindowSize({LastCoordinate.Size_x, LastCoordinate.Size_y});
        permeate_record_ini = false;   
    }

    // 基本信息显示
    ImGui::Text("by 李嘉图");
    ImGui::Text("频道:LiJiaTuNB");
    ImGui::Text("倒卖死妈死爸死全家");
    ImGui::Text("渲染接口: %s", graphics->RenderName);
    
  /*  // 过录制选项
    ImGui::SameLine();
    if (ImGui::Checkbox("过录制", &::permeate_record)) {
        ::permeate_record_ini = true;
    }
*/
    // 触摸点显示
    if (绘制触摸) {
        ImGui::GetForegroundDrawList()->AddCircleFilled(
            ImVec2(sdX, sdY), 
            50.0f,
            Paint_chired, 
            32
        );
        绘制触摸 = false;
    }



      if (ImGui::Button("商店刷新")) {
    g_ShowShopWindow = !g_ShowShopWindow;
}

    // 功能按钮区
    if (ImGui::Button("棋子库存")) {
        show_inventory_window = !show_inventory_window;
    }
    ImGui::SameLine();
   if (ImGui::Button("玩家数据")) {
        g_ShowPlayerWindow = !g_ShowPlayerWindow;
    }

    // FPS显示
    ImGui::TextColored(ImVec4(1.0f, 0.5f, 1.0f, 1.0f), "%.1f FPS", ImGui::GetIO().Framerate);

    ImGui::End();

    // 棋子库存窗口
    if (show_inventory_window) {
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.3f));
        ImGui::Begin("棋子库存", &show_inventory_window, ImGuiWindowFlags_AlwaysAutoResize);
        
        // 费用切换按钮
        ImGui::BeginGroup();
        if (ImGui::Button("五费")) current_inventory_page = 0;
        ImGui::SameLine();
        if (ImGui::Button("四费")) current_inventory_page = 1; 
        ImGui::SameLine();
        if (ImGui::Button("三费")) current_inventory_page = 2;
        ImGui::SameLine();
        if (ImGui::Button("二费")) current_inventory_page = 3;
        ImGui::SameLine();
        if (ImGui::Button("一费")) current_inventory_page = 4;
        ImGui::EndGroup();
        
        ImGui::Separator();
        
        // 显示当前费用库存
        switch(current_inventory_page) {
            case 0: DisplayHeroStock(五费, "五费"); break;
            case 1: DisplayHeroStock(四费, "四费"); break;
            case 2: DisplayHeroStock(三费, "三费"); break;
            case 3: DisplayHeroStock(二费, "二费"); break;
            case 4: DisplayHeroStock(一费, "一费"); break;
        }
        
        ImGui::End();
        ImGui::PopStyleColor();
    }

if (g_ShowShopWindow) {
    // 设置大字体（30px）
    const float fontSize = 30.0f;
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]); // 使用大字体
    
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.3f));
    ImGui::Begin("商店牌库监控", &g_ShowShopWindow, ImGuiWindowFlags_AlwaysAutoResize);
    
    // 设置窗口字体大小
    ImGui::SetWindowFontScale(fontSize/15.0f);
    
    // 检查是否有选中的玩家
    if (selectedPlayerIndex == -1) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "请先在玩家窗口中选择一个玩家或自身");
    } else {
        // 加锁获取商店数据
        int currentShop[5];
        {
            std::lock_guard<std::mutex> lock(shop_mutex);
            memcpy(currentShop, shopCards, sizeof(shopCards));
        }
        
        // 显示商店牌库标题
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "当前玩家商店牌库:");
        ImGui::Separator();
        
        // 设置行间距
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 20));
        
        // 显示5张牌的信息
        for (int i = 0; i < 5; i++) {
            // 查找英雄名称
            auto it = heroNames.find(currentShop[i]);
            std::string heroName = (it != heroNames.end()) ? it->second : "未知英雄";
            
            // 根据ID范围确定颜色
            ImVec4 color;
            if (currentShop[i] >= 15230) { // 五费 - 金色
                color = ImVec4(1.0f, 0.84f, 0.0f, 1.0f);
            } else if (currentShop[i] >= 14260) { // 四费 - 紫色
                color = ImVec4(0.8f, 0.44f, 0.99f, 1.0f);
            } else if (currentShop[i] >= 13270) { // 三费 - 蓝色
                color = ImVec4(0.2f, 0.6f, 1.0f, 1.0f);
            } else if (currentShop[i] >= 12360) { // 二费 - 绿色
                color = ImVec4(0.2f, 0.8f, 0.2f, 1.0f);
            } else { // 一费 - 白色
                color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            }
            
            // 显示牌信息（带序号）
            ImGui::Text("牌%d: ", i+1);
            ImGui::SameLine();
            ImGui::TextColored(color, "%s", heroName.c_str());
        }
        
        ImGui::PopStyleVar(); // 恢复行间距
    }
    
    ImGui::End();
    ImGui::PopFont(); // 恢复字体
    ImGui::PopStyleColor();
}


    // 玩家数据窗口（白色字体）
if (g_ShowPlayerWindow) {
    // 使用更大的默认字体（20px）
    const float fontSize = 20.0f;
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]); // 确保已加载大字体
    
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.3f));
    ImGui::Begin("玩家数据监控", &g_ShowPlayerWindow, 
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
    
    // 加锁获取数据
    std::vector<std::tuple<std::string, int, int, int>> displayData;
    {
        std::lock_guard<std::mutex> lock(g_PlayerDataMutex);
        displayData = g_PlayerData;
    }

    // 设置大字体和间距
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(10, fontSize/2));
    ImGui::SetWindowFontScale(fontSize/15.0f);

    // 数据表格（自动适应大字体）
    if (ImGui::BeginTable("##PlayerData", 5,  // 增加一列用于选择
    ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg,
    ImVec2(0, 0)))
{
    // 表头（增加"选择"列）
    ImGui::TableSetupColumn("选择", ImGuiTableColumnFlags_WidthFixed, fontSize * 5);
    ImGui::TableSetupColumn("玩家名称", ImGuiTableColumnFlags_WidthFixed, fontSize * 10);
    ImGui::TableSetupColumn("血量", ImGuiTableColumnFlags_WidthFixed, fontSize * 5);
    ImGui::TableSetupColumn("等级", ImGuiTableColumnFlags_WidthFixed, fontSize * 5);
    ImGui::TableSetupColumn("金币", ImGuiTableColumnFlags_WidthFixed, fontSize * 8);
    ImGui::TableHeadersRow();
    
    // 数据行
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    for (size_t i = 0; i < displayData.size(); i++) {
        const auto& player = displayData[i];
        ImGui::TableNextRow();
        
        // 选择列 - 单选按钮
        ImGui::TableNextColumn();
        if (ImGui::RadioButton(("##PlayerSelect" + std::to_string(i)).c_str(), 
                              selectedPlayerIndex == static_cast<int>(i))) {
            selectedPlayerIndex = static_cast<int>(i);
        }
        
        // 其他数据列
        ImGui::TableNextColumn();
        ImGui::Text("%s", std::get<0>(player).c_str());
        
        ImGui::TableNextColumn();
        ImGui::Text("%d", std::get<1>(player));
        
        ImGui::TableNextColumn();
        ImGui::Text("%d", std::get<2>(player));
        
        ImGui::TableNextColumn();
        ImGui::Text("%d", std::get<3>(player));
    }
    ImGui::PopStyleColor();
    ImGui::EndTable();
}

    
    // 状态信息（同比例放大）
    ImGui::Separator();
    ImGui::Text("玩家数量: %zu | 更新间隔: 500ms", displayData.size());
    
    // 恢复样式
    ImGui::PopStyleVar();
    ImGui::PopFont();
    ImGui::End();
    ImGui::PopStyleColor();
}


}







