/* 
 * 由光头强内核（原XF内核）开源
 因频道账号被盗用后决定开源，正版为光头强内核，
开源文件仅供学习交流，任何个人或组织可在遵循协议条款的前提下，自由下载、使用、修改、分发本项目代码及衍生作品，无需额外授权。使用过程中请保留原项目版权声明及开源协议文本，不得用于违法违规场景。本项目以“现状”提供，开发者不承担任何直接或间接的使用风险及法律责任，亦不提供商业技术支持。
删除此内容死全家，没几把
* 注入时间: 2025-11-16 20:14:49
 * 联系方式: TG:@GTQNHNB666
QQ群：784316342
 */
// 防止Vector3重复定义 - 必须在所有include之前
#define VECTOR3_DEFINED
#ifndef VECTOR3_DEFINED
#define VECTOR3_DEFINED
#endif

// 防止px,py重复定义
#define PX_PY_DEFINED

// 为kernel.h提供Unity格式的Vector3定义
#define Vector3 Vector3Unity

#include "IsCharacter.h"
#include "include.h"
#include "Confing.h"
#include "Aim.h"
#include "TouchTooT.h"
#include <iomanip>
#include <ctime>
#include <cmath>
#include <unordered_map>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define BYTE4 0x00000004
#define BYTE8 0x00000008
#define BYTE16 0x00000010
#define BYTE10 0x000000A
#define BYTE64 0x00000040
#define BYTE1024 0x00000400
// 已移除CACHE_SIZE宏定义（解析功能已删除）

static uint64_t lil2cpp_base = 0;
static uint64_t lil2cpp_bss = 0;
static uint64_t libGame_base = 0;
static uint64_t libGame = 0;
static uint64_t libunity_base = 0;
static uint64_t duan_addr = 0;
static uint64_t rc_dxl_id = 0;  //初始化顶部技能存储血量颜色判断int 数据;
static uint64_t rc_xxl_id = 0;  //初始化小地图存储血量颜色判断
float offset_y = 25.0f; // 向下移动的距离
float thickness = 3.0f; // 长方形条的厚度（高度保持不变，宽度减半）




uint64_t MatrixAddress = 0; // 初始化矩阵地址
float isGames;              // 储存矩阵的值
int foeComp;                // 敌方阵营
int rientation = 0;         // 对局方向
int AroundNumbers = 0;      // 人头数量
int 视野 = 0;
int 自身视野 = 0;
long coordinatex, coordinatey;
long rotatingdraw = 0;
float 距离敌人 = 0;
int 惩戒伤害 = 0;
bool 召唤师技能判断 = false;
uintptr_t 对象坐标;
long zuobiao;
int 坐标X = 0;


int dr;
int dr_x[5];
int dr_y[5];

// Coord结构体已在TouchTooT.h中定义，这里不重复定义
#ifndef COORD_STRUCT_DEFINED
struct Coord {
  float X; // X轴
  float Y; // Y轴
  float W;
  float H;
};
#endif
struct DynamicData {
  struct Coord coord; // 获取xy坐标
};

struct HeroTemp {
  struct Coord coord; // 获取xy坐标
  int Hp;             // 当前血量
  int MaxHp;          // 最大血量
  int Id;             // 英雄id
  int Space;          // 大招cd
  int Space2;          // 2技能cd
  int Space1;          // 1技能cd
  int Space4;          // 4技能cd (特殊英雄)
  int Spaceid;			//大招ID
  int Space2id;			//2技能ID
  int Space1id;			//1技能ID
  int Skill;          // 召唤师技能cd
  int Space3;
  int camp;           // 阵营id
  int HC;             // 回城
  
   int TB; 
   int TB1;  
    int TB2;  
     int TB3; 
      int TB4;                       // 图标             // 图标
  int confound;       // 坐标混淆
};
/**
 * 获取野怪xy和刷新时间
 */
struct Pve {
  uint64_t X; // X坐标地址
  uint64_t Y; // Y坐标地址
  uint64_t id;
  uint64_t cd; // 刷新时间
  uint64_t cc;
  uint64_t maxcd;
  uint64_t hp;
  uint64_t maxhp;
};
/**
 * 获取野怪xy和刷新时间
 */

struct PveTemp {
  struct Coord coord; // 获取xy坐标
  int hp;
  int maxhp;
  int id;
  int cd; // 刷新时间
  int maxcd;
};

/**
     * 获取dataTable.CeTemp[cont].coord.Xy
 */
struct Pvc {
  uint64_t X; // X坐标地址
  uint64_t Y; // Y坐标地址
};
/**
 * 单个兵线
 */
struct CeTemp {
  struct Coord coord; // 获取xy坐标
  int cd;
};


  struct YWTemp {
  struct Coord coord; // 获取xy坐标
  int cd;
};
 
struct YwTemp {
  struct Coord coord; // 获取X坐标
  uint64_t X; // X坐标地址
  uint64_t Y; // Y坐标地址
};

struct kl {
    struct Coord coord;//获取xy坐标
    struct Coord anim_coord;//获取xy坐标
};



typedef struct {
    float Matrix[BYTE16];                   // 相机矩阵
    struct HeroTemp heroTemp[BYTE16];       // 储存单个英雄
    struct DynamicData DynamicData[BYTE16]; // 储存单个英雄大地图
    struct Pve pve[BYTE1024];               // 储存野怪
    struct PveTemp pveTemp[BYTE1024];       // 储存野怪
    struct CeTemp CeTemp[BYTE1024];         // 储存兵线
    struct Pvc pvc[BYTE1024];               // 储存兵线
    struct YWTemp YWTemp[BYTE1024];         // 储存眼位
	struct YwTemp YwTemp[BYTE1024];
    struct kl kl[BYTE1024];
    int xbsl;                               // 小兵数量
    int ywsl; 
} DataTable;

DataTable dataTable; // 游戏数据表

const static ImColor Paint_chired = ImColor(219, 77, 110);   // 赤红
const static ImColor Paint_purple = ImColor(255, 0, 255);    // 紫色
const static ImColor Paint_red = ImColor(255, 0, 0);         // 红色
const static ImColor Paint_white = ImColor(255, 255, 255);   // 白色
const static ImColor Paint_lightblue = ImColor(0, 255, 255); // 浅蓝色
const static ImColor Paint_yellow = ImColor(255, 255, 0);    // 黄色
const static ImColor Paint_green = ImColor(0, 255, 0);       // 绿色
const static ImColor Paint_pink = ImColor(255, 192, 203); // 一个常见的粉色
const static ImColor Paint_orange = ImColor(255, 165, 0);      // 橙色
const static ImColor Paint_blue = ImColor(0, 0, 255);          // 蓝色
const static ImColor Paint_cyan = ImColor(0, 255, 255);        // 青色
const static ImColor Paint_magenta = ImColor(255, 0, 255);     // 品红色
const static ImColor Paint_black = ImColor(0, 0, 0);           // 黑色
const static ImColor Paint_gray = ImColor(128, 128, 128);      // 灰色
const static ImColor Paint_lightgray = ImColor(200, 200, 200); // 浅灰色
const static ImColor Paint_brown = ImColor(165, 42, 42);       // 褐色
const static ImColor Paint_peachpuff = ImColor(255, 218, 185); // 桃色
const static ImColor Paint_gold = ImColor(255, 215, 0);        // 金色
const static ImColor Paint_silver = ImColor(192, 192, 192);    // 银色
const static ImColor Paint_maroon = ImColor(128, 0, 0);        // 栗色
const static ImColor Paint_navy = ImColor(0, 0, 128);          // 海军蓝
const static ImColor Paint_teal = ImColor(0, 128, 128);        // 青绿色
const static ImColor Paint_lime = ImColor(0, 255, 0);          // 酸橙色
const static ImColor Paint_olive = ImColor(128, 128, 0);       // 橄榄绿

double Wwra = 0; // 转小地图算法

struct Coord 自身_coord = {0}; /*实际地图坐标*/
struct Coord map_coord = {0}; /*实际地图坐标*/
struct Coord map_buff = {0};  /*实际地图坐标*/
struct Coord Pvc_coord = {0}; /*实际地图坐标*/
double 计算距离(ImVec2 thisXY, ImVec2 xy) {
  double dx = xy.x - thisXY.x;
  double dy = xy.y - thisXY.y;
  double distance = sqrt(dx * dx + dy * dy);
  return distance;
}


ImVec2 自身坐标;
ImVec2 自身距离;
ImVec2 目标坐标;
ImVec2 野怪坐标;
ImVec2 实体野怪;
ImVec2 自身实体视野;
ImVec2 野怪坐标缓存;

ImTextureID tubiaoid;
ImTextureID tubiaoid2;
ImTextureID tubiaoid3;
ImTextureID tubiaoid4;

long coorpage = 0;
long offset = 0;

long cooroffest = 0;

// CalMatrixMem 函数已在 TouchTooT.h 中定义，这里不重复定义

// ===== 自瞄系统全局变量 =====
Vector2 Hero_coord;         // 敌人坐标
Vector2 Self_coord;         // 自身坐标
int mindex = 9999999;             // 目标索引
float minAngle = 99999.0f;    // 最小角度
int small_hp = 9999999;           // 最小血量
int small_dist = 9999999;         // 最小距离
long AimTarget = 99999999;         // 自瞄目标
bool isInitialized = false; // 初始化标志
float acuteAngle[BYTE16];   // 角度数组
extern Vector2 CurrentPos;         // 当前位置
extern Vector2 FirstPos;           // 初始位置
bool isAimDown = false;     // 自瞄按下状态
int Target = -1;            // 当前目标索引
Vector2 lastUpLoadPos;      // 上次上传位置
extern Vector2 upLoadPos;          // 上传位置
extern Vector2 upLoadPosN;         // 标准化上传位置
extern Vector2 Angle;              // 角度向量
int localplayerCamp = 1;    // 本地玩家阵营
extern Vector2 Predictions[BYTE16]; // 预判位移数组
// TXdx 在后面定义为 const static float，这里不重复定义
struct Coord Coord[BYTE16];   // 坐标数组
struct Coord AimCoorde = {0}; // 自瞄坐标
struct Coord coord = {0};     // 自身坐标变量
struct Coord AIM_coord = {0};

// 英雄配置表（全局配置）
const std::unordered_map<int, std::pair<float, float>> HeroConfig = {
    {196, {2500.0f, 16556.25f}},   // 百里守约
    {175, {1200.0f, 15714.429f}},  // 钟馗
    {157, {1100.0f, 15714.429f}},  // 火舞
    {108, {13665.0f, 15183.339f}},  // 墨子
    {142, {1007.0f, 11715.0f}},    // 安琪拉
    {133, {10000.0f, 12500.0f}},    // 狄仁杰
    {174, {15113.0f, 16792.0f}},    // 虞姬
    {195, {8696.0f, 12422.0f}}      // 百里玄策
};

// 自瞄辅助函数前向声明
void drawAimStatus();
void performSmoothAimTouch(float targetX, float targetY, int targetIndex);

// 自瞄辅助函数
float q2djl_local(Vector2 pos1, Vector2 pos2) {
    float dx = pos1.x - pos2.x;
    float dy = pos1.y - pos2.y;
    return sqrt(dx * dx + dy * dy);
}

// calculateAngleDifference 已在 TouchTooT.h 中定义，这里声明为 extern
extern float calculateAngleDifference(float angle1, float angle2);

bool isInRange_local(Vector2 pos, Vector2 min, Vector2 max) {
    return (pos.x >= min.x && pos.x <= max.x && pos.y >= min.y && pos.y <= max.y);
}

void performTouch(float x, float y) {
    const int touchX = (屏幕方向 == 1) ? x : yxpx - x;
    const int touchY = (屏幕方向 == 1) ? yxpy - y : y;
    
    Touch::Down(touchY, touchX);
    usleep(10000);
    Touch::Up();
}

void Aimcoor(struct Coord& aimCoord) {
    // 将世界坐标转换为屏幕坐标（使用矩阵变换）
    AIM_coord = CalMatrixMem(aimCoord, dataTable.Matrix);
    AIM_coord.X = AIM_coord.X + SmallHPX;
    AIM_coord.Y = AIM_coord.Y + SmallHPY;
}

void performSmoothAimTouch(float targetX, float targetY, int targetIndex) {
    static float aimPrevPosX[BYTE16] = {0};
    static float aimPrevPosY[BYTE16] = {0};
    
    if (targetIndex >= 0 && targetIndex < BYTE16) {
        float finalX = aimPrevPosX[targetIndex] + (targetX - aimPrevPosX[targetIndex]) * Aim.Smoothness;
        float finalY = aimPrevPosY[targetIndex] + (targetY - aimPrevPosY[targetIndex]) * Aim.Smoothness;
        performTouch(finalX, finalY);
        aimPrevPosX[targetIndex] = finalX;
        aimPrevPosY[targetIndex] = finalY;
    }
}

void drawAimStatus() {
    if (!Aim.Start) return;
    
    if (Aim.Pos) {
        ImGui::GetForegroundDrawList()->AddCircle(
            ImVec2(Aim.TriggerpositionX, Aim.TriggerpositionY),
            范围大小,
            IM_COL32(255, 255, 0, 128),
            16,
            2.0f
        );
    }
}

// getXyPos 函数已在 Aim.h 中定义，这里不重复定义

// ===== 全局变量声明（extern）=====
// 声明 object 和 Matrixindex 变量（实际定义在 draw.cpp 中）
extern struct ObjectData object;
extern struct MatrixData Matrixindex;

// 声明配置变量（实际定义在 draw.cpp 中）
extern bool LoadTouch;
extern int Drivermodel;

// 全局变量用于跟踪初始化状态
static bool isGameInitialized = false;
static int currentGamePid = 0;

void DrawInit() {
  // 移除强制退出逻辑，改为可选初始化
  // 这个函数现在只做基本初始化，不强制要求游戏进程
}

// 新增：手动初始化游戏进程的函数
bool InitializeGameProcess() {
  int pid = getPID("com.tencent.tmgp.sgame");
  
  if (pid <= 0 || pid >= 99999) {
    puts("\n未找到游戏进程\n");
    return false;
  }
  
  // 初始化游戏模块
  libGame_base = getModuleBase("libGameCore.so:bss");  
  lil2cpp_base = getModuleBase("libil2cpp.so:bss");
  libGame = getModuleBase("libGameCore.so");
  
  currentGamePid = pid;
  isGameInitialized = true;
  
  printf("\n游戏进程初始化成功！PID: %d\n", pid);
  return true;
}


ImVec2 Lerp(const ImVec2& a, const ImVec2& b, float t) {
    return ImVec2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t);
}

int linearSearch(long arr[], int n, int x) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == x) {
            return 1; // 找到了
        }
    }
    return 0; // 没找到
}
int isApproximate(int num1, int num2, int error) {
    return fabs(num1 - num2) <= error;
}

// 判断数值是否接近数组中的任意一个数
int isApproximateInArray(int num, int array[], int error) {
    for (int i = 0; i < 5; i++) {
        if (isApproximate(num, array[i], error)) {
            return 1; // 找到接近的数
        }
    }
    return 0; // 未找到接近的数
}

std::pair<float, float> smoothPosition(int i, float currentPosX, float currentPosY, 
                                       std::vector<float>& prevPosX, std::vector<float>& prevPosY) {
    float targetPosX = currentPosX;
    float targetPosY = currentPosY;

    if (i < prevPosX.size() && i < prevPosY.size()) {
        // ⭐ 插值系数：0.1 = 非常平滑，0.15 = 平衡，0.3 = 快速跟随
        targetPosX = prevPosX[i] + (currentPosX - prevPosX[i]) * 0.1f;
        targetPosY = prevPosY[i] + (currentPosY - prevPosY[i]) * 0.1f;
    }

    if (i >= prevPosX.size()) {
        prevPosX.resize(i + 1);
    }
    if (i >= prevPosY.size()) {
        prevPosY.resize(i + 1);
    }

    prevPosX[i] = targetPosX;
    prevPosY[i] = targetPosY;

    return std::make_pair(targetPosX, targetPosY);
}


const static float TXdx =yxpy*0.020834f; //(头像)圈大小(22.5)半径
const static float TXdx1 = yxpy * 0.020534f; //(头像)圈大小(22.5)半径
const static float TXdx2 = yxpy * 0.015534f;
const static float soldiers_dx = yxpy * (double)1000.001851851851851851; //(兵)大小(2.0f)半径
const static float CDjs_X = yxpx * 0.337250f; //绘制cd起
const static float CDjs_Y = yxpy * 0.006481f; //绘制cd起点Y(7)
const static float intervalcdX = yxpx * 0.025200f; //60(cd左右间隔)
const static float intervalcdX1 = intervalcdX+140.0f;
const static float intervalcdX2 = intervalcdX+140.0f;
const static float intervalcdX3 = intervalcdX+140.0f;
const static float intervalcdX4 = intervalcdX+140.0f;
const static float intervalcdY = yxpy * 0.032407f+上下间隔; //35(cd上下间隔)
const static float intervalcdY1 = intervalcdY+20.0f;
const static float skills_dx = yxpy * (double)0.006481481481481481;
const static float skills_txt_dx = skills_dx * 4.0f;    //技能CD文字大小
const static float skills_txt_dx1 = skills_dx * 6.0f;
const static float CDjs_X1= yxpx * 0.30325f; //绘制cd起
const static float CDjs_Y1 = yxpy * 0.047777f; //绘制cd起点Y(7)
const static float intervalcdXX = yxpx * 0.027200f; //60(cd左右间隔)
const static float intervalcdYY = yxpy * 0.052407f; //35(cd上下间隔)
const static float skills_dx1 = yxpy * (double)0.006481481481481481;
const static float skills_txt_dx2 = skills_dx * 4.0f;    //技能CD文字大小
const static float 小地图误差 = yxpy * 0.012685185185185185;//--(小地图上下偏移)(13.7)
const static int 小地图误差X = yxpx * 0.05625;//--(小地图上下
 


