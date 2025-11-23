/* 
 * 由光头强内核（原XF内核）开源
 因频道账号被盗用后决定开源，正版为光头强内核，
开源文件仅供学习交流，任何个人或组织可在遵循协议条款的前提下，自由下载、使用、修改、分发本项目代码及衍生作品，无需额外授权。使用过程中请保留原项目版权声明及开源协议文本，不得用于违法违规场景。本项目以“现状”提供，开发者不承担任何直接或间接的使用风险及法律责任，亦不提供商业技术支持。
删除此内容死全家，没几把
* 注入时间: 2025-11-16 20:14:49
 * 联系方式: TG:@GTQNHNB666
QQ群：784316342
 */
// 防止与kernel.h中的px,py重复定义
#ifndef PX_PY_DEFINED
#define PX_PY_DEFINED
float py = abs_ScreenY/2;  
float px = abs_ScreenX/2;
#endif

static float prevPosX[10] = {0};
static float prevPosY[10] = {0};
static float BprevPosX[41] = {0};
static float BprevPosY[41] = {0};    
static float YprevPosX[25] = {0};
static float YprevPosY[25] = {0};
static float MyprevPosX[25] = {0};
static float MyprevPosY[25] = {0};
static bool touchbuffer = false;
extern float minAngle;

// 声明Matrixindex结构体类型
struct MatrixData {
    float matrix[16];
};

// 声明外部变量
extern struct MatrixData Matrixindex;

// 完整的ObjectData结构体定义
struct ObjectData {
    int SelfHp;
    int SelfId;
    int SelfSkill;
    int SelfSkillId;      // 自身召唤师技能ID
    int SelfSpace1;       // 自身技能1
    int SelfSpace2;       // 自身技能2
    int SelfSpace3;       // 自身技能3
    int SelfVisual;       // 自身视野
    int SelfMaxHp;        // 自身最大血量
    int camp;             // 阵营
    int MaxHp;            // 目标最大血量
    int castle;           // 回城状态
    int ownstate;         // 自身状态
    uint64_t selfarray;
    int Hp[16];
    uint64_t Heroaddr[16];
    int visual[16];
    int speed[16];
    int state[16];        // 目标状态 (1=移动)
    int Displacement[16]; // 位移状态 (1=位移中)
};

extern struct ObjectData object;

// Coord结构体定义（如果未定义）
#ifndef COORD_STRUCT_DEFINED
#define COORD_STRUCT_DEFINED
struct Coord {
    float X;//X轴
    float Y;//Y轴
    float SelfX;//X轴
    float SelfY;//Y轴
    float MapX;
    float MapY;
    
    
    float W;
    float H;    
};
#endif

struct Coord CalMatrixMem(struct Coord coor, const float Matrix[]) {

    
    // 定义一个名为points的Coord结构体变量并初始化为{0, 0}
    struct Coord points = {0};
    // 将coor的X和Y分别除以1000，结果赋值给XM和ZM
    float XM = coor.X / 1000.0f;
    float ZM = coor.Y / 1000.0f;

    // 检查矩阵Matrix的部分元素是否不为0，避免除以0的情况
    if (Matrix[11] && Matrix[15] && Matrix[0] && Matrix[12] && Matrix[9] && Matrix[13] && Matrix[1] && Matrix[5] && Matrix[9] && Matrix[13]) {
        // 计算radio，使用fabs函数获取绝对值
        float radio = (float)fabs(ZM * Matrix[11] + Matrix[15]);
        // 将points的X、Y、W、H分别赋初值为0
        points.X = 0.0f, points.Y = 0.0f, points.W = 0.0f, points.H = 0.0f;
        // 若radio大于0，则进行后续坐标处理
        if (radio > 0) {
            // 计算points的X、Y、W、H，并赋值给对应变量
            points.X = yxpx / 2 + (XM * Matrix[0] + Matrix[12]) / radio * yxpx / 2;
            points.Y = yxpy / 2 - (ZM * Matrix[9] + Matrix[13]) / radio * yxpy / 2;
            points.W = yxpy / 2 - (XM * Matrix[1] + 4.0f * Matrix[5] + ZM * Matrix[9] + Matrix[13]) / radio * yxpy / 2;
            points.H = ((points.Y - points.W) / 2.0f);
        }
        // 返回处理后的points坐标
        return points;
    }
}

