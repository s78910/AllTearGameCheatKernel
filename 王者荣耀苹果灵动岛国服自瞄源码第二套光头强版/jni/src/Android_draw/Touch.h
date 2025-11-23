/* 
 * 由光头强内核（原XF内核）开源
 因频道账号被盗用后决定开源，正版为光头强内核，
开源文件仅供学习交流，任何个人或组织可在遵循协议条款的前提下，自由下载、使用、修改、分发本项目代码及衍生作品，无需额外授权。使用过程中请保留原项目版权声明及开源协议文本，不得用于违法违规场景。本项目以“现状”提供，开发者不承担任何直接或间接的使用风险及法律责任，亦不提供商业技术支持。
删除此内容死全家，没几把
* 注入时间: 2025-11-16 20:14:49
 * 联系方式: TG:@GTQNHNB666
QQ群：784316342
 */
#include "fidenBoard.h"
#include "clipboard.h"

void Rename(const string& 当前名称);

static bool 脚本录制状态=false,操作监听状态=false;
int 脚本录制错误提示 = 0;
int setTypeSelectedIndex = 0;
int hpTypeSelectedIndex = 1;
int menuChild = 1,saveConfigIndex=0;
bool 禁用划出菜单=false;
ImVec2 图标坐标 = ImVec2(400,30);
std::vector<string> 脚本列表;
static bool showSubWindow = true,关闭菜单=false,快捷按钮=true,半血瞄胸=true,自定义脚本=false,预瞄=false,绘制=true,新手指引=false;
using namespace std;
extern char cmd[256];
ImU32 c_透明=IM_COL32(0, 0, 0, 0);
ImU32 c_黑色透明50=IM_COL32(0, 0, 0, 50);
ImU32 c_黑色透明100=IM_COL32(0, 0, 0, 100);
ImU32 c_黑色透明128=IM_COL32(0, 0, 0, 128);
ImU32 c_白色透明50=IM_COL32(255, 255, 255, 50);
ImU32 c_白色透明128=IM_COL32(255, 255, 255, 128);
ImU32 c_白色透明150=IM_COL32(255, 255, 255, 150);
ImU32 c_fafafa=IM_COL32(250, 250, 250, 255);
ImU32 c_cccccc=IM_COL32(204, 204, 204, 255);
ImU32 c_c2c2c2 = IM_COL32(194, 194, 194, 255);
ImU32 c_23292e=IM_COL32(35, 41, 46, 255);
ImU32 c_4023292e=IM_COL32(35, 41, 46, 125);
ImU32 c_2f363c=IM_COL32(47, 54, 60, 255);
ImU32 c_402f363c=IM_COL32(47, 54, 60, 125);
ImU32 c_DAB123 = IM_COL32(218, 177, 35, 255);
ImU32 c_DCD4CA = IM_COL32(220, 212, 202, 255);
ImU32 c_2C88E2 = IM_COL32(44, 136, 226, 255);
ImU32 c_2C88E240 = IM_COL32(44, 136, 226, 128);
ImU32 c_545D6D = IM_COL32(84, 93, 109, 255);
ImU32 c_545D6D40 = IM_COL32(84, 93, 109, 128);
ImU32 c_81A4DC = IM_COL32(0x81, 0xa4, 0xdc, 255);
ImU32 c_FFD49F = IM_COL32(255, 212, 159, 255);
ImU32 c_FFD49F80 = IM_COL32(255, 212, 159, 128);
ImU32 c_16b777 = IM_COL32(0x16, 0xB7, 0x77, 255);
ImU32 c_16b77780 = IM_COL32(0x16, 0xB7, 0x77, 128);
ImU32 c_16b77710 = IM_COL32(0x16, 0xB7, 0x77, 50);
ImU32 c_11243B = IM_COL32(0x11, 0x24, 0x3B, 255);
ImU32 c_11243B80 = IM_COL32(0x11, 0x24, 0x3B, 128);
ImU32 c_558FFD = IM_COL32(0x55, 0x8F, 0xFD, 255);
ImU32 c_9A88FE = IM_COL32(0x9a, 0x88, 0xFe, 255);
ImU32 c_ffb800 = IM_COL32(0xff, 0xb8, 0x00, 255);
ImU32 c_ffb80080 = IM_COL32(0xff, 0xb8, 0x00, 128);
ImU32 c_ff5722 = IM_COL32(0xff, 0x57, 0x22, 255);
ImU32 c_ff572280 = IM_COL32(0xff, 0x57, 0x22, 128);
ImU32 c_ff572210 = IM_COL32(0xff, 0x57, 0x22, 50);
ImU32 c_eeeeee = IM_COL32(0xee, 0xee, 0xee, 255);
ImU32 c_a6afbc = IM_COL32(0xa6, 0xaf, 0xbc, 255);
ImVec4 透明ImVec4 = ImVec4(0, 0, 0, 0);
ImVec4 白色ImVec4 = ImVec4(1,1,1,1);
ImVec4 黑色ImVec4 = ImVec4(0, 0, 0, 1.0f);
ImVec4 灰色ImVec4 = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
ImVec4 灰色ImVec440 = ImVec4(0.1f, 0.1f, 0.1f, 0.5f);
ImVec4 灰色ImVec480 = ImVec4(0.1f, 0.1f, 0.1f, 0.75f);
ImVec4 亮灰色ImVec4 = ImVec4(0.5, 0.5, 0.5, 1);
ImVec4 亮黄色ImVec4 = ImVec4(1.0f, 0.831f, 0.623f, 1.0f);
ImVec4 半透明黑色ImVec410 = ImVec4(0.184314f, 0.211765f, 0.235294f, 0.1f);
ImVec4 半透明黑色ImVec420 = ImVec4(0.184314f, 0.211765f, 0.235294f, 0.2f);

