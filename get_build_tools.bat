@echo off
setlocal EnableDelayedExpansion

set PROJECT_PATH=%~dp0
REM Env variable is set only after calling this script as admin once. Afterwards, it does not need to be called as admin anymore.
@echo Build tools env var is !BUILD_TOOLS_ENV_VAR!

if exist !BUILD_TOOLS_ENV_VAR! (

    set BUILD_TOOLS_PATH=!BUILD_TOOLS_ENV_VAR!
    
    REM Check if all build tools are present. This only checks if the _FOLDER_ is present, if it is present and empty, the scrpit sees the tool as available, maybe I will change that some other time.
    call :download_cmake cmake-3.5.2-win32-x86
    call :download_mingw72 MinGW64_72 
    call :download_mingw73 MinGW64_73 
    call :download_linux msys_i686_linux_494 
    call :download_python Python35 
    call :download_pygeta pygeta

    @echo -------------------------------------------------------
    @echo I found all needed build tools under !BUILD_TOOLS_PATH!
    @echo -------------------------------------------------------

    goto BUILD_TOOLS_FOUND
    
) else (

    @set DEFAULT_PATH=C:\Build_Tools
    @set /p CUSTOMIZE_PATH=Getting build tools from git repository. The default path is !DEFAULT_PATH!. Customize path: (y/n)

    REM Set custom build tools path
    @if "%CUSTOMIZE_PATH%"=="y" (
    @set /p CUSTOM_BUILD_TOOLS_PATH=Enter the path you wish to have as location for the build tools:
    @set BUILD_TOOLS_PATH=!CUSTOM_BUILD_TOOLS_PATH!
    @echo !BUILD_TOOLS_PATH!
    
    REM Check if custom build tools path already exists, else create it.
    if exist !BUILD_TOOLS_PATH! (
    REM nothing
    ) else (
    @echo !BUILD_TOOLS_PATH!
    mkdir !BUILD_TOOLS_PATH!
    )
    
    REM Use default path
    ) else (
    @set BUILD_TOOLS_PATH=!DEFAULT_PATH!
    
    REM Check if default build tools path already exists, else create it.
    if exist !BUILD_TOOLS_PATH! (
    REM nothing
    ) else (
    @echo !BUILD_TOOLS_PATH!
    mkdir !BUILD_TOOLS_PATH!
    )
    )

    call :download_cmake cmake-3.5.2-win32-x86
    call :download_mingw72 MinGW64_72 
    call :download_mingw73 MinGW64_73 
    call :download_linux msys_i686_linux_494 
    call :download_python Python35 
    call :download_pygeta pygeta
    
    setx BUILD_TOOLS_ENV_VAR "!BUILD_TOOLS_PATH!" /m
    @echo I put the build tools under !BUILD_TOOLS_PATH!
    @echo Path has been added to your environment variables if executed as admin.
)

:BUILD_TOOLS_FOUND

chdir /d %PROJECT_PATH%

@set /p PATH_CORRECT=Is --- %PROJECT_PATH:~0,-1% --- the path to the project you want to be able to build: (y/n) 

@if "%PATH_CORRECT%"=="y" (
 	@set PROJECT_PATH=%PROJECT_PATH%
) else (
	@set /p PROJECT_PATH=Please provide the path to the project you want to be able to build:
)

if exist %PROJECT_PATH%\075_Build_Tools RMDIR /S /Q %PROJECT_PATH%\075_Build_Tools
if exist %PROJECT_PATH%\075_Build_Tools RMDIR /S /Q %PROJECT_PATH%\075_Build_Tools

REM @echo I will do the following:
REM @echo mklink /J %PROJECT_PATH%\075_Build_Tools\ %BUILD_TOOLS_PATH%

mklink /J %PROJECT_PATH%\075_Build_Tools %BUILD_TOOLS_PATH%
@echo Symbolic links created, project ready to be built.

pause
exit /B

REM --------------------- HELPER FUNCTIONS --------------------- REM