struct Coord SelfCalMatrixMem(struct Coord coor, const float Matrix[]) {
    // 定义一个名为points的Coord结构体变量并初始化为{0, 0}
    struct Coord points = {0};
    // 将coor的SelfX和SelfY分别除以1000，结果赋值给XM和ZM
    float XM = coor.SelfX / 1000.0f;
    float ZM = coor.SelfY / 1000.0f;

    // 检查矩阵Matrix的部分元素是否不为0，避免除以0的情况
    if (Matrix[11] && Matrix[15] && Matrix[0] && Matrix[12] && Matrix[9] && Matrix[13] && Matrix[1] && Matrix[5] && Matrix[9] && Matrix[13]) {
        // 计算radio，使用fabs函数获取绝对值
        float radio = (float)fabs(ZM * Matrix[11] + Matrix[15]);
        // 将points的X、Y、W、H分别赋初值为0
        points.X = 0.0f, points.Y = 0.0f, points.W = 0.0f, points.H = 0.0f;
        // 若radio大于0，则进行后续坐标处理
        if (radio > 0) {
            // 计算points的X、Y、W、H，并赋值给对应变量
            points.X = yxpx / 2 + (XM * Matrix[0] + Matrix[12]) / radio * yxpx / 2;
            points.Y = yxpy / 2 - (ZM * Matrix[9] + Matrix[13]) / radio * yxpy / 2;
            points.W = yxpy / 2 - (XM * Matrix[1] + 4.0f * Matrix[5] + ZM * Matrix[9] + Matrix[13]) / radio * yxpy / 2;
            points.H = ((points.Y - points.W) / 2.0f);
        }
        // 返回处理后的points坐标
        return points;
    }
}



struct Coord smoothPosition(int i, float currentPosX, float currentPosY, float* prevPosX, float* prevPosY) {

    float targetPosX,targetPosY;
    
	targetPosX = currentPosX;
	targetPosY = currentPosY;

	if (prevPosX[i] != 0.0f && prevPosY[i] != 0.0f && currentPosX - prevPosX[i] < 750.0f && currentPosY - prevPosY[i] < 750.0f) {
		targetPosX = prevPosX[i] + (currentPosX - prevPosX[i]) * 0.14f;
		targetPosY = prevPosY[i] + (currentPosY - prevPosY[i]) * 0.14f;
	}

	prevPosX[i] = targetPosX;
	prevPosY[i] = targetPosY;
	
	return {targetPosX,targetPosY};
}

void RotateRectangle(ImVec2* vertices, int numVertices, float angle, bool clockwise) {
    static float cosAngle; cosAngle = std::cos(angle);
    static float sinAngle; sinAngle = std::sin(angle);
    static float centerX, centerY; centerX = 0.0f, centerY = 0.0f;
    for (int i = 0; i < numVertices; ++i) {
        centerX += vertices[i].x;
        centerY += vertices[i].y;
    }
    centerX /= numVertices;
    centerY /= numVertices;
    for (int i = 0; i < numVertices; ++i) {
        vertices[i].x -= centerX;
        vertices[i].y -= centerY;
    }
    if (clockwise) {
        sinAngle = -sinAngle;
    } else {
        cosAngle = -cosAngle;
    }
    for (int i = 0; i < numVertices; ++i) {
        float x = vertices[i].x;
        float y = vertices[i].y;
        vertices[i].x = x * cosAngle - y * sinAngle;
        vertices[i].y = x * sinAngle + y * cosAngle;
    }
    for (int i = 0; i < numVertices; ++i) {
        vertices[i].x += centerX;
        vertices[i].y += centerY;
    }
}
// DrawPlayer.hpp 风格：增强的文字描边效果
void 绘制字体描边(float size,int x, int y, ImVec4 color, const char* str)
{
    // 更清晰的黑色描边，使用多层描边提升可读性
    ImU32 strokeColor = IM_COL32(0, 0, 0, 200);  // 更深的黑色描边
    ImGui::GetBackgroundDrawList()->AddText(NULL, size,ImVec2(x + 2, y), strokeColor, str);
    ImGui::GetBackgroundDrawList()->AddText(NULL, size,ImVec2(x - 2, y), strokeColor, str);
    ImGui::GetBackgroundDrawList()->AddText(NULL, size,ImVec2(x, y + 2), strokeColor, str);
    ImGui::GetBackgroundDrawList()->AddText(NULL, size,ImVec2(x, y - 2), strokeColor, str);
    // 对角线描边增强立体感
    ImGui::GetBackgroundDrawList()->AddText(NULL, size,ImVec2(x + 1, y + 1), strokeColor, str);
    ImGui::GetBackgroundDrawList()->AddText(NULL, size,ImVec2(x - 1, y - 1), strokeColor, str);
    // 主文字
    ImGui::GetBackgroundDrawList()->AddText(NULL, size,ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(color), str);
}

