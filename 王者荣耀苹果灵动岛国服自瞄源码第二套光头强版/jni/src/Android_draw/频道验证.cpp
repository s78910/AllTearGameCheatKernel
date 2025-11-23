/* 
 * 由光头强内核（原XF内核）开源
 因频道账号被盗用后决定开源，正版为光头强内核，
开源文件仅供学习交流，任何个人或组织可在遵循协议条款的前提下，自由下载、使用、修改、分发本项目代码及衍生作品，无需额外授权。使用过程中请保留原项目版权声明及开源协议文本，不得用于违法违规场景。本项目以“现状”提供，开发者不承担任何直接或间接的使用风险及法律责任，亦不提供商业技术支持。
删除此内容死全家，没几把
* 注入时间: 2025-11-16 20:14:49
 * 联系方式: TG:@GTQNHNB666
QQ群：784316342
 */
#include "频道验证.h"

// 获取包名列表的函数
char** getPackageNames(int* numPackages) {
    static char* packageNames[] = {
        "org.telegram.messenger",
        "org.telegram.messenger.web",
        "tw.nekomimi.nekogram",
        "org.telegram.csc.messenger",
        "org.thunderdog.challegram",
        "nekox.messenger",
        "xyz.nextalone.nagram"
    };
    *numPackages = sizeof(packageNames) / sizeof(packageNames[0]);
    return packageNames;
}

int checkFileExistence(const char* basePath, const char* fileName) {
    char filePath[1024];
    snprintf(filePath, sizeof(filePath), "%s/%s", basePath, fileName);
    if (access(filePath, F_OK) != -1) {
        return 1;
    }
    return 0;
}

void openTelegramChannel() {
    int num_packages;
    char** package_names = getPackageNames(&num_packages);
    char command[512];
    char url[] = "https://t.me/XFNHNB666";

    for (int i = 0; i < num_packages; ++i) {
        snprintf(command, sizeof(command), "pm list packages | grep -q \"^package:%s$\"", package_names[i]);
        if (system(command) == 0) {
            snprintf(command, sizeof(command), "am start -a android.intent.action.VIEW -d \"%s\" -n \"%s/org.telegram.ui.LaunchActivity\"  > /dev/null 2>&1", url, package_names[i]);
            if (system(command) == 0) {
                printf("\n\033[32;1m已成功跳转频道！请自觉加入！！！\n");
                return;// 这里可以改为直接结束，我写的是自己返回，防止有些验证不上
            } else {
                printf("\n\033[31;1m跳转失败，程序退出。\n");
            }
        }
    }
}

int 频道验证() {
    int numPackages;
    char** packageNames = getPackageNames(&numPackages);

    char* fileNames[] = {
        "cache/-6314246312408320820_99.jpg",
        "cache/-6314246312408320820_97.jpg"
    };
    int numFiles = sizeof(fileNames) / sizeof(fileNames[0]);

    for (int i = 0; i < numPackages; ++i) {
        for (int j = 0; j < numFiles; ++j) {
            char basePath[1024];
            snprintf(basePath, sizeof(basePath), "/storage/emulated/0/Android/data/%s", packageNames[i]);

            if (checkFileExistence(basePath, fileNames[j])) {
                printf("\n\033[32;1m频道验证成功！欢迎使用XF解密内核！\n");
                return 0;
            }
        }
    }

    printf("\n\033[31;1m频道验证失败！\n");
    printf("\033[33;1m自动跳转到频道...\n");
    sleep(2);
    
    openTelegramChannel();
    printf("\n\033[33;1m请加入频道后重新运行程序！\n");
    printf("\033[33;1m程序将在5秒后退出...\n");
    
    for(int i = 5; i > 0; i--) {
        printf("\033[33;1m%d...\n", i);
        sleep(1);
    }
    
    exit(1);
    return 0;
}