void DrawBox(ImDrawList* Draw,int id,ImTextureID handId,float hp,float hpb,float maxhp,long 蓝,long 红)
{
// 调试：输出DrawBox被调用的信息
// static int drawBoxCounter = 0;
// if (drawBoxCounter++ % 60 == 0) {
//     printf("[DrawBox调用] id=%d, mindex=%d, Aim.Start=%d, AroundNumbers=%d\n", 
//            id, mindex, Aim.Start, AroundNumbers);
//     fflush(stdout);
// }

//const static float TXdx =yxpy*0.020834f; //(头像)圈大小(22.5)半径
if (蓝 == 257) 
    {rc_xxl_id = id;}
     else
    if (红 == 257) 
    {rc_xxl_id = id;}
    





if (id == rc_xxl_id) 
{
ImVec2 rectMin((int)map_coord.X - (map_coord.H*0.4), (int)map_coord.Y - (map_coord.H*1.1f));
ImVec2 rectMax((int)map_coord.X + (map_coord.H*0.4), (int)map_coord.Y + (map_coord.H*0.1f));
// 被自瞄锁定的目标显示红色方框，否则显示普通颜色
bool isAimTarget = (mindex >= 0 && mindex < AroundNumbers && id == dataTable.heroTemp[mindex].Id && Aim.Start);
ImU32 frameColor = isAimTarget ? ImColor(255, 0, 0, 255) : 颜色配置.方框颜色;
// 调试输出
// static int debugCounter = 0;
// if (debugCounter++ % 60 == 0) {
//     printf("[方框调试] id=%d, mindex=%d, targetId=%d, Aim.Start=%d, isTarget=%d\n", 
//            id, mindex, (mindex >= 0 && mindex < AroundNumbers) ? dataTable.heroTemp[mindex].Id : -1, Aim.Start, isAimTarget);
//     fflush(stdout);
// }
暴露视野不绘 ? void(0) : Draw->AddRect(rectMin, rectMax, frameColor, 0.0f, 0, 2.5f);






float max_hp_percentage = 1.0f;
float current_hp_percentage = (float)hpb / (float)maxhp;
float bar_width = map_coord.H * 0.15f;  //血条宽度
float bar_height = map_coord.H * 1.25f;  //血条长度
float corner_radius = 5.0f;


// 绘制最大血量条填充
暴露视野不绘 ? void(0) : Draw->AddRectFilled(
    ImVec2((int)map_coord.X-20 - bar_width / 2 + corner_radius - (-map_coord.H*0.65f), (int)map_coord.Y + corner_radius - (map_coord.H*1.1f)),
    ImVec2((int)map_coord.X-20 + bar_width / 2 - corner_radius - (-map_coord.H*0.65f), (int)map_coord.Y + bar_height - corner_radius - (map_coord.H*1.2f)),
    Paint_white,
    corner_radius
);

// 计算当前血量条的填充高度和起始位置
float current_hp_fill_height = bar_height * current_hp_percentage;
float fill_correction_factor = 1.1f;  // 初始修正因子为1.35

// 根据敌人剩余血量百分比动态调整修正因子的值
if (current_hp_percentage <= 0.85f) {
    fill_correction_factor = 1.05f;
}

float current_hp_fill_start_y = (int)map_coord.Y + bar_height - current_hp_fill_height - (map_coord.H * fill_correction_factor);

// 绘制当前血量条填充
暴露视野不绘 ? void(0) : Draw->AddRectFilled(
    ImVec2((int)map_coord.X-20 - bar_width / 2 + corner_radius - (-map_coord.H * 0.65f), current_hp_fill_start_y),
    ImVec2((int)map_coord.X-20 + bar_width / 2 - corner_radius - (-map_coord.H * 0.65f), (int)map_coord.Y + bar_height - (map_coord.H * 1.2f)),
    Paint_green,
    corner_radius
);
}
else
{

ImVec2 rectMin((int)map_coord.X - (map_coord.H*0.4), (int)map_coord.Y - (map_coord.H*1.1f));
ImVec2 rectMax((int)map_coord.X + (map_coord.H*0.4), (int)map_coord.Y + (map_coord.H*0.1f));
// 被自瞄锁定的目标显示红色方框，否则显示普通颜色
bool isAimTarget = (mindex >= 0 && mindex < AroundNumbers && id == dataTable.heroTemp[mindex].Id && Aim.Start);
ImU32 frameColor = isAimTarget ? ImColor(255, 0, 0, 255) : 颜色配置.方框颜色;
Draw->AddRect(rectMin, rectMax, frameColor, 0.0f, 0, 2.5f);






float max_hp_percentage = 1.0f;
float current_hp_percentage = (float)hpb / (float)maxhp;
float bar_width = map_coord.H * 0.15f;  //血条宽度
float bar_height = map_coord.H * 1.25f;  //血条长度
float corner_radius = 5.0f;


// 绘制最大血量条填充
Draw->AddRectFilled(
    ImVec2((int)map_coord.X-20 - bar_width / 2 + corner_radius - (-map_coord.H*0.65f), (int)map_coord.Y + corner_radius - (map_coord.H*1.1f)),
    ImVec2((int)map_coord.X-20 + bar_width / 2 - corner_radius - (-map_coord.H*0.65f), (int)map_coord.Y + bar_height - corner_radius - (map_coord.H*1.2f)),
    Paint_white,
    corner_radius
);

// 计算当前血量条的填充高度和起始位置
float current_hp_fill_height = bar_height * current_hp_percentage;
float fill_correction_factor = 1.1f;  // 初始修正因子为1.35

// 根据敌人剩余血量百分比动态调整修正因子的值
if (current_hp_percentage <= 0.85f) {
    fill_correction_factor = 1.05f;
}

float current_hp_fill_start_y = (int)map_coord.Y + bar_height - current_hp_fill_height - (map_coord.H * fill_correction_factor);

// 绘制当前血量条填充
Draw->AddRectFilled(
    ImVec2((int)map_coord.X-20 - bar_width / 2 + corner_radius - (-map_coord.H * 0.65f), current_hp_fill_start_y),
    ImVec2((int)map_coord.X-20 + bar_width / 2 - corner_radius - (-map_coord.H * 0.65f), (int)map_coord.Y + bar_height - (map_coord.H * 1.2f)),
    Paint_green,
    corner_radius
);

}
rc_xxl_id = 0;

}

// Drawcall 函数已在 TouchTooT.h 中定义，这里不重复定义
extern const char* Drawcall(int TB4);

void Drawskill(ImDrawList* Draw,int id,int Space4,int Space3,int Space2,int Space1,int Skill,int TB,float TXdx)
{
// 优化：使用静态缓冲区避免 std::to_string（所有小地图技能CD共用）
static char mapSkillCdText[16];

// 安全检查
if (!Draw || TXdx <= 0 || map_coord.X <= 0 || map_coord.Y <= 0) {
    return;
}

//特殊英雄判断：针对特定英雄ID显示4技能
if(id==125 || id==153 || id==176 || id==179 || id==182 || id==191 || id==225 || id==507 || id==509)
{

if (Space4 > 0) 
{     
snprintf(mapSkillCdText, sizeof(mapSkillCdText), "%d", Space4);
Draw->AddText(NULL, 20.0f, ImVec2((float)map_coord.X+ TXdx*2, (float)map_coord.Y+TXdx*0.8), Paint_lightblue, mapSkillCdText);
} 
else 
{
ImVec2 center((float)map_coord.X + TXdx*2, (float)map_coord.Y+TXdx*1.2); // 圆心坐标
float radius = 10.0f; // 圆的半径
Draw->AddCircleFilled(center, radius, Paint_green, 16);
}

}else
{
if (Space3 > 0) 
{                
snprintf(mapSkillCdText, sizeof(mapSkillCdText), "%d", Space3);
Draw->AddText(NULL, 20.0f, ImVec2((float)map_coord.X+ TXdx*2, (float)map_coord.Y+TXdx*0.7), Paint_gold, mapSkillCdText);
} 
else 
{
ImVec2 center((float)map_coord.X + TXdx*2, (float)map_coord.Y+TXdx*1.2); // 圆心坐标
float radius = 10.0f; // 圆的半径
Draw->AddCircleFilled(center, radius, Paint_red, 16);
}
}
if (Space2 > 0) 
{
snprintf(mapSkillCdText, sizeof(mapSkillCdText), "%d", Space2);
Draw->AddText(NULL, 20.0f, ImVec2((float)map_coord.X+ TXdx*0, (float)map_coord.Y+TXdx*0.7), Paint_peachpuff, mapSkillCdText);
}
else 
{
ImVec2 center((float)map_coord.X + TXdx*0, (float)map_coord.Y+TXdx*1.2); // 圆心坐标
float radius = 10.0f; // 圆的半径
Draw->AddCircleFilled(center, radius, Paint_lightblue, 16);
}
if (Space1 > 0) {               
snprintf(mapSkillCdText, sizeof(mapSkillCdText), "%d", Space1);
Draw->AddText(NULL, 20.0f, ImVec2((float)map_coord.X- TXdx*2, (float)map_coord.Y+TXdx*0.7), Paint_lightblue, mapSkillCdText);
} else {
ImVec2 center((float)map_coord.X - TXdx*2, (float)map_coord.Y+TXdx*1.2); // 圆心坐标
float radius = 10.0f; // 圆的半径
Draw->AddCircleFilled(center, radius, Paint_green, 16);
}

if (Skill > 0) {
snprintf(mapSkillCdText, sizeof(mapSkillCdText), "%d", Skill);
Draw->AddText(NULL, 20.0f, ImVec2((float)map_coord.X-10, (float)map_coord.Y-TXdx*7.2), Paint_lightblue, mapSkillCdText);
} else {

// 召唤师技能文字显示已隐藏
// const char* result = Drawcall(TB);
// if (result && strlen(result) > 0) {
//     Draw->AddText(NULL, 20.0f, ImVec2((int)map_coord.X-TXdx*2, (int)map_coord.Y-TXdx*7.2), Paint_lightblue,result);
// }

}

rc_xxl_id = 0;

}

