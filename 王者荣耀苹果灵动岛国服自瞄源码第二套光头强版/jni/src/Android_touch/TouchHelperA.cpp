/* 
 * 由光头强内核（原XF内核）开源
 因频道账号被盗用后决定开源，正版为光头强内核，
开源文件仅供学习交流，任何个人或组织可在遵循协议条款的前提下，自由下载、使用、修改、分发本项目代码及衍生作品，无需额外授权。使用过程中请保留原项目版权声明及开源协议文本，不得用于违法违规场景。本项目以“现状”提供，开发者不承担任何直接或间接的使用风险及法律责任，亦不提供商业技术支持。
删除此内容死全家，没几把
* 注入时间: 2025-11-16 20:14:49
 * 联系方式: TG:@GTQNHNB666
QQ群：784316342
 */
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <cmath>
#include <linux/input.h>
#include <linux/uinput.h>
#include <vector>
#include <thread>
#include <unordered_map>
#include "spinlock.h"
#include <android/log.h>
#include "imgui.h"
#include "TouchHelperA.h"
#include "Utils.h"
#include "draw.h"
#include "timer.h"

#define maxE 5
#define maxF 10
#define UNGRAB 0
#define GRAB 1
extern int lastSlot;
extern Vector2 CurrentPos;
extern Vector2 FirstPos;
extern Vector2 TriggerArea;
extern Vector2 downPos;
extern float Skillsize;
extern Vector2 upLoadPos;
extern Vector2 upLoadPosN;
extern bool isAimDown;//瞄准
extern bool Start;
extern float FlySkillsize;
extern Vector2 FlyTriggerArea;





namespace Touch {

    static struct {
        input_event downEvent[2]{{{}, EV_KEY, BTN_TOUCH,       1}, {{}, EV_KEY, BTN_TOOL_FINGER, 1}};
        input_event event[512]{0};
    } input;

    static Vector2 touch_scale;

    static Vector2 screenSize;
    
    
    
    static std::vector<Device> devices;

    static int nowfd;

    static int orientation = 0;

    static bool initialized = false;

    static bool readOnly = false;

    static bool otherTouch = false;

    static std::function<void(std::vector<Device> *)> callback;

    static spinlock lock;
    
    Vector2 *ObstaclePos = nullptr;
    
    Vector2 *ObstacleSize = nullptr;
    
    int ObstacleCount = 0;
    
    std::vector<std::pair<Vector2, Vector2>> swipeActions;
    
    
    
    void Upload() {
        static bool isFirstDown = true;
        int tmpCnt = 0, tmpCnt2 = 0;
        for (auto &device: devices) {
            for (auto &finger: device.Finger) {
                if (finger.isDown) {
                    if (tmpCnt2++ > 20) {
                        goto finish;
                    }
                    input.event[tmpCnt].type = EV_ABS;
                    input.event[tmpCnt].code = ABS_X;
                    input.event[tmpCnt].value = (int) finger.pos.x;
                    tmpCnt++;

                    input.event[tmpCnt].type = EV_ABS;
                    input.event[tmpCnt].code = ABS_Y;
                    input.event[tmpCnt].value = (int) finger.pos.y;
                    tmpCnt++;

                    input.event[tmpCnt].type = EV_ABS;
                    input.event[tmpCnt].code = ABS_MT_POSITION_X;
                    input.event[tmpCnt].value = (int) finger.pos.x;
                    tmpCnt++;

                    input.event[tmpCnt].type = EV_ABS;
                    input.event[tmpCnt].code = ABS_MT_POSITION_Y;
                    input.event[tmpCnt].value = (int) finger.pos.y;
                    tmpCnt++;

                    input.event[tmpCnt].type = EV_ABS;
                    input.event[tmpCnt].code = ABS_MT_TRACKING_ID;
                    input.event[tmpCnt].value = finger.id;
                    tmpCnt++;

                    input.event[tmpCnt].type = EV_SYN;
                    input.event[tmpCnt].code = SYN_MT_REPORT;
                    input.event[tmpCnt].value = 0;
                    tmpCnt++;
                }
            }
        }
        finish:
        bool is = false;
        if (tmpCnt == 0) {
            input.event[tmpCnt].type = EV_SYN;
            input.event[tmpCnt].code = SYN_MT_REPORT;
            input.event[tmpCnt].value = 0;
            tmpCnt++;
            if (!isFirstDown) {
                isFirstDown = true;
                input.event[tmpCnt].type = EV_KEY;
                input.event[tmpCnt].code = BTN_TOUCH;
                input.event[tmpCnt].value = 0;
                tmpCnt++;
                input.event[tmpCnt].type = EV_KEY;
                input.event[tmpCnt].code = BTN_TOOL_FINGER;
                input.event[tmpCnt].value = 0;
                tmpCnt++;
            }
        } else {
            is = true;
        }
        input.event[tmpCnt].type = EV_SYN;
        input.event[tmpCnt].code = SYN_REPORT;
        input.event[tmpCnt].value = 0;
        tmpCnt++;

        if (is && isFirstDown) {
            isFirstDown = false;
            write(nowfd, &input, sizeof(struct input_event) * (tmpCnt + 2));
        } else {
            write(nowfd, input.event, sizeof(struct input_event) * tmpCnt);
        }
    }