:download_pygeta
if exist !BUILD_TOOLS_PATH!\%1 (
    @echo I found your %1 under !BUILD_TOOLS_PATH!\%1
) else (
    @echo %1 not found, getting %1 from git repository
    mkdir !BUILD_TOOLS_PATH!\%1
    cp "\\ad.trw.com\koblenz\Departments\EN\DAS\DAS-VSI\projects\KOPIER\KohnenR\%1\%1.zip" "!BUILD_TOOLS_PATH!/%1/"
    call :prepare %1
    @echo I put %1 under !BUILD_TOOLS_PATH!\%1
    
)
exit /B

REM All those functions do the same thing except that the link is different. The powershell does not simply take i.e. %2 here as argument. May be fixed in the future, this was the fastest, but it is pretty ugly.
:download_cmake
if exist !BUILD_TOOLS_PATH!\%1 (
    @echo I found your %1 under !BUILD_TOOLS_PATH!\%1
) else (
    @echo %1 not found, getting %1 from git repository
    mkdir !BUILD_TOOLS_PATH!\%1
    powershell -Command "Invoke-WebRequest 'http://skobde-siltest3/zip/?r=BuildTools/CMake.git&h=win32/3.5.2&format=zip' -OutFile !BUILD_TOOLS_PATH!/%1/%1.zip"
    call :prepare %1
    @echo I put %1 under !BUILD_TOOLS_PATH!\%1
    
)    
exit /B

:download_mingw72
if exist !BUILD_TOOLS_PATH!\%1 (
    @echo I found your %1 under !BUILD_TOOLS_PATH!\%1
) else (
    @echo %1 not found, getting %1 from git repository
    mkdir !BUILD_TOOLS_PATH!\%1
    powershell -Command "Invoke-WebRequest 'http://skobde-siltest3/zip/?r=BuildTools/MinGW-w64.git&h=7.2.0&format=zip' -OutFile !BUILD_TOOLS_PATH!/%1/%1.zip"
    call :prepare %1
    @echo I put %1 under !BUILD_TOOLS_PATH!\%1
    
)    
exit /B

:download_mingw73
if exist !BUILD_TOOLS_PATH!\%1 (
    @echo I found your %1 under !BUILD_TOOLS_PATH!\%1
) else (
    @echo %1 not found, getting %1 from git repository
    mkdir !BUILD_TOOLS_PATH!\%1
    powershell -Command "Invoke-WebRequest 'http://skobde-siltest3/zip/?r=BuildTools/MinGW-w64.git&h=7.3.0&format=zip' -OutFile !BUILD_TOOLS_PATH!/%1/%1.zip"
    call :prepare %1
    @echo I put %1 under !BUILD_TOOLS_PATH!\%1
    
)    
exit /B

:download_linux
if exist !BUILD_TOOLS_PATH!\%1 (
    @echo I found your %1 under !BUILD_TOOLS_PATH!\%1
) else (
    @echo %1 not found, getting %1 from git repository
    mkdir !BUILD_TOOLS_PATH!\%1
    powershell -Command "Invoke-WebRequest 'http://skobde-siltest3/zip/?r=BuildTools/msys_linux.git&h=i686/4.9.4&format=zip' -OutFile !BUILD_TOOLS_PATH!/%1/%1.zip"
    call :prepare %1
    @echo I put %1 under !BUILD_TOOLS_PATH!\%1
    
)    
exit /B

:download_python
if exist !BUILD_TOOLS_PATH!\%1 (
    @echo I found your %1 under !BUILD_TOOLS_PATH!\%1
) else (
    @echo %1 not found, getting %1 from git repository
    mkdir !BUILD_TOOLS_PATH!\%1
    powershell -Command "Invoke-WebRequest 'http://skobde-siltest3/zip/?r=BuildTools/Python.git&h=3.5.1&format=zip' -OutFile !BUILD_TOOLS_PATH!/%1/%1.zip"
    call :prepare %1
    @echo I put %1 under !BUILD_TOOLS_PATH!\%1
    
)    
exit /B

:prepare
echo Preparing %1
chdir /d !BUILD_TOOLS_PATH!\%1
unzip -q %1.zip
del %1.zip
cd ..
exit /B
