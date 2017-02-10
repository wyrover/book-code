@echo off

setlocal

set ADDITIONAL_KERNEL_FLAGS=
set ADDITIONAL_KERNEL_STACK=
set ADDITIONAL_USERPROVIDER_COMMAND=

:NEXTARG
if "%1" == "/?" goto :USAGE
if "%1" == "-?" goto :USAGE

if "%1" == "-kf" set ADDITIONAL_KERNEL_FLAGS=+%2&shift&shift&goto :NEXTARG
if "%1" == "-ks" set ADDITIONAL_KERNEL_STACK=-stackwalk %2&shift&shift&goto :NEXTARG
if "%1" == "-up" set ADDITIONAL_USERPROVIDER_COMMAND=%2&shift&shift&goto :NEXTARG

REM 
REM Check for unknown arguments and continue
REM
if "%1" NEQ "" goto :USAGE

set XPERF_CMDLINE=xperf.exe -on PROC_THREAD+LOADER%ADDITIONAL_KERNEL_FLAGS% %ADDITIONAL_KERNEL_STACK% -start UserSession -on %ADDITIONAL_USERPROVIDER_COMMAND%

echo.
echo INFO: Invoking xperf to start the kernel and user sessions...
echo CmdLine: ^(%XPERF_CMDLINE%^)
echo.
%XPERF_CMDLINE%

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
echo USAGE: start_user_trace.cmd [options] - start a new xperf trace session with user providers
echo. 
echo Options
echo        -kf - additional kernel mode flags (default is only PROC_THREAD+LOADER)
echo        -ks - additional stackwalk flags
echo        -up - additional user providers
echo.

goto :EOF
