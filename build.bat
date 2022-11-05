@echo off

set ARG=%1
set ROOTDIR=%cd%
set CONFIG=release
set BUILDDIR="%ROOTDIR%\build\%CONFIG%"

mkdir "%BUILDDIR%"
cd "%BUILDDIR%"

if "%ARG%"=="install" (
    cmake --build . --target INSTALL --config RELEASE
    exit 0
)

cmake -DCMAKE_BUILD_TYPE=%CONFIG% %ROOTDIR%
cmake --build . --target ALL_BUILD

cd "%ROOTDIR%"
copy "%BUILDDIR%\compile_commands.json" .

echo Finished creation of: gum-maths