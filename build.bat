@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64
if not exist build mkdir build
cl /std:c++20 /EHsc /W4 /I include src\*.cpp /Fe:build\fileflow.exe
if %errorlevel% neq 0 (
    echo Build failed
    exit /b %errorlevel%
)
echo Build successful