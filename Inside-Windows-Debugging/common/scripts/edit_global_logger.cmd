@echo off

setlocal

set ADDITIONAL_KERNEL_FLAGS=
set ADDITIONAL_KERNEL_STACK=

:NEXTARG
if "%1" == "/?" goto :USAGE
if "%1" == "-?" goto :USAGE

if "%1" == "-disable" set DISABLE_GLOBAL_LOGGER=true&shift&goto :NEXTARG
if "%1" == "-kf" set ADDITIONAL_KERNEL_FLAGS=+%2&shift&shift&goto :NEXTARG
if "%1" == "-ks" set ADDITIONAL_KERNEL_STACK=-stackwalk %2&shift&shift&goto :NEXTARG

REM 
REM Check for unknown arguments and continue
REM
if "%1" NEQ "" goto :USAGE

if defined DISABLE_GLOBAL_LOGGER (
    echo.
    echo INFO: Deleting global logger registry key...
    echo.
    reg.exe delete HKLM\SYSTEM\CurrentControlSet\Control\WMI\GlobalLogger /f
    goto :EXIT
)

set XPERF_CMDLINE=xperf.exe -boottrace PROC_THREAD+LOADER%ADDITIONAL_KERNEL_FLAGS% %ADDITIONAL_KERNEL_STACK%

echo.
echo INFO: Invoking xperf to edit the global logger settings...
echo CmdLine: ^(%XPERF_CMDLINE%^)
echo.
%XPERF_CMDLINE%

:EXIT
if %ERRORLEVEL% NEQ 0 (
    echo. 
    echo FAILED!
    echo.
) else (
    echo. 
    echo SUCCESS. 
    echo.
)

goto :EOF

:USAGE
echo.
echo USAGE: edit_global_logger.cmd [options] - edit kernel boot trace settings
echo. 
echo Options
echo        -kf - additional kernel mode flags (default is only PROC_THREAD+LOADER)
echo        -ks - additional stackwalk flags
echo        -disable
echo.

goto :EOF