ImVec4 半透明黑色ImVec480 = ImVec4(0.184314f, 0.211765f, 0.235294f, 0.75f);
ImVec4 ImVec423292e = ImVec4(0.137255f, 0.160784f, 0.180392f, 1.0f);
ImVec4 ImVec80423292e = ImVec4(0.137255f, 0.160784f, 0.180392f, 0.75f);
ImVec4 ImVec40423292e = ImVec4(0.137255f, 0.160784f, 0.180392f, 0.5f);
ImVec4 ImVec416b777 = ImVec4(0x16 / 255.0f, 0xB7 / 255.0f, 0x77 / 255.0f, 1.0f);
ImVec4 ImVec416b77780 = ImVec4(0x16 / 255.0f, 0xB7 / 255.0f, 0x77 / 255.0f, 0.75f);
ImVec4 ImVec416b77740 = ImVec4(0x16 / 255.0f, 0xB7 / 255.0f, 0x77 / 255.0f, 0.5f);
ImVec4 ImVec4c2c2c2 = ImVec4(0xC2 / 255.0f, 0xC2 / 255.0f, 0xC2 / 255.0f, 1.0f);
ImVec4 ImVec42C88E2 = ImVec4(0x2C / 255.0f, 0x88 / 255.0f, 0xE2 / 255.0f, 1.0f);
ImVec4 ImVec411243B = ImVec4(0x11 / 255.0f, 0x24 / 255.0f, 0x3B / 255.0f, 1.0f);
ImVec4 ImVec411243B80 = ImVec4(0x11 / 255.0f, 0x24 / 255.0f, 0x3B / 255.0f, 0.75f);
ImVec4 ImVec411243B40 = ImVec4(0x11 / 255.0f, 0x24 / 255.0f, 0x3B / 255.0f, 0.5f);
ImVec4 ImVec4f3c258 = ImVec4(0xf3 / 255.0f, 0xc2 / 255.0f, 0x58 / 255.0f, 1.0f);
ImVec4 ImVec4333B48 = ImVec4(0x33 / 255.0f, 0x3b / 255.0f, 0x48 / 255.0f, 1.0f);
ImVec4 ImVec4fafafa = ImVec4(0xfa / 255.0f, 0xfa / 255.0f, 0xfa / 255.0f, 1.0f);
ImVec4 ImVec4fafafa25 = ImVec4(0xfa / 255.0f, 0xfa / 255.0f, 0xfa / 255.0f, 0.25f);
ImVec4 ImVec4fafafa50 = ImVec4(0xfa / 255.0f, 0xfa / 255.0f, 0xfa / 255.0f, 0.5f);
ImVec4 ImVec4fafafa75 = ImVec4(0xfa / 255.0f, 0xfa / 255.0f, 0xfa / 255.0f, 0.75f);
ImVec4 ImVec49A88FE = ImVec4(0x9a / 255.0f, 0x88 / 255.0f, 0xfe / 255.0f, 1.0f);
ImVec4 ImVec45D6373 = ImVec4(0x5d / 255.0f, 0x63 / 255.0f, 0x73 / 255.0f, 1.0f);
ImVec4 ImVec4ff5722 = ImVec4(0xff / 255.0f, 0x57 / 255.0f, 0x22 / 255.0f, 1.0f);
ImVec4 ImVec4ffb800 = ImVec4(0xff / 255.0f, 0xb8 / 255.0f, 0x00 / 255.0f, 1.0f);
ImVec4 ImVec4ffb80040 = ImVec4(0xff / 255.0f, 0xb8 / 255.0f, 0x00 / 255.0f, 0.5f);
ImVec4 ImVec4E39B32 = ImVec4(0xe3 / 255.0f, 0x9b / 255.0f, 0x32 / 255.0f, 1.0f);
ImVec4 ImVec4E39B3220 = ImVec4(0xe3 / 255.0f, 0x9b / 255.0f, 0x32 / 255.0f, 0.2f);
ImVec4 ImVec40062B3 = ImVec4(0x00 / 255.0f, 0x62 / 255.0f, 0xb3 / 255.0f, 1.0f);
ImVec4 ImVec423ade5 = ImVec4(0x23 / 255.0f, 0xad / 255.0f, 0xe5 / 255.0f, 1.0f);
ImVec4 ImVec423ade520 = ImVec4(0x23 / 255.0f, 0xad / 255.0f, 0xe5 / 255.0f, 0.2f);
ImVec4 ImVec4E39B3250 = ImVec4(0xe3 / 255.0f, 0x9b / 255.0f, 0x32 / 255.0f, 0.5f);
ImVec4 ImVec423ade550 = ImVec4(0x23 / 255.0f, 0xad / 255.0f, 0xe5 / 255.0f, 0.5f);
ImVec4 ImVec4193062 = ImVec4(0x19 / 255.0f, 0x30 / 255.0f, 0x62 / 255.0f, 1.0f);
ImVec4 ImVec419306290 = ImVec4(0x19 / 255.0f, 0x30 / 255.0f, 0x62 / 255.0f, 0.90f);
ImVec4 ImVec419306275 = ImVec4(0x19 / 255.0f, 0x30 / 255.0f, 0x62 / 255.0f, 0.75f);
ImVec4 ImVec419306250 = ImVec4(0x19 / 255.0f, 0x30 / 255.0f, 0x62 / 255.0f, 0.5f);
ImVec4 ImVec419306225 = ImVec4(0x19 / 255.0f, 0x30 / 255.0f, 0x62 / 255.0f, 0.25f);



struct 脚本参数 {
    std::string 脚本名称;
    std::vector<ImVec2> 脚本内容;
    std::vector<int> 脚本时长;
    std::vector<std::string> 触摸类型;
    int 脚本倍数=1;
    bool 按钮调整=false;
    bool 重命名=false;
    bool 显示轨迹=false;
    int 播放次数=1;
    int 坐标X=0;
    int 坐标Y=0;
    int 状态=1;
    int 大小=0;
    float lastClickTime=0;
    bool isDragging = false;      // ✨ 新增拖动状态
    ImVec2 dragStartPos;          // ✨ 拖动起始坐标
    ImVec2 buttonStartPos;        // ✨ 按钮初始位置
    bool 允许中断 = true; // 新增字段
};


void SaveJastic(脚本参数& 脚本); // 添加函数声明

struct SliderState {
    ImVec2 startPos;
    bool isDragging = false;
    float distance = 0;
    float angle = 0;
    float lastClickTime = 0;
    std::vector<ImVec2> trailPoints;
    std::vector<ImVec2> tmpTrailPoints;
    std::vector<int> touchIntervals;
    std::vector<std::string> touchType;
    float scrollY=0;
    std::vector<ImVec2> quickSlideBuffer;  // 快速滑动缓冲区
    float slideStartTime = 0.0f;          // 滑动开始时间
    bool isQuickSlide = false;            // 快速滑动标志
    bool isRepeating = false;             // 正在重复标志
    std::mutex slideMutex;         
};

SliderState state1;
SliderState state2;
SliderState state3;
SliderState state4;
SliderState state5;

std::vector<ImVec2> 执行的脚本;
bool 运行脚本 = false;
std::string 执行的脚本名称="";

std::vector<int> 执行的间隔;
std::vector<std::string> 执行的触摸类型;
float 执行的倍数=1;
int 执行的次数=0;
int 剩余的次数=0;
int 执行脚本显示轨迹=0;
std::vector<ImVec2> 执行的脚本点击轨迹;
std::vector<ImVec2> 执行的脚本滑动轨迹;
ImVec2 执行的脚本最新坐标点;
ImVec2 执行的脚本最新轨迹点;
std::vector<脚本参数> 缓存的脚本;
std::vector<std::string> GetFiles(const std::string& sdir, bool bsubdir = false) {
  DIR* dp;
  struct dirent* dirp;
  std::vector<std::string> filenames;
  if ((dp = opendir(sdir.c_str())) != NULL) {
    while ((dirp = readdir(dp)) != NULL) {
      if (strcmp(".", dirp->d_name) == 0 || strcmp("..", dirp->d_name) == 0)
        continue;
      if (dirp->d_type != DT_DIR)
        filenames.push_back(dirp->d_name);
    }
  }
  closedir(dp);
  return filenames;
}
std::vector<std::string> Stringsplit(string str,char split){
    std::vector<std::string> returnVal;
	istringstream iss(str);
	string token;
	while (getline(iss, token, split)){
		returnVal.push_back(token);
	}
	return returnVal;
}

