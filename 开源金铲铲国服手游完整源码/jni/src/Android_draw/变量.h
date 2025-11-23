#ifndef MEMORY_UTILS_H
#define MEMORY_UTILS_H
//QQ交流群1027850475
//by风华绝代
//倒卖死妈
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>

// 全局变量声明
extern int c;
extern char extractedString[64];
int 状态 = 0;
int 数据获取状态 = 0;
int 遍历次数=0;
char extractedString[64];
long int MatrixOffset = 0,ArrayaddrOffset = 0;


char libso[256] = {"libclient.so"};
uintptr_t libbase;
uintptr_t Arrayaddr, Count, Matrix;
uintptr_t 对象,对象阵营,自身,自身阵营,namezfcz,namezfc;
uintptr_t 红夫人,红夫人镜像,镜子,捏镜子;
int 数量,zfcz,zfc;
float 过滤矩阵[17];
//float matrix[16];
//float angle;






// 结构体定义
typedef struct {
    unsigned long addr;
    unsigned long taddr;
} ModuleBssInfo;

// 函数声明
ModuleBssInfo get_module_bss(int pid, const char *module_name);
ModuleBssInfo get_module_bssgjf(int pid, const char *module_name);
int get_name_pid1(const char *packageName);
long getModuleBasegjf(int pid, const char *module_name);

// 函数实现
inline ModuleBssInfo get_module_bss(int pid, const char *module_name) {
    FILE *fp;
    ModuleBssInfo info = {0, 0};
    char filename[64];
    char line[1024];

    snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
    fp = fopen(filename, "r");
    bool found_module = false;

    if (fp != NULL) {
        while (fgets(line, sizeof(line), fp)) {
            if (strstr(line, module_name) != NULL) {
                found_module = true;
            }

            if (found_module) {
                long addr,taddr;
                sscanf(line, "%lx-%lx", &addr, &taddr);
                if (strstr(line, "rw") != NULL && strlen(line) < 86 && (taddr-addr)/4096 >= 2800) {
                    char *words[10];
                    int numWords = 0;
                    char *token = strtok(line, " ");
                    while (token != NULL && numWords < 10) {
                        words[numWords++] = token;
                        token = strtok(NULL, " ");
                    }

                    for (int i = 0; i < numWords; i++) {
                        if (sscanf(words[i], "%lx-%lx", &info.addr, &info.taddr) == 2) {
                            fclose(fp);
                            return info;
                        }
                    }

                    info.addr = 0;
                    info.taddr = 0;
                    fclose(fp);
                    return info;
                }
            }
        }
        fclose(fp);
    }
    return info;
}

inline ModuleBssInfo get_module_bssgjf(int pid, const char *module_name) {
    FILE *fp;
    ModuleBssInfo info = {0, 0};
    long addr,taddr;
    char filename[64];
    char line[1024];
    snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
    fp = fopen(filename, "r");
    bool is = false;
    if (fp != NULL) {
        while (fgets(line, sizeof(line), fp)) {
            sscanf(line, "%lx-%lx", &addr, &taddr);
            if (strstr(line, module_name) && strstr(line, "r-xp") != NULL && (taddr-addr) == 114982912) {
                is = true;
            }
            if (is) {
                if (strstr(line, "rw") != NULL && !feof(fp) && (strlen(line) < 86)) {
                    sscanf(line, "%lx-%lx", &addr, &taddr);
                    if ((taddr-addr)/4096 <= 3000)
                        continue;
                    if (sscanf(line, "%lx-%lx", &info.addr, &info.taddr) != 2) {
                        info.addr = 0;
                        info.taddr = 0;
                        break;
                    }
                    break;
                }
            }
        }
        fclose(fp);
    }
    return info;
}

inline int get_name_pid1(const char *packageName) {
    int id = -1;
    DIR *dir;
    FILE *fp;
    char filename[64];
    char cmdline[64];
    struct dirent *entry;
    dir = opendir("/proc");
    while ((entry = readdir(dir)) != NULL) {
        id = atoi(entry->d_name);
        if (id != 0) {
            sprintf(filename, "/proc/%d/cmdline", id);
            fp = fopen(filename, "r");
            if (fp) {
                fgets(cmdline, sizeof(cmdline), fp);
                fclose(fp);
                if ((strstr(cmdline, packageName) != NULL || strstr(cmdline, "com.netease.idv") != NULL) &&
                    strstr(cmdline, "com") != NULL &&
                    strstr(cmdline, "PushService") == NULL &&
                    strstr(cmdline, "gcsdk") == NULL) {
                    sprintf(extractedString, "%s", cmdline);
                    closedir(dir);
                    return id;
                }
            }
        }
    }
    closedir(dir);
    return -1;
}

inline long getModuleBasegjf(int pid, const char *module_name) {
    FILE *fp;
    long addr,taddr;
    char filename[64];
    char line[1024];
    snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
    fp = fopen(filename, "r");
    bool is = false;
    if (fp != NULL) {
        while (fgets(line, sizeof(line), fp)) {
            if (strstr(line, "r-xp") != NULL && !feof(fp) && strstr(line, module_name)) {
                sscanf(line, "%lx-%lx", &addr, &taddr);
                if ((taddr-addr) == 114982912) {
                    fclose(fp);
                    return addr;
                }
            }
        }
        fclose(fp);
    }
    return 0;
}

#endif // MEMORY_UTILS_H
