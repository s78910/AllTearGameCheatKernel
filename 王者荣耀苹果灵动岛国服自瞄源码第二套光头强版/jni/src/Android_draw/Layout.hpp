/* 
 * 由光头强内核（原XF内核）开源
 因频道账号被盗用后决定开源，正版为光头强内核，
开源文件仅供学习交流，任何个人或组织可在遵循协议条款的前提下，自由下载、使用、修改、分发本项目代码及衍生作品，无需额外授权。使用过程中请保留原项目版权声明及开源协议文本，不得用于违法违规场景。本项目以“现状”提供，开发者不承担任何直接或间接的使用风险及法律责任，亦不提供商业技术支持。
删除此内容死全家，没几把
* 注入时间: 2025-11-16 20:14:49
 * 联系方式: TG:@GTQNHNB666
QQ群：784316342
 */
#include <imgui.h>
#include <stb_image.h>

static bool MemuSwitch = true, BallSwitch = false;



ImGuiStyle *Style = nullptr;
ImGuiWindow *Window = nullptr;
static bool IsLoGin = true, IsDown = false, IsWin = true;

int Color[3];
ImColor White, mid, dark;
int MenuTab = 1;
int MenuTab2 = 1;
int MenuTab4 = 1;
ImVec2 ImagePos = {0, 0};
ImTextureID testtexure;
ImTextureID PT;
ImTextureID ZY;
ImTextureID SX;
void DrawPT(ImVec2 center, float size)
{
	
	ImDrawList *draw_list = ImGui::GetWindowDrawList();
	draw_list->AddImage(PT,{center.x - size / 2, center.y - size / 2},{center.x + size / 2, center.y + size / 2});
	
}
void DrawZY(ImVec2 center, float size)
{
	
	ImDrawList *draw_list = ImGui::GetWindowDrawList();
	draw_list->AddImage(ZY,{center.x - size / 2, center.y - size / 2},{center.x + size / 2, center.y + size / 2});
	
}
void DrawSX(ImVec2 center, float size)
{
	
	ImDrawList *draw_list = ImGui::GetWindowDrawList();
	draw_list->AddImage(SX,{center.x - size / 2, center.y - size / 2},{center.x + size / 2, center.y + size / 2});
	
}

/*
void DrawLogo(ImVec2 center, float size)
{
switch (Color[0])
{
case 0:
	/*
White = ImColor(255, 77, 77, 255);
mid = ImColor(255, 0, 50, 255);
dark = ImColor(205, 0, 0, 255);
*//*
White = ImColor(255, 250, 250, 255);
mid = ImColor(255, 250, 250, 255);
dark = ImColor(255, 250, 250, 255);
break;
case 1:
White = ImColor(117, 170, 255, 255);
mid = ImColor(66, 138, 254, 255);
dark = ImColor(54, 116, 217, 255);
break;
case 2:
White = ImColor(255, 125, 70, 255);
mid = ImColor(233, 90, 33, 255);
dark = ImColor(253, 110, 53, 255);
break;
case 3:
White = ImColor(255, 125, 70, 255);
mid = ImColor(233, 90, 33, 255);
dark = ImColor(253, 110, 53, 255);
break;
case 4:
White = ImColor(255, 250, 250, 255);
mid = ImColor(255, 250, 250, 255);
dark = ImColor(255, 250, 250, 255);
break;



}
ImDrawList *draw_list = ImGui::GetWindowDrawList();
draw_list->AddTriangleFilled(center, {center.x, center.y - size}, {center.x - float(size / 2.5), center.y - size / 5}, mid);
draw_list->AddTriangleFilled(center, {center.x, center.y - size}, {center.x + float(size / 2.5), center.y - size / 5},White);
/*draw_list->AddTriangleFilled({center.x - float(size / 2), center.y}, {center.x - float(size * 1.2), center.y + float(size / 1.5)}, {center.x - size, center.y - float(size / 1.5)}, mid);
draw_list->AddTriangleFilled({center.x + float(size / 2), center.y}, {center.x + float(size * 1.2), center.y + float(size / 1.5)}, {center.x + size, center.y - float(size / 1.5)}, dark);
draw_list->AddTriangleFilled({center.x - float(size / 2), center.y}, {center.x - float(size * 1.2), center.y + float(size / 1.5)}, {center.x - size / 4, center.y + float(size / 2.7)}, White);
draw_list->AddTriangleFilled({center.x + float(size / 2), center.y}, {center.x + float(size * 1.2), center.y + float(size / 1.5)}, {center.x + size / 4, center.y + float(size / 2.7)}, White);
draw_list->AddTriangleFilled(center, {center.x, center.y + float(size / 1.2)},{center.x - size, center.y - float(size / 1.5)}, dark);
*//*
draw_list->AddTriangleFilled(center, {center.x, center.y + float(size / 1.2)},{center.x + size, center.y - float(size / 1.5)}, mid);
}
*/
ImTextureID createTexture(char* ImagePath)
{
    int w,h,n;
    stbi_uc * data = stbi_load(ImagePath, &w, &h, &n, 0);
    GLuint texture;
    glGenTextures(1, &texture);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    if(n == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w , h , 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w , h , 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    stbi_image_free(data);
    ImTextureID image_id = texture;
    return image_id;
}


bool ButtonTextColored(const ImVec4& col, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    ImGui::TextColoredV(col, fmt, args);
    va_end(args);
    return ImGui::IsItemClicked();
}


void DrawTexture(int x, int y, int w, int h, ImTextureID Image)
{
    ImGui::GetBackgroundDrawList()->AddImage(Image, ImVec2(x, y), ImVec2(x + w, y + h));
}


/*
bool SetTheme(const char *label)
{
    if (ImGui::Combo(label, &Color[0], "默认主题\0暗色主题\0白色主题\0"))
    {
        switch (Color[0])
        {
            case 1:
            break;
            case 2:
            ImGui::StyleColorsClassic();
            break;
            case 3:
            ImGui::StyleColorsLight();
            break;
        }
        return true;
    }
    return false;
}
*/