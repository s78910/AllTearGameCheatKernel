#include <stdio.h>
#include <string.h>
#include <android/log.h>
#include "chainer/ccformat.h"
#include "chainer/ccscan.h"
//// 泪心修复@TearGame

#define LOG_TAG "CPointerBaiYi"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

int main()
{
    LOGI("Starting CPointer BaiYi scan...");
    printf("Starting CPointer BaiYi scan...\n");
    
    chainer::cscan<size_t> t; // 假定为64位 32位改uint32_t

    // 用户手动输入包名
    char package_name[256];
    printf("Please enter the target package name: ");
    if (fgets(package_name, sizeof(package_name), stdin) == NULL) {
        LOGE("Failed to read package name");
        printf("Failed to read package name\n");
        return -1;
    }
    
    // 移除换行符
    package_name[strcspn(package_name, "\n")] = 0;
    
    printf("Searching for package: %s\n", package_name);
    LOGI("Searching for package: %s", package_name);

    memtool::base::target_pid = memtool::base::get_pid(package_name);
    printf("get_pid returned: %d\n", memtool::base::target_pid);
    
    if (memtool::base::target_pid == -1) {
        LOGE("Failed to get target PID for package: %s", package_name);
        printf("Failed to get target PID for package: %s\n", package_name);
        return -1;
    }
    
    // memtool::base::target_pid = 9999; 获取有错就先这样
    
    LOGI("Target PID: %d", memtool::base::target_pid);
    printf("Target PID: %d\n", memtool::base::target_pid);

    printf("Getting target memory...\n");
    LOGI("Getting target memory...");
    memtool::extend::get_target_mem();

    printf("Setting memory ranges...\n");
    LOGI("Setting memory ranges...");
    memtool::extend::set_mem_ranges(memtool::Anonymous + memtool::C_alloc + memtool::C_bss + memtool::C_data);

    printf("Starting pointer scan...\n");
    LOGI("Starting pointer scan...");
    int result = t.get_pointers(0, 0, false, 10, 1 << 20);
    LOGI("Get pointers result: %d", result);
    printf("Get pointers result: %d\n", result);

    // 用户手动输入十六进制基址
    char hex_address[64];
    printf("Please enter the base address (hex, e.g., 0x715334ED44): ");
    if (fgets(hex_address, sizeof(hex_address), stdin) == NULL) {
        LOGE("Failed to read base address");
        printf("Failed to read base address\n");
        return -1;
    }
    
    // 移除换行符
    hex_address[strcspn(hex_address, "\n")] = 0;
    
    // 解析十六进制地址
    size_t base_addr = 0;
    if (sscanf(hex_address, "0x%zx", &base_addr) != 1 && sscanf(hex_address, "%zx", &base_addr) != 1) {
        LOGE("Invalid hex address format: %s", hex_address);
        printf("Invalid hex address format: %s\n", hex_address);
        return -1;
    }
    
    printf("Using base address: 0x%zx\n", base_addr);
    LOGI("Using base address: 0x%zx", base_addr);

    std::vector<size_t> addr;
    addr.emplace_back(base_addr);
//// 泪心修复@TearGame

    // 用户输入扫描参数
    char depth_str[32], offset_str[32];
    
    printf("Enter scan depth (default 10): ");
    if (fgets(depth_str, sizeof(depth_str), stdin) == NULL) {
        depth_str[0] = '\0';
    }
    depth_str[strcspn(depth_str, "\n")] = 0;
    int scan_depth = (depth_str[0] == '\0') ? 10 : atoi(depth_str);
    
    printf("Enter scan offset (default 500): ");
    if (fgets(offset_str, sizeof(offset_str), stdin) == NULL) {
        offset_str[0] = '\0';
    }
    offset_str[strcspn(offset_str, "\n")] = 0;
    int scan_offset = (offset_str[0] == '\0') ? 500 : atoi(offset_str);
    
    printf("Using scan depth: %d, offset: %d\n", scan_depth, scan_offset);
    LOGI("Using scan depth: %d, offset: %d", scan_depth, scan_offset);

    printf("Scanning pointer chain...\n");
    LOGI("Scanning pointer chain...");
    auto f = fopen("/data/local/tmp/pointer_scan_result", "wb+");
    if (f) {
        long scan_result = t.scan_pointer_chain(addr, scan_depth, scan_offset, false, 0, f);
        LOGI("Scan pointer chain result: %ld", scan_result);
        printf("Scan pointer chain result: %ld\n", scan_result);
        fclose(f);
    } else {
        LOGE("Failed to create output file");
        printf("Failed to create output file\n");
    }

    /*chainer::cformat<size_t> t2;
    auto f2 = fopen("/data/local/tmp/pointer_scan_result", "rb+");

    if (f2) {
        printf("%ld\n", t2.format_bin_chain_data(f2, "/data/local/tmp/formatted_result", 0)); // 文件
        //  printf("%ld\n", t2.format_bin_chain_data(f2, "/data/local/tmp/formatted_result", 1)); // 文件夹 需要在当前目录有2文件夹
        fclose(f2);
    }*/
    
    LOGI("CPointer BaiYi scan completed");
    printf("CPointer BaiYi scan completed\n");
    return 0;
}