void DrawTopskill(ImDrawList* Draw,ImTextureID handId,int id,long 蓝,long 红,int opop,float hp,int Space3,int Space2,int Space1,int Skill,int TB_param)
{//顶上技能2
int TB4 = TB_param; // 使用安全的TB数据但保持TB4变量名
int intvalue1 = TB4;
const static float CDjs_X1= yxpx * 0.30325f; //绘制cd起
const static float CDjs_Y1 = yxpy * 0.047777f; //绘制cd起点Y(7)
const static float intervalcdXX = yxpx * 0.027200f; //60(cd左右间隔)
const static float intervalcdYY = yxpy * 0.052407f; //35(cd上下间隔)
const static float skills_dx1 = yxpy * (double)0.006481481481481481;
const static float skills_txt_dx2 = skills_dx * 4.0f;    //技能CD文字大小
const static float TXdx =yxpy*0.020834f; //(头像)圈大小(22.5)半径
const static int 小地图误差X = yxpx * 0.05625;//--(小地图上下
float Theoffset_X = CDjs_X1 +小地图误差X-25 +jinenglanzX;
float Theoffset_Y = CDjs_Y1+jinenglanzY;

float CDdrawXY[9][2] = {
{(float)(Theoffset_X +  intervalcdX2 * opop-60), (float)(Theoffset_Y + intervalcdY * 0.5)},//大招
                  //大招左右                   //头像上下
{(float)(Theoffset_X +  intervalcdX1 * opop-55), (float)(Theoffset_Y + (TXdx) + intervalcdY1 * 1.5)},            //   血量框框                        //大招上下
{(float)(Theoffset_X +  intervalcdX3 * opop+20), (float)(Theoffset_Y + TXdx + intervalcdY1 * 0.3)},//召唤
{(float)(Theoffset_X +  intervalcdX4 * opop-10), (float)(Theoffset_Y + TXdx + intervalcdY1 *1.5)},//一
{(float)(Theoffset_X +  intervalcdX4 * opop+40), (float)(Theoffset_Y + TXdx + intervalcdY1 *1.5)},//
{(float)(Theoffset_X +  intervalcdX4 * opop-120), (float)(Theoffset_Y + TXdx + intervalcdY1 *1.0)},//
{(float)(Theoffset_X +  intervalcdX4 * opop-20), (float)(Theoffset_Y + TXdx + intervalcdY1 *2.85)},//6

};

//头像1/0 01 框框10 01 40 41 大招00 11 一技能30 31 二技能40 41 召唤20 21

//等于数组第二行第1个元素
CDdrawXY[1][0] = CDdrawXY[1][0] + (TXdx/14.0f); // x 二次改变偏移
CDdrawXY[0][1] = CDdrawXY[0][1] + (TXdx) + (TXdx/5.0f);
CDdrawXY[1][1] = CDdrawXY[1][1] + (TXdx) + (TXdx/5.0f);//绘制头像
//CDdrawXY[2][1] = CDdrawXY[2][1] + (TXdx) + (TXdx/5.0f);
CDdrawXY[3][1] = CDdrawXY[3][1] + (TXdx) + (TXdx/5.0f);
CDdrawXY[4][1] = CDdrawXY[4][1] + (TXdx) + (TXdx/5.0f);

Draw->AddImage(handId!= NULL ?  handId: 0, {CDdrawXY[1][0] - TXdx, CDdrawXY[0][1] - TXdx}, {CDdrawXY[1][0] + TXdx, CDdrawXY[0][1] + TXdx});//顶上头像

//顶上方框左右 - 已注释，不需要透明背景

// ImVec2 rect_min = ImVec2(CDdrawXY[1][0] - 35 - dsfkzy, CDdrawXY[0][1] - 30 - dsfkzy); // 调整左上角坐标使方框缩小
// ImVec2 rect_max = ImVec2(CDdrawXY[4][0] + 10 + dsfkzy, CDdrawXY[4][1] + 25 + dsfkzy); // 调整右下角坐标使方框缩小
// 
// ImU32 filled_color = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 0.2f)); // 透明白色填充颜色
// 
// ImU32 border_color = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 0.2f)); // 透明白色边框颜色
// float rounding = 10.0f;
// Draw->AddRect(rect_min, rect_max, border_color, rounding); // 绘制边框
// Draw->AddRectFilled(rect_min, rect_max, filled_color, rounding); // 绘制填充


if (蓝 == 257) {rc_dxl_id = id;} 
else
if (红 == 257) {rc_dxl_id = id;}
if (id == rc_dxl_id) 
{
Draw->AddCircleArc(ImVec2(CDdrawXY[1][0], CDdrawXY[0][1]),TXdx*0.98,ImVec2(0, 360), Paint_white,  0, 5); //白底背景
Draw->AddCircleArc(ImVec2(CDdrawXY[1][0], CDdrawXY[0][1]),TXdx*0.98, ImVec2(0, hp), Paint_red, 0, 5); //绿色圈边
} 
else 
{
Draw->AddCircleArc(ImVec2(CDdrawXY[1][0], CDdrawXY[0][1]),TXdx*0.98,ImVec2(0, 360), Paint_white,  0, 5); //白底背景
Draw->AddCircleArc(ImVec2(CDdrawXY[1][0], CDdrawXY[0][1]), TXdx*0.98, ImVec2(0, hp), Paint_green, 0, 5); //红色圈边
}

rc_dxl_id = 0;

// 大招状态指示器 - 头像正上方的圆点（模仿wz自瞄）
// 检查是否为四技能英雄
bool isFourSkillHero = (id == 125 || id == 153 || id == 176 || id == 179 || id == 182 || id == 191 || id == 225 || id == 507 || id == 509);
int ultimateSkillCD = isFourSkillHero ? Space2 : Space3; // 四技能英雄的大招是Space2，普通英雄是Space3

// 始终显示圆点：CD时红色，可用时绿色
float dotRadius = 8.0f; // 圆点半径
float avatarX = CDdrawXY[1][0]; // 头像X坐标
float avatarY = CDdrawXY[0][1]; // 头像Y坐标
float dotY = avatarY - TXdx - dotRadius - 8; // 头像上方8像素处

// 圆点阴影
Draw->AddCircleFilled(ImVec2(avatarX + 1.0f, dotY + 1.0f), dotRadius, IM_COL32(0, 0, 0, 120), 16);

// 根据CD状态显示不同颜色
if (ultimateSkillCD == 0) {
    // 大招可用：绿色
    Draw->AddCircleFilled(ImVec2(avatarX, dotY), dotRadius, IM_COL32(0, 200, 0, 255), 16);
} else {
    // 大招CD中：红色
    Draw->AddCircleFilled(ImVec2(avatarX, dotY), dotRadius, IM_COL32(255, 59, 48, 255), 16);
}

 //ImU32 Space1Color;
//顶上绘制大招
// 优化：缓存固定文本尺寸（只计算一次）
// static ImVec2 textSize_yi = ImGui::CalcTextSize("一");
// static ImVec2 textSize_er = ImGui::CalcTextSize("二");
static ImVec2 textSize_da = ImGui::CalcTextSize("大");
// 优化：使用静态缓冲区避免 std::to_string（所有技能共用）
static char skillText[16];

// 1技能显示已注释
// if (Space1 > 0) {
//     snprintf(skillText, sizeof(skillText), "%d", Space1);
//     ImVec2 skillTextSize = ImGui::CalcTextSize(skillText);
//     ImVec2 textPosition = ImVec2(CDdrawXY[0][0] - (TXdx/10) - skillTextSize.x / 2, (CDdrawXY[1][1] - (TXdx/1.4)));
//     Draw->AddText(NULL, skills_txt_dx, textPosition, Paint_white, skillText);
// } else {
//     ImVec2 rectMin(CDdrawXY[0][0] - (TXdx / 15+4) - textSize_yi.x / 3, CDdrawXY[1][1] - (TXdx / 5.4) - textSize_yi.y / 3);
//     ImVec2 rectMin1(CDdrawXY[0][0] + (TXdx / 15+4) + textSize_yi.x / 3, CDdrawXY[1][1] + (TXdx / 5.4) + textSize_yi.y / 3);
//
//        ImGui::GetForegroundDrawList()->AddImage(
//                 tubiaoid4 != NULL ? tubiaoid4 :reinterpret_cast<ImTextureID>(技能贴图.头像[0].textureId),
//              rectMin,rectMin1,ImVec2(0, 0), ImVec2(1, 1));





// }

// 2技能显示已注释
// if (Space2 > 0) {
//     snprintf(skillText, sizeof(skillText), "%d", Space2);
//     ImVec2 skillTextSize2 = ImGui::CalcTextSize(skillText);
//     ImVec2 textPosition = ImVec2(CDdrawXY[3][0] - (TXdx/10) - skillTextSize2.x / 2, (CDdrawXY[3][1] - (TXdx/1.4)));
//     Draw->AddText(NULL, skills_txt_dx, textPosition, Paint_white, skillText);
// } else {
//     ImVec2 rectMin(CDdrawXY[3][0] - (TXdx / 10+4) - textSize_er.x / 3, CDdrawXY[3][1] - (TXdx / 5.4) - textSize_er.y / 3);
//     ImVec2 rectMin1(CDdrawXY[3][0] + (TXdx / 10+4) + textSize_er.x / 3, CDdrawXY[3][1] + (TXdx / 5.4) + textSize_er.y / 3);
//
//     ImGui::GetForegroundDrawList()->AddImage(
//         tubiaoid3 != NULL ? tubiaoid3 : reinterpret_cast<ImTextureID>(技能贴图.头像[0].textureId),
//         rectMin, rectMin1, ImVec2(0, 0), ImVec2(1, 1));
// }

// 3技能/大招显示 - 已注释（只保留头像上方绿点）
// if (Space3 > 0) {
//     snprintf(skillText, sizeof(skillText), "%d", Space3);
//     ImVec2 skillTextSize3 = ImGui::CalcTextSize(skillText);
//     ImVec2 textPosition = ImVec2(CDdrawXY[4][0] - (TXdx/10) - skillTextSize3.x / 2, (CDdrawXY[4][1] - (TXdx/1.4)));
//     Draw->AddText(NULL, skills_txt_dx2, textPosition, Paint_white, skillText);
// } else {
//     ImVec2 circleCenter(CDdrawXY[4][0] - (TXdx / 10+4) - textSize_da.x / 3, CDdrawXY[4][1] - (TXdx / 5.4) - textSize_da.y / 3);
//     ImVec2 circleCenter1(CDdrawXY[4][0] + (TXdx / 10+4) + textSize_da.x / 3, CDdrawXY[4][1] + (TXdx / 5.4) + textSize_da.y / 3);
// 
//     ImTextureID tubiaoid = reinterpret_cast<ImTextureID>(技能贴图.头像[0].textureId);
//     ImGui::GetForegroundDrawList()->AddImage(
//         tubiaoid,
//         circleCenter, circleCenter1, ImVec2(0, 0), ImVec2(1, 1)); 
//     //上面大招
// }


if (Skill > 0) {
ImTextureID tubiaoid;
if(TB4!=0){
if(TB4==801162){
	tubiaoid = reinterpret_cast<ImTextureID>(
                      技能贴图.头像[intvalue1- 800000]
                          .textureId);

}else{
tubiaoid2 = reinterpret_cast<ImTextureID>(
                      技能贴图.头像[intvalue1 - 80000]
                          .textureId);
}
}else{
tubiaoid2 = reinterpret_cast<ImTextureID>(技能贴图.头像[0].textureId);
}

ImTextureID tubiaoid2 =reinterpret_cast<ImTextureID>(技能贴图.头像[0].textureId);

// 计算召唤师技能位置：头像下方
float summonerX = CDdrawXY[1][0]; // 与头像X坐标相同
float summonerY = CDdrawXY[0][1] + TXdx + TXdx*0.6f + 8; // 头像Y + 头像半径 + 技能半径 + 8像素间距
float summonerSize = TXdx * 0.6f; // 技能图标比头像小

//绘制召唤师技能图片（在头像下方）
Draw->AddImage(tubiaoid != NULL ?  tubiaoid: tubiaoid2, {summonerX - summonerSize, summonerY - summonerSize}, {summonerX + summonerSize, summonerY + summonerSize});


//绘制黑色背景图
ImVec2 circleCenter = {summonerX, summonerY};
float circleRadius = summonerSize;
ImU32 black = IM_COL32(0, 0, 0, 100);
Draw->AddCircleFilled(circleCenter, circleRadius, black, 16);


static char skillCdBuf[16];
snprintf(skillCdBuf, sizeof(skillCdBuf), "%d", Skill);
Draw->AddText(NULL, skills_txt_dx * 0.8f, ImVec2(summonerX - summonerSize*0.5f, summonerY - summonerSize*0.5f), Paint_white, skillCdBuf);
} else {
ImTextureID tubiaoid;
if(TB4!=0){
if(TB4==801162){
tubiaoid = reinterpret_cast<ImTextureID>(
                      技能贴图.头像[intvalue1- 800000]
                          .textureId);
}else{
tubiaoid2 = reinterpret_cast<ImTextureID>(
                      技能贴图.头像[intvalue1 - 80000]
                          .textureId);
}
}else{
tubiaoid =reinterpret_cast<ImTextureID>(技能贴图.头像[0].textureId);
}
ImTextureID tubiaoid2 =reinterpret_cast<ImTextureID>(技能贴图.头像[0].textureId);

// 计算召唤师技能位置：头像下方（与CD中位置相同）
float summonerX = CDdrawXY[1][0];
float summonerY = CDdrawXY[0][1] + TXdx + TXdx*0.6f + 8;
float summonerSize = TXdx * 0.6f;

//绘制召唤师技能图片（在头像下方）
Draw->AddImage(tubiaoid!= NULL ?  tubiaoid: tubiaoid2, {summonerX - summonerSize, summonerY - summonerSize}, {summonerX + summonerSize, summonerY + summonerSize});
}
} 

void Drawentity(ImDrawList* Draw,int id,ImTextureID handId,float hp,float TXdx,long 蓝,long 红)
{
if (蓝 == 257) 
    {rc_xxl_id = id;}
     else
    if (红 == 257) 
    {rc_xxl_id = id;}
    


if (id == rc_xxl_id) 
{
DrawIo[31] ? void(0) : Draw->AddImage(handId!= NULL ?  handId: 0,{(int)map_coord.X - TXdx, (int)map_coord.Y-TXdx}, {(int)map_coord.X + TXdx, (int)map_coord.Y+TXdx});
DrawIo[31] ? void(0) : Draw->AddCircleArc({(int)map_coord.X-TXdx*0, (int)map_coord.Y+TXdx*0}, TXdx*0.98, ImVec2(0, 360), Paint_white, 0, 4.0f);
DrawIo[31] ? void(0) : Draw->AddCircleArc({(int)map_coord.X-TXdx*0, (int)map_coord.Y-TXdx*0}, TXdx*0.98, ImVec2(0, hp), Paint_green, 0, 4.0f);
}
else
{
Draw->AddImage(handId!= NULL ?  handId: 0,{(int)map_coord.X - TXdx, (int)map_coord.Y-TXdx}, {(int)map_coord.X + TXdx, (int)map_coord.Y+TXdx});
Draw->AddCircleArc({(int)map_coord.X-TXdx*0, (int)map_coord.Y+TXdx*0}, TXdx*0.98, ImVec2(0, 360), Paint_white, 0, 4.0f);
Draw->AddCircleArc({(int)map_coord.X-TXdx*0, (int)map_coord.Y-TXdx*0}, TXdx*0.98, ImVec2(0, hp), Paint_green, 0, 4.0f);
}
}

