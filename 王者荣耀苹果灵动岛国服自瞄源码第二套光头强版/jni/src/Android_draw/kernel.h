/* 
 * 由光头强内核（原XF内核）开源
 因频道账号被盗用后决定开源，正版为光头强内核，
开源文件仅供学习交流，任何个人或组织可在遵循协议条款的前提下，自由下载、使用、修改、分发本项目代码及衍生作品，无需额外授权。使用过程中请保留原项目版权声明及开源协议文本，不得用于违法违规场景。本项目以“现状”提供，开发者不承担任何直接或间接的使用风险及法律责任，亦不提供商业技术支持。
删除此内容死全家，没几把
* 注入时间: 2025-11-16 20:14:49
 * 联系方式: TG:@GTQNHNB666
QQ群：784316342
 */
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <pthread.h>
#include "频道验证.h"
// 函数声明
unsigned long get_module_base(int pid, const char *module_name);
long get_module_bss(int pid, const char *szModule);
#include <dirent.h>
#include <malloc.h>
#include <thread>
#include <sys/mman.h>
#include <sys/uio.h>
#include <math.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <locale>
#include <string>
#include <dlfcn.h>
#include <regex.h>

// 引入KMA驱动头文件 (直接使用421版本)
#include "../kma/driver.h"


bool checkFolderExists(const std::string& folderPath)
{
    DIR* dir = opendir(folderPath.c_str());
    if (dir)
    {
        closedir(dir);
        return true;
    }
    else if (ENOENT == errno)
    {
        return false;
    }
    else
    {
        return false;
    }
}

bool checkFoldersExistInDirectory(const std::string& directoryPath, const std::vector<std::string>& folderNames)
{
    for (const std::string& folderName : folderNames)
    {
        std::string folderPath = directoryPath + folderName;
        if (checkFolderExists(folderPath))
        {
            return true;
        }
    }
    return false;
}

// 频道验证相关函数已在频道验证.h中定义，此处删除重复定义

using namespace std;
float px,py;
class c_driver
{
private:
    int has_upper = 0;
    int has_lower = 0;
    int has_symbol = 0;
    int has_digit = 0;
    int fd = -1;
    pid_t pid;
    int driver_version = 0; // 0=未加载, 1=方法1, 2=方法2, 3=方法3, 4=KMA驱动, 5=RT Proc驱动, 6=RTHook驱动
    Driver* kma_driver = nullptr; // KMA驱动实例

    // 驱动操作结构体
    typedef struct _COPY_MEMORY {
        pid_t pid;
        uintptr_t addr;
        void *buffer;
        size_t size;
    } COPY_MEMORY, *PCOPY_MEMORY;

    typedef struct _MODULE_BASE {
        pid_t pid;
        char *name;
        uintptr_t base;
    } MODULE_BASE, *PMODULE_BASE;

    enum OPERATIONS {
        OP_INIT_KEY = 0x800,
        OP_READ_MEM = 0x801,
        OP_WRITE_MEM = 0x802,
        OP_MODULE_BASE = 0x803,
    };

    // RTHook驱动专用操作码
    enum RTHOOK_OPERATIONS {
        RT_READ_MEM = 601,
        RT_WRITE_MEM = 602,
        RT_MODULE_BASE = 603,
        RT_HIDE_PROCESS = 605,
    };

    // ====================== 方法1: Shell命令定位 ======================
    char* execCom(const char* shell) {
        FILE* fp = popen(shell, "r");
        if (!fp) return nullptr;

        char buffer[256];
        char* result = (char*)malloc(4096);
        result[0] = '\0';

        while (fgets(buffer, sizeof(buffer), fp)) {
            strcat(result, buffer);
        }
        pclose(fp);
        return result;
    }

