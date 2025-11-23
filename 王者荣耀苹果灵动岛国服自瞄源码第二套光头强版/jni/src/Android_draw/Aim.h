/* 
 * 由光头强内核（原XF内核）开源
 因频道账号被盗用后决定开源，正版为光头强内核，
开源文件仅供学习交流，任何个人或组织可在遵循协议条款的前提下，自由下载、使用、修改、分发本项目代码及衍生作品，无需额外授权。使用过程中请保留原项目版权声明及开源协议文本，不得用于违法违规场景。本项目以“现状”提供，开发者不承担任何直接或间接的使用风险及法律责任，亦不提供商业技术支持。
删除此内容死全家，没几把
* 注入时间: 2025-11-16 20:14:49
 * 联系方式: TG:@GTQNHNB666
QQ群：784316342
 */
#ifndef AIM_H_INCLUDED
#define AIM_H_INCLUDED

// 声明外部变量和函数
extern bool isAimDown;
extern int mindex;
extern void performSmoothAimTouch(float targetX, float targetY, int targetIndex);

Vector2 CurrentPos;
Vector2 FirstPos;
Vector2 TriggerArea = {Aim.TriggerpositionX,Aim.TriggerpositionY};
Vector2 FlyTriggerArea = {Fly.X,Fly.Y};

Vector2 downPos;
Vector2 upLoadPos;
Vector2 upLoadPosN;
Vector2 Angle;

int lastSlot;
float FlySkillsize = Fly.size;
float Skillsize = 范围大小;
Vector2 Predictions[BYTE16];
Touch::touchObj *lastTouchObj;

Vector2 getXyPos(long classthis)
{
    long Address = ReadValue((classthis + 0x120));
    //long Address = ReadValue(ReadValue(ReadValue(ReadValue(classthis+0x120)+0x10)+0x120)+0x130);
    Address = Address + 0x98;
    Vector2 ret;
    ret.x = (float)driver->read<int>(Address);
    ret.y = (float)driver->read<int>(Address + 8);
    return ret;
}



static void TouchAim(std::vector<Touch::Device>* devices) {

 for (auto& device : *devices) {
        for (auto& obj : device.Finger) {
         if(Aim.Start){
 
            if (obj.isDown) {
                if (!isAimDown) {
                    auto pos = Touch::Touch2Screen(obj.pos);                    
                    float rad = q2djl(TriggerArea, pos);
                    float dx = pos.x - TriggerArea.x;

                    if (rad < 范围大小) {
                        lastTouchObj = &obj;
                        isAimDown = true;                        
                        downPos = obj.pos / Touch::GetScale();                        
                        lastSlot = obj.id;
                        // printf("[触摸激活] 触发区域命中! 距离=%.1f, 范围=%.1f\n", rad, 范围大小);
                    }
                    
                } else if (obj.id == lastSlot) {
                    auto pos = Touch::Touch2Screen(obj.pos);
                      
                    if (pos.y < (float)abs_ScreenY / 4 && pos.x  < (float)abs_ScreenX/2) {
                        
                        lastSlot = -1;
                        downPos.zero();
                        isAimDown = false;         
                                                           
                    } else {
                        
                        if (upLoadPosN.y > (float)abs_ScreenY / 4 && upLoadPosN.x > (float)abs_ScreenX/2 && upLoadPos.NotHaveZero() && downPos.NotHaveZero()) {      
                          
                        
                            obj.pos = upLoadPos * Touch::GetScale();
                        
                        }
                    }
                }
            } else if (isAimDown && obj.id == lastSlot) {
                
                lastSlot = -1;
                downPos.zero();
                isAimDown = false;                                
            }
            
            }
        }
    }
    Touch::Upload();
}

static void TraceAim(std::vector<Touch::Device>* devices) {
for (auto& device : *devices) {
    for (auto& obj : device.Finger) {
    if(Aim.Start){                
        if (obj.isDown) {
            if (!isAimDown) {
                auto pos = Touch::Touch2Screen(obj.pos);
                float rad = q2djl(TriggerArea, pos);
                if (rad < 范围大小) {                    
                    isAimDown = true;                    
                    downPos = obj.pos / Touch::GetScale();                    
                    lastSlot = obj.id;
                }
            } else if (obj.id == lastSlot) {
                auto pos = Touch::Touch2Screen(obj.pos);

                if (pos.y < (float)abs_ScreenY / 4 && pos.x < (float)abs_ScreenX/2 ) {                    
                    lastSlot = -1;
                    downPos.zero();
                    isAimDown = false;
                }
            }
        } else if (isAimDown && obj.id == lastSlot) {

            obj.isDown = true;            
            if (upLoadPosN.y > (float)abs_ScreenY / 4 && upLoadPosN.x  > (float)abs_ScreenX/2 && upLoadPos.NotHaveZero() && downPos.NotHaveZero()) {
            obj.pos = upLoadPos * Touch::GetScale();
            }            			
            Touch::Upload();
            usleep(100);
            obj.isDown = false;
            Touch::Upload();
            lastSlot = -1;
            downPos.zero();
            isAimDown = false;
            return;
        }
        
        }
    }
}
Touch::Upload();
}

static void TouchCallBack(std::vector<Touch::Device>* devices) {
    // static int touchDebugCounter = 0;
    // if (touchDebugCounter++ % 300 == 0) { // 每5秒输出一次
    //     printf("[触摸回调] 模式=%d, 自瞄开启=%d\n", Aim.mode, Aim.Start);
    // }
    
    if(Aim.mode == 0)
    {
    TouchAim(devices);
    }
    else
    {
    TraceAim(devices);    
    }
}

#endif // AIM_H_INCLUDED