Vector2 rotatePointx(uint32_t orientation, float x, float y, int32_t displayWidth,int32_t displayHeight) {
    if (orientation == 0) {
        return Vector2(x, y);
    }
    Vector2 xy(x, y);
    if (orientation == 3) {
        xy.x = (float)displayHeight - y;
        xy.y = x;
    } else if (orientation == 2) {
        xy.x = (float)displayWidth - x;
        xy.y =(float) displayHeight - y;
    } else if (orientation == 1) {
        xy.x = y;
        xy.y =(float) displayWidth - x;
    }
    return xy;
}
bool 验证文件是否存在并创建写入(const std::string& filename, const std::string& filevalue) {
    // 打开文件以检查其是否存在
    std::ifstream file(filename);
    // 如果文件打开成功
    if (file.good()) {
        // 关闭文件
        file.close();
        // 尝试以输出模式打开文件
        std::ofstream outFile(filename);
        // 如果文件打开成功
        if (outFile.is_open()) {
            // 写入数据
            outFile << filevalue;
            // 关闭文件
            outFile.close();
            // 输出成功信息
            
            return true; // 返回成功
        } else {
            // 输出错误信息
         
            return false; // 返回失败
        }
    } else {
        // 尝试创建新文件
        std::ofstream newFile(filename);
        // 如果文件创建成功
        if (newFile.is_open()) {
            // 写入数据
            newFile << filevalue;
            // 关闭文件
            newFile.close();
            // 输出成功信息
         
            return true; // 返回成功
        } else {
            // 输出错误信息
            
            return false; // 返回失败
        }
    }
}

bool 删除文件(const std::string& filename) {
    if (std::remove(filename.c_str()) == 0) {
        return true;
    }
    return false;
}

bool isNumeric(const std::string& str) {
    if (str.empty()) {
        return false;
    }
    for (char ch : str) {
        if (!std::isdigit(ch) && ch != '-' && ch != '.') {
            return false;
        }
    }
    if (str.find('-') != std::string::npos) {
        if (str.find('-') != 0) {
            return false;
        }
    }
    size_t dotCount = std::count(str.begin(), str.end(), '.');
    if (dotCount > 1) {
        return false;
    }
    return true;
}

//--------------UI--------------
bool CheckboxWith(const char* label, bool* pValue, float yPos,float xPos=0.0f,float fontSize=1.2f) {
    ImGui::SetWindowFontScale(fontSize);
    ImGuiContext& g = *GImGui;
    ImGuiStyle& style = g.Style;
    float SizeWidth = 60;
    ImVec2 pos = ImVec2(0!=xPos?xPos:ImGui::GetCursorScreenPos().x, yPos);
    ImVec2 textSize = ImGui::CalcTextSize(label);
    ImVec2 bgSize = ImVec2(SizeWidth + style.FramePadding.x, SizeWidth + style.FramePadding.x);
    ImVec2 checkBoxSize = ImVec2(SizeWidth + style.FramePadding.y , SizeWidth + style.FramePadding.y  + 15.0f);
    ImU32 bgColor = c_11243B;
    ImU32 borderColor = c_2C88E2;
    ImU32 triangleColor = c_fafafa;
    ImVec2 buttonMin = ImVec2(pos.x, pos.y);
    ImVec2 buttonMax = ImVec2(pos.x + bgSize.x, pos.y + bgSize.y);
    ImVec2 textPos = ImVec2(pos.x + bgSize.x + 10, pos.y + (textSize.y/2));
    ImGui::SetCursorScreenPos(textPos);
    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(ImRect(buttonMin, buttonMax), ImGui::GetID(label), &hovered, &held);
    ImGui::GetWindowDrawList()->AddImage((ImTextureID)checkbox_normal.DS, ImVec2(pos.x,pos.y+5), ImVec2(pos.x + bgSize.x, pos.y + bgSize.y+5));
    if(*pValue){
      //  ImGui::GetWindowDrawList()->AddImage(checkbox_pressed, ImVec2(pos.x,pos.y+5), ImVec2(pos.x + bgSize.x, pos.y + bgSize.y+5));
    }
    if (pressed) {
        *pValue = !(*pValue);
    }
    ImGui::TextColored(白色ImVec4,"%s", label);
    ImGui::SetWindowFontScale(1.0f);
    return pressed;
}
void CustomTextImageBg(const char* text, ImVec2 size, int padding = 5, int alignment = 0,TextureInfo icon=btn,float fontSize=1.0,ImVec4 textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f)) {
  ImGui::SetWindowFontScale(fontSize);
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;
    const ImGuiStyle& style = ImGui::GetStyle();
    const ImGuiID id = window->GetID(text);
    const ImVec2 contentMin = window->DC.CursorPos;
    const ImVec2 contentMax = ImVec2(contentMin.x + size.x, contentMin.y + size.y);
    ImVec2 textSize = ImGui::CalcTextSize(text);
    ImVec2 textPos;
    switch (alignment) {
        case 1:
            textPos = ImVec2(contentMax.x - textSize.x - padding, contentMin.y + (size.y - textSize.y) * 0.5f);
            break;
        case 2:
            textPos = ImVec2(contentMin.x + (size.x - textSize.x) * 0.5f, contentMin.y + (size.y - textSize.y) * 0.5f);
            break;
        default:
            textPos = ImVec2(contentMin.x + padding, contentMin.y + (size.y - textSize.y) * 0.5f);
            break;
    }
   // ImGui::GetWindowDrawList()->AddImage(icon, contentMin, contentMax);
    window->DrawList->AddText(textPos, ImGui::GetColorU32(textColor), text);
    float textHeightWithPadding = textSize.y + padding * 2;
    ImGui::ItemSize(ImVec2(size.x, textHeightWithPadding), style.FramePadding.y);
    if (!ImGui::ItemAdd(ImRect(contentMin, ImVec2(contentMax.x, contentMin.y + textHeightWithPadding)), id))
        return;
    ImGui::SetWindowFontScale(1.0f);
}

bool Button(const char* label,ImVec2 size,float xPos=0,float yPos=0,TextureInfo  buttonBg=btn,bool topText=false,float fontSize=1.2f) {
    ImGui::SetWindowFontScale(fontSize);
    ImGuiIO& io = ImGui::GetIO();
    ImGuiContext& g = *GImGui;
    ImGuiStyle& style = g.Style;
    ImVec2 pos = ImVec2(0!=xPos?xPos:ImGui::GetCursorScreenPos().x, 0!=yPos?yPos:ImGui::GetCursorScreenPos().y);
    ImVec2 textSize = ImGui::CalcTextSize(label);
    ImVec2 bgSize = ImVec2(size.x + style.FramePadding.x, size.y + style.FramePadding.x);
    ImVec2 checkBoxSize = ImVec2(size.x + style.FramePadding.y , size.y + style.FramePadding.y  + 15.0f);
    ImVec2 buttonMin = ImVec2(pos.x, pos.y);
    ImVec2 buttonMax = ImVec2(pos.x + bgSize.x, pos.y + bgSize.y);
    ImVec2 textPos = ImVec2(pos.x + (bgSize.x - textSize.x) * 0.5f, pos.y + (textSize.y/2) - 5);
    ImGui::SetCursorScreenPos(textPos);
    if(topText){
        //ImGui::GetForegroundDrawList()->AddImage(buttonBg, pos, ImVec2(pos.x + bgSize.x, pos.y + bgSize.y));
    }else{
        //ImGui::GetWindowDrawList()->AddImage(buttonBg, pos, ImVec2(pos.x + bgSize.x, pos.y + bgSize.y));
    }
    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(ImRect(buttonMin, buttonMax), ImGui::GetID(label), &hovered, &held);
    ImGui::GetWindowDrawList()->AddText(textPos, ImGui::GetColorU32(ImVec4fafafa), label);
    ImGui::SetWindowFontScale(1.0f);
    if (hovered && io.MouseDown[0] && io.MouseDownDuration[0] >= 0.05) {
        return true;
    }
    ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + bgSize.y + 10));
    return false;
}