    int findFirstMatchingPath(const char* path, regex_t* regex, char* result) {
        DIR* dir = opendir(path);
        if (!dir) return 0;

        struct dirent* entry;
        while ((entry = readdir(dir))) {
            char fullpath[1024];
            snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);
            
            if (entry->d_type == DT_LNK) {
                char linkpath[1024];
                ssize_t len = readlink(fullpath, linkpath, sizeof(linkpath)-1);
                if (len <= 0) continue;
                
                linkpath[len] = '\0';
                if (regexec(regex, linkpath, 0, NULL, 0) == 0) {
                    strcpy(result, fullpath);
                    closedir(dir);
                    return 1;
                }
            }
        }
        closedir(dir);
        return 0;
    }

    void createDriverNode(char* path, int major, int minor) {
        string cmd = "mknod " + string(path) + " c " + to_string(major) + " " + to_string(minor);
        system(cmd.c_str());
    }

    void removeDeviceNode(char* path) {
        unlink(path);
    }

    int load_driver_method1() {
        char* output = execCom("ls -l /proc/*/exe 2>/dev/null | grep -E \"/data/[a-z]{6} \\(deleted\\)\"");
        if (!output) return 0;

        // 解析PID和路径
        char* procStart = strstr(output, "/proc/");
        if (!procStart) {
            free(output);
            return 0;
        }

        char pid_buf[32] = {0};
        char filePath[256] = {0};
        char* pidStart = procStart + 6;
        char* pidEnd = strchr(pidStart, '/');
        strncpy(pid_buf, pidStart, pidEnd - pidStart);

        char* arrowStart = strstr(output, "->");
        if (!arrowStart) {
            free(output);
            return 0;
        }

        char* start = arrowStart + 3;
        char* end = strchr(start, '(');
        if (!end) {
            free(output);
            return 0;
        }
        strncpy(filePath, start, end - start - 1);

        // 转换data->dev
        char* dataPtr = strstr(filePath, "data");
        if (dataPtr) {
            memmove(dataPtr, "dev", 3);
            memmove(dataPtr+3, dataPtr+4, strlen(dataPtr+3));
        }

        // 获取设备号
        regex_t regex;
        char pattern[256];
        snprintf(pattern, sizeof(pattern), ".*%s.*", filePath + 5);
        if (regcomp(&regex, pattern, 0) != 0) {
            free(output);
            return 0;
        }

        char fdPath[256];
        snprintf(fdPath, sizeof(fdPath), "/proc/%s/fd", pid_buf);
        char result[1024];
        int major = 0;

        if (findFirstMatchingPath(fdPath, &regex, result)) {
            char cmd[256];
            sprintf(cmd, "ls -AL -l %s | grep -Eo '[0-9]{3},' | head -1 | grep -Eo '[0-9]{1,3}'", result);
            char* fdInfo = execCom(cmd);
            if (fdInfo) {
                major = atoi(fdInfo);
                free(fdInfo);
            }
        }

        regfree(&regex);
        free(output);

        // 创建设备节点
        if (major > 0) {
            createDriverNode(filePath, major, 0);
            fd = open(filePath, O_RDWR);
            removeDeviceNode(filePath);
            
            if (fd != -1) {
                driver_version = 1;
                printf("驱动载入成功\n");
                        //    频道验证();
                sleep(1);
                return 1;
            }
        }
        return 0;
    }

    // ====================== 方法2: /dev扫描 ======================
    int symbol_file(const char* filename) {
        int has_upper = 0, has_lower = 0, has_symbol = 0, has_digit = 0;
        for (int i = 0; filename[i]; i++) {
            if (islower(filename[i])) has_lower = 1;
            else if (isupper(filename[i])) has_upper = 1;
            else if (ispunct(filename[i])) has_symbol = 1;
            else if (isdigit(filename[i])) has_digit = 1;
        }
        return has_upper && !has_lower && !has_symbol && !has_digit;
    }

    char* find_driver_node() {
        const char* dev_path = "/dev";
        DIR* dir = opendir(dev_path);
        if (!dir) return nullptr;

        struct dirent* entry;
        while ((entry = readdir(dir))) {
            if (entry->d_name[0] == '.') continue;
            
            // 跳过全小写文件
            bool all_lower = true;
            for (int i = 0; entry->d_name[i]; i++) {
                if (isupper(entry->d_name[i])) {
                    all_lower = false;
                    break;
                }
            }
            if (all_lower) continue;

            // 构建完整路径
            char* file_path = (char*)malloc(PATH_MAX);
            snprintf(file_path, PATH_MAX, "%s/%s", dev_path, entry->d_name);

            // 获取文件信息
            struct stat file_info;
            if (stat(file_path, &file_info) < 0) {
                free(file_path);
                continue;
            }

            // 跳过特殊文件
            if (strstr(entry->d_name, "gpiochip") || 
                !strcmp(entry->d_name, "stdin") ||
                !strcmp(entry->d_name, "stdout") ||
                !strcmp(entry->d_name, "stderr")) {
                free(file_path);
                continue;
            }

            // 检查文件属性
            time_t current_time = time(nullptr);
            struct tm* tm_info = localtime(&current_time);
            int current_year = tm_info ? tm_info->tm_year + 1900 : 0;
            
            tm_info = localtime(&file_info.st_ctime);
            int file_year = tm_info ? tm_info->tm_year + 1900 : 0;
            
            size_t name_len = strlen(entry->d_name);

            // 检查文件类型和属性
            if (
                S_ISCHR(file_info.st_mode) &&  // 确保是字符设备
                (file_info.st_size == 0) &&
                (file_info.st_gid == 0) &&
                (file_info.st_uid == 0) &&
                (name_len <= 7) &&
                (file_year > 1970) &&
                (file_info.st_atime == file_info.st_ctime) &&
                symbol_file(entry->d_name)
            ) {
                closedir(dir);
                return file_path;
            }
            free(file_path);
        }
        closedir(dir);
        return nullptr;
    }

    int load_driver_method2() {
        char* device_path = find_driver_node();
        if (!device_path) return 0;

        // 带图标+颜色高亮+格式对齐，驱动文件路径醒目显示
printf("\n📁 【驱动加载信息】\n");
printf("\033[33;1m当前加载驱动文件：\033[0m"); // 黄色加粗提示文本

        fd = open(device_path, O_RDWR);
        free(device_path);

        if (fd != -1) {
            driver_version = 2;
         //   printf("驱动载入成功 (RT驱动)\n");
                    //    频道验证();
            sleep(1);
            return 1;
        }
        return 0;
    }

    // ====================== 方法3: 预定义文件名列表 ======================
    
    
    
    char *driver_path() {
	// 打开目录
		const char *dev_path = "/dev";
		DIR *dir = opendir(dev_path);
		if (dir == NULL){
			// 错误提示醒目化：图标+红色警示+原因引导
printf("\n❌ 【操作失败】\n");
printf("----------------------------------------\n");
printf("\033[31;1m⚠️  无法打开 /dev 目录！\n");
printf("💡 可能原因：权限不足（需 root 权限）或目录不存在\033[0m\n");
printf("----------------------------------------\n");

			return NULL;
		}

		char *files[] = { "wanbai", "CheckMe", "Ckanri", "lanran","video188"};
		struct dirent *entry;
		char *file_path = NULL;
		while ((entry = readdir(dir)) != NULL) {
			// 跳过当前目录和上级目录
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
				continue;
			}

			size_t path_length = strlen(dev_path) + strlen(entry->d_name) + 2;
			file_path = (char *)malloc(path_length);
			snprintf(file_path, path_length, "%s/%s", dev_path, entry->d_name);
			for (int i = 0; i < 5; i++) {
				if (strcmp(entry->d_name, files[i]) == 0) {
					// 图标点缀+色彩分层，路径更醒目
printf("\n✨ 驱动文件信息\n");
printf("----------------------------------------\n");
printf("\033[33;1m📌 驱动文件路径：\033[0m"); // 黄色加粗提示
printf("----------------------------------------\n");

					closedir(dir);
					return file_path;
				}
			}

			// 获取文件stat结构
			struct stat file_info;
			if (stat(file_path, &file_info) < 0) {
				free(file_path);
				file_path = NULL;
				continue;
			}

			// 跳过gpio接口
			if (strstr(entry->d_name, "gpiochip") != NULL) {
				free(file_path);
				file_path = NULL;
				continue;
			}

			// 检查是否为驱动文件
			if ((S_ISCHR(file_info.st_mode) || S_ISBLK(file_info.st_mode))
				&& strchr(entry->d_name, '_') == NULL && strchr(entry->d_name, '-') == NULL && strchr(entry->d_name, ':') == NULL) {
				// 过滤标准输入输出
				if (strcmp(entry->d_name, "stdin") == 0 || strcmp(entry->d_name, "stdout") == 0
					|| strcmp(entry->d_name, "stderr") == 0) {
					free(file_path);
					file_path = NULL;
					continue;
				}
				
				size_t file_name_length = strlen(entry->d_name);
				time_t current_time;
				time(&current_time);
				int current_year = localtime(&current_time)->tm_year + 1900;
				int file_year = localtime(&file_info.st_ctime)->tm_year + 1900;
				//跳过1980年前的文件
				if (file_year <= 1980) {
					free(file_path);
					file_path = NULL;
					continue;
				}
				
				time_t atime = file_info.st_atime;
				time_t ctime = file_info.st_ctime;
				// 检查最近访问时间和修改时间是否一致并且文件名是否是symbol文件
				if ((atime == ctime)/* && symbol_file(entry->d_name)*/) {
					//检查mode权限类型是否为S_IFREG(普通文件)和大小还有gid和uid是否为0(root)并且文件名称长度在7位或7位以下
					if ((file_info.st_mode & S_IFMT) == 8192 && file_info.st_size == 0
						&& file_info.st_gid == 0 && file_info.st_uid == 0 && file_name_length <= 9) {
						//printf("驱动文件：%s\n", file_path);
              //          sleep(1.5);
                      //      
						closedir(dir);
						return file_path;
					}
				}
			}
			free(file_path);
			file_path = NULL;
		}
		closedir(dir);
		return NULL;
	}
	
    
    

    int load_driver_method3() {
        char* device_name = driver_path();
        if (!device_name) return 0;

        // 图标点缀+色彩分层，路径更醒目
printf("\n✨ 驱动文件信息\n");
printf("----------------------------------------\n");
printf("\033[33;1m📌 驱动文件路径：\033[0m"); // 黄色加粗提示
printf("----------------------------------------\n");

        fd = open(device_name, O_RDWR);
        free(device_name);

        if (fd != -1) {
            driver_version = 3;
            // 成功提示美化：图标+绿色高亮+视觉分割，成就感拉满
printf("\n========================================\n");
printf("\033[32;1m✅ 驱动载入成功！\033[0m\n");
printf("----------------------------------------\n");
printf("\033[36;1m📌 加载驱动：GT驱动\033[0m\n");
printf("========================================\n\n");

            sleep(1);
            return 1;
        }
        return 0;
    }

    // ====================== 方法4: KMA内核模块驱动 ======================
    int load_driver_method4() {
        kma_driver = new Driver();
        if (kma_driver) {
            // 设置CPU亲和性
            kma_driver->cpuset(0, 4);
            
            driver_version = 4;
            // 绿色高亮+图标点缀，成功状态清晰醒目
printf("\n========================================\n");
printf("\033[32;1m🎉 驱动载入成功！\033[0m\n");
printf("----------------------------------------\n");
printf("\033[34;1m🚀 已加载驱动：KMA驱动\033[0m\n");
printf("========================================\n\n");

            sleep(1);
            return 1;
        } else {
// 错误提示层级化：红色警示+图标引导，排查点清晰易读
printf("\n========================================\n");
printf("\033[31;1m❌ KMA驱动初始化失败！\033[0m\n");
printf("----------------------------------------\n");
printf("\033[33;1m💡 请按以下步骤排查：\033[0m\n");
printf("  1. \033[37;1m是否已成功加载KMA内核模块\033[0m\n");
printf("  2. \033[37;1m是否使用支持KPM的root方案（Apatch/sukisu）\033[0m\n");
printf("  3. \033[37;1m内核版本是否在支持范围（4.9-6.6）\033[0m\n");
printf("========================================\n\n");

            if (kma_driver) {
                delete kma_driver;
                kma_driver = nullptr;
            }
        }
        return 0;
    }

    // ====================== 方法5: RT Proc驱动 ======================
    char* find_proc_driver_path() {
        struct dirent *de;
        DIR *dr = opendir("/proc");
        char *device_path = NULL;

        if (dr == NULL) {
// 红色警示+原因引导，错误信息直观且有指导性
printf("\n========================================\n");
printf("\033[31;1m❌ 操作失败：无法打开 /proc 目录\033[0m\n");
printf("----------------------------------------\n");
printf("\033[33;1m💡 可能原因：\033[0m\n");
printf("  • 系统权限不足（需 root 权限运行）\n");
printf("  • /proc 虚拟文件系统未挂载\n");
printf("========================================\n\n");

            return NULL;
        }

        while ((de = readdir(dr)) != NULL) {
            // 检查文件名长度为6位，排除已知系统文件
            if (strlen(de->d_name) != 6 || 
                strcmp(de->d_name, "NVTSPI") == 0 || 
                strcmp(de->d_name, "ccci_log") == 0 || 
                strcmp(de->d_name, "aputag") == 0 || 
                strcmp(de->d_name, "asound") == 0 || 
                strcmp(de->d_name, "clkdbg") == 0 || 
                strcmp(de->d_name, "crypto") == 0 || 
                strcmp(de->d_name, "modules") == 0 || 
                strcmp(de->d_name, "mounts") == 0 || 
                strcmp(de->d_name, "pidmap") == 0 || 
                strcmp(de->d_name, "phoenix") == 0 || 
                strcmp(de->d_name, "uptime") == 0 || 
                strcmp(de->d_name, "vmstat") == 0) {
                continue;
            }
            
            // 检查是否为字母数字组合
            int is_valid = 1;
            for (int i = 0; i < 6; i++) {
                if (!isalnum(de->d_name[i])) {
                    is_valid = 0;
                    break;
                }
            }
            
            if (is_valid) {
                device_path = (char*)malloc(11 + strlen(de->d_name));
                sprintf(device_path, "/proc/%s", de->d_name);
                struct stat sb;
                if (stat(device_path, &sb) == 0 && S_ISREG(sb.st_mode)) {
                    break;
                } else {
                    free(device_path);
                    device_path = NULL;
                }
            }
        }
        
        closedir(dr);
        return device_path;
    }

    int load_driver_method5() {
        char* device_path = find_proc_driver_path();
        if (!device_path) return 0;

        // 图标点缀+色彩分层，路径更醒目
printf("\n✨ 驱动文件信息\n");
printf("----------------------------------------\n");
printf("\033[33;1m📌 驱动文件路径：\033[0m"); // 黄色加粗提示
printf("----------------------------------------\n");

        fd = open(device_path, O_RDWR);
        free(device_path);

        if (fd != -1) {
            driver_version = 5;
            // 清新配色+图标强化，成功状态亮眼不刺眼
printf("\n========================================\n");
printf("\033[32;1m✅ 驱动载入成功！\033[0m\n");
printf("----------------------------------------\n");
printf("\033[35;1m🔧 已加载驱动：RT Proc驱动\033[0m\n");
printf("========================================\n\n");

            sleep(1);
            return 1;
        }
        return 0;
    }

    // ====================== 方法6: RTHook驱动 ======================
    // RTHook驱动按照rthook.h逻辑，外部直接设置fd
    int load_driver_method6() {
        // RTHook驱动只设置版本号，fd已在构造函数中设置为0
        driver_version = 6;
        // printf("RTHook驱动加载完成，fd = %d（按照rthook.h逻辑）\n", fd);
        return 1;
    }




