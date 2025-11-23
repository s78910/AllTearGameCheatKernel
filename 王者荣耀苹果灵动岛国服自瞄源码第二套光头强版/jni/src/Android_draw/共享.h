/* 
 * 由光头强内核（原XF内核）开源
 因频道账号被盗用后决定开源，正版为光头强内核，
开源文件仅供学习交流，任何个人或组织可在遵循协议条款的前提下，自由下载、使用、修改、分发本项目代码及衍生作品，无需额外授权。使用过程中请保留原项目版权声明及开源协议文本，不得用于违法违规场景。本项目以“现状”提供，开发者不承担任何直接或间接的使用风险及法律责任，亦不提供商业技术支持。
删除此内容死全家，没几把
* 注入时间: 2025-11-16 20:14:49
 * 联系方式: TG:@GTQNHNB666
QQ群：784316342
 */
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>

#define MAX_BUFFER_SIZE 1024

     
    //王者共享内容
//共享菜单全部   
int pxx = 0, pyy = 0; double WwrShare = 0.0;//临时分辨率
int jiangea = -9;//小地图间隔
bool 初始化 = false;
bool Touchinit = true;
int Screen_X, Screen_Y;
int xytable[10][10];
int hist1[10][10];
int me1 = 0;
//float ax, ay;
int prex1, prey1;
long a, objarrayaddr, libGameCore1, libil2cpp1, 兵线基址, 人物数组, 野怪数组, 兵线数组;
int 阵营1 = 0, fx1 = 0;
struct sESPMenu {
    bool 绘制调试 = false;
    bool 全部关闭;
    bool 是否开启共享;
    bool 全部开启;
    bool 显示头像;
    bool 保存调试配置到云端;
    bool 技能计时;
    bool 野怪计时;
    bool 显示兵线;
    bool 回城判断;
    bool 敌人视野;
    int 分辨率Y = 2456;//2712  
    int 分辨率X = 2257;//1220  2846

    int 小地图左右调整 = 0;//我的配置87   -3
    int 小地图上下调整 = 0;//   -4
    int 兵线左右调整 =  0;//我的配置6
    int 兵线上下调整 = 0;//31
    int 野怪左右调整 = 0;//我的配置-6
    int 野怪上下调整 =  0;//-1
    int FPS帧率 = 120;

};
sESPMenu ESPMenu;

   
 
 
     //共享传输变量
    int herealx, herealy;int zyz, hcc;   
    //传输变量  野怪传输
    int ygtime;
    int  buffid;
    int  buffx;
    int  buffy;
    long BUFFtemp;  //临时数组
    int 游戏时间;
    int xxx1, yyy1;
    int bui;
    long bbuff1;
        int bingx;
    int bingy;//兵线共享变量
    
    //共享绘制房间号
void read_line_without_newline(FILE* stream, char* buffer, size_t size) {
    fgets(buffer, size, stream);
    buffer[strcspn(buffer, "\n")] = '\0';
}

char imei[100] = "未开启";//leixin666
int ret = -1;
int socket_fd;
struct sockaddr_in serveraddr;
char buffer[MAX_BUFFER_SIZE];
char homeBuffer[MAX_BUFFER_SIZE];//117.72.39.162
int numBytesReceived, homeBytesReceived;//38.55.232.31
std::string 共享房间ip = "154.9";//主机  
bool 是否开启共享 = true;
int 共享开关=0;
void createSocket() {//重要方法
    共享开关=1;
	srand((unsigned int)time(NULL));  // 初始化随机数种子
    int num = rand() % 1000 + 1;  // 生成 1 到 10000 之间的随机数
    sprintf(imei, "%06d", num);  // 将数字转换为字符串并存储在 imei 中
    printf("\033[32;1m");	// 绿色
    //printf("[+] 共享网站: http://www.独久.fun/\n");
    //printf("[+] 默认初始化共享房间为:%s\n",imei);
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("[+] 连接失败\n");
        exit(0);
    }
    if (socket_fd < 0) {
    printf("[+] 连接失败\n");
        exit(0);
    }
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr((char*)共享房间ip.data());
    serveraddr.sin_port = htons(9999);
    ret = connect(socket_fd, (struct sockaddr*)&serveraddr, sizeof(struct sockaddr));
    if (ret < 0) {
    	
    	    for (int i = 0; i < 10; i++) {
        printf("[+] 连接失败\n");
    }
    	
    }
      
}