void CustomNewLine(float lineHeight = -1.0f) {
    if (lineHeight < 0.0f) {
        ImGui::NewLine();
    } else {
        ImGui::Dummy(ImVec2(0.0f, lineHeight));
    }
}
//--------------UI--------------

void 内容滑动监听(ImDrawList* drawList, ImVec2 size, ImVec2 position, SliderState &state) {
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec2 mousePos = io.MousePos;
    ImVec2 contentMin = ImVec2(position.x, position.y);
    ImVec2 contentMax = ImVec2(position.x + size.x, position.y + size.y);
    drawList->AddRectFilled(contentMin, contentMax, c_透明, 5, 0);
    bool isHovered = mousePos.x >= position.x && mousePos.y >= position.y && mousePos.x < (position.x + size.x) && mousePos.y < (position.y + size.y);
    if (isHovered && io.MouseDown[0]) {
        if (!state.isDragging) {
            state.startPos = mousePos;
            state.isDragging = true;
        } else {
            float delta = state.startPos.y - mousePos.y;
            state.scrollY += delta;
            state.startPos = mousePos;
        }
    } else {
        state.isDragging = false;
        state.distance = 0;
        state.angle = 0;
        state.trailPoints.clear();
    }
}



void 脚本线程(ImDrawList *drawList){
    //static std::vector<ImVec2> activeSlide;
    //static std::vector<std::string> activeTypes;
    
    while (1){
       /* // 检测新输入
        if (运行脚本 && state4.isRepeating) {
            std::lock_guard<std::mutex> lock(state4.slideMutex);
            
            // 更新执行脚本
            if (!执行的脚本.empty()) {
                activeSlide = 执行的脚本;
                activeTypes = 执行的触摸类型;
            }
            
            // 执行滑动操作
            for (size_t i = 0; i < activeSlide.size(); ++i) {
                Vector2 rotatedPoint = rotatePointx(3, 
                    activeSlide[i].x, 
                    activeSlide[i].y, 
                    abs_ScreenX, 
                    abs_ScreenY
                );
                
                Touch::Move(rotatedPoint.x, rotatedPoint.y);
                usleep(1000 * 50); // 50ms间隔
            }
            
            // 检测新输入
            if (!state4.quickSlideBuffer.empty()) {
                activeSlide = state4.quickSlideBuffer;
                activeTypes.clear();
                for (size_t i = 0; i < activeSlide.size(); ++i) {
                    activeTypes.push_back("0=" + std::to_string(i));
                }
                state4.quickSlideBuffer.clear();
            }
        }
       */
        if(运行脚本){
            if (0 < 执行的脚本.size() && 0 < 执行的触摸类型.size() && 0 < 执行的间隔.size()
            && 执行的脚本.size() == 执行的间隔.size()){
                for(int i = 执行的次数; i > 0; i--){
                    剩余的次数 = i;
                    for (int i = 2; i < 执行的触摸类型.size(); i++) {
                        if(2 < 执行的触摸类型[i].size()){
                            std::vector<std::string> 触摸类型=Stringsplit(执行的触摸类型[i], '=');
                            int 类型=std::stoi(触摸类型[0].c_str());
                            int 下标=std::stoi(触摸类型[1].c_str());
                            
                            if(0 <=下标 && 执行的脚本.size()>下标){
                                ImVec2 pos = 执行的脚本[下标];
                                Vector2 rotatedPoint = rotatePointx(3, pos.x, pos.y, abs_ScreenX, abs_ScreenY);
                                if(0==类型){
                                    Touch::Move(rotatedPoint.x,rotatedPoint.y);
                                    执行的脚本滑动轨迹.push_back(pos);
                                    
                                }else if(1==类型){
                                    
                                    Touch::Down(rotatedPoint.x,rotatedPoint.y);
                                    执行的脚本点击轨迹.push_back(pos);    
                                    usleep( 50000 );                               
                                    Touch::Up();
                                    usleep( 50000 );   
                                }
                                usleep( 100 * 执行的间隔[下标] * 执行的倍数);
                            }
                        }
                    }
                    Touch_Up(8);
                    执行的脚本点击轨迹.clear();
                    执行的脚本滑动轨迹.clear();
                }
                执行的脚本.clear();
                执行的触摸类型.clear();
            }else{
                执行的脚本点击轨迹.clear();
                执行的脚本滑动轨迹.clear();
                执行的脚本.clear();
                执行的触摸类型.clear();
            }
            运行脚本=false;
            
            执行的脚本名称="";
        }
        usleep(1000 * 2);
    }
}


void 脚本录制按钮组(ImVec2 size,ImVec2 pos,SliderState &state){
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowPos(pos);
    ImGui::Begin("脚本录制按钮组", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground);
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4 defaultButtonColor = style.Colors[ImGuiCol_Button];
        ImVec4 defaultButtonActiveColor = style.Colors[ImGuiCol_ButtonActive];
        ImVec4 defaultButtonHoveredColor = style.Colors[ImGuiCol_ButtonHovered];
        float defaultWindowPadding = style.WindowPadding.x;
        float defaultFrameRounding = style.FrameRounding;

        ImGui::SetWindowFontScale(1.5f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));
        style.FrameRounding = 10.0f;

        if (ImGui::Button("取消", ImVec2(150, 100))) {
            showSubWindow = true;
            脚本录制状态 = false;
            操作监听状态 = false;
            禁用划出菜单 = false;
            state.tmpTrailPoints.clear();
        }
        ImGui::SameLine();
        if (ImGui::Button("重录", ImVec2(150, 100))) {
            state.tmpTrailPoints.clear();
            操作监听状态=true;
        }
        ImGui::SameLine();
        if (ImGui::Button("保存并退出", ImVec2(250, 100))) {
            showSubWindow = true;
            脚本录制状态 = false;
            操作监听状态 = false;
            禁用划出菜单 = false;

            if (0 >= state.tmpTrailPoints.size() || 0 >= state.touchType.size() || 0 >= state.touchIntervals.size()
            || state.tmpTrailPoints.size() != state.touchType.size() || state.tmpTrailPoints.size() != state.touchIntervals.size()  ){
                std::string 文件名="脚本"+std::to_string(脚本列表.size()+1);
                std::string scriptValue="";
                std::string touchType="";
                std::string touchIntervalsValue="";
                std::string 脚本设置参数="0,1,1,100,100,1,0,0,0";//是否显示轨迹，播放速度，播放次数，x，y，状态，大小,调整
                
                int totalPoints = static_cast<int>(state.tmpTrailPoints.size());
                int loopEnd = totalPoints - 4; // 跳过最后4个点
                if (loopEnd < 0) loopEnd = 0;

                // 关键修复：限制循环不超过实际容器大小
                
                //loopEnd = std::min(loopEnd, static_cast<int>(state.touchType.size()));
                //loopEnd = std::min(loopEnd, static_cast<int>(state.tmpTrailPoints.size()));
                
                for (int i = 0; i < loopEnd; i++) {
                ImVec2 pos = state.tmpTrailPoints[i];
                scriptValue += std::to_string(pos.x) + "," + std::to_string(pos.y) + "|";                
                if(i<=state.touchType.size())
                {
                touchType += state.touchType[i] + "|";
                }
                touchIntervalsValue += std::to_string(state.touchIntervals[i]) + "|";
                
                }
                /*
                for(int i=0;i<state.tmpTrailPoints.size()-4;i++){
                    ImVec2 pos=state.tmpTrailPoints[i];
                    scriptValue+=std::to_string(pos.x)+","+std::to_string(pos.y)+"|";
                    touchType+=state.touchType[i]+"|";
                    touchIntervalsValue+=std::to_string(state.touchIntervals[i])+"|";
                }*/
       
                
                验证文件是否存在并创建写入(string(cmd)+"Script/"+文件名,scriptValue);
                
                验证文件是否存在并创建写入(string(cmd)+"Type/"+文件名,touchType);
                
                验证文件是否存在并创建写入(string(cmd)+"Config/"+文件名,脚本设置参数);
                
                验证文件是否存在并创建写入(string(cmd)+"TouchIntervals/"+文件名,touchIntervalsValue);
                
            }else{
                脚本录制错误提示 = 300;
            }
            state.tmpTrailPoints.clear();
            state.touchType.clear();
            state.touchIntervals.clear();
        }
        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar();
        style.FrameRounding = defaultFrameRounding;
        style.WindowPadding.x = defaultWindowPadding;
        style.Colors[ImGuiCol_Button] = defaultButtonColor;
        style.Colors[ImGuiCol_ButtonActive] = defaultButtonActiveColor;
        style.Colors[ImGuiCol_ButtonHovered] = defaultButtonHoveredColor;
        ImGui::SetWindowFontScale(1.0f);
    ImGui::End();
}