// 显示可用驱动列表（无颜色版）
void show_driver_menu() {
    printf("📌 【光头强开源 - 驱动选择菜单】\n");
    printf("===========================================\n");
    printf("  1. qx11.4驱动\n");
    printf("  2. RTdev驱动\n");
    printf("  3. GT驱动\n");
    printf("  4. KMA（426）内核模块驱动\n");
    printf("  5. Proc驱动 \n");
    printf("  6. RTHook驱动 \n");
    printf("-------------------------------------------\n");
    printf("📢 官方交流渠道：\n");
    printf("  频道：@GTQNHNB666\n");
    printf("  QQ群：784316342\n");
    printf("===========================================\n");
}


    // // 显示可用驱动列表
    // void show_driver_menu() {
        // printf("=== 驱动选择菜单 ===\n");
        // printf("\033[35;1m");		// 粉色
        // printf("1. qx11.4驱动\n");
        // printf("2. RTdev驱动\n");
        // printf("3. GT驱动\n");
        // printf("4. KMA（423）内核模块驱动\n");
        // printf("5. RT Proc驱动\n");
        // printf("6. RTHook驱动\n");
        // 
       
        // printf("QQ群：784316342\n");
        // printf("\033[35;1m");		// 粉色
    // }
    
    // 手动加载指定驱动
    bool load_specific_driver(int driver_type) {
        switch (driver_type) {
            case 1:
                return load_driver_method1();
            case 2:
                return load_driver_method2();
            case 3:
                return load_driver_method3();
            case 4:
                return load_driver_method4();
            case 5:
                return load_driver_method5();
            case 6:
                return load_driver_method6();
            default:
                return false;
        }
    }
    
    // 交互式选择驱动
    void interactive_driver_selection() {
        show_driver_menu();
        
        int choice;
// 菜单式引导+色彩区分，选择入口清晰直观
printf("\n========================================\n");
printf("\033[34;1m📋 驱动类型选择\033[0m\n");
printf("----------------------------------------\n");
printf("\033[37;1m请选择要加载的驱动类型（输入1-6）：\033[0m");
printf("\n========================================\n");

        if (scanf("%d", &choice) != 1) {
            // 橙色警示+图标提醒，错误反馈直观且不生硬
printf("\n\033[33;1m⚠️  输入无效！\033[0m\n");
printf("----------------------------------------\n");
printf("\033[37;1m请输入 1-6 之间的有效数字，重新选择驱动类型～\033[0m\n");
printf("----------------------------------------\n\n");

            interactive_driver_selection(); // 递归重新选择
            return;
        }
        
        if (choice >= 1 && choice <= 6) {
            // 手动选择模式
            // 加载中动态提示+色彩过渡，增强等待体验
printf("\n========================================\n");
printf("\033[36;1m🔄 正在加载选定的驱动...\033[0m\n");
printf("\033[37;1m⌛ 请稍候，驱动初始化中...\033[0m\n");
printf("========================================\n\n");

            if (!load_specific_driver(choice)) {
                // 红色警示+引导性提示，失败反馈清晰且不劝退
printf("\n========================================\n");
printf("\033[31;1m❌ 驱动加载失败！\033[0m\n");
printf("----------------------------------------\n");
printf("\033[37;1m所选驱动暂不支持当前设备/环境\n");
printf("建议选择其他驱动类型重新尝试～\033[0m\n");
printf("========================================\n\n");

                interactive_driver_selection(); // 递归重新选择
                return;
            }
        } else {
            // 橙色醒目提示+明确指引，错误反馈精准不模糊
printf("\n----------------------------------------\n");
printf("\033[33;1m⚠️  选择无效！\033[0m\n");
printf("\033[37;1m请严格输入 1-6 之间的整数，重新选择～\033[0m\n");
printf("----------------------------------------\n\n");

            interactive_driver_selection(); // 递归重新选择
            return;
        }
    }

