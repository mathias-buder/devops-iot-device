@echo off
SETLOCAL EnableDelayedExpansion

REM Set environment variables
set TOOLCHAIN_HOME_PATH=C:\Users\Nano\.espressif\tools
set IDF_CCACHE_ENABLE=1
set IDF_PATH=C:\Espressif\esp-idf
set IDF_PYTHON_ENV_PATH=%TOOLCHAIN_HOME_PATH%\..\python_env\idf4.0_py3.7_env

set PATH=^
C:\Users\Nano\.espressif\tools\usr\bin;^
C:\Users\Nano\.espressif\tools\opt\xtensa-esp32-elf\bin;^
C:\Users\Nano\.espressif\tools\xtensa-esp32-elf\esp-2019r2-8.2.0\xtensa-esp32-elf\bin;^
C:\Users\Nano\.espressif\tools\esp32ulp-elf\2.28.51.20170517\esp32ulp-elf-binutils\bin;^
C:\Users\Nano\.espressif\tools\cmake\3.13.4\bin;^
C:\Users\Nano\.espressif\tools\mconf\v4.6.0.0-idf-20190628\;^
C:\Users\Nano\.espressif\tools\ninja\1.9.0\;^
C:\Users\Nano\.espressif\tools\idf-exe\1.0.1\;^
C:\Users\Nano\.espressif\tools\ccache\3.7\;^
C:\Users\Nano\.espressif\python_env\idf4.0_py3.7_env\Scripts;^
C:\Espressif\esp-idf\tools;^
C:\Python37;^
C:\Python37\Scripts;

REM Set build tool
set IDF_BUILD_TOOL=idf.py.exe

REM Print current PATH content
echo PATH set to %PATH%

REM Extract build target
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
    REM %IDF_BUILD_TOOL% all size size-components
    %IDF_BUILD_TOOL% all
    goto end


:build_app
    echo [ESP32] Performing build [app] command
    REM %IDF_BUILD_TOOL% all size size-components
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


REM ---------------------------------------------------------------------------------
REM Usage: idf.py.exe [OPTIONS] COMMAND1 [ARGS]... [COMMAND2 [ARGS]...]...
REM ---------------------------------------------------------------------------------

  REM ESP-IDF build management

REM Options:
  REM -C, --project-dir PATH          Project directory.
  REM -B, --build-dir PATH            Build directory.
  REM -n, --no-warnings               Disable Cmake warnings.
  REM -v, --verbose                   Verbose build output.
  REM --ccache / --no-ccache          Use ccache in build. Disabled by default.
  REM -G, --generator [Ninja|MinGW Makefiles]
                                  REM CMake generator.
  REM -D, --define-cache-entry TEXT   Create a cmake cache entry. This option can
                                  REM be used at most once either globally, or for
                                  REM one subcommand.

  REM -b, --baud INTEGER              Baud rate. This option can be used at most
                                  REM once either globally, or for one subcommand.

  REM -p, --port TEXT                 Serial port. This option can be used at most
                                  REM once either globally, or for one subcommand.

  REM --help                          Show this message and exit.

REM Commands:
  REM all                    Aliases: build. Build the project.
  REM app                    Build only the app.
  REM app-flash              Flash the app only.
  REM bootloader             Build only bootloader.
  REM bootloader-flash       Flash bootloader only.
  REM clean                  Delete build output files from the build directory.
  REM confserver             Run JSON configuration server.
  REM efuse_common_table     Genereate C-source for IDF's eFuse fields.
  REM efuse_custom_table     Genereate C-source for user's eFuse fields.
  REM encrypted-app-flash    Flash the encrypted app only.
  REM encrypted-flash        Flash the encrypted project.
  REM erase_flash            Erase entire flash chip.
  REM erase_otadata          Erase otadata partition.
  REM flash                  Flash the project.
  REM fullclean              Delete the entire build directory contents.
  REM menuconfig             Run "menuconfig" project configuration tool.
  REM monitor                Display serial output.
  REM partition_table        Build only partition table.
  REM partition_table-flash  Flash partition table only.
  REM read_otadata           Read otadata partition.
  REM reconfigure            Re-run CMake.
  REM show_efuse_table       Print eFuse table.
  REM size                   Print basic size information about the app.
  REM size-components        Print per-component size information.
  REM size-files             Print per-source-file size information.
