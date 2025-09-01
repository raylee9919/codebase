@echo off
setlocal enabledelayedexpansion
cd /D "%~dp0"

where /q cl || (
    echo [ERROR]: "cl" not found - please run this from the MSVC x64 native tools command prompt.
    exit /b 1
)

:: Unpack Arguments.
for %%a in (%*) do set "%%a=1"
if not "%release%"=="1" set debug=1
if "%debug%"=="1" set release=0 && echo [Debug Build]
if "%release%"=="1" set debug=0 && echo [Release Build]

:: Compile/Link Line Definitions
set flags_common=/std:c++17 /nologo /FC /Z7 /utf-8 /D_CRT_SECURE_NO_WARNINGS /I..\src\
set flags_debug=/Od /DBUILD_DEBUG=1
set flags_release=/O2 /DBUILD_DEBUG=0
set flags_warning=/W4 /wd4042 /wd4201 /wd4505 /wd4100 /wd4189 /wd4200
set flags_linker=/incremental:no /opt:ref

:: Choose Compile/Link Lines
                        set flags_compile=%flags_common% %flags_warning%
if "%debug%"=="1"       set flags_compile=%flags_compile% %flags_debug%
if "%release%"=="1"     set flags_compile=%flags_compile% %flags_release%

:: ------------ Project ------------ ::
set libs=Gdi32.lib
if not exist build mkdir build
pushd build
call cl ..\src\example.cpp %flags_compile% /link %flags_linker% %libs%
popd