public:
    // 默认构造函数 - 交互式选择驱动
    c_driver() {
       
        
        // 交互式选择驱动
        interactive_driver_selection();
        
        
        printf("当前使用: %s\n", get_driver_type_name());
        
    }
    
    // 手动指定驱动类型的构造函数
    c_driver(int driver_type) {
        // 如果是RTHook驱动，按照rthook.h逻辑设置fd = 0
        if (driver_type == 6) {
            fd = 0;
        }
    
        if (driver_type >= 1 && driver_type <= 6) {
            printf("手动指定使用: ");
            switch (driver_type) {
                case 1: printf("qx11.4驱动\n"); break;
                case 2: printf("RTdev驱动\n"); break;
                case 3: printf("GT驱动\n"); break;
                case 4: printf("KMA内核模块驱动\n"); break;
                case 5: printf("RT Proc驱动\n"); break;
                case 6: printf("RTHook驱动\n"); break;
            }
            
            if (!load_specific_driver(driver_type)) {
                printf("指定驱动加载失败，程序退出\n");
                _Exit(0);
            }
        } else {
            printf("无效的驱动类型，请输入 1-6 之间的数字\n");
            _Exit(0);
        }
        
        
        printf("当前使用: %s\n", get_driver_type_name());
        
    }

    ~c_driver() {
        if (fd > 0) close(fd);
        if (kma_driver) {
            delete kma_driver;
            kma_driver = nullptr;
        }
    }

    void initialize(pid_t target_pid) {
        this->pid = target_pid;
        // 如果使用KMA驱动，也需要初始化KMA驱动的PID
        if (driver_version == 4 && kma_driver) {
            kma_driver->initpid(target_pid);
        }
    }


    bool is_loaded() const {
        return driver_version > 0;
    }

    int version() const {
        return driver_version;
    }

    	
	bool init_key(char* key) {
		char buf[0x100];
		strcpy(buf,key);
		if (ioctl(fd, OP_INIT_KEY, buf) != 0) {
			return false;
		}
		return true;
	}

	bool read(uintptr_t addr, void *buffer, size_t size) {
		// 如果使用KMA驱动
		if (driver_version == 4 && kma_driver) {
			return kma_driver->read(addr, buffer, size);
		}
		
		// 传统ioctl驱动
		addr=addr&0xFFFFFFFFFFFF;
		COPY_MEMORY cm;

		cm.pid = this->pid;
		cm.addr = addr;
		cm.buffer = buffer;
		cm.size = size;

		// 根据驱动类型选择不同的ioctl操作码
		int read_op = (driver_version == 6) ? RT_READ_MEM : OP_READ_MEM;
		if (ioctl(fd, read_op, &cm) != 0) {
			return false;
		}
		return true;
	}

	bool write(uintptr_t addr, void *buffer, size_t size) {
		// 如果使用KMA驱动
		if (driver_version == 4 && kma_driver) {
			return kma_driver->write(addr, buffer, size);
		}
		
		// 传统ioctl驱动
		COPY_MEMORY cm;

		cm.pid = this->pid;
		cm.addr = addr;
		cm.buffer = buffer;
		cm.size = size;

		// 根据驱动类型选择不同的ioctl操作码
		int write_op = (driver_version == 6) ? RT_WRITE_MEM : OP_WRITE_MEM;
		if (ioctl(fd, write_op, &cm) != 0) {
			return false;
		}
		return true;
	}

	template <typename T>
	T read(uintptr_t addr) {
		T res;
		if (this->read(addr, &res, sizeof(T)))
			return res;
		return {};
	}

	template <typename T>
	bool write(uintptr_t addr,T value) {
		return this->write(addr, &value, sizeof(T));
	}

	uintptr_t getModuleBase(char*module_name)
{
	// 声明变量
	char*phgsr;
	char jjjj_N[64];
	long startaddr = 0;
	char path[256],line[1024];
	bool bssOF = false,LastIsSo = false;
	
	// 如果使用KMA驱动
	if (driver_version == 4 && kma_driver) {
		// 检查是否是BSS段查找
		if (!strstr(module_name, ":bss")) {
			uintptr_t kma_base = kma_driver->get_module_base(this->pid, module_name);
			if (kma_base > 0) {
				return kma_base;
			}
		}
	}
	
	// 如果使用RTHook驱动
	if (driver_version == 6) {
		uintptr_t rthook_base = get_rthook_module_base(module_name);
		if (rthook_base > 0) {
			return rthook_base;
		}
	}
	
	// 内核版本适配：6.0及以上版本使用直接读取maps方式
	float kernel_ver = get_kernel_version();
	if (kernel_ver >= 6.0f && !strstr(module_name, ":bss")) {
		// 直接从maps文件获取模块基址（适用于高版本内核）
		sprintf(path, "/proc/%d/maps", this->pid);
		FILE *fp = fopen(path, "r");
		if (fp != NULL) {
			while (fgets(line, sizeof(line), fp)) {
				if (strstr(line, module_name) && strstr(line, "r-xp")) {
					sscanf(line, "%lx-%*lx", &startaddr);
					break;
				}
			}
			fclose(fp);
		}
		if (startaddr > 0) {
			return startaddr;
		}
	}
	
	// 传统方法获取模块基址
	// 如果不是BSS段查找，优先使用简洁的全局函数
	if (!strstr(module_name, ":bss")) {
		unsigned long simple_base = get_module_base(this->pid, module_name);
		if (simple_base > 0) {
			return simple_base;
		}
	}
	
	// 复杂BSS段处理
	strcpy(jjjj_N,module_name);
	phgsr = strtok(jjjj_N,":");
	module_name = phgsr;
	phgsr = strtok(NULL,":");
	if(phgsr)
	{
		if(strcmp(phgsr,"bss")==0)
		{
			bssOF = true;
		}
	}
	sprintf(path,"/proc/%d/maps",pid);
	FILE*p = fopen(path,"r");
	if(p)
	{
		while(fgets(line,sizeof(line),p))
		{
			if(LastIsSo)
			{
				if(strstr(line,"[anon:.bss]")!=NULL)
				{
					sscanf(line,"%lx-%*lx",&startaddr);
					break;
				}
				else
				{
					LastIsSo = false;
				}
			}
			if(strstr(line,module_name)!=NULL)
			{
				if(!bssOF)
				{
					sscanf(line,"%lx-%*lx",&startaddr);
					break;
				}
				else
				{
					LastIsSo = true;
				}
			}
		}
		fclose(p);
	}
	return startaddr;
}

	// ====================== KMA驱动特有功能 ======================
	
	// 硬件级安全读取（仅KMA驱动支持）
	template <typename T>
	T read_safe(uintptr_t addr) {
		if (driver_version == 4 && kma_driver) {
			return kma_driver->read_safe<T>(addr);
		}
		// 传统驱动回退到普通读取
		return read<T>(addr);
	}
	
	// 硬件级安全读取（原始接口）
	bool read_safe(uintptr_t addr, void *buffer, size_t size) {
		if (driver_version == 4 && kma_driver) {
			return kma_driver->read_safe(addr, buffer, size);
		}
		// 传统驱动回退到普通读取
		return read(addr, buffer, size);
	}
	
	// CPU亲和性设置（仅KMA驱动支持）
	void set_cpu_affinity(int cpu_num) {
		if (driver_version == 4 && kma_driver) {
			kma_driver->cpuset(cpu_num);
		}
	}
	
	// CPU亲和性范围设置（仅KMA驱动支持）
	void set_cpu_affinity_range(int start_cpu, int end_cpu) {
		if (driver_version == 4 && kma_driver) {
			kma_driver->cpuset(start_cpu, end_cpu);
		}
	}
	
	// 触摸系统初始化（仅KMA驱动支持）
	bool init_touch_system(int width, int height) {
		if (driver_version == 4 && kma_driver) {
			return kma_driver->uinput_init(width, height);
		}
		return false;
	}
	
	// 触摸按下（仅KMA驱动支持）
	void touch_down(int x, int y) {
		if (driver_version == 4 && kma_driver) {
			kma_driver->uinput_down(x, y);
		}
	}
	
	// 触摸移动（仅KMA驱动支持）
	void touch_move(int x, int y) {
		if (driver_version == 4 && kma_driver) {
			kma_driver->uinput_move(x, y);
		}
	}
	
	// 触摸抬起（仅KMA驱动支持）
	void touch_up() {
		if (driver_version == 4 && kma_driver) {
			kma_driver->uinput_up();
		}
	}
	
	// 获取随机坐标（仅KMA驱动支持）
	int get_random_coord(int val) {
		if (driver_version == 4 && kma_driver) {
			return kma_driver->uinput_rand(val);
		}
		return val; // 传统驱动返回原值
	}
	
	// 获取随机坐标（指定熵值，仅KMA驱动支持）
	int get_random_coord(int val, int entropy) {
		if (driver_version == 4 && kma_driver) {
			return kma_driver->uinput_rand(val, entropy);
		}
		return val; // 传统驱动返回原值
	}
	
	// ====================== RTHook驱动特有功能 ======================
	
	// 进程隐藏功能（仅RTHook驱动支持）
	void hide_process() { ioctl(fd, RT_HIDE_PROCESS); }
	
	// 测试隐藏进程功能（调试用）
	// void test_hide_process() {
	// 	printf("=== 测试隐藏进程功能 ===\n");
	// 	printf("当前驱动版本: %d\n", driver_version);
	// 	printf("当前fd: %d\n", fd);
	// 	bool result = hide_process();
	// 	printf("隐藏进程结果: %s\n", result ? "成功" : "失败");
	// 	printf("========================\n");
	// }
	
	uintptr_t get_rthook_module_base(const char *module_name) {
		if (driver_version != 6) return 0;
		
		long addr = 0;
		char module[64],lj[64],buff[256];
		char *part;
		bool bss = false;
		strcpy(module,module_name);
		part = strtok(module,":");
		strcpy(module,part);
		part = strtok(NULL,":");
		if(part)
		{
			if(strcmp(part,"bss")==0)
				bss = true;
		}
		if (this->pid <= 0)
			snprintf(lj, sizeof(lj), "/proc/self/maps");
		else
			snprintf(lj, sizeof(lj), "/proc/%d/maps", this->pid);
		FILE *fp = fopen(lj, "r");
		if(fp)
		{
			while(fgets(buff,sizeof(buff),fp))
			{
				if(strstr(buff,module)!=NULL)
				{
					if(strstr(buff,".so")!=NULL)
					{
						long add;
						sscanf(buff,"%lx-%*lx",&add);
						fgets(buff,sizeof(buff),fp);
						if(strstr(buff,module)==NULL){
							fgets(buff,sizeof(buff),fp);
						}
						if(strstr(buff,module)!=NULL)
						{
							if(bss){
								while(fgets(buff,sizeof(buff),fp))
								{
									if(strstr(buff,"[anon:.bss]")!=NULL)
									{
										sscanf(buff,"%lx-%*lx",&addr);
										break;
									}
								}
								break;
							}else{
								addr = add;
								break;
							}
						}
					}else{
						sscanf(buff,"%lx-%*lx",&addr);
						break;
					}
				}
			}
			fclose(fp);
		}
		return addr;
	}
	
	// 获取驱动类型名称
	const char* get_driver_type_name() const {
		switch (driver_version) {
			case 1: return "qx11.4驱动";
			case 2: return "RTdev驱动";
			case 3: return "GT驱动";
			case 4: return "KMA内核模块驱动";
			case 5: return "RT Proc驱动";
			case 6: return "RTHook驱动";
			default: return "未知驱动";
		}
	}
	
	// 检查是否为KMA驱动
	bool is_kma_driver() const {
		return driver_version == 4 && kma_driver != nullptr;
	}
	
	// 检查是否为RTHook驱动
	bool is_rthook_driver() const {
		return driver_version == 6;
	}
	
	// 获取内核版本
	float get_kernel_version() {
		const char* command = "uname -r | sed 's/\\.[^.]*$//g'";
		FILE* file = popen(command, "r");
		if (file == NULL) {
			return 0.0f;
		}
		static char result[512];
		if (fgets(result, sizeof(result), file) == NULL) {
			pclose(file);
			return 0.0f;
		}
		pclose(file);
		result[strlen(result)-1] = '\0';
		return atof(result);
	}
	
	// 获取原始KMA驱动实例（高级用法）
	Driver* get_kma_driver() {
		return (driver_version == 4) ? kma_driver : nullptr;
	}

	// KMA驱动调试和测试函数
	void test_kma_driver() {
		if (driver_version != 4 || !kma_driver) {
			printf("当前未使用KMA驱动\n");
			return;
		}
		
		printf("=== KMA驱动测试 ===\n");
		
		// 测试PID获取
		printf("测试PID获取功能...\n");
		pid_t test_pid = kma_driver->get_pid("system_server");
		printf("system_server PID: %d\n", test_pid);
		
		if (this->pid > 0) {
			printf("当前PID: %d\n", this->pid);
			
			// 测试模块基址获取
			printf("测试模块基址获取...\n");
			uintptr_t libc_base = kma_driver->get_module_base(this->pid, "libc.so");
			printf("libc.so 基址: 0x%lX\n", libc_base);
			
			if (libc_base > 0) {
				// 测试内存读取
				printf("测试内存读取...\n");
				uint32_t magic = kma_driver->read<uint32_t>(libc_base);
				printf("libc.so magic: 0x%X\n", magic);
				
				// 测试安全读取
				uint32_t safe_magic = kma_driver->read_safe<uint32_t>(libc_base);
				printf("libc.so safe magic: 0x%X\n", safe_magic);
			}
		}
		
		printf("=== KMA驱动测试完成 ===\n");
	}

	
};

