@echo off
echo Testing NDK build configuration...

REM 检查必要文件是否存在
echo Checking required files...

if not exist "Android.mk" (
    echo Error: Android.mk not found
    exit /b 1
)

if not exist "Application.mk" (
    echo Error: Application.mk not found
    exit /b 1
)

if not exist "jni\src\main.cpp" (
    echo Error: main.cpp not found
    exit /b 1
)

if not exist "jni\src\chainer" (
    echo Error: chainer directory not found
    exit /b 1
)

if not exist "jni\src\memtool" (
    echo Error: memtool directory not found
    exit /b 1
)

if not exist "jni\src\utils" (
    echo Error: utils directory not found
    exit /b 1
)

echo All required files found!
echo.
echo Project structure:
echo - Android.mk: OK
echo - Application.mk: OK
echo - jni/src/main.cpp: OK
echo - jni/src/chainer/: OK
echo - jni/src/memtool/: OK
echo - jni/src/utils/: OK
echo.
echo Ready for NDK build!
echo.
echo To build, run:
echo   ndk-build -j4
echo.
echo Or use the provided build script:
echo   build_android.bat
