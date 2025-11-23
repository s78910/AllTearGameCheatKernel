/* 
 * 由光头强内核（原XF内核）开源
 因频道账号被盗用后决定开源，正版为光头强内核，
开源文件仅供学习交流，任何个人或组织可在遵循协议条款的前提下，自由下载、使用、修改、分发本项目代码及衍生作品，无需额外授权。使用过程中请保留原项目版权声明及开源协议文本，不得用于违法违规场景。本项目以“现状”提供，开发者不承担任何直接或间接的使用风险及法律责任，亦不提供商业技术支持。
删除此内容死全家，没几把
* 注入时间: 2025-11-16 20:14:49
 * 联系方式: TG:@GTQNHNB666
QQ群：784316342
 */
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/mman.h> 
#include <fcntl.h>


int Key() {
    char filename[256];
    sprintf(filename, "/proc/%d/maps", pid);
    FILE* handle = fopen(filename, "r");
    if (!handle) {
        perror("fopen");
        return -1;
    }

    char line[1024];
    while (fgets(line, sizeof(line), handle)) {
        if (strstr(line, "[anon:libc_malloc]")) {
            unsigned long start, end;
            if (sscanf(line, "%lx-%lx", &start, &end) == 2) {
                size_t size = end - start;
                void* addr = mmap(NULL, size, PROT_READ, MAP_PRIVATE, pid, start);
                if (addr == MAP_FAILED) {
                    perror("mmap");
                    continue; // 跳过这个区域，尝试下一个
                }

                unsigned int* p = (unsigned int*)addr;
                for (size_t i = 0; i < size / sizeof(unsigned int); ++i) {
                    if (ReadDword(p + i) == 256) {
                        //printf("Found 256 at address: %p\n", (void*)(p + i));
                    
                
                        // 检查0x50字节后的地址
                        void* next_addr = (char*)(p + i) + 0x50;
                        if (next_addr < (char*)addr + size) {
                            if (ReadDword(next_addr) == 256) {
                                printf("Also found 256 at address: %p\n", next_addr);
                            }
                        }
                    }
                }
                if (munmap(addr, size) == -1) {
                    perror("munmap");
                }
            }
        }
    }

    fclose(handle);
    return 0;
}
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define ReadDword(addr) (*((unsigned int*)(addr)))

int main() {
    pid_t pid = ...; // 进程ID
    char filename[256];
    sprintf(filename, "/proc/%d/maps", pid);
    FILE* handle = fopen(filename, "r");
    if (!handle) {
        perror("fopen");
        return -1;
    }

    char line[1024];
    while (fgets(line, sizeof(line), handle)) {
        if (strstr(line, "[anon:libc_malloc]")) {
            unsigned long start, end;
            if (sscanf(line, "%lx-%lx", &start, &end) == 2) {
                size_t size = end - start;
                void* addr = mmap(NULL, size, PROT_READ, MAP_PRIVATE, pid, start);
                if (addr == MAP_FAILED) {
                    perror("mmap");
                    continue; // 跳过这个区域，尝试下一个
                }

                unsigned int* p = (unsigned int*)addr;
                for (size_t i = 0; i < size / sizeof(unsigned int); ++i) {
                    if (ReadDword(p + i) == 256) {
                        printf("Found 256 at address: %p\n", (void*)(p + i));

                        // 检查0x50字节后的地址
                        void* next_addr = (char*)(p + i) + 0x50;
                        if (next_addr < (char*)addr + size) {
                            if (ReadDword(next_addr) == 256) {
                                printf("Also found 256 at address: %p\n", next_addr);
                            }
                        }
                    }
                }

                if (munmap(addr, size) == -1) {
                    perror("munmap");
                }
            }
        }
    }

    fclose(handle);
    return 0;
}
*/