// 驱动实例 - 默认使用交互式选择
static c_driver* driver = new c_driver();

// 手动创建指定类型的驱动实例
inline c_driver* create_driver_with_type(int driver_type) {
    return new c_driver(driver_type);
}

// 重新初始化驱动为指定类型
inline void reinit_driver_with_type(int driver_type) {
    if (driver) {
        delete driver;
    }
    driver = new c_driver(driver_type);
}

// 显示驱动选择菜单
inline void show_driver_selection_menu() {
    printf("=== 驱动选择菜单 ===\n");
    printf("1. qx11.4驱动\n");
    printf("2. RTdev驱动\n");
    printf("3. GT驱动\n");
    printf("4. KMA内核模块驱动\n");
    printf("5. RT Proc驱动\n");
    printf("6. RTHook驱动\n");
    printf("请主动选择一种驱动类型\n");
    printf("==================\n");
}

// 测试KMA驱动功能
inline void test_kma_functionality() {
    if (driver && driver->is_kma_driver()) {
        driver->test_kma_driver();
    } else {
        printf("当前未使用KMA驱动，无法测试\n");
    }
}


typedef char PACKAGENAME;	// 包名
pid_t pid;	// 进程ID

float Kernel_v()
{
	const char* command = "uname -r | sed 's/\\.[^.]*$//g'";
	FILE* file = popen(command, "r");
	if (file == NULL) {
		return 0.0f;
	}
	static char result[512];
	if (fgets(result, sizeof(result), file) == NULL) {
		pclose(file);
		return 0.0f;
	}
	pclose(file);
	result[strlen(result)-1] = '\0';
	return atof(result);
}

