@echo off
setlocal EnableDelayedExpansion


:: Set environment variables (modify if required)

:: Set IDF_TOOLS_PATH as global environemnt variable or use varieble %userprofile%
set IDF_TOOLS_PATH=%userprofile%\.espressif

set PERL_PATH=C:\Strawberry\perl
set GIT_PATH=C:\Program Files\Git
set IDF_PATH=%userprofile%\Repositories\esp-idf
set IDF_PYTHON_ENV_PATH=%IDF_TOOLS_PATH%\python_env\idf4.0_py3.8_env

:: Don't modify
set IDF_CCACHE_ENABLE=1

set PATH=^
%IDF_TOOLS_PATH%\tools\xtensa-esp32-elf\esp-2019r2-8.2.0\xtensa-esp32-elf\bin;^
%IDF_TOOLS_PATH%\tools\esp32ulp-elf\2.28.51.20170517\esp32ulp-elf-binutils\bin;^
%IDF_TOOLS_PATH%\tools\cmake\3.13.4\bin;^
%IDF_TOOLS_PATH%\tools\mconf\v4.6.0.0-idf-20190628\;^
%IDF_TOOLS_PATH%\tools\ninja\1.9.0\;^
%IDF_TOOLS_PATH%\tools\idf-exe\1.0.1\;^
%IDF_TOOLS_PATH%\tools\ccache\3.7\;^
%IDF_PYTHON_ENV_PATH%\Scripts;^
%IDF_PATH%\tools;^
%PERL_PATH%\..\c\bin;^
%PERL_PATH%\site\bin;^
%PERL_PATH%\bin;^
%GIT_PATH%\cmd;


:: Set build tool
set IDF_BUILD_TOOL=idf.py.exe

:: Print current PATH content
echo PATH set to %PATH%

:: Extract build target
set PARAMS=%*
set PARAMS_WITHOUT_ALL="%PARAMS:all=%"
set PARAMS_WITHOUT_APP="%PARAMS:app=%"
set PARAMS_WITHOUT_CLEAN="%PARAMS:clean=%"

if not %PARAMS_WITHOUT_CLEAN% == "%PARAMS%" ( goto clean     )
if not %PARAMS_WITHOUT_ALL%   == "%PARAMS%" ( goto build_all )
if not %PARAMS_WITHOUT_APP%   == "%PARAMS%" ( goto build_app )

goto default


:default
    echo Usage: build.cmd [-jN|all^|app^|clean]
    goto build_all


:build_all
    echo [ESP32] Performing build [all] command
    :: %IDF_BUILD_TOOL% all size size-components
    %IDF_BUILD_TOOL% all
    goto end


:build_app
    echo [ESP32] Performing build [app] command
    :: %IDF_BUILD_TOOL% all size size-components
    %IDF_BUILD_TOOL% app
    goto end


:clean
    echo [ESP32] Performing clean command
    %IDF_BUILD_TOOL% clean
    goto end


:fullclean
    echo [ESP32] Performing fullclean command
    %IDF_BUILD_TOOL% fullclean
    rmdir /s /q build
    goto end


:end


:: ---------------------------------------------------------------------------------
:: Usage: idf.py.exe [OPTIONS] COMMAND1 [ARGS]... [COMMAND2 [ARGS]...]...
:: ---------------------------------------------------------------------------------

  :: ESP-IDF build management

:: Options:
  :: -C, --project-dir PATH          Project directory.
  :: -B, --build-dir PATH            Build directory.
  :: -n, --no-warnings               Disable Cmake warnings.
  :: -v, --verbose                   Verbose build output.
  :: --ccache / --no-ccache          Use ccache in build. Disabled by default.
  :: -G, --generator [Ninja|MinGW Makefiles]
                                  :: CMake generator.
  :: -D, --define-cache-entry TEXT   Create a cmake cache entry. This option can
                                  :: be used at most once either globally, or for
                                  :: one subcommand.

  :: -b, --baud INTEGER              Baud rate. This option can be used at most
                                  :: once either globally, or for one subcommand.

  :: -p, --port TEXT                 Serial port. This option can be used at most
                                  :: once either globally, or for one subcommand.

  :: --help                          Show this message and exit.

:: Commands:
  :: all                    Aliases: build. Build the project.
  :: app                    Build only the app.
  :: app-flash              Flash the app only.
  :: bootloader             Build only bootloader.
  :: bootloader-flash       Flash bootloader only.
  :: clean                  Delete build output files from the build directory.
  :: confserver             Run JSON configuration server.
  :: efuse_common_table     Genereate C-source for IDF's eFuse fields.
  :: efuse_custom_table     Genereate C-source for user's eFuse fields.
  :: encrypted-app-flash    Flash the encrypted app only.
  :: encrypted-flash        Flash the encrypted project.
  :: erase_flash            Erase entire flash chip.
  :: erase_otadata          Erase otadata partition.
  :: flash                  Flash the project.
  :: fullclean              Delete the entire build directory contents.
  :: menuconfig             Run "menuconfig" project configuration tool.
  :: monitor                Display serial output.
  :: partition_table        Build only partition table.
  :: partition_table-flash  Flash partition table only.
  :: read_otadata           Read otadata partition.
  :: reconfigure            Re-run CMake.
  :: show_efuse_table       Print eFuse table.
  :: size                   Print basic size information about the app.
  :: size-components        Print per-component size information.
  :: size-files             Print per-source-file size information.