void 脚本录制操作状态(ImVec2 pos,ImVec2 size,SliderState &state){
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowPos(pos);
    ImGui::Begin("脚本录制操作状态", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4 defaultButtonColor = style.Colors[ImGuiCol_Button];
        ImVec4 defaultButtonActiveColor = style.Colors[ImGuiCol_ButtonActive];
        ImVec4 defaultButtonHoveredColor = style.Colors[ImGuiCol_ButtonHovered];
        float defaultWindowPadding = style.WindowPadding.x;
        float defaultFrameRounding = style.FrameRounding;

        ImGui::SetWindowFontScale(1.5f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));
        style.FrameRounding = 50.0f;
        ImGui::Button(0<state.tmpTrailPoints.size()?"操作录制中":"点击屏幕开录制", ImVec2(350, 100));
        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar();
        style.FrameRounding = defaultFrameRounding;
        style.WindowPadding.x = defaultWindowPadding;
        style.Colors[ImGuiCol_Button] = defaultButtonColor;
        style.Colors[ImGuiCol_ButtonActive] = defaultButtonActiveColor;
        style.Colors[ImGuiCol_ButtonHovered] = defaultButtonHoveredColor;
        ImGui::SetWindowFontScale(1.0f);
    ImGui::End();
}

/* 全局变量：记录两次触摸操作之间的时间间隔（单位：毫秒）*/
int 间隔时长记录; 

/* 触摸操作监听函数
   drawList: 绘制列表用于绘制UI元素
   size:     监听区域尺寸
   position: 监听区域位置
   state:    触摸状态记录器（存储坐标、类型等信息）*/
void 操作监听(ImDrawList* drawList, ImVec2 size, ImVec2 position, SliderState &state) {
    // 获取ImGui输入输出对象和样式设置
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    
    // 获取当前鼠标/触摸点坐标
    ImVec2 mousePos = io.MousePos;
    
    // 计算监听区域边界
    ImVec2 contentMin = ImVec2(position.x, position.y);
    ImVec2 contentMax = ImVec2(position.x + size.x, position.y + size.y);
    
    // 绘制半透明背景区域
    drawList->AddRectFilled(contentMin, contentMax, c_透明, 5, 0);
    
    // 检测鼠标是否在监听区域内
    bool isHovered = mousePos.x >= position.x && mousePos.y >= position.y 
        && mousePos.x < (position.x + size.x) 
        && mousePos.y < (position.y + size.y);

    // 处理按下状态
    if (isHovered && io.MouseDown[0]) {
        // 首次按下：记录起始位置并标记为拖动状态
        if (!state.isDragging) {
            state.startPos = mousePos;
            state.isDragging = true;
        } 
        // 持续拖动：计算垂直滚动量
        else {
            float delta = state.startPos.y - mousePos.y;
            state.scrollY += delta;
            state.startPos = mousePos;
        }
        
        // 点击类型判定逻辑
        int 点击下标 = static_cast<int>(state.tmpTrailPoints.size()) - 1;  // 获取当前点索引
        float mouseDownDuration = io.MouseDownDuration[0];  // 获取持续按压时间
        
        // 短按判定（150毫秒内视为点击）
        if (mouseDownDuration < 0.07f) {
            float currentTime = ImGui::GetTime();
            float clickInterval = currentTime - state.lastClickTime;
            
            // 防抖处理：两次点击间隔需大于0.1秒
            if (clickInterval >= 0.1f) {                        
                state.touchType.push_back("1=" + std::to_string(点击下标));  // 记录为点击事件
                state.lastClickTime = currentTime;  // 更新最后点击时间                                
            }
        } 
        // 长按视为滑动事件
        else {
            state.touchType.push_back("0=" + std::to_string(点击下标));  // 记录为滑动事件
        }
        
        // 记录坐标点
        state.tmpTrailPoints.push_back(mousePos);
        
        // 计算时间间隔（从epoch开始的毫秒数）
        auto currentTime = static_cast<int>(ImGui::GetTime() * 1000);
        if (state.touchIntervals.size() >= 1) {
            int interval = currentTime - 间隔时长记录;  // 计算与前次操作的时间差
            state.touchIntervals.push_back(interval);   // 记录时间间隔
            
        } else {
            state.touchIntervals.push_back(20);  // 初始值设为20ms
        }
        间隔时长记录 = currentTime;  // 更新全局时间记录
    } 
    // 松开状态处理
    else {
        state.isDragging = false;  // 重置拖动状态
        state.distance = 0;        // 重置拖动距离
        state.angle = 0;           // 重置拖动角度
    }

    // 绘制轨迹线：当有记录点时显示绿色轨迹线
    if (0 < state.tmpTrailPoints.size()) {
        drawList->AddPolyline(&state.tmpTrailPoints[0], 
            static_cast<int>(state.tmpTrailPoints.size()), 
            c_16b777,    // 使用16进制颜色码#16b777（绿色）
            false,       // 不闭合路径
            7.0f);       // 线宽7像素
    }
    
    /*
        // 定义触发区域（示例为屏幕右侧1/4区域）
    ImVec2 triggerAreaMin(abs_ScreenX * 0.75f, 0);
    ImVec2 triggerAreaMax(abs_ScreenX, abs_ScreenY);

    // 处理按下状态
    if (isHovered && io.MouseDown[0]) {
        // 检测是否在触发区域内
        if (mousePos.x >= triggerAreaMin.x && mousePos.y >= triggerAreaMin.y &&
            mousePos.x <= triggerAreaMax.x && mousePos.y <= triggerAreaMax.y) {
            
            if (!state.isDragging) {
                std::lock_guard<std::mutex> lock(state.slideMutex);
                state.quickSlideBuffer.clear();
                state.slideStartTime = ImGui::GetTime();
                state.isQuickSlide = true;
                
            }
            
            // 记录轨迹点
            if (state.isQuickSlide) {
                std::lock_guard<std::mutex> lock(state.slideMutex);
                state.quickSlideBuffer.push_back(mousePos);
            }
        }
    } 
    // 松开状态处理
    else {
        if (state.isDragging && state.isQuickSlide) {
            float duration = ImGui::GetTime() - state.slideStartTime;
            
            if (duration < 0.5f && state.quickSlideBuffer.size() > 2) {
                std::lock_guard<std::mutex> lock(state.slideMutex);
                printf("interval %f\n",mousePos.x);
                // 转移缓冲区到执行脚本
                执行的脚本.clear();
                执行的触摸类型.clear();
                
                for (size_t i = 0; i < state.quickSlideBuffer.size(); ++i) {
                    执行的脚本.push_back(state.quickSlideBuffer[i]);
                    执行的触摸类型.push_back("0=" + std::to_string(i)); // 滑动操作
                    
                }
                
                执行的倍数 = 1.0f;
                执行的次数 = 9999; // 无限循环
                运行脚本 = true;
                state.isRepeating = true;
            }
            
            state.isQuickSlide = false;
        }
        state.isDragging = false;
    }
    */
}
int 计数=0;
void 缓存脚本(std::string 脚本){
    
    std::string 脚本内容="";
    Touch::Up();
    ifstream ifs;
	ifs.open(string(cmd)+"Script/"+脚本,ios::in);
	if (!ifs.is_open()) {
		return;
	}
	char buf[1024*1024] = {0};
	while (ifs >> buf) {
		脚本内容+=buf;
	}
	ifs.close();
    脚本参数 script;
    script.脚本名称=脚本;
    std::stringstream ss(脚本内容);
    std::vector<std::string> arr=Stringsplit(脚本内容, '|');
    std::vector<std::string> arr2;
    for (std::string pos : arr) {
        arr2=Stringsplit(pos, ',');
        float tx=std::stof(arr2[0].c_str());
        float ty=std::stof(arr2[1].c_str());
        script.脚本内容.push_back(ImVec2(tx,ty));
    }
    
    std::string 脚本间隔="";
    ifstream ifsTouchIntervals;
	ifsTouchIntervals.open(string(cmd)+"TouchIntervals/"+脚本,ios::in);
	if (!ifsTouchIntervals.is_open()) {
		return;
	}
	char bufTouchIntervals[1024*1024] = {0};
	while (ifsTouchIntervals >> bufTouchIntervals) {
		脚本间隔+=bufTouchIntervals;
	}
	ifsTouchIntervals.close();
    std::vector<std::string> arrTouchIntervals=Stringsplit(脚本间隔, '|');
    for (std::string time : arrTouchIntervals) {
        script.脚本时长.push_back(std::stoi(time.c_str()));
        
    }

    std::string 触摸类型="";
    ifstream ifsTouchType;
	ifsTouchType.open(string(cmd)+"Type/"+脚本,ios::in);
	if (!ifsTouchType.is_open()) {
		return;
	}
	char bufTouchType[1024*1024] = {0};
	while (ifsTouchType >> bufTouchType) {
		触摸类型+=bufTouchType;
	}
	ifsTouchType.close();
    script.触摸类型=Stringsplit(触摸类型, '|');

    std::string 脚本设置="";
    ifstream ifsConfig;
	ifsConfig.open(string(cmd)+"Config/"+脚本,ios::in);
	if (!ifsConfig.is_open()) {
		return;
	}
	char bufConfig[1024*1024] = {0};
	while (ifsConfig >> bufConfig) {
		脚本设置+=bufConfig;
	}
	
	ifsConfig.close();
    std::stringstream ssConfig(脚本设置);
    std::vector<std::string> arrConfig=Stringsplit(脚本设置, ',');
    
    //是否显示轨迹，播放速度，播放次数，x，y，状态，大小
    script.显示轨迹=std::stoi(arrConfig[0].c_str());        
    script.脚本倍数=std::stoi(arrConfig[1].c_str());
    script.播放次数=std::stoi(arrConfig[2].c_str());
    script.坐标X=std::stoi(arrConfig[3].c_str());
    script.坐标Y=std::stoi(arrConfig[4].c_str());
    script.状态=std::stoi(arrConfig[5].c_str());
    script.大小=std::stoi(arrConfig[6].c_str());
    script.按钮调整=std::stoi(arrConfig[7].c_str());
    script.重命名=std::stoi(arrConfig[8].c_str());
    缓存的脚本.push_back(script);
}