const char* Drawcall(int TB4) {
    const char* 召唤师文字 = "";

    if (TB4 == 80102) {
        召唤师文字 = "【治疗】";
    } else if (TB4 == 80103) {
        召唤师文字 = "【晕眩】";
    } else if (TB4 == 80104 || TB4 == 80116 || TB4 == 53391 || TB4 == 53392 || TB4 == 53393) {
        召唤师文字 = "【惩戒】";
    } else if (TB4 == 80105) {
        召唤师文字 = "【干扰】";
    } else if (TB4 == 80107) {
        召唤师文字 = "【净化】";
    } else if (TB4 == 80108) {
        召唤师文字 = "【斩杀】";
    } else if (TB4 == 80109) {
        召唤师文字 = "【疾跑】";
    } else if (TB4 == 80110) {
        召唤师文字 = "【狂暴】";
    } else if (TB4 == 80115) {
        召唤师文字 = "【闪现】";
    } else if (TB4 == 80121) {
        召唤师文字 = "【弱化】";
    } else if (TB4 == 80117) {
        召唤师文字 = "【传送】";
    }

    if (TB4 == 0) {
        召唤师文字 = "【无召唤】";
    }

    return 召唤师文字; 
}


    inline void WorldToScreen(Vector2A *bscreen, Vector2A obj)
    {
        // 初始化输出为0，避免未赋值的情况
        bscreen->X = 0;
        bscreen->Y = 0;
        
        obj.X = obj.X/1000.0f;
        obj.Y = obj.Y/1000.0f;
        
        float camear = (float)fabs(obj.Y * Matrixindex.matrix[11] + Matrixindex.matrix[15]);
        if (camear > 0.001f) {  // 添加小阈值避免除零
                bscreen->X = abs_ScreenX/2 + (Matrixindex.matrix[0] * obj.X + Matrixindex.matrix[12]) / camear * abs_ScreenX/2;
        
        bscreen->Y =abs_ScreenY/2 - (Matrixindex.matrix[9] * obj.Y  + Matrixindex.matrix[13]) / camear * abs_ScreenY/2 ;
        }
    }

inline void 范围圈(Vector2A obj, ImU32 color, float Range, float thickn)
{       

        
        Vector2A l1, l2, l3, l4, l5, l6, l7, l8;
        Vector2A lw1, lw2, lw3, lw4, lw5, lw6, lw7, lw8;
        
        l1 = Vector2A(obj.X - Range+60.0f, obj.Y - Range+160.0f);
       
        l2 = Vector2A(obj.X, obj.Y - Range);
        l3 = Vector2A(obj.X + Range-560.0f, obj.Y - Range+160.0f);
        l4 = Vector2A(obj.X - Range, obj.Y+160.0f);
        l5 = Vector2A(obj.X + Range-80.0f, obj.Y);
        l6 = Vector2A(obj.X - Range, obj.Y + Range-20.0f);
        l7 = Vector2A(obj.X, obj.Y + Range-40.0f);
        l8 = Vector2A(obj.X + Range-80.0f, obj.Y + Range);

        //x -向左，加向右 y -向下，+向上
        WorldToScreen(&lw1, l1);WorldToScreen(&lw2, l2);
        WorldToScreen(&lw3, l3);WorldToScreen(&lw4, l4);
        WorldToScreen(&lw5, l5);WorldToScreen(&lw6, l6);
        WorldToScreen(&lw7, l7);WorldToScreen(&lw8, l8);
        //printf("%f",lw4.X);
        ImGui::GetForegroundDrawList()->AddBezierCurve({lw4.X, lw4.Y}, {lw1.X, lw1.Y}, {lw2.X, lw2.Y}, {lw2.X, lw2.Y}, color, thickn);
        ImGui::GetForegroundDrawList()->AddBezierCurve({lw2.X, lw2.Y}, {lw3.X, lw3.Y}, {lw5.X, lw5.Y}, {lw5.X, lw5.Y}, color, thickn);
        ImGui::GetForegroundDrawList()->AddBezierCurve({lw5.X, lw5.Y}, {lw8.X, lw8.Y}, {lw7.X, lw7.Y}, {lw7.X, lw7.Y}, color, thickn);
        ImGui::GetForegroundDrawList()->AddBezierCurve({lw7.X, lw7.Y}, {lw6.X, lw6.Y}, {lw4.X, lw4.Y}, {lw4.X, lw4.Y}, color, thickn);
        

}