char *GetVersion(char* PackageName)
{
	char command[256];
	sprintf(command, "dumpsys package %s|grep versionName|sed 's/=/\\n/g'|tail -n 1", PackageName);
	FILE* file = popen(command, "r");
	if (file == NULL) {
		return NULL;
	}
	static char result[512];
	if (fgets(result, sizeof(result), file) == NULL) {
		pclose(file);
		return NULL;
	}
	pclose(file);
	result[strlen(result)-1] = '\0';
	return result;
}

uint64_t GetTime()
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC,&ts);
	return (ts.tv_sec*1000 + ts.tv_nsec/(1000*1000));
}

char *getDirectory()
{






	static char buf[128];
	int rslt = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
	if (rslt < 0 || (rslt >= sizeof(buf) - 1))
	{
		return NULL;
	}
	buf[rslt] = '\0';
	for (int i = rslt; i >= 0; i--)
	{
		if (buf[i] == '/')
		{
			buf[i] = '\0';
			break;
		}
	}
	return buf;
}

int getPID(char* PackageName)
{
	// 如果使用KMA驱动，优先使用KMA的PID获取方法
	if (driver->is_kma_driver()) {
		Driver* kma = driver->get_kma_driver();
		if (kma) {
			pid_t kma_pid = kma->get_pid(PackageName);
			if (kma_pid > 0) {
				pid = kma_pid;
				driver->initialize(pid);
				return pid;
			}
		}
	}
	
	// 传统方法获取PID
	FILE* fp;
    char cmd[0x100] = "pidof ";
    strcat(cmd, PackageName);
    fp = popen(cmd,"r");
    fscanf(fp,"%d", &pid);
    pclose(fp);
	if (pid > 0)
	{
		driver->initialize(pid);
	}
    return pid;
}

