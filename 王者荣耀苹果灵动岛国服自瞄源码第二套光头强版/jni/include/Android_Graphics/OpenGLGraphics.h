/* 
 * 由光头强内核（原XF内核）开源
 因频道账号被盗用后决定开源，正版为光头强内核，
开源文件仅供学习交流，任何个人或组织可在遵循协议条款的前提下，自由下载、使用、修改、分发本项目代码及衍生作品，无需额外授权。使用过程中请保留原项目版权声明及开源协议文本，不得用于违法违规场景。本项目以“现状”提供，开发者不承担任何直接或间接的使用风险及法律责任，亦不提供商业技术支持。
删除此内容死全家，没几把
* 注入时间: 2025-11-16 20:14:49
 * 联系方式: TG:@GTQNHNB666
QQ群：784316342
 */
//
// Created by ITEK on 2024/2/3.
//

#ifndef ANDROIDIMGUI_OPENGLGRAPHICS_H
#define ANDROIDIMGUI_OPENGLGRAPHICS_H

#include <EGL/egl.h>
#include "AndroidImgui.h"

class OpenGLGraphics : public AndroidImgui {
private:
    struct OpenglTextureData : BaseTexData {
    };

    EGLDisplay m_EglDisplay = EGL_NO_DISPLAY;
    EGLSurface m_EglSurface = EGL_NO_SURFACE;
    EGLContext m_EglContext = EGL_NO_CONTEXT;
public:
    OpenGLGraphics() {
		snprintf(RenderName, sizeof(RenderName), "OpenGL");
    }
    bool Create() override;

    void Setup() override;

    void PrepareFrame(bool resize) override;

    void Render(ImDrawData *drawData) override;

    void PrepareShutdown() override;

    void Cleanup() override;

    BaseTexData *LoadTexture(BaseTexData *tex_data, void *pixel_data) override;

    void RemoveTexture(BaseTexData *tex_data) override;
};


#endif //ANDROIDIMGUI_OPENGLGRAPHICS_H
