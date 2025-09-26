# CPointer BaiYi - Android ARM64 版本

这是CPointer BaiYi项目的Android ARM64版本，使用NDK-build构建系统，纯C++实现。

## 项目结构

```
CPointer_BaiYi/
├── Android.mk              # NDK构建配置
├── Application.mk          # Android平台配置
├── build_android.bat       # Windows构建脚本
├── build_android.sh        # Linux/macOS构建脚本
├── jni/
│   └── src/
│       ├── main.cpp        # 主程序入口（纯C++）
│       ├── chainer/        # 指针链扫描模块
│       ├── memtool/        # 内存操作模块
│       └── utils/          # 工具模块
└── README_ANDROID.md       # 本文档
```

## 构建要求

- Android NDK r25.2.9519653 或更高版本
- Windows: Visual Studio 或 MinGW
- Linux/macOS: GCC 或 Clang

## 构建步骤

### Windows

1. 安装Android NDK
2. 修改 `build_android.bat` 中的NDK路径
3. 运行构建脚本：
   ```cmd
   build_android.bat
   ```

### Linux/macOS

1. 安装Android NDK
2. 修改 `build_android.sh` 中的NDK路径
3. 给脚本添加执行权限：
   ```bash
   chmod +x build_android.sh
   ```
4. 运行构建脚本：
   ```bash
   ./build_android.sh
   ```

## 安装和运行

构建成功后，可执行文件位于 `libs/arm64-v8a/cpointer_baiyi`

### 通过ADB安装到设备

```bash
# 推送可执行文件到设备
adb push libs/arm64-v8a/cpointer_baiyi /data/local/tmp/

# 设置执行权限
adb shell chmod 755 /data/local/tmp/cpointer_baiyi

# 运行程序
adb shell /data/local/tmp/cpointer_baiyi
```

### 查看日志

```bash
# 查看程序输出日志
adb logcat -s CPointerBaiYi
```

## 功能说明

- **指针扫描**: 扫描指定进程的内存，查找指针链
- **内存操作**: 读取和写入目标进程内存
- **多线程支持**: 使用线程池提高扫描效率
- **Android日志**: 使用Android日志系统输出调试信息
- **纯C++实现**: 无需Java代码，直接使用main函数作为入口

## 注意事项

1. 需要root权限才能访问其他进程的内存
2. 目标进程名称默认为 "gg.pointers"，可在代码中修改
3. 扫描结果保存在 `/data/local/tmp/pointer_scan_result`
4. 程序使用Android API Level 22 (Android 5.1)

## 故障排除

### 构建失败

1. 检查NDK路径是否正确
2. 确保安装了正确的Android API Level
3. 检查C++编译器是否可用

### 运行时错误

1. 确保设备已root
2. 检查目标进程是否存在
3. 查看logcat输出获取详细错误信息

## 许可证

本项目基于原CPointer BaiYi项目，遵循相同的开源许可证。