void 脚本按钮(ImDrawList* drawList, 脚本参数& 脚本, ImU32 backgroundColor = IM_COL32(0, 0, 0, 75)) {
    ImGuiIO& io = ImGui::GetIO();
    int 按钮大小 = 100;
    if (0 == 脚本.大小) {
        按钮大小 = 100;
    } else if (1 == 脚本.大小) {
        按钮大小 = 140;
    } else if (2 == 脚本.大小) {
        按钮大小 = 180;
    }
    ImVec2 size(按钮大小, 按钮大小);
    ImVec2 position(脚本.坐标X, 脚本.坐标Y);
    std::string text = 脚本.脚本名称;
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec2 mousePos = io.MousePos;
    ImVec2 contentMin = ImVec2(position.x, position.y);
    ImVec2 contentMax = ImVec2(position.x + size.x, position.y + size.y);
    ImVec2 textSize = ImGui::CalcTextSize(text.data());
    ImVec2 textPos = ImVec2(position.x + (size.x - textSize.x) * 0.5f, position.y + size.y / 2 - textSize.y / 2);
    drawList->AddRect(contentMin, contentMax, 执行的脚本名称 == 脚本.脚本名称 ? c_16b777 : c_fafafa, size.x, ImDrawCornerFlags_All, 5);
    drawList->AddRectFilled(contentMin, contentMax, backgroundColor, size.x, ImDrawCornerFlags_All);
    drawList->AddText(textPos, ImGui::GetColorU32(执行的脚本名称 == 脚本.脚本名称 ? ImVec4(0x16/255.0f, 0xB7/255.0f, 0x77/255.0f, 1.0f) : ImVec4(0xfa/255.0f, 0xfa/255.0f, 0xfa/255.0f, 1.0f)), text.data());
    bool isHovered = mousePos.x >= position.x && mousePos.y >= position.y && mousePos.x < (position.x + size.x) && mousePos.y < (position.y + size.y);

    // 新增拖动逻辑
    static bool isDragging = false;
    static ImVec2 dragStartPos;
    static ImVec2 buttonStartPos;
    
    if (脚本.按钮调整&&isHovered && io.MouseDown[0]) {
        if (!脚本.isDragging) { // ✨ 使用结构体成员变量
            脚本.dragStartPos = mousePos;
            脚本.buttonStartPos = position;
            脚本.isDragging = true;
        }
        ImVec2 delta = ImVec2{mousePos.x - 脚本.dragStartPos.x,mousePos.y - 脚本.dragStartPos.y};
        脚本.坐标X = 脚本.buttonStartPos.x + delta.x;
        脚本.坐标Y = 脚本.buttonStartPos.y + delta.y;

        std::string 脚本设置参数 = 
            std::string(脚本.显示轨迹 ? "1," : "0,")
            + std::to_string(脚本.脚本倍数) + ","
            + std::to_string(脚本.播放次数) + ","
            + std::to_string(脚本.坐标X) + ","
            + std::to_string(脚本.坐标Y) + ","
            + std::to_string(脚本.状态) + ","
            + std::to_string(脚本.大小)
            + std::string(脚本.按钮调整 ? ",1" : ",0")  
            + std::string(脚本.重命名 ? ",1" : ",0")  
            ;
            验证文件是否存在并创建写入(string(cmd)+"Config/"+脚本.脚本名称,脚本设置参数);

             
    } else {
        脚本.isDragging = false;
    }

    float i = 0;
    if (isHovered && io.MouseDown[0]) {
        for (float alpha = 1.0f; alpha >= 0.0f; alpha -= 0.1f) {
            backgroundColor = IM_COL32(0x16, 0xB7, 0x77, static_cast<int>(128 * alpha));
            drawList->AddRect(ImVec2(position.x + i, position.y + i), ImVec2(position.x + size.x - i, position.y + size.y - i), backgroundColor, size.x, 15, 5);
            i += 5;
        }

        // 原有点击逻辑保持不变
        float mouseDownDuration = io.MouseDownDuration[0];
        float currentTime = ImGui::GetTime();
        float clickInterval = currentTime - 脚本.lastClickTime;
        
        if (!脚本.按钮调整 && io.MouseDownDuration[0] >= 0.03 && clickInterval >= 0.5f) {
            脚本.lastClickTime = currentTime;
            执行的脚本 = 脚本.脚本内容;
            
            执行的触摸类型 = 脚本.触摸类型;
            执行的间隔 = 脚本.脚本时长;
            执行的倍数 = (float)脚本.脚本倍数;
            执行的次数 = 脚本.播放次数;
            执行脚本显示轨迹 = 脚本.显示轨迹;
            if (0 >= 执行的脚本.size() || 0 >= 执行的触摸类型.size() || 0 >= 执行的间隔.size()
                 || 执行的脚本.size() != 执行的间隔.size()) {
                
                执行的脚本点击轨迹.clear();
                执行的脚本滑动轨迹.clear();
                执行的脚本.clear();
                执行的触摸类型.clear();
                运行脚本 = false;
            } else {
                
                
                运行脚本 = true;
                执行的脚本名称 = 脚本.脚本名称;
                
                
            }
        }
    }

    if (脚本.按钮调整) {
        // 原有退出按钮逻辑保持不变
        ImVec2 exitSize(100, 50);
        ImVec2 exitPos(position.x + (size.x - exitSize.x) * 0.5f, position.y + size.y);
        if (mousePos.x >= exitPos.x && mousePos.y >= exitPos.y && mousePos.x < (exitPos.x + exitSize.x) && mousePos.y < (exitPos.y + exitSize.y) && io.MouseDown[0]) {
            脚本.按钮调整 = false;
            禁用划出菜单 = false;
            std::string 脚本设置参数 = 
            std::string(脚本.显示轨迹 ? "1," : "0,")
            + std::to_string(脚本.脚本倍数) + ","
            + std::to_string(脚本.播放次数) + ","
            + std::to_string(脚本.坐标X) + ","
            + std::to_string(脚本.坐标Y) + ","
            + std::to_string(脚本.状态) + ","
            + std::to_string(脚本.大小)
            + std::string(脚本.按钮调整 ? ",1" : ",0")  
            + std::string(脚本.重命名 ? ",1" : ",0")  
            ;
            验证文件是否存在并创建写入(string(cmd)+"Config/"+脚本.脚本名称,脚本设置参数);

        }

        drawList->AddRectFilled(exitPos, ImVec2(exitPos.x + exitSize.x, exitPos.y + exitSize.y), IM_COL32(0, 0, 0, 50));
        drawList->AddText(ImVec2(exitPos.x + 20, exitPos.y + 10), c_fafafa, "退出");
    }

    if (运行脚本 && 执行的脚本名称 == 脚本.脚本名称) {
        // 原有运行状态显示逻辑保持不变
        ImVec2 exitSize(100, 50);
        ImVec2 exitPos(position.x + (size.x - exitSize.x) * 0.5f, position.y + size.y);
        drawList->AddRectFilled(exitPos, ImVec2(exitPos.x + exitSize.x, exitPos.y + exitSize.y), IM_COL32(0, 0, 0, 50));
        std::string 剩余次数 = std::to_string(剩余的次数);
        ImVec2 textSize = ImGui::CalcTextSize(剩余次数.data());
        ImVec2 textPos = ImVec2(position.x + (exitSize.x - textSize.x) * 0.5f, position.y + exitSize.y / 2 - textSize.y / 2);
        drawList->AddText(ImVec2(textPos.x, exitPos.y + 10), c_fafafa, 剩余次数.data());
    }
}
void 提示信息(ImDrawList* drawList,std::string text,ImVec2 position,ImVec2 size,float fontSize=1.2f) {
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 mousePos = io.MousePos;
    ImVec2 contentMin = ImVec2(position.x, position.y);
    ImVec2 contentMax = ImVec2(position.x + size.x, position.y + size.y);
    drawList->AddRectFilled(contentMin, contentMax, c_ff5722, 10, ImDrawCornerFlags_All);
    ImVec2 textSize = ImGui::CalcTextSize(text.data());
    ImVec2 textPos = ImVec2(position.x + (size.x - textSize.x) * 0.5f, position.y + size.y/2 - textSize.y/2);
    drawList->AddText(textPos, ImGui::GetColorU32(ImVec4fafafa), text.data());
}