void DrawPlayer() {
  // ⭐数据更新检测（帮助诊断方框刷新率）
  static int drawCounter = 0;
  static int lastSelfX = 0, lastSelfY = 0;
  static int dataChangeCounter = 0;
  
  // 插值系统：解决低更新率导致的卡顿
  static int smoothSelfX = 0, smoothSelfY = 0;  // 平滑后的坐标
  static int targetSelfX = 0, targetSelfY = 0;  // 目标坐标
  static bool interpolationEnabled = true;       // 插值开关
  static float lerpSpeed = 0.15f;                // 插值速度
  
  drawCounter++;
  
  const static float TXdx = yxpy * 0.017534f; //(头像)圈大小(22.5)半径
  const static float soldiers_dx =
      yxpy * (double)0.001851851851851851;           //(兵)大小(2.0f)半径
  const static float CDjs_X = yxpx * 0.337250f;      // 绘制cd起
  const static float CDjs_Y = yxpy * 0.006481f;      // 绘制cd起点Y(7)
  const static float intervalcdX = yxpx * 0.025200f; // 60(cd左右间隔)
  const static float intervalcdY = yxpy * 0.032407f; // 35(cd上下间隔)


//共享
    static std::string 设备id = imei;
    static std::string gameDataStr;
    static std::string character;
    static std::string creeps;
    static std::string soldier;
    
    pxx = ESPMenu.分辨率X / 2;//2712和1220
    pyy = ESPMenu.分辨率Y / 2;
    WwrShare = pxx / 11.2f * 1.574074075;
    float bingxianwwra=1660 / 10.2f * (1.574074075+(jiangea*0.01));   // 间隔



//惩戒依赖
//ImVec2 dr_r[5];

  const static float skills_dx = yxpy * (double)0.006481481481481481;
  const static float skills_txt_dx = skills_dx * 5.0f;   // 技能CD文字大小
  Wwra = yxpy / 10.9f * (1.574074075 + (jiange * 0.01)); // 间隔

/* 
 * 由光头强内核（原XF内核）开源
 因频道账号被盗用后决定开源，正版为光头强内核，
开源文件仅供学习交流，任何个人或组织可在遵循协议条款的前提下，自由下载、使用、修改、分发本项目代码及衍生作品，无需额外授权。使用过程中请保留原项目版权声明及开源协议文本，不得用于违法违规场景。本项目以“现状”提供，开发者不承担任何直接或间接的使用风险及法律责任，亦不提供商业技术支持。
删除此内容死全家，没几把
* 注入时间: 2025-11-16 20:14:49
 * 联系方式: TG:@GTQNHNB666
QQ群：784316342
 */
  
    long temp = driver->read<uint64_t>(lil2cpp_base +  0x519D48); // 
  //矩阵数据cb
  
  // 优化：缓存矩阵地址链，避免3层嵌套
  uint64_t matrixChain = driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>(temp + 0xb8) + 0x0) + 0x10);
  MatrixAddress = matrixChain + 0x128;
  
  isGames = driver->read<float>(MatrixAddress);
  // 判断敌方阵营id
  /*  ?  :   三目运算符  如果大于0就返回2 否则返回1 */
  foeComp = isGames > 0 ? 2 : 1;
  // LOGE("foeComp:%d",foeComp);
  // 判断阵营，获取算法，判断方向
 
  int gameStatus = driver->read<int>(libGame_base + 0x26c4);
  // static int statusCounter = 0;
  // if (statusCounter++ % 60 == 0) { // 每60帧输出一次
  //     printf("【对局状态】gameStatus=%d, libGame_base=0x%lx\n", gameStatus, libGame_base);
  // }
  if (gameStatus != 0) { // 对局判断



    rientation = foeComp == 1 ? -1 : 1;

    static int matrixSyncCounter = 0;
    for (int i = 0; i < 16; i++) {
      dataTable.Matrix[i] = driver->read<float>(MatrixAddress + i * 4);
    }
    if (matrixSyncCounter++ % 2 == 0) {
      for (int i = 0; i < 16; i++) {
        Matrixindex.matrix[i] = dataTable.Matrix[i];
      }
    }

//long temp12 = ReadDword(libGame_base + 0x1E02AA0);
    static long cached_temp1 = 0;
    static long cached_bingxiang8 = 0;
    static long cached_bingxiang1 = 0;
    static int baseAddrUpdateCounter = 0;
    
    if (baseAddrUpdateCounter++ % 30 == 0 || cached_temp1 == 0) {
        cached_temp1 = ReadValue(libGame_base + 0x2720);
        cached_bingxiang8 = ReadValue(ReadValue(cached_temp1 + 0xA8) + 0xC0);
        cached_bingxiang1 = cached_temp1 + 0x118;
    }
    
    long temp1 = cached_temp1;
    long bingxiang8 = cached_bingxiang8;
    long bingxiang1 = cached_bingxiang1;
    
    // 设置自身数组地址
    object.selfarray = bingxiang8;

if (十人) {
	AroundNumbers = 20;
} else {
    AroundNumbers = 10;
}

    // ===== 自瞄系统初始化 =====
    static bool configLoaded = false;
    if (!configLoaded) {
        loadConfig(); // 加载自瞄配置
        
        // 如果配置文件不存在，设置默认值
        if (!Aim.Start) {
            // 设置默认自瞄参数
            Aim.Start = false;  // 默认关闭，需要手动开启
            Aim.distance = 15000.0f;
            Aim.angle = 45.0f;
            Aim.Smoothness = 0.8f;
            Aim.mode = 0;  // 死锁模式
            saveConfig();
            Aim.select_mode = 0;  // 血量优先
            Aim.TriggerpositionX = yxpx * 0.8f;
            Aim.TriggerpositionY = yxpy * 0.8f;
            Aim.Bulletspeed = 12500.0f;
            Aim.vision = false;  // 不限制视野
            Aim.Showtarget = true;  // 显示目标信息
            范围大小 = 91.0f;
        }
        
        configLoaded = true;
    }

	int dr = 0;
    int opop = 0;
    int 控制状态 = 0;
    
    // ===== 读取自身数据（在循环外，每帧更新）=====
    if (object.selfarray != 0) {
        object.SelfId = driver->read<int>(object.selfarray + 0x30); // 自身ID
        
        // 调试血量读取
        // static int hpDebugCounter = 0;
        uint64_t hpAddr = ReadValue(object.selfarray+ 0x168);
        object.SelfHp = driver->read<int>(hpAddr + 0xa8); // 自身血量
        // if (hpDebugCounter++ % 300 == 0) {
        //     printf("[血量调试] selfarray=0x%lx, hpAddr=0x%lx, SelfHp=%d, SelfId=%d\n", 
        //            object.selfarray, hpAddr, object.SelfHp, object.SelfId);
        // }
        // 优化：缓存自身技能基址
        uint64_t selfSkillBase = ReadValue(ReadValue(object.selfarray + 0x150));
        object.SelfSkill = driver->read<int>(ReadValue(ReadValue(selfSkillBase +0x150)+0xF8)+0x3C) / 8192000; // 自身召唤师技能
        object.SelfSkillId = driver->read<int>(ReadValue(ReadValue(selfSkillBase + 0x150) + 0xC8) + 0x10); // 自身召唤师技能ID
        object.SelfSpace2 = driver->read<int>(ReadValue(ReadValue(selfSkillBase + 0xF0)+0xF8)+0x3C) / 8192000; // 自身二技能
        object.SelfSpace3 = driver->read<int>(ReadValue(ReadValue(selfSkillBase +0x108)+0xF8)+0x3C) / 8192000; // 自身三技能
        
        // 优化：缓存自身坐标地址
        uint64_t selfCoordAddr = ReadValue(ReadValue(ReadValue(ReadValue(object.selfarray +0x248) +0x10)+0x0)+0x60);
        int rawSelfX = driver->read<int>(selfCoordAddr);
        int rawSelfY = driver->read<int>(selfCoordAddr + 0x8);
        
        // ⭐检测坐标是否变化
        if (rawSelfX != lastSelfX || rawSelfY != lastSelfY) {
            dataChangeCounter++;
            // 数据变化时，更新目标坐标
            targetSelfX = rawSelfX;
            targetSelfY = rawSelfY;
            lastSelfX = rawSelfX;
            lastSelfY = rawSelfY;
        }
        
        // ⭐插值计算：平滑过渡到目标位置
        if (interpolationEnabled) {
            // 线性插值公式：current = current + (target - current) * speed
            smoothSelfX += (int)((targetSelfX - smoothSelfX) * lerpSpeed);
            smoothSelfY += (int)((targetSelfY - smoothSelfY) * lerpSpeed);
            
            // 使用平滑后的坐标
            coord.SelfX = smoothSelfX;
            coord.SelfY = smoothSelfY;
        } else {
            // 不使用插值，直接使用原始坐标
            coord.SelfX = rawSelfX;
            coord.SelfY = rawSelfY;
            smoothSelfX = rawSelfX;
            smoothSelfY = rawSelfY;
        }
        
        // 实时显示数据更新率（改为每帧都显示）
        // static int lastDrawCounter = 0;
        // if (drawCounter - lastDrawCounter >= 1) {
        //     float dataUpdateRate = (dataChangeCounter / (float)(drawCounter - lastDrawCounter + 120)) * 100.0f;
        //     char debugInfo[256];
        //     snprintf(debugInfo, sizeof(debugInfo), 
        //         "【游戏数据】更新率: %.1f%% | 原始: (%d,%d) | 平滑: (%d,%d) | 插值: %s", 
        //         dataUpdateRate, rawSelfX, rawSelfY, smoothSelfX, smoothSelfY,
        //         interpolationEnabled ? "开启" : "关闭");
        //     
        //     // 显示在更显眼的位置
        //     ImGui::GetForegroundDrawList()->AddRectFilled(
        //         ImVec2(5, 35), ImVec2(750, 65), IM_COL32(0, 0, 0, 180));
        //     ImGui::GetForegroundDrawList()->AddText(
        //         ImVec2(10, 40), IM_COL32(255, 255, 0, 255), debugInfo);
        // }
        
        // 每120帧重置计数器
        // if (drawCounter % 120 == 0) {
        //     dataChangeCounter = 0;
        //     lastDrawCounter = drawCounter;
        // }
        
        // 读取阵营
        localplayerCamp = driver->read<int>(object.selfarray + 0x3C);
        
        // 充电器左手模式阵营判断逻辑
        if (Aim.Leftport) {
            // 充电器在左侧：交换阵营判断
            if (localplayerCamp == 2) {
                localplayerCamp = 1;
            } else {
                localplayerCamp = -1;
            }
        } else {
            // 充电器在右侧（默认）：正常阵营判断
            if (localplayerCamp == 2) {
                localplayerCamp = -1;
            } else {
                localplayerCamp = 1;
            }
        }
        
        // ===== 集中处理范围圈绘制（在遍历敌人之前绘制一次）=====
        const Vector2A selfPos(coord.SelfX, coord.SelfY);
        const ImColor rangeColor(255, 0, 0, 255);
        
        if(config.behead && config.Displayrange) {
            范围圈(selfPos, rangeColor, config.Killdistance, 2.5f);
        }
        if(config.Wildiscipline && config.Wildisciplineange) {
            范围圈(selfPos, rangeColor, config.Punishment, 2.5f);
        }
        if(config.purification && config.purificationange) {
            范围圈(selfPos, rangeColor, config.purdistance, 2.5f);
        }
        if(Aim.Start && Aim.scope) {
            范围圈(selfPos, rangeColor, Aim.distance, 2.5f);
        }
    }
    
    // ⭐修复Bug：prevPosX/Y必须是static，否则每帧都被重置为0！
    static std::vector<float> prevPosX(20, 0.0f);
    static std::vector<float> prevPosY(20, 0.0f);
    
    for (int i = 0; i < AroundNumbers; i++) {
        
        // 初始化重置
        config.distance[i] = 0;
        Predictions[i].zero();
        
        // 设置英雄地址
        object.Heroaddr[i] = ReadValue(ReadValue(ReadValue(libGame_base + 0x2720) + 0x118 + i * 0x18)+0x68);
        
        // 读取自身技能1
        object.SelfSpace1 = driver->read<int>(ReadValue(ReadValue(ReadValue(object.Heroaddr[i]+0x150) + 0xD8)+0xF8)+0x3C) / 8192000;
        
        // 读取自身视野判断
        object.SelfVisual = driver->read<int>(ReadValue(ReadValue(object.selfarray + 0x260)+0x68) + (foeComp == 2 ? 0x38 : 0x18));
        
        // 读取阵营
        object.camp = driver->read<int>(object.Heroaddr[i] + 0x3C);
	  
	  
        long bingxiang6 = object.Heroaddr[i]; // 使用已设置的英雄地址
      



      int pand = 1;

      if (pand > 0) {
        int zhengxing = driver->read<int>(bingxiang6 + 0x3C);

        // 优化：缓存技能管理器基址，后续所有技能读取都可复用
        uint64_t skillManagerBase = ReadValue(bingxiang6 + 0x150);
        uint64_t summonerSkillBase = ReadValue(skillManagerBase + 0x150);
        dataTable.heroTemp[i].Skill =driver->read<int>(ReadValue(summonerSkillBase +0xf8) +0x3c) /8192000;
        dataTable.heroTemp[i].TB = driver->read<int>(ReadValue(summonerSkillBase + 0xC8) +0x10);
       

        static bool idInitialized[20] = {false};
        static int idResetCounter = 0;
        
        if (idResetCounter++ % 1000 == 0) {
            for (int j = 0; j < 20; j++) {
                idInitialized[j] = false;
            }
        }
        
        if (!idInitialized[i]) {
            dataTable.heroTemp[i].Id = ReadValue(bingxiang6 + 0x30);
            if (dataTable.heroTemp[i].Id > 0) {
                idInitialized[i] = true;
            }
        }
        
        // 优化：缓存血量基址，避免重复读取
        uint64_t hpBaseAddr = ReadValue(bingxiang6 + 0x168);
        dataTable.heroTemp[i].Hp = driver->read<int>(hpBaseAddr + 0xA8);
        dataTable.heroTemp[i].MaxHp = driver->read<int>(hpBaseAddr + 0xb0);
        
        
        
        

        //
        dataTable.DynamicData[i].coord.X =
            (dataTable.heroTemp[i].coord.X * rientation * Wwra / 50000.0f +
             Wwra);
        dataTable.DynamicData[i].coord.Y =
            (dataTable.heroTemp[i].coord.Y * rientation * Wwra / 50000.0f * -1 +
             Wwra);

        float pos_x = dataTable.DynamicData[i].coord.X + SmallMapX + 93;
        float pos_y = dataTable.DynamicData[i].coord.Y + SmallMapY + 6;

        map_coord = CalMatrixMem(dataTable.heroTemp[i].coord, dataTable.Matrix);
        map_coord.X = map_coord.X + SmallHPX;
        map_coord.Y = map_coord.Y + SmallHPY;

        if (zhengxing != foeComp) {
          // 优化：缓存队友坐标地址
          uint64_t teamCoordAddr = ReadValue(ReadValue(ReadValue(ReadValue(bingxiang6 + 0x248) + 0x10)) +0x60);
          float 坐标x = (float)driver->read<int>(teamCoordAddr + 0x0);
          float 坐标y = (float)driver->read<int>(teamCoordAddr + 0x8);
          
          坐标X = (int)坐标x;
          
          // ⭐添加插值：队友的坐标也要平滑
          float currentPosX = (float)坐标x;
          float currentPosY = (float)坐标y;
          
          // 保存上一帧坐标
          if (i < prevPosX.size()) {
              float 人物跨度X = std::abs(currentPosX - prevPosX[i]);
              float 人物跨度Y = std::abs(currentPosY - prevPosY[i]);
              
              // 跨度>1000才跳过插值
              if (人物跨度X > 1000 || 人物跨度Y > 1000) {
                  dataTable.heroTemp[i].coord.X = (int)currentPosX;
                  dataTable.heroTemp[i].coord.Y = (int)currentPosY;
              } else {
                  // 使用插值
                  std::pair<float, float> smoothedCoords = smoothPosition(i, currentPosX, currentPosY, prevPosX, prevPosY);
                  dataTable.heroTemp[i].coord.X = (int)smoothedCoords.first;
                  dataTable.heroTemp[i].coord.Y = (int)smoothedCoords.second;
              }
          } else {
              // 第一次，直接赋值
              dataTable.heroTemp[i].coord.X = (int)currentPosX;
              dataTable.heroTemp[i].coord.Y = (int)currentPosY;
          }

          float 自身判断 = 计算距离(
              ImVec2(yxpx / 2, yxpy / 2),
              ImVec2(map_coord.X, map_coord.Y + (-map_coord.H * 1.0f)));
          

          if (自身判断 <= 100 || 召唤师ID == dataTable.heroTemp[i].Id) {
            
            召唤师技能ID = dataTable.heroTemp[i].TB;
            召唤师技能 = 获取召唤师技能(dataTable.heroTemp[i].TB);
            召唤师技能CD = dataTable.heroTemp[i].Skill;
            召唤师ID = dataTable.heroTemp[i].Id;         
            召唤师 = PlayerCharacter(dataTable.heroTemp[i].Id);
            自身坐标 = ImVec2(dataTable.heroTemp[i].coord.X, dataTable.heroTemp[i].coord.Y);
            
            // 优化：自身血量和ID已在第1002、1006-1007行读取，删除重复读取
            // object.SelfHp 和 object.SelfId 已经有值了
            
            if(dataTable.heroTemp[i].Id==133)
            {
            召唤师技能CD = dataTable.heroTemp[i].Space;
            }
            //控制状态 = ReadDword(bingxiang6 - 0xB4);
            //沉默1 眩晕2 //击飞3 //冰冻4  正常0
            //控制状态=driver->read<int>(driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>(libGame_base + 0x25C8)+ 0x48) + 0xD8) + 0x108) + 0x110) +0x258);
               uint64_t controlAddr = driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>(libGame_base + 0x25C8)+ 0x48) + 0xD8) + 0x108) + 0x110);
            控制状态 = driver->read<int>(controlAddr + 0x258);
            if (dataTable.heroTemp[i].TB == 80108 ||
                dataTable.heroTemp[i].TB == 80104 ||
                dataTable.heroTemp[i].TB == 80116 ||
                dataTable.heroTemp[i].TB == 80107 ||
                dataTable.heroTemp[i].TB == 801162) {
              
              
              if(召唤师技能CD == 0) {
              	召唤师技能判断 = true;
            } else {
            	召唤师技能判断 = false;
            }
            }
          }
          
          
        }

         if (zhengxing == foeComp) {
    static bool previousState = false;
    static float displayTime = 0.0f;
    const float displayDuration = 3.0f;
    
    if (dataTable.heroTemp[i].Id * 100 + 30 == 16930) {
        // 优化：复用skillManagerBase
        int value = driver->read<int>(ReadValue(ReadValue(skillManagerBase + 0x108) + 0xF8) + 0x3C);
        if (value != 0) {
            if (!previousState) {
                previousState = true;
                displayTime = ImGui::GetTime(); // 记录显示开始时间
            }
            
                // 显示提示语
            if (ImGui::GetTime() - displayTime <= displayDuration) {
                // 设置背景、边框和提示语
                ImU32 rectColor = IM_COL32(121, 170, 236, 150);
                ImGui::GetBackgroundDrawList()->AddRectFilled(
                    ImVec2((int)yxpx / 2 - 450, (int)55),
                    ImVec2((int)yxpx / 2 + 470, (int)145),
                    rectColor, 20, ImDrawFlags_RoundCornersAll);

                ImU32 borderColor = IM_COL32(0, 163, 255, 255);
                ImGui::GetBackgroundDrawList()->AddRect(
                    ImVec2((int)yxpx / 2 - 453, (int)52),
                    ImVec2((int)yxpx / 2 + 473, (int)148),
                    borderColor, 20, ImDrawFlags_RoundCornersAll, 3.f);

                static const char* warningText = "          危险，后羿大招已发射！请注意";
                ImGui::GetForegroundDrawList()->AddText(ImGui::GetFont(), 50, ImVec2((int)yxpx / 2 - 420, (int)70), ImColor(255, 255, 255, 255), warningText);
            } else {
                previousState = true; // 超过显示时间，重置状态
            }
        } else {
            previousState = false;
        }
    }
}

        if (zhengxing == foeComp) { //区分敌我
          
          static int visionCheckCounter = 0;
          uint64_t enemyVisionBase = ReadValue(ReadValue(object.Heroaddr[i] + 0x260)+0x68);
          if (visionCheckCounter++ % 3 == 0) {
              object.visual[i] = driver->read<int>(enemyVisionBase + (foeComp == 2 ? 0x18 : 0x38));
          }
          
          static int stateCheckCounter = 0;
          if (stateCheckCounter++ % 5 == 0) {
              uint64_t stateBase = ReadValue(object.Heroaddr[i] + 0x120);
              object.state[i] = driver->read<int>(stateBase + 0x140);
              object.Displacement[i] = driver->read<int>(object.Heroaddr[i] - 0x430);
              object.speed[i] = driver->read<int>(object.Heroaddr[i] - 0x734);
          }
          
          // 优化：血量已在第1132-1134行读取，删除重复读取
          // dataTable.heroTemp[i].Hp 和 MaxHp 已经有值了
          
          // 优化：缓存坐标地址，避免重复的多层嵌套读取
          uint64_t coordAddr;
          if(dataTable.heroTemp[i].Id == 225) {
              coordAddr = ReadValue(ReadValue(object.Heroaddr[i] +0x230) +0x60);
          } else {
              coordAddr = ReadValue(ReadValue(ReadValue(ReadValue(object.Heroaddr[i] +0x248) +0x10)+0x0)+0x60);
          }
          coord.X = driver->read<int>(coordAddr);
          coord.Y = driver->read<int>(coordAddr + 0x8);
          
          // 关键修复：敌人坐标也要使用插值
          float currentPosX = (float)coord.X;
          float currentPosY = (float)coord.Y;
          
          // 保存上一帧坐标
          if (i < prevPosX.size()) {
              float 人物跨度X = std::abs(currentPosX - prevPosX[i]);
              float 人物跨度Y = std::abs(currentPosY - prevPosY[i]);
              
              // 跨度>1000才跳过插值
              if (人物跨度X > 1000 || 人物跨度Y > 1000) {
                  // 大跨度跳跃，直接更新
                  dataTable.heroTemp[i].coord.X = (int)currentPosX;
                  dataTable.heroTemp[i].coord.Y = (int)currentPosY;
                  prevPosX[i] = currentPosX;
                  prevPosY[i] = currentPosY;
              } else {
                  // 正常移动，使用插值
                  std::pair<float, float> smoothedCoords = smoothPosition(i, currentPosX, currentPosY, prevPosX, prevPosY);
                  dataTable.heroTemp[i].coord.X = (int)smoothedCoords.first;
                  dataTable.heroTemp[i].coord.Y = (int)smoothedCoords.second;
              }
          } else {
              // 第一次，直接赋值并初始化
              dataTable.heroTemp[i].coord.X = (int)currentPosX;
              dataTable.heroTemp[i].coord.Y = (int)currentPosY;
              if (i < prevPosX.size()) {
                  prevPosX[i] = currentPosX;
                  prevPosY[i] = currentPosY;
              }
          }
          
          // 计算距离供自瞄使用
          if(dataTable.heroTemp[i].Hp > 0) {
              config.distance[i] = distance(coord);
          }
          
          static int recallReadCounter = 0;
          if (recallReadCounter++ % 3 == 0) {
              dataTable.heroTemp[i].HC = driver->read<int>(
                  ReadValue(ReadValue(ReadValue(bingxiang6 + 0x150) + 0x168) + 0x168) + 0x20);
          }
          
          // 优化：复用前面已缓存的enemyVisionBase，不再重复读取
          uint64_t selfVisionBaseAddr = driver->read<uint64_t>(driver->read<uint64_t>(bingxiang8 + 0x260) + 0x68);
          
          // 优化：缓存蓝方敌人视野基址
          uint64_t blueVisionBase = ReadValue(ReadValue(bingxiang6 + 0x108)+0x348);
          long 蓝方敌人视野判断 = driver->read<int>(blueVisionBase + 0x38);
          long 红方敌人视野判断 = driver->read<int>(enemyVisionBase + 0x18);
          
          if (zhengxing == 2) {
            视野 = driver->read<int>(enemyVisionBase + 0x18);
            自身视野 = driver->read<int>(selfVisionBaseAddr + 0x38);
          } else {
            视野 = driver->read<int>(enemyVisionBase + 0x38);
            自身视野 = driver->read<int>(selfVisionBaseAddr + 0x18);
          }
          if (自身) {
              if (自身视野 == 257) {
                绘制字体描边(31.0f,
                    ImVec2(屏幕x + yxpx / 2 * 0.970833333333333333,
                           屏幕y + yxpy / 2 * 0.787037037037037037),
                    Paint_red, "[暴露]");
              } else {
                绘制字体描边(31.0f,
                    ImVec2(屏幕x + yxpx / 2 * 0.970833333333333333,
                           屏幕y + yxpy / 2 * 0.787037037037037037),
                    Paint_green, "[安全]");
              }
            }







zuobiao = ReadValue(ReadValue(ReadValue(bingxiang6 + 0x248) + 0x10)) +0x60;
        
             
        
        
// 直接读取坐标地址（已移除解密逻辑）
zuobiao = ReadValue(zuobiao);
 
            prevPosX[i] = (float)dataTable.heroTemp[i].coord.X;
            prevPosY[i] = (float)dataTable.heroTemp[i].coord.Y;
            
          	float 坐标x = (float)driver->read<int>(zuobiao + 0x0);
            float 坐标y = (float)driver->read<int>(zuobiao + 0x8);
            if (坐标x && 坐标y) {
              float currentPosX = (float)坐标x;
              float currentPosY = (float)坐标y;
			  
			  // 跨度检测
			  float 人物跨度X = std::abs(currentPosX - prevPosX[i]);
			  float 人物跨度Y = std::abs(currentPosY - prevPosY[i]);
			  
			  // 跨度>1000才跳过插值
			  if (人物跨度X > 1000 || 人物跨度Y > 1000) {
			  	// 大跨度跳跃（传送、重生），直接更新
			  	dataTable.heroTemp[i].coord.X = currentPosX;
                dataTable.heroTemp[i].coord.Y = currentPosY;
			  } else {
			  	// 正常移动，使用插值
			  	std::pair<float, float> smoothedCoords = smoothPosition(i, currentPosX, currentPosY, prevPosX, prevPosY);
    		  	dataTable.heroTemp[i].coord.X = smoothedCoords.first;
              	dataTable.heroTemp[i].coord.Y = smoothedCoords.second;
            }
            } else {
            dataTable.heroTemp[i].coord.X = 9500;
            dataTable.heroTemp[i].coord.Y = 9500;
            
            }
            // 存储平滑前的坐标
    
        

          目标坐标 = ImVec2(dataTable.heroTemp[i].coord.X,
                            dataTable.heroTemp[i].coord.Y);
          距离敌人 = 计算距离(自身坐标, 目标坐标);
          
          // ===== 自瞄系统数据更新 =====
          if (Aim.Start && i < BYTE16) {
              // 更新距离数据供自瞄使用
              config.distance[i] = 距离敌人;
              
              // 更新坐标数据供自瞄使用
              Hero_coord.x = dataTable.heroTemp[i].coord.X;
              Hero_coord.y = dataTable.heroTemp[i].coord.Y;
              
              // 更新Coord数组供自瞄使用
              Coord[i].X = dataTable.heroTemp[i].coord.X;
              Coord[i].Y = dataTable.heroTemp[i].coord.Y;
              
              // 更新自身坐标
              Self_coord.x = 自身坐标.x;
              Self_coord.y = 自身坐标.y;
          }
          
          //----------------------------自瞄----------------------------\\    
          
          if(Aim.Start) // 自瞄开启
          {
              ImGuiIO& ioooi = ImGui::GetIO();
              
              // 视野判断：只有暴露的敌人才能参与自瞄计算
              bool canAim = true;
              if (Aim.vision && object.visual[i] != 99997) {
                  canAim = false; // 未暴露的敌人不参与自瞄
              }
              
              if (canAim && config.distance[i] < Aim.distance && dataTable.heroTemp[i].Hp > 0) {
                  
                  Vector2 EnemyCoord = {Hero_coord.x, Hero_coord.y};
                  Vector2 MyCoord = {Self_coord.x, Self_coord.y};
                  
                  Vector2 P1 = EnemyCoord - MyCoord;
                  Vector2 P2 = CurrentPos - FirstPos;
                  
                  float AngleP1 = atan2(P1.y, P1.x) * 180.0 / M_PI;
                  float AngleP2 = atan2(P2.y, P2.x) * 180.0 / M_PI;
                  
                  // 优化条件判断，减少嵌套
                  const float threshold = 30.0f;
                  const float angleThreshold = 10.0f;
                  
                  if(fabs(P2.x) > threshold && fabs(P2.y) > threshold && 
                     10.0f < fabs(AngleP1) && fabs(AngleP1) < 170.0f)
                  {
                      // 使用三目运算符简化逻辑
                      if(P2.x > 0)
                          AngleP2 += (P2.y < 0) ? angleThreshold : -angleThreshold;
                      else
                          AngleP2 += (P2.y < 0) ? -angleThreshold : angleThreshold;
                  }
                  
                  // 简化条件赋值
                  acuteAngle[i] = isAimDown ? calculateAngleDifference(AngleP1, AngleP2) : 0;
                  
                  // 优化目标选择逻辑
                  switch(Aim.select_mode)
                  {
                      case 0: // 血量最低
                          if(small_hp <= 0 || small_hp >= dataTable.heroTemp[i].Hp)
                          {
                              small_hp = dataTable.heroTemp[i].Hp;
                              mindex = i;
                          }
                          break;
                          
                      case 1: // 距离最近
                          if(small_dist <= 0 || small_dist >= config.distance[i])
                          {
                              small_dist = config.distance[i];
                              mindex = i;
                          }
                          break;
                  }
              }
          }
          
          dr_x[dr] = dataTable.heroTemp[i].coord.X;
          dr_y[dr] = dataTable.heroTemp[i].coord.Y;
          dr++;





          float hp1 =
              dataTable.heroTemp[i].Hp * 100 / dataTable.heroTemp[i].MaxHp;

          float aa = hp1 * 3.6;
    



        //共享
        // 优化：复用前面缓存的skillManagerBase和summonerSkillBase
        int 大招最大CD = driver->read<int>(ReadValue(ReadValue(skillManagerBase + 0x108) + 0xf8) + 0x3C) / 8192000;
        //    int Space= driver->read<int>(ReadValue(ReadValue(skillManagerBase + 0x108) + 0xf8) + 0x3C) / 8192000;
          
        dataTable.heroTemp[i].TB4 = driver->read<uint64_t>(ReadValue(summonerSkillBase + 0xe0) + 0x10);//召唤师技能



          ImTextureID handId;
          handId = reinterpret_cast<ImTextureID>(贴图1.头像[dataTable.heroTemp[i].Id].textureId);


//共享
          if (ESPMenu.是否开启共享) {
            //启用共享绘制传输变量
          
          int  herealx = (int)(dataTable.heroTemp[i].coord.X * rientation * WwrShare / 50000 + WwrShare);
          int   herealy = (int)(dataTable.heroTemp[i].coord.Y * rientation * WwrShare / 50000 * -1 + WwrShare);

                static char characterBuffer[256];
                snprintf(characterBuffer, sizeof(characterBuffer), 
                    "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d==",
                    dataTable.heroTemp[i].Id,
                    dataTable.heroTemp[i].Hp,
                    dataTable.heroTemp[i].MaxHp,
                    dataTable.heroTemp[i].Space3,
                    dataTable.heroTemp[i].Skill,
                    herealx - 20 + ESPMenu.小地图左右调整,
                    herealy - 20 + ESPMenu.小地图上下调整,
                    hp1,
                    zhengxing,
                    dataTable.heroTemp[i].HC,
                    dataTable.heroTemp[i].TB4
                );
                character += characterBuffer;
                }


          


          if (aa > 0 || dataTable.heroTemp[i].Id == 183) {
            if (血量) {
              if (视野 == 257) {
                if (头像常显 && !暴露不绘小地图头像) {
                  // 小头像血量 - 暴露不绘小地图时也不显示血量
                  ImGui::GetForegroundDrawList()->AddCircleArc(
                      {pos_x, pos_y}, 21 + xiaodituxue, ImVec2(0, 360),
                      Paint_white, 0, 5.5f);
                  ImGui::GetForegroundDrawList()->AddCircleArc(
                      {pos_x, pos_y}, 21 + xiaodituxue, ImVec2(0, aa),
                      颜色配置.血量颜色, 0, 5.5f);
                }
              } else {
                // 小头像血量
                ImGui::GetForegroundDrawList()->AddCircleArc(
                    {pos_x, pos_y}, 21 + xiaodituxue, ImVec2(0, 360),
                    Paint_white, 0, 5.5f);
                ImGui::GetForegroundDrawList()->AddCircleArc(
                    {pos_x, pos_y}, 21 + xiaodituxue, ImVec2(0, aa),
                    颜色配置.无血量颜色, 0, 5.5f);
              }
            }




            rotatingdraw = rotatingdraw + 3;
            if (地图) {
              if (视野 == 257) {
                // 小头像 - 增加暴露不绘小地图头像判断
                if (头像常显 && !暴露不绘小地图头像) {
                  ImGui::GetForegroundDrawList()->AddImage(
                      handId != NULL ? handId : reinterpret_cast<ImTextureID>(技能贴图.头像[0].textureId),
                      {(int)pos_x - TXdx - touxiangdaxiao,
                       (int)pos_y - TXdx - touxiangdaxiao},
                      {(int)pos_x + TXdx + touxiangdaxiao,
                       (int)pos_y + TXdx + touxiangdaxiao});
                }
              } else {
              ImU32 darktx;
              if (暗化) {
              	darktx = IM_COL32(150, 150, 150, 255); // 128表示透明度，你可以根据需要调整
              } else {
              darktx = IM_COL32(255, 255, 255, 255); // 128表示透明度，你可以根据需要调整
              }
                ImGui::GetForegroundDrawList()->AddImage(
                    handId != NULL ? handId : reinterpret_cast<ImTextureID>(技能贴图.头像[0].textureId),
                    {(int)pos_x - TXdx - touxiangdaxiao,
                     (int)pos_y - TXdx - touxiangdaxiao},
                    {(int)pos_x + TXdx + touxiangdaxiao,
                     (int)pos_y + TXdx + touxiangdaxiao}, ImVec2(0, 0), ImVec2(1, 1), darktx);
                     //ImU32 darkColor = IM_COL32(0, 0, 0, 128); // 128表示透明度，你可以根据需要调整



                if (dataTable.heroTemp[i].HC == 1) {
                  ImGui::GetForegroundDrawList()->AddCircleArc(
                      {pos_x, pos_y}, 21 + xiaodituxue,
                      ImVec2(0 + rotatingdraw, 20 + rotatingdraw),
                      Paint_lightblue, 9.5f, 5.5f);
                  ImGui::GetForegroundDrawList()->AddCircleArc(
                      {pos_x, pos_y}, 21 + xiaodituxue,
                      ImVec2(20 + rotatingdraw, 40 + rotatingdraw),
                      Paint_purple, 9.5f, 5.5f);
                  ImGui::GetForegroundDrawList()->AddCircleArc(
                      {pos_x, pos_y}, 21 + xiaodituxue,
                      ImVec2(40 + rotatingdraw, 60 + rotatingdraw),
                      颜色配置.血量颜色, 9.5f, 5.5f);
                }
              }
            }


            
            健康血量 = true;
if(((float)dataTable.heroTemp[i].Hp / (float)dataTable.heroTemp[i].MaxHp) <= zhanshaz) {健康血量 = false;}

const float screenMargin = 100.0f;
if (map_coord.X < -screenMargin || map_coord.X > yxpx + screenMargin ||
    map_coord.Y < -screenMargin || map_coord.Y > yxpy + screenMargin) {
    goto DRAW_TOP_SKILL;
}

if (方框) {
    if (!(暴露视野不绘 && 视野 == 257)) { 
        float current_hp_percentage = (float)dataTable.heroTemp[i].Hp / (float)dataTable.heroTemp[i].MaxHp;
        // 方框尺寸（适配缩小后的头像）
        float boxWidth = map_coord.H * 0.5f;    // 宽度稍减，匹配小头像
        float boxHeight = map_coord.H * 1.8f;   // 纵向保持
        float boxTop = (int)map_coord.Y - (map_coord.H * 1.4f);
        float boxBottom = (int)map_coord.Y + (map_coord.H * 0.4f);

        const float PI = 3.1415926535f;
        const ImVec2 FULL_CIRCLE = ImVec2(0.0f, 2 * PI);

        if (视野 == 257) {
            if (视野方框) {
                // 被自瞄锁定的目标显示自定义方框颜色
                bool isAimTarget = (mindex >= 0 && i == mindex && Aim.Start);
                ImU32 borderColor = isAimTarget ? 颜色配置.自瞄锁定方框颜色 : (健康血量 ? 颜色配置.方框颜色 : Paint_red);
                ImGui::GetForegroundDrawList()->AddRect(
                    ImVec2((int)map_coord.X - boxWidth, boxTop),
                    ImVec2((int)map_coord.X + boxWidth, boxBottom),
                    borderColor,
                    8.0f, 0, 1.2f // 细线条不变
                );
            }
        } else {
            // 被自瞄锁定的目标显示自定义方框颜色
            bool isAimTarget = (mindex >= 0 && i == mindex && Aim.Start);
            ImU32 borderColor = isAimTarget ? 颜色配置.自瞄锁定方框颜色 : (健康血量 ? 颜色配置.方框颜色 : Paint_red);
            // 外部方框
            ImGui::GetForegroundDrawList()->AddRect(
                ImVec2((int)map_coord.X - boxWidth, boxTop),
                ImVec2((int)map_coord.X + boxWidth, boxBottom),
                borderColor,
                8.0f, 0, 1.2f
            );

            // 内部填充
            ImGui::GetForegroundDrawList()->AddRectFilled(
                ImVec2((int)map_coord.X - boxWidth + 1.0f, boxTop + 1.0f),
                ImVec2((int)map_coord.X + boxWidth - 1.0f, boxBottom - 1.0f),
                健康血量 ? ImColor(255, 255, 255, 50) : ImColor(255, 0, 0, 50)
            );

            // 头像缩小（核心调整）
            if (handId != NULL) {
                float avatarSize = map_coord.H * 0.35f; // 头像缩小（原0.6→0.35）
                float avatarCenterY = (int)map_coord.Y - (map_coord.H * 1.0f); // 位置上移，留足下方空间
                ImGui::GetForegroundDrawList()->AddImage(
                    handId,
                    ImVec2((int)map_coord.X - avatarSize, avatarCenterY - avatarSize),
                    ImVec2((int)map_coord.X + avatarSize, avatarCenterY + avatarSize),
                    ImVec2(0, 0), ImVec2(1, 1),
                    ImColor(255, 255, 255, 255)
                );
            }

            // 弧度血条（适配小头像，位置居中）
            float circleRadius = map_coord.H * 0.25f; // 圆环尺寸适配
            float circleCenterY = (int)map_coord.Y - (map_coord.H * 0.4f); // 头像下方适中位置
            // 圆环背景
            ImGui::GetForegroundDrawList()->AddCircleArc(
                ImVec2((int)map_coord.X, circleCenterY),
                circleRadius,
                FULL_CIRCLE,
                Paint_white,
                5.0f
            );
            // 实际血量弧度
            float hpRadian = current_hp_percentage * 2 * PI;
            ImGui::GetForegroundDrawList()->AddCircleArc(
                ImVec2((int)map_coord.X, circleCenterY),
                circleRadius,
                ImVec2(0.0f, hpRadian),
                健康血量 ? 颜色配置.血量颜色 : Paint_red,
                5.0f
            );
        }
    }
}

            // 80//10
            if (辅助方框) {
              ImGui::GetForegroundDrawList()->AddRect(
                  ImVec2(SmallMapX + 78, SmallMapY - 10),
                  ImVec2(SmallMapX + 2.68 * rientation * Wwra,
                         SmallMapY + 2.13 * rientation * Wwra),
                  颜色配置.方框颜色, 5, 0);
            }
//printf("距离  %.0f\n", 距离敌人);
            if (射线) {
                        if (暴露视野不绘 && 视野 == 257)
                        {
                        }
                        else
                        {
              if (视野 == 257) {
                if (视野方框) {
                  if (距离敌人 > 0 && 距离敌人 < 26000) {
                    // 被自瞄锁定的目标射线显示自定义射线颜色
                    bool isAimTarget = (mindex >= 0 && i == mindex && Aim.Start);
                    ImU32 lineColor = isAimTarget ? 颜色配置.自瞄锁定射线颜色 : 颜色配置.射线颜色;
                    ImGui::GetForegroundDrawList()->AddLine(
                        ImVec2(yxpx / 2, yxpy / 2),
                        ImVec2(map_coord.X,
                               map_coord.Y + (-map_coord.H * 1.0f)),
                        lineColor, 1.5f);
                  }
                }
              } else {
                // 射线
                if (距离敌人 > 0 && 距离敌人 < 26000) {
                  // 被自瞄锁定的目标射线显示自定义射线颜色
                  bool isAimTarget = (mindex >= 0 && i == mindex && Aim.Start);
                  ImU32 lineColor = isAimTarget ? 颜色配置.自瞄锁定射线颜色 : 颜色配置.射线颜色;
                  ImGui::GetForegroundDrawList()->AddLine(
                      ImVec2(yxpx / 2, yxpy / 2),
                      ImVec2(map_coord.X, map_coord.Y + (-map_coord.H * 1.0f)),
                      lineColor, 1.5f);
                }
              }
            }
}




            // 优化：复用前面缓存的skillManagerBase（第1131行已定义）
            
            static int skillCdReadCounter = 0;
            if (skillCdReadCounter++ % 3 == 0) {
                //1技能
                dataTable.heroTemp[i].Space1 = driver->read<int>(ReadValue(ReadValue(skillManagerBase + 0xD8) + 0xf8) + 0x3C) / 8192000;
                dataTable.heroTemp[i].Space1id =dataTable.heroTemp[i].Id*100+10;
                //2技能
                dataTable.heroTemp[i].Space2 = driver->read<int>(ReadValue(ReadValue(skillManagerBase + 0xF0) + 0xf8) + 0x3C) / 8192000;
                dataTable.heroTemp[i].Space2id =dataTable.heroTemp[i].Id*100+20;
                //大招
                dataTable.heroTemp[i].Space = driver->read<int>(ReadValue(ReadValue(skillManagerBase + 0x108) + 0xf8) + 0x3C) / 8192000;
                dataTable.heroTemp[i].Spaceid =dataTable.heroTemp[i].Id*100+30;
                //4技能 (特殊英雄)
                dataTable.heroTemp[i].Space4 = driver->read<int>(ReadValue(ReadValue(skillManagerBase + 0x120) + 0xf8) + 0x3C) / 8192000;
            }

          
            当前血量 = (float)dataTable.heroTemp[i].Hp /
                       (float)dataTable.heroTemp[i].MaxHp;


            if (dataTable.heroTemp[i].TB != 80102 &&
                dataTable.heroTemp[i].TB != 80103 &&
                dataTable.heroTemp[i].TB != 80104 &&
                dataTable.heroTemp[i].TB != 80105 &&
                dataTable.heroTemp[i].TB != 80107 &&
                dataTable.heroTemp[i].TB != 80108 &&
                dataTable.heroTemp[i].TB != 80109 &&
                dataTable.heroTemp[i].TB != 80110 &&
                dataTable.heroTemp[i].TB != 80115 &&
                dataTable.heroTemp[i].TB != 80121 &&
                dataTable.heroTemp[i].TB != 80116 &&
                dataTable.heroTemp[i].TB != 801162) {
              dataTable.heroTemp[i].TB = 0;
            }
//大招
tubiaoid=reinterpret_cast<ImTextureID>(技能贴图.头像[dataTable.heroTemp[i].Spaceid].textureId);
//2技能
tubiaoid3=reinterpret_cast<ImTextureID>(技能贴图.头像[dataTable.heroTemp[i].Space2id].textureId);
//1技能
tubiaoid4=reinterpret_cast<ImTextureID>(技能贴图.头像[dataTable.heroTemp[i].Space1id].textureId);
//字体
float text_zt = (float)((double)(0.266777647) * map_coord.H);







if (方框技能) {
    if (!(方框技能不绘 && 视野 == 257)) {
        // 通用样式（强化视觉层次，优化色彩与光影）
        float numFontSize = map_coord.H * 0.22f;
        float cdFontSize = map_coord.H * 0.28f;        // 冷却字体进一步放大，更醒目
        ImU32 normalColor = IM_COL32(255, 255, 255, 240); // 正常文字（略微透明，不刺眼）
        ImU32 cdColor = IM_COL32(100, 100, 100, 220);     // 冷却图标色（深灰，区分度更高）
        ImU32 cdTextColor = IM_COL32(255, 180, 0, 255);    // 冷却时间（亮黄，视觉冲击）
        ImU32 readyColor = IM_COL32(0, 220, 100, 255);     // 技能就绪提示色（亮绿）
        float skillSpacing = map_coord.H * 0.42f;       // 纵向间距微调，更紧凑
        float circleRadius = map_coord.H * 0.17f;       // 技能背景圆半径放大
        float cdOffsetX = map_coord.H * 0.28f;          // 冷却文字与图标间距优化
        float borderThickness = map_coord.H * 0.015f;   // 边框厚度
        ImU32 borderColor = IM_COL32(50, 50, 50, 200);  // 边框颜色（深灰，增强轮廓）

        // 横向基准（与方框右侧对齐，微调偏移更协调）
        float baseX = (float)map_coord.X + (map_coord.H * 0.92f);

        // 通用绘制技能图标+边框+背景圆（提取复用逻辑，精简代码）
        auto DrawSkillIcon = [&](ImVec2 center, ImTextureID icon, ImU32 tint, bool isReady) {
            // 绘制半透明背景圆（增强层次感）
            ImGui::GetForegroundDrawList()->AddCircleFilled(
                center, circleRadius, IM_COL32(30, 30, 30, 120), 16
            );
            // 绘制技能图标（圆形裁剪）
            ImVec2 min = ImVec2(center.x - circleRadius, center.y - circleRadius);
            ImVec2 max = ImVec2(center.x + circleRadius, center.y + circleRadius);
            ImGui::GetForegroundDrawList()->AddImage(
                icon ? icon : reinterpret_cast<ImTextureID>(技能贴图.头像[0].textureId),
                min, max, ImVec2(0, 0), ImVec2(1, 1), tint
            );
            // 绘制边框（就绪时亮绿，冷却时深灰）
            ImGui::GetForegroundDrawList()->AddCircle(
                center, circleRadius, isReady ? readyColor : borderColor, 16, borderThickness
            );
        };

        // 通用绘制冷却文字（带描边，更清晰）
        auto DrawCdText = [&](ImVec2 center, int cd) {
            static char entitySkillCdText[16];
            snprintf(entitySkillCdText, sizeof(entitySkillCdText), "%d", cd);
            ImVec2 cdSize = ImGui::GetFont()->CalcTextSizeA(cdFontSize, FLT_MAX, 0, entitySkillCdText);
            ImVec2 textPos = ImVec2(center.x + cdOffsetX, center.y - (cdFontSize * 0.6f));
            
            // 文字描边（深灰，增强可读性）
            ImGui::GetForegroundDrawList()->AddText(
                NULL, cdFontSize, ImVec2(textPos.x + 1, textPos.y + 1), IM_COL32(0, 0, 0, 180), entitySkillCdText
            );
            // 主文字（亮黄）
            ImGui::GetForegroundDrawList()->AddText(
                NULL, cdFontSize, textPos, cdTextColor, entitySkillCdText
            );
        };

        // 1技能（最上方）
        ImVec2 center1 = ImVec2(baseX, (float)map_coord.Y - (map_coord.H * 1.1f));
        ImTextureID skill1Icon = 0;
        int skill1Id = dataTable.heroTemp[i].Space1id;
        if (skill1Id > 0 && skill1Id < 100000) {
            skill1Icon = reinterpret_cast<ImTextureID>(技能贴图.头像[skill1Id].textureId);
        }
        bool skill1Ready = dataTable.heroTemp[i].Space1 <= 0;
        DrawSkillIcon(center1, skill1Icon, skill1Ready ? normalColor : cdColor, skill1Ready);
        if (!skill1Ready) {
            DrawCdText(center1, dataTable.heroTemp[i].Space1);
        }

        // 2技能（1技能下方）
        ImVec2 center2 = ImVec2(baseX, center1.y + skillSpacing);
        ImTextureID skill2Icon = 0;
        int skill2Id = dataTable.heroTemp[i].Space2id;
        if (skill2Id > 0 && skill2Id < 100000) {
            skill2Icon = reinterpret_cast<ImTextureID>(技能贴图.头像[skill2Id].textureId);
        }
        bool skill2Ready = dataTable.heroTemp[i].Space2 <= 0;
        DrawSkillIcon(center2, skill2Icon, skill2Ready ? normalColor : cdColor, skill2Ready);
        if (!skill2Ready) {
            DrawCdText(center2, dataTable.heroTemp[i].Space2);
        }

        // 3技能/大招（2技能下方，特殊标识）
        ImVec2 center3 = ImVec2(baseX, center2.y + skillSpacing);
        ImTextureID skill3Icon = 0;
        int skill3Id = dataTable.heroTemp[i].Spaceid;
        if (skill3Id > 0 && skill3Id < 100000) {
            skill3Icon = reinterpret_cast<ImTextureID>(技能贴图.头像[skill3Id].textureId);
        }
        bool skill3Ready = dataTable.heroTemp[i].Space <= 0;
        // 大招额外绘制金色内边框，突出重要性
        ImGui::GetForegroundDrawList()->AddCircle(
            center3, circleRadius - borderThickness * 2, IM_COL32(255, 220, 50, 150), 16, borderThickness
        );
        DrawSkillIcon(center3, skill3Icon, skill3Ready ? normalColor : cdColor, skill3Ready);
        if (!skill3Ready) {
            DrawCdText(center3, dataTable.heroTemp[i].Space);
        }

        // 召唤师技能（特殊样式，区分普通技能）
        int summonTb = dataTable.heroTemp[i].TB;
        int summonCd = dataTable.heroTemp[i].Skill;
        ImTextureID tubiaoid2 = reinterpret_cast<ImTextureID>(技能贴图.头像[0].textureId); // 默认贴图

        if (summonTb != 0) {
            if (summonTb == 53391 || summonTb == 53395 || summonTb == 53393) {
                int tb4Index = dataTable.heroTemp[i].TB4;
                if (tb4Index > 0 && tb4Index < 100000) {
                    tubiaoid2 = reinterpret_cast<ImTextureID>(技能贴图.头像[tb4Index].textureId);
                }
            } else if (summonTb == 801162) {
                tubiaoid2 = reinterpret_cast<ImTextureID>(技能贴图.头像[summonTb - 800000].textureId);
            } else if (summonTb >= 80102 && summonTb <= 80121) {
                tubiaoid2 = reinterpret_cast<ImTextureID>(技能贴图.头像[summonTb - 80000].textureId);
            } else {
                tubiaoid2 = reinterpret_cast<ImTextureID>(技能贴图.头像[summonTb - 80000].textureId);
            }
        }

        bool summonReady = summonCd <= 0;
        ImVec2 summonCenter = ImVec2(baseX, center3.y + skillSpacing);
        // 召唤师技能背景圆（浅蓝色，区分普通技能）
        ImGui::GetForegroundDrawList()->AddCircleFilled(
            summonCenter, circleRadius, IM_COL32(50, 100, 200, 100), 16
        );
        // 绘制召唤师技能图标+边框
        DrawSkillIcon(summonCenter, tubiaoid2, summonReady ? normalColor : cdColor, summonReady);
        // 冷却时间（居中显示，带描边）
        if (!summonReady) {
            static char summonCdText[16];
            snprintf(summonCdText, sizeof(summonCdText), "%d", summonCd);
            ImVec2 cdTextSize = ImGui::GetFont()->CalcTextSizeA(cdFontSize, FLT_MAX, 0, summonCdText);
            ImVec2 textPos = ImVec2(
                summonCenter.x - cdTextSize.x * 0.5f,
                summonCenter.y - (cdFontSize * 0.6f)
            );
            // 文字描边
            ImGui::GetForegroundDrawList()->AddText(
                NULL, cdFontSize, ImVec2(textPos.x + 1, textPos.y + 1), IM_COL32(0, 0, 0, 180), summonCdText
            );
            // 主文字（亮绿）
            ImGui::GetForegroundDrawList()->AddText(
                NULL, cdFontSize, textPos, Paint_green, summonCdText
            );
        }
    }
}


// 调用实体技能显示函数
if (实体技能 && map_coord.X > 0 && map_coord.Y > 0 && TXdx > 0 && dataTable.heroTemp[i].Id > 0) {
    Drawskill(ImGui::GetForegroundDrawList(), dataTable.heroTemp[i].Id, 
              dataTable.heroTemp[i].Space4, dataTable.heroTemp[i].Space, 
              dataTable.heroTemp[i].Space2, dataTable.heroTemp[i].Space1, 
              dataTable.heroTemp[i].Skill, dataTable.heroTemp[i].TB, TXdx);
}

                                     if (自动净化) {
                            if (控制状态 != 0 &&
                                距离敌人 <= 12000) {
                                if (召唤师ID == 133 && 召唤师技能CD == 0) {
                                    //触摸缓冲 = true;
                                    if (屏幕方向 == 3) {
                                        Touch::Down(zhsY, yxpx - zhsX);
                                    } else if (屏幕方向 == 1) {
                                        Touch::Down(yxpy - zhsY, zhsX);
                                    }
                                    //std::thread t([&]() {
                                    usleep(1000 * 5);
                                    Touch::Up();

                                } else if (召唤师技能判断) {
                                    //触摸缓冲 = true;
                                    if (屏幕方向 == 3) {
                                        Touch::Down(zhsY, yxpx - zhsX);
                                    } else if (屏幕方向 == 1) {
                                        Touch::Down(yxpy - zhsY, zhsX);
                                    }
                                    //std::thread t([&]() {
                                    usleep(1000 * 5);
                                    Touch::Up();
                                }
                                // std::this_thread::sleep_for(1);
                                //usleep(1000 * 500);
                                //触摸缓冲 = false;
                                //});
                                // 启动线程
                                //t.detach(); // 或者 t.detach();
                            }
                        }
            
            if (自动斩杀) {
              float 当前血量 = (float)dataTable.heroTemp[i].Hp /
                               (float)dataTable.heroTemp[i].MaxHp;
              if (!智能斩杀) {
                if (当前血量 > 0 && 当前血量 < zhanshaz) {
                  血量判断 = true;
                } else {
                  血量判断 = false;
                }
              } else {
                float 智能斩杀 = (float)dataTable.heroTemp[i].Hp -
                                 ((float)dataTable.heroTemp[i].MaxHp -
                                  (float)dataTable.heroTemp[i].Hp) *
                                     0.15;
                if (智能斩杀 < 0) {
                  血量判断 = true;
                } else {
                  血量判断 = false;
                }
              }
              if (血量判断 && 召唤师技能ID == 80108 && 召唤师技能判断 &&
                  距离敌人 <= 5000 && 视野 == 257) {
             //   触摸缓冲 = true;
                if (屏幕方向 == 3) {
                	Touch::Down(zhsY, yxpx - zhsX);
                } else if (屏幕方向 == 1) {
                	Touch::Down(yxpy - zhsY, zhsX);
                }
                std::thread t([&]() {
                  usleep(1000 * 5);
                  Touch::Up();
                  // std::this_thread::sleep_for(1);
                  usleep(1000 * 200);
                 // 触摸缓冲 = false;
                });
                // 启动线程
                t.detach(); // 或者 t.detach();
              }
            }

          } // 血量大于0

DRAW_TOP_SKILL:
          // TODO：顶上技能
          if (顶上技能2) {
            DrawTopskill(ImGui::GetForegroundDrawList(), handId, dataTable.heroTemp[i].Id, 
                        蓝方敌人视野判断, 红方敌人视野判断, opop, aa, 
                        dataTable.heroTemp[i].Space, dataTable.heroTemp[i].Space2, 
                        dataTable.heroTemp[i].Space1, dataTable.heroTemp[i].Skill, 
                        dataTable.heroTemp[i].TB);
          }
         

          opop++;
        }
      }
    }//10次循环结束



   if (上帝) {
        // 优化：缓存上帝模式地址
        uint64_t godModeBase = driver->read<uint64_t>(lil2cpp_base+0x10608);
        long sdaddr = driver->read<uint64_t>(godModeBase + 0xB8) + 0x28C;
        driver->write<float>(sdaddr,shangdi);//自定义数值//正式服
   }



    uint64_t BuffAddress; // buff数量地址
    uint64_t BxAddress;   // 兵线数量地址
    uint64_t bxAddress;   // 兵线属性首位置
    uint64_t Buffsl;      // buff坐标数量
