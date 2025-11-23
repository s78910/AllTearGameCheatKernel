/* 
 * 由光头强内核（原XF内核）开源
 因频道账号被盗用后决定开源，正版为光头强内核，
开源文件仅供学习交流，任何个人或组织可在遵循协议条款的前提下，自由下载、使用、修改、分发本项目代码及衍生作品，无需额外授权。使用过程中请保留原项目版权声明及开源协议文本，不得用于违法违规场景。本项目以“现状”提供，开发者不承担任何直接或间接的使用风险及法律责任，亦不提供商业技术支持。
删除此内容死全家，没几把
* 注入时间: 2025-11-16 20:14:49
 * 联系方式: TG:@GTQNHNB666
QQ群：784316342
 */
#pragma once

#include <linux/input.h>
#include <vector>
#include <functional>
#include "VectorStruct.h"

extern int abs_ScreenX, abs_ScreenY;
extern void Touch_Init(const ImVec2 &s);
extern void Touch_setOrientation(int orientation_);
extern void Touch_Down(int id, int x, int y);
extern void Touch_Move(int id, int x, int y);
extern void Touch_Up(int id);

namespace Touch {
    struct touchObj {
        bool isVirtual = false;  // 标记是否为虚拟触摸点
        Vector2 pos{};
        int id = 0;
        bool isDown = false;
        
    bool isTmpDown = false;
    int x = 0;
    int y = 0;
    
    int size1 = 0;
	int size2 = 0;
	int size3 = 0;
    };

    struct Device {
        int fd;
        float S2TX;
        float S2TY;
        input_absinfo absX, absY;
        touchObj Finger[10];

        Device() { memset((void *) this, 0, sizeof(*this)); }
    };
    
    

    bool Init(const Vector2 &s, bool p_readOnly);

    void Close();

    void Down(float x, float y);

    void Move(float x, float y);

    void Up();

    void Move(touchObj *touch, float x, float y);

    void Upload();
    
    void SetTouchObstacle(Vector2 *Pos, Vector2 *Size, int Count);

    void SetCallBack(const std::function<void(std::vector<Device> *)> &cb);

    Vector2 Touch2Screen(const Vector2 &coord);

    Vector2 GetScale();

    void setOrientation(int orientation);

    void setOtherTouch(bool p_otherTouch);
    
    void Swipe(const Vector2 &startPos, const Vector2 &endPos, int duration_ms);
    
    void InjectVirtualTouch(int id, const Vector2& pos);
    
    bool Touchpoint();
    
    Vector2 Screen2Touch(const Vector2 &screenCoord) ;
    
    bool FlyTouchpoint();
    
    void PrepareTouch(int id, float x, float y) ;
    
    void FastAimTouch(int id, float x, float y);
    
    
}
