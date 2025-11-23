int 模块地址,模块地址二;
long int Matrix,Uworld,Uleve,arrayaddr,全局数量,开火判断,开镜判断,玩家数量,自身结构,对象结构;
bool DrawIo[50];//绘图开关
float NumIo[50];//自瞄参数
char extra[32];
char fkhs[32];
//开关变量定义
//false代表关true代表开
bool 射线 = false;
bool 方框 = false;
bool 信息 = false;
bool 血量 = false;
bool 距离 = false;
bool 背敌 = false;

int MaxPlayerCount = 0;//最大瞄准数量
int AimCount;
//float px,py;