bool PidExamIne()
{
	char path[128];
	sprintf(path, "/proc/%d",pid);
	if (access(path,F_OK) != 0)
	{
		printf("\033[31;1m");
		puts("[!] 获取进程PID失败!");
		exit(1);
	}
	return true;
}

uintptr_t getModuleBase(char* module_name)
{
	uintptr_t base=0;
	
	// 内核版本适配
	float kernel_ver = driver->get_kernel_version();
	if (kernel_ver >= 6.0f && !strstr(module_name, ":bss")) {
		// 高版本内核使用直接maps读取
		char path[256], line[1024];
		sprintf(path, "/proc/%d/maps", pid);
		FILE *fp = fopen(path, "r");
		if (fp != NULL) {
			while (fgets(line, sizeof(line), fp)) {
				if (strstr(line, module_name) && strstr(line, "r-xp")) {
					sscanf(line, "%lx-%*lx", &base);
					break;
				}
			}
			fclose(fp);
		}
		if (base > 0) {
			return base;
		}
	}
	
	// 传统驱动方式
	base = driver->getModuleBase(module_name);
	return base;
}


long ReadValue(long addr)
{
	long he=0;
	if (addr < 0xFFFFFFFF){
		driver->read(addr, &he, 4);
	}else{
		driver->read(addr, &he, 8);
		he=he&0xFFFFFFFFFFFF;
	}
	return he;
}