    /*void *TypeB(void *arg) {
        int i = (int) (long) arg;
        Device &device = devices[i];
        int latest = 0;
        input_event inputEvent[64]{0};

        while (Touch_initialized) {
            auto readSize = (int32_t) read(origfd[i], inputEvent, sizeof(inputEvent));
            if (readSize <= 0 || (readSize % sizeof(input_event)) != 0) {
                continue;
            }
            size_t count = size_t(readSize) / sizeof(input_event);
            for (size_t j = 0; j < count; j++) {
                input_event &ie = inputEvent[j];
                if (latest < 0)
                    latest = 0;
                if (latest >= 10)
                    continue;
                if (ie.code == ABS_MT_TRACKING_ID) {
                    if (ie.value < 0) {
                        Finger[i][latest].isDown = false;
                    } else {
                        Finger[i][latest].isDown = true;
                    }
                    Finger[i][latest].id = (i * 2 + 1) * maxF + ie.value;
                    continue;
                }
                if (ie.code == ABS_MT_POSITION_X) {
                    Finger[i][latest].isDown = true;
                    Finger[i][latest].x = (int) (ie.value * S2TX);
                    continue;
                }
                if (ie.code == ABS_MT_POSITION_Y) {
                    Finger[i][latest].isDown = true;
                    Finger[i][latest].y = (int) (ie.value * S2TY);
                    continue;
                }
                if (ie.code == SYN_MT_REPORT) {
                    latest += 1;
                    continue;
                }
                if (ie.code == SYN_REPORT) {
                    Upload();
                    memset(&Finger[i][0], 0, sizeof(Finger) * 10);
                    latest = -1;
                    continue;
                }
            }
        }
        return nullptr;
    }*/