// 定义一个函数，用于绘制所有缓存的脚本按钮
void 脚本按钮组() {
    // 如果缓存的脚本列表不为空
    if (0 < 缓存的脚本.size()) {
        // 遍历缓存的脚本列表
        for (脚本参数& script : 缓存的脚本) {
            // 如果处于激活状态，则绘制该脚本按钮
            if (script.状态) {
                脚本按钮(ImGui::GetForegroundDrawList(), script);
            }
        }
    }
}
const char* 按钮大小[] = {"小", "中", "大"};
ImVec2 虚拟键盘size(1180,780);
ImVec2 虚拟键盘pos(800,800);
fidenBoard::FidenBoard *keyboard;
bool 打开虚拟键盘=false;
bool 打开键盘=false;
bool 清空键盘内容=false;

void 点击输入框调用() {
    虚拟键盘size=ImVec2(830,870);
    虚拟键盘pos=ImVec2(abs_ScreenX-虚拟键盘size.x,abs_ScreenY/2-虚拟键盘size.y/2);
    打开虚拟键盘=true;
    清空键盘内容=true;
}

std::string 输入框名称;
std::string 键盘内容;
int 数字键盘内容 = 0;
std::vector<std::string> 不支持的内容;

// 在全局或类成员中添加窗口状态变量
static bool 数字键盘窗口焦点 = false;
static ImVec2 数字键盘窗口位置 = ImVec2(100, 100); // 初始位置
static ImVec2 数字键盘窗口大小 = ImVec2(338, 558);  // 初始大小

