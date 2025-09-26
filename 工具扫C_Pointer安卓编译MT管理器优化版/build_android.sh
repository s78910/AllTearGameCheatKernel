#!/bin/bash

echo "Building CPointer BaiYi for Android ARM64 (Pure C++)..."

# 设置NDK路径 - 请根据实际安装路径修改
NDK_PATH="$HOME/Android/Sdk/ndk/25.2.9519653"

# 检查NDK是否存在
if [ ! -d "$NDK_PATH" ]; then
    echo "Error: NDK not found at $NDK_PATH"
    echo "Please install Android NDK and update the NDK_PATH in this script"
    exit 1
fi

# 清理之前的构建
rm -rf libs obj

# 执行NDK构建
echo "Running ndk-build..."
"$NDK_PATH/ndk-build" -j4

if [ $? -eq 0 ]; then
    echo ""
    echo "Build successful!"
    echo "Output: libs/arm64-v8a/cpointer_baiyi"
    echo ""
    echo "To install and run on device:"
    echo "adb push libs/arm64-v8a/cpointer_baiyi /data/local/tmp/"
    echo "adb shell chmod 755 /data/local/tmp/cpointer_baiyi"
    echo "adb shell /data/local/tmp/cpointer_baiyi"
    echo ""
    echo "To view logs:"
    echo "adb logcat -s CPointerBaiYi"
else
    echo ""
    echo "Build failed! Check the error messages above."
    exit 1
fi