    static void *TypeA(void *arg) {
        int i = (int) (long) arg;
        Device &device = devices[i];
        int latest = 0;
        input_event inputEvent[64]{0};        
        //bool touchStarted = false;        
        //std::chrono::steady_clock::time_point touchStartTime;                
        while (initialized) {                
            //auto pos = Touch::Touch2Screen(device.Finger[latest].pos);                
            auto readSize = (int32_t) read(device.fd, inputEvent, sizeof(inputEvent));
            if (readSize <= 0 || (readSize % sizeof(input_event)) != 0) {
                continue;
            }
            size_t count = size_t(readSize) / sizeof(input_event);
            //lock.lock();
            for (size_t j = 0; j < count; j++) {
                input_event &ie = inputEvent[j];
                if (ie.type == EV_ABS) {
                    if (ie.code == ABS_MT_SLOT) {
                        latest = ie.value;
                        continue;
                    }
                    if (ie.code == ABS_MT_TRACKING_ID) {
                        if (ie.value == -1) {
                            device.Finger[latest].isDown = false;


                        } else {
                            device.Finger[latest].id = (i * 2 + 1) * maxF + latest;
                            device.Finger[latest].isDown = true;
                            
                            
                        }
                        continue;
                    }
                    if (ie.code == ABS_MT_POSITION_X) {
                        device.Finger[latest].id = (i * 2 + 1) * maxF + latest;
                        device.Finger[latest].pos.x = (float) ie.value * device.S2TX;
                        device.Finger[latest].isTmpDown = true;
                        continue;
                    }
                    if (ie.code == ABS_MT_POSITION_Y) {
                        device.Finger[latest].id = (i * 2 + 1) * maxF + latest;
                        device.Finger[latest].pos.y = (float) ie.value * device.S2TY;
                        device.Finger[latest].isTmpDown = true;
                        continue;
                    }
                    if (ie.code == ABS_MT_TOUCH_MAJOR)
				    {
					device.Finger[latest].id = (i * 2 + 1) * maxF + latest;
					device.Finger[latest].size1 = ie.value;
					device.Finger[latest].isTmpDown = true;
					continue;
				    }
				    if (ie.code == ABS_MT_WIDTH_MAJOR)
				    {
					device.Finger[latest].id = (i * 2 + 1) * maxF + latest;
					device.Finger[latest].size1 = ie.value;
					device.Finger[latest].isTmpDown = true;
					continue;
				    }
				    if (ie.code == ABS_MT_TOUCH_MINOR)
				    {
					device.Finger[latest].id = (i * 2 + 1) * maxF + latest;
					device.Finger[latest].size1 = ie.value;
					device.Finger[latest].isTmpDown = true;
					continue;
				    }
                }
                if (ie.code == SYN_REPORT) {
                    bool IsObstacle = false;
                    if (ImGui::GetCurrentContext() != nullptr) {
                        ImGuiIO &io = ImGui::GetIO();
                        if (device.Finger[latest].isDown) {
                            
                            auto pos = Touch2Screen(device.Finger[latest].pos); 
                            io.MousePos = ImVec2(pos.x, pos.y);
                            if (io.MousePos.y > (float)abs_ScreenY / 4 && io.MousePos.x > (float)abs_ScreenX/2 ) 
                            {
                            CurrentPos = Vector2{pos.x, pos.y};                            
                            //printf("x:%f y:%f [%d]\n",CurrentPos.x,CurrentPos.y,device.Finger[latest].id);
                            }
                            
                            float rad = q2djl(TriggerArea, Vector2{pos.x, pos.y});
                            
                            if (rad < Skillsize) 
                            {
                            if(FirstPos.x==0.0f&&FirstPos.y==0.0f)
                            {
                            FirstPos = CurrentPos;                                  
                            }

                            if (io.MousePos.y > (float)abs_ScreenY / 4 && io.MousePos.x > (float)abs_ScreenX/2 &&downPos.x==0.0f&&downPos.y==0.0f) 
                            {
                            downPos = device.Finger[latest].pos/Touch::GetScale();
                            //printf("q %f\n",downPos.x);
                            }
                            }
           
                            io.MouseDown[0] = true;                                                                                   
                            for (int i = 0; i < ObstacleCount; i ++) {
                                IsObstacle = (pos.x >= ObstaclePos[i].x && pos.x <= ObstaclePos[i].x + ObstacleSize[i].x)
                                                && (pos.y >= ObstaclePos[i].y && pos.y <= ObstaclePos[i].y + ObstacleSize[i].y);
                                if (IsObstacle) break;
                            }
                        } else {
                            io.MouseDown[0] = false;
                            if(!isAimDown)
                            {
                            FirstPos.zero();                                                        
                            }
                        }                      
                    }
                    
                    if (!readOnly && !IsObstacle) {
                        if (callback) {
                            callback(&devices);
                        } else {
                            if(!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
                            Upload();
                        }
                    }
                    continue;
                }
                
                if (ie.type == EV_SYN)
			{
				if (ie.code == SYN_REPORT)
				{
					if (device.Finger[latest].isTmpDown)
					    Upload();
					continue;
				}
				continue;
			}
            }
            
        }
        return nullptr;
    }

    static bool checkDeviceIsTouch(int fd) {
        uint8_t *bits = NULL;
        ssize_t bits_size = 0;
        int res, j, k;
        bool itmp = false, itmp2 = false, itmp3 = false;
        struct input_absinfo abs{};
        while (true) {
            res = ioctl(fd, EVIOCGBIT(EV_ABS, bits_size), bits);
            if (res < bits_size)
                break;
            bits_size = res + 16;
            bits = (uint8_t *) realloc(bits, bits_size * 2);
        }
        for (j = 0; j < res; j++) {
            for (k = 0; k < 8; k++)
                if (bits[j] & 1 << k && ioctl(fd, EVIOCGABS(j * 8 + k), &abs) == 0) {
                    if (j * 8 + k == ABS_MT_SLOT) {
                        itmp = true;
                        continue;
                    }
                    if (j * 8 + k == ABS_MT_POSITION_X) {
                        itmp2 = true;
                        continue;
                    }
                    if (j * 8 + k == ABS_MT_POSITION_Y) {
                        itmp3 = true;
                        continue;
                    }
                }
        }
        free(bits);
        return itmp && itmp2 && itmp3;
    }

    bool Init(const Vector2 &s, bool p_readOnly) {
        Close();
        devices.clear();
        Vector2 size = s;
        readOnly = p_readOnly;
        if (size.x > size.y) {
            screenSize = size;
        } else {
            screenSize = {size.y, size.x};
        }
        DIR *dir = opendir("/dev/input/");
        if (!dir) {
            return false;
        }

        dirent *ptr = NULL;
        int eventCount = 0;
        while ((ptr = readdir(dir)) != NULL) {
            if (strstr(ptr->d_name, "event"))
                eventCount++;
        }

        char temp[128];
        for (int i = 0; i <= eventCount; i++) {
            sprintf(temp, "/dev/input/event%d", i);
            int fd = open(temp, O_RDWR);
            if (fd < 0) {
                continue;
            }
            if (checkDeviceIsTouch(fd)) {
                Device device{};
                if (ioctl(fd, EVIOCGABS(ABS_MT_POSITION_X), &device.absX) == 0
                    && ioctl(fd, EVIOCGABS(ABS_MT_POSITION_Y), &device.absY) == 0) {
                    device.fd = fd;
                    if (!readOnly) {
                        ioctl(fd, EVIOCGRAB, GRAB);
                    }
                    devices.push_back(device);
                }
            } else {
                close(fd);
            }
        }

        if (devices.empty()) {
            puts("获取屏幕驱动失败");
            return false;
        }
        //LOGD("device count: %zu", devices.size());

        int screenX = devices[0].absX.maximum;
        int screenY = devices[0].absY.maximum;

        if (!readOnly) {
            struct uinput_user_dev ui_dev;
            nowfd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
            if (nowfd <= 0) {
                return false;
            }

            int string_len = rand() % 10 + 5;
            char string[string_len];
            memset(&ui_dev, 0, sizeof(ui_dev));

            genRandomString(string, string_len);
            strncpy(ui_dev.name, string, UINPUT_MAX_NAME_SIZE);
            
            ui_dev.id.bustype = 0;
            ui_dev.id.vendor = rand() % 10 + 5;
            ui_dev.id.product = rand() % 10 + 5;
            ui_dev.id.version = rand() % 10 + 5;

            ioctl(nowfd, UI_SET_PROPBIT, INPUT_PROP_DIRECT);

            ioctl(nowfd, UI_SET_EVBIT, EV_ABS);
            ioctl(nowfd, UI_SET_ABSBIT, ABS_X);
            ioctl(nowfd, UI_SET_ABSBIT, ABS_Y);
            ioctl(nowfd, UI_SET_ABSBIT, ABS_MT_POSITION_X);
            ioctl(nowfd, UI_SET_ABSBIT, ABS_MT_POSITION_Y);
            ioctl(nowfd, UI_SET_ABSBIT, ABS_MT_TRACKING_ID);
            ioctl(nowfd, UI_SET_EVBIT, EV_SYN);
            ioctl(nowfd, UI_SET_EVBIT, EV_KEY);
            ioctl(nowfd, UI_SET_KEYBIT, BTN_TOOL_FINGER);
            ioctl(nowfd, UI_SET_KEYBIT, BTN_TOUCH);

            genRandomString(string, string_len);
            ioctl(nowfd, UI_SET_PHYS, string);
            
            int fd = devices[0].fd;
            {
                struct input_id id{};
                if (ioctl(fd, EVIOCGID, &id) == 0) {
                    ui_dev.id.bustype = id.bustype;
                    ui_dev.id.vendor = id.vendor;
                    ui_dev.id.product = id.product;
                    ui_dev.id.version = id.version;
                }
                uint8_t *bits = NULL;
                ssize_t bits_size = 0;
                int res, j, k;
                while (1) {
                    res = ioctl(fd, EVIOCGBIT(EV_KEY, bits_size), bits);
                    if (res < bits_size)
                        break;
                    bits_size = res + 16;
                    bits = (uint8_t *) realloc(bits, bits_size * 2);
                }
                for (j = 0; j < res; j++) {
                    for (k = 0; k < 8; k++)
                        if (bits[j] & 1 << k) {
                            if (j * 8 + k == BTN_TOUCH || j * 8 + k == BTN_TOOL_FINGER)
                                continue;
                            ioctl(nowfd, UI_SET_KEYBIT, j * 8 + k);
                        }
                }
                free(bits);
            }
            ui_dev.absmin[ABS_MT_POSITION_X] = 0;
            ui_dev.absmax[ABS_MT_POSITION_X] = screenX;
            ui_dev.absmin[ABS_MT_POSITION_Y] = 0;
            ui_dev.absmax[ABS_MT_POSITION_Y] = screenY;
            ui_dev.absmin[ABS_X] = 0;
            ui_dev.absmax[ABS_X] = screenX;
            ui_dev.absmin[ABS_Y] = 0;
            ui_dev.absmax[ABS_Y] = screenY;
            ui_dev.absmin[ABS_MT_TRACKING_ID] = 0;
            ui_dev.absmax[ABS_MT_TRACKING_ID] = 65535;
            write(nowfd, &ui_dev, sizeof(ui_dev));

            if (ioctl(nowfd, UI_DEV_CREATE)) {
                return false;
            }
        }
        initialized = true;

        pthread_t t;
        for (int i = 0; i < devices.size(); i++) {
            devices[i].S2TX = (float) screenX / (float) devices[i].absX.maximum;
            devices[i].S2TY = (float) screenY / (float) devices[i].absY.maximum;
            pthread_create(&t, nullptr, TypeA, (void *) (long) i);
        }
        if (size.x > size.y) {
            std::swap(size.x, size.y);
        }
        if (otherTouch) {
            std::swap(size.x, size.y);
        }
        touch_scale.x = (float) screenX / size.x;
        touch_scale.y = (float) screenY / size.y;

        //system("chmod 000 -R /proc/bus/input/*");
        
        //ObstaclePos = new Vector2 [64];
        //ObstacleSize = new Vector2 [64];
        
        return true;
    }

    void Close() {
        if (initialized) {
            for (auto &device: devices) {
                if (!readOnly)
                    ioctl(device.fd, EVIOCGRAB, UNGRAB);
                close(device.fd);
                device.fd = 0;
            }
            if (nowfd > 0) {
                ioctl(nowfd, UI_DEV_DESTROY);
                close(nowfd);
                nowfd = 0;
            }
            memset(input.event, 0, sizeof(input.event));
            initialized = false;
            devices.clear();
        }
    }

    void Down(float x, float y) {
        lock.lock();
        touchObj &touch = devices[0].Finger[9];
        touch.id = 8;
        touch.pos = Vector2(x, y) * touch_scale;
        touch.isDown = true;
        touch.size1 = 8;
	    touch.size2 = 8;
	    touch.size3 = 8;
        Upload();
        lock.unlock();
    }

    void Move(touchObj *touch, float x, float y) {
        lock.lock();       
        touch->pos = Vector2(x, y) * touch_scale;
        Upload();
        lock.unlock();
    }
    
    void Swipe(const Vector2 &startPos, const Vector2 &endPos, int duration_ms) {
        // 开始位置
        Down(startPos.x, startPos.y);
        std::this_thread::sleep_for(std::chrono::milliseconds(duration_ms / 2));

        // 结束位置
        Up();
        std::this_thread::sleep_for(std::chrono::milliseconds(duration_ms / 2));
        
        // 实现平滑滑动效果
        int steps = 10; // 滑动步骤数
        float stepDuration = duration_ms / (float) steps;
        Vector2 stepDelta = (endPos - startPos) / steps;
        
        for (int i = 1; i < steps; i++) {
            Down(startPos.x + stepDelta.x * i, startPos.y + stepDelta.y * i);
            std::this_thread::sleep_for(std::chrono::milliseconds((int) stepDuration));
            Up();
        }
    }


    void Move(float x, float y) {
    lock.lock();
    touchObj &touch = devices[0].Finger[9];
    touch.pos = Vector2(x, y) * touch_scale;
    touch.isDown = true;
    Upload();
    lock.unlock();
    }

    void Up() {
        lock.lock();
        touchObj &touch = devices[0].Finger[9];
        touch.isDown = false;
        Upload();
        lock.unlock();
    }
    
    void SetTouchObstacle(Vector2 *Pos, Vector2 *Size, int Count) {
        for (int i = 0; i < Count; i ++) {
            ObstaclePos[i] = Pos[i];
            ObstacleSize[i] = Size[i];
        }
        ObstacleCount = Count;
    }

    void SetCallBack(const std::function<void(std::vector<Device> *)> &cb) {
        callback = cb;
    }

    Vector2 Touch2Screen(const Vector2 &coord) {
        float x = coord.x, y = coord.y;
        float xt = x / touch_scale.x;
        float yt = y / touch_scale.y;

        if (otherTouch) {
            switch (orientation) {
                case 1:
                    x = xt;
                    y = yt;
                    break;
                case 2:
                    y = yt;
                    x = screenSize.y - xt;
                    break;
                case 3:
                    x = screenSize.y - xt;
                    y = screenSize.x - yt;
                    break;
                default:
                    y = xt;
                    x = screenSize.y - yt;
                    break;
            }
        } else {
            switch (orientation) {
                case 1:
                    x = yt;
                    y = screenSize.y - xt;
                    break;
                case 2:
                    x = screenSize.y - xt;
                    y = screenSize.x - yt;
                    break;
                case 3:
                    y = xt;
                    x = screenSize.x - yt;
                    break;
                default:
                    x = xt;
                    y = yt;
                    break;
            }
        }
        return {x, y};
    }

    Vector2 Screen2Touch(const Vector2 &screenCoord) {
    float x = screenCoord.x;
    float y = screenCoord.y;
    float xt, yt;

    if (otherTouch) {
        switch (orientation) {
            case 1:
                // 原逻辑：x=xt, y=yt
                xt = x;
                yt = y;
                break;
            case 2:
                // 原逻辑：y=yt, x=screenSize.y - xt → xt=screenSize.y - x, yt=y
                xt = screenSize.y - x;
                yt = y;
                break;
            case 3:
                // 原逻辑：x=screenSize.y - xt, y=screenSize.x - yt → xt=screenSize.y - x, yt=screenSize.x - y
                xt = screenSize.y - x;
                yt = screenSize.x - y;
                break;
            default:
                // 原逻辑：y=xt, x=screenSize.y - yt → xt=y, yt=screenSize.y - x
                xt = y;
                yt = screenSize.y - x;
                break;
        }
    } else {
        switch (orientation) {
            case 1:
                // 原逻辑：x=yt, y=screenSize.y - xt → xt=screenSize.y - y, yt=x
                xt = screenSize.y - y;
                yt = x;
                break;
            case 2:
                // 原逻辑：x=screenSize.y - xt, y=screenSize.x - yt → xt=screenSize.y - x, yt=screenSize.x - y
                xt = screenSize.y - x;
                yt = screenSize.x - y;
                break;
            case 3:
                // 原逻辑：y=xt, x=screenSize.x - yt → xt=y, yt=screenSize.x - x
                xt = y;
                yt = screenSize.x - x;
                break;
            default:
                // 原逻辑：x=xt, y=yt
                xt = x;
                yt = y;
                break;
        }
    }

    // 应用缩放系数转换为设备坐标
    Vector2 touchCoord;
    touchCoord.x = xt * touch_scale.x;
    touchCoord.y = yt * touch_scale.y;
    return touchCoord;
    }

    Vector2 GetScale() {
        return touch_scale;
    }

    void setOrientation(int o) {
        orientation = o;
    }

    void setOtherTouch(bool p_otherTouch) {
        otherTouch = p_otherTouch;
    }
    
    // 检查是否有2个或以上的触摸点
    bool Touchpoint() {
        int count = 0;
        for (auto& device : Touch::devices) {
            for (auto& finger : device.Finger) {
                if (finger.isDown) {
                    count++;
                }
            }
        }    
        return count >= 2 ? true : false;
    }

    bool FlyTouchpoint() {
    int count = 0;
    for (auto& device : Touch::devices) {
        for (auto& finger : device.Finger) {
            auto pos = Touch::Touch2Screen(finger.pos);           
            float rad = q2djl(FlyTriggerArea, pos);
            
            if (rad < FlySkillsize) {
            //printf("CurrentPos %f %f\n",CurrentPos.x,CurrentPos.y);
            if (finger.isDown) {
                count++;
            }
            }
        }
    }
    return count >= 1;
}
    
void InjectVirtualTouch(int id, const Vector2& pos) {
    lock.lock();
    // 在设备0的Finger数组中找一个空闲槽位
    for (auto& finger : devices[0].Finger) {
        if (!finger.isDown) {
            finger.id = id;
            finger.pos = pos * touch_scale;
            finger.isDown = true;
            finger.isVirtual = true;  // 标记为虚拟触摸点
            break;
        }
    }
    lock.unlock();
}






}


#define maxE 5
#define maxF 10
#define UNGRAB 0
#define GRAB 1

struct touchObj {
    bool occupy = false;
    bool isTmpDown = false;
    bool isDown = false;
    bool isUp = false;
    int x = 0;
    int y = 0;
    int id = 0;
    int size1 = 0;
    int size2 = 0;
    int size3 = 0;
};

struct targ {
    int fdNum;
    float S2TX;
    float S2TY;
};

static int Touch_orientation = 0;
// g_Initialized已在draw.h中声明为extern，这里不重复定义
struct touchObj Finger[maxE][maxF];

static pthread_t touchloop;

static struct input_event event[128];
static struct input_event downEvent[2];
static int fdNum = 0, origfd[maxE], nowfd;


static float touch_s = 0.0f;

static ImVec2 touch_screen_size;

static ImVec2 screenSize;

static bool isFirstDown = true;



char *genRandomString(int length) {
    const char *charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    const size_t charsetSize = sizeof(charset) - 1;
    int i;
    char *string = (char *)calloc(length, sizeof(char));

    for (i = 0; i < length - 1; i++) {
        string[i] = charset[rand() % charsetSize];
    }
    string[length - 1] = '\0';

    return string;
}


ImVec2 rotatePointx(uint32_t orientation, ImVec2 mxy, ImVec2 wh) {
    if (orientation == 0) {
        return mxy;
    }
    ImVec2 xy(mxy.x, mxy.y);
    if (orientation == 3) {
        xy.x = (float) wh.y - mxy.y;
        xy.y = mxy.x;
    } else if (orientation == 2) {
        xy.x = (float) wh.x - mxy.x;
        xy.y = (float) wh.y - mxy.y;
    } else if (orientation == 1) {
        xy.x = mxy.y;
        xy.y = (float) wh.x - mxy.x;
    }
    return xy;
}

ImVec2 getTouchScreenDimension(int fd) {
    struct input_absinfo abs_x, abs_y;
    if (ioctl(fd, EVIOCGABS(ABS_MT_POSITION_X), &abs_x) == -1 || ioctl(fd, EVIOCGABS(ABS_MT_POSITION_Y), &abs_y) == -1) {
        // 错误处理
        perror("Error getting touch screen dimensions");
        return ImVec2(0.0f, 0.0f); // 或者使用适当的默认值
    }

    return ImVec2(static_cast<float>(abs_x.maximum), static_cast<float>(abs_y.maximum));
}



void Upload() {
    const int MAX_EVENTS = 128;
    const int MAX_CONSECUTIVE_EVENTS = 10;
    int tmpCnt = 0, tmpCnt2 = 0, i, j;

    for (i = 0; i < fdNum; i++) {
        for (j = 0; j < maxF; j++) {
            if (Finger[i][j].isDown && tmpCnt2 <= MAX_CONSECUTIVE_EVENTS) {
                tmpCnt2++;

                // Fill event array with ABS_X, ABS_Y, ABS_MT_POSITION_X, ABS_MT_POSITION_Y, ABS_MT_TRACKING_ID events
                event[tmpCnt].type = EV_ABS;
                event[tmpCnt].code = ABS_X;
                event[tmpCnt].value = Finger[i][j].x;
                tmpCnt++;

                event[tmpCnt].type = EV_ABS;
                event[tmpCnt].code = ABS_Y;
                event[tmpCnt].value = Finger[i][j].y;
                tmpCnt++;

                event[tmpCnt].type = EV_ABS;
                event[tmpCnt].code = ABS_MT_POSITION_X;
                event[tmpCnt].value = Finger[i][j].x;
                tmpCnt++;

                event[tmpCnt].type = EV_ABS;
                event[tmpCnt].code = ABS_MT_POSITION_Y;
                event[tmpCnt].value = Finger[i][j].y;
                tmpCnt++;

                event[tmpCnt].type = EV_ABS;
                event[tmpCnt].code = ABS_MT_TRACKING_ID;
                event[tmpCnt].value = Finger[i][j].id;
                tmpCnt++;

                event[tmpCnt].type = EV_SYN;
                event[tmpCnt].code = SYN_MT_REPORT;
                event[tmpCnt].value = 0;
                tmpCnt++;
            }
        }
    }

    // Check if any events were added, if not, add SYN_MT_REPORT event
    if (tmpCnt == 0) {
        event[tmpCnt].type = EV_SYN;
        event[tmpCnt].code = SYN_MT_REPORT;
        event[tmpCnt].value = 0;
        tmpCnt++;

        // If it's the first touch, add BTN_TOUCH and BTN_TOOL_FINGER events
        if (!isFirstDown) {
            isFirstDown = true;
            event[tmpCnt].type = EV_KEY;
            event[tmpCnt].code = BTN_TOUCH;
            event[tmpCnt].value = 0;
            tmpCnt++;

            event[tmpCnt].type = EV_KEY;
            event[tmpCnt].code = BTN_TOOL_FINGER;
            event[tmpCnt].value = 0;
            tmpCnt++;
        }
    } else {
        // If not the first touch, write downEvent to nowfd
        if (isFirstDown) {
            isFirstDown = false;
            write(nowfd, downEvent, sizeof(downEvent));
        }
    }

    // Add SYN_REPORT event to event array
    event[tmpCnt].type = EV_SYN;
    event[tmpCnt].code = SYN_REPORT;
    event[tmpCnt].value = 0;
    tmpCnt++;

    // Write event array to nowfd
    write(nowfd, event, sizeof(struct input_event) * tmpCnt);
}

void *TypeA(void *arg) {
  targ tmp = *(targ *)arg;
  int i = tmp.fdNum;
  float S2TX = tmp.S2TX;
  float S2TY = tmp.S2TY;
  struct input_event ie;
  int latest = 0;
  for (;;) {
    ImGuiIO &io = ImGui::GetIO();
    if (read(origfd[i], &ie, sizeof(struct input_event))) {
      if (ie.type == EV_ABS) {
        if (ie.code == ABS_MT_SLOT) {
          latest = ie.value;
          continue;
        }
        if (ie.code == ABS_MT_TRACKING_ID) {
          if (ie.value == -1) {
            Finger[i][latest].isUp = true;
            Finger[i][latest].isDown = false;
            if (g_Initialized) {
              io.MouseDown[0] = false;
            }
          } else {
            Finger[i][latest].id = (i * 2 + 1) * maxF + latest;
            Finger[i][latest].isDown = true;
            if (g_Initialized) {
              io.MouseDown[0] = true;
            }
          }
          continue;
        }
        if (ie.code == ABS_MT_POSITION_X) {
          Finger[i][latest].id = (i * 2 + 1) * maxF + latest;
          Finger[i][latest].x = (int)(ie.value * S2TX);
          Finger[i][latest].isTmpDown = true;
          continue;
        }
        if (ie.code == ABS_MT_POSITION_Y) {
          Finger[i][latest].id = (i * 2 + 1) * maxF + latest;
          Finger[i][latest].y = (int)(ie.value * S2TY);
          Finger[i][latest].isTmpDown = true;
          continue;
        }
        if (ie.code == ABS_MT_TOUCH_MAJOR) {
          Finger[i][latest].id = (i * 2 + 1) * maxF + latest;
          Finger[i][latest].size1 = ie.value;
          Finger[i][latest].isTmpDown = true;
          continue;
        }
        if (ie.code == ABS_MT_WIDTH_MAJOR) {
          Finger[i][latest].id = (i * 2 + 1) * maxF + latest;
          Finger[i][latest].size2 = ie.value;
          Finger[i][latest].isTmpDown = true;
          continue;
        }
        if (ie.code == ABS_MT_TOUCH_MINOR) {
          Finger[i][latest].id = (i * 2 + 1) * maxF + latest;
          Finger[i][latest].size3 = ie.value;
          Finger[i][latest].isTmpDown = true;
          continue;
        }
      }
      if (g_Initialized) {
        struct input_event oneevent;
        ImVec2 point = rotatePointx(Touch_orientation, {(float)Finger[i][latest].x, (float)Finger[i][latest].y}, touch_screen_size);
        io.MousePos = ImVec2(point.x / touch_s, point.y / touch_s);
      }
      if (ie.type == EV_SYN && ie.code == SYN_REPORT) {
        if (Finger[i][latest].isTmpDown) {
          Upload();
        }
        continue;
      }
    }
  }
  return 0;
}
const static char *touchDebug = "/storage/emulated/0/touch_debug.log";

void printTouchScreenInfo() {
    // 打印触摸屏信息
    printf("\033[1;31m已调用独家触摸库\033[0m\n");
    printf("Scale factor (touch_s): %f", touch_s);
}

void initializeDownEvent() {
    // 初始化 downEvent 数组
    downEvent[0].type = EV_KEY;
    downEvent[0].code = BTN_TOUCH;
    downEvent[0].value = 1;
    downEvent[1].type = EV_KEY;
    downEvent[1].code = BTN_TOOL_FINGER;
    downEvent[1].value = 1;
}

void Touch_Init(const ImVec2 &s) {
    screenSize = s;
    printTouchScreenInfo();
    initializeDownEvent();



    downEvent[0].type = EV_KEY;
    downEvent[0].code = BTN_TOUCH;
    downEvent[0].value = 1;
    downEvent[1].type = EV_KEY;
    downEvent[1].code = BTN_TOOL_FINGER;
	downEvent[1].value = 1;
    
    char temp[128];
    DIR *dir = opendir("/dev/input/");
    dirent *ptr = NULL;
    int eventCount = 0;
    while ((ptr = readdir(dir)) != NULL)
    {
        if (strstr(ptr->d_name, "event"))
            eventCount++;
    }
    struct input_absinfo abs, absX[maxE], absY[maxE];
    int fd, i, tmp1, tmp2;
    int screenX, screenY, minCnt = eventCount + 1;
    fdNum = 0;
    for (i = 0; i <= eventCount; i++)
    {
        sprintf(temp, "/dev/input/event%d", i);
        fd = open(temp, O_RDWR);
        if (fd)
        {
            uint8_t *bits = NULL;
            ssize_t bits_size = 0;
            int res, j, k;
            bool itmp1 = false, itmp2 = false, itmp3 = false;
            while (1)
            {
                res = ioctl(fd, EVIOCGBIT(EV_ABS, bits_size), bits);
                if (res < bits_size)
                    break;
                bits_size = res + 16;
                bits = (uint8_t *) realloc(bits, bits_size * 2);
                if (bits == NULL)
                {
                    FILE *fpD = fopen(touchDebug, "w+");
                    fprintf(fpD, "获取事件失败\n");
                    fclose(fpD);
                    while (1)
                    {
                        exit(0);
                    }
                }
            }
            for (j = 0; j < res; j++)
            {
                for (k = 0; k < 8; k++)
                    if (bits[j] & 1 << k && ioctl(fd, EVIOCGABS(j * 8 + k), &abs) == 0)
                    {
                        if (j * 8 + k == ABS_MT_SLOT)
                        {
                            itmp1 = true;
                            continue;
                        }
                        if (j * 8 + k == ABS_MT_POSITION_X)
                        {
                            itmp2 = true;
                            continue;
                        }
                        if (j * 8 + k == ABS_MT_POSITION_Y)
                        {
                            itmp3 = true;
                            continue;
                        }
                    }
            }
            if (itmp1 && itmp2 && itmp3)
            {
                tmp1 = ioctl(fd, EVIOCGABS(ABS_MT_POSITION_X), &absX[fdNum]);
                tmp2 = ioctl(fd, EVIOCGABS(ABS_MT_POSITION_Y), &absY[fdNum]);
                if (tmp1 == 0 && tmp2 == 0)
                {
                    origfd[fdNum] = fd;
                    ioctl(fd, EVIOCGRAB, GRAB);
                    if (i < minCnt)
                    {
                        screenX = absX[fdNum].maximum;
                        screenY = absY[fdNum].maximum;
                        minCnt = i;
                    }
                    fdNum++;
                    if (fdNum >= maxE)
                        break;
                }
            }
            else
            {
                close(fd);
            }
        }
    }
    
    if (minCnt > eventCount)
    {
        FILE *fpD = fopen(touchDebug, "w+");
        fprintf(fpD, "获取屏幕驱动失败\n");
        fclose(fpD);
        while (1)
        {
            exit(0);
        } 
    }

    struct uinput_user_dev ui_dev;
    nowfd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (nowfd <= 0)
    {
        FILE *fpD = fopen(touchDebug, "w+");
        fprintf(fpD, "打开驱动失败\n");
        fclose(fpD);
        while (1)
        {
            exit(0);
        }
    } 

    memset(&ui_dev, 0, sizeof(ui_dev));
    strncpy(ui_dev.name, genRandomString(rand() % 10 + 5), UINPUT_MAX_NAME_SIZE);
    ui_dev.id.bustype = 0;
    ui_dev.id.vendor = rand() % 10 + 5;
    ui_dev.id.product = rand() % 10 + 5;
    ui_dev.id.version = rand() % 10 + 5;

    ioctl(nowfd, UI_SET_PROPBIT, INPUT_PROP_DIRECT);

    ioctl(nowfd, UI_SET_EVBIT, EV_ABS);
    ioctl(nowfd, UI_SET_ABSBIT, ABS_X);
    ioctl(nowfd, UI_SET_ABSBIT, ABS_Y);
    ioctl(nowfd, UI_SET_ABSBIT, ABS_MT_POSITION_X);
    ioctl(nowfd, UI_SET_ABSBIT, ABS_MT_POSITION_Y);
    ioctl(nowfd, UI_SET_ABSBIT, ABS_MT_TRACKING_ID);
    ioctl(nowfd, UI_SET_EVBIT, EV_SYN);
    ioctl(nowfd, UI_SET_EVBIT, EV_KEY);
    ioctl(nowfd, UI_SET_PHYS, genRandomString(rand() % 10 + 5));

    sprintf(temp, "/dev/input/event%d", minCnt);
    fd = open(temp, O_RDWR);
    if (fd)
    {
        struct input_id id;
        if (!ioctl(fd, EVIOCGID, &id))
        {
            ui_dev.id.bustype = id.bustype;
            ui_dev.id.vendor = id.vendor;
            ui_dev.id.product = id.product;
            ui_dev.id.version = id.version;
        }
        uint8_t *bits = NULL;
        ssize_t bits_size = 0;
        int res, j, k;
        while (1)
        {
            res = ioctl(fd, EVIOCGBIT(EV_KEY, bits_size), bits);
            if (res < bits_size)
                break;
            bits_size = res + 16;
            bits = (uint8_t *) realloc(bits, bits_size * 2);
            if (bits == NULL)
            {
                FILE *fpD = fopen(touchDebug, "w+");
                fprintf(fpD, "获取事件失败\n");
                fclose(fpD);
                while (1)
                {
                    exit(0);
                }
            }
        }
        for (j = 0; j < res; j++)
        {
            for (k = 0; k < 8; k++)
                if (bits[j] & 1 << k)
                {
                    if (j * 8 + k == BTN_TOUCH || j * 8 + k == BTN_TOOL_FINGER)
                        continue;
                    ioctl(nowfd, UI_SET_KEYBIT, j * 8 + k);
                }
        } 
    }

    ui_dev.absmin[ABS_MT_POSITION_X] = 0;
    ui_dev.absmax[ABS_MT_POSITION_X] = screenX;
    ui_dev.absmin[ABS_MT_POSITION_Y] = 0;
    ui_dev.absmax[ABS_MT_POSITION_Y] = screenY;

    ui_dev.absmin[ABS_X] = 0;
    ui_dev.absmax[ABS_X] = screenX;
    ui_dev.absmin[ABS_Y] = 0;
    ui_dev.absmax[ABS_Y] = screenY;
    //*retX = screenX;
    //*retY = screenY;

    write(nowfd, &ui_dev, sizeof(ui_dev));

    if (ioctl(nowfd, UI_DEV_CREATE))
    {
        FILE *fpD = fopen(touchDebug, "w+");
        fprintf(fpD, "创建驱动失败\n");
        fclose(fpD);
        while (1)
        {
            exit(0);
        }
    }

    targ tmp[fdNum];
    for (i = 0; i < fdNum; i++)
    {
        tmp[i].fdNum = i;
        tmp[i].S2TX = (float)screenX / (float)absX[i].maximum;
        tmp[i].S2TY = (float)screenY / (float)absY[i].maximum;
        pthread_create(&touchloop, NULL, TypeA, &tmp[i]);
    }
    fdNum++;
    system("chmod 000 -R /proc/bus/input/*");
    system("chmod 311 -R /sys/devices/virtual/input/"); 
    
    touch_screen_size = getTouchScreenDimension(fd);
    touch_s = (touch_screen_size.x + touch_screen_size.y) / (screenSize.x + screenSize.y);
}

void Touch_Down(int id, int x, int y) {
    const int num = fdNum - 1;
    if (num >= 0 && num < maxE && id >= 0 && id < maxF)
    {
        // 边界检查确保数组访问安全
        auto& currentFinger = Finger[num][id];
        
        // 设置手指的唯一标识
        currentFinger.id = (num * 2 + 1) * maxF + id;
        
        // 检查缩放系数是否有效，避免除以零错误
        if (touch_s != 0) {
            // 将触摸坐标乘以缩放系数
            currentFinger.x = x * touch_s;
            currentFinger.y = y * touch_s;
        } else {
            // 缩放系数无效，记录日志或进行错误处理
            // 这里只是简单输出错误信息到控制台
     //       std::cerr << "Error: Invalid touch_s value. Division by zero avoided." << std::endl;
        }
        
        // 设置手指大小参数
        currentFinger.size1 = currentFinger.size2 = currentFinger.size3 = 8;
        
        // 标记手指按下状态，并上传触摸事件
        currentFinger.isDown = true;
        Upload();
    }
    else
    {
        // 手指索引或 ID 超出有效范围，记录日志或进行错误处理
        // 这里只是简单输出错误信息到控制台
     ///   std::cerr << "Error: Invalid finger index or ID." << std::endl;
    }
}


void Touch_Move(int id, int x, int y) {
    int num = fdNum - 1;
    if (num >= 0 && num < maxE && id >= 0 && id < maxF) {
        // 设置手指的唯一标识和触摸坐标，进行有效性检查
        Finger[num][id].id = (num * 2 + 1) * maxF + id;
        Finger[num][id].x = x * touch_s;
        Finger[num][id].y = y * touch_s;
        Finger[num][id].size1 = 8;
        Finger[num][id].size2 = 8;
        Finger[num][id].size3 = 8;
        Finger[num][id].isDown = true;
        // 打印缩放因子   
        Upload();
    }
    else
    {
        // 手指索引或 ID 超出有效范围，记录日志或进行错误处理
        // 这里只是简单输出错误信息到控制台
   //     std::cerr << "Error: Invalid finger index or ID." << std::endl;
    }
}


void Touch_Up(int id) {
    usleep(3 * 1000); // 延时 3 毫秒
    int num = fdNum - 1;
    if (num >= 0 && num < maxE && id >= 0 && id < maxF) {
        // 标记手指抬起状态，并上传触摸事件
        Finger[num][id].isDown = false;
        Finger[num][id].isUp = true;
        Upload();
    } else {
        // 手指索引或 ID 超出有效范围，记录日志或进行错误处理
        // 这里只是简单输出错误信息到控制台
        // std::cerr << "Error: Invalid finger index or ID." << std::endl;
    }
}


void Touch_setOrientation(int orientation_) {
    ::Touch_orientation = orientation_;
}