long ReadDword(long addr)
{
	long he=0;
	driver->read(addr, &he, 4);
	return he;
}

float ReadFloat(long addr)
{
	float he=0;
	driver->read(addr, &he, 4);
	return he;
}

int *ReadArray(long addr)
{
	int *he = (int *) malloc(12);
	driver->read(addr, he, 12);
	return he;
}

int WriteDword(long int addr, int value)
{
	driver->write(addr, &value, 4);
	return 0;
}

int WriteFloat(long int addr, float value)
{
	driver->write(addr, &value, 4);
	return 0;
}

long GetModuleBaseAddr(char* module_name)
{
	long addr = 0;
	char filename[32];
	char line[1024];
	if (pid < 0)
	{
		snprintf(filename, sizeof(filename), "/proc/self/maps", pid);
	}
	else
	{
		snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
	}
	FILE *fp = fopen(filename, "r");
	if (fp != NULL)
	{
		while (fgets(line, sizeof(line), fp))
		{
			if (strstr(line, module_name))
			{
				sscanf(line,"%lx-%*lx",&addr);
				break;
			}
		}
		fclose(fp);
	}
	return addr;
}

long getModuleBase(const char *module_name)
{
	long addr = 0;
	char module[64],lj[64],buff[256];
	char *part;
	bool bss = false;
	strcpy(module,module_name);
	part = strtok(module,":");
	strcpy(module,part);
	part = strtok(NULL,":");
	if(part)
	{
		if(strcmp(part,"bss")==0)
			bss = true;
	}
	if (pid <= 0)
		snprintf(lj, sizeof(lj), "/proc/self/maps");
	else
		snprintf(lj, sizeof(lj), "/proc/%d/maps", pid);
	FILE *fp = fopen(lj, "r");
	if(fp)
	{
		while(fgets(buff,sizeof(buff),fp))
		{
			if(strstr(buff,module)!=NULL)
			{
				if(strstr(buff,".so")!=NULL)
				{
					long add;
					sscanf(buff,"%lx-%*lx",&add);
					fgets(buff,sizeof(buff),fp);
					if(strstr(buff,module)==NULL){
						fgets(buff,sizeof(buff),fp);
					}
					if(strstr(buff,module)!=NULL)
					{
						if(bss){
							while(fgets(buff,sizeof(buff),fp))
							{
								if(strstr(buff,"[anon:.bss]")!=NULL)
								{
									sscanf(buff,"%lx-%*lx",&addr);
									break;
								}
							}
							break;
						}else{
							addr = add;
							break;
						}
					}
				}else{
					sscanf(buff,"%lx-%*lx",&addr);
					break;
				}
			}
		}
		fclose(fp);
	}
	return addr;
}

// 获取基址
unsigned long get_module_base(int pid, const char *module_name)
{
	FILE *fp;
	unsigned long addr = 0;
	char *pch;
	char filename[64];
	char line[1024];
	//char *xa = "r-xp";
	snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
	fp = fopen(filename, "r");
	if (fp != NULL)
	{
		while (fgets(line, sizeof(line), fp))
		{
			if (strstr(line, module_name) && strstr(line, "r-xp"))
			{
				pch = strtok(line, "-");
				addr = strtoul(pch, NULL, 16);
				if (addr == 0x8000)
					addr = 0;
				break;
			}
		}
		fclose(fp);
	}
	return addr;
}

// 获取BSS模块地址
long get_module_bss(int pid, const char *szModule)
{
    FILE *fp;
    int cnt = 0;
    long start;
    char tmp[256];
    fp = NULL;
    char line[1024];
    char name[128];
    sprintf(name, "/proc/%d/maps", pid);
    fp = fopen(name, "r");
    while (!feof(fp))
    {
        fgets(tmp, 256, fp);
        if (cnt == 1)
        {
            if (strstr(tmp, "[anon:.bss]") != NULL)
            {
                sscanf(tmp, "%lx-%*lx", &start);
                break;
            }
            else
            {
                cnt = 0;
            }
        }
        if (strstr(tmp, szModule) != NULL)
        {
            cnt = 1;
        }
    }
    fclose(fp);
    return start;
}

struct Vector2A {
    float X;
    float Y;

    Vector2A() {
        this->X = 0;
        this->Y = 0;
    }

    Vector2A(float x, float y) {
        this->X = x;
        this->Y = y;
    }
};


struct Vector3A {
    float X;
    float Y;
    float Z;

    Vector3A() {
        this->X = 0;
        this->Y = 0;
        this->Z = 0;
    }

    Vector3A(float x, float y, float z) {
        this->X = x;
        this->Y = y;
        this->Z = z;
    }

};

#ifndef VECTOR3_DEFINED
struct Vector3{
    //这边重定义一个xzy形式的坐标(因为u3d是xzy);
    float X;
    float Z;
    float Y;

    Vector3() {
        this->X = 0;
        this->Z = 0;
        this->Y = 0;
    }

    Vector3(float x, float z, float y) {
        this->X = x;
        this->Z = z;
        this->Y = y;
    }
};
#endif