/* 
 * 由光头强内核（原XF内核）开源
 因频道账号被盗用后决定开源，正版为光头强内核，
开源文件仅供学习交流，任何个人或组织可在遵循协议条款的前提下，自由下载、使用、修改、分发本项目代码及衍生作品，无需额外授权。使用过程中请保留原项目版权声明及开源协议文本，不得用于违法违规场景。本项目以“现状”提供，开发者不承担任何直接或间接的使用风险及法律责任，亦不提供商业技术支持。
删除此内容死全家，没几把
* 注入时间: 2025-11-16 20:14:49
 * 联系方式: TG:@GTQNHNB666
QQ群：784316342
 */
    if (野怪) {
      int 野怪距敌 = 0;
      int 距离野怪 = 0;
      bool 野怪距敌判断 = false;

      // ⭐修复Bug：野怪插值也需要static，否则每帧重置
      static std::vector<float> pvePosX(25, 0.0f);
      static std::vector<float> pvePosY(25, 0.0f);
      
      long temp2 = driver->read<uint64_t>(libGame_base + 0x1F60);//野怪抢先服
      
      // 优化：缓存野怪buff地址链，避免5层嵌套
      uint64_t buffChain = driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>(temp2 + 0x3A8) + 0x98) + 0x148) + 0x88) + 0x140);
      BuffAddress = buffChain;//抢先服
      
      
      // printf("START\n");
      for (int i = 0; i < 23; i++) {
      //共享


     
        pvePosX[i] = dataTable.pveTemp[i].coord.X;
        pvePosY[i] = dataTable.pveTemp[i].coord.Y;
        
        // 优化：缓存野怪buff地址，避免重复读取
        uint64_t buffAddr = driver->read<uint64_t>((u_long)(BuffAddress + i * 0x18));
        dataTable.pve[i].cd = buffAddr + 0x240;
        dataTable.pve[i].maxcd = buffAddr + 0x1e4;
        dataTable.pve[i].cc = buffAddr;
        dataTable.pve[i].id = dataTable.pve[i].cc + 0xC0;
        long int 实体指针=driver->read<uint64_t>(dataTable.pve[i].cc+0x3A0);
        
        // 优化：缓存野怪坐标地址
        uint64_t pveCoordAddr = driver->read<uint64_t>(实体指针+0x230)+0x60;
        dataTable.pve[i].X = driver->read<uint64_t>(pveCoordAddr)+0x0;
        dataTable.pve[i].Y = driver->read<uint64_t>(pveCoordAddr)+0x8;
        
        // 优化：缓存野怪血量基址
        uint64_t pveHpBase = driver->read<uint64_t>(实体指针 + 0x168);
        dataTable.pve[i].hp = pveHpBase + 0xa8;
        dataTable.pve[i].maxhp = pveHpBase + 0xb0;


        // 优化：缓存惩戒伤害地址链，避免5层嵌套
        uint64_t punishChain = driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>(lil2cpp_base+0x367D18)+0xB8)+0x0)+0x20)+0x18);
        int 惩戒 = driver->read<int>(punishChain + 0x244);//惩戒

     if (ESPMenu.是否开启共享) {
                    //共享全源数据
       // bbuff1 = Driver->读取指针(野怪数组 + i * 0x18);
        buffid = driver->read<int>(dataTable.pve[i].id);
        ygtime = driver->read<int>(dataTable.pve[i].cd ) / 1000;
         xxx1= driver->read<int>(dataTable.pve[i].cc + 0x2b8);
         yyy1= driver->read<int>(dataTable.pve[i].cc + 0x2C0);
        buffx = xxx1 * rientation * WwrShare / 50000.0f + WwrShare;
        buffy = yyy1 * rientation * WwrShare / 50000.0f * -1 + WwrShare;
        //传输野怪数据
        creeps += std::to_string(0)
            + "," + std::to_string(ygtime)
            + "," + std::to_string(buffid)
            + "," + std::to_string(buffx + ESPMenu.野怪左右调整)
            + "," + std::to_string(buffy + ESPMenu.野怪上下调整)
            + "==";
        //传输野怪数据完毕
        }

            if (召唤师ID == 183) {
            惩戒伤害 = 惩戒 * 1.25;
            } else {
            惩戒伤害 = 惩戒;
            }

        static int pveCdReadCounter = 0;
        if (pveCdReadCounter++ % 5 == 0) {
            dataTable.pveTemp[i].cd = driver->read<int>(dataTable.pve[i].cd) / 1000;
            dataTable.pveTemp[i].maxcd =
                driver->read<int>(dataTable.pve[i].maxcd) / 1000;
            dataTable.pveTemp[i].id = driver->read<int>(dataTable.pve[i].id);
        }
        if(dataTable.pveTemp[i].id == 0){continue;}
        int 固定坐标X = driver->read<int>(dataTable.pve[i].cc + 0x2b8);
        int 固定坐标Y = driver->read<int>(dataTable.pve[i].cc + 0x2C0);
        float pveX =
            (float)driver->read<int>(dataTable.pve[i].X);
        float pveY =
            (float)driver->read<int>(dataTable.pve[i].Y);
            
            float 野怪跨度X = pveX - pvePosX[i];
			  if (野怪跨度X < 0) {
			  	野怪跨度X = 野怪跨度X * -1;
			  }
			  float 野怪跨度Y = pveY - pvePosY[i];
			  if (野怪跨度Y < 0) {
			  	野怪跨度Y = 野怪跨度Y * -1;
			  }
			  //printf("X %.0f\tY %.0f\n", 野怪跨度X, 野怪跨度Y);
			  if (野怪跨度X > 1000 || 野怪跨度Y > 1000) {
			  	dataTable.pveTemp[i].coord.X = pveX;
                dataTable.pveTemp[i].coord.Y = pveY;
			  } else {
            
            std::pair<float, float> pvesmoothedCoords = smoothPosition(i, pveX, pveY, pvePosX, pvePosY);
            
            dataTable.pveTemp[i].coord.X = pvesmoothedCoords.first;
            dataTable.pveTemp[i].coord.Y = pvesmoothedCoords.second;
            }
            
            


            static int pveHpReadCounter = 0;
            if (pveHpReadCounter++ % 2 == 0) {
                dataTable.pveTemp[i].hp = driver->read<int>(dataTable.pve[i].hp);
                dataTable.pveTemp[i].maxhp = driver->read<int>(dataTable.pve[i].maxhp);
            }
            //printf("hp %d  maxhp %d\n", dataTable.pveTemp[i].hp, dataTable.pveTemp[i].maxhp);
        map_buff = CalMatrixMem(dataTable.pveTemp[i].coord, dataTable.Matrix);
        map_buff.X = map_buff.X + SmallHPX;
        map_buff.Y = map_buff.Y + SmallHPY;
        实体野怪 = ImVec2((int)map_coord.X, (int)map_coord.Y);
        ImVec2 固定野怪 = ImVec2((int)pveX, (int)pveY);
        距离野怪 = 计算距离(自身坐标, 固定野怪);


        float pvegdX =
            (float)(固定坐标X * rientation * Wwra /
                        50000.0f +
                    Wwra);
        float pvegdY =
            (float)(固定坐标Y * rientation * Wwra /
                        50000.0f * -1 +
                    Wwra);

         //printf("ID %d\t距离 %d\t血量 %d\t伤害 %d\n",dataTable.pveTemp[i].id, 距离野怪, dataTable.pveTemp[i].hp, 惩戒伤害);

          if (自动惩戒 && 召唤师技能判断 && dataTable.pveTemp[i].hp > 0) {
          // printf("自动惩戒已开启\n");
          if (dataTable.pveTemp[i].id == 166018 ||
              dataTable.pveTemp[i].id == 166009 ||
              dataTable.pveTemp[i].id == 166012 ||
              dataTable.pveTemp[i].id == 1660221 ||
              dataTable.pveTemp[i].id == 166022 ||
              dataTable.pveTemp[i].id == 266010 ||
              dataTable.pveTemp[i].id == 266011 ||
              
              dataTable.pveTemp[i].id == 1010350 ||
              dataTable.pveTemp[i].id == 1010351 ||
              dataTable.pveTemp[i].id == 1010334 ||
              dataTable.pveTemp[i].id == 1010336 ||
              dataTable.pveTemp[i].id == 1010335 ||
              dataTable.pveTemp[i].id == 1010333) {
              
              for (int l = 0; l < 5; l++) {
			  野怪距敌 = 计算距离(固定野怪, ImVec2(dr_x[l], dr_y[l]));
			  //printf("野怪距敌 %d\n", 野怪距敌);
			  //printf("X坐标 %d\tY坐标 %d\t距敌 %d\t距自 %d\n", dr_x[l], dr_y[l], 野怪距敌, 距离野怪);
			  if (距离野怪 <= 10000) {
			  
			  	//提示
              ImGui::GetForegroundDrawList()->AddImage(
                      reinterpret_cast<ImTextureID>(其他贴图.图标[1].textureId),
                      {(int)map_buff.X - (map_buff.H * 0.2f),
                       (int)map_buff.Y - (map_buff.H * 0.2f) +
                           (-map_buff.H * 1.0f)},
                      {(int)map_buff.X + (map_buff.H * 0.2f),
                       (int)map_buff.Y + (map_buff.H * 0.2f) +
                           (-map_buff.H * 1.0f)});

			  if (野怪距敌 <= 10000) {
			  	野怪距敌判断 = true;
			  	//printf("野怪距敌判断成功\n");
			  	break;
			  }
			  }
			  
				}
				
                           
            if (智能不惩) {
              if (距离野怪 < 5900 && dataTable.pveTemp[i].hp <= 惩戒伤害 &&
                   野怪距敌判断 && !触摸缓冲) {
                惩戒判断 = true;
              } else {
                惩戒判断 = false;
              }
            } else {
              if (距离野怪 < 5900 && dataTable.pveTemp[i].hp > 0 && dataTable.pveTemp[i].hp <= 惩戒伤害 && !触摸缓冲) {
                惩戒判断 = true;
              } else {
                惩戒判断 = false;
              }
            }

            if (惩戒判断) {
              // printf("点击惩戒\n");
              触摸缓冲 = true;
              if (屏幕方向 == 3) {
                	Touch::Down(zhsY, yxpx - zhsX);
                } else if (屏幕方向 == 1) {
                	Touch::Down(yxpy - zhsY, zhsX);
                }
              std::thread cj([&]() {
                usleep(1000 * 5);
                Touch::Up();
                // std::this_thread::sleep_for(1);
                usleep(1000 * 250);
                触摸缓冲 = false;
              });
              // 启动线程
              cj.detach(); // 或者 t.detach();
            }
          }
        }

