@echo off
echo Building CPointer BaiYi for Android ARM64 (Pure C++)...

REM 设置NDK路径 - 请根据实际安装路径修改
set NDK_PATH=C:\Users\%USERNAME%\AppData\Local\Android\Sdk\ndk\25.2.9519653

REM 检查NDK是否存在
if not exist "%NDK_PATH%" (
    echo Error: NDK not found at %NDK_PATH%
    echo Please install Android NDK and update the NDK_PATH in this script
    pause
    exit /b 1
)

REM 清理之前的构建
if exist "libs" rmdir /s /q "libs"
if exist "obj" rmdir /s /q "obj"

REM 执行NDK构建
echo Running ndk-build...
"%NDK_PATH%\ndk-build.cmd" -j4

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Build successful!
    echo Output: libs\arm64-v8a\cpointer_baiyi
    echo.
    echo To install and run on device:
    echo adb push libs\arm64-v8a\cpointer_baiyi /data/local/tmp/
    echo adb shell chmod 755 /data/local/tmp/cpointer_baiyi
    echo adb shell /data/local/tmp/cpointer_baiyi
    echo.
    echo To view logs:
    echo adb logcat -s CPointerBaiYi
) else (
    echo.
    echo Build failed! Check the error messages above.
)

pause