void 数字虚拟键盘()
{
    

    // 设置窗口属性
    ImGui::SetNextWindowSize(数字键盘窗口大小, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(数字键盘窗口位置, ImGuiCond_FirstUseEver);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, 透明ImVec4);
    
    // 创建悬浮窗口
    ImGuiWindowFlags window_flags = 
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoSavedSettings;

    if (ImGui::Begin("悬浮数字键盘", &打开虚拟键盘, window_flags))
    {
        数字键盘窗口焦点 = ImGui::IsWindowFocused();
        ImGui::PushStyleColor(ImGuiCol_Text, 白色ImVec4);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);

        // 保存窗口位置和尺寸
        数字键盘窗口位置 = ImGui::GetWindowPos();
        数字键盘窗口大小 = ImGui::GetWindowSize();
        
        
        
        // 输入显示区域
        ImGui::SetWindowFontScale(1.25f);
        ImGui::Text("%s", keyboard->getInputText().c_str());
        ImGui::Separator();

        // 键盘布局
        ImGui::PushStyleColor(ImGuiCol_Button, 透明ImVec4);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, 透明ImVec4);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 透明ImVec4);

        // 创建数字键盘
        keyboard = new fidenBoard::FidenBoard(数字键盘窗口大小, fidenBoard::flags_Numbers);

        // 输入验证逻辑（保持原有功能）
        const std::string inputText = keyboard->getInputText();
        if("-" != inputText){
            if(isNumeric(inputText)){
                数字键盘内容 = std::stoi(inputText);
                if(!不支持的内容.empty()){
                    for(std::string 内容 : 不支持的内容){
                        std::vector<std::string> 类型内容 = Stringsplit(内容, '=');
                        if("no" == 类型内容[0]){
                            if(inputText == 类型内容[1]){
                                keyboard->setInputText("");
                            }
                        }
                        else if("min" == 类型内容[0]){
                            if(std::stoi(类型内容[1]) > std::stoi(inputText)){
                                keyboard->setInputText(类型内容[1]);
                            }
                        }
                    }
                }
            }
            else{
                keyboard->setInputText("");
            }
        }

        // 收起按钮
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 110);
        if(ImGui::Button("收起", ImVec2(80, 60))){
            打开虚拟键盘 = false;
            输入框名称 = "";
            keyboard->setInputText("");
                 
        }

        // 清空逻辑
        if(清空键盘内容){
            keyboard->setInputText("");
            清空键盘内容 = false;
        }

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(6);
    }  // 结束if(Begin)
    ImGui::End();  // End()必须在if块外
    ImGui::PopStyleColor();
}

static 脚本参数* 当前重命名脚本 = nullptr; // 记录当前正在重命名的脚本

void 虚拟键盘(ImVec2 size = 虚拟键盘size, ImVec2 pos = 虚拟键盘pos)
{
    
    static fidenBoard::FidenBoard* keyboard = nullptr;
    
    // 初始化键盘实例
    

    ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, 透明ImVec4);

    ImGuiWindowFlags flags = 
        ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize;

    if (ImGui::Begin("Smart Keyboard", &打开键盘, flags))
    {
        // 输入显示区域
        ImGui::SetWindowFontScale(1.3f);
        ImGui::TextColored(ImVec4(1,1,0,1), "当前输入: ");
        ImGui::SameLine();
        ImGui::Text("%s", keyboard->getInputText().c_str());
        键盘内容 = keyboard->getInputText().c_str();
        // 功能按钮行
        ImGui::Separator();
        if(ImGui::Button("粘贴", {100,60})) 
        {
            keyboard->setInputText(getClipboardText());
            
        }
        
        // 动态布局调整
        ImGui::SameLine(ImGui::GetWindowWidth() - 220);
        if(ImGui::Button("清空", {100,60})) {
            keyboard->setInputText("");
        }
        
        ImGui::SameLine();
        if(ImGui::Button("收起", ImVec2(80, 60))){
    if (当前重命名脚本 && !键盘内容.empty()) {
        // 执行重命名操作
        Rename(当前重命名脚本->脚本名称); 
        当前重命名脚本->脚本名称 = 键盘内容; // 更新缓存名称
        SaveJastic(*当前重命名脚本); // 保存配置
        当前重命名脚本 = nullptr; // 重置指针
    }
    打开键盘 = false;
    输入框名称 = "";
    keyboard->setInputText("");
}

         // 键盘主体布局
         ImGui::Separator();
         keyboard = new fidenBoard::FidenBoard(size);
         fidenBoard::FidenBoard chineseBoard(size, fidenBoard::flags_Chinese);

    }  // 结束if(Begin)
    ImGui::End();  // End()必须在if块外
    ImGui::PopStyleColor();
}

void SaveJastic(脚本参数& 脚本) {
            
            std::string 脚本设置参数 = 
            std::string(脚本.显示轨迹 ? "1," : "0,")
            + std::to_string(脚本.脚本倍数) + ","
            + std::to_string(脚本.播放次数) + ","
            + std::to_string(脚本.坐标X) + ","
            + std::to_string(脚本.坐标Y) + ","
            + std::to_string(脚本.状态) + ","
            + std::to_string(脚本.大小)
            + std::string(脚本.按钮调整 ? ",1" : ",0")  
            + std::string(脚本.重命名 ? ",1" : ",0")  
            ;

验证文件是否存在并创建写入(string(cmd)+"Config/"+脚本.脚本名称,脚本设置参数);
}



std::vector<std::string> 初始化目标文件夹() 
{
    std::vector<std::string> 文件夹列表;
    char cwd[1024];
    
    if (getcwd(cwd, sizeof(cwd))) {
        std::string base_path = std::string(cwd) + "/配置/";
        
        // 创建必要目录
        const int dir_mode = 0755; // rwxr-xr-x
        mkdir(base_path.c_str(), dir_mode);
        mkdir((base_path + "Script").c_str(), dir_mode);
        mkdir((base_path + "Config").c_str(), dir_mode);
        mkdir((base_path + "Type").c_str(), dir_mode);
        mkdir((base_path + "TouchIntervals").c_str(), dir_mode);

        // 填充路径列表
        文件夹列表 = {
            base_path + "Script/",
            base_path + "Config/",
            base_path + "Type/",
            base_path + "TouchIntervals/"
        };
    } else {
        // 错误处理
        perror("无法获取当前工作目录");
        文件夹列表 = {
            "./配置/Script/",
            "./配置/Config/",
            "./配置/Type/",
            "./配置/TouchIntervals/"
        };
    }
    
    return 文件夹列表;
}


std::vector<std::string> 目标文件夹 = 初始化目标文件夹();



// 修改后的GetFiles函数（支持多目录）
vector<string> GetFiles(const vector<string>& dirs) {
    vector<string> all_files;
    for (const auto& dir : dirs) {
        DIR* dp;
        struct dirent* dirp;
        if ((dp = opendir(dir.c_str()))) {
            while ((dirp = readdir(dp)) != NULL) {
                if (strcmp(".", dirp->d_name) == 0 || strcmp("..", dirp->d_name) == 0)
                    continue;
                if (dirp->d_type != DT_DIR) {
                    // 构造完整路径
                    string full_path = dir;
                    if (dir.back() != '/') full_path += "/";
                    full_path += dirp->d_name;
                    all_files.push_back(full_path);
                }
            }
            closedir(dp);
        }
    }
    return all_files;
}

bool File_Exists(const string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

bool File_Rename(const string& old_path, const string& new_path) {
    return rename(old_path.c_str(), new_path.c_str()) == 0;
}



void Rename(const string& 当前名称) {
    // 获取所有目录的文件列表
    vector<string> 脚本列表 = GetFiles(目标文件夹);
    
    计数++;    
    if (计数 >= 20) {
        计数 = 0;
        缓存的脚本.clear();
    }
    
    if (!脚本列表.empty()) {
        for (const string& 文件全路径 : 脚本列表) {
            // 提取纯文件名
            size_t pos = 文件全路径.find_last_of("/\\");
            string 文件名 = (pos != string::npos) ? 文件全路径.substr(pos+1) : 文件全路径;
            
            // 构造新路径
            string 新路径 = 文件全路径.substr(0, pos+1) + 键盘内容;
            
            if (文件名 == 当前名称) {
                if (File_Exists(文件全路径)) {
                    if (File_Rename(文件全路径, 新路径)) {
                        // 成功处理
                    } else {
                        // 错误处理
                    }
                    
                }
            }
        }
    }
}