if (野血) {
    // 王者新潜s40样式野怪血量条系统
    if (dataTable.pveTemp[i].hp > 0 
        && dataTable.pveTemp[i].cd == dataTable.pveTemp[i].maxcd 
        && dataTable.pveTemp[i].hp < dataTable.pveTemp[i].maxhp) {
        
        // 计算血量比例和血量条参数
        float hp_ratio = (float)dataTable.pveTemp[i].hp / (float)dataTable.pveTemp[i].maxhp;
        const float 血量条总宽度 = map_buff.H * 1.0f; // 动态宽度，根据野怪大小调整
        const float 血量条宽度 = 血量条总宽度 * hp_ratio;
        
        // 定义血量条位置
        const ImVec2 背景左上 = ImVec2((float)((int)map_buff.X - (map_buff.H * 0.5f)), (float)((int)map_buff.Y - (map_buff.H * 0.075f)));
        const ImVec2 背景右下 = ImVec2((float)((int)map_buff.X + (map_buff.H * 0.5f)), (float)((int)map_buff.Y + (map_buff.H * 0.075f)));
        
        // 王者新潜s40样式：绘制白色边框
        ImGui::GetForegroundDrawList()->AddRect(背景左上, 背景右下, ImColor(255, 255, 255), 5);
        
        // 计算血量条填充区域
        float buff_hp = (float)dataTable.pveTemp[i].hp / (float)dataTable.pveTemp[i].maxhp - 0.5f;
        
        // 王者新潜s40样式：三色血量系统
        if(dataTable.pveTemp[i].hp <= 惩戒伤害 && dataTable.pveTemp[i].hp != dataTable.pveTemp[i].maxhp) {
            // 🔴 红色：可惩戒血量
            ImGui::GetForegroundDrawList()->AddRectFilled(背景左上, 
                ImVec2((float)((int)map_buff.X + (map_buff.H * buff_hp)), (float)((int)map_buff.Y + (map_buff.H * 0.075f))), 
                ImColor(255, 0, 0, 150), 5);
        } else if(hp_ratio <= 0.5f && dataTable.pveTemp[i].hp > 惩戒伤害) {
            // 🟡 黄色：血量50%以下但高于惩戒伤害
            ImGui::GetForegroundDrawList()->AddRectFilled(背景左上, 
                ImVec2((float)((int)map_buff.X + (map_buff.H * buff_hp)), (float)((int)map_buff.Y + (map_buff.H * 0.075f))), 
                ImColor(255, 255, 0, 150), 5);
        } else {
            // 🔵 蓝色：正常血量
            ImGui::GetForegroundDrawList()->AddRectFilled(背景左上, 
                ImVec2((float)((int)map_buff.X + (map_buff.H * buff_hp)), (float)((int)map_buff.Y + (map_buff.H * 0.075f))), 
                ImColor(0, 255, 255, 150), 5);
        }
        
        // 血量数字显示（居中）
        string hpText = std::to_string(dataTable.pveTemp[i].hp);
        auto textSize = ImGui::CalcTextSize(hpText.c_str(), 0, 25);
        ImVec2 textPos((背景左上.x + 背景右下.x - textSize.x) * 0.5f, (背景左上.y + 背景右下.y - textSize.y) * 0.5f - 25.0f);
        
        ImGui::GetForegroundDrawList()->AddText(NULL, 25.0f, textPos, ImColor(255, 255, 255), hpText.c_str());
    }
}


  
        
        
        // 小地图野怪血量数字显示（移植自无解密版本）
        if (野血) {
            // 同时满足：存活、冷却就绪、血量未满
            if (dataTable.pveTemp[i].hp > 0 
                && dataTable.pveTemp[i].cd == dataTable.pveTemp[i].maxcd 
                && dataTable.pveTemp[i].hp < dataTable.pveTemp[i].maxhp) {
                // 优化：使用静态缓冲区避免 std::to_string
                static char hpText[16];
                snprintf(hpText, sizeof(hpText), "%d", dataTable.pveTemp[i].hp);
                
                ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, 0, hpText);
                ImVec2 textPos = ImVec2(
                    (float)(pvegdX + SmallMapX - 6.0 + 93) - textSize.x * 0.5f,
                    (float)(pvegdY + SmallMapY - 7.9 + 6) - textSize.y * 0.25f - 35.0f  // 向上偏移避免与CD重叠
                );
                
                ImGui::GetForegroundDrawList()->AddText(
                    NULL, 31.0f + xiaodituxue,  // 支持字体大小调节
                    textPos,
                    ImColor(0, 255, 255),  // 青蓝色
                    hpText
                );
            }
        }
        
        if (dataTable.pveTemp[i].cd == 0 || dataTable.pveTemp[i].cd == dataTable.pveTemp[i].maxcd || dataTable.pveTemp[i].cd > 240 || dataTable.pveTemp[i].id == 166009 || dataTable.pveTemp[i].id == 1010333 || dataTable.pveTemp[i].id == 166018 || dataTable.pveTemp[i].id == 1010335 || dataTable.pveTemp[i].id == 166012 || dataTable.pveTemp[i].id == 1010334 || dataTable.pveTemp[i].id == 166022 || dataTable.pveTemp[i].id == 1010336 || dataTable.pveTemp[i].id == 1660221) {
        //166009/1010333/166018/1010335/166012/1010334/166022/1010336/1660221
          continue;
        } else {
          // 优化：使用静态缓冲区避免 std::to_string
          static char cdText[16];
          snprintf(cdText, sizeof(cdText), "%d", dataTable.pveTemp[i].cd);

          ImVec2 textSize2 = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, 0, cdText);
          ImVec2 textPos2 = ImVec2((float)(pvegdX + SmallMapX - 6.0 + 93) - textSize2.x * 0.5f, (float)(pvegdY + SmallMapY - 7.9 + 6) - textSize2.y * 0.25f);
          ImGui::GetForegroundDrawList()->AddText(
              NULL, 31.0f,
              textPos2,
              ImColor(255, 255, 255), cdText);
        }
        
        
        
        
      }
    }

        
   if (野怪) {
    
    	dataTable.pveTemp[1].coord.X = -55000 + 2500;
    	dataTable.pveTemp[1].coord.Y = -55000 + 2500;
    
        Pvc_coord = CalMatrixMem(dataTable.pveTemp[1].coord, dataTable.Matrix);
        Pvc_coord.X = Pvc_coord.X + SmallHPX;
        Pvc_coord.Y = Pvc_coord.Y + SmallHPY;
        
std::string txt = "作者：光头强（原xf）  频道：@GTQNHNB666   QQ群：784316342";
ImVec2 textSize = ImGui::CalcTextSize(txt.c_str());
ImVec2 textPos(Pvc_coord.X - textSize.x * 0.5f, Pvc_coord.Y - textSize.y * 0.5f);
    	绘制字体描边(34.0f,
        textPos,
        ImColor(128, 128, 128), txt.c_str());
    
    }
	