float distance(struct Coord coor) {

  
  float distance = sqrt(pow(coor.SelfX - coor.X, 2) + pow(coor.SelfY - coor.Y, 2));
  
  return distance;
  
}

float Wildistance(struct Coord coor) {

  float X = driver->read<int>(ReadValue(ReadValue(ReadValue(ReadValue(object.selfarray +0x248) +0x10)+0x0)+0x10));
  float Y = driver->read<int>(ReadValue(ReadValue(ReadValue(ReadValue(object.selfarray +0x248) +0x10)+0x0)+0x10) + 0x8);
  
  float distance = sqrt(pow(X - coor.X, 2) + pow(Y - coor.Y, 2));
  
  return distance;
  
}

bool enemy(float distance)
{

  
if(distance < config.Punishment )
{
return true;
}
else
{
return false;
}

}


float calculateAngleDifference(float angle1, float angle2) 
{
    if(angle1<0&&angle2<0)
    {
    return fabs(angle1 - angle2 );
    }
    if(angle1>0&&angle2>0)
    {
    return fabs(angle1 - angle2 );
    }
    if(angle1>0&&angle2<0)
    {
    if(fabs(angle1)+fabs(angle2)>180)
    {
    return (180-fabs(angle1)) + (180-fabs(angle2));
    }
    else
    if(fabs(angle1)+fabs(angle2)<180)
    {
    return fabs(angle1) + fabs(angle2);
    }
    
    
    }
    if(angle1<0&&angle2>0)
    {
    if(fabs(angle1)+fabs(angle2)>180)
    {
    return (180-fabs(angle1)) + (180-fabs(angle2));
    }
    else
    if(fabs(angle1)+fabs(angle2)<180)
    {
    return fabs(angle1) + fabs(angle2);
    }
    }
    
    
}


// 或者你也可以直接通过向量计算角度
float calculateAngleBetweenVectors(Vector2 v1, Vector2 v2) {
  // 计算两个向量的点积
  float dotProduct = v1.x * v2.x + v1.y * v2.y;
  
  // 计算两个向量的模长
  float magnitude1 = sqrt(v1.x * v1.x + v1.y * v1.y);
  float magnitude2 = sqrt(v2.x * v2.x + v2.y * v2.y);
  
  // 防止除以零
  if (magnitude1 < 0.0001f || magnitude2 < 0.0001f) {
    return 0.0f;
  }
  
  // 计算cos值并转换为角度
  float cosValue = dotProduct / (magnitude1 * magnitude2);
  
  // 防止浮点数精度问题导致的域错误
  cosValue = std::max(-1.0f, std::min(1.0f, cosValue));
  
  // 转换为角度 (弧度转角度)
  float angle = acos(cosValue) * 180.0f / M_PI;
  std::vector<float> angles ;
    angles.push_back(angle);
    minAngle = *std::min_element(angles.begin(), angles.end());
    
  return minAngle; // 已经是锐角 (0-90度)
}

// 公共触摸操作函数
void performTouch(int objX, int objY) {
    const int touchX = displayInfo.orientation == 1 ? objX : abs_ScreenX - objX;
    const int touchY = displayInfo.orientation == 1 ? abs_ScreenY - objY : objY;
    Touch::Down(touchY, touchX);
    usleep(5000);     // 5ms延迟
    Touch::Up();
    usleep(250000);   // 250ms延迟
}



