@echo off

setlocal

set TARGET_IMAGE_NAME=
set DISABLE_BREAKONDLLLOAD=

:NEXTARG
if "%1" == "/?" goto :USAGE
if "%1" == "-?" goto :USAGE

if "%1" == "-enable" set TARGET_IMAGE_NAME=%2&shift&shift&goto :NEXTARG
if "%1" == "-disable" set TARGET_IMAGE_NAME=%2&set DISABLE_BREAKONDLLLOAD=true&shift&shift&goto :NEXTARG

REM 
REM Check for unknown arguments and continue
REM
if "%1" NEQ "" goto :USAGE
if not defined TARGET_IMAGE_NAME goto :USAGE

if defined DISABLE_BREAKONDLLLOAD (
    echo.
    echo INFO: Disabling "break on DLL load" for image "%TARGET_IMAGE_NAME%"
    echo.
    reg.exe delete "HKLM\Software\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\%TARGET_IMAGE_NAME%" /v BreakOnDllLoad /f
    reg.exe delete "HKLM\Software\Wow6432Node\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\%TARGET_IMAGE_NAME%" /v BreakOnDllLoad /f
    goto :EXIT
)

echo.
echo INFO: Enabling "break on DLL load" for image "%TARGET_IMAGE_NAME%"
echo.
reg.exe add "HKLM\Software\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\%TARGET_IMAGE_NAME%" /v BreakOnDllLoad /t REG_DWORD /d 1 /f
reg.exe add "HKLM\Software\Wow6432Node\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\%TARGET_IMAGE_NAME%" /v BreakOnDllLoad /t REG_DWORD /d 1 /f

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
echo USAGE: configure_breakondllload.cmd [options] [ImageName] - set break on DLL load IFEO
echo. 
echo Options
echo        -enable ImageName
echo        -disable ImageName
echo.

goto :EOF