if (兵线) {
      int number; // 兵线数量
  long 兵线头 =driver->read<uint64_t>(libGame_base+ 0x16C4C0);
    uint64_t BxAddress = driver->read<uint64_t>(兵线头 + 0x2E0);
      number = 50;
      
      

      long cont = 0;
      for (int l = 0; l < number; l++) {
        bxAddress = driver->read<uint64_t>(BxAddress + (l * 0x18));
        int temp = driver->read<int>(bxAddress + 0x3c);
        if (temp == foeComp) {
          // 优化：缓存兵线血量基址
          uint64_t soldierHpBase = driver->read<uint64_t>(bxAddress + 0x168);
          int bxxl = driver->read<int>(soldierHpBase + 0xa8);

          // 优化：缓存兵线坐标地址
          uint64_t soldierCoordAddr = driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>(bxAddress + 0x230) + 0xf0) + 0x60);
          dataTable.CeTemp[cont].coord.X = (float)driver->read<int>(soldierCoordAddr + 0x0);
          dataTable.CeTemp[cont].coord.Y = (float)driver->read<int>(soldierCoordAddr + 0x8); 
              
               if (ESPMenu.是否开启共享) {
                  int 兵线X = dataTable.CeTemp[cont].coord.X * rientation * bingxianwwra / 50000 + bingxianwwra;
                  int 兵线Y = dataTable.CeTemp[cont].coord.Y * rientation * bingxianwwra / 50000 * -1 + bingxianwwra;

                  //定义传输兵线
                bingx = 兵线X* 0.7;
                bingy = 兵线Y* 0.7;
                soldier += std::to_string(bingx + ESPMenu.兵线左右调整) +
                 "," + std::to_string(bingy + ESPMenu.兵线左右调整) + 
                 "," + std::to_string(temp) + "==";
               }
              
              
              
                 if (dataTable.CeTemp[cont].coord.X == 0 ||
              dataTable.CeTemp[cont].coord.Y == 0) {
            continue;
          }
          if (bxxl <= 0) {
            continue;
          }    
              
              
              
    
            Pvc_coord = CalMatrixMem(dataTable.CeTemp[cont].coord, dataTable.Matrix);
          Pvc_coord.X = Pvc_coord.X + SmallHPX;
        Pvc_coord.Y = Pvc_coord.Y + SmallHPY;
      
        
            
        if (实体兵线) {
            ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(Pvc_coord.X, Pvc_coord.Y), 8.0f,
                                                      Paint_yellow, 16);
                                                      
     ImGui::GetForegroundDrawList()->AddCircle(ImVec2(Pvc_coord.X, Pvc_coord.Y), 11.0f,
                                          Paint_yellow,
                                          16,
                                          2.0f);                                       
                                                      
            }
          dataTable.CeTemp[cont].coord.X =
              dataTable.CeTemp[cont].coord.X * rientation * Wwra / 50000.0f +
              Wwra;
          dataTable.CeTemp[cont].coord.Y = dataTable.CeTemp[cont].coord.Y *
                                               rientation * Wwra / 50000.0f *
                                               -1 +
     Wwra;
                                                           //共享
                                                     

                                
          cont++;
        }
      }
      dataTable.xbsl = cont;

      for (int i = 0; i < dataTable.xbsl; i++) {
        float x = dataTable.CeTemp[i].coord.X + SmallMapX + 93;
        float y = dataTable.CeTemp[i].coord.Y + SmallMapY + 6;
        ImGui::GetForegroundDrawList()->AddRect(
            ImVec2((float)x - soldiers_dx, (float)y - soldiers_dx),
            ImVec2((float)x + soldiers_dx, (float)y + soldiers_dx), Paint_red,
            soldiers_dx, 0, soldiers_dx * 2);
      }
    }

        
 
        
    if (绘制触摸) {
      // zhsX和zhsY是屏幕绘制坐标，直接使用即可
      // 如果显示在左上角(0,0)，说明滑块值未设置，请在菜单中调整“触摸位置左右”和“触摸位置上下”
      ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(zhsX, zhsY), 50.0f,
                                                      Paint_gray, 16);
      绘制触摸 = false;
    }
    
    //}
    
       if (ESPMenu.是否开启共享) {
           
          gameDataStr = 
        "gameData" + 设备id + 
        "[==][==]" + character + 
        "---" + creeps + 
        "---" + soldier;
        char* gameData = (char*)gameDataStr.data();  //设备
        send(socket_fd, gameData, strlen(gameData), 0);
  }
    
  }
  
  // ===== 自瞄系统核心算法 =====
  if (Aim.Start) {
      // 重置目标选择变量
      small_hp = 0;
      small_dist = 0;
      minAngle = 000.0f;
      mindex = -1;
      
      // 调试输出
      // static int debugCounter = 0;
      // if (debugCounter++ % 60 == 0) { // 每60帧输出一次
      //     printf("[自瞄调试] AroundNumbers=%d, 自身视野=%d, Aim.distance=%.1f\n", AroundNumbers, 自身视野, Aim.distance);
      // }
      
      // int validEnemyCount = 0;  // 统计有效敌人数量
      
      // 遍历所有敌人，选择最佳目标
      for (int i = 0; i < AroundNumbers; i++) {
          // 首先检查是否是敌人
          int zhengxing = driver->read<int>(object.Heroaddr[i] + 0x999d);
          if (zhengxing != foeComp) {
              // printf("[跳过] i=%d 不是敌人, zhengxing=%d, foeComp=%d\n", i, zhengxing, foeComp);
              continue; // 跳过队友和非英雄单位
          }
          
          // 检查是否是有效的英雄ID（避免锁定非英雄单位）
          if (dataTable.heroTemp[i].Id < 900 || dataTable.heroTemp[i].Id > 600) {
              // printf("[跳过] i=%d ID无效, Id=%d\n", i, dataTable.heroTemp[i].Id);
              continue;
          }
          
          // validEnemyCount++;
          
          // 调试输出每个目标的信息
          // static int loopDebugCounter = 0;
          // if (loopDebugCounter++ % 300 == 0 && i < 3) { // 每5秒输出前3个目标的信息
          //     printf("[目标%d] ID=%d, 距离=%.1f(限制%.1f), 血量=%d, 自身血量=%d\n", 
          //            i, dataTable.heroTemp[i].Id, config.distance[i], Aim.distance, dataTable.heroTemp[i].Hp, object.SelfHp);
          // }
          
          // 视野判断：只有暴露的敌人才能参与自瞄计算
          bool canAim = true;
          if (Aim.vision && object.visual[i] != 667) {
              canAim = false; // 未暴露的敌人不参与自瞄
          }
          
          if (canAim && config.distance[i] < Aim.distance && dataTable.heroTemp[i].Hp > 0) {
              
              // 根据目标选择模式选择目标
              switch(Aim.select_mode) {
                  case 0: // 血量最低
                      if (small_hp <= 0 || small_hp >= dataTable.heroTemp[i].Hp) {
                          small_hp = dataTable.heroTemp[i].Hp;
                          mindex = i;
                      }
                      break;
                      
                  case 1: // 距离最近
                      if (small_dist <= 0 || small_dist >= config.distance[i]) {
                          small_dist = config.distance[i];
                          mindex = i;
                      }
                      break;
              }
          } // else {
              // 目标不在范围内或已死亡
              // static int outOfRangeCounter = 0;
              // if (outOfRangeCounter++ % 300 == 0 && i < 3) {
              //     printf("[超范围%d] ID=%d, 距离=%.1f>限制%.1f, 血量=%d\n", 
              //            i, dataTable.heroTemp[i].Id, config.distance[i], Aim.distance, dataTable.heroTemp[i].Hp);
              // }
          // }
      }
      
      // 输出目标选择结果
      // static int selectionCounter = 0;
      // if (selectionCounter++ % 60 == 0) {
      //     printf("[目标统计] 有效敌人=%d, 选中目标mindex=%d, select_mode=%d\n", 
      //            validEnemyCount, mindex, Aim.select_mode);
      // }
      
      //------------------------------------------------自瞄------------------------------------------------\\
      
      // 英雄配置表（全局或类成员）
      // 英雄配置表（全局或类成员）
      const std::unordered_map<int, std::pair<float, float>> HeroConfig = {
          {196, {25000.0f, 16556.25f}},   // 百里守约
          {175, {12000.0f, 15714.429f}},  // 钟馗
          {157, {11000.0f, 15714.429f}},  // 火舞
          {108, {13665.0f, 15183.339f}},  // 墨子
          {142, {10075.0f, 11715.0f}},    // 安琪拉
          {133, {10000.0f, 12500.0f}},    // 狄仁杰
          {174, {15113.0f, 16792.0f}},    // 虞姬
          {195, {8696.0f, 12422.0f}}      // 百里玄策
      };

      if (mindex >= 0 && mindex < BYTE16 && config.distance[mindex] < Aim.distance 
          && object.SelfHp > 0
          && dataTable.heroTemp[mindex].Hp > 0)
      {
          Target = mindex;
          
          // 调试：输出选中的目标信息
          // static int aimDebugCounter = 0;
          // if (aimDebugCounter++ % 60 == 0) {
          //     printf("[自瞄锁定] mindex=%d, 英雄ID=%d, 血量=%d, 距离=%.0f\n", 
          //            mindex, dataTable.heroTemp[mindex].Id, dataTable.heroTemp[mindex].Hp, config.distance[mindex]);
          //     fflush(stdout);
          // }
          
          // 调试输出
          // static int targetDebugCounter = 0;
          // if (targetDebugCounter++ % 60 == 0) {
          //     printf("[✅目标选择] mindex=%d, ID=%d, 距离=%.1f, 自身血量=%d, 目标血量=%d, 角度=%.2f\n", 
          //            mindex, dataTable.heroTemp[mindex].Id, config.distance[mindex], object.SelfHp, 
          //            dataTable.heroTemp[mindex].Hp, acuteAngle[mindex]);
          // }
          
          // iOS风格：移除自瞄视野判断，始终执行自瞄逻辑
          // const int visibilityCheck = Aim.vision ? 257 : -1;
          // const bool isVisible = object.visual[mindex] == visibilityCheck ? true : false;

          bool triggerCondition = (Aim.trigger ? Touch::Touchpoint() : true);
          // static int triggerCounter = 0;
          // if (triggerCounter++ % 120 == 0) {
          //     printf("[触发检测] Aim.trigger=%d, Touchpoint=%d, triggerCondition=%d\n", 
          //            Aim.trigger, Touch::Touchpoint(), triggerCondition);
          // }
          
          if ( true && triggerCondition ) // 移除视野限制
          {
              
              // 优化：缓存自瞄系统的自身坐标地址，避免重复4层ReadValue
              uint64_t aimSelfCoordAddr = ReadValue(ReadValue(ReadValue(ReadValue(object.selfarray +0x6948) +0x10)+0x6)+0x64);
              coord.SelfX = driver->read<int>(aimSelfCoordAddr);
              coord.SelfY = driver->read<int>(aimSelfCoordAddr + 0x8);
              
              // 同时设置目标坐标供distance函数使用
              coord.X = Coord[mindex].X;
              coord.Y = Coord[mindex].Y;       
              
              // 2. 根据当前英雄ID动态设置参数
              float currentDistance = 10000.0f;  // 默认射程
              float V_bullet = Aim.Bulletspeed;         // 默认子弹速度
              if(Aim.Enable)
              {
                  auto it = HeroConfig.find(object.SelfId);
                  if (it != HeroConfig.end()) {       
                      currentDistance = it->second.first;
                      V_bullet = it->second.second;
                      Aim.distance = currentDistance;
                  }
              }                
              
              // 3. 计算当前距离
              auto dist = q2djl_local(Vector2{coord.SelfX, coord.SelfY}, Vector2{Coord[mindex].X, Coord[mindex].Y});        
              
              // 4. 计算子弹飞行时间
              float time = dist / V_bullet;
              
              // 5. 获取目标速度向量（直接使用已存在的 object.Heroaddr[mindex]）
              Vector2 targetVelocity = getXyPos(object.Heroaddr[mindex]);
              float angleRadians;              
              angleRadians = atan2(targetVelocity.y, targetVelocity.x);
                                      
              // 6. 计算预测位移（使用已读取的全局数组数据，避免重复读取）
              Vector2 predictedOffset;
              predictedOffset.x = cos(angleRadians) * ((object.speed[mindex] * 0.2)+72) * time * (Aim.Predictionsed * 0.25);
              predictedOffset.y = sin(angleRadians) * ((object.speed[mindex] * 0.2)+795) * time * (Aim.Predictionsed * 0.25);
              
              // 7. 调试输出
              // static int predictDebugCounter = 0;
              // if (predictDebugCounter++ % 60 == 0) {
              //     printf("[HeroID:%d] 射程=%.2f 子弹速度=%.2f 距离=%.2f 预判时间=%.2fs\n", 
              //            object.SelfId, Aim.distance, V_bullet, dist, time);
              // }
              
              // 8. 应用指数平滑
              const float smoothFactor = Aim.Smoothness;
              Predictions[mindex].x = Predictions[mindex].x * (1 - smoothFactor) + predictedOffset.x * smoothFactor;
              Predictions[mindex].y = Predictions[mindex].y * (1 - smoothFactor) + predictedOffset.y * smoothFactor;
              

              if(mindex == mindex)
              {
                  // 优化：缓存自瞄目标坐标地址，避免重复4层ReadValue
                  uint64_t aimTargetCoordAddr = ReadValue(ReadValue(ReadValue(ReadValue(object.Heroaddr[mindex] +0x248) +0x10)+0x0)+0x60);
                  auto visiCoord = Vector2{(float)driver->read<int>(aimTargetCoordAddr)
                  ,(float)driver->read<int>(aimTargetCoordAddr + 0x8)};                
                  //std::cout<< "GetPlayerPos - AimTarget\n";
                  
                  if (isInRange_local({visiCoord.x, visiCoord.y}, Vector2{Coord[mindex].X, Coord[mindex].Y} - 1,
                                Vector2{Coord[mindex].X, Coord[mindex].Y} + 1)) {
                      Coord[mindex].X = visiCoord.x;
                      Coord[mindex].Y = visiCoord.y;
                  }
                  
                  // 根据目标状态应用预判（使用已读取的全局数组）
                  if(object.state[mindex] == 1 || object.Displacement[mindex] == 1)
                  {
                      Coord[mindex].X += Predictions[mindex].x;
                      Coord[mindex].Y += Predictions[mindex].y;
                      AimCoorde.X = Coord[mindex].X;
                      AimCoorde.Y = Coord[mindex].Y;
                      Aimcoor(AimCoorde);
                      ImGui::GetForegroundDrawList()->AddCircleFilled({AIM_coord.X, AIM_coord.Y}, TXdx*0.6f, IM_COL32(255, 105, 180, 255), 16);
                  }
                  else
                  {
                      Coord[mindex].X = Coord[mindex].X;
                      Coord[mindex].Y = Coord[mindex].Y;
                  }
                  
                  // 10. 计算瞄准方向
                  const float x = Coord[mindex].X - coord.SelfX;
                  const float y = Coord[mindex].Y - coord.SelfY;
                  float c = sqrt(x * x + y * y);
                  if (c > 0) {
                      Angle.x = x / c * localplayerCamp;
                      Angle.y = y / c * localplayerCamp;
                      if(downPos.NotHaveZero())
                      {
                          upLoadPos = downPos + Vector2(Angle.y, Angle.x) * 260;
                          upLoadPosN = upLoadPos * Touch::GetScale();
                          upLoadPosN = Touch::Touch2Screen(upLoadPosN);
                          if (lastTouchObj) {
                              if (Aim.mode == 0 && lastUpLoadPos != upLoadPos) {
                                  lastUpLoadPos = upLoadPos;
                                  Touch::Move(lastTouchObj, upLoadPos.x, upLoadPos.y);
                                  // printf("[👆触摸移动] mode=%d, x=%.2f, y=%.2f, Angle=(%.3f,%.3f)\n", 
                                  //        Aim.mode, upLoadPos.x, upLoadPos.y, Angle.x, Angle.y);
                              }
                          }
                      }
                  }
              }
          }
          else
          {
              upLoadPos.zero();
              // static int noTriggerCounter = 0;
              // if (noTriggerCounter++ % 120 == 0) {
              //     printf("[⚠️未触发] 触摸条件不满足\n");
              // }
          }
          
      } 
      else 
      {
          upLoadPos.zero();
          // static int noTargetCounter = 0;
          // if (noTargetCounter++ % 120 == 0 && mindex != -1) {
          //     // 详细诊断为什么目标无效
          //     bool check1 = (mindex >= 0);
          //     bool check2 = (mindex < BYTE16);
          //     bool check3 = (config.distance[mindex] < Aim.distance);
          //     bool check4 = (Aim.select_mode == 3 ? (acuteAngle[mindex] < Aim.angle && acuteAngle[mindex] > 0) : true);
          //     bool check5 = (object.SelfHp > 0);
          //     bool check6 = (dataTable.heroTemp[mindex].Hp > 0);
          //     
          //     printf("[⚠️无效目标详情] mindex=%d, BYTE16=%d, dist=%.1f<%.1f=%d, selfHp=%d>0=%d, targetHp=%d>0=%d, mode=%d\n",
          //            mindex, BYTE16, config.distance[mindex], Aim.distance, check3, 
          //            object.SelfHp, check5, dataTable.heroTemp[mindex].Hp, check6, Aim.select_mode);
          //     
          //     if (Aim.select_mode == 3) {
          //         printf("[角度检查] acuteAngle[%d]=%.2f, Aim.angle=%.2f, 满足=%d\n", 
          //                mindex, acuteAngle[mindex], Aim.angle, check4);
          //     }
          // }
      }               

      //------------------------------------------------自瞄------------------------------------------------\\
      
      // 设置触摸回调
      if (!isInitialized) {
          Touch::SetCallBack(&TouchCallBack);
          isInitialized = true;
      }
      
      // 显示自瞄状态和目标信息（位置：小地图方框下方）
      if (Aim.Showtarget && mindex >= 0 && mindex < AroundNumbers) {
          // 显示当前目标信息
          char targetInfo[256];
          snprintf(targetInfo, sizeof(targetInfo), 
                  "自瞄目标: %d | 血量: %d/%d | 距离: %.0f", 
                  mindex, 
                  dataTable.heroTemp[mindex].Hp, 
                  dataTable.heroTemp[mindex].MaxHp,
                  config.distance[mindex]);
          
          // 在小地图方框下方显示目标信息
          float miniMapBottom = SmallMapY + 2.13f * rientation * Wwra + 10.0f;
          ImGui::GetForegroundDrawList()->AddText(
              NULL, 20.0f, 
              ImVec2(SmallMapX + 78.0f, miniMapBottom),
              IM_COL32(255, 255, 0, 255), 
              targetInfo
          );
      }
      
      // 绘制自瞄状态显示
      drawAimStatus();
  }
  
  // 未进入对局时显示等待文字（使用与对局判断相同的变量）
  if (gameStatus == 0) {
        // 静态缓冲区，避免每帧重新分配
        static char timeBuffer[128];
        static char fullText[256];
        static int frameCounter = 0;
        
        // 每30帧更新一次时间（约0.5秒），减少性能开销
        if (frameCounter++ % 30 == 0) {
            auto now = std::chrono::system_clock::now();
            auto now_c = std::chrono::system_clock::to_time_t(now);
            strftime(timeBuffer, sizeof(timeBuffer), "当前时间：%m月%d日 %H:%M:%S", std::localtime(&now_c));
            
            // 拼接完整文本
            snprintf(fullText, sizeof(fullText), 
                     "光头强解密内核开源版 等待进入对局\n频道：GTQNHNB666 QQ群：784316342\n%s", 
                     timeBuffer);
        }
        
        ImVec2 字体尺寸 = ImGui::CalcTextSize(fullText);
        绘制字体描边(30.0f + yxpx / 1000,
                     ImVec2(0.0375 * yxpx, 0.98 * yxpy - 字体尺寸.y),
                     ImColor(255, 255, 255), fullText);
  }
}


