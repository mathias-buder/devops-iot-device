@echo off
SETLOCAL EnableDelayedExpansion

set ORIG_PATH=%PATH%

set TOOLS_DIR=..\..\075_Build_Tools
set COMPILER_PATH=%TOOLS_DIR%\MinGW64_73\bin\
set CMAKE_PATH=%TOOLS_DIR%\cmake-3.5.2-win32-x86\bin
set PYTHON_PATH=%TOOLS_DIR%\Python35
    
set PARAMS=%*
set PARAMS_WITHOUT_ALL="%PARAMS:all=%"

set PARAMS_WITHOUT_CLEAN="%PARAMS:clean=%"
    
if not %PARAMS_WITHOUT_CLEAN% == "%PARAMS%" (
    goto clean)     
    
if not %PARAMS_WITHOUT_ALL% == "%PARAMS%" (
    goto build
    )
    
goto default

:default
	echo Usage: build.cmd [-jN] [all^|clean]
	set BUILD_PARAMS=%1 %2 %3
	goto build # _rem end

:build
	rem convert COMPILER_PATH to absolute path
	pushd %COMPILER_PATH%
	set ABS_COMPILER_PATH=%CD%
	popd
	pushd %CMAKE_PATH%
	set ABS_CMAKE_PATH=%CD%
	popd
	pushd %PYTHON_PATH%
	set ABS_PYTHON_PATH=%CD%
	popd
	set PATH=%ABS_COMPILER_PATH%;%ABS_CMAKE_PATH%;%ABS_PYTHON_PATH%
	echo PATH set to %PATH%
	echo [WeGetA] Performing build command with options %BUILD_PARAMS% cmake_options: %CMAKE_PARAMS%
	cmake -H. -Bbuild -G "MinGW Makefiles" %CMAKE_PARAMS% -DCMAKE_BUILD_TYPE=Debug
	cmake --build build -- %BUILD_PARAMS%
	goto end

:clean
	echo [WeGetA] Performing clean command
	rmdir /s /q build
	goto end

:end
	set PATH=%ORIG_PATH%

