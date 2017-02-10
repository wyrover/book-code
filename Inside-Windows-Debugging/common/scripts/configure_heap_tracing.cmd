@echo off

setlocal

set TARGET_IMAGE_NAME=
set DISABLE_TRACING=

:NEXTARG
if "%1" == "/?" goto :USAGE
if "%1" == "-?" goto :USAGE

if "%1" == "-enable" set TARGET_IMAGE_NAME=%2&shift&shift&goto :NEXTARG
if "%1" == "-disable" set TARGET_IMAGE_NAME=%2&set DISABLE_TRACING=true&shift&shift&goto :NEXTARG

REM 
REM Check for unknown arguments and continue
REM
if "%1" NEQ "" goto :USAGE
if not defined TARGET_IMAGE_NAME goto :USAGE

if defined DISABLE_TRACING (
    echo.
    echo INFO: Disabling heap tracing for image "%TARGET_IMAGE_NAME%"
    echo.
    reg.exe delete "HKLM\Software\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\%TARGET_IMAGE_NAME%" /v TracingFlags /f
    reg.exe delete "HKLM\Software\Wow6432Node\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\%TARGET_IMAGE_NAME%" /v TracingFlags /f
    goto :EXIT
)

echo.
echo INFO: Enabling heap tracing for image "%TARGET_IMAGE_NAME%"
echo.
reg.exe add "HKLM\Software\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\%TARGET_IMAGE_NAME%" /v TracingFlags /t REG_DWORD /d 1 /f
reg.exe add "HKLM\Software\Wow6432Node\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\%TARGET_IMAGE_NAME%" /v TracingFlags /t REG_DWORD /d 1 /f

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
echo USAGE: configure_heap_tracing.cmd [options] [ImageName] - set heap tracing IFEO
echo. 
echo Options
echo        -enable ImageName
echo        -disable ImageName
echo.

goto :EOF
